/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_process.c
 *
 * Purpose          : This file contains functions that handle processing logic
 *                    of CM in forward and backward directions.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 25-Nov-2004  R Ezhirpavai    ICF LLD      Initial
 * 21-Jan-2005  Rohit Aggarwal                  Beginning of coding phase
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_dbm_prototypes.h"
#include "icf_cm_prototypes.h"
#include "icf_cm_macro.h"


#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_forward_msg
 *
 * DESCRIPTION:     This function is called on forward direction so that it can
 *                  check for actions in its forward path. The action function
 *                  is same, but only the root level tree node structure is
 *                  different, hence same function is called indicating from
 *                  which tree it has to be searched.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_forward_msg(
        INOUT   icf_cm_pdb_st        *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    p_cm_pdb->check_type = (icf_cm_check_type_t)
                                ICF_CM_CHECK_TYPE_FORWARD;
    ret_val = icf_cm_proc_check_action(p_cm_pdb, p_cm_pdb->check_type);
                    /*(icf_cm_check_type_t)ICF_CM_CHECK_TYPE_FORWARD);*/
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_process_forward_msg */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_sc_backward_msg
 *
 * DESCRIPTION:     This function is called on return path of call from Service
 *                  Controller so that it can check for actions in its return
 *                  path. The action function is same, but only the root level
 *                  tree node structure is different, hence same function is
 *                  called indicating from which tree it has to be searched.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_sc_backward_msg(
        INOUT   icf_cm_pdb_st        *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    p_cm_pdb->check_type = (icf_cm_check_type_t)
                                ICF_CM_CHECK_TYPE_BACKWARD_SC;
    ret_val = icf_cm_proc_check_action(p_cm_pdb, p_cm_pdb->check_type);
                    /*(icf_cm_check_type_t)ICF_CM_CHECK_TYPE_BACKWARD_SC);*/

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_process_sc_backward_msg */
#endif /* #ifdef ICF_SERVICE_ENABLE */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_process_cc_backward_msg
 *
 * DESCRIPTION:     This function is called on return path of call from Call
 *                  Control so that it can check for actions in its return path.
 *                  The action function is same, but only the root level tree
 *                  node structure is different, hence same function is called
 *                  indicating from which tree it has to be searched.
 *
 ******************************************************************************/
icf_return_t icf_cm_process_cc_backward_msg(
        INOUT   icf_cm_pdb_st        *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    p_cm_pdb->check_type = (icf_cm_check_type_t)
                                ICF_CM_CHECK_TYPE_BACKWARD_CC;
    ret_val = icf_cm_proc_check_action(p_cm_pdb, p_cm_pdb->check_type);
                    /*(icf_cm_check_type_t)ICF_CM_CHECK_TYPE_BACKWARD_CC);*/

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_process_cc_backward_msg */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_check_action
 *
 * DESCRIPTION:     This function gets the pointer of the root of the tree
 *                  where the data driven logic for routing is kept. Once the
 *                  root level node is identified, it calls a recursive
 *                  function to find the action.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_check_action(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_cm_check_type_t  check_type)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_cm_new_check_st              *p_new_check = ICF_NULL;
    icf_error_t                      ecode = ICF_ERROR_NONE;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* 
     * Initially status is set to continue so that subsequent new checks can be 
     * searched in case the corresponding field has not been verified. When it 
     * is set to ended, then stop further checking.
     */
    ICF_CM_SET_STATUS(p_cm_pdb, ICF_CM_CHECK_STATUS_CONT)

    /* Get CM's global data from DBM */
    ret_val = icf_dbm_get_module_glb_data(
                p_cm_pdb->p_glb_pdb, 
                (icf_int_module_id_t)ICF_INT_MODULE_CM, 
                (icf_void_t *)&(p_cm_pdb->p_cm_data), 
                &ecode);

    if (ICF_SUCCESS == ret_val)
    {
        /* 
         * This shall retrieve the first pointer of the new check
         * node so that we can start looking from that check onwards to see 
         * which of them satisfy the current message received.
         */
        ICF_CM_GET_NEW_CHECK_FRM_CM_DATA(p_cm_pdb, p_new_check, check_type)

        /* 
         * The following function is a recursive function that is called to 
         * find the leaf level node that would satisfy the current message 
         * against the condition mentioned. Once the condition match leaf level 
         * node is identified, it would perform the action. The action is 
         * performed by setting appropriate value in action value in pdb. The 
         * final action takes place at top level function.
         */
        ret_val = icf_cm_proc_recc_new_check(p_cm_pdb, p_new_check);
    } /* End of if for ret_val */

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_proc_check_action */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_recc_new_check
 *
 * DESCRIPTION:     This function forms the recursion that checks from the
 *                  root level structure to find the appropriate branch of tree
 *                  which satisfies the condition.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_recc_new_check(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_cm_new_check_st  *p_new_check)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* 
     * This function gets the pointer to the new check from which searching has
     * to start. So it checks until all subsequent checks are checked to
     * finally arive at a check which statisfies the condition.
     */

    while ((ICF_NULL != p_new_check) &&
           (ICF_SUCCESS == ret_val) &&
           (ICF_INT_MODULE_INVALID == p_cm_pdb->p_action_result->dest) &&
           (ICF_CM_CHECK_STATUS_CONT == p_cm_pdb->p_action_result->status))
    {
        ret_val = icf_cm_proc_a_new_check(p_cm_pdb, p_new_check);

        /* 
         * The next check is taken so that if the current check did not 
         * give a valid matching we can start from next check onwards. This 
         * check is done on the next node at the same level of the tree.
         */
        p_new_check = p_new_check->p_next;
    
        /* ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_ACTION_RESULT) */
        
    } /* End of while of new checks at root level */

    /* 
     * If the top level field itself does not match, this function would scan 
     * across multiple linked new checks. If the top level field matches, then 
     * it goes inside the tree to next level. In next level there can be 
     * further more level of checks. To do their checking, again this same 
     * function is called from subsequent functions and hence forms a recursive 
     * function call.
     */

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_proc_recc_new_check */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_a_new_check
 *
 * DESCRIPTION:     This function checks against the field of the tree node to
 *                  find which action has to be processed. If the complete
 *                  check fails then it returns so that the next field can be
 *                  checked from the current level.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_a_new_check(
        INOUT   icf_cm_pdb_st        *p_cm_pdb,
        IN      icf_cm_new_check_st  *p_new_check)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint32_t                     field_value = ICF_NULL;
    icf_cm_value_action_st           *p_value_action = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* 
     * This function is a recursive function that would find for a condition
     * match and then break from checking. It would also stop when there are 
     * no more conditions to match.
     */

    p_cm_pdb->presence_field = p_new_check->presence_field;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_PRESENCE_FIELD)
        
    if (ICF_CM_CHK_WITH_FUNC & p_new_check->presence_field)
    {
        /* 
         * If function is to be called, then execute the function that would 
         * set the action result in pdb and hence simply return because this 
         * means that there is no more link in this branch of tree.
         */
        ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
                (ICF_NULL != p_new_check->func_ptr))
        ret_val = p_new_check->func_ptr(p_cm_pdb);

        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return ret_val;
    } /* End of if for check with function */

    /* Else it implies that the ICF_CM_FIELD_CHECK is set */
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_CM_FIELD_CHECK & p_new_check->presence_field))
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_new_check->p_field_check))
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
            (ICF_NULL != p_new_check->p_field_check->p_value_action_list))

    ICF_CM_GET_FIELD_PTR(
            p_cm_pdb, 
            field_value, 
            p_new_check->p_field_check->field_type)

    p_cm_pdb->field_type = p_new_check->p_field_check->field_type;
    p_cm_pdb->field_value = field_value;
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_FIELD_DATA)
    
    /* --------Start loop for value action list ---------- */
    p_value_action = p_new_check->p_field_check->p_value_action_list;
    
    do
    {
        ret_val = icf_cm_util_check_field_agnst_value(
                                p_cm_pdb, 
                                field_value, 
                                p_value_action);
   
        if (ICF_SUCCESS == ret_val)
        {
            /* 
             * Do as per the action data, which can further call back this same 
             * function depending on the new_check present or not in action.
             */
            ret_val = icf_cm_proc_do_action(p_cm_pdb, p_value_action);
        }
        else
        {
            /*
             * The ret_val needs to be changed to success since value has not 
             * matched, though there is no failure that has been encountered.
             */
            ret_val = ICF_SUCCESS;
        }
        
        /* 
         * Either Value has not matched or it is posible that top value has 
         * matched, but at some inner level the value has not matched. Hence 
         * check for next value only if the action has not been taken so far. 
         * This check is performed on the next node at the same level of the 
         * tree.
         */
        p_value_action = p_value_action->p_next;

        /* ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_ACTION_RESULT) */
        
    }while ((ICF_NULL != p_value_action) && 
            (ICF_INT_MODULE_INVALID == p_cm_pdb->p_action_result->dest) && 
            (ICF_CM_CHECK_STATUS_CONT == p_cm_pdb->p_action_result->status));

    /* Just return so that we can continue with next check if required */
        
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_proc_a_new_check */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_proc_do_action
 *
 * DESCRIPTION:     This function acts on the action part. This would be called
 *                  if the field values matches. The action could be of
 *                  following types:
 *                  (a) Identify to which destination this message has to be
 *                  routed
 *                  (b) Identify a new function that would do complex job of
 *                  setting some variables or could further do some more checks
 *                  (c) Call another branch level checking with new values
 *                  If the action indicates that new level of checking is
 *                  required, the recursive function is called again so that
 *                  now checking can start from that level of tree.
 *
 ******************************************************************************/
icf_return_t icf_cm_proc_do_action(
        INOUT   icf_cm_pdb_st            *p_cm_pdb,
        IN      icf_cm_value_action_st   *p_value_action)
{
    icf_return_t                     ret_val = ICF_FAILURE;
    icf_cm_action_st                 *p_action = ICF_NULL;
    icf_boolean_t                    checked_recur = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    /* Value has matched and hence take action course */
    p_action = (icf_cm_action_st *)(p_value_action->p_action);
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_action))

    if(ICF_NULL == p_action)
    {
	return ICF_FAILURE;
    }

    do
    {
        /* 
         * If the action indicates event and destination, it implies that we 
         * have found a leaf level node. So now no need to go across other 
         * checks. Simply perform only this action.
         */

        p_cm_pdb->presence_field = p_action->presence_field;
        ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_PRESENCE_FIELD)
        
        if (ICF_CM_ACTION_EVT_DST & p_action->presence_field)
        {
            /* fill the final dest module ID to which msg needs to be sent */
            p_cm_pdb->p_action_result->dest = p_action->dest;
            
            /* fill the final internal message ID which needs to be sent */
            p_cm_pdb->p_action_result->internal_msg.msg_hdr.msg_id =(icf_uint16_t) 
                p_action->event;
           
            /* fill the final msg that needs to be sent to CC/ADM */
            /* 
             * On Forward path, the final msg will be constructed from the 
             * internal msg received by CM entry point function. But, on 
             * Backward path, the final msg will be the same as returned by a 
             * destination module on its return path
             */
            if (ICF_CM_CHECK_TYPE_FORWARD == p_cm_pdb->check_type)
            {
                /* on Forward path, use the msg received by CM entry point */
                /* on Backward path, use the msg returned by dest entry point */
                p_cm_pdb->p_action_result->internal_msg.msg_hdr.payload_length =                     p_cm_pdb->p_recv_int_msg->msg_hdr.payload_length;
                p_cm_pdb->p_action_result->internal_msg.msg_hdr.p_glb_pdb =
                    p_cm_pdb->p_recv_int_msg->msg_hdr.p_glb_pdb;
                p_cm_pdb->p_action_result->internal_msg.msg_hdr.ecode =
                    p_cm_pdb->p_recv_int_msg->msg_hdr.ecode;
                p_cm_pdb->p_action_result->internal_msg.msg_hdr.status =
                    p_cm_pdb->p_recv_int_msg->msg_hdr.status;
                p_cm_pdb->p_action_result->internal_msg.p_msg_data = 
                    p_cm_pdb->p_recv_int_msg->p_msg_data;
            }
            
            ret_val = ICF_SUCCESS;

            /* 
             * There cannot be new check after this as the leaf node of the 
             * tree has the event, though there can be an internal function to 
             * fill any payload data.
             */
            if (ICF_CM_ACTION_FUNC_PTR & p_action->presence_field)
            {
                ret_val = p_action->func_ptr(p_cm_pdb);
            }
            ICF_CM_SET_STATUS(p_cm_pdb, ICF_CM_CHECK_STATUS_END)

        } /* End of if the action was leaf node of the tree */
        else if (ICF_CM_ACTION_NEW_CHK & p_action->presence_field)
        {
           ICF_ASSERT(p_cm_pdb->p_glb_pdb, 
                   (ICF_NULL != p_action->p_new_check))

           /* 
            * If there are other checks as subsequent checks to top level 
            * checks then again call the same recursive function to start from 
            * this level of tree again.
            * This recursive sub-check is done on the children of p_new_check. 
            * The children can be obtained as p_action->p_new_check.
            */
           ret_val = icf_cm_proc_recc_new_check(
                                p_cm_pdb, 
                                p_action->p_new_check);
           checked_recur = ICF_TRUE;

        } /* End of else if the action was new check */

        /* ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_ACTION_RESULT) */
        
    } while ((ICF_NULL != p_action) && 
            (ICF_FALSE == checked_recur) &&
            (ICF_SUCCESS == ret_val) && 
            (ICF_INT_MODULE_INVALID == p_cm_pdb->p_action_result->dest) && 
            (ICF_CM_CHECK_STATUS_CONT == p_cm_pdb->p_action_result->status));

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_proc_do_action */
