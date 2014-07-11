
///////////////////////////////////////////////////////////////////////////////////////
// This file defines the LendaPacker class.  The main purpose of this class is to    //
// take DDASEvents and make LendaEvents.  The Packer is responsible for preforming   //
// the various trace analysis routines (defined in the LendaFilter class) and saving //
// them in the LendaEvent.  The Packer needs a cable map file which tells it which   //
// DDAS channels to associate together into LendaBars.  Will Throw expception if     //
// it cannot find this file.                                                         //
//                                                                                   //  
// The map file also provides the following information to packer:  The scattering   //
// angle that the bar is at at, which other channel provides the time of flight      //
// reference time.  The format of the MapFile is:                                    //
//                                                                                   //
// DDAS_slot_number DDAS_channel_number Bar_name_ending_in_T/B angle Name_of_refTime //
//                                                                                   //
// The reference time name must also be somewhere else in the mapfile so that the    //
// Actual DDAS slot and channel can be determined for it                             // 
//                                                                                   //
//				                                                     //
// Written by Sam Lipschutz Copyright 2014  					     //
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
#include "R00TLeSettings.hh"
#include <math.h>
#include <cstdlib>
#include <unordered_map>
#define CHANPERMOD 16  //Number of DDAS chans per DDAS module



/////////////////////////////////////////////////////////////////
// MapInfo is a Small Container class to hold the information  //
// that is stored in the mapfile.  			       //
/////////////////////////////////////////////////////////////////
class MapInfo{
public:
  MapInfo(): EnergySlope(BAD_NUM),EnergyIntercept(BAD_NUM),
	     TOFOffset(BAD_NUM),FullName(""),BarName(""),ReferenceName(""),
	     HasCorrections(false),ReferenceGlobalID(BAD_NUM),
	     GlobalID(BAD_NUM){;}

  Double_t EnergySlope;
  Double_t EnergyIntercept;
  Double_t TOFOffset;
  
  string FullName;
  string BarName;
  string ReferenceName;


  bool HasCorrections;
  Int_t ReferenceGlobalID;
  Int_t GlobalID;
  
  void Print(){cout<<FullName<<" "<<BarName<<" Slope "<<EnergySlope<<" Intercept "<<EnergyIntercept<<" Offset "<<TOFOffset<<" reference name "<<ReferenceName<<" RefGlobal "<<ReferenceGlobalID;}
};



////////////////////////////////////////////////////
// The LendaPacker Class.  See Comment Box at top //
////////////////////////////////////////////////////
class LendaPacker {

public:

  LendaPacker(R00TLeSettings*); //Defualt Constructor
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


  LendaChannel DDASChannel2LendaChannel(ddaschannel* c,MapInfo info);
  
  void RePackSoftwareTimes(LendaEvent *Event);
  

  void PackEvent(LendaEvent* Event);
  void RePackEvent(LendaEvent* Event);


private:
  void BuildMaps();
  
  void PutDDASChannelInBar(MapInfo info,LendaBar &theBar,ddaschannel *theChannel);

  map<int,string> GlobalIDToFullLocal;

  map<string,int> FullLocalToGlobalID;

  map<int,string> GlobalIDToBar;

  map<string,int> BarNameToUniqueBarNumber;
  
  map<int,MapInfo > GlobalIDToMapInfo;
  
  map<string,LendaBar> ThisEventsBars;


  Bool_t saveTraces;

  Int_t fFL,fFG,fd,fw;
  Double_t lg,sg,lg2,sg2;
  Int_t traceDelay;
  Long64_t jentry;

  Int_t numZeroCrossings;
  
  Double_t thisEventsIntegral;
  Int_t thisEventsPulseHeight;
  Int_t thisEventsFilterHeight;

  Double_t longGate;
  Double_t shortGate;
  Double_t cubicCFD;
  Double_t cubicFitCFD;
  Double_t softwareCFD;
  Double_t start;

  Double_t CFDResidual;
  R00TLeSettings * theSettings;

};

#endif
