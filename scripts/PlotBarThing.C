






void PlotBarThing(string thing,int xlow=-100,int xhigh=100,string plotOpt=""){



  stringstream name;

  TCanvas * c1 = new TCanvas("ETops");
  TCanvas * c2 = new TCanvas("EBottoms");
  
  TCanvas * c3 = new TCanvas("TOFTops");
  TCanvas * c4 = new TCanvas("TOFBottoms");
  


  gStyle->SetOptStat("ne");
  c1->Divide(4,3);
  c2->Divide(4,3);

  c3->Divide(4,3);
  c4->Divide(4,3);

  for (int i=1;i<=12;i++){
    c1->cd(i);
    //    gPad->SetLogy();
    name.str("");

    name<<"SL"<<setfill('0')<<setw(2)<<i<<"_"<<thing;
    cout<<name.str()<<endl;

    ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw(plotOpt.c_str());
    ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlack);

    ((TH1F*)gDirectory->Get(name.str().c_str()))->GetXaxis()->SetRangeUser(xlow,xhigh);
    

    c2->cd(i);
    name.str("");
    //gPad->SetLogy();

    name<<"NL"<<setfill('0')<<setw(2)<<i<<"_"<<thing;
    
    ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw(plotOpt.c_str());
    ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlack);
    ((TH1F*)gDirectory->Get(name.str().c_str()))->GetXaxis()->SetRangeUser(xlow,xhigh);

    c3->cd(i);
    name.str("");
    //gPad->SetLogy();

    name<<"SV"<<setfill('0')<<setw(2)<<i<<"_"<<thing;
    
    ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw(plotOpt.c_str());
    ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlack);
    ((TH1F*)gDirectory->Get(name.str().c_str()))->GetXaxis()->SetRangeUser(xlow,xhigh);

    c4->cd(i);
    name.str("");
    //gPad->SetLogy();

    name<<"NV"<<setfill('0')<<setw(2)<<i<<"_"<<thing;
    
    ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw(plotOpt.c_str());
    ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlack);
    ((TH1F*)gDirectory->Get(name.str().c_str()))->GetXaxis()->SetRangeUser(xlow,xhigh);




    // c3->cd(i);
    // name.str("");

    // name<<"TOF"<<ArrayPart<<setfill('0')<<setw(2)<<i<<"T";
    
    // ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw("");
    // ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlack);

    // c4->cd(i);
    // name.str("");

    // name<<"TOF"<<ArrayPart<<setfill('0')<<setw(2)<<i<<"B";
    
    // ((TH1F*)gDirectory->Get(name.str().c_str()))->Draw("");
    // ((TH1F*)gDirectory->Get(name.str().c_str()))->SetLineColor(kBlack);




  }
}
