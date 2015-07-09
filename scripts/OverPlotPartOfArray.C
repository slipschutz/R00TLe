

TH1F * SafeGet(TString name){

 

  TH1F * hist =(TH1F*) gDirectory->Get(name.Data());
  if ( hist == NULL){
    cout<<"Could not find "<<name.Data()<<endl;
    return NULL;
  }
  return hist;
}

void OverPlotPartOfArray(string part,string thing1,string thing2,int xlow=-100,int xhigh=100,TString opt=""){


  string ArrayPart=part;

  stringstream name;

  TCanvas * c1 = new TCanvas("Tops");
  TCanvas * c2 = new TCanvas("Bottoms");
  
  // TCanvas * c3 = new TCanvas("TOFTops");
  // TCanvas * c4 = new TCanvas("TOFBottoms");
  


  gStyle->SetOptStat("ne");
  c1->Divide(4,3);
  c2->Divide(4,3);

  for (int i=1;i<=12;i++){
    c1->cd(i);
    //    gPad->SetLogy();
    name.str("");
    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Top"<<thing1;
    TH1F * top1 = SafeGet(name.str().c_str());
    
    name.str("");
    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Top"<<thing2;
    TH1F * top2 = SafeGet(name.str().c_str());
    


    top1->SetLineColor(kRed);
    top1->GetXaxis()->SetRangeUser(xlow,xhigh);
    top1->Draw(opt);

    top2->SetLineColor(kBlack);
    top2->GetXaxis()->SetRangeUser(xlow,xhigh);
    top2->Draw("same"+opt);

    
 
    c2->cd(i);
    name.str("");
    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Bottom"<<thing1;
    TH1F * bottom1 = SafeGet(name.str().c_str());
    
    name.str("");
    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Bottom"<<thing2;
    TH1F * bottom2 = SafeGet(name.str().c_str());
    


    bottom1->SetLineColor(kRed);
    bottom1->GetXaxis()->SetRangeUser(xlow,xhigh);
    bottom1->Draw(opt);

    bottom2->SetLineColor(kBlack);
    bottom2->GetXaxis()->SetRangeUser(xlow,xhigh);
    bottom2->Draw("same"+opt);


  }
}
