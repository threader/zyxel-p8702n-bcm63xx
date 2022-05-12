/*******************************************************************************
 *
 * FILE NAME    : icf_scm_util.c
 *
 * DESCRIPTION  : Contains the definitions of SCM utility functions
 *
 *
 * Revision History :
 *    DATE            NAME       REFERENCE       REASON
 * --------    ------------      ---------      ---------------
 * 11-Jan-2005    Navneet Gupta    None            Initial
 * 16-Mar-2005    Shefali Aggarwal                 Update for Call forward and
 *                                                 call waiting
 * 05-Apr-2005    Amit Sharma                      Update return status funct
 *                                                 for TWC,CTA, RNW and added
 *                                                 handling for signaling and
 *                                                 media ongoing ended
 * 12-Apr-2005    Amit Sharma                      Updation for error cause sent
 *                                                 to PA, and common funct
 *                                                 called
 * 23-Apr-2005    Amit Sharma     SPR-7425        Initialize status when trigger
 *                                                SLP thru internal request
 * 23-Apr-2005    Amit Sharma     SPR-7426        Set NULL after SLP is dealloc
 * 26-Apr-2005    Amit Sharma     SPR-7421        Checking service subscription
 *                                                and activation for other call
 * 28-Apr-2005    Amit Sharma     SPR-7455        Failure response when other
 *                                                call id is invalid
 * 29-Apr-2005    Amit Sharma     SPR-7461        Handling for ecode generic 
 *                                                error from MMI or SSA
 * 03-Apr-2005    Amit Sharma     SPR-7475        Forward PA call termination
 *                                                to CTU/CTA
 * 10-May-2005    Amit Sharma     SPR-7517        Assign NULL to service_ctx in
 *                                                call ctx when service ctx is 
 *                                                deallocated
 * 15-May-2005    Amit Sharma   SPR:7444          Update STATS
 * 19-May-2005    Amit Sharma   SPR:7598          Change in handling of internal
 *                                                media commit and conf_req
 * 25-May-2005    Amit Sharma   SPR:7567          Return service ended error
 *                                                even if normal is returned
 *                                                SLP in case of forceful call
 *                                                clear event
 * 29-May-2005    Amit Sharma   SPR:7568          Updation for SPR:7568
 * 31-May-2005    Amit Sharma   SPR:7632          
 * 02-Jun-2005    Amit Sharma   SPR:7535          Wait for TWC service term
 * 13-Jun-2005    Amit Sharma   SPR:7517          p_pdb replaced with p_glb_pdb
 * 14-Jun-2005    Amit Sharma   SPR:7662          Fill conf info in both call ctxs
 * 21-Sep-2005    Shefali Aggarwal                Rel 2.0
 * 27-Oct-2005    Amit Sharma   SPR:8083          
 * 27-Oct-2005    Amit Sharma   SPR:8084          
 * 27-Oct-2005    Amit Sharma   SPR:8136          
 * 27-Oct-2005    Amit Sharma   SPR:8140          
 * 27-Oct-2005    Amit Sharma   SPR:8144          
 * 27-Oct-2005    Amit Sharma   SPR:8145          
 * 27-Oct-2005    Amit Sharma   SPR:8147          
 * 27-Oct-2005    Amit Sharma   SPR:8153          
 * 10-Nov-2005    Sumit Gupta   SPR:8261          
 * 24-Nov-2005    Amit Sharma   SPR:8322          
 * 27-Feb-2006  Amit Sharma                  Merged code for ICF
 * 01-Sep-2006    Amit Sharma   SPR:12702          
 * 13-Jul-2007    Abhishek Dhammawat        Addition for XCONF feature Rel7.0
 * 24-Aug-2007  Amit Sharma                     IPTK merge from PQT6.5
 * 10-sept-2007  Abhishek Dhammawat SPR16304 Modified
 *                                           icf_scm_validate_new_service , 
 *                                           icf_scm_route_mesg for
 *                                           ICF_INTERNAL_REFER_REQ case. 
 * 11-sept-2007  Abhishek Dhammawat SPR16315 Modified
 *                                          icf_scm_get_handler_module 
 *                                          for ICF_INTERNAL_SUBSCRIBE_REQ case 
 * 13-Sept-2007  Abhishek Dhammawat SPR16406 Modified icf_scm_route_message for
 *                                           putting xconf in active_service list
 *                                           in case of incoming xconf  
 * 18-OCT-2007   Abhishek Dhammawat SPR17104 Added functionality in
 *                                           icf_scm_check_subs_and_actv
 *                                           for invoking JOIN header service
 *                                           only if APP_ADD does have join in
 *                                           supported event 
 * 26-Jun-2009	Aman Aggarwal	CSR 1-7442376 IPTK shall populate the failure status 
 *											  in case video stream is received from nw.	
 * Copyright 2006,Aricent.
 ******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_dbm_sc_prototypes.h"
#include "icf_scm_types.h"
#include "icf_scm_prototypes.h"
#include "icf_scm_macro.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_validate_new_service 
 *
 * DESCRIPTION   : This function checks if new service can be activated.
 *                 This is decided based on other active services on the 
 *                 call(s) involved.
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/


icf_result_t icf_scm_validate_new_service
(
 INOUT 		icf_scm_pdb_st	*p_pdb,
 INOUT 		icf_boolean_t	*p_valid, 
 INOUT		icf_error_t		*p_ecode
)
{
    icf_call_ctx_st *p_call_ctx = ICF_NULL;
    icf_call_id_t   call_id;
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;
    icf_uint8_t status = 0;
    icf_line_data_st  *p_line_data = ICF_NULL;
    icf_uint8_t       dbm_line_status;
    icf_boolean_t     serv_actv_or_subs = ICF_TRUE;
    icf_uint8_t       service_id;
    icf_uint8_t       count = 0;
    icf_internal_list_st   *p_list = ICF_NULL;
    icf_stream_sdp_info_st *p_stream_sdp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    *p_valid = ICF_TRUE;
    ret_val = icf_scm_check_subs_and_actv(p_pdb,p_valid,p_ecode);

    /*Check if there is only one active audio stream*/
    if ((ICF_SUCCESS == ret_val) &&
        (ICF_USER_INITIATE_EXTERNAL_CONF_REQ != p_pdb->msg_id))

    {
        p_call_ctx = p_glb_pdb->p_call_ctx;

        p_list = p_call_ctx->p_stream_sdp_info;

        while (ICF_NULL != p_list)
        {
            p_stream_sdp = p_list->p_payload;

            if (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)
            {
                count++;
                
                if (ICF_STREAM_TYPE_AUDIO != p_stream_sdp->stream_type)
                {
                    ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_INVALID_SERVICE_INTERACTION)

					/*Set the status as service ended. This shall enable scm module
					 * to trigger failure for the incoming message from the nw.
					 */
					p_pdb->p_msg->msg_hdr.status |= ICF_STATUS_SERVICE_ENDED_ERROR;

					/*In that case, IPTK shall send 488 for the incoming call
					 * CSR 1-7442376
					 */
					p_call_ctx->response_code = 488;

                    *p_valid = ICF_FALSE;
                    break;
                }
                if (1 < count)
                {
                    ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_INVALID_SERVICE_INTERACTION)
					p_pdb->p_msg->msg_hdr.status |= ICF_STATUS_SERVICE_ENDED_ERROR;

					/*In that case, IPTK shall send 488 for the incoming call
					 * CSR 1-7442376
                     */
					p_call_ctx->response_code = 488;

                    *p_valid = ICF_FALSE;
                    break;
                }
            }
            p_list = p_list->p_next;
        }
	count = 0;
    }
    
    if((ICF_SUCCESS == ret_val)
            &&(ICF_TRUE == *p_valid)) 
    {
        if(ICF_NULL !=p_pdb->p_service_ctx)
        {
            status = 
                p_pdb->p_service_ctx->active_service_list.active_service_status;
        } /* if(ICF_NULL !=p_pdb->p_serv... */

        /* Switch API id */
        switch(p_pdb->msg_id)
        {
            /* Internal Call Hold/Resume Requests are always forwarded */
            case ICF_SC_INTERNAL_CALL_HOLD_REQ:
            case ICF_SC_INTERNAL_CALL_RESUME_REQ:
                break;
            case ICF_NEW_SERVICE_TRIGGER:
                /* This API is recvd for CW / CF. This API will always be for 
                 * a new incoming call. The only validation SCM does for this
                 * is that there should be no active services for this call
                 */
                if (ICF_NULL!=status) 
                    *p_valid = ICF_FALSE;
                break;

            case ICF_USER_CALL_HOLD:
            case ICF_USER_CALL_RESUME:
                /* if CTA/CTU is active,then reject this service*/
                if ((status & ICF_SERVICE_CTU) ||
                    (status & ICF_SERVICE_CTA))
                {
                    *p_valid = ICF_FALSE;
                }
                /* In case conference is already active then also this 
                 * request is valid thus the check is not done
                 */

                /* In case TWC or RNW is active then CHR will be triggered
                 * with the buffering event set as true in the interface 
                 * message
                 */
                else if ((status & ICF_SERVICE_TWC) 
                        || (status & ICF_SERVICE_RNW))
                {
                    /* Allocate memory for the p_msg_data */
                    ICF_MEMGET(p_glb_pdb, 
                            sizeof(icf_call_hold_resume_int_payload_st), 
                            ICF_MEM_COMMON, 
                            p_pdb->p_msg->p_msg_data, 
                            ICF_RET_ON_FAILURE, 
                            p_ecode, 
                            ret_val)

                    ((icf_call_hold_resume_int_payload_st *)
                        (p_pdb->p_msg->p_msg_data))->buffer_event = ICF_TRUE;

                    ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(
                            ICF_INT_MODULE_CHR, service_id);
                    p_pdb->p_chr_buff_evt = p_pdb->p_msg->p_msg_data;

                    /* if TWC is active then set the resp_frm_twc to CHR */
                    if (status & ICF_SERVICE_TWC)
                    {
                        p_pdb->p_service_ctx->expected_events.resp_frm_twc = 
                           service_id; 
                    }
                    else
                    {
                        p_pdb->p_service_ctx->expected_events.resp_frm_rnw = 
                            service_id;
                    }
                }

                /* If TWC or RNW are not active then the buffering event
                 * flag is set to false
                 */
                else 
                {
                    /* Allocate memory for the p_msg_data */
                    ICF_MEMGET(p_glb_pdb, 
                            sizeof(icf_call_hold_resume_int_payload_st), 
                            ICF_MEM_COMMON, 
                            p_pdb->p_msg->p_msg_data, 
                            ICF_RET_ON_FAILURE, 
                            p_ecode, 
                            ret_val)

                    ((icf_call_hold_resume_int_payload_st *)
                    (p_pdb->p_msg->p_msg_data))->buffer_event = ICF_FALSE;

                    p_pdb->p_chr_buff_evt = p_pdb->p_msg->p_msg_data;

                }
                break;
              
            case ICF_USER_CALL_TRANSFER_UNATTENDED: 
            case ICF_NW_INC_REFER:
                /* if TWC/CTA/CTU/XCONF/RNW is active,then reject this service*/
                if ((status & ICF_SERVICE_TWC) 
                        || (status & ICF_SERVICE_RNW) 
                        || (status & ICF_SERVICE_CTU) 
                        || (status & ICF_SERVICE_XCONF)
                        || (status & ICF_SERVICE_CTA))
                    *p_valid = ICF_FALSE;
                /*Check if Conference already active*/
                else if (ICF_NULL != (p_glb_pdb->p_call_ctx->call_status &
                            ICF_CALL_STATUS_CONFERENCE))
                {
                    *p_valid = ICF_FALSE;
                }

                break;
                
            /* For join header, internal call id is present in the call context itself */
            case ICF_INVOKE_TWC_FOR_JOIN:
                /* Call Conference Request recieved */
            case ICF_USER_CALL_CONF:
                /* if TWC/CTA/CTU/XCONF/RNW is active,
                    then reject this service*/
                if ((status & ICF_SERVICE_TWC) 
                        || (status & ICF_SERVICE_RNW) 
                        || (status & ICF_SERVICE_CTU) 
                        || (status & ICF_SERVICE_XCONF)
                        || (status & ICF_SERVICE_CTA))
                    *p_valid = ICF_FALSE;

                /* Request Validated for call-id on which Conf Request 
                 * was recieved. Validate other call id */
                else
                {
                    if(ICF_INVOKE_TWC_FOR_JOIN == p_pdb->msg_id)
                    { 
	               call_id = p_glb_pdb->p_call_ctx->call_to_be_joined;
                    }
                    else
                    { 
                    /* get other conference participant call_id from 
                     * external message */
                        call_id = 
                        ((icf_pa_cc_conference_req_st *) 
                         (p_glb_pdb->p_recv_msg->payload))->merge_call_id;
                     } 

                    /*Check if TWC already active*/
                    if (ICF_NULL != (p_glb_pdb->p_call_ctx->call_status &
                            ICF_CALL_STATUS_CONFERENCE))
                    {
                        ICF_SCM_SET_ECODE(p_pdb,
                                ICF_ERROR_INVALID_SERVICE_INTERACTION)
                        *p_valid = ICF_FALSE;
                        break;
                    }
                            
                    if(call_id == p_glb_pdb->p_call_ctx->call_id)
                    { 
                        ICF_SCM_SET_ECODE(p_pdb,
                                ICF_ERROR_INVALID_SERVICE_INTERACTION)
                        *p_valid = ICF_FALSE;
                        break;
                    }
                    
                    /* Find call context for the other Call-id */
                    if(ICF_SUCCESS == 
                            (ret_val = 
                             icf_dbm_find_call_cntxt_blk(
                                 p_glb_pdb,
                                 call_id, 
                                 &p_call_ctx, 
                                 p_ecode)))
                    {
                        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx))

                        /*Check subs and actv for other call id*/
                        ret_val =  icf_dbm_get_line_cntxt_blk(
                                p_glb_pdb,
                                p_call_ctx->line_id,
                                &p_line_data,
                                &dbm_line_status,
                                p_ecode);

                        ICF_ASSERT(p_glb_pdb,(ICF_NULL!=p_line_data))

                        if((ICF_SUCCESS == ret_val))
                        {
                            if(ICF_USER_CALL_CONF == p_pdb->msg_id)
                             { 
                               /* Set is_valid = FALSE and set error code if 
                                * one of the flags is not set */
                               if(ICF_NULL ==
                                (ICF_SERVICE_SUBS_THREE_WAY_CALL & 
                                 p_line_data->service_subscription_flag)) 
                                {
                                   ICF_SCM_SET_ECODE(p_pdb,
                                        ICF_ERROR_SERVICE_NOT_SUBSCRIBED)
                                    
                                   *p_valid = ICF_FALSE;

                                   serv_actv_or_subs = ICF_FALSE;
                                 } /* if(ICF_NULL ==(p_line_data-... */

                                else if(ICF_NULL ==
                                  (ICF_SERVICE_ACT_THREE_WAY_CALL & 
                                   p_line_data->service_activation_flag)) 
                                {
                                  ICF_SCM_SET_ECODE(p_pdb,
                                        ICF_ERROR_SERVICE_NOT_ACTIVE)
                                  *p_valid = ICF_FALSE;
                                  serv_actv_or_subs = ICF_FALSE;
                                } /* && (p_line_data->service_activ... */
                              }
                            if (ICF_NULL != 
                                     (p_call_ctx->call_status &
                                     ICF_CALL_STATUS_CONFERENCE))
                            {
                                ICF_SCM_SET_ECODE(p_pdb,
                                    ICF_ERROR_INVALID_SERVICE_INTERACTION)
                                *p_valid = ICF_FALSE;
                            }
                            else if (p_call_ctx->p_service_ctx != ICF_NULL)
                            {

                                status = ((icf_service_ctx_st *)
                                        p_call_ctx->p_service_ctx)->
                                    active_service_list.active_service_status;
                                if ((status & ICF_SERVICE_TWC) 
                                        ||(status & ICF_SERVICE_RNW)
                                        ||(status & ICF_SERVICE_CTU) 
                                        ||(status & ICF_SERVICE_CTA))
                                    *p_valid = ICF_FALSE;
                                /*Check if TWC already active on other call*/
                            }
                        }

                        if ((ICF_SUCCESS == ret_val) &&
                            (ICF_FALSE != *p_valid))
                        {   
                            p_list = p_call_ctx->p_stream_sdp_info;
                            
                            while (ICF_NULL != p_list)
                            {
                                p_stream_sdp = p_list->p_payload;
                                
                                if (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)
                                {
                                    count++;
                                    
                                    if (ICF_STREAM_TYPE_AUDIO != p_stream_sdp->stream_type)
                                    {
                                        ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_INVALID_SERVICE_INTERACTION)
                                            *p_valid = ICF_FALSE;
                                        break;
                                    }
                                    if (1 < count)
                                    {
                                        ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_INVALID_SERVICE_INTERACTION)
                                            *p_valid = ICF_FALSE;
                                        break;
                                    }
                                }
                                p_list = p_list->p_next;
                            }
			    count = 0;
                        }                        
                    } /* p_ecode))) */
                    else
                    {
                        *p_valid = ICF_FALSE;
                        ret_val = ICF_SUCCESS;
                    } /* else */                    
                } /* else */
                break;

                /* Call Transfer Attended Request Recieved */
                /* For Add party request XCONF will use
                 * ICF_INTERNAL_REFER_W_REP_REQ as
                 * ICF_USER_CALL_TRANSFER_ATTENDED however latter is
                 * restricted when XCONF is active*/
            case ICF_USER_CALL_TRANSFER_ATTENDED:
            case ICF_INTERNAL_REFER_W_REP_REQ:
                /* if TWC/CTA/CTU/XCONF/RNW is active,then reject this service*/
                if (((status & ICF_SERVICE_RNW) 
                        || (status & ICF_SERVICE_CTU) 
                        || (status & ICF_SERVICE_XCONF)
                        || (status & ICF_SERVICE_CTA)) &&
                        (ICF_USER_CALL_TRANSFER_ATTENDED == p_pdb->msg_id))
                    *p_valid = ICF_FALSE;
                /* check the other participant call for 
                 * Service interaction rules */
                else
                {
                    /* get other cta participant 
                     * call_id from external message */
                    if(ICF_USER_CALL_TRANSFER_ATTENDED == p_pdb->msg_id)
                    {   
                        call_id = ((icf_pa_cc_call_transfer_attended_req_st *) 
                            (p_glb_pdb->p_recv_msg->payload))->\
                        transferred_party_id;
                    }
                    else
                    {
                        /* Add party request for XCONF use the
                         * replaces_call_id populated in XCONF as call_id*/
                      call_id = ((icf_internal_refer_req_st*)
                         (p_pdb->p_msg->p_msg_data))->replaces_call_id;
                    }
                    if(call_id == p_glb_pdb->p_call_ctx->call_id)
                    {
                        *p_valid = ICF_FALSE;
                        break;
                    }

                    /* Find call context for the other Call-id */
                    if(ICF_SUCCESS == 
                            (ret_val = 
                             icf_dbm_find_call_cntxt_blk(
                                 p_glb_pdb, 
                                 call_id, 
                                 &p_call_ctx, 
                                 p_ecode)))
                    {
                        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx))

                        /*Check subs and actv for other call id*/
                        ret_val =  icf_dbm_get_line_cntxt_blk(
                                p_glb_pdb,
                                p_call_ctx->line_id,
                                &p_line_data,
                                &dbm_line_status,
                                p_ecode);

                        ICF_ASSERT(p_glb_pdb,(ICF_NULL!=p_line_data))

                        if((ICF_SUCCESS == ret_val))
                        {

                            /* Set is_valid = FALSE and set error code if 
                             * one of the flags is not set */
                            if(ICF_NULL ==
                                (ICF_SERVICE_SUBS_CALL_TRF_ATTENDED & 
                                 p_line_data->service_subscription_flag))
                            {
                                ICF_SCM_SET_ECODE(p_pdb,
                                        ICF_ERROR_SERVICE_NOT_SUBSCRIBED)
                                    
                                *p_valid = ICF_FALSE;
                                serv_actv_or_subs = ICF_FALSE;
                            } /* if(ICF_NULL ==(p_line_data-... */

                            else if(ICF_NULL ==
                                (ICF_SERVICE_ACT_CALL_TRF_ATTENDED & 
                                 p_line_data->service_activation_flag))
                            {
                                ICF_SCM_SET_ECODE(p_pdb,
                                        ICF_ERROR_SERVICE_NOT_ACTIVE)
                                *p_valid = ICF_FALSE;
                                serv_actv_or_subs = ICF_FALSE;
                            } /* && (p_line_data->service_activ... */
                            else if (p_call_ctx->p_service_ctx != ICF_NULL)
                            {
                                status = ((icf_service_ctx_st *)
                                        p_call_ctx->p_service_ctx)->
                                    active_service_list.active_service_status;
                                if ((status & ICF_SERVICE_RNW)
                                        ||(status & ICF_SERVICE_CTU)
                                        ||(status & ICF_SERVICE_CTA))
                                    *p_valid = ICF_FALSE;
                            }
                        } /* if(ICF_SUCCESS == (ret_val ... */

                        if ((ICF_SUCCESS == ret_val) &&
                            (ICF_FALSE != *p_valid))
                        {   
                            p_list = p_call_ctx->p_stream_sdp_info;
                            
                            while (ICF_NULL != p_list)
                            {
                                p_stream_sdp = p_list->p_payload;
                                
                                if (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)
                                {
                                    count++;
                                    
                                    if (ICF_STREAM_TYPE_AUDIO != p_stream_sdp->stream_type)
                                    {
                                        ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_INVALID_SERVICE_INTERACTION)
                                            *p_valid = ICF_FALSE;
                                        break;
                                    }
                                    if (1 < count)
                                    {
                                        ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_INVALID_SERVICE_INTERACTION)
                                            *p_valid = ICF_FALSE;
                                        break;
                                    }
                                }
                                p_list = p_list->p_next;
                            }
				count = 0;
                        }
                    } /* p_ecode))) */
                    else
                    {
                        *p_valid = ICF_FALSE;
                        serv_actv_or_subs = ICF_FALSE;
                        ICF_SCM_SET_ECODE(p_pdb,ICF_CAUSE_INVALID_CALL_ID);
                        ret_val = ICF_SUCCESS;
                    }
                }  /* else */
                break;
            case ICF_USER_INITIATE_EXTERNAL_CONF_REQ:
            case ICF_INTERNAL_REFER_REQ:
                break;
            default :
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_INV_API_RECVD)
                *p_valid = ICF_FALSE;
                break;
        } /* switch(p_pdb->p_msg->msg_hdr.m... */
        /*If service is not subscribed or activated then does not
         * set invalid service interaction ecode*/
        if((ICF_TRUE == serv_actv_or_subs) && (ICF_FALSE == *p_valid))
        {
            ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_INVALID_SERVICE_INTERACTION)
        } /* if(ICF_FALSE = *p_valid) */
    } /* if(ICF_SUCCESS */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_process_return_status
 *
 * DESCRIPTION   : This function checks status when function call to service 
 *                 logic module returns. Based on the status, it does 
 *                 various actions like deallocating the service logic context, 
 *                 setting the request flag, sending internal events 
 *                 to other modules etc.
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t icf_scm_process_return_status
(
 INOUT 	icf_scm_pdb_st	*p_pdb,
 INOUT 	icf_error_t		*p_ecode
)
{
    icf_uint32_t status = p_pdb->p_msg->msg_hdr.status;
    icf_uint8_t service_index = p_pdb->current_service_index;
    icf_uint8_t service_id;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;
    icf_uint8_t  service_bitmask = ((p_pdb->p_scm_data->
                scm_sl_info)[service_index-1]).service_bitmask;
    icf_service_ctx_st *p_second_service_ctx = ICF_NULL;
    icf_return_t temp_ret = ICF_SUCCESS;
    icf_call_ctx_st    *p_sec_call_ctx = ICF_NULL;
    icf_uint32_t second_status = ICF_NULL;
    icf_boolean_t    swap_ctx = ICF_FALSE;
    icf_call_ctx_st  *p_new_call_ctx = ICF_NULL; 
    icf_uint32_t status_2 = p_pdb->p_msg->msg_hdr.status_2;



    ICF_FUNCTION_ENTER(p_glb_pdb)


    /* Get Service module id */
    ICF_SCM_GET_ID_FROM_SERVICE_MODULE_INDEX(service_id,service_index); 

    if(((ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)||
        (ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)) && 
	(ICF_NULL != p_pdb->p_msg->p_msg_data))
    {
        	p_second_service_ctx= ((icf_service_call_data_st *)
                	(p_pdb->p_msg->p_msg_data))->p_other_call_ctx->p_service_ctx;
        
        if(ICF_STATUS_SERVICE_ENDED_ERROR & status)
        {
            /* Change status to Service Ended error with payload 
             * to enable CM to free the payload.
             */
            status &=
                ~ICF_STATUS_SERVICE_ENDED_ERROR;
            status |=
                ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD;
            p_pdb->p_msg->msg_hdr.status = status;
        }
        else
        {
            /* Free the payload rcvd */
            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_pdb->p_msg->p_msg_data, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
        }
    }

    /*Reset signalling_ongoing if status is signal_ended*/
    if (ICF_STATUS_SIGNAL_ONGOING_ENDED & status)
    {
        p_pdb->p_service_ctx->p_call_ctx->signalling_ongoing = 
            ICF_INT_MODULE_INVALID;

        if (((ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)||
                    (ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)) ||
                (ICF_INT_MODULE_TWC == service_id))
        {
            if(ICF_NULL == p_second_service_ctx && ICF_NULL != p_pdb->p_msg->p_msg_data)
            {
                p_second_service_ctx= ((icf_service_call_data_st *)
                (p_pdb->p_msg->p_msg_data))->p_other_call_ctx->p_service_ctx;

                ICF_MEMFREE(
                    p_glb_pdb, 
                    p_pdb->p_msg->p_msg_data, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
            }
           
            if (ICF_NULL != p_second_service_ctx)
            { 
                p_second_service_ctx->p_call_ctx->signalling_ongoing = 
                    ICF_INT_MODULE_INVALID;
            }
        } /* (ICF_STATUS_SERVICE_REQ_FOR... */
    } /* if (ICF_STATUS_SIGNAL_ONGOI... */
    
    /*Reset media_ongoing if status is media_ended*/
    if (ICF_STATUS_MEDIA_ONGOING_ENDED & status)
    {
        p_pdb->p_service_ctx->p_call_ctx->media_ongoing = 
            ICF_INT_MODULE_INVALID;

        if (((ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)||
                    (ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)) ||
                (ICF_INT_MODULE_TWC == service_id))
        {
            if(ICF_NULL == p_second_service_ctx && ICF_NULL != p_pdb->p_msg->p_msg_data)
            {
                p_second_service_ctx= ((icf_service_call_data_st *)
                (p_pdb->p_msg->p_msg_data))->p_other_call_ctx->p_service_ctx;

                ICF_MEMFREE(
                    p_glb_pdb, 
                    p_pdb->p_msg->p_msg_data, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
            }
            if (ICF_NULL != p_second_service_ctx)
            { 
                p_second_service_ctx->p_call_ctx->media_ongoing = 
                    ICF_INT_MODULE_INVALID;
            }

        } /* (ICF_STATUS_SERVICE_REQ_FOR... */
    }   
       /* check to remove klocwork warning */
 
	if (((ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)||
                    (ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)) &&
                (ICF_NULL == p_second_service_ctx))
	{
		return ICF_FAILURE;
	}

    
    /* If call hold is the current service id and resp from chr is also CHR 
     * then this is the case when the second call involved in the conf is also
     * to be put on hold. scm process mesg will be called again with the msg 
     * id as int twc/rnw resp and the call ctx in the glb pdb from the second 
     * service ctx in . Reset resp from chr to invalid. Set resp from twc back 
     * to chr in the second service ctx. In this case p msg data will be null.
     */

    if ((ICF_INT_MODULE_CHR == service_id) && 
        (ICF_NULL != p_pdb->p_second_call_ctx) && 
        (ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR) == 
            ((icf_service_ctx_st *)(p_pdb->p_second_call_ctx->
            p_service_ctx))->expected_events.resp_frm_chr))
    {
        icf_internal_msg_st    *p_new_msg = ICF_NULL;
        icf_service_ctx_st     *p_pdb_serv_ctx = ICF_NULL;

        p_pdb_serv_ctx = (icf_service_ctx_st *)(p_pdb->p_second_call_ctx->
            p_service_ctx);
        
        /* Allocate New Internal Message */
        ICF_MEMGET(p_glb_pdb, 
                sizeof(icf_internal_msg_st), 
                ICF_MEM_COMMON, 
                p_new_msg, 
                ICF_RET_ON_FAILURE, 
                p_ecode, 
                ret_val)

        p_new_msg->msg_hdr.status = ICF_NULL;
        p_new_msg->msg_hdr.p_glb_pdb=p_glb_pdb;

        /* Send int chr resp to chr */
        p_new_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_CHR_RESP;

        /* Save the first ctx in the SCM pdb and send the second ctx
         * in the global pdb
         */
        p_new_call_ctx = p_new_msg->msg_hdr.p_glb_pdb->p_call_ctx;
        p_new_msg->msg_hdr.p_glb_pdb->p_call_ctx = p_pdb_serv_ctx->p_call_ctx;
        p_pdb->p_second_call_ctx = p_new_call_ctx;
        p_new_msg->p_msg_data = ICF_NULL;

        /* Send Message to appropriate module */
        ret_val=icf_scm_process_mesg(p_new_msg);

        /* Save the status returned in the second call to CHR */
        second_status = p_new_msg->msg_hdr.status;

        p_pdb->p_msg->msg_hdr.status |= p_new_msg->msg_hdr.status;
        
        /* Deallocate Memory for Message */
        ICF_MEMFREE(
                p_glb_pdb, 
                p_new_msg, 
                ICF_MEM_COMMON, 
                p_ecode, 
                temp_ret)

        /* The ctxs are swapped again so that the ctx for the first call id is
         * in the ctx 
         */
        p_new_call_ctx = p_glb_pdb->p_call_ctx;
        p_glb_pdb->p_call_ctx = p_pdb->p_second_call_ctx;            
        p_pdb->p_second_call_ctx = p_new_call_ctx;
    }

    /*When CTU/CTA returns REFER_SUCCESS or REFER_FAILURE response
     * then forward internal refer resp to XCONF*/
    if ((ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP & status_2) || 
        (ICF_STATUS_INTERNAL_REFER_FAILURE_RESP & status_2))
    {
	    icf_internal_msg_st *p_new_msg = ICF_NULL;
	    icf_internal_refer_resp_st   refer_resp = {0,0};

        /* Allocate New Internal Message */
        ICF_MEMGET(p_glb_pdb, 
            sizeof(icf_internal_msg_st), 
            ICF_MEM_COMMON, 
            p_new_msg, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)

        p_new_msg->msg_hdr.p_glb_pdb=p_glb_pdb;
        p_new_msg->msg_hdr.status = ICF_NULL;
        p_new_msg->p_msg_data = (icf_uint8_t *)&refer_resp;
	    p_new_msg->msg_hdr.payload_length = 
		    sizeof(icf_internal_refer_resp_st);

	    /*Invoke XCONF service with REFER_RESP from CTU/CTA*/
        p_new_msg->msg_hdr.msg_id = ICF_INTERNAL_REFER_RESP;

	    /*Populate internal_refer_req payload*/
        if (ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP & status_2)
	    {
		    refer_resp.result = ICF_SUCCESS;
	    }
	    else
	    {
		    refer_resp.result = ICF_FAILURE;
		    refer_resp.ecode = p_pdb->p_msg->msg_hdr.ecode;
	    }
		    
        /* Send Message to appropriate module */
        ret_val=icf_scm_process_mesg(p_new_msg);

        p_pdb->p_msg->msg_hdr.status |= p_new_msg->msg_hdr.status;
       
        /* Reset Call Context in Global PDB */
        p_glb_pdb->p_call_ctx = p_pdb->p_service_ctx->p_call_ctx;
        /* Reset Mesg Id */
        p_pdb->p_msg->msg_hdr.msg_id = p_pdb->msg_id;
        /* Deallocate Memory for Message */
        ICF_MEMFREE(
               p_glb_pdb, 
               p_new_msg, 
               ICF_MEM_COMMON, 
               p_ecode, 
               temp_ret)
    }
 
    if(ICF_STATUS_CC_HANDLE_REINVITE & status)
    {
       p_pdb->p_msg->msg_hdr.status = status;
    }

    /* Service Continue */
    if (!((ICF_STATUS_SERVICE_ENDED_NORMAL & status)
            || (ICF_STATUS_SERVICE_ENDED_ERROR & status)
            || (ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD & status)))
    {
        ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_CONT_RECVD)

        /*********************************************************
         * We might need to add Functionality for This status later 
         * if(ICF_STATUS_SERVICE_MSG_TO_CC_SETUP & status)
         * ********************************************/

        /*If CTA service is waiting for TWC service termination, it sets
         *AWT TWC RESP status in return path*/
        if ( ICF_STATUS_SERVICE_AWT_TWC_RESP & status)
        {
            if (ICF_NULL == p_second_service_ctx)
            {
               return ICF_FAILURE; 
            }
            p_second_service_ctx->expected_events.resp_frm_twc =
                service_index;

            p_pdb->p_service_ctx->expected_events.resp_frm_twc =
                service_index;
        } /* if ( ICF_STATUS_SERVICE_AWT... */    
        /* Request for Call Hold/Resume */
        else if ((ICF_STATUS_SERVICE_CALL_HOLD_REQ & status) ||
                (ICF_STATUS_SERVICE_CALL_RESUME_REQ & status))
        {
            icf_internal_msg_st *p_new_msg = ICF_NULL;
            /* Allocate New Internal Message */
            ICF_MEMGET(p_glb_pdb, 
                sizeof(icf_internal_msg_st), 
                ICF_MEM_COMMON, 
                p_new_msg, 
                ICF_RET_ON_FAILURE, 
                p_ecode, 
                ret_val)

            p_new_msg->msg_hdr.p_glb_pdb=p_glb_pdb;

            p_new_msg->msg_hdr.status = ICF_NULL;

            p_new_msg->p_msg_data = ICF_NULL;

            /* Currently we are assuming that Internal Call Hold never 
             * comes with Status Apply for Both Calls
             */

            if (status & ICF_STATUS_SERVICE_CALL_HOLD_REQ)
            {
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_CH_REQ_STATUS)
                p_new_msg->msg_hdr.msg_id =
                    ICF_SC_INTERNAL_CALL_HOLD_REQ;
                
            } /* if (status & ICF_STATUS_SE... */

            else
            {
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_CR_REQ_STATUS)
                    p_new_msg->msg_hdr.msg_id = 
                    ICF_SC_INTERNAL_CALL_RESUME_REQ;
            }

            if(ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                   ICF_MEMFREE(
                        p_glb_pdb, 
                        p_new_msg, 
                        ICF_MEM_COMMON, 
                        p_ecode, 
                        temp_ret)

                   return ICF_FAILURE;                    
                }
                p_glb_pdb->p_call_ctx = p_second_service_ctx->p_call_ctx;
                p_second_service_ctx->expected_events.resp_frm_chr = 
                    service_index;
            }
            else
            {
                p_pdb->p_service_ctx->expected_events.resp_frm_chr = 
                    service_index;
            } /* else */

            /* Send Message to appropriate module */
            ret_val=icf_scm_process_mesg(p_new_msg);
            p_pdb->p_msg->msg_hdr.status |= p_new_msg->msg_hdr.status;
        
            if(ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD & 
                    p_new_msg->msg_hdr.status)
            {
                p_pdb->p_msg->p_msg_data = p_new_msg->p_msg_data;
            }

            /* Reset Call Context in Global PDB */
            p_glb_pdb->p_call_ctx = p_pdb->p_service_ctx->p_call_ctx;
            /* Reset Mesg Id */
            p_pdb->p_msg->msg_hdr.msg_id = p_pdb->msg_id;
            /* Deallocate Memory for Message */
            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_new_msg, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
        }
    	 /*When XCONF return internal REFER request wo REP then invoke
	      * CTU service with internal refer req*/
        else if ((ICF_STATUS_INTERNAL_REFER_REQ & status_2) || 
                 (ICF_STATUS_INTERNAL_REFER_W_REP_REQ & status_2) )

	    {
	       icf_internal_msg_st *p_new_msg = ICF_NULL;
           /* Allocate New Internal Message */
           ICF_MEMGET(p_glb_pdb, 
               sizeof(icf_internal_msg_st), 
               ICF_MEM_COMMON, 
               p_new_msg, 
               ICF_RET_ON_FAILURE, 
               p_ecode, 
               ret_val)

            p_new_msg->msg_hdr.p_glb_pdb=p_glb_pdb;
            p_new_msg->msg_hdr.status = ICF_NULL;
	        
            /*XCONF will fill the msg_data and SCM will only forward
	         * it to corresponding module*/
            p_new_msg->p_msg_data = p_pdb->p_msg->p_msg_data;

	        if (ICF_STATUS_INTERNAL_REFER_REQ & status_2)
	        {
		    /*INvoke CTU service with REFER_REQ*/
		      p_new_msg->msg_hdr.msg_id = ICF_INTERNAL_REFER_REQ;
              p_pdb->p_service_ctx->expected_events.resp_frm_ctu = 
                               service_index;
	        }
	        else
	        {
		    /*INvoke CTA service with REFER_REQ*/
		      p_new_msg->msg_hdr.msg_id = ICF_INTERNAL_REFER_W_REP_REQ;
              p_pdb->p_service_ctx->expected_events.resp_frm_cta = 
                                  service_index;
	        }

            /* Send Message to appropriate module */
            ret_val=icf_scm_process_mesg(p_new_msg);


            p_pdb->p_msg->msg_hdr.status |= p_new_msg->msg_hdr.status;
        
            if(ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD & 
                    p_new_msg->msg_hdr.status)
            {
                p_pdb->p_msg->p_msg_data = p_new_msg->p_msg_data;
            }

            /* Reset Call Context in Global PDB */
            p_glb_pdb->p_call_ctx = p_pdb->p_service_ctx->p_call_ctx;
            /* Reset Mesg Id */
            p_pdb->p_msg->msg_hdr.msg_id = p_pdb->msg_id;
            /* Deallocate Memory for Message */
            /*The memfree for p_msg_data is done here because the same */
            /*is allocated in XCONF but needs to be forwarded towards CTA */
            /* by SCM, now we have returned from CTA after call to */
            /* SCM process mesg we need to free the msg_data */
            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_pdb->p_msg->p_msg_data, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)

            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_new_msg, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
     	}
        else if ((ICF_INT_MODULE_CHR == service_id) 
                && (ICF_NULL != 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr)
                && (ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR) != 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr))
        {
            icf_internal_msg_st *p_new_msg = ICF_NULL;
            /* Allocate New Internal Message */
            ICF_MEMGET(p_glb_pdb, 
                sizeof(icf_internal_msg_st), 
                ICF_MEM_COMMON, 
                p_new_msg, 
                ICF_RET_ON_FAILURE, 
                p_ecode, 
                ret_val)

            p_new_msg->msg_hdr.p_glb_pdb=p_glb_pdb;

            p_new_msg->msg_hdr.status = ICF_NULL;

            p_new_msg->p_msg_data = ICF_NULL;

            if(status & ICF_STATUS_SERVICE_CALL_HOLD_RESP) 
            {
               ICF_SCM_TRACE(p_pdb,ICF_TRACE_CH_SUCCESS_STATUS)
               p_new_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_CALL_HOLD_RESP;
            }
            else if (status & ICF_STATUS_SERVICE_CALL_RESUME_RESP)
            {
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_CR_SUCCESS_STATUS)
                p_new_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_CALL_RESUME_RESP;
            }
            else
            {
                p_new_msg->msg_hdr.msg_id = ICF_NULL;
            }

            if (ICF_NULL != p_new_msg->msg_hdr.msg_id)
            {
                /*
                *p_pdb->p_service_ctx->active_service_list.active_service_status
                    &= ~service_bitmask;*/
                
                /* Call the appropraite service */
                ret_val=icf_scm_process_mesg(p_new_msg);
                p_pdb->p_msg->msg_hdr.status |= p_new_msg->msg_hdr.status;

                /* Reset Mesg Id */
                p_pdb->p_msg->msg_hdr.msg_id = p_pdb->msg_id;
            }
            /* Deallocate Memory for Message */
            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_new_msg, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
        }

        /* Start Proc Signalling Request */
        if (ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ & status)
        {
            ICF_SCM_TRACE(p_pdb,ICF_TRACE_START_PROC_SIG_STATUS)
            
            if(ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                   return ICF_FAILURE;
                }
                p_second_service_ctx->expected_events.start_proc_signaling = 
                service_index;
            }
            else if(ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                   return ICF_FAILURE;
                }
                p_second_service_ctx->expected_events.start_proc_signaling = 
                service_index;
                p_pdb->p_service_ctx->expected_events.start_proc_signaling = 
                service_index;
            } /* if(ICF_STATUS_SERVICE_APPLY... */
            else
            {
                p_pdb->p_service_ctx->expected_events.start_proc_signaling = 
                service_index;
            } /* else */
        }

        /* Start Proc Media Request */
        if (ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ & status)
        {
            ICF_SCM_TRACE(p_pdb,ICF_TRACE_START_PROC_MEDIA_STATUS)
            if(ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                    return ICF_FAILURE;
                }
                p_second_service_ctx->expected_events.start_proc_media = 
                service_index;
            }
            else if(ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                    return ICF_FAILURE;
                }
                p_second_service_ctx->expected_events.start_proc_media = 
                service_index;
                p_pdb->p_service_ctx->expected_events.start_proc_media = 
                service_index;
            } /* if(ICF_STATUS_SERVICE_APPLY... */
            else
            {
                p_pdb->p_service_ctx->expected_events.start_proc_media = 
                service_index;
            } /* else */
        }

        /* Signalling Events Request */
        if (ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ & status)
        {
            ICF_SCM_TRACE(p_pdb,ICF_TRACE_SIG_EVENTS_REQ_STATUS)
            if(ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                   return ICF_FAILURE;
                }
                p_second_service_ctx->p_call_ctx->signalling_ongoing = 
                    service_id; 
            }
            else if(ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                   return ICF_FAILURE;
                } 
            
                p_second_service_ctx->p_call_ctx->signalling_ongoing = 
                    service_id; 
                p_pdb->p_service_ctx->p_call_ctx->signalling_ongoing = 
                    service_id;
            } /* if(ICF_STATUS_SERVICE_APPLY... */
            else
            {
                p_pdb->p_service_ctx->p_call_ctx->signalling_ongoing = 
                    service_id;
            } /* else */
        }

        /* Media Events Request */
        if (ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ & status)
        {
            ICF_SCM_TRACE(p_pdb,ICF_TRACE_MEDIA_EVENTS_REQ_STATUS)
            if(ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                   return ICF_FAILURE; 
                }
                p_second_service_ctx->p_call_ctx->media_ongoing = 
                    service_id; 
            }
            else if(ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)
            {
                if (ICF_NULL == p_second_service_ctx)
                {
                   return ICF_FAILURE; 
                }
                p_second_service_ctx->p_call_ctx->media_ongoing = 
                    service_id;
                p_pdb->p_service_ctx->p_call_ctx->media_ongoing = 
                    service_id;
            } /* if(ICF_STATUS_SERVICE_APPLY... */
            else
            {
                p_pdb->p_service_ctx->p_call_ctx->media_ongoing = 
                    service_id;
            } /* else */
        }

        if (ICF_STATUS_SERVICE_ENDED_ERROR & second_status)
        {
            /* Return service ended error to CM for second call
             * The call ctx pointer in the glb pdb is already replaced 
             * by the second call id
             */
            p_pdb->p_msg->msg_hdr.status = second_status;
            
            /* The ctxs are swapped again so that the ctx for the second call 
             * id is in the ctx 
             */
            p_new_call_ctx = p_glb_pdb->p_call_ctx;
            p_glb_pdb->p_call_ctx = p_pdb->p_second_call_ctx;            
            p_pdb->p_second_call_ctx = p_new_call_ctx;
        }
        if (ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE & status_2)
        {
           /* This is done because when XCONF SLP got Release conf
              request from the PA the state is changed to 
              ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP so service 
              should not be ended but we need to trigger the call ending
              to CC therefore set the status to SERVICE_ENDED_ERROR. Also
              the SLP ctx and srv ctx will be deallocated by event
              CC_CALL_RELEASED coming from CC to XCONF */
           status |= ICF_STATUS_SERVICE_ENDED_ERROR;
           /*p_pdb->msg_id = ICF_CC_CALL_RELEASED;*/
           p_pdb->p_msg->msg_hdr.status = ICF_STATUS_SERVICE_ENDED_ERROR;
        }    
    } 
    /* Service Ended */
    else
    {
        if (ICF_NULL != p_second_service_ctx)
        {
            p_second_service_ctx->p_call_ctx->signalling_ongoing = 
                ICF_INT_MODULE_INVALID; 
            p_second_service_ctx->p_call_ctx->media_ongoing = 
                ICF_INT_MODULE_INVALID;

            if ((ICF_NULL != 
                    p_second_service_ctx->expected_events.resp_frm_chr) &&
                (p_second_service_ctx->expected_events.resp_frm_chr == 
                 service_index))
            {
                p_second_service_ctx->expected_events.resp_frm_chr = 
                    (icf_uint8_t)( p_second_service_ctx->expected_events.resp_frm_chr - service_index) ;
            }
        }
        p_pdb->p_service_ctx->p_call_ctx->signalling_ongoing = 
            ICF_INT_MODULE_INVALID;
        p_pdb->p_service_ctx->p_call_ctx->media_ongoing = 
            ICF_INT_MODULE_INVALID;

        ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_END_RECVD)
        /* If CHR response needs to be sent to another service 
         */ 
            
        /* Added check for rnw to invoke chr in the return status from RNW
         */ 

        /* Check that if resp from chr is not null then it should not be
         * chr also. As this scenario is already handled above
         */

        if ((ICF_NULL != 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr) &&
                (p_pdb->p_service_ctx->expected_events.resp_frm_chr == 
                 service_index))
        {
            p_pdb->p_service_ctx->expected_events.resp_frm_chr = 
                (icf_uint8_t)(p_pdb->p_service_ctx->expected_events.resp_frm_chr - service_index);
        }
                     
        if ((ICF_CC_CALL_RELEASED != p_pdb->msg_id) && 
                (((ICF_INT_MODULE_CHR == service_id) 
                && (ICF_NULL != 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr)
                && (ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR) != 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr))
                || ((ICF_INT_MODULE_TWC == service_id) 
                && (ICF_NULL != 
                    p_pdb->p_service_ctx->expected_events.resp_frm_twc))
                || ((ICF_INT_MODULE_RNW == service_id) 
                && (ICF_NULL != 
                    p_pdb->p_service_ctx->expected_events.resp_frm_rnw))))
        {
            icf_internal_msg_st *p_new_msg = ICF_NULL;
            /* Allocate New Internal Message */
            ICF_MEMGET(p_glb_pdb, 
                    sizeof(icf_internal_msg_st), 
                    ICF_MEM_COMMON, 
                    p_new_msg, 
                    ICF_RET_ON_FAILURE, 
                    p_ecode, 
                    ret_val)

            p_new_msg->msg_hdr.status = ICF_NULL;

            p_new_msg->msg_hdr.p_glb_pdb=p_glb_pdb;

            p_new_msg->p_msg_data = ICF_NULL;

            /* Service Ended Normal */
            if (status & ICF_STATUS_SERVICE_ENDED_NORMAL)
            {

                p_pdb->p_service_ctx->active_service_list.
                    active_service_status &= ~service_bitmask;

                ret_val = icf_dbm_dealloc_srvc_cntxt_lgc_blk(
                        p_glb_pdb, 
                        p_pdb->p_service_ctx->active_service_list.
                        p_sl_ctx[service_index - 1], p_ecode);

                p_pdb->p_service_ctx->active_service_list.
                        p_sl_ctx[service_index - 1] = ICF_NULL;

                if (ICF_FAILURE == ret_val)
                {
                    ret_val = ICF_FAILURE;
                }
                else if(status & ICF_STATUS_SERVICE_CALL_HOLD_RESP)
                { 
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_CH_SUCCESS_STATUS)
                        p_new_msg->msg_hdr.msg_id = 
                        ICF_SC_INTERNAL_CALL_HOLD_RESP;
                }
                else if (status & ICF_STATUS_SERVICE_CALL_RESUME_RESP)
                {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_CR_SUCCESS_STATUS)
                        p_new_msg->msg_hdr.msg_id = 
                        ICF_SC_INTERNAL_CALL_RESUME_RESP;
                }
                else if (ICF_INT_MODULE_TWC == service_id)
                {
                    p_new_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_TWC_RESP;

                    /* Invoke SCM to send the msg to the appropriate module.
                     * If resp from twc is CHR in the second ctx also then 
                     * the second ctx is send as the payload in the msg else
                     * the payload is null
                     */

                    /* Klockwork warning removal.*/
                    if (ICF_NULL != p_second_service_ctx)
                    {
                        if (ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR)
                             == p_second_service_ctx->expected_events.resp_frm_twc)
                        {
                           p_second_service_ctx->expected_events.resp_frm_chr = 
                              ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR);
                           p_new_msg->p_msg_data = 
                              (icf_uint8_t *)p_second_service_ctx->p_call_ctx;
                        }
                        else
                        {
                           p_new_msg->p_msg_data = ICF_NULL;    
                        }
                        p_second_service_ctx->expected_events.resp_frm_twc=
                             ICF_NULL;
                        p_second_service_ctx->active_service_list.
                             active_service_status &= ~service_bitmask;
                        p_second_service_ctx->active_service_list.
                        p_sl_ctx[service_index - 1] = ICF_NULL;
                   }
                   else
                   {
                        p_new_msg->p_msg_data = ICF_NULL;
                   }
                }
                else if (ICF_INT_MODULE_RNW == service_id)
                {
                    p_new_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_RNW_RESP;

                    /* Invoke SCM to send the msg to the appropriate module.
                     * If resp from rnw is CHR in the second ctx also then 
                     * the second ctx is send as the payload in the msg else
                     * the payload is null
                     */
                    
                    /* Klockwork warning removal.*/
                    if (ICF_NULL != p_second_service_ctx)
                    {  
                       if (ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR) == 
                              p_second_service_ctx->expected_events.resp_frm_rnw)
                       {
                           p_second_service_ctx->expected_events.resp_frm_chr = 
                              ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR);
                        
                           p_new_msg->p_msg_data = 
                             (icf_uint8_t *)p_second_service_ctx->p_call_ctx;
                       }
                       else
                       {
                           p_new_msg->p_msg_data = ICF_NULL;    
                       }
                       p_second_service_ctx->expected_events.resp_frm_rnw=
                          ICF_NULL;

                       p_second_service_ctx->active_service_list.
                          active_service_status &= ~service_bitmask;
                       p_second_service_ctx->active_service_list.
                       p_sl_ctx[service_index - 1] = ICF_NULL;
                    }
                    else
                    {
                       p_new_msg->p_msg_data = ICF_NULL;
                    }
                }
            } /* if (status & ICF_STATUS_SE... */

            /* Service Ended Error */
            else
            {

                ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_ERR_IND_STATUS)
                    
                p_new_msg->msg_hdr.msg_id = ICF_SC_SERVICE_ERROR_IND;

                p_pdb->p_service_ctx->expected_events.service_ind_handler = 
                    ICF_NULL;

                if (ICF_NULL != p_pdb->p_service_ctx->expected_events.
                        resp_frm_chr)
                {
                    p_pdb->p_service_ctx->expected_events.service_ind_handler =
                        p_pdb->p_service_ctx->expected_events.resp_frm_chr;
                }
            } /* else */

            if (ICF_FAILURE == ret_val)
            {
                ret_val = ICF_FAILURE;
            }
            else if ((status & ICF_STATUS_SERVICE_ENDED_NORMAL) ||
                    ((status & ICF_STATUS_SERVICE_ENDED_ERROR) &&
                    (ICF_NULL != p_pdb->p_service_ctx->expected_events.
                     service_ind_handler)))
            {
                /* Send Message to appropriate module */
                ret_val=icf_scm_process_mesg(p_new_msg);

                p_pdb->p_msg->msg_hdr.status |= p_new_msg->msg_hdr.status;

                if(ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD & 
                    p_new_msg->msg_hdr.status)
                {
                    p_pdb->p_msg->p_msg_data = p_new_msg->p_msg_data;
                }

                if (ICF_NULL != p_second_service_ctx)
                {
                    /* Reset Call Context in Global PDB */
                    if (p_glb_pdb->p_call_ctx->call_id == 
                            p_second_service_ctx->p_call_ctx->call_id)
                    {
                        p_second_service_ctx = p_pdb->p_service_ctx;
                        p_second_service_ctx->p_call_ctx = 
                            p_pdb->p_service_ctx->p_call_ctx;
                    }
                }
                /* Reset Mesg Id */
                p_pdb->p_msg->msg_hdr.msg_id = p_pdb->msg_id;
            }
            /* Deallocate Memory for Message */
            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_new_msg, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
                
            if (ICF_FAILURE == ret_val)
            {
                ret_val = ICF_FAILURE;
            }
            else if ((ICF_NULL != p_second_service_ctx) &&
                (((ICF_SERVICE_TWC == p_second_service_ctx->
                    active_service_list.active_service_status) ||
                (ICF_SERVICE_RNW == p_second_service_ctx->
                    active_service_list.active_service_status)) ||
                (ICF_NULL == p_second_service_ctx->
                    active_service_list.active_service_status)))
            {
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_ALL_SVC_ENDED)

                p_sec_call_ctx = p_second_service_ctx->p_call_ctx;

                icf_dbm_dealloc_srvc_cntxt_blk(
                            p_glb_pdb,p_second_service_ctx,
                            p_ecode);

                p_second_service_ctx = ICF_NULL;

                p_sec_call_ctx->p_service_ctx = ICF_NULL;
            } /* if (p_pdb->p_service_ctx == IP... */                 
        } /* != ICF_NULL)) */

        /* This is the case when resp from twc/rnw is not null in the second
         * service ctx but it is null in the curr ctx
         */
        else if ((ICF_NULL != p_second_service_ctx) && 
                (status & ICF_STATUS_SERVICE_ENDED_NORMAL) &&
                (((ICF_INT_MODULE_TWC == service_id) &&
                (ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR) == 
                    p_second_service_ctx->expected_events.resp_frm_twc)) 
                || ((ICF_INT_MODULE_RNW == service_id) &&
               (ICF_SCM_RET_SERVICE_INDEX(ICF_INT_MODULE_CHR) == 
                    p_second_service_ctx->expected_events.resp_frm_rnw))))
        {
            icf_internal_msg_st *p_new_msg = ICF_NULL;
            /* Allocate New Internal Message */
            ICF_MEMGET(p_glb_pdb, 
                    sizeof(icf_internal_msg_st), 
                    ICF_MEM_COMMON, 
                    p_new_msg, 
                    ICF_RET_ON_FAILURE, 
                    p_ecode, 
                    ret_val)

            p_new_msg->msg_hdr.status = ICF_NULL;
            p_new_msg->msg_hdr.p_glb_pdb=p_glb_pdb;
            p_new_msg->p_msg_data = ICF_NULL;    

            ((icf_service_ctx_st *)
             (p_glb_pdb->p_call_ctx->p_service_ctx))->active_service_list.
                active_service_status &= ~service_bitmask;
            
            p_second_service_ctx->active_service_list.
                active_service_status &= ~service_bitmask;
            
            /* Put the second ctx in the glb pdb and the first ctx can be 
             * saved in the field second call ctx in the pdb
             */
            p_new_call_ctx = p_glb_pdb->p_call_ctx;
            p_glb_pdb->p_call_ctx = p_second_service_ctx->p_call_ctx; 
            p_pdb->p_second_call_ctx = p_new_call_ctx;
            p_second_service_ctx = 
                (icf_service_ctx_st *)p_new_call_ctx->p_service_ctx;

            swap_ctx = ICF_TRUE;

            if (ICF_INT_MODULE_TWC == service_id)
            {
                p_new_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_TWC_RESP;      
            }
            else
            {
                p_new_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_RNW_RESP;   
            }
            /* Send Message to appropriate module */
            ret_val=icf_scm_process_mesg(p_new_msg);
            p_pdb->p_msg->msg_hdr.status |= p_new_msg->msg_hdr.status;

            if(ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD & 
                    p_new_msg->msg_hdr.status)
            {
                p_pdb->p_msg->p_msg_data = p_new_msg->p_msg_data;
            }

            /* Reset Mesg Id */
            p_pdb->p_msg->msg_hdr.msg_id = p_pdb->msg_id;
            /* Deallocate Memory for Message */
            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_new_msg, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)

        }
        else if((ICF_NULL != p_second_service_ctx) &&
                ((ICF_STATUS_SERVICE_APPLY_BOTH_CALLS & status)||
                (ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL & status)))
        {
            /* Free SL context for Other Call */
             p_second_service_ctx->active_service_list.\
                 p_sl_ctx[service_index - 1] = ICF_NULL;
             
             p_second_service_ctx->active_service_list.\
                 active_service_status &=~service_bitmask;

             p_second_service_ctx->p_call_ctx->signalling_ongoing = 
                 ICF_INT_MODULE_INVALID;

             p_second_service_ctx->p_call_ctx->media_ongoing = 
                 ICF_INT_MODULE_INVALID;

            /* If there are no active services, De-Allocate Service Context */
            if (ICF_NULL == 
                    p_second_service_ctx->active_service_list.\
                    active_service_status)
            {
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_ALL_SVC_ENDED)

                p_sec_call_ctx = p_second_service_ctx->p_call_ctx;
                
                icf_dbm_dealloc_srvc_cntxt_blk(
                            p_glb_pdb,p_second_service_ctx,
                            p_ecode);

                p_second_service_ctx = ICF_NULL;

                p_sec_call_ctx->p_service_ctx = ICF_NULL;
            } /* if (p_pdb->p_service_ctx == IP... */
        } /* if(ICF_NULL != p_pdb->secon... */
        /*If service is TWC or CTA or RNW and both or other calls is not set
         * then clear other call context*/
        else if ((ICF_INT_MODULE_TWC == service_id) ||
                (ICF_INT_MODULE_CTA == service_id)) 
        {
             p_second_service_ctx= ((icf_service_call_data_st *)
                (p_pdb->p_msg->p_msg_data))->p_other_call_ctx->p_service_ctx;

             /* Free SL context for Other Call */
             p_second_service_ctx->active_service_list.\
                 p_sl_ctx[service_index - 1] = ICF_NULL;
             
             p_second_service_ctx->active_service_list.\
                 active_service_status &=~service_bitmask;

            /* If there are no active services, De-Allocate Service Context */
            if (ICF_NULL == 
                    p_second_service_ctx->active_service_list.\
                    active_service_status)
            {
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_ALL_SVC_ENDED)

                p_sec_call_ctx = p_second_service_ctx->p_call_ctx;
                
                icf_dbm_dealloc_srvc_cntxt_blk(
                            p_glb_pdb,p_second_service_ctx,
                            p_ecode);

                p_second_service_ctx = ICF_NULL;

                p_sec_call_ctx->p_service_ctx = ICF_NULL;
            } /* if (p_pdb->p_service_ctx == IP... */
            else
            {
                /*If signalling_ongoing is true then reset it*/
                if (service_id == 
                        p_second_service_ctx->p_call_ctx->signalling_ongoing)
                {
                    p_second_service_ctx->p_call_ctx->signalling_ongoing = 
                        ICF_INT_MODULE_INVALID;
                } /* p_second_service_ctx->p_call_c... */

                /*If media_ongoing is true then reset it*/
                if (service_id == 
                        p_second_service_ctx->p_call_ctx->media_ongoing)
                {
                    p_second_service_ctx->p_call_ctx->media_ongoing = 
                        ICF_INT_MODULE_INVALID;
                } /* p_second_service_ctx->p_call_c... */
            } /* else */            

            /* Free the payload rcvd */
            ICF_MEMFREE(
                    p_glb_pdb, 
                    p_pdb->p_msg->p_msg_data, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)
        }
        /*If status of second CHR invocation is service ended error
         * and current return status is also service ended error
         * then return service_ended_error with payload*/
        else if ((ICF_STATUS_SERVICE_ENDED_ERROR & status) &&
                (ICF_STATUS_SERVICE_ENDED_ERROR & second_status))
        {
            status &=
                ~ICF_STATUS_SERVICE_ENDED_ERROR;
            status |=
                ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD;
            p_pdb->p_msg->msg_hdr.status = status;

            p_pdb->p_msg->p_msg_data = (icf_uint8_t *)
                (p_pdb->p_second_call_ctx);
        }
        else if ((ICF_STATUS_SERVICE_ENDED_NORMAL & status) &&
                (ICF_STATUS_SERVICE_ENDED_ERROR & second_status))
        {
              status = ICF_STATUS_SERVICE_ENDED_ERROR;
              p_pdb->p_msg->msg_hdr.status = status;
              
              /* The flag to indicate that the ctxs are to be swapped again 
               * is set. This is set as we have to return the ctx of the second
               * call id now but it can not be set here as the function to
               * free the service logic blocks always checks the ctx from the
               * glb pdb
               */
              swap_ctx = ICF_TRUE;
        }

        if ((ICF_USER_CALL_HOLD == p_pdb->msg_id) || 
                (ICF_USER_CALL_RESUME == p_pdb->msg_id))
        {
            p_pdb->p_service_ctx->expected_events.resp_frm_twc = ICF_NULL;
            p_pdb->p_service_ctx->expected_events.resp_frm_rnw = ICF_NULL;
        }

        /* if the flag to swap is set then the ctxs are swapped to return the
         * correct ctx in the glb pdb
         */
        if (ICF_TRUE == swap_ctx)
        {
            p_new_call_ctx = p_glb_pdb->p_call_ctx;
            p_glb_pdb->p_call_ctx = p_pdb->p_second_call_ctx;            
            p_pdb->p_second_call_ctx = p_new_call_ctx;
        }

        if ((ICF_FAILURE != ret_val) && 
                (ICF_NULL != p_glb_pdb->p_call_ctx->p_service_ctx))
        {
            if (ICF_NULL != ((icf_service_ctx_st *)p_glb_pdb->p_call_ctx->
                            p_service_ctx)->active_service_list.
                            p_sl_ctx[service_index - 1])
            {
                ret_val = icf_dbm_dealloc_srvc_cntxt_lgc_blk(
                        p_glb_pdb,
                        ((icf_service_ctx_st *)p_glb_pdb->p_call_ctx->
                         p_service_ctx)->active_service_list.
                        p_sl_ctx[service_index - 1],
                        p_ecode);
                if( ICF_SUCCESS == ret_val)
                { 
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_SL_CTX_FREED)
                        /* Unset service active bit in service context */
                        ((icf_service_ctx_st *)p_glb_pdb->p_call_ctx->
                         p_service_ctx)
                        ->active_service_list.active_service_status &=
                        ~service_bitmask;

                    ((icf_service_ctx_st *)
                     p_glb_pdb->p_call_ctx->p_service_ctx)
                        ->active_service_list.
                        p_sl_ctx[service_index - 1] = ICF_NULL;

                    if (ICF_NULL != p_second_service_ctx)
                    {
                        p_second_service_ctx->active_service_list.
                            active_service_status &= ~service_bitmask;
                    }   
                    /* If there are no active services 
                     * then De-Allocate Service Context */
                    if((ICF_NULL != p_pdb->p_service_ctx)&&
                            (ICF_NULL == 
                             ((icf_service_ctx_st *)p_glb_pdb->p_call_ctx->
                              p_service_ctx)->active_service_list.\
                             active_service_status))
                    {
                        ICF_SCM_TRACE(p_pdb,ICF_TRACE_ALL_SVC_ENDED)
                            icf_dbm_dealloc_srvc_cntxt_blk(
                                    p_glb_pdb,
                                    (icf_service_ctx_st *)(p_glb_pdb->
                                                              p_call_ctx->p_service_ctx),
                                    p_ecode);
                        p_pdb->p_service_ctx = ICF_NULL;

                        p_glb_pdb->p_call_ctx->p_service_ctx = ICF_NULL;

                    } /* if (p_pdb->p_service_ctx == IP... */
                } /* p_ecode))) */
            }
        }        

        
    } /* || (ICF_STATUS_SERVICE_ENDE... */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_get_handler_module
 *
 * DESCRIPTION   : This function determines the handler Service Logic Module 
 *                 responsible for the handling of the API received. If the 
 *                 handler can't be determined a value of 
 *                 ICF_INT_MODULE_INVALID is returned
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t icf_scm_get_handler_module(
        IN          icf_scm_pdb_st   *p_pdb,
        INOUT		icf_service_id_t *p_handler_module,
        INOUT		icf_error_t      *p_ecode
        )
{
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;
    icf_return_t ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_glb_pdb)
    p_ecode=p_ecode;
    /* switch api id */
    switch(p_pdb->msg_id)
    {
        case ICF_SC_INTERNAL_CALL_HOLD_REQ:  
            *p_handler_module = ICF_INT_MODULE_CHR; 
            break;
        case ICF_USER_CALL_HOLD:	
            *p_handler_module = ICF_INT_MODULE_CHR;
            break;
        case ICF_USER_CALL_RESUME:	
            *p_handler_module = ICF_INT_MODULE_CHR;
            break;
        case ICF_SC_INTERNAL_CALL_RESUME_REQ:
            *p_handler_module = ICF_INT_MODULE_CHR;
            break;
        case ICF_USER_CALL_TRANSFER_UNATTENDED:
            *p_handler_module = ICF_INT_MODULE_CTU;
            break; 
        case ICF_MODIFY_MEDIA_RESP:
            *p_handler_module = ICF_INT_MODULE_CHR;
            break; 
        case ICF_NW_INC_MEDIA_UPDATE_RESP:
            *p_handler_module = ICF_INT_MODULE_CHR;
            break; 
        case ICF_NW_INC_MEDIA_UPDATE:  
            *p_handler_module = ICF_INT_MODULE_CHR;
            break; 
        case ICF_NW_INC_CALL_REJECT:
            *p_handler_module = ICF_INT_MODULE_CHR;
            break; 
        case ICF_NW_INC_CALL_ABORT:
            *p_handler_module = ICF_INT_MODULE_CHR;
            break; 
        case ICF_NW_OTG_CALL_REJECT:
            *p_handler_module = ICF_INT_MODULE_CHR;
            break; 
        /*case ICF_CREATE_CONF_RESP:
            *p_handler_module = ICF_INT_MODULE_TWC;
            break; */
        case ICF_NW_INC_NOTIFY_RESP:
            *p_handler_module = ICF_INT_MODULE_RNW;
            break; 
        /*case ICF_SC_INTERNAL_MEDIA_COMMIT:
            *p_handler_module = ICF_INT_MODULE_TWC;
            break; */
        case ICF_USER_CALL_CONF:
        case ICF_INVOKE_TWC_FOR_JOIN:
            *p_handler_module = ICF_INT_MODULE_TWC;
            break;
        case ICF_USER_CALL_TRANSFER_ATTENDED:
            *p_handler_module = ICF_INT_MODULE_CTA;
            break; 
        case ICF_NW_INC_REFER:
            *p_handler_module = ICF_INT_MODULE_RNW;
            break;
        case ICF_NEW_SERVICE_TRIGGER:
            *p_handler_module = ICF_INT_MODULE_CFW;
            break;
        case ICF_TIMER_EXPIRY_IND:
            *p_handler_module = p_glb_pdb->p_recv_msg->hdr.destination_id;
            break;
        case ICF_USER_CALL_TRANSFER_INITIATED_CFM:
            *p_handler_module = ICF_INT_MODULE_RNW;
            break;
        case ICF_USER_INITIATE_EXTERNAL_CONF_REQ:
        case ICF_USER_ADD_PARTY_REQ:
        case ICF_USER_DELETE_PARTY_REQ:
        case ICF_USER_RELEASE_CONFERENCE_REQ:
        case ICF_EXTERNAL_CONFERENCE_CONNECTED:
        case ICF_INCOMING_EXTERNAL_CONFERENCE_REQ:
        case ICF_INTERNAL_NOTIFY_IND:
        case ICF_INTERNAL_SUBSCRIBE_RESP:
        case ICF_INTERNAL_REFER_RESP:    
            *p_handler_module = ICF_INT_MODULE_XCONF;
            break;
        case ICF_INTERNAL_REFER_REQ:
            *p_handler_module = ICF_INT_MODULE_CTU;
            break;
        case ICF_INTERNAL_REFER_W_REP_REQ:
            *p_handler_module = ICF_INT_MODULE_CTA;
            break;
        default:			
            *p_handler_module = ICF_INT_MODULE_INVALID;
            break;

    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_send_service_failure_resp
 *
 * DESCRIPTION   : This function sends a failure response for the 
 *                 service request recieved.
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/

icf_return_t icf_scm_send_service_failure_resp(

        INOUT	icf_scm_pdb_st   *p_pdb,
        INOUT	icf_error_t      *p_ecode
        )
{
    icf_uint32_t     api_len = 0 ;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      *p_msg = ICF_NULL;
    icf_boolean_t    send_to_pa = ICF_TRUE;
    icf_error_t      ecode = ICF_ERROR_NONE;
    icf_api_id_t     pa_api_id = 0;
    icf_call_id_t    call_id;
#ifdef ICF_STAT_ENABLED
    icf_stats_id_t    stat_id;
#endif

    ICF_FUNCTION_ENTER(p_glb_pdb)
    if(ICF_USER_INITIATE_EXTERNAL_CONF_REQ != p_pdb->msg_id)
    {   
       call_id = p_glb_pdb->p_call_ctx->call_id;

       /* To remove insure marning */
       call_id = call_id;
    }  

    /* switch api id */
    switch(p_pdb->msg_id)
    {
       /* Send Failure for Call Hold Req */
        case ICF_USER_CALL_HOLD:
            {
                pa_api_id = ICF_CC_PA_CALL_HOLD_RESP;
#ifdef ICF_STAT_ENABLED
                stat_id = ICF_STAT_CC_PA_CALL_HOLD_RESP;
#endif
                ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CALL_HOLD_REQ)
                ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CALL_HOLD)
                ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CALL_HOLD_RESP)

                api_len = sizeof(icf_api_header_st) + sizeof(icf_api_resp_st);

                break;
            }

       /* Send Failure for Call Resume Req */
        case ICF_USER_CALL_RESUME:
                {
                pa_api_id = ICF_CC_PA_CALL_RESUME_RESP;
#ifdef ICF_STAT_ENABLED
                stat_id = ICF_STAT_CC_PA_CALL_RESUME_RESP;
#endif
                ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CALL_RESUME_REQ)
                ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CALL_RESUME)
                ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CALL_RESUME_RESP)
                api_len = sizeof(icf_api_header_st) + sizeof(icf_api_resp_st);
                break;
                }
        case ICF_USER_INITIATE_EXTERNAL_CONF_REQ:
             {
                pa_api_id = ICF_CC_PA_CREATE_XCONF_RESP; 
                api_len = sizeof(icf_api_header_st) + 
                    sizeof(icf_create_ext_xconf_resp_st);
                break;
             }
        case ICF_USER_ADD_PARTY_REQ:     
        case ICF_INTERNAL_REFER_W_REP_REQ:
             {
                 /*This block of code is added for handling scenario when
                   add party request comes for a fax call. We have to reject
                   the request and send failure add_party_resp */
                pa_api_id = ICF_CC_PA_ADD_XCONF_PARTY_RESP; 
                api_len = sizeof(icf_api_header_st) + 
                    sizeof(icf_add_xconf_party_resp_st);
                break;
             }
        case ICF_USER_DELETE_PARTY_REQ:     
        case ICF_INTERNAL_REFER_REQ:
             {
                pa_api_id = ICF_CC_PA_DELETE_XCONF_PARTY_RESP; 
                api_len = sizeof(icf_api_header_st) +
                    sizeof(icf_delete_xconf_party_resp_st);
                break;
             }


       /* Send Failure for Call Conference Req */
        case ICF_USER_CALL_CONF:
                {
                pa_api_id = ICF_CC_PA_CONFERENCE_RESP;
#ifdef ICF_STAT_ENABLED
                stat_id = ICF_STAT_CC_PA_CONF_RESP;
#endif
                ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CONF_REQ)
                ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CONF)
                ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CONF_RESP)
                api_len = sizeof(icf_api_header_st) + sizeof(icf_msg_resp_st);
                break;
                }
       /* Send Failure for CTA Req */
        case ICF_USER_CALL_TRANSFER_ATTENDED:
                {
                pa_api_id = ICF_CC_PA_CALL_TRANSFER_ATTENDED_RESP;
#ifdef ICF_STAT_ENABLED
                stat_id = ICF_STAT_CC_PA_CTA_RESP;
#endif
                ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CTA_REQ)
                ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTA)
                ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTA_RESP)
                api_len = sizeof(icf_api_header_st) + sizeof(icf_cta_resp_st);
                break;
                }
       /* Send Failure for CTU Req */
        case ICF_USER_CALL_TRANSFER_UNATTENDED:
                {
                pa_api_id = ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP;
#ifdef ICF_STAT_ENABLED
                stat_id = ICF_STAT_CC_PA_CTU_RESP;
#endif
                ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CTU_REQ)
                ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)
                ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)
                api_len = sizeof(icf_api_header_st) + sizeof(icf_ctu_resp_st);
                break;
                }
         
                /***********************************************************
                 * Send Failure for Internal Call Hold/Resume Req 
                 * This case will never arise for now as we never send failure
                 * for internal call hold/resume requests.
                 * The code below should be uncommented if such scenario
                 * arises in future
                 ***************************************/
#if 0                
        case ICF_SC_INTERNAL_CALL_HOLD_REQ: 
        case ICF_SC_INTERNAL_CALL_RESUME_REQ: 
                {
                icf_uint8_t handler_module_index;
                /* Make Service Error Indication Message */
                send_to_pa = ICF_FALSE;
                p_pdb->p_msg->msg_hdr.msg_id = ICF_SC_SERVICE_ERROR_IND;
                handler_module_index = 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr;
                ICF_SCM_GET_ID_FROM_SERVICE_MODULE_INDEX(p_pdb->trace_int,
                        handler_module_index); 
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_SEND_FAILURE_TO_SL)
                ret_val = p_pdb->p_scm_data->
                    scm_sl_info[handler_module_index].msg_handler(p_pdb->p_msg);
                break;
                }
#endif
        case ICF_NEW_SERVICE_TRIGGER: 
                send_to_pa = ICF_FALSE;
                break;
        default : 
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_INV_API_RECVD)
                send_to_pa = ICF_FALSE;
                break;
    }
#ifdef ICF_STAT_ENABLED
    /* To remove warning */
    stat_id = stat_id;
#endif
    
    /* If Response has to be sent to PA */
    if(ICF_TRUE == send_to_pa)
    {
        icf_uint8_t     *p_temp_msg_data;
        icf_return_t     temp_ret = ICF_SUCCESS;

        /* To remove warning */
        temp_ret = temp_ret;
        
        /*Allocate memory for external Msg */
        ICF_MEMGET(p_glb_pdb, 
                api_len, 
                ICF_MEM_COMMON, 
                p_msg, 
                ICF_RET_ON_FAILURE, 
                p_ecode, 
                ret_val)

        switch (*p_ecode)
        {
            case ICF_ERROR_SERVICE_NOT_SUBSCRIBED:
                ecode = ICF_CAUSE_SERVICE_NOT_SUBSCRIBED;
                break;

            case ICF_ERROR_SERVICE_NOT_ACTIVE:
                ecode = ICF_CAUSE_SERVICE_NOT_ACTIVATED;
                break;
            
            case ICF_ERROR_INVALID_SERVICE_INTERACTION:
                ecode = ICF_CAUSE_PROHIBITED_BY_SERVICE_INTERACTION;
                break;
            case ICF_CAUSE_INVALID_CALL_ID:
                ecode = ICF_CAUSE_INVALID_CALL_ID;
                break;
            default:
                break;                
        } /* switch (*p_ecode) */
        
        icf_cmn_fill_resp_msg(p_glb_pdb, 
            (icf_msg_st *)p_msg,
            (icf_int_module_id_t)(ICF_MODULE_PA),
            (icf_api_id_t const)(pa_api_id),
            (icf_result_t const)ICF_FAILURE, 
            ecode);

        ((icf_msg_st *)p_msg)->hdr.api_length = (icf_uint16_t) api_len;
        
        ICF_SCM_TRACE(p_pdb,ICF_TRACE_SEND_FAILURE_TO_PA)
        
        /* Save previous msg_data */
        p_pdb->p_msg->msg_hdr.msg_id = pa_api_id;
        p_temp_msg_data = p_pdb->p_msg->p_msg_data;
        p_pdb->p_msg->p_msg_data = p_msg;
        p_pdb->p_msg->msg_hdr.payload_length = (icf_uint16_t) api_len;
        
        /*Call ADM entry point to send message to PA*/
        ret_val =  icf_adm_process_mesg(p_pdb->p_msg);
        ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON, p_ecode, temp_ret)
        p_pdb->p_msg->p_msg_data = p_temp_msg_data;
            
        ICF_STAT(p_glb_pdb,stat_id) 
    } /* if(ICF_TRUE == send_to_pa) */
    if(ICF_FAILURE == ret_val)
    {
        ICF_SCM_TRACE(p_pdb,ICF_TRACE_FAILURE_RESP_SEND_FAIL)
    } /* if(ICF_FAILURE == ret_val) */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_route_message
 *
 * DESCRIPTION   : This function routes the message to the appropriate service 
 *                 module.
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t icf_scm_route_message(
        INOUT		icf_scm_pdb_st   *p_pdb,
        INOUT		icf_error_t      *p_ecode
        )
{
    icf_service_id_t handler_module;
    icf_uint8_t      handler_module_index;
    icf_boolean_t    is_valid, svc_ctx1_alloc, sl_ctx1_alloc, svc_ctx2_alloc;
    icf_boolean_t    route_message_to_handler_module = ICF_TRUE;
    icf_call_id_t    call_id;
    icf_call_ctx_st 	*p_call_ctx = ICF_NULL;
    icf_return_t     ret_val,temp_ret;
    icf_scm_data_st  *p_scm_data = p_pdb->p_scm_data;
    icf_glb_pdb_st   *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;
    icf_uint8_t      active_service_status = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Initialize boolean variables */
    is_valid=svc_ctx1_alloc=sl_ctx1_alloc=svc_ctx2_alloc=ICF_FALSE;
    ret_val = ICF_SUCCESS;

    /* If service context is allocated copy active service bitmask */
    if(ICF_NULL != p_pdb->p_service_ctx)
        active_service_status=
            p_pdb->p_service_ctx->active_service_list.active_service_status;
    /* get handler module of the api */
    if(ICF_FAILURE == icf_scm_get_handler_module(p_pdb,
                &handler_module,
                p_ecode))
    {
        return ICF_FAILURE;
    }
    p_pdb->trace_int = handler_module;
    ICF_SCM_TRACE(p_pdb,ICF_TRACE_GET_HANDLER_SL)
    ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(handler_module,
            handler_module_index); 
    /* Switch API */
    switch(p_pdb->msg_id)
    {
        case ICF_USER_INITIATE_EXTERNAL_CONF_REQ:
                /*This is initiate_external_conference request so call context
		 * and service context will not be present and will not be 
		 * allocated in SCM also*/
		       if(ICF_FAILURE == icf_scm_validate_new_service(p_pdb, 
                            &is_valid, 
                            p_ecode))
                {
                    route_message_to_handler_module = ICF_FALSE;
                    break;
                } 
		       if (ICF_FALSE == is_valid)
		       {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_REQ_INVALID)
                    ret_val = icf_scm_send_service_failure_resp(p_pdb,
                            p_ecode);
                    route_message_to_handler_module = ICF_FALSE;
		        }
		break;
        case ICF_SC_INTERNAL_CALL_HOLD_REQ:
        case ICF_USER_CALL_HOLD:
        case ICF_USER_CALL_RESUME:
        case ICF_SC_INTERNAL_CALL_RESUME_REQ:
        case ICF_NW_INC_REFER:
        case ICF_USER_CALL_TRANSFER_UNATTENDED: 
        case ICF_NEW_SERVICE_TRIGGER:
        case ICF_INTERNAL_REFER_REQ:
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_REQ_ONE_CALL)
                /* Validate the New Service Request 
                 * against Service Interaction Rules */

                if(ICF_FAILURE == icf_scm_validate_new_service(p_pdb, 
                            &is_valid, 
                            p_ecode))
                {
                    route_message_to_handler_module = ICF_FALSE;
                    break;
                } /* if(ICF_FAILURE == temp_ret) */
                

                /* For Incoming REFER in Invalid state , 
                 * set the msg id and forward */
                if((ICF_NW_INC_REFER == p_pdb->msg_id) 
                        && (ICF_FALSE == is_valid))
                {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_RNW_INV_STATE)
                    p_pdb->p_msg->msg_hdr.msg_id = ICF_SC_INTERNAL_RNW_REJ;
                    is_valid = ICF_TRUE;
                }

                if (ICF_TRUE == is_valid)
                {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_REQ_VALID)
                    
                    /* Validations for NEW SERVICE TRIGGER */
                    if(ICF_NEW_SERVICE_TRIGGER == p_pdb->msg_id) 
                    {
                        switch( ((icf_service_trigger_st *)p_pdb->p_msg->\
                                    p_msg_data)->status)
                        {
                            case ICF_STATUS_CALL_FWD_UNCOND:
                            case ICF_STATUS_CALL_FWD_NO_ANS:
                            case ICF_STATUS_CALL_FWD_BUSY_LINE:
                                /* New service Request for CFW */
                                break;
                            case ICF_STATUS_CALL_WAITING:
                                if(ICF_TRUE ==
                                        p_pdb->call_waiting_fwd)
                                {
                                    icf_uint8_t dbm_line_status;
                                    icf_line_data_st *p_line_data = 
                                        ICF_NULL;
                                    if(ICF_FAILURE ==
                                            icf_dbm_get_line_cntxt_blk(
                                                p_glb_pdb,
                                                p_glb_pdb->p_call_ctx->line_id,
                                                &p_line_data,
                                                &dbm_line_status,
                                                p_ecode))
                                    {
                                        route_message_to_handler_module = 
                                            ICF_FALSE;
                                        break;
                                    } /* &p_line_data,p_ecode)) */
                                        
                                    ICF_ASSERT(p_glb_pdb,
                                            (ICF_NULL!=p_line_data))

                                    /* Increment Number of Waiting Calls */
                                    p_line_data->num_call_waiting_calls++;
                                    
                                    /* Set Call Waiting Status*/
                                    p_pdb->p_msg->msg_hdr.status |= 
                                        ICF_STATUS_CALL_WAITING;
                                    
                                    route_message_to_handler_module = 
                                        ICF_FALSE;
                                }
                                
                                break;
                                
                            case ICF_STATUS_CALL_FWD_BUSY_N_CALL_WAITING:
                                if(ICF_TRUE ==
                                        p_pdb->call_waiting_fwd)
                                {
                                    icf_uint8_t dbm_line_status;
                                    icf_line_data_st *p_line_data;
                                    if(ICF_FAILURE ==
                                            icf_dbm_get_line_cntxt_blk(
                                                p_glb_pdb,
                                                p_glb_pdb->p_call_ctx->line_id,
                                                &p_line_data,
                                                &dbm_line_status,
                                                p_ecode))
                                    {
                                        route_message_to_handler_module = 
                                            ICF_FALSE;
                                        break;
                                    } /* &p_line_data,p_ecode)) */
                                        
                                    ICF_ASSERT(p_glb_pdb,
                                            (ICF_NULL!=p_line_data))

                                    /* Increment Number of Waiting Calls */
                                    p_line_data->num_call_waiting_calls++;
                                    
                                    /* Set Call Waiting Status*/
                                    p_pdb->p_msg->msg_hdr.status |= 
                                        ICF_STATUS_CALL_WAITING;
                                    
                                    route_message_to_handler_module = 
                                        ICF_FALSE;
                                }
                                
                                else
                                {
                                    ((icf_service_trigger_st *)
                                        p_pdb->p_msg->p_msg_data)->status = 
                                            ICF_STATUS_CALL_FWD_BUSY_LINE;
                                }
                                break;
                            
                            default : 
                                /* Status is none of the above Dump the msg*/
                                ICF_SCM_SET_ECODE(p_pdb,
                                        ICF_ERROR_INV_PAYLOAD_RECVD)
                                route_message_to_handler_module = 
                                    ICF_FALSE;
                                break;
                        } /* p_msg_data)->status) */
                    }/* if(ICF_NEW_SERVICE_TRIGGER == .. */
                    
                    if(ICF_FALSE == route_message_to_handler_module)
                        break;
                    /* Create Service Context if not present */
                    if (ICF_NULL == p_pdb->p_service_ctx)
                    {
                        ret_val = icf_scm_alloc_srvc_cntxt_blk(p_pdb,
                                &(p_pdb->p_service_ctx),
                                p_ecode);
                        if(ICF_FAILURE == ret_val)
                        {
                            break;
                        } /* if(ICF_FAILURE == ret_val) */
                        else
                        {
                            ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_CTX_CREATE)
                            p_pdb->p_service_ctx->p_call_ctx = 
                                             p_glb_pdb->p_call_ctx;
                            /* Assign service ctx in the call ctx */
                            p_glb_pdb->p_call_ctx->p_service_ctx = 
                                (icf_void_t *)(p_pdb->p_service_ctx);

                            svc_ctx1_alloc = ICF_TRUE;
#ifdef ICF_TRACE_ENABLE
                       p_glb_pdb->p_call_ctx->p_trace_criteria_data->
                            is_service_call = ICF_TRUE;
                       p_glb_pdb->p_call_ctx->p_trace_criteria_data->
                            bitmask |= ICF_TRACE_IS_SERVICE_CALL_PRESENT;
#endif
                        } /* else */
                    } /* if (ICF_NULL == p_pdb->p_se... */

                    if (!(active_service_status & 
                                p_scm_data->scm_sl_info[handler_module_index-1].
                                service_bitmask))
                    { 
                        ret_val = icf_scm_alloc_srvc_cntxt_lgc_blk(p_pdb,
                                &(((p_pdb->p_service_ctx)->active_service_list).
                                    p_sl_ctx[handler_module_index-1]), 
                                p_ecode);
                        if(ICF_FAILURE == ret_val)
                        {
                            if(ICF_TRUE == svc_ctx1_alloc)
                            {
                                icf_dbm_dealloc_srvc_cntxt_blk(
                                        p_glb_pdb,
                                        p_pdb->p_service_ctx,
                                        p_ecode);
                                p_pdb->p_service_ctx = ICF_NULL;
                                
                                p_glb_pdb->p_call_ctx->p_service_ctx = 
                                    ICF_NULL;
                                break;
                            } /* if(ICF_TRUE == svc_ctx1_all... */
                        } /* if(ICF_FAILURE == ret_val) */
                        ICF_SCM_TRACE(p_pdb,ICF_TRACE_SL_CTX_CREATE)
                        p_pdb->p_service_ctx->active_service_list.
                            active_service_status |= 
                            p_scm_data->scm_sl_info[handler_module_index-1].
                            service_bitmask;
                    }
                }
                else
                {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_REQ_INVALID)
                    ret_val = icf_scm_send_service_failure_resp(p_pdb,
                            p_ecode);
                    route_message_to_handler_module = ICF_FALSE;
                }
                break;

        case ICF_MODIFY_MEDIA_RESP:
        case ICF_NW_INC_MEDIA_UPDATE_RESP:
        case ICF_NW_INC_CALL_REJECT:
        case ICF_NW_INC_CALL_ABORT:
        case ICF_NW_OTG_CALL_REJECT:
        case ICF_NW_INC_NOTIFY_RESP:
        case ICF_TIMER_EXPIRY_IND:
        case ICF_NW_INC_MEDIA_UPDATE:
            break;

        case ICF_CREATE_CONF_RESP:
        case ICF_SC_INTERNAL_MEDIA_COMMIT:
            if (ICF_NULL != (ICF_SERVICE_TWC & active_service_status))
            {
                handler_module = ICF_INT_MODULE_TWC;
            } /* if (ICF_NULL != (ICF_SER... */
            else if (ICF_NULL != 
                    (ICF_SERVICE_RNW & active_service_status))
            {
                handler_module = ICF_INT_MODULE_RNW;
            }
            
            ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(handler_module,
                        handler_module_index);
            break;
                
        case ICF_MEDIA_ERROR_IND:
                handler_module = 
                    p_pdb->p_service_ctx->p_call_ctx->media_ongoing;
                ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(handler_module,
                        handler_module_index); 
                break;

#if 0
        case ICF_TIMER_EXPIRY_IND:
                {
                /*Extract handler module from Timer Data */
                icf_timer_data_st *p_timer_data =
                    (icf_timer_data_st *)p_glb_pdb->p_recv_msg->payload;
                handler_module = p_timer_data->module_id;
                ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(handler_module,
                    handler_module_index); 
                }
                break;
#endif
        case ICF_USER_CALL_TERMINATE:
            {
                icf_uint8_t iterator = 0;

            /*If service is CTU or CTA then only forward PA call termination*/
            if (ICF_NULL != (ICF_SERVICE_CTA & active_service_status))
            {
                iterator = ICF_INT_MODULE_CTA - ICF_INT_MODULE_SL_START;
                handler_module_index = 0;
                ret_val = 
                (p_scm_data->scm_sl_info[iterator - 1]).msg_handler(p_pdb->p_msg);
            }
            else if (ICF_NULL != 
                    (ICF_SERVICE_CTU & active_service_status))
            {
                iterator = ICF_INT_MODULE_CTU - ICF_INT_MODULE_SL_START;
                handler_module_index = 0;
                ret_val = 
                (p_scm_data->scm_sl_info[iterator - 1]). msg_handler(p_pdb->p_msg);
            }

            p_pdb->current_service_index = (icf_uint8_t)(iterator - 1);
            
            ICF_SCM_GET_ID_FROM_SERVICE_MODULE_INDEX(
                    p_pdb->trace_int,iterator - 1); 
            
            ICF_SCM_TRACE(p_pdb,
                    ICF_TRACE_CALL_RELEASE_FORWARDED)
            
            route_message_to_handler_module = ICF_FALSE;
            
            if(ICF_SUCCESS == ret_val)
            {
                ret_val = icf_scm_process_return_status(p_pdb, p_ecode);
                break;
            } /* if(ICF_FAILURE == ret_val) */            

            break;
            } /* case ICF_USER_CALL_TERMINAT... */               
                
        case ICF_FORCEFUL_CALL_CLEAR_IND:
        case ICF_CC_CALL_RELEASED:
             {                    
                icf_uint8_t   iterator;
                icf_uint32_t  status = 0;
                
                /* Send This message to all active Services */
                for(iterator=0 ; iterator< ICF_SCM_MAX_SL; iterator++)
                {
                    if (p_pdb->p_service_ctx->active_service_list.
                            active_service_status & 
                            p_scm_data->scm_sl_info[iterator].\
                        service_bitmask)
                    {
                        p_pdb->p_msg->msg_hdr.msg_id = p_pdb->msg_id;

                        p_pdb->p_msg->msg_hdr.status = ICF_NULL;
                        
                        ret_val = 
                            (p_scm_data->scm_sl_info[iterator]).msg_handler(
                            p_pdb->p_msg);

                        p_pdb->current_service_index = (icf_uint8_t) (iterator + 1);
                        ICF_SCM_GET_ID_FROM_SERVICE_MODULE_INDEX(
                                p_pdb->trace_int,iterator + 1); 
                        ICF_SCM_TRACE(p_pdb,
                                ICF_TRACE_CALL_RELEASE_FORWARDED)

                        if (ICF_SUCCESS == ret_val)
                            ret_val = icf_scm_process_return_status(p_pdb,
                            p_ecode);

                        if (ICF_NULL == status)
                            status = p_pdb->p_msg->msg_hdr.status;              
                        else if (ICF_STATUS_SERVICE_ENDED_ERROR & 
                            p_pdb->p_msg->msg_hdr.status)
                        {
                            status = p_pdb->p_msg->msg_hdr.status;
                        }
                        else if(ICF_STATUS_SERVICE_ENDED_ERROR & status)
                        {
                            status = status;
                        }
                        else
                        {
                            status |= p_pdb->p_msg->msg_hdr.status;
                        }
                        

                        if(ICF_FAILURE == ret_val || 
                                ICF_NULL == p_pdb->p_service_ctx) 
                        {
                            break;
                        } /* if(ICF_FAILURE == ret_val) */
                    }
                }

                p_pdb->p_msg->msg_hdr.status = status;
                route_message_to_handler_module = ICF_FALSE;

                break;
            }
        case ICF_START_PROC_SIGNALING:
                handler_module_index = p_pdb->p_service_ctx->expected_events.
                    start_proc_signaling;
                break;
        case ICF_START_PROC_MEDIA:
                handler_module_index = p_pdb->p_service_ctx->expected_events.
                    start_proc_media;
                break;
        case ICF_SC_INTERNAL_CALL_HOLD_RESP:
        case ICF_SC_INTERNAL_CALL_RESUME_RESP:
                handler_module_index = 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr;
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr=
                        ICF_NULL;
                break;

        case ICF_SC_INTERNAL_TWC_RESP:
                handler_module_index = 
                    p_pdb->p_service_ctx->expected_events.resp_frm_twc;

                p_pdb->p_service_ctx->expected_events.resp_frm_twc=
                        ICF_NULL;
                break;

        case ICF_SC_INTERNAL_RNW_RESP:
                handler_module_index = 
                    p_pdb->p_service_ctx->expected_events.resp_frm_rnw;

                p_pdb->p_service_ctx->expected_events.resp_frm_rnw=
                        ICF_NULL;
                break;                

        case ICF_SC_INTERNAL_CHR_RESP:
                handler_module_index = 
                    p_pdb->p_service_ctx->expected_events.resp_frm_chr;

                p_pdb->p_service_ctx->expected_events.resp_frm_chr=
                        ICF_NULL;
                break;                 

        case ICF_SC_SERVICE_ERROR_IND:
                handler_module_index = p_pdb->p_service_ctx->\
                        expected_events.service_ind_handler;
                p_pdb->p_service_ctx->\
                        expected_events.service_ind_handler = 
                        ICF_NULL;
                break;
        /* Same handling for ICF_INTERNAL_REFER_W_REP_REQ will be done as for 
         * ICF_USER_CALL_TRANSFER_ATTENDED because add party request for XCONF
         * uses exisiting CTA logic */
        case ICF_USER_CALL_CONF:
        case ICF_USER_CALL_TRANSFER_ATTENDED:
        case ICF_INVOKE_TWC_FOR_JOIN:
        case ICF_INTERNAL_REFER_W_REP_REQ:
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_REQ_TWO_CALLS)
                if(ICF_FAILURE == icf_scm_validate_new_service(p_pdb, 
                            &is_valid, 
                            p_ecode))
                {
                    route_message_to_handler_module = ICF_FALSE;
                    break;
                } /* if(ICF_FAILURE == temp_ret) */

                if (ICF_TRUE == is_valid)
                {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_REQ_VALID)
                    /* Allocate Service Context */
                    if (p_pdb->p_service_ctx == ICF_NULL)
                    {
                        ret_val = icf_scm_alloc_srvc_cntxt_blk(
                                p_pdb,
                                &(p_pdb->p_service_ctx), 
                                p_ecode);
                        if(ICF_FAILURE == ret_val)
                        {
                            break;
                        } /* if(ICF_FAILURE == ret_val) */
                        else
                        { 
                            p_pdb->p_service_ctx->p_call_ctx = 
                                p_glb_pdb->p_call_ctx;
                            p_glb_pdb->p_call_ctx->p_service_ctx = 
                                (icf_void_t *)(p_pdb->p_service_ctx);
                            svc_ctx1_alloc = ICF_TRUE;
#ifdef ICF_TRACE_ENABLE
                       p_glb_pdb->p_call_ctx->p_trace_criteria_data->
                            is_service_call = ICF_TRUE;
                       p_glb_pdb->p_call_ctx->p_trace_criteria_data->
                            bitmask |= ICF_TRACE_IS_SERVICE_CALL_PRESENT;
#endif
                        } /* else */
                    } /* if (p_pdb->p_service_ctx == IP... */
                    
                    /* Allocate Service Logic Context */
                    if (!(active_service_status & p_scm_data->
                                scm_sl_info[handler_module_index-1].
                                service_bitmask))
                    {
                        ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_CTX_CREATE)
                        ret_val = icf_scm_alloc_srvc_cntxt_lgc_blk(
                                p_pdb,
                                &(((p_pdb->p_service_ctx)->active_service_list).
                                    p_sl_ctx[handler_module_index-1]),
                                p_ecode);
                        if(ICF_FAILURE == ret_val)
                        {
                            if(ICF_TRUE == svc_ctx1_alloc)
                            {
                                icf_dbm_dealloc_srvc_cntxt_blk(
                                        p_glb_pdb,
                                        p_pdb->p_service_ctx,
                                        p_ecode);

                                p_pdb->p_service_ctx = ICF_NULL;
                                
                                p_glb_pdb->p_call_ctx->p_service_ctx = 
                                    ICF_NULL;
                            }
                            break;
                        } /* if(ICF_FAILURE == ret_val) */
                        else
                        {
                           sl_ctx1_alloc = ICF_TRUE;
                             
                        } /* else */

                        ICF_SCM_TRACE(p_pdb,ICF_TRACE_SL_CTX_CREATE)

                    }
		    if (ICF_USER_CALL_CONF == p_pdb->msg_id)
		    {
                         /* if this call is join header call, send error to PA as 
                            conference can?™t be initiated */
	                if (p_glb_pdb->p_call_ctx->call_to_be_joined != ICF_INVALID_CALL_ID)
	                {
		            *p_ecode = ICF_CAUSE_CALL_MERGED_DUE_TO_JOIN;
		             return ICF_FAILURE;
	                }
			/* get other participant call_id from external message */
		        call_id = ((icf_pa_cc_conference_req_st *) 
		        (p_glb_pdb->p_recv_msg->payload))->merge_call_id;
		    }
            else if (ICF_INVOKE_TWC_FOR_JOIN == p_pdb->msg_id)
	        {
		        call_id = p_glb_pdb->p_call_ctx->call_to_be_joined;
	        }
		    else if(ICF_INTERNAL_REFER_W_REP_REQ == p_pdb->msg_id)
		    {
                /* populate the call_id from replaces_call_id which is
                 * populated in XCONF for add_party_request*/
                call_id = ((icf_internal_refer_req_st*)
                        (p_pdb->p_msg->p_msg_data))->replaces_call_id;
            }

			 /*get other participant call_id from external message */
            else
            {
			    call_id = ((icf_call_transfer_attended_req_st *)
                        (p_glb_pdb->p_recv_msg->payload))->transferred_party_id;
            }
		    
                    p_pdb->trace_int = call_id;
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_CALL_ID_EXTRACTED)

                    temp_ret = icf_dbm_find_call_cntxt_blk(p_glb_pdb, 
                            call_id, 
                            &p_call_ctx, 
                            p_ecode);
                    if(ICF_FAILURE == temp_ret)
                    {
                        if(ICF_TRUE == sl_ctx1_alloc)
                        {
                            icf_dbm_dealloc_srvc_cntxt_lgc_blk(
                                    p_glb_pdb,
                                    p_pdb->p_service_ctx->active_service_list.\
                                    p_sl_ctx[handler_module_index-1],
                                    p_ecode);
                        } 
                        if(ICF_TRUE == svc_ctx1_alloc)
                        {
                            icf_dbm_dealloc_srvc_cntxt_blk(
                                    p_glb_pdb,
                                    p_pdb->p_service_ctx,
                                    p_ecode);

                            p_pdb->p_service_ctx = ICF_NULL;
                                
                            p_glb_pdb->p_call_ctx->p_service_ctx = 
                                ICF_NULL;
                        } 
                        break;
                    } 
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_GET_CALL_CTX)
                    ICF_ASSERT(p_glb_pdb,(ICF_NULL!=p_call_ctx))
                    
                    /* Store Other Call Context in PDB.
                     * This mechanism will not work if Services involving
                     * 2 calls are invoked internally. Currently only services
                     * involving single call are being invoked internally
                     * */
                    if (p_call_ctx->p_service_ctx == ICF_NULL)
                    {

                        ret_val = icf_scm_alloc_srvc_cntxt_blk(
                                p_pdb, 
                                (icf_service_ctx_st **)&(p_call_ctx->
                                    p_service_ctx), 
                                p_ecode);
                        if(ICF_FAILURE == ret_val)
                        {
                            if(ICF_TRUE == sl_ctx1_alloc)
                            {
                                icf_dbm_dealloc_srvc_cntxt_lgc_blk(
                                        p_glb_pdb,
                                        p_pdb->p_service_ctx->
                                        active_service_list.\
                                        p_sl_ctx[handler_module_index-1],
                                        p_ecode);
                            } 
                            if(ICF_TRUE == svc_ctx1_alloc)
                            {
                                icf_dbm_dealloc_srvc_cntxt_blk(
                                        p_glb_pdb,
                                        p_pdb->p_service_ctx,
                                        p_ecode);

                                p_pdb->p_service_ctx = ICF_NULL;
                                
                                p_glb_pdb->p_call_ctx->p_service_ctx = 
                                    ICF_NULL;
                            } 
                            break;
                        } /* if(ICF_FAILURE == ret_val) */
                        else
                        {
                            ((icf_service_ctx_st *)
                                (p_call_ctx->p_service_ctx))->p_call_ctx = 
                                p_call_ctx;
                            svc_ctx2_alloc = ICF_TRUE;
                        } /* else */

                        ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_CTX_CREATE)
                    }
                    /* Set SL Context in other call's service Context*/
                            ((icf_service_ctx_st *)
                    (p_call_ctx->p_service_ctx))->active_service_list.\
                        p_sl_ctx[handler_module_index-1] = 
                        p_pdb->p_service_ctx->active_service_list.\
                        p_sl_ctx[handler_module_index-1];

                    /* Mark Service as active in both Service Contexts */
                    p_pdb->p_service_ctx->active_service_list.
                        active_service_status |= 
                        p_scm_data->scm_sl_info[handler_module_index-1].
                        service_bitmask;

                    ((icf_service_ctx_st *)
                     (p_call_ctx->p_service_ctx))->active_service_list.
                        active_service_status |= 
                        p_scm_data-> scm_sl_info[handler_module_index-1].
                        service_bitmask;
                }
                else
                {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_REQ_INVALID)
                    if(ICF_INTERNAL_REFER_W_REP_REQ ==
                            p_pdb->msg_id)
                    {
                       ((icf_xconf_ctx_st*)(p_pdb->p_service_ctx->active_service_list. \
                            p_sl_ctx[ICF_INT_MODULE_XCONF - \
                            ICF_INT_MODULE_SL_START - 1]))->add_xconf_party_resp_sent
                           = ICF_TRUE;
                    }
                    ret_val = icf_scm_send_service_failure_resp(p_pdb,
                            p_ecode);
                    route_message_to_handler_module = ICF_FALSE;
                    break;
                }
                break;
        case ICF_NW_INC_NOTIFY_IND:
        case ICF_NW_INC_REFER_RESP:
                
                ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(
                        ICF_INT_MODULE_CTA, 
                        handler_module_index);

                /* If CTA is inactive */
                if (!( active_service_status 
                            & p_scm_data->scm_sl_info[handler_module_index-1].
                            service_bitmask))
                {
                    ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(
                            ICF_INT_MODULE_CTU, 
                            handler_module_index);
                    /* If CTU is also inactive, return failure */
                    if (!( active_service_status & 
                                p_scm_data->scm_sl_info[handler_module_index-1].
                                service_bitmask))
                    {
                        route_message_to_handler_module = ICF_FALSE;
                    }
                }
                break;
        case ICF_INTERNAL_REFER_RESP:
        case ICF_INTERNAL_SUBSCRIBE_REQ:
        case ICF_INTERNAL_SUBSCRIBE_RESP:
        case ICF_INTERNAL_NOTIFY_IND:
        case ICF_USER_ADD_PARTY_REQ:
        case ICF_USER_DELETE_PARTY_REQ:
        case ICF_USER_RELEASE_CONFERENCE_REQ:
        case ICF_EXTERNAL_CONFERENCE_CONNECTED:
                break;
        case ICF_INCOMING_EXTERNAL_CONFERENCE_REQ:
              /* Allocate Service Context */
              if (p_pdb->p_service_ctx == ICF_NULL)
              {
                 ret_val = icf_scm_alloc_srvc_cntxt_blk(
                                p_pdb,
                                &(p_pdb->p_service_ctx), 
                                p_ecode);
                 if(ICF_FAILURE == ret_val)
                 {
                    break;
                 } /* if(ICF_FAILURE == ret_val) */
                 else
                 { 
                     p_pdb->p_service_ctx->p_call_ctx = 
                                p_glb_pdb->p_call_ctx;
                     p_glb_pdb->p_call_ctx->p_service_ctx = 
                                (icf_void_t *)(p_pdb->p_service_ctx);
                     svc_ctx1_alloc = ICF_TRUE;
#ifdef ICF_TRACE_ENABLE
                     p_glb_pdb->p_call_ctx->p_trace_criteria_data->
                            is_service_call = ICF_TRUE;
                     p_glb_pdb->p_call_ctx->p_trace_criteria_data->
                           bitmask |= ICF_TRACE_IS_SERVICE_CALL_PRESENT;
#endif
                 } /* else */
              } /* if (p_pdb->p_service_ctx == IP... */
                    
              /* Allocate Service Logic Context */
              if (!(active_service_status & p_scm_data->
                                scm_sl_info[handler_module_index-1].
                                service_bitmask))
              {
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_SVC_CTX_CREATE)
                ret_val = icf_scm_alloc_srvc_cntxt_lgc_blk(
                                p_pdb,
                                &(((p_pdb->p_service_ctx)->active_service_list).
                                    p_sl_ctx[handler_module_index-1]),
                                p_ecode);
                if(ICF_FAILURE == ret_val)
                {
                   if(ICF_TRUE == svc_ctx1_alloc)
                   {
                     icf_dbm_dealloc_srvc_cntxt_blk(
                                        p_glb_pdb,
                                        p_pdb->p_service_ctx,
                                        p_ecode);

                     p_pdb->p_service_ctx = ICF_NULL;
                               
                     p_glb_pdb->p_call_ctx->p_service_ctx = 
                               ICF_NULL;
                   }
                   break;
                } /* if(ICF_FAILURE == ret_val) */
                ICF_SCM_TRACE(p_pdb,ICF_TRACE_SL_CTX_CREATE)
                p_pdb->p_service_ctx->active_service_list.
                      active_service_status |= 
                      p_scm_data->scm_sl_info[handler_module_index-1].
                      service_bitmask;

              }
              break;
        case ICF_USER_CALL_TRANSFER_INITIATED_CFM:
            ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(
                    ICF_INT_MODULE_RNW,
                    handler_module_index);
            break;
                
        default:
                {
                    ICF_SCM_TRACE(p_pdb,ICF_TRACE_INV_API_RECVD)
                    route_message_to_handler_module = ICF_FALSE;
                    break;
                }
    }

    if(ICF_FAILURE == ret_val)
    {
       ICF_SCM_TRACE(p_pdb,ICF_TRACE_MSG_ROUTING_FAILED)
    } /* if(ICF_FAILURE == ret_val) */

    else if(ICF_TRUE == route_message_to_handler_module)
    {
        ICF_SCM_GET_ID_FROM_SERVICE_MODULE_INDEX(p_pdb->trace_int,
            handler_module_index);
	/*If msg is external conference req then call xconf module*/
	    if (ICF_USER_INITIATE_EXTERNAL_CONF_REQ == p_pdb->msg_id) 
	    {
	        ICF_SCM_TRACE(p_pdb,ICF_TRACE_MSG_ROUTING_PROGRESS)
            ret_val = 
	              p_scm_data->scm_sl_info[handler_module_index-1].msg_handler
                (p_pdb->p_msg);
            if(ICF_SUCCESS == ret_val)
            {
                p_pdb->current_service_index = handler_module_index;
                ret_val = icf_scm_process_return_status(p_pdb,
                        p_ecode); 

                /*If message recvd is forceful call clear then return
                 * only service ended error or service ended normal*/
                if (ICF_FORCEFUL_CALL_CLEAR_IND == p_pdb->msg_id)
                {
                    if (ICF_NULL != (ICF_STATUS_SERVICE_ENDED_NORMAL & 
                                p_pdb->p_msg->msg_hdr.status))
                    {
                        p_pdb->p_msg->msg_hdr.status = 
                            ICF_STATUS_SERVICE_ENDED_ERROR;
                    } /* p_pdb->p_msg->msg_hdr.status)) */
                } /* if (ICF_FORCEFUL_CALL_CLEAR... */
            } /* if(ICF_SUCCESS == ret_val) */
	     }
        /* Check if service and service logic context exists */
	     else if(!((ICF_NULL != p_pdb->p_service_ctx)
                 && (ICF_NULL != handler_module_index)
                 && (ICF_NULL != p_pdb->p_service_ctx->\
                    active_service_list.p_sl_ctx[handler_module_index-1])))
         {
             ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_SL_NOT_RUNNING)
         }
         else
         {
             ICF_SCM_TRACE(p_pdb,ICF_TRACE_MSG_ROUTING_PROGRESS)
             ret_val =
             p_scm_data->scm_sl_info[handler_module_index-1].msg_handler(p_pdb->p_msg);
             if(ICF_SUCCESS == ret_val)
             {
                 p_pdb->current_service_index = handler_module_index;
                 ret_val = icf_scm_process_return_status(p_pdb,
                              p_ecode); 

                 /*If message recvd is forceful call clear then return
                  * only service ended error or service ended normal*/
                 if (ICF_FORCEFUL_CALL_CLEAR_IND == p_pdb->msg_id)
                 {
                     if (ICF_NULL != (ICF_STATUS_SERVICE_ENDED_NORMAL & 
                                p_pdb->p_msg->msg_hdr.status))
                     {
                          p_pdb->p_msg->msg_hdr.status = 
                             ICF_STATUS_SERVICE_ENDED_ERROR;
                     } /* p_pdb->p_msg->msg_hdr.status)) */
                 } /* if (ICF_FORCEFUL_CALL_CLEAR... */
              } /* if(ICF_SUCCESS == ret_val) */
                /*If ret_val = failure and ecode is generic error then return
                 * success and generic call clear as return status*/
              else if (ICF_ECODE_GENERIC_ERROR == p_pdb->p_msg->msg_hdr.ecode)
              {
                   p_pdb->p_msg->msg_hdr.status = 
                      ICF_STATUS_GENERIC_CALL_CLEAR;
                
                   ret_val = ICF_SUCCESS;
              } /* else if (ICF_ECODE_GENERIC_... */        

              if (((ICF_USER_CALL_HOLD == p_pdb->msg_id) ||
                    (ICF_USER_CALL_RESUME == p_pdb->msg_id)) &&
                    (ICF_NULL != p_pdb->p_chr_buff_evt))
              {
                /* Free the payload rcvd */
                ICF_MEMFREE(
                    p_glb_pdb,
                    p_pdb->p_chr_buff_evt, 
                    ICF_MEM_COMMON, 
                    p_ecode, 
                    temp_ret)

                 p_pdb->p_msg->p_msg_data = ICF_NULL;                        
              }
        } /* else */
        
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
/*******************************************************************************
*
* FUNCTION NAME : icf_scm_check_subs_and_actv
*
* DESCRIPTION   : This function checks the subscription and activation 
*                 status for Service requests and returns failure if the 
*                 requested service is not subscribed or activated on the line.
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_scm_check_subs_and_actv(
		INOUT   icf_scm_pdb_st   *p_pdb,
          OUT   icf_boolean_t    *p_valid,
		INOUT   icf_error_t      *p_ecode
	)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;
    icf_uint16_t     subs_flag,actv_flag; 
    icf_line_data_st *p_line_data = ICF_NULL;
    icf_uint8_t     dbm_line_status;
    icf_string_list_st      *p_supported_events = ICF_NULL;
    icf_app_conf_st         *p_app_conf = ICF_NULL;
    icf_string_list_node_st *p_cur_node = ICF_NULL;


        
    ICF_FUNCTION_ENTER(p_glb_pdb)

    *p_valid = ICF_TRUE;
    subs_flag = ICF_NULL;
    actv_flag = ICF_NULL;

    if(ICF_USER_INITIATE_EXTERNAL_CONF_REQ !=
            p_pdb->msg_id)
    {   
        ret_val =  icf_dbm_get_line_cntxt_blk(
            p_glb_pdb,
            p_glb_pdb->p_call_ctx->line_id,
            &p_line_data,
            &dbm_line_status,
            p_ecode);
        ICF_ASSERT(p_glb_pdb,(ICF_NULL!=p_line_data))
    }
    else
    {
        ret_val =  icf_dbm_get_line_cntxt_blk(
            p_glb_pdb,
            ((icf_create_ext_xconf_req_st*)&(p_glb_pdb->p_recv_msg->payload[0]))->line_id,
            &p_line_data,
            &dbm_line_status,
            p_ecode);
        ICF_ASSERT(p_glb_pdb,(ICF_NULL!=p_line_data))

    }
    
    if(ICF_SUCCESS == ret_val)
    {
        /* Switch API id */
        switch(p_pdb->msg_id)
        {
            case ICF_SC_INTERNAL_CALL_HOLD_REQ:
            case ICF_SC_INTERNAL_CALL_RESUME_REQ:
            case ICF_USER_CALL_HOLD:
            case ICF_USER_CALL_RESUME:
                subs_flag = ICF_SERVICE_SUBS_CALL_HOLD_RESUME;
                actv_flag = ICF_SERVICE_ACT_CALL_HOLD_RESUME;
                break;
            case ICF_USER_CALL_TRANSFER_UNATTENDED:
            case ICF_INTERNAL_REFER_REQ:    
                subs_flag = ICF_SERVICE_SUBS_CALL_TRF_UNATTENDED;
                actv_flag = ICF_SERVICE_ACT_CALL_TRF_UNATTENDED;
                break;
            case ICF_NW_INC_REFER:
                subs_flag = ICF_SERVICE_SUBS_CALL_TRF_FROM_NW;
                actv_flag = ICF_SERVICE_ACT_CALL_TRF_FROM_NW;
                break;
            case ICF_USER_CALL_CONF:
                subs_flag = ICF_SERVICE_SUBS_THREE_WAY_CALL;
                actv_flag = ICF_SERVICE_ACT_THREE_WAY_CALL;
                break;
            case ICF_USER_CALL_TRANSFER_ATTENDED:
            case ICF_INTERNAL_REFER_W_REP_REQ:    
                subs_flag = ICF_SERVICE_SUBS_CALL_TRF_ATTENDED;
                actv_flag = ICF_SERVICE_ACT_CALL_TRF_ATTENDED;
                break;
            case ICF_USER_INITIATE_EXTERNAL_CONF_REQ:
                 subs_flag = ICF_SERVICE_SUBS_EXTERNAL_CONFERENCE;
                 actv_flag = ICF_SERVICE_ACT_EXTERNAL_CONFERENCE;
                break;
            case ICF_INVOKE_TWC_FOR_JOIN:
		        subs_flag = ICF_SERVICE_SUBS_JOIN_HEADER;
               	actv_flag = ICF_SERVICE_ACT_JOIN_HEADER;
                /*spr17104 invoking service only if supported
                  event does have join */
                if(ICF_FAILURE == icf_cfg_fetch_app_conf(
                      p_glb_pdb,
                      p_glb_pdb->p_call_ctx->app_id,
                      &p_app_conf,
                      p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                else 
                { 
                     p_supported_events = 
                         &(p_app_conf->app_profile.supported_events);
                     if((ICF_NULL != p_supported_events) &&
                        (ICF_NULL != p_supported_events->p_list))  
                     {
                         icf_uint8_t count = 0;

                         for(count = (icf_uint8_t) p_supported_events->count,
                             p_cur_node = p_supported_events->p_list;
                             count--; p_cur_node = p_cur_node->p_next)
                         {
                            if(0 == icf_port_strcasecmp((const icf_int8_t * )"join",(const icf_int8_t * ) p_cur_node->str.str))
                             {
                                /*when join is present then only we will send
                                  merge_media_req and hence further invoke the
                                  service*/
                                *p_valid = ICF_TRUE;
                                break;
                             }
                            else
                             {
                                /* In app_add join is absent in supported
                                   event so we are not invoking service to
                                   send the merge_media_req */
                                *p_valid = ICF_FALSE;
                             }
                         }
                      }
                      else
                      {
                          /* case when there is no supported events in
                           * app_add*/
                          *p_valid = ICF_FALSE;
                      }    
                }/*end spr17104*/    

               	break;
 
            case ICF_NEW_SERVICE_TRIGGER:
                switch(((icf_service_trigger_st *)p_pdb->p_msg->
                            p_msg_data)->status)
                {
                    case ICF_STATUS_CALL_FWD_UNCOND:
                        subs_flag = ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL;
                        actv_flag = ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL;
                        break;
                        
                    case ICF_STATUS_CALL_FWD_NO_ANS:
                        subs_flag = ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER;
                        actv_flag = ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER;
                        break;

                    case ICF_STATUS_CALL_FWD_BUSY_LINE:
                        subs_flag = ICF_SERVICE_SUBS_CALL_FWD_BUSY;
                        actv_flag = ICF_SERVICE_ACT_CALL_FWD_BUSY;
                        break;
                        
                    case ICF_STATUS_CALL_WAITING:
                        subs_flag = ICF_SERVICE_SUBS_CALL_WAITING;
                        actv_flag = ICF_SERVICE_ACT_CALL_WAITING;

                        /* If Call waiting is subscibed and activated */
                        if((ICF_NULL !=
                                    (subs_flag &
                                     p_line_data->service_subscription_flag))
                                     && (ICF_NULL != (actv_flag &
                                         p_line_data->service_activation_flag)))
                       {
                           /* Call waiting active */
                           icf_config_data_st   *p_config_data = ICF_NULL;
                           ret_val =  icf_dbm_get_module_glb_data(
                                   p_glb_pdb,
                                   (icf_int_module_id_t)
                                   ICF_INT_MODULE_CFG,
                                   (icf_void_t **)&p_config_data, 
                                   p_ecode);
                            if(ICF_FAILURE == ret_val)
                            {
                                break;
                            } /* if(ICF_FAILURE == ret_val) */
    
                            /* Validate Call waiting request */
                            if(p_line_data->num_call_waiting_calls < 
                                    p_config_data->
                                    max_num_of_calls_in_call_waiting)
                            {
                                /* Call waiting can be activated */
                                p_pdb->call_waiting_fwd = ICF_TRUE;
                            } 
                            else
                            {
                                *p_valid = ICF_FALSE;
                                ICF_SCM_SET_ECODE(
                                        p_pdb,
                                        ICF_ERROR_CALL_WAITING_FAILURE)
                                
                                p_pdb->p_msg->msg_hdr.status |=
                                    ICF_STATUS_SERVICE_ENDED_ERROR;
                                
                                p_glb_pdb->p_call_ctx->release_cause = 
                                    ICF_REL_CAUSE_USER_BUSY;
                                
                                p_glb_pdb->p_call_ctx->release_type |= 
                                    ICF_REL_TYPE_INTERNAL_INITIATED;
                                
                            } /* else */
                        }
                        break;
                        
                    case ICF_STATUS_CALL_FWD_BUSY_N_CALL_WAITING:
                        {
                            /* In this case we check if Call waiting 
                             * is permitted if not then we check for 
                             * Call Forward busy */
                            subs_flag = ICF_SERVICE_SUBS_CALL_WAITING;
                            actv_flag = ICF_SERVICE_ACT_CALL_WAITING;

                            /* If Call waiting is subscibed and activate */
                            if((ICF_NULL !=
                                        (subs_flag & p_line_data->
                                         service_subscription_flag)) 
                                    && (ICF_NULL != (actv_flag &
                                            p_line_data->
                                            service_activation_flag)))
                            {    
                                /* Call waiting active */
                                icf_config_data_st   *p_config_data = 
                                    ICF_NULL;
                                ret_val =  icf_dbm_get_module_glb_data(
                                        p_glb_pdb,
                                        (icf_int_module_id_t)
                                        ICF_INT_MODULE_CFG,
                                        (icf_void_t **)&p_config_data, 
                                        p_ecode);
                                if(ICF_FAILURE == ret_val)
                                {
                                    break;
                                } /* if(ICF_FAILURE == ret_val) */

                                /* Validate Call waiting request */
                                if(p_line_data->num_call_waiting_calls
                                        < p_config_data->\
                                        max_num_of_calls_in_call_waiting)
                                {
                                    /* Call waiting can be activated */
                                    p_pdb->call_waiting_fwd = ICF_TRUE;
                                } /* if(active_cw < max_cw) */
                            }

                            /* If Call waiting is not valid */
                            if(ICF_FALSE == p_pdb->call_waiting_fwd)
                            {
                                subs_flag = 
                                    ICF_SERVICE_SUBS_CALL_FWD_BUSY;
                                actv_flag = 
                                    ICF_SERVICE_ACT_CALL_FWD_BUSY;
                            } 
                            break;
                        } /* case ICF_STATUS_CALL_FWD_BU... */
                    default:
                        break;
                } /* p_msg_data)->status) */
            default :
                break;
        } /* switch(p_pdb->msg_id) */

        if((ICF_SUCCESS == ret_val) && (ICF_TRUE == *p_valid))
        {

            /* Set is_valid = FALSE and set error code if 
             * one of the flags is not set */
            if(ICF_NULL ==
                    (subs_flag & p_line_data->service_subscription_flag))
            {
                ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_SERVICE_NOT_SUBSCRIBED)
                    *p_valid = ICF_FALSE;
            } /* if(ICF_NULL ==(p_line_data-... */

            else if(ICF_NULL ==
                    (actv_flag & p_line_data->service_activation_flag))
            {
                ICF_SCM_SET_ECODE(p_pdb,ICF_ERROR_SERVICE_NOT_ACTIVE)
                    *p_valid = ICF_FALSE;
            } /* && (p_line_data->service_activ... */
        } /* if(ICF_SUCCESS == ret_val) */
    } /* if((ICF_SUCCESS == ret_val).. */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}

/*******************************************************************************
*
* FUNCTION NAME : icf_scm_alloc_srvc_cntxt_blk 
*
* DESCRIPTION   :This function calls the dbm function to allocate the 
*               service ctx block and initializes the parameters of the
*               service ctx created. 
*
*******************************************************************************/
icf_return_t icf_scm_alloc_srvc_cntxt_blk(
        INOUT icf_scm_pdb_st     *p_pdb,
        OUT   icf_service_ctx_st **p_p_service_ctx,
        INOUT icf_error_t        *p_ecode)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Call dbm to allocate the service ctx block */
    if (ICF_SUCCESS == icf_dbm_alloc_blk(
            p_glb_pdb,
            (icf_dbm_block_type_et)ICF_BLK_TYPE_SRVC_CTX,
            (icf_void_t **)(p_p_service_ctx),
            p_ecode))
    {
        /* Initiailze service context block to 0*/
       icf_port_memset(
                *p_p_service_ctx,
                (icf_uint8_t const)ICF_NULL,
                (icf_uint32_t const)sizeof(icf_service_ctx_st));
    }

    /* Failure to allocate the service ctx block */
    else
    {
        ret_val = ICF_FAILURE;
    }
            
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}/* End function */


/*******************************************************************************
*
* FUNCTION NAME : icf_scm_alloc_srvc_cntxt_lgc_blk 
*
* DESCRIPTION   :This function calls the dbm function to allocate the 
*               service logic ctx block and initializes the parameters of the
*               service logic ctx created. 
*
*******************************************************************************/
icf_return_t icf_scm_alloc_srvc_cntxt_lgc_blk(
        INOUT icf_scm_pdb_st     *p_pdb,
        OUT  icf_void_t          **p_p_service_logic_ctx,
        INOUT icf_error_t        *p_ecode)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = p_pdb->p_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Call dbm to allocate the service ctx block */
    if (ICF_SUCCESS == icf_dbm_alloc_blk(
            p_glb_pdb,
            (icf_dbm_block_type_et)ICF_BLK_TYPE_SL_CTX,
            p_p_service_logic_ctx,
            p_ecode))
    {
        /* Initiailze service logic context block to 0*/
       icf_port_memset(
                *p_p_service_logic_ctx,
                (icf_uint8_t const)ICF_NULL,
                (icf_uint32_t const)sizeof(icf_srvc_ctx_lgc_ut));
    }

    /* Failure to allocate the service logic ctx block */
    else
    {
        ret_val = ICF_FAILURE;
    }
            
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}/* End function */
#endif
