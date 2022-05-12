/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_defs.h
 *
 * Purpose          : This file contains the defines used by CC.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 24-Dec-2004  Rohit Aggarwal  Non Service LLD     Initial
 * 28-Dec-2004  Rohit Aggarwal                      Added #defines
 * 26-Jan-2005  Rohit Aggarwal                      Moved call_state defines
 * 27_May-2005  Sumit Gupta     SPR 7403            Added new event
 * 09-Jun-2005  Jalaj Negi      IMS CLIENT          Added Events for IMS 
 *                                                  CLIENT
  * 07-Mar-2006	Anuradha Gupta	None				Added Events for incoming
 *													Re-invite handling 
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0 
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0             Call Modify Feature changes
 * 26-Mar-2007  Deepti Goyal    ICF 6.4             Changes for Media connect 
 *                                                  request API.
 * 04-Apr-2007  Deepti Goyal    ICF 6.3             Changes for Media connect 
 *                                                  request API.
 * 10-Jul-2007  Abhishek Dutta  ICF 7.0             Changes for Cancel call  
 *                                                  modify request API.
 * 21-Aug-2007  Anuradha Gupta  ICF 7.0             merge from PQT fixes
 * Copyright (c) 2007, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CC_DEFS_H__
#define __ICF_CC_DEFS_H__


/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is 
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */

    
/*------------------------------------------------------------------------------
 *
 * hash define values for Call Control events
 *
 *----------------------------------------------------------------------------*/
#if 0

#define ICF_CC_PA_SETUP_REQ                      0
#define ICF_CC_SSA_SETUP_RESP                    1
#define ICF_CC_SSA_MEDIA_PARAM                   2
#define ICF_CC_SSA_ALERT                         3
#define ICF_CC_SSA_ALERT_WITH_MEDIA              4
#define ICF_CC_SSA_CALL_CONNECT                  5
#define ICF_CC_CMS_RESP                          6
#define ICF_CC_START_CALL_PROC_SIG               7
#define ICF_CC_START_CALL_PROC_MEDIA             8
#define ICF_CC_MMS_RESP                          9
#define ICF_CC_DMS_RESP                          10
#define ICF_CC_SSA_CALL_TERMINATE_REQ            11
#define ICF_CC_SSA_REMOTE_REJECT                 12
#define ICF_CC_SSA_REDIRECT                      13
#define ICF_CC_PA_CALL_TERMINATE_REQ             14
#define ICF_CC_SSA_CALL_CANCEL_RESP              15
#define ICF_CC_PA_CALL_TERMINATE_RESP            16
#define ICF_CC_SSA_CALL_TERMINATE_RESP           17
#define ICF_CC_SSA_OFFER_CHANGE                  18
#define ICF_CC_CALL_MUTE_REQ                     19
#define ICF_CC_CALL_UNMUTE_REQ                   20
#define ICF_CC_MM_ERR_IND                        21
#define ICF_CC_TIMER_EXPIRY                      22
#define ICF_CC_SSA_SETUP_REQ                     23
#define ICF_CC_PA_ALERT                          24
#define ICF_CC_PA_CALL_CONNECT                   25
#define ICF_CC_MEDIA_PARAM_PRACK                 26
#define ICF_CC_ALERT_PRACK                       27
#define ICF_CC_SSA_CONNECT_ACK                   28
#define ICF_CC_SSA_CALL_CANCEL_REQ               29
#define ICF_CC_GENERIC_ERROR                     30
#define ICF_CC_FORCEFUL_CLEAR                    31
#define ICF_CC_INVALID_EVENT                     32
#define ICF_CC_SSA_REINVITE_ACK                  33
#define	ICF_CC_PA_INFO_REQ						34
#define ICF_CC_SSA_INFO_RESP						35
#ifndef IMS_CLIENT
#ifndef ICF_SESSION_TIMER
#define ICF_CC_MAX_EVENTS                        35
#else
#define ICF_CC_SESSION_REFRESH_REQ               36
#define ICF_CC_SESSION_REFRESH_RESP              37
#define ICF_CC_MAX_EVENTS                        37
#endif
#else
#define ICF_CC_MEDIA_PARAM_PRACK_RESP            36
#define ICF_CC_MEDIA_UPDATE_RESP                 37
#ifndef ICF_SESSION_TIMER
#define ICF_CC_MAX_EVENTS                        37
#else
#define ICF_CC_SESSION_REFRESH_REQ               38
#define ICF_CC_SESSION_REFRESH_RESP              39
#define ICF_CC_MAX_EVENTS                        39
#endif
#endif

#endif

/*ICF_HANDLE_REINVITE_START*/
typedef enum
{
	ICF_CC_PA_SETUP_REQ = 0,                     
	ICF_CC_SSA_SETUP_RESP,                    
	ICF_CC_SSA_MEDIA_PARAM,                   
	ICF_CC_SSA_ALERT,                         
	ICF_CC_SSA_ALERT_WITH_MEDIA,              
	ICF_CC_SSA_CALL_CONNECT,                  
	ICF_CC_CMS_RESP,                          
	ICF_CC_START_CALL_PROC_SIG,               
	ICF_CC_START_CALL_PROC_MEDIA,             
	ICF_CC_MMS_RESP,                          
	ICF_CC_DMS_RESP,                          
	ICF_CC_SSA_CALL_TERMINATE_REQ,            
	ICF_CC_SSA_REMOTE_REJECT,                 
	ICF_CC_SSA_REDIRECT,                      
	ICF_CC_PA_CALL_TERMINATE_REQ,             
	ICF_CC_SSA_CALL_CANCEL_RESP,              
	ICF_CC_PA_CALL_TERMINATE_RESP,            
	ICF_CC_SSA_CALL_TERMINATE_RESP ,          
	ICF_CC_SSA_OFFER_CHANGE,                  
	ICF_CC_CALL_MUTE_REQ,                     
	ICF_CC_CALL_UNMUTE_REQ ,                  
	ICF_CC_MM_ERR_IND,                        
	ICF_CC_TIMER_EXPIRY ,                     
	ICF_CC_SSA_SETUP_REQ,                     
	ICF_CC_PA_ALERT,                          
	ICF_CC_PA_CALL_CONNECT,                   
	ICF_CC_MEDIA_PARAM_PRACK ,                
	ICF_CC_ALERT_PRACK ,                      
	ICF_CC_SSA_CONNECT_ACK,                   
	ICF_CC_SSA_CALL_CANCEL_REQ  ,             
	ICF_CC_GENERIC_ERROR,                     
	ICF_CC_FORCEFUL_CLEAR ,                   
	ICF_CC_SSA_MEDIA_CHANGE_ACK ,
	ICF_CC_SSA_REINVITE_ACK ,
	ICF_CC_PA_CALL_MODIFY_REQ,
	ICF_CC_PA_CALL_MODIFY_RSP,
#ifdef ICF_SESSION_TIMER
	ICF_CC_SESSION_REFRESH_REQ ,              
	ICF_CC_SESSION_REFRESH_RESP,              
#endif
	ICF_CC_MEDIA_PARAM_PRACK_RESP,            
	ICF_CC_MEDIA_UPDATE_RESP,                 
	ICF_CC_SSA_INFO_RESP,
	ICF_CC_PA_INFO_REQ,
    ICF_CC_SSA_INFO_REQ,
	ICF_CC_PA_FAX_REQ,
	ICF_CC_SSA_FAX_REQ,
	ICF_CC_SSA_FAX_RESP,
	ICF_CC_PA_INFO_CFM,
    ICF_CC_PA_EARLY_CALL_CFM,
    ICF_CC_PA_MEDIA_CONNECT_REQ,
	ICF_CC_PA_STOP_FAX_REQ,
	ICF_CC_SSA_STOP_FAX_RESP,                   
	ICF_CC_SSA_STOP_FAX_REQ,                   
/*call_mod_cancel*/
    ICF_CC_PA_CALL_MODIFY_CANCEL_REQ,
    ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ,
    ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP,
        ICF_CC_SSA_CALL_PROGRESS,
	ICF_CC_INVALID_EVENT ,
	ICF_CC_MAX_EVENTS
}icf_cc_event_et;

/*ICF_HANDLE_REINVITE_END*/

/*------------------------------------------------------------------------------
 *
 * contains hash define values for ongoing_event field in icf_call_ctx_st
 *
 *----------------------------------------------------------------------------*/

    /*  there is no ongoing event */
#define  ICF_CC_ONGOING_EVENT_NONE                  ICF_CC_MAX_EVENTS

    /*  ongoing event is fax request from application */
#define  ICF_CC_ONGOING_EVENT_LOCAL_FAX_REQ            ICF_CC_MAX_EVENTS + 1

    /*  ongoing event is remote fax req  */
#define  ICF_CC_ONGOING_EVENT_REMOTE_FAX_REQ         ICF_CC_MAX_EVENTS + 2
	
	/* ongoing event is media revert for local offer */
#define ICF_CC_ONGOING_EVENT_LOCAL_MEDIA_REVERT		ICF_CC_MAX_EVENTS + 3

	/* ongoing event is media revert for remote offer */
#define ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT	ICF_CC_MAX_EVENTS + 4

/* ongoing event is Connect from application or from remote */
#define ICF_CC_ONGOING_EVENT_CONNECT            	ICF_CC_MAX_EVENTS + 5

/* ongoing event is UPDATE for QOS */
#define ICF_CC_ONGOING_EVENT_UPDATE_FOR_QOS            	ICF_CC_MAX_EVENTS + 6


/*------------------------------------------------------------------------------
 *
 * contains hash define values for QOS processing
 *
 *----------------------------------------------------------------------------*/

#ifdef ICF_QOS_SUPPORT
/* This is set as status when Qos processing is done */
#define ICF_CC_NW_RES_RESV_DONE                  1
/* This is set as status when Qos processing is incomplete */
#define ICF_CC_NW_RES_RESV_NOT_DONE              2   
/* This is set as status when Qos processing is not required */
#define ICF_CC_NW_RES_RESV_NOT_REQ               3
#endif

#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * contains hash define values for SDP Offer/Answer
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_LOCAL_SDP_OFFER               1
#define ICF_CC_LOCAL_SDP_ANSWER              2
#define ICF_CC_REMOTE_SDP_OFFER              3
#define ICF_CC_REMOTE_SDP_ANSWER             4
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_CC_DEFS_H__ */
