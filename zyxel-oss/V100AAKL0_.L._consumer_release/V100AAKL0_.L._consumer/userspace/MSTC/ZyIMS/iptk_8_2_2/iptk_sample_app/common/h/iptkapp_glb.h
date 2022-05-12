/***********************************************************************************
 *
 * File name        : iptkapp_glb.h
 *
 * Purpose          : The files provide the declaration for vaious global 
 *                    variables.	
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 07-Dec-2007  Amit Sikka   					Initial version
 * 11-Dec-2008  Vipul Varshney  SPR 17945       update for publish database
 *
 * Copyright (c) 2007 Aricent Inc . All Rights Reserved
 ****************************************************************************************/


#ifndef __IPTKAPP_GLB_H__
#define __IPTKAPP_GLB_H__

/* Inclusion of all the header files will go here
 */

#include "iptkapp_cfg_types.h"
#include "iptkapp_port_wrapper.h"
#include "iptkapp_core_fsm_types.h"

/*extern declaration for config database */
extern iptkapp_cfg_db_t g_iptkapp_cfg_db_t;

/* universal application id to be used in all APIs towards IPTK*/
extern icf_uint32_t	g_app_id;

/*socket and tty file descriptors */
extern icf_int32_t g_fd ;
extern icf_int32_t ttyfd ;
extern fd_set fdset;

/* pointer for call context*/
extern iptkapp_core_call_context_list_st *p_call_context_list;

/* function pointer table for call state machine */
extern iptkapp_core_state_event_fprt_ty  
iptkapp_core_state_event[CORE_LAST_STATE][CORE_LAST_EVENT];

/*extern declaration for publish database */
extern icf_publish_req_st g_iptkapp_publish_db;

#endif
