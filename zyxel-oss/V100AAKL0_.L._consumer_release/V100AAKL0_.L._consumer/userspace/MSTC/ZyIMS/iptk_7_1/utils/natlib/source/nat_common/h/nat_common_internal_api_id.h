/*------------------------------------------------------------------------------
 *
 * File name        : nat_common_internal_api_id.h
 *
 * Purpose          : This file contains all the internal api ids used by all
 *                    internal modules of NAT for communication with each
 *                    other.
 *
 * Revision History :
 *
 * Date         Name            Ref#        Comments
 * --------     ------------    ----        ---------------
 * 22-Jun-2006  Abhishek Chhibber NAT LDD     Initial
 * 16-Jul-2006  Anuradha Gupta    NAT LLD     Updated internal msg IDs
 * Copyright (c) 2006, Aricent 
 *----------------------------------------------------------------------------*/

#ifndef __NAT_COMMON_INTERNAL_API_ID_H__
#define __NAT_COMMON_INTERNAL_API_ID_H__

#include "nat_defs.h"

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


#define NAT_INT_API_START                 	    NAT_EXTERNAL_API_END + 1
#define NAT_COMMON_API_START			        NAT_INT_API_START

#define NAT_INT_START_TIMER_RESP		        NAT_COMMON_API_START+1
#define NAT_INT_TIMER_EXPIRY_IND		        NAT_COMMON_API_START+2
#define NAT_INT_ERROR_IND				NAT_COMMON_API_START+3
#define NAT_COMMON_API_END			            NAT_COMMON_API_START + 10

#define NAT_STUN_API_START			            NAT_COMMON_API_END+1

#define NAT_INT_STUN_INIT_REQ			        NAT_STUN_API_START+1 
#define NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ  	NAT_STUN_API_START+2
#define NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP 	NAT_STUN_API_START+3
#define NAT_INT_STUN_REL_PUBLIC_IP_PORT_REQ	    NAT_STUN_API_START+4
#define NAT_INT_STUN_BINDING_RESP		        NAT_STUN_API_START+5
#define NAT_INT_STUN_DEINIT_REQ			        NAT_STUN_API_START+6

#define NAT_STUN_API_END			            NAT_STUN_API_START + 10

#define NAT_STUN_SHSEC_API_START		        NAT_STUN_API_END + 1
#define NAT_INT_STUN_SHSEC_INIT_REQ		        NAT_STUN_SHSEC_API_START+1 
#define NAT_INT_STUN_SHSEC_REQ			        NAT_STUN_SHSEC_API_START+2 
#define NAT_INT_STUN_SHSEC_RESP			        NAT_STUN_SHSEC_API_START+3
#define NAT_INT_STUN_SHSEC_TLS_RESP		        NAT_STUN_SHSEC_API_START+4
#define NAT_INT_STUN_SHSEC_DEINIT_REQ		    NAT_STUN_SHSEC_API_START+5

#define NAT_STUN_SHSEC_API_END			        NAT_STUN_SHSEC_API_START + 10

#define NAT_STUN_DISCOVERY_API_START		    NAT_STUN_SHSEC_API_END+1

#define NAT_INT_STUN_DISCOVERY_INIT_REQ		NAT_STUN_DISCOVERY_API_START+1
#define NAT_INT_STUN_DISCOVERY_TYPE_REQ		NAT_STUN_DISCOVERY_API_START+2
#define NAT_INT_STUN_DISCOVERY_TYPE_RESP	NAT_STUN_DISCOVERY_API_START+3
#define NAT_INT_STUN_DISCOVERY_DEINIT_REQ	NAT_STUN_DISCOVERY_API_START+4

#define NAT_STUN_DISCOVERY_API_END		    NAT_STUN_DISCOVERY_API_START+10


#define NAT_STUN_LIFETIME_API_START		    NAT_STUN_DISCOVERY_API_END+1

#define NAT_INT_STUN_LIFETIME_INIT_REQ		NAT_STUN_LIFETIME_API_START+1
#define NAT_INT_STUN_LIFETIME_DISC_REQ		NAT_STUN_LIFETIME_API_START+2
#define NAT_INT_STUN_LIFETIME_DISC_RESP		NAT_STUN_LIFETIME_API_START+3
#define NAT_INT_STUN_LIFETIME_DEINIT_REQ		NAT_STUN_LIFETIME_API_START+4

#define NAT_STUN_LIFETIME_API_END		    NAT_STUN_LIFETIME_API_START+10

#define NAT_INT_API_END                   	NAT_EXTERNAL_API_END+256


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __NAT_COMMON_INTERNAL_API_ID_H__ */

