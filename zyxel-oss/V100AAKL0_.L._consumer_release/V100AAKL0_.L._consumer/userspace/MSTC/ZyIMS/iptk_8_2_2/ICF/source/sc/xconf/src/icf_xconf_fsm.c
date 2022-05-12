/*******************************************************************************
*
* FILE NAME    : icf_xconf_fsm.c
*
* DESCRIPTION  : Contains the definitions of all State Machine functions
*                of XCONF service
*
*
* Revision History :
*    DATE            NAME           REFERENCE       REASON
* --------      ------------       ---------       ---------------
* 4-Jul-2007 Abhishek Dhammawat     CDD Rel7.0      Initial Creation
* 8-Apr-2008 Sumant Gupta           SPR:18355       Windows porting Merging
* 07-Jun-2008  Amit Sharma       IPTK 8.0 SRTP LLD      SRTP Enhancements
* 5-Jul-2008 Amit Sharma            SPR:18714       Sent display_ind to app
*                                                   on SUBSCRIBE failure
*                                                   and subs_resp in IDLE state
* 04-Apr-2009  Rajiv Kumar          SPR: 19423      Klocwork Warning Removal.
* Copyright 2009,Aricent .
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_xconf_types.h"
#include "icf_xconf_prototypes.h"
#include "icf_xconf_macro.h"
#include "icf_xconf_defs.h"
#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_sc_prototypes.h"
#include "icf_api.h"

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_00_init_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_00_IDLE
*                 Event         ICF_XCONF_INITIATE_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_00_init_conf_req(
        INOUT  icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    /* variable to handle the incoming initiate conf payload */
    icf_create_ext_xconf_req_st  *p_xt_conf_req = ICF_NULL;
    icf_error_t                  ecode_temp = 0;
    icf_uint8_t                  handler_module_index;
    icf_config_data_st           *p_cfg_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    p_xt_conf_req = (icf_create_ext_xconf_req_st  *)
                    &(p_pdb->p_glb_pdb->p_recv_msg->payload[0]);

    handler_module_index = ICF_INT_MODULE_XCONF - ICF_INT_MODULE_SL_START;

    /*Fetch icf_config_data from DBM*/
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(p_pdb->p_glb_pdb,
                                  (icf_module_id_t)(ICF_INT_MODULE_CFG),
                                  (icf_void_t *)&p_cfg_data, p_pdb->p_ecode))
    {
         ret_val = ICF_FAILURE;
    }
    /*Allocate New Call Context block*/
    else if (ICF_FAILURE == 
            icf_dbm_alloc_call_cntxt_blk(p_pdb->p_glb_pdb,
                &(p_pdb->p_glb_pdb->p_recv_msg->hdr.call_id),
                &p_pdb->p_call_ctx, p_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /*If validation for conference uri present in API recvd from PA and in 
      set_service_params is failure then send INITIATE_CONFERENCE_RESP failure
      response and return service ended normal*/
    else if (ICF_FAILURE == icf_xconf_validate_conf_req(p_pdb))
    {
        /* sending failure response to PA for initiate_conference_req */
        /* this is send because validation is failed */
        *(p_pdb->p_ecode) = ICF_CAUSE_EXT_CONFERENCE_VALIDATION_FAILED;
        ret_val = ICF_FAILURE;
        /*Deallocate call context*/
        icf_dbm_dealloc_call_cntxt_blk(p_pdb->p_glb_pdb,
                          p_pdb->p_glb_pdb->p_recv_msg->hdr.call_id,
                          p_pdb->p_ecode);
    }
    /*Fill New Call Context data*/
    else if(ICF_FAILURE == icf_xconf_fill_call_ctx(p_pdb))
    {
        /*Deallocate all the memory allocated for p_call_ctx*/
        icf_xconf_clear_call_ctx(p_pdb, p_pdb->p_call_ctx);
        
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_xconf_alloc_n_fill_serv_ctx(p_pdb))
    {
        icf_dbm_dealloc_srvc_cntxt_blk(p_pdb->p_glb_pdb,
                p_pdb->p_call_ctx->p_service_ctx, &ecode_temp);
        icf_xconf_clear_call_ctx(p_pdb, p_pdb->p_call_ctx);
        ret_val = ICF_FAILURE;
    }
    else if(ICF_FAILURE == icf_xconf_alloc_srvc_cntxt_lgc_blk(p_pdb,
                              &(((p_pdb->p_service_ctx)->active_service_list).
                                  p_sl_ctx[handler_module_index-1]),
                              p_pdb->p_ecode))
    {
        icf_dbm_dealloc_srvc_cntxt_blk(p_pdb->p_glb_pdb,
                p_pdb->p_call_ctx->p_service_ctx, &ecode_temp);
        icf_xconf_clear_call_ctx(p_pdb, p_pdb->p_call_ctx);
        ret_val = ICF_FAILURE;
         
    }    
    else
    {
        p_pdb->p_xconf_ctx->p_call_ctx = 
             p_pdb->p_call_ctx;
        /*Set return status as ICF_STATUS_SERVICE_MSG_TO_CC_SETUP*/
        ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MSG_TO_CC_SETUP)

        ICF_XCONF_TRACE(p_pdb, ICF_TRACE_XCONF_NEW_CALL_ID)

        /*Set SRTP fallbck required flag in call context if same is present in 
        *XCONF request otherwise copy it from global config data*/
       if (ICF_CREATE_XCONF_SRTP_FALLBACK_REQD_PRESENT & p_xt_conf_req->bitmask)
       {
           p_pdb->p_xconf_ctx->p_call_ctx->srtp_fallback_reqd = 
                p_xt_conf_req->srtp_fallback_reqd;
       }
       else
       {
           p_pdb->p_xconf_ctx->p_call_ctx->srtp_fallback_reqd = 
                p_cfg_data->srtp_fallback_reqd;
       }

        /*Set media_transport_type in call context if same is present in 
        *XCONF request otherwise copy it from global config data*/
       if (ICF_CREATE_XCONF_MEDIA_TYPE_PRESENT & p_xt_conf_req->bitmask)
       {
           p_pdb->p_xconf_ctx->p_call_ctx->media_transport_type = 
                p_xt_conf_req->media_transport_type;
       }
       else
       {
           p_pdb->p_xconf_ctx->p_call_ctx->media_transport_type = 
                p_cfg_data->media_transport_type;
       }

        /* iF existing_call_id is present then invoke CTA with mesg 
           INTERNAL_REFER_REQ for handling already estb call moving into 
           conference set status as ICF_STATUS_INTERNAL_REFER_REQ*/
        if(p_xt_conf_req->existing_call_id)
         {
             p_pdb->p_xconf_ctx->conference_id = 
                      p_xt_conf_req->existing_call_id;  
         }
        else
        {
             p_pdb->p_xconf_ctx->conference_id =
                 ICF_INVALID_CALL_ID;
        }
        /*Wait for conference connected indication from CC*/
        ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_01_AWT_CONFERENCE_CONNECT)
    } /* else */

    if (ICF_FAILURE == ret_val)
    {
        icf_xconf_send_xternal_conf_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE);
        ret_val = ICF_SUCCESS;

    } /* if (ICF_FAILURE == ret_val) */
    else
    {
       /*Set common_bitmask in call_ctx that it is a call with conference 
        *server*/ 
       p_pdb->p_call_ctx->common_bitmask_2 |= ICF_CC_EXTERNAL_CONFERENCE_CALL;
       /*Assign new call id in global pdb*/
       p_pdb->p_glb_pdb->p_call_ctx = p_pdb->p_call_ctx;
    }

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)

    return ret_val;
} /* icf_xconf_00_init_conf_req */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_conf_server_connected
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_CONF_SERVER_CONNECTED
*
* RETURNS       : ICF_FAILURE/ICF_SUCCESS
*
*******************************************************************************/
icf_return_t icf_xconf_01_conf_server_connected(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_config_data_st   *p_config_data = ICF_NULL;
    icf_internal_refer_req_st *p_refer_req = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    if(ICF_FAILURE ==icf_dbm_get_module_glb_data(
                     p_pdb->p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t *)&p_config_data, p_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {    
        if(ICF_INVALID_CALL_ID != p_pdb->p_xconf_ctx->conference_id)
        {
            ICF_MEMGET(p_pdb->p_glb_pdb, 
               sizeof(icf_internal_refer_req_st), 
               ICF_MEM_COMMON, 
               p_refer_req,
               ICF_RET_ON_FAILURE, 
               p_pdb->p_ecode, 
               ret_val)
     
             p_refer_req->refer_uri.addr_type = 0;
             p_refer_req->refer_uri.addr_val.str_len = 0;
             p_refer_req->replaces_call_id = ICF_INVALID_CALL_ID;
             p_refer_req->bitmask = 0;
             p_refer_req->replaces_call_id = 
                 p_pdb->p_xconf_ctx->conference_id;
             p_refer_req->bitmask |= 
                 ICF_REFER_REPLACE_CALL_ID_PRESENT;
             p_pdb->p_internal_msg->p_msg_data = 
                  (icf_uint8_t *)p_refer_req; 
             ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_02_AWT_ADD_PARTY_RESP)
             ICF_XCONF_SET_STATUS_2(p_pdb,
                     ICF_STATUS_INTERNAL_REFER_W_REP_REQ)
        }
        else
        {
             if (ICF_FAILURE == icf_xconf_send_xternal_conf_resp(p_pdb, 
                         (icf_boolean_t)ICF_SUCCESS))
             {
                 ret_val = ICF_FAILURE;
             }

             ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_04_CONFERENCE_CONNECTED)
        }    
        /*If subscription is required then send subscribe request
         *to SIC module and wait for subs resp else go to state
         *conference_connected*/
        if (ICF_TRUE == p_config_data->xconf_subs_reqd)
        {
             /*Send SUBSCRIBE request to SIC module*/
             ret_val = icf_xconf_send_sub_req(p_pdb);
        }

    }
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /* icf_xconf_01_conf_server_connected */


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_01_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
 
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
 
    /* Send Failure resp for the ADD_PARTY_REQ to PA with error code as 
       CONF_NOT_CONNECTED*/ 
    *(p_pdb->p_ecode) = ICF_CAUSE_CONF_NOT_CONNECTED_CANNOT_DO_ADD_PARTY;
    ret_val = icf_xconf_send_add_party_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE);
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
	return ret_val;
} /*icf_xconf_01_add_party_req*/

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_06_AWT_CALL_RESUME_RESP   
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_02_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
 
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
 
    /* Send Failure resp for the ADD_PARTY_REQ to PA with error code as 
       ADD_PARTY_RESP_ALREADY_PENDING*/ 
       *(p_pdb->p_ecode) = ICF_CAUSE_ADD_PARTY_RESP_ALREADY_PENDING;
    ret_val = icf_xconf_send_add_party_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE);

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
	return ret_val;
} /*icf_xconf_02_add_party_req*/

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)    
                                ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_03_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
 
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
 
    /* Send Failure resp for the ADD_PARTY_REQ to PA with error code as 
       DELETE_PARTY_RESP_ALREADY_PENDING */
    *(p_pdb->p_ecode) = ICF_CAUSE_DELETE_PARTY_RESP_ALREADY_PENDING;
    ret_val = icf_xconf_send_add_party_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE);
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
	return ret_val;
} /*icf_xconf_03_add_party_req*/


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_04_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_04_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_add_xconf_party_req_st  *p_add_party_req = ICF_NULL;
    icf_internal_refer_req_st   *p_refer_req = ICF_NULL;
    icf_call_ctx_st             add_party_call_ctx;
    icf_call_ctx_st             *p_add_party_call_ctx = &add_party_call_ctx;   
    
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
    p_add_party_req = (icf_add_xconf_party_req_st *)
                        &(p_pdb->p_glb_pdb->p_recv_msg->payload[0]);
    if(ICF_TRUE == p_pdb->p_xconf_ctx->incoming_conf_req)
    {
         /* send failure to PA with error code as 
            INCOMING_CONFERENCE_CANNOT_ADD_PARTY */
        *(p_pdb->p_ecode) = ICF_CAUSE_INC_CONFERENCE_CANNOT_DO_ADD_PARTY;
        ret_val = icf_xconf_send_add_party_resp(p_pdb,
	    (icf_boolean_t)ICF_FAILURE);
     }
     else if(p_add_party_req->add_party_id)
     { 
         if(ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                                    p_pdb->p_glb_pdb,
                                    p_add_party_req->add_party_id,
                                    &p_add_party_call_ctx,
                                    p_pdb->p_ecode))
         {
             /* for handling scenario when add party request comes 
                for party which has no call with host */
            *(p_pdb->p_ecode) = ICF_CAUSE_INVALID_CALL_ID;
            ret_val = icf_xconf_send_add_party_resp(p_pdb,
	                    (icf_boolean_t)ICF_FAILURE);
         }
         else
         {    

             ICF_MEMGET(p_pdb->p_glb_pdb, 
                 sizeof(icf_internal_refer_req_st), 
                 ICF_MEM_COMMON, 
                 p_refer_req,
                 ICF_RET_ON_FAILURE, 
                 p_pdb->p_ecode, 
                 ret_val)
         /* Send INTERNAL_REFER_REQ to CTA with the add_party_id which is 
            callid for existing call between host and the new party*/
             p_refer_req->refer_uri.addr_type = 0;
             p_refer_req->refer_uri.addr_val.str_len = 0;
             p_refer_req->replaces_call_id = ICF_INVALID_CALL_ID;
             p_refer_req->bitmask = 0;
             p_refer_req->replaces_call_id = p_add_party_req->add_party_id;
             p_refer_req->bitmask |= ICF_REFER_REPLACE_CALL_ID_PRESENT;
             p_pdb->p_internal_msg->p_msg_data = 
                  (icf_uint8_t *)p_refer_req; 
             ICF_XCONF_SET_STATUS_2(p_pdb, ICF_STATUS_INTERNAL_REFER_W_REP_REQ)
             ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_02_AWT_ADD_PARTY_RESP)
         }
     }    

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /*icf_xconf_04_add_party_req*/


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_05_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_05_AWT_CONFERENCE_TERMINATE
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_05_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /* send failure response for the add_party request by calling 
       icf_xconf_send_add_party_resp */
    *(p_pdb->p_ecode) = ICF_CAUSE_CONF_NOT_CONNECTED_CANNOT_DO_ADD_PARTY;
    ret_val = icf_xconf_send_add_party_resp(p_pdb, 
              (icf_boolean_t)ICF_FAILURE);
    
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /* icf_xconf_05_add_party_req */


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_add_party_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_ADD_PARTY_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_02_add_party_resp(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    /* variable to parse the internal_refer_resp payload coming in 
       internal message */
    icf_internal_refer_resp_st *p_internal_mesg_param = ICF_NULL;    

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   

    /* If success is received from CTA, the Send ADD_PARTY_RESP (Success) to 
     * Application, else send failure response */
    p_internal_mesg_param = (icf_internal_refer_resp_st *)
                       (p_pdb->p_internal_msg->p_msg_data); 	    
	    
     if(p_internal_mesg_param->result == ICF_SUCCESS)
      {
        /* In case of existing_call_id present in the initiate external
           conference request and now we have got REFER resp success for the
           add party therefore now we will send the external conference resp
           and no need to send add_party_resp to application in this case
           as it is not required because explicitly we do not have
           add_party_req */
        if(ICF_INVALID_CALL_ID != p_pdb->p_xconf_ctx->conference_id)
        {
           if (ICF_FAILURE == icf_xconf_send_xternal_conf_resp(p_pdb,
                       (icf_boolean_t)ICF_SUCCESS))
           {
               ret_val = ICF_FAILURE;
           }
           ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_04_CONFERENCE_CONNECTED)
           p_pdb->p_xconf_ctx->conference_id = ICF_INVALID_CALL_ID; 
        }  
        else
        {    
         /* if SUCCESS add_party resp came for add_party_req we need to
            do call resume request to SCM and will move to
            ICF_XCONF_06_AWT_CALL_RESUME_RESP state and there in 06 state
            we will send the success add_party_resp */
           if(ICF_CALL_STATUS_LOCAL_HELD == p_pdb->p_call_ctx->call_status)
           {   
              ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_06_AWT_CALL_RESUME_RESP)
              ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_RESUME_REQ)
           }
           else
           {
              if(ICF_FAILURE == icf_xconf_send_add_party_resp(p_pdb,
	               (icf_boolean_t)ICF_SUCCESS))
               {
                   ret_val = ICF_FAILURE;
               }
              /* Move to state ICF_XCONF_04_CONFERENCE_CONNECTED */ 	 
              ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_04_CONFERENCE_CONNECTED)
           }
        } 
      } 
     else
      {	    
        /*If refer failure comes then check for existing callid case 
          by conference id value population. If it is there then set 
          status 2 as  ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE  to send 
          CC trigger to clear the call. Then when cc sends call clear,
          We will be able to send xconf_resp failure in 02_call_released*/
        if(ICF_INVALID_CALL_ID != p_pdb->p_xconf_ctx->conference_id)
        {
           /* send trigger to CC for clearing the call */
           ICF_XCONF_SET_STATUS_2(p_pdb, ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE)
        }
        else
        {    
            *(p_pdb->p_ecode) = ICF_CAUSE_INTERNAL_REFER_FAILED;

            if(ICF_FAILURE == icf_xconf_send_add_party_resp(p_pdb,
	               (icf_boolean_t)ICF_FAILURE))
             {
                ret_val = ICF_FAILURE;
             }
            /* Move to state ICF_XCONF_04_CONFERENCE_CONNECTED */ 	 
            ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_04_CONFERENCE_CONNECTED)
        }    
      }

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /* icf_xconf_02_add_party_resp */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_06_call_resume_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_CALL_RESUME_RESP
*                 Event         ICF_XCONF_CALL_RESUME_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_06_call_resume_resp(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    if(ICF_FAILURE == icf_xconf_send_add_party_resp(p_pdb,
	               (icf_boolean_t)ICF_SUCCESS))
     {
        ret_val = ICF_FAILURE;
     }
    /* Move to state ICF_XCONF_04_CONFERENCE_CONNECTED */ 	 
    ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_04_CONFERENCE_CONNECTED)
    
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /* icf_xconf_06_call_resume_resp */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_DELETE_PARTY_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_01_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
 
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
 
    /* Send Failure resp for the DEL_PARTY_REQ to PA with error code as 
       CONF_NOT_CONNECTED*/ 
    *(p_pdb->p_ecode) = ICF_CAUSE_CONF_NOT_CONNECTED_CANNOT_DO_DEL_PARTY;
    ret_val = icf_xconf_send_del_party_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE);
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
	return ret_val;
} /*icf_xconf_01_del_party_req*/


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_02_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    
    /* Send Failure response to PA for the delete party request with error 
       code as ADD_PARTY_RESP_ALREADY_PENDING */ 
    *(p_pdb->p_ecode) = ICF_CAUSE_ADD_PARTY_RESP_ALREADY_PENDING;
    ret_val= icf_xconf_send_del_party_resp(p_pdb, 
                         (icf_boolean_t)ICF_FAILURE);
 
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /*icf_xconf_02_del_party_req*/


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_03_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    /* Reset the flag to fetct del uri from the recv payload
       for this failure delete party resp */
    p_pdb->p_xconf_ctx->del_uri_from_xconf_ctx =
        ICF_FALSE;
    
    /* Send Failure response to PA for the delete party request with error 
       code as DELETE_PARTY_RESP_ALREADY_PENDING */ 
    *(p_pdb->p_ecode) = ICF_CAUSE_DELETE_PARTY_RESP_ALREADY_PENDING;
    ret_val= icf_xconf_send_del_party_resp(p_pdb, 
                           (icf_boolean_t)ICF_FAILURE);

    /* Revert the reset of flag to fetch del uri from the xconf ctx 
       in icf_xconf_03_del_party_resp function when pending delete 
       party resp comes */
    p_pdb->p_xconf_ctx->del_uri_from_xconf_ctx =
        ICF_TRUE;
 
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /*icf_xconf_03_del_party_req*/


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_06_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_06_AWT_CALL_RESUME_RESP
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_06_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    /* Send Failure response to PA for the delete party request with error 
       code as ADD_PARTY_RESP_ALREADY_PENDING */ 
      *(p_pdb->p_ecode) = ICF_CAUSE_ADD_PARTY_RESP_ALREADY_PENDING;
    ret_val= icf_xconf_send_del_party_resp(p_pdb, 
                         (icf_boolean_t)ICF_FAILURE);
 
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /*icf_xconf_06_del_party_req*/


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_04_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_04_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_internal_refer_req_st *p_refer_req = ICF_NULL;
    icf_delete_xconf_party_req_st   *p_del_party_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
    p_del_party_req = (icf_delete_xconf_party_req_st  *)
                    &(p_pdb->p_glb_pdb->p_recv_msg->payload[0]);
   
    if(ICF_TRUE == p_pdb->p_xconf_ctx->incoming_conf_req)
    {
        /* send failure to PA for DEL party req with error code as
           INCOMING_CONF_CANNOT_DEL_PARTY */
       *(p_pdb->p_ecode) = ICF_CAUSE_INC_CONFERENCE_CANNOT_DO_DEL_PARTY; 
        ret_val = icf_xconf_send_del_party_resp(p_pdb,
                   (icf_boolean_t)ICF_FAILURE);
    }
    else
    {  
        /* Send INTERNAL_REFER_REQ to CTU */
        ICF_MEMGET(p_pdb->p_glb_pdb, 
               sizeof(icf_internal_refer_req_st), 
               ICF_MEM_COMMON, 
               p_refer_req,
               ICF_RET_ON_FAILURE, 
               p_pdb->p_ecode, 
               ret_val)
         p_refer_req->refer_uri.addr_type = 0;
         p_refer_req->refer_uri.addr_val.str_len = 0;
         p_refer_req->replaces_call_id = ICF_INVALID_CALL_ID;
         p_refer_req->bitmask = 0;
         p_refer_req->refer_uri = p_del_party_req->del_party_uri;
	 p_refer_req->bitmask = (icf_uint8_t) (p_refer_req->bitmask | ICF_REFER_METHOD_BYE);
         p_refer_req->bitmask = (icf_uint8_t) (p_refer_req->bitmask | ICF_REFER_URI_PRESENT);
         p_pdb->p_internal_msg->p_msg_data =
                    (icf_uint8_t *)p_refer_req;
        
        ICF_XCONF_SET_STATUS_2(p_pdb, ICF_STATUS_INTERNAL_REFER_REQ)
        ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_03_AWT_DEL_PARTY_RESP)

        /*Before sending the control to CTU via SCM store the delete
          party uri in xconf ctx for using the same in response */
        p_pdb->p_xconf_ctx->del_uri_from_xconf_ctx = ICF_TRUE;
        p_pdb->p_xconf_ctx->delete_party_uri =
           p_del_party_req->del_party_uri;

    }
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
}   

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_05_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_05_AWT_CONFERENCE_TERMINATE
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_05_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /* send failure response for the add_party request by calling 
       icf_xconf_send_add_party_resp */
    *(p_pdb->p_ecode) = ICF_CAUSE_CONF_TERMINATE_CANNOT_DO_DEL_PARTY;
    ret_val = icf_xconf_send_del_party_resp(p_pdb, 
              (icf_boolean_t)ICF_FAILURE);
    
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /* icf_xconf_05_del_party_req */


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_del_party_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_DEL_PARTY_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_03_del_party_resp(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    /* local variable for parsing the internal message payload */
    icf_internal_refer_resp_st *p_internal_mesg_param = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    /* If success is received from CTA, the Send DEL_PARTY_RESP (Success) to 
     * Application, else send failure response */
    p_internal_mesg_param = (icf_internal_refer_resp_st *)
                       (p_pdb->p_internal_msg->p_msg_data); 	    
	    
    if(p_internal_mesg_param->result == ICF_SUCCESS)
    { 
         ret_val = icf_xconf_send_del_party_resp(p_pdb,
	    (icf_boolean_t)ICF_SUCCESS);
    } 
    else
    {
         *(p_pdb->p_ecode) = ICF_CAUSE_INTERNAL_REFER_FAILED;
         ret_val= icf_xconf_send_del_party_resp(p_pdb,
	    (icf_boolean_t)ICF_FAILURE);
     } 	
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_04_CONFERENCE_CONNECTED)
    
    /* After sending delete_party_resp we will reset this flag as
       this uri in xconf ctx is no more required for response */
    p_pdb->p_xconf_ctx->del_uri_from_xconf_ctx =
        ICF_FALSE;

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /* icf_xconf_03_del_party_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_01_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    /* send trigger to CC for clearing the call */

    ICF_XCONF_SET_STATUS_2(p_pdb, ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE)    
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP)
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /* icf_xconf_01_release_conf_req */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_06_AWT_CALL_RESUME_RESP  
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_xx_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /*Check if subscription is done then send unsubscribe request
      to SIC */
    if(ICF_TRUE == p_pdb->p_xconf_ctx->subs_done)
    {
        ret_val = icf_xconf_send_unsubscribe_req_to_sic(p_pdb);
    }  
    /*scenario when add party is going on and release conference
      request comes from PA so we will send ADD party failure 
      response and further move to state 05 for sending release
      conference response */
    if(ICF_TRUE != p_pdb->p_xconf_ctx->add_xconf_party_resp_sent)
    {   
        *(p_pdb->p_ecode) = ICF_CAUSE_CALL_RELEASED;
        if(ICF_FAILURE == icf_xconf_send_add_party_resp(p_pdb, 
              (icf_boolean_t)ICF_FAILURE))
        {
           ret_val = ICF_FAILURE;
        }
    }    

    /* send trigger to CC for clearing the call*/
    ICF_XCONF_SET_STATUS_2(p_pdb, ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE)
    ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP)

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /* icf_xconf_xx_release_conf_req */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State         ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_03_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /*Check if subscription is done then send unsubscribe request
      to SIC */
    if(ICF_TRUE == p_pdb->p_xconf_ctx->subs_done)
    {
        ret_val = icf_xconf_send_unsubscribe_req_to_sic(p_pdb);
    }  
    /*scenario when delete party is going on and release conference
      request comes from PA so we will send DEL party failure 
      response and further move to state 05 for sending release
      conference response */
    *(p_pdb->p_ecode) = ICF_CAUSE_CALL_RELEASED;
    if(ICF_FAILURE == icf_xconf_send_del_party_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE))
     {
        ret_val = ICF_FAILURE;
     }

    /* send trigger to CC for clearing the call*/
    ICF_XCONF_SET_STATUS_2(p_pdb, ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE)
    ICF_XCONF_SET_CURR_STATE(p_pdb, ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP)

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
} /* icf_xconf_03_release_conf_req */


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_04_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_04_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
    if(ICF_TRUE == p_pdb->p_xconf_ctx->incoming_conf_req)
    { 
       /*send the RELEASE_CONF_RESP(success) to PA*/
       *(p_pdb->p_ecode) = ICF_CAUSE_INC_CONFERENCE_CANNOT_DO_RELEASE;
       ret_val = icf_xconf_send_pa_release_conf_resp(p_pdb,
                 (icf_boolean_t)ICF_FAILURE);
    }
    else if(ICF_TRUE == p_pdb->p_xconf_ctx->subs_done)
    {
             ret_val = icf_xconf_send_unsubscribe_req_to_sic(p_pdb);
    }  

    /* send trigger to CC for clearing the call */
    ICF_XCONF_SET_STATUS_2(p_pdb, ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE)    
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP)
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;

}


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_notify_for_sub
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                               ICF_XCONF_04_CONFERENCE_CONNECTED
*                               ICF_XCONF_06_AWT_CALL_RESUME_RESP  
*                 Event         ICF_XCONF_NOTIFY_FOR_SUBS
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_xx_notify_for_sub(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_notify_ind_st      *p_notify_ind = ICF_NULL; 

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    /* This mesg is prepared in SIC module icf_sic_send_notify_msg_to_app
       Here we just typecast and set the subs_done flag and call 
       icf_adm_process_mesg for sending notify_ind to PA. */
    p_notify_ind = (icf_notify_ind_st *)(((icf_msg_st*)
                   (p_pdb->p_internal_msg->p_msg_data))->payload);
    
    /* Send notify_ind to PA */
    ret_val = icf_xconf_send_pa_notify_ind(p_pdb); 	    

     if(p_notify_ind->subs_state == ICF_SUBS_STATE_TERMINATED)
     {
       /* Reset the subscription_done flag */
       p_pdb->p_xconf_ctx->subs_done = ICF_FALSE;
     }	     

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
}

    
/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_subs_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                               ICF_XCONF_04_CONFERENCE_CONNECTED
*                               ICF_XCONF_06_AWT_CALL_RESUME_RESP  
*                 Event         ICF_XCONF_SUBS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_xx_subs_resp(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_subscription_status_ind_st *p_subs_resp = ICF_NULL;
    icf_msg_st      *p_msg = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
    /*SIC module send icf_msg_st in internal msg data
     *so that XCONF can send this as it is to application*/
    p_msg = (icf_msg_st*)p_pdb->p_internal_msg->p_msg_data;

    p_subs_resp = (icf_subscription_status_ind_st *)
            &(p_msg->payload[0]);

    p_pdb->p_internal_msg->msg_hdr.msg_id =
    ICF_SUBSCRIPTION_STATUS_IND;

    /* The mesg is prepared in SIC module -  function 
       icf_sic_send_subs_state_to_app and here we type cast to set
       the subs_done flag in xconf ctx and then call adm_process_mesg
       function for sending subscribe_status_ind API to PA */
    ret_val = icf_adm_process_mesg(p_pdb->p_internal_msg);

     if((p_subs_resp->status != ICF_SUBS_ACCEPTED) && 
       (p_subs_resp->status != ICF_SUBS_SUBSCRIBED))
     {
         /* Reset the subscription_done flag */
         p_pdb->p_xconf_ctx->subs_done = ICF_FALSE;
     }	     

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    
    return ret_val;
}

   
/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_05_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_05_AWT_CONFERENCE_TERMINATE
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_05_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
   /*Move to IDLE state */
   ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_00_IDLE)
  
    /*send the RELEASE_CONF_RESP(success) to PA*/
    ret_val = icf_xconf_send_pa_release_conf_resp(p_pdb,
        (icf_boolean_t)ICF_SUCCESS);

    /* put handing for clearing the Xconf ctx */
     ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 states        ICF_XCONF_04_CONFERENCE_CONNECTED
*                               ICF_XCONF_06_AWT_CALL_RESUME_RESP  
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_xx_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /* Send unsubscribe_req to SIC, if subscription was done*/      
    if(ICF_TRUE == p_pdb->p_xconf_ctx->subs_done)
    {
       ret_val = icf_xconf_send_unsubscribe_req_to_sic(p_pdb);
    }  
    if(ICF_TRUE != p_pdb->p_xconf_ctx->incoming_conf_req)
    { 
      /*send the RELEASE_CONF_IND(success) to PA*/
      ret_val = icf_xconf_send_pa_release_conf_ind(p_pdb);
    }
    /*Move to IDLE state */
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_00_IDLE)
        
    /* put handing for clearing the Xconf ctx */
    ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_02_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /* Send unsubscribe_req to SIC, if subscription was done*/      
    if(ICF_TRUE == p_pdb->p_xconf_ctx->subs_done)
    {
       ret_val = icf_xconf_send_unsubscribe_req_to_sic(p_pdb);
    }  
    if(ICF_TRUE != p_pdb->p_xconf_ctx->incoming_conf_req)
    { 
        /*The below if condition will be executed when during */
        /*creating conference PA sends existing call-id but conference
          server sends 4xx for refer with replaces so we will clear call
          and now send failure create_xconf_resp */
        if(ICF_INVALID_CALL_ID != p_pdb->p_xconf_ctx->conference_id)
        {
          *(p_pdb->p_ecode) = ICF_CAUSE_INTERNAL_REFER_FAILED;
           if (ICF_FAILURE == icf_xconf_send_xternal_conf_resp(p_pdb,
                       (icf_boolean_t)ICF_FAILURE))
           {
               ret_val = ICF_FAILURE;
           }
           p_pdb->p_xconf_ctx->conference_id = ICF_INVALID_CALL_ID; 
        }  
        else
        {
          *(p_pdb->p_ecode) = ICF_CAUSE_CALL_RELEASED;
          if(ICF_FAILURE == icf_xconf_send_add_party_resp(p_pdb, 
              (icf_boolean_t)ICF_FAILURE))
          {
              ret_val = ICF_FAILURE;
          }
          else
          {    
             /*send the RELEASE_CONF_IND(success) to PA*/
             ret_val = icf_xconf_send_pa_release_conf_ind(p_pdb);
          }   
        }
    }
    /*Move to IDLE state */
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_00_IDLE)
        
    /* put handing for clearing the Xconf ctx */
    ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}    

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 state         ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_03_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /* Send unsubscribe_req to SIC, if subscription was done*/      
    if(ICF_TRUE == p_pdb->p_xconf_ctx->subs_done)
    {
       ret_val = icf_xconf_send_unsubscribe_req_to_sic(p_pdb);
    }  
    if(ICF_TRUE != p_pdb->p_xconf_ctx->incoming_conf_req)
    { 
      *(p_pdb->p_ecode) = ICF_CAUSE_CONF_TERMINATE_CANNOT_DO_DEL_PARTY;
      if(ICF_FAILURE == icf_xconf_send_del_party_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE))
      {
          ret_val = ICF_FAILURE;
      }
      else
      {
         /*send the RELEASE_CONF_IND(success) to PA*/
         ret_val = icf_xconf_send_pa_release_conf_ind(p_pdb);
      }   
    }
    /*Move to IDLE state */
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_00_IDLE)
        
    /* put handing for clearing the Xconf ctx */
    ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}    

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_06_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_06_AWT_CALL_RESUME_RESP
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_06_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    /* Send unsubscribe_req to SIC, if subscription was done*/      
    if(ICF_TRUE == p_pdb->p_xconf_ctx->subs_done)
    {
       ret_val = icf_xconf_send_unsubscribe_req_to_sic(p_pdb);
    }  
    if(ICF_TRUE != p_pdb->p_xconf_ctx->incoming_conf_req)
    { 
       *(p_pdb->p_ecode) = ICF_CAUSE_CALL_RELEASED;
       if(ICF_FAILURE == icf_xconf_send_add_party_resp(p_pdb, 
            (icf_boolean_t)ICF_FAILURE))
       {
           ret_val = ICF_FAILURE;
       }
       else
       {    

          /*send the RELEASE_CONF_IND(success) to PA*/
          ret_val = icf_xconf_send_pa_release_conf_ind(p_pdb);
       }   
    }
    /*Move to IDLE state */
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_00_IDLE)
        
    /* put handing for clearing the Xconf ctx */
    ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}    




/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_01_call_released(
        INOUT  icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
   
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
  
    *(p_pdb->p_ecode) = ICF_CAUSE_CALL_RELEASED;   
     /* Send failure response to PA for initiate_conf */        
    ret_val = icf_xconf_send_xternal_conf_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE);

    /* put handling for clearing xconf ctx */
    ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

    
/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                               ICF_XCONF_04_CONFERENCE_CONNECTED
*                               ICF_XCONF_06_AWT_CALL_RESUME_RESP  
*                 Event         ICF_XCONF_FORCEFUL_CALL_CLEAR
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_xx_forceful_call_clear(
        INOUT  icf_xconf_pdb_st *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
    /*Send forceful clear to SIC*/
    if((ICF_TRUE == p_pdb->p_xconf_ctx->subs_done) && 
       ((p_pdb->p_xconf_ctx->state == ICF_XCONF_02_AWT_ADD_PARTY_RESP) ||
        (p_pdb->p_xconf_ctx->state == ICF_XCONF_03_AWT_DEL_PARTY_RESP)))
    {
        ret_val = icf_xconf_send_forceful_clear_to_sic(p_pdb);
    } 
    if((ICF_TRUE != p_pdb->p_xconf_ctx->incoming_conf_req))
    { 

        /*send the RELEASE_CONF_IND(success) to PA*/
         ret_val = icf_xconf_send_pa_release_conf_ind(p_pdb);
                  
    }
    /*Move to IDLE state */
    ICF_XCONF_SET_CURR_STATE(p_pdb, 
                     ICF_XCONF_00_IDLE)
       
    /* put handing for clearing the Xconf ctx */
    ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)  

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

    
/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_FORCEFUL_CALL_CLEAR
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_01_forceful_call_clear(
        INOUT  icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    /* Send failure response to PA for initiate_conf */
    *(p_pdb->p_ecode) = ICF_CAUSE_CALL_RELEASED;     
    ret_val = icf_xconf_send_xternal_conf_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE);

    /* put handling for clearing xconf ctx */
    ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

    
/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_00_incoming_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_00_IDLE
*                 Event         ICF_XCONF_INCOMING_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_00_incoming_conf_req(
        INOUT  icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_config_data_st   *p_config_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    ret_val = icf_dbm_get_module_glb_data(
                     p_pdb->p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t *)&p_config_data, p_pdb->p_ecode);
    
    /*If subscription is required then send subscribe request
      Send SUBSCRIBE request to SIC module*/
    if(ICF_TRUE == p_config_data->xconf_subs_reqd)     
     {
          if (ICF_FAILURE == icf_xconf_send_sub_req(p_pdb))
          {
             /* set service ended normal status */
             ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
          }
          else
          {
              p_pdb->p_xconf_ctx->subs_done = ICF_TRUE;
              p_pdb->p_xconf_ctx->incoming_conf_req = ICF_TRUE;
              ICF_XCONF_SET_CURR_STATE(p_pdb, 
                         ICF_XCONF_04_CONFERENCE_CONNECTED)
          }
     }
     else
     {
             /* set service ended normal status */
             ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            /*Clear the XCONF ctx */
     }
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

    
/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_xconf_unexpected_event(
        INOUT   icf_xconf_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    p_pdb=p_pdb;
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    /* No handling as unexpected event */

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /* icf_xconf_unexpected_event */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_00_subs_resp
*
* DESCRIPTION   : Handler for ICF_XCONF_SUBS_RESP in ICF_XCONF_))_IDLE state
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_xconf_00_subs_resp(
        INOUT   icf_xconf_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)

    ret_val = 
        icf_xconf_send_display_ind_on_subs_failure(p_pdb);

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)

    return ret_val;
} /* icf_xconf_unexpected_event */
#endif
