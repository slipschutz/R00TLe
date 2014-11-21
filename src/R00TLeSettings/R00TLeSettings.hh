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
#include <cstdio>
using namespace std;

class R00TLeSettings : public TNamed{


public:
  R00TLeSettings();
 ~R00TLeSettings();
  
  void PrintAll();
  void PrintChannelCorrections(string Name);
  void PrintChannelMapInfo(int GlobalID);
  void PrintChannelMapInfo(string Name);
  
  double GetChannelsSlope(string Name){return TheEnergySlopes[Name];}
  double GetChannelsIntercept(string Name){return TheEnergyIntercepts[Name];}
  double GetChannelsTimeOffset(string Name){return TheTimingOffsets[Name];}
  
  int GetBarId(string Name){return BarIds[Name];}
  string GetBarName(int BarId){return BarId2Name[BarId];}

  void AddCorrectionSettings(string Name,double slope,double inter,double toff);
  void AddMapSettings(string Name,int GlobalID,string RefName, int refGlobalID);

  void SetBarIds(map<string,int> v);
  
  inline Int_t GetNumBars(){return BarIds.size();}

  void SetFilter(int fl_, int fg_ ,int d_, int w_){fl=fl_;fg=fg_;d=d_;w=w_;}
  
  void PrintFilterSettings(){
    cout<<"FL: "<<fl<<" FG: "<<fg<<" d: "<<d<<" w: "<<w<<endl;
  }

private:
  map <string, double> TheTimingOffsets;
  map <string, double> TheEnergySlopes;
  map <string, double> TheEnergyIntercepts;
  
  map <int, string> GlobalID2FullName;
  map <int, int> GlobalID2RefGlobalID;
  map <int,string> GlobalID2RefName;

  map<string,int> Name2GlobalID;

  map<string,int> BarIds;
  void BuildReverseMap();
  map<int,string> BarId2Name;


  int fl,fg,w,d;

public:
  ClassDef(R00TLeSettings,1);
};


#endif 
