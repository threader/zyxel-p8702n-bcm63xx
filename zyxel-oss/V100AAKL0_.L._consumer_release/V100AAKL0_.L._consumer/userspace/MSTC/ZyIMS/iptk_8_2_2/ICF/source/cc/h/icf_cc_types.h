/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_types.h
 *
 * Purpose          : This file contains the datatypes used by CC.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 20-Dec-2004  Rohit Aggarwal  Non Service LLD     Initial
 * 22-Dec-2004  Rohit Aggarwal                      Added timer fields to pdb
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 28-Dec-2004  Rohit Aggarwal                      Added type for global data
 *
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0
 * 09-Jul-2007  Neha Choraria   ICF Rel 7.0         Changes for Forking &
 *                                                  Multiple Answer
 * 09-Feb-2009  Anurag Khare    ICF Rel 8.1         Changes done fro SDP 
 *                                                  Tunnelling
 * 30-Mar-2009  Anurag Khare    Rel 8.2             Changes done for PRACK
 *                                                  Enhancements
 * 02-Apr-2009  Tarun Gupta     Rel 8.2             REFER/NOTIFY Enhancements
 * 25-May-2009  Anuradha Gupta  SPR 19672           CSR Merging
 * 23-Sep-2009  Tarun Gupta     SPR 20063           Merged CSR 1-7616406
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CC_TYPES_H__
#define __ICF_CC_TYPES_H__


#include "icf_common_interface_types.h"
#include "icf_common_defs.h"
#include "icf_cc_defs.h"


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
 * Type Name        : icf_cc_pdb_st
 * Description      : CC process data block
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_list_st			     *p_codec_info_list;
    /* pointer to the internal message */
    icf_internal_msg_st              *p_internal_msg;

    /* pointer to the call context*/
    icf_call_ctx_st                  *p_call_ctx;

    /* pointer to the global database */
    icf_glb_pdb_st                   *p_glb_pdb;

    /* This field is derived from diversion header and filled in
     * pdb to send indication to PA on receiving 302 and 181 */
    icf_address_st                   *p_redirection_addr;

    /* This field is derived from diversion header and filled in
     * pdb to send indication to PA on receiving 302 and 181 */
    icf_name_st                      *p_redirection_name;


    /* bitmask to identify the type of MMS request */
    icf_uint16_t                     modify_media_type;

    /* THis variable is used to idnetify type
       of MMS req sent to MMI */
    icf_uint8_t                     mms_type;

    /* flag that indicates whether forceful DMS request needs to be sent */
    icf_boolean_t                    forceful_delete;

    /* flag for prack required or not */
    icf_boolean_t                    prack_reqd;

    /*  flag indicating whether a message sent or recd contains SDP or not */
    icf_boolean_t                    sdp_present;

    /* current event */
    icf_uint8_t                      current_event;

    /* flag to indicate to SSA whether media has to be sent or not */
    icf_boolean_t                    media_to_be_sent;

    /* flag to indicate to mms was sent to MMI or not */
    icf_boolean_t                    mms_sent;

    /* type of timer currently running (mostly used for traces) */
    icf_timer_type_t                 timer_type;

    /* id of timer currently running (mostly used for traces) */
    icf_timer_id_t                   timer_id;

    /* Flag to identify call was initated due to refer
     * This flag is set in case internal event is initate call */
     icf_boolean_t                   refer_init_call;

    /* pointer to ecode in icf_internal_msg_st */
    icf_error_t                      *p_ecode;

    /*The flag would indicate that media ongoing is for PA connect*/
    icf_boolean_t                    pa_connect_ong;

    /*
     * This flag would tell send DMS request failed because there was no MMI
     * context for the call
     */
    icf_boolean_t                    dms_fail_no_mmi_ctx;

    /* This would tell what error reason is to be send to SSA for update error
     * in icf_nw_otg_media_change_resp_st
     */
    /*icf_ssa_reason_t                 offer_failure_reason;*/

	/* This would tell what error reason is to be send to PA
     */
    /*icf_ssa_reason_t                 app_failure_cause;*/

    /* Following is a common bitmask used for various puprposes */
    /* The following bit is set if call needs to be cleared */
    #define ICF_CC_STREAM_POS_VALIDATION_FAIL                   0x0001
    #define ICF_CC_SEND_OFFER_TO_NW				                0x0002
    #define ICF_CC_REM_IP_NULL                                  0x0004
    #define ICF_CC_CALL_MODIFY_CFM_NT_REQD                      0x0008
    #define ICF_CC_FILL_CALL_MODIFY_DATA                        0x0010
    #define ICF_CC_ALL_STREAMS_DELETED                          0x0020
    #define ICF_CC_LOCAL_FAX_REQ                                0x0040
    #define ICF_STREAM_ID_VALIDATION_FAIL                       0x0080
    #define ICF_CC_REMOTE_REJ_STREAM_DELETE			            0x0100
    /*  CSR 1-5390593 Merge */
    #define ICF_CC_REM_PORT_CHNG                                0x0200
    #define ICF_CC_CALL_CLEAR            			            0x0400
    
    /* This bit is set when there is a failure while processing
       generic message req from application.This is used to send
       a failure ICF_GENERIC_MSG_RESP to the application.
    */
    #define ICF_GENERIC_MSG_RESP_FAILURE                        0x0800

    icf_uint16_t                     common_bitmask;

    /* Used whenever a list is to be passed to a utility function */
    icf_list_st                     *p_list_1;

    /* Used whenever a list is to be passed to a utility function */
    icf_list_st                     *p_list_2;

    /* The following are 2 void pointers to be used whenever any pointers
     * need to be passed to a utility function
     */
     icf_void_t                     *p_generic_ptr_1;
     icf_void_t                     *p_generic_ptr_2;

    /* Used whenever a ptr to icf_internal_sdp_stream_st is to be
       passed to a utility function */
    icf_internal_sdp_stream_st       *p_int_sdp_stream;

    /* This will be used for containing pointer of type
    * icf_stream_sdp_info_st
    */
    icf_stream_sdp_info_st			*p_stream_sdp_info;

#define ICF_CC_MMS_RESP_OFFER_REJECTED          1
#define ICF_CC_MMS_RESP_ADD_REJECTED            2
#define ICF_CC_MMS_RESP_MOD_REJECTED            3

/*  Rel 8.1
 *  This will be set when MMI is not able to 
 *  update the local SDP 
 */
#define ICF_CC_MMS_RESP_LOCAL_SDP_NOT_RECV      4
    icf_uint8_t                     mms_resp_type;

	/* Will be used for storing call_type.*/
	icf_uint8_t						call_type;

	icf_uint8_t						count;

	/* will be used for holding deleted streams info.*/
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];

    /* Will be used to hold stream id, it is added to avoid passing
	 * it as argument to the function.*/
	icf_stream_id_t					stream_id;

	/* Boolean variable used as flag.*/
	icf_boolean_t					flag;

	/* Application mode- Media mode set by application */
	icf_media_mode_t                application_mode;
    icf_media_mode_t                derived_mode;
    icf_media_mode_t                remote_mode;

    icf_boolean_t		     call_modify_ind; 
	
    /* this flag will be used for starting setup timer if it true 
     * then ICF_CC_CALL_MODIFY_SETUP_TIMER duration will be used
     * other wise configured setup timer value will be used */
   
     icf_boolean_t			is_call_modify_timer;

     /*This bit is set when call modify indication is to be sent to
      *application with media parameters*/
     icf_boolean_t                      pa_call_mod_wo_media;

#ifdef ICF_TRACE_ENABLE
	/* variables related to trace data population */
	icf_uint16_t			resp_code;
	icf_uint8_t				result;

      /* Boolean variable used as flag to indicate call hold status .*/
    icf_boolean_t           call_hold_status;

    icf_boolean_t           early_media;

	/* fill values for PA =1 , REMOTE = 2  */
	icf_uint8_t			call_clear_dir;
     icf_uint8_t	  stream_type;

    icf_address_st                   *p_replacing_party_addr;

    icf_name_st                      *p_replacing_party_name;
    icf_transport_address_st        remote_rtp_addr;
    icf_transport_address_st        local_rtp_addr;
#endif

    /* Used whenever forked_resp_id is to be passed to a utility function */
    icf_uint8_t                     forked_resp_id;

    /*  Rel 8.1 
     *  this boolean is used to indicate to function 
     *  icf_cc_send_pa_incoming_call_modify_ind()
     */
    icf_boolean_t                   offer_required;

    icf_config_data_st              *p_config_data;
    /* Fix for CSR 1-7616406: SPR 20053 */
	/* Duration of initial call setup timer that needs to be started for this
     * outgoing call. This shall override the default duration or the duration
     * configured through ICF_SET_CALL_PARAMS_REQ. This is received from APP
	 * in create call request.
     */
	icf_duration_t					setup_timer;
} icf_cc_pdb_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cc_fsm_func_ptr_t
 * Description      : CC's function pointer table type
 *
 *----------------------------------------------------------------------------*/
typedef icf_return_t (*icf_cc_fsm_func_ptr_t)(
        INOUT icf_cc_pdb_st *p_cc_pdb );


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cc_glb_data_st
 * Description      : CC's global data maintained by DBM
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* function pointer table for CC fsm */
    icf_cc_fsm_func_ptr_t
        cc_fsm_table[ICF_CC_MAX_STATES][ICF_CC_MAX_EVENTS];

} icf_cc_glb_data_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_CC_TYPES_H__ */
