/*************************************************************************
 *
 * 	FILENAME	: 	nat_common_prototypes.h
 *
 * 	DESCRIPTION	:	This file contains the prototypes that are used for 
 * 	                 common utility functions.
 * 					
 * 	Revision History	:
 * 		DATE		    NAME				REFERENCE			REASON
 * 		----------------------------------------------------------
 *      17-July-2006   Anuradha Gupta      NAT LLD         Initial version.
 * 	Copyright 2006, Aricent 
 *
 *************************************************************************/

#ifndef __NAT_COMMON_PROTOTYPES_H__
#define __NAT_COMMON_PROTOTYPES_H__

/******************** STANDARD INCLUDE FILES *****************************/

/******************** USER INCLUDE FILES *********************************/
#include "nat_common_types.h"
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
 *   */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */

/*******************************************************************************
 *
 * FUNCTION:        nat_cmn_compare_address
 *
 * DESCRIPTION:     This function compares two IP + PORT address  and returns
 *				    the status accordingly.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of address match and 
 *                  NAT_RETURN_FAILURE in case of no match with proper ecode set.                 
 *
 ******************************************************************************/
 nat_return_t
 nat_cmn_compare_address(
      IN nat_ip_port_address_st   address_1,
      IN nat_ip_port_address_st   address_2,
      INOUT nat_error_t              ecode);
      
/*******************************************************************************
 *
 * FUNCTION:        nat_cmn_convert_ipv4_addr_to_string
 *
 * DESCRIPTION:     This function Converts transport Address to a string
 *                  Currently handles only for IPV4_ADDR
 *
 ******************************************************************************/
nat_return_t 
nat_cmn_convert_ipv4_addr_to_string(
        IN nat_ipv4_address_st *p_address,
		OUT nat_uint8_t *p_addr_string);

/*******************************************************************************
 *
 * FUNCTION:        nat_cmn_convert_ip_string_to_ip4_addr
 *
 * DESCRIPTION:     This function converts the transport address to a string
 *                  to be  sored in the SDP body of the SIP message     
 *
 ******************************************************************************/
nat_return_t nat_cmn_convert_ip_string_to_ip4_addr(
        IN nat_uint8_t *p_addr_string,
        OUT nat_ipv4_address_st *p_address);

/*******************************************************************************
 *
 * FUNCTION:        nat_timer_cb_fn
 *
 * DESCRIPTION:     The callback function to be called on timer expiry
 ******************************************************************************/
extern nat_void_t nat_timer_cb_fn(nat_void_t *p_buff);

#if defined (NAT_PORT_WINDOWS) || defined (NAT_PORT_LINUX) || defined(NAT_PORT_WINCE)

char * self_implemented_strtok_r(char *s, const char *delim, char **last);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*  __ICF_COMMON_PROTOTYPES_H__ */
