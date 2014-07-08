

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

using namespace std;

class LendaChannel : public TObject {
public:
  LendaChannel();
  ~LendaChannel();

  void Clear();
  
  void Clear(Option_t *){Clear();}
  
  inline Int_t GetChannel(){return _channel;}
  inline Int_t GetSlot(){return _slot;}
  inline Int_t GetGlobalID(){return _globalID;}
  inline Int_t GetReferenceGlobalID(){return _refGlobalID;}

  inline Bool_t GetOverFlow(){return _overFlow;}
  inline Int_t GetNumZeroCrossings(){return _numZeroCrossings;}

  inline Double_t GetCorrectedEnergy(){return _correctedEnergy;}
  inline Double_t GetEnergy(){return _energy;}
  inline Double_t GetInternalEnergy(){return _internalEnergy;}
  inline Int_t GetPulseHeight(){return _pulseHeight;}

  inline Double_t GetTime(){return _time;}
  inline Double_t GetSoftTime(){return _softTime;}
  inline Double_t GetCubicTime(){return _cubicTime;}
  inline Double_t GetCubicFitTime(){return _cubicFitTime;}
  inline Double_t GetCorrectedTime(){return _correctedTime;}

  inline UInt_t GetCFDTrigBit(){return _CFDTrigBit;}
  
  inline Double_t GetSoftwareCFD(){return _softwareCFD;}
  inline Double_t GetCubicCFD(){return _cubicCFD;}
  inline Double_t GetCubicFitCFD(){return _cubicFitCFD;}
  inline Double_t GetInternalCFD(){return _internalCFD;}


  inline Double_t GetShortGate(){return _shortGate;}
  inline Double_t GetLongGate() {return _longGate;}

  inline Long64_t GetJentry(){return _jentry;}

  inline UInt_t GetTimeLow(){return _timeLow;}
  inline UInt_t GetTimeHigh(){return _timeHigh;}

  vector <UShort_t> GetTrace(){return _trace;}
  vector <Double_t> GetFilter(){return _filter;}
  vector <Double_t> GetCFD(){return _CFD;}

  inline Double_t GetCFDResidual(){return _CFDResidual;}
  inline Double_t GetReferenceTime(){return _refTime;}
  inline Double_t GetCubicReferenceTime(){return _cubicRefTime;}

  string GetChannelName(){return _channelName;}
  string GetReferenceChannelName(){return _referenceChannelName;}

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

  void SetTime(Double_t v){ _time=v;}
  void SetSoftTime(Double_t v){_softTime=v;}
  void SetCubicTime(Double_t v){_cubicTime=v;}
  void SetCubicFitTime(Double_t v){_cubicFitTime=v;}
  void SetCorrectedTime(Double_t v){_correctedTime=v;}

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

  //  LendaChannel & operator=(const LendaChannel &);

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

  Double_t _time;
  Double_t _softTime;
  Double_t _cubicTime;
  Double_t _cubicFitTime;

  Double_t _correctedTime;
  
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
  Double_t _cubicRefTime;
  
public:
  ClassDef(LendaChannel,2);
};




#endif 
