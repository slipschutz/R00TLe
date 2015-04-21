

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

  //  void ShowDiff(const LendaChannel&);

  void Clear();///<Resets all the values in the LendaChannel.  Should be called after every write to a tree
  void Clear(Option_t *){Clear();}///<To suppress warnings about hidding clear in TObject
  
  MAKE_VAR(Energies,vector<Double_t>);
  MAKE_VAR(SoftwareTimes,vector<Double_t>);
  MAKE_VAR(CubicTimes,vector<Double_t>);
  MAKE_VAR(PulseHeights,vector<Int_t>);
  MAKE_VAR(SoftwareReferenceTimes,vector<Double_t>);
  MAKE_VAR(CubicReferenceTimes,vector<Double_t>);
  
  
  inline Int_t GetChannel(){return rChannel;}///<Gets DDAS channel number for this channel
  inline Int_t GetSlot(){return rSlot;}///<Gets DDAS slot number for this channel
  inline Int_t GetGlobalID(){return rGlobalID;}///<Gets the Global DDAS id for this channel
  inline Int_t GetReferenceGlobalID(){return rRefGlobalID;}///<Gets the Global DDAS id for the reference channel

  inline Bool_t GetOverFlow(){return rOverFlow;}///<Gets bit to check for overflows in the trace
  inline Int_t GetNumZeroCrossings(){return rNumZeroCrossings;}///<Gets number of ZeroCrossings in digital CFD

  inline Double_t GetCorrectedEnergy(){return rCorrectedEnergy;}///<Get Corrected energy (baseline subtracted pulse integral from trace)
  inline Double_t GetEnergy(){return rEnergies[0];}///<Gets energy (baseline subtracted pulse integral from trace)
  inline Double_t GetInternalEnergy(){return rInternalEnergy;}///<Gets result from internal energy filter
  inline Int_t GetPulseHeight(){return rPulseHeights[0];}///<Gets max pulse height in trace
  inline Int_t GetFilterHeight(){return rFilterHeight;}///<Gets max height in the fast filter

  inline Double_t GetTime(){return rTime;}///<Gets the internal timestamp from module 
  inline Double_t GetSoftTime(){return rSoftwareTimes[0];}///<Gets the offline timestamp from the linear algorithm
  inline Double_t GetCubicTime(){return rCubicTimes[0];}///<Gets the offline timestamp from the cubic algorithm
  inline Double_t GetCubicFitTime(){return rCubicFitTime;}///<Gets the offline timestamp from the cubic fit algorithm

  inline Double_t GetCorrectedTime(){return rCorrectedTime;}///<Gets the time shift corrected internal timestamp
  inline Double_t GetCorrectedCubicTime(){return rCorrectedCubicTime;}///<Gets the time shift corrected offline cubic time stamp
  inline Double_t GetCorrectedCubicFitTime(){return rCorrectedCubicFitTime;} ///<Gets the time shift corrected cubic fit timestamp
  inline Double_t GetCorrectedSoftTime(){return rCorrectedSoftTime;}///<Gets the time shift corrected offline linear time stamp

  inline UInt_t GetCFDTrigBit(){return rCFDTrigBit;}///<Gets the CFD trigbit that is provided by the module 
  
  inline Double_t GetSoftwareCFD(){return rSoftwareCFD;}///<Gets the sub clocktic postion of the zero crossing in the CFD starting from the beginning of the trace
  inline Double_t GetCubicCFD(){return rCubicCFD;}///<Same as GetSoftwareCFD() except with the cubic algorithm
  inline Double_t GetCubicFitCFD(){return rCubicFitCFD;}///<Same as GetSoftwareCFD() except with the cubic fit algorithm
  inline Double_t GetInternalCFD(){return rInternalCFD;}///<Sub clock tic portion of the internal time stamp.  Not from beginning of the trace


  inline Double_t GetShortGate(){return rShortGate;}
  inline Double_t GetLongGate() {return rLongGate;}

  inline Long64_t GetJentry(){return rJentry;}

  inline UInt_t GetTimeLow(){return rTimeLow;}///<Low bits of the internal time stamp
  inline UInt_t GetTimeHigh(){return rTimeHigh;}///<High bits of the internal time stamp

  vector <UShort_t> GetTrace(){return rTrace;}///<Gets the trace if it was stored in the channel
  vector <Double_t> GetFilter(){return rFilter;}///<Gets the offline calculated fast filter if it was stored
  vector <Double_t> GetCFD(){return rCFD;}///<Gets the offline calculated CFD if it was stored

  inline Double_t GetCFDResidual(){return rCFDResidual;}///<Not important

  inline Double_t GetReferenceTime(){return rRefTime;}///<Gets the internal timestamp for the reference channel
  inline Double_t GetCubicReferenceTime(){return rCubicReferenceTimes[0];}///<Gets cubic timestamp for the reference channel
  inline Double_t GetSoftwareReferenceTime(){return rSoftwareReferenceTimes[0];}///<Gets the offline caclulated linear timestamp for the reference channel

  string GetChannelName(){return rChannelName;}///<Gets the full channel name
  string GetReferenceChannelName(){return rReferenceChannelName;}///<Gets the full naem of the reference channel


  inline Double_t GetOtherTime(){return rOtherTime;}
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  void SetChannel(Int_t v){rChannel=v;}
  void SetSlot(Int_t v){rSlot=v;}
  void SetGlobalID(Int_t v){rGlobalID=v;}
  void SetReferenceGlobalID(Int_t v){rRefGlobalID=v;}

  void SetOverFlow(Bool_t v){rOverFlow=v;}
  void SetNumZeroCrossings(Int_t v){ rNumZeroCrossings=v;}
  
  void SetCorrectedEnergy(Double_t v){ rCorrectedEnergy=v;}

  void SetInternalEnergy(Double_t v){rInternalEnergy=v;}

  void SetFilterHeight(Int_t v){rFilterHeight=v;}

  void SetTime(Double_t v){ rTime=v;}
  void SetCubicFitTime(Double_t v){rCubicFitTime=v;}

  void SetCorrectedTime(Double_t v){rCorrectedTime=v;}
  void SetCorrectedCubicFitTime(Double_t v){rCorrectedCubicFitTime=v;}
  void SetCorrectedSoftTime(Double_t v){rCorrectedSoftTime=v;}
  void SetCorrectedCubicTime(Double_t v){rCorrectedCubicTime=v;}


  void SetTimeLow(UInt_t v){rTimeLow=v;}
  void SetTimeHigh(UInt_t v){rTimeHigh=v;}
  

  void SetCFDTrigBit(UInt_t v){rCFDTrigBit=v;}
   
  void SetSoftwareCFD(Double_t v){rSoftwareCFD=v;}
  void SetCubicCFD(Double_t v){rCubicCFD=v;}
  void SetCubicFitCFD(Double_t v){rCubicFitCFD=v;}
  void SetInternalCFD(Double_t v){rInternalCFD=v;}

  void SetShortGate(Double_t v){rShortGate=v;}
  void SetLongGate(Double_t v) {rLongGate=v;}
  
  void SetJentry(Long64_t v){rJentry=v;}

  void SetTrace(vector<UShort_t> v){rTrace=v;}
  void SetFilter(vector <Double_t> v){rFilter=v;}
  void SetCFD(vector <Double_t> v){rCFD=v;}

  void SetCFDResidual(Double_t v){rCFDResidual=v;}

  void SetChannelName(string s){rChannelName=s;}
  void SetReferenceChannelName(string s){rReferenceChannelName=s;}
  
  void SetReferenceTime(Double_t v){rRefTime=v;}
  // void SetCubicReferenceTime(Double_t v){rCubicRefTime=v;}
  // void SetSoftReferenceTime(Double_t v){rSoftRefTime=v;}
  //  LendaChannel & operator=(const LendaChannel &);

  void SetOtherTime(Double_t v){rOtherTime=v;}
  
  Bool_t operator==(const LendaChannel & RHS);

private:
  Int_t rChannel;
  Int_t rSlot;
  Int_t rGlobalID;
  Int_t rRefGlobalID;

  
  Bool_t rOverFlow;
  Int_t  rNumZeroCrossings;
  
  Double_t rCorrectedEnergy;

  Double_t rInternalEnergy;

  Int_t rFilterHeight;

  Double_t rTime;

  Double_t rCubicFitTime;

  Double_t rCorrectedTime;
  Double_t rCorrectedCubicFitTime;
  Double_t rCorrectedSoftTime;
  Double_t rCorrectedCubicTime;

  UInt_t rTimeLow;
  UInt_t rTimeHigh;

  //CFD Values for sub clock tic timing
  //These cfds refer to the cfd from calculated by
  //LendaFilter.GetZeroCrossingImproved which 
  //will give the value for the largest zerocrossing
  Double_t rSoftwareCFD;
  Double_t rCubicCFD;
  Double_t rCubicFitCFD;
  //Internal CFD is for wherever the module triggered on
  Double_t rInternalCFD;
  UInt_t rCFDTrigBit;

  Double_t rShortGate;
  Double_t rLongGate;
  
  Long64_t rJentry;
  
  vector <UShort_t> rTrace;
  vector <Double_t> rFilter;
  vector <Double_t> rCFD;

  Double_t rCFDResidual;

  string rChannelName;
  string rReferenceChannelName;

  Double_t rRefTime;
  // Double_t rSoftRefTime;
  // Double_t rCubicRefTime;

  Double_t rOtherTime;
  
public:
  ClassDef(LendaChannel,4);
};




#endif 
