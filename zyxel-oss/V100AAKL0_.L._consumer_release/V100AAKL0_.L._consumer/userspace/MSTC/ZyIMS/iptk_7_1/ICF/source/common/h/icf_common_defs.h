/*------------------------------------------------------------------------------
 *
 * File name        : icf_common_defs.h
 *
 * Purpose          : This file contains all hash define values   used by all 
 *                    internal modules of ICF. 
 *
 * Revision History :
 *
 * Date         Name            Ref#        Comments
 * --------     ------------    ----        ---------------
 * 03-Nov-2004  Mayank Rastogi  ICF_LLD  File created and initial version.
 * 22-Nov-2004  Rohit Aggarwal              Added ids for CC timers
 * 30-Dec-2004  Aashish Suchdev             Changes for avoiding IN/OUT redef
 * 26-Jan-2005  Rohit Aggarwal              Added defines for call state
 * 28-Jan-2005  Amit Sharma                 Added Services timer-ID and Correct  
 *                                          CTU module ID
 * 28-Jan-2005  Navneet Gupta               Added Service Start and End #defs
 *                                          CTU module ID
 * 31-Jan-2005  Jalaj Negi                  Added Timer #Defs for RGM Module
 * 14-Feb-2005  Amit Sharma                 Added Service Initiated REL type
 * 18-Feb-2005  Navneet Gupta               Changed Status Values to 32 bit.
 *                                          Added SL Bitmasks
 * 24-Feb-2005  Amit Sharma                 To add CTA_TIMER id
 * 15-Mar-2005  Amit Sharma                 To add RNW_TIMER id
 * 10-Apr-2005  Amit Sharma                 Added define for request pending 
 *                                          cause
 * 02-Jun-2005  Amit Sharma     SPR:7535    Added status for TWC wait
 * 23-Jun-2005  Jalaj Negi      IMS CLIENT  Added #defines for identifying
 *                                          codecs and their respective
 *                                          bandwidth's
 * 04-Aug-2005	Rohit Aggarwal	ICF			Changes for ICF compilation
 * 10-Aug-2005	Ashutos Rath	ICF			Added Status bit
 * 07-Dec-2005  Rohit Aggarwal  ICF 4.0 CDD Added constants for IPSec
 *
 * 30-Nov-2005	Jyoti Narula	ICF			Code delimiters for ICF4.0, REGM
 * 02-Feb-2006  Amit Sharma                 CFG merged with ICF 2.0
 * 07-Mar-2006	Anuradha Gupta	ICF			Added the new states for Re-invite handling
 *
 * 27-Nov-2006  Deepti Goyal    ICF Rel6.2  NAT Feature Support.
 * 03-Jan-2007  Amit Mahajan                INFO confirmation changes.
 * 26-Mar-2007  Deepti Goyal    ICF 6.4     Changes for media connect req API.
 *
 * 04-Apr-2007  Deepti Goyal    ICF 6.3     Changes for media connect req API.
 * 09-Jul-2007  Amit Sharma     ICF 7.0     Added changes for 7.0 enhancements
 * 11-Jul-2007  Neha Choraria   ICF 7.0     Changes for Retry-After header
 * Copyright (c) 2007, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_COMMON_DEFS_H__
#define __ICF_COMMON_DEFS_H__

#include "icf_port.h"
#include "icf_mm_api.h"
#include "icf_map_extif_feature_flags.h"
#ifdef ICF_STAT_ENABLED
#include "icf_stats_defs.h"
#endif
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

	/* Following flag is used to enable the usage of UATK 2.2 
	 * structures by SSA/REGM etc. This is to be enabled 
	 * if UATK 2.2 is to be used*/
#define ICF_WITH_MUATK_2_2		1

/* NEW ICF 6.2 flags */
#define ICF_STRICT_TEST		1
#define ICF_KLOCWORK_TEST	1

/******* Changes for Retry-After (Rel7.0) *******/
#define ICF_CMN_INC_RACE_TIMER_LOW_LIMIT    0
#define ICF_CMN_INC_RACE_TIMER_UP_LIMIT     2000
#define ICF_CMN_OG_RACE_TIMER_LOW_LIMIT     2100
#define ICF_CMN_OG_RACE_TIMER_UP_LIMIT      4000
/******* Changes for Retry-After (Rel7.0) *******/

/*------------------------------------------------------------------------------
 *
 * Contains hash define values for return_status in icf_internal_msg_hdr_st
 *
 *----------------------------------------------------------------------------*/

    /* the call/service instance for the module will continue execution */
#define  ICF_STATUS_CONTINUE                         0x00000000
#define ICF_STATUS_SERVICE_CONTINUE      ICF_STATUS_CONTINUE

#define ICF_STATUS_FAILURE                           0x00000001
#define ICF_STATUS_SUCCESS                           0x00000002


    /* the call/service instance for the module will end  */
#define  ICF_STATUS_END                              0x00000004
#define ICF_STATUS_SERVICE_ENDED_NORMAL  ICF_STATUS_END

    /* the module has no pending action ongoing on media interface and other
       module can initiate action on media interface */
#define  ICF_STATUS_MEDIA_ONGOING_ENDED              0x00000008

    /* the module has no ongoing sip transaction and other module can initiate
       sip transaction */
#define  ICF_STATUS_SIGNAL_ONGOING_ENDED             0x00000010

    /* CC would send this when media is committed. */
#define ICF_STATUS_MEDIA_COMMITTED                   0x00000020

    /* SCM/CC shall send this when service or call needs to be terminated.
     * */
#define ICF_STATUS_CALL_R_SRV_TERMINATE              0x00000040
#define ICF_STATUS_SERVICE_ENDED_ERROR   ICF_STATUS_CALL_R_SRV_TERMINATE

    /* SCM shall send this on fetting refer from network. */
#define ICF_STATUS_INITIATE_CALL                     0x00000080
#define ICF_STATUS_SERVICE_MSG_TO_CC_SETUP  ICF_STATUS_INITIATE_CALL 

    /* SCM shall send this for call waiting and CC shall also send this 
     * status for call waiting
     */
#define ICF_STATUS_CALL_WAITING                      0x00000100

    /* CC would send this for Call Forwarding Unconditional service */
#define ICF_STATUS_CALL_FWD_UNCOND                   0x00000200

    /* CC would send this for Call Forwarding No Answer service */
#define ICF_STATUS_CALL_FWD_NO_ANS                   0x00000400

    /* CC would send this for Call Forwarding Busy service */
#define ICF_STATUS_CALL_FWD_BUSY_LINE                0x00000800
    
    /* CC would send this for Call Forwarding Busy and call waiting service */
#define ICF_STATUS_CALL_FWD_BUSY_N_CALL_WAITING      0x00001000

/* Service Return Status */

/* TWC will fill this in case of error and CM need to check payload */
#define ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD    0x00002000

/* Internal Call Hold Request */
#define ICF_STATUS_SERVICE_CALL_HOLD_REQ             0x00004000

/* Internal Call Resume Request */
#define ICF_STATUS_SERVICE_CALL_RESUME_REQ           0x00008000

/* Internal TWC request */
#define ICF_STATUS_SERVICE_CALL_MERGE_REQ            0x00010000

/* Start Proc Signalling Req */
#define ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ   0x00020000

/* Start Proc Media Req */
#define ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ       0x00040000

/* Signalling Events Req */
#define ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ      0x00080000

/* Media Events Req */
#define ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ          0x00100000

/* Internal Call Hold Response */
#define ICF_STATUS_SERVICE_CALL_HOLD_RESP            0x00200000

/* Internal Call Resume Response */
#define ICF_STATUS_SERVICE_CALL_RESUME_RESP          0x00400000

/* Service Return Status for Both Calls invovled in the Service */
#define ICF_STATUS_SERVICE_APPLY_BOTH_CALLS          0x00800000

/* Service Return Status for Other Call invovled in the Service */
#define ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL        0x01000000

/* Return status from CC/SCM to CM on getting ecode generic clr from SSA/MMI */
#define ICF_STATUS_GENERIC_CALL_CLEAR                0x02000000

/* Return status from CTA when it is waiting for TWC response*/
#define ICF_STATUS_SERVICE_AWT_TWC_RESP              0x04000000

/* Return status from SIGSA when it is waiting for OPTIONS response from
 * Peer Node for SigComp Capability. Other Module can also use this for
 * asynchronous response of a requested API*/
#define ICF_STATUS_WAIT_TILL_RESP_RCVD               0x08000000

/*indicates that the request is pending, results will
	come later in another API */

#define  ICF_STATUS_NM_REQUEST_PENDING	0x10000000



/*indicates that the request is completed synchronously*/
#define ICF_STATUS_NM_REQUEST_COMPLETED   0x20000000

/* indicates that no user is there on this line */
#define ICF_NO_USER_ON_LINE             0x40000000

/* Return status from CHR when it is handling reinvite in glare condition */
#define ICF_STATUS_CC_HANDLE_REINVITE             0x80000000

/*Following are return status defined for status_2 in internal message */

/*Indicates that Join header is received in incoming call and TWC service is
 *to be invoked to establish conference*/
#define ICF_STATUS_INVOKE_TWC_FOR_JOIN              0x00000001

/*Indicates that CTA to be invoked in the return path of XCONF*/
#define ICF_STATUS_INTERNAL_REFER_W_REP_REQ         0x00000002
    
/*Indicates that CTU to be invoked in the return path of XCONF*/
#define ICF_STATUS_INTERNAL_REFER_REQ               0x00000004
    
/*Indicates that call is connected with conference server and isfocus is received
 *it is returned from CC*/
#define ICF_STATUS_EXTERNAL_CONFERENCE_CONNECTED    0x00000008
    
/*This is returned from CTU/CTA when REFER service is completed successfully*/
#define ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP      0x00000010
    
/*This is returned from CTU/CTA when REFER service is completed unsuccessfully*/
#define ICF_STATUS_INTERNAL_REFER_FAILURE_RESP      0x00000020
    
/*This is returned from CC when incoming INVITE is received with isfocus*/
#define ICF_STATUS_INCOMING_EXTERNAL_CONF_CALL      0x00000040

/* This is returned from XCONF to SCM then to CTU/CTA for aborting REFER */
#define ICF_STATUS_SERVICE_ABORT_INTERNAL_REFER_REQ 0x00000080

/* This is send by XCONF to SCM for trigger to CC for release but
   maintain the service and service logic ctx */
#define ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE        0x00000100    
    
#if 0
/*------------------------------------------------------------------------------
 *
 * contains hash define values for ongoing_event field in icf_call_ctx_st
 *
 *----------------------------------------------------------------------------*/

    /*  there is no ongoing event */
#define  ICF_CC_ONGOING_EVENT_NONE                  0

    /*  ongoing event is ICF_START_FAX_REQ */
#define  ICF_CC_ONGOING_EVENT_APP_FAX_REQ            1

    /*  ongoing event is remote fax req  */
#define  ICF_CC_ONGOING_EVENT_REMOTE_FAX_REQ         2

    /*  ongoing event is alert with sdp (180 with sdp) */
#define  ICF_CC_ONGOING_EVENT_ALERT_WITH_SDP        3

    /*  ongoing event is media commit (183 resp) */
#define  ICF_CC_ONGOING_EVENT_MEDIA_COMMIT          4

    /*  ongoing event is connect (200 ok or PA Connect) */
#define  ICF_CC_ONGOING_EVENT_CONNECT               5

    /*  ongoing event is connect resp with sdp (200 ack with sdp) */
#define  ICF_CC_ONGOING_EVENT_CONNECT_RESP_WITH_SDP 6

    /*  ongoing event is media update */
#define  ICF_CC_ONGOING_EVENT_MEDIA_UPDATE          7

    /*  ongoing event is reinvite */
#define  ICF_CC_ONGOING_EVENT_REINVITE              8

    /*  ongoing event is redirect (302 resp) */
#define  ICF_CC_ONGOING_EVENT_REDIRECT              9

    /* Ongoing Event is Alert    */
#define  ICF_CC_ONGOING_EVENT_PA_ALERT               10 
#endif
/*------------------------------------------------------------------------------
 *
 * contains hash define values for release_type field in icf_call_ctx_st
 *
 *----------------------------------------------------------------------------*/

    /*  release type is having invalid value */
#define  ICF_REL_TYPE_INVALID                       0x00

    /*  release is PA initiated */
#define  ICF_REL_TYPE_PA_INITIATED                  0x01

    /*  release is network initiated */
#define  ICF_REL_TYPE_NW_INITIATED                  0x02

    /*  release is MM initiated */
#define  ICF_REL_TYPE_MM_INITIATED                  0x04

    /*  release is due to internal error which includes processing errors, 
        system errors like memory allocation/deallocation failure, msg 
        send/recv failure */
#define  ICF_REL_TYPE_INTERNAL_INITIATED            0x08

    /*  release is due to timer expiry */
#define  ICF_REL_TYPE_TIMER_EXPIRY                  0x10

    /*  release is due to timer expiry */
#define  ICF_REL_TYPE_SERVICE_INITIATED             0x20

/*------------------------------------------------------------------------------
 *
 * contains hash define values for call_state field in icf_call_ctx_st
 *
 *----------------------------------------------------------------------------*/

#define ICF_CC_00_IDLE                           0
#define ICF_CC_01_AWT_CMS_RESP                   1
#define ICF_CC_02_AWT_SSA_SETUP_RESP             2
#define ICF_CC_03_AWT_ALERT_RESP                 3
#define ICF_CC_04_AWT_MMS_RESP                   4
#define ICF_CC_05_AWT_CONNECT                    5
#define ICF_CC_06_CONNECTED                      6
#define ICF_CC_07_AWT_DMS_RESP                   7
#define ICF_CC_08_AWT_CALL_TERMINATE_RESP        8
#define ICF_CC_09_AWT_MEDIA_PARAM_PRACK          9
#define ICF_CC_10_AWT_ALERT_PRACK                10
#define ICF_CC_11_AWT_SSA_CONNECT_ACK            11
#define ICF_CC_12_WAIT_START_PROC_SIG            12
#define ICF_CC_13_WAIT_START_PROC_MEDIA          13
#define ICF_CC_14_AWT_CALL_MODIFY_RESP		     14
#define	ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP     15  
#define	ICF_CC_16_AWT_UPDATE			         16
/*#define	ICF_CC_17_AWT_SSA_MEDIA_CHANGE_RSP	 17*/
#define	ICF_CC_18_CALL_STATE_INVALID		     18
#define ICF_CC_19_AWT_SSA_CALL_MODIFY_ACK		 19
/* the state specified below is used when the CC is waiting
for the early call indication response from app */
#define ICF_CC_20_AWT_EARLY_IND_CFM				 20
#define ICF_CC_MAX_STATES                        21



/*------------------------------------------------------------------------------
 *
 * contains hash define values for line_status field in icf_line_data_st
 *
 *----------------------------------------------------------------------------*/

    /*  line is free */
#define  ICF_LINE_STATUS_FREE                       0

    /*  line is busy */
#define  ICF_LINE_STATUS_BUSY                       1


/* --------------------------------------------------------------------------
 * Constants to check the error handler in MEMGET macros
 * --------------------------------------------------------------------------*/

#define ICF_RET_ON_FAILURE               0
#define ICF_DONOT_RET_ON_FAILURE         1

/* --------------------------------------------------------------------------
 * Constants to set the maximum modules that can be traversed in one processing
 * without returning control back. This basically tracks how many modules
 * are part of process stack.
 * --------------------------------------------------------------------------*/
#define ICF_MAX_MODULE_TVSL 15 



/* --------------------------------------------------------------------------
 * Constant to set and check for NULL
 * --------------------------------------------------------------------------*/
#define ICF_NULL 0
    
/* --------------------------------------------------------------------------
 * Constants to check the return value of functions
 * --------------------------------------------------------------------------*/
/*
#define ICF_FAILURE   0
#define ICF_SUCCESS   1
*/

/* --------------------------------------------------------------------------
 * Constants to describe function arguments
 * --------------------------------------------------------------------------*/
#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN   const
#endif

/* --------------------------------------------------------------------------
 * Constants to set and check the boolean varibles
 * --------------------------------------------------------------------------*/
/*
#define ICF_TRUE 1
#define ICF_FALSE 0
*/

/* ------------------------------------------------------------------------
 * Hash defines :  icf_int_module_id_t
 * Description  : To define the internal module ids used within SIP packet
 *                phone signaling toolkit. 
 * -------------------------------------------------------------------------*/
#define ICF_INT_MODULE_INVALID    0
#define ICF_INT_MODULE_MRM        1
#define ICF_INT_MODULE_CM         2
#define ICF_INT_MODULE_CC         3
#define ICF_INT_MODULE_DBM        4
#define ICF_INT_MODULE_SM         5
#define ICF_INT_MODULE_REGM       6
#define ICF_INT_MODULE_CFG        7
#define ICF_INT_MODULE_SSA        8
#define ICF_INT_MODULE_SIP_UA     9
#define ICF_INT_MODULE_SIP_STACK  10
#define ICF_INT_MODULE_MMI        11
#define ICF_INT_MODULE_SCM        12
#define ICF_INT_MODULE_ADM        13
#define ICF_INT_MODULE_SL_START   14
#define ICF_INT_MODULE_CHR        15
#define ICF_INT_MODULE_RNW        16
#define ICF_INT_MODULE_TWC        17
#define ICF_INT_MODULE_CTA        18
#define ICF_INT_MODULE_CTU        19
#define ICF_INT_MODULE_CFW        20
#define ICF_INT_MODULE_XCONF      21 	
#define ICF_INT_MODULE_SL_END     22
#define ICF_INT_MODULE_SIGSA      23
#define ICF_INT_MODULE_SIC        24
#define ICF_INT_MODULE_NM         25
#define ICF_INT_MODULE_MPH        26


/* Internal #defines for external entities */
#define ICF_MODULE_SIP_NW         27
/*Identifies a message from mgmt server*/
#define ICF_MODULE_MGMT_SERVER    28
#define ICF_INT_MODULE_MWI		  29
#define ICF_INT_MAX_MODULE        30 

    
/* Number of Service Logic Modules */
#define ICF_SCM_MAX_SL \
    (ICF_INT_MODULE_SL_END - ICF_INT_MODULE_SL_START)

/* Service Bitmasks for Service Logic Modules */
#define ICF_SERVICE_INVALID          0X00
#define ICF_SERVICE_CHR              0x01
#define ICF_SERVICE_RNW              0x02
#define ICF_SERVICE_TWC              0x04
#define ICF_SERVICE_CTA              0x08
#define ICF_SERVICE_CTU              0x10
#define ICF_SERVICE_CFW              0x20
#define ICF_SERVICE_XCONF            0x40


/* ----------------------------------------------------------------------
 * Hash Defines : icf_timer_type_t 
 * Description  : This contains the different timer ids for timers started 
 *                by CC.
 * -----------------------------------------------------------------------*/

#define ICF_TIMER_TYPE_SETUP         0
#define ICF_TIMER_TYPE_ALERTING      1
#define ICF_TIMER_TYPE_MEDIA         2
#define ICF_TIMER_TYPE_RELEASE       3
#define ICF_CTU_TIMER                4
#define ICF_CHR_TIMER                5
#define ICF_TWC_TIMER                6
#define ICF_CTA_TIMER                7
#define ICF_RNW_TIMER                8
#define ICF_NAT_TIMER                9

        /*SSA_INVOKED_TIMERS START*/
#define ICF_SSA_UATK_TIMER_TYPE           9
#define ICF_SSA_PURGE_TIMER_TYPE          10

#ifdef IMS_CLIENT
#define ICF_SSA_TIMER_RETRY_AFTER    11

#define ICF_SSA_TIMER_RE_SUBSCRIBE   12
#endif



/* Timers of SIC */

/* Response timer */
#define ICF_TIMER_TYPE_SUBS_RESPONSE	     13
       
/* Duration timer */        
#define ICF_TIMER_TYPE_SUBS_DURATION     14
        
/* Retry timer */        
#define ICF_TIMER_TYPE_SUBS_RETRY        15  

/*  poll subscription notify timer */
#define ICF_TIMER_TYPE_AWAIT_SUBS_NOTIFY_REQ	 16
/* This timer would be started by CC when sending info request on network*/
#define ICF_TIMER_TYPE_INFO	 17

#define ICF_SSA_OPTIONS_TIMER_TYPE       18

#define ICF_TIMER_TYPE_NOTIFY			 19

/* ICF_6_1_OPTIONS_SUPPORT start */

#define ICF_SIC_OPTIONS_TIMER		20

#ifdef ICF_NAT_RPORT_SUPPORT
#define ICF_BINDING_REFRESH_TIMER_TYPE  21
#endif

/* This timer would be started by CC when sending info indication 
 * to the application.*/
#define ICF_TIMER_TYPE_INFO_CFM	 22

/* DNS Response Timer, this timer will be started while initiating
 * DNS query and will be stopped when DNS query resp is received */
#define ICF_TIMER_TYPE_DNS_RESP_TIMER            23

#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
#define ICF_TIMER_TYPE_LOAD_CALL_DURATION  24
#endif

#define ICF_TIMER_TYPE_RETRY_AFTER    25

/* ICF_6_1_OPTIONS_SUPPORT end */

/*SSA_INVOKED_TIMERS END*/

/* This timer is started when registration request is sent and stopped when 
 * response to registration request is received
 */    
#define ICF_TIMER_TYPE_RGM_RETRY              1

/* This is the registration duration timer */
#define ICF_TIMER_TYPE_RGM_REGISTRATION (ICF_TIMER_TYPE_RGM_RETRY + 1)

/* This timer indicates the duration after which registration is retried */    
#define ICF_TIMER_TYPE_RGM_REATTEMPT (ICF_TIMER_TYPE_RGM_REGISTRATION + 1)

#define ICF_TIMER_TYPE_MPH_NO_RESP (ICF_TIMER_TYPE_RGM_REATTEMPT + 1)

#define ICF_TIMER_TYPE_MPH_CREATE_CONN (ICF_TIMER_TYPE_MPH_NO_RESP + 1)

#define ICF_TIMER_TYPE_PROBATION   (ICF_TIMER_TYPE_MPH_CREATE_CONN + 1)

#define ICF_TIMER_TYPE_CFG_NET_ACT_RETRY	(ICF_TIMER_TYPE_PROBATION + 1)

#ifdef ICF_IPSEC_ENABLED
/*------------------------------------------------------------------------------
 * Timers needed for IPSec Support in ICF Release 4.0
 *----------------------------------------------------------------------------*/

/* This is the timer type for guard timer that shall be started by REGM after 
 * successfully sending SA creation request to NM, and shall be stopped by REGM 
 * on receiving success or failure SA creation response from NM */
#define ICF_TIMER_TYPE_RGM_SA_CREATE (ICF_TIMER_TYPE_CFG_NET_ACT_RETRY + 1)

/* This is the timer type for guard timer that shall be started by REGM after 
 * successfully sending SA modification request to NM, and shall be stopped by 
 * REGM on receiving success or failure SA modification response from NM */
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
#define ICF_TIMER_TYPE_RGM_SA_MODIFY (ICF_TIMER_TYPE_RGM_SA_CREATE + 1)
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */

/* This is the timer type for guard timer that shall be started by REGM after 
 * successfully sending SA deletion request to NM, and shall be stopped by REGM 
 * on receiving success or failure SA deletion response from NM */
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
#define ICF_TIMER_TYPE_RGM_SA_DELETE (ICF_TIMER_TYPE_RGM_SA_MODIFY + 1)
#else
#define ICF_TIMER_TYPE_RGM_SA_DELETE (ICF_TIMER_TYPE_RGM_SA_CREATE + 1)
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */

/* ------------------ End: Timers for IP Security Support ------------------- */
#endif /* ICF_IPSEC_ENABLED */

/*----------------------------------------------------------------------
 *  Timer Type for License Management
 *-----------------------------------------------------------------------*/
/*  This timer is run for the 24 hour interval: Logic to implement
 *  Licensing Policy */
#define ICF_TIMER_TYPE_LICENSE_CHK           1

/* DNS Buffer timer type */
#define ICF_TIMER_TYPE_DNS_BUFFER_TIMER            32

   /* Size of the maximum network call id. */
#define ICF_MAX_NW_CALL_ID_DGTS       256
    
    /* Invalid call id */
/*#define ICF_INVALID_CALL_ID          0xffff   */

    /* Invalid connection id */
#define ICF_INVALID_CONN_ID          65535     

   
    /* Size of the maximum network call id. */
#define ICF_MAX_NW_CALL_ID_DGTS       256
    


/* This will be used by SSA while resolving domain name in registration request
 */
#define ICF_MAX_IP_PER_REGISTRAR     5

#ifdef IMS_CLIENT
/* IANA Payload Types for the Codecs. These values must be same as in 
 * IFX_AL resource manager files.
 */
#define ICF_IANA_PT_G711_ULAW     0 
#define ICF_IANA_PT_G711_ALAW     8
#define ICF_IANA_PT_G729_8        18

/* Bandwidths supported by codecs. This is to be filled in AS: lines */
#define ICF_IANA_PT_G711_ULAW_BW  64
#define ICF_IANA_PT_G711_ALAW_BW  64
#define ICF_IANA_PT_G729_8_BW     8
        
/* This is the maximum number of bandwidths which can be supported
 * by any codec.
 */
#define ICF_MAX_NUM_OF_BW_PER_CODEC  5


#endif

#ifdef ICF_QOS_SUPPORT
/* This defines the percentage of total bandwidth which is to be allocated to
 * active data senders
 */
#define ICF_PERCENT_BW_ALLOC_TO_RS   5

/* This defines the percentage of total bandwidth which is to be allocated to
 * data receivers
 */
#define ICF_PERCENT_BW_ALLOC_TO_RR   0
#endif

 /*------------------------------------------------------------------------------
 *
 * contains hash define for the causes that CC would give at the time of calling
 * icf_chk_line_service
 *
 *----------------------------------------------------------------------------*//*  line is free */
#define  ICF_CAUSE_LINE_FREE                       0

/*  line is busy */
#define  ICF_CAUSE_LINE_BUSY                       1

 /*This is required in case call wait timer expire and we need to initiate CFNR 
 * call service
 */
#define ICF_CAUSE_NO_ANSWER                        2

/*This is required in case PA sends CAll teminate with reason as USER_BUSY to 
 * check if call is to be forwarded
 */
#define ICF_CAUSE_CALL_REJECT_BY_USER              3


#define ICF_CALL_CLEAR_REASON_PRECONDITIONS_FAILURE  4

#define ICF_CAUSE_RESOURCE_NOT_AVAILABLE				5

/* ICF_6_1_OPTIONS_SUPPORT start */

/*-------------------------------------------------------
 * These error codes will be used by SIC to send 
 * OPTIONS failure indications to SSA. current only one
 * error code is used.
 *-------------------------------------------------------*/

/* ICF_6_!_OPTIONS_SUPPORT end */

/*------------------------------------------------------------------------------
 * Internal failure cause
 *------------------------------------------------------------------------------*/

#define ICF_CALL_CLEAR_REASON_REQUEST_PENDING   \
                     ICF_CALL_CLEAR_REASON_END + 1

/*-----------------------------------------------------------------------------
 * CFG Invalid value. Used for traces only 
 *----------------------------------------------------------------------------
 */

#define ICF_CFG_INVALID_VALUE        0xffff
/*typedef enum
{
    ICF_EVENT_PKG_INVALID=0,
    ICF_EVENT_PKG_MESSAGE_SUMMARY,
    ICF_EVENT_PKG_REG,
	ICF_EVENT_PKG_CONFERENCE,
	ICF_EVENT_PKG_POC_SETTINGS,
	ICF_EVENT_PKG_REFER

} icf_event_packages_et;*/

/*------------------------------------------------------------------------------
 *
 * Enum for the states of SIC
 *
 *----------------------------------------------------------------------------*/
typedef enum
{
    
    ICF_SIC_00_IDLE=0,
    ICF_SIC_01_SUBSCRIBING,
    ICF_SIC_02_SUBSCRIBED,
    ICF_SIC_03_UNSUBSCRIBING,
    ICF_SIC_04_SUBSCRIBE_RETRY,
	ICF_SIC_STATE_INVALID,
    ICF_SIC_MAX_STATES
} icf_sic_states_et; 

/* ICF_6_1_OPTIONS_SUPPORT_START */
/*----------------------------------------------------------------------------
 *
 * Enum for the options fsm states
 *
 *---------------------------------------------------------------------------*/
typedef enum
{
	ICF_SIC_00_OPTIONS_IDLE=0,
	ICF_SIC_01_AWT_MEDIA_CAPAB_RSP,
	ICF_SIC_02_AWT_OPTIONS_APP_CFM,
	ICF_SIC_03_AWT_OPTIONS_NW_CFM,
	ICF_SIC_MAX_OPTIONS_STATES
} icf_sic_options_states_et;

/* ICF_6_1_OPTIONS_SUPPORT_END */



#ifdef ICF_SERVICE_ENABLE
/*----------------------------------------------------------------------------
 *
 * TYPEDEF NAME: icf_service_type_et
 *
 * DESCRIPTION:  Would represent service being operated.
 *   
*----------------------------------------------------------------------------*/ 
typedef enum 
{
    ICF_SERVICE_CALL_HOLD_RESUME  = 0,
    ICF_SERVICE_THREE_WAY_CALL,
    ICF_SERVICE_CALL_TRF_ATTENDED,
    ICF_SERVICE_CALL_TRF_UNATTENDED,
    ICF_SERVICE_INVITE_WITH_REPLACE,
    ICF_SERVICE_CALL_TRF_FROM_NW,
    ICF_SERVICE_CALL_FWD_UNCONDITIONAL,
    ICF_SERVICE_CALL_FWD_BUSY,
    ICF_SERVICE_CALL_FWD_NO_ANSWER,
    ICF_SERVICE_CALL_WAITING,
    ICF_SERVICE_MESSAGE_WAITING_IND,
    ICF_SERVICE_EXTERNAL_CONFERENCE,
    ICF_SERVICE_JOIN,
    ICF_MAX_SERVICE
} icf_service_type_et;

#endif

/*------------------------------------------------------------------------------
 * Macros for notify state/event for reg-event package
 *------------------------------------------------------------------------------*/
#define 	ICF_RGM_NOTIFY_NO_STATE				0
#define     ICF_RGM_NOTIFY_STATE_ACTIVE			1
#define     ICF_RGM_NOTIFY_STATE_PENDING			2
#define     ICF_RGM_NOTIFY_STATE_TERMINATED		3

#define     ICF_RGM_NOTIFY_NO_EVENT				0
#define     ICF_RGM_NOTIFY_EVENT_DEACTIVATED		1
#define     ICF_RGM_NOTIFY_EVENT_REJECTED		2
#define		ICF_RGM_NOTIFY_EVENT_SHORTENED		3
/*------------------------------------------------------------------------------
 * ICF code delimiters
 *------------------------------------------------------------------------------*/

/* this is a tag to mark start of CRM related change */
#define ICF_CRM_START
/* this is a tag to mark end of CRM related change */ 
#define ICF_CRM_END
/* this is a tag to mark start of CRM related change */
#define ICF_INIT_START
/* this is a tag to mark end of CRM related change */
#define ICF_INIT_END
/* this is a tag to mark start of session related change */
#define ICF_SESSION_CONTROL_START
/* this is a tag to mark end of session related change */
#define ICF_SESSION_CONTROL_END
/* this is a tag to mark start of App-MM api routing code */
#define ICF_MM_ROUTING_START
/* this is a tag to mark end of App-MM api routing code */
#define ICF_MM_ROUTING_END
/* this is a tag to mark start of SIC related change */
#define ICF_SIC_START
/* this is a tag to mark end of SIC related change */
#define ICF_SIC_END
    
/* this is a tag to mark start of Network Recovery related change */
#define ICF_NW_RECOVERY_START
/* this is a tag to mark end of Network Recovery related change */ 
#define ICF_NW_RECOVERY_END

/* this is a tag to mark start of new configuration staus API's change */
#define ICF_CONF_STATUS_API_START

/* this is a tag to mark end of new configuration staus API's change */
#define ICF_CONF_STATUS_API_END

#define  ICF_PDP_CREATE_NEW_TFT		   			1
#define  ICF_PDP_DELETE_EXISTING_TFT				2
#define  ICF_PDP_ADD_PKT_FILTERS_TFT				3
#define  ICF_PDP_REPLACE_PKT_FILTERS_TFT 		4
#define  ICF_PDP_DELETE_PKT_FILTERS_TFT 			5
#define  ICF_PDP_NO_TFT_OPERATION 				6

#define ICF_MAX_TFT_PKT_FILTER_LIST			    4
#define ICF_MAX_TFT_PARAMS_LIST				    8

#define ICF_MAX_AUTH_TOKEN_OCTET			    50

#define ICF_PDP_PARAM_IDENT_AUTH_TOKEN	 	    1
#define ICF_PDP_PARAM_IDENT_FLOW_ID 			    2
#define ICF_MAX_TFT_FLOW_IDS					    8

#define ICF_QOS_CONVERSATIONAL_TRAFFIC_CLASS		1
#define ICF_QOS_STREAMING_TRAFFIC_CLASS			2
#define ICF_QOS_INTERACTIVE_TRAFFIC_CLASS		3
#define ICF_QOS_BACKGROUND_TRAFFIC_CLASS			4

#define ICF_TFT_COMPONENT_IPV4_SOURCE_ADDRESS_TYPE			16
#define ICF_TFT_COMPONENT_IPV6_SOURCE_ADDRESS_TYPE			32
#define ICF_TFT_COMPONENT_PROTOCOL_IDENTIFIER				48
#define ICF_TFT_COMPONENT_SINGLE_DESTINATION_PORT_TYPE		64
#define ICF_TFT_COMPONENT_DESTINATION_PORT_RANGE_TYPE		65
#define ICF_TFT_COMPONENT_SINGLE_SOURCE_PORT_TYPE			84
#define ICF_TFT_COMPONENT_SINGLE_PORT_RANGE_TYPE			85
#define ICF_TFT_COMPONENT_SECURITY_PARAMETER_INDEX_TYPE		96
#define ICF_TFT_COMPONENT_TRAFFIC_CLASS_TYPE				112
#define ICF_TFT_COMPONENT_FLOW_LABEL_TYPE					128

#define ICF_MAX_TFT_PKT_FILTER_CONTENT						2

/* max media flows is used to allocate the max space required for 
 * media flows array*/
#define ICF_MAX_MEDIA_FLOWS									ICF_MAX_STREAM_PER_CALL * 2

 /* invalid flow index value denotes that the corresponding flow_index
 * is not applicable for RTP flow*/
#define ICF_INVALID_FLOW_IDX								ICF_MAX_MEDIA_FLOWS
#define ICF_MAX_FLOW_DIRECTIONS								2 /*inflow + outflow*/


/* ICF_RELEASE4_TBD */
#define ICF_QOS_AMR_NB_CODEC_BANDWIDTH           10
/* ICF_RELEASE4_TBD_END */

/* Code Delimiters to mark the REL 4.0 changes for ICF */
#define ICF_4_0_CRM_START
#define ICF_4_0_CRM_END 

/* this is a tag to mark start of Network Recovery related change */
#define ICF_NW_RECOVERY_START
/* this is a tag to mark end of Network Recovery related change */ 
#define ICF_NW_RECOVERY_END

/* this is a tag to mark start of new configuration staus API's change */
#define ICF_CONF_STATUS_API_START

/* this is a tag to mark end of new configuration staus API's change */
#define ICF_CONF_STATUS_API_END

/* this is a tag to mark start of Re-Invite handling */
#define ICF_HANDLE_REINVITE_START

/* this is a tag to mark end of Re-Invite handling */
#define ICF_HANDLE_REINVITE_END

#define  ICF_PDP_CREATE_NEW_TFT		   			1
#define  ICF_PDP_DELETE_EXISTING_TFT				2
#define  ICF_PDP_ADD_PKT_FILTERS_TFT				3
#define  ICF_PDP_REPLACE_PKT_FILTERS_TFT 		4
#define  ICF_PDP_DELETE_PKT_FILTERS_TFT 			5
#define  ICF_PDP_NO_TFT_OPERATION 				6

#define ICF_MAX_TFT_PKT_FILTER_LIST			    4
#define ICF_MAX_TFT_PARAMS_LIST				    8

#define ICF_MAX_TFT_PKT_FILTER_OCTET			    50

#define ICF_PDP_PARAM_IDENT_AUTH_TOKEN	 	    1
#define ICF_PDP_PARAM_IDENT_FLOW_ID 			    2
#define ICF_MAX_TFT_FLOW_IDS					    8

#define ICF_QOS_CONVERSATIONAL_TRAFFIC_CLASS		1
#define ICF_QOS_STREAMING_TRAFFIC_CLASS			2
#define ICF_QOS_INTERACTIVE_TRAFFIC_CLASS		3
#define ICF_QOS_BACKGROUND_TRAFFIC_CLASS			4

/*ICF_RELEASE4_TBD*/

#define ICF_QOS_AMR_NB_CODEC_BANDWIDTH           10
#define ICF_QOS_AUDIO_CODEC_BANDWIDTH            10
#define ICF_QOS_VIDEO_CODEC_BANDWIDTH            10

#define ICF_INVALID_FD                           65535


/*----------------------------------------------------------------------------
 *
 * TYPEDEF NAME: icf_dbm_block_type_t
 *
 * DESCRIPTION:  Would represent various block types in DBM_POOL
 *
*----------------------------------------------------------------------------*/
typedef enum 
{
  ICF_BLK_TYPE_CALL_CTX  = 0,
  ICF_BLK_TYPE_SRVC_CTX  = 1,
  ICF_BLK_TYPE_SL_CTX    = 2,
  ICF_BLK_TYPE_RGN_CTX   = 3,
  ICF_BLK_TYPE_SUBM_CTX  = 4,
  ICF_MAX_BLK_TYPE       = 5  
} icf_dbm_block_type_et;


/*-----------------------------------------------------------------------------
 * Macro for system memory and timer resource scaling factors
 *---------------------------------------------------------------------------*/
/* default scaling factor is 1.0 */
#define ICF_SYS_SCALING_FACTOR_DEFAULT           2

/* ICF_SYS_RESOURCE_LIMIT = 1 in icf.cfg => scaling factor is 1.0 */
#define ICF_SYS_SCALING_FACTOR_1_0               2

/* ICF_SYS_RESOURCE_LIMIT = 2 in icf.cfg => scaling factor is 1.5 */
#define ICF_SYS_SCALING_FACTOR_1_5               3

/* ICF_SYS_RESOURCE_LIMIT = 3 in icf.cfg => scaling factor is 2.0 */
#define ICF_SYS_SCALING_FACTOR_2_0               4

/* ICF_SYS_RESOURCE_LIMIT = 4 in icf.cfg => scaling factor is 3.0 */
#define ICF_SYS_SCALING_FACTOR_3_0               6



/*----------------------------------------------------------------------------
 *
 *			New Mapping of API Ids
 * 
 *
*----------------------------------------------------------------------------*/
#define ICF_API_ID_UNDEFINED                  ICF_API_ID_INVALID

/* Denotes invalid app id */
#define ICF_APP_ID_INVALID				ICF_INVALID_APP_ID

#define ICF_CFG_CC_SET_SCALE_PARAMS_REQ     ICF_SET_SCALE_PARAMS_REQ

/* This API is used to configure transport related parameters like proxy 
 * address per line, self IP address and port on which network (SIP) 
 * messages will be received/Sent. */

#define ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ   ICF_SET_TRANSPORT_REQ

/* This API is used to configure system wide call related parameters like
 * timer values, call logging information, handling of SIP procedures etc. */

#define ICF_CFG_CC_SET_CALL_PARAMS_REQ        ICF_SET_CALL_PARAM_REQ

/* This API is used to configure user identity related parameters like 
 * user address and user name. It is configured against each line id. */

#define ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ     ICF_SET_SELF_ID_REQ

#define ICF_CFG_CC_SET_SERVICE_PARAMS_REQ     ICF_SET_SERVICE_PARAMS_REQ

/* This API is used to configure registration related parameters like 
 * registrar address, registration duration, protocol(TCP/UDP) for each 
 * registrar etc. */

#define ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ ICF_SET_REGISTRATION_REQ

/* This is an API used by SIP Packet Phone Signaling Toolkit to respond 
 * back to Configuration Module for any API from Configuration Module. */

#define ICF_CC_CFG_SET_CONFIG_PARAM_RESP      ICF_CONFIGURE_CFM


/* Following APIs are sent from SM to SIP Packet Phone Signaling Toolkit to
 * enable certain system management processing. */

/* This API is sent by system administration module to enable or disable 
 * the reporting of traces of a specific level or type, during system 
 * runtime. There is no response for this message. */

#define ICF_SM_CC_RESTART_REQ		      ICF_RESTART_REQ
#define ICF_SM_CC_DELETE_CONFIG_PARAMS_REQ    ICF_REINITIALISE_REQ

#define ICF_SM_CC_SET_TRACE_LEVEL_REQ         ICF_SET_TRACE_LEVEL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to report a 
 * trace of given level and type.  */

#define ICF_CC_SM_REPORT_TRACE_IND            ICF_REPORT_TRACE_IND

/* This API is sent by system administration module to enable or disable 
 * the reporting of errors of a specific level or type, during system 
 * runtime. There is no response for this message. */

#define ICF_SM_CC_SET_ERROR_REPORT_LEVEL_REQ  ICF_SET_ERROR_REPORT_LEVEL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to report an 
 * error of given level and type.  */

#define ICF_CC_SM_REPORT_ERROR_IND            ICF_REPORT_ERROR_IND

/* This API is sent by system administration module to enable or disable 
 * the updation of a specific stats or stats group. It also specifies whether 
 * the current stats values in the counters needs to be resetor not. There 
 * is no response for this message. */

#define ICF_SM_CC_SET_STATS_REQ               ICF_SET_STATS_REQ

/* This API is sent by system administration module to get the current value 
 * of a specific stats or stats group. It also specifies whether the current
 * stats values in the counters needs to be reset or not. The response to 
 * this API contains the stats value. */

#define ICF_SM_CC_GET_STATS_REQ               ICF_SM_GET_STATS_REQ

/* The API contains the response for ICF_SM_CC_GET_STATS_REQ to 
 * indicate the current value of the requested stats. */

#define ICF_CC_SM_GET_STATS_RESP              ICF_SM_GET_STATS_RESP

/* This API is sent by the Syatem Manager to check the health of the 
 * SIP Packet Phone Signaling Toolkit at regular intervals. */

#define ICF_SM_CC_CHECK_MODULE_STATUS_REQ     ICF_CHECK_MODULE_STATUS_REQ

/* This API is sent in response tp ICF_SM_CC_CHECK_MODULE_STATUS_REQ to
 * indicate that SIP Packet Phone Signaling Toolkit is alive. */

#define ICF_CC_SM_CHECK_MODULE_STATUS_RESP    ICF_CHECK_MODULE_STATUS_RESP


/* The following are list of APIs used to indicate the APIs from PA to 
 * SIP Packet Phone Signaling Toolkit and back. */

#define ICF_PA_API_START                      ICF_PA_API_START

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to inform about the incoming call from network so that the phone 
 * application csn start alerting the user. */

#define ICF_CC_PA_INCOMING_CALL_IND           ICF_INCOMING_CALL_IND

/*ICF_HANDLE_REINVITE_START*/

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to inform about the incoming call modify request */
#define ICF_CC_PA_INCOMING_CALL_MODIFY_IND			ICF_INCOMING_CALL_MODIFY_IND

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit 
 * to send response of the call modify request */
#define ICF_PA_CC_CALL_MODIFY_CFM					ICF_CALL_MODIFY_CFM

/*cta_handling*/
/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * to send response of the call call transfer initiated indication */
#define ICF_PA_CC_REMOTE_CALL_TRANSFER_INITIATED_CFM   ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM


/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to inform about the Cancel of the incoming call modify request */
#define ICF_CC_PA_INCOMING_CALL_MODIFY_CANCEL_IND	ICF_INCOMING_CALL_MODIFY_CANCEL_IND

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit 
 * to make a call modify request */
#define ICF_PA_CC_CALL_MODIFY_REQ					ICF_CALL_MODIFY_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to give final response to call modify request */
#define ICF_CC_PA_CALL_MODIFY_RESP					ICF_CALL_MODIFY_RESP

/*ICF_HANDLE_REINVITE_END*/

/* This API is send by Phone Application towards SIP Packet Phone Signaling 
 * Toolkit to inform about the call initiated by user so that the SIP 
 * Packet Phone Signaling Toolkit can initiate call towards remote party. */

#define ICF_PA_CC_CREATE_CALL_REQ             ICF_CREATE_CALL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit towards PA to 
 * inform that the remote party has been alerted. */

#define ICF_CC_PA_ALERT_USER_IND              ICF_REMOTE_USER_ALERTED_IND

/* This API indicates that Phone Application has alerted the user for an 
 * earlier informed incoming call. It also specifies whether the remote party 
 * should play the ringback tone locally or Phone Application will play 
 * the tone to the remote party. */

#define ICF_PA_CC_USER_ALERTED_REQ            ICF_LOCAL_USER_ALERTED_REQ

/* This API indicates that Phone Application has sent media_connect request 
 * earlier informed incoming call. It also specifies whether the remote party 
 * should play the ringback tone locally or Phone Application will play 
 * the tone to the remote party. */

#define ICF_PA_CC_MEDIA_CONNECT_REQ            ICF_MEDIA_CONNECT_REQ


/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone 
 * Application when remote SDP has been received and applied on the 
 * media session. This signifies that the media with remote party is 
 * connected. */

#define ICF_CC_PA_MEDIA_CONNECTED_IND         ICF_MEDIA_CONNECTED_IND

/* This API indicates that Remote party has accepted the call. This API 
 * is sent from SIP Packet Phone Signaling Toolkit to Phone Application
 * on getting call acception from remote party. */

#define ICF_CC_PA_CONNECT_IND                 ICF_CONNECT_IND


/* This API indicates that Remote party has accepted the call. This API 
 * is sent from SIP Packet Phone Signaling Toolkit to Phone Application
 * on getting connect ACK from remote party. */

#define ICF_CC_PA_CONNECT_RESP                 ICF_CONNECT_RESP


/* This API is sent from Phone Application to SIP Packet Phone Signaling 
 * Toolkit to inform that the user has accepted the incoming call. */

#define ICF_PA_CC_CONNECT_REQ                 ICF_CONNECT_REQ

/* This API is invoked by SIP Packet Phone Signaling Toolkit to Phone
 * Application to indicate that the call clearing procedure has to
 * be started at Phone Application too. */

#define ICF_CC_PA_TERMINATE_CALL_IND          ICF_TERMINATE_CALL_IND

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * in response to ICF_CC_PA_TERMINATE_CALL_IND to indicate that the
 * user has also cleared the call so that a new call can be presented to
 * the user. */

#define ICF_PA_CC_TERMINATE_CALL_CFM         ICF_LOCAL_USER_TERMINATE_CALL_RESP

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * to indicate that the call has to be cleared. */

#define ICF_PA_CC_TERMINATE_CALL_REQ          ICF_TERMINATE_CALL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * in response to ICF_PA_CC_TERMINATE_CALL_REQ to indicate that the
 * call has been cleared. */

#define ICF_CC_PA_TERMINATE_CALL_RESP         ICF_REMOTE_USER_TERMINATE_CALL_RESP

/* This API is used by SIP Packet Phone Signaling Toolkit to request 
 * Phone Application to display certain information to the phone user. 
 * It can also optionally specify a tone to be played. */
/*
#define ICF_CC_PA_DISPLAY_IND                 ICF_DISPLAY_IND
*/

/* This is a utility API that can be used by Phone Application to request 
 * SIP Packet Phone Signaling Toolkit to send data for all the calls 
 * received, dialed and missed. */

#define ICF_PA_CC_GET_CALL_LOG_REQ            ICF_GET_CALL_LOG_REQ

/* This API is sent in response to ICF_GET_CALL_LOG_REQ with the
 * log information of the calls received or dialled or missed. */

#define ICF_CC_PA_CALL_LOG_RESP               ICF_CALL_LOG_RESP

/* This API is used by the Phone Application to SIP Packet Phone 
 * Signaling Toolkit when it wants to put the current call on hold. After
 * putting the current call on hold the Phone Application may or maynot want
 * to initiate a new call. */

#define ICF_PA_CC_CALL_HOLD_REQ               ICF_CALL_HOLD_REQ

/* This API is sent in respone to ICF_CALL_HOLD_REQ to indicate 
 * that the SIP Packet Phone Signaling Toolkit was able to successfully
 * get the current call into held state. */

#define ICF_CC_PA_CALL_HOLD_RESP              ICF_CALL_HOLD_RESP

/* This API is used by the Phone Application to SIP Packet Phone 
 * Signaling Toolkit when it wants to resume an held call. */

#define ICF_PA_CC_CALL_RESUME_REQ             ICF_CALL_RESUME_REQ

/* This API is sent in respone to ICF_CALL_RESUME_REQ to indicate
 * that the SIP Packet Phone Signaling Toolkit was able to successfully
 * resume the held call. */

#define ICF_CC_PA_CALL_RESUME_RESP            ICF_CALL_RESUME_RESP

/* This API is sent from SIP Packet Phone Signaling Toolkit to 
 * Phone Application to indicate that the outgoing call initiated by the
 * user has undergone a call diversion and is now being forwarded to
 * a remote party. */

#define ICF_CC_PA_CALL_REDIRECTION_IND        ICF_CALL_REDIRECTION_IND

/* PA invokes this API toward SIP Packet Phone Signaling Toolkit when 
 * user give request to merge and ongoing call with a held call. */

#define ICF_PA_CC_CONFERENCE_REQ              ICF_CONFERENCE_REQ

/* This API is sent in respone to ICF_CONFERENCE_REQ to indicate
 * that the SIP Packet Phone Signaling Toolkit was able to successfully
 * merge the calls. */

#define ICF_CC_PA_CONFERENCE_RESP             ICF_CONFERENCE_RESP

/* PA invokes this API towards SIP Packet Phone Signaling Toolkit when 
 * user wants to transfer call an active call to another remote party
 * without talking to remote party. */

#define ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ ICF_CALL_TRANSFER_UNATTENDED_REQ

/* This API is sent in respone to ICF_CALL_TRANSFER_UNATTENDED_REQ
 * to indicate that the SIP Packet Phone Signaling Toolkit was able to
 * successfully tranfer the call. This indicates that the remote parties
 * are in conversation. */

#define ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP ICF_CALL_TRANSFER_UNATTENDED_RESP

/* PA invokes this API towards SIP Packet Phone Signaling Toolkit when
 * user wants to transfer call an active call to a remote party
 * which is held state. */

#define ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ  ICF_CALL_TRANSFER_ATTENDED_REQ

/* This API is sent in respone to ICF_CALL_TRANSFER_ATTENDED_REQ 
 * to indicate that the SIP Packet Phone Signaling Toolkit was able to
 * successfully tranfer the call. This indicates that the remote parties
 * are in conversation. */

#define ICF_CC_PA_CALL_TRANSFER_ATTENDED_RESP ICF_CALL_TRANSFER_ATTENDED_RESP

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the remote party has held the call. */

#define ICF_CC_PA_CALL_HOLD_IND               ICF_CALL_HOLD_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the remote party has resumed the earlier held call. */

#define ICF_CC_PA_CALL_RESUME_IND             ICF_CALL_RESUME_IND

#define ICF_PA_CC_CREATE_XCONF_REQ            ICF_CREATE_XCONF_REQ

#define ICF_CC_PA_CREATE_XCONF_RESP ICF_CREATE_XCONF_RESP

#define ICF_PA_CC_ADD_XCONF_PARTY_REQ         ICF_ADD_XCONF_PARTY_REQ

#define ICF_CC_PA_ADD_XCONF_PARTY_RESP        ICF_ADD_XCONF_PARTY_RESP

#define ICF_PA_CC_DELETE_XCONF_PARTY_REQ      ICF_DELETE_XCONF_PARTY_REQ

#define ICF_CC_PA_DELETE_XCONF_PARTY_RESP     ICF_DELETE_XCONF_PARTY_RESP

#define ICF_PA_CC_RELEASE_XCONF_REQ           ICF_RELEASE_XCONF_REQ 

#define ICF_CC_PA_RELEASE_XCONF_RESP          ICF_RELEASE_XCONF_RESP 

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the remote party has transferred the current call to
 * a remote party. */

#define ICF_CC_PA_REMOTE_CALL_TRANSFER_INITIATED_IND ICF_REMOTE_CALL_TRANSFER_INITIATED_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the calls have been successfully merged after the remote
 * party has initiated call transfer. */

#define ICF_CC_PA_CALL_MERGED_IND             ICF_CALL_MERGED_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the current call has the remote party replaced due to 
 * request from remote party. */

#define ICF_CC_PA_PARTY_REPLACED_IND          ICF_PARTY_REPLACED_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * in response to the ICF_INFO_REQ sent by PA */

#define ICF_CC_PA_INFO_RESP					 ICF_INFO_RESP

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * to inform that the call should be muted. */

#define ICF_PA_CC_CALL_MUTE_REQ               ICF_CALL_MUTE_REQ

#define ICF_CC_PA_DISPLAY_IND					ICF_DISPLAY_IND

#define ICF_PA_CC_CALL_UNMUTE_REQ             ICF_CALL_UNMUTE_REQ

#define ICF_PA_CC_INFO_REQ                    ICF_INFO_REQ


#define ICF_PA_CC_START_FAX_REQ                ICF_START_FAX_REQ

#define ICF_CC_PA_START_FAX_RESP                ICF_START_FAX_RESP

#define ICF_CC_PA_START_FAX_IND                 ICF_START_FAX_IND


#define ICF_CC_PA_INFO_IND		      ICF_INFO_IND

#define ICF_PA_CC_INFO_CFM                    ICF_INFO_CFM    

#define ICF_PA_API_END                        ICF_PA_API_END

#define ICF_DEPRECATED_API_START     ICF_MISC_API_START + 8
/*
 * The following are configuration APIs at ICF-CFG interface that have been
 * deprecated in ICF. These are not exposed outside and not used anymore
 */


 /* This API is sent by Configuration Module to toolkit to specify that 
  * the configuration is complete and the toolkit should start processing. 
  * After this API, toolkit shall do registration with configured registrar 
  * and make itself ready to receive/originate calls. */
#define ICF_CFG_CC_INIT_COMPLETE_REQ        ICF_DEPRECATED_API_START
/* This API is used to configure service related parameters like service 
 * enable/disable service, activate/deactivate service. service related 
 * timer values etc. This is also configured for each line. */

/*#define ICF_CFG_CC_SET_SERVICE_PARAMS_REQ   ICF_DEPRECATED_API_START + 1*/

/* This API is sent by Configuration module to clear all the ongoing 
 * calls and deallocate all the call-related buffers, anytime after the 
 * initial configuration is complete. However the configuration will be
 * retained and phone will not be deregistered. */
/*#define ICF_SM_CC_RESTART_REQ               ICF_DEPRECATED_API_START + 2*/

/* This API is used to delete the previous configuration of the SIP Packet
 * Phone Signaling Toolkit and bring it to idle state. All the ongoing calls
 * would be cleared, buffers for calls would be deallocated and SIP Packet 
 * Phone signaling Toolkit would deregister from Registrar. */
/*#define ICF_SM_CC_DELETE_CONFIG_PARAMS_REQ  ICF_DEPRECATED_API_START + 3*/

#define ICF_DEPRECATED_API_END              ICF_DEPRECATED_API_START + 3  

#if 0
#define ICF_MISC_CAUSES_START				ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG


/* Indicates that INFO request from PA cannot be handled because there is 
 * already one ongoing INFO request for which response is awaited from network*/
#define ICF_CAUSE_INFO_ALREADY_PENDING		ICF_MISC_CAUSES_START + 3

/* Indicates that INFO request send on network has been rejected */
#define ICF_CAUSE_INFO_REJECTED_BY_REMOTE	ICF_MISC_CAUSES_START + 4	

/* Indicates that timebased licence violation has been detected and the user is
 * not authorised to use the software */
#define ICF_CAUSE_LICENCE_VIOLATED			ICF_MISC_CAUSES_START + 5

#define ICF_CAUSE_SDP_MISSING_FOR_PRECONDITION	ICF_MISC_CAUSES_START + 6

#define ICF_CAUSE_PRECONDITION_PROV_WITHOUT_100REL ICF_MISC_CAUSES_START + 7

#define ICF_MISC_CAUSES_END					ICF_CAUSE_PRECONDITION_PROV_WITHOUT_100REL

#endif




#ifdef ICF_REL_OLD
/* these API mappings will be removed from the code. We shall no longer use
 * these APIs in the code and directly use the  APIs as defined in the 
 * icf_mm_defs.h */
/*---------------------------------------------------------------------------*/
/* Following are the APIs from SIP Packet Phone Signaling Toolkit to
 * Resource Manager and back. */

#define ICF_RM_API_START                      ICF_GET_CODEC_LIST_REQ

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards RM to 
 * get the list of codecs available for the new call and reserve them. 
 * It can be used without a list of remote codecs in which case the
 * RM needs to reserve all list of available codecs or it can be sent
 * with remote codecs in which case RM needs to reserve one - the best
 * possible - codec. */

#define ICF_CC_RM_GET_CODEC_LIST_REQ          ICF_GET_CODEC_LIST_REQ

/* This API is sent in response to ICF_CC_RM_GET_CODEC_LIST_REQ with
 * list of local codecs allocated for the current media session. */

#define ICF_RM_CC_GET_CODEC_LIST_RESP         ICF_GET_CODEC_LIST_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards 
 * Resource manager to commit one of the codec among the list of codecs
 * that were earlier reserved by RM. */

#define ICF_CC_RM_CODEC_COMMIT_REQ            ICF_CODEC_COMMIT_REQ

/* This API is sent in response to ICF_CC_RM_CODEC_COMMIT_REQ to 
 * inform that the codec has been committed and all other reserved codecs
 * have been unreserved. */

#define ICF_RM_CC_CODEC_COMMIT_RESP           ICF_CODEC_COMMIT_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Resource manager to release all reserved codecs for this media
 * session. */

#define ICF_CC_RM_REL_RES_REQ                 ICF_REL_RES_REQ

/* This API is sent in response to ICF_CC_RM_REL_RES_REQ to inform that the
 * codecs reserved for this media session have been deallocated. */

#define ICF_RM_CC_REL_RES_RESP                ICF_REL_RES_RESP

#define ICF_RM_API_END                        ICF_RM_CC_REL_RES_RESP

/* The following list of APIs list the APIs towards Media Manager and back
 * from Media Manager. */

#define ICF_MM_API_START                      ICF_CC_MM_API_START

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to create media session for a new call. The API may 
 * contain the local parameters for the media session like codec and 
 * the media direction. The API may also contain the remote media 
 * parameters like remote codec, remote RTP port and IP address etc.*/

#define ICF_CC_MM_CREATE_MEDIA_SESSION_REQ    ICF_CREATE_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_CREATE_MEDIA_SESSION_REQ
 * to inform that the media session has been created. It also informs
 * the list of local media parameters for this media session in this API. */

#define ICF_MM_CC_CREATE_MEDIA_SESSION_RESP   ICF_CREATE_MEDIA_SESSION_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to modify the media session parameters for a existing call. 
 * */

#define ICF_CC_MM_MODIFY_MEDIA_SESSION_REQ    ICF_MODIFY_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_MODIFY_MEDIA_SESSION_REQ
 * to inform that the media session has been modified.*/

#define ICF_MM_CC_MODIFY_MEDIA_SESSION_RESP   ICF_MODIFY_MEDIA_SESSION_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to delete an existing media session. */

#define ICF_CC_MM_DELETE_MEDIA_SESSION_REQ    ICF_DELETE_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_DELETE_MEDIA_SESSION_REQ
 * to inform that the media session has been deleted. */

#define ICF_MM_CC_DELETE_MEDIA_SESSION_RESP   ICF_DELETE_MEDIA_SESSION_RESP

/* This API is sent by Media Manager to SIP Packet Phone Signaling Toolkit 
 * to indicate any media failure and that the media session for an 
 * existing call has been deleted. This API has no response. */

#define ICF_MM_CC_DELETE_MEDIA_SESSION_IND    ICF_DELETE_MEDIA_SESSION_IND

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to merge two pre existing media sessions. The two media 
 * sessions are identified by their respective call id. */

#define ICF_CC_MM_MERGE_MEDIA_SESSION_REQ     ICF_MERGE_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_MERGE_MEDIA_SESSION_REQ
 * to indicate that the merging of the media sessions was successful. */

#define ICF_MM_CC_MERGE_MEDIA_SESSION_RESP    ICF_MERGE_MEDIA_SESSION_RESP

#define ICF_MM_CC_MEDIA_CAPABILITIES_RESP	ICF_MEDIA_CAPABILITIES_RESP

#define ICF_CC_MM_MEDIA_CAPABILITIES_REQ	ICF_MEDIA_CAPABILITIES_REQ

#define ICF_MM_API_END                        ICF_CC_MM_API_END

/* Module Mapping for PA, This is a temporary fix for MMI
   which uses the src and dest module-id for routing */
#define ICF_MODULE_PA              ICF_MODULE_APP

#endif

/**************************************************************************************************** 
*
*								New Mapping of FLAGS
*
*
******************************************************************************************************/

#define ICF_CFG_CC_CONFIG_CALL_THRU_SIP_PROXY		   ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY
#define ICF_CFG_CC_CONFIG_PRIVACY_HEADER_PRESENT		   ICF_CALL_CONFIG_PRIVACY_HEADER_PRESENT
#define ICF_CFG_CC_CONFIG_PRACK_REQD					   ICF_CALL_CONFIG_PRACK_REQD



/*******************************************************************************
 *
 * New mapping of data type icf_error_t. Error cause used in all response API's
 *
*******************************************************************************/

/* Undefined error cause */
#define ICF_CAUSE_UNDEFINED ICF_ERROR_UNDEFINED

/* Sent to PA. Specifies that the toolkit cannot accept a new call because 
 * number of calls have exceeded max_audio_calls configured */
#define ICF_CALL_CLEAR_REASON_MAX_AUDIO_CALLS_EXCEEDED ICF_CALL_CLEAR_REASON_MAX_CALLS_EXCEEDED

#define ICF_IMS_AUTH_OPTION			0
#define ICF_NONIMS_AUTH_OPTION		1


  
/*------------------------------------------------------------------------------
 * Description: Values for data type  icf_module_id_t
 *----------------------------------------------------------------------------*/
/* Dummy Module Id for auto activate and auto-register */
#define ICF_DUMMY_MODULE_ID        0xFE



/*------------------------------------------------------------------------------
 *
 *  icf_address_type_t values
 *
 *----------------------------------------------------------------------------*/

#define ICF_ADDRESS_PLAN_RESTRICTED     ICF_ADDRESS_PLAN_RESTRICTED_TO_BE_PRESENTED


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_req_stream_st
 *
 * Description: Denotes Media Stream and its associated parameters.
 *
 *---------------------------------------------------------------------------*/

#define ICF_LOCAL_CODEC_PRESENT		  ICF_CREATE_MEDIA_LOCAL_CODEC_PRESENT
#define ICF_REMOTE_SDP_PRESENT		  ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT                
#define ICF_REMOTE_TBCP_INFO_PRESENT	  ICF_CREATE_MEDIA_REMOTE_TBCP_INFO_PRESENT          
#define ICF_MEDIA_MODE_PRESENT	      ICF_CREATE_MEDIA_MODE_PRESENT                       
/*#define ICF_QOS_MARK_PRESENT			  ICF_CREATE_MEDIA_QOS_MARK_PRESENT      */

/* maximum number if lines in the configuration file */
/*#define ICF_CONF_MAX_LINES			50*//*SPR14489 */
#define ICF_CONF_MAX_LINES			60
/* maximum characters in a line in the configuration file */
#define ICF_CONF_MAX_LINE_SIZE		100
/* comment character in the configuration file */
#define ICF_CONF_COMMENT_CHAR		'#'

/* We need to send a unique call-id for Subscription to "Reg-Event"
 * For any outgoing transaction the call-id range is from 1-32767 
 * so we choose a value in the middle so that application might not
 * reach this value from either side
 */
#define ICF_REG_EVENT_SUBS_CALL_ID		16384 


/* For user database functions */
#define ICF_USER_DB_USE_PUBLIC_URIS       0x01 
#define ICF_USER_DB_USE_ASSOCIATED_URIS   0x02
#define ICF_USER_DB_USE_CONTACT_URIS      0x04
#define ICF_USER_DB_MATCH_ONLY_USER_NAME  0x08


/*-----------------------------------------------------------------------------
 * #defines for type of SDP
 *---------------------------------------------------------------------------*/
#define ICF_SDP_LOCAL			0x01
#define ICF_SDP_REMOTE			0x02
#define ICF_SDP_APPLIED			0x03
#define ICF_SDP_LAST_COMMITED	0x04



#ifdef ICF_REL_OLD
/* these mappings are no longer used as we shall not use these mapped API's. 
 * These API names without the module names will be used directly within the 
 * codec */ 
/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_create_media_session_req_st
 *
 * Description: Used to pass parameters for ICF_CREATE_MEDIA_SESSION_REQ
 *              API      
 *
 *---------------------------------------------------------------------------*/
typedef icf_create_media_session_req_st icf_cc_mm_create_media_session_req_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_create_media_session_resp_st
 *
 * Description: Used to pass parameters for 
 *              ICF_CREATE_MEDIA_SESSION_RESP API
 *
 *---------------------------------------------------------------------------*/
 typedef icf_create_media_session_resp_st icf_mm_cc_create_media_session_resp_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_modify_media_session_req_st
 *
 * Description: Used to pass parameters for 
 *              ICF_MODIFY_MEDIA_SESSION_REQ
 *
 *---------------------------------------------------------------------------*/
 typedef icf_modify_media_session_req_st icf_cc_mm_modify_media_session_req_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_merge_media_session_req_st
 *
 * Description: Used to pass parameters for ICF_MERGE_MEDIA_SESSION_REQ
 *
 *---------------------------------------------------------------------------*/
 typedef icf_merge_media_session_req_st  icf_cc_mm_merge_media_session_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_delete_media_session_ind_st 
 *
 * Description: Used to pass parameters for
 *              ICF_DELETE_MEDIA_SESSION_IND 
 *
 *---------------------------------------------------------------------------*/

typedef icf_delete_media_session_ind_st icf_mm_cc_delete_media_session_ind_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_media_capabilities_resp_st
 *
 * Description: Used to pass parameters for
 *              ICF_MEDIA_CAPABILITIES_RESP
 *
 *---------------------------------------------------------------------------*/

#define ICF_MM_CC_MEDIA_CAPABILITIES_RESP_VIDEO_SDP ICF_MEDIA_CAPABILITIES_RESP_VIDEO_SDP

typedef icf_media_capabilities_resp_st	icf_mm_cc_media_capabilities_resp_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_get_codec_list_req_st
 *
 * Description: Used to pass parameters for ICF_GET_CODEC_LIST_REQ API
 *
 *---------------------------------------------------------------------------*/

#define ICF_CC_RM_GET_CODEC_REMOTE_AUDIO_CODEC_LIST           ICF_GET_CODEC_REMOTE_AUDIO_CODEC_LIST
#define ICF_CC_RM_GET_CODEC_REMOTE_VIDEO_CODEC_LIST           ICF_GET_CODEC_REMOTE_VIDEO_CODEC_LIST

typedef icf_get_codec_list_req_st icf_cc_rm_get_codec_list_req_st;



/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_codec_list_resp_st
 *
 * Description: Used to pass parameters for ICF_GET_CODEC_LIST_RESP API
 *
 *---------------------------------------------------------------------------*/

#define ICF_RM_CC_CODEC_LIST_RESP_AUDIO_CODEC_LIST ICF_CODEC_LIST_RESP_AUDIO_CODEC_LIST
#define ICF_RM_CC_CODEC_LIST_RESP_VIDEO_CODEC_LIST ICF_CODEC_LIST_RESP_VIDEO_CODEC_LIST

typedef icf_get_codec_list_resp_st icf_rm_cc_get_codec_list_resp_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_codec_commit_req_st
 *
 * Description: Used to pass parameters for ICF_CODEC_COMMIT_REQ
 *
 *---------------------------------------------------------------------------*/

#define ICF_CC_RM_CODEC_COMMIT_VIDEO_CODEC ICF_CODEC_COMMIT_VIDEO_CODEC

typedef icf_codec_commit_req_st	 icf_cc_rm_codec_commit_req_st;
#endif


/*
 * Following is being used to define the size of the n/w callid-callObj map
 * array
 */
/*#define ICF_SSA_MAX_CALL_OBJ        (2* ICF_DEFAULT_MAX_CALLS)+ ICF_DEFAULT_MAX_REGISTRATIONS + 1*/
/*Fix for CSR 1-7407479*/
#define ICF_SSA_MAX_CALL_OBJ                1500
#define ICF_SSA_MAX_OPTIONS_CALL_OBJ        (2* ICF_MAX_NUM_OF_LINES)

/* setup timer will be started with this duration when ICF is sending call
 * modify indication to application. */

#define ICF_CC_PA_CALL_MODIFY_SETUP_TIMER		60000

#ifdef ICF_LOAD_STAT
/* Frequency of ststs printing in seconds*/
#define ICF_LOAD_STAT_TIMER    60
#endif

#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
#define ICF_CC_LOAD_CALL_DURATION_TIMER 90000
#endif 

#ifdef ICF_TRACE_ENABLE
#define ICF_CMN_MAX_API_LENGTH      5000
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_COMMON_DEFS_H__ */

    
