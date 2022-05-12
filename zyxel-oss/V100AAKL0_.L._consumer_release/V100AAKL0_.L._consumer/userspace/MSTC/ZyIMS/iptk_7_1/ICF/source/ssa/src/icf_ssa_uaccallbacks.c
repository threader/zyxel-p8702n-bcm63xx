/*********************i********************************************************
 ** FUNCTION:
 **                     This file contains the implementation for SIP UA toolkit
 **                     callbacks for UA Client.
 **
 *****************************************************************************
 **
 ** FILENAME:        icf_ssa_uaccallbacks.c
 **
 ** DESCRIPTION:    This file contains implementation for the
 **                 SIP UA toolkit related callbacks for UAC
 **                    functionality.
 **    
 ** DATE             NAME             REFERENCE         REASON
 ** ----             ----             ---------         ------
 ** 22-JAN-2005     Umang Singh      ICF LLD      INITIAL CREATION
 ** 29-JAN-2005     Aashish Suchdev
 ** 08-JUN-2005     Jalaj Negi       IMS CLIENT      Code Change for IMS 
 **                                                  CLIENT
 ** 22-JUN-2005     Umang Singh      SPR 7665             
 ** 12-AUG-2005     Shekhar Thakur      ICF          ICF additions 
 ** 23-AUG-2005		Jyoti Narula		ICF			 Handling of 200/423 for ICF
 ** 10-SEP-2005     Ram Narayan                      min-expires header was used
 **                                                  without being initialized.
 ** 12-SEP-2005     Ram Narayan                      P-Associated-URI Header was 
 **                                                  not freed properly, and was 
 **                                                  causing core dump
 ** 12-SEP-2005     Ram Narayan                      Core dump if P-Associated-URI 
 **                                                  Header parsing failure
 **
 ** 05-Dec-2005	    Jyoti Narula     ICF 4.0 CDD     Changes under 
 **													 ICF_4_0_CRM_START and END
 ** 09-DEC-2005     Jalaj Negi       ICF_4_0         Function call for 
 **                                                  extracting 
 **													 P-Media-Authorization
 **                                                  token for QoS support
 **	14-DEC-2005		Aman Arora		 ICF_4.0 CDD	 Added code for IPsec 
 **													 feature
 **	22-Feb-2005		Amit Sharma                      Added code for auth support
 ** 03-Mar-2006     Amit Sharma                      ICF merging with ICF2.0
 ** 23-Mar-2006     Amit Sharma                      Code merged with ICF 4.2
 ** 02-Apr-2006		Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
 **											 Multiple Public Users ICF5.0
 **
 ** 29-July-2006    Aman Aggarwal    ICF_6_0         Multiple m-lines changes
 **                 Deepti Goyal 
 ** 25-Aug-2006     Amit Sharma      SPR 12553
 **                 Umang Singh
 ** 28-Aug-2006		Deepti Goyal	 SPR 12605		Memory Leaks in OTG Call Fixed.
 ** 05-Sep-2006	    Umang Singh      SPR 12836	
 ** 06-Sep-2006     Umang Singh      SPR 12855		Parsing of Require hdr for 100rel
 **
 ** 06-Sep-2006     Deepti Goyal     SPR 12699      Call Clear Indication when 
 **                                                 200 OK for Update/Re-Invite
 **                                                 received without SDP, if
 **                                                 SDP was send in request.
 **
 **	7-Sep-2006		Deepti Goyal	SPR 12713		Generic Call Clear if ans
 **													SDP is not correct. 
 **
 ** 23-Nov-2006     Umang Singh     SPR 13894/CSR-1-4022527
 ** 28-Nov-2006     Deepti Goyal    ICF Rel 6.2     NAT "rport" feature support
 ** 19-Dec-2006     Puneet Sharma   503 Handling for subscribe_resp
 **
 ** 08-JAN-2007    Mohit Sharma     ICF Rel 6.3     Forking and Multianswer support
 **
 ** 08-JAN-2007    Mohit Sharma     ICF Rel 6.3     Forking and Multianswer support
 **
 ** 03-Feb-2007    Mohit Sharma     SPR 14462       PRACK handling in forking Response
 ** 26-Mar_2007    Rajat Chandna                    Changes for Update/reinvite 
 **                                                 enhancement.
 ** 17-Apr-2007    Abhishek Dutta   ICF 6.3         Added the changes for
 **                                                 TWC-UPDATE Handling
 ** 10-July-2007   Abhishek Dutta                   Added the changes for
 **                                                 Call modify cancel Handling
 ** 09-Jul-2007    Neha Choraria    ICF 7.0         Changes for Forking &
 **                                                 Multiple Answer
 ** 16-Jan-2008    Alok Tiwari      SPR-17945       Chamges for PUBLISH/MESSAGE
 **                                                 support
 ** 25-Jul-2008    Puneet Sharma    CSR 1-6168806
 **
 ** 28-Jul-2008    Puneet Sharma    CSR 1-6168806   ReREG challenged again after
                                                    success response once
 **	04-Jun-2009	   Aman Aggarwal	SPR 19708		Freed the call-object if BYE 
 **													receives failure response
 ** 04-Jun-2009    Alok Tiwari      CSR 1-7334810   Handling done to clear the
 **                                                 call after recieving 408 resp
 **                                                 for session refresh Re-INVITE
 **                                                 or UPDATE.  
 **	12-Jun-2009	   Aman Aggarwal	SPR 19736		Added functionality for refresh
 **													ReInvite and UPDATE challenge 
 ** 16-Jun-2009  Rohit Aggarwal		SPR 19747	Do not save App-requested hdrs
 **												from sesssion refresh response
 ** 30-Jun-2009    Vinay Jindal		SPR 19759	    If 422 received for initial INVITE
 **                                                 do not use received To Tag.
 ** 2-July-2009    Rajiv Kumar      SPR 19788       Fix for CSR-1-7444867.
 ** 15-Jul-2009    Anurag Khare     SPR 19857       Fix for CSR 1-7463287
 ** 23-Jul-2009	   Aman Aggarwal	SPR 19881 		Fix for CSR 1-7507275
 ** 16-Aug-2009    Anurag Khare     SPR 19989       Chnages done for NOTIFY
 **                                                 challange enhancement
 ** 04-Dec-2009    Pavneet Bajaj     SPR 20231       Fix for CSR 8061646
 ** 16-Dec-2009		Aman Aggarwal	CSR 1-8081459 
 ** 22-Feb-2010     Pavneet Bajaj     SPR 20307     Fix for CSR 8331321
 ** 20-Apr-2010  Rohit Aggarwal	CSR 1-8516341	Added null check for port
 **												in Contact header of 200 OK
 **												for REGISTER request
 **
 *****************************************************************************
 **                Copyrights 2010, Aricent.
 *****************************************************************************/


#include "stack_headers.h"
#include "uatk_headers.h"
#include "sdf_common.h"
/*#include "icf_api_id.h"*/
#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_common_types.h"
#include "icf_common_defs.h"
#include "icf_common_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_macro.h"
#include "icf_port_defs.h"

#include "icf_ssa_macro.h"
#include "icf_ssa_defs.h"
#include "icf_ssa_types.h"
#include "icf_ssa_prototypes.h"
#include "icf_feature_flags.h"
#include "sdf_sessTimer.h"
#include "sdf_sdp.h"
#include "sdf_lookupport.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif
/* To remove warnings */

Sdf_ty_retVal icf_ssa_chal_resp_transaction_Completed_for_register(
                        Sdf_st_callObject               **ppCallObj,
                        Sdf_st_eventContext             *pEventContext,
                        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
                        Sdf_ty_u16bit                           resp_code,
                        Sdf_st_error                    *pErr);


Sdf_ty_retVal icf_ssa_pos_resp_transaction_Completed_for_register(

        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_st_error                    *pErr);


Sdf_ty_retVal icf_ssa_transaction_Completed_for_register(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr);


Sdf_ty_retVal icf_ssa_transaction_Completed_for_update(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr);


Sdf_ty_retVal icf_ssa_transaction_Completed_for_subscribe(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr);


Sdf_ty_retVal icf_ssa_transaction_Completed_for_publish (
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr);


/*********************************************************
** FUNCTION: sdf_cbk_uaCancelCompleted
**
** DESCRIPTION: This callback is issued by the toolkit when
**              a final response to a CANCEL is received or a
**              final response to a BYE is received when the INVITE
**              has not yet been responded to.
**          1. If cancel is sent for INVITE then send call abort resp to
**             CM
**          2. If cancel is sent for Reinvite then reset the bitmask and 
**             no message is sent to CM
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaCancelCompleted(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{

    Sdf_ty_retVal            ret_val = Sdf_co_success;
    Sdf_ty_u16bit            dRespCode = 0;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st           *p_ssa_pdb = ICF_NULL;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pErr;

    if (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx)
    {
       /*If call context is NULL then call is already cleared
        *so no need to send message to CC*/
    }

    /* Fetch the response code from the SIP message */
    else if (Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                (*ppCallObj)->pUacTransaction->pSipMsg, &dRespCode,
                pErr))
    {
        if(481 != dRespCode)
        {

            if (ICF_NULL == (ICF_SSA_REINVITE_CANCELLED & p_ssa_ctxt->bitmask))
            /*Obsolete now - If a re-invite was cancelled, 
             *the SLP is not waiting for the
             * response. So we will not send any API to CM*/
            {

                if(ICF_CALL_MOD_CANCEL_REINV_REQ & 
                   p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
                {
                   /*Case when 200 OK of Re-Invite is 
                    received before 200 OK of Cancel
                    hence re-invite is sent again and ignore this 
                    200 OK of CANCEL*/
                   /*Do nothing*/
           
                   return ret_val;

                }
                /* Populate the internal message header */
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                      ICF_NW_INC_CALL_ABORT_RESP;
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;
                p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;     

               /*
                * All messages to be sent to CC/SC are routed to the CM.
                * CM decides the specific destination of these messages
                */
               p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
#ifdef ICF_STAT_ENABLE
	          icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"CANCEL",200,0);
#endif
            }
            else
           {
/*call_mod_cancel*/
              /* Populate the internal message header */
              p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                 ICF_NW_INC_CANCEL_COMPLETE_RESP;
              p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;

              /*
               * All messages to be sent to CC/SC are routed to the CM.
               * CM decides the specific destination of these messages
               */
              p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
#ifdef ICF_STAT_ENABLE
	          icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"CANCEL",200,0);
#endif
              p_ssa_ctxt->bitmask &= ~ICF_SSA_REINVITE_CANCELLED;
           }
        }
        else /*481 == dRespCode*/
        {
            /*The case when CANCEL is sent over the network for cancel
              call modify request and 481 response is rcvd*/
            /*Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                  ICF_GENERIC_CALL_CLEAR;
            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;

            /*
             * All messages to be sent to CC/SC are routed to the CM.
             * CM decides the specific destination of these messages
             */
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*********************************************************
** FUNCTION: sdf_cbk_uaCallFailed
**
** DESCRIPTION: Callback issued onto the application on the receipt of an
**              final failure message to an INVITE indicating failure of
**              a call request
**           1. Fetch the response code received ion SIP message
**           2. Map this to internal response codes
**           3. Send message to CM
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaCallFailed(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal           ret_val = Sdf_co_success;
    Sdf_ty_u16bit           dRespCode = 0;
    Sdf_ty_u32bit           size = 0;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_reject_st *p_internal_msg_param = ICF_NULL;

   icf_uint8_t             *p_reason_str = ICF_NULL;
#ifdef ICF_SESSION_TIMER
	SipHeader dTempHeader;
	SipOptions options;
    icf_nw_otg_session_refresh_req_st  session_refresh_req;
	/*Fixed Merged For CSR_1_5449895*/
	Sdf_st_error          sdf_error = {0,0,0,"\0"};
    Sdf_ty_u32bit         dCount = 0;
#endif
    Sdf_ty_slist         slMatchedHeadersList;
    SipHeader           *pRetryAfterHeader = Sdf_co_null;
    Sdf_st_headerList   *pRetryAfterHeaderList = Sdf_co_null;
    icf_boolean_t			reset_bitmask = ICF_TRUE;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
   
    if (ICF_NULL == p_ssa_ctxt)
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Ctxt is NULL.\n"))
        return ICF_FAILURE;
    }
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    if ((ICF_NULL == p_ssa_pdb )||
        (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Pdb Fetched from SSA Ctxt is NULL.\n"))
        return ICF_FAILURE;
    }

    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

    /* Fix for SPR 15557: If call xontext is NULL that means call 
       has been forcefully cleared till now. So no need to do processing 
       for the call*/
	/*Fix for CSR 1-8081459. Send ACK on receving the failure response.
	* If the response is coming here, means that Sip Timer 32 sec has not 
	* expired, hence, EGT shall send ACK.*/
    if((ICF_NULL == p_ssa_ctxt->p_call_ctxt) ||
		(ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
    {
		 if (ICF_TRUE == p_ssa_ctxt->no_retrans_req)
		 {
			 p_ssa_ctxt->no_retrans_req = ICF_FALSE;
		 }

		 options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
		 if (Sdf_co_fail == sdf_ivk_uaFormRequest(
			    (Sdf_ty_s8bit *)"ACK",(*ppCallObj), Sdf_co_null,
			    pErr))
	    {
		    ret_val = Sdf_co_fail;
	    }
	    else if(ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),
				    (*ppCallObj)->pUacTransaction->pSipMsg,
				    &options,pErr))
    {
		    /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
		    ret_val = Sdf_co_fail;
	    }

		p_ssa_ctxt->no_retrans_req = ICF_TRUE;

        return ICF_SUCCESS;
    }

    /*FORKING:- If This is not the call object on which last forked
     * response received then ignore the message*/
    /*if (p_ssa_ctxt != p_ssa_ctxt->p_call_ctxt->p_sip_ctx)
    {
        Discuss: if we have to send ACK, also check memory
         leak in this case 
        return ICF_SUCCESS;
    }*/

    /* update bitmask to indicate that now received response is 
       a final response */
    p_ssa_ctxt->bitmask |= ICF_FINAL_RESP_RECEIVED;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pErr;
   /* Protective check for rare scenario when null call_ctx inside ssa_ctx */
    if ((ICF_NULL != p_ssa_ctxt) && (ICF_NULL == p_ssa_ctxt->p_call_ctxt))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Msg recd but call_ctx could not be fetched\n"));
        return Sdf_co_fail;
    }
	/* Cast the payload into the icf_nw_inc_reject_st structure */
	p_internal_msg_param = (icf_nw_inc_reject_st *)
		p_ssa_pdb->p_internal_msg->p_msg_data;
	p_internal_msg_param->reason_code = ICF_NULL;
	
    /* Fetch the response code from the SIP message */
    if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                (*ppCallObj)->pUacTransaction->pSipMsg, &dRespCode,
                pErr))
    {
        ret_val = Sdf_co_fail;
    }
#ifdef ICF_SESSION_TIMER
    else if(dRespCode == 422)
	{
        if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
        {


		/* Fix for SPR 14410 */
		if (sip_getHeader((*ppCallObj)->pUacTransaction->pSipMsg,SipHdrTypeMinSE, &dTempHeader, \
			(SipError*)&(pErr->stkErrCode)) != SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
			/* Fix for SPR 14409 */
			if ( (ICF_NULL !=(*ppCallObj)->pSessionTimer) &&
				(90 > (*ppCallObj)->pSessionTimer->dMinSe))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Min-SE in 422 is less than 90, Clear the Call\n"));
				ret_val = Sdf_co_fail;
			} 
			else
			{
				/* Fix for SPR 31210 glb configuration data updation code deleted */	
				options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
				if (Sdf_co_fail == sdf_ivk_uaFormRequest(
						(Sdf_ty_s8bit *)"ACK",(*ppCallObj), Sdf_co_null,
						pErr))
				{
				   ret_val = Sdf_co_fail;
				}
				else if(ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),
					(*ppCallObj)->pUacTransaction->pSipMsg,
					&options,pErr))
				{
					/* Raise/Set Error: UATK_PROCESSING_FAILURE*/
					ret_val = Sdf_co_fail;
				}

				/* Send ICF_NW_OTG_SESSION_REFRESH_REQ to SSA to send re-invite
				 * with session interval equals to the value of Min-SE header */
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_SESSION_REFRESH_REQ;
				p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
				ICF_PORT_SIZEOF(icf_nw_otg_session_refresh_req_st);
				session_refresh_req.refresher_req_type = ICF_SSA_REFRESHER_METHOD_INVITE;
				p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&session_refresh_req;

				/*Fix Merged for CSR_1_5449895*/
				/* IPTK shall send INVITE without To tag after receiveing 422 for the 
				 * initial INVITE.So, delete the To tag from the Call object.
				 */
				if(SipFail == sip_getTagCountFromToHdr((*ppCallObj)->pCommonInfo->pTo,\
							&dCount,(SipError *)&(sdf_error.stkErrCode)))
				{
						ret_val = ICF_FAILURE;
				}
				if (0 != dCount)
				{
                    Sdf_ty_u32bit dIndex=0;
                    for(dIndex=0;dIndex<dCount;dIndex++)
                    {
                        if(SipFail == sip_deleteTagAtIndexInToHdr((*ppCallObj)->pCommonInfo->pTo,
                                   dIndex,(SipError *)&(sdf_error.stkErrCode)))
                        {
                            ret_val = Sdf_co_fail;
                            break;
                        }
                    }
                }

				if(icf_ssa_process_mesg(p_ssa_pdb->p_internal_msg) == ICF_FAILURE)
				{
					ret_val = Sdf_co_fail;
				}
                else
                {
                    /*
                     * Since this is a fresh invite with Authorization parameters and there 
                     * is no final response received for this INVITE so rset the
                     * ICF_FINAL_RESP_RECEIVED bitmask in ssa_context. This bitmask would
                     * have got set when 422 response was received for the request sent*/
                    p_ssa_ctxt->bitmask &= (~ICF_FINAL_RESP_RECEIVED);
                }
			}
		}
		else
		{
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]: 422 received wo MIn-SE so Call will be cleared\n"));
			ret_val = Sdf_co_fail;
		}
	}
    }
#endif
	/* Fill the appropiate reason cause
	*/
    else if(401 == dRespCode || 407 == dRespCode)
    {
	    /* In case Invite was already challenged earlier and Invite with 
	     * Auth params is being challenged again, terminate the call because 
	     * we do not want to go into a loop of Invite - 401 - Ack
	     */
	    if (ICF_CALL_AUTHENTICATION_REQ & 
			    p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)
	    {
		    reset_bitmask = ICF_TRUE;
	    }
	    else
	    {
                    /*
                     * Since this is a fresh invite with Authorization parameters and there
                     * is no final response received for this INVITE so rset the
                     * ICF_FINAL_RESP_RECEIVED bitmask in ssa_context. This bitmask would
                     * have got set when 407 response was received for the request sent
                     * without authorization parameters
                     */
                    p_ssa_ctxt->bitmask &= (~ICF_FINAL_RESP_RECEIVED);

		    reset_bitmask = ICF_FALSE;
		    /* No Need to check return value as, we need to send ack
		     * If we set failure in ret_val, nothing happens*/
		    icf_ssa_fetch_auth_param_list(p_ssa_pdb,
				    ppCallObj,ICF_NULL);

                    /* Fetch and store the old nonce in the ssa context */
                    if(ICF_FAILURE == icf_ssa_extract_nonce_from_challenge(
                                          p_ssa_pdb,
                                          dRespCode))
                    {
                        ret_val = ICF_FAILURE;
                    }
 

		    if(ICF_NULL != p_ssa_ctxt->auth_params_list_size)
		    {
			    /* fetch line data and check if line has auth param configured,
			     * if yes set call auth as true */
			    /* Fetch Line data from DBM */
			    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
						    p_ssa_pdb->p_glb_pdb,
						    p_ssa_pdb->p_glb_pdb->p_call_ctx->line_id,
						    &(p_ssa_pdb->p_line_data),
						    p_ssa_pdb->p_ecode))
			    {
				    ret_val = ICF_FAILURE;
			    }
                else if(ICF_SUCCESS == 
                        icf_cmn_check_if_auth_params_present(
								p_ssa_pdb->p_glb_pdb, 
								&p_ssa_pdb->p_glb_pdb->p_call_ctx->local_address, 
								p_ssa_pdb->p_line_data->line_id))
			    {
				    /* Auth params are present in response from Peer,
				     * set bitmask in call context for the same */
				    p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask|=
					    ICF_CALL_AUTHENTICATION_REQ;

				    if(ICF_TRUE == ICF_IS_BIT_SET(
							    p_ssa_pdb->p_line_data->line_data_received, ICF_CFG_LINE_DATA_REALM_RECD))
				    {
					    /* We have to compare the received realm with configured one */
					    Sdf_st_authenticationParams *pAuthenticationParams = Sdf_co_null;

					    if(Sdf_co_success == sdf_listGetAt(p_ssa_ctxt->p_auth_params_list, 0, 
								    (Sdf_ty_pvoid *)&pAuthenticationParams, 
								    pErr))
					    {
						    if(Sdf_co_null != pAuthenticationParams->pRealm)
						    {
							    icf_string_st temp_unq_realm = {0, "0"};
							    temp_unq_realm.str[0]='\0';
							    temp_unq_realm.str_len=0;

							    sdf_fn_uaUnq(pAuthenticationParams->pRealm,(icf_int8_t *)temp_unq_realm.str);

							    if(0 != icf_port_strcmp(p_ssa_pdb->p_line_data->configured_realm.str, 
										    temp_unq_realm.str))
							    {
								    p_internal_msg_param->reason_code =
									    ICF_CAUSE_REALM_MISMATCH;
								    /*Set the release cause in the call context*/
								    p_ssa_pdb->p_glb_pdb->p_call_ctx->release_cause = 
									    ICF_CAUSE_REALM_MISMATCH;

								    ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
										    ICF_CALL_AUTHENTICATION_REQ);
							    }
						    }
					    }
				    }
			    }
			    else
			    {
				    p_internal_msg_param->reason_code =
					    ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_CFGRD;

				    /*Set the release cause in the call context*/
				    p_ssa_pdb->p_glb_pdb->p_call_ctx->release_cause = 
					    ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_CFGRD;
			    }
		    } /* if(ICF_NULL != p_ssa_ctxt->auth_params_list_size) */
	    } /* if-else (ICF_CALL_AUTHENTICATION_REQ & ..) */
    } /* if(401 == dRespCode || 407 == dRespCode) */
    /* Fix for CSR 1-4826190 */
    else
    {
	    /* In case Invite was challenged earlier and now Invite with Auth 
		 * params has received final failure resp, terminate the call because 
	     * we do not want to send INVITE again after getting failure resp.
		 * For more clarity, read the comment after this else block.
	     */
	    if (ICF_CALL_AUTHENTICATION_REQ & 
			    p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)
	    {
                ICF_SSA_RESET_COMMON_BITMASK(
                        p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
                        ICF_CALL_AUTHENTICATION_REQ)

		    reset_bitmask = ICF_TRUE;
	    }
        else
        {
            reset_bitmask = ICF_FALSE;
        }
    }
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: call-id = %d, reset auth bitmask = %d\n", \
				p_ssa_pdb->p_glb_pdb->p_call_ctx->call_id, \
				reset_bitmask));

    /* Breaking the loop:
     * Consider the scenario for outgoing call
     * Invite - 401 - Ack - Invite - 486 - Ack - Invite - 486 - Ack.
     * The problem was that once Invite was challenged, subsequently 
     * IPTK continues to initiate Invite on receiving final failure resp 
     * because the function icf_cc_02_ssa_remote_reject() finds the bit 
	 * ICF_CALL_AUTHENTICATION_REQ set in call_ctx and triggers SSA to send 
	 * another Invite.
	 * We need to reset this bitmask to break this loop.
	 */

    if((dRespCode != 422) || (dRespCode == 422 && ret_val == Sdf_co_fail))
    {
                
		ret_val = Sdf_co_success;
        /* Populate the internal message header */
        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CALL_REJECT;

        /*
         * Fill the appropiate reason cause 
         */		
        if ((ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_CFGRD != p_internal_msg_param->reason_code)
			&& (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                    dRespCode,&(p_internal_msg_param->reason_code))))
        {
            ret_val = Sdf_co_fail;
        }
        else
        {
            /*
             * All messages to be sent to CC/SC are routed to the CM.
             * CM decides the specific destination of these messages
             */
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
    } 
        /* Fix for SPR 14940 */
    if(dRespCode == 404)
    {
      p_internal_msg_param->reason_code = ICF_CALL_CLEAR_REASON_USER_NOT_FOUND; 
    }
#ifdef ICF_STAT_ENABLE
	icf_ssa_invite_resp_stats(p_ssa_pdb,dRespCode,0);
#endif
    /* copy the remote call clear response code to call context */
    p_ssa_pdb->p_glb_pdb->p_call_ctx->inc_response_code = dRespCode; 

    icf_ssa_get_reason_string_from_sip_message(
      (*ppCallObj)->pUacTransaction->pSipMsg, 
      (SIP_S8bit**)&(p_reason_str));

    if(ICF_NULL != p_reason_str)
    {
        if(ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx->p_response_str)
        {
            ICF_MEMFREE(
                       p_ssa_pdb->p_glb_pdb,
                       p_ssa_pdb->p_glb_pdb->p_call_ctx->p_response_str,
                       ICF_MEM_COMMON,
                       p_ssa_pdb->p_ecode,
                       ret_val)
            p_ssa_pdb->p_glb_pdb->p_call_ctx->p_response_str = ICF_NULL;
        }   
            ICF_MEMGET(
                p_ssa_pdb->p_glb_pdb,
                (icf_port_strlen(p_reason_str) +1),
                ICF_MEM_COMMON,
                p_ssa_pdb->p_glb_pdb->p_call_ctx->p_response_str,
                ICF_RET_ON_FAILURE,
                p_ssa_pdb->p_ecode,
                ret_val)
 
        icf_port_strcpy(p_ssa_pdb->p_glb_pdb->p_call_ctx->p_response_str,
                              p_reason_str);
    }

#ifdef ICF_ERROR_ENABLE
    if(((480 != dRespCode) && (486 != dRespCode) && 
         (401 != dRespCode) && (407 != dRespCode)) && (ICF_FALSE == reset_bitmask))
    {
          p_ssa_pdb->resp_code = dRespCode;
          ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_FAILURE_RESPONSE)
    }    
    else if(ICF_TRUE == reset_bitmask)
    {
          ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_AUTHENTICATION_FAILURE)
    } 
#endif

     /* reset the birmask in call context to prevent the looping of INVITE and 407 */
  if(ICF_TRUE == reset_bitmask)
	{
		ICF_SSA_RESET_COMMON_BITMASK(
			p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
			ICF_CALL_AUTHENTICATION_REQ)
	}
      
  /* This switch case is used to retrive the retry header field value if 
     present,from some specific final failure responses of initial INVITE */
 
   switch(dRespCode)
       {

        case 404:
        case 413:
        case 480:
        case 486:
        case 500:
        case 503:
        case 600:
        case 603:
                if (Sdf_co_fail == sdf_ivk_uaGetHeaderListByType(
                        (*ppCallObj)->pUacTransaction->slUnprocessedHeaders,
                        SipHdrTypeRetryAfterSec,
                        &slMatchedHeadersList,
                        pErr))
                 {
                    ret_val = Sdf_co_fail;
                 }
                else if (Sdf_co_fail == sdf_listSizeOf(
                        &slMatchedHeadersList,&size, pErr))
                 {
                    sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                    ret_val = Sdf_co_fail;
                 }
                else if (ICF_NULL != size)
                 {

                    /* Get HeaderList at index 0 */

                    if (Sdf_co_fail == sdf_listGetAt(
                               &slMatchedHeadersList,
                               0,
                               (Sdf_ty_pvoid *)&pRetryAfterHeaderList,
                               pErr))
                     {
                       ret_val = Sdf_co_fail;
                     }
                    else if (Sdf_co_fail == sdf_listGetAt(
                             &(pRetryAfterHeaderList)->slHeaders,
                             0,
                            (Sdf_ty_pvoid *)&pRetryAfterHeader,
                             pErr))
                     {
                       ret_val = Sdf_co_fail;
                     }
                    else
                     {
                       p_ssa_ctxt->p_call_ctxt->retry_after_duration =
                            (((SipRetryAfterHeader *)
                            (pRetryAfterHeader->pHeader))->u).dSec;
                       sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                     }
                   }

                break;
      }


    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}

/*********************************************************
** FUNCTION: sdf_cbk_uaCallAccepted
**
** DESCRIPTION: Callback issued onto the application on the receipt of an
**              200 OK message to the INVITE message
**          1. If INVITE response is received with SDP then create SDP block
**             copy media parameters into it and send connect api to CM
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaCallAccepted(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_ty_retVal                       ret_val1 = Sdf_co_success;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_connect_st               *p_internal_msg_param = ICF_NULL;
    SipFromHeader                       *p_from = Sdf_co_null;
    SipHeader                           *p_assert_hdr = SIP_NULL;    
    Sdf_ty_retVal                       ret_value = Sdf_co_success;
    icf_return_t                        retval = ICF_SUCCESS;
    Sdf_ty_u16bit                       dRespCode = 0;
#ifdef ICF_SESSION_TIMER
	Sdf_ty_refresher                    dRefresher;
    SipMessage                          *p_message = ICF_NULL;
#endif
	
	/*This structure is populated if there is sdp in INVITE.
	 * Then we populate the call context corresponding.
	 */
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    
#if 0
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
#endif

    /*FORKING:- If This is not the call object on which last forked
     * response received then ignore the message*/
    /*if (p_ssa_ctxt != p_ssa_ctxt->p_call_ctxt->p_sip_ctx)
    {
        Discuss: if we have to send ACK, also check memory
         leak in this case
        return ICF_SUCCESS;
    }*/
    if (ICF_NULL == p_ssa_ctxt)
    {
       return ICF_FAILURE;
    }  

    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    if (ICF_NULL == p_ssa_pdb)
    {
       return ICF_FAILURE; 
    }    
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
        
    /* update bitmask to indicate that now received response is 
       a final response */
    p_ssa_ctxt->bitmask |= ICF_FINAL_RESP_RECEIVED;

    /* Delete all other call objects other then for which 200 Ok is
     * received */
    /*if (ICF_SUCCESS !=icf_ssa_del_all_other_call_obj(p_ssa_pdb))
    {
        return Sdf_co_fail; 
    }*/

    /* Cast the payload into the icf_nw_inc_connect_st structure */
    p_internal_msg_param = 
        (icf_nw_inc_connect_st *)p_ssa_pdb->p_internal_msg->p_msg_data;
    /*Init the bitmask*/
    p_internal_msg_param->bitmask = ICF_NULL;
    p_internal_msg_param->p_sdp_list = ICF_NULL;

    /********* Changes for Forking & Multiple Answer Start (Rel7.0) *********/
    p_internal_msg_param->forked_resp_id = ICF_NULL;
    /********** Changes for Forking & Multiple Answer End (Rel7.0) **********/

      if(ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx)
      {
          return Sdf_co_success;
      }
          /* Start Fix for SPR 14933 */
    /* Fetch the response code from the SIP message */
      /* LOAD FIX */
      if(Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                        (*ppCallObj)->pUacTransaction->pSipMsg,
                                        &dRespCode,pErr))
      {
              p_ssa_pdb->p_glb_pdb->p_call_ctx->inc_response_code = dRespCode;
      }
     /*   LOAD FIX END */
     else
      { 
        ret_value = Sdf_co_fail;
      }/* End :Fix for SPR 14933 */

      ret_val1 = icf_ssa_check_isfocus_in_contact_header(p_ssa_pdb);
      if(ret_val1==ICF_SUCCESS)
       {
         p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 |= 
                  ICF_NW_INC_CONNECT_ISFOCUS_FOUND;
        /* Condition to check that conference uri is already populated 
           in call_ctx or not if not populated then populate if ISFOCUS 
           is found */
         if (p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str_len 
             == 0 )
          {
             p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri = 
                    p_ssa_pdb->conference_uri;
             ICF_PRINT(((icf_uint8_t *)"Is focus in 200 OK with conference URI %s",
                p_ssa_pdb->p_glb_pdb->p_call_ctx->conference_uri.addr_val.str))
          }
        }		

#ifdef ICF_IPSEC_ENABLED

	if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled)
	{
		/* decrement the active transaction count */
		p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans -= 1;
		ICF_PRINT(("[SSA]: num_active_trans decremented, current value = %d", 
			p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans));
		
		/* reset the threshold counter value */
		p_ssa_pdb->p_ssa_glb_data->num_successive_trans_fail = ICF_NULL;
		
		/* check if the active transaction count == 0. if yes and the
		* response is for a non Register transaction, so send a message
		* to REGM for deleting the SA if the transaction comp. on the 
		* old SA
		*/
		if (ICF_NULL == 
			p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans)
		{
			ret_val = icf_ssa_rgm_delete_old_sa_ind(p_ssa_pdb);
		}
	}
#endif
    
#ifdef ICF_SESSION_TIMER
      if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
      { 
        if(sdf_ivk_uaGetRefresher(*ppCallObj,&dRefresher,pErr) != Sdf_co_fail){
	
		if ( Sdf_en_invalidRefesher == dRefresher)
		{
				p_ssa_ctxt->p_call_obj->pSessionTimer->dSessionInterval = 0;
				p_ssa_ctxt->p_call_obj->pSessionTimer->dMinSe = 0;
				ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                            ICF_NW_INC_CONNECT_GENERIC_ERROR)
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CONNECT ;
                 p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
				ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
				return Sdf_co_success;
		}
             if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                            p_ssa_pdb->p_glb_pdb,
                            p_ssa_ctxt->line_id,
                            &(p_ssa_pdb->p_line_data),
                            p_ssa_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }

                /* A UAC should not start session time in the following
                 * cases
                 * Case 1: Application has configured refresher as none
                 *         and remote has also no sent any refresher
                 *         value
                 * Case 2: Refresher is received as UAS from the peer so
                 *         the peer would be running timer. 
                 */

             if(dRefresher == Sdf_en_refresherLocal ||  
			 (ICF_SESSION_REFRESHER_UAC == 
			p_ssa_pdb->p_line_data->session_timer_refresher
			&& dRefresher == Sdf_en_refresherRemote))
       		{
                         if ((ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx->p_session_timer_data) && \
                              (ICF_FALSE == p_ssa_pdb->p_glb_pdb->p_call_ctx->p_session_timer_data->session_refresher_reqd) && \
                              ((*ppCallObj)->pSessionTimer->dRefresher == Sdf_en_refresherNone))
                          {
                             if(Sdf_co_fail == sdf_ivk_uaSetRefresher(*ppCallObj,
                                                    Sdf_en_refresherLocal,
                                                    pErr))
                             {
                                ret_val = Sdf_co_fail;
                             } 
                        }   
			p_message = (*ppCallObj)->pUacTransaction->pSipMsg;
			/* SPR 9591- pass the SPI message instead of the trans. 
			 * pointer */
			icf_ssa_set_refresher_method(p_message,
				p_ssa_pdb->p_glb_pdb,
				&p_ssa_ctxt,
				pErr);
	
			if(p_ssa_ctxt->p_call_obj->pSessionTimer->dRefreshRequest == Sdf_en_refresherInvite){
				ICF_PRINT(((icf_uint8_t *)"\n [SSA] Subsequent session refresh request Method will be INVITE"));
			}
			else{
				ICF_PRINT(((icf_uint8_t *)"\n [SSA] Subsequent session refresh request Method will be UPDATE"));
			}
		}
		/* start the session timer if it is not started by UATK (if 
		 * 2xx response does not contain Session Expires Header then
		 * UATK will not start the session timer) */
          if(dRefresher != Sdf_en_refresherNone || 
				ICF_SESSION_REFRESHER_NONE != 
				p_ssa_pdb->p_line_data->session_timer_refresher)
		{
	     	if(icf_ssa_start_session_timer(ppCallObj,
			p_ssa_pdb->p_glb_pdb,
#ifdef ICF_WITH_MUATK_2_2
			pEventContext,
#endif
			pErr) == Sdf_co_fail){
			ret_val = Sdf_co_fail;
		}
       }
	}
	else{
		ret_val = Sdf_co_fail;
	}
    }
#endif

    /* Check for the presence of SDP in the received message */
    if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
        ((*ppCallObj)->pUacTransaction->pSipMsg,pErr))
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
                            (*ppCallObj)->pUacTransaction->pSipMsg,
							0,&p_inc_media_sdp_info,
                            p_ssa_pdb->p_ecode))
        {
            if(ICF_NULL != p_inc_media_sdp_info)
            {
                /* Free the acquired memory for the SDP */
                /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
            }
	    ret_val = ICF_FAILURE;	
        }
        else
        {		
            p_internal_msg_param->p_sdp_list =\
                p_inc_media_sdp_info->p_sdp_info_list;
           
            /******* Changes for Forking & Multiple Answer Start (Rel7.0) ******/
            
            p_internal_msg_param->forked_resp_id = \
            ((icf_ssa_ctxt_st *)((*ppCallObj)->pAppData)->pData)->forked_resp_counter;

            /******** Changes for Forking & Multiple Answer End (Rel7.0) *******/
 
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP received in 200 OK"));

            /*
            * Set the bit which indicates that CONNECT
            * has been received with SDP
            */

            if(ICF_NULL != p_inc_media_sdp_info->p_sdp_info_list)
            {
                ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                    ICF_NW_INC_CONNECT_WITH_SDP);
            }
			    /* 
                * Delete the message body list stored in the UAC
                * transaction of the call object.
                * This is done to flush the SDP body from the 
                * transaction substructure of the call object
            */
            sdf_listDeleteAll(
                &((*ppCallObj)->pUacTransaction->slMsgBodyList),
                pErr);
        }
    }     
    
    else
    {
                /******* Changes for Forking & Multiple Answer Start (Rel7.0) ******/

                p_internal_msg_param->forked_resp_id = \
                ((icf_ssa_ctxt_st *)((*ppCallObj)->pAppData)->pData)->forked_resp_counter;

                /******** Changes for Forking & Multiple Answer End (Rel7.0) *******/
        
		ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP not received in 200 OK"));
		/* Changes done for Message Body Headers Start :
		 */
		if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
			    (*ppCallObj)->pUacTransaction->pSipMsg,\
			     /**ppCallObj*/SipMessageResponse,&(p_ssa_pdb->p_glb_pdb->p_call_ctx->body_list)))
		{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
				ret_val = ICF_FAILURE;
		}
		/* Changes done for Message Body Headers End :
		 */

    }


  icf_ssa_is_update_present_in_allow_hdr((*ppCallObj)->pUacTransaction->pSipMsg,
                                p_ssa_pdb->p_glb_pdb,
                                &p_ssa_ctxt,
                                pErr);

    /* get the application specific headers from dbm.      
    * These headers shall be filled in call context based 
    * on the app_id in call ctxt. Currently use app id = 1
    */
    if (Sdf_co_fail != ret_val)
    {
    if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
        p_ssa_pdb,ICF_INT_MODULE_CC,
		(icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
        (*ppCallObj)->pUacTransaction->pSipMsg,
        p_ssa_pdb->p_ecode) )
    {
        /*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/
            
        	/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
			icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
								&p_inc_media_sdp_info);   
        
            ret_val = ICF_FAILURE;
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
			p_ssa_pdb, (*ppCallObj)->pUacTransaction->pSipMsg))
		{
			/*ICF_SSA_TRACE(("\nFailed to get P-Media-Authorization header"))*/

            /* Free the memory allocated to icf_ssa_inc_media_stream_info_st */
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                &p_inc_media_sdp_info);

            ret_val = Sdf_co_fail;
		}
	}
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_IMS_END
#endif
#endif
#endif

    if ((Sdf_co_fail != ret_val)&&
		(ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id))
    {

        /* Rel 6.0 Changes: Delete the 100 rel from the supported header of 
         * profile. Get the supported header from the Call Object.
         */        
        retval = icf_ssa_modify_callobj_profile_after_call_accept(p_ssa_pdb\
            ,(*ppCallObj)->pInitData,pErr);

        if(ICF_SUCCESS == retval)
        {
            /* populate p_asserted_identity in call context */
            if (ICF_FAILURE == icf_ssa_parse_unknown_header_like_from_hdr(
                p_ssa_pdb,(*ppCallObj)->pUacTransaction->pSipMsg,"P-Asserted-Identity",
                &p_assert_hdr))                    
            {
                /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);
                
                ret_val = Sdf_co_fail;                
            }
            if (SIP_NULL != p_assert_hdr)
            {
                p_from = (SipFromHeader *)
                    (p_assert_hdr->pHeader);
                    /*
                    * Set the bit remote_aserted_address
                    * indicating that this is an asserted 
                    * address
                */
                p_ssa_pdb->p_glb_pdb->p_call_ctx->remote_aserted_address = ICF_TRUE;
                
                /* allocate memory for asserted identity, will be freed by CC */
                ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
                    ICF_PORT_SIZEOF(icf_address_st),
                    ICF_MEM_COMMON, 
                    p_ssa_pdb->p_glb_pdb->p_call_ctx->p_asserted_identity,
                    ICF_DONOT_RET_ON_FAILURE, p_ssa_pdb->p_ecode, ret_val)
                    if (Sdf_co_fail == ret_val)
                    {
                      		/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                            &p_inc_media_sdp_info);
                        
                        ret_val = Sdf_co_fail;
                    }
                    else
                    {
                        icf_ssa_populate_address_from_sip_addr_spec(
                            p_ssa_pdb,p_from->pAddrSpec,
                            p_ssa_pdb->p_glb_pdb->p_call_ctx->p_asserted_identity);
                    }
                    /* free the memory associated with assert hdr */
                    sip_freeSipHeader(p_assert_hdr);
                    ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_assert_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_value)
            }                
            
            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CONNECT ;
            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                ICF_PORT_SIZEOF(icf_nw_inc_connect_st);
            /*
             * All messages to be sent to CC/SC are routed to the CM.
             * CM decides the specific destination of these messages
             */
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
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
    else if(ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id)
    {
	    /*Set the bit that some error has occured while 
	     *processing 200ok 
	     */

	    ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
			    ICF_NW_INC_CONNECT_GENERIC_ERROR)

		    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CONNECT ;
	    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
		    ICF_PORT_SIZEOF(icf_nw_inc_connect_st);

	    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
	    ret_val = ICF_SUCCESS;
    }
#ifdef ICF_STAT_ENABLE
    icf_ssa_invite_resp_stats(p_ssa_pdb,200,0);    
#endif
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*********************************************************
 ** FUNCTION:sdf_cbk_uaCallTerminated
 **
** DESCRIPTION: Callback issued onto the application on the receipt of a
**              200 Response message to the BYE sent
**
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaCallTerminated(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal               ret_val = Sdf_co_success;
    icf_ssa_pdb_st           *p_ssa_pdb = ICF_NULL;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    SipMessage               *p_temp_msg = Sdf_co_null;
    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pErr;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    /* Extract the pointer to SSA context */

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* Populate the internal message header */
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_NW_INC_CALL_TERMINATE_RESP ;
    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;

    if(!(p_ssa_ctxt->bitmask_1 & ICF_BYE_ALREADY_SEND))
    {
#ifdef ICF_STAT_ENABLE
        icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"BYE",200,0);
#endif
        /*
         * All messages to be sent to CC/SC are routed to the CM.
         * CM decides the specific destination of these messages
         */
        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

        /*This indicates to the Entry pt code ( processing otg internal msg)
         * that the current internal msg will be the last msg from this
         * dialog to App. After this the bitmask in SSA CTXT will be set
         * to indicate that no further messages are to be sent to App*/
        ICF_SSA_SET_COMMON_BITMASK(p_ssa_pdb->common_bitmask,\
                ICF_SSA_AWAITING_SSA_CTXT_DEL)
    }

    /* Free the SSA context -now it will be freed through the FreeAppData
     * called through FreeCallObject*/
    /*ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,p_ssa_ctxt,
                            ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)*/

    /*
     Fix for FMR in case of TCP call

     200 OK for a BYE is sent via the call back sdf_cbk_uaCallTerminated.
     Setting the TRACE required flag.This flag is being set here so that 
     when a call is being established on TCP and we get 200 ok of the BYE 
     sent,we call the app_trace function here only.
     */
     ICF_SSA_SET_COMMON_BITMASK(
                    p_ssa_pdb->common_bitmask,
                    ICF_SSA_TRACE_REQUIRED)

     /* extracting sip message to be passed to the trace function*/
     p_temp_msg = (*ppCallObj)->pUacTransaction->pSipMsg;
     
#ifdef ICF_TRACE_ENABLE
     /*calling the trace function*/
     icf_ssa_capture_trace(p_ssa_pdb,p_temp_msg,1);
     /* SPR 17555 Fix: Send call direction to app_trace for SIP PDUs
      */
     p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
#endif
     ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
    /* if BYE was rejected then we do not decrement the ref count*/
    if(ICF_FALSE == p_ssa_pdb->bye_rejected)
    {
        sdf_ivk_uaFreeCallObject(ppCallObj);
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*********************************************************
 ** FUNCTION:sdf_cbk_uaInProgress
 **
 ** DESCRIPTION: This callback is issued onto the application on the receipt
 **              of an 1xx message indicating that the call is in progress
  **        1. Check if reinvite has already been sent then ignore the msg
 **        2. Fetch response code from SIP message
 **        3. If it is 100 then send inc_call_setupo_ack to CM
 **        4. If it is 180 with SDP, then create SDP block populate it with 
 **           received SDP, and update in call context and send msg_id 
 **           inc_alert_with_media else send inc_alert to CM
 **        5. If it is 183 populate SDP in call context and send 
 **           inc_media_param to CM
 **********************************************************/
Sdf_ty_retVal sdf_cbk_uaInProgress(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_overlapTransInfo *pOverlapTransInfo,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_ty_u16bit                       dRespCode = 0;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    icf_uint8_t                         msg_identifier=0;
    icf_boolean_t						progress_msg_wo_sdp = ICF_FALSE;


    /* To remove compiler warning */
    (void)pOverlapTransInfo;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    /*FORKING:- If This is not the call object on which last forked
     * response received then ignore the message*/
    /*if (p_ssa_ctxt != p_ssa_ctxt->p_call_ctxt->p_sip_ctx)
    {
        Discuss: if we have to send ACK, also check memory
        leak in this case*
        return ICF_SUCCESS;
    }*/

    /* Initialising dest module to invalid */
    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_INVALID;
    ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->\
       p_internal_msg->p_msg_data))->forked_resp_id = ICF_NULL;
    ((icf_nw_inc_media_param_st*)(p_ssa_pdb->\
       p_internal_msg->p_msg_data))->forked_resp_id = ICF_NULL;
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

/* SPR 8066 FIX START */
    if (pOverlapTransInfo != NULL) 
    {
		/* This is the case for handling 1xx messages for overlap 
		transactions like REGISTER */ 
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return Sdf_co_success;
    }
/* SPR 8066 FIX END */
    if (Sdf_en_reInviteSent == (*ppCallObj)->pCallInfo->dState)
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return Sdf_co_success;
    }
    /* Fetch the response code from the SIP message */
    if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                (*ppCallObj)->pUacTransaction->pSipMsg, &dRespCode,
                pErr))
    {
        ret_val = Sdf_co_fail;
    }
    else
    {
	/* If message is received with Require header containing 100rel
         * and configuraton doesn't support the same ,clear the call
	 */
	if (ICF_SUCCESS == icf_ssa_isHeaderPresent(
				(*ppCallObj)->pUacTransaction->pSipMsg,
				SipHdrTypeRequire,
				pErr))
	{
		if (ICF_FAILURE == icf_ssa_parse_require_header(
					p_ssa_pdb,
					(*ppCallObj)->pUacTransaction->pSipMsg,
					pErr))
		{
			p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
				    ICF_GENERIC_CALL_CLEAR;
			p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
			p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 0;
			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
			ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
			return ret_val;
		}

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
		if((pErr->errCode == ICF_CAUSE_PRECONDITION_PROV_WITHOUT_100REL)
			&&(183 == dRespCode))
		{
			p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
					ICF_GENERIC_CALL_CLEAR;
			p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
			p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 0;
			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
			ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
			return ret_val;
		}
ICF_CHECK_QOS_SUPPORT_END
#endif

	}
	else if(183 == dRespCode)
	{
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
		p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
			    ICF_GENERIC_CALL_CLEAR;
		p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
		p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 0;
		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
		ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
		return ret_val;
ICF_CHECK_QOS_SUPPORT_END
#endif
	}
	/* 
	 * Switch on the response code and fill the appropiate 
	 * API in the internal message
	 */
	switch(dRespCode)
	{

		/* A 100 Trying response is received */
		case 100 :
			p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_NW_INC_CALL_SETUP_ACK;
                /*
                 * All messages to be sent to CC/SC are routed to the CM.
                 * CM decides the specific destination of these messages
                 */
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                break;

            /* A 180 Ringing response is received */
            case 180    :
            /* A 183 Session Progress response is received */
            case 183    :
                /* Check for the presence of SDP in the message */
                if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent((*ppCallObj)->
                            pUacTransaction->pSipMsg, pErr))
                {
               #if 0 
                    /*This function check P-Early-Media Header in the Sip Message
                     *If that Header is present,then it populates the mode value in 
                     *call context which is further populated in the remote mode while
                     *forming SDP.  
                     */
                     icf_ssa_parse_p_early_media_hdr(p_ssa_pdb,
                                 (*ppCallObj)->pUacTransaction->pSipMsg);
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
                    
                    if(ICF_CALL_CONNECT_DONE & p_ssa_pdb->p_glb_pdb->\
                        p_call_ctx->common_bitmask)
                    {
                        msg_identifier = ICF_SSA_RECVD_ANSWER_TO_OFFER;
                    }

                    if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                            p_ssa_pdb,
                                            (*ppCallObj)->pUacTransaction->pSipMsg,
											msg_identifier, &p_inc_media_sdp_info,
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
                    }
                              
                    else
                    {	
							if(dRespCode == 180)
                            {
                                ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->\
                                    p_internal_msg->p_msg_data))->p_sdp_list = \
                                    p_inc_media_sdp_info->p_sdp_info_list;
                               
                                /**** Changes for Forking & Multiple Answer Start 
                                      (Rel7.0) ****/

                                /* forked_resp_id in the SSA context assigned to 
                                   forked_resp_id in icf_nw_inc_alert_media_st 
                                   and sent to CC */
                                ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->\
                                 p_internal_msg->p_msg_data))->forked_resp_id = \
                                ((icf_ssa_ctxt_st *)((*ppCallObj)->pAppData)->pData)->forked_resp_counter;
                                /**** Changes for Forking & Multiple Answer End 
                                      (Rel7.0) ****/
 
                                /*
                                 * Fill the appropiate fields of the 
                                 * internal message
                                 */
                                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                    ICF_NW_INC_ALERT_WITH_MEDIA;
                                ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->
                                    p_internal_msg->p_msg_data))->bitmask = ICF_NULL;
                                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                                    ICF_PORT_SIZEOF(icf_nw_inc_alert_media_st);							
                            }

                            if(dRespCode == 183)
                            {
                                ((icf_nw_inc_media_param_st*)(p_ssa_pdb->\
                                    p_internal_msg->p_msg_data))->p_sdp_list =\
                                    p_inc_media_sdp_info->p_sdp_info_list;
                               
                                /**** Changes for Forking & Multiple Answer Start 
                                      (Rel7.0) ****/

                                /* forked_resp_id in the SSA context assigned to 
                                   forked_resp_id in icf_nw_inc_alert_media_st 
                                   and sent to CC */

                                 ((icf_nw_inc_media_param_st*)(p_ssa_pdb->\
                                  p_internal_msg->p_msg_data))->forked_resp_id = \
                                 ((icf_ssa_ctxt_st *)((*ppCallObj)->pAppData)->pData)->forked_resp_counter;

                                 /**** Changes for Forking & Multiple Answer End 
                                      (Rel7.0) ****/ 
                                /*
                                 * Fill the appropiate fields of the 
                                 * internal message
                                 */
                                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                    ICF_NW_INC_MEDIA_PARAM;
                                
                                ((icf_nw_inc_media_param_st*)(p_ssa_pdb->
                                    p_internal_msg->p_msg_data))->bitmask = ICF_NULL;
                                
                                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                                    ICF_PORT_SIZEOF(icf_nw_inc_media_param_st);						
                            }
                            					
							/* 
							* Delete the message body list stored in the UAC
							* transaction of the call object.
							* This is done to flush the SDP body from the 
							* transaction substructure of the call object
							*/
                            sdf_listDeleteAll(
                                &((*ppCallObj)->pUacTransaction->slMsgBodyList),
                                pErr);
                            
                        }
                }/* end of if sdp is present */
                
                else
                {
                   /* Message body header changes start for 180/183 response
                    */
                    /* Extract the message bodies from SIP message.
                    */
                    if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
			        (*ppCallObj)->pUacTransaction->pSipMsg,\
			         /**ppCallObj*/SipMessageResponse,&(p_ssa_pdb->p_glb_pdb->p_call_ctx->\
				    body_list)))
		            {
				        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
    				    ret_val = ICF_FAILURE;
	        	    }
                    /* Message Body Header Changes Ends
                    */
                    
                    if(180 == dRespCode)
                    {
                    /*
                     * If SDP is not present ,
                     * set msg_id = ICF_NW_INC_CALL_ALERT
                     */
                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                            ICF_NW_INC_CALL_ALERT;
                        ((icf_nw_inc_call_alert_st*)(p_ssa_pdb->
                            p_internal_msg->p_msg_data))->bitmask = ICF_NULL;
                        p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                            ICF_PORT_SIZEOF(icf_nw_inc_call_alert_st);
                    }
                   else if (183 == dRespCode)
					{
						progress_msg_wo_sdp = ICF_TRUE;
					}
                }
              
                /*twc_update handling*/
                icf_ssa_is_update_present_in_allow_hdr((*ppCallObj)->pUacTransaction->pSipMsg,
                                                        p_ssa_pdb->p_glb_pdb,
                                                        &p_ssa_ctxt,
                                                        pErr);

                /*
                 * All messages to be sent to CC/SC are routed to the
                 * CM.CM decides the specific destination of these
                 * messages
                 */
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                break;
#if 0
            case 181    :
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                    ICF_NW_INC_REDIRECT_IND;
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_nw_inc_redirect_ind_st);
                break;
#endif   

            case 181:
			case 182:
				progress_msg_wo_sdp = ICF_TRUE;
				break;
      
            default :
                /*ICF_SSA_TRACE(("Unhandled message \n"))*/
                ;

        } /* switch(dRespCode) */
    } /* else */
#ifdef ICF_STAT_ENABLE
        if( 405 == dRespCode)
       {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                   misc_stats.outbound_req_405);
       }
	icf_ssa_invite_resp_stats(p_ssa_pdb,dRespCode,0);
#endif
    /* for 180 headers are required as there is an API to 
	 * be returned to Application (alert user).
	 */
	if ( (ICF_FAILURE != ret_val) && (180 == dRespCode) )
    {
        /* get the application specific headers from dbm.      
        * These headers shall be filled in call context based 
        * on the app_id in call ctxt. Currently use app id = 1
        */
        if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
            p_ssa_pdb,ICF_INT_MODULE_CC,
		(icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
            (*ppCallObj)->pUacTransaction->pSipMsg,
            p_ssa_pdb->p_ecode) )
        {
                /*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/
                
            /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                &p_inc_media_sdp_info);
                
            ret_val = ICF_FAILURE;
        }
    } /* if !failure ends */

	/* When 181/182/183 without sdp is received, send call progress ind to CC */
	if ((ICF_SUCCESS == ret_val) && (ICF_TRUE == progress_msg_wo_sdp))


	{
		icf_nw_inc_call_progress_st			*p_call_progress = ICF_NULL;

		p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CALL_PROGRESS;
        p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
			ICF_PORT_SIZEOF(icf_nw_inc_call_progress_st);
		
		p_call_progress = (icf_nw_inc_call_progress_st *)
								(p_ssa_pdb->p_internal_msg->p_msg_data);

		/* initialize the payload */
		icf_port_memset(	
			p_call_progress,
			0,
			ICF_PORT_SIZEOF(icf_nw_inc_call_progress_st));

		/* fill the status code */
		p_call_progress->response_code = dRespCode;


        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

	}
	
#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
	/* Extract P-Media-Authorization Header */
	if (ICF_FAILURE != ret_val)
	{
		if (ICF_FAILURE == icf_ssa_extract_media_authorization_token(
			p_ssa_pdb, (*ppCallObj)->pUacTransaction->pSipMsg))
		{
			/*ICF_SSA_TRACE(("\nFailed to get P-Media-Authorization header"))*/

            /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                &p_inc_media_sdp_info);
                  
			ret_val = ICF_FAILURE;
		}
	}
ICF_CHECK_IMS_END
ICF_CHECK_QOS_SUPPORT_END
#endif
#endif
	if(ICF_FAILURE != ret_val)
	{
		/* Free the p_inc_media_sdp_info structure. 
		*/
		if(ICF_NULL != p_inc_media_sdp_info)
		{
			p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
			p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;

            if(ICF_NULL != p_inc_media_sdp_info->p_nw_reject_stream_list)
            {
                icf_cmn_free_media_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                        &((p_inc_media_sdp_info)->p_nw_reject_stream_list),
                        p_ssa_pdb->p_ecode);
            }

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

/*********************************************************
** FUNCTION: sdf_cbk_uaReInviteFailed
**
** DESCRIPTION: Callback issued onto the application on the receipt of an
**              final failure response message indicating failure of the
**              reinvite request
**         1. Send ACK to peer
**         2. Map response code to internal response codes and set bitmask as 
**            mc_change_resp_reason
**         3. Send msg to CM
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaReInviteFailed(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_ty_u16bit                       dRespCode = 0;
    SipOptions                          options;
    Sdf_ty_u32bit                       size = 0;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_media_change_resp_st     *p_internal_msg_param = ICF_NULL;
#ifdef ICF_SESSION_TIMER
	SipHeader	dTempHeader;
    icf_nw_otg_session_refresh_req_st   session_refresh_req;
#endif
    Sdf_ty_slist         slMatchedHeadersList;
    SipHeader           *pRetryAfterHeader = Sdf_co_null;
    Sdf_st_headerList   *pRetryAfterHeaderList = Sdf_co_null;
    icf_boolean_t       authenticate_req = ICF_FALSE;
    /*CSR 8061646 SPR 20231*/
    icf_nw_otg_media_change_req_st	   media_chng_req;
 

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* Cast the payload into the icf_nw_inc_media_change_resp_st structure */
    p_internal_msg_param = 
    (icf_nw_inc_media_change_resp_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    /* Initialise various fields of the internal message */
    p_internal_msg_param->bitmask = ICF_NULL;
    p_internal_msg_param->p_sdp_list = ICF_NULL;
     
    /* Fetch the response code from the SIP message */
    if(Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                (*ppCallObj)->pUacTransaction->pSipMsg,
                &dRespCode,pErr))
    {
       p_ssa_ctxt->p_call_ctxt->inc_response_code = dRespCode;
      
      /* This switch case is used to retrive the retry header field value if 
         present,from some specific final failure responses of Re-INVITE */ 

       switch(dRespCode)
       {
        
        case 404:
        case 413:
        case 480:
        case 486:
        case 500:
        case 503:
        case 600:
        case 603:
                 if (Sdf_co_fail == sdf_ivk_uaGetHeaderListByType(
                        (*ppCallObj)->pUacTransaction->slUnprocessedHeaders,
                        SipHdrTypeRetryAfterSec,
                        &slMatchedHeadersList,
                        pErr))
                 {
                    ret_val = Sdf_co_fail;
                 }
                 else if (Sdf_co_fail == sdf_listSizeOf(
                        &slMatchedHeadersList,&size, pErr))
                 {
                    sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                    ret_val = Sdf_co_fail;
                 }
                else if (ICF_NULL != size)
                 {

                   /* Get HeaderList at index 0 */
                    if (Sdf_co_fail == sdf_listGetAt(
                                 &slMatchedHeadersList,
                                 0,
                                 (Sdf_ty_pvoid *)&pRetryAfterHeaderList,
                                 pErr))
                    {
                        ret_val = Sdf_co_fail;
                    }
                    else if (Sdf_co_fail == sdf_listGetAt(
                               &(pRetryAfterHeaderList)->slHeaders,
                                0,
                                (Sdf_ty_pvoid *)&pRetryAfterHeader,
                                pErr))
                    {
                        ret_val = Sdf_co_fail;
                    }
                    else
                    {
                        p_ssa_ctxt->p_call_ctxt->retry_after_duration =
                                 (((SipRetryAfterHeader *)
                                 (pRetryAfterHeader->pHeader))->u).dSec;
                        sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                    }
                 }
                break;
       }

#ifdef ICF_SESSION_TIMER 
      if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
      {
	if ( 422 == dRespCode)
	{
		if (sip_getHeader((*ppCallObj)->pUacTransaction->pSipMsg,SipHdrTypeMinSE, &dTempHeader, \
			(SipError*)&(pErr->stkErrCode)) != SipFail)
		{
			sip_freeSipHeader(&dTempHeader);
			/* Fix for SPR 14409 */
			if ( (ICF_NULL !=(*ppCallObj)->pSessionTimer) &&
				(90 > (*ppCallObj)->pSessionTimer->dMinSe))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Min-SE in 422 is less than 90, Clear the Call\n"));
				p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
					ICF_GENERIC_CALL_CLEAR;
				p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
					p_ssa_pdb->p_glb_pdb;

				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

				ret_val = Sdf_co_fail;
			} 

		}
		else 
		{
			p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
			p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
				ICF_GENERIC_CALL_CLEAR;
			p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
				p_ssa_pdb->p_glb_pdb;

			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]: 422 received wo MIn-SE so Call will be cleared\n"));

		}	

	}
    }
#endif
    /* SPR 17555 Fix: Send call direction to app_trace for SIP PDUs
     */
#ifdef ICF_TRACE_ENABLE
          p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
#endif
    ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
    p_ssa_pdb->common_bitmask |= ICF_SSA_MSG_TRACE_ALREADY_SENT;
    /* 
     * Now send an ACK message to peer
     */
    /* Specify the stack options */
    options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
    if (Sdf_co_fail == sdf_ivk_uaFormRequest(
                (Sdf_ty_s8bit *)"ACK",(*ppCallObj), Sdf_co_null,
                pErr))
    {
        ret_val = Sdf_co_fail;
    }
    else if(ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),
                    (*ppCallObj)->pUacTransaction->pSipMsg,
                    &options,pErr))
        {
            /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
            ret_val = Sdf_co_fail;
        }
#ifdef ICF_SESSION_TIMER
    if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    { 
	if (ICF_INT_MODULE_CM == p_ssa_pdb->dest_module_id)
	{
		ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
		return Sdf_co_success;
	}
    if((dRespCode == 422) && (p_ssa_pdb->p_internal_msg->msg_hdr.msg_id !=ICF_GENERIC_CALL_CLEAR))
	    {
		   /* CSR 1-8061646 SPR 20231: Check the bitmask_1 in ssa context. 
			 * 1. if Reinvite sent because of Call Modify then form internal message
			 * to send the same Reinvite again with new Min-SE value.
			 * 2. If Reinvite sent because of Call-Hold/Resume then form internal 
			 * message to send the same reinvite again with new Min-SE value.
			 * 3. Else Assume it as a session refresh Reinvite and form the internal 
			 * message for the same.
			 */

		   if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY == (ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY & p_ssa_ctxt->bitmask_1))
			{
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_MODIFY_REQ;
				p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st);
				media_chng_req.mesg_type = ICF_NW_OTG_SEND_RE_INVITE;
				p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_chng_req;
			}
			else if((ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD == (ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD & p_ssa_ctxt->bitmask_1))
						||
					(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME == (ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME & p_ssa_ctxt->bitmask_1)))
			{
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE;
				p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
						ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st);
				if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD  & p_ssa_ctxt->bitmask_1)
				{
					media_chng_req.call_hold = ICF_TRUE;
				}
				else
	{
					media_chng_req.call_hold = ICF_FALSE;
				}
				p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_chng_req;
			}
			else
            {
			    /* Send ICF_NW_OTG_SESSION_REFRESH_REQ to SSA to send re-invite
						 * with session interval equals to the value of Min-SE header */

				/* Fix for SPR 31210 glb configuration data updation code deleted */ 
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_SESSION_REFRESH_REQ;
				p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
				ICF_PORT_SIZEOF(icf_nw_otg_session_refresh_req_st);
				session_refresh_req.refresher_req_type = ICF_SSA_REFRESHER_METHOD_INVITE;
				p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&session_refresh_req;
				}

			/* SPR 18147: Session Expires header not sent when 422 doesnot contain
					 * session-expires and Supported contain "timer" parameter
					 */
			(*ppCallObj)->pSessionTimer->dSessionInterval = \
                        (*ppCallObj)->pSessionTimer->dMinSe;

			ret_val = icf_ssa_process_mesg(p_ssa_pdb->p_internal_msg) ;

                return ret_val;
			
			}

    
	if(p_ssa_ctxt->session_timer_expired_refresh == ICF_TRUE)
	{
           /* Fix for CSR 1_7334810: SPR-19732*/
           /* The call will be clered after receiving 481/408 response from peer
            * for session refresh Re-INVITE.
            * Refered from RFC-3261 section-12.2.1.2
            * "If the response for a request within a dialog is a 481
               (Call/Transaction Does Not Exist) or a 408 (Request Timeout), the UAC
               SHOULD terminate the dialog.  A UAC SHOULD also terminate a dialog if
               no response at all is received for the request (the client
               transaction would inform the TU about the timeout.)"
            */ 
            if((481 != dRespCode) && (408 != dRespCode) && 
					(401 != dRespCode) && (407 != dRespCode) && (Sdf_co_fail != ret_val))
            {
                                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                ICF_NW_OTG_SESSION_REFRESH_ACK;
                                p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;
                                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

		                p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                                ret_val = Sdf_co_success;
                                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                                return ret_val;
            }
            else if(((dRespCode != 422) && (401 != dRespCode) && (407 != dRespCode)) || 
				(dRespCode == 422 && ret_val == Sdf_co_fail))
            {
                                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                 ICF_NW_OTG_SESSION_REFRESH_ACK;     
				p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
					p_ssa_pdb->p_glb_pdb;
                                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
               		               ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);
               ((icf_nw_inc_media_change_resp_st*)p_ssa_pdb->p_internal_msg->p_msg_data)->status = ICF_FALSE; 
   
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
				ret_val = Sdf_co_success;
                                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                                return ret_val; 
			}
    }
    }
#endif
        /*   Handling of 481 REceived for ReInvite */
        if((481 == dRespCode)||(408 == dRespCode))
        {
		    p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                            ICF_GENERIC_CALL_CLEAR;
            p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb =
                            p_ssa_pdb->p_glb_pdb;

            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: 481 received for ReInvite so Call will be cleared\n"));
            return Sdf_co_success;
        } 
        else if(401 == dRespCode || 407 == dRespCode)
		{
		    /* In case ReInvite was already challenged earlier and Invite with
		     *  Auth params is being challenged again, call the function icf_ssa_validate_challenge
                     *  to do the stale processing
			 * CSR 1-7302382_SPR_19736. In case, if no other request is challenged initially,
			 * then there is no need to validate challenge and IPTK should handle this challenge.
		     */
		     if ((ICF_NULL == (ICF_CALL_AUTHENTICATION_REQ & 
			      p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)) ||
				((ICF_NULL != (ICF_CALL_AUTHENTICATION_REQ & 
			      p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)) &&
	                 (ICF_SUCCESS == icf_ssa_validate_challenge(ppCallObj,ICF_NULL,
                                        dRespCode,p_ssa_pdb,pErr,(icf_uint8_t *)"INVITE"))))
		     {		 
                  /*
				  * Since this is a fresh reinvite with Authorization parameters and there
				  * is no final response received for this INVITE so rset the
				  * ICF_FINAL_RESP_RECEIVED bitmask in ssa_context. This bitmask would
				  * have got set when 407 response was received for the request sent
				  * without authorization parameters
				  */
				p_ssa_ctxt->bitmask &= (~ICF_FINAL_RESP_RECEIVED);

				/* No Need to check return value as, we need to send ack
				 * If we set failure in ret_val, nothing happens*/
				icf_ssa_fetch_auth_param_list(p_ssa_pdb,
						ppCallObj,ICF_NULL);


				if(ICF_NULL != p_ssa_ctxt->auth_params_list_size)
				{
					/* fetch line data and check if line has auth param configured,
					 * if yes set call auth as true */
					/* Fetch Line data from DBM */
					if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
								p_ssa_pdb->p_glb_pdb,
								p_ssa_pdb->p_glb_pdb->p_call_ctx->line_id,
								&(p_ssa_pdb->p_line_data),
								p_ssa_pdb->p_ecode))
					{
						ret_val = ICF_FAILURE;
					}
					else if((p_ssa_pdb->p_line_data->line_data_received &
								ICF_CFG_LINE_DATA_AUTH_KEY_RECD) &&
							(p_ssa_pdb->p_line_data->line_data_received &
							 ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD))

					{
						/* Auth params are present in response from Peer,
						 * set bitmask in call context for the same */
						p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask|=
							ICF_CALL_AUTHENTICATION_REQ;

						if(ICF_TRUE == ICF_IS_BIT_SET(
									p_ssa_pdb->p_line_data->line_data_received, ICF_CFG_LINE_DATA_REALM_RECD))
						{
							/* We have to compare the received realm with configured one */
							Sdf_st_authenticationParams *pAuthenticationParams = Sdf_co_null;

							if(Sdf_co_success == sdf_listGetAt(p_ssa_ctxt->p_auth_params_list, 0,
										(Sdf_ty_pvoid *)&pAuthenticationParams,
										pErr))
							{
								if(Sdf_co_null != pAuthenticationParams->pRealm)
								{
									icf_string_st temp_unq_realm = {0, "0"};
									temp_unq_realm.str[0]='\0';
									temp_unq_realm.str_len=0;

									sdf_fn_uaUnq(pAuthenticationParams->pRealm,(icf_int8_t *)temp_unq_realm.str);

									if(0 != icf_port_strcmp(p_ssa_pdb->p_line_data->configured_realm.str,
												temp_unq_realm.str))
									{
										/*Set the release cause in the call context*/
										p_ssa_pdb->p_glb_pdb->p_call_ctx->release_cause =
											ICF_CAUSE_REALM_MISMATCH;

										ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
												ICF_CALL_AUTHENTICATION_REQ);
									}
									else
									{
										authenticate_req = ICF_TRUE;
									}
								}
							}
						}
					}
				} /* if(ICF_NULL != p_ssa_ctxt->auth_params_list_size) */
			} /* if-else (ICF_CALL_AUTHENTICATION_REQ & ..) */
			else
			{
				/* SPR 19736 
                 * If validate challenge fails, then call should be clear*/
#ifdef ICF_SESSION_TIMER	
			if ((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && 
					(ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh))
				{           
					p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
								ICF_NW_OTG_SESSION_REFRESH_ACK;
					p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);
					p_internal_msg_param->status = ICF_FALSE;

					p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
								p_ssa_pdb->p_glb_pdb;

					p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
					return Sdf_co_success;
				}
#endif
			}
		} /* if(401 == dRespCode || 407 == dRespCode) */
  
        if(ICF_TRUE == authenticate_req)
		{
            /* Fetch and store the old nonce in the ssa context */
            if(ICF_FAILURE == icf_ssa_extract_nonce_from_challenge(
                                  p_ssa_pdb,
                                  dRespCode))
            {
                ret_val = ICF_FAILURE;
            }
			p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE"; 
            ret_val = icf_ssa_send_authorized_req(p_ssa_pdb);
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;
		}     
        else
        {
            /* CSR 8061646 SPR 20231 : In case of sending failure to application, reset the media modify
				   method bitmasks */
				   if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY & p_ssa_ctxt->bitmask_1)
		           {
			          p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY);
		           }
		           else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD & p_ssa_ctxt->bitmask_1)
		           {
			          p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD);
		           }
		           else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME & p_ssa_ctxt->bitmask_1)
		           {
			          p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME);
		           }
/*call_mod_cancel*/
        if(ICF_CALL_MOD_CANCEL_REQ & 
           p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
        {
           /*This is the case when either :
             1. 4xx resp for Re-Invite is rcvd and media revert
                is going on for cancel call modify request
             2. 4xx resp for Re-Invite is rcvd and CANCEL was already 
                sent on n/w
                and waiting for 200 OK of CANCEL*/
           /*Reset this common bitmask*/
           p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 &= 
                                           ~(ICF_CALL_MOD_CANCEL_REQ);

           /*Set the flag to send response to PA for case:
             4xx resp for Re-Invite is rcvd and CANCEL was already
             sent on n/w and waiting for 200 OK of CANCEL*/

           ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                                          ICF_CALL_MOD_REINV_CANCELLED)

        }
        else if(ICF_CALL_MOD_CANCEL_REINV_REQ & 
           p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
        {
           /*Case when 4xx of 2nd ReInvite is received after sending
             ReInvite again after receiving 200 OK of ReInvite when
             CANCEL is sent on network before*/

           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: 4xx received for 2nd ReInvite so Call will be cleared\n"));

           ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                                          ICF_CALL_MOD_REINV_2_CANCELLED)
           /*Reset this common bitmask*/
           p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 &= 
                                           ~(ICF_CALL_MOD_CANCEL_REINV_REQ);
        }

        /*Also reset this flag so that if 200 OK if CANCEL 
          is received in future 
          then it gets ignored */
        p_ssa_ctxt->bitmask &= ~ICF_SSA_REINVITE_CANCELLED;

	    /* Populate the internal message header */
	    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
		    ICF_NW_INC_MEDIA_UPDATE_RESP;
	    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
		    ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);
	    /*
	     * Populate the resp reason after mapping the 
	     * appropiate reason .
	     * Set the status as failure
	     */
	    ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
			    ICF_NW_INC_MC_RESP_REASON)

		    if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
					    dRespCode,&(p_internal_msg_param->reason_type)))
		    {
			    ret_val = Sdf_co_fail;
		    }
		    else
		    {
                /*In case of 488/606, set this reason*/
                if((ICF_CALL_TYPE_T38 & 
                           p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->call_type)
                                  && (606 == dRespCode))
                {
                   ((icf_nw_inc_media_change_resp_st*)
                    p_ssa_pdb->p_internal_msg->p_msg_data)->reason_type =
                            ICF_CALL_CLEAR_MEDIA_MISMATCH;
                }
			    ((icf_nw_inc_media_change_resp_st*)
			     p_ssa_pdb->p_internal_msg->p_msg_data)->status = ICF_FALSE; 

			    /*
			     * All messages to be sent to CC/SC are routed to the CM.
			     * CM decides the specific destination of these messages
			     */
			    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
			    /*
			     * Reset the media state so that new RE_INVITE can
			     * be sent 
			     * SPR FIX - 7410
			     */
			    (*ppCallObj)->pMediaInfo->dMediaState = Sdf_en_idleMedia;
		    }
        }
    }
else
{
        ret_val = Sdf_co_fail;
    } /* else */
#ifdef ICF_STAT_ENABLE
	icf_ssa_invite_resp_stats(p_ssa_pdb,dRespCode,0);
#endif

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*********************************************************
** FUNCTION: sdf_cbk_uaReInviteAccepted
**
** DESCRIPTION: Callback issued onto the application on the receipt of an
**              200 OK message to the re-invite message sent
**         1. If SDP body present then extract it from incoming message
**         2. Send ACK to peer
**         3. Send inc_media_update_resp to CM
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaReInviteAccepted(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    SipOptions                          options;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_media_change_resp_st     *p_internal_msg_param = ICF_NULL;
    icf_boolean_t                       call_clear_ind = ICF_FALSE;
#ifdef ICF_SESSION_TIMER
	Sdf_ty_refresher                    dRefresher;
    SipMessage                          *pMessage = ICF_NULL;
#endif

	/*This structure is populated if there is any SDP.*/
	/*Then we populate SDP in the internal stream*/
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    if (ICF_NULL == p_ssa_ctxt)
    {
       return ICF_FAILURE;
    } 
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    if ((ICF_NULL == p_ssa_pdb) || 
        (ICF_NULL == p_ssa_pdb->p_glb_pdb) ||
        (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
    {
       return ICF_FAILURE;
    }
  
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
/* CSR 8061646 SPR 20231 : reset the media modify method bitmask here */
	if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY & p_ssa_ctxt->bitmask_1)
	{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY);
	}
	else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD & p_ssa_ctxt->bitmask_1)
	{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD);
	}
	else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME & p_ssa_ctxt->bitmask_1)
	{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME);
	}

#ifdef ICF_SESSION_TIMER
    if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    {
      if(sdf_ivk_uaGetRefresher(*ppCallObj,&dRefresher,pErr) != Sdf_co_fail)
      {   
	if ( Sdf_en_invalidRefesher == dRefresher)
        {       
                p_ssa_ctxt->p_call_obj->pSessionTimer->dSessionInterval = 0;
                p_ssa_ctxt->p_call_obj->pSessionTimer->dMinSe = 0;
				p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                          ICF_GENERIC_CALL_CLEAR;
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb =
                          p_ssa_pdb->p_glb_pdb;
				options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        		if (Sdf_co_fail == sdf_ivk_uaFormRequest(
                	(Sdf_ty_s8bit *)"ACK",(*ppCallObj), Sdf_co_null,
                	pErr))
        		{
            		ret_val = Sdf_co_fail;
        		}
        		else if(ICF_FAILURE == 
					icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),
               		(*ppCallObj)->pUacTransaction->pSipMsg,
               		&options,pErr))
        		{
           			 /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
            		ret_val = Sdf_co_fail;
        		}

                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_success;
        }
    }

    /* SPR 9591- pass the SPI message instead of the trans. pointer */
    /* If Remote UA supports UPDATE method then 
     * store update as subsequent session refresh request
     * otherwise store invite as subsequest session refresh request */
    pMessage = (*ppCallObj)->pUacTransaction->pSipMsg;
    icf_ssa_set_refresher_method(pMessage,
        p_ssa_pdb->p_glb_pdb,
        &p_ssa_ctxt,
        pErr);
   icf_dbm_fetch_line_cntxt_blk(p_ssa_pdb->p_glb_pdb,
                         p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->line_id,
                         &p_ssa_pdb->p_line_data,
                         p_ssa_pdb->p_ecode);

    /* start the session timer if it is not started by UATK (if 
     * 2xx response does not contain Session Expires Header then
     * UATK will not start the session timer) */
  if(dRefresher != Sdf_en_refresherNone || 
				ICF_SESSION_REFRESHER_NONE != 
				p_ssa_pdb->p_line_data->session_timer_refresher)
    {
    if(icf_ssa_start_session_timer(ppCallObj,
         p_ssa_pdb->p_glb_pdb,
#ifdef ICF_WITH_MUATK_2_2
         pEventContext,
#endif
        pErr) == Sdf_co_fail)
    {
        ret_val = Sdf_co_fail;
    }
    }
   }
#endif /*#ifdef ICF_SESSION_TIMER*/

    /* Cast the payload into the icf_nw_inc_media_change_resp_st structure */
    p_internal_msg_param = 
        (icf_nw_inc_media_change_resp_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    if(ICF_NULL != p_internal_msg_param)
    {
        p_internal_msg_param->bitmask = ICF_NULL;
        p_internal_msg_param->p_sdp_list = ICF_NULL;
    }

    /* Check for presence of SDP */
    if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent(
                (*ppCallObj)->pUacTransaction->pSipMsg,
                pErr))
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
                                (*ppCallObj)->pUacTransaction->pSipMsg,\
								ICF_SSA_RECVD_ANSWER_TO_OFFER, \
                                &p_inc_media_sdp_info,
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
			  call_clear_ind = ICF_TRUE;	
                ret_val = Sdf_co_success;
            }
            else
            {
                ret_val = Sdf_co_fail ;
            }
        }
        else
        {

           if (ICF_NULL != (p_inc_media_sdp_info)->p_nw_reject_stream_list)
           {
                icf_cmn_free_media_sdp_info_list(p_ssa_pdb->p_glb_pdb,
                               &((p_inc_media_sdp_info)->p_nw_reject_stream_list),
                               p_ssa_pdb->p_ecode);
           }

           /*
            * Set the bit which indicates that 200 OK
            * of RE_INVITE has been received with SDP
            * Also set the status as success
            */
            if(ICF_NULL != p_inc_media_sdp_info->p_sdp_info_list &&
				ICF_NULL != p_internal_msg_param)
			{
            	p_internal_msg_param->p_sdp_list =\
                	p_inc_media_sdp_info->p_sdp_info_list;
				ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                	ICF_NW_INC_MC_SDP_PRESENT)
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
            /* ICF_SSA_ADD_MEM_CHUNK_TO_PTR_LIST(p_ssa_pdb,
                &p_internal_msg_param->p_sdp_list,
                ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST)*/


        }
    }
    /* Changes done for Message Body Headers Start : */
    else
    {	
        if(ICF_SSA_UPDATE_WITH_SDP & p_ssa_ctxt->bitmask)
        {
            /* Update Response received without SDP. Send Generic Call Clear
             * Indication to CC and proper reason cause.
             */
            p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                ICF_GENERIC_CALL_CLEAR;
            p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                p_ssa_pdb->p_glb_pdb;
            
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
            p_internal_msg_param->reason_type = \
                ICF_CALL_CLEAR_REASON_REMOTE_MEDIA_NOT_RECEIVED;
            call_clear_ind = ICF_TRUE;
            p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->signalling_ongoing=ICF_FALSE;

        }
        else
        {
            if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
                        (*ppCallObj)->pUacTransaction->pSipMsg,\
                        SipMessageResponse,&(p_ssa_pdb->p_glb_pdb->p_call_ctx->body_list)))
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
                ret_val = ICF_FAILURE;
            }
        }
    }
    /* Changes done for Message Body Headers End :
     */

    /* get the application specific headers from dbm.
     * These headers shall be filled in call context based
     * on the app_id in call ctxt. Currently use app id = 1
     */
	/* Fix for CSR 1-7397689: SPR 19747 */
    /* Do not populate the application specific headers in call context if the
     * success response is received for session refresh ReInvite.
     * Reason: In such scenario no indication is given to application and thus
     * header list populated here will never get free and subsequently these 
     * headers will be send in all outgoing request.*/
#ifdef ICF_SESSION_TIMER     
    if (ICF_FALSE == p_ssa_ctxt->session_timer_expired_refresh)
    {
#endif
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
         p_ssa_pdb,ICF_INT_MODULE_CC,
         (icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
         (*ppCallObj)->pUacTransaction->pSipMsg,
         p_ssa_pdb->p_ecode)))
    {
        /*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/

        /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                &p_inc_media_sdp_info);

        ret_val = ICF_FAILURE;
    }
#ifdef ICF_SESSION_TIMER          
    }
#endif

#ifdef ICF_TRACE_ENABLE
    /* SPR 17555 Fix: Send call direction to app_trace for SIP PDUs
     */
         p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
#endif    
    ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
    p_ssa_pdb->common_bitmask |= ICF_SSA_MSG_TRACE_ALREADY_SENT;
    /* 
     * Now send an ACK message to peer
     */
    /* Specify the stack options */
    options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
    if (Sdf_co_fail == sdf_ivk_uaFormRequest(
                (Sdf_ty_s8bit *)"ACK",(*ppCallObj), Sdf_co_null,

                pErr))
    {
      		/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
            &p_inc_media_sdp_info);

        ret_val = Sdf_co_fail;
    }
    else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                 (*ppCallObj)->pUacTransaction->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),
                (*ppCallObj)->pUacTransaction->pSipMsg,
                &options,pErr))
    {
        /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
            &p_inc_media_sdp_info);

        /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
        ret_val = Sdf_co_fail;
    }
    else
    {
	ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.
			        stat_outbound_req.stats_ACK);
    }
    if ((Sdf_co_fail != ret_val)&&
		(ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id))
    {
        if(ICF_FALSE == call_clear_ind)
        {
            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);
#ifdef ICF_SESSION_TIMER
            if((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && \
               (ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh))
            {       
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_NW_OTG_SESSION_REFRESH_ACK;
                p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;
            }
            else
#endif
            {
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                    ICF_NW_INC_MEDIA_UPDATE_RESP;
            }
            ((icf_nw_inc_media_change_resp_st*)
                p_ssa_pdb->p_internal_msg->p_msg_data)->status = ICF_TRUE; 
                /*
                 * All messages to be sent to CC/SC are routed to the CM.
                 * CM decides the specific destination of these messages
                 */
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
        /* 
         * Delete the message body list stored in the UAC
         * transaction of the call object.
         * This is done to flush the SDP body from the 
         * transaction substructure of the call object
         */
        sdf_listDeleteAll(
                &((*ppCallObj)->pUacTransaction->slMsgBodyList),
                pErr);
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

	/* Reset the bitmask for SDP in Update/Re-Invite if set 
	*/
	if(ICF_SSA_UPDATE_WITH_SDP & p_ssa_ctxt->bitmask)
	{
		p_ssa_ctxt->bitmask &= ~(ICF_SSA_UPDATE_WITH_SDP);
	}
#ifdef ICF_STAT_ENABLE
	icf_ssa_invite_resp_stats(p_ssa_pdb,200,0);
#endif

/*call_mod_cancel*/
	/* Reset the bitmask for Call Modify req cancel 
       as 200 OK of Re-Invite is
       received before 200 OK of CANCEL 
	*/

	if(ICF_CALL_MOD_CANCEL_REQ & 
       p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
	{
        /*Reset this common bitmask*/
		p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 &= 
                                   ~(ICF_CALL_MOD_CANCEL_REQ);

        /*Set common bitmask to send Re-Invite again after 
          receiving 200 OK of Re-Invite when 
         *CANCEL was sent for call modify cancel request*/

		p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 |= 
                                  ICF_CALL_MOD_CANCEL_REINV_REQ;

        /*Reset this flag so that if 200 OK if CANCEL is received 
          in future then it gets ignored */
        p_ssa_ctxt->bitmask &= ~ICF_SSA_REINVITE_CANCELLED;

    }
    else
    {

	    if(ICF_CALL_MOD_CANCEL_REINV_REQ & 
              p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
	    {

            /*Set common bitmask to send Re-Invite again after 
              receiving 200 OK of Re-Invite when 
             *CANCEL was sent for call modify cancel request*/

		    p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 |= 
                                         ICF_CALL_MOD_CANCEL_REQ;

        }
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/**********************************************************
 ** FUNCTION:  sdf_cbk_uaRPRReceived
 **
 ** DESCRIPTION: Callback issued to the application on the receipt of a
 **              reliable provisional response
 **     1. Fetch response code from SIP message
 **     2. If it is 180 and SDP is received, fetch SDP from SIP msg. Set 
 **        bitmask that RPR required and send inc_alert_with_media received, 
 **        else send inc_alert to CM.
 **     3. If resp_code is 183, then fetch SDP from SIP msg and set bitmask
 **        inc_media_param_rpr_reqd and send inc_media_param to CM
 **********************************************************/
Sdf_ty_retVal sdf_cbk_uaRPRReceived(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_overlapTransInfo *pOverlapTransInfo,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_ty_u16bit                       dRespCode = 0;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_error_t                         *p_ecode =  ICF_NULL;    
    icf_uint8_t                         msg_identifier=0;

	/*This structure is basically if there is any sdp
	 * this is then populated in internal msg
	 */
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->
        p_internal_msg->p_msg_data))->forked_resp_id = ICF_NULL;
    ((icf_nw_inc_media_param_st*)(p_ssa_pdb->
        p_internal_msg->p_msg_data))->forked_resp_id = ICF_NULL;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* To remove compiler warning */
    (void)ppCallObj;

    if (ICF_NULL == p_ssa_ctxt->p_call_ctxt)
    {
        return Sdf_co_success;
    }

    /* Fetch the response code from the SIP message */
    if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                pOverlapTransInfo->pSipMsg, &dRespCode,
                pErr)) 
    {
        ret_val = Sdf_co_fail;
    }
    else
    {
        /* 
         * Switch on the response code and fill the appropiate 
         * API in the internal message
         */
        switch(dRespCode)
        {
            /* A reliable 180 Ringing response is received */
            case 180:
                /* Check for the presence of SDP in the message */
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

                    /* 
                     * Cast the payload of the internal msg into the
                     * icf_nw_inc_alert_media_st structure 
                     */
                    icf_nw_inc_alert_media_st *p_internal_msg_param = 
                        (icf_nw_inc_alert_media_st *)
                        p_ssa_pdb->p_internal_msg->p_msg_data;
	            
                    if(ICF_CALL_CONNECT_DONE & p_ssa_pdb->p_glb_pdb->\
                        p_call_ctx->common_bitmask)
                    {
                        msg_identifier = ICF_SSA_RECVD_ANSWER_TO_OFFER;
                    }

                    if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                            p_ssa_pdb,
                                            pOverlapTransInfo->pSipMsg,
											msg_identifier, &p_inc_media_sdp_info,
                                            p_ecode))
                      
                    {
                        /* Free the acquired memory for the SDP */
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
                    }
                    else
                    {
                        
                        ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->p_internal_msg->p_msg_data))->\
                            p_sdp_list = p_inc_media_sdp_info->p_sdp_info_list;

                        /******** Changes for Forking & Multiple Answer Start (Rel7.0) ********/

                        /* forked_resp_id in the SSA context assigned to 
                            forked_resp_id in icf_nw_inc_alert_media_st 
                            and sent to CC */
                        
                        ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->
                         p_internal_msg->p_msg_data))->forked_resp_id = \
                        ((icf_ssa_ctxt_st *)((*ppCallObj)->pAppData)->pData)->forked_resp_counter;

                        /********* Changes for Forking & Multiple Answer End (Rel7.0) *********/
                        /*
                         * Fill the appropiate fields of the 
                         * internal message.Set the bit which
                         * indicates that this is a RPR
                         */
                        
                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                            ICF_NW_INC_ALERT_WITH_MEDIA;
                        ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->
                            p_internal_msg->p_msg_data))->bitmask = ICF_NULL;
                        ICF_SSA_SET_COMMON_BITMASK(
                            p_internal_msg_param->bitmask,
                            ICF_NW_INC_ALERT_MEDIA_RPR_REQD)
                            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                            ICF_PORT_SIZEOF(icf_nw_inc_alert_media_st);
                        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                        /* 
                        * Delete the message body list stored in the UAC
                        * transaction of the call object.
                        * This is done to flush the SDP body from the 
                        * transaction substructure of the call object
                        */
						sdf_listDeleteAll(
                            &((*ppCallObj)->pUacTransaction->slMsgBodyList),
                            pErr);
						}
                    }                
                else
                {
                    /*
                     * If SDP is not present ,
                     * set msg_id = ICF_NW_INC_CALL_ALERT
                     */
                    icf_nw_inc_call_alert_st *p_internal_msg_param = 
                        (icf_nw_inc_call_alert_st *)
                        p_ssa_pdb->p_internal_msg->p_msg_data;
				   /* Message body header changes start for 180 response
				    */
				   /* Extract the message bodies from SIP message.
				    */
				  if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
					    (*ppCallObj)->pUacTransaction->pSipMsg,\
					     /**ppCallObj*/SipMessageResponse,&(p_ssa_pdb->p_glb_pdb->p_call_ctx->\
						 body_list)))
					{
						ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
						ret_val = ICF_FAILURE;
					}
				    /* Message Body Header Changes Ends
					 */
				    				    
                    /*
                     * Fill the appropiate fields of the 
                     * internal message.Set the bit which
                     * indiactes that this is a RPR
                     */
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                        ICF_NW_INC_CALL_ALERT;
                    ((icf_nw_inc_call_alert_st*)(p_ssa_pdb->
                    p_internal_msg->p_msg_data))->bitmask = ICF_NULL;
                    ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
                            ICF_NW_INC_ALERT_RPR_REQD)
                    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                        ICF_PORT_SIZEOF(icf_nw_inc_call_alert_st);
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                }
                break;
#if 0
            case 181    :
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                    ICF_NW_INC_REDIRECT_IND;
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_nw_inc_redirect_ind_st);
                break;
#endif

            /* A reliable 183 Session Progress response is received */
            case 183:

                /* Check for the presence of SDP in the message */
                if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent(pOverlapTransInfo->\
					pSipMsg, pErr))
                {
                    /* 
                     * Cast the payload of the internal msg into the
                     * icf_nw_inc_alert_media_st structure 
                     */
                    icf_nw_inc_media_param_st *p_internal_msg_param =
                        (icf_nw_inc_media_param_st *)
                        p_ssa_pdb->p_internal_msg->p_msg_data;

                    if(ICF_CALL_CONNECT_DONE & p_ssa_pdb->p_glb_pdb->\
                        p_call_ctx->common_bitmask)
                    {
                        msg_identifier = ICF_SSA_RECVD_ANSWER_TO_OFFER;
                    }

                    if (ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                            p_ssa_pdb,
                                            pOverlapTransInfo->pSipMsg,
											msg_identifier, &p_inc_media_sdp_info,
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
                    }
                    else
					{	
                        
                        ((icf_nw_inc_media_param_st*)(p_ssa_pdb->p_internal_msg->p_msg_data))->\
                            p_sdp_list = p_inc_media_sdp_info->p_sdp_info_list;

                        /******** Changes for Forking & Multiple Answer Start (Rel7.0) ********/

                        /* forked_resp_id in the SSA context assigned to 
                            forked_resp_id in icf_nw_inc_media_param_st 
                            and sent to CC */

                        ((icf_nw_inc_media_param_st*)(p_ssa_pdb->\
                         p_internal_msg->p_msg_data))->forked_resp_id = \
                        ((icf_ssa_ctxt_st *)((*ppCallObj)->pAppData)->pData)->forked_resp_counter;
                        
                        /********* Changes for Forking & Multiple Answer End (Rel7.0) *********/

                        /*
                         * Fill the appropiate fields of the 
                         * internal message.Set the bit which
                         * indiactes that this is a RPR
                         */
                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                            ICF_NW_INC_MEDIA_PARAM;
                        ((icf_nw_inc_media_param_st*)(p_ssa_pdb->
                            p_internal_msg->p_msg_data))->bitmask = ICF_NULL;
                        ICF_SSA_SET_COMMON_BITMASK(
                            p_internal_msg_param->bitmask,
                            ICF_NW_INC_MEDIA_PARAM_RPR_REQD)
                            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                            ICF_PORT_SIZEOF(icf_nw_inc_media_param_st);
                        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                        
						/* 
                        * Delete the message body list stored in the UAC
                        * transaction of the call object.
                        * This is done to flush the SDP body from the 
                        * transaction substructure of the call object
                        */
                        sdf_listDeleteAll(
                            &(pOverlapTransInfo->slMsgBodyList),
                            pErr);
						}
                    }                 
                else
                {
                  /* Changes done for Message Body Headers Start :
                    */
                    /* Extract the message bodies from SIP message.
                    */
                    if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
					    (*ppCallObj)->pUacTransaction->pSipMsg,\
					     /**ppCallObj*/SipMessageResponse,&(p_ssa_pdb->p_glb_pdb->p_call_ctx->\
						 body_list)))
					{
						ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
						ret_val = ICF_FAILURE;
					}
                    /* Changes done for Message Body Headers End :
                     */
                    /* 
                    * In case SDP is not present in 183,
                    * do not invoke ICF_NW_INC_MEDIA_PARAM
                    * but send back a PRACK for the same
                    */
                    if (Sdf_co_fail == sdf_ivk_uaMakeTransaction(
                        (Sdf_ty_s8bit *)"PRACK",(*ppCallObj),
                        pOverlapTransInfo, pErr))
                    {
                        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
                    }
                    else
                    {
                        SipOptions              options;
                        /* Specify the stack options */
                        options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
                        
                        /* Send PRACK to remote end */
                        if (ICF_FAILURE == icf_ssa_send_to_peer(
                            p_ssa_pdb,
                            (*ppCallObj),
                            pOverlapTransInfo->pSipMsg,
                            &options,pErr))
                        {
                            /*Raise/Set an error here-UATK_PROCESSING_FAILURE*/
                            ret_val = Sdf_co_fail ;
                        }
                    }
                    
                }
                break;

            default :
                ICF_PRINT(((icf_uint8_t *)"Unknown provisional response message\n"));
                ret_val = Sdf_co_fail;

        } /* switch(dRespCode) */
    } /* else */
#ifdef ICF_STAT_ENABLE
	icf_ssa_invite_resp_stats(p_ssa_pdb,dRespCode,0);
#endif

    /* for 180 headers are required as there is an API to 
	 * be returned to Application (alert user).
	 */
	if ( (ICF_FAILURE != ret_val) && (180 == dRespCode) )
    {
        /* get the application specific headers from dbm.      
        * These headers shall be filled in call context based 
        * on the app_id in call ctxt. Currently use app id = 1
        */
        if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
            p_ssa_pdb,ICF_INT_MODULE_CC,
		    (icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
            (*ppCallObj)->pUacTransaction->pSipMsg,
            p_ssa_pdb->p_ecode) )
        {        
            /*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/
                
          		/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                &p_inc_media_sdp_info);

            ret_val = ICF_FAILURE;
        }
    } /* if !failure ends */

#ifdef ICF_QOS_SUPPORT
#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
	/* Extract P-Media-Authorization Header */
	if (ICF_FAILURE != ret_val)
	{
		if (ICF_FAILURE == icf_ssa_extract_media_authorization_token(
			p_ssa_pdb, (*ppCallObj)->pUacTransaction->pSipMsg))
		{
			/*ICF_SSA_TRACE(("\nFailed to get P-Media-Authorization header"))*/

          		/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                &p_inc_media_sdp_info);

			ret_val = ICF_FAILURE;
		}
	}
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_IMS_END
#endif
#endif
#endif

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

/**********************************************************
** FUNCTION:  sdf_cbk_uaOverlapTransactionCompleted
**
** DESCRIPTION: Callback issued onto the application on the 
**              receipt of a final response to a PRACK not UPDATE
**
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaOverlapTransactionCompleted(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_overlapTransInfo *pOverlapTransInfo,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_retVal           ret_val = Sdf_co_success;
    
/*#ifdef IMS_CLIENT*/
    Sdf_ty_s8bit            *pMethod;
    Sdf_ty_u16bit           dRespCode = 0;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;
/*    icf_sdp_st           *p_sdp_data = ICF_NULL;*/
/*    icf_error_t          *p_ecode =  ICF_NULL;*/

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
#ifdef IMS_CLIENT

    /* To remove compiler warning */
    (void)pEventContext;
    (void)ppCallObj;
#ifdef ICF_IPSEC_ENABLED
    if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled)
	{
		/* decrement the active transaction count */
		p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans -= 1;
		ICF_PRINT(("[SSA]: num_active_trans decremented, current value = %d", 
			p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans));
		
		/* reset the threshold counter value */
		p_ssa_pdb->p_ssa_glb_data->num_successive_trans_fail = ICF_NULL;
		
		/* check if the active transaction count == 0. if yes and the
		* response is for a non Register transaction, so send a message
		* to REGM for deleting the SA if the transaction comp. on the 
		* old SA
		*/
		if (ICF_NULL == 
			p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans)
		{
			ret_val = icf_ssa_rgm_delete_old_sa_ind(p_ssa_pdb);
		}
	}
#endif
#endif
        /* Get Method from the SIP Message */
        if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                    pOverlapTransInfo->pSipMsg, &pMethod,pErr))
        {
            ret_val = Sdf_co_fail;
        }
        /* Fetch the response code from the SIP message */
        else if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                    pOverlapTransInfo->pSipMsg, &dRespCode,
                    pErr)) 
        {
            ret_val = Sdf_co_fail;
        }        
        else if (0 == (Sdf_mc_strcmp(pMethod, "PRACK")))
        {
            /* If response for PRACK is received 
             * Cast the payload of the internal msg into the
             * icf_nw_inc_prack_resp_st structure 
             */
            icf_nw_inc_prack_resp_st *p_internal_msg_param = 
                (icf_nw_inc_prack_resp_st *)
                p_ssa_pdb->p_internal_msg->p_msg_data;

            /*
             * Fill the appropiate fields of the internal message.
             */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                ICF_NW_INC_MEDIA_PARAM_PRACK_RESP;

            if (200 == dRespCode)
            {

                /* Set the status which indicates SUCCESS response for PRACK. */
                p_internal_msg_param->status = ICF_TRUE;
            }
            else
            {
				/*Fix for CSR 1-7507275 SPR_19881*/
				if ((401 == dRespCode) || (407 == dRespCode))		
	    	    {	 
					p_ssa_pdb->p_method = (icf_uint8_t *)"PRACK"; 

		   			if (ICF_SUCCESS == icf_ssa_handle_chal_resp(ppCallObj,pEventContext,
													 pOverlapTransInfo,dRespCode, pErr))
					{
						/* Auth params are present in response from Peer,
				  	    * set bitmask in call context for the same */
						p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask|=
										ICF_CALL_AUTHENTICATION_REQ;

						ret_val = Sdf_co_success;
   					    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
						return ret_val;
					}
				}	

                /* Set the status which indicates FAILURE response for PRACK. */
                p_internal_msg_param->status = ICF_FALSE;

                icf_ssa_map_sip_resp_code(dRespCode, 
                    &(p_internal_msg_param->error_cause));
            }

            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                ICF_PORT_SIZEOF(icf_nw_inc_prack_resp_st);

            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

            ret_val = Sdf_co_success;
        }
        else
        {
            /* Neither response for PRACK nor UPDATE was received */
            /* We are ignoring this response */
            ret_val = Sdf_co_success;
        }        
#ifdef ICF_STAT_ENABLE
        if (0 == (Sdf_mc_strcmp(pMethod, "PRACK")))
        {
                icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)pMethod,dRespCode,0);
        }
#endif

    ret_val = Sdf_co_success;
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} 


/*********************************************************
** FUNCTION:sdf_cbk_uaByeRejected
**
** DESCRIPTION: Callback issued onto the application on the receipt of an
**              Non 2xx response message to the BYE sent
**
**********************************************************/
Sdf_ty_retVal sdf_cbk_uaByeRejected(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
        Sdf_ty_retVal           ret_val = Sdf_co_success;
    Sdf_ty_u16bit           dRespCode = 0;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;
    icf_boolean_t        clear_call = ICF_TRUE;
    icf_boolean_t        err_resp = ICF_TRUE;
    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    
    /* Fetch the response code from the SIP message */
    if (Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                (*ppCallObj)->pUacTransaction->pSipMsg, &dRespCode,
                pErr))
    {
        /*
         * Check against 401/407 
         */
        if((401 == dRespCode || 407 == dRespCode) &&
           (ICF_SUCCESS == icf_ssa_validate_challenge(
                               ppCallObj,ICF_NULL,
                               dRespCode,p_ssa_pdb,pErr,(icf_uint8_t *)"BYE")))
        {
            err_resp = ICF_FALSE;
            icf_ssa_fetch_auth_param_list(p_ssa_pdb,
                    ppCallObj,ICF_NULL);

            /* If list is not null and auth param are configured on line
             * send bye with auth params else clear the call */
            if(ICF_NULL != p_ssa_ctxt->auth_params_list_size )
            {
                /* Fetch Line data from DBM */
                if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                            p_ssa_pdb->p_glb_pdb,
                            p_ssa_ctxt->line_id,
                            &(p_ssa_pdb->p_line_data),
                            p_ssa_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                else if((p_ssa_ctxt->p_call_ctxt)&&
                        (ICF_SUCCESS == 
                         icf_cmn_check_if_auth_params_present(
                             p_ssa_pdb->p_glb_pdb, 
                             &p_ssa_ctxt->p_call_ctxt->local_address, 
                             p_ssa_ctxt->line_id)))
                {
               	    /* Send a new BYE and no need to clear call */ 
                    clear_call = ICF_FALSE;

                    if(ICF_TRUE == ICF_IS_BIT_SET(
                        p_ssa_pdb->p_line_data->line_data_received, ICF_CFG_LINE_DATA_REALM_RECD))
                    {
                        /* We have to compare the received realm with configured one */
                        Sdf_st_authenticationParams *pAuthenticationParams = Sdf_co_null;
                        if(Sdf_co_success == sdf_listGetAt(p_ssa_ctxt->p_auth_params_list, 0, 
                            (Sdf_ty_pvoid *)&pAuthenticationParams, 
                            pErr))
                        {
                            if(Sdf_co_null != pAuthenticationParams->pRealm)
                            {
                                icf_string_st temp_unq_realm = {0,"0"};
                                temp_unq_realm.str[0]='\0';
								temp_unq_realm.str_len=0;
                                sdf_fn_uaUnq((icf_int8_t *)pAuthenticationParams->pRealm,(icf_int8_t *)temp_unq_realm.str);
                                if(0 != icf_port_strcmp(p_ssa_pdb->p_line_data->configured_realm.str, 
                                    temp_unq_realm.str))
                                {
                                    clear_call = ICF_TRUE;
                                }
                            }
                        }
                    }
                    if(clear_call == ICF_FALSE)
                    {
                        /* Fetch and store the old nonce in the ssa context */
               
                        if(ICF_FAILURE == icf_ssa_extract_nonce_from_challenge(
                                              p_ssa_pdb,
                                              dRespCode))
                        {
                            ret_val = ICF_FAILURE;
                        }
						p_ssa_pdb->p_method=(icf_uint8_t *)"BYE"; 
						icf_ssa_send_authorized_req(p_ssa_pdb);
					}
                 }
            } 
        }
        else
           {
#ifdef ICF_ERROR_ENABLE
             if(ICF_TRUE == err_resp)
               {
                 p_ssa_pdb->resp_code = dRespCode;
                 ICF_SSA_SET_ECODE(p_ssa_pdb,"BYE",ICF_ERROR_FAILURE_RESPONSE)
               }
#endif

           }      
    }
#ifdef ICF_STAT_ENABLE
    icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"BYE",dRespCode,0);
#endif
   
    if(ICF_TRUE == clear_call)
    {
#ifdef ICF_LOAD_STAT
        printf("\n~~~~~~~~~~~~~~BYE REJECTED~~~~~~~~~~~~~~~\n");
#endif
        sdf_ivk_uaAbortCall(
                *ppCallObj,
                p_ssa_ctxt,
                pErr);

        return sdf_cbk_uaCallTerminated(
                ppCallObj,pEventContext,pErr);
    }
    return ret_val;
}


/**********************************************************
 * LOCAL FUNCTIONS FOR HANDLING FINAL RESPONSE RECEVIED OF
 * VARIOUS METHODS LIKE  
 *                  REGISTER/OPTIONS/INFO/SUBSCRIBE/NOTIFY
 **********************************************************/

/*********************************************************
** FUNCTION: icf_ssa_chal_resp_transaction_Completed_for_register
**
** DESCRIPTION: This Function handles Challange response to 
**              Register
**
**********************************************************/

Sdf_ty_retVal icf_ssa_chal_resp_transaction_Completed_for_register(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit			        resp_code,
        Sdf_st_error                    *pErr)
{
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                   *p_ssa_pdb = ICF_NULL;
#ifdef  IMS_CLIENT
    SipMessage                          *p_message = ICF_NULL;
#endif
    icf_rgm_nw_resp_st               *p_rgm_msg = ICF_NULL;

    Sdf_ty_retVal ret_val = Sdf_co_success;

#ifdef ICF_IPSEC_ENABLED
	SipHeader              		        dHeader;
    SIP_S8bit              		        *pUnkHdrName = SIP_NULL;
	Sdf_ty_u32bit		   		        unknown_hdr_count =0;
	icf_uint8_t		   		        curr_count =0;
	icf_ipsec_sa_context_st	        *p_sa_context = ICF_NULL;
    SipError                            sip_err = 0;
#endif /* ICF_IPSEC_ENABLED */

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    /* Populate the internal message payload */
    p_rgm_msg = (icf_rgm_nw_resp_st *)
        p_ssa_pdb->p_internal_msg->p_msg_data;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_msg))

	p_rgm_msg->challenge_len = 0;
	p_rgm_msg->p_www_auth_challenge = ICF_NULL;
    /* Re-initialize the list in ssa ctxt */
    if (ICF_NULL != p_ssa_ctxt->p_auth_params_list)
    {
        sdf_listDeleteAll(
                p_ssa_ctxt->p_auth_params_list,
                pErr);
    }
    else
    {
        ICF_MEMGET(
                p_ssa_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(Sdf_ty_slist),
                ICF_MEM_COMMON,
                p_ssa_ctxt->p_auth_params_list,
                ICF_RET_ON_FAILURE,
                p_ssa_pdb->p_ecode,
                ret_val)
    }

    if (Sdf_co_fail == sdf_listInit(
                p_ssa_ctxt->p_auth_params_list,
                __sdf_fn_uaFreeAuthenticationParams,
                Sdf_co_false,
                pErr))
    {
        ret_val = Sdf_co_fail;
    }
    else if (Sdf_co_fail == sdf_ivk_uaGetAuthParams(
                *ppCallObj,
                pOverlapTransInfo,
                p_ssa_ctxt->p_auth_params_list,
                pErr))
    {
        if(Sdf_en_basicSchemeNotImplemented == pErr->errCode)
        {
			p_ssa_pdb->p_ecode = (unsigned short *)ICF_REGM_SSA_BASIC_AUTH_SCHEME_NOT_SUPPORTED;
			p_rgm_msg->ecode = ICF_REGM_SSA_BASIC_AUTH_SCHEME_NOT_SUPPORTED;
        }
		else
        {
			p_ssa_pdb->p_ecode = (unsigned short *)ICF_REGM_SSA_AUTH_FAILURE;
			p_rgm_msg->ecode = ICF_REGM_SSA_AUTH_FAILURE;
        }
		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CFG;
		p_ssa_pdb->common_bitmask = ICF_SSA_INTER_MODULAR_MSG;
        ret_val = Sdf_co_fail;
    }
    else
    {
        sdf_listSizeOf(
                p_ssa_ctxt->p_auth_params_list,
                &(p_ssa_ctxt->auth_params_list_size),
                pErr);

        /* 
         * If no auth params are recd from Registrar, then 
         * tell REGM to retry after sometime without auth info
         */
        if (ICF_NULL == p_ssa_ctxt->auth_params_list_size)
        {
            sdf_listDeleteAll(
                    p_ssa_ctxt->p_auth_params_list,
                    pErr);
            ICF_MEMFREE(
                    p_ssa_pdb->p_glb_pdb,
                    p_ssa_ctxt->p_auth_params_list,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    ret_val)
                p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;
        }
        else
        {
#ifdef ICF_TRACE_ENABLE
                   if(401 == resp_code)
                   {
                        ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_REG_AUTH_401)
                   }
                  else if(407 == resp_code)
                   {
                        ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_REG_AUTH_407)
                  }
#endif
            
         if (ICF_FAILURE == icf_ssa_extract_challenge(
                p_ssa_pdb,
                (icf_uint8_t)((resp_code == 401)?SipHdrTypeWwwAuthenticate:SipHdrTypeProxyAuthenticate)))
            {
                sdf_listDeleteAll(
                    p_ssa_ctxt->p_auth_params_list,
                    pErr);
                ICF_MEMFREE(
                    p_ssa_pdb->p_glb_pdb,
                    p_ssa_ctxt->p_auth_params_list,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    ret_val)
                    p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;
                
            }
            else
            {
                p_rgm_msg->ecode = 
                    ICF_RGM_SSA_AUTHENTICATION_REQ;
				p_ssa_ctxt->p_rgm_ctxt->indicator |= 
					ICF_REQ_AGAIN_AFTER_CHALLENGE;
                
                p_ssa_ctxt->bitmask |= ICF_SSA_AUTH_IN_PROGRESS;
                #ifdef ICF_IPSEC_ENABLED

				if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
					(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt->rgm_duration))
				{
                p_message = pOverlapTransInfo->pSipMsg;

                /* Since the received message has a valid challenge, need to 
				 * fetch security server header params from the received 401 */


				if(sip_getHeaderCount(p_message, SipHdrTypeUnknown, 
                    &unknown_hdr_count, &sip_err)==SipFail)
        		{
                	ret_val = Sdf_co_fail;
        		}
        		for(curr_count=0;curr_count<unknown_hdr_count;curr_count++)
        		{
					ret_val = ICF_SUCCESS;	
            		/* For loop: on unknown header list : Processing begins */
            		if (SipFail == sip_getHeaderAtIndex(p_message, 
                        SipHdrTypeUnknown, &dHeader, 
                        curr_count, &sip_err))
            		{
                    	ret_val = ICF_FAILURE;
                		continue;
            		}
					else
                    if (SipFail == sip_getNameFromUnknownHdr(&dHeader,
                        &pUnkHdrName,&sip_err))
            		{
                		/*Continue to try to fetch other unknown headers*/    
                		sip_freeSipHeader(&dHeader);
                		continue;
            		}

            		/* Check for the Service-Route header */
            		if(0 == sip_strcasecmp(pUnkHdrName,"Security-Server"))
            		{
                		/* re-assign pointer of the new SA context to the old 
						 * SA context in the REGM CTXT
						 * */
						if (ICF_NULL != 
								((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context)
						{
							((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context = 
								((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
						}
						
						((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context = 
									ICF_NULL;
												
						/* allocate memory for the new SA */
						ICF_MEMGET(
							p_ssa_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_ipsec_sa_context_st),
                            ICF_MEM_COMMON,
                        	p_sa_context,
                            ICF_RET_ON_FAILURE,
                            p_ssa_pdb->p_ecode,
                            ret_val);
						
						/* extract the security params and save in the 
						 * context. Set the SA context status to inactive */
						p_sa_context->is_active = ICF_FALSE;
						
						if (ICF_SUCCESS == 
							(ret_val = icf_ssa_parse_sec_serv_header(
                                        p_ssa_pdb,
                                        p_sa_context,
                                        *ppCallObj,
                                        dHeader)))
						{
						    ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context = 
									p_sa_context;
							p_sa_context = ICF_NULL;
						}
						else
						{
							sdf_listDeleteAll(
                    			p_ssa_ctxt->p_auth_params_list,
                    			pErr);
							
							ICF_MEMFREE(
                    			p_ssa_pdb->p_glb_pdb,
                    			p_ssa_ctxt->p_auth_params_list,
                    			ICF_MEM_COMMON,
                    			p_ssa_pdb->p_ecode,
                    			ret_val);
							/* free mem allocated for SA_CONTEXT */
                            if (p_sa_context->p_q_value)
                            {
                                ICF_MEMFREE(
                    			p_ssa_pdb->p_glb_pdb,
                    			p_sa_context->p_q_value,
                    			ICF_MEM_COMMON,
                    			p_ssa_pdb->p_ecode,
                    			ret_val);
                            }

                            if (p_sa_context->p_integrity_algo)
                            {
                                ICF_MEMFREE(
                    			p_ssa_pdb->p_glb_pdb,
                    			p_sa_context->p_integrity_algo,
                    			ICF_MEM_COMMON,
                    			p_ssa_pdb->p_ecode,
                    			ret_val);
                            }

                            if (p_sa_context->p_encryption_algo)
                            {
                                ICF_MEMFREE(
                    			p_ssa_pdb->p_glb_pdb,
                    			p_sa_context->p_encryption_algo,
                    			ICF_MEM_COMMON,
                    			p_ssa_pdb->p_ecode,
                    			ret_val);
                            }

							ICF_MEMFREE(
                    			p_ssa_pdb->p_glb_pdb,
                    			p_sa_context,
                    			ICF_MEM_COMMON,
                    			p_ssa_pdb->p_ecode,
                    			ret_val);
							
                  			p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;
							/* need to set the return success for the 
							 * processing of message in REGM */
							ret_val = ICF_SUCCESS;
						}
                		sip_freeSipHeader(&dHeader);
					} /* if ends */
				}/* for loop ends */
				}/* if(ipsec_enabled && rgm_duration) */
#endif /* ICF_IPSEC_ENABLED */
				}/* else ends */
            
            
            
        }
        
    }

    p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
    return ret_val;
}

/*********************************************************
** FUNCTION: icf_ssa_pos_resp_transaction_Completed_for_register
**
** DESCRIPTION: This Function handles Success response For 
**              Register Request Sent Out
**
**********************************************************/
Sdf_ty_retVal icf_ssa_pos_resp_transaction_Completed_for_register(

        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_st_error                    *pErr)
{
    icf_boolean_t        expires_present = ICF_FALSE;
	icf_duration_t       expires_duration = ICF_NULL;
    Sdf_ty_slist            slMatchedHeadersList;
    Sdf_ty_u32bit           size;
#ifdef IMS_CLIENT
    Sdf_ty_u32bit                unknown_hdr_count = 0,curr_count;
    SIP_S8bit                    *pUnkHdrName = SIP_NULL, *pBuffer = SIP_NULL ;
    SipHeader                    *p_serv_rte = SIP_NULL;        
    Sdf_ty_u32bit                buf_count = 0;
#endif

    Sdf_ty_u32bit                 param_count = 0, iterator = 0;
    SipHeader                    contact_hdr;        
    SipContactParam                *pContactParam = Sdf_co_null;
#ifdef IMS_CLIENT
    SipHeader				   *p_temp_asso_uri;
    Sdf_st_error                sdf_err;
	SipHeader                 dHeader;
    icf_return_t		    temp_ret_val = 0;
    icf_boolean_t		    assoc_uris_present = ICF_FALSE;
#endif

	icf_rgm_context_st		*p_rgm_context;
	SipError                    sip_err;

    icf_address_st                   temp_addr;
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                   *p_ssa_pdb = ICF_NULL;
    icf_rgm_nw_resp_st               *p_rgm_msg = ICF_NULL;
    SipMessage                          *p_message = ICF_NULL;
    
    icf_user_db_node_st              *p_node;

   /*Fix for CSR 8331321 SPR 20307*/
   Sdf_ty_u32bit                   contact_count1 = 0;
    Sdf_ty_u32bit                   index = 0;
    SipAddrSpec                     *p_addr_spec = ICF_NULL;
    SipUrl                          *p_contact_sip_url = ICF_NULL;
    icf_boolean_t                   contact_found = ICF_FALSE;
    icf_address_st					rgm_user_addr;
    icf_uint8_t					    *p_rgm_user_info = ICF_NULL;
 
    Sdf_ty_retVal ret_val = Sdf_co_success;
     /* To remove compiler warning */
     (void)ppCallObj;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
ICF_4_0_CRM_START
	/* extract the pointer to rgm_context locally */
	p_rgm_context = p_ssa_ctxt->p_rgm_ctxt;
	ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_context))
ICF_4_0_CRM_END



    p_message = pOverlapTransInfo->pSipMsg;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_message))
    /* Populate the internal message payload */
    p_rgm_msg = (icf_rgm_nw_resp_st *)
        p_ssa_pdb->p_internal_msg->p_msg_data;
    /* Fix for CSR 8331321 SPR 20307 Merged from 1-6191604 */
	icf_port_memset(
		&rgm_user_addr,
		ICF_NULL,
		ICF_PORT_SIZEOF(icf_address_st));
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_msg))
    /* Before Calling subsequent FormRequest delete previous 
     * entries of authorization and proxy-authorization headers 
     * from the persistent list. Previous entries might be 
     * there if we were challenged more than once.
     */
    sdf_ivk_uaDeleteHeaderListByCorrelation(
            &(p_ssa_ctxt->p_call_obj->slPersistentHeaders),
            "uat-authenticate",
            pErr);
    sdf_ivk_uaDeleteHeaderListByCorrelation(
            &(p_ssa_ctxt->p_call_obj->slPersistentHeaders),
            "uat-proxyauth",
            pErr);

/*CSR 1-6168806*/
    /*Also reset the AUTH bitmask now so that we donot fail registration in case
      it is challenged in further reregisters*/
    if (ICF_SSA_AUTH_IN_PROGRESS & p_ssa_ctxt->bitmask)
    {
        p_ssa_ctxt->bitmask &= ~ICF_SSA_AUTH_IN_PROGRESS;
    }

ICF_CRM_START
	
    	/* Fix for CSR 8331321 SPR 20307 Merged from CSR 1-6191604  */
        /*Initially when register response is received with multiple contact URI 
         then IPTK used to fetch expires value from the topmost contact URI and used its
         value for registration refresh. 
         Now behaviour has been modified such that now EGT will fetch the value 
         of expires header from the contact URI with which IPTK is binded*/
/* Change start */
     if (SipFail == sip_getHeaderCount(
                 pOverlapTransInfo->pSipMsg,SipHdrTypeContactNormal,
                 &contact_count1,&sip_err))
     {
         p_rgm_msg->p_rgm_context->indicator &= ~ICF_RGM_SSA_CONTACT_RECD;
     }
     else 
     {
         if (0 != contact_count1)
         {
            p_rgm_msg->p_rgm_context->indicator |= ICF_RGM_SSA_CONTACT_RECD;
         }

         for (index = 0; index < contact_count1; index++)
         {
             if (SipFail == sip_getHeaderAtIndex(pOverlapTransInfo->pSipMsg,
                 SipHdrTypeContactNormal,&contact_hdr,index, &sip_err))
             {
                 ret_val = Sdf_co_fail;
             }
             else if (SipFail == sip_getAddrSpecFromContactHdr(
                         &contact_hdr,&p_addr_spec,&sip_err))
             {
                 sip_freeSipHeader(&contact_hdr);
                 ret_val = Sdf_co_fail;
             }
             else if (SipAddrSipUri == p_addr_spec->dType)
             {
                 if (SipFail == sip_getUrlFromAddrSpec(p_addr_spec,
                             &p_contact_sip_url,&sip_err))
     {
                     sip_freeSipHeader(&contact_hdr);
                     ret_val = Sdf_co_fail;
                 }
     }
	 else
	 {
                 /*addr type is not SIP */
                 sip_freeSipHeader(&contact_hdr);
                 ret_val = Sdf_co_fail;
             }

             if (Sdf_co_fail != ret_val)
             {
                 /* Check the type and then compare accordingly
                  */
                 if (SipAddrSipUri == p_addr_spec->dType)
                 {
                     icf_string_st cfg_contact_ip = {0,{"\0"}};

					 /* Compare the user Id in req and resp */
					 if (ICF_ADDRESS_PLAN_SIP_URI ==
						 p_rgm_context->rgm_user_address.addr_type)
					 {
						icf_port_memcpy(
							&rgm_user_addr,
							&(p_rgm_context->rgm_user_address),
							ICF_PORT_SIZEOF(icf_address_st));
						
						p_rgm_user_info = (icf_uint8_t *)icf_port_strtok(
											(icf_int8_t *)rgm_user_addr.addr_val.str,
											"@");
					 }

#ifdef ICF_NAT_RPORT_SUPPORT
                     if (ICF_SUCCESS != icf_ssa_convert_to_IP_addr(
                                 &p_ssa_pdb->p_glb_cfg_data->
                                 rport_config_data.contact_ip_addr, 
                                 (icf_uint8_t*)&(cfg_contact_ip.str)))
                     {
                         ret_val = Sdf_co_fail;
                     }
#else
                     if (ICF_SUCCESS != icf_ssa_convert_to_IP_addr(
                                 &p_ssa_pdb->p_glb_cfg_data->
                                 self_ip_address, 
                                 (icf_uint8_t*)&(cfg_contact_ip.str)))
                     {
                         ret_val = Sdf_co_fail;
                     }
#endif
					 /* Check if IP match in req and resp */
                     else if ((ICF_NULL != p_contact_sip_url->pHost) && 
                             (0 == icf_port_strcmp(cfg_contact_ip.str,
                                                   p_contact_sip_url->pHost)))
                     {
					    /* Check if user id match in req and resp */
					 	if ((ICF_NULL != p_rgm_user_info) &&
							  (ICF_NULL != p_contact_sip_url->pUser) &&
							  (ICF_NULL != icf_port_strcmp(
												p_rgm_user_info,
												p_contact_sip_url->pUser)))
					 	{
							ICF_PRINT(("\n[SSA]: User IDs do not match in Contact response\n"));
					 	}
						else if ((ICF_NULL != p_rgm_user_info) &&
								 (ICF_NULL == p_contact_sip_url->pUser))
						{
							ICF_PRINT(("\n[SSA]: User IDs do not match in Contact response\n"));
						}
						else if ((ICF_NULL == p_rgm_user_info) &&
								 (ICF_NULL != p_contact_sip_url->pUser))
						{
							ICF_PRINT(("\n[SSA]: User IDs do not match in Contact response\n"));
						}
					    /* Check if port match in req and resp */
#ifdef ICF_NAT_RPORT_SUPPORT
						/* Fix for CSR 1-8516341 */
						/* Added null check for dPort pointer to avoid 
						 * crash when 200 OK for REGISTER does not contain
						 * port in Contact header
						 */
						else if ((Sdf_co_null != p_contact_sip_url->dPort) &&
                                 (p_ssa_pdb->p_glb_cfg_data->rport_config_data.contact_ip_addr.port_num == 
									*p_contact_sip_url->dPort))
                         {
                             /*set flag to indicate that contact is found*/
                             contact_found = ICF_TRUE;
                             sip_freeSipUrl(p_contact_sip_url);
                             sip_freeSipAddrSpec(p_addr_spec);
                             break;
                         }
#else
						/* Fix for CSR 1-8516341 */
						/* Added null check for dPort pointer to avoid 
						 * crash when 200 OK for REGISTER does not contain
						 * port in Contact header
						 */
						else if ((Sdf_co_null != p_contact_sip_url->dPort) &&
                                  (p_ssa_pdb->p_glb_cfg_data->self_ip_address.port_num == 
									*p_contact_sip_url->dPort))
                         {
                             /*set flag to indicate that contact is found*/
                             contact_found = ICF_TRUE;
                             sip_freeSipUrl(p_contact_sip_url);
                             sip_freeSipAddrSpec(p_addr_spec);
                             break;
                         }
#endif
                     }
                     sip_freeSipUrl(p_contact_sip_url); 
                 }
          sip_freeSipHeader(&contact_hdr);
                 sip_freeSipAddrSpec(p_addr_spec);
             }/* if (Sdf_co_fail != ret */
             else
             {
                 /* Some failure */
                 sip_freeSipAddrSpec(p_addr_spec);
                 break;
	 }
         }/* End loop */
     }/* else */
/* Change end */

ICF_CRM_END
     /* Clear the previous contact uris and p-associated and contact uris from the line data */
     /*for each line in the rgm context */
     /* Change for CSR-1-5318850 start */
     /* Code removed from here to give priority to expires of Conatct instead of Expires header */
        
     /* Fix for CSR 8331321 SPR 20307 */   
     if ((ret_val != Sdf_co_fail) && (ICF_TRUE == contact_found))
        {
         if(SipFail == sip_getContactParamsCountFromContactHdr(
                    &contact_hdr, &param_count, &sip_err))
        {
            ICF_PRINT(((icf_uint8_t *)"No Params in Contact header present \n"));
            sip_freeSipHeader(&contact_hdr);
        }

        else 
        {
            for(iterator = 0; iterator < param_count; iterator++)
            {
                if(SipFail == sip_getContactParamAtIndexFromContactHdr(
                            &contact_hdr, &pContactParam, iterator, &sip_err))
                {
                    ICF_PRINT(((icf_uint8_t *)"No Param in Contact header list present \n"));
                }
                else if( SipCParamExpires != pContactParam->dType)
                {
                    sip_freeSipContactParam(pContactParam);
                    continue;
                }
                else
                {
                    SipExpiresStruct* p_exp = pContactParam->u.pExpire;
                    expires_duration =  p_exp->u.dSec;
                    expires_present = ICF_TRUE;
                    sip_freeSipContactParam(pContactParam);
                     /*  CSR 1-5473446 Merging */
                     break;
                    /*    sip_freeSipExpiresStruct(p_exp);*/
                }
            }
            sip_freeSipHeader(&contact_hdr);
        }
     }
     else
     {
         /*Case of De-register response when 200 OK may not contain contact value
          *treat it as an success response*/
          ret_val = Sdf_co_success;         
     }
        
        /* if Contact header field does not contain the Expire parameter
         * and Expires header exists then get the expire duration from 
         * Expires header field*/
         if (ICF_TRUE != expires_present)
         {


       /* Get the Expires header value from the recd msg */
       /* Currently we are assuming that the registered duration 
        * will be only in time format */

      /* Extract registered duration from sip message */
       if (Sdf_co_fail == sdf_ivk_uaGetHeaderListByType(
                pOverlapTransInfo->slUnprocessedHeaders,
                SipHdrTypeExpiresSec,
                &slMatchedHeadersList,
                pErr))
       {
           /*Klocwork Warning removal*/
            sip_freeSipContactParam(pContactParam);
            return Sdf_co_fail;
       }
       if (Sdf_co_fail == sdf_listSizeOf(
                &slMatchedHeadersList,&size, pErr))

       {
            /*Klocwork Warning removal*/
            sip_freeSipContactParam(pContactParam);
           sdf_listDeleteAll(&slMatchedHeadersList,pErr);
           return Sdf_co_fail;
       }

       if(ICF_NULL != size)
       {
        SipHeader           *pExpiresHeader = Sdf_co_null;
        Sdf_st_headerList   *pExpiresHeaderList = Sdf_co_null;

        expires_present = ICF_TRUE;

        /* Get HeaderList at index 0 */
        if (Sdf_co_fail == sdf_listGetAt(
                    &slMatchedHeadersList,
                    0,
                    (Sdf_ty_pvoid *)&pExpiresHeaderList,
                    pErr))
        {
            /*Klocwork Warning removal*/
            sip_freeSipContactParam(pContactParam);
            sdf_listDeleteAll(&slMatchedHeadersList,pErr);
            return Sdf_co_fail;
        }

        if (Sdf_co_fail == sdf_listGetAt(
                    &(pExpiresHeaderList)->slHeaders,
                    0,
                    (Sdf_ty_pvoid *)&pExpiresHeader,
                    pErr))
        {
            /*Klocwork Warning removal*/
            sip_freeSipContactParam(pContactParam);
            sdf_listDeleteAll(&slMatchedHeadersList,pErr);
            return Sdf_co_fail;
        }

        expires_duration = (((SipExpiresHeader *)(
                        pExpiresHeader->pHeader))->u).dSec;
		expires_present = ICF_TRUE;

        sdf_listDeleteAll(&slMatchedHeadersList,pErr);
    }/* size != 0 */
   }
    /* Change for CSR-1-5318850 end */

		if ((ICF_NULL == p_rgm_context->rgm_duration) &&
				(ICF_NULL == expires_duration))
		{
			p_rgm_msg->expires_duration = ICF_NULL;
			p_rgm_msg->status = ICF_RGM_SSA_NW_SUCCESS;
		}
		else if ((ICF_NULL == p_rgm_context->rgm_duration) &&
				(ICF_NULL != expires_duration))
		{
			p_rgm_msg->expires_duration = ICF_NULL;
                        /*CSR 1-6168806 Even if the n/w 200OK contains
                          a non-zero duration we should treat the dereg
                          succssful*/
			p_rgm_msg->status = ICF_RGM_SSA_NW_SUCCESS;
		}
		else if ((ICF_NULL != p_rgm_context->rgm_duration) &&
				(ICF_NULL == expires_duration))
		{
			p_rgm_msg->expires_duration = ICF_NULL;
			p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
			p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_NO_REATTEMPT;
		}
/* Start: Multiple Public Users ICF5.0*/
        /* 
     * If expires duration received from the network is lesser than the one sent
     * in request consider the one sent in request for running the refresh timer
     */
    else if ((expires_duration*1000) > p_rgm_context->rgm_duration)
		{
      p_rgm_msg->expires_duration = p_rgm_context->rgm_duration;
			p_rgm_msg->status = ICF_RGM_SSA_NW_SUCCESS;
#if 0
			p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
			p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_TIMEOUT;
			p_rgm_context->rgm_duration = (expires_duration*1000);
ICF_CHECK_IMS_END
#endif
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
			p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;
ICF_CHECK_IMS_END
#endif
		}
/* End: Multiple Public Users ICF5.0*/
		else
		{/* store the expires in rgm_nw_mesg */

			/* Because the duration from network is in seconds and
			 * internally duration is stored as milliseconds */
			p_rgm_msg->expires_duration = (expires_duration * 1000);
			p_rgm_msg->status = ICF_RGM_SSA_NW_SUCCESS;

		}		

    /*Extract Service-Route And Associated Uris 
     * header if present. Since it is not 
     * defined in decoder, it is to be parsed as unknown header*/
    /*Check if the message has any headers that are not understood */
    

	/* Clear the contact list anc the p-assoc-uri list for user */
    for(iterator = p_rgm_context->num_of_lines;iterator--;)
    {
        icf_dbm_clear_public_uri_node_for_line (p_ssa_pdb->p_glb_pdb,
            p_rgm_context->rgm_list_of_lines[iterator],
            &(p_rgm_context->rgm_user_address),
            ICF_USER_DB_USE_ASSOCIATED_URIS | ICF_USER_DB_USE_CONTACT_URIS,
            p_ssa_pdb->p_ecode);
    }

    if(0 != p_ssa_ctxt->p_rgm_ctxt->rgm_duration)
    {
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)

        if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt->p_srv_rte)
        {
            sdf_listDeleteAll((Sdf_ty_slist*)
                    p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                    &sdf_err);
            sdf_listFree((Sdf_ty_slist*)
                    p_ssa_ctxt->p_rgm_ctxt->p_srv_rte, &sdf_err);
        }
        if(sip_getHeaderCount(p_message, SipHdrTypeUnknown, 
                    &unknown_hdr_count, &sip_err)==SipFail)
        {
            /*ICF_PRINT(("\n[SSA]:Failure sip_getHeaderCount Failed"));*/
            ret_val = Sdf_co_fail;
        }
        for(curr_count=0;curr_count<unknown_hdr_count;curr_count++)
        {
            /* For loop: on unknown header list : Processing begins */
            if(SipFail == sip_getHeaderAtIndex(p_message, 
                        SipHdrTypeUnknown, &dHeader, 
                        curr_count, &sip_err))
            {
                /*ICF_PRINT(("\n[SSA]:Failure \
                  sip_getHeaderAtIndex Failed"));
                 */
                ret_val = Sdf_co_fail;
                break;
            }

            /*ICF_PRINT(("\n[SSA]:Parsing Unknown Header at index : %d",curr_count-1));*/
            if(SipFail == sip_getNameFromUnknownHdr(&dHeader,
                        &pUnkHdrName,&sip_err))
            {
                /*Continue to try to fetch other unknown headers*/    
                sip_freeSipHeader(&dHeader);
                continue;
            }

            /*ICF_PRINT(("\n[SSA]:Got unknown header : %s",pUnkHdrName));*/

            /* Check for the Service-Route header */
            if(0 == sip_strcasecmp(pUnkHdrName,"Service-Route"))

            {
                icf_return_t temp_ret_val;
                icf_boolean_t is_srv_rte_allocated = ICF_FALSE;
                if(ICF_NULL == p_ssa_ctxt->p_rgm_ctxt->p_srv_rte)
                {
                    /* Take Memory For New Srv ROUTE LIST 
                     *It should allocated Once and freed while 
                     * deleting the Register*/
                    ICF_MEMGET(
                            p_ssa_pdb->p_glb_pdb,
                            ICF_PORT_SIZEOF(Sdf_ty_slist),
                            ICF_MEM_COMMON,
                            p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                            ICF_DONOT_RET_ON_FAILURE,
                            p_ssa_pdb->p_ecode,
                            temp_ret_val);

                    if(ICF_FAILURE != temp_ret_val)

                    {
                        is_srv_rte_allocated = ICF_TRUE;
                    }
                    else

                    {
                        sip_freeSipHeader(&dHeader);
                        break;
                    }

                }

                if(Sdf_co_fail == ret_val)
                {
                    sip_freeSipHeader(&dHeader);
                    break;
                }                        

                /*ICF_PRINT(("\n[SSA]:Found Service-Route Header"));*/
                /*Initialize the list that will contain the 
                  parsed headers */
                else if(is_srv_rte_allocated) 
                {
                    if(SipFail == sdf_listInit((Sdf_ty_slist*)
                                p_ssa_ctxt->p_rgm_ctxt->p_srv_rte, 
                                headerListFreeFunction, Sdf_co_false, &sdf_err))
                    {
                        /*ICF_PRINT(("\n[SSA]:Failure sip_listInit Failed "));*/
                        ret_val = Sdf_co_fail;
                    }
                }
                if(Sdf_co_fail == ret_val)
                {

                }
                else if (SipFail ==sip_initSipHeader(&p_serv_rte, 
                            SipHdrTypeRoute, &sip_err))
                {
                    /*ICF_PRINT(("\n[SSA]: sip_initSipHeader \
                      failed));
                     */
                    ret_val = Sdf_co_fail;
                }
                else
                {
                    buf_count=icf_port_strlen((
                                (SipUnknownHeader *)
#ifdef ICF_WITH_MUATK_2_2
                                (dHeader.pHeader))->pStr2);
                    pBuffer = (((SipUnknownHeader *)
                                (dHeader.pHeader))->pStr2);
#else
                                (dHeader.pHeader))->pBody);
                    pBuffer = (((SipUnknownHeader *)
                                (dHeader.pHeader))->pBody);
#endif

                    /* parse the Service-Route header as per Route 
                       header*/
                    if(SipFail == sip_parseFromHeader(
                                pBuffer,pBuffer+buf_count-1,
                                p_serv_rte, SipHdrTypeRoute,
                                &sip_err))
                    {
                        ret_val = Sdf_co_fail;

                    }
                    else

                    {
                        if(SipFail == sdf_listAppend(
                                    (Sdf_ty_slist*)
                                    p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                                    p_serv_rte, &sdf_err))
                        {
                            ret_val = Sdf_co_fail;
                        }    
                    }
                }
                if(Sdf_co_fail == ret_val)
                {
                    sip_freeSipHeader(&dHeader);
                    if(ICF_NULL != p_serv_rte)
                    {
                        sip_freeSipHeader(p_serv_rte);
                        ICF_MEMFREE(
                                p_ssa_pdb->p_glb_pdb,
                                p_serv_rte,
                                ICF_MEM_COMMON,
                                p_ssa_pdb->p_ecode,
                                temp_ret_val)

                    }
                    if(ICF_TRUE == is_srv_rte_allocated)
                    {
                        icf_return_t temp_ret_val;
                        sip_freeSipHeader(p_serv_rte);
                        ICF_MEMFREE(
                                p_ssa_pdb->p_glb_pdb,
                                p_serv_rte,
                                ICF_MEM_COMMON,
                                p_ssa_pdb->p_ecode,
                                temp_ret_val)
                    }
                    break;
                }

            }/* Service-Route handling ends here */
ICF_CRM_START
    /* Check for P-Associated-Uri Header */
            else if (0 == sip_strcasecmp(pUnkHdrName,"P-Associated-URI"))
            {
				assoc_uris_present = ICF_TRUE;

                /* P-Associated-URI header found: Processing Begins */
#ifdef ICF_WITH_MUATK_2_2
                pBuffer = (((SipUnknownHeader *)
                            (dHeader.pHeader))->pStr2);
#else
                pBuffer = (((SipUnknownHeader *)
                            (dHeader.pHeader))->pBody);
#endif
				
				if(ICF_NULL == pBuffer)
				{
					/*Continue to try to fetch other unknown headers*/    
					sip_freeSipHeader(&dHeader);
					continue;
				}

				buf_count=icf_port_strlen(pBuffer);
				if (SipFail == sip_initSipHeader(&p_temp_asso_uri, 
                            SipHdrTypeFrom, &sip_err))
                {
                    ret_val = Sdf_co_fail;
                    sip_freeSipHeader(&dHeader);
                    break;
                }
                /* parse the P-Associated-URI header as per From 
                   header*/
                else if(SipFail == sip_parseFromHeader(
                            pBuffer,pBuffer+buf_count-1,
                            (SipHeader *)(p_temp_asso_uri), 
                            SipHdrTypeFrom,
                            &sip_err))
                {
                    
                    sip_freeSipHeader(p_temp_asso_uri);
                    sip_freeSipHeader(&dHeader);
                    ret_val = Sdf_co_fail;
                    break; 
                }
                else
                {
/* Start: Multiple Public Users ICF5.0*/
					/* Header correctly parsed:Processing Begins
                     * The addr_spec can be used to put the
                     * uri into the rgm_context */


					if(ICF_NULL != p_temp_asso_uri)
					{
						temp_ret_val = icf_ssa_populate_address_from_sip_addr_spec(
								(icf_ssa_pdb_st *)(p_ssa_ctxt->p_ssa_pdb), 
								(SipAddrSpec *)(((SipFromHeader *)
								 (p_temp_asso_uri->pHeader))->pAddrSpec),
								&temp_addr);
						
						if (ICF_SUCCESS == temp_ret_val)
						{
                        
							/* for each line in rgm context */
							for(iterator = p_rgm_context->num_of_lines;iterator--;)
							{
								/* Make sure that the case when no assoc-uris are recieved in the 
								   response, the temp public id is added the the assoc list */
								/* get corresponding user db node */
								if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
									p_ssa_pdb->p_glb_pdb,
									p_rgm_context->rgm_list_of_lines[iterator],
									&(p_rgm_context->rgm_user_address),
									ICF_USER_DB_USE_PUBLIC_URIS,
									&p_node,
									p_ssa_pdb->p_ecode))
								{
									icf_dbm_append_uri_to_address_list(
									   p_ssa_pdb->p_glb_pdb,
									   &temp_addr,
									   &(p_node->p_assoc_uri_list),
									   p_ssa_pdb->p_ecode);
								}
							}
						}
					}
					else /* case when empty P-assoc URI header is recieved */
					{
						/* Do nothing */					
					}
					
/* End: Multiple Public Users ICF5.0*/	
                    sip_freeSipHeader(p_temp_asso_uri);
                    ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_temp_asso_uri,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            temp_ret_val)

                }/* Header correctly parsed: Processing Ends */

            }/* P-Associated-URI header found: Processing Ends */
ICF_CRM_END
            sip_freeSipHeader(&dHeader);
        }/* For loop: on unknown header list : Processing Ends */

		/* if assoc uris were not recieved in the resposne then we set the public user as
		   unbarred by storing it as the first assoc uri */
		   if (ICF_FALSE == assoc_uris_present)
		   {
                for(iterator = p_rgm_context->num_of_lines;iterator--;)
                {
                    if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
                        p_ssa_pdb->p_glb_pdb,
                        p_rgm_context->rgm_list_of_lines[iterator],
                        &(p_rgm_context->rgm_user_address),
                        ICF_USER_DB_USE_PUBLIC_URIS,
                        &p_node,
                        p_ssa_pdb->p_ecode))
                    {
                        icf_dbm_append_uri_to_address_list(
                           p_ssa_pdb->p_glb_pdb,
                           &(p_rgm_context->rgm_user_address),
                           &(p_node->p_assoc_uri_list),
                           p_ssa_pdb->p_ecode);
                    }
                }		   
		   
		   }
ICF_CHECK_IMS_END
#endif
        /*Code for getting the Contact Header string to be
          added in the user address list in line data*/
        if (ICF_FAILURE != ret_val )
        {
            Sdf_ty_u32bit				contact_count = ICF_NULL,
                                        index = ICF_NULL; 
             
            /* There can be multiple hdrs for that type */
            if (SipFail == sip_getHeaderCount(p_message, 
                 SipHdrTypeContactNormal, &contact_count,
                 &sip_err))
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failed to get Header Count "));
                ret_val = ICF_FAILURE;
            }
            else
            {
                for(index = 0; index < contact_count; index++)
                {
                    SipHeader   contact_header;
                    icf_port_memset(&contact_header,'\0',ICF_PORT_SIZEOF(contact_header));
                     
                    if(SipFail == sip_getHeaderAtIndex(p_message, 
                         SipHdrTypeContactNormal, &contact_header,index, &sip_err))
                    {
				         return ICF_FAILURE;
                    }
                    else
                    {
/* Start: Multiple Public Users ICF5.0*/
                        SipAddrSpec *p_sip_addr_spec =
                                ((SipContactHeader*)(contact_header.pHeader))->pAddrSpec;

						if ( ICF_FAILURE == 
							 icf_ssa_populate_address_from_sip_addr_spec(p_ssa_pdb,
						 ((SipContactHeader*)(contact_header.pHeader))->pAddrSpec, 
						 &(temp_addr)))
						{
							sip_freeSipHeader(&contact_hdr);
							return ICF_FAILURE;
						}
                        else
                        {
                            if(p_sip_addr_spec->dType == SipAddrSipUri) 
                            {
                                SipUrl *pSipUrl = p_sip_addr_spec->u.pSipUrl;
                                if ( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                    p_ssa_pdb->p_glb_cfg_data->self_ip_address.addr.addr_type)
                                {
                                    if(0 != icf_port_strcmp(
                                        p_ssa_pdb->p_glb_cfg_data->self_ip_address.addr.addr.domain.str,
                                        (icf_uint8_t *)pSipUrl->pHost))
                                    {
                                       /* This contact is not meant for ICF
                                        * So dont store the contact just ignore it
                                        */
                                        ICF_PRINT(((icf_uint8_t *)"[SSA] Contact %s ignored:host_name not with self_ip\n",\
                                                   temp_addr.addr_val.str));
                                        sip_freeSipHeader(&contact_header);
                                        continue;
                                        
                                    }
                                }
                                else/*Assumption that it will be IPV4*/
                                {
                                    icf_uint8_t remote_ip[32]={0};
                                    
                                    if ( ICF_SUCCESS == icf_ssa_convert_to_IP_addr(
                                        &(p_ssa_pdb->p_glb_cfg_data->self_ip_address), remote_ip))
                                    {
                                        if(0 != icf_port_strcmp(
                                            remote_ip,
                                            (icf_uint8_t *)pSipUrl->pHost))
                                        {
                                            /* This contact is not meant for ICF
                                             * So dont store the contact just ignore it
                                             */
                                            ICF_PRINT(((icf_uint8_t *)"[SSA] Contact %s ignored:host_name not match with self_ip\n",
                                                temp_addr.addr_val.str));
                                            sip_freeSipHeader(&contact_header);
                                            continue;
                                            
                                        }

                                    }
                                }
                            }
                                /* for each line in rgm context */
                            for(iterator = p_rgm_context->num_of_lines;iterator--;)
                            {
                                /* get corresponding user db node */
                                if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
                                    p_ssa_pdb->p_glb_pdb,
                                    p_rgm_context->rgm_list_of_lines[iterator],
                                    &(p_rgm_context->rgm_user_address),
                                    ICF_USER_DB_USE_PUBLIC_URIS,
                                    &p_node,
                                    p_ssa_pdb->p_ecode))
                                {
                                    icf_dbm_append_uri_to_address_list(
                                       p_ssa_pdb->p_glb_pdb,
                                       &temp_addr,
                                       &(p_node->p_contact_uri_list),
                                       p_ssa_pdb->p_ecode);
                                }
                            }
                        
                        }
                     }
                     sip_freeSipHeader(&contact_header);
/* End: Multiple Public Users ICF5.0*/
                 }
             }
        }
    }/* check for if only valid expire value */
    return ret_val;
}

/*********************************************************
** FUNCTION: icf_ssa_transaction_Completed_for_register
**
** DESCRIPTION: This Function handles All response received for 
**              Register Request Sent Out
**
**********************************************************/

Sdf_ty_retVal icf_ssa_transaction_Completed_for_register(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr)
{

    Sdf_ty_u32bit                unknown_hdr_count = 0;

    Sdf_ty_u32bit                curr_count = 0;

    SIP_S8bit                    *pUnkHdrName = SIP_NULL, *pBuffer = SIP_NULL ;
    Sdf_ty_u32bit                buf_count = 0;
    
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                   *p_ssa_pdb = ICF_NULL;
    icf_rgm_nw_resp_st               *p_rgm_msg = ICF_NULL;
    SipMessage                          *p_message = ICF_NULL;
    icf_boolean_t			    err_resp = ICF_TRUE;
    SipError                    sip_err;
    SipHeader                    dHeader;
    Sdf_ty_retVal               ret_val = Sdf_co_success;
    SipCommonHeader              *p_exp = ICF_NULL;
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_rport_config_data_st            *p_rport_config_data = ICF_NULL;
    icf_line_data_st  		*p_line_data = ICF_NULL;
#endif

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);



    /* Populate the internal message header */
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_REG_RESP;
    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
        ICF_PORT_SIZEOF(icf_rgm_nw_resp_st);

    /* Populate the internal message payload */
    p_rgm_msg = (icf_rgm_nw_resp_st *)
        p_ssa_pdb->p_internal_msg->p_msg_data;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_msg))

    /* RN*/
    p_rgm_msg->p_rgm_context = 0;

    p_rgm_msg->p_rgm_context = p_ssa_ctxt->p_rgm_ctxt;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
            (ICF_NULL != p_rgm_msg->p_rgm_context))
    p_message = pOverlapTransInfo->pSipMsg;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_message))

    /* RN*/

    p_rgm_msg->challenge_len = 0;
	p_rgm_msg->p_www_auth_challenge = ICF_NULL;

    switch (resp_code)
    {
        case 200:
#ifdef ICF_NAT_RPORT_SUPPORT


    if (ICF_CFG_SELF_ADDR_PER_LINE ==
            p_ssa_pdb->p_glb_cfg_data->self_addr_per_line)
    {
          if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_ssa_pdb->p_glb_pdb,  p_ssa_ctxt->line_id,
                    &p_line_data, p_ssa_pdb->p_ecode))
          {
               ret_val = ICF_FAILURE;
          }
          else
          {
               p_rport_config_data = p_line_data->p_rport_config_data;
        }
    
    }
    else
    {
        p_rport_config_data = &(p_ssa_pdb->p_glb_cfg_data->rport_config_data);
    }

    if (ICF_NULL == p_rport_config_data)
    {
       return ICF_FAILURE;
    } 

          if(p_rport_config_data->reg_id ==\
                    p_ssa_ctxt->p_rgm_ctxt->reg_id)
            {
                ret_val = icf_ssa_handle_register_response_for_rport(p_ssa_pdb);
            }
#endif
            if(ICF_SUCCESS == ret_val)
            {
                ret_val = icf_ssa_pos_resp_transaction_Completed_for_register(
                        ppCallObj,pEventContext,pOverlapTransInfo,pErr);
                err_resp = ICF_FALSE;
            }
            else
            {
                ret_val = Sdf_co_fail;
            }
           break;

        case 302:
		case 491:
			p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
            p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;
 #ifdef ICF_LOAD_STAT
               p_ssa_pdb->p_glb_pdb->load_data.register_failure++;
#endif
            break;

        case 400:
        case 500:
        case 502:
        case 503:
			/* Followed in ICF but changed in ICF as for
			 * these response codes, the REATTEMPT is not to be done 
			 * and warning header will be passed to Application */

			 /* p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
			    p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;
                break;*/
        

		/* for these failure responses, Registration failure is conveyed
		   to the application. In case warning header is present, it is
		   delivered to the application. 
		   Note: to reattempt in this case is application purrogative 
		   */
           

        case 403:
        case 404:
        case 481:
			{
				/* Handling 403 : Begins */


				p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;

				p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_NO_REATTEMPT;

                /*Check if the message has any headers that are not understood */
                if(sip_getHeaderCount(p_message, SipHdrTypeUnknown, 
                            &unknown_hdr_count, &sip_err)==SipFail)
                {
                    /*ICF_PRINT(("\n[SSA]:Failure sip_getHeaderCount Failed"));*/
                    ret_val = ICF_FAILURE;
                    break;
                }

                /*ICF_PRINT(("\n[SSA]:Unknown headers count in 423 Rsp for \
                  Register is %d",unknown_hdr_count));*/

                for(curr_count=0;curr_count<unknown_hdr_count;curr_count++)
                {/* For loop: on unknown header list : Processing begins */
                    if(SipFail == sip_getHeaderAtIndex(p_message, 
                                SipHdrTypeUnknown, &dHeader, 
                                curr_count, &sip_err))
                    {
                        /*ICF_PRINT(("\n[SSA]:Failure \
                          sip_getHeaderAtIndex Failed"));
                         */
                        ret_val = ICF_FAILURE;
                        break;
                    }

                    /*ICF_PRINT(("\n[SSA]:Parsing Unknown Header at index : %d",curr_count-1));*/
                    if(SipFail == sip_getNameFromUnknownHdr(&dHeader,
                                &pUnkHdrName,&sip_err))
                    {
                        /*Continue to try to fetch other unknown headers*/    
                        sip_freeSipHeader(&dHeader);
                        continue;
                    }
                    /* Check for the Warning header */
                    if(0 == sip_strcasecmp(pUnkHdrName,"Warning"))
                    {    
                        /* Warning header found: Processing Begins */
#ifdef ICF_WITH_MUATK_2_2
                        buf_count=icf_port_strlen((icf_uint8_t *)(
							(SipUnknownHeader *)
							(dHeader.pHeader))->pStr2);
                        pBuffer = (((SipUnknownHeader *)
							(dHeader.pHeader))->pStr2);
						

#else
                        buf_count=icf_port_strlen((
							(SipUnknownHeader *)
							(dHeader.pHeader))->pBody);            
                        pBuffer = (((SipUnknownHeader *)
							(dHeader.pHeader))->pBody);
#endif
						pBuffer[buf_count] = '\0';
						/* Terminate the value of warning with null char*/
						
						icf_port_strcpy(p_rgm_msg->p_rgm_context->
							warning_hdr_value.str,						
							(icf_uint8_t *)pBuffer);
						p_rgm_msg->p_rgm_context->warning_hdr_value.str_len =
							(icf_uint16_t)icf_port_strlen(
							p_rgm_msg->p_rgm_context->
							warning_hdr_value.str);
						
						/* indicate the REGM that warning header is present */
						p_rgm_msg->p_rgm_context->indicator |=
							ICF_RGM_SSA_WARNING_FILLED;
						
						sip_freeSipHeader(&dHeader);
						
					}/* Warning header found: processing ends */
					else
					{
						sip_freeSipHeader(&dHeader);
					}
					
				}/* For loop on unknown headers : ends here */

				
			}/* Handling 403 : Ends */

            break;

        case 401:
        case 407:
            /*  if(ICF_SSA_AUTH_IN_PROGRESS & p_ssa_ctxt->bitmask)*/
            /* if we receive the 407 with stale=true or if the bit 
            ICF_IGNORE_STALE_PARAM_IN_CHALLENGE is set,then we will
            honour it even if ICF_SSA_AUTH_IN_PROGRESS is true */

            if ((ICF_SSA_AUTH_IN_PROGRESS & p_ssa_ctxt->bitmask) &&
             (ICF_FAILURE == icf_ssa_validate_challenge( ppCallObj, pOverlapTransInfo,
             resp_code, p_ssa_pdb,pErr,(icf_uint8_t *)"REGISTER")))
            {
                p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;

                p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_NO_REATTEMPT;
            }
            else
            {
                ret_val = icf_ssa_chal_resp_transaction_Completed_for_register(
                    ppCallObj,pEventContext,pOverlapTransInfo, resp_code, pErr);
                err_resp = ICF_FALSE;
            } 
            break;

        case 423:
            {
                p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
                p_rgm_msg->ecode = ICF_RGM_SSA_DURATION_SHORT;
 
                if(ICF_SUCCESS == icf_ssa_isHeaderPresent(
                                       p_message,
                                       SipHdrTypeMinExpires,
                                       pErr))
                {
                     if(SipFail == sip_getHeaderAtIndex(p_message,
                                     SipHdrTypeMinExpires, &dHeader,
                                     curr_count, &sip_err))

                      {
                          ret_val = ICF_FAILURE;
                      } 
                      else
                      {

                         p_exp = (SipCommonHeader *)(dHeader.pHeader);
                         p_rgm_msg->expires_duration = p_exp->dIntVar1;
                         p_rgm_msg->p_rgm_context->indicator |= ICF_RGM_SSA_DURATION_FILLED;
                         sip_freeSipHeader(&dHeader);
                      }
                }       
#if 0
                /*Check if the message has any headers that are not understood */
                if(sip_getHeaderCount(p_message, SipHdrTypeUnknown, 
                            &unknown_hdr_count, &sip_err)==SipFail)
                {
                    /*ICF_PRINT(("\n[SSA]:Failure sip_getHeaderCount Failed"));*/
                    ret_val = ICF_FAILURE;
                    break;
                }

                /*ICF_PRINT(("\n[SSA]:Unknown headers count in 423 Rsp for \
                  Register is %d",unknown_hdr_count));*/


                for(curr_count=0;curr_count<unknown_hdr_count;curr_count++)
                {/* For loop: on unknown header list : Processing begins */
                    icf_return_t temp_ret_val=ICF_FAILURE;
                    if(SipFail == sip_getHeaderAtIndex(p_message, 
                                SipHdrTypeUnknown, &dHeader, 
                                curr_count, &sip_err))
                    {
                        /*ICF_PRINT(("\n[SSA]:Failure \
                          sip_getHeaderAtIndex Failed"));
                         */
                        ret_val = ICF_FAILURE;
                        break;
                    }

                    /*ICF_PRINT(("\n[SSA]:Parsing Unknown Header at index : %d",curr_count-1));*/
                    if(SipFail == sip_getNameFromUnknownHdr(&dHeader,
                                &pUnkHdrName,&sip_err))
                    {
                        /*Continue to try to fetch other unknown headers*/    
                        sip_freeSipHeader(&dHeader);
                        continue;
                    }
                    /* Check for the Min-Expires header */
                    if(0 == sip_strcasecmp(pUnkHdrName,"Min-Expires"))
                    {    
                        /* Min-Expires header found: Processing Begins */
#ifdef ICF_WITH_MUATK_2_2
                        buf_count=icf_port_strlen((
                                    (SipUnknownHeader *)
                                    (dHeader.pHeader))->pStr2);            
                        pBuffer = (((SipUnknownHeader *)
                                    (dHeader.pHeader))->pStr2);
#else
                        buf_count=icf_port_strlen((
                                    (SipUnknownHeader *)
                                    (dHeader.pHeader))->pBody);            
                        pBuffer = (((SipUnknownHeader *)
                                    (dHeader.pHeader))->pBody);
#endif
                        /* parse the Min-Expires header as per ExpiresSec
                         * header: Here we are assuming that only seconds are
                         * received in Min-Expires field*/

                        /* RN - Bug fixed*/
                        if (SipFail == (sip_initSipHeader(&p_min_expires, 
                                        SipHdrTypeExpiresSec,&sip_err)))
                        {
                            sip_freeSipHeader(&dHeader);
                            ICF_PRINT(("\nFailed to initialize Event Header"));
                            ret_val = ICF_FAILURE ;
                            break;
                        }
                        else if (ICF_NULL == p_min_expires)
                        {
                            sip_freeSipHeader(&dHeader);
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        else if(SipFail == sip_parseExpiresHeader(
                                    pBuffer,pBuffer+buf_count-1,
                                    (SipHeader *)(p_min_expires), 
                                    SipHdrTypeExpiresSec,
                                    &sip_err))
                        {
                            sip_freeSipHeader(&dHeader);
                            sip_freeSipHeader(p_min_expires);
                            ICF_MEMFREE(
                                    p_ssa_pdb->p_glb_pdb,
                                    p_min_expires,
                                    ICF_MEM_COMMON,
                                    p_ssa_pdb->p_ecode,
                                     temp_ret_val)
                            ret_val = ICF_FAILURE ;
                            break;
                        }
                        else
                        {
                            /* Header correctly parsed:Processing Begins
                             * the assumption here is that only seconds are
                             * received in this header, to be copied into the
                             * expires_duration of the rgm_msg and the
                             * indicator bit(of rgm_context) set such that
                             * REGM uses this value in the next registration */

                            p_rgm_msg->expires_duration = (((SipExpiresHeader *)(
                                            p_min_expires->pHeader))->u).dSec;
                            p_rgm_msg->p_rgm_context->indicator |= 
                                ICF_RGM_SSA_DURATION_FILLED;
                            sip_freeSipHeader(p_min_expires);
                            ICF_MEMFREE(
                                    p_ssa_pdb->p_glb_pdb,
                                    p_min_expires,
                                    ICF_MEM_COMMON,
                                    p_ssa_pdb->p_ecode,
                                     temp_ret_val);
                            
                        }

                    }/* Min-Expires header found */
                    sip_freeSipHeader(&dHeader);

                }/* For loop on unknown headers : ends here */
#endif

            }
            break;
        default :
            p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
			if (resp_code > 400 && resp_code < 700 )
			{
				p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_NO_REATTEMPT;
				
			}
			else if (resp_code > 300 && resp_code <400)
			{
				p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;

			}
			else if(resp_code >200 && resp_code <300)
			{

		           icf_ssa_pos_resp_transaction_Completed_for_register(
				ppCallObj,pEventContext,pOverlapTransInfo,pErr);
			}

            /*ICF_SSA_TRACE(("Unhandled message \n"))*/
    } /* End switch (resp_code) */



 #ifdef ICF_LOAD_STAT
    if(ICF_NULL == p_rgm_msg->ecode)
    {  
        p_ssa_pdb->p_glb_pdb->load_data.register_cfm++;
    }
    else
    {
         p_ssa_pdb->p_glb_pdb->load_data.register_failure++;
    }
#endif
	if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                p_ssa_pdb, 
                ICF_INT_MODULE_REGM,
                (icf_void_t*)p_rgm_msg->p_rgm_context,
                pOverlapTransInfo->pSipMsg,
                p_ssa_pdb->p_ecode) )
    {
        ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));

        ret_val = Sdf_co_fail;
    }

    if(Sdf_co_fail != ret_val)
    {
        /* Send response to REGM based on the recd response code */
        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CFG;
    }
#ifdef ICF_ERROR_ENABLE 
   if(ICF_TRUE == err_resp)
   {
     if(ICF_SSA_AUTH_IN_PROGRESS & p_ssa_ctxt->bitmask)
     {
         ICF_SSA_SET_ECODE(p_ssa_pdb,"REGISTER",ICF_ERROR_AUTHENTICATION_FAILURE)
         p_ssa_ctxt->bitmask &= ~(ICF_SSA_AUTH_IN_PROGRESS);
     }
     else
     {
         p_ssa_pdb->resp_code = resp_code;
        ICF_SSA_SET_ECODE(p_ssa_pdb,"REGISTER",ICF_ERROR_FAILURE_RESPONSE)
     }
   }
#endif
    return ret_val;
}


/*********************************************************
** FUNCTION:  icf_ssa_transaction_Completed_for_update  
**
** DESCRIPTION: This Function handles All response received for 
**              Update Request Sent to Network
**
**********************************************************/
Sdf_ty_retVal icf_ssa_transaction_Completed_for_update(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr)
{
    SipMessage                          *p_message = ICF_NULL;
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_media_change_resp_st     *p_media_chng_resp = ICF_NULL;
    icf_boolean_t                       err_resp = ICF_TRUE;
#ifdef ICF_SESSION_TIMER
	SipHeader	dTempHeader;
	Sdf_ty_refresher                    dRefresher;
    icf_nw_otg_session_refresh_req_st   session_refresh_req;

#endif

	/*This st is populated if there is any SDP*/
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL; 
    /*CSR 8061646 SPR 20231*/
    icf_nw_otg_media_change_req_st	   media_chng_req;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    if (ICF_NULL == p_ssa_ctxt)
    {
       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Session timer, SSA Ctxt was is NULL.\n"))
       return ICF_FAILURE; 
    }    
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    if (ICF_NULL == p_ssa_pdb || 
        ICF_NULL == p_ssa_pdb->p_glb_pdb || 
        ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx)
    {
       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Session timer, SSA Pdb is NULL.\n"))
       return ICF_FAILURE; 
    }   
    p_message = pOverlapTransInfo->pSipMsg;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_message))
/* KLOCWORKS */

#if 0
#ifdef ICF_SESSION_TIMER
    if((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && \
           (p_ssa_ctxt->session_timer_expired_refresh == ICF_FALSE))
    {
#endif
#endif
    p_media_chng_resp = 
        (icf_nw_inc_media_change_resp_st*)(p_ssa_pdb->p_internal_msg->p_msg_data);
	if(ICF_NULL == p_media_chng_resp)
	{
		return Sdf_co_fail;
	}
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_MEDIA_UPDATE_RESP;

    p_media_chng_resp->bitmask = ICF_NULL;
    p_media_chng_resp->p_sdp_list = ICF_NULL;
#if 0   
#ifdef ICF_SESSION_TIMER
	}
#endif
#endif
    switch (resp_code)
    {
        case 200:
#ifdef ICF_SESSION_TIMER
    if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    { 
      if(sdf_ivk_uaGetRefresher(*ppCallObj,&dRefresher,pErr) != Sdf_co_fail)
      {  
        if ( Sdf_en_invalidRefesher == dRefresher)
        {
                p_ssa_ctxt->p_call_obj->pSessionTimer->dSessionInterval = 0;
                p_ssa_ctxt->p_call_obj->pSessionTimer->dMinSe = 0;
                p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                          ICF_GENERIC_CALL_CLEAR;
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb =
                          p_ssa_pdb->p_glb_pdb;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_success;
        }
     }
    }
    if(((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && \
       (p_ssa_ctxt->session_timer_expired_refresh == ICF_FALSE)) \
       ||
       (ICF_FALSE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)){      
#endif
                err_resp = ICF_FALSE;
            p_media_chng_resp->status = ICF_TRUE;

            /* Check for the presence of SDP in the received message */
            if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
                    (p_message,pErr))
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
											p_ssa_pdb,p_message,
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
                }			
				else
				{
					p_media_chng_resp->p_sdp_list =\
						p_inc_media_sdp_info->p_sdp_info_list;
					
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP received in 200 OK for UPDATE"));
					
					/*
					* Set the bit which indicates that CONNECT
					* has been received with SDP
					*/
					ICF_SSA_SET_COMMON_BITMASK(p_media_chng_resp->bitmask,
						ICF_NW_INC_MC_SDP_PRESENT);
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
						* Delete the message body list stored in the UAC
						* transaction of the call object.
						* This is done to flush the SDP body from the 
						* transaction substructure of the call object
						*/
						sdf_listDeleteAll(
						&(pOverlapTransInfo->slMsgBodyList),
						pErr);
						/*
						* SPR fix 7403
						* SSA will reset signalling_ongoing flag of Call
						* Context. SSA will also set 
						* ICF_STATUS_SIGNAL_ONOING_ENDED 
						* in status field of internal     message in 
						* return path.
						* TO DO
					*/
					
                }
            } /* ((*ppCallObj)->pUacTransaction... */
            else
            {
               ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP not received in 200 OK"));
			   /* Changes done for Message Body Headers Start :
		        */
               if(ICF_SSA_UPDATE_WITH_SDP & p_ssa_ctxt->bitmask)
               {
                  /* Update Response received without SDP. Send Generic Call Clear
                   * Indication to CC and proper reason cause.
                   */
                   p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                   p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                       ICF_GENERIC_CALL_CLEAR;
                   p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                       p_ssa_pdb->p_glb_pdb;
                   
                   p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                   p_media_chng_resp->reason_type = \
                       ICF_CALL_CLEAR_REASON_REMOTE_MEDIA_NOT_RECEIVED;
                   
               }
                /* Extract the message bodies from SIP message.
				 */
			   else
               {
                   if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
                       (*ppCallObj)->pUacTransaction->pSipMsg,\
                       /**ppCallObj*/SipMessageResponse,&(p_ssa_pdb->p_glb_pdb->p_call_ctx->\
                       body_list)))
                   {
                       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
                       ret_val = ICF_FAILURE;
                   }
                   /* Changes done for Message Body Headers End :
                   */
               }
				
            }
#ifdef ICF_SESSION_TIMER
			}
                else if(ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable){ 
                err_resp = ICF_FALSE;
				p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
				ICF_NW_OTG_SESSION_REFRESH_ACK;
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
				p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
				/* SPR 9591- pass the SPI message instead of the trans. 
				 * pointer */
             	if(dRefresher != Sdf_en_refresherNone || 
						ICF_SESSION_REFRESHER_NONE != 
						p_ssa_pdb->p_line_data->session_timer_refresher)
				{
   				   
				icf_ssa_set_refresher_method(p_message,
					p_ssa_pdb->p_glb_pdb,
					&p_ssa_ctxt,
					pErr);

				if(icf_ssa_start_session_timer(ppCallObj,
					p_ssa_pdb->p_glb_pdb,
#ifdef ICF_WITH_MUATK_2_2
				pEventContext,
#endif					
				pErr) == Sdf_co_fail){
					ret_val = Sdf_co_fail;
				}
			}	
           }
#endif
		if(ICF_SSA_UPDATE_WITH_SDP & p_ssa_ctxt->bitmask)
		{
			p_ssa_ctxt->bitmask &= ~(ICF_SSA_UPDATE_WITH_SDP);
		}

    /* CSR 8061646 SPR 20231: Reset the bitmasks if set when sending Update
		 */
		if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY & p_ssa_ctxt->bitmask_1)
		{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY);
		}
		else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD & p_ssa_ctxt->bitmask_1)
		{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD);
		}
		else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME & p_ssa_ctxt->bitmask_1)
		{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME);
		}
         break;
#ifdef ICF_SESSION_TIMER
		case 422:
                if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
                {                       
			if (sip_getHeader(p_message,SipHdrTypeMinSE, &dTempHeader, \
				(SipError*)&(pErr->stkErrCode)) != SipFail)
			{
				sip_freeSipHeader(&dTempHeader);
				/* Fix for SPR 14409 */
				if ( (ICF_NULL !=(*ppCallObj)->pSessionTimer) &&
					(90 > (*ppCallObj)->pSessionTimer->dMinSe))
				{
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Min-SE in 422 is less than 90, Clear the Call\n"));
					p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
					p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
 						ICF_GENERIC_CALL_CLEAR;
					p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
						p_ssa_pdb->p_glb_pdb;
				
					p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

					ret_val = Sdf_co_fail;
				} 
				else
				{

                        /* CSR 8061646 SPR 20231: Check the bitmask_1 in ssa context. 
					 * 1. if Update sent because of Call Modify then form internal message
					 * to send the same update again with new Min-SE value.
					 * 2. If update sent because of Call-Hold/Resume then form internal 
					 * message to send the same update again with new Min-SE value.
					 * 3. Else Assume it as a session refresh update and form the internal 
					 * message for the same.
					 */
					if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY == (ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY & p_ssa_ctxt->bitmask_1))
					{
						p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_MODIFY_REQ;
						p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st);
						media_chng_req.mesg_type = ICF_NW_OTG_SEND_UPDATE;
						p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_chng_req;
					}
					else if((ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD == (ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD & p_ssa_ctxt->bitmask_1))
							||
							(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME == (ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME & p_ssa_ctxt->bitmask_1)))
					{
						p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_MEDIA_UPDATE;
						p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_nw_otg_media_change_req_st);
						if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD  & p_ssa_ctxt->bitmask_1)
						{
							media_chng_req.call_hold = ICF_TRUE;
						}
						else
						{
							media_chng_req.call_hold = ICF_FALSE;
						}
						p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&media_chng_req;
					}
					else
					{

					/* Send ICF_NW_OTG_SESSION_REFRESH_REQ to SSA to send re-invite
					 * with session interval equals to the value of Min-SE header */
	
					/* Fix for SPR 31210 glb configuration data updation code deleted */
	
					p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_SESSION_REFRESH_REQ;
					p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
						ICF_PORT_SIZEOF(icf_nw_otg_session_refresh_req_st);
					session_refresh_req.refresher_req_type = ICF_SSA_REFRESHER_METHOD_UPDATE;
					p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&session_refresh_req;
                    }
					if(icf_ssa_process_mesg(p_ssa_pdb->p_internal_msg) == ICF_FAILURE){
						ret_val = Sdf_co_fail;
					}
                    else
                     {
                       err_resp = ICF_FALSE;  
                     }
				}
			}
			else
			{
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]: 422 received wo MIn-SE so Call will be cleared\n"));

			    p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
 						ICF_GENERIC_CALL_CLEAR;
				p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
					p_ssa_pdb->p_glb_pdb;
				
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

			}
                            }
                            else
                            {
                                ICF_PRINT(((icf_uint8_t *)"\nSession Timer functionality is Disabled.\n"))
                            }         
			break;

#endif
 		case 401:
        case 407:
    	{
	         p_ssa_pdb->p_method = (icf_uint8_t *)"UPDATE"; 
			 if(ICF_FAILURE == icf_ssa_handle_chal_resp(ppCallObj,
                         pEventContext,pOverlapTransInfo,resp_code,pErr))
        	 {
				/*In case, validate challenge fails, IPTK shall clear the call*/
#ifdef ICF_SESSION_TIMER	
				if ((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && 
					(ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh))
				{           
					p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
								ICF_NW_OTG_SESSION_REFRESH_ACK;
					p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);

					((icf_nw_inc_media_change_resp_st*)(p_ssa_pdb->p_internal_msg->p_msg_data))->status =
                                                          ICF_FALSE;
	
					p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
								p_ssa_pdb->p_glb_pdb;

					p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
					return Sdf_co_success;
				}
				else
				{
#endif
					 /*Call should be cleared if the validate challenge fails*/
					 p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                     p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                ICF_GENERIC_CALL_CLEAR;
                     p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                             p_ssa_pdb->p_glb_pdb;                            
                     p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
					 return Sdf_co_success;
#ifdef ICF_SESSION_TIMER	
				}
#endif

    	     }
			 else
			 {
					/* Auth params are present in response from Peer,
					 * set bitmask in call context for the same */
					p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask|=
						ICF_CALL_AUTHENTICATION_REQ;

					return Sdf_co_success;
			 }
		}
        break;

         case 491 :
                   err_resp = ICF_FALSE; 
                   p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                    ICF_NW_INC_MEDIA_UPDATE_RESP;
                   
                   p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                    ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);

                   ICF_SSA_SET_COMMON_BITMASK(p_media_chng_resp->bitmask,
				            ICF_NW_INC_MC_RESP_REASON);
 
                   p_media_chng_resp->reason_type = 
                       ICF_CALL_CLEAR_REASON_REQUEST_PENDING;
                   p_media_chng_resp->status = ICF_FALSE;

                   break;      
        default :
#ifdef ICF_SESSION_TIMER
               if((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && \
               (p_ssa_ctxt->session_timer_expired_refresh == ICF_TRUE))
	       {
                   err_resp = ICF_TRUE;
           /* Fix for CSR 1_7334810: SPR-19732*/
           /* The call will be clered after receiving 481/408 response from peer
            * for session refresh UPDATE.
            * Refered from RFC-3261 section-12.2.1.2
            * "If the response for a request within a dialog is a 481
               (Call/Transaction Does Not Exist) or a 408 (Request Timeout), the UAC
               SHOULD terminate the dialog.  A UAC SHOULD also terminate a dialog if
               no response at all is received for the request (the client
               transaction would inform the TU about the timeout.)"
            */ 
		   if((481 == resp_code) || (408 == resp_code))
                   {
                       p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                           ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);

		       p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
 					ICF_NW_OTG_SESSION_REFRESH_ACK;
		       p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
			    	p_ssa_pdb->p_glb_pdb;

                       ((icf_nw_inc_media_change_resp_st*)(p_ssa_pdb->p_internal_msg->p_msg_data))->status =
                                                          ICF_FALSE;
		        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                   }
                   else
                   {
                       p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                       ICF_NW_OTG_SESSION_REFRESH_ACK;
                       p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
				   p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                       p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;
   
                       icf_ssa_set_refresher_method(p_message,
					p_ssa_pdb->p_glb_pdb,
					&p_ssa_ctxt,
					pErr);

   	               if(icf_ssa_start_session_timer(ppCallObj,
 				     	p_ssa_pdb->p_glb_pdb,
#ifdef ICF_WITH_MUATK_2_2
           	       pEventContext,
#endif					
		       pErr) == Sdf_co_fail)
                       {
		        	ret_val = Sdf_co_fail;
		       }
        	   }
              }       
	      else
	      {
#endif
              /* Klocwork warning removal.*/
              if (ICF_NULL != p_media_chng_resp)
              { 
				if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                        resp_code,&(p_media_chng_resp->reason_type)))
				{
				    ret_val = Sdf_co_fail;
				}
				else
				{
                                    if((ICF_CALL_TYPE_T38 	& 
                                        p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->call_type) && 
                                        (606 == resp_code))
                                    {
                                       p_media_chng_resp->reason_type = 
                                       ICF_CALL_CLEAR_MEDIA_MISMATCH;
                                    }    
				    p_media_chng_resp->status = ICF_FALSE;
				    ICF_SSA_SET_COMMON_BITMASK(p_media_chng_resp->bitmask,
				            ICF_NW_INC_MC_RESP_REASON);
				}
              }
              else
              {
                ret_val = Sdf_co_fail;   
              }
        /* CSR 8061646 SPR 20231 : In case of failure responses , the bitmask of Media Modify Method should be reset*/
         if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY & p_ssa_ctxt->bitmask_1)
		{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY);
		}
		else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD & p_ssa_ctxt->bitmask_1)
		{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD);
		}
		else if(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME & p_ssa_ctxt->bitmask_1)
		{
			p_ssa_ctxt->bitmask_1 &= ~(ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME);
		}
#ifdef ICF_SESSION_TIMER
			}
#endif
    }
    if ( Sdf_co_fail != ret_val)
    {
        /* Send response to CM */
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
#ifdef ICF_ERROR_ENABLE
   if(ICF_TRUE == err_resp)
   {
      if(resp_code >= 400)
        {
          p_ssa_pdb->resp_code = resp_code;
        }
      ICF_SSA_SET_ECODE(p_ssa_pdb,"UPDATE",ICF_ERROR_FAILURE_RESPONSE)
   }
#endif
    return ret_val;
}


/*********************************************************
** FUNCTION: icf_ssa_transaction_Completed_for_subscribe
**
** DESCRIPTION: This Function handles All response received for 
**              Subscribe Request Sent to Network
**
**********************************************************/

Sdf_ty_retVal icf_ssa_transaction_Completed_for_subscribe(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr)
{

    icf_boolean_t        expires_present = ICF_FALSE;
    icf_duration_t       expires_duration = ICF_NULL;
	icf_duration_t       retry_after_duration = ICF_NULL;
    Sdf_ty_slist            slMatchedHeadersList;
    Sdf_ty_u32bit           size;
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                   *p_ssa_pdb = ICF_NULL;

    icf_nw_inc_subs_resp_st          *p_subs_resp = ICF_NULL;
	icf_boolean_t                    end_subs = ICF_FALSE , err_resp = ICF_TRUE;
    /* To remove compiler warning */
     (void)ppCallObj;


    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);

    if (ICF_NULL == p_ssa_ctxt->p_sic_ctxt)
    {
        /* In this case application has delinked itself
         * and hence it should ignore this response 
         * and return SUCCESS */
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: response to SUBSCRIBE ignored "));
        return ICF_SUCCESS;
    }

    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    /* Send response to SIC based on the recd response code */
    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;

    /* Populate the internal message header */
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_NW_INC_SUBSCRIBE_RESP;

    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
        ICF_PORT_SIZEOF(icf_nw_inc_subs_resp_st);
    p_subs_resp = (icf_nw_inc_subs_resp_st*)(p_ssa_pdb->p_internal_msg->p_msg_data);
    /* Populate the internal message payload */
    p_ssa_pdb->p_internal_msg->p_msg_data = 
        (icf_void_t *)p_subs_resp;

    p_subs_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
    p_subs_resp->bitmask = ICF_NULL;
    p_subs_resp->duration = 0;
	p_subs_resp->response = ICF_NULL;
    p_subs_resp->resp_code = resp_code;

    switch(resp_code)
    {
        case 200:
        case 202:
            /* reset the bit indicating that request was challenged */
            ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);
		
            p_subs_resp->response = ICF_SUCCESS;

            /* Extract registered duration from sip message */
            if (Sdf_co_fail == sdf_ivk_uaGetHeaderListByType(
                        pOverlapTransInfo->slUnprocessedHeaders,
                        SipHdrTypeExpiresSec,
                        &slMatchedHeadersList,
                        pErr))
            {
                ret_val = Sdf_co_fail;
            }
            else if (Sdf_co_fail == sdf_listSizeOf(
                        &slMatchedHeadersList,&size, pErr))
            {
                sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                ret_val = Sdf_co_fail;
            }
            else if (ICF_NULL == size)
            {
                expires_present = ICF_FALSE;
                p_subs_resp->response = 
                    ICF_NW_INC_SUBS_RESP_WITH_RETRY;
            }/* size == 0 */
            else
            {
                SipHeader           *pExpiresHeader = Sdf_co_null;
                Sdf_st_headerList   *pExpiresHeaderList = Sdf_co_null;
                expires_present = ICF_TRUE;

                /* Get HeaderList at index 0 */
                if (Sdf_co_fail == sdf_listGetAt(
                            &slMatchedHeadersList,
                            0,
                            (Sdf_ty_pvoid *)&pExpiresHeaderList,
                            pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                else if (Sdf_co_fail == sdf_listGetAt(
                            &(pExpiresHeaderList)->slHeaders,
                            0,
                            (Sdf_ty_pvoid *)&pExpiresHeader,
                            pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                else
                {

                    expires_duration = 
                        (((SipExpiresHeader *)
                          (pExpiresHeader->pHeader))->u).dSec;
  
                    /*If expires duration is 0 then treat it as failure
                     * response with retry otherwise set bitmask for 
                    * duration present further if the response has 
					 * duration higher than what * we sent we'll treat it
					 * as failure reponse with retry*/
                    /* If Expires contains 0, clear the context and send 
                     * failure response to application */
                      /* Failure should be only sent if the APP did not request the
                       unsubscribe. If the APP has initiated a SUBS with expires 0
                       then success should be sent*/
                    if ((ICF_NULL == expires_duration)&&
                        (ICF_NULL != p_subs_resp->p_sic_ctx)&&
                        (ICF_NULL != p_subs_resp->p_sic_ctx->app_requested_duration))
		          {
                      if ((ICF_NULL != p_subs_resp->p_sic_ctx) &&
                          (p_subs_resp->p_sic_ctx->app_requested_duration !=0))
                         {
                            p_subs_resp->response = 
                                ICF_NW_INC_SUBS_RESP_NO_RETRY;
                         }
                    }
                    /* If Expires contains larger value than request, treat it 
                     * as if the value received is same as that in request */
                    else if ((ICF_NULL != p_subs_resp->p_sic_ctx) && 
                             (p_subs_resp->p_sic_ctx->app_requested_duration < 
                                expires_duration))
		    {
                        p_subs_resp->bitmask =
                            ICF_NW_INC_SUBS_DURATION_PRESENT;

                        p_subs_resp->duration = 
                            p_subs_resp->p_sic_ctx->app_requested_duration;
		    }	
                    else
                    {
                        p_subs_resp->bitmask =
                            ICF_NW_INC_SUBS_DURATION_PRESENT;

                        p_subs_resp->duration = expires_duration;

                    }
                }
                sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                err_resp = ICF_FALSE;
            }/* size != 0 */

            break;

            /*Failure responses for which retry subscription*/
        case 302:
        case 400:
        case 500:
        case 502:
        case 491:
             /* reset the bit indicating that request was challenged */
            ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);

            p_subs_resp->response = ICF_NW_INC_SUBS_RESP_WITH_RETRY;
            break;

        case 503:
            /* reset the bit indicating that request was challenged */
            ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);

	    p_subs_resp->response = ICF_NW_INC_SUBS_RESP_WITH_RETRY;

            if (Sdf_co_fail == sdf_ivk_uaGetHeaderListByType(
                        pOverlapTransInfo->slUnprocessedHeaders,
                        SipHdrTypeRetryAfterSec,
                        &slMatchedHeadersList,
                        pErr))
            {
                ret_val = Sdf_co_fail;
            }
            else if (Sdf_co_fail == sdf_listSizeOf(
                        &slMatchedHeadersList,&size, pErr))
            {
                sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                ret_val = Sdf_co_fail;
            }
            else if (ICF_NULL != size)
            {
                SipHeader           *pRetryAfterHeader = Sdf_co_null;
                Sdf_st_headerList   *pRetryAfterHeaderList = Sdf_co_null;

                /* Get HeaderList at index 0 */
                if (Sdf_co_fail == sdf_listGetAt(
                            &slMatchedHeadersList,
                            0,
                            (Sdf_ty_pvoid *)&pRetryAfterHeaderList,
                            pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                else if (Sdf_co_fail == sdf_listGetAt(
                            &(pRetryAfterHeaderList)->slHeaders,
                            0,
                            (Sdf_ty_pvoid *)&pRetryAfterHeader,
                            pErr))
                {
                    ret_val = Sdf_co_fail;
                }
                else
                {
                    retry_after_duration = 
                        (((SipRetryAfterHeader *)
                          (pRetryAfterHeader->pHeader))->u).dSec;

                    if(ICF_NULL != retry_after_duration)
                    {
                        p_subs_resp->bitmask =
                            ICF_NW_INC_SUBS_DURATION_PRESENT;
                        p_subs_resp->duration = retry_after_duration;
                    }
                }
                sdf_listDeleteAll(&slMatchedHeadersList,pErr);
            }/* size != 0 */

            break;

        /* Handle the challenge for Subscribe request */
        case 401:
        case 407:
            if(resp_code == 401)
            {
                ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_SUBS_AUTH_401)
            }
            else
            { 
                  ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_SUBS_AUTH_407)
            }
            icf_ssa_fetch_auth_param_list(p_ssa_pdb,
                    ppCallObj, pOverlapTransInfo);

            /* If list is not null and auth param are configured on line
             * send SUBSCRIBE with auth params else send failure response */
            if ((ICF_NULL != p_ssa_ctxt->auth_params_list_size) &&
                (ICF_NULL == (p_ssa_ctxt->p_sic_ctxt->bitmask & 
                              ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE)))
             {
                /* Fetch Line data from DBM */
                if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                            p_ssa_pdb->p_glb_pdb,
                            p_ssa_ctxt->line_id,
                            &(p_ssa_pdb->p_line_data),
                            p_ssa_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                else if((p_ssa_ctxt->p_sic_ctxt)&&
                        (ICF_SUCCESS == 
                         icf_cmn_check_if_auth_params_present(
                             p_ssa_pdb->p_glb_pdb, 
                             p_ssa_ctxt->p_sic_ctxt->p_user_address, 
                             p_ssa_ctxt->line_id)))
                {
                    if (ICF_TRUE == ICF_IS_BIT_SET(
                                    p_ssa_pdb->p_line_data->line_data_received,
                                    ICF_CFG_LINE_DATA_REALM_RECD))
                    {
                        /* We have to compare the received realm with configured
                         * one */
                        Sdf_st_authenticationParams *pAuthenticationParams = 
                                                        Sdf_co_null;
                        if (Sdf_co_success == sdf_listGetAt(
                                        p_ssa_ctxt->p_auth_params_list,
                                        0, 
                                        (Sdf_ty_pvoid *)&pAuthenticationParams, 
                                        pErr))
                        {
                            if (Sdf_co_null != pAuthenticationParams->pRealm)
                            {
                                icf_string_st temp_unq_realm = {0,"0"};
                                temp_unq_realm.str[0] = '\0';
                                sdf_fn_uaUnq(
                                    pAuthenticationParams->pRealm,
                                    (icf_int8_t *)temp_unq_realm.str);
                                if (0 != icf_port_strcmp(
                                   p_ssa_pdb->p_line_data->configured_realm.str,
                                   temp_unq_realm.str))
                                {
                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA] Subscribe challenge: "\
                                    "challenge realm not same as configured"));
                                    end_subs = ICF_TRUE;
                                }
                            }
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                        }
                    }
                 } /* if (line_data_received) */
                 else
                 {
                    ICF_PRINT(((icf_uint8_t *)"\nUser auth params not configured on line"));
                    end_subs = ICF_TRUE;
                 }
            } /* if(ICF_NULL != p_ssa_ctxt->auth_params_list_size ) */
            else
            {
                end_subs = ICF_TRUE;
               /* reset the bit indicating that request was challenged */
                ICF_RESET_BIT(
                    p_ssa_ctxt->p_sic_ctxt->bitmask,
                    ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);
            }
            
            if (ICF_TRUE == end_subs)
            {
                ICF_PRINT(((icf_uint8_t *)"\nSubscription challenge failed"));
                p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
            }
            else if (ICF_SUCCESS == ret_val)
            {
                if(ICF_FAILURE == icf_ssa_validate_challenge(ppCallObj,
                                       pOverlapTransInfo,
                                       resp_code,p_ssa_pdb,pErr,
                                       (icf_uint8_t *)"SUBSCRIBE"))
                {
                    ICF_PRINT(((icf_uint8_t *)"\nSubscription challenge failed"));
                    p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
                }
                else
                {
                    /* set the bit indicating that request has been challenged */
                    p_ssa_ctxt->p_sic_ctxt->bitmask |= 
                        ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE;
                    /* Fetch and store the old nonce in the ssa context */
               
                    if(ICF_FAILURE == icf_ssa_extract_nonce_from_challenge(
                                          p_ssa_pdb,
                                          resp_code))
                    {
                        ret_val = ICF_FAILURE;
                    }
	
                    p_ssa_pdb->p_method = (icf_uint8_t *)"SUBSCRIBE"; 
                    ret_val = icf_ssa_send_authorized_req(p_ssa_pdb);

                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                    return ret_val;
                }
            }
            break;

        case 403:
        case 404:
        case 481:
            /* reset the bit indicating that request was challenged */
	    ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);

            p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
            break;

        default :
   /* reset the bit indicating that request was challenged */
            ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);
            p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
            /*ICF_SSA_TRACE(("Unhandled message \n"))*/
    } /* switch(resp_code) */



    if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                p_ssa_pdb, 
                ICF_INT_MODULE_SIC,
                (icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
                pOverlapTransInfo->pSipMsg,
                p_ssa_pdb->p_ecode) )
    {
        ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));

        ret_val = Sdf_co_fail;
    }
#ifdef ICF_ERROR_ENABLE
   if(ICF_TRUE == err_resp)
   {
      if(ICF_SSA_AUTH_IN_PROGRESS & p_ssa_ctxt->bitmask)
      {
        p_ssa_ctxt->bitmask &= ~(ICF_SSA_AUTH_IN_PROGRESS);
       ICF_SSA_SET_ECODE(p_ssa_pdb,"SUBSCRIBE", ICF_ERROR_AUTHENTICATION_FAILURE)
      }
      else
      {
          p_ssa_pdb->resp_code = resp_code;
          ICF_SSA_SET_ECODE(p_ssa_pdb,"SUBSCRIBE", ICF_ERROR_FAILURE_RESPONSE)
      }
    }
#endif
    return ret_val;
}

/*********************************************************
** FUNCTION: icf_ssa_transaction_Completed_for_publish
**
** DESCRIPTION: This Function handles All response received for 
**              Publish Request Sent to Network
**
**********************************************************/

Sdf_ty_retVal icf_ssa_transaction_Completed_for_publish(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr)
{


    icf_boolean_t        expires_present = ICF_FALSE;
    icf_duration_t       expires_duration = ICF_NULL;
    Sdf_ty_slist            slMatchedHeadersList;
    Sdf_ty_u32bit           size;
    SipMessage                          *p_message = ICF_NULL;
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                   *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_publish_resp_st        *p_publish_resp;
    /* To remove compiler warning */
    (void)ppCallObj;
    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

/*        (icf_nw_inc_publish_resp_st*)(p_ssa_pdb->p_internal_msg->p_msg_data);*/

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

	p_message = pOverlapTransInfo->pSipMsg;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_message))

    /* Send response to SIC based on the recd response code */
    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;

    /* Populate the internal message header */
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
        ICF_NW_INC_PUBLISH_RESP;

    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
        ICF_PORT_SIZEOF(icf_nw_inc_publish_resp_st);
    
	p_publish_resp = (icf_nw_inc_publish_resp_st*)(p_ssa_pdb->p_internal_msg->p_msg_data);
	/* Populate the internal message payload */
    p_ssa_pdb->p_internal_msg->p_msg_data = 
        (icf_void_t *)p_publish_resp;

    p_publish_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
    p_publish_resp->duration = 0;

    p_publish_resp->response = resp_code;

    if((resp_code == 200) || (resp_code == 202) )
    {
		en_HeaderType 				dType = SipHdrTypeAny;
		icf_uint8_t*				p_temp_buff = ICF_NULL;
		icf_uint8_t*				p_remainingbuff = ICF_NULL;
		Sdf_ty_u32bit				dCount = 0;
		icf_uint8_t*				p_token = ICF_NULL;
		SipError                    sip_err = 0;

        /* Extract registered duration from sip message */
        if (Sdf_co_fail == sdf_ivk_uaGetHeaderListByType(
                    pOverlapTransInfo->slUnprocessedHeaders,
                    SipHdrTypeExpiresSec,
                    &slMatchedHeadersList,
                    pErr))
        {
            ret_val = Sdf_co_fail;
        }
        else if (Sdf_co_fail == sdf_listSizeOf(
                    &slMatchedHeadersList,&size, pErr))
        {
            sdf_listDeleteAll(&slMatchedHeadersList,pErr);
            ret_val = Sdf_co_fail;
        }
        else if (ICF_NULL == size)
        {
            expires_present = ICF_FALSE;
        }/* size == 0 */
        else
        {
            SipHeader           *pExpiresHeader = Sdf_co_null;
            Sdf_st_headerList   *pExpiresHeaderList = Sdf_co_null;
            expires_present = ICF_TRUE;

            /* Get HeaderList at index 0 */
            if (Sdf_co_fail == sdf_listGetAt(
                        &slMatchedHeadersList,
                        0,
                        (Sdf_ty_pvoid *)&pExpiresHeaderList,
                        pErr))
            {
                ret_val = Sdf_co_fail;
            }
            else if (Sdf_co_fail == sdf_listGetAt(
                        &(pExpiresHeaderList)->slHeaders,
                        0,
                        (Sdf_ty_pvoid *)&pExpiresHeader,
                        pErr))
            {
                ret_val = Sdf_co_fail;
            }
            else
            {
                expires_duration = 
                    (((SipExpiresHeader *)
                      (pExpiresHeader->pHeader))->u).dSec;

                p_publish_resp->duration = expires_duration;

            }
            sdf_listDeleteAll(&slMatchedHeadersList,pErr);
        }/* size != 0 */


		/*Fetch the SIP-ETag header*/

		icf_ssa_get_hdr_type_from_name((icf_uint8_t *)"SIP-ETag", &dType, &sip_err);

		/* There can be multiple hdrs for that type */
		if (sip_getHeaderCount(pOverlapTransInfo->pSipMsg, dType, &dCount, \
			&sip_err) == SipFail)
		{
			/*ICF_SSA_TRACE(("\nFailed to get Header Count "))*/
				return ICF_FAILURE;
		}
		if(dCount)
		{
			icf_uint32_t	    dEtagValueLength = 0;
            Sdf_ty_u32bit       curr_count = 0;
            SipHeader           dHeader;

            for (curr_count=0; curr_count < dCount; curr_count++)
            {
                if(SipFail == sip_getHeaderAtIndex(
                                    pOverlapTransInfo->pSipMsg, 
                                    dType,
                                    &dHeader, 
                                    curr_count,
                                    &sip_err))
                {
                    ret_val = ICF_FAILURE;
                    break;
                }		
                else
                {
                    /* SIP-ETag header found: Processing Begins */

                    /* WarTBD - Hope the p_temp_buff works */
                    SIP_S8bit ** p_p_temp_buff= (SIP_S8bit**) &p_temp_buff;
                    icf_int8_t ** p_p_remainingbuff=(icf_int8_t **) &p_remainingbuff;
                    
                    sip_getHeaderAsStringAtIndex(
                        pOverlapTransInfo->pSipMsg,
                        dType,
                        p_p_temp_buff,
                        curr_count,
                        &sip_err);

                    p_token = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)p_temp_buff,":",
                               p_p_remainingbuff); 
                    
                    /* Strip the leading spaces*/
                    p_remainingbuff = 
                        (icf_uint8_t *)sip_stripLeadingLWS((icf_int8_t *)p_remainingbuff,
                        (icf_int8_t *)((p_remainingbuff + icf_port_strlen((icf_uint8_t *)p_remainingbuff))));
                    /*Strip the trailing CRLF*/
                    dEtagValueLength = icf_port_strlen(p_remainingbuff);
                    p_remainingbuff[dEtagValueLength] = '\0';
                    
                    icf_port_strcpy(p_ssa_ctxt->p_sic_ctxt->entity_tag.str,p_remainingbuff);
                    p_ssa_ctxt->p_sic_ctxt->entity_tag.str_len = (icf_uint16_t )icf_port_strlen(
                        (icf_uint8_t *)(p_ssa_ctxt->p_sic_ctxt->entity_tag.str));
                    fast_memfree(0, p_temp_buff, &sip_err);

                    sip_freeSipHeader(&dHeader);
                    break;
                }
                sip_freeSipHeader(&dHeader);

            } /* End for (curr_count) */
		}/* End if(dCount) */


    }
	else if ( 423 == resp_code)
	{             
		SipError            sip_err;
		Sdf_ty_u32bit       curr_count = 0;
		SipHeader           dHeader;
		SipCommonHeader     *p_min_expires = ICF_NULL;

        /* Fetch the Min-Expires header field if present */
        if(ICF_SUCCESS == icf_ssa_isHeaderPresent(
                                       p_message,
                                       SipHdrTypeMinExpires,
                                       pErr))
         {
             
		if(SipFail == sip_getHeaderAtIndex(p_message,
                                 SipHdrTypeMinExpires, &dHeader,
                                 curr_count, &sip_err))

              {
                    ret_val = ICF_FAILURE;
              }
              else
              {
                    p_min_expires = (SipCommonHeader *)(dHeader.pHeader);
                    p_publish_resp->duration = p_min_expires->dIntVar1;
                    sip_freeSipHeader(&dHeader);
              }
          }

	}


		
	
    if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                p_ssa_pdb, 
                ICF_INT_MODULE_SIC,
                (icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
                pOverlapTransInfo->pSipMsg,
                p_ssa_pdb->p_ecode) )
    {
        ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));

        ret_val = Sdf_co_fail;
    }

	p_ssa_ctxt->p_sic_ctxt = ICF_NULL;

	/*for details of this fix, check the comment in ssa_types.h
	for the above mentioned bitmask*/
	ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
			ICF_SSA_REL_CALL_OBJ_ON_RET );
	/*sdf_ivk_uaFreeCallObject(ppCallObj);*/

    return ret_val;
}

/*******************************************************************************
 * 
 * FUNCTION:        sdf_cbk_uaTransactionCompleted
 *
 * DESCRIPTION:     Callback issued onto the application on the receipt of a 
 *                  final response message to one of 
 *                  REGISTER/OPTIONS/INFO/SUBSCRIBE/NOTIFY
 *
 ******************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTransactionCompleted(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_st_error                    *pErr)
{
    Sdf_ty_u16bit                       resp_code;
    SipMessage                          *p_message = ICF_NULL;
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_ty_s8bit                        *p_method = ICF_NULL;
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                   *p_ssa_pdb = ICF_NULL;
    SipHeader                        *pRetryAfterHeader = Sdf_co_null;
    Sdf_st_headerList                *pRetryAfterHeaderList = Sdf_co_null;
    Sdf_ty_slist                      slMatchedHeadersList;
    Sdf_ty_u32bit                       size = 0;
    /*This structure is populated if there is sdp in the response msg.
	 * Then we populate the call context correspondingly.
	 */
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    

    /* to remove compiler warning */
    ppCallObj = ppCallObj;
    
    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    p_message = pOverlapTransInfo->pSipMsg;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_message))

    /* get the method from received SIP response message */
    if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                                p_message, &p_method, pErr))
    {
        ret_val = Sdf_co_fail;
    }
    /* get the response code from received SIP response message */
    else if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                                p_message, &resp_code, pErr))
    {
        ret_val = Sdf_co_fail;
    }
#ifdef ICF_STAT_ENABLE
   else
   {

	icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)p_method,resp_code,0);
   }
#endif
#ifdef ICF_IPSEC_ENABLED

    if ((ICF_NULL != p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
        (ICF_NULL != p_ssa_ctxt->ssa_sa_context.p_sa_context))
    {
        /* decrement the active transaction count */
	    p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans -= 1;
		ICF_PRINT(("[SSA]: num_active_trans decremented, current value = %d", 
			p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans));

	    /* reset the threshold counter value */
	    p_ssa_pdb->p_ssa_glb_data->num_successive_trans_fail = ICF_NULL;

	    /* check if the active transaction count == 0. if yes and the
	     * response is for a non Register transaction, so send a message
	     * to REGM for deleting the SA if the transaction comp. on the 
	     * old SA
	     */
	    if ((ICF_NULL == p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans) &&
		    (ICF_NULL != Sdf_mc_strcasecmp(p_method,"REGISTER")))
	    {
		    ret_val = icf_ssa_rgm_delete_old_sa_ind(p_ssa_pdb);
	    }
    }
#endif
    if (ICF_FAILURE != ret_val)
    {
        /* Check if it was a registration transaction */
        if (0 == Sdf_mc_strcasecmp(p_method,"REGISTER"))
        {
           ret_val = icf_ssa_transaction_Completed_for_register(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr);

        } /* if (0 == Sdf_mc_strcasecmp(p_m... */
         /* End if (method == REGISTER) */
        else if (0 == Sdf_mc_strcasecmp(p_method,"UPDATE"))
        {
           ret_val = icf_ssa_transaction_Completed_for_update(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr);

        }
        else if (0 == Sdf_mc_strcasecmp(p_method,"NOTIFY"))
        {
            /*  Fix for CSR 1-7618644: SPR 19989 */
            p_ssa_pdb->p_method="NOTIFY";
            if(401 == resp_code || 407 == resp_code)
            {
                if(ICF_FAILURE == icf_ssa_handle_chal_resp(ppCallObj,
                            pEventContext,pOverlapTransInfo,resp_code,pErr))
                {
#ifdef ICF_ERROR_ENABLE
                    ICF_SSA_SET_ECODE(p_ssa_pdb,"NOTIFY",ICF_ERROR_AUTHENTICATION_FAILURE)
#endif
                    ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2,
                            ICF_NOTIFY_AUTHENTICATION_REQ);
                    /* Send response to CM */
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                    /* Populate the internal message header */
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id
                        = ICF_NW_INC_NOTIFY_RESP;
                    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 0;


                }
                else
                {
                    p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask|=
                        ICF_CALL_AUTHENTICATION_REQ;
                    p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2|=
                        ICF_NOTIFY_AUTHENTICATION_REQ;
                }
            }
            else
            {
                ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2,
                        ICF_NOTIFY_AUTHENTICATION_REQ);

                /* Send response to CM */
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

                /* Populate the internal message header */
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_NOTIFY_RESP;
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 0;
            }

        }
        else if (0 == Sdf_mc_strcasecmp(p_method,"REFER"))
        {
            icf_nw_inc_refer_resp_st     *p_refer_resp =
                (icf_nw_inc_refer_resp_st*)
                (p_ssa_pdb->p_internal_msg->p_msg_data);

            p_refer_resp->response = resp_code;
             p_ssa_pdb->p_method="REFER";

            if(401 == resp_code || 407 == resp_code)
            {
                 if(ICF_FAILURE == icf_ssa_handle_chal_resp(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr))
                 {
                     ret_val = ICF_FAILURE;
                 }
                 else
                 {
                     /* Increment stats for outgoing REFER requests sent */
                     ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                           stat_requests.stat_outbound_req.stats_REFER);
                 }
            }


        /* The value of rettry header field is retrived is present in the 
           failure response of REFER */
    
         switch(resp_code)
         {

          case 404:
          case 413:
          case 480:
          case 486:
          case 500:
          case 503:
          case 600:
          case 603: 
               if (Sdf_co_fail == sdf_ivk_uaGetHeaderListByType(
                        pOverlapTransInfo->slUnprocessedHeaders,
                        SipHdrTypeRetryAfterSec,
                        &slMatchedHeadersList,
                        pErr))
                 {
                    ret_val = Sdf_co_fail;
                 }
                else if (Sdf_co_fail == sdf_listSizeOf(
                        &slMatchedHeadersList,&size, pErr))
                 {
                    sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                    ret_val = Sdf_co_fail;
                 }
                else if (ICF_NULL != size)
                 {

                    /* Get HeaderList at index 0 */
                    if (Sdf_co_fail == sdf_listGetAt(
                              &slMatchedHeadersList,
                              0,
                             (Sdf_ty_pvoid *)&pRetryAfterHeaderList,
                              pErr))
                     {
                          ret_val = Sdf_co_fail;
                     }
                    else if (Sdf_co_fail == sdf_listGetAt(
                              &(pRetryAfterHeaderList)->slHeaders,
                              0,
                             (Sdf_ty_pvoid *)&pRetryAfterHeader,
                              pErr))
                     {
                          ret_val = Sdf_co_fail;
                     }
                    else
                     {
                        p_ssa_ctxt->p_call_ctxt->retry_after_duration =
                             (((SipRetryAfterHeader *)
                             (pRetryAfterHeader->pHeader))->u).dSec;
                        sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                     }
                 }

                break;
		 }
            if(ICF_FAILURE != ret_val &&
                             ICF_FALSE == p_ssa_pdb->authorised_req)
            {

            /*If SIC context is not null then send REFER response to
             * SIC module else send REFER response to service(CM) module*/
            if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
            {
                p_refer_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;
                /* Populate the internal message header */
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_NW_INC_REFER_RESP;
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_nw_inc_refer_resp_st);
                
                if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                    p_ssa_pdb, 
                    ICF_INT_MODULE_SIC,
                    (icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
                    pOverlapTransInfo->pSipMsg,
                    p_ssa_pdb->p_ecode) )
                {
                    ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));
                    
                    ret_val = ICF_FAILURE;
                }
                if ( 
                    !( p_ssa_ctxt->p_sic_ctxt->subs_reqd ) &&
                    !(p_ssa_ctxt->p_sic_ctxt->bitmask & ICF_SIC_REQUEST_WITHIN_DIALOG) 
                    )
                {
                      /* If the request is outside dialog and subscription is not */
                    /* requested, clear the dialog*/
                    p_ssa_ctxt->p_sic_ctxt= ICF_NULL;
		/*Fix in TLS SI*/
		ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
			ICF_SSA_REL_CALL_OBJ_ON_RET );
        /*            sdf_ivk_uaFreeCallObject(ppCallObj);*/
                }
            }
            else
            {
                p_refer_resp->error_cause = ICF_NULL;
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                /* Populate the internal message header */
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_NW_INC_REFER_RESP;
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_nw_inc_refer_resp_st);
                
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
                
                if ( 202 == resp_code )
                {
                    p_refer_resp->status = ICF_TRUE;
                }
                else
                {
                    p_refer_resp->status = ICF_FALSE;
                    
                    if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                        resp_code,&(p_refer_resp->error_cause)))
                    {
                        ret_val = ICF_FAILURE;
                    }
                }
            }
            }

        }
ICF_SIC_START
         else if (0 == Sdf_mc_strcasecmp(p_method,"SUBSCRIBE"))
         {
             ret_val = icf_ssa_transaction_Completed_for_subscribe(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr);

         }
		else if (0 == Sdf_mc_strcasecmp(p_method,"OPTIONS"))
        {

		icf_nw_inc_opt_resp_st  *p_opt_resp = 
                (icf_nw_inc_opt_resp_st*)
                (p_ssa_pdb->p_internal_msg->p_msg_data);


            /* If sic_ctx is null, it means that the SIC context corresponding 
             * to this request has been deleted, probably due to app_remove */
            if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
            {
#if 0 
                /* Send response to SIC based on the recd response code */
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;


                /* Populate the internal message header */
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_NW_INC_OPTIONS_RESP;
#endif
                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                    ICF_PORT_SIZEOF(icf_nw_inc_opt_resp_st);

                /* Populate the internal message payload */
                p_ssa_pdb->p_internal_msg->p_msg_data = 
                    (icf_void_t *)p_opt_resp;

                p_opt_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
            
			    p_opt_resp->response = resp_code;
               p_ssa_pdb->resp_code = resp_code;
#ifdef ICF_TRACE_ENABLE
               /*
               icf_ssa_populate_address_from_sip_addr_spec(
                          p_ssa_pdb,p_from->pAddrSpec,
                          &(p_ssa_ctxt->p_sic_ctxt->dest_addr));


              icf_ssa_populate_address_from_sip_addr_spec(
                          p_ssa_pdb,p_to->pAddrSpec,
                          &(p_ssa_ctxt->p_sic_ctxt->source_addr));

            icf_port_strcpy(p_ssa_pdb->local_addr,p_from->pAddrSpec);
            icf_port_strcpy(p_ssa_pdb->remote_addr,p_to->pAddrSpec);
               */
            ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_OTG_OPTIONS_RESP)
#endif
   /*Check if the rsp code is 401/407, then send the appropriate */
                if((401 == resp_code) || (407 == resp_code))
                {
                   icf_ssa_fetch_auth_param_list(p_ssa_pdb,
                                    ppCallObj,pOverlapTransInfo);
                   if(ICF_NULL == p_ssa_ctxt->auth_params_list_size) 
                   {  
                       /*p_opt_resp->p_sic_ctx->bitmask |= 
                                  ICF_OPT_REQ_CHALLENGD_WO_AUTH_PRMS;*/
                       return ICF_SUCCESS;
                   }
                   else
                   {
                          /* Fetch Line data from DBM */
                         if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                            p_ssa_pdb->p_glb_pdb,
                            p_ssa_ctxt->line_id,
                            &(p_ssa_pdb->p_line_data),
                            p_ssa_pdb->p_ecode))
                         {
                             ret_val = ICF_FAILURE;
                          }
                        else if ((p_ssa_pdb->p_line_data->line_data_received &
                            ICF_CFG_LINE_DATA_AUTH_KEY_RECD) &&
                         (p_ssa_pdb->p_line_data->line_data_received &
                            ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD))
                       {
                              if (ICF_TRUE == ICF_IS_BIT_SET(
                                    p_ssa_pdb->p_line_data->line_data_received,
                                    ICF_CFG_LINE_DATA_REALM_RECD))
                              {
                                     /* We have to compare the received realm with configured
                                       * one */
                                   Sdf_st_authenticationParams *pAuthenticationParams = 
                                                             Sdf_co_null;
                                  if (Sdf_co_success == sdf_listGetAt(
                                                       p_ssa_ctxt->p_auth_params_list,
                                                       0, 
                                                      (Sdf_ty_pvoid *)&pAuthenticationParams, 
                                                      pErr)) 
                                 {
                                        if (Sdf_co_null != pAuthenticationParams->pRealm)
                                       {
                                            icf_string_st temp_unq_realm = {0, "0"};
                                            temp_unq_realm.str[0] = '\0';
                                            sdf_fn_uaUnq(
                                                   pAuthenticationParams->pRealm,
                                                  (icf_int8_t *)temp_unq_realm.str);
                                            if (0 != icf_port_strcmp(
                                              p_ssa_pdb->p_line_data->configured_realm.str,
                                              temp_unq_realm.str))
                                            {
                                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA] Option challenge: "\
                                                    "challenge realm not same as configured"));
                                            }
                                       }
                                  }
                                  else
                                  {
                                      ret_val = ICF_FAILURE;
                                  }
                    }
                 } /* if (line_data_received) */
                 else
                 {
                    ICF_PRINT(((icf_uint8_t *)"\nUser auth params not configured on line"));
                 }
            } /* if(ICF_NULL != p_ssa_ctxt->auth_params_list_size ) */
          
            if (ICF_SUCCESS == ret_val)
            {	
		  p_ssa_pdb->p_method = (icf_uint8_t *)"OPTIONS"; 
                ret_val = icf_ssa_send_authorized_req(p_ssa_pdb);
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return ret_val;
            }

	    }
			 
     	       /* Check for the presence of SDP in the received message */
          	   if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
					(pOverlapTransInfo->pSipMsg,pErr))
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
										pOverlapTransInfo->pSipMsg,
										0,&p_inc_media_sdp_info,
										p_ssa_pdb->p_ecode))
					{
						if(ICF_NULL != p_inc_media_sdp_info)
						{
							/* Free the acquired memory for the SDP */
							/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
							icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
								&p_inc_media_sdp_info);
						}

						ret_val = ICF_FAILURE;
					}
					else if(ICF_NULL == p_inc_media_sdp_info)
					{
						ret_val = ICF_FAILURE;
					}
					else
					{		
						p_opt_resp->p_sic_ctx->p_sdp_list =\
							p_inc_media_sdp_info->p_sdp_info_list;
           
						/* spr 14084 MemLeak*/
						ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,\
                            (p_inc_media_sdp_info),\
                            ICF_MEM_COMMON,\
                            p_ssa_pdb->p_ecode,\
                            ret_val)
 
						ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP received in the 200 OK"));
             
						/* 
						 * Delete the message body list stored in the Overlap
						 * transaction of the call object.
						 * This is done to flush the SDP body from the 
						 * transaction substructure of the call object
						*/
						sdf_listDeleteAll(
							&(pOverlapTransInfo->slMsgBodyList),
							pErr);
					}
				}     
    
				else
				{
        
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP not received in 200 OK"));
					/* Changes done for Message Body Headers Start :
					 */
					if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
							pOverlapTransInfo->pSipMsg,\
							 SipMessageResponse,&(p_ssa_ctxt->p_sic_ctxt->body_list)))
					{
							ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
							ret_val = ICF_FAILURE;
					}
					/* Changes done for Message Body Headers End :
					 */
				}

			    if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
								    p_ssa_pdb, 
								    ICF_INT_MODULE_SIC,
								    (icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
								    pOverlapTransInfo->pSipMsg,
								    p_ssa_pdb->p_ecode) )
			    {
				    ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));

					/*Free the memory allocated to icf_ssa_inc_media_stream_info_st */
					icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
										&p_inc_media_sdp_info); 

				    ret_val = ICF_FAILURE;
			    }
				p_ssa_ctxt->p_sic_ctxt = ICF_NULL;

            } /* End if(p_ssa_ctxt->p_sic_ctxt) */
            else
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: response to OPTIONS ignored "));
            }

            /* code for in-dialog options */
            if(ICF_NULL != p_ssa_ctxt->p_call_ctxt &&
                    p_ssa_ctxt->p_call_ctxt->common_bitmask & ICF_IN_DLG_OPTIONS_SEND) 
            {
            }
			/*for details of this fix, check the comment in ssa_types.h
			  for the above mentioned bitmask*/

            else
            {
                ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
				      ICF_SSA_REL_CALL_OBJ_ON_RET );		    
            }
#ifdef ICF_LOAD_STAT
/*       
 if(ICF_SUCCESS == ret_val)
        {
            p_ssa_pdb->p_glb_pdb->load_data.otg_options_success++;
        }
*/
#endif

           /* Send response to SIC based on the recd response code */
           p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;


            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_NW_INC_OPTIONS_RESP;

		}
		else if (0 == Sdf_mc_strcasecmp(p_method,"MESSAGE"))
    	{
		icf_nw_inc_msg_resp_st  *p_msg_resp = 
			(icf_nw_inc_msg_resp_st*)
			(p_ssa_pdb->p_internal_msg->p_msg_data);

		/* If sic_ctx is null, it means that the SIC context corresponding 
		 * to this request has been deleted, probably due to app_remove */
		if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
		{
            ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
			/* Send response to SIC based on the recd response code */
			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;

			/* Populate the internal message header */
			p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
				ICF_NW_INC_MESSAGE_RESP;

			p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
				ICF_PORT_SIZEOF(icf_nw_inc_msg_resp_st);

			/* Populate the internal message payload */
			p_ssa_pdb->p_internal_msg->p_msg_data = 
				(icf_void_t *)p_msg_resp;


			p_msg_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;

			p_msg_resp->response = resp_code;

			if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
						p_ssa_pdb, 
						ICF_INT_MODULE_SIC,
						(icf_void_t*)p_ssa_ctxt->p_sic_ctxt,
						pOverlapTransInfo->pSipMsg,
						p_ssa_pdb->p_ecode) )
			{
				ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));

				ret_val = ICF_FAILURE;
			}

			p_ssa_ctxt->p_sic_ctxt = ICF_NULL;

		} /* End if(p_ssa_ctxt->p_sic_ctxt) */
		else
		{
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: response to MESSAGE ignored "));
		}

		/*for details of this fix, check the comment in ssa_types.h
		  for the above mentioned bitmask*/
		ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
				ICF_SSA_REL_CALL_OBJ_ON_RET );

	}
		else if (0 == Sdf_mc_strcasecmp(p_method,"PUBLISH"))
        {
            /* If sic_ctx is null, it means that the SIC context corresponding 
             * to this request has been deleted, probably due to app_remove */
            if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
            {
                ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
                ret_val = icf_ssa_transaction_Completed_for_publish(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr);
            }
            else
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: response to PUBLISH ignored "));
                
                /*for details of this fix, check the comment in ssa_types.h
                for the above mentioned bitmask*/
                ICF_SSA_SET_COMMON_BITMASK((p_ssa_ctxt->bitmask),\
                    ICF_SSA_REL_CALL_OBJ_ON_RET );
            }
        }
		else if (0 == Sdf_mc_strcasecmp(p_method,"INFO"))
        {
           ret_val = icf_ssa_transaction_Completed_for_info(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr);
        }
		
ICF_SIC_END
    } /* End else */

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*********************************************************
 ** FUNCTION:sdf_cbk_uaCallRedirected
 **
 ** DESCRIPTION: Callback issued to the application on the receipt of an
 **                              3xx message indicating redirection of a call
 **
 **********************************************************/
Sdf_ty_retVal sdf_cbk_uaCallRedirected(
        Sdf_st_callObject **ppCallObj,
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)

{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_diversion_info_st    *p_div_hdr_list = ICF_NULL;
    icf_uint8_t              *p_contact_disp_name = ICF_NULL;
    icf_ssa_pdb_st           *p_ssa_pdb = 
        (icf_ssa_pdb_st*)(((icf_ssa_ctxt_st*)(pEventContext->pData))->p_ssa_pdb);

    Sdf_ty_u16bit       resp_code;
    Sdf_ty_u32bit       size = 0;
    Sdf_st_commonInfo   *p_common_info = Sdf_co_null;
    SipHeader           *p_contact_hdr = SIP_NULL;
    SipAddrSpec         *p_contact_spec = SIP_NULL;
    Sdf_ty_retVal       sdf_ret_val = Sdf_co_success;
    SipOptions          options;
	icf_uint8_t			param_str[ICF_MAX_STR_LEN] = "\0";
    /* Added for forking support */
    icf_ssa_ctxt_st       *p_ssa_ctxt = ICF_NULL;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);    

    /*FORKING:- If This is not the call object on which last forked
     * response received then ignore the message*/
    /*if (p_ssa_ctxt != p_ssa_ctxt->p_call_ctxt->p_sip_ctx)
    {
        Discuss: if we have to send ACK, also check memory
         leak in this case
        return ICF_SUCCESS;
    }*/
    /* update bitmask to indicate that now received response is 
       a final response */
    p_ssa_ctxt->bitmask |= ICF_FINAL_RESP_RECEIVED;

    if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                (*ppCallObj)->pUacTransaction->pSipMsg, &resp_code,
                pErr))
    {
        ret_val = ICF_FAILURE;
        sdf_ret_val = Sdf_co_fail;
    }
    else if ( Sdf_en_reInviteFinalResponseReceived == 
            (*ppCallObj)->pCallInfo->dState)
    {
        /* 
         * Now send an ACK message to peer
         */
        /* Specify the stack options */
        options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        if (Sdf_co_fail == sdf_ivk_uaFormRequest(
                    (Sdf_ty_s8bit *)"ACK",(*ppCallObj), Sdf_co_null,
                    pErr))
        {
            ret_val = ICF_FAILURE;
            sdf_ret_val = Sdf_co_fail;
        }
        else if(ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),

                    (*ppCallObj)->pUacTransaction->pSipMsg,
                    &options,pErr))
        {
            ret_val = ICF_FAILURE;
            /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
            sdf_ret_val = Sdf_co_fail;
        }
        /*
         * Even if there is a failure in sending the ACK
         * SSA should be indicating the receipt of this failure
         * response to SC,hence not checking ret_val here 
         */
        if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                    resp_code,
                    &((((icf_nw_inc_media_change_resp_st*)
                                (p_ssa_pdb->p_internal_msg->p_msg_data))
                            ->reason_type))))
        {
            ret_val = ICF_FAILURE;
            sdf_ret_val = Sdf_co_fail;
        }
        else
        {
            icf_nw_inc_media_change_resp_st 
                *p_internal_msg_param = 
                (icf_nw_inc_media_change_resp_st *)
                (p_ssa_pdb->p_internal_msg->p_msg_data);

            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_NW_INC_MEDIA_UPDATE_RESP;
            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);
            ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask, \
                    ICF_NW_INC_MC_RESP_REASON)
            ((icf_nw_inc_media_change_resp_st*)
             (p_ssa_pdb->p_internal_msg->p_msg_data))
                ->status = ICF_FAILURE;
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
    }
#if 0
   else if ( 301 != resp_code && 302 != resp_code && 305 != resp_code &&
              300 != resp_code)
    {/* Non 302, 3xx response fo Invite*/
        if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                    resp_code,
                    &(((icf_nw_inc_reject_st*)
                            (p_ssa_pdb->p_internal_msg->p_msg_data))
                        ->reason_code)))
        {
            ret_val = ICF_FAILURE;
            sdf_ret_val = Sdf_co_fail;
        }
        else
        {
            /*We will send a IncCallReject API to CM*/
            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_NW_INC_CALL_REJECT;
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
        /* Fix for 14933 */
        p_ssa_pdb->p_glb_pdb->p_call_ctx->inc_response_code = resp_code;
    }
	else
 if ( 301 == resp_code || 302 == resp_code || 305 == resp_code || 300 == resp_code)
#endif
 if ( (300 <= resp_code) && (399 >= resp_code) )
   {
		/*Block for handling of call redirection:302 for an INVITE*/

        icf_nw_inc_call_redirect_st 
            *p_call_redirect = 
            (icf_nw_inc_call_redirect_st *)
            (p_ssa_pdb->p_internal_msg->p_msg_data);

        /*Fix for SPR: 19788*/
        /*Free the diversion header if already present in call context*/
        if ((ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx) &&
                (ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx->
                 p_diversion_header_list))
        {
            if(ICF_FAILURE == icf_ssa_clear_div_info(p_ssa_pdb))
        {
                ICF_PRINT(((icf_uint8_t *)"\nFailed 3xx handling while clearing div info."));
            ret_val = ICF_FAILURE;
            sdf_ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return sdf_ret_val;
        }
        }
        
        p_call_redirect->bitmask = ICF_NULL;

        if ( ICF_FAILURE == icf_ssa_populate_div_hdr(p_ssa_pdb,
                    (*ppCallObj)->pUacTransaction->pSipMsg))
        {
            ICF_PRINT(((icf_uint8_t *)"\nFailure in 3xx handling in diverrioninfo retrieval"));
            p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list = 
                ICF_NULL;
			sdf_ret_val = Sdf_co_fail;
        }
        else
        {
            if (ICF_NULL == 
                    p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list)
            {
                /* This means that no Diversion header was present */
                ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_diversion_info_st),
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_glb_pdb->p_call_ctx->
                                            p_diversion_header_list,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)

                p_div_hdr_list = 
                    p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list;

                p_div_hdr_list->no_of_diversion_data = 0;
                p_div_hdr_list->no_of_hops = 0;
                p_div_hdr_list->p_contact_addr = ICF_NULL;
                p_div_hdr_list->p_contact_name = ICF_NULL;
                p_div_hdr_list->p_diversion_data = ICF_NULL;

            }
            else
            {
            
                p_div_hdr_list = 
                    p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list;
            }

                /* Get the Contact address and fill in the call context
                 * Check for diversion headers present in the message
                 * If hop count ( redirect counter ) is there set it, other
                 * wise set to default configured value from CFG.*/
                if (Sdf_co_fail == sdf_ivk_uaGetCommonInfoFromCallObject(
                            *ppCallObj, &p_common_info, pErr))
                {
                    ret_val = ICF_FAILURE;
                    sdf_ret_val = Sdf_co_fail;
                }

                else if (Sdf_co_fail == sdf_listSizeOf(&(p_common_info->slContact),
                            &size, pErr))
                {
                    ret_val = ICF_FAILURE;
                    sdf_ret_val = Sdf_co_fail;
                }
                /*We are handling only one contact address */
                else if (Sdf_co_fail == sdf_listGetAt(&(p_common_info->slContact), 0,                      (Sdf_ty_pvoid *)&p_contact_hdr, pErr))
                {
                    ret_val = ICF_FAILURE;
                    sdf_ret_val = Sdf_co_fail;
                }
                else if(SipFail == sip_getAddrSpecFromContactHdr(
                                    p_contact_hdr,
                                    &p_contact_spec,

                                    (SipError *)&(pErr->errCode)))

                {
                    ret_val = ICF_FAILURE;
                    sdf_ret_val = Sdf_co_fail;
                }
                else if (SipFail == sip_getDispNameFromContactHdr(
                                        p_contact_hdr,
                                        (SIP_S8bit **)(&p_contact_disp_name),
                                        (SipError *)&(pErr->errCode)))
                {
                    if (E_NO_EXIST != (SipError)(pErr->errCode))
                    {
                        ret_val = ICF_FAILURE;
                        sdf_ret_val = Sdf_co_fail;
                    }
                }
                if (ICF_SUCCESS == ret_val)
                {
                    ICF_MEMGET(
                        p_ssa_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_address_st),
                        ICF_MEM_COMMON,
                        p_div_hdr_list->p_contact_addr,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode,
                        ret_val)
                }
                if ((ICF_SUCCESS == ret_val) && 
                        ((SipAddrSipUri == p_contact_spec->dType) || 
                         (SipAddrSipSUri ==p_contact_spec->dType)))
                {
                 if(SipAddrSipUri == p_contact_spec->dType)
                    {
                        p_div_hdr_list->p_contact_addr->addr_type = 
                           ICF_ADDRESS_PLAN_SIP_URI;
                    }
                    else 
                    {
                        p_div_hdr_list->p_contact_addr->addr_type = 
                           ICF_ADDRESS_PLAN_SIPS_URI;
                    }

                    /* fill the contact_addr */
                    p_div_hdr_list->p_contact_addr->addr_type = 
                        ICF_ADDRESS_PLAN_SIP_URI;
					if(ICF_NULL != p_contact_spec->u.pSipUrl->pUser)
					{
							icf_port_strcpy(p_div_hdr_list->p_contact_addr->
											addr_val.str, (icf_uint8_t *)p_contact_spec->u.pSipUrl->pUser);
							icf_port_strcat(p_div_hdr_list->p_contact_addr->
											addr_val.str,(icf_uint8_t *)"@");
							icf_port_strcat((icf_uint8_t *)p_div_hdr_list->p_contact_addr->
											addr_val.str, (icf_uint8_t *)p_contact_spec->u.pSipUrl->pHost);
					}
					else
					{
							icf_port_strcpy(p_div_hdr_list->p_contact_addr->
											addr_val.str,(icf_uint8_t *) p_contact_spec->u.pSipUrl->pHost);
					}
					
                    if ( ICF_NULL != p_contact_spec->u.pSipUrl->dPort)
                    {
                        icf_uint8_t  p_contact_port[6] = "\0";
                        icf_port_strcat(p_div_hdr_list->p_contact_addr->
                                addr_val.str,(icf_uint8_t *)":");
                        icf_port_sprintf(
                                (icf_int8_t *)p_contact_port,"%d",
                                *(p_contact_spec->u.pSipUrl->dPort));
                        icf_port_strcat(p_div_hdr_list->p_contact_addr->
                                addr_val.str, p_contact_port);
                    }
                    p_div_hdr_list->p_contact_addr->addr_val.str_len = 
                        (icf_uint16_t )icf_port_strlen(p_div_hdr_list->p_contact_addr->
                                            addr_val.str);
	
					if(ICF_SUCCESS == icf_ssa_check_for_param_in_sipurl(
						p_contact_spec->u.pSipUrl,(icf_uint8_t *)"transport",param_str,p_ssa_pdb->p_ecode))
					{
						if ('\0' != param_str[0])
						{
							icf_port_strcat(p_div_hdr_list->p_contact_addr->
                                addr_val.str, (icf_uint8_t *)";");
							icf_port_strcat(p_div_hdr_list->p_contact_addr->
                                addr_val.str, param_str);
						}
					}
                    /* fill the contact_name */
                    if (ICF_NULL != p_contact_disp_name)
                    {
                        ICF_MEMGET(
                                p_ssa_pdb->p_glb_pdb,
                                ICF_PORT_SIZEOF(icf_name_st),
                                ICF_MEM_COMMON,
                                p_div_hdr_list->p_contact_name,
                                ICF_DONOT_RET_ON_FAILURE,
                                p_ssa_pdb->p_ecode,
                                ret_val)

                        if (ICF_SUCCESS == ret_val)
                        {
                            icf_port_strcpy(
                                    p_div_hdr_list->p_contact_name->str,
                                    p_contact_disp_name);
                            p_div_hdr_list->p_contact_name->str_len = 
                                (icf_uint16_t )icf_port_strlen(
                                        (icf_uint8_t *)p_div_hdr_list->p_contact_name->str);
                        }
                    } /* if (ICF_NULL != p_contact_disp_name) */
                }
                else if ((ICF_SUCCESS == ret_val) && 
                        (SipAddrSipUri != p_contact_spec->dType))
                {
                    icf_uint8_t      *p_temp= ICF_NULL, 
                                        p_temp2[ICF_MAX_STR_LEN] = "\0",
                                        str[ICF_MAX_STR_LEN] = "\0";
                    
                    p_div_hdr_list->p_contact_addr->addr_type = 
                        ICF_ADDRESS_PLAN_TEL_URI;
                    
                    icf_port_strcpy(str, (icf_uint8_t *)p_contact_spec->u.pUri);
                    /* SPR14492 */
                    if (icf_port_strchr((icf_uint8_t *)str, ':'))
                    {
          
                    p_temp = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)str,":");
                    p_temp = (icf_uint8_t *)icf_port_strtok(0,":");
                     if(ICF_NULL != p_temp) 
                    icf_port_strcpy(p_temp2, p_temp);
                    if (' ' == p_temp2[0])
                    {
                        p_temp = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)p_temp2, " ");
                    }
                   if(ICF_NULL != p_temp)

                   {
                       icf_port_strcpy(p_div_hdr_list->p_contact_addr->
                               addr_val.str, p_temp);
                   } 
                    }
                    p_div_hdr_list->p_contact_addr->addr_val.str_len = 
                        (icf_uint16_t )icf_port_strlen(p_div_hdr_list->p_contact_addr->
                                            addr_val.str);
					
					/* fill the contact_name */
                    if (ICF_NULL != p_contact_disp_name)
                    {
                        ICF_MEMGET(
                                p_ssa_pdb->p_glb_pdb,
                                ICF_PORT_SIZEOF(icf_name_st),
                                ICF_MEM_COMMON,
                                p_div_hdr_list->p_contact_name,
                                ICF_DONOT_RET_ON_FAILURE,
                                p_ssa_pdb->p_ecode,
                                ret_val)

                        if (ICF_SUCCESS == ret_val)
                        {
                            icf_port_strcpy(
                                    p_div_hdr_list->p_contact_name->str,
                                    p_contact_disp_name);
                            p_div_hdr_list->p_contact_name->str_len = 
                                (icf_uint16_t )icf_port_strlen(
                                        p_div_hdr_list->p_contact_name->str);
                        }
                    } /* if (ICF_NULL != p_contact_disp_name) */
                }/* tel uri */
                if ( ICF_FAILURE != ret_val)
                {
                    /*Prep to send message to CM*/
                    ICF_SSA_SET_COMMON_BITMASK(p_call_redirect->bitmask, \
                            ICF_NW_INC_CALL_REDIRECT_DIV_INFO)
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_NW_INC_CALL_REDIRECT;
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                }
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error in handling Contact address for 302"));
                    ICF_SSA_FREE_DIV_DATA_LIST(p_ssa_pdb, \
                            (p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list))
                    if (ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx->
                                        p_diversion_header_list->p_contact_name)
                    {
                        ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,\
                            (p_ssa_pdb->p_glb_pdb->p_call_ctx->
                                p_diversion_header_list->p_contact_name),\
                            ICF_MEM_COMMON,\
                            p_ssa_pdb->p_ecode,\
                            ret_val)
                    }
                    if (ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx->
                                        p_diversion_header_list->p_contact_addr)
                    {
                        ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,\
                            (p_ssa_pdb->p_glb_pdb->p_call_ctx->
                                p_diversion_header_list->p_contact_addr),\
                            ICF_MEM_COMMON,\
                            p_ssa_pdb->p_ecode,\
                            ret_val)
                    }
                    ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,\
                            (p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list),\
                            ICF_MEM_COMMON,\
                            p_ssa_pdb->p_ecode,\
                            ret_val)
                }
            sip_freeSipAddrSpec(p_contact_spec);
            sdf_ivk_uaFreeCommonInfo(p_common_info);
        }
    }/*End block Call redirected*/
    else 
    {/* Non 302, 3xx response fo Invite*/
        if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                    resp_code,
                    &(((icf_nw_inc_reject_st*)
                            (p_ssa_pdb->p_internal_msg->p_msg_data))
                        ->reason_code)))
        {
            ret_val = ICF_FAILURE;
            sdf_ret_val = Sdf_co_fail;
        }
        else
        {
            /*We will send a IncCallReject API to CM*/
            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                ICF_NW_INC_CALL_REJECT;
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
        /* Fix for 14933 */
        p_ssa_pdb->p_glb_pdb->p_call_ctx->inc_response_code = resp_code;
    }    
#ifdef ICF_STAT_ENABLE
    icf_ssa_invite_resp_stats(p_ssa_pdb,resp_code,0);
#endif

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return sdf_ret_val;
}
#ifdef SDF_IM
Sdf_ty_retVal sdf_cbk_uaGetIpFromImUrl (
										Sdf_st_callObject *pCallobj, 
										ImUrl *pImUrl, 
										Sdf_st_transportInfo *pDestInfo, 
										Sdf_st_error *pErr)
{
/* To remove warning */
(void)pImUrl;
(void)pCallobj;
(void)pErr;
(void)pDestInfo;


	return Sdf_co_fail;
}
#endif




/*********************************************************
** FUNCTION: icf_ssa_transaction_Completed_for_info
**
** DESCRIPTION: This Function handles All response received for 
**              INFO Request Sent to Network
**
**********************************************************/
Sdf_ty_retVal icf_ssa_transaction_Completed_for_info(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr)
{
    SipMessage                          *p_message = ICF_NULL;
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    icf_ssa_ctxt_st						*p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st						*p_ssa_pdb = ICF_NULL;
    icf_boolean_t                           err_resp = ICF_TRUE;
    icf_nw_inc_info_resp_st				*p_info_resp = ICF_NULL;

    /* To remove warning */
    (void )ppCallObj;
    (void) pErr;

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);

    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    p_message = pOverlapTransInfo->pSipMsg;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_message))


        p_info_resp = (icf_nw_inc_info_resp_st*)p_ssa_pdb->p_internal_msg->p_msg_data;
    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_INFO_RESP;
    p_info_resp->response_code = resp_code;


    if(200 <= resp_code && 299 >= resp_code)
    {
                err_resp = ICF_FALSE;
#ifdef ICF_LOAD_STAT
        p_ssa_pdb->p_glb_pdb->load_data.info_success++;
#endif
        p_info_resp->status = ICF_TRUE;

        /* Changes done for Message Body Headers Start :
         */
        /* Extract the message bodies from SIP message.
         */
        if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message(p_ssa_pdb,\
                    p_message,\
                    SipMessageResponse,&(p_ssa_pdb->p_glb_pdb->p_call_ctx->\
                        body_list)))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
            ret_val = ICF_FAILURE;
        }
        /* Changes done for Message Body Headers End :
         */
        else if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                    p_ssa_pdb,ICF_INT_MODULE_CC,
                    (icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
                    p_message,
                    p_ssa_pdb->p_ecode) )
        {
            ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));             
            ret_val = ICF_FAILURE;
        }

    }
    else
    {
        if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                    resp_code,&(p_info_resp->error_cause)))
        {
            ret_val = Sdf_co_fail;
        }
        else
        {
            p_info_resp->status = ICF_FALSE;
        }
    }
    if ( Sdf_co_fail != ret_val)
    {
        /* Send response to CM */
        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
    }
#ifdef ICF_ERROR_ENABLE
   if(ICF_TRUE == err_resp)
   {
      p_ssa_pdb->resp_code = resp_code;
      ICF_SSA_SET_ECODE(p_ssa_pdb,"INFO",ICF_ERROR_FAILURE_RESPONSE)
   }
#endif
    return ret_val;
}


#ifdef SDF_TLS

/**********************************************************
 ** FUNCTION:  sdf_cbk_uaGetPrivateKeyPassword
 **
 ** DESCRIPTION: This function will set the password in the output parameter
 **              buf and return the length of the password. While constructing
 **              the SSL_CTX, UATK would have set the password as the callback
 **              userdata using the API SSL_CTX_set_default_passwd_cb_userdata.
 **              The same userdata will be provided as an input argument to
 **              this callback function.
 **
 **********************************************************/
    Sdf_ty_s32bit sdf_cbk_uaGetPrivateKeyPassword
(Sdf_ty_s8bit  *buf,
 Sdf_ty_s32bit num,
 Sdf_ty_s32bit rwflag,
 Sdf_ty_pvoid  pUserData)
{
    /* To avoid warnings. */
    (void)num;
    (void)rwflag;

    Sdf_mc_strcpy(buf, (Sdf_ty_s8bit *)pUserData);
    return (Sdf_mc_strlen((Sdf_ty_s8bit *)buf));
}


/**********************************************************
** FUNCTION:  sdf_cbk_uaVerifyCeritificate
**
** DESCRIPTION: The SSL layer would have done its part of verifications and
**              set the result in preverifyOk. Extra verifications can be
**              done by the application in this function.
**
**********************************************************/
Sdf_ty_s32bit sdf_cbk_uaVerifyCeritificate
        (Sdf_ty_s32bit  preverifyOk,
         X509_STORE_CTX *ctx)
{
        /* To avoid warnings. */
        (void)ctx;


        /* Do application level certificate verifications, if any. */

        return preverifyOk;
}

#endif /* SDF_TLS */

/*********************************************************
 ** FUNCTION: sdf_cbk_uaForkedRespReceived
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **                              Forked Response to an INVITE
 **
 **********************************************************/
Sdf_ty_retVal sdf_cbk_uaForkedRespReceived(
        Sdf_st_callObject *pForkCallObj,
        Sdf_st_callObject *pOrigCallObj,
#ifdef ICF_WITH_MUATK_2_2
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
#endif
        Sdf_st_eventContext *pEventContext,
        Sdf_st_error *pErr)
{
    Sdf_ty_u16bit 	      dRespCode = 0;
    Sdf_ty_retVal		  ret_val = Sdf_co_success;
    icf_ssa_ctxt_st       *p_ssa_ctxt = ICF_NULL;
    icf_call_ctx_st       *p_call_ctxt = ICF_NULL;
    icf_ssa_pdb_st		  *p_ssa_pdb = ICF_NULL;
    icf_ssa_ctxt_st		  *p_temp_ssa_ctxt = ICF_NULL;
    icf_error_t            p_ecode = 0;
    icf_return_t           icf_ret_val = 0; 
    SipMessage		        *pMessage =Sdf_co_null;
    SipOptions              options;
    Sdf_st_overlapTransInfo *p_overlaap_txn = Sdf_co_null;
    icf_uint32_t          size=ICF_NULL;

   /* Extract ssa context from original call object for removing it */
   p_ssa_ctxt = (icf_ssa_ctxt_st *)(pOrigCallObj->pAppData->pData);
   p_temp_ssa_ctxt = p_ssa_ctxt;

   /* Extract the call context from the SSA context */
   p_call_ctxt = (icf_call_ctx_st *)(p_ssa_ctxt->p_call_ctxt);

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pForkCallObj->pAppData->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    /* update new ssa context with original call context value */

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    
    (void)sdf_listSizeOf(&(pOrigCallObj->pCommonInfo->slInvRoute), &size, pErr);

    if((Sdf_co_true == pOrigCallObj->pCommonInfo->dPreloadedSet)&&
        (0 != size))
    {
        if (sdf_fn_uaCloneSipHeadersList(
            &(pForkCallObj->pCommonInfo->slInvRoute),
            &(pOrigCallObj->pCommonInfo->slInvRoute), 
            pErr) == Sdf_co_fail)
        {
            return Sdf_co_fail;
        }
        else
        {
            pForkCallObj->pCommonInfo->dPreloadedSet = Sdf_co_true;
        }
    }    
    if(ICF_FINAL_RESP_RECEIVED != (p_temp_ssa_ctxt->bitmask &
            ICF_FINAL_RESP_RECEIVED))
     {       
        p_ssa_ctxt->p_call_ctxt = p_call_ctxt;
        p_ssa_ctxt->p_call_ctxt->p_sip_ctx = p_ssa_ctxt;

        /* update bitmask to indicate that now received response is 
        a forked response */
        p_call_ctxt->common_bitmask = p_call_ctxt->common_bitmask |
                                         ICF_FORKED_RESP;
       
        /********** Changes for Forking & Multiple Answer Start (Rel7.0) *********/

        /* Checks if it is a forked response from a different UAS (compared 
           to previously received responses, if so, the global forked_resp_counter
           is incremented and assigned to the ssa context's forked response counter */ 

        if(ICF_NULL == p_ssa_ctxt->forked_resp_counter)
        {
            /* The forked_resp_counter which is initially 0 is incremented for 
               every new response from different UAS and aasigned to every 
               ssa_ctxt */

            p_call_ctxt->forked_resp_counter++;
            p_ssa_ctxt->forked_resp_counter = p_call_ctxt->forked_resp_counter;
        }
 
        /*********** Changes for Forking & Multiple Answer End (Rel7.0) **********/
 
        /* Add new ssa context in global context array */
        icf_ret_val = icf_ssa_add_callid_callobj_map(p_ssa_pdb, p_ssa_ctxt,&p_ecode);        
        HSS_LOCKEDINCREF(pForkCallObj->dRefCount);

        if (ICF_SUCCESS == icf_ret_val )
        {
            /* Extract the response code from the message */
            ret_val = sdf_ivk_uaGetRespCodeFromSipMessage(\
                            pForkCallObj->pUacTransaction->pSipMsg,\
        	    	        &dRespCode, pErr);
        }
        else
        {
            ret_val = Sdf_co_fail;
        }
    
        if(Sdf_co_success == ret_val) 
        {
             if((dRespCode >= 100) && (dRespCode <= 199))
                {
                    pMessage = pForkCallObj->pUacTransaction->pSipMsg;
                    if (sdf_fn_uaIsMessageRPR(pMessage, pErr) == 0)
	                {
                        ret_val = sdf_cbk_uaRPRReceived(&pForkCallObj, pOverlapTransInfo, \
						                                pEventContext, pErr);
                    }
                    else
                    {
                         ret_val = sdf_cbk_uaInProgress(&pForkCallObj,pOverlapTransInfo,
                                                        pEventContext,pErr);
                    }
                }
            else if( (dRespCode >= 200) && (dRespCode <= 299) )
             {
                 ret_val = sdf_cbk_uaCallAccepted( &pForkCallObj,pEventContext,pErr);
             }
#if 0
            else if( (dRespCode >= 300) && (dRespCode <= 399) )
            {
                 ret_val = sdf_cbk_uaCallRedirected (&pForkCallObj, pEventContext, pErr);
            }
           else if(dRespCode >= 400)
            {
                 ret_val = sdf_cbk_uaCallFailed(&pForkCallObj,pEventContext, pErr);
            }
#endif
        }
    }
    else
    {
        /* update bitmask to indicate that now received response is 
        a forked response */
        
        /* Add new ssa context in global context array */
        HSS_LOCKEDINCREF(pForkCallObj->dRefCount);

        /* Extract the response code from the message */
        ret_val = sdf_ivk_uaGetRespCodeFromSipMessage(\
                            pForkCallObj->pUacTransaction->pSipMsg,\
        	    	        &dRespCode, pErr);
    
        if(Sdf_co_success == ret_val) 
         {
            /* Sending ACK */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            if (Sdf_co_fail == sdf_ivk_uaFormRequest(
                          (Sdf_ty_s8bit *)"ACK",(pForkCallObj), Sdf_co_null,
                                pErr))
            {
               ret_val = Sdf_co_fail;
            }
            else if(ICF_FAILURE ==
                     icf_ssa_send_to_peer(p_ssa_pdb,(pForkCallObj),
                                  (pForkCallObj)->pUacTransaction->pSipMsg,
                                  &options,pErr))
            {
                  ret_val = Sdf_co_fail;
            }
            else if(dRespCode == 200)
            {
               if (Sdf_co_fail == sdf_ivk_uaStartTransaction(
                    p_ssa_ctxt->p_call_obj,&p_overlaap_txn,"BYE", pErr))
               {
                   ret_val = ICF_FAILURE;
               }
               else if (Sdf_co_fail == sdf_ivk_uaTerminateCall(
                    p_ssa_ctxt->p_call_obj,pErr))
               {
                  /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
                  /* Clear the transaction and unlock call object */
                  sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                          p_overlaap_txn,"BYE",Sdf_en_uacTransaction);
                  ret_val = ICF_FAILURE;
               }
               else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_ssa_ctxt->p_call_obj,
                    p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg,
                    &options,pErr))
               {
                    sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                        p_overlaap_txn,"BYE",Sdf_en_uacTransaction);
                    ret_val = ICF_FAILURE;
               }
               else
               {
                   sdf_ivk_uaEndTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlaap_txn, "BYE", pErr);
                    /* set bitmask for BYE send which will be used in 
                    CallTerminated callback to avoid control going to CC in this case */
                   p_ssa_ctxt->bitmask_1 |= ICF_BYE_ALREADY_SEND;
                   icf_ret_val = icf_ssa_add_callid_callobj_map(p_ssa_pdb, 
                            p_ssa_ctxt,&p_ecode);
               }
            }
         }   
     }    

#ifdef ICF_STAT_ENABLE
    icf_ssa_invite_resp_stats(p_ssa_pdb,dRespCode,0);
#endif

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function entry trace */
    
    return ret_val;
}  


