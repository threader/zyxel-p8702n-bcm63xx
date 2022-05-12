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


#ifndef __MM_CFG_TYPES_H__
#define __MM_CFG_TYPES_H__


#include "mm_port_wrapper.h"
#include "mm_macros.h"
#include "mm_core_fsm_defs.h"

typedef struct mmRtpPortRangePool_s
{
	icf_uint16_t portNum;
	struct mmRtpPortRangePool_s* next;
} mmRtpPortRangePool_t;

/* 
 * This structure will be used to fill information from config file
 * which will be utilised to fire configuration and registration API's 
 * towards IPTK*/

typedef struct
{
//Ssu-Ying, support Multiple Profile
	icf_cfg_set_self_ip_req_st	self_ip[MAX_SIP_LINE_COUNT];
	icf_cfg_codec_info_req_st	codec_info[MAX_SIP_LINE_COUNT];
	icf_cfg_ringer_tones_req_st	ringer_tones;
	#if 1 /*Eason, for per-line port range config*/
	icf_cfg_set_port_range_req_st	port_range[MAX_SIP_LINE_COUNT];
	#else
	icf_cfg_set_port_range_req_st	port_range;
	#endif

	icf_cfg_media_req_st	media;
	icf_cfg_jbm_mgmt_req_st	jbm_mgmt;
	icf_cfg_set_digit_trans_req_st	digit_trans;
	icf_cfg_max_earp_vol_req_st	max_earp_vol;
	icf_cfg_max_mic_gain_req_st	max_mic_gain;
	icf_cfg_max_speaker_vol_req_st	max_speaker_vol;
	icf_cfg_set_remote_video_display_st	remote_video_display;
	icf_cfg_set_preview_video_display_st	preview_video_display;

	mmRtpPortRangePool_t*	p_portRangePool;

}mmCfgDb_t;


/* Function Prototypes for all the function are defined here
 */

/*******************************************************************************
 *
 * FUNCTION NAME : mmProcessMessage
 *
 * DESCRIPTION   : This function recieves and process the messages recieved
 *				   from IPTK 
 *
 * RETURNS       : mm_return_t
 *
 ******************************************************************************/

mm_return_t mmProcessMessage(icf_msg_st *);



#endif
