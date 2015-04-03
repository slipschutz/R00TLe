

////////////////////////////////////////////////////
// This is the header for the lenda channel class //
// Will hold the actual information of the event  //
////////////////////////////////////////////////////


#ifndef __LENDACHANNEL_HH
#define __LENDACHANNEL_HH

#include "TObject.h"
#include <vector>
#include <iostream>
#define BAD_NUM -10008


/**MACRO TO AUTOMATICALLY Create private member variable with a Getter and Setter
   MAKE_VAR(Energy,Double_t) will make a private variable rEnergy.  Setter
   Will be SetEnergy(const Double_t &) and Getter GetEnergy() const
 */
#define MAKE_VAR(X,Y) private: Y r##X;  public: void Set##X(const Y &val) {r##X=val;} Y Get##X() const{return r##X;}


using namespace std;

/**Lowest level container for DDAS information.  Each DDAS channel with have a coresponding LendaChannel object.
   The channel by channel infomation for the module and the results of waveform analysis are saved in LendaChannels.
   
 */
class LendaChannel : public TObject {
public:
  LendaChannel();///<Defualt Constructor
  ~LendaChannel();///<Defualt Constructor

  void ShowDiff(const LendaChannel&);

  void Clear();///<Resets all the values in the LendaChannel.  Should be called after every write to a tree
  void Clear(Option_t *){Clear();}///<To suppress warnings about hidding clear in TObject
  
  MAKE_VAR(Energies,vector<Double_t>);
  MAKE_VAR(Times,vector<Double_t>);
  MAKE_VAR(CubicTimes,vector<Double_t>);


  
  inline Int_t GetChannel(){return _channel;}///<Gets DDAS channel number for this channel
  inline Int_t GetSlot(){return _slot;}///<Gets DDAS slot number for this channel
  inline Int_t GetGlobalID(){return _globalID;}///<Gets the Global DDAS id for this channel
  inline Int_t GetReferenceGlobalID(){return _refGlobalID;}///<Gets the Global DDAS id for the reference channel

  inline Bool_t GetOverFlow(){return _overFlow;}///<Gets bit to check for overflows in the trace
  inline Int_t GetNumZeroCrossings(){return _numZeroCrossings;}///<Gets number of ZeroCrossings in digital CFD

  inline Double_t GetCorrectedEnergy(){return _correctedEnergy;}///<Get Corrected energy (baseline subtracted pulse integral from trace)
  inline Double_t GetEnergy(){return _energy;}///<Gets energy (baseline subtracted pulse integral from trace)
  inline Double_t GetInternalEnergy(){return _internalEnergy;}///<Gets result from internal energy filter
  inline Int_t GetPulseHeight(){return _pulseHeight;}///<Gets max pulse height in trace
  inline Int_t GetFilterHeight(){return _filterHeight;}///<Gets max height in the fast filter

  inline Double_t GetTime(){return _time;}///<Gets the internal timestamp from module 
  inline Double_t GetSoftTime(){return _softTime;}///<Gets the offline timestamp from the linear algorithm
  inline Double_t GetCubicTime(){return _cubicTime;}///<Gets the offline timestamp from the cubic algorithm
  inline Double_t GetCubicFitTime(){return _cubicFitTime;}///<Gets the offline timestamp from the cubic fit algorithm

  inline Double_t GetCorrectedTime(){return _correctedTime;}///<Gets the time shift corrected internal timestamp
  inline Double_t GetCorrectedCubicTime(){return _correctedCubicTime;}///<Gets the time shift corrected offline cubic time stamp
  inline Double_t GetCorrectedCubicFitTime(){return _correctedCubicFitTime;} ///<Gets the time shift corrected cubic fit timestamp
  inline Double_t GetCorrectedSoftTime(){return _correctedSoftTime;}///<Gets the time shift corrected offline linear time stamp

  inline UInt_t GetCFDTrigBit(){return _CFDTrigBit;}///<Gets the CFD trigbit that is provided by the module 
  
  inline Double_t GetSoftwareCFD(){return _softwareCFD;}///<Gets the sub clocktic postion of the zero crossing in the CFD starting from the beginning of the trace
  inline Double_t GetCubicCFD(){return _cubicCFD;}///<Same as GetSoftwareCFD() except with the cubic algorithm
  inline Double_t GetCubicFitCFD(){return _cubicFitCFD;}///<Same as GetSoftwareCFD() except with the cubic fit algorithm
  inline Double_t GetInternalCFD(){return _internalCFD;}///<Sub clock tic portion of the internal time stamp.  Not from beginning of the trace


  inline Double_t GetShortGate(){return _shortGate;}
  inline Double_t GetLongGate() {return _longGate;}

  inline Long64_t GetJentry(){return _jentry;}

  inline UInt_t GetTimeLow(){return _timeLow;}///<Low bits of the internal time stamp
  inline UInt_t GetTimeHigh(){return _timeHigh;}///<High bits of the internal time stamp

  vector <UShort_t> GetTrace(){return _trace;}///<Gets the trace if it was stored in the channel
  vector <Double_t> GetFilter(){return _filter;}///<Gets the offline calculated fast filter if it was stored
  vector <Double_t> GetCFD(){return _CFD;}///<Gets the offline calculated CFD if it was stored

  inline Double_t GetCFDResidual(){return _CFDResidual;}///<Not important
  inline Double_t GetReferenceTime(){return _refTime;}///<Gets the internal timestamp for the reference channel
  inline Double_t GetCubicReferenceTime(){return _cubicRefTime;}///<Gets cubic timestamp for the reference channel
  inline Double_t GetSoftReferenceTime(){return _softRefTime;}///<Gets the offline caclulated linear timestamp for the reference channel

  string GetChannelName(){return _channelName;}///<Gets the full channel name
  string GetReferenceChannelName(){return _referenceChannelName;}///<Gets the full naem of the reference channel


  inline Double_t GetOtherTime(){return _otherTime;}


  void SetChannel(Int_t v){_channel=v;}
  void SetSlot(Int_t v){_slot=v;}
  void SetGlobalID(Int_t v){_globalID=v;}
  void SetReferenceGlobalID(Int_t v){_refGlobalID=v;}

  void SetOverFlow(Bool_t v){_overFlow=v;}
  void SetNumZeroCrossings(Int_t v){ _numZeroCrossings=v;}
  
  void SetCorrectedEnergy(Double_t v){ _correctedEnergy=v;}
  void SetEnergy(Double_t v){ _energy=v;}
  void SetInternalEnergy(Double_t v){_internalEnergy=v;}
  void SetPulseHeight(Int_t v){ _pulseHeight=v;}
  void SetFilterHeight(Int_t v){_filterHeight=v;}

  void SetTime(Double_t v){ _time=v;}
  void SetSoftTime(Double_t v){_softTime=v;}
  void SetCubicTime(Double_t v){_cubicTime=v;}
  void SetCubicFitTime(Double_t v){_cubicFitTime=v;}

  void SetCorrectedTime(Double_t v){_correctedTime=v;}
  void SetCorrectedCubicFitTime(Double_t v){_correctedCubicFitTime=v;}
  void SetCorrectedSoftTime(Double_t v){_correctedSoftTime=v;}
  void SetCorrectedCubicTime(Double_t v){_correctedCubicTime=v;}


  void SetTimeLow(UInt_t v){_timeLow=v;}
  void SetTimeHigh(UInt_t v){_timeHigh=v;}
  

  void SetCFDTrigBit(UInt_t v){_CFDTrigBit=v;}
   
  void SetSoftwareCFD(Double_t v){_softwareCFD=v;}
  void SetCubicCFD(Double_t v){_cubicCFD=v;}
  void SetCubicFitCFD(Double_t v){_cubicFitCFD=v;}
  void SetInternalCFD(Double_t v){_internalCFD=v;}

  void SetShortGate(Double_t v){_shortGate=v;}
  void SetLongGate(Double_t v) {_longGate=v;}
  
  void SetJentry(Long64_t v){_jentry=v;}

  void SetTrace(vector<UShort_t> v){_trace=v;}
  void SetFilter(vector <Double_t> v){_filter=v;}
  void SetCFD(vector <Double_t> v){_CFD=v;}

  void SetCFDResidual(Double_t v){_CFDResidual=v;}

  void SetChannelName(string s){_channelName=s;}
  void SetReferenceChannelName(string s){_referenceChannelName=s;}
  
  void SetReferenceTime(Double_t v){_refTime=v;}
  void SetCubicReferenceTime(Double_t v){_cubicRefTime=v;}
  void SetSoftReferenceTime(Double_t v){_softRefTime=v;}
  //  LendaChannel & operator=(const LendaChannel &);

  void SetOtherTime(Double_t v){_otherTime=v;}
  
  Bool_t operator==(const LendaChannel & RHS);

private:
  Int_t _channel;
  Int_t _slot;
  Int_t _globalID;
  Int_t _refGlobalID;

  
  Bool_t _overFlow;
  Int_t _numZeroCrossings;
  
  Double_t _correctedEnergy;
  Double_t _energy;
  Double_t _internalEnergy;
  Int_t _pulseHeight;
  Int_t _filterHeight;

  Double_t _time;
  Double_t _softTime;
  Double_t _cubicTime;
  Double_t _cubicFitTime;

  Double_t _correctedTime;
  Double_t _correctedCubicFitTime;
  Double_t _correctedSoftTime;
  Double_t _correctedCubicTime;

  UInt_t _timeLow;
  UInt_t _timeHigh;

  Double_t _softwareCFD;
  Double_t _cubicCFD;
  Double_t _cubicFitCFD;
  Double_t _internalCFD;
  UInt_t _CFDTrigBit;

  Double_t _shortGate;
  Double_t _longGate;
  
  Long64_t _jentry;
  
  vector <UShort_t> _trace;
  vector <Double_t> _filter;
  vector <Double_t> _CFD;

  Double_t _CFDResidual;

  string _channelName;
  string _referenceChannelName;

  Double_t _refTime;
  Double_t _softRefTime;
  Double_t _cubicRefTime;

  Double_t _otherTime;
  
public:
  ClassDef(LendaChannel,4);
};




#endif 
