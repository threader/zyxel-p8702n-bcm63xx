/*************************************************************************
 *
 *  FILENAME        :   icf_port_dns_client.h
 *
 *  DESCRIPTION     :   This file contains the prototypes that are used for
 *                      porting DNS CLIENT.
 *
 *  Revision History 
 *  DATE          NAME            REFERENCE            REASON
 *  03-Mar-2009   Rajiv Kumar     IPTK Rel 8.1         Initial version.
 *************************************************************************/

#ifndef __ICF_PORT_DNS_CLIENT_H__
#define __ICF_PORT_DNS_CLIENT_H__

#include "arDnsClient_types.h"
#include "arDnsClient_defs.h"
#include "arDnsClient_api.h"

#ifdef ARDNSCL_PORT_LINUX
#include "arDnsClient_port_types.h"
#include "arDnsClient_port_defs.h"
#include "arDnsClient_port_prototypes.h"
#endif

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */

/*****************************************************************************
 ** FUNCTION:    icf_process_queryresp_from_dnsclient 
 ** DESCRIPTION: The function is invoked by ES of IPTK for the processing of
 **              query result from DNS Client.This function will replace the
 **              function Ares_cbk_uaQueryResult in UATK module.
 ** 
 *****************************************************************************/
void icf_process_queryresp_from_dnsclient(
        IN arDnsClient_response_st *pResponse);
    
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_PORT_DNS_CLIENT_H__ */    
