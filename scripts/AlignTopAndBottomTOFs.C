





void AlignTopAndBottomTOFs(string ArrayPart="NV",Float_t Start=-5,Float_t End=25){
  
  for (int bar=1;bar<13;bar++){
    stringstream nameTop;
    stringstream nameBottom;

    nameTop<<ArrayPart<<setfill('0')<<setw(2)<<bar<<"_TopCutTOFPr";
    TH1F * CurrentTop = (TH1F*)gDirectory->Get(nameTop.str().c_str());


    nameBottom<<ArrayPart<<setfill('0')<<setw(2)<<bar<<"_BottomCutTOFPr";
    TH1F * CurrentBottom = (TH1F*)gDirectory->Get(nameBottom.str().c_str());
  
    // TCanvas *c=new TCanvas("c");
    // c->cd(1);
    // CurrentTop->Draw();
    // CurrentBottom->Draw("same");

    int binsTop = CurrentTop->GetNbinsX();

    int binsBottom= CurrentBottom->GetNbinsX();
  
    if ( binsTop !=binsBottom){
      cout<<"Nubmer of bins for the top channel not the same as for the bottom channel"<<endl;
      cout<<"This was for "<<CurrentTop->GetName()<<" and "<<CurrentBottom->GetName()<<endl;
      return;
    }
  
    int bins=binsTop;
  
    double topLow = CurrentTop->GetBinLowEdge(1);//ROOT histograms start at bin 1. bin 0 is underflow
    double topHigh= CurrentBottom->GetBinLowEdge(binsTop) +CurrentBottom->GetBinWidth(binsTop);//ROOT histograms end at bin N. N+1 is overflow bin.

    double NanoSecsPerBin = ((topHigh-topLow)*4)/binsTop;
  
    //    cout<<topLow<<" "<<topHigh<<endl;
    //    cout<<"NanoSecsPerBin "<<NanoSecsPerBin<<endl;
  
 
    int ForthOfBins = TMath::Floor(0.25* bins);
    int EighthOfBins = TMath::Floor( (1.0/8)* bins);

    int NumberOfShifts = 2*EighthOfBins; // 1/8 of the number of bins *2 for left/right shifts
  
    int TopZeroBin = CurrentTop->GetMaximumBin();
  
    int StartBin = CurrentTop->FindBin(Start);
    int EndBin = CurrentTop->FindBin(End);

    //Find shift That moves Bottom -> Top
    vector <double> TheChi2s;
    vector <int> TheShifts;
    for (int i=0;i<NumberOfShifts;i++){
      double binShift = (i - NumberOfShifts/2);

      double chi2=0;
      // cout<<"Looking in bin range "<<TopZeroBin-EighthOfBins<<" "<<TopZeroBin+EighthOfBins<<endl;
      // cout<<"That is from "<<CurrentTop->GetBinCenter(TopZeroBin-EighthOfBins)<<" "<<CurrentTop->GetBinCenter(TopZeroBin+EighthOfBins)<<endl;

      for (int bin=StartBin ;bin<EndBin;bin++){
	double b = CurrentBottom->GetBinContent(bin + binShift);
	double t = CurrentTop->GetBinContent(bin);
	if (b !=0 && t!=0){
	  double temp =((b-t)*(b-t))/( TMath::Sqrt(t) );
	  chi2+=temp;
	}
      }
    
      TheChi2s.push_back(chi2);
      TheShifts.push_back(binShift);

    }
  
    ///Find miminum by linear search 
    double min = 9999999999.0;
    int MinSpot=-1;
    for (int i=0;i<NumberOfShifts;i++){
      if ( TheChi2s[i] < min){
	min=TheChi2s[i];
	MinSpot=i;
      }
    }

    cout<<ArrayPart<<setfill('0')<<setw(2)<<bar<<"B  "<<"1  0  "<<fixed<<setw(6)<<setprecision(4)<<TheShifts[MinSpot]/4.0*NanoSecsPerBin<<endl;
    cout<<ArrayPart<<setfill('0')<<setw(2)<<bar<<"T  "<<"1  0  0"<<endl;

  }


  cout<<"There were "<<NanoSecsPerBin<<" nano secs per bin"<<endl;
  // TGraph * graph = new TGraph();
  
  // cout<<"Size of Chi2s "<<TheChi2s.size()<<endl;
  // cout<<"Num "<<NumberOfShifts<<endl;

  // for (int i=0;i<NumberOfShifts;i++){
  //   graph->SetPoint(i,TheShifts[i],TheChi2s[i]);
  // }

  // TCanvas *c2 = new TCanvas("c2");
  // c2->cd(1);
  // graph->Draw("A*");


}

