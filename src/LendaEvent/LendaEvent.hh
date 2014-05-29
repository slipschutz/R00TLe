#ifndef __LENDA_EVENT_HH
#define __LENDA_EVENT_HH

#include "TObject.h"
#include "TFile.h"
#include <vector>
#include <map>
#include "LendaSettings.hh"
#include "LendaBar.hh"
using namespace std;

class LendaEvent :  public TObject  {
public:

  LendaEvent(); //Constructor 

  void Clear(); //Clear the Event.  Should be called after every write to a tree
  void Finalize(); //Applies internal corrections and calculates convient Branches/leaves

  vector <LendaBar> Bars;
  void PushABar(LendaBar aBar){Bars.push_back(aBar);NumBars++;}
  

  Int_t NumOfChannelsInEvent; //The Number of channels associated with this event  
  Int_t N;//Same thing as NumOfChannelsInEvent
  Int_t NumBars;//Number of bars in event.  Due to pileup could be different from N/2
  


  void Fatal();//overload this stupid inherited method


  void WriteSettings(LendaSettings * theSettings);


private:


  
public:
  ClassDef(LendaEvent, 20);
};

#endif



