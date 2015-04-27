


void PrintTrace(TTree* theTree,long entry,int num=0){

  DDASEvent *e = new DDASEvent();
  
  theTree->SetBranchAddress("ddasevent",&e);
  theTree->GetEntry(entry);

  int size = e->GetData().size();


  if (num >= size ){
    cout<<"There are only "<<size<<" trace in this event"<<endl;
    return;
  } else {

    vector <UShort_t> trace = e->GetData()[num]->GetTrace();
    
    for (int i=0;i<trace.size();i++){
      cout<<trace[i]<<",";
    }
    cout<<endl;
  }


  
}
