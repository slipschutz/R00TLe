
#include <vector>
#include "LendaEvent.hh"
#include "TGraph.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TDirectory.h"
void GraphObjs(Long64_t entry=0,int num=8,int BarNum=0,bool traceOnly=false){


  LendaEvent * event = new LendaEvent();
  
  TTree* tree =(TTree*)gDirectory->Get("caltree");
  
  tree->SetBranchAddress("lendaevent",&event);
  

  tree->GetEntry(entry);

  event->Dump();
  Int_t NumObjects = num;// event->NumObjectScintillators;
  if (NumObjects == 0){
    cout<<"NO OBJECTS"<<endl;
    return ;
  }
  TCanvas * objCanvas;
  if (NumObjects !=0){
     objCanvas= new TCanvas("objects");

    objCanvas->Divide(1,NumObjects);
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

  
  for (int i=0;i<NumObjects;i++){
    for (int j=0;j<traceSize;j++){
      temp[j]=event->TheObjectScintillators[i].GetTrace()[j];
    }
    Objects[i]=new TGraph(traceSize,x,temp);
    Objects[i]->SetTitle(event->TheObjectScintillators[i].GetChannelName().c_str());
    Objects[i]->SetName(event->TheObjectScintillators[i].GetChannelName().c_str());


    for (int j=0;j<traceSize;j++){
      temp[j]=event->TheObjectScintillators[i].GetTrace()[j];
    }
    Objects[i]=new TGraph(traceSize,x,temp);
    Objects[i]->SetTitle(event->TheObjectScintillators[i].GetChannelName().c_str());
    Objects[i]->SetName(event->TheObjectScintillators[i].GetChannelName().c_str());

}
  
  

  
  for (int i=0;i<NumObjects;i++){
    objCanvas->cd(i+1);
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
