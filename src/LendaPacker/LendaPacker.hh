



#ifndef __LENDAPACKER_HH
#define __LENDAPACKER_HH


#include "LendaFilter.hh"
#include <vector>
#include <sstream>
#include <fstream>
#include "LendaEvent.hh"
#include "DDASEvent.hh"
#include <math.h>

#define CHANPERMOD 16
class LendaPacker {

public:

  LendaPacker();
  ~LendaPacker();
  
  LendaFilter theFilter;

  void Reset();

  void CalcTimeFilters(ddaschannel *theChannel);
  void CalcEnergyGates(ddaschannel *theChannel);
  void CalcAll(ddaschannel * theChannel);

  void SetFilter(Int_t,Int_t,Int_t,Int_t);
  void SetGates(Double_t,Double_t,Double_t,Double_t);
  inline void SetTraceDelay(Int_t x){traceDelay=x;}


  inline void SetJEntry(Long64_t n){jentry=n;}

  vector <Double_t> thisEventsFF;
  vector <Double_t> thisEventsCFD;

  void MakeLendaEvent(LendaEvent *Event,DDASEvent *theDDASEvent,
		      Long64_t jentry);


  LendaChannel DDASChannel2LendaChannel(ddaschannel* c);
  
  void RePackSoftwareTimes(LendaEvent *Event);
  

  void PackEvent(LendaEvent* Event);
  void RePackEvent(LendaEvent* Event);

private:
  void BuildMaps();
  
  void PutDDASChannelInBar(int GlobalID,LendaBar &theBar,ddaschannel *theChannel);

  map<int,string> GlobalIDToFullLocal;
  map<int,string> GlobalIDToBar;

  map<string,LendaBar> ThisEventsBars;


  Int_t fFL,fFG,fd,fw;
  Double_t lg,sg,lg2,sg2;
  Int_t traceDelay;
  Long64_t jentry;

  Int_t numZeroCrossings;
  
  Double_t thisEventsIntegral;
  Double_t thisEventsPulseHeight;
  Double_t longGate;
  Double_t shortGate;
  Double_t cubicCFD;
  Double_t cubicFitCFD;
  Double_t softwareCFD;
  Double_t start;

  

};

#endif
