



void PlotCFD(ddaschannel *theChannel){

  int size = theChannel->GetTrace().size();
  //  cout<<"THE Size "<<size<<endl;
  int *x = (int*)malloc(size*sizeof(int));
  int *y = (int*)malloc(size*sizeof(int));
  // int x[size];
  // int y[size];
  // cout<<"THE SIZE IS "<<size<<endl;
  LendaFilter aFilter;
  vector <UShort_t> trace=theChannel->GetTrace();
  vector <Double_t> CFD  =aFilter.GetNewFirmwareCFD(trace,6,0,3,3);


  for (int i=0;i<CFD.size();i++){
    x[i]=i;
    y[i]=CFD[i];
  }
   
  int slot = theChannel->GetSlotID();
  int chan = theChannel->GetChannelID();

   
  TGraph * theGraph = new TGraph(size,x,y);
  
  
  stringstream ss;
  ss<<"slot "<<slot<<" channel "<<chan;
  //  cout<<ss.str()<<endl;
  theGraph->SetTitle(ss.str().c_str());
  
  theGraph->Draw("AL*");



}

void PlotCFD(TTree* theTree,long entry,int num=0){
  DDASEvent *e = new DDASEvent();
  
  theTree->SetBranchAddress("ddasevent",&e);
  theTree->GetEntry(entry);

  int size = e->GetData().size();

  if (num >= size ){
    cout<<"There are only "<<size<<" trace in this event"<<endl;
    return;
  } else if (num == -1){
    //plot all the traces
    TCanvas * c1 = new TCanvas("c1");
    TCanvas * c2 = new TCanvas("c2");
    TCanvas * c3 = new TCanvas("c3");
    TCanvas * c4 = new TCanvas("c4");
    TCanvas * c5 = new TCanvas("c5");
    
    c1->Divide(1,4);
    c2->Divide(1,4);
    c3->Divide(1,4);
    c4->Divide(1,4);
    c5->Divide(1,4);

    for (int i=0;i<size;i++){
      if (i <=3 ){
	c1->cd(i+1);
      }else if ( i > 3 && i <=7){
	c2->cd(i+1 - 4);
      }else if (i >7 && i<=11){
	c3->cd(i+1 -8);
      }else if (i>11 && i<=15){
	c4->cd(i+1 -12);
      }else if (i >15 && i<=19){
	c5->cd(i+1-16);
      }else{
	cout<<"TOO MANY TRACES"<<endl;
      }
      

      ddaschannel *theChannel = e->GetData()[i];
      PlotCFD(theChannel);
    }

  }else {

  ddaschannel *theChannel = e->GetData()[num];
  
  PlotCFD(theChannel);
  }

}
