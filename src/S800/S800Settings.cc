#include "S800Settings.hh"

S800Settings::S800Settings(){
}

S800Settings::S800Settings(const char* filename){
  SetFile( filename );
  ReadSettings();
}

S800Settings::~S800Settings(){
  std::cout << "destructor" << std::endl;
}

void S800Settings::ReadSettings(){
  char* defaultfile = (char*)"~/analysis/settings/nocal.dat";

  TEnv *set = new TEnv(fInputFile.data()); 
  fCalFile = set->GetValue("Crdc.File",defaultfile);
  fPedestalFile = set->GetValue("Crdc.Ped.File",defaultfile);
  for(int i=0;i<2;i++){
    fXFit       = set->GetValue("Crdc.X.Fit",0);
    fXFitFunc   = set->GetValue("Crdc.X.FitFunc",1);
    fxOffset[i] = set->GetValue(Form("Crdc.X.Offset.%d",i),0.0);
    fxSlope[i]  = set->GetValue(Form("Crdc.X.Slope.%d",i),1.0);
    fyOffset[i] = set->GetValue(Form("Crdc.Y.Offset.%d",i),0.0);
    fySlope[i]  = set->GetValue(Form("Crdc.Y.Slope.%d",i),1.0);
  }
  fBadFile = set->GetValue("BadPad.File",defaultfile);

  //fMatrixFile = set->GetValue("Gretina.Matrix.File",defaultfile);
  //for (int icore = 0; icore < 4; icore++) {
  //  fCoreCalibFile[icore] = set->GetValue(Form("Gretina.Core.%d.Calibration.File",icore),defaultfile);
  //}

  //fdet2hole.clear();
  //fhole2det.clear();
  //for(int det=0; det<7; det++){
  //  int hole = set->GetValue(Form("Detector.%d",det),-1);
  //  if (hole!=-1){
  //    fdet2hole[det] = hole;
  //    fhole2det[hole] = det;
  //  }
  //}

  //fcry2spec.clear();
  //fspec2cry.clear();
  //for(int spec=0; spec<28; spec++){
  //  int cry = set->GetValue(Form("Spectcl.%02d",spec),-1);
  //  fcry2spec[cry] = spec;
  //  fspec2cry[spec] = cry;
  //}

}

//int Settings::Det2Hole(int det){
//  if (fdet2hole.count(det)==1){
//    return fdet2hole[det];
//  } else {
//    return -1;
//  }
//}

//int Settings::Hole2Det(int hole){
//  if (fhole2det.count(hole)==1){
//    return fhole2det[hole];
//  } else {
//    return -1;
//  }
//}

//int Settings::Cry2Spec(int cry){
//  if (fcry2spec.count(cry)==1){
//    return fcry2spec[cry];
//  } else {
//    return -1;
//  }
//}

//int Settings::Spec2Cry(int spec){
//  if (fspec2cry.count(spec)==1){
//    return fspec2cry[spec];
//  } else {
//    return -1;
//  }
//}

// void Settings::PrintSettings(){
//   std::cout << "Crdc.File\t"     << fCalFile      << std::endl;
//   std::cout << "Crdc.Ped.File\t" << fPedestalFile << std::endl;
//   std::cout << "Crdc.X.Fit\t"    << fXFit         << std::endl;
//   for(int i=0;i<2;i++){
//     std::cout << Form("Crdc.X.Offset.%d\t",i) << fxOffset[i] << std::endl;
//     std::cout << Form("Crdc.X.Slope.%d\t",i)  << fxSlope[i]  << std::endl;
//     std::cout << Form("Crdc.Y.Offset.%d\t",i) << fyOffset[i] << std::endl;
//     std::cout << Form("Crdc.Y.Slope.%d\t",i)  << fySlope[i]  << std::endl;
//   }
//   std::cout << "BadPad.File\t"         << fBadFile    << std::endl;
//   std::cout << "Gretina.Matrix.File\t" << fMatrixFile << std::endl;
// }
