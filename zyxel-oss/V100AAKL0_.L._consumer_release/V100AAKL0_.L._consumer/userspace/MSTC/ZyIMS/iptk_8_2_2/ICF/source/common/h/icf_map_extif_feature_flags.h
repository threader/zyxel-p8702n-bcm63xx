/******************************************************************************
**
** FILE NAME: icf_icf_map_feature_flags.h
**
*******************************************************************************
**
** DESCRIPTION: This file contains mapping of the FEATURE FLAGS 
*               From ICF feature flag to some internal flags
**
** DATE        NAME            REFERENCE    REASON
** ----        ----            ---------    ------
** 23-08-05   Ashutos Rath     ICF_LLD   Creating a common Feature flag
**                                          for enabling various feature of
**                                          IP Phone Toolkit 
** 29-11-05   Kirti Dhingra                 Addition of feature Flag for QOS 
											support 
** 13-01-06   Aman Arora		IPSEC		Added a feature flag to loop back 
**
** Copyright (c) 2006, Aricent.
******************************************************************************/

#ifndef _ICF_ICF_MAP_FEATURE_FLAGS_H_
#define _ICF_ICF_MAP_FEATURE_FLAGS_H_

#include "icf_feature_flags.h"
/****************************************************************************/
/*****USER CONFIGURATION OF COMPILE TIME FEATURE FLAGS BEGINS HERE***********/
/****************************************************************************/

/* Flag for Enabling Traces */
#ifdef ICF_TRACE_ENABLE
#ifndef ICF_PORT_WINCE   /* WE will switch off the Logs for Wince */


#ifndef ICF_PORT_SYMBIAN  /* We will not switch off the Stack trace for symbian*/
/* Below defines are for FSS SIPUATK Treaces, Only required to switch on
 * for debugging. Dont switch on the SDF trace as it takes array of size 200
 * in all functions */
 /* #define SDF_TRACE                      1 */
#endif /*ICF_PORT_SYMBIAN*/

#endif /*ICF_PORT_WINCE*/

#endif /*ICF_TRACE_ENABLE*/




/* Flag for Enabling Error Reporting */
#ifdef ICF_ERROR_ENABLE 

#if !defined(ICF_PORT_WINCE) && !defined(ICF_PORT_SYMBIAN)

#define SDF_ERROR		        1
#define SIP_ERROR

#endif /*ICF_PORT_WINCE*/

#endif /*ICF_ERROR_ENABLE*/


#ifdef ICF_IPV6_ENABLED
#define SDF_IPV6                    1
#endif

/* Flag to Support ICF SERVICE */
#ifdef ICF_SERVICE_ENABLE
/*Flag for enabling CHR Service*/
#define ICF_CHR_SERVICE		1

/*Flag for enabling TWC Service*/
#define ICF_TWC_SERVICE		1

/*Flag for enabling CTU Service*/
#define ICF_CTU_SERVICE		1

/*Flag for enabling CTA Service*/
#define ICF_CTA_SERVICE		1

/*Flag for enabling RNW Service*/
#define ICF_RNW_SERVICE		1

/*Flag for enabling CW Service*/
#define ICF_CW_SERVICE		1

/*Flag for enabling XCONF Service*/
#define ICF_XCONF_SERVICE   1

#endif

/****************************************************************************/
/*****USER CONFIGURATION OF COMPILE TIME FEATURE FLAGS END HERE**************/
/*****USER SHOULD ABSTAIN FROM CONFIGURING COMPILE TIME FEATURE FLAGS *******/
/**********************BEYOND THIS POINT*************************************/

/* Flag to omit the functions that are abandoned in ICF */
/* Comment this flag for standard ICF releases */
#define ICF_FEATURE_ABANDON            1

/* Flag to enable debug traces for function/module enter/exit/reset */
/* Comment this flag for disabling these traces */
/* #define ICF_DETAILED_DEBUG_TRACE                   1 */

/* Flag to enable/disable printing of memory debug statistics */
#ifdef ICF_PRINT_DEBUG_STATS
#define IPPSTK_PRINT_DEBUG_STATS        1
#endif

/*Flag for enabling  TWC Service*/
#ifdef ICF_TWC_SERVICE
#define ICF_CHR_SERVICE 1
#define ICF_TWC_SERVICE 1
#endif

/*Flag for enabling CTU Service*/
#ifdef ICF_CTU_SERVICE
#define ICF_CHR_SERVICE 1
#define ICF_CTU_SERVICE 1
#endif

/*Flag for enabling CTA Service*/
#ifdef ICF_CTA_SERVICE
#define ICF_CHR_SERVICE 1
#define ICF_CTA_SERVICE 1
#endif

/*Flag for enabling RNW Service*/
#ifdef ICF_RNW_SERVICE
#define ICF_CHR_SERVICE 1
#define ICF_RNW_SERVICE 1
#endif

/*Flag for enabling CW Service*/
#ifdef ICF_CW_SERVICE
#define ICF_CHR_SERVICE 1
#define ICF_CW_SERVICE 1
#endif

/*Flag for enabling XCONF Service*/
#ifdef ICF_XCONF_SERVICE
#define ICF_CHR_SERVICE 1
#define ICF_CTA_SERVICE 1
#define ICF_CTU_SERVICE 1
#endif

#endif
