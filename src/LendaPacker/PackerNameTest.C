



#include <iostream>
#include "LendaPacker.hh"
#include "LendaEvent.hh"
#include "DDASEvent.hh"
#include "TFile.h"
#include "TTree.h"
using namespace std;


void PrintInfo(LendaEvent* e){

  for (int i=0;i<e->Bars.size();i++){
    cout<<"Event has bar "<<e->Bars[i].Name<<endl;
    cout<<"It had "<<e->Bars[i].Tops.size()<<" Top firings"<<endl;
    cout<<"It had "<<e->Bars[i].Bottoms.size()<<" Bottom firings"<<endl;
    cout<<"It had average time of "<<e->Bars[i].GetAvgT()<<endl;
    cout<<"The name is "<<e->Bars[i].Name<<endl;
  }
  cout<<"********************"<<endl;
}

int main(){
  
  TFile * file = new TFile("NameTest.root","recreate");
  TTree * tree = new TTree("caltree","test");
  
  
  vector <UShort_t> aTrace1(1000,10);
  vector <UShort_t> aTrace2(1000,10);

  ddaschannel* chan1 = new ddaschannel;
  chan1->chanid = 0;
  chan1->slotid = 2;
  chan1->trace = aTrace1;

  ddaschannel* chan2 = new ddaschannel;
  chan2->chanid = 0;
  chan2->slotid = 3;
  chan2->trace = aTrace2;


  ddaschannel* chan3 = new ddaschannel;
  chan3->chanid = 4;
  chan3->slotid = 2;
  chan3->trace = aTrace1;

  ddaschannel* chan4 = new ddaschannel;
  chan4->chanid = 4;
  chan4->slotid = 3;
  chan4->trace = aTrace2;

  //A normal event with 2 top bottom pairs
  DDASEvent *ddasTest1 = new DDASEvent();
  ddasTest1->GetData().push_back(chan1);
  ddasTest1->GetData().push_back(chan2);
  ddasTest1->GetData().push_back(chan3);
  ddasTest1->GetData().push_back(chan4);
  

  LendaEvent * e = new LendaEvent();
  
  tree->Branch("lendaevent",&e);
	       

  LendaPacker thePacker;
  thePacker.SetFilter(6,0,6,0);
  thePacker.SetGates(10,20,10,20);

  
  thePacker.MakeLendaEvent(e,ddasTest1,0);
  e->Finalize();
  tree->Fill();
  PrintInfo(e);
  e->Clear();
  tree->Write();
  file->Close();
  
  
  return 0;

}
