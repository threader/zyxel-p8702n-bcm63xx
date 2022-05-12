/*------------------------------------------------------------------------------
 *
 * File name        : icf_internal_api_id.h
 *
 * Purpose          : This file contains all the internal api ids used by all
 *                    internal modules of ICF for communication with each
 *                    other.
 *
 * Revision History :
 *
 * Date         Name            Ref#        Comments
 * --------     ------------    ----        ---------------
 * 03-Dec-2004  Pankaj Negi     HSS LDD     Initial
 * 21-Dec-2004  Rohit Aggarwal              Changed #define for MMS request
 * 25-Jan-2005  Navneet Gupta               Added API's
 * 21-Apr-2005  Amit Sharma                 Map call_release with 
 *                                          generic_call_clear
 * 25-May-2005  Umang Singh                 Added API for RE-INVITE ACK
 * 02-Jun-2005  Amit Sharma     SPR:7535    Added internal TWC resp event
 * 08-Jun-2005  Jalaj Negi                  Corrected collision in API IDs
 *                                          by incrementing API ID values by
 *                                          10 from ICF_INT_API_MEDIA_START
 *                                          onwards.
 * 06-Dec-2005	Jyoti Narula	ICF 4.0		Api ID for AUTO_REGISTER
 * 07-Dec-2005  Rohit Aggarwal  ICF 4.0 CDD Code changes for IPSec support
 * 13-Jan-2005  Aman Arora      IPSEC       Added APIs for IPSec- DELETE_SA,
 *                                          CLOSE_SA_SERVER, OPEN_SA_SERVER
 * 01-Mar-2006     Amit Sharma              ICF merging with ICF2.0
 * 07-Mar-2006	Anuradha Gupta	ICF			Added the new APIs for Re-invite handling
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0     Call Modify Feature changes
 * 03-Jan-2007  Amit Mahajan                INFO confirmation changes.
 * 26-MAr-2007  Deepti Goyal    ICF 6.4     Changes for media connect req. API.
 * 09-Jul-2007  Amit Sharma     ICF 7.0     Added changes for 7.0 enhancements
 * 07-Jun-2008  Amit Sharma     IPTK 8.0 SRTP LLD      SRTP Enhancements
 * 09-Feb-2009  Anurag Khare    ICF 8.1     OPTIONS Enhancements
 * 30-Mar-2009  Tarun Gupta     Rel 8.2     Refer/Notify Enhancements
 * 02-Jun-2009  Preksha         Rel 8.2     Asynchronus MESSAGE Enhancements
 *
 * Copyright (c) 2009, Aricent
 *----------------------------------------------------------------------------*/

#ifndef __ICF_INTERNAL_API_ID_H__
#define __ICF_INTERNAL_API_ID_H__


/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

#include "icf_map_extif_feature_flags.h"
/* The above File needs to be included for SIGCOMP INCLUSION */

#define ICF_INT_API_START                 ICF_EXTERNAL_API_END + 1


/* This Api is used to inform that user has sent req to create call*/
#define ICF_USER_CREATE_CALL              (ICF_INT_API_START)

/* This Api is used to inform that user has been alerted by PA*/
#define ICF_USER_CALL_ALERT               (ICF_INT_API_START + 1)

/* This Api is used to inform that PA has sent connect */
#define ICF_USER_CALL_CONNECT             (ICF_INT_API_START + 2)

/* This Api is used to inform that PA has put call on hold */
#define ICF_USER_CALL_HOLD                (ICF_INT_API_START + 3)

/* This Api is used to inform that PA has put call on resume */
#define ICF_USER_CALL_RESUME              (ICF_INT_API_START + 4)

/* This Api is used to inform that PA has initated call confrence */
#define ICF_USER_CALL_CONF                (ICF_INT_API_START + 5)

/* This Api is used to inform that user has terminated the call */
#define ICF_USER_CALL_TERMINATE           (ICF_INT_API_START + 6)

/* This Api is used to inform that user has terminated the call */
#define ICF_USER_CALL_TERMINATE_RESP      (ICF_INT_API_START + 7)

/* This Api is used to inform that user has terminated the call */
#define ICF_USER_CALL_MUTE                (ICF_INT_API_START + 8)

/* This Api is used to inform that user has terminated the call */
#define ICF_USER_CALL_UNMUTE              (ICF_INT_API_START + 9)

/* This Api is used to inform that user has invoked CTA */
#define ICF_USER_CALL_TRANSFER_ATTENDED   (ICF_INT_API_START + 10)

/* This Api is used to inform that user has invoked CTU */
#define ICF_USER_CALL_TRANSFER_UNATTENDED (ICF_INT_API_START + 11)

#define ICF_USER_INFO					 (ICF_INT_API_START + 12)

/*ICF_HANDLE_REINVITE_START*/
/* This Api is used to inform that user responded to the incoming call modify req */
#define ICF_USER_CALL_MODIFY_CFM			(ICF_INT_API_START + 13)

/* This Api is used to inform that user has requested for the call modify */
#define ICF_USER_CALL_MODIFY_REQ			(ICF_INT_API_START + 14)

#define ICF_USER_START_FAX_REQ			  (ICF_INT_API_START + 15)
/*ICF_HANDLE_REINVITE_END*/

#define ICF_USER_INFO_CFM                     (ICF_INT_API_START + 16)

/* confirmation for early incoming call indication */
#define ICF_USER_EARLY_CALL_CFM                 (ICF_INT_API_START + 17)
#define ICF_USER_CALL_MEDIA_CONNECT                 (ICF_INT_API_START + 18)

 #define ICF_USER_STOP_FAX_REQ       (ICF_INT_API_START + 19)

  /*cta_handling*/
#define ICF_USER_CALL_TRANSFER_INITIATED_CFM          (ICF_INT_API_START + 20)

#define ICF_USER_CALL_MOD_CANCEL_REQ       (ICF_INT_API_START + 21)

#define ICF_USER_SESSION_TIMER_TOGGLE_REQ       (ICF_INT_API_START + 22)

/* This API is used by CM to inform CC that application has
   sent a request to send a generic message on the network.
   Currently this API will be mapped to CC event ICF_CC_PA_GENERIC_MSG_REQ.
*/
#define ICF_USER_GENERIC_MSG_REQ        (ICF_INT_API_START + 23)    

/* This API is used by CM to inform CC that application has 
   sent a response to generic message received from the network.
   Currently this API will be mapped to CC event ICF_CC_PA_GENERIC_MSG_CFM
*/
#define ICF_USER_GENERIC_MSG_CFM    (ICF_INT_API_START + 24)    
    

/* Start of NW API's */
#define ICF_INT_API_NW_OTG_START          (ICF_INT_API_START + 25)


/* In case adding new API on user interface change the counter */


/* This Api is used to inform that Call setup has to be initated towards NW */
#define ICF_NW_OTG_CALL_SETUP             (ICF_INT_API_NW_OTG_START)

/* This Api is used to inform that Call setup ACK has to be sent towards NW */
#define ICF_NW_OTG_CALL_SETUP_ACK         (ICF_INT_API_NW_OTG_START + 1)

/* This Api is used to inform that local user has been alerted */
#define ICF_NW_OTG_CALL_ALERT             (ICF_INT_API_NW_OTG_START + 2)

/* This Api is used to inform that local user has been alerted and local tone/
 * annc has to be played.*/
#define ICF_NW_OTG_ALERT_WITH_MEDIA        (ICF_INT_API_NW_OTG_START + 3)

/* This Api is used to inform that local media has been committed. */
#define ICF_NW_OTG_MEDIA_PARAM             (ICF_INT_API_NW_OTG_START + 4)

/* This Api is used to inform that local user has accepted the call */
#define ICF_NW_OTG_CONNECT                 (ICF_INT_API_NW_OTG_START + 5)

/* This Api is used to inform that connect from nw has been processed.*/
#define ICF_NW_OTG_CONNECT_ACK             (ICF_INT_API_NW_OTG_START + 6)

/* This Api is used to inform that local entity wants to reject the call */
#define ICF_NW_OTG_CALL_REJECT             (ICF_INT_API_NW_OTG_START + 7)

/* This Api is used to inform that local party has released call */
#define ICF_NW_OTG_CALL_TERMINATE          (ICF_INT_API_NW_OTG_START + 8)

/* This Api is used to inform that call release from remote party
 * has been accesspted */
#define ICF_NW_OTG_CALL_TERMINATE_RESP     (ICF_INT_API_NW_OTG_START + 9)

/* This Api is used to inform that remote party has redirected call */
#define ICF_NW_OTG_CALL_REDIRECT           (ICF_INT_API_NW_OTG_START + 10)

/* This Api is used to inform that local media update has taken place */
#define ICF_NW_OTG_MEDIA_UPDATE            (ICF_INT_API_NW_OTG_START + 11)

/*This API is used to indicate that CANCEL has to be sent on NW*/
#define ICF_NW_OTG_CALL_ABORT              (ICF_INT_API_NW_OTG_START + 12)

/* This API is used to indicate that PRACK for media param has been
 * accepted*/
#define ICF_NW_OTG_MEDIA_PARAM_PRACK_ACK   (ICF_INT_API_NW_OTG_START + 13)

/* This API is used to indicate that PRACK for alert has been
 * Accepted*/
#define ICF_NW_OTG_ALERT_PRACK_ACK         (ICF_INT_API_NW_OTG_START + 14)

/* This Api is used to inform that reject from remote entity has been
 * accepted */
#define ICF_NW_OTG_CALL_REJECT_ACK         (ICF_INT_API_NW_OTG_START + 15)

/* This API is used to indicate that PRACK for media param has to be sent
 * to NW*/
#define ICF_NW_OTG_MEDIA_PARAM_PRACK       (ICF_INT_API_NW_OTG_START + 16)

/* This API is used to indicate that PRACK for alert has to be sent
 * to NW*/
#define ICF_NW_OTG_ALERT_PRACK             (ICF_INT_API_NW_OTG_START + 17)

/* This Api is used to inform that media update has been processed*/
#define ICF_NW_OTG_MEDIA_UPDATE_RESP       (ICF_INT_API_NW_OTG_START + 18)

/* This Api is used to inform that refer needs to be sent on NW*/
#define ICF_NW_OTG_REFER                   (ICF_INT_API_NW_OTG_START + 19)

/* This Api is used to inform that refer has been processed */
#define ICF_NW_OTG_REFER_RESP              (ICF_INT_API_NW_OTG_START + 20)

/* This Api is used to inform that Notify needs to be sent on NW*/
#define ICF_NW_OTG_NOTIFY                  (ICF_INT_API_NW_OTG_START + 21)

/* This Api is used to inform that Notify from NW has been processed.*/
#define ICF_NW_OTG_NOTIFY_RESP             (ICF_INT_API_NW_OTG_START + 22)

/* This API is used to indicate that PRACK for redirection indication has to
 * be sent to NW*/
#define ICF_NW_OTG_REDIRECT_IND_PRACK      (ICF_INT_API_NW_OTG_START + 23)

/* This API is used to indicate that  Ack for redirection indication has to
 * be sent to NW*/
#define ICF_NW_OTG_CALL_REDIRECT_ACK       (ICF_INT_API_NW_OTG_START + 24)

/* This API is used to indicate that  Ack for abort indication has to
 * be sent to NW*/
#define ICF_NW_OTG_CALL_ABORT_RESP         (ICF_INT_API_NW_OTG_START + 25)

/* This API is used to indicate that the pending refer transaction has to be aborted in SSA*/
#define ICF_NW_OTG_REFER_ABORT_REQ         (ICF_INT_API_NW_OTG_START + 27)

/* This API is used to send Cancel for Re-Invite/Update to NW */
#define ICF_NW_OTG_MEDIA_UPDATE_ABORT_REQ  (ICF_INT_API_NW_OTG_START + 28)

#ifdef ICF_SESSION_TIMER
/* This API is used by SSA to send session timer expired indication to CC*/
#define ICF_NW_OTG_SESSION_REFRESH_IND     (ICF_INT_API_NW_OTG_START + 29)

/* This API is used by SSA to send session refresh request is acknowleged to CC*/
#define ICF_NW_OTG_SESSION_REFRESH_ACK     (ICF_INT_API_NW_OTG_START + 30)

/* This API is used by UATK callbacks to send session refresh request to N/W*/
#define ICF_NW_OTG_SESSION_REFRESH_REQ     (ICF_INT_API_NW_OTG_START + 31)

/* This API is used by CFG to set the minimum session timer value*/
#define ICF_CFG_SSA_SET_MIN_SE			  (ICF_INT_API_NW_OTG_START + 32)
#endif

/* This API is used to send INFO req to NW */ 
#define ICF_NW_OTG_INFO_REQ			  (ICF_INT_API_NW_OTG_START + 33)

/*ICF_HANDLE_REINVITE_START*/

/* This Api is used to inform that local call modify has taken place */
#define ICF_NW_OTG_CALL_MODIFY_REQ		 (ICF_INT_API_NW_OTG_START + 34)	
#define ICF_NW_OTG_OPTIONS_ABORT_REQ    (ICF_INT_API_NW_OTG_START + 35)

/* This API is used by CC to inform SSA that application has
   sent a request to send a generic message on the network.
   Currently this API will be issued towards SSA when in-call
   REFER/NOTIFY request is received from the application in tunnel mode.
*/
#define ICF_NW_OTG_GENERIC_MSG_REQ    (ICF_INT_API_NW_OTG_START + 36)    

/* This API is used by CC to inform SSA that application has 
   sent a response to generic message received from the network.
   Currently this API will be issued towards SSA when a response to an in-call
   REFER/NOTIFY request received has to be sent to the peer in tunnel mode.
*/
#define ICF_NW_OTG_GENERIC_MSG_CFM    (ICF_INT_API_NW_OTG_START + 37)    

/*ICF_HANDLE_REINVITE_END*/

    /* Start of INC API is ICF_INT_API_START+ICF_INT_API_NW_OTG_START(12)
     * + 50. Direct Macro to depict this addition has not been done so as to
     * reduce the code area as after pre-processing it shall show x+y+z.*/
#define ICF_INT_API_NW_INC_START           (ICF_INT_API_START + 60)

/* This Api is used to inform that Call setup has been received from NW */
#define ICF_NW_INC_CALL_SETUP              (ICF_INT_API_NW_INC_START)

/* This Api is used to inform that Call setup ACK has been received from NW */
#define ICF_NW_INC_CALL_SETUP_ACK          (ICF_INT_API_NW_INC_START + 1)

/* This Api is used to inform that remote user has been alerted */
#define ICF_NW_INC_CALL_ALERT              (ICF_INT_API_NW_INC_START + 2)

/* This Api is used to inform that remote user has been alerted and remote tone/
 * annc has to be played.*/
#define ICF_NW_INC_ALERT_WITH_MEDIA        (ICF_INT_API_NW_INC_START + 3)

/* This Api is used to inform that remote media has been committed. */
#define ICF_NW_INC_MEDIA_PARAM             (ICF_INT_API_NW_INC_START + 4)

/* This Api is used to inform that remote user has accepted the call */
#define ICF_NW_INC_CONNECT                 (ICF_INT_API_NW_INC_START + 5)

/* This Api is used to inform that nw has processed the connect sent.*/
#define ICF_NW_INC_CONNECT_ACK             (ICF_INT_API_NW_INC_START + 6)

/* This Api is used to inform that remote entity wants to reject the call */
#define ICF_NW_INC_CALL_REJECT             (ICF_INT_API_NW_INC_START + 7)

/* This Api is used to inform that remote party has released call */
#define ICF_NW_INC_CALL_TERMINATE          (ICF_INT_API_NW_INC_START + 8)

/* This Api is used to inform that call release from remote party
 * has been accepted */
#define ICF_NW_INC_CALL_TERMINATE_RESP     (ICF_INT_API_NW_INC_START + 9)

/* This Api is used to inform that remote party has redirected call */
#define ICF_NW_INC_CALL_REDIRECT           (ICF_INT_API_NW_INC_START + 10)

/* This Api is used to inform that remote media update has taken place */
#define ICF_NW_INC_MEDIA_UPDATE            (ICF_INT_API_NW_INC_START + 11)

/* This API is used to indicate that CANCEL has been received from NW*/
#define ICF_NW_INC_CALL_ABORT            (ICF_INT_API_NW_INC_START + 12)

/* This API is used to indicate that PRACK for media param has been
 * received from NW*/
#define ICF_NW_INC_MEDIA_PARAM_PRACK     (ICF_INT_API_NW_INC_START + 13)

/* This API is used to indicate that PRACK for alert has been
 * received from NW*/
#define ICF_NW_INC_ALERT_PRACK           (ICF_INT_API_NW_INC_START + 14)

/* This Api is used to inform that reject from local entity has been
 * accepted */
#define ICF_NW_INC_CALL_REJECT_ACK       (ICF_INT_API_NW_INC_START + 15)

/* This Api is used to inform that remote has responded for media update sent*/
#define ICF_NW_INC_MEDIA_UPDATE_RESP     (ICF_INT_API_NW_INC_START + 16)

/* This Api is used to inform that refer has been received from NW*/
#define ICF_NW_INC_REFER                (ICF_INT_API_NW_INC_START + 17)

/* This Api is used to inform that refer has been processed */
#define ICF_NW_INC_REFER_RESP           (ICF_INT_API_NW_INC_START + 18)

/* This Api is used to inform that Notify has been received from NW*/
#define ICF_NW_INC_NOTIFY               (ICF_INT_API_NW_INC_START + 19)

/* This Api is used to inform that Notify from NW has been processed.*/
#define ICF_NW_INC_NOTIFY_RESP          (ICF_INT_API_NW_INC_START + 20)

/* This Api is used to inform that remote party has redirected call at
 * its end and is indicating it to the originating party.*/
#define ICF_NW_INC_REDIRECT_IND         (ICF_INT_API_NW_INC_START + 21)

/* This API is used to indicate that CANCEL has been received from NW*/
#define ICF_NW_INC_CALL_ABORT_RESP      (ICF_INT_API_NW_INC_START + 22)

/* This API is used to indicate that ACK for RE-INVITE is received from NW  */
#define ICF_NW_INC_MEDIA_CHANGE_ACK      (ICF_INT_API_NW_INC_START + 23)

#define ICF_NW_INC_INFO_RESP		    (ICF_INT_API_NW_INC_START + 24)		

/* This Api is used to inform that remote has responded for media PRACK sent*/
#define ICF_NW_INC_MEDIA_PARAM_PRACK_RESP     (ICF_INT_API_NW_INC_START + 25)

#define ICF_SSA_INTERNAL_TIMER_EXPIRY    (ICF_INT_API_NW_INC_START + 26)

#define ICF_NW_INC_INFO_IND				 (ICF_INT_API_NW_INC_START + 27)

#define ICF_NW_INC_INFO_CFM              (ICF_INT_API_NW_INC_START + 28)     

#define ICF_NW_INC_CANCEL_COMPLETE_RESP  (ICF_INT_API_NW_INC_START + 29)

/* This API is used by SSA to inform CC that a progress message has been 
 * received for the outgoing INVITE (181/182/183 without sdp)
 */
#define ICF_NW_INC_CALL_PROGRESS	  (ICF_INT_API_NW_INC_START + 30)

/*This API is used from SSA to invoke SRTP fallback procedure by CC*/
#define ICF_SRTP_FALLBACK_REQ             (ICF_INT_API_NW_INC_START + 31)

/* This API is used by SSA to inform CC that a generic message has 
   been received from the network. Currently this API will be issued
   towards CC when in-call REFER/NOTIFY is received from the network in tunnel mode.
*/
#define ICF_NW_INC_GENERIC_MSG_IND  (ICF_INT_API_NW_INC_START + 32)

/* This API is used by SSA to inform CC that a response to a generic message sent has 
   been received from the network. Currently this API will be issued
   towards CC when a response to an in-call REFER/NOTIFY sent is received from the network in tunnel mode.
*/
#define ICF_NW_INC_GENERIC_MSG_RESP  (ICF_INT_API_NW_INC_START + 33)
    

   /* Start of Media API is ICF_INT_API_START+ICF_INT_API_NW_OTG_START(10)
   * ICF_INT_API_NW_INC_START (30)+ 30. Direct Macro to depict this addition
   * has not been done so as to
   * reduce the code area as after pre-processing it shall show x+y+z.*/

#define ICF_INT_API_MEDIA_START           (ICF_INT_API_START + 70 + 10+10)

/* This Api is used to initate Create Media Session toward MMI */
#define ICF_CREATE_MEDIA_REQ         (ICF_INT_API_MEDIA_START)

/* This Api is used to initate Modify Media Session toward MMI */
#define ICF_MODIFY_MEDIA_REQ         (ICF_INT_API_MEDIA_START + 1)

/* This Api is used to initate Delete Media Session toward MMI */
#define ICF_DELETE_MEDIA_REQ         (ICF_INT_API_MEDIA_START + 2)

/* This Api is used to initate  confrence request toward MMI */
#define ICF_CREATE_CONF_REQ          (ICF_INT_API_MEDIA_START + 3)

/* This Api is used by MMI to send Create Media Session Response  */
#define ICF_CREATE_MEDIA_RESP        (ICF_INT_API_MEDIA_START + 4)

/* This Api is used by MMI to send Modify Media Session Response  */
#define ICF_MODIFY_MEDIA_RESP        (ICF_INT_API_MEDIA_START + 5)

/* This Api is used by MMI to send Delete Media Session Response  */
#define ICF_DELETE_MEDIA_RESP        (ICF_INT_API_MEDIA_START + 6)

/* This Api is used by MMI to send Create Media Session Response  */
#define ICF_CREATE_CONF_RESP         (ICF_INT_API_MEDIA_START + 7)

/* This Api is used by MMI to indicate failure from Media Manager */
#define ICF_MEDIA_ERROR_IND          (ICF_INT_API_MEDIA_START + 8)

/* This API is used bye SSA to request for media capabilities whemn it received 
 * options request from network */    
#define ICF_GET_MEDIA_CAP_REQ        (ICF_INT_API_MEDIA_START + 9)

/* This API is used bye MMI to send back media capabilities response to SSA when
 * it received media capabilities request from SSA */    
#define ICF_GET_MEDIA_CAP_RESP       (ICF_INT_API_MEDIA_START + 10)

/* This API is used by CFG to request for configuring the IP for RTP transport
 * options request from network */
#define ICF_CONFIG_SELF_IP_REQ       (ICF_INT_API_MEDIA_START + 11)
		
    /* Start of CM API is ICF_INT_API_START+ICF_INT_API_NW_OTG_START(10)
     * ICF_INT_API_NW_INC_START(30) + ICF_INT_API_MEDIA_START(30) + 20.
     * Direct Macro to depict this addition has not been done so as to
     * reduce the code area as after pre-processing it shall show x+y+z.*/

#define ICF_INT_API_CM_START           (ICF_INT_API_START + 80 + 20+10)

/*This API is used by CM to indicate start media*/
#define ICF_START_PROC_SIGNALING       (ICF_INT_API_CM_START)

/*This API is used by CM to indicate start Signaling*/
#define ICF_START_PROC_MEDIA           (ICF_INT_API_CM_START + 1)

/* This API is used to indicate that the calls have to be cleared forcefully
 * */
#define ICF_FORCEFUL_CALL_CLEAR_IND     (ICF_INT_API_CM_START + 3)

/*This API indicates that message from MRM is received*/
#define ICF_MSG_FROM_MRM                (ICF_INT_API_CM_START + 4)

/*This API indicates that internal call hold request is received*/
#define ICF_SC_INTERNAL_CALL_HOLD_REQ   (ICF_INT_API_CM_START + 5)

/*This API indicates that internal call resume request is received*/
#define ICF_SC_INTERNAL_CALL_RESUME_REQ (ICF_INT_API_CM_START + 6)

/*This API indicates that RNW Service shoud reject the incoming REFER*/
#define ICF_SC_INTERNAL_RNW_REJ         (ICF_INT_API_CM_START + 7)

/*This API indicates that call need to be cleared due to generic failure/cfg*/
#define ICF_GENERIC_CALL_CLEAR          (ICF_INT_API_CM_START + 8)

/*This API is used to indicate that call is released to sevice logics*/
#define ICF_CC_CALL_RELEASED            ICF_GENERIC_CALL_CLEAR

/*This API indicates that timer expiry has been received*/
#define ICF_TIMER_EXPIRY_IND            (ICF_INT_API_CM_START + 9)

/*This API indicates Call Resume resp for internal call resume request*/
#define ICF_SC_INTERNAL_CALL_RESUME_RESP (ICF_INT_API_CM_START + 10)

/*This API indicates Call hold resp for internal call hold request*/
#define ICF_SC_INTERNAL_CALL_HOLD_RESP   (ICF_INT_API_CM_START + 11)

/*This API indicates that media is committed for the call in glb pdb*/
#define ICF_SC_INTERNAL_MEDIA_COMMIT     (ICF_INT_API_CM_START + 12)

/*This API indicates that some error has been occured in called service*/
#define ICF_SC_SERVICE_ERROR_IND         (ICF_INT_API_CM_START + 13)

/*This API indicates that some error has been occured in called service*/
#define ICF_INTERNAL_INITIATE_CALL       (ICF_INT_API_CM_START + 14)

/*This API indicates that some error has been occured in called service*/
#define ICF_CALL_WAIT_CONTINUE           (ICF_INT_API_CM_START + 15)

/*This API is invoked by CM towards SCM for Call Forwarding in status from CC */
#define ICF_NEW_SERVICE_TRIGGER          (ICF_INT_API_CM_START + 16)

/*This API indicates that some error has been occured in called service*/
#define ICF_SC_SRV_ERR_IND_W_PAYLOAD     (ICF_INT_API_CM_START + 17)

/*This API is sent to CTA when TWC service is terminated normal and CTA is
 * waiting for its response*/
#define ICF_SC_INTERNAL_TWC_RESP         (ICF_INT_API_CM_START + 18)

#define ICF_SC_INTERNAL_RNW_RESP         (ICF_INT_API_CM_START + 19)

#define ICF_SC_INTERNAL_CHR_RESP         (ICF_INT_API_CM_START + 20)

#define ICF_SSA_SESSION_TIMER_GET        (ICF_INT_API_CM_START + 21) 

#define ICF_FORCEFUL_REINIT_IND          (ICF_INT_API_CM_START + 22)

/*This will be send by XCONF to CTU to delete a party*/
#define ICF_INTERNAL_REFER_REQ                        (ICF_INT_API_CM_START + 23)

/*This will be send by CTA to XCONF as the add party response*/
#define ICF_INTERNAL_REFER_RESP                       (ICF_INT_API_CM_START + 24)

/*This message will be send by XCONF to SIC to send SUBSCRIBE*/
#define ICF_INTERNAL_SUBSCRIBE_REQ                    (ICF_INT_API_CM_START + 25)

/*This message will be send by SIC to XCONF after getting the 200 OK of SUBSCRIBE*/
#define ICF_INTERNAL_SUBSCRIBE_RESP                   (ICF_INT_API_CM_START + 26)

/*This will be send by SIC to XCONF when NOTIFY comes*/
#define ICF_INTERNAL_NOTIFY_IND                       (ICF_INT_API_CM_START + 27)

/*This will be send by CM to SCM and then SCM to XCONF to initiate the Conference*/
#define ICF_USER_INITIATE_EXTERNAL_CONF_REQ           (ICF_INT_API_CM_START + 28)

/*This will be send by CM to SCM and then SCM to XCONF to add a party from Conference*/
#define ICF_USER_ADD_PARTY_REQ                        (ICF_INT_API_CM_START + 29)

/*This will be send by CM to SCM and then XOCNF to delete a party from Conference*/
#define ICF_USER_DELETE_PARTY_REQ                     (ICF_INT_API_CM_START + 30)

/*This will be send by CM to SCM and then XOCNF to Release a Conference*/
#define ICF_USER_RELEASE_CONFERENCE_REQ               (ICF_INT_API_CM_START + 31)

/*This will be send to XCONF by SSA through SCM.*/
#define ICF_EXTERNAL_CONFERENCE_CONNECTED             (ICF_INT_API_CM_START + 32)

/*This will be send to XCONF by SSA through CM. It indicates that IPTK got a 
 *INVITE with isfocus parameter.*/
#define ICF_INCOMING_EXTERNAL_CONFERENCE_REQ          (ICF_INT_API_CM_START + 33)

/*This will be send by XCONF to CTA for handling abort REFER REQ case.*/
#define ICF_INTERNAL_ABORT_REFER_REQ                  (ICF_INT_API_CM_START + 34)

#define ICF_XCONF_DEL_PARTY_RESP                      (ICF_INT_API_CM_START + 35)

#define ICF_XCONF_DEL_PARTY_REQ                       (ICF_INT_API_CM_START + 36)

/*This will be send by XCONF to CTA to add a party*/
#define ICF_INTERNAL_REFER_W_REP_REQ                  (ICF_INT_API_CM_START + 37)

#define ICF_INVOKE_TWC_FOR_JOIN                       (ICF_INT_API_CM_START + 38)

#define ICF_SSA_SESSION_TIMER_TOGGLE                     (ICF_INT_API_CM_START + 39) 
    /* Start of reg API is ICF_INT_API_START+ICF_INT_API_NW_OTG_START(10)
     * ICF_INT_API_NW_INC_START(30) + ICF_INT_API_MEDIA_START(30) +
     * ICF_INT_API_CM_START (20) + 20.
     * Direct Macro to depict this addition has not been done so as to
     * reduce the code area as after pre-processing it shall show x+y+z.*/

#define ICF_INT_API_REG_START           (ICF_INT_API_START + 100 + 10 + 40)

/* API from registration to SSA to send registration request. */
#define ICF_NW_OTG_REG_REQ              (ICF_INT_API_REG_START)

/* API from SSA to Registration module on getting response. */
#define ICF_NW_INC_REG_RESP            (ICF_INT_API_REG_START + 1)

/* API from Conf to REG to start registration for single user. */
#define ICF_CONF_START_SINGLE_REG_REQ  (ICF_INT_API_REG_START + 2)

/* API from Conf to REG to start registaration for all users of line. */
#define ICF_CONF_START_ALL_USR_REG_REQ (ICF_INT_API_REG_START + 3)

/* API from REGM to SSA for clearing Registration Call Object */
#define ICF_CLEAR_REG_OBJECT           (ICF_INT_API_REG_START + 4)

#ifdef IMS_CLIENT
/* API from SSA to Registration module on getting Notify with Terminated state. */
#define ICF_SSA_SUBS_NOTIFY_IND		  (ICF_INT_API_REG_START + 5)
#endif

/* API from SSA to REG indicating that a number of consecutive transactions have
 * timed-out.SSA shall invoke this API towards REGM, when n number of 
 * transactions timeout in ICF. Here "n" is a configured number and defines the 
 * system threshold */
#define ICF_NW_NOT_RESPONDING          (ICF_INT_API_REG_START + 6)

#ifdef ICF_IPSEC_ENABLED
/* API from SSA to REG indicating that there are no more pending transactions on
 * the old SA and it should be deleted now */
#define ICF_DELETE_OLD_SA              (ICF_INT_API_REG_START + 7)
/* API to indicate that SA has been created and we need to open server 
 * for prot ports */
#define ICF_SSA_SA_OPEN_SERVER         (ICF_INT_API_REG_START + 8)  
/* API to indicate that the SA's have been deleted and we need to close
 * the servers on the protected ports */
#define ICF_SSA_SA_CLOSE_SERVER        (ICF_INT_API_REG_START + 9)
#endif


    /* Start of cfg API is ICF_INT_API_START+ICF_INT_API_NW_OTG_START(10)
     * ICF_INT_API_NW_INC_START(30) + ICF_INT_API_MEDIA_START(30) +
     * ICF_INT_API_CM_START (20) + 20 + 10 (REG API Range).
     * Direct Macro to depict this addition has not been done so as to
     * reduce the code area as after pre-processing it shall show x+y+z.*/

#define ICF_INT_API_CFG_START          (ICF_INT_API_START + 110 +20 +40)

/* API from CFG to all the other sub-modules to indicate INIT COMPLETE detection*/
#define ICF_CFG_INIT_COMPLETE          (ICF_INT_API_CFG_START + 1)
/*CFG to SSA API to trigger network server setup*/
#define ICF_CFG_OPEN_NW_SERVERS	(ICF_INT_API_CFG_START + 2)		
/*CFG to SSA API to trigger network server closure-both TCP/UDP servers are closed*/
#define ICF_CFG_CLOSE_NW_SERVERS	(ICF_INT_API_CFG_START + 3)
/*ICF_INIT_START*/
#define ICF_CFG_INTERNAL_INIT	(ICF_INT_API_CFG_START + 4)
/*ICF_INIT_END*/

/*ICF_CRM_START*/
#if 0
/* Non Blocking network activate response from NM for NC */
#define ICF_NM_ACTIVATE_RESP            (ICF_INT_API_CFG_START + 5)

/* Non Blocking network deactivate response from NM for NC */
#define ICF_NM_DEACTIVATE_RESP          (ICF_INT_API_CFG_START + 6)
#endif

/* API from NM for NC to indicate any event that has caused network state change */
/*#define ICF_NM_EVENT_IND                (ICF_INT_API_CFG_START + 7)*/
/*ICF_CRM_END*/
/*ICF_INIT_START*/
#ifdef ICF_NW_ACTIVATE
#define ICF_CFG_AUTO_ACTIVATE		   (ICF_INT_API_CFG_START + 8)
#endif
/*ICF_INIT_END*/

/* ICF_4_0_CRM_START */
#define ICF_CFG_AUTO_REGISTER		   (ICF_INT_API_CFG_START + 9)

/* API from CFG to SSA to delete all remainig context and timers at the time of
 * delete config*/
#define ICF_CFG_SSA_INITIATE_CLEAN_UP   (ICF_INT_API_CFG_START + 10)

#define ICF_CFG_GET_NW_INFO		   (ICF_INT_API_CFG_START + 11)

/* This API will be send by MMI when mm_init is completed */
#define ICF_CFG_MMI_MM_INIT_COMPLETE	(ICF_INT_API_CFG_START + 12)

/* ICF_4_0_CRM_END */

    /* Start of SIC API is ICF_INT_API_START+ICF_INT_API_NW_OTG_START(10)
     * ICF_INT_API_NW_INC_START(30) + ICF_INT_API_MEDIA_START(30) +
     * ICF_INT_API_CM_START (20) + 20 + 10 (REG API Range)+10 (CFG_API).
     * Direct Macro to depict this addition has not been done so as to
     * reduce the code area as after pre-processing it shall show x+y+z.*/

#define ICF_INT_API_SIC_START            (ICF_INT_API_START + 110 +31 +40)

/* API From Other Module to SIC to send OPTION Request */
#define ICF_SIC_OPTION_REQ            (ICF_INT_API_SIC_START + 1)
/* API From SIC to Other Modult On getting OPTION Response from Network  */
#define ICF_SIC_OPTION_STATUS          (ICF_INT_API_SIC_START + 2)
    
/* API From Other Module to SIC to send SUBSCRIBE Request */
#define ICF_SIC_SUBSCRIBE_REQ         (ICF_INT_API_SIC_START + 3)
/* API From Other Module to SIC to send UNSUBSCRIBE Request */
#define ICF_SIC_UNSUBSCRIBE_REQ       (ICF_INT_API_SIC_START + 4)
/* API From SIC to Other Modult On getting SUBSCRIBE Response from Network*/
#define ICF_SIC_SUBSCRIPTION_STATUS   (ICF_INT_API_SIC_START + 5)

/* API From Other Module to SIC to send MESSAGE Request */
#define ICF_SIC_MESSAGE_REQ           (ICF_INT_API_SIC_START + 6)
/* API From SIC to Other Modult On getting MESSAGE Response from Network */
#define ICF_SIC_MESSAGE_STATUS          (ICF_INT_API_SIC_START + 7)
/* API From SIC to Other Module On getting MESSAGE Indication from Network*/
#define ICF_SIC_NEW_MESSAGE_IND         (ICF_INT_API_SIC_START + 8)
    
/* API From Other Module to SIC to send REFER Request */
#define ICF_SIC_REFER_REQ             (ICF_INT_API_SIC_START + 9)
/* API From SIC to Other Modult On getting REFER Response from Network  */
#define ICF_SIC_REFER_STATUS          (ICF_INT_API_SIC_START + 10)

/* API From Other Module to SIC to send PUBLISH Request */
#define ICF_SIC_PUBLISH_REQ           (ICF_INT_API_SIC_START + 11)
/* API From SIC to Other Modult On getting PUBLISH Response from Network*/
#define ICF_SIC_PUBLISH_STATUS        (ICF_INT_API_SIC_START + 12)

/* API from SIC to SSA to send Subscribe request */
#define ICF_NW_OTG_SUBSCRIBE_REQ         (ICF_INT_API_SIC_START + 13)

/* API from SSA to SIC to send Subscribe response */
#define ICF_NW_INC_SUBSCRIBE_RESP        (ICF_INT_API_SIC_START + 14)   
    
/* API from SSA to SIC to send Notify ind */
#define ICF_NW_INC_NOTIFY_IND            (ICF_INT_API_SIC_START + 15)

#define ICF_SIC_NOTIFY_IND         (ICF_INT_API_SIC_START + 16)
/* API from SIC to SSA to send MESSAGE request */
#define ICF_NW_OTG_MESSAGE_REQ         (ICF_INT_API_SIC_START + 17)

/* API from SSA to SIC to send MESSAGE response */
#define ICF_NW_INC_MESSAGE_RESP        (ICF_INT_API_SIC_START + 18) 

/* API from SSA to SIC to send MESSAGE ind */
#define ICF_NW_INC_MESSAGE_IND            (ICF_INT_API_SIC_START + 19)

/* API from SIC to SSA to send MESSAGE response */
#define ICF_NW_OTG_MESSAGE_RESP           (ICF_INT_API_SIC_START + 20)

/* API from SIC to SSA to send OPTIONS request */
#define ICF_NW_OTG_OPTIONS_REQ         (ICF_INT_API_SIC_START + 21)

/* API from SSA to SIC to send OPTIONS response */
#define ICF_NW_INC_OPTIONS_RESP        (ICF_INT_API_SIC_START + 22) 

/* API from SIC to SSA to send PUBLISH request */
#define ICF_NW_OTG_PUBLISH_REQ         (ICF_INT_API_SIC_START + 23)

/* API from SSA to SIC to send PUBLISH response */
#define ICF_NW_INC_PUBLISH_RESP        (ICF_INT_API_SIC_START + 24) 
    
/* API from the application modules to SSA to de-link */
#define ICF_DELINK_APP_CTX               (ICF_INT_API_SIC_START + 25)

/* API from the Any Application modules to SIC for de-link */
#define ICF_CLEAR_SIC_CTX               (ICF_INT_API_SIC_START + 26)

/* API from application to SIC to send stray notify response */
#define ICF_SIC_NOTIFY_CFM				(ICF_INT_API_SIC_START + 27)
/* ICF_6_1_OPTIONS_SUPPORT_START */
/* API from SSA to SIC to indicate incoming options request for nw */
#define ICF_NW_INC_OPTIONS_IND	      (ICF_INT_API_SIC_START + 28)

/* API from MRM to SIC to indicate application has sent
 * conformation for icf_options_ind_st */
#define ICF_SIC_OPTIONS_CFM	      (ICF_INT_API_SIC_START + 29) 

/* API from SIC to SSA to send response for incoming options */
#define ICF_NW_INC_OPTIONS_CFM    (ICF_INT_API_SIC_START + 30)

/* API for SSA to SIC to clear the sic context */
#define ICF_SIC_CLEAR_OPTIONS_CTX  (ICF_INT_API_SIC_START + 31)

/* Rel 8.1 
 * API from MRM to SIC to indicate that 
 * Application has sent  ICF_EARLY_SESSION_CFM
 */
#define ICF_SIC_EARLY_SESSION_CFM           (ICF_INT_API_SIC_START + 32)

/* ICF_6_1_OPTIONS_SUPPORT_END */

/* API from SSA to SIC to send MESSAGE CFM */
#define ICF_SIC_MESSAGE_CFM            (ICF_INT_API_SIC_START + 33)

#define ICF_INT_API_SIC_END           (ICF_INT_API_SIC_START + 41 )



#define ICF_INT_API_NM_START          (ICF_INT_API_START + 110 +70+40)

#ifdef ICF_NW_ACTIVATE
/*To get the network inforamtion from the NM*/
#define ICF_NM_NW_INFO	ICF_INT_API_NM_START+1
#endif

/*To get the platform information from the NM*/
#define	ICF_NM_MOB_INFO	ICF_INT_API_NM_START+2

/*To get the network access information*/
#define ICF_NM_ACS_INFO	ICF_INT_API_NM_START+3

#ifdef ICF_NW_ACTIVATE
/*To Activate a primary PDP context */
#define ICF_NM_ACT		ICF_INT_API_NM_START+4

/*To Activate a secondary PDP context */
#define ICF_NM_SEC_ACT	ICF_INT_API_NM_START+5

/*To Deacitvate a PDP context */
#define ICF_NM_DEACT		ICF_INT_API_NM_START+6
#endif

/*To get the P-NW-Access information*/
#define ICF_NM_P_NW_INFO	ICF_INT_API_NM_START+8

#define ICF_NM_SCALABLE_PARAMS_REQ	ICF_INT_API_NM_START+9

/*To de-initialize the NM*/
#define ICF_NM_DEINIT	ICF_INT_API_NM_START+10

/*To perform authentication from SIM*/
#define ICF_NM_AUTH		ICF_INT_API_NM_START+11

#ifdef ICF_NW_ACTIVATE
#define ICF_NM_CFG_EVENT_IND		ICF_INT_API_NM_START + 12
#endif

#define ICF_NM_PLATFORM_EVENT    ICF_INT_API_NM_START + 13


#ifdef ICF_IPSEC_ENABLED
/*------------------------------------------------------------------------------
 * APIs needed for IPSec Support in ICF Release 4.0
 *----------------------------------------------------------------------------*/

/* API invoked by REGM towards NM for fetching local security algorithms */
#define ICF_RGM_NM_SA_FETCH_ALGO     (ICF_INT_API_NM_START + 14)

/* API invoked by REGM towards NM for requesting the creation of a new SA */
#define ICF_RGM_NM_SA_CREATE_REQ     (ICF_INT_API_NM_START + 15)

/* Response API for ICF_NM_SA_CREATE_REQ invoked by NM towards REGM */
#define ICF_NM_RGM_SA_CREATE_RESP    (ICF_INT_API_NM_START + 16)

#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/* API invoked by REGM towards NM for requesting the modification of SA */
#define ICF_RGM_NM_SA_MODIFY_REQ     (ICF_INT_API_NM_START + 17)

/* Response API for ICF_NM_SA_MODIFY_REQ invoked by NM towards REGM */
#define ICF_NM_RGM_SA_MODIFY_RESP    (ICF_INT_API_NM_START + 18)
#endif

/* API invoked by REGM towards NM for requesting the deletion of SA */
#define ICF_RGM_NM_SA_DELETE_REQ     (ICF_INT_API_NM_START + 19)

/* Response API for ICF_NM_SA_DELETE_REQ invoked by NM towards REGM */
#define ICF_NM_RGM_SA_DELETE_RESP    (ICF_INT_API_NM_START + 20)

/* API invoked by NM towards REGM indicating the lifetime expiry of SA */
#define ICF_NM_RGM_SA_EXPIRY_IND     (ICF_INT_API_NM_START + 21)

/* ------------------ End: APIs for IP Security Support ---------------- */
#endif /* ICF_IPSEC_ENABLED */

#define ICF_INT_API_NM_END       ICF_INT_API_NM_START+50


/* ADD ANY NEW APIS ABOVE SIGCOMP SO THAT THERE WILL ONE #IFDEF for
 * SIGCOMP INTERNAL APIS
 */

#ifdef ICF_SIGCOMP_SUPPORT

#define ICF_INT_API_SIGSA_START          (ICF_INT_API_START + 110 +120+40)
/* API from RGM to SIGSA to Indicate the Secure coneection with peer is
 * Established 
 */
#define ICF_SECURE_CONN_UP_IND           (ICF_INT_API_SIGSA_START + 1)
/* API from RGM to SIGSA for creating Compartment */
#define ICF_CREATE_COMP_REQ              (ICF_INT_API_SIGSA_START + 2)
/* API from SIGSA to RGM as a response for creating Compartment */
#define ICF_CREATE_COMP_RESP             (ICF_INT_API_SIGSA_START + 3)
/* API from RGM to SIGSA for deleting the Compartment */
#define ICF_DELETE_COMP_IND              (ICF_INT_API_SIGSA_START + 4)

#define ICF_INT_API_SIGSA_END            (ICF_INT_API_SIGSA_START + 20)

#endif 

/* New Apis to facilitate MMI-NM Interaction required to add QOS Capability
 */

#define ICF_INT_API_QOS_START            (ICF_INT_API_START + 110 +190 +40)
/* API from NM to MMI providing Response to Secondary PDP Context Activation Request */
#define ICF_NM_MMI_SEC_PDP_ACT_RESP      (ICF_INT_API_QOS_START + 1)
/* API from NM to MMI providing Response to Secondary PDP Context DeActivation Request */
#define ICF_NM_MMI_SEC_PDP_DEACT_RESP    (ICF_INT_API_QOS_START + 2)
/* API from NM to MMI signalling Secondary PDP Context DeActivation Indication */
#define ICF_NM_MMI_SEC_PDP_DEACT_IND     (ICF_INT_API_QOS_START + 3)
/* API from NM to MMI signalling Secondary PDP Context Modification Indication */
#define ICF_NM_MMI_SEC_PDP_MODIFY_IND    (ICF_INT_API_QOS_START + 4)
/* Primary or Secondary PDP Context Deactivated from the network */
#define ICF_NM_DEACT_IND  	  (ICF_INT_API_QOS_START + 5)
/* Primary or Secondary PDP Context Modified from the network */
#define ICF_NM_MODIFY_IND  	(ICF_INT_API_QOS_START + 6)
/*To Activate multiple secondary PDP contexts */
#define ICF_NM_MULTIPLE_SEC_ACT		ICF_INT_API_QOS_START + 7
/*To DeActivate multiple secondary PDP contexts */
#define ICF_NM_MULTIPLE_SEC_DEACT		ICF_INT_API_QOS_START + 8

#define ICF_NM_QOS_RESERVE_FOR_NON_IMS  	(ICF_INT_API_QOS_START + 9)

#define ICF_NM_QOS_RELEASE_FOR_NON_IMS  	(ICF_INT_API_QOS_START + 10)

#define ICF_NM_MMI_QOS_RESERVE_NON_IMS_RESP  	(ICF_INT_API_QOS_START + 11)

#define ICF_NM_MMI_QOS_RELEASE_NON_IMS_RESP  	(ICF_INT_API_QOS_START + 12)


#define ICF_INT_API_QOS_END              (ICF_INT_API_QOS_START + 20)

/*Added so that CFG could trigger MPH on app removal*/
#define ICF_INT_API_APP_REMOVAL		(ICF_INT_API_QOS_END  + 1)
/*Added so that CFG could trigger MPH on network deactivation*/
#define ICF_INT_API_NW_DEACTIVATED	ICF_INT_API_APP_REMOVAL + 1
/*Added so that CFG could trigger MPH after reading scalable params*/
#define ICF_MPH_SCALABLE_PARAMS_REQ	ICF_INT_API_NW_DEACTIVATED + 1

#define ICF_DETACH_CALL_CTX_WITH_SSA_CTX ICF_MPH_SCALABLE_PARAMS_REQ + 1
/*  This feature is now out of scope in 8.0 */
#if 0
/*  This API is invoked from SSA towards CFG to clear the calls 
    and register on through proxy    */
#define ICF_REDISCOVER_SIP_SERVER ICF_DETACH_CALL_CTX_WITH_SSA_CTX + 1
#endif 
#define ICF_INT_API_END                  ICF_DETACH_CALL_CTX_WITH_SSA_CTX + 1

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_INTERNAL_API_ID_H__ */


