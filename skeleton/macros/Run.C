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




TChain* ch;//Make Global so it can be seen in both versions of proof function

void Run(TString InputFileName,TString OutPutName="histograms/temp.root"){
  
  ch = new TChain("caltree");
  std::cout << "Creating TChain... ";
  ch->Add(InputFileName);
  
  Run(-99,OutPutName);
  
}


int Run(Int_t runNum=-1,TString OutPutName="histograms/temp.root")
{
    
  if (runNum != -99){
    ch = new TChain("caltree");
    std::cout << "Creating TChain... ";
    if (runNum ==-1){
      ch->Add("./rootfiles/run-0573-??.root");//Defualt run

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
  int FileSuccessCount=0;
  
  int count=0;
  while (( chEl=(TChainElement*)next() )) {
    TFile f(chEl->GetTitle());
    
    if (f.IsOpen()){
      FileSuccessCount++;
    }else {
      cout<<"Warning could not open "<<chEl->GetTitle()<<endl;
    }

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
    count++;
  }
  
  if ( FileSuccessCount == 0 ){
    cout<<"No files were successfully opened"<<endl;
    return 0;
  }
    

  
  std::cout << "\nList of files:" << std::endl;
  ch->GetListOfFiles()->Print();
  cout<<endl;

  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString src = install+"/users/"+user+"/src/Analoop.cc+g";


  ////////////////////////////////////////////////////////////////
  // magic line that runs the Analoop program                   //
  // over the chain you have specified			        //
  ////////////////////////////////////////////////////////////////


  
  TSelector* myAnaloop = TSelector::GetSelector(src);
  
  TList * list = new TList();
  list->Add(ListOfSettings[0]);
  myAnaloop->SetInputList(list);
  
  ch->Process(myAnaloop,OutPutName);

  


  delete list;
  delete ch; 
}
