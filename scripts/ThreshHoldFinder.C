
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TFitResult.h>

#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "LendaEvent.hh"

using namespace std;

Double_t FindCoumptonEdge(TH1F*h,int start,int endd){

  int  binStart = h->FindBin(start);
  int binEnd = h->FindBin(endd);


  Double_t max =0;

  //find the max
  int maxBin=-1;
  for(int i=binStart ; i<binEnd ;i++){
    if ( h->GetBinContent(i) > max)
      {
	max = h->GetBinContent(i);
	maxBin=i;
      }
  }

  max= (1/3.0)*(max+h->GetBinContent(maxBin-1)+
		h->GetBinContent(maxBin+1));


  Double_t twoThirdmax = (2.0/3.0)*max;

  Double_t answer=-1;
  for (int i=maxBin;i<binEnd;i++){


    if ( h->GetBinContent(i) <= twoThirdmax ){
      answer = h->GetBinCenter(i);
      break;
    }

  }
  return answer;
}

void ThreshHoldFinder(){
  
  vector <string> filesForCalibration;
  filesForCalibration.resize(3);
  
  filesForCalibration[0]="/events/e10003/rootfiles/run-0369-00.root";
  filesForCalibration[1]="/events/e10003/rootfiles/run-0373-00.root";
  filesForCalibration[2]="/events/e10003/rootfiles/run-0377-00.root";

  // files should go in the following order
  // Am Cs NA
  //
  map <int,string> num2source;
  num2source[0]="AM";
  num2source[1]="CS";
  num2source[2]="NA";
  
  int NumBars=52;
  vector <TH1F*> AmSpectrums;
  vector <TH1F*> CSSpectrums;
  vector <TH1F*> NASpectrums;
  
  map <int, vector<TH1F*> > num2Spectrums;
  num2Spectrums[0]=AmSpectrums;
  num2Spectrums[1]=CSSpectrums;
  num2Spectrums[2]=NASpectrums;
  
  num2Spectrums[0].resize(NumBars);
  num2Spectrums[1].resize(NumBars);
  num2Spectrums[2].resize(NumBars);


  TTree * caltree;  //The current tree
  TFile * currentFile;
  for (int fileNum=0;fileNum<filesForCalibration.size();fileNum++){
    currentFile = new TFile(filesForCalibration[fileNum].c_str());
    
    if (gDirectory->Get("caltree") == NULL){
      cout<<"No caltree in current directory"<<endl;
      return;
    }
    
    stringstream namestream;
    for (int i=0;i<NumBars;i++){
      namestream.str("");
      namestream<<num2source[fileNum]<<"BarId"<<i;
      num2Spectrums[fileNum][i] = new TH1F(namestream.str().c_str(),namestream.str().c_str(),2000,0,50000);
      num2Spectrums[fileNum][i]->SetDirectory(0);
    }

    caltree = (TTree*)gDirectory->Get("caltree");
    LendaEvent * event = new LendaEvent();
    caltree->SetBranchAddress("lendaevent",&event);
    
    cout<<"Dumping the Spectrums for "<<num2source[fileNum]<<endl;
    Long64_t numEntries = caltree->GetEntries();
    cout<<"There are "<<numEntries<<" in this tree"<<endl;
    
    for (Long64_t jentry=0;jentry<numEntries;jentry++){
      caltree->GetEntry(jentry);
      
      int numBars = event->NumBars;
      if (numBars!=0){
	for (int j=0;j<numBars;j++){
	  if ( event->Bars[j].SimpleEventBit && event->Bars[j].Tops[0].GetEnergy()>0 && event->Bars[j].Bottoms[0].GetEnergy()>0){

	    int BarId = event->Bars[j].BarId;
	    num2Spectrums[fileNum][BarId]->Fill(event->Bars[j].Bottoms[0].GetEnergy());
	  }
	}
      }

      
      if (jentry % 1000 == 0){
	cout<<"On Entry "<<jentry<<endl;
      }
      
    }
    
    
    currentFile->Close();
    delete currentFile;
  }

  vector <TGraphErrors*> theThreshPlots;


  Double_t Na1 = 341;
  Double_t Na2 = 1062;
  
  Double_t Am1 = 26.3;
  Double_t Am2 = 59.5;
  
  Double_t Cs1 = 33;
  Double_t Cs2 = 478;
  
  Double_t y[3];
  y[0]= Na1;
  y[1]= Na2;
  // y[2]= Am1;
  // y[3]= Am2;
  y[2]= Cs2;
  // y[5]= Cs2;

  TFile out("out.root","recreate");
  for (int i=0;i<NumBars;i++){
    Double_t x[3];
    Double_t ex[3];
    for (int j=0;j<filesForCalibration.size();j++){
      num2Spectrums[j][i]->Write();
      
      if (num2source[j] == "NA"){//These are NA-22
	x[0] = FindCoumptonEdge(num2Spectrums[j][i],4000,9000);
	x[1] = FindCoumptonEdge(num2Spectrums[j][i],16000,25000);
	ex[0]= 0.05*x[0];
	ex[1]= 0.05*x[1];
      } else if (num2source[j]=="CS"){
	x[2] = FindCoumptonEdge(num2Spectrums[j][i],8000,12000);
	ex[2]=0.05*x[2];
      }else if (num2source[j]=="AM"){
	// x[4]=0;
	// x[5]=0;
      } 

    }
    theThreshPlots.push_back(new TGraphErrors(3,x,y,ex,0));
    theThreshPlots[i]->SetName(TString("Thresh")+num2Spectrums[0][i]->GetName());
    theThreshPlots[i]->SetTitle(TString("Thresh")+num2Spectrums[0][i]->GetName());
    
    TFitResultPtr result = theThreshPlots[i]->Fit("pol1","SQ");
    Int_t status =result;
    if (status == 0){
      cout<<theThreshPlots[i]->GetName()<<endl;
      cout<<result->Value(0)<<endl;
      cout<<result->Value(1)<<endl;
    }
    

    theThreshPlots[i]->Write();
  }


  out.Close();



}
