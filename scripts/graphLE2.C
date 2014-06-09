


void graphLE2(Long64_t entry=0,int num=2,int spot=0){



  LendaEvent * event = new LendaEvent();
  
  TTree* tree =(TTree*)gDirectory->Get("caltree");
  
  tree->SetBranchAddress("lendaevent",&event);

  TCanvas *c = new TCanvas("c12");
  TCanvas *c1 = new TCanvas("c22");
  TCanvas *c2 = new TCanvas("c32");
  
  if (num!=1){ //special case
    if (num !=0 ){
      c->Divide(2,num/2);
      c1->Divide(2,num/2);
      c2->Divide(2,num/2);
      
    }
    if (num % 2 !=0 ){
      cout<<"Use even number"<<endl;
      return;
      
    }
  }

  for (int j=entry;j<num+entry;j++){

    tree->GetEntry(j);
    
    
    int size = (int) event->Bars[0].Tops[0].GetTrace().size();

    Double_t* x = malloc(size*sizeof(Double_t));
    Double_t* y = malloc(size*sizeof(Double_t));

    Double_t* y1= malloc(size*sizeof(Double_t));
    

    Double_t* y2=malloc(size*sizeof(Double_t));
    

    cout<<"size is "<<size<<endl;
    for (int i=0;i<size;i++){

      x[i]=i;
      y[i]=event->Bars[0].Tops[0].GetTrace()[i];
      y1[i]=event->Bars[0].Tops[0].GetFilter()[i];
      y2[i]=event->Bars[0].Tops[0].GetCFD()[i];
    }

    TGraph *gr = new TGraph(size,x,y);
    TGraph *gr1 = new TGraph(size,x,y1);
    TGraph *gr2 = new TGraph(size,x,y2);
    
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
