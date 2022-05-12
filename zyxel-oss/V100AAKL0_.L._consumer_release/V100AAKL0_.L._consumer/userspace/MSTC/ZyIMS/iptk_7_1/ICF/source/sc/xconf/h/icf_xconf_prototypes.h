/*****************************************************************************
*
* FILE NAME    : icf_xconf_prototypes.h
*
* DESCRIPTION  : Contains the prototypes used in XCONF service
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 4-Jul-2007  Abhishek Dhammawat CDD Rel7.0     Created Origional
*
* Copyright 2007,Aricent.
*
*******************************************************************************/

#ifndef __ICF_XCONF_PROTOTYPES_H__
#define __ICF_XCONF_PROTOTYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_xconf_defs.h"
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_sc_common_types.h"

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_init
*
* DESCRIPTION   : This is the initialization function of XCONF service module
*                 which is called by DBM at the time of system startup
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t   icf_xconf_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_xconf_data,
        OUT     icf_error_t           *p_ecode
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_process_mesg
*
* DESCRIPTION   : This is the Entry Point function of XCONF service module
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_xconf_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_convert_api_to_event
*
* DESCRIPTION   : This function converts incoming APIs to internal events
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t  icf_xconf_convert_api_to_event(
        INOUT  icf_xconf_pdb_st *p_pdb,
        INOUT  icf_uint8_t      *p_event
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_deinit_ctx
*
* DESCRIPTION   : This is the deinit function of XCONF service module
*                 which is called by DBM at the time of system stop
*
* RETURNS       : icf_void_t
*
*******************************************************************************/
icf_void_t   icf_xconf_deinit_ctx(
        INOUT icf_xconf_pdb_st  *p_pdb
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_00_init_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_00_IDLE
*                 Event         ICF_XCONF_INITIATE_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_00_init_conf_req(
        INOUT  icf_xconf_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_init_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                               ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_04_CONFERENCE_CONNECTED
*                               ICF_XCONF_05_AWT_CONFERENCE_TERMINATE
*
*                 Event         ICF_XCONF_INITIATE_CONF_REQ
*
* RETURNS       : ICF_FAILURE/ICF_SUCCESS
*
*******************************************************************************/
icf_return_t icf_xconf_xx_init_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_conf_server_connected
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_CONF_SERVER_CONNECTED
*
* RETURNS       : ICF_FAILURE/ICF_SUCCESS
*
*******************************************************************************/
icf_return_t icf_xconf_01_conf_server_connected(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_01_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_02_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_03_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_04_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_04_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_05_add_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_05_AWT_CONFERENCE_TERMINATE
*                 Event         ICF_XCONF_ADD_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_05_add_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_05_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_05_AWT_CONFERENCE_TERMINATE
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_05_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_add_party_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_ADD_PARTY_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_02_add_party_resp(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFEERENCE_CONNECT
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_01_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_02_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_03_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_06_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_06_AWT_CALL_RESUME_RESP
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_06_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_04_del_party_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_DEL_PARTY_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_04_del_party_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_del_party_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_DEL_PARTY_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_03_del_party_resp(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_01_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_xx_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State         ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_03_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_04_release_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_RELEASE_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_04_release_conf_req(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_notify_for_sub
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                               ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_NOTIFY_FOR_SUBS
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t icf_xconf_xx_notify_for_sub(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_subs_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                               ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_SUBS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_xx_subs_resp(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_05_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_05_AWT_CONFERENCE_TERMINATE
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_05_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 state              ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_xx_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_02_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_02_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_03_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_03_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_06_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_06_AWT_CALL_RESUME_RESP
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_06_call_released(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_01_call_released(
        INOUT  icf_xconf_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_xx_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_02_AWT_ADD_PARTY_RESP
*                               ICF_XCONF_03_AWT_DEL_PARTY_RESP
*                               ICF_XCONF_04_CONFERENCE_CONNECTED
*                 Event         ICF_XCONF_FORCEFUL_CALL_CLEAR
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_xx_forceful_call_clear(
        INOUT  icf_xconf_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_01_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_01_AWT_CONFERENCE_CONNECT
*                 Event         ICF_XCONF_FORCEFUL_CALL_CLEAR
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_01_forceful_call_clear(
        INOUT  icf_xconf_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_00_incoming_conf_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_00_IDLE
*                 Event         ICF_XCONF_INCOMING_CONF_REQ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_00_incoming_conf_req(
        INOUT  icf_xconf_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_06_call_resume_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_XCONF_06_AWT_CALL_RESUME_RESP
*                 Event         ICF_XCONF_CALL_RESUME_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_xconf_06_call_resume_resp(
        INOUT  icf_xconf_pdb_st *p_pdb);


/******************************************************************************
* FUNCTION NAME : icf_xconf_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_xconf_unexpected_event(
        INOUT   icf_xconf_pdb_st   *p_pdb);

#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME    :   icf_xconf_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS: none
******************************************************************************/
icf_void_t icf_xconf_state_trace_string(
        IN    icf_uint8_t          state,
        INOUT icf_trace_data_st   *p_trace_data);

/******************************************************************************
*
* FUNCTION NAME    :   icf_xconf_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:none
******************************************************************************/
icf_void_t icf_xconf_event_trace_string(
        IN    icf_uint8_t         event,
        INOUT icf_trace_data_st   *p_trace_data);

/******************************************************************************
*
* FUNCTION NAME:   icf_xconf_trace
*
* DESCRIPTION  : This function is used to define the trace data for
*                XCONF service
*
*
******************************************************************************/
icf_void_t    icf_xconf_trace(
        INOUT icf_xconf_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);


#endif

/*******************************************************************************
*
* FUNCTION NAME    :   icf_xconf_fill_pa_msg_hdr
*
* DESCRIPTION  : This function fills the icf_api_header_st to send message
*                to PA
*
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_void_t icf_xconf_fill_pa_msg_hdr(
        INOUT   icf_xconf_pdb_st *p_pdb,
        INOUT   icf_msg_st     *p_msg,
        IN      icf_api_id_t    api_id,
        IN      icf_uint16_t    api_len
        );

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_add_party_resp
*
* DESCRIPTION   : send failure or success resp for add_party_resp to PA ,
*                 SUCCESS/FAILURE is identified by second argument
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_add_party_resp(
        INOUT  icf_xconf_pdb_st    *p_pdb,
        IN     icf_uint8_t         result);

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_del_party_resp
*
* DESCRIPTION   : send failure or success resp for del_party_resp to PA ,
*                 SUCCESS/FAILURE is identified by second argument
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_del_party_resp(
        INOUT  icf_xconf_pdb_st    *p_pdb,
        IN      icf_uint8_t    result);

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_xternal_conf_resp
*
* DESCRIPTION   : send failure or success resp for initiate_conf_resp to PA,
*                 SUCCESS/FAILURE is identified by second argument
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*****************************************************************************/
icf_return_t icf_xconf_send_xternal_conf_resp(
            INOUT  icf_xconf_pdb_st    *p_pdb,
            IN     icf_uint8_t         result);

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_pa_release_conf_resp
*
* DESCRIPTION   :
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_pa_release_conf_resp(
                INOUT  icf_xconf_pdb_st    *p_pdb,
                IN     icf_uint8_t    result);

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_fill_call_ctx
*
* DESCRIPTION   : Fill/Prepare the call ctx block from initiate_conference_request
*                 received from PA.
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_fill_call_ctx(
        INOUT  icf_xconf_pdb_st    *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_clear_call_ctx
*
* DESCRIPTION   : Free the allocated ctx
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_clear_call_ctx(
        INOUT  icf_xconf_pdb_st    *p_pdb,
        INOUT  icf_call_ctx_st     *p_call_ctx);

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_pa_notify_ind
*
* DESCRIPTION   : This function sends notify_ind to PA.
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_pa_notify_ind(
        INOUT  icf_xconf_pdb_st    *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME : icf_xconf_send_pa_release_conf_ind
*
* DESCRIPTION   :
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_xconf_send_pa_release_conf_ind(
        INOUT  icf_xconf_pdb_st    *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME:   icf_xconf_alloc_n_fill_serv_ctx
*
* DESCRIPTION  : This function is used to allocate new service context for
*                new call and fill all the information
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_alloc_n_fill_serv_ctx(
        INOUT   icf_xconf_pdb_st    *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME:   icf_xconf_validate_conf_req
*
* DESCRIPTION  : This function is used to validate that if conference_uri is
*                 present in recvd API then use it else check for same in line_data
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_validate_conf_req(
        INOUT   icf_xconf_pdb_st    *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME:  icf_xconf_send_forceful_clear_to_sic
*
* DESCRIPTION  : This function is used to trigger forceful clear to SIC.
*                Invoke internal msg with msg id as ICF_FORCEFUL_CALL_CLEAR_IND
*                will be triggered to SIC for clearing it's ctx.
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_send_forceful_clear_to_sic(
        INOUT   icf_xconf_pdb_st    *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME:  icf_xconf_send_unsubscribe_req_to_sic
*
* DESCRIPTION  : This function is used to trigger Unsubscribe req to SIC.
*                A internal message ICF_INTERNAL_SUBSCRIBE_REQ with duration as 0
*                will be triggered.
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_send_unsubscribe_req_to_sic(
        INOUT   icf_xconf_pdb_st    *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME:  icf_xconf_send_sub_req
*
* DESCRIPTION  : This function is used to trigger subscribe req to SIC.
*                A internal message ICF_INTERNAL_SUBSCRIBE_REQ with duration as
*                non zero will be triggered .
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_xconf_send_sub_req(
        INOUT   icf_xconf_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_alloc_srvc_cntxt_lgc_blk 
*
* DESCRIPTION   :This function calls the dbm function to allocate the 
*               service logic ctx block and initializes the parameters of the
*               service logic ctx created. 
*
*******************************************************************************/
icf_return_t icf_xconf_alloc_srvc_cntxt_lgc_blk(
        INOUT icf_xconf_pdb_st     *p_pdb,
        OUT  icf_void_t          **p_p_service_logic_ctx,
        INOUT icf_error_t        *p_ecode);


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_XCONF_PROTOTYPES_H__ */

