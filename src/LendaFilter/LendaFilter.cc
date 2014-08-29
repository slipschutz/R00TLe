
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






#include <map>

Double_t LendaFilter::GetZeroCubic(std::vector <Double_t> & CFD){
  

  std::map <double,int> zeroCrossings;
  double max=0;

  int begin = (CFD.size()/2)-40;
  int end = (CFD.size()/2)+40;

  for (int i =begin;i<end;i++){
    if (CFD[i]>0 && CFD[i+1]<0){
      double val = CFD[i] - CFD[i+1];
      if ( val > max)
	max = val;
      //put this crossing in map
      zeroCrossings[val]=i;
    }
  }
  
  int theSpotAbove = zeroCrossings[max];

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

Double_t LendaFilter::GetZeroFitCubic(std::vector <Double_t> & CFD){
  

  //Find the largest zero Crossing
  std::map <double,int> zeroCrossings;
  double max=0;
  
  //restrict search to center part of tace
  int begin = (CFD.size()/2)-40;
  int end = (CFD.size()/2)+40;

  for (int i =begin;i<end;i++){
    if (CFD[i]>0 && CFD[i+1]<0){
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


  for ( int i=0 ;i<LengthForBackGround;i++){
    sumBegin = sumBegin + trace[i];
    sumEnd = sumEnd + trace[traceLength-1-i];
  }
  sumBegin = sumBegin/LengthForBackGround;
  sumEnd = sumEnd/LengthForBackGround;

  Int_t windowForEnergy=20;

  for (int i=MaxSpot-windowForEnergy;i< MaxSpot+windowForEnergy;++i) {
    signalIntegral = trace[i]+ signalIntegral;
  }

  if (  signalIntegral - sumBegin *(2*windowForEnergy>0)){
    thisEventsIntegral = signalIntegral - sumBegin *(2*windowForEnergy);
  }  else{
    thisEventsIntegral = BAD_NUM;
  }


  return thisEventsIntegral;

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
    return (MaxSpot-4);
}
