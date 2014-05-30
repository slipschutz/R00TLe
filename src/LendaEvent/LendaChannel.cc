

#include "LendaChannel.hh"
#define BAD_NUM -10008

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
  
  _overFlow=0;
  _numZeroCrossings=BAD_NUM;
  
  _correctedEnergy=BAD_NUM;
  _energy=BAD_NUM;
  _internalEnergy=BAD_NUM;
  _pulseHeight=BAD_NUM;

  _time=BAD_NUM;
  _softTime=BAD_NUM;
  _cubicTime=BAD_NUM;
  _cubicFitTime=BAD_NUM;

  _timeLow=BAD_NUM;
  _timeHigh=BAD_NUM;
  
  _softwareCFD=BAD_NUM;
  _cubicCFD=BAD_NUM;
  _cubicFitCFD=BAD_NUM;
  _internalCFD=BAD_NUM;
  _CFDTrigBit=0;
  
  _shortGate=BAD_NUM;
  _longGate=BAD_NUM;

  _jentry=BAD_NUM;

  _trace.clear();
  _filter.clear();
  _CFD.clear();

}

// #include <iostream>
// using namespace std;
// LendaChannel & LendaChannel::operator=(const LendaChannel & rhs){

//   cout<<"Hello From the = operator"<<endl;
//   cout<<"the rhs address is "<<&rhs<<endl;
//   cout<<"this is "<<this<<endl;
  
//   this->_channel= rhs._channel;


//   return *this;

// }