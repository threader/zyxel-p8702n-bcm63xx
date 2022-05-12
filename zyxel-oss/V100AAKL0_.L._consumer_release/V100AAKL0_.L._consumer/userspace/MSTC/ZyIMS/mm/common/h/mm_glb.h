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


#ifndef __MM_GLB_H__
#define __MM_GLB_H__

/* Inclusion of all the header files will go here
 */

#include "mm_cfg_types.h"
#include "mm_port_wrapper.h"
#include "mm_core_fsm_types.h"
#include "voice_dsp_core_fsm_types.h"
/*extern declaration for config database */
extern mmCfgDb_t gMmCfgDb_t;

/* universal application id to be used in all APIs towards IPTK*/

/*socket and tty file descriptors */
extern icf_int32_t gMmFd ;
extern fd_set mmFdset;
extern dspPool_t *gDspPool_p;
extern uint8 mmRmDebug;
extern uint8 mmRtpDebug;
extern uint8 mmDspDebug;
extern uint8 mmIvrDebug;

/* pointer for call context*/
extern mmCoreCallCtxList_t *p_call_context_list;

/* function pointer table for call state machine */

extern mmCoreStateEventFprtTy mmRmCoreStateEvent[RM_CORE_LAST_EVENT];

extern mmCoreStateEventFprtTy mmRtpCoreStateEvent[RTP_CORE_LAST_EVENT];

extern voiceDspCoreStateEventFprtTy voiceDspCoreStateEvent[DSP_LAST_STATE][DSP_CORE_LAST_EVENT];

/*#define ZYXEL_PORTING	1*/
#define MM_CHANNEL_PATH "/var/icf_mm.chanl"
#define ICF_ES_CHANNEL_PATH "/var/iptk_es.chanl"
#define VOICEAPP_MM_CHANNEL_PATH "/var/voiceApp_mm.chanl"
#define CONFIG_CHANNEL_PATH "/var/config.chanl"

#endif
