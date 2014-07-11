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
#include <map>
using namespace std;

class R00TLeSettings : public TNamed{


public:
  R00TLeSettings();
 ~R00TLeSettings();
  

  
  void AddSettings(string Name,double slope,double inter,double toff);
  void SetBarIds(map<string,int> v){BarIds=v;}

private:
  map <string, double> TheTimmingOffSets;
  map <string, double> TheEnergySlopes;
  map <string, double> TheEnergyIntercepts;

  map<string,int> BarIds;

public:
  ClassDef(R00TLeSettings,1);
};


#endif 
