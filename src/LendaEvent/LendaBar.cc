


/////////////////////////////////////////////////////////
// This is the source file for the LendaBar ROOT class //
/////////////////////////////////////////////////////////

#include "LendaBar.hh"

LendaBar::LendaBar(){
  Tops.clear();
  Bottoms.clear();
}
LendaBar::~LendaBar(){
  Tops.clear();
  Bottoms.clear();
  NumTops=0;
  NumBottoms=0;
  BarMultiplicity=0;

}

void LendaBar::Finalize(){
  NumTops=Tops.size();
  NumBottoms=Bottoms.size();
  
  BarMultiplicity=NumTops+NumBottoms;


}
