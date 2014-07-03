


void OverPlotLE2(Long64_t entry=0,int BarSpot=0){



  LendaEvent * event = new LendaEvent();
  
  TTree* tree =(TTree*)gDirectory->Get("caltree");
  
  tree->SetBranchAddress("lendaevent",&event);

  TCanvas *c = new TCanvas("c1");
  TCanvas *c1 = new TCanvas("c2");
  TCanvas *c2 = new TCanvas("c3");
  

  tree->GetEntry(entry);
  
  
  int TraceSize = (int)event->UnMappedChannels[0].GetTrace().size();

  
  if (event->N==2){
    
    Double_t* x  = malloc(TraceSize*sizeof(Double_t));
    Double_t* y1T = malloc(TraceSize*sizeof(Double_t));
    Double_t* y1F = malloc(TraceSize*sizeof(Double_t));
    Double_t* y1C = malloc(TraceSize*sizeof(Double_t));
    
    Double_t* y2T = malloc(TraceSize*sizeof(Double_t));
    Double_t* y2F = malloc(TraceSize*sizeof(Double_t));
    Double_t* y2C = malloc(TraceSize*sizeof(Double_t));
    

    for (int i=0;i<TraceSize;i++){

      x[i]=i;
    
      if (event->UnMappedChannels[0].GetGlobalID()==96){
	y1T[i]=event->UnMappedChannels[0].GetTrace()[i];
	y1F[i]=event->UnMappedChannels[0].GetFilter()[i];
	y1C[i]=event->UnMappedChannels[0].GetCFD()[i];
	
	
	y2T[i]=event->UnMappedChannels[1].GetTrace()[i];
	y2F[i]=event->UnMappedChannels[1].GetFilter()[i];
	y2C[i]=event->UnMappedChannels[1].GetCFD()[i];
      } else {
	
	y1T[i]=event->UnMappedChannels[1].GetTrace()[i];
	y1F[i]=event->UnMappedChannels[1].GetFilter()[i];
	y1C[i]=event->UnMappedChannels[1].GetCFD()[i];
	
	
	y2T[i]=event->UnMappedChannels[0].GetTrace()[i];
	y2F[i]=event->UnMappedChannels[0].GetFilter()[i];
	y2C[i]=event->UnMappedChannels[0].GetCFD()[i];

      }
      

    } 
   


  
    TGraph * gr = new TGraph(TraceSize,x,y1T);
    TGraph * gr1 = new TGraph(TraceSize,x,y1F);
    TGraph * gr2 = new TGraph(TraceSize,x,y1C);


    TGraph * gr3 = new TGraph(TraceSize,x,y2T);
    TGraph * gr4 = new TGraph(TraceSize,x,y2F);
    TGraph * gr5 = new TGraph(TraceSize,x,y2C);

    gr3->SetLineColor(kBlue);
    gr4->SetLineColor(kBlue);
    gr5->SetLineColor(kBlue);


    c->cd();
    gr->Draw("AL*");
    gr3->Draw("sameL*");
   
    c1->cd();
    gr1->Draw("AL*");


    c2->cd();
    gr2->Draw("AL*");
    gr5->Draw("sameL*");
  }else {
      cout<<"Bad event"<<endl;
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
