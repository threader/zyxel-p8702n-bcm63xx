/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/

#ifndef __MM_PORT_PROTOTYPES_H__
#define __MM_PORT_PROTOTYPES_H__

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

#include "mm_port_wrapper.h"
#include "mm_port_macro.h"

/*************************************************************************
*
*  FUNCTION NAME  : mm_port_udp_init
*
*  DESCRIPTION    : This function is called to initialize the ipc. It  
*					opens socket and bind it to given port.
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

mm_return_t mmPortUdpInit(icf_void_t);

/*************************************************************************
*
*  FUNCTION NAME  : mmPortClose
*
*  DESCRIPTION    : This function is called to close the ipc.  
*
*  RETURNS		  : void
*************************************************************************/

icf_void_t mmPortClose(icf_void_t);

/*************************************************************************
*
*  FUNCTION NAME  :  mmPortUdpRecv
*
*  DESCRIPTION    : This functions implements the udp recieve. It will 
*					first receive the message from UDP and then decode it
*					by calling IPTK clib decode function. Finally it will
*					assign the decoded message pointer to p_buf.
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

mm_return_t 
mmPortUdpRecv(icf_uint8_t **p_buf);


/*************************************************************************
*
*  FUNCTION NAME  : mmIptkIpcSend
*
*  DESCRIPTION    : This is the function that sends the UDP packet to n/w
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

mm_return_t 
mmIptkIpcSend(icf_void_t *p_msg,icf_uint32_t buf_len, mm_error_t *p_error);
/*************************************************************************
*
*  FUNCTION NAME  : mmPortSelect
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

mm_return_t
mmPortSelect(icf_uint32_t usec, icf_boolean_t *p_timeout_flag);



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __IMSC_GUI_TYPES_H__ */

