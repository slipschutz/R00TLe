
///////////////////////////////////////////////////////////////////////////////////////
// This file defines the LendaPacker class.  The main purpose of this class is to    //
// take DDASEvents and make LendaEvents.  The Packer is responsible for preforming   //
// the various trace analysis routines (defined in the LendaFilter class) and saving //
// them in the LendaEvent.  The Packer needs a cable map file which tells it which   //
// DDAS channels to associate together into LendaBars.  Will Throw expception if     //
// it cannot find this file.  							     //
// 										     //
// Written by Sam Lipschut Copyright 2014  					     //
///////////////////////////////////////////////////////////////////////////////////////




#ifndef __LENDAPACKER_HH
#define __LENDAPACKER_HH


#include "LendaFilter.hh"
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include "LendaEvent.hh"
#include "DDASEvent.hh"
#include <math.h>
#include <cstdlib>
#include <unordered_map>
#define CHANPERMOD 16


class Correction{
public:
  Correction(Double_t s,Double_t i,Double_t t) :
    slope(s),intercept(i),timeOffSet(t) {;}
  Correction(){;}
  Double_t slope;
  Double_t intercept;
  Double_t timeOffSet;
};


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


  LendaChannel DDASChannel2LendaChannel(ddaschannel* c,string name);
  
  void RePackSoftwareTimes(LendaEvent *Event);
  

  void PackEvent(LendaEvent* Event);
  void RePackEvent(LendaEvent* Event);

private:
  void BuildMaps();
  
  void PutDDASChannelInBar(string fullName,LendaBar &theBar,ddaschannel *theChannel);

  map<int,string> GlobalIDToFullLocal;

  map<string,int> FullLocalToGlobalID;

  map<int,string> GlobalIDToBar;

  map<string,int> BarNameToUniqueBarNumber;
  
  map<string,Correction > FullNameToCorrection;
  
  map<string,LendaBar> ThisEventsBars;


  Bool_t saveTraces;

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
