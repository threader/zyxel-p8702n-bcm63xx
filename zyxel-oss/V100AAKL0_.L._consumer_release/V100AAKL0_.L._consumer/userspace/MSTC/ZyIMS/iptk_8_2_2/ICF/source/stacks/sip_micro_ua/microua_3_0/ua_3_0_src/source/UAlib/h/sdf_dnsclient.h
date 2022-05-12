/*************************************************************************
 *
 *  FILENAME        :   sdf_dnsclient.h
 *
 *  DESCRIPTION     :   This file contains the prototypes that are used for
 *                      porting DNS CLIENT.
 *
 *  Revision History 
 *  DATE          NAME            REFERENCE            REASON
 *  03-Mar-2009   Rajiv Kumar     IPTK Rel 8.1         Initial version.
 *************************************************************************/

#ifndef __SDF_DNS_CLIENT_H__
#define __SDF_DNS_CLIENT_H__

#include "arDnsClient_types.h"
#include "arDnsClient_defs.h"
#include "arDnsClient_api.h"

#if defined(ARDNSCL_PORT_LINUX) || defined(ARDNSCL_PORT_VXWORKS)
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
    
typedef enum
{
	/* ------------------------------------------------------------------
	 DESCRIPTION:	Enum defining the return values.
	 
	 Sdf_co_dns_success	-	Return value indicating success.
	 Sdf_co_dns_fail		-	Return value indicating failure.
	 -----------------------------------------------------------------*/

	Sdf_co_dns_fail = 0,
	Sdf_co_dns_success = 1
} Sdf_ty_dnsRetVal;

/*****************************************************************************
 ** FUNCTION:    sdf_fn_uaProcessQueryRespFromDnsClient 
 ** DESCRIPTION: The function is invoked by porting layer of UATK for the 
 **              processing of query result from DNS Client.This function will
 **              replace the function Ares_cbk_uaQueryResult in UATK module.
 ** 
 *****************************************************************************/
void sdf_fn_uaProcessQueryRespFromDnsClient(
        IN arDnsClient_response_st *pResponse);
/******************************************************************************
*
*  FUNCTION NAME  :   	sdf_fn_uaProcessDnsQueryResp
*
*  DESCRIPTION    :   	This function is used to process the DNS query
*                       response from DNS client.In this function we will read
*                       the buffer from fd and call the unpack routine of DNS
*                       client for unpacking.After unpacking is successful,a
*                       function defined in porting layer of UATK is call to
*                       further process the response.
*
*  RETURNS        :   	Success : If desired options are performed.
*                       Failure : If desired options can not be performed.
*
 ******************************************************************************/
Sdf_ty_dnsRetVal sdf_fn_uaRecvDnsQueryResp(void);   

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __SDF_DNS_CLIENT_H__ */    
