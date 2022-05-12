/******************************************************************************
*                                    
* FILE NAME     : nat_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the Constants used in NAT CLIENT
*                APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME              REFERENCE       REASON        
* ------         ------------        ----------     ----------
* 29-Nov-2006	Anuradha Gupta		HLD				Initial Version
* 14-May-2010	Preksha Gupta		SPR 20375	    Merged CSR 1-7845448
*
* Copyright 2010, Aricent                      
*                                    
*******************************************************************************/


#ifndef __NAT_DEFS_H__
#define __NAT_DEFS_H__

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/* --------------------------------------------------------------------------
 * PORT specific hash defines
 * --------------------------------------------------------------------------*/
#define NAT_MODULE_NW_UDP       0
#define NAT_MODULE_NW_TCP       1
#define NAT_MODULE_NW_TLS       2
#define NAT_MODULE_NAT      3
#define NAT_MODULE_APP      4
#define NAT_MAX_MODULE      5
/*
 * This is used at Eshell interface while filling the ip address
 * of the source of a received SIP message so that that ICF can further
 * use the IP address
 */
#define NAT_MAX_IPV6_ADDRESS_LENGTH    46
#define NAT_MAX_IPV4_ADDRESS_LENGTH    15

/* --------------------------------------------------------------------------
 * nat_return_t values
 * --------------------------------------------------------------------------*/
#define NAT_RETURN_FAILURE          0
#define NAT_RETURN_SUCCESS          1
#define NAT_RETURN_PENDING          2

/* --------------------------------------------------------------------------
 * nat_result_t values
 * --------------------------------------------------------------------------*/
#define NAT_RESULT_SUCCESS	  		1
#define NAT_RESULT_FAILURE			0

/* --------------------------------------------------------------------------
 * General hash defines
 * --------------------------------------------------------------------------*/
#define NAT_STUN_DEFAULT_BIND_DISC_TIMER    60
#define NAT_MAX_STR_LEN             128
#define NAT_MAX_OCTET_LEN           512
#define NAT_STUN_HMAC_DIGEST_LENGTH	20
#define NAT_MAX_STR_LEN             128
#define NAT_API_ID_UNDEFINED		-1
#define NAT_INVALID_CONTEXT_VALUE	0

/* --------------------------------------------------------------------------
 * NAT Protocol Names
 * --------------------------------------------------------------------------*/
#define NAT_PROTOCOL_STUN                0x01
#define NAT_PROTOCOL_TURN                0x02
#define NAT_PROTOCOL_ICE                 0x04

/* --------------------------------------------------------------------------
 * NAT Procedure Names
 * --------------------------------------------------------------------------*/
#define NAT_STUN_PROC_NOT_PRESENT                     0x00
#define NAT_STUN_PROC_BINDING_LIFETIME_SUPP           0x01
#define NAT_STUN_PROC_TYPE_DISCOVERY_SUPP             0x02
#define NAT_STUN_PROC_SHARED_SECRET_SUPP              0x04

/* --------------------------------------------------------------------------
 * NAT Types
 * --------------------------------------------------------------------------*/
#define NAT_TYPE_PRIVATE_NETWORK_ONLY                 0x01
#define NAT_TYPE_OPEN_INTERNET_INTERFACE              0x02
#define NAT_TYPE_FULL_CONE                            0x03
#define NAT_TYPE_RESTRICTED                           0x04
#define NAT_TYPE_PORT_RESTRICTED                      0x05
#define NAT_TYPE_SYMMETRIC                            0x06
#define NAT_TYPE_INVALID                              0xff

/* --------------------------------------------------------------------------
 * Procedure Context IDs
 * --------------------------------------------------------------------------*/
#define NAT_PROC_BINDING_LIFETIME_ID       0xFFF1
#define NAT_PROC_TYPE_DISCOVERY_ID         0xFF
#define NAT_PROC_SHSEC_ID                  0xFFF4

/* --------------------------------------------------------------------------
 * External Message API IDs
 * --------------------------------------------------------------------------*/
#define NAT_API_BASE                            1

#define NAT_EXTERNAL_API_START			        NAT_API_BASE

#define NAT_STUN_INIT_REQ			            NAT_API_BASE+1 
#define NAT_STUN_INIT_RESP			            NAT_API_BASE+2
#define NAT_STUN_EXEC_PROC_REQ			        NAT_API_BASE+3
#define NAT_STUN_EXEC_PROC_RESP			        NAT_API_BASE+4
#define NAT_GET_PUBLIC_IP_PORT_REQ  		    NAT_API_BASE+5
#define NAT_GET_PUBLIC_IP_PORT_RESP 		    NAT_API_BASE+6
#define NAT_RECV_UDP_PKT_IND			        NAT_API_BASE+7
#define NAT_RECV_TCP_SEC_PKT_IND		        NAT_API_BASE+8
#define NAT_START_TIMER_REQ			            NAT_API_BASE+9
#define NAT_START_TIMER_RESP			        NAT_API_BASE+10
#define NAT_TIMER_EXP_IND			            NAT_API_BASE+11
#define NAT_STOP_TIMER_REQ			            NAT_API_BASE+12
#define NAT_ERROR_IND				            NAT_API_BASE+13
#define NAT_STUN_PROC_STATUS_IND		        NAT_API_BASE+14
#define NAT_REL_PUBLIC_IP_PORT_REQ              NAT_API_BASE+15
#define NAT_DEINIT_REQ                          NAT_API_BASE+16

#define NAT_EXTERNAL_API_END			        NAT_DEINIT_REQ



/*----------------------------------------------------------------------------
 *
 *  nat_transport_mode_t These values specifies the mode of
 *   transport as TCP or UDP or both
 *
 *--------------------------------------------------------------------------*/

/* Specifies transport mode is UDP. */
#define NAT_TRANSPORT_TYPE_UDP                                   0    

/* Specifies transport mode is TCP. */
#define NAT_TRANSPORT_TYPE_TCP                                   1    

/* Specifies that transport could be either TCP or UDP. */
#define NAT_TRANSPORT_MODE_BOTH                                   2    

#define NAT_TRANSPORT_TYPE_TLS                                    3    

#ifdef __cplusplus

}
#endif /* __cplusplus */

#endif /* __NAT_DEFS_H__ */
