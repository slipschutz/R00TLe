
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












/**
MapInfo is a small Container class to hold the information 
that is stored in the Mapfile/Corrections file.
*/
class MapInfo{
public:
  /**Constructor defaults all member variables to bad values. User must
     set all meembers they wish to use
   */
  MapInfo(): EnergySlope(BAD_NUM),EnergyIntercept(BAD_NUM),
	     TOFOffset(BAD_NUM),FullName(""),BarName(""),ReferenceName(""),BarAngle(BAD_NUM),
	     HasCorrections(false),IsAReferenceChannel(false),ReferenceGlobalID(BAD_NUM),
	     GlobalID(BAD_NUM){;}

  Double_t EnergySlope; ///<Slope for energy calibration of light output.
  Double_t EnergyIntercept; ///<Intercept for energy calibration of light output
  Double_t TOFOffset; ///<Constant time offset (in clock tics) used in Corrected Times
  
  string FullName; ///<Full Channel Name. Exmaple: SL01T
  string BarName; ///<Name of Bar. Example: SL01
  string ReferenceName; ///<Full Reference channel Name. Example: OBJ1T

  Double_t BarAngle;///<The angle of bar relative to the beam direction

  bool HasCorrections; ///<Flag to be set when there is correction information available

  bool IsAReferenceChannel;

  ///Global DDAS ID of the reference channel.  
  /**The Global ID is the spot in the crate starting at the top left channel counting down and to the right
     (starting at zero).  Example the first channel in the second module would be ID=16*/
  Int_t ReferenceGlobalID;

  ///Global DDAS ID of this Channel 
  /**The Global ID is the spot in the crate starting at the top left channel counting down and to the right
     (starting at zero).  Example the first channel in the second module would be ID=16*/
  Int_t GlobalID; 
  
  ///Print method to dump the above information
  void Print(){cout<<FullName<<" "<<BarName<<" Slope "<<EnergySlope<<" Intercept "<<EnergyIntercept<<" Offset "
		   <<TOFOffset<<" reference name "<<ReferenceName<<" RefGlobal "<<ReferenceGlobalID;}
};










/**RefTimeContainer is a small container class.  It will hold the different types of times that  
will be set as the reference times for a particular channel		      
*/
class RefTimeContainer{
public:
  RefTimeContainer(): RefTime(0),RefSoftTime(0),RefCubicTime(0){} ///<defualt constructor 
  RefTimeContainer(Double_t v1,Double_t v2, Double_t v3) : RefTime(v1),RefSoftTime(v2),RefCubicTime(v3){;} //<Constrctor to set all parameters
  Double_t RefTime; ///<Reference time from modules time
  Double_t RefSoftTime;///<Reference time from offline linear algorithm
  Double_t RefCubicTime;///<Reference time for offlien cubic algorithm
};








/**
The main purpose of LendaPacker class is to take DDASEvents and make LendaEvents. 
The Packer is responsible for preforming  the various trace analysis routines 
(defined in the LendaFilter class) and saving them in the LendaEvent. 
The Packer needs a cable map file which tells it which   
DDAS channels to associate together into LendaBars.  Will Throw expception if     
it cannot find this file.  Currently only supports single crate setups            
                                                                                   
The map file also provides the following information to packer:  The scattering   
angle that the bar is at at, which other channel provides the time of flight      
reference time.  The format of the MapFile is:                                    
                                                                                  
DDAS_slot_number DDAS_channel_number Bar_name_ending_in_T/B angle Name_of_refTime 
                                                                                  
The reference time name must also be somewhere else in the mapfile so that the    
Actual DDAS slot and channel can be determined for it                             
                                                                                  
				                                                     
Written by Sam Lipschutz Copyright 2014  					     
*/
class LendaPacker {

public:

  LendaPacker(){;}///<Defualt constructor 
  /**Constructor where R00TLeSettings Object is provided.  Many features of the
   packer require a R00TLeSetting object be set.  Could seg fualt if you don't*/
  LendaPacker(R00TLeSettings*);
  
  ~LendaPacker();///<De-constructor
  
  LendaFilter theFilter;///<Instance of the LendaFilter class which preforms the wave form analysis


  void SetSettingFileNames(string MapFileName,string CorrectionsFileName);///<Set the filenames for Corrections/Map file.  Looks in prm directory.  Calls BuildMaps
  
  /**Reset the Packer Variables used in making individual LendaChannels.
     For example the results of the waveform analysis: PulseIntegral, longGate, shortGate, timing filters
     Packer should be Reset before every channel that it analyses*/
  void Reset();



  void CalcTimeFilters(vector<UShort_t>& theTrace);///<Call the waveform timing analysis routines provided by the LendaFilter object
  void CalcObjectTimeFilters(vector<UShort_t>& theTrace);///<Same as CalcTimeFilters, except uses object filter parameters

  void CalcEnergyGates(vector<UShort_t>& theTrace);///<Call the waveform energy analysis routines provided by the LendaFilter object
  void CalcAll(ddaschannel * theChannel);///<Call all waveform analysis routines


  void SetFilter(Int_t _FL,Int_t _FG ,Int_t _d,Int_t _w);///<Set parameters for the waveform timing analysis routines
  void SetObjectFilter(Int_t _FL, Int_t _FG, Int_t _d,Int_t _w);/// <Set parameters for waveform timing alanysis routines for object scint signals
  void SetGates(Double_t,Double_t,Double_t,Double_t);///<Set parameters for the waveform energy analysis routines

  inline void SetTraceDelay(Int_t x){traceDelay=x;}///<Set the trace delay value

  
  inline void SetJEntry(Long64_t n){jentry=n;}///<Set the entry number that this event had in the RAW tree

  
  /**Main Method used in building LendaEvents.
     Takes the ddaschannels in the DDASEvent and maps them to the appropriate
     Dectors.  Preforms all waveform analysis and saves infomration in 
     a LendaEvent*/
  void MakeLendaEvent(LendaEvent *Event,DDASEvent *theDDASEvent,
		      Long64_t jentry);
  

  /** Take a LendaEvent and rebuild the event.
      Used to recalculate timing algorithms with new parameters.
      
      Does NOT re-map the channels.  However they were organized
      in InEvent is how they will be in outEvent.
  */
  void ReMakeLendaEvent(LendaEvent*inEvent,LendaEvent*outEvent);


  void RePackChannel(LendaChannel *,bool isAnObject=false);///<RePack a LendaChannel.  Used in ReMakeLendaEvent

  /**Convert a DDASChannel to a LendaChannel.  Will call CalcAllMethod
     and save waveform analysis results into the LendaChannel
   */
  LendaChannel DDASChannel2LendaChannel(ddaschannel* c,MapInfo info);
  


  void RePackSoftwareTimes(LendaEvent *Event);///<OBSOLETE!!!!!
  

  void PackEvent(LendaEvent* Event);///<OBSOLETE!!!
  void RePackEvent(LendaEvent* Event);///<OBSOLETE!!!

  void FindAndSetMapAndCorrectionsFileNames(int RunNumber);///<Looks in prm directory for map/corrections files and sets them

  MapInfo GetMapInfo(string FullName);///<Get the MapInfo for a channel 





private:
  void BuildMaps();///<Method to build Maps for all the corrections and map file information
  
  void PutDDASChannelInBar(MapInfo info,LendaBar &theBar,ddaschannel *theChannel);

  void PackCalculatedValues(LendaChannel*,MapInfo &);

  void FillReferenceTimesInEvent(LendaEvent* Event,map<string,LendaBar>& ThisEventsBars,map <int,RefTimeContainer > & GlobalIDToReferenceTimes);

  vector <Double_t> thisEventsFF; ///<Temporary place for each channels fast filter.  Gets reset when Reset is called
  vector <Double_t> thisEventsCFD;///<Temporary place for each channels digital CFD.  Gets reset when Reset is called
  

  map<int,string> GlobalIDToFullLocal;

  map<string,int> FullLocalToGlobalID;

  map<int,string> GlobalIDToBar;

  map<string,int> BarNameToUniqueBarNumber;

  map<string,Double_t> BarNameToBarAngle;
  
  map<int,MapInfo > GlobalIDToMapInfo;
  
  //  map<string,LendaBar> ThisEventsBars;

  string _mapFileName,_correctionsFileName;

  Bool_t saveTraces;

  Int_t fFL,fFG,fd,fw;

  Int_t objFL,objFG,objd,objw;

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


  string referenceChannelPattern;//A string to define what channel names will be considered the reference channels ex OBJ
  
  R00TLeSettings * theSettings;

};

#endif
