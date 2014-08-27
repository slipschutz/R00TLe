

#include "LendaChannel.hh"

LendaChannel::LendaChannel(){
  Clear();
}

LendaChannel::~LendaChannel(){
  Clear();
}
void LendaChannel::Clear(){
  _channel=BAD_NUM;
  _slot=BAD_NUM;
  _globalID=BAD_NUM;
  _refGlobalID=BAD_NUM;

  _overFlow=0;
  _numZeroCrossings=BAD_NUM;
  
  _correctedEnergy=BAD_NUM;
  _energy=BAD_NUM;
  _internalEnergy=BAD_NUM;
  _pulseHeight=BAD_NUM;
  _filterHeight=BAD_NUM;

  _time=BAD_NUM;
  _softTime=BAD_NUM;
  _cubicTime=BAD_NUM;
  _cubicFitTime=BAD_NUM;
  _correctedTime=BAD_NUM;
  _correctedCubicFitTime=BAD_NUM;
  _correctedSoftTime=BAD_NUM;

  _timeLow=0;
  _timeHigh=0;
  
  _softwareCFD=BAD_NUM;
  _cubicCFD=BAD_NUM;
  _cubicFitCFD=BAD_NUM;
  _internalCFD=BAD_NUM;
  _CFDTrigBit=0;
  
  _shortGate=BAD_NUM;
  _longGate=BAD_NUM;

  _jentry=BAD_NUM;

  _refTime=BAD_NUM;
  _cubicRefTime=BAD_NUM;
  _softRefTime=BAD_NUM;

  _trace.clear();
  _filter.clear();
  _CFD.clear();

  _CFDResidual=BAD_NUM;

  _channelName="";
  _referenceChannelName="";
}


Bool_t LendaChannel::operator==(const LendaChannel & RHS){

  if (this->_channel==RHS._channel&&
      this->_slot==RHS._slot&&
      this->_globalID==RHS._globalID&&
      this->_refGlobalID==RHS._refGlobalID&&
      this->_overFlow==RHS._overFlow&&
      this->_numZeroCrossings==RHS._numZeroCrossings&&
      this->_correctedEnergy==RHS._correctedEnergy&&
      this->_energy==RHS._energy&&
      this->_internalEnergy==RHS._internalEnergy&&
      this->_pulseHeight==RHS._pulseHeight&&
      this->_filterHeight==RHS._filterHeight&&
      this->_time==RHS._time&&
      this->_softTime==RHS._softTime&&
      this->_cubicTime==RHS._cubicTime&&
      this->_cubicFitTime==RHS._cubicFitTime&&
      this->_correctedTime==RHS._correctedTime&&
      this->_correctedCubicFitTime==RHS._correctedCubicFitTime&&
      this->_correctedSoftTime==RHS._correctedSoftTime&&
      this->_timeLow==RHS._timeLow&&
      this->_timeHigh==RHS._timeHigh&&
      this->_softwareCFD==RHS._softwareCFD&&
      this->_cubicCFD==RHS._cubicCFD&&
      this->_cubicFitCFD==RHS._cubicFitCFD&&
      this->_internalCFD==RHS._internalCFD&&
      this->_CFDTrigBit==RHS._CFDTrigBit&&
      this->_shortGate==RHS._shortGate&&
      this->_longGate==RHS._longGate&&
      this->_jentry==RHS._jentry&&
      this->_refTime==RHS._refTime&&
      this->_cubicRefTime==RHS._cubicRefTime&&
      this->_softRefTime==RHS._softRefTime&&
      this->_CFDResidual==RHS._CFDResidual&&
      this->_channelName==RHS._channelName&&
      this->_referenceChannelName==RHS._referenceChannelName){
    return true;
  }else {
    return false;
  }
}
