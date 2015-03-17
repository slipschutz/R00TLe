



#include "S800.hh"
#include "S800Event.hh"
void GraphCrdcPads(int num=0){

  //This script will look at the Raw root trees
  //and plot a single crdc pad distribution 

  S800Event * event= new S800Event();

  rawtree->SetBranchAddress("s800event",&event);

  rawtree->GetEntry(num);

  // cout<<"Size of samples "<<event->GetS800()->GetCrdc(0)->GetSample().size()<<endl;
  // cout<<"Size of data "<<event->GetS800()->GetCrdc(0)->GetData().size()<<endl;
  // cout<<"Size of channels "<<event->GetS800()->GetCrdc(0)->GetChannels().size()<<endl;

  int size = event->GetS800()->GetCrdc(0)->GetChannels().size();

  // for (int i=0;i<size;i++){
  //   cout<<event->GetS800()->GetCrdc(0)->GetChannels()[i]<<" "
  // 	<<event->GetS800()->GetCrdc(0)->GetData()[i]<<endl;
  // }cout<<endl;

  S800Calibration calibration(new S800Settings());
  calibration.ReadCrdcCalibration("/user/e10003/R00TLe/prm/crdcal.dat","/user/e10003/R00TLe/prm/crdcpedestals_real.dat");

  calibration.CrdcCal(event->GetS800()->GetCrdc(0)->GetChannels(),event->GetS800()->GetCrdc(0)->GetData(),0);

  int size2=calibration.GetCRDCCal().size();
  TGraph * graph = new TGraph();
  for (int i=0;i<size2;i++){
    graph->SetPoint(i,i,calibration.GetCRDCCal()[i]);
  }
  graph->Fit("gaus");
  graph->Draw("A*");
  return;
}
