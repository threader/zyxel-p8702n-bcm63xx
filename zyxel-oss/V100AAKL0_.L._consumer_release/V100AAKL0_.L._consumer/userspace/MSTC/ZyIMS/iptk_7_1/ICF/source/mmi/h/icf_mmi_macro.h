/******************************************************************************
 *      
 * 
 * FILE NAME:   icf_mmi_macro.h 
 * 
 * DESCRIPTION: This file contains the macros used to implement the 
 *              functionality of MMI.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Shefali Aggarwal     26-nov-2004        Initial
 *          
 * Copyright 2006, Aricent
 *
 * ****************************************************************************/

#include "icf_common_types.h"
#include "icf_macro.h"

/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_PDB_RESET_BIT
 * 
 * Description: This is used to reset the bit in the mmi pdb.
 * 
 *----------------------------------------------------------------------------*/

#define ICF_MMI_PDB_RESET_BIT(p_mmi_pdb,bit_mask)\
{\
    p_mmi_pdb->bitmask &= ~bit_mask;\
}

/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_PDB_SET_BIT
 * 
 * Description: This is used to set the bit in the mmi context.
 * 
 *----------------------------------------------------------------------------*/

#define ICF_MMI_PDB_SET_BIT(p_mmi_pdb,bit_mask)\
{\
    p_mmi_pdb->bitmask |= bit_mask;\
}

/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_SET_STATE
 * 
 * Description: This is used to set the current state in the mmi context.
 * 
 *----------------------------------------------------------------------------*/ 
#define ICF_MMI_SET_STATE(p_mmi_pdb,state)\
{\
    p_mmi_pdb->p_mmi_ctx->mmi_state = state;\
}  

/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_RESET_BIT
 * 
 * Description: This is used to reset the bit in the mmi context.
 * 
 *----------------------------------------------------------------------------*/

#define ICF_MMI_RESET_BIT(p_mmi_pdb,bitmask)\
{\
    p_mmi_pdb->p_mmi_ctx->mmi_bitmask = (icf_uint8_t) ((p_mmi_pdb->\
            p_mmi_ctx->mmi_bitmask) & ~(bitmask));\
}       

/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_SET_BIT
 * 
 * Description: This is used to set the bit in the mmi context.
 * 
 *----------------------------------------------------------------------------*/

#define ICF_MMI_SET_BIT(p_mmi_pdb,bitmask)\
{\
    p_mmi_pdb->p_mmi_ctx->mmi_bitmask =  (icf_uint8_t) ((p_mmi_pdb->\
            p_mmi_ctx->mmi_bitmask) | bitmask);\
}

/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_SET_EVENT
 * 
 * Description: This is used to set the event in the mmi context.
 * 
 *----------------------------------------------------------------------------*/

#define ICF_MMI_SET_EVENT(p_mmi_pdb,event)\
{\
    p_mmi_pdb->p_mmi_ctx->mmi_event = event;\
}        
    
/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_TRACE
 * 
 * Description: This is the macro used for traces.
 * 
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_MMI_TRACE(p_mmi_pdb,trace_id)\
{\
    icf_mmi_trace(p_mmi_pdb,(icf_trace_id_t)(trace_id));\
}
#else
#define ICF_MMI_TRACE(p_mmi_pdb, trace_id)

#endif /* End of #ifdef ICF_TRACE_ENABLE */

#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro name: ICF_MMI_APP_TRACE
 *
 * Description: This is the macro used for traces.
 *
 *----------------------------------------------------------------------------*/
#define ICF_MMI_APP_TRACE(p_mmi_pdb,trace_id)  \
{  \
    icf_mmi_app_trace(p_mmi_pdb,(icf_trace_id_t)(trace_id));  \
}
#else
#define ICF_MMI_APP_TRACE(p_mmi_pdb, trace_id)

#endif /* End of #ifdef ICF_TRACE_ENABLE */



/*------------------------------------------------------------------------------
 * 
 * Macro name: ICF_MMI_SET_ECODE
 * 
 * Description: This is the macro used for setting ecodes.
 * 
 *----------------------------------------------------------------------------*/
#ifdef ICF_ERROR_ENABLE
#define ICF_MMI_SET_ECODE(p_mmi_pdb,ecode)\
{\
    icf_mmi_ecode(p_mmi_pdb,(icf_error_id_t)(ecode));\
}
#else
#define ICF_MMI_SET_ECODE(p_mmi_pdb,error)\
{\
    *(p_mmi_pdb->p_ecode) = (icf_error_t)(error);\
}
#endif

/*-----------------------------------------------------------------------------
 * 
 * Macro Name: ICF_MMI_CLEAR_CONTEXT
 * 
 * Description: This is used to clear the context of MMI.
 * 
 *----------------------------------------------------------------------------*/

#define ICF_MMI_CLEAR_CONTEXT(p_mmi_pdb)\
{\
    p_mmi_pdb->p_mmi_ctx->mmi_bitmask = ICF_NULL;\
    p_mmi_pdb->p_mmi_ctx->merge_call_id = ICF_INVALID_CALL_ID;\
    p_mmi_pdb->p_mmi_ctx->codec_for_commit = ICF_NULL;\
    p_mmi_pdb->p_mmi_ctx->mmi_event = ICF_NULL;\
}    



