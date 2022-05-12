/*****************************************************************************
*
* FILE NAME    : icf_xconf_defs.h
*
* DESCRIPTION  : Contains the definitions used in XCONF service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 4-Jul-2007  Abhishek Dhammawat CDD Rel7.0    Created Origional
*
* Copyright 2007, Aricent .
*
*******************************************************************************/

#ifndef __ICF_XCONF_DEFS_H__
#define __ICF_XCONF_DEFS_H__
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
#endif /* __cplusplus */

#define ICF_XCONF_MAX_STATE           7
#define ICF_XCONF_MAX_EVENT          13

/*States in XCONF service*/
#define ICF_XCONF_STATE_INVALID              255
#define ICF_XCONF_00_IDLE                      0
#define ICF_XCONF_01_AWT_CONFERENCE_CONNECT    1
#define ICF_XCONF_02_AWT_ADD_PARTY_RESP        2
#define ICF_XCONF_03_AWT_DEL_PARTY_RESP        3
#define ICF_XCONF_04_CONFERENCE_CONNECTED      4
#define ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP  5
#define ICF_XCONF_06_AWT_CALL_RESUME_RESP      6

/*Events handled in XCONF service*/
#define ICF_EVENT_UNDEFINED                           255
#define ICF_XCONF_INITIATE_CONF_REQ                     0
#define ICF_XCONF_CONF_SERVER_CONNECTED                 1
#define ICF_XCONF_ADD_PARTY_REQ                         2
#define ICF_XCONF_ADD_PARTY_RESP                        3
#define ICF_XCONF_DELETE_PARTY_REQ                      4
#define ICF_XCONF_DELETE_PARTY_RESP                     5
#define ICF_XCONF_RELEASE_CONF_REQ                      6
#define ICF_XCONF_NOTIFY_FOR_SUBS                       7
#define ICF_XCONF_SUBS_RESP                             8
#define ICF_XCONF_CALL_RELEASED                         9
#define ICF_XCONF_FORCEFUL_CALL_CLEAR                  10
#define ICF_XCONF_INCOMING_CONF_REQ		               11
#define ICF_XCONF_CALL_RESUME_RESP                     12  

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /*  __ICF_XCONF_DEFS_H__ */

