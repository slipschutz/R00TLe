


#include "R00TLeSettings.hh"
#include "TH1F.h"
#include "TF1.h"
#include "TFitResult.h"

#include "TDirectory.h"

#include <iomanip>

#include <iostream>
#include "TString.h"
using namespace std;




double GetPeak(TH1F* Chan){
  double answer = -1000;
  
  if (Chan->GetEntries()>0){
      

    int nbins = Chan->GetEntries();
    double maxVal=0;
    int theBin=-133;
    for (int i =1;i<nbins;i++){
      double val = Chan->GetBinContent(i);
      if (val > 100){
	if (val > maxVal){
	  maxVal=val;
	} else if ( val < maxVal){
	  // the previous bin was the one
	  theBin=i-1;
	  break;
	}

      }
      
    }
    //    theBin=Chan->GetMaximumBin();
    Double_t BinCenter =Chan->GetBinCenter(theBin);
    
    TF1 * Func = new TF1("Func","gaus",BinCenter-0.15,BinCenter+0.15);
    
    TFitResultPtr result=Chan->Fit("Func","QSR");
    Int_t status1 = result;
    
    if (status1==0 ){
      
      answer= result->Value(1);
    }
    
  }
  return answer;
}

void JitterFinder2(){
  cout<<"HELLo"<<endl;
  R00TLeSettings * settings = (R00TLeSettings*)gDirectory->Get("Settings0");

  cout<<"HELLo"<<endl;
  int numBars =48;//settings->GetNumBars();
  cout<<"HELLo"<<endl;
  
  
  int precLevel=5;
  for (int i=0;i<numBars;i++){
    TString BarName = settings->GetBarName(i);
    //    cout<<"BarName "<<BarName<<endl;
    
    double top = GetPeak( (TH1F*)gDirectory->Get(BarName+"_AvgPIDCutTrueTOF"));
    
    //double bottom= GetPeak((TH1F*)gDirectory->Get(BarName+"_BottomCutTOFPr"));
    
    cout<<BarName+"T"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<top<<endl;;
    cout<<BarName+"B"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<top<<endl;;
  }


  
}
