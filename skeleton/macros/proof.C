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


void proof(Int_t runNum=-1,Int_t NumCores=10,TString OutPutName="histograms/temp.root")
{

  ////////////////////////////////////////////////////////////
  // open the root files and put the together into a TChain //
  ////////////////////////////////////////////////////////////
  
  TChain* ch = new TChain("caltree");
  std::cout << "Creating TChain... ";
  
  if (runNum ==-1){
    // if no runNum give make a chain manually
    ch->Add("./rootfiles/run-0341-00.root");//Defualt run
  } else {
    stringstream ss;
    ss<<"./rootfiles/run-"<<setfill('0')<<setw(4)<<runNum<<"-??.root";
    ch->Add(ss.str().c_str());
    if (OutPutName == "histograms/temp.root"){
      ss.str("");
      ss<<"histograms/HistogramsFromRun"<<runNum<<".root";
      OutPutName=ss.str();      
    }
  }
  std::cout << "Done." << std::endl;


  
  ///////////////////////////////////////////////////////////////
  // look for settings objects in each of the files 	       //
  // in the chain.  Set their names as settings0 1 2...	       //
  // for saving in the final histogram file	  	       //
  ///////////////////////////////////////////////////////////////

  vector <R00TLeSettings *> ListOfSettings;
  TObjArray *fileElements=ch->GetListOfFiles();
  TIter next(fileElements);
  TChainElement *chEl=0;
  int count=0;
  while (( chEl=(TChainElement*)next() )) {
    TFile f(chEl->GetTitle());
    if ( f.GetListOfKeys()->Contains("TheSettings")) {
      stringstream ss;
      ss<<"Settings"<<count;
      TString newName = ss.str().c_str();
      R00TLeSettings *temp = (R00TLeSettings*)f.Get("TheSettings");
      temp->SetName(newName);

      ListOfSettings.push_back(temp);
    } else {
      cout<<"No Seetings object found in "<<chEl->GetTitle()<<endl;
      exit(1);
    }
    count++;//Counter for settings Names 
  }


  
  std::cout << "List of files:" << std::endl;
  ch->GetListOfFiles()->Print();



  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString src = install+"/users/"+user+"/src/Analoop.cc+g";
  TString soFile=install+"/users/"+user+"/src/Analoop_cc.so";

  /////////////////////////////////////////
  // compile and load Analoop in to ROOT //
  /////////////////////////////////////////
  gROOT->ProcessLine(".L " +src);

  ////////////////////////////////////////
  // Set up the PROOF Sandbox directory //
  ////////////////////////////////////////
  TString sandbox = install + "/users/"+user+"/PROOF/";
  gEnv->SetValue("ProofLite.SandBox",sandbox);


  /////////////////
  // start PROOF //
  /////////////////

  std::stringstream ss; ss<<NumCores;
  TString workerString = "workers=" + TString(ss.str().c_str());
  TProof * p =TProof::Open(workerString);


  p->SetParameter("PROOF_UseTreeCache",1);
  p->SetParameter("PROOF_CacheSize",1024.);

  //////////////////////////////////////////////////////
  // load the analoop library on each worker thread   //
  //////////////////////////////////////////////////////
  TString so_cmd = "gSystem->Load(\"" +soFile+ "\")";
  p->Exec(so_cmd);



  
  /////////////////////////////////////////////////////////////
  // make instance of analoop that will be passed into PROOF //
  /////////////////////////////////////////////////////////////
  
  TSelector* myAnaloop = TSelector::GetSelector(src);

   
  //////////////////////////////////////////////////////////////
  // Put the settings object from the first file in the input //
  // list of the selector				      //
  //////////////////////////////////////////////////////////////
  
  TList * list = new TList();
  list->Add(ListOfSettings[0]);
  myAnaloop->SetInputList(list);

  ////////////////////////////////////////////////////////////////
  // Magic lines that loop over the tchain and analyzes it with //
  // the analoop program using PROOF on different worker        //
  // nodes						        //
  ////////////////////////////////////////////////////////////////

  ch->SetProof(kTRUE);
  ch->SetCacheSize(1024.);
  ch->AddBranchToCache("*",kTRUE);

  ch->Process(myAnaloop,OutPutName);//Deep Magicks 

  


  //Preform Clean up for PROOF
  p->CleanupSession(gProof->GetSessionTag());
  p->ClearCache();
  p->Close();

  
  delete ch;

}
