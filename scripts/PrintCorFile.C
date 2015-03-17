



#include <map>

void PrintCorFile(Int_t runNumber){





  Int_t num = gDirectory->GetListOfKeys()->GetSize();









  TString user =gSystem->Getenv("R00TLe_User");
  TString install =gSystem->Getenv("R00TLeInstall");





  ifstream InCorrectionsFile;
  stringstream ss1;

  ss1<<install<<"/prm/Corrections"<<runNumber<<".txt";

  InCorrectionsFile.open(ss1.str().c_str());
  if (!InCorrectionsFile.is_open()){
    cout<<"File not found"<<endl;
    return;
  }


  std::map<TString,Double_t> timingOffsetsFromFile;
  TString NameInFile;
  Double_t trash,T_Offset;
  while (true){
    InCorrectionsFile>>NameInFile>>trash>>trash>>T_Offset;
    if (InCorrectionsFile.eof()){
      break;
    }

    timingOffsetsFromFile[NameInFile]=T_Offset;
  }






  TF1 * aFunc = new TF1("aFunc","pol1");
  
  aFunc->SetParLimits(0,-20,20);
  aFunc->SetParLimits(1,0,2);

  for (int i=0;i<num;i++){
    TString name = gDirectory->GetListOfKeys()->At(i)->GetName();
    TGraphErrors* graph = (TGraphErrors*)gDirectory->Get(name);

    if (name[0]== 'S'||name[0]=='N'){
      TFitResultPtr result = graph->Fit("aFunc","QS");
      Int_t status = result;

      if (status == 0){
	
	printf("%s  %8.4lf  %8.4lf  %8.4lf\n",name.Data(),result->Value(1),result->Value(0),timingOffsetsFromFile[name]);
	//	cout<<name<<"  "<<result->Value(1)<<"  "<<result->Value(0)<<"  "<<timingOffsetsFromFile[name]<<endl;
      }
    }
    
  }


}


