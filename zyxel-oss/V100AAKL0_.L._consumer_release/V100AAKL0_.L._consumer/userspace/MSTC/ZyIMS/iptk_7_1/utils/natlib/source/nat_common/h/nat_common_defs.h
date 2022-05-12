/*------------------------------------------------------------------------------
 *
 * File name        : nat_common_defs.h
 *
 * Purpose          : This file contains all hash define values   used by all 
 *                    internal modules of NAT Client Library. 
 *
 * Revision History :
 *
 * Date         Name              Ref#        Comments
 * --------     ------------      ----        ---------------
 * 22-Jun-2006  Abhishek Chhibber NAT_LLD  File created and initial version.
 * 11-Jul-2006	Anuradha Gupta	  LLD	   Coding
 * 16-Nov-2006	Anuradha Gupta	  LLD		New macros added
 *											
 * Copyright (c) 2006, Aricent
 *----------------------------------------------------------------------------*/

#ifndef __NAT_COMMON_DEFS_H__
#define __NAT_COMMON_DEFS_H__


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

/* --------------------------------------------------------------------------
 * Constant to set and check for NULL
 * --------------------------------------------------------------------------*/
#define NAT_NULL                    0
#define NAT_INVALID_TIMER_ID		0
/* --------------------------------------------------------------------------
 * Constants to check the return value of functions
 * --------------------------------------------------------------------------*/

#define NAT_RETURN_FAILURE   0
#define NAT_RETURN_SUCCESS   1
#define NAT_RETURN_PENDING   2



/* --------------------------------------------------------------------------
 * Constants to describe function arguments
 * --------------------------------------------------------------------------*/
#ifndef INOUT
#define INOUT  
#endif

#ifndef OUT
#define OUT  
#endif

#ifndef IN
#define IN 	
#endif

#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_LINUX) || defined(NAT_PORT_WINCE) || defined(NAT_PORT_VXWORKS)
#define NAT_PRINT(x)    printf x
#endif

/* --------------------------------------------------------------------------
 * Constants to set and check the boolean varibles
 * --------------------------------------------------------------------------*/

#define NAT_TRUE 	1
#define NAT_FALSE 	0


/* --------------------------------------------------------------------------
 * Hash defines for Internal Module ID for NAT , used with nat_int_module_id_t
 * --------------------------------------------------------------------------*/

#define NAT_STUN_MODULE_SHSEC		0
#define NAT_STUN_MODULE_LIFETIME	1	
#define NAT_STUN_MODULE_DISCOVERY	2	
#define NAT_STUN_MODULE_BINDING     3
#define NAT_STUN_MODULE_EXTERNAL	4	

/* --------------------------------------------------------------------------
 * General Hash defines 
 * --------------------------------------------------------------------------*/
#define NAT_INVALID_TRANSACTION_ID	0

/* --------------------------------------------------------------------------
 * Common Usage Hash defines 
 * --------------------------------------------------------------------------*/
#define NAT_CMN_IP_ADDR_NO_MATCH    0
#define NAT_CMN_PORT_ADDR_NO_MATCH  1

/* ----------------------------------------------------------------------
 * Hash Defines : nat_timer_type_t 
 * Description  : This contains the different timer ids for timers started 
 *                by different modules.
 * -----------------------------------------------------------------------*/

#define NAT_TIMER_TYPE_RETRANSMIT       0
#define NAT_TIMER_TYPE_REFRESH          1
#define NAT_TIMER_TYPE_GUARD            2

/*------------------------------------------------------------------------------
 * 
 * Hash defines for the timer status of STUN.
 * 
 * --------------------------------------------------------------------------*/
#define NAT_TIMER_STATUS_INVALID	0
#define NAT_TIMER_STATUS_VALID		1
#define NAT_TIMER_STATUS_PENDING	2

/* ----------------------------------------------------------------------
 * Hash Defines : nat_pool_id_t 
 * Description  : This contains the different pool ids for allocation of
 *                memory pools as required by TK.
 * -----------------------------------------------------------------------*/

#define NAT_MEM_COMMON           0
    /*
     * Following are task specific poolids
     */
#define NAT_MEM_MESSAGE_BUFFER   1
#define NAT_MEM_INT_MSG          2
#define NAT_MEM_TIMER            3
#define NAT_MAX_NUM_POOL         14
 
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __NAT_COMMON_DEFS_H__ */

    
