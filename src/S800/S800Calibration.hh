#ifndef __S800CALIBRATION_HH
#define __S800CALIBRATION_HH

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <map>

#include "TSystem.h"
#include "TEnv.h"
#include "TMath.h"

#include "S800Settings.hh"
#include "S800Calc.hh"

class GTimeOfFlight;

class S800Calibration {
public:
   S800Calibration();
   S800Calibration(S800Settings*);
   ~S800Calibration();

   void ReadCrdcCalibration(const char* filename, const char* pedestalfile);
   void ReadCrdcBadPads(const char* filename);
   //bool IsCrdcBadPad(int ch);
   bool IsBad(int ch){
     for(UShort_t b=0;b<fbad[fcrdc.GetID()].size();b++){
       if(ch==fbad[fcrdc.GetID()][b])
         return true;
     }
     return false;
   };
 
   void S800Calculate(S800* in, S800Calc* out);

   void CrdcCal(std::vector<Short_t> channel, std::vector<Short_t> data, Int_t id);
   Float_t CalcX();

   void SetCrdc(std::vector<Short_t> channel, std::vector<Short_t> data, Float_t tac, Float_t anode, Int_t id);
   CRDC GetCrdc(){return fcrdc;}

   Float_t TimeOffset(Float_t time1, Float_t time2);
   Float_t TimeOffset(Float_t time);
   void SetTof(GTimeOfFlight *tof);
   TOF  GetTof(){return ftof;}

   void ReadICCalibration(const char *filename);
   std::vector<Float_t> ICCal(std::vector<int> chan, std::vector<float> raw);
   Float_t ICSum(std::vector<Float_t> cal);
   Float_t ICDE(Float_t sum, Float_t x, Float_t y);


   void SetTS800(Short_t ts800){fts800 = ts800;}

  std::vector<Float_t> GetCRDCCal(){return fcrdccal;}
private:
   S800Settings* fSett;
   std::vector<std::vector<Float_t> > fped;  
   std::vector<std::vector<Float_t> > fslope;
   std::vector<std::vector<Float_t> > foffset;
   std::vector<std::vector<Int_t> > fbad;
   std::vector<Float_t> fcrdccal;
   Short_t fts800;  

   std::vector<Float_t> fICoffset;
   std::vector<Float_t> fICslope;
   Float_t fde_slope;
   Float_t fde_offset;

   CRDC fcrdc;
   TOF ftof;
};

#endif
