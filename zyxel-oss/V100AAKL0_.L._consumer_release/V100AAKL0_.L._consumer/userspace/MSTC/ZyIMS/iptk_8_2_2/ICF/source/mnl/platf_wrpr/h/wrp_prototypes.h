/*------------------------------------------------------------------------------

 *

 * File name        : wrp_prototypes.h

 *

 * Purpose          : This file contains the prototypes of the functions of 

 *						Platform wrapper

 *

 * Revision History :

 *

 * Date         Name            Ref#            Comments

 * --------     ------------    ----            ---------------

 * 10-Aug-2005  Irfan Khan						Initial

 *

 *

 * Copyright (c) 2006, Aricent.

 *----------------------------------------------------------------------------*/



#ifndef __WRP_PRTOTYPES_H_

#define __WRP_PRTOTYPES_H_





/*INCLUDES*/
#ifdef __SYMBIAN32__
#include "hssutilityhandle.h"
#include "gprsconnector.h"
#endif

#include "wrp_types.h"
#include "icf_port_pltfm_types.h"



#ifdef __SYMBIAN32__

#include <e32base.h>
#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */

#else
#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */

#define IMPORT_C

#define GLDEF_C

#endif

#ifdef __SYMBIAN32__

IMPORT_C GLDEF_C icf_return_t	icf_port_pltfm_getGprsConnectorfromWrp(\
                                                CGPRSConnector **aGprsConnectorPtr);
#endif

/** 

* icf_port_wrpr_init

* Initializes the wrapper.

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @return The status of request completion

*/

IMPORT_C GLDEF_C icf_return_t	icf_port_pltfm_init(
									icf_msg_st					*p_port_req,
									icf_port_pltfm_ret_val_t	*p_port_ret_val,
									icf_msg_st					* p_port_resp,
									icf_return_t (*aRsporIndCallBack)(icf_msg_st *pMsg));

/** 

* icf_port_wrpr_deinit

* De Initializes the wrapper.

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @return The status of request completion

*/

IMPORT_C GLDEF_C void icf_port_pltfm_deinit(void);





/** 

* mimGetDeviceParameters

* Fetches the Device parametes from the platform.

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @param aDevParam Request-Response structure.

* @return The status of request completion

*/

IMPORT_C GLDEF_C  icf_return_t	icf_port_pltfm_get_device_params (
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);




/** 

* mimRunAka

* Runs the Authentication and Key Agreement procedure and sets the result .

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @param aClngRes Request-Response structure to run the AKA.

* @return The status of request completion

*/

IMPORT_C GLDEF_C icf_return_t
				icf_port_pltfm_run_aka_procedure (\
								icf_msg_st					*p_port_req,
								icf_port_pltfm_ret_val_t	*p_port_ret_val,
								icf_msg_st					*p_port_resp);



/** 

* mimGetAccNwInfo

* Gets the P-Network Access information(MCC,MNC,Cell id etc)from the platform.

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @param aAccsNw Request-Response structure.

* @return The status of request completion

*/

IMPORT_C GLDEF_C icf_return_t icf_port_pltfm_get_access_nw_info(\
				 icf_msg_st* p_port_req,
				 icf_port_pltfm_ret_val_t	*p_port_ret_val,
				 icf_msg_st* p_port_resp);







#ifdef ICF_NW_ACTIVATE
/** 

* smcPdpActivate

* Activates the PDP context as requested(Primary and Secondary).

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @param aPdpActReq Request-Response structure for PDP Activation.

* @return The status of request completion

*/

IMPORT_C GLDEF_C icf_return_t icf_port_pltfm_pdp_activate_req (
							icf_msg_st					*p_port_req,
							icf_port_pltfm_ret_val_t	*p_port_ret_val,
							icf_msg_st					*p_port_resp);


/** 

* smcPdpDeactivate

* Deactivates the PDP context as requested(Primary and Secondary).

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @param aPdpList Request-Response structure for PDP Deactivation.

* @return The status of request completion

*/

IMPORT_C GLDEF_C icf_return_t	icf_port_pltfm_pdp_deactivate_req (
				icf_msg_st* p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st* p_port_resp);
#endif

#ifdef ICF_QOS_SUPPORT
IMPORT_C GLDEF_C icf_return_t icf_port_pltfm_qos_reserve_req_non_ims (
							icf_msg_st					*p_port_req,
							icf_port_pltfm_ret_val_t	*p_port_ret_val,
							icf_msg_st					*p_port_resp);


IMPORT_C GLDEF_C icf_return_t	icf_port_pltfm_qos_release_req_non_ims (
				icf_msg_st* p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st* p_port_resp);
#endif

/** 

* smcPdpAddrGet

* Gets the IP address associated with the PDP context.

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @param aPdpAddrReq Request-Response structure.

* @return The status of request completion

*/

IMPORT_C GLDEF_C icf_return_t				icf_port_wrpr_pdp_addr_get(
											icf_string_st * p_pdp_addr);






/** 

* nqiPcscfAddrGet

* Gets the P-CSCF address through DHCP request. 

* @param aWrpGlbCntxt Pointer to the global structure of wrapper.

* @param aDhcpReq Request-Response structure for DHCP.

* @return The status of request completion

*/

IMPORT_C GLDEF_C icf_return_t	icf_port_wrpr_pcscf_addr_get(
								icf_string_st * p_pcscf_addr);
IMPORT_C GLDEF_C icf_return_t icf_port_pltfm_init_send(icf_return_t (*asmcWrprSend)(void *msg));
/* ************************************************************************** */
/* IP Security related functions start */
/* ************************************************************************** */


IMPORT_C GLDEF_C void freeIPSecEncAlgo(void* req_pld);
IMPORT_C GLDEF_C void freeIPSecAuthAlgo(void* req_pld);

/* Function invoked by ICF to fetch the list of locally supported 
 * encryption algorithms
 */


IMPORT_C GLDEF_C icf_return_t
				icf_port_pltfm_fetch_enc_algo_req (
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);

/* Function invoked by ICF to fetch the list of locally supported 
 * authentication algorithms
 */
IMPORT_C GLDEF_C icf_return_t
				icf_port_pltfm_fetch_auth_algo_req (
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);

#ifdef ICF_IPSEC_ENABLED

IMPORT_C GLDEF_C  icf_return_t	icf_port_pltfm_sa_create_req(
				icf_msg_st					*p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st					* p_port_resp);


IMPORT_C GLDEF_C icf_return_t	icf_port_pltfm_sa_delete_req(
				icf_msg_st					*p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st					* p_port_resp);

IMPORT_C GLDEF_C  icf_return_t icf_port_pltfm_sa_modify_req(
				icf_msg_st					*p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st					* p_port_resp);
#endif/*#ifdef ICF_IPSEC_ENABLED*/

IMPORT_C GLDEF_C  icf_return_t icf_port_pltfm_get_host_ip(
												icf_msg_st					*p_port_req,
												icf_port_pltfm_ret_val_t		*p_port_ret_val,
												icf_msg_st					*p_port_resp);

IMPORT_C GLDEF_C icf_return_t icf_port_pltfm_get_host_ip_on_symbian(
											   icf_msg_st	*p_port_req,
											   icf_msg_st	*p_port_resp);


IMPORT_C GLDEF_C  icf_return_t	icf_port_pltfm_pcscf_addr_get(
								icf_string_st * p_pcscf_addr);
icf_void_t icf_port_pltfrm_get_pdp_addresses(
                icf_port_pltfm_pdp_def_resp_st *p_pdp_def_resp);


#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
IMPORT_C GLDEF_C icf_return_t icf_port_pltfm_dhcp_query(
		icf_transport_list_st *p_dhcp_addr,
		icf_transport_list_st *p_dns_addr,
		icf_transport_list_st *p_pcscf_addr);
#endif

IMPORT_C GLDEF_C icf_return_t	icf_port_pltfm_get_param_string_from_wrapper(
				icf_string_st *p_param,
				icf_uint8_t    param);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_wrpr_memget
*
*  DESCRIPTION    :   This function is called to allocate buffers in the system.
*  Arguments      :   size      - Size of memory buffer required
*                     p_eocde   - pointer to variable to return error code
*
*  RETURNS        :   Pointer to the allocated buffer if there is available 
*                     memory in the system, else ICF_NULL with ecode as 
*                     ICF_ERROR_MEM_ALLOC_FAILURE.
*
 *************************************************************************/
icf_void_t* 
icf_wrpr_memget(icf_uint32_t size, icf_error_t *p_ecode);

/*************************************************************************
*
*  FUNCTION NAME  :   icf_wrpr_memfree
*
*  DESCRIPTION    :   This function is called to deallocate buffers.
*  RETURNS        :   void.
*
 *************************************************************************/
icf_return_t
icf_wrpr_memfree(icf_void_t* p_buf);

#ifdef ICF_IPSEC_ENABLED
#if 0 
/* Function invoked by ICF to create SA for encryption */
IMPORT_C GLDEF_C IPSecRetVal_et	ipsecCreateHtHEnc(
	void				*aWrGlbCntxt,
	IPSecSaCreateEncReq_st		*pSaCreateEncReq);

/* Function invoked by ICF to create SA for authentication */
IMPORT_C GLDEF_C IPSecRetVal_et	ipsecCreateHtHAuth(
	void				*aWrGlbCntxt,
	IPSecSaCreateAuthReq_st		*pSaCreateAuthReq);

#endif
#endif/*#ifdef ICF_IPSEC_ENABLED*/

/*#ifndef __SYMBIAN32__*/
#ifdef __cplusplus
}
#endif /* __cplusplus */
/*#endif*/


#endif /*__WRP_PRTOTYPES_H_*/



