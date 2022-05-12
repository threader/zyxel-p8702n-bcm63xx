/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_lifetime_defs.h 
 * 
 * DESCRIPTION: This file contains the hash defines  used to implement the 
 *              functionality of STUN Lifetime Discovery.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Abhishek Chhibber		23-Jun-2006		Initial Version
 *          
 * Copyright 2006, Aricent 
 * ****************************************************************************/



/*------------------------------------------------------------------------------
 * 
 * Hash defines for the states of STUN.
 * 
 * --------------------------------------------------------------------------*/

#define NAT_STUN_LIFETIME_00_IDLE					0
#define NAT_STUN_LIFETIME_01_AWT_BIND_RESP 			1
#define NAT_STUN_LIFETIME_02_AWT_TIMER_EXPIRY_IND			2
#define NAT_STUN_LIFETIME_03_AWT_NEW_BIND_RESP			3
#define NAT_STUN_LIFETIME_MAX_STATES				4
#define NAT_STUN_LIFETIME_INVALID_STATE				255






