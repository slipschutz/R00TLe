


#include "R00TLeSettings.hh"
#include "TH1F.h"
#include "TF1.h"
#include "TFitResult.h"

#include "TDirectory.h"

#include <iomanip>

#include <iostream>
#include "TString.h"
using namespace std;


/**This script is meant to be from within the ROOT interpreter.
   It assumes that there are TOF histograms in the current file that 
   follow the following naming convention
   BarName_HistoName
  
   where HistoName is an argument 
   ex.
   SL01_CutAvgTOF

   The second argument is the run number where there a corrections file
   containing offsets to align the TOP and Bottom PMTs
  
   This script will then fit the first peak (which should be gamma peak)
   with a guassian and print the offsets to the screen.  These offsets
   will include the offsets in corrections file RunNumberForCorrectionsFile
*/



double GetPeak(TH1F* Chan){
  double answer = -1000;
  
  if (Chan->GetEntries()>0){

    ///First try and find the first maximum starting from 
    //the left.  That should be the GAMMA PEAk
    int nbins = Chan->GetEntries();
    double maxVal=-1000;
    int theBin=-133;
    int count =0;
    for (int i =1;i<nbins;i++){
      double val = Chan->GetBinContent(i);
      if (val > 500){
	if (val > maxVal){
	  maxVal=val;
	  count=0;
	}
      }
      if (val < maxVal){
	theBin=i-1;
	break;
      }
    }
//////////////////////////////////////////////////////////////////
    
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

void FindTOFOffset(TString HistoName,Int_t RunNumberForCorrectionsFile){
 
  R00TLeSettings * settings;
  settings= (R00TLeSettings*)gDirectory->Get("Settings0");
  
  if (settings == NULL){
    cout<<"Could not find a R00TLeSettings Object. 
 \n Run this script from within a file with histograms and a settings object in it"<<endl;
    return;
  }
  
  int numBars =48;//settings->GetNumBars();
  cout<<"Num bars "<<numBars<<endl;
  
  LendaPacker aPack(new R00TLeSettings());
  aPack.FindAndSetMapAndCorrectionsFileNames(RunNumberForCorrectionsFile);
  
  int precLevel=5;
  for (int i=0;i<numBars;i++){
    TString BarName = settings->GetBarName(i);
  
    TString histoName = BarName+"_"+HistoName;

    double top = GetPeak( (TH1F*)gDirectory->Get(histoName));
    
    //double bottom= GetPeak((TH1F*)gDirectory->Get(BarName+"_BottomCutTOFPr"));
    MapInfo topInfo = aPack.GetMapInfo((BarName+"T").Data());
    MapInfo bottomInfo = aPack.GetMapInfo((BarName+"B").Data());
    
    cout<<BarName+"T"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<top+topInfo.TOFOffset<<endl;;
    cout<<BarName+"B"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<top+bottomInfo.TOFOffset<<endl;;
  }


  
}
