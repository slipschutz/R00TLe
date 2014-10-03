#define Analoop_cxx

#include "Analoop.h"


/////////////////////////////////////////////////
// This method is called once at the beginning //
// it is where histograms are allocated        //
// ie when new is called for them	       //
/////////////////////////////////////////////////
void Analoop::SlaveBegin(TTree * t) {
  
  TString option = GetOption();

  // Set signal handler
  signal_received = kFALSE;
  signal(SIGINT, signalhandler);


  ///////////////////////////////////////////////////////////////////
  // Get the settings object from the file that the TChain is from //
  ///////////////////////////////////////////////////////////////////
  if (t->GetCurrentFile()->GetListOfKeys()->Contains("TheSettings")){
    TheSettings=(R00TLeSettings*)t->GetCurrentFile()->Get("TheSettings");
  } else {
    cout<<"No Settings object found.  Exiting..."<<endl;
    exit(1);
    return;
  }




  ////////////////////////////////////////////////////////////////////
  // make histograms for standard quantities (TOFs and PulsHeights) //
  ////////////////////////////////////////////////////////////////////

  Int_t NumBars =-1;
  NumBars = TheSettings->GetNumBars();//The number of bars in this file from TheSettings
  
  //Resize the vectors for the standard quanties for LendaBars
  AvgEnergies.resize(NumBars);
  AvgTOFs.resize(NumBars);
  TopTOFs.resize(NumBars);
  BottomTOFs.resize(NumBars);
  BottomEnergies.resize(NumBars);
  TopEnergies.resize(NumBars);

  //Binning of the TOF and energy histograms 
  int EBins=5000; 
  int EMax=100000;
  int TOFBins=2000;
  int TOFMinMax=100;

  //For each Bar in the file make these standar histograms
  stringstream nameStream;
  for (int i=0;i<NumBars;i++){
     nameStream.str("");
     nameStream<<TheSettings->GetBarName(i)<<"_AvgE";
     AvgEnergies[i] = new TH1F(nameStream.str().c_str(),nameStream.str().c_str(),EBins,0,EMax);

     nameStream.str("");
     nameStream<<TheSettings->GetBarName(i)<<"_TopE";
     TopEnergies[i] = new TH1F(nameStream.str().c_str(),nameStream.str().c_str(),EBins,0,EMax);

     nameStream.str("");
     nameStream<<TheSettings->GetBarName(i)<<"_BottomE";
     BottomEnergies[i] = new TH1F(nameStream.str().c_str(),nameStream.str().c_str(),EBins,0,EMax);

     nameStream.str("");
     nameStream<<TheSettings->GetBarName(i)<<"_TopTOF";
     TopTOFs[i] = new TH1F(nameStream.str().c_str(),nameStream.str().c_str(),TOFBins,-TOFMinMax,TOFMinMax);

     nameStream.str("");
     nameStream<<TheSettings->GetBarName(i)<<"_BottomTOF";
     BottomTOFs[i] = new TH1F(nameStream.str().c_str(),nameStream.str().c_str(),TOFBins,-TOFMinMax,TOFMinMax);

     nameStream.str("");
     nameStream<<TheSettings->GetBarName(i)<<"_AvgTOF";
     AvgTOFs[i] = new TH1F(nameStream.str().c_str(),nameStream.str().c_str(),TOFBins,-TOFMinMax,TOFMinMax);
     
  }

  ///////////////////////////////
  // new more histograms here  //
  ///////////////////////////////
  



  // Add all the histograms to Output list of the Selector
  fOutput->AddAll(gDirectory->GetList());
}




//////////////////////////////////////////////////////////////////
// This is where you fill histograms and make cuts on the data. //
// Process is called for every entry in the ROOT tree	        //
// 							        //
// The tree contains two branches s800calc and lendaevent       //
//////////////////////////////////////////////////////////////////
Bool_t Analoop::Process(Long64_t entry) {
  fChain->GetTree()->GetEntry(entry);
  
  if (signal_received) {
    Info(__FUNCTION__, "At the entry No. %lld", entry);
    this->Abort("Signal received.", kAbortProcess);
  }


  //Nice load Bar feature from S.Noji
  loadBar(entry, nentries, 1000, 50);


  
  //////////////////////////////////////////////////////
  // Each lendaevent has variable number of bars      //
  // Each Bar has varibale number of tops and bottoms //
  // Loop over all of them below                      //
  //////////////////////////////////////////////////////
  
  int NumBarsInEvent= lendaevent->NumBars; // Get them number of bars in the event
  
  for (int i =0;i<NumBarsInEvent;i++){ //Loop over all Bars in the Lenda event
    
    int NumTopsInBar = lendaevent->Bars[i].NumTops; //Number of tops in ith bar
    int NumBottomsInBar = lendaevent->Bars[i].NumBottoms;//Number of bottoms in ith bar
    int BarId = lendaevent->Bars[i].BarId; //The ith Bars Unique Bar Id

    for (int t=0;t<NumTopsInBar;t++){ // Loop over all the TOPS in the Bar
      double E=lendaevent->Bars[i].Tops[t].GetEnergy();
      if (E>0){
	TopEnergies[BarId]->Fill(E); //Fill Top Energy if >0
      }
    }//End for over tops
    
    for (int b=0;b<NumBottomsInBar;b++){//Loop over all the Bottoms in the Bar
      double E=lendaevent->Bars[i].Bottoms[b].GetEnergy();
      if (E>0){
	BottomEnergies[BarId]->Fill(E); //Fill Bottom Energy if >0
      }
    }//End for over bottoms


    //Fill histograms that are only valid when there was 1 top and 1 bottom
    if (lendaevent->Bars[i].SimpleEventBit){//has 1 top and 1 bottom
      if (lendaevent->Bars[i].Tops[0].GetEnergy() > 0 && lendaevent->Bars[i].Bottoms[0].GetEnergy()>0){
	AvgEnergies[BarId]->Fill(lendaevent->Bars[i].GetAvgEnergy());
      }
      AvgTOFs[BarId]->Fill(lendaevent->Bars[i].GetAvgTOF());
    }
    
    if ( NumTopsInBar == 1 ){
      TopTOFs[BarId]->Fill(lendaevent->Bars[i].GetTopTOF());
    }
    if (NumBottomsInBar == 1 ){
      BottomTOFs[BarId]->Fill(lendaevent->Bars[i].GetBottomTOF());
    }
  }//End for over bars in event
  
  

  
  
  return kTRUE;
}








///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////The casual user can ignore the vodoo magic below//////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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

  accum_nentries = 0;
}

Bool_t Analoop::Notify() {
  filename = fChain->GetCurrentFile()->GetName();
  Info(__FUNCTION__,"File: %s (%7.2f MB)",
       filename.Data(),
       fChain->GetTree()->GetTotBytes()/1024./1024.);
  treenum = fChain->GetTreeNumber() + 1;
  entrynum = fChain->GetChainEntryNumber(0);

  nentries = fChain->GetTree()->GetEntries();
  accum_nentries += nentries;
  Info(__FUNCTION__,"%lld entries (%6.2f%%) in this tree",
       nentries, (double)nentries / fChain->GetEntries() * 100.);
  Info(__FUNCTION__,"%lld entries (%6.2f%%) up to this tree",
       accum_nentries, (double)accum_nentries / fChain->GetEntries() * 100.);

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
