#ifndef __ZYIMS_GLOBAL_H__
#define __ZYIMS_GLOBAL_H__
#include "syscall_wrapper.h"

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
**      primitive type definition
**____________________________________________________________________________*/

#ifndef OS_uITRON
typedef unsigned char   boolean;
typedef boolean bool;
#endif

#if 0
#ifndef UINT8
#define UINT8  unsigned char
#endif

#ifndef UINT16
#define UINT16  unsigned short
#endif

#ifndef UINT32
#define UINT32  unsigned int
#endif
#endif
#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint16
#define uint16  unsigned short
#endif

#ifndef uint32
#define uint32  unsigned int
#endif

#ifndef ip4a
#define ip4a	unsigned long int
#endif

#ifndef sint15
#define sint15	signed short
#endif

#ifndef int8
#define int8  char
#endif

#ifndef int16
#define int16  signed short
#endif

#ifndef int32
#define int32  signed int
#endif

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
**      constant
**____________________________________________________________________________*/
/*----- Define null object pointer in case stdio.h isn't included ----------------*/
#ifndef NULL
#define NULL            0
#endif
/*----- standard boolean constants -------------------------------------*/
#define FALSE                   0
#define TRUE                    1

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
**      macro
**____________________________________________________________________________*/

/* General purpose function macros already defined in turbo C 
**----------------------------------------------------------------------*/
#ifndef min
        #define min(x,y)        ((x)<(y)?(x):(y))       /* Lesser of two args */
#endif
#ifndef max
        #define max(x,y)        ((x)>(y)?(x):(y))       /* Greater of two args */
#endif

/* From ras\inc\Timer.h */
typedef	int	tm_id_t;		/* timer ID			*/

/* From ras\inc\Chann.h */
/*______________________________________________________________________*/
/*              chann_sig_t
**______________________________________________________________________*/
typedef struct {
        uint16  func_id;                                /* function-ID                                  */
} chann_sig_t;

/* From ras\inc\UDP.H */
typedef struct udp_cb udp_cb_t;

typedef enum {
	MSG_SEND_TO_CCMCONN,
	MSG_SEND_TO_PHONE,
	MSG_SEND_TO_SIP,
	ORIG_FUNC_CCMCONN,
	ORIG_FUNC_PHONE,
	ORIG_FUNC_SIP
}MsgDest;

#define RET_SUCCESS 1
#define RET_FAILURE 0
#endif
