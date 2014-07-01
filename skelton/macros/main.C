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

int main(TString OutPutName="temp.root")
{
  // Load libraries necessary to compile Analoop.cc
  gSystem->AddDynamicPath("/users/e10003/R00TLe/lib");
  gSystem->Load("libS800.so");
  gSystem->Load("libLendaEvent.so");

  gROOT->ProcessLine(".include /users/e10003/R00TLe/src/include");

  //gDebug=2;

  TChain* ch = new TChain("caltree");
  std::cout << "Creating TChain... ";

  // make a chain manually
  
  ch->Add("./rootfiles/run-0341-00.root");

  std::cout << "Done." << std::endl;
  //ch->Print();
  
  std::cout << "List of files:" << std::endl;
  ch->GetListOfFiles()->Print();
  TObjArray *fileElements = ch->GetListOfFiles();
  TIter next(fileElements);
  TChainElement *chEl=0;
  Int_t nfiles = 0;
  while (( chEl=(TChainElement*)next() )) {
    nfiles++;
    std::cout << "\t" << chEl->GetTitle() << std::endl;
  }

  //std::cout << ch->GetEntries() << " entries in ";
  //std::cout << nfiles << " runs in total.\n" << std::endl;
  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString src = install+"/users/"+user+"/src/Analoop.cc+O";
  ch->Process(src,"-g");

  //// store histograms 
  TString rootfilesdirname = install+"/users/"+user+"/histograms";
  TString outfilename = rootfilesdirname + "/"+OutPutName;
  TString writehistcommandstring = "WriteHist.C(\"" + outfilename + "\")";
  cout<<"Command is "<<writehistcommandstring<<endl;
  gROOT->Macro(writehistcommandstring);


  ch->Delete();
}
