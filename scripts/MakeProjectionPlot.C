






#include <sstream>
TGraphErrors* MakeProjectionPlot(TH2F* h , Double_t Start=0,Double_t End=16000,Int_t NumPoints=20,Double_t scale=1,TString plotG="AP",Int_t color=kBlack){


  Int_t numYbins = h->GetNbinsY();
  cout<<"Has "<<numYbins <<" bins"<<endl;
  TGraphErrors * gr = new TGraphErrors();
  stringstream dd;


  Int_t PointCount=0;
  
  Double_t step = (End-Start)/NumPoints;
  cout<<"Step is "<<step<<endl;


  for (int i=0;i<NumPoints;i++){
    dd.str("");
    dd<<"hProj"<<i;
    Double_t ii =i;
    Int_t StartBin = h->GetYaxis()->FindBin(Start + i*step);
    //    Int_t EndBin = h->GetYaxis()->FindBin(Start + ((ii+window)/100.0)*(End-Start));
    Int_t EndBin = h->GetYaxis()->FindBin(Start+ (i+1)*step);

    cout<<"Start Bin "<<StartBin<<" end bin "<<EndBin<<endl;
    TH1D* temp = h->ProjectionX(dd.str().c_str(),StartBin,EndBin);
    


    if (temp->GetEntries() > 0 ){

      TFitResultPtr result = temp->Fit("gaus","QSN");
      
      Int_t status = result;
      
      if (status == 0){
	//	cout<<temp->GetEntries()<<" "<<result->Value(2)*4*2.35*1000<<","<<h->GetYaxis()->GetBinCenter(TMath::Floor((StartBin+EndBin)/2))<<endl;

	gr->SetPoint(PointCount,h->GetYaxis()->GetBinCenter(TMath::Nint((StartBin+EndBin)/2.0))/scale,result->Value(2)*4*2.35*1000/sqrt(2));

	gr->SetPointError(PointCount,0,result->UpperError(2)*4*2.35*1000/sqrt(2));
	PointCount++;
      }
    }
    
    //    gDirectory->RecursiveRemove(temp);

  }//end loop over i
  // gStyle->SetMarkerStyle(8);
  // gStyle->SetMarkerSize(2);
  // gStyle->SetMarkerColor(color);

  gr->SetMarkerStyle(8);
  gr->SetMarkerSize(2);
  gr->SetMarkerColor(color);
  
  gr->SetLineColor(color);
  gr->SetFillColor(color);
  gr->SetLineWidth(2);


  gr->Draw(plotG.Data());

  gr->SetName(h->GetName()+TString("_Graph"));
  return gr;
}
