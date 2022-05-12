/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_types.h
 *
 * Purpose          : This file contains the interface structures and there 
 *                    enumerated values used by CM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 22-Nov-2004  R Ezhirpavai    ICF LLD      Initial
 * 21-Jan-2005  Rohit Aggarwal                  Beginning of coding phase
 * 01-Feb-2005  Rohit Aggarwal                  Added a flag in pdb
 * 17-May-2005  Rohit Aggarwal                  Fixed SPR 7567
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CM_TYPES_H__
#define __ICF_CM_TYPES_H__


#include "icf_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_cm_defs.h"

/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */


/*---------------------------- Internal Data types ---------------------------*/

/*------------------------------------------------------------------------------
 * Datatypes for getting the event, dst and internal payload 
 *----------------------------------------------------------------------------*/

typedef icf_uint8_t icf_cm_check_status_t;

typedef icf_uint8_t icf_cm_check_type_t;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_action_result_st
 * Description      : Structure that is used by CM to track the processing of 
 *                    an internal message received by it
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* destination module for the current internal message received by CM */
    icf_int_module_id_t              dest;
    
    /* check status of CM's processing of the received internal message */
    icf_cm_check_status_t            status;
    
    /* internal message that will be finally sent by CM to CC or ADM */
    icf_internal_msg_st              internal_msg;
    
} icf_cm_action_result_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_data_st
 * Description      : Structure that defines CM's global data
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* 
     * array of pointers to icf_cm_new_check_st structure:
     * p_new_check[0] is pointer for forward message,
     * p_new_check[1] is pointer for backward message from SC,
     * p_new_check[2] is pointer for backward message from CC
     */
    icf_void_t                       *p_new_check[ICF_CM_CHECK_TYPE_MAX];
    
} icf_cm_data_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_buffer_union_ut
 * Description      : Static buffer that can be used for sending the payload
 *
 *----------------------------------------------------------------------------*/
typedef union
{
    icf_service_call_data_st            service_call_buff;
    icf_service_trigger_st              service_trig_buff;

} icf_cm_buffer_union_ut;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_pdb_st
 * Description      : Structure that defines CM's process data block
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* 
     * pointer to structure that tracks the processing that needs to be 
     * ultimately carried out for this message
     */
    icf_cm_action_result_st          *p_action_result;
    
    /* pointer to internal message received by CM's entry point function */
    icf_internal_msg_st              *p_recv_int_msg;
    
    /* pointer to internal message being sent by CM to CC's/ADM's entry pt fn */
    icf_internal_msg_st              *p_sent_int_msg;
    
    /* local pointer to CM's global data */
    icf_cm_data_st                   *p_cm_data;
    
    /* local pointer to global pdb */
    icf_glb_pdb_st                   *p_glb_pdb;

    /* bitmask whose bits correspond to various flags explained below */
    /* 
     * bit that indicates whether the Initiate Call event sent by CM to CC was 
     * due to Invite with Replace or not. In case the Initiate Call was sent due
     * to Invite with Replace, this bit will be set, else it will be unset 
     * (for Refer triggered Initiate Call event). 
     * This will help in deciding in CC's backward path of Initiate Call event 
     * whether PA connect should be sent to CC or not (it should be sent only 
     * in case the event was triggered by Invite with Replace header).
     */
#define ICF_CM_REPLACE_INITIATED_CALL        0x01
    /*
     * bit that indicates whether Generic Call Clear event sent by CM to SCM 
     * was on the backward path of CC's Call Termination status or not. In case 
     * Generic Call Clear was sent to SCM on the backward path of CC, then this 
     * bit will be set, else it will be unset.
     * This will help in deciding in SCM's backward path of Generic Call Clear 
     * event whether Forceful Call Clear needs to be sent to CC or not.
     */
#define ICF_CM_CC_TRIG_GENERIC_CALL_CLR      0x02
    /*
     * bit that indicates whether Generic Call Clear event sent by CM to SCM 
     * was on the backward path of CC's Call Termination status or not. In case 
     * Generic Call Clear was sent to SCM on the backward path of CC, then this 
     * bit will be set, else it will be unset.
     * This will help in deciding in SCM's backward path of Generic Call Clear 
     * event whether Forceful Call Clear needs to be sent to CC or not.
     */
#define ICF_CM_ECODE_TRIG_GENERIC_CALL_CLR   0x04
    /*
     * bit that indicates that the ADM message for PA was constructed by CM and 
     * that the memory for same should be freed by CM on return from ADM
     */
#define ICF_CM_FREE_PA_FAILURE_MSG           0x08
    icf_uint8_t                      common_bitmask;

    /* contains buffer array that is used for transferring payload in msgs */
    icf_cm_buffer_union_ut           buff;
        
    /* contains the checktype that is used to know the current path */
    icf_cm_check_type_t              check_type;

#if 0
    /* call id used in message flow handling */
    icf_call_id_t                    call_id;
#endif
    
    /* 
     * The following are used mainly for traces.
     */
    icf_uint16_t                     field_type;
    icf_uint32_t                     field_value;
    icf_uint8_t                      presence_field;
    icf_uint8_t                      operation;
    icf_void_t                       *p_value_action;
    icf_void_t                       *p_action;
    
} icf_cm_pdb_st;


/*------------------------------------------------------------------------------
 * The intension of the following data driven check is as depicted below:
 * First with help of new check find if the required field contains a 
 * particular value.
 * For a value, it is in turn defined as to what is the action to be done
 * when that field contains this value. The action can be one of the following:
 * - Generate an internal event to the respective module
 * - Further check if any other field has a particular value
 * - Check/do some complex thing based on a user defined function.
 * Thus finally for set of field value combination there would be
 * a leaf level action of sending message to another module or just
 * set a particular value for any field from within the user defined function.
 * So we get a tree of actions with root as one field value.
 * We can create multiple roots or multiple branches by definiing new checks
 * either at root level or at action level.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Function pointer definition for proprietary functions for verification
 * of the input message against the current values of different fields.
 *----------------------------------------------------------------------------*/
typedef    icf_return_t    (* icf_cm_ctx_chk_func_ptr_t)
                               (icf_cm_pdb_st *p_pdb);

                               
/*------------------------------------------------------------------------------
 * Function pointer definition for proprietary functions for defining an 
 * action.
 *----------------------------------------------------------------------------*/
typedef    icf_return_t    (* icf_cm_action_func_ptr_t)
                               (icf_cm_pdb_st *p_pdb);


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_value_action_st
 * Description      : Structure that defines the action for a particular value 
 *                    of the field
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_cm_value_action_s
{
    /* 
     * flag that indicates whether a particular value of a field is to be 
     * looked for or not
     */
    icf_uint8_t                      presence_field;
#define ICF_CM_VALUE                 0x01

    /* If value is not set, then it implies any value */
    icf_uint32_t                     value;
    
    /* operation for comparison of value */
    icf_uint8_t                      operation;
#define ICF_CM_OPR_EQUAL             1
#define ICF_CM_OPR_NOT_EQUAL         2
#define ICF_CM_OPR_BIT_SET_CHK       3
#define ICF_CM_OPR_BIT_UNSET_CHK     4
    
    /* 
     * pointer to icf_cm_action_st structure that defines the action to be 
     * taken in case this field check for a particular value of a particular 
     * node is successful. This action might result in dispatching the message 
     * to CC or ADM, or it may result in another check at the sub-level of the 
     * tree (i.e., checking the children of current node which corresponds to 
     * another new_check)
     */
    icf_void_t                       *p_action;
    
    /* 
     * In case the field check fails for the current node, this is the next 
     * node at the same level of the tree that needs to be checked
     */
    struct icf_cm_value_action_s     *p_next;
    
} icf_cm_value_action_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_field_check_st
 * Description      : Structure that defines the pointer for the field for 
 *                    which a particular action is taken
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* 
     * Correspondig to each field type a field is extracted in separate
     * macro which would be used in handling function
     */
    icf_uint16_t                     field_type;
#define ICF_CM_FT_FP_START                          1
#define ICF_CM_FT_FP_INT_MSG_RCVD                   ICF_CM_FT_FP_START+0
#define ICF_CM_FT_FP_INT_MSG_SENT                   ICF_CM_FT_FP_START+1
#define ICF_CM_FT_FP_API                            ICF_CM_FT_FP_START+2
#define ICF_CM_FT_FP_EVENT                          ICF_CM_FT_FP_START+3
#define ICF_CM_FT_FP_MEDIA_ERROR_CAUSE              ICF_CM_FT_FP_START+4
#define ICF_CM_FT_FP_MEDIA_ONG_IN_SC                ICF_CM_FT_FP_START+5
#define ICF_CM_FT_FP_DST_IN_TMR_BUF                 ICF_CM_FT_FP_START+6
#define ICF_CM_FT_FP_REPLACE_INITIATED_CALL         ICF_CM_FT_FP_START+7
#define ICF_CM_FT_FP_ACTIVE_SERVICE_LST             ICF_CM_FT_FP_START+8
#define ICF_CM_FT_FP_CHK_SERVICE_CTX                ICF_CM_FT_FP_START+9
#define ICF_CM_FT_FP_CHK_CALL_CTX                   ICF_CM_FT_FP_START+10
#define ICF_CM_FT_FP_SIG_ONG_MODULE                 ICF_CM_FT_FP_START+11

#define ICF_CM_FT_BP_START                          32
#define ICF_CM_FT_BP_STATUS                         ICF_CM_FT_BP_START+0
#define ICF_CM_FT_BP_WAIT_FOR_MEDIA_PROC_SC         ICF_CM_FT_BP_START+1
#define ICF_CM_FT_BP_WAIT_FOR_SIG_PROC_SC           ICF_CM_FT_BP_START+2
#define ICF_CM_FT_BP_WAIT_FOR_START_PROC_CC         ICF_CM_FT_BP_START+3
#define ICF_CM_FT_BP_EVENT                          ICF_CM_FT_BP_START+4
#define ICF_CM_FT_BP_ACTIVE_SERVICE_LST             ICF_CM_FT_BP_START+5
#define ICF_CM_FT_BP_REPLACE_INITIATED_CALL         ICF_CM_FT_BP_START+6
#define ICF_CM_FT_BP_CC_TRIG_GENERIC_CALL_CLR       ICF_CM_FT_BP_START+7
#define ICF_CM_FT_BP_CHK_SERVICE_CTX                ICF_CM_FT_BP_START+8
#define ICF_CM_FT_BP_CHK_CALL_CTX                   ICF_CM_FT_BP_START+9
#define ICF_CM_FT_BP_ECODE_TRIG_GENERIC_CALL_CLR    ICF_CM_FT_BP_START+10
#define ICF_CM_FT_BP_SERV_CALL_BUFF                 ICF_CM_FT_BP_START+11
#define ICF_CM_FT_BP_STATUS_2                       ICF_CM_FT_BP_START+12

    /* pointer to the field value check that will be ultimately performed */
    icf_cm_value_action_st           *p_value_action_list;
    
} icf_cm_field_check_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_new_check_st
 * Description      : Structure that defines a new check which is the root 
 *                    level check and also used for checks at the branch level 
 *                    from within action
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_cm_new_check_s
{
    /* 
     * flag that indicates whether a field value needs to be matched directly 
     * or a function needs to be executed to check a complex condition
     */
    icf_uint8_t                      presence_field;
#define ICF_CM_FIELD_CHECK           0x01
#define ICF_CM_CHK_WITH_FUNC         0x02
    
    /* direct field check to be performed in case of ICF_CM_FIELD_CHECK */
    icf_cm_field_check_st            *p_field_check;
    
    /* function check to be performed in case of ICF_CM_CHK_WITH_FUNC */
    icf_cm_ctx_chk_func_ptr_t        func_ptr;
   
    /* 
     * pointer to the next check at this level of the tree (At the topmost 
     * level, this is currently null since all checks are performed for msg_id 
     * only) 
     */
    struct icf_cm_new_check_s        *p_next;
    
} icf_cm_new_check_st; 


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cm_action_st
 * Description      : Structure that defines the action to be taken when the 
 *                    value matches for a field (i.e., condition is satisfied)
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* presence flag that specifies the kind of action to be taken */
    icf_uint8_t                      presence_field;
#define ICF_CM_ACTION_EVT_DST        0x01
#define ICF_CM_ACTION_NEW_CHK        0x02
#define ICF_CM_ACTION_FUNC_PTR       0x04
    
    /* 
     * 'internal event (msg_id) that needs to be finally sent to CC/ADM' 
     * -> in case the action to be taken for the current check is 
     * ICF_CM_ACTION_EVT_DST
     */
    icf_uint32_t                      event;
    
    /* 
     * 'destination module for this message - CC or ADM' 
     * -> in case the action to be taken for the current check is 
     * ICF_CM_ACTION_EVT_DST
     */
    icf_int_module_id_t              dest;
    
    /* 
     * 'pointer to new check that needs to be performed' 
     * -> in case the action to be taken for the current check is a sub-check 
     * ICF_CM_ACTION_NEW_CHK. 
     * This takes the checking from one level of the tree recursively to its 
     * sub-level.
     */
    icf_cm_new_check_st              *p_new_check;
    
    /* 
     * 'pointer to the user-defined function that needs to be executed' 
     * -> in case the action to be taken for the current check is 
     * ICF_CM_ACTION_FUNC_PTR
     */
    icf_cm_action_func_ptr_t         func_ptr;
    
} icf_cm_action_st;



/*---------------------- End of types for data setting -----------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif  /* End of __ICF_CM_TYPES_H__ */
