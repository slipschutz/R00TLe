

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>

#include "TFile.h"
#include "TSystem.h"
#include "TObject.h"
#include "TTree.h"
#include "TStopwatch.h"

#include "S800Event.hh"
#include "DDASEvent.hh"
#include "LendaPacker.hh"
#include "R00TLeSettings.hh"

#include "S800Settings.hh"
#include "S800Calibration.hh"

#include "DAQdefs.h"

#include "Utilities.hh"


int main(){


  // ddaschannel * c = new ddaschannel();

  // ddaschannel * c2 = new ddaschannel();
  
  // c->slotid=2;
  // c->chanid=0;

  // c2->slotid=2;
  // c2->chanid=1;

  // DDASEvent * e = new DDASEvent();

  // e->AddChannelData(c);

  // e->AddChannelData(c2);

  // LendaEvent * event = new LendaEvent();

  // LendaPacker aPacker(new R00TLeSettings());
  // aPacker.FindAndSetMapAndCorrectionsFileNames(0);
  
  // while (true){
  //   aPacker.MakeLendaEvent(event,e,0);
    
  //   LendaBar b = event->GetBar("SL01");

  //   if (b.Name != "SL01"){
  //     cout<<"fail"<<endl;
  //     throw 1;
  //   }

  //   LendaBar * b1 = event->Bars2(0);
  //   cout<<b1->Name<<endl;

    
  //   event->Clear();
  // }



  




  return 0;

}
