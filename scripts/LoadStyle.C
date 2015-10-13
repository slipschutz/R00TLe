


/////////////////////////////////////////////////////////////
// hopefully this will be a script to make setup a gStyle  //
// that makes pretty graphs				   //
/////////////////////////////////////////////////////////////





void LoadStyle(){

  gStyle->SetOptStat(0);//Get rid of histogram stats box

  gStyle->SetLabelSize(0.050,"X");
  gStyle->SetLabelSize(0.050,"Y");
  gStyle->SetLabelSize(0.050,"Z");

  gStyle->SetTitleSize(0.06,"X");
  gStyle->SetTitleSize(0.06,"Y");
  gStyle->SetTitleSize(0.06,"Z");
  

  gStyle->SetTickLength(0.02,"X");
  gStyle->SetTickLength(0.02,"Y");
  gStyle->SetTickLength(0.02,"Z");

  gStyle->SetTitleOffset(0.8,"X");
  gStyle->SetTitleOffset(0.8,"Y");
  gStyle->SetTitleOffset(0.8,"Z");

  gStyle->SetLegendFillColor(kWhite);

  gROOT->ForceStyle();

  TList *list = gDirectory->GetList();

  for (int i=0;i<list->GetSize();i++){
    list->At(i)->UseCurrentStyle();
    TObject *obj =gPad->GetPrimitive(list->At(i)->GetName());
    if (obj !=NULL){
      obj->Draw();
    }
  }

  list =gDirectory->GetListOfKeys();
  for (int i=0;i<list->GetSize();i++){
    cout<<list->At(i)->GetName()<<endl;
    TObject * obj =(gDirectory->Get(list->At(i)->GetName()));
    obj->UseCurrentStyle();
    obj->Draw();
  }

  


}
