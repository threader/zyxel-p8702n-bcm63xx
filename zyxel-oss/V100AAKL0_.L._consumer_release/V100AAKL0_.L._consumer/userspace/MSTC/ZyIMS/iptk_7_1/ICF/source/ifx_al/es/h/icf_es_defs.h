/*------------------------------------------------------------------------------
 *
 * File name        : icf_es_defs.h
 *
 * Purpose          : This file contains the prototypes of all functions 
 *                    used by ES 
 *                    
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 14-Feb-2005  Pankaj Negi     Non Service LLD     Initial
 * 2-Jan-2007   Manoneet Khosla SPR 13087,13113	    
 *
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __ICF_ES_DEFS_H__
#define __ICF_ES_DEFS_H__


#include "icf_es_types.h"
#include "icf_feature_flags.h"


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

#define ICF_NULL 0    
#define ICF_NUM_MANDATORY_PARAMS	11
#define AUTO_ACTIVATE_PRESENT		0x01
#define AUTO_REGISTER_PRESENT  		0x02
#define OPERATION_MODE_PRESENT  	0x04 
#define AUTO_SCALE_PRESENT		0x08      
#define ICF_GENERIC_APP_IP_PRESENT	0x10 
#define ICF_GENERIC_APP_PORT_PRESENT	0x20 
#define ICF_MM_IP_PRESENT		0x40 
#define ICF_MM_PORT_PRESENT		0x80 
#define ICF_IPC_SELF_IP_PRESENT		0x100

#ifdef ICF_NAT_MEDIA_TRAVERSAL

#define ICF_NAT_IP_PRESENT			0x200

#define ICF_NAT_PORT				0x400

#endif

#define ICF_ES_CHANNEL_PATH "/var/iptk_es.chanl"
#define VOICEAPP_IPTK_CHANNEL_PATH "/var/voiceApp_iptk.chanl"
#define IPTK_SAMPLE_APP_CHANNEL_PATH "/var/sample_app.chanl"

#define ICF_MM_CHANNEL_PATH "/var/icf_mm.chanl"


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_ES_DEFS_H__ */    
