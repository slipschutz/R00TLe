
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TFitResult.h>
#include <TF1.h>
#include <TCanvas.h>

#include <TMath.h>


#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "LendaEvent.hh"

using namespace std;

Double_t FindCoumptonEdge(TH1F*h,int start,int endd){

  int  binStart = h->FindBin(start);
  int binEnd = h->FindBin(endd);


  Double_t max =0;
  Double_t min = 100000;
  //find the max
  int maxBin=-1;
  int minBin=-1;
  double MaxBinCenter;
  double MinBinCenter;
  
  for(int i=binStart ; i<binEnd ;i++){
    if ( h->GetBinContent(i) > max)
      {
	max = h->GetBinContent(i);
	maxBin=i;
      }
    if (h->GetBinContent(i)< min){
      min = h->GetBinContent(i);
      minBin=i;
    }
  }

  MaxBinCenter =h->GetBinCenter(maxBin);
  MinBinCenter =h->GetBinCenter(minBin);
  TF1 myFunc("myFunc","pol3",MaxBinCenter,MinBinCenter);
  
  TFitResultPtr result =h->Fit("myFunc","RSQ");
  

  Int_t status = result;

  max= (1/3.0)*(max+h->GetBinContent(maxBin-1)+
		h->GetBinContent(maxBin+1));



  Double_t twoThirdmax = (2.0/3.0)*max;

  Double_t theSpot=-1;
  if (status ==0 ){
    Double_t p0 = result->Value(0) - twoThirdmax;
    Double_t p1 = result->Value(1);
    Double_t p2 = result->Value(2);
    Double_t p3 = result->Value(3);
    
    for (double i=0;i<1000;i++){
      double xVal =MaxBinCenter+ (i/1000.0)*(MinBinCenter-MaxBinCenter);
      
      
      double yVal = p0+ p1*xVal + p2*xVal*xVal+p3*xVal*xVal*xVal;
      
      if (TMath::Abs(yVal) < 1){
	theSpot=xVal;
	break;
      }
    }
    
  }else {
    cout<<"Fit failed on "<<h->GetName()<<endl;
    return -100;
  }

  return theSpot;
}

void ThreshHoldFinder(){
  

  vector <string> filesForCalibration;
  filesForCalibration.resize(3);
  
  filesForCalibration[0]="/events/e10003/rootfiles/run-0380-00.root";
  filesForCalibration[1]="/events/e10003/rootfiles/run-0372-00.root";
  filesForCalibration[2]="/events/e10003/rootfiles/run-0376-00.root";

  // files should go in the following order
  // Am Cs NA
  //
  map <int,string> num2source;
  num2source[0]="AM";
  num2source[1]="CS";
  num2source[2]="NA";
  
  int NumBars=52;
  vector <TH1F*> AmSpectrums;
  vector <TH1F*> CSSpectrums;
  vector <TH1F*> NASpectrums;
  
  map <int, vector<TH1F*> > num2Spectrums;
  num2Spectrums[0].resize(NumBars);
  num2Spectrums[1].resize(NumBars);
  num2Spectrums[2].resize(NumBars);


  TTree * caltree;  //The current tree
  TFile * currentFile;
  for (int fileNum=0;fileNum<filesForCalibration.size();fileNum++){
    currentFile = new TFile(filesForCalibration[fileNum].c_str());
    
    if (gDirectory->Get("caltree") == NULL){
      cout<<"No caltree in current directory"<<endl;
      return;
    }
    
    stringstream namestream;
    for (int i=0;i<NumBars;i++){
      namestream.str("");
      namestream<<num2source[fileNum]<<"BarId"<<i;
      num2Spectrums[fileNum][i] = new TH1F(namestream.str().c_str(),namestream.str().c_str(),500,0,50000);
      num2Spectrums[fileNum][i]->SetDirectory(0);
    }

    caltree = (TTree*)gDirectory->Get("caltree");
    LendaEvent * event = new LendaEvent();
    caltree->SetBranchAddress("lendaevent",&event);
    
    cout<<"Dumping the Spectrums for "<<num2source[fileNum]<<endl;
    Long64_t numEntries = caltree->GetEntries();
    cout<<"There are "<<numEntries<<" in this tree"<<endl;
    
    for (Long64_t jentry=0;jentry<numEntries;jentry++){
      caltree->GetEntry(jentry);
      
      int numBars = event->NumBars;
      if (numBars!=0){
	for (int j=0;j<numBars;j++){
	  if ( event->Bars[j].SimpleEventBit && event->Bars[j].Tops[0].GetEnergy()>0 && event->Bars[j].Bottoms[0].GetEnergy()>0){

	    int BarId = event->Bars[j].BarId;
	    num2Spectrums[fileNum][BarId]->Fill(event->Bars[j].Tops[0].GetEnergy());
	    num2Spectrums[fileNum][BarId]->SetTitle(event->Bars[j].Tops[0].GetChannelName().c_str());
	  }
	}
      }

      
      if (jentry % 1000 == 0){
	cout<<"On Entry "<<jentry<<endl;
      }
      
    }
    
    
    currentFile->Close();
    delete currentFile;
  }

  vector <TGraphErrors*> theThreshPlots;


  Double_t Na1 = 341;
  Double_t Na2 = 1062;
  
  Double_t Am1 = 26.3;
  Double_t Am2 = 59.5;
  
  Double_t Cs1 = 33;
  Double_t Cs2 = 478;
  
  Double_t y[3];
  y[0]= Na1;
  y[1]= Na2;
  // y[2]= Am1;
  // y[3]= Am2;
  y[2]= Cs2;
  // y[5]= Cs2;

  for (int i=0;i<NumBars;i++){
    Double_t integral=1
    integral= num2Spectrums[0][i]->Integral();
    num2Spectrums[0][i]->Scale(1.0/integral);

    integral= num2Spectrums[1][i]->Integral();
    num2Spectrums[1][i]->Scale(1.0/integral);

    integral= num2Spectrums[2][i]->Integral();
    num2Spectrums[2][i]->Scale(1.0/integral);

  }


  TFile out("out.root","recreate");
  for (int i=0;i<NumBars;i++){
    Double_t x[3];
    Double_t ex[3];
    for (int j=0;j<filesForCalibration.size();j++){
      num2Spectrums[j][i]->Write();
      
      if (num2source[j] == "NA"){//These are NA-22
	x[0] = FindCoumptonEdge(num2Spectrums[j][i],4000,9000);
	x[1] = FindCoumptonEdge(num2Spectrums[j][i],16000,25000);
	ex[0]= 0.05*x[0];
	ex[1]= 0.05*x[1];
      } else if (num2source[j]=="CS"){
	x[2] = FindCoumptonEdge(num2Spectrums[j][i],8000,20000);
	ex[2]=0.05*x[2];
      }else if (num2source[j]=="AM"){
	// x[4]=0;
	// x[5]=0;
      } 

    }
    theThreshPlots.push_back(new TGraphErrors(3,x,y,ex,0));
    theThreshPlots[i]->SetName(TString("Thresh")+num2Spectrums[0][i]->GetName());
    theThreshPlots[i]->SetTitle(TString("Thresh")+num2Spectrums[0][i]->GetName());
    
    TF1 * func = new TF1("func","[0]*x");
    TFitResultPtr result = theThreshPlots[i]->Fit("func","SQ");
    Int_t status =result;
    if (status == 0){
      cout<<"****************"<<endl;
      cout<<theThreshPlots[i]->GetName()<<endl;
      cout<<result->Value(0)<<endl;
    }
    

    theThreshPlots[i]->Write();
  }


  out.Close();



}
