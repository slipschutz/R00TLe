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


  
  //////////////////////////////////////////////////////////////////
  // Look for a settings object in the Input list to the selector //
  // if it is not there then all is lost			  //
  //////////////////////////////////////////////////////////////////

  TheSettings = (R00TLeSettings*) fInput->FindObject("Settings0");

  //  TheSettings=(R00TLeSettings*)gDirectory->Get("TheSettings");
  if (TheSettings != NULL){
    cout<<"Found A R00TLeSettings Object!"<<endl;
  }else{
    cout<<"Could Not Find a R00TLeSettings Object.  Hard Exit"<<endl;
    exit(1);
  }

  PreviousTreeNumber=-9999;//Set tree number to an intial bad value

  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString CorrectionsFileName = install+"/users/"+user+"/AllTheCorrections.txt";


  MyCorrections = new TEnv(CorrectionsFileName);
  
  CurrentRunNumber =-1;
  
  TString S800MapFile= install+"/users/"+user+"/invmap.inv";
  loadInverseMap(S800MapFile,&MapA,&MapY,&MapB,&MapD);

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

  // ///////////////////////////////
  // // new more histograms here  //
  ///////////////////////////////
  


  // Add all the histograms to Output list of the Selector
  // fOutput is a TSelectorList which will take ownership of
  // an object when it is added
  fOutput->AddAll(gDirectory->GetList());
}




//////////////////////////////////////////////////////////////////
// This is where you fill histograms and make cuts on the data. //
// Process is called for every entry in the ROOT tree	        //
// 							        //
// The tree contains two branches s800calc and lendaevent       //
//////////////////////////////////////////////////////////////////
Bool_t Analoop::Process(Long64_t entry) {
  //  cout<<"IN PROCESS "<<entry<<endl;
  fChain->GetTree()->GetEntry(entry);
  
  if (signal_received) {
    Info(__FUNCTION__, "At the entry No. %lld", entry);
    this->Abort("Signal received.", kAbortProcess);
  }

  ///////////////////////////////////////////////////////////////////////
  // The following catch is to figure out if the file has been changed //
  // since the last time time process was called.  PROOF will process  //
  // the files in an arbitrary order so one needs to check	       //
  ///////////////////////////////////////////////////////////////////////
 if (fChain->GetTreeNumber() != PreviousTreeNumber){
    PreviousTreeNumber= fChain->GetTreeNumber();

    TString temp=fChain->GetCurrentFile()->GetName();
    string name = temp.Data(); 

    int index = name.find("run-");
    name=  name.substr(index+4,4);
    int RunNum=atoi(name.c_str());

    CurrentRunNumber=RunNum;
  } 


  //Nice load Bar feature from S.Noji
  loadBar(entry, nentries, 1000, 50);
  
                                 /////////////////////////
                                 // Begin s800 analysis //
                                 /////////////////////////

  double IC = s800calc->GetIC()->GetSum();
  double TOF=0;
  if (s800calc->GetMultiHitTOF()->fObj.size() !=0){
    TOF=s800calc->GetMultiHitTOF()->GetFirstObjHit();
  }

  TOF+=1000;//Make the TOF a more reasonable Positive number
  Hist("PID",TOF,IC,8000,0,3000,1000,0,2000);


  //Extract CRDC information
  Float_t crdc1Xcog = s800calc->GetCRDC(0)->GetXcog();
  Float_t crdc2Xcog = s800calc->GetCRDC(1)->GetXcog();
  
  Float_t crdc1TAC =  s800calc->GetCRDC(0)->GetTAC();
  Float_t crdc2TAC =  s800calc->GetCRDC(1)->GetTAC();

  //Get the mask calibrations for the current run
  Double_t Crdc1XOffset = MyCorrections->GetValue(Form("run%04d.crdc1.xOffset",CurrentRunNumber),0.0);
  Double_t Crdc1YOffset = MyCorrections->GetValue(Form("run%04d.crdc1.yOffset",CurrentRunNumber),0.0);
  Double_t Crdc1YSlope = MyCorrections->GetValue(Form("run%04d.crdc1.ySlope",CurrentRunNumber),0.0);
  
  Double_t Crdc2XOffset = MyCorrections->GetValue(Form("run%04d.crdc2.xOffset",CurrentRunNumber),0.0);
  Double_t Crdc2YOffset = MyCorrections->GetValue(Form("run%04d.crdc2.yOffset",CurrentRunNumber),0.0);
  Double_t Crdc2YSlope = MyCorrections->GetValue(Form("run%04d.crdc2.ySlope",CurrentRunNumber),0.0);

  //Form the calibrated CRDC information
  Float_t xfp =crdc1Xcog*2.54 + Crdc1XOffset; 
  Float_t yfp =crdc1TAC *Crdc1YSlope + Crdc1YOffset;

  Float_t x_crdc2 = crdc2Xcog*2.54 +Crdc2XOffset;
  Float_t y_crdc2 = crdc2TAC*Crdc2YSlope +Crdc2YOffset;

  Float_t afp = TMath::ATan((x_crdc2 - xfp)/1073.0);
  Float_t bfp = TMath::ATan((y_crdc2-yfp)/1073.0);

  //Prefom the ray tracing
  //The ray tracing routine needs input in meters and radian
  Float_t ata,yta,bta,dta;
  ata=yta=bta=dta=0;
  Raytracing(MapA,MapY,MapB,MapD, xfp/1000. ,afp ,yfp/1000, bfp ,ata,yta,bta,dta);
  //Returns in Meters and raidian
  //Put the angles in degrees
  //Put yta in mm
  //put Di in percent
  ata*=(180./3.14159265);
  bta*=(180./3.14159265);
  yta*=1000;
  dta*=100;

  //Put in Degrees
  afp*=(180./3.14159265);
  bfp*=(180./3.14159265);

  Hist("afp",afp,1000,-100,100);
  Hist("bfp",bfp,1000,-100,100); 
  Hist("xfp",xfp,1000,-400,400);
  Hist("yfp",yfp,1000,-400,400);
    
  Hist("dta",dta,1000,-100,100);
  Hist("ata",ata,1000,-100,100);
  Hist("bta",bta,1000,-100,100);
  Hist("yta",yta,1000,-100,100);
  


  
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
  if (fChain->GetCurrentFile() != NULL){
    
    filename = fChain->GetCurrentFile()->GetName();

    Info("Notify","File: %s (%7.2f MB)", filename.Data(), fChain->GetTree()->GetTotBytes()/1024./1024.);

    treenum = fChain->GetTreeNumber() + 1;
    entrynum = fChain->GetChainEntryNumber(0);

    nentries = fChain->GetTree()->GetEntries();
    accum_nentries += nentries;
    Info("Notify","%lld entries (%6.2f%%) in this tree",nentries, (double)nentries / fChain->GetEntries() * 100.);
    Info("Notify","%lld entries (%6.2f%%) up to this tree",accum_nentries, (double)accum_nentries / fChain->GetEntries() * 100.);
  }
  return kTRUE;
}

void Analoop::Begin(TTree *) {
  TString option = GetOption();

  OutFileName=option;
}


void Analoop::SlaveTerminate() {

}

void Analoop::Terminate() {

  TheSettings = (R00TLeSettings*) fInput->FindObject("Settings0");

  cout<<"\n Writing histograms to "<<OutFileName<<endl;
  TFile out(OutFileName,"recreate");
  fOutput->Write();//Write all the histograms to disk
  TheSettings->Write();

  out.Close();

  this->ResetAbort();
  signal_received = kFALSE;
}



/**Make a 1D histogram with a Name

 */
void Analoop::MakeHistogram(TString name,Int_t bins,Double_t xlow,Double_t xhigh){
  
  fOutput->AddLast( new TH1F(name,name,bins,xlow,xhigh));

}

/**Fill a 1D histogram with a name

 */
void Analoop::FillHistogram(TString name,Float_t value){

  TObject * object = fOutput->FindObject(name);

  if (object == NULL){
    Error("Analoop::FillHistogram",name+" not found");
    return;
  }
  TString className=object->ClassName();
  if (className !="TH1F"){
    Error("Analoop::FillHistogram",name+" not a histogram");
  }

  ((TH1F*)object)->Fill(value);

}



/**Makea 2D histogram with a Name
 */
void Analoop::MakeHistogram(TString name,Int_t binsX,Double_t xlow,Double_t xhigh,Int_t binsY,Double_t yLow,Double_t yHigh){
  fOutput->AddLast( new TH2F(name,name,binsX,xlow,xhigh,binsY,yLow,yHigh));
}


/**Fill a 2D histogram with a Name
 */
void Analoop::FillHistogram(TString name,Float_t Xvalue,Float_t Yvalue){

  TObject * object = fOutput->FindObject(name);
  
  if (object == NULL){
    Error("Analoop::FillHistogram",name+" not found");
    return;
  }
  TString className=object->ClassName();
  if (className !="TH2F"){
    Error("Analoop::FillHistogram",name+" not a histogram");
  }

  ((TH2F*)object)->Fill(Xvalue,Yvalue);

}




void Analoop::AutoHisto(TString name,Float_t value,Int_t bins, Double_t xlow, Double_t xhigh){
  
  TObject * object = fOutput->FindObject(name);
  if ( object == NULL) {//The histogram is not there 
    MakeHistogram(name,bins,xlow,xhigh);
  }
  FillHistogram(name,value);
}

void Analoop::AutoHisto(TString name,Float_t Xvalue,Float_t Yvalue,Int_t binsX, Double_t xlow, Double_t xhigh,Int_t binsY,Double_t yLow,Double_t yHigh){

  TObject * object = fOutput->FindObject(name);
  if ( object == NULL) {//The histogram is not there 
    MakeHistogram(name,binsX,xlow,xhigh,binsY,yLow,yHigh);
  }
  FillHistogram(name,Xvalue,Yvalue);
  
}


void Analoop::AutoHisto(Int_t HistNumber,Float_t value,Int_t bins, Double_t xlow, Double_t xhigh){
  stringstream s;
  s<<"h"<<HistNumber;
  AutoHisto(s.str().c_str(),value,bins,xlow,xhigh);

}
void Analoop::AutoHisto(Int_t HistNumber,Float_t Xvalue,Float_t Yvalue,Int_t binsX, Double_t xlow, Double_t xhigh,Int_t binsY,Double_t ylow,Double_t yhigh){
  stringstream s;
  s<<"h"<<HistNumber;
  AutoHisto(s.str().c_str(),Xvalue,Yvalue,binsX,xlow,xhigh,binsY,ylow,yhigh);

}


