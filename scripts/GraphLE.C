
#include <vector>
/**Plots trace, trigger filter and CFD filter for a lendaevent.  Meant to be run
   in the ROOT interpreter when a calibrated rootfile is open.  Script will
   look for a tree called 'caltree' in the current directory.  Will only plot
   the trace, trigger filter or CFD filter if they have been saved in the 
   calibarated tree.

   Entry referes to the entry number in the tree.  

   Bar num selects which bar in the
   event to plot (if two bars fired barNum=0 is first one barNum=1 is second one)

   traceOnly will force only traces to be ploted 

 */
void GraphLE(Long64_t entry=0,int BarNum=0,bool traceOnly=false){


  LendaEvent * event = new LendaEvent();
  
  TTree* tree =(TTree*)gDirectory->Get("caltree");
  if (tree == NULL){
    cout<<"Can't find caltree.  Script meant to be run when in calbirated ROOT file."<<endl;
    return;
  }
  tree->SetBranchAddress("lendaevent",&event);
  

  tree->GetEntry(entry);

  Int_t NumBars = event->NumBars;
  
  if (BarNum >= NumBars){
    cout<<"Event has "<<NumBars<<" bars in it.  Can't plot bar "<<BarNum<<endl;
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


  TCanvas* c;
  TCanvas* c1;
  TCanvas* c2;


  int traceSize=-1000;  
  if (NumTops != 0){
    traceSize = event->Bars[BarNum].Tops[0].GetTrace().size();

    if (event->Bars[BarNum].Tops[0].GetTrace().size()!=0){
      c = new TCanvas("cTraces");
      c->Divide(2,largerNum);  
    }
    if (! traceOnly){
      if (event->Bars[BarNum].Tops[0].GetCFD().size()!=0){
	c1 = new TCanvas("cCFDs");
	c1->Divide(2,largerNum);
      }
      
      if (event->Bars[BarNum].Tops[0].GetFilter().size()!=0){
	c2 = new TCanvas("cFilters");
	c2->Divide(2,largerNum);
      }
    }
  } else if (NumBottoms !=0){
    traceSize = event->Bars[BarNum].Bottoms[0].GetTrace().size();
    
    if (event->Bars[BarNum].Bottoms[0].GetTrace().size()!=0){
      c = new TCanvas("cTraces");
      c->Divide(2,largerNum);  
    }
    if (! traceOnly){
      if (event->Bars[BarNum].Bottoms[0].GetCFD().size()!=0){
	c1 = new TCanvas("cCFDs");
	c1->Divide(2,largerNum);
      }
      
      if (event->Bars[BarNum].Bottoms[0].GetFilter().size()!=0){
	c2 = new TCanvas("cFilters");
      c2->Divide(2,largerNum);
      }
    }
  }

  Int_t NumObjects = event->NumObjectScintillators;
  if (NumObjects !=0){
    TCanvas * objCanvas = new TCanvas("objects");

    objCanvas->Divide(1,NumObjects);
  }
  std::vector <TGraph*> Objects;
  Objects.resize(NumObjects,NULL);
  


  cout<<"TRACE SIZE IS "<<traceSize<<endl;
  Double_t *x = (Double_t*)malloc(traceSize*sizeof(Double_t));
  
  for (int i=0;i<traceSize;i++){
    x[i]=i;
  }
  

  std::vector <TGraph*> BottomTraces;
  BottomTraces.resize(NumBottoms,NULL);

  std::vector <TGraph*> TopTraces;
  TopTraces.resize(NumTops,NULL);


  std::vector <TGraph*> BottomCFDs;
  BottomCFDs.resize(NumBottoms,NULL);

  std::vector <TGraph*> TopCFDs;
  TopCFDs.resize(NumTops,NULL);


  std::vector <TGraph*> TopFilters;
  TopFilters.resize(NumTops,NULL);

  std::vector <TGraph*> BottomFilters;
  BottomFilters.resize(NumTops,NULL);



  Double_t * temp =(Double_t*)malloc(traceSize*sizeof(Double_t));

  for (int i=0;i<NumBottoms;i++){

    if (event->Bars[BarNum].Bottoms[i].GetTrace().size()!=0){
      for (int j=0;j<traceSize;j++){
  	temp[j]=event->Bars[BarNum].Bottoms[i].GetTrace()[j];
  	//temp[j]=event->TheObjectScintillators[0]->GetTrace()[j];
      }
      BottomTraces[i] = new TGraph(traceSize,x,temp);
      BottomTraces[i]->SetTitle((event->Bars[BarNum].Bottoms[i].GetChannelName()+string(" Trace")).c_str());
      BottomTraces[i]->SetName(event->Bars[BarNum].Bottoms[i].GetChannelName().c_str());
    }
    if (event->Bars[BarNum].Bottoms[i].GetCFD().size()!=0){
      for (int j=0;j<traceSize;j++){
  	temp[j]=event->Bars[BarNum].Bottoms[i].GetCFD()[j];
      }
      BottomCFDs[i] = new TGraph(traceSize,x,temp);
      BottomCFDs[i]->SetTitle((event->Bars[BarNum].Bottoms[i].GetChannelName()+string(" CFD")).c_str());
      BottomCFDs[i]->SetName(event->Bars[BarNum].Bottoms[i].GetChannelName().c_str());
    }

    if (event->Bars[BarNum].Bottoms[i].GetFilter().size()!=0){
      for (int j=0;j<traceSize;j++){
  	temp[j]=event->Bars[BarNum].Bottoms[i].GetFilter()[j];
      }
      BottomFilters[i] = new TGraph(traceSize,x,temp);
      BottomFilters[i]->SetTitle((event->Bars[BarNum].Bottoms[i].GetChannelName()+string(" Filter")).c_str());
      BottomFilters[i]->SetName(event->Bars[BarNum].Bottoms[i].GetChannelName().c_str());
    }
  }
  

  for (int i=0;i<NumTops;i++){

    if (event->Bars[BarNum].Tops[i].GetTrace().size()!=0){
      for (int j=0;j<traceSize;j++){
  	temp[j]=event->Bars[BarNum].Tops[i].GetTrace()[j];
      }
      TopTraces[i] = new TGraph(traceSize,x,temp);
      TopTraces[i]->SetTitle((event->Bars[BarNum].Tops[i].GetChannelName()+string(" Trace")).c_str());
      TopTraces[i]->SetName(event->Bars[BarNum].Tops[i].GetChannelName().c_str());
    }
    
    if (event->Bars[BarNum].Tops[i].GetCFD().size()!=0){
      for (int j=0;j<traceSize;j++){
  	temp[j]=event->Bars[BarNum].Tops[i].GetCFD()[j];
      }
      TopCFDs[i] = new TGraph(traceSize,x,temp);
      TopCFDs[i]->SetTitle((event->Bars[BarNum].Tops[i].GetChannelName()+string(" CFD")).c_str());
      TopCFDs[i]->SetName(event->Bars[BarNum].Tops[i].GetChannelName().c_str());
    }

    if (event->Bars[BarNum].Tops[i].GetFilter().size()!=0){
      for (int j=0;j<traceSize;j++){
  	temp[j]=event->Bars[BarNum].Tops[i].GetFilter()[j];
      }
      TopFilters[i] = new TGraph(traceSize,x,temp);
      TopFilters[i]->SetTitle((event->Bars[BarNum].Tops[i].GetChannelName()+string(" Filter")).c_str());
      TopFilters[i]->SetName(event->Bars[BarNum].Tops[i].GetChannelName().c_str());
    }
 
  }


  for (int i=0;i<NumObjects;i++){
    for (int j=0;j<traceSize;j++){
      temp[j]=event->TheObjectScintillators[i].GetTrace()[j];
    }
    Objects[i]=new TGraph(traceSize,x,temp);
    Objects[i]->SetTitle("OBJECT");
    Objects[i]->SetName(event->TheObjectScintillators[i].GetChannelName().c_str());
}



  free(temp);
  gStyle->SetMarkerStyle(7);
  gStyle->UseCurrentStyle();
  for (int i=0;i<NumBottoms;i++){
    if (BottomTraces[i]!=NULL){
      c->cd(2*i+1);
      BottomTraces[i]->Draw("ALp");
    }
    if (! traceOnly){
      if (BottomCFDs[i]!=NULL){
	c1->cd(2*i+1);
	BottomCFDs[i]->Draw("ALp");
      }
      if (BottomFilters[i]!=NULL){
	c2->cd(2*i+1);
	BottomFilters[i]->Draw("ALp");
      }
    }
  }
  for (int i=0;i<NumTops;i++){
    if (TopTraces[i]!=NULL){
      c->cd(2*i+2);
      TopTraces[i]->Draw("ALp");
    }
    if (! traceOnly){
      if(TopCFDs[i]!=NULL){
	c1->cd(2*i+2);
	TopCFDs[i]->Draw("ALp");
      }
      if(TopFilters[i]!=NULL){
	c2->cd(2*i+2);
	TopFilters[i]->Draw("ALp");
      }
    }
  }
  
  for (int i=0;i<NumObjects;i++){
    objCanvas->cd(i+1);
    Objects[i]->Draw("ALp");
  }
  
}
