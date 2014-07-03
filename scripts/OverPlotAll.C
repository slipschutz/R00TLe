





void OverPlotAll(){

  int num=gDirectory->GetListOfKeys()->GetSize();

  
  gDirectory->Get(gDirectory->GetListOfKeys()->At(0)->GetName())->Draw();

  for (int i=1;i<num;i++){
    gDirectory->Get(gDirectory->GetListOfKeys()->At(i)->GetName())->Draw("same");


  }




}
