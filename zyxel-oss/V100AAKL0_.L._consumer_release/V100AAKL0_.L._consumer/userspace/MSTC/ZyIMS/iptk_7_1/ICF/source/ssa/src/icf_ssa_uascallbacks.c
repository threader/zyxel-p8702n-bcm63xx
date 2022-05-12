 /** FUNCTION:
 **                     This file contains the implementation for SIP UA toolkit
 **                     callbacks for UA Server.
 **
 *****************************************************************************
 **
 ** FILENAME:		icf_ssa_uascallbacks.c
 **
 ** DESCRIPTION:        This file contains implementation for the
 **                     SIP UA toolkit related callbacks for UAS
 **                     functionality.
 **	
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 22-JAN-2005     Umang Singh                           INITIAL CREATION
 **                 Aashish Suchdev  
 **
 ** 18-May-2005     Umang Singh         SPR Fix 7555    Setting of Contact 
 **                                                     header done on receving
 **                                                     INVITE
 ** 25-May-2005     Umang Singh         SPR Fix 7403
 ** 06-Jun-2005     Jalaj Negi          IMS CLIENT      Added code for IMS
 **                                                     CLIENT    
 ** 07-Jun-2005     Umang Singh         SPR 7650
 ** 14-Jun-2005     Umang Singh         SPR 7663        Privacy header handling
 ** 22-JUN-2005     Umang Singh         SPR 7665             
 ** 30-JUN-2005     Umang Singh         CSR 1-2200068   Cleanup of transaction
 ** 12-AUG-2005     Shekhar Thakur      ICF             ICF additions 
 **                                                     info on callback failure
 ** 15-SEP-2005     Jalaj Negi          ICF             Incoming Call Memory
 **                                                     Leaks fixes
 ** 
 ** 09-DEC-2005     Jalaj Negi          ICF_4_0         Function call for 
 **                                                     extracting P-Media-Authorization
 **                                                     token for QoS support
 ** 03-Mar-2006     Amit Sharma                         ICF merging with ICF2.0
 ** 23-Mar-2006     Amit Sharma                         Code merged with ICF 4.2
 ** 3-Apr-2006      Deepti Goyal        ICF_5_0         Code changes for Message
 **                                                     Body headers support
 ** 29-July-2006    Aman Aggarwal       ICF_6_0         Multiple m-lines changes
 **                 Deepti Goyal                                    
 **
 ** 29-July-2006	Deepti Goyal		SPR 12645		SSA Not filling the SDP
 **														list for Re-Invite.
 **
 ** 4-Sep-2006      Deepti Goyal        SPR 12759       Merged Options timer code.
 ** 06-Sep-2006     Umang Singh         SPR 12855	Parsing of Require hdr for 100rel
 **
 **
 ** 7-Sep-2006	   Deepti Goyal		SPR 12713       Generic Call Clear if
 **						        Incorrect SDP in PRACK/
 **							ACK
 ** 24-Nov-2006     Umang Singh        SPR 13908/CSR-1-3704087
 ** 19-Dec-2006     Puneet Sharma      SPR 13867 / SPR14073
 ** 04-Jan-2007     Amit Mahajan   Amit Mahajan          INFO confirmation changes.  
 ** 15-Jan-2007     Umang Singh        SPR 14070
 ** 19-Jan-2007	    Aman Aggarwal      SPR 14017/14003
 ** 15-March-2007   Abhishek Dhammawat  SPR 14507        Added handling for 416
 **                                                      for OPTIONS method
 ** 19-Apr-2007     Rajat Chandna                        Changes for update/reinvite
 **                                                      enhancement
 ** 12-Mar-2007     Deepti Goyal        SPR 14508        Set SipMethod as "Message"
 **                                                      for sending the response
 **                                                      for Incoming Message 
 **                                                      Indication   
 ** 17-Apr-2007     Abhishek Dutta     ICF 6.3           Added the changes for
 **                                                      TWC-UPDATE Handling
 ** 10-Jul-2007     Abhishek Dutta     ICF 7.0           Added the changes for
 **                                                      Cancel call modify 
 **                                                      Handling
 ** 12-Jul-2007     Puneet Sharma
 **                 & Amit Mahajan      SPR-15259        IPTK should send 488 for incorrect media format.
 ** 28-Sept-2007    Abhishek Dhammawat  SPR16176         freeing the
 **                                                      sdp_stream_info and header list when
 **                                                      failure occurs for incoming INVITE
 ** 18-OCT-2007     Abhishek Dhammawat  SPR16877         free p_user_address
 **                                                      before deallocating sic ctx in 
 **                                                      sdf_cbk_uaTransactionReceived.
 ** 06-Dec-2007     Jagmohan Chauhan    SPR17648         Fix for options send out of dialog from network 
 **
 ** 07-Jan-2008     Amit Sikka          SPR 17945        Support for PUBLISH/MESSSAGE
 ** 18-Jun-2009     Anurag Khare        CSR 1-7407430    Cnahged function 
 **                                                      sdf_cbk_uaNewCallReceived
 ** 19-Jun-2009		Aman Aggarwal		CSR 1-7413101 	 Set the Min-SE value from 
 **														 session timer data before sending 422 response
 ** 23-Jun-2009		Aman Aggarwal		CSR 1-7427573	 IPTK shall populate rejected stream in INVITE
 **														 only once
 ** 23-Jun-2009     Alok Tiwari  CSR 1-7407430/SPR-19773  Modified Function
 **                                                       sdf_cbk_uaFailedCallAcknowledged
 ** 01-Jul-2009		Aman Aggarwal		CSR 1-7294946	  Changes done for P-Early-Media hdr
 ** 10-Jul-2009     Anurag Khare        CSR 1-7497277     Modifed Function
 **                                                       sdf_cbk_uaTransactionReceived
 ** 13-Aug-2009     Anurag Khare       CSR 1-7644298      Fix for CSR 7644298
 ** 18-Nov-2009		Pavneet Bajaj		SPR 20220         Fix for CSR 7991608
 ** 23-Nov-2009		Pavneet Bajaj		SPR 20221		  Fix for CSR 8012793 
 ** 10-Dec-2009     Pavneet Bajaj       SPR 20235         Fix for CSR 8071369 
 ** 04-Jan-2010     Pavneet Bajaj       SPR 20251         Fix for CSR 7945729 
 *****************************************************************************
 **            Copyrights 2007, Aricent
 *****************************************************************************/


#include "uatk_headers.h"
#include "stack_headers.h"


#include "icf_dbm_prototypes.h"
#include "icf_common_types.h"
#include "icf_common_defs.h"
#include "icf_internal_api_id.h"
#include "icf_macro.h"
#include "icf_port_defs.h"
#include "icf_ssa_macro.h"
#include "icf_ssa_defs.h"
#include "icf_ssa_types.h"
#include "icf_ssa_prototypes.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif

 /*****************************************************************************
 ** FUNCTION:		sdf_cbk_uaNewCallReceived
 **
 ** DESCRIPTION:	Callback issued onto the application on the receipt of a
 *					fresh INVITE message indicating receipt 
 *       1. Fetch ssa_ctxt from pEventContext received and ssa_pdb from 
 *          ssa_ctxt Check via header for loop detection 
 *          i.e. local address should not be there in the header, 
 *          otherwise return failure
 *       2. Check if REPLACES header is present in the new call request, 
 *          if present then fetch the header extract n/w call id and map it 
 *          to ipphone internal call id. 
 *          If not matched then send failure response to n/w.
 *       3. Allocate and initialize new call context.
 *       4. Populate diversion header
 *       5. Check if referred_by header is present, then extract refer 
 *          address and display name and populate it in call context.
 *       6. Check if SDP is present, then create sdp block and populate it 
 *          with received SDP
 *       7. Validate request uri, 
 *          if fail then send appropriate failure response
 *       8. Check p_asserted identity header
 *       9. Populate call context block and mark message for CM
 *
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaNewCallReceived(
        Sdf_st_callObject	 **ppCallObj,
        Sdf_st_eventContext  *pEventContext,
        Sdf_st_error		 *pErr)
{

    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_st_commonInfo                   *p_common_info = Sdf_co_null;
    Sdf_st_appData                      *p_app_data = Sdf_co_null;
    SipFromHeader                       *p_from = 
        ((SipFromHeader *)(*ppCallObj)->pCommonInfo->pFrom->pHeader);
    SipHeader                           *p_assert_hdr = SIP_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_call_ctx_st                     *p_call_ctx = ICF_NULL ;
    icf_error_t                         *p_ecode = ICF_NULL;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_nw_inc_call_setup_st            *p_internal_msg_param = ICF_NULL;
    SipHeader                           *p_called_hdr = SIP_NULL;
    SipFromHeader                       *p_temp = SIP_NULL;                                                                           Sdf_ty_retVal                       ret_value = Sdf_co_success;
/* For Join Header */
    icf_call_id_t                       join_call_id = ICF_INVALID_CALL_ID;
    icf_boolean_t                       join_header_present = ICF_FALSE;
   
#if 0
	/* These flags will be used to free memory allocated by SSA
	 * for asserted_identity and called_party_id headers in case
	 * due to any FAILURE CC is not given indication then SSA has
	 * to free these headers
	 */
	icf_boolean_t                       asserted_identity_allocated = ICF_FALSE,
							            called_party_id_allocated = ICF_FALSE;
#endif
    icf_config_data_st                  *p_cfg_data = ICF_NULL;
    /* list of type icf_inc_media_sdp_info_st */
    icf_list_st		                    *p_media_sdp_info = ICF_NULL;
	
	/*This st is populated if there is any SDP*/
	/*Then this sdp is populated */
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL; 

    Sdf_ty_messageValidity   			validity = Sdf_en_valid;

     if (pEventContext->pData != (*ppCallObj)->pAppData->pData)
     {
         printf ("\n sdf_cbk_uaNewCallReceived: ssa context are not matching\n");
         ret_val = ICF_FAILURE;
         return ret_val;
     }
    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

	p_ecode = p_ssa_pdb->p_ecode;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
#if 0 /*THis part commented when self transport enhancement done
       *not required now*/
#ifndef ICF_LOOPBACK_SUPPORT
    /* Loop detection - check that the Via header does not contain local addr */
    if (Sdf_en_discarded == 
            icf_ssa_validate_via(p_ssa_pdb, *ppCallObj, pErr))
    {
        p_ssa_ctxt->failure_bitmask |= 
            ICF_SSA_FAILURE_RESP_CODE_482;
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }
#endif
#endif

      ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_INVITE)

         
      /* this is the code for sending the 414 for long URI in INVITTE */ 
      if((p_ssa_ctxt->bitmask_1 & ICF_REQ_URI_TOO_LONG)==ICF_REQ_URI_TOO_LONG)
      {
        p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_414;
        ret_val = Sdf_co_fail;
        
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
      }
 
    /* Check that the Expires is not 0 */
    if (ICF_FAILURE == icf_ssa_validate_expires(
                              p_ssa_pdb,
                              *ppCallObj,
                              &validity,
                              pErr))
    {
        /* Expires header contains value 0 */
        if (Sdf_en_invalid == validity)
        {
            p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_487;
        }
        else
        {
            p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_500;
        }
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }

    /* Check whether session handling is required */
    if (ICF_FAILURE == icf_ssa_validate_content_disp(
		p_ssa_pdb,*ppCallObj,pErr))
    {
	if (ICF_SSA_FAILURE_RESP_CODE_488 != 
		(ICF_SSA_FAILURE_RESP_CODE_488 & p_ssa_ctxt->failure_bitmask))
        {
            p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_500;
        }
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }

     /* Accept Header Handling */
     if(Sdf_co_fail == icf_ssa_parse_AcceptHdr_for_supported_Msgbody(
                               (*ppCallObj)->pUasTransaction->pSipMsg,
                               p_ssa_pdb->p_glb_pdb,
                               pErr))
               {
                    p_ssa_pdb->p_ssa_ctxt->failure_bitmask |=
                                    ICF_SSA_FAILURE_RESP_CODE_400;
                    return Sdf_co_fail;
               }

      
    /* Cast the payload into the icf_nw_inc_call_setup_st structure */
    p_internal_msg_param = 
        (icf_nw_inc_call_setup_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    p_internal_msg_param->bitmask = ICF_NULL;
    p_cfg_data = p_ssa_pdb->p_glb_cfg_data ;

    /* Check for the presence of Join header */
    /* Check for the presence of Replaces header */
    if (ICF_SUCCESS == icf_ssa_isHeaderPresent(
                (*ppCallObj)->pUasTransaction->pSipMsg,
                SipHdrTypeReplaces,
                pErr))
    {
        /* Invoke utility function to parse the Replaces header */
        if (ICF_FAILURE == icf_ssa_parse_replaces_header(
                    p_ssa_pdb,pErr))
        {
            ret_val = Sdf_co_fail;
        }
    } /* pErr)) */

    if (Sdf_co_fail != ret_val)
    {
        icf_call_id_t   call_id = ICF_INVALID_CALL_ID;
        /* 
         * Allocate call context by making request to the DBM
         */
        if(ICF_FAILURE == icf_dbm_alloc_call_cntxt_blk(
                    p_ssa_pdb->p_glb_pdb,
                    &call_id,
                    &p_call_ctx,
                    p_ecode))
        {
            /*
             * Set bitmask to send 503(insufficient resources) to peer
             * as the DBM is unable to allocate a new call context 
             */ 
            p_ssa_ctxt->failure_bitmask |= 
                ICF_SSA_FAILURE_RESP_CODE_503;
            ret_val = Sdf_co_fail;
            return ret_val;
        }

        if (ICF_SUCCESS == icf_ssa_isHeaderPresent(
                    (*ppCallObj)->pUasTransaction->pSipMsg,
                    SipHdrTypeJoin,
                    pErr))
        {
            /*Invoke utility function to parse the Join header */
            if (ICF_FAILURE == icf_ssa_validate_join_header_and_get_call_id(
                        p_ssa_pdb,  &join_call_id, call_id))
            {
                icf_dbm_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        call_id,
                        p_ecode);
                return Sdf_co_fail;
            }
            else
                join_header_present = ICF_TRUE;
        }/*End of icf_ssa_isHeaderPresent */
        /*
         * The SSA context is now allocated before invocation of UATK
         * HandleCall API on reception of a new SIP request.  The
         * pointer to the same is passed as Event Context
         */

        /* Storing the pointer to SSA context in call context */
        p_call_ctx->p_sip_ctx = p_ssa_ctxt;
        /* Storing the pointer to call context in SSA context */
        p_ssa_ctxt->p_call_ctxt = p_call_ctx;
        /* Storing the call object pointer in SSA context */
        p_ssa_ctxt->p_call_obj = *ppCallObj;
        /* Storing the call ctx ptr in the p_glb_pdb */
        p_ssa_pdb->p_glb_pdb->p_call_ctx = p_call_ctx;
        if (ICF_TRUE == join_header_present)
        {
            p_call_ctx->call_to_be_joined = join_call_id;
            ICF_PRINT(((icf_uint8_t *)"Joining call id is %lu", join_call_id));
        }
        else
        {
            p_call_ctx->call_to_be_joined = ICF_INVALID_CALL_ID;
            ICF_PRINT(((icf_uint8_t *)"Joining call id is INVALID"));
        }

        if(ICF_SUCCESS == ret_val)
        {
          if (ICF_SUCCESS == icf_ssa_check_isfocus_in_contact_header(p_ssa_pdb))
          {
                 p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 |=
                     ICF_CC_INCOMING_CONFERENCE_REQ;
           if (p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str_len
                   == 0 )
            {    /*SPR15999:- Using single bitmask set here for identifying
                   the incoming xconf call and using the same in
                   icf_cc_send_pa_connect_resp*/ 
                 p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri = 
                       p_ssa_pdb->conference_uri;
                 ICF_PRINT(((icf_uint8_t *)"ISFOCUS conferencr URI = %s",
                 p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str))
            }
          }
        }

        /* 
         * Store the replaces call ID in the newly allocated
         * call context if INVITE was received with replaces
         * call ID 
         */
        if (ICF_NULL != ICF_SSA_GET_COMMON_BITMASK(
                    p_internal_msg_param->bitmask,
                    ICF_NW_INC_SETUP_RELACE_CALL_ID))
        {
            p_ssa_ctxt->p_call_ctxt->replace_initiated_call_id =
                p_internal_msg_param->replace_call_id;

			 /* also set the call_direction */
            p_ssa_ctxt->p_call_ctxt->common_bitmask |= ICF_CALL_DIR;
        }
        
		
	/*Following routine checks for diversion info.Returns failure
         * on some failure in UATK processing.  If nothing fails and 
         * it is able to * get diversion data from the given message, 
         * it allocates memory for diversion info and sets the pointer
         * for the same in call ctxt
         */
        if ( ICF_FAILURE == icf_ssa_populate_div_hdr(p_ssa_pdb,
                    (*ppCallObj)->pUasTransaction->pSipMsg))
        {
            ICF_PRINT((
                       (icf_uint8_t *)"\nFailure in 302 handling in diversioninfo retrieval"));
            p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list = 
                ICF_NULL;
            /* De allocate call context */
            icf_dbm_dealloc_call_cntxt_blk(
                    p_ssa_pdb->p_glb_pdb,
                    p_call_ctx->call_id,
                    p_ecode);

            p_ssa_ctxt->p_call_ctxt = ICF_NULL;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return Sdf_co_fail;
        }

        /* Referred-by header processing */
        if (ICF_SUCCESS == icf_ssa_isHeaderPresent(
                    (*ppCallObj)->pUasTransaction->pSipMsg,
                    SipHdrTypeReferredBy,
                    pErr))
        {
            if (ICF_FAILURE == icf_ssa_parse_ref_by_header(
                        p_ssa_pdb,pErr))
            {
                /* De allocate call context */
                icf_dbm_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx->call_id,
                        p_ecode);
            p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_fail;
            }
        }

/* Precondition: Merged from ICF_QOS_PRE_M_LINE_MERGE */
        /* Require header processing */
        if (ICF_SUCCESS == icf_ssa_isHeaderPresent(
                    (*ppCallObj)->pUasTransaction->pSipMsg,
                    SipHdrTypeRequire,
                    pErr))
        {
            if (ICF_FAILURE == icf_ssa_parse_require_header(
				    p_ssa_pdb,
				    (*ppCallObj)->pUasTransaction->pSipMsg,
				    pErr))
            {
                if ((ICF_CAUSE_SDP_MISSING_FOR_PRECONDITION == pErr->errCode) || 
                    (ICF_SSA_REQUIRE_VALUE_IS_NOT_SUPPORTED & p_ssa_pdb->common_bitmask))
				{
					if (Sdf_co_fail == 
						sdf_ivk_uaFormResponse(420, "INVITE", 
										p_ssa_pdb->p_ssa_ctxt->p_call_obj,
										Sdf_co_null,
										Sdf_co_false,
										pErr))
					{
						/*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
						ret_val = ICF_FAILURE;
					}
					else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
						p_ssa_pdb->p_ssa_ctxt->p_call_obj,
						p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
						ICF_NULL,pErr))
					{
						/*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
						ret_val = ICF_FAILURE;
					}
					else
					{
   						ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
   						return ret_val;
					}				
				
				}
				else
				{
					p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_403;
					/* De allocate call context */
					icf_dbm_dealloc_call_cntxt_blk(
							p_ssa_pdb->p_glb_pdb,
							p_call_ctx->call_id,
							p_ecode);
                                        p_ssa_ctxt->p_call_ctxt = ICF_NULL;
					ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
					return Sdf_co_fail;
				}
            }
        }
        /* Check if UPDTAE is received in the ALLOW header.
         * if UPDATE is received and in config data application has set
         * call modify method as UPDATE then in SSA Ctxt we will set refresher
         * method as UPDATE for incoming call otherwise we will set it as INVITE.
         */
        if (Sdf_co_success == 
            icf_ssa_is_update_present_in_allow_hdr((*ppCallObj)->pUasTransaction->pSipMsg,
                                       p_ssa_pdb->p_glb_pdb,
                                       &(p_ssa_pdb->p_ssa_ctxt),
                                       pErr))
        {
           /*  Fix for CSR 1-7407430 */
           /*  If call context is NULL then clear the call context 
            *  locally available and return failure
            */
           if(ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)
           {
                /* De allocate call context */
                icf_dbm_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx->call_id,
                        p_ecode);
               ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Call Context is NUll in ssa Context"));

               ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
               return Sdf_co_fail;
           }
           if ((ICF_UPDATE_PRESENT_IN_ALLOW_HDR == 
                p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->allowed_value) && \
               (ICF_CFG_CALL_MODIFY_METHOD_UPDATE == p_cfg_data->call_modify_req_method))
           {
               p_ssa_pdb->p_ssa_ctxt->refresher_method_type = Sdf_en_refresherUpdate;
               
           }
           else
           {
               p_ssa_pdb->p_ssa_ctxt->refresher_method_type = Sdf_en_refresherInvite;
           }
        }
        /* Supported header processing */
        if (ICF_SUCCESS == icf_ssa_isHeaderPresent(
                    (*ppCallObj)->pUasTransaction->pSipMsg,
                    SipHdrTypeSupported,
                    pErr))
        {
            if (ICF_FAILURE == icf_ssa_parse_supp_header(
                        p_ssa_pdb,
                        (*ppCallObj)->pUasTransaction->pSipMsg,
                        pErr))
            {
                /* De allocate call context */
                icf_dbm_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx->call_id,
                        p_ecode);
                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_fail;
            }
        }
/* 6_4_Nortel Branch merge */
#if 0
        else
        {
            /*
             * Even in the absence of Supported header
             * set the bit indicating the support of reliable
             * provisional responses by peer. Absence of Supported header 
             * and the presence of 100 rel in the Supported header 
             * are considered equivalent
             */
            p_call_ctx->common_bitmask |=
                ICF_REL_PROV_RESP_SUPP_BY_PEER;
        }
#endif
        /* Check for the presence of SDP in the INVITE only if 
         * the flag ICF_SSA_SDP_TO_BE_IGNORED is not set via the 
         * function icf_ssa_validate_content_dsip
         */
        if((!(ICF_SSA_SDP_TO_BE_IGNORED & p_ssa_pdb->common_bitmask)) &&
           (Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent(
                (*ppCallObj)->pUasTransaction->pSipMsg, pErr))) 
        {
#ifdef ICF_TRACE_ENABLE
		p_ssa_pdb->sdp_present = ICF_TRUE;
#endif
            /*
             * SSA is not using any media handling related 
             * functionality provided by the UATK.
             * On receiving SDP in any SIP message,SSA
             * converts the SDP into the generic SDP structure
             * of the call context and then either populates
             * the p_remote_received_sdp structure of the call
             * context or passes this SDP in the internal message
             * Since the SDP structure of the call context doesn't
             * have a mode field, SSA will be calling the SDP
             * conversion function with the pointer to the mode 
             * variable defined below.The SDP conversion function 
             * will extract the mode from the SDP and store
             * it into this variable.
             */

			 /*Initialize the mode as sendrecv. This will be the mode received in 
			  * SDP.This is used as a backup when P-Early-Media header is received
			  */
			 p_call_ctx->recvd_media_attrib_backup = ICF_MEDIA_MODE_SEND_RECV; 

            if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                    p_ssa_pdb,
                                    (*ppCallObj)->pUasTransaction->pSipMsg,
                                    ICF_SSA_INCOMING_INVITE,
				     &p_inc_media_sdp_info,
                                    p_ecode))
            {
#ifdef IMS_CLIENT
                /* If failure is due to preconditions processing
                 * it might be possible to send different
                 * response code.
                 * TBD
                 */
#endif
				if (ICF_FALSE == ICF_IS_BIT_SET(p_ssa_ctxt->failure_bitmask,
												ICF_SSA_FAILURE_RESP_CODE_488))
				{
                	p_ssa_ctxt->failure_bitmask |= 
                    	ICF_SSA_FAILURE_RESP_CODE_415;
				}

                if(ICF_NULL != p_inc_media_sdp_info)
                {
                    /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                    icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                        &p_inc_media_sdp_info);
                }

                /* De allocate call context and its contents*/
                icf_ssa_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx,
                        p_ecode);

                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                ret_val = Sdf_co_fail;
            }
#ifdef ICF_KLOCWORK_TEST
			else if(ICF_NULL == p_inc_media_sdp_info)
			{
				if (ICF_FALSE == ICF_IS_BIT_SET(p_ssa_ctxt->failure_bitmask,
                                                ICF_SSA_FAILURE_RESP_CODE_488))
                {
                    p_ssa_ctxt->failure_bitmask |=
                        ICF_SSA_FAILURE_RESP_CODE_415;
                }

				 /* De allocate call context and its contents*/
                icf_ssa_dealloc_call_cntxt_blk( 
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx,
                        p_ecode);

                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                ret_val = Sdf_co_fail;
			}
#endif
            /* 
             * Update the p_remote_received_sdp  and
             * rem_media_mode fields of the call
             * context
             */
            else if (ICF_FAILURE == icf_ssa_create_stream_sdp_info_in_cctx(
                                        p_ssa_pdb,
                                        p_inc_media_sdp_info->p_sdp_info_list,
                                        p_ssa_pdb->p_glb_pdb->p_call_ctx))
            {
                /* Free the acquired memory for the SDP */
                p_ssa_ctxt->failure_bitmask |= 
                    ICF_SSA_FAILURE_RESP_CODE_500;

                 /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);

                /* De allocate call context and its contents*/
                icf_ssa_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx,
                        p_ecode);

                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                ret_val = Sdf_co_fail;
            }
            else if (ICF_FAILURE == icf_ssa_create_stream_sdp_info_in_cctx(
                                        p_ssa_pdb,
                                        p_inc_media_sdp_info->\
                                        p_nw_reject_stream_list,
                                        p_ssa_pdb->p_glb_pdb->p_call_ctx))
            {
                /* Free the acquired memory for the SDP */
                p_ssa_ctxt->failure_bitmask |=
                    ICF_SSA_FAILURE_RESP_CODE_500;

                 /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);

                /* De allocate call context and its contents*/
                icf_ssa_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx,
                        p_ecode);

                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                ret_val = Sdf_co_fail;
            }

			/*SPR 19775 - Here in the above code since we have alredy populated the NW
             *rejected streams in call context ,after which 
             *p_nw_reject_stream_list has been deleted ,Calling the same
             *function i.e. icf_ssa_create_stream_sdp_info_in_cctx with same
             *set of parameters will result in FMR and subsequently crash.
			 *the code deleted for another invocation of
			 * icf_ssa_create_stream_sdp_info_in_cctx
             */
            else
            {
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                            p_inc_media_sdp_info,
                	    ICF_MEM_COMMON,
                	    p_ssa_pdb->p_ecode,
                	    ret_val)

                /* Free the acquired memory for the SDP */
                icf_cmn_delete_list(
                    p_ssa_pdb->p_glb_pdb,
                    &p_media_sdp_info,
                    ICF_MEM_COMMON);
                /* 
                 * Set the bit indicating that this is
                 * a Fast Start Call(INVITE with SDP)
                 * in the common_bitmask field of the 
                 * call context
                 */
                ICF_SSA_SET_COMMON_BITMASK(p_call_ctx->common_bitmask,
                        ICF_CALL_MEDIA_SETUP_TYPE)
            }
        }
		 /* Extract the message body other than SDP from SIP message.
         * In the case where we are ignoring the SDP body present
         * for ETSI conformance fixes,this code leg would get hit
         * because the bit ICF_SSA_SDP_TO_BE_IGNORED is set by the
         * function icf_ssa_validate_content_disp. However the function
         * called below simply ignores that SDP.
         */	
		else
		{
          ret_val = icf_ssa_is_update_present_in_allow_hdr((*ppCallObj)->pUasTransaction->pSipMsg,
                                p_ssa_pdb->p_glb_pdb,
                                &p_ssa_ctxt,
                                pErr);

            p_ssa_ctxt->call_type  = ICF_CALL_TYPE_INVALID;

			if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
			(*ppCallObj)->pUasTransaction->pSipMsg,SipMessageRequest,\
			&(p_ssa_ctxt->p_call_ctxt->\
						body_list)))
		       {
		        	ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
			        icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb, \
						call_id, p_ssa_pdb->p_ecode);
         			ret_val = ICF_FAILURE;
		        }
		}
		/* Changes for Message body header ends
		 */
		

		if (Sdf_co_fail != ret_val)
    {
        p_call_ctx->app_id = p_ssa_pdb->p_app_conf->app_id;

	/* get the application specific headers from dbm.      
	* These headers shall be filled in call context based 
	* on the app_id stored in call ctx. Currently use app id = 1
	*/
		if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
			p_ssa_pdb,ICF_INT_MODULE_CC, 
			(icf_void_t*)(p_call_ctx),
			(*ppCallObj)->pUasTransaction->pSipMsg,
			p_ecode) )
		{
			p_ssa_ctxt->failure_bitmask |= 
				ICF_SSA_FAILURE_RESP_CODE_500;
			/*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/

				 /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
                /* This is to free memory allocated in
                 * icf_ssa_create_sdp_info_incctx*/ 
                if(ICF_NULL != p_call_ctx->p_stream_sdp_info)
                {   
                     icf_cmn_free_stream_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                                                       &(p_call_ctx->p_stream_sdp_info),
                                                       p_ecode);
                }

				/* De allocate call context and its contents*/
				icf_ssa_dealloc_call_cntxt_blk(
					p_ssa_pdb->p_glb_pdb,
					p_call_ctx,
					p_ecode);
				
                                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
				ret_val = Sdf_co_fail;
		}
	}

#ifdef ICF_QOS_SUPPORT
#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
	/* Extract P-Media-Authorization Header */
    if (Sdf_co_fail != ret_val)
	{
		if (ICF_FAILURE == icf_ssa_extract_media_authorization_token(
			p_ssa_pdb, (*ppCallObj)->pUasTransaction->pSipMsg))
		{
			p_ssa_ctxt->failure_bitmask |= 
				ICF_SSA_FAILURE_RESP_CODE_415;
			/*ICF_SSA_TRACE(("\nFailed to get P-Media-Authorization header"))*/

            /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                &p_inc_media_sdp_info);
            if(ICF_NULL != p_call_ctx->p_stream_sdp_info)
            {   
                 icf_cmn_free_stream_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                                                   &(p_call_ctx->p_stream_sdp_info),
                                                   p_ecode);
            }    

            icf_cmn_free_header_list(p_ssa_pdb->p_glb_pdb,
                                     &(p_call_ctx->header_list),
                                     p_ecode);

			/* De allocate call context and its contents*/
			icf_ssa_dealloc_call_cntxt_blk(
				p_ssa_pdb->p_glb_pdb,
				p_call_ctx,
				p_ecode);
			
                        p_ssa_ctxt->p_call_ctxt = ICF_NULL;
			ret_val = Sdf_co_fail;
		}
	}
ICF_CHECK_IMS_END
ICF_CHECK_QOS_SUPPORT_END
#endif
#endif
#endif
        /*
         * Construct message to send to CM with 
         * msg type = ICF_NW_INC_CALL_SETUP 
         */
        if (Sdf_co_fail != ret_val)
        {
            if (ICF_FAILURE == icf_ssa_parse_n_validate_requri(
                        p_ssa_pdb,pErr))
            {
		/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
                /* This is to free memory allocated in
                 * icf_ssa_create_sdp_info_incctx*/ 
                if(ICF_NULL != p_call_ctx->p_stream_sdp_info)
                {   
                     icf_cmn_free_stream_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                                                       &(p_call_ctx->p_stream_sdp_info),
                                                       p_ecode);
                }
                /*This is for freeing memory allocated in
                 * icf_ssa_populate_app_specific_hdr function above */

                icf_cmn_free_header_list(p_ssa_pdb->p_glb_pdb,
                                         &(p_call_ctx->header_list),
                                         p_ecode);

                /* De allocate call context */
                /*CSR-5213984 */
                /*icf_dbm_dealloc_call_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        p_call_ctx->call_id,
                        p_ecode);*/
         
                icf_ssa_dealloc_call_cntxt_blk(
						p_ssa_pdb->p_glb_pdb,
						p_call_ctx,
						p_ecode);
                p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_400;
                /*Fix for CSR 7945729 SPR 20251*/
                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                p_ssa_pdb->p_glb_pdb->p_call_ctx = ICF_NULL;

                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_fail ;
            }          
            
            /*
             * If calling party address cannot be 
             * ascertained from the p-asserted identity header
             * extract the same from From header
             */

            if (ICF_FAILURE == 
                    icf_ssa_parse_unknown_header_like_from_hdr(
                        p_ssa_pdb,
                        (*ppCallObj)->pUasTransaction->pSipMsg,
                        "P-Asserted-Identity",
                        &p_assert_hdr))

            {
			    /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
                
                /* This is to free memory allocated in
                 * icf_ssa_create_sdp_info_incctx*/ 
                if(ICF_NULL != p_call_ctx->p_stream_sdp_info)
                {   
                     icf_cmn_free_stream_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                                                       &(p_call_ctx->p_stream_sdp_info),
                                                       p_ecode);
                }
                /*This is for freeing memory allocated in
                 * icf_ssa_populate_app_specific_hdr function above */

                icf_cmn_free_header_list(p_ssa_pdb->p_glb_pdb,
                                         &(p_call_ctx->header_list),
                                         p_ecode);

				/* De allocate call context and its contents*/
				icf_ssa_dealloc_call_cntxt_blk(
						p_ssa_pdb->p_glb_pdb,
						p_call_ctx,
						p_ecode);

                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                /*5227179*/
                p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_400;  
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_fail ;
            }

            /*
             * Extract P-CALLED-PARTY-ID header and populate 
             * called_party_addr in call context.
             */

            if (ICF_FAILURE == 
                    icf_ssa_parse_unknown_header_like_from_hdr(
                        p_ssa_pdb,
                        (*ppCallObj)->pUasTransaction->pSipMsg,
                        "P-Called-Party-ID",
                        &p_called_hdr))
            {
				/*ICF_SSA_TRACE(("\nUnable to parse P-called-party-ID hdr"))*/

			    /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
                
                /* This is to free memory allocated in
                 * icf_ssa_create_sdp_info_incctx*/ 
                if(ICF_NULL != p_call_ctx->p_stream_sdp_info)
                {   
                     icf_cmn_free_stream_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                                                       &(p_call_ctx->p_stream_sdp_info),
                                                       p_ecode);
                }
                /*This is for freeing memory allocated in
                 * icf_ssa_populate_app_specific_hdr function above */
                sip_freeSipHeader(p_assert_hdr);

                ICF_MEMFREE(
                           p_ssa_pdb->p_glb_pdb,
                           p_assert_hdr,
                           ICF_MEM_COMMON,
                           p_ssa_pdb->p_ecode,
                           ret_value)


                icf_cmn_free_header_list(p_ssa_pdb->p_glb_pdb,
                                         &(p_call_ctx->header_list),
                                         p_ecode);

				/* De allocate call context and its contents*/
				icf_ssa_dealloc_call_cntxt_blk(
						p_ssa_pdb->p_glb_pdb,
						p_call_ctx,
						p_ecode);

                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_fail ;
            }
            if (ICF_FAILURE == 
                    icf_ssa_parse_privacy_header(
                        p_ssa_pdb,*ppCallObj))
            {
                if ((ICF_ERROR_UATK_FAILURE == *(p_ssa_pdb->p_ecode))) 
                {
			     /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
                
                /* This is to free memory allocated in
                 * icf_ssa_create_sdp_info_incctx*/ 
                if(ICF_NULL != p_call_ctx->p_stream_sdp_info)
                {   
                     icf_cmn_free_stream_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                                                       &(p_call_ctx->p_stream_sdp_info),
                                                       p_ecode);
                }
                /*This is for freeing memory allocated in
                 * icf_ssa_populate_app_specific_hdr function above */
                sip_freeSipHeader(p_called_hdr);
                sip_freeSipHeader(p_assert_hdr);

                ICF_MEMFREE(
                           p_ssa_pdb->p_glb_pdb,
                           p_called_hdr,
                           ICF_MEM_COMMON,
                           p_ssa_pdb->p_ecode,
                           ret_value)
                ICF_MEMFREE(
                           p_ssa_pdb->p_glb_pdb,
                           p_assert_hdr,
                           ICF_MEM_COMMON,
                           p_ssa_pdb->p_ecode,
                           ret_value)



                icf_cmn_free_header_list(p_ssa_pdb->p_glb_pdb,
                                         &(p_call_ctx->header_list),
                                         p_ecode);

				/* De allocate call context and its contents*/
				icf_ssa_dealloc_call_cntxt_blk(
						p_ssa_pdb->p_glb_pdb,
						p_call_ctx,
						p_ecode);

                    p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return Sdf_co_fail ;
                }
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:privacy header not present"));
                }
            }
            else
            {
                /*
                 * Set the common bitmask to indicate the presence of
                 * the privacy header
                 */
                p_call_ctx->common_bitmask |= ICF_PRIVACY_HDR_PRESENT;
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:privacy header present"));
            }
            /*
             * If p_assert_header is not NULL,
             * it means that the p-asserted identity header 
             * was present and successfully parsed.
             * In this case we re-assign the pointer p_from
             * to p_asserted as both are of the same type.
             * Otherwise we proceed to extract the calling
             * party address from From header
             */
            if (SIP_NULL != p_assert_hdr)
            {
                p_from = (SipFromHeader *)
                    (p_assert_hdr->pHeader);
                /*
                 * Set the bit remote_aserted_address
                 * indicating that this is an asserted 
                 * address
                 */
                p_call_ctx->remote_aserted_address = ICF_TRUE;

				/* allocate memory for asserted identity, will be freed by CC */
                ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
                        ICF_PORT_SIZEOF(icf_address_st),
                        ICF_MEM_COMMON, 
                        p_call_ctx->p_asserted_identity,
                        ICF_DONOT_RET_ON_FAILURE, p_ssa_pdb->p_ecode, ret_val)
				if ( ICF_FAILURE == ret_val)				
				{
					sip_freeSipHeader(p_assert_hdr);
					ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_assert_hdr,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            ret_value)
				}
				else
				{

					icf_ssa_populate_address_from_sip_addr_spec(
						p_ssa_pdb,p_from->pAddrSpec,
						p_call_ctx->p_asserted_identity);
				}

            }

            if (SIP_NULL != p_called_hdr)
            {
                /*
                 * Set the value in call context
                 * indicating that this is called party  
                 * address
                 */
                 p_temp = (SipFromHeader *)(p_called_hdr->pHeader);
				 ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
                        ICF_PORT_SIZEOF(icf_address_st),
                        ICF_MEM_COMMON, 
                        p_call_ctx->p_called_party_id,
                        ICF_DONOT_RET_ON_FAILURE, p_ssa_pdb->p_ecode, ret_val)

				 if ( ICF_SUCCESS == ret_val)
				 {

					icf_ssa_populate_address_from_sip_addr_spec(
                    p_ssa_pdb,p_temp->pAddrSpec,
                    p_call_ctx->p_called_party_id);
				 }
				 
					/* free the associated memory for called hdr */
                    sip_freeSipHeader(p_called_hdr);
                    ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_called_hdr,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            ret_value)				 
            }
#if 0
			/* free the memory and return failure */
			if (Sdf_co_fail == ret_val)
			{
                if (ICF_NULL != p_sdp_data)
                {
                    icf_dbm_free_sdp_block(
                            p_ssa_pdb->p_glb_pdb,
                            (icf_void_t *)p_sdp_data,
                            p_ssa_pdb->p_ecode);
                }
				if (ICF_NULL != p_call_ctx->p_asserted_identity)
				{
                   ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_call_ctx->p_asserted_identity,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            ret_value)	
				}
				/* De allocate call context and its contents*/
					icf_ssa_dealloc_call_cntxt_blk(
							p_ssa_pdb->p_glb_pdb,
							p_call_ctx,
							p_ecode);
                                        p_ssa_ctxt->p_call_ctxt = ICF_NULL;

                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_fail ;
            }	
#endif

            if (ICF_SUCCESS == ret_val)
			{
#ifdef ICF_WITH_MUATK_2_2
				if (SIP_NULL != p_from->pStr1)
				{
                    /*If the string length of p_from is greater than
                     * 128 bytes we only copy the first 127 bytes into
                     * remote_party_name
                     */ 
                    if (icf_port_strlen((icf_uint8_t *)p_from->pStr1) >= ICF_MAX_STR_LEN)
                    {
                        icf_port_strncpy(
                            (icf_int8_t *)p_call_ctx->remote_party_name.str,
                            p_from->pStr1,ICF_MAX_STR_LEN - 1);
                        p_call_ctx->remote_party_name.str[\
                        ICF_MAX_STR_LEN - 1] = '\0';  
                    }        
                    else
                    {    
					    icf_port_strcpy(
                            p_call_ctx->remote_party_name.str,
                            (icf_uint8_t *)p_from->pStr1);
                    }    
#else
				if (SIP_NULL != p_from->pDispName)
				{
                    else
					    icf_port_strcpy(
                            p_call_ctx->remote_party_name.str,
                            p_from->pDispName);
#endif
					p_call_ctx->remote_party_name.str_len =
						(icf_uint16_t)icf_port_strlen(
						p_call_ctx->remote_party_name.str);
						/* 
						* Set the bit in common bitmask of call ctx
						* indicationg the presence of display name
					*/
					p_call_ctx->common_bitmask |= 
						ICF_REMOTE_NAME_PRESENT;
				}
				/* 
				* Now populate the remote address 
				* Here we are not checking the retval as
				* it is acceptable to receive a SIP URI
				* of the form sip:131349314 in the 
				* From/p-asserted identity header
				* wheras it is not acceptable in the case
				* of the Request URI
				*/
				icf_ssa_populate_address_from_sip_addr_spec(
                    p_ssa_pdb,p_from->pAddrSpec,
                    &(p_call_ctx->remote_party_addr));
				if (SIP_NULL != p_assert_hdr)
				{
					sip_freeSipHeader(p_assert_hdr);
					ICF_MEMFREE(
						p_ssa_pdb->p_glb_pdb,
						p_assert_hdr,
						ICF_MEM_COMMON,
						p_ssa_pdb->p_ecode,
						ret_val)
				}
				/* 
				* Extract the common info from the call object
				* The common info stores a lot of common information
				* about the call object,for e.g the network call ID,
				* the From and To information etc.
				* Here,we are extarcting the call ID for adding 
				* the to the global call ID - SSA context map
				*/
				if (Sdf_co_fail == sdf_ivk_uaGetCommonInfoFromCallObject(
					*ppCallObj,&p_common_info, pErr))
				{
					/*ICF_SSA_TRACE(("\nFailed to get common infofrom call object")) */
						ret_val = Sdf_co_fail;
				}
				/*Add the SSA ctxt to CallId mapping in the SSA DB*/
				else if (ICF_FAILURE == icf_ssa_add_callid_callobj_map(
					p_ssa_pdb,					
					p_call_ctx->p_sip_ctx,
					p_ecode))
				{
					/* Free the common info */
					sdf_ivk_uaFreeCommonInfo(p_common_info);
					/* 
					* Free the acquired memory for the SDP if this
					* is a fast start call
					*/
                    /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                    icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                        &p_inc_media_sdp_info);

					/* De allocate call context and its contents*/	
				    icf_ssa_dealloc_call_cntxt_blk(
						p_ssa_pdb->p_glb_pdb,
						p_call_ctx,
						p_ecode);
                                    p_ssa_ctxt->p_call_ctxt = ICF_NULL;
					
						ret_val = Sdf_co_fail;
				}
				else
				{
					/* Free the common info */
					sdf_ivk_uaFreeCommonInfo(p_common_info);
					/*
					* Store SSA context in the pAppData
					* member of the call object.
					* This is done to facilitate the fetching 
					* of the matching call context when any message
					* is received from the mnetwork
					*/
					p_app_data = (*ppCallObj)->pAppData ;
					/* 
					* Delete the message body list stored in the UAS
					* transaction of the call object.
					* This is done to flush the SDP body from the 
					* transaction substructure of the call object
					*/
					sdf_listDeleteAll(
                        &((*ppCallObj)->pUasTransaction->slMsgBodyList),
                        pErr);
					
					/* Store SSA context in pAppData of call object */
					p_app_data->pData = (Sdf_ty_pvoid *)p_ssa_ctxt;
					
					/*
					* All messages to be sent to CC/SC are
					* routed to the CM.CM decides the specific 
					* destination of these messages
					*/
					p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
					
					/* Populate the internal message header */
					p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
						ICF_NW_INC_CALL_SETUP;
					p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
						ICF_PORT_SIZEOF(icf_nw_inc_call_setup_st);
#ifdef ICF_TRACE_ENABLE
					 if(ICF_SUCCESS ==  icf_ssa_create_criteria_data(p_ssa_pdb))
                     { 
					
					     icf_ssa_capture_trace(p_ssa_pdb,
						    (*ppCallObj)->pUasTransaction->pSipMsg,1);
					     ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_INVITE_RECVD)
					 }	 
#endif
				}
			}
			else
			{
                /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);

				if (ICF_NULL != p_call_ctx->p_asserted_identity)
				{
					ICF_MEMFREE(
						p_ssa_pdb->p_glb_pdb,
						p_call_ctx->p_asserted_identity,
						ICF_MEM_COMMON,
						p_ssa_pdb->p_ecode,
						ret_value)
						
					p_call_ctx->p_asserted_identity = ICF_NULL;
				}
				if (ICF_NULL != p_call_ctx->p_called_party_id)
				{
					ICF_MEMFREE(
						p_ssa_pdb->p_glb_pdb,
						p_call_ctx->p_called_party_id,
						ICF_MEM_COMMON,
						p_ssa_pdb->p_ecode,
						ret_value)
					p_call_ctx->p_called_party_id = ICF_NULL;
				}

				/* De allocate call context and its contents*/
				icf_ssa_dealloc_call_cntxt_blk(
					p_ssa_pdb->p_glb_pdb,
					p_call_ctx,
					p_ecode);
                                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
			}
        } /* else */
    } /* else */
#if 0
    if (Sdf_co_fail == ret_val) 
    {
         if(ICF_SSA_FAILURE_RESP_CODE_415 == (p_ssa_ctxt->failure_bitmask &
                        ICF_SSA_FAILURE_RESP_CODE_415))
         {
            if (ICF_FAILURE == icf_ssa_form_and_send_response(
                        p_ssa_pdb,415,
                        *ppCallObj,
                        ICF_NULL, 
                         "INVITE", Sdf_co_false))
            {
                ret_val = Sdf_co_fail;
            }
         }
    }
#endif

    /*ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)  function exit trace */
    return ret_val;
} /* if (Sdf_co_fail == ret_val) */

/*****************************************************************************
 ** FUNCTION:           sdf_cbk_uaFailedCallAcknowledged
 **
 ** DESCRIPTION:        Callback issued to the application on the receipt of an
 **                     ACK message to the final failure message sent
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaFailedCallAcknowledged(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal           ret_val=Sdf_co_success;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* To remove compiler warning */
    (void)ppCallObj;    
    (void)pErr;    

    /* Populate the internal message header */
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CALL_REJECT_ACK;
    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;

        /*This indicates to the Entry pt code ( processing otg internal msg)
         * that the current internal msg will be the last msg from this
         * dialog to App. After this the bitmask in SSA CTXT will be set
         * to indicate that no further messages are to be sent to App*/
        ICF_SSA_SET_COMMON_BITMASK(p_ssa_pdb->common_bitmask,\
                ICF_SSA_AWAITING_SSA_CTXT_DEL)
    /*
     * All messages to be sent to CC/SC are routed to the CM.
     * CM decides the specific destination of these messages
     * If this ACK is corresponding to the failure response
     * sent from entrypoint.c because of some failure
     * in sdf_cbk_uaNewCallReceived,then we don't want to invoke
     * CC,hence do not set the destination module ID
     */
    if (ICF_NULL == p_ssa_ctxt->failure_bitmask)
    {
        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
    }
	/* this call has been removed from UpdateCallDetails and added here*/
	sdf_listDeleteAll(&((*ppCallObj)->pUasTransaction->slUnprocessedHeaders), \
                        pErr);
   /*CSR1-5310156 SPR 17555 Fix: Send call direction to app_trace for SIP PDUs
    */
#ifdef ICF_TRACE_ENABLE
    p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
#endif
    ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)

    /* Fix for CSR 5290548 */
    /*This bitmask is used to prevent trace from icf_ssa_entrypoint.c,
     *as the trace is raised from here. This is done because in case of TCP
     *p_ssa_ctxt is freed from here only and will not be available there*/
    ICF_SSA_SET_COMMON_BITMASK(
                    p_ssa_pdb->common_bitmask,
                    ICF_SSA_TRACE_REQUIRED)


    /*Fix for CSR 1-7407430 : SPR-19773*/
    /* This code is added to handle the following scenario.
     * 1) IPTK receives INVITE from peer.
     * 2) IPTK sends failure response (3xx/4xx/5xx).
     * 3) IPTK receives ACK.
     * 4) IPTK receives the another INVITE with same From tag and call-id
     *    as received in earlier INVITE within 32 seconds.
     * 
     * Issue:
     * IPTK try to fetch the SSA context from the list based on hash key
     * generated by using From tag, to tag and call-id.
     *
     * In above scenario IPTK gets the SSA context created for first INVITE
     * after receiving second INVITE and trying to process the second INVITE
     * which eventually gets FAILURE.
     *
     * Fix Description:
     * To handle the above mentioned issue below code has been added which
     * removes the SSA context from list after receiving ACK for failure response.
     */   
    icf_ssa_remove_ctxt_list_entry(p_ssa_pdb->p_glb_pdb,
                   p_ssa_pdb->p_ssa_ctxt);
    /* Fix for CSR 1-7644298 */
    sdf_ivk_uaFreeCallObject(ppCallObj);

    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_ACK);
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           sdf_cbk_uaCallTerminateRequest
 **
 ** DESCRIPTION:        Callback issued onto the application on the receipt of
 **                     a BYE message indicating termination of an
 **                     existing call.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCallTerminateRequest(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal           ret_val = Sdf_co_success;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;

    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pErr;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Check if OPTIONS is ongoing.If yes,invoke
     * the utility function to send the failure response 
     * to peer as well as clear the OPTIONS the context
     */
    if (ICF_NULL != ( p_ssa_ctxt->bitmask & ICF_SSA_OPTIONS_ONGOING ))
    {
        /* Not checking ret_val as this is best effort */         
        icf_ssa_clear_options_context(p_ssa_pdb);
    }
    else if(ICF_NULL != p_ssa_ctxt->p_call_ctxt &&
            p_ssa_ctxt->p_call_ctxt->common_bitmask & ICF_IN_DLG_OPTIONS_SEND)
    {
        icf_internal_msg_st         internal_msg_to_sic;
        internal_msg_to_sic.msg_hdr.msg_id = ICF_FORCEFUL_CALL_CLEAR_IND;
        internal_msg_to_sic.msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;
        internal_msg_to_sic.msg_hdr.ecode = ICF_ERROR_UNDEFINED;
        internal_msg_to_sic.msg_hdr.status = ICF_STATUS_CONTINUE;
        internal_msg_to_sic.p_msg_data = ICF_NULL;
         /* Not checking ret_val as this is best effort */ 
        icf_sic_process_mesg(&internal_msg_to_sic);
    }


    /* Populate the internal message header */
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CALL_TERMINATE ;
    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;

    /*
     * All messages to be sent to CC/SC are routed to the CM.
     * CM decides the specific destination of these messages
     */
    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_BYE);

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*********************************************************
 ** FUNCTION: sdf_cbk_uaReInviteReceived
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **              INVITE message in the call established state,
 **              in indicating receipt of a reinvite
 **
 **********************************************************/
Sdf_ty_retVal sdf_cbk_uaReInviteReceived(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_ty_retVal                       ret_val1 = Sdf_co_success;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_media_change_req_st      *p_internal_msg_param = ICF_NULL;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;

	SipOptions		            dOptions;
	SipMessage					*pMessage = Sdf_co_null;
	Sdf_st_transaction 			*pTransaction = Sdf_co_null;
	Sdf_st_overlapTransInfo 	*pOverlapTransaction = Sdf_co_null;
			
	/*This st is populated if there is SDP in incoming message
	 *it is then populated in the internal structures*/

	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    if (pEventContext->pData != (*ppCallObj)->pAppData->pData)
    {
        printf ("\n sdf_cbk_uaReInviteReceived: ssa context are not matching\n");
        ret_val = ICF_FAILURE;
        return ret_val;
    }
    
	/* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */


    /* Cast the payload into the icf_nw_inc_media_change_req_st structure */
    p_internal_msg_param = 
        (icf_nw_inc_media_change_req_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    p_internal_msg_param->bitmask = ICF_NULL;
    p_internal_msg_param->p_sdp_list = ICF_NULL;
    p_internal_msg_param->p_added_sdp_list = ICF_NULL;
    p_internal_msg_param->p_nw_reject_stream_list = ICF_NULL;
    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;

    /*Set method as ReInvite in bitmask*/
    p_internal_msg_param->bitmask = ICF_INC_MEDIA_CHANGE_METHOD_REINVITE;

    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.
				stat_inbound_req.stats_ReINVITE);

    /* The function icf_ssa_check_isfocus_in_contact_header returns success*/
    /* if the contact header does have the isfocus parameter. In this case */
    /* we will copy the conference_uri in call ctx */
    ret_val1 = icf_ssa_check_isfocus_in_contact_header(p_ssa_pdb);
    if (ICF_SUCCESS == ret_val1)
    {
        p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 |=
                ICF_INC_MEDIA_CHANGE_ISFOCUS_FOUND;
        if (p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str_len 
                == 0 )
        {
            p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri = 
                p_ssa_pdb->conference_uri;
            ICF_PRINT(((icf_uint8_t *)"ISFOCUS conferencr URI = %s",
              p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str))
        }
    }

    /*Reset this Bitmask if set
     *This bitmask checks the presence of 100rel in the message
     *This would be set in case of Incoming Invite before
     */	
    if(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask & 
		ICF_REL_PROV_RESP_SUPP_BY_PEER) 
    {
	p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask &=
					 ~(ICF_REL_PROV_RESP_SUPP_BY_PEER);
    }	

	if(ICF_NULL != (*ppCallObj)->pUasTransaction->pSipMsg)
		pMessage = (*ppCallObj)->pUasTransaction->pSipMsg;
	else
		pMessage = (*ppCallObj)->pUacTransaction->pSipMsg;

	if(sdf_ivk_uaGetTransactionForSipMsg(\
			(*ppCallObj), pMessage, &pTransaction, \
			&pOverlapTransaction, pErr) == Sdf_co_success)
	{
		if(pOverlapTransaction == Sdf_co_null)
		{
			pMessage = pTransaction->pSipMsg;
			sdf_ivk_uaFreeTransaction(pTransaction);
		}
		else
		{
			pMessage = pOverlapTransaction->pSipMsg;
			sdf_ivk_uaFreeOverlapTransInfo( \
					pOverlapTransaction);
		}
	}

    if(Sdf_en_invalid == sdf_ivk_uaIsSupportedMsgBody(*ppCallObj,
		(*ppCallObj)->pInitData,pErr))
	{
        /* sdf_ivk_uaIsSupportedMsgBody function has called 
	     * sdf_ivk_uaRejectRequest so we will directly send the
	     * message 
		 */
		dOptions.dOption = SIP_OPT_CLEN | SIP_OPT_FULLFORM;
		
		
		
		/* 
		 * Bad Contnet received in 
		 * Content-type header. 415 unsuppreted 
		 * formed by UATK. Send the SipMsg by using .
		 * sdf_ivk_uaGetTransactionForSipMsg
		 */
		
			
		ICF_PRINT(((icf_uint8_t *)"[SSA:] Validation of Content-Type, Content-Encoding"
				" or Content-Language failed\n"));
		if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
				(*ppCallObj),
				pMessage,
				&dOptions,pErr))
		{
			ret_val = ICF_FAILURE;
		}
		return ret_val;
	}   

        if((ICF_FAILURE == icf_ssa_parse_require_header(p_ssa_pdb,pMessage,pErr))||
       (ICF_NULL != (p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask &\
        ICF_REL_PROV_RESP_SUPP_BY_PEER)))
         {
             dOptions.dOption = SIP_OPT_CLEN | SIP_OPT_FULLFORM;

             if (Sdf_co_fail ==
                         sdf_ivk_uaFormResponse(420, "INVITE",
                                                p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                                pOverlapTransaction,
                                                Sdf_co_false,
                                                pErr))
            {
                 /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                 ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE ==
                       icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                                                           (icf_uint8_t *)" Unsupported",
                                                           (icf_uint8_t *)"100rel",
                                                           pMessage))
            {
                   ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                 p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                 pMessage,
                                 &dOptions,pErr))
            {
                   /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                   ret_val = ICF_FAILURE;
            }
            else
            {

                 ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                 return ret_val;
             }
        }
	
	
/* Commented the below mentioned code to add the functionality for STOP FAX REQUEST 
     * in which we get ReInvite for stop fax request 
     * while fax call is on going */

#if 0    
   
    if(ICF_CALL_TYPE_T38 & p_ssa_pdb->p_glb_pdb->p_call_ctx->call_type)
    {
        ICF_PRINT(("\n Re-Invite not supported when T.38 call is ongoing"));
       
                        if (Sdf_co_fail ==
                                sdf_ivk_uaFormResponse(488, "INVITE",
                                                                p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                                                pOverlapTransaction,
                                                                Sdf_co_false,
                                                                pErr))
            {
                                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE ==
                                icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                                                                        " Unsupported",
                                                                        "100rel",
                                                                        p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
                        {
                                ret_val = ICF_FAILURE;
                        }
                        else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
                                &dOptions,pErr))
                        {
                                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                                ret_val = ICF_FAILURE;
                        }
                        else
                        {
                                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                                return ret_val;
                        }
 
    }
   
#endif

    /* Check for the presence of SDP in the received message */
    if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
            ((*ppCallObj)->pUasTransaction->pSipMsg,pErr)) 
    {
        p_ssa_ctxt->bitmask &= ~ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP;
        p_ssa_ctxt->bitmask |= ICF_SSA_CTXT_REINVITE_RECVD;

        /*
         * SSA is not using any media handling related 
         * functionality provided by the UATK.
         * On receiving SDP in any SIP message,SSA
         * converts the SDP into the generic SDP structure
         * of the call context and then either populates
         * the p_remote_received_sdp structure of the call
         * context or passes this SDP in the internal message
         * Since the SDP structure of the call context doesn't
         * have a mode field, SSA will be calling the SDP
         * conversion function with the pointer to the mode 
         * variable defined below.The SDP conversion function 
         * will extract the mode from the SDP and store
         * it into this variable.
         */

        if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                p_ssa_pdb,
                                (*ppCallObj)->pUasTransaction->pSipMsg,
								0, &p_inc_media_sdp_info,
                                p_ssa_pdb->p_ecode))

        {
			icf_uint16_t    	resp_code = 0;

            if(ICF_NULL != p_inc_media_sdp_info)
            {
                 /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
            }
	
			if (p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_415)
			{
				resp_code = 415;
			}
			else
			{
				resp_code = 488;
			}

			dOptions.dOption= SIP_OPT_CLEN | SIP_OPT_FULLFORM;

			if (Sdf_co_fail == sdf_ivk_uaFormResponse(
									resp_code,
									"INVITE",
									p_ssa_pdb->p_ssa_ctxt->p_call_obj,
									pOverlapTransaction,
									Sdf_co_false,
									pErr))
			{
					/*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
					ret_val = ICF_FAILURE;
			}
			else if (ICF_FAILURE ==
							icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
									(icf_uint8_t *)" Unsupported",
									(icf_uint8_t *)"100rel",
									p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
			{
					ret_val = ICF_FAILURE;
			}
			else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
									p_ssa_pdb->p_ssa_ctxt->p_call_obj,
									p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
									&dOptions,pErr))
			{
					/*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
					ret_val = ICF_FAILURE;
			}
			else
			{
					ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
							return ret_val;
			}

			ret_val = Sdf_co_fail;
			ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
					return ret_val;
        }
		else
        {
            /* The following code leg has been commented for SPR 17808. This functionality is now
             * being handled by CC wherein if the REINVITE received is for STOP/START fax, it is
             * being rejected by 500.
             */
#if 0
            /*If the retry timer for STOP FAX is going on, and IPTK
             * receives ReINVITE from network besides STOP FAX, then IPTK 
             * shall send 500 for that ReINVITE.
             */
             if((ICF_TIMER_ID_INVALID != p_ssa_ctxt->p_call_ctxt->retry_after_timer_id)
                         && (!(ICF_SSA_INC_STOP_FAX_REINV &
                                      p_ssa_pdb->common_bitmask)))
             {
		            p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_500;
  
                    if(ICF_NULL != p_inc_media_sdp_info)
                    {
                       /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                       icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                        &p_inc_media_sdp_info);
                    }
             
                   if (Sdf_co_fail == sdf_ivk_uaFormResponse(
									500,
									"INVITE",
									p_ssa_pdb->p_ssa_ctxt->p_call_obj,
									pOverlapTransaction,
									Sdf_co_false,
									pErr))
           			{
		        			/*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
				         	ret_val = ICF_FAILURE;
			        }
               		else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
									p_ssa_pdb->p_ssa_ctxt->p_call_obj,
									p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
									&dOptions,pErr))
          			{
		       			/*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
			       		ret_val = ICF_FAILURE;
			        }
          			else
		        	{
				        	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        					return ret_val;
		        	}
				    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        			return ret_val;
             } 
#endif

			if(ICF_NULL != p_inc_media_sdp_info->count)
			{
				icf_uint8_t count = ICF_NULL;

				p_internal_msg_param->count = p_inc_media_sdp_info->count;

				while(count != p_inc_media_sdp_info->count)
				{
					p_internal_msg_param->rejected_streams[count].stream_id =\
						p_inc_media_sdp_info->rejected_streams[count].stream_id;
					p_internal_msg_param->rejected_streams[count].stream_type =\
						p_inc_media_sdp_info->rejected_streams[count].stream_type;
					count++;
				}
				p_internal_msg_param->bitmask |=\
									ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM;
			}

			if(ICF_NULL != p_inc_media_sdp_info->p_added_sdp_list)
			{
				p_internal_msg_param->p_added_sdp_list =\
					p_inc_media_sdp_info->p_added_sdp_list;
				
				p_internal_msg_param->bitmask |=\
									ICF_NW_INC_MEDIA_ADD_STREAM;
				
				ICF_SSA_ADD_MEM_CHUNK_TO_PTR_LIST(
                				p_ssa_pdb,
				                &p_internal_msg_param->p_added_sdp_list,
				                ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST)
			}
			
	   		 /* 
             * Set the bit which indicates that RE_INVITE 
             * has been received with SDP
             */
            if(ICF_NULL != p_inc_media_sdp_info->p_sdp_info_list)
			{
				p_internal_msg_param->p_sdp_list =\
									p_inc_media_sdp_info->p_sdp_info_list;
				ICF_SSA_SET_COMMON_BITMASK(\
                    p_internal_msg_param->bitmask,
                    ICF_NW_INC_MEDIA_CHG_SDP_PRESENT)
			}	
            if(ICF_NULL != p_inc_media_sdp_info->p_nw_reject_stream_list)
            {
                p_internal_msg_param->p_nw_reject_stream_list =\
                    p_inc_media_sdp_info->p_nw_reject_stream_list;

                p_internal_msg_param->bitmask |=\
                    ICF_NW_INC_MEDIA_NW_REJECT_STREAM;

            }			
            /* 
             * The memory allocated for the SDP has to be freed
             * by the SSA on the return path.
             * For this purpose,SSA is maintaining the list
             * of all such memory chunks which have to be freed 
             * in the return path.The memory acquired for the SDP
             * is one such memory chunk.Add this memory into the 
             * allocated_memory list maintained in the SSA PDB
             */
	   /*		ICF_SSA_ADD_MEM_CHUNK_TO_PTR_LIST(
                p_ssa_pdb,
                &p_internal_msg_param->p_sdp_list,
                ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST)*/
            /* 
             * Delete the message body list stored in the UAS
             * transaction of the call object.
             * This is done to flush the SDP body from the 
             * transaction substructure of the call object
             */
            sdf_listDeleteAll(
                            &((*ppCallObj)->pUasTransaction->slMsgBodyList),
                            pErr);
			
#if 0				
            if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                p_ssa_pdb, 
                ICF_INT_MODULE_CC,
                (icf_void_t*)(p_ssa_pdb->p_glb_pdb->p_call_ctx),
                (*ppCallObj)->pUasTransaction->pSipMsg,
                p_ssa_pdb->p_ecode) )
            {
                p_ssa_ctxt->failure_bitmask |= 
                    ICF_SSA_FAILURE_RESP_CODE_415;
                ICF_PRINT(("\nUnable to get application specific headers"));
                             
                 /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);

                if (Sdf_co_fail == sdf_ivk_uaFormResponse(
                    415,"INVITE", *ppCallObj, Sdf_co_null,
                    Sdf_co_false, pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    *ppCallObj,
                    (*ppCallObj)->pUasTransaction->pSipMsg,
                    &dOptions,pErr))
                {
                    ret_val = Sdf_co_fail;
                }  

                ret_val = Sdf_co_fail;
            }
#endif           	
        }
	}
    /* Extract the message body other than SDP from SIP message */
	else 
	{
        p_ssa_ctxt->bitmask &= ~ICF_SSA_CTXT_REINVITE_RECVD;
        p_ssa_ctxt->bitmask |= ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP;

       if(ICF_NULL == p_ssa_ctxt->p_call_ctxt)
       {
                ICF_PRINT(((icf_uint8_t *)"\nTo Header have missing tag"));
                if (Sdf_co_fail == sdf_ivk_uaFormResponse(
                    481,"INVITE", *ppCallObj, Sdf_co_null,
                    Sdf_co_false, pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    *ppCallObj,
                    (*ppCallObj)->pUasTransaction->pSipMsg,
                    &dOptions,pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                ret_val = Sdf_co_fail;

       }
       
      else if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
		(*ppCallObj)->pUasTransaction->pSipMsg,/**ppCallObj*/SipMessageRequest,\
		&(p_ssa_ctxt->p_call_ctxt->body_list)))
	   {
	        if(p_ssa_pdb->p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_415)
			{
					/* SPR 11096 Changes : Generate 415 Response and free any local 
					 * reference
					 */
				     p_ssa_ctxt->failure_bitmask |= \
                        ICF_SSA_FAILURE_RESP_CODE_415;
					 ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
			 }
			else
			{
			   ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
			}
		    ret_val = Sdf_co_fail;
			/* SPR 11096 Changes Ends.
			 */
	    }
	}
	/* Changes for Message body header ends
	 */      
	if (Sdf_co_fail != ret_val)
        {
	   if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                p_ssa_pdb, 
                ICF_INT_MODULE_CC,
                (icf_void_t*)(p_ssa_pdb->p_glb_pdb->p_call_ctx),
                (*ppCallObj)->pUasTransaction->pSipMsg,
                p_ssa_pdb->p_ecode) )
            {
                p_ssa_ctxt->failure_bitmask |= 
                    ICF_SSA_FAILURE_RESP_CODE_500;
                ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
                if (Sdf_co_fail == sdf_ivk_uaFormResponse(
                    500,"INVITE", *ppCallObj, Sdf_co_null,
                    Sdf_co_false, pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    *ppCallObj,
                    (*ppCallObj)->pUasTransaction->pSipMsg,
                    &dOptions,pErr))
                {
                    ret_val = Sdf_co_fail;
                }                             
                ret_val = Sdf_co_fail;
            }
	    else 
        {

            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                ICF_PORT_SIZEOF(icf_nw_inc_media_change_req_st);
            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_NW_INC_MEDIA_UPDATE ;

            /*
             * All messages to be sent to CC/SC are routed to the CM.
             * CM decides the specific destination of these messages
             */
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
        }
        else
        {
            /* Abort the transaction */
            sdf_ivk_uaAbortTransaction((*ppCallObj),
                    (*ppCallObj)->pUasTransaction->dTxnId,
                    (Sdf_ty_pvoid*)p_ssa_ctxt, pErr);
        }
#ifdef ICF_SESSION_TIMER
   if(ICF_TRUE == p_ssa_ctxt->p_call_ctxt->session_timer_enable)
   {
        ret_val = icf_ssa_session_timer_get(p_ssa_pdb);
        if(ret_val == ICF_FAILURE)
        { 
          ret_val = ICF_SUCCESS;
          ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) 
          return ret_val;
        }  

        if(Sdf_co_fail != ret_val)
        {
            ret_val = icf_ssa_session_timer_set(p_ssa_pdb);
        }
   }
   else
   {
        if(Sdf_co_null != (*ppCallObj)->pSessionTimer)
        {
              if(Sdf_en_refresherInvalid != (*ppCallObj)->pSessionTimer->dRefreshRequest)
              {
                  (*ppCallObj)->pSessionTimer->dRefreshRequest = Sdf_en_refresherInvalid;
              }
        }
   }
#endif
	if(Sdf_co_fail != ret_val && ICF_NULL != p_inc_media_sdp_info)
	{
		p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
		p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
           		 p_inc_media_sdp_info,
           		ICF_MEM_COMMON,
           		p_ssa_pdb->p_ecode,
           		ret_val)
	}
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
/*****************************************************************************
 ** FUNCTION:           sdf_cbk_uaCallAcknowledged
 **
 ** DESCRIPTION:        Callback issued onto the application on the receipt of
 **                     an ACK message.
 **                     1. Check if SDP is present in received message
 **                     2. If yes, then create SDP block and populate it with 
 **                        received SDP
 **                     3. Fill internal msg to send message to CM
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCallAcknowledged
        (Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_connect_ack_st           *p_internal_msg_param = ICF_NULL;

	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* Cast the payload into the icf_nw_inc_connect_ack_st structure */
    p_internal_msg_param = 
        (icf_nw_inc_connect_ack_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    p_internal_msg_param->bitmask = ICF_NULL;
    p_internal_msg_param->p_sdp_list = ICF_NULL;


    /* Check for the presence of SDP in the received message */
    if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
            ((*ppCallObj)->pUasTransaction->pSipMsg,pErr)) 
    {
        /*
         * SSA is not using any media handling related 
         * functionality provided by the UATK.
         * On receiving SDP in any SIP message,SSA
         * converts the SDP into the generic SDP structure
         * of the call context and then either populates
         * the p_remote_received_sdp structure of the call
         * context or passes this SDP in the internal message
         * Since the SDP structure of the call context doesn't
         * have a mode field, SSA will be calling the SDP
         * conversion function with the pointer to the mode 
         * variable defined below.The SDP conversion function 
         * will extract the mode from the SDP and store
         * it into this variable.
         */
        if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                p_ssa_pdb,
                                (*ppCallObj)->pUasTransaction->pSipMsg,
								0, &p_inc_media_sdp_info,
                                p_ssa_pdb->p_ecode))

        {
            if(ICF_NULL != p_inc_media_sdp_info)
            {
                 /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
            }
            if((ICF_SSA_FAILURE_RESP_CODE_415 & p_ssa_pdb->p_ssa_ctxt->\
                  failure_bitmask)||
               (ICF_SSA_FAILURE_RESP_CODE_488 & p_ssa_pdb->p_ssa_ctxt->\
                  failure_bitmask)||
               (ICF_SSA_FAILURE_RESP_CODE_580 & p_ssa_pdb->p_ssa_ctxt->\
                  failure_bitmask))
            {
                /*p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                 p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                     ICF_GENERIC_CALL_CLEAR;
                 p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                     p_ssa_pdb->p_glb_pdb;                            
                 p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                 ret_val = Sdf_co_success;*/
                 p_internal_msg_param->bitmask |= ICF_NW_INC_CONNECT_ACK_ERROR;
            }
            else
			{
				ret_val = Sdf_co_fail;
			}
        }        
        else
        {
			p_internal_msg_param->p_sdp_list =\
							p_inc_media_sdp_info->p_sdp_info_list;
            /* 
             * Set the bit which indicates that ACK
             * has been received with SDP
             */
            ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                    ICF_NW_INC_CONNECT_ACK_SDP)
            /* 
             * Delete the message body list stored in the UAS
             * transaction of the call object.
             * This is done to flush the SDP body from the 
             * transaction substructure of the call object
             */
             sdf_listDeleteAll(
                     &((*ppCallObj)->pUasTransaction->slMsgBodyList),
                     pErr);
        }
    }

   /* get the application specific headers from dbm.
    * These headers shall be filled in call context based
    * on the app_id in call ctxt. Currently use app id = 1
    */
    if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
        p_ssa_pdb,ICF_INT_MODULE_CC,
        (icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
        (*ppCallObj)->pUasTransaction->pSipMsg,
        p_ssa_pdb->p_ecode))
    {
        /*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/

            /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                &p_inc_media_sdp_info);

            ret_val = ICF_FAILURE;
    }

    if ((Sdf_co_fail != ret_val)&&
		(ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id))
    {
        /* Populate the internal message header */
        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CONNECT_ACK ;
        p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
            ICF_PORT_SIZEOF(icf_nw_inc_connect_ack_st);

        /*
         * All messages to be sent to CC/SC are routed to the CM.
         * CM decides the specific destination of these messages
         */
        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
		if(ICF_NULL != p_inc_media_sdp_info)
		{
			p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
			p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;
			ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
            	 p_inc_media_sdp_info,
             	ICF_MEM_COMMON,
             	p_ssa_pdb->p_ecode,
             	ret_val)
		}
    }
    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_ACK);
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/**********************************************************
** FUNCTION:  sdf_cbk_uaOverlapTransactionReceived
**
** DESCRIPTION: Callback issued onto the application on the
**              receipt of a PRACK
**              1. Extract method from SIP message and compare whether it is
**                 PRACK or not.
**              2. If yes, then determine for which message this PRACK is
**                 received.
**              3. If SDP is present with PRACK then create a SDP block and 
**                 copy received SDP.
**              4. Fill internal msg for the appropriate resp code
**
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaOverlapTransactionReceived(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_overlapTransInfo *pOverlapTransInfo,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    Sdf_ty_s8bit                        *p_method = ICF_NULL;
    icf_nw_inc_prack_st                 *p_internal_msg_param = ICF_NULL;
    Sdf_ty_u16bit                       dRespCode = 0;

	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    /* Cast the payload into the icf_nw_inc_prack_st structure */
    p_internal_msg_param = 
        (icf_nw_inc_prack_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    p_internal_msg_param->bitmask = ICF_NULL;
    p_internal_msg_param->p_sdp_list = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
    /* 
     * Extract the method. If this overlapped transaction
     * is not PRACK,SSA will return FAILURE as it
     * is supporting only PRACK in this callback
     */
    if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                pOverlapTransInfo->pSipMsg,
                &p_method,pErr))
    {
        ret_val = Sdf_co_fail;
    }
    else
    {
        /* separate API for PRACK for 180/183 */
        if (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"PRACK"))
        {
            /* Determine which RPR has PRACK come for */
            /* Workaround in SSA for CSR 1-2102528 + 1-2095105 */
            /*
            if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                        (*ppCallObj)->pUasTransaction->pSipMsg,
                        &dRespCode, pErr))
            {
                ret_val = Sdf_co_fail;
            }*/ /* (*ppCallObj)->pUasTransaction-... */
            if (ICF_SSA_OTG_RPR_ALERT & p_ssa_ctxt->bitmask)
            {
                dRespCode = 180;
            }
            else
            {
                dRespCode = 183;
            }
            
            if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent(
                        pOverlapTransInfo->pSipMsg, pErr))
            {
                /*
                 * SSA is not using any media handling related 
                 * functionality provided by the UATK.
                 * On receiving SDP in any SIP message,SSA
                 * converts the SDP into the generic SDP structure
                 * of the call context and then either populates
                 * the p_remote_received_sdp structure of the call
                 * context or passes this SDP in the internal message
                 * Since the SDP structure of the call context doesn't
                 * have a mode field, SSA will be calling the SDP
                 * conversion function with the pointer to the mode 
                 * variable defined below.The SDP conversion function 
                 * will extract the mode from the SDP and store
                 * it into this variable.
                 */
                if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                        p_ssa_pdb,
                                        pOverlapTransInfo->pSipMsg,0,
                                        &p_inc_media_sdp_info,
                                        p_ssa_pdb->p_ecode))

                {
			ret_val = Sdf_co_fail;
			if(ICF_NULL != p_inc_media_sdp_info)
			{
				/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
				icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
						&p_inc_media_sdp_info);
			}
	        if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                     p_ssa_pdb,415,
                                     *ppCallObj,
                                    pOverlapTransInfo,
                                    "PRACK", Sdf_co_false))
            {
                  ret_val = ICF_FAILURE;
            }
#if 0
			else
			{
				p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->offer_failure_reason = 
					ICF_SA_NETWORK_ERROR;
			}
#endif

            if((ICF_SSA_FAILURE_RESP_CODE_415 & p_ssa_pdb->p_ssa_ctxt->\
                  failure_bitmask)||
               (ICF_SSA_FAILURE_RESP_CODE_488 & p_ssa_pdb->p_ssa_ctxt->\
                  failure_bitmask)||  
               (ICF_SSA_FAILURE_RESP_CODE_580 & p_ssa_pdb->p_ssa_ctxt->\
                  failure_bitmask))
			{
				p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
					ICF_GENERIC_CALL_CLEAR;
				p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
					p_ssa_pdb->p_glb_pdb;                            
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
				ret_val = Sdf_co_success;

			}
			else
			{
				ret_val = Sdf_co_fail;
			}
            p_ssa_pdb->p_ssa_ctxt->failure_bitmask = ICF_NULL; 
		}
		else
		{
			if(ICF_NULL != p_inc_media_sdp_info->count)
			{
				icf_uint8_t count = ICF_NULL;

				p_internal_msg_param->count = p_inc_media_sdp_info->count;

				while(count != p_inc_media_sdp_info->count)
				{
					p_internal_msg_param->rejected_streams[count].stream_id =\
												 p_inc_media_sdp_info->rejected_streams[count].stream_id;
					p_internal_msg_param->rejected_streams[count].stream_type=\
												  p_inc_media_sdp_info->rejected_streams[count].stream_type;
					count++;
				}

				p_internal_msg_param->bitmask |=\
								ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM;
			}
			if(ICF_NULL != p_inc_media_sdp_info->p_added_sdp_list)
			{
				p_internal_msg_param->p_added_sdp_list =\
									p_inc_media_sdp_info->p_added_sdp_list;
				p_internal_msg_param->bitmask |=\
								ICF_NW_INC_MEDIA_ADD_STREAM;
				ICF_SSA_ADD_MEM_CHUNK_TO_PTR_LIST(p_ssa_pdb,
						&p_internal_msg_param->p_added_sdp_list,
                             ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST)
					}

 
					/* 
                     * Set the bit which indicates that PRACK
                     * has been received with SDP
                     */
					if(ICF_NULL != p_inc_media_sdp_info->p_sdp_info_list)
					{
						p_internal_msg_param->p_sdp_list =\
									p_inc_media_sdp_info->p_sdp_info_list;

	                    ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
    	                        ICF_NW_INC_PRACK_SDP)
					
					}
                    /* 
                     * The memory allocated for the SDP has to be freed
                     * by the SSA on the return path.
                     * For this purpose,SSA is maintaining the list
                     * of all such memory chunks which have to be freed 
                     * in the return path.The memory acquired for the SDP
                     * is one such memory chunk.Add this memory into the 
                     * allocated_memory list maintained in the SSA PDB
                     */
                     

			
                     /* 
                      * Delete the message body list stored in the UAS
                      * transaction of the call object.
                      * This is done to flush the SDP body from the 
                      * transaction substructure of the call object
                      */
                     sdf_listDeleteAll(
                             &((*ppCallObj)->pUasTransaction->slMsgBodyList),
                             pErr);
                }
            }
            if ( ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id && Sdf_co_fail != ret_val)
            {
                /* Populate the internal message header */
				if(180 == dRespCode)
				{
					p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
						ICF_NW_INC_ALERT_PRACK ;
				} /* if(180 == dRespCode) */
				else
				{
					p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
						ICF_NW_INC_MEDIA_PARAM_PRACK ;
				} /* else */
				p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
					ICF_PORT_SIZEOF(icf_nw_inc_prack_st);
					/*
					* All messages to be sent to CC/SC are routed to the CM.
					* CM decides the specific destination of these messages
				*/
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
            } /* if (Sdf_co_fail != ret_val) */
        }
        else
        {
            /*
             * SSA is handling only PRACK in this 
             * overlapped transaction
             * Abort the transaction
             */
            sdf_ivk_uaAbortTransaction((*ppCallObj),
                    pOverlapTransInfo->dTxnId,
                    (Sdf_ty_pvoid*)p_ssa_ctxt, pErr);
            ret_val = Sdf_co_fail;
        } /* else */
    }
	if(Sdf_co_fail != ret_val && ICF_NULL != p_inc_media_sdp_info)
	{
       	p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
		p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
	    		p_inc_media_sdp_info,
       		    ICF_MEM_COMMON, 
	    	    p_ssa_pdb->p_ecode, 
	    	    ret_val)
	}
    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_PRACK);
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:    sdf_cbk_uaCancelReceived
 **
 ** DESCRIPTION: This callback is issued by the toolkit when a CANCEL is
 **              received or a BYE is received when the INVITE has not 
 **              yet been responded to.
 **             1. SSA itself send 200 OK for CANCEL. 
 **             2. If CANCEL is not received for REINVITE then send call abort
 **                to CM
 **             3. Otherwise, if cancel is received for REINVITE then send 487
 **                response to peer
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCancelReceived(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)

{
    Sdf_ty_retVal           ret_val = Sdf_co_success;
    SipOptions              dOptions ;
    Sdf_st_callObject       *p_orig_call_obj = Sdf_co_null;
	Sdf_ty_s8bit            *p_method = Sdf_co_null;	
    icf_uint8_t           method[10];


    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;  

    /* To remove compiler warning */
    (void)ppCallObj;
    /*Fix for CSR 8071369 SPR 20235*/
    /* Issue:IPTK does not send 200 Ok for CANCEL received from peer 
    for an incoming call,when transport address is configured per line.
    This is beacsue line_id is incorrectin ppCallObj)->pAppData->pData
    which is further passed to icf_ssa_send_to_peer to send message on 
    network
    Fix:Correct line_id is fetched from pEventContext->pData and 
    populated in ppCallObj->pAppData->pData*/

    ((icf_ssa_ctxt_st*)((*ppCallObj)->pAppData->pData))->line_id = \
        ((icf_ssa_ctxt_st*)(pEventContext->pData))->line_id;
    icf_port_memset(method,'\0',10*sizeof(icf_uint8_t));
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    p_orig_call_obj = p_ssa_ctxt->p_call_obj;

    /*
     * Check if OPTIONS is ongoing.If yes,invoke
     * the utility function to send the failure response 
     * to peer as well as clear the OPTIONS the context
     */
    if (ICF_NULL != ( p_ssa_ctxt->bitmask & ICF_SSA_OPTIONS_ONGOING ))
    {
        /* Not checking ret_val as this is best effort */         
        icf_ssa_clear_options_context(p_ssa_pdb);
    }

    /* Specify the stack options */
    dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    /* Get the received method */
    if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                            (*ppCallObj)->pUasTransaction->pSipMsg,
                            &p_method,
                            pErr))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }
    icf_port_strcpy(method,(icf_uint8_t *)p_method);

    /* In case ICF has already sent final response or received ACK, send 481.
     * Else if ICF has sent provisional resp for Invite, send 200 OK for CANCEL/BYE
     * followed by 487 as is already being done */
    /* For this, check the call state in call object */
    if ((Sdf_en_finalSuccessSent == p_orig_call_obj->pCallInfo->dState) ||
        (Sdf_en_callEstablished == p_orig_call_obj->pCallInfo->dState) ||
        (Sdf_en_finalFailureSent == p_orig_call_obj->pCallInfo->dState))
    {
        if (Sdf_co_fail == sdf_ivk_uaFormResponse(
                    481,p_method, p_orig_call_obj, Sdf_co_null,
                    Sdf_co_false, pErr))
        {
            ret_val = Sdf_co_fail;
        }
        else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_orig_call_obj,
                    p_orig_call_obj->pUasTransaction->pSipMsg,
                    &dOptions,pErr))
        {
            ret_val = Sdf_co_fail;
        }

        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }
#ifdef ICF_STAT_ENABLE
        else
        {
          icf_ssa_resp_stats(p_ssa_pdb,method,481,1);
        }
#endif
    /* Now send a 200 OK for the CANCEL/BYE received */
    if (Sdf_co_fail == sdf_ivk_uaFormResponse(
                200,p_method, *ppCallObj, Sdf_co_null,
                Sdf_co_false, pErr))
    {
        ret_val = Sdf_co_fail;
    }
    else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                *ppCallObj,
                (*ppCallObj)->pUasTransaction->pSipMsg,
                &dOptions,pErr))
    {
        ret_val = Sdf_co_fail;
    }
#ifdef ICF_STAT_ENABLE
    else
    {
          icf_ssa_resp_stats(p_ssa_pdb,method,200,1);
     }
#endif
    if ( Sdf_co_fail != ret_val)
    {
        /* If call context is null, don't call CM*/
            
         if (ICF_NULL == p_ssa_ctxt->p_call_ctxt)
        {
             ret_val = Sdf_co_success;
        }
        else
        {
ICF_HANDLE_REINVITE_START
            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                          ICF_NW_INC_CALL_ABORT ;
            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                                                    ICF_NULL;

            /*
             * All messages to be sent to CC/SC are routed 
             * to the CM.CM decides the specific destination 
             * of these messages
             */
             p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
         }
ICF_HANDLE_REINVITE_END
    }
    else
    {
        /* 
         * Abort the UAS transaction of the temp call 
         * object as the 200 OK for the CANCEL couldn't
         * be sent
         */
        sdf_ivk_uaAbortTransaction(*ppCallObj,
                (*ppCallObj)->pUasTransaction->dTxnId,
                (Sdf_ty_pvoid*)p_ssa_ctxt, pErr);
    }
    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_CANCEL);
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val ;
} 

/*********************************************************
** FUNCTION: sdf_cbk_uaUnexpectedRequestReceived
**
** DESCRIPTION: This callback is issued by the toolkit when
**              it processing a request containing to tag in
**              sdf_ivk_uaHandleCall. This is to facilitate
**              crash-recovery by application. The application
**              may attempt to re-construct the call object and
**              continue the call.In SSA, we are just sending
**              a 481 response.
**
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaUnexpectedRequestReceived(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_overlapTransInfo *pOverlapTransInfo,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal           ret_val = Sdf_co_success;
    SipMessage              *p_message = Sdf_co_null;
    Sdf_ty_s8bit            *p_method = Sdf_co_null;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;

    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* Extract the SIP message */
    if( pOverlapTransInfo != Sdf_co_null )
    {
        p_message = pOverlapTransInfo->pSipMsg;
        p_ssa_ctxt->bitmask |= IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV;
    }
    else
    {
        p_message = (*ppCallObj)->pUasTransaction->pSipMsg;
        p_ssa_ctxt->bitmask |= IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV;
    }

    /* Return failure if pErr or ppCallObj is NULL */
    if ((Sdf_co_null == pErr) || (Sdf_co_null == ppCallObj))
    {
        ret_val = Sdf_co_fail;
    }
    else
    {
        /* Extract the method from the SIP message */
        if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                    p_message, &p_method,pErr))
        {
            ret_val = Sdf_co_fail;
        }
        else if (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"NOTIFY"))
        {
           p_ssa_ctxt->bitmask &= ~(IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV);
 
           p_ssa_pdb->notify_tag_mismatch = ICF_TRUE; 
  	      /* 
    	   * Issue the sdf_cbk_uaTransactionReceived callback to the 
      	   * application. 
           */
            ret_val = sdf_cbk_uaTransactionReceived(ppCallObj, \
                       				pEventContext, pOverlapTransInfo, pErr);
        }
        else
        {
            p_ssa_ctxt->bitmask_1 &= ~ICF_MSG_RECVD_OUT_OF_DIALOG;
            /*
             * Send 481 to peer
             */
            if (ICF_FAILURE == icf_ssa_form_and_send_response(
                        p_ssa_pdb,481,
                        *ppCallObj,
                        pOverlapTransInfo,
                        p_method, Sdf_co_false))
            {
                ret_val = Sdf_co_fail;
            }
            else
			{
					if(ICF_FAILURE !=
									icf_ssa_add_callid_callobj_map(
											p_ssa_pdb,
											p_ssa_ctxt,
											p_ssa_pdb->p_ecode))
					{
							/* 
							 * Increment the refcount of the call object
							 * and set the bit indicating that SSA ctxt
							 * needs to be stored 
							 */
							p_ssa_ctxt->bitmask
									|= ICF_SSA_SSA_CTXT_STORED;
							/*
							 * As this failure is not going to be
							 * propagated upwards and we want to 
							 * retain the SSA ctxt,return SUCCESS 
							 * from here
							 */
							ret_val = ICF_SUCCESS;
					}
					else
					{
							ret_val = ICF_FAILURE;
					}
			} /* else */
		} /* else */
    } /* else */
		
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val ;
}

/*****************************************************************************
 ** FUNCTION:		sdf_cbk_uaTransactionReceived
 **
 * DESCRIPTION:	Callback issued onto the application on the receipt of
 *					an REGISTER/OPTIONS/INFO/SUBSCRIBE/NOTIFY/REFER/MEESAGE message 
 *              1. Fetch method from received SIP message
 *              2. If REGISTER received, then send 501 response, as toolkit
 *                 does not support REGISTER
 *              3. If message is UPDATE and SDP is present then create SDP
 *                 block, populate it with received SDP and fill internal
 *                 msg for incoming media update api and set SDP present flag
 *                 otherwise this flag will not be set in int msg
 *              4. If method is REFER, allocate memory for refer_to address
 *                 and extract this address from incoming message
 *              5. If REPLACES header is present then extract call-id, 
 *                 from and to tag fill internal msg with above extracted 
 *                 values and send msg to CM.
 *              6. If the method is NOTIFY then iterate thru the list of 
 *                 headers and if event header present and it is refer, 
 *                 then fetch subscription state and if it is terminated then 
 *                 fetch notification state. If refer event not present 
 *                 send 481 response else subscription state is not terminated 
 *                 send 200 OK, else send inc_notify API to CM with 
 *                 notification state.
 *              7. If the method is OPTIONS,then first ascertain whether
 *                 it is received in/out of dialog.If OPTIONS is received 
 *                 out of dialog,query DBM for the availability/existence
 *                 of the user.
 *                 After this,proceed to query MMI for media capabilities.
 *              8. If UPDATE received without payload then send 200 OK
 *                 response without triggering CC.
 *
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTransactionReceived(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext, 
        Sdf_st_overlapTransInfo *pOverlapTransInfo,
        Sdf_st_error *pErr)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_return_t                        ret_val1 = ICF_SUCCESS;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_glb_pdb_st                      *p_glb_pdb = ICF_NULL;
    icf_error_t                         *p_ecode = ICF_NULL;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_uint32_t                        api_to_invoke = ICF_NULL;
    icf_boolean_t                       event_header_present = ICF_FALSE,
                                        refer_event_present = ICF_FALSE,
				        other_event_present = ICF_FALSE;
    SipOptions                          options;
    Sdf_ty_s8bit                        *p_method = Sdf_co_null;
    Sdf_st_callInfo                     *p_call_info = Sdf_co_null;
    Sdf_st_callObject                   *p_call_obj = Sdf_co_null;
    SipHeader                           replaces_header;
    Sdf_ty_state                        call_state;
    Sdf_st_error                        sdf_error;
    SipHeader                           *p_header = SIP_NULL;
    SipEventHeader                      *p_event_header = SIP_NULL;
	icf_app_id_t                        app_id = ICF_INVALID_APP_ID;

	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    icf_boolean_t                       handle_notify=ICF_FALSE;
    icf_boolean_t                       fail_resp_sent = ICF_FALSE;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

	p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

    p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    p_ecode = p_ssa_pdb->p_ecode;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_call_obj = *ppCallObj;

    options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    /*
     * Add this to the list of call object's maintained by the 
     * application only if it has come outside of a normal call
     */
    if (Sdf_co_fail == sdf_ivk_uaGetCallInfoFromCallObject((*ppCallObj), \
                &p_call_info, &sdf_error))
    {
        /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*DO not know how I will use it, but it makes sense to have the 
         * following as this callback can be hit out of dialog also, while
         * we are not accepting any out of dialog REFERs/UPDATEs. So 
         * we may need to distinguish between the in-dialog and out-dialog
         * invocations of this callback*/
        call_state = p_call_info->dState;

        /* Freeing the local reference */
        sdf_ivk_uaFreeCallInfo(p_call_info);	

        if(Sdf_co_fail == 
                sdf_ivk_uaGetMethodFromSipMessage(
                    pOverlapTransInfo->pSipMsg,
                    &p_method, &sdf_error))
        {
            /*sdf_ivk_uaFreeCallObject(&p_call_obj);*/
            /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE;
        }
		else if (ICF_NULL != p_ssa_pdb->p_app_conf)
        {
	        app_id = p_ssa_pdb->p_app_conf->app_id;
	}

    }
    if ( ICF_FAILURE != ret_val)
    {
        /*CSR-5213984 */
        /* just check if the URI received is longer then the one
         * IPTK supports set the error code here
         */
       if(ICF_NULL != pOverlapTransInfo)
        {

            icf_address_st      dummy_addr;
            SipReqLine              *p_req_line = SIP_NULL ; 
            SipAddrSpec             *p_req_uri = SIP_NULL;
            SipError                err;
            icf_return_t            dummy_ret_val = ICF_SUCCESS;
            
            if (SipFail == sip_getReqLineFromSipReqMsg(
                        pOverlapTransInfo->pSipMsg,
                        &p_req_line,&err))
            {
                dummy_ret_val = ICF_FAILURE;
            }
            else if (SipFail == sip_getAddrSpecFromReqLine(
                        p_req_line, &p_req_uri,&err))
            {
               dummy_ret_val = ICF_FAILURE;
            }
           /* this fuction is called just to set the ICF_SSA_FAILURE_RESP_CODE_414
            * bitmask in case URI is longer then the one we supports
            */
            if (ICF_FAILURE == icf_ssa_populate_address_from_sip_addr_spec(
                        p_ssa_pdb,p_req_uri,
                        &dummy_addr))
            {
                dummy_ret_val = ICF_FAILURE;
            }
            if(p_ssa_pdb->p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_414)
            {
                p_ssa_ctxt->bitmask_1 |= ICF_REQ_URI_TOO_LONG;
            }
            if (SIP_NULL != p_req_uri)
            {
                sip_freeSipAddrSpec(p_req_uri);
            } /* if (SIP_NULL != p_req_uri) */

            /* Free references of the Req line and addr spec */
            if (SIP_NULL != p_req_line)
            {
                sip_freeSipReqLine(p_req_line);
            } /* if (SIP_NULL != p_req_line) */

        }
        if ((ICF_NULL != p_ssa_ctxt) && \
                (ICF_SSA_APP_CTXT_TERMINATED == \
                 (ICF_SSA_APP_CTXT_TERMINATED & p_ssa_ctxt->bitmask)))
        {
            /* If app ctxt is terminated at this point, then send 481.
             * Fix for CSR 1-7497277 
             */
            if (Sdf_co_fail == sdf_ivk_uaFormResponse(481, p_method, 
                        p_call_obj,
                        pOverlapTransInfo,
                        Sdf_co_false,
                        &sdf_error))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_call_obj,
                        pOverlapTransInfo->pSipMsg,
                        &options,pErr))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else
            {
                /*  Beacuse failure response has been sent */ 
               fail_resp_sent = ICF_TRUE;
            }

        }
        if ((ICF_SUCCESS == ret_val) &&
           (ICF_FALSE == fail_resp_sent)) 
        {
        if(ICF_NULL == Sdf_mc_strcmp(p_method,"REGISTER"))
        {

/*
#ifdef ICF_LOAD_STAT
       p_glb_pdb->load_data.register_cfm++;
#endif
*/           /* 
             * Since REGISTER is a method not supported by a UA, 
             * we respond with a 501 
             */
            if (Sdf_co_fail == sdf_ivk_uaFormResponse(501, p_method, 
                        p_call_obj,
                        pOverlapTransInfo,
                        Sdf_co_false,
                        &sdf_error))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_call_obj,
                        pOverlapTransInfo->pSipMsg,
                        &options,pErr))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
        }
         /*CSR-5213984*/
        /* this is check for sending the 414 in case of long URI received */
        else if((p_ssa_ctxt->bitmask_1 & ICF_REQ_URI_TOO_LONG) == ICF_REQ_URI_TOO_LONG)
        {
            /* send the 414 here */
                       if (Sdf_co_fail == sdf_ivk_uaFormResponse(414, p_method, 
                        p_call_obj,
                        pOverlapTransInfo,
                        Sdf_co_false,
                        &sdf_error))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_call_obj,
                        pOverlapTransInfo->pSipMsg,
                        &options,pErr))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
           p_ssa_ctxt->bitmask |= IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV;
        }

        else if ( ICF_NULL == Sdf_mc_strcmp(p_method, "UPDATE"))
        {
            if (ICF_NULL == p_ssa_ctxt->p_call_ctxt)
            {
                ret_val = icf_ssa_form_and_send_response(
                                               p_ssa_pdb, 481,
                                               p_call_obj,
                                               pOverlapTransInfo,
                                               "UPDATE", Sdf_co_false);
            }
            else
            {
            icf_nw_inc_media_change_req_st  *p_internal_msg_param = ICF_NULL;
               
            p_internal_msg_param = 
                (icf_nw_inc_media_change_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

            /* Initialise the fields of the internal message */
            p_internal_msg_param->bitmask = ICF_NULL;
            p_internal_msg_param->p_sdp_list = ICF_NULL;
            p_internal_msg_param->p_added_sdp_list = ICF_NULL;
            p_internal_msg_param->p_nw_reject_stream_list = ICF_NULL;

/*twc_update handling*/
            if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                    p_ssa_pdb,
                    ICF_INT_MODULE_CC,
                    (icf_void_t*)p_ssa_ctxt->p_call_ctxt,
                    pOverlapTransInfo->pSipMsg,
                    p_ssa_pdb->p_ecode) )
            {
                 ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
                 ret_val = ICF_FAILURE;
            }

            ret_val1 = icf_ssa_check_isfocus_in_contact_header(p_ssa_pdb);
            if (ICF_SUCCESS == ret_val1)
            {
              p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 |=   
                   ICF_INC_MEDIA_CHANGE_ISFOCUS_FOUND;
              if(p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str_len 
                   == 0 )
               {
                 p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri = 
                        p_ssa_pdb->conference_uri;
                 ICF_PRINT(((icf_uint8_t *)"ISFOCUS conferencr URI = %s",
                 p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str))
               }
            }


#ifdef ICF_SESSION_TIMER
        if(ICF_TRUE == p_ssa_ctxt->p_call_ctxt->session_timer_enable)
        {    if(ret_val == ICF_FAILURE)
            {
                ret_val = ICF_SUCCESS;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return ret_val;
            }  

            ret_val = icf_ssa_session_timer_get_update(p_ssa_pdb,
                                                       pOverlapTransInfo);

    	if(ICF_FAILURE != ret_val)
        {
            ret_val = icf_ssa_session_timer_set(p_ssa_pdb);
        }
       }
       else
       {   
               if(Sdf_co_null != p_call_obj->pSessionTimer)
               {
                  if(Sdf_en_refresherInvalid != p_call_obj->pSessionTimer->dRefreshRequest)
                  {
                      p_call_obj->pSessionTimer->dRefreshRequest = Sdf_en_refresherInvalid;
                  }
               }
       }
#endif            
/* SPR 17399 : Fix for CSR 1-5314156*/
#if 0
            /* IF SDP received when T.38 call is ongoing then send 4xx
            */
            if(ICF_CALL_TYPE_T38 & p_ssa_pdb->p_glb_pdb->p_call_ctx->\
                call_type)
            {
                p_ssa_ctxt->failure_bitmask |= 
                    ICF_SSA_FAILURE_RESP_CODE_415;
                
                ICF_PRINT(("\nUpdate not supported when T.38 call is ongoing"));
                ret_val = ICF_FAILURE;                    
            }
            /* Check for the presence of SDP in the received message */
            else
#endif
                if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
                        (pOverlapTransInfo->pSipMsg,&sdf_error))             
            {
                /*
                 * SSA is not using any media handling related 
                 * functionality provided by the UATK.
                 * On receiving SDP in any SIP message,SSA
                 * converts the SDP into the generic SDP structure
                 * of the call context and then either populates
                 * the p_remote_received_sdp structure of the call
                 * context or passes this SDP in the internal message
                 * Since the SDP structure of the call context doesn't
                 * have a mode field, SSA will be calling the SDP
                 * conversion function with the pointer to the mode 
                 * variable defined below.The SDP conversion function 
                 * will extract the mode from the SDP and store
                 * it into this variable.
                 */

               p_ssa_ctxt->bitmask |= ICF_SSA_CTXT_UPDATE_RECVD;

               if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                        p_ssa_pdb,
                                        pOverlapTransInfo->pSipMsg,0,
                                        &p_inc_media_sdp_info,
                                        p_ecode))

                {
                   if(ICF_NULL != p_inc_media_sdp_info )
                   {
                       /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                       icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                           &p_inc_media_sdp_info);
                   }
		
                    /* If 415 bitmask is set then send generic call clear indication
                     */
                    if(ICF_SSA_FAILURE_RESP_CODE_415 & p_ssa_pdb->p_ssa_ctxt->\
                      failure_bitmask)
                    {
                        p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                             ICF_GENERIC_CALL_CLEAR;
                        p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                             p_ssa_pdb->p_glb_pdb;                            
                        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

                        if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                               p_ssa_pdb,415,
                                               p_call_obj,
                                               pOverlapTransInfo,
                                               "UPDATE", Sdf_co_false))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
    	                    ret_val = Sdf_co_success;
                        }
                    }
                    else if(ICF_SSA_FAILURE_RESP_CODE_488 & p_ssa_pdb->p_ssa_ctxt->\
                      failure_bitmask)
                    {
                        p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                             ICF_GENERIC_CALL_CLEAR;
                        p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb =
                             p_ssa_pdb->p_glb_pdb;
                        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

                        if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                               p_ssa_pdb,488,
                                               p_call_obj,
                                               pOverlapTransInfo,
                                               "UPDATE", Sdf_co_false))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            ret_val = Sdf_co_success;
                        }
                    }
                    else
                    {
		                ret_val = Sdf_co_fail;
		            }
                }
                else
                {
					if(ICF_NULL != p_inc_media_sdp_info->count)
					{
						icf_uint8_t count = ICF_NULL;
						
						p_internal_msg_param->count = p_inc_media_sdp_info->count;
						
						while(count != p_inc_media_sdp_info->count)
						{
							p_internal_msg_param->rejected_streams[count].stream_id =\
								p_inc_media_sdp_info->rejected_streams[count].stream_id;
							p_internal_msg_param->rejected_streams[count].stream_type =\
                                p_inc_media_sdp_info->rejected_streams[count].stream_type;

							count++;
						}
						
						p_internal_msg_param->bitmask |=\
							ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM;
					}
					if(ICF_NULL != p_inc_media_sdp_info->p_added_sdp_list)
					{
						p_internal_msg_param->p_added_sdp_list =\
									p_inc_media_sdp_info->p_added_sdp_list;
						p_internal_msg_param->bitmask |=\
									ICF_NW_INC_MEDIA_ADD_STREAM;
                    	
						ICF_SSA_ADD_MEM_CHUNK_TO_PTR_LIST(p_ssa_pdb,
                            &p_internal_msg_param->p_added_sdp_list,
                            ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST)
					}

					/* 
                     * Set the bit which indicates that UPDATE
                     * has been received with SDP
                     */
                    if(ICF_NULL != p_inc_media_sdp_info->p_sdp_info_list)
					{
			
						p_internal_msg_param->p_sdp_list =\
									p_inc_media_sdp_info->p_sdp_info_list;
						ICF_SSA_SET_COMMON_BITMASK(
                	            p_internal_msg_param->bitmask,
                    	        ICF_NW_INC_MEDIA_CHG_SDP_PRESENT)
                        	p_ssa_ctxt->bitmask &= ~ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP;
						p_ssa_ctxt->bitmask |= ICF_SSA_CTXT_REINVITE_RECVD;


					}
                    if(ICF_NULL != p_inc_media_sdp_info->p_nw_reject_stream_list)
                    {
                         p_internal_msg_param->p_nw_reject_stream_list =\
                                              p_inc_media_sdp_info->p_nw_reject_stream_list;

                         p_internal_msg_param->bitmask |=\
                                            ICF_NW_INC_MEDIA_NW_REJECT_STREAM;

                    }
                    /* 
                     * The memory allocated for the SDP has to be freed
                     * by the SSA on the return path.
                     * For this purpose,SSA is maintaining the list
                     * of all such memory chunks which have to be freed 
                     * in the return path.The memory acquired for the SDP
                     * is one such memory chunk.Add this memory into the 
                     * allocated_memory list maintained in the SSA PDB
                     */
                    /* 
                     * Delete the message body list stored in the overlapped
                     * transaction of the call object.
                     * This is done to flush the SDP body from the 
                     * transaction substructure of the call object
                     */
                    sdf_listDeleteAll(
                            &(pOverlapTransInfo->slMsgBodyList),
                            pErr);
                }

                if (( ICF_FAILURE != ret_val)&&
                    (ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id))
                {
                    
                    /* Populate the internal message header */
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_NW_INC_MEDIA_UPDATE ;
                    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                        ICF_PORT_SIZEOF(icf_nw_inc_media_change_req_st);

                   /* This bitmask is set above just before the call to
                     * icf_ssa_get_media_parameters_from_sdp.
                     * Fix for CSR_1-5278440/SPR 17321*/

                    /* p_ssa_ctxt->bitmask |= ICF_SSA_CTXT_UPDATE_RECVD;*/ 
                    /*
                    * All messages to be sent to CC/SC are routed to the CM.
                    * CM decides the specific destination of these messages
                    */
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                }
            }
            /*If UPDATE received without payload then form 200 OK response
             * and send to peer*/
            else
            {
                if (Sdf_co_fail == sdf_ivk_uaFormResponse(200, p_method, 
                            p_call_obj,
                            pOverlapTransInfo,
                            Sdf_co_false,
                            &sdf_error))
                {
                    /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                    ret_val = ICF_FAILURE;
                }
                else if (ICF_FAILURE == 
                        icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                            (icf_uint8_t *)"User-Agent", 
                            p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                            pOverlapTransInfo->pSipMsg))
                {
                    ret_val = ICF_FAILURE;
                }
                else if (ICF_FAILURE ==
                        icf_ssa_add_transport_param_in_contact_hdr(
                            p_ssa_pdb,
                            p_ssa_ctxt->p_call_obj->pInitData,
                            &sdf_error))

                {
                    ret_val = ICF_FAILURE;
                }
                else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                            p_call_obj,
                            pOverlapTransInfo->pSipMsg,
                            &options,pErr))
                {
                    /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                    ret_val = ICF_FAILURE;
                }
                 /*twc_update handling*/
                /*Send message to application*/
                if (( ICF_FAILURE != ret_val)&&
                    (ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id))
                {

                    /* Populate the internal message header */
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                        ICF_NW_INC_MEDIA_UPDATE ;
                    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                        ICF_PORT_SIZEOF(icf_nw_inc_media_change_req_st);
                    /*p_ssa_ctxt->bitmask |= ICF_SSA_CTXT_UPDATE_RECVD;*/

                    /*
                    * All messages to be sent to CC/SC are routed to the CM.
                    * CM decides the specific destination of these messages
                    */
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                }
            }
			if((ICF_FAILURE == ret_val) && 
			 	(p_ssa_ctxt->failure_bitmask & 
                    ICF_SSA_FAILURE_RESP_CODE_415))
			{
  				if (Sdf_co_fail == sdf_ivk_uaFormResponse (415, p_method,
					     p_call_obj,
					     pOverlapTransInfo,
					     Sdf_co_false, &sdf_error))
    			{
      				/*Raise/Set Error here:UATK_PROCESSING_FAILURE */
     				 ret_val = ICF_FAILURE;
   				 }
  				 else if (ICF_FAILURE == icf_ssa_send_to_peer (p_ssa_pdb,
						p_call_obj,
						pOverlapTransInfo->pSipMsg,
						&options, pErr))
    			{	
      				/*Raise/Set Error here:UATK_PROCESSING_FAILURE */
      				ret_val = ICF_FAILURE;
    			}
			 	ret_val = ICF_SUCCESS;
			}
        }
        }
        else if ( ICF_NULL == Sdf_mc_strcmp(p_method, "REFER"))
        {
            icf_ssa_ctxt_st      *p_mapped_ssa_ctxt = ICF_NULL;
            /*
             * If REFER is received out of dialog,
             * then we shall send 481 for the same
             */
            if ( ICF_FAILURE == icf_ssa_map_nw_callid(
                    p_ssa_pdb, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_call_obj, 
#endif
                    (icf_uint8_t *)p_call_obj->pCommonInfo->pCallid,
                    &p_mapped_ssa_ctxt, 
#ifdef ICF_LOOPBACK_SUPPORT
                    &sdf_error, 
#endif
                    p_ecode))
            {
                /* Send 481 for the same */
                if (ICF_FAILURE == icf_ssa_form_and_send_response(
                            p_ssa_pdb,481,
                            p_call_obj,
                            pOverlapTransInfo,
                            "REFER", Sdf_co_false))
                {
                    ret_val = ICF_FAILURE;
                }
            } /* &p_mapped_ssa_ctxt,p_ecode)) */
            else
            {
                
                icf_nw_inc_refer_st *p_internal_msg_param = 
                    (icf_nw_inc_refer_st *)
                    (p_ssa_pdb->p_internal_msg->p_msg_data);
                icf_uint8_t          *p_refer_to_string = ICF_NULL,
                    *p_refer_to_name = ICF_NULL,
                    *p_replaces_call_id = ICF_NULL,
                    *p_replaces_from_tag = ICF_NULL,
                    *p_replaces_to_tag = ICF_NULL;
                icf_address_st       *p_address = ICF_NULL;
                icf_replaces_hdr_st  *p_repl_hdr = ICF_NULL;
                
                SipAddrSpec             *p_addr_spec = SIP_NULL ;
                SipError                sip_error;
                SipHeader       		refer_to_hdr;
                SipReplacesHeader       *p_replaces_hdr = SIP_NULL ;
                
                api_to_invoke = ICF_NW_INC_REFER;
                
                p_internal_msg_param->bitmask = ICF_NULL;
                p_internal_msg_param->p_replaces_hdr = ICF_NULL;
                p_internal_msg_param->p_referred_to = ICF_NULL;
				p_internal_msg_param->p_referred_to_name = ICF_NULL;
                
                ICF_MEMGET(p_glb_pdb, 
                    ICF_PORT_SIZEOF(icf_address_st),
                    ICF_MEM_COMMON, p_address,
                    ICF_RET_ON_FAILURE, p_ecode, ret_val)
                    
                p_refer_to_string = p_address->addr_val.str;
                
                /* Extract Refer-To Header */
                if(SipFail == sip_getHeader(
                    pOverlapTransInfo->pSipMsg,
                    SipHdrTypeReferTo,
                    &refer_to_hdr,
                    (SipError*)(&sip_error)))
                {
                    /*Raise/SetError:ExpectedHdrAbsentInReceivedSipMsg*/
                    ICF_MEMFREE(p_glb_pdb,p_address,
                        ICF_MEM_COMMON, p_ecode, ret_val)
                        ret_val = ICF_FAILURE;
                }
                else if(Sdf_co_fail == sip_getAddrSpecFromReferToHdr(
                    &refer_to_hdr,
                    &p_addr_spec,
                    &sip_error))
                {
                    /*Raise/Set Error:UATK_PROCESSING_FAILURE*/
                    ICF_MEMFREE(p_glb_pdb,p_address,
                        ICF_MEM_COMMON, p_ecode, ret_val)
                        ret_val = ICF_FAILURE;
                }
                else if (SipFail == sip_getDispNameFromReferToHdr(
					&refer_to_hdr,
					(SIP_S8bit **)(&p_refer_to_name),
					&sip_error))
				{
					if (E_NO_EXIST != sip_error)
 					{
	                    /*Raise/Set Error:UATK_PROCESSING_FAILURE*/
    	                ICF_MEMFREE(p_glb_pdb,p_address,
	                        ICF_MEM_COMMON, p_ecode, ret_val)
	                    ret_val = ICF_FAILURE;
					}
				}

               if (ICF_SUCCESS == ret_val)
               {
                    /*Replaces parameters are not required to copy in
                     p_address*/
                    p_ssa_pdb->addr_param_not_reqd = ICF_TRUE;
               }

				if (ICF_FAILURE == ret_val)
				{
					ret_val = ICF_FAILURE;
				}
                else if (ICF_FAILURE == 
                        icf_ssa_populate_address_from_sip_addr_spec(
                            p_ssa_pdb,p_addr_spec,p_address))
                {
                    /*Raise/Set Error:UATK_PROCESSING_FAILURE*/
                    ICF_MEMFREE(p_glb_pdb,p_address,
                        ICF_MEM_COMMON, p_ecode, ret_val)
                    sip_freeSipAddrSpec(p_addr_spec); 
                    ret_val = ICF_FAILURE;
                }
                else if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                    p_ssa_pdb, 
                    ICF_INT_MODULE_CC,
                    (icf_void_t*)p_ssa_ctxt->p_call_ctxt,
                    pOverlapTransInfo->pSipMsg,
                    p_ssa_pdb->p_ecode) )
                {
                    ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
                    
                    ret_val = ICF_FAILURE;
                }
                else if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb, \
                    pOverlapTransInfo->pSipMsg, /*p_call_obj*/SipMessageRequest, \
                    &(p_ssa_ctxt->p_call_ctxt->body_list)))
                {
                    if(p_ssa_pdb->p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_415)
					{
						/* SPR 11096 Changes : Generate 415 Response and free any local 
						 * reference
						 */
				    	 if (ICF_FAILURE == icf_ssa_form_and_send_response(
                            p_ssa_pdb,415,
                            p_call_obj,
                            pOverlapTransInfo,
                            "REFER", Sdf_co_false))
						 {
                             ret_val = ICF_FAILURE;
						 }
						 else
						 {
							 ret_val = ICF_SUCCESS;
							 ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
						 }
						 /* End of SPR 11096 Changes
						  */
					}
                }
                else
                {
                    /*
                     * The function 
                     * icf_ssa_populate_address_from_sip_addr_spec
                     * doesn't populate the port if present in the
                     * SIP URI.Hence,if the address type is SIP URI 
                     * and port is present concatenate the same to the
                     * referred to address
                     */
	/*Done By Aman SPR-14057 28.12.2006*/
#if 0
                     if (SipAddrSipUri == p_addr_spec->dType) 
					{
						icf_string_st   temp_addr;			
						icf_int8_t     *p_rem_str_scln = ICF_NULL;

					    icf_port_strcpy(temp_addr.str, p_address->addr_val.str);
					    icf_port_strtokr(temp_addr.str, " ;", &p_rem_str_scln);
						icf_port_strcpy(p_address->addr_val.str, temp_addr.str);
					}
#endif
                    sip_freeSipAddrSpec(p_addr_spec); 
                    /*
                    * Hitting of this leg means that we have successfully
                    * retreived the Refer-To header(since the Refer-To
                    * is mandatory in protocol, i am assuming that it's
                    * absence would have resulted in a failure at decode
                    * itself. So the previous get Refer-To header would 
                    * have been succesful.Now we will attempt to access
                    * the Replaces header embedded in the Refer-To header
                    */
                     if(Sdf_co_fail == 
                        sdf_ivk_uaGetReplacesHeaderFromReferTo(
                        ((SipReferToHeader *)refer_to_hdr.pHeader), 
                        ((SipReplacesHeader **)&p_replaces_hdr), 
                        &sdf_error))
                   /* if(ICF_FAILURE == 
                        icf_ssa_get_replaces_header_from_referto(
                        ((SipReferToHeader *)refer_to_hdr.pHeader), 
                        ((SipReplacesHeader **)&p_replaces_hdr), 
                        &sdf_error))*/
                    {
                    /*
                    * A failure here means that either some processing of
                    * the message failed or the Replaces header is not present
                    * For the time being we assume that a failure here means
                    * that Replaces header is not present.Hence do not
                    * return failure
                        */
                        ret_val = ICF_SUCCESS ;
                    }
                    if ( ICF_NULL != p_replaces_hdr)
                    {
                        replaces_header.pHeader = p_replaces_hdr;
                        replaces_header.dType = SipHdrTypeReplaces;
                        
                        if(SipFail == 
                            sip_getCallidFromReplacesHdr(
                            (SipHeader*)&replaces_header,
                            (Sdf_ty_s8bit**)(&p_replaces_call_id),
                            &sip_error))
                        {
                            ICF_MEMFREE(p_glb_pdb,p_address,
                                ICF_MEM_COMMON, p_ecode, ret_val)
                                sip_freeSipAddrSpec(p_addr_spec);
                            ret_val = ICF_FAILURE;
                        }
                        else if(SipFail == sip_getFromTagFromReplacesHdr(
                            (SipHeader*)&replaces_header,
                            (Sdf_ty_s8bit**)(&p_replaces_from_tag),
                            &sip_error))
                        {
                            ICF_MEMFREE(p_glb_pdb,p_address,
                                ICF_MEM_COMMON, p_ecode, ret_val)
                                sip_freeSipAddrSpec(p_addr_spec);
                            ret_val = ICF_FAILURE;
                        }
                        else if(SipFail == sip_getToTagFromReplacesHdr(
                            (SipHeader*)&replaces_header,
                            (Sdf_ty_s8bit**)(&p_replaces_to_tag),
                            &sip_error))
                        {
                            ICF_MEMFREE(p_glb_pdb,p_address,
                                ICF_MEM_COMMON, p_ecode, ret_val)
                                sip_freeSipAddrSpec(p_addr_spec);
                            ret_val = ICF_FAILURE;
                        }
#ifdef ICF_KLOCWORK_TEST
						else if(ICF_NULL == p_replaces_from_tag ||
							ICF_NULL == p_replaces_to_tag)
						{
							ICF_MEMFREE(p_glb_pdb,p_address,
                                ICF_MEM_COMMON, p_ecode, ret_val)
                                sip_freeSipAddrSpec(p_addr_spec);
                            ret_val = ICF_FAILURE;
                        }
#endif
                    }
                    if(ICF_NULL != p_replaces_call_id && ICF_SUCCESS == ret_val)
                    {
                        ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_replaces_hdr_st),
                            ICF_MEM_COMMON, p_repl_hdr,
                            ICF_RET_ON_FAILURE,p_ecode, ret_val)
                            
                            p_repl_hdr->replace_nw_call_id.length = 
                            (icf_uint8_t )icf_port_strlen(p_replaces_call_id);
                        icf_port_strcpy(
                            p_repl_hdr->replace_nw_call_id.nw_call_id,
                            p_replaces_call_id);
                        
                        p_repl_hdr->from_tag.str_len = 
                            (icf_uint16_t)icf_port_strlen(p_replaces_from_tag);
                        icf_port_strcpy(
                            p_repl_hdr->from_tag.str,
                            p_replaces_from_tag);
                        
                        p_repl_hdr->to_tag.str_len = 
                            (icf_uint16_t)icf_port_strlen(p_replaces_to_tag);
                        icf_port_strcpy(
                            p_repl_hdr->to_tag.str,
                            p_replaces_to_tag);
                        
                        ICF_SSA_SET_COMMON_BITMASK(
                            p_internal_msg_param->bitmask,\
                            ICF_NW_INC_REFER_REPLACE_CALL_ID);
                        p_internal_msg_param->p_replaces_hdr = 
                            p_repl_hdr;
                            /*
                            * Now free the memory for the from 
                            * and to tags and callid
                        */
                        ICF_MEMFREE(p_glb_pdb,p_replaces_from_tag,
                            ICF_MEM_COMMON, p_ecode, ret_val)
                            ICF_MEMFREE(p_glb_pdb,p_replaces_to_tag,
                            ICF_MEM_COMMON, p_ecode, ret_val)
                            
                    }
                    /*Now assign the refer to string to the outgoing 
                    * message to CM*/
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                        (icf_uint16_t )api_to_invoke ;
                    p_internal_msg_param->p_referred_to = p_address;
                    p_internal_msg_param->bitmask |= 
                        ICF_NW_INC_REFER_REF_TO;


					if (ICF_NULL != p_refer_to_name)
					{
           				ICF_MEMGET(
                                p_ssa_pdb->p_glb_pdb,
                                ICF_PORT_SIZEOF(icf_name_st),
                                ICF_MEM_COMMON,
                                p_internal_msg_param->p_referred_to_name,
                                ICF_DONOT_RET_ON_FAILURE,
                                p_ssa_pdb->p_ecode,
                                ret_val)

                        if (ICF_SUCCESS == ret_val)
                        {
                            icf_port_strcpy(
                                    p_internal_msg_param->p_referred_to_name->str,
                                    p_refer_to_name);
                            p_internal_msg_param->p_referred_to_name->str_len =
                                (icf_uint16_t)icf_port_strlen(
                                        p_internal_msg_param->p_referred_to_name->str);

						    p_internal_msg_param->bitmask |= ICF_NW_INC_REFER_REF_TO_NAME;
                        }
					}
            }
            if (SIP_NULL != p_replaces_hdr)
            {
                sip_freeSipHeader((SipHeader*)&replaces_header);
            }
            sip_freeSipHeader((SipHeader*)&refer_to_hdr);
            }
        }
        else if ( ICF_NULL == Sdf_mc_strcmp(p_method, "NOTIFY"))
        {
            Sdf_ty_u32bit       size = 0, l_index = 0;
            Sdf_st_headerList   *p_header_list = Sdf_co_null;

            if(Sdf_co_fail == sdf_listSizeOf(
                        &pOverlapTransInfo->slUnprocessedHeaders,
                        &size, &sdf_error))
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Failure in getting size of unprocessed\
                            header list from NotifyEvent Header"));
                ret_val = ICF_FAILURE;
            }
            else if ( ICF_NULL == size)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:No header in unprocessed list for NOTIFY msg"));
                ret_val = ICF_FAILURE;
            }
            for(l_index = 0; l_index < size ;l_index++)
            {
                if(Sdf_co_fail == sdf_listGetAt(
                            &pOverlapTransInfo->slUnprocessedHeaders, l_index,
                            (Sdf_ty_pvoid *)&p_header_list, &sdf_error))
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in hdr retrieval,going for next"));
                }
                if (SipHdrTypeEvent == p_header_list->dType)
                {
                    event_header_present = ICF_TRUE;
                    break;
                }
            }
            if (ICF_FAILURE != ret_val && 
                    ICF_TRUE == event_header_present)
            {
                if(Sdf_co_fail == sdf_listSizeOf(
                            &p_header_list->slHeaders,
                            &size, &sdf_error))
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Failure in getting size of unprocessed\
                                header list from NotifyEvent Header"));
                    ret_val = ICF_FAILURE;
                }
                for(l_index = 0; l_index < size ;l_index++)
                {

                    if (Sdf_co_fail == sdf_listGetAt(
                                &(p_header_list->slHeaders), l_index,
                                (Sdf_ty_pvoid *)&p_header, &sdf_error))
                    {
                        ret_val = ICF_FAILURE;
                    }
#ifdef ICF_KLOCWORK_TEST
                    else if(SIP_NULL == p_header)
                    {
                        ret_val = ICF_FAILURE;
                    }
#endif
                    else if(ICF_NULL == (ICF_SSA_MSG_RECVD_OUT_OF_DIALOG &
                                p_ssa_pdb->common_bitmask))
                    {
                        icf_nw_inc_notify_ind_st  *p_notify_ind = 
                            (icf_nw_inc_notify_ind_st *)
                            (p_ssa_pdb->p_internal_msg->p_msg_data);

                        p_event_header = (SipEventHeader *)(p_header->pHeader);
                        p_notify_ind->p_sic_ctx =  p_ssa_ctxt->p_sic_ctxt;

#ifdef ICF_WITH_MUATK_2_2
                        if (0 == (icf_port_strcmp((icf_uint8_t *)((SipEventHeader *)
                                            (p_header->pHeader))->pStr1,(icf_uint8_t *)"refer")))
#else
                            if (0 == (icf_port_strcmp(((SipEventHeader *)
                                                (p_header->pHeader))->pEventType,"refer")))
#endif
                            {
                                /* NOTIFY with event as refer will be handled only
                                 * in two cases: a) if refer is services based i.e.
                                 * p_sic_ctx = ICF_NULL or b) if refer is SIC based 
                                 * i.e. event in the p_sic_ctx is refer */
                                if(((p_ssa_ctxt->p_sic_ctxt != ICF_NULL) &&
                                            (icf_port_strcmp((icf_uint8_t *)"refer",
                                                             p_ssa_ctxt->p_sic_ctxt->event_package.str) == 0))
                                        ||(p_ssa_ctxt->p_sic_ctxt == ICF_NULL))
                                {
                                    refer_event_present = ICF_TRUE;

                                    ret_val = icf_ssa_handle_notify_for_refer(
                                            p_ssa_pdb, p_notify_ind, 
                                            pOverlapTransInfo, 
                                            pOverlapTransInfo->pSipMsg);
                                }

                                break; /* Break the for loop */                                     
                            }
                        /*Code added for NOTIFY received for subscription*/
                            else if((p_ssa_ctxt->p_sic_ctxt != ICF_NULL) &&
                                    (icf_port_strcmp(
#ifdef ICF_WITH_MUATK_2_2
                                                     (icf_uint8_t *)( (SipEventHeader *)(p_header->pHeader) )->pStr1,
#else
                                                     ( (SipEventHeader *)(p_header->pHeader) )->pEventType,
#endif
                                                     p_ssa_ctxt->p_sic_ctxt->event_package.str) == 0)
                                   )
                            {
                                other_event_present = ICF_TRUE;

                                ret_val = 
                                    icf_ssa_handle_notify_for_subs(p_ssa_pdb, 
                                            p_notify_ind, pOverlapTransInfo,
                                            pOverlapTransInfo->pSipMsg);

                                /*If application has delinked, return*/
                                if( ICF_NULL == p_ssa_ctxt->p_sic_ctxt)
                                {
                                    other_event_present = ICF_TRUE;

                                    ret_val = 
                                        icf_ssa_handle_notify_for_subs(p_ssa_pdb, 
                                                p_notify_ind, pOverlapTransInfo,
                                                pOverlapTransInfo->pSipMsg);

                                    /*If application has delinked, return*/
                                    if( ICF_NULL == p_ssa_ctxt->p_sic_ctxt)
                                    {
                                        if( ret_val == ICF_FAILURE )
                                        {
                                            /* Abort the transaction */
                                            sdf_ivk_uaAbortTransaction((*ppCallObj),
                                                    pOverlapTransInfo->dTxnId,
                                                    (Sdf_ty_pvoid*)p_ssa_ctxt,&sdf_error);
                                            return Sdf_co_fail ;
                                        } 

                                        return ret_val;
                                    }
                                    break; /* Break the for loop and we will send 
                                            * 481 response to Notify*/

                                } /* "message_summary"))) */
                            }
                    }
                }
            }
            /* We have got all the required information till this point to
             * decide
             * how this notify is to be handled. Now we decide the action and
             * execute it*/
            if (ICF_FAILURE != ret_val)
            {
                /*check if no event header is present*/
                if(ICF_FALSE == event_header_present)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:NOTIFY received without event header"));

                    /*Send a 400 Bad request*/
                    if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                p_ssa_pdb,400,
                                p_call_obj,
                                pOverlapTransInfo,
                                "NOTIFY", Sdf_co_false))
                    {
                        ret_val = ICF_FAILURE;
                    }
                }
                /*NOTIFY_OUT_OF_DIALOG_START*/
                /*check if received out of dialog*/
                else if  ( ICF_SSA_MSG_RECVD_OUT_OF_DIALOG == 
                        (ICF_SSA_MSG_RECVD_OUT_OF_DIALOG & 
                         p_ssa_pdb->common_bitmask))
                {

                    icf_call_id_t	call_id = ICF_INVALID_CALL_ID;
                    icf_config_data_st   *p_cfg_data = ICF_NULL;

#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
                    SipReqLine               *p_req_line = SIP_NULL ; 
					SipAddrSpec              *p_req_uri = SIP_NULL;
                                             SipError                 err;
					icf_address_st           *p_user_address = ICF_NULL;
                    icf_line_id_t		 line_id = (icf_line_id_t)ICF_INVALID_LINE_ID;
                    icf_nw_inc_notify_ind_st *p_nw_inc_notify_ind = 
								   (icf_nw_inc_notify_ind_st *)
								   (p_ssa_pdb->p_internal_msg->p_msg_data);

                    p_cfg_data = p_ssa_pdb->p_glb_cfg_data;

                    /*
					 * Query DBM for the existence and availability 
					 * of the user.Extract the user address from
					 * the Req URI
					 */
					if (SipFail == sip_getReqLineFromSipReqMsg(
							pOverlapTransInfo->pSipMsg,
							&p_req_line,&err))
					{
					   ret_val = ICF_FAILURE;
					}
					else if (SipFail == sip_getAddrSpecFromReqLine(
					                  	p_req_line, &p_req_uri,&err))
					{
					   ret_val = ICF_FAILURE;
					}
					else 
					{
					   ICF_MEMGET(
						p_ssa_pdb->p_glb_pdb,
						sizeof(icf_address_st),
						ICF_MEM_COMMON,
						p_user_address,
						ICF_RET_ON_FAILURE,
						p_ssa_pdb->p_ecode,
						ret_val)
									
					   if (ICF_FAILURE == icf_ssa_populate_address_from_sip_addr_spec(
										p_ssa_pdb,p_req_uri,
										p_user_address))
					   {
                                               ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                                                             p_user_address,
                                                             ICF_MEM_COMMON,
                                                             p_ssa_pdb->p_ecode,
                                                             ret_val)

						ret_val = ICF_FAILURE;
					   }
					   else
					   {
						p_nw_inc_notify_ind->bitmask |= ICF_NW_INC_NOTIFY_USER_ADDR_PRESENT;
					   }
							
					}
					if (SIP_NULL != p_req_uri)
					{
					    sip_freeSipAddrSpec(p_req_uri);
					} /* if (SIP_NULL != p_req_uri) */
							
					/* Free references of the Req line and addr spec */
					if (SIP_NULL != p_req_line)
					{
					    sip_freeSipReqLine(p_req_line);
					} /* if (SIP_NULL != p_req_line) */
							
					if(ICF_FAILURE == ret_val)
					{
					   /* will be handled */
					}
					else if (ICF_FAILURE == icf_dbm_get_line_id_for_user(
					        		p_ssa_pdb->p_glb_pdb,
								p_user_address,
								&line_id, p_ssa_pdb->p_ecode))
					{
                                           ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                                                             p_user_address,
                                                             ICF_MEM_COMMON,
                                                             p_ssa_pdb->p_ecode,
                                                             ret_val)
					   ret_val = ICF_FAILURE;
					}
					else if(ICF_INVALID_LINE_ID == line_id)
					{
					   p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_404;
                                           ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                                                             p_user_address,
                                                             ICF_MEM_COMMON,
                                                             p_ssa_pdb->p_ecode,
                                                             ret_val)

					}
                    else if (ICF_NULL == p_ssa_pdb->p_line_data)
                    { 
                        if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                                                p_ssa_pdb->p_glb_pdb, line_id,
                                                &(p_ssa_pdb->p_line_data),
                                                p_ssa_pdb->p_ecode))
                        {
                            ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                                        p_user_address,
                                        ICF_MEM_COMMON,
                                        p_ssa_pdb->p_ecode,
                                        ret_val) 
                             ret_val = ICF_FAILURE;                                             
                         }
                    }
                          
                    if (ICF_FAILURE == ret_val)
                    {
                        return ICF_FAILURE; 
                    }

                    if(ICF_SSA_FAILURE_RESP_CODE_404 & p_ssa_ctxt->failure_bitmask)
                    {
                          if(ICF_FAILURE == icf_ssa_form_and_send_response(\
								p_ssa_pdb,404,p_call_obj,
								pOverlapTransInfo,"NOTIFY",Sdf_co_false))
                          {
					          ret_val = ICF_FAILURE;
                          }
                    }
                    else if(ICF_TRUE == p_cfg_data->non_sub_notify_enable)
                    {
                        if(p_ssa_pdb->p_line_data->default_settings &
                              ICF_SUPPORT_OUT_OF_DIALOG_NOTIFY_IGNORE_DIALOG_INFO)
                        {
                            handle_notify = ICF_TRUE;
                        }
                        else if(ICF_FALSE == p_ssa_pdb->notify_tag_mismatch)
                        {
                            handle_notify = ICF_TRUE;
                        }
                    }

                    if(ICF_TRUE == handle_notify)
                    {

                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:NOTIFY received out of dialog"));

                        /* we have to send ICF_NW_INC_NOTIFY_IND*/
                        if (ICF_INVALID_APP_ID == app_id)
                        {
                            p_ssa_ctxt->failure_bitmask |= 
                                ICF_SSA_FAILURE_RESP_CODE_481;
                            ret_val = ICF_FAILURE;
                        }
                        /*Allocate a sic context*/
                        else if (ICF_FAILURE == icf_dbm_alloc_sic_cntxt_blk(
                                    p_ssa_pdb->p_glb_pdb,
                                    &call_id,
                                    &(p_ssa_ctxt->p_sic_ctxt),
                                    p_ssa_pdb->p_ecode))
                        {
                            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInSicCtxt allocation"));
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            /* Extract the sender of this NOTIFY request */
                            SipFromHeader           *p_from = ((SipFromHeader *)
                                    (*ppCallObj)->pCommonInfo->pFrom->pHeader);

                            SipFromHeader           *p_to = ((SipFromHeader *)
                                    (*ppCallObj)->pCommonInfo->pTo->pHeader);

                            icf_config_data_st *p_glb_cfg_data = p_ssa_pdb->p_glb_cfg_data;

                            Sdf_ty_s8bit contact_addr[ICF_MAX_STR_LEN] = "\0";

							/*Fix for CSR 8012793 SPR 20221*/	
							icf_uint16_t 		contact_port = ICF_NULL;
                            icf_port_strcpy((icf_uint8_t *)contact_addr,(icf_uint8_t *)p_to->pAddrSpec->u.pSipUrl->pUser);
                            icf_port_strcat((icf_uint8_t *)contact_addr,(icf_uint8_t *)"@");
					/*Fix for CSR 7991608 SPR 20220*/
					/*If Out of Dialog NOTIFY is received when transport address is configured per line
						then while formong contact header transport address configured per line should
						be copied into Contact header*/ 
					if(ICF_CFG_SELF_ADDR_PER_LINE == p_glb_cfg_data->self_addr_per_line)
               		 {
                   		 if(ICF_NULL != p_ssa_pdb->p_line_data)
						{
					 		if ( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
									p_ssa_pdb->p_line_data->self_addr.addr.addr_type)
                            {
                                icf_port_strcat((icf_uint8_t *)contact_addr,
                                        (icf_uint8_t *)p_ssa_pdb->p_line_data->self_addr.\
                                        addr.addr.domain.str);
                            }
                            else/*Assumption that it will be IPV4*/
                            {
                                icf_uint8_t remote_ip[16];

                                if ( ICF_FAILURE == icf_ssa_convert_to_IP_addr(
                                            &(p_ssa_pdb->p_line_data->self_addr), remote_ip))
                                {
                                    ret_val = ICF_FAILURE;
                                }
                                else
                                {
                                    icf_port_strcat((icf_uint8_t *)contact_addr, (icf_uint8_t *)remote_ip);
                                }
                            }
							/*CSR 8012793 SPR 20221*/
							/*Self Port is copied into contact_port in case of per line 
							configuration*/
							contact_port = p_ssa_pdb->p_line_data->self_addr.port_num;
						}
						else
                    	{
                        	ret_val = ICF_FAILURE;
                    	}
					}
					else 
					{
                            if ( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                    p_glb_cfg_data->self_ip_address.addr.addr_type)
                            {
                                icf_port_strcat((icf_uint8_t *)contact_addr,
                                        (icf_uint8_t *)p_glb_cfg_data->self_ip_address.\
                                        addr.addr.domain.str);
                            }
                            else/*Assumption that it will be IPV4*/
                            {
                                icf_uint8_t remote_ip[16];

                                if ( ICF_FAILURE == icf_ssa_convert_to_IP_addr(
                                            &(p_glb_cfg_data->self_ip_address), remote_ip))
                                {
                                    ret_val = ICF_FAILURE;
                                }
                                else
                                {
                                    icf_port_strcat((icf_uint8_t *)contact_addr, (icf_uint8_t *)remote_ip);
                                }
                            }
							/*CSR 8012793 SPR 20221*/
							/*Self Port is copied into contact_port in case of system wide 
							configuration*/
							contact_port = p_glb_cfg_data->self_ip_address.port_num;
					}
 
                            /*In case this was a special notify which was handled only
                              because of a special request by APP then we indicate
                              the APP in the notify_ind. for this we need to mark in 
                              sic context*/
 
                            if((p_ssa_pdb->p_line_data->default_settings &
                                  ICF_SUPPORT_OUT_OF_DIALOG_NOTIFY_IGNORE_DIALOG_INFO)&&
                                ((ICF_TRUE == p_ssa_pdb->notify_tag_mismatch)))
                            {
                                ICF_SSA_SET_COMMON_BITMASK(
                                    p_ssa_ctxt->p_sic_ctxt->bitmask, 
                                    ICF_SIC_METHOD_TYPE_STRAY_NOTIFY)
                            }

                            /*As we do not have filter criteria logic for 
                             * phase I, app id is hardcoded for incoming
                             * dialogs*/
                            ICF_SSA_SET_COMMON_BITMASK(
                                    p_ssa_ctxt->p_sic_ctxt->bitmask, ICF_SIC_METHOD_TYPE_OOD_NOTIFY)

                                p_ssa_ctxt->p_sic_ctxt->app_id = app_id;
                            if (ICF_FAILURE == icf_ssa_populate_address_from_sip_addr_spec(
                                        p_ssa_pdb,p_from->pAddrSpec,
                                        &(p_ssa_ctxt->p_sic_ctxt->dest_addr)))
                            {
                                ret_val = ICF_FAILURE;
                            }
                            else
                            {
                                p_nw_inc_notify_ind->bitmask |= ICF_NW_INC_NOTIFY_SENDER_PRESENT;
                            }

                            p_ssa_ctxt->p_sic_ctxt->p_user_address = p_user_address;
                           
                            p_ssa_ctxt->p_sic_ctxt->line_id = line_id;

                                p_nw_inc_notify_ind->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                                p_nw_inc_notify_ind->p_sic_ctx->p_ssa_ctx = p_ssa_ctxt;

                                icf_port_strcpy(p_ssa_ctxt->p_sic_ctxt->event_package.str,
#ifdef ICF_WITH_MUATK_2_2
                                        (icf_uint8_t *)((SipEventHeader *)(p_header->pHeader))->pStr1);
#else
                                ((SipEventHeader *)(p_header->pHeader))->pEventType);
#endif
								/*Fix for CSR 8012793 SPR 20221*/
								/*contact_port is passed here which contains self port
								configured per line in case of per line configuration and 
								self port configured system wide in case of system
								wide configuration.Earlier self_port configured system wide 
								was passed always,even in case of per line configuration*/
                                if (Sdf_co_fail == sdf_ivk_uaAddDefaultContact(
                                            p_ssa_pdb->p_ssa_ctxt->p_call_obj->pInitData,
                                            "",
                                            contact_addr,
                                            contact_port,
                                            "sip",
                                            &sdf_error))
                                {
                                    icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb, 
                                            call_id, 
                                            p_ssa_pdb->p_ecode);

                                    ret_val = ICF_FAILURE;
                                }
                                else
                                {

                                    if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                                                p_ssa_pdb, 
                                                ICF_INT_MODULE_SIC,
                                                (icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
                                                pOverlapTransInfo->pSipMsg,
                                                p_ecode) )
                                    {
                                        p_ssa_ctxt->failure_bitmask |= 
                                        ICF_SSA_FAILURE_RESP_CODE_500;
                                        ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
                                        ret_val = ICF_FAILURE;
                                    }
                                    else
                                    {
                                        ret_val = icf_ssa_handle_notify_out_of_dialog(
                                                p_ssa_pdb, p_nw_inc_notify_ind, 
                                                pOverlapTransInfo, pOverlapTransInfo->pSipMsg);
                                    }

                                    if(p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_415)
                                    {
                                        /* spr16877:Free the memory for the pointer to the user address */
                                        if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt->p_user_address)
                                        {
                                              ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, \
                                                          p_ssa_ctxt->p_sic_ctxt->p_user_address, \
                                                          ICF_MEM_COMMON,\
                                                          p_ecode, ret_val)
                                        }
                                        icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb, 
                                                call_id, 
                                                p_ssa_pdb->p_ecode);

                                        if(ICF_FAILURE == icf_ssa_form_and_send_response(\
                                                    p_ssa_pdb,415,p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                                    pOverlapTransInfo,"NOTIFY",Sdf_co_false))
                                        {
                                            ret_val = ICF_FAILURE;
                                        }
                                        p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;

                                    }/*code end for 415*/
                                    else if(p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_488)
                                    {
                                        /* spr16877:Free the memory for the pointer to the user address */
                                        if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt->p_user_address)
                                        {
                                              ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, \
                                                          p_ssa_ctxt->p_sic_ctxt->p_user_address, \
                                                          ICF_MEM_COMMON,\
                                                          p_ecode, ret_val)
                                        }
                                        icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb,
                                            call_id,
                                            p_ssa_pdb->p_ecode);

                                        if(ICF_FAILURE == icf_ssa_form_and_send_response(\
                                            p_ssa_pdb,415,p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                            pOverlapTransInfo,"NOTIFY",Sdf_co_false))
                                        {
                                            ret_val = ICF_FAILURE;
                                        }
                                        p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;
                                    }/*code end for 415*/
                                    else if(p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_500)
                                    {
                                        /* spr16877:Free the memory for the pointer to the user address */
                                        if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt->p_user_address)
                                        {
                                              ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, \
                                                          p_ssa_ctxt->p_sic_ctxt->p_user_address, \
                                                          ICF_MEM_COMMON,\
                                                          p_ecode, ret_val)
                                        }
                                        icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb,
                                            call_id,
                                            p_ssa_pdb->p_ecode);

                                        if(ICF_FAILURE == icf_ssa_form_and_send_response(\
                                            p_ssa_pdb,500,p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                            pOverlapTransInfo,"NOTIFY",Sdf_co_false))
                                        {
                                            ret_val = ICF_FAILURE;
                                        }
                                        p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;
                                    }/*code end for 500*/ 
                                }/*success leg for sdf_ivk_uaAddDefaultContact ends*/
                            if(ICF_FAILURE == ret_val)
                            {
                                if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt->p_user_address)
                                {
                                    ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, \
                                            p_ssa_ctxt->p_sic_ctxt->p_user_address, \
                                            ICF_MEM_COMMON,\
                                            p_ecode, ret_val)
                                }  
                                icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb, 
                                        call_id, 
                                        p_ssa_pdb->p_ecode);
                                if(ICF_SSA_FAILURE_RESP_CODE_404 ==
                                        (ICF_SSA_FAILURE_RESP_CODE_404 & p_ssa_ctxt->failure_bitmask))
                                {
                                    if(ICF_FAILURE == icf_ssa_form_and_send_response(\
                                                p_ssa_pdb,404,p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                                pOverlapTransInfo,"NOTIFY",Sdf_co_false))
                                    {
                                        ret_val = ICF_FAILURE;
                                    }
                                    else
                                    {
                                        ret_val = ICF_SUCCESS;
                                        p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;
                                    }
                                }/*leg for 404 handling*/

                            }/*retval FAILURE leg*/
                        }/*sic context allocation success leg*/
                    }/*non_sub_notify_enable TRUE leg*/
                    else
                    {
                        ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                                        p_user_address,
                                        ICF_MEM_COMMON,
                                        p_ssa_pdb->p_ecode,
                                        ret_val)

                        /* set the bitmask so that in successful decode ref count of 
                           call obj is not increased -- Memory Leak Fix*/

                        /* Fix done for Nortel (SPR 16394) memory leaks in case
                         * of case RT_NON_IMS_78.It has been reverted for SPR 17157
                         */
                        
                       /* p_ssa_ctxt->bitmask |= IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV;*/
                        /*Send a 481 Service not available*/
                        if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                    p_ssa_pdb,481,
                                    p_call_obj,
                                    pOverlapTransInfo,
                                    "NOTIFY", Sdf_co_false))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;
                    }
                    ICF_CHECK_IMS_END
#endif  

                }/*Else block for SIC ctxt allocation*/
                else if ( (ICF_FALSE == refer_event_present) &&
                        (ICF_FALSE == other_event_present)) 
                {
                    /*Send a 489 Bad event*/
                    if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                p_ssa_pdb,489,
                                p_call_obj,
                                pOverlapTransInfo,
                                "NOTIFY", Sdf_co_false))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;
                }
                else if(ICF_SSA_FAILURE_RESP_CODE_415 ==
                        (ICF_SSA_FAILURE_RESP_CODE_415 & p_ssa_ctxt->failure_bitmask))

                {
                    if(ICF_FAILURE == icf_ssa_form_and_send_response(\
                                p_ssa_pdb,415,p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                                pOverlapTransInfo,"NOTIFY",Sdf_co_false))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;
                }
                else if(ICF_SSA_FAILURE_RESP_CODE_488 ==
                    (ICF_SSA_FAILURE_RESP_CODE_488 & p_ssa_ctxt->failure_bitmask))
                {
                    if(ICF_FAILURE == icf_ssa_form_and_send_response(\
                        p_ssa_pdb,488,p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                        pOverlapTransInfo,"NOTIFY",Sdf_co_false))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    p_ssa_pdb->common_bitmask |= ICF_SSA_RESPONSE_SENT_TO_NETWORK;
                }
                else if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
                {
                    if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                                p_ssa_pdb, 
                                ICF_INT_MODULE_SIC,
                                (icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
                                pOverlapTransInfo->pSipMsg,
                                p_ecode) )
                    {
                        p_ssa_ctxt->failure_bitmask |= 
                            ICF_SSA_FAILURE_RESP_CODE_500;
                        ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));

                        
                        ret_val = ICF_FAILURE;
                    }
                }

            }

      }
        /*IF block for Notify Handling*/
	else if ( ICF_NULL == Sdf_mc_strcmp(p_method, "MESSAGE"))
	{
		/*check if rceived in dialog*/
		if  ( ICF_SSA_MSG_RECVD_OUT_OF_DIALOG != 
				(ICF_SSA_MSG_RECVD_OUT_OF_DIALOG & p_ssa_pdb->common_bitmask))
		{/*Do nothing*/
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]:MESSAGE received in dialog"));
			if (ICF_FAILURE == icf_ssa_form_and_send_response(\
						p_ssa_pdb,405,p_call_obj,pOverlapTransInfo,"MESSAGE",\
						Sdf_co_false))
			{
					ret_val = ICF_FAILURE;
			}
		}
		else
		{
			/* we have to send ICF_NW_INC_MESSAGE_IND*/
			icf_call_id_t	call_id = ICF_INVALID_CALL_ID;
			SipReqLine              *p_req_line = SIP_NULL ; 
			SipAddrSpec             *p_req_uri = SIP_NULL;
			SipError                err;
			icf_address_st       user_address;
			icf_line_id_t        line_id = (icf_line_id_t)ICF_INVALID_LINE_ID ; 
			icf_uint32_t         status = ICF_STATUS_CONTINUE;
			icf_boolean_t        fail_resp_sent = ICF_FALSE;

			if (ICF_INVALID_APP_ID == app_id)
			{
				p_ssa_ctxt->failure_bitmask |= 
					ICF_SSA_FAILURE_RESP_CODE_415;
				ret_val = ICF_FAILURE;
			}
			/*ICF_SSA_TRACE(p_ssa_pdb,*/
			/*                        ICF_TRACE_OPTIONS_RCVD_OUT_OF_DIALOG)*/
			/*
			 * Query DBM for the existence and availability 
			 * of the user.Extract the user address from
			 * the Req URI
			 */
			else if (SipFail == sip_getReqLineFromSipReqMsg(
						pOverlapTransInfo->pSipMsg,
						&p_req_line,&err))
			{
				ret_val = Sdf_co_fail;
			}
			else if (SipFail == sip_getAddrSpecFromReqLine(
						p_req_line, &p_req_uri,&err))
			{
				ret_val = Sdf_co_fail;
			} 
			else if (ICF_FAILURE == 
					icf_ssa_populate_address_from_sip_addr_spec(
						p_ssa_pdb,p_req_uri,&user_address))
			{ 
				ret_val = Sdf_co_fail;
			}
			/*  
			 *  1. If status is busy ,send 486 to peer.
			 *  2. If status is idle ,proceeds to query MMI for media 
			 *  capabilities.
			 *  3. If line_id is INVALID ,send 404 to peer.
			 */
			else if (ICF_FAILURE ==
					icf_dbm_get_line_for_inc_call(
						p_glb_pdb,ICF_FALSE,&user_address,
						&line_id,&status,p_ecode))
			{
				ret_val = Sdf_co_fail;
			}
			else if (ICF_INVALID_LINE_ID == line_id)
			{
				p_ssa_ctxt->bitmask_1 &= ~ICF_MSG_RECVD_OUT_OF_DIALOG;
				/* Send 404 to peer */
				if (ICF_FAILURE == icf_ssa_form_and_send_response(
							p_ssa_pdb,404,p_call_obj,pOverlapTransInfo,
							"MESSAGE",Sdf_co_false))
				{
					ret_val = Sdf_co_fail;
				}
				else
				{
					fail_resp_sent = ICF_TRUE;
					HSS_LOCKEDDECREF((*ppCallObj)->dRefCount);
				}
			} /* else if (ICF_INVALID_LINE_I... */
			else
			{

				/*Allocate a sic context*/
				if (ICF_FAILURE == icf_dbm_alloc_sic_cntxt_blk(
							p_ssa_pdb->p_glb_pdb,
							&call_id,
							&(p_ssa_ctxt->p_sic_ctxt),
							p_ssa_pdb->p_ecode))
				{
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInSicCtxt allocation"));
					p_ssa_ctxt->bitmask_1 &= ~ICF_MSG_RECVD_OUT_OF_DIALOG;
					/* Send 500 to peer */
					if (ICF_FAILURE == icf_ssa_form_and_send_response(
							p_ssa_pdb,500,p_call_obj,pOverlapTransInfo,
							"MESSAGE",Sdf_co_false))
					{
						ret_val = Sdf_co_fail;
					}
					else
					{
						fail_resp_sent = ICF_TRUE;
						HSS_LOCKEDDECREF((*ppCallObj)->dRefCount);
					}
				}
				else
				{
					/*As we do not have filter criteria logic for 
					 * phase I, app id is hardcoded for incoming
					 * dialogs*/
					ICF_SSA_SET_COMMON_BITMASK(
							p_ssa_ctxt->p_sic_ctxt->bitmask, ICF_SIC_METHOD_TYPE_MESSAGE)

						p_ssa_ctxt->p_sic_ctxt->app_id = app_id;

					if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
								p_ssa_pdb, 
								ICF_INT_MODULE_SIC,
								(icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
								(*ppCallObj)->pUasTransaction->pSipMsg,
								p_ecode) )
					{
						p_ssa_ctxt->failure_bitmask |= 
							ICF_SSA_FAILURE_RESP_CODE_500;
						ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
					
						
						icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb, 
								call_id, 
								p_ssa_pdb->p_ecode);
						p_ssa_ctxt->bitmask_1 &= ~ICF_MSG_RECVD_OUT_OF_DIALOG;
						/* Send 500 to peer */
						if (ICF_FAILURE == icf_ssa_form_and_send_response(
							p_ssa_pdb,500,p_call_obj,pOverlapTransInfo,
							"MESSAGE",Sdf_co_false))
						{
							ret_val = Sdf_co_fail;
						}
						else
						{
							fail_resp_sent = ICF_TRUE;
							HSS_LOCKEDDECREF((*ppCallObj)->dRefCount);
						}
						
					}
					/*Message body header changes Start for 
					 *parameters
					 */
					else if (ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb, \
								(*ppCallObj)->pUasTransaction->pSipMsg,\
								/**ppCallObj*/SipMessageRequest,&(p_ssa_ctxt->p_sic_ctxt->\
									body_list)))
					{
						ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
						icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb, 
								call_id, 
								p_ssa_pdb->p_ecode);
						
						/* SPR 11096 Changes : Generate 415 Response and free any local 
						 * reference
						 */
						if(p_ssa_pdb->p_ssa_ctxt->failure_bitmask & \
								ICF_SSA_FAILURE_RESP_CODE_415)
						{
							if (ICF_FAILURE == icf_ssa_form_and_send_response(\
										p_ssa_pdb,415,p_call_obj,pOverlapTransInfo,"MESSAGE",\
										Sdf_co_false))
							{
								ret_val = ICF_FAILURE;
							}
							else
							{
								ret_val = ICF_SUCCESS;
								ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
							}
						}
						else
						{
							p_ssa_ctxt->bitmask_1 &= ~ICF_MSG_RECVD_OUT_OF_DIALOG;
							/* Send 500 to peer */
							if (ICF_FAILURE == icf_ssa_form_and_send_response(
								p_ssa_pdb,500,p_call_obj,pOverlapTransInfo,
								"MESSAGE",Sdf_co_false))
							{
								ret_val = Sdf_co_fail;
							}
							else
							{
								fail_resp_sent = ICF_TRUE;
								HSS_LOCKEDDECREF((*ppCallObj)->dRefCount);
							}
						}
						
						/* End of SPR 11096 Changes
						 */
					}
					/*Message body header changes Ends*/
					else
					{
						/* Extract the sender of this MESSAGE request */
						SipFromHeader           *p_from = ((SipFromHeader *)
								(*ppCallObj)->pCommonInfo->pFrom->pHeader);

						icf_nw_inc_msg_ind_st *p_nw_inc_msg_ind = 
							(icf_nw_inc_msg_ind_st *)
							(p_ssa_pdb->p_internal_msg->p_msg_data);

						icf_ssa_populate_address_from_sip_addr_spec(
								p_ssa_pdb,p_from->pAddrSpec,
								&(p_ssa_ctxt->p_sic_ctxt->dest_addr));


                        ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
                                   ICF_PORT_SIZEOF(icf_address_st),
                                   ICF_MEM_COMMON,
                                   p_ssa_ctxt->p_sic_ctxt->p_user_address,
                                   ICF_DONOT_RET_ON_FAILURE, 
                                   p_ssa_pdb->p_ecode, ret_val)

                        if ( ICF_SUCCESS == ret_val)
						{
						icf_ssa_populate_address_from_sip_addr_spec(
								p_ssa_pdb,p_req_uri,
								p_ssa_ctxt->p_sic_ctxt->p_user_address);
                        }

						p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;
						p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
							ICF_NW_INC_MESSAGE_IND ;
						p_nw_inc_msg_ind->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
						p_nw_inc_msg_ind->p_sic_ctx->p_ssa_ctx = p_ssa_ctxt;



					}/*Else block for fetching app specific headers*/
				}/*Else block for SIC ctxt allocation*/
			}/*Else block for handling out-of-dialog MESSAGE*/
			/* Free references of the Req line and addr spec */
			if (SIP_NULL != p_req_line)
			{
				sip_freeSipReqLine(p_req_line);
			} /* if (SIP_NULL != p_req_line) */
			if (SIP_NULL != p_req_uri)
			{
				sip_freeSipAddrSpec(p_req_uri);
			} /* if (SIP_NULL != p_req_uri) */
		}
	}
        else if (ICF_NULL == Sdf_mc_strcmp(p_method, "OPTIONS"))
        {
            icf_ssa_ctxt_st      *p_mapped_ssa_ctxt = ICF_NULL;
            icf_address_st       user_address;
            icf_line_id_t        line_id = (icf_line_id_t)ICF_INVALID_LINE_ID ; 
            icf_uint32_t         status = ICF_STATUS_CONTINUE;
            icf_boolean_t        fail_resp_sent = ICF_FALSE;
            /*
             * If icf_ssa_map_nw_call_id() is failure
             * it means that the OPTIONS request has been 
             * received out of dialog
             */
            if ( ICF_FAILURE == icf_ssa_map_nw_callid(
                    p_ssa_pdb, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_call_obj, 
#endif
                    (icf_uint8_t *)p_call_obj->pCommonInfo->pCallid,
                    &p_mapped_ssa_ctxt, 
#ifdef ICF_LOOPBACK_SUPPORT
                    &sdf_error, 
#endif
                    p_ecode))
            {
                SipReqLine              *p_req_line = SIP_NULL ; 
                SipAddrSpec             *p_req_uri = SIP_NULL;
                SipError                err;
       		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_outdlg_OPTION);
                /*ICF_SSA_TRACE(p_ssa_pdb,*/
		/*ICF_TRACE_OPTIONS_RCVD_OUT_OF_DIALOG)*/
                /*
                 * Query DBM for the existence and availability 
                 * of the user.Extract the user address from
                 * the Req URI
                 */
                if (SipFail == sip_getReqLineFromSipReqMsg(
                            pOverlapTransInfo->pSipMsg,
                            &p_req_line,&err))
                {
                    ret_val = Sdf_co_fail;
                }
                else if (SipFail == sip_getAddrSpecFromReqLine(
                            p_req_line, &p_req_uri,&err))
                {
                    ret_val = Sdf_co_fail;
                } 
                else if (ICF_FAILURE == 
                        icf_ssa_populate_address_from_sip_addr_spec(
                            p_ssa_pdb,p_req_uri,&user_address))
                { 
                    /* Start : SPR 14507 */
                    if(p_ssa_pdb->p_ssa_ctxt->failure_bitmask &
                             ICF_SSA_FAILURE_RESP_CODE_416)
                     {
                       if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                     p_ssa_pdb,416,p_call_obj,pOverlapTransInfo,
                                     "OPTIONS",Sdf_co_false))
                        {
                           ret_val = Sdf_co_fail;
                        }
                        else
                        {
                          fail_resp_sent = ICF_TRUE;
                          HSS_LOCKEDDECREF((*ppCallObj)->dRefCount);
                        }
                     }
                     else
                     {
                       ret_val = Sdf_co_fail;
                     }
                     /* End : SPR 14507 */
                }
                /*  
                 *  1. If status is busy ,send 486 to peer.
                 *  2. If status is idle ,proceeds to query MMI for media 
                 *  capabilities.
                 *  3. If line_id is INVALID ,send 404 to peer.
                 */
                else if (ICF_FAILURE ==
                        icf_dbm_get_line_for_inc_call(
                            p_glb_pdb,ICF_FALSE,&user_address,
                            &line_id,&status,p_ecode))
                {
                    ret_val = Sdf_co_fail;
                }
                else if (ICF_LINE_STATUS_BUSY == status)
                {
					/*Fix for CSR 7991608 SPR 20220*/
					/*  Assign line_id to p_ssa_pdb->p_ssa_ctxt->line_id in case out of 
					DIALOG received after establishing a call */
					p_ssa_pdb->p_ssa_ctxt->line_id = line_id;
                    /* In case app wants ICF's internal line arbitration mechanism and does
                     * not specify line_id for simultaneous calls, this function will simply 
                     * skip validation of line_id & process OPTIONS on single line */
                    if ((ICF_NULL != p_ssa_pdb->p_app_conf) && (ICF_TRUE == 
                        p_ssa_pdb->p_app_conf->app_profile.icf_simul_call_line_arbitration))
                    {
                        /* Set the bit indicating that OPTIONS has been 
                         * received out of dialog */
                        p_ssa_ctxt->bitmask |= ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG;
                    }
                    else
                    {
/*                    ICF_SSA_TRACE(p_ssa_pdb,*/
/*                            ICF_TRACE_OPTIONS_USER_BUSY)*/
                        /* Send 486 to peer */
                        if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                    p_ssa_pdb,486,p_call_obj,pOverlapTransInfo,
                                    "OPTIONS",Sdf_co_false))
                        {
                            ret_val = Sdf_co_fail;
                        }
                        else
                        {
                            fail_resp_sent = ICF_TRUE;
			  /*  HSS_LOCKEDDECREF((*ppCallObj)->dRefCount); */
                        }
                    }
                } /* else if (ICF_LINE_STATUS_BU... */
                else if (ICF_INVALID_LINE_ID == line_id)
                {
/*                    ICF_SSA_TRACE(p_ssa_pdb,*/
/*                            ICF_TRACE_OPTIONS_USER_NOT_FOUND)*/
                        /* Send 404 to peer */
                        if (ICF_FAILURE == icf_ssa_form_and_send_response(
                                    p_ssa_pdb,404,p_call_obj,pOverlapTransInfo,
                                    "OPTIONS",Sdf_co_false))
                        {
                            ret_val = Sdf_co_fail;
                        }
                        else
                        {
                            fail_resp_sent = ICF_TRUE;
			    /* HSS_LOCKEDDECREF((*ppCallObj)->dRefCount); */
                        }
                } /* else if (ICF_INVALID_LINE_I... */
                else
                {
                    /*
                     * Set the bit indicating that OPTIONS has
                     * been received out of dialog.
                     */
                    p_ssa_ctxt->bitmask |= 
                        ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG;
			/*Fix for CSR 7991608 SPR 20220*/
                p_ssa_pdb->p_ssa_ctxt->line_id = line_id; /*SPR fix:17648.  Assign line id to p_ssa_pdb->p_ssa_ctxt->line_id
                                                          in case of out of dialog options */

                }
                /* Free references of the Req line and addr spec */
                if (SIP_NULL != p_req_line)
                {
                    sip_freeSipReqLine(p_req_line);
                } /* if (SIP_NULL != p_req_line) */
                if (SIP_NULL != p_req_uri)
                {
                    sip_freeSipAddrSpec(p_req_uri);
                } /* if (SIP_NULL != p_req_uri) */
            }
            else
            {
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.stat_inbound_req.stats_indlg_OPTION);
#ifdef ICF_TRACE_ENABLE	
                p_ssa_pdb->dialog_status=1;
                ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_INC_OPTIONS)
#endif
            }
#ifdef ICF_LOAD_STAT
           /*Incoming OPTIONS attempts*/
           /*inc_opt_attempts++*/
           p_glb_pdb->load_data.inc_options_attempts++;
           if (ICF_TRUE == fail_resp_sent)
           {
                /*Failure sent opt_fail_wo_app*/
                p_glb_pdb->load_data.inc_options_failure_wo_app++;
           }
#endif
            if ((ICF_FALSE == fail_resp_sent) &&
                    (Sdf_co_fail != ret_val))
            {
		/* ICF_6_1_OPTIONS_SUPPORT start */
		/* From ICF 6.1 ssa will send incoming options ind to 
 		 * SIC */
		p_ssa_ctxt->bitmask |= ICF_SSA_OPTIONS_ONGOING;
		ret_val = icf_ssa_handle_successful_incoming_options_req(
				p_ssa_pdb,ppCallObj,pOverlapTransInfo);
		/* ICF_6_1_OPTIONS_SUPPORT end */

            }
            if (Sdf_co_fail == ret_val)
            {
                /* 
                 * Reset the mapping in the map maintained 
                 * in the SSA global data.Also reset 
                 * other OPTIONS specific data from context
                 */
                icf_ssa_flush_entry_frm_map(p_ssa_pdb,p_ssa_ctxt);
            }
        }
        else if (ICF_NULL == Sdf_mc_strcmp(p_method, "INFO"))
        { 
			/*HANDLE_INFO_IND_START*/
			if  ( ICF_SSA_MSG_RECVD_OUT_OF_DIALOG == 
				(ICF_SSA_MSG_RECVD_OUT_OF_DIALOG & 
				p_ssa_pdb->common_bitmask) )
			{
				p_ssa_ctxt->failure_bitmask |= 
					ICF_SSA_FAILURE_RESP_CODE_481;
				ICF_PRINT(((icf_uint8_t *)"\nINFO Received out of Dialog\n"));
				if (ICF_FAILURE == 
					icf_ssa_form_and_send_response(
					p_ssa_pdb,481,p_call_obj,
					pOverlapTransInfo,"INFO",Sdf_co_false))
				{
					ret_val = ICF_FAILURE;
				}
			}
			else if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
				p_ssa_pdb, 
				ICF_INT_MODULE_CC,
				(icf_void_t*)p_ssa_ctxt->p_call_ctxt,
				pOverlapTransInfo->pSipMsg,
				p_ecode) )
			{
				p_ssa_ctxt->failure_bitmask |= 
					ICF_SSA_FAILURE_RESP_CODE_500;
				ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
				ret_val = ICF_FAILURE;
			}
			else if (ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb, \
				pOverlapTransInfo->pSipMsg,\
				/**ppCallObj*/SipMessageRequest,&(p_ssa_ctxt->p_call_ctxt->\
				body_list)))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
				p_ssa_ctxt->failure_bitmask |= 
					ICF_SSA_FAILURE_RESP_CODE_415;
				ret_val = ICF_FAILURE;
			}
			else if(p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_415)
			{
				if (ICF_FAILURE == 
					icf_ssa_form_and_send_response(
					p_ssa_pdb,415,p_call_obj,
					pOverlapTransInfo,"INFO",Sdf_co_false))
                                {
                                     ret_val = ICF_FAILURE;
                                }
			}
            else if(p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_488)
            {
                if (ICF_FAILURE ==
                    icf_ssa_form_and_send_response(
                    p_ssa_pdb,488,p_call_obj,
                    pOverlapTransInfo,"INFO",Sdf_co_false))
                {
                    ret_val = ICF_FAILURE;
                }
            }
            else if(p_ssa_ctxt->failure_bitmask & ICF_SSA_FAILURE_RESP_CODE_500)
            {
                if (ICF_FAILURE ==
                    icf_ssa_form_and_send_response(
                    p_ssa_pdb,500,p_call_obj,
                    pOverlapTransInfo,"INFO",Sdf_co_false))
                {
                    ret_val = ICF_FAILURE;
                }
            }
			else
			{
					/*ret_val = icf_ssa_handle_info_ind(p_ssa_pdb,p_nw_inc_info_ind,
				pOverlapTransInfo);*/
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
					ICF_NW_INC_INFO_IND ;
				p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
					ICF_PORT_SIZEOF(icf_nw_inc_info_ind_st);
			        if (ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx)
                                {
                                    if (p_ssa_pdb->p_glb_pdb->p_call_ctx->pending_info_cfm)
                                    {
                                        if (ICF_FAILURE == 
					        icf_ssa_form_and_send_response(
					                  p_ssa_pdb,400,p_call_obj,
					                  pOverlapTransInfo,"INFO",Sdf_co_false))
				        {
					     ret_val = ICF_FAILURE;
				        }  
                                    }
                                }	
#ifdef ICF_REL_OLD
/* This is being put under ICF_REL_OLD flag so that 200 OK is not issued on n/w just on receiving
 * INFO requets. */
				if (ICF_FAILURE == 
					icf_ssa_form_and_send_response(
					p_ssa_pdb,200,p_call_obj,
					pOverlapTransInfo,"INFO",Sdf_co_false))
				{
					ret_val = ICF_FAILURE;
				}
		
#endif		
			}/*Else block for fetching app specific headers*/
        } /* else if (ICF_NULL == Sdf_mc... */
    }/* end fail_resp_sent == false */
    }/*If -after successful retreival of method*/
       /* Collect stats of different INBOUND request messages*/

                if(ICF_NULL == Sdf_mc_strcmp(p_method,"INFO"))
                {
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_inbound_req.stats_INFO);
                }

                else if(ICF_NULL == Sdf_mc_strcmp(p_method,"REFER"))
                {
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_inbound_req.stats_REFER);
                }

                else if(ICF_NULL == Sdf_mc_strcmp(p_method,"NOTIFY"))
                {
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_inbound_req.stats_NOTIFY);
                }

                else if(ICF_NULL == Sdf_mc_strcmp(p_method,"UPDATE"))
                {
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_inbound_req.stats_UPDATE);
                }

                else if(ICF_NULL == Sdf_mc_strcmp(p_method,"MESSAGE"))
                {
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_inbound_req.stats_MESSAGE);
                }

	ICF_FUNCTION_EXIT(p_glb_pdb)
	if (ICF_FAILURE == ret_val)
	{
		/* Abort the transaction */
		sdf_ivk_uaAbortTransaction((*ppCallObj),
               pOverlapTransInfo->dTxnId,
               (Sdf_ty_pvoid*)p_ssa_ctxt,&sdf_error);
		return Sdf_co_fail ;
	} /* if (ICF_FAILURE == ret_val) */
	else
	{
		if(ICF_NULL != p_inc_media_sdp_info)
		{
			p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
			p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;
			ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
             p_inc_media_sdp_info,
             ICF_MEM_COMMON,
             p_ssa_pdb->p_ecode,
             ret_val)
		}
		return Sdf_co_success;
	}
}

/*********************************************************
 ** FUNCTION: sdf_cbk_uaReInviteAcknowledged
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **              ACK message to a final repsonse to a reinvite
 **
 **********************************************************/
Sdf_ty_retVal sdf_cbk_uaReInviteAcknowledged(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
   
    icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    icf_nw_inc_media_change_ack_st      *p_internal_msg_param = ICF_NULL;

    /* To remove compiler warning */
    (void)ppCallObj;    
    (void)pErr;    

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the SSA context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;
    
    /* Cast the payload into the icf_nw_inc_media_change_req_st structure */
    p_internal_msg_param = 
        (icf_nw_inc_media_change_ack_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    p_internal_msg_param->bitmask = ICF_NULL;
    p_internal_msg_param->p_sdp_list = ICF_NULL;
  
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
     ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.
                                         stat_inbound_req.stats_ACK);
        
        /* Check for the presence of SDP in the received message */
    if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
            ((*ppCallObj)->pUasTransaction->pSipMsg,pErr))
    {
        /*
        * SSA is not using any media handling related
        * functionality provided by the UATK.
        * On receiving SDP in any SIP message,SSA
        * converts the SDP into the generic SDP structure
        * of the call context and then either populates
        * the p_remote_received_sdp structure of the call
        * context or passes this SDP in the internal message
        * Since the SDP structure of the call context doesn't
        * have a mode field, SSA will be calling the SDP
        * conversion function with the pointer to the mode
        * variable defined below.The SDP conversion function
        * will extract the mode from the SDP and store
        * it into this variable.
            */
            if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                p_ssa_pdb,\
                (*ppCallObj)->pUasTransaction->pSipMsg,
                ICF_SSA_RECVD_ANSWER_TO_OFFER,\
                &p_inc_media_sdp_info,\
                p_ssa_pdb->p_ecode))	
            {
                if(ICF_NULL != p_inc_media_sdp_info)
                {
                    /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                    icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                        &p_inc_media_sdp_info);
                }
				 /* If 415 bitmask is set then send generic call clear indication
                  */
                if((ICF_SSA_FAILURE_RESP_CODE_415 & p_ssa_pdb->p_ssa_ctxt->\
                     failure_bitmask)||
                   (ICF_SSA_FAILURE_RESP_CODE_488 & p_ssa_pdb->p_ssa_ctxt->\
                     failure_bitmask))
                 {
                      p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                      p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                           ICF_GENERIC_CALL_CLEAR;
                      p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                           p_ssa_pdb->p_glb_pdb;                            
                      p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
    	            ret_val = Sdf_co_success;
                  }
                else
				{
					ret_val = Sdf_co_fail;
				}
            }			
            else
            {
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                                sizeof(icf_nw_inc_media_change_ack_st);
                
                if(ICF_NULL != p_inc_media_sdp_info->count)
                { 
                    icf_uint8_t count = 0;
                    
                    p_internal_msg_param->bitmask |= \
                        ICF_NW_INC_MC_ACK_REJECTED_STREAM;
                    
                    while(count != p_inc_media_sdp_info->count)
                    {
                        p_internal_msg_param->rejected_streams[count] =\
                            p_inc_media_sdp_info->rejected_streams[count];
                        count+=1;  
                    }    
                }
                
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP received in ACK for REINVITE"));
                
                /*
                * Set the bit which indicates that ACK for REINVITE
                * has been received with SDP
                */
                
                if(ICF_NULL != p_inc_media_sdp_info->p_sdp_info_list)
				{
                	p_internal_msg_param->p_sdp_list =\
                    	p_inc_media_sdp_info->p_sdp_info_list;
					ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                    	ICF_NW_INC_MC_ACK_SDP_PRESENT);
	
				}
                    /* 
                    * The memory allocated for the SDP has to be freed
                    * by the SSA on the return path.
                    * For this purpose,SSA is maintaining the list
                    * of all such memory chunks which have to be freed 
                    * in the return path.The memory acquired for the SDP
                    * is one such memory chunk.Add this memory into the 
                    * allocated_memory list maintained in the SSA PDB
                */
                /*ICF_SSA_ADD_MEM_CHUNK_TO_PTR_LIST(p_ssa_pdb,
                    &p_internal_msg_param->p_sdp_list,
                    ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST)*/
                    /* 
                    * Delete the message body list stored in the UAC
                    * transaction of the call object.
                    * This is done to flush the SDP body from the 
                    * transaction substructure of the call object
                    */
                    sdf_listDeleteAll(
                    &(*ppCallObj)->pUasTransaction->slMsgBodyList,
                    pErr);
                
            }
    } /* ((*ppCallObj)->pUacTransaction... */
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP not received in ACK for ReInvite"));
    }

    if(p_ssa_pdb->p_ssa_ctxt->bitmask & ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP)
    {
        p_ssa_pdb->p_ssa_ctxt->bitmask &= ~ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP;
    }

	if(ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id)
	{
        /* Populate the internal message header */
        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_MEDIA_CHANGE_ACK ;
       
        /*
         * All messages to be sent to CC/SC are routed to the CM.
         * CM decides the specific destination of these messages
         */
        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
	}
	if(ICF_FAILURE != ret_val)
	{
		if(ICF_NULL != p_inc_media_sdp_info)
		{
			p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
			p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;
			ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
           	  p_inc_media_sdp_info,
            	 ICF_MEM_COMMON,
          	   p_ssa_pdb->p_ecode,
            	 ret_val)
		}
	}

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


#ifdef SDF_IM

Sdf_ty_retVal sdf_cbk_uaResolveImUrl (
									  Sdf_ty_s8bit *pImUrl, 
									  Sdf_ty_s8bit **ppReslUrl, 
									  Sdf_st_error *pErr)
{
(void)pImUrl;
(void)ppReslUrl;
(void)pErr;
	return Sdf_co_fail;
}

#endif


/*********************************************************
* FUNCTION: sdf_cbk_uaUnknownMethodReceived
*
* DESCRIPTION: As we have set some methods in the user 
*              profile, any unknown method will not be passed to 
*              SSA in this callback.
*
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaUnknownMethodReceived(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_overlapTransInfo *pOverlapTransInfo,
        Sdf_st_error *pErr)
{
    /* To remove compiler warning */
    (void)ppCallObj;    
    (void)pEventContext;    
    (void)pOverlapTransInfo;    
    (void)pErr;    

    return Sdf_co_success;
}
/*****************************************************************************
 ** FUNCTION:		icf_ssa_handle_successful_incoming_options_req
 **
 * DESCRIPTION:		This function will send an internal
 *					Event to SIC module. 
 *
 ****************************************************************************/
icf_return_t icf_ssa_handle_successful_incoming_options_req(
	icf_ssa_pdb_st *p_ssa_pdb,
	Sdf_st_callObject **ppCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransInfo)
{

	icf_call_id_t	call_id = ICF_INVALID_CALL_ID;
	icf_ssa_ctxt_st *p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_int_filter_st  temp_filter  ;
	icf_string_node_st string_node ;
	icf_boolean_t token_found = ICF_FALSE;
	icf_app_id_t app_id = ICF_INVALID_APP_ID;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

	string_node.p_str=ICF_NULL;
    string_node.p_next=ICF_NULL;
    *ppCallObj = *ppCallObj;
#ifdef ICF_LOAD_STAT
    p_ssa_pdb->p_glb_pdb->load_data.options_attempts++;
#endif
	if (ICF_NULL != p_ssa_pdb->p_app_conf)
        {
            app_id = p_ssa_pdb->p_app_conf->app_id;
        }
	else if ( ICF_NULL != p_ssa_ctxt->p_call_ctxt)
	{
	    app_id = p_ssa_ctxt->p_call_ctxt->app_id;
	}
	else if ( ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
	{
	    app_id = p_ssa_ctxt->p_sic_ctxt->app_id;
	}

	string_node.p_next = ICF_NULL;
	string_node.p_str = (icf_uint8_t *)"application/sdp";
	
	/* If Accept header contains applicaiton/sdp then SSA will set
	 * is_sdp_reqd flag to true in icf_nw_inc_opt_ind_st */
	temp_filter.filter_expr.count = 1;
	temp_filter.filter_expr.p_match_str_list = &string_node;
	temp_filter.filter_expr.match_all = ICF_TRUE;
	temp_filter.filter_type=ICF_FILTER_BY_HEADER_CONTENT;
	temp_filter.match_type=ICF_SUB_STRING_MATCH;
	temp_filter.p_next=ICF_NULL;
	icf_port_strcpy(temp_filter.target_header.str,(icf_uint8_t *)"Accept");
	if (ICF_FAILURE == 
		icf_ssa_match_header_filter(pOverlapTransInfo->pSipMsg,&temp_filter,&token_found,p_ssa_pdb->p_ecode))
	{
		ret_val = Sdf_co_fail;
	}
	else if (ICF_NULL != (p_ssa_ctxt->bitmask &
		ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG))
	{
		/*Add the SSA ctxt to CallId mapping in the SSA DB*/
		if (ICF_FAILURE == 
			icf_ssa_add_callid_callobj_map(
			p_ssa_pdb,
			p_ssa_ctxt,p_ssa_pdb->p_ecode))
		{
			ret_val = Sdf_co_fail;
		}
                if (ICF_FAILURE == icf_dbm_alloc_sic_cntxt_blk(
                        p_ssa_pdb->p_glb_pdb,
                        &call_id,
                        &(p_ssa_ctxt->p_sic_ctxt),
                        p_ssa_pdb->p_ecode))
                {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInSicCtxt allocation"));
                        ret_val = ICF_FAILURE;
                }
                 p_ssa_ctxt->p_sic_ctxt->options_timer_id =
                        ICF_TIMER_ID_INVALID;
                p_ssa_ctxt->p_sic_ctxt->p_options_timer =
                 ICF_NULL;
                 p_ssa_ctxt->p_sic_ctxt->p_mm_media_capab =
                         ICF_NULL;
                 p_ssa_ctxt->p_sic_ctxt->pending_event = 0;
                 p_ssa_ctxt->p_sic_ctxt->sic_state =
                        ICF_SIC_00_OPTIONS_IDLE;
		/*Fix for CSR 7991608 SPR 20220*/
		/*EGT was not sending 200 OK for out of dialog OPTIONS as no line_id
		was available in sic_ctx*/ 
		p_ssa_ctxt->p_sic_ctxt->line_id =p_ssa_pdb->p_ssa_ctxt->line_id; 

	}
	else if ( ICF_NULL == p_ssa_ctxt->p_sic_ctxt &&
	     ICF_NULL != p_ssa_ctxt->p_call_ctxt)
	{
		call_id = p_ssa_ctxt->p_call_ctxt->call_id;
		/*Allocate a sic context*/
		if (ICF_FAILURE == icf_dbm_alloc_sic_cntxt_blk(
			p_ssa_pdb->p_glb_pdb,
			&call_id,
			&(p_ssa_ctxt->p_sic_ctxt),
			p_ssa_pdb->p_ecode))
		{
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInSicCtxt allocation"));
			ret_val = ICF_FAILURE;
		}
	 	 p_ssa_ctxt->p_sic_ctxt->options_timer_id = 
        	        ICF_TIMER_ID_INVALID;
       		p_ssa_ctxt->p_sic_ctxt->p_options_timer =
       	         ICF_NULL;
       		 p_ssa_ctxt->p_sic_ctxt->p_mm_media_capab = 
       		         ICF_NULL;
       		 p_ssa_ctxt->p_sic_ctxt->pending_event = 0;
		 p_ssa_ctxt->p_sic_ctxt->sic_state =
			ICF_SIC_00_OPTIONS_IDLE;
	}
	else if ( ICF_NULL == p_ssa_ctxt->p_sic_ctxt)
	{
		ret_val = Sdf_co_fail;
	}
	if ( Sdf_co_fail != ret_val)
	{
		if (ICF_INVALID_APP_ID == app_id)
		{
			p_ssa_ctxt->failure_bitmask |= 
				ICF_SSA_FAILURE_RESP_CODE_415;
 			if (ICF_NULL != (p_ssa_ctxt->bitmask &
                		ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG))
                        {
                           icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb,
                                        call_id,
                                        p_ssa_pdb->p_ecode);
                        }

			ret_val = ICF_FAILURE;
		}
		else if ( ICF_NULL == p_ssa_ctxt->p_sic_ctxt)
		{
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SIC Context is NULL."));
			ret_val = ICF_FAILURE;
		}
		else
		{
			ICF_SSA_SET_COMMON_BITMASK(
				p_ssa_ctxt->p_sic_ctxt->bitmask, ICF_SIC_METHOD_TYPE_OPTIONS)
				
				p_ssa_ctxt->p_sic_ctxt->app_id = app_id;
			
			if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
				p_ssa_pdb, 
				ICF_INT_MODULE_SIC,
				(icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
				pOverlapTransInfo->pSipMsg,
				p_ssa_pdb->p_ecode) )
			{
				p_ssa_ctxt->failure_bitmask |= 
					ICF_SSA_FAILURE_RESP_CODE_500;
				ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
				if (ICF_NULL != (p_ssa_ctxt->bitmask &
		ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG))		
				{
				icf_dbm_dealloc_sic_cntxt_blk(p_ssa_pdb->p_glb_pdb, 
					call_id, 
					p_ssa_pdb->p_ecode);
				}	
				ret_val = ICF_FAILURE;
			}
			else
			{
                                icf_nw_inc_opt_ind_st *p_nw_inc_opt_ind = 
					(icf_nw_inc_opt_ind_st *)
					(p_ssa_pdb->p_internal_msg->p_msg_data);

				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
					ICF_NW_INC_OPTIONS_IND ;
				p_nw_inc_opt_ind->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
				p_nw_inc_opt_ind->p_sic_ctx->p_ssa_ctx = p_ssa_ctxt;
				p_nw_inc_opt_ind->is_sdp_reqd = token_found;
				if (ICF_NULL != (p_ssa_ctxt->bitmask &
					ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG))
				{
					/*
					 * Increment the refcount of the callobject
					 * so that the same is not freed in the return
					 * path. Also set the bit in the common bitmask
					 * indicating that an inter modular message 
					 * needs to be sent.
					 */
					/*HSS_LOCKEDINCREF(p_call_obj->dRefCount);*/
					p_ssa_pdb->common_bitmask |=
						ICF_SSA_INTER_MODULAR_MSG;
                                        
#ifdef ICF_TRACE_ENABLE
                                        p_ssa_pdb->dialog_status=0;  
                                        /* Print traces for out of dialog options from network */
                                        ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_INC_OPTIONS)
#endif

				}

			}
		}
	}
  
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}
#ifdef ICF_SESSION_TIMER

/*****************************************************************************
 ** FUNCTION:       icf_ssa_session_timer_set 
 **
 * DESCRIPTION:     this function sets the session timer values in case
 *                  the initial INVITE or Re-INVITE from n/w is recvd without
 *                  se header. In both these cases we have to start session 
 *                  timer on our part
 *
 ****************************************************************************/

icf_return_t   icf_ssa_session_timer_set(icf_ssa_pdb_st *p_ssa_pdb)
{
    Sdf_st_callObject      *pCallObj = Sdf_co_null;
    Sdf_ty_refresher       refresher;
    icf_return_t           ret_val = ICF_SUCCESS;
    Sdf_st_error           sdf_error;
    SipMessage             *p_message = ICF_NULL;
    icf_session_timer_data_st      *p_session_timer_data = ICF_NULL;

    if (ICF_NULL == p_ssa_pdb ||
        ICF_NULL == p_ssa_pdb->p_ssa_ctxt ||
        ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)
    {
       return Sdf_co_fail;
    }

    if (ICF_FALSE == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->session_timer_enable)
    {
       ICF_PRINT(((icf_uint8_t *)"\nSession Timer is Disabled,this function should not get hit.\n"))
       return Sdf_co_fail;
    }
    if ((ICF_TRUE == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->session_timer_enable) &&
        (ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->p_session_timer_data))
    {
       ICF_PRINT(((icf_uint8_t *)"\nSession Timer is Either Disabled or sesion timer data is unavailable.\n"))
       return Sdf_co_fail;
    }
   
   ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

   p_session_timer_data = p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->p_session_timer_data;
   pCallObj = p_ssa_pdb->p_ssa_ctxt->p_call_obj ;

   if(Sdf_co_success == sdf_ivk_uaGetRefresher(pCallObj,&refresher,
                                     (Sdf_st_error*)p_ssa_pdb->p_ecode))
   {
         if(refresher == Sdf_en_refresherNone && 
         	    ICF_SESSION_REFRESHER_NONE != 
 		     p_session_timer_data->session_timer_refresher) 
           {
                if(Sdf_co_fail == sdf_ivk_uaSetRefresher(pCallObj,
                                      Sdf_en_refresherLocal,
                                      (Sdf_st_error*)&sdf_error ))
                {
                    ret_val = Sdf_co_fail;
                }
            }
            
            if((refresher == Sdf_en_refresherLocal)||
               (refresher == Sdf_en_refresherNone && 
			    ICF_SESSION_REFRESHER_NONE != 
			    p_session_timer_data->session_timer_refresher))
            {
                /* SPR 9591 */
                p_message = pCallObj->pUasTransaction->pSipMsg;
 
                icf_ssa_set_refresher_method(p_message,
                        p_ssa_pdb->p_glb_pdb,
                        &p_ssa_pdb->p_ssa_ctxt,
                        (Sdf_st_error*)&sdf_error);

                pCallObj->pSessionTimer->dRefreshRequest =
                    p_ssa_pdb->p_ssa_ctxt->refresher_method_type;

                if(Sdf_co_fail == sdf_ivk_uaSetSessionInterval(pCallObj,
                                    p_session_timer_data->session_timer, 
                                    (Sdf_st_error*)&sdf_error))
                {
                    (pCallObj)->pSessionTimer->dSessionInterval = 
                        p_session_timer_data->session_timer;

                    (pCallObj)->pSessionTimer->dRefreshRequest = 
                        Sdf_en_refresherInvite;
                }
            }
        }
        else
        {
            ret_val = Sdf_co_fail;
        }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb);
    return ret_val;
}

icf_return_t   icf_ssa_session_timer_get(icf_ssa_pdb_st *p_ssa_pdb)
{
 Sdf_st_callObject      *pCallObj = Sdf_co_null;
 Sdf_ty_u32bit          session_timer;
 icf_return_t           ret_val = ICF_SUCCESS;
 Sdf_st_error           sdf_error;
 icf_session_timer_data_st      *p_session_timer_data = ICF_NULL;

 if (ICF_NULL == p_ssa_pdb ||
     ICF_NULL == p_ssa_pdb->p_ssa_ctxt ||
     ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)
 {
    return ICF_FAILURE;
 }
 if (ICF_FALSE == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->session_timer_enable)
 {
    ICF_PRINT(((icf_uint8_t *)"\nSession Timer is Disabled,this function should not get hit.\n"))
    return ICF_FAILURE;
 }
 if ((ICF_TRUE == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->session_timer_enable) &&
     (ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->p_session_timer_data))
 {
    ICF_PRINT(((icf_uint8_t *)"\nSession Timer is Either Disabled or sesion timer data is unavailable.\n"))
    return ICF_FAILURE;
 }
 ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

 pCallObj = p_ssa_pdb->p_ssa_ctxt->p_call_obj ;
 p_session_timer_data = p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->p_session_timer_data;

 if(sdf_ivk_uaGetSessionInterval(pCallObj,&session_timer,&sdf_error)!= Sdf_co_fail)
 { 
      if((p_session_timer_data->min_se != ICF_NULL) &&
            (0 != session_timer) &&
            (p_session_timer_data->min_se > session_timer))
      {
			/* SPR 19759: Set the MinSE value as present in the sessio timer
             * data so that while rejecting INVITE, MinSE is correctly
             * populated in the 422 error response.
             */ 
            pCallObj->pSessionTimer->dMinSe = p_session_timer_data->min_se;

            if ( Sdf_co_fail != sdf_ivk_uaFormResponse(422, "INVITE",
                                       pCallObj,
                                       Sdf_co_null,
                                       Sdf_co_false,
                                       &sdf_error))
            {
                 icf_ssa_send_to_peer(p_ssa_pdb,
                                      pCallObj,
                                      pCallObj->pUasTransaction->pSipMsg,
                                      ICF_NULL,&sdf_error);

            }
            ret_val = ICF_FAILURE;
       }
   }
  ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb);
  return ret_val;
}


icf_return_t   icf_ssa_session_timer_get_update(icf_ssa_pdb_st          *p_ssa_pdb,
                                                Sdf_st_overlapTransInfo *pOverlapTransInfo)
{
 Sdf_st_callObject      *pCallObj = Sdf_co_null;
 Sdf_ty_u32bit          session_timer;
 icf_return_t           ret_val = ICF_SUCCESS;
 Sdf_st_error           sdf_error;
 icf_session_timer_data_st      *p_session_timer_data = ICF_NULL;

 if (ICF_NULL == p_ssa_pdb ||
     ICF_NULL == p_ssa_pdb->p_ssa_ctxt ||
     ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)
 {
     return ICF_FAILURE;
 }
 if (ICF_FALSE == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->session_timer_enable)
 {
    ICF_PRINT(((icf_uint8_t *)"\nSession Timer is Disabled,this function should not get hit.\n"))
    return ICF_FAILURE;
 }
 if ((ICF_TRUE == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->session_timer_enable) &&
     (ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->p_session_timer_data))
 {
    ICF_PRINT(((icf_uint8_t *)"\nSession Timer is Either Disabled or sesion timer data is unavailable.\n"))
    return ICF_FAILURE;
 }
 ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
 p_session_timer_data = p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->p_session_timer_data;
 pCallObj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;

 if(sdf_ivk_uaGetSessionInterval(pCallObj,&session_timer,&sdf_error)!= Sdf_co_fail)
  { 
        if((p_session_timer_data->min_se != ICF_NULL) &&
              (0 != session_timer) &&
              (p_session_timer_data->min_se > session_timer))
         {
              icf_ssa_form_and_send_response(p_ssa_pdb,
                                               422,
                                               pCallObj,
                                               pOverlapTransInfo,
                                               "UPDATE",
                                               Sdf_co_false);
                                               
                ret_val = ICF_FAILURE;
          }
   }
  ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb);
  return ret_val;
}
#endif
