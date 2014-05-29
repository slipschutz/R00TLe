#include "LendaEvent.hh"
#include <iostream>
#include "TMath.h"
#include <sstream>

using namespace std;


#define BAD_NUM -1008
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
  Bars.clear();//Will call deconstructor on sub objects
  N=0;
  NumBars=0;
  NumOfChannelsInEvent=0;
}
void LendaEvent::Finalize(){
 
}



void LendaEvent::Fatal(){
  cout<<"This Method should not exist.  Don't call it"<<endl;
}




