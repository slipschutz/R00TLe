
// #include <vector>
// #include "LendaEvent.hh"
// #include "TGraph.h"
// #include "TTree.h"
// #include "TCanvas.h"
// #include "TDirectory.h"

/**PLots the traces from the objects scintillator channels.  Meant to be run
   from within the ROOT interpreter when a calibrated ROOT file is open.
   Looks for a tree called 'caltree' in the current directory.

   Entry refers to the entry in caltree
   Num refers the number of Object traces to plot
   Start refers to which trace to plot first
   
   GraphObs(0,1,3) will plot only the forth copy of the 
   object scintillator from event 0

 */
void GraphObjs(Long64_t entry=0,int num=8,int start=0){


  LendaEvent * event = new LendaEvent();
  
  TTree* tree =(TTree*)gDirectory->Get("caltree");
  if (tree == NULL){
    cout<<"Cannot find caltree. Script meant to be run from within a calibrated ROOT file."<<endl;
    return;
  }
  cout<<"Found caltree..."<<endl;

  tree->SetBranchAddress("lendaevent",&event);
  tree->GetEntry(entry);

    
  Int_t NumObjects = event->NumObjectScintillators;
  if ( NumObjects == 0 ){
    cout<<"No objects found"<<endl;
    return ;
  }

  if (num +start> NumObjects ){
    cout<<"There are only "<<NumObjects <<" object scintillators in this event. Can't plot "<<num+start<<endl;
    return ;
  }

  TCanvas * objCanvas;
  if (NumObjects !=0){
    objCanvas= new TCanvas("objects");
    
    objCanvas->Divide(1,num);
  }

  TGraph ** Objects = (TGraph**)malloc(NumObjects*sizeof(TGraph*));

  cout<<event->Bars[0].Name<<"  "<<event->Bars[0].Tops[0].GetReferenceChannelName()<<" "<<event->Bars[0].Tops[0].GetCubicReferenceTimes().size()<<endl;
  for (int i=0;i<event->Bars[0].Tops[0].GetCubicReferenceTimes().size();i++){
    printf("%10.5lf\n",event->Bars[0].Tops[0].GetCubicReferenceTimes()[i]);
  }

  

  int traceSize = event->TheObjectScintillators[0].GetTrace().size()*3;
  

  cout<<"TRACE SIZE IS "<<traceSize<<endl;

  cout<<"NUMBER OF OBJECTS "<<NumObjects<<endl;

  Double_t *x = (Double_t*)malloc(traceSize*sizeof(Double_t));
  
  for (int i=0;i<traceSize;i++){
    x[i]=i;
  }


  
  Double_t * temp =(Double_t*)malloc(traceSize*sizeof(Double_t));



  Double_t TriggerTimeOfFirstObject=0;
  
  Double_t TimesForLines[40];
  Double_t TimesForTrigPoints[40];
  Double_t TimesFor1stObjTrigPoint[40];

  for (int i=start;i<start+num;i++){
    for (int j=0;j<traceSize;j++){
      temp[j]=4000;
    }

    int count=0;
    for (int j=traceSize/3;j<(2/3.)*traceSize;j++){
      temp[j]=event->TheObjectScintillators[i].GetTrace()[count];
      count++;
    }



    
    LendaFilter filt;
    vector <Double_t> CFD = filt.GetNewFirmwareCFD(event->TheObjectScintillators[i].GetTrace(),6,0,6,0);// Int_t FL, Int_t FG, Int_t d, Int_t w);
    vector <Double_t> AllZeroCrossings =filt.GetAllZeroCrossings(CFD);


    // count=0;
    // for (int j=traceSize/3;j<(2/3.)*traceSize;j++){
    //   temp[j]=CFD[count];
    //   count++;
    // }




    Double_t internalCFD=event->TheObjectScintillators[i].GetInternalCFD();

    for (int j=0;j<AllZeroCrossings.size();j++){
      double diff=internalCFD - (AllZeroCrossings[j]-TMath::Floor(AllZeroCrossings[j]));
      if (TMath::Abs(diff) < 0.001){
	//This is the correct Zero Crossing and the trigger point
	TimesForTrigPoints[i]=AllZeroCrossings[j]+traceSize/3;
	j=100000;
	break;
      }
    }

    if (i==0){
      TriggerTimeOfFirstObject=TimesForTrigPoints[i];
    }


    Double_t temp22 =(event->TheObjectScintillators[i]).GetTime());//*4.0/(TMath::Power(10,9));
    stringstream ss;
    ss.precision(20);
    ss<<" "<<temp22;


    
    
    double differenceInTrigTime2FirstEvent=TimesForTrigPoints[i]-TimesForTrigPoints[0];


    double diffInRawTStamp = event->TheObjectScintillators[i].GetTime()-event->TheObjectScintillators[0].GetTime();
    
    double CFDSpotInTrace=event->TheObjectScintillators[i].GetCubicCFD();//This is the first CFD value in the trace (relative to start of trace)
    cout<<"NUMBER OF BUMPBS "<<event->TheObjectScintillators[i].GetCubicTimes().size()<<endl;
    cout<<"The energy is "<<event->TheObjectScintillators[i].GetEnergy()<<endl;

    TimesFor1stObjTrigPoint[i]=TimesForTrigPoints[i]-diffInRawTStamp;
    TimesForLines[i]=CFDSpotInTrace + traceSize/3;//-diffInRawTStamp;
    
    
    
//    printf("Difference in Raw TStamp %30.15lf   \n",diffInRawTStamp);
    printf("Raw TStamp %10.2lf  TriggerPoint %10.2lf  CFD SpotInTrace %30.15lf Diff inRaw T-Stamp %8.4lf TimesFor1stObj %8.4lf\n",event->TheObjectScintillators[i].GetTime(),TimesForTrigPoints[i],CFDSpotInTrace,diffInRawTStamp,TimesFor1stObjTrigPoint[i]);
    //    cout<<"Diff is "<<differenceInTrigTime2FirstEvent<<"   "<<CFDSpotInTrace<<setprecision(20)<<" raw TStamp "<<event->TheObjectScintillators[i].GetTime()<<endl;

    TString timeString(ss.str().c_str());
    Objects[i]=new TGraph(traceSize,x,temp);
    Objects[i]->GetHistogram()->SetTitleSize(0.46);
    Objects[i]->SetTitle(TString(event->TheObjectScintillators[i].GetChannelName().c_str()) +timeString );
    Objects[i]->SetName(event->TheObjectScintillators[i].GetChannelName().c_str());
    Objects[i]->GetXaxis()->SetRangeUser(124,2*124);

    // for (int j=0;j<traceSize;j++){
    //   temp[j]=event->TheObjectScintillators[i].GetTrace()[j];
    // }
    // Objects[i]=new TGraph(traceSize,x,temp);
    // Objects[i]->SetTitle(event->TheObjectScintillators[i].GetChannelName().c_str());
    // Objects[i]->SetName(event->TheObjectScintillators[i].GetChannelName().c_str());

}
  
  

  
  for (int i=start;i<start+num;i++){
    objCanvas->cd(i+1-start);
    //    cout<<Objects[i]->GetHistogram()->GetMinimum()<<endl;
    TLine *l = new TLine(TimesForTrigPoints[i],Objects[i]->GetHistogram()->GetMinimum(),TimesForTrigPoints[i],Objects[i]->GetHistogram()->GetMaximum());
    l->SetLineWidth(3);

    TLine *l2 = new TLine(TimesForLines[i],Objects[i]->GetHistogram()->GetMinimum(),TimesForLines[i],Objects[i]->GetHistogram()->GetMaximum());
    l2->SetLineWidth(3);
    l2->SetLineColor(kBlue);

    TLine *l3 = new TLine(TimesFor1stObjTrigPoint[i],Objects[i]->GetHistogram()->GetMinimum(),TimesFor1stObjTrigPoint[i],Objects[i]->GetHistogram()->GetMaximum());
    l3->SetLineWidth(4);
    l3->SetLineColor(kYellow);
    
    Objects[i]->Draw("AL*");
    l->Draw();
    l2->Draw();
    if(i!=0){
      l3->Draw();
    }
  }
  
}

//   Double_t* x = malloc(size*sizeof(Double_t));
  
//   Double_t* y = malloc(size*sizeof(Double_t));

//   Double_t* y1= malloc(size*sizeof(Double_t));
  
//   Double_t* y2=malloc(size*sizeof(Double_t));
    

//   cout<<"size is "<<size<<endl;
//   for (int i=0;i<size;i++){

//     x[i]=i;
//     y[i]=event->Bars[0].Tops[0].GetTrace()[i];
//     y1[i]=event->Bars[0].Tops[0].GetFilter()[i];
//     y2[i]=event->Bars[0].Tops[0].GetCFD()[i];
//   }

//   TGraph *gr = new TGraph(size,x,y);
//   TGraph *gr1 = new TGraph(size,x,y1);
//   TGraph *gr2 = new TGraph(size,x,y2);
    
//   stringstream s;
//   s<<"Traces row "<<j<<" chan "<<event->Bars[0].Name;
//   gr->SetTitle(s.str().c_str());
//   s.str(""); s<<"Filters row "<<j<<" chan "<<event->Bars[0].Name;
//   gr1->SetTitle(s.str().c_str());
//   s.str(""); s<<"CFDs row"<<j<<" chan "<<event->Bars[0].Name;
//   gr2->SetTitle(s.str().c_str());

//   if(num!=1){
//     c->cd(j+1-entry);
//     gr->Draw("AL*");
      
//     c1->cd(j+1-entry);
//     gr1->Draw("AL*");
      
//     c2->cd(j+1-entry);
//     gr2->Draw("AL*");
//   } else{


//     c->cd();
//     gr->Draw("AL*");

//     c1->cd();
//     gr1->Draw("AL*");

//     c2->cd();
//     gr2->Draw("AL*");

//   }
  
// }
