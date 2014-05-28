



void PlotTrace(ddaschannel *theChannel){
  
  int size = theChannel->GetTrace().size();
  cout<<"THE Size "<<size<<endl;
  int *x = (int*)malloc(size*sizeof(int));
  int *y = (int*)malloc(size*sizeof(int));
  // int x[size];
  // int y[size];
  // cout<<"THE SIZE IS "<<size<<endl;
  for (int i=0;i<size;i++){
     x[i]=i;
     y[i]=theChannel->GetTrace()[i];
   }
   
   
   TGraph * theGraph = new TGraph(size,x,y);

   theGraph->Draw("AL");



}

void PlotTrace(TTree* theTree,long entry){
  DDASEvent *e = new DDASEvent();
  
  theTree->SetBranchAddress("ddasevent",&e);
  theTree->GetEntry(entry);

  ddaschannel *theChannel = e->GetData()[0];
  
  PlotTrace(theChannel);


}
