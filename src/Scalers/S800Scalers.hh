/*

  Header for the S800 Scalers class

 */
#ifndef __S800SCALERS_HH
#define __S800SCALERS_HH

#include <TFile.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <cstdio>

#include "Scalers.hh"

using namespace std;

class S800Scalers : public Scalers{


public:
  S800Scalers();
  ~S800Scalers();
  

private:


public:
  ClassDef(S800Scalers,1);
};



#endif /* __S800SCALERS_HH */


