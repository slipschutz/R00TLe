#include <vector>
#include "TFile.h"
/*
random functions for the builder

 */


struct Sl_Event {
  //4~  ddaschannel *dchan;
  ddaschannel dchan2;
  
  //  Int_t channel;
  // vector <UShort_t> trace;
  Long64_t jentry;
  // Double_t time;
  Double_t softTime;
  // Double_t timelow;
  // Double_t timehigh;
  // Double_t timecfd;
  //  Double_t energy;
};



// Bool_t checkChannels(vector <Sl_Event>&EventsInWindow,
// 		     map <Long64_t,bool>&mapOfUsedEntries){

//   cout <<"num events "<<EventsInWindow.size()<<endl;
//   vector < vector <bool> > ch;
//   for (int i=0;i<16;i++){
//     vector <bool> temp;
//     ch.push_back(temp);
//   }


  
//   map <int,Long64_t> entries;
  
//   bool duplicateChannelsInWindow=false;
//   for (int i=0;i<(int)EventsInWindow.size();i++){
//     ch[EventsInWindow[i].dchan2.chanid].push_back(true);
//     entries[i]=EventsInWindow[i].jentry;
    
//     if (ch[EventsInWindow[i].dchan2.chanid].size() >1)
//       duplicateChannelsInWindow=true;
//   }

//   for (int i=0;i<16;i++)
//     cout<<"i is "<<i<<" "<<ch[i].size()<<endl;

//   if (duplicateChannelsInWindow)
//     return false;

//   cout<<"the conditions "<<endl;
//   cout<<(ch[0].size()==1 && ch[1].size()==1)<<endl;
//   cout<<(ch[2].size()==1 && ch[3].size()==1)<<endl;
  
//   cout<<"The entries "<<endl;
//   for (int i=0;i<(int)entries.size();i++)
//     cout<<"entry i "<<entries[i]<<endl;
  

//   //allowed combinations 
//   if ( (ch[0].size()==1 && ch[1].size()==1)){
//     //currently bar11

//   } else if (ch[2].size()==1 && ch[3].size()==1){
//     //curently bar 23
    
//   } else {
//     return false;
//   }
//   return false;
// }

// void pushRollingWindow(vector <Sl_Event> &previousEvents,ddaschannel *dchan,Long64_t jentry){

// 	Sl_Event e;
// 	//	e.dchan =dchan;
// 	e.jentry = jentry;
// 	previousEvents.push_back(e);

// }


void UnPackIt(TTree* inT){
  TFile * fout = new TFile("upPack.root","recreate");

  DDASEvent *inEvent = new DDASEvent();
  ddaschannel * Event= new ddaschannel();

  inT->SetBranchAddress("ddasevent",&inEvent);

  TTree * outT = new TTree("dchan","time ordered ddas channels");

  outT->Branch("dchan",&Event);
  

  Long64_t maxEntry = (Long64_t)inT->GetEntries();
  cout<<"Num entries is "<<maxEntry<<endl;


  for (Long64_t jentry=0;jentry<maxEntry;jentry++){
    inT->GetEntry(jentry);///Get the DDASEvent
    int size = inEvent->GetData().size();
    for ( int i=0;i<size;i++){
      
      Event= (inEvent->GetData()[i]);
      outT->Fill();
    }
    
    if (jentry %1000==0 ){
      cout<<"On "<<jentry<<endl;
    }

  }
  outT->Write();
  fout->Close();
  
}


void packEvent(LendaEvent *Event,vector <Sl_Event> inEvents,
	       LendaPacker * thePacker){
  //sort the events by channel
  
  vector <Sl_Event*> events2(16,NULL);
  vector <Sl_Event*> events;
  bool test=false;
  for (int i=0;i<(int)inEvents.size();i++){
    if (events2[inEvents[i].dchan2.chanid] == NULL )
      events2[inEvents[i].dchan2.chanid]=&inEvents[i];
    else {
      events2.insert(events2.begin()+inEvents[i].dchan2.chanid,&inEvents[i]);
      // cout<<"CRAP"<<endl;
      // test=true;
    }
  }
  
  for (int i=0;i<(int)events2.size();i++){
    if (events2[i] !=NULL )
      events.push_back(events2[i]);
  }
  if (test){
    cout<<"\n\n\n";
    cout<<"inEvents "<<endl;
    for (int i=0;i<inEvents.size();i++){
      cout<<inEvents[i].dchan2.chanid<<endl;
    }
    cout<<"events: "<<endl;
    for (int i=0;i<events.size();i++)
      cout<<events[i]->dchan2.chanid<<endl;
    int t ;cin>>t;
  }


  // LendaPacker thePacker;
  // thePacker.SetFilter(inMan.FL,inMan.FG,inMan.d,inMan.w);
  // thePacker.SetGates(inMan.long_gate,inMan.short_gate,inMan.long_gate2,inMan.short_gate2);
  // thePacker.lean=inMan.lean;
  // thePacker.SetTraceDelay(inMan.traceDelay);
  
  for (int i=0;i<(int)events.size();++i){
    thePacker->SetDDASChannel(&events[i]->dchan2);
    thePacker->SetJEntry(events[i]->jentry);   
    thePacker->CalcAll();
    thePacker->PackEvent(Event);
  }
  
   


}



void PackEventClean(LendaEvent *Event,DDASEvent *theDDASEvent,
		    LendaPacker * thePacker,Long64_t jentry){
  ////////////////////////////////////////////////////////////////////
  //////////////Get the ddaschannels form the DDASEVent///////////////
  vector <ddaschannel*> theDDASChannels = theDDASEvent->GetData();
  //cout<<"THE SIZE OF DDAS "<<theDDASChannels.size()<<endl;

  //sort the events by channel using a Merge sort
  //Only expecting on module right now
  vector <ddaschannel*> eventsTemp(16,NULL); // temp Vector to hold events
  vector <ddaschannel*> eventsSorted; //the final sorted vector
  bool test=false;
  for (int i=0;i<(int)theDDASChannels.size();i++){
    if (eventsTemp[theDDASChannels[i]->chanid] == NULL )
      eventsTemp[theDDASChannels[i]->chanid]=theDDASChannels[i];
    else {
      //if there is already something there. IE a pile up occured where there
      //were two instances of the same channel in an Event. Put the event in 
      //the next spot with std::vector::insert
      eventsTemp.insert(eventsTemp.begin()+theDDASChannels[i]->chanid,theDDASChannels[i]);
//      cout<<"********WARNING Found Pile up Event********"<<endl;
    }
  }
  
  //Now copy non empty spots in the vector to the final sorted vector
  for (int i=0;i<(int)eventsTemp.size();i++){
    if (eventsTemp[i] !=NULL )
      eventsSorted.push_back(eventsTemp[i]);
  }
  //Now call the Packer to build the Event
  for (int i=0;i<(int)eventsSorted.size();++i){
    thePacker->SetDDASChannel(eventsSorted[i]);//Set the current ddaschannel
    thePacker->SetJEntry(jentry);   //set jEntry (just to save it in final tree)
    thePacker->CalcAll(); //call all the waveform analysis steps
    thePacker->PackEvent(Event); //push all the results on to the final LendEvent
  }
  
  
  

}





  /*

  vector <Double_t> thisEventsFF;
  vector <Double_t> thisEventsCFD;
  for (int i=0;i<(int)events.size();++i){
    Double_t thisEventsIntegral=0; //intialize
    Double_t longGate=0; //intialize
    Double_t shortGate=0; //intialize
    Double_t cubicCFD=0;
    Double_t softwareCFD=0;

    Double_t start=0;

    thisEventsFF.clear(); //clear
    thisEventsCFD.clear();//clear
    if ((events[i]->dchan2.trace).size()!=0 &&inMan.fast==false){ //if this event has a trace calculate filters and such
      theFilter.FastFilter(events[i]->dchan2.trace,thisEventsFF,FL,FG); //run FF algorithim
      thisEventsCFD = theFilter.CFD(thisEventsFF,CFD_delay,CFD_scale_factor); //run CFD algorithim
      
      softwareCFD=theFilter.GetZeroCrossing(thisEventsCFD)-traceDelay; //find zeroCrossig of CFD
      
      cubicCFD = theFilter.GetZeroCubic(thisEventsCFD)-traceDelay;
      
      start = theFilter.getStartForPulseShape(softwareCFD,traceDelay);
      thisEventsIntegral = theFilter.getEnergy(events[i]->dchan2.trace);
      longGate = theFilter.getGate(events[i]->dchan2.trace,start,inMan.long_gate);
      shortGate = theFilter.getGate(events[i]->dchan2.trace,start,inMan.short_gate);
            
    }
    if (lean == false){
      Event->pushTrace(events[i]->dchan2.trace);//save the trace for later if its there
      //it is 0 if it isn't


      Event->pushFilter(thisEventsFF); //save filter if it is there
      Event->pushCFD(thisEventsCFD); //save CFD if it is there
    }
    //Push other thing into the event
    Event->pushLongGate(longGate); //longer integration window
    Event->pushShortGate(shortGate);//shorter integration window
    Event->pushChannel(events[i]->dchan2.chanid);//the channel for this pulse
    Event->pushEnergy(thisEventsIntegral);;//push trace energy if there
    Event->pushInternEnergy(events[i]->dchan2.energy);//push internal energy
    Event->pushTime(events[i]->dchan2.time);
    Event->pushSoftTime(events[i]->dchan2.timelow +events[i]->dchan2.timehigh*4294967296.0+softwareCFD);
    if (lean == false){
      Event->pushTrace(events[i]->dchan2.trace);//save the trace for later if its there
      //it is 0 if it isn't
      Event->pushFilter(thisEventsFF); //save filter if it is there
      Event->pushCFD(thisEventsCFD); //save CFD if it is there
    }
    //Push other thing into the event
    Event->pushLongGate(longGate); //longer integration window
    Event->pushShortGate(shortGate);//shorter integration window
    Event->pushChannel(events[i]->dchan2.chanid);//the channel for this pulse
    Event->pushEnergy(thisEventsIntegral);;//push trace energy if there
    Event->pushInternEnergy(events[i]->dchan2.energy);//push internal energy
    Event->pushTime(events[i]->dchan2.time);
    Event->pushSoftTime(events[i]->dchan2.timelow +events[i]->dchan2.timehigh*4294967296.0+softwareCFD);
    Event->pushSoftwareCFD(softwareCFD);
    Event->pushCubicCFD(cubicCFD);
    Event->pushCubicTime(events[i]->dchan2.timelow +events[i]->dchan2.timehigh*4294967296.0+cubicCFD);
    Event->pushInternalCFD((events[i]->dchan2.timecfd)/65536.0);
    Event->pushEntryNum(events[i]->jentry);
  }

  //  Event->Finalize(); //Finalize Calculates various parameters and applies corrections

*/

