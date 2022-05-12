/*****************************************************************************
*
* FILE NAME    : icf_rnw_util.c
*
* DESCRIPTION  : Contains the definitions of all Utility functions
*                of RNW service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 07-Mar-2005    Amit Sharma     None            Initial
* 27-Apr-2005    Amit Sharma                 Updations for REFER replaces_hdr
* 15-May-2005    Amit Sharma   SPR:7444      Update STATS
* 19-May-2005    Amit Sharma   SPR:7594      restored call_ctx of p_glb_pdb
* 29-May-2005    Amit Sharma                 Fix for SPR 7569
* 14-Jun-2005    Amit Sharma   SPR:7662      Fill conf info in both call ctxs
* 19-Oct-2005    Amit Sharma                 Updation for SCM changes
* 10-Nov-2005    Amit Sharma   SPR:8257
* 27-Feb-2006    Amit Sharma                 ICF Merging with icf rel2.0
* 01-Sep-2006    Amit Sharma   SPR:12710
* 01-Sep-2006    Amit Sharma   SPR:12728
* 24-Jan-2008    Neha Gupta    ICF Rel8.0    Modifications for precondition
*					     support in icf_rnw_fill_call_ctx
* 8-Apr-2008     Sumant Gupta  SPR:18355     Rel7.1 Windows porting Mergin
* 15-Apr-2008    Alok Tiwari   SPR:18364     Klocwork warning removal
* 07-Jun-2008  Amit Sharma       IPTK 8.0 SRTP LLD      SRTP Enhancements
* 19-JUn-2008  Anurag Khare    SPR 18684     CSR 1-6017450 Merge
* 24-Nov-2008  Rajiv Kumar     SPR 19215     CSR 1-5433195 Merge
* 24-Nov-2008  Rajiv Kumar     SPR 19188     Rel 7.1.2 SPR merging (SPR
*                                            18947) 
* 26-Nov-2008  Rajiv Kumar     SPR 19188     Rel 7.1.2 SPR merging (SPR
*                                            19096) 
* 08-Dec-2008  Rajiv Kumar     SPR 19218     CSR-1-6421580
* 15-Dec-2008  Tarun Gupta     SPR 19218     Merged CSR 1-5478785 
* 15-May-2009  Kamal Ashraf    SPR 19672     Merged CSR 1-6619361
* 28-May-2009  Kamal Ashraf    SPR 19672     Merged CSR 1-7129410
* 29-May-2009  Kamal Ashraf    SPR 19672     Merged CSR 1-6837599
* 10-Jun-2009  Kamal Ashraf    SPR 19590     Modified icf_rnw_send_refer_ind 
* 10-Jun-2009 Abhishek Dhammawat SPR 19590   header list overwrite enh
* 22-Jun-2009 Abhishek Dhammawat SPR 19769   Modified icf_rnw_send_refer_ind
* 30-Jul-2009 Rajiv Kumar        SPR 19886   Fix merged for CSR-1-7249691 
* 03-Sep-2009 Anuradha Gupta    SPR 19999    Merged CSR 1-7616300
* 26-Oct-2009 Abhishek Dhammawat SPR 20185   Modified
*                                            icf_rnw_validate_incoming_refer
* 26-Mar-2010 Udit Gattani     SPR 20352     Check configuration for hiding
*                                            identity in case of incoming call  
* 24-Jun-2010  Rohit Aggarwal  CSR 1-8716179  Updated icf_rnw_fill_call_ctx
*                                             to fetch line data in the 
*                                             beginning itself
*
* Copyright 2010,Aricent.
*****************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_rnw_types.h"
#include "icf_rnw_prototypes.h"
#include "icf_rnw_macro.h"
#include "icf_rnw_defs.h"
#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_sc_prototypes.h"


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_send_refer_resp
*
* DESCRIPTION   : This function is used to send REFER response message to SSA
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_rnw_send_refer_resp(
        INOUT  icf_rnw_pdb_st    *p_pdb,
        IN     icf_uint8_t       result)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_glb_pdb_st              *p_glb_pdb = ICF_NULL;
    icf_nw_otg_refer_resp_st    refer_resp = {0,0,0} ;
     icf_remote_call_transfer_initiated_cfm_st     
            *p_cta_init_cfm = ICF_NULL;
    icf_error_t         ecode = ICF_NULL;
    icf_config_data_st   *p_config_data = ICF_NULL;
    icf_list_st          *p_temp_list_node = ICF_NULL;


    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    ret_val =  icf_dbm_get_module_glb_data(
                                   p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                                   (icf_int_module_id_t)
                                   ICF_INT_MODULE_CFG,
                                   (icf_void_t **)&p_config_data,
                                   &ecode);

    if(ICF_TRUE == p_config_data->conf_cfm_reqd_at_transferee)
    {
        p_cta_init_cfm =
            (icf_remote_call_transfer_initiated_cfm_st*)
                &(p_glb_pdb->p_recv_msg->payload[0]);
    }


	refer_resp.bitmask = ICF_NULL;
    refer_resp.reason = ICF_FAILURE;

    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REFER_RESP;

    /*Initialize payload length = 0*/
    p_pdb->p_internal_msg->msg_hdr.payload_length = 0;

    /*If REFER response to be send as failure then only assign its values
     * in the response structre*/
    if (ICF_FAILURE == result)
    {
        refer_resp.bitmask = ICF_NW_OTG_REFER_RESP_REASON;
          if(p_pdb->resp_code == ICF_RNW_TIMER_EXPIRY)
        {
            refer_resp.reason = ICF_CAUSE_TIMER_EXPIRY;

        }
        else
        {
            if((ICF_TRUE == p_config_data->conf_cfm_reqd_at_transferee) && 
                (ICF_NULL != p_cta_init_cfm))
            {
                if(ICF_NULL != (
                ICF_CALL_TRANS_INIT_ERROR_CAUSE_PRESENT & 
                    p_cta_init_cfm->bit_mask))
                {
                    /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19096)*/
                    /*SPR 19096 In CSR 1-5478785 fix, the error code is 
                      wrongly used to populate the resp_code. As per the API
                      document sent with the CSR fix the response code should
                      be 403 in error situation except for the timer expiry
                      when it should be 486. So assigned the resp_code with 403
                      because here we know the confirmation API is received and
                      result is failure. Also error_code is correctly used to
                      populate the reason.
                    */
                    refer_resp.reason = p_cta_init_cfm->error_code;
                    refer_resp.resp_code = 403;                    
                }
                /* SPR 19218 : Merged SPR 19095 while merging CSR 1-5478785 */
                else if(ICF_RNW_TIMER_EXPIRY == p_pdb->resp_code)
                {
                    /*
                       SPR 19095 Previously ICF_REFER_RESP_REASON_TIMER_EXPIRY
                       which was defined as zero in rnw pdb structure was use for
                       RNW timer expiry, problem was in defining it as zero as well
                       not setting the same in 07 timer expiry function. The resp
                       code set in 07 timer expiry is RNW_TIMER_EXPIRY so the same 
                       should be used. For more information CSR 1-5478785 should be
                       referred.
                    */
                    refer_resp.resp_code = 486;
                }
                else if(ICF_NULL != (
                    ICF_CALL_TRANS_INIT_CFM_CODE_PRESENT &
                    p_cta_init_cfm->bit_mask))
                {
                    refer_resp.resp_code = p_cta_init_cfm->response_code;
                }
                else
                {
                    refer_resp.reason = ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ;  
                }
            }
            else
            {
                refer_resp.reason = ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ;    
            }
            ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CALL_TRANS_FRM_NW)
        }
    }


    p_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_nw_otg_refer_resp_st);

    /* SPR 19218 : Merged CSR 1-5478785 */
    if((ICF_NULL != p_pdb->p_rnw_ctx)&& (ICF_NULL != p_pdb->p_rnw_ctx->p_call_ctx_c)
            && (ICF_NULL != p_cta_init_cfm))
    {
        if (ICF_HDR_LIST_PRESENT & p_cta_init_cfm->bit_mask)
        {
            /*SPR 19590 change start */
            p_temp_list_node = p_pdb->p_rnw_ctx->p_call_ctx_c->p_list_of_header_list;
            if(ICF_NULL != p_temp_list_node)
            {    
                while(p_pdb->p_rnw_ctx->p_call_ctx_c->txn_id != 
                ((icf_header_link_list_st*)((p_temp_list_node)->p_data))->txn_id)
                {
                    p_temp_list_node = p_temp_list_node->p_next;
                    if(ICF_NULL == p_temp_list_node)
                       break;
                }
            }
            if(ICF_NULL != p_temp_list_node)
            {    
                  ret_val = icf_cmn_clone_header_list(
                   p_glb_pdb,
                   &(p_cta_init_cfm->header_list),
                   &(((icf_header_link_list_st*)((p_temp_list_node)->p_data))\
                        ->header_list),
                   &ecode);
            }      
            /*SPR 19590 change end */
        }
    }
    
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&refer_resp;

    ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_REFER_RESP_SENT)

    /*If REFER response is success then payload will be null*/
    
    /*Call SSA entry point to send the message*/
    ret_val =  icf_ssa_process_mesg(p_pdb->p_internal_msg);
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_send_refer_resp */


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_send_refer_ind
*
* DESCRIPTION   : This function is used to send REFER indication message to PA
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_rnw_send_refer_ind(
        INOUT  icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t       ret_val = ICF_SUCCESS, dummy_ret_val = ICF_SUCCESS;
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;
    icf_cc_pa_remote_call_transfer_initiated_ind_st  
        *p_refer_ind = ICF_NULL;
    icf_msg_st         *p_msg = ICF_NULL;
    icf_uint16_t        api_len;
    icf_error_t        *p_ecode = ICF_NULL;
    /* SPR 19590 : Start */
    icf_list_st        *p_temp_list_node = ICF_NULL;
    /* SPR 19590 End */

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
        
    api_len = sizeof(icf_api_header_st) +
    sizeof(icf_cc_pa_remote_call_transfer_initiated_ind_st);
  
    ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_RMT_CALL_TRANS_INIT_IND)

    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON, p_msg,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Assign pointer of the payload of the PA message*/
    p_refer_ind = 
    (icf_cc_pa_remote_call_transfer_initiated_ind_st *)&(p_msg->payload[0]);

    /*Fill PA API header in p_msg*/
    icf_rnw_fill_pa_msg_hdr(p_pdb, 
            p_msg, 
            (icf_api_id_t)(ICF_CC_PA_REMOTE_CALL_TRANSFER_INITIATED_IND),
            api_len);

    /*Fill refer_ind structure*/
    /*Fill call-id allocated to new call*/
    p_refer_ind->new_call_id = p_pdb->p_rnw_ctx->p_call_ctx_c->call_id;
    
    /*Fill REFER party ADDRESS from party C's call context 
     * remote party address*/
    p_refer_ind->referred_party_addr.addr_val.str_len = 
        p_pdb->p_rnw_ctx->p_call_ctx_c->remote_party_addr.addr_val.str_len;
    
    icf_port_strcpy(p_refer_ind->referred_party_addr.addr_val.str,
        p_pdb->p_rnw_ctx->p_call_ctx_c->remote_party_addr.addr_val.str);
    
    p_refer_ind->referred_party_addr.addr_type = 
        p_pdb->p_rnw_ctx->p_call_ctx_c->remote_party_addr.addr_type;
    
	if (ICF_NULL != 
		 p_pdb->p_rnw_ctx->p_call_ctx_c->remote_party_name.str_len)
    {
    	p_refer_ind->bit_mask =
        	ICF_CC_PA_REFER_INITIATED_REFERRED_PARTY_NAME;
   
    	/*Copy referred party name from party C's call context remote party name*/
    	p_refer_ind->referred_party_name.str_len = 
        	p_pdb->p_rnw_ctx->p_call_ctx_c->remote_party_name.str_len;

    	icf_port_strcpy(p_refer_ind->referred_party_name.str,
        	p_pdb->p_rnw_ctx->p_call_ctx_c->remote_party_name.str);
	}

    /* SPR 19590 : Start */
    /* Fetch the nodes from the header list maintained in the call context
       on the basis of txn_id */ 
    ret_val = icf_cmn_fetch_curr_hdr_list_node_from_call_ctx(p_glb_pdb,
                                                      &p_temp_list_node);
    /* checking the condition for ret_val, p_temp_list_node and header_list_count */
    if ((ICF_SUCCESS == ret_val) && (ICF_NULL != p_temp_list_node) &&
        (ICF_NULL != p_temp_list_node->p_data) &&    
        (ICF_NULL != ((icf_header_link_list_st*)(p_temp_list_node->p_data))
        ->header_list.count))
    {
        /* clone the header list */
        if (ICF_SUCCESS == icf_cmn_clone_header_list(
                           p_glb_pdb,
                           &(((icf_header_link_list_st*)
                           (p_temp_list_node->p_data))->header_list),
                           &(p_refer_ind->header_list),
                           p_ecode))
        {
            /* Setting bitmask for the presence of header list */
            p_refer_ind->bit_mask |= ICF_HDR_LIST_PRESENT;

            /* free the list in call ctx */
            ret_val = icf_cmn_del_header_list_node_in_call_ctx(p_glb_pdb,
                                                         p_temp_list_node);
        }
    /* SPR 19590 : End */
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_refer_ind->header_list.hdr = ICF_NULL;
        p_refer_ind->header_list.count = 0;
    }

    /* SPR 19590 : Start */ 
    /* Fetch the nodes from the body list maintained in the call context
       on the basis of txn_id */
    ret_val = icf_cmn_fetch_curr_body_list_node_from_call_ctx(p_glb_pdb,
                                                       &p_temp_list_node);
    /* checking the condition for ret_val, p_temp_list_node and body_list_count */
    if ((ICF_SUCCESS == ret_val) && (ICF_NULL != p_temp_list_node) &&
        (ICF_NULL != p_temp_list_node->p_data) &&    
        (ICF_NULL != ((icf_msg_body_link_list_st*)(p_temp_list_node->p_data))
        ->body_list.count))
    {
        /* clone the body list */
        if (ICF_SUCCESS == icf_cmn_clone_body_list(
                           p_glb_pdb,
                           &(((icf_msg_body_link_list_st*)
                           (p_temp_list_node->p_data))->body_list),
                           &(p_refer_ind->body_list),
                           p_ecode))
        {
            /* Setting bitmask for the presence of body list */
            p_refer_ind->bit_mask |= ICF_BODY_LIST_PRESENT;

            /* free the list in call ctx */
            ret_val = icf_cmn_del_body_list_node_in_call_ctx(p_glb_pdb,
                                                       p_temp_list_node);
        }
    /* SPR 19590 : End */
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_refer_ind->body_list.body = ICF_NULL;
        p_refer_ind->body_list.count = 0;
    }
    
    /*SPR 19835 reset the txn_id in call ctx as the node is deleted for
     * the header/body list.*/
    ICF_RESET_TXN_ID_IN_CALL_CTX(p_glb_pdb)
    /*Fill Internal message*/
    p_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_CC_PA_REMOTE_CALL_TRANSFER_INITIATED_IND;
    /*Fix for CSR1-5249829 */
    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_pdb->p_call_ctx->call_id;

    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;

    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE == 
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
		dummy_ret_val = ICF_FAILURE;
	}/* icf_adm_process_mesg(p_pdb-... */
    /* free the list in call ctx */
    ret_val = icf_cmn_free_header_list(p_glb_pdb,
                &(p_refer_ind->header_list),
                p_ecode);

     /* free the list in call ctx */
     ret_val = icf_cmn_free_msg_body_list(p_glb_pdb,
                &(p_refer_ind->body_list),
                p_ecode);

    ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return dummy_ret_val;
} /* icf_rnw_send_refer_ind */

/*******************************************************************************
*
* FUNCTION NAME    :   icf_rnw_fill_pa_msg_hdr
*
* DESCRIPTION  : This function fills the icf_api_header_st to send message 
*                to PA
*                
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_void_t icf_rnw_fill_pa_msg_hdr(
        INOUT   icf_rnw_pdb_st *p_pdb,
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
    
    p_msg->hdr.call_id = p_pdb->p_call_ctx->call_id;
    p_msg->hdr.app_id = p_pdb->p_call_ctx->app_id;

	ret_val = ret_val;
    ICF_FUNCTION_EXIT(p_glb_pdb)
} /* icf_rnw_fill_pa_msg_hdr */

/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_fill_call_ctx
*
* DESCRIPTION   : This function is used to fill newly allocated call context
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_rnw_fill_call_ctx(
        INOUT  icf_rnw_pdb_st    *p_pdb,
        INOUT  icf_call_ctx_st   *p_call_ctx)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_nw_inc_refer_st    *p_refer_req = ICF_NULL;
    icf_error_t            *p_ecode = ICF_NULL;
    icf_call_ctx_st        *p_call_ctx_b = ICF_NULL;
    icf_line_data_st       *p_line_data = ICF_NULL;
    /*  CSR 1-6017450 Merge */
    icf_user_db_node_st *p_user_db_node = ICF_NULL;
    icf_uint8_t search_bitmask = 0;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    p_call_ctx_b = p_call_ctx;
    p_call_ctx = p_glb_pdb->p_call_ctx;
    p_glb_pdb->p_call_ctx = p_call_ctx_b;

    /*Copy refer req msg pointer received from SSA*/
    p_refer_req = 
        (icf_nw_inc_refer_st *)(p_pdb->p_internal_msg->p_msg_data);

    p_call_ctx->line_id = p_glb_pdb->p_call_ctx->line_id;

    /* Fix for CSR 1-8716179 */
    /* Fetch the line data here itself */
    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                        p_glb_pdb,
                        p_glb_pdb->p_call_ctx->line_id,
                        (icf_line_data_st **)(&p_line_data),
                        p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
    }


#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_glb_pdb,ICF_TRUE)
    p_call_ctx->common_bitmask_2 |= ICF_PRECONDITION_REQD;
ICF_CHECK_QOS_SUPPORT_END
#endif
    
    /*Copy media transport_type and srtp_fallback_reqd flags from src
     *call context to new call context*/
    p_call_ctx->media_transport_type = p_glb_pdb->p_call_ctx->media_transport_type;
    p_call_ctx->srtp_fallback_reqd = p_glb_pdb->p_call_ctx->srtp_fallback_reqd;
                
    /*Allocate memnory for referred party address*/
    ICF_MEMGET(p_glb_pdb, sizeof(icf_address_st),
            ICF_MEM_COMMON, 
            p_call_ctx->p_referred_party_addr,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Copy Referred party Address from current call's remote party address*/
    icf_port_memcpy(p_call_ctx->p_referred_party_addr, 
            &(p_glb_pdb->p_call_ctx->remote_party_addr),
            (icf_uint32_t)sizeof(icf_address_st));

    /*Allocate memnory for referred party name*/
    ICF_MEMGET(p_glb_pdb, sizeof(icf_name_st),
            ICF_MEM_COMMON, 
            p_call_ctx->p_referred_party_name,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)
    
    /*Copy referred_party_name from current call's remote party name*/
    icf_port_memcpy(p_call_ctx->p_referred_party_name, 
            &(p_glb_pdb->p_call_ctx->remote_party_name), 
            (icf_uint32_t)sizeof(icf_name_st));
    
    /*New call's remote address will be referred-to address received
     *with REFER request*/
    icf_port_memcpy(&(p_call_ctx->remote_party_addr), 
            p_refer_req->p_referred_to,
            (icf_uint32_t)sizeof(icf_address_st));

    /*Free replace_nw_call_id in SSA*/
    ICF_MEMFREE(p_glb_pdb, p_refer_req->p_referred_to, 
            ICF_MEM_COMMON,
            p_ecode, ret_val)
 
    if (ICF_NW_INC_REFER_REF_TO_NAME & p_refer_req->bitmask)
    {
   	 	/*New call's remote name will be referred-to name received
     	*with REFER request*/
    	icf_port_memcpy(&(p_call_ctx->remote_party_name),
            p_refer_req->p_referred_to_name,
            (icf_uint32_t)sizeof(icf_name_st));
    
    	ICF_MEMFREE(p_glb_pdb, p_refer_req->p_referred_to_name, 
            ICF_MEM_COMMON,
            p_ecode, ret_val)
 	}

    if (ICF_SUCCESS == ret_val)
    {
        /* CSR 1-6017450 Merge */
        /*  If it is an incoming call then copy the Public 
            user with whom the call was established.
            else if outgoing call the copy the local address */
        if (ICF_NULL != (ICF_CALL_DIR & p_glb_pdb->p_call_ctx->common_bitmask))
        {
             	/* Fix for CSR 1-8436330: SPR 20352 */
 		/* For a REFER initiated call where initial call was outgoing, local
     	 * address stored in call context is used. This already takes care
     	 * of the presentation restriction setting while attempting otg call.
     	 * For a REFER initiated call where initial call was incoming, check if
     	 * local address in new call needs to be set as anonymous based on App setting.
     	 */

                    if (ICF_NULL == (ICF_SETTINGS_CONFIG_PRESENTATION_IND &
                                            p_line_data->default_settings))
                    { 
                        icf_port_strcpy(
                            p_call_ctx->local_name.str,
                            (icf_uint8_t *)"anonymous");
                        p_call_ctx->local_name.str_len =
                            icf_port_strlen(p_call_ctx->local_name.str);

                        icf_port_strcpy(
                                        p_call_ctx->local_address.addr_val.str,
                                        (icf_uint8_t *)"anonymous@anonymous.invalid");
                        p_call_ctx->local_address.addr_val.str_len =
                            icf_port_strlen(p_call_ctx->local_address.addr_val.str);

			/* set the address type same as that in initial call so that
			 * SIP URI remains as SIP URI and SIPS remains same as SIPS.
			 */
                        p_call_ctx->local_address.addr_type = 
                            p_glb_pdb->p_call_ctx->local_address.addr_type;

                  }
               
              else 
               {  
                search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;
                search_bitmask |= ICF_USER_DB_MATCH_ONLY_USER_NAME;
                /*  We accept the call on Assoc Uri so this check should also be available */
                search_bitmask |= ICF_USER_DB_USE_ASSOCIATED_URIS;
                if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line(p_glb_pdb,
                                                  p_line_data->line_id,
                                                  &(p_glb_pdb->p_call_ctx->local_address),
                                                  search_bitmask,
                                                  &p_user_db_node,
                                                  p_ecode))
                {
                    if (ICF_NULL != p_user_db_node)
                    {
                        icf_port_memcpy(&(p_call_ctx->local_address),
                        &(p_user_db_node->public_user),
                        (icf_uint32_t)sizeof(icf_address_st)); 
                    }
                    else
                    {
                        ret_val = ICF_FAILURE;
                    }
                }
                else 
                {
                    ret_val = ICF_FAILURE;
                }
            }
        } 
        else
        {
            /*Copy local_party name and address from other call context*/
            icf_port_memcpy(&(p_call_ctx->local_address), 
                    &(p_glb_pdb->p_call_ctx->local_address),
                    (icf_uint32_t)sizeof(icf_address_st));

           	/* CSR 1-8436330: SPR 20352 */
            /* Copy local name also in addition to local address */
            icf_port_memcpy(&(p_call_ctx->local_name), 
                &(p_glb_pdb->p_call_ctx->local_name),
                (icf_uint32_t)sizeof(icf_name_st));
       
        }
        /*  CSR 1-6017450 Merge*/
        if (ICF_FAILURE == ret_val)
        {
            return ret_val;
        }

        icf_port_memcpy(&(p_call_ctx->local_name), 
                &(p_glb_pdb->p_call_ctx->local_name),
                (icf_uint32_t)sizeof(icf_name_st));

	if (ICF_NULL != p_glb_pdb->p_call_ctx->p_pref_media_profile)
	{
		icf_list_st		*p_list_1 = ICF_NULL;
		icf_stream_capab_st	*p_stream_capab = ICF_NULL;
	
		p_list_1 = p_glb_pdb->p_call_ctx->p_pref_media_profile;
		while (ICF_NULL != p_list_1)
		{
            /*p_stream_capab = ICF_NULL;  */
			ICF_MEMGET(p_glb_pdb, sizeof(icf_stream_capab_st),
					ICF_MEM_COMMON, 
					p_stream_capab,
					ICF_RET_ON_FAILURE, p_ecode, ret_val)
            /*klocwork warning removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_stream_capab)

            if (ICF_NULL == p_stream_capab) 
            {
               ret_val = ICF_FAILURE;
               break;
            }
			icf_port_memcpy(p_stream_capab,
					p_list_1->p_data,
					(icf_uint32_t)sizeof(icf_stream_capab_st));
            p_stream_capab->p_codec_info_list = ICF_NULL;
            p_stream_capab->p_encoded_attributes = ICF_NULL;
                        
            /*Start : Addition for per line */        
            if ( ICF_NULL==((icf_stream_capab_st*)(p_list_1->p_data))->p_codec_info_list) 
             {
               if(p_stream_capab->stream_type == ICF_STREAM_TYPE_AUDIO)
               {
                if(p_line_data->p_codec_attributes_audio != ICF_NULL)
                {
                 ((icf_stream_capab_st*)(p_list_1->p_data))->p_codec_info_list =
                 p_line_data->p_codec_attributes_audio;
                }
               }
               else if(p_stream_capab->stream_type == ICF_STREAM_TYPE_VIDEO)
               {
                if(p_line_data->p_codec_attributes_video != ICF_NULL)
                 {
                  ((icf_stream_capab_st*)(p_list_1->p_data))->p_codec_info_list =
                  p_line_data->p_codec_attributes_video;
                 }
               }  
             }/* End Added for per line */  

			ret_val = icf_cmn_clone_codec_attrib_list(
					p_glb_pdb,
					&(p_stream_capab->p_codec_info_list),
					((icf_stream_capab_st*)(p_list_1->p_data))->p_codec_info_list);

			if (ICF_FAILURE == ret_val)
				break;
            /*klocwork warning removal*/
            else
            {
               ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,
                                 p_stream_capab->p_codec_info_list)
            }

			if (((icf_stream_capab_st*)(p_list_1->p_data))->p_encoded_attributes != NULL)
			{
				/*This function will be making deep copy of list of type 
				  icf_stream_list_st.*/
				ret_val=icf_cmn_copy_string_list(
						p_glb_pdb,
						&(p_stream_capab->p_encoded_attributes),
						((icf_stream_capab_st*)(p_list_1->p_data))->p_encoded_attributes,
						p_ecode);
				if (ICF_FAILURE == ret_val)
					break;
                /*klocwork warning removal*/
                else
                 {
                     ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,
                                   p_stream_capab->p_encoded_attributes)
                 }
			}

			ret_val = icf_cmn_add_node_to_list(p_glb_pdb,
					&(p_call_ctx->p_pref_media_profile),
					p_stream_capab);
			if (ICF_FAILURE == ret_val)
				break;
			p_list_1 = p_list_1->p_next;
		}
        if (ICF_FAILURE == ret_val && ICF_NULL != p_stream_capab)
        {
           /*Free p_codec_info_list before free the p_stream_capab*/
             icf_cmn_delete_codec_attrib_list(p_glb_pdb,
                              &(p_stream_capab->p_codec_info_list),
                                                          p_ecode);

           /*Free p_encoded_attributes before free the p_stream_capab*/
             icf_cmn_delete_list(p_glb_pdb,
                            &(p_stream_capab->p_encoded_attributes),
                                                    ICF_MEM_COMMON);

             ICF_MEMFREE(p_glb_pdb, 
                       p_stream_capab, 
                       ICF_MEM_COMMON,
                       p_ecode, 
                       ret_val) 
           ret_val = ICF_FAILURE;
        }
	}
	else
	{
		p_call_ctx->p_pref_media_profile = ICF_NULL;
	}

	if (ICF_SUCCESS == ret_val)
	{

		/*If REFER request received with REPLACES header, copy it to
		 *new call context*/
                if(ICF_TRUE == ICF_IS_BIT_SET(p_refer_req->bitmask,ICF_NW_INC_REFER_REPLACE_CALL_ID))
                {
			p_call_ctx->p_replaces_header = p_refer_req->p_replaces_hdr;

			ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_WITH_REPLACES)
                }

		p_call_ctx->app_id = p_glb_pdb->p_call_ctx->app_id;

		/*Copy call context pointer of the party which sent the REFER*/
		p_pdb->p_rnw_ctx->p_call_ctx_b = p_glb_pdb->p_call_ctx;

		/*Copy address of the party to which new call request to be sent*/
		p_pdb->p_rnw_ctx->p_call_ctx_c = p_call_ctx;

	}
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_fill_call_ctx */

/****************************************************************************
 *
 * FUNCTION NAME    :   icf_rnw_check_med_for_merge_req
 *
 * DESCRIPTION  : This function is used to check media ongoing for both the
 *                calls and if media is not ongoing then send merge media req
 *                
 *
 * RETURNS:  ICF_SUCCESS
 *
 *****************************************************************************/
icf_return_t  icf_rnw_check_med_for_merge_req(
        INOUT icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*If media is going on in Call B*/
    if(((ICF_INT_MODULE_INVALID != 
            ICF_RNW_GET_MEDIA_ONGOING(p_pdb->p_rnw_ctx->p_call_ctx_b))
            && (ICF_INT_MODULE_RNW != 
            ICF_RNW_GET_MEDIA_ONGOING(p_pdb->p_rnw_ctx->p_call_ctx_b))))
    {
        ICF_RNW_SET_STATUS(p_pdb, 
                ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

        /*If start proc media received on the other call Set service request
         * for the other call*/        
        if(p_pdb->p_rnw_ctx->p_call_ctx_b->call_id != 
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id)
        {
            ret_val = icf_rnw_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_rnw_ctx->p_call_ctx_b);
            
            ICF_RNW_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)
        } /* p_pdb->p_internal_msg->msg_hdr... */

        ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_02_AWT_START_PROC_MEDIA)
    } /* ICF_RNW_GET_MEDIA_ONGOING(p... */
    /*If media is ongoing in Call C*/
    else if (((ICF_INT_MODULE_INVALID !=
            ICF_RNW_GET_MEDIA_ONGOING(p_pdb->p_rnw_ctx->p_call_ctx_c))
            && (ICF_INT_MODULE_RNW != 
            ICF_RNW_GET_MEDIA_ONGOING(p_pdb->p_rnw_ctx->p_call_ctx_c))))
    {
        /*Set  start proc media*/
        ICF_RNW_SET_STATUS(p_pdb, 
                ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

        /*If start proc media recieved for the other call set service request
         * for the other call*/
        if(p_pdb->p_rnw_ctx->p_call_ctx_c->call_id != 
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id)
        {
            ret_val = 
                icf_rnw_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_rnw_ctx->p_call_ctx_c);
            
            ICF_RNW_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)
        } /* p_pdb->p_internal_msg->msg_hdr... */
        
        ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_02_AWT_START_PROC_MEDIA)
    } /* ICF_RNW_GET_MEDIA_ONGOING(p... */
    /* CSR_1_7129410 Merged SPR 19672 Start */
    else if ((ICF_INT_MODULE_INVALID != 
		(p_pdb->p_rnw_ctx->p_call_ctx_c->signalling_ongoing)) &&
            (ICF_INT_MODULE_RNW != 
		(p_pdb->p_rnw_ctx->p_call_ctx_c->signalling_ongoing)))
    {
	ICF_RNW_SET_STATUS(p_pdb,ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
	
	ICF_RNW_SET_CURR_STATE(p_pdb,ICF_RNW_05_AWT_START_PROC_SIGNALING)
    }
    /* CSR_1_7129410 Merged SPR 19672 End */

    /*If media is not onoing then send merge media request to MMI*/
    else if(ICF_FAILURE == icf_rnw_send_merge_media_req(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*Set media events req in both the calls*/
        ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_03_AWT_MERGE_MEDIA_RESP)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_rnw_check_med_for_merge_req */


/*****************************************************************************
 *
 * FUNCTION NAME    :   icf_rnw_send_merge_media_req
 *
 * DESCRIPTION  : This function sends Call merge Indication to MMI,
 *                which triggers Merge Media req. To send indicaion function
 *                creates an internal message for MMI entry point function
 *                for API ICF_SC_MERGE_MEDIA_SESSION_REQ.
 *               
 *
 * RETURNS:
 *
 ****************************************************************************/
icf_return_t  icf_rnw_send_merge_media_req(
        INOUT  icf_rnw_pdb_st    *p_pdb)
{
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_mmi_merge_request_st  merge_req;
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_call_ctx_st           *p_sec_call_ctx = ICF_NULL;
    icf_internal_list_st      *p_list = ICF_NULL;
    icf_stream_sdp_info_st    *p_stream_sdp = ICF_NULL;


    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Set media ongoing in RNW service in both service contexts */
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ)
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

    /*Assign message id for MMI*/    
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CREATE_CONF_REQ;

    p_pdb->p_internal_msg->msg_hdr.payload_length =
        sizeof(icf_mmi_merge_request_st);

    merge_req.count = 1;

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type) &&
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
        {
            merge_req.merge_stream[0].stream_id =
                p_stream_sdp->stream_id;
            break;
        }
        p_list = p_list->p_next;
    }

    /*Assign second call id*/
    merge_req.merge_call_id_2 = icf_rnw_get_other_call_id(p_pdb);
    if (merge_req.merge_call_id_2 == p_pdb->p_rnw_ctx->p_call_ctx_b->call_id)
	p_sec_call_ctx  = p_pdb->p_rnw_ctx->p_call_ctx_b;
    else
	p_sec_call_ctx  = p_pdb->p_rnw_ctx->p_call_ctx_c;

    p_list = p_sec_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type) &&
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
        {
            merge_req.merge_stream[0].merge_stream_id =
                p_stream_sdp->stream_id;
            break;
        }
        p_list = p_list->p_next;
    }

    
    /*Assign merge_media_req to p_msg_data of internal message*/
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&merge_req;

    /*Call entry point function of MMI*/
    ret_val = icf_mmi_process_mesg(p_pdb->p_internal_msg);

    if (ICF_SUCCESS == ret_val)
    {
        ret_val = icf_rnw_apply_both_calls(p_pdb);
    }
    /* CSR_1_7129410 Merged SPR 19672 Start */
    p_pdb->p_rnw_ctx->merge_media_sent = ICF_TRUE;
    /* CSR_1_7129410 Merged SPR 19672 End */

    ICF_RNW_TRACE(p_pdb, ICF_TRACE_MERGE_MEDIA_SENT)

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_send_merge_media_req */


/*****************************************************************************
*
* FUNCTION NAME:   icf_rnw_get_other_call_id
*
* DESCRIPTION  : This function is used to get the call id of the call other
*                then one in p_glb_pdb
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_call_id_t   icf_rnw_get_other_call_id(
        INOUT   icf_rnw_pdb_st    *p_pdb)
{
    icf_call_id_t    ret_call_id = ICF_INVALID_CALL_ID;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    
    icf_return_t    ret_val = ICF_SUCCESS;
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    if(p_pdb->p_rnw_ctx->p_call_ctx_b->call_id == 
            p_glb_pdb->p_call_ctx->call_id)
    {
        ret_call_id = p_pdb->p_rnw_ctx->p_call_ctx_c->call_id;
    } /* p_pdb->p_internal_msg->msg_hdr... */
    else
    {
        ret_call_id = p_pdb->p_rnw_ctx->p_call_ctx_b->call_id;
    } /* else */

	ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_call_id;
} /* icf_rnw_get_other_call_id */

/*****************************************************************************
*
* FUNCTION NAME:   icf_rnw_alloc_n_fill_serv_ctx
*
* DESCRIPTION  : This function is used to allocate new service context for
*                new call and fill all the information
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_rnw_alloc_n_fill_serv_ctx(
        INOUT   icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_call_ctx_st   *p_call_ctx_c = ICF_NULL;
    icf_error_t       *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Copy new call context pointer which address of the
     * party for new call*/
    p_call_ctx_c = p_pdb->p_rnw_ctx->p_call_ctx_c;
    
    /*Call DBM function to allocate new service context*/
    ret_val = icf_dbm_get_srvc_cntxt_blk(p_glb_pdb, p_call_ctx_c->call_id, 
                    (icf_service_ctx_st **)&(p_call_ctx_c->p_service_ctx),
                                p_ecode);

    /*If service context is allocate successfully*/
    if (ICF_SUCCESS == ret_val)
    {
        /*Allocate the call context pointer of which it is service ctx*/
        ((icf_service_ctx_st *)p_call_ctx_c->p_service_ctx)->p_call_ctx = 
            p_call_ctx_c;

        /*Activate the RNW service in this service context*/
        ((icf_service_ctx_st *)p_call_ctx_c->p_service_ctx)
            ->active_service_list.active_service_status 
            |= ICF_SERVICE_RNW;
       
        /*Store RNW service logic context pointer in service context*/
        ((icf_service_ctx_st *)p_call_ctx_c->p_service_ctx)
            ->active_service_list.p_sl_ctx[ICF_INT_MODULE_RNW - 
               ICF_INT_MODULE_SL_START - 1] = 
            p_pdb->p_rnw_ctx;
    } /* if (ICF_SUCCESS == ret_val) */
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_alloc_n_fill_serv_ctx */


/*****************************************************************************
*
* FUNCTION NAME:   icf_rnw_send_merge_media_ind
*
* DESCRIPTION  : This function is used to send merge media ind to PA
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_rnw_send_merge_media_ind(
        INOUT   icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_glb_pdb_st               *p_glb_pdb = ICF_NULL;
    icf_cc_pa_call_merged_ind_st *p_merge_ind = ICF_NULL;
    icf_msg_st                   *p_msg = ICF_NULL;
    icf_uint16_t                  api_len;
    icf_error_t                  *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
        
    api_len = sizeof(icf_api_header_st) +
    sizeof(icf_cc_pa_call_merged_ind_st);
   
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON, p_msg,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Set conference information in both the call contexts*/
    p_pdb->p_rnw_ctx->p_call_ctx_b->call_status |=
        ICF_CALL_STATUS_CONFERENCE;

    p_pdb->p_rnw_ctx->p_call_ctx_c->call_status |=
        ICF_CALL_STATUS_CONFERENCE;

    p_pdb->p_rnw_ctx->p_call_ctx_b->p_conf_other_call_ctx =
        p_pdb->p_rnw_ctx->p_call_ctx_c;

    p_pdb->p_rnw_ctx->p_call_ctx_c->p_conf_other_call_ctx =
        p_pdb->p_rnw_ctx->p_call_ctx_b;


    /*Assign pointer of the payload of the PA message*/
    p_merge_ind = (icf_cc_pa_call_merged_ind_st *)&(p_msg->payload[0]);

    /*Fill PA API header in p_msg*/
    icf_rnw_fill_pa_msg_hdr(p_pdb, 
            p_msg, 
            (icf_api_id_t)(ICF_CC_PA_CALL_MERGED_IND),
            api_len);

    /*Fill refer_ind structure*/
    /*Fill merge_call_id same as refer ind*/
    p_merge_ind->merge_call_id = icf_rnw_get_other_call_id(p_pdb);
    
    /*Fill Internal message*/
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CALL_MERGED_IND;
    /* Fix for CSR1-5249829 */
    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_pdb->p_call_ctx->call_id;

    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;

    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    ICF_RNW_TRACE(p_pdb, ICF_TRACE_MERGE_IND_SENT)

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE == 
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)

        ret_val = ICF_FAILURE;
    } /* icf_adm_process_mesg(p_pdb-... */
    else
    {
        ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)
    } /* else */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_send_merge_media_ind */


/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_apply_both_calls
*
* DESCRIPTION  : This function is used to fill status as both calls and also
*                fills the return path payload
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_apply_both_calls(
        INOUT   icf_rnw_pdb_st   *p_pdb
        )
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

    if (p_pdb->p_rnw_ctx->p_call_ctx_b->call_id
            == p_glb_pdb->p_call_ctx->call_id)
    {
        ret_val = icf_rnw_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_rnw_ctx->p_call_ctx_c);
    }
    else
    {
        ret_val = icf_rnw_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_rnw_ctx->p_call_ctx_b);
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_rnw_apply_both_calls */


/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_fill_other_call_info
*
* DESCRIPTION  : This function is used to allocate return payload and
*                set call ctx pointer in it
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_fill_other_call_info(
        INOUT   icf_internal_msg_st   *p_internal_msg,
        INOUT   icf_call_ctx_st       *p_call_ctx)
{
    icf_service_call_data_st   *p_msg_data = ICF_NULL;
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_glb_pdb_st             *p_glb_pdb = ICF_NULL;
    icf_error_t                *p_ecode = ICF_NULL;

    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Allocate memory for payload structure*/
    ICF_MEMGET(p_glb_pdb, sizeof(icf_service_call_data_st),
            ICF_MEM_COMMON, p_msg_data,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Assign call context pointer*/
    p_msg_data->p_other_call_ctx = p_call_ctx;

    /*Fill payload length*/
    p_internal_msg->msg_hdr.payload_length =
        sizeof(icf_service_call_data_st);

    /*Assign p_msg_data into p_msg_data in internal message*/
    p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_fill_other_call_info */


/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_check_to_send_notify
*
* DESCRIPTION  : This function is used to check sognaling ongoing in other
*                modules to notify message to SSA
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_check_to_send_notify(
        INOUT   icf_rnw_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if (ICF_INT_MODULE_INVALID != 
            p_pdb->p_rnw_ctx->p_call_ctx_b->signalling_ongoing)
    {
        ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_05_AWT_START_PROC_SIGNALING)

        p_pdb->p_rnw_ctx->next_state = ICF_RNW_05_AWT_START_PROC_SIGNALING;

        ICF_RNW_SET_STATUS(p_pdb, 
                ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)

        /*IF call in p_glb_pdb is other then call context B
         * then request for other call*/
        if (p_glb_pdb->p_call_ctx->call_id !=
                p_pdb->p_rnw_ctx->p_call_ctx_b->call_id)
        {
            ret_val = icf_rnw_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_rnw_ctx->p_call_ctx_b);
            
            ICF_RNW_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)
        }
    }
    else
    {
        ret_val = icf_rnw_send_notify_msg(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS);

        ICF_RNW_SET_CURR_STATE(p_pdb, ICF_RNW_06_AWT_NOTIFY_ACK)
    }
        
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_check_to_send_notify */


/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_send_notify_msg
*
* DESCRIPTION  : This function is used to send notify message to SSA
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_send_notify_msg(
        INOUT   icf_rnw_pdb_st   *p_pdb,
        IN      icf_uint8_t      reason
        )
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_glb_pdb_st              *p_glb_pdb = ICF_NULL;
    icf_call_ctx_st             *p_temp_call_ctx = ICF_NULL;
    icf_nw_otg_notify_st        notify_req;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ)

    ICF_RNW_TRACE(p_pdb, ICF_RNW_TRACE_NOTIFY_SENT)

    ICF_STAT(p_glb_pdb, ICF_STAT_CC_NW_NOTIFY)

    /*If call context in p_glb_pdb is other then the one which sent REFER then
     * update p_glb_pdb*/
    if (p_glb_pdb->p_call_ctx->call_id != 
            p_pdb->p_rnw_ctx->p_call_ctx_b->call_id)
    {
        p_temp_call_ctx = p_glb_pdb->p_call_ctx;
        p_glb_pdb->p_call_ctx = p_pdb->p_rnw_ctx->p_call_ctx_b;
    }

    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_NOTIFY;

    /*Send reason as success or failure to SSA*/
    notify_req.status= reason;

    /*Initialize payload length*/
    p_pdb->p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_nw_otg_notify_st);

    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&notify_req;

    /*Call SSA entry point to send the message*/
    ret_val =  icf_ssa_process_mesg(p_pdb->p_internal_msg);

    if (ICF_NULL != p_temp_call_ctx)
    {
        p_glb_pdb->p_call_ctx = p_temp_call_ctx;
    }
    /* CSR_1_7129410 Merged SPR 19672 Start */ 
    p_pdb->p_rnw_ctx->notify_sent = ICF_TRUE;
    /* CSR_1_7129410 Merged SPR 19672 End */
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_send_notify_msg */


#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME:   icf_rnw_trace
*
* DESCRIPTION  : This function is used to define the trace data for RNW service
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_void_t    icf_rnw_trace(
        INOUT icf_rnw_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    icf_port_memset((icf_uint8_t *)&trace_data,0,ICF_PORT_SIZEOF(icf_trace_data_st));

    switch(trace_id)
    {
        case ICF_TRACE_RNW_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            break;

        case ICF_TRACE_RNW_WITH_REPLACES:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_RNW_NEW_CALL_ID:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_rnw_ctx->p_call_ctx_c->call_id;
            break;
            
        case ICF_RNW_TRACE_REFER_RESP_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;

            break;

         case ICF_RNW_TRACE_NOTIFY_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_rnw_ctx->p_call_ctx_b->call_id;

            break;

        case ICF_RNW_TRACE_CC_INITIATED_CALL_REL:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            trace_data.int_val[0] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            break;

        case ICF_TRACE_MERGE_IND_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_rnw_ctx->p_call_ctx_c->call_id;
            break;

        case ICF_TRACE_MERGE_MEDIA_SENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_rnw_ctx->p_call_ctx_b->call_id;
            
            trace_data.int_val[1] = 
                (icf_uint32_t)p_pdb->p_rnw_ctx->p_call_ctx_c->call_id;
            break;

        case ICF_TRACE_REFER_IND_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            break;

         case ICF_TRACE_CURR_RET_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_internal_msg->msg_hdr.status;
            break;

        case ICF_TRACE_RNW_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_rnw_ctx->state;
            icf_rnw_state_trace_string(p_pdb->p_rnw_ctx->state, &trace_data);
            break;

        case ICF_RNW_TRACE_NEXT_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_rnw_ctx->next_state;
            icf_rnw_state_trace_string(p_pdb->p_rnw_ctx->next_state,
                    &trace_data);
            break;
            
        case ICF_TRACE_RNW_EVENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_rnw_ctx->event;
            
            trace_data.int_val[1] = 
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            
            icf_rnw_event_trace_string(p_pdb->p_rnw_ctx->event, &trace_data);

            break;
        
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    } /* switch(trace_id) */       

    ICF_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data)
} /* icf_rnw_trace */

/******************************************************************************
*
* FUNCTION NAME:   icf_rnw_app_trace
*
* DESCRIPTION  : This function is used to define the trace data for RNW service
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_rnw_app_trace(
        INOUT icf_rnw_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data ;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_trace_criteria_data_st    *p_criteria_data = p_pdb->p_internal_msg->
                           msg_hdr.p_glb_pdb->p_call_ctx->p_trace_criteria_data;
    /*Fix for CSR1-5249829 */
    icf_return_t ret_val = ICF_SUCCESS;
    ret_val = icf_cmn_check_app_trace(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if(ICF_FAILURE == ret_val) 
    {
      return ret_val;
    }
    else
    {

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    icf_port_memset((icf_uint8_t *)&trace_data,0,ICF_PORT_SIZEOF(icf_trace_data_st));

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->app_id;

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id;
   
 
    switch(trace_id)
    {
        case ICF_TRACE_REFER_RECVD:

            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

	    trace_data.int_val[0] =
                 p_pdb->replace_header_present;

            trace_data.int_val[1] =
                          p_pdb->new_call_id; 


            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;

	    icf_port_strcpy(trace_data.string_val[0].str,
                                        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->remote_party_addr.addr_val.str);
            trace_data.string_val[0].str_len =
                    (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);

	    icf_port_strcpy(trace_data.string_val[1].str,
                                        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->remote_party_name.str);
            trace_data.string_val[1].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[1].str);

	    break;
	
        case ICF_TRACE_REMOTE_CALL_TRANSFER_CFM:

            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            trace_data.int_val[0] =p_pdb->result;

            trace_data.int_val[1] =p_pdb->error_cause;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;


        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    } /* switch(trace_id) */

    ICF_APP_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data, p_criteria_data)
}
    return ret_val;
} /* icf_rnw_trace */

/******************************************************************************
*
* FUNCTION NAME    :   icf_rnw_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_rnw_state_trace_string(
        IN    icf_uint8_t          state, 
        INOUT icf_trace_data_st   *p_trace_data)
{

    /*Switch for trace_id*/
    switch(state)
    {
        case ICF_RNW_00_IDLE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_00_IDLE");
            break;
            
        case ICF_RNW_01_AWT_MEDIA_COMMIT:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                (icf_uint8_t *)"ICF_RNW_01_AWT_MEDIA_COMMIT");
            break;

        case ICF_RNW_02_AWT_START_PROC_MEDIA:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                (icf_uint8_t *)"ICF_RNW_02_AWT_START_PROC_MEDIA");
            break;

        case ICF_RNW_03_AWT_MERGE_MEDIA_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_03_AWT_MERGE_MEDIA_RESP");
            break;

        case ICF_RNW_04_AWT_FOR_CONNECT:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_04_AWT_FOR_CONNECT");
            break;

        case ICF_RNW_05_AWT_START_PROC_SIGNALING:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_05_AWT_START_PROC_SIGNALING");
            break;

        case ICF_RNW_06_AWT_NOTIFY_ACK:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_06_AWT_NOTIFY_ACK");
            break;

        case ICF_RNW_STATE_INVALID:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_STATE_INVALID");
            break;
    } /* switch(trace_id) */
    
}/*icf_rnw_state_trace_string*/

/******************************************************************************
*
* FUNCTION NAME    :   icf_rnw_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_rnw_event_trace_string(
        IN    icf_uint8_t         event, 
        INOUT icf_trace_data_st   *p_trace_data)
{
    switch(event)
    {
        case ICF_RNW_REFER:
            icf_port_strcpy(p_trace_data->string_val[0].str,
            (icf_uint8_t *)"ICF_RNW_REFER");
            break;

        case ICF_RNW_START_PROC_SIGNALING:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_START_PROC_SIGNALING");
            break;

        case ICF_RNW_START_PROC_MEDIA:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_START_PROC_MEDIA");
            break;

        case ICF_RNW_NOTIFY_SUCCESS_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_NOTIFY_SUCCESS_RESP");
            break;

        case ICF_RNW_MEDIA_COMMITTED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_MEDIA_COMMITTED");
            break;

        case ICF_RNW_MERGE_MEDIA_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_MERGE_MEDIA_RESP");
            break;

        case ICF_RNW_INVITE_SUCCESS_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_INVITE_SUCCESS_RESP");
            break;

        case ICF_RNW_MEDIA_ERROR:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_MEDIA_ERROR");
            break;

        case ICF_RNW_CALL_RELEASED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_CALL_RELEASED");
            break;

        case ICF_RNW_TIMER_EXPIRY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_TIMER_EXPIRY");
            break;

        case ICF_RNW_FORCEFUL_CALL_CLEAR:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_RNW_FORCEFUL_CALL_CLEAR");
            break;
        
        /* CSR_1_6837599 Merged SPR 19672 Start */
        case ICF_RNW_INTERNAL_RNW_REJ:
        /* CSR_1_6837599 Merged SPR 19672 End */
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    /* CSR_1_6837599 Merged SPR 19672 Start */
                    (icf_uint8_t *)"ICF_RNW_INTERNAL_RNW_REJ");
                    /* CSR_1_6837599 Merged SPR 19672 End */
            break;

        default:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"Invalid Event");
            break;
    } /* switch(trace_id) */
} /* INOUT icf_trace_data_st   *... */
#endif


/***************************************************************************
*
* FUNCTION NAME:   icf_rnw_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize RNW context data
*
* RETURNS:
******************************************************************************/
icf_void_t  icf_rnw_deinit_ctx(
        INOUT icf_rnw_pdb_st  *p_pdb
        )
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_error_t     *p_ecode = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
 
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Set state as ICF_RNW_00_IDLE*/
    p_pdb->p_rnw_ctx->state = ICF_RNW_00_IDLE;

    /*If timer ID is not 0 then stop timer*/
    if(0 != p_pdb->p_rnw_ctx->rnw_timer_id)
    {
        ICF_STOP_TIMER(p_glb_pdb,
                    p_pdb->p_rnw_ctx->rnw_timer_id, p_ecode, ret_val)

        p_pdb->p_rnw_ctx->rnw_timer_id = ICF_NULL;
    }

    /*If timer data has not been freed free it*/
    if(ICF_NULL != p_pdb->p_rnw_ctx->p_rnw_timer_data)
    {
        icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                (icf_void_t *)(p_pdb->p_rnw_ctx->p_rnw_timer_data),
                p_ecode);
        p_pdb->p_rnw_ctx->p_rnw_timer_data = ICF_NULL;
    }

    /*Call context NULL*/
    p_pdb->p_rnw_ctx->p_call_ctx_b = ICF_NULL;
    p_pdb->p_rnw_ctx->p_call_ctx_c = ICF_NULL;

    p_pdb->p_rnw_ctx->next_state = ICF_RNW_STATE_INVALID;
    p_pdb->p_rnw_ctx->pending_event = ICF_EVENT_UNDEFINED;
    /* CSR_1_7129410 Merged SPR 19672 Start */
    p_pdb->p_rnw_ctx->notify_sent = ICF_FALSE;
    /* CSR_1_7129410 Merged SPR 19672 End */

    /* CSR_1_7129410 Merged SPR 19672(after review comments) Start */
    p_pdb->p_rnw_ctx->merge_media_sent = ICF_FALSE;
    /* CSR_1_7129410 Merged SPR 19672 End */


    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
} /* icf_rnw_deinit_ctx */


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_start_rnw_timer
*
* DESCRIPTION   : Function is used to allocate memory for rnw timer data and
*                  start RNW timer
*
* RETURNS       : icf_return_t
*
******************************************************************************/
icf_return_t    icf_rnw_start_rnw_timer(
        INOUT icf_rnw_pdb_st     *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_config_data_st   *p_config_data = ICF_NULL;
    icf_error_t          *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(ICF_FAILURE ==
            icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)&p_config_data, p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /*Call DBM to allocate timer buffer value*/
    else if(ICF_FAILURE ==
            icf_dbm_get_mem_from_pool(p_glb_pdb, ICF_MEM_TIMER,
                (icf_void_t **)&(p_pdb->p_rnw_ctx->p_rnw_timer_data),
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {

        /* ICF_STAT(p_glb_pdb, ICF_STAT_NOTIF_TMR_STARTED) */

        p_pdb->p_rnw_ctx->p_rnw_timer_data->timer_type = ICF_RNW_TIMER;
        p_pdb->p_rnw_ctx->p_rnw_timer_data->module_id = ICF_INT_MODULE_RNW;
        /*
         * Store call context pointer in p_timer buffer
         * so that when timer expiry receives module can
         * identify the call data
         */
        p_pdb->p_rnw_ctx->p_rnw_timer_data->p_timer_buffer =
            (icf_void_t *)p_glb_pdb->p_call_ctx;
        p_pdb->p_rnw_ctx->p_rnw_timer_data->p_glb_pdb = p_glb_pdb;

        /*Start ICF_RNW_TIMER */
            ICF_START_TIMER(p_glb_pdb, p_pdb->p_rnw_ctx->p_rnw_timer_data,
                p_pdb->p_rnw_ctx->rnw_timer_id,
                p_config_data->rnw_timer,
                p_ecode, ret_val)
        ICF_STAT(p_glb_pdb, ICF_STAT_RNW_TMR_STARTED)


    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_rnw_start_rnw_timer */


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_clear_call_ctx
*
* DESCRIPTION   : Function is used to deallocate memory for newly 
*                 allocated call context
*
* RETURNS       : icf_return_t
*
******************************************************************************/
icf_return_t   icf_rnw_clear_call_ctx(
        INOUT  icf_rnw_pdb_st    *p_pdb,
        INOUT  icf_call_ctx_st   *p_call_ctx)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
    icf_error_t     ecode;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*If referred party address is not null then free it*/    
    if (ICF_NULL != p_call_ctx->p_referred_party_addr)
    {
        ICF_MEMFREE(p_glb_pdb, p_call_ctx->p_referred_party_addr, 
                ICF_MEM_COMMON,
                &ecode, ret_val)
    }

    /*If referred party name is not null then free it*/    
    if (ICF_NULL != p_call_ctx->p_referred_party_name)
    {
        ICF_MEMFREE(p_glb_pdb, p_call_ctx->p_referred_party_name, 
                ICF_MEM_COMMON,
                &ecode, ret_val)
    }

    if (ICF_NULL != p_call_ctx->p_replaces_header)
    {
        ICF_MEMFREE(p_glb_pdb, p_call_ctx->p_replaces_header, 
                ICF_MEM_COMMON,
                &ecode, ret_val)
    }

    /* SPR 19218 : Merged SPR 19095 while merging CSR 1-5478785 :
       Free the memory for preferred media profile to
       resolve the MIU for the same allocated in icf_rnw_fill_call_ctx*/
    if(ICF_NULL != p_call_ctx->p_pref_media_profile)
    {        
         ret_val = icf_cmn_free_icf_stream_capab_list(p_glb_pdb,
                         &(p_call_ctx->p_pref_media_profile));  
    }
    /*Deallocate call context*/
    ret_val = icf_dbm_dealloc_call_cntxt_blk(p_glb_pdb,
            p_call_ctx->call_id, &ecode);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_clear_call_ctx */


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_validate_incoming_refer
*
* DESCRIPTION   : Function is used to validate REFER received from network
*
* RETURNS       : icf_return_t
*
******************************************************************************/
icf_boolean_t   icf_rnw_validate_incoming_refer(
        INOUT  icf_rnw_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_boolean_t   result = ICF_TRUE;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
    icf_address_st  *p_refer_to_addr = ICF_NULL;
    icf_error_t     *p_ecode = ICF_NULL;
    icf_uint8_t     param_val[5] = {'\0'};

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Fix for SPR: 19886 (Merged CSR 1-7249691)*/
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
            p_glb_pdb->p_call_ctx,
            ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }
    /*Copy referred_to address from received message*/
    p_refer_to_addr = 
    ((icf_nw_inc_refer_st *)
    (p_pdb->p_internal_msg->p_msg_data))->p_referred_to;
#if 0   
    /*Check if refer_to address is same as local address*/
    if ((p_local_addr->addr_val.str_len == p_refer_to_addr->addr_val.str_len)
            && ICF_NULL == icf_port_strcmp(p_local_addr->addr_val.str, 
                p_refer_to_addr->addr_val.str)
            && (p_local_addr->addr_type == p_refer_to_addr->addr_type))
    {
        result = ICF_FALSE;
    } /* && (p_local_addr->addr_type ==... */
    else
    {
        result = ICF_TRUE;
    } /* else */
#endif
   
/*Fix merged for CSR-1-5433195*/   
#ifndef IPTK_INTEROP_SPECIFIC_CHANGE
    /*SPR18947 -The below mentioned code was commented for CSR1-5433195.
      Now this conflicts the regular behaviour of rejecting the REFER 
      if incoming refer contains the remote party address. 
      This change is placed in interop specific flag corresponding to
      CSR1-5433195 changes*/
    
    /* Fix for SPR: 19886 (Merged CSR 1-7249691)*/
    /* Replacing the function icf_port_strcmp with icf_cmn_compare_address.This
     * is done for following reason:
     * Consider remote party address in call context is User@172.16.15.12
     * and refer to header recived as User@172.16.15.12;User=phone; Now the
     * above two addresses are of same party and this refer should be rejected
     * with 403 response. However beacause of strict checking in
     * icf_port_strcmp we are sending 202 response for this REFER; considering
     * these are two different addresses.Function icf_cmn_compare_address
     * will strtok port and any parameter before comparision. Also this
     * function take care of E164/TEL uri.*/
    if (ICF_TRUE == icf_cmn_compare_address(p_refer_to_addr,
                    &(p_glb_pdb->p_call_ctx->remote_party_addr)))
    {
        result = ICF_FALSE;
    }
    /*If the address in refer to header is same as local address in call then
     * also reject the REFER with 403 response*/
    else if (ICF_TRUE == icf_cmn_compare_address(p_refer_to_addr,
                    &(p_glb_pdb->p_call_ctx->local_address)))
    {
        result = ICF_FALSE;
    }   
/* code removed for CSR-1-6619361 merge (SPR 19672) to mainline */

#endif

    /*Check for transport mode received in refer_to address, with the self mode
     * and proxy mode*/
    /* Fix for SPR: 19886 (Merged CSR 1-7249691)*/
    /* If the validation of remote/local address with refer to address is not 
       failed then perform the transport parameter validation.*/
    if ((ICF_FALSE != result) &&
            ICF_TRUE == icf_cmn_check_if_param_present(
                &(p_refer_to_addr->addr_val), (icf_uint8_t * )"transport",
                param_val, ICF_NULL, ICF_NULL))  
    {
        icf_config_data_st   *p_glb_cfg_data = ICF_NULL;
        icf_line_data_st     *p_line_data = ICF_NULL;
        icf_transport_mode_t transport_mode;
		/* CSR 1-6421580 : SPR 19024 */
		/* This comparison should be case insensitive */
        /*SPR 20185 6.4 code sync up merged missed change
          for CSR  1-6102712*/
        if (!icf_port_strcmpi(param_val, (icf_uint8_t *)"tcp"))
				transport_mode = ICF_TRANSPORT_TYPE_TCP;
		else if (!icf_port_strcmpi(param_val, (icf_uint8_t *)"tls"))
				transport_mode = ICF_TRANSPORT_TYPE_TLS;
		else
				transport_mode = ICF_TRANSPORT_TYPE_UDP;
        /*Fetch global cfg data*/
        if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                    p_glb_pdb, (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t **)&(p_glb_cfg_data), p_ecode))
        {
            result = ICF_FALSE;
        }
        else if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                            p_glb_pdb, p_glb_pdb->p_call_ctx->line_id,
                            (icf_void_t *)&(p_line_data),
                            p_ecode))
        {
            /* SPR 19999- CSR 7616300 SPR 19911-Added an additional check such that IPTK
			should establish proxy routed call only if both conditions are true 
			i.e proxy is configured and ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY is set 
			in set_call_params*/
            if ((ICF_CFG_LINE_DATA_PROXY_RECD &
                    p_line_data->line_data_received) && 
						(ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY & 
							 p_glb_cfg_data->default_settings))
            {
                if ((ICF_TRANSPORT_MODE_BOTH == p_line_data->transport_mode)
                    || ((ICF_TRANSPORT_TYPE_UDP == 
                        p_line_data->transport_mode)
                        && (ICF_TRANSPORT_TYPE_TCP != 
                            transport_mode))
                    || ((ICF_TRANSPORT_TYPE_TCP == 
                            p_line_data->transport_mode)
                        && (ICF_TRANSPORT_TYPE_UDP !=
                            transport_mode)))
                {
                    result = ICF_TRUE;
                } /* p_refer_to_addr->transport))) */
                else
                {
                    result = ICF_FALSE;
                }
            }
            else
            { /*If not proxy routed call then Compare with self mode*/
                if ((ICF_TRANSPORT_MODE_BOTH == p_glb_cfg_data->self_mode)
                    || ((ICF_TRANSPORT_TYPE_UDP == 
                        p_glb_cfg_data->self_mode)
                        && (ICF_TRANSPORT_TYPE_TCP != 
                            transport_mode))
                    || ((ICF_TRANSPORT_TYPE_TCP == 
                            p_glb_cfg_data->self_mode)
                        && (ICF_TRANSPORT_TYPE_UDP !=
                            transport_mode)))
                {
                    result = ICF_TRUE;
                } /* p_refer_to_addr->transport))) */
                else
                {
                    result = ICF_FALSE;
                }
            }
        } /* ssa_pdb.p_ecode)) */
        else
        {
            result = ICF_FALSE;
        }
    }
    
    if (ICF_FALSE == ret_val)
    { 
        /*Free replace_nw_call_id in SSA*/
        ICF_MEMFREE(p_glb_pdb, p_refer_to_addr,
                ICF_MEM_COMMON,
                p_ecode, ret_val)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return result;
} /* icf_rnw_validate_incoming_refer */
#endif

/**********************************************************************
 *FUNCTION:- icf_rnw_init_pdb
 *
 *Description:- This function initialises the structure icf_rnw_pdb_st.
 *
*********************************************************************/
icf_void_t icf_rnw_init_pdb(icf_rnw_pdb_st* p_rnw_pdb)
{
  p_rnw_pdb->p_rnw_ctx = ICF_NULL;
  p_rnw_pdb->p_call_ctx = ICF_NULL;
  p_rnw_pdb->p_service_ctx = ICF_NULL;
  p_rnw_pdb->p_internal_msg = ICF_NULL;
  p_rnw_pdb->resp_code = 0;
  
#ifdef ICF_TRACE_ENABLE
  p_rnw_pdb->replace_header_present = 0;
  p_rnw_pdb->new_call_id            = ICF_INVALID_CALL_ID;
  p_rnw_pdb->result                 = ICF_FAILURE;
  p_rnw_pdb->error_cause            = ICF_ERROR_NONE;
#endif

}/* End of the function icf_rnw_init_pdb......*/



 