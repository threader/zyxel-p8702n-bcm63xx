/****************************************************************************
 **
 ** FILENAME:        iptkapp_core_fill_call_api.c
 **
 ** DESCRIPTION:    This is the main file where all call related APIs are filled 
 **                 to be fired towards IPTK
 **
 ** DATE            NAME        REFERENCE        REASON
 ** ----            ----        ---------        ------
 ** 13-DEC-07       Amit Sikka                    Initial Creation
 *****************************************************************************
 **            Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

/***************************************************************************
 * Include Files area
 ***************************************************************************/
#include"iptkapp_core_fsm_types.h"
#include"iptkapp_macros.h"
#include"iptkapp_port_wrapper.h"
#include"iptkapp_core_fsm_defs.h"
#include"iptkapp_core_prototypes.h"
#include"iptkapp_port_prototypes.h"
#include"icf_types.h"
#include"icf_api.h"
#include"iptkapp_glb.h"
#include"icf_clib.h"
#include "iptkapp_feature_flags.h"

/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_create_call_api
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_create_call_api(
        icf_void_t   *p_input_param,
        icf_uint16_t call_id,
        icf_error_t    *p_err)

{
    icf_create_call_req_st                      *p_payload = IPTKAPP_NULL;
    icf_msg_st                                  *p_msg = IPTKAPP_NULL;
    iptkapp_return_t                            ret_val = IPTKAPP_SUCCESS;

    /* get the memory for sending message to the ICF */
    IPTKAPP_MSG_ALLOC(p_msg,
            sizeof(icf_create_call_req_st),
            ret_val,
            p_err);

    if(IPTKAPP_FAILURE == ret_val)
    {
        return IPTKAPP_FAILURE;
    }

    /* This API is sent by the application to inform IP PHONE TOOLKIT
     * about the call initiated by the local user. After receiving 
     * this API, IPTK can initiate a SIP ‘INVITE’ message towards 
     * the remote user
     */

    p_payload = (icf_create_call_req_st*)(IPTKAPP_MSG_GET_PAYLOAD(p_msg));

    /* Reset the bit_mask */
    /* Indicates the presence of optional parameters in the API
     */
    p_payload->bit_mask = 0;


    /* Fill the called party address here */
    iptkapp_port_strcpy((icf_int8_t *)((icf_string_st *)p_payload->called_party_addr.addr_val.str),
            ((icf_int8_t *)((icf_string_st *)p_input_param)->str));

    p_payload->called_party_addr.addr_val.str_len = ((icf_string_st*)p_input_param)->str_len;

    p_payload->called_party_addr.addr_type = ICF_ADDRESS_PLAN_SIP_URI; /* this is for SIP URL */

    if(iptkapp_core_remove_type_from_addr(p_payload->called_party_addr.addr_val.str) == ICF_FAILURE)
    {
        return ICF_FAILURE;
    }

    p_payload->line_id = IPTKAPP_DEFAULT_LINE_ID;
    p_payload->called_party_addr.addr_val.str_len = 
        iptkapp_port_strlen((icf_int8_t *)p_payload->called_party_addr.addr_val.str);

    /*call_id    = iptkapp_core_create_call_id();*/

    if(iptkapp_core_fill_hdr_and_send_cc(p_msg,
                ICF_CREATE_CALL_REQ,
                call_id,
                p_err) == ICF_FAILURE)

    {  /* Error in sending the message to IPPSTK */
        return ICF_FAILURE;
    }

    return IPTKAPP_SUCCESS;
}



/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_local_user_alerted_req_api
 *
 *  DESCRIPTION    : This function fills local user alerted req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_local_user_alerted_req_api(
        icf_uint16_t call_id,
        icf_error_t    *p_err)
{

    icf_local_user_alerted_req_st      *p_payload = IPTKAPP_NULL;
    icf_msg_st                          *p_msg_alert_to_cc = IPTKAPP_NULL;
    iptkapp_return_t                    ret_val = IPTKAPP_SUCCESS;

    /* This API is invoked by the application towards the IP Phone Toolkit
     * in response to the ICF_INCOMING_CALL_IND API. This API indicates 
     * that the application has alerted the local user about the incoming 
     * call. The IPTK initiates 180 SIP message towards the remote user.
     * The application can also invoke this API in response to the 
     * ICF_INCOMING_CALL_MODIFY_IND from IP Phone Toolkit. This feature 
     * is supported for POC application (to add users to an already 
     * established session).
     */

    /* get the memory to send the user alerted cc */
    IPTKAPP_MSG_ALLOC(p_msg_alert_to_cc,
            sizeof(icf_local_user_alerted_req_st),
            ret_val,
            p_err);

    if(ret_val==IPTKAPP_FAILURE)
    {
        return IPTKAPP_FAILURE;
    }


    /* Payload is being extracted from the messge to be posted
     */
    p_payload = (icf_local_user_alerted_req_st*)
        (IPTKAPP_MSG_GET_PAYLOAD(p_msg_alert_to_cc));

    /* Indicates the presence of optional parameters in the API
     */
    p_payload->bit_mask = 0;

    /* Informs IP Phone Toolkit whether the application or the remote user 
     * should play the ring back tone. This parameter is used by IP Phone
     * Toolkit only if the INVITE received from the network contains the SDP.
     * If the INVITE does not contain SDP, IP Phone Toolkit assumes that 
     * the inband info is not present.
     */
    /* we are not playing the ring back to the n/w */
    p_payload->inband_info_status = IPTKAPP_FALSE;

    /* send the user alerted to the CC */
    if(iptkapp_core_fill_hdr_and_send_cc(p_msg_alert_to_cc,
                ICF_LOCAL_USER_ALERTED_REQ,
                call_id,
                p_err)==IPTKAPP_FAILURE)
    {
        return IPTKAPP_FAILURE;
    }

    return IPTKAPP_SUCCESS;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_terminate_call_ind_api
 *
 *  DESCRIPTION    : This function fills local user alerted req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_terminate_call_ind_api(
        icf_uint16_t call_id,
        icf_error_t  *p_err)
{
    icf_msg_st                                *p_term_call_resp = IPTKAPP_NULL;
    iptkapp_return_t                          ret_val = IPTKAPP_SUCCESS;

    /* This API is invoked by IP Phone Toolkit Module towards the 
     * application when SIP BYE/CANCEL/4xx/5xx/6xx request from 
     * remote user is received or call is released due to some local error
     */

    IPTKAPP_MSG_ALLOC(p_term_call_resp, 0, ret_val,p_err);
    if(IPTKAPP_FAILURE == ret_val)
    {
        /* no memory at the system */
        return IPTKAPP_FAILURE;
    }

    if(iptkapp_core_fill_hdr_and_send_cc(p_term_call_resp,
                ICF_LOCAL_USER_TERMINATE_CALL_RESP,
                call_id,
                p_err)==IPTKAPP_FAILURE)
    {
        /* raise the alarm.. unable to send term call resp to CC
           After some time cc will time out and remove the call context*/
        return IPTKAPP_FAILURE;
    }

    return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_terminate_call_req_api
 *
 *  DESCRIPTION    : This function fills terminate call req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_terminate_call_req_api(
        icf_uint16_t            call_id,
        icf_call_clear_reason_t call_clear_reason,
        icf_error_t             *p_err)
{
    icf_msg_st                         *p_term_call_cc = IPTKAPP_NULL;
    iptkapp_return_t                   ret_val = IPTKAPP_SUCCESS;
    icf_terminate_call_req_st          *p_payload = IPTKAPP_NULL;


    /* This API is invoked by the application towards IPTK when 
     * the local user rejects an incoming call or terminates an 
     * ongoing call. This API is also used to indicate any internal
     * failure in the application. This results in a BYE/CANCEL 
     * /4xx/5xx/6xx SIP message towards the remote user depending 
     * on the call state. Any time during an on-going call, this 
     * API will result in a SIP BYE message. If the user has 
     * initiated the call and this API is used before the final 
     * response has arrived, then this will result in a SIP 
     * CANCEL message towards the network.
     */

    IPTKAPP_MSG_ALLOC(p_term_call_cc,
            sizeof(icf_terminate_call_req_st),
            ret_val,
            p_err);

    if(IPTKAPP_FAILURE == ret_val)
    {
        return IPTKAPP_FAILURE;
    }

    /* extracting the payload */
    p_payload = (icf_terminate_call_req_st*)IPTKAPP_MSG_GET_PAYLOAD(p_term_call_cc);


    /* Indicates the presence of optional parameters in the API
     */
    p_payload->bit_mask = 0;

    /* fill the call clear reason .
     * Indicates the reason for terminating the call. 
     * IPTK can use this reason to send the appropriate 
     * SIP message to the remote party.*/
    p_payload->call_clear_reason = call_clear_reason;

    if(iptkapp_core_fill_hdr_and_send_cc(p_term_call_cc,
                ICF_TERMINATE_CALL_REQ,
                call_id,
                p_err)==IPTKAPP_FAILURE)
    {
        return IPTKAPP_FAILURE ;
    }

    return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_connect_call_req_api
 *
 *  DESCRIPTION    : This function fills connect call req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_connect_call_req_api(
        icf_uint16_t call_id,
        icf_error_t    *p_err)
{
    icf_msg_st                            *p_accept_call = IPTKAPP_NULL;
    icf_connect_req_st                    *p_payload = IPTKAPP_NULL;
    iptkapp_return_t                      ret_val = IPTKAPP_SUCCESS;


    /* This API is invoked by the application to IPTK when the local 
     * user accepts the call. On receiving this API, IP Phone Toolkit 
     * sends a SIP 200 OK message towards the network. This message 
     * is sent in response to the INVITE message received earlier for
     * an incoming call.
     */

    IPTKAPP_MSG_ALLOC(p_accept_call,sizeof(icf_connect_req_st),ret_val,p_err);
    if(ret_val == IPTKAPP_FAILURE)
    {
        return IPTKAPP_FAILURE;
    }

    /* extracting the payload
     */
    p_payload = (icf_connect_req_st*)IPTKAPP_MSG_GET_PAYLOAD(p_accept_call);

    /* Indicates the presence of optional parameters in the API
     */
    p_payload->bit_mask = 0;

    /* Number of deleted media streams.*/
    /*Initialise Count as well, for deleted stream, it may be filled later*/
    p_payload->count = 0;

    if(iptkapp_core_fill_hdr_and_send_cc(p_accept_call,
                ICF_CONNECT_REQ,
                call_id,
                p_err)==IPTKAPP_FAILURE)
    {
        /* Unable to send the connect to the CC */
        return IPTKAPP_FAILURE;
    }

    return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_fill_hdr_and_send_cc
 *
 *  DESCRIPTION    : This function will fill the API header and send it to 
 *                    to the IPTK
 *
 *  RETURNS          : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t 
iptkapp_core_fill_hdr_and_send_cc(
        icf_msg_st            *p_msg, 
        icf_api_id_t          api_id,
        icf_call_id_t         call_id,
        iptkapp_error_t       *p_err)
{

    iptkapp_return_t    ret_val = IPTKAPP_SUCCESS;
    icf_uint8_t         *p_buffer = IPTKAPP_NULL;
    icf_uint32_t        buf_len=0;
#ifdef IPTK_REL_7_0
    icf_vldt_err_st     vldt_err;
#endif

    p_buffer = (icf_uint8_t *)p_msg;

    IPTKAPP_MSG_FILL_HDR(p_msg,api_id,IPTKAPP_MODULE_PA,
            ICF_MODULE_ICF,call_id,g_app_id);
    buf_len = IPTKAPP_MSG_GET_PAYLOAD_LENGTH(p_msg);

#ifdef IPTK_REL_7_0
    ret_val = icf_encode_msg((icf_msg_st*)p_msg, &p_buffer, &buf_len,p_err,&vldt_err);
#else
    ret_val = icf_encode_msg((icf_msg_st*)p_msg, &p_buffer, &buf_len,p_err);
#endif

    if( ret_val !=  IPTKAPP_SUCCESS)
    {
        /* add the trace */
        return(IPTKAPP_FAILURE);
    }


    IPTKAPP_MSG_SEND(p_buffer,buf_len,ret_val,p_err);

    /* Error in sending */
	if(ret_val==IPTKAPP_FAILURE)
	{
		icf_free_encoded_msg(p_buffer,p_err);  /* ICF - free message encoded */
		return(IPTKAPP_FAILURE);
	}

    /* memory taken by this buffer needs to be freed after sending to IPTK
     */
	
	icf_free_encoded_msg(p_buffer,p_err);   /* ICF - free encoded message*/
    
    iptkapp_port_free((icf_void_t*)p_msg);

    return IPTKAPP_SUCCESS;
}





