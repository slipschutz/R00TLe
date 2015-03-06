
 #include "LendaPacker.hh"
 #include "Utilities.hh"

 LendaPacker::LendaPacker(R00TLeSettings*v){
   ///Will use bad defaults to ensure that the calling program set 
   ///all filter parameters

   lg=15;
   sg=8;
   lg2=-1;
   sg2=-1;
   traceDelay=0;
   jentry=-1;

   theSettings=v;

   
   referenceChannelPattern="OBJ";
   
   Reset();//Reset the member variables that have to do with building Lenda Events
   //Such as the software CFDs and the energy values

   saveTraces=false;


 }

 void LendaPacker::SetSettingFileNames(string MapFileName,string CorrectionsFileName){
   _mapFileName=MapFileName;
   _correctionsFileName=CorrectionsFileName;

   Info("LendaPacker","Using MapFile %s and CorrectionsFile %s",_mapFileName.c_str(),_correctionsFileName.c_str());

   BuildMaps();
 }

void LendaPacker::ForceAllBarFilters(Int_t FL, Int_t FG, Int_t d, Int_t w){
  
  for ( auto & i : GlobalIDToMapInfo){
    if (i.second.IsAReferenceChannel == false){
      i.second.FL=FL;
      i.second.FG=FG;
      i.second.d=d;
      i.second.w=w;
    }
  }
  UpdateSettings();
}

void LendaPacker::ForceAllReferenceFilters(Int_t FL, Int_t FG, Int_t d, Int_t w){
  for ( auto & i : GlobalIDToMapInfo){
    if (i.second.IsAReferenceChannel == true){
      i.second.FL=FL;
      i.second.FG=FG;
      i.second.d=d;
      i.second.w=w;
    }
  }
  UpdateSettings();
}

void LendaPacker::ForceAllFilters(Int_t FL, Int_t FG, Int_t d, Int_t w){
  for ( auto & i : GlobalIDToMapInfo){
    i.second.FL=FL;
    i.second.FG=FG;
    i.second.d=d;
    i.second.w=w;
  }
  UpdateSettings();
}



 void LendaPacker::SetGates(Double_t _lg,Double_t _sg,Double_t _lg2,Double_t _sg2){

   lg=_lg;
   sg=_sg;
   lg2=_lg2;
   sg2=_sg2;
 } 


 LendaPacker::~LendaPacker(){
   Reset();

 }

 void LendaPacker::Reset(){

   //Clear the temporary filter vectors 
   thisEventsFF.clear();
   thisEventsCFD.clear();

   //Reset packer variables
   thisEventsIntegral=0;
   thisEventsFilterHeight=0;
   thisEventsPulseHeight=0;
   longGate=0;
   shortGate=0;
   cubicCFD=0;
   cubicFitCFD=0;
   softwareCFD=0;
   start=0;
   numZeroCrossings=0;
   CFDResidual=0;
   jentry=-1;

 }

void LendaPacker::CalcAll(ddaschannel*theChannel,MapInfo info){
  CalcTimeFilters(theChannel->trace,info);
  CalcEnergyGates(theChannel->trace,info);
 }



void LendaPacker::CalcTimeFilters(vector<UShort_t> & theTrace,MapInfo info){

  Int_t FL=info.FL;
  Int_t FG=info.FG;
  Int_t d =info.d;
  Int_t w =info.w;
   ///////////////////////////////////////////////////////////////////////////////////
   // if there are trace present in the data preform timing related trace anaylsis  //
   // routines									   //
   ///////////////////////////////////////////////////////////////////////////////////
   if (theTrace.size()!=0){
     theFilter.FastFilter(theTrace,thisEventsFF,FL,FG); //run FF algorithim
     // thisEventsCFD = theFilter.CFD(thisEventsFF,d,w); //run CFD algorithim
     thisEventsCFD=theFilter.GetNewFirmwareCFD(theTrace,FL,FG,d,w);
     
     softwareCFD=theFilter.GetZeroCrossingImproved(thisEventsCFD,numZeroCrossings,CFDResidual); //find zeroCrossig of CFD
     
     cubicCFD = theFilter.GetZeroCubic(thisEventsCFD);
     //    cubicFitCFD=theFilter.GetZeroFitCubic(thisEventsCFD);
   }
 }

void LendaPacker::CalcEnergyGates(vector<UShort_t> & theTrace, MapInfo info){

   ////////////////////////////////////////////////////////////////////
   // if there are traces present in the data preform energy related //
   // trace analysis routines 					    //
   ////////////////////////////////////////////////////////////////////

   if ( theTrace.size()!=0){
     if ( thisEventsFF.size() == 0 ){
       // the filter hasn't been calculated.  It is need 
       // To get the maximum Filter Height
       theFilter.FastFilter(theTrace,thisEventsFF,info.FL,info.FG); //run FF algorithim
     }



     Int_t MaxSpotInTrace=0;
     Int_t MaxSpotInFilter=0;

     thisEventsPulseHeight=theFilter.GetMaxPulseHeight(theTrace,MaxSpotInTrace);
     thisEventsFilterHeight=theFilter.GetMaxPulseHeight(thisEventsFF,MaxSpotInFilter);


     start = theFilter.GetStartForPulseShape(MaxSpotInTrace);
     thisEventsIntegral = theFilter.GetEnergy(theTrace,MaxSpotInTrace);

     // longGate = theFilter.GetGate(theTrace,start,lg);
     // shortGate = theFilter.GetGate(theTrace,start,sg);

   }
 }

 MapInfo LendaPacker::GetMapInfo(string FullName){

   return GlobalIDToMapInfo[FullLocalToGlobalID[FullName]];

 }

 void LendaPacker::BuildMaps(){

   stringstream stream;
   ifstream MapFile;
   ifstream CorrectionsFile;

   stringstream ss,ss1;
   ss<<string(getenv("R00TLe_PRM"));
   ss1<<ss.str();

   ss<<"/"<<_mapFileName;
   ss1<<"/"<<_correctionsFileName;

   MapFile.open(ss.str().c_str());
   if (!MapFile.is_open()){
     cout<<"Cable Map File not found"<<endl;
     cout<<"Looking in "<<ss.str()<<endl;
     cout<<"Make a MapFile"<<endl;
     throw -1;
   }

   CorrectionsFile.open(ss1.str().c_str());
   if ( ! CorrectionsFile.is_open()){
     cout<<"Correction File not found"<<endl;
     cout<<"Looking in "<<ss1.str()<<endl;
     cout<<"Make a Corrections file"<<endl;
     throw -1;
   }

   //Now read in the map information
   //should be 5 columns 
   //slot channel Name Angle ReferenceName
   int slot,channel;
   string name,ReferenceName;
   double Angle;
   int UniqueBarNumber=0;
   
   
   std::string line;
   while (std::getline(MapFile>>std::ws, line)){
   int FL,FG,d,w;
   FL=FG=d=w=0;
     if (line[0] != '#' ){
       std::istringstream iss(line);
       iss>>slot>>channel>>name>>Angle>>ReferenceName>>FL>>FG>>d>>w;
      
       //Make Global ID
       int spot= CHANPERMOD*(slot-2) + channel;
       
       
       MapInfo tempInfo;
       //Check to see if this name contains the ReferenceChannelPattern
       if( name.find(referenceChannelPattern) != string::npos ){//Channel contains the pattern
	 //It is a reference channel.  Set flag in mapinfo for later
	 tempInfo.IsAReferenceChannel=true;
       } else{
	 tempInfo.IsAReferenceChannel=false;
       }

       GlobalIDToFullLocal[spot]=name;
       FullLocalToGlobalID[name]=spot;
       string BarName=name.substr(0,name.size()-1);//The string minus last letter
       GlobalIDToBar[spot]=BarName;
       
       tempInfo.FL=FL;
       tempInfo.FG=FG;
       tempInfo.d=d;
       tempInfo.w=w;

       tempInfo.GlobalID=spot;
       tempInfo.FullName = name;
       tempInfo.BarName = BarName;
       
       tempInfo.ReferenceName=ReferenceName;
       tempInfo.BarAngle=Angle;
       if (Angle !=0){
	 BarNameToBarAngle[BarName]=Angle;
       }
       if (BarNameToUniqueBarNumber.count(BarName)==0){ //Isn't already there
	 BarNameToUniqueBarNumber[BarName]=UniqueBarNumber;
	 UniqueBarNumber++;
       } 
       GlobalIDToMapInfo[spot]=tempInfo;
     }//end if not a comment
   }//end while for Map file


   Double_t slope,intercept,timeOffSet;
   while (std::getline(CorrectionsFile>>std::ws, line)){
     if (line[0] != '#' ){
       std::istringstream iss(line);
       iss>>name>>slope>>intercept>>timeOffSet;
       
       if (FullLocalToGlobalID.count(name) == 0){
	 //There is a name in the corrections file that isn't in the 
	 //Cable Map file.  
	 cout<<"Found a name in the corrections file that"<<endl;
	 cout<<"wasn't in the cable map file"<<endl;
	 cout<<"Name is "<<name<<endl;
	 throw -99;
       }
       int GlobalID = FullLocalToGlobalID[name];
       MapInfo * temp =&GlobalIDToMapInfo[GlobalID];
       temp->EnergySlope=slope;
       temp->EnergyIntercept=intercept;
       temp->TOFOffset=timeOffSet;
       temp->HasCorrections=true;
     }
   }
   /////////////////////////////////////////////////////////////////////////////////////
   // now that the maps have been built.  Loop over the main conatainer and determine //
   // the global ID of the reference name.  If it is not in the map throw error	     //
   /////////////////////////////////////////////////////////////////////////////////////
   for (auto & ii : GlobalIDToMapInfo){
     int GlobalID = ii.first;

     if (FullLocalToGlobalID.count(ii.second.ReferenceName) !=0){
       //If the name given as the reference channel is in the
       //map some where
       int RefGlobalID = FullLocalToGlobalID[ii.second.ReferenceName];
       ii.second.GlobalID=GlobalID;
       ii.second.ReferenceGlobalID=RefGlobalID;

     } else {
       cout<<"Found a reference name in the map file that does not map to a channel"<<endl;
       cout<<"The name was "<<ii.second.ReferenceName<<" it is from map info of "<<ii.second.FullName<<endl;
       throw -12;
     }

     if (ii.second.BarAngle == 0 ){//This channel doesn't have a Bar angle
       //Get angle from the BarNameToBarAngle Map
       //This rigamaroll is to let you type the angle of the bar once in the bar
       //map.  Instead of doing it both for the top and bottom channel.  One can be 0
       ii.second.BarAngle = BarNameToBarAngle[ii.second.BarName];
     }
   }

   UpdateSettings();

   // for (auto i : GlobalIDToMapInfo){
   //   cout<< i.first <<" ";
   //   i.second.Print();
   //   cout<<endl;
   // }
 }

void LendaPacker::UpdateSettings(){
  
  for (auto ii : GlobalIDToMapInfo){
     int GlobalID = ii.first;
    
    theSettings->AddCorrectionSettings(ii.second.FullName,ii.second.EnergySlope,
				       ii.second.EnergyIntercept,ii.second.TOFOffset);
  
    theSettings->AddMapSettings(ii.second.FullName,GlobalID,ii.second.ReferenceName,ii.second.ReferenceGlobalID);
  
    theSettings->AddFilterSettings(ii.second.FullName, ii.second.FL, ii.second.FG, ii.second.d, ii.second.w);
 
  }

  theSettings->SetBarIds(BarNameToUniqueBarNumber);
}

 LendaChannel LendaPacker::DDASChannel2LendaChannel(ddaschannel* c,MapInfo info){
   // if (info.IsAReferenceChannel){
   //   CalcObjectTimeFilters(c->trace);
   // }else{
   //   CalcTimeFilters(c->trace);
   // }

   CalcTimeFilters(c->trace,info);

   CalcEnergyGates(c->trace,info);

   /////////////////////////////////////////////////////////////////////////////
   // Now all the member variables should have been set (if there are traces) //
   // in the data. Next begin packing all the results into the LendaChannel   //
   /////////////////////////////////////////////////////////////////////////////
   LendaChannel tempLenda;

   tempLenda.SetChannel(c->chanid);
   tempLenda.SetSlot(c->slotid);

   tempLenda.SetGlobalID(info.GlobalID);
   tempLenda.SetReferenceGlobalID(info.ReferenceGlobalID);

   tempLenda.SetChannelName(info.FullName);
   tempLenda.SetReferenceChannelName(info.ReferenceName);

   tempLenda.SetEnergy(thisEventsIntegral);
   tempLenda.SetPulseHeight(thisEventsPulseHeight);
   tempLenda.SetInternalEnergy(c->energy);
   tempLenda.SetFilterHeight(thisEventsFilterHeight);

   tempLenda.SetTime(c->time);
   tempLenda.SetTimeLow(c->timelow);
   tempLenda.SetTimeHigh(c->timehigh);
   tempLenda.SetCFDTrigBit(c->GetCFDTriggerSourceBit());
   tempLenda.SetInternalCFD(c->timecfd/16384.0);


   if (saveTraces){
     tempLenda.SetTrace(c->trace);
   }

   PackCalculatedValues(&tempLenda,info);

   //RESET THE PACKERS VARIABLES
   Reset();
   //Return this packed channel
   return tempLenda;
 }

 void LendaPacker::PackCalculatedValues(LendaChannel* theChannel,MapInfo & info){
   theChannel->SetSoftTime(2*(theChannel->GetTimeLow() + theChannel->GetTimeHigh() * 4294967296.0) +softwareCFD);
   theChannel->SetCubicTime(2*(theChannel->GetTimeLow() + theChannel->GetTimeHigh() * 4294967296.0)+cubicCFD);
   theChannel->SetCubicFitTime(2*(theChannel->GetTimeLow() + theChannel->GetTimeHigh() * 4294967296.0)+cubicFitCFD);

   Double_t Temp=cubicCFD-TMath::Floor(cubicCFD);
   theChannel->SetOtherTime(2*(theChannel->GetTimeLow() + theChannel->GetTimeHigh() * 4294967296.0)+Temp -theChannel->GetCFDTrigBit());

   theChannel->SetSoftwareCFD(softwareCFD);
   theChannel->SetCubicCFD(cubicCFD);
   theChannel->SetCubicFitCFD(cubicFitCFD);


   if (saveTraces){
     theChannel->SetFilter(thisEventsFF);
     theChannel->SetCFD(thisEventsCFD);
   }
   theChannel->SetLongGate(longGate);
   theChannel->SetShortGate(shortGate);

   theChannel->SetJentry(jentry);
   theChannel->SetNumZeroCrossings(numZeroCrossings);
   theChannel->SetCFDResidual(CFDResidual);


   if (info.HasCorrections){
    theChannel->SetCorrectedEnergy( thisEventsIntegral*info.EnergySlope + info.EnergyIntercept);
    theChannel->SetCorrectedTime( theChannel->GetTime() - info.TOFOffset);
    theChannel->SetCorrectedSoftTime(theChannel->GetSoftTime()-info.TOFOffset);
    theChannel->SetCorrectedCubicFitTime( theChannel->GetCubicFitTime() - info.TOFOffset);
    theChannel->SetCorrectedCubicTime( theChannel->GetCubicTime() - info.TOFOffset);
   }

 }


 void LendaPacker::PutDDASChannelInBar(MapInfo info,LendaBar &theBar,ddaschannel*theChannel){
   string fullName = info.FullName;
   string lastLetter = fullName.substr(fullName.size()-1,1);

   //All we need to do is determine whether this fullName coresponds to
   //the TOP or bottom PMT of the bar 

   if (lastLetter == "T" ){
     //IT IS A top PMT
     LendaChannel temp = DDASChannel2LendaChannel(theChannel,info);
     theBar.Tops.push_back(temp);
   } else if (lastLetter == "B"){
     LendaChannel temp =DDASChannel2LendaChannel(theChannel,info);
     theBar.Bottoms.push_back(temp);
   } else {
     cout<<"***************************************************************************"<<endl;
     cout<<"***************************************************************************"<<endl;
     cout<<"Found a ddaschannel that maps to a bar name without a T or B as last letter"<<endl;
     cout<<"****Bars in the cable map file must have names that end in T or B****"<<endl;
     cout<<"Name was "<<fullName;
     throw -99;
   }

 }




 void LendaPacker::MakeLendaEvent(LendaEvent *Event,DDASEvent *theDDASEvent,
				  Long64_t jentry){


   ////////////////////////////////////////////////////////////////////
   //////////////Get the ddaschannels from the DDASEVent///////////////
   vector <ddaschannel*> theDDASChannels = theDDASEvent->GetData();

   map <int,RefTimeContainer > GlobalIDToReferenceTimes; //Container to hold the time stamps from the reference channels
   map<string,LendaBar> ThisEventsBars;//Container to hold the Bars that have fired in the event. Is copied into the LendaEvent

   //Container to hold the Reference channels for the event.  Is a multimap to allow for more than one
   //of the same channel to be in one event.
   multimap<string,LendaChannel> ThisEventsObjectScintillators;

   //Loop over all the ddas Channels in the event and pack them into the LendaEvent
   for (int i=0;i<(int)theDDASChannels.size();i++){
     //First Form the global ID of the channel
     int id = theDDASChannels[i]->chanid + CHANPERMOD* (theDDASChannels[i]->slotid-2);

     map<int,MapInfo>::iterator it = GlobalIDToMapInfo.find(id);//Look for this channel in the MapInfo Map
     string fullName;
     
     if ( it != GlobalIDToMapInfo.end()){ // the channel is in the map
       fullName = it->second.FullName;

       if (fullName.find(referenceChannelPattern) != string::npos ){ //Special check for the reference channels
	 //If the channel is one of the Object Scintillators
	 LendaChannel Temp = DDASChannel2LendaChannel(theDDASChannels[i],it->second);
	 //store the times in reference time map for later
	 GlobalIDToReferenceTimes[id] = RefTimeContainer(Temp.GetTime(),Temp.GetSoftTime(),Temp.GetCubicTime());
	 
	 //Copy the Temp lendachannel into the multimap.  They are automatically sorted by their name
	 //The channels will be packed into the LendaEent later below
	 ThisEventsObjectScintillators.insert(make_pair(fullName,Temp));
	 
       }else if (fullName == "IGNORE"){ //Special check for IGNORE
	 //Do nothing
       } else { //It is a LENDA BAR
	 SetJEntry(jentry);  
	 //Get the bar name from the MapInfo object
	 //NOTE: it is the iterator for the GlobalIDToMapInfo container
	 string nameOfBar=it->second.BarName;

	 //Check to see if this bar has been found in this event yet
	 if ( ThisEventsBars.count(nameOfBar) == 0 ) { // Bar hasn't been found yet
	   //The bar object will be put in a map to keep track of things
	   LendaBar tempBar(nameOfBar,it->second.BarAngle);
	   //Only look up the Bar Id the first time the bar is found
	   int UniqueBarNum = BarNameToUniqueBarNumber[nameOfBar];
	   tempBar.SetBarId(UniqueBarNum);//Give the bar its ID num
	   //here it->second is refering to the Mapinfo Object.  it is from the
	   //GlobalIdToMapInfo map.
	   PutDDASChannelInBar(it->second,tempBar,theDDASChannels[i]);//Build The tempBar

	   ThisEventsBars[nameOfBar]=tempBar; //Copy the tempBar into thisEventsBars map
	 } else {
	   //The bar has already had a channel in it from a previous iteration of this loop 
	   //over DDAS Channels Instead of making a new bar take the already allocated one 
	   //from the map and push this channel on to it
	   PutDDASChannelInBar(it->second,ThisEventsBars[nameOfBar],theDDASChannels[i]);
	 }
       }
     } else { // This is when the channel was not in the map
       Event->PushUnMappedChannel( DDASChannel2LendaChannel(theDDASChannels[i],MapInfo()) );
     }
   }//End FOR over DDASChannels
   
   /////////////////////////////////////////////////////////////////////////////////////
   // All channels have now been extracted from the DDASEvent.  The channels are also //
   // now sorted by channelName/barName						      //
   /////////////////////////////////////////////////////////////////////////////////////
   
   //Push the reference channels into the lendaevent.
   for (auto  i : ThisEventsObjectScintillators){
     Event->TheObjectScintillators.push_back(i.second);
   }

  ///Now put the bars into the Lenda Event by iterating over the temporary bar map
  ///And Set the reference times for each LendaChannel
  FillReferenceTimesInEvent(Event,ThisEventsBars,GlobalIDToReferenceTimes);
 
  ThisEventsBars.clear();//Clear the temporary map of bars
  GlobalIDToReferenceTimes.clear();
  ThisEventsObjectScintillators.clear();
}


void LendaPacker::FillReferenceTimesInEvent(LendaEvent* Event,map<string,LendaBar>&ThisEventsBars, map <int,RefTimeContainer > & GlobalIDToReferenceTimes){

  //Loop over the ThisEventsBars container and set all the reference times 
  for (map<string,LendaBar>::iterator ii=ThisEventsBars.begin();ii!=ThisEventsBars.end();ii++){
    //Loop over all tops in this bar
    for (int t=0;t<ii->second.Tops.size();t++){
      RefTimeContainer * temp = &GlobalIDToReferenceTimes[ii->second.Tops[t].GetReferenceGlobalID()];
      ii->second.Tops[t].SetReferenceTime(temp->RefTime);
      ii->second.Tops[t].SetSoftReferenceTime(temp->RefSoftTime);
      ii->second.Tops[t].SetCubicReferenceTime(temp->RefCubicTime);
    }
    //Loop over all bottoms in this bar
    for (int b=0;b<ii->second.Bottoms.size();b++){
      RefTimeContainer * temp = &GlobalIDToReferenceTimes[ii->second.Bottoms[b].GetReferenceGlobalID()];
      ii->second.Bottoms[b].SetReferenceTime(temp->RefTime);
      ii->second.Bottoms[b].SetSoftReferenceTime(temp->RefSoftTime);
      ii->second.Bottoms[b].SetCubicReferenceTime(temp->RefCubicTime);
    }
    //Add the LendaBar Objects to the lendaEvent
    Event->PushABar(ii->second);
  }
}



void LendaPacker::RePackChannel(LendaChannel *channel,bool isAnObject){
  //Get The global ID stored in the LendaChannel
  //Assuming now that only the corrections information is
  //being repacked.  Not the Map information

  int GlobalID = channel->GetGlobalID();
  
  map<int,MapInfo>::iterator it = GlobalIDToMapInfo.find(GlobalID); 
  
  if (it != GlobalIDToMapInfo.end() ){ //This global ID is in the map file
    MapInfo info = it->second;//Get the mapinfo object from map
    
    Reset();//Reseet the Packers variables
    if ( channel->GetTrace().size() !=0 ){//There is a trace
      vector<UShort_t> theTrace =channel->GetTrace();
      
      CalcTimeFilters(theTrace,info);
      
      CalcEnergyGates(theTrace,info);
    }
    //Set the waveform analysis results to the channel.
    //including things like energy cubic times and
    //the corrected times and energies
    PackCalculatedValues(channel,info);
  }else {//This Global Id has no map information


  }
  Reset();
}

void LendaPacker::ReMakeLendaEvent(LendaEvent* inEvent,LendaEvent* outEvent){
  //
  // first copy the information in inEvent to outEvent
  //
  // Can't do this outEvent = new LendaEvent(*inEvent);
  // OutEvent already newed for the tree and it's address can't change


  map<string,LendaBar> ThisEventsBars;
  
  for (int i=0;i<inEvent->NumBars;i++){
    
    int numTops = inEvent->Bars[i].NumTops;
    int numBottoms= inEvent->Bars[i].NumBottoms;
    
    for (int t=0;t<numTops;t++){
      RePackChannel(&inEvent->Bars[i].Tops[t]);
    }

    for (int b=0;b<numBottoms;b++){
      RePackChannel(&inEvent->Bars[i].Bottoms[b]);
    }
    ThisEventsBars[inEvent->Bars[i].Name]=inEvent->Bars[i];//Put the repacked bars into the ThisEventsBars
  }

  map <int,RefTimeContainer >  GlobalIDToReferenceTimes;
  
  for (int i=0;i<inEvent->NumObjectScintillators;i++){
    outEvent->TheObjectScintillators.push_back(inEvent->TheObjectScintillators[i]);//Copy a Obj Scint.
    LendaChannel * thisObjectScintillator = &(outEvent->TheObjectScintillators[i]);
    RePackChannel(thisObjectScintillator,true);
    GlobalIDToReferenceTimes[thisObjectScintillator->GetGlobalID()]=RefTimeContainer(thisObjectScintillator->GetTime(),
										     thisObjectScintillator->GetSoftTime(),
										     thisObjectScintillator->GetCubicFitTime());
  }
  
  // for (auto & i : GlobalIDToReferenceTimes){
  //   cout<<setprecision(20)<<i.first<<" "<<i.second.RefTime<<endl;
  // }

  
  FillReferenceTimesInEvent(outEvent,ThisEventsBars,GlobalIDToReferenceTimes);


  ThisEventsBars.clear();
}


void LendaPacker::PackEvent(LendaEvent * Event){
  cout<<"LendaPacker::PackEvent is OBSOLETE!!!!"<<endl;

  // Event->pushTrace(theChannel->trace);//save the trace for later 
  
  // Event->pushFilter(thisEventsFF); //save filter if it is there
  // Event->pushCFD(thisEventsCFD); //save CFD if it is there
  
  // //Push other thing into the event
  // Event->pushLongGate(longGate); //longer integration window
  // Event->pushShortGate(shortGate);//shorter integration window

  // Event->pushChannel(theChannel->chanid);//the channel for this pulse
  // Event->pushSlot(theChannel->slotid);
  // Event->pushGlobalID(theChannel->chanid+ 16*(theChannel->slotid-2));

  // Event->pushEnergy(thisEventsIntegral);;//push trace energy if therex
  // Event->pushInternEnergy(theChannel->energy);//push internal energy
  // Event->pushTime(theChannel->time);
  // Event->pushTimeLow(theChannel->timelow);
  // Event->pushTimeHigh(theChannel->timehigh);
  // Event->pushCFDTrigBit(theChannel->GetCFDTriggerSourceBit());
  // Event->pushSoftTime(2*(theChannel->timelow + theChannel->timehigh * 4294967296.0) +softwareCFD);// -theChannel->GetCFDTriggerSourceBit());
  // //Event->pushSoftTime(theChannel->time - (theChannel->timecfd/32768.0) +softwareCFD);

  // Event->pushSoftwareCFD(softwareCFD);
  // Event->pushCubicCFD(cubicCFD);
  // Event->pushCubicFitCFD(cubicFitCFD);

  // Event->pushCubicTime(2*(theChannel->timelow + theChannel->timehigh * 4294967296.0)+cubicCFD);
  // Event->pushCubicFitTime(2*(theChannel->timelow + theChannel->timehigh * 4294967296.0)+cubicFitCFD);

  // Event->pushInternalCFD((theChannel->timecfd)/32768.0);
  // Event->pushEntryNum(jentry);
  // Event->pushNumZeroCrossings(numZeroCrossings);
  // Event->pushPulseHeight(thisEventsPulseHeight);

  //  Reset();//Reset the Packers variables
}





void LendaPacker::RePackEvent(LendaEvent * Event){
  stringstream stream;
  ifstream MapFile;
  ifstream CorrectionsFile;

  stringstream ss,ss1;
  ss<<string(getenv("R00TLe_PRM"));
  ss1<<ss.str();

  ss<<"/"<<_mapFileName;
  ss1<<"/"<<_correctionsFileName;



}

void LendaPacker::RePackSoftwareTimes(LendaEvent *Event){
  cout<<"<LendaPacker::RePackSoftareTimes> This should not be called"<<endl;
  /*
  //Event should already be packed  
  traceDelay=0;
  int num;
  for (int i=0;i<(int)Event->NumBars;i++){
    for (int t=0;t<(int)Event->Bars[i].Tops.size();t++){
      
      vector <Double_t> tempFF;
      vector <Double_t> tempCFD;
      vector <UShort_t> tempTrace = Event->Bars[i].Tops[t].GetTrace();
      theFilter.FastFilter(tempTrace,tempFF,fFL,fFG); //run FF algorithim
      tempCFD = theFilter.CFD(tempFF,fd,fw); //run CFD algorithim
      Double_t Basetime = 2*(Event->Bars[i].Tops[t].GetTimeLow() + Event->Bars[i].Tops[t].GetTimeHigh() * 4294967296.0);
      Double_t tempSoftTime=theFilter.GetZeroCrossingImproved(tempCFD,num,CFDResidual)-traceDelay;
      Double_t tempCubicTime=theFilter.GetZeroCubic(tempCFD)-traceDelay;
      
      Event->Bars[i].Tops[t].SetSoftwareCFD(tempSoftTime);
      Event->Bars[i].Tops[t].SetSoftTime(tempSoftTime+Basetime);
      Event->Bars[i].Tops[t].SetCubicCFD(tempCubicTime);
      Event->Bars[i].Tops[t].SetCubicTime(tempCubicTime+Basetime);
    }
    for (int b=0;b<(int)Event->Bars[i].Bottoms.size();b++){
      vector <Double_t> tempFF;
      vector <Double_t> tempCFD;
      vector <UShort_t> tempTrace = Event->Bars[i].Bottoms[b].GetTrace();
      theFilter.FastFilter(tempTrace,tempFF,fFL,fFG); //run FF algorithim
      tempCFD = theFilter.CFD(tempFF,fd,fw); //run CFD algorithim
      Double_t Basetime = 2*(Event->Bars[i].Bottoms[b].GetTimeLow() + Event->Bars[i].Bottoms[b].GetTimeHigh() * 4294967296.0);
      Double_t tempSoftTime=theFilter.GetZeroCrossingImproved(tempCFD,num,CFDResidual)-traceDelay;
      Double_t tempCubicTime=theFilter.GetZeroCubic(tempCFD)-traceDelay;
      
      Event->Bars[i].Bottoms[b].SetSoftwareCFD(tempSoftTime);
      Event->Bars[i].Bottoms[b].SetSoftTime(tempSoftTime+Basetime);
      Event->Bars[i].Bottoms[b].SetCubicCFD(tempCubicTime);
      Event->Bars[i].Bottoms[b].SetCubicTime(tempCubicTime+Basetime);
    }
  }

  // vector <Double_t> tempFF;
  // vector <Double_t> tempCFD;
  
  // if (Event->TheObjectScintilator.GetTime()!=-10008){
  //   vector <UShort_t> tempTrace = Event->TheObjectScintilator.GetTrace();
  //   theFilter.FastFilter(tempTrace,tempFF,fFL,fFG); //run FF algorithim
  //   tempCFD = theFilter.CFD(tempFF,fd,fw); //run CFD algorithim
  //   Double_t Basetime = 2*(Event->TheObjectScintilator.GetTimeLow() + Event->TheObjectScintilator.GetTimeHigh() * 4294967296.0);
  //   Double_t tempSoftTime=theFilter.GetZeroCrossing(tempCFD,num,CFDResidual)-traceDelay;
  //   Double_t tempCubicTime=theFilter.GetZeroCubic(tempCFD)-traceDelay;
  
  //   Event->TheObjectScintilator.SetSoftwareCFD(tempSoftTime);
  //   Event->TheObjectScintilator.SetSoftTime(tempSoftTime+Basetime);
  //   Event->TheObjectScintilator.SetCubicCFD(tempCubicTime);
  //   Event->TheObjectScintilator.SetCubicTime(tempCubicTime+Basetime);
  // }

*/
}




void LendaPacker::FindAndSetMapAndCorrectionsFileNames(int RunNumber){
  /////////////////////////////////////////////////////////////////////////////
  // Look for MapFiles named MapFileRunNuber.txt and correction files	     //
  // name CorrectionsRunNumber.txt		      			     //
  /////////////////////////////////////////////////////////////////////////////

  stringstream filePath,mapFile,correctionsFile;
  
  ifstream testFile;

  filePath<<string(getenv("R00TLe_PRM"));
  filePath<<"/";

  mapFile<<filePath.str()<<"MapFile"<<RunNumber<<".txt";  
  correctionsFile<<filePath.str()<<"Corrections"<<RunNumber<<".txt";  
  
  string corName="Corrections.txt";
  string mapName="MapFile.txt";

  

  testFile.open(mapFile.str().c_str());
  if (testFile.good()){
    stringstream s;
    s<<"MapFile"<<RunNumber<<".txt";
    mapName = s.str();
  }
  testFile.close();
  
  testFile.open(correctionsFile.str().c_str());
  if (testFile.good()){
    stringstream s;
    s<<"Corrections"<<RunNumber<<".txt";
    corName = s.str();
  }
  testFile.close();
  
  SetSettingFileNames(mapName,corName);




}
