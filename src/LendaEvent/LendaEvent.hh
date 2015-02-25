#ifndef __LENDA_EVENT_HH
#define __LENDA_EVENT_HH
 
#include "TObject.h" //Include ROOT
#include <vector>
#include <map>

#include "LendaBar.hh"
using namespace std;

/**
Main container class for DDAS inforamation in calibrated trees.
A ROOT Dictionary is made from the class inorder to have it 
available in the interpreter.
*/
class LendaEvent :  public TObject  {
public:

  LendaEvent(); ///<Defualt Constructor 

  void Clear(); ///<Clear the Event. Should be called after every write to a tree
  void Clear(Option_t*){Clear();} ///<To suppress warnings about hidding clear in TObject

  /**Finalize should be called before writing to a tree.
     Finalize calculates convient quantities that will appear as 
     branches and leaves in the tree.  Calls the finalize methods 
     the Bars and ObjectScinitillators.
   */
  void Finalize(); 



  void PushABar(LendaBar aBar){Bars.push_back(aBar);NumBars++;}///<Add a LendaBar to the LendaEvent
  void PushUnMappedChannel(LendaChannel c){UnMappedChannels.push_back(c);NumUnMappedChannels++;}///<Add an unmapped channel to the event

  vector <LendaBar> Bars; ///<The vector of LendaBars that contains main real event information
  vector <LendaChannel> UnMappedChannels;///<vector of LendaChannels to hold any channels that are not mapped in the map file
  vector <LendaChannel> TheObjectScintillators;///<Vector contain the lendachannels for the object scintillators

  Int_t NumOfChannelsInEvent; ///<The Number of channels associated with this event  
  Int_t N;///<Same thing as NumOfChannelsInEvent
  Int_t NumBars;///<Number of bars in event.  Due to pileup and the object Scintillators  could be different from N/2

  Int_t NumUnMappedChannels;///<Number of DDAS channels in the event that were not in the map file
  
  Int_t NumObjectScintillators;///<Number of ObjectScintillators


  Bool_t operator==(const LendaEvent & RHS);

  LendaBar * FindBar(int BarId);

  LendaBar * FindBar(string Name);

  LendaBar FindBarSafe(string Name);

  LendaBar FindBarSafe(int BarId);

private:


  
public:
  ClassDef(LendaEvent, 22);///<ROOT Macro for dictionary building
};
#endif



