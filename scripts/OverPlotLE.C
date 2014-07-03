


void OverPlotLE(Long64_t entry=0,int BarSpot=0){



  LendaEvent * event = new LendaEvent();
  
  TTree* tree =(TTree*)gDirectory->Get("caltree");
  
  tree->SetBranchAddress("lendaevent",&event);

  TCanvas *c = new TCanvas("c1");
  TCanvas *c1 = new TCanvas("c2");
  TCanvas *c2 = new TCanvas("c3");
  

  tree->GetEntry(entry);
  
  if (event->NumBars==0){
    cout<<"NO BARS IN EVENT"<<endl;
    return -1;
  }
  
  int NumTops  = (int) event->Bars[BarSpot].NumTops;
  int NumBottoms  = (int) event->Bars[BarSpot].NumBottoms;
  
  int TraceSize = (int)event->Bars[BarSpot].Tops[0].GetTrace().size();

  
  if ( NumBottoms ==1 && NumTops == 1){
    
    Double_t* x  = malloc(TraceSize*sizeof(Double_t));
    Double_t* yTT = malloc(TraceSize*sizeof(Double_t));
    Double_t* yTF = malloc(TraceSize*sizeof(Double_t));
    Double_t* yTC = malloc(TraceSize*sizeof(Double_t));
    
    Double_t* yBT = malloc(TraceSize*sizeof(Double_t));
    Double_t* yBF = malloc(TraceSize*sizeof(Double_t));
    Double_t* yBC = malloc(TraceSize*sizeof(Double_t));
    

    Double_t* yOT = malloc(TraceSize*sizeof(Double_t));
    Double_t* yOF = malloc(TraceSize*sizeof(Double_t));
    Double_t* yOC = malloc(TraceSize*sizeof(Double_t));
    

    for (int i=0;i<TraceSize;i++){

      x[i]=i;
      yTT[i]=event->Bars[BarSpot].Tops[0].GetTrace()[i];
      yTF[i]=event->Bars[BarSpot].Tops[0].GetFilter()[i];
      yTC[i]=event->Bars[BarSpot].Tops[0].GetCFD()[i];

      yBT[i]=event->Bars[BarSpot].Bottoms[0].GetTrace()[i];
      yBF[i]=event->Bars[BarSpot].Bottoms[0].GetFilter()[i];
      yBC[i]=event->Bars[BarSpot].Bottoms[0].GetCFD()[i];

      yOT[i]=event->TheObjectScintilator.GetTrace()[i]-840;
      yOF[i]=event->TheObjectScintilator.GetFilter()[i];
      yOC[i]=event->TheObjectScintilator.GetCFD()[i];
      

    }


  
    TGraph * gr = new TGraph(TraceSize,x,yTT);
    TGraph * gr1 = new TGraph(TraceSize,x,yTF);
    TGraph * gr2 = new TGraph(TraceSize,x,yTC);


    TGraph * gr3 = new TGraph(TraceSize,x,yBT);
    TGraph * gr4 = new TGraph(TraceSize,x,yBF);
    TGraph * gr5 = new TGraph(TraceSize,x,yBC);

    TGraph * gr11 = new TGraph(TraceSize,x,yOT);
    TGraph * gr12 = new TGraph(TraceSize,x,yOF);
    TGraph * gr13 = new TGraph(TraceSize,x,yOC);
    gr11->SetLineColor(kBlue);
    gr13->SetLineColor(kBlue);



    c->cd();
    gr->Draw("AL*");
    //    gr3->Draw("sameL*");
    gr11->Draw("sameL*");

    c1->cd();
    gr1->Draw("AL*");
    c2->cd();
    gr2->Draw("AL*");
    gr13->Draw("sameL*");
  }  


  return 0;
}

/*
    stringstream s;
    s<<"Traces row "<<j<<" chan "<<event->Bars[0].Name;
    gr->SetTitle(s.str().c_str());
    s.str(""); s<<"Filters row "<<j<<" chan "<<event->Bars[0].Name;
    gr1->SetTitle(s.str().c_str());
    s.str(""); s<<"CFDs row"<<j<<" chan "<<event->Bars[0].Name;
    gr2->SetTitle(s.str().c_str());

    if(num!=1){
      c->cd(j+1-entry);
      gr->Draw("AL*");
      
      c1->cd(j+1-entry);
      gr1->Draw("AL*");
      
      c2->cd(j+1-entry);
      gr2->Draw("AL*");
    } else{


    c->cd();
    gr->Draw("AL*");

    c1->cd();
    gr1->Draw("AL*");

    c2->cd();
    gr2->Draw("AL*");

    }
  }
}
*/
