




/**This script is meant to take a 2-D histogram and show the centroid of X (from gaussian fit) as a function of Y.
   For example if X is a time difference and Y is a pulse height it will show how the centriod
   of the time difference spectra changes with pulse height ( 'Walk')
   
   Argument description:
   h: the 2-D histogram <BR>
   Start: the lowest value in the Y variable to consider <BR>
   End: the highest value in the Y variable to consdier <BR>
   NumPoints: the number of slices to make and the number of points in the resulting graph <BR>
   YScale: number to DIVIDE the Y values by. <BR>
   XScale: number to MULTIPLY the X values by. <BR>

   *NOTE* the Y values will be on the x axis in the resulting graph.  Script makes Width of X as function of Y <BR>

   FitLowCut: low edge in X for the gaussian fit <BR>
   FitHighCut: high edge in X for the gaussian fit <BR>
   plotG: Ploting flags for TGraph::Draw() <BR>
   color: color number for resulting plot <BR>
   

 */
TGraphErrors* MakeCentroiProjectionPlot(TH2F* h , Double_t Start=0,Double_t End=16000,Int_t NumPoints=20,Double_t YScale=1,Double_t XScale=1,Double_t FitLowCut=-10,Double_t FitHighCut=10,TString plotG="AP",Int_t color=kBlack){


  Int_t numYbins = h->GetNbinsY();
  cout<<h->GetName()<<" has "<<numYbins <<" bins along the y axis"<<endl;
  TGraphErrors * gr = new TGraphErrors();
  stringstream dd;


  Int_t PointCount=0;
  
  Double_t step = (End-Start)/NumPoints;
  cout<<"Slice size along Y axis is "<<step<<endl;


  for (int i=0;i<NumPoints;i++){
    dd.str("");
    dd<<"hProj"<<i;
    Double_t ii =i;
    Int_t StartBin = h->GetYaxis()->FindBin(Start + i*step);
    //    Int_t EndBin = h->GetYaxis()->FindBin(Start + ((ii+window)/100.0)*(End-Start));
    Int_t EndBin = h->GetYaxis()->FindBin(Start+ (i+1)*step);

    //    cout<<"Start Bin "<<StartBin<<" end bin "<<EndBin<<endl;
    TH1D* temp = h->ProjectionX(dd.str().c_str(),StartBin,EndBin);
    


    if (temp->GetEntries() > 0 ){

      TFitResultPtr result = temp->Fit("gaus","QS","",FitLowCut,FitHighCut);
      
      Int_t status = result;
      
      if (status == 0){
	//	cout<<temp->GetEntries()<<" "<<result->Value(2)*4*2.35*1000<<","<<h->GetYaxis()->GetBinCenter(TMath::Floor((StartBin+EndBin)/2))<<endl;

	gr->SetPoint(PointCount,h->GetYaxis()->GetBinCenter(TMath::Nint((StartBin+EndBin)/2.0))/YScale,result->Value(1)*XScale);

	gr->SetPointError(PointCount,0,result->UpperError(2)*XScale);
	PointCount++;
      }
    }
    
    //    gDirectory->RecursiveRemove(temp);

  }//end loop over i

  gr->SetMarkerStyle(8);
  gr->SetMarkerSize(1);
  gr->SetMarkerColor(color);
  
  gr->SetLineColor(color);
  gr->SetFillColor(color);
  gr->SetLineWidth(2);


  gr->Draw(plotG.Data());

  gr->SetName(h->GetName()+TString("_Graph"));
  return gr;
}
