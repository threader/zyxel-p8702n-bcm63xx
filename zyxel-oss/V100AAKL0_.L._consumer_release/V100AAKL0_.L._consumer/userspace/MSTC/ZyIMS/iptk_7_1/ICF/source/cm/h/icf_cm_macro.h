/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_macro.h
 *
 * Purpose          : This file contains the macro definitions used by CM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 21-Jan-2005  Rohit Aggarwal  ICF LLD      Beginning of coding phase
 * 14-Mar-2005  Rohit Aggarwal                  ASSERT p_call_ctx before 
 *                                              retreiving a field value
 * 11-Apr-2005  Rohit Aggarwal                  Fixed SPR 7368 and 7375
 * 17-May-2005  Rohit Aggarwal                  Fixed SPR 7567
 * 25-May-2005  Rohit Aggarwal                  Fixed SPR 7600
 * 07-Jul-2005  Amit Sharma                     Check for service_ctx=NULL
 *                                              in ICF_CM_GET_FIELD_PTR
 * 01-Mar-2006  Amit Sharma                     ICF merging with ICF2.0
 * 16-Jul-2007  Abhishek Dhammawat Rel7.0 CDD   Changes for XCONF and Join
 *                                              header.
 * 22-Aug-2007  Amit Sharma                     Merge from PQT_6_5                                             
 * 
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CM_MACRO_H__
#define __ICF_CM_MACRO_H__


#include "icf_macro.h"
#include "icf_internal_api_id.h"
#include "icf_common_interface_types.h"
#ifdef ICF_SERVICE_ENABLE
#include "icf_sc_common_types.h"
#endif /* #ifdef ICF_SERVICE_ENABLE */
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


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CM_TRACE
 * Description      : Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_CM_TRACE(p_cm_pdb, trace_id)
/*#define ICF_CM_TRACE(p_cm_pdb, trace_id) \
{ \
    icf_cm_trace(p_cm_pdb, (icf_trace_id_t)(trace_id)); \
}*/
#else
#define ICF_CM_TRACE(p_cm_pdb, trace_id)
#endif /* End of #ifdef ICF_TRACE_ENABLE */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CM_SET_ECODE
 * Description      : Macro for ecode handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_ERROR_ENABLE
#define ICF_CM_SET_ECODE(p_cm_pdb, ecode) \
{ \
    icf_cm_error(p_cm_pdb, (icf_error_t)(ecode)); \
}
#else
#define ICF_CM_SET_ECODE(p_cm_pdb, error) \
{ \
    p_cm_pdb->p_recv_int_msg->msg_hdr.ecode = (icf_error_t)(error); \
}
#endif /* End of #ifdef ICF_ERROR_ENABLE */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CM_SET_STATUS
 * Description      : Sets the status in action_result
 *
 *----------------------------------------------------------------------------*/
#define ICF_CM_SET_STATUS(p_cm_pdb, new_status) \
{ \
    p_cm_pdb->p_action_result->status = new_status; \
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_STATUS) \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CM_SET_FIELD_TYPE
 * Description      : Sets the field_type in field_check structure
 *
 *----------------------------------------------------------------------------*/
#define ICF_CM_SET_FIELD_TYPE(dest, src) \
{ \
    dest = src; \
    p_cm_pdb->field_type = dest; \
    ICF_CM_TRACE(p_cm_pdb, ICF_MSG_PROC_FIELD_TYPE) \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CM_GET_NEW_CHECK_FRM_CM_DATA
 * Description      : Retreives the first pointer of new check node
 *
 *----------------------------------------------------------------------------*/
#define ICF_CM_GET_NEW_CHECK_FRM_CM_DATA(p_cm_pdb, p_new_check, check_type) \
{ \
    p_new_check = p_cm_pdb->p_cm_data->p_new_check[check_type]; \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CM_GET_FIELD_PTR
 * Description      : Retrieves the value of a field as defined by field_type
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_SERVICE_ENABLE
#define ICF_CM_GET_FIELD_PTR(p_cm_pdb, field_value, type) \
{ \
    ICF_ASSERT( \
            p_cm_pdb->p_glb_pdb, \
            (!( \
             (ICF_MSG_FROM_MRM == p_cm_pdb->p_recv_int_msg->msg_hdr.msg_id) \
             && \
             (!((ICF_PA_CC_CREATE_CALL_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_TIMER_MSG == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_HOLD_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_RESUME_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_STATUS_INVOKE_TWC_FOR_JOIN == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CONFERENCE_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id))) \
             && \
             (ICF_NULL == p_cm_pdb->p_glb_pdb->p_call_ctx) \
             )) \
            ) \
    switch(type) \
    { \
        case ICF_CM_FT_FP_INT_MSG_RCVD: \
            field_value = p_cm_pdb->p_recv_int_msg->msg_hdr.msg_id; \
            break; \
        case ICF_CM_FT_FP_API: \
            field_value = p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id; \
            break; \
        case ICF_CM_FT_FP_MEDIA_ERROR_CAUSE: \
            field_value = ((icf_mm_err_ind_st *)(p_cm_pdb->p_recv_int_msg-> \
                                p_msg_data))->reason; \
            break; \
        case ICF_CM_FT_FP_MEDIA_ONG_IN_SC: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx->media_ongoing; \
            break; \
        case ICF_CM_FT_FP_DST_IN_TMR_BUF: \
            field_value = ((icf_timer_data_st *) \
                                (*((icf_timer_data_st **) \
                                (p_cm_pdb->p_glb_pdb->p_recv_msg->payload)))) \
                                ->module_id; \
            break; \
        case ICF_CM_FT_FP_REPLACE_INITIATED_CALL: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx-> \
                                replace_initiated_call_id; \
            break; \
        case ICF_CM_FT_FP_SIG_ONG_MODULE: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx->signalling_ongoing; \
            break; \
		case ICF_CM_FT_FP_ACTIVE_SERVICE_LST: \
        case ICF_CM_FT_BP_ACTIVE_SERVICE_LST: \
            if (ICF_NULL ==  p_cm_pdb->p_glb_pdb->p_call_ctx->p_service_ctx)\
            { \
                field_value = ICF_SERVICE_INVALID; \
            } \
            else \
            { \
                field_value = ((icf_service_ctx_st *)(p_cm_pdb->p_glb_pdb-> \
                                p_call_ctx->p_service_ctx))-> \
                                active_service_list.active_service_status; \
            } \
            break; \
        case ICF_CM_FT_BP_STATUS: \
            field_value = p_cm_pdb->p_action_result->internal_msg.msg_hdr. \
                                status; \
            break; \
        case ICF_CM_FT_BP_STATUS_2: \
             field_value = p_cm_pdb->p_action_result->internal_msg.msg_hdr. \
                                status_2; \
            break; \
        case ICF_CM_FT_BP_WAIT_FOR_MEDIA_PROC_SC: \
            if (ICF_NULL ==  p_cm_pdb->p_glb_pdb->p_call_ctx->p_service_ctx)\
            { \
                field_value = ICF_SERVICE_INVALID; \
            } \
            else \
            { \
            field_value = ((icf_service_ctx_st *)(p_cm_pdb->p_glb_pdb-> \
                                p_call_ctx->p_service_ctx))-> \
                                expected_events.start_proc_media; \
            }\
            break; \
        /*
        case ICF_CM_FT_BP_WAIT_FOR_SIG_PROC_SC: \
            field_value = ((icf_service_ctx_st *)(p_cm_pdb->p_glb_pdb-> \
                                p_call_ctx->p_service_ctx))-> \
                                expected_events.start_proc_signaling; \
            break; \
        */ \
        case ICF_CM_FT_BP_WAIT_FOR_START_PROC_CC: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx->call_state; \
            break; \
        case ICF_CM_FT_BP_EVENT: \
            field_value = p_cm_pdb->p_sent_int_msg->msg_hdr.msg_id; \
            break; \
        case ICF_CM_FT_BP_REPLACE_INITIATED_CALL: \
        /*
        case ICF_CM_FT_BP_CC_TRIG_GENERIC_CALL_CLR: \
        */ \
        case ICF_CM_FT_BP_ECODE_TRIG_GENERIC_CALL_CLR: \
            field_value = p_cm_pdb->common_bitmask; \
            break; \
        case ICF_CM_FT_FP_CHK_SERVICE_CTX: \
        case ICF_CM_FT_BP_CHK_SERVICE_CTX: \
            { \
                if (ICF_NULL == p_cm_pdb->p_glb_pdb->p_call_ctx) \
                { \
                    field_value = ICF_NULL; \
                } \
                else if (ICF_NULL == \
                        p_cm_pdb->p_glb_pdb->p_call_ctx->p_service_ctx) \
                { \
                    field_value = ICF_NULL; \
                } \
                else \
                { \
                    field_value = ICF_NULL + 1; \
                } \
            } \
            break; \
        case ICF_CM_FT_FP_CHK_CALL_CTX: \
        case ICF_CM_FT_BP_CHK_CALL_CTX: \
            { \
                if (ICF_NULL !=  \
                        p_cm_pdb->p_glb_pdb->p_call_ctx) \
                    field_value = ICF_NULL + 1; \
                else \
                    field_value = ICF_NULL; \
            } \
            break; \
        case ICF_CM_FT_BP_SERV_CALL_BUFF: \
            { \
                if (ICF_NULL !=  \
                        p_cm_pdb->buff.service_call_buff.p_other_call_ctx) \
                    field_value = ICF_NULL + 1; \
                else \
                    field_value = ICF_NULL; \
            } \
            break; \
        default: \
            p_cm_pdb->field_type = type; \
            ICF_CM_SET_ECODE(p_cm_pdb, ICF_MSG_PROC_FIELD_TYPE_MISMATCH) \
            break; \
    } \
}
#else
#define ICF_CM_GET_FIELD_PTR(p_cm_pdb, field_value, type) \
{ \
    ICF_ASSERT( \
            p_cm_pdb->p_glb_pdb, \
            (!( \
             (ICF_MSG_FROM_MRM == p_cm_pdb->p_recv_int_msg->msg_hdr.msg_id) \
             && \
             (!((ICF_PA_CC_CREATE_CALL_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_TIMER_MSG == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_HOLD_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CALL_RESUME_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_CONFERENCE_REQ == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id) \
              || \
             (ICF_PA_CC_REMOTE_CALL_TRANSFER_INITIATED_CFM == \
                p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id))) \
             && \
             (ICF_NULL == p_cm_pdb->p_glb_pdb->p_call_ctx) \
             )) \
            ) \
    switch(type) \
    { \
        case ICF_CM_FT_FP_INT_MSG_RCVD: \
            field_value = p_cm_pdb->p_recv_int_msg->msg_hdr.msg_id; \
            break; \
        case ICF_CM_FT_FP_API: \
            field_value = p_cm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id; \
            break; \
        case ICF_CM_FT_FP_MEDIA_ERROR_CAUSE: \
            field_value = ((icf_mm_err_ind_st *)(p_cm_pdb->p_recv_int_msg-> \
                                p_msg_data))->reason; \
            break; \
        case ICF_CM_FT_FP_MEDIA_ONG_IN_SC: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx->media_ongoing; \
            break; \
        case ICF_CM_FT_FP_DST_IN_TMR_BUF: \
            field_value = ((icf_timer_data_st *) \
                                (*((icf_timer_data_st **) \
                                (p_cm_pdb->p_glb_pdb->p_recv_msg->payload)))) \
                                ->module_id; \
            break; \
        case ICF_CM_FT_FP_REPLACE_INITIATED_CALL: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx-> \
                                replace_initiated_call_id; \
            break; \
        case ICF_CM_FT_FP_SIG_ONG_MODULE: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx->signalling_ongoing; \
            break; \
        /* #ifdef ICF_SERVICE_ENABLE
		case ICF_CM_FT_FP_ACTIVE_SERVICE_LST: \
        case ICF_CM_FT_BP_ACTIVE_SERVICE_LST: \
            if (ICF_NULL ==  p_cm_pdb->p_glb_pdb->p_call_ctx->p_service_ctx)\
            { \
                field_value = ICF_SERVICE_INVALID; \
            } \
            else \
            { \
                field_value = ((icf_service_ctx_st *)(p_cm_pdb->p_glb_pdb-> \
                                p_call_ctx->p_service_ctx))-> \
                                active_service_list.active_service_status; \
            } \
            break; \
		#endif */ \
        case ICF_CM_FT_BP_STATUS: \
            field_value = p_cm_pdb->p_action_result->internal_msg.msg_hdr. \
                                status; \
            break; \
		/* #ifdef ICF_SERVICE_ENABLE
        case ICF_CM_FT_BP_WAIT_FOR_MEDIA_PROC_SC: \
            if (ICF_NULL ==  p_cm_pdb->p_glb_pdb->p_call_ctx->p_service_ctx)\
            { \
                field_value = ICF_SERVICE_INVALID; \
            } \
            else \
            { \
            field_value = ((icf_service_ctx_st *)(p_cm_pdb->p_glb_pdb-> \
                                p_call_ctx->p_service_ctx))-> \
                                expected_events.start_proc_media; \
            }\
            break; \
		#endif */ \
        /*
        case ICF_CM_FT_BP_WAIT_FOR_SIG_PROC_SC: \
            field_value = ((icf_service_ctx_st *)(p_cm_pdb->p_glb_pdb-> \
                                p_call_ctx->p_service_ctx))-> \
                                expected_events.start_proc_signaling; \
            break; \
        */ \
        case ICF_CM_FT_BP_WAIT_FOR_START_PROC_CC: \
            field_value = p_cm_pdb->p_glb_pdb->p_call_ctx->call_state; \
            break; \
        case ICF_CM_FT_BP_EVENT: \
            field_value = p_cm_pdb->p_sent_int_msg->msg_hdr.msg_id; \
            break; \
        case ICF_CM_FT_BP_REPLACE_INITIATED_CALL: \
        /*
        case ICF_CM_FT_BP_CC_TRIG_GENERIC_CALL_CLR: \
        */ \
        case ICF_CM_FT_BP_ECODE_TRIG_GENERIC_CALL_CLR: \
            field_value = p_cm_pdb->common_bitmask; \
            break; \
        case ICF_CM_FT_FP_CHK_SERVICE_CTX: \
        case ICF_CM_FT_BP_CHK_SERVICE_CTX: \
            { \
                if (ICF_NULL == p_cm_pdb->p_glb_pdb->p_call_ctx) \
                { \
                    field_value = ICF_NULL; \
                } \
                else if (ICF_NULL == \
                        p_cm_pdb->p_glb_pdb->p_call_ctx->p_service_ctx) \
                { \
                    field_value = ICF_NULL; \
                } \
                else \
                { \
                    field_value = ICF_NULL + 1; \
                } \
            } \
            break; \
        case ICF_CM_FT_FP_CHK_CALL_CTX: \
        case ICF_CM_FT_BP_CHK_CALL_CTX: \
            { \
                if (ICF_NULL !=  \
                        p_cm_pdb->p_glb_pdb->p_call_ctx) \
                    field_value = ICF_NULL + 1; \
                else \
                    field_value = ICF_NULL; \
            } \
            break; \
        case ICF_CM_FT_BP_SERV_CALL_BUFF: \
            { \
                if (ICF_NULL !=  \
                        p_cm_pdb->buff.service_call_buff.p_other_call_ctx) \
                    field_value = ICF_NULL + 1; \
                else \
                    field_value = ICF_NULL; \
            } \
            break; \
        default: \
            p_cm_pdb->field_type = type; \
            ICF_CM_SET_ECODE(p_cm_pdb, ICF_MSG_PROC_FIELD_TYPE_MISMATCH) \
            break; \
    } \
}
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_CM_MACRO_H__ */
