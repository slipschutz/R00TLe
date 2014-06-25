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
  TheObjectScintilator.Clear();

  NumUnMappedChannels=0;
  UnMappedChannels.clear();
  
  TOF=BAD_NUM;
  CorrectedTOF=BAD_NUM;
}
void LendaEvent::Finalize(){
  int tot=0;
  for (int i=0;i<Bars.size();i++){
    Bars[i].Finalize();
    tot=Bars[i].BarMultiplicity+tot;
  }
  N=tot+NumUnMappedChannels;
  NumOfChannelsInEvent=N;
  
  if ( Bars.size()==1 ){//IF only one bar in Event
    TOF=Bars[0].GetAvgT() - TheObjectScintilator.GetTime();
    CorrectedTOF =Bars[0].GetCorrectedAvgT() -TheObjectScintilator.GetCorrectedTime();
  }
}



void LendaEvent::Fatal(){
  cout<<"This Method should not exist.  Don't call it"<<endl;
}




