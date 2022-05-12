/******************************************************************************
**
** FILE NAME: icf_feature_flags.h
**
*******************************************************************************
**
** DESCRIPTION: This file contains the FEATURE FLAGS of IMS_CLIENT FRAMEWORK. 
**
** DATE        NAME            REFERENCE    REASON
** ----        ----            ---------    ------
** 23-08-05   Ashutos Rath     ICF_LLD   Creating a common Feature flag
**                                          for enabling various feature of
**                                          IMS CLIENT FRAME WORK
** 04-10-05   Ashutos Rath     ICF_LLD   ICF_SIGCOMP_SUPPORT Flag Made On 
**                                       by default.
** 29-11-05	  Kirti Dhingra              ICF_QOS_SUPPORT Flag added and Made 
**        								 on by default. 
** 07-12-2005 Rohit Aggarwal   ICF 4.0   Flags for IPSec support
**                                     
** 27-Nov-2006 Deepti Goyal    ICF 6.2   ICF Nat feature support
**
** 22-Aug-2007 Deepti Goyal    ICF 6.5   A new flag for DNS Name Server configurtion
**
** 09-Feb-2008 Amit Sikka      SPR 18084 All Trace flag to be put under one Flag
**
** Copyright (c) 2007, Aricent.
******************************************************************************/

#ifndef _ICF_FEATURE_FLAGS_H_
#define _ICF_FEATURE_FLAGS_H_

/****************************************************************************/
/*****USER CONFIGURATION OF COMPILE TIME FEATURE FLAGS BEGINS HERE***********/
/****************************************************************************/
/*#define ICF_LOAD_STAT*/

/* Flag to determine whether the req-uri of Incoming INVITE needs to be parsed
 * to check if alternate address formats can be generated.
 */
#define ICF_REQ_URI_ALTERNATE_ADDR_PARSING 1

/*#define ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING 1*/

/* The approach in this flag has been discarded */
/* Flag to determine whether the req-uri of Incoming INVITE needs to be parsed
 * to check if alternate address formats can be generated.
 */

#define ICF_LOOPBACK_SUPPORT 1 

#define ICF_DNS_LOOKUP_ENABLED      1
#ifdef ICF_DNS_LOOKUP_ENABLED
/* Flag to determine that the DNS server address can be changed at runtime. 
 * So ICF will read the DNS server address for each DNS query
 */
#define ICF_RUNTIME_NAMESERVER_CONFIG  1
#endif

#define ICF_STAT_ENABLE            1

#ifndef ICF_LOAD_STAT
/* Flag for Enabling Traces */
/* Comment this Flag to Disable Trace */ 
/* #define ICF_TRACE_ENABLE             1 */
/* #define ICF_CLIB_TRACE_ENABLE   1 */
#ifdef ICF_TRACE_ENABLE

    /* Flag to get detailed function enter exit traces */
    #define ICF_DETAILED_DEBUG_TRACE 1 
    #define ICF_INTERFACE_TRACE_ENABLE 1
    #define ICF_DETAILED_DEBUG_FILE_TRACE 1
#endif

#endif

/* Flag for Enabling Error Reporting */
/* Comment this Flag to Disabel Error reporting*/
#define ICF_ERROR_ENABLE            1 

/* Flag to enabling Session timer compile time option*/
/* Comment this Flag to Disable Session Timer Feature */
#define ICF_SESSION_TIMER			1

/* Flag to enabling Voice call Services like Call Hold/Resume etc*/
/* Comment this Flag to Disable ICF Service Layer */
#define ICF_SERVICE_ENABLE          1

/* adding compile time flag for Licensing in ICF */
/* #define ICF_LICENSE_ENABLE          1 */

/*********************************************************************/
/* IMS specific Feature Flag: USER SHOULD NOT SWITCH ON THIS FLAG
 * IF ICF IS MEANT FOR IPphone client Framework*/
/*********************************************************************/
/* Flag for IMS Support*/
/*UnComment this Flag to Enable IMS Support on ICF*/ 
/*#define IMS_CLIENT                  1*/

/* Flag for NON-IMS Support*/
#define NON_IMS_CLIENT                 1 

/* Flag for ICF NETWORK ACTIVATION*/
/* TBD */
/*#define ICF_NW_ACTIVATE                  1*/

/*#define ICF_SECURE_SUPPORT              1*/

#define ICF_QOS_SUPPORT          1

#define ICF_QOS_STUBBED_OUT	     1

#ifdef IMS_CLIENT

/* Flag for enabling EmuzedMedia Manager, Make sure to disable MM Stubbing */
/*#define FSS_EMUZED_MEDIA_MANAGER			1*/

#ifdef FSS_EMUZED_MEDIA_MANAGER
/* Comment this flag to disable buffering of MM REQUEST API */
#define ICF_MM_REQUEST_BUFFERING       1
#endif 

/* Flag to Support SIGCOMP in IPPHONETK product */
/* UnComment this Flag to Enable SigComp Support */
/*#define ICF_SIGCOMP_SUPPORT          1*/

/* Flag to Support QoS in IPPHONETK product */
/* Comment this Flag to Disable QOS Support */
/* Qos would be supported if ICF_NETWORK_ACTIVATE is defined -
		check in icf_extif_map_feature_flags.h */

/* Flag to DEBUG MEMORY POOL STATISTICS */
#define ICF_DEBUG_MEMPOOL          		1 
/* Uncomment/Comment this two flags to Enable/disable Memory Pool Statistic */
#define ICF_PRINT_DEBUG_STATS			1

/* UnComment this Flag to Enable IPV6 support */
/*#define ICF_IPV6_ENABLED			1*/

/* Flag for enabling IP Security in ICF */
/* Comment this flag to disable IPSec support */
#define ICF_IPSEC_ENABLED               1

#define ICF_IPSEC_STUBBED_OUT	     1
/* Flag to determine whether underlying OS supports SA lifetime modification */
/* Comment this flag if the platform does not allow change in duration of an 
 * already existing SA.
 * This flag should be defined only if ICF_IPSEC_ENABLED is defined */
#define ICF_SA_LIFETIME_MOD_SUPPORT     1

#endif
/****************************************************************************/
/*****USER CONFIGURATION OF COMPILE TIME FEATURE FLAGS END HERE**************/
/*****USER SHOULD ABSTAIN FROM CONFIGURING COMPILE TIME FEATURE FLAGS *******/
/**********************BEYOND THIS POINT*************************************/
#define ICF_FAX_SUPPORT                  1

#define ICF_FAX_PASS_THROUGH		 1
/* Enable this flag for platform testing */
/*#define ICF_PLTFM_TEST_ENABLE			1*/

/* Enable this flag PCSCSF Discovery  */
/* #define ICF_P_CSCF_DISCOVERY_ENABLED		1 */
/* Enable this flag for NAT support in ICF */

/*#define ICF_NAT_TRAVERSAL     1 */

#ifdef ICF_NAT_TRAVERSAL

/* Enable this flag to support rport for NAT signalling.
   "rport" will be send in the Via Header of SIP request messages.
 */
#define ICF_NAT_RPORT_SUPPORT    1

/* Enable this flag to support the NAT traversal for media as well.
   Enabling this flag will allow the messages to be send/recv to/from NAT
   library.
 */
#define ICF_NAT_MEDIA_TRAVERSAL  1

/* Enable this flag to support the RTCP attribute as a line in SDP.
 */
#define ICF_RTCP_ATTR_SUPPORT    1

#endif

/*Enable this  flag to print the  ICF API*/

/*#define ICF_LOAD_DEBUG 1*/
/*#define ICF_LOAD_FORCEFUL_CALL_CLEAR 1*/


#if 1 /*Jason , syslog*/
/*support voip syslog */
#define VOIP_SYSLOG	1	
#endif

#endif
