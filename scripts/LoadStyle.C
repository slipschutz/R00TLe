


/////////////////////////////////////////////////////////////
// hopefully this will be a script to make setup a gStyle  //
// that makes pretty graphs				   //
/////////////////////////////////////////////////////////////





void LoadStyle(){

  gStyle->SetOptStat(0);//Get rid of histogram stats box

  gStyle->SetLabelSize(0.045,"X");
  gStyle->SetLabelSize(0.045,"Y");
  gStyle->SetLabelSize(0.045,"Z");

  gStyle->SetTitleSize(0.048,"X");
  gStyle->SetTitleSize(0.048,"Y");
  gStyle->SetTitleSize(0.048,"Z");
  

  gStyle->SetTickLength(0.02,"X");
  gStyle->SetTickLength(0.02,"Y");
  gStyle->SetTickLength(0.02,"Z");

  gStyle->SetTitleOffset(1,"X");
  gStyle->SetTitleOffset(1,"Y");
  gStyle->SetTitleOffset(1,"Z");

  gStyle->SetLegendFillColor(kWhite);

  gROOT->ForceStyle();






}
