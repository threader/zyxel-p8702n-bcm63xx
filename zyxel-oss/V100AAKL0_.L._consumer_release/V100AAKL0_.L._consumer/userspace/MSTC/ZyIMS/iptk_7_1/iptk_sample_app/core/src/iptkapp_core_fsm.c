/****************************************************************************
 **
 ** FILENAME:		iptkapp_core_call_fsm.c
 **
 ** DESCRIPTION:	This is the main file of core and contains the state machine 
 **                 function for sample application
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 **	06-DEC-07	   Vidyut Gupta	 		Initial Creation
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

/***************************************************************************
* Include Files area
***************************************************************************/
#include"iptkapp_core_fsm_types.h"
#include"iptkapp_macros.h"
#include"iptkapp_port_wrapper.h"
#include"iptkapp_core_fsm_defs.h"
#include"iptkapp_core_prototypes.h"
#include"iptkapp_port_prototypes.h"
#include"icf_types.h"
#include"icf_api.h"
#include"iptkapp_glb.h"
#include"icf_defs.h"

/**************************************************************************
* Static Variables declaration and extern area
**************************************************************************/

/*************************************************************************
* The Static Function declaration area 
*************************************************************************/

/******************* for idle state ************************/
static iptkapp_return_t
iptkapp_core_idle_make_call_pa(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

static iptkapp_return_t
iptkapp_core_idle_incoming_call_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

/****************** for progress state *******************/
static iptkapp_return_t
iptkapp_core_prog_alert_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

static iptkapp_return_t
iptkapp_core_prog_term_call_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

static iptkapp_return_t
iptkapp_core_prog_term_call_pa(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

/*********************** for alerted state *****************/
static iptkapp_return_t
iptkapp_core_alerted_accept_call_pa(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

static iptkapp_return_t
iptkapp_core_alerted_connect_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

static iptkapp_return_t
iptkapp_core_alerted_term_call_pa(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);

static iptkapp_return_t
iptkapp_core_alerted_term_call_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);
/************************* for Connected state **********************/


/************************* for terminated state *********************/
static iptkapp_return_t
iptkapp_core_termd_term_call_resp_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);
/*****************************common ignore function *****************************************/

static iptkapp_return_t
iptkapp_core_ignore(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err);
/***********************************************************************/

/* State-Event Table for CORE CALL */

iptkapp_core_state_event_fprt_ty  iptkapp_core_state_event[CORE_LAST_STATE][CORE_LAST_EVENT] =
{
	/* IDLE state */
	{
		iptkapp_core_idle_make_call_pa,		/* make call from pa */
		iptkapp_core_ignore,					/* accept call from pa */
		iptkapp_core_ignore,					/* terminate call from pa*/
		iptkapp_core_idle_incoming_call_cc,  /* incoming call cc */
		iptkapp_core_ignore,					/* alert call cc */
		iptkapp_core_ignore,					/* connect call from cc */
		iptkapp_core_ignore,					/* terminate call from cc */
		iptkapp_core_ignore					/* terminate call response cc*/
	},

	/* PROGRESS state */ 
	{
		iptkapp_core_ignore,						/* make call from pa */
		iptkapp_core_ignore,						/* accept call from pa */
		iptkapp_core_prog_term_call_pa,			/* terminate call from pa*/
		iptkapp_core_ignore,				 		/* incoming call cc */
		iptkapp_core_prog_alert_cc,				/* alert call cc */
		iptkapp_core_ignore,				/* connect call from cc */
		iptkapp_core_prog_term_call_cc,			/* terminate call from cc */
		iptkapp_core_ignore						/* terminate call response cc*/
	},

	/* ALERTED state */
	{
		iptkapp_core_ignore,						/* make call from pa */
		iptkapp_core_alerted_accept_call_pa,		/* accept call from pa */
		iptkapp_core_alerted_term_call_pa,		/* terminate call from pa*/
		iptkapp_core_ignore,						/* incoming call cc */
		iptkapp_core_ignore,						/* alert call cc */
		iptkapp_core_alerted_connect_cc,			/* connect call from cc */
		iptkapp_core_alerted_term_call_cc,		/* terminate call from cc */
		iptkapp_core_ignore						/* terminate call response cc*/
	},

	/* CONNECTED state */
	{
		iptkapp_core_ignore,						/* make call from pa */
		iptkapp_core_ignore,						/* accept call from pa */
		iptkapp_core_prog_term_call_pa,			/* terminate call from pa*/
		iptkapp_core_ignore,						/* incoming call cc */					
		iptkapp_core_ignore,						/* alert call cc */					
		iptkapp_core_ignore,						/* connect call from cc */					
		iptkapp_core_prog_term_call_cc,			/* terminate call from cc */
		iptkapp_core_ignore						/* terminate call response cc*/
	},
	
	/* TERMINATED state */
	{
		iptkapp_core_ignore,						/* make call from pa */
		iptkapp_core_ignore,						/* accept call from pa */
		iptkapp_core_ignore,						/* terminate call from pa*/
		iptkapp_core_ignore,						/* incoming call cc */		
		iptkapp_core_ignore,						/* alert call cc */			
		iptkapp_core_ignore,						/* connect call from cc */	
		iptkapp_core_prog_term_call_cc,			/* terminate call from cc */
		iptkapp_core_termd_term_call_resp_cc	,	/* terminate call response cc*/
	}

};


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_idle_make_call_pa
*
*  DESCRIPTION    : This function receives the incoming call indication from 
*					the UIM in the idle state. Enrty point function for outgoing
*					calls
*
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
*************************************************************************/
iptkapp_return_t
iptkapp_core_idle_make_call_pa(
  		icf_void_t						     *p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)

{
	
	iptkapp_core_call_context_list_st			*p_new_call_ctx = IPTKAPP_NULL;
	icf_msg_st									*p_msg = IPTKAPP_NULL;
	icf_call_id_t								call_id = IPTKAPP_DEFAULT_CALL_ID;
	
    p_call_context_node = p_call_context_node;

	if(IPTKAPP_NULL == p_input_param )
	{
		return IPTKAPP_FAILURE;
	}

    /* get the memory to create the call context */
	if(IPTKAPP_FAILURE == iptkapp_core_create_new_call_context(&p_new_call_ctx))
	{
		iptkapp_port_free(p_msg);
		return IPTKAPP_FAILURE;
	}

    iptkapp_port_strcpy((icf_int8_t *)((icf_string_st *)p_new_call_ctx->call_context.remote_party_addr.addr_val.str),
                        ((icf_int8_t *)((icf_string_st *)p_input_param)->str));
    p_new_call_ctx->call_context.remote_party_addr.addr_val.str_len = ((icf_string_st*)p_input_param)->str_len;
	
	p_new_call_ctx->call_context.remote_party_addr.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
	
	
	if(iptkapp_core_fill_create_call_api(p_input_param,call_id,p_err) == IPTKAPP_FAILURE)

	{  
        /* Create Call API Failed*/
		iptkapp_core_remove_call_context(p_new_call_ctx->call_context.call_id);
		return IPTKAPP_FAILURE;
	}
	
	p_new_call_ctx->call_context.call_id =	 call_id;
	p_new_call_ctx->call_context.call_type = CORE_OUTGOING_CALL;
	p_new_call_ctx->call_context.state =		CORE_PROGRESS;
	
	return IPTKAPP_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_idle_incoming_call_cc
*
*  DESCRIPTION    : This function receives the incoming call indication from 
*					the CC and informs the UIM about it
*
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
*************************************************************************/
iptkapp_return_t
iptkapp_core_idle_incoming_call_cc(
  		icf_void_t						    *p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					    *p_err)
{

	icf_incoming_call_ind_st			*p_incoming_call_info = IPTKAPP_NULL;
	icf_uint16_t						call_id = IPTKAPP_DEFAULT_CALL_ID;
	icf_msg_st							*p_msg_alert_to_cc = IPTKAPP_NULL;
	iptkapp_core_call_context_list_st		*p_new_call_ctx = IPTKAPP_NULL;
    icf_int8_t                              choice = 'y';
	
	if(IPTKAPP_NULL == p_input_param)
	{
		return IPTKAPP_FAILURE;
	}
    p_call_context_node = p_call_context_node;
	p_incoming_call_info = (icf_incoming_call_ind_st*)
							(IPTKAPP_MSG_GET_PAYLOAD(p_input_param));

	
	call_id	= IPTKAPP_MSG_GET_CALL_ID(p_input_param);
	/* get the memory to create the call context */
	if(iptkapp_core_create_new_call_context(&p_new_call_ctx)
									 == IPTKAPP_FAILURE)
	{
		iptkapp_port_free(p_msg_alert_to_cc);
		return IPTKAPP_FAILURE;
	}
	
	/* initialize the call context */
	p_new_call_ctx->call_context.call_id	=	call_id;
	p_new_call_ctx->call_context.call_type = CORE_INCOMING_CALL;

	/* check whether the calling party address present */
	if((p_incoming_call_info->bit_mask & 
		ICF_CALLING_PARTY_ADD_PRESENT))
	{
		/* put the address in the call context */
		p_new_call_ctx->call_context.remote_party_addr.addr_type = p_incoming_call_info->calling_party_addr.addr_type;
		p_new_call_ctx->call_context.remote_party_addr.addr_val.str_len = p_incoming_call_info->calling_party_addr.addr_val.str_len; 
		iptkapp_port_strncpy((icf_int8_t *)p_new_call_ctx->call_context.remote_party_addr.addr_val.str,
                          (icf_int8_t *)p_incoming_call_info->calling_party_addr.addr_val.str,
                          p_incoming_call_info->calling_party_addr.addr_val.str_len);
		p_new_call_ctx->call_context.remote_party_addr.addr_val.str[p_new_call_ctx->call_context.remote_party_addr.addr_val.str_len]=0;
		iptkapp_core_add_type_to_addr(&(p_new_call_ctx->call_context.remote_party_addr));
	}

	else
	{	
		/*calling party address was not there put the anonymous*/

		iptkapp_port_strcpy((icf_int8_t *)p_new_call_ctx->call_context.remote_party_addr.addr_val.str,(icf_int8_t *)"Anonymous");
		p_new_call_ctx->call_context.remote_party_addr.addr_val.str_len = iptkapp_port_strlen("Anonymous");
		p_new_call_ctx->call_context.remote_party_addr.addr_type = ICF_ADDRESS_PLAN_RESTRICTED_TO_BE_PRESENTED;
	
	}

    printf("\n\tYou have an incoming call from %s\n",p_new_call_ctx->call_context.remote_party_addr.addr_val.str);

	
    /* send the user alerted to the IPTK */
	if(iptkapp_core_fill_local_user_alerted_req_api(call_id,p_err)==IPTKAPP_FAILURE)
	{
			return IPTKAPP_FAILURE;
	}

	p_new_call_ctx->call_context.state = CORE_ALERTED;

    /* send the incoming call indication to CLI */
    printf("\n\t Do you want to accept the call (y/n)  :: ");
    choice = getc(stdin);
    if (choice == '\n' || choice == '\r')
        choice = 'y';
    else
        getc(stdin);

    if(choice == 'n' || choice == 'N' )
    {
        /* send the ternimate call request here */
        iptkapp_core_call_entry(IPTKAPP_NULL,CORE_TERMINATE_CALL_CLI,p_err);
    }
    else
    {
        /* accept the call */
        iptkapp_core_call_entry(IPTKAPP_NULL,CORE_ACCEPT_CALL_CLI,p_err);

    }

	return IPTKAPP_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_prog_alert_cc
*
*  DESCRIPTION    : This function handles the alert indication from the cc
*					in the progressing state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/	

iptkapp_return_t
iptkapp_core_prog_alert_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)

{	
	/* this is for outgoing call */	
	icf_remote_user_alerted_ind_st		*p_msg_alert = IPTKAPP_NULL;
    p_err = p_err;

	/* TO be implemented : ask the media manager to play ringback */

    if(IPTKAPP_NULL == p_input_param)
	{
		return IPTKAPP_FAILURE;
	}
	p_msg_alert = (icf_remote_user_alerted_ind_st*)(IPTKAPP_MSG_GET_PAYLOAD(p_input_param));
    /* send the alert indication to CLI */

	p_call_context_node->call_context.state = CORE_ALERTED;
	
	return IPTKAPP_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_prog_term_call_cc
*
*  DESCRIPTION    : This function handles the terminate call indiaction 
*					from the cc in the progressing state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/	
iptkapp_return_t
iptkapp_core_prog_term_call_cc(
		icf_void_t						*p_input_param, 
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{

	icf_terminate_call_ind_st				*p_msg_term_call = IPTKAPP_NULL;

	if(IPTKAPP_NULL == p_input_param)
	{
		return IPTKAPP_FAILURE;
	}
	
	p_msg_term_call = (icf_terminate_call_ind_st*)
						(IPTKAPP_MSG_GET_PAYLOAD(p_input_param));
	
    if(iptkapp_core_fill_terminate_call_ind_api(
							   p_call_context_node->call_context.call_id,
							   p_err)==IPTKAPP_FAILURE)
	{
		/* raise the alarm.. unable to send term call resp to CC
		After some time cc will time out and remove the call context*/
		return IPTKAPP_FAILURE;
	}

    printf ("\n\t Call with %s has been terminated \n",\
            p_call_context_node->call_context.remote_party_addr.addr_val.str);

    iptkapp_core_remove_call_context(p_call_context_node->call_context.call_id); 
	return IPTKAPP_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_prog_term_call_pa
*
*  DESCRIPTION    : This function handles the terminate call indiaction 
*					from the UIM in the progressing state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/
iptkapp_return_t
iptkapp_core_prog_term_call_pa(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{
	
    icf_call_clear_reason_t                         call_clear_reason;
    
    p_input_param = p_input_param;

	if(p_call_context_node->call_context.state == CORE_ALERTED &&
		p_call_context_node->call_context.call_type == CORE_INCOMING_CALL)
		
	{
		call_clear_reason = ICF_CALL_CLEAR_REASON_CALL_ABANDONED;
	}
	
	else
	{
		call_clear_reason = ICF_CALL_CLEAR_REASON_UNDEFINED;
		
	}

	if(iptkapp_core_fill_terminate_call_req_api(
							   p_call_context_node->call_context.call_id,
                               call_clear_reason,
							   p_err)==IPTKAPP_FAILURE)
	{
		return IPTKAPP_FAILURE ;
	}
	
	/* we will not remove the call context here it will be removed
     * once we got the confirmation from IPTK */
    p_call_context_node->call_context.state = CORE_TERMINATED;

	return IPTKAPP_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_alerted_connect_cc
*
*  DESCRIPTION    : This function handles the Connect Indication 
*					from the CC in the Alerted state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/
iptkapp_return_t
iptkapp_core_alerted_connect_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{
	
	icf_connect_ind_st *			p_msg_con = IPTKAPP_NULL;
    p_err = p_err;
    printf("\n\tIPTKAPP     >  Call is Connected \n");

	if(IPTKAPP_NULL == p_input_param )
	{
		return IPTKAPP_FAILURE;
	}
	
	p_msg_con = ((icf_connect_ind_st*)((icf_msg_st*)p_input_param)->payload);

	p_call_context_node->call_context.state = CORE_CONNECTED;
    /* send the call connnect indication to CLI */
	return IPTKAPP_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_alerted_accept_call_pa
*
*  DESCRIPTION    : This function handles the accept call indiaction 
*					from the UIM in the progressing state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/
iptkapp_return_t
iptkapp_core_alerted_accept_call_pa(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{

	p_call_context_node = p_call_context_node;

	if(iptkapp_core_fill_connect_call_req_api(
							   p_call_context_node->call_context.call_id,
							   p_err)==IPTKAPP_FAILURE)
	{
		/* Unable to send the connect to the CC */
		return IPTKAPP_FAILURE;
	}
	p_call_context_node->call_context.state = CORE_CONNECTED;

    /* To Remove Warning */
    p_input_param = p_input_param;

	return IPTKAPP_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_alerted_term_call_pa
*
*  DESCRIPTION    : This function handles the terminate call indiaction 
*					from the UIM in the alerted state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/
iptkapp_return_t
iptkapp_core_alerted_term_call_pa(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{


	/* call the same function that CORE uses to terminate the call in progress state */
		
	if(iptkapp_core_prog_term_call_pa(p_input_param,
								p_call_context_node,
								p_err) == IPTKAPP_FAILURE)
	{
		return IPTKAPP_FAILURE;
	}
	
	return IPTKAPP_SUCCESS;

}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_alerted_term_call_cc
*
*  DESCRIPTION    : This function handles the terminate call indiaction 
*					from the CC in the alerted state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/
iptkapp_return_t
iptkapp_core_alerted_term_call_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{
	/* call the same function that CORE uses to terminate the call in progress state */
	return(iptkapp_core_prog_term_call_cc(p_input_param,p_call_context_node,p_err));
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_termd_term_call_resp_cc
*
*  DESCRIPTION    : This function handles the terminate call respose 
*					from the CC in terminated state
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/
iptkapp_return_t
iptkapp_core_termd_term_call_resp_cc(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{
	p_err = p_err;
    p_input_param = p_input_param;
	/* delete the call context */
	 
    iptkapp_core_remove_call_context(p_call_context_node->call_context.call_id);
     /* inform the cli that call has been terminated */
	return IPTKAPP_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_ignore
*
*  DESCRIPTION    : This function will simply ignore the message received
*	
*  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE		 	
*************************************************************************/
iptkapp_return_t
iptkapp_core_ignore(
		icf_void_t						*p_input_param,
		iptkapp_core_call_context_list_st	*p_call_context_node,
		iptkapp_error_t					*p_err)
{
    p_input_param = p_input_param;
    p_err = p_err;
    p_call_context_node = p_call_context_node;
	return IPTKAPP_FAILURE;
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_create_new_call_context
*
*  DESCRIPTION    : The function creates the new call context and initialize it.
*					Also this fucntion makes the entry of this new context in the
*					existing call context list
*
*  RETURNS		  : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
*************************************************************************/

iptkapp_return_t
iptkapp_core_create_new_call_context(
	iptkapp_core_call_context_list_st	**pp_new_call_context)
{
	
	iptkapp_core_call_context_list_st			*p_temp_call_context = IPTKAPP_NULL;
	
	*pp_new_call_context = iptkapp_port_malloc(sizeof(iptkapp_core_call_context_list_st));
	if(IPTKAPP_NULL == pp_new_call_context)
	{
		return IPTKAPP_FAILURE;
	}
	
	(*pp_new_call_context)->next = IPTKAPP_NULL;
	
    if(IPTKAPP_NULL == p_call_context_list)
	{
		p_call_context_list = *pp_new_call_context;
	}
	
    else
	{
		for(p_temp_call_context = p_call_context_list;
			p_temp_call_context->next != IPTKAPP_NULL; p_temp_call_context = p_temp_call_context->next);
			
			p_temp_call_context->next = *pp_new_call_context;
	}
	return IPTKAPP_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_add_type_to_addr
*
*  DESCRIPTION    : The function will take sip address whithout any sip: or
*					tel: and then depending on type that is present in the 
*					address it will prepend the sip: or tel:
*
*  RETURNS		  : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
*************************************************************************/

iptkapp_return_t
iptkapp_core_add_type_to_addr(icf_address_st *sip_addr)
{
	icf_int8_t		*temp_addr;
	icf_uint16_t    size;

	size  = iptkapp_port_strlen((const icf_int8_t *)sip_addr->addr_val.str);

	size = size +5;
	
	temp_addr  = (icf_int8_t*)iptkapp_port_malloc(size);
	if(temp_addr == IPTKAPP_NULL)
	{
		return IPTKAPP_FAILURE;
	}

	if(sip_addr->addr_type == ICF_ADDRESS_PLAN_SIP_URI)
	{
		iptkapp_port_strcpy(temp_addr,(icf_int8_t *)"sip:");
		iptkapp_port_strcat(temp_addr, (icf_int8_t *)sip_addr->addr_val.str);
		iptkapp_port_strcpy((icf_int8_t *)sip_addr->addr_val.str, temp_addr);


	}
	else if(sip_addr->addr_type == ICF_ADDRESS_PLAN_TEL_URI)
	{
		iptkapp_port_strcpy(temp_addr,"tel:");
		iptkapp_port_strcat(temp_addr, (icf_int8_t *)sip_addr->addr_val.str);
		iptkapp_port_strcpy((icf_int8_t *)sip_addr->addr_val.str, temp_addr);
	}
	else
	{ /* make it as a sip: address */
		sip_addr->addr_type = ICF_ADDRESS_PLAN_SIP_URI;
		iptkapp_port_strcpy(temp_addr,(icf_int8_t *)"sip:");
		iptkapp_port_strcat(temp_addr, (icf_int8_t *)sip_addr->addr_val.str);
		iptkapp_port_strcpy((icf_int8_t *)sip_addr->addr_val.str, temp_addr);
	
	}

	iptkapp_port_free(temp_addr);
	return IPTKAPP_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_remove_type_from_addr
*
*  DESCRIPTION    : The function will remove sip: or tel: from the URI received
*
*  RETURNS		  : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
*************************************************************************/

iptkapp_return_t
iptkapp_core_remove_type_from_addr(icf_uint8_t *p_addr)
{
	icf_int8_t		*p_mem_pointer;
	icf_uint16_t		len=0;
	icf_int8_t		*temp_str;
	
	/* validate calling party's address is valid sip url or not
     */
    if(IPTKAPP_FAILURE == iptkapp_validate_addr((icf_int8_t *)p_addr))
        return IPTKAPP_FAILURE;

    
    for(p_mem_pointer = (icf_int8_t *)p_addr; *p_mem_pointer != ':'; ++p_mem_pointer)
    {
        len ++;
        if (len == 3)
            break;
    }

    len = 0 ;
	
	++p_mem_pointer;
	/* this is to remove the space */
	while(*p_mem_pointer == ' ')
	{
		++p_mem_pointer;
	}
	len = iptkapp_port_strlen(p_mem_pointer);
	len++;
	temp_str = (icf_int8_t*)iptkapp_port_malloc(len);
	
	if(IPTKAPP_NULL == temp_str)
	{
		return IPTKAPP_FAILURE;
	}
    iptkapp_port_strcpy(temp_str,p_mem_pointer);
	iptkapp_port_strcpy((icf_int8_t *)p_addr, temp_str);
	iptkapp_port_free(temp_str);
	return IPTKAPP_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_remove_call_context
*
*  DESCRIPTION    : The function will remove call context
*
*  RETURNS		  : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
*************************************************************************/

iptkapp_return_t
iptkapp_core_remove_call_context(
							icf_call_id_t 	call_id)
{
	iptkapp_core_call_context_list_st *p_prev_node_to_delete = IPTKAPP_NULL, *p_temp_pointer = IPTKAPP_NULL;
	
	for(p_temp_pointer = p_call_context_list; 
		p_temp_pointer != IPTKAPP_NULL && p_temp_pointer->call_context.call_id != call_id;
		p_temp_pointer = p_temp_pointer->next)
	{
			p_prev_node_to_delete = p_temp_pointer;
	}

	if(p_prev_node_to_delete == IPTKAPP_NULL && p_temp_pointer->call_context.call_id == call_id)
	{
		/* delete the very first node */
		p_call_context_list = p_call_context_list->next;
		iptkapp_port_free(p_temp_pointer);
	}
	else if(p_temp_pointer->call_context.call_id == call_id)
	{
		p_prev_node_to_delete->next =  (p_prev_node_to_delete->next)->next;
		iptkapp_port_free(p_temp_pointer);
	}
	else
	{
		return IPTKAPP_FAILURE;
	}
	return IPTKAPP_SUCCESS;
}



/************************************************************************
 * NAME:          iptkapp_validate_addr 
 * DESCRIPTION:   Function to validate characters in address
 ***********************************************************************/

iptkapp_return_t iptkapp_validate_addr(icf_int8_t *addr)
{
    icf_uint16_t		count=0;
    
    icf_int8_t request_uri[IPTKAPP_MAX_REQUEST_URL_LEN];
    icf_int8_t          *token;

    token =addr;

    while(*token!=('\0'))
    {
        if((*token>47&&*token<58)
                ||(*token>64&&*token<91)
                ||(*token>96&&*token<123)
                ||*token==('@')
                ||*token==('.')
                ||*token==('_')
                ||*token==('-')
                ||*token==(':'))
        {
            count++;
        }
        else
        {
            return(IPTKAPP_FAILURE);
        }
        token++;
    }

    iptkapp_port_strncpy(request_uri,addr,4);
    request_uri[4]='\0';
    if (0 == iptkapp_port_strcmp(request_uri,"tel:"))
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > TEL url are not supported currently\n");
        return IPTKAPP_FAILURE;
        /*
        request_uri = iptkapp_port_strtok(addr,":@.");
        while(*request_uri!=('\0'))
        {
            if((*request_uri>47&&*request_uri<58)
                continue;
            else
                return IPTKAPP_FAILURE;
        }*/
    }

    else if (0 == iptkapp_port_strcmp(request_uri,"sip:"))
    {
        iptkapp_port_strcpy(request_uri,addr);
        token = iptkapp_port_strtok(request_uri,":@.");
        token = iptkapp_port_strtok(IPTKAPP_NULL,":@.");
        if (token != IPTKAPP_NULL)
        {
            token = iptkapp_port_strtok(IPTKAPP_NULL,":@.");
            if (token != IPTKAPP_NULL)
            {
                token = iptkapp_port_strtok(IPTKAPP_NULL,":@.");
                if (token != IPTKAPP_NULL)
                {
                    return IPTKAPP_SUCCESS;
                }
            }
        }
    }
    return(IPTKAPP_FAILURE);
}

