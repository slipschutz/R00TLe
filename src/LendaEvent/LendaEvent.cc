#include "LendaEvent.hh"
#include <iostream>
#include "TMath.h"
#include <sstream>

using namespace std;


LendaEvent::LendaEvent(){  

  Clear();

}



void LendaEvent::Clear(){
  ////REMEBER TO CLEAR THINGS THAT were thing.push_Back!!!!
  
  //Clear the main information holder the bars vector
  
  for (int i=0;i<Bars.size();i++){
    Bars[i].Clear();
  }

  Bars.clear();//Will call deconstructor on sub objects
  N=0;
  NumBars=0;
  NumOfChannelsInEvent=0;
  TheObjectScintillators.clear();//Clear Vector of Obj scintillators

  NumUnMappedChannels=0;
  NumObjectScintillators=0;
  UnMappedChannels.clear();
  
}
void LendaEvent::Finalize(){
  int tot=0;
  for (int i=0;i<Bars.size();i++){
    Bars[i].Finalize();
    tot=Bars[i].BarMultiplicity+tot;
  }
  NumObjectScintillators=TheObjectScintillators.size();
  
  N=tot+NumUnMappedChannels+NumObjectScintillators;
  NumOfChannelsInEvent=N;
  
}




Bool_t LendaEvent::operator==(const LendaEvent & RHS){

  //First Calculated quantities
  bool CalcValuesEqual=true;
  
  if (this->N == RHS.N&&
      this->NumBars == RHS.NumBars &&
      this->NumOfChannelsInEvent==RHS.NumOfChannelsInEvent&&
      this->NumUnMappedChannels==RHS.NumUnMappedChannels&&
      this->NumObjectScintillators==RHS.NumObjectScintillators){
    CalcValuesEqual=true;
  }else {
    cout<<"Event Failed"<<endl;
    return false;
  }

  for (int i=0;i<Bars.size();i++){
    if (!(this->Bars[i] == RHS.Bars[i])){
      return false;
    }
  }
  
  for (int i=0;i<TheObjectScintillators.size();i++){
    if (!(this->TheObjectScintillators[i]==RHS.TheObjectScintillators[i])){
      return false;
    }
  }
  
  for (int i=0;i<UnMappedChannels.size();i++){
    if (!(this->UnMappedChannels[i]==RHS.UnMappedChannels[i])){
      return false;
    }
  }

  return true;
  
}

