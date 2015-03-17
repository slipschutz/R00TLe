#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

#include "S800Calibration.hh"
#include "S800.hh"
#include "S800defs.h"

#include "lmcurve.h"
#include "lmmin.h"
#include "lmfit.h"

S800Calibration::S800Calibration(){
}

S800Calibration::S800Calibration(S800Settings* setting){
   fSett = setting;

   // S800
   fped.resize(2);
   fslope.resize(2);
   foffset.resize(2);
   fbad.resize(2);
   for(int i=0;i<2;i++){
      fped[i].resize(S800_FP_CRDC_CHANNELS);
      fslope[i].resize(S800_FP_CRDC_CHANNELS);
      foffset[i].resize(S800_FP_CRDC_CHANNELS);
   }
   fcrdccal.resize(S800_FP_CRDC_CHANNELS);
   ReadCrdcCalibration(fSett->CalFile(),fSett->PedestalFile());
   ReadCrdcBadPads(fSett->BadFile());

   fICoffset.resize(S800_FP_IC_CHANNELS);
   fICslope.resize(S800_FP_IC_CHANNELS);
   ReadICCalibration(fSett->CalFileIC());
}

S800Calibration::~S800Calibration(){
   std::cout << "destructor" << std::endl;
}

// S800 
// CRDC cathode pedestal/gain
void S800Calibration::ReadCrdcCalibration(const char *filename, const char *pedfile){
   TEnv *crdcpedestals = new TEnv(pedfile); 
   TEnv *crdccal = new TEnv(filename); 
   for (int c=0; c<2; c++) {
      for(int p=0; p<S800_FP_CRDC_CHANNELS; p++) {
	 fped[c][p] = crdcpedestals->GetValue(Form("Crdc.%d.Ped.%03d",c,p),0.0);
	 fslope[c][p] = crdccal->GetValue(Form("Crdc.%d.Slope.%03d",c,p),1.0);
	 foffset[c][p] = crdccal->GetValue(Form("Crdc.%d.Offset.%03d",c,p),0.0);
      }
   }

   // for (int p=0;p<S800_FP_CRDC_CHANNELS;p++){
   //   std::cout<<fped[0][p]<<"   "<<fslope[0][p]<<"  "<<foffset[0][p]<<std::endl;
   // }
}

// CRDC cathode bad pad
void S800Calibration::ReadCrdcBadPads(const char *filename){
   TEnv *bad = new TEnv(filename); 
   for(UShort_t i=0;i<2;i++){
      fbad[i].resize(bad->GetValue(Form("Crdc.%d.Nofbadpads",i),0));
      for(UShort_t k=0;k<fbad[i].size();k++){
	 fbad[i][k] = bad->GetValue(Form("Crdc.%d.badpad.%d",i,k),0);
	 // std::cout << i << " " << k << " " << fbad[i][k] << std::endl;
      }
   }
}

void S800Calibration::CrdcCal(std::vector<Short_t> channel, std::vector<Short_t> data, Int_t id){
   Short_t index;
   std::vector<Float_t> sum;
   std::vector<Short_t> samples;
   sum.clear();
   sum.resize(S800_FP_CRDC_CHANNELS);
   samples.clear();
   samples.resize(S800_FP_CRDC_CHANNELS);
   fcrdccal.clear();
   fcrdccal.resize(S800_FP_CRDC_CHANNELS);
   if(channel.size() != data.size()){
      std::cerr << " channel ("<<channel.size()<<") and data ("<<data.size()<<") have different sizes " << std::endl;
      return; 
   }

   for(UShort_t f=0; f < channel.size(); f++){
      index = channel[f];
      sum[index] += data[f] - fped[id][index];
      samples[index]++;
   }
   for(UShort_t ch=0; ch < S800_FP_CRDC_CHANNELS; ch++){
      if (samples[ch] > 0) {
	 fcrdccal[ch] = sum[ch]; // / fSett->SampleWidth();
	 fcrdccal[ch] *= fslope[id][ch];
	 fcrdccal[ch] += foffset[id][ch];
      } else {
	fcrdccal[ch]=0;//sqrt(-1.0);
      }
   }
   return ;
}

void S800Calibration::SetCrdc(std::vector<Short_t> channel, std::vector<Short_t> data, 
			      Float_t tac, Float_t anode, Int_t id) {
   fcrdc.Clear();
   fcrdc.SetID(id);
   this->CrdcCal(channel,data,id);
   fcrdc.SetCal(fcrdccal);
  
   // debug
   // std::cout << "\n" << id << std::endl;
   // for (int i = 0; i < channel.size(); i++) {
   //    std::cout << channel[i] << " " << data[i] << std::endl;
   // }
   double x = fSett->XOffset(id) + fSett->XSlope(id) * this->CalcX();
   double y = fSett->YOffset(id) + fSett->YSlope(id) * tac;
   fcrdc.SetX(x);
   fcrdc.SetY(y);
   fcrdc.SetTAC(tac);
   fcrdc.SetAnode(anode);
}

Float_t S800Calibration::CalcX(){
   // Cluster search
   Bool_t flg_clstr = kFALSE;
   Int_t  iclstr = -1;
   const Int_t maxclstr = S800_FP_CRDC_CHANNELS;
                             // maximum number of clusters (I know this is too many...)
   Int_t clstr[maxclstr][3];
   Float_t maxchg[maxclstr];
   Float_t maxpad[maxclstr];
   const Float_t qmax = 25.; // MINUMUM value of max charge
                             // to be considered to form a cluster
   const Float_t qthr =  8.; // MINUMUM value of charge to be considered to be hit
   Float_t tmp_qmax = 0.0;
   Int_t gclstr = 0;
  
   for (UShort_t i = 0; i < S800_FP_CRDC_CHANNELS; i++) {
      if (IsBad(i)) continue;
      if ((flg_clstr == kFALSE) && (!std::isnan(fcrdccal[i]))) {
	 flg_clstr = kTRUE;
	 iclstr = iclstr + 1;
	 clstr[iclstr][0] =  i; // leading edge
	 clstr[iclstr][1] = -1; // trailing edge (tentative)
	 maxchg[iclstr] = fcrdccal[i];
      } else if ((flg_clstr == kTRUE) && (!std::isnan(fcrdccal[i]))) {
	 if (fcrdccal[i] > maxchg[iclstr]) {
	    maxchg[iclstr] = fcrdccal[i];
	    maxpad[iclstr] = i;
	 }
      } else if ((flg_clstr == kTRUE) && (std::isnan(fcrdccal[i]))) {
	 flg_clstr = kFALSE;
	 clstr[iclstr][1] = i - 1;
	 clstr[iclstr][2] = i - clstr[iclstr][0];
	 //if (maxpad[iclstr] < qmax) {
         if (maxchg[iclstr] < qmax) { // As pointed out by Sasano-san 2015/3/9
	    iclstr = iclstr - 1;
	 }
      }
   }

   // if a cluster is located at the end of the cathode
   if (flg_clstr == kTRUE) {
      clstr[iclstr][1] = S800_FP_CRDC_CHANNELS - 1;
      clstr[iclstr][2] = S800_FP_CRDC_CHANNELS - clstr[iclstr][0];
   }

   if (iclstr == 0) {
      gclstr = 0;
   } else if (iclstr > 0) {
      tmp_qmax = maxchg[0];
      // look for the GOOD cluster (gclstr) to be used to the analysis 
      // (the cluster with the max charge is used)
      for (Int_t i = 0; i < iclstr + 1; i++) {
	 if (maxchg[i] > tmp_qmax) {
	    tmp_qmax = maxchg[i];
	    gclstr = i;
	 }
      }
   } else {
      return sqrt(-1.0);
   }
   fcrdc.SetMaxChg((Float_t)maxchg[gclstr]);
   fcrdc.SetMaxPad((Float_t)maxpad[gclstr]);

   Int_t j = 0;
   Double_t xpad[S800_FP_CRDC_CHANNELS], qcal[S800_FP_CRDC_CHANNELS]; // (I know this is too many...)
   Float_t sum_q = 0.0, sum_qx = 0.0, sum_qxx = 0.0;

   for (UShort_t i = clstr[gclstr][0]; i <= clstr[gclstr][1]; i++) {
      if (IsBad(i)) continue;
      if (fcrdccal[i] < qthr) continue;
      sum_q   += fcrdccal[i];
      sum_qx  += fcrdccal[i] * i;
      sum_qxx += fcrdccal[i] * i * i;
      // for fit
      xpad[j] = (Double_t)i;
      qcal[j] = (Double_t)fcrdccal[i];
      // debug
      // std::cout << j << " " << xpad[j] << " " << qcal[j] << std::endl;
      j++;
   }

   fcrdc.SetXpad(xpad,j);
   fcrdc.SetYpad(qcal,j);

   Double_t xcog  = (Double_t)sum_qx/sum_q;
   Double_t sigma = (Double_t)TMath::Sqrt(sum_qxx/sum_q - (sum_qx/sum_q)*(sum_qx/sum_q));
   if(xcog < 0 || xcog > S800_FP_CRDC_CHANNELS){ // no gravity center found
      xcog = sqrt(-1.0);
      std::cout << "Something strange happens with the CRDC data." << std::endl;
   }
   fcrdc.SetXcog((Float_t)xcog);
   fcrdc.SetCathode((Float_t)sum_q);

   if (fSett->XFit()) {
      // Do fit
      Double_t xfit;
      Double_t par[3];
      Int_t    n_par = 3; // number of parameters in model function f
   
      lm_status_struct status;
      lm_control_struct control = lm_control_double;
      control.printflags = 0; // monitor status (+1) and parameters (+2)
   
      // Initial guess
      par[0] = (Double_t)maxchg[gclstr];
      par[1] = xcog;
      par[2] = sigma;
   
      if (fSett->XFitFunc() == 1) {
	 // Secant Hyperbolic Squared
	 lmcurve_fit( n_par, par, j, xpad, qcal, sechs, &control, &status );
      } else if (fSett->XFitFunc() == 2) {
	 // gaussian
	 // lmcurve_fit( n_par, par, j, xpad, qcal, gauss, &control, &status );
      }
   
      xfit = par[1];
      fcrdc.SetXfit((Float_t)xfit);
      fcrdc.SetFitPrm(0,(Float_t)par[0]);
      fcrdc.SetFitPrm(1,(Float_t)par[1]);
      fcrdc.SetFitPrm(2,(Float_t)par[2]);
      fcrdc.SetFnorm(status.fnorm);
      return (Float_t)xfit;
   }

   // If we do not do fit, return xcog
   return (Float_t)xcog;
}

Float_t S800Calibration::TimeOffset(Float_t time1, Float_t time2){
   return time1 - time2;
}

Float_t S800Calibration::TimeOffset(Float_t time){
   return time - fts800;
}

void S800Calibration::SetTof(GTimeOfFlight *tof){
   ftof.Set(TimeOffset(tof->GetRF()), TimeOffset(tof->GetOBJ()), TimeOffset(tof->GetXFP()));
   ftof.SetTAC(tof->GetTACOBJ(), tof->GetTACXFP());
}

void S800Calibration::ReadICCalibration(const char *filename){
   TEnv *iccal = new TEnv(filename);
   for(int i=0;i<S800_FP_IC_CHANNELS;i++){
      fICoffset[i] = iccal->GetValue(Form("IonChamber.Offset.%d",i),0.0);
      fICslope[i] = iccal->GetValue(Form("IonChamber.Slope.%d",i),1.0);
   }
   fde_slope = iccal->GetValue("IonChamber.Slope.DE",1.0);
   fde_offset = iccal->GetValue("IonChamber.Offset.DE",0.0);
} 

std::vector<Float_t> S800Calibration::ICCal(std::vector<int> chan, std::vector<float> raw){
   std::vector<Float_t> cal;
   cal.resize(S800_FP_IC_CHANNELS);
   if(chan.size() != raw.size()){
      std::cerr << " channel ("<<chan.size()<<") and data ("<<raw.size()<<" have different sizes " << std::endl;
      return cal; 
   }
   for(unsigned int f=0;f<chan.size();f++){
      if( (chan[f]>-1) && (chan[f]<S800_FP_IC_CHANNELS) ){
	 cal[chan[f]] = raw[f]*fICslope[chan[f]]+fICoffset[chan[f]];
      }
      else{
	 std::cerr << " channel "<<chan[f]<<" not found!" << std::endl;
      }
   }
   return cal; 
}
Float_t S800Calibration::ICSum(std::vector<Float_t> cal){
   Short_t ch = 0;
   Float_t sum =0;
   for(UShort_t j=0; j<cal.size(); j++){
      if(cal[j]>0){
	 sum += cal[j];
	 ch++;
      }
   }
   if(ch > 0)
      sum/= ch;
   else 
      sum = sqrt(-1.0);
  
   return sum;

}
Float_t S800Calibration::ICDE(Float_t sum, Float_t x, Float_t y){
   // something needs to be done...
   // if(!isnan(sum) && !isnan(ftrack.GetAFP())){
   //   if(!isnan(y))
   //     sum += sum*fSett->dE_ytilt()*y;
   //   if(!isnan(x) && x < fSett->dE_x0tilt())
   //     sum *= exp(fSett->dE_xtilt()* (fSett->dE_x0tilt() -x) );
   //   fs800valid = 0;
   //   return sum * fde_slope + fde_offset;
   // }
   // else return sqrt(-1.0);
   return sum;
}

void S800Calibration::S800Calculate(S800* in, S800Calc* out){
   //s800
   CRDC crdc[2];
   TOF tof;
   SCINT scint[3];
   HODOSCOPE hodoscope[32];
   IC ich;

   ich.Clear();
   tof.Clear();
   bool icgood = false;

   out->Clear();
   out->SetTimeS800(in->GetTrigger()->GetS800());
   SetTS800(in->GetTrigger()->GetS800());

   // timestamp
   out->SetTS(in->GetTS());

   // TOF
   tof.Clear();
   this->SetTof(in->GetTimeOfFlight());
   tof = this->GetTof();

   // CRDC
   for(UShort_t k=0; k<2; k++) {
      crdc[k].Clear();
      this->SetCrdc(in->GetCrdc(k)->GetChannels(),
		    in->GetCrdc(k)->GetData(),
		    in->GetCrdc(k)->GetTAC(), 
		    in->GetCrdc(k)->GetAnode(), 
		    k);
      crdc[k] = this->GetCrdc();
   }

   //SCINTILLATOR
   for(UShort_t s=0; s<3; s++){
      scint[s].Clear();
      scint[s].SetTime(TimeOffset(in->GetScintillator(s)->GetTime_up()),
		       TimeOffset(in->GetScintillator(s)->GetTime_down()));
      scint[s].SetDE(in->GetScintillator(s)->GetDE_up(), in->GetScintillator(s)->GetDE_down());
   }

   //HODOSCOPE
   for (UShort_t s = 0; s < 32; s++) {
      hodoscope[s].Clear();
      hodoscope[s].SetEnergy(in->GetHodoscope(s)->GetEnergy());
   }

   //IC
   ich.SetCal(ICCal(in->GetIonChamber()->GetChannels(),in->GetIonChamber()->GetData() ) );
   ich.SetSum(ICSum(ich.GetCal()));
   if(ich.GetSum() > 400){
      icgood = true;
   }
   //ich.SetDE(ICDE(ich.GetSum(), crdc[0].GetX(), crdc[0].GetY()));

   //set Calculated S800
   out->SetTOF(tof);
   for (UShort_t s = 0; s < 2;  s++) {out->SetCRDC(crdc[s],s);}
   for (UShort_t s = 0; s < 3;  s++) {out->SetSCINT(scint[s],s);}
   for (UShort_t s = 0; s < 32; s++) {out->SetHODOSCOPE(hodoscope[s],s);}
   out->SetIC(ich);
}
