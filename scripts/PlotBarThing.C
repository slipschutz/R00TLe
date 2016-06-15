


TH1F * SafeGet(TString name,TString xLabel, TString yLabel){

 

  TH1F * hist =(TH1F*) gDirectory->Get(name.Data());
  if ( hist == NULL){
    cout<<"Could not find "<<name.Data()<<endl;
    return NULL;
  }
  // //  hist->Rebin(10);
  // hist->SetTitle("");
  // hist->GetXaxis()->SetTitle(xLabel);
  // hist->GetYaxis()->SetTitle(yLabel);


  // hist->GetListOfFunctions()->Clear();

  // gStyle->SetOptStat(0);

  // hist->UseCurrentStyle();
  // gPad->SetLogy();
  // hist->RebinX(4);
  return hist;
}




void PlotBarThing(string thing,double xlow=-100,double xhigh=100,double ylow=-1,double yhigh=-1,string plotOpt="",TString xLabel="",TString yLabel=""){



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

  TH1F * hist;

  for (int i=1;i<=12;i++){
    c1->cd(i);
    //gPad->SetLogy();
    name.str("");

    name<<"SL"<<setfill('0')<<setw(2)<<i<<"_"<<thing;
    cout<<name.str()<<endl;

    
    hist = SafeGet(name.str().c_str(),xLabel,yLabel);
    if (hist){
      hist->Draw(plotOpt.c_str());
      hist->SetLineColor(kBlack);
      hist->GetXaxis()->SetRangeUser(xlow,xhigh);
      if (ylow!=-1&&yhigh!=-1){
	hist->GetYaxis()->SetRangeUser(ylow,yhigh);
      }
    }

    c2->cd(i);
    name.str("");
    //    gPad->SetLogy();

    name<<"NL"<<setfill('0')<<setw(2)<<i<<"_"<<thing;

    hist = SafeGet(name.str().c_str(),xLabel,yLabel);
    if (hist){
      hist->Draw(plotOpt.c_str());
      hist->SetLineColor(kBlack);
      hist->GetXaxis()->SetRangeUser(xlow,xhigh);
      if (ylow!=-1&&yhigh!=-1){
	hist->GetYaxis()->SetRangeUser(ylow,yhigh);
      }

    }
    c3->cd(i);
    name.str("");
    //    gPad->SetLogy();

    name<<"SV"<<setfill('0')<<setw(2)<<i<<"_"<<thing;
    hist = SafeGet(name.str().c_str(),xLabel,yLabel);
    if (hist){
      hist->Draw(plotOpt.c_str());
      hist->SetLineColor(kBlack);
      hist->GetXaxis()->SetRangeUser(xlow,xhigh);
      if (ylow!=-1&&yhigh!=-1){
	hist->GetYaxis()->SetRangeUser(ylow,yhigh);
      }

    }
    
    c4->cd(i);
    name.str("");
    // gPad->SetLogy();

    

    name<<"NV"<<setfill('0')<<setw(2)<<i<<"_"<<thing;

    hist = SafeGet(name.str().c_str(),xLabel,yLabel);
    if (hist){
      hist->Draw(plotOpt.c_str());
      hist->SetLineColor(kBlack);
      hist->GetXaxis()->SetRangeUser(xlow,xhigh);
      if (ylow!=-1&&yhigh!=-1){
	hist->GetYaxis()->SetRangeUser(ylow,yhigh);
      }

    }



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
