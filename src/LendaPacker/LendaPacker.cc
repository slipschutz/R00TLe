
#include "LendaPacker.hh"


LendaPacker::LendaPacker(){
  ///Use bad Defaults to ensure that the calling program set 
  //everything
  fFL=-1; 
  fFG=-1;
  fd=-1;
  fw=-1;
  lg=-1;
  sg=-1;
  lg2=-1;
  sg2=-1;
  traceDelay=-1;
  jentry=-1;

  Reset();//Reset the member variables that have to do with building Lenda Events
  //Such as the software CFDs and the energy values

  saveTraces=true;
  
  BuildMaps();
}

void LendaPacker::SetFilter(Int_t _FL,Int_t _FG,Int_t _d,Int_t _w){
  fFL=_FL;
  fFG=_FG;
  fd=_d;
  fw=_w;
}

void LendaPacker::SetGates(Double_t _lg,Double_t _sg,Double_t _lg2,Double_t _sg2){

  lg=_lg;
  sg=_sg;
  lg2=_lg2;
  sg2=_sg2;
} 


LendaPacker::~LendaPacker(){


}

void LendaPacker::Reset(){

  thisEventsFF.clear();
  thisEventsCFD.clear();
  

  thisEventsIntegral=0;
  thisEventsPulseHeight=0;
  longGate=0;
  shortGate=0;
  cubicCFD=0;
  cubicFitCFD=0;
  softwareCFD=0;
  start=0;
  numZeroCrossings=0;
  CFDResidual=0;

}

void LendaPacker::CalcAll(ddaschannel*theChannel){
   CalcTimeFilters(theChannel);
   CalcEnergyGates(theChannel);
}



void LendaPacker::CalcTimeFilters(ddaschannel*theChannel){

  if (theChannel->trace.size()!=0){
    theFilter.FastFilter(theChannel->trace,thisEventsFF,fFL,fFG); //run FF algorithim
    thisEventsCFD = theFilter.CFD(thisEventsFF,fd,fw); //run CFD algorithim
    
    softwareCFD=theFilter.GetZeroCrossing(thisEventsCFD,numZeroCrossings,CFDResidual)-traceDelay; //find zeroCrossig of CFD
    
    
    //  softwareCFD=softwareCFD-floor(softwareCFD);
    
    
    //  if (softwareCFD>1)
    //  softwareCFD=softwareCFD-1;
    //  softwareCFD=softwareCFD-floor(softwareCFD);
    cubicCFD = theFilter.GetZeroCubic(thisEventsCFD)-traceDelay;
    cubicFitCFD=theFilter.GetZeroFitCubic(thisEventsCFD)-traceDelay;
  }
}
void LendaPacker::CalcEnergyGates(ddaschannel*theChannel){

  if ( theChannel->trace.size()!=0){
    if (softwareCFD!=0)
      start = theFilter.getStartForPulseShape(softwareCFD,traceDelay);
    else{
      softwareCFD=theFilter.GetZeroCrossing(thisEventsCFD,numZeroCrossings,CFDResidual)-traceDelay;
      start = theFilter.getStartForPulseShape(softwareCFD,traceDelay);
    }

    thisEventsPulseHeight=theFilter.getMaxPulseHeight(theChannel->trace);
    
    thisEventsIntegral = theFilter.getEnergy(theChannel->trace);
    longGate = theFilter.getGate(theChannel->trace,start,lg);
    shortGate = theFilter.getGate(theChannel->trace,start,sg);
    
  }
}
void LendaPacker::BuildMaps(){
  stringstream stream;
  ifstream MapFile;
  ifstream CorrectionsFile;

  stringstream ss,ss1;
  ss<<string(getenv("ANAROOT_PRM"));
  ss1<<ss.str();

  ss<<"/MapFile.txt";
  ss1<<"/Corrections.txt";

  MapFile.open(ss.str().c_str());
  if (!MapFile.is_open()){
    cout<<"Cable Map File not found"<<endl;
    cout<<"Looking in "<<ss.str()<<endl;
    cout<<"Make a ./MapFile.txt"<<endl;
    throw -1;
  }

  CorrectionsFile.open(ss1.str().c_str());
  if ( ! CorrectionsFile.is_open()){
    cout<<"Correction File not found"<<endl;
    cout<<"Looking in "<<ss1.str()<<endl;
    cout<<"Make a ./Corrections.txt file"<<endl;
    throw -1;
  }

  //Now read in the map information
  //should be three columns 
  //slot  channel   Name
  int slot,channel;
  string name;
  int UniqueBarNumber=0;
  while (true){
    if (MapFile.eof()==true)
      break;
    MapFile>>slot>>channel>>name;
    int spot= CHANPERMOD*(slot-2) + channel;
    GlobalIDToFullLocal[spot]=name;
    FullLocalToGlobalID[name]=spot;
    string BarName=name.substr(0,name.size()-1);//The string minus last letter
    GlobalIDToBar[spot]=BarName;
    
    if (BarNameToUniqueBarNumber.count(BarName)==0){ //Isn't already there
      BarNameToUniqueBarNumber[BarName]=UniqueBarNumber;
      UniqueBarNumber++;
    }
  }
  Double_t slope,intercept,timeOffSet;
  while (true){
    if (CorrectionsFile.eof()==true)
      break;
    CorrectionsFile>>name>>slope>>intercept>>timeOffSet;
    
    if (FullLocalToGlobalID.count(name) == 0){
      //There is a name in the corrections file that isn't in the 
      //Cable Map file.  
      cout<<"Found a name in the corrections file that"<<endl;
      cout<<"wasn't in the cable map file"<<endl;
      cout<<"Name is "<<name<<endl;
      throw -99;
    }

    FullNameToCorrection[name]=Correction(slope,intercept,0);
  }
  

}


LendaChannel LendaPacker::DDASChannel2LendaChannel(ddaschannel* c,string name){
  CalcAll(c);//Preform all the waveform analysis

  /////////////////////////////////////////////////////////////////////////////
  // Now all the member variables should have been set (if there are traces) //
  // in the data. Next begin packing all the results into the LendaChannel   //
  /////////////////////////////////////////////////////////////////////////////
  LendaChannel tempLenda;

  tempLenda.SetChannel(c->chanid);
  tempLenda.SetSlot(c->slotid);
  tempLenda.SetGlobalID(CHANPERMOD*(c->slotid-2)+c->chanid);

  tempLenda.SetEnergy(thisEventsIntegral);
  tempLenda.SetPulseHeight(thisEventsPulseHeight);
  tempLenda.SetInternalEnergy(c->energy);

  tempLenda.SetTime(c->time);
  tempLenda.SetTimeLow(c->timelow);
  tempLenda.SetTimeHigh(c->timehigh);
  tempLenda.SetCFDTrigBit(c->GetCFDTriggerSourceBit());
  
  tempLenda.SetSoftTime(2*(c->timelow + c->timehigh * 4294967296.0) +softwareCFD);
  tempLenda.SetCubicTime(2*(c->timelow + c->timehigh * 4294967296.0)+cubicCFD);
  tempLenda.SetCubicFitTime(2*(c->timelow + c->timehigh * 4294967296.0)+cubicFitCFD);


  tempLenda.SetSoftwareCFD(softwareCFD);
  tempLenda.SetCubicCFD(cubicCFD);
  tempLenda.SetCubicFitCFD(cubicFitCFD);
  tempLenda.SetInternalCFD(c->timecfd/32768.0);
  
  if (saveTraces){
    tempLenda.SetTrace(c->trace);
    tempLenda.SetFilter(thisEventsFF);
    tempLenda.SetCFD(thisEventsCFD);
  }
  tempLenda.SetLongGate(longGate);
  tempLenda.SetShortGate(shortGate);
  
  tempLenda.SetJentry(jentry);
  tempLenda.SetNumZeroCrossings(numZeroCrossings);
  tempLenda.SetCFDResidual(CFDResidual);

  //Corrections should be Slope then Intercept then Timming offset
  Correction cor = FullNameToCorrection[name];
  tempLenda.SetCorrectedEnergy( thisEventsIntegral*cor.slope + cor.intercept);
  tempLenda.SetCorrectedTime( tempLenda.GetTime() + cor.timeOffSet);
  //RESET THE PACKERS VARIABLES
  Reset();
  //Return this packed channel
  return tempLenda;
}

void LendaPacker::PutDDASChannelInBar(string fullName,LendaBar &theBar,ddaschannel*theChannel){
  string lastLetter = fullName.substr(fullName.size()-1,1);

  //All we need to do is determine whether this fullName coresponds to
  //the TOP or bottom PMT of the bar 
  
  if (lastLetter == "T" ){
    //IT IS A top PMT
    LendaChannel temp = DDASChannel2LendaChannel(theChannel,fullName);
    theBar.Tops.push_back(temp);
  } else if (lastLetter == "B"){
    LendaChannel temp =DDASChannel2LendaChannel(theChannel,fullName);
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
  //////////////Get the ddaschannels form the DDASEVent///////////////
  vector <ddaschannel*> theDDASChannels = theDDASEvent->GetData();

  for (int i=0;i<(int)theDDASChannels.size();i++){
    //First Form the global ID of the channel
    int id = theDDASChannels[i]->chanid + CHANPERMOD* (theDDASChannels[i]->slotid-2);
    
    map<int,string>::iterator it = GlobalIDToFullLocal.find(id);
    string fullName;
    if ( it != GlobalIDToFullLocal.end()){ // the channel is in the map
      fullName = it->second;
      if (fullName =="OBJSCINT"){ //Special check for the Object Scintillator
	Event->TheObjectScintilator = DDASChannel2LendaChannel(theDDASChannels[i],"");
      }else if (fullName == "IGNORE"){ //Special check for IGNORE
	//Do nothing
      } else { //It is a LENDA BAR
	SetJEntry(jentry);  
	//Get Which Bar this channel belongs to from the map
	string nameOfBar=GlobalIDToBar[id];

	//Check to see if this bar has been found in this event yet
	if ( ThisEventsBars.count(nameOfBar) == 0 ) { // Bar hasn't been found yet
	  //Put a bar object into a map to keep track of things
	  LendaBar tempBar(nameOfBar);
	  //Only look up the Bar Id the first time the bar is found
	  int UniqueBarNum = BarNameToUniqueBarNumber[nameOfBar];
	  tempBar.SetBarId(UniqueBarNum);//Give the bar its ID num
	  PutDDASChannelInBar(fullName,tempBar,theDDASChannels[i]);
	  ThisEventsBars[nameOfBar]=tempBar;
	} else {
	  //The bar has already had a channel in it from a previous iteration of this loop
	  //Instead of making a new bar take the already allocated one from the map and
	  //push this channel on to it
	  PutDDASChannelInBar(fullName,ThisEventsBars[nameOfBar],theDDASChannels[i]);
	}
      }
    } else { // This is when the channel was not in the map
      Event->PushUnMappedChannel( DDASChannel2LendaChannel(theDDASChannels[i],"") );
    }
  }
  ///Now put the bars into the Lenda Event by iterating over the temporary bar map
  for (map<string,LendaBar>::iterator ii=ThisEventsBars.begin();
       ii!=ThisEventsBars.end();ii++){
    Event->PushABar(ii->second);
  }
 
  ThisEventsBars.clear();//Clear the temporary map
}



void LendaPacker::PackEvent(LendaEvent * Event){


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

  Reset();//Reset the Packers variables
}





void LendaPacker::RePackEvent(LendaEvent * Event){



}

void LendaPacker::RePackSoftwareTimes(LendaEvent *Event){
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
      Double_t tempSoftTime=theFilter.GetZeroCrossing(tempCFD,num,CFDResidual)-traceDelay;
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
      Double_t tempSoftTime=theFilter.GetZeroCrossing(tempCFD,num,CFDResidual)-traceDelay;
      Double_t tempCubicTime=theFilter.GetZeroCubic(tempCFD)-traceDelay;
      
      Event->Bars[i].Bottoms[b].SetSoftwareCFD(tempSoftTime);
      Event->Bars[i].Bottoms[b].SetSoftTime(tempSoftTime+Basetime);
      Event->Bars[i].Bottoms[b].SetCubicCFD(tempCubicTime);
      Event->Bars[i].Bottoms[b].SetCubicTime(tempCubicTime+Basetime);
    }
  }

  vector <Double_t> tempFF;
  vector <Double_t> tempCFD;
  
  if (Event->TheObjectScintilator.GetTime()!=-10008){
    vector <UShort_t> tempTrace = Event->TheObjectScintilator.GetTrace();
    theFilter.FastFilter(tempTrace,tempFF,fFL,fFG); //run FF algorithim
    tempCFD = theFilter.CFD(tempFF,fd,fw); //run CFD algorithim
    Double_t Basetime = 2*(Event->TheObjectScintilator.GetTimeLow() + Event->TheObjectScintilator.GetTimeHigh() * 4294967296.0);
    Double_t tempSoftTime=theFilter.GetZeroCrossing(tempCFD,num,CFDResidual)-traceDelay;
    Double_t tempCubicTime=theFilter.GetZeroCubic(tempCFD)-traceDelay;
  
    Event->TheObjectScintilator.SetSoftwareCFD(tempSoftTime);
    Event->TheObjectScintilator.SetSoftTime(tempSoftTime+Basetime);
    Event->TheObjectScintilator.SetCubicCFD(tempCubicTime);
    Event->TheObjectScintilator.SetCubicTime(tempCubicTime+Basetime);
  }


}
