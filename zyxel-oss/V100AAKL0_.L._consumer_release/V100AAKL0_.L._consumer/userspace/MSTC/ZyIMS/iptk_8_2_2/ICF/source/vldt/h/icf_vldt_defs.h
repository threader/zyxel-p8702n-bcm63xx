/******************************************************************************
*
* FILE NAME    : icf_vldt_defs.h
*
* DESCRIPTION  : Contains the max and min values defined for val;idation code
*
* Revision History :
*
* Date         Name              Ref#            Comments
* --------     ------------      ----            ---------------
* 17-Jun-2009  Tarun Gupta       SPR 19742       Added a string comparision 
*                                                macro
* 08-Oct-2009 Abhishek Dhammawat SPR 20063       Merged CSR 1-7718121
* Copyright (c) 2009, Aricent.
*
*******************************************************************************/
#ifndef __ICF_VLDT_DEFS_H__
#define __ICF_VLDT_DEFS_H__

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
 */

#ifdef __cplusplus
/* Yes, C++ compiler is present.
* Use standard C.
* 
*/

extern "C"
{
 #endif /* __cplusplus */



/* NEW MAX MIN VALUES DEFINED FOR VALIDATION CODE */
#define ICF_MIN_ADDR_TYPE                       ICF_ADDRESS_PLAN_SIP_URI
#define ICF_MAX_ADDR_TYPE                       ICF_ADDRESS_PLAN_SIPS_URI
#define ICF_MIN_PORT_VALUE                      1024
#define ICF_MAX_PORT_VALUE                      65535
#define ICF_MAX_OTG_CALL_ID_VALUE               32767
#define ICF_MIN_CONFIG_ACTION_VALUE             ICF_CONFIG_ACTION_ADD
#define ICF_MAX_CONFIG_ACTION_VALUE             ICF_CONFIG_ACTION_DELETE
#define ICF_MIN_TAG_TYPE                        ICF_TAG_TYPE_SIP_URI_PARAM
#define ICF_MAX_TAG_TYPE                        ICF_TAG_TYPE_HDR_PARAM
#define ICF_MIN_TRANSPORT_ADDRESS               ICF_TRANSPORT_ADDRESS_DOMAIN_NAME
#define ICF_MAX_TRANSPORT_ADDRESS               ICF_TRANSPORT_ADDRESS_IPV6_ADDR
#define ICF_MIN_ENCODING_QUALITY                0
#define ICF_MAX_ENCODING_QUALITY                10
#define ICF_MAX_TCP_TYPE                        ICF_TCP_SERVER
#define ICF_MIN_TCP_TYPE                        ICF_TCP_CLIENT
#define ICF_MIN_BIT_RATE                        ICF_T38_2400_BIT_PER_SEC
#define ICF_MAX_BIT_RATE                        ICF_T38_33600_BIT_PER_SEC
#define ICF_MIN_FAX_VERSION                     ICF_T38_1998
#define ICF_MAX_FAX_VERSION                     ICF_T38_2002
#define ICF_MIN_RATE_MANAGEMENT                 ICF_T38_RATE_MANAGEMENT_LOCAL
#define ICF_MAX_RATE_MANAGEMENT                 ICF_T38_RATE_MANAGEMENT_TRANSFERRED
#define ICF_MIN_STREAM_TYPE                     ICF_STREAM_TYPE_AUDIO
#define ICF_MAX_STREAM_TYPE                     ICF_STREAM_TYPE_FAX
#define ICF_MIN_TRANSPORT_TYPE                  ICF_TRANSPORT_TYPE_UDP
#define ICF_MAX_TRANSPORT_TYPE                  ICF_TRANSPORT_TYPE_TLS
#define ICF_MIN_ERR_CORRECTION_METHOD           ICF_T38_ERROR_CORR_REDUNDANCY
#define ICF_MAX_ERR_CORRECTION_METHOD           ICF_T38_ERROR_CORR_FEC
#define ICF_MAX_MODE                            ICF_MEDIA_MODE_INACTIVE
#define ICF_MIN_MODE                            ICF_MEDIA_MODE_SEND_RECV 
#define ICF_MIN_CALL_HOLD_OPTION                ICF_CALL_HOLD_OPT_INV_WTH_C_0
#define ICF_MAX_CALL_HOLD_OPTION                ICF_CALL_HOLD_OPT_UPDT_WTH_MODE_SEND_ONLY_N_C_0
#define ICF_MAX_MEDIA_TRANSPORT_TYPE            4

/* Macro for string comparision */    
#ifdef ICF_PORT_WINCE
#define icf_vldt_port_strcasecmp _stricmp
#else

#if defined (ICF_PORT_WINDOWS)
#define icf_vldt_port_strcasecmp strcmpi

#else
#if defined (ICF_PORT_VXWORKS)
#define icf_vldt_port_strcasecmp icf_vldt_port_strcmpi
#else
#define icf_vldt_port_strcasecmp strcasecmp
#endif/* end of ifdef for VXWORKS */
#endif /* end of ifdef for windows */
#endif /* end of ifdef for WINCE */    
/*End of  file */
/*CSR 1-7718121 merge*/    
#define icf_vldt_port_strtok  strtok
/*CSR 1-7718121 merge*/    

/*Merged CSR 1-7931294. Fix in 1-8561947.*/
#ifdef ICF_VLDT_TRACE_ENABLE
#define ICF_VLDT_PRINT(arg) printf arg
#else
#define ICF_VLDT_PRINT(arg)
#endif

/* Hash functions prototypes end*/
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_VLDT_PROTOTYPES_H__ */


