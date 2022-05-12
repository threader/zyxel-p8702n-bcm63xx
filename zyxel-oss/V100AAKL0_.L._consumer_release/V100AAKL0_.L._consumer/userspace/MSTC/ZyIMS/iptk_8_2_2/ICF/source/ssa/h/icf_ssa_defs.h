/*************************************************************************
 *
 *     FILENAME           :  icf_ssa_defs.h
 *
 *     DESCRIPTION        : 
 *
 *     Revision History   :
 *     DATE        NAME                REFERENCE            REASON
 *     ----------------------------------------------------------
 *     22/01/05    Aashish Suchdev                       Initial 
 *     27/01/05    Aashish Suchdev                       Macro for reset
 *                                                       of callid-ssa ctxt
 *                                                       map
 *     02/06/05    Jalaj Negi           IMS CLIENT       Addded #define for
 *                                                       IMS CLIENT
 *     07-JUL-2005 Aashish Suchdev                       TCP checkin
 *     06-Mar-2006 Amit Sharma                           ICF merging with ICF2.0
 *     23-Mar-2006 Amit Sharma                           Code merged with ICF 4.2
 *     05-JAN-2007 Krishna				                 code added for DNS Buffering
 *							i         ICF Rel 6.3 
 *	   17-Jan-2008 Shruti Thakral                        Enhancements for rel8.0
 *     27-Jan-2008 Abhishek Dhammawat Rel8.0             precondition additions 
 *     07-Mar-2007 Abhishek Dhammawat Rel8.0             change value of 
 *                                                       ICF_SSA_NUM_OF_EXTN_SUPP
 *                                                        to 3 for 
 *                                                       precondition addition
 *                                                       to supported list.
 *     21-Nov-2008 Rajiv Kumar                           Rel 7.1.2 SPR merging
 *                                                       (SPR 18988) 
 *     02-Dec-2008 Tarun Gupta        SPR 19218          Merged CSR 1-6457307
 *     13-Mar-2009 Rajiv Kumar        IPTK Rel8.1        Merging CSR-1-6452321
 *     27-May-2009 Anuradha Gupta     SPR 19672          Merging CSR-1-7009929
 *
 *     Copyright 2009, Aricent.
 *
 *************************************************************************/

#ifndef __ICF_SSA_DEFS_H__
#define __ICF_SSA_DEFS_H__

/* Maximum number of remote servers (proxy/registrar) tried for sending 
 * request on ntwk */
#define ICF_SSA_MAX_REM_SERVERS_TRIED   2
#define ICF_SSA_MAX_DNS_SERVERS_TRIED   2
#define ICF_SSA_MAX_FQDN_ENTRIES        2

#define ICF_SSA_DEFAULT_SIP_PORT_NUM    5060
#define ICF_SSA_NO_HOLD                 0x01
#define ICF_SSA_HOLD_MEDIA_CLINE_ZERO   0x02
#define ICF_SSA_HOLD_MODE_SENDONLY      0x04
#define ICF_SSA_HOLD_PORT_ZERO          0x08
#define ICF_SSA_OPTIONS_MODE_INACT      0x10
/*CSR_1-7009929 Fixed*/
#define ICF_SSA_HOLD_SESSION_CLINE_ZERO 0x20

#ifdef ICF_SESSION_TIMER
#define ICF_SSA_REFRESHER_METHOD_NONE	0
#define ICF_SSA_REFRESHER_METHOD_INVITE	1
#define ICF_SSA_REFRESHER_METHOD_UPDATE	2
#endif 

#define ICF_MAX_SIZE_INT_USAGE_STR   10
#define ICF_MAX_SIZE_MEDIUM_STR		100

#define ICF_SSA_MAX_SIZE_SUPP_EXTN   4 
#define ICF_SSA_MAX_SIZE_SHORT_STR   25
/*
 * Following hash define controls the max number of pointers
 * which the SSA can free on return path ( after invocation of entry
 * pt towards CC/CM etec.) This is the list of memory chunk pointers
 * which were allocated in callbacks and need to be released by SSA only
 */
#define ICF_SSA_MAX_MMRY_CHUNK_LST   10
/*
 * Following is being used to define the size of the n/w callid-callObj map
 * array
 */
/*#define ICF_SSA_MAX_CALL_OBJ         10 - moved in common/h */

/* Maximum ICF_SSA_IP address length*/

#define ICF_SSA_IP_MAX_LEN           (15)

/* Minimum IP address length*/

#define ICF_SSA_IP_MIN_LEN           (7)

/* No more than 3 dots "." in an IP Address*/

#define ICF_SSA_IP_MAX_DOTS          (3)  

/* Maximum value of an octet*/

#define ICF_SSA_IP_MAX_RANGE         (255)  

/* Minimum value of an octet*/

#define ICF_SSA_IP_MIN_RANGE         (0)  

/* Four octets per IP address*/

#define ICF_SSA_IP_MAX_OCTETS        (4)	

/* Three digits per octet*/

#define ICF_SSA_IP_MAX_OCTET_DIGITS  (3)    

/*FOllowing are the types of memory allocated by SSA
 * which SSA is supposed to free on return path from 
 * other internal module invocations.  This is required as
 * some memory may be allocated from DBM (like SDP) while
 * some we may get straight from Genlib pools. Based on that
 * the routine which is to be invoked for freeing will change
 */
#define ICF_SSA_MEM_TYPE_DBM_SDP                0x01
#define ICF_SSA_MEM_TYPE_GL_CMN                 0x02
#define ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST    0x04


/* Index of REGISTER method in Allow header of Register request */
#define ICF_SSA_REG_INDEX_IN_ALLOW_HDR   (11)

/* Max number of Supported header options stored in user profile */
#define ICF_SSA_NUM_OF_EXTN_SUPP         (3)

#ifdef ICF_QOS_SUPPORT
/* Maximum number of lines which can be present in SDP for Qos support */
#define ICF_MAX_NUM_QOS_LINES            7
/* Maximum length of a Qos line in SDP */
#define ICF_MAX_LENGTH_OF_QOS_LINE       40
/* Max number of bandwidth lines */
#define ICF_MAX_NUM_BW_LINES             3
/* Maximum length of a Bandwidth line in SDP */
#define ICF_MAX_LENGTH_OF_BW_LINE        40
#endif

#ifdef IMS_CLIENT
#define ICF_SSA_SUBSCRIPTION_TIMER_VAL  600000

#endif

/* Max no of methods allowed */
#define ICF_SSA_MAX_METHODS_ALLOWED      (10)

/*
 * Hash defines for TCPM
 */
/*TCP Manager states*/
/*Initial State*/
#define ICF_SSA_TCPM_IDLE                0
/*A connect has been issued, response is awaited
 * the outgoing messages will be buffered by TCPM*/
#define ICF_SSA_TCPM_AWT_CON_RESP        1
/*Connect completed */
#define ICF_SSA_TCPM_CONNECTED           2
/*For incoming connection, a message was received from 
 * peer in last PURGE TIMER seconds*/
#define ICF_SSA_TCPM_INUSE               3
/*For incoming connection, a message has not been
 * received from peer in last PURGE TIMER seconds*/
#define ICF_SSA_TCPM_NOT_INUSE           4
#define ICF_SSA_TCPM_MAX_STATES          ICF_SSA_TCPM_NOT_INUSE + 1

/*Events from SSA to ES*/
#define ICF_SSA_TCPM_OPEN_CONN_REQ       0
#define ICF_SSA_TCPM_CLOSE_CONN_REQ      1
#define ICF_SSA_TCPM_WRITE_TO_CONN       2

/*Events from ES to SSA*/
#define ICF_SSA_TCPM_SUCC_OPEN_CONN_RESP 3
#define ICF_SSA_TCPM_FAIL_OPEN_CONN_RESP 4
#define ICF_SSA_TCPM_PURGE_TIMER_XPRY    5
#define ICF_SSA_TCPM_CLOSE_CONN_IND      6
#define ICF_SSA_TCPM_OPEN_CONN_IND       7
#define ICF_SSA_TCPM_DATA_IND            8
#define ICF_SSA_TCPM_REPLACE_CONN_FD     9
#define ICF_SSA_TCPM_MAX_EVENTS      ICF_SSA_TCPM_DATA_IND + 9

/* Max length of Security-Client header */
#define ICF_MAX_SEC_HDR_LEN              500

#define ICF_NUM_APP_PROFILE_HDRS 5 

#define ICF_START_SDP_O_VERSION         (786)
/*Define INCOMING INVITE MESSAGE IDENTIFIER */
#define ICF_SSA_INCOMING_INVITE             1
#define ICF_SSA_RECVD_ANSWER_TO_OFFER       2

/* Maximum size of an IP address */
#define ICF_SSA_DNS_MAX_IP_ADDR_SIZE                16

/*Following are for the Tracing of DNS type */
#define ICF_DNS_SRV_QUERY             1
#define ICF_DNS_AAA_QUERY	      2

/*  Reason String for Responses */
#define ICF_REASON_STRING_FOR_481_SUBS        "Subscription does not exist"

/* Fix for CSR 1-7009929: SPR 19594 */
/* Macro that specifies whether sendrecv attribute needs to be explicitly 
 * specified in SDP.
 * If the value is 1, a=sendrecv line will be inserted in SDP when the media 
 * mode is sendrecv. If the value is not 1, then a=sendrecv line will not be 
 * explicitly inserted in SDP.
 */
#define ICF_SSA_EXPLICIT_SENDRECV		1

#endif /* __ICF_SSA_DEFS_H__ */
