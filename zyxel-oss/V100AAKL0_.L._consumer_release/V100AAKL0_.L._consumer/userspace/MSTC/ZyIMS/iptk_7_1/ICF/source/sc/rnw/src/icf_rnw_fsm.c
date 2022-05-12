/*******************************************************************************
*
* FILE NAME    : icf_rnw_fsm.c
*
* DESCRIPTION  : Contains the definitions of all State Machine functions
*                of RNW service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 07-Mar-2005    Amit Sharma     None            Initial
* 27-Apr-2005    Amit Sharma                 Updations for REFER replaces_hdr
* 15-May-2005    Amit Sharma   SPR:7444      Update STATS
* 19-May-2005    Amit Sharma   SPR:7565      MEMFREE referred_to in rnw_rej req
* 19-May-2005    Amit Sharma   SPR:7594      NOTIFY is sent only when call_rel
*                                            received from new initiated call
* 19-May-2005    Amit Sharma   SPR:7595      call connect is checked in media
*                                            committed state
* 29-May-2005    Amit Sharma   SPR:7568      Forceful call clear in idle state
* 30-May-2005    Amit Sharma   SPR:7628      Remove memleak
* 24-Aug-2007    Amit Sharma                 IPTK merge from PQT6.5
* 08-Dec-2007    Abhishek Dhammawat SPR17667 Modified icf_rnw_07_cta_initiated_cfm
* Copyright 2006,Aricent .
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_rnw_types.h"
#include "icf_rnw_prototypes.h"
#include "icf_rnw_macro.h"
#include "icf_rnw_defs.h"
#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_sc_prototypes.h"

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_00_refer
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_00_IDLE
*                 Event         ICF_RNW_REFER
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_00_refer(
        INOUT  icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_call_ctx_st     *p_call_ctx = ICF_NULL;
    icf_call_ctx_st     *p_call_ctx_orig = ICF_NULL;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_error_t         ecode_temp = 0;
    icf_call_id_t       call_id = ICF_INVALID_CALL_ID;
    icf_nw_inc_refer_st *p_refer_req = ICF_NULL;
    icf_config_data_st   *p_config_data = ICF_NULL;


    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ret_val =  icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)
                ICF_INT_MODULE_CFG,
                (icf_void_t **)&p_config_data,
                p_ecode);

    /*Copy refer req msg pointer received from SSA*/
    p_refer_req =
        (icf_nw_inc_refer_st *)(p_pdb->p_internal_msg->p_msg_data);

    /*Initialize next state as invalid*/
    p_pdb->p_rnw_ctx->next_state = ICF_RNW_STATE_INVALID;

    ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NEXT_STATE)

    ICF_STAT(p_glb_pdb, ICF_STAT_NW_CC_REFER)
    

    ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_RECVD)
    
#ifdef ICF_TRACE_ENABLE
    if(p_refer_req->bitmask & ICF_NW_INC_REFER_REPLACE_CALL_ID)
     {
         p_pdb->replace_header_present=1;
         /*p_pdb->new_call_id=p_refer_req->p_replaces_hdr->replace_nw_call_id; */
         ICF_RNW_APP_TRACE(p_pdb, ICF_TRACE_REFER_RECVD)
     }
#endif
    p_call_ctx_orig = p_glb_pdb->p_call_ctx;
   
    /*If validation is failure then send REFER failure response and
     * return service ended normal*/
    if (ICF_FALSE == icf_rnw_validate_incoming_refer(p_pdb))
    {
        ret_val = icf_rnw_send_refer_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE);

        if (ICF_NULL != p_refer_req->p_referred_to)
        {
            /*Free replace_nw_call_id in SSA*/
            ICF_MEMFREE(p_glb_pdb, p_refer_req->p_referred_to,
                ICF_MEM_COMMON,
                p_ecode, ret_val)
        }

        ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        return ret_val;
    }
    /*Allocate New Call Context block*/
    else if (ICF_FAILURE == 
            icf_dbm_alloc_call_cntxt_blk(p_glb_pdb,
                &call_id,
                &p_call_ctx, p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /*Fill New Call Context data*/
    else if (ICF_FAILURE == icf_rnw_fill_call_ctx(p_pdb, p_call_ctx_orig))
    {
        /*Deallocate all the memory allocated for p_call_ctx*/
        ret_val = icf_rnw_clear_call_ctx(p_pdb, p_call_ctx);
        
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_rnw_alloc_n_fill_serv_ctx(p_pdb))
    {
        /*Deallocate all the memory allocated for p_call_ctx*/
        ret_val = icf_dbm_dealloc_srvc_cntxt_blk(p_glb_pdb,
                p_call_ctx->p_service_ctx, &ecode_temp);

        ret_val = ICF_FAILURE;
    }
    /*Indicate PA that REFER from network received*/
    else if (ICF_FAILURE == icf_rnw_send_refer_ind(p_pdb))
    {
        /*Deallocate all the memory allocated for p_call_ctx*/
        ret_val = icf_rnw_clear_call_ctx(p_pdb, p_call_ctx);

        ret_val = icf_dbm_dealloc_srvc_cntxt_blk(p_glb_pdb, 
                p_call_ctx->p_service_ctx, &ecode_temp);
        
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_rnw_start_rnw_timer(p_pdb))
    {
        /*Deallocate all the memory allocated for p_call_ctx*/
        ret_val = icf_rnw_clear_call_ctx(p_pdb, p_call_ctx);

          ret_val = icf_dbm_dealloc_srvc_cntxt_blk(p_glb_pdb,
                  p_call_ctx->p_service_ctx, &ecode_temp);
 
        ret_val = ICF_FAILURE;
    } /* else if (ICF_FAILURE == ipp... */
    else if (ICF_FALSE ==  p_config_data->conf_cfm_reqd_at_transferee)
    {
        /*Set return status as ICF_STATUS_SERVICE_MSG_TO_CC_SETUP*/
        ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MSG_TO_CC_SETUP)

        ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_NEW_CALL_ID)

        /*Wait for media committed in new call*/
        ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_01_AWT_MEDIA_COMMIT)

        p_pdb->p_rnw_ctx->next_state = ICF_RNW_04_AWT_FOR_CONNECT;

       if (ICF_SUCCESS == icf_rnw_send_refer_resp(p_pdb,
                        (icf_boolean_t)ICF_SUCCESS))
        { 
            /*Assign new call id in global pdb*/
            p_glb_pdb->p_call_ctx = p_call_ctx;
        }
        else
        {
            /*Deallocate all the memory allocated for p_call_ctx*/
            ret_val = icf_rnw_clear_call_ctx(p_pdb, p_call_ctx);

            ret_val = icf_dbm_dealloc_srvc_cntxt_blk(p_glb_pdb, 
                      p_call_ctx->p_service_ctx, &ecode_temp);

            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        /* Set the state as awt cfm from application */
        ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_07_AWT_FOR_CTA_INITIATED_CFM)

        p_pdb->p_rnw_ctx->next_state = ICF_RNW_07_AWT_FOR_CTA_INITIATED_CFM;
    }
    /*Send REFER response to SSA*/
    if (ICF_FAILURE == ret_val)
    {
        ret_val = icf_rnw_send_refer_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE);

        if (ICF_NULL != p_refer_req->p_referred_to)
        {
            /*Free replace_nw_call_id in SSA*/
            ICF_MEMFREE(p_glb_pdb, p_refer_req->p_referred_to,
                ICF_MEM_COMMON,
                p_ecode, ret_val)
        }

        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == ret_val) */


    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_00_refer */

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_refer
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*
*                 Event         ICF_RNW_REFER
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_xx_refer(
        INOUT  icf_rnw_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Send failure response to SSA, for INVALID REFER request*/
    ret_val = icf_rnw_send_refer_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE);
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_rnw_xx_refer */

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_01_media_committed
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                 Event         ICF_RNW_MEDIA_COMMITTED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_01_media_committed(
        INOUT  icf_rnw_pdb_st *p_pdb)
{
	icf_return_t    ret_val = ICF_SUCCESS;
	icf_error_t     *p_ecode = ICF_NULL;
	icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
	icf_config_data_st   *p_config_data = ICF_NULL;

	p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

	ICF_FUNCTION_ENTER(p_glb_pdb)

		p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

	if(ICF_FAILURE ==
			icf_dbm_get_module_glb_data(p_glb_pdb,
				(icf_module_id_t)(ICF_INT_MODULE_CFG),
				(icf_void_t **)&p_config_data, p_ecode))
	{
		ICF_PRINT(((icf_uint8_t * )"No global config data. Returning failure\n"));
		return ICF_FAILURE;
	}
    /*
     * The service logic context would be same for both the call
     * origionated because of REFER and the call on which refer was received.
     * Now for a NFS call if the MMS response for ACK is received the REFER 
     * message from network, we would end up processing the 
     * MEDIA_CONNECT_INDICATION for the origional call as if it would have been
     * received for the new call and hence generate a NOTIFY for REFER even 
     * before the new call gets connected.
     * To overcome this race check for the call for which 
     * MEDIA_CONNECT_INDICATION to be the new call. If it is not the new
     * call ignore the event.
     */
    if(p_pdb->p_rnw_ctx->p_call_ctx_c->call_id == 
		    p_glb_pdb->p_call_ctx->call_id)
    {
	if (ICF_NULL != p_glb_pdb->p_call_ctx->p_replaces_header)
	{
		/*Free replace nw call ID in call context*/
		ICF_MEMFREE(p_glb_pdb, p_glb_pdb->p_call_ctx->p_replaces_header,
				ICF_MEM_COMMON, p_ecode, ret_val)
	} /* if (ICF_NULL != p_glb_pdb->... */    


	/*Check if call is also in connected state send notify */
	if (ICF_NULL != 
			(ICF_CALL_CONNECT_DONE & p_glb_pdb->p_call_ctx->common_bitmask))
	{
		if (ICF_INT_MODULE_INVALID == 
				p_glb_pdb->p_call_ctx->signalling_ongoing)
		{
			ret_val = icf_rnw_send_notify_msg(p_pdb, 
					(icf_boolean_t)ICF_SUCCESS);

			p_pdb->p_rnw_ctx->next_state = ICF_RNW_06_AWT_NOTIFY_ACK;

			ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NEXT_STATE)
		}
		else
		{
			p_pdb->p_rnw_ctx->next_state = 
				ICF_RNW_05_AWT_START_PROC_SIGNALING;
                      		    
              	        ICF_RNW_SET_STATUS(p_pdb,
	       		ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
		} /* else */
	} /* (ICF_CALL_CONNECT_DONE & p_... */
	else
	{
		p_pdb->p_rnw_ctx->next_state = 
			ICF_RNW_05_AWT_START_PROC_SIGNALING;

		ICF_RNW_SET_STATUS(p_pdb, 
				ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
	} /* else */

	/*Check media ongoing in both calls*/
	/*If Media ongoing is set wait for start proc media*/
	/*Else send merge media request to MMI*/
	if (ICF_SUCCESS == ret_val)
	{
		if (ICF_TRUE == p_config_data->conf_reqd_at_transferee)
			ret_val = icf_rnw_check_med_for_merge_req(p_pdb);
		else
		{
			ICF_PRINT(((icf_uint8_t * )"Since conference is not configured so not doing anything"));
			ret_val  = ICF_SUCCESS;
		}
	}
        }
	ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;

} /* icf_rnw_01_media_committed */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_02_start_proc_media
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_02_AWT_START_PROC_MEDIA
*                 Event         ICF_RNW_START_PROC_MEDIA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_02_start_proc_media(
        INOUT   icf_rnw_pdb_st *p_pdb)
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_config_data_st   *p_config_data = ICF_NULL;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_error_t         ecode = ICF_ERROR_NONE;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb);

    /*Check if media is ongoing in other call, if yes then wait in the same 
     *state; otherwise send merge media request and go to merge media resp
     *state*/
    if(ICF_FAILURE ==
            icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)&p_config_data, &ecode))
    {
        ICF_PRINT(((icf_uint8_t * )"No global config data. Returning failure\n"));
        return ICF_FAILURE;
    }

    if (ICF_TRUE == p_config_data->conf_reqd_at_transferee)
        ret_val = icf_rnw_check_med_for_merge_req (p_pdb);
    else
    {
        ICF_PRINT(((icf_uint8_t * )"Since conference is not configured so not doing anything"));
        ret_val  = ICF_SUCCESS;
    }
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb);

    return ret_val;
} /* icf_rnw_02_start_proc_media */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_03_merge_media_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                 Event         ICF_RNW_MERGE_MEDIA_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_03_merge_media_resp (
        INOUT  icf_rnw_pdb_st *p_pdb)
{
    icf_return_t     ret_val = ICF_FAILURE;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    /*If any one of two calls are terminated or notify response
     * received service ended normal for both calls and send 
     * merge media indication to PA*/
    if (ICF_SUCCESS == icf_rnw_send_merge_media_ind(p_pdb))
    {
        if((ICF_RNW_CALL_RELEASED == p_pdb->p_rnw_ctx->pending_event)
            || (ICF_RNW_STATE_INVALID == p_pdb->p_rnw_ctx->next_state))
        {
            ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }
        else
        {
            ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)
            
            ICF_RNW_SET_CURR_STATE(p_pdb, p_pdb->p_rnw_ctx->next_state)
        }

        ret_val = icf_rnw_apply_both_calls(p_pdb);
    } /* p_pdb->p_rnw_ctx->pending_even... */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_rnw_03_merge_media_resp */

 
/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_start_proc_signaling
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_START_PROC_SIGNALING
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_xx_start_proc_signaling(
        INOUT  icf_rnw_pdb_st *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    if (p_glb_pdb->p_call_ctx->call_id == 
            p_pdb->p_rnw_ctx->p_call_ctx_c->call_id)
    {
        if (ICF_RNW_05_AWT_START_PROC_SIGNALING == 
                p_pdb->p_rnw_ctx->next_state)
        {
            ICF_RNW_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ)
        
            /*Send NOTIFY message*/
            ret_val = icf_rnw_send_notify_msg(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS);
        
            p_pdb->p_rnw_ctx->next_state = ICF_RNW_06_AWT_NOTIFY_ACK;

            ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NEXT_STATE)

            if (ICF_RNW_05_AWT_START_PROC_SIGNALING == 
                    p_pdb->p_rnw_ctx->state)
            {
                ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_06_AWT_NOTIFY_ACK)
            }
        } /* p_pdb->p_rnw_ctx->next_state) */        
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_rnw_xx_start_proc_signaling */



/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_notify_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                 Event         ICF_RNW_NOTIFY_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_notify_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Check if next_state is notify ack, means notify has been sent to
     * network, then set next_state as invalid*/
    if (ICF_RNW_06_AWT_NOTIFY_ACK == 
            p_pdb->p_rnw_ctx->next_state)
    {
        p_pdb->p_rnw_ctx->next_state = ICF_RNW_STATE_INVALID;

        ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NEXT_STATE)
    }

    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_xx_notify_succ_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_06_notify_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_06_AWT_NOTIFY_ACK
*                               
*                 Event         ICF_RNW_NOTIFY_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_06_notify_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_FAILURE;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Set return status as normal indicating that service is 
     * terminated for both the calls*/            
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    /*Set BOTH CALLS in return status and also set other call in call
     * context*/
    ret_val = icf_rnw_apply_both_calls(p_pdb);
    
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_06_notify_succ_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_invite_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_INVITE_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_invite_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    if (ICF_RNW_04_AWT_FOR_CONNECT == p_pdb->p_rnw_ctx->next_state)
    {
        p_pdb->p_rnw_ctx->next_state = ICF_RNW_05_AWT_START_PROC_SIGNALING;
        
        ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NEXT_STATE)

        /*Indicate PA that REFER from network received*/
       if (ICF_FAILURE == icf_rnw_send_refer_ind(p_pdb))
       {
          ret_val = ICF_FAILURE;
        }

        /*Check if signaling ongoing is true then wait for start proc signaling
         * otherwise send NOTIFY request to SSA*/
        if (ICF_INT_MODULE_INVALID == 
            p_pdb->p_rnw_ctx->p_call_ctx_b->signalling_ongoing)
        {
            ret_val = icf_rnw_send_notify_msg(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS);

            p_pdb->p_rnw_ctx->next_state = ICF_RNW_06_AWT_NOTIFY_ACK;

            ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NEXT_STATE)
        }
    } /* if (ICF_RNW_04_AWT_FOR_CONN... */
    else
    {
        ICF_RNW_SET_STATUS(p_pdb, 
                ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)

        if (p_glb_pdb->p_call_ctx->call_id !=
                p_pdb->p_rnw_ctx->p_call_ctx_b->call_id)
        {
            ret_val = icf_rnw_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_rnw_ctx->p_call_ctx_b);
            
            ICF_RNW_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)
        }
    } /* else */   
    
    if ((ICF_NULL != p_glb_pdb->p_call_ctx->p_replaces_header)
            && (ICF_SUCCESS == ret_val))
    {
        /*Free replace nw call ID in call context*/
        ICF_MEMFREE(p_glb_pdb, p_glb_pdb->p_call_ctx->p_replaces_header,
                ICF_MEM_COMMON, p_ecode, ret_val)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_xx_invite_succ_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_04_invite_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_04_AWT_FOR_CONNECT
*                               
*                 Event         ICF_RNW_INVITE_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_04_invite_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Check if signaling ongoing is true then wait for start proc signaling
     * otherwise send NOTIFY request to SSA*/
    ret_val = icf_rnw_check_to_send_notify(p_pdb);

    if ((ICF_NULL != p_glb_pdb->p_call_ctx->p_replaces_header)
            && (ICF_SUCCESS == ret_val))
    {
        /*Free replace nw call ID in call context*/
        ICF_MEMFREE(p_glb_pdb, p_glb_pdb->p_call_ctx->p_replaces_header,
                ICF_MEM_COMMON, p_ecode, ret_val)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_04_invite_succ_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_media_error
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_MEDIA_ERROR_REFERING_CALL
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_media_error(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Trigger to terminate both the calls*/ 
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    ret_val = icf_rnw_apply_both_calls(p_pdb);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_xx_media_error*/


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_03_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               
*                 Event         ICF_RNW_CALL_RELEASED_REFERING_CALL
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_03_call_released(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_CC_INITIATED_CALL_REL)
    
    /*Buffer call released, it will be processed after merge media
     * response received*/
    p_pdb->p_rnw_ctx->pending_event = ICF_RNW_CALL_RELEASED;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_03_call_released */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_CALL_RELEASED_REFERING_CALL
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_call_released(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Send NOTIFY to referring party if the referred party released
     * the call*/
    if (p_glb_pdb->p_call_ctx->call_id !=
            p_pdb->p_rnw_ctx->p_call_ctx_b->call_id)
    {
        ret_val = icf_rnw_send_notify_msg(p_pdb, 
                (icf_boolean_t)ICF_FAILURE);
    } /* p_pdb->p_rnw_ctx->p_call_ctx_b... */    

    if (ICF_SUCCESS == ret_val)
    {
        /*Terminate service*/
        ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        
        ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_CC_INITIATED_CALL_REL)

        /*Return status will be applicable for both the calls*/
        ret_val = icf_rnw_apply_both_calls(p_pdb);
    } /* if (ICF_SUCCESS == ret_val) */    
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_xx_call_released */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_timer_expiry(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Terminate service*/
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    /*Return status will be applicable for both the calls*/
    ret_val = icf_rnw_apply_both_calls(p_pdb);
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_xx_timer_expiry */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_internal_rnw_rej
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_00_IDLE
*                               ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_SC_INTERNAL_RNW_REJ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_internal_rnw_rej(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_nw_inc_refer_st    *p_refer_req = ICF_NULL;
    icf_error_t            *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Copy refer req msg pointer received from SSA*/
    p_refer_req =
        (icf_nw_inc_refer_st *)(p_pdb->p_internal_msg->p_msg_data);

    /*Free replace_nw_call_id in SSA*/
    ICF_MEMFREE(p_glb_pdb, p_refer_req->p_referred_to,
            ICF_MEM_COMMON,
            p_ecode, ret_val)

    /*Send failure REFER response to network*/
    ret_val = icf_rnw_send_refer_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE);

    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_xx_internal_rnw_rej */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      
*                               ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_SC_INTERNAL_RNW_REJ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_forceful_call_clear(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    return ret_val;
} /* icf_rnw_xx_forceful_call_clear */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_03_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                 Event         ICF_SC_INTERNAL_RNW_REJ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_03_forceful_call_clear(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    /*Return status will be applicable for both the calls*/
    ret_val = icf_rnw_apply_both_calls(p_pdb);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    return ret_val;
} /* icf_rnw_03_forceful_call_clear */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_rnw_unexpected_event(
        INOUT   icf_rnw_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
  
    /*To remove warning*/
    p_pdb = p_pdb;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_rnw_unexpected_event */
/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_07_cta_initiated_cfm
*
* DESCRIPTION   : This function process the cta initiated cfm response from PA.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_rnw_07_cta_initiated_cfm(
        INOUT   icf_rnw_pdb_st   *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_error_t         ecode = ICF_NULL;
    icf_config_data_st   *p_config_data = ICF_NULL;
    icf_error_t         ecode_temp;
    icf_remote_call_transfer_initiated_cfm_st     *p_cta_init_cfm = ICF_NULL;

    ret_val =  icf_dbm_get_module_glb_data(
                                   p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                                   (icf_int_module_id_t)
                                   ICF_INT_MODULE_CFG,
                                   (icf_void_t **)&p_config_data,
                                   &ecode);

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_cta_init_cfm = 
       (icf_remote_call_transfer_initiated_cfm_st*)&(p_glb_pdb->p_recv_msg->payload[0]);
   
    /*SPR17667 : In case PA sends wrong call-id in rem_call_transfer_init_cfm
      then we need to send unexpected_msg_ind to PA. Also we return success
      from this function so that once RNW timer expires 4XX is send towards
      NW.*/
    if((p_pdb->p_rnw_ctx->p_call_ctx_b->call_id !=
       p_glb_pdb->p_call_ctx->call_id))
    {
        ecode = ICF_CAUSE_INVALID_CALL_ID;
        ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_glb_pdb,
                   p_glb_pdb->p_msg_lst->p_msg, &ecode);
    }/*SPR17667*/
    else 
    {    

       /* Check result if call transfer initiated is accepted or rejected *
        * If true then accepted and hence send refer and process else send*
        * 500 response to the transferrer and reject */

        if (ICF_TRUE == p_cta_init_cfm->result) 
        {
          ecode = (p_pdb->p_internal_msg->msg_hdr.ecode);

          /*Set return status as ICF_STATUS_SERVICE_MSG_TO_CC_SETUP*/
          ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MSG_TO_CC_SETUP)
          ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_NEW_CALL_ID)
          /*Wait for media committed in new call*/
          ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_01_AWT_MEDIA_COMMIT)

          p_pdb->p_rnw_ctx->next_state = ICF_RNW_04_AWT_FOR_CONNECT;
          ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NEXT_STATE)
          /*Send REFER response to SSA*/

          if (ICF_SUCCESS == icf_rnw_send_refer_resp(p_pdb,
                  (icf_boolean_t)ICF_SUCCESS))
          {
              /*Assign new call id in global pdb*/
              p_glb_pdb->p_call_ctx = p_pdb->p_rnw_ctx->p_call_ctx_c;
          }
          else
          {
              ret_val = icf_dbm_dealloc_srvc_cntxt_blk(p_glb_pdb,
                      p_pdb->p_rnw_ctx->p_call_ctx_c->p_service_ctx, &ecode_temp);

              /*Deallocate all the memory allocated for p_call_ctx*/
              ret_val = icf_rnw_clear_call_ctx(p_pdb, p_pdb->p_rnw_ctx->p_call_ctx_c);

              ret_val = ICF_FAILURE;
          }/* else */
         }
        else
        {
           /*Send failure REFER response to SSA*/
           ret_val = icf_rnw_send_refer_resp(p_pdb,
                 (icf_boolean_t)ICF_FAILURE);

            ICF_STAT(p_glb_pdb, ICF_STAT_NW_CC_REFER)

            /*Initialize next state as invalid*/
            p_pdb->p_rnw_ctx->next_state = ICF_RNW_STATE_INVALID;

            ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

            ret_val = icf_dbm_dealloc_srvc_cntxt_blk(p_glb_pdb,
                 p_pdb->p_rnw_ctx->p_call_ctx_c->p_service_ctx, &ecode_temp);

	        /*Deallocate all the memory allocated for p_call_ctx*/
            ret_val = icf_rnw_clear_call_ctx(p_pdb, p_pdb->p_rnw_ctx->p_call_ctx_c);
         }
#ifdef ICF_TRACE_ENABLE
       p_pdb->result=p_cta_init_cfm->result;
       p_pdb->error_cause=ecode;
       ICF_RNW_APP_TRACE(p_pdb, ICF_TRACE_REMOTE_CALL_TRANSFER_CFM)       

#endif 
    }/* End of SPR17667 else */   
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;

}

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_07_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_07_AWT_FOR_CTA_INITIATED_CFM
*                 Event         ICF_RNW_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_07_timer_expiry(
        INOUT    icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_error_t         ecode_temp;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Terminate service*/
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    p_pdb->resp_code = ICF_RNW_TIMER_EXPIRY;

    /*Send REFER response to SSA*/
    ret_val = icf_rnw_send_refer_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE);

    /*ADUTTA-Check with Amit*/

    ICF_STAT(p_glb_pdb, ICF_STAT_NW_CC_REFER)

    /*Initialize next state as invalid*/
    p_pdb->p_rnw_ctx->next_state = ICF_RNW_STATE_INVALID;

    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    ret_val = icf_dbm_dealloc_srvc_cntxt_blk(p_glb_pdb,
                 p_pdb->p_rnw_ctx->p_call_ctx_c->p_service_ctx, &ecode_temp);

	    /*Deallocate all the memory allocated for p_call_ctx*/
    ret_val = icf_rnw_clear_call_ctx(p_pdb, p_pdb->p_rnw_ctx->p_call_ctx_c);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_07_timer_expiry */

#endif

