



#include <iostream>
#include "LendaPacker.hh"
#include "LendaEvent.hh"
#include "DDASEvent.hh"
using namespace std;


void PrintInfo(LendaEvent* e){

  for (int i=0;i<e->Bars.size();i++){
    cout<<"Event has bar "<<e->Bars[i].Name<<endl;
    cout<<"It had "<<e->Bars[i].Tops.size()<<" Top firings"<<endl;
    cout<<"It had "<<e->Bars[i].Bottoms.size()<<" Bottom firings"<<endl;
    cout<<"It had average time of "<<e->Bars[i].GetAvgT()<<endl;
    e->Bars[i].Dump();
  }
  cout<<"********************"<<endl;
}

int main(){
  
  
  
  
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

  //A normal event with 1 top bottom pair
  DDASEvent *ddasTest1 = new DDASEvent();

  ddasTest1->GetData().push_back(chan1);
  ddasTest1->GetData().push_back(chan2);


  //A event with two pmts from different bars
  DDASEvent *ddasTest2 = new DDASEvent();
  
  ddasTest2->GetData().push_back(chan1);
  ddasTest2->GetData().push_back(chan3);


  //A event with 2 tops and 1 bottom in one bar
  //and a top, bottom in another bar
  DDASEvent *ddasTest3 = new DDASEvent();
  
  ddasTest3->GetData().push_back(chan1);
  ddasTest3->GetData().push_back(chan2);
  ddasTest3->GetData().push_back(chan2);

  ddasTest3->GetData().push_back(chan3);
  ddasTest3->GetData().push_back(chan4);


  LendaEvent * e = new LendaEvent();
  


  LendaPacker thePacker;
  thePacker.SetFilter(6,0,6,0);
  thePacker.SetGates(10,20,10,20);

  
  thePacker.MakeLendaEvent(e,ddasTest1,0);
  PrintInfo(e);

  e->Clear();
  thePacker.MakeLendaEvent(e,ddasTest2,0);
  PrintInfo(e);
  e->Clear();

  cout<<endl<<endl;
  cout<<"Flowing event should have SL01 with 1 top and 2 bottoms"<<endl;
  cout<<"And it should have SL05 with 1 top and 1 bottom"<<endl;
  thePacker.MakeLendaEvent(e,ddasTest3,0);
  PrintInfo(e);
  e->Clear();
  
  
  return 0;

}
