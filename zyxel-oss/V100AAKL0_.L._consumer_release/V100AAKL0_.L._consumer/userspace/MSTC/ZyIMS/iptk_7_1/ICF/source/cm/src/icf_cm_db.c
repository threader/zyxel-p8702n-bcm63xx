/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_db.c
 *
 * Purpose          : This file contains functions that are used for setting
 *                    rules for routing the messages in forward and backward
 *                    directions in Call Manager module's global data.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 25-Nov-2004  R Ezhirpavai    ICF LLD      Initial
 * 21-Jan-2005  Rohit Aggarwal                  Beginning of coding phase
 * 31-Jan-2005  Rohit Aggarwal                  Added handling for CF and CW
 * 25-Feb-2005  Rohit Aggarwal                  Added a hash define 
 *                                              ICF_CM_SCM_GENERIC_CALL_CLR 
 *                                              to remove handlng Generic Call 
 *                                              Clear event on SCM's return path
 * 21-Apr-2005  Rohit Aggarwal                  Fixed SPR 7420
 * 17-May-2005  Rohit Aggarwal                  Fixed SPR 7567
 * 25-May-2005  Rohit Aggarwal                  Fixed SPR 7600
 * 16-Jul-2007  Abhishek Dhammawat ICF7.0 CDD   XCONF feature additions      
 * 17-Jul-2007 Abhishek Dhammawat ICF7.0 CDD    JOIN header changes
 * 06-Feb-2009   Rohit Aggarwal  SPR 19316   	Changes for INVITE with Replaces
 *
 * Copyright (c) 2007, Aricent.
 *----------------------------------------------------------------------------*/



#include "icf_internal_api_id.h"
#include "icf_cm_prototypes.h"
#include "icf_cm_macro.h"


#ifdef ICF_SERVICE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cm_db_set_forward_data
 *
 * DESCRIPTION:     This function is used to set the rules for routing the
 *                  messages in forward direction into the module's global data.
 *                  It checks on events or APIs to see where to route the
 *                  message.
 *                  For doing a routing, CM goes through the tree structure in
 *                  its global data to find where the appropriate check matches
 *                  with the received message. Once it finds that, it does the
 *                  action associated with the same. In case it does not get
 *                  appropriate check for routing the message in forward
 *                  direction, it routes the message to Call Control as that is
 *                  the default destination for all messages.
 *
 ******************************************************************************/
icf_return_t icf_cm_db_set_forward_data(
        INOUT   icf_cm_pdb_st        *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      *p_ecode = ICF_NULL;
    icf_cm_new_check_st              *p_new_check_root = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_root = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);
    /*
     * Currently, there is only one root level new_check. 
     * However, inside this new_check, there will be multiple value_action(s) 
     * for multiple possible msg_id values received by CM in the internal 
     * message. The list of all these possible value_action(s) that are 
     * children of the same parent is contained in field_check.
     * For each of the msg_id that matches, there can be an action defining a 
     * further new_check that will have further value_action(s) for different 
     * low-level checks.
     */
    
    /*------------------------------------------------------------------------*/
    /*------------------- Root level new_check pointer -----------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to global data for FORWARD messages */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_new_check_st), 
            p_cm_pdb->p_cm_data->p_new_check[ICF_CM_CHECK_TYPE_FORWARD], 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_new_check_root = 
                p_cm_pdb->p_cm_data->p_new_check[ICF_CM_CHECK_TYPE_FORWARD];
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_root))

    /* Field checks will be performed on the msg_id field */
    p_new_check_root->presence_field = ICF_CM_FIELD_CHECK;

    /* 
     * As of now, there will be no other node at this root level. In other 
     * words, there will be no other new_check node at this starting level
     */
    p_new_check_root->p_next = ICF_NULL;

    
    /*------------------------------------------------------------------------*/
    /*------------------- Root level field_check pointer ---------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to the field check pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_field_check_st), 
            p_new_check_root->p_field_check, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_field_check_root = p_new_check_root->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_root))

    /* Checks will be performed on msg_id type field at root level */
    ICF_CM_SET_FIELD_TYPE(p_field_check_root->field_type, 
            ICF_CM_FT_FP_INT_MSG_RCVD)

    p_field_check_root->p_value_action_list = ICF_NULL;
    
    /*------------------------------------------------------------------------*/
    /*
     * The functionality till this point is common but now value_action 
     * pointers will be created depending on the number of checks for the field
     */
    /*------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /* Action for Refer from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NW_INC_REFER, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_REFER, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_field_check_root->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_field_check_root->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Forceful Call Clear */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_forceful_clr(
                            p_cm_pdb, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
    /*------------------------------------------------------------------------*/
    /* Action for Message from MRM (i.e., from PA) */
    /*------------------------------------------------------------------------*/
    /* Handling: check the external api_id and take action accordingly */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_msg_frm_mrm(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
        
/*#ifdef IMS_CLIENT*/
/*ICF_CHECK_IMS_START(p_cm_pdb->p_glb_pdb,ICF_TRUE)*/
    /*------------------------------------------------------------------------*/
    /* Action for Response (200 OK) or Error Response for Reinvite from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message to request initiating module */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_media_update_resp(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
/*ICF_CHECK_IMS_END*/
/*#endif*/
#if 0
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_cm_pdb->p_glb_pdb,ICF_FALSE)
    /*------------------------------------------------------------------------*/
    /* Action for Response (200 OK) or Error Response for Reinvite from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE_RESP,
                            (icf_uint8_t)ICF_CM_OPR_EQUAL,
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE_RESP,
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Error Response for Reinvite from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE_RESP,
                            (icf_uint8_t)ICF_CM_OPR_EQUAL,
                            (icf_uint32_t)ICF_NW_INC_MEDIA_UPDATE_RESP,
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

ICF_CHECK_IMS_END
#endif
#endif /* if 0 */
    
    /*------------------------------------------------------------------------*/
    /* Action for Response for Refer from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NW_INC_REFER_RESP, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_REFER_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
      
    /*------------------------------------------------------------------------*/
    /* Action for Error Response for Refer from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NW_INC_REFER_RESP, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_REFER_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
#if 0     
    /*------------------------------------------------------------------------*/
    /* Action for Cancel for Refer from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NW_INC_REFER_ABORT, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_REFER_ABORT, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
#endif        
    /*------------------------------------------------------------------------*/
    /* Action for Notify from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NW_INC_NOTIFY_IND, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_NOTIFY_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
          
    /*------------------------------------------------------------------------*/
    /* Action for Response (200 OK) for Notify from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_NW_INC_NOTIFY_RESP, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_NW_INC_NOTIFY_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
         
    /*------------------------------------------------------------------------*/
    /* Action for Reinvite from SSA */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: send this to SCM only if CHD is currently active, else send 
     * this message to CC
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_reinvite(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
           
    /*------------------------------------------------------------------------*/
    /* Action for MMS response from MMI */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message to CC if media_ongoing is set to CC, else 
     * send to SCM via ADM
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_mms_resp(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
      
    /*------------------------------------------------------------------------*/
    /* Action for Media Error Ind from MMI */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: If this is a response to a media request, send to the module 
     * that sent the request, else send to CC
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_media_err(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
           
    /*------------------------------------------------------------------------*/
    /* Action for Merge Media Response from MMI */
    /*------------------------------------------------------------------------*/
    /* Handling: send this message directly to SCM via ADM */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_CREATE_CONF_RESP, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_CREATE_CONF_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /* Additions for the XCONF feature. Setting up db for SIC response
       which needs to be fwd to XCONF through SCM. */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INTERNAL_SUBSCRIBE_RESP, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_INTERNAL_SUBSCRIBE_RESP, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_INTERNAL_NOTIFY_IND, 
                            (icf_uint8_t)ICF_CM_OPR_EQUAL, 
                            (icf_uint32_t)ICF_INTERNAL_NOTIFY_IND, 
                            (icf_int_module_id_t)ICF_INT_MODULE_ADM, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
       
    /*------------------------------------------------------------------------*/
    /* Action for Setup with Replace from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: This is needed in handling Invite with Replace.
     * In forward path of Setup req from SSA, CM shall check if the replace 
     * call id is present in setup req, it shall find the call ctx 
     * corresponding to the replace call id (called as old call ctx and the 
     * current one in glb_pdb is named as new call ctx), and update the old 
     * call ctx's replace call id with new call id. CM shall also check if the 
     * call status of the old call ctx has the local call held status as true, 
     * it shall update the local call held status in new call ctx too. CM shall 
     * now send PA initiated call terminate to CC after filling the old call 
     * ctx in glb_pdb.
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_setup(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
          
    /*------------------------------------------------------------------------*/
    /* Action for Cancel from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: This is needed in handling Invite with Replace
     * In forward path if Cancel from network is received on a call id, CM 
     * shall check the call ctx (new call ctx) in glb_pdb to see if it contains 
     * replace call id, it shall first find the call ctx (old call ctx) 
     * corresponding to the replace call id of this call ctx (new call ctx). 
     * It shall remove the replace call id from the old call ctx. It shall 
     * send Forceful Call Clear to CC for the received (new) call.
     */
    if (ICF_FAILURE == icf_cm_util_fill_fp_va_cancel(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
        
    /*------------------------------------------------------------------------*/
    /* End the root level list of values */
    /*------------------------------------------------------------------------*/
    p_value_action_root->p_next = ICF_NULL;
    
    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_db_set_forward_data */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_db_set_sc_backward_data
 *
 * DESCRIPTION:     This function sets the rules for re-routing the received
 *                  message or forming a new message after getting back the
 *                  control from Service Controller and routing back to
 *                  Call Control or Service Controller.
 *                  In case the CM does not get the appropriate leaf node that
 *                  matches the received status or the event in backward
 *                  direction, it implies that there is no action required for
 *                  this status and it returns back to the calling function.
 *
 ******************************************************************************/
icf_return_t icf_cm_db_set_sc_backward_data(
        INOUT   icf_cm_pdb_st        *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      *p_ecode = ICF_NULL;
    icf_cm_new_check_st              *p_new_check_root = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_root = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)
    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);

    /*------------------------------------------------------------------------*/
    /*------------------- Root level new_check pointer -----------------------*/
    /* this is for status field */
    /*------------------------------------------------------------------------*/
    /* Allocate memory to global data for BACKWARD messages */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_new_check_st), 
            p_cm_pdb->p_cm_data->p_new_check[ICF_CM_CHECK_TYPE_BACKWARD_SC], 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_new_check_root = 
            p_cm_pdb->p_cm_data->p_new_check[ICF_CM_CHECK_TYPE_BACKWARD_SC];
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_root))

    /* Field checks will be performed on the status field */
    p_new_check_root->presence_field = ICF_CM_FIELD_CHECK;
   
    /* Init the pointer to next new check */ 
    p_new_check_root->p_next = ICF_NULL;

    /*------------------------------------------------------------------------*/
    /*------------------- Root level field_check pointer ---------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to the field check pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_field_check_st), 
            p_new_check_root->p_field_check, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_field_check_root = p_new_check_root->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_root))

    /* Checks will be performed on status field at root level */
    ICF_CM_SET_FIELD_TYPE(p_field_check_root->field_type, 
            ICF_CM_FT_BP_STATUS)

    p_field_check_root->p_value_action_list = ICF_NULL;
    
    /*------------------------------------------------------------------------*/
    /* Action for Status = Generic Call Clear */
    /*------------------------------------------------------------------------*/
    /* Handling: send Call Release to SCM followed by Generic Call Clr to CC */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_ecode_generic_err(
                            p_cm_pdb,
                            &(p_field_check_root->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_field_check_root->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
    /*------------------------------------------------------------------------*/
    /* Action for Status = Media Ongoing end */
    /*------------------------------------------------------------------------*/
    /* Handling: send trigger to CC if it is in media wait state, else send 
     * trigger to SCM if any service is waiting for media
     */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_media_ong_end(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
    /*------------------------------------------------------------------------*/
    /* Action for Status = Signaling Ongoing end */
    /*------------------------------------------------------------------------*/
    /* Handling: send trigger to CC if it is in signaling wait state */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_sig_ong_end(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
     
    /*------------------------------------------------------------------------*/
    /* Action for Status = Service Ended Normal */
    /*------------------------------------------------------------------------*/
    /* Handling: send Generic Call Clear to CC if call state is idle */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_service_ended_normal(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
    /*------------------------------------------------------------------------*/
    /* Action for Status = Service Error without payload */
    /*------------------------------------------------------------------------*/
    /* Handling: If this is returning from event Forceful Call Clear to SCM, 
     * then send Forceful Call Clear to CC, else send Service Err Ind to CC
     */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_service_err_ind_wo_pld(
                            p_cm_pdb, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
       
    /*------------------------------------------------------------------------*/
    /* Action for Status = Service Error with payload */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: send service error event to CC twice, one for each call. This 
     * is done as follows:
     * (1) When SC gives status = 'Service Error with payload', CM sends Event =
     *     'Service Error Ind With Payload' to CC for one call id
     * (2) On return path of (1), CM sends Event = 'Service Error Ind Without 
     *     Payload' to CC for second call id (obtained from payload of (1))
     * In case this is the return path of Forceful Call Clear from SCM, then 
     * Forceful Call Clear is sent to CC for both calls.
     */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_service_err_ind_with_pld(
                            p_cm_pdb, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Status = Call Waiting */
    /*------------------------------------------------------------------------*/
    /* Handling: forward the call waiting indication to CC */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_STATUS_CALL_WAITING,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_CALL_WAIT_CONTINUE, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
     
    /*------------------------------------------------------------------------*/
    /* Action for Status = Initiate Call */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: This is needed in handling Refer with/without replace header. 
     * In this case, send Initiate Call event to CC.
     */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb, 
                            (icf_uint32_t)ICF_STATUS_INITIATE_CALL,
                            (icf_uint8_t)ICF_CM_OPR_BIT_SET_CHK, 
                            (icf_uint32_t)ICF_INTERNAL_INITIATE_CALL, 
                            (icf_int_module_id_t)ICF_INT_MODULE_CC, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Status = Handle Reinvite */
    /*------------------------------------------------------------------------*/
    /* Handling: CHR is busy, forward Reinvite glare to CC for processing */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_cc_handle_reinvite(
                            p_cm_pdb, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

        
    /*------------------------------------------------------------------------*/
    /* End the root level list of values */
    /*------------------------------------------------------------------------*/
    p_value_action_root->p_next = ICF_NULL;

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------- Root level new_check pointer -----------------------*/
    /* this is for event/msg_id field */
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to global data for BACKWARD messages */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_new_check_st),
            p_new_check_root->p_next,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    p_new_check_root = p_new_check_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_root))

    /* Field checks will be performed on the msg_id field */
    p_new_check_root->presence_field = ICF_CM_FIELD_CHECK;

    /* there are no new_checks other than for status and event fields */
    p_new_check_root->p_next = ICF_NULL;

    /*------------------------------------------------------------------------*/
    /*------------------- Root level field_check pointer ---------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to the field check pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_cm_field_check_st),
            p_new_check_root->p_field_check,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val)
    p_field_check_root = p_new_check_root->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_root))

    /* Checks will be performed on msg_id type field at root level */
    ICF_CM_SET_FIELD_TYPE(p_field_check_root->field_type,
            ICF_CM_FT_BP_EVENT)

    p_field_check_root->p_value_action_list = ICF_NULL;
    
    /*------------------------------------------------------------------------*/
    /* Action for Event = Forceful Call Clear */
    /*------------------------------------------------------------------------*/
    /* Handling: send forceful call clear indication to CC */
    if (ICF_FAILURE == icf_cm_util_fill_value_action(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND,
                            (icf_uint8_t)ICF_CM_OPR_EQUAL,
                            (icf_uint32_t)ICF_FORCEFUL_CALL_CLEAR_IND,
                            (icf_int_module_id_t)ICF_INT_MODULE_CC,
                            &(p_field_check_root->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_field_check_root->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))


#ifdef ICF_CM_SCM_GENERIC_CALL_CLR
    /*------------------------------------------------------------------------*/
    /* Action for Event = Generic Call Clear */
    /*------------------------------------------------------------------------*/
    /* Handling: send forceful call clear indication to CC if pdb bit is set */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_generic_clr(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
#endif
        
    /*------------------------------------------------------------------------*/
    /* Action for Event = CC Call Released */
    /*------------------------------------------------------------------------*/
    /* Handling: send generic call clear indication to CC if pdb bit is set */
    if (ICF_FAILURE == icf_cm_util_fill_sc_bp_va_call_rel(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
        
    /*------------------------------------------------------------------------*/
    /* End the root level list of values */
    /*------------------------------------------------------------------------*/
    p_value_action_root->p_next = ICF_NULL;

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_db_set_sc_backward_data */
#endif /* #ifdef ICF_SERVICE_ENABLE */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_db_set_cc_backward_data
 *
 * DESCRIPTION:     This function sets the rules for re-routing the received
 *                  message or forming a new message after getting back the
 *                  control from Call Control and routing back to Call Control
 *                  or Service Controller.
 *                  In case the CM does not get the appropriate leaf node that
 *                  matches the received status or the event in backward
 *                  direction, it implies that there is no action required for
 *                  this status and it returns back to the calling function.
 *
 ******************************************************************************/
icf_return_t icf_cm_db_set_cc_backward_data(
        INOUT   icf_cm_pdb_st        *p_cm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      *p_ecode = ICF_NULL;
    icf_cm_new_check_st              *p_new_check_root = ICF_NULL;
    icf_cm_field_check_st            *p_field_check_root = ICF_NULL;
    icf_cm_value_action_st           *p_value_action_root = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cm_pdb->p_glb_pdb)

    p_ecode = &(p_cm_pdb->p_recv_int_msg->msg_hdr.ecode);
	    
    /*------------------------------------------------------------------------*/
    /*------------------- Root level new_check pointer -----------------------*/
    /* this is for status field */
    /*------------------------------------------------------------------------*/
    /* Allocate memory to global data for BACKWARD messages */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_new_check_st), 
            p_cm_pdb->p_cm_data->p_new_check[ICF_CM_CHECK_TYPE_BACKWARD_CC], 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_new_check_root = 
            p_cm_pdb->p_cm_data->p_new_check[ICF_CM_CHECK_TYPE_BACKWARD_CC];
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_root))

    /* Field checks will be performed on the status field */
    p_new_check_root->presence_field = ICF_CM_FIELD_CHECK;
    
    /* Init the pointer to next new check */ 
    p_new_check_root->p_next = ICF_NULL;

    /*------------------------------------------------------------------------*/
    /*------------------- Root level field_check pointer ---------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to the field check pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_field_check_st), 
            p_new_check_root->p_field_check, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_field_check_root = p_new_check_root->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_root))

    /* Checks will be performed on msg_id type field at root level */
    ICF_CM_SET_FIELD_TYPE(p_field_check_root->field_type, 
            ICF_CM_FT_BP_STATUS_2)

    p_field_check_root->p_value_action_list = ICF_NULL;
    
    /*------------------------------------------------------------*/
    /*Action for status = XCONF (external conference) connected   */    
    /*------------------------------------------------------------*/
#ifdef ICF_SERVICE_ENABLE
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_xconf_ext_connect(
                            p_cm_pdb,
                            (icf_uint32_t)
                                ICF_STATUS_EXTERNAL_CONFERENCE_CONNECTED,
                            &(p_field_check_root->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_field_check_root->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
    /*------------------------------------------------------------*/
    /*Action for status = XCONF , incoming conference request     */    
    /*------------------------------------------------------------*/
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_xconf_inc_call(
                            p_cm_pdb,
                            (icf_uint32_t)
                                ICF_STATUS_INCOMING_EXTERNAL_CONF_CALL,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /* Start: JOIN Header Changes */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_twc(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_STATUS_INVOKE_TWC_FOR_JOIN,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    /* End: JOIN Header Changes */
#endif       
    
    /*------------------------------------------------------------------------*/
    /* End the root level list of values */
    /*------------------------------------------------------------------------*/
    p_value_action_root->p_next = ICF_NULL;
  
    /* Allocate memory to global data for BACKWARD messages */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_new_check_st), 
            p_new_check_root->p_next, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_new_check_root = p_new_check_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_root))

    /* Field checks will be performed on the msg_id field */
    p_new_check_root->presence_field = ICF_CM_FIELD_CHECK;

    /* there are no new_checks other than for status and event fields */
    p_new_check_root->p_next = ICF_NULL;

    /*------------------------------------------------------------------------*/
    /*------------------- Root level field_check pointer ---------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to the field check pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_field_check_st), 
            p_new_check_root->p_field_check, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_field_check_root = p_new_check_root->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_root))

    /* Checks will be performed on status field at root level */
    ICF_CM_SET_FIELD_TYPE(p_field_check_root->field_type, 
            ICF_CM_FT_BP_STATUS)

    p_field_check_root->p_value_action_list = ICF_NULL;
    
    /*------------------------------------------------------------------------*/
    /* Action for Status = Generic Call Clear */
    /*------------------------------------------------------------------------*/
    /* Handling: send Call Release to SCM followed by Generic Call Clr to CC */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_ecode_generic_err(
                            p_cm_pdb,
                            &(p_field_check_root->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_field_check_root->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
#ifdef ICF_SERVICE_ENABLE
    /*------------------------------------------------------------------------*/
    /* Action for Status = Media Committed */
    /*------------------------------------------------------------------------*/
    /* Handling: send media commit ind to SCM if service ctx is not null */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_med_comtd(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
     
    /*------------------------------------------------------------------------*/
    /* Action for Status = Media Ongoing end */
    /*------------------------------------------------------------------------*/
    /* Handling: send trigger to SCM if service context is not null */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_media_ong_end(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
    /*------------------------------------------------------------------------*/
    /* Action for Status = Signaling Ongoing end */
    /*------------------------------------------------------------------------*/
    /* Handling: send trigger to SCM if service context is not null */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_sig_ong_end(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
#endif /* #ifdef ICF_SERVICE_ENABLE */

    /*------------------------------------------------------------------------*/
    /* Action for Status = Call Termination */
    /*------------------------------------------------------------------------*/
    /* Handling: send generic call clear to SCM and set the pdb bit */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_call_term(
                            p_cm_pdb, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
 
#ifdef ICF_SERVICE_ENABLE
    /*------------------------------------------------------------------------*/
    /* Action for Status = Call Forwarding Unconditional */
    /*------------------------------------------------------------------------*/
    /* Handling: send new service trigger to SCM with this status in payload */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_call_fwd(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_STATUS_CALL_FWD_UNCOND,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Status = Call Forwarding No Answer */
    /*------------------------------------------------------------------------*/
    /* Handling: send new service trigger to SCM with this status in payload */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_call_fwd(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_STATUS_CALL_FWD_NO_ANS,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
  
    /*------------------------------------------------------------------------*/
    /* Action for Status = Call Forwarding Busy Line */
    /*------------------------------------------------------------------------*/
    /* Handling: send new service trigger to SCM with this status in payload */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_call_fwd(
                            p_cm_pdb,
                            (icf_uint32_t)ICF_STATUS_CALL_FWD_BUSY_LINE,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Status = Call Forwarding Busy and Call Waiting service */
    /*------------------------------------------------------------------------*/
    /* Handling: send new service trigger to SCM with this status in payload */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_call_fwd(
                            p_cm_pdb,
                            (icf_uint32_t)
                                ICF_STATUS_CALL_FWD_BUSY_N_CALL_WAITING,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
        
#endif /* #ifdef ICF_SERVICE_ENABLE */

    /*------------------------------------------------------------------------*/
    /* End the root level list of values */
    /*------------------------------------------------------------------------*/
    p_value_action_root->p_next = ICF_NULL;


#ifdef ICF_SERVICE_ENABLE
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------- Root level new_check pointer -----------------------*/
    /* this is for event/msg_id field */
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to global data for BACKWARD messages */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_new_check_st), 
            p_new_check_root->p_next, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_new_check_root = p_new_check_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_new_check_root))

    /* Field checks will be performed on the msg_id field */
    p_new_check_root->presence_field = ICF_CM_FIELD_CHECK;

    /* there are no new_checks other than for status and event fields */
    p_new_check_root->p_next = ICF_NULL;
    
    /*------------------------------------------------------------------------*/
    /*------------------- Root level field_check pointer ---------------------*/
    /*------------------------------------------------------------------------*/
    /* Allocate memory to the field check pointer */
    ICF_STATIC_MEMGET(
            p_cm_pdb->p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_field_check_st), 
            p_new_check_root->p_field_check, 
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    p_field_check_root = p_new_check_root->p_field_check;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_field_check_root))

    /* Checks will be performed on msg_id type field at root level */
    ICF_CM_SET_FIELD_TYPE(p_field_check_root->field_type, 
            ICF_CM_FT_BP_EVENT)

    p_field_check_root->p_value_action_list = ICF_NULL;
    
    /*------------------------------------------------------------------------*/
    /* Action for Event = 200 OK of Invite from SSA */
    /*------------------------------------------------------------------------*/
    /* Handling: send connect to SCM if Refer is ongoing */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_invite_resp(
                            p_cm_pdb,
                            &(p_field_check_root->p_value_action_list),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_field_check_root->p_value_action_list;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
    
    /*------------------------------------------------------------------------*/
    /* Action for Event = DMS response from MMI */
    /*------------------------------------------------------------------------*/
    /* Handling: This is needed in handling Invite with Replace
     * In backward path of CC for DMS resp, CM shall check if the replace call 
     * id is present in the call ctx of glb_pdb (old call ctx), it shall find 
     * the call ctx corresponding to the replace call id (new call ctx). Now it 
     * shall send Initiate call event after replacing the call ctx as new call 
     * ctx in glb_pdb. It shall also update a flag in CM pdb that Initiate call 
     * return path handling to be done. CM shall also remove the replace call 
     * id from old and new call ctx.
     */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_dms_resp(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
            
	/* Fix for CSR 1-6727350: SPR 19316 */
	/* This event from CM to CC is no longer required because on getting CMS
	 * response, CC will send connect to SSA on its own.
	 */
#if 0
    /*------------------------------------------------------------------------*/
    /* Action for Event = Initiate Call */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: This is needed in handling Invite with Replace. 
     * If the pdb's bit for handling Initiate Call is true, send PA connect 
     * to CC.
     */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_initiate_call(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))
#endif
        
    /*------------------------------------------------------------------------*/
    /* Action for Event = Service Error Ind with payload */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: send service error event to CC twice, one for each call. This 
     * is done as follows:
     * (1) When SC gives status = 'Service Error with payload', CM sends Event =
     *     'Service Error Ind With Payload' to CC for one call id
     * (2) On return path of (1), CM sends Event = 'Service Error Ind Without 
     *     Payload' to CC for second call id (obtained from payload of (1))
     */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_service_err_ind(
                            p_cm_pdb, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Event = Forceful Call Clear */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: send service error event to CC twice, one for each call. This 
     * is done as follows:
     * (1) When SC gives status = 'Service Error with payload', CM sends Event =
     *     'Service Error Ind With Payload' to CC for one call id
     * (2) On return path of (1), CM sends Event = 'Service Error Ind Without 
     *     Payload' to CC for second call id (obtained from payload of (1))
     * In case this is the return path of Forceful Call Clear from SCM, then 
     * Forceful Call Clear is sent to CC for both calls. The following handles 
     * the scenario for Forceful clear.
     */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_forceful_clear(
                            p_cm_pdb, 
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* Action for Event = Incoming Call Terminate from SSA */
    /*------------------------------------------------------------------------*/
    /* 
     * Handling: send Generic Call Clear to SCM if service_ctx exists. This is 
     * needed to send failure responses for app service requests before CC 
     * sends call terminate indication/response to the application.
     */
    if (ICF_FAILURE == icf_cm_util_fill_cc_bp_va_inc_call_term(
                            p_cm_pdb,
                            &(p_value_action_root->p_next),
                            p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
        return (ICF_FAILURE);
    }
    p_value_action_root = p_value_action_root->p_next;
    ICF_ASSERT(p_cm_pdb->p_glb_pdb, (ICF_NULL != p_value_action_root))

    /*------------------------------------------------------------------------*/
    /* End the root level list of values */
    /*------------------------------------------------------------------------*/
    p_value_action_root->p_next = ICF_NULL;
    

#endif /* #ifdef ICF_SERVICE_ENABLE */

    ICF_FUNCTION_EXIT(p_cm_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_cm_db_set_cc_backward_data */
