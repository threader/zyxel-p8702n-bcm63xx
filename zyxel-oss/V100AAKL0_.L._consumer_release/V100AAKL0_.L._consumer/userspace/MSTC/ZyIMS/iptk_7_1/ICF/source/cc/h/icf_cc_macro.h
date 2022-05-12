/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_macro.h
 *
 * Purpose          : This file contains the macro definitions.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 15-Dec-2004  Rohit Aggarwal  Non Service LLD     Initial
 * 22-Dec-2004  Rohit Aggarwal                      Added macro for ecode
 * 23-Dec-2004  Rohit Aggarwal                      Corrected certain #defines
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 25-Dec-2004  Pankaj, Rohit                       Compilation fixes
 * 28-Dec-2004  Rohit Aggarwal                      Added trace compilation flag
 * 07-Mar-2006	Anuradha Gupta						Changed the Invalid event number
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CC_MACRO_H__
#define __ICF_CC_MACRO_H__

#include "icf_macro.h"


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
 * Macro Name       : ICF_CC_TRACE
 * Description      : Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_CC_TRACE(p_cc_pdb, trace_id)                                 \
{                                                                           \
    icf_cc_trace(p_cc_pdb, (icf_trace_id_t)(trace_id));                 \
}
#else
#define ICF_CC_TRACE(p_cc_pdb, trace_id)
#endif

#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_APP_TRACE
 * Description      : New Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CC_APP_TRACE(p_cc_pdb, trace_id)   \
 {\
     icf_cc_app_trace(p_cc_pdb, (icf_trace_id_t)(trace_id));\
 }
#else
 #define ICF_CC_APP_TRACE(p_cc_pdb, trace_id)
#endif /* end of #ifdef ICF_TRACE_ENABLE */
    
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_ECODE
 * Description      : Macro for ecode handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_ERROR_ENABLE
#define ICF_CC_SET_ECODE(p_cc_pdb, ecode)                                \
{                                                                           \
    icf_cc_error(p_cc_pdb, (icf_error_t)ecode);                       \
}
#else
#define ICF_CC_SET_ECODE(p_cc_pdb, ecode)                                \
{                                                                           \
    *(p_cc_pdb->p_ecode) = (icf_error_t)(ecode);							\
}
#endif

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_RELEASE_CAUSE
 * Description      : Resets the release cause from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_RELEASE_CAUSE(p_cc_pdb)                               \
p_cc_pdb->p_call_ctx->release_cause = ICF_NULL;

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_PREV_STATE
 * Description      : Gives the previous call state from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_PREV_STATE(p_cc_pdb)    p_cc_pdb->p_call_ctx->prev_state

    
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_PREV_STATE
 * Description      : Sets the previous call state in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_PREV_STATE(p_cc_pdb, state)                           \
{                                                                           \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PREVIOUS_STATE)                  \
    p_cc_pdb->p_call_ctx->prev_state = state;                               \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_PREVIOUS_STATE)              \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_PREV_STATE
 * Description      : Resets the previous call state in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_PREV_STATE(p_cc_pdb)                                \
{                                                                           \
    p_cc_pdb->p_call_ctx->prev_state = ICF_CC_18_CALL_STATE_INVALID;     \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_PREVIOUS_STATE)              \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_CALL_STATE
 * Description      : Gives the current call state from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_CALL_STATE(p_cc_pdb)    p_cc_pdb->p_call_ctx->call_state

    
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_CALL_STATE
 * Description      : Sets the current call state in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_CALL_STATE(p_cc_pdb, state)                           \
{                                                                           \
    ICF_CC_SET_PREV_STATE(p_cc_pdb, p_cc_pdb->p_call_ctx->call_state)    \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CURRENT_STATE)                   \
    p_cc_pdb->p_call_ctx->call_state = state;                               \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_CURRENT_STATE)               \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_NEXT_STATE
 * Description      : Gives the next call state from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_NEXT_STATE(p_cc_pdb)    p_cc_pdb->p_call_ctx->next_state

    
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_NEXT_STATE
 * Description      : Sets the next call state in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_NEXT_STATE(p_cc_pdb, state)                           \
{                                                                           \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_NEXT_STATE)                      \
    p_cc_pdb->p_call_ctx->next_state = state;                               \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_NEXT_STATE)                  \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_NEXT_STATE
 * Description      : Resets the next call state in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_NEXT_STATE(p_cc_pdb)                                \
{                                                                           \
    p_cc_pdb->p_call_ctx->next_state = ICF_CC_18_CALL_STATE_INVALID;     \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_NEXT_STATE)                  \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_MEDIA_MODE
 * Description      : Gives the current media mode from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_MEDIA_MODE(p_cc_pdb)    p_cc_pdb->p_call_ctx->media_mode


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_MEDIA_MODE
 * Description      : Sets the current media mode in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_MEDIA_MODE(p_cc_pdb, mode)                            \
{                                                                           \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_MODE)                      \
    p_cc_pdb->p_call_ctx->media_mode = mode;                                \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_MEDIA_MODE)                  \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_MODE_TO_BE_APPLIED
 * Description      : Gives the media mode to be applied from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_MODE_TO_BE_APPLIED(p_cc_pdb)                          \
p_cc_pdb->p_call_ctx->media_mode_to_be_applied


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_MODE_TO_BE_APPLIED
 * Description      : Sets the media mode to be applied in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_MODE_TO_BE_APPLIED(p_cc_pdb, mode)                    \
{                                                                           \
    p_cc_pdb->p_call_ctx->media_mode_to_be_applied = mode;                  \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_MODE_TO_BE_APP)              \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_MEDIA_STATE
 * Description      : Checks for a bit in media state bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_MEDIA_STATE(p_cc_pdb, bit)                          \
(p_cc_pdb->p_call_ctx->media_state & bit)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_MEDIA_STATE
 * Description      : Sets a bit in media state bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_MEDIA_STATE(p_cc_pdb, bit)                            \
{                                                                           \
    p_cc_pdb->p_call_ctx->media_state |= bit;                               \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_MEDIA_STATE
 * Description      : Resets a bit in media state bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_MEDIA_STATE(p_cc_pdb, bit)                          \
{                                                                           \
    p_cc_pdb->p_call_ctx->media_state &= (~(bit));                          \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_ALL_MEDIA_STATE
 * Description      : Resets all bits in media state bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_ALL_MEDIA_STATE(p_cc_pdb)                           \
{                                                                           \
    p_cc_pdb->p_call_ctx->media_state = ICF_NULL;                        \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_COMMON_BITMASK
 * Description      : Checks if a bit is set in common bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, bit)                       \
(p_cc_pdb->p_call_ctx->common_bitmask & bit)

/*call_mod_cancel*/
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_COMMON_BITMASK_2
 * Description      : Checks if a bit is set in common bitmask 2 in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, bit)                       \
(p_cc_pdb->p_call_ctx->common_bitmask_2 & bit)

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_COMMON_BITMASK
 * Description      : Sets a bit in common bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, bit)                          \
{ \
    p_cc_pdb->p_call_ctx->common_bitmask |= bit;                          \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_COMMON_BITMASK_2
 * Description      : Sets a bit in common bitmask 2 in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, bit)                          \
{ \
    p_cc_pdb->p_call_ctx->common_bitmask_2 |= bit;                          \
}

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_COMMON_BITMASK
 * Description      : Resets a bit in common bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb, bit)                       \
{                                                                           \
    p_cc_pdb->p_call_ctx->common_bitmask &= (~(bit));                       \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_COMMON_BITMASK_2
 * Description      : Resets a bit in common bitmask 2 in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb, bit)                       \
{                                                                           \
    p_cc_pdb->p_call_ctx->common_bitmask_2 &= (~(bit));                       \
}

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_PDB_COMMON_BITMASK
 * Description      : Checks if a bit is set in common bitmask in call context
 *					  pdb
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_PDB_COMMON_BITMASK(p_cc_pdb, bit)                       \
(p_cc_pdb->common_bitmask & bit)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_PDB_COMMON_BITMASK
 * Description      : Sets a bit in common bitmask in call context pdb
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb, bit)                         \
{                                                                           \
    p_cc_pdb->common_bitmask |= bit;                            \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_PDB_COMMON_BITMASK
 * Description      : Resets a bit in common bitmask in call context pdb
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_PDB_COMMON_BITMASK(p_cc_pdb, bit)                       \
{                                                                           \
    p_cc_pdb->common_bitmask &= (~(bit));                       \
}

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_COMMON_BITMASK_2
 * Description      : Checks if a bit is set in common bitmask 2 in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb, bit)                       \
(p_cc_pdb->p_call_ctx->common_bitmask_2 & bit)

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_COMMON_BITMASK_2
 * Description      : Sets a bit in common bitmask 2 in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_COMMON_BITMASK_2(p_cc_pdb, bit)                          \
{ \
    p_cc_pdb->p_call_ctx->common_bitmask_2 |= bit;                          \
}

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_COMMON_BITMASK_2
 * Description      : Resets a bit in common bitmask 2 in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb, bit)                       \
{                                                                           \
    p_cc_pdb->p_call_ctx->common_bitmask_2 &= (~(bit));                       \
}

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_RELEASE_TYPE
 * Description      : Checks a bit in release type bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, bit)                         \
(p_cc_pdb->p_call_ctx->release_type & bit)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_RELEASE_TYPE
 * Description      : Sets a bit in release type bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, bit)                           \
{                                                                           \
    p_cc_pdb->p_call_ctx->release_type |= bit;                              \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_RELEASE_TYPE
 * Description      : Resets a bit in release type bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_RELEASE_TYPE(p_cc_pdb, bit)                         \
{                                                                           \
    p_cc_pdb->p_call_ctx->release_type &= (~(bit));                         \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_RELEASE_CAUSE
 * Description      : Gives the release cause from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_RELEASE_CAUSE(p_cc_pdb)                               \
p_cc_pdb->p_call_ctx->release_cause


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_RELEASE_CAUSE
 * Description      : Sets the release cause in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, cause)                        \
{                                                                           \
    if(ICF_CALL_CLEAR_REASON_INVALID ==                                  \
            p_cc_pdb->p_call_ctx->release_cause)                            \
    {                                                                       \
        p_cc_pdb->p_call_ctx->release_cause = cause;                        \
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_RELEASE_CAUSE)           \
    }                                                                       \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_CALL_CLR_STATUS
 * Description      : Checks a bit in call clear status bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb, bit)                      \
(p_cc_pdb->p_call_ctx->call_clear_status & bit)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_CALL_CLR_STATUS
 * Description      : Sets a bit in call clear status bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, bit)                        \
{                                                                           \
    p_cc_pdb->p_call_ctx->call_clear_status |= bit;                         \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_CALL_CLR_STATUS
 * Description      : Resets a bit in call clear status bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb, bit)                      \
{                                                                           \
    p_cc_pdb->p_call_ctx->call_clear_status &= (~(bit));                    \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_MEDIA_ONGOING
 * Description      : Gives current value of media ongoing flag in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_MEDIA_ONGOING(p_cc_pdb)                               \
p_cc_pdb->p_call_ctx->media_ongoing


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_MEDIA_ONGOING
 * Description      : Sets the media ongoing flag in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_MEDIA_ONGOING(p_cc_pdb)                               \
{                                                                           \
    p_cc_pdb->p_call_ctx->media_ongoing = ICF_INT_MODULE_CC;             \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_MEDIA_ONGOING)               \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_MEDIA_ONGOING
 * Description      : Resets the media ongoing flag in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_MEDIA_ONGOING(p_cc_pdb)                             \
{                                                                           \
    p_cc_pdb->p_call_ctx->media_ongoing = ICF_INT_MODULE_INVALID;        \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_MEDIA_ONGOING)               \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_SIG_ONGOING
 * Description      : Gives value of signaling ongoing flag in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_SIG_ONGOING(p_cc_pdb)                                 \
p_cc_pdb->p_call_ctx->signalling_ongoing


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_SIG_ONGOING
 * Description      : Sets the signaling ongoing flag in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_SIG_ONGOING(p_cc_pdb)                                 \
{                                                                           \
    p_cc_pdb->p_call_ctx->signalling_ongoing = ICF_INT_MODULE_CC;        \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_SIG_ONGOING)                 \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_SIG_ONGOING
 * Description      : Resets the signaling ongoing flag in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)                               \
{                                                                           \
    p_cc_pdb->p_call_ctx->signalling_ongoing = ICF_INT_MODULE_INVALID;   \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_SIG_ONGOING)                 \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_CHECK_PENDING_EVENT
 * Description      : Checks a bit in pending event bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, bit)                        \
(p_cc_pdb->p_call_ctx->pending_event & bit)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CHECK_PENDING_LIST_IS_EMPTY
 * Description      : Checks if the pending event list in call context is empty
 *
 *----------------------------------------------------------------------------*/
#define ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb)                        \
p_cc_pdb->p_call_ctx->pending_event


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_PENDING_EVENT
 * Description      : Sets a bit in pending event bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_PENDING_EVENT(p_cc_pdb, bit)                          \
{                                                                           \
    p_cc_pdb->p_call_ctx->pending_event |= bit;                             \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_PENDING_EVENT
 * Description      : Resets a bit in pending event bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_PENDING_EVENT(p_cc_pdb, bit)                        \
{                                                                           \
    p_cc_pdb->p_call_ctx->pending_event &= (~(bit));                        \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RST_ALL_PENDING_EVENT
 * Description      : Resets the entire pending event bitmask in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RST_ALL_PENDING_EVENT(p_cc_pdb)                           \
{                                                                           \
    p_cc_pdb->p_call_ctx->pending_event = ICF_NULL;                      \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_GET_ONGOING_EVENT
 * Description      : Gives the ongoing event from call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_GET_ONGOING_EVENT(p_cc_pdb)                               \
p_cc_pdb->p_call_ctx->ongoing_event

    
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_ONGOING_EVENT
 * Description      : Sets the ongoing event in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, event)                        \
{                                                                           \
    p_cc_pdb->p_call_ctx->ongoing_event = event;                            \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_ONGOING_EVENT)               \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_ONGOING_EVENT
 * Description      : Resets the ongoing event in call context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)                             \
{                                                                           \
    p_cc_pdb->p_call_ctx->ongoing_event = ICF_CC_INVALID_EVENT;          \
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SET_ONGOING_EVENT)               \
}

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_STATUS
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CC_SET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status |= status_id; \
     ICF_CC_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_CHR_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_SET_STATUS_2
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CC_SET_STATUS_2(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status_2 |= status_id; \
     ICF_CC_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_CHR_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CC_RESET_MODIFY_MEDIA_TYPE
 * Description      : Reset modify_media_type on per stream basis
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb) \
 {\
    icf_internal_list_st *p_list = ICF_NULL;\
    icf_stream_sdp_info_st	*p_stream_sdp_info = ICF_NULL;\
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;\
  \
    while(ICF_NULL != p_list)\
    {\
        p_stream_sdp_info = (icf_stream_sdp_info_st *)p_list->p_payload;\
	if(ICF_NULL != p_stream_sdp_info)\
        	p_stream_sdp_info->media_modify_type = ICF_NULL;\
        p_list = p_list->p_next;\
    }\
 }


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_CC_MACRO_H__ */
