/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_shsec_macro.h 
 * 
 * DESCRIPTION: This file contains the macros used to implement the 
 *              functionality of MMI.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Shefali Aggarwal     26-nov-2004        Initial
 *          
 * Copyright 2006, Aricent                                 *
 * ****************************************************************************/

#include "nat_common_types.h"
#include "nat_macro.h"

/*-----------------------------------------------------------------------------
 * 
 * Macro Name: NAT_STUN_SHSEC_INIT_CONTEXT
 * 
 * Description: This is used to init the context of Shared Secret Context
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_SHSEC_INIT_CONTEXT(p_stun_shsec_context)\
{\
    nat_port_memset(p_stun_shsec_context,0,nat_port_sizeof(stun_shsec_context_st));\
    p_stun_shsec_context->current_state = NAT_STUN_SHSEC_INVALID_STATE;\
    p_stun_shsec_context->current_event = NAT_STUN_SHSEC_INVALID_EVENT;\
}    



/*-----------------------------------------------------------------------------
 * 
 * Macro Name: NAT_STUN_SHSEC_DEINIT_CONTEXT
 * 
 * Description: This is used to clear the context of STUN Shared Secret.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_SHSEC_DEINIT_CONTEXT(p_stun_shsec_pdb)\
{\
   nat_port_memset(&(p_stun_shsec_pdb->p_stun_context),0,nat_port_sizeof(stun_context_shsec_st));\
   p_stun_pdb->p_stun_context->current_state = NAT_STUN_INVALID_STATE;\
   p_stun_pdb->p_stun_context->current_event = NAT_STUN_INVALID_EVENT;\
}    
















/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_STATE
 * 
 * Description: This is used to set the current state in the mmi context.
 * 
 *----------------------------------------------------------------------------*/ 
#define NAT_STUN_SET_STATE(p_stun_pdb,state)\
{\
    p_stun_pdb->p_stun_context->current_state = state;\
}  

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_EVENT
 * 
 * Description: This is used to set the event in the mmi context.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_SET_EVENT(p_mmi_pdb,event)\
{\
p_stun_pdb->p_stun_context->current_event = event;\
}        
    
/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_ECODE
 * 
 * Description: This is the macro used for setting ecodes.
 * 
 *----------------------------------------------------------------------------*/
#define NAT_STUN_SET_ECODE(p_mmi_pdb,error)\
{\
    *(p_stun_pdb->->p_ecode) = (nat_error_t)(error);\
}
#endif

/*-----------------------------------------------------------------------------
 * 
 * Macro Name: NAT_STUN_CLEAR_CONTEXT
 * 
 * Description: This is used to clear the context of MMI.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_CLEAR_CONTEXT(p_mmi_pdb)\
{\
	nat_uint8_t i=0;
    p_stun_pdb->p_stun_context->current_state = NAT_STUN_INVALID_STATE;\
    p_stun_pdb->p_stun_context->current_event = NAT_STUN_INVALID_EVENT;\
    p_stun_pdb->p_stun_context->current_timer_id = 0;\
    p_stun_pdb->p_stun_context->is_refresh_enabled = NAT_FALSE;\
	for(i=0;i<16;i++) \
		p_stun_pdb->p_stun_context->transactionid[i]=0;\
}    



