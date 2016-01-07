
//
//  Filter object
//


#include "LendaFilter.hh"

#include "TMath.h"
#include "TGraph.h"
#include "TFitResult.h"
#include "TF1.h"

#include <sstream>
using namespace std;

LendaFilter::LendaFilter()
{
  numOfBadFits=0;

}
LendaFilter::~LendaFilter()
{

}


void LendaFilter::FastFilter(std::vector <UShort_t> &trace,std::vector <Double_t> &thisEventsFF,Double_t FL,Double_t FG){
  Double_t sumNum1=0;
  Double_t sumNum2=0;
  
  int start =2*FL+FG-1;
  
  for (int i=0;i< (int) trace.size();i++)
    {
      if (i>=start){
	for (int j= i-(FL-1) ;j<=i;j++)
	  {
	    if (j>=0)
	      sumNum1 = sumNum1+ trace[j];
	    else 
	      cout<<"Oh NO"<<endl;
	  }
	
	for (int j=i-(2*FL+FG-1);j<=i-(FL+FG);j++)
	  {
	    if (j>=0)
	      sumNum2 = sumNum2+ trace[j];
	    else
	      cout<<"oh no"<<endl;
	  }
      }


      thisEventsFF.push_back(sumNum1-sumNum2);

      sumNum1=0;
      sumNum2=0;
    }//End for    
}

void LendaFilter::FastFilterOp(std::vector <UShort_t> &trace,std::vector <Double_t> &thisEventsFF,Double_t FL,Double_t FG){
  Double_t sumNum1=0;
  Double_t sumNum2=0;
  
  int size = trace.size();
  int half = size/2;
  thisEventsFF.resize(size,0);

  
  int start =half -0.15*size;
  int end = half + 0.15*size;

  for (int i=start;i<end;i++){
    
    for (int j= i-(FL-1) ;j<=i;j++)
      {
	if (j>=0)
	  sumNum1 = sumNum1+ trace[j];
	else 
	  cout<<"Oh NO"<<endl;
      }
    
    for (int j=i-(2*FL+FG-1);j<=i-(FL+FG);j++)
      {
	if (j>=0)
	  sumNum2 = sumNum2+ trace[j];
	else
	  cout<<"oh no"<<endl;
      }
    thisEventsFF[i]=sumNum1-sumNum2;
    sumNum1=0;
    sumNum2=0;
  }
}




void LendaFilter:: FastFilterFull(std::vector <UShort_t> &trace,
			     std::vector <Double_t> &thisEventsFF,
			     Double_t FL,Double_t FG,Double_t decayTime)
{
  /*
  Double_t S0, Sg, S1; // Varibale names from Tan paper
  
  Double_t deltaT = 1; //1 clock tick

  Double_t b1 = TMath::Exp(-deltaT/decayTime);

  Double_t r1 = 1.0 / ( 1-b1);

  Double_t A0,A1,J;
  
  Double_t baseLine=0;

  for (int k=0;k<20;k++)
    baseLine+=trace[k];
  
  baseLine = baseLine/20.0;
  
  thisEventsFF.resize(trace.size(),0);

  for (int i=FL*2+FG;i< (int) trace.size(); ++i){
   
    S0=0;
    Sg=0;
    S1=0;
    
    //S0 loop
    for (int j=i-FL*2-FG;j<i-FL-FG;++j){
      if (j>=0 && j <(int) trace.size()){
	S0+=(trace[j]-baseLine);
	if (S0 <0)
	  S0=0;
      }
    }

    //Sg loop
    for (int j=i-FL-FG;j<i-FL;++j){
      if (j>=0 &&j <(int) trace.size()){
	Sg+=(trace[j]-baseLine);
	if (Sg<0)
	  Sg=0;
      }
    }

    //S1 loop

    for (int j=i-FL;j<i;++j){
      if (j>=0 &&j < (int) trace.size()){
	S1+=(trace[j]-baseLine);
	if (S1 <0)
	  S1=0;
      }
    }
    
     
      A0= S0 / (r1*(1-Power(b1,FL)));
    
    J=(Power(b1,FL)*(1-Power(b1,FG))*S0 )/ (Power(b1,FL)-1) +Sg;
    
    A1=(1/r1)*( (Power(b1,FL+FG)*S0)/(Power(b1,FL)-1) - S1/(Power(b1,FL)-1));
  
    
    Double_t a0,ag,a1;
    ag=1;
    a0=TMath::Power(b1,FL)/(TMath::Power(b1,FL)-1);
    a1=(-1.0/((1-TMath::Power(b1,FL))) );

    
    if (i+2*FL+FG < thisEventsFF.size()){
      thisEventsFF[i]=ag*Sg+a0*S0+a1*S1;
    }
  }

*/
}


std::vector <Double_t> LendaFilter::CFD(std::vector <Double_t> &thisEventsFF,
				   Double_t CFD_delay,
				   Double_t CFD_scale_factor){

  std::vector <Double_t> thisEventsCFD;
  thisEventsCFD.resize(thisEventsFF.size(),0);


  for (int j=0;j<(int) thisEventsFF.size() - CFD_delay;j++) {
    thisEventsCFD[j+CFD_delay] = thisEventsFF[j+CFD_delay] - 
      thisEventsFF[j]/ ( TMath::Power(2,CFD_scale_factor+1) );
  }

  return thisEventsCFD;

}

std::vector <Double_t> LendaFilter::CFDOp(std::vector <Double_t> &thisEventsFF,
				   Double_t CFD_delay,
				   Double_t CFD_scale_factor){

  std::vector <Double_t> thisEventsCFD;
  int size = thisEventsFF.size();
  thisEventsCFD.resize(thisEventsFF.size(),0);
  
  int start = size/2 -0.15*size;
  int end = size/2 + 0.15*size;

  for (int j=start;j<end;j++) {
    thisEventsCFD[j+CFD_delay] = thisEventsFF[j+CFD_delay] - 
      thisEventsFF[j]/ ( TMath::Power(2,CFD_scale_factor+1) );
  }
  
  return thisEventsCFD;
}


#define BAD_NUM -10008

Double_t LendaFilter::GetZeroCrossing(std::vector <Double_t> & CFD,Int_t & NumZeroCrossings,Double_t & residual){

  Double_t softwareCFD;
  std::vector <Double_t> thisEventsZeroCrossings(0);
  Double_t MaxValue=0;
  Int_t MaxIndex=-1;
  int Window=40;
  for (int j=(CFD.size()/2.0)-Window;j< (int) (CFD.size()/2.0)+Window;j++) { 
    if (CFD[j] >= 0 && CFD[j+1] < 0 && 
	TMath::Abs(CFD[j] - CFD[j+1]) > 40)
      {//zero crossing point
	softwareCFD =j + CFD[j] / ( CFD[j] + TMath::Abs(CFD[j+1]) );
	thisEventsZeroCrossings.push_back(softwareCFD);
	if (TMath::Abs(CFD[j] - CFD[j+1]) > MaxValue){
	  MaxValue=TMath::Abs(CFD[j] - CFD[j+1]);
	  MaxIndex =thisEventsZeroCrossings.size()-1;
	  residual=CFD[j];
	}
      }
  }
  NumZeroCrossings=thisEventsZeroCrossings.size();
  if (thisEventsZeroCrossings.size() == 0) // no Zero Crossing found
    return BAD_NUM;
  else
  return thisEventsZeroCrossings[MaxIndex]; // take the max one
  /*  if (thisEventsZeroCrossings.size() != 1 )
    return 2*BAD_NUM;
  */

}




Double_t LendaFilter::GetZeroCrossingImproved(std::vector <Double_t> & CFD,Int_t& MaxInTrace,Double_t & residual){
 
  Double_t softwareCFD=BAD_NUM;
  Double_t thisEventsZeroCrossing;

  int Window=40;


  //Take the first Zerocrossing after the peak in the TRACE

  for (int j=MaxInTrace;j< (int) (CFD.size()/2)+Window;j++) { 
    if (CFD[j] >= 0 && CFD[j+1] < 0 ){
      //zero crossing point
      softwareCFD =j + CFD[j] / ( CFD[j] + TMath::Abs(CFD[j+1]) );
      
      residual=CFD[j];
      break;
      
    }
  }

  return softwareCFD;
}

vector <Double_t> LendaFilter::GetAllZeroCrossings(std::vector <Double_t> & CFD){


  Double_t softwareCFD;
  std::vector <Double_t> thisEventsZeroCrossings(0);


  for (int j=0;j< (int) CFD.size()-1;j++){
    if (CFD[j] >= 0 && CFD[j+1] < 0 ){
      //zero crossing point
      softwareCFD =j + CFD[j] / ( CFD[j] + TMath::Abs(CFD[j+1]) );
      thisEventsZeroCrossings.push_back(softwareCFD);
    }
  }

  return thisEventsZeroCrossings;
}




Double_t LendaFilter::GetZeroCrossingOp(std::vector <Double_t> & CFD,Int_t & NumZeroCrossings){

  Double_t softwareCFD=-10;
  std::vector <Double_t> thisEventsZeroCrossings(10,0);
  Double_t MaxValue=0;
  Int_t MaxIndex=-1;

  for (int j=(CFD.size()/2.0)-20;j< (int) (CFD.size()/2.0)+20;j++) { 
    if (CFD[j] >= 0 && CFD[j+1] < 0 && 
	TMath::Abs(CFD[j] - CFD[j+1]) > 5)
      {//zero crossing point
	softwareCFD =j + CFD[j] / ( CFD[j] + TMath::Abs(CFD[j+1]) );
	break;
      }
  }
  NumZeroCrossings=-1;
  if (softwareCFD==-10) // no Zero Crossing found
    return BAD_NUM;
  else
    return softwareCFD;
  /*  if (thisEventsZeroCrossings.size() != 1 )
    return 2*BAD_NUM;
  */

}


vector <Double_t> LendaFilter::GetZeroCrossingHighRate(const std::vector <Double_t> & CFD,const std::vector<Int_t> & PeakSpots){

  std::vector <Double_t> thisEventsZeroCrossings(0);  
  for (auto i : PeakSpots){
    for (int j=i;j< (int)CFD.size()-1;j++) { 
      if (CFD.at(j)>= 0 && CFD.at(j+1) < 0 ){
	Double_t softwareCFD =j + CFD[j] / ( CFD[j] + TMath::Abs(CFD[j+1]) );
	thisEventsZeroCrossings.push_back(softwareCFD);
	j=CFD.size()+1000;
	break;
      }//end if 
    }//end for j
  }//end for i
 
  return thisEventsZeroCrossings;
}


vector <Double_t> LendaFilter::GetZeroCrossingCubicHighRate(const std::vector <Double_t> & CFD,const std::vector<Int_t> & PeakSpots){

  std::vector <Double_t> thisEventsZeroCrossings(0);  
  for (auto i : PeakSpots){
    for (int j=i;j< (int)CFD.size()-1;j++) { 
      if (CFD.at(j)>= 0 && CFD.at(j+1) < 0 ){
	Double_t cubicCFD =DoMatrixInversionAlgorithm(CFD,j);
	thisEventsZeroCrossings.push_back(cubicCFD);
	j=CFD.size()+1000;
	break;
      }//end if 
    }//end for j
  }//end for i
 
  return thisEventsZeroCrossings;
}

vector<Int_t> LendaFilter::GetPulseHeightHighRate(const std::vector <UShort_t> & trace,const std::vector<Int_t> &PeakSpots){
  vector <Int_t> result;
  for (auto & i : PeakSpots){
    result.push_back(trace[i]);
  }
  return result;
}


#include <map>

Double_t LendaFilter::DoMatrixInversionAlgorithm(const std::vector <Double_t> & CFD, Int_t theSpotAbove){
  
  Double_t x[4];
  TMatrixT<Double_t> Y(4,1);//a column vector
  
  for (int i=0;i<4;i++){
    x[i]= theSpotAbove -1+ i; //first point is the one before zerocrossing
    Y[i][0]=CFD[ theSpotAbove -1+i];
  }


  TMatrixT<Double_t> A(4,4);//declare 4 by 4 matrix

  for (int row=0;row<4;row++){
    for (int col=0;col<4;col++){
      A[row][col]= pow(x[row],3-col);
    }
  }

  //  A.Print();

  TMatrixT<Double_t> invertA = A.Invert();

  // invertA.Print();

  TMatrixT<Double_t> Coeffs(4,1);
  Coeffs = invertA*Y;
  
  //cout<<"COEFFS are "<<endl;
  //Coeffs.Print();

  //the x[1] is theSpot above so start there
  bool notDone =true;
  double left = x[1];//initial above
  double right =x[2];//initial below
  double valUp = getFunc(Coeffs,left);
  double valDown =getFunc(Coeffs,right);

  int loopCount=0;
  while (notDone){
    loopCount++;
    if (TMath::Abs(TMath::Abs(valUp)-TMath::Abs(valDown) ) <0.001)
      notDone = false;
      
    double mid = (left+right)/2.0;
    double midVal = getFunc(Coeffs,mid);
    

    if (midVal > 0)
      left=mid;
    else 
      right=mid;
    
    valUp = getFunc(Coeffs,left);
    valDown =getFunc(Coeffs,right);
    if (loopCount >30 ){//kill stuck loop
      notDone=false;
      left =BAD_NUM;
    }

  }

  return left;
}

vector <Double_t> LendaFilter::GetMatrixInversionAlgorithmCoeffients
(const std::vector <Double_t> & CFD, Int_t &ReturnSpotAbove){


  std::map <double,int> zeroCrossings;
  double max=0;

  int begin = (CFD.size()/2)-40;
  int end = (CFD.size()/2)+40;

  for (int i =begin;i<end;i++){
    if (CFD[i]>=0 && CFD[i+1]<0){
      double val = CFD[i] - CFD[i+1];
      if ( val > max)
	max = val;
      //put this crossing in map
      zeroCrossings[val]=i;
    }
  }
  
  int theSpotAbove = zeroCrossings[max];
  ReturnSpotAbove=theSpotAbove;

  Double_t x[4];
  TMatrixT<Double_t> Y(4,1);//a column vector
  
  for (int i=0;i<4;i++){
    x[i]= theSpotAbove -1+ i; //first point is the one before zerocrossing
    Y[i][0]=CFD[ theSpotAbove -1+i];
  }


  TMatrixT<Double_t> A(4,4);//declare 4 by 4 matrix

  for (int row=0;row<4;row++){
    for (int col=0;col<4;col++){
      A[row][col]= pow(x[row],3-col);
    }
  }

  //  A.Print();

  TMatrixT<Double_t> invertA = A.Invert();

  // invertA.Print();

  TMatrixT<Double_t> Coeffs(4,1);
  Coeffs = invertA*Y;
  
  vector <Double_t> retVec;
  for (int i=0;i<4;i++){
    retVec.push_back(Coeffs[i][0]);
  }

  return retVec;
}


Double_t LendaFilter::GetZeroCubic(std::vector <Double_t> & CFD,Int_t &MaxInTrace){
  

  // std::map <double,int> zeroCrossings;
  // double max=0;

  int begin = MaxInTrace;
  int end = (CFD.size()/2)+40;

  for (int i =begin;i<end;i++){
    if (CFD[i]>=0 && CFD[i+1]<0){
      
      return DoMatrixInversionAlgorithm(CFD,i);
      break;
    }
  }
  //     double val = CFD[i] - CFD[i+1];
  //     if ( val > max)
  // 	max = val;
  //     //put this crossing in map
  //     zeroCrossings[val]=i;
  //   }
  // }
  
  // int theSpotAbove = zeroCrossings[max];

  // return DoMatrixInversionAlgorithm(CFD,theSpotAbove);
}

Double_t LendaFilter::GetZeroFitCubic(std::vector <Double_t> & CFD){
  

  //Find the largest zero Crossing
  std::map <double,int> zeroCrossings;
  double max=0;
  
  //restrict search to center part of tace
  int begin = (CFD.size()/2)-40;
  int end = (CFD.size()/2)+40;

  for (int i =begin;i<end;i++){
    if (CFD[i]>=0 && CFD[i+1]<0){
      double val = CFD[i] - CFD[i+1];
      if ( val > max)
	max = val;
      //put this crossing in map
      zeroCrossings[val]=i;
    }
  }
  //Take the spot before out of map
  int theSpotAbove = zeroCrossings[max];

  //Points to consider are { spotAbove-2,spotAbove-1,
  //spotAbove, spotBelow, spotBelow+1,spotBelow+2}
  //
  //  Use fitting to find 3rd order polynomial from these 6 points
  //

  

  Double_t x[6];
  Double_t y[6];
  for (int i=0;i<6;i++){
    x[i]= theSpotAbove -2+ i; //first point is the one before zerocrossing
    y[i]=CFD[theSpotAbove -2+i];
  }
  TGraph theGraphForFitting(6,x,y);
  
  TFitResultPtr fitPointer =  theGraphForFitting.Fit("pol3","QSN");
  Int_t status = fitPointer;
  
  
  if ( status == 0){ // if the Fit was a success
    vector<double> Coeffs(4);//3rd order polynomial should have 4 coeffs
    Coeffs[0]=fitPointer->Value(0);
    Coeffs[1]=fitPointer->Value(1);
    Coeffs[2]=fitPointer->Value(2);
    Coeffs[3]=fitPointer->Value(3);
  

    //the x[1] is theSpot above so start there
    bool notDone =true;
    double left = x[2];//initial above should be in 3rd spot 
    double right =x[3];//initial below should be in 4th spot
    double valUp = getFunc(Coeffs,left);
    double valDown =getFunc(Coeffs,right);
    int loopCount=0;
    while (notDone){
      loopCount++;
      if (TMath::Abs(TMath::Abs(valUp)-TMath::Abs(valDown) ) <0.001)
	notDone = false;
      
      double mid = (left+right)/2.0;
      double midVal = getFunc(Coeffs,mid);
    

      if (midVal > 0)
	left=mid;
      else 
	right=mid;
    
      valUp = getFunc(Coeffs,left);
      valDown =getFunc(Coeffs,right);
      if (loopCount >30 ){//kill stuck loop
	notDone=false;
	left =BAD_NUM;
      }

    }
    return left;
  } else {// End Fit Status If
    cout<<"FIT FAILED"<<endl;
    return BAD_NUM;
  }

 
}



double LendaFilter::getFunc(TMatrixT<Double_t> Coeffs,double x){
  double total =0;
  for (int i=0;i<4;i++){
    total = total + Coeffs[i][0]*TMath::Power(x,3-i);
  }
  return total;

}

double LendaFilter::getFunc(vector<double> &Coeffs,double x){
  double total =0;
  for (int i=0;i<4;i++){
    total = total + Coeffs[i]*TMath::Power(x,i);
  }
  return total;

}



Double_t LendaFilter::fitTrace(std::vector <UShort_t> & trace,Double_t sigma,Double_t num){

  Int_t size = (Int_t) trace.size();
  std::vector <Double_t> y_values,x_values;


  for ( Int_t i=0;i < size;++i){
    y_values.push_back( (double_t) trace[i]  );
    x_values.push_back( (Double_t) i);
    
  }

  //Find Maximum 
  Double_t max=-1;
  Int_t maxBin=-1;
  for (Int_t i=0;i <size;++i){
    if (y_values[i] > max){
      max = y_values[i];
      maxBin=i;
    }
  }
  //Set up fit function
  Double_t base=300;
  Double_t A =0;
  Int_t fitWindowWidth=10;  //plus or minus 5 bins on either side of max
  //to be taken into acount during fit
  Double_t mu=-1000;
  
  //1.7711 the sigma determined from looking at traces
  stringstream stream;
  stream<< "[2]+[0]*exp(-0.5*( ((x-[1])/"<<sigma<<  ")^2) )";
  
  


  TF1 *myfit = new TF1("myfit",stream.str().c_str(),0,200); 

  myfit->SetParameter(0, 1);
  myfit->SetParameter(1, 100);
  myfit->SetParameter(2,300);
  myfit->SetParLimits(0,0,1000);// Make sure the constant out in front is>0

  
  //Define the trace as a Tgraph
  TGraph * gr = new TGraph(size,x_values.data(),y_values.data());


  TFitResultPtr fitPointer = gr->Fit("myfit","S0Q",
				     "",maxBin-fitWindowWidth,maxBin+fitWindowWidth);
  
  Int_t fitStatus = fitPointer;//Ridiculous root
  
  if ( fitStatus == 0 ) { //no errors in fit
    mu =fitPointer->Value(1);
    A=fitPointer->Value(0);
    base = fitPointer->Value(2);
  } else {
    //        cout<<"***Warning bad fit result retured**"<<endl;
    //  cout<<"***jentry is "<<num<<"***"<<endl;
    numOfBadFits++;
  }
  
  //Detele objects
  gr->Delete();
  myfit->Delete();

  
  
  return mu;
}

Double_t LendaFilter::GetEnergyOld(std::vector <UShort_t> &trace){
  /////////////////////////////////////////////////////////////////////
  // Basic Algorithm for extracting pulse integral from trace	     //
  // uses first 10 points as background level. Sums up the	     //
  // whole trace then subtracts background to background subtracted  //
  // energy 							     //
  /////////////////////////////////////////////////////////////////////

  Double_t thisEventsIntegral;
  Double_t sum=0;
  Double_t signalTotalIntegral=0;
  for ( int i=0 ;i<10;i++)
    sum = sum + trace[i];
  sum = sum/10.0; // average of first 10 points should be pretty good background
  for (int i=0;i< (int) trace.size();++i) {
    signalTotalIntegral = trace[i]+ signalTotalIntegral;
  }
  if (  signalTotalIntegral - sum *trace.size() > 0 )
    thisEventsIntegral = signalTotalIntegral - sum *trace.size();
  else{
    thisEventsIntegral = BAD_NUM;
  }


  return thisEventsIntegral;

}

Double_t LendaFilter::GetEnergy(std::vector <UShort_t> &trace,Int_t MaxSpot){
  
  ////////////////////////////////////////////////////////////////////////////
  // Slightly more complicated algorithm for getting energy of pulse.	    //
  // Takes the first 20% of the trace as a background.  Then it takes only  //
  // a window around the maximum spot as the pulse part of the trace.  This //
  // helps with strange pulse where there might be additional structures in //
  // the trace that would distort things (afterpulse, crazy noise..)	    //
  ////////////////////////////////////////////////////////////////////////////

  Double_t thisEventsIntegral;
  Double_t sumBegin=0;
  Double_t sumEnd=0;

  Double_t signalIntegral=0;

  int traceLength=trace.size();
  int LengthForBackGround=0.2*traceLength;

  Int_t windowForEnergy=20;

  if ( MaxSpot - windowForEnergy < 0 ){
    return BAD_NUM;
  } else if (MaxSpot + windowForEnergy > (traceLength -1) ){
    return BAD_NUM;
  }


  for ( int i=0 ;i<LengthForBackGround;i++){
    sumBegin = sumBegin + trace[i];
    sumEnd = sumEnd + trace[traceLength-1-i];
  }
  
  Double_t BackGround=BAD_NUM;

  if (MaxSpot > LengthForBackGround && MaxSpot < (traceLength-LengthForBackGround) ){
    //The peak is in the middle use the beginning of trace as background
    BackGround=sumBegin/LengthForBackGround;    
  } else if (MaxSpot < LengthForBackGround) {
    //The peak is in the beginning of trace use end as background
    BackGround=sumEnd/LengthForBackGround;
  } else if (MaxSpot > (traceLength-LengthForBackGround)){
    //Peak is at end use beginning for backaground
    BackGround=sumBegin/LengthForBackGround;
  } else{
    //Something makes no sense
    return BAD_NUM;
  }




  for (int i=MaxSpot-windowForEnergy;i< MaxSpot+windowForEnergy;++i) {
    signalIntegral = trace[i]+ signalIntegral;
  }

  if (  signalIntegral - BackGround *(2*windowForEnergy)>0){
    thisEventsIntegral = signalIntegral - BackGround *(2*windowForEnergy);
  }  else{
    thisEventsIntegral = BAD_NUM;
  }


  return thisEventsIntegral;

}


vector<Double_t> LendaFilter::GetEnergyHighRate(const std::vector <UShort_t> & trace,std::vector <Int_t> &PeakSpots,Double_t & MaxValueOut,Int_t & MaxIndexOut,FilterDebugInfo* debug){

  Int_t NumberOfSamples = 8;
  Int_t TraceLength = trace.size();
  
  Int_t SampleSize=TMath::Floor(TraceLength/Float_t(NumberOfSamples));
  Int_t Remainder = TraceLength-SampleSize*NumberOfSamples;


  //  Int_t SampleSize = TMath::Floor(fracForSample*TraceLength);

  vector <Double_t> StandardDeviations(NumberOfSamples,0);
  vector <Double_t> Averages(NumberOfSamples,0);

  //First For each sample window find the average of those points
  //And find the Maximum point (will exclude the reamining points at the end of the 
  //trace
  Double_t Max=0;
  Int_t MaxIndex=-1;
  for (int i=0;i <NumberOfSamples;i++){
    for (int j=0;j<SampleSize;j++){
      Double_t val =trace[j +i*SampleSize];
      Averages[i] += val;

      if (val > Max){
	Max=val;
	MaxIndex=j+i*SampleSize;
      }

    }//end for over j
    Averages[i]/=SampleSize;
  }
  ///Now look at the end of the trace to see if the maximum is there
  for (int i=SampleSize*NumberOfSamples;i<TraceLength;i++){
    Double_t val =trace[i];
    if (val > Max){
      Max=val;
      MaxIndex=i;
    }
  }
  
  MaxValueOut=Max;
  MaxIndexOut=MaxIndex;
  
  map<Double_t,pair<int,int> > baseLineRanges;
  map<Double_t,Double_t> StandardDeviation2Average;
  for (int i=0;i<NumberOfSamples;i++){
    for (int j=0;j <SampleSize;j++){
      StandardDeviations[i] += TMath::Power((trace[j +i*SampleSize]-Averages[i]),2);
    }

    baseLineRanges.insert( make_pair(StandardDeviations[i],make_pair(i*SampleSize,(i+1)*SampleSize)));
    StandardDeviation2Average.insert(make_pair(StandardDeviations[i],Averages[i]));
  }

  auto it = StandardDeviation2Average.begin();
  Double_t BaseLine = it->second;
  
  Double_t stanDev =it->first;
  




  //Now we have found the base line and the maximum point.  We look for other real pulses in trace
  //Look for other maximum in the trace that are greater than 30% of total max
  //  Int_t ThresholdForOtherPulseInTrace=  TMath::Floor(15*stanDev);//0.10*BaseLine);    ///0.3*(Max-BaseLine));

  Int_t  ThresholdForOtherPulseInTrace= TMath::Floor(   std::max(0.1*MaxValueOut,4500.0-BaseLine));

  //  vector <int> IndexOfMaximums;


  if (debug!=NULL){
    debug->BaseLine=BaseLine;
    debug->StandardDev=TMath::Sqrt(stanDev);
    debug->BaseLineWindowBegin =baseLineRanges[stanDev].first;
    debug->BaseLineWindowEnd =baseLineRanges[stanDev].second;
    debug->Threshold=ThresholdForOtherPulseInTrace;
  }

  Int_t windowForEnergy=2;
  Int_t EndTraceCut =2;//2*windowForEnergy;
  Double_t currentMax=0;
  bool HasCrossedThreshold=false;

  for (int i=0;i<TraceLength-EndTraceCut;i++){
    Double_t val = trace[i]-BaseLine;
    if (val > ThresholdForOtherPulseInTrace){
      HasCrossedThreshold=true;
    }
    
    if (val > currentMax){
      currentMax=val;
    } else if (val <currentMax && HasCrossedThreshold){

      //Possible peak spot
      int PossibleSpot = i-1;
      //check to see that spot before/after peak is less than peak value
      if ( trace[PossibleSpot-1] <trace[PossibleSpot]&&
	   trace[PossibleSpot+1] <trace[PossibleSpot]){
	PeakSpots.push_back(PossibleSpot);

	//reset the current max value
	currentMax=0;
	HasCrossedThreshold=false;
	//Now must skip foward in time  around the length of one RF bucket
	//IE the minimum amount of time before a second real pulse could be there
	int index = i + 8;//(+8 because we are at i+1 and will get +1 again at end of loop) 10 clock tics is 40 nanosecs
	if (index > TraceLength - windowForEnergy){
	  //too close to end of trace.  End the search
	  i=TraceLength+1000;
	  break;
	}else{
	  //Skip foward to place
	  i=index;
	    
	}
      }//end confirmed peak spot
    }//end possible peak spot
  }//end for

  //The Maximums have been found
  //Now finally we calculate pulse integrals
  vector <Double_t> theEnergies;
  for (auto i : PeakSpots){
    Double_t temp=0;
    for (int j=0;j<2;j++){
      temp+= (trace[i+j]-BaseLine);
    }
    theEnergies.push_back(temp);
    //    cout<<"Energy window "<<i-windowForEnergy<<" to "<<i+windowForEnergy<<" "<<temp<<endl;
  }
  

  
  return theEnergies;
  
}

Double_t LendaFilter::GetGate(std::vector <UShort_t> &trace,int start,int L){


  int range =L;
  int  window = floor( trace.size()/5.0);
  Double_t bgk=0;

  for (int i=0;i<window;i++){
    bgk = trace[i]+bgk;
  }
  bgk=bgk/(window);

  Double_t total=0;
  for (int i=start;i<start+L;i++)
    total =total+trace[i];


  return total-(bgk*range);
}


Int_t LendaFilter::GetMaxPulseHeight(vector <UShort_t> &trace,Int_t& MaxSpot){

  int _maxSpot=-1;
  Double_t max=0;
  for (int i=0;i<trace.size();i++){
    if (trace[i]>max){
      max=trace[i];
      _maxSpot=i;
    }
    
  }
  MaxSpot=_maxSpot;
  return max;
}


Int_t LendaFilter::GetMaxPulseHeight(vector <Double_t> &trace,Int_t& MaxSpot){

  int _maxSpot=-1;
  Double_t max=0;
  for (int i=0;i<trace.size();i++){
    if (trace[i]>max){
      max=trace[i];
      _maxSpot=i;
    }
  }
  MaxSpot=_maxSpot;
  return max;
}



Int_t LendaFilter::GetStartForPulseShape(Int_t MaxSpot){
  if (MaxSpot-4 > 0){
    return (MaxSpot-4);
  }else {
    return 0;
  }
}



Int_t LendaFilter::CalculateCFD(vector<UShort_t> trace,vector<Double_t> &CFDOut) {
  CFDOut.clear();
  //  vector<Double_t> CFDOut;
  Int_t TFLength=6;
  Int_t TFGap=0;
  Int_t CFDSF=4;
  Int_t CFDDelay=6;
  Double_t CFDWeight=0.5;
  Int_t CFDThreshold=50;

  Int_t CFDTime=0;
  Int_t CFDTriggerPoint;

  Double_t cfdvalue = -1;
  Int_t cfdtrigpoint = -1;
  Int_t A=0;
  Int_t m=0;

  Double_t S0;
  Double_t S1;
  Double_t S2;
  Double_t S3;

  
  if(TFLength<0) {
    cout<<"Set the Trigger Filter Length!"<<endl;
    return -107;
  }
  if(TFGap<0) {
    cout<<"Set the Trigger Filter Gap!"<<endl;
    return -108;
  }  
  if(CFDSF <0) {
    cout<<"Set CFD Scale Factor!"<<endl;
    return -115;
  }
  if(CFDDelay <0) {
    cout<<"Set CFD Delay!"<<endl;
    return -116;
  } 
  
  
  if(trace.size() > 0) {
    
    //Calculate the CFD Response
    for(m=0; m<(CFDDelay+TFLength+TFGap+1); m++) {
      CFDOut.push_back(0);
    }


    
    m=CFDDelay+2*TFLength+TFGap;


    for(A=(m-CFDDelay-2*TFLength-TFGap); A<(m-CFDDelay-TFGap-TFLength); A++){
      S3+=trace[A];
    }
    
    for(A=(m-CFDDelay-TFLength); A<(m-CFDDelay); A++) {
     S2+=trace[A];
    }  

    for(A=(m-2*TFLength-TFGap); A<(m-TFGap-TFLength); A++){
      S1+=trace[A];
    }

    for(A=m-TFLength; A<m; A++){
      S0+=trace[A];
    }


    for(m=(CFDDelay+2*TFLength+TFGap+1); m<(Int_t)(trace.size()); m++) {

      S3=S3-trace[m-CFDDelay-2*TFLength-TFGap-1]+trace[m-CFDDelay-TFGap-TFLength-1];
      S2=S2-trace[m-CFDDelay-TFLength-1]+trace[m-CFDDelay-1];
      S1=S1-trace[m-2*TFLength-TFGap-1]+trace[m-TFGap-TFLength-1];
      S0=S0-trace[m-TFLength-1]+trace[m-1];
      

      //      Int_t trash;cin>>trash;
      
      Double_t cfdvalue2 = CFDWeight*(S0-S1)-(S2-S3);
      CFDOut.push_back(cfdvalue2);     
    }

    



    
  //   //Find the CFD over threshold point
  //   for(m=0; m<(Int_t)trace.size(); m++) {
  //     if(CFDOut[m-1]<=CFDThreshold && CFDOut[m]>CFDThreshold) {
  // 	cfdtrigpoint = m-1;
  // 	break;
  //     }
  //   }
    
  //   //Find the zero crossing after threshold and calculate time stamp
  //   for(m=cfdtrigpoint; m<(Int_t)trace.size(); m++) {
  //     if(CFDOut[m-1]>=0 && CFDOut[m]<0) {
  // 	CFDTime = CFDOut[m-1]/(CFDOut[m-1]+fabs(CFDOut[m]));
  // 	CFDTriggerPoint = m-1;
  // 	break;
  //     }
  //     if(m >= (cfdtrigpoint+31)) {
  // 	cout<<"Warning! CFD Faild to Zero Cross within 32 clock ticks of crossing the CFD Threshold!"<<endl;
  // 	CFDTime = 0;
  //     }
  //   }    
  }
  
  else {
    cout<<"Trace is length 0!"<<endl;
    return -105;
  }
  return 0;  
}




//Double_t LendaFilter::GetNewFirmwareCFD(vector<UShort_t> trace, Int_t FL, Int_t FG, Int_t d, Int_t w) {
vector <Double_t> LendaFilter::GetNewFirmwareCFD(const vector<UShort_t> & trace, Int_t FL, Int_t FG, Int_t d, Int_t w){

  vector<Double_t> CFDOut;
  if(trace.size() == 0) {
    CFDOut.resize(1,BAD_NUM);//Return a BAD CFD!
    return CFDOut;
  } else { //
  
  }



  Int_t TFLength=FL;//FL;
  Int_t TFGap=FG;//FG;
  Int_t CFDSF=0;//w;
  Int_t CFDDelay=d;

  Double_t CFDWeight=GetNewFirmwareCFDWeight(w);

  Int_t CFDThreshold=50;


  Int_t CFDTime=0;
  Int_t CFDTriggerPoint;


  Int_t cfdtrigpoint = -1;
  Int_t A=0;
  Int_t m=0;


  ///////////////////////////////////////////
  // There are 4 sums in the CFD algorithm //
  ///////////////////////////////////////////
  Double_t S0=0;
  Double_t S1=0;
  Double_t S2=0;
  Double_t S3=0;

  for(m=0; m<(CFDDelay+2*TFLength+TFGap+1); m++) {
    CFDOut.push_back(0);
  }

  // for(m=0; m<(CFDDelay+TFLength+TFGap+1); m++) {
  //   CFDOut.push_back(0);
  // }

  
    
  m=CFDDelay+2*TFLength+TFGap;

  for(A=(m-CFDDelay-2*TFLength-TFGap); A<(m-CFDDelay-TFGap-TFLength); A++){
    S3+=trace[A];
  }
    
  for(A=(m-CFDDelay-TFLength); A<(m-CFDDelay); A++) {
    S2+=trace[A];
  }  


  for(A=(m-2*TFLength-TFGap); A<(m-TFGap-TFLength); A++){
    S1+=trace[A];
  }


  for(A=m-TFLength; A<m; A++){
    S0+=trace[A];
  }

  
  //CFDOut.push_back(CFDWeight*(S0-S1)-(S2-S3));

  for(m=(CFDDelay+2*TFLength+TFGap+1); m<(Int_t)(trace.size()); m++) {

    //  CFDOut[m-1]= CFDWeight*(S0-S1)-(S2-S3);

     
    S3=S3-trace[m-CFDDelay-2*TFLength-TFGap-1]+trace[m-CFDDelay-TFGap-TFLength-1];
    S2=S2-trace[m-CFDDelay-TFLength-1]+trace[m-CFDDelay-1];
    S1=S1-trace[m-2*TFLength-TFGap-1]+trace[m-TFGap-TFLength-1];
    S0=S0-trace[m-TFLength-1]+trace[m-1];

    Double_t cfdvalue = CFDWeight*(S0-S1)-(S2-S3);

    //    Double_t cfdvalue=S0-S1;
    CFDOut.push_back(cfdvalue);     

    //CFDOut[m-1]=(S0-S1);

  }


  // for (int i=0;i<CFDOut.size();i++){
  //   cout<<CFDOut[i]<<endl;
  // }

  
  return CFDOut;

}



/*  
  //Find the CFD over threshold point
  for(m=0; m<(Int_t)trace.size(); m++) {
    if(CFDOut[m-1]<=CFDThreshold && CFDOut[m]>CFDThreshold) {
      cfdtrigpoint = m-1;
      break;
    }
  }
    
    //Find the zero crossing after threshold and calculate time stamp
    for(m=cfdtrigpoint; m<(Int_t)trace.size(); m++) {
      if(CFDOut[m-1]>=0 && CFDOut[m]<0) {
	CFDTime = CFDOut[m-1]/(CFDOut[m-1]+fabs(CFDOut[m]));
	CFDTriggerPoint = m-1;
	break;
      }
      if(m >= (cfdtrigpoint+31)) {
	cout<<"Warning! CFD Faild to Zero Cross within 32 clock ticks of crossing the CFD Threshold!"<<endl;
	CFDTime = 0;
      }
    }    
  }
  
  else {
    cout<<"Trace is length 0!"<<endl;
    return -105;
  }
  return 0;  
}
*/



Double_t LendaFilter::GetNewFirmwareCFDWeight(Int_t CFDScaleFactor){

  Double_t temp=(Double_t) CFDScaleFactor;
  return (8.0-temp)/8.0;  
}

// //Set the CFD SF and Weighting Factor based on the CFD Scale 

// Int_t LendaFilter::SetCFDSF(Int_t SF) {
  
//   // if(SF > CFDSFMax) {
//   //   cout<<"CFD Scale Factor is too long!"<<endl;
//   //   return -12;
//   // }  
//   // else {
//   //   CFDSF = SF;
//   //   CFDWeight = (double)(8.0-CFDSF)/8.0;
//   // }
//   return 0; 
// }
