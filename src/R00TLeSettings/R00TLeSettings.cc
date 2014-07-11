#include "R00TLeSettings.hh"

R00TLeSettings::R00TLeSettings(){

  this->fName="theSettings";
  this->fTitle="The Settings from the Builder";
  
}

R00TLeSettings::~R00TLeSettings(){

}



void R00TLeSettings::AddSettings(string Name,double slope,double inter,double toff){
  TheTimmingOffSets[Name] = toff;
  TheEnergySlopes[Name]=slope;
  TheEnergyIntercepts[Name]=inter;

}
