//////////////////////////////////////////////////////////
//
//    Evt2Raw.cc
//    Created       : 2014/05/08 01:11:43
//--------------------------------------------------------
//    Comment : Convert raw data (NSCL DAQ 10.2-104)
//              to uncalibrated ROOT TTree
//--------------------------------------------------------
//
//////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>

#include "TFile.h"
#include "TSystem.h"
#include "TObject.h"
#include "TTree.h"
#include "TStopwatch.h"

#include "S800Event.hh"
#include "DDASEvent.hh"

#include "DAQdefs.h"

#include "Utilities.hh"

int main(int argc, char* argv[])
{
   if (argc != 3) {
      Error("Evt2Raw","Usage: Evt2Raw InputFile OutputFile");
      return 0;
   }
   
   double time_start = get_time();  
   TStopwatch* sw;
   sw = new TStopwatch();
   sw->Start(kTRUE);
   signal(SIGINT,signalhandler);

   // Get the name of the input file from the arguments
   TString InputFile  = TString(argv[1]);

   FileStat_t info;
   if (gSystem->GetPathInfo(InputFile, info) != 0) {
      Error("Evt2Raw", "Input file %s does not exist.", InputFile.Data());
      return 0;
   }
   // Open the input file.
   FILE *infile;
   infile = fopen(InputFile.Data(),"r");
   
   Info("Evt2Raw", "Input file:  %s", InputFile.Data());
   Info("Evt2Raw", "Input file size: %7.2f MB", (float)info.fSize/1024./1024.);

   // Get the name of the output file from the arguments
   TString OutputFile = TString(argv[2]);
   
   // Open the output file.
   TFile *outfile = new TFile(OutputFile,"RECREATE");
   if (outfile->IsZombie()) {
      return 0;
   }

   Info("Evt2Raw", "Output file: %s", OutputFile.Data());

   // Initialize the data structures for the event building.
   // Needs cleanup
   Int_t   buffers = 0;
   int64_t bytes_read = 0;
   size_t  count_read;

   int8_t   buffer08[4096];
   uint16_t buffer16[4096];
   uint32_t buffer32[4096];

   // for S800
   uint16_t evtlength;
   uint16_t *pevent;
   S800* s800 = new S800;

   // for ddaschannel
   uint32_t *body_ptr;
   ddaschannel* dchan = new ddaschannel;

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

   // StateChangeItem
   // const int TITLE_MAXSIZE = 80;
   // uint32_t s_runNumber;
   // uint32_t s_timeOffset;
   // uint32_t s_Timestamp;
   // char     s_title[TITLE_MAXSIZE+1];

   Bool_t s800_hit;
   Bool_t ddas_hit;

   // prepare a tree instead of DecodedEvent
   int nentries;
   TTree* outtree   = new TTree("rawtree","S800 and DDAS raw events");
   S800Event* s800event = new S800Event;
   DDASEvent* ddasevent = new DDASEvent;
   outtree->Branch("s800event", &s800event, 320000);
   outtree->Branch("ddasevent", &ddasevent, 320000);
   outtree->BranchRef();

   // initialize
   nentries    = 0;
   s800event->Clear();
   ddasevent->Reset();

   //Loop over the entirety of the input file.
   while (!feof(infile) && !signal_received) {

      // The header of built ring item: 2 uint32_t's.
      // A 32-bit size of entire item (in bytes)
      count_read  = fread(&size_of_entire_item, sizeof(uint32_t), 1, infile);
      bytes_read += 1 * sizeof(uint32_t);

      // A 32-bit type of item (/usr/opt/nscldaq/10.2-104/include/DataFormat.h)
      count_read  = fread(&type_of_entire_item, sizeof(uint32_t), 1, infile);
      bytes_read += 1 * sizeof(uint32_t);

      // The pointer should be pointing at the beginning of the body of built ring item.
      // Switching according to the type of the entire event.
      // Doing nothing except for physics events.
      switch (type_of_entire_item) {
	 case PHYSICS_EVENT:
	    // bytes_read_in_body is used to check whether all the data in the body is read.
	    bytes_read_in_body = 0;
	    ddas_hit = kFALSE;
	    s800_hit = kFALSE;

	    // The first uint32_t is the total size of body in bytes_
	    count_read  = fread(&total_size_of_body, sizeof(uint32_t), 1, infile);
	    bytes_read += sizeof(uint32_t);
	    bytes_read_in_body += sizeof(uint32_t);

	    while(kTRUE) {
	       // Reading a fragment
	       // 64-bit timestamp
	       count_read  = fread(&timestamp, 
				   sizeof(uint64_t), 1, infile);
	       bytes_read += sizeof(uint64_t);

	       // 32-bit sourceid
	       count_read  = fread(&sourceid, 
				   sizeof(uint32_t), 1, infile);
	       bytes_read += sizeof(uint32_t);

	       // 32-bit fragment payload size (in bytes)
	       count_read  = fread(&fragment_payload_size, 
				   sizeof(uint32_t), 1, infile);
	       bytes_read += sizeof(uint32_t);

	       // 32-bit barrier (See /usr/opt/nscldaq/10.2-104/include/fragment.h for details)
	       // Just read and skip for now
	       count_read  = fread(&barrier, 
				   sizeof(uint32_t), 1, infile);
	       bytes_read += sizeof(uint32_t);

	       // 32-bit total size of ring item (in bytes)
	       count_read  = fread(&size_of_ring_item, 
				   sizeof(uint32_t), 1, infile);
	       bytes_read += sizeof(uint32_t);

	       // 32-bit type of ring item
	       count_read  = fread(&type_of_ring_item, 
				   sizeof(uint32_t), 1, infile);
	       bytes_read += sizeof(uint32_t);

	       // Decode the event differently depending on which type of data is identified in the header.
	       // For each, pass the data from the file into the DecodedEvent to be read.
	       switch (sourceid) {
		  case DDAS_SOURCEID:
		     switch (type_of_ring_item) {
			case PHYSICS_EVENT:
			{
			   // two sizeof(uint32_t)'s need to be subtracted from the size of ring item
			   count       = size_of_ring_item - 2*sizeof(uint32_t);
			   count_read  = fread(buffer32, sizeof(int8_t), count, infile);
			   bytes_read += count_read * sizeof(int8_t);

			   // The pointer should now be pointing at the end of the ring item.
			   // buffer32[0] should have the address of the beginning of the DDAS event body
			   body_ptr = buffer32;

			   //dchan->Reset();
			   dchan = new ddaschannel; // Needs to be careful with push_back with class object...
			   dchan->UnpackChannelData(body_ptr);

			   // Add decoded ddaschannel instance to fDDASEvent
			   ddasevent->AddChannelData(dchan);
			   // (ddasevent->GetData()).emplace_back(dchan); // This may also work...

			   ddas_hit = kTRUE;
			   break;

			}
			default:
			{
			   // In case of non-physics events, just translate the pointer
			   // to the end of this ring item
			   count       = size_of_ring_item - 2*sizeof(uint32_t);
			   count_read  = fread(buffer32, sizeof(int8_t), count, infile);
			   bytes_read += count_read * sizeof(int8_t);
		
			   ddas_hit = kFALSE;
			}
		     }
		     break;
	  
		  case S800_SOURCEID:
		  {
		     switch (type_of_ring_item) {
			case PHYSICS_EVENT:
			{
			   // 16-bit event length (inclusive)
			   count_read = fread(&evtlength, sizeof(uint16_t), 1, infile);
			   if (count_read == 0) {
			      Warning("Evt2Raw","S800 header but no data.");
			      break;
			   }
			   bytes_read += sizeof(uint16_t);
			   evtlength--;

			   // (evtlength-1) 16-bit words for the data
			   count_read = fread(buffer16, sizeof(uint16_t), evtlength, infile);
			   if (count_read == 0) {
			      Warning("Evt2Raw","S800 expected %u words but found none.",evtlength);
			      break;
			   }
			   bytes_read += evtlength*sizeof(uint16_t);

			   // The pointer should now be pointing at the end of the ring item.
			   // buffer16[0] should have the address of the beginning of the S800 data to be analyzed
			   pevent = buffer16;

			   s800->Clear();
			   int error = s800->DecodeS800(pevent,evtlength);
			   if(error){
			      Warning("Evt2Raw", 
				      "An error (%d) occurred in DecodeS800() while processing %s. Continuing...",
				      error,
				      InputFile.Data());
			      continue;
			   }

			   // Add decoded S800 instance to fS800
			   s800event->SetS800(*s800);
			   s800_hit = kTRUE;
			   break;
			}
			default:
			{
			   // In case of non-physics events, just translate the pointer
			   // to the end of this ring item
			   count_read = fread(buffer08, sizeof(int8_t),
					 size_of_ring_item - 2 * sizeof(uint32_t),
					 infile);
			   bytes_read += (size_of_ring_item-2*sizeof(uint32_t))*sizeof(int8_t);
			   s800_hit = kFALSE;
			}
		     }
		  }
		  break;
	  
		  // If source is neither DDAS nor S800, just skip the data
		  default:
		     count_read = fread(buffer08, sizeof(int8_t),
				   size_of_ring_item - 2 * sizeof(uint32_t),
				   infile);
		     bytes_read += (size_of_ring_item-2*sizeof(uint32_t))*sizeof(int8_t);
	       }

	       // Data read in bytes from the beginning of fragments
	       bytes_read_in_body +=     sizeof(uint64_t) // size of the holder for timestamp
		                     +   sizeof(uint32_t) //                        sourceid
		                     +   sizeof(uint32_t) //                        fragment payload size
		                     +   sizeof(uint32_t) //                        barrier
		                     +   fragment_payload_size; // actual fragment payload size
	
	       // check if all the event fragments are processed
	       if (total_size_of_body == bytes_read_in_body) {
		  // populate tree; clear event for the next;
		  outtree->Fill();
		  s800event->Clear();
		  //ddasevent->Reset();
		  for (int i=0;i<ddasevent->GetData().size();i++){
		    delete ddasevent->GetData()[i];
		  }
		  ddasevent->GetData().clear();//Clearing Vector

		  nentries++;

		  break;
	       } else {
		  // Reading the next fragment...
	       }
	    } // Done reading all the fragments.
	    break;
    
	 default:
	    // anything other than PHYSICS_EVENT (state change, scalers, etc.)
	    count       = size_of_entire_item - 2*sizeof(uint32_t);
	    count_read  = fread(buffer08, sizeof(int8_t), count, infile);
	    // The pointer should now be pointing at the beginning of the next built ring item.
	    // Increment the total bytes read.
	    bytes_read += count_read * sizeof(int8_t);
      } // switch (type_of_entire_item)

      // Write the trees out to disk every denom events.
      const Int_t denom = 10000;
      buffers++;
      if(buffers % denom == 0){
	 outtree->AutoSave();
     
	 double time_end = get_time();
	 Progress("Evt2Raw", "%8d buffers %5.0f MB (%6.2f%%) read at %5.2f buffers/sec... %6.2f sec to go\r",
		  buffers, 
		  float(bytes_read/(1024*1024)), 
		  float(bytes_read/(1024*1024)) / float(info.fSize/1024./1024.) * 100.,
		  float(buffers/(time_end-time_start)),
		  ((float)(info.fSize-bytes_read)/(float)bytes_read)*(time_end-time_start)
	    );
      }
   }

   Info("Evt2Raw","Total of %d data buffers (%5.2f MB read)",
	buffers,
	(float)(bytes_read/(1024.*1024.)));

   Info("Evt2Raw","Total of %d raw events written",
	nentries);

   double time_end = get_time();
   Info("Evt2Raw","Decoded %f buffers/sec.",
	(float)(buffers/(time_end - time_start)));

   sw->Stop();
   sw->Print("u");
  
   //Final cleanup and writing of files.
   outtree->Write("",TObject::kOverwrite);
  
   outfile->Close();
   // If outfile cannot be closed because the file size exceeds fgMaxTreeSize, try TTree::SetMaxTreeSize

   return 0;
}
