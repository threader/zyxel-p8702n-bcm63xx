/****************************************************************************
 **
 ** FILENAME:		iptkapp_core_fsm_intf.c
 **
 ** DESCRIPTION:	This the innterface file for fsm. it contains the entry point
 **                 functions for fsm and event to API mapping related functionality
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 **	10-DEC-07	   Vidyut Gupta	 		Initial Creation
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/
#include "icf_api.h"
#include "iptkapp_core_fsm_types.h"
#include "iptkapp_macros.h"
#include "iptkapp_core_prototypes.h"
#include "iptkapp_glb.h"

iptkapp_core_call_context_list_st *p_call_context_list = IPTKAPP_NULL;

/***********************************************************************
*
*  FUNCTION NAME  : iptkapp_core_call_entry
*
*  DESCRIPTION    : This function will be called by the UIM to initiate 
*					a new call
*
*  RETURNS		  : Either DMP_SUCCESS or DMP_FAILURE
*************************************************************************/
icf_return_t
iptkapp_core_call_entry(iptkapp_core_call_data_st    *p_call_data,
                       iptkapp_core_event_et 	    event_from_cli,
				       icf_error_t					*p_err )
{

    switch(event_from_cli)
    {
        case CORE_MAKE_CALL_CLI:
            {

                /* IPTKAPP has same event for make call and accept call */
                if(p_call_context_list == ICF_NULL)
                {
                    if(iptkapp_core_state_event[CORE_IDLE][CORE_MAKE_CALL_CLI](p_call_data, ICF_NULL,
                                p_err)==ICF_FAILURE)
                    {
                        return ICF_FAILURE;
                    }
                }
                else
                {
                    return ICF_FAILURE;
                }
            }
            break;
		
        case CORE_ACCEPT_CALL_CLI :/* case for accept call */
        case CORE_TERMINATE_CALL_CLI :
		{
			if(ICF_NULL == p_call_context_list)
            {
                return ICF_FAILURE;
            }
            if(iptkapp_core_state_event[p_call_context_list->call_context.state][event_from_cli]
															(ICF_NULL,p_call_context_list,p_err)==ICF_FAILURE)
			{
				return ICF_FAILURE;
			}
		}
		break;

	default :
        return ICF_FAILURE;
	}
	return ICF_SUCCESS;
}




/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_msg_handler_cc
*
*  DESCRIPTION    : This function is called by the CORE main and, it receives
*					the message from the CC and will call the CALL FSM 
*					function of the CORE
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
iptkapp_core_msg_handler_cc(icf_msg_st *p_msg,icf_error_t *p_err)
{
	icf_api_id_t					    	api_id;
	icf_call_id_t						    call_id = IPTKAPP_DEFAULT_CALL_ID;
	icf_uint8_t						        event_from_icf;

	if(ICF_NULL == p_msg)
	{
		return ICF_FAILURE;
	}
	api_id  = IPTKAPP_MSG_GET_API(p_msg);
	call_id = IPTKAPP_MSG_GET_CALL_ID(p_msg);
		
	
	if(api_id == ICF_INCOMING_CALL_IND)
	{
		if(iptkapp_core_state_event[CORE_IDLE]
							  [CORE_INCOMING_CALL_CC](p_msg,ICF_NULL,p_err)==ICF_FAILURE)
		{
			/* raise the alarm */
			return ICF_FAILURE;
		}
		return ICF_SUCCESS;
	}
	
	/* get the event to be send to the FSM */
	 if(iptkapp_core_get_event(api_id,&event_from_icf) == ICF_FAILURE)
	 {
		 /* some undefined event is received from the ICF */
		 /* raise the alarm */
		 return ICF_SUCCESS;
	 }
		/* call the function of va call state event table */
		if(iptkapp_core_state_event[p_call_context_list->call_context.state][event_from_icf](p_msg,p_call_context_list,
													 p_err)==ICF_FAILURE)
		{
			return ICF_FAILURE;
		}
	return ICF_SUCCESS;

}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_get_event
*
*  DESCRIPTION    : 
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/


icf_return_t
iptkapp_core_get_event
		(icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf)
{	
	switch(api_id)
	{
		case ICF_INCOMING_CALL_IND:
			
			*p_event_from_icf = CORE_INCOMING_CALL_CC;
			break;

		case ICF_REMOTE_USER_ALERTED_IND :

			*p_event_from_icf = CORE_ALERT_CC;
			 break;


		case ICF_CONNECT_IND :
			
			*p_event_from_icf = CORE_CONNECT_CC;
	    	  break;

		case ICF_TERMINATE_CALL_IND :
			
			*p_event_from_icf = CORE_TERMINATE_CALL_CC;
			break;

		case ICF_REMOTE_USER_TERMINATE_CALL_RESP :
		
			*p_event_from_icf = CORE_TERMINATE_CALL_RESP_CC;
			break;
		
		case ICF_MEDIA_CONNECTED_IND:
            /* do nothing */
				break;
		
		default :
			return ICF_FAILURE;
			/* error condition raise the error*/
			
		}/* end of switch case statement */
		return ICF_SUCCESS;
}
