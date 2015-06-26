



#include "S800.hh"
#include "S800Event.hh"
void GraphCrdcPads(int num=0,int CRDCNum=0){

  //This script will look at the Raw root trees
  //and plot a single crdc pad distribution 

  S800Event * event= new S800Event();

  rawtree->SetBranchAddress("s800event",&event);

  rawtree->GetEntry(num);

  // cout<<"Size of samples "<<event->GetS800()->GetCrdc(0)->GetSample().size()<<endl;
  // cout<<"Size of data "<<event->GetS800()->GetCrdc(0)->GetData().size()<<endl;
  // cout<<"Size of channels "<<event->GetS800()->GetCrdc(0)->GetChannels().size()<<endl;

  int size = event->GetS800()->GetCrdc(CRDCNum)->GetChannels().size();



  TString install =gSystem->Getenv("R00TLeInstall");
  
  TString calfile = install+"/prm/crdccalNone.dat";
  TString pedfile = install+"/prm/crdcpedestals.dat";
  
  S800Calibration calibration;//new S800Settings(install+"/prm/Raw2Cal.dat"));
  calibration.ReadCrdcCalibration(calfile,pedfile);


  //calibration.SetCrdc(event->GetS800()->GetCrdc(0)->GetChannels(),event->GetS800()->GetCrdc(0)->GetData(),0,0,0);

  calibration.CrdcCal(event->GetS800()->GetCrdc(CRDCNum)->GetChannels(),event->GetS800()->GetCrdc(CRDCNum)->GetData(),0);

  int size2=calibration.GetCRDCCal().size();
  TGraph * graph = new TGraph();
  for (int i=0;i<size2;i++){
    if (TMath::IsNaN(calibration.GetCRDCCal()[i])){
      graph->SetPoint(i,i,0);
    }else {
      graph->SetPoint(i,i,calibration.GetCRDCCal()[i]);
    }
  }
  graph->Fit("gaus");
  graph->Draw("A*");
  return;
}
