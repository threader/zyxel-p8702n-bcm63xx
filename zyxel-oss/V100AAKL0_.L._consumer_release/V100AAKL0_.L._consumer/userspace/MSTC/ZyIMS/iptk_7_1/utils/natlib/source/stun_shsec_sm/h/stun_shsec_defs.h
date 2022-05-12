/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_shsec_defs.h 
 * 
 * DESCRIPTION: This file contains the hash defines  used to implement the 
 *              functionality of STUN.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Abhishek Chhibber		23-Jun-2006		Initial Version
 *          
 * Copyright 2006, Aricent 
 * ****************************************************************************/
#ifndef __STUN_SHSEC_DEFS_H__
#define __STUN_SHSEC_DEFS_H__

#ifdef __cpluplus
extern "C"
{
#endif

/*------------------------------------------------------------------------------
 * 
 * General Hash defines
 * 
 * --------------------------------------------------------------------------*/
#define STUN_SHSEC_MAX_TIMERS                       2

/*------------------------------------------------------------------------------
 * 
 * Hash defines for the states of SHSEC.
 * 
 * --------------------------------------------------------------------------*/

#define NAT_STUN_SHSEC_00_IDLE						0
#define NAT_STUN_SHSEC_01_AWT_RESP 					1
#define NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP		2
#define NAT_STUN_SHSEC_MAX_STATES					3
#define NAT_STUN_SHSEC_INVALID_STATE				255

/*------------------------------------------------------------------------------
 * 
 * Hash defines for the events of SHSEC.
 * 
 * --------------------------------------------------------------------------*/
#define NAT_STUN_SHSEC_REQ_EVENT				 		0
#define NAT_STUN_SHSEC_RESP_EVENT			 	        1
#define NAT_STUN_SHSEC_TIMER_EXPIRY_IND_EVENT			2
#define NAT_STUN_SHSEC_TIMER_START_RESP_EVENT			3
#define NAT_STUN_SHSEC_MAX_EVENTS						4
#define NAT_STUN_SHSEC_INVALID_EVENT					255


#ifdef __cpluplus
}
#endif

#endif




