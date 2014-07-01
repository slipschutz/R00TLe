#include "LendaEvent.hh"
#include <iostream>
#include "TMath.h"
#include <sstream>

using namespace std;


LendaEvent::LendaEvent(){  

  Clear();

}

void LendaEvent::WriteSettings(LendaSettings * theSettings){
  // theSettings->PushLine("####Settings From the LendaEvent####\n");
  // stringstream line;
  // line<<"TOF Fudge "<<TOFFudge<<endl;
  // line<<"GammaPeakTime "<<GammaPeakTime<<endl;
  // line<<"GammaPeakTimeLiqLiq "<<GammaPeakTimeLiqLiq<<endl;
  // theSettings->PushLine(line.str());
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



void LendaEvent::Fatal(){
  cout<<"This Method should not exist.  Don't call it"<<endl;
}




