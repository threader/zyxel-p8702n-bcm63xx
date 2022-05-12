/*------------------------------------------------------------------------------
*
* File name        : iptkapps_port_prototypes.h 
*
* Purpose          : This file contains all the prototypes for 
*                    for the wrapper functions.
* 
* Revision History :
*  DATE            NAME      REFERENCE       REASON
* --------      ------------   ----------   ---------------
* 5-Dec-2007   Vidyut Gupta                 Created Origional
*
*   Copyright © 2006 Aricent Inc . All Rights Reserved.
*
*******************************************************************************/

#ifndef __IPTKAPPS_PORT_PROTOTYPES_H__
#define __IPTKAPPS_PORT_PROTOTYPES_H__

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

#include "iptkapp_port_wrapper.h"
#include "iptkapp_port_macro.h"

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_udp_init
*
*  DESCRIPTION    : This function is called to initialize the ipc. It  
*					opens socket and bind it to given port.
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t iptkapp_port_udp_init(icf_void_t);

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_close
*
*  DESCRIPTION    : This function is called to close the ipc.  
*
*  RETURNS		  : void
*************************************************************************/

icf_void_t iptkapp_port_close(icf_void_t);

/*************************************************************************
*
*  FUNCTION NAME  :  iptkapp_port_udp_recv
*
*  DESCRIPTION    : This functions implements the udp recieve. It will 
*					first receive the message from UDP and then decode it
*					by calling IPTK clib decode function. Finally it will
*					assign the decoded message pointer to p_buf.
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t 
iptkapp_port_udp_recv(icf_uint8_t **p_buf);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_iptk_ipc_send
*
*  DESCRIPTION    : This is the function that sends the UDP packet to n/w
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t 
iptkapp_iptk_ipc_send(icf_void_t *p_msg,icf_uint32_t buf_len, iptkapp_error_t *p_error);

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_select
*
*  DESCRIPTION    : This is the wrapper function to the select. it returns 
*				    ICF_SUCCESS whether the data is there on the fd or
*					if the select is timed out.
* 
*  PARAMETER	  :	 p_timeout : will be TRUE if the select has timed out and no
*					 data was there on the FD
*				  :  usec : is the time in micro sec				
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t
iptkapp_port_select(icf_uint32_t usec, icf_boolean_t *p_timeout_flag);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_getline
*
*  DESCRIPTION    : This is the function that take read a line
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t 
iptkapp_port_getline(icf_uint8_t str[],icf_int32_t limit);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __IMSC_GUI_TYPES_H__ */



