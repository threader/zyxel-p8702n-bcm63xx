/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_api_handlers.c
 * 
 * DESCRIPTION: This file contains the api handlers of MMI.  
 *              
 * REVISION HISTORY:    
 *   Date            Name            Reference       Comments
 * 21-Dec-2004    Shefali Aggarwal       LLD         Initial Version
 *         
 * 28-Mar-2005    Shefali Aggarwal                   Updations
 *          
 * 28-Mar-2005    Shefali Aggarwal     SPR 7488
 *          
 * 09-May-2005    Shefali Aggarwal     SPR 7405
 *          
 * 19-May-2005    Shefali Aggarwal                   Fix SPR 7553
 *          
 * 19-May-2005    Shefali Aggarwal                   Fix SPR 7586
 * 
 * 02-Sept-2005   Kirti Dhingra					  Change in types
 *        	
 * 30-Nov-2005    Kirti Dhingra        ICF LLD       QOS Support for ICF   
 * 09-Mar-2006    Amit Sharma                        ICF merging with ICF2.0
 * 30-Mar-2006  Anuradha Gupta		   ICF 5.0     Call Modify Feature changes
 * 24-Jan-2008  Abhishek Dhammawat     Rel8.0     Modified 
 *                                                icf_mmi_any_state_dms_ind
 * 14-Feb-2008  Abhishek Dhammawat     Rel8.0     Moved functions of STATE 08
 *                                                to
 *                                                icf_mmi_08_awt_nm_qos_reserve_resp.c
 * 09-Feb-2009  Tarun Gupta         Rel 8.1 LLD   SDP Tunnelling Enhancements
 * 27-Feb-2009  Anurag Khare        Rel 8.1       changes done for options enhancements
 * 29-May-2009  Kamal Ashraf         SPR 19672    CSR_1_6837599 Merged
 *
 * Copyright (c) 2009, Aricent.  
 ******************************************************************************/

#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_any_state_dms_ind
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives DMS indication
 *                  from MM
 *                  Current State: Any State
 *                  Current Event: ICF_MMI_DMS_IND
 *                  
 * RETURNS:        ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                   
 *
 ******************************************************************************/

icf_return_t icf_mmi_any_state_dms_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    /* When DMS indication is recvd from MM. Error indication to clear 
     * everything will be send to CC as no media session exists now.
     */
        
    /* All the bits set as per the processing going will be reset.
     */ 
     
    ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_RESTORATION_IN_PROGRESS)
    ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)
    
    /* Delete is complete now */
    ICF_MMI_SET_BIT(p_mmi_pdb,ICF_DELETE_COMPLETE)

	/* Need to clear the stored CMS req/resp or MMS req/resp in the
	 * MMI ctxt as this is an any state function */
#ifndef ICF_MM_REQUEST_BUFFERING
	if (ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_media_session_req)
	{
		ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_mmi_pdb->p_mmi_ctx->p_media_session_req,
			ICF_MEM_COMMON,
			p_mmi_pdb->p_ecode,
			ret_val)
			
		p_mmi_pdb->p_mmi_ctx->p_media_session_req = ICF_NULL;
	}
#endif 

	/* No need to Send REL_RES_IND or CLOSE_CHANNEL_IND because evrything is
	 * finished on the part of Media Manager */

#ifdef ICF_NAT_MEDIA_TRAVERSAL
	/* Send REL_PUBLIC_IP_PORT for all active streams in Call ctxt */
	icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
    /* Rel8.0: This is hook for resource release. Previously
       icf_mmi_send_qos_release_req function was invoked here.
       However now we do not require to do PDP handling
       for IMS mode and keeping behaviour same for both 
       the IMS/NON IMS mode we invoke non ims function*/
	ret_val = icf_mmi_send_qos_release_res_req(p_mmi_pdb);

ICF_CHECK_QOS_SUPPORT_END
#endif		
	/* Send error ind to CC */
	ret_val = icf_mmi_send_err_ind_dms_ind(p_mmi_pdb);
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */ 
#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_xx_nm_deact_ind
 * 
 * DESCRIPTION:     This function is invoked when the MMI is in IDLE state and
 *                  it receives Secondary PDP Context Deactivation Indication
 *                  from NM.
 *					Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_NM_SEC_DEACT_IND
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_xx_nm_deact_ind(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    /* When Deactivation indication is recvd from NM. Error indication to clear 
     * everything will be send to CC as no media session should exist now.
     */
        
    /* All the bits set as per the processing going will be reset.
     */ 
     
    ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_RESTORATION_IN_PROGRESS)
   
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)

	ret_val = icf_mmi_send_qos_release_req(p_mmi_pdb);
	
ICF_CHECK_QOS_SUPPORT_END
	
	ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);	
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_xx_nm_modify_ind
 * 
 * DESCRIPTION:     This function is invoked when the MMI is in IDLE state and
 *                  it receives Secondary PDP Context Modification Indication
 *                  from NM.
 *					Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_NM_SEC_MODIFY_IND
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_xx_nm_modify_ind(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    /* When Modification Indication is recvd from NM. Error indication to clear 
     * everything will be send to CC as no media session exists now.
     */
        
    /* All the bits set as per the processing going will be reset.
     */ 
     
    ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_RESTORATION_IN_PROGRESS)
    
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)

		ret_val = icf_mmi_send_qos_release_req(p_mmi_pdb);

ICF_CHECK_QOS_SUPPORT_END
	
	ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);	
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;      
}/* End function */

#endif

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_unexpected_event
 * 
 * DESCRIPTION:     This function is invoked when any unexpected event is 
 *                  received. 
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_unexpected_event(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_error_t                          ecode = ICF_ERROR_NONE;
    /* CSR_1_6837599 Merged SPR 19672 Start */
       /* Variable "icf_mm_unexpected_msg_ind_st" removed to solve
          compile time warning */ 
    /* CSR_1_6837599 Merged SPR 19672 End */
    if(ICF_NULL == p_mmi_pdb)
    {
       return ICF_FAILURE;
    }    

	p_mmi_pdb = p_mmi_pdb;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    switch(p_mmi_pdb->p_mmi_ctx->mmi_event)
         {
           case ICF_MMI_GCL_RESP:
           case ICF_MMI_COMMIT_RESP:
           case ICF_MMI_OPEN_CHANNEL_RESP:
           case ICF_MMI_CMS_RESP:
           case ICF_MMI_MMS_RESP:
           case ICF_MMI_DMS_RESP:
           case ICF_MMI_DMS_IND:
           case ICF_MMI_MERGE_RESP:
           /* Added cases for new tunnel mode specific APIs */
		   case ICF_MMI_TUNNELLED_CMS_RESP:
		   case ICF_MMI_TUNNELLED_MMS_RESP:

           case ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP:
                 ret_val = icf_mmi_send_unexpected_msg_ind(p_mmi_pdb,&ecode);
                 break;
           /* CSR_1_6837599 Merged SPR 19672 Start */  
	       /* This case(ICF_MM_UNEXPECTED_MSG_IND) will never happen
                * as no module will send ICF_MM_UNEXPECTED_MSG_IND API
                *to MMI,hence Removed. */
           /* CSR_1_6837599 Merged SPR 19672 End */
         }

        
    ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_UNEXPECTED_EVENT)

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)    
    
    return ret_val;
}/* End function */    

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_call_clear
 * 
 * DESCRIPTION:     This function is invoked when call clear ind is recvd 
 *                  from MRM. 
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_call_clear(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
	icf_internal_list_st			*p_src_list = ICF_NULL;
	icf_stream_sdp_info_st			*p_src_data = ICF_NULL;
	icf_boolean_t					sdp_exists = ICF_FALSE;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb) 

    /* Media context clearing should be done only if p_local_sdp in call ctx
     * is not null. For all other scenarios, no MMI ctx exists for this call */

	 p_src_list = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;

	 while(ICF_NULL != p_src_list)
	 {
		p_src_data = p_src_list->p_payload;
		if(ICF_NULL != p_src_data->p_local_sdp)
		{
			sdp_exists = ICF_TRUE;
			break;
		}
		p_src_list = p_src_list->p_next;
	 }

     if (ICF_TRUE == sdp_exists)
     {
        /* Forceful call clear will be treated as forceful DMS request so
         * DMS and rel res request will be send and state will be changed to 
		 * IDLE */

         p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;

         ret_val = icf_mmi_send_dms_req(p_mmi_pdb);
         
		 if(ICF_SUCCESS == ret_val)
		 {
			 /* Change the state */
			 ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_05_AWT_DMS_RESP)
			 ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
		 }
		 ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

         if ( ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_media_session_req)
         {
             ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
                 p_mmi_pdb->p_mmi_ctx->p_media_session_req,ICF_MEM_COMMON,
                 p_mmi_pdb->p_ecode,ret_val)
                 p_mmi_pdb->p_mmi_ctx->p_media_session_req = ICF_NULL;
         }

         icf_mmi_send_rel_res_ind(p_mmi_pdb);

		ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
		icf_mmi_send_close_channel_ind(p_mmi_pdb); /* need to send the close channel ind. for all
													* streams in the call */
		ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)


#ifdef ICF_NAT_MEDIA_TRAVERSAL
		 icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif
     }

	 icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
    /* Change the state */
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
    
	/* Set delete complete bit */
    ICF_MMI_SET_BIT(p_mmi_pdb,ICF_DELETE_COMPLETE) 	 

    ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
    
    ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) 
    return ret_val;
}    
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_call_clear
 * 
 * DESCRIPTION:     This function is invoked when call clear ind is recvd 
 *                  from MRM. 
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_call_clear(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_internal_list_st			*p_src_list = ICF_NULL;
    icf_stream_sdp_info_st			*p_src_data = ICF_NULL;
    icf_boolean_t					sdp_exists = ICF_FALSE;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb) 
    
    /* 
       In tunnel mode, whenever MMI receives ICF_FORCEFUL_CALL_CLEAR_IND,
       it triggers the handler function icf_mmi_call_clear. 
       Now in tunnel mode,we do not want to send a DMS request as it would
       lead to ES appending a dummy DMS response and posting it back to MMI.
       However due to absence of a valid call context, MMI will reject
       that DMS response with ICF_UNEXPECTED_MSG_IND through function
       icf_mmi_process_mesg.Thus the application would be given this
       indication without it sending any API.

       To stop this, we will return from this function without going into
       the process of DMS request/response.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
    return ret_val;
ICF_CHECK_TUNNEL_MODE_END

    /* Media context clearing should be done only if p_local_sdp in call ctx
    * is not null. For all other scenarios, no MMI ctx exists for this call */
    
    p_src_list = p_mmi_pdb->p_call_ctx->p_stream_sdp_info;
    
    while(ICF_NULL != p_src_list)
    {
        p_src_data = p_src_list->p_payload;
        if(ICF_NULL != p_src_data->p_local_sdp)
        {
            sdp_exists = ICF_TRUE;
            break;
        }
        p_src_list = p_src_list->p_next;
    }
    
    if (ICF_TRUE == sdp_exists)
    {
    /* Forceful call clear will be treated as forceful DMS request so
    * DMS and rel res request will be send and state will be changed to 
        * IDLE */
        
        p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;
        
        
        switch(p_mmi_pdb->p_mmi_ctx->mmi_state)
        {
        case ICF_MMI_00_IDLE:
            
            if(!(ICF_DELETE_COMPLETE & p_mmi_pdb->p_mmi_ctx->mmi_bitmask))
            {
                icf_mmi_send_forceful_dms_req(p_mmi_pdb);
            }
            
            break;
            
        case ICF_MMI_01_AWT_GCL_RESP:
        case ICF_MMI_06_AWT_OPEN_CHANNEL_RESP:
        case ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP:
            
            if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
            {
                icf_mmi_send_forceful_dms_req(p_mmi_pdb);
            }
            else
            { 
                p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;
                icf_mmi_send_rel_res_ind(p_mmi_pdb);
            }
            break;
            
        case ICF_MMI_02_AWT_CMS_RESP:
        case ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP:
        case ICF_MMI_04_AWT_MMS_RESP:
            
            icf_mmi_send_forceful_dms_req(p_mmi_pdb);
            break;
            
        case ICF_MMI_03_AWT_COMMIT_RESP:
            
            if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
            {
                icf_mmi_send_rel_res_ind(p_mmi_pdb);
                ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
            }
            else
            {
                icf_mmi_send_forceful_dms_req(p_mmi_pdb);
            }
            break;
            
            
        case ICF_MMI_05_AWT_DMS_RESP:
            ICF_MMI_RESET_BIT(p_mmi_pdb,ICF_MEDIA_ESTABLISHED)
                
                if(ICF_MMI_DMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
                {
                    icf_mmi_send_rel_res_ind(p_mmi_pdb);
                    
                    ICF_MMI_PDB_SET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
                    icf_mmi_send_close_channel_ind(p_mmi_pdb); /* need to send close channel for 
                    * for all streams including TBCP */
                    ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,ICF_MMI_PDB_ALL_STREAMS)
                        
                }
                else if(ICF_MMI_MMS_DEL_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
                {
                    icf_mmi_send_forceful_dms_req(p_mmi_pdb);
                }
                break;
                
        case ICF_MMI_07_AWT_MERGE_RESP:
		{
	 icf_call_ctx_st                      *p_dbm_call_ctx = ICF_NULL;	
            ret_val = icf_dbm_find_call_cntxt_blk(
                p_mmi_pdb->p_glb_pdb,
                p_mmi_pdb->p_mmi_ctx->merge_call_id,
                &(p_dbm_call_ctx),
                p_mmi_pdb->p_ecode);
            
            if(ICF_SUCCESS == ret_val)
            {
                p_dbm_call_ctx->mmi_context.merge_call_id  = ICF_INVALID_CALL_ID;
                p_mmi_pdb->p_mmi_ctx->merge_call_id = ICF_INVALID_CALL_ID;
            }
            else
            {
                ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
                    return ret_val;                
            }
            
            icf_mmi_send_forceful_dms_req(p_mmi_pdb);
            break;
		}
            
        }
        
#ifdef ICF_NAT_MEDIA_TRAVERSAL
        icf_mmi_rel_nat_resources(p_mmi_pdb);
#endif
    }
    
    icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
    
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
        
        ICF_MMI_SET_BIT(p_mmi_pdb,ICF_DELETE_COMPLETE) 	 
        
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)
        
        ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) 
        return ret_val;
} 

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_xx_med_cap_request
 *
 * DESCRIPTION:     This function is invoked outsice the fsm when Media 
 *                  Capabilities Request is received from SSA
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t 
icf_mmi_xx_med_cap_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    ret_val = icf_mmi_send_med_cap_req(p_mmi_pdb);
       
    if (ICF_SUCCESS == ret_val)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_MED_CAP_REQ)            
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End icf_mmi_xx_med_cap_request */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_xx_med_cap_resp
 *
 * DESCRIPTION:     This function is invoked outsice the fsm when Media 
 *                  Capabilities Response is received from MM
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t 
icf_mmi_xx_med_cap_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
ICF_CHECK_TUNNEL_MODE_START(p_mmi_pdb->p_glb_pdb, ICF_FALSE)
    ret_val = icf_mmi_send_sic_med_cap_resp(p_mmi_pdb);
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_mmi_pdb->p_glb_pdb, ICF_TRUE)
    ret_val = icf_mmi_send_sic_tunnelled_med_cap_resp(p_mmi_pdb);
ICF_CHECK_TUNNEL_MODE_END
       
    if (ICF_SUCCESS == ret_val)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_MED_CAP_RESP)            
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End icf_mmi_xx_med_cap_resp */

/******************************************************************************
 * FUNCTION:        icf_mmi_xx_tunnelled_dms_request
 * DESCRIPTION:     This function is invoked when the MMI is in any tunnel
 *                  mode specific state and it receives a tunnelled DMS request.
 *                  Current State: ICF_MMI_00_IDLE/
 *                                 ICF_MMI_11_AWT_TUNNELLED_CMS_RESP/
 *								   ICF_MMI_12_AWT_TUNNELLED_MMS_RESP
 *                  Current Event: ICF_MMI_TUNNELLED_DMS_REQ
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *****************************************************************************/
icf_return_t icf_mmi_xx_tunnelled_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_return_t                    	dummy_ret_val = ICF_SUCCESS;
    icf_msg_st                          *p_mmi_msg = ICF_NULL;
    icf_uint8_t							api_len = ICF_NULL;
    
   	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)  /* Function entry trace */

    ICF_CHECK_IF_VAR_IS_NULL(p_mmi_pdb->p_glb_pdb,
                             p_mmi_pdb->p_call_ctx,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }
	
	/* Set state to ICF_MMI_05_AWT_DMS_RESP */	
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_05_AWT_DMS_RESP)
	
    api_len = ICF_PORT_SIZEOF(icf_delete_media_session_req_st) +
	          ICF_PORT_SIZEOF(icf_api_header_st);
    
	ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
               api_len,
               ICF_MEM_COMMON,
      	       p_mmi_msg,
               ICF_DONOT_RET_ON_FAILURE,
               p_mmi_pdb->p_ecode,
               dummy_ret_val)

    if(ICF_FAILURE == dummy_ret_val)
    {
        ret_val = ICF_FAILURE;
	    ICF_ALARM(((icf_uint8_t *)"[MMI]: MEMGET failure"))
		ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    	return dummy_ret_val;
    }
    
	/* Fill the header of the message to be send */
    p_mmi_msg->hdr.api_id = ICF_DELETE_MEDIA_SESSION_REQ;
    p_mmi_msg->hdr.source_id = ICF_MODULE_ICF;
    p_mmi_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mmi_msg->hdr.api_length = api_len;
    p_mmi_msg->hdr.call_id = p_mmi_pdb->p_call_ctx->call_id;
    p_mmi_msg->hdr.version = ICF_VERSION_1_0;
    p_mmi_msg->hdr.app_id = p_mmi_pdb->p_call_ctx->app_id;

	ret_val = icf_mmi_send(p_mmi_pdb,p_mmi_msg,p_mmi_pdb->p_ecode);
    
	ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
	        	p_mmi_msg,
        		ICF_MEM_COMMON,
             	p_mmi_pdb->p_ecode,
        		dummy_ret_val)
    
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb) /* Function exit trace */
    return ret_val;   
}
