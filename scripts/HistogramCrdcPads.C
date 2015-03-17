#include "TCanvas.h"



#include "S800.hh"
#include "S800Event.hh"
#include "S800Calibration.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TDirectory.h"
#include "TTree.h"
#include <iostream>
#include <sstream>
using namespace std;
void HistogramCrdcPads(int num=0){

  TTree * rawtree = (TTree*)gDirectory->Get("rawtree");

  if (rawtree == NULL ){
    cout<<"Can't find rawtree"<<endl;
    return;
  }
  
  //This script will look at the Raw root trees
  //and plot a single crdc pad distribution 

  TH1F * PADS[224];
  stringstream name;

  for (int i=0;i<224;i++){
    name.str("");
    name<<"PAD"<<i;
    PADS[i] = new TH1F(name.str().c_str(),name.str().c_str(),300,0,5000);

  }

  TH2F * PADGain = new TH2F("PADGain","PADGain",220,0,220,1000,0,8000);

  TH2F* IC_hist = new TH2F("IC_hist","IC_hist",1000,0,10000,1000,0,10000);

  S800Calibration calibration(new S800Settings());
  calibration.ReadCrdcCalibration("/user/e10003/R00TLe/prm/crdccal_temp.dat","/user/e10003/R00TLe/prm/crdcpedestals_real.dat");
  S800Event * event= new S800Event();
  rawtree->SetBranchAddress("s800event",&event);
  int count =rawtree->GetEntries();
  for (int i=0;i<count;i++){
    rawtree->GetEntry(i);

    int s=event->GetS800()->GetIonChamber()->GetData().size();
    double ICSum=0;
    for (int i=0;i<s;i++){
      ICSum+=event->GetS800()->GetIonChamber()->GetData()[i];
    }

    double TOF= event->GetS800()->GetTimeOfFlight()->GetTACOBJ();
    IC_hist->Fill(event->GetS800()->GetTimeOfFlight()->GetTACOBJ(),ICSum);

    int size = event->GetS800()->GetCrdc(0)->GetChannels().size();
    
    
    calibration.CrdcCal(event->GetS800()->GetCrdc(0)->GetChannels(),event->GetS800()->GetCrdc(0)->GetData(),0);

    if (ICSum>6800 && ICSum<8300 && TOF>1625 &&TOF<1680){

      int size2=calibration.GetCRDCCal().size();
      double max =-1;
      int maxPad =-1;
      
      double maxM1=-1;
      double maxM2=-1;

      double maxP1=-1;
      double maxP2=-1;

      
      for (int i=3;i<size2-3;i++){
	double val = calibration.GetCRDCCal()[i];
	if (val != 0 && val <8000){
	  //	  PADS[i]->Fill(val);
	  if (val > max ){
	    max=val;
	    maxPad=i;
	    maxM1=calibration.GetCRDCCal()[i-1];
	    maxM2=calibration.GetCRDCCal()[i-2];

	    maxP1=calibration.GetCRDCCal()[i+1];
	    maxP2=calibration.GetCRDCCal()[i+2];

	  }
	}
      }
      if (maxPad != -1){
	PADGain->Fill(maxPad,max);
	//	PADGain->Fill(maxPad-1,maxM1);
	//PADGain->Fill(maxPad-2,maxM2);
	//PADGain->Fill(maxPad+1,maxP1);
	// PADGain->Fill(maxPad+2,maxP2);


	PADS[maxPad]->Fill(max);
      }else{
	cout<<"UH?"<<endl;
      }

    }
  }


  TCanvas * c =new TCanvas("test");
  c->Divide(3,3);

  int start=60;
  for (int i=start;i<start+9;i++){
    c->cd(i+1-start);
    PADS[i]->Draw("");
  }
  
  return;
}
