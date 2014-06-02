

/////////////////////////////////////////////////////////
// This is the header file for the LendaBar ROOT class //
/////////////////////////////////////////////////////////

#ifndef __LENDABAR_HH
#define __LENDABAR_HH
#include "TObject.h"
#include "LendaChannel.hh"
#include <string>
#include <vector>
#include "TMath.h"
using namespace std;

class LendaBar : public TObject{
public:
  LendaBar();
  LendaBar(string s) {Clear(); Name=s;}
  ~LendaBar();
  
  //Storing a vector of Tops and bottoms
  //to allow for 0 or more than one of each
  //channel to be stored
  vector <LendaChannel> Tops;
  vector <LendaChannel> Bottoms;
  
  void PushTopChannel(LendaChannel c){Tops.push_back(c);}
  void PushBottomChannel(LendaChannel c){Bottoms.push_back(c);}

  inline string GetName(){return Name;}
  inline Int_t GetNumTops(){return NumTops;}
  inline Int_t GetNumBottoms(){return NumBottoms;}
  inline Int_t GetBarMultiplicity(){return BarMultiplicity;}

  inline Double_t GetDt(){return Dt;}
  inline Double_t GetCOG(){return COG;}
  inline Double_t GetAvgEnergy(){return AvgEnergy;}
  inline Double_t GetAvgPulseHeight(){return AvgPulseHeight;}

  inline Double_t GetCorrectedDt(){return CorrectedDt;}
  inline Double_t GetCorrectedCOG(){return CorrectedCOG;}
  inline Double_t GetCorrectedAvgEnergy(){return CorrectedAvgEnergy;}

  inline Double_t GetAvgT(){return AvgT;}
  inline Double_t GetCorrectedAvgT(){return AvgT;}
  

  void SetBarId(Int_t v){BarId=v;}
  void SetName(string v){Name=v;}

  void Clear();

  string Name;//the name of the bar to make it unique
  Int_t BarId; //The Unique BarID Defined by the order in MapFile

  Int_t NumTops;
  Int_t NumBottoms;
  Int_t BarMultiplicity;
  
  Bool_t SimpleEventBit; //true if bar has only 1 Top and 1 bottom

  void Finalize();//Method to calcualte things like Dt, COG, AvgE, TOF


private:
  Double_t Dt;//Top Time - Bottom Time
  Double_t COG;//ET -EB/(ET+EB)
  Double_t AvgEnergy;//Sqrt(ET*EB)
  Double_t AvgPulseHeight;//Sqrt(PH_T*PH_B)

  Double_t AvgT;
  Double_t CorrectedAvgT;

  Double_t CorrectedCOG;//Gain Corrected COG
  Double_t CorrectedDt;//Time Offset Corrected Dt
  Double_t CorrectedAvgEnergy;//Gain Corrected AvgE
  

public:
  ClassDef(LendaBar,1);
};


#endif
