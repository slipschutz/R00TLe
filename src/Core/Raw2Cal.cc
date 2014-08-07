//////////////////////////////////////////////////////////
//
//    Raw2Cal.cc
//    Created       : 2014/05/21 14:16:48
//--------------------------------------------------------
//    Comment : Convert uncalibrated ROOT TTree
//              to calibrated/calculated ROOT TTree
//--------------------------------------------------------
//
//////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
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

#include "S800Settings.hh"
#include "S800Calibration.hh"

#include "R00TLeSettings.hh"

#include "Utilities.hh"

int main(int argc, char* argv[]) 
{
   if (argc != 3 && argc !=4) {
      Error("Raw2Cal","Usage: Raw2Cal InputFile OutputFile [RunNumber]");
      return 0;
   }

   // A stopwatch to measure runtime of the program
   TStopwatch* sw;
   sw = new TStopwatch();
   sw->Start(kTRUE);
   signal(SIGINT, signalhandler);


   Int_t RunNumber=-1;
   if (argc == 4 ){ // A RunNumber was given
     RunNumber = atoi(argv[3]);
   }


   // Input file and tree
   TString InputFile  = TString(argv[1]);
   FileStat_t info;
   if (gSystem->GetPathInfo(InputFile, info) != 0) {
      Error("Raw2Cal", "Input file %s does not exist.", InputFile.Data());
      return 1;
   }
   Info("Raw2Cal", "Input file:  %s",InputFile.Data());
   Info("Raw2Cal", "Input file size: %7.2f MB", (float)info.fSize/1024./1024.);

   TFile* infile = new TFile(InputFile);
   TTree* intree   = (TTree*) infile->Get("rawtree");
   if (intree == NULL) {
      Error("Raw2Cal","Cannot find tree rawtree in the input file");
      return 2;
   }
   Long64_t nentries = intree->GetEntries();
   Info("Raw2Cal", "%lld entries in the input file", nentries);

   // S800Event branch
   S800Event* s800event = new S800Event;
   TBranch* s800branch = intree->GetBranch("s800event");
   s800branch->SetAddress(&s800event);
   s800branch->SetAutoDelete(kTRUE);

   // DDASEvent branch
   DDASEvent* ddasevent = new DDASEvent;
   TBranch* ddasbranch = intree->GetBranch("ddasevent");
   ddasbranch->SetAddress(&ddasevent);
   ddasbranch->SetAutoDelete(kTRUE);

   // Output file and tree
   TString OutputFile = TString(argv[2]);
   Info("Raw2Cal", "Output file: %s",OutputFile.Data());
   TFile* outfile = new TFile(OutputFile,"RECREATE");
   if (outfile->IsZombie()) {
      return 3;
   }
   TTree* outtree   = new TTree("caltree","S800 and DDAS calibrated events");

   R00TLeSettings * TheR00TLeSettings = new R00TLeSettings();

   // S800Calc branch
   S800Calc*  s800calc    = new S800Calc;
   outtree->Branch("s800calc",   &s800calc,   320000);

   // LendaEvent branch
   LendaPacker *thePacker = new LendaPacker(TheR00TLeSettings);
   thePacker->SetFilter(6,0,6,0);
   thePacker->SetGates(15,5,15,5);
   thePacker->SetTraceDelay(120);
   thePacker->FindAndSetMapAndCorrectionsFileNames(RunNumber);
   LendaEvent* lendaevent = new LendaEvent;
   outtree->Branch("lendaevent", &lendaevent, 320000);

   outtree->BranchRef();

   // Parameters and settings
   TString prmdirname = gSystem->Getenv("R00TLe_PRM");
   TString prmfilename("Raw2Cal.prm");
   TString prmfilenamefull = prmdirname + "/" + prmfilename;
   std::ifstream prmfile(prmfilenamefull);
   Info("Raw2Cal", "Reading a parameter file %s", prmfilenamefull.Data());
   if (prmfile.fail()) {
      Error("Raw2Cal", "No such prm file %s", prmfilenamefull.Data());
      return 4;
   }

   S800Settings    *set = new S800Settings(prmfilenamefull.Data());
   S800Calibration *cal = new S800Calibration(set);

   //Loop over the entirety of the input file.
   Int_t nbytes_read = 0;
   Int_t status;
   for (Long64_t jentry = 0; jentry < nentries; jentry++) {
      if (signal_received) {
	 break;
      }

      // initialize
      s800calc  ->Clear();
      lendaevent->Clear();

      // read all branches and store total number of bytes read to status
      status       = intree->GetEntry(jentry); // return value is the number of bytes read from the input buffer.
      nbytes_read += status;

      if (status == -1) {
	 Error("Raw2Cal",
	       "Error: Entry #%lld from %s in %s does not exist.",
	       jentry, intree->GetName(), infile->GetName());
	 return 5;
      }
      else if (status == 0) {
	 Error("Raw2Cal",
	       "Error: An I/O error occurred in attempting to read entry #%lld from %s in %s",
	       jentry, intree->GetName(), infile->GetName());
	 return 6;
      }

      // Build the calibrated S800 object from data in the uncalibrated S800 object
      // cal->S800Calculate(s800event->GetS800(),s800calc); // this should work
      s800calc->ApplyCalibration(s800event->GetS800(),cal); // this should work as well

      thePacker->MakeLendaEvent(lendaevent, ddasevent,jentry);
      lendaevent->Finalize();

      // write the processed datum to tree
      outtree->Fill();

      loadBar(jentry, nentries, 1000, 50);
   }

   outtree->Write("",TObject::kOverwrite);
   TheR00TLeSettings->Write();
   delete intree;
   delete outtree;

   sw->Stop();
   sw->Print("u");

   outfile->Close();
   return 0;
}
