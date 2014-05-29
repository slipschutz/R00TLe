

/////////////////////////////////////////////////////////
// This is the header file for the LendaBar ROOT Dictionary //
/////////////////////////////////////////////////////////

#ifndef __LENDABAR_HH
#define __LENDABAR_HH
#include "TObject.h"
#include "LendaChannel.hh"


class LendaBar : public TObject{
public:
  LendaBar();
  ~LendaBar();
  
  //Storing a vector of Tops and bottoms
  //to allow for 0 or more than one of each
  //channel to be stored
  vector <LendaChannel> Tops;
  vector <LendaChannel> Bottoms;

public:
  ClassDef(LendaBar,1);
};


#endif
