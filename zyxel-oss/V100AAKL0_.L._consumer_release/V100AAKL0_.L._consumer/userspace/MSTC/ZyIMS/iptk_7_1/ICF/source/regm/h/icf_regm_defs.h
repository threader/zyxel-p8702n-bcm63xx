/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_defs.h
 *
 * Purpose          : This file contains the defines used by RGM.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 20-Dec-2004  Jalaj Negi      Non Service LLD     Initial
 * 05-Dec-2005	Jyoti Narula	ICF 4.0 CDD			Changes under ICF_4_0_CRM_START and END
 * 07-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD     Coding for IPSec support
 *
 * 27-Nov-2006  Deepti Goyal    ICF Rel 6.2         NAT "rport"Feature Support.
 *
 * 11-Dec-2006	Deepti Goyal	SPR 14029	    MAX events for regm state machine updated
 *						   for NAT binding change event.		
 * 17-Dec-2009  Pavneet Bajaj   SPR 20244          Guard Timer Implementation			
 * Copyright (c) 2006, Aricent 
 *----------------------------------------------------------------------------*/

#ifndef __ICF_REGM_DEFS_H__
#define __ICF_REGM_DEFS_H__

/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is 
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */


/* Maximum number of States */
#ifdef ICF_IPSEC_ENABLED
#define ICF_RGM_MAX_STATES        8
#else
#define ICF_RGM_MAX_STATES        6
#endif

ICF_4_0_CRM_START
/*	This value of maximum events is being changed from 7 to 13 ICF 4.0 
 *  as 6 new events have been added to the old list */
ICF_4_0_CRM_END

/* Maximum number of Events */

#ifdef ICF_NAT_RPORT_SUPPORT
#define ICF_RGM_MAX_EVENTS        19
#else
#define ICF_RGM_MAX_EVENTS        18
#endif

/* Event type Starts */

/* This is the event when success response is received for registration 
 * request.
 */
#define ICF_RGM_SUCCESS_RESPONSE         0

/* This is the event when failure response 
 * (302,400,500,502,503,491,stack timer expiry) is received in response to
 * registration request
 */
#define ICF_RGM_FAILURE_REATTEMPT        1

/* This is the event when failure response
 * (403,404,481) in which registration should not be retried is received in
 * response to registration request.
 */
#define ICF_RGM_FAILURE_NO_REATTEMPT     2

/* This is the event when authentication required(401/407) is received in
 * response to registration request
 */
#define ICF_RGM_AUTHENTICATION_REQ       3

/* This is the event when duration too brief(423) is received in response to
 * registration request 
 */  
#define ICF_RGM_DURATION_SHORT           4

/* This is the internal event when registration duration changes in a 
 * registration block. This might be due to change in registration duration
 * on a line
 */
#define ICF_RGM_DURATION_CHANGE          5

/* This is the internal event when a registration block is to be deleted from
 * registration context
 */    
#define ICF_RGM_DELETE_USER              6

ICF_4_0_CRM_START
/* New Events for release 4.0 of ICF */

/* This event is the receipt of REGISTER_REQUEST API */
#define ICF_RGM_REG_REQ_RECVD				7

/* This event is the mapping for following NOTIFY configuration:
 * state of registration element = active and 
 * event of contact subelement = shortened.
 * This is the case of Network Initiated Re-Authentication */ 
#define ICF_RGM_NOTIFY_SHORTENED			8

/* this event is the mapping of NOTIFY message body for Network
 * initiated DEREGISTRATION */
#define ICF_RGM_NOTIFY_TERMINATED		9

 /* Event for Network Deactivation */
#define ICF_RGM_NW_DEACTIVATED			10

 /* event ot indicate that Network has come up */
#define ICF_RGM_NW_ACTIVATED				11

 /* Event to indicate the timer expiry */
#define ICF_RGM_TIMER_EXPIRY				12

 /* Event to indicate the timeout failure indication from SSA */
#define	ICF_RGM_FAILURE_TIMEOUT			13

/* This is the internal event when Authentication information on a line changes
 * which results in a change in registration context
 */
/*#define ICF_RGM_AUTH_INFO_CHANGE         7*/


/* This is the internal event when REGM receives SA create response from NM */
#define ICF_RGM_SA_CREATE_RESP               14

/* This is the internal event when REGM receives SA modify response from NM */
#define ICF_RGM_SA_MODIFY_RESP               15

/* This is the internal event when REGM receives SA delete response from NM */
#define ICF_RGM_SA_DELETE_RESP               16

/* This is the internal event when REGM receives SA expiry indication from NM */
#define ICF_RGM_SA_EXPIRY_IND                17

#ifdef ICF_NAT_RPORT_SUPPORT
#define ICF_RGM_NAT_BINDING_CHNG_IND         18
#endif

/* Event type Ends */

/* Constants used for finding the status of the received NONCE as compared to
 * the old NONCE stored in rgm_context */
#define ICF_RGM_INVALID_CHALLENGE		0
#define ICF_RGM_FIRST_NONCE				1
#define ICF_RGM_FRESH_NONCE				2
#define ICF_RGM_SAME_NONCE				3



ICF_4_0_CRM_END

    

/* Default Registration Duration */
ICF_CRM_START

/*#ifdef IMS_CLIENT*/
/* IMS spec says that default duration should be 600,000 seconds */
#define ICF_RGM_DEFAULT_DURATION_IMS_CLIENT        600000000
/*#else*/
/* As per rfc 3261 Section 10.2 Default value as 3600 seconds */
#define ICF_RGM_DEFAULT_DURATION_NON_IMS        3600000
/*#endif*/

ICF_CRM_END


/* Bit Masks */
/* This is used to Reset 1st bit of indicator bitmask */   
#define ICF_RGM_AUTH_RESET             0xFE
/* This is used to reset 2nd bit of indicator bitmask */    
#define ICF_RGM_RESET_PA_IND           0xFD
    
/* This is used as a value to indicate that a block is not to be processed */
#define ICF_RGM_NODE_INVALID           255

#ifdef IMS_CLIENT
ICF_CRM_START




#define ICF_RGM_MAX_RETRY					5



		
ICF_CRM_END
#endif

/* This is the duration which is to be subtracted from actual
 * registration duration in order to prevent expiry of registration
 * at Network Node end.
 * This is in milliseconds
 */
#define ICF_RGM_DUR_TO_BE_SUB          600000
ICF_CRM_START

/* If registration duration > 1200 seconds (=ICF_RGM_BASE_DURATION)
 * then re-register 600 seconds(=ICF_RGM_MARGIN_TIME) before the 
 * duration expires. Else re-register at half of registration time. */
#define	ICF_RGM_MARGIN_TIME			  600000
#define ICF_RGM_BASE_DURATION		  1200000

/* If "reg-event" subscription duration > 1200 seconds (=ICF_REG_EVENT_BASE_DURATION)
 * then re-subscribe at 600 seconds(=ICF_REG_EVENT_MARGIN_TIME) before the expiry
 * of subscription time.
 * Else re-subscribe in half the subs duration.
 */
#define ICF_REG_EVENT_MARGIN_TIME		600000
#define ICF_REG_EVENT_BASE_DURATION		1200000

/* The duration should not be Exceed to this value (2^32)/1000, If exceded
 * then RGM will not run the timer*/
#define ICF_RGM_MAX_DURATION		     4294967

/*CSR 8052488- SPR 20244 This variable defines the value of 
 registration guard timer.
It starts when a register/deregister request is send to peer */

#define ICF_REGM_GUARD_TIMER_DURATION        120000

/* To be deleted */
/*#define ICF_DISPLAY_USER_REGISTERED         1
#define ICF_DISPLAY_USER_NOT_REGISTERED     2*/



#ifdef ICF_IPSEC_ENABLED
/*------------------------------------------------------------------------------
 * Constants needed for IPSec Support in ICF Release 4.0
 *----------------------------------------------------------------------------*/

/* This defines the lower limit of the permissible range of spi values, as 
 * specified in RFC 3329 */
#define ICF_RGM_IPSEC_SPI_MIN                0
/* This defines the upper limit of the permissible range of spi values, as 
 * specified in RFC 3329 */
/* Please do not remove UL ..it gives warning */
#define ICF_RGM_IPSEC_SPI_MAX                4294967295UL

/* This defines the difference in registration duration and SA lifetime. This
 * value is in milliseconds */
#define ICF_RGM_IPSEC_DELTA_DURATION         30000

/* This defines the number of unidirectional SAs in one association between an 
 * IMPI and P-CSCF */
#define ICF_RGM_IPSEC_NUM_SA                 4

/* This defines the default SA lifetime if modify is not supported */
#define ICF_RGM_IPSEC_DEF_SA_LIFETIME        4294967295

/* ------------------ End: Constants for IP Security Support ---------------- */
#endif /* ICF_IPSEC_ENABLED */

#ifdef ICF_NAT_RPORT_SUPPORT
#define ICF_INITIAL_REG_ID      1
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_REGM_DEFS_H__ */ 
