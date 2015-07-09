
#include <sstream>
#include "TH1F.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TFile.h"
#include "TSystem.h"
#include "R00TLeSettings.hh"
#include "TH2F.h"
#include "TList.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include <fstream>

#include <iostream>
#include <vector>

using namespace std;

double gErrorInCompton=0.03;
map<TString,TGraphErrors*> theGraphs;//Global Container to hold the resulting Graphs of KeV_ee vs ADC

Double_t Na1 = 341;
Double_t Na2 = 1062;

Double_t Am1 = 26.3;
Double_t Am2 = 59.5;

Double_t Cs1 = 33;
Double_t Cs2 = 478;

Double_t gY[]={Na1,Na2,Am1,Am2,Cs1,Cs2};

//Order of points 
//Na1 Na2 Am1 Am2 Cs1 Cs2


Int_t ReBinFactor=4;

TH1D * GetRightTimeProjection(TH2F* tempHisto){

  TH1D * tempProjection = tempHisto->ProjectionY();
  
  //Find the maximum point IE The time when the Source was
  //in front of this channel
  
  int window = 15;//seconds
  
    
  int maxBin =tempProjection->GetMaximumBin();

  double maxBinCenter = tempProjection->GetBinCenter(maxBin);

  Int_t lowBin = tempProjection->FindBin(maxBinCenter-window);
  Int_t highBin = tempProjection->FindBin(maxBinCenter+window);
    
  delete tempProjection;
  stringstream name;
  name<<tempHisto->GetName()<<"_Projection";
  TH1D * theProjection = tempHisto->ProjectionX(name.str().c_str(),lowBin,highBin);
  //TH1D * theProjection = tempHisto->ProjectionX(name.str().c_str(),0,-1);

  return theProjection;
}




TH1D * BoxCarSmooth(TH1D* theHisto,int HalfWindow=6){
  
  if (theHisto->GetEntries()==0){
    return theHisto;
  }else{
    
    int NumBins = theHisto->GetNbinsX();
  
    TH1D * theSmoothHisto = (TH1D*) theHisto->Clone(theHisto->GetName()+TString("_Smooth"));

    //halfWindow =1 take {i-1,i,i+1}
    //halfWindow =2 take {i-2,i-i,i,i+1,i+2}

    //start at the 2nd bin
    //end at the bin befor the last
    for (int i=HalfWindow+1;i<NumBins;i++){
    
      double val=0;
      for (int j=0;j<2*HalfWindow+1;j++){
	val += theHisto->GetBinContent( (i-HalfWindow)+j); 
      }
      double size = 2*HalfWindow + 1;
      theSmoothHisto->SetBinContent(i,val/size);
    }
    return theSmoothHisto;

  }
}

TH1D * Derivative(TH1D *theHisto){

  int NumBins = theHisto->GetNbinsX();
  TH1D * theDerHisto = (TH1D*) theHisto->Clone(theHisto->GetName()+TString("_Der"));

  //start at the 2nd bin
  //end at the bin befor the last
  for (int i=2;i<NumBins;i++){
    theDerHisto->SetBinContent(i,theHisto->GetBinContent(i)-theHisto->GetBinContent(i-1));
  }
  return theDerHisto;


}


vector<Double_t> FindPhotoPeak(TH1D* theHisto,Double_t MaxPoint,Double_t searchWindow,TString Name){

  if (theHisto->GetEntries() == 0){
    cout<<"Warning from FindPhotoPeak: No entries in histogram "<<theHisto->GetName()<<endl;
    vector<Double_t> d;
    return d;
  }
  
  Int_t numberOfTriesMinusOne = 8;
  
  Double_t StepSize = 0.03*searchWindow;
  //cout<<Name<<" searchWindow "<<searchWindow<<" maxPoint "<<MaxPoint<<"   "<<StepSize<<endl;



  //Make the list of fit functions with different ranges 
  
  map<Double_t,TF1*> Chi2ToFunction;

  
  for (int i=0;i<numberOfTriesMinusOne+1;i++){
    stringstream func_Name;
    func_Name<<"_Temp_Func_"<<i;
    Double_t offSet = (i+(numberOfTriesMinusOne)/2.0) - numberOfTriesMinusOne;
    offSet = offSet*StepSize;

    // cout<<Name<<" "<<MaxPoint -searchWindow + offSet<< " "<<MaxPoint+searchWindow +offSet<<"     "<<endl;
    //TF1* TEMP = new TF1(func_Name.str().c_str(),"gaus",MaxPoint -searchWindow + offSet,MaxPoint+searchWindow +offSet);
    TF1* TEMP = new TF1(func_Name.str().c_str(),"gaus",MaxPoint -searchWindow + offSet,MaxPoint+searchWindow +offSet);
    // TEMP->SetParLimits(0,0,10000);
    // TEMP->SetParLimits(1,0,10000);
    // TEMP->SetParLimits(2,0,10000);


    TFitResultPtr result = theHisto->Fit(func_Name.str().c_str(),"RQSN");
    Int_t status = result;
    if (status==0){
      if ( result->Ndf() !=0){
	Double_t Chi2 = result->Chi2()/result->Ndf();
	//	cout<<Chi2<<endl;
	Chi2ToFunction[TMath::Abs(Chi2-1)]=TEMP;
      }
    }
  }
  // cin.get();

  
  //Remove References to the functions in the GLOBAL LIST
  
  for (int i=0;i<numberOfTriesMinusOne+1;i++){
    stringstream func_Name;
    func_Name<<"_Temp_Func_"<<i;
    gROOT->GetListOfFunctions()->RecursiveRemove(gROOT->GetListOfFunctions()->FindObject(func_Name.str().c_str()));
  }

  vector<Double_t> returnValues;

  if (Chi2ToFunction.size()> 0){  
    Chi2ToFunction.begin()->second->SetName(Name);
    theHisto->GetListOfFunctions()->Add(Chi2ToFunction.begin()->second);
    
    returnValues.push_back(Chi2ToFunction.begin()->second->GetParameter(1));
    returnValues.push_back(Chi2ToFunction.begin()->second->GetParameter(2));
    
    return returnValues;
  }else {
    cout<<"Warning Find Photo Peak Failed on "<<theHisto->GetName()<<" "<<Name<<endl;
    return returnValues;
  }
}







Double_t FindComptonEdge(TH1D* theHisto,Double_t start,Double_t end,TString Name){
  //Look for 

  TH1D * theSmoothHisto = BoxCarSmooth(theHisto);
  
  Int_t StartBin = theHisto->FindBin(start);
  Int_t EndBin = theHisto->FindBin(end);

  //Look for the top of the coumpton edge in the smoothed
  //histogram
 
  //First find the min before the compton edge
  double theMinimum=1000000;
  Int_t theBin=-1;
  Int_t numLowerInRow=0;
  Double_t previousValue=0;
  for (int i=StartBin;i<EndBin;i++){
    double binVal = theSmoothHisto->GetBinContent(i);
    
    if (previousValue > binVal){
      numLowerInRow++;
    }else{
      numLowerInRow=0;
    }

    if (numLowerInRow > 80){
      break;
    }

    if(binVal <theMinimum){//This could be a min

      int NumLess=0;
      for (int j=i;j<i+6;j++){
	if (theSmoothHisto->GetBinContent(j)<binVal){
	  NumLess++;
	}
      }//end for
      //if at least 3 of the those 6 points are lower then
      //this point this is not the min we seek

      //the minimum should have nothing lowe then it next to it
      if (NumLess == 0){

	theMinimum=binVal;
	theBin=i;
	break;
      }
    }
    previousValue=binVal;
  }//end for over start/end bin

  //Now we have found the minimum spot before the coupton edge
  //look for the peak of the coumpton edge
  Double_t minBeforeCouptonEdge = theSmoothHisto->GetBinCenter(theBin);

  Int_t maxBin=-1;
  Double_t MaxValue=-1;
  for (int i=theBin;i<EndBin;i++){
    double binVal = theSmoothHisto->GetBinContent(i);
    if (binVal > MaxValue){
      MaxValue=binVal;
      maxBin=i;
    }
  }

  //find the 93% of the max bin and the 15% of the max

  Int_t Max90Bin = -1;
  Int_t Max10Bin = -1;

  for (int i=maxBin;i<EndBin;i++){

    double binVal = theSmoothHisto->GetBinContent(i);
    
    //    find the last bin that is above 93% of maximum
    if (binVal>= 0.93*MaxValue){
      Max90Bin=i;
    }
    if (binVal>=0.15*MaxValue){
      Max10Bin=i;
    }
  }

  // cout<<"Final range is bin "<<Max90Bin<<" to "<<Max10Bin<<endl;
  // cout<<"Values "<< theSmoothHisto->GetBinCenter(Max90Bin) <<" "<< theSmoothHisto->GetBinCenter(Max10Bin) << endl;

  Double_t xLow =theSmoothHisto->GetBinCenter(Max90Bin);
  Double_t xHigh =theSmoothHisto->GetBinCenter(Max10Bin);
  

  if (theHisto->GetEntries() != 0){
    //Make a fitting function
    TF1 *myFunc = new TF1("myFunc","pol2",xLow,xHigh);

    TFitResultPtr result =theHisto->Fit("myFunc","RSQN");
    myFunc->SetName(Name);
    theHisto->GetListOfFunctions()->Add((TF1*)gROOT->GetListOfFunctions()->FindObject(Name));
    

    //    theHisto->GetListOfFunctions()->Print();


    Int_t status = result;
    TF1 *myNewFunc = new TF1("myNewFunc","pol2-(2.0/3.0)*[3]",xLow,xHigh);

    if (status ==0 ){//Good fit
      //look for 2/3 maximum spot
  
      myNewFunc->SetParameter(3,MaxValue);
      myNewFunc->SetParameter(0,result->Value(0));
      myNewFunc->SetParameter(1,result->Value(1));
      myNewFunc->SetParameter(2,result->Value(2));

      
    }else{
      cout<<"Fit Failed in Find Coupton Edge"<<endl;
    }
    
    delete theSmoothHisto;
    Double_t ret =myNewFunc->GetX(0,xLow,xHigh);
    delete myNewFunc;
    return ret;
  }else {
    return 0;
  }
}


void CalibrateNAData(TH1D* theProjectionTop,TH1D* theProjectionBottom, TString BarName){

  //Order of points 
  //Na1 Na2 Am1 Am2 Cs1 Cs2

  Double_t x[2];
  
  x[0]=FindComptonEdge(theProjectionTop,200,500,BarName+"T_Nafunc1");
  x[1]=FindComptonEdge(theProjectionTop,900,1400,BarName+"T_Nafunc2");
  if ( x[0]!=0 && x[1]!=0){//if either are zero then there was a failed fit.  Don't do that one
    if (theGraphs.count(BarName+"T") !=0 ){

      theGraphs[BarName+"T"]->SetPoint(0,x[0],gY[0]);
      theGraphs[BarName+"T"]->SetPointError(0,gErrorInCompton*x[0],0);

      theGraphs[BarName+"T"]->SetPoint(1,x[1],gY[1]);
      theGraphs[BarName+"T"]->SetPointError(1,gErrorInCompton*x[1],0);
    }else{
      cout<<"CRAP "<<endl;
    }
  }

  x[0]=FindComptonEdge(theProjectionBottom,200,500,BarName+"B_Nafunc1");
  x[1]=FindComptonEdge(theProjectionBottom,900,1400,BarName+"B_Nafunc2");
  if ( x[0]!=0 && x[1]!=0){//if either are zero then there was a failed fit.  Don't do that one
    if (theGraphs.count(BarName+"B") !=0 ){

      theGraphs[BarName+"B"]->SetPoint(0,x[0],gY[0]);
      theGraphs[BarName+"B"]->SetPointError(0,gErrorInCompton*x[0],0);

      theGraphs[BarName+"B"]->SetPoint(1,x[1],gY[1]);
      theGraphs[BarName+"B"]->SetPointError(1,gErrorInCompton*x[1],0);
    }else{
      cout<<"CRAP "<<endl;
    }
  }

}
Int_t FindNextMin(TH1D* theHisto,int currentBin){

  int nBins=theHisto->GetNbinsX();

  Double_t min=9999999999.0;
  
  Int_t minIndex=-1;
  
  for (int i=currentBin;i<nBins;i++){
    Double_t val = theHisto->GetBinContent(i);

    if (val< min){
      min=val;
      //A possible min
      //Check next few bins to see if goes back up
      int count=0;
      for (int j=i+1;j<i+4;j++){
	if (theHisto->GetBinContent(j)>= val){
	  //if the bin is larger then the possible min
	  count++;
	}
      }
      if (count==3){ //if 3 of the 3 bins to the right are larger
	//this is the one we want
	min=val;
	minIndex=i;
	break;
      }//end if count>1
    } //end if val<min
  }//end for of i

  return minIndex;
}

Int_t FindNextMax(TH1D* theHisto,int currentBin){

  if(theHisto->GetEntries() == 0 ){
    return -1;
  }

  int nBins=theHisto->GetNbinsX();

  Double_t max=60;
  Int_t maxIndex=-1;

  for (int i=currentBin;i<nBins;i++){
    Double_t val = theHisto->GetBinContent(i);
 
    if (val> max){
      max=val;

      //A possible max
      //Check next few bins to see if goes down
      int count=0;

      for (int j=i+1;j<i+4;j++){
	if (theHisto->GetBinContent(j)<= val){
	  //if the bin is less then the possible max
	  count++;
	}
      }

      if (count==3){ //if 3 of the 3 bins to the right are larger
	//this is the one we want
	max=val;
	maxIndex=i;
	break;
      }//end if count>
    } //end if val>max
  }//end for of i

  
  if (maxIndex == -1 ){///SOmething failed
    cout<<"Failed to find Next Max for histo "<<theHisto->GetName()<<endl;
    cout<<"Start bin was "<<currentBin<<endl;
  }


  return maxIndex;
}




void CalibrateAMData(TH1D* theProjectionTop,TH1D* theProjectionBottom, TString BarName){

  //Order of points 
  //Na1 Na2 Am1 Am2 Cs1 Cs2

  TH1D* topSmooth = BoxCarSmooth(theProjectionTop);
  TH1D* bottomSmooth = BoxCarSmooth(theProjectionBottom);



  Int_t firstTopMaxIndex = theProjectionTop->GetMaximumBin();//FindNextMax(topSmooth,0);
  Int_t temp = FindNextMin(theProjectionTop,firstTopMaxIndex)+3;
  Int_t secondTopMaxIndex=2*FindNextMax((TH1D*)theProjectionTop->Rebin(2,BarName+"TRebinAm"),temp/2);
  
  Int_t firstBottomMaxIndex =theProjectionBottom->GetMaximumBin();//FindNextMax(bottomSmooth,0);
  temp = FindNextMin(theProjectionBottom,firstBottomMaxIndex)+3;

  Int_t secondBottomMaxIndex=2*FindNextMax((TH1D*)theProjectionBottom->Rebin(2,BarName+"BRebinAm"),temp/2);

  cout<<"Min spot is "<<temp<<" "<<theProjectionTop->GetBinCenter(temp)<<endl;


  Double_t binCenterFirstTop = theProjectionTop->GetBinCenter(firstTopMaxIndex);
  Double_t binCenterSecondTop = theProjectionTop->GetBinCenter(secondTopMaxIndex);

  Double_t binCenterFirstBottom = theProjectionBottom->GetBinCenter(firstBottomMaxIndex);
  Double_t binCenterSecondBottom= theProjectionBottom->GetBinCenter(secondBottomMaxIndex);

  Int_t AmWindow1=5;
  Int_t AmWindow2=10;

  //FindPhotoPeak returns length 2 array.  result[0] = mean of gaussian result[1] =sigma of guassian

  if (theProjectionTop->GetEntries()!=0){
    if (binCenterFirstTop > 0){
      vector<Double_t> tempVal = FindPhotoPeak(theProjectionTop,binCenterFirstTop,AmWindow1,BarName+"T_AmFirstFunc");
      if (tempVal.size()!=0){
	theGraphs[BarName+"T"]->SetPoint(2,tempVal[0],gY[2]);
	theGraphs[BarName+"T"]->SetPointError(2,tempVal[1],0);
      }
    }
    if (binCenterSecondTop >0 ){
      vector<Double_t> tempVal = FindPhotoPeak(theProjectionTop,binCenterSecondTop,AmWindow2,BarName+"T_AmSecondFunc");
      if (tempVal.size() != 0){
	theGraphs[BarName+"T"]->SetPoint(3,tempVal[0],gY[3]);
	theGraphs[BarName+"T"]->SetPointError(3,tempVal[1],0);
      }
    }
  }


  if (theProjectionBottom->GetEntries()!=0){
    if (binCenterFirstBottom > 0 ){
      vector<Double_t> tempVal = FindPhotoPeak(theProjectionBottom,binCenterFirstBottom,AmWindow1,BarName+"B_AmFirstFunc");
      if (tempVal.size() !=0){
	theGraphs[BarName+"B"]->SetPoint(2,tempVal[0],gY[2]);
	theGraphs[BarName+"B"]->SetPointError(2,tempVal[1],0);
      }
    }
    if (binCenterSecondTop > 0 ){
      vector<Double_t> tempVal = FindPhotoPeak(theProjectionBottom,binCenterSecondBottom,AmWindow2,BarName+"B_AmSecondFunc");
      if (tempVal.size() !=0){
	theGraphs[BarName+"B"]->SetPoint(3,tempVal[0],gY[3]);
	theGraphs[BarName+"B"]->SetPointError(3,tempVal[1],0);
      }
    }
  }



  //   TF1 *FuncTopFirstMax = new TF1("FuncTopFirstMax","gaus",binCenterFirstTop-AmWindow1,binCenterFirstTop+AmWindow1);
  //   TFitResultPtr result = theProjectionTop->Fit("FuncTopFirstMax","RSQN");
  //   Int_t status = result;
  //   if (status==0){
  //     theGraphs[BarName+"T"]->SetPoint(2,result->Value(1),gY[2]);
  //     theGraphs[BarName+"T"]->SetPointError(2,0.05*result->Value(1),0);
  //     FuncTopFirstMax->SetName(BarName+"T_AmFirstFunc");
  //     theProjectionTop->GetListOfFunctions()->Add((TF1*)gROOT->GetListOfFunctions()->FindObject(BarName+"T_AmFirstFunc"));
  //   }
    

  //   TF1 *FuncTopSecondMax = new TF1("FuncTopSecondMax","gaus",binCenterSecondTop-AmWindow2,binCenterSecondTop+AmWindow2);
  //   result = theProjectionTop->Fit("FuncTopSecondMax","RSQN");

  //   status = result;
  //   if (status==0){

  //     theGraphs[BarName+"T"]->SetPoint(3,result->Value(1),gY[3]);
  //     theGraphs[BarName+"T"]->SetPointError(3,0.05*result->Value(1),0);
  //     FuncTopSecondMax->SetName(BarName+"T_AmSecondFunc");
  //     theProjectionTop->GetListOfFunctions()->Add((TF1*)gROOT->GetListOfFunctions()->FindObject(BarName+"T_AmSecondFunc"));

  //   }
    
  // }
  // ////////////////////////////////////////////////////////////////////////////
  // ////////////////////////////////////////////////////////////////////////////
  // ///////////////////////////Bottom stuf now//////////////////////////////////
  // ////////////////////////////////////////////////////////////////////////////
  // ////////////////////////////////////////////////////////////////////////////

  // if (theProjectionBottom->GetEntries()!=0){
  //   TF1 *FuncBottomFirstMax = new TF1("FuncBottomFirstMax","gaus",binCenterFirstBottom-AmWindow1,binCenterFirstBottom+AmWindow1);
  //   TFitResultPtr result = theProjectionBottom->Fit("FuncBottomFirstMax","RSQN");


    
  //   Int_t status = result;
  //   if (status==0){
  //     theGraphs[BarName+"B"]->SetPoint(2,result->Value(1),gY[2]);
  //     theGraphs[BarName+"B"]->SetPointError(2,0.05*result->Value(1),0);
  //     FuncBottomFirstMax->SetName(BarName+"B_AmFirstFunc");
  //     theProjectionBottom->GetListOfFunctions()->Add((TF1*)gROOT->GetListOfFunctions()->FindObject(BarName+"B_AmFirstFunc"));
  //   }


  //   TF1 *FuncBottomSecondMax = new TF1("FuncBottomSecondMax","gaus",binCenterSecondBottom-AmWindow2,binCenterSecondBottom+AmWindow2);
  //   result = theProjectionBottom->Fit("FuncBottomSecondMax","RSQN");
    
  //   status=result;
  //   if (status==0){
  //     theGraphs[BarName+"B"]->SetPoint(3,result->Value(1),gY[3]);
  //     theGraphs[BarName+"B"]->SetPointError(3,0.05*result->Value(1),0);
  //     FuncBottomSecondMax->SetName(BarName+"B_AmSecondFunc");
  //     theProjectionBottom->GetListOfFunctions()->Add((TF1*)gROOT->GetListOfFunctions()->FindObject(BarName+"B_AmSecondFunc"));

  //   }

  // }



  
}


void CalibrateCSData(TH1D* theProjectionTop,TH1D* theProjectionBottom, TString BarName){

  //Order of points 
  //Na1 Na2 Am1 Am2 Cs1 Cs2


  int SmoothFudge=0;

  int PhotoPeakWindow=300;

  /////////////////TOP CHANNEL///////////////////////////////
  if (theProjectionTop->GetEntries()!=0){
    Int_t TopMaxIndex = ReBinFactor*FindNextMax((TH1D*)theProjectionTop->Rebin(ReBinFactor,BarName+"_Trebinned"),0);//BoxCarSmooth(theProjectionTop,2),0);
    Double_t binCenterTop = theProjectionTop->GetBinCenter(TopMaxIndex-SmoothFudge);
    if (binCenterTop>0){
      vector<Double_t> temp = FindPhotoPeak(theProjectionTop,binCenterTop,PhotoPeakWindow,BarName+"T_CsFirstFunc");
      if (temp.size() !=0){
	theGraphs[BarName+"T"]->SetPoint(4,temp[0],gY[4]);
	theGraphs[BarName+"T"]->SetPointError(4,temp[1],0);
      }
    }
  }

  /////////////////Bottom CHANNEL///////////////////////////////
  if (theProjectionBottom->GetEntries()!=0){
    Int_t BottomMaxIndex = ReBinFactor*FindNextMax((TH1D*)theProjectionBottom->Rebin(ReBinFactor,BarName+"_Brebinned"),0);
    Double_t binCenterBottom = theProjectionBottom->GetBinCenter(BottomMaxIndex-SmoothFudge);

    if (binCenterBottom>0){
      vector<Double_t> temp = FindPhotoPeak(theProjectionBottom,binCenterBottom,PhotoPeakWindow,BarName+"B_CsFirstFunc");
      if (temp.size()!=0){
	theGraphs[BarName+"B"]->SetPoint(4,temp[0],gY[4]);
	theGraphs[BarName+"B"]->SetPointError(4,temp[1],0);
      }
    }
  }

  
  Double_t edgeT = FindComptonEdge(theProjectionTop,5000,20000,BarName+"T_CsFunc2");
  Double_t edgeB = FindComptonEdge(theProjectionBottom,5000,20000,BarName+"B_CsFunc2");

  theGraphs[BarName+"T"]->SetPoint(5,edgeT,gY[5]);
  theGraphs[BarName+"T"]->SetPointError(5,gErrorInCompton*edgeT,0);


  theGraphs[BarName+"B"]->SetPoint(5,edgeB,gY[5]);
  theGraphs[BarName+"B"]->SetPointError(5,gErrorInCompton*edgeB,0);

}



void BuildListOfTGraphs(R00TLeSettings * theSettings){
  //The Graphs will be stored in the order they appear in the
  //Settings object. Top then bottom
  
  Int_t numBars = theSettings->GetNumBars();
  cout<<"There are "<<numBars<<" to Calibrate "<<endl;


  for (int i=0;i<numBars;i++){
    TString FullName =theSettings->GetBarName(i)+TString("T");
    theGraphs[FullName]= new TGraphErrors();
    theGraphs[FullName]->SetName(FullName);


    FullName =theSettings->GetBarName(i)+TString("B");
    theGraphs[FullName]= new TGraphErrors();
    theGraphs[FullName]->SetName(FullName);
  }
  


}













































#include "InputManager.hh"






//
//  the main here should be a comment block please add
//


TFile * OpenFile(int runNumber){

  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString rootfilesdirname = install+"/users/"+user+"/histograms";
  stringstream ss;
  ss<<runNumber;
  TString file = rootfilesdirname + "/HistogramsFromRun"+ss.str().c_str()+".root";


  
  return new TFile(file,"read");
  
}

TFile * OpenFileUpdated(int runNumber){

  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString rootfilesdirname = install+"/users/"+user+"/histograms";
  stringstream ss;
  ss<<runNumber;
  TString file = rootfilesdirname + "/HistogramsFromRun"+ss.str().c_str()+"updated"+".root";


  
  return new TFile(file,"recreate");
  
}



//Order of run numbers should be na am cs
int main(int argc,char**argv){
  TString install =gSystem->Getenv("R00TLeInstall");
  
  TString flagFile = install+"/src/Calibration/FlagFile.txt";
  InputManager InputMan(flagFile.Data());
  if ( !InputMan.LoadInputs(argc,argv) ){
    return -1;
  }


  int runNumber=-1;

  int runNumberNa = InputMan.GetIntValue("narun");
  int runNumberAm = InputMan.GetIntValue("amrun");
  int runNumberCs = InputMan.GetIntValue("csrun");
  


  

  
  ifstream InCorrectionsFile;
  stringstream ss1;
  ss1<<install<<"/prm/Corrections"<<runNumber<<".txt";

  InCorrectionsFile.open(ss1.str().c_str());
  if (!InCorrectionsFile.is_open()){
    cout<<"No correction file for run "<<runNumber<<endl;
    cout<<"Looking For Defualt"<<endl;
    ss1.str("");
    ss1<<install<<"/prm/Corrections.txt";
    InCorrectionsFile.open(ss1.str().c_str());
    if (!InCorrectionsFile.is_open()){
      cout<<"No Defualt Corrections file found.\n Exiting.."<<endl;
      return -1;
    }else{
      cout<<"Found Defualt Corrections File"<<endl;
    }
  }else {
    cout<<"Found Corrections File for run "<<runNumber<<endl;
  }

  map<string,Double_t> timingOffsetsFromFile;
  string NameInFile;
  Double_t trash,T_Offset;
  // while (true){
  //   InCorrectionsFile>>NameInFile>>trash>>trash>>T_Offset;
  //   if (InCorrectionsFile.eof()){
  //     break;
  //   }
  //   timingOffsetsFromFile[NameInFile]=T_Offset;
  // }




  TFile * theFile_Temp;
  if (runNumberCs != -1){
    theFile_Temp=  OpenFile(runNumberCs);
  }else if (runNumberNa!=-1){
    theFile_Temp=  OpenFile(runNumberNa);
  }else if (runNumberAm !=-1){
    theFile_Temp=  OpenFile(runNumberAm);
  }else {
    cout<<"All bad run numbers"<<endl;
    return -1;
  }



  ////////////////////////////////////////////////
  // Look for a R00TLeSettings Object in        //
  // in the file 	       		        //
  ////////////////////////////////////////////////

  R00TLeSettings * theSettings;
  if ( gDirectory->Get("Settings0") != NULL){
    theSettings = (R00TLeSettings*)gDirectory->Get("Settings0");
  }else {
    cout<<"Could not find settings object in "<<runNumberCs<<endl;
  }

  
  stringstream name;

  Int_t numBars = theSettings->GetNumBars();  

  BuildListOfTGraphs(theSettings);
  theFile_Temp->Close();
  delete theFile_Temp;



  if (runNumberCs != -1){

    TFile * theFile_Cs =  OpenFile(runNumberCs);
    TFile * theFile_Cs_Updated = OpenFileUpdated(runNumberCs);

    cout<<"Analyzing Cs Data"<<endl;
    for ( int i=0;i<numBars;i++){

      ////////////////////////////////////////////////
      ////////////////////////////////////////////////
      //Get the Bottom Energy vs time plot///////////
      theFile_Cs->cd();

      name.str("");
      name <<theSettings->GetBarName(i)<<"_TopCalibrate";
      cout<<"ON bar "<<theSettings->GetBarName(i)<<endl;

      TH2F * tempHistoTop = (TH2F*)gDirectory->Get(name.str().c_str());
      name.str("");
      name <<theSettings->GetBarName(i)<<"_BottomCalibrate";
      TH2F * tempHistoBottom = (TH2F*)gDirectory->Get(name.str().c_str());

    
      if (tempHistoTop != NULL &&tempHistoBottom!=NULL){

	TH1D * theProjectionBottom = GetRightTimeProjection(tempHistoBottom);
	TH1D * theProjectionTop = GetRightTimeProjection(tempHistoTop);

	theFile_Cs_Updated->cd();
	// BoxCarSmooth(theProjectionBottom,3)->Write("",TObject::kOverwrite);
	// BoxCarSmooth(theProjectionTop,3)->Write("",TObject::kOverwrite);
	theProjectionBottom->Rebin(ReBinFactor,TString(theProjectionBottom->GetName())+"_Rebin")->Write("",TObject::kOverwrite);
	theProjectionTop   ->Rebin(ReBinFactor,TString(theProjectionTop   ->GetName())+"_Rebin")->Write("",TObject::kOverwrite);

	CalibrateCSData(theProjectionTop,theProjectionBottom, theSettings->GetBarName(i) );

	// if (gROOT->GetListOfFunctions()->FindObject(TString(theSettings->GetBarName(i))+"T_CsFirstFunc")!=NULL){
	//   theProjectionTop->GetListOfFunctions()->Add((TF1*)gROOT->GetListOfFunctions()->FindObject(TString(theSettings->GetBarName(i))+"T_CsFirstFunc"));
	//   theProjectionBottom->GetListOfFunctions()->Add((TF1*)gROOT->GetListOfFunctions()->FindObject(TString(theSettings->GetBarName(i))+"B_CsFirstFunc"));
	// }
      

	theProjectionTop->Write("",TObject::kOverwrite);
	theProjectionBottom->Write("",TObject::kOverwrite);
      }else {
	cout<<"Problem"<<endl;
      }

    }//End for over bars

 
    theFile_Cs_Updated->Close();
    theFile_Cs->Close();
  }

  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////



  if (runNumberNa != -1){

    TFile *theFile_Na = OpenFile(runNumberNa);
    TFile * theFile_Na_Updated = OpenFileUpdated(runNumberNa);
    cout<<"Analyzing NA DATA"<<endl;
    for ( int i=0;i<numBars;i++){

      ////////////////////////////////////////////////
      ////////////////////////////////////////////////
      theFile_Na->cd();
      name.str("");
      name <<theSettings->GetBarName(i)<<"_AvgCorrected";
      cout<<"On bar "<<name.str()<<endl;
      TH2F * tempHisto = (TH2F*)gDirectory->Get(name.str().c_str());

      TString tttt = name.str().c_str();

      if(tempHisto!=NULL && ! tttt.Contains("N")){
  	theFile_Na_Updated->cd();
  	TH1D * theProjection = GetRightTimeProjection(tempHisto);


  	BoxCarSmooth(theProjection)->Write("",TObject::kOverwrite);

  	CalibrateNAData(theProjection,theProjection, theSettings->GetBarName(i) );

  	theProjection->Write("",TObject::kOverwrite);

      }else {
  	cout<<"Problem"<<endl;
      }
    }//End for over bars
    theFile_Na_Updated->Close();
    theFile_Na->Close();
  }  





  // if (runNumberNa != -1){

  //   TFile *theFile_Na = OpenFile(runNumberNa);
  //   TFile * theFile_Na_Updated = OpenFileUpdated(runNumberNa);
  //   cout<<"Analyzing NA DATA"<<endl;
  //   for ( int i=0;i<numBars;i++){

  //     ////////////////////////////////////////////////
  //     ////////////////////////////////////////////////
  //     //Get the Bottom Energy vs time plot///////////
  //     theFile_Na->cd();
  //     name.str("");
  //     name <<theSettings->GetBarName(i)<<"_TopCalibrate";
  //     cout<<"On bar "<<name.str()<<endl;
  //     TH2F * tempHistoTop = (TH2F*)gDirectory->Get(name.str().c_str());


  //     ////////////////////////////////////////////////
  //     ////////////////////////////////////////////////
  //     //Get the top Energy vs time plot///////////////
  //     name.str("");
  //     name <<theSettings->GetBarName(i)<<"_BottomCalibrate";
  //     TH2F * tempHistoBottom = (TH2F*)gDirectory->Get(name.str().c_str());

  //     if (tempHistoBottom != NULL && tempHistoTop != NULL){
  // 	theFile_Na_Updated->cd();
  // 	TH1D * theProjectionBottom = GetRightTimeProjection(tempHistoBottom);
  // 	TH1D * theProjectionTop = GetRightTimeProjection(tempHistoTop);

  // 	BoxCarSmooth(theProjectionBottom)->Write("",TObject::kOverwrite);
  // 	BoxCarSmooth(theProjectionTop)->Write("",TObject::kOverwrite);


  // 	CalibrateNAData(theProjectionTop,theProjectionBottom, theSettings->GetBarName(i) );

  // 	theProjectionTop->Write("",TObject::kOverwrite);
  // 	theProjectionBottom->Write("",TObject::kOverwrite);
  //     }else {
  // 	cout<<"Problem"<<endl;
  //     }
  //   }//End for over bars
  //   theFile_Na_Updated->Close();
  //   theFile_Na->Close();
  // }  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////

  /*
  TFile * theFile_Am = new TFile(rootfilesdirname + TString("/HistogramsFromRun527.root"),"update");
  cout<<"Analyzing AM Data"<<endl;
  for ( int i=0;i<numBars;i++){

    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    //Get the Bottom Energy vs time plot///////////

    name.str("");
    name <<theSettings->GetBarName(i)<<"_TopE";

    TH2F * tempHistoTop = (TH2F*)gDirectory->Get(name.str().c_str());
    TH1D * theProjectionTop = GetRightTimeProjection(tempHistoTop);

    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    //Get the top Energy vs time plot///////////////
    name.str("");
    name <<theSettings->GetBarName(i)<<"_BottomE";

    TH2F * tempHistoBottom = (TH2F*)gDirectory->Get(name.str().c_str());
    TH1D * theProjectionBottom = GetRightTimeProjection(tempHistoBottom);

    BoxCarSmooth(theProjectionBottom)->Write("",TObject::kOverwrite);
    BoxCarSmooth(theProjectionTop)->Write("",TObject::kOverwrite);



    CalibrateAMData(theProjectionTop,theProjectionBottom, theSettings->GetBarName(i) );


    theProjectionTop->Write("",TObject::kOverwrite);
    theProjectionBottom->Write("",TObject::kOverwrite);


  }//End for over bars

 
  */

  if (runNumberAm !=-1){
    TFile *theFile_Am = OpenFile(runNumberAm);
    TFile * theFile_Am_Updated = OpenFileUpdated(runNumberAm);

    cout<<"Analyzing AM Data"<<endl;
    for ( int i=0;i<numBars;i++){

      ////////////////////////////////////////////////
      ////////////////////////////////////////////////
      //Get the Bottom Energy vs time plot///////////
      theFile_Am->cd();
      name.str("");
      name <<theSettings->GetBarName(i)<<"_AvgCorrected";

      TH2F * tempHisto = (TH2F*)gDirectory->Get(name.str().c_str());

      TString tttt = name.str().c_str();
      if (tempHisto !=NULL && ! tttt.Contains("N")){
	theFile_Am_Updated->cd();
	TH1D * theProjection = GetRightTimeProjection(tempHisto);

	theProjection->Rebin(2,TString(theProjection->GetName())+"Rebin")->Write("",TObject::kOverwrite);

	CalibrateAMData(theProjection,theProjection, theSettings->GetBarName(i) );

	theProjection->Write("",TObject::kOverwrite);
      }

    }//End for over bars


  }


  //  ((TGraph*)theGraphs.At(0))->Draw("A*");

  TFile f("OutFile.root","recreate");
  

  for (map<TString,TGraphErrors*>::iterator ii= theGraphs.begin();ii!=theGraphs.end();ii++){
    Double_t x,y;
    ii->second->GetPoint(0,x,y);
    //    ii->second->RemovePoint(1);//Na22 2nd point
    if (x!=0){
      // TFitResultPtr result =ii->second->Fit("pol1","QSN");
      // Int_t status = result;
      // if (status==0){
      // 	Double_t toff = timingOffsetsFromFile[ii->second->GetName()];
      // 	cout<<ii->first<<" "<<result->Value(1)<<" "<<result->Value(0)<<" "<<toff<<endl;
      // }else{
      // 	cout<<"Fit Failed in linear fit part"<<endl;
      // }
      ii->second->Write();
    }
  }
  return 0;
}





