




void D(TH1D* h){
  h->Draw();
}



void D(TH2D* h){
  h->Draw();
}



void D(TH1F* h){
  h->Draw();
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
