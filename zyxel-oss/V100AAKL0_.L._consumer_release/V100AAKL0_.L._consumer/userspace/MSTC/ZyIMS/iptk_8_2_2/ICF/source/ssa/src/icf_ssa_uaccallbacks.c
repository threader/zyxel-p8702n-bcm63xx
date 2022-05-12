/*****************************************************************************
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
 ** 23-Jan-2008    Rajiv Kumar      ICF 8.0         Registration Procedure
 **                                                 Enhancements 
 ** 25-Jan-2008    Tarun Gupta     IPTK Rel 8.0 LLD Changes done for AKA handling.
 ** 25-Jan-2008    Abhishek Dhammawat Rel8.0        precondition additions 
 ** 10-Mar-2008    Alok Tiwari     Rel8.0           Modified function 
 **                         icf_ssa_pos_resp_transaction_Completed_for_register
 **                         to fetch the valid proxy address.
 ** 28-Mar-2008    Tarun Gupta     SPR 18311        Merged Publish-Message Enhancements
 ** 28-Mar-2008    Shruti Thakral  SPR 18316        Klocwork warning removal
 ** 02-Apr-2008    Tarun Gupta     SPR 18311        Klocwork warning removal
 ** 03-Apr-2008    Divij Agarwal   SPR 18338        Free call object in case of BYE 
 **                                                 failure
 ** 06-Apr-2008    Tarun Gupta     SPR 18355        Merged Windows Porting Changes
 ** 15-Apr-2008    Abhishek Dhammawat SPR18395      Modified
 **                                                 sdf_cbk_uaReInviteFailed
 ** 18-Apr-2008    Alok Tiwari     SPR 18456        Klocwork warning resolved
 ** 21-Apr-2008    Alok Tiwari     SPR 18456        Klocwork warning resolved
 ** 01-May-2008    Shruti Thakral  SPR 18316        Klocwork warning removal
 ** 02-May-2008    Shruti Thakral  SPR 18316        Klocwork warning removal
 ** 10-May-2008 Abhishek Dhammawat SPR 18523      modified
 **                                               sdf_cbk_uaInProgress
 ** 14-May-2008  Jagmohan Chauhan IPTK Rel 8.0   SPR 18551 fix. IPTK not compilable 
 **                                              when NAT,TRACES and SESSION TIMER Flags are 
 **                                              off and only SECURE SUPPORT is on.
 ** 27-May-2008    Tarun Gupta     SPR 18585     Modified icf_ssa_transaction_Completed_for_register       
 ** 28-May-2008    Tarun Gupta     SPR 18585     Modified sdf_cbk_uaTransactionCompleted
 ** 02-Jun-2008    Tarun Gupta     SPR 18585     Modified sdf_cbk_uaReInviteAccepted,icf_ssa_transaction_Completed_for_update and sdf_cbk_uaReInviteFailed
 ** 07-Jun-2008  Amit Sharma       IPTK 8.0 SRTP LLD      SRTP Enhancements
 ** 16-Jun-2008  Tarun Gupta       SPR 18585     Modified icf_ssa_transaction_Completed_for_update
 ** 18-Jun-2008  Anurag Khare      SPR 18684     Modified for Maddr handling
 ** 20-Jun-2008  Tarun Gupta       SPR 18688     Modified sdf_cbk_uaReInviteFailed
 ** 24-Jun-2008  Tarun Gupta       SPR 18697     Modified callbacks for stats handling
 ** 24-Jun-2008  Anurag Khare      SPR 18717     Modified function sdf_cbk_uaReInviteFailed
 ** 25-June-2008 Jagmohan Chauhan  SPR 18684     Merge CSR 1-6044134
 ** 26-June-2008 Jagmohan Chauhan  SPR 18684     Merge CSR 1-6072476
 ** 30-Jun-2008  Tarun Gupta       SPR 18579     Modified sdf_cbk_uaReInviteFailed
 ** 03-Jul-2008  Anurag Khare      SPR 18684     CSR 1-6044375 Merge 
 ** 04-July-2008 Anurag Khare      SPR 18777     Modified function sdf_cbk_uaTransactionCompleted 
 ** 07-July-2008 Anurag Khare      SPR 18684     CSR 1-6083896 Merge
 ** 10-Jul-2008 Abhishek Dhammawat SPR 18630     Modified
 **                                              sdf_cbk_uaTransactionCompleted
 ** 19-July-2008 Anuradha Gupta    SPR 18817     Modified function sdf_cbk_uaForkedRespReceived 
 ** 03-Nov-2008  Rajiv Kumar       SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                               19026)
 ** 04-Nov-2008 Abhishek Dhammawat SPR 19189     merging SPR 18846 from rel 7.1.2
 ** 04-Nov-2008 Abhishek Dhammawat SPR 19189     merging SPR 19015 from rel 7.1.2
 ** 06-Nov-2008 Tarun Gupta        SPR 19189     Merged Rel 7.1.2 SPR 19013
 ** 06-Nov-2008 Abhishek Dhammawat SPR 19189     merging SPR 18879 from rel 7.1.2
 ** 11-Nov-2008  Rajiv Kumar       SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              18986)
 ** 12-Nov-2008 Tarun Gupta        SPR 19189     Merged Rel 7.1.2 SPR 18872
 ** 17-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              18895)
 ** 17-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              19139)
 ** 18-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              19145) 
 ** 19-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              18918)  
 ** 21-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              19003)  
 ** 21-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              19022)  
 ** 24-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              18922)  
 ** 26-Nov-2008 Rajiv Kumar        SPR 19188     Rel 7.1.2 SPR merging (SPR
 **                                              19135)  
 ** 27-Nov-2008 Rajiv Kumar        SPR 19215     CSR-1-6250701 Merged 
 ** 27-Nov-2008 Abhishek Dhammawat SPR 19218     CSR 1-6340954 Merged
 *  28-Nov-2008 Tarun Gupta        SPR 19218     Merged CSR 1-6191604 : SPR 18834
 ** 01-Dec-2008 Rajiv Kumar        SPR 19215     CSR-1-6212448 Merged 
 ** 04-Dec-2008 Tarun Gupta        SPR 19218     Merged CSR 1-6178201
 ** 08-Dec-2008 Abhishek Dhammawat SPR 19223     Modified
 **                                              sdf_cbk_uaReInviteAccepted
 ** 08-Dec-2008 Anurag Khare       SPR 19218     CSR 1-6486039 Merge
 ** 12-Dec-2008 Rajiv Kumar        SPR 19218     Merged CSR 1-6240518
 ** 12-Dec-2008 Abhishek Dhammawat SPR 19218     Merged CSR 1-6307543
 ** 13-Dec-2008 Abhishek Dhammawat SPR 19218     Merged CSR 1-6132503
 ** 15-Dec-2008 Anurag Khare       SPR 19218     SPR 18901 Merged
 ** 15-Dec-2008 Abhishek Dhammawat SPR 19218     Merged CSR 1-6472378
 ** 15-Dec-2008 Anurag Khare       SPR 19218     CSR 1-6168806 Merged 
 ** 17-Dec-2008 Rajiv Kumar        SPR 19218     Resolving compilation issue
 **                                              on VXWORK
 ** 10-Feb-2009 Alok Tiwari        ICF Rel-8.1   slMatchedHeadersList is 
 **                                              initialized in function
 **                        icf_ssa_pos_resp_transaction_Completed_for_register
 **                                                within flag SDF_THREAD_SAFE  
 ** 13-Feb-2009 Kamal Ashraf       Rel 8.1       code added for invoking the APIs
 **                                              sip_getTypeFromContactHdr, 
 **                                              sip_getTypeFromContactParam,
 **                                              sip_getExpiresFromContactParam and
 **                                              sip_getExtensionAttrFromContactParam under 
 **                                              ICF_UT_TEST flag
 ** 03-Mar-2009 Anurag Khare      Rel 8.1        changes done in 
 **                                              icf_ssa_transaction_Completed_for_update
 **                                              for UT bug
 ** 20-Mar-2009 Anuradha Gupta       Rel 8.1     Modified sdf_cbk_uaCallFailed
 ** 21-Mar-2009 Rajiv Kumar          Rel 8.1     UT Defect fix for DNS CLIENT
 ** 23-Mar-2009 Alok Tiwari       Rel 8.1        UT Defect Fix for MULTIPLE
 ** 27-Mar-2009 Ashutosh Mohan    Rel 8.2        Modified
 **                                              icf_ssa_transaction_Completed_for_register
 **                                              sdf_cbk_uaCallRedirected
 **                                              Contact.   
 ** 30-Mar-2009 Anurag Khare      Rel 8.2        changs done for PRACK Enh
 ** 31-Mar-2009 Tarun Gupta       Rel 8.2        REFER/NOTIFY Enhancements
 ** 31-Mar-2009 Alok Tiwari       Rel 8.2        Function Modified:
 **                               icf_ssa_transaction_Completed_for_register
 ** 04-Apr-2009 Alok Tiwari       Rel 8.2        UT Defect Fixed.
 ** 07-Apr-2009 Abhishek Dhammawat SPR 19430     Modified
 **                                              sdf_cbk_uaTransactionCompleted
 ** 09-Apr-2009 Rajiv Kumar        SPR 19428     Merged CSR 1-6924198 
 ** 17-Apr-2009 Anuradha Gupta     SPR 19475     Modified sdf_cbk_uaCallFailed,
 **                                                 sdf_cbk_uaReInviteFailed ,
 **                                                 icf_ssa_transaction_Completed_for_update 
 ** 21-Apr-2009 Abhishek Dhammawat SPR 19476     Modified
 **                                              sdf_cbk_uaTransactionCompleted
 ** 29-Apr-2009 Anuradha Gupta     SPR 19535     Modified sdf_cbk_uaRPRReceived
 ** 29-Apr-2009 Rajiv Kumar        SPR 19537     Modified sdf_cbk_uaReInviteFailed
 **                                              and sdf_cbk_uaReInviteAccepted
 ** 30-Apr-2009 Rajiv Kumar        SPR 19531     Klocwork warning removal
 ** 2-May-2009 Abhishek Dhammawat  SPR 19571     Modified
 **                                              sdf_cbk_uaCallRedirected
 ** 18-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6347417 merging
 ** 20-May-2009 Kamal Ashraf       SPR 19672     CSR_1_7027627 Merged
 ** 21-May-2009 Ashutosh Mohan     CSR-1-7092663 Modified
 **                                              sdf_cbk_uaCallAccepted
 **                                              sdf_cbk_uaReInviteAccepted
 **                                              icf_ssa_transaction_Completed_for_update
 ** 22-May-2009 Kamal Ashraf       SPR 19672     CSR_1_6763883 Merged
 ** 25-May-2009 Rajiv Kumar        SPR 19666     Fix For SPR: 19666 
 ** 27-May-2009 Anuradha Gupta     SPR 19672     CSR 1-6727350 Merging, MIU issue resolved
 ** 27-May-2009 Kamal Ashraf       SPR 19672     CSR_1_7108373 Merged
 ** 29-May-2009 Kamal Ashraf       SPR 19672     CSR_1_7113139 Merged
 ** 10-Jun-2009 Kamal Ashraf       SPR 19590     Modified sdf_cbk_uaCallFailed, 
 **                                            sdf_cbk_uaCallAccepted,sdf_cbk_uaReInviteAccepted
 **                                              and sdf_cbk_uaCallRedirected 
 ** 11-Jun-2009 Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal
 ** 20-Jun-2009 Abhishek Dhammawat SPR 19590    Modified sdf_cbk_uaRPRReceived
 ** 20-Jun-2009 Anuradha Gupta     SPR 19763    Modified icf_ssa_transaction_Completed_for_update
 ** 07-Jul-2009 Tarun Gupta        SPR 19826    Modified icf_ssa_pos_resp_transaction_Completed_for_register
 ** 13-Jul-2009 Rajiv Kumar        IPTK Rel8.2  Openssl Removal
 ** 15-Jul-2009 Anuradha Gupta     SPR 19853    Modified 
 **                                             icf_ssa_pos_resp_transaction_Completed_for_register
 ** 27-Jul-2009 Tarun Gupta        SPR 19886    Merged CSR 1-7009954
 ** 30-Jul-2009 Tarun Gupta        SPR 19886    Merged CSR 1-7279855
 ** 02-Aug-2009 Rajiv Kumar        SPR 19886    Merged CSR 1-7444867
 ** 04-Aug-2009 Tarun Gupta        SPR 19886    Merged CSR 1-7208970
 ** 04-Aug-2009 Abhishek Dhammawat SPR 19886    Merged CSR 1-7208970
 ** 06-Aug-2009 Tarun Gupta        SPR 19886    Merged CSR 1-7239695
 ** 17-Aug-2009 Anuradha Gupta     SPR 19927    klocworks warning removal
 ** 03-Sep-2009 Rajiv Kumar        SPR 19999    Merged CSR 1-7615941 
 ** 07-Sep-2009 Rajiv Kumar        SPR 19999    Merged CSR 1-7559038
 ** 15-Sep-2009 Anuradha Gupta     SPR 19999    Merged CSR 1-7618644
 ** 16-Sep-2009 Rajiv Kumar        SPR 19999    Merged CSR 1-7615966
 ** 17-Sep-2009 Rajiv Kumar        SPR 19999    Merged CSR 1-7694021
 ** 07-Oct-2009 Rajiv Kumar        SPR 20110    Fix for SPR:20110
 ** 08-Oct-2009 Abhishek Dhammawat SPR 20063    Merged CSR 1-7507275
 ** 12-Oct-2009 Rajiv Kumar        SPR 20124    Fix for SPR:20124
 ** 24-Oct-2009 Abhishek Dhammawat SPR 20182    Modified sdf_cbk_uaCallFailed 
 ** 30-Oct-2009 Anuradha Gupta     SPR 20205    Modified sdf_cbk_uaCallFailed
 ** 16-Mar-2010 Alok Tiwari        SPR 20333    Modified 
 **                       icf_ssa_pos_resp_transaction_Completed_for_register
 ** 19-May-2010  Rohit Aggarwal	SPR 20401	Check for self addr per line when
 **											200 OK of Register is received
 ** 11-Jun-2010  Rohit Aggarwal	SPR 20428	Fetching of App data while handling
 **                                         subscription response
 ** 18-Jun-2010 Aman Aggarwal     SPR 20473     Fix for CSR 1-8648269
 ** 29-Apr-2010 Alok Tiwari       SPR 20473     Fix for CSR 1-8648269
 ** 25-Jan-2013 Pratishtha        Bug273        Fix for CSR 00024478
 *****************************************************************************
 **                Copyrights 2013, Aricent.
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
    /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
       /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
       if(ICF_NULL != p_ssa_pdb)
       {     
          ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
       }
       else
       {
           ICF_PRINT(((icf_uint8_t *)"\nSSA PDB is NULL.\n"))
           ret_val = Sdf_co_fail;
       }/*End: SPR 18846*/    
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = Sdf_co_fail;
    }/*End: SPR 18846*/    
    if(Sdf_co_fail == ret_val)
    {
        return ret_val;
    }

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
            /*  SPR 18064 Merge */
            if((401 == dRespCode) || (407 == dRespCode))     
            {           
                /*Check for Auth params*/         
                icf_ssa_fetch_auth_param_list(p_ssa_pdb,                  
                                            ppCallObj,ICF_NULL);                   
                if(ICF_NULL == p_ssa_ctxt->auth_params_list_size)    
                {               
                    p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask |=      
                                                   ICF_AUTH_PARAM_NOT_RCVD;                 
                  /*  CSR 1-6044375 Merge */
#ifdef ICF_ERROR_ENABLE
                    /* Indicate error to application in case invalid 401/407 
                     * response is received from remote */
                    p_ssa_pdb->resp_code = dRespCode;
                    ICF_SSA_SET_ECODE(p_ssa_pdb,"CANCEL",ICF_ERROR_FAILURE_RESPONSE)
#endif
                }      
            }
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
#ifdef ICF_SESSION_TIMER
    /*Fixed Merged For CSR_1_5449895*/
    Sdf_st_error          sdf_error;
    Sdf_ty_u32bit         dCount = 0;    
#endif

   icf_uint8_t             *p_reason_str = ICF_NULL;
#ifdef ICF_SESSION_TIMER
	SipHeader dTempHeader;
	SipOptions options;
    icf_nw_otg_session_refresh_req_st  session_refresh_req;
#endif
    Sdf_ty_slist         slMatchedHeadersList;
    SipHeader           *pRetryAfterHeader = Sdf_co_null;
    Sdf_st_headerList   *pRetryAfterHeaderList = Sdf_co_null;
    icf_boolean_t			reset_bitmask = ICF_TRUE;
    
    /* This variable is used to store the header type(WWW-Authenticate/
     * Proxy-Authenticate) on the basis of the challenge received(401/407).
     * This is passed to the function icf_ssa_extract_chalenge as an
     * input parameter.
     */
    en_HeaderType       header_type = 0;
    icf_list_st         *p_fallback_resp_code = ICF_NULL;
    icf_boolean_t       send_srtp_fallback = ICF_FALSE;
    /*SPR18846 Added protective null check for the pEventContext*/
    if(Sdf_co_null != pEventContext)
    {   
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    }   
   
    if (ICF_NULL == p_ssa_ctxt)
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Ctxt is NULL.\n"))
        return ICF_FAILURE;
    }
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    if ((ICF_NULL == p_ssa_pdb )||
        (ICF_NULL == p_ssa_pdb->p_glb_pdb) ||
        (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx)) 
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Pdb Fetched from SSA Ctxt is NULL.\n"))
        return ICF_FAILURE;
    }

    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

    /* Fix for SPR 15557: If call xontext is NULL that means call 
       has been forcefully cleared till now. So no need to do processing 
       for the call*/
    if(ICF_NULL == p_ssa_ctxt->p_call_ctxt)
    {
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
				((90 > (*ppCallObj)->pSessionTimer->dMinSe) 
                                /*  SPR 17993 Merge */                      
                                || (65535 < (*ppCallObj)->pSessionTimer->dMinSe)))
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
                /* To add security association headers*/
                else if(ICF_FAILURE == 
                      icf_ssa_form_and_insert_security_agreement_hdrs(
                           p_ssa_pdb,(icf_uint8_t *)"ACK",
                           (*ppCallObj)->pUacTransaction->pSipMsg))
                {
                    ret_val = ICF_FAILURE;
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
                if(sip_getTagCountFromToHdr((*ppCallObj)->pCommonInfo->pTo,\
                        &dCount,(SipError *)&(sdf_error.stkErrCode))==SipFail)
                {
                    ret_val = ICF_FAILURE;
                }
                if (dCount != 0)
                {
                    Sdf_ty_u32bit dIndex=0;
                    for(dIndex=0;dIndex<dCount;dIndex++)
                    {
                        if(sip_deleteTagAtIndexInToHdr((*ppCallObj)->pCommonInfo->pTo,
                                   dIndex,(SipError *)&(sdf_error.stkErrCode))==SipFail)
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                    }
                }
                              
                 /*  SPR 17993 Merge */
                 (*ppCallObj)->pSessionTimer->dSessionInterval = \
                     (*ppCallObj)->pSessionTimer->dMinSe;
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
            /*  SPR 18064 Merge */
            /* No Need to check return value as, we need to send ack
             * If we set failure in ret_val, nothing happens*/
             icf_ssa_fetch_auth_param_list(p_ssa_pdb,
                                ppCallObj,ICF_NULL);

             if(ICF_NULL == p_ssa_ctxt->auth_params_list_size)    
             {               
                  p_internal_msg_param->reason_code =
                         ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_RCVD;
             }      
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
            if(dRespCode  == 401)
            { 
                header_type =  SipHdrTypeWwwAuthenticate;
            }
            else
            {  
                header_type =  SipHdrTypeProxyAuthenticate;
            }
                
        if(ICF_NULL != p_ssa_ctxt->auth_params_list_size)
		    {
                icf_user_db_node_st    *p_user_db = ICF_NULL;
                /*Fix for SPR:19999 (Merged CSR 1-7559038 : SPR-19945)*/
                icf_uint8_t  search_bitmask = 0;

                search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;
                search_bitmask |= ICF_USER_DB_USE_CONTACT_URIS;

                /*SPR 20182 This was causing in INVITE not being sent
                  after receiving 401 as a fix the CSR 1-7615941 is merged*/ 
                 /* CSR 1-7615941 removed the check for the request
                 * URI failure because Even If user is not configured we do not
                 return failure and per line value will be used to match the Realm*/
                /* Fetch user_db node to access the realm 
                 * if configured per user*/
                icf_dbm_get_public_uri_node_from_uri_for_line(
                          p_ssa_pdb->p_glb_pdb,
                          p_ssa_pdb->p_glb_pdb->p_call_ctx->line_id,
                          &(p_ssa_pdb->p_glb_pdb->p_call_ctx->local_address),
	                      search_bitmask,
        	              &p_user_db,
                	      p_ssa_pdb->p_ecode);
                /*  SPR 18064 Merge */
                /* Fetch and store the old nonce in the ssa context */
                if(ICF_FAILURE == icf_ssa_extract_challenge( 
                                        p_ssa_pdb,
                                        header_type))
                {
                    ret_val = ICF_FAILURE;
                }
                /*CSR_1-7009954 Fixed*/

			    /* fetch line data and check if line has auth param configured,
			     * if yes set call auth as true */
			    /* Fetch Line data from DBM */
			    else if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
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
                    
                    /*CSR_1-7009954 Fixed*/
                    /* Compare the realm received in authentication param with
                     * locally configured realm (per line or per user)
                     */
 
                    if((ICF_TRUE == ICF_IS_BIT_SET(
                                p_ssa_pdb->p_line_data->line_data_received,
                                          ICF_CFG_LINE_DATA_REALM_RECD)) ||
                       ((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                              (p_user_db->flags & ICF_USER_DB_REALM_RECD))))
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

                                /*CSR_1-7009954 Fixed*/
                                /* Check if realm is configured per user then compare with
                                 * realm configured per user not with the realm value configured
                                 * per line.
                                 */
                                if((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                                              (p_user_db->flags & ICF_USER_DB_REALM_RECD)))                                
                                {
                                   if(0 != icf_port_strcmp(p_user_db->configured_realm.str,
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

                                else if(0 != icf_port_strcmp(p_ssa_pdb->p_line_data->configured_realm.str, 
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

        /*SSA will trigger SRTP fallback only when same is configured in call context*/
        if (ICF_TRUE == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->srtp_fallback_reqd)
        {
           /*Fetch SRTP fallback response codes configured by application from icf_config_data_st*/
           p_fallback_resp_code = p_ssa_pdb->p_glb_cfg_data->p_srtp_fallback_resp_codes;

             /*Check whether failure response code is one of the response codes
              *configured by the application for SRTP fallback*/
            while (ICF_NULL != p_fallback_resp_code)
            {
                if (ICF_NULL == p_fallback_resp_code->p_data)
                {
                     break;
                }
                else
                {
                    /*If response code received from NW is same as app configured then 
                     *trigger CC for SRTP fallback operation*/
                    if (dRespCode == *((icf_uint16_t*)p_fallback_resp_code->p_data))
                    {
                         send_srtp_fallback = ICF_TRUE;
                         break;
                    }
                }
                p_fallback_resp_code = p_fallback_resp_code->p_next;
            }
            if (ICF_TRUE == send_srtp_fallback)
            {
                /*Populate the internal message header */
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                      ICF_SRTP_FALLBACK_REQ;

                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;
    
                /*All messages to be sent to CC/SC are routed to the CM.
                 *CM decides the specific destination of these messages*/
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

                /*Added traces for SRTP Fallback invocation */
                ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_SRTP_FALLBACK_INVOCATION)

#ifdef ICF_STAT_ENABLE
                /*Added stats for SRTP Fallback */
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                   misc_stats.stats_srtp_fallback_done);
#endif     
                return Sdf_co_success;
            }
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

    /* The explanation of various conditions in the if block is:
     * 1. response code is not 422
     * 2. If response code is 422 and there is some failure in processing
     * 3. if response code is 422 and Session Timer Handling is disabled
     */      
    if((dRespCode != 422) || (dRespCode == 422 && ((ret_val == Sdf_co_fail) ||
                (ICF_FALSE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)))) 
    {
                
		ret_val = Sdf_co_success;
        /* Populate the internal message header */
        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CALL_REJECT;

        /*
         * Fill the appropiate reason cause 
         */	
        /* SPR 18064 Merge */	
        if ((ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_CFGRD != p_internal_msg_param->reason_code) &&
            (ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_RCVD != 
	                              p_internal_msg_param->reason_code)
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
/* CSR 1-6212448: SPR 18873: Send final failure response trace to application */
#ifdef ICF_TRACE_ENABLE
		p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE"; 
		p_ssa_pdb->resp_code = dRespCode;
		ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_FAILURE_RESP_RECVD)
#endif        
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

    /* populate the header list when BYE is received. */
    icf_ssa_populate_app_specific_hdrs(
			p_ssa_pdb,ICF_INT_MODULE_CC, 
			(icf_void_t*)( p_ssa_ctxt->p_call_ctxt),
			(*ppCallObj)->pUacTransaction->pSipMsg,
			p_ssa_pdb->p_ecode);

    /* populate the body list from the SIP message in case present */
    /* SPR 19590 : Start */
    icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,
                        (*ppCallObj)->pUacTransaction->pSipMsg,
                        SipMessageRequest); 
    /* SPR 19590 : End */

#ifdef ICF_ERROR_ENABLE
/*  SPR 18064 Merge */
/*  CSR 1-5685130 Merge */
    /* Fix for CSR 1-6132503 : SPR 18794 */
	/* This will happen in following cases:
	 * (1) Final failure response other than 401,407
     *     received
     * (2) 401/407 received without Auth header
	 */
    /* Fix for CSR 1-6472378 : SPR 19184 */
    /*Fix for CSR 1-7279855*/
	if(((401 != dRespCode) && (407 != dRespCode) && (422 != dRespCode))||
        (ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_RCVD == p_internal_msg_param->reason_code))
    {
          p_ssa_pdb->resp_code = dRespCode;
          ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_FAILURE_RESPONSE)
    }
    /* 
     * Authentication failure should be raised in case a challenge is recd and 
     * no authentication request was generated for the same.
     */
	/* This will happen in following cases:
     * (1) 401/407 received second time
     * (2) 401/407 received with mismatching params
	 */
    else if(((ICF_TRUE == reset_bitmask) ||
		    (ICF_REL_CAUSE_INVALID !=
             p_ssa_pdb->p_glb_pdb->p_call_ctx->release_cause)) && 
		     (401 == dRespCode || 407 == dRespCode))
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
  
    /* SPR 202025 : Additional fix required due to CSR-1-7694021 merge*/    
    /* Once the 200 response of outgoing INVITE request is received reset the
     * value of conn_not_to_close in connection table entry of FALSE if it is
     * currently set to TRUE. This is done because if after call connect 
     * UATK give the callback to close the connection same can be achieved in
     * function sdf_cbk_uaCloseSocket.Null check for p_conn_table_req_entry is
     * placed so that this code not executed when protocol is UDP.We have
     * checked the p_conn_table_req_entry only as for sending outgoing request
     * only p_conn_table_req_entry is filled.*/        
    if((ICF_NULL != p_ssa_ctxt->p_conn_table_req_entry) &&
            ICF_TRUE == p_ssa_ctxt->p_conn_table_req_entry->conn_not_to_close)    
    {
        p_ssa_ctxt->p_conn_table_req_entry->conn_not_to_close = ICF_FALSE;
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
    /*SPR18846 Added protective null check for the pEventContext*/
    if(Sdf_co_null != pEventContext) 
    {
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    }/*End SPR18846*/     
    
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
       ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
       return ICF_FAILURE;
    }  

    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    if (ICF_NULL == p_ssa_pdb)
    {
       ICF_PRINT(((icf_uint8_t *)"\nSSA PDB is NULL.\n"))
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
          ICF_PRINT(((icf_uint8_t *)"\nCall Ctx is NULL.\n"))
          return Sdf_co_success;
      }
      
      /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18895)*/
      /*SPR 18895: The problem was that line-id in SSA ctx is not
        set here and further when fetching line data using this line-id
        (65535) failure is returned. Due to ret_val failure 
        ICF_NW_INC_CONNECT_GENERIC_ERROR gets set in 
        p_internal_msg_param->bitmask leading to success in function
        icf_cc_validate_ssa_connect_failure and call clear. Therefore
        populate the correct line-id from call ctx in ssa ctx. */
      p_ssa_ctxt->line_id = 
          p_ssa_pdb->p_glb_pdb->p_call_ctx->line_id;      

#ifdef ICF_UT_TEST
     
/*IPTK REl 8.1*/
/*code added for invoking APIs sip_getTypeFromContactHdr,
  sip_getTypeFromContactParam, sip_getExpiresFromContactParam
  and sip_getExtensionAttrFromContactParam */ 

    SipContactParam      *pContactParam = Sdf_co_null;
    en_ContactParamsType pType;
    SipError             sip_err = 0;
    Sdf_ty_u32bit        param_count = 0, iterator = 0;
    SipHeader            contact_hdr = {SipHdrTypeAny,SIP_NULL};
    SipExpiresStruct     *pExpires   = Sdf_co_null;
    SIP_S8bit            *ppExtensionAttribute = Sdf_co_null;
    SIP_U32bit           Value;

    /* invoke this API to get the contact header in "contact" variable */
    if(SipFail == sip_getHeaderAtIndex((*ppCallObj)->pUacTransaction->pSipMsg,
                     SipHdrTypeContactNormal,&contact_hdr,0,&sip_err))
    {
        ret_val = Sdf_co_fail;
    }

    /* The value 16 depicts the following API under this condition will be invoked 
       when uatk_api_test API(used in script) contains the value of var1 as 16 */

    if(16 == p_ssa_pdb->p_glb_pdb->dVar1)
    {
        /* invoke API sip_getTypeFromContactHdr for getting the 
          type of contact form contact header, after successful return
          from this API "Value" will contain the type of the contact */ 

        if(SipFail == sip_getTypeFromContactHdr(&contact_hdr,
                     &Value,&sip_err))
            printf("Failure returned by API sip_getTypeFromContactHdr");
        else  
        {
            printf("Success returned by API sip_getTypeFromContactHdr");
            if(1 == Value)
                printf("Contact header type : SipContactHdrTypeNormal");
        }
    }   

    /* The value 12. 13 depicts the following API will be invoked when uatk_api_test API(used in script)
       contains the value of var1 as 12 or 13 */

    if(11 == p_ssa_pdb->p_glb_pdb->dVar1 || 12 == p_ssa_pdb->p_glb_pdb->dVar1 
                     || 13 == p_ssa_pdb->p_glb_pdb->dVar1)
    {
        /* Get the count for the contact param from contact header */
        if(SipFail == sip_getContactParamsCountFromContactHdr(
                         &contact_hdr, &param_count, &sip_err))
        {
            ICF_PRINT(((icf_uint8_t *)"No Params in Contact header present \n"));
                         sip_freeSipHeader(&contact_hdr);
        }
    }
    for(iterator = 0; iterator < param_count; iterator++)
    {
        /* Extract the contact param at a particular index from the
           contact header */
        if(SipFail == sip_getContactParamAtIndexFromContactHdr(
                         &contact_hdr, &pContactParam, iterator, &sip_err))
        {
            ICF_PRINT(((icf_uint8_t *)"No Param in Contact header list present \n"));
        }   
 
        /*  The value 11, 12 and 13 depicts the following API under this condition will be invoked 
            when uatk_api_test API(used in script) contains the value of var1 as 11,12 or 13 */
        if(11 == p_ssa_pdb->p_glb_pdb->dVar1 || 12 == p_ssa_pdb->p_glb_pdb->dVar1 
                                    || 13 == p_ssa_pdb->p_glb_pdb->dVar1)
        {
            /* invoke the API sip_getTypeFromContactParam for getting the type 
               from the contact param obtained from contact header */
            if(ICF_SUCCESS == sip_getTypeFromContactParam(pContactParam,&pType,&sip_err) )
            {
                if(SipCParamExpires == pType)
	            printf("\nContactParam of dType is = 1");
                if(SipCParamExtension == pType)
                    printf("\nContactParam of dType is = 3");

	        if(SipCParamExpires == pType && 12 == p_ssa_pdb->p_glb_pdb->dVar1)
	        {
#ifdef SIP_BY_REFERENCE
                    /* invoke the API sip_getExpiresFromContactParam for getting 
                       the expire in pExpire */
	            if(ICF_SUCCESS == sip_getExpiresFromContactParam(pContactParam,
                                     &pExpires,&sip_err))
#else
		    if(ICF_SUCCESS == sip_getExpiresFromContactParam(pContactParam,
					     pExpires,&sip_err))
#endif
                    {
			printf("Success returned by API sip_getExpiresFromContactParam");
                        printf("\nExpire value is %d",pExpires->u.dSec);
                        sip_freeSipCommonHeader(pExpires);
                    }
		    else 
			printf("failure returned by API sip_getExpiresFromContactParam");
                } 
                else if(SipCParamExtension == pType && 13 == p_ssa_pdb->p_glb_pdb->dVar1)
                {
                    /* invoke the API sip_getExtensionAttrFromContactParam for getting 
                       the extension attribute from the contact param in ppExtensionAttribute */ 
                    if(ICF_SUCCESS == sip_getExtensionAttrFromContactParam(pContactParam,
                               &ppExtensionAttribute,&sip_err))
                    { 
                        printf("Success returned by API sip_getExtensionAttrFromContactParam");
                        printf("\nExtension Attribute is %s", ppExtensionAttribute);
                        sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)&ppExtensionAttribute,pErr); 
                    }
                    else 
                        printf("failure returned by API sip_getExtensionAttrFromContactParam");
                }
            }
            else
                printf("Failure returned by API : sip_getTypeFromContactParam");
        }
    } 
    if(Sdf_co_null != pContactParam)
        sip_freeSipContactParam(pContactParam);
   /* if(Sdf_co_null != contact_hdr)*/
        sip_freeSipHeader(&contact_hdr);

/*IPTK REl 8.1 END*/
#endif


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
      /* CSR_1_7027627 Merged SPR 19672 Start */ 
      icf_ssa_is_update_present_in_allow_hdr((*ppCallObj)->pUacTransaction->pSipMsg,
                                p_ssa_pdb->p_glb_pdb,
                                &p_ssa_ctxt,
                                pErr);
      /* CSR_1_7027627 Merged SPR 19672 End */
#ifdef ICF_SESSION_TIMER
      if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
      { 
        if(sdf_ivk_uaGetRefresher(*ppCallObj,&dRefresher,pErr) != Sdf_co_fail){
        /* Fix for CSR 1-7239695: SPR 19638 */
		if ((Sdf_en_invalidRefesher == dRefresher) ||
			((*ppCallObj)->pSessionTimer->dSessionInterval <
					(*ppCallObj)->pSessionTimer->dMinSe))
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
             /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18895)*/
             if((dRefresher == Sdf_en_refresherLocal) ||  
			    ((ICF_NULL != p_ssa_pdb->p_line_data)&&
                 (ICF_SESSION_REFRESHER_UAC == 
			        p_ssa_pdb->p_line_data->session_timer_refresher) &&
			     (dRefresher == Sdf_en_refresherRemote)))
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

                        /* Fix for CSR 1-7092663 : If update is present in allow
                         * header then IPTK will use UPDATE as subsequent
                         * refresh method*/
                        icf_ssa_is_update_present_in_allow_hdr(
                            p_message,p_ssa_pdb->p_glb_pdb,
                            &p_ssa_ctxt,pErr);

			icf_ssa_set_refresher_method(p_message,
				p_ssa_pdb->p_glb_pdb,
				&p_ssa_ctxt,
				pErr);
	
            /* CSR 1-6178201 : Check for session refresh method in ssa context instead of 
			 * checking in session timer data
			 */	
			if(p_ssa_ctxt->refresher_method_type == Sdf_en_refresherInvite)
            {
				ICF_PRINT(((icf_uint8_t *)"\n [SSA] Subsequent session refresh request Method will be INVITE"));
			}
			else{
				ICF_PRINT(((icf_uint8_t *)"\n [SSA] Subsequent session refresh request Method will be UPDATE"));
			}
		}
		/* start the session timer if it is not started by UATK (if 
		 * 2xx response does not contain Session Expires Header then
		 * UATK will not start the session timer) */
          if( p_ssa_pdb->p_line_data && (dRefresher != Sdf_en_refresherNone || 
				ICF_SESSION_REFRESHER_NONE != 
				p_ssa_pdb->p_line_data->session_timer_refresher))
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
      /* CSR 1-6250701: If Session timer is disabled in IPTK then reset the values of 
       * Session timer in call object.
       */
      else if(ICF_FALSE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
      {
          (*ppCallObj)->pSessionTimer->dIsSupportedTimer = ICF_FALSE;
          (*ppCallObj)->pSessionTimer->dSessionInterval = 0;
          (*ppCallObj)->pSessionTimer->dMinSe = 0;
          (*ppCallObj)->pSessionTimer->dRefresher = Sdf_en_refresherNone;
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
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
             /*If its a precondition call and QOS parameters are not 
              *received in 200 OK response then send ICF_NW_INC_CONNECT
              *with GENERIC_ERROR bit set in internal msg so that CC can 
              *initiate call clear. Bit ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL is set by
              *icf_ssa_validate_qos_status_line function when either 
              *qos status lines are not received or invalid received*/
             /* SPR18282 Added check for bitmask 
                ICF_PRECONDITION_NEGOTIATION_DONE to avoid sending CANCEL if
                200OK for QOS UPDATE is received from peer*/
             if ((ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL & 
                p_ssa_pdb->p_ssa_ctxt->failure_bitmask) &&
                 (!(ICF_PRECONDITION_NEGOTIATION_DONE &
                    p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->common_bitmask_2)))
             {
                  /*Set the bit that some error has occured while 
	           *processing 200ok */
	          ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
			    ICF_NW_INC_CONNECT_GENERIC_ERROR)

		  p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CONNECT ;
                  p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                       ICF_PORT_SIZEOF(icf_nw_inc_connect_st);

                  p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                     p_ssa_pdb->p_glb_pdb;                           
                  p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                  ret_val = Sdf_co_success;

                 if(ICF_NULL != p_inc_media_sdp_info)
                 {
                     /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                     icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                         &p_inc_media_sdp_info);
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
                 ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                 return ret_val;
       	    }
ICF_CHECK_QOS_SUPPORT_END	
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
#ifdef ICF_TRACE_ENABLE            
			/* CSR 1-6240518: Set sdp true in ssa pdb */
			p_ssa_pdb->sdp_present= ICF_TRUE;  
#endif            
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
        /* CSR 1-6240518: Set sdp false in ssa pdb */
#ifdef ICF_TRACE_ENABLE          
        p_ssa_pdb->sdp_present= ICF_FALSE;     
#endif        
        /******* Changes for Forking & Multiple Answer Start (Rel7.0) ******/

        p_internal_msg_param->forked_resp_id = \
         ((icf_ssa_ctxt_st *)((*ppCallObj)->pAppData)->pData)->forked_resp_counter;

        /******** Changes for Forking & Multiple Answer End (Rel7.0) *******/
        
		ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SDP not received in 200 OK"));
		/* Changes done for Message Body Headers Start :
		 */
        /* SPR 19590 : Start */      
		if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
			    (*ppCallObj)->pUacTransaction->pSipMsg,\
			     /**ppCallObj*/SipMessageResponse))
        /* SPR 19590 : End */
		{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
                /*Klocwork Warning removal*/ 
               /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                                &p_inc_media_sdp_info);
				ret_val = ICF_FAILURE;
		}
		/* Changes done for Message Body Headers End :
		 */

    }


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

    /* SPR 18872: In case image line is received in 200 OK of call setup, 
     * generic call clear should be triggered.
     */ 
    if((ICF_NULL != p_inc_media_sdp_info) &&
       (ICF_NULL != p_inc_media_sdp_info->p_added_sdp_list))
    {
        ICF_SSA_SET_COMMON_BITMASK(p_internal_msg_param->bitmask,
			    ICF_NW_INC_CONNECT_GENERIC_ERROR)

		p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_CONNECT ;
	    p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
		    ICF_PORT_SIZEOF(icf_nw_inc_connect_st);

	    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                          &p_inc_media_sdp_info);
	    ret_val = ICF_SUCCESS;
    }
    else if(ICF_FAILURE != ret_val)
    {	
	    if(ICF_NULL != p_inc_media_sdp_info)
	    {
            p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
		    p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;
            /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19022)*/
            /*Free the memory allocated to 
              icf_ssa_inc_media_stream_info_st*/
            icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                    &p_inc_media_sdp_info);               
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
    /*Klocwork Warning removal*/
    if(ICF_FAILURE == ret_val)
    {
        /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
         icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                          &p_inc_media_sdp_info);
    }
    else if(ICF_NULL != p_inc_media_sdp_info)
    {
          p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
          p_inc_media_sdp_info->p_added_sdp_list = ICF_NULL;
          /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19022)*/
          /*Fix For SPR : 19022*/
          /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
          icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                  &p_inc_media_sdp_info);
     }
#ifdef ICF_STAT_ENABLE
    icf_ssa_invite_resp_stats(p_ssa_pdb,200,0);    
#endif
/* CSR 1-6240518: Send trace for 200 Ok for INVITE */
#ifdef ICF_TRACE_ENABLE
	p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE"; 
	p_ssa_pdb->resp_code = 200;
	ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_200_RECVD)
#endif    
    /* Fix done While merging of CSR-1-7694021*/    
    /* Once the 200 response of outgoing INVITE request is received reset the
     * value of conn_not_to_close in connection table entry of FALSE if it is
     * currently set to TRUE. This is done because if after call connect 
     * UATK give the callback to close the connection same can be achieved in
     * function sdf_cbk_uaCloseSocket.Null check for p_conn_table_req_entry is
     * placed so that this code not executed when protocol is UDP.We have
     * checked the p_conn_table_req_entry only as for sending outgoing request
     * only p_conn_table_req_entry is filled.*/        
    if((ICF_NULL != p_ssa_ctxt->p_conn_table_req_entry) &&
            ICF_TRUE == p_ssa_ctxt->p_conn_table_req_entry->conn_not_to_close)    
    {
        p_ssa_ctxt->p_conn_table_req_entry->conn_not_to_close = ICF_FALSE;
    }
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
    /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {   
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
       /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }
    else
    { 
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = Sdf_co_fail;
    }    
    if((Sdf_co_fail == ret_val) ||
       (ICF_NULL == p_ssa_pdb))
    {   
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context/PDB is NULL.\n"))
        return  Sdf_co_fail;
    }/*End SPR 18846*/      
    
    /* CSR 1-6044134: SPR 18684: If call context is NULL 
      then return failure. */
    if(ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx)
    {
        /* in case the call context is NULL then return from
         * here but free the call object as being done at the
         * end of the function else the call object will
         * not be freed.
         */ 
        if(ICF_FALSE == p_ssa_pdb->bye_rejected)
        {
            sdf_ivk_uaFreeCallObject(ppCallObj);
        }
        return Sdf_co_success;
    }

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
     /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
       /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }   
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = Sdf_co_fail;
    }    
    if((Sdf_co_fail == ret_val) ||
       (ICF_NULL == p_ssa_pdb))
    {   
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context/PDB is NULL.\n"))
        return Sdf_co_success;
    }/*End SPR 18846 */    

#ifdef ICF_TRACE_ENABLE
    p_ssa_pdb->sdp_present = ICF_FALSE;
#endif
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
       /*SPR18523 : To avoid crash when call ctx is cleared 
         and 180/183 is received for the  call */
       /*SPR18846 Moved this check above to avoid trigger CM
        when call ctx is null */
       if(ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx)
       {
          return Sdf_co_success;
       }
       /* CSR 1-6212448:Send provisional response traces to application */
#ifdef ICF_TRACE_ENABLE
       p_ssa_pdb->resp_code = dRespCode;  
       p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE";
#endif       
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
        if((ICF_SSA_PRECONDITION_PROV_WITHOUT_100REL == 
           (p_ssa_pdb->p_ssa_ctxt->bitmask_1 & 
            ICF_SSA_PRECONDITION_PROV_WITHOUT_100REL))
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
                /* CSr 1-6212448: Send 100 Trying trace to application */
#ifdef ICF_TRACE_ENABLE
                p_ssa_pdb->sdp_present = ICF_FALSE;  
                ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_100_RECVD)
#endif                
                break;

            /* A 180 Ringing response is received */
            case 180    :
            /* A 183 Session Progress response is received */
            case 183    :
               if(ICF_PER_CALL_PRACK_FEATURE_ENABLED == 
               (p_ssa_ctxt->p_call_ctxt->common_bitmask_2 & ICF_PER_CALL_PRACK_FEATURE_ENABLED))
               {
                    /* It is the case when in outgoing INVITE is sent with
                     * 100rel in require header and
                     * non reliable 18x received
                     */
                    if(ICF_PRACK_IN_REQUIRE == p_ssa_ctxt->p_call_ctxt->prack_value)
                    {
                        p_ssa_ctxt->p_call_ctxt->release_cause =
                               ICF_CALL_CLEAR_REASON_NON_PRACK_CALL_NOT_ALLOWED;
                        p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                     ICF_GENERIC_CALL_CLEAR;
                        p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                        p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 0;
                        p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                        /* function exit trace */
                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) 
                        return ret_val;
                    }
               }
               /*  Rel 8.2 changes End */ 
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

                    if ((ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                            p_ssa_pdb,
                                            (*ppCallObj)->pUacTransaction->pSipMsg,
											msg_identifier, &p_inc_media_sdp_info,
                                            p_ssa_pdb->p_ecode)) &&
                            (ICF_NULL != p_ssa_pdb->p_ssa_ctxt))
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
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
                        /*If its a precondition call and QOS parameters are not 
                         *received in provisional response then send Generic call 
                         *clear to CC. Bit ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL is set by
                         *icf_ssa_validate_qos_status_line function when either 
                         *qos status lines are not received or invalid received*/
                        /* SPR18282 Added check for bitmask 
                           ICF_PRECONDITION_NEGOTIATION_DONE to avoid sending CANCEL if
                           200OK for QOS UPDATE is received from peer*/
                        if ( p_ssa_pdb->p_ssa_ctxt && ((ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL & 
                              p_ssa_pdb->p_ssa_ctxt->failure_bitmask) &&
                            (!(ICF_PRECONDITION_NEGOTIATION_DONE &
                               p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->common_bitmask_2))))
                    	{
                             p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                             p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                 ICF_GENERIC_CALL_CLEAR;
                             p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                                p_ssa_pdb->p_glb_pdb;                           
                             p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                             ret_val = Sdf_co_success;

                            if(ICF_NULL != p_inc_media_sdp_info)
                            {
                                /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                    &p_inc_media_sdp_info);
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
                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                            return ret_val;
                    	}
ICF_CHECK_QOS_SUPPORT_END
#ifdef ICF_TRACE_ENABLE    
                            p_ssa_pdb->sdp_present = ICF_TRUE;
#endif
                            if(dRespCode == 180)
                            {
                                if (p_inc_media_sdp_info) 
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
                                if (p_inc_media_sdp_info) 
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
                    if(ICF_FAILURE ==
                            icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
			        (*ppCallObj)->pUacTransaction->pSipMsg,\
			         /**ppCallObj*/SipMessageResponse))
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
                /* CSr 1-6212448: Send 180/183 trace to application */
#ifdef ICF_TRACE_ENABLE
				if(180 == dRespCode)
				{
					ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_180_RECVD)
				}
				else if (183 == dRespCode)
				{
					ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_183_RECVD)
				}

#endif
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
			/* CSR 1-6240518: Send provisional response trace to application */
#ifdef ICF_TRACE_ENABLE
				p_ssa_pdb->sdp_present = ICF_FALSE;
				p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE"; 
				p_ssa_pdb->resp_code = dRespCode; 
				ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_PROVISIONAL_RESP_RECVD)
#endif                
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
            if (p_inc_media_sdp_info) icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
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
    SipOptions                          options = {0};
    Sdf_ty_u32bit                       size = 0;
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_media_change_resp_st     *p_internal_msg_param = ICF_NULL;
#ifdef ICF_SESSION_TIMER
    icf_boolean_t                       send_ReInv = ICF_TRUE;
#endif 
#ifdef ICF_ERROR_ENABLE
/*  CSR 1-5685130 Merge */
    icf_boolean_t                       report_error_ind_sent = ICF_FALSE;
#endif
#ifdef ICF_SESSION_TIMER
	SipHeader	dTempHeader = {SipHdrTypeAny,SIP_NULL};
    icf_nw_otg_session_refresh_req_st   session_refresh_req = {0};
    icf_boolean_t						call_clear = ICF_FALSE;
    Sdf_ty_refresher                    dRefresher = Sdf_en_refresherNone;
#endif
    Sdf_ty_slist         slMatchedHeadersList;
    SipHeader           *pRetryAfterHeader = Sdf_co_null;
    Sdf_st_headerList   *pRetryAfterHeaderList = Sdf_co_null;

    /* This variable is used to store the header type(WWW-Authenticate/
     * Proxy-Authenticate) on the basis of the challenge received(401/407).
     * This is passed to the function icf_ssa_extract_chalenge as an
     * input parameter.
     */
    en_HeaderType       header_type = 0;
    icf_nw_otg_media_change_req_st	   media_chng_req  = {0,0,0,0};
    icf_return_t                       ret_value = ICF_SUCCESS;

    /*Start:SPR 18879:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
        /* Extract the pointer to SSA CTXT from the event context */
        p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
        /* Extract the pointer to SSA PDB from the event context */
        p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
        if(ICF_NULL == p_ssa_ctxt->p_call_ctxt)
        {
           ICF_PRINT(((icf_uint8_t *)"\nCall Context is NULL.\n"))
           return Sdf_co_fail;
        }
    }   
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        return Sdf_co_fail;
    }
    if((ICF_NULL == p_ssa_pdb)||
       (ICF_NULL == p_ssa_pdb->p_glb_pdb)||
       (ICF_NULL == p_ssa_pdb->p_internal_msg)||
       (ICF_NULL == p_ssa_pdb->p_internal_msg->p_msg_data)||
       (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA PDB/GLB PDB/Int Msg/Call ctx is NULL.\n"))
        return Sdf_co_fail;
    }/*End SPR 18879 */

    /*Fix For SPR:19531 Klocwork warning removal*/
    if (Sdf_co_null == (*ppCallObj)->pUacTransaction)
    {
        ICF_PRINT(((icf_uint8_t *)"\nUAC Transaction in callobject is NULL.\n"))
        return Sdf_co_fail;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* Cast the payload into the icf_nw_inc_media_change_resp_st structure */
    p_internal_msg_param = 
    (icf_nw_inc_media_change_resp_st *)p_ssa_pdb->p_internal_msg->p_msg_data;

    /* Initialise various fields of the internal message */
    p_internal_msg_param->bitmask = ICF_NULL;
    p_internal_msg_param->p_sdp_list = ICF_NULL;
    
    /* Fix For SPR: 19537*/
    /* Do not populate the application specific headers in call context if the
     * failure response is received for session refresh ReInvite.
     * Reason: In such scenario no indication is given to application and thus
     * header list populated here will never get free and subsequently these 
     * headers will be send in all outgoing request.*/
#ifdef ICF_SESSION_TIMER     
    if (ICF_FALSE == p_ssa_ctxt->session_timer_expired_refresh)
    {
#endif  
        /* populate app specific headers */ 
        if(ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                    p_ssa_pdb,ICF_INT_MODULE_CC,
                    (icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
                    (*ppCallObj)->pUacTransaction->pSipMsg,
                    p_ssa_pdb->p_ecode))
        {
            /*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/
            ret_val = ICF_FAILURE;
        } 
#ifdef ICF_SESSION_TIMER        
    }
#endif 
    
   if((ICF_FAILURE != ret_val) &&
           (Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                           (*ppCallObj)->pUacTransaction->pSipMsg,&dRespCode,pErr)))
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
                    else if ((Sdf_co_null != pRetryAfterHeaderList) &&
                             (Sdf_co_fail == sdf_listGetAt(
                               &(pRetryAfterHeaderList)->slHeaders,
                                0,
                                (Sdf_ty_pvoid *)&pRetryAfterHeader,
                                pErr)))
                    {
                        ret_val = Sdf_co_fail;
                    }
                    else
                    {
                        /*SPR 18879 : added null check for pRetryAfterHeader */
                        if((Sdf_co_null != pRetryAfterHeader) &&
                           (Sdf_co_null != pRetryAfterHeader->pHeader)) 
                        {
                           p_ssa_ctxt->p_call_ctxt->retry_after_duration =
                                 (((SipRetryAfterHeader *)
                                 (pRetryAfterHeader->pHeader))->u).dSec;
                        }   
                        sdf_listDeleteAll(&slMatchedHeadersList,pErr);
                    }
                 }
       }
       /*  Any response code other than 401 or 407 received. 
           then reset the bitmask
           SCenario when REInvite has been successfully 
          sent on network with authe headers. and 500 or any other failure 
          response received then this bitmask must be reset if it is set*/
       if (401 != dRespCode && 407 != dRespCode)
       {
           if (ICF_NULL != (ICF_CALL_AUTHENTICATION_REQ &
                              p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask))
           {

			  		/* Fix for SPR Bug273 */
               /* If any failure response code other than 401 and 407 of 
                * ReInvite is received and fax type is 
                * ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL then set the bit
                * ICF_CALL_CHALLENGE_T38_FAILURE_SWITCH_PT. This shall indicate
                * that the ReIvite sent for T.38 switch to PT should be send
                * with credentials.  
               */       
               if (ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL == 
                       p_ssa_pdb->p_glb_pdb->p_call_ctx->fax_type)
               {
                   ICF_SET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_3,
                           ICF_CALL_CHALLENGE_T38_FAILURE_SWITCH_PT);
               }
					
                ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
                                          ICF_CALL_AUTHENTICATION_REQ);

           }
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
				((90 > (*ppCallObj)->pSessionTimer->dMinSe) 
                                /*  SPR 17993 Merge */
                                || (65535 < (*ppCallObj)->pSessionTimer->dMinSe)))
			{
                /* SPR 18069: Call Clear when 422 received with 
			     * value of MinSE < 90. Treat 422 as a valid response 
			     * but set the status of Response as False,so that 
			     * in CC the call will be cleared.Bye will be sent.
			     */
			    call_clear = ICF_TRUE;
			    send_ReInv = ICF_FALSE;
				ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Min-SE in 422 is less than 90, Clear the Call\n"));
			} 

		}
		else 
		{
            /* SPR 18069: Call Clear when 422 received without 
			 * MinSE header. Treat 422 as a valid response 
			 * but set the status of Response as False,so that 
			 * in CC the call will be cleared.Bye will be sent.
			 */ 
			call_clear = ICF_TRUE;
			send_ReInv = ICF_FALSE;
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
   /* To add security association headers*/
    else if(ICF_FAILURE == icf_ssa_form_and_insert_security_agreement_hdrs(
                                   p_ssa_pdb,(icf_uint8_t *)"ACK",
                                   (*ppCallObj)->pUacTransaction->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),
                    (*ppCallObj)->pUacTransaction->pSipMsg,
                    &options,pErr))
    {
        /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
        ret_val = Sdf_co_fail;
    }
    else
    {

        /*SPR 19189 Added protective null check*/
        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                   p_ssa_pdb->p_glb_pdb->p_stats_info,
                   ret_value)
        if(ICF_FAILURE == ret_value)
        {
            ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
            return ret_val;
        }/*End SPR 19189*/
	    ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.
			        stat_outbound_req.stats_ACK);
    }
    /*  Fix Merge for CSR 1-6486039 */
    /* Reset the DNS state to idle only if it is IP used.This is done to cater
     * the scenario in which for sending the ACK for failure response to
     * Re-Invite we have initiated a the dns query.In this scenario the dns
     * state is set to dnsquery and reseting that to idle here will stop the
     * sending of ACK and further requests if buffered to network once the
     * response to dns query is received*/
    if(Sdf_en_dnsIpUsed == (*ppCallObj)->pUacTransaction->dTxnDnsState)
    {
        (*ppCallObj)->pUacTransaction->dTxnDnsState = Sdf_en_Idle;
    }

    /* Fix for CSR 1-6340954: Fix merged from SPR 18598*/
    /* send_to_peer might  set this pointer NULL.
	   So we are restoring it because further this 
	   pointer is getting accessed. */
    if (ICF_NULL == p_ssa_ctxt->p_ssa_pdb)
	{
		p_ssa_ctxt->p_ssa_pdb = p_ssa_pdb;
	} 
#ifdef ICF_SESSION_TIMER
    /* Sending Reinvite again in case 422 received
	 * for the Reinvites generated as a result of either
	 * Session Refresh or after call established using multiple codecs:
	 * This section of code is moved outside the check for session
	 * timer expiry.
	 */
    /* added the check that session_timer_enable is set as true in the call context */
    if((dRespCode == 422) && (ICF_FALSE == call_clear) && 
            (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && 
            (ICF_NULL != p_ssa_ctxt) &&
            (ICF_NULL != p_ssa_pdb->p_internal_msg))
	    {
		   /* CSR 1-5850995 fix : SPR 18426: Check the bitmask_1 in ssa context. 
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
			/*SPR 18879 Added null checks for pSessionTimer */
           if(Sdf_co_null != (*ppCallObj)->pSessionTimer)
           {   
              (*ppCallObj)->pSessionTimer->dSessionInterval = \
                        (*ppCallObj)->pSessionTimer->dMinSe;
           }   

			ret_val = icf_ssa_process_mesg(p_ssa_pdb->p_internal_msg) ;

		    return ret_val; 
			
        }
   if((ICF_NULL != p_ssa_pdb->p_glb_pdb) && 
      (ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx) &&
      (ICF_NULL != p_ssa_pdb->p_internal_msg) &&
      (ICF_NULL != p_ssa_ctxt))
    {   
      if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
      { 
	   if(p_ssa_ctxt->session_timer_expired_refresh == ICF_TRUE)
	   {
         if((408 == dRespCode) ||(481 == dRespCode)||
						(dRespCode == 422 && ret_val == Sdf_co_fail))
		 {
				 call_clear = ICF_TRUE;
		 }
		 else if((401 != dRespCode) &&(407 != dRespCode) && (422 != dRespCode))
		 {
			/* SPR 18069:
			 * If the ReInvite gets fails not because of being challenged, 
			 * then set this value so that reInvite is not sent again
			 */
			send_ReInv = ICF_FALSE;		
			/* SPR 18091: Send Response to CC as success. 
			 */
			p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
					ICF_NW_OTG_SESSION_REFRESH_ACK;
			p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
					ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);
			p_internal_msg_param->status = ICF_TRUE;
			p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
					p_ssa_pdb->p_glb_pdb;
			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
			/* SPR 18147: Start the session timer if not started by UATK 
			 * when 4xx response
			 * except 422/408/481 received. 
			 */
			p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;

            ret_val = icf_dbm_fetch_line_cntxt_blk(p_ssa_pdb->p_glb_pdb,
                      p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->line_id,
                      &p_ssa_pdb->p_line_data,
                      p_ssa_pdb->p_ecode);

            /* In case of 491,if num_491_retries_allowed becomes 0,
             * then clear the call
             */
            if(491 == dRespCode)
            {
                if(0 == p_ssa_ctxt->num_491_retries_allowed)
                {
                    /* clear the call */
                    ret_val = ICF_FAILURE;
                    call_clear = ICF_TRUE;

                }
                p_ssa_ctxt->num_491_retries_allowed-- ;
            }

            if(ICF_FAILURE != ret_val)			
            {
                if(dRefresher != Sdf_en_refresherNone || 
                        ICF_SESSION_REFRESHER_NONE != 
                        p_ssa_pdb->p_line_data->session_timer_refresher)
                {

                    icf_ssa_set_refresher_method((*ppCallObj)->pUacTransaction->pSipMsg,
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
         }
         /* If the response code is not 491, then reset 
          * 'num_491_retries_allowed' to value configured
          * in line data 
          */ 
         if(491 != dRespCode)
         {
             p_ssa_ctxt->num_491_retries_allowed = 
                 ICF_SESSION_REFRESH_NUM_491_RETRY_ALWD;
         }

		if(ICF_TRUE == call_clear)
		{
			/* SPR 18069: Call Clear set to TRUE. 
			 * Treat response as a valid response. 
			 * but set the status of Response as False,so that 
			 * in CC the call will be cleared.Bye will be sent.
			 */ 
			p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
				ICF_NW_OTG_SESSION_REFRESH_ACK;
			p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
				ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);
			p_internal_msg_param->status = ICF_FALSE;

			p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
				p_ssa_pdb->p_glb_pdb;

			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
			ret_val = Sdf_co_success;
		}
       }
       else if(ICF_TRUE == call_clear && 422 == dRespCode)
	   {
         /*CSR1-5634763 */   
		/* SPR 18069: Call Clear set to TRUE. 
		 * Treat response as a valid response. 
		 * but set the status of Response as False,so that 
		 * in CC the call will be cleared.Bye will be sent.
		 */ 
		p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
			ICF_GENERIC_CALL_CLEAR;
		p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
			p_ssa_pdb->p_glb_pdb;
		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
		ret_val = Sdf_co_success;
	   }
       if (ICF_INT_MODULE_CM == p_ssa_pdb->dest_module_id)
	   {
           /* SPR 18426 : In case Failure indication sent to CM then reset the
		      media modify bitmasks */
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
              /*  CSR 1-5685130 Merge */
#ifdef ICF_ERROR_ENABLE
              p_ssa_pdb->resp_code = dRespCode;
              ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_FAILURE_RESPONSE)
#endif
              ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
		      return Sdf_co_success;
	   }
      }
    }/*End SPR 18879 if block for ssa ctx check*/
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SSA ctx/GLB PDB/Call ctx is null \n"));
        ret_val = Sdf_co_fail;
        if (p_ssa_pdb->p_glb_pdb) ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return Sdf_co_fail;
    }
    /*CSR1-5619984 modified below condition adding send_ReInv check*/
    if(ICF_TRUE == send_ReInv)
   
    {
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
            /*  CSR 1-5685130 Merge */
#ifdef ICF_ERROR_ENABLE
            p_ssa_pdb->resp_code = dRespCode;
            ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_FAILURE_RESPONSE)
#endif
            return Sdf_co_success;
        } 
        else if(401 == dRespCode || 407 == dRespCode)
		{
		    /* In case ReInvite was already challenged earlier and Invite with
		     *  Auth params is being challenged again, call the function icf_ssa_validate_challenge
                     *  to do the stale processing
		     */
             /* Following conditions are for two cases 
              * 1. Reinvite is challenged first time and ICF_CALL_AUTHENTICATION_REQ bit is not set
			  * 2. Reinvite with auth param is challneged subsequently then the bit is set so validate
              * challenge function is called for stale processing */

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
                if(dRespCode  == 401)
                { 
                    header_type =  SipHdrTypeWwwAuthenticate;
                }
                else
                {  
                    header_type =  SipHdrTypeProxyAuthenticate;
                }
                                                                
                /* Extracting the auth data received and storing in ssa_ctxt */
                if(ICF_FAILURE == icf_ssa_extract_challenge(
                                      p_ssa_pdb,
                                      header_type))
                {
                    ret_val = ICF_FAILURE;
                }

				else if(ICF_NULL != p_ssa_ctxt->auth_params_list_size)
				{
                    /*Fix for CSR_1-7009929*/
                    icf_user_db_node_st    *p_user_db = ICF_NULL;
                    /*Fix for SPR:19999 (Merged CSR 1-7559038 : SPR-19945)*/
                    icf_uint8_t  search_bitmask = 0;

                    search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;
                    search_bitmask |= ICF_USER_DB_USE_CONTACT_URIS;
                    /*CSR 7615941 SPR 19912*/
					/*Even If user is not configured we do not return 
                      failure and per line value will be used to match 
                      the Realm*/
                    icf_dbm_get_public_uri_node_from_uri_for_line(
                          p_ssa_pdb->p_glb_pdb,
                          p_ssa_pdb->p_glb_pdb->p_call_ctx->line_id,
                          &(p_ssa_pdb->p_glb_pdb->p_call_ctx->local_address),
	                      search_bitmask,
        	              &p_user_db,
                	      p_ssa_pdb->p_ecode);
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

						if((ICF_TRUE == ICF_IS_BIT_SET(
									p_ssa_pdb->p_line_data->line_data_received,
                                    ICF_CFG_LINE_DATA_REALM_RECD)) || 
                           ((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                                   (p_user_db->flags & ICF_USER_DB_REALM_RECD))))                                
						{
							/* We have to compare the received realm with configured one */
							Sdf_st_authenticationParams *pAuthenticationParams = Sdf_co_null;

							if(Sdf_co_success == sdf_listGetAt(p_ssa_ctxt->p_auth_params_list, 0,
										(Sdf_ty_pvoid *)&pAuthenticationParams,
										pErr))
							{
								if((Sdf_co_null != pAuthenticationParams) &&
                                   (Sdf_co_null != pAuthenticationParams->pRealm))
								{
									icf_string_st temp_unq_realm = {0, "0"};
									temp_unq_realm.str[0]='\0';
									temp_unq_realm.str_len=0;

									sdf_fn_uaUnq(pAuthenticationParams->pRealm,(icf_int8_t *)temp_unq_realm.str);

                                    /*CSR_1-7009954 Fixed*/
                                    /* Check if realm is configured per user then compare with
                                     * realm configured per user not with the realm value configured
                                     * per line.
                                     */
                                    if((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                                        (p_user_db->flags & ICF_USER_DB_REALM_RECD)))                                    
                                    {
                                       if(0 != icf_port_strcmp(
                                                   p_user_db->configured_realm.str,
                                                   temp_unq_realm.str))
                                       {
                                        /*Set the release cause in the call context*/
                                         p_ssa_pdb->p_glb_pdb->p_call_ctx->release_cause =
                                                       ICF_CAUSE_REALM_MISMATCH;

                                         ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
                                                       ICF_CALL_AUTHENTICATION_REQ);
#ifdef ICF_ERROR_ENABLE
                                         /*Set error code for sending report_error_ind to application*/
                                         ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_AUTHENTICATION_FAILURE)
                                         report_error_ind_sent = ICF_TRUE;
#endif
                                       }
                                    }

                                    else if(0 != icf_port_strcmp(p_ssa_pdb->p_line_data->configured_realm.str,
												temp_unq_realm.str))
									{
										/*Set the release cause in the call context*/
										p_ssa_pdb->p_glb_pdb->p_call_ctx->release_cause =
											ICF_CAUSE_REALM_MISMATCH;

										ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
												ICF_CALL_AUTHENTICATION_REQ);
                                         /*  CSR 1-5685130 Merge */
#ifdef ICF_ERROR_ENABLE
                                         /*Set error code for sending report_error_ind to application*/
                                          ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",
                                                  ICF_ERROR_AUTHENTICATION_FAILURE)
                                          report_error_ind_sent = ICF_TRUE;
#endif
									}
								}
							}
						}
					}
                                        /*  CSR 1-5685130 Merge */
                                        else
                                        {
                                            p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 |= 
                                                                               ICF_AUTH_INFO_ABSENT;
#ifdef ICF_ERROR_ENABLE
                                            if(ICF_FALSE == report_error_ind_sent)
                                            {
                                                /*Set error code for sending report_error_ind to application
                                                   when username/password was not configured*/
                                                ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_AUTHENTICATION_FAILURE)
                                                report_error_ind_sent = ICF_TRUE;
                                            }
#endif
                                        }
				} /* if(ICF_NULL != p_ssa_ctxt->auth_params_list_size) */
        /*  SPR 18064 Merge */
        else
        {
            p_ssa_pdb->auth_params_pres_in_chal = ICF_FALSE;
        }
			} /* if-else (ICF_CALL_AUTHENTICATION_REQ & ..) */
            else
            {
                /* This code leg will be hit in the following scenario:
                   ICF_CALL_AUTHENTICATION_REQ bit is set and the function icf_ssa_validate_challenge fails
                   the stale processing. In such a case, we should not send a new request with credentials and
                   thus the bit ICF_CALL_AUTHENTICATION_REQ is being reset.
                */
               	ICF_RESET_BIT(p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
												ICF_CALL_AUTHENTICATION_REQ);
            }
		} /* if(401 == dRespCode || 407 == dRespCode) */
  
        /*if(ICF_TRUE == authenticate_req)*/
       if(ICF_CALL_AUTHENTICATION_REQ & p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)
		{
            if(ICF_SSA_DNS_QUERY_ONGOING & p_ssa_ctxt->bitmask_1)
            {
                /* SPR 18872: In case DNS query is issued for sending ACK, then
                 * in the same execution leg, reinvite transaction should not be invoked.
                 * rather call clear should be issued in case of such failure
                 */ 
                p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                            ICF_GENERIC_CALL_CLEAR;
                p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb =
                            p_ssa_pdb->p_glb_pdb;

                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
               ICF_PRINT(((icf_uint8_t *)"\n[SSA]: DNS query ongoing for ACK, can not send Reinvite with Authorisation params so Call will be cleared\n"));
#ifdef ICF_ERROR_ENABLE
                p_ssa_pdb->resp_code = dRespCode;
                ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_FAILURE_RESPONSE)
#endif
            }
            else
            {
                p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE"; 
                ret_val = icf_ssa_send_authorized_req(p_ssa_pdb);
            }
            /* If a new INVITE with credentials was sent successfully,increment the stats 
               accordingly.
            */
            if(ICF_SUCCESS == ret_val)
            {
                /*SPR 19189 Added protective null check*/
                ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                    p_ssa_pdb->p_glb_pdb->p_stats_info,
                    ret_value)
                if(ICF_FAILURE == ret_value)
                {
                    ret_val = Sdf_co_fail;
                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;
                }/*End SPR 19189*/
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                           stat_requests.stat_outbound_req.stats_ReINVITE);
            }
		} 
       /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18922)*/
       /*If ICF_CALL_AUTHENTICATION_REQ bitmask is not set then this specify 
         that function icf_ssa_validate_challenge fails the stale processing.
         and in such a case, we should not send a new request with credentials.
         Further we check if it is Session refresh reInvite then clear the call
        */
       else
       {
#ifdef ICF_SESSION_TIMER
           /*  Fix for spr 18922 */
           /*  If it is Session refresh reInvite then clear the call and 
               resp_code is 401 or 407*/
            if((401 == dRespCode || 407 == dRespCode) && (
               (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && 
                (ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh)))
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
       /*SPR18395 DMS for fax was not happening as previously we were
         checking the ret_val irrespective of CALL_AUTHENTICATE_REQ
         bitmask being set and doing nothing and returning. Now added
         the condition for bitmask too.*/
        if((ICF_SUCCESS == ret_val) &&
          (ICF_CALL_AUTHENTICATION_REQ & 
           p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask))
        {
           /* do nothing */
        }    
        else
        {
            ret_val = ICF_SUCCESS;
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

        /* SPR 18426 : In case of sending failure to application, reset the media modify
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
/*  CSR 1-5685130 Merge */
#ifdef ICF_ERROR_ENABLE
                    if(ICF_FALSE == report_error_ind_sent) 
                    {
                        p_ssa_pdb->resp_code = dRespCode;
                        ICF_SSA_SET_ECODE(p_ssa_pdb,"INVITE",ICF_ERROR_FAILURE_RESPONSE)
                    }
#endif

        /*  SPR 18064 Merge */ 
        if(ICF_FALSE == p_ssa_pdb->auth_params_pres_in_chal)
        {
             p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask
                               |= ICF_AUTH_PARAM_NOT_RCVD;
        }
        else
        {
		        ret_val = icf_ssa_map_sip_resp_code(
					      dRespCode,&(p_internal_msg_param->reason_type));
		    }
                 
       if( ICF_FAILURE != ret_val) 
		   {
                /*SPR 18879 */
                if((ICF_NULL == p_ssa_pdb->p_ssa_ctxt) ||
                  (ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_ctxt) ||
                  (ICF_NULL == p_ssa_pdb->p_internal_msg->p_msg_data) ||
                  (ICF_NULL == (*ppCallObj)->pMediaInfo))
                {
                   ICF_PRINT(((icf_uint8_t *)"\nSSA/Call ctx or p_msg_data/pMediaInfo is null\n"));
                   ret_val = Sdf_co_fail;
                   ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                   return ret_val;
                }/*End SPR 18879*/
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
#ifdef ICF_SESSION_TIMER
    }
#endif
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
    icf_return_t                        ret_value = ICF_SUCCESS;
    SipOptions                          options = {0};
    icf_ssa_ctxt_st                     *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                      *p_ssa_pdb = ICF_NULL;
    icf_nw_inc_media_change_resp_st     *p_internal_msg_param = ICF_NULL;
    icf_boolean_t                       call_clear_ind = ICF_FALSE;
#ifdef ICF_SESSION_TIMER
	Sdf_ty_refresher                    dRefresher = Sdf_en_refresherNone;
    SipMessage                          *pMessage = ICF_NULL;
#endif

	/*This structure is populated if there is any SDP.*/
	/*Then we populate SDP in the internal stream*/
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    /*SPR 18879 Added null check for the pEventContext*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
        /* Extract the pointer to SSA CTXT from the event context */
        p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
        /* Extract the pointer to SSA PDB from the event context */
        p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\npEventContext or pData is null\n"));
        return Sdf_co_fail;
    }/*End SPR 18879 */    

    if ((ICF_NULL == p_ssa_pdb) || 
        (ICF_NULL == p_ssa_pdb->p_glb_pdb) ||
        (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
    {
       ICF_PRINT(((icf_uint8_t *)"\nSSA PDB/GLB PDB/Call ctx is null\n"));
       return Sdf_co_fail;
    }
  
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
    /*Start SPR 18879 */
    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                        (*ppCallObj)->pUacTransaction, ret_value)
    if(ICF_FAILURE == ret_value)
    {
        return Sdf_co_fail;
    }
    /*End SPR 18879 */
    /* CSR_1_7108373 Merged SPR 19672 Start */
    /* Reset the inc_response code */
    p_ssa_ctxt->p_call_ctxt->inc_response_code = 200;
    /* CSR_1_7108373 Merged SPR 19672 End */ 
    /* SPR 18426 : reset the media modify method bitmask here */
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
#ifdef ICF_LOAD_STAT
               p_ssa_pdb->p_glb_pdb->load_data.otg_session_refresh_success++;
#endif
      if(sdf_ivk_uaGetRefresher(*ppCallObj,&dRefresher,pErr) != Sdf_co_fail)
      {  
         /* Fix for CSR 1-7239695: SPR 19638 */
		 if ((Sdf_en_invalidRefesher == dRefresher) ||
			((*ppCallObj)->pSessionTimer->dSessionInterval <
					(*ppCallObj)->pSessionTimer->dMinSe))
        {
                /*Start SPR 18879 */
                ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                  p_ssa_ctxt->p_call_obj,p_ssa_ctxt->p_call_obj->pSessionTimer, \
                       p_ssa_pdb->p_internal_msg,ret_value)
                if(ICF_FAILURE == ret_value)
                {
                    return Sdf_co_fail;
                }
                /*End SPR 18879 */
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
               /* To add security association headers*/
                else if(ICF_FAILURE ==
                      icf_ssa_form_and_insert_security_agreement_hdrs(
                           p_ssa_pdb,(icf_uint8_t *)"ACK",
                           (*ppCallObj)->pUacTransaction->pSipMsg))
                {
                    ret_val = ICF_FAILURE;
                }
        		else if(ICF_FAILURE == 
					icf_ssa_send_to_peer(p_ssa_pdb,(*ppCallObj),
               		(*ppCallObj)->pUacTransaction->pSipMsg,
               		&options,pErr))
        		{
           			 /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
            		ret_val = Sdf_co_fail;
        		}

                /*  Fix Merge for CSR 1-6486039 */
                /* Reset the DNS state to idle only if it is IP used.This is done to cater
                 * the scenario in which for sending the ACK for failure response to
                 * Re-Invite we have initiated a the dns query.In this scenario the dns
                 * state is set to dnsquery and reseting that to idle here will stop the
                 * sending of ACK and further requests if buffered to network once the
                 * response to dns query is received*/               
                if(Sdf_en_dnsIpUsed == (*ppCallObj)->pUacTransaction->dTxnDnsState)
                {
                    (*ppCallObj)->pUacTransaction->dTxnDnsState = Sdf_en_Idle;
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
    
    /* Fix for CSR 1-7092663 : If update is present in allow
     * header then IPTK will use UPDATE as subsequent
     * refresh method*/
    
    icf_ssa_is_update_present_in_allow_hdr(
        pMessage,p_ssa_pdb->p_glb_pdb,
        &p_ssa_ctxt,pErr);

    icf_ssa_set_refresher_method(pMessage,
        p_ssa_pdb->p_glb_pdb,
        &p_ssa_ctxt,
        pErr);
    /*Start SPR 18879 */
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
            p_ssa_pdb->p_ssa_ctxt, \
            p_ssa_pdb->p_ssa_ctxt->p_call_ctxt, ret_value)
    if(ICF_FAILURE == ret_value)
    {
         ret_val = Sdf_co_fail;
         ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
         return Sdf_co_fail;
    }
   /*End SPR 18879 */
   icf_dbm_fetch_line_cntxt_blk(p_ssa_pdb->p_glb_pdb,
                         p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->line_id,
                         &p_ssa_pdb->p_line_data,
                         p_ssa_pdb->p_ecode);
   /*Start SPR 18879 */
   ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,p_ssa_pdb->p_line_data, \
   p_ssa_pdb->p_internal_msg, ret_value)
   if(ICF_FAILURE == ret_value)
    {
         ret_val = Sdf_co_fail;
         ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
         return Sdf_co_fail;
    }
   /*End SPR 18879 */

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

    /* Reset the field 'num_491_retries_allowed' on receiving 200 OK */
    p_ssa_ctxt->num_491_retries_allowed = 
			ICF_SESSION_REFRESH_NUM_491_RETRY_ALWD;

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
            /*Start SPR 18879 */
             ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                     p_ssa_pdb->p_ssa_ctxt, \
                     p_ssa_pdb->p_internal_msg, ret_value)
             if(ICF_FAILURE == ret_value)
             {
                  ret_val = Sdf_co_fail;
                  ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                  return Sdf_co_fail;
             }
             /*End SPR 18879 */
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
           /*Start SPR 18879 */
           ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                       p_inc_media_sdp_info, ret_value)
           if(ICF_FAILURE == ret_value)
           {
                ret_val = Sdf_co_fail;
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_fail;
           }
           /*End SPR 18879 */
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
		/* CSR 1-6240518: Set sdp true in ssa pdb */
#ifdef ICF_TRACE_ENABLE        
		p_ssa_pdb->sdp_present= ICF_TRUE;    
#endif        
    }
    /* Changes done for Message Body Headers Start : */
    else
    {	
		/* CSR 1-6240518: Set sdp false in ssa pdb */
#ifdef ICF_TRACE_ENABLE         
		p_ssa_pdb->sdp_present= ICF_FALSE; 
#endif
        /*Start SPR 18879 */
        ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                     p_ssa_ctxt, p_ssa_pdb->p_glb_pdb->p_call_ctx,
                     p_ssa_pdb->p_ssa_ctxt->p_call_ctxt, ret_value)
        if(ICF_FAILURE == ret_value)
         {
               ret_val = Sdf_co_fail;
               ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
               return Sdf_co_fail;
         }
        /*End SPR 18879 */
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
            if (p_internal_msg_param) p_internal_msg_param->reason_type = \
                ICF_CALL_CLEAR_REASON_REMOTE_MEDIA_NOT_RECEIVED;
            call_clear_ind = ICF_TRUE;
            p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->signalling_ongoing=ICF_FALSE;

        }
        else
        {
            /* SPR 19590 : Start */ 
            if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
                        (*ppCallObj)->pUacTransaction->pSipMsg,SipMessageResponse))
            /* SPR 19590 : End */
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
                ret_val = ICF_FAILURE;
            }
        }
    }
    /*Start SPR 18879 */
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                     p_ssa_pdb->p_glb_pdb->p_call_ctx,
                     p_ssa_pdb->p_ssa_glb_data, ret_value)
    if(ICF_FAILURE == ret_value)
     {
           ret_val = Sdf_co_fail;
           /*SPR 19223 klocwork warning removal */
           /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
           icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
            &p_inc_media_sdp_info);
           ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
           return Sdf_co_fail;
     }
    /*End SPR 18879 */
    /* Changes done for Message Body Headers End :
     */

    /* get the application specific headers from dbm.
     * These headers shall be filled in call context based
     * on the app_id in call ctxt. Currently use app id = 1
     */
    /* Fix For SPR: 19537*/
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
	/* CSR 1-6240518: Send 200 Ok trace for Reinvite accepted*/
	p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE"; 
	p_ssa_pdb->resp_code = 200;
	ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_200_RECVD)
    
    /* SPR 17555 Fix: Send call direction to app_trace for SIP PDUs */
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
   /* To add security association headers*/
    else if(ICF_FAILURE == icf_ssa_form_and_insert_security_agreement_hdrs(
                                  p_ssa_pdb,(icf_uint8_t *)"ACK",
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

       /*Start SPR 18879 */
       ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                       p_ssa_pdb->p_glb_pdb->p_stats_info, ret_value)
       if(ICF_FAILURE == ret_value)
        {
            ret_val = Sdf_co_fail;
           /*SPR 19223 klocwork warning removal */
           /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
           icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
            &p_inc_media_sdp_info);
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return Sdf_co_fail;
        }
       /*End SPR 18879 */
	   ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.
			        stat_outbound_req.stats_ACK);
    }
    /*  Fix Merge for CSR 1-6486039 */
    /* Reset the DNS state to idle only if it is IP used.This is done to cater
     * the scenario in which for sending the ACK for failure response to
     * Re-Invite we have initiated a the dns query.In this scenario the dns
     * state is set to dnsquery and reseting that to idle here will stop the
     * sending of ACK and further requests if buffered to network once the
     * response to dns query is received*/               
    if(Sdf_en_dnsIpUsed == (*ppCallObj)->pUacTransaction->dTxnDnsState)    
    {
        (*ppCallObj)->pUacTransaction->dTxnDnsState = Sdf_en_Idle;
    }
    if ((Sdf_co_fail != ret_val)&&
		(ICF_INT_MODULE_INVALID == p_ssa_pdb->dest_module_id))
    {
        if(ICF_FALSE == call_clear_ind)
        {
            /*Start SPR 18879 */
            ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
              p_ssa_pdb->p_internal_msg,p_ssa_pdb->p_glb_pdb->p_call_ctx, \
              p_ssa_pdb->p_internal_msg->p_msg_data, ret_value)
            if(ICF_FAILURE == ret_value)
            {
                 ret_val = Sdf_co_fail;
                 /*SPR 19223 klocwork warning removal */
                 /*Free the memory allocated to 
                   icf_ssa_inc_media_stream_info_st*/
                 icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                 &p_inc_media_sdp_info);
                 ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                 return Sdf_co_fail;
            }
            /*End SPR 18879 */
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
    /*Start SPR 18879 */
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
              p_ssa_ctxt, p_ssa_pdb->p_glb_pdb->p_call_ctx, \
              ret_value)
    if(ICF_FAILURE == ret_value)
    {
        /*SPR 19223 klocwork warning removal */
        /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
          &p_inc_media_sdp_info);
        return Sdf_co_fail;
    }
    /*End SPR 18879 */

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
    /*Klocwork Warning removal*/
    if(ICF_FAILURE == ret_val)
    {
        /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
         icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                          &p_inc_media_sdp_info);
    }
    else if(ICF_NULL != p_inc_media_sdp_info)
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
    /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {    
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
       /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }   
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = Sdf_co_fail;
    }    
    if((Sdf_co_fail == ret_val) ||
       (ICF_NULL == p_ssa_pdb))
    {   
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context/PDB is NULL.\n"))
        return Sdf_co_success;
    }/*End SPR 18846*/    
    ((icf_nw_inc_alert_media_st*)(p_ssa_pdb->
        p_internal_msg->p_msg_data))->forked_resp_id = ICF_NULL;
    ((icf_nw_inc_media_param_st*)(p_ssa_pdb->
        p_internal_msg->p_msg_data))->forked_resp_id = ICF_NULL;
#ifdef ICF_TRACE_ENABLE
    p_ssa_pdb->sdp_present= ICF_FALSE;
#endif    

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* To remove compiler warning */
    (void)ppCallObj;

    if (ICF_NULL == p_ssa_ctxt->p_call_ctxt)
    {
        ICF_PRINT(((icf_uint8_t *)"\nCall Context is NULL.\n"))
        return Sdf_co_success;
    }

    /*  Fix for spr 19145 */
    if (ICF_NULL == pOverlapTransInfo)
    {
        ICF_PRINT(((icf_uint8_t *)"\nOverLap TRansaction is NULL.\n")) 
        /*  If returning failure then update the STATs*/
        return Sdf_co_fail; 
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
        /*  Rel 8.2 changes Start */
        if(ICF_PER_CALL_PRACK_FEATURE_ENABLED == 
           (p_ssa_ctxt->p_call_ctxt->common_bitmask_2 & ICF_PER_CALL_PRACK_FEATURE_ENABLED))
        {
            /* It is the case when in outgoing INVITE neither 100rel
             * present in supported header nor in require header and
             * 18x received with 100rel in require header
             */
            if(ICF_NO_PRACK == p_ssa_ctxt->p_call_ctxt->prack_value)
            {
                p_ssa_ctxt->p_call_ctxt->release_cause = 
                        ICF_CALL_CLEAR_REASON_PRACK_CALL_NOT_ALLOWED;
                p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                              ICF_GENERIC_CALL_CLEAR;
                p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb =
                           p_ssa_pdb->p_glb_pdb;
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return Sdf_co_success;
            }
        }
        /*  Rel 8.2 changes End */
        /* CSR 1-6212448:Send provisional response traces to application */
#ifdef ICF_TRACE_ENABLE
        p_ssa_pdb->resp_code = dRespCode;  
        p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE";
#endif        
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
#ifdef ICF_TRACE_ENABLE
                    p_ssa_pdb->sdp_present= ICF_TRUE;
#endif                    
	            
                    if(ICF_CALL_CONNECT_DONE & p_ssa_pdb->p_glb_pdb->\
                        p_call_ctx->common_bitmask)
                    {
                        msg_identifier = ICF_SSA_RECVD_ANSWER_TO_OFFER;
                    }

                    if ((ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                            p_ssa_pdb,
                                            pOverlapTransInfo->pSipMsg,
											msg_identifier, &p_inc_media_sdp_info,
                                            p_ecode)) &&
                            (ICF_NULL != p_ssa_pdb->p_ssa_ctxt))
                      
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
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
                        /*If its a precondition call and QOS parameters are not 
                         *received in provisional response then send Generic call 
                         *clear to CC. Bit ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL is set by
                         *icf_ssa_validate_qos_status_line function when either 
                         *qos status lines are not received or invalid received*/
                        /* SPR18282 Added check for bitmask 
                           ICF_PRECONDITION_NEGOTIATION_DONE to avoid sending CANCEL if
                           200OK for QOS UPDATE is received from peer*/
                        if ( p_ssa_pdb->p_ssa_ctxt && ((ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL & 
                             p_ssa_pdb->p_ssa_ctxt->failure_bitmask) &&
                            (!(ICF_PRECONDITION_NEGOTIATION_DONE &
                              p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->common_bitmask_2))))
                    	{
                             p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                             p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                 ICF_GENERIC_CALL_CLEAR;
                             p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                                p_ssa_pdb->p_glb_pdb;                           
                             p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                             ret_val = Sdf_co_success;

                            if(ICF_NULL != p_inc_media_sdp_info)
                            {
                                /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                    &p_inc_media_sdp_info);
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
                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                            return ret_val;
                    	}
ICF_CHECK_QOS_SUPPORT_END                        
                        if (p_inc_media_sdp_info) 
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
				   /* Extract the message bodies from SIP message in overlap
                    * txn as its reliable 18x.
				    */
				  if(ICF_FAILURE ==
                          icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
					      pOverlapTransInfo->pSipMsg,\
					     /**ppCallObj*/SipMessageResponse))
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
/* CSR 1-6212448:Send 180 received trace to application */
#ifdef ICF_TRACE_ENABLE
				ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_180_RECVD)
#endif                
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
#ifdef ICF_TRACE_ENABLE
		    p_ssa_pdb->sdp_present= ICF_TRUE;
#endif                    

                    if(ICF_CALL_CONNECT_DONE & p_ssa_pdb->p_glb_pdb->\
                        p_call_ctx->common_bitmask)
                    {
                        msg_identifier = ICF_SSA_RECVD_ANSWER_TO_OFFER;
                    }

                    if ((ICF_FAILURE == icf_ssa_get_media_parameters_from_sdp(
                                            p_ssa_pdb,
                                            pOverlapTransInfo->pSipMsg,
											msg_identifier, &p_inc_media_sdp_info,
                                            p_ssa_pdb->p_ecode)) &&
                            (ICF_NULL != p_ssa_pdb->p_ssa_ctxt))
                     
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
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
                        /*If its a precondition call and QOS parameters are not 
                         *received in provisional response then send Generic call 
                         *clear to CC. Bit ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL is set by
                         *icf_ssa_validate_qos_status_line function when either 
                         *qos status lines are not received or invalid received*/
                         /* SPR18282 Added check for bitmask 
                            ICF_PRECONDITION_NEGOTIATION_DONE to avoid sending CANCEL if
                            200OK for QOS UPDATE is received from peer*/
                        if ( p_ssa_pdb->p_ssa_ctxt && ((ICF_SSA_QOS_VALIDATION_FAILURE_CANCEL_CALL & 
                           p_ssa_pdb->p_ssa_ctxt->failure_bitmask) &&
                            (!(ICF_PRECONDITION_NEGOTIATION_DONE &
                             p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->common_bitmask_2))))
                    	{
                             p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                             p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                                 ICF_GENERIC_CALL_CLEAR;
                             p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
                                p_ssa_pdb->p_glb_pdb;                           
                             p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                             ret_val = Sdf_co_success;

                            if(ICF_NULL != p_inc_media_sdp_info)
                            {
                                /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                                icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                    &p_inc_media_sdp_info);
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
                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
                            return ret_val;
                    	}
ICF_CHECK_QOS_SUPPORT_END                       
                        if (p_inc_media_sdp_info ) 
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
    /*  Rel 8.2 changes Start */
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb, ICF_FALSE)
                  /* Changes done for Message Body Headers Start :
                    */
                    /* Extract the message bodies from SIP message in overlap
                     * txn as its reliable 18x.
                    */
                    if(ICF_FAILURE ==
                            icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
					             pOverlapTransInfo->pSipMsg,\
					     /**ppCallObj*/SipMessageResponse))
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

                        /*Insert P-Access-Network-Info header in SipMsg*/
                        if ( ICF_FAILURE == 
                                icf_ssa_form_and_insert_access_nw_info_hdr(
                                    p_ssa_pdb,pOverlapTransInfo->pSipMsg))
                        {
                            ret_val = ICF_FAILURE ;
                        } 
                        /* SPR 19535: Insert Security Association header in Re-INVITE/UPDATE message */
                        else if(ICF_FAILURE == icf_ssa_form_and_insert_security_agreement_hdrs(
                                    p_ssa_pdb, (icf_uint8_t *)"PRACK",
                                    pOverlapTransInfo->pSipMsg))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        /* Send PRACK to remote end */
                        else if (ICF_FAILURE == icf_ssa_send_to_peer(
                                  p_ssa_pdb,
                                  (*ppCallObj),
                                  pOverlapTransInfo->pSipMsg,
                                  &options,pErr))
                        {
                                /*Raise/Set an error here-UATK_PROCESSING_FAILURE*/
                                 ret_val = Sdf_co_fail ;
                        }
                    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb, ICF_TRUE)
                    /*  Trigger CC to send PRACK rather thatn sending it from here
                     */
                    /* 
                     * Cast the payload of the internal msg into the
                     * icf_nw_inc_alert_media_st structure 
                     */
                    icf_nw_inc_media_param_st *p_internal_msg_param =
                              (icf_nw_inc_media_param_st *)
                             p_ssa_pdb->p_internal_msg->p_msg_data;
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
        
ICF_CHECK_TUNNEL_MODE_END
/*  Rel 8.2 changes End */
                    
                }
/* CSR 1-6212448:Send 183 received trace to application */
#ifdef ICF_TRACE_ENABLE
				ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_183_RECVD)
#endif                
                break;

            default :
                ICF_PRINT(((icf_uint8_t *)"Unknown provisional response message\n"));
                ret_val = Sdf_co_fail;

        } /* switch(dRespCode) */
    } /* else */
#ifdef ICF_STAT_ENABLE
	icf_ssa_invite_resp_stats(p_ssa_pdb,dRespCode,0);
#endif

    /*Check if UPDATE present in Allow header in incoming 180 or 183
     *response with 100rel, if present then mark in call context 
     *allowed_value parameter*/
    if (ICF_SUCCESS == ret_val)
    {
        icf_ssa_is_update_present_in_allow_hdr(pOverlapTransInfo->pSipMsg,
                                p_ssa_pdb->p_glb_pdb,
                                &p_ssa_ctxt,
                                pErr);
    }
    
    /* for 180 headers are required as there is an API to 
	 * be returned to Application (alert user).
	 */
	if ( (ICF_FAILURE != ret_val) && (180 == dRespCode) )
    {
        /*  Fix for SPR 19145 */
        /*  When RPR received callback gets hit then
         *  New sipmsg is kept in Overlap_txn 
         *  Because PRACK needs to send and PRACK is
         * an Overlap Transaction.
         SCENARIO :
         INVITE sent and 180 received with 100rel in require header
         THen in user_alert_ind the contact header was set of outgoing
         INVITE not of incoming 180.
         It is happening because we are filling the header list in call ctx from uac
         Transaction Sip Msg not from overlap txn sip msg.
         */
         /* get the application specific headers from dbm. 
          * These headers shall be filled in call context based 
          * on the app_id in call ctxt. Currently use app id = 1
          */
        if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                    p_ssa_pdb,ICF_INT_MODULE_CC,
                    (icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
                    pOverlapTransInfo->pSipMsg,
                    p_ssa_pdb->p_ecode) )
        {        
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
/*  icf_sdp_st           *p_sdp_data = ICF_NULL;*/
/*  icf_error_t          *p_ecode =  ICF_NULL;*/
    icf_nw_inc_prack_resp_st *p_internal_msg_param = ICF_NULL;
    /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {    
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
       /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }    
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = Sdf_co_fail;
    }    
    if((Sdf_co_fail == ret_val) ||
       (ICF_NULL == p_ssa_pdb))
    {   
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context/PDB is NULL.\n"))
        return Sdf_co_success;
    }/*End SPR 18846 */    

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

    /*  Rel 8.2 chages start */
    /*  Now every time final resposnse of outgoing prack is
     *  received SSA will trigger CC 
     */
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
            /* populate the header list when 200OK of PRACK is received. */
            icf_ssa_populate_app_specific_hdrs(
                        p_ssa_pdb,ICF_INT_MODULE_CC,
                        (icf_void_t*)( p_ssa_ctxt->p_call_ctxt),
                        pOverlapTransInfo->pSipMsg,
                        p_ssa_pdb->p_ecode);

            /* populate the body list from the SIP message in case present */
            icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,
                                pOverlapTransInfo->pSipMsg,
                                SipMessageResponse);

            /* If response for PRACK is received 
             * Cast the payload of the internal msg into the
             * icf_nw_inc_prack_resp_st structure 
             */
            p_internal_msg_param = 
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
            p_internal_msg_param->resp_code = dRespCode;
            p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
                ICF_PORT_SIZEOF(icf_nw_inc_prack_resp_st);

            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

            ret_val = Sdf_co_success;
#ifdef ICF_STAT_ENABLE
            icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)pMethod,dRespCode,0);
#endif
        }
        else
        {
            /* Neither response for PRACK nor UPDATE was received */
            /* We are ignoring this response */
            ret_val = Sdf_co_success;
        }        
        /* Code is removed from here in Rel 8.2 so
         * that whenever final response received trigger
         * will be sent to CC */
    (void)ppCallObj;
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
    /*  CSR 1-5598742 Merge */
    icf_boolean_t        forceful_clear = ICF_FALSE;
    icf_internal_msg_st    internal_msg;

    /* This variable is used to store the header type(WWW-Authenticate/
     * Proxy-Authenticate) on the basis of the challenge received(401/407).
     * This is passed to the function icf_ssa_extract_chalenge as an
     * input parameter.
     */
    en_HeaderType       header_type = 0;
    /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
       /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }   
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = Sdf_co_fail;
    }    
    if((Sdf_co_fail == ret_val) ||
       (ICF_NULL == p_ssa_pdb))
    {   
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context/PDB is NULL.\n"))
        return Sdf_co_success;
    }/*End SPR 18846 */       
    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

    /*  CSR 1-5598742 Merge */
    internal_msg.msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    
    /* Fetch the response code from the SIP message */
    if (Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                (*ppCallObj)->pUacTransaction->pSipMsg, &dRespCode,
                pErr))
    {
        /*
         * Check against 401/407 
         */
        if((401 == dRespCode || 407 == dRespCode)) 
        {
            err_resp = ICF_FALSE;
            icf_ssa_fetch_auth_param_list(p_ssa_pdb,
                    ppCallObj,ICF_NULL);
             
            if(dRespCode  == 401)
            { 
                header_type =  SipHdrTypeWwwAuthenticate;
            }
            else
            {  
                header_type =  SipHdrTypeProxyAuthenticate;
            }
                                                                
            /* Extracting the auth data received and storing in ssa_ctxt */
            if(ICF_FAILURE == icf_ssa_extract_challenge(
                                      p_ssa_pdb,
                                      header_type))
            {
                 ret_val = ICF_FAILURE;
            }

            /* If list is not null and auth param are configured on line
             * send bye with auth params else clear the call */
            else if(ICF_NULL != p_ssa_ctxt->auth_params_list_size )
            {
                /*CSR_1-7009954 Fixed*/
                icf_user_db_node_st    *p_user_db = ICF_NULL;
                /*Fix for SPR:19999 (Merged CSR 1-7559038 : SPR-19945)*/
                icf_uint8_t  search_bitmask = 0;

                search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;
                search_bitmask |= ICF_USER_DB_USE_CONTACT_URIS;
                /*CSR 7615941-SPR 19912*/
				/*Even If user is not configured we do not return failure and 
					per line value will be used to match the Realm*/
                icf_dbm_get_public_uri_node_from_uri_for_line(
                          p_ssa_pdb->p_glb_pdb,
                          p_ssa_pdb->p_glb_pdb->p_call_ctx->line_id,
                          &(p_ssa_pdb->p_glb_pdb->p_call_ctx->local_address),
	                      search_bitmask,
        	              &p_user_db,
                	      p_ssa_pdb->p_ecode);
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

                    /*CSR_1-7009954 Fixed*/
                    if((ICF_TRUE == ICF_IS_BIT_SET(
                                p_ssa_pdb->p_line_data->line_data_received,
                                ICF_CFG_LINE_DATA_REALM_RECD)) ||
                       ((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                                    (p_user_db->flags & ICF_USER_DB_REALM_RECD))))
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
                                /*CSR_1-7009954 Fixed*/
                               /* Check if realm is configured per user then compare with
                                * realm configured per user not with the realm value configured
                                * per line.
                                */
                                if((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                                              (p_user_db->flags & ICF_USER_DB_REALM_RECD)))                                
                                {
                                   if(0 != icf_port_strcmp(p_user_db->configured_realm.str,
                                                                 temp_unq_realm.str))
                                   {
                                     forceful_clear = ICF_TRUE;
                                     clear_call = ICF_TRUE;
                                   }
                                }
                                else if(0 != icf_port_strcmp(p_ssa_pdb->p_line_data->configured_realm.str, 
                                    temp_unq_realm.str))
                                {
                                    /*  CSR 1-5598742 Merge */
                                    forceful_clear = ICF_TRUE;
                                    clear_call = ICF_TRUE;
                                }
                            }
                        }
                    }
                    if(clear_call == ICF_FALSE)
                    {
						p_ssa_pdb->p_method=(icf_uint8_t *)"BYE"; 
                        p_ssa_pdb->copy_msg_body_from_persitent_data=ICF_FALSE;
					 	ret_val = icf_ssa_send_authorized_req(p_ssa_pdb);
                        /* If a new BYE with credentials was sent successfully,increment the stats 
                           accordingly.
                         */
                        if(ICF_SUCCESS == ret_val)
                        {
                            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                               stat_requests.stat_outbound_req.stats_BYE);
                        }

                        /* If the above function fails, set the boolean 
                         * clear_call so that call can be terminated
                         * forcefully via the callback sdf_cbk_uaCallTerminated
                         */
                        if(ICF_FAILURE == ret_val)
                        {
                            clear_call = ICF_TRUE;
                        }
					}
                 }
/*  	Merge for CSR_1-5685130 */
#ifdef ICF_ERROR_ENABLE
               /* If request with authentication parameters was not sent then
                * set the ECODE as ICF_ERROR_AUTHENTICATION_FAILURE */
               if(ICF_FALSE == p_ssa_pdb->authorised_req)
               {
                    /*Set error code for sending report_error_ind to application*/
                    ICF_SSA_SET_ECODE(p_ssa_pdb,"BYE",ICF_ERROR_AUTHENTICATION_FAILURE)
               }
#endif

            } 
        else
        {
            /*  CSR 1-5598742 Merge */
            forceful_clear = ICF_TRUE;
            /*  SPR 18064 Merge */
             if(ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx)
             {
                 p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask |=      
   		         ICF_AUTH_PARAM_NOT_RCVD;
             }    
        }
        }
        /*  SPR 18064 Merge */
        else if (ICF_FALSE == p_ssa_pdb->auth_params_pres_in_chal)
        {
            if(ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx)
            {
                 p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask |=
                     ICF_AUTH_PARAM_NOT_RCVD;
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
  
    /*  CSR 1-5598742 Merge */
    /* CSR 1-5598742 : SPR 18061 : 
     * Check if call needs to be cleared forcefully then
     * Invoke Abort call if no timers running else simulate timer expiry and 
     * terminate call gracefully. Forceful clear will be set to TRUE only when the 
     * authentication parameters received in BYE challenge validation fails or
     * realm parameter validation fails.
     */

   if(ICF_TRUE == clear_call && ICF_TRUE == forceful_clear)
    {
        p_ssa_ctxt->no_retrans_req = ICF_TRUE;

        /* 
         * Check if the DNS response timer is running. If yes,then Stop DNS timer
         * and simulate the timer expiry for the DNS response timer
         */
        if((ICF_TIMER_ID_INVALID !=  p_ssa_ctxt->dns_resp_timer_id) &&
                (ICF_NULL != p_ssa_ctxt->p_dns_resp_timer_data))
        {
            /* Non-zero id means that the timer is running, so stop it */
            ICF_STOP_TIMER(p_ssa_ctxt->p_glb_pdb, p_ssa_ctxt->dns_resp_timer_id,
                    &(internal_msg.msg_hdr.ecode), ret_val)

            internal_msg.msg_hdr.msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;

            internal_msg.msg_hdr.status = ICF_NULL;

            internal_msg.p_msg_data = 
                (icf_void_t *)(p_ssa_ctxt->p_dns_resp_timer_data);

            ret_val = icf_ssa_process_timer_expiry(&internal_msg);
        } /* if(ICF_TIMER_ID_INVALID !=  p_ssa_ctxt->dns_res ...*/


        /*If any running timer exists, then fetch all the timers 
         * stop these timers and simulate timer expiry for each
         * of the timer*/
        if (ICF_TRUE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt))
        {
            icf_ssa_timer_list_st      *p_timer_list = p_ssa_ctxt->p_timer_list;
            while (0 != p_ssa_ctxt->timer_count)
            {
                p_timer_list = p_ssa_ctxt->p_timer_list;

                if (ICF_NULL != p_timer_list)
                {
                    icf_timer_id_t timer_id = p_timer_list->timer_id;

                    ICF_STOP_TIMER(p_ssa_pdb->p_glb_pdb, timer_id, 
                        &(internal_msg.msg_hdr.ecode), ret_val)

                    internal_msg.msg_hdr.msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;
                    internal_msg.msg_hdr.status = ICF_NULL;
                    internal_msg.p_msg_data = 
                    (icf_void_t *)p_timer_list->p_timer_data;
                    ret_val = icf_ssa_process_timer_expiry(&internal_msg);
                } /* if (ICF_NULL != p_timer_lis... */            

            }       

            p_ssa_ctxt->p_ssa_pdb = (icf_ssa_pdb_st*)p_ssa_pdb;
            p_ssa_ctxt->bitmask |= ICF_SSA_CLEAN_TCP_CONN_FORCEFUL_CLR;
            ret_val = sdf_cbk_uaCallTerminated(
                ppCallObj,pEventContext,pErr);
        } /* if (ICF_FALSE == icf_ssa... */
        /*This field is set to clean TCPM connection if transport mode is TCP*/
        else
        {
            p_ssa_ctxt->bitmask |= ICF_SSA_CLEAN_TCP_CONN_FORCEFUL_CLR;
            ret_val = sdf_cbk_uaCallTerminated(
                    ppCallObj,pEventContext,pErr);
            sdf_ivk_uaAbortCall(
                    p_ssa_ctxt->p_call_obj, 
                (Sdf_ty_pvoid*)p_ssa_ctxt, pErr);
        }
    }
    /*  CSR 1-5598742 Merge */ 
    else if((ICF_TRUE == clear_call) && 
                (ICF_FALSE == forceful_clear))
    {
#ifdef ICF_LOAD_STAT
        printf("\n~~~~~~~~~~~~~~BYE REJECTED~~~~~~~~~~~~~~~\n");
#endif
        sdf_ivk_uaAbortCall(
                *ppCallObj,

                p_ssa_ctxt,
                pErr);
        /* Call object should be cleared in case BYE fails */
        /* p_ssa_pdb->bye_rejected = ICF_TRUE; */

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
    icf_return_t                    ret_value = ICF_SUCCESS;

    /*SPR 19015 :Added Protective null check*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
        /* Extract the pointer to SSA CTXT from the event context */
        p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
        /* Extract the pointer to SSA PDB from the event context */
        p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }   
    else
    {
       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:pEventContext/pData is NULL.\n"))
       return Sdf_co_fail; 
    } 
    if((ICF_NULL == p_ssa_pdb) || (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SSA PDB or GLB PDB is NULL.\n"))
       return Sdf_co_fail; 
    }
	/* Fix for CSR 1-6924198: SPR 19343 */
	/* If the registration context corresponding to the transaction
     * was already cleared then also return a failure.
     */  
    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                    p_ssa_pdb->p_internal_msg,
                    p_ssa_pdb->p_internal_msg->p_msg_data,	
					p_ssa_ctxt->p_rgm_ctxt,
                    ret_value)    
    if(ICF_FAILURE == ret_value)
    {
       return Sdf_co_fail; 
    }  /*End SPR 19015*/   
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
        /*Start SPR 19015 */
        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                pErr,
                ret_value)
        if(ICF_FAILURE == ret_value)
        {
            return Sdf_co_fail;
        }/*End SPR 19015 */
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
              /*  SPR 18064 Merge */
              p_rgm_msg->ecode = ICF_REGM_SSA_AUTH_PARAM_NOT_RCVD;

             /*  CSR 1-5685130 Merge */
             /*
              * A 401/407 without authentication parameters would not be considered a
              * challenge so is would be treated as other 4xx responses as far as
              * alarm is concerned, hence set the response code for the failure response
              */
             p_rgm_msg->response_code = resp_code;
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
                /*Start SPR 19015*/
                ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                   p_ssa_ctxt->p_rgm_ctxt,
                   ret_value)
                if(ICF_FAILURE == ret_value)
                {
                    return Sdf_co_fail;
                }/*End SPR 19015*/
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
#ifdef SDF_THREAD_SAFE
    Sdf_ty_slist         slMatchedHeadersList = {0,0,'\0','\0',{PTHREAD_MUTEX_INITIALIZER,0,0},0};
#else
    Sdf_ty_slist         slMatchedHeadersList = {0,0,'\0','\0',0};  
#endif
    Sdf_ty_u32bit        size = 0;
    Sdf_ty_u32bit        service_route_hdr_count = 0,
                         associate_uri_hdr_count=0,curr_count = 0;
    SIP_S8bit            *pTempBuffer = SIP_NULL;
    SipHeader            *p_serv_rte = SIP_NULL;        

    Sdf_ty_u32bit        param_count = 0, iterator = 0;
    SipHeader            contact_hdr = {SipHdrTypeAny,SIP_NULL};        
    SipContactParam      *pContactParam = Sdf_co_null;
    Sdf_st_error         sdf_err = {0,0,0,"\0"};
	SipHeader            dHeader = {SipHdrTypeAny,SIP_NULL};
    icf_return_t		 temp_ret_val = 0;
    icf_boolean_t        is_srv_rte_allocated = ICF_FALSE;
    
	icf_rgm_context_st	 *p_rgm_context = ICF_NULL;
	SipError             sip_err = 0;

    icf_address_st       temp_addr = {{0,"\0"},0};
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st       *p_ssa_pdb = ICF_NULL;
    icf_rgm_nw_resp_st   *p_rgm_msg = ICF_NULL;
    SipMessage           *p_message = ICF_NULL;

    icf_transport_address_st  *p_proxy_addr = ICF_NULL;
    icf_uint8_t               proxy_address[ICF_MAX_STR_LEN] = "\0";      
    icf_uint8_t               proxy_port[ICF_MAX_SHORT_STR_LEN] = "\0";      
    icf_uint8_t               route_address[ICF_MAX_STR_LEN] = "\0";

    icf_user_db_node_st  *p_node = ICF_NULL;
    /*CSR 1-5473446 Merging */
    Sdf_ty_u32bit                   contact_count1 = 0;
    Sdf_ty_u32bit                   index = 0;
    SipAddrSpec                     *p_addr_spec = ICF_NULL;
    SipUrl                          *p_contact_sip_url = ICF_NULL;
    icf_boolean_t                   contact_found = ICF_FALSE;
    Sdf_ty_retVal ret_val = Sdf_co_success;

    SIP_S8bit porttext[6] = {0}; /* max val of dPort = 65535 */
    SipUrl *pSipUrl      = SIP_NULL;
    icf_return_t return_val = ICF_SUCCESS;
    SipAddrSpec *p_sip_addr_spec = ICF_NULL;

    /* Fix for CSR 1-6191604 : SPR 18834 */
    icf_address_st					rgm_user_addr;
    icf_uint8_t					    *p_rgm_user_info = ICF_NULL;
	/* Fix for SPR: 19999 (Merged CSR 1-7615941: SPR 19912)*/
	icf_line_id_t                    line_index = 0;    
    
	/* Fix for CSR 1-8612071: SPR 20401 */
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_rport_config_data_st            *p_rport_config_data = ICF_NULL;
#else
	icf_transport_address_st			self_addr;
#endif
    
     /* To remove compiler warning */
     (void)ppCallObj;

    if ((ICF_NULL == pEventContext) || (ICF_NULL == pEventContext->pData))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Either Event Context or SSA ctxt is NULL"));
        return Sdf_co_fail;
    }

    /* Extract the pointer to SSA CTXT from the event context */
    p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
    /* Extract the pointer to SSA PDB from the event context */
    p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);

    if ((ICF_NULL == p_ssa_pdb) || (ICF_NULL == p_ssa_pdb->p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Either Event Context or SSA ctxt is NULL"));
        return Sdf_co_fail;
    }


    /* SPR 19189 */
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    /* SPR 19189 end */ 

ICF_4_0_CRM_START
	/* extract the pointer to rgm_context locally */
	p_rgm_context = p_ssa_ctxt->p_rgm_ctxt;
	ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_context))
ICF_4_0_CRM_END

    /* SPR 19007: NULL check has been added */
    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                             p_rgm_context,
                             return_val)
    if(ICF_FAILURE == return_val) 
    {
        /* SPR 19189 */
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        /* SPR 19189 end */
        return Sdf_co_fail;
    }
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                             pOverlapTransInfo,
                             p_ssa_pdb->p_internal_msg,
                             return_val)
    if (ICF_FAILURE == return_val)
    {
        /* SPR 19189 */
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        /* SPR 19189 end */
        return Sdf_co_fail;
    } 

	/* Fix for CSR 1-8612071: SPR 20401 */
    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                             p_ssa_pdb->p_glb_cfg_data,
                             return_val)
    if (ICF_FAILURE == return_val)
    {
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return Sdf_co_fail;
    } 

    /* Fix for SPR: 19999 (Merged CSR 1-7615941: SPR 19912)*/
	/* Get the line data pointer for the last line in the received msg array */
    line_index = (icf_uint16_t )(p_rgm_context->line_id);
    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_ssa_pdb->p_glb_pdb,
                    line_index,
                    &(p_ssa_pdb->p_line_data),
                    p_ssa_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }

    p_message = pOverlapTransInfo->pSipMsg;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_message))
    /* Populate the internal message payload */
    p_rgm_msg = (icf_rgm_nw_resp_st *)
        p_ssa_pdb->p_internal_msg->p_msg_data;

    /* SPR 19007: NULL check has been added */
    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                             p_rgm_msg,
                             p_rgm_msg->p_rgm_context,
                             p_ssa_ctxt->p_call_obj,
                             return_val)
    if(ICF_FAILURE == return_val) 
    {
        /* SPR 19189 */
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        /* SPR 19189 end */    
        return Sdf_co_fail;
    }

    /* Fix for CSR 1-6191604 : SPR 18834 */
	icf_port_memset(
		&rgm_user_addr,
		ICF_NULL,
		ICF_PORT_SIZEOF(icf_address_st));

	/* Fix for CSR 1-8612071: SPR 20401 */
    /* If per line self transport then copy address from line_data*/
    if (ICF_CFG_SELF_ADDR_PER_LINE == p_ssa_pdb->p_glb_cfg_data->self_addr_per_line)
    {
#ifdef ICF_NAT_RPORT_SUPPORT
        p_rport_config_data = p_ssa_pdb->p_line_data->p_rport_config_data;
#else
		ICF_SSA_INIT_TRANS_ADDR(&self_addr)
        icf_port_memcpy(
			&self_addr,
			&(p_ssa_pdb->p_line_data->self_addr),
            sizeof(icf_transport_address_st));
#endif
    }
    else
    {
#ifdef ICF_NAT_RPORT_SUPPORT
        p_rport_config_data = &(p_ssa_pdb->p_glb_cfg_data->rport_config_data);
#else
		ICF_SSA_INIT_TRANS_ADDR(&self_addr)
        icf_port_memcpy(
			&self_addr,
			&(p_ssa_pdb->p_glb_cfg_data->self_ip_address),
            sizeof(icf_transport_address_st));
#endif
    }

#ifdef ICF_NAT_RPORT_SUPPORT
    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                             p_rport_config_data,
                             return_val)
    if (ICF_FAILURE == return_val)
    {
        ret_val = Sdf_co_fail;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return Sdf_co_fail;
    } 
#endif


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
    /*Fix Merge for CSR 1-6168806*/
    /*Also reset the AUTH bitmask now so that we donot fail registration in case
      it is challenged in further reregisters*/
    if (ICF_SSA_AUTH_IN_PROGRESS & p_ssa_ctxt->bitmask)
    {
        p_ssa_ctxt->bitmask &= ~ICF_SSA_AUTH_IN_PROGRESS;
    }


#ifdef ICF_UT_TEST
/*IPTK REl 8.1*/

    SIP_U32bit           Value = 10; 

    /* The value 17 depicts the following API under this condition will be invoked 
       when uatk_api_test API(used in script) contains the value of var1 as 17 */

    if(17 == p_ssa_pdb->p_glb_pdb->dVar1)
    {
       if(SipFail == sip_getHeaderAtIndex(p_message,
                     SipHdrTypeContactNormal,&contact_hdr,0,&sip_err))
       {
            ret_val = Sdf_co_fail;
       }

       /* invoke the API sip_getTypeFromContactHdr here in case of registration 
           and contact is passed as '*' in 200 OK of registration*/
       if(SipFail == sip_getTypeFromContactHdr(&contact_hdr,
                     &Value,&sip_err))
           printf("Failure returned by API sip_getTypeFromContactHdr");
       else 
       {  
           printf("Success returned by API sip_getTypeFromContactHdr");
           if(0 == Value)
           {
               printf("Contact header type : SipContactHdrTypeWildCarded");
           }   
       }    
            
    }  

/*IPTK REl 8.1 END*/ 
#endif 


ICF_CRM_START
/*Merging CSR_1-5473446 */
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
             /* Free the memory allocated for p_addr_spec only in below
              * mentioned scenario:-
              * 1) The function sip_getAddrSpecFromContactHdr is executed
              *    successfully i.e dRefcount of p_addr_spec is 2.
              * 2) If p_addr_spec->dType is not the SIP/SIPS uri.
              * 3) If Function sip_getUrlFromAddrSpec returns failure.
              */   
            
             else if (SipFail == sip_getAddrSpecFromContactHdr(
                         &contact_hdr,&p_addr_spec,&sip_err))
             {
                 sip_freeSipHeader(&contact_hdr);
                 ret_val = Sdf_co_fail;
             }
             /* Fix for CSR 1-8406028 : SPR-20333*/
             else if
#ifdef ICF_SECURE_SUPPORT
             ((SipAddrSipSUri == p_addr_spec->dType) ||
#endif
             (SipAddrSipUri == p_addr_spec->dType)
#ifdef ICF_SECURE_SUPPORT
             )
#endif
             {
                 if (SipFail == sip_getUrlFromAddrSpec(p_addr_spec,
                             &p_contact_sip_url,&sip_err))
                 {
                     sip_freeSipHeader(&contact_hdr);
                     sip_freeSipAddrSpec(p_addr_spec);
                     ret_val = Sdf_co_fail;
                 }
             }
             else 
             { 
                 /*addr type is not SIP */
                 sip_freeSipHeader(&contact_hdr);
                 sip_freeSipAddrSpec(p_addr_spec);
                 ret_val = Sdf_co_fail;
             }

             if (Sdf_co_fail != ret_val)
             {
                 /* Check the type and then compare accordingly
                  */
                 /* Fix for CSR 1-8406028 : SPR-20333*/
                 /* Check added for SIPS uri. This validation is required to
                  * process the 200 OK of REGISTER successfully.
                  */
                if
#ifdef ICF_SECURE_SUPPORT
                 ((SipAddrSipSUri == p_addr_spec->dType) ||
#endif
                  (SipAddrSipUri == p_addr_spec->dType)
#ifdef ICF_SECURE_SUPPORT
                 )
#endif
                 {
                     icf_string_st cfg_contact_ip = {0,{"\0"}};

                     /* Fix for SPR: 19999 (Merged CSR 1-7615941: SPR 19912)*/
                     /* Compare the Contact User Info  received in 
                      * 200 OK with configured contact_user_info.
                      * If contact_user_info is not configured, then
                      * compare with rgm user address.
                      */
                     if (ICF_NULL != p_ssa_pdb->p_line_data->contact_user_info.str_len)
                     {
                         p_rgm_user_info = p_ssa_pdb->p_line_data->contact_user_info.str;
                     }                     
                     /* SPR 19218 : Merged CSR 1-6191604 : SPR 18834 */
                     /* Compare the user Id in req and resp */

                    /* Fix for CSR 1-8648269 SPR 20473.
                     * EGT shall copy the User Address irrespective of Addr Type. 
                     * p_rgm_user_info is also used in case of E164 address.
                     */  
                     else
                     { 
						icf_port_memcpy(
							&rgm_user_addr,
							&(p_rgm_context->rgm_user_address),
							ICF_PORT_SIZEOF(icf_address_st));
						
						p_rgm_user_info = (icf_uint8_t *)icf_port_strtok(
                                (icf_int8_t *)rgm_user_addr.addr_val.str,"@");
                     }
#ifdef ICF_NAT_RPORT_SUPPORT
					/* Fix for CSR 1-8612071: SPR 20401 */
                     if (ICF_SUCCESS != icf_ssa_convert_to_IP_addr(
                                 &(p_rport_config_data->contact_ip_addr), 
                                 (icf_uint8_t*)&(cfg_contact_ip.str)))
                     {
                         ret_val = Sdf_co_fail;
                     }
#else
					/* Fix for CSR 1-8612071: SPR 20401 */
                     if (ICF_SUCCESS != icf_ssa_convert_to_IP_addr(
                                 &self_addr, 
                                 (icf_uint8_t*)&(cfg_contact_ip.str)))
                     {
                         ret_val = Sdf_co_fail;
                     }
#endif
                     /* SPR 19853: Instead of the string value, the address was 
                      * compared making the comparison failure. The issue is 
                      * corrected so to compare string values for matching the 
                      * contact address.
                      */ 
                     else if ((ICF_NULL != p_contact_sip_url->pHost) && 
                             (0 == icf_port_strcmp((icf_uint8_t*)(cfg_contact_ip.str),
                                                  (icf_uint8_t*)(p_contact_sip_url->pHost))))
                     {
                         /* SPR 19218 : Merged CSR 1-6191604 : SPR 18834 */
                         /* Check if user id match in req and resp */
					 	if ((ICF_NULL != p_rgm_user_info) &&
							  (ICF_NULL != p_contact_sip_url->pUser) &&
							  (ICF_NULL != icf_port_strcmp(
												(icf_uint8_t*)(p_rgm_user_info),
												(icf_uint8_t*)(p_contact_sip_url->pUser))))
					 	{
							ICF_PRINT(((icf_uint8_t *)"\n[SSA]: User IDs do not match in Contact response\n"));
					 	}
                        /* Fix for SPR-20473 : CSR 1-8648269 */
                        /* When configured user is of type TEL-URI, EGT does
                         * not send the user part in contact address and
                         * therefore the user part in not persent in Contact
                         * header received in 200 OK response.
                         */
						else if ((ICF_NULL != p_rgm_user_info) &&
								 (ICF_NULL == p_contact_sip_url->pUser) &&
                                 (ICF_ADDRESS_PLAN_TEL_URI != rgm_user_addr.addr_type))
						{
							ICF_PRINT(((icf_uint8_t *)"\n[SSA]: User IDs do not match in Contact response\n"));
						}
						else if ((ICF_NULL == p_rgm_user_info) &&
								 (ICF_NULL != p_contact_sip_url->pUser))
						{
							ICF_PRINT(((icf_uint8_t *)"\n[SSA]: User IDs do not match in Contact response\n"));
						}
					    /* Check if port match in req and resp */
#ifdef ICF_NAT_RPORT_SUPPORT
                         /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR
                          * 18986)*/
                         /*SPR18986 Added null check for dPort pointer to avoid
                           crash when the 200OK for Register does have contact
                           hdr with no port mentioned in URI*/
						/* Fix for CSR 1-8612071: SPR 20401 */
                        else if ((Sdf_co_null != p_contact_sip_url->dPort) &&
                                  (p_rport_config_data->contact_ip_addr.port_num == 
                                  *p_contact_sip_url->dPort))                         
                         {
                             /*set flag to indicate that contact is found*/
                             contact_found = ICF_TRUE;
                             sip_freeSipUrl(p_contact_sip_url);
                             sip_freeSipAddrSpec(p_addr_spec);
                             break;
                         }
#else
                        /* Fix for CSR 1-8406028 : SPR-20333*/
                        /* The port in contact address should be equal to the
                         * port tls_sip_serv_port_num.
                         */
#ifdef ICF_SECURE_SUPPORT
                        else if ((ICF_ADDRESS_PLAN_SIPS_URI == 
                                  p_rgm_context->rgm_user_address.addr_type) &&
                                 (Sdf_co_null != p_contact_sip_url->dPort) &&
                                 (p_ssa_pdb->p_glb_cfg_data->
                                  p_tls_init_data->tls_sip_serv_port_num == 
                                                    *p_contact_sip_url->dPort))
                         {
                             /*set flag to indicate that contact is found*/
                             contact_found = ICF_TRUE;
                             sip_freeSipUrl(p_contact_sip_url);
                             sip_freeSipAddrSpec(p_addr_spec);
                             break;
                         }
#endif
                         /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR
                          * 18986)*/
                         /*SPR18986 Added null check for dPort pointer to avoid
                           crash when the 200OK for Register does have contact
                           hdr with no port mentioned in URI*/
						/* Fix for CSR 1-8612071: SPR 20401 */
                        else if ((Sdf_co_null != p_contact_sip_url->dPort) &&
                                  (self_addr.port_num == 
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
                 /* Earlier the memory allocated to p_addr_spec was getting
                  * freed here for failure scenarios irrespective of the
                  * function sip_getAddrSpecFromContactHdr is executed
                  * successfully or not.Now the issue is resolved and the    
                  * memory of p_addr_spec is freed in all failure legs
                  * above where required and so the step of freeing
                  * memory of p_addr_spec is removed from here.
                  */
                 /*sip_freeSipAddrSpec(p_addr_spec);*/
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
        /*  CSR Merging 1-5473446 */   
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
                     
                     ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                              p_exp,
                                              return_val)
                     if (ICF_FAILURE == return_val)
                     {
                         sip_freeSipContactParam(pContactParam);
                         /* SPR 19189 */
                         ret_val = Sdf_co_fail;
                         ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                         /* SPR 19189 end */
                         return Sdf_co_fail;
                     }
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
            /* SPR 19189 */
            ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            /* SPR 19189 end */
            return Sdf_co_fail;
       }
       if (Sdf_co_fail == sdf_listSizeOf(
                &slMatchedHeadersList,&size, pErr))

       {
           /*Klocwork Warning removal*/
           sip_freeSipContactParam(pContactParam);
           sdf_listDeleteAll(&slMatchedHeadersList,pErr);
           /* SPR 19189 */
           ret_val = Sdf_co_fail;
           ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
           /* SPR 19189 end */
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
            /* SPR 19189 */
            ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            /* SPR 19189 end */
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
            /* SPR 19189 */
            ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            /* SPR 19189 end */
            return Sdf_co_fail;
        }
        
        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                 pExpiresHeader->pHeader,
                                 return_val)
        if (ICF_FAILURE == return_val)
        {
            sip_freeSipContactParam(pContactParam);
            sdf_listDeleteAll(&slMatchedHeadersList,pErr);
            /* SPR 19189 */
            ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            /* SPR 19189 end */
            return Sdf_co_fail;
        }

        expires_duration = (((SipExpiresHeader *)(
                        pExpiresHeader->pHeader))->u).dSec;
		expires_present = ICF_TRUE;

        sdf_listDeleteAll(&slMatchedHeadersList,pErr);
    }/* size != 0 */
   }
    /* Change for CSR-1-5318850 end */
    
	/* CSR_1_6763883 Merged SPR 19672 Start */
#ifdef ICF_ERROR_ENABLE
#ifndef ICF_NAT_RPORT_SUPPORT
	if ((ICF_NULL != p_rgm_context->rgm_duration) && (ICF_FALSE == expires_present))
	{
        p_ssa_pdb->resp_code = 200;
		icf_port_strcpy(p_ssa_pdb->err_str, (icf_uint8_t*)"Duration not received");
        ICF_SSA_SET_ECODE(p_ssa_pdb, "REGISTER", ICF_ERROR_INCOMPLETE_MSG_FROM_NW)
	}
	if ((ICF_NULL != p_rgm_context->rgm_duration) && (ICF_FALSE == contact_found))
	{
        p_ssa_pdb->resp_code = 200;
		icf_port_strcpy(p_ssa_pdb->err_str,(icf_uint8_t*)"Contact not received");
        ICF_SSA_SET_ECODE(p_ssa_pdb, "REGISTER", ICF_ERROR_INCOMPLETE_MSG_FROM_NW)
	}
#endif
#endif
        /* CSR_1_6763883 Merged SPR 19672 End */

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

                        /*Fix Merge for CSR 1-6168806 Even if the n/w 200OK contains
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
#if 0 //ZyXEL         
    else if ((expires_duration*1000) > p_rgm_context->rgm_duration)
		{
      p_rgm_msg->expires_duration = p_rgm_context->rgm_duration;
			p_rgm_msg->status = ICF_RGM_SSA_NW_SUCCESS;
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
			p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_REATTEMPT;
ICF_CHECK_IMS_END
#endif
		}
/* End: Multiple Public Users ICF5.0*/
#endif
		else
		{/* store the expires in rgm_nw_mesg */

			/* Because the duration from network is in seconds and
			 * internally duration is stored as milliseconds */
			p_rgm_msg->expires_duration = (expires_duration * 1000);
			p_rgm_msg->status = ICF_RGM_SSA_NW_SUCCESS;

		}		
        
	/* Clear the contact list and the p-assoc-uri list for user */
    for(iterator = p_rgm_context->num_of_lines;iterator--;)
    {
        icf_dbm_clear_public_uri_node_for_line (p_ssa_pdb->p_glb_pdb,
            p_rgm_context->rgm_list_of_lines[iterator],
            &(p_rgm_context->rgm_user_address),
            ICF_USER_DB_USE_ASSOCIATED_URIS | ICF_USER_DB_USE_CONTACT_URIS,
            p_ssa_pdb->p_ecode);
    }
    /********************ICF 8.0*****************/
    /*Handling for service route received in sucess response for 200 0k*/
    
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
        /* SPR 19189 */
        ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                            p_ssa_ctxt,
                                            p_ssa_ctxt->p_rgm_ctxt,
                                            return_val)
        if(ICF_FAILURE == return_val)
        {
            ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return Sdf_co_fail;
        }
        /* SPR 19189 end */

        /*If service route list is alredy present , delete the list*/
        if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt->p_srv_rte)
        {
            sdf_listDeleteAll((Sdf_ty_slist*)
                    p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                    &sdf_err);
            ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                        p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        temp_ret_val)
        }
ICF_CHECK_IMS_END    
    
    if(0 != p_ssa_ctxt->p_rgm_ctxt->rgm_duration)
    {
        /* SPR 19189 */
        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                 p_message,
                                 return_val)
        if(ICF_FAILURE == return_val)
        {
            ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return Sdf_co_fail;
        }
        /* SPR 19189 end */


        /*Service route handling is applicable in IMS MODE only*/
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)        
        /*Get the header count for service route header from p_message*/
        if(sip_getHeaderCount(p_message, SipHdrTypeServiceRoute, 
                    &service_route_hdr_count, &sip_err)==SipFail)
        {
            ret_val = Sdf_co_fail;
        }
        /*If service_route_hdr_count is greater than zero allocate the memory
         * for service route header list  */
        if (service_route_hdr_count > 0)
        {    
            if(ICF_NULL == p_ssa_ctxt->p_rgm_ctxt->p_srv_rte)
            {
                /*Take Memory For New Srv ROUTE LIST 
                 *It should allocated Once and freed while
                 *deleting the Register*/
                ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
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
            }
            if(is_srv_rte_allocated) 
            {
                /*Initialize the list that will contain the 
                 parsed headers */
  
                if(SipFail == sdf_listInit((Sdf_ty_slist*)
                            p_ssa_ctxt->p_rgm_ctxt->p_srv_rte, 
                            __sdf_fn_uaFreeCompleteHeader, Sdf_co_false, &sdf_err))
                {
                    /*If there is a failure in the initialisation of list,set
                      is_srv_rte_allocated as false and freed the memory
                      allocated for p_ssa_ctxt->p_rgm_ctxt->p_srv_rte*/
                    is_srv_rte_allocated = ICF_FALSE;
                    
                    ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                                   p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                                   ICF_MEM_COMMON,
                                   p_ssa_pdb->p_ecode,
                                   temp_ret_val)
 
                    ret_val = Sdf_co_fail;
                }
            }
        }/*if (service_route_hdr_count > 0)*/
        if(ICF_TRUE == is_srv_rte_allocated)
        {    
            for(curr_count=0;curr_count < service_route_hdr_count;curr_count++)
            {
                /* For loop: on service route header list : Processing begins*/
                /*Get the header value presentat index identify by curr_count*/
                if(SipFail == sip_getHeaderAtIndex(p_message, 
                            SipHdrTypeServiceRoute, &dHeader, 
                            curr_count, &sip_err))
                {
                    ret_val = Sdf_co_fail;
                    break;
                }
                else if (SipFail == sip_initSipHeader(&p_serv_rte, 
                                SipHdrTypeRoute, &sip_err))
                {
                    ret_val = Sdf_co_fail;
                }
                else
                {
                    /*Since Service-Route element Must conform to the syntax
                     * of a Route element which itself must be a sip or sips
                     * uri ,in service route we must  always received a sip or
                     * sips uri*/
                    if ((SipAddrSipUri == ((SipCommonHeader*)(dHeader.pHeader))
                                             ->pAddrSpec->dType ) ||
                         (SipAddrSipSUri ==((SipCommonHeader*)(dHeader.pHeader))
                                             ->pAddrSpec->dType ))
                    {  
                        /*For the first value of service route only do the
                         * following processing.(curr_count = 0)*/
                        
                        /*Here we checking whether the first value in the
                         * service route is same as the proxy configured ; if
                         * it is same we are ignoring the value under
                         * consideration ,and not storing it in the service
                         * route list. */
                        if (curr_count == 0)
                        {
                            /* fetch line data*/
                            if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                                               p_ssa_pdb->p_glb_pdb,
                                               p_ssa_ctxt->line_id,
                                               &p_ssa_pdb->p_line_data,
                                               p_ssa_pdb->p_ecode))
                            {
                                ret_val = Sdf_co_fail;
                            }
                           /* find the proxy_address from line_data 
                              and fill in ssa_ctxt*/
                            else if (ICF_FAILURE == 
                                 icf_ssa_find_proxy_address_list(p_ssa_pdb))
                            {
                                ret_val = Sdf_co_fail;
                            }
                            else
                            {
                                if (!(ICF_SSA_SEC_SERVER_ADDRESS_IS_USED & 
                                        p_ssa_ctxt->bitmask_1))
                                {
                                   /* Set the Proxy address as stored in the 
                                      line_data */
                                   p_proxy_addr = p_ssa_ctxt->p_rem_addr_active;
                                }
                                else
                                {
                                   p_proxy_addr = p_ssa_ctxt->p_rem_addr_inactive;
                                }
                                if (ICF_NULL == p_proxy_addr)
                                {
                                    /*Here if we are not able to find the proxy
                                     * address set ret_val as failure and break
                                     * from the for loop*/
                                    ret_val = Sdf_co_fail;
                                }
                            }
                            if (Sdf_co_fail != ret_val)
                            {
                                if ( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
                                                 p_proxy_addr->addr.addr_type)
                                {
                                    icf_port_strcpy(proxy_address,p_proxy_addr->
                                            addr.addr.domain.str);
                                }
                                else
                                {
                                    icf_ssa_convert_to_IP_addr(
                                                  p_proxy_addr,proxy_address);
                                    icf_port_strcat((icf_uint8_t*)proxy_address,
                                            (icf_uint8_t *)(":"));
                                    icf_port_sprintf((icf_int8_t *)proxy_port,"%d",p_proxy_addr->port_num);
                                    /*Store the proxy address in form of
                                     * 10.203.153.33:33287 ie proxyip:port*/
                                    icf_port_strcat(proxy_address,proxy_port);

                                }
                                
                                pTempBuffer = ((SipCommonHeader *)(dHeader.pHeader))->
                                    pAddrSpec->u.pSipUrl->pHost;
                                
                                /*Also store the route address in the form of
                                 * ip:port or domain:port*/
                                if (ICF_NULL != pTempBuffer)
                                {
                                    pSipUrl = ((SipCommonHeader *)(dHeader.pHeader))->
                                                        pAddrSpec->u.pSipUrl;
                                    porttext[0]='\0';
                                    
                                    icf_port_strcpy(route_address,(icf_uint8_t*)pTempBuffer);
                                    
                                    if (SIP_NULL != pSipUrl->dPort)
                                    {    
                                        sip_snprintf((SIP_S8bit *)porttext, 6, "%u", *(pSipUrl->dPort) );
                                        porttext[5]='\0';
                                        icf_port_strcat(route_address,(icf_uint8_t*)":");
                                        /*Route address in form of ip:port or
                                         * domain:port*/
                                        icf_port_strcat(route_address,(icf_uint8_t*)porttext);
                                    }    
                                }
#ifndef ICF_LOAD_STAT                                
                                if ((ICF_NULL != proxy_address) && (ICF_NULL !=
                                        route_address)) 
                                {
                                    /*If both addresses are same continue from
                                     * here and ignore the given service route
                                     * value.*/
                                    if(!icf_port_strcmp(route_address,proxy_address))
                                    {
                                        /* Memory Leak fix : If the 
                                         * proxy address and the first
                                         * entry of service route is  same
                                         * then we are ignoring the route header
                                         * ,so before continuing free the
                                         * memory for p_serv_rte and dHeader*/
                                        
                                        sip_freeSipHeader(p_serv_rte);
                                        
                                        ICF_MEMFREE(
                                             p_ssa_pdb->p_glb_pdb,
                                             p_serv_rte,
                                             ICF_MEM_COMMON,
                                             p_ssa_pdb->p_ecode,
                                             temp_ret_val)
                                            
                                        sip_freeSipHeader(&dHeader);
                                        continue ; 
                                    }   
                                }
#endif
                            }
                            else 
                            {
                                break ;
                            }   
                        }    

                        if (Sdf_co_fail != ret_val)
                        {
                            /*If Service Route header received in 200ok is valid
                             *then clone this header into new p_serv_rte header
                             *and append this in icf_rgm_context_st's p_srv_rte list
                             *Also, change header type to SipHdrTypeRoute as while
                             *cloning header type becomes service route*/
                            if (SipFail == sip_cloneSipHeader(p_serv_rte,
                                             &dHeader,
                                             &sip_err))
                            {
                                ret_val = Sdf_co_fail;
                            }
                            else
                            {
                                p_serv_rte->dType = SipHdrTypeRoute;
                                /*Append the service route list to service route list 
                                present in regm context*/
                                if(SipFail == sdf_listAppend(
                                              (Sdf_ty_slist*)
                                               p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                                               p_serv_rte, &sdf_err))
                                {
                                    ret_val = Sdf_co_fail;
                                }     
                            }
                        } 
                    }     
                    else
                    {
                        ret_val = Sdf_co_fail;
                    }    
                }  
                if(Sdf_co_fail == ret_val)
                {
                    sip_freeSipHeader(&dHeader);
                    
                    if(ICF_NULL != p_serv_rte)
                    {
                        sip_freeSipHeader(p_serv_rte);
                        
                        /*Stack has decrement the ref count but the memory
                         * allocated has not been free so freeing the memory
                         * in failure case.*/
                        
                        ICF_MEMFREE(
                                    p_ssa_pdb->p_glb_pdb,
                                    p_serv_rte,
                                    ICF_MEM_COMMON,
                                    p_ssa_pdb->p_ecode,
                                    temp_ret_val)
                        
                    }
                    /*If service route list is  present , delete the list
                     as there is some failure while processing the service
                     route*/
                    if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt->p_srv_rte)
                    {
                        sdf_listDeleteAll((Sdf_ty_slist*)
                            p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                            &sdf_err);
                        sdf_listFree((Sdf_ty_slist*)
                            p_ssa_ctxt->p_rgm_ctxt->p_srv_rte, &sdf_err);
                        
                        /*If there is failure in the processing of first
                         * header of service route ,then stack has not freed
                         * the memory aloocated and in that case free the
                         * memory through ICF_MEMFREE*/
                        if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt->p_srv_rte)
                        {    
                            ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                                        p_ssa_ctxt->p_rgm_ctxt->p_srv_rte,
                                        ICF_MEM_COMMON,
                                        p_ssa_pdb->p_ecode,
                                        temp_ret_val)
                        }        
                    }

                    break;
                }

                /*Free the dHeader which is allocated while getting the header
                  value present at index identify by curr_count*/
                sip_freeSipHeader(&dHeader);

            }/* Service-Route for loop handling ends here */
           
        }/*if(is_srv_rte_allocated)*/ 
        
ICF_CHECK_IMS_END        
        
        /*Associated Uri handling start ; get the number of P-assoc Uri header 
          present*/
        if (ICF_FAILURE != ret_val)
        {
            /*Local variable to store the SipAddrSpec structure extracted from
              PAssociatedUri header*/
            SipAddrSpec   *p_sip_addr_spec = ICF_NULL;
            SipError      sip_error        = 0;
            
            if(sip_getHeaderCount(p_message,SipHdrTypePAssociatedUri, 
                        &associate_uri_hdr_count, &sip_err)==SipFail)
            {
                /*ICF_PRINT(("\n[SSA]:Failure sip_getHeaderCount Failed"));*/
                ret_val = Sdf_co_fail;
            }
            for(curr_count=0;curr_count<associate_uri_hdr_count;curr_count++)
            {
                /* For loop: on associated uri header list:Processing begins */
                /*Get the header value present at index identify by 
                  curr_count*/
                if(SipFail == sip_getHeaderAtIndex(p_message, 
                            SipHdrTypePAssociatedUri, &dHeader, 
                            curr_count, &sip_err))
                {
                    /*ICF_PRINT(("\n[SSA]:Failure \
                      sip_getHeaderAtIndex Failed"));
                     */
                    ret_val = Sdf_co_fail;
                    break;
                }
                
                p_sip_addr_spec = (SipAddrSpec *)(((SipFromHeader
                                *)(dHeader.pHeader))->pAddrSpec);
                
                /*Populates the temp_addr structure which is of type
                  icf_address_st structure from the SipAddrSpec structure
                  passed to it*/
                
                /*This temp_addr structure is basically stored in the 
                  p_assoc_uri_list for the user , for whom we get the 
                  p_assoc_uri_list in the 200 ok response of register request*/
              
        		temp_ret_val = icf_ssa_populate_address_from_sip_addr_spec(
                                 (icf_ssa_pdb_st *)(p_ssa_ctxt->p_ssa_pdb), 
                                 p_sip_addr_spec,
                                 &temp_addr);
                

                if (ICF_SUCCESS == temp_ret_val)
                {
                    /* If the assoc uri received in neither sip,sips or tel uri
                     * ignore the same and send the assoc uri to application
                     * in report trace indication*/
                    
                    if ((SipAddrSipUri != p_sip_addr_spec->dType) &&
                        (SipAddrSipSUri!= p_sip_addr_spec->dType) &&
                        SipFail == sip_isTelUrl(p_sip_addr_spec,&sip_error))
                    {
                        /*Send the invalid assoc uri received to application
                         * in report trace indication*/
                        p_ssa_pdb->p_invalid_assoc_uri=temp_addr.addr_val.str;
                        
                        ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_INVALID_ASSOC_URI)
                            
                        /*Free the dHeader which is allocated while getting the header 
                          value present at index identify by curr_count*/
                            
                        sip_freeSipHeader(&dHeader);

                        /*Reinitialised p_invalid_assoc_uri pointer to NULL*/
                        p_ssa_pdb->p_invalid_assoc_uri=ICF_NULL;
                        
                        continue;
                    }    
                    else
    			    {
		    	        /* for each line in rgm context */
                        for(iterator = p_rgm_context->num_of_lines;iterator--;)
				        {
				            /* Get corresponding user db node */
                            if(ICF_SUCCESS == 
                                 icf_dbm_get_public_uri_node_from_uri_for_line (
                                 p_ssa_pdb->p_glb_pdb,
                                 p_rgm_context->rgm_list_of_lines[iterator],
				    		     &(p_rgm_context->rgm_user_address),
					             ICF_USER_DB_USE_PUBLIC_URIS,
                                 &p_node,
                                 p_ssa_pdb->p_ecode))
					        {
                                /*Append the assoc uri list to p_assoc_uri_list of
                                  user node*/        
                                icf_dbm_append_uri_to_address_list(
                                     p_ssa_pdb->p_glb_pdb,
                                     &temp_addr,
						             &(p_node->p_assoc_uri_list),
                                     p_ssa_pdb->p_ecode);
					        }
                        }
                    }
                }
                /*Free the dHeader which is allocated while getting the header 
                  value present at index identify by curr_count*/
                sip_freeSipHeader(&dHeader);

            }/* Header correctly parsed: Processing Ends */
            
        }/*if (ICF_FAILURE != ret_val)*/
        
        /*If assoc uris were not recieved in the resposne ie 
          associate_uri_hdr_count is 0  then we set the public user as unbarred
          by storing it as the first assoc uri */
        if (0 == associate_uri_hdr_count)
        {
            for(iterator = p_rgm_context->num_of_lines;iterator--;)
            {
                /* Get corresponding user db node */
                if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line(
                    p_ssa_pdb->p_glb_pdb,
                    p_rgm_context->rgm_list_of_lines[iterator],
                    &(p_rgm_context->rgm_user_address),
                    ICF_USER_DB_USE_PUBLIC_URIS,
                    &p_node,
                    p_ssa_pdb->p_ecode))
                {
                    /*Append the assoc uri list to p_assoc_uri_list of 
                      user node*/
                    icf_dbm_append_uri_to_address_list(
                       p_ssa_pdb->p_glb_pdb,
                       &(p_rgm_context->rgm_user_address),
                       &(p_node->p_assoc_uri_list),
                       p_ssa_pdb->p_ecode);
                }
            }
        }   

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
                        /*Klocwork Warning removal*/
                         sip_freeSipContactParam(pContactParam);
                         /* SPR 19189 */
                         ret_val = Sdf_co_fail;
                         ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                         /* SPR 19189 end */
                         return Sdf_co_fail;
                    }
                    else
                    {
                       /* Start: Multiple Public Users ICF5.0*/
                        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                 contact_header.pHeader,
                                                 return_val)
                        if (ICF_FAILURE == return_val)
                        {
                            sip_freeSipContactParam(pContactParam);
                            /* SPR 19189 */
                            ret_val = Sdf_co_fail;
                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                            /* SPR 19189 end */
                            return Sdf_co_fail;
                        }

                        p_sip_addr_spec =
                                ((SipContactHeader*)(contact_header.pHeader))->pAddrSpec;

						if ( ICF_FAILURE == 
							 icf_ssa_populate_address_from_sip_addr_spec(p_ssa_pdb,
						 ((SipContactHeader*)(contact_header.pHeader))->pAddrSpec, 
						 &(temp_addr)))
						{
                           /*Klocwork Warning removal*/
                            sip_freeSipContactParam(pContactParam);
							sip_freeSipHeader(&contact_hdr);
                            /* SPR 19189 */
                            ret_val = Sdf_co_fail;
                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                            /* SPR 19189 end */
							return Sdf_co_fail;
						}
                        else
                        {
                            ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                      p_sip_addr_spec,
                                                      return_val)
                            if (ICF_FAILURE == return_val)
                            {
                                /*Klocwork Warning removal*/
                                 sip_freeSipContactParam(pContactParam);
                                 sip_freeSipHeader(&contact_hdr);
                                 /* SPR 19189 */
                                 ret_val = Sdf_co_fail;
                                 ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                                 /* SPR 19189 end */
                                 return Sdf_co_fail;

                            }

                            if(p_sip_addr_spec->dType == SipAddrSipUri) 
                            {
                                SipUrl *pSipUrl = p_sip_addr_spec->u.pSipUrl;
                                
                                ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                 p_ssa_pdb->p_glb_cfg_data,
                                                 return_val)
                                if (ICF_FAILURE == return_val)
                                {
                                    sip_freeSipContactParam(pContactParam);
                                    sip_freeSipHeader(&contact_hdr);
                                    /* SPR 19189 */
                                    ret_val = Sdf_co_fail;
                                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                                    /* SPR 19189 end */
                                    return Sdf_co_fail;
                                }
                                if ( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                                    p_ssa_pdb->p_glb_cfg_data->self_ip_address.addr.addr_type)
                                {
                                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                      pSipUrl,
                                                      return_val)
                                    if (ICF_FAILURE == return_val)
                                    {
                                        sip_freeSipContactParam(pContactParam);
                                        sip_freeSipHeader(&contact_hdr);
                                        /* SPR 19189 */
                                        ret_val = Sdf_co_fail;
                                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                                        /* SPR 19189 end */
                                        return Sdf_co_fail;
                                    }
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
                                    icf_uint8_t remote_ip[32] = {0};
                                    /* SPR 19826 : In case NAT is enabled, compare the IP received in
                                       200 OK od REGISTER with the Rport data IP.
                                    */
#ifdef ICF_NAT_RPORT_SUPPORT
									/* Fix for CSR 1-8612071: SPR 20401 */
                                    if(ICF_SUCCESS == icf_ssa_convert_to_IP_addr(
                                                          &(p_rport_config_data->contact_ip_addr), 
                                                          remote_ip))
                                    {
                                        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                                 pSipUrl,
                                                                 return_val)

                                        if(ICF_FAILURE == return_val)
                                        {
                                            sip_freeSipContactParam(pContactParam);
                                            sip_freeSipHeader(&contact_hdr);
                                            ret_val = Sdf_co_fail;
                                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                                            return Sdf_co_fail;
                                        }

                                        if(ICF_NULL != icf_port_strcmp(
                                                           remote_ip,
                                                           (icf_uint8_t *)pSipUrl->pHost))
                                        {
                                            /* This contact is not meant for ICF
                                             * So dont store the contact just ignore it
                                             */
                                            ICF_PRINT(((icf_uint8_t *)"[SSA] \
                                            Contact %s ignored:host_name does \
                                            not match with rport data IP\n",
                                                temp_addr.addr_val.str));
                                            sip_freeSipHeader(&contact_header);
                                            continue;
                                       }
                                   }
#else
									/* Fix for CSR 1-8612071: SPR 20401 */
                                   if ( ICF_SUCCESS == icf_ssa_convert_to_IP_addr(
                                        &self_addr, remote_ip))
                                    {
                                        ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                                 pSipUrl,
                                                                 return_val)
                                        if (ICF_FAILURE == return_val)
                                        {
                                            /*Klocwork Warning removal*/
                                            sip_freeSipContactParam(pContactParam);
                                            sip_freeSipHeader(&contact_hdr);
                                            /* SPR 19189 */
                                            ret_val = Sdf_co_fail;
                                            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                                            /* SPR 19189 end */
                                            return Sdf_co_fail;
                                        }

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
#endif
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
                                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb,
                                                             p_node,
                                                             return_val)
                                    if (ICF_FAILURE == return_val)
                                    {
                                        sip_freeSipContactParam(pContactParam);
                                        sip_freeSipHeader(&contact_header);
                                        /* SPR 19189 */
                                        ret_val = Sdf_co_fail;
                                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                                        /* SPR 19189 end */
                                        return Sdf_co_fail;
                                    }

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

    /*Klocwork warning removal*/
    if(ICF_FAILURE == ret_val)
     {
       sip_freeSipContactParam(pContactParam);
       sip_freeSipHeader(p_serv_rte);
       if(ICF_NULL != p_serv_rte)
        {
            ICF_MEMFREE(
                 p_ssa_pdb->p_glb_pdb,
                 p_serv_rte,
                 ICF_MEM_COMMON,
                 p_ssa_pdb->p_ecode,
                 temp_ret_val)
        }
     }
     else
     {
        ICF_KLOCWORK_FALSE_POSITIVE(p_ssa_pdb->p_glb_pdb,pContactParam)
     }
    /* SPR 19189 */
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    /* SPR 19189 end */
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
            p_ssa_ctxt->bitmask &= ~(ICF_SSA_AUTH_IN_PROGRESS);
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
                /*  SPR 18064 Merge */
                /*Fix for CSR_1-5806578*/
                if(ICF_FALSE == p_ssa_pdb->auth_params_pres_in_chal)
                {
                    p_rgm_msg->ecode = ICF_REGM_SSA_AUTH_PARAM_NOT_RCVD;
                }
                else
                {
                    p_rgm_msg->ecode = ICF_REGM_SSA_AUTH_FAILURE;
                }
                p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;
                /*Fix for CSR_1-5806578*/

                /*  CSR 1-5685130 Merge */
                /*Reset the bitmask and mark err_resp False as in this case
                 error indication will be sent from REGM*/
                p_ssa_ctxt->bitmask &= ~(ICF_SSA_AUTH_IN_PROGRESS);
                err_resp = ICF_FALSE;
                p_rgm_msg->response_code = resp_code;

            }
            else
            {
                ret_val = icf_ssa_chal_resp_transaction_Completed_for_register(
                    ppCallObj,pEventContext,pOverlapTransInfo, resp_code, pErr);
                err_resp = ICF_FALSE;
            }

           /* Check for security-client field in regm context. If not NULL 
            * then extract and fill the security-server header (if present 
            * in 401/407 response received from network) in p_seurity_server or
            * p_security_verify field of regm_context depending on
            * security association status.
            */
            if(ICF_NULL != p_rgm_msg->p_rgm_context->p_security_client)
            {
               icf_return_t temp_ret_val=ICF_FAILURE;

               if(ICF_CHALLENGE_ALGO_AKA == 
                       p_rgm_msg->p_rgm_context->challenge_algo)
               {
                   if(ICF_FAILURE == 
                       icf_ssa_extract_n_populate_security_server_hdr(
                                         p_message,p_ssa_ctxt,&sip_err))
                   {
                       ret_val = ICF_FAILURE;
                   }
               }
               /* Remove the information related to security-association if   
                * the challenge algo is other than AKA.To remove information 
                * related to Security-Association, the all security association
                * fields(p_security_client, p_security_server, p_security_verify)
                * in regm context will be freed if not NULL.
                */   
               else
               {
                   ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_rgm_msg->p_rgm_context->p_security_client,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        temp_ret_val)

                   if(ICF_NULL != p_rgm_msg->p_rgm_context->p_security_verify)
                   {
                        ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_rgm_msg->p_rgm_context->p_security_client,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            temp_ret_val)
                   }

                   if(ICF_NULL != p_rgm_msg->p_rgm_context->p_security_server)
                   {
                        ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_rgm_msg->p_rgm_context->p_security_client,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            temp_ret_val)
                   } 
               }
               
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

    /* If for a register request network gives a failure response then 
     * response code is filled and sent to application in register confirm & 
     * register status indication*/
     /* SPR 19535: Fill the response code the message to REGM so that in case
      * of failure in processing of success response, there is no UMR encountered
      * while accessing the response code.
      */
      p_rgm_msg->response_code = resp_code;

 #ifdef ICF_LOAD_STAT
    if(ICF_NULL == p_rgm_msg->ecode
             || ICF_RGM_SSA_AUTHENTICATION_REQ == p_rgm_msg->ecode)
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
	Sdf_ty_refresher                    dRefresher = Sdf_en_refresherNone;
    icf_nw_otg_session_refresh_req_st   session_refresh_req = {0};
#endif

	/*This st is populated if there is any SDP*/
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    icf_boolean_t                      call_clear = ICF_FALSE;
    icf_return_t                       ret_value = ICF_SUCCESS;
#ifdef ICF_SESSION_TIMER
	icf_nw_otg_media_change_req_st	   media_chng_req = {0,0,0,0};
#endif
    /*SPR 18879 */
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
        /* Extract the pointer to SSA CTXT from the event context */
        p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
        /* Extract the pointer to SSA PDB from the event context */
        p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }   
    else
    {
       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:pEventContext/pData is NULL.\n"))
       return Sdf_co_fail; 
    }  /*End SPR 18879*/      
    if (ICF_NULL == p_ssa_pdb || 
        ICF_NULL == p_ssa_pdb->p_glb_pdb || 
        ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx)
    {
       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:SSA PDB/GLB PDB/Call Ctx is NULL.\n"))
       return Sdf_co_fail; 
    }
    /*Start SPR 18879 */
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
         pOverlapTransInfo, \
         p_ssa_pdb->p_internal_msg, ret_value)
    if(ICF_FAILURE == ret_value)
     {
           return Sdf_co_fail;
     }
    /*End SPR 18879 */
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
    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;

    p_media_chng_resp->bitmask = ICF_NULL;
    p_media_chng_resp->p_sdp_list = ICF_NULL;
#if 0   
#ifdef ICF_SESSION_TIMER
	}
#endif
#endif
    /*  REl 8.1 
     *  The response code was not getting updated
     *  in call_ctx. In case of failure it must be
     *  updated.
     */
    if(200 != resp_code)
    {
        p_ssa_pdb->p_glb_pdb->p_call_ctx->inc_response_code = resp_code;
    }

    /* If the response code is not 491, then reset 
	 * 'num_491_retries_allowed' to value configured
	 * in line data 
	 */ 
#ifdef ICF_SESSION_TIMER
	if(491 != resp_code)
	{
			p_ssa_ctxt->num_491_retries_allowed = 
					ICF_SESSION_REFRESH_NUM_491_RETRY_ALWD;
	}
#endif

    switch (resp_code)
    {
        case 200:
#ifdef ICF_SESSION_TIMER
    if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    { 
      if(sdf_ivk_uaGetRefresher(*ppCallObj,&dRefresher,pErr) != Sdf_co_fail)
      { 
        /* Fix for CSR 1-7239695: SPR 19638 */
		if ((Sdf_en_invalidRefesher == dRefresher) ||
			((*ppCallObj)->pSessionTimer->dSessionInterval <
					(*ppCallObj)->pSessionTimer->dMinSe))  
        {
                /*Start SPR 18879 */
                ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                    p_ssa_ctxt->p_call_obj,p_ssa_ctxt->p_call_obj->pSessionTimer, \
                    ret_value)
                if(ICF_FAILURE == ret_value)
                {
                   return Sdf_co_fail;
                }
                /*End SPR 18879 */
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
       (ICF_FALSE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable))
    {      
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
                    /*Start SPR 18879 */
                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                       p_ssa_pdb->p_ssa_ctxt, \
                       ret_value)
                    if(ICF_FAILURE == ret_value)
                    {
                        return Sdf_co_fail;
                    }
                    /*End SPR 18879 */
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
                    /*Start SPR 18879 */
                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                       p_inc_media_sdp_info, \
                       ret_value)
                    if(ICF_FAILURE == ret_value)
                    {
                        return Sdf_co_fail;
                    }
                    /*End SPR 18879 */
					p_media_chng_resp->p_sdp_list =\
						p_inc_media_sdp_info->p_sdp_info_list;
					
                    /* CSR 1-6240518: Set sdp true in ssa pdb */
#ifdef ICF_TRACE_ENABLE                    
					p_ssa_pdb->sdp_present= ICF_TRUE;
#endif                    
                    /* SPR 18147: Set the destination module id
					 */	
                    
					p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
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
			   
               /* CSR 1-6240518: Set sdp false in ssa pdb */
#ifdef ICF_TRACE_ENABLE               
			   p_ssa_pdb->sdp_present= ICF_FALSE;
#endif               
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
                   /*  CSR 1-6083896 Merge*/
                   /* CSR 1-6083896 Update is an overlap transaction. Thus use p_message
                    * instead of SIp message in UAC Transaction
                    */ 
                   if(ICF_FAILURE ==
                           icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
                       p_message,\
                       /**ppCallObj*/SipMessageResponse))
                   {
                       ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
                       ret_val = ICF_FAILURE;
                   }
                   /* Changes done for Message Body Headers End :
                   */
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
               }
				
            }
#ifdef ICF_SESSION_TIMER
			}
                else if(ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh)
                { 
                    err_resp = ICF_FALSE;
                    p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_NW_OTG_SESSION_REFRESH_ACK;
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                    p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
                    /* CSR_1_7113139 Merged SPR 19672 Start */
                    /* Fix for CSR 1-7113139: SPR 19462 */	
                    /* Session timer should be started irrespective of
                     * the value of session_timer_expired_refresh.
                     */
                }
                /* CSR_1_7113139 Merged SPR 19672 Start */
		
                if(ICF_FAILURE == ret_val)
                {
                    /* do nothing */
                }    
                else if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
                {    
                    if (ICF_FAILURE == 
                            icf_dbm_fetch_line_cntxt_blk(
                                p_ssa_pdb->p_glb_pdb,
                                p_ssa_ctxt->line_id,
                                &(p_ssa_pdb->p_line_data),
                                p_ssa_pdb->p_ecode))
                    {
                        return ICF_FAILURE;
                    }
                    /*Start SPR 18879 */
                    ICF_CHECK_IF_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                            p_ssa_pdb->p_line_data, \
                            ret_value)
                    if(ICF_FAILURE == ret_value)
                    {
                        return Sdf_co_fail;
                    }

                    /*End SPR 18879 */
                    /* SPR 9591- pass the SPI message instead of the trans. 
                     * pointer */
                    if(dRefresher != Sdf_en_refresherNone || 
                            ICF_SESSION_REFRESHER_NONE != 
                            p_ssa_pdb->p_line_data->session_timer_refresher)
                    {

                        /* Fix for CSR 1-7092663 : If update is present in allow
                         * header then IPTK will use UPDATE as subsequent
                         * refresh method*/

                        icf_ssa_is_update_present_in_allow_hdr(
                                p_message,p_ssa_pdb->p_glb_pdb,
                                &p_ssa_ctxt,pErr);

                        icf_ssa_set_refresher_method(p_message,
                                p_ssa_pdb->p_glb_pdb,
                                &p_ssa_ctxt,
                                pErr);

                        /* SPR 19763: Start the session timer only if call has 
                         * been connected and the UPDATE transaction is after 
                         * call setup
                         */ 
                        if(ICF_CALL_CONNECT_DONE & 
                                p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)
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
                }
#endif
		if(ICF_SSA_UPDATE_WITH_SDP & p_ssa_ctxt->bitmask)
		{
			p_ssa_ctxt->bitmask &= ~(ICF_SSA_UPDATE_WITH_SDP);
		}
        /*CSR 1-5671918*/
        /* SPR 18147: Reset the bitmasks if set when sending Update
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
#ifdef ICF_TRACE_ENABLE
        p_ssa_pdb->p_method = (icf_uint8_t *)"UPDATE"; 
        p_ssa_pdb->resp_code = 200;
        ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_200_RECVD)
#endif        
        break;
		case 422:
#ifdef ICF_SESSION_TIMER
            /*Start SPR 18879 */
             ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                 pErr,(*ppCallObj)->pSessionTimer,ret_value)
             if(ICF_FAILURE == ret_value)
             {
                  return Sdf_co_fail;
             }

             if(ICF_FALSE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
             {
                 /* do nothing */
             }
             /*End SPR 18879 */
            /*CSR1-5594992 change to get header from overlap trx*/
             else if (sip_getHeader(pOverlapTransInfo->pSipMsg,SipHdrTypeMinSE, &dTempHeader, \
				(SipError*)&(pErr->stkErrCode)) != SipFail)
			{
				sip_freeSipHeader(&dTempHeader);
				/* Fix for SPR 14409 */
				if ( (ICF_NULL !=(*ppCallObj)->pSessionTimer) &&
					((90 > (*ppCallObj)->pSessionTimer->dMinSe)
                                         /*  SPR 17993 Merge */
                                         || (65535 < (*ppCallObj)->pSessionTimer->dMinSe)))
				{
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Min-SE in 422 is less than 90, Clear the Call\n"));
                    /* SPR 18147 : Call Clear when 422 received with 
					 * value of MinSE < 90. Treat 422 as a valid response 
					 * but set the status of Response as False,so that 
					 * in CC the call will be cleared.Bye will be sent.
					 */
					call_clear = ICF_TRUE;
				} 
				else
				{
                    /* SPR 18147: Check the bitmask_1 in ssa context. 
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
                    /* SPR 18147: Session Expires header not sent when 422 doesnot contain
					 * session-expires and Supported contain "timer" parameter
					 */
                    /*Fix for SPR:19531 (Klocwork warning removal)*/
                    if (ICF_NULL !=(*ppCallObj)->pSessionTimer)
                    {
                        (*ppCallObj)->pSessionTimer->dSessionInterval = \
                                              (*ppCallObj)->pSessionTimer->dMinSe;
                    }
					ret_val = icf_ssa_process_mesg(p_ssa_pdb->p_internal_msg) ;
					
					if(ICF_FAILURE == ret_val)
					{
							call_clear = ICF_TRUE;
							ret_val = ICF_SUCCESS;
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
                /* SPR 18147: Call Clear when 422 received without 
		    	 * MinSE header. Treat 422 as a valid response 
			     * but set the status of Response as False,so that 
			     * in CC the call will be cleared.Bye will be sent.
			     */ 
			     call_clear = ICF_TRUE;
			}
#endif
		break;
        case 491 :
        {
            /* If Update was sent for Session refresh, then it should be handled in SSA.
             */
#ifdef ICF_SESSION_TIMER
            if((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && \
                    (ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh))
            { 
                err_resp = ICF_FALSE;
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
					     ICF_NW_OTG_SESSION_REFRESH_ACK;
                p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;

                /* Decrement the count by 1*/
                if( 0 == p_ssa_ctxt->num_491_retries_allowed)
                    call_clear = ICF_TRUE;
                else
                    p_ssa_ctxt->num_491_retries_allowed --;

                p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;
                if (ICF_FAILURE == 
                        icf_dbm_fetch_line_cntxt_blk(
                            p_ssa_pdb->p_glb_pdb,
                            p_ssa_ctxt->line_id,
                            &(p_ssa_pdb->p_line_data),
                            p_ssa_pdb->p_ecode))
                {
                    return ICF_FAILURE;
                }
                if(sdf_ivk_uaGetRefresher(*ppCallObj,&dRefresher,pErr) != Sdf_co_fail)
                {  
                    /* SPR 9591- pass the SPI message instead of the trans. 
                     * pointer */
                    if((ICF_FALSE == call_clear) && (dRefresher != Sdf_en_refresherNone || 
                                ICF_SESSION_REFRESHER_NONE != 
                                p_ssa_pdb->p_line_data->session_timer_refresher))
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
            }
            else
            {
#endif
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
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
#ifdef ICF_SESSION_TIMER
            }
#endif
            break;    
        } 
        /*  SPR 18064 Merge */
        case 401:
        case 407:
                   icf_ssa_fetch_auth_param_list(p_ssa_pdb,
                                         ppCallObj,ICF_NULL);
                   if(ICF_NULL == p_ssa_ctxt->auth_params_list_size)
                   {
                       p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask |= 
                       ICF_AUTH_PARAM_NOT_RCVD;
                   }
          p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;         
          break;
        /*SPR 18147: If 481/408 received for any update then clear the call.
		 */
		case 481:
		case 408:
			call_clear = ICF_TRUE;
		break;
        default :
         /*Start SPR 18879 */
        ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_ssa_pdb->p_glb_pdb, \
                 p_ssa_pdb->p_ssa_ctxt, \
                 p_ssa_pdb->p_ssa_ctxt->p_call_ctxt,ret_value)
        if(ICF_FAILURE == ret_value)
         {
              return Sdf_co_fail;
         }
        /*End SPR 18879 */
#ifdef ICF_SESSION_TIMER
          if((ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable) && \
               (p_ssa_ctxt->session_timer_expired_refresh == ICF_TRUE))
	      {
              err_resp = ICF_TRUE;
              p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                                 ICF_NW_OTG_SESSION_REFRESH_ACK;
              p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
			  p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
              p_ssa_ctxt->session_timer_expired_refresh = ICF_FALSE;

             /* Fix for CSR 1-7092663 : If update is present in allow
              * header then IPTK will use UPDATE as subsequent
              * refresh method*/

              icf_ssa_is_update_present_in_allow_hdr(
                  p_message,p_ssa_pdb->p_glb_pdb,
                  &p_ssa_ctxt,pErr);

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
                          p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                              ICF_NW_INC_MEDIA_UPDATE_RESP;
                      }    
                      p_media_chng_resp->status = ICF_FALSE;
                      ICF_SSA_SET_COMMON_BITMASK(p_media_chng_resp->bitmask,
                              ICF_NW_INC_MC_RESP_REASON);
                  }
                  p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
              }
              else
              {
                  ret_val = Sdf_co_fail;   
              }
              /* SPR 18426 : In case of failure responses , the bitmask of Media Modify Method should be reset*/
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
    }/* End of Switch */
    if ( Sdf_co_fail != ret_val)
    {
        /* CSR 1-5671918 SPR 18147: If call clear True then send Indication to CC with appropriate
		 * events so that all can be cleared.
		 */
		if(ICF_TRUE == call_clear)
		{
#ifdef ICF_SESSION_TIMER
			if (ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh)
			{
				/*If Update sent due to session timer expiry then fill the session
				 * refresh ack event in the internal message and set status to false,
				 * so that BYE sent on network and call cleared.
				 */
                 p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
							ICF_NW_OTG_SESSION_REFRESH_ACK;
				 p_ssa_pdb->p_internal_msg->msg_hdr.payload_length = 
							ICF_PORT_SIZEOF(icf_nw_inc_media_change_resp_st);

                 if(ICF_NULL != p_ssa_pdb->p_internal_msg->p_msg_data)
                 {
				    ((icf_nw_inc_media_change_resp_st*)
                       (p_ssa_pdb->p_internal_msg->p_msg_data))->
					             status = ICF_FALSE;
                 }

                p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
							p_ssa_pdb->p_glb_pdb;

                /* Send response to CM */
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
                ret_val = Sdf_co_success;
            }
            else
			{
#endif
				/* SPR 18147: Update was sent as a result of call modify/call hold/resume
				 * from application
				 * Send GENERIC CAll Clear event to application
				 */
				p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
						ICF_GENERIC_CALL_CLEAR;
				p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb = 
						p_ssa_pdb->p_glb_pdb;
				p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
				ret_val = Sdf_co_success;
#ifdef ICF_SESSION_TIMER

			}

#endif
		}
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

   if((ICF_INT_MODULE_CM == p_ssa_pdb->dest_module_id) && 
             (ICF_NW_OTG_SESSION_REFRESH_ACK != p_ssa_pdb->p_internal_msg->msg_hdr.msg_id))     
   {
       /* populate app specific headers */ 
       if ((ICF_SUCCESS == ret_val) && 
               (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                                   p_ssa_pdb,ICF_INT_MODULE_CC,
                                   (icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
                                   pOverlapTransInfo->pSipMsg,
                                   p_ssa_pdb->p_ecode)))
       {
           /*ICF_SSA_TRACE(("\nUnable to get application specific headers"))*/
           ret_val = ICF_FAILURE;
       }
   }
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

    /* This variable is used to store the header type(WWW-Authenticate/
     * Proxy-Authenticate) on the basis of the challenge received(401/407).
     * This is passed to the function icf_ssa_extract_chalenge as an
     * input parameter.
     */
    en_HeaderType       header_type = 0;
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

    /* Fix for CSR 1-8678911: SPR 20428 */
    /* Get app profile pointer in p_ssa_pdb. This will be required later in 
     * function icf_ssa_make_allow_hdr_from_app_profile otherwise it will set failure
     * bitmask in SSA ctxt to 405 that may cause problem in sending correct response code
     * for subsequent incoming request.
     */
    if (ICF_FAILURE == icf_cfg_fetch_app_conf(
                           p_ssa_pdb->p_glb_pdb,
                           p_ssa_ctxt->p_sic_ctxt->app_id,
                           &(p_ssa_pdb->p_app_conf),
                           p_ssa_pdb->p_ecode))
    {
        ICF_PRINT(((icf_uint8_t *)"\nUnable to get application profile"));
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return Sdf_co_fail;
    }

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
                /* Fix for SPR: 19999 (Merged CSR 1-7615966 : SPR 20030)*/
                /* Set the bitmask ICF_NW_INC_SUBS_RESP_NO_RETRY if the bitmask
                 * ICF_SUB_REQ_RETRY_NOT_REQUIRED was configured by
                 * application. The internal mapping of this bitmask is 
                 * ICF_SIC_SUBS_RETRY_NOT_REQUIRED which is stored in sic context.
                 */ 
                if(ICF_SIC_SUBS_RETRY_NOT_REQUIRED == 
                        (ICF_SIC_SUBS_RETRY_NOT_REQUIRED &
                         p_ssa_ctxt->p_sic_ctxt->bitmask))
                {
                    p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
                }
                else
                {
                    p_subs_resp->response = ICF_NW_INC_SUBS_RESP_WITH_RETRY;
                }                
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
            /* CSR 1-6212448: Send 202 received trace for subscribe to application */
#ifdef ICF_TRACE_ENABLE
           /*  Fix Merge for SPR 18901*/
           if(200 == resp_code)
           {
               ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_200_RECVD)
           }
           else if(202 == resp_code)
           {
               ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_202_RECVD)
           }
#endif            

            break;

            /*Failure responses for which retry subscription*/
        case 302:
            /* Fix for SPR: 19999 (Merged CSR 1-7615966 : SPR 20030)*/
            /* reset the bit indicating that request was challenged */
            ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);
          
            /* Set the bitmask ICF_NW_INC_SUBS_RESP_NO_RETRY if the bitmask
             * ICF_SUB_REQ_RETRY_NOT_REQUIRED_FOR_3XX was configured by
             * application. The internal mapping of this bitmask is 
             * ICF_SIC_SUBS_RETRY_NOT_REQUIRED_FOR_3XX which is stored in sic
             * context.
             */ 
            if(ICF_SIC_SUBS_RETRY_NOT_REQUIRED_FOR_3XX == 
                   (ICF_SIC_SUBS_RETRY_NOT_REQUIRED_FOR_3XX &
                          p_ssa_ctxt->p_sic_ctxt->bitmask))
            {
              p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
            }
            else
            {
              p_subs_resp->response = ICF_NW_INC_SUBS_RESP_WITH_RETRY;
            }
            break;            
        case 400:
        case 500:
        case 502:
        case 491:
             /* reset the bit indicating that request was challenged */
            ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);
          
            /* Fix for SPR: 19999 (Merged CSR 1-7615966 : SPR 20030)*/
            /* Set the bitmask ICF_NW_INC_SUBS_RESP_NO_RETRY if the bitmask
             * ICF_SUB_REQ_RETRY_NOT_REQUIRED was configured by
             * application. The internal mapping of this bitmask is 
             * ICF_SIC_SUBS_RETRY_NOT_REQUIRED which is stored in sic context.
             */ 
            if(ICF_SIC_SUBS_RETRY_NOT_REQUIRED == 
                    (ICF_SIC_SUBS_RETRY_NOT_REQUIRED &
                     p_ssa_ctxt->p_sic_ctxt->bitmask))
            {
                p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
            }
            else
            {
                p_subs_resp->response = ICF_NW_INC_SUBS_RESP_WITH_RETRY;
            }
            break;
        case 503:
            /* reset the bit indicating that request was challenged */
            ICF_RESET_BIT(
                p_ssa_ctxt->p_sic_ctxt->bitmask,
                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);

            /* Fix for SPR: 19999 (Merged CSR 1-7615966 : SPR 20030)*/
            /* Set the bitmask ICF_NW_INC_SUBS_RESP_NO_RETRY if the bitmask
             * ICF_SUB_REQ_RETRY_NOT_REQUIRED was configured by
             * application. The internal mapping of this bitmask is 
             * ICF_SIC_SUBS_RETRY_NOT_REQUIRED which is stored in sic context.
             */ 
            if(ICF_SIC_SUBS_RETRY_NOT_REQUIRED == 
                    (ICF_SIC_SUBS_RETRY_NOT_REQUIRED &
                     p_ssa_ctxt->p_sic_ctxt->bitmask))
            {
                p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
            }
            else
            {
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
            }
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
                 /*CSR_1-7009954 Fixed*/
                icf_user_db_node_st    *p_user_db = ICF_NULL;
                /*Fix for SPR:19999 (Merged CSR 1-7559038 : SPR-19945)*/
                icf_uint8_t  search_bitmask = 0;

                search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;
                search_bitmask |= ICF_USER_DB_USE_CONTACT_URIS;
                /*CSR 7615941- SPR 19912*/
				/* Fix done when userID used for subscription is not
				configured on any line.So per-line value will be used 
				to match the realm in challenge*/
                if(ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
                {     
                     icf_dbm_get_public_uri_node_from_uri_for_line(
                                 p_ssa_pdb->p_glb_pdb,
                                 p_ssa_ctxt->line_id,
                                 p_ssa_ctxt->p_sic_ctxt->p_user_address,
	                             search_bitmask,
        	                     &p_user_db,
                	             p_ssa_pdb->p_ecode);
                }
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
                    if ((ICF_TRUE == ICF_IS_BIT_SET(
                                    p_ssa_pdb->p_line_data->line_data_received,
                                    ICF_CFG_LINE_DATA_REALM_RECD)) ||
                        ((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD ==
                                   (p_user_db->flags & ICF_USER_DB_REALM_RECD))))                            
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

                                /*CSR_1-7009954 Fixed*/
                                /* Check if realm is configured per user then compare with
                                 * realm configured per user not with the realm value configured
                                 * per line.
                                 */
                                if((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                                         (p_user_db->flags & ICF_USER_DB_REALM_RECD)))                                
                                {
                                   if(0 != icf_port_strcmp(p_user_db->configured_realm.str,
                                                                 temp_unq_realm.str))
                                   {
                                     p_ssa_ctxt->bitmask |= ICF_SSA_AUTH_IN_PROGRESS;
                                     ICF_PRINT(((icf_uint8_t *)"\n[SSA] Subscribe challenge: "\
                                              "challenge realm not same as configured"));
                                     end_subs = ICF_TRUE;
                                   }
                                }
                                else if (0 != icf_port_strcmp(
                                   p_ssa_pdb->p_line_data->configured_realm.str,
                                   temp_unq_realm.str))
                                {
                                    /*  CSR 1-5685130 Merge */
                                    p_ssa_ctxt->bitmask |= ICF_SSA_AUTH_IN_PROGRESS;
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
                    /*  CSR 1-5685130 Merge */
                    p_ssa_ctxt->bitmask |= ICF_SSA_AUTH_IN_PROGRESS;
                    ICF_PRINT(((icf_uint8_t *)"\nUser auth params not configured on line"));
                    end_subs = ICF_TRUE;
                 }
            } /* if(ICF_NULL != p_ssa_ctxt->auth_params_list_size ) */
            else
            {
                /*  SPR 18064 Merge */
                if (ICF_NULL == p_ssa_ctxt->auth_params_list_size)
                {
                    p_ssa_ctxt->p_sic_ctxt->bitmask |=
                    ICF_SIC_AUTH_PARAM_NOT_RCVD;
                 }
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
                    if(resp_code  == 401)
                    { 
                        header_type =  SipHdrTypeWwwAuthenticate;
                    }
                    else
                    {  
                        header_type =  SipHdrTypeProxyAuthenticate;
                    }
                                                                
                    /* Extracting the auth data received and storing in ssa_ctxt */
                    if(ICF_FAILURE == icf_ssa_extract_challenge(
                                         p_ssa_pdb,
                                         header_type))
                    {
                        ICF_RESET_BIT(
                            p_ssa_ctxt->p_sic_ctxt->bitmask,
                                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);
                        
                        ICF_PRINT(((icf_uint8_t *)"\nSubscription challenge failed"));
                        p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
                        /* set ret_val as ICF_SUCCESS to send indication to app */
                        ret_val = ICF_SUCCESS;
                        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;
                    }
                    else
                    {

                    /* set the bit indicating that request has been challenged */
                    p_ssa_ctxt->p_sic_ctxt->bitmask |= 
                        ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE;
               
                    p_ssa_pdb->p_method = (icf_uint8_t *)"SUBSCRIBE"; 
                    ret_val = icf_ssa_send_authorized_req(p_ssa_pdb);
                    
                    /* Failure in processing challenge */
                    if(ICF_FAILURE == ret_val)
                    {
                        /* reset the bit indicating that request was challenged */
                        ICF_RESET_BIT(
                            p_ssa_ctxt->p_sic_ctxt->bitmask,
                                ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE);
                        ICF_PRINT(((icf_uint8_t *)"\nSubscription challenge failed"));
                        p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
			p_ssa_ctxt->p_sic_ctxt->error_code = *(p_ssa_pdb->p_ecode);
                        /* set ret_val as ICF_SUCCESS to send indication to app */
                        ret_val = ICF_SUCCESS;
                    }
                    else
                    {
                        /* If a new SUBSCRIBE with credentials was sent successfully, increment 
                           the stats accordingly.
                        */
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                           stat_requests.stat_outbound_req.stats_SUBSCRIBE);
                    }

                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                    return ret_val;
                    }
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

#ifdef ICF_LOAD_STAT
          if(ICF_SUCCESS == p_subs_resp->response)
          {     
               p_ssa_pdb->p_glb_pdb->load_data.subscribe_cfm++;
          }
          else
          {
               p_ssa_pdb->p_glb_pdb->load_data.subscribe_fail++; 
          }    
#endif

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

      /* CSR_1_6763883 Merged SPR 19672 Start */
      else if ((ICF_FALSE == expires_present) && 
               ((200 == resp_code) || (202 == resp_code)))
      {
          p_ssa_pdb->resp_code = resp_code;
          icf_port_strcpy(p_ssa_pdb->err_str,(icf_uint8_t*)"Duration not received");
          ICF_SSA_SET_ECODE(p_ssa_pdb, "SUBSCRIBE", ICF_ERROR_INCOMPLETE_MSG_FROM_NW)
      }
      /* CSR_1_6763883 Merged SPR 19672 End */

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
       
            
            /* CSR_1_6763883 Merged SPR 19672 Start */
#ifdef ICF_ERROR_ENABLE
            p_ssa_pdb->resp_code = resp_code;
            icf_port_strcpy(p_ssa_pdb->err_str,(icf_uint8_t*)"Duration not received");
            ICF_SSA_SET_ECODE(p_ssa_pdb, "PUBLISH", ICF_ERROR_INCOMPLETE_MSG_FROM_NW)
#endif
            /* CSR_1_6763883 Merged SPR 19672 End */

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
                    /* fix for Klocworks warning */
                    if(ICF_NULL != p_temp_buff)
                    {
                        fast_memfree(0, p_temp_buff, &sip_err);
                    }
                    sip_freeSipHeader(&dHeader);
                    break;
                }
                sip_freeSipHeader(&dHeader);

            } /* End for (curr_count) */
		}/* End if(dCount) */

        /* CSR_1_6763883 Merged SPR 19672 Start */
#ifdef ICF_ERROR_ENABLE
        else
        {
            p_ssa_pdb->resp_code = resp_code;
            icf_port_strcpy(p_ssa_pdb->err_str,(icf_uint8_t*)"SIP-ETag not received");
            ICF_SSA_SET_ECODE(p_ssa_pdb, "PUBLISH", ICF_ERROR_INCOMPLETE_MSG_FROM_NW)
        }
#endif
        /* CSR_1_6763883 Merged SPR 19672 End */
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
#ifdef ICF_LOAD_STAT
   /* Code changes for updating the publish stats if response is a success */
   if(200 == resp_code)
     {
        p_ssa_pdb->p_glb_pdb->load_data.otg_publish_success++;
      }
#endif
                        
	
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

    /* Fix For SPR : 19135*/
    /* Do not set the sic context as null in ssa context.Setting
     * this as NULL will prevent the setting of ssa context in sic
     * context as null while application data (ssa context ) is
     * freed by function sdf_cbk_uaFreeApplicationData*/
    /* In SIC module depending upon this check for ssa context in
     * sic context we call delink ssa function.Since in case of
     * TCP we have alredy freed the callobject before invoking the
     * SIC module ,depending upon check for ssa context in sic 
     * context delink function will be invoked and this results in
     * FMR & FMW*/
	/*p_ssa_ctxt->p_sic_ctxt = ICF_NULL;*/
    
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
    Sdf_ty_u16bit                       resp_code = 0;
    SipMessage                          *p_message = ICF_NULL;
    Sdf_ty_retVal                       ret_val = Sdf_co_success;
    Sdf_ty_s8bit                        *p_method = ICF_NULL;
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    icf_ssa_pdb_st                   *p_ssa_pdb = ICF_NULL;
    SipHeader                        *pRetryAfterHeader = Sdf_co_null;
    Sdf_st_headerList                *pRetryAfterHeaderList = Sdf_co_null;
    Sdf_ty_slist                      slMatchedHeadersList;
    Sdf_ty_u32bit                       size = 0;

    /* This variable is used to store the header type(WWW-Authenticate/
     * Proxy-Authenticate) on the basis of the challenge received(401/407).
     * This is passed to the function icf_ssa_extract_chalenge as an
     * input parameter.
     */
    en_HeaderType       header_type = 0;
    /*This structure is populated if there is sdp in the response msg.
	 * Then we populate the call context correspondingly.
	 */
	icf_ssa_inc_media_stream_info_st	*p_inc_media_sdp_info = ICF_NULL;
    

    /* to remove compiler warning */
    ppCallObj = ppCallObj;
    
    /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {
        /* Extract the pointer to SSA CTXT from the event context */
        p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);
        /* Extract the pointer to SSA PDB from the event context */
        p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
    }    
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = Sdf_co_fail;
    }    
    if((Sdf_co_fail == ret_val) ||
       (ICF_NULL == p_ssa_pdb))
    {   
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context/PDB is NULL.\n"))
        return Sdf_co_success;
    }/*End SPR 18846 */    

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
/* CSr 1-6212448: Fill method name and response code in ssa pdb */
#ifdef ICF_TRACE_ENABLE
		p_ssa_pdb->sdp_present = ICF_FALSE;
		p_ssa_pdb->p_method = (icf_uint8_t*)p_method;
#endif        
        /*SPR 19476 The response code population was done previously
          in ICF_TRACE_ENABLE but now we require this to identify 
          whether response is 200OK or not in icf_ssa_populate_app_specific_hdrs
          when invoked from icf_ssa_transaction_Completed_for_register.
          Therefore moved the population of response code outside the
           ICF_TRACE_ENABLE flag*/
		p_ssa_pdb->resp_code = resp_code;
        /* Check if it was a registration transaction */
        if (0 == Sdf_mc_strcasecmp(p_method,"REGISTER"))
        {
           ret_val = icf_ssa_transaction_Completed_for_register(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr);

        } /* if (0 == Sdf_mc_strcasecmp(p_m... */
         /* End if (method == REGISTER) */
        else if (0 == Sdf_mc_strcasecmp(p_method,"UPDATE"))
        {
           /*  Fix for spr 18777 */ 
           if (ICF_NULL == p_ssa_ctxt->p_call_ctxt)
           {
               ICF_PRINT(((icf_uint8_t *)"\n[SSA]: UPDATE response: Call ctxt null\n")) 
               return ICF_FAILURE;
           } 
           else if((ICF_TRUE == 
                   p_ssa_ctxt->p_call_ctxt->resource_reservation_completed) &&
                   (ICF_PRECONDITION_NEGOTIATION_DONE != (ICF_PRECONDITION_NEGOTIATION_DONE
                     &  p_ssa_ctxt->p_call_ctxt->common_bitmask_2)))                                    
           {
               /*SPR 19430: Added the ICF_PRECONDITION_NEGOTIATION_DONE check
                 to handle scenario when qos call is established and UPDATE 
                 for the call hold is send and when 200OK was received it was
                checked that resource_reservation_completed flag is TRUE then
                set the qos_update_succ_resp_recv to TRUE interpreting it as
                response for QOS UPDATE, which caused call clearing in
                icf_ssa_populate_media_sdp_info because of absence of QOS
                lines in call hold UPDATE response. */
               /*SPR 18282/18630: resource reservation completed flag
                 is used to identify that 200OK for UPDATE is recvd for
                 UPDATE being sent by IPTK during QOS negotiation.*/
               p_ssa_pdb->qos_update_succ_resp_recv = ICF_TRUE;
           }
           ret_val = icf_ssa_transaction_Completed_for_update(ppCallObj,
                    pEventContext,pOverlapTransInfo,resp_code,pErr);

        }
        else if (0 == Sdf_mc_strcasecmp(p_method,"NOTIFY"))
        {
            /* In tunnel mode, this response to a NOTIFY
               sent will be given to CC as a generic response.
            */   
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)    
            /* SPR 19999 Fix for CSR 1-7618644: SPR 19989 */
            /* In case NOTIFY is challeneged with 401 or 407 then the challenge
             * should be responded. As of now challenge is handled only one time
             * and on subsequent challenge, it is treated as a failure response
             * for NOTIFY and accordingly handled.
             */
            p_ssa_pdb->p_method=(icf_uint8_t*)"NOTIFY";
            if((401 == resp_code) || (407 == resp_code))
            {
                /* Fix For SPR:20110*/
                /* Following additional condition is added as a fix for
                 * SPR:20110.We have updated the icf_ssa_handle_chal_resp
                 * function where code changes done in fuction for returning
                 * the FAILURE is removed.
                 * Now send failure response to NOTIFY challenge if function
                 * icf_ssa_handle_chal_resp fails or authorised_req variable
                 * in ssa_pdb is FALSE(This indicates NOTIFY has challenged
                 * second time or scenario where authentication fails) .*/
                if((ICF_FAILURE == icf_ssa_handle_chal_resp(ppCallObj,
                            pEventContext,pOverlapTransInfo,resp_code,pErr)) ||
                       (ICF_FALSE == p_ssa_pdb->authorised_req))
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
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
            /* In tunnel mode, call the following handler function
               to process this response received.
            */   
            ret_val = icf_ssa_handle_generic_msg_resp(
                          p_ssa_pdb,pOverlapTransInfo,
                          p_method,resp_code);
ICF_CHECK_TUNNEL_MODE_END
        }
        else if (0 == Sdf_mc_strcasecmp(p_method,"REFER"))
        {
            /* In tunnel mode, this response to a REFER
               sent will be given to CC as a generic response.
            */   
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)             
            icf_nw_inc_refer_resp_st     *p_refer_resp =
                (icf_nw_inc_refer_resp_st*)
                (p_ssa_pdb->p_internal_msg->p_msg_data);

            p_refer_resp->response = resp_code;
            /*  CSR 1-5685130 Merge */
            p_ssa_pdb->p_method=(icf_uint8_t*)"REFER";

            if(401 == resp_code || 407 == resp_code)
            {
                 p_ssa_pdb->copy_msg_body_from_persitent_data=ICF_FALSE;
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
                         if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
                         {
                            p_ssa_ctxt->p_call_ctxt->retry_after_duration =
                                (((SipRetryAfterHeader *)
                                (pRetryAfterHeader->pHeader))->u).dSec;
                         }  
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
            else if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
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
                    
                    /*CSR-1-5634729*/
                    if(ICF_NULL != p_ssa_pdb->reason_code_1)
                    {
                        p_refer_resp->error_cause =
				p_ssa_pdb->reason_code_1;
                    }
                    else if (ICF_FAILURE ==  icf_ssa_map_sip_resp_code(
                        resp_code,&(p_refer_resp->error_cause)))
                    {
                        ret_val = ICF_FAILURE;
                    }
                }
            }
          }
#ifdef ICF_TRACE_ENABLE
		 if ( 202 == resp_code )
		 {
             p_refer_resp->status = ICF_TRUE;
             /* CSR 1-6212448: Send App trace for 202 received */
             ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_202_RECVD)
         }
#endif 
ICF_CHECK_TUNNEL_MODE_END         
ICF_CHECK_TUNNEL_MODE_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
         /* In tunnel mode, call the following handler function
             to process this response received.
          */   
         ret_val = icf_ssa_handle_generic_msg_resp(
                       p_ssa_pdb,pOverlapTransInfo,
                       p_method,resp_code);
ICF_CHECK_TUNNEL_MODE_END
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
                /* Merging CSR 1-6072476: If response code is not 401/407 then
                 * trigger to SIC. Else not.*/
                if((401 != resp_code) && (407 != resp_code))
                {
                    /* Send response to SIC based on the recd response code */
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;

                   /* Populate the internal message header */
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
		        ICF_NW_INC_OPTIONS_RESP;
                }

                p_ssa_pdb->p_internal_msg->msg_hdr.payload_length =
                    ICF_PORT_SIZEOF(icf_nw_inc_opt_resp_st);

                /* Populate the internal message payload */
                p_ssa_pdb->p_internal_msg->p_msg_data = 
                    (icf_void_t *)p_opt_resp;

                p_opt_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
            
                p_opt_resp->response = resp_code;
                p_ssa_pdb->resp_code = resp_code;
            /*
     	 	 * CSR 1-5856190: SPR 18425: Fix for FMR in case of Out-Of-Dialog
             * OPTIONS on TCP Set ICF_SSA_TRACE_REQUIRED bitmask so that in
             * case of TCP the SIP PDU trace invoked before freeing the
             * call object, else this will lead to invalid ssa context
             * pointer access. 
     		 */
	     	ICF_SSA_SET_COMMON_BITMASK(
    	                p_ssa_pdb->common_bitmask,
        	            ICF_SSA_TRACE_REQUIRED)
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
            /* extracting sip message to be passed to the trace function*/
			p_ssa_pdb->call_direction = ICF_CALL_DIRECTION_INCOMING;
			ICF_SSA_APP_TRACE(p_ssa_pdb , ICF_TRACE_SIP_PDU)
			/* Fix for CSR 1-5856190 Ends */
#endif
   /*Check if the rsp code is 401/407, then send the appropriate */
                if((401 == resp_code) || (407 == resp_code))
                {
                   icf_boolean_t  realm_match = ICF_TRUE;
                   icf_ssa_fetch_auth_param_list(p_ssa_pdb,
                                    ppCallObj,pOverlapTransInfo);
                   if(resp_code  == 401)
                   { 
                       header_type =  SipHdrTypeWwwAuthenticate;
                   }
                   else
                   {  
                       header_type =  SipHdrTypeProxyAuthenticate;
                   }
                   /* Extracting the auth data received and storing in ssa_ctxt */
                   if(ICF_FAILURE == icf_ssa_extract_challenge(
                                          p_ssa_pdb,
                                          header_type))
                   {
                       /* Send response to SIC based on the recd response code */
                       p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;

                       /* Populate the internal message header */
                       p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                           ICF_NW_INC_OPTIONS_RESP;
                       /*  SPR 18064 Merge */
                       if(ICF_NULL == p_ssa_ctxt->auth_params_list_size)
                       {
                           p_opt_resp->p_sic_ctx->bitmask |= 
                                    ICF_SIC_AUTH_PARAM_NOT_RCVD;
                           return ICF_SUCCESS;
                       } 
                       /* set ret_val as ICF_SUCCESS to send indication to app */
                       ret_val = ICF_SUCCESS;
                       ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                       return ret_val;
                   }

                   else if(ICF_NULL == p_ssa_ctxt->auth_params_list_size) 
                   {  
                       /*p_opt_resp->p_sic_ctx->bitmask |= 
                                  ICF_OPT_REQ_CHALLENGD_WO_AUTH_PRMS;*/
                       return ICF_SUCCESS;
                   }
                   else
                   {
                       /*CSR_1-7009954 Fixed*/
                      icf_user_db_node_st    *p_user_db = ICF_NULL;
                      icf_address_st          local_address = {{0,"\0"},0};
                      /*Fix for SPR:19999 (Merged CSR 1-7559038 : SPR-19945)*/
                      icf_uint8_t  search_bitmask = 0;

                      search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;
                      search_bitmask |= ICF_USER_DB_USE_CONTACT_URIS;
                      
                      /* Fetch the local user address to get the user db node*/
                      /*If the options is out of dialog*/
                      if(ICF_NULL == p_ssa_ctxt->p_call_ctxt)
                      {
                          icf_port_memcpy(
                                   &local_address,
                                   p_ssa_ctxt->p_sic_ctxt->p_user_address,  
                                   sizeof(icf_address_st));
                      }
                      /*If the options is indialog*/
                      else
                      {
                          icf_port_memcpy(
                                   &local_address,
                                   &(p_ssa_ctxt->p_call_ctxt->local_address),  
                                   sizeof(icf_address_st));
                      }
                      /* Fetch user_db node for user with local address*/
                      /*CSR 7615941 SPR 19912*/
					/*Even If user is not configured we do not return failure 
					and per line value will be used to match the Realm */
                      icf_dbm_get_public_uri_node_from_uri_for_line(
                             p_ssa_pdb->p_glb_pdb,
                             p_ssa_ctxt->line_id,
                             &local_address,
	                         search_bitmask,
        	                 &p_user_db,
                             p_ssa_pdb->p_ecode);
                      /* Fetch Line data from DBM */
                      if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                                    p_ssa_pdb->p_glb_pdb,
                                    p_ssa_ctxt->line_id,
                                    &(p_ssa_pdb->p_line_data),
                                    p_ssa_pdb->p_ecode))
                      {
                          ret_val = ICF_FAILURE;
                      }
                      else if(ICF_SUCCESS == 
                                  icf_cmn_check_if_auth_params_present(
                                    p_ssa_pdb->p_glb_pdb, 
                                    &local_address, 
                                    p_ssa_pdb->p_line_data->line_id))
                      {
                              if ((ICF_TRUE == ICF_IS_BIT_SET(
                                    p_ssa_pdb->p_line_data->line_data_received,
                                    ICF_CFG_LINE_DATA_REALM_RECD)) ||
                                  ((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD ==
                                    (p_user_db->flags & ICF_USER_DB_REALM_RECD))))                                      
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
                                            /*CSR_1-7009954 Fixed*/
                                            /* Check if realm is configured per user then
                                             * compare with realm configured per user not
                                             * with the realm value configured per line.
                                             */
                                            if((ICF_NULL != p_user_db) && (ICF_USER_DB_REALM_RECD == 
                                                    (p_user_db->flags & ICF_USER_DB_REALM_RECD)))                                            
                                            {
                                               if(0 != icf_port_strcmp(
                                                            p_user_db->configured_realm.str,
                                                            temp_unq_realm.str))
                                               {
                                                    realm_match = ICF_FALSE;
                                                    ICF_PRINT(((icf_uint8_t *)"\n[SSA] Option challenge: "\
                                                    "challenge realm not same as configured"));
                                               }
                                            }
                                            else if (0 != icf_port_strcmp(
                                              p_ssa_pdb->p_line_data->configured_realm.str,
                                              temp_unq_realm.str))
                                            {
                                                    realm_match = ICF_FALSE;
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
          
            if ((ICF_SUCCESS == ret_val) && (ICF_TRUE == realm_match))
            {	
		  p_ssa_pdb->p_method = (icf_uint8_t *)"OPTIONS"; 
                ret_val = icf_ssa_send_authorized_req(p_ssa_pdb);
                
                /* Failure in processing challenge */ 
                if(ICF_FAILURE == ret_val)
                {
                    /* Send response to SIC based on the recd response code */
                    p_ssa_pdb->dest_module_id = ICF_INT_MODULE_SIC;

                    /* Populate the internal message header */
                    p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
                        ICF_NW_INC_OPTIONS_RESP;
                    p_ssa_ctxt->p_sic_ctxt->error_code = *(p_ssa_pdb->p_ecode);

                    /* set ret_val as ICF_SUCCESS to send indication to app */
                    ret_val = ICF_SUCCESS;
                }
                else
                {
                   /* If OPTIONS with credentials was sent successfuly, increment the stats accordingly */
                   if(ICF_NULL == p_ssa_ctxt->p_call_ctxt)
	               {
                       /* Incremnent out of dialog OPTIONS stats */
                      ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                           stat_requests.stat_outbound_req.stats_outdlg_OPTION);
                   }
                   else
                   {
                      /* Incremnent in dialog OPTIONS stats */
                      ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                           stat_requests.stat_outbound_req.stats_indlg_OPTION);
                   }
                }
                ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return ret_val;
            }

	    }
			 
     	       /* Check for the presence of SDP in the received message */
          	   if(Sdf_co_success == sdf_ivk_uaIsSdpBodyPresent
					(pOverlapTransInfo->pSipMsg,pErr))
				{
                    /* Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR
                     * 19003)*/
                    p_ssa_pdb->p_method = (icf_uint8_t *)"OPTIONS";
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
                        /* Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR
                         * 19003)*/
                        /* SPR 19003: In case lists other than SDP list
                         * are populated then free the other lists as only
                         * the SDP list is used for evaluating response for
                         * OPTIONS message.
                         */ 
                        p_inc_media_sdp_info->p_sdp_info_list = ICF_NULL;
                        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
								&p_inc_media_sdp_info);
            
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
                /* Fix For SPR : 19135*/
                /* Do not set the sic context as null in ssa context.Setting
                 * this as NULL will prevent the setting of ssa context in sic
                 * context as null while application data (ssa context ) is
                 * freed by function sdf_cbk_uaFreeApplicationData*/
                /* In SIC module depending upon this check for ssa context in
                 * sic context we call delink ssa function.Since in case of
                 * TCP we have alredy freed the callobject before invoking the
                 * SIC module ,depending upon check for ssa context in sic 
                 * context delink function will be invoked and this results in
                 * FMR & FMW*/
				/*p_ssa_ctxt->p_sic_ctxt = ICF_NULL;*/
                
            } /* End if(p_ssa_ctxt->p_sic_ctxt) */
            else
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: response to OPTIONS ignored "));

                /* Merging CSR 1-6072476:If sic context is null then reset the dest_module_id */
                p_ssa_pdb->dest_module_id = ICF_INT_MODULE_INVALID;
                /* reset the msg_id */
                p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 0;        
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

        /* Merging CSR 1-6072476: No need to set the module id and msg id as 
        * this is already being set if the sic context is not null.
        * Code removed. */

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
#ifdef ICF_LOAD_STAT
        /* Code changes for updating the message stats if response is a success */
        if(200 == resp_code)
        {
          p_ssa_pdb->p_glb_pdb->load_data.otg_message_success++;
        }
#endif

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
            /* Fix For SPR : 19135*/
            /* Do not set the sic context as null in ssa context.Setting
             * this as NULL will prevent the setting of ssa context in sic
             * context as null while application data (ssa context ) is
             * freed by function sdf_cbk_uaFreeApplicationData*/
            /* In SIC module depending upon this check for ssa context in
             * sic context we call delink ssa function.Since in case of
             * TCP we have alredy freed the callobject before invoking the
             * SIC module ,depending upon check for ssa context in sic 
             * context delink function will be invoked and this results in
             * FMR & FMW*/            
			/*p_ssa_ctxt->p_sic_ctxt = ICF_NULL;*/
            
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
    icf_ssa_pdb_st           *p_ssa_pdb = ICF_NULL;

    Sdf_ty_u16bit       resp_code;
    Sdf_ty_u32bit       size = 0;
    Sdf_st_commonInfo   *p_common_info = Sdf_co_null;
    SipHeader           *p_contact_hdr = SIP_NULL;
    SipAddrSpec         *p_contact_spec = SIP_NULL;
    Sdf_ty_retVal       sdf_ret_val = Sdf_co_success;
    SipOptions          options;
    /* Added for forking support */
    icf_ssa_ctxt_st       *p_ssa_ctxt = ICF_NULL;
    icf_boolean_t         contact_present = ICF_TRUE;
    icf_uint8_t           max_contact_str[ICF_MAX_LARGE_STR_LEN] = "\0";

    /*Start:SPR 18846:Added null checks for the SSA context retrieval*/
    if((Sdf_co_null != pEventContext) &&
       (Sdf_co_null != pEventContext->pData))
    {    

       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pEventContext->pData);    
        /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
       if((ICF_NULL == p_ssa_pdb) ||
          (ICF_NULL == p_ssa_pdb->p_glb_pdb) ||     
          (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx))
       {
           ICF_PRINT(((icf_uint8_t *)"\nSSA PDB or Call Ctx is NULL.\n"))
           ret_val = ICF_FAILURE;
       }    
    }    
    else
    {
        ICF_PRINT(((icf_uint8_t *)"\nSSA Context is NULL.\n"))
        ret_val = ICF_FAILURE;
    }    
    if(ICF_FAILURE == ret_val)
    {   
        return Sdf_co_fail;
    }/*End SPR 18846 */    

    /*FORKING:- If This is not the call object on which last forked
     * response received then ignore the message*/
    /*if (p_ssa_ctxt != p_ssa_ctxt->p_call_ctxt->p_sip_ctx)
    {
        Discuss: if we have to send ACK, also check memory
         leak in this case
        return ICF_SUCCESS;
    }*/
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    /* update bitmask to indicate that now received response is 
       a final response */
    p_ssa_ctxt->bitmask |= ICF_FINAL_RESP_RECEIVED;
    
    /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18918)*/
    /* SPR 18918: If call has been redirected but it was previously challenged then
     * reset the challenge bitmask. So that on receipt of call redirection response
     * new call is initiated
     */
    if(ICF_CALL_AUTHENTICATION_REQ & p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)
	{
         ICF_SSA_RESET_COMMON_BITMASK(
			p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask,
			ICF_CALL_AUTHENTICATION_REQ)
	}

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
        /* To add security association headers*/
        else if(ICF_FAILURE ==
                icf_ssa_form_and_insert_security_agreement_hdrs(
                        p_ssa_pdb,(icf_uint8_t *)"ACK",
                       (*ppCallObj)->pUacTransaction->pSipMsg))
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
 /*  Check for 380 is added to merge CSR 1-5760906 */

 /*Rel8.1 If redirect_support_reqd is set to FALSE IPTK shall support 
  * all the 3xx responses apart from 380 as 302 (IPTK automatically 
  * initiate new INVITE), if set to true IPTK shall give final failure 
  * indication (in ICF_TERMINATE_CALL_IND API) to application and clear the call*/ 

 if ( (((300 <= resp_code) && (399 >= resp_code)) && (380 != resp_code)) 
    && (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->redirect_support_reqd))  
   {
		/*Block for handling of call redirection:302 for an INVITE*/

        icf_nw_inc_call_redirect_st 
            *p_call_redirect = 
            (icf_nw_inc_call_redirect_st *)
            (p_ssa_pdb->p_internal_msg->p_msg_data);

        /* Fix for SPR: 19886 (Merged CSR 1-7444867)*/
        /* Fix for SPR: 19788*/
        /* Free the diversion header if already present in call context*/
		/* SPR 19927 : the call context has already been checked for NULL so not needed
		 * here again. result into klocworks warning at all the locations where call
	     * context is accessed.
	     */	 
        if ((ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx->p_diversion_header_list))
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
        /* Fix for CSR 1-7208970: SPR 19589 */
		/* Fill App requested headers from 302 response */
     	else if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
            				p_ssa_pdb,ICF_INT_MODULE_CC,
        					(icf_void_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx,
            				(*ppCallObj)->pUacTransaction->pSipMsg,
            				p_ssa_pdb->p_ecode) )
        {
            ret_val = ICF_FAILURE;
            sdf_ret_val = Sdf_co_fail;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return sdf_ret_val;
        }
        else
        {
            /*SPR 19886 Fix for CSR 1-7208970: The function
            icf_ssa_populate_app_specific_hdrs creates a node for the
            received 302 but before sending call redirection indication
            to application redirected INVITE is sent and in that same
            headers should be sent as for initial INVITE therefore the
            current txn id is decreased by one which shall now be 
            corresponding to the initial INVITE. The 302 corresponding
            headers shall be sent in the icf_cc_send_pa_forwarding_ind
            function where we shall be compensating for this decrement by
            incrementing the txn_id call redirection API shall have headers
            corresponding to 302*/
            p_ssa_pdb->p_glb_pdb->p_call_ctx->txn_id--;
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
                /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19026)*/                
                /*If the value of size is 0 set the value of contact_present
                 * as ICF_FALSE ; it specifies contact header is not received
                 * in 3XX response.*/
                /*Also set the value of ret_val as ICF_FAILURE ; this will
                 * prevent the execution of following success legs , which are
                 * not required when contact header is not present*/                
                else if (ICF_NULL == size)
                {
                    ret_val = ICF_FAILURE;
                    contact_present = ICF_FALSE;
                }                
                /*We are handling only one contact address */
                else if (Sdf_co_fail == sdf_listGetAt(&(p_common_info->slContact),0,
                                             (Sdf_ty_pvoid *)&p_contact_hdr, pErr))
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
                 /* Fill the contact addr type. If address type is sip
                    fill it is SIP_URI else as SIPS_URI */
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

					if(ICF_NULL != p_contact_spec->u.pSipUrl->pUser)
					{
                            /*SPR 19571/19484 - merge CSR 7132323*/
                            icf_port_strcpy(max_contact_str,
                                    (icf_uint8_t *)p_contact_spec->u.pSipUrl->pUser);
                            icf_port_strcat(max_contact_str,(icf_uint8_t *)"@");
                            icf_port_strcat(max_contact_str,
                                    (icf_uint8_t *)p_contact_spec->u.pSipUrl->pHost);

					}
					else
					{
                            /*SPR 19571/19484 - merge CSR 7132323*/
							icf_port_strcpy(max_contact_str
                                  ,(icf_uint8_t *) p_contact_spec->u.pSipUrl->pHost);
					}
					
                    if ( ICF_NULL != p_contact_spec->u.pSipUrl->dPort)
                    {
                        icf_uint8_t  p_contact_port[6] = "\0";
                        icf_port_strcat(max_contact_str,(icf_uint8_t *)":");
                        icf_port_snprintf(
                                (icf_uint8_t *)p_contact_port,
                                ICF_PORT_SIZEOF(p_contact_port),(icf_uint8_t*)"%d",
                                *(p_contact_spec->u.pSipUrl->dPort));
                        icf_port_strcat(max_contact_str, p_contact_port);
                    }
                    /* CSR 1-6013602 Merge: Add all the received SIP parameters in the 
                    * contact address string
                    */ 
                   ret_val = icf_ssa_add_param_in_uri_address(
                                  p_contact_spec->u.pSipUrl,
                                  max_contact_str,
                                  p_ssa_pdb->p_ecode);
                   /* Codenomicon fixes: CSR 1-7132323: SPR 19484 */
					/* String Copy should not exceed ICF_MAX_STR_LEN value */
					/* Copy only max of 90 bytes of user addr to keep space for 
				 	 * othe contact header parameters.
					 */
                   icf_port_strncpy(
                            (icf_int8_t *)p_div_hdr_list->p_contact_addr->addr_val.str,
                            (const icf_int8_t *)max_contact_str,
						    (ICF_MAX_STR_LEN - 1));
					p_div_hdr_list->p_contact_addr->addr_val.str[ICF_MAX_STR_LEN - 1] = '\0';
					p_div_hdr_list->p_contact_addr->addr_val.str_len = 
								(icf_uint16_t )icf_port_strlen(
									p_div_hdr_list->p_contact_addr->addr_val.str);

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
                            /* Codenomicon fixes: CSR 1-7132323: SPR 19484 */
							/* String Copy should not exceed ICF_MAX_STR_LEN value */
                            icf_port_strncpy(
                                    (icf_int8_t *)p_div_hdr_list->p_contact_name->str,
                                    (const icf_int8_t *)p_contact_disp_name,
									(ICF_MAX_STR_LEN - 1));
							p_div_hdr_list->p_contact_name->str[ICF_MAX_STR_LEN - 1] = '\0';
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
                            /* Codenomicon fixes: CSR 1-7132323: SPR 19484 */
							/* String Copy should not exceed ICF_MAX_STR_LEN value */
                            icf_port_strncpy(
                                    (icf_int8_t *)p_div_hdr_list->p_contact_name->str,
                                    (const icf_int8_t *)p_contact_disp_name,
									(ICF_MAX_STR_LEN - 1));
							p_div_hdr_list->p_contact_name->str[ICF_MAX_STR_LEN - 1] = '\0';
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
                /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19026)*/
                /*As per RFC 3261 contact header in 3xx response in not 
                  mandatory,also SIP_CC_OE_CE_V_028 ETSI test plan states that
                  for such scenario UAC must send ACK with same request URI as
                  original INVITE */
                /*As a fix for SPR if contact header is not present in 3xx 
                  response form the internal message with message id as 
                  ICF_NW_INC_CALL_REDIRECT which in turn trigger CC and ACK
                  will be send with same request URI as original INVITE*/
                else if (ICF_FALSE == contact_present)
                {                        
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
        /* CSR 1-6307543: Send 302 received trace to application */
#ifdef ICF_TRACE_ENABLE
		if(302 == resp_code)
		{
            p_ssa_pdb->p_method= (icf_uint8_t *)"INVITE";
            ICF_SSA_APP_TRACE(p_ssa_pdb,ICF_TRACE_302_RECVD)
        }
#endif
#ifdef ICF_LOAD_STAT
        p_ssa_pdb->p_glb_pdb->load_data.num_of_302_reponse_recv++;
#endif        
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
            /* Populate application specified headers */
            if (ICF_FAILURE == icf_ssa_populate_app_specific_hdrs(
                        p_ssa_pdb,ICF_INT_MODULE_CC, 
                        (icf_void_t*)( p_ssa_pdb->p_glb_pdb->p_call_ctx ),
                        (*ppCallObj)->pUacTransaction->pSipMsg,
                        p_ssa_pdb->p_ecode))
            {
                ICF_PRINT(((icf_uint8_t *)"\nUnable to get application specific headers"));

                ret_val = Sdf_co_fail;
            }

            /* Populate the message body */

            /* SPR 19590 : Start */
            else if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
                        (*ppCallObj)->pUacTransaction->pSipMsg,\
                        SipMessageResponse))
            /* SPR 19590 : End */
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInMsgBody retreival from MESSAGE"));
                ret_val = ICF_FAILURE;
            }


            /*We will send a IncCallReject API to CM*/
            /* Populate the internal message header */
            p_ssa_pdb->p_internal_msg->msg_hdr.msg_id =
                ICF_NW_INC_CALL_REJECT;
            p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
        }
    }
    /* Fix For SPR:20124*/
    /* Update the inc_response_code in call context so that same can be filled
     * in ICF_TERMINATE_CAL_IND API if the validation of contact header of 302
     * response is failed in CC.*/
    p_ssa_pdb->p_glb_pdb->p_call_ctx->inc_response_code = resp_code;

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

    /*  SPR 18064 Merge */
    p_info_resp->error_cause = ICF_NULL;

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
        if(ICF_FAILURE == icf_ssa_get_msg_body_frm_sip_message_in_call_ctx(p_ssa_pdb,\
                    p_message,\
                    SipMessageResponse))
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
        /*  SPR 18064 Merge */
        p_info_resp->status = ICF_FALSE;
	      if((401 == resp_code) || (407 == resp_code))     
      	{            /*Check for Auth params*/         
      	    icf_ssa_fetch_auth_param_list(p_ssa_pdb,                  
      			    ppCallObj,ICF_NULL);                   
      	    if(ICF_NULL == p_ssa_ctxt->auth_params_list_size)    
      	    {               
      	        p_info_resp->error_cause =
      			    ICF_CAUSE_AUTH_PARAM_NOT_RCVD;                 
      	    }      
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

/**************************************************************************************
 ** FUNCTION:  sdf_cbk_uaGetPrivateKeyPassword
 **
 ** DESCRIPTION: This function will set the password in the output parameter
 **              buf and return the length of the password. While constructing
 **              the SSL_CTX, UATK would have set the password as the callback
 **              userdata using the API wrapper_SSL_CTX_set_default_passwd_cb_userdata.
 **              The same userdata will be provided as an input argument to
 **              this callback function.
 **
 ***************************************************************************************/
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
   /*Start SPR 18846: Added null checks*/ 
   if((Sdf_co_null != pOrigCallObj->pAppData) &&
      (Sdf_co_null != pOrigCallObj->pAppData->pData))
   {   
      /* Extract ssa context from original call object for removing it */
      p_ssa_ctxt = (icf_ssa_ctxt_st *)(pOrigCallObj->pAppData->pData);
      p_temp_ssa_ctxt = p_ssa_ctxt;
      if(ICF_NULL != p_ssa_ctxt->p_call_ctxt)
      {  
         /* Extract the call context from the SSA context */
         p_call_ctxt = (icf_call_ctx_st *)(p_ssa_ctxt->p_call_ctxt);
      } 
      else
      { 
          ICF_PRINT(((icf_uint8_t *)"\nCall Context is NULL.\n"))
          ret_val = Sdf_co_fail;
      }
   }
   else
   {
       ICF_PRINT(((icf_uint8_t *)"\nSSA Context in Org Call Obj is NULL.\n"))
       ret_val = Sdf_co_fail;
   }

   if((Sdf_co_null != pForkCallObj->pAppData) &&
      (Sdf_co_null != pForkCallObj->pAppData->pData))
   {
       /* Extract the pointer to SSA CTXT from the event context */
       p_ssa_ctxt = (icf_ssa_ctxt_st *)(pForkCallObj->pAppData->pData);
       /* Extract the pointer to SSA PDB from the event context */
       p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
   }   
   else
   {
      ICF_PRINT(((icf_uint8_t *)"\nSSA Context in Fork Call Obj is NULL.\n"))
      ret_val = Sdf_co_fail;
   }    
   if((Sdf_co_fail == ret_val) ||
       (ICF_NULL == p_ssa_pdb))
   {   
       ICF_PRINT(((icf_uint8_t *)"\nSSA Ctx/PDB is NULL.\n"))
       return Sdf_co_fail;
   }/*End SPR 18846 */ 

    /* update new ssa context with original call context value */

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* SPR 18817: If call context is not present ie the call has been already 
     * forcefully cleared, no need to process the message.
     */     
    if(ICF_NULL == p_call_ctxt)
    {
        return Sdf_co_success;
    }   
    /*IPTK will ignore Forked response received for a precondition call*/
    else if (ICF_PRECONDITION_REQD ==
            (p_call_ctxt->common_bitmask_2 & ICF_PRECONDITION_REQD))
    {
        return Sdf_co_success;
    }
    
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

        /* CSR_1_7113139 Merged SPR 19672 Start */
       	/* Fix for CSR 1-7113139: SPR 19462 */
        /* In case of forked responses, new SSA context will contain the
         * SDP version initialized to ICF_START_SDP_O_VERSION.
         * Increment this by one because normally this is incremented
         * while sending initial INVITE but that would have not 
         * occurred for this SSA context.
         */
        if (ICF_START_SDP_O_VERSION == p_ssa_ctxt->sdp_o_version)
        {
            (p_ssa_ctxt->sdp_o_version)++;
        }
        /* CSR_1_7113139 Merged SPR 19672 End */


       
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
            /* To add security association headers*/
            else if(ICF_FAILURE ==
                     icf_ssa_form_and_insert_security_agreement_hdrs(
                       p_ssa_pdb,(icf_uint8_t *)"ACK",
                       (pForkCallObj)->pUacTransaction->pSipMsg))
            {
               ret_val = ICF_FAILURE;
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


