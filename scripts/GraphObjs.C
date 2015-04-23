
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
  std::vector <TGraph*> Objects;
  Objects.resize(NumObjects,NULL);
  

  int traceSize = event->TheObjectScintillators[0].GetTrace().size();
  

  cout<<"TRACE SIZE IS "<<traceSize<<endl;
  Double_t *x = (Double_t*)malloc(traceSize*sizeof(Double_t));
  
  for (int i=0;i<traceSize;i++){
    x[i]=i;
  }


  
  Double_t * temp =(Double_t*)malloc(traceSize*sizeof(Double_t));

  
  for (int i=start;i<start+num;i++){
    for (int j=0;j<traceSize;j++){
      temp[j]=event->TheObjectScintillators[i].GetTrace()[j];
    }
    Objects[i]=new TGraph(traceSize,x,temp);
    Objects[i]->SetTitle(event->TheObjectScintillators[i].GetChannelName().c_str());
    Objects[i]->SetName(event->TheObjectScintillators[i].GetChannelName().c_str());

    // for (int j=0;j<traceSize;j++){
    //   temp[j]=event->TheObjectScintillators[i].GetTrace()[j];
    // }
    // Objects[i]=new TGraph(traceSize,x,temp);
    // Objects[i]->SetTitle(event->TheObjectScintillators[i].GetChannelName().c_str());
    // Objects[i]->SetName(event->TheObjectScintillators[i].GetChannelName().c_str());

}
  
  

  
  for (int i=start;i<start+num;i++){
    objCanvas->cd(i+1-start);
    Objects[i]->Draw("AL*");
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
