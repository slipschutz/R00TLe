

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

/**Holds information from individual LENDA bars. Stored within LendaEvents
   Contains LendaChannel objects.  Each bar has a name and unique bar ID
   that are set by the LendaPacker
   
 */
class LendaBar : public TObject{
public:
  LendaBar();///<Defualt constructor
  LendaBar(string s,Double_t a) {Clear(); Name=s;BarAngle=a;} ///<Constrcutor specifying name and angle of bar
  ~LendaBar();///< De-constructor
  
  //Storing a vector of Tops and bottoms
  //to allow for 0 or more than one of each
  //channel to be stored
  vector <LendaChannel> Tops;///<vector holding the top channels in this bar 
  vector <LendaChannel> Bottoms;///<vector holding hte bottom channels in this bar
  
  void PushTopChannel(LendaChannel c){Tops.push_back(c);}///<Adds a channel to the list of top channels
  void PushBottomChannel(LendaChannel c){Bottoms.push_back(c);}///<Adds a channel to the list of bottom channels

  inline string GetBarName(){return Name;} ///<Gets Bar name
  inline Int_t GetNumTops(){return NumTops;}///<Gets number of top channels in this bar
  inline Int_t GetNumBottoms(){return NumBottoms;}///<Gets the number of bottom Channels in this bar
  inline Int_t GetBarMultiplicity(){return BarMultiplicity;}///<Gets the total number times a channel fired in this bar in an event

  inline Double_t GetCubicDt(){return CubicDt;}///<Top-bottom times from cubic algorithm.  Only set for "SimpleEvents"
  inline Double_t GetDt(){return Dt;}///<Top-bottom times from internal linear algorithm.  Only set for "SimpleEvents"

  inline Double_t GetSyncDt(){return SyncDt;}
  inline Double_t GetCubicSyncDt(){return CubicSyncDt;}

  inline Double_t GetCOG(){return COG;}///<ETop-EBottom/(ETop+EBottom).  Only Set for "SimpleEvents"
  inline Double_t GetAvgEnergy(){return AvgEnergy;}///<Geometric average of ETop and EBottom. Only Set for "SimpleEvents" and ETop>0 EBottom>0
  inline Double_t GetAvgPulseHeight(){return AvgPulseHeight;}///<Geometric average of top and bottom pulseheights. Only Set for "SimpleEvents" 

  inline Double_t GetCorrectedDt(){return CorrectedDt;}///<Top-bottom from linear algorithm with timing shift corrections applied. Only set for "SimpleEvents"
  inline Double_t GetCorrectedCOG(){return CorrectedCOG;}///<Same as GetCOG except calculated with corrected energies. Only set for "SimpleEvents"
  inline Double_t GetCorrectedAvgEnergy(){return CorrectedAvgEnergy;}///<Same as GetAvgEnergy() except with corrected energies. Only set for "SimpleEvents"

  inline Double_t GetAvgT(){return AvgT;}///<1/2*(TopTime+BottomTime) calculated from internal timing. Only set for "SimpleEvents"
  inline Double_t GetCorrectedAvgT(){return AvgT;}///<Same as GetAvgT() except calcualted with timing shift corrections
  
  inline Double_t GetTopTOF(){return TopTOF;}///<Top channel internal time - internal time of the reference channel.
  inline Double_t GetBottomTOF(){return BottomTOF;}///<Bottom channel internal time - internal time of the reference channel.

  inline Double_t GetAvgTOF(){return 0.5*(TopTOF+BottomTOF);}///<Average of TopTOF and BottomTOF
  inline Double_t GetCorrectedAvgTOF(){return 0.5*(CorrectedTopTOF + CorrectedBottomTOF);}///<Same as GetAvgTOF except timing shift corrections are applied

  inline Double_t GetCubicTopTOF(){return CubicTopTOF;}///<Same was GetTopTOF() except calculated with cubic algorithm
  inline Double_t GetCubicBottomTOF(){return CubicBottomTOF;}///<Same as GetBottomTOF() except calculated with cubic algorithm

  inline Double_t GetCorrectedTopTOF(){return CorrectedTopTOF;}///<Gets the TopTOF from internal timing with timing shift corrections applied
  inline Double_t GetCorrectedBottomTOF(){return CorrectedBottomTOF;}///<Gets the BottomTOF from internal timing with timing shift corrections applied

  inline Double_t GetCorrectedCubicTopTOF(){return CorrectedCubicTopTOF;}///<Same as GetCorrectedTopTOF() but with cubic algorithm
  inline Double_t GetCorrectedCubicBottomTOF(){return CorrectedCubicBottomTOF;}///<Same as GetCorrectedBottomTOF() but with cubic algorithm



  inline Double_t GetCorrectedSoftTopTOF(){return CorrectedSoftTopTOF;}///<Same as GetCorrectedTopTOF() except uses offline linear algorithm
  inline Double_t GetCorrectedSoftBottomTOF(){return CorrectedSoftBottomTOF;}///<Same as GetCorrectedBottomTOF() except uses offline linear algorithm

  inline Double_t GetCorrectedAvgSoftTOF(){return 0.5*(CorrectedSoftTopTOF+CorrectedSoftBottomTOF);}///<Average of CorrectedSoftTopTOF and CorrectedSoftBottomTOF

  void SetBarId(Int_t v){BarId=v;}///<Sets the bar ID for the bar
  void SetBarName(string v){Name=v;}///<Sets the bar name

  void Clear();///<Clears the LendaBar.  Should be called after every write to a tree
  void Clear(Option_t*){Clear();}///<To suppress warnings about hiding clear in TObject

  string Name;///<the name of the bar to make it unique
  Int_t BarId; ///<The Unique BarID Defined by the order in MapFile
  
  Double_t BarAngle;///<The bar angle

  Int_t NumTops;///< The number of top channels in this bar
  Int_t NumBottoms;///< The number of bottom channels in this bar
  Int_t BarMultiplicity;///<The total number times a channel fired in this bar in an event. 
  
  Bool_t SimpleEventBit; ///<true if bar has only 1 Top and 1 bottom

  void Finalize();///<Calcuates convient quantities 
  
  Bool_t operator==(const LendaBar & RHS);

private:
  Double_t Dt;//Top Time - Bottom Time
  Double_t CubicDt;

  //The Top Bottom time diffrence through the reference signal
  Double_t SyncDt;
  Double_t CubicSyncDt;

  Double_t COG;//ET -EB/(ET+EB)
  Double_t AvgEnergy;//Sqrt(ET*EB)
  Double_t AvgPulseHeight;//Sqrt(PH_T*PH_B)


  Double_t AvgT;
  Double_t CorrectedAvgT;

  Double_t CorrectedCOG;//Gain Corrected COG
  Double_t CorrectedDt;//Time Offset Corrected Dt
  Double_t CorrectedAvgEnergy;//Gain Corrected AvgE

  Double_t TopTOF; //Time of flight calculated from the TOP channel
  Double_t BottomTOF; //Time of flight calculated from the Bottom channel

  Double_t CubicTopTOF;
  Double_t CubicBottomTOF;
  
  Double_t CorrectedCubicTopTOF;
  Double_t CorrectedCubicBottomTOF;

  Double_t CorrectedCubicFitTopTOF;
  Double_t CorrectedCubicFitBottomTOF;
  
  Double_t CorrectedSoftTopTOF;
  Double_t CorrectedSoftBottomTOF;

  Double_t CorrectedTopTOF;
  Double_t CorrectedBottomTOF;

public:
  ClassDef(LendaBar,2);///<ROOT Marco for dictionary generation
};


#endif
