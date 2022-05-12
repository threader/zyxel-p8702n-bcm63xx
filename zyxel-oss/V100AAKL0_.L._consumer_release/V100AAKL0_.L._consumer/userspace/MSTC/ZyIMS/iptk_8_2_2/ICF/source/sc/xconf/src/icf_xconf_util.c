/*****************************************************************************
*
* FILE NAME    : icf_xconf_util.c
*
* DESCRIPTION  : Contains the definitions of all Utility functions
*                of XCONF service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 5-Jul-2007    Abhishek Dhammawat CDD Rel7.0   Initial
* 20-sept-2007  Abhishek Dhammawat SPR 16541    icf_xconf_validate_conf_req
*                                               function is modified to
*                                               validate existing_call_id
*                                               field.
* 08-Apr-2008  Sumant Gupta    SPR:18355        Windows porting Merging
* 15-Apr-2008  Alok Tiwari     SPR:18364        Klocwork warning removal
* 05-Jul-2008  Amit Sharma     SPR 18714        Added function to send
*                                               display_ind to application
* 11-Jun-2009  Tarun Gupta     Rel 8.2          Modified 
*                                               icf_xconf_send_unsubscribe_req_to_sic
* 12-Sep-2009  Rajiv Kumar     SPR 19999        Merged CSR 1-7467002
*                                               icf_sic_subs_req_st variable
*                                               initialized for new Var added
* Copyright 2009,Aricent.
*****************************************************************************/

#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_xconf_types.h"
#include "icf_xconf_prototypes.h"
#include "icf_xconf_macro.h"
#include "icf_xconf_defs.h"
#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_sc_prototypes.h"

#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME    :   icf_xconf_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS: none
******************************************************************************/
icf_void_t icf_xconf_state_trace_string(
        IN    icf_uint8_t          state,
        INOUT icf_trace_data_st   *p_trace_data)
{

    /*Switch for trace_id*/
    switch(state)
    {
        case ICF_XCONF_00_IDLE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_00_IDLE");
            break;

        case ICF_XCONF_01_AWT_CONFERENCE_CONNECT:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                (icf_uint8_t *)"ICF_XCONF_01_AWT_CONFERENCE_CONNECT");
            break;

        case ICF_XCONF_02_AWT_ADD_PARTY_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                (icf_uint8_t *)"ICF_XCONF_02_AWT_ADD_PARTY_RESP");
            break;

        case ICF_XCONF_03_AWT_DEL_PARTY_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_03_AWT_DEL_PARTY_RESP");
            break;

        case ICF_XCONF_04_CONFERENCE_CONNECTED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_04_CONFERENCE_CONNECTED");
            break;

        case ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_05_AWT_CONFERENCE_TERMINATE");
            break;

       case ICF_XCONF_STATE_INVALID:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_STATE_INVALID");
            break;
    } /* switch(trace_id) */
}

/******************************************************************************
*
* FUNCTION NAME    :   icf_xconf_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:none
******************************************************************************/
icf_void_t icf_xconf_event_trace_string(
        IN    icf_uint8_t         event,
        INOUT icf_trace_data_st   *p_trace_data)
{
    switch(event)
    {
        case ICF_XCONF_INITIATE_CONF_REQ:
            icf_port_strcpy(p_trace_data->string_val[0].str,
            (icf_uint8_t *)"ICF_XCONF_INITIATE_CONF_REQ ");
            break;

        case ICF_XCONF_CONF_SERVER_CONNECTED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_CONF_SERVER_CONNECTED ");
            break;

        case ICF_XCONF_ADD_PARTY_REQ:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_ADD_PARTY_REQ ");
            break;

        case ICF_XCONF_ADD_PARTY_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_ADD_PARTY_RESP");
            break;

        case ICF_XCONF_DELETE_PARTY_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_DEL_PARTY_RESP");
            break;

        case ICF_XCONF_DELETE_PARTY_REQ:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_DEL_PARTY_REQ");
            break;

        case ICF_XCONF_SUBS_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_SUBS_RESP");
            break;

        case ICF_XCONF_CALL_RELEASED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_CALL_RELEASED");
            break;

        case ICF_XCONF_RELEASE_CONF_REQ:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_RELEASE_CONF_REQ");
            break;

        case ICF_XCONF_FORCEFUL_CALL_CLEAR:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_FORCEFUL_CALL_CLEAR");
            break;
        case ICF_XCONF_NOTIFY_FOR_SUBS:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_NOTIFY_FOR_SUBS");
            break;

        case ICF_XCONF_INCOMING_CONF_REQ:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_XCONF_INCOMING_CONF_REQ");
            break;

        default:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"Invalid Event");
            break;
    } /* switch(trace_id) */
} /* INOUT icf_trace_data_st   *... */


/******************************************************************************
*
* FUNCTION NAME:   icf_xconf_trace
*
* DESCRIPTION  : This function is used to define the trace data for 
*                XCONF service
*
*
******************************************************************************/
icf_void_t    icf_xconf_trace(
        INOUT icf_xconf_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
 
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    icf_port_memset((icf_void_t *)&trace_data,0,
                    ICF_PORT_SIZEOF(icf_trace_data_st));
    switch(trace_id)
    {
        case ICF_TRACE_XCONF_NEW_CALL_ID:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            if((ICF_NULL != p_pdb->p_xconf_ctx) &&
               (ICF_NULL != p_pdb->p_xconf_ctx->p_call_ctx))
            {    
                trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_xconf_ctx->p_call_ctx->call_id;
            } 
            break;
        case ICF_TRACE_XCONF_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if(ICF_NULL != p_pdb->p_xconf_ctx)
            {    
               trace_data.int_val[0] =
                 (icf_uint32_t)p_pdb->p_xconf_ctx->state;
               icf_xconf_state_trace_string(p_pdb->p_xconf_ctx->state, 
                                         &trace_data);
            }  
            break;
        case ICF_TRACE_XCONF_EVENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if(ICF_NULL != p_pdb->p_xconf_ctx) 
            {    
               trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_xconf_ctx->event;
               trace_data.int_val[1] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
               icf_xconf_event_trace_string(p_pdb->p_xconf_ctx->event, 
                                         &trace_data);
            }  
            break;
      case ICF_TRACE_CURR_RET_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_internal_msg->msg_hdr.status;
            break;
      default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    } /* switch(trace_id) */

    ICF_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data)
} /* icf_xconf_trace */


#endif
/*******************************************************************************
*
* FUNCTION NAME    :   icf_xconf_fill_pa_msg_hdr
*
* DESCRIPTION  : This function fills the icf_api_header_st to send message 
*                to PA
*                
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_void_t icf_xconf_fill_pa_msg_hdr(
        INOUT   icf_xconf_pdb_st *p_pdb,
        INOUT   icf_msg_st     *p_msg,
        IN      icf_api_id_t    api_id,
        IN      icf_uint16_t    api_len
        )
{
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;
    icf_return_t       ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    p_msg->hdr.api_id = api_id;

    p_msg->hdr.source_id = ICF_MODULE_ICF;  

    p_msg->hdr.destination_id = ICF_MODULE_PA;
   
    p_msg->hdr.api_length = api_len;
    
   if((ICF_NULL == p_pdb->p_call_ctx) ||
      (ICF_NULL == p_pdb->p_call_ctx->call_id) ||
      (ICF_INVALID_CALL_ID == p_pdb->p_call_ctx->call_id))
    {
       p_msg->hdr.call_id = p_pdb->p_glb_pdb->p_recv_msg->hdr.call_id;
       p_msg->hdr.app_id = p_pdb->p_glb_pdb->p_recv_msg->hdr.app_id;
    }
    else
    {
       p_msg->hdr.call_id = p_pdb->p_call_ctx->call_id;
       p_msg->hdr.app_id = p_pdb->p_call_ctx->app_id;
    }   

    ret_val = ret_val;
    ICF_FUNCTION_EXIT(p_glb_pdb)
} /* icf_xconf_fill_pa_msg_hdr */

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_add_party_resp
*
* DESCRIPTION   : send failure or success resp for add_party_resp to PA , 
*                 SUCCESS/FAILURE is identified by second argument
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_add_party_resp(
        INOUT  icf_xconf_pdb_st    *p_pdb,
        IN     icf_uint8_t         result)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_add_xconf_party_resp_st   *p_add_party_resp = ICF_NULL;
    icf_msg_st                    *p_msg = ICF_NULL;
    icf_uint16_t                  api_len = 0;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    api_len = sizeof(icf_api_header_st) +
              sizeof(icf_add_xconf_party_resp_st);
    
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_pdb->p_glb_pdb, 
               api_len, 
               ICF_MEM_COMMON, 
               p_msg,
               ICF_RET_ON_FAILURE, 
               p_pdb->p_ecode, 
               ret_val)
    
    p_add_party_resp = (icf_add_xconf_party_resp_st *)&(p_msg->payload[0]);
    
    /* Fill PA API hdr in p_msg */
    icf_xconf_fill_pa_msg_hdr(p_pdb, 
            p_msg, 
            (icf_api_id_t)(ICF_ADD_XCONF_PARTY_RESP),
            api_len);

    /* Fill the p_add_party_resp structure fields result,
     * ecode */
    if (ICF_FAILURE == result)
     {
       /*set the reult to failure (0) and set ecode */       
       p_add_party_resp->result = ICF_FAILURE;
       p_add_party_resp->error_cause = *(p_pdb->p_ecode);
       
     }
    else
     {
       /* set result as 1 */
       p_add_party_resp->result = ICF_SUCCESS;
     }       
        
    p_pdb->p_internal_msg->msg_hdr.msg_id =
        ICF_ADD_XCONF_PARTY_RESP;
    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE ==
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)

        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */
    else
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)
    } /* else */
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /* icf_xconf_send_add_party_resp */


/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_del_party_resp
*
* DESCRIPTION   : send failure or success resp for del_party_resp to PA , 
*                 SUCCESS/FAILURE is identified by second argument
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_del_party_resp(
        INOUT  icf_xconf_pdb_st    *p_pdb,
    IN      icf_uint8_t    result)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_delete_xconf_party_resp_st  *p_del_party_resp = ICF_NULL;
    icf_delete_xconf_party_req_st   *p_del_party_req = ICF_NULL;
    icf_msg_st                      *p_msg = ICF_NULL;
    icf_uint16_t                    api_len = 0;
    
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    api_len = sizeof(icf_api_header_st) +
    sizeof(icf_delete_xconf_party_resp_st);
    
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_pdb->p_glb_pdb, 
               api_len, 
               ICF_MEM_COMMON, 
               p_msg,
               ICF_RET_ON_FAILURE, 
               p_pdb->p_ecode, 
               ret_val)

    p_del_party_resp = (icf_delete_xconf_party_resp_st *)&(p_msg->payload[0]);
    
    if(ICF_TRUE == p_pdb->p_xconf_ctx->del_uri_from_xconf_ctx)
    {
        /* Flag in xconf ctx is set so fetch the del_party_uri from ctx
           as in success delete party scenario the recv payload does not 
           have the required uri to be filled in resp */
        if(p_pdb->p_xconf_ctx->delete_party_uri.addr_val.str_len)
        {
          icf_port_memcpy(&(p_del_party_resp->deleted_party_uri), 
              &(p_pdb->p_xconf_ctx->delete_party_uri),
              (icf_uint32_t)sizeof(icf_address_st));
        }
    }
    else
    {
        /*fetching the deleted party URI from the delete party request
        recevived from PA and populating the same in resp */
        p_del_party_req  = (icf_delete_xconf_party_req_st  *)
                         &(p_pdb->p_glb_pdb->p_recv_msg->payload[0]);
        if(p_del_party_req->del_party_uri.addr_val.str_len)
        {
          icf_port_memcpy(&(p_del_party_resp->deleted_party_uri), 
              &(p_del_party_req->del_party_uri),
              (icf_uint32_t)sizeof(icf_address_st));
        }
    }   

    /* Fill PA API hdr in p_msg */
    icf_xconf_fill_pa_msg_hdr(p_pdb,
            p_msg,
            (icf_api_id_t)(ICF_DELETE_XCONF_PARTY_RESP),
            api_len);
    /* Fill the p_del_party_resp structure fields result,
     * ecode */
    if (ICF_FAILURE == result)
    {
       /*set the reult to failure (0) and set ecode */       
       p_del_party_resp->result = ICF_FAILURE;
       p_del_party_resp->error_code = *(p_pdb->p_ecode);
    }
    else
    {
       /* set result as 1 */
       p_del_party_resp->result = ICF_SUCCESS;
    }        
        
    p_pdb->p_internal_msg->msg_hdr.msg_id =
        ICF_DELETE_XCONF_PARTY_RESP;
    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE ==
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)

        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */
    else
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)
    } /* else */
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /* icf_xconf_send_del_party_resp */

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_xternal_conf_resp
*
* DESCRIPTION   : send failure or success resp for initiate_conf_resp to PA, 
*                 SUCCESS/FAILURE is identified by second argument
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*****************************************************************************/
icf_return_t icf_xconf_send_xternal_conf_resp(
            INOUT  icf_xconf_pdb_st    *p_pdb,
            IN     icf_uint8_t         result)
{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_create_ext_xconf_resp_st      *p_xconf_init_resp = ICF_NULL;
    icf_msg_st                        *p_msg = ICF_NULL;
    icf_uint16_t                      api_len = 0;
   
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
    ICF_PORT_SIZEOF(icf_create_ext_xconf_resp_st);
    
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_pdb->p_glb_pdb, 
               api_len, 
               ICF_MEM_COMMON, 
               p_msg,
               ICF_RET_ON_FAILURE, 
               p_pdb->p_ecode, 
               ret_val)

    /* Fill PA API hdr in p_msg */
    icf_xconf_fill_pa_msg_hdr(p_pdb,
            p_msg,
            (icf_api_id_t)(ICF_CREATE_XCONF_RESP),
            api_len);
    /* Fill the p_xconf_init_resp structure fields result,
     * ecode */
    p_xconf_init_resp = (icf_create_ext_xconf_resp_st *) 
                           (p_msg->payload);
    if (ICF_FAILURE == result)
    {
       /*set the result to failure (0) and set ecode */      
       p_xconf_init_resp->result = ICF_FAILURE;
       p_xconf_init_resp->error_cause = *(p_pdb->p_ecode);
    }
    else
    {
       /* set result as 1 */
       p_xconf_init_resp->result = ICF_SUCCESS;
    }        
        
    p_pdb->p_internal_msg->msg_hdr.msg_id =
        ICF_CREATE_XCONF_RESP;
    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE ==
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)

        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */
    else
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /* icf_xconf_send_xternal_conf_resp */

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_pa_release_conf_resp
*
* DESCRIPTION   :  
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_pa_release_conf_resp(
                INOUT  icf_xconf_pdb_st    *p_pdb,
                IN     icf_uint8_t    result)

{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_release_xconf_resp_st         *p_release_xconf_resp = ICF_NULL;
    icf_msg_st                        *p_msg = ICF_NULL;
    icf_uint16_t                      api_len = 0;
   
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    api_len = sizeof(icf_api_header_st) +
    sizeof(icf_release_xconf_resp_st);
    
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_pdb->p_glb_pdb, 
               api_len, 
               ICF_MEM_COMMON, 
               p_msg,
               ICF_RET_ON_FAILURE, 
               p_pdb->p_ecode, 
               ret_val)
    p_release_xconf_resp = (icf_release_xconf_resp_st *) 
                           &(p_msg->payload[0]);

    /* Fill PA API hdr in p_msg */
    icf_xconf_fill_pa_msg_hdr(p_pdb,
            p_msg,
            (icf_api_id_t)(ICF_RELEASE_XCONF_RESP),
            api_len);
    /* Fill the p_release_xconf_resp structure fields result,
     * ecode */
    if (ICF_FAILURE == result)
    {
       /*set the result to failure (0) and set ecode */      
       p_release_xconf_resp->result = ICF_FAILURE;
       p_release_xconf_resp->error_code = *(p_pdb->p_ecode);
    }
    else
    {
       /* set result as 1 */
       p_release_xconf_resp->result = ICF_SUCCESS;
    }        
        
    p_pdb->p_internal_msg->msg_hdr.msg_id =
        (icf_uint16_t)ICF_RELEASE_XCONF_RESP;
    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE ==
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)

        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */
    else
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;

} /* icf_xconf_send_pa_release_conf_resp */

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_fill_call_ctx
*
* DESCRIPTION   : Fill/Prepare the call ctx block from initiate_conference_request
*                 received from PA.
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_fill_call_ctx(
        INOUT  icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_create_ext_xconf_req_st  *p_xt_conf_req = ICF_NULL;
    icf_call_ctx_st              *p_call_ctx = ICF_NULL;
    icf_line_data_st             *p_line_data = ICF_NULL;
    icf_uint16_t                 api_len = 0; 
    icf_error_t                  ecode = ICF_ERROR_UNDEFINED;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
  
    p_call_ctx = p_pdb->p_call_ctx;

    /*Copy remote party address as conference_uri received, which is 
      address of conference server */
    p_xt_conf_req = (icf_create_ext_xconf_req_st *)
                      &(p_pdb->p_glb_pdb->p_recv_msg->payload[0]);

    if(p_xt_conf_req->conference_uri.addr_val.str_len)
    {
        icf_port_memcpy(&(p_call_ctx->remote_party_addr), 
            &(p_xt_conf_req->conference_uri),
            (icf_uint32_t)sizeof(icf_address_st));
    }    
    else
    {
        /*In case the conference uri is absent in the create
          xconf request then take the conference uri set in 
          line_data which was fetched according to 
          set_service_params_req API*/
       if(ICF_FAILURE == 
             icf_dbm_fetch_line_cntxt_blk(p_pdb->p_glb_pdb, 
                                    p_xt_conf_req->line_id,
                       (icf_line_data_st **)(&p_line_data),
                                            p_pdb->p_ecode))
       {
           ret_val = ICF_FAILURE;
       }
       else
       {
           icf_port_memcpy(&(p_call_ctx->remote_party_addr), 
            &(p_line_data->conf_server_uri),
            (icf_uint32_t)sizeof(icf_address_st));
       }
    }
   if(p_xt_conf_req->bitmask & 
           ICF_CREATE_XCONF_CALLING_PARTY_ADDR_PRESENT)
   {   
      icf_port_memcpy(&(p_call_ctx->local_address), 
            &(p_xt_conf_req->calling_party_address),
            (icf_uint32_t)sizeof(icf_address_st));
   }
   if(p_xt_conf_req->bitmask & 
           ICF_CREATE_XCONF_CALLING_PARTY_NAME_PRESENT)
   {    
      icf_port_memcpy(&(p_call_ctx->local_name), 
            &(p_xt_conf_req->calling_party_name),
            (icf_uint32_t)sizeof(icf_string_st));
   }  

   /* Copy line_id received in init_conf_req */
   p_call_ctx->line_id = p_xt_conf_req->line_id;
   p_call_ctx->call_id = p_pdb->p_glb_pdb->p_recv_msg->hdr.call_id; 
   p_call_ctx->app_id = p_pdb->p_glb_pdb->p_recv_msg->hdr.app_id; 

   /*Copy preferred_media_profile which is received in init_conference_req*/
   
   if(ICF_NULL != p_xt_conf_req->p_pref_media_profile)
   {
        icf_list_st     *p_list_1 = ICF_NULL;
        icf_stream_capab_st *p_stream_capab = ICF_NULL;
        api_len = sizeof(icf_stream_capab_st);
    
        p_list_1 = p_xt_conf_req->p_pref_media_profile;
        while (ICF_NULL != p_list_1)
        {
            ICF_MEMGET(p_pdb->p_glb_pdb, 
                       api_len,
                       ICF_MEM_COMMON, 
                       p_stream_capab,
                       ICF_RET_ON_FAILURE, 
                       p_pdb->p_ecode,
                       ret_val)
            /*klocwork warning removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_pdb->p_glb_pdb,p_stream_capab);

            icf_port_memcpy(p_stream_capab,
                    p_list_1->p_data,
                    (icf_uint32_t)sizeof(icf_stream_capab_st));
            p_stream_capab->p_codec_info_list = ICF_NULL;
            p_stream_capab->p_encoded_attributes = ICF_NULL;
                        
            /*Start : Addition for per line */        
            if ( ICF_NULL==((icf_stream_capab_st*)
                            (p_list_1->p_data))->p_codec_info_list) 
             {
                if (ICF_FAILURE == 
                      icf_dbm_fetch_line_cntxt_blk(p_pdb->p_glb_pdb, 
                                                p_call_ctx->line_id,
                                 (icf_line_data_st **)(&p_line_data),
                                                     p_pdb->p_ecode))
                 {
                      ret_val = ICF_FAILURE;
                      break;
                 }
                if(p_stream_capab->stream_type == ICF_STREAM_TYPE_AUDIO)
                 {
                     if(p_line_data->p_codec_attributes_audio != ICF_NULL)
                      {
                            ((icf_stream_capab_st*)
                             (p_list_1->p_data))->p_codec_info_list =
                                          p_line_data->p_codec_attributes_audio;
                      }
                  }
                  else if(p_stream_capab->stream_type == ICF_STREAM_TYPE_VIDEO)
                  {
                      if(p_line_data->p_codec_attributes_video != ICF_NULL)
                       {
                           ((icf_stream_capab_st*)
                            (p_list_1->p_data))->p_codec_info_list =
                             p_line_data->p_codec_attributes_video;
                        }
                   }  
                }/* End Added for per line */  

                ret_val = icf_cmn_clone_codec_attrib_list(
                           p_pdb->p_glb_pdb,
                           &(p_stream_capab->p_codec_info_list),
                           ((icf_stream_capab_st*)
                           (p_list_1->p_data))->p_codec_info_list);

                if (ICF_FAILURE == ret_val)
                {
                    break;
                }

                if (((icf_stream_capab_st*)
                    (p_list_1->p_data))->p_encoded_attributes != NULL)
                 {
                      /*This function will be making deep copy of list of type 
                        icf_stream_list_st.*/
                       ret_val=icf_cmn_copy_string_list(
                                   p_pdb->p_glb_pdb,
                                   &(p_stream_capab->p_encoded_attributes),
                                   ((icf_stream_capab_st*)
                                    (p_list_1->p_data))->p_encoded_attributes,
                                   p_pdb->p_ecode);
                       if (ICF_FAILURE == ret_val)
                       {
                            break;
                       }
                  }

                  ret_val = icf_cmn_add_node_to_list(p_pdb->p_glb_pdb,
                                    &(p_pdb->p_call_ctx->p_pref_media_profile),
                                      p_stream_capab);

                  if (ICF_FAILURE == ret_val)
                  {
                       break;
                  }
                  p_list_1 = p_list_1->p_next;
          }/* End of While */
          if((ICF_FAILURE == ret_val) &&
             (ICF_NULL !=  p_stream_capab)) 
          {
              if(ICF_NULL != p_stream_capab->p_encoded_attributes)
               {
                  icf_cmn_delete_list(p_pdb->p_glb_pdb,
                    &(p_stream_capab->p_encoded_attributes),
                                            ICF_MEM_COMMON);
               }
              if(ICF_NULL !=  p_stream_capab->p_codec_info_list)
               {
                  icf_cmn_delete_codec_attrib_list(p_pdb->p_glb_pdb,
                               &(p_stream_capab->p_codec_info_list),
                                                           &ecode);
               }
               
              ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_stream_capab, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)
              ret_val = ICF_FAILURE;      
          }        
    }/* End of If for p_preferred_media_profile not null */
    else
    {
        p_call_ctx->p_pref_media_profile = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_clear_call_ctx
*
* DESCRIPTION   : Free the allocated ctx
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_clear_call_ctx(
        INOUT  icf_xconf_pdb_st    *p_pdb,
        INOUT  icf_call_ctx_st     *p_call_ctx)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_error_t     ecode = 0;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
   /*Deallocate call context*/
    ret_val = icf_dbm_dealloc_call_cntxt_blk(p_pdb->p_glb_pdb,
                                             p_call_ctx->call_id,
                                             &ecode);

    ret_val = icf_cmn_free_icf_stream_capab_list(p_pdb->p_glb_pdb,
                         &(p_pdb->p_call_ctx->p_pref_media_profile));                          

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_pa_notify_ind
*
* DESCRIPTION   : This function sends notify_ind to PA.
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_pa_notify_ind(
        INOUT  icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
   
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NOTIFY_IND;
    
    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE == icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}
   
/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_pa_release_conf_ind
*
* DESCRIPTION   :  
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_pa_release_conf_ind(
        INOUT  icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t                             ret_val = ICF_SUCCESS;
    /* API for the release ind */
    icf_xconf_release_ind_st   *p_release_conf_ind = ICF_NULL;
    icf_msg_st                               *p_msg = ICF_NULL;
    icf_uint16_t                             api_len = 0;
    /* error code */
    icf_error_t                              *p_ecode = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    api_len = sizeof(icf_api_header_st) +
                 sizeof(icf_xconf_release_ind_st);
    
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_pdb->p_glb_pdb, 
               api_len, 
               ICF_MEM_COMMON, 
               p_msg,
               ICF_RET_ON_FAILURE, 
               p_ecode, 
               ret_val)
    p_release_conf_ind = (icf_xconf_release_ind_st  *)
                           &(p_msg->payload[0]);

    p_release_conf_ind->call_clear_reason = 
         p_pdb->p_call_ctx->release_cause;


    /* Fill PA API hdr in p_msg */
    icf_xconf_fill_pa_msg_hdr(p_pdb,
            p_msg,
            (icf_api_id_t)(ICF_RELEASE_XCONF_IND),
            api_len);
    
    p_pdb->p_internal_msg->msg_hdr.msg_id =
               ICF_RELEASE_XCONF_IND;

    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;

    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE ==
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_ecode, 
                    ret_val)

        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */
    else
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
} /* icf_xconf_send_pa_release_conf_ind */

/*****************************************************************************
*
* FUNCTION NAME:   icf_xconf_alloc_n_fill_serv_ctx
*
* DESCRIPTION  : This function is used to allocate new service context for
*                new call and fill all the information
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_alloc_n_fill_serv_ctx(
        INOUT   icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    /* pointer to call context */
    icf_call_ctx_st   *p_call_ctx = ICF_NULL;
    /* error code */
    icf_error_t       *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Copy new call context pointer which address of the
     * party for new call*/
    p_call_ctx = p_pdb->p_call_ctx;
   
    /*Call DBM function to allocate new service context*/
    ret_val = icf_dbm_get_srvc_cntxt_blk(p_glb_pdb, 
                      p_call_ctx->call_id,
                      (icf_service_ctx_st **)&(p_call_ctx->p_service_ctx),
                      p_ecode);

    /*If service context is allocate successfully*/
    if (ICF_SUCCESS == ret_val)
    {
        /*Allocate the call context pointer of which it is service ctx*/
        ((icf_service_ctx_st *)p_call_ctx->p_service_ctx)->p_call_ctx =
            p_call_ctx;

        /*Activate the XCONF service in this service context*/
        ((icf_service_ctx_st *)p_call_ctx->p_service_ctx)
            ->active_service_list.active_service_status
            |= ICF_SERVICE_XCONF;

        p_pdb->p_service_ctx =(icf_service_ctx_st *)p_call_ctx->p_service_ctx; 
    } /* if (ICF_SUCCESS == ret_val) */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_xconf_alloc_n_fill_serv_ctx */

/*****************************************************************************
*
* FUNCTION NAME:   icf_xconf_validate_conf_req
*
* DESCRIPTION  : This function is used to validate that if conference_uri is 
*                 present in recvd API then use it else check for same in line_data
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_validate_conf_req(
        INOUT   icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t                  ret_val = ICF_SUCCESS;
    icf_glb_pdb_st                *p_glb_pdb = ICF_NULL;
    icf_create_ext_xconf_req_st   *p_xt_conf_req = ICF_NULL;
    icf_line_data_st              *p_line_data = ICF_NULL;
    icf_call_ctx_st              *p_add_party_call_ctx = ICF_NULL;   
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* Fetch initiate_conf_req data from the recvd API */
    p_xt_conf_req = (icf_create_ext_xconf_req_st *)
                    &(p_pdb->p_glb_pdb->p_recv_msg->payload[0]);

    /* Check in p_xt_conf_req if conference_uri is populated 
     * Use line id present in initiate_conference_req API and
     * fetch the line data by calling icf_dbm_fetch_line_cntxt_blk
     * and then from line_data reterieve the conference uri by using
     * new field conference_uri in icf_line_data_st */
    
    if(p_xt_conf_req->bitmask & ICF_CREATE_XCONF_CONF_URI_PRESENT)    
    {
        p_pdb->p_call_ctx->conference_uri = 
                p_xt_conf_req->conference_uri;
    }
   else
    { 
        if(ICF_FAILURE == 
            icf_dbm_fetch_line_cntxt_blk(p_pdb->p_glb_pdb,
                                         p_xt_conf_req->line_id,
                                         (icf_line_data_st **)(&p_line_data),
                                         p_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
       else
        {
            if(p_line_data->conf_server_uri.addr_val.str_len != 0)    
             {
                 p_pdb->p_call_ctx->conference_uri =
                       p_line_data->conf_server_uri;
             }
            else
             {
                   ret_val = ICF_FAILURE;
             }

        }
    }

   /* The below code is for searching the call ctx corresponding to call id
      coming in existing_call_id field in create xconf request if there is 
      no call corresponding to it we will fail the validation */
    if(p_xt_conf_req->bitmask & ICF_CREATE_XCONF_EXIST_CALL_ID_PRESENT)
    {
        if (ICF_INVALID_CALL_ID == p_xt_conf_req->existing_call_id)
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                                    p_pdb->p_glb_pdb,
                                    p_xt_conf_req->existing_call_id,
                                    &p_add_party_call_ctx,
                                    p_pdb->p_ecode))
         {
            ret_val = ICF_FAILURE;
         }
    }   
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_xconf_validate_conf_req */


/*****************************************************************************
*
* FUNCTION NAME:  icf_xconf_send_forceful_clear_to_sic 
*
* DESCRIPTION  : This function is used to trigger forceful clear to SIC. 
*                Invoke internal msg with msg id as ICF_FORCEFUL_CALL_CLEAR_IND 
*                will be triggered to SIC for clearing it's ctx.
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_send_forceful_clear_to_sic(
        INOUT   icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_internal_msg_st  internal_msg = {{0,0,0,0,0,0},0};
   
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
     /* Prepare internal message ICF_FORCEFUL_CALL_IND
        and icf_sic_process_mesg. 
        SIC on recving this will clear it's ctx */ 
    
    internal_msg.msg_hdr.msg_id = ICF_FORCEFUL_CALL_CLEAR_IND;
    internal_msg.msg_hdr.p_glb_pdb = p_pdb->p_glb_pdb;
    internal_msg.msg_hdr.ecode = ICF_ERROR_TYPE_UNDEFINED;
    internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE;
    internal_msg.p_msg_data = ICF_NULL;
    icf_sic_process_mesg(&internal_msg);
 
    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}
/*****************************************************************************
*
* FUNCTION NAME:  icf_xconf_send_unsubscribe_req_to_sic
*
* DESCRIPTION  : This function is used to trigger Unsubscribe req to SIC. 
*                A internal message ICF_INTERNAL_SUBSCRIBE_REQ with duration as 0
*                will be triggered. 
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_send_unsubscribe_req_to_sic(
        INOUT   icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t      ret_val = ICF_SUCCESS;
    /* Fix for UMR while reading subs_req in SIC */
    icf_sic_subs_req_st   subs_req = {0,{0,"\0"},0,0,0,ICF_NULL,{{0,"\0"},0},
                                      0,{{0,"\0"},0},{0,0},{0,0},{0,{0,{0}},0},0};
    /* End of fix for UMR */
    icf_internal_msg_st  internal_msg = {{0,0,0,0,0,0},0};

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    
    /* Prepare internal message ICF_INTERNAL_SUBSCRIBE_REQ with
     * duration as 0 , call icf_sic_process_mesg which will trigger
     * unsubscribe REQ */ 
    internal_msg.msg_hdr.msg_id = ICF_INTERNAL_SUBSCRIBE_REQ;
    internal_msg.msg_hdr.p_glb_pdb = p_pdb->p_glb_pdb;
    internal_msg.msg_hdr.ecode = ICF_ERROR_TYPE_UNDEFINED;
    internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE;
    internal_msg.msg_hdr.status_2 = ICF_NULL;
    internal_msg.p_msg_data = (icf_uint8_t*)&subs_req;
    subs_req.call_id = p_pdb->p_call_ctx->call_id;
    subs_req.duration = 0;
    icf_port_memcpy(&(subs_req.destination_info),
                    &(p_pdb->p_call_ctx->conference_uri),
                    sizeof(icf_address_st) );

    icf_port_strcpy( subs_req.event_package.str, 
                        (icf_uint8_t*)"conference");
    if(ICF_FAILURE == icf_sic_process_mesg(&internal_msg))
    {
        ret_val = ICF_FAILURE;
    }   

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
*
* FUNCTION NAME:  icf_xconf_send_sub_req
*
* DESCRIPTION  : This function is used to trigger subscribe req to SIC. 
*                A internal message ICF_INTERNAL_SUBSCRIBE_REQ with duration as 
*                non zero will be triggered .
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_send_sub_req(
        INOUT   icf_xconf_pdb_st    *p_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_sic_subs_req_st  subs_req;
    icf_internal_msg_st  internal_msg = {{0,0,0,0,0,0},0};
    icf_config_data_st   *p_config_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
    icf_port_memset((icf_void_t*)&subs_req, 0, 
            ICF_PORT_SIZEOF(icf_sic_subs_req_st));
    ret_val = icf_dbm_get_module_glb_data(
                     p_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, p_pdb->p_ecode);
    
    /* Prepare internal message ICF_INTERNAL_SUBSCRIBE_REQ with 
       duration as non zero ,destination_info as conference server URI,
       event_pkg_name as “conference”. Call icf_sic_process_mesg which will 
       trigger subscribe REQ */ 
    internal_msg.msg_hdr.msg_id = ICF_INTERNAL_SUBSCRIBE_REQ;
    internal_msg.msg_hdr.p_glb_pdb = p_pdb->p_glb_pdb;
    /* call ctx is required in SCM return path from SIC
       and for SIC module we need to populate the same 
       in internal mesg */
    internal_msg.msg_hdr.p_glb_pdb->p_call_ctx = p_pdb->p_glb_pdb->p_call_ctx;
    internal_msg.msg_hdr.ecode = ICF_ERROR_TYPE_UNDEFINED;
    internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE;
    internal_msg.p_msg_data = (icf_uint8_t*)&subs_req;
    subs_req.call_id = p_pdb->p_call_ctx->call_id;
    subs_req.app_id = p_pdb->p_call_ctx->app_id;
    subs_req.line_id = p_pdb->p_call_ctx->line_id;
    subs_req.duration = 
       p_config_data->xconf_subs_duration;
    icf_port_memcpy(&(subs_req.destination_info),
                    &(p_pdb->p_call_ctx->conference_uri),
                    sizeof(icf_address_st) );

    icf_port_strcpy( subs_req.event_package.str, 
                       (icf_uint8_t*) "conference");
    if(ICF_FAILURE == icf_sic_process_mesg(&internal_msg))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        p_pdb->p_xconf_ctx->subs_done = ICF_TRUE;
    }

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)
    return ret_val;
}

/***************************************************************************
*
* FUNCTION NAME:   icf_xconf_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize XCONF context data
*
* RETURNS:
******************************************************************************/
icf_void_t  icf_xconf_deinit_ctx(
        INOUT icf_xconf_pdb_st  *p_pdb
        )
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_error_t     *p_ecode = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    if(ICF_NULL != p_pdb->p_xconf_ctx)
    {   

       /*Set state as ICF_XCONF_00_IDLE*/
       p_pdb->p_xconf_ctx->state = ICF_XCONF_00_IDLE;

       /*Call context NULL*/
       if(ICF_NULL != p_pdb->p_xconf_ctx->p_call_ctx)
       {    
          p_pdb->p_xconf_ctx->p_call_ctx = ICF_NULL;
       }   
    }  
    ret_val=ret_val;

    ICF_FUNCTION_EXIT(p_glb_pdb)
} /* icf_xconf_deinit_ctx */   
/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_alloc_srvc_cntxt_lgc_blk 
*
* DESCRIPTION   :This function calls the dbm function to allocate the 
*               service logic ctx block and initializes the parameters of the
*               service logic ctx created. 
*
*******************************************************************************/
icf_return_t icf_xconf_alloc_srvc_cntxt_lgc_blk(
        INOUT icf_xconf_pdb_st     *p_pdb,
        OUT  icf_void_t          **p_p_service_logic_ctx,
        INOUT icf_error_t        *p_ecode)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Call dbm to allocate the service ctx block */
    if (ICF_SUCCESS == icf_dbm_alloc_blk(
            p_glb_pdb,
            (icf_dbm_block_type_et)ICF_BLK_TYPE_SL_CTX,
            (icf_void_t **)p_p_service_logic_ctx,
            p_ecode))
    {
        /* Initiailze service logic context block to 0*/
       p_pdb->p_xconf_ctx = (icf_xconf_ctx_st *)
           *p_p_service_logic_ctx;
       icf_port_memset(
                (icf_void_t *)*p_p_service_logic_ctx,
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

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_send_display_ind_on_subs_failure 
*
* DESCRIPTION   :This function create a display ind message and send to 
*                application on receiving failure from SIC to send 
*                SUBSCRIBE request
*
*******************************************************************************/
icf_return_t icf_xconf_send_display_ind_on_subs_failure(
        INOUT icf_xconf_pdb_st     *p_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_msg_st           *p_msg = ICF_NULL;
    icf_display_ind_st   *p_display_ind = ICF_NULL;
    icf_uint16_t         api_len = 0;
   
    ICF_FUNCTION_ENTER(p_pdb->p_glb_pdb)
   
    api_len = sizeof(icf_api_header_st) +
              sizeof(icf_display_ind_st);
    
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_pdb->p_glb_pdb, 
               api_len, 
               ICF_MEM_COMMON, 
               p_msg,
               ICF_RET_ON_FAILURE, 
               p_pdb->p_ecode, 
               ret_val)

    p_display_ind = (icf_display_ind_st *)&(p_msg->payload[0]);

    /* Fill PA API hdr in p_msg */
    icf_xconf_fill_pa_msg_hdr(p_pdb,
            p_msg,
            (icf_api_id_t)(ICF_DISPLAY_IND),
            api_len);

    /* Fill the p_display_ind structure fields*/
    p_display_ind->display_id = 
        ICF_DISPLAY_SUBCRIPTION_FAILED_FOR_EXTERNAL_CONFERENCE;
        
    p_pdb->p_internal_msg->msg_hdr.msg_id =
        (icf_uint16_t)ICF_DISPLAY_IND;
    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE ==
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)

        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */
    else
    {
        ICF_MEMFREE(p_pdb->p_glb_pdb, 
                    p_msg, 
                    ICF_MEM_COMMON,
                    p_pdb->p_ecode, 
                    ret_val)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)

    return ret_val;
}
#endif
