#define Analoop_cxx

#include "Analoop.h"

Analoop::~Analoop() {
  // Destructor
  Info(__FUNCTION__,"Destructing.");  
  delete s800calc;
  delete lendaevent;
}

void Analoop::Init(TTree *tree) {
  s800calc    = new S800Calc();
  lendaevent  = new LendaEvent();

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(0);
  fChain->SetBranchAddress("s800calc",    &s800calc,    &b_s800calc);
  fChain->SetBranchAddress("lendaevent",  &lendaevent,  &b_lendaevent);
}

Bool_t Analoop::Notify() {
  return kTRUE;
}

void Analoop::Begin(TTree *) {
  TString option = GetOption();
}


void Analoop::SlaveTerminate() {
}

void Analoop::Terminate() {
  TIter next_object(fOutput);
  TObject* obj;
  while ((obj = next_object())) { 
    obj->Clone();
    fOutput->RecursiveRemove(obj);
  }

  this->ResetAbort();
  signal_received = kFALSE;
}






void Analoop::SlaveBegin(TTree *) {
  TString option = GetOption();

  // Set signal handler
  signal_received = kFALSE;
  signal(SIGINT, signalhandler);



  /////////////////////////////////////////////////
  // this is where you new the histogram objects //
  /////////////////////////////////////////////////

  htest = new TH1I("htest", "htest", 200, -1000., 1000.);

  // Add all the histograms to Output list of the Selector
  fOutput->AddAll(gDirectory->GetList());
}





//////////////////////////////////////////////////////////////////
// This is where you fill histograms and make cuts on the data. //
// Process is called for every entry in the ROOT tree or chain  //
// 							        //
// The tree contains two branches s800calc and lendaevent       //
//////////////////////////////////////////////////////////////////

Bool_t Analoop::Process(Long64_t entry) {
  fChain->GetTree()->GetEntry(entry);
  
  if (signal_received) {
    Info(__FUNCTION__, "At the entry No. %lld", entry);
    this->Abort("Signal received.", kAbortProcess);
  }

  // retrieve variables, process them, and fill histograms with them, etc. 
  if (entry % 10000 ==0 ){
    std::cout <<"Entry in current tree "<< entry << std::endl;
    
  }
  // example...
  htest->Fill(s800calc->GetCRDC(0)->GetX());

  return kTRUE;
}


