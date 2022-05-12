/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_discovery_defs.h 
 * 
 * DESCRIPTION: This file contains the hash defines  used to implement the 
 *              functionality of STUN Type Discovery.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Abhishek Chhibber		23-Jun-2006		Initial Version
 *          
 * Copyright 2006, Aricent 
 * ****************************************************************************/

#ifndef __STUN_DISCOVERY_DEFS_H__
#define __STUN_DISCOVERY_DEFS_H__

/* Prevent Name Mangling By C++ Compilers */
#include "nat_defs.h"
#include "nat_types.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/*------------------------------------------------------------------------------
 * 
 * General hash defines
 * 
 * --------------------------------------------------------------------------*/
#define STUN_MAX_DISC_TIMERS                     2

/*------------------------------------------------------------------------------
 * 
 * Hash defines for the states of STUN.
 * 
 * --------------------------------------------------------------------------*/

#define NAT_STUN_DISCOVERY_00_IDLE					0
#define NAT_STUN_DISCOVERY_01_AWT_RESP 				1
#define NAT_STUN_DISCOVERY_02_AWT_RESP				2
#define NAT_STUN_DISCOVERY_02_01_AWT_RESP				3
#define NAT_STUN_DISCOVERY_03_AWT_RESP				4
#define NAT_STUN_DISCOVERY_MAX_STATES				5
#define NAT_STUN_DISCOVERY_INVALID_STATE				255

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STUN_DISCOVERY_DEFS_H__ */




