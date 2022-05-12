/*****************************************************************************
 **
 ** FILENAME:		iptkapp_core_cli_intf.c
 **
 ** DESCRIPTION:	This file contains the command level interface of IPTK 
 **                 application which can be used to utilise various 
 **                 functionalities provided by IPTK.
 **
 ** DATE			NAME    		REFERENCE		REASON
 ** ----			----	    	---------		------
 **05-DEC-07		Amit Sikka       			Initial Creation
 ** 10-JAN-08           Vipul Varsheny  SPR 17945   CLI option for PUBLISH and MESSAGE
 ** 1-May-2008          Kamini Gangwani SPR 18498   Display screen modified 
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

#include "iptkapp_glb.h"
#include "iptkapp_core_fsm_types.h"
#include "iptkapp_core_prototypes.h"
#include "iptkapp_port_wrapper.h"
#include "iptkapp_port_prototypes.h"
#include "iptkapp_core_log.h"


icf_publish_req_st g_iptkapp_publish_db;
/******************************************************************************
 *
 * FUNCTION:        iptkapp_core_display_menu
 *
 * DESCRIPTION:     This function displays the menu to invoke various 
 *                  functionalities of IPTK
 *
 *                  IP Phone Toolkit Demo Application
 *
 *                  1) Register UA
 *                  2) De-Register UA
 *                  3) Create a call
 *                  4) Terminate a call
 *                  5) Exit
 *
 *****************************************************************************/
 
void iptkapp_core_display_menu()
{
    IPTKAPP_PRINTF ("\n\t\t\tIP Phone Toolkit Sample Application\n");

    IPTKAPP_PRINTF ("\n\t-----------------------------------------\n\t\tRegistration APIs\n\t-----------------------------------------\n");

    IPTKAPP_PRINTF ("\t 1) Register UA \n\t 2) De-Register UA \n\t ");
    fflush(stdout);

    IPTKAPP_PRINTF ("\n\t-----------------------------------------\n\t\tCall Control APIs\n\t-----------------------------------------\n");
    IPTKAPP_PRINTF("\t3) Create a call \n\t4) Terminate a call \n" );
    fflush(stdout);

    IPTKAPP_PRINTF ("\t-----------------------------------------\n\t\tOut-of-Dialog APIs\n\t-----------------------------------------\n");
    IPTKAPP_PRINTF("\n\t5) Send Publish Request\n\t6) Send Instant Message \n\n\t7) Exit \n\t" );
    fflush(stdout);

    IPTKAPP_PRINTF ("\nEnter Your Choice :  ");
    fflush(stdout);
}

/******************************************************************************
 *
 * FUNCTION:        iptkapp_core_handle_display_menu
 *
 * DESCRIPTION:     This function handles the input from menu to invoke various 
 *                  functionalities of IPTK
 *
 *                  IP Phone Toolkit Demo Application
 *
 *                  1) Register UA
 *                  2) De-Register UA
 *                  3) Create a call
 *                  4) Terminate a call
 *                  5) Exit
 *
 *****************************************************************************/
 
void iptkapp_core_handle_display_menu()
{
    icf_int8_t choice='\n';
    icf_int8_t exit_crit;
    iptkapp_core_call_data_st request_url;
    iptkapp_error_t ecode;
    static icf_int8_t registered = 0;
    icf_uint8_t message_destination_info[100];
    icf_uint8_t message_user_address[100];
    icf_uint8_t user_message[MAX_MESSAGE_LENGTH];
    icf_int16_t publish_state;

    choice = getc(stdin);
    if (choice != '\n' && choice != '\r')
        getc(stdin);

    switch (choice)
    {
        case '1':
            {
                IPTKAPP_PRINTF("\n\tPlease enter the value for duration of registration :  ");
                IPTKAPP_SCANF(" %d",&g_iptkapp_cfg_db_t.registrar_duration);
                getc(stdin);

                if (IPTKAPP_SUCCESS == iptkapp_cfg_send_register_req(g_iptkapp_cfg_db_t.registrar_duration,&ecode))
                {
                    IPTKAPP_PRINTF("\n\tRegister Request sent succesfully to Registrar server");
                    registered = 1;
                }
                else
                {
                    IPTKAPP_PRINTF("\n\tRegister Request could not be sent to Registrar server because of wrong configuration");
                }
                iptkapp_core_display_menu();
                break;
            }

        case '2':
            {
                if (1 == registered)
                {
                    g_iptkapp_cfg_db_t.registrar_duration = 0;
                    if (IPTKAPP_SUCCESS == iptkapp_cfg_send_register_req(g_iptkapp_cfg_db_t.registrar_duration,&ecode))
                    {
                        IPTKAPP_PRINTF("\n\tDe-Register Request sent succesfully to Registrar server");
                        registered = 0;
                    }
                    else
                    {
                        IPTKAPP_PRINTF("\n\tDe-Register Request could not be sent to Registrar server because of wrong configuration");
                    }
                }
                else
                {
                    IPTKAPP_PRINTF("\n\tIPTKAPP > Application is not registered with Registrar. Please Register First\n");
                }
                iptkapp_core_display_menu();
                break;
            }
        case '3':
            {
                if (1 == registered)
                {
                    IPTKAPP_PRINTF("\n\tPlease Enter Destination SIP URL \n\t(in sip format, e.g sip:abc@aricent.com) :   ");
                    IPTKAPP_SCANF("%s",request_url.calling_party_addr.str);
                    fflush(stdin);
                    request_url.calling_party_addr.str_len = iptkapp_port_strlen((icf_int8_t *)request_url.calling_party_addr.str);

                    if (IPTKAPP_SUCCESS == iptkapp_core_call_entry(&request_url,CORE_MAKE_CALL_CLI,&ecode))
                    {
                        IPTKAPP_PRINTF("\n\tCreate call Request sent succesfully to IP Phone Toolkit");
                    }
                    else
                    {
                        IPTKAPP_PRINTF("\n\tCreate call Request could not be sent to IP Phone Toolkit because of wrong configuration");
                    }
                }
                else
                {
                    IPTKAPP_PRINTF("\n\tIPTKAPP > Application is not registered with Registrar. Please Register First\n");
                }

                iptkapp_core_display_menu();
                break;
            }
        case '4':
            {
                if (IPTKAPP_SUCCESS == iptkapp_core_call_entry(&request_url,CORE_TERMINATE_CALL_CLI,&ecode))
                {
                    IPTKAPP_PRINTF("\n\tTerminate call Request sent succesfully to IP Phone Toolkit");
                }
                else
                {
                    IPTKAPP_PRINTF("\n\tTerminate call Request could not be sent to IP Phone Toolkit because of wrong configuration");
                }
                iptkapp_core_display_menu();
                break;
            }
        case '5':                       
            {
                if (1 == registered)
                {
                    publish_state = iptkapp_publish_display_menu();


                    if (IPTKAPP_SUCCESS == iptkapp_cfg_publish_req(&ecode,publish_state))
                    {
                        IPTKAPP_PRINTF("\n\tPublish Request sent succesfully to IP Phone Toolkit");
                    } 
                    else
                    {
                        IPTKAPP_PRINTF("\n\tPublish Request can not be sent to IP Phone Toolkit due to wrong configuration");
                    }

                }
                else
                {
                    IPTKAPP_PRINTF("\n\tIPTKAPP > Application is not registered with Registrar. Please Register First\n");
                }

                iptkapp_core_display_menu();
                break;
            }
        case '6':                       
            {
                if (1 == registered)
                {
                    IPTKAPP_PRINTF("\n\tPlease Enter Destination SIP URL \n\t(in sip format, e.g sip:abc@aricent.com) :   ");
                    IPTKAPP_SCANF("%s",message_destination_info);
                    fflush(stdin);

                    IPTKAPP_PRINTF("\n\tPlease Enter self id url URL \n\t(in sip format, e.g sip:abc@aricent.com) :   ");
                    IPTKAPP_SCANF("%s",message_user_address);
                    fflush(stdin);

                    IPTKAPP_PRINTF("\n\tEnter the message:- ");
                    iptkapp_port_getline(user_message,MAX_MESSAGE_LENGTH);

                    if (IPTKAPP_SUCCESS == iptkapp_cfg_message_req(message_destination_info,message_user_address,user_message,&ecode))
                    {
                        IPTKAPP_PRINTF("\n\tMessage Request sent succesfully to IP Phone Toolkit");
                    } 
                    else
                    {
                        IPTKAPP_PRINTF("\n\tMessage Request can not be sent to IP Phone Toolkit due to wrong configuration");
                    }
                }
                else
                {
                    IPTKAPP_PRINTF("\n\tIPTKAPP > Application is not registered with Registrar. Please Register First\n");
                }

                iptkapp_core_display_menu();
                break;
            }
        case '7':
            {
                IPTKAPP_PRINTF("\n\tAre You sure you want to exit (Y/N) :- ");
                IPTKAPP_SCANF(" %c",&exit_crit);
                fflush(stdin);
                if ('Y' == exit_crit || 'y' == exit_crit )
                {
                    /* For graceful exit its necessary to send BYE to the peer entity
                     * and terminate the outgoing call
                     */
                    iptkapp_core_call_entry(&request_url,CORE_TERMINATE_CALL_CLI,&ecode);
                    /*g_iptkapp_cfg_db_t.registrar_duration = 0;
                    iptkapp_cfg_send_register_req(&ecode) TBD*/
                    iptkapp_trace_close_file();

                    iptkapp_port_close();
                    exit (0);
                }
                iptkapp_core_display_menu();
                break;
            }
        default :
            {
                IPTKAPP_PRINTF("\n\tPlease Enter any valid option");
                iptkapp_core_display_menu();
                break;
            }
    } 
}


/******************************************************************************
 *
 * FUNCTION:        iptkapp_publish_display_menu
 *
 * DESCRIPTION:     This function displays the menu and take input from CLI 
 *                  for furthur options under PUBLISH request
 *
 *                  IP Phone Toolkit Demo Application
 *
 *                  1) INITIAL 
 *                  2) REFRESH
 *                  3) MODIFY
 *                  4) REMOVE
 *
 *****************************************************************************/
icf_int16_t iptkapp_publish_display_menu()
{
    icf_int16_t publish_choice;
    icf_int16_t state;

    IPTKAPP_PRINTF ("\n\n\n\n\n\n\n\t\t\tIP Phone Toolkit Sample Application\n\n");

    IPTKAPP_PRINTF ("\n\t-----------------------------------------\n\t\tPUBLISH OPTIONS\n\t-----------------------------------------\n\n");

    IPTKAPP_PRINTF ("\t1)INITIAL \n\n\t2)REFRESH  \n\t ");
    fflush(stdout);

    IPTKAPP_PRINTF("\n\t3)MODIFY \n\n\t4) REMOVE \n\n\t" );
    fflush(stdout);

    IPTKAPP_PRINTF ("Enter Your Choice :  ");
    fflush(stdout);

    publish_choice = getc(stdin);
    if (publish_choice != '\n' && publish_choice != '\r')
        getc(stdin);
    
    /* For header list*/
 
    /* Allocates memory for the header linked list*/
        g_iptkapp_publish_db.header_list.hdr = iptkapp_port_malloc(sizeof(icf_header_st));
        g_iptkapp_publish_db.header_list.hdr->next = ICF_NULL;
        g_iptkapp_publish_db.header_list.count = 1;
        iptkapp_port_strcpy((icf_int8_t *)g_iptkapp_publish_db.header_list.hdr->hdr_name.str,"SIP-IF-MATCH");
        g_iptkapp_publish_db.header_list.hdr->hdr_name.str_len = iptkapp_port_strlen("SIP-IF-MATCH");

        g_iptkapp_publish_db.destination_info.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
        g_iptkapp_publish_db.user_address.addr_type = ICF_ADDRESS_PLAN_SIP_URI;
        iptkapp_port_strcpy((icf_int8_t *)g_iptkapp_publish_db.header_list.hdr->hdr_value.str,"Default");
        g_iptkapp_publish_db.header_list.hdr->hdr_value.str_len = iptkapp_port_strlen("Default");





    switch (publish_choice)
    {
        case '1':
            {
                    IPTKAPP_PRINTF("\n\tPlease Enter line id:");
                    IPTKAPP_SCANF("%d",&g_iptkapp_publish_db.line_id);
                    fflush(stdin);

                    IPTKAPP_PRINTF("\n\tPlease Enter Destination SIP URL \n\t(in sip format, e.g sip:abc@aricent.com) :   ");
                    IPTKAPP_SCANF("%s",g_iptkapp_publish_db.destination_info.addr_val.str);
                    g_iptkapp_publish_db.destination_info.addr_val.str_len = 
                        iptkapp_port_strlen((icf_int8_t *)g_iptkapp_publish_db.destination_info.addr_val.str);
                    fflush(stdin);

                    IPTKAPP_PRINTF("\n\tPlease Enter Self Id URL \n\t(in sip format, e.g sip:abc@aricent.com) :   ");
                    IPTKAPP_SCANF("%s",g_iptkapp_publish_db.user_address.addr_val.str);
                    g_iptkapp_publish_db.user_address.addr_val.str_len = iptkapp_port_strlen
				((icf_int8_t *)g_iptkapp_publish_db.user_address.addr_val.str);
                    fflush(stdin);


                    IPTKAPP_PRINTF("\n\tEnter Duration: ");
                    IPTKAPP_SCANF("%d",&g_iptkapp_publish_db.duration);
                    fflush(stdin);

                    IPTKAPP_PRINTF("\n\tEnter Event Package Name :");
                    IPTKAPP_SCANF("%s",g_iptkapp_publish_db.event_pkg_name.str);
                    g_iptkapp_publish_db.event_pkg_name.str_len = iptkapp_port_strlen(
				(icf_int8_t *)g_iptkapp_publish_db.event_pkg_name.str);
                    fflush(stdin);
                    state=INITIAL;

                break;
            }


        case '2':
            {
                    IPTKAPP_PRINTF("\n\tEnter E-tag value of Event :");
                    IPTKAPP_SCANF("%s",g_iptkapp_publish_db.header_list.hdr->hdr_value.str);
                    g_iptkapp_publish_db.header_list.hdr->hdr_value.str_len = 
			iptkapp_port_strlen((icf_int8_t *)
				g_iptkapp_publish_db.header_list.hdr->hdr_value.str);
                    state=REFRESH;
                break;
            }

        case '3':
            {
                    IPTKAPP_PRINTF("\n\tEnter E-tag value of Event :");
                    IPTKAPP_SCANF("%s",g_iptkapp_publish_db.header_list.hdr->hdr_value.str);
                    g_iptkapp_publish_db.header_list.hdr->hdr_value.str_len =  
			iptkapp_port_strlen((icf_int8_t *)
				g_iptkapp_publish_db.header_list.hdr->hdr_value.str);

                    IPTKAPP_PRINTF("\n\tEnter Event Package Name :");
                    IPTKAPP_SCANF("%s",g_iptkapp_publish_db.event_pkg_name.str);
                    g_iptkapp_publish_db.event_pkg_name.str_len = iptkapp_port_strlen(
				(icf_int8_t *)g_iptkapp_publish_db.event_pkg_name.str);
                    state=MODIFY;
                break;
            }

        case '4':
            {
                    IPTKAPP_PRINTF("\n\tEnter E-tag value of Event :");
                    IPTKAPP_SCANF("%s",g_iptkapp_publish_db.header_list.hdr->hdr_value.str);
                    g_iptkapp_publish_db.header_list.hdr->hdr_value.str_len =  
			iptkapp_port_strlen(
				(icf_int8_t *)g_iptkapp_publish_db.header_list.hdr->hdr_value.str);
                    state=REMOVE;
                break;
            }

        default :
            {
                IPTKAPP_PRINTF("\n\tPlease Enter any valid option");
                iptkapp_publish_display_menu();
                break;
            }

        }  
 return state;
}
