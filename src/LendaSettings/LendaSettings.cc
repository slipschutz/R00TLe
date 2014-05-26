#include "LendaSettings.hh"

LendaSettings::LendaSettings(){
  theInfo.clear();

  this->fName="theSettings";
  this->fTitle="The Settings from the Builder";
  
}

LendaSettings::~LendaSettings(){
  theInfo.clear();
}

void LendaSettings::PushLine(string s){
  theInfo.push_back(s);
}

void LendaSettings::Print(){
  cout<<"******Settings Information******"<<endl;
  for (int i=0;i<theInfo.size();i++){
    cout<<theInfo[i];
  }

}
