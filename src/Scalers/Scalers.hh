

///////////////////////////////////////////////////////////////////////////
// Header for the Scalers base class   A root dictionary to conatin the  //
// inforamation from  the NSCLDAQ data sturct ScalerItem excluding the 	 //
// ringitem header.							 //
// typedef struct _ScalerItem {						 //
//   //  RingItemHeader  s_header;					 //
//   uint32_t        s_intervalStartOffset;				 //
//   uint32_t        s_intervalEndOffset;				 //
//   uint32_t        s_timestamp;					 //
//   uint32_t        s_scalerCount;					 //
//   uint32_t        s_scalers[1];					 //
// } ScalerItem, *pScalerItem;						 //
///////////////////////////////////////////////////////////////////////////




#ifndef __SCALERS_HH
#define __SCALERS_HH


#include <TFile.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

#include "DAQdefs.h" //Local DataFormat Definitions
using namespace std;

class Scalers : public TObject{


public:
  Scalers();
  virtual ~Scalers();
  
  void SetScalerItem(ScalerItemNoHeader*);
  void Reset();
  
public:
  uint32_t fIntervalStart;
  uint32_t fIntervalEnd;
  uint32_t fTimeStamp;
  uint32_t fScalerCount;
  vector <uint32_t> fScalers;

  vector <long> fTotalScalers;
  vector <Double_t> fScalerRates;

  int fTimeInterval;
  

public:
  ClassDef(Scalers,1);
};




#endif /* __SCALERS_HH */

