/*****************************************************************************
 **
 ** FILENAME:		iptkapp_core_fsm.h
 **
 ** DESCRIPTION:	This is the main file of va and contains the entry point 
 **					function for va.
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 **	23-MAY-05	   Vidyut Gupta					Initial Creation
 *****************************************************************************
 **		Copyright (c) 2006 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

#ifndef __IPTKAPP_CORE_FSM_TYPES_H__
#define __IPTKAPP_CORE_FSM_TYPES_H__

/***************************************************************************
* Include files area 
**************************************************************************/

#include"icf_types.h"

/************************************************************************
*All the typedefs should go here
************************************************************************/

/* the enum defines the type of call that FSM is handling*/
typedef enum
{
	CORE_INCOMING_CALL,
	CORE_OUTGOING_CALL
}iptkapp_core_call_type_et;

/* Following enum defines the events handled in CORE call FSM */
typedef enum
{
	CORE_MAKE_CALL_CLI,
	CORE_ACCEPT_CALL_CLI,
	CORE_TERMINATE_CALL_CLI,
	CORE_INCOMING_CALL_CC,
	CORE_ALERT_CC,
	CORE_CONNECT_CC,
	CORE_TERMINATE_CALL_CC,
	CORE_TERMINATE_CALL_RESP_CC,
	CORE_LAST_EVENT
}iptkapp_core_event_et;

/* following enum defines the states of the CORE call FSM */
typedef enum
{
	CORE_IDLE,
	CORE_PROGRESS,
	CORE_ALERTED,
	CORE_CONNECTED,
	CORE_TERMINATED,
	CORE_LAST_STATE
}iptkapp_core_state_et;

/* following is the structure to keep the call context */
typedef struct
{
	icf_boolean_t				is_playing_ringing;	/* ring-tone played */
	icf_boolean_t				is_playing_ring_back; /* ring-back tone played */
	icf_call_id_t				call_id;  /* key to identify the FSM */
	iptkapp_core_state_et			state;   /* state of the FSM */
	iptkapp_core_call_type_et		call_type; /* type of call incoming or outgoing*/
	icf_address_st			remote_party_addr;
}iptkapp_core_call_context_st;

/* Link list to keep the call context */

typedef struct iptkapp_core_call_context_list_tg
{
	iptkapp_core_call_context_st			call_context;
	struct iptkapp_core_call_context_list_tg		*next;
}iptkapp_core_call_context_list_st;

typedef struct 
{
    icf_string_st       calling_party_addr;

}iptkapp_core_call_data_st;



typedef icf_return_t (*iptkapp_core_state_event_fprt_ty)
						(icf_void_t *input_param,
                         iptkapp_core_call_context_list_st	*p_call_context_node,
                         icf_error_t	*err);

#endif
