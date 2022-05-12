/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_defs.h 
 * 
 * DESCRIPTION: This file contains the hash defines  used to implement the 
 *              functionality of STUN.
 *              
 * REVISION HISTORY:
 *  DATE            NAME            REFERENCE       REASON        
 * ------         ------------      ---------     ----------
 * 24-Nov-2006	Anuradha Gupta     LLD			 Initial Version
 *          
 * Copyright 2006, Aricent 
 * ****************************************************************************/

#ifndef __STUN_DEFS_H__
#define __STUN_DEFS_H__


#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

#define STUN_MAX_BIND_TIMERS 		3


/*------------------------------------------------------------------------------
 * 
 * Hash defines for the states of STUN.
 * 
 * --------------------------------------------------------------------------*/

#define NAT_STUN_00_IDLE						0
#define NAT_STUN_01_AWT_BIND_RESP					1
#define NAT_STUN_02_AWT_EXTRA_BIND_RESP				2
#define NAT_STUN_03_AWT_START_TIMER_RESP 				3
#define NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK 			4
#define NAT_STUN_MAX_STATES							5
#define NAT_STUN_INVALID_STATE						255

/*------------------------------------------------------------------------------
 * 
 * Hash defines for the events of STUN.
 * 
 * --------------------------------------------------------------------------*/
#define NAT_STUN_BINDING_REQ_EVENT				 0
#define NAT_STUN_TIMER_START_RESP_EVENT			 1
#define NAT_STUN_TIMER_EXPIRY_IND_EVENT			 2
#define NAT_STUN_BINDING_RESP_EVENT				 3
#define NAT_STUN_RELEASE_REQ_EVENT				 4
#define NAT_STUN_MAX_EVENTS						 5
#define NAT_STUN_INVALID_EVENT					255


/*------------------------------------------------------------------------------
 * 
 * Hash defines for the timer types of STUN.
 * 
 * --------------------------------------------------------------------------*/
#define NAT_STUN_BIND_RETRANSMIT_TIMER				 0
#define NAT_STUN_BIND_GUARD_TIMER					 1
#define NAT_STUN_BIND_REFRESH_TIMER			 	     2

#define NAT_STUN_BIND_GUARD_TIMER_VALUE                 10000
#define NAT_STUN_MAX_RETRANSMISSION_COUNT           9


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif




