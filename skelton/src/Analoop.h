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

#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <signal.h>

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
#include <TLorentzVector.h>
#include <TLorentzRotation.h>

#include "S800Calc.hh"
#include "LendaEvent.hh"

// forward declarations
class TObject;
class S800Calc;
class CRDC;
class IC;
class TOF;
class SCINT;
class TRACK;
class DDASEvent;
class LendaEvent;

// These variables are made global. Is it right way to do?
S800Calc    *s800calc;
LendaEvent  *lendaevent;

// for signal handling
Bool_t signal_received;

class Analoop : public TSelector {
public :
   TTree    *fChain;   //! pointer to the analyzed TTree or TChain
   // List of branches
   TBranch  *b_s800calc;
   TBranch  *b_lendaevent;

   Analoop(TTree * /*tree*/ = 0) { }
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

   
   /****************************************/
   /* This is where you declar histograms  */
   /****************************************/
   
   // declaration of variables for histograms
   // S800
   TH1I *htest;






   
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

#endif // #ifndef Analoop_h
