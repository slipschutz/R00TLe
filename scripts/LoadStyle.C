


/////////////////////////////////////////////////////////////
// hopefully this will be a script to make setup a gStyle  //
// that makes pretty graphs				   //
/////////////////////////////////////////////////////////////





void LoadStyle(){

  gStyle->SetOptStat("");//Get rid of histogram stats box

  gStyle->SetLabelSize(0.06,"X");
  gStyle->SetLabelSize(0.06,"Y");
  gStyle->SetLabelSize(0.06,"Z");

  gStyle->SetTitleSize(0.06,"X");
  gStyle->SetTitleSize(0.06,"Y");
  gStyle->SetTitleSize(0.06,"Z");
  

  gStyle->SetTickLength(0.02,"X");
  gStyle->SetTickLength(0.02,"Y");
  gStyle->SetTickLength(0.02,"Z");

  gStyle->SetTitleOffset(0.8,"X");
  gStyle->SetTitleOffset(0.8,"Y");
  gStyle->SetTitleOffset(0.8,"Z");

  gStyle->SetLegendFillColor(kWhite);








}
