/*******************************************************************************
*
* FILE NAME    : icf_adm_main.c
*
* DESCRIPTION  : Contains the definitions of entry point function of ADM.
*
*
* Revision History :
*    DATE            NAME          REFERENCE              REASON
* --------       ------------     ---------             ---------------
* 22-Feb-2005    Mayank Rastogi   Non-Service LLD         Initial
* 01-Mar-2005    Amit Sharma                              Definition of SCM 
*                                                         process mesg removed 
* 08-Feb-2006	 Aman Arora		  ICF rel 2.1.2			fixed for configuring AMR
*													mode in MM at init time.
* 05-Feb-2008    Amit Sikka       SPR 18066               Warning removal
* Copyright 2006,Aricent.
*******************************************************************************/



#include "icf_internal_api_id.h"
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_adm_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_es_prototypes.h"
#ifdef ICF_SERVICE_ENABLE
#include "icf_scm_prototypes.h"
#endif /* #ifdef ICF_SERVICE_ENABLE */


/*******************************************************************************
*
* FUNCTION NAME   : icf_adm_process_mesg
*
* DESCRIPTION     : This is the entry point function of ADM which dispatched the
*                   messages to PA or SCM. All internal messages are dispatched
*                   SCM and all external messages are dispatched to PA.
*
* RETURNS         : (ICF_SUCCESS/ICF_FAILURE)
*
*******************************************************************************/

icf_return_t   icf_adm_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
)
{


    icf_return_t				ret_val = ICF_SUCCESS;
    icf_msg_st				*p_msg_data = ICF_NULL;
	icf_app_id_t				app_id ;
    icf_dbm_data_st			*p_dbm_data = ICF_NULL;
	icf_void_st				*p_routing_info = ICF_NULL;
    icf_app_conf_st			*p_app_conf = ICF_NULL;
	icf_config_data_st		*p_config_data = ICF_NULL;
    icf_glb_pdb_st                      *p_glb_pdb = ICF_NULL;
#ifdef ICF_TRACE_ENABLE
    icf_uint8_t                          p_transp_addr_str[100] = "";
    icf_global_port_info_st             *p_glb_port_data = 
		          (icf_global_port_info_st *)p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->p_glb_port_info;	
#endif
	ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb, ICF_INT_MODULE_ADM)
	ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb) /*function entry trace */

	/* check if the api_id is in the range of PA API Ids the send message to PA */
#ifdef ICF_SERVICE_ENABLE
    if (ICF_EXTERNAL_API_END >= p_internal_msg->msg_hdr.msg_id)
    {
#endif /* #ifdef ICF_SERVICE_ENABLE */
        /* send message to PA using external interface */
		ret_val = icf_dbm_get_module_glb_data(     
				p_internal_msg->msg_hdr.p_glb_pdb,
				(icf_int_module_id_t const)ICF_INT_MODULE_DBM,
				(icf_void_t *)&p_dbm_data,
				&(p_internal_msg->msg_hdr.ecode));

		
		if(ret_val == ICF_SUCCESS)

		{			
		
			p_msg_data = (icf_msg_st *)p_internal_msg->p_msg_data;
			ICF_ASSERT(p_internal_msg->msg_hdr.p_glb_pdb,(ICF_NULL != p_msg_data))
			
			app_id = p_msg_data->hdr.app_id;

			/*CFM for entity which sends the activation/deactivation request
			for the network*/
			if(ICF_GENERIC_APP_ID == app_id)
			{
                p_msg_data->hdr.destination_id = ICF_GENERIC_APP_MODULE_ID;

				ret_val = icf_dbm_get_module_glb_data(     
					p_internal_msg->msg_hdr.p_glb_pdb,
					(icf_int_module_id_t const)ICF_INT_MODULE_CFG,
					(icf_void_t *)&p_config_data,
					&(p_internal_msg->msg_hdr.ecode));

				if(ICF_NULL != p_config_data->p_generic_routing_info)
				    p_routing_info = p_config_data->p_generic_routing_info;
				else
				{
					ret_val = ICF_FAILURE;
				}
                
			}
			else/*any other CFM or indication for application*/
			{

				if(ICF_SUCCESS == 
                    icf_cfg_fetch_app_conf(p_internal_msg->msg_hdr.p_glb_pdb,app_id, 
					&p_app_conf,
					&(p_internal_msg->msg_hdr.ecode)))
                {
                    if((ICF_NULL != p_app_conf) &&
                       (ICF_TRUE == p_app_conf->is_valid))
                    {
                        p_routing_info = &(p_app_conf->routing_info);
			 p_msg_data->hdr.destination_id = p_app_conf->app_source_id;
                    }
                }
				
			}
			if (ICF_NULL == p_routing_info)
            {
                /* No Routing information Retrived*/
				/*if its for codec configure response with APP ID 65534 then its not an
				 * alarm
				 */
				if ((ICF_DUMMY_APP_ID == app_id) &&
					(ICF_CFG_CODEC_INFO_RESP == p_msg_data->hdr.api_id))
				{
					ICF_PRINT(((icf_uint8_t *) "[ADM]: Codec Configure response \
						received for AMR codec configure\n"));
				}
				else
				{
					/* Raise Alarm */
					ICF_ALARM(((icf_uint8_t *) " Not able to fetch Routing info For APP_ID %d",app_id));
				}
                /* Ret Val made to be suceess */
                ret_val = ICF_SUCCESS;

            }
            else
			{
                p_msg_data->hdr.source_id = ICF_MODULE_ICF;
                p_msg_data->hdr.version = ICF_VERSION_1_0;
                
                p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

                if(ICF_FALSE == p_glb_pdb->p_glb_data->recvd_restart_reinit_event)
                {
#ifdef ICF_TRACE_ENABLE
                    icf_msg_st *p_msg = 
                         (icf_msg_st *)p_internal_msg->p_msg_data;
                    icf_cmn_trace_api_from_api_id(
                             p_internal_msg->msg_hdr.p_glb_pdb,
                             p_msg,
                             &(p_internal_msg->msg_hdr.ecode));
#endif

                    ret_val = icf_send_to_app(p_internal_msg->msg_hdr.p_glb_pdb,
                            (icf_msg_st *)p_internal_msg->p_msg_data,
                            p_routing_info,
                            &(p_internal_msg->msg_hdr.ecode));
                   if( ICF_FAILURE == ret_val)
		    	    {
#ifdef ICF_TRACE_ENABLE
					  p_glb_port_data->icf_trace_ipc_info.local_port = 
									p_config_data->self_ip_address.port_num;
				 	  if( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME
                  			     == p_config_data->self_ip_address.addr.addr_type)
            		  {
                			icf_port_strcpy(p_glb_port_data->icf_trace_ipc_info.local_ip,
                                      p_config_data->self_ip_address.addr.addr.domain.str);
            		  }
            		  else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR
                        		 == p_config_data->self_ip_address.addr.addr_type)
            		  {
                                  icf_cmn_convert_transport_addr_to_ip_string(
                                      &(p_config_data->self_ip_address.addr),
                                      p_transp_addr_str);
                		  icf_port_strcpy(p_glb_port_data->icf_trace_ipc_info.local_ip,
                          	      p_transp_addr_str);
            		  }
            		  else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR
                     		        == p_config_data->self_ip_address.addr.addr_type)
            		  {
                				icf_port_strcpy(p_glb_port_data->icf_trace_ipc_info.local_ip,
                                		p_config_data->self_ip_address.addr.addr.ipv6_addr.str);
            		  }
	
                       ICF_CMN_APP_TRACE(p_glb_pdb, ICF_TRACE_UDP_SEND_ERROR,p_msg)
#endif
		            }
                }
                else
                {
                    ret_val = ICF_SUCCESS;
                }
			}
            
		}

#ifdef ICF_SERVICE_ENABLE
    }
    else
    {
        /* send message to SCM using internal interface */
        ret_val = icf_scm_process_mesg(p_internal_msg);
    }
#endif /* #ifdef ICF_SERVICE_ENABLE */

    ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb) /* function exit trace */
    ICF_RESET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
}
