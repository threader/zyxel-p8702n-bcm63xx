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
 **17-Jan-2008     Shruti Thakral                Enhancements for Rel 8.0
 **5-Mar-2008      Kamini Gangwani    Rel8.0 UT  Modified function
 **                                              icf_ssa_handle_successful_decode
 **07-Mar-2008     Abhishek Dhammawat Rel8.0     modified  
 **                                              icf_ssa_handle_successful_decode
 **28-Mar-2008    Sumant Gupta       SPR 18311  Merged Publish Message Enhancements
 **                                              icf_ssa_handle_successful_decode 
 **28-Mar-2008     Shruti Thakral     SPR 18314  Klocwork warning removal
 **02-Apr-2008     Tarun Gupta        SPR 18311  Klocwork warning removal 
 **03-Apr-2008     Alok Tiwari        SPR 18363  Klocwork warning removal
 **02-Apr-2008     Anuradha Gupta     SPR 18357  Klocwork warning removal (MLK_MUST_5)
 **06-Apr-2008     Tarun Gupta        SPR 18355  Merged Windows Porting Changes
 **16-Apr-2008     Abhishek Dhammawat SPR18397   Modified
 **                                             icf_ssa_handle_successful_decode
 **18-Apr-2008     Alok Tiwari        SPR 18456  Klocwork warning removal
 * 30-Apr-2008     Amit Sharma        SPR 18487  Sent 400 response in case of
 *                                               BasicHdrError
 * 01-May-2008     Amit Sharma        SPR 18390  Fix for IPTK crash in TLS case
 *                                               when Pri-server is not responding
 **06-May-2008     Alok Tiwari        SPR 18503  Modified function 
 **                                              icf_ssa_process_nw_msg
 **09-May-2008     Alok Tiwari        SPR 18503 Change done in function 
 **                                              icf_ssa_process_nw_msg and
 **                                            icf_ssa_handle_successful_decode
 * 09-May-2008  Abhishek Dhammawat    SPR 18526 modified function
 *                                              icf_ssa_handle_successful_decode
 * 09-May-2008    Amit Sharma         SPR:18503 Review comments incorporated
 * 15-May-2008    Kamini Gangwani     SPR:18554 Modified function
 *                                              icf_ssa_process_nw_msg
 * 17-May-2008         Amit Sharma      SPR 18577     Assigning APP ctxt to glb_pdb
 *
 * 03-Jun-2008    Tarun Gupta         SPR 18585 Modified icf_ssa_handle_decode_failure
 * 04-Jun-2008    Tarun Gupta         SPR 18585 Early NOTIFY handling added.
 * 24-Jun-2008    Jagmohan Chauhan    SPR 18684 CSR Merge 1-6022631
 * 28-Jun-2008    Amit Sharma         SPR 18049 Enhanced to send 400 for non-INVITE requests
 * 03-Jul-2008    Anuradha Gupta      SPR 18761 On decode failure for ACK, crash resolved
 * 11-July-2008   Anurag Khare        SPR 18788 Modified function icf_ssa_handle_successful_decode
 * 16-Jul-2008    Anuradha Gupta      SPR 18813 On decode failure for ACK, if call context
 *                                              does not exist then dont trigger generic error.
 * 18-Jul-2008    Anuradha Gupta      SPR 18803 All occurences of function 
 *                                              icf_ssa_fill_conn_info_from_Via modified due
 *                                              to prototype change.
 **19-Jul-2008    Anuradha Gupta      SPR 18819 Modified function
 **                                              icf_ssa_handle_successful_decode
 **21-Jul-2008    Anuradha Gupta      SPR 18821 Modified function
 **                                              icf_ssa_handle_successful_decode
 **22-Jul-2008    Anuradha Gupta      SPR 18820 Modified function
 **                                              icf_ssa_handle_decode_failure
 **23-Jul-2008    Anuradha Gupta      SPR 18580 Modified function
 **                                              icf_ssa_handle_decode_failure for
 **                                             disabling strict Require check by UATK
 **24-Jul-2008    Anuradha Gupta      SPR 18832 Modified function
 **                                              icf_ssa_handle_decode_failure
 **05-Nov-2008    Rajiv Kumar         SPR 19188 Rel 7.1.2 SPR merging (SPR
 **                                              18867)
 **07-Nov-2008    Rajiv Kumar         SPR 19188 Rel 7.1.2 SPR merging (SPR
 **                                              18956)
 **10-Nov-2008 Abhishek Dhammawat     SPR 19189 Merge SPR18864 function
 **                                             icf_ssa_handle_decode_failure
 **10-Nov-2008 Abhishek Dhammawat     SPR 19189 Merge SPR18864 function
 **                                             icf_ssa_process_nw_msg
 **10-Nov-2008 Abhishek Dhammawat     SPR 19189 Merge SPR18864 function
 **                                             icf_ssa_handle_successful_decode
 **11-Nov-2008 Abhishek Dhammawat     SPR 19189 Modified
 **                                             icf_ssa_handle_successful_decode
 **28-Nov-2008 Abhishek Dhammawat     SPR 19218 Merged CSR 1-6188454
 **01-Dec-2008     Rajiv Kumar        SPR 19215 CSR-1-6212448 Merged
 **08-Dec-2008 Abhishek Dhammawat     SPR 19223 Modified
 **                                             icf_ssa_handle_decode_failure
 **08-Dec-2008 Rajiv Kumar            SPR 19218 Merged CSR 1-6431768
 **29-Jan-2009 Alok Tiwari           Rel_8.1   compilation warning resolved
 **                                after enabling the flag SDF_THREAD_SAFE and
 **                                SIP_LOCKEDREFCOUNT.
 **02-Feb-2008 Kamal Ashraf           SPR 19294 Fixed for forking CSR 1-5232801 merge
 ** 02-Mar-2009     Alok Tiwari      IPTK Rel8.1 IPTK open source replacement:
 **                                              Data related to ares has
 **                                              been removed.
 ** 04-Mar-2009 Saurabh Gupta         SPR 19349      Klocworks fix
 ** 18-Mar-2009 Rajiv Kumar           IPTK Rel8.1    UT Defect Fix 
 ** 30-Mar-2009 Tarun Gupta           Rel 8.2    REFER/NOTIFY Enhancements
 ** 1-Apr-2009  Abhishek Dhammawat    Rel 8.2    REFER/NOTIFY Enhancements
 ** 4-Apr-2009  Rajiv Kumar           SPR 19423  Klocwork Warning Fix. 
 ** 7-Apr-2009  Rajiv Kumar           SPR 19428  CSR-1-6847773 Merged 
 ** 7-Apr-2009  Rajiv Kumar           SPR 19428  CSR-1-6779028 Merged
 ** 26-Apr-2009 Anuradha Gupta        SPR 19438  Modified fn 
 **                                               icf_ssa_handle_successful_decode
 ** 27-Apr-2009 Tarun Gupta           SPR 19388  Modified icf_ssa_process_nw_msg,
 **                                              icf_ssa_handle_decode_failure
 ** 28-Apr-2009 Abhishek Dhammawat    SPR 19527  Modified icf_ssa_handle_decode_failure
 ** 02-May-2009 Abhishek Dhammawat    SPR 19571  Modified
 **                                              icf_ssa_process_nw_mesg 
 ** 5-May-2009  Tarun Gupta           SPR 19592  Modified icf_ssa_handle_successful_decode
 ** 6-May-2009  Rajiv Kumar           SPR 19531  Klocworks fix
 ** 25-May-2009 Rajiv Kumar           SPR 19666  Fix For SPR: 19666
 ** 11-Jun-2009 Kamal Ashraf          SPR 19590  Modified icf_ssa_handle_successful_decode 
 ** 11-Jun-2009 Ashutosh Mohan        SPR 19737  Changes done for GCC 4.1 warnings removal
 ** 21-Jul-2009 Anuradha Gupta        SPR 18906  Modified icf_ssa_process_nw_msg
 ** 24-Jul-2009 Anuradha Gupta        SPR 19097  Modified icf_ssa_handle_decode_failure
 ** 27-Jul-2009 Tarun Gupta           SPR 19886  CSR_1-7233803 merged
 ** 28-Jul-2009 Tarun Gupta           SPR 19886  CSR_1-7155416 merged
 ** 30-Jul-2009 Anuradha Gupta        UT Defect  Modified icf_ssa_process_nw_msg
 ** 30-Jul-2009 Anuradha Gupta        UT Defect  Modified icf_ssa_handle_successful_decode 
 ** 03-Aug-2009 Rajiv Kumar           SPR 19886  Merged SPR 19534  
 ** 17-Aug-2009 Anuradha Gupta        SPR 19927  klocworks warning removal
 ** 20-Aug-2009 Anuradha Gupta        SPR 19927  GCC warning removal
 ** 03-Sep-2009 Anuradha Gupta        SPR 19999  Modified icf_ssa_handle_successful_decode
 ** 04-Sep-2009 Preksha               SPR 19999  Merged CSR 1-7586154
 ** 07-Sep-2009 Preksha               SPR 19999  Merged SPR 19992
 ** 08-Sep-2009 Rajiv Kumar           SPR 19999  Merged CSR 1-7586127 
 ** 08-Sep-2009 Preksha               SPR 19999  Merged SPR 19993
 ** 09-Sep-2009 Preksha               SPR 19999  Merged SPR 19995
 ** 14-Sep-2009 Rajiv Kumar           SPR 19999  Merged CSR 1-7605218
 ** 17-Sep-2009 Rajiv Kumar           SPR 19999  Merged CSR 1-7694021
 ** 18-Sep-2009 Tarun Gupta           SPR 20061  Modified icf_ssa_handle_successful_decode
 ** 23-Sep-2009 Tarun Gupta           SPR 20063  Merged CSR 1-7576668
 ** 24-Sep-2009 Rajiv Kumar           SPR 20067  Fix for SPR 20067
 ** 05-Oct-2009 Abhishek Dhammawat    SPR 20063  Merged CSR 1-7436195
 ** 09-Oct-2009 Anuradha Gupta        SPR 20104  Modified icf_ssa_handle_successful_decode
 ** 21-Oct-2009 Abhishek Dhammawat    SPR 20148  Modified icf_ssa_handle_decode_failure
 ** 23-Oct-2009 Anurag Khare          SPR 20172  Fix for SPR: 20172
 ** 27-Oct-2009 Rajiv Kumar           SPR 20192  Fix For SPR:20192
 ** 10-Jun-2010  Rohit Aggarwal  CSR 1-8678911  Save line id from mapped ctxt
 **                                             into temp ssa ctxt
 **
 * Copyright 2010 (c) Aricent.
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
#include "sdf_portlayer.h"

/*DBM inclusions*/
#include "icf_dbm_prototypes.h"

/*CFG inclusions*/
#include "icf_cfg_defs.h"

#ifdef ICF_SIGCOMP_SUPPORT
#include "icf_sigsa_defs.h"
#endif


#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif


/* Global defintion of persistent global pdb */
icf_glb_pdb_st        *p_persistent_glb_pdb;
/* Codenomicon fixes: CSR 1-7132323: SPR 19484/19571 */
extern Sdf_ty_retVal sdf_ivk_uaIPTKCheckMessageLength(Sdf_ty_s8bit *pString, 
		Sdf_ty_u32bit dLen,Sdf_ty_u32bit dLen2, Sdf_ty_u32bit *pOption,
        Sdf_ty_IPTKmessageLengthValidity *pResult, Sdf_st_error *pError);

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

#ifdef ICF_UT_TEST
    /* p_ecode declared in icf_glb_pdb_st will point to 
       ecode in icf_internal_msg_st. To change this ecode, 
       value of pointer will be changed */
    p_internal_msg->msg_hdr.p_glb_pdb->p_ecode =
        &(p_internal_msg->msg_hdr.ecode);
#endif

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
            /* WE are not checkin the ret_val of process_sip_msg here
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

#ifdef ICF_SESSION_TIMER
        case ICF_SSA_SESSION_TIMER_TOGGLE:
           {
               if ((ICF_NULL == p_ssa_pdb->p_glb_pdb) || 
                       (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
               {
                   ret_val = ICF_FAILURE; 
               } 
               else
               {
                   ret_val = icf_ssa_session_timer_toggle(p_ssa_pdb);
               }
               break;
           }
#endif 
        /* REFER/NOTIFY Enhancements */
        case ICF_NW_OTG_GENERIC_MSG_CFM:
           {
               ret_val = icf_ssa_process_generic_msg_cfm(p_ssa_pdb);
               break;
           }
        case ICF_NW_OTG_GENERIC_MSG_REQ:
           {
               ret_val = icf_ssa_process_generic_msg_req(p_ssa_pdb);
               break;
           }
        /* End REFER/NOTIFY Enhancements */

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

										if ( ((p_rgm_ctxt->num_of_lines - 1) < ICF_MAX_NUM_OF_LINES) && ((p_rgm_ctxt->num_of_lines - 1) >= 0) && (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
																						p_glb_pdb,
																						p_rgm_ctxt->rgm_list_of_lines[p_rgm_ctxt->num_of_lines - 1],
																						&(ssa_pdb.p_line_data),
																						ssa_pdb.p_ecode)))
										{
														ret_val = ICF_FAILURE;
														ICF_FUNCTION_EXIT(ssa_pdb.p_glb_pdb)
																		return ret_val;
										}
										if ((ICF_NULL != p_ssa_ctxt) && ((p_rgm_ctxt->num_of_lines - 1) < ICF_MAX_NUM_OF_LINES) && ((p_rgm_ctxt->num_of_lines - 1) >= 0))
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

    /*This bit is set in ssa_pdb when primary server is not responding
     *and SSA sent request on secondary server, so call object with primary server
     *will be freed in this case*/
    if (ICF_SSA_DEALLOCATE_CALL_OBJECT & ssa_pdb.common_bitmask)
    {
        Sdf_st_callObject *p_call_obj = ssa_pdb.p_call_obj_to_dealloc;

        sdf_ivk_uaFreeCallObject(&p_call_obj);
    }
  
    /*klocwork warning removal*/
    ICF_KLOCWORK_FALSE_POSITIVE(ssa_pdb.p_glb_pdb,
                           ssa_pdb.p_ssa_glb_data->p_purge_timer)

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
    icf_return_t             ret_value = ICF_SUCCESS;
    icf_return_t             ret_val_memfree = ICF_SUCCESS;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;
    Sdf_st_commonInfo           *p_common_info = Sdf_co_null;
    Sdf_st_callObject           *p_orig_call_obj = Sdf_co_null;
    SipOptions                      dOptions = {0};
    Sdf_ty_s8bit                *p_method = Sdf_co_null;
    Sdf_ty_s8bit                *p_method_temp = Sdf_co_null;
    SipMessage		            *pMessage = Sdf_co_null,
                                *p_sip_msg = Sdf_co_null;
    Sdf_ty_callLegMatch         call_leg_match = Sdf_en_notMatching;
    icf_ssa_ctxt_st			    *p_tmp_ssa_ctxt = ICF_NULL;
    icf_boolean_t               dummy_flag = ICF_TRUE;
    icf_line_data_st            *p_line_data = ICF_NULL;
    icf_string_st               rem_addr_active_str;
    icf_string_st               rem_addr_inactive_str;
    /* FQDN is present update the active IP in DNS Buffer */
    icf_dns_buffer_st           dns_info_to_store =\
                                   {{0,{'\0'}},{0},0,0,0,{0,{'\0'}},{0,{'\0'}},{0,{'\0'}},0};
    Sdf_st_overlapTransInfo   *pTransInfo = Sdf_co_null;
    en_SipMessageType   dMsgType = SipMessageAny;
	/* Fix for CSR 1-6847773: SPR 19296 */
	icf_boolean_t            	cc_context_cleared = ICF_FALSE;
    /*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
    Sdf_ty_s8bit                *pViaBranch = Sdf_co_null;
    SipHeader                   dViaHeader;
    Sdf_ty_s8bit                key[ICF_MAX_LARGE_STR_LEN] = "\0";
    Sdf_ty_s8bit                *p_key = ICF_NULL;
    icf_boolean_t               via_handling = ICF_FALSE;  

    /* Fix for SPR: 19999(Merged CSR 1-7605218: SPR-19997 fixed)*/
    SipReqLine                  *p_req_line = SIP_NULL;
    SipAddrSpec                 *p_req_addr_spec = SIP_NULL;
    icf_address_st              req_uri = {{0,"\0"},0};

    /*Start SPR 18864*/
    if((ICF_NULL == p_ssa_pdb) || 
       (ICF_NULL == p_ssa_pdb->p_glb_pdb) ||
       (ICF_NULL == p_event_ctxt))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SSA PDB/GLB PDB/Event Ctx is null"));
        return ICF_FAILURE;
    }
    else
    {
        p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    }/*End SPR 18864*/
    ICF_FUNCTION_ENTER(p_glb_pdb)

    icf_cmn_init_string(&rem_addr_active_str);

    icf_cmn_init_string(&rem_addr_inactive_str);

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

        /*Fix for CSR 1-7586127 : SPR-19988*/
        /*SPR 18864 Added null checks for the UAC/UAS txn*/
        if ((Sdf_co_null != p_call_obj->pUacTransaction) &&
                (Sdf_co_null != p_call_obj->pUacTransaction->pSipMsg))
        {
            p_temp_msg = p_call_obj->pUacTransaction->pSipMsg;
        }
        else if(Sdf_co_null != p_call_obj->pUasTransaction) 
        {
            p_temp_msg = p_call_obj->pUasTransaction->pSipMsg;
        }

        /* get the config data pointer from DBM */
        if (ICF_FAILURE ==
                icf_dbm_get_module_glb_data(
                    p_ssa_pdb->p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t *)&(p_ssa_pdb->p_glb_cfg_data),
                    p_ssa_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;
        }
        /* If Application has set the bitmask for strict parsing then
         * validate the CANCEL transaction based on via.
         */
        if (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING ==
                (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING &
                 p_ssa_pdb->p_glb_cfg_data->default_settings))
        {
            sip_getMessageType(p_temp_msg, &dMsgType, \
                    (SipError*)	&(p_sdf_error->stkErrCode)) ;

            if(Sdf_co_fail ==
                    sdf_ivk_uaGetMethodFromSipMessage(
                        p_temp_msg, &p_method, p_sdf_error))
            {
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return ret_val;
            }
            else if (SipFail == sip_getHeaderAtIndex(
                        p_temp_msg, SipHdrTypeVia,
                        &dViaHeader, 0, (SipError*)&(p_sdf_error->stkErrCode)))
            {
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return ret_val;
            }
            else if (Sdf_co_fail == sdf_fn_GetBranchFromViaHdr(&dViaHeader,
                        &pViaBranch,p_sdf_error))
            {
                pViaBranch = ICF_NULL;
            }
        }
        /* Do the transaction matching of CANCEL request based on via header
         * if following is TRUE:
         * 1) Application has set the bitmask for strict parsing.
         * 2) Message type is request and method s CANCEL.
         * 3) if Via header is present and it contains the magic cookie.
         */ 
        if((ICF_CALL_CONFIG_ENABLE_STRICT_PARSING ==
               (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING &
                p_ssa_pdb->p_glb_cfg_data->default_settings))
                                 &&
               (SipMessageRequest == dMsgType) && (ICF_NULL ==
               icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t*)"CANCEL"))
                                 &&
                ((ICF_NULL != pViaBranch) &&
                (0 == icf_port_strncmp((icf_uint8_t *)pViaBranch,\
                                        (icf_uint8_t *)"z9hG4bK",7))))
        {
            if(SipFail ==
                    sip_getSentByFromViaHdr(&dViaHeader,\
                        &p_key,\
                        (SipError*)&(p_sdf_error->stkErrCode)))
            {
                ret_val = ICF_FAILURE;
            }

            if (ICF_NULL != p_key)
            {
                icf_port_strcpy((icf_uint8_t *)key,(icf_uint8_t *)p_key);
                icf_port_strcat((icf_uint8_t *)key,(icf_uint8_t *)pViaBranch);

                if(ICF_SUCCESS == icf_ssa_map_nw_via(
                            p_ssa_pdb, 
                            p_call_obj, 
                            (icf_uint8_t *)key,
                            &p_ssa_ctxt, 
                            p_sdf_error ))
                {
                    ret_val = ICF_SUCCESS;
                    call_leg_match = Sdf_en_sameCallLeg;
                    p_orig_call_obj = p_ssa_ctxt->p_call_obj;
                    via_handling = ICF_TRUE;
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

            sip_freeSipHeader(&dViaHeader);
        }
        else
        {
            /*Fix for CSR 1-7586127 : SPR-19988*/
            /* If Application has set the bitmask for strict parsing then
             * free the dViaHeader which was allocated above .
             */
            if (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING ==
                    (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING &
                     p_ssa_pdb->p_glb_cfg_data->default_settings))
            {
                sip_freeSipHeader(&dViaHeader);
            }
            p_ssa_pdb->hash_index = 0;
            if(ICF_SUCCESS == icf_ssa_map_nw_callid(
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
                if(ICF_NULL == p_ssa_ctxt)
                {
                    ret_val = ICF_FAILURE;
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SSA Ctx is null"));
                    ICF_FUNCTION_EXIT(p_glb_pdb)
                        return ICF_FAILURE;
                }
                p_orig_call_obj = p_ssa_ctxt->p_call_obj;

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
                    /*  Fix for spr 18788 */
                    /*  It should be done for Incoming Notify */
                    /*  SCENARIO wher problem: If INcoming forked 180 received without to tag
                        then this call_leg_match is becoming Sdf_en_sameCallLeg that is not 
                        right for forked responses. Because in forked responses the ssa_ctxt 
                        that should be sent to UATK shopuld be the new one and not the 
                        old one(permamanant one)*/
                    if(Sdf_co_success ==
                            sdf_ivk_uaGetMethodFromSipMessage(
                                p_temp_msg, &p_method, p_sdf_error))
                    {
                        if(ICF_NULL == 
                                icf_port_strcmp((icf_uint8_t *)p_method,
                                    (icf_uint8_t *)"NOTIFY"))
                        {
                            /* SPR 18599: In case of early NOTIFY, it should
                             * be treated as in dialog NOTIFY therefore the
                             * following check is added */
                            if(ICF_SUCCESS == icf_ssa_util_cmp_tags(
                                        p_ssa_pdb,
                                        p_call_obj, 
                                        p_orig_call_obj,
                                        p_sdf_error))
                            {
                                call_leg_match = Sdf_en_sameCallLeg;
                            }
                            else
                            {
                                ret_val = ICF_FAILURE; 
                            } 
                        }
                        /* SPR 18821: In case of SIP messages, other than NOTIFY
                         * if the call leg is not matching then ret val should be set
                         * as failure so that the message is not treated as in-dialog
                         * message and processing should be done as an out of dialog
                         * message, the problem was encountered when a reinvite was 
                         * received which has To tag missing then the correct event context
                         * could not be found resulting into junt ssa context and eventual
                         * crash as the call back for reinvite was issued.
                         */
                        else if(ICF_NULL == 
                                icf_port_strcmp((icf_uint8_t *)p_method,
                                    (icf_uint8_t *)"INVITE"))
                        {
                            if(Sdf_co_null != p_sdf_error)
                            {
                                sip_getMessageType(p_temp_msg, &dMsgType, \
                                        (SipError*)	&(p_sdf_error->stkErrCode)) ;
                            }    

                            if (SipMessageRequest == dMsgType)
                            {
                                ret_val = ICF_FAILURE;
                            }
                            else
                            {
                                /* In case of response, this could possibly be a forked
                                 * response so it should match the transaction
                                 */ 
                                ret_val = ICF_SUCCESS;
                            }
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
            /* Fix for SPR: 19999(Merged CSR 1-7605218: SPR-19997 fixed)*/
            if (ICF_NULL != p_call_obj->pUasTransaction->pSipMsg)
            {
                p_sip_msg = p_call_obj->pUasTransaction->pSipMsg;
            }
            else 
            {
                Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;
                if (ICF_SUCCESS == icf_ssa_fetch_matching_overlap_txn(
                            p_ssa_pdb,Sdf_en_options,Sdf_en_transactionSent,
                            p_call_obj,&p_ovtxn))
                {
                    p_sip_msg = p_ovtxn->pSipMsg;
                }
            }
        }
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = ICF_SUCCESS;
            if((Sdf_en_sameCallLeg != call_leg_match) &&
               (Sdf_co_null != p_sdf_error))
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
		        if((ICF_NULL != p_ssa_ctxt) && 
        		   (ICF_NULL != p_ssa_ctxt->p_call_ctxt))
	        	{
                    /* Fix for CSR 1-6847773: SPR 19296 */
                    if ((ICF_INVALID_CALL_ID == p_ssa_ctxt->p_call_ctxt->call_id) ||
                            (ICF_INVALID_APP_ID == p_ssa_ctxt->p_call_ctxt->app_id))
                    {
                        cc_context_cleared = ICF_TRUE;
                    }
                    else  
                    {
                        p_ssa_ctxt->p_call_ctxt->p_sip_ctx = p_ssa_ctxt;
                    }                    
		        }
            }

            /* SPR 19438: The call context should be updated in the new SSA context
             * only when the received message is response as this code fix is required for
             * forked response. For Request messages , the call context shall be updated
             * in the respective callback messages.
             * This following code fix of updating the call context has conflicted with
             * the scenarios whre an incoming request message has been rejected being invalid
             * after returning failure from UpdateCallDetails. Since the call context has been 
             * updated therefore on timer expiry the temporary call object is not released taking
             * the temp call object as a call object for forked response. Therefore the check for 
             * incoming message as Response has been added here.
             */ 
            sip_getMessageType(p_temp_msg, &dMsgType, \
                            (SipError*)	&(p_sdf_error->stkErrCode)) ;

            if (SipMessageResponse == dMsgType)
            { 
                /*Fix For SPR : 19294 Start*/
                /*Here in the following code p_ssa_pdb->p_ssa_ctxt specify the ssa context which 
                  is allocated for the received message from network and p_ssa_ctxt specify the 
                  ssa context which is allocated for the initial request.For supporting the session
                  timer functionality in forking scenario populate the call context value for 
                  p_ssa_pdb->p_ssa_ctxt->p_call_ctxt with  p_ssa_ctxt->p_call_ctxt if the same in NULL.
                  This is done since before starting the session timer IPTK checks call context in ssa 
                  context for NULL*/
                /* Fix for SPR:20067*/
                /* Update the call context of ssa context in ssa pdb only when
                 * we have not received the final response yet. This is done
                 * as when forked 200 OK response is received after call
                 * connect is done with prevoius  200 OK(from UAS1) response then
                 * while sending the ACK followed by BYE for the UAS2 if we
                 * are updating the  p_ssa_pdb->p_ssa_ctxt->p_call_ctxt with
                 * p_ssa_ctxt->p_call_ctxt while processing the 200 OK
                 * response to BYE we are incorrectly updating the sip context
                 * of call context. As a result when application wants to
                 * terminate the call we are not able to do so as for sending
                 * BYE for call we are using incorrect ssa context ie
                 * incorrect call object.This check also make sure that before
                 * call connect session timer functionality must work fine in
                 * forking scenario.*/
                if ((ICF_FALSE == ICF_IS_BIT_SET(p_ssa_ctxt->bitmask,
                                      ICF_FINAL_RESP_RECEIVED)) &&
                        ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt) 
                {
                    p_ssa_pdb->p_ssa_ctxt->p_call_ctxt = p_ssa_ctxt->p_call_ctxt;
                }	
                /*Fix for SPR : 19294 End*/
            } 
            /* SPR 19590 : Start */
            if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
            {
               /*SPR 19590 Increment the last_txn_id in call ctx to mark the
                 incoming message this is required as the header/body list
                 txnid needs to be unique with each message*/
               p_ssa_ctxt->p_call_ctxt->last_txn_id++;
               p_ssa_ctxt->p_call_ctxt->txn_id =  p_ssa_ctxt->p_call_ctxt->last_txn_id;
            }
            /* SPR 19590 : End */

            p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
            icf_ssa_alloc_app_ctx_in_glb_pdb(p_ssa_pdb);

#ifdef ICF_DNS_LOOKUP_ENABLED
			/* code for DNS buffer starts here */	    
	    if((ICF_NULL != p_ssa_ctxt)
            && (p_ssa_ctxt->bitmask & ICF_SSA_DNS_FQDN_PRESENT))
	    {
              p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
              p_ssa_ctxt->p_ssa_pdb = p_ssa_pdb;
            
              icf_port_strcpy(dns_info_to_store.ip_address, p_ssa_ctxt->ip_fr_req);
              dns_info_to_store.port = p_ssa_ctxt->port_fr_req;

              /* Copy transport mode from icf_ssa_ctxt_st into DNSBuffer */
              dns_info_to_store.transport_mode = p_ssa_ctxt->transport;

              /* Populate the SRV and NAPTR query if any*/
              /* Following are the fqdn for SRV and NAPTR record.These are populated
               * in sdf_lookupport.c if any srv or naptr query is performed.Also
               * while adding the A query response in dns buffer same will be
               * populated in dns buffer,so that at dns purge timer expiry same can
               * be removed from DNS Client cache*/              
              if (0 != p_ssa_ctxt->naptr_fqdn.str_len)
              {
                  icf_port_strcpy(dns_info_to_store.naptr_fqdn.str,\
                          p_ssa_ctxt->naptr_fqdn.str);
                  
                  dns_info_to_store.naptr_fqdn.str_len = p_ssa_ctxt->\
                                                       naptr_fqdn.str_len; 
              }
              if (0 != p_ssa_ctxt->srv_fqdn.str_len)
              {
                  icf_port_strcpy(dns_info_to_store.srv_fqdn.str,\
                          p_ssa_ctxt->srv_fqdn.str);
                  
                  dns_info_to_store.srv_fqdn.str_len = p_ssa_ctxt->\
                                                       srv_fqdn.str_len; 
              }    
              /* Fix For SPR:20192*/
              /* Following are the fqdn for A query.This is populated
               * in sdf_lookupport.c if any A query is performed.Also
               * while adding the A query response in dns buffer same will be
               * populated in dns buffer,so that at dns purge timer expiry same can
               * be removed from DNS Client cache*/               
              if (0 != p_ssa_ctxt->queryA_fqdn.str_len)
              {
                  icf_port_strcpy(dns_info_to_store.queryA_fqdn.str,\
                          p_ssa_ctxt->queryA_fqdn.str);
                  
                  dns_info_to_store.queryA_fqdn.str_len = p_ssa_ctxt->\
                                                       queryA_fqdn.str_len; 
              }                    

              /* Fix for CSR 1-5078472 : SPR 15937, also check if conn_table_entry 
               * is not NULL, It can be NULL when UATK internally invokes TCP connection*/
              if ((ICF_NULL != p_ssa_ctxt->p_conn_table_req_entry) &&
                  ((Sdf_en_protoTcp == p_event_ctxt->dProtocol)
#ifdef ICF_SECURE_SUPPORT
                    || (Sdf_en_protoTls == p_event_ctxt->dProtocol )))
#else
                  ))
#endif	
                {
                        icf_ssa_tcpm_conn_data_st  *p_ssa_ctxt_db_node =
                                    p_ssa_ctxt->p_conn_table_req_entry;
                        /*Start SPR 18864: Added null check for p_ssa_ctxt_db_node */
                        if(ICF_NULL != p_ssa_ctxt_db_node)
                        {
                            icf_port_strcpy(dns_info_to_store.ip_address,
                                p_ssa_ctxt_db_node->peer_ip);
                        }    
                        dns_info_to_store.port = p_ssa_ctxt_db_node->peer_port_num;
                }
                icf_port_strcpy(dns_info_to_store.fqdn.str, p_ssa_ctxt->fqdn.str);
                dns_info_to_store.fqdn.str_len = icf_port_strlen(
								       dns_info_to_store.fqdn.str);
        }
#endif
        /*This code part is marking primary or secondary server as active
         *from where the response is receivde, this should be done only
         *when p_rem_addr_active and p_rem_addr_inactive are NULL in SSA context
         *as non-NULL of these parameters means primary and secondary servers were
         *configured and request was sent through these servers, if only p_rem_addr_active
         *is non-NULL and p_rem_addr_inactive is NULL means only primary server is configured
         *then also this code should not be invoked, as we don't have sec server to be mark
         *as active, both NULL means request is direct or some response is already received
         *from remote*/
        if ((ICF_NULL != p_ssa_ctxt) &&
            (ICF_NULL != p_ssa_ctxt->p_rem_addr_active) &&
            (ICF_NULL != p_ssa_ctxt->p_rem_addr_inactive))
        {
                icf_cmn_convert_transport_addr_to_string(&(p_ssa_ctxt->
                        p_rem_addr_active->addr),rem_addr_active_str.str);
                icf_cmn_convert_transport_addr_to_string(&(p_ssa_ctxt->
                        p_rem_addr_inactive->addr),rem_addr_inactive_str.str);

                /* If request is routed through proxy, first DHCP response 
                 * will be checked for fetching proxy address. If DHCP server 
                 * was not configured, application configured proxy shall be 
                 * fetched. Next, the currently active server shall be decided 
                 * and marked active*/
                /* If request is routed through proxy */
                if (ICF_NULL != (p_ssa_ctxt->bitmask_1 & 
                            ICF_SSA_REQ_ROUTED_THROUGH_PROXY))
                {
                    /*Start SPR 19189 Added null check*/
                    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                            p_ssa_pdb->p_glb_cfg_data, ret_value)
                    if(ICF_FAILURE == ret_value)
                    {
                        ret_val = ICF_FAILURE;
                        ICF_FUNCTION_EXIT(p_glb_pdb)
                        return ICF_FAILURE;
                    }/*End SPR 19189 */
                    /* if proxy is to be fetched from DHCP response */
	                if (ICF_NULL != p_ssa_pdb->p_glb_cfg_data->p_dhcp_resp)
	                {
                        /* if active server address is used */
		                if (ICF_NULL == (p_ssa_ctxt->bitmask_1 &
                                    ICF_SSA_SEC_SERVER_ADDRESS_IS_USED))
		                {
                            /*if active server used and sec. proxy are same*/
			                if (0 == icf_port_strcmp(
                                    rem_addr_active_str.str,
			                        p_ssa_pdb->p_glb_cfg_data->
                                        p_dhcp_resp->proxy_list[1].str))
			                {
				                p_ssa_ctxt->bitmask_1 |= 
                                    ICF_SEC_PROXY_SERVER_ACTIVE;
                                /* Flag to mark secondary server active
                                 * is set in global config data if
                                 * proxy is configured from DHCP response */
				                p_ssa_pdb->p_glb_cfg_data->
                                    sec_proxy_server_active = ICF_TRUE;
                                
                                /* Generate trace for secondary proxy active */
                                ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_SECONDARY_PROXY_SERVER_ACTIVE)
                                    
			                }/* end if for compared addresses */
                            /* else if compared addresses are not same */
			                else
			                {
				                p_ssa_ctxt->bitmask_1 &= 
                                    ~ICF_SEC_PROXY_SERVER_ACTIVE;
				                p_ssa_pdb->p_glb_cfg_data->
                                    sec_proxy_server_active = ICF_FALSE;

                                /* Generate trace for primary proxy active */
                                ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_PRIMARY_PROXY_SERVER_ACTIVE)
                                    
			                }/* end else for compared addresses not same */
		                }/* end if active server address used */
                		else /* inactive server address used */
		                {
                            /* if inactive address and sec. proxy are same */
			                if (0 == icf_port_strcmp(
                                    rem_addr_inactive_str.str,
                        			p_ssa_pdb->p_glb_cfg_data->
                                        p_dhcp_resp->proxy_list[1].str))
			                {
                                /* mark sec. proxy active */
				                p_ssa_ctxt->bitmask_1 |= 
                                    ICF_SEC_PROXY_SERVER_ACTIVE;
                				p_ssa_pdb->p_glb_cfg_data->
                                    sec_proxy_server_active = ICF_TRUE;

                                /* Generate trace for secondary proxy active */
                                ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_SECONDARY_PROXY_SERVER_ACTIVE)

			                }
                            /* else if compared addresses are not same */
			                else
                			{
                                /* mark primary proxy active */
                				p_ssa_ctxt->bitmask_1 &= 
                                    ~ICF_SEC_PROXY_SERVER_ACTIVE;
				                p_ssa_pdb->p_glb_cfg_data->
                                    sec_proxy_server_active = ICF_FALSE;

                                /* Generate trace for primary proxy active */
                                ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_PRIMARY_PROXY_SERVER_ACTIVE)
                                    
			                }/* end else compared addresses not same */
		                }/* end else inactive server address used */
	                }/* end if proxy is to be fetched from DHCP response */
	                /* else proxy to be fetched from line_data */
                    else
	                {
                        /* Fetch line_data from line_id */
		                if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                                p_glb_pdb, p_ssa_ctxt->line_id, 
                                &p_line_data, p_ecode))
		                {
			                ret_val = ICF_FAILURE;
		                }
		                else/* SUCCESS - fetch line_data */
		                {
                            /* if active server address is used */
		                    if (ICF_NULL == (p_ssa_ctxt->bitmask_1 &
                                    ICF_SSA_SEC_SERVER_ADDRESS_IS_USED))
                            {
                                /* if sec proxy in line_data matches active
                                 * server used */
			                    if (ICF_SUCCESS == 
                                    icf_cmn_compare_transport_addresses(
                                    p_line_data->p_sec_proxy_addr,
				                    p_ssa_ctxt->p_rem_addr_active,
                                    p_glb_pdb))
			                    {
                                    /* mark secondary active */
                				    p_ssa_ctxt->bitmask_1 |= 
                                        ICF_SEC_PROXY_SERVER_ACTIVE;
                                    /* Flag to mark secondary server active
                                     * is set in line data if proxy is 
                                     * configured by application */
				                    p_line_data->sec_proxy_server_active = 
                                        ICF_TRUE;

                                    /* Generate trace for sec. proxy active*/
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_SECONDARY_PROXY_SERVER_ACTIVE)
			                    }
			                    else /* primary proxy used */
			                    {
                                    /* mark primary active */
				                    p_ssa_ctxt->bitmask_1 &= 
                                        ~ICF_SEC_PROXY_SERVER_ACTIVE;
				                    p_line_data->sec_proxy_server_active = 
                                        ICF_FALSE;

                                    /* Generate trace for primary proxy active*/
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_PRIMARY_PROXY_SERVER_ACTIVE)
                                        
			                    }/* primary proxy used */
                            }/* end if inactive server address used */
                            else/* inavtive server address is used */
                            {
                                /*if inactive address and sec. proxy are same */
			                    if (ICF_SUCCESS == 
                                    icf_cmn_compare_transport_addresses(
                                    p_ssa_ctxt->p_rem_addr_inactive,
                        			p_line_data->p_sec_proxy_addr,
                                    p_glb_pdb))
			                    {
                                    /* mark sec. proxy active */
				                    p_ssa_ctxt->bitmask_1 |= 
                                        ICF_SEC_PROXY_SERVER_ACTIVE;
                                    p_line_data->
                                        sec_proxy_server_active = ICF_TRUE;

                                    /* Generate trace for sec. proxy active */
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_SECONDARY_PROXY_SERVER_ACTIVE)
			                    }
                                /* else if compared addresses are not same */
			                    else
                			    {
                                    /* mark primary proxy active */
                				    p_ssa_ctxt->bitmask_1 &= 
                                        ~ICF_SEC_PROXY_SERVER_ACTIVE;
				                    p_line_data->
                                        sec_proxy_server_active = ICF_FALSE;

                                    /* Generate trace for primary proxy active*/
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_PRIMARY_PROXY_SERVER_ACTIVE)
                                        
			                    }/* end else compared addresses not same */
                            }/* end else - inactive server address used */
                        }/* SUCCESS - fetch line data */
		            }/* end else proxy fetched from line_data */
                }/* end if request routed through proxy */
                else /* Request is not routed through proxy */
                {
                    /* Get Method from the message */
	                if(Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                         p_temp_msg, &p_method, p_sdf_error))
                    {
        	            ret_val = ICF_FAILURE;
                    }
                    /* Update registrar active/inactive only for REGISTER req */
                    else if ( ICF_NULL == icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"REGISTER"))
                    {
                        /* Fetch line_data for line_id */
		                if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                                    p_glb_pdb, p_ssa_ctxt->line_id, 
                                    &p_line_data, p_ecode))
		                {
			                ret_val = ICF_FAILURE;
		                }/* end if - FAILURE - fetch line data */
		                else/* SUCCESS - fetch line data */
		                {
			                /* if active server address used */
                            if (ICF_NULL == (p_ssa_ctxt->bitmask_1 &
                                    ICF_SSA_SEC_SERVER_ADDRESS_IS_USED))
			                {
                                /* if active server and sec. registrar same */
				                if (ICF_SUCCESS == 
                                        icf_cmn_compare_transport_addresses(
                                            p_ssa_ctxt->p_rem_addr_active,
				                            p_line_data->p_sec_registrar_addr,
                                            p_glb_pdb))
				                {
                                    /* mark secondary reg. active */
					                p_ssa_ctxt->bitmask_1 |= 
                                        ICF_SEC_REGISTRAR_ACTIVE;
					                p_line_data->sec_registrar_active =ICF_TRUE;

                                    /*Generate trace for sec. registrar active*/
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_SECONDARY_REGISTRAR_ACTIVE)
				                }
				                else/* active server and prim. reg same */
				                {
                                    /* mark primary active */
					                p_ssa_ctxt->bitmask_1 &= 
                                        ~ICF_SEC_REGISTRAR_ACTIVE;
					                p_line_data->sec_registrar_active =ICF_FALSE;

                                    /*Generate trace - prim. registrar active*/
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_PRIMARY_REGISTRAR_ACTIVE)
				                }/*end else active server and prim. reg. same */
			                }
			                else /* Inactive server address used */
			                {
                                /* if inactive server and sec. registrar same */
				                if (ICF_SUCCESS == 
                                        icf_cmn_compare_transport_addresses(
                                            p_ssa_ctxt->p_rem_addr_inactive,
				                            p_line_data->p_sec_registrar_addr,
                                            p_glb_pdb))
				                {
                                    /* mark secondary reg. active */
					                p_ssa_ctxt->bitmask_1 |= 
                                        ICF_SEC_REGISTRAR_ACTIVE;
					                p_line_data->sec_registrar_active =ICF_TRUE;

                                    /*Generate trace for sec. registrar active*/
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_SECONDARY_REGISTRAR_ACTIVE)
				                }
				                else/* inactive addr and prim. reg. same */
				                {
                                    /* mark prim. reg. active */
					                p_ssa_ctxt->bitmask_1 &= 
                                        ~ICF_SEC_REGISTRAR_ACTIVE;
					                p_line_data->sec_registrar_active =ICF_FALSE;

                                    /*Generate trace - prim. registrar active*/
                                    ICF_SSA_APP_TRACE(p_ssa_pdb,
                                        ICF_TRACE_PRIMARY_REGISTRAR_ACTIVE)
				                }/*end else inactive server and prim. reg same*/
			                }/* end else - inactive server addr used */
		                }/* end of - SUCCESS - fetch line data */
	                }/* end if - REGISTER method */
                }/* end else - request not routed through proxy */
        }

#ifdef ICF_DNS_LOOKUP_ENABLED
        /* code for DNS buffer starts here */	    
        /* Fix For SPR: 19666
           Do Not add the FQDN to IP mapping in dns buffer if the resolved IP
           address for FQDN is fetched from DNS Buffer.
           This is done as while adding the DNS entry is DNS buffer in
           function icf_ssa_dns_add_fqdn_to_ip_mapping when we try to extract
           the ttl value from destinfo we are encountring ABR as the ttl value
           is not present at the desired memory location. TTL value is only
           populated by stack when DNS query is initiated for resolving the
           domain name to DNS server.*/
        if((ICF_NULL != p_ssa_ctxt)
            && (p_ssa_ctxt->bitmask & ICF_SSA_DNS_FQDN_PRESENT)
            && !(p_ssa_ctxt->bitmask & ICF_DNS_RECORD_FETCHED))
	    {

                if((ICF_SUCCESS == ret_val) &&
                        (ICF_SUCCESS == icf_ssa_dns_add_fqdn_to_ip_mapping(&dns_info_to_store,
                            p_ssa_ctxt,
                            p_call_obj,
                            p_ecode,
                            (icf_uint8_t)ICF_DNS_MAP_CHANGE_QUERY_INVOCATION_SUCCESS)))
                {
                    /*Start SPR 18864: Added null check for SSA ctx */
                    if(ICF_NULL != p_ssa_ctxt)
                    {   
                        /* the address is updated reset the bitmask*/
                        p_ssa_ctxt->bitmask &= (~ICF_SSA_DNS_FQDN_PRESENT);
                    }  
                }
        }
            /* end of the DNS buffer code */
#endif

           /*Once response received for any request free memory allocated for
            *p_rem_addr_active and p_rem_addr_inactive, as once the dialog is established
            *these parameter are not required. This is also necessary as NULL check of these
            *parameters will indicate that indialog request will not be tried on sec addr and
            *also only top FQDN will be used of DNS response for remote Contact received,
            *In case no response is received i.e. this function is not called, both parameters
            *are freed from sdf_cbk_uaFreeApplicatonData before freeing icf_ssa_ctxt_st*/
           if (ICF_NULL != p_ssa_ctxt)
           {
               /*Initialize rem_server_address to 0 again*/
               p_ssa_ctxt->rem_servers_tried = 0;

               if (ICF_NULL != p_ssa_ctxt->p_rem_addr_active)
               {
                   ICF_MEMFREE(
                           p_glb_pdb,
                           p_ssa_ctxt->p_rem_addr_active,
                           ICF_MEM_COMMON,
                           p_ssa_pdb->p_ecode,
                           ret_val_memfree)
                } 

               if (ICF_NULL != p_ssa_ctxt->p_rem_addr_inactive)
                {
                    ICF_MEMFREE(
                           p_glb_pdb,
                           p_ssa_ctxt->p_rem_addr_inactive,
                           ICF_MEM_COMMON,
                           p_ssa_pdb->p_ecode,
                           ret_val_memfree)
                }
           }

            if (ICF_SUCCESS == ret_val)
            {
                icf_boolean_t    to_be_sent = ICF_TRUE;
                /*Message mapped to existing dialog/txn*/
                Sdf_ty_messageValidity      sdf_retval = Sdf_en_valid;
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
                /*Start SPR 18864 : Added null check for p_sdf_error*/
                if(Sdf_co_null != p_sdf_error)
                {
                    sip_getMessageType(p_temp_msg, &dMsgType, \
                        (SipError*)	&(p_sdf_error->stkErrCode));
                }
                if(ICF_NULL != p_ssa_pdb)
                {
                   p_ssa_pdb->dMsgType = dMsgType;
                }/*End SPR 18864 */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SSA PDB is null"));
                    return ICF_FAILURE;
                }
                if (ICF_NULL != p_ssa_ctxt)
                {
                    /* Moved below condition inside "if" to avoid
                       klocwork warning */
                    p_ssa_ctxt->bitmask_1 &= ~ICF_MSG_RECVD_OUT_OF_DIALOG;
                    if (SipMessageRequest == dMsgType)
                    {
                        if ((p_ssa_pdb->p_conn_table_entry != 
                                p_ssa_ctxt->p_conn_table_resp_entry) &&
                            (ICF_NULL != p_ssa_pdb->p_conn_table_entry))
                        {
                            /* CSR 1-7576668. In case of UDP, there will no conn table
                               entry. Hence,IPTK shall not try to update the connection
                               table entry in case of UDP.The NULL check in 
                               p_conn_table_entry signifies that the message is 
                               received on UDP.
						    */
                            icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, 
                                                 p_call_obj,
                                                 p_ssa_pdb->p_conn_table_entry);

                            p_ssa_pdb->p_conn_table_entry->ref_count++;
                            /*Start SPR 18864 */
                            if(ICF_NULL != p_ssa_ctxt->p_call_obj)
                            {
                               ret_val = icf_ssa_check_for_new_conn(p_ssa_pdb,
                                    p_ssa_ctxt,
                                    p_ssa_ctxt->p_call_obj);
                            }/*End SPR 18864*/   
                            p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask |=
                                ICF_CONN_TYPE_RESPONSE;
                            if ((p_ssa_pdb->p_conn_table_entry->prot_conn_bitmask & 
                                        ICF_TRANSPORT_TLS) &&
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
                        if((p_ssa_pdb->p_conn_table_entry != 
                                p_ssa_ctxt->p_conn_table_req_entry) &&
                           (ICF_NULL != p_ssa_pdb->p_conn_table_entry))
                        {
                            /* Fix for CSR 1-7576668 */
                            /* icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, 
                                    p_call_obj,
                                    p_ssa_pdb->p_conn_table_entry); */
                            /* Start SPR 18864 */
                            /* Update the connection table entry of ssa context
                             * only with the one in ssa pdb for any response
                             * received only when the value of
                             * conn_not_to_close is not TRUE. This indicate
                             * that the connection on which call setup is
                             * initiated can not be closed/replcaed untill call setup
                             * is successfull and we have not received any
                             * request on new connection.The replacing and
                             * closing of connection is handle by IPTK by
                             * abobe block. */
                            if((ICF_NULL != p_ssa_ctxt->p_call_obj) && 
                                    (ICF_NULL !=
                                     p_ssa_ctxt->p_conn_table_req_entry) &&
                                    (ICF_TRUE !=
                                     p_ssa_ctxt->p_conn_table_req_entry->conn_not_to_close))
                            {
                                ret_val = icf_ssa_check_for_new_conn(p_ssa_pdb,
                                        p_ssa_ctxt,
                                        p_ssa_ctxt->p_call_obj);
                            }   
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
                    /*Start SPR 18864: Added null check for ssa ctx in PDB */
                    if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                    { 
                       p_ssa_pdb->p_ssa_ctxt->p_conn_table_req_entry = 
                          p_ssa_ctxt->p_conn_table_req_entry;
                       p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry = 
                          p_ssa_ctxt->p_conn_table_resp_entry;
                    }/*End SPR 18864 */   
                }

                if ( SipMessageRequest == dMsgType)
                {
                    icf_boolean_t    is_mem_available = ICF_TRUE;
                    icf_boolean_t    is_sub_or_reg_req = ICF_FALSE;
                    SipHeader       *pHdr = Sdf_co_null;
                    SipOptions		dOptions = {0};
                    Sdf_st_overlapTransInfo   *pTransInfo = Sdf_co_null;
                    Sdf_st_transaction 			*pTransaction = Sdf_co_null;
                    Sdf_ty_u16bit    dRespCode = 500;
                    icf_boolean_t    is_invalid_method = ICF_FALSE;
                    icf_app_id_t     application_id = ICF_APP_ID_INVALID;
                    icf_ssa_ctxt_st           *p_temp_ssa_ctxt = ICF_NULL;

                    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                            p_call_obj->pAppData, ret_value);
                    if(ICF_FAILURE == ret_value)
                    {
                        ret_val = ICF_FAILURE;
                        ICF_FUNCTION_EXIT(p_glb_pdb)
                        return ret_val;
                    }
                    p_temp_ssa_ctxt = (icf_ssa_ctxt_st*)(p_call_obj->pAppData->pData);

                    /* Fix for CSR 1-8678911 */
                    /* Save the line id from mapped ctxt so that it can be used
                     * in subsequent dispatch function if self addr is 
                     * configured per line. If this is not done, then dispatch
                     * function will receive temp ssa ctxt with line_id = 65535
                     * and return failure without sending the response to peer.
                     */
                    if ((ICF_NULL != p_temp_ssa_ctxt) &&
                        (ICF_NULL != p_ssa_ctxt) &&
                        (ICF_INVALID_LINE_ID == p_temp_ssa_ctxt->line_id))
                    {
                        p_temp_ssa_ctxt->line_id = p_ssa_ctxt->line_id;
                    }

                    dOptions.dOption=\
                                     SIP_OPT_CLEN | SIP_OPT_FULLFORM;
                     /*SPR 18864:Added null check for UAS/UAC txn */
                    if((Sdf_co_null != p_call_obj->pUasTransaction) &&
                       (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg))
                    {
                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                    }
                    else if(Sdf_co_null != p_call_obj->pUacTransaction)
                    {
                        pMessage = p_call_obj->pUacTransaction->pSipMsg;
                    }/*End SPR 18864 */

#ifdef ICF_PORT_SYMBIAN
                    if ( (ICF_FAILURE == 
                                icf_port_is_mem_available(p_glb_pdb)) || 
                            (ICF_FAILURE ==
                             icf_port_is_timer_available(p_glb_pdb)))
                    {
                        is_mem_available = ICF_FALSE;
                    }
#endif /* ICF_PORT_SYMBIAN */

                    /* SPR 19999: Issue fixed, in case of CANCEL is received
                     * for an incoming INVITE then to send response for
                     * CANCEL, IPTK tries to create new connection as the
                     * connection information is not updated in the temporary
                     * SSA context. The connection information is updated here
                     */
					if(ICF_NULL != p_ssa_ctxt->p_conn_table_req_entry)
					{
							p_temp_ssa_ctxt->p_conn_table_req_entry =
									p_ssa_ctxt->p_conn_table_req_entry;
					}

					if(ICF_NULL != p_ssa_ctxt->p_conn_table_resp_entry)
					{
							p_temp_ssa_ctxt->p_conn_table_resp_entry =
									p_ssa_ctxt->p_conn_table_resp_entry;
					}

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
                    if((ICF_NULL ==  p_ssa_pdb->p_app_conf) &&
                       (ICF_NULL !=  p_ssa_ctxt))
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
                        /* Fix for CSR 1-6847773: SPR 19296 */
                        else if (ICF_TRUE == cc_context_cleared)
                        {
                            application_id = ICF_INVALID_APP_ID; 
                        }                        
                        else
                        {
                        	/*Default app id*/
                            application_id = 1;
                        }
                        /*SPR 18864 Added null check for glb pdb inside SSA
                         * PDB */
                        if((ICF_FALSE == cc_context_cleared) &&
                           (ICF_NULL != p_ssa_pdb->p_glb_pdb) &&
                           (ICF_FAILURE == icf_cfg_fetch_app_conf(p_ssa_pdb->p_glb_pdb,
                            application_id,
                            &(p_ssa_pdb->p_app_conf),
                            p_ssa_pdb->p_ecode)))
                        {
                        	ret_val = ICF_FAILURE;
                        }
                    }

                    /* Fix for CSR 1-6847773: SPR 19296 */
                    if (ICF_TRUE == cc_context_cleared)
                    {
                        icf_ssa_ctxt_st    *p_old_ssa_ctxt = p_ssa_ctxt;
                        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
                        /* Call the following function to clean the INVITE
                           transaction and the related call object.
                         */
                        if((Sdf_co_null != p_orig_call_obj) &&
                                (Sdf_co_fail == 
                                 sdf_ivk_uaRejectRequest(p_call_obj,
                                     p_orig_call_obj->pInitData,
                                     481, &pTransInfo, p_sdf_error)))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            if( pTransInfo != Sdf_co_null )
                            {
                                pMessage = pTransInfo->pSipMsg;
                            }
                            else if(Sdf_co_null != p_call_obj->pUasTransaction)
                            {
                                pMessage = 
                                    (p_call_obj)->pUasTransaction->pSipMsg;
                            }

                            if(sdf_ivk_uaGetTransactionForSipMsg(\
                                        (p_call_obj), pMessage, &pTransaction, \
                                        &pTransInfo, p_sdf_error) == Sdf_co_success)
                            {
                                if((Sdf_co_null == pTransInfo) &&
                                        (Sdf_co_null != pTransaction)) 
                                {
                                    pMessage = pTransaction->pSipMsg;
                                    sdf_ivk_uaFreeTransaction(pTransaction);
                                }
                                else if(Sdf_co_null != pTransInfo)
                                {
                                    pMessage = pTransInfo->pSipMsg;
                                    sdf_ivk_uaFreeOverlapTransInfo( \
                                            pTransInfo);
                                }
                            }

                             /* Strore the Request / Response connection entry in the 
                              * temporary ssa context so that while sending the reject
                              * request on tcp/tls, the connection entry is updated
                              * in the ssa context of new call object as the same will
                              * be used to extract connection info while sending error
                              * response to network. 
                              */ 

                             if(ICF_NULL != p_ssa_ctxt->p_conn_table_req_entry)
                             {
                                 p_temp_ssa_ctxt->p_conn_table_req_entry =
                                     p_ssa_ctxt->p_conn_table_req_entry;
                             }

                             if(ICF_NULL != p_ssa_ctxt->p_conn_table_resp_entry)
                             {
                                 p_temp_ssa_ctxt->p_conn_table_resp_entry =
                                     p_ssa_ctxt->p_conn_table_resp_entry;
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
                        }
                        p_ssa_pdb->p_ssa_ctxt = p_old_ssa_ctxt;
                        icf_ssa_process_forceful_call_clear(p_ssa_pdb);
                        p_ssa_pdb->p_ssa_ctxt = ICF_NULL;
                        return(ICF_SUCCESS);
                    } /* if (ICF_TRUE == cc_context_cleared) */

                    /* Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18867)*/
                    /* Fix for SPR 18867 : We will perform the method validity check only
                       if no failure response has been sent to the peer so far.
                       
                       Scenarios:
                      
                       <------------ INVITE
                       ------------> 405
                       <------------ ACK

                       <------------ INVITE
                       ------------> 405
                       <------------ CANCEL
                       ------------> 481
                       <------------ ACK

                       At this point of time, we have already performed the method 
                       validity check and have sent a 405 response to the peer. On 
                       receipt of an 'ACK' or any other request like a 'CANCEL' or 
                       a 'BYE' we must not perform the same check again. Instead
                       such messages should be responded with a 481 response as 
                       no transaction has been created so far or they should be ignored.
                    */   

                    if ((ICF_NULL != p_ssa_ctxt) &&
                        (ICF_NULL == p_ssa_ctxt->failure_bitmask) &&
                        (ICF_FAILURE == icf_ssa_validate_basic_method(p_ssa_pdb,                    
                                        pMessage,
                                        &is_invalid_method)))
					{
						ret_val = ICF_FAILURE;
					}
                    else if((ICF_TRUE == is_invalid_method) &&
                            (ICF_NULL != p_ssa_pdb->p_ssa_ctxt))
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
                        if((Sdf_co_null != p_orig_call_obj) &&
                           (Sdf_co_fail == 
                                sdf_ivk_uaRejectRequest(p_call_obj,
                                    p_orig_call_obj->pInitData,
                                    dRespCode,&pTransInfo,p_sdf_error)))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            if(sdf_ivk_uaGetTransactionForSipMsg(\
                                        (p_call_obj), pMessage, &pTransaction, \
                                        &pTransInfo, p_sdf_error) == Sdf_co_success)
                            {
                                if((pTransInfo == Sdf_co_null) &&
                                   (Sdf_co_null != pTransaction))    
                                {
                                    pMessage = pTransaction->pSipMsg;
                                    sdf_ivk_uaFreeTransaction(pTransaction);
                                }
                                else if(Sdf_co_null != pTransInfo)
                                {
                                    pMessage = pTransInfo->pSipMsg;
                                    sdf_ivk_uaFreeOverlapTransInfo( \
                                            pTransInfo);
                                }
                            }
                            if((ICF_FALSE == is_mem_available) &&
                               (Sdf_co_null != p_sdf_error))
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

/*  CSR 1-5385208 Merge */
#ifdef ICF_TRACE_ENABLE
                             if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                              {
                                  if((ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)&&
                                     (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt) &&
                                     (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_rgm_ctxt))
                                   {
                                       if(ICF_FAILURE == icf_ssa_make_criteria_data_from_msg(
                                                            p_ssa_pdb,
                                                            p_call_obj,
                                                            p_sdf_error))
                                        {
                                            ret_val = ICF_FAILURE;
                                        }
                                   }
                              }

                             p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
                             ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)

                             /* This bit is set here to indicate that trace for
                              * this pdu is already sent from here and should be
                              * sent again from icf_ssa_process_nw_message,when
                              * control returns to it. */ 
                             p_ssa_pdb->common_bitmask |= ICF_SSA_TRACE_REQUIRED;
#endif
                             /*Insert P-Access-nw-info header for indialog failure
                              *responses*/
                             if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                             {   
                               if((ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt)||
                                  (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_ctxt))
                               {
                                   /*SPR18526 Added precautionary null check
                                     for p_method_temp*/
                                  if (Sdf_co_null != p_method_temp)
                                  {   
                                    if(0 != icf_port_strcmp((icf_uint8_t
                                                    *)p_method_temp,
                                                     (icf_uint8_t *)"CANCEL")) 
                                     {   
                                          if(ICF_FAILURE == 
                                                 icf_ssa_form_and_insert_access_nw_info_hdr(
                                                  p_ssa_pdb,pMessage))
                                          {
                                                ret_val = ICF_FAILURE ;
                                          }
                                      }
                                  }  
                               }
                             } 

                             /* Strore the Request / Response connection entry in the 
                              * temporary ssa context so that while sending the reject
                              * request on tcp/tls, the connection entry is updated
                              * in the ssa context of new call object as the same will
                              * be used to extract connection info while sending error
                              * response to network. 
                              */ 
                             if(ICF_NULL != p_ssa_ctxt->p_conn_table_req_entry)
                             {
                                 p_temp_ssa_ctxt->p_conn_table_req_entry =
                                     p_ssa_ctxt->p_conn_table_req_entry;
                             }

                             if(ICF_NULL != p_ssa_ctxt->p_conn_table_resp_entry)
                             {
                                 p_temp_ssa_ctxt->p_conn_table_resp_entry =
                                     p_ssa_ctxt->p_conn_table_resp_entry;
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
                /*SPR 18864 : Added null check for the SSA ctx */
                if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                {
                     icf_port_memcpy(p_ssa_ctxt->destination_ip,\
                            (icf_uint8_t*)p_ssa_pdb->p_ssa_ctxt->destination_ip,\
                            sizeof(p_ssa_ctxt->destination_ip));
                     p_ssa_ctxt->destination_port = 
                         p_ssa_pdb->p_ssa_ctxt->destination_port;
                }     
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
                    /*SPR 18864 Added null checks for the UAC/UAS txn*/
                    if((Sdf_co_null != p_call_obj->pUacTransaction) &&
                       (Sdf_co_null != p_call_obj->pUacTransaction->pSipMsg))
                    {
                        pMessage = p_call_obj->pUacTransaction->pSipMsg;
                    }    
                    else if(Sdf_co_null != p_call_obj->pUasTransaction)
                    {    
                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                    }    
                }
                if(Sdf_co_null != p_sdf_error)
                {    
                   ret_val = icf_ssa_get_params_from_Via(pMessage,\
                        p_ssa_pdb,dMsgType,(SipError*)	&(p_sdf_error->\
                            stkErrCode));
                }  
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
                        /* CSR 1-6212448: Send Cancel received trace to application */
                        ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_CANCEL_RECVD)                        
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
                    /*SPR 19992 We do not want to check the content-encoding,
                     * content-language and content-type for the CANCEL because
                     * anyway peer has terminated the INVITE txn, not sending
                     * 415 here shall lead to sending 200 for CANCEL further 
                     * leading to termination of the INVITE txn */

                    if((0 != Sdf_mc_strcasecmp(p_method,"INVITE")) &&
                       (0 != Sdf_mc_strcasecmp(p_method,"CANCEL")) &&
                       (Sdf_co_null != p_orig_call_obj) &&
                       (Sdf_en_invalid == sdf_ivk_uaIsSupportedMsgBody(p_call_obj,
                                     p_orig_call_obj->pInitData,p_sdf_error)) && 
                       (dMsgType == SipMessageRequest))
                    {
                        /* sdf_ivk_uaIsSupportedMsgBody function has called 
                         * sdf_ivk_uaRejectRequest so we will directly send the
                         * message 
                         */
                        SipOptions		dOptions;
                        SipMessage					*pMessage = Sdf_co_null;
                        Sdf_st_transaction 			*pTransaction = Sdf_co_null;
                        Sdf_st_overlapTransInfo 	*pOverlapTransaction = Sdf_co_null;
                        icf_ssa_ctxt_st           *p_temp_ssa_ctxt = ICF_NULL;
                        /*Start SPR 19189 */
                        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                            p_call_obj->pAppData, ret_value)
                        if(ICF_FAILURE == ret_value)
                        {
                            ret_val = ICF_FAILURE;
                            ICF_FUNCTION_EXIT(p_glb_pdb)
                            return ret_val;
                        }/*End SPR 19189*/
                        p_temp_ssa_ctxt = (icf_ssa_ctxt_st*)(p_call_obj->pAppData->pData);

                        dOptions.dOption=\
                                         SIP_OPT_CLEN | SIP_OPT_FULLFORM;
                        /*SPR 18864: Added null checks for UAS/UAC txn */
                        if((Sdf_co_null != p_call_obj->pUasTransaction) && 
                           (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg))
                        {
                            pMessage = p_call_obj->pUasTransaction->pSipMsg;
                        }    
                        else if(Sdf_co_null != p_call_obj->pUacTransaction)
                        {    
                            pMessage = p_call_obj->pUacTransaction->pSipMsg;
                        }    

                         /* SPR 18832: Strore the Request / Response connection entry in the 
                          * temporary ssa context, 
                          */ 
                         if(ICF_NULL != p_ssa_ctxt->p_conn_table_req_entry)
                         {
                            p_temp_ssa_ctxt->p_conn_table_req_entry =
                                  p_ssa_ctxt->p_conn_table_req_entry;
                         }

                         if(ICF_NULL != p_ssa_ctxt->p_conn_table_resp_entry)
                         {
                            p_temp_ssa_ctxt->p_conn_table_resp_entry =
                                  p_ssa_ctxt->p_conn_table_resp_entry;
                         }
                       
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
                              /*SPR 18864:Added null check for the pTransaction */
                            if((Sdf_co_null == pOverlapTransaction) &&
                               (Sdf_co_null != pTransaction))
                            {
                                pMessage = pTransaction->pSipMsg;
                                sdf_ivk_uaFreeTransaction(pTransaction);
                            }
                            else if(Sdf_co_null != pOverlapTransaction)
                            {
                                pMessage = pOverlapTransaction->pSipMsg;
                                sdf_ivk_uaFreeOverlapTransInfo( \
                                        pOverlapTransaction);
                            }

/*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
                            ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, 
                              p_ssa_pdb->p_glb_pdb->p_stats_info, ret_value)
                            if(ICF_FAILURE == ret_value)
                            {
                                 ret_val = ICF_FAILURE;
                                 ICF_FUNCTION_EXIT(p_glb_pdb)
                                 return ret_val;
                            }
                            if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"CANCEL")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                    stat_requests.stat_inbound_req.stats_CANCEL)
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"UPDATE")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_UPDATE)
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"BYE")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_BYE)
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"PRACK")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_PRACK)
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"REFER")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                    stat_requests.stat_inbound_req.stats_REFER)
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"NOTIFY")== 0)
                            {
                                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                     stat_requests.stat_inbound_req.stats_NOTIFY);
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"OPTIONS")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                    stat_requests.stat_inbound_req.stats_indlg_OPTION);
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"INFO")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                    stat_requests.stat_inbound_req.stats_INFO);
                            }
                            if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"ACK")== 0)
                            {
                                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                    stat_requests.stat_inbound_req.stats_ACK);
                            } 
                            else
                            {
                                icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)p_method_temp,415,1);
                            }
#endif

/*  CSR 1-5385208 Merge */
#ifdef ICF_TRACE_ENABLE
                        if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                         {
                             if((ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)&&
                                (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt) &&
                                (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_rgm_ctxt))
                              {
                                  if(ICF_FAILURE == icf_ssa_make_criteria_data_from_msg(
                                                        p_ssa_pdb,
                                                        p_call_obj,
                                                        p_sdf_error))
                                   {
                                       ret_val = ICF_FAILURE;
                                   }
                              }
                         }
                        p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
                        ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)

                        /* This bit is set here to indicate that trace for
                         * this pdu is already sent from here and should be
                         * sent again from icf_ssa_process_nw_message,when
                         * control returns to it. */
                        p_ssa_pdb->common_bitmask |= ICF_SSA_TRACE_REQUIRED;
#endif
                            ICF_PRINT(((icf_uint8_t *)"[SSA:] Validation of Content-Type, Content-Encoding"
                                        " or Content-Language failed\n"));

                            /*To insert P-Access-Network-Info header in in-dialog
                              response 415,call context check is added and sic context
                              check is added to insert header in out-of-dialog response 
                              as for response for notify.*/

                        if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                        {   
                            if((ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt)||
                                 (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_ctxt))
                            {
                                  /*SPR18526" Used p_method_temp instead of
                                   * p_method to avoid FMR*/
                                if (Sdf_co_null != p_method_temp)
                                {    
                                  if(0 != icf_port_strcmp((icf_uint8_t*)p_method_temp,
                                                            (icf_uint8_t*)"CANCEL")) 
                                  {   
                                      if(ICF_FAILURE == 
                                             icf_ssa_form_and_insert_access_nw_info_hdr(
                                                p_ssa_pdb,pMessage))
                                      {
                                        ret_val = ICF_FAILURE ;
                                      }
                                  }
                                }  
                            }
                        }   
                        if(ICF_SUCCESS == ret_val)
                            {
                                if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                        (p_call_obj),
                                        pMessage,
                                        &dOptions,p_sdf_error))
                                {
                                    ret_val = ICF_FAILURE;
                                }
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
                        /*ICF_SSA_TRACE(*/

                        /*p_ssa_pdb,ICF_TRACE_UPDATE_CALL_DET_STATUS)*/

                        Sdf_st_callObject           *p_tmp_call_obj = Sdf_co_null;

/*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
                        Sdf_ty_u16bit               resp_code = 0;
#endif
                        /* commented for ETSI conformance case 
                        * SIP_CC_TE_CR_V_005
                        * SPR 18580: For handlign the conformance case, the require
                        * header validation is added in the callback function of 
                        * sdf_cbk_uaCallTerminateRequest.
                        */
                         p_event_ctxt->dSdfDisabledChecks |= 
                               Sdf_mc_disableRequireCheck;

                         if (Sdf_en_sameCallLeg ==  call_leg_match)
                         {
                              p_event_ctxt->pData = p_ssa_ctxt;
                         }
                         /*SPR 18864: added null checks for orig call obj */
                         if((Sdf_co_null != p_orig_call_obj) &&
                            (Sdf_co_null != p_orig_call_obj->pCallInfo))
                         {
                            /* Fix for CSR No: 1-5230713 */
                            
                            /* SPR 20061 : An additional NULL check for
                               p_call_obj->pUasTransaction->pSipMsg has been
                               put to cater to the following scenario:

                               <---- INVITE
                               ----> 200 OK
                               <---- ACK
                               
                               Incoming call established.

                               <---- ReINVITE from peer
                               ----> 100 Trying

                               ----> REFER
                               <---- 202 Accepted

                               Now on receipt of 202 response, the call state of the
                               original call object is Sdf_en_reInviteprovRespSent.
                               A new call object is created whose pUasTransaction
                               is non NULL. However pUasTransaction->pSipMsg of the
                               new call object is NULL.
                               
                               As a result, in the subsequent code leg, the 
                               API sdf_ivk_uaGetMethodFromSipMessage returns failure
                               and the 202 response is ignored and no callback
                               function is issued. To prevent the same, an additional 
                               check of p_call_obj->pUasTransaction->pSipMsg has been put.

                               Now the flow will not enter the subsequent code leg and
                               API sdf_ivk_uaUpdateCallDetails will be invoked, thus 
                               issuing  the relevant callback where the 202 response
                               will be rightly processed.
                            */

                            if(((Sdf_en_reInviteReceived ==
                                     p_orig_call_obj->pCallInfo->dState) ||
                                (Sdf_en_reInviteprovRespSent ==
                                     p_orig_call_obj->pCallInfo->dState)) &&
                                (Sdf_co_null != p_call_obj->pUasTransaction) &&
                                (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg))
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
                                   /*SPR 18864: Added null checks for pData*/
                                    if((Sdf_co_null != p_event_ctxt) &&
                                       (Sdf_co_null != p_event_ctxt->pData))
                                    {   
                                        ((icf_ssa_ctxt_st*)p_event_ctxt->pData)->\
                                         p_ssa_pdb = p_ssa_pdb;
                                    }    
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
                         }/*End SPR 18864 check for callinfo presence*/
                        sdf_retval = sdf_ivk_uaUpdateCallDetails(\
                                &p_orig_call_obj, p_call_obj, \
                                p_event_ctxt, p_sdf_error);


                        switch(sdf_retval){
                            case Sdf_en_discarded:
                                /*TO DO : Failure Handling to be added*/

/*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
                                /*Start SPR 19189*/
                                ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, 
                                  p_ssa_pdb->p_glb_pdb->p_stats_info, ret_value)
                                if(ICF_FAILURE == ret_value)
                                {
                                    ret_val = ICF_FAILURE;
                                    ICF_FUNCTION_EXIT(p_glb_pdb)
                                    return ret_val;
                                }/*End SPR 19189*/
                                if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"ACK")== 0)
                                {
                                    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                        stat_requests.stat_inbound_req.stats_ACK);
                                }
#endif
                                /* SPR 20104: The case statement for discarded message was having
                                 * no handling. In case a message is discarded then the transaction
                                 * should be freed here.
                                 * Scenario: 
								 *      INVITE <--------
                                 *      200 OK --------->
                                 *      INVITE (without VIA branch) - message discarded
                                 *      ACK <------------
                                 */
                                if (ICF_NULL != p_call_obj->pUasTransaction)
                                {    
                                    icf_ssa_free_sip_txn_data_on_failure(
                                             p_call_obj->pUasTransaction->pTxnKey);
                                }
                                break;
                            case Sdf_en_valid:
                                {
                                    /*Add an info trace here*/
#ifdef  ICF_TRACE_ENABLE
                                  /*SPR 18864:Added null check for UAS/UAC txn */  
                				  if((Sdf_co_null !=
                                               p_call_obj->pUacTransaction) &&
                                     (Sdf_co_null !=
                                               p_call_obj->pUacTransaction->pSipMsg))
                                   {
                                      p_temp_msg = 
                                        p_call_obj->pUacTransaction->pSipMsg;
                                   }
                                  else if(Sdf_co_null !=
                                          p_call_obj->pUasTransaction)
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
                                }
                                break;
                            case Sdf_en_failed:
                            case Sdf_en_invalid:
                                if (ICF_TRUE == via_handling)
                                {
                                    /*  call the force ful call clear */
                                    /* Assigning the mapped ssa context to ssa pdb so
                                       that the following function does cleanup of the
                                       correct call object.
                                     */
                                    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
                                    /* Call the following function to clean the INVITE
                                       transaction and the related call object.
                                     */
                                    if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
                                    {
                                        /* Trigger generic call clear towards CC */
                                        if(ICF_NULL != p_ssa_pdb->p_internal_msg)
                                        {
                                            p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                                            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                                ICF_FORCEFUL_CALL_CLEAR_IND;
                                            p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb =
                                                p_ssa_pdb->p_glb_pdb;
                                            p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx =
                                                p_ssa_ctxt->p_call_ctxt;
                                        }
                                        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                                        p_ssa_ctxt->p_call_ctxt->signalling_ongoing =
                                            ICF_INT_MODULE_INVALID;
                                        ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                                                ICF_SSA_INTER_MODULAR_MSG)
                                    }
                                    via_handling = ICF_FALSE;
                                }
                                else
                                {
                                p_tmp_call_obj = p_call_obj;
/*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
                                /*Start SPR 19189*/
                                ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, 
                                   p_ssa_pdb->p_glb_pdb->p_stats_info, ret_value)
                                if(ICF_FAILURE == ret_value)
                                {
                                    ret_val = ICF_FAILURE;
                                    ICF_FUNCTION_EXIT(p_glb_pdb)
                                    return ret_val;
                                }/*End SPR 19189*/
                                if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"ACK")== 0)
                                {
                                    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                        stat_requests.stat_inbound_req.stats_ACK);
                                }
#endif
                                /*SPR 18864: Added null check for the
                                 * p_sdf_error*/ 
                                if((Sdf_co_null != p_sdf_error) &&
                                   ((p_sdf_error->errCode==Sdf_en_badExtensionsError) || \
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
                                    Sdf_ty_state                trans_state=Sdf_en_transactionReceived;
                                    Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;
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
                                    /*Fix for SPR:18837 : CSR-1-6188454*/
                                    /*Fix done for the scenario when incoming
                                     * update received with Session Expire
                                     * header having value less than Min-Se
                                     * header and "timer" is present in
                                     * supported header.In this case we are
                                     * not able send the 422 response on
                                     * network as pMessage value is not
                                     * populated correctly in the following
                                     * code.As a fix code is added to populate
                                     * the pMessage in case of overlap
                                     * transaction.*/
                                    if (p_sdf_error->errCode==Sdf_en_lowSessionTimerIntervalError)
                                    {
                                       /*For error response Sdf_en_lowSessionTimerIntervalError 
                                         set the value of transaction state as idle as stack has
                                         rejected the request*/
                                        trans_state = Sdf_en_idle;
                                    }
                                    if (ICF_SUCCESS == icf_ssa_fetch_matching_overlap_txn(
                                               p_ssa_pdb,Sdf_en_update,trans_state,
                                               p_tmp_call_obj,&p_ovtxn))
                                    {
                                         pMessage = p_ovtxn->pSipMsg;
                                    }
                                    else if((Sdf_co_null != p_tmp_call_obj->pUasTransaction)&&
                                       (Sdf_co_null != p_tmp_call_obj->pUasTransaction->pSipMsg))
                                    {
                                        pMessage = p_tmp_call_obj->pUasTransaction->pSipMsg;
                                    }
                                    else if(Sdf_co_null != p_tmp_call_obj->pUacTransaction)
                                    {
                                        pMessage = p_tmp_call_obj->pUacTransaction->pSipMsg;
                                    }

                                    if(sdf_ivk_uaGetTransactionForSipMsg(\
                                                p_tmp_call_obj, pMessage, &pTransaction, \
                                                &pOverlapTransaction, p_sdf_error) == Sdf_co_fail)
                                    {
                                        ret_val = ICF_FAILURE ;
                                        break;
                                    }
                                    else if((pOverlapTransaction ==
                                                Sdf_co_null) &&
                                            (Sdf_co_null != pTransaction))
                                    {
                                        pMessage = pTransaction->pSipMsg;
                                        sdf_ivk_uaFreeTransaction(pTransaction);
                                    }
                                    else if(Sdf_co_null !=
                                            pOverlapTransaction)
                                    {
                                        pMessage = pOverlapTransaction->pSipMsg;
                                        sdf_ivk_uaFreeOverlapTransInfo( \
                                                pOverlapTransaction);
                                    }
  /*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
                                    if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                                                           pMessage, &resp_code,
                                                           p_sdf_error))
                                    {
                                        ret_val = Sdf_co_fail;
                                        break;
                                    }
                                    if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"CANCEL")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                             stat_requests.stat_inbound_req.stats_CANCEL)
                                    }
                                    else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"UPDATE")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_UPDATE)
                                    }
                                    else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"BYE")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_BYE)
                                    }
                                    else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"PRACK")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_PRACK)
                                    }
                                    else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"REFER")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_REFER)
                                    }
                                    else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"NOTIFY")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_NOTIFY);
                                    }
                                    else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"OPTIONS")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_indlg_OPTION);
                                    }
                                    else if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"INFO")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_INFO);
                                    }
                                    if(icf_port_strcmp((icf_uint8_t*)p_method_temp,(icf_uint8_t*)"INVITE")== 0)
                                    {
                                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                            stat_requests.stat_inbound_req.stats_INVITE)
                                        icf_ssa_invite_resp_stats(p_ssa_pdb,resp_code,1);
                                    }
                                    else
                                    {
                                        icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)p_method_temp,resp_code,1);
                                    }


#endif

/*  CSR 1-5383208 Merge */
#ifdef ICF_TRACE_ENABLE
                                    if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                                     {
                                         if((ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)&&
                                            (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt) &&
                                            (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_rgm_ctxt))
                                          {
                                              if(ICF_FAILURE == icf_ssa_make_criteria_data_from_msg(
                                                                    p_ssa_pdb,
                                                                    p_call_obj,
                                                                    p_sdf_error))
                                               {
                                                   ret_val = ICF_FAILURE;
                                               }
                                          }
                                     }
                                    p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
                                    ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)

                                    /* This bit is set here to indicate that trace for
                                     * this pdu is already sent from here and should be
                                     * sent again from icf_ssa_process_nw_message,when
                                     * control returns to it. */
                                    p_ssa_pdb->common_bitmask |= ICF_SSA_TRACE_REQUIRED;
#endif
                             /*Insert P-Access-nw-info header for indialog failure
                              *responses other than responses for method CANCEL*/
                             /* SOAK FIX:SPR 18450 : Method type is compare with 
                              * p_method_temp which we have strdup previously.
                              * This is done to prevent the FMR in a scenario 
                              * in which p_method is freed by UATK */       
                             if(0 != icf_port_strcmp(
                                         (icf_uint8_t *)p_method_temp,
                                                       (icf_uint8_t *)"CANCEL")) 
                                  {   
                                      if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt) 
                                      {   
                                          if((ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt)||
                                             (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_ctxt))
                                          {
                                            if(ICF_FAILURE == 
                                              icf_ssa_form_and_insert_access_nw_info_hdr(
                                                p_ssa_pdb,pMessage))
                                            {
                                               ret_val = ICF_FAILURE ;
                                            }
                                          }  
                                      }
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
                                }
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
            /*  Fix for CSR 1-7605218 */
            if (ICF_NULL != p_sip_msg)
            {
                if (SipFail == sip_getReqLineFromSipReqMsg(
                            p_sip_msg,
                            &p_req_line,
                            (SipError*)&(p_sdf_error->stkErrCode)))
                {
                    ret_val = ICF_FAILURE;
                }
                else if (SipFail == sip_getAddrSpecFromReqLine(
                            p_req_line, &p_req_addr_spec, 
                            (SipError*) &(p_sdf_error->stkErrCode)))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    icf_ssa_populate_address_from_sip_addr_spec(p_ssa_pdb,
                            p_req_addr_spec,
                            &req_uri);
                    sip_freeSipAddrSpec(p_req_addr_spec);
                    sip_freeSipReqLine(p_req_line);
                    icf_port_memcpy(&(p_ssa_pdb->p_ssa_ctxt->req_uri),
                            &req_uri,
                            ICF_PORT_SIZEOF(icf_address_st));
                }
            }            
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
                /*  Previously This function returns failure from here
                    Due to which the common info accessed above was 
                    not getting freed Now by setting this retval failure 
                    the commmon info would get freed from the next leg
                    and will return failure */
                /*  FIX for spr 18472 */
                ret_val= ICF_FAILURE;
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
                Sdf_st_error         sdf_error = {0,0,0,{0}};
                icf_app_id_t     app_id = ICF_APP_ID_INVALID;
                icf_boolean_t    send_failure_resp = ICF_FALSE;
                icf_boolean_t    rej_request_called = ICF_FALSE;
                icf_uint8_t     methodName[100] = {0};
                en_SipMessageType   dMsgType = SipMessageAny;
                Sdf_ty_s8bit *p_method = Sdf_co_null;
                   /*SPR 18864 */
                if((Sdf_co_null != p_event_ctxt) &&
                   (Sdf_co_null != p_event_ctxt->pData))  
                {
                   p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_event_ctxt->pData);
                }
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Event Ctx or pData is null"));
                    return ICF_FAILURE;
                } /*End SPR 18864*/
                ICF_SSA_SET_COMMON_BITMASK(
                        p_ssa_pdb->common_bitmask,
                        ICF_SSA_MSG_RECVD_OUT_OF_DIALOG)
                p_ssa_ctxt->bitmask_1 |= ICF_MSG_RECVD_OUT_OF_DIALOG;               
              
                p_glb_pdb->p_call_ctx = p_ssa_ctxt->p_call_ctxt;
                p_ssa_pdb->p_app_conf = ICF_NULL;
#ifdef ICF_NAT_RPORT_SUPPORT
                 /*SPR 18864 Added null check for UAS txn */
                if((Sdf_co_null != p_call_obj->pUasTransaction) &&
                   (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg))
                {
                    pMessage = p_call_obj->pUasTransaction->pSipMsg;
                }
                if(Sdf_co_null != p_sdf_error)
                {
                   ret_val = icf_ssa_get_params_from_Via(pMessage,\
                        p_ssa_pdb,SipMessageRequest,(SipError*)	&(p_sdf_error->\
                            stkErrCode));
                }   
#endif
                /*SPR 18864 Added null check for SSA ctx in PDB*/
                if ((ICF_NULL != p_ssa_pdb->p_conn_table_entry) &&
                    (ICF_NULL != p_ssa_pdb->p_ssa_ctxt))
                {
                    ICF_PRINT(((icf_uint8_t *)"Updating connection table entry for this ssa context\n"));
                    icf_ssa_fill_conn_info_from_Via(p_ssa_pdb,
                            p_call_obj,
                            p_ssa_pdb->p_conn_table_entry);
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
                        /*SPR 18864:added null check for UAS txn */
                        if((Sdf_co_null != p_call_obj->pUasTransaction) &&
                           (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg))
                        {
                            pMessage = p_call_obj->pUasTransaction->pSipMsg;
                        }    
                        if (icf_ssa_is_alias_present_in_Via(pMessage))
                        {
                            ICF_PRINT(((icf_uint8_t *)"Alias header is present"));
                            if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                            {
                               p_ssa_pdb->p_ssa_ctxt->bitmask_1 |= ICF_ALIAS_HEADER_PRESENT;
                            }   
                            if (0 != ICF_PURGE_TIMER_DURATION)
                            {   
                                p_ssa_pdb->p_conn_table_entry->is_persistent = ICF_TRUE;
                            }    
                        }
                    }
#endif
                    p_ssa_pdb->p_conn_table_entry->ref_count++;
                    p_ssa_pdb->p_conn_table_entry->timer_count = 0;
                    ICF_SSA_PRINT_CONN_DB(p_ssa_pdb->p_conn_table_entry)
                }

                if((Sdf_co_null != p_call_obj->pUasTransaction) &&
                   (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(
                            p_call_obj->pUasTransaction->pSipMsg,
                            &p_method,p_sdf_error)))

                {
                    ret_val = ICF_FAILURE;
                }
                /* Use filter criteria to determine handler application, fetch app conf 
                   accordingly */
                if((Sdf_co_null != p_call_obj->pUasTransaction)&&
                   (ICF_FAILURE == icf_ssa_map_req_to_app_filter (p_ssa_pdb,
                            p_call_obj->pUasTransaction->pSipMsg,&app_id,
                            p_ecode)))
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
                    /*SPR 18864 */
                    if(ICF_NULL != p_ssa_ctxt)
                    {
                       p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_405;
                    }   
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
                /* CSR 1-7586154 : SPR-19920*/
                /* get the config data pointer from DBM */
                else if (ICF_FAILURE ==
                                icf_dbm_get_module_glb_data(
                                    p_ssa_pdb->p_glb_pdb,
                                   (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                   (icf_void_t *)&(p_ssa_pdb->p_glb_cfg_data),
                                   p_ssa_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                /* If Application has set the bitmask for strict parsing then
                 * validate the Call-ID header in incoming message.
                 */
                else if((ICF_CALL_CONFIG_ENABLE_STRICT_PARSING ==
                         (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING &
                           p_ssa_pdb->p_glb_cfg_data->default_settings)) &&
                       (Sdf_co_null != p_call_obj->pUasTransaction) &&
                       (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg) &&
                       (ICF_FAILURE == icf_ssa_validate_incoming_request(
                                           p_ssa_pdb,
                                           p_call_obj->pUasTransaction->pSipMsg)))
                {
                    send_failure_resp = ICF_TRUE;
                    
                    if(ICF_NULL != p_ssa_ctxt)
                    {   
                      p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_400;
                    }
                }
                /* Break the else if flow here for sending failure response 
                  in case of handler application not found */
                if(ICF_FAILURE == ret_val)
                {
                }


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

                if((ICF_FALSE == send_failure_resp) &&
                    (Sdf_co_null != p_call_obj->pUasTransaction))
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
                    Sdf_st_error                SdfError = {0,0,0,{0}};
                    /*SPR 18864 Added null checks for UAS/UAC txn */
                    if((Sdf_co_null != p_call_obj->pUasTransaction) &&
                       (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg))
                    {
                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                    }
                    else if(Sdf_co_null != p_call_obj->pUacTransaction)
                    {
                        pMessage = p_call_obj->pUacTransaction->pSipMsg;
                    }
                    /*SPR 18864 Added null check for p_sdf_error */
                    if ((ICF_TRUE == rej_request_called) ||
                        ((Sdf_co_null != p_sdf_error) &&
                         ((p_sdf_error->errCode == Sdf_en_badExtensionsError) ||
                         (p_sdf_error->errCode == Sdf_en_unknownMethodError) ||
                         (p_sdf_error->errCode == Sdf_en_noCallExists) ||
                         (p_sdf_error->errCode ==
                             Sdf_en_lowSessionTimerIntervalError))))
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
                            /*SPR 18864 Added null checks for pTransaction*/
                            if((Sdf_co_null == pOverlapTransaction) &&
                               (Sdf_co_null != pTransaction))
                            {
                                pMessage = pTransaction->pSipMsg;
                                sdf_ivk_uaFreeTransaction(pTransaction);
                            }
                            else if(Sdf_co_null != pOverlapTransaction)
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

/*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
                        if(ICF_FAILURE != ret_val)
                        {
                            if (Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                                                   pMessage, &resp_code,
                                                   &SdfError))
                            {
                                ret_val = Sdf_co_fail;
                            }
                        }
#endif
                    }
                    else if(ICF_NULL != p_ssa_ctxt)
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
                                    (ICF_SSA_FAILURE_RESP_CODE_415
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb, ICF_FALSE)
                            resp_code = 415;
ICF_CHECK_TUNNEL_MODE_END
                        }
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_488
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb, ICF_FALSE)
                            resp_code = 488;
ICF_CHECK_TUNNEL_MODE_END
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
                        else if((ICF_NULL != 
                                    (ICF_SSA_FAILURE_RESP_CODE_400
                                     &(p_ssa_ctxt->failure_bitmask))))
                        {
                            resp_code = 400;
                        }
                        else if(Sdf_co_null != p_sdf_error) 
                        {
                            /*Null check for call context added to prevent crash in 
                              basic call.*/
                            if ( (p_sdf_error->errCode == Sdf_en_cseqValueExceeded) 
#ifdef SDF_SESSION_TIMER
#ifdef ICF_SESSION_TIMER
                                || ((ICF_NULL != p_ssa_ctxt->p_call_ctxt) && 
                                    ((ICF_TRUE == p_ssa_ctxt->p_call_ctxt->session_timer_enable) &&
                                    ((p_sdf_error->errCode == Sdf_en_invalidMinSe) ||
                                    (p_sdf_error->errCode == Sdf_en_lowsessionTimerError))))
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
                        /*Start SPR 18864 Added null check for fetching SSA
                         * ctx*/
                        if((Sdf_co_null != p_call_obj) &&
                           (Sdf_co_null != p_call_obj->pAppData))
                        {
                            p_call_obj->pAppData->pData = 
                               (Sdf_ty_pvoid *)p_ssa_ctxt;
                        }
                        else
                        {
                            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Call obj or pAppData is null"));
                            return ICF_FAILURE;
                        }/*End SPR 18864 */

                        if (Sdf_co_fail == sdf_ivk_uaRejectRequest(p_call_obj,p_call_obj->pInitData,
                                    (icf_uint16_t)resp_code,&pOverlapTransaction,p_sdf_error))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        if( pOverlapTransaction != Sdf_co_null )
                        {
                            pMessage = pOverlapTransaction->pSipMsg;
                        }
                        else if(Sdf_co_null != p_call_obj->pUasTransaction)
                        {
                            pMessage = 
                                (p_call_obj)->pUasTransaction->pSipMsg;
                        }

                    }


                     /*SPR 18864 */
                    if ((Sdf_co_null != p_call_obj->pUasTransaction) &&
                        (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(\
                                p_call_obj->pUasTransaction->pSipMsg,\
                                &p_method, p_sdf_error)))
                    {
                        /* Add an informational trace*/
                        ret_val = ICF_FAILURE;
                    }
                    else if(ICF_NULL != p_method)
                    {
                        icf_port_strcpy((icf_uint8_t *)methodName,(icf_uint8_t *)p_method);
#ifdef ICF_STAT_ENABLE
                        /*SPR18864 Added null check for stats info in glb pdb*/
                        if(ICF_NULL != p_ssa_pdb->p_glb_pdb->p_stats_info)
                        {
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
                            /*  CSR 1-5385147 Merge */
                            else if(icf_port_strcmp((icf_uint8_t*)methodName,(icf_uint8_t*)"NOTIFY")== 0)
                            {
                             ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                  stat_requests.stat_inbound_req.stats_NOTIFY)
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)methodName,(icf_uint8_t*)"INFO")== 0)
                            {
                             ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                  stat_requests.stat_inbound_req.stats_INFO)
                            }
                            else if(icf_port_strcmp((icf_uint8_t*)methodName,(icf_uint8_t*)"OPTIONS")== 0)
                            {
                             ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                  stat_requests.stat_inbound_req.stats_outdlg_OPTION)
                            }
                            if(0 == icf_port_strcmp((icf_uint8_t*)methodName,(icf_uint8_t*)"INVITE"))
                            {
                                if(!(p_ssa_pdb->common_bitmask & ICF_SSA_STATS_ALREADY_UPDATED))                                                                         {                                              
                                    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                        stat_requests.stat_inbound_req.stats_INVITE)
                                }
                                else
                                {
                                    p_ssa_pdb->common_bitmask &= ~(ICF_SSA_STATS_ALREADY_UPDATED);
                                }
                                icf_ssa_invite_resp_stats(p_ssa_pdb,resp_code,1);
                            }
                            else
                            {
                                icf_ssa_resp_stats(p_ssa_pdb, methodName, resp_code, 1);
                            }
                        }
#endif
                    }

/*  CSR 1-5385208 Merge */
#ifdef ICF_TRACE_ENABLE
                        if(ICF_FAILURE != ret_val)
                         {
                             if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                              {
                                  if((ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)&&
                                     (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt) &&
                                     (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_rgm_ctxt))
                                   {
                                       if(ICF_FAILURE == icf_ssa_make_criteria_data_from_msg(
                                                            p_ssa_pdb,
                                                            p_call_obj,
                                                            &SdfError))
                                        {
                                            ret_val = ICF_FAILURE;
                                        }
                                   }
                              }
                       
                             p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
                             ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)

                             /* This bit is set here to indicate that trace for
                              * this pdu is already sent from here and should be
                              * sent again from icf_ssa_process_nw_message,when
                              * control returns to it. */
                             p_ssa_pdb->common_bitmask |= ICF_SSA_TRACE_REQUIRED;                             
                         }
#endif
                    if(ICF_SUCCESS == ret_val)
                    {
                       /*Insert P-Access-nw-info header for indialog failure
                        *responses other than responses for method CANCEL*/
                       if(ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx)
                       {  
                          if(0 != icf_port_strcmp((icf_uint8_t *)p_method,
                                                       (icf_uint8_t *)"CANCEL"))
                          {   
                              if(ICF_FAILURE == 
                                      icf_ssa_form_and_insert_access_nw_info_hdr(
                                                p_ssa_pdb,pMessage))
                              {
                                  ret_val = ICF_FAILURE ;
                              }
                          }    
                       }
                    }
                    
                    /* SPR 18819: While adding the code for inserting P-Access-nw
                     * -info header, the code leg of if-elseif have been modified 
                     * therefore after sending the SIP message, for INVITE responses
                     * the entry was not been added in the callid - ssa context map
                     * resulting into ACK being treated as Stray message and memory leak.
                     */ 
                    if(ICF_FAILURE == ret_val)
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

                        if((ICF_NULL != p_ssa_ctxt) &&
                           (ICF_FAILURE != 
                                icf_ssa_add_callid_callobj_map(
                                    p_ssa_pdb,
                                    p_ssa_ctxt,
                                    p_ssa_pdb->p_ecode)))
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
                            if(Sdf_co_null != p_call_obj)
                            {
                               HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                            }   
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
                /*Do further processing only when call object is not already freed
                 *Call object can be freed in case of TCP/TLS out-of-dialog request
                 *is received and failure has already been send to remote*/
                else if ((ICF_SSA_CALL_OBJ_ALREADY_FREED != 
                           (ICF_SSA_CALL_OBJ_ALREADY_FREED & p_ssa_pdb->common_bitmask)) &&
                         (Sdf_co_null != p_event_ctxt)&&
                         (Sdf_co_null != p_event_ctxt->pData) &&
                         (!(((icf_ssa_ctxt_st*)(p_event_ctxt->pData))->bitmask
                            & IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV)))
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
                   if ((Sdf_co_null != p_call_obj->pUasTransaction) &&
                       (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(\
                                p_call_obj->pUasTransaction->pSipMsg,\
                                &p_method, p_sdf_error)))
                    {
                        /* Add an informational trace*/
                        ret_val = ICF_FAILURE;
                    }

                    else if(0 != icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"ACK"))
                    {
                        /* spr16769: We don't need to increment ref count 
                           in case of ACK as it does not creates any dialog */
                          /*SPR 18864 Added the null check for call obj*/
                        if(Sdf_co_null != p_call_obj)
                        {
                           HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                        }   
                    }
                    ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                            ICF_SSA_INTER_MODULAR_MSG);
                }
                else
                {
                    /* Fix for SPR 18976 is merged are a part of CSR merging :
                     * CSR-1-6779028*/
                    /* SPR 18976: In this execution leg , the assumption was that INVITE
                     * message with To Tag will be received and the ssa context shall be
                     * added in the Call ID map. Therefore the RefCount of call object has
                     * been incremented by 1.
                     * For messages other than INVITE, this handling is not required therefore
                     * the check for method as INVITE has been added here 
                     */

                    /* SPR 19592 :

                       Scenario:

                       Call established on TCP.
                       Peer sends OPTIONS.
                       IPTK sends 486 response.
                       The call object is freed in the function icf_ssa_form_and_send_response.
                       Also the bit ICF_SSA_CALL_OBJ_ALREADY_FREED is set.

                       We should access p_call_obj only if it has not been freed.
                       Thus its check has been moved outside.

                    */   
                    if(ICF_SSA_CALL_OBJ_ALREADY_FREED != 
                                (ICF_SSA_CALL_OBJ_ALREADY_FREED & p_ssa_pdb->common_bitmask))
                    {
                        if(Sdf_co_fail == 
                                sdf_ivk_uaGetMethodFromSipMessage(
                                    p_call_obj->pUasTransaction->pSipMsg,
                                    &p_method, p_sdf_error))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else if ( 0 == icf_port_strcmp((icf_uint8_t *)p_method,
                                (icf_uint8_t *)"INVITE"))
                        {
                            /* spr17154: This leg will be hit when the Incoming INVITE with To
                             * tag comes and sdf_cbk_uaUnexpectedRequestReceived
                             * callback is invoked then 481 is sent timer is started
                             * we do not have to free call object now but after timer
                             * expiry so increment ref count */
                            /*Do further processing only when call object is not already freed
                             *Call object can be freed in case of TCP/TLS out-of-dialog request
                             *is received and failure has already been send to remote*/
                            if(Sdf_co_null != p_call_obj)
                            {   
                                HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                            }  
                        }
                    }
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
            if(Sdf_co_null != p_call_obj)
            {
                /* Fix for SPR: 19886 (Merged SPR:19534)*/
                /* Fix for SPR 19534 */
                /* If the CallObject and SSA Ctxt are about to be freed,
                 * then set the bit in SSA pdb for the same. This bit will be
                 * checked after returning from here for traces otherwise there
                 * will be FMRs when this function returns and SSA tries to
                 * use p_ssa_pdb->p_ssa_ctxt in icf_ssa_app_trace().
                 * Also free the txn as it indicates no response could be sent.
                 */
                if (1 == p_call_obj->dRefCount)
                {
                    if (ICF_NULL != p_call_obj->pUasTransaction->pTxnKey)
                    {    
                        icf_ssa_free_sip_txn_data_on_failure(
                                p_call_obj->pUasTransaction->pTxnKey);
                    }    
                }                
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            }   
            /* set the bitmask for identifying that call object is free
               This is done for scenario when precondition call is established
               and the UPDATE is received with "precondition" tag in
               the "require" header */
            if(ICF_NULL == p_call_obj)
            {   
		       p_ssa_pdb->common_bitmask |= ICF_SSA_CALL_OBJ_ALREADY_FREED;
            }   
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
 * FUNCTION:        icf_ssa_handle_stray_msg
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

/*  CSR 1-5385208 Merge */
#ifdef ICF_TRACE_ENABLE
    if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
     {
         if((ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_call_ctxt)&&
            (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt) &&
            (ICF_NULL ==  p_ssa_pdb->p_ssa_ctxt->p_rgm_ctxt))
          {
              if(ICF_FAILURE == icf_ssa_make_criteria_data_for_undecoded_msgs(
                                    p_ssa_pdb,
                                    p_call_obj,
                                    p_sdf_error))
               {
                   ret_val = ICF_FAILURE;
               }
          }
     }
     p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
     ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)

     /* This bit is set here to indicate that trace for
      * this pdu is already sent from here and should be
      * sent again from icf_ssa_process_nw_message,when
      * control returns to it. */
     p_ssa_pdb->common_bitmask |= ICF_SSA_TRACE_REQUIRED;
#endif

if(ICF_FAILURE != ret_val)
  {
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
            sdf_ivk_uaFreeInitData(p_def_prof);
            ret_val = ICF_FAILURE;
        }
        else if ( ICF_FAILURE == 
                icf_ssa_clone_profile_data(p_glb_pdb,
                    p_def_prof, p_default_profile,p_ecode))
        {
            sdf_ivk_uaFreeInitData(p_def_prof);
            p_def_prof = Sdf_co_null;
            ret_val = ICF_FAILURE;
        }
        else if (Sdf_co_fail == 
                sdf_ivk_uaSetInitDataInCallObject(p_call_obj, \
                    p_def_prof, p_sdf_error))
        {
            sdf_ivk_uaFreeInitData(p_def_prof);
            p_def_prof = Sdf_co_null;
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

   /*klocwork warning removal*/
    ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_overlap_txn)

#ifdef ICF_STAT_ENABLE
    if(ret_val != ICF_FAILURE)
    {
        /*  CSR 1-5385147 Merge */
        if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"CANCEL")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_CANCEL)
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"UPDATE")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
            stat_requests.stat_inbound_req.stats_UPDATE)
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"BYE")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_BYE)
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"PRACK")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_PRACK)
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"REFER")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_REFER)
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"NOTIFY")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
		        stat_requests.stat_inbound_req.stats_NOTIFY);
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"OPTIONS")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_outdlg_OPTION);
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"INFO")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_INFO);
        }
        else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"ACK")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_ACK);
        }
        if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"INVITE")== 0)
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_inbound_req.stats_INVITE)
            icf_ssa_invite_resp_stats(p_ssa_pdb,481,1);
        }
        else
        {
            icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)p_method,481,1);
        }
    }
#endif
  }
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
/* Fix for CSR 1-7155416: SPR 19707 */
/* Added new parameters to send specific response code in
 * case of entry level header length decode failure.
 * The parameter "iptk_decode_fail_resp" is the failure response
 * code to be sent in this case. It will be read only when
 * the boolean "iptk_decode_failed" is TRUE.
 */
icf_return_t     icf_ssa_handle_decode_failure(
                        icf_ssa_pdb_st       *p_ssa_pdb,
                        Sdf_st_eventContext  *p_event_ctxt,
                        Sdf_st_callObject    *p_call_obj,
                        Sdf_st_error         *p_sdf_error,
                        icf_error_t          *p_ecode,
						icf_boolean_t		 iptk_decode_failed,
						Sdf_ty_u16bit		 iptk_decode_fail_resp)
{
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_return_t               ret_value = ICF_SUCCESS;
    icf_ssa_ctxt_st            *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st             *p_glb_pdb = ICF_NULL;
    icf_ssa_glb_data_st        *p_ssa_glb_data = ICF_NULL;
    icf_ssa_ctxt_st            *p_mapped_ssa_ctxt = ICF_NULL;
    Sdf_st_initData            *p_default_profile = ICF_NULL;
    SipMessage		           *p_sip_msg = Sdf_co_null;
    Sdf_ty_s8bit               *p_method = Sdf_co_null;
    Sdf_ty_u16bit              failure_resp_code = 0;
    SipOptions                 sip_options = {0};
    Sdf_st_overlapTransInfo    *p_overlap_txn = Sdf_co_null;
    icf_ssa_tcpm_conn_data_st  *p_resp_conn = ICF_NULL;
    icf_boolean_t              add_call_id_failed = ICF_FALSE;
    /* New error code is defined so as the value of p_sdf_error,which is passed
       to this function is not overwritten */
    Sdf_st_error               Sdf_error = {0,0,0,"\0"};
/*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
    Sdf_ty_u16bit              resp_code = 0;
#endif
    /*Variable to store p_method fetched from UATK data*/
    icf_int8_t                 *p_method_temp = ICF_NULL;
    icf_uint16_t               ref_count = 0;
    
    Sdf_ty_errorCode           uatk_err_code = 0;
    Sdf_ty_errorCode           stk_err_code  = 0;
    Sdf_ty_state 	           temp_call_state = Sdf_en_invalidStateTransition;

    /*Start SPR18864 */
    if((Sdf_co_null != p_event_ctxt) &&
       (Sdf_co_null != p_event_ctxt->pData) &&
       (ICF_NULL != p_ssa_pdb))
    {   
        p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_event_ctxt->pData);
        p_glb_pdb = p_ssa_pdb->p_glb_pdb;
        p_ssa_glb_data = p_ssa_pdb->p_ssa_glb_data;
        if((ICF_NULL != p_ssa_glb_data) &&
           (ICF_NULL != p_glb_pdb))  
        {   
            p_default_profile = p_ssa_glb_data->p_default_profile;
        }
        else
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SSA GLB Data or GLB PDB is null"));
            return ICF_FAILURE;
        }
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SSA PDB/Event Ctx/pData is null"));
        return ICF_FAILURE;
    }/*End SPR 18864*/
#ifdef ICF_TRACE_ENABLE
    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
#endif
    ICF_FUNCTION_ENTER(p_glb_pdb)

    if (ICF_NULL != p_ssa_pdb->p_conn_table_entry)
    {
        ret_val = icf_ssa_check_for_new_conn(p_ssa_pdb,
            p_ssa_ctxt,
            p_ssa_ctxt->p_call_obj);
        if(ICF_SUCCESS == 
           icf_ssa_fill_conn_info_from_Via(p_ssa_pdb, p_call_obj,
               p_ssa_pdb->p_conn_table_entry))
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
    if((Sdf_co_null == p_call_obj->pUasTransaction) ||
       (Sdf_co_null == p_sdf_error)) 
    {
        /*SPR 18864 In all the legs downside p_call_obj->pUasTransaction
          was accessed to retrieve pSipMsg, therefore instead of putting
          protective null checks before every access doing it here. */
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: UAS txn or sdf_error pointer is null"));
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* SPR 19886 : Merged CSR 1-7233803, saving the error codes
           returned by UATK to avoid overwriting.
        */
         uatk_err_code = p_sdf_error->errCode;
         stk_err_code  = p_sdf_error->stkErrCode;

    if (Sdf_en_notImplemented == 
            p_sdf_error->errCode)
    {
        /*
         * Raise a Trace that Request type received is not implemented
         * Failure will not be returned here
         * Get the method From the SipMessage and stop the timer
         * if it is a ACK
         */
        /*SPR 18864 : Added null checks for UAS/UAC txn*/
        if(Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg)
        {   
            p_sip_msg = p_call_obj->pUasTransaction->pSipMsg;
        }    
        else  if(Sdf_co_null != p_call_obj->pUacTransaction)
        {    
            p_sip_msg = p_call_obj->pUacTransaction->pSipMsg;
        }

        failure_resp_code = 501;
    }
    else if (Sdf_en_protocolMismatchInVia == 
            p_sdf_error->errCode)
    {
        SipHeader       via_hdr = {SipHdrTypeAny,SIP_NULL};
        Sdf_ty_s8bit    *p_protocol = Sdf_co_null;

        /*
         * Modify the topmost Via header to the actual
         * protocol type
         */
         if(Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg)
         {
              /*SPR18864 Added null check for p_call_obj->pUasTransaction->pSipMsg
                before invocation to sip_getHeaderAtIndex function to avoid
                crash if pSipMsg is null*/
              if (SipFail == sip_getHeaderAtIndex(p_call_obj->pUasTransaction\
                    ->pSipMsg, SipHdrTypeVia, &via_hdr, 0, \
                    (SipError *)&(p_sdf_error->errCode)))
              {
                  /*
                   * Raise an informational trace here
                   */
                  ret_val = ICF_FAILURE;
              }
         }
         else
         {
             ICF_PRINT(((icf_uint8_t *)"\n[SSA]: SIP is null in UAS txn"));
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


/*  CSR 1-5385208 Merge */
if ( ICF_FAILURE != ret_val)
     {
#ifdef ICF_TRACE_ENABLE
         if(ICF_FAILURE == icf_ssa_make_criteria_data_for_undecoded_msgs(
                                               p_ssa_pdb,
                                               p_call_obj,
                                               p_sdf_error))
          {
              /*Free Txn Key and buffer when IPTK could not send
               *failure response for decode failure of an incoming message*/
              icf_ssa_free_sip_txn_data_on_failure(
                  p_call_obj->pUasTransaction->pTxnKey);
              ret_val = ICF_FAILURE;
          }
          else
          {  
              p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;                           
              ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
          }
#endif
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
                /*SPR 18761 : Copy the p_method to p_method_temp here as it will be
                 * used to generate STATS for ACK message. Also the following code of capturing
                 * ACK as response stats have been removed as it is not relevant. */

                Sdf_mc_strdup(p_method_temp, p_method);
                /*Start SPR 19189 Added null check*/
                ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                        p_call_obj->pCommonInfo,
                        p_call_obj->pCommonInfo->pCallid,
                        ret_value)
                if(ICF_FAILURE == ret_value)
                {
                    ret_val = ICF_FAILURE;
                    /*SPR 19527 Free memory taken for method name*/
                    if(ICF_NULL != p_method_temp)
                    {
                        sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                   p_sdf_error);
                    }
                    ICF_FUNCTION_EXIT(p_glb_pdb)
                    return ICF_FAILURE;
                }/*End SPR 19189*/

                /* update the call state in the call object and also trigger 
                 * generic call clear to CC.
                 * Call state is changed otherwise when CC will try to send BYE , 
                 * UATK will not allow
                 */
                /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18956)*/                
                /* Fix for SPR 18956:
                   The fix caters to scenarios wherein incoming ACK fails to
                   decode. There are broadly two possible scenarios:

                   1.

                   <----------------- INVITE (decode fails/successfully decoded)
                   -----------------> 4xx/5xx
                   <----------------- ACK (fails to decode)

                   2.


                   <----------------- INVITE (successfully decoded)
                   -----------------> 2xx
                   <----------------- ACK (fails to decode)

                   In scenario 1), we would fetch the ssa context and simply
                   call the function icf_ssa_process_forceful_call_clear to 
                   clear the ongoing transaction and do call object related
                   clean-up.

                   In scenario 2), we would fetch the ssa context and would
                   update the call state in the call object and also trigger
                   generic call clear to CC.Call state is changed so that UATK
                   allows sending of BYE.
                 */                
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
                     /* SPR 18813: if the call context exist only then send
                      * Generic Call Clear Indication and also change the call state
                      * for attempting BYE.
                      */ 
                     /* Scenario 2 */
                     if( p_ssa_ctxt && (ICF_NULL != p_ssa_ctxt->p_call_ctxt))
                     {
                         /* Trigger generic call clear towards CC */
                         if(ICF_NULL != p_ssa_pdb->p_internal_msg)
                         {                         
                             p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                             p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                 ICF_GENERIC_CALL_CLEAR;
                             p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                                 p_ssa_pdb->p_glb_pdb;
                             p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx = 
                                 p_ssa_ctxt->p_call_ctxt;
                         }
                         p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                         p_ssa_ctxt->p_call_ctxt->signalling_ongoing=
                             ICF_INT_MODULE_INVALID;

                         ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                                 ICF_SSA_INTER_MODULAR_MSG)

                         sdf_ivk_uaChangeCallState(p_ssa_ctxt->p_call_obj,
                                 p_overlap_txn,
                                 p_call_obj->pUasTransaction->pSipMsg, 
                                 Sdf_en_msgModeRecd, p_sdf_error);
                     }
                     /* Scenario 1 */
                     else
                     {
                         /* Assigning the mapped ssa context to ssa pdb so
                            that the following function does cleanup of the
                            correct call object.
                          */
                         p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
                         /* Call the following function to clean the INVITE
                            transaction and the related call object.
                          */
                         icf_ssa_process_forceful_call_clear(p_ssa_pdb);
                     }                     
                }
            }
            /*CSR_1-7233803 Fixed*/
            /* Fix for CSR 1-7155416: SPR 19707 */
			/* Added failure handling for MESSAGE, OPTIONS, BYE, REFER, CANCEL request */
            else if ((0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"INVITE")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"NOTIFY")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"UPDATE")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"INFO")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"MESSAGE")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"OPTIONS")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"BYE")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"REFER")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"CANCEL")) ||
                     (0 == icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"PRACK")))
            {
                Sdf_st_initData     *p_def_prof = Sdf_co_null;

                /*Create copy of p_method for local use, as p_method 
                 *can be removed from sdf_ivk_uaRejectRequest*/
                Sdf_mc_strdup(p_method_temp, p_method);

                if ( Sdf_co_fail == 
                        sdf_ivk_uaInitInitData(&p_def_prof, p_sdf_error))
                {
                    sdf_ivk_uaFreeInitData(p_def_prof);
                    ret_val = ICF_FAILURE;
                }
                else if ( ICF_FAILURE == 
                        icf_ssa_clone_profile_data(p_glb_pdb,
                            p_def_prof, p_default_profile,p_ecode))
                {
                    sdf_ivk_uaFreeInitData(p_def_prof);
                    p_def_prof = Sdf_co_null;
                    ret_val = ICF_FAILURE;
                }
                else if (Sdf_co_fail == 
                        sdf_ivk_uaSetInitDataInCallObject(p_call_obj, \
                            p_def_prof, p_sdf_error))
                {
                    sdf_ivk_uaFreeInitData(p_def_prof);
                    p_def_prof = Sdf_co_null;
                    /* SPR14498 removed MEMFREE*/
                    ret_val = ICF_FAILURE;
                }
                else if (Sdf_co_fail == sdf_ivk_uaFreeInitData (
                            p_def_prof))
                {
                    /*Raise a Major error*/
                    ret_val = ICF_FAILURE;
                }
                /* Fix for CSR 1-7155416: SPR 19707 */
				/* If IPTK entry level header length validation failed,
				 * then response code = iptk_decode_fail_resp
				 * else response code = failure_resp_code
				 */
                else if (Sdf_co_fail == 
                            sdf_ivk_uaRejectRequest(p_call_obj, \
                                p_def_prof, 
								(ICF_TRUE == iptk_decode_failed)?iptk_decode_fail_resp:failure_resp_code, 
                                &p_overlap_txn, p_sdf_error))
                {
                    /* SPR14498 removed MEMFREE*/
                    ret_val = ICF_FAILURE;
                }
                else
                {

                    sip_options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

                    /* SPR 18820: SSA conext should be stored only if the 
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
                       /*Start SPR 19189*/ 
                       ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                               p_call_obj->pAppData, 
                               p_call_obj->pCallInfo,
                               ret_value)
                       if(ICF_FAILURE == ret_value)
                       {
                           ret_val = ICF_FAILURE;
                           /*SPR 19527 Free memory taken for method name*/
                           if(ICF_NULL != p_method_temp)
                           {
                              sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                   p_sdf_error);
                           }
                           /*klocwork warning removal*/
                           ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_overlap_txn)
                           ICF_FUNCTION_EXIT(p_glb_pdb)
                           return ICF_FAILURE;
                       }/*End SPR 19189*/
                       /*SPR 20148 Store the call state for identifying if
                        * it's the initial INVITE request then later we shall
                        * not be doing forceful clear for enabling
                        * retransmission of 400 response*/
                       temp_call_state = p_call_obj->pCallInfo->dState;
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

                    /* Since decode failure is to be sent in same connection.
                     * So setting it */
                    /*SPR 18864 Added the null check for ssa ctx */
                    if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                    {
                        p_resp_conn = p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry;
                        p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry =
                               p_ssa_pdb->p_conn_table_entry;
                    }   

                    /*CSR_1-7233803 Fixed*/
                    /* Fix for CSR 1-7155416: SPR 19707 */
					/* The below logic for sending response message is needed 
					 * for UATK decode failures and not for IPTK entry level
					 * header length validation failures.
					 */
                    if ((ICF_FALSE == iptk_decode_failed) &&
                        ((0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t *)"NOTIFY")) ||
                        (0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t *)"UPDATE")) ||
                        (0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t *)"INFO")) ||
                        (0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t *)"PRACK")) ||
                        ((0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t *)"INVITE")) &&
                        (Sdf_en_parserError == uatk_err_code) && 
                        (Sdf_en_parserError == stk_err_code))))
                    {
                           SipTranspAddr* pTranspaddr = Sdf_co_null;
                           SIP_S8bit      dTranspType = 0;
                           Sdf_st_txnContextData txn_data = {0,0,0,0,0,0,0};
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

                           if(p_call_obj->pUasTransaction->pDestInfo->dScheme == \
                                                    Sdf_en_protoUdp)
                           {    
                               dTranspType = dTranspType | SIP_UDP;
                           }
                           else if(p_call_obj->pUasTransaction->pDestInfo->dScheme == \
                                   Sdf_en_protoTcp)

                           {
                               dTranspType = dTranspType | SIP_TCP;
                           }
#ifdef ICF_SECURE_SUPPORT
                           else if(p_call_obj->pUasTransaction->pDestInfo->dScheme == \
                                   Sdf_en_protoTls)

                           {
                               dTranspType = dTranspType | SIP_TLS;
                           }
#endif
                           txn_data.pRefCallObject = p_call_obj;

                           p_event_ctxt->pData = (Sdf_st_txnContextData *)&txn_data;

                           HSS_LOCKEDINCREF(p_event_ctxt->dRefCount);
                           ref_count =  p_event_ctxt->dRefCount;
                           if ((sip_sendMessage(p_sip_msg, 
                                                (SipOptions*)&(sip_options.dOption),
                                                pTranspaddr, 
                                                dTranspType, 
                                                (SipEventContext*)p_event_ctxt, 
                                                (SipError*)&(p_sdf_error->stkErrCode))) 
                                                == SipFail)
                           {
                                ICF_PRINT(((icf_uint8_t *)"\n[SSA]: sip_sendMessage failed!\n"))
                           }
                           if (p_event_ctxt->dRefCount >= ref_count)
                           {
                               HSS_LOCKEDDECREF(p_event_ctxt->dRefCount);
                           }
                           sdf_memfree(Sdf_mc_callHandlerMemId,
                                       (Sdf_ty_pvoid *) &(pTranspaddr->pHost),
                                        Sdf_co_null);

                           sdf_memfree(Sdf_mc_callHandlerMemId,
                                       (Sdf_ty_pvoid *)&pTranspaddr,
                                       Sdf_co_null);
                           
                           /* Fix for CSR 1-7155416: SPR 19707 */
                           if (ICF_NULL != p_call_obj->pUasTransaction->pTxnKey)
                           {    
                               icf_ssa_free_sip_txn_data_on_failure(
                                       p_call_obj->pUasTransaction->pTxnKey);
                           }
                    }
                    /*Call send_to_peer only when pTxnKey is non-NULL otherwise
                     *UATK will not be able to send message on NW and this function
                     *will return failure*/
                    if (ICF_NULL != p_call_obj->pUasTransaction->pTxnKey) 
                    {
                        if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                p_call_obj,
                                p_sip_msg,
                                &sip_options,p_sdf_error))
                        {
#ifdef SDF_THREAD_SAFE
                            if (p_call_obj->dRefCount.ref > 1) 
#else
                            if (p_call_obj->dRefCount > 1)
#endif
                            {
                                HSS_LOCKEDDECREF(p_call_obj->dRefCount);
                            }
#ifdef SDF_THREAD_SAFE
                            if (p_call_obj->pUasTransaction->dRefCount.ref > 1)
#else
                            if (p_call_obj->pUasTransaction->dRefCount > 1)
#endif
                            {
                                HSS_LOCKEDDECREF(p_call_obj->pUasTransaction->dRefCount);
                            }
                            /*Free Txn Key and buffer when IPTK could not send
                              failure response for decode failure of an INVITE*/
                            icf_ssa_free_sip_txn_data_on_failure(
                                    p_call_obj->pUasTransaction->pTxnKey);
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
#ifdef ICF_STAT_ENABLE
                            /*Updating response INVITE STATS as 400 successfully sent*/
                            icf_ssa_invite_resp_stats(p_ssa_pdb,failure_resp_code,1);
#endif
                            p_event_ctxt->pData = p_ssa_ctxt;
                            HSS_LOCKEDINCREF(p_call_obj->dRefCount);
                            p_ssa_ctxt->failure_bitmask |=
                                ICF_SSA_DECODE_FAILURE ;
                        }
                        else
                        {
#ifdef ICF_STAT_ENABLE
                            /*Update STATS for response of non-INVITE requests*/
                            icf_ssa_resp_stats(p_ssa_pdb, (icf_uint8_t *)p_method_temp, failure_resp_code, 1);
#endif
                        }

                    }
                    if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
                    {
                      p_ssa_pdb->p_ssa_ctxt->p_conn_table_resp_entry = p_resp_conn;
                    }  

                    if (add_call_id_failed)
                    {
                        /* spr16358 : In case of incoming INVITE if via is in 
                           invalid then call object was not getting free 
                           because p_call_obj->dRefCount turns out as 2 so
                           decremented the same before call to 
                           icf_ssa_process_forceful_call_clear */
#ifdef SDF_THREAD_SAFE
                        if (p_call_obj->dRefCount.ref > 1)
#else
                        if (p_call_obj->dRefCount > 1)
#endif
                        {
                            HSS_LOCKEDDECREF(p_call_obj->dRefCount);
                        }
                        /*SPR 20148 If the initial INVITE is having the issue
                         * then no need to trigger the forceful call clear
                         * because triggering forceful call clear shall
                         * release txn and then retransmission of  400 shall
                         * not happen*/
                        if(Sdf_en_inviteReceived != temp_call_state)
                        {        
                            icf_ssa_process_forceful_call_clear(p_ssa_pdb);
                            p_ssa_pdb->common_bitmask |=
                                    ICF_SSA_CALL_OBJ_ALREADY_FREED;
                        }    
                    }
                }
            }
           /*  CSR 1-5385147 Merge */
#ifdef ICF_STAT_ENABLE
            /*Start SPR 19189 Added the null check */
            ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                p_ssa_pdb->p_glb_pdb->p_stats_info, ret_value)
            if(ICF_FAILURE == ret_value)
            {
                ret_val = ICF_FAILURE;
                    /*SPR 19527 Free memory taken for method name*/
                if(ICF_NULL != p_method_temp)
                {
                     sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                                   p_sdf_error);
                }
                /*klocwork warning removal*/
                ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_overlap_txn)
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
            }/*End SPR 19189 */
            if(ICF_NULL == p_method_temp)
            {
               /* do nothing */
            } 
            else if (0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"INVITE"))
             {
                 /* CSR 1-5385147 Merge */
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                       stat_requests.stat_inbound_req.stats_INVITE)
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"CANCEL")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_CANCEL)
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"UPDATE")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_UPDATE)
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"BYE")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_BYE)
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"PRACK")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_PRACK)
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"REFER")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_REFER)
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"NOTIFY")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_NOTIFY);
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"OPTIONS")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_outdlg_OPTION);
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"INFO")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_INFO);
             }
             else if(icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"ACK")== 0)
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_ACK);
             }
             else if(0 == icf_port_strcmp((icf_uint8_t *)p_method_temp,(icf_uint8_t*)"MESSAGE"))
             {
                 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                     stat_requests.stat_inbound_req.stats_MESSAGE);
             }
#endif
             /*Free memory taken for method name*/
             if (Sdf_co_null != p_method_temp)
             {
                 sdf_memfree(0, (Sdf_ty_pvoid *)&p_method_temp,
                             p_sdf_error);
             }
        }
        else if ((Sdf_co_null != p_call_obj->pUacTransaction) &&
                 (Sdf_co_null != p_call_obj->pUacTransaction->pSipMsg))
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
            /*SPR 18864 Added the null check for pCommonInfo */
            if((Sdf_co_null != p_call_obj->pCommonInfo) &&
               (Sdf_co_null != p_call_obj->pCommonInfo->pCallid))
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
                  *(p_ssa_pdb->p_ecode) = ICF_ECODE_GENERIC_ERROR;
                  ret_val = ICF_FAILURE;
                } 

              /*If contact header is not received in final 2xx response
               * of invite method,then fetch the ssa ctxt and set the bit
               * in its bitmask so as to abort the invite transaction 
               * during the forceful call clear.*/
               else if((ICF_NULL != p_method) &&
                       (0 == icf_port_strcmp((icf_uint8_t *)p_method, (icf_uint8_t *)"INVITE")))
                {
                  if( ICF_FAILURE != icf_ssa_map_nw_callid(
                    p_ssa_pdb,
#ifdef ICF_LOOPBACK_SUPPORT
                    p_call_obj,
#endif
                    (icf_uint8_t *)p_call_obj->pCommonInfo->pCallid,
                    &p_ssa_ctxt,
#ifdef ICF_LOOPBACK_SUPPORT
                    &Sdf_error,
#endif
                    p_ecode))
                      {
#ifdef ICF_STAT_ENABLE
                            /*  CSR 1-5385147 Merge */
                            if(Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                                    p_call_obj->pUacTransaction->pSipMsg, &resp_code,
                                    &Sdf_error))

                            {
                                /* No need to track ret_val here */
                            }
                            else
                            {
                                icf_ssa_invite_resp_stats(p_ssa_pdb,resp_code,0);
                            }
#endif          
                            /* SPR 19097: Do the handling only is the response received 
                             * is a final respone , in case of provisional response
                             * it may be possible the final response is received correctly
                             */  
                            if((Sdf_en_mandatoryHdrMissing == p_sdf_error->errCode) && 
                                    (200 <= resp_code))
                            {
                                /* SPR 19097 : Since the SSA context and Call Context
                                 * is available therefore GENERC CALL CLEAR should be
                                 * triggered towards CM.
                                 */ 
                                if((ICF_NULL != p_ssa_ctxt) && (ICF_NULL != p_ssa_ctxt->p_call_ctxt))
                                {
                                    /* Trigger generic call clear towards CC */
                                    if(ICF_NULL != p_ssa_pdb->p_internal_msg)
                                    {                         
                                        p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                            ICF_GENERIC_CALL_CLEAR;
                                        p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                                            p_ssa_pdb->p_glb_pdb;
                                        p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx = 
                                            p_ssa_ctxt->p_call_ctxt;
                                    }
                                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                                    p_ssa_ctxt->p_call_ctxt->signalling_ongoing=
                                        ICF_INT_MODULE_INVALID;

                                    ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                                            ICF_SSA_INTER_MODULAR_MSG)

									if(ICF_CALL_CONNECT_DONE == 
                                            (p_ssa_ctxt->p_call_ctxt->common_bitmask & ICF_CALL_CONNECT_DONE))
                                    {
                                        /* SPR 19097: Since the transaction could not be completed
                                         * as ACK is not sent for the response therefore the 
                                         * transaction should be aborted. this is only required
                                         * only in the case of reinvite, for initial invite the
                                         * transaction will be terminated through call clear.
                                         */ 
                                        sdf_ivk_uaAbortTransaction(
                                                p_ssa_ctxt->p_call_obj, 
                                                p_ssa_ctxt->p_call_obj->pUacTransaction->dTxnId,
                                                (Sdf_ty_pvoid*)p_ssa_ctxt, 
                                                &Sdf_error);
                                    }	
                                }
                                else
                                {
                                    *(p_ssa_pdb->p_ecode) = ICF_ECODE_GENERIC_ERROR;
                                    ret_val = ICF_FAILURE;
                                }

                                if(ICF_NULL != p_ssa_ctxt)
                                {
                                    p_ssa_ctxt->bitmask_1 |= ICF_MANDATORY_HDR_MISSING ;
                                }
                            }
                            else if(Sdf_en_tooManyViaHdrsInResponse == p_sdf_error->errCode)
                            {
                                /* Fix for SPR 19388:
                                   In case error code returned by stack is 
                                   Sdf_en_tooManyViaHdrsInResponse, fetch the ssa
                                   context and clear the call by issuing generic call
                                   clear towards CC for 200 OK response only.
                               
                                   Clear the call only if the response code is 200,for 
                                   provisional responses, no need to clear the call as 
                                   we may still get a final response with a single Via header.
                                */   
                                if(200 == resp_code)
                                {
                                    if(ICF_NULL != p_ssa_pdb->p_internal_msg)
                                    {                         
                                        p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                            ICF_GENERIC_CALL_CLEAR;
                                        p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                                            p_ssa_pdb->p_glb_pdb;
                                        if(ICF_NULL != p_ssa_ctxt)
                                        {
                                            p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx = 
                                                p_ssa_ctxt->p_call_ctxt;
                                        }
                                    }
                    
                                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                                    if(ICF_NULL != p_ssa_ctxt)
                                    {
                                        p_ssa_ctxt->p_call_ctxt->signalling_ongoing =
                                            ICF_INT_MODULE_INVALID;
                                    }

                                    /* Set the following bit so that SSA is able to route this 
                                       internal message to CC.
                                    */   
                                    ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask),\
                                        ICF_SSA_INTER_MODULAR_MSG)
                                }
                            }/* End of Fix for SPR 19388 */
                        }
                }
                
               /* Sdf_en_mandatoryHdrMissing or Sdf_en_tooManyViaHdrsInResponse 
                  error code is not received */
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
#ifdef ICF_STAT_ENABLE
                    /*  CSR 1-5385147 Merge */
                    if(Sdf_co_fail == sdf_ivk_uaGetRespCodeFromSipMessage(
                                p_call_obj->pUacTransaction->pSipMsg, &resp_code,
                                p_sdf_error))

                    {
                        /* No need to track ret_val here */
                    }
                    else if( p_method && (icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"INVITE")== 0))
                    {
                        icf_ssa_invite_resp_stats(p_ssa_pdb,resp_code,0);
                    }
                    else
                    {
                        icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t*)p_method,resp_code,0);
                    }
                    icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t*)p_method,resp_code,0);
#endif                    
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
             /*SPR 18864 Added the null check for pCommonInfo */
            if((Sdf_co_null != p_call_obj->pCommonInfo) &&
               (Sdf_co_null != p_call_obj->pCommonInfo->pCallid))
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
    }/*End of else for UAS txn presence */
    /*klocwork warning removal*/
    ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_overlap_txn)
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
        /* klocwork fix */
	ICF_STATIC_MEMFREE(p_glb_pdb,
		p_glb_pdb->p_glb_data->p_dns_info,
		p_ecode, ret_val) 
        sdf_ivk_uaFreeInitData(p_default_profile);
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
            /* klocwork fix */
			ICF_STATIC_MEMFREE(p_glb_pdb,
					p_glb_pdb->p_glb_data->p_dns_info,
					p_ecode, ret_val)

			sdf_ivk_uaFreeInitData(p_default_profile);
			ret_val = ICF_FAILURE;
		}
		else
		{
				ICF_STATIC_MEMGET(p_glb_pdb, ICF_PORT_SIZEOF(
				     icf_ssa_glb_data_st),p_ssa_data,
                     ICF_DONOT_RET_ON_FAILURE, p_ecode, ret_val)
			
                      if(ICF_FAILURE == ret_val)
		      {
                         /* klocwork fix */
			ICF_STATIC_MEMFREE(p_glb_pdb,
				p_glb_pdb->p_glb_data->p_dns_info,
				p_ecode, ret_val)

			sdf_ivk_uaFreeInitData(p_default_profile);
			return ret_val;
		     }
	
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
        /* Fix for CSR 1-7436195 */
        /* Coverity bug fix */
        if(ICF_SSA_TCPM_MAX_EVENTS != tcp_event)
        {
            ICF_SSA_IVK_TCPSM((&ssa_pdb), tcp_event, ret_val)
            if( ICF_FAILURE == ret_val)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInTCPSM"));
            }
        }
    }

    /*This bit is set in ssa_pdb when primary server is not responding
     *and SSA sent request on secondary server, so call object with primary server
     *will be freed in this case*/
    if (ICF_SSA_DEALLOCATE_CALL_OBJECT & ssa_pdb.common_bitmask)
    {
        Sdf_st_callObject *p_call_obj = ssa_pdb.p_call_obj_to_dealloc;

        sdf_ivk_uaFreeCallObject(&p_call_obj);
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
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_nw_msg(
        INOUT icf_internal_msg_st    *p_internal_msg)
{
    icf_ssa_pdb_st           ssa_pdb ;
    /*
     * Pointer to internal msg union, used by SSA to 
     * pass parameter structure to the destination module
     */
    /* Local variable is replaced with pointer variable to resolve the klocwork
     * warning*/
    icf_ssa_intrnl_intf_ut   *p_internal_msg_payload = ICF_NULL;
    icf_ssa_glb_data_st      *p_ssa_glb_data = ICF_NULL;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    Sdf_ty_retVal             mesg_length_status = Sdf_co_fail;
    /* CSR 1-5718212: Fix by Rohit: change func argument type */
    Sdf_ty_IPTKmessageLengthValidity   sdf_retval = Sdf_en_valid;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             return_val = ICF_SUCCESS;
    icf_return_t             dummy_ret_val = ICF_SUCCESS;
    icf_error_t              *p_ecode = ICF_NULL;
    icf_uint32_t             pdu_len = 0, count = 0;
    Sdf_ty_u32bit               check_option = 0, dCount = 0;
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
                             *p_current_sip_pdu = ICF_NULL,

    /***Change start spr-18503***/
                             *p_local_pending_pdu = ICF_NULL;  
    /*It is used to store the previously stored pending Pdu length
      before appending the new message on socket*/
    icf_uint16_t              pre_pending_pdu_length =0;
   
    /*It is used to store the pending Pdu length after appending
      the new message on socket to p_pending_pdu*/  
    icf_uint16_t             total_pending_pdu_length =0;

    Sdf_ty_matchMessageType     msg_type = 0;
    /***Change end spr-18503****/

    icf_boolean_t            more_messages = ICF_FALSE; 

    /*
     * Variables for usage of Stack/UATK APIs
     */
    Sdf_st_error            sdf_error = {0,0,0,"\0"};
    Sdf_st_eventContext     *p_event_ctxt = Sdf_co_null;
    SipTranspAddr           *p_transp_addr = Sdf_co_null;
    Sdf_ty_protocol         protocol = Sdf_en_protoNone;
    Sdf_st_callObject       *p_call_obj = Sdf_co_null;
    /* Fix for CSR 1-7155416: SPR 19707 */
	icf_boolean_t			iptk_decode_failed = ICF_FALSE;
    SipMessage              *p_temp_msg = Sdf_co_null;
/*Following variable is used to store ssa_ctxt from conn_db which will
 *be used in case of decode failure*/

    /*CERT fix, if BadHdr list is not Null*/
    
    /* SPR 18864 */
    if((ICF_NULL == p_internal_msg) ||
       (ICF_NULL == p_internal_msg->msg_hdr.p_glb_pdb)) 
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: internal msg/GLB PDB is null"));
        return ICF_FAILURE;
    }
   /* SPR 15181 changes */
    ICF_SSA_GET_GLB_PDB_FRM_INTRNL_MSG(p_glb_pdb,p_internal_msg)

        ICF_FUNCTION_ENTER(p_glb_pdb)

        /*Get common parameters */
        ICF_SSA_GET_RECV_MSG_FRM_INTRNL_MSG(p_recvd_msg,p_internal_msg)
        ICF_SSA_GET_EXT_API_HDR_FRM_INTRNL_MSG(p_ext_api_hdr,p_internal_msg)
        ICF_SSA_GET_EXT_PAYLOAD_FRM_INTRNL_MSG(p_ext_api_body,p_internal_msg)
        /*Start SPR 18864 */ 
        if((ICF_NULL != p_recvd_msg) &&
           (ICF_NULL != p_recvd_msg->payload))
        {   
           p_nw_int_info = (icf_nw_interface_info_st*)(p_recvd_msg->payload);
           p_recvd_pdu = (icf_int8_t *)p_nw_int_info->p_sip_msg;
        }   
        else
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: recvd msg or its payload is null"));
           ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
           return ICF_FAILURE;
        }/*End SPR18864 */ 
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
    if(ICF_FAILURE == icf_dbm_get_module_glb_data(p_glb_pdb,ICF_INT_MODULE_SSA,
            (icf_void_t **)&p_ssa_glb_data,ssa_pdb.p_ecode))
    {
        /*SPR18864 Added failure leg for avoiding crash
          when accessing p_ssa_glb_data */
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        return ICF_FAILURE;
    }
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
      if (ICF_INTF_INFO_PROTOCOL_TCP & p_nw_int_info->bitmap)
      {
        protocol = Sdf_en_protoTcp;
      }
#ifdef ICF_SECURE_SUPPORT
      else if(ICF_INTF_INFO_PROTOCOL_TLS & p_nw_int_info->bitmap)
      {
             protocol = Sdf_en_protoTls;
      }
#endif
      else if (ICF_INTF_INFO_PROTOCOL_UDP & p_nw_int_info->bitmap)
      {
             protocol = Sdf_en_protoUdp;
      }
      

    if (Sdf_en_protoTcp == protocol 
#ifdef ICF_SECURE_SUPPORT
	|| Sdf_en_protoTls == protocol)
#else
	)
#endif
   {
        /***Change for SPR-18503 start****/
        ssa_pdb.p_conn_table_entry = 
                icf_ssa_get_conn_db((icf_uint16_t)p_nw_int_info->receiver_fd,
                                 ssa_pdb.p_ssa_glb_data->p_ssa_tcpm_conn_db);

        if ( ICF_NULL == ssa_pdb.p_conn_table_entry)
        {
            ret_val = ICF_FAILURE;
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!FD not found in ConnDB"));
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
        }

        /*Check for p_pending_pdu.If it is not NULL append received pdu 
          into this and assign it to current_pdu. */
        if(ICF_NULL != ssa_pdb.p_conn_table_entry->p_pending_pdu)
        {
              /*Calculate total length for which the memory will
               *be allocated and assign to p_pending_pdu*/
              pre_pending_pdu_length = 
                   ssa_pdb.p_conn_table_entry->pending_pdu_length ;

              /*Total PDU length will be last time's pending length
               *and new bytes received on this connection*/
              total_pending_pdu_length = pre_pending_pdu_length +
                                         p_nw_int_info->pdu_len;    

              /*if the total length of PDU is less than maximum allowed
              TCP message length then allocate memory to p_pending_pdu 
              else discard it.  */
              if (ICF_MAX_TCP_MSG_LNGTH > total_pending_pdu_length)
              {
                  ICF_MEMGET(ssa_pdb.p_glb_pdb,
                          total_pending_pdu_length + 1,
                          ICF_MEM_COMMON,
                          p_local_pending_pdu,
                          ICF_DONOT_RET_ON_FAILURE,
                          ssa_pdb.p_ecode,
                          ret_val)
              }
              else
              {

                  /* Received message is more than the maximum limit of TCP length
                   * then discard the message as it is not allowed to handle message
                   * beyond ICF_MAX_TCP_MSG_LNGTH.
                   */ 
                  ret_val = ICF_FAILURE;
                  ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!TCP message received beyong MAX limit"));

                  /*remove the read bytes from TCP/TLS socket*/
                  if (Sdf_en_protoTcp == protocol)
                  {
                      /* remove the message from the socket and discard the same.
                       */ 
                      if (ICF_FAILURE == icf_port_tcp_recv(
                                  (icf_uint16_t)ssa_pdb.p_conn_table_entry->conn_fd,
                                  p_recvd_pdu, p_nw_int_info->pdu_len))
                      {
                          ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in removal of bytes from conn"));
                          ret_val = ICF_FAILURE;
                      }
                  }
#ifdef ICF_SECURE_SUPPORT		
                  else
                  {
                      icf_void_t      *p_secure_data = ICF_NULL;
                      /* 
                       * Now extract the SSL Data corrresponding to this
                       * socket fd from the map */
                      if((ICF_NULL != ssa_pdb.p_glb_pdb->p_glb_data)&&
                              (ICF_NULL !=
                               ssa_pdb.p_glb_pdb->p_glb_data->p_glb_port_info))
                      {
                          p_secure_data = icf_es_fetch_secure_data_from_map(
                                  ssa_pdb.p_glb_pdb->p_glb_data->p_glb_port_info,
                                  p_nw_int_info->receiver_fd,
                                  p_ecode);
                      }     
                      if (ICF_NULL == p_secure_data)
                      {
                          ret_val = ICF_FAILURE;
                      }
                      else
                      {

                          if (ICF_FAILURE == icf_port_secure_read(
                                      p_secure_data,(icf_uint16_t)p_nw_int_info->receiver_fd,
                                      p_recvd_pdu, p_nw_int_info->pdu_len))
                          {
                              ret_val = ICF_FAILURE;
                              ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in removal of bytes from conn"));
                          }
                      }
                  }
#endif
                  p_nw_int_info->pdu_len = 0;

                  /* free the pending pdu data as well */
                  if(ICF_NULL != ssa_pdb.p_conn_table_entry->p_pending_pdu)
                  {
                      ICF_MEMFREE(p_glb_pdb,
                              ssa_pdb.p_conn_table_entry->p_pending_pdu,
                              ICF_MEM_COMMON,
                              ssa_pdb.p_ecode,dummy_ret_val)
                  }
                  ssa_pdb.p_conn_table_entry->pending_pdu_length = 0;
                  ICF_FUNCTION_EXIT(p_glb_pdb)
                  return ret_val;
              }

             /*Klocwork warning removal*/
             ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,
                                             p_local_pending_pdu)

           if (ICF_SUCCESS == ret_val)
           {
               /*Copy total pending PDU in connection database after
                *successful memget*/
               ssa_pdb.p_conn_table_entry->pending_pdu_length =
                                            total_pending_pdu_length;

               /*copy the previous stored incomplete message into
                *p_pending_pdu. */
               icf_port_memcpy(p_local_pending_pdu,
                       ssa_pdb.p_conn_table_entry->p_pending_pdu,
                       pre_pending_pdu_length);

               /*Append newly received message on socket into earlier
                *incomplete message.*/ 
               icf_port_memcpy((p_local_pending_pdu + pre_pending_pdu_length),
                       p_current_sip_pdu,
                       p_nw_int_info->pdu_len);

               /* Free the memory earlier allocated to
                *p_conn_table_entry->p_pending_pdu*/
               ICF_MEMFREE(p_glb_pdb,
                           ssa_pdb.p_conn_table_entry->p_pending_pdu,
                           ICF_MEM_COMMON,
                           ssa_pdb.p_ecode,dummy_ret_val)
 
               /*Again assign the memory address where incomplete
                *message is stored*/
               ssa_pdb.p_conn_table_entry->p_pending_pdu = p_local_pending_pdu;
 
               /*Assign the current pdu to p_pending_pdu */
               p_current_sip_pdu = p_local_pending_pdu;
           }
           else
           {
               /*If MEMGET is failed for combined pending PDU then return fail
                *This PDU is not removed from Socket, so this will again be received
                *and MEMGET will be tried again*/
               ret_val = ICF_FAILURE;
               ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!FD not found in ConnDB"));
               ICF_FUNCTION_EXIT(p_glb_pdb)
               return ret_val;
           }
       }
          
       /*remove the read bytes from TCP/TLS socket*/
       if (Sdf_en_protoTcp == protocol)
       {
           /*If the total_pending_pdu_length is equal to MAX allowed TCP
            *message then only that messsage of data will removed from
            *socket which is require to make the pending_pdu_length equal
            *to ICF_MAX_TCP_MSG_LNGTH */
           if (ICF_MAX_TCP_MSG_LNGTH == total_pending_pdu_length)
           {
               if (ICF_FAILURE == icf_port_tcp_recv(
                    (icf_uint16_t)ssa_pdb.p_conn_table_entry->conn_fd,
                    p_recvd_pdu, (ICF_MAX_TCP_MSG_LNGTH - pre_pending_pdu_length)))
               {
                   ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in removal of bytes from conn"));
               }
           }
           else
           {
               if (ICF_FAILURE == icf_port_tcp_recv(
                   (icf_uint16_t)ssa_pdb.p_conn_table_entry->conn_fd,
                   p_recvd_pdu, p_nw_int_info->pdu_len))
               {
                   ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in removal of bytes from conn"));
               }
           }
       }
#ifdef ICF_SECURE_SUPPORT		
       else
       {
             icf_void_t      *p_secure_data = ICF_NULL;
             /* 
              * Now extract the SSL Data corrresponding to this
              * socket fd from the map */
              /*SPR 18864 Added null check
                for the glb data */
              if((ICF_NULL != ssa_pdb.p_glb_pdb->p_glb_data)&&
                 (ICF_NULL !=
                   ssa_pdb.p_glb_pdb->p_glb_data->p_glb_port_info))
              {
                   p_secure_data = icf_es_fetch_secure_data_from_map(
                                     ssa_pdb.p_glb_pdb->p_glb_data->p_glb_port_info,
                                     p_nw_int_info->receiver_fd,
                                     p_ecode);
              }     
             if (ICF_NULL == p_secure_data)
             {
                 ret_val = ICF_FAILURE;
             }
             else
             {
                 if (ICF_MAX_TCP_MSG_LNGTH == total_pending_pdu_length)
                 {
                    if (ICF_FAILURE == icf_port_secure_read(
                         p_secure_data,(icf_uint16_t)p_nw_int_info->receiver_fd,
                         p_recvd_pdu, 
                         (ICF_MAX_TCP_MSG_LNGTH - pre_pending_pdu_length)))
                    {
                        ret_val = ICF_FAILURE;
                    }
                 }
                 else
                 {
                     if (ICF_FAILURE == icf_port_secure_read(
                         p_secure_data,(icf_uint16_t)p_nw_int_info->receiver_fd,
                         p_recvd_pdu, p_nw_int_info->pdu_len))
                     {
                        ret_val = ICF_FAILURE;
                     }
                 }
            }
       }
#endif
   }


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
        /* Following function is added for the purpose of handling large
         * multiple messages on TCP where this do while loop executed multiple
         * times.As per the desired behaviour the ssa_pdb must be new each
         * time this loop is executed;however we cannot reinitialised the ssa
         * pdb here as the same ssa pdb contain the p_conn_table_entry which
         * utimatelty control the fragmented messages handling of TCP/TLS.For
         * handling this new function is defined which initialised all the
         * variable in pdb except p_conn_table_entry and the one which are
         * initialised at the begining.This is required for handling the
         * following such scenarios:
         * INVITE1 and INVITE2 received by IPTK at a same time from TCP
         * socket. While handling the INVITE1 in this do while loop IPTK set
         * the value of line_recvd parameter in ssa pdb.In the same do while
         * loop when IPTK try to handle INVITE2 depending upon the check for 
         * line_recvd variable IPTK send 415 response.Ideally when do while
         * loop is executed for INVITE2 line_recvd in the ssa pdb must be
         * reset. As a pecautioary measure same is done for other variables in
         * ssa pdb.*/
        icf_ssa_init_pdb_for_tcp_extra_msg_hdlg(&ssa_pdb,p_glb_pdb);
        /* Fix for CSR 1-7155416: SPR 19707 */
		/* Initialize this to FALSE for each msg to be decoded */
		iptk_decode_failed = ICF_FALSE;
     if ( ICF_INTF_INFO_PROTOCOL_UDP & p_nw_int_info->bitmap)
      {
        protocol = Sdf_en_protoUdp;
      }
     else if (ICF_INTF_INFO_PROTOCOL_TCP & p_nw_int_info->bitmap)
      {
        protocol = Sdf_en_protoTcp;
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
            /* Since we are in do while loop it might be possible that the
             * memory is allocated for p_internal_msg_payload , so freeing the
             * same if allocated.*/
            if (ICF_NULL != p_internal_msg_payload)
            {     
                ICF_MEMFREE(p_glb_pdb,
                        p_internal_msg_payload,
                        ICF_MEM_COMMON,
                        ssa_pdb.p_ecode,return_val)
                p_internal_msg->p_msg_data = ICF_NULL;
            }
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
        }/* SPR 15181 changes */
        ICF_SSA_RESET_CONN_TIMER_COUNT(ssa_pdb.p_conn_table_entry);
       }
#ifdef ICF_SECURE_SUPPORT
    else if (ICF_INTF_INFO_PROTOCOL_TLS & p_nw_int_info->bitmap)
        
    {
        protocol = Sdf_en_protoTls;
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
            /* Since we are in do while loop it might be possible that the
             * memory is allocated for p_internal_msg_payload , so freeing the
             * same if allocated.*/
            if (ICF_NULL != p_internal_msg_payload)
            {              
                ICF_MEMFREE(p_glb_pdb,
                        p_internal_msg_payload,
                        ICF_MEM_COMMON,
                        ssa_pdb.p_ecode,return_val)
                p_internal_msg->p_msg_data = ICF_NULL;     
            }
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
        /* Since we are in do while loop it might be possible that the
         * memory is allocated for p_internal_msg_payload , so freeing the
         * same if allocated.*/
        if (ICF_NULL != p_internal_msg_payload)
        {          
            ICF_MEMFREE(p_glb_pdb,
                    p_internal_msg_payload,
                    ICF_MEM_COMMON,
                    ssa_pdb.p_ecode,return_val)
            p_internal_msg->p_msg_data = ICF_NULL;        
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
        /* Since the scope of local variable is lost as soon as flow enter some
         * other function, instead of saving the reference to local
         * variable,we are allocating the memory for the same. This is done to
         * resolve the klocwork warning.*/

        /* Null check for p_internal_msg_payload is put to cater the possible
         * memory leak in TCP scenario when fragmented messages are received
         * and this do while loop executed multtiple time. */
        if (ICF_NULL == p_internal_msg_payload)
        {    
            ICF_MEMGET(p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut),ICF_MEM_COMMON,\
                    p_internal_msg_payload,\
                    ICF_DONOT_RET_ON_FAILURE,\
                    p_ecode, return_val)          

            if (ICF_FAILURE == return_val)
            {
                ret_val = ICF_FAILURE;
                ICF_PRINT(((icf_uint8_t *)"Memory allocation failure for internal message\n"));
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ICF_FAILURE;
            }
        }            
        p_internal_msg->p_msg_data  = (icf_uint8_t*)p_internal_msg_payload;
        p_internal_msg->msg_hdr.payload_length  = 
            ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut);

        p_ecode = &(p_internal_msg->msg_hdr.ecode);
        icf_port_memset(p_internal_msg_payload, 0, 
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
                ICF_DONOT_RET_ON_FAILURE,\
                p_ecode, ret_val)
      
        if( ssa_pdb.p_conn_table_entry && (ICF_FAILURE == ret_val))
	     {
           /* Free the memory earlier allocated to
              p_conn_table_entry->p_pending_pdu*/
              ret_val = ICF_FAILURE;
              ICF_MEMFREE(p_glb_pdb,
                          ssa_pdb.p_conn_table_entry->p_pending_pdu,
                          ICF_MEM_COMMON,
                          ssa_pdb.p_ecode,return_val)
              ssa_pdb.p_conn_table_entry->pending_pdu_length = 0;
              ICF_MEMFREE(p_glb_pdb,
                      p_internal_msg_payload,
                      ICF_MEM_COMMON,
                      ssa_pdb.p_ecode,return_val)
              p_internal_msg->p_msg_data = ICF_NULL;   
              ICF_FUNCTION_EXIT(p_glb_pdb)
			  return ICF_FAILURE;
	    }

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
            /* Klocworks warning fix for SPR 18547*/
            ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
            /* Initialize the elements of new ssa ctxt */
			icf_ssa_init_ssa_context(&ssa_pdb,p_ssa_ctxt);            
			
			p_ssa_ctxt->p_ssa_pdb = 
                    (struct icf_ssa_pdb_st*)(&ssa_pdb);
            ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_glb_pdb = 
                                                                p_glb_pdb;
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
                /*Klocwork Warning Removal*/
                ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_event_ctxt)
                ret_val  = ICF_FAILURE;
            }
            else /*Transport Addr, Evt Ctxt and CallObj allocation
                   successful, proceeding..*/
            {
                SipOptions                  sip_options = {0};
                /*For INcoming Requests the Protocol was not getting set
                  in ssa_ctxt. That ultimately creates the problem that 
                  specified in spr18457 (Fix for spr 18457) */
                if(Sdf_en_protoUdp == protocol)
                {
                    p_ssa_ctxt->transport = ICF_TRANSPORT_TYPE_UDP;
                }
                else if(Sdf_en_protoTcp == protocol)
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
					if (ssa_pdb.p_conn_table_entry) {
                    p_event_ctxt->pTranspAddr->pHost =
               (icf_int8_t *)ssa_pdb.p_conn_table_entry->peer_ip;
                    p_event_ctxt->pTranspAddr->dPort =
                        ssa_pdb.p_conn_table_entry->peer_port_num;
						}
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
                /*CSR 1-6022631  Merge  */
                /* If the UDP protocol is used then pick total length of message
                 * recieved from network  */

                if(ICF_INTF_INFO_PROTOCOL_UDP & p_nw_int_info->bitmap)
                {
                    pdu_len = p_nw_int_info->pdu_len;
                }
                else
                {
               	    /* fix for TCP UT */
		    pdu_len=icf_port_strlen((icf_uint8_t *)p_current_sip_pdu) ;	
                }

                /* Klocwork warning removal */
                if((ICF_NULL != p_call_obj) &&
                   (ICF_NULL != p_call_obj->pAppData))     
                {
				    p_call_obj->pAppData->pData = (SIP_Pvoid)(p_ssa_ctxt);
                }
				p_ssa_ctxt->p_call_obj = p_call_obj;
                
                if (Sdf_en_protoTcp == protocol
#ifdef ICF_SECURE_SUPPORT
		        || Sdf_en_protoTls == protocol)
#else
		        )
#endif
                {
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
#ifdef ICF_TRACE_ENABLE
		ssa_pdb.p_buf = (icf_uint8_t *)p_current_sip_pdu;
        ssa_pdb.len = p_nw_int_info->pdu_len;
#endif
				icf_print_pdu(p_glb_pdb, (icf_uint8_t *)p_current_sip_pdu,
                              p_nw_int_info->remote_ip_addr,
                              p_nw_int_info->remote_port_num,
                              p_nw_int_info->pdu_len);
                /*Fix for TCP : SPR 14954*/ 
 
                /*Check for the Message Length*/ 
                /*Fix for TCP : SPR 14954*/

                ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Invoking sdf_ivk_uaDecodeMessage with "\
							"p_nw_int_info->pdu_len = %d\n", \
							p_nw_int_info->pdu_len));
                check_option = 0;
                sdf_retval = 0;

                /* Fix for CSR 1-7155416: SPR 19707 */
		    	/* Before UATK decode, IPTK will also validate the header
			     * length in incoming message. This shall be handled as 
    			 * follows:
	    		 * (1) Invoke IPTK decoder.
		    	 * (2) Even if above function returns some failure, 
			     *     invoke UATK decoder. This will provide a handle on the
    			 *     SIP msg for which failure response needs to be sent.
	    		 * (3) If UATK decoder returns failure, logic already
		    	 *     exists to send 4xx wherever applicable.
			     * (4) If UATK decode succeeds but IPTK decode had failed,
    			 *     use existing logic to send 4xx for these cases also.
	    		 */

                /*CSR1-5718212 */
                /* Codenomicon fixes: CSR 1-7132323: SPR 19484/19571 */
            mesg_length_status = 
                sdf_ivk_uaIPTKCheckMessageLength(p_current_sip_pdu,
                        ICF_MAX_STR_LEN,ICF_MAX_LARGE_STR_LEN,
                        &check_option,(Sdf_ty_IPTKmessageLengthValidity *) &sdf_retval, &sdf_error);
/* Message body need not to be checked as message body can contain XML data
 * too */
            if((sdf_retval & Sdf_en_reqresplinelong) == Sdf_en_reqresplinelong)
            {
                /* send 414 here */
                p_ssa_ctxt->bitmask_1 |= ICF_REQ_URI_TOO_LONG;
            }
            if (mesg_length_status == Sdf_co_fail)
            {
                ICF_PRINT(((icf_uint8_t *)"[SSA:] CheckIPTKMessagelength returns error\n"));
            }
            /* Codenomicon fixes: CSR 1-7132323: SPR 19484/19571 */
            else if (((sdf_retval & Sdf_en_headervaluelong) == Sdf_en_headervaluelong) &&
					 ((sdf_retval & Sdf_en_headernamelong) == Sdf_en_headernamelong))
            {
                /* Fix for CSR 1-7155416: SPR 19707 */
				iptk_decode_failed = ICF_TRUE;
                ICF_PRINT(((icf_uint8_t *)"[SSA]: Total header length is very long\n"));
            }
            else if ((sdf_retval & Sdf_en_headernamelong) == Sdf_en_headernamelong)
            {
                /* Fix for CSR 1-7155416: SPR 19707 */
				iptk_decode_failed = ICF_TRUE;
                ICF_PRINT(((icf_uint8_t *)"[SSA]: Header name is too long\n"));
            }
            else if ((sdf_retval & Sdf_en_headervaluelong) == Sdf_en_headervaluelong)
            {
                /* Fix for CSR 1-7155416: SPR 19707 */
				iptk_decode_failed = ICF_TRUE;
                ICF_PRINT(((icf_uint8_t *)"[SSA]: Header value is either missing or too long\n"));
            }
            else if ((sdf_retval & Sdf_en_mandatoryhdrlong) == Sdf_en_mandatoryhdrlong)
            {
                ICF_PRINT(((icf_uint8_t *)"[SSA]: Mandatory header is very long\n"));
            }
            else if ((sdf_retval & Sdf_en_optionalhdrlong) == Sdf_en_optionalhdrlong)
            {
                ICF_PRINT(((icf_uint8_t *)"[SSA]: Optional header is very long\n"));
            }
            /* Fix for CSR 1-7155416: SPR 19707 */
			/* Even if IPTK length validation fails, invoke UATK decode function.
			 * This will be required to send 4xx response for failed requests.
			 */
               
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

                /* Fix for CSR 1-7155416: SPR 19707 */
				/* Even if UATK decode succeeds, check if IPTK's
				 * validation succeeded or not. If it failed,
				 * handling for sending 4xx will be similar as in
				 * case of UATK decode failure. If UATK decode has also failed,
				 * then failure response will be sent from UATK decode fail
				 * code leg in switch case.
				 */
				if ((ICF_FAILURE != ret_val) && 
					(ICF_TRUE == iptk_decode_failed) &&
					(Sdf_en_fail != msg_type))
				{
                	ret_val = icf_ssa_handle_decode_failure(
                                    &ssa_pdb,
                                    p_event_ctxt, 
                                    p_call_obj, 
                                    &sdf_error, 
                                    p_ecode,
									ICF_TRUE, (Sdf_ty_u16bit)400);
				}
                else if(ICF_FAILURE != ret_val)
                {
                    if(Sdf_en_versionNotSupported == sdf_error.errCode)
                    {
                        msg_type = Sdf_en_success;
                    }
                    /* Fix for SPR 19388:
                       If the error code returned is Sdf_en_tooManyViaHdrsInResponse,
                       set msg_type as Sdf_en_fail. This is to ensure that the function
                       icf_ssa_handle_decode_failure is called. In that we will
                       issue a generic call clear towards CC as the response received
                       has more than 1 Via headers (This is a fix for ETSI conformance case
                       SIP_CC_OE_CE_V_015.
                    */   
                    else if (Sdf_en_tooManyViaHdrsInResponse == sdf_error.errCode)
                    {
                        msg_type = Sdf_en_fail;
                    }
                    /* End fix for SPR 19388 */
                    /*  Fix for SPR 20172 */
                    /*  Check that if ICF_CALL_CONFIG_ENABLE_STRICT_PARSING is
                     *  TRUE then check the Bad header list returned by UATK
                     *  if any BAD header is received then send 400.
                     */
                    if(ICF_CALL_CONFIG_HANDLE_NON_MANDATORY_MALFORMED_HEADER !=
                       (ICF_CALL_CONFIG_HANDLE_NON_MANDATORY_MALFORMED_HEADER &
                             ssa_pdb.p_glb_cfg_data->default_settings) &&
                            (Sdf_en_success == msg_type))
                    {
                        if ((Sdf_co_null != p_call_obj->pUacTransaction) &&
                                (Sdf_co_null != p_call_obj->pUacTransaction->pSipMsg))
                        {
                            p_temp_msg = p_call_obj->pUacTransaction->pSipMsg;
                        }
                        else if(Sdf_co_null != p_call_obj->pUasTransaction) 
                        {
                            p_temp_msg = p_call_obj->pUasTransaction->pSipMsg;
                        }
                        if(SipFail == sip_getBadHeaderCount(p_temp_msg, &dCount, \
                                    (SipError*) &(sdf_error.stkErrCode)))
                        {
                            ret_val = ICF_FAILURE;
                            if ((ICF_NULL != p_call_obj->pUasTransaction) &&
                                (ICF_NULL !=
                                p_call_obj->pUasTransaction->pTxnKey))
                            {
                                icf_ssa_free_sip_txn_data_on_failure (
                                          p_call_obj->pUasTransaction->pTxnKey);
                            }
                        }
                        /*  If bad headers are present then send 400 */
                        else if (0 < dCount)
                        {
                            msg_type = Sdf_en_fail;
                        }
                    }
                    
                if (ICF_FAILURE != ret_val)
                {

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

                                /* Fix for CSR 1-7586127 : SPR-19988*/
                                /* get the config data pointer from DBM */
                                if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                                            ssa_pdb.p_glb_pdb,
                                            (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                            (icf_void_t *)&(ssa_pdb.p_glb_cfg_data),
                                            ssa_pdb.p_ecode))
                                {
                                       ret_val = ICF_FAILURE;
                                       ICF_FUNCTION_EXIT(ssa_pdb.p_glb_pdb)
                                       return ret_val;
                                }
                                /* If application has set the bitmask for
                                 * strict validation, Match the CANCEL request
                                 * transaction with INVITE based on Via
                                 * header if received in Sip Message
                                 */
                                /* SPR 19993 fixed: The below condition should
                                 * only be checked if the function sdf_ivk_uaIPTKCheckMessageLength
                                 * is not returning failure i.e. the iptk_decode_failed
                                 * is not set as TRUE.
                                 */
                                if ((ICF_FALSE == iptk_decode_failed) &&
                                    (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING ==
                                              (ICF_CALL_CONFIG_ENABLE_STRICT_PARSING &
                                                  ssa_pdb.p_glb_cfg_data->default_settings)) &&
                                    (ICF_SUCCESS == icf_ssa_match_via_and_clear(&ssa_pdb, p_call_obj)))
                                {
                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: CANCEL is matched with \
                                                 INVITE transaction based on via key\n"));
                                    /* Fix for SPR 19995 */
                                    /* Memory in use for txn resolved here */
                                    if ((ICF_NULL !=
                                        p_call_obj->pUasTransaction) &&
                                        (ICF_NULL !=
                                        p_call_obj->pUasTransaction->pTxnKey))
                                    {    
                                        icf_ssa_free_sip_txn_data_on_failure(
                                                p_call_obj->pUasTransaction->pTxnKey);
                                    }
                                }
                                                                
                                /* Fix for SPR 17518: Handling of Error Code 
                                 * Sdf_en_sipIncomplete
                                 */
                                else if (((Sdf_en_maybeIncomplete == sdf_error.stkErrCode) ||
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
                                    /***Changes for SPR-18503 start ****/
                                    /*Allocate the memory to p_pending pdu if 
                                      is null.It is used to buffer the
                                      incomplete message received on socket.*/
                                  if ( ssa_pdb.p_conn_table_entry && (ICF_NULL ==
                                      ssa_pdb.p_conn_table_entry->p_pending_pdu))
                                  {
                                     ICF_MEMGET(
                                         p_glb_pdb,
                                         (p_nw_int_info->pdu_len + 1),
                                         ICF_MEM_COMMON,
                                         ssa_pdb.p_conn_table_entry->p_pending_pdu,
                                         ICF_DONOT_RET_ON_FAILURE,
                                         ssa_pdb.p_ecode,
                                         ret_val)

                                     /*Klocwork Warning Removal*/
						 		     ICF_KLOCWORK_FALSE_POSITIVE(
                                     p_persistent_glb_pdb,
			                         ssa_pdb.p_conn_table_entry->p_pending_pdu)

                                     /*Buffer the incomplete message in
                                      *ssa_pdb.p_conn_table_entry->p_pending_pdu*/
                                     if (ICF_SUCCESS == ret_val)
                                     {
                                       icf_port_memcpy(
                                           ssa_pdb.p_conn_table_entry-> \
                                                      p_pending_pdu,
                                           p_current_sip_pdu,
                                           (p_nw_int_info->pdu_len));

                                       /*Also store the length of message 
                                        *stored in p_conn_table_entry->p_pending_pdu*/
                                       ssa_pdb.p_conn_table_entry->
                                                       pending_pdu_length =
                                       (icf_uint16_t)(p_nw_int_info->pdu_len);
                                     }
                                  }
                                     /***Changes for SPR-18503 end ****/

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
				                    if(processed_bytes 
                                                 == ICF_MAX_TCP_MSG_LNGTH)
                                    {
                                       /*Drop the message buffered in 
                                        p_pending_pdu.*/
                                      
                                        ICF_PRINT(((icf_uint8_t*)"\nSince message length \
                                        equals to ICF_MAX_TCP_MSG_LNGTH but \
                                        incomplete.It has been dropped\n"))

                                        if( ssa_pdb.p_conn_table_entry && (ICF_NULL != 
                                           ssa_pdb.p_conn_table_entry->p_pending_pdu))
                                        {
                                          ICF_MEMFREE(p_glb_pdb,
                                             ssa_pdb.p_conn_table_entry->p_pending_pdu,
                                             ICF_MEM_COMMON,
                                             ssa_pdb.p_ecode,dummy_ret_val)
                                          ssa_pdb.p_conn_table_entry->
                                                  pending_pdu_length = 0;
                                        }
                                    }
                                }
                                 /*Free Txn Key and buffer when decode 
                                   failure is because of some error in parsing
                                   of basic headers.For these two errors we do not send 400 
                                   bad request on the network and so icf_ssa_handle_decode_failure
                                   is not called.For all other error codes we send 400 bad request
                                   on the  network */
                                else if ((Sdf_en_headerManipulationError == sdf_error.errCode) &&
                                         (Sdf_en_maybeIncomplete != sdf_error.stkErrCode) &&
                                         (Sdf_en_incomplete != sdf_error.stkErrCode) &&
                                         (Sdf_en_sipIncomplete != sdf_error.stkErrCode) &&
                                         (Sdf_en_parserError != sdf_error.stkErrCode))

                                {
                                    ICF_PRINT(((icf_uint8_t*)"Header/protocol mismatch error\n"));
                                    if (p_call_obj && p_call_obj->pUasTransaction )
                                    {
									        icf_ssa_free_sip_txn_data_on_failure(
                                            p_call_obj->pUasTransaction->pTxnKey);
									}
                                }
                                /* SPR 18906: In case of parser error , new function is 
                                 * called that either sends trigger to CC or frees the
                                 * transaction.
                                 */ 
                                else if((Sdf_en_parserError == sdf_error.errCode) &&
                                        (Sdf_en_maybeIncomplete != sdf_error.stkErrCode) &&
                                        (Sdf_en_incomplete != sdf_error.stkErrCode) &&
                                        (Sdf_en_sipIncomplete != sdf_error.stkErrCode) &&
                                        (Sdf_en_parserError != sdf_error.stkErrCode))
                                {
                                    ICF_PRINT(((icf_uint8_t*)"Header/protocol mismatch error\n"));
                                     ret_val = icf_ssa_handle_parser_error(&ssa_pdb,
                                             p_call_obj);
                                }
                                else
                                {
                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Msg decode fail \n"));
                                    if (p_call_obj) { 
										ret_val = icf_ssa_handle_decode_failure(
                                        &ssa_pdb,
                                        p_event_ctxt, 
                                        p_call_obj, 
                                        &sdf_error, 
                                        p_ecode,
                                        ICF_FALSE, 
                                        ICF_NULL);
									}
                                    /*Klocwork Warning Removal*/
                                    ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_call_obj)                                  
                                    /* if this bitmask is set then send the failure to internal
                                    * IPTK modules */
                                   if ( ICF_SSA_INTER_MODULAR_MSG & 
                                            ssa_pdb.common_bitmask)
                                   {
                                       ret_val = icf_ssa_process_otg_intrnl_msg(
                                            &ssa_pdb);
                                   }  
                                }
                            }
                            break;
                        case Sdf_en_strayMessage:
                            {
                                ICF_PRINT(((icf_uint8_t *)"[SSA] SIP MESSAGE IS A STRAY MESSAGE\n"));
                                /*Add Informational trace here for stray msg*/
                                if (p_call_obj && ((Sdf_co_null !=
                                         p_call_obj->pUasTransaction) &&
                                    (Sdf_co_null != 
                                         p_call_obj->pUasTransaction->pSipMsg)))
                                {
                                    ret_val = icf_ssa_handle_stray_msg(
                                        &ssa_pdb,
                                        p_event_ctxt, 
                                        p_call_obj, 
                                        &sdf_error, 
                                        p_ecode);
                                    /*Klocwork Warning Removal*/
                                    ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_call_obj)                                   
                                }
			                }
			                break;
                        default:/*Assumed this to be successful decode*/
                            {
                                icf_boolean_t         send_fail_flag = ICF_FALSE;
                                SipMessage            *pMessage = ICF_NULL;
                                SipHeader              dHeader = {SipHdrTypeAny,SIP_NULL};
                                SipError            error = 0;
                                Sdf_ty_s8bit        *pMediaType = ICF_NULL;

                                if ( p_call_obj && (Sdf_en_msgbodylinelong == 
                                        (sdf_retval & Sdf_en_msgbodylinelong)))
                                {
                                     /*SPR 18864 Added null check for pUacTransaction*/ 
                                    if((Sdf_co_null != p_call_obj->pUacTransaction) &&
                                       (Sdf_co_null != p_call_obj->pUacTransaction->pSipMsg))
                                    {
                                        pMessage = p_call_obj->pUacTransaction->pSipMsg;
                                    }    
                                    else if(Sdf_co_null != p_call_obj->pUasTransaction)
                                    {    
                                        pMessage = p_call_obj->pUasTransaction->pSipMsg;
                                    }    
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
                           /*****SPR-18503 start*********/
                            if((Sdf_en_protoTcp == protocol) 
#ifdef ICF_SECURE_SUPPORT				
         		    || (Sdf_en_protoTls == protocol)
#endif
                              )
                            { 

                                /*Free allocated memory to pointer 
                                  p_pending_pdu which is allocated
                                  in case of msg_incomplete*/

                                  if( ssa_pdb.p_conn_table_entry && (ICF_NULL != 
                                    ssa_pdb.p_conn_table_entry->p_pending_pdu))
                                   {
                                    ICF_MEMFREE(ssa_pdb.p_glb_pdb,
                                    ssa_pdb.p_conn_table_entry->p_pending_pdu,
                                    ICF_MEM_COMMON,
                                    ssa_pdb.p_ecode, dummy_ret_val)
                                    ssa_pdb.p_conn_table_entry->
                                               pending_pdu_length = 0;
                                    }
                             } 
                            /*****SPR-18503 end*********************/
                                    ICF_MEMFREE(p_glb_pdb,
                                            p_internal_msg_payload,
                                            ICF_MEM_COMMON,
                                            ssa_pdb.p_ecode,return_val)
                                    p_internal_msg->p_msg_data = ICF_NULL;    
                                    ret_val = ICF_FAILURE;
                                    ICF_FUNCTION_EXIT(p_glb_pdb)
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
                                            p_ecode,
                                            ICF_FALSE,
                                            ICF_NULL);
                                    /*Klocwork Warning Removal*/
                                    ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_call_obj)                                   
                                }
                                else
                                {
                                    ICF_PRINT(((icf_uint8_t *)"[SSA] SIP MESSAGE SUCCESSFULLY DECODED\n"));

#ifdef ICF_UT_TEST
#if 0
              /* CSR_1-6501937 and 1-6527624 Testing Start */
                 SipMessage    *dest;
                 SipError		dErr;
                 if((Sdf_co_null != p_call_obj->pUasTransaction) &&
                    (Sdf_co_null != p_call_obj->pUasTransaction->pSipMsg)) 
                 {
                 sip_initSipMessage(&dest, 
                         p_call_obj->pUasTransaction->pSipMsg->dType, &dErr);
                 if(SipFail ==sip_cloneSipMessage(dest,
                               p_call_obj->pUasTransaction->pSipMsg, &dErr))
                     (void)fast_memfree(NON_SPECIFIC_MEM_ID,dest,&dErr);
                 }
              /* CSR_1-6501937 1-6527624 Testing End */

#endif
#endif
                                    ret_val = icf_ssa_handle_successful_decode(
                                            &ssa_pdb,
                                            p_event_ctxt, 
                                            p_call_obj, 
                                            &sdf_error, 
                                            p_ecode);

                                     /*
                                    Fix for FMR in case of TCP call
                                    We would call this macro only if the flag ICF_SSA_TRACE_REQUIRED is
                                    not set.This flag in ssa pdb ensures that the trace on the receipt on
                                    200 OK for a BYE is sent via the call back sdf_cbk_uaCallTerminated.In
                                    other scenarios the trace is sent from here.
                                    */
                                    /* Fix for SPR: 19886 (Merged SPR:19534)*/
                                    /* Fix for SPR 19534 */
                                    /* Also check whether the CallObj & SSA ctxt have been 
                                     * freed or not. This is required to avoid a case
                                     * where these have already freed but ssa_pdb still 
                                     * contains non-null pointer to SSA ctxt leading to FMRs.
                                     */
                                    if ((!(ssa_pdb.common_bitmask & ICF_SSA_TRACE_REQUIRED)) &&
                                            (ICF_SSA_CALL_OBJ_ALREADY_FREED != 
                                             (ICF_SSA_CALL_OBJ_ALREADY_FREED & ssa_pdb.common_bitmask)))
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
                }
                    
                }/* End to if ret_val is success */
                else
                {
                    
                }
                ICF_MEMFREE(p_glb_pdb, p_transp_addr, \
                        ICF_MEM_COMMON, p_ecode, dummy_ret_val)

                if (ICF_NULL != p_event_ctxt)
                {    
                    sdf_ivk_uaFreeEventContext(p_event_ctxt);
                }    
                if (!(ssa_pdb.common_bitmask & ICF_SSA_CALL_OBJ_ALREADY_FREED))
                   sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
        }
        if ((ICF_NULL != p_next_msg) && (Sdf_en_protoTcp == protocol
#ifdef ICF_SECURE_SUPPORT
               || Sdf_en_protoTls == protocol))
#else
           ))
#endif
        {
			if (ICF_NULL ==  ssa_pdb.p_conn_table_entry)
			{
            	ret_val = ICF_FAILURE;
                if (ICF_NULL != p_call_obj)
                {
                    sdf_ivk_uaFreeCallObject(&p_call_obj);
                }
            	ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Error!!FD not found in ConnDB"));
                ICF_MEMFREE(p_glb_pdb,
                        p_internal_msg_payload,
                        ICF_MEM_COMMON,
                        ssa_pdb.p_ecode,return_val)
                p_internal_msg->p_msg_data = ICF_NULL;                
            	ICF_FUNCTION_EXIT(p_glb_pdb)
                return ret_val;
			}
            else
            {
              /* Added changes to handle scneraio where more than one
               * Messages are received in message*/

                if(0 != ssa_pdb.p_conn_table_entry->pending_pdu_length)
                {
                  p_nw_int_info->pdu_len = 
                  (icf_uint16_t)(ssa_pdb.p_conn_table_entry->pending_pdu_length
                                                       - processed_bytes);

                  ICF_MEMGET(ssa_pdb.p_glb_pdb,
                          p_nw_int_info->pdu_len + 1,
                          ICF_MEM_COMMON,
                          p_local_pending_pdu,
                          ICF_DONOT_RET_ON_FAILURE,
                          ssa_pdb.p_ecode,ret_val)
                  if(ICF_SUCCESS == ret_val)
                  {
                    icf_port_memcpy(p_local_pending_pdu,
                                    p_next_msg,
                                    p_nw_int_info->pdu_len);

                    ICF_MEMFREE(p_glb_pdb,
                           ssa_pdb.p_conn_table_entry->p_pending_pdu,
                           ICF_MEM_COMMON,
                           ssa_pdb.p_ecode,dummy_ret_val)
                  }
                p_current_sip_pdu = p_local_pending_pdu;
                ssa_pdb.p_conn_table_entry->p_pending_pdu = p_local_pending_pdu;
                ssa_pdb.p_conn_table_entry->pending_pdu_length = 
                                                       p_nw_int_info->pdu_len ;
                ICF_MEMFREE(p_glb_pdb, p_next_msg, \
                                       ICF_MEM_COMMON, p_ecode, dummy_ret_val)

                ICF_PRINT(((icf_uint8_t *)"[SSA]:FreedPduBufferAllocatedByStack\n"));
                }
                else
                {
                  ICF_MEMFREE(p_glb_pdb, p_next_msg, \
                                       ICF_MEM_COMMON, p_ecode, dummy_ret_val)
                  p_current_sip_pdu += processed_bytes;
                  p_nw_int_info->pdu_len = 
                     (icf_uint16_t)(p_nw_int_info->pdu_len - processed_bytes);
                } 
                more_messages = ICF_TRUE;
            }
        }
        else
        { 
                 more_messages = ICF_FALSE;
        }
    }while((ICF_TRUE == more_messages) && (ICF_SUCCESS == ret_val));

  if((Sdf_en_success == msg_type) && (Sdf_en_protoTcp == protocol
#ifdef ICF_SECURE_SUPPORT
               || Sdf_en_protoTls == protocol))
#else
    ))
#endif  
   {
      /*SPR 18554:Null check added to avoid crash*/ 
      if((ICF_NULL != ssa_pdb.p_conn_table_entry) &&
              (ICF_NULL != ssa_pdb.p_conn_table_entry->p_pending_pdu))
      { 
          ICF_MEMFREE(p_glb_pdb,
                           ssa_pdb.p_conn_table_entry->p_pending_pdu,
                           ICF_MEM_COMMON,
                           ssa_pdb.p_ecode,dummy_ret_val)
          ssa_pdb.p_conn_table_entry->pending_pdu_length = 0;
      }
    }

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
    
    if(ICF_FAILURE == ret_val)
	{
          /* Free the memory earlier allocated to
             p_conn_table_entry->p_pending_pdu*/
          if((ICF_NULL != ssa_pdb.p_conn_table_entry) &&
              (ICF_NULL != ssa_pdb.p_conn_table_entry->p_pending_pdu))
           {
             ICF_MEMFREE(p_glb_pdb,
                         ssa_pdb.p_conn_table_entry->p_pending_pdu,
                         ICF_MEM_COMMON,
                         ssa_pdb.p_ecode,dummy_ret_val)
             ssa_pdb.p_conn_table_entry->pending_pdu_length = 0;
           }
    }
    ICF_MEMFREE(p_glb_pdb,
            p_internal_msg_payload,
            ICF_MEM_COMMON,
            ssa_pdb.p_ecode,return_val)
    p_internal_msg->p_msg_data = ICF_NULL;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}
