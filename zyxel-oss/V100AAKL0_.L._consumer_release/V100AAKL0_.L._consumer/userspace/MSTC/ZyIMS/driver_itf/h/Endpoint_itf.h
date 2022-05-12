#if defined(__cplusplus)
extern "C"
{
#endif
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
#if defined (SUPPORT_406_VOICE) && !defined(BUILD_MUSIC_ON_HOLD)	/* ZyXEL_VOICE */
#if (NUM_FXS_CHANNELS == 1)
#define MAX_ENDPT 1	/* modify temporarily for only 1 endpoint without Music On Hold */
#else
#define MAX_ENDPT 2	/* modify temporarily for only 2 endpoint without Music On Hold */
#endif
#else
#if (NUM_FXS_CHANNELS == 1)
#define MAX_ENDPT 2	/*reference GLOBAL_CFG_MAX_ENDPT in globalCfgCustom.h*/
#else
#define MAX_ENDPT 3	/*reference GLOBAL_CFG_MAX_ENDPT in globalCfgCustom.h*/
#endif
#endif
#define MAX_CNX 4	/*reference GLOBAL_CFG_MAX_CNX in globalCfgCustom.h*/
#define MAX_PHYS_ENDPT MAX_ENDPT
#define UNKNOWN -1
/*
** VRG endpoint structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'ENDPT_STATE' base-class(structure).
** The base endpt structure specifies a set of function pointers that defines
** the generic endpt API that must be provided by all derived endpt types.
**
** When the VRG endpt initialization function is invoked, the base endpt
** structure function pointers will be intialized to a set of VRG specific
** endpt functions. Therefore, given a pointer to the base endpt struct,
** clients can interface to a generic endpt API that will vector to the
** derived endpt function.
*/
typedef struct
{
   /* Must be first elements of this struct. */
   const ENDPT_FUNCS   *endptFuncs;
   int                  lineId;

   /* VRG specific endpt state below. */

} VRG_ENDPT_STATE;

/* --------------------------------------------------------------------------
** Endpoint API function pointers.
*/
/*                                        *
 * This STRUCT models a physical endpoint *
 *                                        */
typedef struct
{
   ENDPT_STATE endptObjState;         /* state of the endpt object, not to confused with the "state" field" */
   EPZCAP      capabilities;          /* capabilities of the endpt */
   int         assignedCmEndptId;        /* Index of cmEndpt this physical endpoint is assigned to */   
   int 		   toneDetection;
} CMPHYSENDPT;
/*
** Endpt initialization structure
*/
typedef struct VRG_ENDPT_INIT_CFG
{
   /* current country code based on nvol settings */
   #if 1
   int country;
   #else
   VRG_COUNTRY    country;

   /* current power source - 0 = utility power
   **                        1 = battery power */
   #endif
   int            currentPowerSource;

} VRG_ENDPT_INIT_CFG;
#define  VRG_CLASS_DATE_SIZE         8
#define  VRG_CLASS_MAX_NUMBER_SIZE   20
#define  VRG_CLASS_MAX_NAME_SIZE     16

typedef struct
{
   char  date[VRG_CLASS_DATE_SIZE + 1];          /* date string */
   char  number[VRG_CLASS_MAX_NUMBER_SIZE + 1];  /* phone number string */
   char  name[VRG_CLASS_MAX_NAME_SIZE + 1];      /* name string */
} VRG_CLIDRX_DATA;
#if defined(__cplusplus)
}
#endif
extern CMPHYSENDPT cmPhysEndpt[MAX_ENDPT];
EPSTATUS vrgEndptGetRtpStatistics(int cnxId,int phyId,int * tx_rtp_pkt_cnt,int * tx_rtp_octet_cnt,int * rx_rtp_pkt_cnt, int * rx_rtp_octet_cnt, int * lost, int * jitter);
