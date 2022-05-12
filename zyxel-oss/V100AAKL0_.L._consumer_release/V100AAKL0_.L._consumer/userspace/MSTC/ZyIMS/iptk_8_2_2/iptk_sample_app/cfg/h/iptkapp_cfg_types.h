/*****************************************************************************
 **
 ** FILENAME:		iptkapp_cfg_types.h
 **
 ** DESCRIPTION:	This file contains the declaration of various data types
 **                 used in IPTK application .
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 **	05-DEC-07		Amit Sikka       			Initial Creation
 **     11-Dec-2008             Anurag Khare      SPR 19218             CSR 1-5838506
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

#ifndef __IPTKAPP_CFG_TYPES_H__
#define __IPTKAPP_CFG_TYPES_H__


#include "iptkapp_port_wrapper.h"
#include "iptkapp_macros.h"


/* 
 * This structure will be used to fill information from config file
 * which will be utilised to fire configuration and registration API's 
 * towards IPTK*/

typedef struct
{
    /* IP and port for IPTK Application */
    icf_int8_t iptk_app_ip[IPTKAPP_MAX_IP_ADDRESS_LEN];
    icf_uint16_t iptk_app_port;

    /* IPTK IP & Port towards application */
    icf_int8_t iptk_ip[IPTKAPP_MAX_IP_ADDRESS_LEN];
    icf_uint16_t iptk_port;
    /* IPTK port towards network*/
    icf_uint16_t iptk_nw_port;

    icf_int8_t proxy_ip[IPTKAPP_MAX_IP_ADDRESS_LEN];
    icf_uint16_t proxy_port;

    icf_int8_t registrar_ip[IPTKAPP_MAX_IP_ADDRESS_LEN];
    icf_uint16_t registrar_port;
    icf_uint32_t registrar_duration;

    icf_int8_t user_address[IPTKAPP_MAX_REQUEST_URL_LEN];
    icf_int8_t authentication_key[IPTKAPP_MAX_REQUEST_URL_LEN];
    icf_int8_t authentication_password[IPTKAPP_MAX_REQUEST_URL_LEN];

}iptkapp_cfg_db_t;

/* This enum will be used to match tokens from config file
 */
typedef enum
{
    IPTK_APP_IP = 0,
    IPTK_APP_PORT,
    IPTK_IP,
    IPTK_PORT,
    IPTK_NETWORK_PORT,
    PROXY_IP,
    PROXY_PORT,
    REGISTRAR_IP,
    REGISTRAR_PORT,
    REGISTER_DURATION,
    SELF_ID_URL,
    AUTHENTICATION_KEY,
    AUTHENTICATION_PASSWORD
} iptkapp_cfg_tokens_et;
    





/* Function Prototypes for all the function are defined here
 */


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_init
 *
 * DESCRIPTION   : The function is to register application with IPTK
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/
iptkapp_return_t iptkapp_cfg_init(iptkapp_error_t *);



/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_app_add_iptk
 *
 * DESCRIPTION   : The function is to register application with IPTK
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_app_add_iptk(iptkapp_error_t *);



/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_init_add_proxy_address_iptk
 *
 * DESCRIPTION   : The function to fill the proxy address to be added to be send
 *					towards ICF during intialization
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_init_add_proxy_address_iptk(iptkapp_error_t *);




/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_init_add_registrar_address_iptk
 *
 * DESCRIPTION   : The function to fill the registrar address, private password
 *					private user identity and authorization related parameters
 *					to be added	to be send towards IPTK during intialization
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_init_add_registrar_address_iptk(iptkapp_error_t *);

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_register_req
 *
 * DESCRIPTION   : The function to fill the proxy address to be added to be send
 *					towards ICF during intialization
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_register_req(icf_uint32_t, iptkapp_error_t *);


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_init_add_public_userid_iptk
 *
 * DESCRIPTION   : The function to fill the public address to be added to be send
 *					towards ICF during intialization
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_init_add_public_userid_iptk(iptkapp_error_t *);


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_fill_hdr_and_send_icf
 *
 * DESCRIPTION   : The function to fill the header of the message and send to ICF
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/
iptkapp_return_t iptkapp_cfg_fill_hdr_and_send_iptk(icf_msg_st *,
                                                icf_api_id_t,
												icf_call_id_t ,
												iptkapp_error_t *);




/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_process_message
 *
 * DESCRIPTION   : This function recieves and process the messages recieved
 *				   from IPTK 
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_process_message(icf_msg_st *);



/******************************************************************************
 *
 * FUNCTION:        iptkapp_cfg_read_config_file
 *
 * DESCRIPTION:     This function reads the config file and fills config DB
 *                  accordingly
 *
 *****************************************************************************/
iptkapp_return_t iptkapp_cfg_read_config_file(icf_void_t);


/******************************************************************************
 *
 * FUNCTION:        iptkapp_cfg_display_cfg_db
 *
 * DESCRIPTION:     This function displays the config DB structure
 *
 *****************************************************************************/

icf_void_t iptkapp_cfg_display_cfg_db(icf_void_t);

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_codec_info_mm
 *
 * DESCRIPTION   : The function to fill the codec related information to be send
 *					towards MM
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_codec_info_mm(iptkapp_error_t *p_err);

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_self_ip_address_mm
 *
 * DESCRIPTION   : The function to fill the self ip address to be send
 *					towards MM
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_self_ip_address_mm(iptkapp_error_t *p_err);

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_codec_info_mm
 *
 * DESCRIPTION   : The function to fill the codec related information to be send
 *					towards MM
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/

icf_return_t iptkapp_cfg_send_codec_info_mm(icf_error_t *p_err) ;

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_self_ip_address_mm
 *
 * DESCRIPTION   : The function to fill the self ip address to be send
 *					towards MM
 *
 * RETURNS       : imsc_return_t
 *
 ******************************************************************************/

icf_return_t iptkapp_cfg_send_self_ip_address_mm(icf_error_t *p_err);



/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_message_req
 *
 * DESCRIPTION   : The function to fill the message request API parameters to send
 *					towards ICF
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_message_req(icf_uint8_t *message_destination_info,icf_uint8_t *message_self_id_url,icf_uint8_t *user_message, iptkapp_error_t *p_err);




/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_publish_req
 *
 * DESCRIPTION   : The function to fill the publish request API parameters to send
 *					towards ICF
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_publish_req( iptkapp_error_t *p_err,
                                          icf_int16_t publish_state );

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_print_publish_resp
 *
 * DESCRIPTION   : The function send the response of the publish request towards
 *					application
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_print_publish_resp(icf_msg_st* p_msg);

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_service_params_iptk
 *
 * DESCRIPTION   : The function to fill the set_service_request API parameters to send
 *                                      towards ICF
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_service_params_iptk(iptkapp_error_t *p_err);


#endif
