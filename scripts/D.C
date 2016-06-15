



void OP(TString s){
  TFile::Open(s);
}

void DS(TH1* h){
  h->Draw("same");
}



void D(TH1D* h){
  h->Draw();
}



void D(TH2D* h){
  h->Draw();
}



void D(TH1F* h){
  h->Draw();
}

void DF(TH1F* h){
  h->Fit("gaus");
}


void D(TH2F* h){
  h->Draw("colz");
}

void PX(TH2F* h){
  h->ProjectionX()->Draw();
}

void PY(TH2F* h){
  h->ProjectionY()->Draw();
}

void PX(TH2D* h){
  h->ProjectionX()->Draw();
}

void PY(TH2D* h){
  h->ProjectionY()->Draw();
}


void PXF(TH2F* h){
  h->ProjectionX()->Fit("gaus");
}

void PYF(TH2F* h){
  h->ProjectionY()->Fit("gaus");
}

void PXF(TH2D* h){
  h->ProjectionX()->Fit("gaus");
}

void PYF(TH2D* h){
  h->ProjectionY()->Fit("gaus");
}








void NWD(TH1F *h){
  new TCanvas();
  D(h);
}

void NWD(TH2F *h){
  new TCanvas();
  D(h);
}

void NWD(TH1D *h){
  new TCanvas();
  D(h);
}

