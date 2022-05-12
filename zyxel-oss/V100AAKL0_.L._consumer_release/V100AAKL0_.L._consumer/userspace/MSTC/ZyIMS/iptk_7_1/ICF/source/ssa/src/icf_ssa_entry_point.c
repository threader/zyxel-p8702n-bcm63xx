/******************************************************************************
 * 
 * 
 *FILE NAME:   icf_ssa_entry_point.c
 * 
 * DESCRIPTION: This file contains the entry point function for SSA.
 *              
 * REVISION HISTORY:    
 *   Date           Name              Reference       Comments
 *   16-Jan-2005    Aashish Suchdev   LLD           Initial Version
 *
 *   24-May-2005    Umang Singh       SPR 7551      Failure handling for decode
 *                                                  failures  
 *   22-JUN-2005    Umang Singh       SPR 7665             
 *   09-SEP-2004    Ashutos Rath	  ICF UT FIX    In Entry Point get the line
 *                                                  from SIC context if the 
 *                                                  Incoming internal message 
 *                                                  belongs to SIC
 *	01-DEC-2005		Mohan Dhawan	  ICF 4.0 LLD	merged for TCP UT
 *   13-Dec-2005    Aman Arora        ICF 4.0 CDD   Coding for IPSec support
 *	22-Feb-2005		Amit Sharma                     Added code for auth support
 *  02-Mar-2006     Amit Sharma                     ICF merging with ICF2.0
 * 30-Mar-2006      Anuradha Gupta	   ICF 5.0             Call Modify Feature changes
 * 30-Jun-2006      Umang Singh        Rel 5.2         TLS support
 * 06-Sep-2006     Umang Singh         SPR 12855	Parsing of Require hdr for 100rel
 * 06-Sep-2006     Umang Singh         SPR 12863
 * 05-Oct-2006     Aman Arora         ICF Rel 6.1.1. licensing related changes
 *
 * 27-Nov-2006     Deepti Goyal        ICF Rel 6.2    NAT Feature Support
 * 27-Nov-2006     Umang Singh       SPR 13927
 * 11-Dec-2006	   Deepti Goyal	     SPR 14029	      Handling of rport_support updated.
 *
 * 12-Dec-2006     Deepti Goyal      SPR 14036        Get the Via Parameters
 *                                                    in case of Incoming INVITE
 * 04-Jan-2007     Amit Mahajan                       INFO confirmation changes.
 * 05-Jan-2007     Umang Singh       SPR 13850
 * 05-Jan-2007     Umang Singh       SPR 14467 
 * 01-May-2007     Deepti Goyal      SPR 14954        Fix for TCP.
 * 14-Mar-2007     Deepti Goyal      SPR 13937        Handling of incorrect messages
 *                                                    on TCP/TLS.
 * 16-Mar-2007     Deepti Goyal      SPR 14604       Rport in resposne messages.
 * 20-Jun-2007     Tarun Gupta        SPR 15150 In the function icf_ssa_handle_successful_decode
 *                                                a bitmask has been set in ssa pdb.
 * 05-Jul-2007     Sudhesh Singh      Rel 7.0    Persistent TCP connection
 * 24-Aug-2007     Deepti Goyal      SPR 15937        Store the listening IP/Port
 *                                                    of server in DNS+TCP.
 * 04-OCT-2007     Abhishek Dhammawat SPR16358   Modified 
 *                                               icf_ssa_handle_decode_failure
 * 06-OCT-2007     Abhishek Dhammawat SPR16769   Modified 
 *                                               icf_ssa_handle_successful_decode 
 * 20-OCT-2007     Abhishek Dhammawat SPR17154   Modified
 *                                               icf_ssa_handle_successful_decode
 * 23-Dec-2007     Abhishek Dhammawat SPR17855   Modified 
 *                                               icf_ssa_handle_successful_decode
 *                                               icf_ssa_handle_internal_api
 * 25-Jun-2008     Rajiv Kumar        SPR - 18722        IPTK not compilable with gcc
 *                                                       version 4.2.3                                               
 *
 * 12-Jan-2009     Vinay Jindal       SPR - 19254        Argument of 'sdf_ivk_uaIPTKCheckMessageLength'
 *                                                       corrected
 * 03-Aug-09	   Aman Aggarwal	 CSR 1-7576668		Fix for TLS connection initiated by Peer response
 * 04-Jan-2010      Pavneet Bajaj     SPR 20251         Fix for CSR 7945729
 * Copyright 2009 (c) Aricent.
 ******************************************************************************/
/*Extif inclusions*/
#include "icf_port.h"

/*Common H  inclusions*/
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_internal_api_id.h"
#include "icf_common_prototypes.h"
#include "icf_ecode.h"

/*Stack/UATK inclusions*/
#include "stack_headers.h"
#include "uatk_headers.h"

/*SSA inclusions*/
#include "icf_ssa_types.h"
#include "icf_ssa_defs.h"
#include "icf_ssa_prototypes.h"
#include "icf_ssa_macro.h"

/* SDF inclussions */
#include "sdf.h"

/*DBM inclusions*/
#include "icf_dbm_prototypes.h"

/*CFG inclusions*/
#include "icf_cfg_defs.h"

#ifdef ICF_SIGCOMP_SUPPORT
#include "icf_sigsa_defs.h"
#endif


#ifdef ICF_DNS_LOOKUP_ENABLED
#include "ares.h"
#include "ares_dns.h"
#endif

#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif


/* Global defintion of persistent global pdb */
icf_glb_pdb_st        *p_persistent_glb_pdb;

extern Sdf_ty_retVal sdf_ivk_uaIPTKCheckMessageLength(Sdf_ty_s8bit *pString, 
		Sdf_ty_u32bit dLen, Sdf_ty_u32bit dOption,
        Sdf_ty_u32bit *pResult, Sdf_st_error *pError);

/*******************************************************************************
 *  
 *******************************************************************************/
#ifdef ICF_SIGCOMP_SUPPORT

extern icf_return_t icf_sigsa_ProcessNetworkMessage(
                icf_uint8_t            *p_CompMsg,
                icf_uint16_t           dMsgLen,
                icf_uint8_t            enTransport,
                icf_uint16_t           *p_processed_bytes,
                icf_uint8_t            **p_p_next_msg,
                icf_void_t             *p_Appdata,
                icf_error_t            *p_ecode);
#endif
icf_return_t icf_ssa_process_nw_msg(
        INOUT icf_internal_msg_st    *p_internal_msg);


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of SSA
 *                  This entry point is called by MRM when it has a message 
 *                  to be processed by SSA.
 *                  The message can be:
 *                      - a SIP message from Network
 *                      - a indication from TCP stack related to some 
 *                          connections
 *                      - timer events    
 *                  This entry point can also be called by CC/CFG and SLPs
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_mesg(
        INOUT icf_internal_msg_st        *p_internal_msg)



{
    /*
     * This entry point is called by MRM when it has a message to be
     * processed by SSA. The message can be:
     * - a SIP message from Network
     * - a indication from TCP stack related to some connections

  * This entry point can also be called by CC/CFG and SLPs

	*/


    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb,ICF_INT_MODULE_SSA)
    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)        

    /*
     * SSA can receive triggers from SIP peers whose messages
     * will be delievered through MRM or messages from internal
     * modules which will be preocessed through 
     * icf_ssa_process_int_msg
     */
    if (ICF_MSG_FROM_MRM ==
            p_internal_msg->msg_hdr.msg_id)
    {
        icf_msg_st   *p_recv_msg = 
            p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg;

        if (ICF_TIMER_MSG == 
                p_recv_msg->hdr.api_id)
        {
            ret_val = icf_ssa_process_timer_expiry(p_internal_msg);
        }
        else if (ICF_NW_API_MSG_FROM_PEER == 
                p_recv_msg->hdr.api_id)
        {
            /* WE are not checkin ghte ret_val of process_sip_msg here
             * as a failure in nw SIP message processing cannot be handled 
             * by MRM.*/
            icf_ssa_process_nw_msg(p_internal_msg);
        }
        else if (p_recv_msg->hdr.api_id <= ICF_NW_API_CLOSE_CONN_IND)
        {
            /*TCP conn related Event*/
            icf_ssa_handle_tcp_event_frm_mrm(p_internal_msg);
        }
        else
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure:InvalidMessageIdFromMRM"));
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        ret_val = icf_ssa_process_int_msg(p_internal_msg);
    }
    /*
     * If no ecode is set ,then set the error code as
     * generic error
     */
    if ((ICF_FAILURE == ret_val) &&
            (ICF_ECODE_NO_ERROR == p_internal_msg->msg_hdr.ecode))
    {
        p_internal_msg->msg_hdr.ecode = ICF_ECODE_GENERIC_ERROR;
    }
    ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
    ICF_RESET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_handle_internal_api
 *
 * DESCRIPTION:     This just invokes appropriate handlers based on IDs
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_ssa_handle_internal_api(
        INOUT icf_ssa_pdb_st     *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_internal_msg_st  *p_internal_msg = p_ssa_pdb->p_internal_msg;
    icf_uint16_t          msg_id = p_internal_msg->msg_hdr.msg_id;
    /* Added null check for klocwork warning removal */
    if(ICF_NULL != p_ssa_pdb->p_glb_pdb)
    {   
        ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    }        
    /*To set outgoing direction*/
    p_ssa_pdb->is_otg_msg = ICF_TRUE;

    switch(msg_id)
    {
        case ICF_FORCEFUL_CALL_CLEAR_IND :
            {
                 /* In case of forking, delete first delete all
                 * other ssa contexts before clearing final ssa
                 * context*/
                ret_val =
                    icf_ssa_del_all_other_call_obj(p_ssa_pdb);
                if(ICF_SUCCESS == ret_val)
                {
                    ret_val = 
                        icf_ssa_process_forceful_call_clear(p_ssa_pdb);
                }
                break;
            }
        case ICF_CLEAR_REG_OBJECT :
            {
                ret_val = 
                    icf_ssa_process_clear_reg_obj(p_ssa_pdb);
                break;
            }
        case ICF_NW_OTG_REG_REQ :
            {
                ret_val = 
                    icf_ssa_process_otg_reg_req(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_SETUP :
            {
                ret_val = 
                    icf_ssa_process_call_setup(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_SETUP_ACK :
            {
                ret_val = 
                    icf_ssa_process_call_setup_resp(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_ALERT :
            {
                ret_val = 
                    icf_ssa_process_prov_response(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_ALERT_WITH_MEDIA :
            {
                ret_val = 
                    icf_ssa_process_prov_response(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_MEDIA_PARAM :
            {
                ret_val = 
                    icf_ssa_process_prov_response(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CONNECT :
            {
                ret_val = 
                    icf_ssa_process_connect(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CONNECT_ACK :
            {
                ret_val = 
                    icf_ssa_process_ack(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_REJECT :
            {
                ret_val = 
                    icf_ssa_process_reject(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_TERMINATE :
            {
                ret_val = 
                    icf_ssa_process_call_terminate(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_TERMINATE_RESP :
            {
                ret_val = 
                    icf_ssa_process_call_terminate_resp(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_REDIRECT :
            {
                ret_val = 
                    icf_ssa_process_redirect(p_ssa_pdb);
                break;
            }
#ifdef ICF_SERVICE_ENABLE

        case ICF_NW_OTG_MEDIA_UPDATE :
            {
                ret_val = 
                    icf_ssa_process_media_change(p_ssa_pdb);
                break;
            }
#endif

ICF_HANDLE_REINVITE_START

        case ICF_NW_OTG_CALL_MODIFY_REQ :
            {
                ret_val = 
                    icf_ssa_process_call_modify_req(p_ssa_pdb);
                break;
            }
ICF_HANDLE_REINVITE_END

#ifdef ICF_SESSION_TIMER
		case ICF_NW_OTG_SESSION_REFRESH_REQ :
		{
			if ((ICF_NULL == p_ssa_pdb->p_glb_pdb) || 
                       (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
            {
                ret_val = ICF_FAILURE; 
            } 
            else if ((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable))
            {
               ret_val = 
			     icf_ssa_process_session_refresh_req(p_ssa_pdb);
            }
            else
            {
                  ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Outgoing Session refresh req, but Session timer functionality is Disabled.\n")) 
            }
			break;
		}
		case ICF_CFG_SSA_SET_MIN_SE:
		{
        	if ((ICF_NULL == p_ssa_pdb->p_glb_pdb) || 
                       (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
            {
                 ret_val = ICF_FAILURE; 
            } 
            else if ((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable))
            {
                ret_val = icf_ssa_process_set_min_se(p_ssa_pdb);
            }
            else
            {
               ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Outgoing Session refresh req, but Session timer functionality is Disabled.\n")) 
            }
			break;
		}
#endif
        case ICF_NW_OTG_MEDIA_UPDATE_ABORT_REQ :
            {
                ret_val = 
                    icf_ssa_process_media_update_abort(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_ABORT :
            {
                ret_val = 
                    icf_ssa_process_abort(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_MEDIA_PARAM_PRACK_ACK :
            {
                ret_val = 
                    icf_ssa_process_prack_resp(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_ALERT_PRACK_ACK :
            {
                ret_val = 
                    icf_ssa_process_prack_resp(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_REJECT_ACK :
            {
                ret_val = 
                    icf_ssa_process_reject_ack(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_MEDIA_PARAM_PRACK :
            {
                ret_val = 
                    icf_ssa_process_prack(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_ALERT_PRACK :
            {
                ret_val = 
                    icf_ssa_process_prack(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_MEDIA_UPDATE_RESP :
            {
                ret_val = 
                    icf_ssa_process_media_change_resp(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_REFER :
            {
                ret_val = 
                    icf_ssa_process_otg_refer_req(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_REFER_RESP :
            {
                ret_val = 
                    icf_ssa_process_refer_resp(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_REFER_ABORT_REQ :
            /*The REFER transaction will be aborted, the callobj will remain*/
            {
                ret_val = 
                    icf_ssa_process_refer_abort_req(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_NOTIFY :
            {
                ret_val = 
                    icf_ssa_process_notify(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_NOTIFY_RESP :
            {
                ret_val = 
                    icf_ssa_process_notify_resp(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_REDIRECT_IND_PRACK :
            {
                ret_val = 
                    icf_ssa_process_prack(p_ssa_pdb);
                break;
            }

        case ICF_NW_OTG_CALL_REDIRECT_ACK :
            {
                ret_val = 
                    icf_ssa_process_reject_ack(p_ssa_pdb);
                break;
            }
        case ICF_NW_OTG_CALL_ABORT_RESP :
            {
                ret_val = 
                    icf_ssa_process_abort_resp(p_ssa_pdb);
                break;
            }

        case ICF_CFG_OPEN_NW_SERVERS :
            {
                ret_val = 
                    icf_ssa_process_open_nw_servers(p_ssa_pdb);
                break;
            }

        case ICF_CFG_CLOSE_NW_SERVERS :
            {
                ret_val = 
                    icf_ssa_process_close_nw_servers(p_ssa_pdb);
                break;
            }
        case ICF_NW_INC_OPTIONS_CFM:
            {
                ret_val = 
                    icf_ssa_process_options_resp (p_ssa_pdb);
                break;
            }
        case ICF_NW_OTG_INFO_REQ:
            {
                ret_val = 
                    icf_ssa_process_info (p_ssa_pdb);
                break;
            }
        case ICF_NW_INC_INFO_CFM:
            {
                ret_val = icf_ssa_process_info_cfm(p_ssa_pdb);
                break;
            }

ICF_SIC_START
		 case ICF_NW_OTG_SUBSCRIBE_REQ:
            {
                
                ret_val = icf_ssa_process_otg_subs_req(p_ssa_pdb);
                break;
            }

		 case ICF_NW_OTG_MESSAGE_REQ:
            {
                
                ret_val = icf_ssa_process_otg_msg_req(p_ssa_pdb);
                break;
            }

		 case ICF_NW_OTG_PUBLISH_REQ:
            {
                
                ret_val = icf_ssa_process_otg_publish_req(p_ssa_pdb);
                break;
            }

		 case ICF_NW_OTG_OPTIONS_REQ:
            {
                
                ret_val = icf_ssa_process_otg_options_req(p_ssa_pdb);
                break;
            }
		case ICF_NW_OTG_MESSAGE_RESP :
            {
                ret_val = 
                    icf_ssa_process_message_resp(p_ssa_pdb);
                break;
            }
		 case ICF_DELINK_APP_CTX:
            {
                ret_val = icf_ssa_process_delink_req(p_ssa_pdb);
                break;
            }
#ifdef ICF_IPSEC_ENABLED
         case ICF_SSA_SA_OPEN_SERVER:
            {
                ret_val = icf_ssa_open_sa_server(p_ssa_pdb);
                break;
            }
         case ICF_SSA_SA_CLOSE_SERVER:
            {
                ret_val = icf_ssa_close_sa_server(p_ssa_pdb);
                break;
            }
#endif
        case ICF_CFG_SSA_INITIATE_CLEAN_UP:
            {
                ret_val = icf_ssa_process_initiate_clean_up(p_ssa_pdb);
                break;
            }
#ifdef ICF_SESSION_TIMER
        case ICF_SSA_SESSION_TIMER_GET:
            {
              if ((ICF_NULL == p_ssa_pdb->p_glb_pdb) || 
                   (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
               {
                  ret_val = ICF_FAILURE; 
               } 
               else if ((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable))
               {
                  ret_val = icf_ssa_session_timer_get(p_ssa_pdb);
               }
               else
               {
                  ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Outgoing Session refresh req, but Session timer functionality is Disabled.\n")) 
               }
              break;
            }
#endif            
        case ICF_DETACH_CALL_CTX_WITH_SSA_CTX:
           {
               ret_val = icf_ssa_process_detach_call_ctx_frm_sip_ctx(p_ssa_pdb);
               break;
            }
        case ICF_NW_OTG_OPTIONS_ABORT_REQ :
           {
               ret_val = icf_ssa_process_options_abort_req(p_ssa_pdb);
                break;
           }

        default:
            {
                /*
                 * Raise a Minor Error here for unknown API ID
                 */
                ret_val = ICF_FAILURE ;
                break;
            }
    }
    /* Added null check for klocwork warning removal */
    if(ICF_NULL != p_ssa_pdb->p_glb_pdb)
    {   
       ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    }       
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_int_msg
 *
 * DESCRIPTION:     This is the entry pt.routine for internal messages
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_int_msg(
        INOUT icf_internal_msg_st    *p_internal_msg)
{
    icf_return_t             ret_val = ICF_SUCCESS;

    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;

    icf_ssa_pdb_st           ssa_pdb ;

    icf_uint8_t              count = 0;

    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;

    icf_rgm_context_st          *p_rgm_ctxt = ICF_NULL;
	icf_line_id_t              line_id = ICF_INVALID_LINE_ID;

    icf_uint8_t              p_transp_addr_str[100] = "";
    icf_transport_address_st *p_transport_address = ICF_NULL;
    Sdf_st_error             sdf_error;

    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_SSA_GET_GLB_PDB_FRM_INTRNL_MSG(p_glb_pdb,p_internal_msg)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Populate SSA PDB.This will be sent as pData in Event Context
     */
    icf_ssa_init_pdb(&ssa_pdb,p_glb_pdb);
    ssa_pdb.p_internal_msg      = p_internal_msg;
    ssa_pdb.p_ecode             = &(p_internal_msg->msg_hdr.ecode);

    /* For requests from RGM, get ssa ctxt pointer from payload */
    if ((ICF_CLEAR_REG_OBJECT == p_internal_msg->msg_hdr.msg_id) || 
            (ICF_NW_OTG_REG_REQ == p_internal_msg->msg_hdr.msg_id) 
#ifdef ICF_IPSEC_ENABLED
            || (ICF_SSA_SA_OPEN_SERVER == p_internal_msg->msg_hdr.msg_id)
            || (ICF_SSA_SA_CLOSE_SERVER == p_internal_msg->msg_hdr.msg_id)
#endif
            )
    {
        p_rgm_ctxt = (icf_rgm_context_st *)(p_internal_msg->p_msg_data);
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgm_ctxt))

        p_ssa_ctxt = (icf_ssa_ctxt_st *)(p_rgm_ctxt->p_ssa_ctxt);
            
        if ( ICF_NULL != p_ssa_ctxt)
        {
            p_ssa_ctxt->p_ssa_pdb = &ssa_pdb;
        }
    }
    /* For requests not from RGM, get ssa ctxt pointer from call ctx */
    else if ( ICF_NULL != p_glb_pdb->p_call_ctx)
    {
        p_ssa_ctxt = 
            (icf_ssa_ctxt_st*)(p_glb_pdb->p_call_ctx->p_sip_ctx);
        if ( ICF_NULL != p_ssa_ctxt)
        {
            p_ssa_ctxt->p_ssa_pdb = &ssa_pdb;
            /* Add the reverse mapping too */
            ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
        }
		if(ICF_FAILURE == icf_cfg_fetch_app_conf(p_glb_pdb,
							 p_glb_pdb->p_call_ctx->app_id,
							 &(ssa_pdb.p_app_conf),
							 ssa_pdb.p_ecode))
        {
            ICF_ALARM(((icf_uint8_t *)"[SSA] Not able set the applcation conf in the call context\n"));
        }
    }

    if ( ICF_NULL != p_ssa_ctxt)
    {
        if (( ICF_NULL != ((ICF_SSA_INVITE_PENDING | 
                        ICF_SSA_REG_PENDING | ICF_SSA_SUBS_PENDING) &
                    p_ssa_ctxt->bitmask))
                    && ((ICF_FORCEFUL_CALL_CLEAR_IND != 
                    p_internal_msg->msg_hdr.msg_id)
                    && (ICF_CLEAR_REG_OBJECT != 
                    p_internal_msg->msg_hdr.msg_id)))

        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!CallRelatedReq received while INVITE is\
                        yet to be sent to peer"));
            return ICF_FAILURE;
        }
    }

    /* 
     * If this is a registration related API,do not
     * fetch the line data as the call ctx is NULL
     * in this case
     */
    if ((ICF_CLEAR_REG_OBJECT != p_internal_msg->msg_hdr.msg_id) && 
            /*(ICF_NW_OTG_REG_REQ != p_internal_msg->msg_hdr.msg_id) &&*/
            (ICF_CFG_OPEN_NW_SERVERS != p_internal_msg->msg_hdr.msg_id) &&
            (ICF_CFG_CLOSE_NW_SERVERS != p_internal_msg->msg_hdr.msg_id) 
#ifdef ICF_IPSEC_ENABLED
            &&(ICF_SSA_SA_OPEN_SERVER != p_internal_msg->msg_hdr.msg_id)
            &&(ICF_SSA_SA_CLOSE_SERVER != p_internal_msg->msg_hdr.msg_id)
#endif
            )
		{
						/*
						 * SPR fix 7515 & 7528
						 * Don't fetch the line data if line id is invalid or call
						 * ctx is null.Forceful call clear is the only scenario in
						 * which call ctx can be null.Hence ,we are not putting check
						 * for call ctx being NULL in other scenarios
						 */
						if ((ICF_NULL == p_glb_pdb->p_call_ctx) &&
														(ICF_FORCEFUL_CALL_CLEAR_IND == 
														 p_internal_msg->msg_hdr.msg_id))
						{
										ICF_FUNCTION_EXIT(p_glb_pdb)
														return ICF_SUCCESS;

						}
						else if(ICF_NULL != p_glb_pdb->p_call_ctx)
						{
										line_id = p_glb_pdb->p_call_ctx->line_id;

						}
						else
						{	icf_sic_ctx_st  *p_sic_ctxt = ICF_NULL;

										switch(p_internal_msg->msg_hdr.msg_id)
										{
														case ICF_NW_OTG_SUBSCRIBE_REQ:
																		p_sic_ctxt = ((icf_nw_otg_subs_req_st*)
																										(p_internal_msg->p_msg_data))->p_sic_ctx;
																		break;

														case ICF_NW_OTG_MESSAGE_REQ:
																		p_sic_ctxt = ((icf_nw_otg_msg_req_st*)
																										(p_internal_msg->p_msg_data))->p_sic_ctx;
																		break;
														case ICF_NW_OTG_OPTIONS_REQ:
																		p_sic_ctxt = ((icf_nw_otg_opt_req_st*)
																										(p_internal_msg->p_msg_data))->p_sic_ctx;
																		break;
														case ICF_NW_OTG_PUBLISH_REQ:
																		p_sic_ctxt = ((icf_nw_otg_publish_req_st*)
																										(p_internal_msg->p_msg_data))->p_sic_ctx;
																		break;
														case ICF_NW_OTG_REFER:
																		p_sic_ctxt = ((icf_nw_otg_refer_req_st*)
																										(p_internal_msg->p_msg_data))->p_sic_ctx;
																		break;
														case ICF_DELINK_APP_CTX:
																		p_sic_ctxt = ((icf_delink_app_ctx_st*)
																										(p_internal_msg->p_msg_data))->p_sic_ctx;

																		break;
														default:
																		break;
										}

										if(ICF_NULL != p_sic_ctxt)
										{
														line_id = p_sic_ctxt->line_id;
														p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_sic_ctxt->p_ssa_ctx);
														if ( ICF_NULL != p_ssa_ctxt)
														{
																		p_ssa_ctxt->p_ssa_pdb = 
																						(icf_ssa_pdb_st *)&ssa_pdb;
														}
														if(ICF_DUMMY_APP_ID != p_sic_ctxt->app_id)
														{
																		if(ICF_FAILURE == icf_cfg_fetch_app_conf(p_glb_pdb,
																														p_sic_ctxt->app_id,
																														&(ssa_pdb.p_app_conf),
																														ssa_pdb.p_ecode))
																		{
																			ICF_ALARM(((icf_uint8_t *)"[SSA] Not able set the applcation conf in the SIC context\n"));
                                                                            ret_val = ICF_FAILURE;
                      														return ret_val;
																		}
														}
										}
						}

						if (ICF_INVALID_LINE_ID != line_id)
						{
										if (ICF_NULL != p_ssa_ctxt)
														p_ssa_ctxt->line_id = line_id;
										/*Get line data, we may need it to get the proxy address*/
										if ( ICF_FAILURE == 
																		icf_dbm_fetch_line_cntxt_blk(
																						p_glb_pdb, line_id,
																						(icf_void_t *)&(ssa_pdb.p_line_data),
																						ssa_pdb.p_ecode))
										{
														/*Raise a Major Error Here- CFG_DATA_NOT_FOUND*/
														ret_val = ICF_FAILURE;
										}
						}
						/*Fetch line data when register request is received*/
						if ((ICF_NW_OTG_REG_REQ == p_internal_msg->msg_hdr.msg_id) &&
														(ICF_CLEAR_REG_OBJECT != p_internal_msg->msg_hdr.msg_id))
						{
										icf_rgm_context_st               *p_rgm_ctxt = ICF_NULL;

										p_rgm_ctxt = (icf_rgm_context_st *)
														(ssa_pdb.p_internal_msg->p_msg_data);

										if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
																						p_glb_pdb,
																						p_rgm_ctxt->rgm_list_of_lines[p_rgm_ctxt->num_of_lines - 1],
																						&(ssa_pdb.p_line_data),
																						ssa_pdb.p_ecode))
										{
														ret_val = ICF_FAILURE;
														ICF_FUNCTION_EXIT(ssa_pdb.p_glb_pdb)
																		return ret_val;
										}
										if (ICF_NULL != p_ssa_ctxt)
										{
														p_ssa_ctxt->line_id = p_rgm_ctxt->rgm_list_of_lines[p_rgm_ctxt->num_of_lines - 1];
										}
						}
		}
		if (ICF_FAILURE != ret_val)
		{
						if (ICF_FAILURE == icf_dbm_get_module_glb_data(
																		p_glb_pdb,ICF_INT_MODULE_SSA,
																		(icf_void_t **)&(ssa_pdb.p_ssa_glb_data) ,
																		ssa_pdb.p_ecode))
						{
            ret_val = ICF_FAILURE;
        }
        /* get the config data pointer from DBM */
        else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                    ssa_pdb.p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                    ssa_pdb.p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
            ssa_pdb.p_glb_pdb,
            (icf_int_module_id_t)ICF_INT_MODULE_REGM,
            (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
            ssa_pdb.p_ecode))
        {
            ret_val = ICF_FAILURE;
        }

    }
    if (ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        return ret_val;
    }
    
    /*
     * Now we init the void pointer which will be used to store the
     * list of memory chunks that will be allocated in callbacks and
     * will be freed on return path (after invocation of destination 
     * module-CM, CFG etc.)
     */
    ssa_pdb.alloc_index = 0;
    for ( count = 0; count < ICF_SSA_MAX_MMRY_CHUNK_LST;
            count++)
    {
        ssa_pdb.allocated_memory[count].p_mem_chunk = ICF_NULL;
        ssa_pdb.allocated_memory[count].ssa_mem_type = 0;
    }

    if ( ICF_NULL != p_ssa_ctxt)
    {
	    ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
    }
    if ((ICF_CFG_SELF_ADDR_PER_LINE == 
        ssa_pdb.p_glb_cfg_data->self_addr_per_line) &&
        (ICF_NULL != ssa_pdb.p_line_data))
    {
        p_transport_address = &(ssa_pdb.p_line_data->self_addr);
        if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                    p_transport_address->addr.addr_type)
        {
            icf_port_strcpy(p_transp_addr_str,
                            p_transport_address->addr.addr.domain.str);
        }
        else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                        p_transport_address->addr.addr_type)
        {
            icf_ssa_convert_to_IP_addr(p_transport_address,
                            p_transp_addr_str);
            if ( 0 == icf_port_strcmp((icf_uint8_t *)"0.0.0.0", p_transp_addr_str))
            {
                ret_val = ICF_FAILURE;
            }
        }
        else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                        p_transport_address->addr.addr_type)
        {
            icf_port_strcpy(p_transp_addr_str,
                            p_transport_address->addr.addr.ipv6_addr.str);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }

        if (Sdf_co_fail == sdf_ivk_uaSetDefaultTransport(
                    ssa_pdb.p_ssa_glb_data->p_default_profile,
                    (Sdf_ty_s8bit *)p_transp_addr_str, \
                    p_transport_address->port_num, &sdf_error))
        {
            /* Raise a Major Category Error from here */
            ret_val = ICF_FAILURE;
        }
        if ((ICF_NULL != ssa_pdb.p_ssa_ctxt) &&
            (ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_obj) &&
            (ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_obj->pInitData))
        {
            if (Sdf_co_fail == sdf_ivk_uaSetDefaultTransport(
                    ssa_pdb.p_ssa_ctxt->p_call_obj->pInitData,
                    (Sdf_ty_s8bit *)p_transp_addr_str, \
                    p_transport_address->port_num, &sdf_error))
            {
                /* Raise a Major Category Error from here */
                ret_val = ICF_FAILURE;
            }
        }
    }

    if (ICF_SUCCESS == ret_val)
    ret_val = icf_ssa_handle_internal_api(&ssa_pdb);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_handle_successful_decode
 *
 * DESCRIPTION:     This routine covers handling after a success is returned
 *                  by SDF Decode API
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t     icf_ssa_handle_successful_decode(
                        icf_ssa_pdb_st       *p_ssa_pdb,
                        Sdf_st_eventContext     *p_event_ctxt,
                        Sdf_st_callObject       *p_call_obj,
                        Sdf_st_error            *p_sdf_error,
                        icf_error_t          *p_ecode)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st           *p_glb_pdb = 
        p_ssa_pdb->p_glb_pdb;
    Sdf_st_commonInfo           *p_common_info = Sdf_co_null;
    Sdf_st_callObject           *p_orig_call_obj = Sdf_co_null;
    SipOptions                      dOptions;
    Sdf_ty_s8bit                *p_method = Sdf_co_null;
    Sdf_ty_s8bit                *p_method_temp = Sdf_co_null;
    SipMessage		            *pMessage = Sdf_co_null;
    Sdf_ty_callLegMatch         call_leg_match = Sdf_en_notMatching;
    icf_ssa_ctxt_st			*p_tmp_ssa_ctxt = ICF_NULL;
    icf_boolean_t               dummy_flag = ICF_TRUE;
    Sdf_st_overlapTransInfo   *pTransInfo = Sdf_co_null;

    ICF_FUNCTION_ENTER(p_glb_pdb)
        /* 
         * Fetch for the key of the incoming SipMessage
         * in the hash table of existing calls.
         */
        dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
    if (Sdf_co_fail == sdf_ivk_uaGetCommonInfoFromCallObject(\
                p_call_obj, &p_common_info, p_sdf_error))
    {
        /*Raise an informational error here*/
        ret_val = ICF_FAILURE;
    }
    else if ( Sdf_co_null != p_common_info->pCallid)
    {
        SipMessage              *p_temp_msg = Sdf_co_null;
		icf_boolean_t			call_obj_found = ICF_FALSE;	

		p_ssa_pdb->hash_index = 0;
        if ( ICF_SUCCESS == icf_ssa_map_nw_callid(
                    p_ssa_pdb, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_call_obj, 
#endif
                    (icf_uint8_t *)p_common_info->pCallid,
                    &p_ssa_ctxt, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_sdf_error, 
#endif
                    p_ecode))
        {

            p_orig_call_obj = p_ssa_ctxt->p_call_obj;

            if (p_call_obj->pUacTransaction->pSipMsg != Sdf_co_null)
                p_temp_msg = p_call_obj->pUacTransaction->pSipMsg;
            else
                p_temp_msg = p_call_obj->pUasTransaction->pSipMsg;

            if (Sdf_co_fail == (sdf_ivk_uaCompareCallObjectKeys(\
                            p_call_obj, p_orig_call_obj,\
                            p_temp_msg, &call_leg_match, \
                            p_sdf_error)))
            {
                /*Raise a Minor Error here*/
                ret_val = ICF_FAILURE;
            }
            if(Sdf_en_notMatching == call_leg_match)
            {
               ret_val = ICF_FAILURE;
            }
            else if (Sdf_en_sameCallLeg !=  call_leg_match)
            {
                p_tmp_ssa_ctxt = p_ssa_ctxt;
            }
        }
        else
        {
            ret_val = ICF_FAILURE;
            call_obj_found = ICF_TRUE;	
        }


        if (ICF_SUCCESS == ret_val)
        {
            ret_val = ICF_SUCCESS;
            if(Sdf_en_sameCallLeg != call_leg_match)
            {
                if (Sdf_en_tagMismatchError == p_sdf_error->errCode)
                {
                    if(Sdf_co_fail == 
                            sdf_ivk_uaGetMethodFromSipMessage(
                                p_temp_msg,
                                &p_method, p_sdf_error))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else if ( 0 != icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"NOTIFY"))
                    {
                        ret_val = ICF_FAILURE;
                    }
                }
            }
            else if(Sdf_en_sameCallLeg == call_leg_match)
            {
		        if ( ICF_NULL != p_ssa_ctxt && 
        		     ICF_NULL != p_ssa_ctxt->p_call_ctxt)
	        	{
                   	p_ssa_ctxt->p_call_ctxt->p_sip_ctx = p_ssa_ctxt;
		        }
            }

#ifdef ICF_DNS_LOOKUP_ENABLED
			/* code for DNS buffer starts here */	    
	    if((ICF_NULL != p_ssa_ctxt)
            && (p_ssa_ctxt->bitmask & ICF_SSA_DNS_FQDN_PRESENT))
	    {
            /* FQDN is present update the active IP in DNS Buffer */
            icf_dns_buffer_st                 dns_info_to_store;
            
              icf_port_strcpy(dns_info_to_store.ip_address, p_ssa_ctxt->ip_fr_req);
              dns_info_to_store.port = p_ssa_ctxt->port_fr_req;
              /* Fix for CSR 1-5078472 : SPR 15937*/
              if (( Sdf_en_protoTcp == p_event_ctxt->dProtocol)
#ifdef ICF_SECURE_SUPPORT
                        || (Sdf_en_protoTls == p_event_ctxt->dProtocol ))
#else
					)
#endif	
                  {
                        icf_ssa_tcpm_conn_data_st  *p_ssa_ctxt_db_node =
                                    p_ssa_ctxt->p_conn_table_req_entry;

                        icf_port_strcpy(dns_info_to_store.ip_address,
                                p_ssa_ctxt_db_node->peer_ip);
                        dns_info_to_store.port = p_ssa_ctxt_db_node->peer_port_num;
                    }

                icf_port_strcpy(dns_info_to_store.fqdn.str, p_ssa_ctxt->fqdn.str);
                if(ICF_SUCCESS == icf_ssa_dns_add_fqdn_to_ip_mapping(&dns_info_to_store,
                            p_ssa_ctxt,
                            p_call_obj,
                            p_ecode))
                {
                    /* the address is updated reset the bitmask*/
                    p_ssa_ctxt->bitmask &= (~ICF_SSA_DNS_FQDN_PRESENT );
                }
            }
            /* end of the DNS buffer code */
#endif


            if (ICF_SUCCESS == ret_val)
            {
                icf_boolean_t    to_be_sent = ICF_TRUE;
                /*Message mapped to existing dialog/txn*/
                Sdf_ty_messageValidity      sdf_retval;
                en_SipMessageType   dMsgType;
                /*No match, this is a new transaction/dialog*/
                p_event_ctxt->dSdfDisabledChecks=0;

                /* 
                 * Check the validity of the message.
                 * Invalid messages in terms of Cseq are
                 * automatically responded to by the toolkit
                 */
                /*
                 * We set the ssa_pdb in the temporary call object also
                 * as in case of Cancel received, the callback will 
                 * have the temporary call object instead of the mapped
                 * call object.
                 */
                /*p_call_obj->pAppData->pData = 
                  (SIP_Pvoid)p_ssa_ctxt;*/
                sip_getMessageType(p_temp_msg, &dMsgType, \
                        (SipError*)	&(p_sdf_error->stkErrCode)) ;
                p_ssa_pdb->dMsgType = dMsgType;
                if (ICF_NULL != p_ssa_ctxt)
                {
                    /* Moved below condition inside "if" to avoid
                       klocwork warning */
                    p_ssa_ctxt->bitmask_1 &= ~ICF_MSG_RECVD_OUT_OF_DIALOG;
                    if (SipMessageRequest == dMsgType)
                    {
						/*CSR 1-7576668. In case of UDP, there will no conn table entry. Hence,
						 * IPTK shall not try to update the connection table entry in case of UDP.
						 * The NULL check in p_conn_table_entry signifies that the message is received on
						 *	UDP.
						 */	
						if (ICF_NULL != p_ssa_pdb->p_conn_table_entry)
						{
							 icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, p_ssa_pdb->p_conn_table_entry);
						}

                        if (p_ssa_pdb->p_conn_table_entry != p_ssa_ctxt->p_conn_table_resp_entry)
                        {
                            p_ssa_pdb->p_conn_table_entry->ref_count++;
                            ret_val = icf_ssa_check_for_new_conn(p_ssa_pdb,
                                    p_ssa_ctxt,
                                    p_ssa_ctxt->p_call_obj);
                        /*    icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, p_ssa_pdb->p_conn_table_entry);*/
                            p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |=
                                ICF_CONN_TYPE_RESPONSE;
                            if ((p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask & ICF_TRANSPORT_TLS) &&
                                     (icf_ssa_is_alias_present_in_Via(p_temp_msg)))
                            {
                                ICF_PRINT(((icf_uint8_t *)"Its persistent TLS" \
                                            "connection.Alias found\n"));
                                if (0 != ICF_PURGE_TIMER_DURATION)
                                    p_ssa_pdb->p_conn_table_entry->is_persistent = ICF_TRUE;
                            }

                            ICF_SSA_PRINT_CONN_DB(p_ssa_ctxt->p_conn_table_resp_entry)


                        } /* p_ssa_ctxt->ssa_conn_db_index) */
                    }
                    else if (SipMessageResponse == dMsgType)
                    {

                        if (p_ssa_pdb->p_conn_table_entry != p_ssa_ctxt->p_conn_table_req_entry)
                        {
                        	
							/*Fix for CSR 1-7576668*/	
						/*  icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, p_ssa_pdb->p_conn_table_entry); */
						 
                            ret_val = icf_ssa_check_for_new_conn(p_ssa_pdb,
                                    p_ssa_ctxt,
                                    p_ssa_ctxt->p_call_obj);
                            p_ssa_pdb->p_conn_table_entry->ref_count++;
                            p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |=
                                ICF_CONN_TYPE_REQUEST;
                            if ((p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask & ICF_TRANSPORT_TLS) &&
                                     (icf_ssa_is_alias_present_in_Via(p_temp_msg)))
                            {
                                ICF_PRINT(((icf_uint8_t *)"Its persistent TLS" \
                                            "connection.Alias found\n"));
                                if (0 != ICF_PURGE_TIMER_DURATION)
                                    p_ssa_pdb->p_conn_table_entry->is_persistent = ICF_TRUE;
                            }
                            ICF_SSA_PRINT_CONN_DB(p_ssa_ctxt->p_conn_table_req_entry)
                        } /* p_ssa_ctxt->ssa_conn_db_index) */
                    }

                p_ssa_pdb->p_ssa_ctxt->p_conn_table_req_entry = 
                    p_ssa_ctxt->p_conn_table_req_entry;
                p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry = 
                    p_ssa_ctxt->p_conn_table_resp_entry;
                }

                if ( SipMessageRequest == dMsgType)
                {
                    icf_boolean_t    is_mem_available = ICF_TRUE;
                    icf_boolean_t    is_sub_or_reg_req = ICF_FALSE;
                    SipHeader       *pHdr = Sdf_co_null;
                    SipOptions		dOptions;
                    Sdf_st_transaction 			*pTransaction = Sdf_co_null;
                    Sdf_ty_u16bit    dRespCode = 500;
                    icf_boolean_t    is_invalid_method = ICF_FALSE;
                    icf_app_id_t     application_id = ICF_APP_ID_INVALID;


                    dOptions.dOption=\
                                     SIP_OPT_CLEN | SIP_OPT_FULLFORM;

                    if(ICF_NULL != p_call_obj->pUasTransaction->pSipMsg)
                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                    else
                        pMessage = p_call_obj->pUacTransaction->pSipMsg;

#ifdef ICF_PORT_SYMBIAN
                    if ( (ICF_FAILURE == 
                                icf_port_is_mem_available(p_glb_pdb)) || 
                            (ICF_FAILURE ==
                             icf_port_is_timer_available(p_glb_pdb)))
                    {
                        is_mem_available = ICF_FALSE;
                    }
#endif /* ICF_PORT_SYMBIAN */
                    if(Sdf_co_fail == 
                            sdf_ivk_uaGetMethodFromSipMessage(
                                pMessage,
                                &p_method, p_sdf_error))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else if(( 0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"SUBSCRIBE")) || 
                            ( 0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"REGISTER")) ||
                            ( 0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"PUBLISH")))
                    {
                        is_sub_or_reg_req = ICF_TRUE;
                        dRespCode = 405;
                    }
                    if(ICF_NULL ==  p_ssa_pdb->p_app_conf)
                    {
                    	if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
                        {
                        	application_id = p_ssa_ctxt->p_call_ctxt->app_id;
                        }
                        else if(ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
                        {
                            application_id = p_ssa_ctxt->p_sic_ctxt->app_id;
                        }
                        else if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt)
                        {
                        	if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt->p_app_data_list)
                            {
                            	 application_id = p_ssa_ctxt->p_rgm_ctxt-> \
                                                      p_app_data_list->app_id;
                            }
                        }
                        else
                        {
                        	/*Default app id*/
                            application_id = 1;
                        }
                        if(ICF_FAILURE == icf_cfg_fetch_app_conf(p_ssa_pdb->p_glb_pdb,
                            application_id,
                            &(p_ssa_pdb->p_app_conf),
                            p_ssa_pdb->p_ecode))
                        {
                        	ret_val = ICF_FAILURE;
                        }
                    }
                    if (ICF_FAILURE == icf_ssa_validate_basic_method(p_ssa_pdb,
                                        pMessage,
                                        &is_invalid_method))
					{
						ret_val = ICF_FAILURE;
					}
                    else if(ICF_TRUE == is_invalid_method)
					{
						if (p_ssa_pdb->p_ssa_ctxt->failure_bitmask &
							ICF_SSA_FAILURE_RESP_CODE_501)
						{
							dRespCode = 501;
                            p_ssa_pdb->p_ssa_ctxt->failure_bitmask &= ~ICF_SSA_FAILURE_RESP_CODE_501;
						}
						else if (p_ssa_pdb->p_ssa_ctxt->failure_bitmask &
							ICF_SSA_FAILURE_RESP_CODE_405)
						{
						    dRespCode = 405;
                            p_ssa_pdb->p_ssa_ctxt->failure_bitmask &= ~ICF_SSA_FAILURE_RESP_CODE_405;
						}
					}
					if (ICF_FAILURE != ret_val)
                    {
                        if (Sdf_co_null != p_method_temp)
                        {
                            sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                    p_sdf_error);
                        }
                        Sdf_mc_strdup(p_method_temp, p_method);
                    }
                    if((ICF_TRUE == is_sub_or_reg_req) ||
                            (ICF_FALSE == is_mem_available)||
                            (ICF_TRUE == is_invalid_method))
                    {
                        if ( Sdf_co_fail == 
                                sdf_ivk_uaRejectRequest(p_call_obj,
                                    p_orig_call_obj->pInitData,
                                    dRespCode,&pTransInfo,p_sdf_error))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            if(sdf_ivk_uaGetTransactionForSipMsg(\
                                        (p_call_obj), pMessage, &pTransaction, \
                                        &pTransInfo, p_sdf_error) == Sdf_co_success)
                            {
                                if(pTransInfo == Sdf_co_null)
                                {
                                    pMessage = pTransaction->pSipMsg;
                                    sdf_ivk_uaFreeTransaction(pTransaction);
                                }
                                else
                                {
                                    pMessage = pTransInfo->pSipMsg;
                                    sdf_ivk_uaFreeOverlapTransInfo( \
                                            pTransInfo);
                                }
                            }
                            if(ICF_FALSE == is_mem_available)
                            {
                                if(sip_initSipHeader(&pHdr,SipHdrTypeRetryAfterSec,
                                            (SipError *)&(p_sdf_error->stkErrCode)) == 
                                        SipFail)
                                {
                                    return ICF_FAILURE;
                                }
                                if(sip_setDeltaSecondsInRetryAfterHdr(pHdr,
                                            140,(SipError *)&(p_sdf_error->stkErrCode)) == 
                                        SipFail)
                                {
                                    sip_freeSipHeader(pHdr);
                                    ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,pHdr, \
                                            ICF_MEM_COMMON, p_ecode, ret_val)
                                        return ICF_FAILURE;
                                }
                                if(sip_setHeader(pMessage,pHdr,(SipError *)&(p_sdf_error->stkErrCode)) \
                                        ==SipFail)
                                {
                                    sip_freeSipHeader(pHdr);
                                    ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,pHdr, \
                                            ICF_MEM_COMMON, p_ecode, ret_val)
                                        return ICF_FAILURE;
                                }

                                sip_freeSipHeader(pHdr);
                                ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,pHdr, \
                                        ICF_MEM_COMMON, p_ecode, ret_val)
                            }

                            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                        (p_call_obj),
                                        pMessage,
                                        &dOptions,p_sdf_error))
                            {
                                ret_val = ICF_FAILURE;
                            }
                            if (Sdf_co_null != p_method_temp)
                            {
                                 sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                            p_sdf_error);
                            }
                            sdf_ivk_uaFreeCommonInfo(p_common_info);
                            return ret_val;
                        }/* If rejectRequest Suceeed */
                    }/* If Failure Case */
                }/* If Request */

                if (ICF_NULL == p_ssa_ctxt)
                {
                   if (ICF_NULL != pTransInfo)
                   {
                       sdf_ivk_uaFreeOverlapTransInfo(pTransInfo);
                   }
                   return ICF_FAILURE;
                }
                p_ssa_ctxt->p_ssa_pdb = (struct icf_ssa_pdb_st*)p_ssa_pdb;
#ifdef ICF_NAT_RPORT_SUPPORT
                icf_port_memcpy(p_ssa_ctxt->destination_ip,\
                            (icf_uint8_t*)p_ssa_pdb->p_ssa_ctxt->destination_ip,\
                            sizeof(p_ssa_ctxt->destination_ip));

                p_ssa_ctxt->destination_port = p_ssa_pdb->p_ssa_ctxt->destination_port;
#endif
                /* ICF_SSA_TRACE(p_ssa_pdb,ICF_TRACE_SSA_BITMASK)*/

                /*
                 * As currently the PDB is updated with the SSA ctxt,
                 * as the mapped SSA ctxt has been retreived,update the 
                 * pointer accordingly
                 */
                p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
#ifdef ICF_NAT_RPORT_SUPPORT
                if(SipMessageResponse == dMsgType)
                {
                    if(ICF_NULL != p_call_obj->pUacTransaction->pSipMsg)
                        pMessage = p_call_obj->pUacTransaction->pSipMsg;
                    else
                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                }
                ret_val = icf_ssa_get_params_from_Via(pMessage,\
                        p_ssa_pdb,dMsgType,(SipError*)	&(p_sdf_error->\
                            stkErrCode));
#endif

                if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                            p_temp_msg, &p_method,p_sdf_error ))
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]:FailureIngettingMethod from Message\n"));
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    if (Sdf_co_null != p_method_temp)
                    {
                        sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                p_sdf_error);
                    }
                    Sdf_mc_strdup(p_method_temp, p_method);
                    if (0 != Sdf_mc_strcasecmp(p_method,"REGISTER"))
                    {
                        if(ICF_NULL == p_ssa_ctxt->p_call_ctxt)
                        {
                            dummy_flag = ICF_FALSE;
                        }
                        /*If not Register response, then for all
                         * others we will have call ctxt here.
                         * So we check for valid value of the same
                         * to determine if APP wants a message for
                         * this dialof*/
                        if ( ICF_SSA_APP_CTXT_TERMINATED ==
                                (ICF_SSA_APP_CTXT_TERMINATED & p_ssa_ctxt->bitmask) ||
                            ((ICF_NULL != p_ssa_ctxt->p_call_ctxt) &&
                               ( ICF_CALL_CLEAR_STATUS_PA_CLEARED ==
                               ( ICF_CALL_CLEAR_STATUS_PA_CLEARED &
                                 p_ssa_ctxt->p_call_ctxt->call_clear_status))))
                        {
                            ICF_PRINT(((icf_uint8_t *)"[SSA]:MsgRecvdInDialog,Yet no message\
                                        to be sent to APP\n"));
                            
                            ICF_SSA_SET_COMMON_BITMASK(
                                p_ssa_pdb->common_bitmask,
                                ICF_SSA_MSG_RECVD_OUT_OF_DIALOG)
                                
                            /*to_be_sent = ICF_FALSE;*/

                        }
                    }

                    p_glb_pdb->p_call_ctx = p_ssa_ctxt->p_call_ctxt; 
                    if(0 == Sdf_mc_strcasecmp(p_method,"CANCEL"))
                    {
#ifdef ICF_TRACE_ENABLE
                        /* SPR 17555 Fix: Send call direction to app_trace for 
                         * SIP PDUs
                         */
                        p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
#endif
                         ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
                          p_ssa_pdb->common_bitmask |= ICF_SSA_MSG_TRACE_ALREADY_SENT; 

                    }
                    if (((ICF_NULL != p_ssa_ctxt->p_call_ctxt) &&
                                (ICF_INVALID_CALL_ID == 
                                 p_ssa_ctxt->p_call_ctxt->call_id )) &&
                            ((0 == Sdf_mc_strcasecmp(p_method,"INVITE"))||
                             (0 == Sdf_mc_strcasecmp(p_method,"CANCEL"))||
                             (0 == Sdf_mc_strcasecmp(p_method,"BYE"))||
                             (0 == Sdf_mc_strcasecmp(p_method,"PRACK"))))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Unable to fetch call context\n"));
                        sdf_ivk_uaFreeCommonInfo(p_common_info);
                        /* MEM LEAK FIX  Start */ 
                        if (Sdf_co_null != p_method_temp)
                        {
                            sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                    p_sdf_error);
                        }
                        /* MEM LEAK FIX  End */
                        if (ICF_NULL != pTransInfo)
                        {
                            sdf_ivk_uaFreeOverlapTransInfo(pTransInfo);
                        }
                        return ICF_FAILURE;
                    }

                    /*
                     * One instance of SSA CTXT was allocated in the calling
                     * routine for this function. Since we have now found the 
                     * actual call object and the SSA ctxt, we will overwrite the
                     * value in event context by the original SSA ctxt
                     */
                        /* 7.0 */

                    if((0 != Sdf_mc_strcasecmp(p_method,"INVITE")) &&
                            (Sdf_en_invalid == sdf_ivk_uaIsSupportedMsgBody(p_call_obj,
                                                                            p_orig_call_obj->pInitData,p_sdf_error)) && 
                            ( dMsgType == SipMessageRequest))
                    {
                        /* sdf_ivk_uaIsSupportedMsgBody function has called 
                         * sdf_ivk_uaRejectRequest so we will directly send the
                         * message 
                         */
                        SipOptions		dOptions;
                        SipMessage					*pMessage = Sdf_co_null;
                        Sdf_st_transaction 			*pTransaction = Sdf_co_null;
                        Sdf_st_overlapTransInfo 	*pOverlapTransaction = Sdf_co_null;
                        dOptions.dOption=\
                                         SIP_OPT_CLEN | SIP_OPT_FULLFORM;

                        if(ICF_NULL != p_call_obj->pUasTransaction->pSipMsg)
                            pMessage = p_call_obj->pUasTransaction->pSipMsg;
                        else
                            pMessage = p_call_obj->pUacTransaction->pSipMsg;

                        /* 
                         * Bad Contnet received in 
                         * Content-type header. 415 unsuppreted 
                         * formed by UATK. Send the SipMsg by using .
                         * sdf_ivk_uaGetTransactionForSipMsg
                         */
                            
                        if(sdf_ivk_uaGetTransactionForSipMsg(\
                                    (p_call_obj), pMessage, &pTransaction, \
                                    &pOverlapTransaction, p_sdf_error) == Sdf_co_success)
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

                            ICF_PRINT(((icf_uint8_t *)"[SSA:] Validation of Content-Type, Content-Encoding"
                                        " or Content-Language failed\n"));
                            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                        (p_call_obj),
                                        pMessage,
                                        &dOptions,p_sdf_error))
                            {
                                ret_val = ICF_FAILURE;
                            }
                        }
                        if (Sdf_co_null != p_method_temp)
                        {
                            sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                    p_sdf_error);
                        }
                    }
                    else
                    {
                        /*                        ICF_SSA_TRACE(*/

                        /*                            p_ssa_pdb,ICF_TRACE_UPDATE_CALL_DET_STATUS)*/

                        Sdf_st_callObject           *p_tmp_call_obj = Sdf_co_null;
                        /* commented for ETSI conformance case 
                        * SIP_CC_TE_CR_V_005
                        */
                        /*
                         p_event_ctxt->dSdfDisabledChecks |= 
                                Sdf_mc_disableRequireCheck;*/

                         if (Sdf_en_sameCallLeg ==  call_leg_match)
                         {
                              p_event_ctxt->pData = p_ssa_ctxt;
                         }
                        /*Fix for CSR No: 1-5230713*/ 
                        
                         if(((Sdf_en_reInviteReceived ==
                              p_orig_call_obj->pCallInfo->dState)||
                            (Sdf_en_reInviteprovRespSent ==
                              p_orig_call_obj->pCallInfo->dState))&&
                           (ICF_NULL != p_call_obj->pUasTransaction))
                        {
                            if(Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                                                  p_call_obj->pUasTransaction->\
                                                      pSipMsg,
                                                  &p_method, p_sdf_error))
                            {
                                ret_val = ICF_FAILURE;
                            }
                            else if(0 == icf_port_strcmp((icf_uint8_t *)"BYE",(icf_uint8_t *)p_method))
                            {
                                ret_val = icf_ssa_form_and_send_response(
                                              p_ssa_pdb,
                                              487,
                                              p_orig_call_obj,
                                              0, "INVITE", Sdf_co_false);
                                ((icf_ssa_ctxt_st*)p_event_ctxt->pData)->\
                                    p_ssa_pdb = p_ssa_pdb;
                            }
                            if(ICF_FAILURE == ret_val)
                            {
                                if (ICF_NULL != pTransInfo)
                                {
                                    sdf_ivk_uaFreeOverlapTransInfo(pTransInfo);
                                }

                                return ret_val;
                            }
                        }
                        sdf_retval = sdf_ivk_uaUpdateCallDetails(\
                                &p_orig_call_obj, p_call_obj, \
                                p_event_ctxt, p_sdf_error);


                        switch(sdf_retval){
                            case Sdf_en_discarded:
                                /*TO DO : Failure Handling to be added*/
                                break;
                            case Sdf_en_valid:
                                {
                                    /*Add an info trace here*/
#ifdef  ICF_TRACE_ENABLE
                				   if (p_call_obj->pUacTransaction->pSipMsg 
                                      != Sdf_co_null)
                                   {
                                      p_temp_msg = 
                                        p_call_obj->pUacTransaction->pSipMsg;
                                   }
                                  else
                                  {
                                      p_temp_msg = 
                                        p_call_obj->pUasTransaction->pSipMsg;
                                  }

                                   if(ICF_NULL != p_temp_msg)
                                   {
                                       if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent(
                                                   p_temp_msg, p_sdf_error))
                                       {
                                           p_ssa_pdb->sdp_present = ICF_TRUE;
                                       }
                                       else
                                       {
                                           p_ssa_pdb->sdp_present = ICF_FALSE;
                                       }
                                       /* FMR in TCP UT since ssa_ctxt is
                                        * deleted */
                                      /* icf_ssa_capture_trace(
                                               p_ssa_pdb,p_temp_msg, 1);*/
                                  }
#endif
                                    ret_val = ICF_SUCCESS;
                                    if (ICF_TRUE == to_be_sent) 
                                    {
                                        ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                                                ICF_SSA_INTER_MODULAR_MSG)
                                            if ( ICF_SSA_AWAITING_SSA_CTXT_DEL == 
                                                    (ICF_SSA_AWAITING_SSA_CTXT_DEL & p_ssa_pdb->common_bitmask))
                                            {
                                                if (ICF_NULL != p_orig_call_obj)
                                                {
                                                    p_ssa_ctxt->bitmask |= ICF_SSA_APP_CTXT_TERMINATED;
                                                }
                                            }
                                    }
                                    if (ICF_NULL != p_orig_call_obj)
                                    {

#if 0
                                        if ( p_ssa_pdb->ssa_conn_db_index != 
                                                temp_conn_db_index)
                                        {
                                            /*                                        ICF_PRINT(("\n[SSA]:MsgRecvdOnDifferent ConnId"));*/

                                            /*On reception of a msg on a diff conn
                                             * then what is there in the CallObj, the 
                                             * UATK closes the previous conn and 
                                             * updates the callobj for the new one,
                                             * So here we update the SSA ctxt also*/
                                            p_ssa_ctxt->ssa_conn_db_index = 
                                                p_ssa_pdb->ssa_conn_db_index;
                                        }
#endif
                                    }
                                }
                                break;
                            case Sdf_en_failed:
                            case Sdf_en_invalid:
                                p_tmp_call_obj = p_call_obj;
                                if(((p_sdf_error->errCode==Sdf_en_badExtensionsError) || \
                                            (p_sdf_error->errCode==Sdf_en_InvalidCseqNumber) || \
                                            (p_sdf_error->errCode==Sdf_en_serverInternalError) || \
                                            (p_sdf_error->errCode==Sdf_en_regularTxnPendingError) || \
                                            (p_sdf_error->errCode==Sdf_en_requestPendingError) || \
                                            (p_sdf_error->errCode==Sdf_en_localTagMismatchError) || \
                                            (p_sdf_error->errCode==Sdf_en_lowSessionTimerIntervalError) || \
                                             /* Fix for CSR_1-5152624*/
       			                    (p_sdf_error->errCode==Sdf_en_cancelTxnMismatch)|| \
				                  /* Fix for CSR_1-5152624*/
                                            (p_sdf_error->errCode==Sdf_en_unknownMethodError)) && \
                                        ((dMsgType == SipMessageRequest) && 
                                         (0 != Sdf_mc_strcasecmp(p_method_temp,"ACK"))))
                                {
                                    SipOptions		dOptions;
                                    SipMessage					*pMessage = Sdf_co_null;
                                    Sdf_st_transaction 			*pTransaction = Sdf_co_null;
                                    Sdf_st_overlapTransInfo 	*pOverlapTransaction = Sdf_co_null;
                                    dOptions.dOption=\
                                                     SIP_OPT_CLEN | SIP_OPT_FULLFORM;

                                    /* SPR 14920 : Added badExtension condition*/
                                    /* added sdf_en_badExtensionError condition for ETSI conformance case 
                                    * SIP_CC_TE_CR_V_005
                                    */
                                   if ((p_sdf_error->errCode==Sdf_en_lowSessionTimerIntervalError) ||
                                        (p_sdf_error->errCode==Sdf_en_badExtensionsError))
                                    {
                                        p_tmp_call_obj = p_orig_call_obj;
                                    }
                                    else
                                    {
                                        p_tmp_call_obj = p_call_obj;
                                    }
                                    if(p_tmp_call_obj->pUasTransaction->pSipMsg !=Sdf_co_null)
                                        pMessage = p_tmp_call_obj->pUasTransaction->pSipMsg;
                                    else
                                        pMessage = p_tmp_call_obj->pUacTransaction->pSipMsg;

                                    if(sdf_ivk_uaGetTransactionForSipMsg(\
                                                p_tmp_call_obj, pMessage, &pTransaction, \
                                                &pOverlapTransaction, p_sdf_error) == Sdf_co_fail)
                                    {
                                        ret_val = ICF_FAILURE ;
                                        break;
                                    }
                                    else if(pOverlapTransaction == Sdf_co_null)
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
                                    if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                                p_tmp_call_obj,
                                                pMessage,
                                                &dOptions,p_sdf_error))
                                    {
                                        ret_val = ICF_FAILURE;
                                        break;
                                    }

                                }
                                p_tmp_call_obj = Sdf_co_null;
                                ret_val = ICF_FAILURE ;
                                break;
                            default:/*Invalid State transition*/
                                break;
                        }
                        if ( ICF_SSA_REL_CALL_OBJ_ON_RET == 
                                (ICF_SSA_REL_CALL_OBJ_ON_RET & p_ssa_pdb->common_bitmask))
                        {
                            ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)

                            /*This bitmask is used to prevent trace from icf_ssa_entrypoint.c,
                             *as the trace is raised from here. This is done because in case of TCP
                             *p_ssa_ctxt is freed from here only and will not be available there*/
                            ICF_SSA_SET_COMMON_BITMASK(
                                p_ssa_pdb->common_bitmask,
                                         ICF_SSA_TRACE_REQUIRED)
                
                            /*for details of this fix, check the comment in ssa_types.h
                              for the above mentioned bitmask*/
                            sdf_ivk_uaFreeCallObject(&p_orig_call_obj);
                        }
                        if (Sdf_co_null != p_method_temp)
                        {
                            sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                    p_sdf_error);
                        }
                    }
                }

            }/*Msg was mapped to an existing dialog/txn*/
        }
        else
        {
            ret_val = ICF_SUCCESS;
            /*
             * In Rel 1.0 ,if the method received is not INVITE,
             * we will not process it .Simply return SUCCESS.
             * This is inline with the assumption that only
             * INVITE can be received out of dialog.This
             * assumption is not true as other methods such as
             * OPTIONS can be received out of dialog.
             * In 2.0 ,this has to be modified to accomodate 
             * other out-of-dialog requests.
             */
            /*
             * Get the method from the Sip Message in the UAS
             * transaction of the callobject and check whether
             * it is INVITE
             */

            if ((Sdf_co_null == p_call_obj->pUasTransaction)||
                (Sdf_co_null == p_call_obj->pUasTransaction->pSipMsg))
            {
                return ICF_FAILURE;
            }

            if (ICF_FAILURE == ret_val)
            {
                sdf_ivk_uaFreeCommonInfo(p_common_info);
                ICF_FUNCTION_EXIT(p_glb_pdb)
                    return ret_val;
            }
            else
            {
                Sdf_st_initData     *p_profile = Sdf_co_null;
                Sdf_st_error         sdf_error;
                icf_app_id_t     app_id = ICF_APP_ID_INVALID;
                icf_boolean_t    send_failure_resp = ICF_FALSE;
                icf_boolean_t    rej_request_called = ICF_FALSE;
                icf_uint8_t     methodName[100];
                en_SipMessageType   dMsgType = SipMessageAny;
                Sdf_ty_s8bit *p_method = Sdf_co_null;
                p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_event_ctxt->pData);

                ICF_SSA_SET_COMMON_BITMASK(
                        p_ssa_pdb->common_bitmask,
                        ICF_SSA_MSG_RECVD_OUT_OF_DIALOG)
                p_ssa_ctxt->bitmask_1 |= ICF_MSG_RECVD_OUT_OF_DIALOG;               
              
                p_glb_pdb->p_call_ctx = p_ssa_ctxt->p_call_ctxt;
                p_ssa_pdb->p_app_conf = ICF_NULL;
#ifdef ICF_NAT_RPORT_SUPPORT
                if(ICF_NULL != p_call_obj->pUasTransaction->pSipMsg)
                    pMessage = p_call_obj->pUasTransaction->pSipMsg;
                ret_val = icf_ssa_get_params_from_Via(pMessage,\
                        p_ssa_pdb,SipMessageRequest,(SipError*)	&(p_sdf_error->\
                            stkErrCode));
#endif
                if (p_ssa_pdb->p_conn_table_entry != ICF_NULL)
                {
                    ICF_PRINT(((icf_uint8_t *)"Updating connection table entry for this ssa context\n"));
                    icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, p_ssa_pdb->p_conn_table_entry);
                    dMsgType = icf_ssa_get_msg_type(p_ssa_pdb, p_ssa_pdb->p_ssa_ctxt->p_call_obj);
                    if (dMsgType == SipMessageRequest)
                    {
                        ICF_PRINT(((icf_uint8_t *)"TCP/TLS connection of REQUEST type"));
                        p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |= ICF_CONN_TYPE_RESPONSE;
                        p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry = p_ssa_pdb->p_conn_table_entry;
                    }
                    else if (dMsgType == SipMessageResponse)
                    {
                        ICF_PRINT(((icf_uint8_t *)"TCP/TLS connection of RESPONSE type"));
                        p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask  |= ICF_CONN_TYPE_REQUEST;
                        p_ssa_pdb->p_ssa_ctxt->p_conn_table_req_entry = p_ssa_pdb->p_conn_table_entry;
                    }
                    else
                    {
                        ICF_PRINT(((icf_uint8_t *)"Some error occured while finding the msg type\n"));
                    }
#ifdef ICF_SECURE_SUPPORT
                    if (p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask & ICF_TRANSPORT_TLS)
                    {
                        if (dMsgType == SipMessageRequest)
                        {
                            p_ssa_pdb->p_ssa_ctxt->p_conn_table_req_entry = 
                                p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry;
                        }
                        else if (dMsgType == SipMessageResponse)
                        {
                            p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry = 
                                p_ssa_pdb->p_ssa_ctxt->p_conn_table_req_entry;
                        }
                        if(ICF_NULL != p_call_obj->pUasTransaction->pSipMsg)
                            pMessage = p_call_obj->pUasTransaction->pSipMsg;
                        if (icf_ssa_is_alias_present_in_Via(pMessage))
                        {
                            ICF_PRINT(("Alias header is present"));
                            p_ssa_pdb->p_ssa_ctxt->bitmask_1 |= ICF_ALIAS_HEADER_PRESENT;
                            if (0 != ICF_PURGE_TIMER_DURATION)
                                p_ssa_pdb->p_conn_table_entry->is_persistent = ICF_TRUE;
                        }
                    }
#endif
                    p_ssa_pdb->p_conn_table_entry->ref_count++;
                    p_ssa_pdb->p_conn_table_entry->timer_count = 0;
                    ICF_SSA_PRINT_CONN_DB(p_ssa_pdb->p_conn_table_entry)
                }

                 if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                            p_call_obj->pUasTransaction->pSipMsg,
                            &p_method,p_sdf_error))

                {
                    ret_val = ICF_FAILURE;
                }
                /* Use filter criteria to determine handler application, fetch app conf 
                   accordingly */
                if ( ICF_FAILURE == icf_ssa_map_req_to_app_filter (p_ssa_pdb,
                            p_call_obj->pUasTransaction->pSipMsg,&app_id, p_ecode))
                {
                    ICF_PRINT(((icf_uint8_t *)"\nFailure in Mapping Request to Handler Application"));
                    ret_val = ICF_FAILURE;
                }

                /* Use filter criteria to get the handler application 
                   and put it in SSA pdb, assign this app id in uascallbacks
                   ,where map_filter was called previously */
                else if(ICF_APP_ID_INVALID == app_id)
                {
                    send_failure_resp = ICF_TRUE;
                    p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_405;
                    /* The most obvious reason for sending 405 is that none of 
                     * the configured applications supports the received method.
                     * But the 405 response must contain Allow header. So we 
                     * will form the Allow header from the profile of 1st app.
                     * ICF always assigns app_id=1 to the 1st app.
                     */
                    app_id = 1;
                    ret_val = icf_cfg_fetch_app_conf(
                            p_ssa_pdb->p_glb_pdb, app_id,
                            &(p_ssa_pdb->p_app_conf),p_ssa_pdb->p_ecode);

                }
                else if(ICF_FAILURE == icf_cfg_fetch_app_conf(p_ssa_pdb->p_glb_pdb,
                            app_id,
                            &(p_ssa_pdb->p_app_conf),
                            p_ssa_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                /* Break the else if flow here for sending failure response 
                  in case of handler application not found */
                if(ICF_FAILURE == ret_val)
                {
                }


                /* Validate whether the method in message is a basic one and the application
                   supports it. This is done because uatk always accepts the basic methods
                   even if they are not specified in the user profile */
#if 0          
                /* Commented  code .Wrongly merged code */
                else if(ICF_FAILURE == icf_ssa_validate_basic_method(p_ssa_pdb,
                            p_call_obj->pUasTransaction->pSipMsg,
                            &send_failure_resp))
                {
                    ret_val = ICF_FAILURE;
                }
#endif
                /* 6_3 merge */
#if 0
                    else if(ICF_TRUE == send_failure_resp)
                    {
                        p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_405;
                    }


                /* Break the else if flow here for sending failure response 
                   in case of handler application not found */
                if(ICF_FAILURE == ret_val)
                {
                }
#endif
                else if(Sdf_co_fail == sdf_ivk_uaInitInitData(&p_profile, &sdf_error))
                {
                    ret_val = ICF_FAILURE;
                }
                else if(ICF_FAILURE == icf_ssa_make_app_profile(p_profile,p_ssa_pdb))
                {
                    sdf_ivk_uaFreeInitData(p_profile);
                    ret_val = ICF_FAILURE;
                }
                else if (Sdf_co_fail == sdf_ivk_uaSetInitDataInCallObject\
                        (p_call_obj, p_profile,\
                         p_sdf_error))
                {
                    sdf_ivk_uaFreeInitData(p_profile);
                    /*Raise a Major error*/
                    ret_val = ICF_FAILURE;
                }
                /* Free the profile after Setting it in the call-object */
                else if(Sdf_co_fail == sdf_ivk_uaFreeInitData(p_profile))
                {
                    ret_val = ICF_FAILURE;
                }
/*NOTIFY_OUT_OF_DIALOG_START*/

                /*else if(Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                  p_call_obj->pUasTransaction->pSipMsg,
                  &p_method, p_sdf_error))
                  {
                  ret_val = ICF_FAILURE;
                  }
                  else if( 0 != icf_port_strcmp(p_method,"NOTIFY")	&& 
                  Sdf_en_invalid == sdf_ivk_uaIsSupportedMsgBody(p_call_obj,
                  p_call_obj->pInitData,p_sdf_error))*/
 /*NOTIFY_OUT_OF_DIALOG_END*/
                /* We are not calling this API in case of INVITE message.
                 * The handling done by this API is incorporated in the function
                 * icf_ssa_validate_content_disp called in the callback 
                 * sdf_cbk_uaNewCallReceived.This change is done for ETSI 
                 * conformance case SIP_CC_TE_CR_V_005
                 */
                 else if((0 != Sdf_mc_strcasecmp(p_method,(icf_int8_t *)"INVITE")) &&
                         (Sdf_en_invalid == sdf_ivk_uaIsSupportedMsgBody(p_call_obj,
											p_call_obj->pInitData,p_sdf_error)))
                 {
                    /* sdf_ivk_uaIsSupportedMsgBody function has called 
                     * sdf_ivk_uaRejectRequest so we will directly send the
                     * message 
                     */
                    send_failure_resp = ICF_TRUE;
                    rej_request_called = ICF_TRUE;

                }

                /* We are not setting any media parameters here as we 
                 * will not use
                 * SIP UA tk functionality for Media Handling
                 *(here we are diverting from HSSUA functionality*/

                /* This condition makes use of the shortcut expression 
                   evaluation. If send_failure_resp is true, sdf_ivk_uaHandleCall
                   will not be called */
                /* commented for ETSI conformance case 
                 * SIP_CC_TE_CR_V_005
                */
                /* 
                p_event_ctxt->dSdfDisabledChecks |= 
                                Sdf_mc_disableRequireCheck;
                */

                if(ICF_FALSE == send_failure_resp)
                  {
                      /* Validate whether the method in message is a basic one and the application
                         supports it. This is done because uatk always accepts the basic methods
                         even if they are not specified in the user profile */
                      if(ICF_FAILURE == icf_ssa_validate_basic_method(p_ssa_pdb,
                            p_call_obj->pUasTransaction->pSipMsg,
                            &send_failure_resp))
                       {
                          ret_val = ICF_FAILURE;
                       }
                  }

                if (ICF_SUCCESS != ret_val)
                {
                    /* Dont do anything */
                }
                else if((ICF_TRUE == send_failure_resp) || 
                        (Sdf_co_fail == sdf_ivk_uaHandleCall(&p_call_obj,\
                                                             p_event_ctxt, p_sdf_error)))
                {
                    icf_uint16_t resp_code = 0;
                    Sdf_st_overlapTransInfo 	*pOverlapTransaction = Sdf_co_null;
                    SipMessage					*pMessage = Sdf_co_null;
                    Sdf_st_transaction 			*pTransaction = Sdf_co_null;
                    Sdf_st_error                SdfError;

                    if(p_call_obj->pUasTransaction->pSipMsg !=Sdf_co_null)
                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                    else
                        pMessage = p_call_obj->pUacTransaction->pSipMsg;

                    if ((ICF_TRUE == rej_request_called) || 
                            (p_sdf_error->errCode == Sdf_en_badExtensionsError) ||
                            (p_sdf_error->errCode == Sdf_en_unknownMethodError) ||
                            (p_sdf_error->errCode == Sdf_en_noCallExists) ||
                            (p_sdf_error->errCode == Sdf_en_lowSessionTimerIntervalError))
                    {
                       if(p_sdf_error->errCode == Sdf_en_noCallExists)
                        {
                            resp_code = 481;
                        }
                        /* 
                         * Bad Extensions received in 
                         * Require header. 420 is already
                         * formed by UATK. Send the SipMsg by using .
                         * sdf_ivk_uaGetTransactionForSipMsg
                         */
                        /* Fix - 420 not being sent out as option was not properly set */
                        dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

                        if(sdf_ivk_uaGetTransactionForSipMsg(\
                                    p_call_obj, pMessage, &pTransaction, \
                                    &pOverlapTransaction, &SdfError) == Sdf_co_success)
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
                        else
                        {
                            ret_val = ICF_FAILURE;
                        }
                    }
                    else
                    {
                        /*
                         * If there is a failure in the callback processing
                         * SSA will be sending a failure response from here
                         * instead of the callback.
                         * We will be setting ICF_SSA_SSA_CTXT_STORED
                         * so that the SSA ctxt is not freed in the return
                         * path.
                         */
                        if (ICF_NULL == p_ssa_ctxt->failure_bitmask)
                        {
                            p_ssa_ctxt->failure_bitmask |=
                                ICF_SSA_FAILURE_RESP_CODE_500;

                        } /* if (ICF_NULL == p_ssa_ctxt-... */
                        /* Map the response code */
                        if (ICF_NULL != (ICF_SSA_FAILURE_RESP_CODE_503
                                    &(p_ssa_ctxt->failure_bitmask)))
                        {
                            resp_code = 503;
                        }
                        else if((ICF_NULL != 
                            (ICF_SSA_FAILURE_RESP_CODE_501
                            &(p_ssa_ctxt->failure_bitmask))))
                        { 
                            resp_code = 501;
                        }
                        /* Fix for CSR-5213984 */
                        else if((ICF_NULL != 
                            (ICF_SSA_FAILURE_RESP_CODE_414
                            &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 414;

                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_481
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 481;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_488
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 488;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_415
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 415;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_482
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 482;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_416
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 416;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_400
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 400;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_505
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 505;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_404
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 404;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_405
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 405;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_487
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 487;
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_403
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 403;
                        }
                        else if((ICF_NULL !=
                                    (ICF_SSA_FAILURE_RESP_CODE_603
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 603;
                        }
                        else
                        {
                            if ( (p_sdf_error->errCode == Sdf_en_cseqValueExceeded) 
#ifdef SDF_SESSION_TIMER
#ifdef ICF_SESSION_TIMER
                                || (((ICF_NULL != p_ssa_ctxt->p_call_ctxt && 
									ICF_TRUE == p_ssa_ctxt->p_call_ctxt->session_timer_enable)) &&
                                   ((p_sdf_error->errCode == Sdf_en_invalidMinSe) ||
                                    (p_sdf_error->errCode == Sdf_en_lowsessionTimerError)))
#endif                   
#endif
                               )
                            {
                                resp_code = 400;
                            }
                            else
                            {
                                /* default response code is 500 */
                                resp_code = 500;
                            }

                        }
                        /*
                         * Form and send failure response
                         */
                        dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
                        p_ssa_ctxt->p_call_obj = p_call_obj;
                        p_call_obj->pAppData->pData = 
                            (Sdf_ty_pvoid *)p_ssa_ctxt;

                        if (Sdf_co_fail == sdf_ivk_uaRejectRequest(p_call_obj,p_call_obj->pInitData,
                                    (icf_uint16_t)resp_code,&pOverlapTransaction,p_sdf_error))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        if( pOverlapTransaction != Sdf_co_null )
                            pMessage = pOverlapTransaction->pSipMsg;
                        else
                            pMessage = 
                                (p_call_obj)->pUasTransaction->pSipMsg;

                    }
                    if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(\
                                p_call_obj->pUasTransaction->pSipMsg,\
                                &p_method, p_sdf_error))
                    {
                        /* Add an informational trace*/
                        ret_val = ICF_FAILURE;
                    }
                    else if(ICF_NULL != p_method)
                    {
                        icf_port_strcpy((icf_uint8_t *)methodName,(icf_uint8_t *)p_method);
#ifdef ICF_STAT_ENABLE
                        if(icf_port_strcmp(methodName,(icf_uint8_t *)"CANCEL")== 0)
                        {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                              stat_requests.stat_inbound_req.stats_CANCEL)
                        }
                        else if(icf_port_strcmp(methodName,(icf_uint8_t *)"UPDATE")== 0)
                        {
                         ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                              stat_requests.stat_inbound_req.stats_UPDATE)
                        }
                        else if(icf_port_strcmp(methodName,(icf_uint8_t *)"BYE")== 0)
                        {
                         ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                              stat_requests.stat_inbound_req.stats_BYE)
                        }
                        else if(icf_port_strcmp(methodName,(icf_uint8_t *)"PRACK")== 0)
                        {
                         ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                              stat_requests.stat_inbound_req.stats_PRACK)
                        }
                        else if(icf_port_strcmp(methodName,(icf_uint8_t *)"REFER")== 0)
                        {
                         ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                              stat_requests.stat_inbound_req.stats_REFER)
                        }
						else if(icf_port_strcmp(methodName,(icf_uint8_t *)"MESSAGE")== 0)
                        {
                         ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                              stat_requests.stat_inbound_req.stats_MESSAGE)
                        }

                        icf_ssa_resp_stats(p_ssa_pdb, methodName, resp_code, 1);
#endif
                    }
                    if( ICF_FAILURE == ret_val)
                    {
                        /* dont do any thing */
                    }
                    else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                p_call_obj,
                                pMessage,
                                &dOptions,p_sdf_error))
                    {
                        ret_val = ICF_FAILURE;
                    }

                    /*
                     * Now ,try to add an entry into
                     * the callid-ssa ctxt map so that
                     * when ACK is received fro this
                     * failure response,
                     * SSA is able to map the SSA ctxt and
                     * do the appropiate handling
                     */
                    else if (0 == icf_port_strcmp(methodName/*p_method*/,(icf_uint8_t *)"INVITE"))
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
                            /* 
                             * Increment the refcount of the call object
                             * here as we are freeing the call object at the 
                             * the end of this leg and in the case of a new 
                             * call,the temporary call object is the one which
                             * becomes the permanent call object,hence need to
                             * increment the ref count
                             */
                            HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                            ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                                    ICF_SSA_INTER_MODULAR_MSG);
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                        }
                    }

                } 
                /* Response was sent For Unexpected sip message and temp call
                 * object should not be made to permanent call object
                 * fix for mem leak after timer J expiry
                 */
                else if ( !(((icf_ssa_ctxt_st*)(p_event_ctxt->pData))->bitmask
                            & IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV))
                {

                    /*
                     * The call object (which was considered temp till 
                     * invocation of HandleCall) will be added in the map
                     * in the callback processing.  Also, the SSA CTXT
                     * will be allocated along with the CC Context in 
                     * Callback processing for incoming calls
                     */

                    ((icf_ssa_ctxt_st*)(p_event_ctxt->pData))->bitmask 
                        |= ICF_SSA_SSA_CTXT_STORED;
                    if (( Sdf_en_protoTcp == p_event_ctxt->dProtocol)
#ifdef ICF_SECURE_SUPPORT
                            || (Sdf_en_protoTls == p_event_ctxt->dProtocol ))
#else
                        )
#endif	

                        {
                            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:NewCallOnTCPConn, setting \
                                        the fd in SSA ctxt"));
                            /* 7.0 */
                            if (dMsgType == SipMessageRequest)
                            {
                            ((icf_ssa_ctxt_st*)(p_event_ctxt->pData))->
                                p_conn_table_resp_entry = p_ssa_pdb->p_conn_table_entry;
                            }
                            else
                            {
                            ((icf_ssa_ctxt_st*)(p_event_ctxt->pData))->
                                p_conn_table_req_entry = p_ssa_pdb->p_conn_table_entry;
                            }

                            /* save the ssa context in conn_db */
#if 0
                            p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_conn_db
                                [p_ssa_pdb->ssa_conn_db_index].p_ssa_ctxt = 
                                (icf_ssa_ctxt_st*)(p_event_ctxt->pData);
#endif
                        }
                    /*Add an info trace here*/
                    /* 
                     * Increment the refcount of the call object
                     * here as we are freeing the call object at the 
                     * the end of this leg and in the case of a new 
                     * call,the temporary call object is the one which
                     * becomes the permanent call object,hence need to
                     * increment the ref count
                     */
                   if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(\
                                p_call_obj->pUasTransaction->pSipMsg,\
                                &p_method, p_sdf_error))
                    {
                        /* Add an informational trace*/
                        ret_val = ICF_FAILURE;
                    }

                    else if(0 != icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"ACK"))
                    {
                        /* spr16769: We don't need to increment ref count 
                           in case of ACK as it does not creates any dialog */
                        HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                    }
                    ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                            ICF_SSA_INTER_MODULAR_MSG);
                }
                else
                {
                  /* spr17154: This leg will be hit when the Incoming INVITE with To
                   * tag comes and sdf_cbk_uaUnexpectedRequestReceived
                   * callback is invoked then 481 is sent timer is started
                   * we do not have to free call object now but after timer
                   * expiry so increment ref count */
                    HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                }
                if(ICF_SSA_RESPONSE_SENT_TO_NETWORK == (ICF_SSA_RESPONSE_SENT_TO_NETWORK 
                         & p_ssa_pdb->common_bitmask))
                {
                     ICF_SSA_RESET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                           ICF_SSA_INTER_MODULAR_MSG) 
                     ICF_SSA_RESET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                           ICF_SSA_RESPONSE_SENT_TO_NETWORK)
                }
            }
        }

        if ( ICF_FAILURE != ret_val)
        {/*ICF_INT_MODULE_INVALID*/
        }
        /* Freeing the local reference */
        sdf_ivk_uaFreeCommonInfo(p_common_info);

        /*spr16769 This bitmask was set in icf_ssa_parse_require_header
          when failure occurs due to invalid require, proxy-require 
          header in Incoming INVITE. However after sending 420 the call object
          is not released. Below we check the bitmask to identify the scenario
          and invoke sdf_ivk_uaFreeCallObject for releasing the call obj */
        if(ICF_SSA_REQUIRE_VALUE_IS_NOT_SUPPORTED ==
                (ICF_SSA_REQUIRE_VALUE_IS_NOT_SUPPORTED & 
                 p_ssa_pdb->common_bitmask))
        {
            sdf_ivk_uaFreeCallObject(&p_call_obj);
        }

    }
    else /*Nw call id was null*/
    {
        ret_val = ICF_FAILURE;
        sdf_ivk_uaFreeCommonInfo(p_common_info);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_intrnl_msg
 *
 * DESCRIPTION:     This routine checks the destination ID value set in 
 *                  p_internal_msg (filled by callback processing) and then
 *
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t     icf_ssa_process_otg_intrnl_msg(
        icf_ssa_pdb_st       *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_glb_pdb_st           *p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    /*Internal message has been populated by the callback processing*/
    icf_internal_msg_st      *p_internal_msg = 
        p_ssa_pdb->p_internal_msg;
    icf_int_module_id_t      dest_module_id = p_ssa_pdb->dest_module_id;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_glb_pdb = p_glb_pdb;
    if ( ICF_INT_MODULE_CFG == dest_module_id)
    {
        if (ICF_FAILURE == icf_cfg_process_mesg(p_internal_msg))
        {
            ret_val = ICF_FAILURE;
        }
    }
    else if ( ICF_INT_MODULE_CM == dest_module_id)
    {
        if (ICF_FAILURE == icf_cm_process_mesg(p_internal_msg))
        {
            ret_val = ICF_FAILURE;
        }
    }
	else if ( ICF_INT_MODULE_SIC == dest_module_id)
    {
        if (ICF_FAILURE == icf_sic_process_mesg(p_internal_msg))
        {
            ret_val = ICF_FAILURE;
        }
    }
    else if ( ICF_INT_MODULE_MMI == dest_module_id)
    {
        if (ICF_FAILURE == icf_mmi_process_mesg(p_internal_msg))
        {
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"[SSA]:OTG Intrl Msg routine called with invalid Internal Module\n"));
        /*Raise Major Error here*/
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_handle_stray_req
 *
 * DESCRIPTION:     This routine handles stray req by sending 481
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t     icf_ssa_handle_stray_msg(
                        icf_ssa_pdb_st       *p_ssa_pdb,
                        Sdf_st_eventContext     *p_event_ctxt,
                        Sdf_st_callObject       *p_call_obj,
                        Sdf_st_error            *p_sdf_error,
                        icf_error_t          *p_ecode)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_glb_pdb_st           *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_ssa_glb_data_st      *p_ssa_glb_data = p_ssa_pdb->p_ssa_glb_data;
    SipMessage		            *p_sip_msg = Sdf_co_null;
    Sdf_ty_s8bit                *p_method = Sdf_co_null;
    Sdf_ty_u16bit               failure_resp_code = 481;
    SipOptions                  sip_options ;
	Sdf_st_overlapTransInfo 	*p_overlap_txn = Sdf_co_null;
    Sdf_st_initData             *p_default_profile =
        p_ssa_glb_data->p_default_profile;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    /* To remove compiler warning */
    (void)p_event_ctxt;    

    p_glb_pdb = p_glb_pdb;
    p_ecode = p_ecode;

    if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(\
                p_call_obj->pUasTransaction->pSipMsg,\
                &p_method, p_sdf_error))
    {
        /* Add an informational trace*/
        ret_val = ICF_FAILURE;
    }
    else if (0 != icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *) "ACK"))
    {
Sdf_st_initData     *p_def_prof = Sdf_co_null;
        /*It was an Invite, so we will receive an ACK
         * to our failure response, so we add this
         * temporary call object to our map
         */
        if ( Sdf_co_fail == 
                sdf_ivk_uaInitInitData(&p_def_prof, p_sdf_error))
        {
            ret_val = ICF_FAILURE;
        }
        else if ( ICF_FAILURE == 
                icf_ssa_clone_profile_data(p_glb_pdb,
                    p_def_prof, p_default_profile,p_ecode))
        {
            sdf_ivk_uaFreeInitData(p_def_prof);
            ret_val = ICF_FAILURE;
        }
        else if (Sdf_co_fail == 
                sdf_ivk_uaSetInitDataInCallObject(p_call_obj, \
                    p_def_prof, p_sdf_error))
        {
            ret_val = ICF_FAILURE;
        }
        else if (Sdf_co_fail == sdf_ivk_uaFreeInitData (
                    p_def_prof))
        {
               /*Raise a Major error*/
               ret_val = ICF_FAILURE;
        }
        else if (Sdf_co_fail == 
                sdf_ivk_uaRejectRequest(p_call_obj, \
                    p_default_profile, failure_resp_code, \
                    &p_overlap_txn, p_sdf_error))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            sip_options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

            if( p_overlap_txn != Sdf_co_null )
                p_sip_msg = p_overlap_txn->pSipMsg;
            else
                p_sip_msg = (p_call_obj)->pUasTransaction->pSipMsg;
                /*This API is used to send a message to the peer entity */
            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_call_obj,
                        p_sip_msg,
                        &sip_options,p_sdf_error))
            {
                    /*Raise an error of major category here*/
                    ret_val = ICF_FAILURE;
            }
        }
    }
#ifdef ICF_STAT_ENABLE
    if(ret_val != ICF_FAILURE)
	{
		icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)p_method,481,1);
	}
#endif
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_handle_decode_failure
 *
 * DESCRIPTION:     This routine handles the analysis of error returned by
 *                  DecodeMsg API and sends appropriate error message if 
 *                  required
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t     icf_ssa_handle_decode_failure(
                        icf_ssa_pdb_st       *p_ssa_pdb,
                        Sdf_st_eventContext     *p_event_ctxt,
                        Sdf_st_callObject       *p_call_obj,
                        Sdf_st_error            *p_sdf_error,
                        icf_error_t          *p_ecode)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st          *p_ssa_ctxt =
        (icf_ssa_ctxt_st*)(p_event_ctxt->pData);
    icf_glb_pdb_st           *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_ssa_glb_data_st      *p_ssa_glb_data = p_ssa_pdb->p_ssa_glb_data;
    icf_ssa_ctxt_st          *p_mapped_ssa_ctxt = ICF_NULL;
    Sdf_st_initData             *p_default_profile =
        p_ssa_glb_data->p_default_profile;

    SipMessage		            *p_sip_msg = Sdf_co_null;
    Sdf_ty_s8bit                *p_method = Sdf_co_null;
    Sdf_ty_u16bit               failure_resp_code = 0;
    SipOptions                  sip_options ;
    Sdf_st_overlapTransInfo 	*p_overlap_txn = Sdf_co_null;
    icf_ssa_tcpm_conn_data_st    *p_resp_conn = ICF_NULL;
    icf_boolean_t               add_call_id_failed = ICF_FALSE;
    /* New error code is defined so as the value of p_sdf_error,which is passed
       to this function is not overwritten */
    Sdf_st_error                Sdf_error = {0,0,0,"\0"};
    /*Fix for CSR 7945729 SPR 20251*/
    /*Variable to store p_method fetched from UATK data*/
    icf_int8_t                 *p_method_temp = ICF_NULL;


    ICF_FUNCTION_ENTER(p_glb_pdb)

    if (ICF_NULL != p_ssa_pdb->p_conn_table_entry)
    {
        ret_val = icf_ssa_check_for_new_conn(p_ssa_pdb,
            p_ssa_ctxt,
            p_ssa_ctxt->p_call_obj);
        if(ICF_SUCCESS == 
           icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, p_ssa_pdb->p_conn_table_entry))
        { 
             p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |=
                            ICF_CONN_TYPE_RESPONSE;
             if ((p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask & ICF_TRANSPORT_TLS) &&
                        (icf_ssa_is_alias_present_in_Via(p_sip_msg)))
              {
                  ICF_PRINT(((icf_uint8_t *)"Its persistent TLS" \
                        "connection.Alias found\n"));
                  if (0 != ICF_PURGE_TIMER_DURATION)
                     p_ssa_pdb->p_conn_table_entry->is_persistent = ICF_TRUE;
              }

              ICF_SSA_PRINT_CONN_DB(p_ssa_ctxt->p_conn_table_resp_entry)
        }
       else
       {
           ret_val = ICF_FAILURE;
       }
    } /* p_ssa_ctxt->ssa_conn_db_index) */

    if (Sdf_en_notImplemented == 
            p_sdf_error->errCode)
    {
        /*
         * Raise a Trace that Request type received is not implemented
         * Failure will not be returned here
         * Get the method From the SipMessage and stop the timer
         * if it is a ACK
         */
        if (p_call_obj->pUasTransaction->pSipMsg != Sdf_co_null)
            p_sip_msg = p_call_obj->pUasTransaction->pSipMsg;
        else
            p_sip_msg = p_call_obj->pUacTransaction->pSipMsg;

        failure_resp_code = 501;
    }
    else if (Sdf_en_protocolMismatchInVia == 
            p_sdf_error->errCode)
    {
        SipHeader       via_hdr;
        Sdf_ty_s8bit    *p_protocol = Sdf_co_null;

        /*
         * Modify the topmost Via header to the actual
         * protocol type
         */
        if (SipFail == sip_getHeaderAtIndex(p_call_obj->pUasTransaction\
                    ->pSipMsg, SipHdrTypeVia, &via_hdr, 0, \
                    (SipError *)&(p_sdf_error->errCode)))
        {
            /*
             * Raise an informational trace here
             */
            ret_val = ICF_FAILURE;
        }
        if ( ICF_FAILURE != ret_val)
        {
            if (Sdf_en_protoTcp == p_event_ctxt->dProtocol)
                Sdf_mc_strdup(p_protocol, "SIP/2.0/TCP");
#ifdef ICF_SECURE_SUPPORT
            else if (Sdf_en_protoTls == p_event_ctxt->dProtocol)
                Sdf_mc_strdup(p_protocol, "SIP/2.0/TLS");
#endif	
            else
                Sdf_mc_strdup(p_protocol, "SIP/2.0/UDP");

            if (SipFail == sip_setSentProtocolInViaHdr(&via_hdr, \
                        p_protocol, (SipError *)&(p_sdf_error->errCode)))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                failure_resp_code = 400;
            }
            sip_freeSipHeader(&via_hdr);
        }
    }
#ifdef SDF_REFER
    /* If "Refer-To" header is missing in REFER request or
     * if "Replaces" header has come in any non-INVITE request,or
     * if multiple Instances of "Replaces" header is present
     * then reject the request with 400 */
    else if ((Sdf_en_noReferToHdrError == p_sdf_error->errCode) ||
            (Sdf_en_replacesInNonInviteRequestError == p_sdf_error->errCode)||
            (Sdf_en_badReplacesHeaderError == p_sdf_error->errCode))
    {
        failure_resp_code = 400;
    }
#endif
    else
    {
        /* Default response code is 400 */
        failure_resp_code = 400;
    }

    if ( ICF_FAILURE != ret_val)
    {
        /*
         * non null UAS txn means that either a request was received
         * or an ACK was received
         */
        if (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg)
        {
            if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(\
                        p_call_obj->pUasTransaction->pSipMsg,\
                        &p_method, p_sdf_error))
            {
                /* Add an informational trace*/
                ret_val = ICF_FAILURE;
            }
            else if (0 == icf_port_strcmp((icf_uint8_t *)p_method, (icf_uint8_t *)"ACK"))
            {
                /* No response to be sent, Add an informational trace*/
#ifdef ICF_STAT_ENABLE
                if( ret_val != ICF_FAILURE)
                {
                    if(failure_resp_code == 400)
                    {
                        icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)p_method,400,1);
                    }
                }
#endif
            }
            /*Fix for CSR 7945729 SPR 20251*/
            /*Added failure handling for CANCEL request*/
            else if ((0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"INVITE")) ||
                    (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"CANCEL")))
            {
                Sdf_st_initData     *p_def_prof = Sdf_co_null;
                /*It was an Invite, so we will receive an ACK
                 * to our failure response, so we add this
                 * temporary call object to our map
                 */
                 /*Create copy of p_method for local use, as p_method 
                 *can be removed from sdf_ivk_uaRejectRequest*/
                 Sdf_mc_strdup(p_method_temp, p_method);
#ifdef ICF_STAT_ENABLE
                if( ret_val != ICF_FAILURE)
                {
                    if(failure_resp_code == 400)
                    {
                        icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)p_method,400,1);
                    }
            	}
#endif
                if ( Sdf_co_fail == 
                        sdf_ivk_uaInitInitData(&p_def_prof, p_sdf_error))
                {
                    ret_val = ICF_FAILURE;
                }
                else if ( ICF_FAILURE == 
                        icf_ssa_clone_profile_data(p_glb_pdb,
                            p_def_prof, p_default_profile,p_ecode))
                {
                    sdf_ivk_uaFreeInitData(p_def_prof);
                    ret_val = ICF_FAILURE;
                }
                else if (Sdf_co_fail == 
                        sdf_ivk_uaSetInitDataInCallObject(p_call_obj, \
                            p_def_prof, p_sdf_error))
                {
                    /* SPR14498 removed MEMFREE*/
                    ret_val = ICF_FAILURE;
                }
                else if (Sdf_co_fail == sdf_ivk_uaFreeInitData (
                            p_def_prof))
                {
                    /*Raise a Major error*/
                    ret_val = ICF_FAILURE;
                }
                else if (Sdf_co_fail == 
                        sdf_ivk_uaRejectRequest(p_call_obj, \
                            p_def_prof, failure_resp_code, \
                            &p_overlap_txn, p_sdf_error))
                {
                    /* SPR14498 removed MEMFREE*/
                    ret_val = ICF_FAILURE;
                }
                else
                {

                    sip_options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
                    /*SSA conext should be stored only if the 
                     * incoming message was INVITE and after sending failure
                     * response ACK is expected for it. In case of CANCEL, attempt
                     * was made to store the ssa context that fails and then froceful
                     * call clear was invoked which was not required.
                     */ 

                    
                    if(ICF_NULL == p_method_temp)
            		{
               			/* do nothing */
            		} 

                    else if (0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"INVITE"))
                    {
                              sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                   p_sdf_error);


                    p_call_obj->pAppData->pData = (Sdf_ty_pvoid)p_ssa_ctxt;
                    p_ssa_ctxt->p_call_obj = p_call_obj;
                    if (ICF_FAILURE == icf_ssa_add_callid_callobj_map(p_ssa_pdb,                            
                                          p_ssa_ctxt,
                                          p_ecode))
                    {
                        add_call_id_failed = ICF_TRUE;
                    }
                    /* 
                     * we have set this flag here, irrespective of whether
                     * send_to fails or succeeds as in case of failure,
                     * FreeCalObj will be called in the calling routine which 
                     * hould invoke FreeAppData which will then release 
                     * the memory for SSA ctxt
                     */
                    ((icf_ssa_ctxt_st*)(p_event_ctxt->pData))->bitmask 
                        |= ICF_SSA_SSA_CTXT_STORED;
                    }

                    if( p_overlap_txn != Sdf_co_null )
                        p_sip_msg = p_overlap_txn->pSipMsg;
                    else
                        p_sip_msg = (p_call_obj)->pUasTransaction->pSipMsg;
#if 0
                    sdf_ivk_uaUpdateDestinationInfo(
                          p_call_obj, Sdf_co_null, p_sip_msg, p_sdf_error);
 
                    pTranspaddr = (SipTranspAddr*)sdf_memget(
                       Sdf_mc_callHandlerMemId, sizeof(SipTranspAddr), 
                       p_sdf_error);
   
                    pTranspaddr->pHost = Sdf_mc_strdupCallHandler(
                       p_call_obj->pUasTransaction->pDestInfo->pIp);
                    pTranspaddr->dPort = 
                       p_call_obj->pUasTransaction->pDestInfo->dPort;
                    pTranspaddr->dSockFd = p_event_ctxt->pTranspAddr->dSockFd;

                    if (p_call_obj->pUasTransaction->pDestInfo->dScheme == 
                           Sdf_en_protoUdp)
                        dTranspType = dTranspType | SIP_UDP;
                    else
                        dTranspType = dTranspType | SIP_TCP;

                    txn_data.pRefCallObject = p_call_obj;

                    p_event_ctxt->pData = (Sdf_st_txnContextData *)&txn_data;
   
                    if ((sip_sendMessage(p_sip_msg, &dOptions, pTranspaddr, 
                                        dTranspType, 
                                        (SipEventContext*)p_event_ctxt, 
                                        (SipError*)&(p_sdf_error->stkErrCode))) 
                          == SipFail)
                    {
                        printf("*** sip_sendMessage failed!\n");
                    }
#endif
                    /* Since decode failure is to be sent in same connection.
                     * So setting it */
                    p_resp_conn = p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry;

                    p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry = p_ssa_pdb->p_conn_table_entry;
                    if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                p_call_obj,
                                p_sip_msg,
                                &sip_options,p_sdf_error))
                    {
                        if (p_call_obj->dRefCount > 1)
                        {
                            HSS_LOCKEDDECREF(p_call_obj->dRefCount);
                        }
                        if (p_call_obj->pUasTransaction->dRefCount > 1)
                        {
                            HSS_LOCKEDDECREF(p_call_obj->pUasTransaction->dRefCount);
                        }
                        /*Fix for CSR 1-6792719 referred from SPR : 18432*/ 
                        /*Free Txn Key and buffer when IPTK could not send
                          failure response for decode failure of an INVITE*/
                        /*  KlocWorks Fix */
                        if (ICF_NULL != p_call_obj->pUasTransaction->pTxnKey)
                        {    
                            icf_ssa_free_sip_txn_data_on_failure(
                                    p_call_obj->pUasTransaction->pTxnKey);
                        }    

                        /*Raise an error of major category here*/
                        ret_val = ICF_FAILURE;
                    }
                    /* Resuming the connection */

                    /*
                     * Set the bit corresponding to decode fail 
                     * in the failure bitmask present in the SSA CTXT.
                     * This is done so that when we receive an ACK for this
                     * failure response,the same is not intimated to CM/CC
                     */
                    else if (p_method_temp && (0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"INVITE"))) 
                    {
			            p_event_ctxt->pData = p_ssa_ctxt;
                        HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                        p_ssa_ctxt->failure_bitmask |=
                            ICF_SSA_DECODE_FAILURE ;
                    }
                    p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry = p_resp_conn;

                    if (add_call_id_failed) 
		            {
                        /* spr16358 : In case of incoming INVITE if via is in 
                           invalid then call object was not getting free 
                           because p_call_obj->dRefCount turns out as 2 so
                           decremented the same before call to 
                           icf_ssa_process_forceful_call_clear */
                        if (p_call_obj->dRefCount > 1)
                        {
                            HSS_LOCKEDDECREF(p_call_obj->dRefCount);
                        }
                        icf_ssa_process_forceful_call_clear(p_ssa_pdb);
		                 p_ssa_pdb->common_bitmask |= ICF_SSA_CALL_OBJ_ALREADY_FREED;
		            }

#if 0
                    /*To remove memory leaks for BYE decode failure case.*/
                    sdf_fn_uaAbortTransaction(
                          p_call_obj,
                          p_call_obj->pUasTransaction->pTxnKey, 
                          ICF_NULL,p_sdf_error);
                    sdf_memfree(0, (Sdf_ty_pvoid *)&pTranspaddr->pHost, 
                                p_sdf_error);
                    sdf_memfree(0, (Sdf_ty_pvoid *)&pTranspaddr, p_sdf_error);

                    sdf_ivk_uaFreeCallObject(&p_call_obj);
#endif
                }
            }
            if (Sdf_co_null != p_method_temp)
             {
                 sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                             p_sdf_error);
             }
        }
        else if (Sdf_co_null != p_call_obj->pUacTransaction->pSipMsg)
        {
            
            /*
             * Hitting this leg means that we have received 
             * a response to some request initiated by us.
             * Could be a 1xx/2xx response to an INVITE
             * Here we try to find a matching dialog.If we 
             * find a matching dialog,we will propagate failure
             * with ecode as generic error,else do nothing
             * If there is a SIP decode fail,then the pCallid
             * of the common info of the call object will
             * not be populated.In that case ,just ignore the message
             */
            if(Sdf_co_null != p_call_obj->pCommonInfo->pCallid)
            {
              
               if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(\
                        p_call_obj->pUacTransaction->pSipMsg,\
                        &p_method, &Sdf_error))
                {
                  /* Add an informational trace*/
                  ret_val = ICF_FAILURE;
                  p_sdf_error = &Sdf_error;
                }

              /*If contact header is not received in final 2xx response
               * of a non-invite method*/
              if((Sdf_en_mandatoryHdrMissing == p_sdf_error->errCode) &&
                 (0 != icf_port_strcmp((icf_uint8_t *)p_method, (icf_uint8_t *)"INVITE")))
                {
                  sip_freeSipMessage(p_call_obj->pUacTransaction->pSipMsg);
                  *(p_ssa_pdb->p_ecode) = ICF_ECODE_GENERIC_ERROR;
                  ret_val = ICF_FAILURE;
                } 

              /*If contact header is not received in final 2xx response
               * of invite method,then fetch the ssa ctxt and set the bit
               * in its bitmask so as to abort the invite transaction 
               * during the forceful call clear.*/
               else if ((0 == icf_port_strcmp((icf_uint8_t *)p_method, (icf_uint8_t *)"INVITE")) &&
                        (Sdf_en_mandatoryHdrMissing == p_sdf_error->errCode))
                {
                  if( ICF_FAILURE != icf_ssa_map_nw_callid(
                    p_ssa_pdb,
#ifdef ICF_LOOPBACK_SUPPORT
                    p_call_obj,
#endif
                    (icf_uint8_t *)p_call_obj->pCommonInfo->pCallid,
                    &p_ssa_ctxt,
#ifdef ICF_LOOPBACK_SUPPORT
                    p_sdf_error,
#endif
                    p_ecode))
                      {
                        p_ssa_ctxt->bitmask_1 |= ICF_MANDATORY_CONTACT_HDR_MISSING ;
                        sip_freeSipMessage(p_call_obj->pUacTransaction->pSipMsg);
                        *(p_ssa_pdb->p_ecode) = ICF_ECODE_GENERIC_ERROR;
                        ret_val = ICF_FAILURE;
                      }
                }
                     
               /*Sdf_en_mandatoryHdrMissing error code is not received */
               else if( ICF_FAILURE != icf_ssa_map_nw_callid(
                    p_ssa_pdb, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_call_obj, 
#endif
                    (icf_uint8_t *)p_call_obj->pCommonInfo->pCallid,
                    &p_ssa_ctxt, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_sdf_error, 
#endif
                    p_ecode))
                {
                    *(p_ssa_pdb->p_ecode) = ICF_ECODE_GENERIC_ERROR;
                    ret_val = ICF_FAILURE;                    
                }    
            }

        }
        else
        {
            /*
             * If a matching dialog is found return FAILURE and set ecode
             * else do nothing,return SUCCESS
             */
            if(Sdf_co_null != p_call_obj->pCommonInfo->pCallid)
            {
                if ( ICF_FAILURE != icf_ssa_map_nw_callid(
                    p_ssa_pdb, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_call_obj, 
#endif
                    (icf_uint8_t *)p_call_obj->pCommonInfo->pCallid,
                    &p_mapped_ssa_ctxt, 
#ifdef ICF_LOOPBACK_SUPPORT
                    p_sdf_error, 
#endif
                    p_ecode))
                {
                    *(p_ssa_pdb->p_ecode) = ICF_ECODE_GENERIC_ERROR;
                    ret_val = ICF_FAILURE;
                }
            }
        }
    }/*if pUasTransaction*/
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_init
 *
 * DESCRIPTION:     Allocates and initializes global data required by SSA,
 * stores the pointer in p_glb_pdb
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_ssa_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_ssa_data,
        OUT     icf_error_t           *p_ecode)
{
    icf_uint16_t             count = 0;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_glb_data_st      *p_ssa_data = ICF_NULL;
    /*icf_ssa_callid_callobj_map_st    *p_map = ICF_NULL;*/
    icf_ssa_pdb_st           ssa_pdb ;

    Sdf_st_initData             *p_default_profile = Sdf_co_null;
    Sdf_st_error                sdf_error;

    /* Store the pointer to global pdb in p_persistent_glb_pdb */
    p_persistent_glb_pdb = p_glb_pdb; 

    /* initialize the dns buffer related information to NULL */    
#ifdef ICF_DNS_LOOKUP_ENABLED
     ICF_STATIC_MEMGET(p_glb_pdb,
             sizeof(icf_dns_info_st),
             p_glb_pdb->p_glb_data->p_dns_info,
             ICF_RET_ON_FAILURE,
             p_ecode, ret_val)

         if(ICF_FAILURE == ret_val)
         {
             return ICF_FAILURE;
         }
     p_glb_pdb->p_glb_data->p_dns_info->p_dns_buffer_list = ICF_NULL;
     p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id = ICF_TIMER_ID_INVALID;
#endif

    if ( Sdf_co_fail == 
            sdf_ivk_uaInitInitData(&p_default_profile, &sdf_error))
    {
        /*
         * Raise a Critical Error here
         */
        ret_val = ICF_FAILURE;
    }
    else
    {
        Sdf_ty_initToolkitParams init_params;

        icf_port_memset(
                (icf_void_t *)(&init_params),ICF_NULL,
                sizeof(init_params));

#ifdef SDF_USE_PREALLOCBUFFERS
        init_params.dNumPreAllocMsgBuffers = 1000;
        init_params.dPreAllocBufSize = 3072;
#endif

        init_params.dTxnParams.pTxnFetchFunc 	= Sdf_co_null;
        init_params.dTxnParams.pTxnReleaseFunc	= Sdf_co_null;
        /*
         * Set the trace and debug functions
         */
        init_params.pTraceFunc	= Sdf_co_null;
        init_params.pDebugFunc	= Sdf_co_null;

#ifdef ICF_DNS_LOOKUP_ENABLED
        init_params.dDnsParams.pFillDestInfoFromUnknownDnsRec = icf_ssa_cbk_FillDestInfoFromUnknownDnsRec;
        init_params.dDnsParams.pFreeUnknownDnsList=icf_ssa_FreeUnknownDnsList;
#endif

#ifdef  ICF_TRACE_ENABLE
        init_params.pTraceFunc   = 
            (Sdf_ty_uaTraceFunc)icf_ssa_port_stack_trace;
#endif

#ifdef  ICF_DEBUG_TRACE_ENABLE
        init_params.pDebugFunc	=  
            (Sdf_ty_uaDebugFunc)icf_ssa_port_stack_debug;
#endif

#if defined(ICF_ERROR_ENABLE) && !defined(ICF_PORT_SYMBIAN) && !defined(ICF_PORT_WINCE)
        sdf_ivk_setErrorLevel(Sdf_en_allErrors, &sdf_error);
#endif
#ifdef ICF_SECURE_SUPPORT
        /* Initialize the SSL related parameters. */
        Sdf_mc_strdup(init_params.pEntropySource,"/dev/urandom");
#endif
		/*This variable False means only Tag checking will be done for
		 * Dialog identification
		 */
		init_params.dStrictURICheck = Sdf_co_false;

        if( Sdf_co_fail == sdf_ivk_uaInitToolkit (&init_params, &sdf_error))
		{
			ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure InSipToolkitInit"));
			ret_val = ICF_FAILURE;
		}
		else
		{
				ICF_STATIC_MEMGET(p_glb_pdb, ICF_PORT_SIZEOF(
				icf_ssa_glb_data_st),p_ssa_data,
                ICF_RET_ON_FAILURE, p_ecode, ret_val)
				
				/*p_map = p_ssa_data->call_id_obj_map;*/
			
			for ( count = 0; count < ICF_SSA_MAX_OPTIONS_CALL_OBJ; count++)
			{
				/*p_ssa_data->call_id_obj_map[count].p_ssa_ctxt = ICF_NULL;*/
                p_ssa_data->options_map[count].p_ssa_ctxt = ICF_NULL;
                p_ssa_data->options_map[count].context_identifier = ICF_NULL;
			}
            p_ssa_data->p_default_profile = p_default_profile;
            p_ssa_data->ssa_glb_bitmask |= ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS;
            /* Initilaize the user agent string */
            icf_cmn_init_string((icf_string_st *)
                &(p_ssa_data->user_agent_str));

			ssa_pdb.p_glb_pdb = p_glb_pdb;
			ssa_pdb.p_ssa_glb_data = p_ssa_data;

			icf_ssa_init_tcp_sm(&ssa_pdb);

#ifdef ICF_IPSEC_ENABLED
			/* initialize the number of failed transactions to zero */
			p_ssa_data->num_successive_trans_fail = ICF_NULL;

            /* initialise the data structures for IPsec */
            for (count =0; count <ICF_MAX_SA_CONNECTIONS; count++)
            {
                ssa_pdb.p_ssa_glb_data->sa_conn_db_node[count].fd = 
                                ICF_INVALID_FD;
                ssa_pdb.p_ssa_glb_data->sa_conn_db_node[count].prot_port_num =
                                0;
                
            }
#endif /* ICF_IPSEC_ENABLED */

			
			*p_p_ssa_data = (icf_void_t*)p_ssa_data;
#ifdef ICF_SESSION_TIMER
			sdf_fn_uaSetGlbMinSe(ICF_CFG_DEF_MIN_SESSION_TIMER_INTERVAL,&sdf_error);
#endif
#ifdef SDF_TLS
		sdf_memfree(Sdf_mc_callHandlerMemId, \
                (Sdf_ty_pvoid *)&(init_params.pEntropySource),&sdf_error);
#endif
		}
    }
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_deinit
 *
 * DESCRIPTION:     De-inits and de-allocates SSA module data
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_void_t icf_ssa_deinit(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            *p_ssa_data,
        OUT     icf_error_t           *p_ecode)
{
    #ifdef ICF_SECURE_SUPPORT
    icf_uint16_t i;
    #endif

    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_glb_data_st  *p_ssa_glb_data = ICF_NULL;
    icf_dbm_get_module_glb_data(p_glb_pdb,ICF_INT_MODULE_SSA,
            (icf_void_t **)&p_ssa_glb_data,p_ecode);
	if(ICF_NULL == p_ssa_data)
	{
		return;
	}
    else if (Sdf_co_fail == 
            sdf_ivk_uaFreeInitData(
                ((icf_ssa_glb_data_st*)p_ssa_data)
                ->p_default_profile))
    {
        /*Raise a Minor Error here */
    }
    /*
     * De-Init the UA Toolkit
     */
    sdf_ivk_uaCloseToolkit();
    if (ICF_NULL != p_ssa_glb_data)
    {
#ifdef ICF_SECURE_SUPPORT
        for (i = 0; i < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; i++)
        {
            if (ICF_NULL != p_ssa_glb_data->p_ssa_tcpm_conn_db[i].p_peer_name)
            {
                ICF_MEMFREE( p_glb_pdb,p_ssa_glb_data->p_ssa_tcpm_conn_db[i].p_peer_name,
                        ICF_MEM_COMMON, p_ecode, ret_val);
            }
        }
#endif
        ICF_MEMFREE( p_glb_pdb,p_ssa_glb_data->p_ssa_tcpm_conn_db,
                ICF_MEM_COMMON, p_ecode, ret_val);

    }
    ICF_STATIC_MEMFREE(p_glb_pdb, p_ssa_data, p_ecode, ret_val)
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_handle_tcp_event_frm_mrm
 *
 * DESCRIPTION:     
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_ssa_handle_tcp_event_frm_mrm(
        INOUT icf_internal_msg_st        *p_internal_msg)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_msg_st       *p_recv_msg = ICF_NULL;
    icf_nw_interface_info_st *p_nw_intf_info = ICF_NULL;
    icf_ssa_pdb_st           ssa_pdb ;
    icf_ssa_glb_data_st      *p_ssa_glb_data;
    icf_ssa_tcpm_conn_data_st  *p_conn  =  ICF_NULL;

    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_SSA_GET_RECV_MSG_FRM_INTRNL_MSG(p_recv_msg,p_internal_msg)
    ICF_ASSERT(p_internal_msg->msg_hdr.p_glb_pdb, p_recv_msg)
    p_nw_intf_info = (icf_nw_interface_info_st*)(p_recv_msg->payload);
    /*
     * Init SSA PDb
     */
    icf_ssa_init_pdb(&ssa_pdb, p_internal_msg->msg_hdr.p_glb_pdb);
    ssa_pdb.p_internal_msg      = p_internal_msg;    
    ssa_pdb.p_ecode             = &(p_internal_msg->msg_hdr.ecode);
    /*Dest module id set to invalid, the callback processing should
     * fill this up correctly with the dest module id ( REGM or CM)*/
    ssa_pdb.dest_module_id  = ICF_INT_MODULE_INVALID;
    /*
     * Now we init the void pointer which will be used to store the
     * list of memory chunks that will be allocated in callbacks and will
     * be freed on return path (after invocation of destination module-CM,
     * CFG etc.)
     */
    
    /* get the ssa global data pointer from DBM */
    icf_dbm_get_module_glb_data(ssa_pdb.p_glb_pdb,ICF_INT_MODULE_SSA,
            (icf_void_t **)&p_ssa_glb_data,ssa_pdb.p_ecode);
    ssa_pdb.p_ssa_glb_data = p_ssa_glb_data;
    /* get the config data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                ssa_pdb.p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(ssa_pdb.p_glb_cfg_data),
                ssa_pdb.p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
            return ret_val;
    }
    else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                ssa_pdb.p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_REGM,
                (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
                ssa_pdb.p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        return ret_val;
    }

    /*
     * As message is coming from MRM, there should not be anything
     * Get the FD from the internal message
     * Get the TCPM block if the API is not ICF_NW_API_OPEN_CONN_IND
     * else allocate a new TCPM block and set the state to INVALID in it
     * and populate the remote address in it
     * Now trigger the SM
     */
    if ( ICF_NW_API_OPEN_CONN_IND == 
            p_recv_msg->hdr.api_id)
    {
        {
            p_conn = icf_ssa_add_conn_entry(ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db);
                    
            if (ICF_NULL == p_conn)
            {
                ICF_PRINT(((icf_uint8_t *)"Addition of new connection failed\n"));
                ret_val = ICF_FAILURE;
            }
            else
            {
                ICF_PRINT(((icf_uint8_t *)"Addition of new connection succeded\n"));
                icf_port_strcpy(p_conn->peer_ip, 
                        p_nw_intf_info->remote_ip_addr);
                p_conn->peer_port_num = p_nw_intf_info->remote_port_num;

                p_conn->conn_fd = (icf_int16_t)p_nw_intf_info->receiver_fd;
                p_conn->prot_conn_bitmask = 0;
                p_conn->tcpm_sm_state = ICF_SSA_TCPM_IDLE;
                ssa_pdb.p_conn_table_entry = p_conn;
                /* Need to discuss */
#ifdef ICF_SECURE_SUPPORT
                if (p_nw_intf_info->bitmap & ICF_INTF_INFO_PROTOCOL_TLS)
                {
                    if (0 != ICF_PURGE_TIMER_DURATION)
                        p_conn->is_persistent = ICF_FALSE;
                    p_conn->prot_conn_bitmask |=  ICF_TRANSPORT_TLS;
                }
                else
#endif
                {
                    if (0 != ICF_PURGE_TIMER_DURATION)
                        p_conn->is_persistent = ICF_TRUE;
                    p_conn->prot_conn_bitmask |=  ICF_TRANSPORT_TCP;
                }
            }
        }
    }
    else
    {
        /*Search for the conn db index based on the fd value for the conn
         * fill it in ssa pdb and then invoke the SM*/
        ssa_pdb.p_conn_table_entry = 
            icf_ssa_get_conn_db((icf_uint16_t)p_nw_intf_info->receiver_fd, 
                    ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db );
        if (ICF_NULL == ssa_pdb.p_conn_table_entry)
        {
            ICF_PRINT(((icf_uint8_t *)"No connection entry found. Can be malicious packet\n"));
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = ICF_SUCCESS;
        }
    }
    if ( ICF_FAILURE != ret_val)
    {
        icf_uint8_t  tcp_event;
        switch (p_recv_msg->hdr.api_id){
            case ICF_NW_API_OPEN_CONN_IND:
                tcp_event = ICF_SSA_TCPM_OPEN_CONN_IND;
                break;
            case ICF_NW_API_CONNECT_SUCCESS:
                tcp_event = ICF_SSA_TCPM_SUCC_OPEN_CONN_RESP;
                break;
            case ICF_NW_API_CONNECT_FAILURE:
                tcp_event = ICF_SSA_TCPM_FAIL_OPEN_CONN_RESP;
                break;
            case ICF_NW_API_CLOSE_CONN_IND:
                tcp_event = ICF_SSA_TCPM_CLOSE_CONN_IND;
                break;
            default:tcp_event = ICF_SSA_TCPM_MAX_EVENTS - 1;
                    break;
        }

        ICF_SSA_IVK_TCPSM((&ssa_pdb), tcp_event, ret_val)
        if ( ICF_FAILURE == ret_val)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInTCPSM"));
        }
    }
    ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_nw_msg
 *
 * DESCRIPTION:     This is the processor for SIP/SIGCOMP messages from n/w
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_nw_msg(
        INOUT icf_internal_msg_st    *p_internal_msg)
{
    icf_ssa_pdb_st           ssa_pdb ;
    /*
     * Local instance of internal msg union, used by SSA to 
     * pass parameter structure to the destination module
     */
    icf_ssa_intrnl_intf_ut   internal_msg_payload;
    icf_ssa_glb_data_st      *p_ssa_glb_data = ICF_NULL;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    Sdf_ty_retVal             mesg_length_status = Sdf_co_fail;
    Sdf_ty_messageValidity   sdf_retval;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             dummy_ret_val = ICF_SUCCESS;
    icf_error_t              *p_ecode = ICF_NULL;
    icf_uint32_t             pdu_len = 0, count = 0;
    Sdf_ty_u32bit               check_option = 0;
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;

    /*External received message, this is made by Eshell */
    icf_msg_st               *p_recvd_msg = ICF_NULL;

    /*External message API header, filled by Eshell */
    icf_api_header_st        *p_ext_api_hdr = ICF_NULL;

    /*
     * Payload for external message, in case of SIP message, this payload
     * contains an instance of interface_info structure
     * which contains the pointer to the received PDU
     */
    icf_uint8_t              *p_ext_api_body = ICF_NULL;

    icf_nw_interface_info_st *p_nw_int_info = ICF_NULL;
    icf_uint16_t             processed_bytes = 0; 

    icf_int8_t               *p_next_msg = ICF_NULL,
                                *p_recvd_pdu = ICF_NULL,
                                *p_current_sip_pdu = ICF_NULL;

    icf_boolean_t            more_messages = ICF_FALSE, 
    remove_bytes_from_conn = ICF_TRUE;

    /*
     * Variables for usage of Stack/UATK APIs
     */
    Sdf_st_error            sdf_error = {0,0,0,"\0"};
    Sdf_st_eventContext     *p_event_ctxt = Sdf_co_null;
    SipTranspAddr           *p_transp_addr = Sdf_co_null;
    Sdf_ty_protocol         protocol;
#ifdef ICF_SIGCOMP_SUPPORT
    icf_uint8_t          sigcTransportType = ICF_SIGSA_TRANSPORT_TYPE_UDP;
#endif
    Sdf_st_callObject       *p_call_obj = Sdf_co_null;
/*Following variable is used to store ssa_ctxt from conn_db which will
 *be used in case of decode failure*/
#if 0
    icf_ssa_ctxt_st         *p_tcp_ssa_ctxt = ICF_NULL;
#endif

    /*CERT fix, if BadHdr list is not Null*/
    
   /* SPR 15181 changes */

    ICF_SSA_GET_GLB_PDB_FRM_INTRNL_MSG(p_glb_pdb,p_internal_msg)

        ICF_FUNCTION_ENTER(p_glb_pdb)

        /*Get common parameters */
        ICF_SSA_GET_RECV_MSG_FRM_INTRNL_MSG(p_recvd_msg,p_internal_msg)
        ICF_SSA_GET_EXT_API_HDR_FRM_INTRNL_MSG(p_ext_api_hdr,p_internal_msg)
        ICF_SSA_GET_EXT_PAYLOAD_FRM_INTRNL_MSG(p_ext_api_body,p_internal_msg)

        p_nw_int_info = (icf_nw_interface_info_st*)(p_recvd_msg->payload);
        p_recvd_pdu = (icf_int8_t *)p_nw_int_info->p_sip_msg;
    p_current_sip_pdu = p_recvd_pdu;

    /*
     * Populate SSA PDB
     */
	icf_ssa_init_pdb(&ssa_pdb,p_glb_pdb);
    ssa_pdb.p_internal_msg      = p_internal_msg;
    ssa_pdb.p_ecode             = &(p_internal_msg->msg_hdr.ecode);
    ssa_pdb.p_conn_table_entry   = ICF_NULL;
    
    /*
     * Now we init the void pointer which will be used to store the
     * list of memory chunks that will be allocated in callbacks and will
     * be freed on return path (after invocation of destination module-CM,
     * CFG etc.)
     */
    ssa_pdb.alloc_index = 0;
    for ( count = 0; count < ICF_SSA_MAX_MMRY_CHUNK_LST;
            count++)
    {
        ssa_pdb.allocated_memory[count].p_mem_chunk =
            ICF_NULL;
    }
    /* get the ssa global data pointer from DBM */
    icf_dbm_get_module_glb_data(p_glb_pdb,ICF_INT_MODULE_SSA,
            (icf_void_t **)&p_ssa_glb_data,ssa_pdb.p_ecode);
    ssa_pdb.p_ssa_glb_data = p_ssa_glb_data;
    /* get the config data pointer from DBM */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                ssa_pdb.p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(ssa_pdb.p_glb_cfg_data),
                ssa_pdb.p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        return ret_val;
    }
    else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                ssa_pdb.p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_REGM,
                (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
                ssa_pdb.p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        return ret_val;
    }
#ifdef ICF_IPSEC_ENABLED
	/* check if ipsec is enabled and that the message is received
	 * on a valid SA. If the message is received on a valid SA, 
	 * the SA pointer is fetched and saved temporarily in the ssa_pdb 
	 * structure. Only if the message is received on a SA, will the 
	 * message be further decoded, else the message is silently 
	 * discarded in ICF before reaching the stack.
	 */
	/* some of our assumptions here are:
	 * 1. Response sent/received on the same SA as the Request
	 * 2. all messages received on a valid SA
	 * Limitation:
	 * 1. Don't check if an error response is received unprotected
	 */
	if ((ssa_pdb.p_glb_cfg_data->ipsec_enabled) &&
		(ICF_FAILURE == icf_ssa_fetch_sa_context(&ssa_pdb, p_nw_int_info)))

	{
		ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        return ret_val;
	}
#endif

    /*
     * Following code will be in loop for processing of > 1 message in
     * a pdu
     */
     
    /* If the protocol is TCP, then initialise the field
     * ssa_pdb.p_conn_table_entry every time when it receives 
     * a new message from network.
     */ 
    do
    {

     if ( ICF_INTF_INFO_PROTOCOL_UDP & p_nw_int_info->bitmap)
      {
        protocol = Sdf_en_protoUdp;
      }
     else if (ICF_INTF_INFO_PROTOCOL_TCP & p_nw_int_info->bitmap)
      {
        protocol = Sdf_en_protoTcp;
#ifdef ICF_SIGCOMP_SUPPORT
        sigcTransportType = ICF_SIGSA_TRANSPORT_TYPE_TCP;
#endif
        ssa_pdb.p_conn_table_entry = icf_ssa_get_conn_db((icf_uint16_t)p_nw_int_info->receiver_fd,
                ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db);

        if ( ICF_NULL == ssa_pdb.p_conn_table_entry)
        {
            ret_val = ICF_FAILURE;
            if (ICF_NULL != p_call_obj)
            {
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!FD not found in ConnDB"));
            ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
        }/* SPR 15181 changes */
        ICF_SSA_RESET_CONN_TIMER_COUNT(ssa_pdb.p_conn_table_entry);
       }
#ifdef ICF_SECURE_SUPPORT
    else if (ICF_INTF_INFO_PROTOCOL_TLS & p_nw_int_info->bitmap)
        
    {
        protocol = Sdf_en_protoTls;
#ifdef ICF_SIGCOMP_SUPPORT
        sigcTransportType = ICF_SIGSA_TRANSPORT_TYPE_TCP;
#endif
        ssa_pdb.p_conn_table_entry = icf_ssa_get_conn_db((icf_uint16_t)p_nw_int_info->receiver_fd,
                ssa_pdb.p_ssa_glb_data-> p_ssa_tcpm_conn_db);

        if ( ICF_NULL == ssa_pdb.p_conn_table_entry)
        {
            ret_val = ICF_FAILURE;
            if (ICF_NULL != p_call_obj)
            {
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
            ICF_PRINT(("\n[SSA]:Error!!FD not found in ConnDB"));
            ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
        }
        ICF_SSA_RESET_CONN_TIMER_COUNT(ssa_pdb.p_conn_table_entry);

    }
#endif
    else
    {
        /*
         * Raise Error of major category
         */
        ret_val  = ICF_FAILURE;
        if (ICF_NULL != p_call_obj)
        {
            sdf_ivk_uaFreeCallObject(&p_call_obj);
        }
        ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
    }


    if (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT
	|| Sdf_en_protoTls == protocol)
#else
	)
#endif
    {
        /* Trigger the TCPM state machine to reset the purge timer
         * state if any*/
        ICF_SSA_IVK_TCPSM((&ssa_pdb), \
                ICF_SSA_TCPM_DATA_IND, ret_val)
    }

    
     /*
     * As message is coming from MRM, there should not be anything
     * in the payload part of the internal message, so we are over-
     * writing the pointer here.
     */
        p_internal_msg->p_msg_data  = (icf_uint8_t*)(&internal_msg_payload);
        p_internal_msg->msg_hdr.payload_length  = 
            ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut);

        p_ecode = &(p_internal_msg->msg_hdr.ecode);
        icf_port_memset(&internal_msg_payload, 0, 
                        sizeof(icf_ssa_intrnl_intf_ut));


        ssa_pdb.common_bitmask      = ICF_NULL;
        /*Dest module id set to invalid, the callback processing should
         * fill this up correctly with the dest module id ( CFG or CM)*/
        ssa_pdb.dest_module_id  = ICF_INT_MODULE_INVALID;

        /*
         * Now we will allocate memory for transport address and pass the 
         * address of the IP address variable received from Eshell in event
         * context further on
         */
        ICF_MEMGET(p_glb_pdb,
                ICF_PORT_SIZEOF(SipTranspAddr), ICF_MEM_COMMON,\
                p_transp_addr,\
                ICF_RET_ON_FAILURE,\
                p_ecode, ret_val)

        /*
         * Here we allocate a SSA ctxt instance and populate it
         * down below with the bare minimum info-p_ssa_pdb, p_glb_pdb
         */
        ICF_MEMGET(p_glb_pdb,\
                sizeof(icf_ssa_ctxt_st), ICF_MEM_COMMON, \
                p_ssa_ctxt, ICF_DONOT_RET_ON_FAILURE, \
                p_ecode, ret_val)

        if ( ICF_NULL == p_ssa_ctxt)
        {
            ICF_MEMFREE(p_glb_pdb, \
                    p_transp_addr, ICF_MEM_COMMON,\
                    p_ecode, ret_val)
             ret_val = ICF_FAILURE;
        }
        else
        {
            /* Initialize the elements of new ssa ctxt */
			icf_ssa_init_ssa_context(&ssa_pdb,p_ssa_ctxt);            
			
			p_ssa_ctxt->p_ssa_pdb = 
                    (struct icf_ssa_pdb_st*)(&ssa_pdb);
            ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_glb_pdb = 
                                                                p_glb_pdb;
			ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
#ifdef ICF_NAT_RPORT_SUPPORT
            /* Store the received Ip address in the SSA Context
             */
            icf_port_memcpy(p_ssa_ctxt->destination_ip,\
                    (icf_uint8_t*)p_nw_int_info->remote_ip_addr,\
                    sizeof(p_ssa_ctxt->destination_ip));
            p_ssa_ctxt->destination_port = p_nw_int_info->remote_port_num;
#endif
            /*
             * Allocate and Init a temporary call object
             */
            if ( ICF_FAILURE == icf_ssa_get_new_call_obj(&ssa_pdb,
                        &p_call_obj))
            {
                /*
                 * TO DO : trace to be added here. error of major cateogry
                 * to be set
                 */
                ICF_MEMFREE(p_glb_pdb, p_transp_addr, \
                        ICF_MEM_COMMON, p_ecode, ret_val)

                ICF_MEMFREE(p_glb_pdb, p_ssa_ctxt,\
                            ICF_MEM_COMMON, p_ecode, ret_val)

                ret_val  = ICF_FAILURE;
            }
            else if ( Sdf_co_fail == sdf_ivk_uaInitEventContext(
                        &p_event_ctxt, &sdf_error))
            {
                /*
                 * TO DO : trace to be added here. error of major cateogry
                 * to be set
                 */
                ICF_MEMFREE(p_glb_pdb, p_transp_addr, \
                        ICF_MEM_COMMON, p_ecode, ret_val);

                ICF_MEMFREE(p_glb_pdb, p_ssa_ctxt,\
                            ICF_MEM_COMMON, p_ecode, ret_val);

                sdf_ivk_uaFreeCallObject(&p_call_obj);
                ret_val  = ICF_FAILURE;
            }
            else /*Transport Addr, Evt Ctxt and CallObj allocation
                   successful, proceeding..*/
            {
                SipOptions                  sip_options ;
                Sdf_ty_matchMessageType     msg_type;
                if(Sdf_en_protoTcp == protocol)
                {
                    p_ssa_ctxt->transport = ICF_TRANSPORT_TYPE_TCP;
                }
#ifdef ICF_SECURE_SUPPORT
                else if(Sdf_en_protoTls == protocol)
                {
                    p_ssa_ctxt->transport = ICF_TRANSPORT_TYPE_TLS;
                }
#endif

                p_ssa_ctxt->bitmask = ICF_NULL;
                p_ssa_ctxt->callobj_map_index = ICF_SSA_MAX_OPTIONS_CALL_OBJ;
                p_ssa_ctxt->p_call_ctxt = ICF_NULL;
                p_ssa_ctxt->failure_bitmask = ICF_NULL;
				p_ssa_ctxt->trans_mode_reverted = ICF_FALSE;
                p_ssa_ctxt->p_ssa_pdb = 
                    (struct icf_ssa_pdb_st*)(&ssa_pdb);
                ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))
                    ->p_glb_pdb = p_glb_pdb;

                p_event_ctxt->dProtocol = protocol;
                p_event_ctxt->pTranspAddr = p_transp_addr;
                if (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT
		|| Sdf_en_protoTls == protocol)
#else
		)
#endif
                {
                    p_event_ctxt->pTranspAddr->pHost =
               (icf_int8_t *)ssa_pdb.p_conn_table_entry->peer_ip;
                    p_event_ctxt->pTranspAddr->dPort =
                        ssa_pdb.p_conn_table_entry->peer_port_num;
                }
                else
                {
                    p_event_ctxt->pTranspAddr->pHost =
                        (icf_int8_t*)(p_nw_int_info->remote_ip_addr);
                    p_event_ctxt->pTranspAddr->dPort =
                        p_nw_int_info->remote_port_num;
                }
                p_event_ctxt->pTranspAddr->dSockFd =
                    (icf_uint16_t )p_nw_int_info->receiver_fd;
                p_event_ctxt->dNextMessageLength = 0;
                p_event_ctxt->pData = (SIP_Pvoid)(p_ssa_ctxt);

                /*
                 * Following stops SIP stack from stopping txn timer 
                 * on it's own(HSSUA)
                 */
                sip_options.dOption = SIP_OPT_NOTIMER | SIP_OPT_BADMESSAGE;

                /*
                 * Call sdf_ivk_uaDecodeMessage. This will parse the buffer
                 * and fill up the pMsg SipMessage structure of the Call Object
                 */
				/* fix for TCP UT */
                pdu_len=icf_port_strlen((icf_uint8_t *)p_current_sip_pdu) ;	

                remove_bytes_from_conn = ICF_TRUE;
				p_call_obj->pAppData->pData = (SIP_Pvoid)(p_ssa_ctxt);
				p_ssa_ctxt->p_call_obj = p_call_obj;
                
                if (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT
		        || Sdf_en_protoTls == protocol)
#else
		        )
#endif
                {
                    /* Release 7.0 */
#if 0
                    p_tcp_ssa_ctxt = p_ssa_glb_data->ssa_tcpm_conn_db
                        [ssa_pdb.ssa_conn_db_index].p_ssa_ctxt;
                    p_ssa_glb_data->ssa_tcpm_conn_db
                        [ssa_pdb.ssa_conn_db_index].p_ssa_ctxt
                        = p_ssa_ctxt;
                    p_ssa_ctxt->ssa_conn_db_index =
                        ssa_pdb.ssa_conn_db_index;
#endif
#ifdef ICF_SECURE_SUPPORT		    
                    if (Sdf_en_protoTls == protocol)
                    {
                        p_ssa_ctxt->bitmask |= ICF_SSA_TLS_TRANSPORT;
                    }
                    else
                    {
#endif			    
                        p_ssa_ctxt->bitmask |= ICF_SSA_TCP_TRANSPORT;
#ifdef ICF_SECURE_SUPPORT		    
                    }
#endif
                }

                 /*Call Decompress with ssa pdb havin return value for
                    msg_type, error_code and a bitmask indicating whether 
                    message has been decoded. decdec call object will be in
                    ssa context. decompressed pdu will be returned as p to p.
                 */
				ssa_pdb.processed_bytes = 0;
#ifdef ICF_SIGCOMP_SUPPORT
                if(ICF_TRUE == ssa_pdb.p_glb_cfg_data->sigcomp_required)
                {
                    ret_val = icf_sigsa_ProcessNetworkMessage(
                        p_nw_int_info->p_sip_msg,
                        (icf_uint16_t)p_nw_int_info->pdu_len,
                        sigcTransportType,
                        &processed_bytes,
                        &p_next_msg,p_event_ctxt,
                        ssa_pdb.p_ecode);
                    msg_type = ssa_pdb.msg_type;
                    if(0!=ssa_pdb.processed_bytes)
                    {
                        /* SIP MESSAGE HAS COME  ON TCP */
                        processed_bytes = ssa_pdb.processed_bytes;
                    }
                }
                else
                {
#ifdef VOIP_SYSLOG /*Jason , syslog*/
                    icf_print_pdu(p_glb_pdb, p_current_sip_pdu,p_nw_int_info->remote_ip_addr, 
                        p_nw_int_info->remote_port_num, p_nw_int_info->pdu_len,0);
#else                
                    icf_print_pdu(p_glb_pdb, p_current_sip_pdu,p_nw_int_info->remote_ip_addr, 
                        p_nw_int_info->remote_port_num, p_nw_int_info->pdu_len);
#endif               
                    msg_type = sdf_ivk_uaDecodeMessage(
                        p_call_obj, p_current_sip_pdu, \
                        &sip_options, pdu_len, &p_next_msg, p_event_ctxt, \
                        &sdf_error);
                    if ((ICF_NULL != p_next_msg) &&
                    (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT		
		            || Sdf_en_protoTls == protocol))
#else
		            ))
#endif
                    {
                    /* Another message is there to be processed.
                    * So we invoke ES/port interface to read the processed
                    * bytes from the connection and then we set the pointer
                        * for current_sip_pdu to the p_next_msg*/
                        processed_bytes = 
                            icf_port_strlen(p_current_sip_pdu) - 
                            icf_port_strlen(p_next_msg);
                    }
                }


#else
#ifdef ICF_TRACE_ENABLE
		ssa_pdb.p_buf = (icf_uint8_t *)p_current_sip_pdu;
        ssa_pdb.len = p_nw_int_info->pdu_len;
#endif
/*Jason , syslog*/
#if 0 //eric.chung for support voipsyslog , mark it.
				icf_print_pdu(p_glb_pdb, (icf_uint8_t *)p_current_sip_pdu,p_nw_int_info->remote_ip_addr, 
                        p_nw_int_info->remote_port_num, p_nw_int_info->pdu_len);
#endif              
                /*Check for the Message Length*/ 
                /*Fix for TCP : SPR 14954*/
                
                 ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Invoking sdf_ivk_uaDecodeMessage with "\
							"p_nw_int_info->pdu_len = %d\n", \
							p_nw_int_info->pdu_len));
                check_option = 0;
                sdf_retval = 0;

            mesg_length_status = 
                sdf_ivk_uaIPTKCheckMessageLength(p_current_sip_pdu, ICF_SSA_MAX_HEADER_VALUE_LEN,
                        check_option,(Sdf_ty_u32bit*) &sdf_retval, &sdf_error);
/* Message body need not to be checked as message body can contain XML data
 * too */
            if((sdf_retval & Sdf_en_reqresplinelong) == Sdf_en_reqresplinelong)
            {
                /* send 414 here */
                p_ssa_ctxt->bitmask_1 |= ICF_REQ_URI_TOO_LONG;
            }
            if (mesg_length_status == Sdf_co_fail)
            {
                ICF_PRINT(((icf_uint8_t *)"CheckIPTKMessagelength returns error\n"));
            }
            else if ((sdf_retval & Sdf_en_mandatoryhdrlong) == Sdf_en_mandatoryhdrlong)
            {
                ICF_PRINT(((icf_uint8_t *)"Mandatory header is very long\n"));
            }
            else if ((sdf_retval & Sdf_en_optionalhdrlong) == Sdf_en_optionalhdrlong)
            {
                ICF_PRINT(((icf_uint8_t *)"Optional header is very long\n"));
            }
            else
            {              
               
				msg_type = sdf_ivk_uaDecodeMessage(
                        p_call_obj, p_current_sip_pdu, \
                        &sip_options, pdu_len, &p_next_msg, p_event_ctxt, \
                        &sdf_error);
                    
                
                if ((ICF_NULL != p_next_msg) &&
                    (Sdf_en_protoTcp == protocol
#ifdef ICF_SECURE_SUPPORT
                    || Sdf_en_protoTls == p_event_ctxt->dProtocol ))
#else
					))
#endif	
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: More messages to be processed \n"));
                    /* Another message is there to be processed.
                     * So we invoke ES/port interface to read the processed
                     * bytes from the connection and then we set the pointer
                     * for current_sip_pdu to the p_next_msg*/
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: More messages to be processed \n"));
                        processed_bytes = 
                            (icf_uint16_t )(icf_port_strlen((icf_uint8_t *)p_current_sip_pdu) - 
                            icf_port_strlen((icf_uint8_t *)p_next_msg));
                }
				else
				{
					 /*Fix for TCP : SPR 14954*/
						ICF_PRINT(((icf_uint8_t *)"\n[SSA]: No more msg to be processed \n")); 
						 processed_bytes = p_nw_int_info->pdu_len;
				}
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]: processed_bytes = %d\n", processed_bytes));
					ssa_pdb.processed_bytes = processed_bytes;


#endif
                if(ICF_FAILURE != ret_val)
                {
                    if(Sdf_en_versionNotSupported == sdf_error.errCode)
                    {
                        msg_type = Sdf_en_success;
                    }

                    switch (msg_type)
                    {
                        case Sdf_en_remoteRetransmission:
                            {
                            /*
                            * Add for informational trace here
                                */
                                ICF_PRINT(((icf_uint8_t *)"[SSA] SIP MESSAGE REMOTE RETRANSMISSION\n"));

                                break;
                            }
                        case Sdf_en_fail:
                            {
                                ICF_PRINT(((icf_uint8_t *)"[SSA] SIP MESSAGE DECODE FAILED\n"));
                                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:  sdf_error.stkErrCode = "\
           					   "%d",  sdf_error.stkErrCode));
                                /* Fix for SPR 17518: Handling of Error Code 
                                 * Sdf_en_sipIncomplete
                                 */
                                 if (((Sdf_en_maybeIncomplete == sdf_error.stkErrCode) ||
                                    (Sdf_en_incomplete == sdf_error.stkErrCode)||
                                    (Sdf_en_sipIncomplete == sdf_error.stkErrCode)) &&
                                    (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT				
				            	|| Sdf_en_protoTls == protocol))
#else
				            	))
#endif
                                {
                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Message incomplete \n"));
                                /* Incomplete msg received on TCP conn.
                                * Assumption:There is no message in the pdu
                                * following this message.
                                * Since this is an incomplete message, we will
                                * not process it. Release the memory and return
                                    */
                                    /*Commenting the following memfree for SPR 14223*/   
                                    /*ICF_MEMFREE(p_glb_pdb, p_ssa_ctxt,\
                                        ICF_MEM_COMMON, p_ecode, ret_val)*/
									/* Fix for CSR 1-5549580 */
                                    /* Check the length of this msg. If it has
                                     * reached max supported pdu size, drop it.
                                     */
                                    if (processed_bytes < ICF_MAX_TCP_MSG_LNGTH)
                                    {
                                        remove_bytes_from_conn = ICF_FALSE;
                                    }
                                    else
                                    {
                                        /* At the max, processed_bytes will be
                                         * equal to the ICF_MAX_TCP_MSG_LNGTH
                                         */
										/*Fix done for SPR-18722 : CSR-1-6068965*/
                                        ICF_PRINT(((icf_uint8_t *)"\nUnsupported size pdu\n"));
                                        remove_bytes_from_conn = ICF_TRUE;
                                    }
                                }
                               else if (/*(*/Sdf_en_headerManipulationError ==
                                        sdf_error.errCode)/* ||
                                        (Sdf_en_basicHeaderError == sdf_error.errCode) )*/
                                {
                                    /*CSR 7945729 SPR 20251 Memory in use for txn resolved here*/
                                    if (ICF_NULL != p_call_obj->pUasTransaction)
                                    {    
                                        icf_ssa_free_sip_txn_data_on_failure(
                                                p_call_obj->pUasTransaction->pTxnKey);
                                    }
                                    ICF_PRINT(((icf_uint8_t *)"Header manipulation error\n"));
                                }
                                else
                                {
                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Msg decode fail \n"));
                                    ret_val = icf_ssa_handle_decode_failure(
                                        &ssa_pdb,
                                        p_event_ctxt, 
                                        p_call_obj, 
                                        &sdf_error, 
                                        p_ecode);
                                }
                            }
                            break;
                        case Sdf_en_strayMessage:
                            {
                                ICF_PRINT(((icf_uint8_t *)"[SSA] SIP MESSAGE IS A STRAY MESSAGE\n"));
                                /*Add Informational trace here for stray msg*/
                                if (Sdf_co_null != 
                                        p_call_obj->pUasTransaction->pSipMsg)
                                {
                                    ret_val = icf_ssa_handle_stray_msg(
                                            &ssa_pdb,
                                            p_event_ctxt, 
                                            p_call_obj, 
                                            &sdf_error, 
                                            p_ecode);
                                }
#if 0
                                if (Sdf_en_protoTcp == protocol
#ifdef ICF_SECURE_SUPPORT
                                        || Sdf_en_protoTls == protocol)
#else
                                    )
#endif
                                    {
                                        p_ssa_glb_data->ssa_tcpm_conn_db
                                            [ssa_pdb.ssa_conn_db_index].p_ssa_ctxt =
                                            p_tcp_ssa_ctxt;
                                    }
#endif
                            }
			                break;
                        default:/*Assumed this to be successful decode*/
                            {
                                icf_boolean_t         send_fail_flag = ICF_FALSE;
                                SipMessage            *pMessage = ICF_NULL;
                                SipHeader              dHeader;
                                SipError            error;
                                Sdf_ty_s8bit        *pMediaType = ICF_NULL;
                                if (Sdf_en_msgbodylinelong == 
                                        (sdf_retval & Sdf_en_msgbodylinelong))
                                {
                                    if (p_call_obj->pUacTransaction->pSipMsg != Sdf_co_null)
                                        pMessage = p_call_obj->pUacTransaction->pSipMsg;
                                    else
                                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                                    if (sip_getHeader(pMessage, SipHdrTypeContentType, \
                                                &dHeader, &error) == SipFail)
                                    {
                                        ICF_PRINT(((icf_uint8_t *)"\nFailed to get Content-Type Header"));
                                    }
                                    if (sip_getMediaTypeFromContentTypeHdr(&dHeader, &pMediaType,\
                                                &error) == SipFail)
                                    {
                                        ICF_PRINT(((icf_uint8_t *)"\nFailed to get media-type from "
                                                    "Content-Type Header"));
                                        sip_freeSipHeader(&dHeader);
                                        return ICF_FAILURE;
                                    }
                                    if (Sdf_mc_strcasecmp(pMediaType, "application/sdp") == 0)
                                    {
                                        ICF_PRINT(((icf_uint8_t *)"\nMedia Type: application/reginfo+xml"));
                                        send_fail_flag = ICF_TRUE;
                                    }
                                    sip_freeSipHeader(&dHeader);
                                }
                                if (ICF_TRUE == send_fail_flag)
                                {
                                    ret_val = icf_ssa_handle_decode_failure(
                                            &ssa_pdb,
                                            p_event_ctxt, 
                                            p_call_obj, 
                                            &sdf_error, 
                                            p_ecode);
                                }
                                else
                                {
                                    ICF_PRINT(((icf_uint8_t *)"[SSA] SIP MESSAGE SUCCESSFULLY DECODED\n"));
                                    /* SPR 16019: remove the read bytes from 
                                     * TCP socket before attempting to close 
                                     * the socket.
                                     */
                                    if ((ICF_TRUE == remove_bytes_from_conn) &&
                                            (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT			
                                             || Sdf_en_protoTls == protocol))
#else
                                        ))
#endif
                                        {
                                            if (Sdf_en_protoTcp == protocol)
                                            {
                                                /* It may be possible that the 
                                                 * connection on which we are 
                                                 * about to read now may have 
                                                 * been already closed. Further,
                                                 * in such a case, p_conn_db_node->
                                                 * conn_fd will be reset to invalid but 
                                                 * p_nw_int_info->fd_id may not be reset to
                                                 * invalid.
                                                 */
                                                if (ICF_NULL == ssa_pdb.p_conn_table_entry)
                                                {
                                                    ret_val = ICF_FAILURE;
                                                    if (ICF_NULL != p_call_obj)
                                                    {
                                                        sdf_ivk_uaFreeCallObject(&p_call_obj);
                                                    }

                                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!FD not found in ConnDB"));
                                                    ICF_FUNCTION_EXIT(p_glb_pdb)
                                                        return ret_val;
                                                }
                                                else if (ICF_FAILURE == icf_port_tcp_recv(
                                                            (icf_uint16_t)ssa_pdb.p_conn_table_entry->conn_fd,
                                                            p_recvd_pdu, processed_bytes))
                                                {
                                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in removal of bytes from conn"));
                                                }
                                            }
#ifdef ICF_SECURE_SUPPORT		
                                            else
                                            {
                                                icf_void_t      *p_secure_data = ICF_NULL;
                                                /* 
                                                 * Now extract the SSL Data corrresponding to this
                                                 * socket fd from the map 
                                                 */
                                                p_secure_data = icf_es_fetch_secure_data_from_map(
                                                        ssa_pdb.p_glb_pdb->p_glb_data->p_glb_port_info,
                                                        p_nw_int_info->receiver_fd,
                                                        p_ecode) ;
                                                if (ICF_NULL == p_secure_data)
                                                {
                                                    ret_val = ICF_FAILURE;
                                                }
                                                else if (ICF_FAILURE == icf_port_secure_read(
                                                            p_secure_data,(icf_uint16_t)p_nw_int_info->receiver_fd,
                                                            p_recvd_pdu, pdu_len))
                                                {
                                                    ret_val = ICF_FAILURE;
                                                }
                                            }
#endif		
                                             remove_bytes_from_conn = ICF_FALSE;
                                        }


                                    /* End of removing bytes
                                     */
                                    ret_val = icf_ssa_handle_successful_decode(
                                            &ssa_pdb,
                                            p_event_ctxt, 
                                            p_call_obj, 
                                            &sdf_error, 
                                            p_ecode);
#ifdef VOIP_SYSLOG
				icf_print_pdu(ssa_pdb.p_glb_pdb, (icf_uint8_t *)p_current_sip_pdu,p_nw_int_info->remote_ip_addr, 
                        p_nw_int_info->remote_port_num, p_nw_int_info->pdu_len,0);
#endif									
#if 0
                                    if ((ICF_FAILURE == ret_val) && (Sdf_en_protoTcp == protocol
#ifdef ICF_SECURE_SUPPORT
                                                || Sdf_en_protoTls == protocol))
#else
                                        ))
#endif
                                        {
                                            p_ssa_glb_data->p_ssa_tcpm_conn_db
                                                [ssa_pdb.ssa_conn_db_index].p_ssa_ctxt = 
                                                p_tcp_ssa_ctxt;

                                        }
#endif

                                     /*
                                    Fix for FMR in case of TCP call
                                    We would call this macro only if the flag ICF_SSA_TRACE_REQUIRED is
                                    not set.This flag in ssa pdb ensures that the trace on the receipt on
                                    200 OK for a BYE is sent via the call back sdf_cbk_uaCallTerminated.In
                                    other scenarios the trace is sent from here.
                                    */
                                    if(!(ssa_pdb.common_bitmask & ICF_SSA_TRACE_REQUIRED))
                                    {
#ifdef ICF_TRACE_ENABLE
                                        /* SPR 17555 Fix: Send call direction to 
                                         * app_trace for SIP PDUs
                                         */
                                        ssa_pdb.call_direction = ICF_CALL_DIRECTION_INCOMING;
#endif
                                        ICF_SSA_APP_TRACE(&ssa_pdb , ICF_TRACE_SIP_PDU)
                                    }

                                    if ( ICF_SSA_INTER_MODULAR_MSG & 
                                            ssa_pdb.common_bitmask)
                                    {
                                        /* Checing if alias has come in TLS
                                         * connection */
#if 0
#ifdef ICF_SECURE_SUPPORT
                                        if (ICF_NULL != ssa_pdb.p_conn_table_entry)
                                        {
                                            if (ssa_pdb.p_conn_table_entry->prot_conn_bitmask
                                                    == ICF_TRANSPORT_TLS)
                                            {
                                                if (icf_ssa_is_alias_present_in_Via(pMessage))
                                                {
                                                    ICF_PRINT(("Its persistent TLS" \
                                                                "connection.Alias found\n"));
                                                    if (0 != ICF_PURGE_TIMER_DURATION)
                                                    ssa_pdb.p_conn_table_entry->is_persistent = ICF_TRUE;
                                                }
                                                else
                                                {
                                                    ICF_PRINT(("No alias found. Non persistent TSL\n"));
                                                }
                                            }
                                        }
#endif
#endif
                                        ret_val = icf_ssa_process_otg_intrnl_msg(
                                                &ssa_pdb);
                                        /*
                                         * We are sending a dummy ret_val to
                                         * memfree as it resets the ret_val
                                         * in the case of failure
                                         */
                                        ICF_SSA_FREE_SSA_SCOPE_MEM_CHUNKS(p_glb_pdb,\
                                                (&ssa_pdb), p_ecode, dummy_ret_val)
                                    }
                                    /*FMR in TCP call. */
                                }
                            }
                            
                            break;
                      }/* End to Switch */
                    
                }/* End to if ret_val is success */
                else
                {
                    /* check the ecode filled in ssa_pdb.p_ecode */
                    /* if it is incomplete then set remove_bytes_from_conn
                     * to false
                     */
#ifdef ICF_SIGCOMP_SUPPORT
					if(ICF_TRUE == ssa_pdb.p_glb_cfg_data->sigcomp_required)
                    {
						if(ICF_ECODE_INCOMPLETE_MSG_RECEIVED == *(ssa_pdb.p_ecode))
						{
							remove_bytes_from_conn = ICF_FALSE;
						}
					}
#endif
                }
            }
                ICF_MEMFREE(p_glb_pdb, p_transp_addr, \
                        ICF_MEM_COMMON, p_ecode, dummy_ret_val)

                sdf_ivk_uaFreeEventContext(p_event_ctxt);
                if (!(ssa_pdb.common_bitmask & ICF_SSA_CALL_OBJ_ALREADY_FREED))
                   sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
        }
        if ((ICF_NULL != p_next_msg) && (Sdf_en_protoTcp == protocol) && 
            (ICF_TRUE == remove_bytes_from_conn))
        {
            /* SPR 16019: Check added for invoking TCP recv only in 
             * case of Incomplete messages
             */
            /*Temp fix as the complete fix is awaited from 
             * UATK team*/
            ICF_MEMFREE(p_glb_pdb, p_next_msg, \
                    ICF_MEM_COMMON, p_ecode, dummy_ret_val)

            ICF_PRINT(((icf_uint8_t *)"[SSA]:FreedPduBufferAllocatedByStack\n"));

            /* It may be possible that the connection on which we are about 
             * to read now may have been already closed. Further, in such a 
			 * case, p_conn_db_node->conn_fd will be reset to invalid but 
			 * p_nw_int_info->fd_id may not be reset to invalid.
			 */
			if (ICF_NULL ==  ssa_pdb.p_conn_table_entry)
			{
            	ret_val = ICF_FAILURE;
                if (ICF_NULL != p_call_obj)
                {
                    sdf_ivk_uaFreeCallObject(&p_call_obj);
                }
            	ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!FD not found in ConnDB"));
            	ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
			}
            else if (ICF_FAILURE == icf_port_tcp_recv(
                        (icf_uint16_t)ssa_pdb.p_conn_table_entry->conn_fd,
                        p_recvd_pdu, processed_bytes))
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in removal of bytes from conn"));
            }
            else
            {
                p_current_sip_pdu += processed_bytes;

              /* Added changes to handle scneraio where 2 Messages are 
		        * received in message 
		        */ 
                p_nw_int_info->pdu_len = (icf_uint16_t)(p_nw_int_info->pdu_len - processed_bytes);
                more_messages = ICF_TRUE;
            }
        }
#ifdef ICF_SECURE_SUPPORT	
        else if ((ICF_NULL != p_next_msg) && (Sdf_en_protoTls == protocol) && 
                 (ICF_TRUE == remove_bytes_from_conn))
        {
            icf_void_t      *p_secure_data = ICF_NULL;
            /*Temp fix as the complete fix is awaited from 
             * UATK team*/
            ICF_MEMFREE(p_glb_pdb, p_next_msg, \
                    ICF_MEM_COMMON, p_ecode, dummy_ret_val)

            ICF_PRINT(("[SSA]:FreedPduBufferAllocatedByStack\n"));

           /* 
            * Now extract the SSL Data corrresponding to this
            * socket fd from the map 
            */
            p_secure_data = icf_es_fetch_secure_data_from_map(
                                 ssa_pdb.p_glb_pdb->p_glb_data->p_glb_port_info,
                                 p_nw_int_info->receiver_fd,
                                 p_ecode) ;
            if (ICF_NULL == p_secure_data)
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_port_secure_read(
                p_secure_data,(icf_uint16_t)p_nw_int_info->receiver_fd,
                p_recvd_pdu, processed_bytes))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                p_current_sip_pdu += processed_bytes;
                more_messages = ICF_TRUE;
            }
        }
#endif	
        /* SPR 16019: If next message on TCp is present then 
         * process the message after freeing the memory allo
         * cated by UATK for the next message.(Memory Leak Fix
         * if TCP connection not closed in return path of deregister
         * request.
         */
        /* SPR 17484: CSR 1_5340575 Fixed: this is case of 
         * handle_successful_decode.In this case remove_bytes_from_conn 
         * will be set to false after message is read from the socket.
         */
        else if ((ICF_NULL != p_next_msg) && (Sdf_en_protoTcp == protocol)&&
                (ICF_FALSE == remove_bytes_from_conn))
        {
            icf_port_memcpy(p_current_sip_pdu,\
                    (icf_uint8_t*)p_next_msg,\
                    icf_port_strlen((icf_uint8_t *)p_next_msg)+1);

            ICF_MEMFREE(p_glb_pdb, p_next_msg, \
                    ICF_MEM_COMMON, p_ecode, dummy_ret_val)

            ICF_PRINT(((icf_uint8_t *)"[SSA]:FreedPduBufferAllocatedByStack\n"));

            /* Added changes to handle scneraio where 2 Messages are 
             * received in message 
             */ 
            p_nw_int_info->pdu_len = (icf_uint16_t)(p_nw_int_info->pdu_len - processed_bytes);
            more_messages = ICF_TRUE;
        }
        else
        {
            if ( (ICF_TRUE == remove_bytes_from_conn) &&
                    (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT			
			|| Sdf_en_protoTls == protocol))
#else
		    ))
#endif
            {
                if (Sdf_en_protoTcp == protocol)
                {
                    
				    /*Fix for TCP : SPR 14954*/
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]: No more msg to be processed \n")); 
					processed_bytes = p_nw_int_info->pdu_len;
				
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]: processed_bytes = %d\n", processed_bytes));
					ssa_pdb.processed_bytes = processed_bytes;
					if (ICF_FAILURE == icf_port_tcp_recv(
                            (icf_uint16_t)p_nw_int_info->receiver_fd,
                            p_recvd_pdu, processed_bytes))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in removal of bytes from conn"));
                    }
                }
#ifdef ICF_SECURE_SUPPORT		
                else
                {
                            icf_void_t      *p_secure_data = ICF_NULL;
                            /* 
                             * Now extract the SSL Data corrresponding to this
                             * socket fd from the map 
                             */
                            p_secure_data = icf_es_fetch_secure_data_from_map(
                                    ssa_pdb.p_glb_pdb->p_glb_data->p_glb_port_info,
				                    p_nw_int_info->receiver_fd,
                                    p_ecode) ;
                            if (ICF_NULL == p_secure_data)
                            {
                                ret_val = ICF_FAILURE;
                            }
                            else if (ICF_FAILURE == icf_port_secure_read(
                                p_secure_data,(icf_uint16_t)p_nw_int_info->receiver_fd,
                                p_recvd_pdu, pdu_len))
                            {
                                ret_val = ICF_FAILURE;
                            }
                }
#endif		
            }
            more_messages = ICF_FALSE;
        }
    }while(ICF_TRUE == more_messages);
#ifdef ICF_SESSION_TIMER
    if(Sdf_en_lowSessionTimerIntervalError == sdf_error.errCode)
    {
	    ret_val = ICF_SUCCESS;
    }
#endif
    /*Pdu memory will be freed only when it's a UDP message*/
    if (Sdf_en_protoUdp == protocol)
    {
        /* Freeing the PDU memory. This was allocated in Eshell.
         * Has to be documented when we move on to user documentation*/
        ICF_MEMFREE(p_glb_pdb, p_recvd_pdu, \
                ICF_MEM_COMMON, p_ecode, dummy_ret_val)
            
            /* Added to avoid a leak in cases like #2361 where there is a valid SDP body in the p_next_msg buffer 
             * however the content-length in the Sip message does not match with that of the body length
             * hence the body is not parsed and is freed here in case it is already not.
             * this portion should hit only when the buffer is not freed by  uatk at microsip_decode.c:1946
             * Note that the uatk only frees the buffer if it consists of only whitespaces ,
             * Hence all other improper body lengths this would get hit. 
             */

            if(ICF_NULL != p_next_msg)
                ICF_MEMFREE(p_glb_pdb, p_next_msg, \
                        ICF_MEM_COMMON, p_ecode, dummy_ret_val)

    }    

    ICF_FUNCTION_EXIT(p_glb_pdb)
    

        return ret_val;
}
