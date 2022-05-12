/***********************************************************************************
 *
 * File name        : iptkapp_log_display.c
 *
 * Purpose          : This file provides the various functions for displaying traces
 *                    on screen and printing them in file
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 12-Dec-2007  Vipul Varshney                       Initial version
 *
 * Copyright (c) 2007 Aricent Inc . All Rights Reserved
 ****************************************************************************************/

#include "iptkapp_core_log.h"
#include "iptkapp_macros.h"
#include "icf_trace_id.h"
#include "iptkapp_cfg_types.h"
#include "iptkapp_feature_flags.h"



static FILE * p_log_file;

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_init
 *
 * DESCRIPTION   : This function initilize the trace settings
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_trace_init(iptkapp_error_t *p_err)
{
	/*For reporting of traces to the application API ICF_SET_TRACE_REQ is 
	 *invokes by application towards iptk.This API specify the trace type,
	 *trace criteria and the trace rule and also apecifying the status as 
	 *enabled
	 */

	if (IPTKAPP_FAILURE == iptkapp_trace_enable_req(p_err))
	{
		IPTKAPP_PRINTF("\n\tIPTKAPP > ICF_SET_TRACE_REQ towards IP Phone Toolkit Failed. \n\t IPTKAPP > TRACE_INIT Failed \n");
	        return IPTKAPP_FAILURE;
	}

    /* Opening the log file */
    if (IPTKAPP_FAILURE == iptkapp_trace_open_file ())
    {
        IPTKAPP_PRINTF("\n\tIPTKAPP > Log file could not be opened \n");
        return IPTKAPP_FAILURE;
    }
	
	return IPTKAPP_SUCCESS;

}




/*******************************************************************************
 *  
 * FUNCTION NAME : iptkapp_trace_enable_req
 *      
 * DESCRIPTION   : The function is to enable traces
 *      
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_trace_enable_req(iptkapp_error_t *p_err)
{

	/*In this function we will fill the initilization data in the payload of 
	 *the ICF_SET_TRACE_REQ API structure.
	 */

	icf_msg_st			*p_msg;
	iptkapp_return_t		ret_val;
	icf_set_trace_req_st		*p_payload;
	icf_uint8_t 				size;

	IPTKAPP_MSG_ALLOC(p_msg, sizeof(icf_set_trace_req_st),ret_val,p_err);

    size = sizeof(icf_set_trace_req_st );

    if(ret_val==IPTKAPP_FAILURE)
    {
        IPTKAPP_PRINTF("IPTKAPP:- MEMORY ALLOC FAIL : ICF_SET_TRACE_REQ");
        return(IPTKAPP_FAILURE);
    }
    p_payload=(icf_set_trace_req_st *)IPTKAPP_MSG_GET_PAYLOAD(p_msg);

    /*Enable the reporting of traces*/
    p_payload->bitmask=0x07;
    p_payload->status=1;

    /*Theses data fields are to filled if specific logs are to be generated.
     *They change the trace type and filtering criteria
     */
	p_payload->trace_type = 0;

    p_payload->trace_type=ICF_TRACE_TYPE_SIP_INTERFACE ;
	p_payload->trace_type|=ICF_TRACE_TYPE_APP_INTERFACE;
	p_payload->trace_type|=ICF_TRACE_TYPE_CALL_CONTROL;

    p_payload->trace_criteria=ICF_TRACE_CRITERIA_LINE_ID;

    p_payload->trace_rule=ICF_TRACE_RULE_OR;


    /*invoking API towards iptk */
#ifdef IPTK_REL_7_0
	/* By Default all the trace type is enabled.
	We have just put this api filling to display
	how to apply filtering logic in trace receival
	by application. Thtas why we are commenting this code*/
    /*ret_val = iptkapp_cfg_fill_hdr_and_send_iptk(p_msg,ICF_SET_TRACE_REQ,ICF_INVALID_CALL_ID,p_err);*/
#endif

    iptkapp_port_free(p_msg);
    if(ret_val==IPTKAPP_FAILURE)
    {
        IPTKAPP_PRINTF("EXIT : TRACE_SET_REQ");
        return(IPTKAPP_FAILURE);
    }
    p_err = p_err;

    return(IPTKAPP_SUCCESS);
}




/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_core_trace_handler
 *
 * DESCRIPTION   : The function extracts payload data from message coming from iptk
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_core_trace_handler(icf_msg_st * p_msg)
{

	/*This function extracts the payload data from the message that
	 *we get from iptk.These are the TRACE data to be printed on the screen
	 */
  
    iptkapp_return_t	ret_val;
    icf_report_trace_ind_st	*p_payload;

    p_payload=(icf_report_trace_ind_st * )IPTKAPP_MSG_GET_PAYLOAD(p_msg);

    ret_val=iptkapp_trace_send_data_to_screen(p_payload);
    if(ret_val==IPTKAPP_FAILURE)
    {
        IPTKAPP_PRINTF("IPTKAPP:- TRACE DISPLAY ERROR: SCREEN_DISPLAY_DATA");
        return(IPTKAPP_FAILURE);
    }

    ret_val=iptkapp_trace_send_data_to_file(p_payload);
    if(ret_val==IPTKAPP_FAILURE)
    {
        IPTKAPP_PRINTF("IPTKAPP:- TRACE DISPLAY ERROR: FILE_DISPLAY_DATA");
        return(IPTKAPP_FAILURE);
    }


    return(IPTKAPP_SUCCESS);
}


/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_send_data_to_screen
 *
 * DESCRIPTION   : The function send the payload data of message received to a 
 *		   log file and also to standard output
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/
iptkapp_return_t iptkapp_trace_send_data_to_screen(icf_report_trace_ind_st * p_payload)
{
	
    icf_generic_string_st *p_msg;

    IPTKAPP_PRINTF("\n\n\nTRACE STRING : %s",p_payload->trace_data.trc_string.str);

    switch(p_payload->trace_id)
    {
        case ICF_TRACE_APP_MESG:
        case ICF_TRACE_MEDIA_MESG:
        case ICF_TRACE_SIP_PDU:
            {
                p_msg = (icf_generic_string_st *)(p_payload->trace_data.p_var_data);
                IPTKAPP_PRINTF("%s\n\n",p_msg->p_buff);
            }
            break;
        default:
            break;
    }  

    return(IPTKAPP_SUCCESS);
}
	

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_open_file
 *
 * DESCRIPTION   : The function opens the log file
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

icf_return_t iptkapp_trace_open_file ()
{

    /*The name of the logfile will comprising of the log generated date and time*/

    struct tm *today;
    time_t time_val;
    icf_int8_t filename[100];
/*ZyXEL porting*/
#if 1 
    icf_int8_t app_name[20]="/var/IPTKAPP";
#else
    icf_int8_t app_name[20]="IPTKAPP";
#endif
    icf_int8_t buffer[10]={0};
    icf_int8_t *mon[12]= {"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};

    icf_uint16_t local_ctr,filename_ctr;
    local_ctr = filename_ctr = 0;

    time_val=time(NULL);
    today=localtime(&time_val);


    /********************** Making File Name***********************/

    while(app_name[filename_ctr]!='\0')
    {
        filename[filename_ctr++]=app_name[local_ctr++];
    }
    filename[filename_ctr++]='_';
    local_ctr=0;
    while(*(mon[today->tm_mon]+local_ctr)!='\0')
    {
        filename[filename_ctr++]=*(mon[today->tm_mon]+local_ctr);
        local_ctr++;
    }
    local_ctr=0;
    sprintf(buffer,"%d",today->tm_mday);
    while(buffer[local_ctr]!='\0')
    {
        filename[filename_ctr++]=buffer[local_ctr++];
    }
    filename[filename_ctr++]='_';
    local_ctr=0;
    sprintf(buffer,"%d",today->tm_year+1900);
    while(buffer[local_ctr]!='\0')
    {
        filename[filename_ctr++]=buffer[local_ctr++];
    }
    filename[filename_ctr++]='_';
    local_ctr=0;
    sprintf(buffer,"%d",today->tm_hour);
    while(buffer[local_ctr]!='\0')
    {
        filename[filename_ctr++]=buffer[local_ctr++];
    }
    filename[filename_ctr++]='_';
    local_ctr=0;
    sprintf(buffer,"%d",today->tm_min);
    while(buffer[local_ctr]!='\0')
    {
        filename[filename_ctr++]=buffer[local_ctr++];
    }
    local_ctr=0;
    filename[filename_ctr++]='_';
    sprintf(buffer,"%d",today->tm_sec);
    while(buffer[local_ctr]!='\0')
    {
        filename[filename_ctr++]=buffer[local_ctr++];
    }
    filename[filename_ctr++]='\0';

    /**********************File name completed*******************/
	
    p_log_file=fopen(filename,"wa");
    if (p_log_file == IPTKAPP_NULL)
        return IPTKAPP_FAILURE;

    return IPTKAPP_SUCCESS;

}

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_close_file
 *
 * DESCRIPTION   : The function closes the file pointer
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

icf_void_t iptkapp_trace_close_file ()
{

    fclose(p_log_file);
}





/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_trace_send_data_to_file
 *
 * DESCRIPTION   : The function send the payload data of message received to a
 *                 log file and also to standard output
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/

iptkapp_return_t iptkapp_trace_send_data_to_file (icf_report_trace_ind_st * p_payload)
{
    icf_generic_string_st *p_msg;
    /*
       for(local_ctr=0;local_ctr<(p_payload->trace_data.number_of_string_values);local_ctr++)
       {
       IPTKAPP_FPRINTF(p_log_file,"\n STRING VALUE %d : %s",local_ctr,p_payload->trace_data.string_val[local_ctr].str);
       }
     */
    IPTKAPP_FPRINTF(p_log_file,"\nTRACE STRING : %s",p_payload->trace_data.trc_string.str);


    switch(p_payload->trace_id)
    {
        case ICF_TRACE_APP_MESG:
        case ICF_TRACE_MEDIA_MESG:
        case ICF_TRACE_SIP_PDU:
            {
                p_msg = (icf_generic_string_st *)(p_payload->trace_data.p_var_data);
                IPTKAPP_FPRINTF(p_log_file,"%s\n\n",p_msg->p_buff);
            }
            break;
        default:
            break;
    }  
    return(IPTKAPP_SUCCESS);
}

	
