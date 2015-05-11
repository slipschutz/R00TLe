#include "LendaEvent.hh"
#include <iostream>
#include "TMath.h"
#include <sstream>

using namespace std;


LendaEvent::LendaEvent(){  

  Clear();

}



void LendaEvent::Clear(){
  //REMEBER TO CLEAR THINGS THAT were thing.push_Back!!!!
  
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

LendaBar * LendaEvent::FindBar(int BarId){
  for (int i=0;i<(int)Bars.size();i++){
    if (Bars[i].BarId==BarId){
      return &(Bars[i]);
    }
  }
  //Finished list of bars.  Bar wasn't there
  return NULL;
}


LendaBar * LendaEvent::FindBar(string Name){
  for (int i=0;i<(int)Bars.size();i++){
    if (Bars[i].Name==Name){
      return &(Bars[i]);
    }
  }
  //Finished list of bars.  Bar wasn't there
  return NULL;
}


LendaBar LendaEvent::FindBarSafe(string Name){
   for (int i=0;i<(int)Bars.size();i++){
    if (Bars[i].Name==Name){
      return (Bars[i]);
    }
  }
   //Could not find the bar return an empty bar object
   return LendaBar();
}


LendaBar LendaEvent::FindBarSafe(int BarId){
  for (int i=0;i<(int)Bars.size();i++){
    if (Bars[i].BarId==BarId){
      return (Bars[i]);
    }
  }
  //Could not find the bar return an empty bar object
  return LendaBar();
 }

LendaChannel * LendaEvent::FindChannel(string Name){
  //Perform linear search over the tops bottoms, Reference channels
  //Does not search in the UnMapped channels because those don't 
  //have a name

  for (auto & bars : Bars){

    for (auto & i : bars.Tops){
      if (Name == i.GetName()){
	return &i;
      }
    }
    for (auto & i : bars.Bottoms){
      if (Name == i.GetName()){
	return &i;
      }
    }
  }
  
  for (auto & i : TheObjectScintillators){
    if (Name == i.GetName()){
      return &i;
    }
  }

  //Channel has not been found return NULL
  return NULL;

}

LendaChannel* LendaEvent::FindReferenceChannel(string Name){

  for (auto & i : TheObjectScintillators){
    if (Name == i.GetName()){
      //This is the correct channel
      return &i;
    }
  }
  //Could not find the channel retrun NULL
  return NULL;
}
