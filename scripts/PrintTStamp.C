


void PrintTStamp(TTree* theTree,long entry,int num=0){

  DDASEvent *e = new DDASEvent();
  
  theTree->SetBranchAddress("ddasevent",&e);
  theTree->GetEntry(entry);

  int size = e->GetData().size();


  if (num >= size ){
    cout<<"There are only "<<size<<" trace in this event"<<endl;
    return;
  } else {
    
    printf("%20.10lf\n",e->GetData()[num]->GetTime());
  }


  
}
