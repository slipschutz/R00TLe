#include "R00TLeSettings.hh"

R00TLeSettings::R00TLeSettings(){

  this->fName="TheSettings";
  this->fTitle="R00TLe's Settings";
  
}

R00TLeSettings::~R00TLeSettings(){

}



void R00TLeSettings::AddCorrectionSettings(string Name,double slope,double inter,double toff){
  TheTimingOffsets[Name] = toff;
  TheEnergySlopes[Name]=slope;
  TheEnergyIntercepts[Name]=inter;

}

void R00TLeSettings::AddMapSettings(string Name,int GlobalID,string RefName, int refGlobalID){
  GlobalID2FullName[GlobalID] = Name;
  GlobalID2RefGlobalID[GlobalID] = refGlobalID;
  GlobalID2RefName[GlobalID] = RefName;
  
  Name2GlobalID[Name]=GlobalID;
}



void R00TLeSettings::PrintAll(){

  for (auto & ii : TheTimingOffsets){
    cout<<endl;
    PrintChannelCorrections(ii.first);
    PrintChannelMapInfo(Name2GlobalID[ii.first]);
  }

}

void R00TLeSettings::PrintChannelCorrections(string Name){
  printf("Channel Name %6s has slope %10.4lf has intercept %10.4lf and timming offset %10.4lf\n",Name.c_str(),
	 TheEnergySlopes[Name],TheEnergyIntercepts[Name],TheTimingOffsets[Name]);

}

void R00TLeSettings::PrintChannelMapInfo(int GlobalID){
  printf("Channel Name %6s has DDAS ID %4d it's reference channel is %6s whith DDAS ID %4d\n",GlobalID2FullName[GlobalID].c_str(),
	 GlobalID,GlobalID2RefName[GlobalID].c_str(),GlobalID2RefGlobalID[GlobalID]);


}

void R00TLeSettings::PrintChannelMapInfo(string Name){
  PrintChannelMapInfo(Name2GlobalID[Name]);
}



void R00TLeSettings::SetBarIds(map<string,int> v){
  BarIds=v;
  BuildReverseMap();
}

void R00TLeSettings::BuildReverseMap(){
  for (auto & ii : BarIds){
 
    BarId2Name[ii.second]=ii.first;
  }
}
