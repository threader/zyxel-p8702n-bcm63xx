/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_entry_pt.c
 *
 * Purpose          : This file contains the entry point function of CM that
 *                    is invoked by other modules of the toolkit.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 25-Nov-2004  R Ezhirpavai    ICF LLD      Initial
 * 21-Jan-2005  Rohit Aggarwal                  Beginning of coding phase
 * 17-May-2005  Rohit Aggarwal                  Fixed SPR 7567
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_internal_api_id.h"
#include "icf_cm_prototypes.h"
#include "icf_cm_macro.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of CM to process an
 *                  incoming message. The message can be received either from
 *                  MRM or RGM or SSA or MMI.
 *                  This function first checks the rules in forward direction
 *                  and then after getting control back, it can send messages
 *                  to either CC or ADM based on the return status or the event
 *                  received.
 *                  The Call Manager does not allocate a new internal message
 *                  or assign a new payload. It just re-routes the message to
 *                  the relevant destination.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_mesg(
        INOUT   icf_internal_msg_st  *p_internal_msg)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_action_result_st          action_result= {0,0,{{0,0,0,0,0,0},0}};
    icf_cm_pdb_st                    cm_pdb, *p_cm_pdb = &cm_pdb;
    icf_internal_msg_st              sent_int_msg;
    icf_error_t                      *p_ecode = ICF_NULL;

    ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb, ICF_INT_MODULE_CM)

#ifdef ICF_UT_TEST
    /* p_ecode declared in icf_glb_pdb_st will point to 
       ecode in icf_internal_msg_st. To change this ecode, 
       value of pointer will be changed */
    p_internal_msg->msg_hdr.p_glb_pdb->p_ecode =
        &(p_internal_msg->msg_hdr.ecode);
#endif

    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)

    /* Initialize the action with the received message */
    action_result.dest = ICF_INT_MODULE_INVALID;
    action_result.status = ICF_CM_CHECK_STATUS_CONT;

    action_result.internal_msg.msg_hdr.msg_id = ICF_NULL;
    action_result.internal_msg.msg_hdr.payload_length = ICF_NULL;
    action_result.internal_msg.msg_hdr.p_glb_pdb =
              p_internal_msg->msg_hdr.p_glb_pdb;
    action_result.internal_msg.msg_hdr.ecode = ICF_ERROR_NONE;
    action_result.internal_msg.msg_hdr.status = ICF_NULL;
    action_result.internal_msg.msg_hdr.status_2 = ICF_NULL;

    action_result.internal_msg.p_msg_data = ICF_NULL;

    icf_port_memcpy(
            (icf_void_t*)&(action_result.internal_msg),
            (icf_void_t*)p_internal_msg,
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_msg_st));
        
    /* Initialize the CM pdb */
    cm_pdb.p_action_result = &action_result;
    cm_pdb.p_recv_int_msg = p_internal_msg;
    cm_pdb.p_sent_int_msg = &sent_int_msg;
    cm_pdb.p_cm_data = ICF_NULL;
    cm_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    cm_pdb.common_bitmask = 0;
    cm_pdb.buff.service_call_buff.p_other_call_ctx = ICF_NULL;
    cm_pdb.buff.service_trig_buff.status = ICF_STATUS_CONTINUE;
    cm_pdb.check_type = (icf_cm_check_type_t)ICF_CM_CHECK_TYPE_FORWARD;
    cm_pdb.field_type = ICF_CM_FT_FP_START;
    cm_pdb.field_value = ICF_NULL;
    cm_pdb.presence_field = ICF_NULL;
    cm_pdb.operation = ICF_NULL;
    cm_pdb.p_value_action = ICF_NULL;
    cm_pdb.p_action = ICF_NULL;
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);

    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_RECV_INT_MSG)

    
    /*------------------------------------------------------------------------*/
    /* Process the message in FORWARD direction */
    /*------------------------------------------------------------------------*/
#ifdef ICF_SERVICE_ENABLE
    ret_val = icf_cm_process_forward_msg(p_cm_pdb);
#endif /* #ifdef ICF_SERVICE_ENABLE */

    /* Default destination for all forward messages is CC */
    if ((ICF_SUCCESS == ret_val) && 
            (ICF_INT_MODULE_INVALID == action_result.dest))
    {
        action_result.dest = ICF_INT_MODULE_CC;

        /* 
         * For forward messages that do not have dest module id set as invalid 
         * by this time, the exact event/msg_id to be sent in CC or ADM entry 
         * point functions would have already been populated in 
         * icf_cm_proc_do_action. Therefore, do it here only when the 
         * routing is being done due to default routing rule.
         */
        if (ICF_MSG_FROM_MRM == p_internal_msg->msg_hdr.msg_id)
        {
            ret_val = icf_cm_util_convert_api_to_event(p_cm_pdb);
        }
    } /* End of if for ret_val and action_result.dest */
    
    /*---------------------------------*/
    /* Take action for FORWARD message */
    /*---------------------------------*/

    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_ACTION_RESULT)

#ifdef ICF_SERVICE_ENABLE
    if ((ICF_SUCCESS == ret_val) && 
            (ICF_INT_MODULE_CC == action_result.dest))
    {
#endif /* #ifdef ICF_SERVICE_ENABLE */

        /* copy the message to be sent in pdb */
        icf_port_memcpy(
            (icf_void_t*)p_cm_pdb->p_sent_int_msg,
            (icf_void_t*)&(p_cm_pdb->p_action_result->internal_msg),
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_msg_st));
        
        /* Call entry point of CC */
        ret_val = icf_cc_process_mesg(
                                &(p_cm_pdb->p_action_result->internal_msg));

#ifdef ICF_SERVICE_ENABLE
    }
    else if ((ICF_SUCCESS == ret_val) && 
            (ICF_INT_MODULE_ADM == action_result.dest))
    {
        /* copy the message to be sent in pdb */
        icf_port_memcpy(
            (icf_void_t*)p_cm_pdb->p_sent_int_msg,
            (icf_void_t*)&(p_cm_pdb->p_action_result->internal_msg),
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_msg_st));
        
        /* Call entry point of ADM */
        ret_val = icf_adm_process_mesg(
                                &(p_cm_pdb->p_action_result->internal_msg));

        /* free the external msg if it was allocated by CM */
        if (ICF_CM_FREE_PA_FAILURE_MSG & p_cm_pdb->common_bitmask)
        {
            ICF_MEMFREE(
                    p_cm_pdb->p_glb_pdb,
                    p_cm_pdb->p_action_result->internal_msg.p_msg_data,
                    ICF_MEM_COMMON,
                    p_ecode,
                    ret_val)
            p_cm_pdb->common_bitmask &= (~(ICF_CM_FREE_PA_FAILURE_MSG));
        }
    } /* End of if-else for ret_val and action_result.dest */
#endif /* #ifdef ICF_SERVICE_ENABLE */
    
    if (ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        ICF_RESET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb)
        return ICF_FAILURE;
    }

    
    /*------------------------------------------------------------------------*/
    /* Process the message in BACKWARD direction */
    /*------------------------------------------------------------------------*/
    do
    {
        if (ICF_INT_MODULE_CC == action_result.dest)
        {
            action_result.dest = ICF_INT_MODULE_INVALID;
            ret_val = icf_cm_process_cc_backward_msg(p_cm_pdb);
        }
#ifdef ICF_SERVICE_ENABLE
        else if (ICF_INT_MODULE_ADM == action_result.dest)
        {
            action_result.dest = ICF_INT_MODULE_INVALID;
            ret_val = icf_cm_process_sc_backward_msg(p_cm_pdb);
        } /* End of if-else for action_result.dest */
#endif /* #ifdef ICF_SERVICE_ENABLE */

        /*
         * Before taking action on backward path, reset the status in internal 
         * message
         */
        p_cm_pdb->p_action_result->internal_msg.msg_hdr.status = 
            ICF_STATUS_CONTINUE;
        
        /*----------------------------------*/
        /* Take action for BACKWARD message */
        /*----------------------------------*/
        /* 
         * There is no default destination for backward messages. Invalid 
         * module ID in destination means that CM's processing of that message 
         * is over.
         */
        
        ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_ACTION_RESULT)
        
        if ((ICF_SUCCESS == ret_val) && 
                (ICF_INT_MODULE_CC == action_result.dest))
        {
            /* copy the message to be sent in pdb */
            icf_port_memcpy(
                (icf_void_t*)p_cm_pdb->p_sent_int_msg,
                (icf_void_t*)&(p_cm_pdb->p_action_result->internal_msg),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_msg_st));
        
            /* Call entry point of CC */
            ret_val = icf_cc_process_mesg(
                                    &(p_cm_pdb->p_action_result->internal_msg));
        }
#ifdef ICF_SERVICE_ENABLE
        else if ((ICF_SUCCESS == ret_val) && 
                (ICF_INT_MODULE_ADM == action_result.dest))
        {
            /* copy the message to be sent in pdb */
            icf_port_memcpy(
                (icf_void_t*)p_cm_pdb->p_sent_int_msg,
                (icf_void_t*)&(p_cm_pdb->p_action_result->internal_msg),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_internal_msg_st));
        
            /* Call entry point of ADM */
            ret_val = icf_adm_process_mesg(
                                    &(p_cm_pdb->p_action_result->internal_msg));
        } /* End of if-else for action_result.dest */
#endif /* #ifdef ICF_SERVICE_ENABLE */

        if (ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
            ICF_RESET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb)
            return ICF_FAILURE;
        }

    } while (ICF_INT_MODULE_CC == action_result.dest || 
             ICF_INT_MODULE_ADM == action_result.dest);

    ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
    ICF_RESET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb)
    return ret_val;
} /* End of icf_cm_process_msg */
