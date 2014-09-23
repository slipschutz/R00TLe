
#include <vector>

void GraphLE(Long64_t entry=0,int BarNum=0){


  LendaEvent * event = new LendaEvent();
  
  TTree* tree =(TTree*)gDirectory->Get("caltree");
  
  tree->SetBranchAddress("lendaevent",&event);
  
  TCanvas *c = new TCanvas("c121");
  TCanvas *c1 = new TCanvas("c221");
  TCanvas *c2 = new TCanvas("c321");

  tree->GetEntry(entry);

  Int_t NumBars = event->NumBars;
  
  if (BarNum >= NumBars){
    cout<<"Event has "<<NumBars<<" can't plot bar "<<BarNum<<endl;
    return;
  }
  
  Int_t NumTops = event->Bars[BarNum].NumTops;
  Int_t NumBottoms = event->Bars[BarNum].NumBottoms;

  Int_t largerNum;

  if (NumTops>NumBottoms){
    largerNum=NumTops;
  }else {
    largerNum=NumBottoms;
  }

  
  c->Divide(2,largerNum);
  c1->Divide(2,largerNum);
  c2->Divide(2,largerNum);

  int traceSize=-1000;  
  if (NumTops != 0){
    traceSize = event->Bars[BarNum].Tops[0].GetTrace().size();
  } else if (NumBottoms !=0){
    traceSize = event->Bars[BarNum].Bottoms[0].GetTrace().size();
  }

  cout<<"TRACE SIZE IS "<<traceSize<<endl;
  Double_t *x = (Double_t*)malloc(traceSize*sizeof(Double_t));
  
  for (int i=0;i<traceSize;i++){
    x[i]=i;
  }
  

  std::vector <TGraph*> BottomTraces;
  BottomTraces.resize(NumBottoms);

  std::vector <TGraph*> TopTraces;
  TopTraces.resize(NumTops);


  std::vector <TGraph*> BottomCFDs;
  BottomCFDs.resize(NumBottoms);

  std::vector <TGraph*> TopCFDs;
  TopCFDs.resize(NumTops);


  std::vector <TGraph*> TopFilters;
  TopFilters.resize(NumTops);

  std::vector <TGraph*> BottomFilters;
  BottomFilters.resize(NumTops);



  Double_t * temp =(Double_t*)malloc(traceSize*sizeof(Double_t));

  for (int i=0;i<NumBottoms;i++){

    for (int j=0;j<traceSize;j++){
      temp[j]=event->Bars[BarNum].Bottoms[i].GetTrace()[j];
    }
    BottomTraces[i] = new TGraph(traceSize,x,temp);
    BottomTraces[i]->SetTitle((event->Bars[BarNum].Bottoms[i].GetChannelName()+string(" Trace")).c_str());
    BottomTraces[i]->SetName(event->Bars[BarNum].Bottoms[i].GetChannelName().c_str());

    
    for (int j=0;j<traceSize;j++){
      temp[j]=event->Bars[BarNum].Bottoms[i].GetCFD()[j];
    }
    BottomCFDs[i] = new TGraph(traceSize,x,temp);
    BottomCFDs[i]->SetTitle((event->Bars[BarNum].Bottoms[i].GetChannelName()+string(" CFD")).c_str());
    BottomCFDs[i]->SetName(event->Bars[BarNum].Bottoms[i].GetChannelName().c_str());


    for (int j=0;j<traceSize;j++){
      temp[j]=event->Bars[BarNum].Bottoms[i].GetFilter()[j];
    }
    BottomFilters[i] = new TGraph(traceSize,x,temp);
    BottomFilters[i]->SetTitle((event->Bars[BarNum].Bottoms[i].GetChannelName()+string(" Filter")).c_str());
    BottomFilters[i]->SetName(event->Bars[BarNum].Bottoms[i].GetChannelName().c_str());



  }
  

  for (int i=0;i<NumTops;i++){
    for (int j=0;j<traceSize;j++){
      temp[j]=event->Bars[BarNum].Tops[i].GetTrace()[j];
    }
    TopTraces[i] = new TGraph(traceSize,x,temp);
    TopTraces[i]->SetTitle((event->Bars[BarNum].Tops[i].GetChannelName()+string(" Trace")).c_str());
    TopTraces[i]->SetName(event->Bars[BarNum].Tops[i].GetChannelName().c_str());
    
    for (int j=0;j<traceSize;j++){
      temp[j]=event->Bars[BarNum].Tops[i].GetCFD()[j];
    }
    TopCFDs[i] = new TGraph(traceSize,x,temp);
    TopCFDs[i]->SetTitle((event->Bars[BarNum].Tops[i].GetChannelName()+string(" CFD")).c_str());
    TopCFDs[i]->SetName(event->Bars[BarNum].Tops[i].GetChannelName().c_str());


    for (int j=0;j<traceSize;j++){
      temp[j]=event->Bars[BarNum].Tops[i].GetFilter()[j];
    }
    TopFilters[i] = new TGraph(traceSize,x,temp);
    TopFilters[i]->SetTitle((event->Bars[BarNum].Tops[i].GetChannelName()+string(" Filter")).c_str());
    TopFilters[i]->SetName(event->Bars[BarNum].Tops[i].GetChannelName().c_str());

 
  }






  free(temp);
  gStyle->SetMarkerStyle(4);
  gStyle->UseCurrentStyle();
  for (int i=0;i<NumBottoms;i++){
    c->cd(2*i+1);
    BottomTraces[i]->Draw("ALp");
    c1->cd(2*i+1);
    BottomCFDs[i]->Draw("ALp");
    c2->cd(2*i+1);
    BottomFilters[i]->Draw("ALp");
  }
  for (int i=0;i<NumTops;i++){
    c->cd(2*i+2);
    TopTraces[i]->Draw("ALp");
    c1->cd(2*i+2);
    TopCFDs[i]->Draw("ALp");
    c2->cd(2*i+2);
    TopFilters[i]->Draw("ALp");
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
