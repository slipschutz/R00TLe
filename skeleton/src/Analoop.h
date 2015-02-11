#ifndef Analoop_h
#define Analoop_h
//////////////////////////////////////////////////////////
//
//    Analoop.h
//    Created       : 2012/08/30 22:17:02
//    Last Modified : 2014/05/22 14:18:53
//--------------------------------------------------------
//    Comment : Selector for analysis of e10003
//--------------------------------------------------------
//
//////////////////////////////////////////////////////////


//C++ Std library headers
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <signal.h>


//ROOT headers
#include <TROOT.h>
#include <TObject.h>
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TSelector.h>
#include <TH2.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TEnv.h>
#include <TCanvas.h>
#include <TCutG.h>
///////////////////


//Local From R00TLe Headers
#include "S800Calc.hh"
#include "LendaEvent.hh"
#include "R00TLeSettings.hh"


// for signal handling
Bool_t signal_received;

class Analoop : public TSelector {
public :

  /****************************************/
  /* This is where you declar histograms  */
  /****************************************/
  
  //Declar vectors to hold standard quantities for 
  //All of the Lenda Bars
  vector <TH1F*> AvgEnergies;
  vector <TH1F*> TopEnergies;
  vector <TH1F*> BottomEnergies;
  
  vector <TH1F*> TopTOFs;
  vector <TH1F*> BottomTOFs;
  vector <TH1F*> AvgTOFs;
  










///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////The casual user can ignore the vodoo magic below//////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

  // These variables are made global.
  S800Calc    *s800calc;
  LendaEvent  *lendaevent;
  

   TTree    *fChain;   //! pointer to the analyzed TTree or TChain
   // List of branches
   TBranch  *b_s800calc;
   TBranch  *b_lendaevent;

 Analoop(TTree * /*tree*/ = 0) : OutFileName("./DEFUALT.root")
     { }
   virtual ~Analoop();
   virtual Int_t   Version() const {
      return 2;
   }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) {
      return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0;
   }
   virtual void    SetOption(const char *option) {
      fOption = option;
   }
   virtual void    SetObject(TObject *obj) {
      fObject = obj;
   }
   virtual void    SetInputList(TList *input) {
      fInput = input;
   }
   virtual TList  *GetOutputList() const {
      return fOutput;
   }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   Long64_t nentries;
   Long64_t accum_nentries;
   Int_t treenum;
   Long64_t entrynum;
   TString filename;

   R00TLeSettings *TheSettings;

   TString OutFileName;

   //Histogramming methods with hisogram names 
   void MakeHistogram(TString name,Int_t bins,Double_t xlow,Double_t xhigh);
   void FillHistogram(TString name,Float_t value);

   void MakeHistogram(TString name,Int_t binsX,Double_t xlow,Double_t xhigh,Int_t binsY,Double_t yLow,Double_t yHigh);
   void FillHistogram(TString name,Float_t Xvalue,Float_t Yvalue);
   


   void AutoHisto(TString name,Float_t value,Int_t bins, Double_t xlow, Double_t xhigh);
   void AutoHisto(TString name,Float_t Xvalue,Float_t Yvalue,Int_t binsX, Double_t xlow, Double_t xhigh,Int_t binsY,Double_t yLow,Double_t yHigh);

   void AutoHisto(Int_t HistNumber,Float_t value,Int_t bins, Double_t xlow, Double_t xhigh);
   void AutoHisto(Int_t HistNumber,Float_t Xvalue,Float_t Yvalue,Int_t binsX, Double_t xlow, Double_t xhigh,Int_t binsY,Double_t yLow,Double_t yHigh);
   

   void Hist(Int_t HistNumber,Float_t value,Int_t bins, Double_t xlow, Double_t xhigh){
     AutoHisto(HistNumber,value,bins,xlow,xhigh);}
   void Hist(Int_t HistNumber,Float_t Xvalue,Float_t Yvalue,Int_t binsX, Double_t xlow, Double_t xhigh,Int_t binsY,Double_t ylow,Double_t yhigh){
     AutoHisto(HistNumber,Xvalue,Yvalue,binsX,xlow,xhigh,binsY,ylow,yhigh);
   }

   void Hist(TString name,Float_t value,Int_t bins, Double_t xlow, Double_t xhigh){
     AutoHisto(name,value,bins,xlow,xhigh);}
   void Hist(TString name,Float_t Xvalue,Float_t Yvalue,Int_t binsX, Double_t xlow, Double_t xhigh,Int_t binsY,Double_t ylow,Double_t yhigh){
     AutoHisto(name,Xvalue,Yvalue,binsX,xlow,xhigh,binsY,ylow,yhigh);
   }

   

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
   
 public:
   ClassDef(Analoop, 0);
};





// For signal handler
extern "C" {
   void signalhandler(Int_t sig)
   {
      if (sig == SIGINT) {
         signal_received = kTRUE;
      }
   }
}

// http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/                          
// Process has done i out of n rounds, and we want a bar of width w and resolution r.   
static inline void loadBar(Long64_t x, Long64_t n, Int_t resolution, Int_t width)
{
  // Only update r times.                                                                                               
  if (x % (n / resolution) != 0) return;
  // Calculuate the ratio of complete-to-incomplete.                                                                    
  Float_t ratio = x / (Float_t)n;
  Int_t   c     = ratio * width;
  // Show the percentage complete.                                                                                      
  std::cout << setw(5) << setiosflags(ios::fixed) << setprecision(1) << (Float_t)(ratio * 100.) << "% [";
  // Show the load bar.                                                                                                 
  for (Int_t i = 0; i < c;       i++) std::cout << "=";
  for (Int_t i = c; i < width - 1; i++) std::cout << " ";
  std::cout << "]";
  // dirty hack below                                                                                                   
  if (x == n - 1) {
    //    std::cout << std::endl << std::endl;                                                                          
  } else {
    std::cout << "\r" << std::flush;
  }
}


#endif // #ifndef Analoop_h
