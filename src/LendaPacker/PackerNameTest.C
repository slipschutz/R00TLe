



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
    
    cout<<"The top channels name/ID is "<<e->Bars[i].Tops[0].GetChannelName()<<" "<<e->Bars[i].Tops[0].GetGlobalID()<<endl;
    cout<<"The top Ref channels name/ID is "<<e->Bars[i].Tops[0].GetReferenceChannelName()<<" "<<e->Bars[i].Tops[0].GetReferenceGlobalID()<<endl;
    cout<<"The reference time is "<<e->Bars[i].Tops[0].GetReferenceTime()<<endl; 
    cout<<"The Bottom channels name/ID is "<<e->Bars[i].Bottoms[0].GetChannelName()<<" "<<e->Bars[i].Bottoms[0].GetGlobalID()<<endl;
    cout<<"The Bottom Ref channels name/ID is "<<e->Bars[i].Bottoms[0].GetReferenceChannelName()<<" "<<e->Bars[i].Bottoms[0].GetReferenceGlobalID()<<endl;
    cout<<"The reference time is "<<e->Bars[i].Bottoms[0].GetReferenceTime()<<endl;


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
  chan1->time=1;

  ddaschannel* chan2 = new ddaschannel;
  chan2->chanid = 0;
  chan2->slotid = 3;
  chan2->trace = aTrace2;
  chan2->time=2;


  ddaschannel* chan3 = new ddaschannel;
  chan3->chanid = 4;
  chan3->slotid = 2;
  chan3->trace = aTrace1;
  chan3->time=3;


  ddaschannel* chan4 = new ddaschannel;
  chan4->chanid = 4;
  chan4->slotid = 3;
  chan4->trace = aTrace2;
  chan4->time=4;


  ddaschannel *obj1 = new ddaschannel;
  obj1->chanid =15;
  obj1->slotid=2;
  obj1->time = 100;

  ddaschannel *obj2 = new ddaschannel;
  obj2->chanid =15;
  obj2->slotid=3;
  obj2->time=200;

  //A normal event with 2 top bottom pairs and one object pair
  DDASEvent *ddasTest1 = new DDASEvent();
  ddasTest1->GetData().push_back(chan1);
  ddasTest1->GetData().push_back(chan2);
  ddasTest1->GetData().push_back(chan3);
  ddasTest1->GetData().push_back(chan4);
  ddasTest1->GetData().push_back(obj1);
  ddasTest1->GetData().push_back(obj2);

  LendaEvent * e = new LendaEvent();
  
  tree->Branch("lendaevent",&e);
	       

  LendaPacker thePacker;
  thePacker.SetFilter(6,0,6,0);
  thePacker.SetGates(10,20,10,20);

  cout<<"\n\n\n\n"<<endl;
  thePacker.MakeLendaEvent(e,ddasTest1,0);
  e->Finalize();
  tree->Fill();
  PrintInfo(e);
  e->Clear();
  tree->Write();
  file->Close();
  
  
  return 0;

}
