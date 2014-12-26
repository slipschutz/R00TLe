/*****************************************/
/* This channel initializes a new        */
/* DDAS channel event.                   */
/*****************************************/

#include "ddaschannel.hh"


ClassImp(ddaschannel);

ddaschannel::ddaschannel() : TObject() {
  channelnum = 0;
  chanid = 0;
  slotid = 0;
  crateid = 0;
  channelheaderlength = 0;
  channellength = 0;
  finishcode = 0;
  overflowcode = 0;
  tracelength = 0;
  timelow = 0;
  timehigh = 0;
  timecfd = 0;
  cfdtrigsourcebit=0;  //added CP
  energy = 0;
  
  time = 0;
  cfd = 0;
  
  energySums.reserve(4);
  qdcSums.reserve(8);
  trace.reserve(200);
}

ddaschannel::ddaschannel(const ddaschannel& obj)
  : TObject(obj), 
    channelnum(obj.channelnum),
    chanid(obj.chanid),
    slotid(obj.slotid),
    crateid(obj.crateid),
    channelheaderlength(obj.channelheaderlength),
    channellength(obj.channellength),
    finishcode(obj.finishcode),
    overflowcode(obj.overflowcode),
    tracelength(obj.tracelength),
    timelow(obj.timelow),
    timehigh(obj.timehigh),
    timecfd(obj.timecfd),
    cfdtrigsourcebit(obj.cfdtrigsourcebit),  //added CP
    energy(obj.energy),
    time(obj.time),
    cfd(obj.cfd),
    energySums(obj.energySums),
    qdcSums(obj.qdcSums),
    trace(obj.trace)
{} 

ddaschannel& ddaschannel::operator=(const ddaschannel& obj)
{
  if (this!=&obj) {
    channelnum = obj.channelnum;
    chanid = obj.chanid;
    slotid = obj.slotid;
    crateid = obj.crateid;
    channelheaderlength = obj.channelheaderlength;
    channellength = obj.channellength;
    finishcode = obj.finishcode;
    overflowcode = obj.overflowcode;
    tracelength = obj.tracelength;
    timelow = obj.timelow;
    timehigh = obj.timehigh;
    timecfd = obj.timecfd;
    cfdtrigsourcebit = obj.cfdtrigsourcebit;  //added CP
    energy = obj.energy;
    time = obj.time;
    cfd = obj.cfd;
    energySums = obj.energySums;
    qdcSums = obj.qdcSums;
    trace = obj.trace;
  }

  return *this;
}

void ddaschannel::Reset() {
  channelnum = 0;
  chanid = 0;
  slotid = 0;
  crateid = 0;
  channelheaderlength = 0;
  channellength = 0;
  finishcode = 0;
  tracelength = 0;
  cfdtrigsourcebit = 0;  //Added CP
  
  timelow = 0;
  timehigh = 0;
  timecfd = 0;
  energy = 0;
  
  time = 0;
  cfd = 0;
  
  energySums.clear();
  qdcSums.clear();
  trace.clear();
}

ddaschannel::~ddaschannel() {


}

void ddaschannel::SetChannelID(UInt_t data){
  chanid = ((Int_t) ((data & CHANNELIDMASK)));
}

void ddaschannel::SetSlotID(UInt_t data){
  slotid = ((Int_t) ((data & SLOTIDMASK) >> 4));
}

void ddaschannel::SetCrateID(UInt_t data){
  crateid = ((Int_t) ((data & CRATEIDMASK) >> 8));
}

void ddaschannel::SetChannelHeaderLength(UInt_t data){
  channelheaderlength = ((Int_t) ((data & HEADERLENGTHMASK) >> 12 ));
}

void ddaschannel::SetChannelLength(UInt_t data){
  channellength = ((Int_t) ((data & CHANNELLENGTHMASK) >> 17));
}

void ddaschannel::SetOverflowCode(UInt_t data){
  overflowcode = ((Int_t) ((data & OVERFLOWMASK) >> 30));
}

void ddaschannel::SetFinishCode(UInt_t data){
  finishcode = ((Int_t) ((data & FINISHCODEMASK) >> 31 ));
}

void ddaschannel::SetID(UInt_t data){
  //broken function
}

void ddaschannel::SetTimeLow(UInt_t data){
  timelow = data;
}

void ddaschannel::SetTimeHigh(UInt_t data){
  timehigh = ((Int_t) ((data & LOWER16BITMASK)));
}

void ddaschannel::SetTimeCFD(UInt_t data){
  timecfd = ((Int_t) ((data & BIT29to16MASK) >> 16));
}

void ddaschannel::SetCFDTriggerSourceBit(UInt_t data){
  cfdtrigsourcebit = ((Int_t) ((data & BIT30MASK) >> 30 ));  //modified CP
}

void ddaschannel::SetTime(){
  time = timecfd/16384.0 + 2*(timelow + timehigh * 4294967296.0) - cfdtrigsourcebit;
}

void ddaschannel::SetEnergy(UInt_t data){
  energy = ((Int_t) ((data & LOWER16BITMASK)));
}

void ddaschannel::SetTraceLength(UInt_t data){
  tracelength = ((Int_t) ((data & UPPER16BITMASK) >> 16));
}

void ddaschannel::SetEnergySums(UInt_t data){
  energySums.push_back(data);
}

void ddaschannel::SetQDCSums(UInt_t data){
  qdcSums.push_back(data);
}

void ddaschannel::SetTraceValues(UInt_t data){
  //cout << "set trace values " << data << endl;
  trace.push_back((data & LOWER16BITMASK));
  trace.push_back((data & UPPER16BITMASK)>>16);
  //cout << "set trace values1 " << data << endl;
}

void ddaschannel::UnpackChannelData(const uint32_t *data){

  //put all variables in known state before unpacking
  Reset();

  //first four words of data identifiers, times, and energies
  //cout << "data ----------" << endl;
  //cout << data[0] << " " << data[1] << " " << data[2] <<" " << data[3] << " " << data[4] <<endl;

  //data[0] is the number of half words in the event, skip it
  *data++;

  //Using the first word of DDAS information extract channel identifiers
  SetChannelID(*data);
  SetSlotID(*data);
  SetCrateID(*data);
  SetChannelHeaderLength(*data);
  SetChannelLength(*data);
  SetOverflowCode(*data);
  SetFinishCode(*data++);

  //Second word
  // set most significant bits of time stamp
  SetTimeLow(*data++);

  //Third word
  // set least significant bits of time stamp
  SetTimeHigh(*data);

  //set cfd time
  SetTimeCFD(*data);

  //set CFDTriggerSourceBit
  SetCFDTriggerSourceBit(*data++); //added CP

  //set the full time
  SetTime();

  //Fourth word
  //set energy
  SetEnergy(*data);

  //set trace length
  SetTraceLength(*data++);

  // finished upacking the minimum set of data

  //determine if more data should be unpacked based on a comparison between channel header length
  //and channel length
  if(channelheaderlength != channellength){
    //cout << "extra info unpack " <<endl;
    //more unpacking data
    if(channellength != (channelheaderlength + tracelength/2)){
      cout << "Inconsistent lengths between channel length - " << channellength
           << " , header length - " << channelheaderlength 
           << " , and trace length - " << tracelength << endl;
      cout << "This is a fatal error - data is corrupted, please investigate " << endl;
      exit(1);
    }

    //if channel header length is 8 then the extra 4 words are energy sums and baselines
    if(channelheaderlength == 8){
      for(int z=0; z<4; z++){
	SetEnergySums(*data++);
      }
    }
    //if channel header length is 12 then the extra 8 words are QDC sums
    if(channelheaderlength == 12){
      for(int z=0; z<8; z++){
	SetQDCSums(*data++);
      }
    }

    //if channel header length is 16 then the extra 12 words are energy and QDC sums
    if(channelheaderlength == 16){
      for(int z=0; z<4; z++){
	SetEnergySums(*data++);
      }
      for(int z=0; z<8; z++){
	SetQDCSums(*data++);
      }
    }

    // if trace length is non zero, retrieve the trace
    if(tracelength !=0) {
      //cout << "trace unpack "<<endl;
      for(Int_t z = 0; z < tracelength/2; z++){
	//upack two 12 bit trace values from 32 bit data word
	SetTraceValues(*data++);
      }   
    } //finished unpacking trace

  } //finished unpacking extra data

}
