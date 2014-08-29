





void PlotERes(string FilterSet){


  stringstream name;
  name<<FilterSet<<"_vsEnergy";

  
  TH2F* thePlot = (TH2F*)gDirectory->Get(name.str().c_str());

  if (thePlot==NULL){
    cout<<"No plot"<<endl;
    return;
  }

  int numYBins = thePlot->GetNbinsY();

  TF1 * myFunc = new TF1("myFunc","gaus",-0.3,0.3);
  

  double * x = (double*)malloc(numYBins*sizeof(double));
  double * y = (double*)malloc(numYBins*sizeof(double));
  double * ey = (double*)malloc(numYBins*sizeof(double));
  
  for (int i=0;i<numYBins;i++){
    


    TH1D* proj=thePlot->ProjectionX("_px",i,numYBins-1);
    if (proj->GetEntries()>40){
      TFitResultPtr result = proj->Fit("myFunc","RQS");
      Int_t status = result;
      if (status==0){
	cout<<result->Value(2)*2.35*4<<endl;
	x[i]=i;
	y[i]=result->Value(2)*2.35*4;
	ey[i]=result->UpperError(2)*2.35*4;
      }
    }
  }
  
  
  TGraphErrors * graph = new TGraphErrors(numYBins,x,y,0,ey);
  graph->GetHistogram()->SetMarkerStyle(5);
  graph->Draw("AP");
}


