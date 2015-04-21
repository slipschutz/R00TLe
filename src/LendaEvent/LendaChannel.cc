

#include "LendaChannel.hh"

LendaChannel::LendaChannel(){
  Clear();
}

LendaChannel::~LendaChannel(){
  Clear();
}
void LendaChannel::Clear(){
  //The vectors that hold the many times
  //and the many energies
  //need for when this channel hold info for high
  //rate event
  rSoftwareTimes.clear();
  rEnergies.clear();
  rCubicTimes.clear();
  rPulseHeights.clear();
  rSoftwareReferenceTimes.clear();
  rCubicReferenceTimes.clear();

  rSoftwareTimes.push_back(BAD_NUM);
  rEnergies.push_back(BAD_NUM);
  rCubicTimes.push_back(BAD_NUM);
  rPulseHeights.push_back(BAD_NUM);
  rSoftwareReferenceTimes.push_back(BAD_NUM);
  rCubicReferenceTimes.push_back(BAD_NUM);

  rChannel=BAD_NUM;
  rSlot=BAD_NUM;
  rGlobalID=BAD_NUM;
  rRefGlobalID=BAD_NUM;

  rOverFlow=0;
  rNumZeroCrossings=BAD_NUM;
  
  rCorrectedEnergy=BAD_NUM;
  
  rInternalEnergy=BAD_NUM;
  
  rFilterHeight=BAD_NUM;

  rTime=BAD_NUM;
  rCubicFitTime=BAD_NUM;

  rCorrectedTime=BAD_NUM;
  rCorrectedCubicFitTime=BAD_NUM;
  rCorrectedCubicTime=BAD_NUM;
  rCorrectedSoftTime=BAD_NUM;


  rTimeLow=0;
  rTimeHigh=0;
  
  rSoftwareCFD=BAD_NUM;
  rCubicCFD=BAD_NUM;
  rCubicFitCFD=BAD_NUM;
  rInternalCFD=BAD_NUM;
  rCFDTrigBit=0;
  
  rShortGate=BAD_NUM;
  rLongGate=BAD_NUM;

  rJentry=BAD_NUM;

  rRefTime=BAD_NUM;
  // rCubicRefTime=BAD_NUM;
  // rSoftRefTime=BAD_NUM;

  rTrace.clear();
  rFilter.clear();
  rCFD.clear();

  rCFDResidual=BAD_NUM;

  rChannelName="";
  rReferenceChannelName="";

  
  rOtherTime=BAD_NUM;
}


Bool_t LendaChannel::operator==(const LendaChannel & RHS){
  
  // if (this->_channel==RHS._channel&&
  //     this->_slot==RHS._slot&&
  //     this->_globalID==RHS._globalID&&
  //     this->_refGlobalID==RHS._refGlobalID&&
  //     this->_overFlow==RHS._overFlow&&
  //     this->_numZeroCrossings==RHS._numZeroCrossings&&
  //     // this->_correctedEnergy==RHS._correctedEnergy&&
  //     //      this->_energy==RHS._energy&&
  //     this->_internalEnergy==RHS._internalEnergy&&
  //     this->_pulseHeight==RHS._pulseHeight&&
  //     this->_filterHeight==RHS._filterHeight&&
  //     this->_time==RHS._time&&
  //     this->_softTime==RHS._softTime&&
  //     this->_cubicTime==RHS._cubicTime&&
  //     this->_cubicFitTime==RHS._cubicFitTime&&
  //     this->_correctedTime==RHS._correctedTime&&
  //     this->_correctedCubicFitTime==RHS._correctedCubicFitTime&&
  //     //      this->_correctedSoftTime==RHS._correctedSoftTime&&
  //     this->_timeLow==RHS._timeLow&&
  //     this->_timeHigh==RHS._timeHigh&&
  //     this->_softwareCFD==RHS._softwareCFD&&
  //     this->_cubicCFD==RHS._cubicCFD&&
  //     this->_cubicFitCFD==RHS._cubicFitCFD&&
  //     this->_internalCFD==RHS._internalCFD&&
  //     this->_CFDTrigBit==RHS._CFDTrigBit&&
  //     //      this->_shortGate==RHS._shortGate&&
  //     //      this->_longGate==RHS._longGate&&
  //     this->_jentry==RHS._jentry&&
  //     this->_refTime==RHS._refTime&&
  //     this->_cubicRefTime==RHS._cubicRefTime&&
  //     //      this->_softRefTime==RHS._softRefTime&&
  //     this->_CFDResidual==RHS._CFDResidual&&
  //     this->_channelName==RHS._channelName&&
  //     this->_referenceChannelName==RHS._referenceChannelName){
  //   return true;
  // }else {
  //   cout<<"Channel Fail"<<endl;
  //   //  ShowDiff(RHS);
  //   return false;
  // }
  cout<<"<LendaChannel::operator==()> Needs to be updated"<<endl;
  return false;
  // if (this->_channel==RHS._channel&&
  //     this->_slot==RHS._slot&&
  //     this->_softRefTime==RHS._softRefTime&&]
  //     this->_globalID==RHS._globalID){
  //   return true;
  // }else {
  //   cout<<"Channel Failed"<<endl;
  //   return false;
  // }


}

/*
void LendaChannel::ShowDiff(const LendaChannel& RHS){
  
  cout<<"---------------------------------"<<endl;
  
  cout<<"Channel "<<this->_channel<<"  "<<RHS._channel<<endl;
  cout<<"Slot "<<this->_slot<<"  "<<RHS._slot<<endl;
  cout<<"GlobalID "<<this->_globalID<<"  "<<RHS._globalID<<endl;
  cout<<"RefGlobalID "<<this->_refGlobalID<<"  "<<RHS._refGlobalID<<endl;
  cout<<"OverFlow "<<this->_overFlow<<"  "<<RHS._overFlow<<endl;
  cout<<"numZero "<<this->_numZeroCrossings<<"  "<<RHS._numZeroCrossings<<endl;
  cout<<"CorrecctedE "<<this->_correctedEnergy<<"  "<<RHS._correctedEnergy<<endl;
  cout<<"Energy "<<this->_energy<<"  "<<RHS._energy<<endl;
  cout<<"internE "<<this->_internalEnergy<<"  "<<RHS._internalEnergy<<endl;
  cout<<"PulseHeight "<<this->_pulseHeight<<"  "<<RHS._pulseHeight<<endl;
  cout<<"filterHeight "<<this->_filterHeight<<"  "<<RHS._filterHeight<<endl;
  cout<<"Time "<<this->_time<<"  "<<RHS._time<<endl;
  cout<<"SoftTime "<<this->_softTime<<"  "<<RHS._softTime<<endl;
  cout<<"cubicTime "<<this->_cubicTime<<"  "<<RHS._cubicTime<<endl;
  cout<<"CubicFit "<<this->_cubicFitTime<<"  "<<RHS._cubicFitTime<<endl;
  cout<<"CorTime "<<this->_correctedTime<<"  "<<RHS._correctedTime<<endl;
  cout<<"CorCubicFitTime "<<this->_correctedCubicFitTime<<"  "<<RHS._correctedCubicFitTime<<endl;

  cout<<"timeLow "<<this->_timeLow<<"  "<<RHS._timeLow<<endl;
  cout<<"timeHigh "<<this->_timeHigh<<"  "<<RHS._timeHigh<<endl;
  cout<<"softCFD "<<this->_softwareCFD<<"  "<<RHS._softwareCFD<<endl;
  cout<<"cubicCFD "<<this->_cubicCFD<<"  "<<RHS._cubicCFD<<endl;
  cout<<"cubicFitCFD "<<this->_cubicFitCFD<<"  "<<RHS._cubicFitCFD<<endl;
  cout<<"internCFD "<<this->_internalCFD<<"  "<<RHS._internalCFD<<endl;
  cout<<"CFDTrigBit "<<this->_CFDTrigBit<<"  "<<RHS._CFDTrigBit<<endl;
  cout<<"shortGate "<<this->_shortGate<<"  "<<RHS._shortGate<<endl;
  cout<<"longGate "<<this->_longGate<<"  "<<RHS._longGate<<endl;
  cout<<"jentry "<<this->_jentry<<"  "<<RHS._jentry<<endl;
  cout<<"refTime "<<this->_refTime<<"  "<<RHS._refTime<<endl;
  cout<<"cubicRefTime "<<this->_cubicRefTime<<"  "<<RHS._cubicRefTime<<endl;

  cout<<"cfdResidual "<<this->_CFDResidual<<"  "<<RHS._CFDResidual<<endl;
  cout<<"Channel Name "<<this->_channelName<<"  "<<RHS._channelName<<endl;
  cout<<"refChannelName "<<this->_referenceChannelName<<"  "<<RHS._referenceChannelName<<endl;
  

  cout<<"---------------------------------"<<endl;




}
*/
