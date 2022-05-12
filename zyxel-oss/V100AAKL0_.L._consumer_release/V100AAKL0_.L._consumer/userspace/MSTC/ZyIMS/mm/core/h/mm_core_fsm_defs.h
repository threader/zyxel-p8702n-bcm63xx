/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/


#ifndef __MM_CORE_FSM_DEFS_H__
#define __MM_CORE_FSM_DEFS_H__

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

#define MAX_DSP	1
#define MAX_DSP_CHAN	6
#define MAX_CODEC	10
#define NUM_RTPRTCP_PORT	2
#define MM_INC_CALL_ID_START 0x8000
#define MAX_SIP_LINE_COUNT	10

/* added by eason, dsp pool limitations*/
#define MAX_TIMESLOT_NUM		32
#define MAX_DSP_USAGE_PER_DSP   6
#define MAX_DSP_CHAN_PER_DSP 	4
#define MAX_CONF_PER_DSP		2
#define USED 1
#define UNUSED 0

#define MM_INVALID_DSP_ID	0xffff
#define MM_INVALID_CODEC_NUM	0xffff

#define ID_NOT_USED 	0xffff
#define STREAM_ID_NOT_USED 0
#define ID_EARLYMEDIA	2000
#define ID_MUSICONHOLD	3000	/* aircheng add */
#define MOH_PHY_ID	2			/* aircheng add */

/* RTP */
#define RANDOM_SEED		0x76549812
#define ROTATE_SEED			0x00000028
#define RTP_LOOPBACK       0x7F000001	/* IPv4 loopback address */


/* All memory related macros will go here */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif



