
///////////////////////////////////////////////////////////////////////////////////////
// This file defines the LendaPacker class.  The main purpose of this class is to    //
// take DDASEvents and make LendaEvents.  The Packer is responsible for preforming   //
// the various trace analysis routines (defined in the LendaFilter class) and saving //
// them in the LendaEvent.  The Packer needs a cable map file which tells it which   //
// DDAS channels to associate together into LendaBars.  Will Throw expception if     //
// it cannot find this file.  Currently only supports single crate setups            //
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
// 				                                                     //
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

#define CHANPERMOD 16  //Number of DDAS chans per DDAS module









/////////////////////////////////////////////////////////////////
// MapInfo is a Small Container class to hold the information  //
// that is stored in the mapfile/Corrections file 	       //
/////////////////////////////////////////////////////////////////
class MapInfo{
public:
  //Constructor defaults all member variables to bad values
  MapInfo(): EnergySlope(BAD_NUM),EnergyIntercept(BAD_NUM),
	     TOFOffset(BAD_NUM),FullName(""),BarName(""),ReferenceName(""),
	     HasCorrections(false),ReferenceGlobalID(BAD_NUM),
	     GlobalID(BAD_NUM){;}

  Double_t EnergySlope; //Slope for energy calibration of light output
  Double_t EnergyIntercept; //Intercept for energy calibration of light output
  Double_t TOFOffset; //Constant time offset (clock tics) used in Corrected Times
  
  string FullName; //Full Channel Name. Exmaple: SL01T
  string BarName; //Name of Bar. Example: SL01
  string ReferenceName; //Full Reference channel Name. Example: OBJ1T


  bool HasCorrections; //Flag to be set when there is correction information available
  Int_t ReferenceGlobalID; //Global DDAS ID of the reference channel
  Int_t GlobalID; //Global ID of this Channel
  
  //Print method to dump the above information
  void Print(){cout<<FullName<<" "<<BarName<<" Slope "<<EnergySlope<<" Intercept "<<EnergyIntercept<<" Offset "
		   <<TOFOffset<<" reference name "<<ReferenceName<<" RefGlobal "<<ReferenceGlobalID;}
};









////////////////////////////////////////////////////////////////////////////////
// This is a small container class to hold the different types of times that  //
// will be set as the reference times for a particular channel		      //
////////////////////////////////////////////////////////////////////////////////
class RefTimeContainer{
public:
  RefTimeContainer(): RefTime(0),RefSoftTime(0),RefCubicTime(0){}
  RefTimeContainer(Double_t v1,Double_t v2, Double_t v3) : RefTime(v1),RefSoftTime(v2),RefCubicTime(v3){;}
  Double_t RefTime;
  Double_t RefSoftTime;
  Double_t RefCubicTime;
};













////////////////////////////////////////////////////////////////////////////////////////
// The LendaPacker Class.  Main class responsible for taking DDASEvents and creating  //
// LendaEvents.  See Comment Box at top 					      //
////////////////////////////////////////////////////////////////////////////////////////

class LendaPacker {

public:

  LendaPacker(){;}//Defualt constructor 
  LendaPacker(R00TLeSettings*);// Constructor where R00TLeSettings Object is made
  
  ~LendaPacker();//Deconstructor
  
  LendaFilter theFilter;//Instance of the LendaFilter class which preforms the wave form analysis


  void SetSettingFileNames(string MapFileName,string CorrectionsFileName);//Set the filenames for Corrections/Maps

  //Reset the Packer Variables used in making individual LendaChannels
  //For example the results of the waveform analysis: PulseIntegral, longGate, shortGate, timing filters
  void Reset();

  //Call the Waveform analysis routines provided by the LendaFilterobject
  void CalcTimeFilters(vector<UShort_t>& theTrace);
  void CalcEnergyGates(vector<UShort_t>& theTrace);
  void CalcAll(ddaschannel * theChannel);

  //Set parameters for the waveform analysis routines
  void SetFilter(Int_t,Int_t,Int_t,Int_t);
  void SetGates(Double_t,Double_t,Double_t,Double_t);
  inline void SetTraceDelay(Int_t x){traceDelay=x;}

  
  inline void SetJEntry(Long64_t n){jentry=n;}

  vector <Double_t> thisEventsFF;
  vector <Double_t> thisEventsCFD;

  
  //Main Method used in building
  //Takes the ddaschannels in the DDASEvent and maps them to the appropriate
  //Dectors.  Preforms all waveform analysis and saves infomration in 
  //a LendaEvent
  void MakeLendaEvent(LendaEvent *Event,DDASEvent *theDDASEvent,
		      Long64_t jentry);
  
  //Take a LendaEvent and rebuild the event
  void ReMakeLendaEvent(LendaEvent*inEvent,LendaEvent*outEvent);
  //RePack a LendaChannel
  void RePackChannel(LendaChannel *);

  LendaChannel DDASChannel2LendaChannel(ddaschannel* c,MapInfo info);
  


  void RePackSoftwareTimes(LendaEvent *Event);
  

  void PackEvent(LendaEvent* Event);
  void RePackEvent(LendaEvent* Event);

  void FindAndSetMapAndCorrectionsFileNames(int RunNumber);

private:
  void BuildMaps();
  
  void PutDDASChannelInBar(MapInfo info,LendaBar &theBar,ddaschannel *theChannel);

  void PackCalculatedValues(LendaChannel*,MapInfo &);

  void FillReferenceTimesInEvent(LendaEvent* Event,map<string,LendaBar>& ThisEventsBars,map <int,RefTimeContainer > & GlobalIDToReferenceTimes);

  

  map<int,string> GlobalIDToFullLocal;

  map<string,int> FullLocalToGlobalID;

  map<int,string> GlobalIDToBar;

  map<string,int> BarNameToUniqueBarNumber;
  
  map<int,MapInfo > GlobalIDToMapInfo;
  
  //  map<string,LendaBar> ThisEventsBars;

  string _mapFileName,_correctionsFileName;

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
