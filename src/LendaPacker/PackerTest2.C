



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
    //e->Bars[i].Dump();
  }
  cout<<"********************"<<endl;
}

int main(){
  
  DDASEvent * test4 = new DDASEvent();
  ddaschannel *c1=new ddaschannel();
  ddaschannel *c2=new ddaschannel();

  c1->slotid=3; c1->chanid=3;
  c2->slotid=7; c2->chanid=3;

  c1->time=100;
  c2->time =102;

  ddaschannel *c3=new ddaschannel();
  ddaschannel *c4=new ddaschannel();

  c3->slotid=6; c3->chanid=4;
  c4->slotid=7; c4->chanid=4;
  
  c3->time=500;
  c4->time=501;
  
  test4->GetData().push_back(c1);
  test4->GetData().push_back(c2);
  test4->GetData().push_back(c3);
  test4->GetData().push_back(c4);

  LendaEvent * e = new LendaEvent();
  


  LendaPacker thePacker;
  thePacker.SetFilter(6,0,6,0);
  thePacker.SetGates(10,20,10,20);

  thePacker.MakeLendaEvent(e,test4,0);
  e->Finalize();
  PrintInfo(e);
  
  return 0;

}
