

/////////////////////////////////////////////////////////
// This is the header file for the LendaBar ROOT Dictionary //
/////////////////////////////////////////////////////////

#ifndef __LENDABAR_HH
#define __LENDABAR_HH
#include "TObject.h"
#include "LendaChannel.hh"
#include <string>
#include <vector>
using namespace std;

class LendaBar : public TObject{
public:
  LendaBar();
  LendaBar(string s) : Name(s) {}
  ~LendaBar();
  
  //Storing a vector of Tops and bottoms
  //to allow for 0 or more than one of each
  //channel to be stored
  vector <LendaChannel> Tops;
  vector <LendaChannel> Bottoms;
  
  void PushTopChannel(LendaChannel c){Tops.push_back(c);}
  void PushBottomChannel(LendaChannel c){Bottoms.push_back(c);}

  string Name;//the name of the bar to make it unique
  Int_t NumTops;
  Int_t NumBottoms;
  Int_t BarMultiplicity;
  void Finalize();//Method to calcualte things like Dt, COG, AvgE, TOF

private:
  

public:
  ClassDef(LendaBar,1);
};


#endif
