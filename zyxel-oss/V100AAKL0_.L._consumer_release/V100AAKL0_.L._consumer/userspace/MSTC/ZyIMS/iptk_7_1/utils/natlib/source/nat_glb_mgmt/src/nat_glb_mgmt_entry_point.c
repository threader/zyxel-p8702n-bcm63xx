/******************************************************************************

 * 

 * FILE NAME:   nat_glb_mgmt_entry_point.c

 * 

 * DESCRIPTION: This file contains the entry point function and the utitlity 

 *              function called by the entry point function to call the API 

 *              handlers.

 *              

 * REVISION HISTORY:    

 *

 *   Date       Name                 Reference			Comments

 * --------    ------------       ------------------  -------------

 * 22-June-2006	Abhishek Chhibber       LLD				Initial Version

 * 10-July-2006	Anuradha Gupta		    LLD				Coding

 * 16-Nov-2006	Anuradha Gupta			LLD				Coding addition

 *

 * Copyright 2006, Aricent 

 ******************************************************************************/     



#include "nat_common_types.h"

#include "nat_glb_mgmt_prototypes.h"

#include "nat_common_internal_api_id.h"

#include "nat_port_pltfm_prototypes.h"

#include "nat_api.h"

#include "stun_prototypes.h"

#include "nat_error_id.h"

#include "nat_common_prototypes.h"

#include "nat_port_pltfm_prototypes.h"

#include "stun_discovery_prototypes.h"

#ifdef NAT_PORT_VXWORKS
#include "icf_port_prototypes.h"
#endif


nat_glb_pdb_st      *p_nat_global_pdb=NAT_NULL;



/*******************************************************************************

 *

 * FUNCTION:        nat_print_msg_hdr

 *

 * DESCRIPTION:     Prints the message Header

 *                  

 * RETURNS:         nothing.                 

 *

 ******************************************************************************/

nat_void_t nat_print_msg_hdr(

        IN	 nat_msg_st        *p_msg)

{

	nat_uint8_t app_context_id[32];

	NAT_PRINT(("\n**************************************************\n"));

	NAT_PRINT(("\n*******        EXTERNAL MESSAGE          *********\n"));

	nat_print_external_msg_id(p_msg->msg_hdr.api_id);

	nat_port_sprintf((nat_int8_t *)app_context_id,"%d",p_msg->msg_hdr.app_context_id);

	NAT_PRINT(("APP CONTEXT ID : %s\n",app_context_id));

	NAT_PRINT(("SOURCE ID : %d   DESTINATION ID : %d \n",

		p_msg->msg_hdr.source_id,p_msg->msg_hdr.destination_id));

	NAT_PRINT(("API LENGTH : %d\n",p_msg->msg_hdr.api_length));

	NAT_PRINT(("\n**************************************************\n"));

    return;

}



/*******************************************************************************

 *

 * FUNCTION:        nat_process_mesg

 *

 * DESCRIPTION:     This is the entry point function of NAT Traversal Client that is 

 * 		    invoked by application to send a external message to the library

 *		    The function checks the API Id in the message 

 *                  received and invokes appropriate handler

 *                  

 * RETURNS:         NAT_RETURN_SUCCESS in acse of success and NAT_RETURN_FAILURE 

 *                  in case of failure.                 

 *

 ******************************************************************************/

nat_return_t nat_process_mesg(

        INOUT	 nat_msg_st        *p_msg)

{

    nat_return_t        ret_val=NAT_RETURN_SUCCESS;

    nat_error_t 	ecode;

    nat_msg_list_st     msg_lst;



    if(NAT_NULL == p_nat_global_pdb->p_recv_msg )

    {

    	p_nat_global_pdb->p_recv_msg = &msg_lst;

    	p_nat_global_pdb->p_recv_msg->p_msg_next = NAT_NULL;

    }



	p_nat_global_pdb->p_recv_msg->p_msg = p_msg;

	

    	nat_print_msg_hdr(p_msg);

	

	switch(p_msg->msg_hdr.api_id)

	{

		

	case NAT_STUN_INIT_REQ :

		

		if(NAT_RETURN_SUCCESS != nat_stun_init_req(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;

		}

		break;

		

	case NAT_STUN_EXEC_PROC_REQ :

		

        if(NAT_RETURN_SUCCESS != nat_stun_exec_proc_req(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;

		}

		break;

		

	case NAT_GET_PUBLIC_IP_PORT_REQ  :

		

        if(NAT_RETURN_SUCCESS != nat_get_public_ip_port_req(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;                

		}

		break;

		

	case NAT_REL_PUBLIC_IP_PORT_REQ:

		

		if(NAT_RETURN_SUCCESS != nat_rel_public_ip_port_req(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;                

		}

		break;

		

	case NAT_RECV_UDP_PKT_IND :

		

		if(NAT_RETURN_SUCCESS != nat_process_udp_pkt_ind(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;

		}

		break;

		

	case NAT_RECV_TCP_SEC_PKT_IND :

		

		if(NAT_RETURN_SUCCESS != nat_process_tcp_sec_pkt_ind(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;

		}

		break;

		

	case NAT_START_TIMER_RESP :

		

		if(NAT_RETURN_SUCCESS != nat_process_start_timer_resp(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;                   

		}

		break;

		

	case NAT_TIMER_EXP_IND :

		

		if(NAT_RETURN_SUCCESS != nat_process_timer_exp_ind(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;                        

		}

		break;



	case NAT_DEINIT_REQ :

		if(NAT_RETURN_SUCCESS != nat_deinit_req(p_nat_global_pdb))

		{

			ret_val = NAT_RETURN_FAILURE;                        

		}

		else

		{

			nat_deinit();

		}

		break;

		

	default :

		ret_val = NAT_RETURN_FAILURE;

		break;

	}

	

    nat_port_static_memfree(p_msg,&ecode);	

    return ret_val;

	

}/* End function */





/*******************************************************************************

 *

 * FUNCTION:        nat_process_internal_mesg

 *

 * DESCRIPTION:     This is the function of NAT Traversal Client that is 

 * 		            invoked by internal modules to send a external message to 

 *                  the application.

 *		            The function checks the internal API Id in the message 

 *                  received and creates appropriate external message and sends it.

 *                  

 * RETURNS:         NAT_RETURN_SUCCESS in acse of success and NAT_RETURN_FAILURE 

 *                  in case of failure.                 

 *

 ******************************************************************************/

nat_return_t nat_process_internal_mesg(

        INOUT	 nat_internal_msg_st        *p_msg)

{

    nat_return_t                         ret_val=NAT_RETURN_SUCCESS;

    



	switch(p_msg->msg_hdr.msg_id)

	{



	case NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ:

         	if(NAT_RETURN_SUCCESS != nat_stun_process_mesg(p_msg))

        	{

                	ret_val = NAT_RETURN_FAILURE;

               	}

               	break;

         

    case NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP:



		if(NAT_RETURN_SUCCESS != nat_send_get_public_ip_port_resp(p_msg))

        	{

                	ret_val = NAT_RETURN_FAILURE;

        	}

		break;



	case NAT_INT_STUN_SHSEC_RESP:



		if(NAT_RETURN_SUCCESS != nat_send_shsec_resp(p_msg))

       		{

                	ret_val = NAT_RETURN_FAILURE;

        	}

		break;





	case NAT_INT_STUN_DISCOVERY_TYPE_RESP:



		if(NAT_RETURN_SUCCESS != nat_send_discovery_type_resp(p_msg))

        	{

                	ret_val = NAT_RETURN_FAILURE;

        	}

		break;



	case NAT_INT_STUN_LIFETIME_DISC_RESP:



		if(NAT_RETURN_SUCCESS != nat_send_lifetime_disc_resp(p_msg))

        	{

                	ret_val = NAT_RETURN_FAILURE;

        	}

		break;

		

    case NAT_INT_TIMER_EXPIRY_IND:

        if(NAT_RETURN_SUCCESS != nat_send_timer_expiry_ind(p_msg))

        	{

                	ret_val = NAT_RETURN_FAILURE;

        	}

		break;



	case NAT_INT_ERROR_IND:

		if(NAT_RETURN_SUCCESS != nat_send_error_ind(p_msg))

		{

                	ret_val = NAT_RETURN_FAILURE;

        	}

		break;



	default :

		ret_val = NAT_RETURN_FAILURE;

		break;

	}



    return ret_val;



}/* End function */





/*******************************************************************************

 *

 * FUNCTION:        nat_init

 *

 * DESCRIPTION:     This is the initialiation function of NAT Client Library

 *                  

 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 

 *                  in case of failure.                 

 *

 ******************************************************************************/

nat_return_t nat_init(nat_void_t **p_p_nat_glb_data)

{

    nat_return_t            ret_val = NAT_RETURN_SUCCESS;

    nat_error_t 		ecode = NAT_ERR_UNDEFINED;

    

    /* Allocate memory to table */

    p_nat_global_pdb = nat_port_static_memget(nat_port_sizeof(nat_glb_pdb_st),&ecode);

   

    if(NAT_NULL == p_nat_global_pdb)

    {

	NAT_PRINT(("NAT_INIT : memory alloc failure"));

	return NAT_RETURN_FAILURE;

    }

    

    nat_port_memset(p_nat_global_pdb,0,nat_port_sizeof(nat_glb_pdb_st));

 

    /* Allocate memory to table */

    p_nat_global_pdb->p_glb_data = 

        nat_port_static_memget(nat_port_sizeof(nat_glb_data_st),&ecode);

    

    if(NAT_NULL == p_nat_global_pdb->p_glb_data)

    {

	NAT_PRINT(("NAT_INIT : memory alloc failure"));

	return NAT_RETURN_FAILURE;

    }



    nat_port_memset(p_nat_global_pdb->p_glb_data,0,nat_port_sizeof(nat_glb_data_st));

    

    *p_p_nat_glb_data = p_nat_global_pdb;

    

    if(NAT_RETURN_FAILURE == nat_update_conf_info())

    {

	return NAT_RETURN_FAILURE;

	

     }

       

    return ret_val;    

    

}





/*******************************************************************************

 *

 * FUNCTION:        nat_deinit

 *

 * DESCRIPTION:     This is the deinitialiation function of NAT Client Library

 *                  

 * RETURNS:         nothing                

 *

 ******************************************************************************/

nat_void_t nat_deinit()

{

    nat_error_t 	ecode = NAT_ERR_UNDEFINED;

	nat_internal_msg_st     internal_msg;



	if(NAT_NULL != p_nat_global_pdb->p_glb_data->p_stun_data)

	{

		internal_msg.msg_hdr.p_glb_pdb = p_nat_global_pdb;

        	internal_msg.msg_hdr.msg_id = NAT_INT_STUN_DEINIT_REQ;

		internal_msg.p_msg_data = NAT_NULL;	

		nat_stun_process_mesg(&internal_msg);		

		p_nat_global_pdb->p_glb_data->p_stun_data = NAT_NULL;

	}	



	if(NAT_NULL != p_nat_global_pdb->p_glb_data->p_stun_discovery_data)

	{

                internal_msg.msg_hdr.p_glb_pdb = p_nat_global_pdb;

                internal_msg.msg_hdr.msg_id = NAT_INT_STUN_DISCOVERY_DEINIT_REQ;

                internal_msg.p_msg_data = NAT_NULL;            

		nat_stun_discovery_process_mesg(&internal_msg);

		p_nat_global_pdb->p_glb_data->p_stun_discovery_data = NAT_NULL;

        }



	if(NAT_NULL != p_nat_global_pdb->p_glb_data->p_stun_context_map)

	{

		nat_port_static_memfree(p_nat_global_pdb->p_glb_data->p_stun_context_map,&ecode);

		p_nat_global_pdb->p_glb_data->p_stun_context_map = NAT_NULL;

	}



	

#if 0

	nat_port_static_memfree(p_nat_global_pdb->p_glb_data->p_stun_lifetime_data,&ecode);

	nat_port_static_memfree(p_nat_global_pdb->p_glb_data->p_stun_shsec_data,&ecode);

#endif



	if(NAT_NULL != p_nat_global_pdb->p_glb_data->p_glb_port_info)

	{

		p_nat_global_pdb->es_nat_rel_port_data();

	}



    return;    

    

}





/****************************************************************************

** Function: 	nat_read_conf_file

**

** DESCRIPTION: This function reads the configuration file and stores

**              in a structure p_file.

**              Comments in the configuration files are not stored.

** 

***************************************************************************/

nat_return_t nat_read_conf_file(nat_void_t* file_ptr, nat_conf_file_st* p_file)

{

    nat_port_file_t *fp = file_ptr;

    p_file->count = 0;



    /* 

    ** reading conf file line by line up to end of file 

    */

  

    for (   p_file->count = 0 ; 

            p_file->count < NAT_CONF_MAX_LINES; 

            p_file->count++)

    {

        if (

              nat_port_fgets((nat_int8_t*)p_file->line[p_file->count].data,

                NAT_CONF_MAX_LINE_SIZE, fp) == NAT_NULL )

        {

            if ( feof(fp))

            {

               return NAT_RETURN_SUCCESS;

            }     

        }



        /* 

        ** string start and termination 

        */



        nat_str_trim((nat_uint8_t*)p_file->line[p_file->count].data);



        /* 

        ** Check whether Line starts from # or \n or NULL 

        */



        if (p_file->line[p_file->count].data[0] == NAT_CONF_COMMENT_CHAR || 

            p_file->line[p_file->count].data[0] == '\n' || 

	    p_file->line[p_file->count].data[0] == '\r' || 

            p_file->line[p_file->count].data[0] == 0)

        {

            p_file->count--; 

            /* if line starts #,\n or NULL , decrement the count  */

        }

        else

        {

            /* do nothing */

        }

    }



    return NAT_RETURN_SUCCESS; 

}



/****************************************************************************

** Function: 	nat_init_config_data

**

** DESCRIPTION: This function initializes the config data.

** 

***************************************************************************/

nat_void_t nat_init_config_data(stun_config_data_st **p_p_config_data)

{

	stun_config_data_st *p_config_data = *p_p_config_data;



	nat_port_memset(p_config_data,0,nat_port_sizeof(stun_config_data_st));



	p_config_data->lifetime_disc_enabled = NAT_FALSE;

	p_config_data->shsec_enabled = NAT_FALSE;

	p_config_data->stun_server_addr.port = NAT_DEFAULT_STUN_SERVER_PORT;

	nat_port_strcpy(p_config_data->self_ip.str,(nat_uint8_t *)"127.0.0.1");



	p_config_data->password[0] = '\0';

	p_config_data->username[0] = '\0';



	return;



}



/****************************************************************************

** Function: 	nat_validate_config_data

**

** DESCRIPTION: This function validates the config data.

** 

***************************************************************************/

nat_return_t nat_validate_config_data(stun_config_data_st **p_p_config_data)

{

	nat_return_t ret_val = NAT_RETURN_SUCCESS;



	stun_config_data_st *p_config_data = *p_p_config_data;



	if((NAT_NULL == p_config_data->password[0]) && 

		(NAT_NULL == p_config_data->username[0]))

	{

		NAT_PRINT(("\n Message integrity not required\n"));



		/* put off message integrity*/

		p_nat_global_pdb->p_glb_data->shared_secret_proc_status = 

			NAT_STUN_SHARED_SECRET_NOT_REQD;



		p_nat_global_pdb->p_glb_data->message_integrity = NAT_FALSE;

	}

	else if((NAT_NULL != p_config_data->password[0]) && 

		(NAT_NULL != p_config_data->username[0]))

	{

		NAT_PRINT(("\n Message integrity required\n"));



		/* put on message integrity*/

		p_nat_global_pdb->p_glb_data->shared_secret_proc_status = 

			NAT_STUN_SHARED_SECRET_COMPLETE;



		p_nat_global_pdb->p_glb_data->message_integrity = NAT_TRUE;

	}

	else if((NAT_NULL == p_config_data->password[0]) && 

		(NAT_NULL != p_config_data->username[0]))

	{

		NAT_PRINT(("\nusername present , password missing\n"));

		ret_val = NAT_RETURN_FAILURE;

	}

	else if((NAT_NULL != p_config_data->password[0]) && 

		(NAT_NULL == p_config_data->username[0]))

	{

		NAT_PRINT(("\npassword present , username missing\n"));

		ret_val = NAT_RETURN_FAILURE;

	}

	return ret_val;
}


/****************************************************************************

** Function: 	nat_update_conf_info

**

** DESCRIPTION: This function updates the configuration info.

** 

***************************************************************************/

nat_return_t nat_update_conf_info()

{

	nat_return_t            ret_val = NAT_RETURN_SUCCESS;

	

#ifdef ICF_VERIZON



	nat_uint8_t						 name[NAT_CONF_MAX_LINE_SIZE] ; 



	HKEY key;



	unsigned char				filename_unicode[256],filepath[256];



	unsigned long				len=sizeof(filename_unicode);



#else



#ifdef ICF_PORT_WINCE

	nat_uint8_t				filename[NAT_CONF_MAX_LINE_SIZE] = "\\icf-6\\cfg\\nat.cfg";



#else



    char						 filename[NAT_CONF_MAX_LINE_SIZE] = "nat.cfg";

#endif



#endif









	nat_port_file_t*        fp = NAT_NULL;

	nat_conf_file_st        conf_file;

	nat_int8_t 	        temp_str[NAT_CONF_MAX_LINE_SIZE] = "\0",

                                 val_str[NAT_CONF_MAX_LINE_SIZE] = "\0";

    nat_uint32_t		value =  0, count = 0;

    nat_error_t 		ecode;

    stun_config_data_st     *p_config_data = NAT_NULL;



 	

#ifdef ICF_VERIZON



	if( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, L"SOFTWARE\\FSS_Microphone",0,KEY_READ, &key ))



	{



		return FALSE;



	}







	RegQueryValueEx( key, L"Path", NULL, NULL, filename_unicode, &len );







	len = 0;



	len = wcslen(filename_unicode);



	count = wcstombs((char*)filepath,(WCHAR*)filename_unicode,len + 1);



	if(count != len)



	{



		return NAT_RETURN_FAILURE;



	}



	strcpy(filename,filepath);



	strcat(filename,"/cfg/nat.cfg");



#endif

    

    conf_file.line = nat_port_static_memget(nat_port_sizeof(nat_conf_line_st)*NAT_CONF_MAX_LINES,&ecode);

       if(NAT_NULL == conf_file.line)

	{

		NAT_PRINT(("NAT CFG : memory alloc failure"));

		return NAT_RETURN_FAILURE;

	}



	/* Allocate memory to table */

	p_nat_global_pdb->p_glb_data->p_stun_config_data = 

		nat_port_static_memget(nat_port_sizeof(stun_config_data_st),&ecode);

	if(NAT_NULL == p_nat_global_pdb->p_glb_data->p_stun_config_data)

	{

		nat_port_static_memfree(conf_file.line,&ecode);

		NAT_PRINT(("NAT CFG : memory alloc failure"));

		return NAT_RETURN_FAILURE;

	}



	

	p_config_data = p_nat_global_pdb->p_glb_data->p_stun_config_data;



	nat_init_config_data(&p_config_data);

	
#ifdef NAT_PORT_VXWORKS	
	if ((fp = icf_port_fopen (filename, "r")) == NAT_NULL)
#else
	if ((fp = (nat_port_file_t *)nat_port_fopen (filename, "r")) == NAT_NULL)
#endif
    {

	NAT_PRINT(("\n NAT config file missing"));

	nat_port_static_memfree(conf_file.line,&ecode);		

	    return NAT_RETURN_FAILURE;

    }

    else

    {

        ret_val = nat_read_conf_file(fp, &conf_file);

        nat_port_fclose(fp);

    }

    	

    for (count=0; count < conf_file.count; count++)

	{

	value = 0;

	

        if (nat_port_strmatch( (nat_uint8_t *)"NAT_SELFIP", conf_file.line[count].data ) == 0)

		{

			/* NAT_SELFIP parameter */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %s\n", 

				temp_str, p_config_data->self_ip.str);

			continue;

		}

        else if (nat_port_strmatch((nat_uint8_t *)"NAT_APP_PORT", conf_file.line[count].data) == 0)

		{

			/* NAT_APP_PORT */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			if(value)

				p_config_data->app_recv_port = (nat_uint16_t)value;

			continue;

		}

        else if (nat_port_strmatch( (nat_uint8_t *)"NAT_APP_IP", conf_file.line[count].data ) == 0)

		{

			/* NAT_SELFIP parameter */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %s\n", 

				temp_str, p_config_data->app_ip.str);

			continue;

		}

        else if (nat_port_strmatch((nat_uint8_t *)"NAT_APP_SEND_PORT", conf_file.line[count].data) == 0)

		{

			/* NAT_APP_PORT */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			if(value)

				p_config_data->app_send_port = (nat_uint16_t)value;

			continue;

		}

		else if (nat_port_strmatch( (nat_uint8_t *)"STUN_SERVER_ADDRESS", conf_file.line[count].data ) == 0)

		{

			

			/* STUN_SERVER_ADDRESS parameter */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %s\n", 

				temp_str, val_str);

			

			nat_cmn_convert_ip_string_to_ip4_addr((nat_uint8_t *)val_str,

				&p_config_data->stun_server_addr.ipv4_addr);



			continue;

		}

        else if (nat_port_strmatch( (nat_uint8_t *)"STUN_SERVER_PORT", conf_file.line[count].data ) == 0)

		{

			/* STUN_SERVER_PORT */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			if(value)

				p_config_data->stun_server_addr.port = (nat_uint16_t)value;

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"SHSEC_ENABLED", conf_file.line[count].data) == 0)

		{

			/* SHSEC_ENABLED */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			if (1 == value)

			{

				p_config_data->shsec_enabled = NAT_TRUE;

			}

			else

			{

				p_config_data->shsec_enabled = NAT_FALSE;

			}

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"BINDING_LIFETIME_DISCOVERY_ENABLED", conf_file.line[count].data) == 0)

		{

			/* BINDING_LIFETIME_DISCOVERY_ENABLED */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			if (1 == value)

			{

				p_config_data->lifetime_disc_enabled = NAT_TRUE;

			}

			else

			{

				p_config_data->lifetime_disc_enabled = NAT_FALSE;

			}

			continue;

		}

		else if (nat_port_strmatch( (nat_uint8_t *)"USERNAME", conf_file.line[count].data ) == 0)

		{

			/* USERNAME parameter */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %s\n", 

				temp_str, p_config_data->username);

			continue;

		}

		else if (nat_port_strmatch( (nat_uint8_t *)"PASSWORD", conf_file.line[count].data ) == 0)

		{

			/* PASSWORD parameter */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %s\n", 

				temp_str, p_config_data->password);

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"LIFETIME_DISCOVERY_TIMER", conf_file.line[count].data) == 0)

		{

			/* LIFETIME_DISCOVERY_TIMER */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			p_config_data->lifetime_disc_timer = (nat_uint16_t)value;

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"LIFETIME_DISCOVERY_DELTA_TIMER", conf_file.line[count].data) == 0)

		{

			/* LIFETIME_DISCOVERY_DELTA_TIMER */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			p_config_data->lifetime_delta_timer = (nat_uint16_t)value;

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"SHSEC_REFRESH_TIMER", conf_file.line[count].data) == 0)

		{

			/* SHSEC_REFRESH_TIMER */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			p_config_data->shsec_refresh_timer = (nat_uint16_t)value;

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"BINDING_REFRESH_TIMER", conf_file.line[count].data) == 0)

		{

			/* BINDING_REFRESH_TIMER */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			p_config_data->binding_refresh_timer = (nat_uint16_t)value;

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"TYPE_DISCOVERY_TIMER", conf_file.line[count].data) == 0)

		{

			/* TYPE_DISCOVERY_TIMER */

			nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

			p_config_data->type_disc_timer = (nat_uint16_t)value;

			continue;

		}

		else if (nat_port_strmatch((nat_uint8_t *)"TIMER_SCALE_VALUE", conf_file.line[count].data) == 0)

        {

            /* SCALE_TIMER_VALUE */

            nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

            p_config_data->scale_timer_value = value;

            continue;

        }

		else if (nat_port_strmatch((nat_uint8_t *)"TYPE_DISCOVERY_PORT", conf_file.line[count].data) == 0)

        {

            /* TYPE_DISCOVERY_PORT */

            nat_port_sscanf((nat_int8_t *)conf_file.line[count].data, "%s %d\n", temp_str, &value);

            p_config_data->type_disc_port = (nat_uint16_t )value;

            continue;

        }



	}



	nat_port_static_memfree(conf_file.line,&ecode);		

	ret_val = nat_validate_config_data(&p_config_data);



	return ret_val; 

}





/****************************************************************************



** Function: nat_str_trim 

**

** DESCRIPTION: This function removes extra TAB/spaces from start and 

**              end of the string. Replaces all tabs by space in the string.

** 

** 

**

***************************************************************************/

nat_void_t nat_str_trim(INOUT nat_uint8_t * str)

{

    nat_int16_t starts_from, ends_at, count=0;

    ends_at = (nat_int16_t)nat_port_strlen(str);

    /* 

    ** removing all tab bar from the line 

    */

    for (count = 0; str[count] !=  (nat_uint8_t)'\n' && count < ends_at; count++)

    {

        if (str[count] == (nat_uint8_t)'\t')

            str[count]=(nat_uint8_t)' ';

    }



   

    count = 0;

    while(str[count]==' ') count++;

    starts_from = count;



    while(str[ends_at]==(nat_uint8_t)'\n' || str[ends_at]==0) 

    {

        ends_at--;

        if ( ends_at < 0 )

        {

            break;

        }

    }



    str[ends_at+1] = NAT_NULL;

    nat_port_strcpy(str, str+starts_from);

}



/****************************************************************************

** Function: 	nat_print_external_msg_id

**

** DESCRIPTION: This function prints the external message ID.

** 

***************************************************************************/

nat_void_t nat_print_external_msg_id(nat_uint8_t api_id)

{

	switch(api_id)

	{

		

	case NAT_STUN_INIT_REQ : 

		NAT_PRINT(("\nAPI ID : NAT_STUN_INIT_REQ \n"));

		break;

		

	case NAT_STUN_INIT_RESP : 

		NAT_PRINT(("\nAPI ID : NAT_STUN_INIT_RESP \n"));

		break;



	case NAT_STUN_EXEC_PROC_REQ :

		NAT_PRINT(("\n API ID : NAT_STUN_EXEC_PROC_REQ \n"));

		break;

		

	case NAT_GET_PUBLIC_IP_PORT_REQ  :

		NAT_PRINT(("\n API ID : NAT_GET_PUBLIC_IP_PORT_REQ \n"));

		break;

		

	case NAT_REL_PUBLIC_IP_PORT_REQ:

		NAT_PRINT(("\n API ID : NAT_REL_PUBLIC_IP_PORT_REQ \n"));

		break;

		

	case NAT_RECV_UDP_PKT_IND :

		NAT_PRINT(("\n API ID : NAT_RECV_UDP_PKT_IND \n"));

		break;

		

	case NAT_RECV_TCP_SEC_PKT_IND :

		NAT_PRINT(("\n API ID : NAT_RECV_TCP_SEC_PKT_IND \n"));

		break;

		

	case NAT_START_TIMER_RESP :

		NAT_PRINT(("\n API ID : NAT_START_TIMER_RESP \n"));

		break;

		

	case NAT_TIMER_EXP_IND :

		NAT_PRINT(("\n API ID : NAT_TIMER_EXP_IND \n"));

		break;

	

	case NAT_GET_PUBLIC_IP_PORT_RESP:	

		NAT_PRINT(("\n API ID : NAT_GET_PUBLIC_IP_PORT_RESP \n"));

                break;



	case NAT_STUN_EXEC_PROC_RESP:

		NAT_PRINT(("\n API ID : NAT_STUN_EXEC_PROC_RESP \n"));

                break;



	case NAT_STUN_PROC_STATUS_IND:

		NAT_PRINT(("\n API ID : NAT_STUN_PROC_STATUS_IND \n"));

                break;



	default :

		break;

	}

	

	

    return;

	

}



