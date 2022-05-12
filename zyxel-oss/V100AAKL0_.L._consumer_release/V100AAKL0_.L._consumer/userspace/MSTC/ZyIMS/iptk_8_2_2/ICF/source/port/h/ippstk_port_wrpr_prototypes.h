/*------------------------------------------------------------------------------
 *
 * File name        : icf_port_wrpr_prototypes.h
 *
 * Purpose          : This file contains the prototypes of functions used for 
 *					  platform request/response handling.
 *
 * Revision History :
 *
 *   Date       Name			Ref#		Comments
 * --------     ------------	----		---------------
 * 12-Dec-2005	Abhishek/Rohit	ICF 4.0 CDD	Initial
 * 
 *                                        
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_PORT_WRPR_PROTOTYPES_H__
#define __ICF_PORT_WRPR_PROTOTYPES_H__

#include "icf_port_wrpr_types.h"

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */



/*------------------------------------------------------------------------------
 * FUNCTION:    icf_port_cbk_recv_from_wrpr
 *
 * DESCRIPTION: This function is the callback invoked by wrapper whenver it 
 *				receives a response or unsolicited indication from underlying 
 *				platform.
 *
 * RETURNS:     ICF_SUCCESS or ICF_FAILURE
 *              
 *----------------------------------------------------------------------------*/

icf_return_t icf_port_cbk_recv_from_wrpr(
	INOUT	icf_port_wrpr_resp_st		port_resp);
					



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
