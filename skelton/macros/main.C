#ifndef __CINT__
#include <TROOT.h>
#include <TSystem.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TObjArray.h>
#include <TCollection.h>
#include <TChainElement.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#endif // __CINT__


int main(Int_t runNum=-1,TString OutPutName="temp.root")
{
  
  TChain* ch = new TChain("caltree");
  std::cout << "Creating TChain... ";
  
  if (runNum ==-1){
    // if no runNum give make a chain manually
    ch->Add("./rootfiles/run-0341-00.root");//Defualt run
  } else {
    stringstream ss;
    ss<<"./rootfiles/run-"<<setfill('0')<<setw(4)<<runNum<<"-00.root";
    ch->Add(ss.str().c_str());
    if (OutPutName == "temp.root"){
      ss.str("");
      ss<<"HistogramsFromRun"<<runNum<<".root";
      OutPutName=ss.str();      
    }
  }
  std::cout << "Done." << std::endl;

  
  std::cout << "List of files:" << std::endl;
  ch->GetListOfFiles()->Print();

  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString src = install+"/users/"+user+"/src/Analoop.cc+O";
  
  ////////////////////////////////////////////////////////////////
  // magic line that compiles and then runs the Analoop program //
  // over the chain you have specified			        //
  ////////////////////////////////////////////////////////////////
  ch->Process(src,"-g");

  

  //// store histograms 
  TString rootfilesdirname = install+"/users/"+user+"/histograms";
  TString outfilename = rootfilesdirname + "/"+OutPutName;
  cout<<"File will be saved in "<<outfilename<<endl;
  
  gROOT->ProcessLine(".L WriteHist.C");
  WriteHist(outfilename);

  
  ch->Delete();
}
