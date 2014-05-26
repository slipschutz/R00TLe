/* ddaschannel class definition header file */

#ifndef DDASCHANNEL_H
#define DDASCHANNEL_H

#include <vector>
#include "TObject.h"
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

// define bit masks to extract data from specific locations
// of pixie16 data
#define CHANNELIDMASK             0xF  // Bits 0-3 inclusive
#define SLOTIDMASK               0xF0  // Bits 4-7 inclusive
#define CRATEIDMASK             0xF00  // Bits 8-11 inclusive
#define HEADERLENGTHMASK      0x1F000  // Bits 12-16 inclusive
#define CHANNELLENGTHMASK  0x3FFE0000  // Bits 17-29 inclusive
#define OVERFLOWMASK       0x40000000  // Bit 30 has overflow information (1 - overflow)
#define FINISHCODEMASK     0x80000000  // Bit 31 has pileup information (1 - pileup)
#define LOWER16BITMASK         0xFFFF  // Lower 16 bits
#define UPPER16BITMASK     0xFFFF0000  // Upper 16 bits
#define LOWER12BITMASK          0xFFF  // Lower 12 bits
#define BIT31MASK          0x80000000  // Bit 31 
#define BIT30to16MASK      0x7FFF0000  // Bits 30 through 16

// number of words added to pixie16 channel event header when energy sums
// and baselines are recorded
#define SIZEOFESUMS                 4
// number of words added to pixie16 channel event header when QDC sums
// are recorded
#define SIZEOFQDCSUMS               8


using namespace std;

/* Define a class that can accumulate ddas channels
   to be sorted into true events. */

class ddaschannel : public TObject {
public:

  /********** Variables **********/

  // ordering is important with regards to access and file size.  Should
  // always try to maintain order from largest to smallest data type
  // Double_t, Int_t, Short_t, Bool_t, pointers

  /* Channel events always have the following info. */
  Double_t time;              //< assembled time including cfd
  Double_t cfd;               //< cfd time only

  UInt_t energy;              //< energy of event
  UInt_t timehigh;            //< portion of timestamp
  UInt_t timelow;             //< portion of timestamp
  UInt_t timecfd;             //< portion of timestamp

  Int_t channelnum;           //< obsolete
  Int_t finishcode;           //< indicate whether event is 
  Int_t channellength;        //<
  Int_t channelheaderlength;
  Int_t overflowcode;
  Int_t chanid;
  Int_t slotid;
  Int_t crateid;
  Int_t id;
  UInt_t cfdtrigsourcebit;     //< cfd trigger source bit, 0- even, 1 - odd

  Int_t tracelength;


  /* A channel may have extra information... */
  vector<UInt_t> energySums;
  vector<UShort_t> qdcSums;
  
  /* A waveform (trace) may be stored too. */
  vector<UShort_t> trace;

  /********** Functions **********/
  ddaschannel();
  ddaschannel(const ddaschannel& obj);
  ddaschannel& operator=(const ddaschannel& obj);
  ~ddaschannel();
  void Reset();

  UInt_t GetEnergy() {return energy;}
  UInt_t GetTimeHigh() {return timehigh;}
  UInt_t GetTimeLow() {return timelow;}
  UInt_t GetCFDTime() {return timecfd;}
  Double_t GetTime() {return time;}
  Double_t GetCFD() {return cfd;}
  UInt_t GetEnergySums(Int_t i) {return energySums[i];}
  Int_t GetChannelNum() {return channelnum;}
  Int_t GetFinishCode() {return finishcode;}
  Int_t GetChannelLength() {return channellength;}
  Int_t GetChannelLengthHeader() {return channelheaderlength;}
  Int_t GetOverflowCode() {return overflowcode;}
  Int_t GetSlotID() {return slotid;}
  Int_t GetCrateID() {return crateid;}
  Int_t GetChannelID() {return chanid;}
  Int_t GetID() {return id;}
  UInt_t GetCFDTriggerSourceBit() {return cfdtrigsourcebit;}

  vector<UShort_t> GetTrace() {return trace;}

  void SetChannelID(UInt_t data);
  void SetSlotID(UInt_t data);
  void SetCrateID(UInt_t data);
  void SetChannelHeaderLength(UInt_t data);
  void SetChannelLength(UInt_t data);
  void SetOverflowCode(UInt_t data);
  void SetFinishCode(UInt_t data);
  void SetID(UInt_t data);
  void SetCFDTriggerSourceBit(UInt_t data);

  void SetTimeLow(UInt_t data);
  void SetTimeHigh(UInt_t data);
  void SetTimeCFD(UInt_t data);
  void SetTime();
  void SetEnergy(UInt_t data);
  void SetTraceLength(UInt_t data);

  void SetEnergySums(UInt_t data);
  void SetQDCSums(UInt_t data);
  void SetTraceValues(UInt_t data);

  void UnpackChannelData(const uint32_t *data);

  ClassDef(ddaschannel, 2)
};

#endif
