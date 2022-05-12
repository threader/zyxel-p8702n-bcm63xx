/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_util.c
 *
 * Purpose          : This file contains the utility functions used in CM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 25-Nov-2004  R Ezhirpavai    ICF LLD      Initial
 * 21-Jan-2005  Rohit Aggarwal                  Beginning of coding phase
 * 25-Feb-2005  Rohit Aggarwal                  Added a hash define
 *                                              ICF_CM_SCM_GENERIC_CALL_CLR
 *                                              to remove handlng Generic Call
 *                                              Clear event on SCM's return path
 * 14-Mar-2005  Rohit Aggarwal                  Added check for service_ctx ptr 
 *                                              before checking for Refer active
 * 17-May-2005  Rohit Aggarwal                  Fixed SPR 7567
 * 25-May-2005  Rohit Aggarwal                  Fixed SPR 7600
 * 30-Mar-2006    Anuradha Gupta	ICF 5.0		Call Modify Feature changes
 * 03-Jan-2007  Amit Mahajan                    INFO confirmation changes. 
 * 26-Mar-2007  Deepti Goyal        ICF 6.4     Changes for Media connect req
 *                                              API from PA.
 * * 04-Apr-2007  Deepti Goyal        ICF 6.3     Changes for Media connect req
 *                                              API from PA.
 * 16-Jul-2007 Abhishek Dhammawat   ICF 7.0     XCONF feature additions.
 * 17-Jul-2007 Abhishek Dhammawat   ICF 7.0     JOIN Header Changes.
 * 22-Aug-2007 Amit Sharma          ICF 7.0     Merge from PQT_6_5
 * 02-Apr-2008  Shruti Thakral      SPR 18350   Klocwork warning removal
 * 03-Apr-2008  Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 12-Apr-2008  Shruti Thakral      SPR 18350   Klocwork warning removal
 * 30-Mar-2009  Tarun Gupta         Rel 8.2     REFER/NOTIFY Enhancements
 * 26-May-2009  Anuradha Gupta      SPR 19672   CSR 1-6727350 merge
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_internal_api_id.h"
#include "icf_common_defs.h"
#ifdef ICF_STAT_ENABLED
#include "icf_stats_defs.h"
#endif /* #ifdef ICF_STAT_ENABLED */
#include "icf_dbm_prototypes.h"
#include "icf_cm_prototypes.h"
#include "icf_cm_macro.h"


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_trace
 *
 * DESCRIPTION:     This function is used by CM for trace handling.
 *
 ******************************************************************************/
icf_void_t icf_cm_trace(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_trace_id_t       trace_id)
{
    icf_trace_data_st                trace_data;
    icf_cm_value_action_st           *p_value_action = ICF_NULL;
    icf_cm_action_st                 *p_action = ICF_NULL;

    switch(trace_id)
    {
        case ICF_MSG_PROC_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_cm_pdb->p_action_result->
                                        status);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_MSG_PROC_RECV_INT_MSG:
            if (ICF_NULL != p_cm_pdb->p_glb_pdb->p_recv_msg)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            }
            else
            {
                trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            }
            if (ICF_NULL != p_cm_pdb->p_glb_pdb->p_call_ctx)
            {
                trace_data.int_val[0] = (icf_int32_t)(p_cm_pdb->p_glb_pdb->
                                            p_call_ctx->call_id);
            }
            else
            {
                trace_data.int_val[0] = (icf_int32_t)ICF_INVALID_CALL_ID;
            }
            trace_data.int_val[1] = (icf_int32_t)(p_cm_pdb->p_recv_int_msg->
                                        msg_hdr.msg_id);
            trace_data.int_val[2] = (icf_int32_t)(p_cm_pdb->p_recv_int_msg->
                                        msg_hdr.status);
            if (ICF_NULL != p_cm_pdb->p_glb_pdb->p_recv_msg)
            {
                trace_data.int_val[3] = (icf_int32_t)(p_cm_pdb->p_glb_pdb->
                                            p_recv_msg->hdr.api_id);
            }
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
           
        case ICF_MSG_PROC_ACTION_RESULT:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            if (ICF_NULL != p_cm_pdb->p_glb_pdb->p_call_ctx)
            {
                trace_data.int_val[0] = (icf_int32_t)(p_cm_pdb->p_glb_pdb->
                                            p_call_ctx->call_id);
            }
            else
            {
                trace_data.int_val[0] = (icf_int32_t)ICF_INVALID_CALL_ID;
            }
            trace_data.int_val[1] = (icf_int32_t)(p_cm_pdb->p_action_result->
                                        internal_msg.msg_hdr.msg_id);
            trace_data.int_val[2] = (icf_int32_t)(p_cm_pdb->p_action_result->
                                        status);
            trace_data.int_val[3] = (icf_int32_t)(p_cm_pdb->p_action_result->
                                        dest);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
          
        case ICF_MSG_PROC_FIELD_DATA:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_cm_pdb->field_type);
            trace_data.int_val[1] = (icf_int32_t)(p_cm_pdb->field_value);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
   
        case ICF_MSG_PROC_FIELD_TYPE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_cm_pdb->field_type);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
        case ICF_MSG_PROC_PRESENCE_FIELD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_cm_pdb->presence_field);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
        
        case ICF_MSG_PROC_VALUE_ACTION:
            p_value_action = (icf_cm_value_action_st *)(p_cm_pdb->
                                p_value_action);

            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_value_action->
                                        presence_field);
            trace_data.int_val[1] = (icf_int32_t)(p_value_action->
                                        value);
            trace_data.int_val[2] = (icf_int32_t)(p_value_action->
                                        operation);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
           
        case ICF_MSG_PROC_ACTION:
            p_action = (icf_cm_action_st *)(p_cm_pdb->p_action);

            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_action->presence_field);
            trace_data.int_val[1] = (icf_int32_t)(p_action->event);
            trace_data.int_val[2] = (icf_int32_t)(p_action->dest);
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
           
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
            
    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
    ICF_TRACE(p_cm_pdb->p_glb_pdb, trace_id, &trace_data)

} /* End of icf_cm_trace */
#endif /* End of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_error
 *
 * DESCRIPTION:     This function is used by CM for error handling.
 *
 ******************************************************************************/
icf_void_t icf_cm_error(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_error_t          ecode)
{
    icf_error_data_st                error_data;
    icf_error_t                      *p_ecode = ICF_ERROR_NONE;

    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);

    switch(ecode)
    {
        case ICF_MSG_PROC_FIELD_TYPE_MISMATCH:
            error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            error_data.int_val[0] = (icf_int32_t)(p_cm_pdb->field_type);
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_MSG_PROC_OPR_MISMATCH:
            error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            error_data.int_val[0] = (icf_int32_t)(p_cm_pdb->operation);
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_INVALID_API_ID_RECD:
            error_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            error_data.int_val[0] =
                (icf_int32_t)(p_cm_pdb->p_recv_int_msg->msg_hdr.msg_id);
            error_data.int_val[1] =
                (icf_int32_t)(p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id);
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        default:
            error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
                 
    } /* end of switch-case for ecode */

    /* Finally, invoke the common macro for error handling */
    ICF_SET_ECODE(p_cm_pdb->p_glb_pdb, &error_data, ecode, p_ecode)
   
} /* End of icf_cm_error */
#endif /* End of #ifdef ICF_ERROR_ENABLE */


#ifdef ICF_STAT_ENABLED
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_update_stats
 *
 * DESCRIPTION:     This function checks the received api and updates its stats.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_update_stats(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint32_t                     stat_id = ICF_STAT_UNDEFINED;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* check the received api and set the corresponding stat_id */
    if(ICF_NULL != p_cm_pdb->p_glb_pdb->p_recv_msg)
    {
        switch (p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id)
        {
            case ICF_PA_CC_CREATE_CALL_REQ:
                stat_id = ICF_STAT_PA_CC_CREATE_CALL_REQ;
                break;

            case ICF_PA_CC_USER_ALERTED_REQ:
                stat_id = ICF_STAT_PA_CC_USR_ALERTED_REQ;
                break;

            case ICF_PA_CC_CONNECT_REQ:
                stat_id = ICF_STAT_PA_CC_CONNECT_REQ;
                break;

            case ICF_PA_CC_TERMINATE_CALL_CFM:
                stat_id = ICF_STAT_PA_CC_TERM_CALL_CFM;
                break;

            case ICF_PA_CC_TERMINATE_CALL_REQ:
                stat_id = ICF_STAT_PA_CC_TERM_CALL_REQ;
                break;

            case ICF_PA_CC_GET_CALL_LOG_REQ:
                stat_id = ICF_STAT_PA_CC_GET_CALL_LOG_REQ;
                break;

            case ICF_PA_CC_CALL_HOLD_REQ:
                stat_id = ICF_STAT_PA_CC_CALL_HOLD_REQ;
                break;

            case ICF_PA_CC_CALL_RESUME_REQ:
                stat_id = ICF_STAT_PA_CC_CALL_RESUME_REQ;
                break;

            case ICF_PA_CC_CONFERENCE_REQ:
                stat_id = ICF_STAT_PA_CC_CONF_REQ;
                break;

            case ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ:
                stat_id = ICF_STAT_PA_CC_CTU_REQ;
                break;

            case ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ:
                stat_id = ICF_STAT_PA_CC_CTA_REQ;
                break;

            case ICF_PA_CC_CALL_MUTE_REQ:
                stat_id = ICF_STAT_PA_CC_CALL_MUTE_REQ;
                break;

            case ICF_PA_CC_CALL_UNMUTE_REQ:
                stat_id = ICF_STAT_PA_CC_CALL_UNMUTE_REQ;
                break;

            default:
                /* nothing to be done here */
                break;

        } /* End of switch */

        /* invoke macro to finally update the stat */
        ICF_STAT(p_cm_pdb->p_glb_pdb, stat_id)
    } /* End of if */
        
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_update_stats */
#endif /* End of #ifdef ICF_STAT_ENABLED */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_check_field_agnst_value
 *
 * DESCRIPTION:     This function checks whether the condition value matches
 *                  with the value that has come in the field.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_check_field_agnst_value(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             field_value,
        OUT     icf_cm_value_action_st   *p_value_action)
{
    icf_return_t                     ret_val = ICF_FAILURE;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
 
    p_cm_pdb->p_value_action = p_value_action;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
        
    /* Check for the presence of a particular value */ 
    if (ICF_CM_VALUE & p_value_action->presence_field)
    {
        /* Check if the operation yields success or not */
        switch (p_value_action->operation)
        {
            case ICF_CM_OPR_EQUAL:
                {
                    if (field_value == p_value_action->value)
                    {
                        ret_val = ICF_SUCCESS;
                    }
                    break;
                }
            case ICF_CM_OPR_NOT_EQUAL:
                {
                    if (field_value != p_value_action->value)
                    {
                        ret_val = ICF_SUCCESS;
                    }
                    break;
                }
            case ICF_CM_OPR_BIT_SET_CHK:
                {
                    if (field_value & p_value_action->value)
                    {
                        ret_val = ICF_SUCCESS;
                    }
                    break;
                }
            case ICF_CM_OPR_BIT_UNSET_CHK:
                {
                    if (!(field_value & p_value_action->value))
                    {
                        ret_val = ICF_SUCCESS;
                    }
                    break;
                }
            default:
                p_cm_pdb->operation = p_value_action->operation;
                ICF_CM_SET_ECODE(p_cm_pdb, ICF_MSG_PROC_OPR_MISMATCH)
                break;
                
        } /* End of switch on operation */
        
    } /* End of if when value is set */
    else
    {
        /* Any value and hence return success */
        ret_val = ICF_SUCCESS;
    } /* End of else when value is not set */
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_check_field_agnst_value */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_convert_api_to_event
 *
 * DESCRIPTION:     This function maps external APIs received by MRM to 
 *                  internal message IDs that can be understood by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_convert_api_to_event(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint16_t                      *p_msg_id = ICF_NULL;
    icf_api_id_t                     *p_api_id = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* local pointer for referencing the external api id received by CM */
    p_api_id = &(p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id);
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_api_id))
   
    /* local pointer for referencing the internal msg id to be sent to CC */
    p_msg_id = &(p_cm_pdb->p_action_result->internal_msg.msg_hdr.msg_id);
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_msg_id))
   
    switch(*p_api_id)
    {
        case ICF_PA_CC_INFO_REQ:
            *p_msg_id = ICF_USER_INFO;
            break;

        case ICF_PA_CC_INFO_CFM:
            *p_msg_id = ICF_USER_INFO_CFM;
            break;

        case ICF_PA_CC_CREATE_CALL_REQ:
            *p_msg_id = ICF_USER_CREATE_CALL;
            break;

        case ICF_PA_CC_USER_ALERTED_REQ:
            *p_msg_id = ICF_USER_CALL_ALERT;
            break;

        case ICF_PA_CC_CONNECT_REQ:
            *p_msg_id = ICF_USER_CALL_CONNECT;
            break;

        case ICF_PA_CC_TERMINATE_CALL_CFM:
            *p_msg_id = ICF_USER_CALL_TERMINATE_RESP;
            break;
            
        case ICF_PA_CC_TERMINATE_CALL_REQ:
            *p_msg_id = ICF_USER_CALL_TERMINATE;
            break;

#ifdef ICF_SERVICE_ENABLE
        case ICF_PA_CC_CALL_MUTE_REQ:
            *p_msg_id = ICF_USER_CALL_MUTE;
            break;

        case ICF_PA_CC_CALL_UNMUTE_REQ:
            *p_msg_id = ICF_USER_CALL_UNMUTE;
            break;
#endif /* #ifdef ICF_SERVICE_ENABLE */

		case ICF_TIMER_MSG:
			*p_msg_id = ICF_TIMER_EXPIRY_IND;
			break;

ICF_HANDLE_REINVITE_START
		case ICF_PA_CC_CALL_MODIFY_CFM:
            *p_msg_id = ICF_USER_CALL_MODIFY_CFM;
            break;

		case ICF_PA_CC_CALL_MODIFY_REQ:
            *p_msg_id = ICF_USER_CALL_MODIFY_REQ;
            break;

        case ICF_PA_CC_START_FAX_REQ:
            *p_msg_id = ICF_USER_START_FAX_REQ;
            break;

ICF_HANDLE_REINVITE_END
        case ICF_EARLY_INC_CALL_CFM :
            *p_msg_id = ICF_USER_EARLY_CALL_CFM;
            break;

        case ICF_PA_CC_MEDIA_CONNECT_REQ:
            *p_msg_id = ICF_USER_CALL_MEDIA_CONNECT;
            break;

	case ICF_STOP_FAX_REQ:
		*p_msg_id = ICF_USER_STOP_FAX_REQ;
		break;
       
        case ICF_CALL_MODIFY_CANCEL_REQ:
                *p_msg_id = ICF_USER_CALL_MOD_CANCEL_REQ;
	        break;

        case ICF_SESSION_TIMER_TOGGLE_REQ:
                *p_msg_id = ICF_USER_SESSION_TIMER_TOGGLE_REQ;
	        break;   

        /* Added for in-call REFER/NOTIFY enhancements */
        case ICF_GENERIC_MSG_REQ:
                *p_msg_id = ICF_USER_GENERIC_MSG_REQ;
	        break;

        case ICF_GENERIC_MSG_CFM:
                *p_msg_id = ICF_USER_GENERIC_MSG_CFM;
	        break;

        default:
            ICF_CM_SET_ECODE(p_cm_pdb, ICF_INVALID_API_ID_RECD)
            ret_val = ICF_FAILURE;
            break;
    }

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_convert_api_to_event */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_value_action
 *
 * DESCRIPTION:     This function accepts a value_action pointer and fills the 
 *                  action for that particular value in its action pointer. It 
 *                  is used for those actions that do not have further checks.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_value_action(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             value,
        IN      icf_uint8_t              opr,
        IN      icf_uint32_t              event,
        IN      icf_int_module_id_t      dest,
        INOUT   icf_cm_value_action_st   **p_p_value_action,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_value_action_st           *p_value_action = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
 
    /* allocate memory to the value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_value_action_st), 
            p_value_action, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)

    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action = p_value_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action))
    
    p_value_action->presence_field = ICF_CM_VALUE;
    p_value_action->value = value;
    p_value_action->operation = opr;
    p_value_action->p_next = ICF_NULL;

    p_cm_pdb->p_value_action = p_value_action;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_action_st), 
            p_value_action->p_action, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)

    ((icf_cm_action_st *)(p_value_action->p_action))->presence_field = 
            ICF_CM_ACTION_EVT_DST;
    ((icf_cm_action_st *)(p_value_action->p_action))->event = event;
    ((icf_cm_action_st *)(p_value_action->p_action))->dest = dest;
    ((icf_cm_action_st *)(p_value_action->p_action))->p_new_check = 
             ICF_NULL;
 
    p_cm_pdb->p_action = ((icf_cm_action_st *)(p_value_action->p_action));
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_ACTION)

    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_value_action */


#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_forceful_clr
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Forceful Call Clear to be sent to SCM.
 *                  This function sets destination to SCM if service context is 
 *                  not null, else it routes the message to CC if call context 
 *                  is not null.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_forceful_clr(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;
    icf_cm_action_st                 *p_action_child = ICF_NULL;
    /* pointers at two levels below root */
    icf_cm_new_check_st              *p_new_check_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_grand = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_FORCEFUL_CALL_CLEAR_IND;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    /* 
     * Send Forceful Call Clear to SCM only if service_ctx exists, else send to 
     * CC after checking that the call_ctx is not null.
     */

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If service_ctx is not null, send to ADM */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL,
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If service_ctx is null, do new check */
    /*-----------------------------------------------------------*/        
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_child->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_next)
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_NULL;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_child->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))
    
    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type, 
            ICF_CM_FT_FP_CHK_CALL_CTX)
        
    p_field_check_grand->p_value_action_list = ICF_NULL;

    /*--------------------------------------------*/
    /* value_action_grand : 2.1 */
    /* If call_ctx is not null, then send to CC   */
    /*--------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    }
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
      
    /*--------------------------------------------*/
    /* value_action_grand : 2.2 */
    /* If call_ctx is null, then do nothing       */
    /*--------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                           p_cm_pdb, 
                           (icf_uint32_t)ICF_NULL, 
                           (icf_uint8_t)ICF_CM_OPR_EQUAL,
                           (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND, 
                           (icf_int_module_id_t)(ICF_CM_DESTINATION_NONE),
                           &(p_value_action_grand->p_next),
                           p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    }
    p_value_action_grand = p_value_action_grand->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))   
        
    /*------------------------------------*/
    /* value_action_grand : NULL */
    /* end the child level list of values */
    /*------------------------------------*/        
    p_value_action_grand->p_next = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
     
     ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
     return ret_val;
} /* End of icf_cm_util_fill_fp_va_forceful_clr */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_srv_api_fail
 *
 * DESCRIPTION:     This function forms the failure response to be sent to ADM 
 *                  in case req_api is recd from PA with invalid callid.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_srv_api_fail(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_msg = ICF_NULL;
    icf_uint16_t                      api_len = ICF_NULL;
    icf_error_t                      *p_ecode = ICF_NULL;
    icf_uint32_t                     req_api = ICF_API_ID_UNDEFINED;
    icf_uint32_t                     resp_api = ICF_API_ID_UNDEFINED;
        
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)   
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode); 
    
    /* fill the response msg */
    req_api = p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id;
    if (ICF_PA_CC_CALL_HOLD_REQ == req_api)
    {
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_UNSUCC_CALL_HOLD)
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_CC_PA_CALL_HOLD_RESP)
        resp_api = ICF_CC_PA_CALL_HOLD_RESP;
        /* get the size of external msg */
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_api_resp_st);
    }
    else if (ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ == req_api)
    {
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_CC_PA_CTA_RESP)
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_UNSUCC_CTA)
        resp_api = ICF_CC_PA_CALL_TRANSFER_ATTENDED_RESP;
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_cta_resp_st);
    }
    else if (ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ == req_api)
    {
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_UNSUCC_CTU)
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)
        resp_api = ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP;
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_ctu_resp_st);
    }
    else if (ICF_PA_CC_CALL_RESUME_REQ == req_api)
    {
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_UNSUCC_CALL_RESUME)
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_CC_PA_CALL_RESUME_RESP)
        resp_api = ICF_CC_PA_CALL_RESUME_RESP;
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_api_resp_st);
    }
    else if (ICF_PA_CC_CONFERENCE_REQ == req_api)
    {
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_CC_PA_CONF_RESP)
        ICF_STAT(p_cm_pdb->p_glb_pdb, ICF_STAT_UNSUCC_CONF)
        resp_api = ICF_CC_PA_CONFERENCE_RESP;
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_msg_resp_st);
    }
    else if(ICF_PA_CC_CREATE_XCONF_REQ == req_api)
    {
        resp_api = ICF_CC_PA_CREATE_XCONF_RESP;
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_create_ext_xconf_resp_st);

    }
    else if(ICF_PA_CC_ADD_XCONF_PARTY_REQ  == req_api)
    {
       resp_api = ICF_CC_PA_ADD_XCONF_PARTY_RESP;
       api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_add_xconf_party_resp_st);
    }
    else if(ICF_PA_CC_DELETE_XCONF_PARTY_REQ == req_api)
    {
      resp_api = ICF_CC_PA_DELETE_XCONF_PARTY_RESP;
      api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
         ICF_PORT_SIZEOF(icf_delete_xconf_party_resp_st);
    }
    else if(ICF_PA_CC_RELEASE_XCONF_REQ == req_api)
    {
      resp_api = ICF_CC_PA_RELEASE_XCONF_RESP;
      api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
              ICF_PORT_SIZEOF(icf_release_xconf_resp_st);
    }
    

    /* allocate memory for response msg */
    ICF_MEMGET(
            p_cm_pdb->p_glb_pdb,
            api_len,
            ICF_MEM_COMMON,
            p_msg,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)

    icf_cmn_fill_resp_msg(
            p_cm_pdb->p_glb_pdb,
            p_msg,
            (icf_module_id_t)(ICF_MODULE_PA),
            (icf_api_id_t)(resp_api),
            (icf_boolean_t)ICF_FAILURE,
            (icf_uint8_t)ICF_CAUSE_INVALID_CALL_ID);
    p_msg->hdr.api_length = api_len;
    p_msg->hdr.app_id = p_cm_pdb->p_action_result->internal_msg.msg_hdr.p_glb_pdb->p_recv_msg->hdr.app_id;

    /* set the msg in action result */
    p_cm_pdb->p_action_result->internal_msg.msg_hdr.msg_id = (icf_uint16_t)resp_api;
    p_cm_pdb->p_action_result->internal_msg.p_msg_data = 
        (icf_uint8_t *)p_msg;
    
    /* set pdb bit indicating that the allocated msg needs to be freed by CM */
    p_cm_pdb->common_bitmask |= ICF_CM_FREE_PA_FAILURE_MSG;

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_fp_srv_api_fail */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_mrm_srv_api
 *
 * DESCRIPTION:     This function checks the received PA service req_api and 
 *                  forwards internal event to ADM if call_ctx is not null, 
 *                  else it sends a failure response to ADM.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_mrm_srv_api(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             req_api,
        IN      icf_uint32_t              event,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_uint32_t             resp_api,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = req_api;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))
    
    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_CHK_CALL_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If call_ctx is not null, then send recd PA api to ADM */
    /*-----------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)event, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
      
    /*-----------------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If call_ctx is null, then make and send failure response to ADM */
    /*-----------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)resp_api, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_child->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
    
    /* also set the function to make the failure response */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr = 
                    icf_cm_fp_srv_api_fail;
        
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_fill_fp_va_mrm_srv_api */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_mrm_srv_init_conf_api
 *
 * DESCRIPTION:     This function checks the received PA service req_api and
 *                  forwards internal event to ADM if call_ctx is  null,
 *                  else it sends a failure response to ADM.
 * RETURN: ICF_FAILURE or ICF_SUCCESS
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_mrm_srv_init_conf_api(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             req_api,
        IN      icf_uint32_t              event,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_uint32_t             resp_api,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)

    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))

    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = req_api;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;

    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)

    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type,
            ICF_CM_FT_FP_CHK_CALL_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If call_ctx is null, then send recd PA api to ADM */
    /*-----------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_NULL,
                            (icf_uint8_t)ICF_CM_OPR_EQUAL,
                            (icf_uint32_t)event,
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*--------------------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If call_ctx is not null,then make and send failure response to ADM */
    /*--------------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_NULL,
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL,
                            (icf_uint32_t)resp_api,
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_child->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /* also set the function to make the failure response */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr =
                    icf_cm_fp_srv_api_fail;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/
    p_value_action_child->p_next = ICF_NULL;


    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_msg_frm_mrm
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  messages received from MRM.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_msg_frm_mrm(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;
    icf_cm_action_st                 *p_action_child = ICF_NULL;
    /* pointers at two levels below root */
    icf_cm_new_check_st              *p_new_check_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_grand = ICF_NULL;
    icf_cm_action_st                 *p_action_grand = ICF_NULL;
    /* pointers at three levels below root */
    icf_cm_new_check_st              *p_new_check_g_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_g_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_g_grand = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

#ifdef ICF_STAT_ENABLED
    /* invoke utility function to update stats */
    ret_val = icf_cm_util_update_stats(p_cm_pdb);
#endif /* End of #ifdef ICF_STAT_ENABLED */
 
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_MSG_FROM_MRM;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_API)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* API is Call Hold */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_CALL_HOLD_REQ, 
                            (icf_uint32_t)ICF_USER_CALL_HOLD, 
                            &(p_field_check_child->p_value_action_list),
                            (icf_uint32_t)ICF_CC_PA_CALL_HOLD_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* API is Call Transfer Attended */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)
                                ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ,
                            (icf_uint32_t)ICF_USER_CALL_TRANSFER_ATTENDED, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)
                                ICF_CC_PA_CALL_TRANSFER_ATTENDED_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
    
    /*-----------------------------------------------------------*/
    /* value_action_child : 3 */
    /* API is Call Transfer Unattended */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)
                                ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ,
                            (icf_uint32_t)
                                ICF_USER_CALL_TRANSFER_UNATTENDED, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)
                                ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
 
    /*-----------------------------------------------------------*/
    /* value_action_child : 4 */
    /* API is Call Resume */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_CALL_RESUME_REQ,
                            (icf_uint32_t)ICF_USER_CALL_RESUME, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)ICF_CC_PA_CALL_RESUME_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
 
    /*-----------------------------------------------------------*/
    /* value_action_child : 5 */
    /* API is Merge Call */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_CONFERENCE_REQ,
                            (icf_uint32_t)ICF_USER_CALL_CONF, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)ICF_CC_PA_CONFERENCE_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    /*-----------------------------------------------------------*/
    /* value_action_child : 6 */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is null, else send failure response to ADM
     */
      
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_init_conf_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_CREATE_XCONF_REQ,
                            (icf_uint32_t)ICF_USER_INITIATE_EXTERNAL_CONF_REQ, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)ICF_CC_PA_CREATE_XCONF_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
    
    /*-----------------------------------------------------------*/
    /* value_action_child : 7 */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */

   if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_ADD_XCONF_PARTY_REQ,
                            (icf_uint32_t)ICF_USER_ADD_PARTY_REQ, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)ICF_CC_PA_ADD_XCONF_PARTY_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
    
    /*-----------------------------------------------------------*/
    /* value_action_child : 8 */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
           
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_DELETE_XCONF_PARTY_REQ,
                            (icf_uint32_t)ICF_USER_DELETE_PARTY_REQ, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)ICF_CC_PA_DELETE_XCONF_PARTY_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : 9 */
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
     if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_RELEASE_XCONF_REQ,
                            (icf_uint32_t)ICF_USER_RELEASE_CONFERENCE_REQ, 
                            &(p_value_action_child->p_next),
                            (icf_uint32_t)ICF_CC_PA_RELEASE_XCONF_RESP,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
 
    /*-----------------------------------------------------------*/
    /* value_action_child : 10 */
    /* API is  ICF_PA_CC_REMOTE_CALL_TRANSFER_INITIATED_CFM*/
    /*-----------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM if 
     * call_ctx is not null, else send failure response to ADM
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mrm_srv_api(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_PA_CC_REMOTE_CALL_TRANSFER_INITIATED_CFM,
                            (icf_uint32_t)ICF_USER_CALL_TRANSFER_INITIATED_CFM, 
                            &(p_value_action_child->p_next),
                            ICF_NULL,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
           
    /*-----------------------------------------------------------*/
    /* value_action_child : 11 */
    /* API is Timer Expiry */
    /*-----------------------------------------------------------*/        
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_child->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_next)
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_TIMER_MSG;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_child->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;

    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))
    
    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type, 
            ICF_CM_FT_FP_DST_IN_TMR_BUF)
    
    p_field_check_grand->p_value_action_list = ICF_NULL;

    /*--------------------------------------------*/
    /* value_action_grand : 10.1 */
    /* API is Timer Expiry for CC */
    /*--------------------------------------------*/
    /* Handling: send this message to CC */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_TIMER_EXPIRY_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
       
    /*-------------------------------------------------*/
    /* value_action_grand : 10.2 */
    /* API is Timer Expiry for ADM */
    /*-------------------------------------------------*/        
    /* Handling: send this message to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_TIMER_EXPIRY_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_grand->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_value_action_grand->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))

    /*------------------------------------*/
    /* value_action_grand : NULL */
    /* end the child level list of values */
    /*------------------------------------*/        
    p_value_action_grand->p_next = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 11 */
    /* API is Call Terminate Request */
    /*-----------------------------------------------------------*/
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_child->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_next)
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_PA_CC_TERMINATE_CALL_REQ;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_child->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;

    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))
    
    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type, 
            ICF_CM_FT_FP_CHK_SERVICE_CTX)
    
    p_field_check_grand->p_value_action_list = ICF_NULL;

    /*-------------------------------------------------------------*/
    /* value_action_grand : 11.1 */
    /* API is Call Terminate Request when service ctx is not null */
    /*------------------------------------------------------------*/
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_field_check_grand->p_value_action_list,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_field_check_grand->p_value_action_list)
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
        
    p_value_action_grand->presence_field = ICF_CM_VALUE;
    p_value_action_grand->value = ICF_NULL;
    p_value_action_grand->operation = (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL;
    p_value_action_grand->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_grand;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_grand->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_grand->p_action)
    p_action_grand = p_value_action_grand->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_grand))

    p_action_grand->presence_field = ICF_CM_ACTION_NEW_CHK;

    /* allocate memory for grand-grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_grand->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_grand->p_new_check)
    p_new_check_g_grand = p_action_grand->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_g_grand))
    
    p_new_check_g_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_g_grand->p_next = ICF_NULL;
    /* allocate memory for grand-grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_g_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_g_grand->p_field_check)
    p_field_check_g_grand = p_new_check_g_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_g_grand))
       
    ICF_CM_SET_FIELD_TYPE(p_field_check_g_grand->field_type, 
            ICF_CM_FT_FP_ACTIVE_SERVICE_LST)
    
    p_field_check_g_grand->p_value_action_list = ICF_NULL;
       
    /*---------------------------------------------------*/
    /* value_action_g_grand : 11.1.1 */
    /* API is Call Terminate Request when CTA is ongoing */
    /*---------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_SERVICE_CTA, 
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_USER_CALL_TERMINATE, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_g_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_g_grand = p_field_check_g_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_g_grand))
       
    /*---------------------------------------------------*/
    /* value_action_g_grand : 11.1.2 */
    /* API is Call Terminate Request when CTU is ongoing */
    /*---------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_SERVICE_CTU, 
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_USER_CALL_TERMINATE, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_g_grand->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_g_grand = p_value_action_g_grand->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_g_grand))

    /*------------------------------------------------*/
    /* value_action_g_grand : NULL */
    /* end the grand grand child level list of values */
    /*------------------------------------------------*/        
    p_value_action_g_grand->p_next = ICF_NULL;

    /*------------------------------------------*/
    /* value_action_grand : NULL */
    /* end the grand child level list of values */
    /*------------------------------------------*/        
    p_value_action_grand->p_next = ICF_NULL;
    
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_fp_va_msg_frm_mrm */

/*#ifdef IMS_CLIENT*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_media_update_resp
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  success response for Reinvite received from SSA.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_media_update_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_NW_INC_MEDIA_UPDATE_RESP;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_SIG_ONG_MODULE)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If CC had sent the request, send to CC */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If CC had not sent the request, send to ADM */
    /*-----------------------------------------------------------*/        
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_child->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_fp_va_media_update_resp */
/*#endif*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_reinvite
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Reinvite received from SSA.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_reinvite(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_NW_INC_MEDIA_UPDATE;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_ACTIVE_SERVICE_LST)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If CHR service is currently active, send to ADM */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_SERVICE_CHR, 
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If CHR service is currently not active, send to CC */
    /*-----------------------------------------------------------*/        
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_SERVICE_CHR, 
                            (icf_uint8_t)ICF_CM_OPR_BIT_UNSET_CHK, 
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_value_action_child->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_fp_va_reinvite */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_mms_resp
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  MMS response received from MMI.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_mms_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_MODIFY_MEDIA_RESP;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_MEDIA_ONG_IN_SC)
    
    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If media_ongoing is for CC then send to CC */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_MODIFY_MEDIA_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If media_ongoing is not for CC then send to ADM */
    /*-----------------------------------------------------------*/        
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_MODIFY_MEDIA_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_child->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_fill_fp_va_mms_resp */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_media_err
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Media Error indication received from MMI.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_media_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;
    icf_cm_action_st                 *p_action_child = ICF_NULL;
    /* pointers at two levels below root */
    icf_cm_new_check_st              *p_new_check_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_grand = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_MEDIA_ERROR_IND;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_MEDIA_ERROR_CAUSE)
    
    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If media error cause is DMS ind from MM then send to CC */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_ECODE_DMS_IND_RECVD, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_MEDIA_ERROR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If media error cause is not DMS ind from MM, do new check */
    /*-----------------------------------------------------------*/        
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_child->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_next)
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_ECODE_DMS_IND_RECVD;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL;
    p_value_action_child->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))
    
    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type, 
            ICF_CM_FT_FP_MEDIA_ONG_IN_SC)
        
    p_field_check_grand->p_value_action_list = ICF_NULL;

    /*--------------------------------------------*/
    /* value_action_grand : 2.1 */
    /* If media_ongoing is for CC then send to CC */
    /*--------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_MEDIA_ERROR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
       
    /*-------------------------------------------------*/
    /* value_action_grand : 2.2 */
    /* If media_ongoing is not for CC then send to ADM */
    /*-------------------------------------------------*/        
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INT_MODULE_CC, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_MEDIA_ERROR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_grand->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_value_action_grand->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))

    /*------------------------------------*/
    /* value_action_grand : NULL */
    /* end the child level list of values */
    /*------------------------------------*/        
    p_value_action_grand->p_next = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
     

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_fp_va_media_err */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_setup
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  setup request received from SSA.
 *                  CM shall check if the replace call id is present in setup 
 *                  req. If present, this function shall invoke an action 
 *                  routine that processes the received message before sending 
 *                  call terminate to CC for old call id.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_setup(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_NW_INC_CALL_SETUP;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_REPLACE_INITIATED_CALL)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If replace_initiated call id is set, define action */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INVALID_CALL_ID,
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_USER_CALL_TERMINATE, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /* 
     * also set the action as execution of a function that processes the 
     * received message before sending it to CC
     */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr = 
                    icf_cm_fp_setup_with_replace;
        
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;

 
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_fp_va_setup */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_setup_with_replace
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  setup request received from SSA.
 *                  CM shall check if the replace call id is present in setup 
 *                  req. If present, this function does the following:
 *                  (1) Find the call ctx corresponding to the replace call id 
 *                      (called as old call ctx and the current one in glb_pdb 
 *                      is named as new call ctx).
 *                  (2) Update old call ctx's replace call id with new call id. 
 *                  (3) It shall also check if the call status of the old call 
 *                      ctx has the local call held status as true, then it 
 *                      shall update the local call held status in new call ctx.
 *                      This will only be done if IPTK handles the new call on
 *						its own and does not wait for the APP trigger for call
 *						connect.
 *                  (4) Fill the old call ctx in glb_pdb of message to be sent.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_setup_with_replace(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      *p_ecode = ICF_ERROR_NONE;
    icf_call_ctx_st                  *p_old_call_ctx = ICF_NULL;
    icf_call_ctx_st                  *p_new_call_ctx = ICF_NULL;
    icf_config_data_st				 *p_config_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
    
    /* copy the ecode pointer locally */
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))
        
    /* copy the new call's context pointer locally */
    p_new_call_ctx = p_cm_pdb->p_glb_pdb->p_call_ctx;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_call_ctx))

    /* Fix for CSR 1-6727350: SPR 19316 */
	if (ICF_FAILURE == icf_dbm_get_module_glb_data(
							p_cm_pdb->p_glb_pdb,
							(icf_int_module_id_t)ICF_INT_MODULE_CFG,
							(icf_void_t *)&(p_config_data),
							p_ecode))
	{
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
	}
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_config_data))

    /* 
     * Step 1:
     * Find the old call ctx corresponding to the replace call id
     */
    if (ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                            p_cm_pdb->p_glb_pdb,
                            p_new_call_ctx->replace_initiated_call_id,
                            &p_old_call_ctx,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_old_call_ctx))

    /*
     * Step 2:
     * Update old call ctx's replace call id with new call id
     */
    p_old_call_ctx->replace_initiated_call_id = p_new_call_ctx->call_id;
  
    /*
     * Step 3:
     * Check if the call status of the old call ctx has the local call held 
     * status as true, then update the local call held status in new call ctx
     */
    /* Fix for CSR 1-6727350: SPR 19316 */
	/* This needs to be done only when IPTK connects the new call on its own
     * and does not present it to APP as normal incoming call.
	 */
    if ((ICF_FALSE == p_config_data->app_connect_reqd_for_replaces) &&
		(ICF_CALL_STATUS_LOCAL_HELD & p_old_call_ctx->call_status))
    {
        p_new_call_ctx->call_status |= ICF_CALL_STATUS_LOCAL_HELD;
    }
  
    /*
     * Step 4:
     * Fill the old call ctx in glb_pdb of message to be sent
     */
    p_cm_pdb->p_glb_pdb->p_call_ctx = p_old_call_ctx;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_cm_pdb->p_glb_pdb->p_call_ctx))
        
  
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_fp_setup_with_replace */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_fp_va_cancel
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Cancel request received from SSA.
 *                  CM shall check if the replace call id is present in Cancel 
 *                  request. If present, this function shall invoke an action 
 *                  routine that does further processing of received message.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_fp_va_cancel(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_NW_INC_CALL_ABORT;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_REPLACE_INITIATED_CALL)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If replace_initiated call id is set, define action */
    /*-----------------------------------------------------------*/
    /* send Forceful Call Clear to CC so that call ctx ptr can be freed */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INVALID_CALL_ID,
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /* 
     * also set the action as execution of a function that processes the 
     * received message
     */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr = 
                    icf_cm_fp_cancel_for_replace;
        
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;

 
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_fp_va_cancel */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_cancel_for_replace
 *
 * DESCRIPTION:     This function defines the value action on forward path for 
 *                  Cancel request received from SSA.
 *                  CM shall check if the replace call id is present in Cancel 
 *                  req. If present, this function does the following:
 *                  (1) Find the call ctx corresponding to the replace call id 
 *                      (old call ctx; current one in glb_pdb is new call ctx).
 *                  (2) Remove old call ctx's replace call id. 
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_cancel_for_replace(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      *p_ecode = ICF_ERROR_NONE;
    icf_call_ctx_st                  *p_old_call_ctx = ICF_NULL;
    icf_call_ctx_st                  *p_new_call_ctx = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
    
    /* copy the ecode pointer locally */
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))
        
    /* copy the new call's context pointer locally */
    p_new_call_ctx = p_cm_pdb->p_glb_pdb->p_call_ctx;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_call_ctx))

    /* 
     * Step 1:
     * Find the old call ctx corresponding to the replace call id
     */
    if (ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                            p_cm_pdb->p_glb_pdb,
                            p_new_call_ctx->replace_initiated_call_id,
                            &p_old_call_ctx,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_old_call_ctx))

    /*
     * Step 2:
     * Remove old call ctx's replace call id
     */
    p_old_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;
  
  
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_fp_cancel_for_replace */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_media_ong_end
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Media Ongoing ended by SC.
 *                  It sends Start Proc Media to CC if it is in Start Proc 
 *                  Media state, else it sends the trigger to SCM if any slp 
 *                  is waiting for it.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_media_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;
    icf_cm_action_st                 *p_action_child = ICF_NULL;
    /* pointers at two levels below root */
    icf_cm_new_check_st              *p_new_check_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_grand = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_MEDIA_ONGOING_ENDED;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_WAIT_FOR_START_PROC_CC)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If CC is in Wait for Start Proc Media state then send to CC */
    /*-------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_CC_13_WAIT_START_PROC_MEDIA,
                            (icf_uint8_t)ICF_CM_OPR_EQUAL,
                            (icf_uint32_t)ICF_START_PROC_MEDIA,
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If CC is not waiting for start proc media, do new check */
    /*-----------------------------------------------------------*/        
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_child->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_next)
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_CC_13_WAIT_START_PROC_MEDIA;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL;
    p_value_action_child->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))
    
    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type, 
            ICF_CM_FT_BP_WAIT_FOR_MEDIA_PROC_SC)
        
    p_field_check_grand->p_value_action_list = ICF_NULL;

    /*--------------------------------------------*/
    /* value_action_grand : 2.1 */
    /* If slp is waiting, then send to SCM   */
    /*--------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_SERVICE_INVALID, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_START_PROC_MEDIA, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
       
    /*------------------------------------*/
    /* value_action_grand : NULL */
    /* end the child level list of values */
    /*------------------------------------*/        
    p_value_action_grand->p_next = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
     

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_media_ong_end */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_sig_ong_end
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Signaling Ongoing ended by SC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_sig_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_SIGNAL_ONGOING_ENDED;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_WAIT_FOR_START_PROC_CC)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If CC is in Wait for Start Proc Sig state then send to CC */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_CC_12_WAIT_START_PROC_SIG,  
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_START_PROC_SIGNALING, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
        

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_sig_ong_end */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_service_ended_normal
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Service ended normal status by SC.
 *                  This function sends Generic Call Clear to CC if call state 
 *                  is idle.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_service_ended_normal(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_SERVICE_ENDED_NORMAL;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_WAIT_FOR_START_PROC_CC)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If CC is in Idle state then send Generic Call Clear to CC */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_CC_00_IDLE,  
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_GENERIC_CALL_CLEAR, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
        

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_service_ended_normal */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_service_err_ind_wo_pld
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Service ended error without payload.
 *                  This function sends Forceful Call Clear to CC if the event 
 *                  was Forceful Call Clear to SCM, else it sends Service Error 
 *                  Indication to CC. 
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_service_err_ind_wo_pld(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_SERVICE_ENDED_ERROR;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_EVENT)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If event was Forceful Clr to SCM, then send Forceful Clr to CC */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND,  
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*------------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If event was not Forceful Clr to SCM, then send Service Err 
     * Indication to CC */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND,  
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_SC_SERVICE_ERROR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_value_action_child->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
        

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_service_err_ind_wo_pld */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_service_err_ind_with_pld
 *
 * DESCRIPTION:     This function defines the value action on SCM backward path 
 *                  for Service error indication with payload.
 *                  This function sets Service Error event in action for CC and 
 *                  also invokes an action function that prepares the payload 
 *                  for the message to be sent to CC. In case this is the 
 *                  return of Forceful Call Clear from SCM, then Forceful Call 
 *                  Clear is sent to CC without any payload but after saving 
 *                  the second call ctx.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_service_err_ind_with_pld(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_SERVICE_ENDED_ERR_WITH_PAYLOAD;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_EVENT)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If event was Forceful Clr to SCM, then send Forceful Clr 
     * to CC for first call. On return path, it will be sent for 
     * second call */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND,  
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /* also set the function to save the second call ctx */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr =
                    icf_cm_bp_sc_service_term_payload;

    /*------------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If event was not Forceful Clr to SCM, then send Service Err 
     * Indication with payload to CC */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND,  
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_SC_SRV_ERR_IND_W_PAYLOAD, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_value_action_child->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /* also set the function to populate the payload */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr =
                    icf_cm_bp_sc_service_term_payload;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
        

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_service_err_ind_with_pld */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_bp_sc_service_term_payload
 *
 * DESCRIPTION:     This function fills the payload on SCM backward path 
 *                  for Service error indication to be sent to CC (for TWC).
 *
 ******************************************************************************/
icf_return_t icf_cm_bp_sc_service_term_payload(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      *p_ecode = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);
    /* copy the received payload in pdb buffer */
    icf_port_memcpy(
           (icf_void_t *)&(p_cm_pdb->buff.service_call_buff),
           (icf_void_t *)p_cm_pdb->p_action_result->internal_msg.p_msg_data,
           (icf_uint32_t)p_cm_pdb->p_action_result->internal_msg.msg_hdr.
                    payload_length);
    
    /* free the received payload memory */
    ICF_MEMFREE(
            p_cm_pdb->p_glb_pdb,
            p_cm_pdb->p_action_result->internal_msg.p_msg_data,
            ICF_MEM_COMMON,
            p_ecode,
            ret_val)
    
    /* 
     * Set the pdb buffer as payload for CC message in case the msg to be sent 
     * is not Forceful Call Clear. It should be set while sending Service Err 
     * Ind with payload to CC.
     */
    if (ICF_FORCEFUL_CALL_CLEAR_IND != 
            p_cm_pdb->p_action_result->internal_msg.msg_hdr.msg_id)
    {
        p_cm_pdb->p_action_result->internal_msg.p_msg_data = 
                (icf_uint8_t *)&(p_cm_pdb->buff.service_call_buff);
        ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_cm_pdb->p_action_result->internal_msg.p_msg_data))
    }
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_bp_sc_service_term_payload */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_cc_handle_reinvite
 *
 * DESCRIPTION:     This function defines the value action on SCM backward path 
 *                  for CC handle reinvite.
 *                  This function invokes a handler function that copies the 
 *                  internal message payload sent from SSA to CHR and sends the 
 *                  same payload on return path to CC. 
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_cc_handle_reinvite(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{   
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_STATUS_CC_HANDLE_REINVITE,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK,
                            ICF_NW_INC_MEDIA_UPDATE,
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_value_action_root),
                            p_ecode))
    {
        *p_p_value_action_root = p_value_action_root;
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }

    /* also set the function to copy the internal msg payload recd from SSA */
    ((icf_cm_action_st *)p_value_action_root->p_action)->presence_field |= 
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)p_value_action_root->p_action)->func_ptr = 
                    icf_cm_bp_sc_cc_handle_reinvite;
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_cc_handle_reinvite */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_bp_sc_cc_handle_reinvite
 *
 * DESCRIPTION:     This function fills the payload on SCM backward path 
 *                  for incoming media update to be sent to CC for handling 
 *                  Re-invite glare scenario. The payload that was sent by CM 
 *                  to CHR needs to be copied and sent on backward path from CM 
 *                  to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_bp_sc_cc_handle_reinvite(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
     
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
   
    /* copy the payload sent to CHR to be sent to CC */

    if(p_cm_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2 & 
        ICF_CC_BUFFERED_INC_MEDIA_UPDATE)
    {
        p_cm_pdb->p_action_result->internal_msg.msg_hdr.msg_id= 
            ICF_NW_INC_MEDIA_UPDATE;
    }
    else
    {
        p_cm_pdb->p_action_result->internal_msg.p_msg_data = 
    	    p_cm_pdb->p_sent_int_msg->p_msg_data;
        ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_cm_pdb->p_action_result->internal_msg.p_msg_data))

        p_cm_pdb->p_action_result->internal_msg.msg_hdr.payload_length = 
            p_cm_pdb->p_sent_int_msg->msg_hdr.payload_length;
    }

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 

} /* End of icf_cm_bp_sc_cc_handle_reinvite */


#ifdef ICF_CM_SCM_GENERIC_CALL_CLR
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_generic_clr
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Generic Call Clear event for SC.
 *                  It checks the pdb bit and sends Forceful Call Clear event 
 *                  to CC if the bit is set. Set bit indicates that this 
 *                  Generic Call Clear that was sent to SCM was triggered by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_generic_clr(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_GENERIC_CALL_CLEAR;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CC_TRIG_GENERIC_CALL_CLR)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If CC triggered event bit is set, define action */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_CM_CC_TRIG_GENERIC_CALL_CLR,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK,
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND,
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;

 
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_generic_clr */
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_call_rel
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  CC Call Released event for SC.
 *                  It checks the pdb bit and sends Generic Call Clear event 
 *                  to CC if the bit is set. Set bit indicates that this 
 *                  Generic Call Clear that was sent to SCM was triggered by 
 *                  ecode Generic Error.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_call_rel(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_CC_CALL_RELEASED;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_ECODE_TRIG_GENERIC_CALL_CLR)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If pdb bit is set, define action */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)
                                ICF_CM_ECODE_TRIG_GENERIC_CALL_CLR,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK,
                            (icf_uint32_t)ICF_GENERIC_CALL_CLEAR,
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;

 
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_sc_bp_va_call_rel */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_sc_bp_va_ecode_generic_err
 *
 * DESCRIPTION:     This function defines the value action on SC backward path 
 *                  for Ecode Generic Error.
 *                  This function sends CC Call Released to SCM and also sets a 
 *                  pdb bit indicating that Generic Call Clear needs to be sent 
 *                  to CC in the return path from SCM. If service ctx ptr is 
 *                  null, it sends Generic Call Clear to CC directly.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_sc_bp_va_ecode_generic_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
    
    ret_val = icf_cm_util_fill_cc_bp_va_ecode_generic_err(
                            p_cm_pdb,
                            p_p_value_action_root,
                            p_ecode);
        
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_fill_sc_bp_va_ecode_generic_err */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_media_ong_end
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Media Ongoing ended by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_media_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_MEDIA_ONGOING_ENDED;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*--------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If Service context is not null, send to ADM */
    /*--------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_START_PROC_MEDIA, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
   

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_media_ong_end */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_sig_ong_end
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Signaling Ongoing ended by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_sig_ong_end(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_SIGNAL_ONGOING_ENDED;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If Service context is not null, send to ADM */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_START_PROC_SIGNALING, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_sig_ong_end */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_med_comtd
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Media Committed status by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_med_comtd(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_MEDIA_COMMITTED;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If Service context is not null, send to ADM */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_SC_INTERNAL_MEDIA_COMMIT, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_med_comtd */
#endif /* #ifdef ICF_SERVICE_ENABLE */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_call_term
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Call Termination status received from CC.
 *                  This function sends Generic Call Clear to SCM if service_ctx
 *                  is not null, else it sends Generic Call Clear to CC if 
 *                  call_ctx is not null.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_call_term(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;
    icf_cm_action_st                 *p_action_child = ICF_NULL;
    /* pointers at two levels below root */
    icf_cm_new_check_st              *p_new_check_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_grand = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_CALL_R_SRV_TERMINATE;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    /* 
     * Send Generic Call Clear to SCM only if service_ctx exists, else 
     * send Generic Call Clear to CC only if call_ctx exists.
     */

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If service_ctx is not null, send Generic Call Clear to ADM */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL,
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_GENERIC_CALL_CLEAR, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

#ifdef ICF_CM_SCM_GENERIC_CALL_CLR
    /* also set the function to set the corresponding bit in pdb */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr = 
                    icf_cm_fp_cc_call_term;
#endif

    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If service_ctx is null, do new check */
    /*-----------------------------------------------------------*/        
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_child->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_next)
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_NULL;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_child->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))
    
    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type, 
            ICF_CM_FT_BP_CHK_CALL_CTX)
        
    p_field_check_grand->p_value_action_list = ICF_NULL;

    /*--------------------------------------------*/
    /* value_action_grand : 2.1 */
    /* If call_ctx is not null, then send to CC   */
    /*--------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_GENERIC_CALL_CLEAR, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
       
    /*------------------------------------*/
    /* value_action_grand : NULL */
    /* end the child level list of values */
    /*------------------------------------*/        
    p_value_action_grand->p_next = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
     

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_fill_cc_bp_va_call_term */


#ifdef ICF_SERVICE_ENABLE
#ifdef ICF_CM_SCM_GENERIC_CALL_CLR
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_cc_call_term
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Call Termination status from CC.
 *                  This function sets a bit in CM pdb that is checked in the 
 *                  return path of Generic Call Clear event from SC to determine
 *                  whether Forceful Call Clear needs to be sent to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_cc_call_term(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
 
    /* set the pdb bit */
    p_cm_pdb->common_bitmask |= ICF_CM_CC_TRIG_GENERIC_CALL_CLR;   

    /* 
     * Reset the status set by CC. This is needed in the following case:
     * (1) CM receives api = User Call Terminate Request and sends it to CC
     * (2) CC sets the status = Call Terminate in its return path
     * (3) CM checks this status and sends Generic Call Clear to ADM
     * (4) CM sends Forceful Call Clear to CC
     * Now, if the status is not reset in the line below, then backward path 
     * of (4) will again trigger CM to send (3). This leads to an infinite loop 
     * between (3) and (4).
     */
    p_cm_pdb->p_action_result->internal_msg.msg_hdr.status = ICF_NULL;
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_fp_cc_call_term */
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_call_fwd
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Call Forward scenarios by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_call_fwd(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            status_value,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_NEW_SERVICE_TRIGGER, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root),
                            p_ecode))
    {
        *p_p_value_action_root = p_value_action_root;
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }

    /* also set the function to populate the received status in payload */
    ((icf_cm_action_st *)(p_value_action_root->p_action))->presence_field |= 
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_root->p_action))->func_ptr = 
                    icf_cm_fp_call_fwd_payload;

        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_call_fwd */


/******************************************************************************
 *
 * FUNCTION:		icf_cm_util_fill_cc_bp_va_twc
 * DESCRIPTION:    This function allocates a node to the routing table of CM 
 *               to route the message on returning path of CC to SCM and finally to TWC 
 ***********************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_twc(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            status_value,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_INVOKE_TWC_FOR_JOIN, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root),
                            p_ecode))
    {
        *p_p_value_action_root = p_value_action_root;
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }

    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_twc */



/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_xconf_ext_connect
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for xconf external conference connect scenario by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_xconf_ext_connect(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            status_value,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_EXTERNAL_CONFERENCE_CONNECTED, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root),
                            p_ecode))
    {
        *p_p_value_action_root = p_value_action_root;
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }

    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_xconf_ext_connect */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_xconf_inc_call
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for xconf incoming conference call scenario by CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_xconf_inc_call(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_uint32_t             status_value,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            status_value,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_INCOMING_EXTERNAL_CONFERENCE_REQ, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root),
                            p_ecode))
    {
        *p_p_value_action_root = p_value_action_root;
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }

    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_xconf_inc_call */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_fp_call_fwd_payload
 *
 * DESCRIPTION:     This function fills the payload on CC backward path 
 *                  for Call Forward scenarios to be sent to SCM.
 *
 ******************************************************************************/
icf_return_t icf_cm_fp_call_fwd_payload(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_service_trigger_st           payload;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    p_cm_pdb->p_action_result->internal_msg.msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_service_trigger_st);

    /* set the received status in the local payload */
    payload.status = p_cm_pdb->p_action_result->internal_msg.msg_hdr.status;

    /* copy the payload in pdb buffer */
    icf_port_memcpy(
            (icf_void_t *)&(p_cm_pdb->buff.service_trig_buff),
            (icf_void_t *)&payload,
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_service_trigger_st));

    /* set the pdb buffer in payload of SCM message */
    p_cm_pdb->p_action_result->internal_msg.p_msg_data = 
            (icf_uint8_t *)&(p_cm_pdb->buff.service_trig_buff);
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_cm_pdb->p_action_result->internal_msg.p_msg_data))
        
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_fp_call_fwd_payload */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_invite_resp
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for 200 OK of Invite received from SSA.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_invite_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;
    icf_cm_action_st                 *p_action_child = ICF_NULL;
    /* pointers at two levels below root */
    icf_cm_new_check_st              *p_new_check_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_grand = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_NW_INC_CONNECT;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If service ctx is not null, do new check */
    /*-----------------------------------------------------------*/
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_field_check_child->p_value_action_list,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_field_check_child->p_value_action_list)
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_NULL;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL;
    p_value_action_child->p_next = ICF_NULL;

    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)

    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))

    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type,
            ICF_CM_FT_BP_ACTIVE_SERVICE_LST)

    p_field_check_grand->p_value_action_list = ICF_NULL;
   
    /*-----------------------------------------------------------*/
    /* value_action_grand : 1.1 */
    /* If Refer is currently ongoing, send to ADM */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_SERVICE_RNW, 
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_NW_INC_CONNECT, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
       
    /*------------------------------------*/
    /* value_action_grand : NULL */
    /* end the child level list of values */
    /*------------------------------------*/  
    p_value_action_grand->p_next = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_invite_resp */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_dms_resp
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  DMS response received from MMI by CC.
 *                  CM shall check if the replace call id is present in call
 *                  context of DMS response. If present, this function shall 
 *                  invoke an action routine that does further processing of 
 *                  received message and sends Initiate Call to CC for new ctx.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_dms_resp(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_DELETE_MEDIA_RESP;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_FP_REPLACE_INITIATED_CALL)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If replace_initiated call id is set, define action */
    /*-----------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INVALID_CALL_ID,
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_INTERNAL_INITIATE_CALL, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /* 
     * also set the action as execution of a function that processes the 
     * received message
     */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr = 
                    icf_cm_cc_bp_dms_resp_for_replace;
        
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;

 
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_dms_resp */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_cc_bp_dms_resp_for_replace
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  DMS response received from MMI by CC.
 *                  CM shall check if the replace call id is present in call 
 *                  context of the DMS response received by CC. If present, 
 *                  this function does the following:
 *                  (1) Find the call ctx corresponding to the replace call id 
 *                      (new call ctx; current one in glb_pdb is old call ctx).
 *                  (2) Replace the call ctx in glb_pdb with new call ctx so 
 *                      that Initiate Call is sent to CC with new call ctx
 *                  (3) Remove old and new call ctx's replace call id.
 *                  (4) Set a pdb flag indicating that an action needs to be 
 *                      taken on the backward path of Initiate Call being sent.
 *
 ******************************************************************************/
icf_return_t icf_cm_cc_bp_dms_resp_for_replace(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      *p_ecode = ICF_ERROR_NONE;
    icf_call_ctx_st                  *p_old_call_ctx = ICF_NULL;
    icf_call_ctx_st                  *p_new_call_ctx = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
    
    /* copy the ecode pointer locally */
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_ecode))
        
    /* copy the received old call's context pointer locally */
    p_old_call_ctx = p_cm_pdb->p_glb_pdb->p_call_ctx;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_old_call_ctx))

    /* 
     * Step 1:
     * Find the new call ctx corresponding to the replace call id
     */
    if (ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                            p_cm_pdb->p_glb_pdb,
                            p_old_call_ctx->replace_initiated_call_id,
                            &p_new_call_ctx,
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_call_ctx))

    /*
     * Step 2:
     * Replace the call ctx in glb_pdb with new call ctx
     */
    p_cm_pdb->p_glb_pdb->p_call_ctx = p_new_call_ctx;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_cm_pdb->p_glb_pdb->p_call_ctx))

    /*
     * Step 3:
     * Remove old and new call ctx's replace call id
     */
    p_old_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;
    /*p_new_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;*/

    /*
     * Step 4:
     * Set the pdb flag indicating that this Initiate Call is being sent due to 
     * Invite with Replace and not due to Refer from network
     */
    p_cm_pdb->common_bitmask |= ICF_CM_REPLACE_INITIATED_CALL;
        
        
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_cc_bp_dms_resp_for_replace */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_initiate_call
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Initiate Call event.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_initiate_call(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_INTERNAL_INITIATE_CALL;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_REPLACE_INITIATED_CALL)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*-----------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If Initiate Call event was triggered by Invite with Replace, then 
     * this flag is true and send PA Connect to CC */
    /*-----------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_CM_REPLACE_INITIATED_CALL, 
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_USER_CALL_CONNECT, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
       
    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_initiate_call */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_service_err_ind
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Service error indication with payload.
 *                  This function sets Service Error event in action for CC and 
 *                  also invokes an action function that sets call ctx pointer 
 *                  for the message to be sent to CC for second call.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_service_err_ind(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_SC_SRV_ERR_IND_W_PAYLOAD,
                            (icf_uint8_t)ICF_CM_OPR_EQUAL,
                            (icf_uint32_t)ICF_SC_SERVICE_ERROR_IND,
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_value_action_root),
                            p_ecode))
    {
        *p_p_value_action_root = p_value_action_root;
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }

    /* also set the function to populate the call_ctx pointer from payload */
    ((icf_cm_action_st *)(p_value_action_root->p_action))->presence_field |= 
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_root->p_action))->func_ptr = 
                    icf_cm_bp_cc_service_term_payload;

        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_service_err_ind */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_bp_cc_service_term_payload
 *
 * DESCRIPTION:     This function sets the call ctx pointer from the payload of 
 *                  Service Error indication on CC backward path in global pdb 
 *                  to be sent to CC for Service Error Ind of second call 
 *                  (for TWC).
 *
 ******************************************************************************/
icf_return_t icf_cm_bp_cc_service_term_payload(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
   
    /* Replace the call ctx in glb_pdb with call ctx for second call */
    p_cm_pdb->p_glb_pdb->p_call_ctx = 
            p_cm_pdb->buff.service_call_buff.p_other_call_ctx;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_cm_pdb->p_glb_pdb->p_call_ctx))

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_bp_cc_service_term_payload */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_forceful_clear
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Forceful Call Clear.
 *                  This function sets Forceful Call Clear in action for CC if 
 *                  the call ctx ptr in service_call_buff is not null. It 
 *                  also invokes an action function that sets call ctx pointer 
 *                  for the message to be sent to CC for second call.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_forceful_clear(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_FORCEFUL_CALL_CLEAR_IND;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_SERV_CALL_BUFF)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If the call ctx ptr in service_ctx buff is not null, then 
     * send Forceful Clr to CC for second call */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL,  
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /* also set the function to get the second call ctx */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr =
                    icf_cm_bp_cc_service_term_payload;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
        
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_forceful_clear */

/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_inc_call_term
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Incoming Call Terminate Request.
 *                  This function sends Generic Call Clear to SCM if service 
 *                  context exists. This will be used on return path of CC 
 *                  after CC has sent dms_req but before CC sends 
 *                  call_term_ind/resp to app so that response to application 
 *                  for service is sent before response for call.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_inc_call_term(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_NW_INC_CALL_TERMINATE;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
    
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If the service_ctx ptr is not null, then send Generic Call 
     * Clr to SCM */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL,  
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_GENERIC_CALL_CLEAR, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
        
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val; 
} /* End of icf_cm_util_fill_cc_bp_va_inc_call_term */

#endif /* #ifdef ICF_SERVICE_ENABLE */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_fill_cc_bp_va_ecode_generic_err
 *
 * DESCRIPTION:     This function defines the value action on CC backward path 
 *                  for Ecode Generic Error.
 *                  This function sends CC Call Released to SCM and also sets a 
 *                  pdb bit indicating that Generic Call Clear needs to be sent 
 *                  to CC in the return path from SCM. If service ctx ptr is 
 *                  null, it sends Generic Call Clear to CC directly.
 *
 ******************************************************************************/
icf_return_t icf_cm_util_fill_cc_bp_va_ecode_generic_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        INOUT   icf_cm_value_action_st   **p_p_value_action_root,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    /* pointers at root level */
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;
    icf_cm_action_st                 *p_action_root = ICF_NULL;
    /* pointers at one level below root */
    icf_cm_new_check_st              *p_new_check_child = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_child = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_child = ICF_NULL;
    icf_cm_action_st                 *p_action_child = ICF_NULL;
    /* pointers at two levels below root */
    icf_cm_new_check_st              *p_new_check_grand = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_grand = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_grand = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_root,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
        
    /* save the newly assigned address to be sent to the calling function */
    *p_p_value_action_root = p_value_action_root;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != *p_p_value_action_root))
    
    p_value_action_root->presence_field = ICF_CM_VALUE;
    p_value_action_root->value = ICF_STATUS_GENERIC_CALL_CLEAR;
    p_value_action_root->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_root->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_root;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_root->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_root->p_action)
    p_action_root = p_value_action_root->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_root))

    /* 
     * Send CC Call Released to SCM only if service_ctx exists, else 
     * send Generic Call Clear to CC.
     */

    p_action_root->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for sub-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_root->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_root->p_new_check)
    p_new_check_child = p_action_root->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_child))
    
    p_new_check_child->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_child->p_next = ICF_NULL;
    /* allocate memory for sub-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_child->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_child->p_field_check)
    p_field_check_child = p_new_check_child->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_child))

    ICF_CM_SET_FIELD_TYPE(p_field_check_child->field_type, 
            ICF_CM_FT_BP_CHK_SERVICE_CTX)

    p_field_check_child->p_value_action_list = ICF_NULL;

    /*------------------------------------------------------------*/
    /* value_action_child : 1 */
    /* If service_ctx is not null, send CC Call Release to ADM */
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL,
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_CC_CALL_RELEASED, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_field_check_child->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_child = p_field_check_child->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))

    /* also set the function to set the corresponding bit in pdb */
    ((icf_cm_action_st *)(p_value_action_child->p_action))->presence_field |=
                    ICF_CM_ACTION_FUNC_PTR;
    ((icf_cm_action_st *)(p_value_action_child->p_action))->func_ptr = 
                    icf_cm_bp_ecode_generic_err;

    /*-----------------------------------------------------------*/
    /* value_action_child : 2 */
    /* If service_ctx is null, do new check */
    /*-----------------------------------------------------------*/        
    /* allocate memory to the next value_action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_value_action_st),
            p_value_action_child->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_next)
    p_value_action_child = p_value_action_child->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_child))
        
    p_value_action_child->presence_field = ICF_CM_VALUE;
    p_value_action_child->value = ICF_NULL;
    p_value_action_child->operation = (icf_uint8_t)ICF_CM_OPR_EQUAL;
    p_value_action_child->p_next = ICF_NULL;
 
    p_cm_pdb->p_value_action = p_value_action_child;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
    
    /* allocate memory to the action pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_action_st),
            p_value_action_child->p_action,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_value_action_child->p_action)
    p_action_child = p_value_action_child->p_action;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action_child))

    p_action_child->presence_field = ICF_CM_ACTION_NEW_CHK;
    /* allocate memory for grand-new_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_action_child->p_new_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,p_action_child->p_new_check)
    p_new_check_grand = p_action_child->p_new_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_grand))
    
    p_new_check_grand->presence_field = ICF_CM_FIELD_CHECK;
    p_new_check_grand->p_next = ICF_NULL;
    /* allocate memory for grand-field_check */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_grand->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    ICF_KLOCWORK_FALSE_POSITIVE(p_cm_pdb->p_glb_pdb,
            p_new_check_grand->p_field_check)
    p_field_check_grand = p_new_check_grand->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_grand))

    ICF_CM_SET_FIELD_TYPE(p_field_check_grand->field_type, 
            ICF_CM_FT_BP_CHK_CALL_CTX)
        
    p_field_check_grand->p_value_action_list = ICF_NULL;

    /*--------------------------------------------*/
    /* value_action_grand : 2.1 */
    /* If call_ctx is not null, then send to CC   */
    /*--------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NULL, 
                            (icf_uint8_t)ICF_CM_OPR_NOT_EQUAL, 
                            (icf_uint32_t)ICF_GENERIC_CALL_CLEAR,  
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_grand->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_grand = p_field_check_grand->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_grand))
       
    /*------------------------------------*/
    /* value_action_grand : NULL */
    /* end the child level list of values */
    /*------------------------------------*/        
    p_value_action_grand->p_next = ICF_NULL;

    /*-----------------------------------------------------------*/
    /* value_action_child : NULL */
    /* end the child level list of values */
    /*-----------------------------------------------------------*/        
    p_value_action_child->p_next = ICF_NULL;
     

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_fill_cc_bp_va_ecode_generic_err */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_bp_ecode_generic_err
 *
 * DESCRIPTION:     This function defines the value action on backward path for 
 *                  Ecode Generic Error from CC.
 *                  This function sets a bit in CM pdb that is checked in the 
 *                  return path of CC Call Released event from SC to determine
 *                  whether Generic Call Clear needs to be sent to CC.
 *
 ******************************************************************************/
icf_return_t icf_cm_bp_ecode_generic_err(
        INOUT   icf_cm_pdb_st            *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
 
    /* set the pdb bit */
    p_cm_pdb->common_bitmask |= ICF_CM_ECODE_TRIG_GENERIC_CALL_CLR;
    
    /* reset the ecode to avoid infinite looping */
    p_cm_pdb->p_action_result->internal_msg.msg_hdr.status = ICF_NULL;

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_bp_ecode_generic_err */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_util_delete_nodes
 *
 * DESCRIPTION:     This function recursively deletes all the new checks. It 
 *                  goes inside each new_check to see if the check is a field 
 *                  check. In that case, it further traverses the list of 
 *                  value_action(s) and deletes all action(s). In case an 
 *                  action is further new_check, this same function is called 
 *                  recursively.
 *                  
 *                  The following figure illustrates the hierarchy to be freed:
 *
 *                  icf_cm_init()-> icf_cm_util_delete_nodes()
 *                  ------------------------------------------------
 *                  p_new_check                 <- icf_cm_util_delete_nodes()
 *                  |
 *                  ----p_field_check
 *                      |
 *                      ----p_value_action_list (list)
 *                          |
 *                          ----p_action
 *                              |
 *                              ----p_new_check -> icf_cm_util_delete_nodes()
 *
 ******************************************************************************/
icf_return_t  icf_cm_util_delete_nodes(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        OUT     icf_cm_new_check_st  *p_new_check,
        OUT     icf_error_t          *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_field_check_st            *p_field_check = ICF_NULL;
    icf_cm_value_action_st           *p_value_action = ICF_NULL;
    icf_cm_value_action_st           *p_next_value_action = ICF_NULL;
    icf_cm_action_st                 *p_action = ICF_NULL;

    icf_cm_new_check_st              *p_sub_new_check = ICF_NULL;
    icf_cm_new_check_st              *p_next_sub_new_check = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* Check to end recursion */
    if (ICF_NULL == p_new_check)
    {
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    } /* End of if (p_new_check) */
    
    /* Check if the presence_field is ICF_CM_FIELD_CHECK */
    if (ICF_NULL != p_new_check->p_field_check)
    {
        p_field_check = p_new_check->p_field_check;
    
        /* 
         * If p_field_check is not null, then we need to free the 
         * value_actions and action pointers
         */
        p_value_action = p_field_check->p_value_action_list;

        /* traverse the list of all possible values this field can take */
        while (ICF_NULL != p_value_action)
        {
            p_action = p_value_action->p_action;
            
            if (ICF_NULL != p_action)
            {
                p_cm_pdb->p_value_action = p_value_action;
                ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_VALUE_ACTION)
                p_cm_pdb->p_action = p_action;
                ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_ACTION)

                /*
                 * If p_action->p_new_check is not null, then call this 
                 * function again to free it recursively
                 */
                /* Check if the presence_field is ICF_CM_ACTION_NEW_CHK */
                if (ICF_NULL != p_action->p_new_check)
                {
                    p_sub_new_check = p_action->p_new_check;

                    /* Free all the new_check nodes in the list */
                    while (ICF_NULL != p_sub_new_check)
                    {
                        /* hold the pointer to next sub_new_check */
                        p_next_sub_new_check = p_sub_new_check->p_next;

                        /* Invoke recursive function to delete all sub-checks */
                        icf_cm_util_delete_nodes(
                                p_cm_pdb, 
                                p_sub_new_check, 
                                p_ecode);

                        /* point to the next sub_new_check to be freed */
                        p_sub_new_check = p_next_sub_new_check;

                    } /* End of while (p_sub_new_check) */

                } /* End of if (p_action->p_new_check) */

                /* Free the action pointer */
                ICF_STATIC_MEMFREE(
                        p_cm_pdb->p_glb_pdb, 
                        p_action, 
                        p_ecode, 
                        ret_val)
            } /* End of if (p_action) */

            /* hold the pointer to next value_action */
            p_next_value_action = p_value_action->p_next;
            
            /* Free the value_action pointer */
            ICF_STATIC_MEMFREE(
                    p_cm_pdb->p_glb_pdb, 
                    p_value_action, 
                    p_ecode, 
                    ret_val)

            /* point to the next value_action to be freed */
            p_value_action = p_next_value_action;
            
        } /* End of while (p_value_action) */

        /* Free the field_check pointer */
        ICF_STATIC_MEMFREE(
                p_cm_pdb->p_glb_pdb, 
                p_field_check, 
                p_ecode, 
                ret_val)
    
    } /* End of if (p_new_check->p_field_check) */

    /* Free the new_check pointer */
    ICF_STATIC_MEMFREE(
            p_cm_pdb->p_glb_pdb, 
            p_new_check, 
            p_ecode, 
            ret_val)
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_util_delete_nodes */

