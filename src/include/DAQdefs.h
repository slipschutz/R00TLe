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
