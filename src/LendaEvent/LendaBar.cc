



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

  CorrectedCubicTopTOF=BAD_NUM;
  CorrectedCubicBottomTOF=BAD_NUM;

  CorrectedCubicFitTopTOF=BAD_NUM;
  CorrectedCubicFitBottomTOF=BAD_NUM;

  CorrectedSoftTopTOF=BAD_NUM;
  CorrectedSoftBottomTOF=BAD_NUM;
}

void LendaBar::Finalize(){
  NumTops=Tops.size();
  NumBottoms=Bottoms.size();
  BarMultiplicity=NumTops+NumBottoms;

  if (NumTops == 1 ){
    TopTOF = Tops[0].GetTime()-Tops[0].GetReferenceTime();
    CubicTopTOF = Tops[0].GetCubicTime()-Tops[0].GetCubicReferenceTime();

    CorrectedTopTOF = Tops[0].GetCorrectedTime()-Tops[0].GetReferenceTime();
    CorrectedCubicTopTOF=Tops[0].GetCorrectedCubicTime()-Tops[0].GetCubicReferenceTime();
    CorrectedSoftTopTOF=Tops[0].GetCorrectedSoftTime() - Tops[0].GetSoftReferenceTime();
  } 
  if (NumBottoms == 1 ){
    BottomTOF =Bottoms[0].GetTime()-Bottoms[0].GetReferenceTime();
    CubicBottomTOF = Bottoms[0].GetCubicTime()-Bottoms[0].GetCubicReferenceTime();

    CorrectedBottomTOF =Bottoms[0].GetCorrectedTime()-Bottoms[0].GetReferenceTime();
    CorrectedCubicBottomTOF=Bottoms[0].GetCorrectedCubicTime()-Bottoms[0].GetCubicReferenceTime();
    CorrectedSoftBottomTOF=Bottoms[0].GetCorrectedSoftTime() - Bottoms[0].GetSoftReferenceTime();
  }


  //For simple events IE ones with only 1 Top and 1 Bottom
  //Caluclate some nice quantities
  if (NumTops==1 && NumBottoms==1){
    SimpleEventBit=1;

    Dt=Tops[0].GetTime()-Bottoms[0].GetTime();
    CubicDt=Tops[0].GetCubicTime()-Bottoms[0].GetCubicTime();

    SyncDt =CorrectedTopTOF-CorrectedBottomTOF;
    CubicSyncDt=CorrectedCubicTopTOF-CorrectedCubicBottomTOF;

    COG=(Tops[0].GetEnergy()-Bottoms[0].GetEnergy())/(Tops[0].GetEnergy()+Bottoms[0].GetEnergy());

    if ( Tops[0].GetEnergy() >0 && Bottoms[0].GetEnergy()>0){
      AvgEnergy=TMath::Sqrt(Tops[0].GetEnergy()*Bottoms[0].GetEnergy());
    }

    AvgPulseHeight=TMath::Sqrt(Tops[0].GetPulseHeight()*Bottoms[0].GetPulseHeight());
    
    CorrectedCOG=(Tops[0].GetCorrectedEnergy()-Bottoms[0].GetCorrectedEnergy())/(Tops[0].GetCorrectedEnergy()+Bottoms[0].GetCorrectedEnergy());
    CorrectedDt=Tops[0].GetCorrectedTime()-Bottoms[0].GetCorrectedTime();

    if ( Tops[0].GetCorrectedEnergy()>0 && Bottoms[0].GetCorrectedEnergy()>0){
      CorrectedAvgEnergy=TMath::Sqrt(Tops[0].GetCorrectedEnergy()*Bottoms[0].GetCorrectedEnergy());
    }
    
    AvgT=0.5*(Tops[0].GetTime() + Bottoms[0].GetTime());
    CorrectedAvgT=0.5*(Tops[0].GetCorrectedTime()+Bottoms[0].GetCorrectedTime());


    
  }	  
  

  

}


Bool_t LendaBar::operator==(const LendaBar & RHS){

  Bool_t CalcValuesEqual=true;
  //check to see if the Calculated values are equal
  //Things like Dt, AvgT
  if (this->Dt==RHS.Dt&&
      this->CubicDt==RHS.CubicDt&&
      //this->SyncDt==RHS.SyncDt&&
      //this->CubicSyncDt==RHS.CubicSyncDt&&
      //      this->COG==RHS.COG&&
      // this->AvgEnergy==RHS.AvgEnergy&&
      this->AvgPulseHeight==RHS.AvgPulseHeight&&
      // this->CorrectedCOG==RHS.CorrectedCOG&&
      this->CorrectedDt==RHS.CorrectedDt &&
      //this->CorrectedAvgEnergy==RHS.CorrectedAvgEnergy&&
      this->AvgT==RHS.AvgT&&
      this->CorrectedAvgT==RHS.CorrectedAvgT&&
      this->BarId==RHS.BarId&&
      this->TopTOF==RHS.TopTOF&&
      this->BottomTOF==RHS.BottomTOF){
      // this->CorrectedTopTOF==RHS.CorrectedTopTOF&&
      // this->CorrectedBottomTOF==RHS.CorrectedBottomTOF&&
      // this->CorrectedCubicTopTOF==RHS.CorrectedCubicTopTOF&&
      // this->CorrectedCubicBottomTOF==RHS.CorrectedCubicBottomTOF&&
      // this->CorrectedSoftTopTOF==RHS.CorrectedSoftTopTOF&&
      // this->CorrectedSoftBottomTOF==RHS.CorrectedSoftBottomTOF){
    CalcValuesEqual=true;
  }else{
    //The calculated valeus are not equal
    //Can return false
    cout<<"BAR Failed"<<endl;
    this->Dump();
    RHS.Dump();
    exit(1);
    return false;
  }

  
   //Check to see if the Top/Bottom channels are
  //Equal.
  for (int i=0;i<Tops.size();i++){
    if (!(this->Tops[i] == RHS.Tops[i])){
      return false;
    }
  }
  
  for (int i=0;i<Bottoms.size();i++){
    if (!(this->Bottoms[i] == RHS.Bottoms[i])){
      return false;
    }
  }

  //If it has made it through all the checks return true

  return true;

}


