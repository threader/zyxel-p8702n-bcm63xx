/*******************************************************************************
*
* FILE NAME    : icf_twc_fsm.c
*
* DESCRIPTION  : Contains all the state machine functions used in TWC service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   10-Jan-2005  Amit Sharma     None           Created Origional
*   15-Feb-2005  Amit Sharma     None           Added functionality for other
*                                               call and both calls
*   25-Feb-2005  Amit Sharma                    Added handling for forceful 
*                                               call clear
*   25-Apr-2005  Amit Sharma     SPR:7423       Updated other call info in 
*                                               return path
*   28-Apr-2005  Amit Sharma     SPR:7461       Added function for call release
*                                               in IDLE state
*   28-Apr-2005  Amit Sharma     SPR:7463       Failure response when call rel
*                                               received
*   15-May-2005  Amit Sharma     SPR:7444       Update STATS
*   19-May-2005  Amit Sharma     SPR:7581       call resume req if any of two
*                                               calls are in held state
*
*   02-May-2005  Amit Sharma     SPR:7535       Service is terminated after conf estab.
*   14-Jun-2005  Amit Sharma     SPR:7662       Fill conf info in both call ctxs
*   08-Nov-2005  Amit Sharma     SPR:8129
*   27-Feb-2006  Amit Sharma                    Merged code for ICF
*   24-Aug-2007  Amit Sharma                     IPTK merge from PQT6.5
*   07-Jan-2009  Vinay Jindal    SPR 19250      When merge media response fails or twc
*                                               twc timer expires, then do not release 
*                                               the calls.
* Copyright 2009,Aricent.
*
******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE

#include "icf_twc_types.h"
#include "icf_twc_prototypes.h"
#include "icf_twc_macro.h"


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_00_create_conference
*
* DESCRIPTION  : This function is called when create conference request 
*                receives in idle state
*                
*                State: ICF_TWC_00_IDLE
*                Event: ICF_TWC_CREATE_CONFERENCE
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_twc_00_create_conference(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t     ret_val = ICF_FAILURE;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_app_conf_st  *p_app_conf = ICF_NULL;
    icf_error_t      *p_ecode = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CONF_REQ)

    /*Set both call contexts in TWC service logic contexts*/
    ret_val = icf_twc_set_call_ctx_in_twc_ctx(p_pdb);

    if (ICF_NULL != p_glb_pdb->p_recv_msg)
    {
        if (ICF_FAILURE == icf_twc_clone_app_spec_hdr_lists(p_pdb))
        {
            ret_val = ICF_FAILURE;
        }
    }
 
    if (ICF_SUCCESS == ret_val)
    {
        ret_val = icf_cfg_fetch_app_conf(
                    p_glb_pdb,
                    p_glb_pdb->p_call_ctx->app_id,
                    &p_app_conf,
                    p_ecode);
    }
    if (ICF_SUCCESS == ret_val && 
        !ICF_TWC_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    {
        ICF_TWC_TRACE(p_pdb, ICF_TRACE_TWC_RECVD)
        /*Check media is committed in the new call or not
         *if not committed then wait for media commit*/
        if ((ICF_FALSE == p_pdb->p_twc_ctx->p_call_ctx_c->media_committed) &&
            (ICF_FALSE == p_app_conf->app_profile.merge_req_wo_rem_sdp))
        {
            ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_MEDIA_NOT_COMMITTED)
            
            ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_03_AWT_MEDIA_COMMITTED)
            
            ret_val = icf_twc_start_twc_timer(p_pdb);
        } /* if(ICF_FALSE == p_pdb->p_ca... */
        /*Else check whether call hold service is active, if activethen wait
         * for CHR service response else send merge media req*/
        else if ((ICF_SUCCESS == icf_twc_check_chr_service(p_pdb)) || 
                 (ICF_INVALID_CALL_ID != 
                  p_glb_pdb->p_call_ctx->call_to_be_joined))
        {
            ret_val = icf_twc_start_twc_timer(p_pdb);
        } /* else if(ICF_SUCCESS == ipps... */
        else
        {
            ret_val = ICF_FAILURE;
        } /* else */        
    } /* if(IPPATK_SUCCESS == ret_val) */

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
}/*icf_twc_00_create_conference*/

/*******************************************************************************
*
* FUNCTION NAME: icf_twc_01_merge_media_resp
*
* DESCRIPTION  : This function is called when merge media response receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_MERGE_MEDIA_RESP
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_merge_media_resp(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_TWC_TRACE(p_pdb, ICF_TRACE_MERGE_MEDIA_RESP)
   
    
    /*Set return status that media is completed*/
    ICF_TWC_SET_STATUS(p_pdb, 
                    ICF_STATUS_MEDIA_ONGOING_ENDED)
    
    if(ICF_INVALID_CALL_ID !=
       p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_to_be_joined)
    {
	/*Only send media merge indication and return */
       	if (ICF_SUCCESS == icf_twc_send_merge_media_ind(p_pdb))
        {
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

            p_pdb->p_twc_ctx->p_call_ctx_b->call_status |=
            ICF_CALL_STATUS_CONFERENCE;

            p_pdb->p_twc_ctx->p_call_ctx_c->call_status |=
            ICF_CALL_STATUS_CONFERENCE;

            p_pdb->p_twc_ctx->p_call_ctx_b->p_conf_other_call_ctx =
            p_pdb->p_twc_ctx->p_call_ctx_c;

            p_pdb->p_twc_ctx->p_call_ctx_c->p_conf_other_call_ctx =
            p_pdb->p_twc_ctx->p_call_ctx_b;

            if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id
                    == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_b);
            } /* == p_pdb->p_twc_ctx->p_call_ct... */
            else
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
            }

            ret_val = icf_twc_stop_twc_timer(p_pdb);
        }
    }/*if(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx...*/ 
    else
    {
        
         /* If call_status is local held then send resume req to CHR
         *  service and wait for resume response
         */
        if(ICF_CALL_STATUS_LOCAL_HELD & 
                p_pdb->p_twc_ctx->p_call_ctx_b->call_status)
        {
            ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_CALL_LOCAL_HOLD)
       
            /*Indicating that fsm is waiting for call resume response*/
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_RESUME_REQ)

            ICF_TWC_SET_NEXT_STATE(p_pdb, ICF_TWC_06_CONFERENCE)

            ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_02_AWT_CALL_RESUME_RESP)

            /*If call hold request to be send for the call other then
             * present call context*/
            if(p_pdb->p_twc_ctx->p_call_ctx_b->call_id != 
                    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id)
            {
                ICF_TWC_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_b);
            } /* p_pdb->p_internal_msg->msg_hdr... */
            else
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
            }
        } /* if(ICF_CALL_STATUS_LOCAL_HE... */
        /*If other call is held then send resume request for that call and 
         * wait for resume response*/
        else if(ICF_CALL_STATUS_LOCAL_HELD & 
                p_pdb->p_twc_ctx->p_call_ctx_c->call_status)
        {
            ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_CALL_LOCAL_HOLD)
       
            /*Indicating that fsm is waiting for call resume response*/
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_RESUME_REQ)

            ICF_TWC_SET_NEXT_STATE(p_pdb, ICF_TWC_06_CONFERENCE)

            ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_02_AWT_CALL_RESUME_RESP)

            /*If call hold request to be send for the call other then
             * present call context*/
            if(p_pdb->p_twc_ctx->p_call_ctx_c->call_id != 
                    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id)
            {
                ICF_TWC_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
            } /* p_pdb->p_internal_msg->msg_hdr... */
            else
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_b);
            }
        } /* if(ICF_CALL_STATUS_LOCAL_HE... */
        else if(ICF_SUCCESS == 
                icf_twc_send_adm_conference_resp(p_pdb, 
                    (icf_boolean_t)(ICF_SUCCESS), 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED)))
        {
            /*ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_06_CONFERENCE)*/

            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

            p_pdb->p_twc_ctx->p_call_ctx_b->call_status |= 
            ICF_CALL_STATUS_CONFERENCE;

            p_pdb->p_twc_ctx->p_call_ctx_c->call_status |= 
            ICF_CALL_STATUS_CONFERENCE;

            p_pdb->p_twc_ctx->p_call_ctx_b->p_conf_other_call_ctx = 
            p_pdb->p_twc_ctx->p_call_ctx_c;

            p_pdb->p_twc_ctx->p_call_ctx_c->p_conf_other_call_ctx = 
            p_pdb->p_twc_ctx->p_call_ctx_b;

            if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
                    == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_b);
            } /* == p_pdb->p_twc_ctx->p_call_ct... */
            else 
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
            }

            ret_val = icf_twc_stop_twc_timer(p_pdb);
        } /* ICF_CAUSE_UNDEFINED)) */
        else
        {
            ret_val = ICF_FAILURE;
        } /* else */
    }/* End of else for p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_to_be_joined == ICF_INVALID_CALL_ID */   
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_twc_01_merge_media_resp */

/*******************************************************************************
*
* FUNCTION NAME: icf_twc_02_call_resume_resp
*
* DESCRIPTION  : This function is called when Call Resume response receives in 
*                following state
*                State: ICF_TWC_02_AWT_CALL_RESUME_RESP
*                Event: ICF_TWC_CALL_RESUME_RESP
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_02_call_resume_resp(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t     ret_val = ICF_FAILURE;
    icf_call_ctx_st  *p_held_call_ctx = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*If call resume response is received for call B then check if call C 
     * is held or not and vice-versa*/
    if (p_pdb->p_twc_ctx->p_call_ctx_b->call_id == 
            p_glb_pdb->p_call_ctx->call_id)
    {
        if (ICF_FALSE == p_pdb->p_twc_ctx->chr_for_call_c)
        {
            p_held_call_ctx = p_pdb->p_twc_ctx->p_call_ctx_c;
            p_pdb->p_twc_ctx->chr_for_call_c = ICF_TRUE;
        }
    } /* p_glb_pdb->p_call_ctx->call_id... */
    else
    {
        if (ICF_FALSE == p_pdb->p_twc_ctx->chr_for_call_b)
        {
            p_held_call_ctx =p_pdb->p_twc_ctx->p_call_ctx_b;
            p_pdb->p_twc_ctx->chr_for_call_b = ICF_TRUE;
        }
    } /* else */    
   
    /*If call is held then send resume request for other call also
     * and wait in the same state for call resume response*/
    if((ICF_NULL != p_held_call_ctx) && 
            (ICF_CALL_STATUS_LOCAL_HELD & p_held_call_ctx->call_status))
    {
        ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_CALL_LOCAL_HOLD)
       
        /*Indicating that fsm is waiting for call resume response*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_RESUME_REQ)

        ICF_TWC_SET_NEXT_STATE(p_pdb, ICF_TWC_06_CONFERENCE)

        ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_02_AWT_CALL_RESUME_RESP)

        ICF_TWC_SET_STATUS(p_pdb, 
                ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                p_held_call_ctx);
    } /* if(ICF_CALL_STATUS_LOCAL_HE... */
    else if(ICF_TWC_06_CONFERENCE & ICF_TWC_GET_NEXT_STATE(p_pdb))
    {
        
        /*Send Successful conference response to PA*/
        ret_val = icf_twc_send_adm_conference_resp(p_pdb, 
                (icf_boolean_t)(ICF_SUCCESS),
                (icf_error_t)(ICF_CAUSE_UNDEFINED));
        
        if(ICF_SUCCESS == ret_val)
        {
            /*Conference is established*/
            /*ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_06_CONFERENCE)*/

            /*Terminate the service for both calls when conference is
             * successfully implemented*/
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            ICF_TWC_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

            p_pdb->p_twc_ctx->p_call_ctx_b->call_status |= 
            ICF_CALL_STATUS_CONFERENCE;

            p_pdb->p_twc_ctx->p_call_ctx_c->call_status |= 
                ICF_CALL_STATUS_CONFERENCE;

            p_pdb->p_twc_ctx->p_call_ctx_b->p_conf_other_call_ctx = 
                p_pdb->p_twc_ctx->p_call_ctx_c;

            p_pdb->p_twc_ctx->p_call_ctx_c->p_conf_other_call_ctx = 
                p_pdb->p_twc_ctx->p_call_ctx_b;

            if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
                == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_b);
            } /* == p_pdb->p_twc_ctx->p_call_ct... */
            else 
            {
                ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
            }
            
            /*Stop TWC timer*/
            ret_val = icf_twc_stop_twc_timer(p_pdb);
        } /* ICF_CAUSE_UNDEFINED)) */
        
        /*Reset next_state flag*/
        ICF_TWC_RESET_NEXT_STATE(p_pdb)

    } /* if(ICF_TWC_06_CONFERENCE ==... */
    /*Next state is merge media resp, means merge media request to
     *be sent know*/
    else if(ICF_TWC_01_AWT_MERGE_MEDIA_RESP & 
            ICF_TWC_GET_NEXT_STATE(p_pdb))
    {
        ret_val = icf_twc_check_med_for_merge_req(p_pdb);

        /*Reset next_state flag*/
        ICF_TWC_RESET_NEXT_STATE(p_pdb)
    
    } /* else if(ICF_TWC_01_AWT_MERG... */  
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_twc_02_call_resume_resp */



/*******************************************************************************
*
* FUNCTION NAME: icf_twc_03_media_committed
*
* DESCRIPTION  : This function is called when media committed response 
*                for second call receives in 
*                following state
*                State: ICF_TWC_03_AWT_MEDIA_COMMITTED
*                Event: ICF_TWC_MEDIA_COMMITTED
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_03_media_committed(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*Check if CHR service is active or not, if active then wait for
     *call hold or call resume based on call_status, else check media
     *ongiong flag, if not set then send merge media request else
     *wait for start proc media*/
    ret_val = icf_twc_check_chr_service(p_pdb);

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_twc_03_media_committed */


/*******************************************************************************
*
* FUNCTION NAME: ICF_TWC_04_AWT_CALL_HOLD_RESP
*
* DESCRIPTION  : This function is called when call hold response receives in 
*                following state
*                State: ICF_TWC_04_AWT_CALL_HOLD_RESP
*                Event: ICF_TWC_CHD_RESP
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_04_call_hold_resp(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*Check if media is going in other module then wait for start proc media
     * else send merge media request
     */
    ret_val = icf_twc_check_med_for_merge_req(p_pdb);

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* ICF_TWC_04_AWT_CALL_HOLD_RESP */


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_05_awt_start_proc_media
*
* DESCRIPTION  : This function is called when start proc media receives in 
*                following state
*                State: ICF_TWC_05_AWT_START_PROC_MEDIA
*                Event: ICF_TWC_START_PROC_MEDIA
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_05_awt_start_proc_media(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
   
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*
     * This function check if media ongoing for any of two calls
     * then wait in the same state else send merge media request
     */
    ret_val = icf_twc_check_med_for_merge_req(p_pdb);

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_twc_05_awt_start_proc_media */


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_01_call_release
*
* DESCRIPTION  : This function is called when call release receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_CALL_RELEASE
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_call_release(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t   ret_val = ICF_FAILURE;
    icf_glb_pdb_st  *p_glb_pdb = 
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*
     * This function stop TWC timer and
     * send conference response to PA
     */
    if (ICF_REL_TYPE_MM_INITIATED == p_glb_pdb->p_call_ctx->release_type)
    {
        ret_val = icf_twc_resp_to_pa_n_stop_timer(p_pdb, 
            (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
    }
    else
    {
        ret_val = icf_twc_resp_to_pa_n_stop_timer(p_pdb, 
            (icf_error_t)(ICF_CAUSE_INVALID_SERVICE_ACTION));
    }

    if (ICF_SUCCESS == ret_val)
    {
        /*If call release received for this call B then
         * send service ended for call C*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

        ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_END_OTHER_CALL)

        /*If call context in p_glb_pdb is same as call context C then
         *set call context B in return payload other wise set call context C
         *in return payload*/
        if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
                == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_b);
        } /* == p_pdb->p_twc_ctx->p_call_ct... */
        else 
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_c);
        } /* else */
    } /* if (ICF_SUCCESS == ret_val) */
    
    

    ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_twc_01_call_release */


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_xx_call_release
*
* DESCRIPTION  : This function is called when call release receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                       ICF_TWC_02_AWT_CALL_RESUME_RESP
*                       ICF_TWC_03_AWT_MEDIA_COMMITTED
*                       ICF_TWC_04_AWT_CALL_HOLD_RESP
*                       ICF_TWC_05_AWT_START_PROC_MEDIA
*                Event: ICF_TWC_CALL_RELEASE
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_xx_call_release(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = 
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    if (ICF_REL_TYPE_MM_INITIATED == p_glb_pdb->p_call_ctx->release_type)
    {
        ret_val = icf_twc_resp_to_pa_n_stop_timer(p_pdb,
            (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
    }
    else
    {
        ret_val = icf_twc_resp_to_pa_n_stop_timer(p_pdb,
            (icf_error_t)(ICF_CAUSE_INVALID_SERVICE_ACTION));
    }

    if (ICF_SUCCESS == ret_val)
    {
        /*If call release received for this call B then
         * send service ended for call C*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

        /*If call context in p_glb_pdb is same as call context C then
         *set call context B in return payload other wise set call context C
         *in return payload*/
        if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
                == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_b);
        } /* == p_pdb->p_twc_ctx->p_call_ct... */
        else
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_c);
        } /* else */
    } /* if (ICF_SUCCESS == ret_val) */    

    ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_TERMINATE_SERVICE)
        
    ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_twc_xx_call_release */


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_06_call_release
*
* DESCRIPTION  : This function is called when call release receives in 
*                following state
*                State: ICF_TWC_06_CONFERENCE
*                Event: ICF_TWC_CALL_RELEASE
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_06_call_release(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Send service ended normal to call B*/
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

    /*If call context in p_glb_pdb is same as call context C then
     *set call context B in return payload other wise set call context C
     *in return payload*/
    if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
            == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
    {
        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_twc_ctx->p_call_ctx_b);
    } /* == p_pdb->p_twc_ctx->p_call_ct... */
    else
    {
        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_twc_ctx->p_call_ctx_c);
    } /* else */

    ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_TERMINATE_SERVICE)
    
    ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /*icf_twc_06_call_release*/ 


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_01_mm_err_ind
*
* DESCRIPTION  : This function is called when mm error indication receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_MM_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_mm_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
	icf_config_data_st      *p_config_data = ICF_NULL;
	icf_error_t             *p_ecode = ICF_NULL;
	icf_msg_resp_st         *p_recv_msg_payload = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Set return status that media is completed*/
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)

	/* Get the config data */
	ret_val = icf_dbm_get_module_glb_data(
					p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
					(icf_int_module_id_t)ICF_INT_MODULE_CFG,
					(icf_void_t *)&p_config_data, p_ecode);

	ICF_ASSERT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,\
                                   (ICF_NULL != p_config_data))

    if(ICF_TRUE == p_config_data->conf_fail_call_clear_not_reqd)
	{
       /* Get the error code present in merge_media_resp */\
       p_recv_msg_payload = (icf_msg_resp_st *)
            (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->payload); 

       /* Send this error code to Application in conference failure response */
	   ret_val = icf_twc_terminate_service(p_pdb, 
						p_recv_msg_payload->error_cause);

       p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg = ICF_NULL;
       p_pdb->p_internal_msg->msg_hdr.payload_length = 0;

	}
    else
    {
		/*This code let shall stop TWC timer and send failure conference
		 * response to PA, and set service ended error in both
		 * call contexts to terminate calls*/
		ret_val = icf_twc_terminate_service(p_pdb, 
						(icf_uint32_t)(ICF_CAUSE_CALL_MERGE_FAIL));
	}
  
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_twc_01_mm_err_ind */


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_01_timer_expiry
*
* DESCRIPTION  : This function is called when TWC timer expiry receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_TIMER_EXPIRY
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_timer_expiry(
        INOUT   icf_twc_pdb_st *p_pdb)
{
   icf_return_t    ret_val = ICF_SUCCESS;
   icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
   if(ICF_NULL == p_glb_pdb)
    {
          ICF_PRINT(((icf_uint8_t *)"p_glb_pdb is null"));
           return ICF_FALSE;
    }
  
	p_glb_pdb = p_glb_pdb;

   ICF_FUNCTION_ENTER(p_glb_pdb)

   ICF_STAT(p_glb_pdb, ICF_STAT_TWC_TMR_EXPIRY)

   /*
    * This function clears both the calls
    * and stops TWC timer and sends failure
    * conference response to PA
    */
   ret_val = icf_twc_terminate_service(p_pdb, 
           (icf_uint8_t)(ICF_CAUSE_TIMER_EXPIRY));

   ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

   return ret_val;
} /* icf_twc_01_timer_expiry */


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_xx_timer_expiry
*
* DESCRIPTION  : This function is called when TWC timer expiry receives in 
*                following state
*                State: ICF_TWC_02_AWT_CALL_RESUME_RESP
*                       ICF_TWC_03_AWT_MEDIA_COMMITTED
*                       ICF_TWC_04_AWT_START_PROC_MEDIA
*                       ICF_TWC_05_AWT_CHD_RESP
*                Event: ICF_TWC_TIMER_EXPIRY
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_xx_timer_expiry(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    if(ICF_NULL == p_glb_pdb)
    {
          ICF_PRINT(((icf_uint8_t *)"p_glb_pdb is null"));
           return ICF_FALSE;
    }

	p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_STAT(p_glb_pdb, ICF_STAT_TWC_TMR_EXPIRY)

    ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_TERMINATE_SERVICE)

    ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_00_IDLE)

    /*
     * This funciton stops TWC timer
     * and sends failure conference response
     * to PA
     */
    ret_val = icf_twc_resp_to_pa_n_stop_timer(p_pdb, 
            (icf_error_t)(ICF_CAUSE_TIMER_EXPIRY));

    if (ICF_SUCCESS == ret_val)
    {

        /*Send service ended normal to call B*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

        /*If call context in p_glb_pdb is same as call context C then
         *set call context B in return payload other wise set call context C
         *in return payload*/
        if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
                == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_b);
        } /* == p_pdb->p_twc_ctx->p_call_ct... */
        else
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_c);
        } /* else */
    } /* else */
       
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_twc_xx_timer_expiry */

/*******************************************************************************
*
* FUNCTION NAME: icf_twc_xx_service_err_ind
*
* DESCRIPTION  : This function is called when service error indication 
*                receives from other services(CHR)
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                       ICF_TWC_02_AWT_CALL_RESUME_RESP
*                       ICF_TWC_03_AWT_MEDIA_COMMITTED
*                       ICF_TWC_04_AWT_CALL_HOLD_RESP
*                       ICF_TWC_05_AWT_START_PROC_MEDIA
*                       ICF_TWC_06_CONFERENCE
*                Event: ICF_TWC_SERVICE_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_xx_service_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*
     * This function stops TWC timer and sends
     * failure conference response to PA
     */
    ret_val = icf_twc_resp_to_pa_n_stop_timer(p_pdb, 
            (icf_error_t)(ICF_CAUSE_INVALID_SERVICE_ACTION));

    if (ICF_SUCCESS == ret_val)
    {
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

        /*If call context in p_glb_pdb is same as call context C then
         *set call context B in return payload other wise set call context C
         *in return payload*/
        if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
                == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_b);
        } /* == p_pdb->p_twc_ctx->p_call_ct... */
        else
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_c);
        }
    } /* if (ICF_SUCCESS == ret_val) */    
    
    ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_TERMINATE_SERVICE)

    ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* INOUT   icf_twc_pdb_st *p_p... */


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_unexpected_event
*
* DESCRIPTION  : This function is called when any unexpected event receives
*                
* RETURNS:  
*******************************************************************************/
icf_return_t   icf_twc_unexpected_event(
        INOUT   icf_twc_pdb_st   *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    p_pdb = p_pdb;
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_twc_unexpected_event */

/*******************************************************************************
*
* FUNCTION NAME : icf_twc_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_twc_xx_forceful_call_clear(
        INOUT  icf_twc_pdb_st    *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Fill return status as both calls and fill call context pointer
     * of other call in return payload*/
    if (p_glb_pdb->p_call_ctx->call_id 
            == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
    {
        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_twc_ctx->p_call_ctx_b);
    } /* == p_pdb->p_twc_ctx->p_call_ct... */
    else
    {
        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_twc_ctx->p_call_ctx_c);
    }    

    /*Send service ended error for both the calls*/
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    return ret_val;
} /* icf_twc_xx_forceful_call_clear */

/******************************************************************************
*
* FUNCTION NAME : icf_twc_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication received in state 
*                 ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*
* RETURNS       : icf_return_t
******************************************************************************/
icf_return_t   icf_twc_01_forceful_call_clear(
        INOUT  icf_twc_pdb_st    *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Fill return status as both calls and fill call context pointer
     * of other call in return payload*/
    if (p_glb_pdb->p_call_ctx->call_id 
            == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
    {
        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_twc_ctx->p_call_ctx_b);
    } /* == p_pdb->p_twc_ctx->p_call_ct... */
    else
    {
        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_twc_ctx->p_call_ctx_c);
    }    

    /*Send service ended error for both the calls*/
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    return ret_val;
} /* icf_twc_01_forceful_call_clear */

/******************************************************************************
*
* FUNCTION NAME : icf_twc_00_call_release
*
* DESCRIPTION   : This function is used to handle call clear
*                 indication received in state 
*                 ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*
* RETURNS       : icf_return_t
******************************************************************************/
icf_return_t   icf_twc_00_call_release(
        INOUT  icf_twc_pdb_st    *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_twc_01_forceful_call_clear(p_pdb);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    return ret_val;
} /* icf_twc_00_call_release */
#endif


