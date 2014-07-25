


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
  CubicDt=BAD_NUM;

  SyncDt=BAD_NUM;
  CubicSyncDt=BAD_NUM;
  
  COG=BAD_NUM;
  AvgEnergy=BAD_NUM;
  AvgPulseHeight=BAD_NUM;

  CorrectedCOG=BAD_NUM;
  CorrectedDt=BAD_NUM;
  CorrectedAvgEnergy=BAD_NUM;

  AvgT=BAD_NUM;
  CorrectedAvgT=BAD_NUM;
  SimpleEventBit=0;

  Name="_No_NAME_";
  BarId=BAD_NUM;

  TopTOF=BAD_NUM;
  BottomTOF=BAD_NUM;
  
  CorrectedTopTOF=BAD_NUM;
  CorrectedBottomTOF=BAD_NUM;
}

void LendaBar::Finalize(){
  NumTops=Tops.size();
  NumBottoms=Bottoms.size();
  BarMultiplicity=NumTops+NumBottoms;

  if (NumTops == 1 ){
    TopTOF = Tops[0].GetTime()-Tops[0].GetReferenceTime();
    CubicTopTOF = Tops[0].GetCubicFitTime()-Tops[0].GetCubicReferenceTime();
    CorrectedTopTOF = Tops[0].GetCorrectedTime()-Tops[0].GetReferenceTime();
  } 
  if (NumBottoms == 1 ){
    BottomTOF =Bottoms[0].GetTime()-Bottoms[0].GetReferenceTime();
    CubicBottomTOF = Bottoms[0].GetCubicFitTime()-Bottoms[0].GetCubicReferenceTime();
    CorrectedBottomTOF =Bottoms[0].GetCorrectedTime()-Bottoms[0].GetReferenceTime();
  }


  //For simple events IE ones with only 1 Top and 1 Bottom
  //Caluclate some nice quantities
  if (NumTops==1 && NumBottoms==1){
    SimpleEventBit=1;

    Dt=Tops[0].GetTime()-Bottoms[0].GetTime();
    CubicDt=Tops[0].GetCubicFitTime()-Bottoms[0].GetCubicFitTime();

    SyncDt =TopTOF-BottomTOF;
    CubicSyncDt=CubicTopTOF-CubicBottomTOF;

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
