






void OverPlotPartOfArray(string part,string thing,int xlow=-100,int xhigh=100){


  string ArrayPart=part;

  stringstream name;

  TCanvas * c1 = new TCanvas("ETops");
  
  // TCanvas * c3 = new TCanvas("TOFTops");
  // TCanvas * c4 = new TCanvas("TOFBottoms");
  


  gStyle->SetOptStat("ne");
  c1->Divide(4,3);

  for (int i=1;i<=12;i++){
    c1->cd(i);
    //    gPad->SetLogy();
    name.str("");

    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Top"<<thing;
    cout<<name.str()<<endl;
    ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw("");

    ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlack);
    ((TH1F*)gDirectory->Get(name.str().c_str()))->GetXaxis()->SetRangeUser(xlow,xhigh);    
    name.str("");

    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Bottom"<<thing;
    

    ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlue);
    ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineWidth(2);

    ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw("same");





  }
}
