/*

  Header For ROOT Settings Object.
  Will Store information for various builds.
  


 */
#ifndef __SETTINGS_HH
#define __SETTINGS_HH

#include <TFile.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class R00TLeSettings : public TNamed{


public:
  R00TLeSettings();
 ~R00TLeSettings();
  
  void PushLine(string);
  void print();
  inline vector <string>* GetInfoVector(){return &theInfo;}

private:
  vector <string> theInfo;

public:
  ClassDef(R00TLeSettings,1);
};


#endif 
