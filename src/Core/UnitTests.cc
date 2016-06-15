
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>

#include "TFile.h"
#include "TSystem.h"
#include "TObject.h"
#include "TTree.h"
#include "TStopwatch.h"

#include "S800Event.hh"
#include "DDASEvent.hh"
#include "LendaPacker.hh"
#include "R00TLeSettings.hh"

#include "S800Settings.hh"
#include "S800Calibration.hh"



#include "DAQdefs.h"

#include "Utilities.hh"

#include "TRandom3.h"


#include <assert.h>


#define MyAssert(X,Y) if (!(X)){cout<<Y<<endl;} assert(X);


int main(int argc,char** argv){
  
  if (argc!=3){
    cout<<"Usage EventNumber ChannelNumber "<<endl;
    return -1;
  }
  int eventNum =atoi(argv[1]);
  int channelNum=atoi(argv[2]);
  cout<<"EVENT NUMBER "<<eventNum<<endl;
  cout<<"CHANNEL NUMBER "<<channelNum<<endl;
  //  vector <UShort_t> theTrace={2985,2971,2971,2964,2968,2967,2961,2956,2958,2964,2960,2977,2954,2971,2982,3627,5811,7245,6988,5869,4718,4028,3676,3528,3412,3312,3227,3176,3138,3112,3084,3060,3075,3082,3084,3076,3057,3047,3025,3008,2993,2967,2947,2932,2946,2963,2966,2968,3003,3730,6030,7366,7008,5845,4705,4011,3682,3540,3430,3331,3365,4533,6622,7226,6511,5309,4397,3933,3713,3574,3441,3341,3281,3233,3190,3153,3118,3118,3133,3128,3105,3074,3064,3069,3063,3058,3041,3026,3002,2999,3027,3015,2774,2293,2023,2043,2238,2440,2585,2681,2745,2817,2851,2898,2896,2918,3208,5188,7437,7649,6635,5251,4297,3846,3651,3526,3397,3466,5061,7372,7806,6865,5453,4449};
  //  Double_t InternalCFD =0.5593261719;

  TFile f("/mnt/analysis/e10003/rootfiles/run-0573-00-RAW.root");
  TTree * rawtree= (TTree*)f.Get("rawtree");
  if (rawtree== NULL){
    cout<<"NO TREE"<<endl;
    return 0;
  }
  DDASEvent * eventIn= new DDASEvent();
  rawtree->SetBranchAddress("ddasevent",&eventIn);
  rawtree->GetEntry(eventNum);
  if (channelNum >= eventIn->GetData().size()){
    cout<<"Channel num too big.  Event only has "<<eventIn->GetData().size()<<endl;
    return -2;
  }
  vector <UShort_t> theTrace = eventIn->GetData()[channelNum]->GetTrace();
  


  Double_t InternalCFD = eventIn->GetData()[channelNum]->timecfd/16384.0;

  //The trace is coming from run-0537-00 event 53 andg is the first DDAS Channel.
  //it is fromt he Objecte Scintillator
  assert(theTrace.size() == 124);
  
  //Preform some test with the filtering algorithms 
  LendaFilter aFilter;
  Int_t FL=6;
  Int_t FG=0;
  Int_t d=6;
  Int_t w=0;
  vector <Double_t> CFD = aFilter.GetNewFirmwareCFD(theTrace,FL,FG,d,w);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////TEST for length of CFD.  Should be within the filter lengths of the tracae length
  assert(TMath::Abs(int(CFD.size())-int(theTrace.size())) < FL+d);
  




  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////Test simple trace ananlysis routines
  Int_t NumZeroCrossingsFromSimple=-1;
  Double_t CFDResidualFromSimple=-1;
  Double_t zeroCrossingFromSimple=aFilter.GetZeroCrossingImproved(CFD,NumZeroCrossingsFromSimple,CFDResidualFromSimple);
  
  Int_t MaxSpotFromSimple =-1;
  Int_t MaxValueFromSimple = aFilter.GetMaxPulseHeight(theTrace,MaxSpotFromSimple);
  Double_t EnergyFromSimple=aFilter.GetEnergy(theTrace,MaxSpotFromSimple);
  

  MyAssert(MaxValueFromSimple > 0 && MaxValueFromSimple < 16384,"Max Value out of ADC range");
  MyAssert(MaxSpotFromSimple != -1,"MaxPulseHeight Spot not found");
  MyAssert((zeroCrossingFromSimple > 0 &&zeroCrossingFromSimple<theTrace.size() )||(zeroCrossingFromSimple==-10008),"Simple CFD linear Zero finder failed "<<zeroCrossingFromSimple);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////Test for the HIGH rate algorithms
  vector <Int_t> PeakSpots;
  vector <double> UnderShoots;
  Double_t MaxValueFromHighRate=-1;
  Int_t MaxIndexFromHighRate=-1;
  vector <Double_t> theEnergies = aFilter.GetEnergyHighRate(theTrace,PeakSpots,UnderShoots,MaxValueFromHighRate,MaxIndexFromHighRate);
  vector<Double_t> theTimes = aFilter.GetZeroCrossingHighRate(CFD,PeakSpots);

  MyAssert(PeakSpots.size()!=0,"Found No peaks");
  MyAssert(theTimes.size() > 0, "Found No zero crossings in highrate algorithm");
  MyAssert(theEnergies.size() > 0,"Found no energies in highrate algorithm");
  MyAssert(MaxValueFromHighRate!=-1,"Could not find MaxValue from high rate algorithm");
  
  for ( auto i : PeakSpots){
    cout<<"Peak at "<<i<<endl;
  }


  bool SameZeroCrossing=false;
  for (auto i : theTimes ){
    if (i == zeroCrossingFromSimple){
      SameZeroCrossing=true;
    }
  }
  if (SameZeroCrossing==false){
    cout<<"HIGH RATE "<<endl;
    for (auto i : theTimes){
      cout<<i<<endl;
    }
    cout<<zeroCrossingFromSimple<<endl;
  }

  //  MyAssert(SameZeroCrossing,"Did not find the same zero crossing in both normal and high rate algorithms");
  
  MyAssert(MaxSpotFromSimple == MaxIndexFromHighRate,"Max indicies from high rate and simple algorithms don't match "<<MaxSpotFromSimple<<" "<<MaxIndexFromHighRate);
  MyAssert(MaxValueFromHighRate==MaxValueFromSimple,"Max values from high rate and simple algorithms don't match "<<MaxValueFromHighRate<<" "<<MaxValueFromSimple);
  
  bool MatchesInternalTime=false;
  for (auto i : theTimes){
    Double_t temp = i-TMath::Floor(i);
  
    if (TMath::Abs(temp-InternalCFD) < 0.0001){
      MatchesInternalTime=true;
    }
  }
  //MyAssert(MatchesInternalTime,"There is no software time that mathes the internal CFD value. Is filter 6 0 6 0? InternalCFD is "<<InternalCFD);


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////Test for LENDAPACKER
  R00TLeSettings aSettings;
  LendaPacker aPacker(&aSettings);
  aPacker.FindAndSetMapAndCorrectionsFileNames(1);
  ddaschannel * aChan = new ddaschannel();
  
  aChan->trace =theTrace;
  aChan->slotid=2;
  aChan->chanid=13;
  aChan->time=1000004;
  
  DDASEvent event;
  event.AddChannelData(aChan);
  LendaEvent le;
  aPacker.MakeLendaEvent(&le,&event,0);
  le.Finalize();
  MyAssert(le.N ==event.GetNEvents(),"Number of lendaChannels not the same as number of DDASChannels "<<le.N<<" "<<event.GetNEvents());
  
  // MyAssert(le.NumBars !=0,"No bars check map file that is being loaded");
  
  // MyAssert(le.Bars[0].NumTops !=0,"No Top. Test assumes map file puts slot2 chan0 as a top");

  LendaChannel theChannel = le.TheObjectScintillators[0];
  
  MyAssert(theChannel.GetSoftwareTimes().size() == theTimes.size(),"Number of Times found in packer not the same as manually "<<theChannel.GetSoftwareTimes().size()<<" "<<theTimes.size());
  
  MyAssert(abs(int(theChannel.GetSoftwareTimes().size())-int(theChannel.GetEnergies().size()))<=1,
	   "high Alogorithms returned different number of eneries and times "<<theChannel.GetEnergies().size()<<" "<<theChannel.GetSoftwareTimes().size());
  MyAssert(theChannel.GetSoftwareTimes().size()==theChannel.GetCubicTimes().size(),"high Alogorithms returned different number of cubicTimes and times");

  

  

  cout<<"\n****************************\nAll Tests Pass\n****************************"<<endl;
  return 0;
}
