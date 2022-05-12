/*****************************************************************************
 **
 ** FILENAME:		iptkapp_main.c
 **
 ** DESCRIPTION:	This file contains the main for Sample Program.
 **
 ** DATE			NAME	        	REFERENCE		REASON
 ** ----			----		        ---------		------
 **	05-DEC-07		Amit Sikka   		            	Initial Creation
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

#include "iptkapp_port_prototypes.h"
#include "iptkapp_macros.h"
#include "icf_types.h"
#include "icf_api.h"
#include "iptkapp_port_wrapper.h"
#include "iptkapp_glb.h"
#include "iptkapp_core_prototypes.h"
#include "iptkapp_core_log.h"
#include "icf_clib.h"

#ifdef ICF_PORT_WINDOWS
#include <windows.h>
#include <winsock2.h>
#include <conio.h>
#endif

/****************************************************************************/
/******************************************************************************
* This is the main starting function of Sample Program. 
******************************************************************************/

icf_int32_t main(void)
/*int main(void)*/ 
{
    icf_boolean_t timeout_flag;
    icf_uint32_t usec = IPTKAPP_SELECT_INFINITE;
    icf_return_t  ret_val;

    icf_uint8_t *p_buf = IPTKAPP_NULL;
    iptkapp_error_t ecode = 0;

    /* During initialisation IPTK App first needs to know the configuration
     * Paramters. These Paramters will be used to initialise IPTK App and send 
     * APP_ADD_REQ, SET_TRANSPORT_PARAMS, SET_SELF_ID towards IP Phone toolkit
     */
    if (IPTKAPP_FAILURE == iptkapp_cfg_read_config_file())
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > Reading from config file failed \n");
        return IPTKAPP_FAILURE;
    }

    /*This Function initialises the socket level functionality
     */
    if (IPTKAPP_SUCCESS != iptkapp_port_udp_init())
    {
        return IPTKAPP_FAILURE;
    }
    
    /*This Function initialises the cfg module
     */
    if (IPTKAPP_SUCCESS != iptkapp_cfg_init(&ecode))
    {
        return IPTKAPP_FAILURE;
    }

    if (IPTKAPP_SUCCESS != iptkapp_trace_init(&ecode))
    {
        return IPTKAPP_FAILURE;
    }

    /* infinite select loop polls between keyboard input and messages from IPTK 
     */
    for(;;)
    {
        ret_val = iptkapp_port_select(usec,&timeout_flag);
        if (IPTKAPP_SUCCESS == ret_val)
        {

			if(FD_ISSET(g_fd,&fdset))
            {
                if (IPTKAPP_SUCCESS != iptkapp_port_udp_recv(&p_buf))
                {
                    return IPTKAPP_FAILURE;
                }

                iptkapp_process_message((icf_msg_st *)p_buf);
                icf_free_decoded_msg((icf_msg_st *)p_buf,&ecode);
            }
#ifndef ICF_PORT_WINDOWS
			else if(FD_ISSET(ttyfd,&fdset))
            {
                /* Input from screen CLI */
                iptkapp_core_handle_display_menu();
            }
            
#endif      
        }
#ifdef ICF_PORT_WINDOWS
		if (_kbhit())
		{
			iptkapp_core_handle_display_menu();
		}
#endif
    }

    return IPTKAPP_SUCCESS;
}

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

iptkapp_return_t iptkapp_process_message(icf_msg_st *p_msg)
{
	icf_api_id_t	api_id,cfm_api_id;
	icf_configure_cfm_st  *p_resp_icf = IPTKAPP_NULL;
    iptkapp_error_t ecode;

	icf_uint8_t *p_resp = IPTKAPP_NULL; 
	icf_module_id_t source_id;

	if(p_msg == IPTKAPP_NULL)
	{
		return(IPTKAPP_FAILURE);
	}


	source_id=IPTKAPP_MSG_GET_SRC(p_msg);
	api_id  = IPTKAPP_MSG_GET_API(p_msg);

    switch(api_id)
    {
        case ICF_APP_ADD_CFM :

            {
                p_resp = IPTKAPP_MSG_GET_PAYLOAD(p_msg);

                /* Verifying the result fot APP_ADD_REQ */
                if( ICF_FALSE == ((icf_app_add_cfm_st*)p_resp)->result)
                {
                    IPTKAPP_PRINTF("\n\tIPTKAPP> APPLICATION ADD FAILURE\n");
                    return IPTKAPP_FAILURE;
                }
                else
                {
                    g_app_id = IPTKAPP_MSG_GET_APP_ID(p_msg);
                    IPTKAPP_PRINTF("\n\tIPTKAPP> APP_ADD_REQ Successfull\n");
                }
            }
            break;

        case ICF_REGISTER_CFM :

            {
                p_resp = IPTKAPP_MSG_GET_PAYLOAD(p_msg);

                /* Waiting for successfull response of registration with registrar */
                if( ((icf_register_cfm_st*)p_resp)->result == ICF_FALSE)
                {
                    IPTKAPP_PRINTF("\n\tIPTKAPP> REGISTER_CFM FAILURE\n");
                    return IPTKAPP_FAILURE;
                }
                else
                {
                    if( ((icf_register_cfm_st*)p_resp)->request_type == 
                            ICF_REQUEST_TYPE_DEREGISTER)
                    {
                        IPTKAPP_PRINTF("\n\tIPTKAPP> Succesful Response received from REGISTRAR\n");
                        return IPTKAPP_SUCCESS;
                    }
                    else
                    {
                        IPTKAPP_PRINTF("\n\tIPTKAPP> Succesful Response received from REGISTRAR \n");
                        return IPTKAPP_SUCCESS;
                    }

                }

            }
            break;

        case ICF_REGISTER_STATUS_IND :

            {
                /* IP Phone Toolkit sends unsolicited registration failure 
                 * indications to the application. These are sent in case 
                 * of network initiated de-registration or when 
                 * re-registration attempts are failed. 
                 * “IP Phone Toolkit” performs re-registrations on 
                 * behalf of the application. Re-registrations are 
                 * started automatically after a successful initial registration. 
                 * Initial registration is triggered by the application using 
                 * Register-Request API.
                 */

                p_resp = IPTKAPP_MSG_GET_PAYLOAD(p_msg);
                if( ((icf_register_status_ind_st*)p_resp)->result == ICF_FALSE)
                {
                    IPTKAPP_PRINTF("\n\tIPTKAPP> REGISTER_STATUS_IND FAILURE from Registrar\n");
                    return IPTKAPP_FAILURE;
                }
            }
            break;

        case ICF_CONFIGURE_CFM:

            /* ICF_CONFIGURE_CFM can have the response for a number of
             * configuration APIs. Therefor e verifying the result on basis 
             * of req-id.
             */
            p_resp_icf=(icf_configure_cfm_st *)IPTKAPP_MSG_GET_PAYLOAD(p_msg);

            cfm_api_id = p_resp_icf->api_id;
            switch(cfm_api_id)
            {
                case ICF_SET_REGISTRATION_REQ:

                    {
                        p_resp_icf=(icf_configure_cfm_st *)IPTKAPP_MSG_GET_PAYLOAD(p_msg);
                        if( p_resp_icf->result==IPTKAPP_SUCCESS )
                        {
                            return IPTKAPP_SUCCESS;
                        }
                        else
                        {
                            IPTKAPP_PRINTF("\n\tIPTKAPP> REGISTER_RESP FAILURE from Registrar\n");
                            return IPTKAPP_FAILURE;
                        }
                    }
                    break;

                case ICF_SET_TRANSPORT_REQ:
                    {
                        p_resp_icf=(icf_configure_cfm_st *)IPTKAPP_MSG_GET_PAYLOAD(p_msg);
                        if( p_resp_icf->result==IPTKAPP_SUCCESS )
                        {
                            IPTKAPP_PRINTF("\n\tIPTKAPP> SET_TRANSPORT_REQ successfull \n");
                            return IPTKAPP_SUCCESS;
                        }
                        else
                        {
                            IPTKAPP_PRINTF("\n\tIPTKAPP> SET_TRANSPORT_REQ Failure\n");
                            return IPTKAPP_FAILURE;
                        }
                    }
                    break;

                case ICF_SET_SELF_ID_REQ:
                    {
                        p_resp_icf=(icf_configure_cfm_st *)IPTKAPP_MSG_GET_PAYLOAD(p_msg);
                        if( p_resp_icf->result==IPTKAPP_SUCCESS )
                        {
                            IPTKAPP_PRINTF("\n\tIPTKAPP> SET_SELF_ID_REQ successfull \n");
                            return IPTKAPP_SUCCESS;
                        }
                        else
                        {
                            IPTKAPP_PRINTF("\n\tIPTKAPP> SET_SELF_ID_REQ Failure\n");
                            return IPTKAPP_FAILURE;
                        }
                    }
                    break;

                case ICF_SET_CALL_PARAM_REQ:
                    {
                        p_resp_icf=(icf_configure_cfm_st *)IPTKAPP_MSG_GET_PAYLOAD(p_msg);
                        if( p_resp_icf->result==IPTKAPP_SUCCESS )
                        {
                            IPTKAPP_PRINTF("\n\tIPTKAPP> SET_CALL_PARAM_REQ successfull \n");
                            return IPTKAPP_SUCCESS;
                        }
                        else
                        {
                            IPTKAPP_PRINTF("\n\tIPTKAPP> SET_CALL_PARAM_REQ Failure\n");
                            return IPTKAPP_FAILURE;
                        }
                    }
                    break;
            }

        /* These all APIs are related to call control and 
         * will be directly routed to call state machine entry point
         */
        case ICF_PUBLISH_RESP :
        case ICF_REMOTE_USER_ALERTED_IND:
        case ICF_MEDIA_CONNECTED_IND:
        case ICF_CONNECT_IND:
        case ICF_REMOTE_USER_TERMINATE_CALL_RESP:
        case ICF_TERMINATE_CALL_IND:
        case ICF_INCOMING_CALL_IND:
        case ICF_CONNECT_RESP:
            iptkapp_core_msg_handler_cc(p_msg,&ecode);
            break;


        case ICF_REPORT_TRACE_IND:
            /* This API is sent by IPTK for reporting traces to the
             * phone application.
             */
            iptkapp_core_trace_handler(p_msg);
            break;
    }

    iptkapp_core_display_menu();

    return IPTKAPP_SUCCESS;
}






