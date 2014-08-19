


#include "Scalers.hh"




Scalers::Scalers(): 
fIntervalStart(0),fIntervalEnd(0),fTimeStamp(0),
fScalerCount(0){
  fScalers.clear();
  fTotalScalers.clear();
}

Scalers::~Scalers(){
  fScalers.clear();


}

void Scalers::Reset(){
  fIntervalStart=0;
  fIntervalEnd=0;
  fTimeStamp=0;
  fScalerCount=0;
  fScalers.clear();
  //Don't clear the total Scalersvector
}


void Scalers::SetScalerItem(ScalerItemNoHeader* scaleItem){
  
  fIntervalStart = scaleItem->s_intervalStartOffset;
  fIntervalEnd = scaleItem->s_intervalEndOffset;
  fTimeStamp = scaleItem->s_timestamp;
  fScalerCount=scaleItem->s_scalerCount;

  if (fScalerCount != fTotalScalers.size()){
    fTotalScalers.resize(fScalerCount,0);
    fScalerRates.resize(fScalerCount,0);
  }

  fTimeInterval=fIntervalEnd-fIntervalStart;
  
  for (uint32_t i=0;i<fScalerCount;i++){
    fScalers.push_back(scaleItem->s_scalers[i]);

    fScalerRates[i]=fScalers[i]/fTimeInterval;

    fTotalScalers[i]+=fScalers[i];

  }


}


