/*******************************************************************************
*
* FILE NAME    : icf_chr_util.c
*
* DESCRIPTION  : Contains the definitions of utility functions 
*                used in CHR service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   7-Dec-2004  Amit Sharma     None           Created Origional
*  25-Jan-2005  Amit Sharma                    Changes for call_status field
*                                              in call context and added 
*                                              funciton to compare SDPs
*  12-Apr-2005  Amit Sharma     None           Update request pending cause name
*  04-May-2005  Amit Sharma   SPR:7495         Trigger call resume for internal
*                                              call resume request
*  15-May-2005  Amit Sharma   SPR:7560         CHR timer is stopped in chr_deinit
*  15-May-2005  Amit Sharma   SPR:7444         Update STATS
*  15-May-2005  Amit Sharma   SPR:7560         CHR timer is stopped only if not
*                                              previously stopped
*  08-Jul-2005  Amit Sharma                    Updates modify_media_type
*  29-Sep-2005  Shefali Aggarwal               Code for rel 2.0  
*  4-Oct-2005   Shefali Aggarwal               Changes at the time of UT - 
*                                              Function - 
*                                             icf_chr_internal_req_handler
*  21-Nov-2005  Amit Sharma   SPR:8306
*  07-Sep-2006  Amit Sharma   SPR:12840
*  11-Jul-2007  Neha Choraria  ICF 7.0         Function icf_chr_generate_timer
*                                              commented for changes in retry-
*                                              after.
*  24-Aug-2007  Amit Sharma                     IPTK merge from PQT6.5
*  30-Aug-2007  Amit Sharma   SPR:15974
*                                             
*  10-Apr-2008  Alok Tiwari                     Klocwork warning removal
*
*  19-Jun-2008  Jagmohan Chauhan SPR 18684     IPTK is sending incorrect media
*                    /CSR Merge CSR 1-6016916   mode (inactive) for resume request
*                                               when tried to toggle the calls
*  04-Jul-2008 Anuradha Gupta   SPR 18733      While sending call hold response, 
*                                              NULL check added
*  01-Apr-2009  Kamal Ashraf    Rel 8.2        Klocwork Fix
*  18-May-2009  Kamal Ashraf    SPR 19672      CSR_1_6809616 Merged  
*  10-Jun-2009 Abhishek Dhammawat SPR 19590    header list overwrite enh 
*  21-Jun-2009 Abhishek Dhammawat SPR 19590    header list overwrite enh 
*
* Copyright 2009,Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_chr_types.h"
#include "icf_chr_defs.h"
#include "icf_chr_macro.h"
#include "icf_chr_prototypes.h"
#include "icf_common_types.h"
#include "icf_sc_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_send_resume_resp
*
* DESCRIPTION  : This function sends Call Resume response to ADM 
*                which sends it to PA. To send response function
*                creates an internal message for ADM entry point function
*                and fill it for PP_CC_PA_CALL_RESUME_RESP API.
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_send_resume_resp(
        INOUT  icf_chr_pdb_st *p_pdb,
        IN     icf_uint8_t    result,
        IN     icf_error_t    error_cause
        )
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_msg_st         *p_msg = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_uint16_t         api_len = 0;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    api_len = 
    sizeof(icf_api_header_st) + sizeof(icf_api_resp_st);

    ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CALL_RESUME_RESP)

    /*Update stats*/
    if (ICF_FAILURE == result)
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CALL_RESUME)
    } /* if (ICF_FAILURE == result) */
    else
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_SUCC_CALL_RESUME)
    } 

    /*If Call resume request is from internal module*/
    if (ICF_TRUE == p_pdb->p_chr_ctx->internal_resume_resp)
    {        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_RESUME_RESP)

        ret_val = ICF_SUCCESS;
    } /* if (ICF_TRUE == p_pdb->p_chr... */

        
#ifdef ICF_TRACE_ENABLE
    /* Added to send traces */
    p_pdb->result=result;
    p_pdb->error_cause=error_cause;
    ICF_CHR_APP_TRACE(p_pdb, ICF_TRACE_CALL_RESUME_RESP)
#endif
    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON, p_msg, 
            ICF_RET_ON_FAILURE, p_ecode, ret_val)
    
    /*  SPR 18064 Merge */
    /*
     * In case ReINVITE/UPDATE for Call hold is challenge with 407 
     * and no authentication parameters were received, the same 
     * should be indicated to application
     */
    
    if(ICF_AUTH_PARAM_NOT_RCVD & p_pdb->p_call_ctx->common_bitmask)
    {
        /*Fill the appropriate error cause*/
	ret_val = icf_cmn_fill_api_resp(p_glb_pdb, p_msg, 
           (icf_module_id_t)(ICF_MODULE_PA),
           (icf_api_id_t)(ICF_CC_PA_CALL_RESUME_RESP),
           result, 
           (icf_error_t const)ICF_CAUSE_AUTH_PARAM_NOT_RCVD, 
	   p_ecode);

         /*Reset the bitmask*/
         p_pdb->p_call_ctx->common_bitmask &= 
                                  ~(ICF_AUTH_PARAM_NOT_RCVD);		
    }
    else
    {    
        ret_val = icf_cmn_fill_api_resp(p_glb_pdb, p_msg, 
                (icf_module_id_t)(ICF_MODULE_PA),
                (icf_api_id_t)(ICF_CC_PA_CALL_RESUME_RESP),
                result, (icf_error_t const)error_cause, p_ecode);
    }
            
    if (ICF_SUCCESS == ret_val)
    {
        /*Assign p_msg to p_msg_data of icf-internal_msg_st*/
        
        p_pdb->p_internal_msg->msg_hdr.msg_id = 
            ICF_CC_PA_CALL_RESUME_RESP;
        /* Fix for CSR1-5249829 */
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_pdb->p_call_ctx->call_id;
        
        p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;
       
        /*Fill retry_after_duration to application message*/ 
        if (0 != p_glb_pdb->p_call_ctx->retry_after_duration)
        {
             ((icf_api_resp_st*)p_msg->payload)->retry_after_duration = 
                 p_glb_pdb->p_call_ctx->retry_after_duration;
             p_glb_pdb->p_call_ctx->retry_after_duration = 0;
             ((icf_api_resp_st*)p_msg->payload)->bit_mask |=
                               ICF_RETRY_AFTER_DURATION_PRESENT;
        }
         
        p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        
        /*Call icf_adm_process_mesg(p_internal_msg)*/
        if (ICF_FAILURE == 
            icf_adm_process_mesg(p_pdb->p_internal_msg))
        {
            ret_val = icf_chr_free_api_resp(p_pdb, 
                (icf_api_resp_st*)&(p_msg->payload[0]));
            
            ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)

            p_pdb->p_internal_msg->p_msg_data = ICF_NULL;            
            
            ret_val = ICF_FAILURE;
        } /* icf_adm_process_mesg(p_pdb-... */
        else
        {
            ret_val = icf_chr_free_api_resp(p_pdb, 
                (icf_api_resp_st*)&(p_msg->payload[0]));

            ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)
                p_pdb->p_internal_msg->p_msg_data = ICF_NULL;            
        } /* else */
    }
    else
    {
        ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                    p_ecode, ret_val)      
    }
   
#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==result)
    {
        p_glb_pdb->load_data.call_resume_success++;
    }
#endif 
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}/*End of function icf_chr_adm_create_n_send_resume_mesg */


/*******************************************************************************
*
* FUNCTION NAME    :  icf_chr_free_api_resp
*
* DESCRIPTION  : This function frees API response message
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_free_api_resp(
       icf_chr_pdb_st *p_pdb, 
       icf_api_resp_st *p_api_resp)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
    icf_error_t     *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    if (ICF_NULL != p_api_resp->header_list.count)
    {
        if (ICF_FAILURE == icf_cmn_free_header_list(p_glb_pdb,
            &(p_api_resp->header_list),
            p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
    }
    
    if ((ICF_SUCCESS == ret_val) &&
        (ICF_NULL != p_api_resp->body_list.count))
    {
        if (ICF_FAILURE == icf_cmn_free_msg_body_list(p_glb_pdb,
            &(p_api_resp->body_list),
            p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
    }
    
    return ret_val;
}



/*******************************************************************************
*
* FUNCTION NAME    :  icf_chr_send_hold_resp
*
* DESCRIPTION  : This function sends Call Hold response to ADM 
*                which sends it to PA. To send response function
*                creates an internal message for ADM entry point function
*                and fill it for PP_CC_PA_CALL_HOLD_RESP API.
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_send_hold_resp(
        INOUT  icf_chr_pdb_st *p_pdb,
        IN     icf_uint8_t    result,
        IN     icf_error_t    error_cause )
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_msg_st          *p_msg = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_uint16_t         api_len = 0;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    api_len = 
    sizeof(icf_api_header_st) + sizeof(icf_api_resp_st);

    ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CALL_HOLD_RESP)

    /*Update stats*/
    if (ICF_FAILURE == result)
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CALL_HOLD)
    } /* if (ICF_FAILURE == result) */
    else
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_SUCC_CALL_HOLD)
    } /* else */    
    
    /*If call hold request is from internal module*/
    if (ICF_TRUE == p_pdb->p_chr_ctx->internal_hold_resp)
    {        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_HOLD_RESP)

        ret_val = ICF_SUCCESS;
    } /* if (ICF_TRUE == p_pdb->p_chr... */

#ifdef ICF_TRACE_ENABLE
    /* Added to send traces */
    p_pdb->result=result;
    p_pdb->error_cause=error_cause;
    ICF_CHR_APP_TRACE(p_pdb, ICF_TRACE_CALL_HOLD_RESP)
#endif
    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON, p_msg,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)
    
    /*  SPR 18064 Merge */
    /*
     * In case ReINVITE/UPDATE for Call hold is challenge with 407 
     * and no authentication parameters were received, the same 
     * should be indicated to application
     */
    
    if(ICF_AUTH_PARAM_NOT_RCVD & p_pdb->p_call_ctx->common_bitmask)
    {
        /*Fill the appropriate error cause*/
         ret_val = icf_cmn_fill_api_resp(p_glb_pdb, p_msg, 
             (icf_module_id_t)(ICF_MODULE_PA),
             (icf_api_id_t)(ICF_CC_PA_CALL_HOLD_RESP),
             result, 
             (icf_error_t const)ICF_CAUSE_AUTH_PARAM_NOT_RCVD, 
             p_ecode);

         /*Reset the bitmask*/
         p_pdb->p_call_ctx->common_bitmask &= 
                                   ~(ICF_AUTH_PARAM_NOT_RCVD);		
    }
    else
    {
        icf_cmn_fill_api_resp(p_glb_pdb, p_msg, 
                (icf_module_id_t)(ICF_MODULE_PA),
                (icf_api_id_t)(ICF_CC_PA_CALL_HOLD_RESP),
                result, (icf_error_t const)error_cause, p_ecode);
    }
    if (ICF_SUCCESS == ret_val)
    {
        p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CALL_HOLD_RESP;
        /* Fix for CSR1-5249829 */
 
        if(ICF_NULL != p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst &&
           ICF_NULL != p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg)
        {
           p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_pdb->p_call_ctx->call_id;
        }
        p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;

        /*Fill retry_after_duration to application message*/
        if (0 != p_glb_pdb->p_call_ctx->retry_after_duration)
        {
             ((icf_api_resp_st*)p_msg->payload)->retry_after_duration = 
                 p_glb_pdb->p_call_ctx->retry_after_duration;
              p_glb_pdb->p_call_ctx->retry_after_duration = 0;
              ((icf_api_resp_st*)p_msg->payload)->bit_mask |=
                               ICF_RETRY_AFTER_DURATION_PRESENT;
        }
        
        /*Assign p_msg to p_msg_data of icf_internal_msg_st*/
        p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        
        /*Call ADM entry point to send message to PA*/
        if (ICF_FAILURE == 
            icf_adm_process_mesg(p_pdb->p_internal_msg))
        {
            ret_val = icf_chr_free_api_resp(p_pdb, 
                (icf_api_resp_st*)&(p_msg->payload[0]));

            ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)
                
                p_pdb->p_internal_msg->p_msg_data = ICF_NULL;
            ret_val = ICF_FAILURE;
        } /* icf_adm_process_mesg(p_pdb-... */
        else
        {
            ret_val = icf_chr_free_api_resp(p_pdb, 
                (icf_api_resp_st*)&(p_msg->payload[0]));

            ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)
                p_pdb->p_internal_msg->p_msg_data = ICF_NULL;
        } /* else */    
    }
    /*klocwork warning removal*/
    else
    {
       ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)
       ret_val = ICF_FAILURE;
    }

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==result)
    {
        p_glb_pdb->load_data.call_hold_success++;
    }
#endif
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}/*icf_chr_send_hold_resp*/


/*******************************************************************************
 *
 * FUNCTION NAME    :   icf_chr_mmi_send_resume_mesg
 *
 * DESCRIPTION  : This function sends Call Resume Indication to MMI,
 *                which triggers Modify Media req. To send indicaion function
 *                creates an internal message for MMI entry point function
 *                for API ICF_SC_MODIFY_MEDIA_SESSION_REQ.
 *                
 *
 * RETURNS:  
 *
 *******************************************************************************/

icf_return_t icf_chr_mmi_send_resume_mesg(
        INOUT  icf_chr_pdb_st *p_pdb
        )
{
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;
    icf_mmi_mms_request_st   mmi_resume_req = {0,0,0,{{0,0,0}},0} ;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_internal_list_st	 *p_list = ICF_NULL;
    icf_stream_sdp_info_st   *p_stream_sdp_info = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        /*
         * Reset call proc media
         */
    ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

    /*
     * Set media ongoing in HCR service
     */
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ)

    /* Initializing msg data */
    mmi_resume_req.bit_mask = 0;
    mmi_resume_req.modify_stream_count = 0;
    mmi_resume_req.p_add_media = ICF_NULL;
    mmi_resume_req.p_delete_media = ICF_NULL;

    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_MODIFY_MEDIA_REQ;

    p_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_mmi_mms_request_st);

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while(ICF_NULL != p_list)
    {
        p_stream_sdp_info = p_list->p_payload;

        if (ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type)
        {
            /* Merging done for CSR-1-6016916*/
            /* Here we are changing the mm_applied_mode (this value is the mode
               applied with Media Manager) to sendrecv .This is done so that
               whenever we received a call resume request ,we must do mms with
               mode as sendrecv.This will result in the successfull resume of
               call*/
            /* Here we are not doing any validation on remote mode.Validation
             * of remote mode is done once we received the success response to
             * (RE-INVITE/UPDATE) send for resuming the call .*/
            /* Following set of combination are possible once we received the
             * success response to resuming the call*/
            /* 1)Remote Mode : recvonly -------->No mms is done for this.
               2)Remote Mode : sendrecv -------->No mms is done for this.
               3)Remote Mode : sendonly -------->We will do mms for this with
                 mode as recvonly once we received the success response.
               4)Remote Mode : inactive -------->We will do mms for this with
                 mode as inactive once we received the success response.*/

            p_stream_sdp_info->mm_applied_mode = ICF_MEDIA_MODE_SEND_RECV;

            p_stream_sdp_info->application_mode = ICF_MEDIA_MODE_SEND_RECV;
            mmi_resume_req.modify_stream_count = 1;
            mmi_resume_req.bit_mask = ICF_MMI_MMS_ACTION_MODIFY;
            mmi_resume_req.modify_media[0].bitmask = ICF_MODE_CHANGE;
            mmi_resume_req.modify_media[0].stream_id = 
                p_stream_sdp_info->stream_id;
            mmi_resume_req.modify_media[0].stream_type = 
                ICF_STREAM_TYPE_AUDIO;
            
            /*Assign mmi_resume_req to p_msg_data of internal message*/
            p_pdb->p_internal_msg->p_msg_data = 
                (icf_uint8_t *)&mmi_resume_req;
            
            /*Call entry point function of MMI*/
            ret_val = icf_mmi_process_mesg(p_pdb->p_internal_msg);
            break;
        }

        p_list = p_list->p_next;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val; 
}


/*******************************************************************************
 *
 * FUNCTION NAME    :   icf_chr_send_mms_for_remote_resp
 *
 * DESCRIPTION  : This function sends media change Indication to MMI,
 *                which triggers Modify Media req. To send indicaion function
 *                creates an internal message for MMI entry point function
 *                for API ICF_SC_MODIFY_MEDIA_SESSION_REQ.
 *                
 *
 * RETURNS:  
 *
 *****************************************************************************/

icf_return_t icf_chr_send_mms_for_remote_resp(
        INOUT  icf_chr_pdb_st *p_pdb
        )
{
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;
    icf_mmi_mms_request_st   mmi_resume_req = {0,0,0,{{0,0,0}},0} ;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_internal_list_st	 *p_list = ICF_NULL;
    icf_stream_sdp_info_st   *p_stream_sdp_info = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    /*
     * Reset call proc media
     */
    ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

    /*
     * Set media ongoing in CHR service
     */
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ)
    
     /* Initializing msg data */
    mmi_resume_req.bit_mask = 0;
    mmi_resume_req.modify_stream_count = 0;
    mmi_resume_req.p_add_media = ICF_NULL;
    mmi_resume_req.p_delete_media = ICF_NULL;


    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_MODIFY_MEDIA_REQ;

    p_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_mmi_mms_request_st);

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while(ICF_NULL != p_list)
    {
        p_stream_sdp_info = p_list->p_payload;

        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type) &&
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp_info->stream_status))
        {
            if(ICF_CHR_OA_MMS_FOR_RESUME_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb))
            {
                p_stream_sdp_info->application_mode = ICF_MEDIA_MODE_SEND_RECV;
                if(ICF_TRUE == p_pdb->mms_for_mode)
                {
                    p_stream_sdp_info->mm_applied_mode = icf_chr_get_remote_mode(p_pdb);
                    mmi_resume_req.modify_media[0].bitmask |= ICF_MODE_CHANGE;
                }

            }
            mmi_resume_req.modify_stream_count = 1;
            mmi_resume_req.bit_mask = ICF_MMI_MMS_ACTION_MODIFY;
            mmi_resume_req.modify_media[0].bitmask |= ICF_REMOTE_SDP_MEDIA_CHANGE;
            mmi_resume_req.modify_media[0].stream_id = 
                p_stream_sdp_info->stream_id;
            mmi_resume_req.modify_media[0].stream_type = 
                ICF_STREAM_TYPE_AUDIO;
            
            /*Assign mmi_resume_req to p_msg_data of internal message*/
            p_pdb->p_internal_msg->p_msg_data = 
                (icf_uint8_t *)&mmi_resume_req;
            
            /*Call entry point function of MMI*/
            ret_val = icf_mmi_process_mesg(p_pdb->p_internal_msg);
            break;
        }

        p_list = p_list->p_next;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val; 
} /* icf_chr_send_mms_for_remote_resp */

/*******************************************************************************
 *
* FUNCTION NAME    :   icf_chr_mmi_send_hold_msg
*
* DESCRIPTION  : This function sends Call Hold Indication to MMI,
*                which triggers Modify Media req. To send indicaion function
*                creates an internal message for MMI entry point function
*                for API <TO DO>.
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_mmi_send_hold_msg(
        INOUT  icf_chr_pdb_st *p_pdb)
{
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_mmi_mms_request_st   mmi_hold_req = {0,0,0,{{0,0,0}},0} ;
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_internal_list_st	 *p_list = ICF_NULL;
    icf_stream_sdp_info_st   *p_stream_sdp_info = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    /*
     * Reset call proc media
     */
    ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)
    
    /*
     * Set media ongoing in HCR service
     */
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ)

    /* initializing the structure with 0 */
    mmi_hold_req.bit_mask = 0;
    
    mmi_hold_req.modify_stream_count = 0;
    mmi_hold_req.p_add_media = ICF_NULL;
    mmi_hold_req.p_delete_media = ICF_NULL;


    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_MODIFY_MEDIA_REQ;

    p_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_mmi_mms_request_st);

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while(ICF_NULL != p_list)
    {
        p_stream_sdp_info = p_list->p_payload;

        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type) &&
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp_info->stream_status))
        {
            p_stream_sdp_info->mm_applied_mode = ICF_MEDIA_MODE_INACTIVE;
            p_stream_sdp_info->application_mode = ICF_MEDIA_MODE_INACTIVE;
            p_stream_sdp_info->p_local_sdp->media_mode = ICF_MEDIA_MODE_INACTIVE;
            mmi_hold_req.modify_stream_count = 1;
            mmi_hold_req.bit_mask = ICF_MMI_MMS_ACTION_MODIFY;
            mmi_hold_req.modify_media[0].bitmask = ICF_MODE_CHANGE;
            mmi_hold_req.modify_media[0].stream_id = 
                p_stream_sdp_info->stream_id;
            mmi_hold_req.modify_media[0].stream_type = 
                ICF_STREAM_TYPE_AUDIO;
            
            /*Assign mmi_resume_req to p_msg_data of internal message*/
            p_pdb->p_internal_msg->p_msg_data = 
                (icf_uint8_t *)&mmi_hold_req;
            
            /*Call entry point function of MMI*/
            ret_val = icf_mmi_process_mesg(p_pdb->p_internal_msg);
            break;
        }

        p_list = p_list->p_next;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val; 

}


/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_reinvite_msg
*
* DESCRIPTION  : This function triggers SSA to send INVITE request on NW,
*                So it creates an internal message for SSA entry point function
*                for API ICF_SA_MEDIA_CHANGE.
*                Check mode needed or not
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_ssa_send_reinvite_msg(
        INOUT  icf_chr_pdb_st *p_pdb,
        IN     icf_boolean_t  call_hold
        )
{
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_nw_otg_media_change_req_st   media_change_req;
    icf_internal_list_st	 *p_list = ICF_NULL;
    icf_stream_sdp_info_st   *p_stream_sdp_info = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Reset call proc media
     */
    ICF_CHR_RESET_STATUS(p_pdb, 
            ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
    
    /*
     * Set media ongoing in CHR service
     */
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ)

    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE;

    /*New mode to be sent with nw request*/
    media_change_req.bitmask = ICF_NULL;
    media_change_req.call_hold = call_hold;
    media_change_req.mesg_type = 0;

    p_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_nw_otg_media_change_req_st);

    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_change_req;

    p_list = p_pdb->p_call_ctx->p_stream_sdp_info;

    while(ICF_NULL != p_list)
    {
        p_stream_sdp_info = p_list->p_payload;

        if (ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type)
        {
            if(ICF_FALSE == call_hold)
            {
                p_stream_sdp_info->application_mode = 
                    ICF_MEDIA_MODE_SEND_RECV;
               p_stream_sdp_info->p_local_sdp->media_mode = 
                    ICF_MEDIA_MODE_SEND_RECV;
				/* p_stream_sdp_info->p_local_sdp->media_mode = 
					icf_chr_get_remote_mode(p_pdb); */
            }
            else
            {
               /* p_stream_sdp_info->p_local_sdp->media_mode = 
                    ICF_MEDIA_MODE_SEND_ONLY;*/
            }
            break;
        }

        p_list = p_list->p_next;
    }

    /*p_glb_pdb->p_call_ctx->media_mode = ICF_MEDIA_MODE_INACTIVE;*/
    ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;    
}/*icf_chr_ssa_send_reinvite_msg*/



/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_cancel_msg
*
* DESCRIPTION  : This function triggers SSA to send CANCEL request on NW 
*                to cancel already sent INVITE,
*                So it creates an internal message for SSA entry point function
*                for API ICF_NW_OTG_CALL_ABORT.
*                
*
* RETURNS:  
*******************************************************************************/

icf_return_t icf_chr_ssa_send_cancel_msg(
        INOUT  icf_chr_pdb_st *p_pdb
        )
{
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_pdb->p_internal_msg->msg_hdr.msg_id = 
    ICF_NW_OTG_MEDIA_UPDATE_ABORT_REQ;

    p_pdb->p_internal_msg->msg_hdr.payload_length = 0;

    ret_val =  icf_ssa_process_mesg(p_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val; 
}/* icf_chr_ssa_send_cancel_msg*/


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_invite_reject 
*
* DESCRIPTION  : This function triggers SSA to send ICF_SEND_NW_CALL_REJECT
*                response for an incoming INVITE request,
*                So it creates an internal message for SSA entry point function
*                for API ICF_SEND_NW_CALL_REJECT.
*                
*
* RETURNS:  
*******************************************************************************/


icf_return_t icf_chr_ssa_send_invite_reject(
        INOUT  icf_chr_pdb_st *p_pdb
        )
{
    icf_glb_pdb_st                  *p_glb_pdb = ICF_NULL;    
    icf_nw_otg_media_change_resp_st invite_reject;
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_req_st  *p_inc_med_change = ICF_NULL; 
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    p_inc_med_change = (icf_nw_inc_media_change_req_st *)
            p_pdb->p_internal_msg->p_msg_data;

    if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_inc_med_change->bitmask)
    {
	icf_list_st	*p_inc_media_sdp= p_inc_med_change->p_sdp_list;
	icf_return_t ret_val=ICF_SUCCESS;
	icf_inc_media_sdp_info_st *p_inc_media_sdp_info_data = ICF_NULL;
        icf_error_t     ecode;

	/*Following while loop is deleting first the payload part
	of the node and then deleting the node itself.*/
	while(p_inc_media_sdp != ICF_NULL)
	{
            /*Freeing the payload part.*/
            p_inc_media_sdp_info_data=(icf_inc_media_sdp_info_st *)(p_inc_media_sdp->p_data);

            if (p_inc_media_sdp_info_data != ICF_NULL)
            {
                if (p_inc_media_sdp_info_data->p_int_sdp_stream != ICF_NULL)
                {
                    /*Deleting p_int_sdp_stream present in icf_inc_media_sdp_info*/
                    ret_val=icf_cmn_delete_internal_sdp_stream(
                                p_glb_pdb,
                                &(p_inc_media_sdp_info_data->p_int_sdp_stream),
                                &ecode);
                }
            }
            p_inc_media_sdp= p_inc_media_sdp->p_next;
        }

        ret_val = icf_cmn_delete_list(
                      p_glb_pdb,&p_inc_med_change->p_sdp_list,ICF_MEM_COMMON);
    }

    /*
     * Fill internal message structure for invite reject
     */

    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE_RESP;

    p_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_nw_otg_media_change_resp_st);
    
    /*Send failure for incoming hold-resume request*/
    invite_reject.status = ICF_FALSE;

    /*Set bitmask to indicate the reason_type*/
    invite_reject.bitmask = ICF_NW_OTG_MC_RESP_REASON;

    /* CSR_1_6809616 Merged for SPR-19672 start */
    /* If the local retry timer for 491 is running, Set the reason code as 500 
     * since we cannot handle the event otherwise send 491 */
    if(p_pdb->p_chr_ctx->p_race_timer_data)
    {
        invite_reject.reason_type = ICF_CALL_CLEAR_REASON_SERVICE_FAILURE;
    }
    else
    {
        invite_reject.reason_type = ICF_CALL_CLEAR_REASON_REQUEST_PENDING;
    }
    /* CSR_1_6809616 Merged for SPR-19672 end */

    /*Set pointer of message to SSA in msg_data*/
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&invite_reject;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;    
} /* INOUT  icf_chr_pdb_st *p_pd... */


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_og_setup_fail_ack 
*
* DESCRIPTION  : This function triggers SSA to send 
*                ICF_NW_OTG_CALL_REJECT_ACK
*                response for an outgoing INVITE request failure response,
*                So it creates an internal message for SSA entry point function
*                for API ICF_NW_OTG_CALL_REJECT_ACK.
*                
*
* RETURNS:  
*******************************************************************************/

icf_return_t icf_chr_ssa_send_og_setup_fail_ack(
        INOUT  icf_chr_pdb_st *p_pdb
        )
{
    icf_glb_pdb_st                  *p_glb_pdb = ICF_NULL;    
    icf_return_t                    ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Fill internal message structure for invite reject
     */

    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_REJECT_ACK;


    /*Set pointer of message to SSA in msg_data*/
    p_pdb->p_internal_msg->p_msg_data = ICF_NULL;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;    
} /* INOUT  icf_chr_pdb_st *p_pd... */


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_og_setup_succ_ack 
*
* DESCRIPTION  : This function triggers SSA to send 
*                ICF_NW_OTG_CALL_SETUP_ACK
*                response for an outgoing INVITE request Success response,
*                So it creates an internal message for SSA entry point function
*                for API ICF_NW_INC_CALL_SETUP_ACK.
*                
*
* RETURNS:  
*******************************************************************************/

icf_return_t icf_chr_ssa_send_og_setup_succ_ack(
        INOUT  icf_chr_pdb_st *p_pdb
        )
{
    icf_glb_pdb_st                  *p_glb_pdb = ICF_NULL;    
    icf_return_t                    ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Fill internal message structure for invite reject
     */

    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_SETUP_ACK;

    /*Call entry point of SSA*/
    ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;    
} /* INOUT  icf_chr_pdb_st *p_pd... */


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_start_timer
*
* DESCRIPTION  : This function is used to start CHR timer
*
* RETURNS:
******************************************************************************/
icf_return_t   icf_chr_start_timer(
        icf_chr_pdb_st     *p_pdb,
        icf_uint8_t        timer_type)
{
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_config_data_st   *p_config_data = ICF_NULL;
    icf_error_t          *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))
        
    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_ecode))

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Fill timer data structure */
    if(ICF_FAILURE == 
            icf_dbm_get_module_glb_data(p_glb_pdb, 
                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t *)&p_config_data, p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /* Call DBM to allocate timer buffer value */
    else if(ICF_FAILURE == 
            icf_dbm_get_mem_from_pool(p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                (icf_void_t **)&(p_pdb->p_chr_ctx->p_chr_timer_data),
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        p_pdb->p_chr_ctx->p_chr_timer_data->timer_type = timer_type;
        p_pdb->p_chr_ctx->p_chr_timer_data->module_id = ICF_INT_MODULE_CHR;
        /*
         * Store call context pointer in p_timer buffer
         * so that when timer expiry receives module can
         * identify the call data
         */
        p_pdb->p_chr_ctx->p_chr_timer_data->p_timer_buffer = 
            (icf_void_t *)p_pdb->p_call_ctx;
        p_pdb->p_chr_ctx->p_chr_timer_data->p_glb_pdb = p_glb_pdb;

        if(ICF_CHR_HOLD_RESUME_TIMER == timer_type)
        {            
            
            /*Start ICF_CHR_CALL_HOLD_RESUME_TIMER */
            ICF_START_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->p_chr_timer_data, 
                p_pdb->p_chr_ctx->chr_timer_id, 
                p_config_data->call_hold_resume_timer,
                p_ecode, ret_val)

            ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STARTED)
        } /* if(ICF_CHR_HOLD_RESUME_TIME... */
    } /* else */
   
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_uint8_t        timer_id... */

/****** The function below has been shifted to common in response to changes 
 for retry-after (Rel7.0) *******/

#if 0
/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_generate_timer
*
* DESCRIPTION  : This function is used to generate random timer
*
* RETURNS:
******************************************************************************/

icf_return_t icf_chr_generate_timer(
        INOUT icf_duration_t     *p_timer_val,
        IN    icf_uint16_t     st_val,
        IN    icf_uint16_t     end_val)
{
    icf_return_t       ret_val = ICF_SUCCESS;

    /*Get random timer value in specified range*/
    *p_timer_val = icf_port_random(st_val, end_val);

    /*If returned timer value is not in specified range*/
    if (*p_timer_val < st_val || *p_timer_val>end_val)
    {
        ret_val = ICF_FAILURE;
    } /* if (*timer_val < st_val || *tim... */    

    return ret_val;
} /* IN    icf_uint8_t     end_v... */


#endif

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_check_recvd_sdp_info
*
* DESCRIPTION  : This function is used to check whether remote SDP is same
*                as previous or not
*
* RETURNS: ICF_FAILURE in case of any change in remote media 
*          else ICF_SUCCESS
******************************************************************************/
icf_return_t   icf_chr_check_recvd_sdp_info(
       INOUT icf_chr_pdb_st   *p_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st                   *p_glb_pdb = 
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_stream_sdp_info_st           *p_stream_sdp = ICF_NULL;
    icf_internal_list_st             *p_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if (ICF_FALSE == p_pdb->sdp_present)
    {
         return ret_val;
    }
    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;
    /*Fetch sdp_list from nw_inc_media_change and fetch corresponding node from
     *call context stream_sdp_info and compare remote received SDP in it*/
    while(ICF_NULL != p_list)
    {
		p_stream_sdp = p_list->p_payload;

        if(ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type)
        {
                if (ICF_FALSE == 
                    icf_chr_compare_tranport_addr(p_pdb, 
                    &(p_stream_sdp->p_applied_sdp->sdp_stream.stream_address),
                    &(p_stream_sdp->p_remote_received_sdp->sdp_stream.stream_address)))
                {
                    ICF_CHR_TRACE(p_pdb, ICF_TRACE_TRANSPORT_ADDR_DIFF)
                        /*RTP address of committed SDP and incoming SDP are different*/
                        ret_val = ICF_FAILURE;
                } /* p_remote_commited_sdp->rtp_add... */
                /*Check if audio_profile is changed*/
                else if(ICF_FALSE == icf_chr_compare_audio_profile(p_pdb,
                    &(p_stream_sdp->p_applied_sdp->sdp_stream.media_stream_param.audio_profile),
                    &(p_stream_sdp->p_remote_received_sdp->sdp_stream.media_stream_param.audio_profile)))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ICF_CHR_TRACE(p_pdb, ICF_TRACE_SDP_SAME)
                        /*Both SDPs are same*/
                        ret_val = ICF_SUCCESS;
                } /* else */
				break;
            }
		p_list = p_list->p_next;
	}

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;            
} /* icf_chr_check_recvd_sdp_info*/

/******************************************************************************
* FUNCTION NAME    :   icf_chr_check_recvd_mode
* DESCRIPTION  : This function is used to check whether remote mode is same
*                as previous or not
* RETURNS: ICF_FAILURE in case of any change in remote media
*          else ICF_SUCCESS
******************************************************************************/
icf_return_t   icf_chr_check_recvd_mode(
       INOUT icf_chr_pdb_st   *p_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st                   *p_glb_pdb =
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_stream_sdp_info_st           *p_stream_sdp = ICF_NULL;
    icf_internal_list_st             *p_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if (ICF_FALSE == p_pdb->sdp_present)
    {
         return ret_val;
    }
    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;
    /*Fetch sdp_list from nw_inc_media_change and fetch corresponding node from
     *call context stream_sdp_info and compare remote received SDP in it*/
    while(ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if(ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type)
        {
            if(
               /*if previously call was active on platform and now we 
                 have recvd remote sdp for HOLD*/
               ((ICF_MEDIA_MODE_SEND_RECV ==
                     p_stream_sdp->mm_applied_mode)&&
                ((ICF_MEDIA_MODE_SEND_ONLY ==
                      p_stream_sdp->p_remote_received_sdp->media_mode)||
                 (ICF_MEDIA_MODE_INACTIVE ==
                         p_stream_sdp->p_remote_received_sdp->media_mode)))||
               /*if previously call was inactive on platform because of 
                 remote hold and now we have recvd remote sdp for RESUME*/
               ((ICF_MEDIA_MODE_INACTIVE ==
                     p_stream_sdp->mm_applied_mode)&&
                ((ICF_MEDIA_MODE_SEND_RECV ==
                      p_stream_sdp->p_remote_received_sdp->media_mode)||
                 (ICF_MEDIA_MODE_RECV_ONLY ==
                         p_stream_sdp->p_remote_received_sdp->media_mode))))
                {
                    ret_val = ICF_FAILURE;
                    p_pdb->mms_for_mode = ICF_TRUE;
                }
                else
                {
                    ICF_CHR_TRACE(p_pdb, ICF_TRACE_SDP_SAME)
                        /*Both SDPs are same*/
                        ret_val = ICF_SUCCESS;
                } /* else */
                break;
        }
        p_list = p_list->p_next;
    }

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_chr_check_recvd_mode*/


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_check_recvd_codec_info
*
* DESCRIPTION  : This function is used to check whether remote CODEC is same
*                as previous or not
*
* RETURNS:
******************************************************************************/
icf_return_t   icf_chr_check_recvd_codec_info(
       INOUT icf_chr_pdb_st   *p_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st                   *p_glb_pdb = 
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_stream_sdp_info_st           *p_stream_sdp = ICF_NULL;
    icf_internal_list_st             *p_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if (ICF_FALSE == p_pdb->sdp_present)
    {
         return ret_val;
    }
    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;
        /*Fetch sdp_list from nw_inc_media_change and fetch corresponding node from
        *call context stream_sdp_info and compare remote received SDP in it*/
        while(ICF_NULL != p_list)
        {
			p_stream_sdp = p_list->p_payload;
            
            if(ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type)
            {
                    if (ICF_FAILURE == 
                        icf_chr_compare_codec_list(p_pdb, 
                        p_stream_sdp->p_applied_sdp->sdp_stream.p_codec_info_list,
                        p_stream_sdp->p_remote_received_sdp->sdp_stream.p_codec_info_list))
                    {
                        ret_val = ICF_FAILURE;
                    }
                break;
            }
            p_list = p_list->p_next;
        }

    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return ret_val;            
} /* icf_chr_check_recvd_codec_info */

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_get_remote_mode
*
* DESCRIPTION  : This function is used to get the media mode to be sent with
*                network request
*
* RETURNS:
******************************************************************************/
icf_media_mode_t icf_chr_get_remote_mode(
        INOUT icf_chr_pdb_st *p_pdb)
{
    icf_return_t           ret_val = ICF_SUCCESS;

    /* THis variable store the media mode received from remote */
    icf_media_mode_t       remote_media_mode = ICF_MEDIA_MODE_INVALID;

    /* This variable store the media mode which
     * should be applied to MMI*/
    icf_media_mode_t       applied_media_mode,application_mode;
    icf_internal_list_st	 *p_list = ICF_NULL;
    icf_stream_sdp_info_st   *p_stream_sdp_info = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb) 
        /*function entry trace*/

    p_list = p_pdb->p_call_ctx->p_stream_sdp_info;

    while(ICF_NULL != p_list)
    {
        p_stream_sdp_info = p_list->p_payload;

        if (ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type)
        {
            remote_media_mode = 
                p_stream_sdp_info->p_remote_received_sdp->media_mode;
            application_mode = p_stream_sdp_info->application_mode;

            break;
        }

        p_list = p_list->p_next;
    }

    if(ICF_MEDIA_MODE_SEND_ONLY == remote_media_mode)
    {
        applied_media_mode = ICF_MEDIA_MODE_RECV_ONLY;
    }
    else if(ICF_MEDIA_MODE_RECV_ONLY == remote_media_mode)
    {
        applied_media_mode = application_mode;
    }
    else
    {
        applied_media_mode = remote_media_mode;
    }
   	
   ret_val = ICF_SUCCESS;    
   ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    /* function exit trace */
	return applied_media_mode;

}

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_compare_tranport_addr
*
* DESCRIPTION  : This function is used to compare two transport addresses and
*                return false if different else returns true
*
* RETURNS:
******************************************************************************/
icf_boolean_t  icf_chr_compare_tranport_addr(
        INOUT icf_chr_pdb_st       *p_pdb,
        IN  icf_transport_address_st  *p_addr_first,
        IN  icf_transport_address_st  *p_addr_second)
{
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_boolean_t     result = ICF_TRUE;
    icf_uint32_t      first_ip_addr, sec_ip_addr;

    if(ICF_NULL == p_pdb)
    {
         return ICF_FAILURE;
    }
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    if (p_addr_first->addr.addr_type != p_addr_second->addr.addr_type)
    {
        /*Address types are not same*/
        result = ICF_FALSE;
    } /* if(p_addr_first->addr_type != ... */
    else if(p_addr_first->port_num != p_addr_second->port_num)
    {
        /*Both port_num are not same*/
        result = ICF_FALSE;
    }
    else if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
            p_addr_first->addr.addr_type)
    {
        /*Compare both address' domain names*/
        if(!icf_port_strcmp(p_addr_first->addr.addr.domain.str, 
                    p_addr_second->addr.addr.domain.str))
        {
            /*Both domain names are not same*/
            result = ICF_FALSE;
        } /* if(!icf_port_strcmp(p_addr_... */
    } /* else if (ICF_TRANSPORT_ADDR... */
    else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
            p_addr_first->addr.addr_type)
    {
        /*Convert ip_addr of first address to 32-bit integer*/
        first_ip_addr = ICF_MAKE_32BIT_INT(
                p_addr_first->addr.addr.ipv4_addr.octet_1,
                p_addr_first->addr.addr.ipv4_addr.octet_2,
                p_addr_first->addr.addr.ipv4_addr.octet_3,
                p_addr_first->addr.addr.ipv4_addr.octet_4);

        /*Convert ip_addr of second address to 32-bit integer*/
        sec_ip_addr = ICF_MAKE_32BIT_INT(
                p_addr_second->addr.addr.ipv4_addr.octet_1,
                p_addr_second->addr.addr.ipv4_addr.octet_2,
                p_addr_second->addr.addr.ipv4_addr.octet_3,
                p_addr_second->addr.addr.ipv4_addr.octet_4);

        /*Compare both IP addresses*/
        if (first_ip_addr != sec_ip_addr)
        {
            /*Both IP addresses are not same*/
            result = ICF_FALSE;
        } /* if (first_ip_addr != sec_ip_ad... */
    } /* p_addr_first->addr.addr_type) */  
   
	ret_val = ICF_SUCCESS;  
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return result;
} /* IN  icf_transport_addr_st  ... */

/*****************************************************************************
 * FUNCTION NAME    :   icf_chr_cc_send_reinvite_msg
 * DESCRIPTION  : This function will send the reinvite message to CC
 * RETURNS:  
 ****************************************************************************/

icf_return_t icf_chr_cc_send_reinvite_msg
(
   INOUT  icf_chr_pdb_st *p_pdb
)
{
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;
    icf_return_t             ret_val = ICF_SUCCESS;
    /* icf_error_t              ecode = ICF_NULL; */
    /* icf_nw_inc_media_change_req_st  med_change; */

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    /*
     * Reset call proc signalling
     */
    ICF_CHR_RESET_STATUS(p_pdb, 
            ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
    
    /*If Signaling ongoing in some other module*/
    if(ICF_INT_MODULE_INVALID == ICF_CHR_GET_SIG_ONGOING(p_pdb))
    {
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_SIG_ONGOING)
    /*
     * Set  ICF_STATUS_REINVITE_REQUEST_TO_CC in CHR service
     */
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_CC_HANDLE_REINVITE);

        if(p_pdb->p_call_ctx->common_bitmask_2 & 
            ICF_CC_BUFFERED_INC_MEDIA_UPDATE)
        {
            p_pdb->p_internal_msg->p_msg_data = 
                p_pdb->p_chr_ctx->p_pending_req;
        }
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb);
    return ret_val;
}

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize CHR context data
*
* RETURNS:
******************************************************************************/

icf_void_t  icf_chr_deinit_ctx(
        icf_chr_pdb_st  *p_pdb
        )
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_error_t   *p_ecode = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
   
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    p_pdb->p_chr_ctx->pending_event = ICF_CHR_PE_INVALID;
    p_pdb->p_chr_ctx->ongoing_action = ICF_CHR_OA_INVALID;
    p_pdb->p_chr_ctx->internal_resume_resp = 0;
    p_pdb->p_chr_ctx->internal_hold_resp = 0;
    p_pdb->p_chr_ctx->state = ICF_CHR_00_IDLE;

    if(ICF_NULL != p_pdb->p_chr_ctx->p_chr_timer_data)
    {
        if (ICF_NULL != p_pdb->p_chr_ctx->chr_timer_id)
        {
            ICF_STOP_TIMER(p_glb_pdb,
                p_pdb->p_chr_ctx->chr_timer_id, p_ecode, ret_val)
        }

        /*Free CHR timer block*/
        icf_dbm_ret_mem_to_pool(p_glb_pdb, 
                (icf_pool_id_t)ICF_MEM_TIMER,
                (icf_void_t *)(p_pdb->p_chr_ctx->p_chr_timer_data), 
                p_ecode);

        p_pdb->p_chr_ctx->p_chr_timer_data = ICF_NULL;
    }

    if(ICF_NULL != p_pdb->p_chr_ctx->p_race_timer_data)
    {
        /*Stop reinvite race timer*/
        ICF_STOP_TIMER(p_glb_pdb,
                p_pdb->p_chr_ctx->race_timer_id, p_ecode, ret_val)

        ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)
        
        /*Free race timer block*/
        icf_dbm_ret_mem_to_pool(p_glb_pdb, 
                (icf_pool_id_t)ICF_MEM_TIMER,
                (icf_void_t *)(p_pdb->p_chr_ctx->p_race_timer_data), 
                p_ecode);
        
        p_pdb->p_chr_ctx->p_race_timer_data = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
} /* icf_chr_deinit_ctx */

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_internal_req_handler
*
* DESCRIPTION  : This function is used to initialize internal call hold and
*                resume data
*
* RETURNS:
******************************************************************************/
icf_boolean_t  icf_chr_internal_req_handler(
        INOUT icf_chr_pdb_st   *p_pdb,
        INOUT icf_uint32_t      event
        )
{
    icf_boolean_t    ret_val = ICF_FALSE;
   
    /*When internal event received mark it in CHR context and also
     * check if service already not ongoing then return TRUE so that
     * new CHR service can be initiated*/

    /* Internal call hold or resume request recvd*/
    if (ICF_SC_INTERNAL_CALL_HOLD_REQ == event)
    {
        p_pdb->p_chr_ctx->internal_hold_resp = ICF_TRUE;
        
        /* Fail the req if Call Hold is already OA or if there is some OA and 
         * call hold is pending
         */
        if ((ICF_CHR_OA_CALL_HOLD == p_pdb->p_chr_ctx->ongoing_action) ||
            ((ICF_CHR_OA_INVALID != p_pdb->p_chr_ctx->ongoing_action) &&
            (ICF_CHR_PE_CALL_HOLD == p_pdb->p_chr_ctx->pending_event)))
        {
            ret_val = ICF_FALSE;
        } /* if (ICF_CHR_OA_CALL_HOLD ==... */

        /* If there is no ongoing action and call hold is pending then this
         * is the case when call hold is buffered due to call hold req while
         * TWC/RNW is ongoing. Set the flag in the pdb to send immidiate
         * success response of the request. The pending event will be 
         * processed when TWC/RNW will be over
         */
        else
        {
            ret_val = ICF_TRUE;

            if ((ICF_CHR_OA_INVALID == p_pdb->p_chr_ctx->ongoing_action) &&
                (ICF_CHR_PE_CALL_HOLD == p_pdb->p_chr_ctx->pending_event))
            {
                p_pdb->imm_internal_req_resp = ICF_TRUE;
            }
        } /* else */        
    } /* if (ICF_TRUE == p_pdb->inte... */
    else if (ICF_SC_INTERNAL_CALL_RESUME_REQ == event)
    {
        p_pdb->p_chr_ctx->internal_resume_resp = ICF_TRUE;
         
        /* Fail the req if call resume is the ongoing action or if no action
         * is ongoing and call resume is pending
         */
        if ((ICF_CHR_OA_CALL_RESUME == p_pdb->p_chr_ctx->ongoing_action) ||
            ((ICF_CHR_OA_INVALID != p_pdb->p_chr_ctx->ongoing_action)
            && (ICF_CHR_PE_CALL_RESUME == p_pdb->p_chr_ctx->pending_event)))
        {
            ret_val = ICF_FALSE;
        } /* if (ICF_CHR_OA_CALL_HOLD ==... */

        else
        {
            ret_val = ICF_TRUE;

            /* Set the flag to send the immidiate success resp for the 
             * req if no action is ongoing and call resume is pending
             */
            if ((ICF_CHR_OA_INVALID == p_pdb->p_chr_ctx->ongoing_action) &&
                (ICF_CHR_PE_CALL_RESUME == p_pdb->p_chr_ctx->pending_event))
            {
                p_pdb->imm_internal_req_resp = ICF_TRUE;
            }
        } /* else */        
    } /* else if (ICF_TRUE == p_pdb-... */
    else
    {
        ret_val = ICF_FALSE;
    } /* else */
    
    return ret_val;    
} /* icf_chr_internal_req_handler */

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_assign_init_data
*
* DESCRIPTION  : This function is used to initialize CHR data
*
* RETURNS:
******************************************************************************/
icf_return_t   icf_chr_init_pdb_data(
        INOUT  icf_chr_pdb_st          *p_pdb,
        INOUT  icf_internal_msg_st     *p_internal_msg)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;
   
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* Copy call context pointer to p_pdb */
    p_pdb->p_call_ctx = p_glb_pdb->p_call_ctx;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pdb->p_call_ctx))

    /* Copy service context pointer to p_pdb */
    p_pdb->p_service_ctx = p_glb_pdb->p_call_ctx->p_service_ctx;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pdb->p_service_ctx))

    /* Copy CHR context pointer to p_pdb */
    p_pdb->p_chr_ctx = 
    (icf_chr_ctx_st *)p_pdb->p_service_ctx->active_service_list.
    p_sl_ctx[(ICF_INT_MODULE_CHR - ICF_INT_MODULE_SL_START) - 1];

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pdb->p_chr_ctx))

	p_pdb->imm_internal_req_resp = ICF_FALSE;
    p_pdb->mm_err_recvd = ICF_FALSE;
    p_pdb->sdp_present = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* INOUT  icf_internal_msg_st ... */

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_trace
*
* DESCRIPTION  : This function is used to initialize trace data
*
* RETURNS:
******************************************************************************/
#ifdef ICF_TRACE_ENABLE
icf_void_t icf_chr_trace(
        INOUT icf_chr_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    /*  SPR 18064 Merge */
    icf_trace_data_st      trace_data;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */ 

    switch(trace_id)
    {
        case ICF_TRACE_CALL_HOLD_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;          
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
            (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CALL_HOLD_SUCC_RESP:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CALL_RESUME_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;

        case ICF_TRACE_MMS_SEND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_HOLD_REQ_TO_NW:     
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_HLD_SUCC_FROM_NW:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_RSM_REQ_TO_NW:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_RSM_RESP_FROM_NW:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_HLD_FAIL_FROM_NW:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_REQ_CANCEL_SEND: 
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CANCEL_RESP_RECV:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_MED_ERR_DMS_IND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_MED_ERR_MMS_FAIL:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CHR_TIMER_EXPD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_RACE_TIMER_EXPD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_ST_PROC_SIG_RCVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_ST_PROC_MED_RCVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CALL_RELEASED: 
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CALL_ALREADY_HOLD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_SERVICE_TERM_NORMAL:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_SERVICE_TERM_ERROR:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CALL_REMOTE_HOLD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_SIG_ONGOING:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_SET_ST_PROC_SIG: 
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_SSA_FAIL_RESP:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_SSA_SUCC_RESP:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CHR_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            icf_chr_state_trace_string(p_pdb->p_chr_ctx->state, &trace_data);
            break;
            
        case ICF_TRACE_CHR_EVENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            icf_chr_event_trace_string(p_pdb->p_chr_ctx->event, &trace_data);
            break;
            
        case ICF_TRACE_ONGOING_ACT:   
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_chr_ctx->ongoing_action;
            break;
            
        case ICF_TRACE_PENDING_EVT: 
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_chr_ctx->pending_event;
            break;
            
        case ICF_TRACE_MMI_FAIL_RESP: 
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_MMI_SUCC_RESP:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_SET_ST_PROC_MED:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_MED_ONGOING:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_CALL_ALRADY_ACTIVE: 
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;
            break;
            
        case ICF_TRACE_MMS_SUCC_RESP:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_call_ctx->call_id;            
            break;
            
        case ICF_TRACE_CURR_RET_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_internal_msg->msg_hdr.status;
            break;

        case ICF_TRACE_NO_SDP_PRESENT:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
        
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    }

    ICF_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb, 
            trace_id, &trace_data)
} /* IN    icf_trace_id_t     tr... */

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_chr_state_trace_string(
        IN    icf_uint8_t          state, 
        INOUT icf_trace_data_st   *p_trace_data)
{

    /*Switch for trace_id*/
    switch(state)
    {
        case ICF_CHR_00_IDLE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_00_IDLE");
            break;
            
        case ICF_CHR_01_AWT_MMS_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_01_AWT_MMS_RESP");
            break;

        case ICF_CHR_02_AWT_NW_ACK:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_02_AWT_NW_ACK");
            break;

        case ICF_CHR_03_AWT_CANCEL_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_03_AWT_CANCEL_RESP");
            break;

        case ICF_CHR_04_AWT_ST_PROC_SIG:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_04_AWT_ST_PROC_SIG");
            break;

        case ICF_CHR_05_AWT_ST_PROC_MEDIA:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_05_AWT_ST_PROC_MEDIA");
            break;

        default:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"Invalid state");
            break;       

    } /* switch(trace_id) */
    
}/*icf_chr_state_trace_string*/

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_chr_event_trace_string(
        IN    icf_uint8_t         event, 
        INOUT icf_trace_data_st   *p_trace_data)
{
    switch(event)
    {
        case ICF_CHR_CALL_HOLD:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_CALL_HOLD");
            break;

        case ICF_CHR_MMS_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_MMS_RESP");
            break;

        case ICF_CHR_MEDIA_ERROR:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_MEDIA_ERROR");
            break;

        case ICF_CHR_NW_HLD_RSM_SUC_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_NW_HLD_RSM_SUC_RESP");
            break;

        case ICF_CHR_CALL_RESUME:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_CALL_RESUME");
            break;

        case ICF_CHR_HLD_RSM_REQ_FROM_NW:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_HLD_RSM_REQ_FROM_NW");
            break;

        case ICF_CHR_NW_ACK_FOR_REQ_CNL:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_NW_ACK_FOR_REQ_CNL");
            break;

        case ICF_CHR_NW_OG_SETUP_FAIL:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_NW_OG_SETUP_FAIL");
            break;

        case ICF_CHR_TIMER_EXPIRY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_TIMER_EXPIRY");
            break;

        case ICF_CHR_CALL_RELEASED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_CALL_RELEASED");
            break;

        case ICF_CHR_ST_PROC_SIG:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_ST_PROC_SIG");
            break;

        case ICF_CHR_ST_PROC_MEDIA:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CHR_ST_PROC_MEDIA");
            break;

        default:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"Invalid Event");
            break;
    } /* switch(trace_id) */
} /* INOUT icf_trace_data_st   *... */
#endif


/******************************************************************************
*
* FUNCTION NAME: icf_chr_clone_app_spec_hdr_lists
*
* DESCRIPTION  : This function is used to update application specific headers
*                in call context
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_clone_app_spec_hdr_lists(
    icf_chr_pdb_st   *p_chr_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = p_chr_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_call_hold_req_st *p_chr_msg = 
        (icf_call_hold_req_st *)&(p_glb_pdb->p_recv_msg->payload[0]);
    
    /*SPR 19590 -If any of the header/body/tag lists are present in the
      CH request then we shall increment the txn id in call ctx
      so that a unique node is created for the hdr/body/tag list
      in their respective lists of list in call ctx. Additionally
     we should always increment the last_txn_id which is global for 
     call and then copy the same in txn_id because txn_id is for the
     current node and not necessary its next number is unused.*/
    if((ICF_HDR_LIST_PRESENT & p_chr_msg->bit_mask) ||
       (ICF_TAG_LIST_PRESENT & p_chr_msg->bit_mask) ||    
       (ICF_BODY_LIST_PRESENT & p_chr_msg->bit_mask))
    {
        if(ICF_NULL != p_glb_pdb->p_call_ctx)
        {    
            p_glb_pdb->p_call_ctx->last_txn_id++;
            p_glb_pdb->p_call_ctx->txn_id = 
               p_glb_pdb->p_call_ctx->last_txn_id;
        }   
    }
    /* clone the header list into call ctx, if received from app */
	if (ICF_HDR_LIST_PRESENT & p_chr_msg->bit_mask)
    {
        /*SPR 19590 change start */
        ret_val = icf_cmn_make_hdr_list_node(
                     p_glb_pdb,
                     &(p_chr_msg->header_list));
        /*SPR 19590 change end */
    }
    
    /* clone the tag list into call ctx, if received from app */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_TAG_LIST_PRESENT & p_chr_msg->bit_mask))
    {
        /*SPR 19590 change start */
        ret_val = icf_cmn_make_tag_list_node(
                     p_glb_pdb,
                     &(p_chr_msg->tag_list));
        /*SPR 19590 change end */
    }
    
    /* clone the body list into call ctx, if received from app */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_BODY_LIST_PRESENT & p_chr_msg->bit_mask))
    {
        /*SPR 19590 change start */
        ret_val = icf_cmn_make_body_list_node(
                     p_glb_pdb,
                     &(p_chr_msg->body_list));
        /*SPR 19590 change end */
    }
    return ret_val;
}

/******************************************************************************
*
* FUNCTION NAME: icf_chr_compare_audio_profile
*
* DESCRIPTION  : This function is used to compare audio profiles
*
* RETURNS:      ICF_TRUE/ICF_FALSE
******************************************************************************/
icf_boolean_t icf_chr_compare_audio_profile(icf_chr_pdb_st *p_pdb,
                    icf_audio_profile_st  *p_audio_profile_1,
                    icf_audio_profile_st  *p_audio_profile_2)
{
    icf_boolean_t    result = ICF_TRUE;
    /* To remove warnings */
     p_pdb=p_pdb;
    
    if (p_audio_profile_1->bit_mask != p_audio_profile_2->bit_mask)
    {
        result = ICF_FALSE;
    }
    if ((ICF_FALSE != result) && 
        (ICF_AUDIO_PROF_BNDWDTH_PRESENT & p_audio_profile_1->bit_mask) &&
        (p_audio_profile_1->bandwidth != p_audio_profile_2->bandwidth))
    {        
        result = ICF_FALSE;            
    }
    if ((ICF_FALSE != result) && 
        (ICF_AUDIO_PROF_PTIME_PRESENT & p_audio_profile_1->bit_mask) &&
        (p_audio_profile_1->pref_ptime != p_audio_profile_2->pref_ptime))
    {
        result = ICF_FALSE;
    }
    if ((ICF_FALSE != result) && 
        (ICF_AUDIO_PROF_SILN_SUPP_PRESENT & p_audio_profile_1->bit_mask) &&
        (p_audio_profile_1->silence_suppression != 
            p_audio_profile_2->silence_suppression))
    {
        result = ICF_FALSE;
    }
    return result;
}


/******************************************************************************
*
* FUNCTION NAME: icf_chr_compare_codec_list
*
* DESCRIPTION  : This function is used to compare codec lists
*
* RETURNS:      
******************************************************************************/
icf_return_t icf_chr_compare_codec_list(icf_chr_pdb_st *p_pdb,
                    icf_list_st  *p_codec_list_1,
                    icf_list_st  *p_codec_list_2)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_codec_attrib_st *p_codec_1 = ICF_NULL;
    icf_codec_attrib_st *p_codec_2 = ICF_NULL;

     /* To remove warnings */
     p_pdb=p_pdb;

    while (ICF_NULL != p_codec_list_1)
    {
        if (ICF_NULL == p_codec_list_2)
        {
            ret_val = ICF_FAILURE;
            break;
        }
        p_codec_1 = p_codec_list_1->p_data;
        p_codec_2 = p_codec_list_2->p_data;
        
        /*if (p_codec_1->bit_mask !=  p_codec_2->bit_mask)
        {
            ret_val = ICF_FAILURE;
            break;
        }*/

		if ((ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT ==
				(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_1->bit_mask ) &&
				(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT ==
				(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_2->bit_mask )))
			&& (p_codec_1->codec_num != p_codec_2->codec_num))
		{
				ret_val = ICF_FAILURE;
				break;
		}
		/* If codec number is not same check there name only if codec numbers are greater 
		 * then 95 */
		else if(p_codec_1->codec_num > 95)
		{

		   if (ICF_NULL != (icf_port_strcmp(p_codec_1->codec_name.str,
	  			p_codec_2->codec_name.str)))
		   {
				ret_val = ICF_FAILURE;
				break;
		   }
		}

        p_codec_list_1 = p_codec_list_1->p_next;
        p_codec_list_2 = p_codec_list_2->p_next;
    }
    if ((ICF_SUCCESS == ret_val) && (ICF_NULL != p_codec_list_2))
    {
        ret_val = ICF_FAILURE;
    }
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_chr_copy_internal_sdp_stream
 *
 * DESCRIPTION:     This function will be copying internal_sdp_stream 
 ******************************************************************************/
icf_return_t
icf_chr_copy_remote_sdp(
	                icf_chr_pdb_st	             *p_pdb)
{
	icf_return_t	            ret_val = ICF_SUCCESS;
    icf_error_t                 *p_ecode = 
        &(p_pdb->p_internal_msg->msg_hdr.ecode);	
    icf_glb_pdb_st              *p_glb_pdb = 
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    icf_internal_sdp_stream_st   *p_src = ICF_NULL;
    icf_nw_inc_media_change_resp_st *p_nw_inc_msg = 
        (icf_nw_inc_media_change_resp_st*)p_pdb->p_internal_msg->p_msg_data;
    icf_list_st                  *p_list = ICF_NULL;
    icf_internal_list_st         *p_tmp_node = ICF_NULL;
    icf_stream_id_t              stream_id = 0;
    icf_uint32_t                 loc_stream = ICF_NULL;
    icf_media_mode_t             media_mode = ICF_MEDIA_MODE_INVALID;
    
    p_list = p_nw_inc_msg->p_sdp_list;

    if (ICF_NULL == p_nw_inc_msg->p_sdp_list)
    {
        p_pdb->sdp_present = ICF_FALSE;
        return ret_val;
    }
    p_pdb->sdp_present = ICF_TRUE;
    while(ICF_NULL != p_list)
    {
        if (ICF_STREAM_TYPE_AUDIO == 
            ((icf_inc_media_sdp_info_st*)(p_list->p_data))->stream_type)
        {
            p_src = 
                ((icf_inc_media_sdp_info_st*)(p_list->p_data))->p_int_sdp_stream;

            stream_id = ((icf_inc_media_sdp_info_st*)(p_list->p_data))->stream_id;
            loc_stream = stream_id;
            p_tmp_node = icf_cmn_get_node_in_internal_list(
					p_glb_pdb->p_call_ctx->p_stream_sdp_info,
					&loc_stream);

            if (ICF_NULL != p_tmp_node)
            {
					/* Delete the already existing internal sdp stream, if it exists.*/
					if (ICF_NULL != 
					   ((icf_stream_sdp_info_st*)((p_tmp_node)->p_payload))->p_remote_received_sdp)
					{
                         /* Copying the remote sdp mode to save it */
					     media_mode = ((icf_stream_sdp_info_st*)((p_tmp_node)->
                                               p_payload))->p_remote_received_sdp->media_mode;
							ret_val = icf_cmn_delete_internal_sdp_stream(p_glb_pdb,
											&(((icf_stream_sdp_info_st*)
											((p_tmp_node)->p_payload))->p_remote_received_sdp),
											p_ecode);
                            if (ICF_FAILURE == ret_val)
                                break;
					}
  
					((icf_stream_sdp_info_st*)((p_tmp_node)->p_payload))->p_remote_received_sdp = p_src;
                    
                    /* Copying the remote sdp mode to save it */
                    if (ICF_CHR_OA_CALL_HOLD & ICF_CHR_GET_ONGOING_ACT(p_pdb))
                    {
				        ((icf_stream_sdp_info_st*)((p_tmp_node)->p_payload))->
                                   p_remote_received_sdp->media_mode = media_mode;
                    } 
					/*Deleting p_int_sdp_stream present in icf_inc_media_sdp_info*/
                    ((icf_inc_media_sdp_info_st*)(p_list->p_data))->p_int_sdp_stream = ICF_NULL;
/*                    ((icf_stream_sdp_info_st*)((p_tmp_node)->p_payload))->p_applied_sdp->media_mode =
                           p_src->media_mode; PANKAJ */
            }            
            break;
        }
        p_list = p_list->p_next;
    }

	ret_val = icf_cmn_delete_list(
                p_glb_pdb,&(p_nw_inc_msg->p_sdp_list),ICF_MEM_COMMON);

    ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_chr_check_remote_status
 *
 * DESCRIPTION:     This function will return success if remote mode is inactive
 ******************************************************************************/
icf_return_t   icf_chr_check_remote_status(
                icf_chr_pdb_st  *p_pdb)
{
    icf_return_t	            ret_val = ICF_FAILURE;    
    icf_glb_pdb_st              *p_glb_pdb = 
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;    
    icf_internal_list_st         *p_list = ICF_NULL;
    icf_stream_sdp_info_st       *p_stream_sdp = ICF_NULL;

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if (ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_id)
        {
            if (ICF_MEDIA_MODE_INACTIVE == 
                p_stream_sdp->p_applied_sdp->media_mode)
            {
                ret_val = ICF_SUCCESS;
            }
            break;
        }
        p_list = p_list->p_next;
    }
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_chr_check_mms_resp
 *
 * DESCRIPTION:     This function will check MMS response is failure or success
 ******************************************************************************/
icf_return_t   icf_chr_check_mms_resp(
                icf_chr_pdb_st  *p_pdb,
                icf_uint8_t     *p_event)
{
    icf_return_t	            ret_val = ICF_SUCCESS;    
    icf_glb_pdb_st              *p_glb_pdb = 
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_mmi_mms_resp_st         *p_mms_resp = ICF_NULL;

    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_mms_resp = 
        (icf_mmi_mms_resp_st*)(p_pdb->p_internal_msg->p_msg_data);

    *p_event = ICF_CHR_MMS_RESP;

    if (ICF_FAILURE == p_mms_resp->result)
    {
        p_pdb->mm_err_recvd = ICF_FALSE;
        *p_event = ICF_CHR_MEDIA_ERROR;
    }
    else if (ICF_MMI_MMS_ACTION_MODIFY_RESP & p_mms_resp->bit_mask)
    {
        if (ICF_FALSE == p_mms_resp->modified_stream_resp[0].status_success)
        {
            p_pdb->mm_err_recvd = ICF_FALSE;
            *p_event = ICF_CHR_MEDIA_ERROR;
        }
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)    
    return ret_val;
}


/******************************************************************************
*
* FUNCTION NAME: icf_chr_send_app_resp_on_call_term
*
* DESCRIPTION  : This function is used to send response to the application when 
*                application has requested hold/resume but call termination is 
*                triggered.
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_send_app_resp_on_call_term(
    INOUT  icf_chr_pdb_st   *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_error_t    send_error = ICF_CAUSE_NETWORK_ERROR;
  
    /* to remove compiler warning */
    p_glb_pdb = p_glb_pdb;

    if (ICF_REL_TYPE_MM_INITIATED == p_glb_pdb->p_call_ctx->release_type)
    {
         send_error = ICF_CAUSE_MEDIA_FAILURE;
    }

    if (ICF_CHR_OA_INVALID != ICF_CHR_GET_ONGOING_ACT(p_pdb))
    {
        if (ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp)
          {
            /*Send call resume success response to PA*/
            ret_val = icf_chr_send_hold_resp(p_pdb,
                           (icf_boolean_t)ICF_SUCCESS,send_error);
          }

        }
        if (ICF_CHR_OA_MMS_FOR_RESUME_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp)
          {
            /*Send call resume success response to PA*/
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                           (icf_boolean_t)ICF_SUCCESS, send_error);
          }

       }

        /*If network outgoing setup failure response received and hold or resume
         * is not pending then service terminated normal*/
        else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
              ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD_NO_NW_OP ==
              ICF_CHR_GET_PEND_EVT(p_pdb)))
        {
         if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp)
         {
           /*Send failure resp for call hold*/

           ret_val = icf_chr_send_hold_resp(p_pdb, 
              (icf_boolean_t)ICF_FAILURE, send_error);
         }

        }
        else if ((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
              ICF_CHR_GET_ONGOING_ACT(p_pdb))
            &&  (ICF_CHR_PE_CALL_RESUME_NO_NW_OP ==
              ICF_CHR_GET_PEND_EVT(p_pdb)))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp)
          {
            /*Send failure resp for call resume*/
            ret_val = icf_chr_send_resume_resp(p_pdb, 
              (icf_boolean_t)ICF_FAILURE, send_error);
          }

        } /* ICF_CHR_GET_PEND_EVT(p_pdb)... */    
        /*If MMS is due to nw req for hold or resume failure*/
        else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
              ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb)))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp)
          {
           /*If pending_event is call resume, send call hold and resume success
            * response to PA*/
            if(ICF_FAILURE == icf_chr_send_hold_resp(p_pdb, 
                  (icf_boolean_t)ICF_SUCCESS, send_error))
            {
              ret_val = ICF_FAILURE;
            }
          }
          if ((ICF_FAILURE != ret_val) && 
              (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp))
          {
            if(ICF_FAILURE == icf_chr_send_resume_resp(p_pdb,
                  (icf_boolean_t)ICF_SUCCESS, send_error))
            {
              ret_val = ICF_FAILURE;
            }
          }

        } /* If nw og setup is ongoing */
        /*If pending_event is call hold, send call resume and hold
         * success response to PA*/
        else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
              ICF_CHR_GET_ONGOING_ACT(p_pdb)) && 
            (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb)))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp)
          {
          /* Send call resume reponse */            
            if(ICF_FAILURE == icf_chr_send_resume_resp(p_pdb, 
                  (icf_boolean_t)ICF_SUCCESS, send_error))
            {
              ret_val = ICF_FAILURE;
            }
          }
         if ((ICF_FAILURE != ret_val) &&
             (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp))
         {
          if(ICF_FAILURE == icf_chr_send_hold_resp(p_pdb,
                  (icf_boolean_t)ICF_SUCCESS, send_error))
            {
              ret_val = ICF_FAILURE;
            }
         }

        } /* (ICF_CHR_PE_CALL_HOLD == IP... */    
        /*If call hold is pending*/
        else if(ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp)
          {
            /*Send success response of call resume */        
            if(ICF_FAILURE == icf_chr_send_resume_resp(p_pdb, 
                  (icf_boolean_t)ICF_SUCCESS, send_error))
            {
              ret_val = ICF_FAILURE;
            } /* ICF_SUCCESS, ICF_ERR_NO_... */
          }
          if ((ICF_FAILURE != ret_val) &&
             (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp))
          {
             if(ICF_FAILURE == icf_chr_send_hold_resp(p_pdb,
                  (icf_boolean_t)ICF_SUCCESS, send_error))
             {
              ret_val = ICF_FAILURE;
             }
          }
        } /* else if(ICF_CHR_PE_CALL_HOL... */    
        /*If call resume is ongoing*/
        else if(ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
         if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp)
         {
            /*Send success response of call hold */        
            if(ICF_FAILURE == icf_chr_send_hold_resp(p_pdb, 
                  (icf_boolean_t)ICF_SUCCESS, send_error))
            {
              ret_val = ICF_FAILURE;
            } /* ICF_SUCCESS, ICF_ERR_NO_... */
         }
         if ((ICF_FAILURE != ret_val) &&
             (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp))
          {
            /*Send success response of call resume */        
            if(ICF_FAILURE == icf_chr_send_resume_resp(p_pdb, 
                  (icf_boolean_t)ICF_SUCCESS, send_error))
            {
              ret_val = ICF_FAILURE;
            } /* ICF_SUCCESS, ICF_ERR_NO_... */
          }
        } /* else */   
        else if(ICF_CHR_PE_CALL_HOLD_NO_NW_OP == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
         if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp)
         {
            /*Send success response of call hold */        
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS, send_error);
         }

        }
        else if(ICF_CHR_PE_CALL_RESUME_NO_NW_OP == 
            ICF_CHR_GET_PEND_EVT(p_pdb))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp)
          {
            /*Send success response of call resume */        
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS, send_error);
          }

        }

        /* If the call hold is ongoing then send hold response */
        else if (ICF_CHR_OA_CALL_HOLD & ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_hold_resp)
          {
            /*Send call hold failure to PA*/
            ret_val = icf_chr_send_hold_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE, send_error);
          }
        }
        /* If the resume is ongoing then send resume response */
        else if (ICF_CHR_OA_CALL_RESUME & ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
          if (ICF_FALSE ==  p_pdb->p_chr_ctx->internal_resume_resp)
          {
            /*Send call resume failure to PA*/
            ret_val = icf_chr_send_resume_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE, send_error);
          }
        }

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)

    } /* if (ICF_CHR_OA_INVALID != ICF_CHR_GET_ONGOING_ACT(p_pdb)) */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/******************************************************************************
*
* FUNCTION NAME: icf_chr_is_given_audio_mode
*
* DESCRIPTION  : This function is checks whether AUDIO mode is same as given mode
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_is_given_audio_mode(
    INOUT  icf_chr_pdb_st   *p_pdb,
    icf_media_mode_t        media_mode)
{
    icf_return_t   ret_val = ICF_FAILURE;
    icf_glb_pdb_st              *p_glb_pdb =
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_internal_list_st         *p_list = ICF_NULL;
    icf_stream_sdp_info_st       *p_stream_sdp = ICF_NULL;

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if (ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_id)
        {
            if (media_mode == p_stream_sdp->application_mode)
            {
                ret_val = ICF_SUCCESS;
            }
            break;
        }
        p_list = p_list->p_next;
    }
    return ret_val;
}


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_app_trace
*
* DESCRIPTION  : This function is used to initialize trace data
*
* RETURNS:
******************************************************************************/

#ifdef ICF_TRACE_ENABLE
icf_return_t icf_chr_app_trace(
        INOUT icf_chr_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data;
    icf_trace_criteria_data_st    *p_criteria_data = p_pdb->p_internal_msg->
                           msg_hdr.p_glb_pdb->p_call_ctx->p_trace_criteria_data;
    /* Fix for CSR1-5249829 */
    icf_return_t ret_val = ICF_SUCCESS;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */

    ret_val = icf_cmn_check_app_trace(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
       return ret_val;
    }
    else
    {
    
    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->app_id;

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id;

    switch(trace_id)
    {
        case ICF_TRACE_CALL_HOLD_REQ:
        case ICF_TRACE_CALL_RESUME_REQ:
        case ICF_TRACE_CALL_HOLD_RESUME_TIMER_EXPIRE:

            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
	    break; 

        case ICF_TRACE_CALL_HOLD_RESP:
        case ICF_TRACE_CALL_RESUME_RESP:

            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            trace_data.int_val[0] = p_pdb->result;

            trace_data.int_val[1] = p_pdb->error_cause;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;

        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    }

    ICF_APP_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data, p_criteria_data)
}
    return ret_val;
}
#endif

/******************************************************************************
*
* FUNCTION NAME: icf_chr_set_app_media_mode
*
* DESCRIPTION  : This function sets AUDIO mode as given mode
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_set_app_media_mode(
    INOUT  icf_chr_pdb_st   *p_pdb,
    icf_media_mode_t        media_mode)
{
    icf_return_t   ret_val = ICF_FAILURE;
    icf_glb_pdb_st              *p_glb_pdb =
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_internal_list_st         *p_list = ICF_NULL;
    icf_stream_sdp_info_st       *p_stream_sdp = ICF_NULL;

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if (ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_id)
        {
            p_stream_sdp->application_mode = media_mode;
            ret_val = ICF_SUCCESS;
            break;
        }

        p_list = p_list->p_next;
    }
    return ret_val;
}

#endif
