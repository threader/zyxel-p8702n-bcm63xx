/*****************************************************************************
 **
 ** FILENAME:		iptkapp_cfg_init.c
 **
 ** DESCRIPTION:	This file contains the functionality to initialise 
 **                 configuration module.
 **
 ** DATE		NAME    		REFERENCE		REASON
 ** ----		----	    	---------		------
 **	05-DEC-07	Amit Sikka       			Initial Creation
 ** 10-DEC-08   Vipul Varshney  SPR17945     Updated for PUBLISH and MESSAGE 
 **                                             request handlers
 ** 11-Dec-08          Anurag Khare          CSR 1-5838506 Merged
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

#include "iptkapp_macros.h"
#include "iptkapp_glb.h"
#include "iptkapp_core_fsm_types.h"
#include "iptkapp_core_prototypes.h"
#include "iptkapp_port_wrapper.h"
#include "iptkapp_cfg_types.h"

static iptkapp_cfg_db_t g_iptkapp_cfg_db_temp;
icf_publish_req_st g_iptkapp_publish_db;

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_init
 *
 * DESCRIPTION   : The function is to register application with IPTK
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/
iptkapp_return_t iptkapp_cfg_init(iptkapp_error_t *p_err)
{
    
    if (IPTKAPP_NULL == iptkapp_port_memcpy(&g_iptkapp_cfg_db_temp,&g_iptkapp_cfg_db_t,
                                             sizeof(iptkapp_cfg_db_t)))
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP> MEMCPY Fail\n");
    }

    if(ICF_NULL == iptkapp_port_memset(&g_iptkapp_publish_db,0,sizeof(g_iptkapp_publish_db)))
    {
        IPTKAPP_PRINTF("iptkapp> iptkapp_port_memset for publish database fails");
    }

    
    /* Through this function APP_ADD_REQ is sent towards IP Phone Toolkit.
     * On invocation of this API, Application registers itself with 
     * IP Phone Toolkit. IP Phone Toolkit  returns a unique application 
     * id (app_id) .The application is required to specify this app_id 
     * in all subsequent APIs to IP Phone Toolkit. 
     * Moreover, the application also receives configured public-URIs
     * and P-CSCF addresses in the response
     */
    if (IPTKAPP_FAILURE == iptkapp_cfg_send_app_add_iptk(p_err))
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > APP_ADD_REQ towards IP Phone Toolkit\
                               Failed. \n\t IPTKAPP > CFG_INIT Failed \n");
        return IPTKAPP_FAILURE;
    }

    /* Through this function SET_TRANSPORT_PARAMS API is invoked towards 
     * IP Phone Toolkit.This API is used to configure transport-related
     * parameters such as, proxy address per line, self-IP address, and 
     * the port on which the network (SIP) messages are received.
     */
    if (IPTKAPP_FAILURE == iptkapp_cfg_send_init_add_proxy_address_iptk(p_err))
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > SET_TRANSPORT_PARAMS_REQ towards IP Phone\
                       Toolkit Failed \n\t IPTKAPP > CFG_INIT Failed \n");
        return IPTKAPP_FAILURE;
    }

    /* Through this function SET_REGISTRATION_PARAMS_REQ is invoked towards 
     * IP Phone Toolkit.This API is used to configure registration-related
     * parameters, such as, the registrar address, and registration duration.
     * After initial configuration, any of these parameters can be modified 
     * using the same API.
     */

    if (IPTKAPP_FAILURE == iptkapp_cfg_send_init_add_registrar_address_iptk(\
                                                                       p_err))
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > SET_REGISTRATION_PARAMS_REQ towards IP \
                Phone Toolkit Failed \n\t IPTKAPP > CFG_INIT Failed \n");
        return IPTKAPP_FAILURE;
    }

    /* Through this function SET_SELF_ID_REQ is invoked towards 
     * IP Phone Toolkit.This API is used to configure user 
     * identity-related parameters such as, the user name and 
     * address. After initial application configuration, 
     * any of these parameters can be modified using the same API.
     */
    if (IPTKAPP_FAILURE == iptkapp_cfg_send_init_add_public_userid_iptk(\
                                                                    p_err))
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > SET_SELF_ID_REQ towards IP Phone \
                        Toolkit Failed \n\t IPTKAPP > CFG_INIT Failed \n");
        return IPTKAPP_FAILURE;
    }
    /*  CSR 1-5838506 Merge */
    /* Through this function SET_SERVICE_PARAMS_REQ is invoked towards 
     * IP Phone Toolkit.This API is used to configure various services 
     */
    if (IPTKAPP_FAILURE == iptkapp_cfg_send_service_params_iptk(\
                                                                p_err))
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > SET_SERVICE_PARAMS_REQ towards IP Phone \
                        Toolkit Failed \n\t IPTKAPP > CFG_INIT Failed \n");
        return IPTKAPP_FAILURE;
    }


    return IPTKAPP_SUCCESS;
}


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_send_app_add_iptk
 *
 * DESCRIPTION   : The function is to register application with IPTK
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_cfg_send_app_add_iptk(iptkapp_error_t *p_err)
{
	icf_msg_st                              *p_msg = IPTKAPP_NULL;
	iptkapp_return_t                        ret_val;
    icf_app_add_req_st                      *p_payload = IPTKAPP_NULL;
	icf_string_list_node_st                 * node_list = IPTKAPP_NULL;
	icf_string_list_node_st                 *node_list1 = IPTKAPP_NULL;
	icf_string_list_node_st                 *node_list2 = IPTKAPP_NULL;
	int                                     size;
	icf_list_st	                            *index_ptr = IPTKAPP_NULL;	
	icf_list_st	  							*p_audio_stream_data = IPTKAPP_NULL;


    /* Memory to be allocated for buffer i.e. to be sent towards IPTK */
	IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_app_add_req_st),ret_val,p_err);

	size = sizeof(icf_app_add_req_st);

	if(ret_val==IPTKAPP_FAILURE)
	{
		IPTKAPP_PRINTF("IPTKAPP:-   MEMORY ALLOC FAIL : APP_ADD_REQ");
		return(IPTKAPP_FAILURE);
	}
	p_payload=(icf_app_add_req_st * )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));


    /* Indicates the presence of optional parameters in the API*/
	p_payload->bit_mask = 0;

    /* Application name, used for tracing purposes */
	iptkapp_port_strncpy((icf_int8_t *)p_payload->app_name.str,\
            (icf_int8_t *)"Voice Application",\
            iptkapp_port_strlen((icf_int8_t *)"Voice Application")+1);

	p_payload->app_name.str_len = iptkapp_port_strlen((icf_int8_t *)\
                                                 "Voice Application");

    
    /* Contains the IPC information to reach application. */
	p_payload->routing_info.len = sizeof(icf_route_info_st);

	/* allocate memory for icf_route_info_st*/
	p_payload->routing_info.ptr = iptkapp_port_malloc(sizeof(icf_route_info_st));
	
    /* UPD will be used for communication bewteen IPTK App and IPTK */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->type=ICF_IPC_UDP;
	  
    /* addr_type could be domain name or IPV4 address */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;

    /* IP address is being filled like octet[1] =172, octet[2] = 16 */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_1\
        = atoi(iptkapp_port_strtok(\
            g_iptkapp_cfg_db_temp.iptk_app_ip,(icf_int8_t *)".\0"));/*octet_info[0];*/

	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_2\
        = atoi(iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));/*octet_info[1];*/

	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_3\
        = atoi(iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));/*octet_info[2];*/

	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.ip.addr.ipv4_addr.octet_4\
        = atoi(iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));/*octet_info[3];*/

	/* IPTKAPP Port */
	((icf_route_info_st*)(p_payload->routing_info.ptr))->ipc.udp.port\
        = g_iptkapp_cfg_db_temp.iptk_app_port ;

	/* IP Phone Toolkit uses filter to decide the application corresponding to 
     * the message obtained from the network and where call_id field is not 
     * maintained by IP Phone Toolkit.
     * Currently no filter used & no header attached*/
	p_payload->filter.count = 0;
	p_payload->filter.p_filter_list = IPTKAPP_NULL;

    /* All the APIs from IP Phone Toolkit to the application includes mandatory
     * parameter and provision for additional headers as desired by application.
     * All the additional headers required by the application are specified in 
     * this field.This field is a List of additional headers that the application
     * wants to receive in the APIs from IP Phone Toolkit if present in the 
     * incoming SIP message.
     */
	p_payload->header_name_list.count = 1;
	p_payload->header_name_list.hdr = iptkapp_port_malloc(sizeof(icf_header_name_st));
	p_payload->header_name_list.hdr->hdr_name.str_len = iptkapp_port_strlen((icf_int8_t *)"Content-Type");
	iptkapp_port_strncpy((icf_int8_t *)p_payload->header_name_list.hdr->hdr_name.str,(icf_int8_t *)\
            "Content-Type",iptkapp_port_strlen((icf_int8_t *)"Content-Type")+1);
	p_payload->header_name_list.hdr->next = IPTKAPP_NULL;

    
    /* Specifies the list of methods, supported by application, to be
     * sent in “Allow” header in outgoing requests. If the application
     * does not specify any method, then IP Phone Toolkit will 
     * return failure response to this API.
     */
	p_payload->supported_methods.count=3;

	node_list = iptkapp_port_malloc(sizeof(icf_string_list_node_st));
	node_list->str.str_len = iptkapp_port_strlen((icf_int8_t *)"INVITE");
	iptkapp_port_strncpy((icf_int8_t *)node_list->str.str,(icf_int8_t *)"INVITE",\
            iptkapp_port_strlen((icf_int8_t *)"INVITE")+1);
	p_payload->supported_methods.p_list = node_list;

	node_list1 = iptkapp_port_malloc(sizeof(icf_string_list_node_st));
	node_list1->str.str_len = iptkapp_port_strlen((icf_int8_t *)"NOTIFY");
	iptkapp_port_strncpy((icf_int8_t *)node_list1->str.str,(icf_int8_t *)"NOTIFY",\
            iptkapp_port_strlen((icf_int8_t *)"NOTIFY")+1);
	node_list1->p_next = IPTKAPP_NULL;
	p_payload->supported_methods.p_list->p_next = node_list1;

    node_list2 = iptkapp_port_malloc(sizeof(icf_string_list_node_st));
	node_list2->str.str_len = iptkapp_port_strlen((icf_int8_t *)"MESSAGE");
	iptkapp_port_strncpy((icf_int8_t *)node_list2->str.str,(icf_int8_t *)"MESSAGE",\
            iptkapp_port_strlen((icf_int8_t *)"MESSAGE")+1);
	node_list2->p_next = IPTKAPP_NULL;
	p_payload->supported_methods.p_list->p_next->p_next = node_list2;


    /* Specifies the list of events application supports for 
     * incoming dialog requests. If application does not specify
     * any supported event, then the count should be 0.
     */
	p_payload->supported_events.count=1;
	p_payload->supported_events.p_list = iptkapp_port_malloc(sizeof(icf_string_list_node_st));
	p_payload->supported_events.p_list->str.str_len = iptkapp_port_strlen(\
            (icf_int8_t *)"message-summary");
	iptkapp_port_strncpy((icf_int8_t *)p_payload->supported_events.p_list->str.str,\
            (icf_int8_t *)"message-summary",\
            iptkapp_port_strlen((icf_int8_t *)"message-summary")+1);
	p_payload->supported_events.p_list->p_next = IPTKAPP_NULL;


    /* Specifies the list of message body content types, 
     * supported by application, to be sent in “Accept” header in outgoing requests.
     * If application does not specify any supported content type,
     * then the count should be 0.
     */
	p_payload->supported_content_types.count=2;
	p_payload->supported_content_types.p_list = iptkapp_port_malloc(sizeof\
            (icf_string_list_node_st));
	p_payload->supported_content_types.p_list->str.str_len = iptkapp_port_strlen\
                            ((icf_int8_t *)"application/simple-message-summary");
	iptkapp_port_strncpy((icf_int8_t *)p_payload->supported_content_types.p_list->str.str,\
            (icf_int8_t *)"application/simple-message-summary",\
            iptkapp_port_strlen((icf_int8_t *)"application/simple-message-summary")+1);
	p_payload->supported_content_types.p_list->p_next = IPTKAPP_NULL;

	p_payload->supported_content_types.p_list->p_next = iptkapp_port_malloc(\
            sizeof(icf_string_list_node_st));
	p_payload->supported_content_types.p_list->p_next->str.str_len = iptkapp_port_strlen\
                                                        ((icf_int8_t *)"application/sdp");
	iptkapp_port_strncpy((icf_int8_t *)p_payload->supported_content_types.p_list->p_next->str.str,\
            "application/sdp",iptkapp_port_strlen((icf_int8_t *)"application/sdp")+1);
	p_payload->supported_content_types.p_list->p_next->p_next = IPTKAPP_NULL;

    /*Specifies the list of message body encoding methods, supported by 
     * application, to be sent in “Accept-Encoding” header in the
     * outgoing requests. If application does not specify any 
     * supported encoding, then the count should be 0.
     */
	p_payload->supported_encodings.count=0;


    /* Specifies the list of message body language, supported 
     * by application, to be sent in “Accept-Language” header in 
     * outgoing requests. If application does not specify any 
     * supported languages, then the count should be 0.
     */
	p_payload->supported_languages.count=0;

	p_payload->rtp_detection_reqd = 0;

    /* This structure below defines the media profile for an application. 
     * The application can define its preference for media streams, 
     * codecs (and there parameters) within a media stream using this API.
     * IP Phone Toolkit will save this information within its database. 
     * The application is expected to define only those codecs in a stream,
     * which have been already configured with MM (by the application).
     * This is a list of the data type, icf_stream_capab_st.
     */
	p_payload->bit_mask |= ICF_APP_ADD_MEDIA_PROFILE_PRESENT;

	/* Fill info for audio stream */
	p_audio_stream_data  = iptkapp_port_malloc(sizeof(icf_list_st));
	p_audio_stream_data->p_data = iptkapp_port_malloc(sizeof(icf_stream_capab_st));
				 
	((icf_stream_capab_st*)p_audio_stream_data->p_data)->bit_mask = 0; 

	((icf_stream_capab_st*)p_audio_stream_data->p_data)->stream_id = 
        IPTKAPP_CALL_TYPE_AUDIO_CALL; /*Can be generated dynamically is required in future*/

	((icf_stream_capab_st*)p_audio_stream_data->p_data)->stream_type = 
        ICF_STREAM_TYPE_AUDIO;

	p_audio_stream_data->p_next = IPTKAPP_NULL;	
	index_ptr = p_audio_stream_data;

	p_payload->p_media_profile = index_ptr;


    /* Posting the message to port layer */
	ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg, ICF_APP_ADD_REQ,0,p_err);


	iptkapp_port_free(p_audio_stream_data->p_data);
	iptkapp_port_free(p_audio_stream_data);
    iptkapp_port_free(p_payload->supported_events.p_list);
	iptkapp_port_free(p_payload->routing_info.ptr);
    iptkapp_port_free(p_payload->header_name_list.hdr);
	iptkapp_port_free(node_list);
    iptkapp_port_free(node_list1);
    iptkapp_port_free(node_list2);
    iptkapp_port_free(p_payload->supported_content_types.p_list->p_next);
    iptkapp_port_free(p_payload->supported_content_types.p_list);

	iptkapp_port_free(p_msg);

	if(ret_val==IPTKAPP_FAILURE)
	{
		IPTKAPP_PRINTF("EXIT : APPLICATION_ADD_REQ");
		return(IPTKAPP_FAILURE);
	}
	
	return(IPTKAPP_SUCCESS);
}


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

iptkapp_return_t iptkapp_cfg_send_init_add_proxy_address_iptk(iptkapp_error_t *p_err)
{

	icf_msg_st  *p_msg;
	iptkapp_return_t ret_val;
	icf_set_transport_req_st   *p_payload;

    IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_set_transport_req_st),ret_val,p_err);
	
	if(IPTKAPP_FAILURE == ret_val)
	{
		IPTKAPP_PRINTF("MEMORY ALLOC FAIL : SET_TRANSPORT_PARAM_REQ");
		return(IPTKAPP_FAILURE);
	}

	p_payload=(icf_set_transport_req_st* )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;

    /* Contains, the presence information of optional fields in this structure.*/
    p_payload->bit_mask = ICF_SET_TRP_REQ_PROXY_ADDR;

    /*Indicates the SIP proxy IP address. 
     * This parameter is configured per line.
     * This is an optional field. 
     * IP PHONE TOOLKIT also supports direct routing 
     * in which case proxy address is optional.
     */
    p_payload->proxy_addr.line_id = 0;
    p_payload->proxy_addr.action = ICF_CONFIG_ACTION_ADD;

    p_payload->proxy_addr.transport_addr.addr.addr_type=ICF_TRANSPORT_ADDRESS_IPV4_ADDR;

    /* IP address is being filled like octet[1] =172, octet[2] = 16 */
    p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_1=atoi(\
            iptkapp_port_strtok(g_iptkapp_cfg_db_temp.proxy_ip,(icf_int8_t *)".\0"));

    p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_2=atoi(\
            iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_3=atoi(\
            iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_4=atoi(\
            iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->proxy_addr.transport_addr.port_num = g_iptkapp_cfg_db_temp.proxy_port;
    p_payload->proxy_addr.transport_mode = ICF_TRANSPORT_TYPE_UDP;
	
         
    /* Indicates the mode of transport for incoming calls. 
     * The default mode is ICF_TRANSPORT_MODE_BOTH_UDP_TCP.
     * This is an optional field.
     */
    p_payload->bit_mask |= ICF_SET_TRP_REQ_SLF_MODE ;
	p_payload->self_mode=ICF_TRANSPORT_TYPE_UDP;
	

    /* Denotes the self-IP address.This is an optional field. 
     * This parameter can be optional if IP Phone Toolkit 
     * can obtain Self-IP address.
     */
    /* IP address is being filled like octet[1] =172, octet[2] = 16 */
    p_payload->bit_mask |= ICF_SET_TRP_REQ_SLF_TRANSPORT_ADDRESS ;
    p_payload->self_transport_address.addr_type=ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
    p_payload->self_transport_address.addr.ipv4_addr.octet_1=atoi(\
            iptkapp_port_strtok(g_iptkapp_cfg_db_temp.iptk_ip,(icf_int8_t *)".\0"));

    p_payload->self_transport_address.addr.ipv4_addr.octet_2=atoi(\
            iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->self_transport_address.addr.ipv4_addr.octet_3=atoi(\
            iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->self_transport_address.addr.ipv4_addr.octet_4=atoi(\
            iptkapp_port_strtok(IPTKAPP_NULL,(icf_int8_t *)".\0"));

    /* Indicates the port number of the SIP phone for receiving
     * SIP messages. The default port number is 5060.
     * This is an optional field.
     */
    p_payload->bit_mask |= ICF_SET_TRP_REQ_SLF_PORT ;
    p_payload->self_port= g_iptkapp_cfg_db_temp.iptk_nw_port;
	
	
    /* Posting the message to port layer */
    ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg, ICF_SET_TRANSPORT_REQ,0,p_err);

	iptkapp_port_free(p_msg);
	return ret_val;
}


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

iptkapp_return_t iptkapp_cfg_send_init_add_registrar_address_iptk(iptkapp_error_t *p_err)
{
	icf_msg_st					 *p_msg;
	icf_return_t				 ret_val;
	icf_set_registration_req_st  *p_payload;
	
	IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_set_registration_req_st  ),ret_val,p_err);

	if(ret_val==IPTKAPP_FAILURE)
	{
		IPTKAPP_PRINTF("MEMORY ALLOC FAIL : SET_REGISTRATION_PARAM_REQ");
		return(IPTKAPP_FAILURE);
	}


	p_payload=(icf_set_registration_req_st* )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));

    /* Specifies the line id for which the registration parameters are
     * applicable. If the value is ICF_ALL_LINES, the parameters 
     * specified in this API is applicable for all lines configured.
     */
	p_payload->line_id = 0;


    /* Actions can be add, delete or modify.
     *
     * If action is add then registrar address is mandatory. 
     * Other params are optional.
     *
     * If action is modify, any parameter other then 
     * registrar_address can be present.
     *
     * If action is delete, then no other param is required. 
     * All the registration data for the line is deleted.
     */
	p_payload->action = ICF_CONFIG_ACTION_ADD;

    /* Contains, the presence information of optional fields in this structure*/
	p_payload->bit_mask=0;


	p_payload->bit_mask |= ICF_SET_REG_REQ_MODE ;

    /* Indicates the SIP registrar IP address. This field 
     * contains significant value only when action is add.
     */
    p_payload->registrar_addr.addr.addr_type=ICF_TRANSPORT_ADDRESS_IPV4_ADDR;

    /* IP address is being filled like octet[1] =172, octet[2] = 16 */
    p_payload->registrar_addr.addr.addr.ipv4_addr.octet_1=atoi(iptkapp_port_strtok\
            (g_iptkapp_cfg_db_temp.registrar_ip,(icf_int8_t *)".\0"));

    p_payload->registrar_addr.addr.addr.ipv4_addr.octet_2=atoi(iptkapp_port_strtok\
            (IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->registrar_addr.addr.addr.ipv4_addr.octet_3=atoi(iptkapp_port_strtok\
            (IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->registrar_addr.addr.addr.ipv4_addr.octet_4=atoi(iptkapp_port_strtok\
            (IPTKAPP_NULL,(icf_int8_t *)".\0"));

    p_payload->registrar_addr.port_num = g_iptkapp_cfg_db_temp.registrar_port ;

    /* Indicates the mode of transport for registration request
     */
    p_payload->reg_mode=ICF_TRANSPORT_TYPE_UDP;

    /* Indicates the duration of registration in milliseconds. 
     * This parameter is configured per line. The 0xffff 
     * value indicates infinite duration for registration. 
     * Default value of registration_duration is 300000 ms.*/
    p_payload->bit_mask |= ICF_SET_REG_REQ_DURATION;
	p_payload->registration_duration= g_iptkapp_cfg_db_temp.registrar_duration;

	
	ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg,ICF_SET_REGISTRATION_REQ,0,p_err);

	iptkapp_port_free(p_msg);

	if(ret_val==IPTKAPP_FAILURE)
	{
		return(IPTKAPP_FAILURE);
	}


	return(IPTKAPP_SUCCESS);
}


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

iptkapp_return_t iptkapp_cfg_send_init_add_public_userid_iptk(iptkapp_error_t *p_err)
{
	icf_msg_st  *p_msg;
	iptkapp_return_t ret_val;
	icf_set_self_id_req_st *p_payload = IPTKAPP_NULL;
	icf_int8_t *addr = IPTKAPP_NULL;
    icf_int8_t *p_self_name;
    icf_int8_t self_addr_temp[IPTKAPP_MAX_REQUEST_URL_LEN];

	IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_set_self_id_req_st ),ret_val,p_err);

	if(ret_val==IPTKAPP_FAILURE)
	{
		IPTKAPP_PRINTF("MEMORY ALLOC FAIL : SET_SELF_ID_PARAM_REQ");
		return(IPTKAPP_FAILURE);
	}

	p_payload=(icf_set_self_id_req_st* )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));

    /* Contains the presence information of optional fields in this
     * structure, bitwise.
     */
	p_payload->bit_mask=0;


    /* Specifies the line id for which the parameters are applicable.
     * The special value of ICF_ALL_LINES signifies that the 
     * specified values are applicable for all lines.
     */
	p_payload->line_id=0;

    p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_NAME ;

    p_payload->line_id=0;

    /* This field specifies the action for self_addr
     * and self_name field. Action can be add or delete.
     */
    p_payload->action=ICF_CONFIG_ACTION_ADD;

    /* Contains the user name for each line. 
     * These can be same for more than one line.
     */
    iptkapp_port_strcpy(self_addr_temp,(icf_int8_t *)\
            g_iptkapp_cfg_db_temp.user_address);
    p_self_name = iptkapp_port_strtok(self_addr_temp, (icf_int8_t *)"@");

    p_payload->self_name.str_len=iptkapp_port_strlen(p_self_name);
    iptkapp_port_strcpy((icf_int8_t *)p_payload->self_name.str,p_self_name);


    p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_ADDR;

    p_payload->line_id = 0;
    p_payload->action= ICF_CONFIG_ACTION_ADD;

    /* Contains self-address for the line. 
     * The same address can be used for more than one line. 
     */
    p_payload->self_addr.addr_val.str_len = iptkapp_port_strlen((icf_int8_t *)\
            g_iptkapp_cfg_db_temp.user_address); 

    iptkapp_port_strcpy((icf_int8_t *)p_payload->self_addr.addr_val.str,(icf_int8_t *)\
            g_iptkapp_cfg_db_temp.user_address);

    p_payload->self_addr.addr_type =  ICF_ADDRESS_PLAN_SIP_URI; 

    p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_AUTH_KEY;
    iptkapp_port_strcpy((icf_int8_t *)p_payload->authentication_key.str,g_iptkapp_cfg_db_temp.authentication_key);
    p_payload->authentication_key.str_len = iptkapp_port_strlen(g_iptkapp_cfg_db_temp.authentication_key);

    p_payload->bit_mask |= ICF_SET_SLF_ID_PRM_AUTH_PASSWORD;
    iptkapp_port_strcpy((icf_int8_t *)p_payload->authentication_password.str,g_iptkapp_cfg_db_temp.authentication_password);
    p_payload->authentication_password.str_len = iptkapp_port_strlen(g_iptkapp_cfg_db_temp.authentication_password);


    iptkapp_port_free(addr);

	ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg, ICF_SET_SELF_ID_REQ,0,p_err);
	
    iptkapp_port_free(p_msg);
	if(ret_val==IPTKAPP_FAILURE)
	{
		return(IPTKAPP_FAILURE);
	}
	

	return(IPTKAPP_SUCCESS);
}


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

iptkapp_return_t iptkapp_cfg_send_register_req(icf_uint32_t duration, iptkapp_error_t *p_err)
{

	icf_msg_st  *p_msg;
	iptkapp_return_t ret_val;
	icf_register_req_st  *p_payload;

    IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_register_req_st),ret_val,p_err);
	
	if(IPTKAPP_FAILURE == ret_val)
	{
		IPTKAPP_PRINTF("MEMORY ALLOC FAIL : REGISTER_REQ");
		return(IPTKAPP_FAILURE);
	}

	p_payload=(icf_register_req_st* )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));

    /* Indicates the presence of optional parameters in the API */
    p_payload->bit_mask = 0;

    p_payload->bit_mask |= ICF_REG_REQ_EXPIRY_PRESENT;
    p_payload->line_id = 0;


    /* This parameter specifies the expiry to be sent in the
     * SIP REGISTER message. If application does not specify this
     * parameter, then previously configured value
     * (using Api ICF_SET_REGISTRATION_REQ) will be used.
     * This parameter is mandatory in case the application needs to 
     * de-register. In this case, the expiry must be set to 0. For 
     * registration, specify a large non-zero value. 
     * The unit of expiry duration is in milli seconds.
     */
    p_payload->expiry = duration;
	
	ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg, ICF_REGISTER_REQ,ICF_INVALID_CALL_ID,p_err);

	iptkapp_port_free(p_msg);
	return ret_val;
}

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

iptkapp_return_t iptkapp_cfg_publish_req( iptkapp_error_t *p_err, icf_int16_t publish_state )
{


	icf_msg_st  *p_msg;
	iptkapp_return_t ret_val;
    icf_publish_req_st *p_payload;
    icf_uint8_t *p_dest,*p_self_id;
    IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_publish_req_st),ret_val,p_err);
	
	if(IPTKAPP_FAILURE == ret_val)
	{
		IPTKAPP_PRINTF("MEMORY ALLOC FAIL : PUBLISH_REQ");
		return(IPTKAPP_FAILURE);
	}

	p_payload=(icf_publish_req_st* )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));
  
	/* Indicates the presence of optional parameters in the API */
    p_payload->bit_mask=0;

	/* specifies the count of any additional headers to be attached*/
    p_payload->header_list.count = 0;
    p_payload->header_list.hdr   = ICF_NULL;


	/* specifies the count of any additional tags to be attached*/
    p_payload->tag_list.count = 0;
    p_payload->tag_list.tag   = ICF_NULL;

	/* specifies the count of any additional body list to be attached*/
    p_payload->body_list.count = 0;
    p_payload->body_list.body   = ICF_NULL;

    
	/* we are removing the sip: from destination info.
	This field specifies the destination to which publish is issued*/
    p_dest = g_iptkapp_publish_db.destination_info.addr_val.str;

    if(IPTKAPP_FAILURE == iptkapp_core_remove_type_from_addr((icf_uint8_t *)p_dest))
    {
        IPTKAPP_PRINTF("\nEnter Valid SIP URL\n");
    }
    p_dest++;


	/* we are removing the sip: from user address.
	This field specifies the user address for which publish is issued*/
	p_self_id = g_iptkapp_publish_db.user_address.addr_val.str;

    if(IPTKAPP_FAILURE == iptkapp_core_remove_type_from_addr((icf_uint8_t *)p_self_id))
    {
		IPTKAPP_PRINTF("\nEnter Valid SIP URL\n");
	}
    p_self_id++;


    /* For header list*/
 
    /* Allocate memory for the header linked list if state is Refresh,Modify or Remove*/
    if((publish_state == REFRESH)||(publish_state == MODIFY)||(publish_state == REMOVE))
    {
        p_payload->bit_mask|=ICF_HDR_LIST_PRESENT;
        p_payload->header_list.hdr = iptkapp_port_malloc(sizeof(icf_header_st));
        p_payload->header_list.hdr->next = ICF_NULL;
        p_payload->header_list.count = 1;
        iptkapp_port_strcpy((icf_int8_t *)p_payload->header_list.hdr->hdr_name.str,"SIP-If-Match");
        p_payload->header_list.hdr->hdr_name.str_len = iptkapp_port_strlen("SIP-If-Match");
    }

    
    /*Body list*/
        
    if((publish_state == INITIAL)||(publish_state == MODIFY))
    {
        p_payload->bit_mask|=ICF_BODY_LIST_PRESENT;
        p_payload->body_list.count = 1;
        iptkapp_port_strcpy((icf_int8_t *)p_payload->body_list.content_type.str, "application/simple-message-summary"); 
        p_payload->body_list.content_type.str_len = iptkapp_port_strlen("application/simple-message-summary");
        p_payload->body_list.body = (icf_msg_body_st*)iptkapp_port_malloc(sizeof(icf_msg_body_st));
        p_payload->body_list.body->next = ICF_NULL;
        p_payload->body_list.body->value = (icf_uint8_t*)iptkapp_port_malloc(100);

        iptkapp_port_strcpy((icf_int8_t *)p_payload->body_list.body->value,"This is a test message");
        p_payload->body_list.body->length = iptkapp_port_strlen("This is a test message"); 
        iptkapp_port_strcpy((icf_int8_t *)p_payload->body_list.body->content_type.str,"application/simple-message-summary"); 
        p_payload->body_list.body->content_type.str_len = iptkapp_port_strlen("application/simple-message-summary"); 
    }

    /* Line id */
    p_payload->line_id = g_iptkapp_publish_db.line_id;
    
    /* Duration specifies the duration for which this publication exists*/
    if((publish_state == REFRESH)||(publish_state == MODIFY)||(publish_state == INITIAL))
    {
        p_payload->duration = g_iptkapp_publish_db.duration;
    }
    else
    {
        p_payload->duration = 0;
    }
    /* Destination Info */
    iptkapp_port_strcpy((icf_int8_t *)p_payload->destination_info.addr_val.str,(icf_int8_t *)p_dest);
    p_payload->destination_info.addr_val.str_len = iptkapp_port_strlen((icf_int8_t *)p_dest);
    p_payload->destination_info.addr_type = g_iptkapp_publish_db.destination_info.addr_type;
    
    /*Self_id_url*/
    iptkapp_port_strcpy((icf_int8_t *)p_payload->user_address.addr_val.str,(icf_int8_t *)p_self_id);
    p_payload->user_address.addr_val.str_len =iptkapp_port_strlen((icf_int8_t *)p_self_id);
    p_payload->user_address.addr_type = g_iptkapp_publish_db.user_address.addr_type;

    /* Event Package name */
    iptkapp_port_strcpy((icf_int8_t *)p_payload->event_pkg_name.str,
            (icf_int8_t *)g_iptkapp_publish_db.event_pkg_name.str);
    p_payload->event_pkg_name.str_len = g_iptkapp_publish_db.event_pkg_name.str_len;
    
    /* E_Tag Value if state is Refresh, Modify or Remove */
    if((publish_state == REFRESH)||(publish_state == MODIFY)||(publish_state == REMOVE))
    {
        iptkapp_port_strcpy((icf_int8_t *)p_payload->header_list.hdr->hdr_value.str,
                (icf_int8_t *)g_iptkapp_publish_db.header_list.hdr->hdr_value.str);
        p_payload->header_list.hdr->hdr_value.str_len = 
            g_iptkapp_publish_db.header_list.hdr->hdr_value.str_len;
    }
        ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg, ICF_PUBLISH_REQ,
                ICF_INVALID_CALL_ID,p_err);

	iptkapp_port_free(p_msg);
	return ret_val;
}

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

iptkapp_return_t iptkapp_cfg_message_req(icf_uint8_t *message_destination_info,
										 icf_uint8_t *message_user_address,
										 icf_uint8_t *user_message, 
										 iptkapp_error_t *p_err)
{


	icf_msg_st  *p_msg;
	iptkapp_return_t ret_val;
	icf_message_req_st  *p_payload;

    IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_message_req_st),ret_val,p_err);
	
	if(IPTKAPP_FAILURE == ret_val)
	{
		IPTKAPP_PRINTF("MEMORY ALLOC FAIL : MESSAGE_REQ");
		return(IPTKAPP_FAILURE);
	}

	p_payload=(icf_message_req_st* )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));

    p_payload->bit_mask=0;

    p_payload->header_list.count = 0;
    p_payload->header_list.hdr   = ICF_NULL;

    p_payload->tag_list.count = 0;
    p_payload->tag_list.tag   = ICF_NULL;

    p_payload->body_list.count = 1;
    p_payload->body_list.body   = ICF_NULL;



	/* we are removing the sip: from destination info.
	This field specifies the destination info for which message is issued*/
    if(IPTKAPP_FAILURE == iptkapp_core_remove_type_from_addr(message_destination_info))
    {
        IPTKAPP_PRINTF("\nEnter Valid SIP URL\n");
    }
    message_destination_info++;


	/* we are removing the sip: from user address.
	This field specifies the user address for which message is issued*/
    if(IPTKAPP_FAILURE == iptkapp_core_remove_type_from_addr(message_user_address))
    {
        IPTKAPP_PRINTF("\nEnter Valid Self ID URL\n");
    }
    message_user_address++;


    /*Destination Info*/
    

    iptkapp_port_strcpy((icf_int8_t *)p_payload->destination_info.addr_val.str,(icf_int8_t *)message_destination_info);
    p_payload->destination_info.addr_val.str_len = iptkapp_port_strlen((icf_int8_t *)message_destination_info);
    p_payload->destination_info.addr_type = ICF_ADDRESS_PLAN_SIP_URI;


    /*Self id Url*/
    p_payload->bit_mask|=ICF_USER_ADDRESS_PRESENT;
    iptkapp_port_strcpy((icf_int8_t *)p_payload->user_address.addr_val.str,(icf_int8_t *)message_user_address);
    p_payload->user_address.addr_val.str_len = iptkapp_port_strlen((icf_int8_t *)message_user_address);
    p_payload->user_address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
    

    /*Body list*/
    p_payload->bit_mask|=ICF_BODY_LIST_PRESENT;
    p_payload->body_list.count = 1;
    iptkapp_port_strcpy((icf_int8_t *)p_payload->body_list.content_type.str, "multipart/mixed;boundary=ICGMsgBoundary"); 
    p_payload->body_list.content_type.str_len = iptkapp_port_strlen("multipart/mixed;boundary=ICGMsgBoundary");
    p_payload->body_list.body = (icf_msg_body_st*)iptkapp_port_malloc(sizeof(icf_msg_body_st));
    p_payload->body_list.body->next = ICF_NULL;
    p_payload->body_list.body->value = (icf_uint8_t*)iptkapp_port_malloc(MAX_MESSAGE_LENGTH);

    iptkapp_port_strcpy((icf_int8_t *)p_payload->body_list.body->value,(icf_int8_t *)user_message);
    p_payload->body_list.body->length = iptkapp_port_strlen((icf_int8_t *)user_message); 
    iptkapp_port_strcpy((icf_int8_t *)p_payload->body_list.body->content_type.str,
                                                                   (icf_int8_t *)"text/plain"); 
    p_payload->body_list.body->content_type.str_len = iptkapp_port_strlen((icf_int8_t *)"text/plain"); 

    /*Header List*/
    p_payload->bit_mask|=ICF_HDR_LIST_PRESENT;
    p_payload->header_list.count=1;
    p_payload->header_list.hdr = (icf_header_st*)iptkapp_port_malloc(sizeof(icf_header_st));
    p_payload->header_list.hdr->next = ICF_NULL;
    iptkapp_port_strcpy((icf_int8_t *)p_payload->header_list.hdr->hdr_name.str,"Content-Description");
    p_payload->header_list.hdr->hdr_name.str_len = iptkapp_port_strlen("Content-Description");
    iptkapp_port_strcpy((icf_int8_t *)p_payload->header_list.hdr->hdr_value.str,"This is simple test message");
    p_payload->header_list.hdr->hdr_value.str_len = iptkapp_port_strlen("This is simple test message");





	ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg, ICF_MESSAGE_REQ,ICF_INVALID_CALL_ID,p_err);

	iptkapp_port_free(p_msg);
	return ret_val;
}
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

iptkapp_return_t iptkapp_print_publish_resp(icf_msg_st* p_msg)
{
    icf_publish_resp_st *p_publish_resp = ICF_NULL;
    
    p_publish_resp = ( icf_publish_resp_st *)p_msg->payload;
    
    if(IPTKAPP_SUCCESS == p_publish_resp->result)
    {
        IPTKAPP_PRINTF("\n\tPUBLISH request successful at the peer end\n");
        IPTKAPP_PRINTF("\n\tResponse Code :- %d ,\n\tError Code :- %d,\n\tEntity Tag :- %s,\n\tDuration :- %d ",p_publish_resp->response_code,p_publish_resp->error_code,p_publish_resp->entity_tag.str,p_publish_resp->duration);
        return IPTKAPP_SUCCESS;
    }
    else
    {
        IPTKAPP_PRINTF("\nPUBLISH request failed at the peer end\n");
        return IPTKAPP_FAILURE;
    }

}
/*  CSR 1-5838506 Meged */
iptkapp_return_t iptkapp_cfg_send_service_params_iptk(iptkapp_error_t *p_err)
{

    icf_msg_st  *p_msg;
	iptkapp_return_t ret_val;
	icf_set_service_params_req_st  *p_payload;

    IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_set_service_params_req_st),ret_val,p_err);
	
	if(IPTKAPP_FAILURE == ret_val)
	{
		IPTKAPP_PRINTF("MEMORY ALLOC FAIL : REGISTER_REQ");
		return(IPTKAPP_FAILURE);
	}

	p_payload=(icf_set_service_params_req_st* )(IPTKAPP_MSG_GET_PAYLOAD(p_msg));

    /* Indicates the presence of optional parameters in the API */
    p_payload->bit_mask = 0;
    p_payload->bit_mask |= ICF_SET_SER_PRM_SUBSCRIPTION_FLAG;
    p_payload->bit_mask |= ICF_SET_SER_PRM_ACTIVATION_FLAG;
    p_payload->bit_mask |= ICF_SET_SER_PRM_CFU_ADDR;
    p_payload->bit_mask |= ICF_SET_SER_PRM_CFD_ADDR;
    p_payload->bit_mask |= ICF_SET_SER_PRM_CFB_ADDR;
    p_payload->service_activation_flag.line_id = ICF_ALL_LINES;
    p_payload->service_activation_flag.status = ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL;
    p_payload->service_subscription_flag.line_id = ICF_ALL_LINES;
    p_payload->service_subscription_flag.status = ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL;
    p_payload->addr_for_cfu.line_id = ICF_ALL_LINES;
    p_payload->addr_for_cfu.address.addr_type = 3;
    strcpy((icf_int8_t *)((icf_string_st *)p_payload->addr_for_cfu.address.addr_val.str), 
            (icf_int8_t*)"iptk@aricent.com");
    p_payload->addr_for_cfu.address.addr_val.str_len = (icf_uint16_t)strlen("iptk@aricent.com");

    p_payload->addr_for_cfb.line_id = ICF_ALL_LINES;
    p_payload->addr_for_cfb.address.addr_type = 3;
    strcpy((icf_int8_t *)((icf_string_st *)p_payload->addr_for_cfb.address.addr_val.str), 
            (icf_int8_t*)"iptk@aricent.com");
    p_payload->addr_for_cfb.address.addr_val.str_len = (icf_uint16_t)strlen("iptk@aricent.com");

    p_payload->addr_for_cfd.line_id = ICF_ALL_LINES;
    p_payload->addr_for_cfd.address.addr_type = 3;
    strcpy((icf_int8_t *)((icf_string_st *)p_payload->addr_for_cfd.address.addr_val.str), 
            (icf_int8_t*)"iptk@aricent.com");
    p_payload->addr_for_cfd.address.addr_val.str_len = (icf_uint16_t)strlen("iptk@aricent.com");


    ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg, ICF_SET_SERVICE_PARAMS_REQ,0,p_err);
	
    iptkapp_port_free(p_msg);
	if(ret_val==IPTKAPP_FAILURE)
	{
		return(IPTKAPP_FAILURE);
	}
	

	return(IPTKAPP_SUCCESS);
}



