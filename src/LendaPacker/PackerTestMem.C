



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

  c1->slotid=2; c1->chanid=3;
  c2->slotid=3; c2->chanid=3;

  c1->time=100;
  c2->time =102;

  ddaschannel *obj1 = new ddaschannel;
  obj1->chanid =15;
  obj1->slotid=2;
  obj1->time = 100;

  ddaschannel *obj2 = new ddaschannel;
  obj2->chanid =15;
  obj2->slotid=3;
  obj2->time=200;



  vector <UShort_t> fakeTrace1(248,300);
  vector <UShort_t> fakeTrace2(248,300);

  fakeTrace1[120]=310;
  fakeTrace1[121]=350;
  fakeTrace1[122]=500;
  fakeTrace1[123]=480;
  fakeTrace1[124]=400;
  fakeTrace1[125]=380;
  fakeTrace1[125]=320;
  fakeTrace1[126]=310;
    


  fakeTrace2[120]=340;
  fakeTrace2[121]=390;
  fakeTrace2[122]=470;
  fakeTrace2[123]=440;
  fakeTrace2[124]=410;
  fakeTrace2[125]=330;
  fakeTrace2[125]=310;
  fakeTrace2[126]=305;
  
  c1->trace =fakeTrace1;
  c2->trace =fakeTrace2;

  obj1->trace=fakeTrace2;
  obj2->trace=fakeTrace2;


  
  test4->GetData().push_back(c1);
  test4->GetData().push_back(c2);
  test4->GetData().push_back(obj1);
  test4->GetData().push_back(obj2);

  LendaEvent * e = new LendaEvent();
  


  LendaPacker thePacker;
  thePacker.SetFilter(6,0,6,0);
  thePacker.SetGates(10,20,10,20);

  for (Long64_t i=0;i<100000000;i++){
    
    thePacker.MakeLendaEvent(e,test4,0);
    e->Finalize();
    e->Clear();
    
    if (i % 10000 == 0){
      cout<<"I is "<<i<<endl;
    }

  }
  return 0;

}
