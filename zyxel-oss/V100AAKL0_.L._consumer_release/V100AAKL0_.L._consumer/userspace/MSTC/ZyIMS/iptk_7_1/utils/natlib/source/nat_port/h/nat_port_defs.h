/******************************************************************************
*                                    
* FILE NAME    : nat_port_defs.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT Client Prototypes
*                APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 20-Nov-2006    Anuradha Gupta                  HLD         Initial Version
*
* Copyright 2006, Aricent                    
*                                    
*******************************************************************************/



#ifndef __NAT_PORT_DEFS_H_

#define __NAT_PORT_DEFS_H_

#include "nat_types.h"

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

#define NAT_MAX_TIMER_LIST_COUNT            	1
#define NAT_PORT_CMN_TASK_ID 			1

#define NAT_GL_ECODE_MAP(p_icf_ecode, gl_ecode) \
{ \
    *p_icf_ecode = gl_ecode; \
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__NAT_PORT_DEFS_H_



