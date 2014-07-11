#include "R00TLeSettings.hh"

R00TLeSettings::R00TLeSettings(){
  theInfo.clear();

  this->fName="theSettings";
  this->fTitle="The Settings from the Builder";
  
}

R00TLeSettings::~R00TLeSettings(){
  theInfo.clear();
}

void R00TLeSettings::PushLine(string s){
  theInfo.push_back(s);
}

void R00TLeSettings::print(){
  cout<<"******Settings Information******"<<endl;
  for (int i=0;i<theInfo.size();i++){
    cout<<theInfo[i];
  }

}
