/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_init.c
 *
 * Purpose          : This file contains the function for creation of global
 *                    registration data including registration FSM.
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 23-Dec-2004  Jalaj Negi      Non Service LLD      Initial
 * 02-Dec-2005	Jyoti Narula	ICF 4.0				Changes under ICF_4_0_CRM_START/END
 * 07-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD     Coding for IPSec support
 *
 * 28-Nov-2006  Deepti Goyal    ICF 6.2             NAT "rport" Feature Support
 * 13-MAr-2007  Deepti Goyal    SPR 14598           Rport handling in case of 
 *                                                  401
 * 10-Mar-2008  Divij Agarwal   SPR 18122           Async AKA negotiation
 * 22-Jul-2009  Anuradha Gupta  SPR 18940           Modified fn icf_regm_init
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_common_prototypes.h"
#include "icf_regm_prototypes.h"
#include "icf_regm_macro.h"

/****************************************************************************
 * FUNCTION:            icf_regm_init
 *
 * DESCRIPTION:         This function is called by the DBM at init time so as
 *                      to allow RGM to create its global data.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/

icf_return_t icf_regm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t **p_data,
        INOUT icf_error_t *p_ecode)
{
    
    icf_void_t *p_buff = ICF_NULL;
    icf_rgm_glb_data_st *p_rgm_glb_data;
    icf_uint16_t size;
    icf_return_t ret_val;
    icf_uint16_t count = 0;

    /* function entry trace */
    ICF_FUNCTION_ENTER(p_glb_pdb) ;
        
    size = ICF_PORT_SIZEOF(icf_rgm_glb_data_st);
 
    /* Allocate Memory for Registration global data */
    ICF_STATIC_MEMGET(p_glb_pdb,
            size,
            p_buff,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val);
    
    /* p_buff is pointer to the Registration global data */
    /* Typecast p_buff to pointer to registration global data structure */
    p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_buff);
    
    /* Call function to initialize Registration FSM */
    ret_val = icf_rgm_initialize_fsm(p_rgm_glb_data);
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    p_rgm_glb_data->reg_ongoing = ICF_FALSE;
    p_rgm_glb_data->p_reg_event_subs_ctx = ICF_NULL;
#ifdef ICF_IPSEC_ENABLED
    /* Reset the list of locally supported security algorithms */
    p_rgm_glb_data->p_ue_algo_list = ICF_NULL;
    p_rgm_glb_data->p_sa_resp_awaiting = ICF_NULL;
#endif /* ICF_IPSEC_ENABLED */
    p_rgm_glb_data->last_buffer_id = 0;

ICF_CHECK_IMS_END
#endif /*IMS_CLIENT*/

    /* SPR 18940: Intialise the ongoing reg counter for each
     * line
     */ 
    for (count = 0;count < ICF_MAX_NUM_OF_LINES;count++)
    {
        p_rgm_glb_data->reg_count_for_line[count] = 0;
    }

    *p_data = p_buff;

    /* function exit trace */
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}

/****************************************************************************
 * FUNCTION:            icf_regm_deinit
 *
 * DESCRIPTION:         This function is called by the DBM at so as
 *                      to allow RGM to free its global data.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_void_t icf_regm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode)
{
    icf_return_t             ret_val = ICF_SUCCESS;
#ifdef ICF_IPSEC_ENABLED
    icf_rgm_glb_data_st	    *p_rgm_glb_data = ICF_NULL;
#endif

    ICF_FUNCTION_ENTER(p_glb_pdb)

#ifdef ICF_IPSEC_ENABLED

    /* free the security algorithm list stored in NM global data */
    p_rgm_glb_data = (icf_rgm_glb_data_st *)(p_data);
    
    if (ICF_NULL != p_rgm_glb_data->p_ue_algo_list)
    {
        ICF_STATIC_MEMFREE(
            p_glb_pdb,
            p_rgm_glb_data->p_ue_algo_list,
            p_ecode,
            ret_val)
    }

#endif /* ICF_IPSEC_ENABLED */


    ICF_STATIC_MEMFREE(p_glb_pdb, p_data, p_ecode, ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)
}


/****************************************************************************
 * FUNCTION:            icf_rgm_initialize_fsm
 *
 * DESCRIPTION:         This function is called by icf_rgm_init to 
 *                      initialize the Registration FSM.
 *
 *
 * RETURNS:             
 *
 ***************************************************************************/
icf_return_t icf_rgm_initialize_fsm(
        OUT icf_rgm_glb_data_st *p_rgm_glb_data)
{
    icf_return_t ret_val;

    /* Initializing for IDLE STATE */
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;

ICF_4_0_CRM_START
    
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_00_reg_req_recvd;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
		[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_ignore;
			
#ifdef ICF_IPSEC_ENABLED
		p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ignore;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_ignore;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_SA_DELETE_RESP] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_ignore;
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_01_03_nat_binding_chng;
#endif
ICF_4_0_CRM_END
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_00_IDLE]
        [ICF_RGM_AUTH_RESP]= icf_rgm_ignore;

    /* Initializing for REGISTERING STATE */
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_01_04_success_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_01_04_failure_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_01_04_fail_no_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_01_auth_req_resp;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_01_04_dur_short_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;

ICF_4_0_CRM_START
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_01_reg_req_recvd;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
		[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_01_04_timeout;
	
#ifdef ICF_IPSEC_ENABLED
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_01_sa_delete_timer_exp;
#else
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_retry_timer_exp;
#endif
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_01_03_04_05_nw_deactivated;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_ignore;
    			
#ifdef ICF_IPSEC_ENABLED
		p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ignore;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_ignore;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
		[ICF_RGM_SA_DELETE_RESP] = icf_rgm_01_sa_delete_resp;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_ignore;
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_01_03_nat_binding_chng;
#endif

ICF_4_0_CRM_END
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_REGISTERING]
    [ICF_RGM_AUTH_RESP]= icf_rgm_ignore;

    /* Initializing for UNREGISTERED STATE */
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_ignore;

/* REL 5.0 starts */
/*#ifdef IMS_CLIENT*/
	/*ICF_CHECK_IMS_START(p_glb_data,ICF_TRUE)*/
     
	 p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_ignore;

/*	ICF_CHECK_IMS_END*/
/*#endif*/
	/* REL 5.0 ends */
   


    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_ignore;

	
#ifdef ICF_FEATURE_ABANDON /* Incase of ICF: this feature not supported */

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;
	
#else
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_02_duration_change;

#endif/* ICF: feature absent */

#ifdef ICF_FEATURE_ABANDON /* Incase of ICF: this feature not supported */
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;    
#else 
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_DELETE_USER] = icf_rgm_02_delete_user;
	
#endif/* ICF: feature absent */

ICF_4_0_CRM_START
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_02_reg_req_recvd;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
			[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_reattempt_timer_exp;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_02_nw_activated;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_ignore;

			
#ifdef ICF_IPSEC_ENABLED
		p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ignore;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_ignore;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_SA_DELETE_RESP] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_ignore;
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_ignore;
#endif

ICF_4_0_CRM_END

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_02_UNREGISTERED]
        [ICF_RGM_AUTH_RESP]= icf_rgm_ignore;
    
    /* Initializing for REGISTERED STATE */
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_ignore;

#ifdef ICF_FEATURE_ABANDON /* Incase of ICF: this feature not supported */
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;
    
#else
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_03_duration_change;
	
#endif

#ifdef ICF_FEATURE_ABANDON /* Incase of ICF: this feature not supported */
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;
    
#else
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_DELETE_USER] = icf_rgm_03_delete_user;	
	
#endif
ICF_4_0_CRM_START
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_03_reg_req_recvd;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
		[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_reg_timer_exp;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_01_03_04_05_nw_deactivated;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_03_notify_shortened;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_03_notify_terminated;
			
#ifdef ICF_IPSEC_ENABLED
		p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ignore;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_03_sa_modify_resp;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_SA_DELETE_RESP] = icf_rgm_03_sa_delete_resp;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_03_sa_expiry_ind;
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_01_03_nat_binding_chng;
#endif

ICF_4_0_CRM_END
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_03_REGISTERED]
        [ICF_RGM_AUTH_RESP]= icf_rgm_ignore; 

    /* Initializing for AUTHORIZING STATE */
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_01_04_success_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_01_04_failure_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_01_04_fail_no_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_04_auth_req_resp;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_01_04_dur_short_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;



ICF_4_0_CRM_START
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_04_reg_req_recvd;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
		[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_01_04_timeout;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_retry_timer_exp;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_01_03_04_05_nw_deactivated;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_ignore;
			
#ifdef ICF_IPSEC_ENABLED
		p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ignore;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_ignore;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_SA_DELETE_RESP] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_ignore;
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_04_nat_binding_chng;

#endif

ICF_4_0_CRM_END
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_04_AUTHORIZING]
        [ICF_RGM_AUTH_RESP]= icf_rgm_ignore;

    /* Initializing for DEREGISTERING STATE */
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_05_any_nw_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_05_any_nw_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_05_any_nw_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_05_auth_req_resp;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_05_any_nw_resp;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;

ICF_4_0_CRM_START
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_05_reg_req_recvd;

#ifdef ICF_NAT_RPORT_SUPPORT	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
                [ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_01_04_timeout;
#else	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
		[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_05_timeout;
#endif

#ifdef ICF_NAT_RPORT_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_01_04_timeout;
#else	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_retry_timer_exp;
#endif
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_01_03_04_05_nw_deactivated;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_ignore;
			
#ifdef ICF_IPSEC_ENABLED
		p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ignore;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_ignore;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_SA_DELETE_RESP] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_ignore;
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_05_nat_binding_chng;
#endif

ICF_4_0_CRM_END
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_05_DEREGISTERING]
        [ICF_RGM_AUTH_RESP]= icf_rgm_ignore;

#ifdef ICF_IPSEC_ENABLED

    /* FSM for IPSec related new states */

    /**** Initializing for Await SA create response ****/
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;
  
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
		[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_ipsec_01_sa_create_timer_exp;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_ignore;
			
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ipsec_01_sa_create_resp;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_ignore;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_SA_DELETE_RESP] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_ignore;
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_ignore;
#endif

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_01_IPSEC_AWT_SA_CREATE_RESP]
        [ICF_RGM_AUTH_RESP]= icf_rgm_ignore;
    
    /**** Initializing for Await SA delete response ****/
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;
    
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
		[ICF_RGM_REG_REQ_RECVD] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
		[ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_ignore;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_ipsec_07_sa_delete_timer_exp;
	
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_ignore;
			
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_SA_CREATE_RESP] = icf_rgm_ignore;
	
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_SA_MODIFY_RESP] = icf_rgm_ignore;
#endif

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_SA_DELETE_RESP] = icf_rgm_ipsec_07_sa_delete_resp;

	p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_SA_EXPIRY_IND] = icf_rgm_ignore;
#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_NAT_BINDING_CHNG_IND] = icf_rgm_ignore;
#endif 
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP]
        [ICF_RGM_AUTH_RESP]= icf_rgm_ignore;
#endif

    /* Initializing for AWT_AUTH_RESP STATE */
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_SUCCESS_RESPONSE] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_FAILURE_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_FAILURE_NO_REATTEMPT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_AUTHENTICATION_REQ] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_DURATION_SHORT] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_DURATION_CHANGE] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_DELETE_USER] = icf_rgm_ignore;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_REG_REQ_RECVD] = icf_rgm_06_reg_req_recvd;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_FAILURE_TIMEOUT] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_TIMER_EXPIRY] = icf_rgm_06_guard_timer_exp;
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_NW_DEACTIVATED] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_NW_ACTIVATED] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_NOTIFY_SHORTENED] = icf_rgm_ignore;

    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_NOTIFY_TERMINATED] = icf_rgm_06_notify_terminated;

#ifdef ICF_NAT_RPORT_SUPPORT
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_NAT_BINDING_CHNG_IND]= icf_rgm_ignore;
#endif
    
    p_rgm_glb_data->icf_rgm_state_machine[ICF_RGM_06_AWT_AUTH_RESP]
        [ICF_RGM_AUTH_RESP]= icf_rgm_06_user_auth_resp;
    
    ret_val = ICF_SUCCESS;
    return ret_val;
}
        
