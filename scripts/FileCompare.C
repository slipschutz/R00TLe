

void DumpEvent(LendaEvent* e){

  cout<<"N: "<<e->N<<endl;
  cout<<"Numbars: "<<e->NumBars<<endl;

  for (int i=0;i<e->NumBars;i++){
    cout<<"Bar "<<i<<endl;
    int numTops = e->Bars[i].NumTops;
    int numBottoms = e->Bars[i].NumBottoms;
    cout<<"Has "<<numTops <<" top channels"<<endl;
    cout<<"Has "<<numBottoms<<" bottom channels"<<endl;
    for (int t=0;t<numTops;t++){
      cout<<"Top "<<t<<" time "<<e->Bars[i].Tops[t].GetTime()<<endl;
      
      
    }

    for (int b=0;b<numBottoms;b++){
      cout<<"Bottom "<<b<<" time "<<e->Bars[i].Bottoms[b].GetTime()<<endl;

    }

  }
  


}



void DumpEvents(LendaEvent* Event1,LendaEvent* Event2){

  cout<<"Event1:"<<endl;
  DumpEvent(Event1);
  cout<<"\n\nEvent2"<<endl;
  DumpEvent(Event2);

}



void FileCompare(TString file1Name,TString file2Name){


  TFile * file1 = new TFile(file1Name);

  TFile * file2 = new TFile(file2Name);

  TTree * file1Tree = (TTree*)file1->Get("caltree");

  TTree * file2Tree = (TTree*)file2->Get("caltree");


  Long64_t numEntries1 = file1Tree->GetEntries();
  Long64_t numEntries2 = file2Tree->GetEntries();

  if (numEntries1 != numEntries2){
    cout<<"BROKEN"<<endl;
    cout<<"File1 "<<numEntries1<<" File2 "<<numEntries2<<endl;
    return;
  }else {
    cout<<"\n\n\n\n Files have same number of events "<<numEntries1<<endl;
  }

  LendaEvent* Event1 = new LendaEvent();
  LendaEvent* Event2 = new LendaEvent();

  file1Tree->SetBranchAddress("lendaevent",&Event1);
  file2Tree->SetBranchAddress("lendaevent",&Event2);
    

  for (Long64_t i=0;i<numEntries1;i++){
    file1Tree->GetEntry(i);
    file2Tree->GetEntry(i);
    
    if (Event1->N != Event2->N){
      DumpEvents(Event1,Event2);
    }

    if ( i %1000==0){
      cout<<"On "<<i<<endl;
    }
  }
  
  








}
