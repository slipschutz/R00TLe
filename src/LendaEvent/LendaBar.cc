


/////////////////////////////////////////////////////////
// This is the source file for the LendaBar ROOT class //
/////////////////////////////////////////////////////////

#include "LendaBar.hh"

LendaBar::LendaBar(){
  Clear();
}
LendaBar::~LendaBar(){
  Clear();
}

void LendaBar::Clear(){
  Tops.clear();
  Bottoms.clear();
  NumTops=0;
  NumBottoms=0;
  BarMultiplicity=0;

  Dt=BAD_NUM;
  COG=BAD_NUM;
  AvgEnergy=BAD_NUM;
  AvgPulseHeight=BAD_NUM;

  CorrectedCOG=BAD_NUM;
  CorrectedDt=BAD_NUM;
  CorrectedAvgEnergy=BAD_NUM;

  AvgT=BAD_NUM;
  CorrectedAvgT=BAD_NUM;
  SimpleEventBit=0;
}

void LendaBar::Finalize(){
  NumTops=Tops.size();
  NumBottoms=Bottoms.size();
  BarMultiplicity=NumTops+NumBottoms;

  //For simple events IE ones with only 1 Top and 1 Bottom
  //Caluclate some nice quantities
  if (NumTops==1 && NumBottoms==1){
    SimpleEventBit=1;
    Dt=Tops[0].GetTime()-Bottoms[0].GetTime();
    COG=(Tops[0].GetEnergy()-Bottoms[0].GetEnergy())/(Tops[0].GetEnergy()+Bottoms[0].GetEnergy());
    AvgEnergy=TMath::Sqrt(Tops[0].GetEnergy()*Bottoms[0].GetEnergy());
    AvgPulseHeight=TMath::Sqrt(Tops[0].GetPulseHeight()*Bottoms[0].GetPulseHeight());
    
    CorrectedCOG=(Tops[0].GetCorrectedEnergy()-Bottoms[0].GetCorrectedEnergy())/(Tops[0].GetCorrectedEnergy()+Bottoms[0].GetCorrectedEnergy());
    CorrectedDt=Tops[0].GetCorrectedTime()-Bottoms[0].GetCorrectedTime();

    CorrectedAvgEnergy=TMath::Sqrt(Tops[0].GetCorrectedEnergy()*Bottoms[0].GetCorrectedEnergy());
    
    
    AvgT=0.5*(Tops[0].GetTime() + Bottoms[0].GetTime());
    CorrectedAvgT=0.5*(Tops[0].GetCorrectedTime()+Bottoms[0].GetCorrectedTime());
  }

}
