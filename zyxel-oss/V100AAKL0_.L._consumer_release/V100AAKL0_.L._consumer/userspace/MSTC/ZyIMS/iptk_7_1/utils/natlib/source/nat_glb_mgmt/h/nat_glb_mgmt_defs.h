/******************************************************************************
*                                    
* FILE NAME    : nat_glb_mgmt_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the Constants used in NAT CLIENT
*                APIs.    
*                                    
* Revision History :                            
*
*    DATE            NAME             REFERENCE       REASON        
*   ------         ------------      ---------     ----------
* 12-Jul-2006	  Anuradha Gupta	Initial LLD  Initial Version
* 24-Nov-2006	  Anuradha Gupta	LLD			 new macros added
*
* Copyright 2006, Aricent             
*                                    
*******************************************************************************/


#ifndef __NAT_GLB_MGMT_DEFS_H__
#define __NAT_GLB_MGMT_DEFS_H__

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

#define NAT_STUN_MSG_LEN                    	512
#define NAT_STUN_DEFAULT_BIND_DISC_TIMER    	60
#define NAT_STUN_DEFAULT_NUM_OF_CONTEXTS		16
#define NAT_MAX_STR_LEN             			128
#define	NAT_CONF_MAX_LINE_SIZE		 			100
#define NAT_CONF_MAX_LINES						50
#define NAT_CONF_COMMENT_CHAR					'#'

#define NAT_DEFAULT_STUN_SERVER_PORT		    3478

#ifdef __cpluplus
}
#endif

#endif /* __NAT_GLB_MGMT_DEFS_H__ */
