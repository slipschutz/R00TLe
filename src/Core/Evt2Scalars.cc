//////////////////////////////////////////////////////////
//
//    Evt2Cal.cc
//    Created       : 2014/05/22 16:53:56
//--------------------------------------------------------
//    Comment : Convert raw data (NSCL DAQ 10.2-104)
//              to calibrated/calculated ROOT TTree
//--------------------------------------------------------
//
//////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>

#include "TFile.h"
#include "TSystem.h"
#include "TObject.h"
#include "TTree.h"
#include "TStopwatch.h"

#include "S800Event.hh"
#include "DDASEvent.hh"
#include "LendaPacker.hh"
#include "R00TLeSettings.hh"

#include "S800Settings.hh"
#include "S800Calibration.hh"

#include "DAQdefs.h"
#include "S800Scalers.hh"
#include "LendaScalers.hh"

#include "Utilities.hh"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 3 ) {
    Error("Evt2Scalars","Usage: Evt2Scalars InputFile OutputFile");
    return 0;
  }
   
  double time_start = get_time();  
  TStopwatch* sw;
  sw = new TStopwatch();
  sw->Start(kTRUE);
  signal(SIGINT,signalhandler);

  Int_t RunNumber=-1;
  if (argc == 4 ){ // A RunNumber was given
    RunNumber = atoi(argv[3]);
  }

  // Get the name of the input file from the arguments
  TString InputFile  = TString(argv[1]);

  FileStat_t info;
  if (gSystem->GetPathInfo(InputFile, info) != 0) {
    Error("Evt2Scalars", "Input file %s does not exist.", InputFile.Data());
    return 0;
  }

  // Open the input file.
  FILE *infile;
  infile = fopen(InputFile.Data(),"r");
   
  Info("Evt2Scalars", "Input file:  %s", InputFile.Data());
  Info("Evt2Scalars", "Input file size: %7.2f MB", (float)info.fSize/1024./1024.);

  // Get the name of the output file from the arguments
  TString OutputFile = TString(argv[2]);
   
  //  Open the output file.
  TFile *outfile = new TFile(OutputFile,"RECREATE");
  if (outfile->IsZombie()) {
    return 0;
  }

  Info("Evt2Scalars", "Output file: %s", OutputFile.Data());

  // Initialize the data structures for the event building.
  // Needs cleanup
  Int_t   buffers = 0;
  int64_t bytes_read = 0;
  size_t  count_read;

  Int_t bufferSize=16384;

  int8_t   buffer08[bufferSize];
  uint16_t buffer16[bufferSize];
  uint32_t buffer32[bufferSize];

  // for S800
  uint16_t evtlength;
  uint16_t *pevent;


  // for NSCLDAQ-format data
  size_t count;
  uint32_t size_of_entire_item;
  uint32_t type_of_entire_item;
  uint32_t total_size_of_body;
  uint32_t bytes_read_in_body;
  uint64_t timestamp;
  uint32_t sourceid;
  uint32_t fragment_payload_size;
  uint32_t barrier;
  uint32_t size_of_ring_item;
  uint32_t type_of_ring_item;

  uint64_t s800_tstamp;
  // uint64_t ddas_tstamp;

  LendaScalers * lendascaler = new LendaScalers();
  S800Scalers * s800scaler = new S800Scalers();

  // prepare a tree for out put
  int nentries;
  TTree* outtreeLenda   = new TTree("lendascalers","Lenda Scalars");
  TTree* outtreeS800   = new TTree("s800scalers","S800 Scalars");
  outtreeS800->Branch("s800scalers", &s800scaler, 320000);
  outtreeLenda->Branch("lendascalers", &lendascaler, 320000);



  // // S800Calc branch
  // S800Calc*  s800calc    = new S800Calc;
  // outtree->Branch("s800calc",   &s800calc,   320000);

  // // LendaEvent branch
  // LendaPacker *thePacker = new LendaPacker(TheR00TLeSettings);
  // thePacker->SetFilter(6,0,6,0);
  // thePacker->SetGates(15,5,15,5);
  // thePacker->SetTraceDelay(120);
  // thePacker->FindAndSetMapAndCorrectionsFileNames(RunNumber);

  // LendaEvent* lendaevent = new LendaEvent;
  // outtree->Branch("lendaevent", &lendaevent, 320000);

  // outtree->BranchRef();

  // Parameters and settings
  // TString prmdirname = gSystem->Getenv("R00TLe_PRM");
  // TString prmfilename("Evt2Cal.prm");
  // TString prmfilenamefull = prmdirname + "/" + prmfilename;
  // std::ifstream prmfile(prmfilenamefull);
  // Info("Evt2Cal", "Reading a parameter file %s", prmfilenamefull.Data());
  // if (prmfile.fail()) {
  //   Error("Evt2Cal", "No such prm file %s", prmfilenamefull.Data());
  //   return 4;
  // }

  // S800Settings    *set = new S800Settings(prmfilenamefull.Data());
  // S800Calibration *cal = new S800Calibration(set);

  // initialize
  nentries    = 0;


  //Loop over the entirety of the input file.
  while (!signal_received) {
    //first try and read something from the file
    //if it is at the end then you must try and read 
    //from the file inorder for the eof bit to true
     
    // The header of built ring item: 2 uint32_t's.
    // A 32-bit size of entire item (in bytes)
    count_read  = fread(&size_of_entire_item, sizeof(uint32_t), 1, infile);
    if ( feof(infile) ){ // if the end of tile bit has been set
      if ( bytes_read != info.fSize){
	Error("Evt2Scalars","Total number of bytes read %ld not equal to file size %lld at the end of file parsing",bytes_read,info.fSize);
      }
      break; //end main unpacking loop
    }
    bytes_read += 1 * sizeof(uint32_t);
    

  for (int i=0;i<bufferSize;i++){
    buffer08[i]=0;
    buffer16[i]=0;
    buffer32[i]=0;
  }

    // A 32-bit type of item (/usr/opt/nscldaq/10.2-104/include/DataFormat.h)
    count_read  = fread(&type_of_entire_item, sizeof(uint32_t), 1, infile);
    bytes_read += 1 * sizeof(uint32_t);

    if(type_of_entire_item == INCREMENTAL_SCALERS) {
      //Load the entire item into a buffer then send it to RingItemFactory to make scalars object
      count       = (size_of_entire_item - 2*sizeof(uint32_t))/4; //4bytes per 32-bit integer
      count_read  = fread(buffer32, sizeof(uint32_t), count, infile);
      bytes_read += count_read * sizeof(uint32_t);

      pScalerItemNoHeader DDASsclrs;	
      pScalerItemNoHeader s800sclrs;

      if(size_of_entire_item>500){//is a DDAS
	DDASsclrs = reinterpret_cast<pScalerItemNoHeader>(buffer32);	
	lendascaler->SetScalerItem(DDASsclrs);
	
	outtreeLenda->Fill();
	lendascaler->Reset();


      } else if (size_of_entire_item<500){ //is a s800 

	s800sclrs = reinterpret_cast<pScalerItemNoHeader>(buffer32);	
	s800scaler->SetScalerItem(s800sclrs);


	outtreeS800->Fill();
	s800scaler->Reset();
      }
      

    }else if (type_of_entire_item==TIMESTAMPED_NONINCR_SCALERS){
      cout<<"HELLO"<<endl;
      exit(1);
    }else {
      // anything other then a scalar just skip in the file
      count       = size_of_entire_item - 2*sizeof(uint32_t);
      count_read  = fread(buffer08, sizeof(int8_t), count, infile);
      // The pointer should now be pointing at the beginning of the next built ring item.
      // Increment the total bytes read.
      bytes_read += count_read * sizeof(int8_t);
    }

    // Write the trees out to disk every denom events.
    const Int_t denom = 10000;
    buffers++;
    if(buffers % denom == 0){
      //      outtree->AutoSave();
     
      double time_end = get_time();
      Progress("Evt2Scalars", "%8d buffers %5.0f MB (%6.2f%%) read at %5.2f buffers/sec... %6.2f sec to go\r",
	       buffers, 
	       float(bytes_read/(1024*1024)), 
	       float(bytes_read/(1024*1024)) / float(info.fSize/1024./1024.) * 100.,
	       float(buffers/(time_end-time_start)),
	       ((float)(info.fSize-bytes_read)/(float)bytes_read)*(time_end-time_start)
	       );
    }//END if(buffers%denom==0)
  }//END MAIN LOOP

  Info("Evt2Scalars","Total of %d data buffers (%5.2f MB read)",
       buffers,
       (float)(bytes_read/(1024.*1024.)));

  Info("Evt2Scalars","Total of %d raw events written",
       nentries);

  double time_end = get_time();
  Info("Evt2Scalars","Decoded %f buffers/sec.",
       (float)(buffers/(time_end - time_start)));

  sw->Stop();
  sw->Print("u");
  
  //Final cleanup and writing of files.
  outtreeLenda->Write("",TObject::kOverwrite);
  outtreeS800->Write("",TObject::kOverwrite);
  // TheR00TLeSettings->Write();
  outfile->Close();
  // If outfile cannot be closed because the file size exceeds fgMaxTreeSize, try TTree::SetMaxTreeSize

  return 0;


}
