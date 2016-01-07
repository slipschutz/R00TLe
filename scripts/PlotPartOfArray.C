






void PlotPartOfArray(string part,string thing,int xlow=-100,int xhigh=100,TString opt=""){


  string ArrayPart=part;

  std::stringstream name;

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

    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Top"<<thing;
    cout<<"Plotting "<<name.str()<<endl;

    TH1F * top =(TH1F*) gDirectory->Get(name.str().c_str());
    if ( top == NULL){
      cout<<"Could not find "<<name.str()<<endl;
      return;
    }
    top->SetLineColor(kBlack);
    top->GetXaxis()->SetRangeUser(xlow,xhigh);

    top->Draw(opt);

    
 
    c2->cd(i);
    name.str("");
    name<<ArrayPart<<setfill('0')<<setw(2)<<i<<"_Bottom"<<thing;
    cout<<"Plotting "<<name.str()<<endl;

    TH1F * bottom =(TH1F*) gDirectory->Get(name.str().c_str());
    if ( bottom== NULL){
      cout<<"Could not find "<<name.str()<<endl;
      return;
    }
    bottom->SetLineColor(kBlue);
    bottom->GetXaxis()->SetRangeUser(xlow,xhigh);

    bottom->Draw(opt);
  }


}
