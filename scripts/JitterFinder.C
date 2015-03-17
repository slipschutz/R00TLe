

#ifndef __MAP__STL
#define __MAP__STL
#include <map>
#endif


void JitterFinder(int Num,int NumOfFiles=1,bool OverWrite=false){

  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");
  TString rootfilesdirname = install+"/users/"+user+"/histograms";

  std::map<string,double> theMapOfFirstCorrections;

  for (int I=0;I<NumOfFiles;I++){
    stringstream ss;
    ss<<"/HistogramsFromRun"<<Num+I<<".root";
    
    TFile *theFile = new TFile(rootfilesdirname+ss.str().c_str());
    if (!theFile->IsZombie()){

      R00TLeSettings * settings = (R00TLeSettings*)theFile->Get("Settings0");
    
      int numBars =settings->GetNumBars();
    
      ofstream myFile;
      if (OverWrite){ //If script should overwrite the prm file for this run in the prm directory
	stringstream ss;
	TString user =gSystem->Getenv("R00TLe_User");
	TString install =gSystem->Getenv("R00TLeInstall");
	TString prm = install + "/prm/";
	ss<<prm<<"Corrections"<<Num+I<<".txt";
	myFile.open(ss.str().c_str());
      }	

      int precLevel=5;
      for (int i=0;i<numBars;i++){
	TString BarName = settings->GetBarName(i);
    
	TH1F * TopChan = (TH1F*)theFile->Get(BarName+"_CutTopTOF");
	TH1F * BottomChan = (TH1F*)theFile->Get(BarName+"_CutBottomTOF");
      
	if (TopChan->GetEntries()>0 && BottomChan->GetEntries()>0){
    
	  Double_t topBinCenter =TopChan->GetBinCenter(TopChan->GetMaximumBin());
	  Double_t BottomBinCenter =BottomChan->GetBinCenter(BottomChan->GetMaximumBin());

	  TF1 * topFunc = new TF1("topFunc","gaus",topBinCenter-0.5,topBinCenter+0.5);
	  TF1 * bottomFunc = new TF1("bottomFunc","gaus",BottomBinCenter-0.5,BottomBinCenter+0.5);

	  TFitResultPtr resultTop=TopChan->Fit("topFunc","QSR");
	  TFitResultPtr resultBottom=BottomChan->Fit("bottomFunc","QSR");

	  Int_t status1 = resultTop;
	  Int_t status2 = resultBottom;
    
      
	  if (status1==0 && status2==0){
	    //      double answer = TMath::Nint(resultBottom->Value(1)-resultTop->Value(1));
	    double TopAnswer = resultTop->Value(1);
	    double BottomAnswer = resultBottom->Value(1);

	    if (I ==0){//This is the first file
	      string temp = BarName +"T";
	      theMapOfFirstCorrections[temp]=TopAnswer;
	      temp = BarName+"B";
	      theMapOfFirstCorrections[temp]=BottomAnswer;
	    } else { // it is not the first file
	      string temp = BarName+"T";
	      double firstVal=theMapOfFirstCorrections[temp];
	      double diffTop = TMath::Nint(firstVal-TopAnswer);
	      TopAnswer=firstVal-diffTop;
	      temp = BarName+"B";
	      firstVal = theMapOfFirstCorrections[temp];
	      double diffBottom = TMath::Nint(firstVal-BottomAnswer);
	      BottomAnswer = firstVal-diffBottom;	    

	    }

	    cout<<BarName+"T"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<TopAnswer<<endl;;
	    cout<<BarName+"B"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<BottomAnswer<<endl;;

	    if (OverWrite){
	      myFile<<BarName+"T"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<TopAnswer<<endl;;
	      myFile<<BarName+"B"<<"  1  0  "<<fixed<<setw(6)<<setprecision(precLevel)<<BottomAnswer<<endl;;
	    }//end if(overWrite)
	  
	  }//end if(status1 && status2)

	}//end if (TopEntries>0 &&BottomEntrie>0)
      }//End for(i :Numbars)
      cout<<"End File "<<Num+I<<endl;
    } else {
      cout<<"File "<<Num+I<<" not found "<<endl;
    }
  }//end for (I: numFiles)
}//end script
