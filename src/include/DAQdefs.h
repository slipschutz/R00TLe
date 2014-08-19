#ifndef __DAQDEFS_HH
#define __DAQDEFS_HH


/***********************************************************************************************/
/* This file contains definitions for the format of the Data saved to disk in the .evt files   */
/* Much of the file is copied from the DataFormat.h file from DAQ10.2.  		       */
/* The define statements define data packet type flags.  PHYSICS_EVENTS are ID=30 for instance */
/* The Structure definitions define the format of the various types data saved to disk.        */
/* Extra versions of the incremental and non-incremental scalers have been added to the file   */
/* where there is no ring-item header struct at the beginning.  This is for convience in the   */
/* parsing the scaler data in Evt2Scalers						       */
/***********************************************************************************************/


#ifndef __CRT_STDINT_H
#include <stdint.h>
#ifndef __CRT_STDINT_H
#define __CRT_STDINT_H
#endif
#endif



// Copied from DataFormat.h
//>>> from here
// state change item type codes:
#define BEGIN_RUN  1
#define END_RUN    2
#define PAUSE_RUN  3
#define RESUME_RUN 4

// Documentation item type codes:
#define PACKET_TYPES                10
#define MONITORED_VARIABLES         11

// Scaler data:
#define INCREMENTAL_SCALERS         20
#define TIMESTAMPED_NONINCR_SCALERS 21

// Physics events:
#define PHYSICS_EVENT               30
#define PHYSICS_EVENT_COUNT         31

// Event builder related items:
#define EVB_FRAGMENT                40 /* Event builder fragment. */
#define EVB_UNKNOWN_PAYLOAD         41 /* Evb fragment whose payload isn't a ring item */
//<<< to here

// Copied from fragment.h
//>>> from here
#define BARRIER_NOTBARRIER   0	/* Not a barrier event. */
#define BARRIER_START        1	/* Data taking starting (BEGIN/RESUME) */
#define BARRIER_END          2	/* Data taking endng (END/PAUSE) */
#define BARRIER_SYNCH        3  /* time synchronization barrier */
//<<< to here

// DAQ configuration
#define DDAS_SOURCEID 1
#define S800_SOURCEID 2


#ifndef TITLE_MAXSIZE
#define TITLE_MAXSIZE 80
#endif





/*!  All ring items have common header structures: */

typedef struct _RingItemHeader {
  uint32_t     s_size;
  uint32_t     s_type;
} RingItemHeader, *pRingItemHeader;

/*!
  This  is the most basic item.. a generic item.  It consists only of the
  header and a generic body
*/

typedef struct _RingItem {
  RingItemHeader s_header;
  uint8_t        s_body[1];
} RingItem, *pRingItem;


/*!
  Run state changes are documented by inserting state change items that have the
  structure shown below:
*/
typedef struct _StateChangeItem {
  RingItemHeader  s_header;
  uint32_t        s_runNumber;
  uint32_t        s_timeOffset;
  uint32_t          s_Timestamp;
  char            s_title[TITLE_MAXSIZE+1];
} StateChangeItem, *pStateChangeItem;

/*!
   Scaler items contain run time counters.
*/

typedef struct _ScalerItem {
  RingItemHeader  s_header;
  uint32_t        s_intervalStartOffset;
  uint32_t        s_intervalEndOffset;
  uint32_t        s_timestamp;
  uint32_t        s_scalerCount;
  uint32_t        s_scalers[1];
} ScalerItem, *pScalerItem;



/*!
  This is a version of Scaler item without the
  ring item header
*/
typedef struct _ScalerItemNoHeader {
  uint32_t        s_intervalStartOffset;
  uint32_t        s_intervalEndOffset;
  uint32_t        s_timestamp;
  uint32_t        s_scalerCount;
  uint32_t        s_scalers[1];
} ScalerItemNoHeader, *pScalerItemNoHeader;



/*
  Nonincremental timestamped scalers are scalers that dont' get 
  reset on reads and have a 64 bit timestamp.  The timing information
  put in those scalers includes both the raw time and a divisor that
  can be used to support sub-second timing information
*/
typedef struct _NonIncrTimestampedScaler {
  RingItemHeader  s_header;
  uint64_t        s_eventTimestamp; /* For event building. */
  uint32_t        s_intervalStartOffset;
  uint32_t        s_intervalEndOffset;
  uint32_t        s_intervalDivisor;
  uint32_t        s_clockTimestamp; /* unix time */
  uint32_t        s_scalerCount;
  uint32_t        s_scalers[1];
} NonIncrTimestampedScaler, *pNonIncrTimestampedScaler;

/*!
  this a version of the non-incremental scalers with no header
*/
typedef struct _NonIncrTimestampedScalerNoHeader {
  uint64_t        s_eventTimestamp; /* For event building. */
  uint32_t        s_intervalStartOffset;
  uint32_t        s_intervalEndOffset;
  uint32_t        s_intervalDivisor;
  uint32_t        s_clockTimestamp; /* unix time */
  uint32_t        s_scalerCount;
  uint32_t        s_scalers[1];
} NonIncrTimestampedScalerNoHeader, *pNonIncrTimestampedScalerNoHeader;

/*!
  The various documentation Events are just a bunch of null terminated strings that
  are back to back in the body of the ring buffer. item.
*/
typedef struct _TextItem {
  RingItemHeader s_header;
  uint32_t       s_timeOffset;
  uint32_t         s_timestamp;
  uint32_t       s_stringCount;
  char           s_strings[1];
} TextItem, *pTextItem;


/*!
  For now a physics event is just a header and a body of uint16_t's.
*/

typedef struct _PhysicsEventItem {
  RingItemHeader s_header;
  uint16_t       s_body[];
} PhysicsEventItem, *pPhysicsEventItem;

/*!
   Clients that sample physics events will often
   need to know how many physics events have been produced
   so that they can figure out the sampling fraction.
*/
typedef struct __PhysicsEventCountItem {
  RingItemHeader s_header;
  uint32_t       s_timeOffset;
  uint32_t         s_timestamp;
  uint64_t       s_eventCount;	/* Maybe 4Gevents is too small ;-) */
} PhysicsEventCountItem, *pPhysicsEventCountItem;

/**
 * Event builder stages can put event fragments into the
 * ring buffer for monitoring software:
 * (EVB_FRAGMENT):
 */
typedef struct _EventBuilderFragment {
  RingItemHeader s_header;
  uint64_t       s_timestamp;
  uint32_t       s_sourceId;
  uint32_t       s_payloadSize;
  uint32_t       s_barrierType;
  uint32_t        s_body[1];	/* Really s_payload bytes of data.. */
} EventBuilderFragment, *pEventBuilderFragment;


#endif /* __DAQDEFS_HH */
