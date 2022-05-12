/******************************************************************************
 ** FUNCTION:
 **                     This file contains the api handler functions of SSA
 **
 *****************************************************************************
 **
 ** FILENAME:           icf_ssa_apihandlers.c
 **
 ** DESCRIPTION:        This file contains api handler functions of the SSA
 **
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 29-JAN-2005     Umang Singh                           INITIAL CREATION
 ** 29-JAN-2005     Aashish Suchdev           
 ** 
 ** 18-May-2005     Umang Singh         SPR Fix 7555    Setting of Contact 
 **                                                     header done on receving                                                      INVITE    
 ** 25-May-2005     Umang Singh         SPR Fix 7403   
 ** 31-May-2005     Umang Singh         SPR Fix 7618   
 ** 01-Jun-2005     Umang Singh         SPR Fix 7636   
 ** 11-Jun-2005     Jalaj Negi          IMS CLIENT      Code change for IMS
 **                                                     CLIENT
 ** 14-Jun-2005     Umang Singh         SPR 7663        Privacy header handling
 ** 22-JUN-2005     Umang Singh         SPR 7665             
 ** 07-JUL-2005     Aashish Suchdev                     TCP checkin 
 ** 13-AUG-2005     Ram Narayan         None            Added code to incorporate
 **                                                     header/tag/body & stream 
 **                                                     list & some new headers.
 ** 23-AUG-2005		Jyoti Narula		ICF LLD			To init new members of SSA ctxt
 ** 14-SEP-2005     Jalaj Negi          ICF             Fixed Memory leaks
 **                                                     for SIC functionality
 **	15-Dec-2005		Aman Arora			ICF 4.0 CDD		Added code for IPsec
 **	22-Feb-2005		Amit Sharma                         Added code for auth support
 ** 01-Mar-2006     Amit Sharma                         ICF merging with ICF2.0
 ** 30-Mar-2006     Anuradha Gupta			ICF 5.0         Call Modify Feature changes
 **
 ** 03-Apr-2006     Deepti Goyal         ICF 5.0         Update for message body 
 **                                                     header support.
 **
 ** 08-Jun-2006		Anuradha Gupta		ICF 5.0			Call Modify Header related fix
 ** 30-Jun-2006     Umang Singh        Rel 5.2             TLS support
 ** 29-Aug-2006		Deepti Goyal	   Rel 6.0			SPR 12625: T38 handling
 **                                                     in Media Change request.	
 ** 06-Sep-2006     Umang Singh        SPR 12769
 ** 06-Sep-2006     Deepti Goyal       SPR 12699        Bitmask set in SSA CTX
 **                                                     when Update/Re-Invite
 **                                                     send with SDP.
 **
 **                                                     in Media Change request.	
 ** 13-Jan-2007     Abhishek Dhammawat ICF 6.3          Per Line Feature - Added
 **                                                     for obsolete hold parameter
 ** 02-Feb-2007     Mohit Sharma       SPR 14417        In Case of forking, Cancel 
 **                                                     can't be sent on forked call
 **                                                     object
 ** 03-Feb-2007     Mohit Sharma       SPR 14416        In Case of Forking,memory 
                                                        leak in case of 3xx/4xx/5xx
 ** 04-Apr-2007     Deepti Goyal       Rel 6.3          Changes for 183 with SDP.
 **
 ** 17-Apr-2007     Abhishek Dutta     ICF 6.3          Added the changes for 
 **                                                     TWC-UPDATE Handling
 **
 ** 08-JUN-2007     Alok Tiwari        Rel early 7.0    Added code for stat collection
 ** 31-Jul-2007     Amit Mahajan       SPR 15478
 ** 18-Sept-2007    Abhishek Dhammawat SPR 16521
 ** 07-Sep-2007     Deepti Goyal        SPR 16019       Forceful clear of register 
 ** 08-Dec-2007     Abhishek Dhammawat SPR17667         Modified 
 **                                                     icf_ssa_process_refer_resp
 ** 20-Dec-2007     Amit Sharma        SPR-17772        Decrement open nw server
 **                                                     count when server closed 
 ** 25-Jun-2008     Rajiv Kumar        SPR - 18722      IPTK not compilable with gcc
 **                                                     version 4.2.3 
 ** 20-Aug-2008     Puneet Sharam      CSR 1-6179301
 **	04-Jun-2009		Aman Aggarwal	   SPR 19708		Change is done to sent with BYE 
 **														with credentials, if initially INVITE
 **														challenged
 ** 16-Jun-2009  Rohit Aggarwal		SPR 19747	Store the msg body of outgoing 
 **												request so that it can be 
 **												resent in request with credentials
 ** 26-Aug-2009  Anurag Khare       SPR 19989   Changes done for Notfiy
 **                                             chanllenge enhancement.
 ** 24-Sep-2009  Anurag Khare       SPR 20071   Fix for CSR 1-7798174
 **
 ** 18-Nov-2009	Pavneet Bajaj		SPR 20220	Fix for CSR 7991608
 ** 28-Nov-2009 Pavneet Bajaj       SPR 20226   Fix for CSR 8038197
 ** 04-Dec-2009 Pavneet Bajaj       SPR 20231  Fix for CSR 8061646 
 ** 16-Dec-2009	Aman Aggarwal		CSR 1-8081459 
 ** 30-Dec-2009 Pavneet Bajaj       SPR 20236   Fix for CSR 8070301
 ** 14-Apr-2010 Aman Aggarwal       SPR 20377   Fix for CSR 1-8492091
 *****************************************************************************
 **                Copyrights 2010, Aricent.
 *****************************************************************************/
/*Extif inclusions*/


/*Common H  inclusions*/
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_internal_api_id.h"
#include "icf_common_prototypes.h"
#include "icf_ecode.h"

/*Stack/UATK inclusions*/
#include "stack_headers.h"
#include "uatk_headers.h"
#include "sdf_configs.h"
/*SSA inclusions*/
#include "icf_ssa_types.h"
#include "icf_ssa_defs.h"
#include "icf_ssa_prototypes.h"
#include "icf_ssa_macro.h"

/*DBM inclusions*/
#include "icf_dbm_prototypes.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_close_nw_servers
 **
 ** DESCRIPTION:     Sent by CFG To trigger the closure of network servers
 *                  1.Call the e-shell API to close network servers
 *                  2.Stop the TCP purge timer if runing 
 *                  
 *****************************************************************************/
icf_return_t icf_ssa_process_close_nw_servers(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_nw_interface_info_st nw_intf_info  ;
    icf_cfg_close_nw_server_st    *p_close_nw_server =
        (icf_cfg_close_nw_server_st*)(p_ssa_pdb->\
                                    p_internal_msg->p_msg_data);
    icf_uint8_t              i = 0;
    icf_line_data_st         *p_line_data = ICF_NULL;
    icf_uint8_t server_type = 0;


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /*Invoke Eshell API to close all network FDs*/
    icf_port_memset(&nw_intf_info,0,sizeof(icf_nw_interface_info_st));
    nw_intf_info.bitmap = ICF_NULL;

    if (ICF_CFG_SELF_ADDR_PER_LINE == 
        p_ssa_pdb->p_glb_cfg_data->self_addr_per_line)
    {
        if ( ICF_SUCCESS ==
            icf_dbm_fetch_line_cntxt_blk(
                p_ssa_pdb->p_glb_pdb, p_close_nw_server->line_id,
                &p_line_data, p_ssa_pdb->p_ecode))
        {   
            server_type =  p_line_data->self_mode ; 

            p_ssa_pdb->p_line_data = p_line_data;  /* if per line feature is enable */

            for (i = 0; i <= ICF_WIN_TLS_SIP_SERVER; i++)
            {
                if (p_line_data->server_index[i] < ICF_MAX_NUM_OF_SERVERS)
                {
                    icf_es_close_nw_servers(
                      p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info,
                      p_line_data->server_index[i]);
                    p_line_data->server_index[i] = ICF_MAX_NUM_OF_SERVERS;
                    ((icf_global_port_info_st*)p_ssa_pdb->p_glb_pdb->p_glb_data->\
                      p_glb_port_info)->win_ipc_info.num_of_open_servers--;
                }
            }
        }
    }
    else
    {
        server_type =  p_ssa_pdb->p_glb_cfg_data->self_mode;   
        for (i = 0; i <= ICF_WIN_TLS_SIP_SERVER; i++)
        {
            if (p_ssa_pdb->p_glb_cfg_data->server_index[i] < 
                    ICF_MAX_NUM_OF_SERVERS) 
            {

                 icf_es_close_nw_servers(
                    p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info,
                    p_ssa_pdb->p_glb_cfg_data->server_index[i]);
                 p_ssa_pdb->p_glb_cfg_data->server_index[i] = 
                    ICF_MAX_NUM_OF_SERVERS;
                 ((icf_global_port_info_st*)p_ssa_pdb->p_glb_pdb->p_glb_data->\
                     p_glb_port_info)->win_ipc_info.num_of_open_servers--;
  
            }
        }
    }

    /*Stopping the PurgeTimer if running*/
    if (ICF_NULL != p_ssa_pdb->p_ssa_glb_data->p_purge_timer)
    {
        ICF_STOP_TIMER(p_ssa_pdb->p_glb_pdb, 
                p_ssa_pdb->p_ssa_glb_data->purge_timer_id,
                p_ssa_pdb->p_ecode, ret_val)
        if ( ret_val == ICF_FAILURE)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure!!PurgeTimerStop"));
        }
        
        /*Free the allocated timer block here*/
        icf_dbm_ret_mem_to_pool(p_ssa_pdb->p_glb_pdb, ICF_MEM_TIMER,
                p_ssa_pdb->p_ssa_glb_data->p_purge_timer, 
                p_ssa_pdb->p_ecode);
        p_ssa_pdb->p_ssa_glb_data->p_purge_timer = ICF_NULL;
        p_ssa_pdb->p_ssa_glb_data->purge_timer_id = ICF_NULL;
    }
    switch(server_type)
    {
#ifdef ICF_TRACE_ENABLE
        case ICF_TRANSPORT_TYPE_UDP : ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_UDP_SERVER_CLOSE)
             break;
        case ICF_TRANSPORT_TYPE_TCP :  ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TCP_SERVER_CLOSE)
             break;
#ifdef ICF_SECURE_SUPPORT
        case ICF_TRANSPORT_TYPE_TLS :  ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TLS_SERVER_CLOSE)
             break;
#endif
#endif

    }       
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ICF_SUCCESS;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_open_nw_servers
 **
 * DESCRIPTION:     Sent by CFG To trigger the opening of network servers
 *                  1.Set the transport address in default profile(used for Via)
 *                  2.Call ES routine to open network servers
 *                  3.Start the TCP Purge Timer
 *                      
 *****************************************************************************/
icf_return_t icf_ssa_process_open_nw_servers(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_glb_pdb_st           *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_ssa_glb_data_st      *p_ssa_glb_data = ICF_NULL;
    icf_config_data_st       *p_cfg_data = p_ssa_pdb->p_glb_cfg_data;
    icf_transport_address_st *p_transport_address = ICF_NULL;
    icf_nw_interface_info_st nw_intf_info;
    icf_error_t              *p_ecode;
    icf_uint8_t              p_transp_addr_str[100] = "";
    icf_cfg_open_nw_server_st    *p_open_nw_server = 
        (icf_cfg_open_nw_server_st*)(p_ssa_pdb->\
                                    p_internal_msg->p_msg_data);

    Sdf_st_initData             *p_default_profile = ICF_NULL;
    Sdf_st_error                sdf_error;
    icf_uint8_t                 ctr = 0;
    icf_line_data_st            *p_line_data = ICF_NULL;
    icf_line_id_t               line_id = 0;
    icf_uint8_t                 i = 0;
    icf_uint8_t                 serv_cnt = 0;


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    p_ecode = p_ssa_pdb->p_ecode;
    p_ssa_glb_data = p_ssa_pdb->p_ssa_glb_data;
    p_default_profile = p_ssa_glb_data->p_default_profile;

    /*
     * Now we have to set the transport address in default profile
     * For this we need to get the self-address parameter as configured
     * by User.
     */

    nw_intf_info.bitmap = 0;

    if (ICF_TRANSPORT_TYPE_UDP == p_open_nw_server->mode)
    {
        nw_intf_info.bitmap |= ICF_INTF_INFO_PROTOCOL_UDP;
        serv_cnt = 1;
    }
    else if (ICF_TRANSPORT_TYPE_TCP == p_open_nw_server->mode)
    {
        nw_intf_info.bitmap |= ICF_INTF_INFO_PROTOCOL_TCP;
        serv_cnt = 1;
    }
    else if (ICF_TRANSPORT_MODE_BOTH == p_open_nw_server->mode)
    {
        nw_intf_info.bitmap |= ICF_INTF_INFO_PROTOCOL_UDP;
        nw_intf_info.bitmap |= ICF_INTF_INFO_PROTOCOL_TCP;
        serv_cnt = 2;
    }
#ifdef ICF_SECURE_SUPPORT    
    else if (ICF_TRANSPORT_TYPE_TLS== p_open_nw_server->mode)
    {       
	    Sdf_st_sslParams    secure_params;
	    icf_port_memset(&secure_params,0,sizeof(Sdf_st_sslParams));

	    /* First check whether the TLS information has been configured by the user */
	    if (ICF_NULL == p_cfg_data->p_tls_init_data)
	    {
		    ret_val = ICF_FAILURE;
	    }
	    else
	    {
                    serv_cnt = 2;
		    nw_intf_info.bitmap |= ICF_INTF_INFO_PROTOCOL_TLS;

		    nw_intf_info.bitmap |= ICF_INTF_INFO_PROTOCOL_UDP;
		    nw_intf_info.bitmap |= ICF_INTF_INFO_PROTOCOL_TCP;

		    /* Set the TLS information into the default profile */
		    secure_params.dCertFormat = 
			    p_cfg_data->p_tls_init_data->cert_format;
		    secure_params.dCertVerifyMode = 
			    p_cfg_data->p_tls_init_data->cert_verify_mode;
		    secure_params.dDHParamFileFormat = 
			    p_cfg_data->p_tls_init_data->dh_param_file_format;
                    if (0 != icf_port_strlen(p_cfg_data->p_tls_init_data->ca_cert_dir_path.str))
                    {
		        secure_params.pCACertDirPath = 
			    Sdf_mc_strdupConfig(p_cfg_data->p_tls_init_data->ca_cert_dir_path.str);
                    }
                    else
                    {
		        secure_params.pCACertDirPath = ICF_NULL;
                    }
                    if (0 != icf_port_strlen(p_cfg_data->p_tls_init_data->cert_file.str))
                    {
		        secure_params.pCertFile = 
			    Sdf_mc_strdupConfig(p_cfg_data->p_tls_init_data->cert_file.str);
                    }
                    else
                    {
		        secure_params.pCertFile = ICF_NULL;
                    }
                    if (0 != icf_port_strlen(p_cfg_data->p_tls_init_data->ca_cert_file.str))
                    {
		        secure_params.pCACertFile = 
			    Sdf_mc_strdupConfig(p_cfg_data->p_tls_init_data->ca_cert_file.str);
                    }
                    else
                    {
		        secure_params.pCACertFile = ICF_NULL;
                    }
                    if (0 != icf_port_strlen(p_cfg_data->p_tls_init_data->cipher_suites.str))
                    {
		        secure_params.pCipherSuites = 
			    Sdf_mc_strdupConfig(p_cfg_data->p_tls_init_data->cipher_suites.str);
                    }
                    else
                    {
		        secure_params.pCipherSuites = ICF_NULL;
                    }
                    if (0 != icf_port_strlen(p_cfg_data->p_tls_init_data->dh_param_file.str))
                    {
		        secure_params.pDHParamFile = 
			    Sdf_mc_strdupConfig(p_cfg_data->p_tls_init_data->dh_param_file.str);
                    }
                    else
                    {
                        secure_params.pDHParamFile = ICF_NULL;
                    }
                    if (0 != icf_port_strlen(p_cfg_data->p_tls_init_data->private_key_file.str))
                    {
		        secure_params.pPrivateKeyFile = 
			    Sdf_mc_strdupConfig(p_cfg_data->p_tls_init_data->private_key_file.str);
                    }
                    else
                    {
                        secure_params.pPrivateKeyFile = ICF_NULL;
                    }
                    if (0 != icf_port_strlen(p_cfg_data->p_tls_init_data->private_key_passwd.str))
                    {
		        secure_params.pPrivateKeyPasswd = 
			    Sdf_mc_strdupConfig(p_cfg_data->p_tls_init_data->private_key_passwd.str);
                    }
                    else
                    {
                        secure_params.pPrivateKeyPasswd = ICF_NULL;
                    }

		    if (Sdf_co_fail == sdf_ivk_uaSetDefaultTlsInfo (
					    p_default_profile,&secure_params,&sdf_error))
		    {
			    ret_val = ICF_FAILURE;
		    }
		    else
		    {
			    /* 
			     * Set the information in the network interface info
			     * for opening a TLS server socket 
			     */ 
			    nw_intf_info.p_secure_data = (icf_void_t *)p_default_profile->pSslData->pData;
			    nw_intf_info.tls_server_port_num = p_cfg_data->p_tls_init_data->tls_sip_serv_port_num;
		    }
		    /* Free the mem alocated to SSL params */           
		    if(ICF_NULL != secure_params.pCACertDirPath)
		    {
			    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
					    (secure_params.pCACertDirPath),&sdf_error);
		    }
		    if(ICF_NULL != secure_params.pCertFile)
		    {
			    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
					    (secure_params.pCertFile),&sdf_error);
		    }
            if(ICF_NULL != secure_params.pCACertFile)
            {
                sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
                       (secure_params.pCACertFile),&sdf_error);
            }
		    if(ICF_NULL != secure_params.pCipherSuites)
		    {
			    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
					    (secure_params.pCipherSuites),&sdf_error);
		    }
		    if(ICF_NULL != secure_params.pDHParamFile)
		    {
			    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
					    (secure_params.pDHParamFile),&sdf_error);
		    }
		    if(ICF_NULL != secure_params.pPrivateKeyFile)
		    {
			    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
					    (secure_params.pPrivateKeyFile),&sdf_error);
		    }
		    if(ICF_NULL != secure_params.pPrivateKeyPasswd)
		    {
			    sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
					    (secure_params.pPrivateKeyPasswd),&sdf_error);
		    }
	    }
	    if (ICF_FAILURE == ret_val)
	    {
		return ret_val;
	    }
    }
#endif
    /*Initialize server indexes*/
    for (i = 0; i <= ICF_WIN_TLS_SIP_SERVER; i++)
        nw_intf_info.server_index[i] = ICF_MAX_NUM_OF_SERVERS;

    if (ICF_CFG_SELF_ADDR_PER_LINE == p_cfg_data->self_addr_per_line)
    {
        line_id = p_open_nw_server->line_id;
        if ( ICF_SUCCESS ==
            icf_dbm_fetch_line_cntxt_blk(
                p_glb_pdb, p_open_nw_server->line_id,
                &p_line_data, p_ssa_pdb->p_ecode))
        {
            nw_intf_info.remote_port_num =
	       p_line_data->self_addr.port_num;
		/* Fix for CSR-7991608 SPR-20220*/
		/*Assign line_data to p_ssa_pdb*/
		p_ssa_pdb->p_line_data=p_line_data;

            p_transport_address = &(p_line_data->self_addr);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        line_id = 0;

        nw_intf_info.remote_port_num =
	    p_cfg_data->self_ip_address.port_num;

        p_transport_address = &(p_cfg_data->self_ip_address);
    }

    if (ICF_FAILURE == ret_val)
    {
    }
    else if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
		    p_transport_address->addr.addr_type)
    {
	    icf_port_strcpy(p_transp_addr_str, 
			    p_transport_address->addr.addr.domain.str);
    }
    else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR == 
		    p_transport_address->addr.addr_type)
    {
	    icf_ssa_convert_to_IP_addr(p_transport_address,
			    p_transp_addr_str);
		    if ( 0 == icf_port_strcmp((icf_uint8_t *)"0.0.0.0",(icf_uint8_t *) p_transp_addr_str))
		    {
            /*Self IP address was not configured. We will get the IP
             * address, through system calls, in dotted notation string
             * form. Then we will use it to update the 4-octet structure
             * in p_transport_address for the IP address*/
            if (ICF_FAILURE ==
                    icf_ssa_get_and_populate_self_ip(
                        p_ssa_pdb, p_transport_address, p_transp_addr_str))
            {
                ret_val = ICF_FAILURE;
            }
        }/*End of block for resolution for self ip address and 
           popualtion of the config data with it*/
    }
    else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
				p_transport_address->addr.addr_type)
    {
        icf_port_strcpy(p_transp_addr_str,
			p_transport_address->addr.addr.ipv6_addr.str);
    }
	else{
		ret_val = ICF_FAILURE;
	}
    if ( ICF_FAILURE != ret_val)
    {
        /*
         * Set the string to be set into the User-Agent header into the 
         * SSA global data.On the first invocation of this
         * function ,if this is set in the CFG data,then this header
         * will be inserted in all the requests and responses
         */
        if (ICF_NULL != p_cfg_data->p_phone_info)
        {
            icf_port_strcpy(p_ssa_glb_data->user_agent_str.str,
                    p_cfg_data->p_phone_info->str);
            p_ssa_glb_data->user_agent_str.str_len = 
                (icf_uint16_t)icf_port_strlen(p_ssa_glb_data->user_agent_str.str);
        }

        /*- Set Default Transport [Used for via] -*/
        if (Sdf_co_fail == sdf_ivk_uaSetDefaultTransport(p_default_profile,
                    (Sdf_ty_s8bit *)p_transp_addr_str, \
		    p_transport_address->port_num, &sdf_error))
        {
            /*
             * Raise a Major Category Error from here
             */
            ret_val = ICF_FAILURE;
        }
        /*
         * If this is the first invocation of this function
         * add the User-Agent,Supported and Allow header
         * into the global profile
         */
        else if ((ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS == 
                    (p_ssa_glb_data->ssa_glb_bitmask & 
                     ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS)))
        {
            /*Add User-Agent header in InitData*/
            if((ICF_NULL != icf_port_strlen(
                            p_ssa_glb_data->user_agent_str.str)) &&
                    (Sdf_co_fail == 
                     sdf_ivk_uaAddDefaultHeader(
                         p_default_profile,(icf_int8_t *)"User-Agent",
                         (icf_int8_t *)p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                         Sdf_co_true,&sdf_error)))
            {
                sdf_ivk_uaFreeInitData(p_default_profile);
                /*Raise a Major error*/
                ret_val = ICF_FAILURE;
            }
            else
            {
                Sdf_ty_s8bit    *p_supported_list[ICF_SSA_NUM_OF_EXTN_SUPP] 
                    = {"100rel", "replaces"};

                /* Add Supported header to the user profile */
                for (ctr=0; ctr < ICF_SSA_NUM_OF_EXTN_SUPP; ctr++)
                {
                    if(Sdf_co_fail == sdf_ivk_uaAddDefaultHeader (
                                p_default_profile,
                                "Supported",
                                p_supported_list[ctr],
                                Sdf_co_true, &sdf_error))
                    {
                        sdf_ivk_uaFreeInitData(p_default_profile);
                        /*Raise a Major error*/
                        ret_val = ICF_FAILURE;

                        break;
                    }
                }
            } /* else */

            if (ICF_FAILURE != ret_val)
            {
                Sdf_ty_s8bit *p_allowed[ICF_SSA_MAX_METHODS_ALLOWED] = {
                    (Sdf_ty_s8bit *)"INVITE",
                    (Sdf_ty_s8bit *) "BYE",
                    (Sdf_ty_s8bit *) "CANCEL",
                    (Sdf_ty_s8bit *) "ACK",
                    (Sdf_ty_s8bit *)"INFO",
                    (Sdf_ty_s8bit *)"PRACK",
                    (Sdf_ty_s8bit *)"OPTIONS",                        
                    (Sdf_ty_s8bit *)"NOTIFY",
                    (Sdf_ty_s8bit *)"REFER",                        
                    (Sdf_ty_s8bit *)"UPDATE"
                };
                for( ctr =0;ctr < ICF_SSA_MAX_METHODS_ALLOWED; ctr++)
                {
                    if (Sdf_co_fail == sdf_ivk_uaAddDefaultHeader (
                                p_default_profile,
                                "Allow",
                                p_allowed[ctr],
                                Sdf_co_true,&sdf_error))
                    {
                        sdf_ivk_uaFreeInitData(p_default_profile);
                        /*Raise a Major error*/
                        ret_val = ICF_FAILURE;

                        break;
                    }
                }
            }
            if (ICF_SUCCESS == ret_val)
            {
                /*
                 * Reset the bit ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS
                 * in SSA global data so that these headers are 
                 * not inserted on every invocation of 
                 * ICF_CFG_OPEN_NW_SERVER
                 */
                p_ssa_glb_data->ssa_glb_bitmask &= 
                     ~ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS;
            }
        }

        if (ICF_FAILURE == ret_val)
        {
            ret_val = ICF_FAILURE;
        }
        else if ((((icf_global_port_info_st *)(p_glb_pdb->p_glb_data->p_glb_port_info))->win_ipc_info.num_of_open_servers + serv_cnt)
                 > ICF_MAX_NUM_OF_SERVERS)
        {
             *p_ecode = ICF_CAUSE_MAX_SERVER_LIMIT_REACHED;
             ret_val = ICF_FAILURE;
        }
        /*We are not setting the ContactTransport List as we will
         * not use multiple ports for receiving requests*/
		/*Fix done for SPR-18722 : CSR-1-6068965*/
        else if ( ICF_FAILURE == icf_es_open_nw_port(
                    p_glb_pdb->p_glb_data->p_glb_port_info,
                    (icf_nw_interface_info_st *)&nw_intf_info,(icf_int8_t *)p_transp_addr_str,
                    p_ecode))
	{
#ifdef ICF_TRACE_ENABLE
	    p_ssa_pdb->result= ICF_FAILURE;
	    if( ICF_TRANSPORT_TYPE_UDP == p_open_nw_server->mode )
            {
                ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_UDP_SERVER_OPEN)
            }
            else if (ICF_TRANSPORT_TYPE_TCP == p_open_nw_server->mode)
            {
                ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TCP_SERVER_OPEN)
            }
#ifdef ICF_SECURE_SUPPORT  
            else if (ICF_TRANSPORT_TYPE_TLS== p_open_nw_server->mode)
            {
                ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TLS_SERVER_OPEN)
            }
#endif
#endif
            /*
             * Raise a Major Category Error from here
             */
            for(i = 0;i <= ICF_WIN_TLS_SIP_SERVER; i++)
            {
                if(ICF_MAX_NUM_OF_SERVERS != nw_intf_info.server_index[i])
                {
                    icf_es_close_nw_servers(
                        p_glb_pdb->p_glb_data->p_glb_port_info,
                    nw_intf_info.server_index[i]);
                    /*Initialize server_index to MAX, which means this server
                     * is not active*/
                    nw_intf_info.server_index[i] = ICF_MAX_NUM_OF_SERVERS;

                    /*Decrement num of open server as server is closed now*/ 
                    ((icf_global_port_info_st*)p_glb_pdb->p_glb_data->\
                        p_glb_port_info)->win_ipc_info.num_of_open_servers--;
                }
            }
            p_ssa_pdb->p_ecode = p_ecode;
            ret_val = ICF_FAILURE;
        }
        else
        {
#ifdef ICF_TRACE_ENABLE
	    p_ssa_pdb->result = ICF_SUCCESS;
	    if( ICF_TRANSPORT_TYPE_UDP == p_open_nw_server->mode )
	    {
		ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_UDP_SERVER_OPEN)
    	    }
    	    else if (ICF_TRANSPORT_TYPE_TCP == p_open_nw_server->mode)
    	    {
        	ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TCP_SERVER_OPEN)
    	    }
#ifdef ICF_SECURE_SUPPORT   
    	    else if (ICF_TRANSPORT_TYPE_TLS== p_open_nw_server->mode)
    	    {      
            	ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_TLS_SERVER_OPEN)
	    }
#endif
#endif
            /*If Transport is TCP or TLS then start purge timer*/
            if ((ICF_NULL !=
                (nw_intf_info.bitmap & ICF_INTF_INFO_PROTOCOL_TCP)) ||
                (ICF_NULL != (nw_intf_info.bitmap & ICF_INTF_INFO_PROTOCOL_TLS)))
            {
                /*Start the Purge Timer*/
                if ( ICF_FAILURE == icf_ssa_start_purge_timer(p_ssa_pdb))
                {
                    /*If Purge timer start is fail then close all opened socket*/
                    for(i = 0;i <= ICF_WIN_TLS_SIP_SERVER; i++)
                    {
                        if(ICF_MAX_NUM_OF_SERVERS !=
                                nw_intf_info.server_index[i])
                        {
                            icf_es_close_nw_servers(
                                p_glb_pdb->p_glb_data->p_glb_port_info,
                                nw_intf_info.server_index[i]);

                            nw_intf_info.server_index[i] = ICF_MAX_NUM_OF_SERVERS;

                            ((icf_global_port_info_st*)p_glb_pdb->p_glb_data->\
                             p_glb_port_info)->win_ipc_info.num_of_open_servers--;
                        }
                    }
                }
            }

            if (ICF_CFG_SELF_ADDR_PER_LINE == 
                p_cfg_data->self_addr_per_line)
            {
                for (i = 0; i <= ICF_WIN_TLS_SIP_SERVER; i++)
                {
                    /* Fix for CSR 1-5338107 */
	            /* Overwrite this in CFG data only if it was not valid */
		    if (ICF_MAX_NUM_OF_SERVERS == p_line_data->server_index[i])
		    {
                       p_line_data->server_index[i] = 
                       nw_intf_info.server_index[i];
                    }
                }
            }
            else
            {
                for (i = 0; i <= ICF_WIN_TLS_SIP_SERVER; i++)
                {
                    /* Fix for CSR 1-5338107 */
	            /* Overwrite this in CFG data only if it was not valid */
	            if (ICF_MAX_NUM_OF_SERVERS == p_cfg_data->server_index[i])
		    {
                       p_cfg_data->server_index[i] =
                       nw_intf_info.server_index[i];
                    }
                }
            }
        }/*Purge timer started if required*/
    }
    
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_setup
 **
 * DESCRIPTION:     API handler for CALL INITIATION FROM CC
 *                  1.Allocate SSA Context 
 *                  2.Call StartTransaction here on the call object
 *                  3.Clone the global profile and set From, 
 *                    Contact in it.
 *                  4.Set the cloned profile in CallObject
 *                  5.Insert headers like Privacy,Replaces,Referred By,
 *                    Diversion (if applicable). 
 *                  6.Call routine to form SdpMessage for local SDP
 *                  7.Call SDF API to set it in MsgBodyList 
 *                    in Transaction
 *                  8.Set Dest Transport Address
 *                  9.If the tramsport mode is set as BOTH and there is a failure
 *                    in sending the message over TCP,then we revert to UDP
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_call_setup(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st           *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_call_ctx_st          *p_call_ctxt = p_glb_pdb->p_call_ctx;
    icf_line_data_st         *p_line_data = p_ssa_pdb->p_line_data;
    icf_error_t              *p_ecode = p_ssa_pdb->p_ecode;
    icf_nw_otg_call_setup_st *p_otg_call_setup = 
        (icf_nw_otg_call_setup_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);

    Sdf_st_overlapTransInfo     *p_overlap_txn = Sdf_co_null;
    SdpMessage                  *p_sdp_msg = Sdf_co_null;
    Sdf_st_callObject           *p_call_obj = Sdf_co_null;
    Sdf_st_error                sdf_error;
    Sdf_ty_s8bit                transport[5];
    icf_return_t             memfree_retval;
    icf_boolean_t            trans_mode_present = ICF_FALSE;
    icf_int8_t              *p_content_type = ICF_NULL;
    icf_uint8_t              count = 0;
    
    icf_uint8_t              param_val[5]={'\0'};
#if	defined ICF_SIGCOMP_SUPPORT || defined ICF_IPSEC_ENABLED
    icf_rgm_context_st   *p_rgm_ctxt = ICF_NULL;
#endif

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Klocworks */
    if (ICF_NULL == p_call_ctxt)
    {
        return ICF_FAILURE;
    }
    if(ICF_CALL_AUTHENTICATION_REQ & p_call_ctxt->common_bitmask)
    {
		p_ssa_ctxt = p_call_ctxt->p_sip_ctx;
		p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
        p_ssa_pdb->p_method = (icf_uint8_t *)"INVITE";
        icf_ssa_send_authorized_req(p_ssa_pdb);
        return ret_val;
    }


    /* If transport received in remote address is not same as transport_mode
     * on that line, then return failure, otherwise set transport=(UDP/TCP)
     * in req_uri and contact*/
    if (ICF_TRUE == icf_cmn_check_if_param_present(
        &(p_call_ctxt->remote_party_addr.addr_val), (icf_uint8_t *)"transport", param_val,
        (icf_uint8_t *)"To", &(p_call_ctxt->tag_list)))       
    {
         trans_mode_present = ICF_TRUE;
         /* Some servers like Cisco, Nortel CS1K do not insert the 
          * ';transport=udp' in To URI, due to which strict URI comparison
         * in UATK fails and message is rejected. To ensure interop for now,
         * IPTK will not send transport URI param when the value is UDP.
         */
        if (ICF_NULL == icf_port_strcmp(param_val, (icf_uint8_t *)"udp"))
        {
            p_ssa_pdb->send_trans_param = ICF_FALSE;
        }
        else
        {

            p_ssa_pdb->send_trans_param = ICF_TRUE;
        }
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Include transport URI param in request: %d\n",
                    p_ssa_pdb->send_trans_param));

    }

     
     /* 
     * Set the pdb bitmask to indicate that the user address is to be 
     * read from call ctxt
     */
     p_ssa_pdb->common_bitmask |= ICF_SSA_USER_ADDR_FRM_CC;
     
     /*
     * Allocate SSA Context here, If it fails we return failure.
     * Assumption: This failure will be propagted back to MRM
     * which then will trigger FORCEFULL CALL CLEAR, SSA will check
     * it' SSA CTXT in the received CC CTXT and since it will not
     * find any, it will just return success there
     * Further steps of processing in this routine:
     * Call StartTransaction here on the call object
     * Clone the global profile and set From, Contact in it.
     * Set the cloned profile in CallObject
     * Call routine to form SdpMessage for local SDP
     * Call SDF API to set it in MsgBodyList in Transaction
     * Set Dest Transport Address
     */
     ICF_MEMGET(p_glb_pdb,\
         ICF_PORT_SIZEOF(icf_ssa_ctxt_st), ICF_MEM_COMMON, \
         p_ssa_ctxt, ICF_RET_ON_FAILURE, p_ecode, \
         ret_val)

	 if(ICF_NULL == p_ssa_ctxt)
	 {
		return ICF_FAILURE;
	 }
         
     /* Initialize the elements of new ssa ctxt */
     icf_ssa_init_ssa_context(p_ssa_pdb,p_ssa_ctxt);
     p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

     if ( ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx)
     {
	 p_ssa_ctxt->p_call_ctxt = p_ssa_pdb->p_glb_pdb->p_call_ctx;
         p_ssa_ctxt->p_call_ctxt->p_initial_sip_ctx = p_ssa_ctxt;
#ifdef ICF_LOAD_DEBUG
         printf("\nSSA ctxt allocated	%d\n", p_ssa_ctxt->p_call_ctxt->call_id);
         p_ssa_ctxt->call_id = p_ssa_ctxt->p_call_ctxt->call_id;
#endif
     }

     if (ICF_NULL != (ICF_CFG_CC_CALL_THRU_SIP_PROXY & 
         p_ssa_pdb->p_glb_cfg_data->default_settings))
     {  
         /*Fetch transport_mode for INVITE request*/
         if(ICF_FAILURE == icf_ssa_check_for_trans_param(p_ssa_pdb, 
             p_line_data->transport_mode,
             ((ICF_TRUE == trans_mode_present)?param_val:ICF_NULL),
             ICF_TRUE))
         {
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_FAILURE;
         }
     } /* p_line_data->line_data_receive... */
     else
     {
          /*Fix for CSR 8038197 SPR 20226  */
          /*Issue:When mode set as udp through ICF_SET_TRANS_PARAMS 
            in case of direct call per line, EGT tried to open a tcp socket
            because trasnport mode was picked from  p_ssa_pdb->
            p_glb_cfg_data which by default is set as BOTH*/
            /*Fix:In case of direct call pick transport mode
            from line data as done for call through proxy*/

         /*Fetch transport_mode for INVITE request.*/
         if(ICF_FAILURE == icf_ssa_check_for_trans_param(p_ssa_pdb, 
             p_line_data->transport_mode,
             ((ICF_TRUE == trans_mode_present)?param_val:ICF_NULL),
             ICF_FALSE))
         {
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_FAILURE;
         }
     }
     
     /* Invoke a function which shall validate the mode and addr type */
     if (ICF_FAILURE == icf_ssa_validate_mode_n_addr_type(p_ssa_pdb))
     {
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
            ret_val = ICF_FAILURE;
	    *p_ecode = ICF_ECODE_MISMATCH_IN_ADDR_PLAN;
            return ret_val;
     }
     if ((ICF_TRANSPORT_TYPE_TCP == 
         p_ssa_pdb->p_ssa_ctxt->transport) || 
         (ICF_TRANSPORT_MODE_BOTH == p_ssa_pdb->p_ssa_ctxt->transport))
     {
         icf_port_strcpy((icf_uint8_t *)transport,(icf_uint8_t *)"TCP");
     }
     else if (ICF_TRANSPORT_TYPE_TLS == 
         p_ssa_pdb->p_ssa_ctxt->transport)
     {
         icf_port_strcpy((icf_uint8_t *)transport,(icf_uint8_t *)"TLS");
     }
     else
     {
         icf_port_strcpy((icf_uint8_t *)transport,(icf_uint8_t *)"UDP");
     }	

#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
/* We will net the transport mode to none for P-CSCF Discovery */
    icf_port_strcpy(transport,"");
ICF_CHECK_IMS_END
#endif
#endif

#if	defined ICF_SIGCOMP_SUPPORT || defined ICF_IPSEC_ENABLED
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
     if(ICF_FAILURE == 
         icf_rgm_fetch_ctxt_for_uri(p_glb_pdb,
         p_call_ctxt->p_preferred_identity,
         &p_rgm_ctxt,
         p_ssa_pdb->p_ecode))
     {
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE;
     }
ICF_CHECK_IMS_END
#endif
#ifdef ICF_SIGCOMP_SUPPORT
    if((ICF_SUCCESS == ret_val) && 
		(ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required))
    {
         if (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt)
         {
             p_ssa_ctxt->p_sigsa_ctxt = ((icf_ssa_ctxt_st*)
                 (p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt;
             if(ICF_NULL != p_ssa_ctxt->p_sigsa_ctxt)
             {
                 /* In crement the reference Counter */
                 icf_sigsa_incr_ref_count_to_sigsa_ctxt(
                     p_ssa_ctxt->p_sigsa_ctxt);
             }
         }
         else
         {
             p_ssa_ctxt->p_sigsa_ctxt = ICF_NULL;
         }
    }
#endif
#ifdef ICF_IPSEC_ENABLED
    if(ICF_SUCCESS == ret_val)
    {
         /* if IPsec is configured, then set the SA context and the
         * RGM context into the SSA context and SSA pdb structures 
         */
         if (ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled)
         {
             if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context) && 
                 /* SA exists */
                 (ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context->is_active))
             {
             /* save the SA context and the REGM context pointers in the
             * SSA PDB for future reference
                 */
                 p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
                 p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;
                 
                 /* also set these pointers in the SSA ctxt. Required incase of 
                 * transaction time-out scenarios */
                 p_ssa_ctxt->ssa_sa_context.p_sa_context = 
                     ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
                 p_ssa_ctxt->ssa_sa_context.p_rgm_context = 
                     p_rgm_ctxt;
             }
             else if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context) && 
                 (ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context->is_active))
             {
             /* save the SA context and the REGM context pointers in the
             * SSA PDB for future reference
                 */
                 p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
                 p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;
                 
                 /* also set these pointers in the SSA ctxt. Required incase of 
                 * transaction time-out scenarios */
                 p_ssa_ctxt->ssa_sa_context.p_sa_context = 
                     ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
                 p_ssa_ctxt->ssa_sa_context.p_rgm_context = 
                     p_rgm_ctxt;
             }
        }
    }
#endif
     if(ret_val == ICF_FAILURE)
     {
     }
     else if ( ICF_FAILURE == icf_ssa_get_new_call_obj(
         p_ssa_pdb, &p_call_obj))
     {
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE;
     }
     /*Success means that the call object is allocated, 
     * the SSA ctxt-nw call ID map has space for this new
     * mapping and the SSA pdb has been updated for the
     * allocated call object pointer*/
     else if (Sdf_co_fail == sdf_ivk_uaStartTransaction(
         p_call_obj,
         &p_overlap_txn,
         "INVITE",
         &sdf_error))
     {
         /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
         sdf_ivk_uaFreeCallObject(&p_call_obj);
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE;
     }
     else if ( ICF_FAILURE == 
         icf_ssa_update_callobj_for_profile(p_ssa_pdb))
     {
         sdf_ivk_uaClearTransaction(
             p_call_obj,
             p_overlap_txn,
             "INVITE",
             Sdf_en_uacTransaction);
         sdf_ivk_uaFreeCallObject(&p_call_obj);
         
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE;
     }
     /*Till this point we have also allocated a profile, so
     * in case of failures from now-on we will need to free
     * that instance of profile too by sdf_ivk_uaFreeInitData*/
     else if (ICF_FAILURE == 
         icf_ssa_set_to_in_callobj(p_ssa_pdb))
     {
         sdf_ivk_uaClearTransaction(
             p_call_obj, p_overlap_txn, 
             "INVITE", Sdf_en_uacTransaction);
         sdf_ivk_uaFreeCallObject(&p_call_obj);
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE ;
     }
     else if ( Sdf_co_fail == 
         sdf_ivk_uaSetTransportSchemeInTransaction(
         p_call_obj->pUacTransaction,p_overlap_txn, 
         transport, &sdf_error))
     {
         sdf_ivk_uaClearTransaction(
             p_call_obj, p_overlap_txn, 
             "INVITE", Sdf_en_uacTransaction);
         sdf_ivk_uaFreeCallObject(&p_call_obj);
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE ;
     }
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
     if (ICF_FAILURE == ret_val)
     {
     }
    /* Fill Route Header from Service Route if stored for user-id while 
     registering*/
     else if(ICF_FAILURE == 
         icf_ssa_set_RouteHeaderListToInitData(p_ssa_pdb,
         p_call_ctxt->p_preferred_identity,
         p_glb_pdb->p_call_ctx->line_id,p_ecode))
     {
		 sdf_ivk_uaClearTransaction(
             p_call_obj, p_overlap_txn, 
             "INVITE", Sdf_en_uacTransaction);
         sdf_ivk_uaFreeCallObject(&p_call_obj);
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE ;
     }  
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
     if (ICF_FAILURE == ret_val)
     {
     }
     else if ((ICF_FAILURE == 
         	 icf_ssa_set_RouteHeaderListToInitData(p_ssa_pdb,
		 ICF_NULL,
		 p_glb_pdb->p_call_ctx->line_id,p_ecode)))
     {
	 sdf_ivk_uaClearTransaction(
		 p_call_obj, p_overlap_txn, 
		 "INVITE", Sdf_en_uacTransaction);
		 sdf_ivk_uaFreeCallObject(&p_call_obj);
		 ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
		 ICF_MEM_COMMON, p_ecode, ret_val)
		 ret_val = ICF_FAILURE ;
     }
ICF_CHECK_IMS_END
#endif   
 		
#ifdef ICF_SESSION_TIMER
    if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    { 
       if (ICF_FAILURE == ret_val)
       {
       }
       else if(icf_ssa_set_session_timer_details(p_ssa_pdb,p_call_obj) == ICF_FAILURE)
       {
         sdf_ivk_uaClearTransaction(
             p_call_obj, p_overlap_txn, 
             "INVITE", Sdf_en_uacTransaction);
         sdf_ivk_uaFreeCallObject(&p_call_obj);
         ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, 
             ICF_MEM_COMMON, p_ecode, ret_val)
             ret_val = ICF_FAILURE;	
       } 
    }
#endif
    if (ICF_FAILURE == ret_val)
    {
    }
    else if (Sdf_co_fail == 
        sdf_ivk_uaMakeCall (p_call_obj, &sdf_error))
    {
        /*Raise/Set Major Error here->UATK_PROCESSING_FAILURE*/
        sdf_ivk_uaClearTransaction(p_call_obj,
            p_overlap_txn, 
            "INVITE", Sdf_en_uacTransaction);
        sdf_ivk_uaFreeCallObject(&p_call_obj);
        ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
            ICF_MEM_COMMON, p_ecode, ret_val)
            ret_val = ICF_FAILURE ;
    }
    else
    {
        SipOptions 		options;
        
        if (ICF_NULL == p_ssa_ctxt)
        {
            if (p_call_obj!= ICF_NULL)
            sdf_ivk_uaFreeCallObject(&p_call_obj);
            return ICF_FAILURE;
        }
        p_call_ctxt->p_sip_ctx = p_ssa_ctxt;

        p_ssa_ctxt->p_ssa_pdb = p_ssa_pdb;
        ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_glb_pdb = p_glb_pdb;
        p_ssa_ctxt->p_call_ctxt = p_glb_pdb->p_call_ctx;
        p_ssa_ctxt->p_call_obj = p_call_obj;
      
        /*check if diversion data is to be sent,
        * if yes, make the headers*/
        if ((ICF_NULL != p_ssa_ctxt->p_call_ctxt->
            p_diversion_header_list) && 
            (ICF_NULL != p_ssa_ctxt->p_call_ctxt->
            p_diversion_header_list->no_of_diversion_data))
        {
            if ( ICF_FAILURE == 
                icf_ssa_form_and_insert_div_hdr(
                p_ssa_pdb,
                p_call_obj->pUacTransaction->pSipMsg))
            {
                sdf_ivk_uaClearTransaction(p_call_obj,
                    p_overlap_txn, 
                    "INVITE", Sdf_en_uacTransaction);
                sdf_ivk_uaFreeCallObject(&p_call_obj);
                ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                    ICF_MEM_COMMON, p_ecode, ret_val)

                /* Set SSA ctxt as NULL in call context */
                p_call_ctxt->p_sip_ctx = ICF_NULL;
                return ICF_FAILURE;
            }
        }
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
        /*Commenting this code as we are not using globally configured
        privacy flag inICF*/
        if ((ICF_NULL == icf_port_strcmp(
            p_call_ctxt->local_address.addr_val.str,
            (icf_uint8_t *)"anonymous@anonymous.invalid")) &&
            (ICF_CFG_CC_PRIVACY_HEADER_PRESENT == 
            (p_ssa_pdb->p_glb_cfg_data->default_settings &
            ICF_CFG_CC_PRIVACY_HEADER_PRESENT)))
        {
            if ( ICF_FAILURE == 
                icf_ssa_form_and_insert_priv_hdr(
                p_ssa_pdb,
                p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
            {
                sdf_ivk_uaClearTransaction(p_call_obj,
                    p_overlap_txn, 
                    "INVITE", Sdf_en_uacTransaction);
                sdf_ivk_uaFreeCallObject(&p_call_obj);
                ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
                p_call_ctxt->p_sip_ctx = ICF_NULL;
                return ICF_FAILURE;
            }
        }
ICF_CHECK_IMS_END     
#endif   
 
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
        if ( ICF_FAILURE == 
            icf_ssa_form_and_insert_request_disposition_hdr(
            p_ssa_pdb, p_call_obj->pUacTransaction->pSipMsg))
        {
            sdf_ivk_uaClearTransaction(p_call_obj,
                p_overlap_txn, "INVITE", Sdf_en_uacTransaction);
            sdf_ivk_uaFreeCallObject(&(p_call_obj));
            ICF_MEMFREE(p_glb_pdb, p_ssa_ctxt, ICF_MEM_COMMON, \
                p_ecode, memfree_retval)
                return ICF_FAILURE;
        }

        if ( ICF_FAILURE == 
            icf_ssa_form_and_insert_access_nw_info_hdr(
            p_ssa_pdb, p_call_obj->pUacTransaction->pSipMsg))
        {
            sdf_ivk_uaClearTransaction(p_call_obj,
                p_overlap_txn, "INVITE", Sdf_en_uacTransaction);
            sdf_ivk_uaFreeCallObject(&p_call_obj);
            ICF_MEMFREE(p_glb_pdb, p_ssa_ctxt, ICF_MEM_COMMON, \
                p_ecode, memfree_retval)
                return ICF_FAILURE;
        }
    
        if (ICF_TRUE == p_call_ctxt->send_privacy_hdr) 
        {
            if ( ICF_FAILURE == 
                icf_ssa_form_and_insert_priv_hdr(
                p_ssa_pdb,
                p_call_obj->pUacTransaction->pSipMsg))
            {
                sdf_ivk_uaClearTransaction(p_call_obj,
                    p_overlap_txn, "INVITE", Sdf_en_uacTransaction);
                sdf_ivk_uaFreeCallObject(&p_call_obj);
                ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, ICF_MEM_COMMON, \
                    p_ecode, memfree_retval)
                    return ICF_FAILURE;
            }
            else
            {
                p_call_ctxt->send_privacy_hdr = ICF_FALSE;
            }            
        }
ICF_CHECK_IMS_END
#endif

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_call_ctxt->p_preferred_identity))
        if (ICF_FAILURE == icf_ssa_form_and_insert_pref_identity_hdr(
            p_ssa_pdb, p_call_obj->pUacTransaction->pSipMsg))
        {
            sdf_ivk_uaClearTransaction(p_call_obj,
                p_overlap_txn, "INVITE", Sdf_en_uacTransaction);
            sdf_ivk_uaFreeCallObject(&p_call_obj);
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, memfree_retval)
                return ICF_FAILURE;
        }
ICF_CHECK_IMS_END
#endif
        /* This is being commented out and will be freed by CC on clearing 
        * the call context. We cannot free the call_ctx->p_pref_id here 
        * since it might be needed in subsequent INVITE of 302 response,
        * which uses the same call ctx.*/
        /*else
        {    
        ICF_MEMFREE(p_glb_pdb, p_call_ctxt->p_preferred_identity, \
        ICF_MEM_COMMON, p_ecode, ret_val)
            }*/
            



    /* 
    * Check if referred by header needs to be added
    * If yes,call function which makes and inserts the
    * referred by header
    */
        if (ICF_NULL != (p_otg_call_setup->bitmask &
            ICF_NW_OTG_SETUP_REF_BY))
        {
            if ( ICF_FAILURE == icf_ssa_form_and_insert_ref_by_hdr(
                p_ssa_pdb,
                p_call_obj->pUacTransaction->pSipMsg))
            {
                sdf_ivk_uaClearTransaction(p_call_obj,
                    p_overlap_txn, 
                    "INVITE", Sdf_en_uacTransaction);
                sdf_ivk_uaFreeCallObject(&p_call_obj);
                ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                    ICF_MEM_COMMON, p_ecode, ret_val)
                /* Set SSA ctxt as NULL in call context */
                p_call_ctxt->p_sip_ctx = ICF_NULL;
                return ICF_FAILURE;
            }
        }
        /* 
        * Check if replaces  header needs to be added
        * If yes,call function which makes and inserts the
        * replaces header
        */
        if (ICF_NULL != (p_otg_call_setup->bitmask &
            ICF_NW_OTG_SETUP_REPLACE_CALL_ID))
        {
            if ( ICF_FAILURE == icf_ssa_form_and_insert_replaces_hdr(
                p_ssa_pdb,
                p_call_obj->pUacTransaction->pSipMsg))
            {
                sdf_ivk_uaClearTransaction(p_call_obj,
                    p_overlap_txn, 
                    "INVITE", Sdf_en_uacTransaction);
                sdf_ivk_uaFreeCallObject(&p_call_obj);
                ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
                /* Set SSA ctxt as NULL in call context */
                p_call_ctxt->p_sip_ctx = ICF_NULL;
                    return ICF_FAILURE;
            }
        }
#ifdef ICF_IPSEC_ENABLED
        if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
            /* if SA exits */
            (ICF_NULL != p_ssa_pdb->p_sa_context))
        {
            icf_uint8_t    *p_value = ICF_NULL;

            /* insert security-verify header in the otg INVITE */
            if (ICF_FAILURE == icf_ssa_get_sec_verify_string (
                p_ssa_pdb,
                &p_value,
                p_ssa_pdb->p_sa_context))
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == 
                icf_ssa_insert_unknown_hdr_in_sip_msg(
                p_ssa_pdb,
                (icf_uint8_t*)"Security-Verify",
                p_value, 
                p_call_obj->pUacTransaction->pSipMsg))
            {
                ICF_PRINT(("\nFailed to insert unknown header \
                    (Security-Verify)"));
                ret_val = ICF_FAILURE;
            }
            if (ICF_NULL != p_value)
            {
                ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                    p_value,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    ret_val)
            }
            if (ICF_SUCCESS == ret_val )
            {
            ICF_PRINT(("\nSecurity Verify Header inserted to \
                outgoing Register "));
            }
        }
#endif
        
        /*If the bitmask is set, then send P-Preferred-Identity header in INVITE*/
        if(ICF_PREFERRED_ID_HDR_REQD & p_call_ctxt->common_bitmask_2)
        {
           if(ICF_FAILURE == icf_ssa_form_and_insert_pref_identity_hdr(
                                                     p_ssa_pdb,
                                                     p_call_obj->pUacTransaction->pSipMsg))
           {
                ICF_PRINT(((icf_uint8_t *)"\nFailed to insert unknown header \
                    (P-Preferred-Identity)"));
				sdf_ivk_uaClearTransaction(p_call_obj,
					p_overlap_txn, "INVITE", Sdf_en_uacTransaction);
				sdf_ivk_uaFreeCallObject(&p_call_obj);
					ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, memfree_retval)
                return ICF_FAILURE;            
           }
        } 
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
		if (ICF_FAILURE == 
                icf_ssa_insert_unknown_hdr_in_sip_msg(
                p_ssa_pdb,
                (icf_uint8_t*)"Require",
                (icf_uint8_t*)"precondition", 
                p_call_obj->pUacTransaction->pSipMsg))
            {
                ICF_PRINT(((icf_uint8_t *)"\nFailed to insert unknown header \
                    (Require)"));
				sdf_ivk_uaClearTransaction(p_call_obj,
					p_overlap_txn, "INVITE", Sdf_en_uacTransaction);
				sdf_ivk_uaFreeCallObject(&p_call_obj);
					ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, memfree_retval)
                return ICF_FAILURE;            
			}
ICF_CHECK_QOS_SUPPORT_END
#endif  
       /*Checks that if P-Early-Media Hdr needs to be inserted
        *If yes,then insert with value as "recvonly,supported"
        */
        if(ICF_CALL_CONFIG_EARLY_MEDIA_HDR_SUPPORTED & 
                p_ssa_pdb->p_glb_cfg_data->default_settings)
        {
           if (ICF_FAILURE == 
                icf_ssa_insert_unknown_hdr_in_sip_msg(
                p_ssa_pdb,
                (icf_uint8_t*)"P-Early-Media",
                (icf_uint8_t*)"recvonly,supported",
                p_call_obj->pUacTransaction->pSipMsg))
          {
                ICF_PRINT(((icf_uint8_t *)"\nFailed to insert unknown header \
                    (P-Early-Media)"));
				sdf_ivk_uaClearTransaction(p_call_obj,
					p_overlap_txn, "INVITE", Sdf_en_uacTransaction);
				sdf_ivk_uaFreeCallObject(&p_call_obj);
					ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, memfree_retval)
                return ICF_FAILURE;            
		   }
        }
      
        if (ICF_NULL == p_ssa_pdb->p_glb_pdb->p_call_ctx) 
        {
           return ICF_FAILURE; 
        }
        p_ssa_pdb->p_ssa_ctxt->call_type = p_ssa_pdb->p_glb_pdb->\
            p_call_ctx->call_type;
        /* Form SDP message body */
        if(ICF_FAILURE == 
            icf_ssa_form_sdp_from_stream_sdp_info( 
            &p_sdp_msg, 
            p_ssa_pdb,
            ICF_SSA_NO_HOLD,
            &sdf_error))
        {
            /*Raise a Major error here - UATK_PROCESSING FAILURE*/
            sdf_ivk_uaClearTransaction(p_call_obj,
                p_overlap_txn, "INVITE",
                Sdf_en_uacTransaction);
            sdf_ivk_uaFreeCallObject(&p_call_obj);
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
                ret_val = ICF_FAILURE;
        }

        /*Message body header changes Start*/
	/*Set the count to the number of message bodies in the request
		 */
	count = (icf_uint8_t)p_ssa_pdb->p_glb_pdb->p_call_ctx->body_list.count;
    if(0 == count)
	{
		Sdf_mc_strdup(p_content_type,(icf_uint8_t *)"application/sdp");
	}
	else
	{
	 if(p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->body_list\
						 .content_type.str_len != 0)
	 {
		Sdf_mc_strdup(p_content_type,(icf_uint8_t *)p_ssa_pdb->p_ssa_ctxt->\
			p_call_ctxt->body_list.content_type.str);

	 }
	}
        if (ICF_NULL == p_content_type)
        {
            ret_val = ICF_FAILURE;
        } 
	 /*Message body header changes Ends*/

        /* Set SDP in Outgoing Transaction */
        if((ICF_FAILURE == ret_val) || (ICF_NULL == p_ssa_ctxt))
        {
            ret_val = ICF_FAILURE;
        }
        else if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
            p_call_obj->pUacTransaction, p_sdp_msg, 
            &sdf_error))
        {
            sip_freeSdpMessage(p_sdp_msg);
            sdf_ivk_uaClearTransaction(p_call_obj,
                p_overlap_txn,"INVITE",
                Sdf_en_uacTransaction);
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            /*Raise a Major error here - UATK_PROCESSING FAILURE*/
            ret_val = ICF_FAILURE;
        }
        /*The above API added the SDP msg body in the msg body list
        * of the transaction. Now we need to add this to the 
        * actual outgoing SIP Message*/
        
        else if (ICF_FAILURE == icf_insert_header_tag_body_list(
            p_ssa_pdb, ICF_TRUE,
            p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction, ICF_NULL))
        {
            
            sdf_ivk_uaClearTransaction(p_call_obj,
                p_overlap_txn, 
                "INVITE", Sdf_en_uacTransaction);
            sdf_ivk_uaFreeCallObject(&p_call_obj);
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, memfree_retval)
                return ICF_FAILURE;
        }
	/*Message body header changes Start*/
        
        else if ( ICF_FAILURE == icf_ssa_SetAllMsgBody(p_call_obj->
				 pUacTransaction,Sdf_co_null,/*OverlapTxn*/\
                                 (icf_int8_t *)p_content_type, &sdf_error))
		{
            sip_freeSdpMessage(p_sdp_msg);
            sdf_ivk_uaClearTransaction(p_call_obj,
                p_overlap_txn,"INVITE",
                Sdf_en_uacTransaction);
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, p_ecode, ret_val)
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            if(ICF_NULL != p_content_type)
				sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
				    (p_content_type),&sdf_error);
	    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
            ret_val = ICF_FAILURE;
        }
	else if ( Sdf_co_fail == sdf_listInit(
						&(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
						__sdf_fn_uaFreeMsgBody,Sdf_co_false, &sdf_error))
	{
		sip_freeSdpMessage(p_sdp_msg);
		sdf_ivk_uaClearTransaction(p_call_obj,
			p_overlap_txn,"INVITE",
			Sdf_en_uacTransaction);
		ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
			ICF_MEM_COMMON, p_ecode, ret_val)
			sdf_ivk_uaFreeCallObject(&p_call_obj);
		if(ICF_NULL != p_content_type)
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
				(p_content_type),&sdf_error);
		/*Raise a Major error here - UATK_PROCESSING FAILURE*/
		ret_val = ICF_FAILURE;
	}
	else if ((ICF_NULL == p_ssa_ctxt) ||
               (Sdf_co_fail == sdf_ivk_uaCopyMsgBodyToPersistent (
				&(p_ssa_ctxt->p_call_obj->pUacTransaction->slMsgBodyList),
				&(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
				(Sdf_ty_s8bit *)"persistent",&sdf_error)))
	{
		sip_freeSdpMessage(p_sdp_msg);
		sdf_ivk_uaClearTransaction(p_call_obj,
			p_overlap_txn,"INVITE",
			Sdf_en_uacTransaction);
		ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
			ICF_MEM_COMMON, p_ecode, ret_val)
		sdf_ivk_uaFreeCallObject(&p_call_obj);
		if(ICF_NULL != p_content_type)
		sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
				(p_content_type),&sdf_error);
		/*Raise a Major error here - UATK_PROCESSING FAILURE*/
		ret_val = ICF_FAILURE;
	}
        /*Message body header changes Ends*/
        else/*Now we send the sip message to peer*/
        {
            sip_freeSdpMessage(p_sdp_msg);
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            
            p_call_obj->pAppData->pData = (Sdf_ty_pvoid)p_ssa_ctxt;
            /* We are adding the nw_call_id - ssa ctxt map here, 
            * after all the other processing is over successfully.
            * This call should succeed as we check for space in the 
            * map while we allocate a call object prior to doing 
            * anything else ini this routine*/
            icf_ssa_add_callid_callobj_map(p_ssa_pdb,                
                p_ssa_ctxt,
                p_ecode);
            
			/*Fix for CSR 1-8081459*/
			p_ssa_ctxt->line_id = p_ssa_ctxt->p_call_ctxt->line_id;

            if((ICF_TRANSPORT_TYPE_TCP == 
                p_ssa_ctxt->transport) ||
                (ICF_TRANSPORT_TYPE_TLS == 
                p_ssa_ctxt->transport) || 
                (ICF_TRANSPORT_MODE_BOTH == 
                p_ssa_ctxt->transport))
	        {
		        ICF_SSA_SET_COMMON_BITMASK(p_ssa_pdb->p_ssa_ctxt->bitmask,
				    ICF_SSA_EXPLICIT_CONNECT_REQ);
    		    if (ICF_TRANSPORT_TYPE_TLS == 
	        	    p_ssa_ctxt->transport)
		        {
			        p_ssa_ctxt->bitmask |= ICF_SSA_TLS_TRANSPORT;
		        }
		        else
		        {
			        p_ssa_ctxt->bitmask |= ICF_SSA_TCP_TRANSPORT;
		        }
	        }
            
            if ( ICF_FAILURE == ret_val)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInTCPConnEstablishment, returning"));
                sdf_ivk_uaClearTransaction(p_call_obj, 
                    p_overlap_txn,"INVITE", 
                    Sdf_en_uacTransaction);
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
            /*Send INVITE to peer */
            else 
            { 
               if( ICF_FAILURE == 
                  icf_ssa_send_to_peer(p_ssa_pdb,p_call_obj,
                  p_call_obj->pUacTransaction->pSipMsg,
                  &options,&sdf_error))
               {
                    /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
                    sdf_ivk_uaClearTransaction(p_call_obj, 
                        p_overlap_txn,"INVITE", 
                        Sdf_en_uacTransaction);
                    sdf_ivk_uaFreeCallObject(&p_call_obj);
                    /*Not freeing the SSA ctxt memory explicitly as
                    * that will now have been freed through 
                    * FreeAppData cbk invoked through FreeCallObj*/
                    ret_val = ICF_FAILURE;
                }
                else if ( Sdf_co_fail ==
                    sdf_ivk_uaEndTransaction(p_call_obj,
                    Sdf_co_null, "INVITE", &sdf_error))
                {
                    /* Raise/Set Error: UATK_PROCESSING_FAILURE*/
                    sdf_ivk_uaClearTransaction(p_call_obj, 
                        p_overlap_txn, "INVITE", 
                        Sdf_en_uacTransaction);
                    sdf_ivk_uaFreeCallObject(&p_call_obj);
                    /*Not freeing the SSA ctxt memory explicitly as
                    * that will now have been freed through 
                    * FreeAppData cbk invoked through FreeCallObj*/
                    ret_val = ICF_FAILURE;
                }
                
               /* Stats collection of OUTBOUND INVITE requests*/
               if(ret_val == ICF_SUCCESS)
               {

                   ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                           stat_requests.stat_outbound_req.stats_INVITE);
               }

            }        
       }/*EndOfProcessing*/
    }/*End of else for line data retreival*/

     /* FMR fix under SPR 11096
     */
    if(ICF_NULL != p_content_type)
    {
	sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
	    (p_content_type),&sdf_error);
    }
    if (ICF_FAILURE == ret_val)
    {
        /* Set SSA ctxt as NULL in call context */
        p_call_ctxt->p_sip_ctx = ICF_NULL;
        if (p_call_obj!= ICF_NULL)
            sdf_ivk_uaFreeCallObject(&p_call_obj);
    }
   
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_setup_resp
 **
 ** DESCRIPTION:        This function sends the call setup response
 **                     (100 TRYING) to peer
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_call_setup_resp(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    Sdf_st_error            error;
    SipOptions              options;	

     /* new added code */
     Sdf_ty_s8bit                    from_user_addr[ICF_MAX_STR_LEN] = "\0";
     icf_config_data_st           *p_glb_cfg_data = p_ssa_pdb->p_glb_cfg_data;
     /* new added code ends here*/

    icf_ssa_ctxt_st *p_ssa_ctxt = 
		(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;
    icf_address_st		*p_user_address = ICF_NULL;
    Sdf_ty_s8bit         contact_addr[ICF_MAX_STR_LEN] = "\0",
    display_name[ICF_MAX_STR_LEN] = "\0",
    tpt_scheme[ICF_MAX_STR_LEN] = "\0";
    icf_transport_address_st	*p_contact_address = ICF_NULL;
    Sdf_ty_s8bit			*p_uname= ICF_NULL;
    Sdf_ty_s8bit			temp_uaddr[ICF_MAX_STR_LEN]="\0";
    icf_line_data_st        *p_line_data = ICF_NULL;
    icf_transport_address_st self_addr ;
    Sdf_st_error          sdf_error;

#ifdef ICF_NAT_RPORT_SUPPORT
    icf_rport_config_data_st            *p_rport_config_data = ICF_NULL;
#endif

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
    ICF_SSA_INIT_TRANS_ADDR(&self_addr)    
	/*Now add the contact header in the profile.  This is required since
	  CC must have identified the line for the call so this is the first time in 
	  incoming call flow, that SSA is aware of the local address for which the
	  call is being processed*/
    /* 
       We will always use the local address(filled from Request URI) to
      form the username part of the outgoing contact header
     */
    p_user_address = &p_ssa_pdb->p_glb_pdb->p_call_ctx->local_address;
    if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                p_ssa_pdb->p_glb_pdb, p_ssa_ctxt->p_call_ctxt->line_id,
                    &p_line_data, p_ssa_pdb->p_ecode))
    {
         return ICF_FAILURE;
    }
    else
    {
       /*If per line self transport then copy address from line_data*/
       if (ICF_CFG_SELF_ADDR_PER_LINE == p_glb_cfg_data->self_addr_per_line)
       {
           icf_port_memcpy(&self_addr, &(p_line_data->self_addr),
                           sizeof(icf_transport_address_st));

#ifdef ICF_NAT_RPORT_SUPPORT
           p_rport_config_data = p_line_data->p_rport_config_data;
#endif
       }
       else
       {
           icf_port_memcpy(&self_addr, &(p_glb_cfg_data->self_ip_address),
                           sizeof(icf_transport_address_st));
#ifdef ICF_NAT_RPORT_SUPPORT
           p_rport_config_data = &(p_glb_cfg_data->rport_config_data); 
#endif
       }
    }
	
	/*Fix for CSR 1-8081459*/
	p_ssa_ctxt->line_id = p_ssa_ctxt->p_call_ctxt->line_id;

#ifdef ICF_NAT_RPORT_SUPPORT
    
	if(ICF_TRUE == p_rport_config_data->is_rport_reqd) /* problem it is not crm_pdb */
	{
		p_contact_address = &(p_rport_config_data->contact_ip_addr);
	}
	else
	{
		p_contact_address = &self_addr;
	}
#else
    p_contact_address = &self_addr;

#endif
	if ( ICF_LOCAL_NAME_PRESENT &
			p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)
	{
		icf_port_strcpy((icf_uint8_t *)display_name, 
				(icf_uint8_t *)p_ssa_pdb->p_glb_pdb->p_call_ctx->local_name.str);
	}
	else
	{
		icf_port_strcpy((icf_uint8_t *)display_name,(icf_uint8_t *) "");
	}

	/*Addressing scheme for contact in 200 OK to invite will
	only be SIP*/
    if (ICF_ADDRESS_PLAN_SIPS_URI == p_user_address->addr_type)
	{
		icf_port_strcpy((icf_uint8_t *)tpt_scheme, (icf_uint8_t *)"sips");
	}
	else
	{
		icf_port_strcpy((icf_uint8_t *)tpt_scheme, (icf_uint8_t *)"sip");
	}
	/*
     * We copy the complete user address string from the local
     * address and then check if it is a E164, if it is then 
     * we append the self user address from global config data
     * to the string to get complete userid@hostname string
     */

	/*remove the '+' from TEL address if presetn*/
	if ((ICF_ADDRESS_PLAN_TEL_URI ==
            p_user_address->addr_type) &&
			('+' == p_user_address->addr_val.str[0]))
	{
		    icf_port_strcpy((icf_uint8_t *)from_user_addr,
            &(p_user_address->addr_val.str[1]));
	}
	else
	{
        icf_port_strcpy((icf_uint8_t *)from_user_addr,
                (icf_uint8_t *)p_user_address->addr_val.str);
    }
    icf_port_strcpy((icf_uint8_t *)temp_uaddr,(icf_uint8_t *)from_user_addr);	
    p_uname = icf_port_strtok(temp_uaddr,"@");

    if(ICF_NULL != p_uname)
    {   
        icf_port_strcpy((icf_uint8_t *)contact_addr,(icf_uint8_t *)p_uname);
        icf_port_strcpy((icf_uint8_t *)temp_uaddr,(icf_uint8_t *)"\0");
        p_uname = ICF_NULL;
    }

	/* ------------------------------------------------- */
	/* The userId in Contact header should be the userid configured through 
     * set_line_params api. If this is not configured, then the userid part
     * of From header should be used.
	 */

    if (ICF_NULL != p_ssa_pdb->p_line_data->contact_user_info.str_len)
    {
		contact_addr[0] = '\0';
        icf_port_strcpy((icf_uint8_t *)contact_addr, 
            (icf_uint8_t *)p_ssa_pdb->p_line_data->contact_user_info.str);
    }
	icf_port_strcat((icf_uint8_t *)contact_addr, (icf_uint8_t *)"@");
    /* ------------------------------------------------- */

    if ( (ICF_ADDRESS_PLAN_TEL_URI ==
            p_user_address->addr_type) ||
			(ICF_ADDRESS_PLAN_E164 ==
            p_user_address->addr_type))
    {
        icf_port_strcat((icf_uint8_t *)from_user_addr,(icf_uint8_t *)"@");

        if ( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                self_addr.addr.addr_type)
        {
            icf_port_strcat((icf_uint8_t *)from_user_addr,
                    (icf_uint8_t *)self_addr.\
                    addr.addr.domain.str);
	    icf_port_strcat((icf_uint8_t *)contact_addr,(icf_uint8_t *)self_addr.addr.addr.domain.str);
        }
        else/*Assumption that it will be IPV4*/
        {
            icf_uint8_t remote_ip[16];
            icf_uint8_t contact_ip[16];

            if ( ICF_FAILURE == icf_ssa_convert_to_IP_addr(
                        &(self_addr), remote_ip))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                 
		icf_port_strcat((icf_uint8_t *)from_user_addr, (icf_uint8_t *)remote_ip);
		if ( ICF_FAILURE == icf_ssa_convert_to_IP_addr(
					p_contact_address, contact_ip))
		{
			ret_val = ICF_FAILURE;
		}   
		else
		{
			icf_port_strcat((icf_uint8_t *)contact_addr, (icf_uint8_t *)contact_ip);
		}

            }
        }
    }
    /*
     * If the address type is SIP URI ,then we will
     * extract the userid from the SIP URI and then 
     * append the self IP address and port in order
     * to form the contact header
     */

    if (( ICF_ADDRESS_PLAN_SIP_URI ==
			    p_user_address->addr_type) ||
		    ( ICF_ADDRESS_PLAN_SIPS_URI ==
		      p_user_address->addr_type))
    {
        if ( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME ==
                self_addr.addr.addr_type)
        {
            icf_port_strcat((icf_uint8_t *)contact_addr,
                    (icf_uint8_t *)self_addr.\
                    addr.addr.domain.str);
        }
        else/*Assumption that it will be IPV4*/
        {
            icf_uint8_t remote_ip[16];
	    icf_uint8_t contact_ip[16];

            if ( ICF_FAILURE == icf_ssa_convert_to_IP_addr(
                        &(self_addr), remote_ip))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                 /*icf_port_strcpy(contact_addr,from_user_addr);*/
		icf_port_strcat((icf_uint8_t *)from_user_addr, (icf_uint8_t *)remote_ip);
		if ( ICF_FAILURE == icf_ssa_convert_to_IP_addr(
					p_contact_address, contact_ip))
		{
			ret_val = ICF_FAILURE;
		}   
		else
		{
			icf_port_strcat((icf_uint8_t *)contact_addr, (icf_uint8_t *)contact_ip);
		}
            }
        }
    }
	if ( Sdf_co_fail == sdf_listDeleteAll(
				&(p_ssa_ctxt->p_call_obj->pInitData->slContact),
				&error))
	{
		ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in Deleting existing contacts from profile"));
		ret_val = ICF_FAILURE;
	}
	else if (Sdf_co_fail == sdf_ivk_uaAddDefaultContact(
				p_ssa_ctxt->p_call_obj->pInitData,
				display_name,
				contact_addr,
				/*p_user_address->addr_val.str,*/
				(icf_uint16_t)((ICF_ADDRESS_PLAN_SIPS_URI == p_user_address->addr_type)?
				p_ssa_pdb->p_glb_cfg_data->p_tls_init_data->tls_sip_serv_port_num:
				p_contact_address->port_num),
				tpt_scheme,
				&error))
	{

		ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in adding DefaultContact"));
		ret_val = ICF_FAILURE;
	}
    else if (ICF_FAILURE ==
            icf_ssa_add_transport_param_in_contact_hdr(
                p_ssa_pdb,
                p_ssa_ctxt->p_call_obj->pInitData,
                &sdf_error))

    {
        ret_val = ICF_FAILURE;
    }
    else
    {
#ifdef ICF_SIGCOMP_SUPPORT
       /* Check The Config data whether we are supporting 
        * sigcomp or not,If Yes then add comp=sigcomp param
        * in the contact header
        */
        if(ICF_TRUE == 
            p_ssa_pdb->p_glb_cfg_data->sigcomp_required)
        {
            if(ICF_FAILURE == 
                icf_ssa_AddParamToContactHdrList
                (&(p_ssa_ctxt->p_call_obj->pInitData->slContact),
                "comp=sigcomp",
                &error))
            {
                /*Raise/Set alarm here for UATK_PROCESSING_FAILURE
                 * major ERROR*/
                ret_val = ICF_FAILURE;
            }
            
        }
        
#else
        ret_val = ICF_SUCCESS;
#endif
    }
    
    if(ret_val == ICF_FAILURE)
    {
    /*Raise/Set alarm here for UATK_PROCESSING_FAILURE
        * major ERROR*/
        
    }
    /* Send 100 trying response to peer */
    else if (Sdf_co_fail == sdf_ivk_uaFormResponse((Sdf_ty_u16bit)100,
        (const Sdf_ty_s8bit *)"INVITE", 
				p_ssa_ctxt->p_call_obj,Sdf_co_null,Sdf_co_false, &error))
	{
		ret_val = ICF_FAILURE;
	}
    else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
	else
	{
        /* ICF will copy the Timestamp header received in the request into 
         * 100 Trying but it will not compute the delta/delay time
         */
        /* In case the incoming request contains Timestamp header, it should
         * be sent in only 100 Trying and not in other responses. So delete 
         * the Timestamp header from processed header list now.
         */
        icf_uint32_t            ts_size = ICF_NULL;
        /*Sdf_ty_u32bit           ts_size = Sdf_co_null;*/

        /* Extract Timestamp Header */
        if (Sdf_co_fail == sdf_listSizeOf(
              &(p_ssa_ctxt->p_call_obj->pUasTransaction->slProcessedHeaders),
              &ts_size,
              &error))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Failed to get count of ProcessedHeaderList"));
            ret_val = ICF_FAILURE;
        }
        /* Timestamp Header present */
        else if (ICF_NULL < ts_size)
        {
            SipHeader           *pHeader = Sdf_co_null;
            icf_uint32_t       index = ICF_NULL;
            
            for (index = 0; index < ts_size; index++)
            {
                if (Sdf_co_fail == sdf_listGetAt(
                                    &(p_ssa_ctxt->p_call_obj->pUasTransaction->\
                                        slProcessedHeaders),
                                    index,
                                    (Sdf_ty_pvoid *)&pHeader,
                                    &error))
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Failed to get header from list"));
                    ret_val = Sdf_co_fail;
                }
                else if (SipHdrTypeTimestamp == pHeader->dType)
                {
                    if (Sdf_co_fail == sdf_listDeleteAt(
                                    &(p_ssa_ctxt->p_call_obj->pUasTransaction->\
                                        slProcessedHeaders),
                                    index,
                                    &error))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Failed to delete Timestamp" \
                                    " header from ProcessedHeaderList"));
                        ret_val = Sdf_co_fail;
                    }
                }
            } /* for (index) */
        } /* else if (ts_size) */
    } /* delete Timestamp header from processed header list */

    if (ICF_SUCCESS == ret_val)
	{
		/* Specify the stack options */
		options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
		/*  (0 for UDP, 1 for TCP) */
		if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,p_ssa_ctxt->p_call_obj,
					p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
					&options,&error))
		{
			ret_val = ICF_FAILURE;
		}

	} /* else */
#ifdef ICF_STAT_ENABLE
     if (ICF_SUCCESS == ret_val)
     {
         icf_ssa_invite_resp_stats(p_ssa_pdb,100,1);
     }
#endif

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
	return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_connect
 **
 ** DESCRIPTION:        This function sends the 200 OK for the INVITE
 **                     indicating that the call has been accepted.
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_connect(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;

    icf_glb_pdb_st           *p_glb_pdb =
        p_ssa_pdb->p_glb_pdb;

    icf_call_ctx_st          *p_call_ctxt =
        p_glb_pdb->p_call_ctx;

    icf_ssa_ctxt_st *p_ssa_ctxt = 
        (icf_ssa_ctxt_st *)p_call_ctxt->p_sip_ctx;

    icf_nw_otg_connect_st    *p_otg_connect = 
        (icf_nw_otg_connect_st*)(p_ssa_pdb->\
                                    p_internal_msg->p_msg_data);

	Sdf_st_callObject       *p_call_obj = Sdf_co_null;

    icf_error_t ecode = ICF_ERROR_NONE;
    icf_return_t  memfree_retval;

    Sdf_st_error            error;
    SipOptions              options;
    SdpMessage              *p_SdpMsg = Sdf_co_null;
    icf_int8_t             *p_content_type = ICF_NULL;
    icf_uint8_t             count = 0;
  

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if(ICF_NULL == p_ssa_ctxt)
    {
	return ICF_FAILURE;
    }
    else
    {
	p_call_obj = p_ssa_ctxt->p_call_obj;
    }

    /* 
     * Flush the message bodies (if any) stored in the 
     * UAS transaction of the call object
     */
#ifdef ICF_SESSION_TIMER 
    if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    { 
        icf_ssa_session_timer_set(p_ssa_pdb);
    }
    else
    {
        if(Sdf_co_null != p_call_obj->pSessionTimer)
        {
             if(Sdf_en_refresherInvalid != p_call_obj->pSessionTimer->dRefreshRequest)
             {
                   p_call_obj->pSessionTimer->dRefreshRequest = Sdf_en_refresherInvalid;
             }
        }
     }
#endif

    sdf_listDeleteAll(&(p_ssa_ctxt->p_call_obj->pUasTransaction->slMsgBodyList),
                &error);

    /* Make the 200 OK response now */
    if (Sdf_co_fail == sdf_ivk_uaFormResponse(200,"INVITE", 
                p_ssa_ctxt->p_call_obj, Sdf_co_null,
                Sdf_co_false, &error))
    {
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
#ifdef ICF_SESSION_TIMER 
	   /* icf_ssa_chk_n_insert_timer_in_supp_hdr(p_ssa_pdb); */
#endif
          if ( ICF_FAILURE == icf_ssa_form_and_insert_access_nw_info_hdr(
			p_ssa_pdb, p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
	  {
                    sdf_ivk_uaFreeCallObject(&p_call_obj);
                    ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                            ICF_MEM_COMMON, &ecode, memfree_retval)
                        return ICF_FAILURE;
	  }
          else if (ICF_TRUE == p_call_ctxt->send_privacy_hdr) 
          {
                if ( ICF_FAILURE == icf_ssa_form_and_insert_priv_hdr(
                    p_ssa_pdb, p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
                {
                    sdf_ivk_uaFreeCallObject(&p_call_obj);
                    ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                            ICF_MEM_COMMON, &ecode, memfree_retval)
                        return ICF_FAILURE;
                }
                else
                {
                    p_call_ctxt->send_privacy_hdr = ICF_FALSE;
                }

            }


          

        /* Check whether media(SDP) has to be sent or not */
        if(ICF_NULL == (ICF_NW_OTG_CONNECT_WITHOUT_SDP &
                    p_otg_connect->bitmask))
        {
            /*
             * SSA is not using any media handling related
             * functionality provided by the UATK.
             * Whenever SDP needs to be sent in a message,
             * SSA picks up the local SDP stored in the call 
             * context and forms an SDP message body from it.
             * This SDP body is then set into the UAS/UAC/Overlap
             * transaction of the call object
             */
            /* Form SDP message body */ 
            
            if(ICF_CALL_TYPE_INVALID == p_ssa_pdb->p_ssa_ctxt->call_type)
            {
                p_ssa_pdb->p_ssa_ctxt->call_type = p_ssa_pdb->p_glb_pdb->\
                p_call_ctx->call_type;
            }

        
            if(ICF_FAILURE == icf_ssa_form_sdp_from_stream_sdp_info( 
                        &p_SdpMsg, 
                        p_ssa_pdb,
                        ICF_SSA_NO_HOLD,
                        &error))
            {
                ret_val = ICF_FAILURE;
            }

			if ( ICF_FAILURE != ret_val )
			{
				
				/*Message body header changes Start*/
				count = (icf_uint8_t )p_ssa_pdb->p_glb_pdb->p_call_ctx->\
					body_list.count;
				/*Message body header changes Ends*/
				/* Set SDP in Outgoing Transaction */
				if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
					p_ssa_ctxt->p_call_obj->pUasTransaction,
					p_SdpMsg, &error))
				{
					ret_val = ICF_FAILURE;
				}
				
				
				else if (ICF_FAILURE == icf_insert_header_tag_body_list(
					p_ssa_pdb, ICF_TRUE,
					p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUasTransaction, 
					ICF_NULL))
				{
					
					sdf_ivk_uaFreeCallObject(&p_call_obj);
					ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
						ICF_MEM_COMMON,&ecode, memfree_retval)
						return ICF_FAILURE;
				}
			}


        }


        if ( ICF_FAILURE != ret_val )
        {
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            /*Message body header changes Start*/
	    if(0 == count)
  	    {
                 Sdf_mc_strdup(p_content_type,(icf_uint8_t *)"application/sdp");
	    }
	    else
	    {
	       if(p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->body_list\
						 .content_type.str_len != 0)
	         {
				 /*Fix done for SPR-18722 : CSR-1-6068965*/
				 Sdf_mc_strdup(p_content_type,(icf_uint8_t *)p_ssa_pdb->p_ssa_ctxt->\
						 p_call_ctxt->body_list.content_type.str);

	         }
	    }
            if (ICF_NULL == p_content_type)
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_SetAllMsgBody(\
                        p_ssa_ctxt->p_call_obj->pUasTransaction,\
                        Sdf_co_null,/*OverlapTxn*/(icf_int8_t *)p_content_type,
                         &error))
            {
                /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
                sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
                  (p_content_type),&error);
	        ret_val = ICF_FAILURE;
            }
	    /*Message body header changes Ends*/
            /* this is needed for sending Allow header in 200 OK */
		    else if(ICF_FAILURE == icf_cfg_fetch_app_conf(
					p_ssa_pdb->p_glb_pdb,
					p_ssa_ctxt->p_call_ctxt->app_id,
					&(p_ssa_pdb->p_app_conf),
					p_ssa_pdb->p_ecode))
		    {
                /*Raise/Set Major Error: UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
		    }  
            /* Specify the stack options */
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        	p_ssa_ctxt->p_call_obj,
                        	p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
                       	 	&options,&error))
            	{
                /*Raise/Set Major Error: UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else
            {
				 /*Fix for CSR 1-8081459*/
				 p_ssa_ctxt->line_id = p_ssa_ctxt->p_call_ctxt->line_id;

                /* Reset if the bitmask for fax call in Incoming Invite was set
                 */
                if(ICF_SSA_CTXT_T38_RCVD_IN_NEW_CALL == (p_ssa_ctxt->bitmask &\
                        ICF_SSA_CTXT_T38_RCVD_IN_NEW_CALL))
                {
                    p_ssa_ctxt->bitmask &=~(ICF_SSA_CTXT_T38_RCVD_IN_NEW_CALL);

                }
            }
    }
    }
#ifdef ICF_STAT_ENABLE
		if(ICF_SUCCESS == ret_val)
        {
            icf_ssa_invite_resp_stats(p_ssa_pdb,200,1);
        }
#endif
    /*Message body header changes Start*/
    if(ICF_NULL != p_content_type)
    {
         sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
                     (p_content_type),&error);
    }
    /*Message body header changes Ends*/
    if(Sdf_co_null != p_SdpMsg)
    {
        sip_freeSdpMessage(p_SdpMsg);
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_reject
 **
 ** DESCRIPTION:        This function is used to reject an incoming call
 **                     the call rejection can be because of various
 **                     reasons.The appropiate error response(3xx/4xx/5xx)
 **                     needs to be sent corresponding to the reason code
 **                     indicated by CC
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_reject(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    Sdf_st_error            error;
    SipOptions              options;
    Sdf_ty_u16bit           response = 0;
    icf_nw_otg_reject_st	*p_otg_reject = ICF_NULL; 
#ifdef ICF_QOS_SUPPORT	
    SdpMessage				*p_SdpMsg = Sdf_co_null;
    icf_int8_t				*p_content_type = ICF_NULL;
#endif
    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* Extract the payload from the internal msg */
    p_otg_reject = (icf_nw_otg_reject_st *)
                        p_ssa_pdb->p_internal_msg->p_msg_data;

    if(ICF_NULL != p_otg_reject->response_code)
     {
       response = p_otg_reject->response_code;
     }
  
    /* 
     * Map the response code 400-699 to send to peer based on the 
     * value of reason_code 
     */
    else if (ICF_FAILURE == icf_ssa_map_cc_reason_code(
                p_otg_reject->reason_code,&response))
    {
        /*Raise/Set Error: MAJOR: UNDEFINED CALL REJECT REASON CODE*/
        ret_val = ICF_FAILURE;
    }

    if(ICF_SUCCESS == ret_val)
    {
        /* Fix for CSR 1-7798174 : SPR 20071 */
        /* If the response is 488, then set the warning header*/	
        if (ICF_CALL_CLEAR_REASON_CODEC_NOT_AVAILABLE == 
                                  p_ssa_ctxt->p_call_ctxt->release_cause)
        {	
            p_ssa_pdb->p_ssa_ctxt->failure_bitmask=\
                                    ICF_SSA_FAILURE_RESP_CODE_488;
            p_ssa_pdb->warning_header_code = 305;
            icf_port_strcpy(p_ssa_pdb->warning_text.str, 
                         "SDP: Incompatible media format: No common codec");
            p_ssa_pdb->warning_text.str_len = 
                         icf_port_strlen(p_ssa_pdb->warning_text.str);
        }
        if (Sdf_co_fail == sdf_ivk_uaFormResponse(response,"INVITE", 
                    p_ssa_ctxt->p_call_obj, Sdf_co_null,
                    Sdf_co_false, &error))
        {
            /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                    p_ssa_pdb,
                    (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                    p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
			if(580 == response)
			{
				Sdf_mc_strdup(p_content_type,"application/sdp");
                                if (ICF_NULL != p_content_type)
                                {
                                   return ICF_FAILURE;
                                }
				if(ICF_FAILURE == icf_ssa_form_sdp_from_stream_sdp_info(
							&p_SdpMsg, 
							p_ssa_pdb,
							ICF_SSA_NO_HOLD,
							&error))
				{
					ret_val = ICF_FAILURE;
				}
				else if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
						p_ssa_ctxt->p_call_obj->pUasTransaction,
						p_SdpMsg, &error))
				{
					ret_val = ICF_FAILURE;
				}
				else if( Sdf_co_fail == icf_ssa_SetAllMsgBody(p_ssa_ctxt->p_call_obj->\
						pUasTransaction,Sdf_co_null,
						(icf_int8_t *)p_content_type, &error))
				{
					sip_freeSdpMessage(p_SdpMsg);
					ret_val = ICF_FAILURE;
				}
	
				if(ICF_NULL != p_content_type)
				{
					sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
								 (p_content_type),&error);
				}
			}
ICF_CHECK_QOS_SUPPORT_END
#endif
            /* Specify the stack options */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_ssa_ctxt->p_call_obj,
                        p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg, 
			&options,&error))
            {
                /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }


        } /* else */
    }
#ifdef ICF_STAT_ENABLE
	if(ret_val != ICF_FAILURE)
	{
    		icf_ssa_invite_resp_stats(p_ssa_pdb,response,1);
	}
#endif
    /* Fix for CSR 1-7798174 : SPR 20071 
     * Reset this bitmask so that IPTK can send ACK for 
     * failure response to CC */
    if (ICF_CALL_CLEAR_REASON_CODEC_NOT_AVAILABLE == 
            p_ssa_ctxt->p_call_ctxt->release_cause)
    {
        p_ssa_pdb->p_ssa_ctxt->failure_bitmask &=
            ~(ICF_SSA_FAILURE_RESP_CODE_488);
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_prov_response
 **
 * DESCRIPTION:        This function is used to send the provisional 18x
 *                     responses to the peer
 *                     1. If the msg_id indicates that prov resp needs to be 
 *                     a reliable one,the same is indicated to UATK which 
 *                     inserts the Require header with 100 rel
 *                     2. The SDP body is formed and populated in the 
 *                     message body list in the UAS/Overlapped transaction
 *                     depending on whether it is an unreliable/reliable
 *                     response respectively.
 *                     3. The message is then sent to peer. 
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_prov_response(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    icf_glb_pdb_st           *p_glb_pdb =
        p_ssa_pdb->p_glb_pdb;

    icf_call_ctx_st          *p_call_ctxt =
        p_glb_pdb->p_call_ctx;

    icf_uint8_t          *p_internal_payload
        = p_ssa_pdb->p_internal_msg->p_msg_data;

    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;

    Sdf_st_callObject           *p_call_obj = Sdf_co_null;
    icf_boolean_t        clear_list = ICF_TRUE;
    icf_boolean_t        reset_send_privacy_header = ICF_FALSE;
    icf_return_t         memfree_retval;
    SipOptions              options;
    Sdf_ty_u16bit           dResponse =0;
    Sdf_ty_bool             rel_prov_resp = Sdf_co_false,
    sdp_to_be_sent = Sdf_co_false;
    SdpMessage *p_SdpMsg = Sdf_co_null;
    Sdf_st_error            error;
    Sdf_st_overlapTransInfo         *p_overlap_txn = Sdf_co_null;
    SipMessage                      *p_sip_msg = Sdf_co_null;
    icf_error_t            Error;
    icf_int8_t            *p_content_type = ICF_NULL;
    icf_uint8_t            count = 0;

/*UMR Fix */
    error.errCode=0;
    error.stkErrCode=0;
    error.appErrCode=0;
    error.ErrMsg[0]='\0';

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

	if(ICF_NULL == p_ssa_ctxt)
	{
		return ICF_FAILURE;
	}

    p_call_obj = p_ssa_ctxt->p_call_obj;

   /* 
    * Flush the message bodies (if any) stored in the 
    * UAS transaction of the call object
    */
    sdf_listDeleteAll(
            &(p_ssa_ctxt->p_call_obj->pUasTransaction->slMsgBodyList),
            &error);
    /* 
     * Extract the appropiate response code corresponding
     * to the internal API issued by CC
     */

    if((ICF_NW_OTG_CALL_ALERT == p_ssa_pdb->p_internal_msg->
                msg_hdr.msg_id))
    {
        dResponse = 180;
        if (ICF_NULL != 
                (ICF_NW_OTG_ALERT_RPR_REQD &
                 (((icf_nw_otg_call_alert_st*)p_internal_payload)
                  ->bitmask)))
        {
            rel_prov_resp = Sdf_co_true;
            p_ssa_ctxt->bitmask |= ICF_SSA_OTG_RPR_ALERT;
        }



        clear_list = ICF_TRUE;
        reset_send_privacy_header = ICF_FALSE;




    } /* ==  p_ssa_pdb->p_internal_msg-... */
    else if (ICF_NW_OTG_ALERT_WITH_MEDIA
            ==  p_ssa_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        dResponse = 180;
        sdp_to_be_sent = Sdf_co_true;
        if (ICF_NULL != 
                (ICF_NW_OTG_ALERT_WITH_MEDIA_RPR_REQD &
                 (((icf_nw_otg_alertmedia_st*)p_internal_payload)
                  ->bitmask)))
        {
            rel_prov_resp = Sdf_co_true;
            p_ssa_ctxt->bitmask |= ICF_SSA_OTG_RPR_ALERT;
        }



        clear_list = ICF_TRUE;
        reset_send_privacy_header = ICF_FALSE;


    }
    else
    {
        dResponse = 183;
        /* check whether 183 should carry SDP or not */
        if (ICF_NULL == (ICF_NW_OTG_MEDIA_PARAM_WITHOUT_SDP &
                 (((icf_nw_otg_media_param_st *)p_internal_payload)->bitmask)))
        {
            sdp_to_be_sent = Sdf_co_true;
        }
        if (ICF_NULL != 
                (ICF_NW_OTG_MEDIA_PARAM_RPR_REQD &
                 (((icf_nw_otg_media_param_st*)p_internal_payload)
                  ->bitmask)))
        {
            rel_prov_resp = Sdf_co_true;
            p_ssa_ctxt->bitmask &= ~(ICF_SSA_OTG_RPR_ALERT);
        }
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Sending provisional response :: "));
        ICF_PRINT(((icf_uint8_t *)"dResponse = %d, sdp_to_be_sent = %d, rel_prov_resp = %d\n",
                    dResponse, sdp_to_be_sent, rel_prov_resp));

        clear_list = ICF_FALSE;
        reset_send_privacy_header = ICF_TRUE;



    }

    if (Sdf_co_true == rel_prov_resp)
    {
        sdf_ivk_uaInitOverlapTransInfo(&p_overlap_txn, &error);
/*        ICF_SSA_TRACE(p_ssa_pdb,ICF_TRACE_OTG_RPR_PRESENT)*/
    }

    if (Sdf_co_fail == sdf_ivk_uaFormResponse((Sdf_ty_u16bit)dResponse,
                (const Sdf_ty_s8bit *)"INVITE", 
                p_ssa_ctxt->p_call_obj, p_overlap_txn,
                rel_prov_resp, &error))
    {
        /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else
    {

	    if (p_overlap_txn == Sdf_co_null)
	    {
		    p_sip_msg = p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg; 
	    }
	    else
	    {
		    p_sip_msg = p_overlap_txn->pSipMsg;
	    }
        
        /* UATK does not add all supported parameter in the message for
        * 1xx response, So call the function directly
        */
        if (Sdf_co_fail == sdf_fn_uaAddSupportedHdrToSipMsg(
            p_sip_msg,
            p_ssa_ctxt->p_call_obj->pInitData, \
            &error))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_ssa_form_and_insert_access_nw_info_hdr(
            p_ssa_pdb, p_sip_msg))
        {
            sdf_ivk_uaClearTransaction(p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                p_overlap_txn, "INVITE", Sdf_en_uasTransaction);
            sdf_ivk_uaFreeCallObject(&p_call_obj);
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, &Error, memfree_retval)
                return ICF_FAILURE;
        }


        if (ICF_TRUE == p_call_ctxt->send_privacy_hdr) 
        {
            if ( ICF_FAILURE == 
                icf_ssa_form_and_insert_priv_hdr(
                p_ssa_pdb, p_sip_msg))
            {
                sdf_ivk_uaClearTransaction(p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                    p_overlap_txn, "INVITE", Sdf_en_uasTransaction);
                sdf_ivk_uaFreeCallObject(&p_call_obj);
                ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                    ICF_MEM_COMMON, &Error, memfree_retval)
                    return ICF_FAILURE;
            }

            if (ICF_TRUE == reset_send_privacy_header)
            {
                    p_call_ctxt->send_privacy_hdr = ICF_TRUE;
            }
            else
            {
                    p_call_ctxt->send_privacy_hdr = ICF_FALSE;
            }
            
        }
        


        /* Check whether SDP needs to be sent in response */
        if( Sdf_co_true == sdp_to_be_sent)
        {
            /*
             * SSA is not using any media handling related
             * functionality provided by the UATK.
             * Whenever SDP needs to be sent in a message,
             * SSA picks up the local SDP stored in the call 
             * context and forms an SDP message body from it.
             * This SDP body is then set into the UAS/UAC/Overlap
             * transaction of the call object
             */

            if(ICF_CALL_TYPE_INVALID == p_ssa_pdb->p_ssa_ctxt->call_type)
            {
                p_ssa_pdb->p_ssa_ctxt->call_type = p_ssa_pdb->p_glb_pdb->\
                p_call_ctx->call_type;
            }

           
            /* Form SDP message body */
            if (ICF_FAILURE == 
                    icf_ssa_form_sdp_from_stream_sdp_info( 
                        &p_SdpMsg,
                        p_ssa_pdb,
                        ICF_SSA_NO_HOLD,
                        &error))
            {
                ret_val = ICF_FAILURE;
            }
            /* Set SDP in Outgoing Transaction */
            else
            {
				if (ICF_FAILURE == ret_val)
                { /*Do nothing*/
                }
                else if (Sdf_co_null != p_overlap_txn)
                {
                    if(Sdf_co_fail == sdf_ivk_uaSetSdpInOverlapTransaction(
                                p_overlap_txn,
                                p_SdpMsg, &error))
                    {
                        sip_freeSdpMessage(p_SdpMsg);
                        ret_val = ICF_FAILURE;
                    }
			        else if (ICF_FAILURE == icf_insert_header_tag_body_list(
			            p_ssa_pdb, clear_list,
			            ICF_NULL,
						p_overlap_txn))
					{
						sdf_ivk_uaClearTransaction(p_call_obj,
							p_overlap_txn, "INVITE", Sdf_en_uasTransaction);
						sdf_ivk_uaFreeCallObject(&p_call_obj);
						ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
							ICF_MEM_COMMON, &Error, memfree_retval)
						return ICF_FAILURE;
					}
        

                }
                else
                {
                    if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
                                p_ssa_ctxt->p_call_obj->pUasTransaction,
                                p_SdpMsg, &error))
                    {
                        sip_freeSdpMessage(p_SdpMsg);
                        ret_val = ICF_FAILURE;
                    }
		    /*Message body header changes Start*/
		    count = (icf_uint8_t)p_ssa_pdb->p_glb_pdb->p_call_ctx->body_list.count;
	            if (ICF_FAILURE == icf_insert_header_tag_body_list(
		            p_ssa_pdb, clear_list,\
  			    p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUasTransaction,\
			    ICF_NULL))
		    {
				sdf_ivk_uaClearTransaction(p_call_obj,
					p_overlap_txn, "INVITE", Sdf_en_uasTransaction);
				sdf_ivk_uaFreeCallObject(&p_call_obj);
				ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
					ICF_MEM_COMMON, &Error, memfree_retval)
				return ICF_FAILURE;
		    }
		    /*Message body header changes Ends*/
                }
            }
        }

        if ( ICF_FAILURE != ret_val)
        {
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            /* If no message body is present then set the content-type as 
	     * application/sdp
	     */
	    if(0 == count)
	    {
		    Sdf_mc_strdup(p_content_type,(icf_uint8_t *)"application/sdp");
	    }
            if (ICF_NULL == p_content_type)
            {
                ret_val = ICF_FAILURE;
            } 
            else if ( Sdf_co_fail == 
                    icf_ssa_SetAllMsgBody(p_ssa_ctxt->p_call_obj->\
			    pUasTransaction,p_overlap_txn,/*OverlapTxn*/\
			    (icf_int8_t *)p_content_type, &error))
            {
                sip_freeSdpMessage(p_SdpMsg);
                if(ICF_NULL != p_content_type)
		{
			sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
                         (p_content_type),&error);
		}
                
		/*Raise/Set Error: UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else if(ICF_EARLY_MEDIA_HDR_SUPPORTED &
                              p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask_2)
            {
               if(ICF_FAILURE == icf_ssa_form_and_insert_p_early_media_hdr(
                                            p_ssa_pdb,
                                            p_sip_msg))
               {
                   ret_val = ICF_FAILURE;
               }
            }
 
            /* Specify the stack options */
            if (ICF_FAILURE == ret_val)
            {
                /* no need to do anything here */
                ret_val = ret_val;
            }
            else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                        p_ssa_pdb,
                        (icf_uint8_t *)"User-Agent", 
                        p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                        p_sip_msg))
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
        	                p_ssa_ctxt->p_call_obj,
                	        p_sip_msg,
                        	&options,&error))
            		{
                		sip_freeSdpMessage(p_SdpMsg);
                		ret_val = ICF_FAILURE;
            		}
            else
            {
                sip_freeSdpMessage(p_SdpMsg);
            }
        }
    }
#ifdef ICF_STAT_ENABLE
	if(ret_val == ICF_SUCCESS)
	{
		icf_ssa_invite_resp_stats(p_ssa_pdb,dResponse,1);
	}
#endif
    /*Message body header changes Start*/
     if(ICF_NULL != p_content_type)
    {
         sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&\
            (p_content_type),&error);
    }
    /*Message body header changes Ends*/
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_ack
 **
 * DESCRIPTION:        This function is used to send the acknowledgement
 *                     (ACK) messages to peer.
 *                     1.Form the ACK request and send it to peer
 *                     2. ACK messages are only sent for INVITE and
 *                     RE-INVITE's.In the current implementation,as all 
 *                     outgoing INVITE's will contain SDP,hence there is no
 *                     scenarion in which SDP needs to be sent in the ACK.
 *                     Hence,the ACK is always sent without the SDP. 
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_ack(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    SipOptions              options = {0};
    Sdf_st_error            error = {0,0,0,"\0"};

    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;
    /*icf_port_memset(&options, 0 , sizeof(SipOptions));
    icf_port_memset(&error, 0 , sizeof(Sdf_st_error));*/
#ifdef ICF_SESSION_TIMER
     if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
    {	
      if(p_ssa_ctxt->p_call_obj->pMediaInfo != ICF_NULL)
	  {
	  	p_ssa_ctxt->p_call_obj->pMediaInfo->dMediaState = Sdf_en_idleMedia;
	  }
    }
#endif
#ifdef ICF_IPSEC_ENABLED
	/* ACK may go on a differnet SA from Invite. Since it is possible that
	 * the old SA gets deleted following the arrival of 200 OK, we need to once
	 * again check if there is a new SA. In which case ACK will be sent on the 
	 * new SA. Again we need to update the SA ctxt pointers in the SSA ctxt and
	 * the SSA pdb data structures.
	 */
	/* fetch the SA context node and corresponding REGM context node and save 
	 * them in the SSSA context and the SSA pdb
	 */
	if (p_ssa_pdb->p_glb_cfg_data->ipsec_enabled)
	{
        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
		ret_val = icf_ssa_set_sa_ctxt_for_otg_req(p_ssa_pdb,
			p_ssa_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity) ;
	}
#endif
    /* 
     * Flush the message bodies (if any) stored in the 
     * UAC transaction of the call object
     */
    sdf_listDeleteAll(
            &(p_ssa_ctxt->p_call_obj->pUacTransaction->slMsgBodyList),&error);
    /*
     * Form an ACK request
     */
#if 0
	/* this might not be required, ACK may pick headers from Invite */
	/* need to insert the Sec-Verify header in the ACK */
#ifdef ICF_IPSEC_ENABLED
	/* insert the security verify header in the outgoing protected
	 * message if an SA exists 
	 */
	if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
		/* if SA exits */
		(ICF_NULL != p_ssa_pdb->p_sa_context))
	{
		/* insert security-verify header in the otg BYE */
        if (ICF_FAILURE == icf_ssa_get_sec_verify_string(
            p_ssa_pdb,
            &p_value,
            p_ssa_pdb->p_sa_context))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE ==     
		  icf_ssa_insert_unknown_hdr_in_sip_msg (
			  p_ssa_pdb,
			  icf_uint8_t*)"Security-Verify",
			  p_value, 
			  p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
		{
			ICF_PRINT(("\nFailed to insert unknown header \
					(Security-Verify)"));
			ret_val = ICF_FAILURE;
		}
		if (ICF_NULL != p_value)
		{
			ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
				p_value,
				ICF_MEM_COMMON,
				p_ssa_pdb->p_ecode,
				ret_val)
		}
		if (ICF_SUCCESS == ret_val )
		{
			ICF_PRINT(("\nSecurity Verify Header inserted to \
							outgoing ACK "));
		}
	}
#endif
#endif
    if (Sdf_co_fail == 
            sdf_ivk_uaFormRequest((Sdf_ty_s8bit *)"ACK",p_ssa_ctxt->p_call_obj, 
                Sdf_co_null, &error))
    {
        /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Specify the stack options */
        options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_ssa_ctxt->p_call_obj,
                    p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg,
                    &options,&error))
        {
            /*Raise/Set Error: UATK_PROCESSING_FAILURE */
            ret_val = ICF_FAILURE;
        }

        /* Stats collection of OUTBOund ACK */
        else
        {
            ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->stat_requests.
                    stat_outbound_req.stats_ACK);
        }
    }

    /* Delete all ssa call objects other than one for which
     * 200 Ok is recived */
    icf_ssa_del_all_other_call_obj(p_ssa_pdb);

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_terminate
 **
 ** DESCRIPTION:        This function sends the BYE message to peer
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_call_terminate(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    Sdf_st_error            error;
    SipOptions              options;
    Sdf_st_overlapTransInfo *p_overlaap_txn = Sdf_co_null;
#ifdef ICF_IPSEC_ENABLED
	icf_uint8_t			*p_value = ICF_NULL;
#endif

    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* 
     * If SSA ctxt is not present,just return SUCCESS.
     * It is possible that in some erroneous condition,SSA
     * gets a trigger to send BYE on network when it has already
     * cleared its context.
     */
    if (ICF_NULL == p_ssa_ctxt)
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }
    /*
     * Check if OPTIONS is ongoing.If yes,invoke
     * the utility function to send the failure response 
     * to peer as well as clear the OPTIONS the context
     */
    if (ICF_NULL != ( p_ssa_ctxt->bitmask & ICF_SSA_OPTIONS_ONGOING ))
    {
        /* Not checking ret_val as this is best effort */         
        icf_ssa_clear_options_context(p_ssa_pdb);
    }

#ifdef ICF_IPSEC_ENABLED
	if (p_ssa_pdb->p_glb_cfg_data->ipsec_enabled)
		/* if SA exists */
	{
		/* fetch the new SA context for sending out this new transaction. It is
		 * possible that the SA on which previous transactions of this call were
		 * sent now no more exists 
		 */
		/* save the SA context and REGM context pointers to SSA PDB */
		ret_val = icf_ssa_set_sa_ctxt_for_otg_req(p_ssa_pdb,
			p_ssa_pdb->p_glb_pdb->p_call_ctx->p_preferred_identity) ;
	}
#endif
    /* Start a new transaction for sending the BYE */
    if (Sdf_co_fail == sdf_ivk_uaStartTransaction(
                p_ssa_ctxt->p_call_obj,&p_overlaap_txn,"BYE", &error))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else
    {
#if 0
        /*Remove authentication parameters from BYE request*/
		Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null;		
		
		if(p_ssa_ctxt->p_call_obj != Sdf_co_null)
		{
			pProcessedHeaderList = &(p_ssa_ctxt->p_call_obj->slPersistentHeaders);			
			
			/*Remove parameters for WWW-Authenticate*/
			sdf_ivk_uaDeleteHeaderListByCorrelation(
				pProcessedHeaderList, "uat-authenticate", &error);

			/*Remove parameters for Proxy-Authenticate*/
			sdf_ivk_uaDeleteHeaderListByCorrelation(
				pProcessedHeaderList, "uat-proxyauth", &error);
		}
#endif
        /*
         * sdf_ivk_uaTerminateCall simply forms the correct BYE for
         * the call. It does not send it yet.
         */
        if (Sdf_co_fail == sdf_ivk_uaTerminateCall(
                    p_ssa_ctxt->p_call_obj,&error))
        {
            /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
            /* Clear the transaction and unlock call object */
            sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlaap_txn,"BYE",Sdf_en_uacTransaction);
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                    p_ssa_pdb,
                    (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                    p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {


            /* insert Privacy header in BYE request if application requested 
             * it at the time of create_call */
            if (ICF_PRIVACY_HDR_ALL_REQ & 
                p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask) 
            {
                if (ICF_FAILURE == icf_ssa_form_and_insert_priv_hdr(
                                        p_ssa_pdb,
                                        p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
                {
	                sdf_ivk_uaClearTransaction(
                        p_ssa_ctxt->p_call_obj,
                        p_overlaap_txn,
                        "BYE",
                        Sdf_en_uacTransaction);
				    ret_val = ICF_FAILURE;
                }         
            }

            if (ICF_FAILURE == ret_val)
            {
                ICF_PRINT(((icf_uint8_t *)"\nFailed to insert Privacy header in BYE request"));
            }
            else if ( ICF_FAILURE == 
				icf_ssa_form_and_insert_access_nw_info_hdr(
				p_ssa_pdb, p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
			{
	            sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlaap_txn,"BYE",Sdf_en_uacTransaction);
				ret_val = ICF_FAILURE;
			}
			
#ifdef ICF_IPSEC_ENABLED
			/* insert the security verify header in the outgoing protected
			 * message if an SA exists 
			 */
			if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
				/* if SA exits */
				(ICF_NULL != p_ssa_pdb->p_sa_context))
			{
				/* insert security-verify header in the otg BYE */
				if (ICF_FAILURE == icf_ssa_get_sec_verify_string (p_ssa_pdb, &p_value,
					p_ssa_pdb->p_sa_context))
                {
                    ret_val = ICF_FAILURE;
                }
				else if (ICF_FAILURE ==     
				  icf_ssa_insert_unknown_hdr_in_sip_msg (
					  p_ssa_pdb,
					  (icf_uint8_t*)"Security-Verify",
					  p_value, 
					  p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
				{
					ICF_PRINT(("\nFailed to insert unknown header \
							(Security-Verify)"));
					ret_val = ICF_FAILURE;
				}
				if (ICF_NULL != p_value)
				{
					ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
						p_value,
						ICF_MEM_COMMON,
						p_ssa_pdb->p_ecode,
						ret_val)
				}
				if (ICF_SUCCESS == ret_val )
				{
					ICF_PRINT(("\nSecurity Verify Header inserted to \
									outgoing Register "));
				}
			}
#endif


            if (ICF_FAILURE != ret_val)
            {


                /* Specify the stack options */
                options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
                /*Send BYE to peer */
                if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_ssa_ctxt->p_call_obj,
                    p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg,
                    &options,&error))
                {
                    /*Raise/Set Error: UATK_PROCESSING_FAILURE */
                    /* Clear the transaction and unlock call object */
                    sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                        p_overlaap_txn,"BYE",Sdf_en_uacTransaction);
                    ret_val = ICF_FAILURE;
                }
                	else
                    {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_requests.stat_outbound_req.stats_BYE);
                    }

            }


        }
        if (ICF_FAILURE != ret_val)
        {
            /* Save line id in SSA context */
			/*Fix for CSR 1-8081459*/
            p_ssa_ctxt->line_id = p_ssa_pdb->p_glb_pdb->p_call_ctx->line_id;
            /* 
             * sdf_ivk_uaEndTransaction unlocks the call object
             * which was locked in the call to 
             * sdf_ivk_uaStartTransaction
             */
            sdf_ivk_uaEndTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlaap_txn, "BYE", &error);
        } /* if (Sdf_co_fail != ret_val) */
    } /* else */

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */

#ifdef ICF_DNS_LOOKUP_ENABLED

     /* Temporary fix : If any thing failed then it comes to a loop

      * as CM sends failure indication again to ssa */

     /* Returned success: Timer will be expired in cc and forcefull

      * call clear will happen */

     if(ICF_FAILURE == ret_val)

     {

        ret_val = ICF_SUCCESS;

     }

#endif

    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_abort_resp
 **
 * DESCRIPTION:        This function sends a 487 Req Terminated message
 *                     to peer. This is triggered by CC 
 *                     1. Invoke sdf_ivk_uaFormResponse and then send the 
 *                     response to peer.
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_abort_resp(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;

    Sdf_st_error                        sdf_error;
    SipOptions                          options;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

        if (Sdf_co_fail == sdf_ivk_uaFormResponse(487,"INVITE", 
                    p_ssa_ctxt->p_call_obj, Sdf_co_null,
                    Sdf_co_false, &sdf_error))
        {
            /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in Forming 487 for AbortResp"));
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Specify the stack options */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_ssa_ctxt->p_call_obj,
                        p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg, 
                        &options,&sdf_error))
            {
                /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
#ifdef ICF_STAT_ENABLE
            else
            {
                icf_ssa_invite_resp_stats(p_ssa_pdb,487,1);
            }  
#endif
        } /* else */

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_abort
 **
 * DESCRIPTION:        This function sends CANCEL message to peer
 *                     1.This is also used to send CANCEL for Re-Invite
 *                     The re-invite processor calls this function
 *                     2.Invoke sdf_ivk_uaCancelCall which makes the
 *                     CANCEL message and then send the same to peer.
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_abort(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st     *p_temp_ssa_ctxt = ICF_NULL;
    Sdf_st_error            error;
    SipOptions              options;
    Sdf_st_overlapTransInfo *p_overlaap_txn = Sdf_co_null;
    Sdf_st_callObject     *p_call_obj = ICF_NULL;
#ifdef ICF_LOOPBACK_SUPPORT
    Sdf_st_error           p_err ;
#endif
    icf_error_t            p_ecode = 0;

    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_ssa_ctxt;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    
    p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
    
    /* Forking - Find the first ssa context for same callid
     * which will be same from which INVITE was sent originally */
    if ( ICF_FAILURE != icf_ssa_map_nw_callid(
                p_ssa_pdb,
#ifdef ICF_LOOPBACK_SUPPORT
                p_call_obj,
#endif
                (icf_uint8_t *)p_call_obj->pCommonInfo->pCallid,
                &p_temp_ssa_ctxt,
#ifdef ICF_LOOPBACK_SUPPORT
                &p_err,
#endif
                &p_ecode))
    {
           /* Change the original ssa context with the one found 
            * as CANCEL can't be sent on forked call object it should
            * be sent on original call object which initiated the Call*/
           p_ssa_ctxt = p_temp_ssa_ctxt; 
           p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
           p_ssa_ctxt->p_call_ctxt->p_sip_ctx = p_ssa_ctxt;
    }

        /* 
     * If SSA ctxt is not present,just return SUCCESS.
     * It is possible that in some erroneous condition,SSA
     * gets a trigger to send BYE on network when it has already
     * cleared its context.
     */
    if (ICF_NULL == p_ssa_ctxt)
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }
    /*
     * Check if OPTIONS is ongoing.If yes,invoke
     * the utility function to send the failure response 
     * to peer as well as clear the OPTIONS the context
     */
    if (ICF_NULL != ( p_ssa_ctxt->bitmask & ICF_SSA_OPTIONS_ONGOING ))
    {
        /* Not checking ret_val as this is best effort */         
        icf_ssa_clear_options_context(p_ssa_pdb);
    }

    /* Start a new transaction for sending the CANCEL */
    if (Sdf_co_fail == sdf_ivk_uaStartTransaction(
                p_ssa_ctxt->p_call_obj,&p_overlaap_txn,"CANCEL", &error))
    {
        ret_val = Sdf_co_fail;
    }
    else
    {
        /*
         * sdf_ivk_uaCancelCall simply forms the correct CANCEL for
         * the call. It does not send it yet.
         */
        if (Sdf_co_fail == sdf_ivk_uaCancelCall(
                    p_ssa_pdb->p_ssa_ctxt->p_call_obj,&error))
        {
            /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
            /* Clear the transaction and unlock call object */
            sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlaap_txn,"CANCEL",Sdf_en_uacTransaction);
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                    p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {

            /* Specify the stack options */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_ssa_ctxt->p_call_obj,
                        p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg,
                        &options,&error))
            {
                /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
                /* Clear the transaction and unlock call object */
                sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                        p_overlaap_txn,"CANCEL",Sdf_en_uacTransaction);

                /*Fix for CSR 1-8492091 SPR 20377*/  
                /* Free the call object*/ 
                sdf_ivk_uaFreeCallObject(&(p_ssa_ctxt->p_call_obj));

                ret_val = ICF_FAILURE;
            }
            /*Stat collection of CANCEL request sent*/
            else
            {
                ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                        stat_requests.stat_outbound_req.stats_CANCEL);
            }
        }
        if (ICF_FAILURE != ret_val)
        {
            /* For forking, check and delete all other ssa context forcefully */
            icf_ssa_del_all_other_call_obj(p_ssa_pdb);
            /* 
             * sdf_ivk_uaEndTransaction unlocks the call object
             * which was locked in the call to 
             * sdf_ivk_uaStartTransaction
             */
            sdf_ivk_uaEndTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlaap_txn,"CANCEL", &error);
        } /* if (Sdf_co_fail != ret_val) */
    } /* else */

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_prack
 **
 * DESCRIPTION:        This functions sends a PRACK for the Reliable 
 *                     provisional responses (RPR) received
 *                     1.It is called for following three APIs
 *                          a.ICF_NW_OTG_REDIRECT_IND_PRACK
 *                          b.ICF_NW_OTG_MEDIA_PARAM_PRACK
 *                          c.ICF_NW_OTG_ALERT_PRACK
 *                     2.Fetch overlapped transaction corresponding to the 
 *                     RPR received and form the PRACK request on it and
 *                     then send the request to peer.
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_prack(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    Sdf_st_error            error;
    SipOptions              options;
    Sdf_st_overlapTransInfo *p_ovtxn = Sdf_co_null;
    Sdf_ty_s8bit            proto[ICF_MAX_SIZE_INT_USAGE_STR];
    
	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /*
     * The overlapped transcations are maintained as a list
     * in the call object.Fetch the matching overlapped 
     * transaction on the basis of the message type and
     * the transaction state
     */
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_invite,Sdf_en_rprReceived,
                p_ssa_pdb->p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else
    {
                        /* Fix for CSR 1-4826401 */
			/* PRACK transaction should use the transport mode that was used 
			 * for initial INVITE
			 */
#if 0

			if ((ICF_TRANSPORT_TYPE_TCP == p_ssa_pdb->p_line_data->transport_mode) ||
							(ICF_TRANSPORT_MODE_BOTH == 
							 p_ssa_pdb->p_line_data->transport_mode))
			{
					icf_port_strcpy(proto, "TCP"); 
			}
			else if (ICF_TRANSPORT_TYPE_TLS == 
							p_ssa_pdb->p_line_data->transport_mode)
			{
					icf_port_strcpy(proto, "TLS"); 
			}
			else
			{
					icf_port_strcpy(proto, "UDP"); 
			}
#endif

        if (ICF_SSA_TLS_TRANSPORT & p_ssa_pdb->p_ssa_ctxt->bitmask)
			{
				icf_port_strcpy((icf_uint8_t *)proto, (icf_uint8_t *)"TLS");
			}
			else if (ICF_SSA_TCP_TRANSPORT & p_ssa_pdb->p_ssa_ctxt->bitmask)
			{
				icf_port_strcpy((icf_uint8_t *)proto, (icf_uint8_t *)"TCP");
			}
			else
			{
				icf_port_strcpy((icf_uint8_t *)proto, (icf_uint8_t *)"UDP");
			}
        /*
         * Form a PRACK request
         */
        if (Sdf_co_fail == sdf_ivk_uaMakeTransaction(
                    (Sdf_ty_s8bit *)"PRACK",p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                    p_ovtxn, &error))
        {
            /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE; 
        }
		else if (Sdf_co_fail ==    
						sdf_ivk_uaSetTransportSchemeInTransaction(
						ICF_NULL, p_ovtxn, proto, &error))
		{
				ret_val = ICF_FAILURE; 
		}
        else
        {
            /* Specify the stack options */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

            /* Send PRACK to remote end */
            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                        p_ovtxn->pSipMsg,
                        &options,&error))
            {
                /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE; 
            }
            else
            {
                               /* Stat collection of PRACK sent*/
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_outbound_req.stats_PRACK);

#ifdef ICF_LOAD_DEBUG
                printf("\nPRACK sent for       %d\n", p_ssa_pdb->p_ssa_ctxt->call_id);
#endif
            }
        }
    }


    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_prack_resp
 **
 * DESCRIPTION:     This functions sends a response for PRACK received 
 *                  1.Fetch the overlapped transaction from the list of
 *                  overlapped transactions stored in the call object 
 *                  corresponding to the PRACK request received.
 *                  2.Check if SDP needs to be sent in the response.If yes,
 *                  form the SDP and set it in the overlapped transaction
 *                  3.Send the response to peer.
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_prack_resp(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;

    icf_nw_otg_prack_ack_st  *p_prack_resp = ICF_NULL;

    Sdf_st_error                error;
    SipOptions                  options;
    Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;
    SdpMessage                  *p_SdpMsg = Sdf_co_null;
    Sdf_ty_u16bit               sdf_resp_code = 200;
    icf_ssa_ctxt_st *p_ssa_ctxt =
                        (icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;
    Sdf_st_callObject   *p_call_obj = p_ssa_ctxt->p_call_obj;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /* Extract the payload from the internal msg */
    p_prack_resp = (icf_nw_otg_prack_ack_st *)
                    p_ssa_pdb->p_internal_msg->p_msg_data;

    /* Map the reasone code if the status is FALSE */
    if (ICF_FALSE == p_prack_resp->status)
    {
        icf_ssa_map_interface_reason_code(p_prack_resp->reason_type,
                    &sdf_resp_code);
    }
    /*
     * The overlapped transcations are maintained as a list
     * in the call object.Fetch the matching overlapped 
     * transaction on the basis of the message type and
     * the transaction state
     */
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_prack,Sdf_en_overlapTransactionReceived,
                p_call_obj,&p_ovtxn))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* 
         * Flush the message bodies (if any) stored in the 
         * overlap transaction of the call object
         */
        sdf_listDeleteAll(
                &(p_ovtxn->slMsgBodyList),&error);
        /* Form the response for PRACK */
        if (Sdf_co_fail == sdf_ivk_uaFormResponse(sdf_resp_code,"PRACK", 
                    p_call_obj, p_ovtxn,
                    Sdf_co_false, &error))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == 
                icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ovtxn->pSipMsg))
        {
            ret_val = ICF_FAILURE;
        }
        /*
         * Now check whether SDP needs to be sent in
         * the 200 OK of PRACK
         */
        else
        {
            if (ICF_NULL != ICF_SSA_GET_COMMON_BITMASK(
                        p_prack_resp->bitmask,ICF_NW_OTG_PRACK_ACK_SDP)
                        && (200 == sdf_resp_code))
            {
                /*
                 * SSA is not using any media handling related
                 * functionality provided by the UATK.
                 * Whenever SDP needs to be sent in a message,
                 * SSA picks up the local SDP stored in the call 
                 * context and forms an SDP message body from it.
                 * This SDP body is then set into the UAS/UAC/Overlap
                 * transaction of the call object
                 */                          

                /* Form SDP message body */
                if(ICF_FAILURE == 
                        icf_ssa_form_sdp_from_stream_sdp_info( 
                            &p_SdpMsg,
                            p_ssa_pdb,
                            ICF_SSA_NO_HOLD,
                            &error))
                {
                    ret_val = ICF_FAILURE;
                }
                /* Set SDP in Outgoing Transaction */
                else 
                if(Sdf_co_fail == 
                        sdf_ivk_uaSetSdpInOverlapTransaction(
                            p_ovtxn, 
                            p_SdpMsg,&error))
                {
                    sip_freeSdpMessage(p_SdpMsg);
                    ret_val = ICF_FAILURE;
                }
		/*Message body header changes Start*/
                /*The above API added the SDP msg body in the msg body list
                 * of the transaction. Now we need to add this to the 
                 * actual outgoing SIP Message*/
                else if ( Sdf_co_fail == 
                          icf_ssa_SetAllMsgBody(p_call_obj->pUasTransaction,\
                           p_ovtxn,/*Overlap Txn Info*/"application/sdp",\
                            &error))
                {
                    sip_freeSdpMessage(p_SdpMsg);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
		/*Message body header changes Ends*/

            }
            if (ICF_SUCCESS == ret_val)
            {
                /* Specify the stack options */
                options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
                if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                            p_call_obj,
                            p_ovtxn->pSipMsg,
                            &options,&error))
                {
                    sip_freeSdpMessage(p_SdpMsg);
                    ret_val = ICF_FAILURE;
                }
                else
                {
#ifdef ICF_STAT_ENABLE
		    icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"PRACK",sdf_resp_code,1);
#endif
                    sip_freeSdpMessage(p_SdpMsg);
                }
            } 
        }
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_terminate_resp
 **
 * DESCRIPTION:        This function sends the 200 OK for the BYE message
 *                     received from peer
 *                     1. Form and send the response and then free the call
 *                     object.
 *                     2.After the 200 OK for the BYE has been sent,
 *                     the link between SSA and the call control needs to be 
 *                     broken as the call context for the same has been
 *                     destroyed.For this purpose set the bit 
 *                     ICF_SSA_APP_CTXT_TERMINATED in the SSA ctxt
 *                     which indicates that no further messages for this 
 *                     dialog needs to be sent to CC
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_call_terminate_resp(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    Sdf_st_error            error;
    SipOptions              options;

    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;
    if(ICF_NULL == p_ssa_ctxt)
    {
	ret_val = ICF_FAILURE;
    }
    else
    {
    Sdf_st_callObject	*p_call_obj = p_ssa_ctxt->p_call_obj;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    /*
     * This indicates to SSA that on reception of any further 
     * msg from N/w for this dialog, the CM is NOT TO BE
     * INDICATED
     */
    p_ssa_ctxt->bitmask |= ICF_SSA_APP_CTXT_TERMINATED;

    /* Send 200 OK response to peer */
    if (Sdf_co_fail == sdf_ivk_uaFormResponse(200,(Sdf_ty_s8bit *)"BYE", 
                p_ssa_ctxt->p_call_obj,Sdf_co_null,Sdf_co_false, &error))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Add Supported:timer header in the 200 ok response */
         /* UATK does not add it */
		
#ifdef ICF_SESSION_TIMER
       if (ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)
       {
	
	    SipHeader	    *pSupportedHeader = Sdf_co_null;
            SipError        SipErr;
            SIP_U32bit      count=0;
	    if (sip_initSipHeader(&pSupportedHeader,SipHdrTypeSupported,\
				 &(SipErr)) == SipFail)
	    {
                ret_val = ICF_FAILURE;
       	    }
	    else if(sip_setOptionInSupportedHdr(pSupportedHeader,
                        sip_strdup("timer",Sdf_mc_initMemId), \
		 	&SipErr)==SipFail)
	    {
                ret_val = ICF_FAILURE;
	    }
            else if(SipFail == 
                 sip_getHeaderCount(p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
                 SipHdrTypeSupported,&count,&SipErr))
            {
                ret_val = ICF_FAILURE;
            }
	    else if (sip_insertHeaderAtIndex(p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg,
                        pSupportedHeader,count, \
	        	&(SipErr)) == SipFail)
	    {
                ret_val = ICF_FAILURE;
	    }
	    /* Freeing the Local reference */

	    /* purify */
            if(ICF_NULL != pSupportedHeader)
            {
			    sip_freeSipHeader(pSupportedHeader); 
		        sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
				&pSupportedHeader, Sdf_co_null); 
            }
        }
#endif

        if(ICF_SUCCESS == ret_val)
        {
            if ( ICF_FAILURE == icf_ssa_form_and_insert_access_nw_info_hdr(
				    p_ssa_pdb, p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }

            /* Specify the stack options */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
            /*  (0 for UDP, 1 for TCP) */
            if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_ssa_ctxt->p_call_obj,
                        p_ssa_ctxt->p_call_obj->pUasTransaction->pSipMsg, 
                        &options,&error))
                /*  (0 for UDP, 1 for TCP) */
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
#ifdef ICF_STAT_ENABLE
		icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"BYE",200,1);
#endif
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
        }
    }
    } /* else */
    
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_media_change_resp
 **
  * DESCRIPTION:        This function is called to send resp to received 
 *                     UPDATE/RE-INVITE
 *                     1.The information as to whether to send the response 
 *                     for UPDATE or RE-INVITE is retrieved from the SSA ctxt
 *                     where this information is stored on the receipt 
 *                     of the message.
 *                     2. If failure response needs to be sent,the appropiate
 *                     response code needs to be mapped corresponding to the
 *                     reason type set by call control.
 *                     3.Check whether SDP needs to be sent and if yes
 *                     form the SDP body and append it in the message body
 *                     list in the transaction.
 *                     4.If the response is for UPDATE,
 *                     SSA will reset signalling_ongoing flag of Call Context. 
 *                     SSA will also set ICF_STATUS_SIGNAL_ONOING_ENDED in 
 *                     status field of internal message in return path.
 *                     
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_media_change_resp(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;

    icf_glb_pdb_st                  *p_glb_pdb =
        p_ssa_pdb->p_glb_pdb;

    icf_call_ctx_st                  *p_call_ctxt =
        p_glb_pdb->p_call_ctx;

    icf_ssa_ctxt_st                  *p_ssa_ctxt =ICF_NULL;

    icf_internal_msg_st              *p_internal_msg =
        p_ssa_pdb->p_internal_msg;

    icf_nw_otg_media_change_resp_st  *p_otg_media_chng_resp;

    Sdf_ty_u16bit                       sdf_resp_code = 200;
    Sdf_st_error                        sdf_error;
    Sdf_st_overlapTransInfo             *p_ovtxn =
        Sdf_co_null;
    Sdf_st_callObject                   *p_call_obj = Sdf_co_null;
    SipOptions                          dOptions;
    SdpMessage                          *p_sdp_msg = Sdf_co_null;
    SipMessage                          *p_sip_msg = Sdf_co_null;
    icf_uint8_t 			hold_option = ICF_SSA_NO_HOLD;

    ICF_ASSERT(p_glb_pdb, p_call_ctxt)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ssa_ctxt = (icf_ssa_ctxt_st*)
        (p_call_ctxt->p_sip_ctx);

    p_call_obj = p_ssa_ctxt->p_call_obj;

    p_otg_media_chng_resp = (icf_nw_otg_media_change_resp_st*)
        (p_internal_msg->p_msg_data);

    /*Things to do in this routine:
     * check what media change mesg was received-reinvite or update?
     * based on that get the pointer to the transaction 
     * then set the SDP in the transaction if required
     * then send the transaction
     * reset the bitmask in SSA CTXT
     */

    if ( ICF_FALSE == p_otg_media_chng_resp->status)
    {
        p_ssa_ctxt->bitmask &= ~ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP;

       /* FIx for CSR 1-4555732 */
        p_ssa_ctxt->bitmask &= ~ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP;

        if (ICF_NW_OTG_MC_RESP_NW_RESPONSE & 
                p_otg_media_chng_resp->bitmask )
        {
            sdf_resp_code = p_otg_media_chng_resp->nw_response_code;
        }
        else if (ICF_NW_OTG_MC_RESP_REASON & 
                p_otg_media_chng_resp->bitmask )
        {
            if ( ICF_FAILURE == icf_ssa_map_cc_reason_code(
                        p_otg_media_chng_resp->reason_type,
                        &sdf_resp_code))
            {
                /*Raise/Set Error:INVALID_REASON_CODE*/
                ret_val = ICF_FAILURE;
            }
        }
        else
        {
            /*Default failure response code*/
            sdf_resp_code = 500;
        }
    }

    /*
     * It traverses in the overlap
     * txn list in the call object to get the UPDATE txn 
     * for which the resposne is to be sent*/
    if ( (ICF_FAILURE != ret_val) &&
            (ICF_SSA_CTXT_UPDATE_RECVD & p_ssa_ctxt->bitmask))
    {

        /*
         * The overlapped transcations are maintained as a list
         * in the call object.Fetch the matching overlapped 
         * transaction on the basis of the message type and
         * the transaction state
         */
        if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                    p_ssa_pdb,Sdf_en_update,Sdf_en_transactionReceived,
                    p_call_obj,&p_ovtxn))
        {
            ret_val = ICF_FAILURE;
        }        
    }

    if ( ICF_FAILURE != ret_val)
    {
        /*Block for checking of SDP is to be sent in response, 
         * and for addition of the same if required*/
        if ( ICF_NULL != (ICF_NW_OTG_SDP_PRESENT & 
                    p_otg_media_chng_resp->bitmask))
		{                     
			 /* 
	          * If the call is locall held and the applcation has selected to send 
              * C = 0.0.0.0 as call hold option the send C=0 in answer to offer as well
              */
			if((p_glb_pdb->p_call_ctx->media_state & 
                 ICF_MEDIA_STATE_LOCAL_HELD)) 
	      	{ 
		      if ((ICF_CFG_CC_MUTE_OPTION_INVITE_C_ZERO == 
					      p_ssa_pdb->p_glb_cfg_data->hold_option) ||
				      (ICF_CFG_CC_MUTE_OPTION_UPDATE_C_ZERO == 
				       p_ssa_pdb->p_glb_cfg_data->hold_option))
		      {
			      hold_option = ICF_SSA_HOLD_CLINE_ZERO;
		      }
		      else if ((ICF_CFG_CC_HOLD_OPTION_INV_WTH_MODE_SEND_ONLY_N_C_0 == 
					      p_ssa_pdb->p_glb_cfg_data->hold_option) ||
				      (ICF_CFG_CC_HOLD_OPTION_UPD_WTH_MODE_SEND_ONLY_N_C_0 == 
										 p_ssa_pdb->p_glb_cfg_data->hold_option))
					{
							hold_option = ICF_SSA_HOLD_CLINE_ZERO;
							hold_option |= ICF_SSA_HOLD_MODE_SENDONLY; 
					}
				}
			/* 
             * Flush the message bodies (if any) stored in the 
             * UAC transaction of the call object.
             */
            if ( ICF_FAILURE == 
                    icf_ssa_form_sdp_from_stream_sdp_info(
                        &p_sdp_msg,
                        p_ssa_pdb,
                        hold_option, 
                        &sdf_error))
            {
                ret_val = ICF_FAILURE;
            }
        }/*End of processing if SDP is to be sent*/
    }
    if ( ICF_FAILURE != ret_val)
    {
        if(ICF_SSA_CTXT_UPDATE_RECVD & p_ssa_ctxt->bitmask)
        {
            if (ICF_NULL == p_ovtxn)
                return ICF_FAILURE;

            sdf_listDeleteAll(
                    &(p_ovtxn->slMsgBodyList),&sdf_error);
 
            if (Sdf_co_fail == 
                    sdf_ivk_uaFormResponse(sdf_resp_code,
                        (Sdf_ty_s8bit *)"UPDATE", 
                        p_call_obj,
                        p_ovtxn,
                        Sdf_co_false, 
                        &sdf_error))
            {
                /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else if (ICF_NULL == p_ovtxn)
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                        p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ovtxn->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }

            /* Set SDP in Otg Txn if p_sdp_msg is notnull */
            if ( ICF_FAILURE != ret_val &&
                    ICF_NULL != p_sdp_msg)
            {
                if(Sdf_co_fail == sdf_ivk_uaSetSdpInOverlapTransaction(
                            p_ovtxn,
                            p_sdp_msg, 
                            &sdf_error))
                {
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
            }
            if ( ICF_FAILURE != ret_val)
            {
                p_sip_msg = p_ovtxn->pSipMsg;
            }
        }
        else
        {
            sdf_listDeleteAll(
                    &(p_call_obj->pUacTransaction->slMsgBodyList),&sdf_error);
            if (Sdf_co_fail == sdf_ivk_uaFormResponse(sdf_resp_code,
                        (Sdf_ty_s8bit *)"INVITE", 
                        p_call_obj,
                        Sdf_co_null,
                        Sdf_co_false, 
                        &sdf_error))
            {
                /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                        p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_call_obj->pUasTransaction->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }            

            /* Set SDP in Outgoing Transaction */
            if ( ICF_FAILURE != ret_val &&
                    ICF_NULL != p_sdp_msg)
            {
                if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
                            p_call_obj->pUasTransaction,
                            p_sdp_msg, 
                            &sdf_error))
                {
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
            }
            if (ICF_FAILURE != ret_val)
            {
                p_sip_msg = p_call_obj->pUasTransaction->pSipMsg;
            }
        }
#ifdef ICF_SESSION_TIMER
            /*icf_ssa_chk_n_insert_timer_in_supp_hdr(p_ssa_pdb);*/
#endif
	/*Message body header changes Start*/
        /*The above API added the SDP msg body in the msg body list
         * of the transaction. Now we need to add this to the 
         * actual outgoing SIP Message
         * Also, if it is a reinvite which is being responded to,the 
         * p_call_obj->pUasTransaction will not be null while
         * p_ovtxn will be null.  INOUT case of UPDATE it will be
         * the other way round*/
        if ( Sdf_co_fail == 
                icf_ssa_SetAllMsgBody(p_call_obj->pUasTransaction,\
			p_ovtxn,/*OverlapTxn*/"application/sdp",&sdf_error))
        {
            /*Raise a Major error here - UATK_PROCESSING FAILURE*/
            ret_val = ICF_FAILURE;
        }
	/*Message body header changes Ends*/
    }

    if (ICF_FAILURE == ret_val)
    {
        ret_val = ICF_FAILURE;
    }
    /* this is needed to send Allow header in 200 OK */
    else if(ICF_FAILURE == icf_cfg_fetch_app_conf(
					p_ssa_pdb->p_glb_pdb,
					p_ssa_ctxt->p_call_ctxt->app_id,
					&(p_ssa_pdb->p_app_conf),
					p_ssa_pdb->p_ecode))
	{
		ret_val = ICF_FAILURE;
	}            
    else
    {
        dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_call_obj,
                    p_sip_msg,
                    &dOptions,&sdf_error))
        {
            /*Raise a Major error here - UATK_PROCESSING FAILURE*/
            ret_val = ICF_FAILURE;
        }
        else if(ICF_SSA_CTXT_UPDATE_RECVD & p_ssa_ctxt->bitmask)
        {
            /*
             * When CC sends offer change response for UPDATE,
             * SSA will reset signalling_ongoing flag of Call Context. 
             * SSA will also set ICF_STATUS_SIGNAL_ONOING_ENDED in 
             * status field of internal message in return path.
             */
            p_call_ctxt->signalling_ongoing = 
                ICF_INT_MODULE_INVALID ;
            p_internal_msg->msg_hdr.status |= 
                ICF_STATUS_SIGNAL_ONGOING_ENDED ;
#ifdef ICF_STAT_ENABLE
		icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"UPDATE",sdf_resp_code,1);
#endif
        }
#ifdef ICF_STAT_ENABLE
      else
	{
	  icf_ssa_invite_resp_stats(p_ssa_pdb,sdf_resp_code,1);
	}
#endif
    }
    if ( Sdf_co_null != p_sdp_msg)
    {
            sip_freeSdpMessage(p_sdp_msg);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_media_update_abort
 **
 * DESCRIPTION:     This function is called to abort an
 *                  outstanding UPDATE/RE-INVITE
 *                  1.The media change was sent as a Re-Invite,
 *                  so a Cancel can be sent if a provisional response
 *                  has been received .
 *                  2.If there is a failure in sending the CANCEL for
 *                  RE-INVITE,simply abort the RE-INVITE transaction by
 *                  a call to sdf_ivk_uaAbortTransaction
 *                  3.If the media change was sent as an Update, Cancel is not
 *                  possible. So we abort the Overlap transaction
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_media_update_abort(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st              *p_ssa_ctxt = ICF_NULL;
    Sdf_st_overlapTransInfo         *p_ovtxn = Sdf_co_null;
    Sdf_st_error                    sdf_error;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_ssa_pdb->p_ssa_ctxt);

    if((Sdf_en_reInviteprovRespReceived == 
            p_ssa_ctxt->p_call_obj->pCallInfo->dState)
            || (Sdf_en_reInviteSent ==
            p_ssa_ctxt->p_call_obj->pCallInfo->dState))
    {
        /*
         * The media change was sent as a Re-Invite,
         * so a Cancel can be sent if a provisional response
         * has been received .If there is a failure in sending the 
         * CANCEL,simply abort the RE-INVITE transaction by  a call
         * to sdf_ivk_uaAbortTransaction
         */
        if ( ICF_FAILURE == icf_ssa_process_abort(p_ssa_pdb))
        {
            sdf_ivk_uaAbortTransaction(p_ssa_ctxt->p_call_obj,
                    p_ssa_ctxt->p_call_obj->pUacTransaction->dTxnId,
                    (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
        }
        p_ssa_ctxt->bitmask |= ICF_SSA_REINVITE_CANCELLED;
    }
    else
    {
        /*
         * Assumed that the media change was sent as an Update, Cancel is not
         * possible. So we abort the Overlap transaction
         * Even if there is a failure in aborting the transaction
         * we don't return failure as this is the best effort
         * The overlapped transcations are maintained as a list
         * in the call object.Fetch the matching overlapped 
         * transaction on the basis of the message type and
         * the transaction state
         */
        if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                    p_ssa_pdb,Sdf_en_notify,Sdf_en_transactionReceived,
                    p_ssa_ctxt->p_call_obj,&p_ovtxn))
        {
            /* Do not return FAILURE Do nothing */
        }
        else
        {
            sdf_ivk_uaAbortTransaction(p_ssa_ctxt->p_call_obj, 
                p_ovtxn->dTxnId, (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
        }
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

#ifdef ICF_SERVICE_ENABLE
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_media_change
 **
 * DESCRIPTION:        This function is called to send UPDATE/RE-INVITE to
 *                     the remote party.
 *                     1.The decision as to whether to send
 *                     an UPDATE or RE-INVITE is made on the basis of a 
 *                     configuration parameter
 *                     2.Hold can be indicated by one of the following
 *                          a. Cline 0.0.0.0
 *                          b. Port 0
 *                          c. Mode = sendonly
 *                     The same is also picked up from config paramter.
 *                     3. The appropiate SDP as described above is constructed
 *                     and the request is sent to peer.
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_media_change(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;

    icf_glb_pdb_st               *p_glb_pdb =
        p_ssa_pdb->p_glb_pdb;

    icf_call_ctx_st              *p_call_ctxt =
        p_glb_pdb->p_call_ctx;

    icf_ssa_ctxt_st              *p_ssa_ctxt = ICF_NULL;
        

    icf_config_data_st           *p_glb_cfg_data = 
        p_ssa_pdb->p_glb_cfg_data;

    icf_internal_msg_st          *p_internal_msg = p_ssa_pdb->p_internal_msg;

    icf_nw_otg_media_change_req_st *p_media_chng_req =
        (icf_nw_otg_media_change_req_st*)(p_internal_msg->p_msg_data);

    icf_uint8_t                  method[ICF_MAX_SIZE_INT_USAGE_STR],
    hold_option = ICF_SSA_NO_HOLD;

    Sdf_st_callObject               *p_call_obj = ICF_NULL;
    Sdf_st_error                    sdf_error;
    Sdf_st_overlapTransInfo         *p_overlap_txn = Sdf_co_null;
    SipMessage	                    *p_sip_msg = Sdf_co_null;
    SdpMessage                      *p_sdp_msg =Sdf_co_null ;
    icf_boolean_t                   is_call_connect = ICF_TRUE;
    icf_int8_t                     *p_content_type = ICF_NULL;
    Sdf_st_initData                 *p_callobj_spec_profile = 
                                    p_ssa_pdb->p_ssa_ctxt->p_call_obj->pInitData;
    Sdf_ty_s8bit                    display_name[ICF_MAX_STR_LEN] = "\0";
    Sdf_ty_s8bit                    tpt_scheme[ICF_MAX_STR_LEN] = "\0";
    icf_uint8_t                     *p_contact_str = ICF_NULL;

    /* Start: Addition for Per Line Features */
    icf_line_id_t                     line_id;
    icf_error_t                     ecode; 
    icf_line_data_st                *p_line_data;
    /* End: Addition for Per Line Features */

    if (ICF_NULL == p_call_ctxt)
    {
        return ICF_FAILURE;
    }
    ICF_ASSERT(p_glb_pdb, p_call_ctxt)

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_call_ctxt->p_sip_ctx);

    if (ICF_NULL == p_ssa_ctxt)
    {
       return ICF_FAILURE;
    }
    p_call_obj = p_ssa_ctxt->p_call_obj;

#ifdef ICF_SESSION_TIMER
   if((ICF_TRUE == p_glb_pdb->p_call_ctx->session_timer_enable) && \
	 (p_ssa_ctxt->session_timer_expired_refresh == ICF_TRUE))
    {
		p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
			ICF_NW_OTG_SESSION_REFRESH_IND;
		p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
    }
#endif

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    if (!p_call_ctxt->common_bitmask & ICF_CALL_CONNECT_DONE)
    {
        is_call_connect = ICF_FALSE;
    }
ICF_CHECK_IMS_END
#endif

/* Start: Added Code for Obsolete Hold Parameter */
if(ICF_FAILURE == icf_ssa_get_line_id_from_glb_pdb( p_glb_pdb,
                                                    p_ssa_ctxt,
                                                    &line_id,
                                                    &ecode) )
 {
   ret_val = ICF_FAILURE;
 }
 if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk( p_glb_pdb,
                                                  line_id,
                                                  (icf_line_data_st **)(&p_line_data),
                                                  &ecode) )
 {
   ret_val = ICF_FAILURE;
 } 
 if(ICF_SUCCESS == ret_val)
  p_glb_cfg_data->hold_option = p_line_data->hold_option;

  /* End : Added Code for Obsolete Hold Parameter */

    if (ICF_TRUE == is_call_connect)
    {
        if ((ICF_CFG_CC_MUTE_OPTION_INVITE_C_ZERO == 
                    p_glb_cfg_data->hold_option )||
                (ICF_CFG_CC_MUTE_OPTION_INVITE_MODE_INACTIVE == 
                 p_glb_cfg_data->hold_option )||
                (ICF_CFG_CC_HOLD_OPTION_INV_WTH_MODE_SEND_ONLY_N_C_0 ==
                 p_glb_cfg_data->hold_option))
        {
            icf_port_strcpy(method,(icf_uint8_t *)"INVITE");
        }
        else if(ICF_UPDATE_PRESENT_IN_ALLOW_HDR == p_call_ctxt->allowed_value)
        {
            icf_port_strcpy(method,(icf_uint8_t *)"UPDATE");
        }
        else
        {
            icf_port_strcpy(method,(icf_uint8_t *)"INVITE");
        }
    }
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
   if (ICF_FALSE == is_call_connect)    
    {
        icf_port_strcpy(method,"UPDATE");
    }  
ICF_CHECK_IMS_END
#endif

	if(p_ssa_ctxt->p_call_obj->pMediaInfo != ICF_NULL)
	{
		p_ssa_ctxt->p_call_obj->pMediaInfo->dMediaState = Sdf_en_idleMedia;
	}

    if ( ICF_TRUE == p_media_chng_req->call_hold)
    {
        if ((ICF_CFG_CC_MUTE_OPTION_INVITE_C_ZERO == 
                    p_glb_cfg_data->hold_option) ||
                (ICF_CFG_CC_MUTE_OPTION_UPDATE_C_ZERO == 
                 p_glb_cfg_data->hold_option))
        {
            hold_option = ICF_SSA_HOLD_CLINE_ZERO;
        }
        else if ((ICF_CFG_CC_MUTE_OPTION_INVITE_MODE_INACTIVE == 
                    p_glb_cfg_data->hold_option) ||
                (ICF_CFG_CC_MUTE_OPTION_UPDATE_MODE_INACTIVE == 
                 p_glb_cfg_data->hold_option))
        {
            hold_option = ICF_SSA_HOLD_MODE_SENDONLY;
        }
        else if ((ICF_CFG_CC_HOLD_OPTION_INV_WTH_MODE_SEND_ONLY_N_C_0 == 
                    p_glb_cfg_data->hold_option) ||
                (ICF_CFG_CC_HOLD_OPTION_UPD_WTH_MODE_SEND_ONLY_N_C_0 == 
                 p_glb_cfg_data->hold_option))
        {
            hold_option |= ICF_SSA_HOLD_MODE_SENDONLY;
            hold_option |= ICF_SSA_HOLD_CLINE_ZERO;
        }
        else
        {
            hold_option = ICF_SSA_HOLD_PORT_ZERO;
        }
    }

    /*Check if Contact header is present in call context header list*/
    if (ICF_TRUE == icf_ssa_is_contact_present_in_hdr_list(p_ssa_pdb, &p_contact_str, (icf_uint8_t *)display_name, (icf_uint8_t *)tpt_scheme))
    {
        /*If Contact header present then delete previous contact from init_data
         *and call AddDefaultContact to add application specific Contact header*/
        if (Sdf_co_fail == sdf_listDeleteAt(&(p_callobj_spec_profile->slContact), 0, &sdf_error))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = (icf_return_t)sdf_ivk_uaAddDefaultContact(
                    p_callobj_spec_profile,
                    (icf_int8_t *)display_name,
                    (icf_int8_t *)p_contact_str,
                    ICF_NULL,
                    tpt_scheme,
                    &sdf_error);
        }
        /* spr16521 */
        ICF_MEMFREE(
            p_ssa_pdb->p_glb_pdb,
            p_contact_str,
            ICF_MEM_COMMON,
            p_ssa_pdb->p_ecode,
            ret_val)

    }

    if (ICF_SUCCESS == ret_val)
    {
	    if (Sdf_co_fail == sdf_ivk_uaStartTransaction(
				    p_call_obj,
				    &p_overlap_txn,
				    (const icf_int8_t *)method,
				    &sdf_error))
	    {
		    /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
		    ret_val = ICF_FAILURE;
	    }
	    else if (Sdf_co_fail == 
			    sdf_ivk_uaMakeTransaction((icf_int8_t *)method, 
				    p_call_obj, 
				    p_overlap_txn, 
				    &sdf_error))
	    {
		    /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
		    sdf_ivk_uaClearTransaction(
				    p_call_obj,
				    p_overlap_txn,
				    (icf_int8_t *)method,
				    Sdf_en_uacTransaction);
		    ret_val = ICF_FAILURE;
	    }
    }

    if ( ICF_SUCCESS == ret_val)
    {
        SipOptions  dOptions;
        /* 
         * Flush the message bodies (if any) stored in the 
         * UAC transaction of the call object
         */
        sdf_listDeleteAll(
                &(p_call_obj->pUacTransaction->slMsgBodyList),&sdf_error);


        dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

      /*  ICF_ASSERT(p_glb_pdb, p_call_ctxt->p_local_sdp)*/
        
	if(ICF_CALL_TYPE_T38 & p_call_ctxt->call_type)
	{
		/* Update SSA Call type
		*/
		p_ssa_pdb->p_ssa_ctxt->call_type |= ICF_CALL_TYPE_T38;
	}
        if ( ICF_FAILURE == 
                icf_ssa_form_sdp_from_stream_sdp_info(
                    &p_sdp_msg,
                    p_ssa_pdb,
                    hold_option,
                    &sdf_error))
        {
            sdf_ivk_uaClearTransaction(
                    p_call_obj,
                    p_overlap_txn,
                    (icf_int8_t *)method,
                    Sdf_en_uacTransaction);
            ret_val = ICF_FAILURE;
        }
        else
        {   
            icf_uint8_t  count = 0;
            
            /*Message body header changes Start*/
            /*Set the count to the number of message bodies in the request
            */
            count = (icf_uint8_t)p_ssa_pdb->p_glb_pdb->p_call_ctx->body_list.count;
            if(0 == count)
            {
                Sdf_mc_strdup(p_content_type,"application/sdp");
            }
           else
               {
                         if(p_ssa_ctxt->p_call_ctxt->body_list.content_type.str_len != 0)
                         {
						     /*Fix done for SPR-18722 : CSR-1-6068965*/
							 p_content_type = (icf_int8_t *)p_ssa_ctxt->p_call_ctxt->body_list.content_type.str;
                         }
               }


            /* 
             * If method is UPDATE set SDP in overlapped transaction
             * else set SDP in UAC transaction
             */
            if (0 == icf_port_strcmp(method,(icf_uint8_t *)"UPDATE"))
            {
                if (Sdf_co_fail == sdf_ivk_uaSetSdpInOverlapTransaction(
                            p_overlap_txn, 
                            p_sdp_msg, &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    p_sip_msg = p_overlap_txn->pSipMsg;
                }
            }
            else
            {
                if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
                            p_call_obj->pUacTransaction, p_sdp_msg, 
                            &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn,(icf_int8_t *) method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
                else if (ICF_FAILURE == icf_insert_header_tag_body_list(
                    p_ssa_pdb, ICF_TRUE,
                    p_call_obj->pUacTransaction, ICF_NULL))
                {
                    
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;                        
                }
                else
                {
                    p_sip_msg = p_call_obj->pUacTransaction->pSipMsg;
                }
            }
            /* CSR 8061646 SPR 20231: Set the bitmask which will indicate whether Update sent for
			 * call hold/call resume.
			 */
			 if(ICF_TRUE == p_media_chng_req->call_hold)
			 {
					p_ssa_ctxt->bitmask_1 |= ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD;
			 }	
			 else
			 {
					p_ssa_ctxt->bitmask_1 |= ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME;
			 }

            
	    /*Message body header changes Start*/
            if(ICF_FAILURE != ret_val)
            {
                /* Set the bitmask in SSA context for Update with SDP.
                 */
                p_ssa_ctxt->bitmask |= ICF_SSA_UPDATE_WITH_SDP;

                /*The above API added the SDP msg body in the msg body list
                 * of the transaction. Now we need to add this to the 
                 * actual outgoing SIP Message*/               
    
                if (ICF_NULL == p_content_type)
                {
                    ret_val = ICF_FAILURE;
                }
                else if ( Sdf_co_fail == 
                        icf_ssa_SetAllMsgBody(
                            p_call_obj->pUacTransaction,\
			    p_overlap_txn,/*OverlapTxn*/(icf_int8_t *)p_content_type,\
                            &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
		/*Message body header changes Ends*/
                else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                            p_call_obj,
                            p_sip_msg, &dOptions,&sdf_error))
                {
                    /*Reset the bitmask for Update with SDP
		     */
		    if(ICF_SSA_UPDATE_WITH_SDP & p_ssa_ctxt->bitmask)
		    {
			p_ssa_ctxt->bitmask &= ~(ICF_SSA_UPDATE_WITH_SDP);
		    }
		    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
                    ret_val = ICF_FAILURE;
                }
                else
                {
					/* Fix for CSR 1-7397689: SPR 19747 */
					/* Copy the message body to persistent body list so
					 * that it can be used again to send SDP in request with
					 * credentials if the current request is challenged.
					 */
					if ( Sdf_co_fail == sdf_listDeleteAll(
                            &p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList,
                            &sdf_error))
                    {
                        sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, method, 
                            Sdf_en_uacTransaction);
                       /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                       ret_val = ICF_FAILURE;
                    }
                    else if ( Sdf_co_fail == sdf_listInit(
                            &(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
                            __sdf_fn_uaFreeMsgBody,Sdf_co_false, &sdf_error))
                    {
                       sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, method, 
                            Sdf_en_uacTransaction);
                       /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                       ret_val = ICF_FAILURE;
                    }
                    else if (Sdf_co_fail == sdf_ivk_uaCopyMsgBodyToPersistent (
                            &(p_ssa_ctxt->p_call_obj->pUacTransaction->slMsgBodyList),
                            &(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
                            (Sdf_ty_s8bit *)"persistent",&sdf_error))
                   	{
                       sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, method, 
                            Sdf_en_uacTransaction);
                       /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                       ret_val = ICF_FAILURE;
                   	}

                    if(icf_port_strcmp(method,(icf_uint8_t *)"UPDATE") == 0)
                    {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_requests.stat_outbound_req.stats_UPDATE);
                    }

                    else if(icf_port_strcmp(method,(icf_uint8_t *)"INVITE") == 0)

                    {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_requests.stat_outbound_req.stats_ReINVITE);
                    }
                    sdf_ivk_uaEndTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method, &sdf_error);
                }
            }
        }
    }
    /* Fix for Memleaks under SPR 11096
	 */
	if(ICF_NULL != p_content_type)
    {
       sdf_memfree(Sdf_mc_callHandlerMemId,
                    (Sdf_ty_pvoid *)&(p_content_type),&sdf_error);
    }

	if(Sdf_co_null != p_sdp_msg)
    {
        sip_freeSdpMessage(p_sdp_msg);
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
#endif /* ICF_SERVICE_ENABLE */

#ifdef ICF_SESSION_TIMER
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_set_min_se
 **
 ** DESCRIPTION:        This function sets the GlbMinSe of UATK
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_set_min_se(
				  icf_ssa_pdb_st *p_ssa_pdb)
{
	Sdf_st_error dErr;
	icf_cfg_ssa_set_min_se *p_internal_msg = ICF_NULL;

        icf_return_t    ret_val = ICF_SUCCESS;


	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb);
	p_internal_msg = (icf_cfg_ssa_set_min_se *)
					p_ssa_pdb->p_internal_msg->p_msg_data;
	sdf_fn_uaSetGlbMinSe(p_internal_msg->min_se,&dErr);
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb);
	return ret_val;
}
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_session_refresh_req
 **
 ** DESCRIPTION:        This function sends Re-INVITE/UPDATE when session
 **						timer expired and ICF acts as session timer refresher
 **						or when 422 response had received for session refresh
 **						request
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_session_refresh_req(
				   icf_ssa_pdb_st *p_ssa_pdb)
{
   icf_return_t                 ret_val = ICF_SUCCESS;

    icf_glb_pdb_st               *p_glb_pdb =
        p_ssa_pdb->p_glb_pdb;

    icf_call_ctx_st              *p_call_ctxt =
        p_glb_pdb->p_call_ctx;

    icf_ssa_ctxt_st              *p_ssa_ctxt = ICF_NULL;

    icf_config_data_st           *p_glb_cfg_data = 
        p_ssa_pdb->p_glb_cfg_data;

    icf_internal_msg_st          *p_internal_msg = p_ssa_pdb->p_internal_msg;

    icf_nw_otg_session_refresh_req_st *p_media_chng_req =
        (icf_nw_otg_session_refresh_req_st*)(p_internal_msg->p_msg_data);

    icf_uint8_t                  method[ICF_MAX_SIZE_INT_USAGE_STR],
    hold_option = ICF_SSA_NO_HOLD;

    Sdf_st_callObject               *p_call_obj = ICF_NULL;
    Sdf_st_error                    sdf_error;
    Sdf_st_overlapTransInfo         *p_overlap_txn = Sdf_co_null;
    SipMessage	                    *p_sip_msg = Sdf_co_null;
    SdpMessage                      *p_sdp_msg =Sdf_co_null ;
	Sdf_st_transportInfo			*pDestInfo = Sdf_co_null;
	Sdf_ty_s8bit					proto[ICF_MAX_SIZE_INT_USAGE_STR] = "\0";
    icf_header_list_st              tmp_hdr_list;
    /* Start: Addition for Per Line Features */
    icf_line_id_t                     line_id;
    icf_error_t                     ecode; 
    icf_line_data_st                *p_line_data;
    /* End: Addition for Per Line Features */

    if (ICF_NULL == p_call_ctxt)
    {
        return ICF_FAILURE;
    }
    ICF_ASSERT(p_glb_pdb, p_call_ctxt)

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_call_ctxt->p_sip_ctx);

    if (ICF_NULL == p_ssa_ctxt)
    {
       return ICF_FAILURE;
    }  
    p_call_obj = p_ssa_ctxt->p_call_obj;

    /* Start: Added for Obsolete Hold Parameter */
    if (ICF_FAILURE == icf_ssa_get_line_id_from_glb_pdb( p_glb_pdb,
                                                         p_ssa_ctxt,
                                                         &line_id,
                                                         &ecode ) )
     {
       ret_val = ICF_FAILURE;
     }
    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk( p_glb_pdb,
                                                     line_id,
                                                     (icf_line_data_st **)(&p_line_data),
                                                     &ecode ) )
     {
       ret_val = ICF_FAILURE;
     }
     if(ICF_SUCCESS == ret_val)
     p_glb_cfg_data->hold_option = p_line_data->hold_option;
     /* End : Added for Obsolete Hold Parameter */

	if(p_media_chng_req->refresher_req_type == ICF_SSA_REFRESHER_METHOD_INVITE){
		strcpy((icf_int8_t *)method,(const icf_int8_t *)"INVITE");
        if(p_ssa_ctxt->p_call_obj->pMediaInfo != ICF_NULL)
        {
            p_ssa_ctxt->p_call_obj->pMediaInfo->dMediaState = Sdf_en_idleMedia;
        }
	}
	else if(p_media_chng_req->refresher_req_type == ICF_SSA_REFRESHER_METHOD_UPDATE){
		strcpy((icf_int8_t *)method, (const icf_int8_t *)"UPDATE");
	}
	else
	{
		return ICF_FAILURE;
	}
	
/*	if(ICF_FAILURE == 
        icf_ssa_set_RouteHeaderListToInitData(p_ssa_pdb,
                p_ssa_ctxt->p_call_ctxt->p_preferred_identity,
               p_ssa_ctxt->p_call_ctxt->line_id,p_ssa_pdb->p_ecode))
    {
        
        ret_val = ICF_FAILURE;
    }
    */
	
    if (ICF_NULL != (p_ssa_pdb->p_glb_pdb->p_call_ctx->media_state &
					ICF_MEDIA_STATE_LOCAL_HELD))
    {
        if ((ICF_CFG_CC_MUTE_OPTION_INVITE_C_ZERO == 
                    p_glb_cfg_data->hold_option) ||
                (ICF_CFG_CC_MUTE_OPTION_UPDATE_C_ZERO == 
                 p_glb_cfg_data->hold_option))
        {
            hold_option = ICF_SSA_HOLD_CLINE_ZERO;
        }
        else if ((ICF_CFG_CC_MUTE_OPTION_INVITE_MODE_INACTIVE == 
                    p_glb_cfg_data->hold_option) ||
                (ICF_CFG_CC_MUTE_OPTION_UPDATE_MODE_INACTIVE == 
                 p_glb_cfg_data->hold_option))
        {
            hold_option = ICF_SSA_HOLD_MODE_SENDONLY;
        }
        else if ((ICF_CFG_CC_HOLD_OPTION_INV_WTH_MODE_SEND_ONLY_N_C_0 == 
                    p_glb_cfg_data->hold_option) ||
                (ICF_CFG_CC_HOLD_OPTION_UPD_WTH_MODE_SEND_ONLY_N_C_0 == 
                 p_glb_cfg_data->hold_option))
        {
            hold_option |= ICF_SSA_HOLD_MODE_SENDONLY;
            hold_option |= ICF_SSA_HOLD_CLINE_ZERO;
        }
        else
        {
            hold_option = ICF_SSA_HOLD_PORT_ZERO;
        }
    }

	if (Sdf_co_fail == sdf_ivk_uaStartTransaction(
                p_call_obj,
                &p_overlap_txn,
                (const icf_int8_t *)method,
                &sdf_error))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
	else if (Sdf_co_fail == 
            sdf_ivk_uaMakeTransaction((icf_int8_t *)method, 
                p_call_obj, 
                p_overlap_txn, 
                &sdf_error))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        sdf_ivk_uaClearTransaction(
                p_call_obj,
                p_overlap_txn,
                (const icf_int8_t *) method,
                Sdf_en_uacTransaction);
        ret_val = ICF_FAILURE;
    }
	
    if ( ICF_SUCCESS == ret_val)
    {
        SipOptions  dOptions;
        /* 
         * Flush the message bodies (if any) stored in the 
         * UAC transaction of the call object
         */
        sdf_listDeleteAll(
                &(p_call_obj->pUacTransaction->slMsgBodyList),&sdf_error);

		if (sdf_ivk_uaGetDestInfoFromTransaction(p_call_obj->pUacTransaction,\
				p_overlap_txn, &pDestInfo, &sdf_error) != Sdf_co_fail)
		{
			Sdf_ty_protocol         dTempScheme = Sdf_en_protoNone;

			if(pDestInfo->dScheme == Sdf_en_protoNone)
				dTempScheme = p_call_obj->pCommonInfo->dScheme;
			else
				dTempScheme = pDestInfo->dScheme;

			if(dTempScheme == Sdf_en_protoTcp)
			{
				icf_port_strcpy((icf_uint8_t *)proto, (icf_uint8_t *)"TCP");
			}
			else if(dTempScheme == Sdf_en_protoUdp)
			{
				icf_port_strcpy((icf_uint8_t *)proto, (icf_uint8_t *)"UDP");
			}
            sdf_ivk_uaFreeTransportInfo(pDestInfo);
		}

        dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;      

        if ( ICF_FAILURE == 
                icf_ssa_form_sdp_from_stream_sdp_info(
                    &p_sdp_msg,
                    p_ssa_pdb,
                    hold_option,
                    &sdf_error))
        {
            sdf_ivk_uaClearTransaction(
                    p_call_obj,
                    p_overlap_txn,
                    (const icf_int8_t *) method,
                    Sdf_en_uacTransaction);
            ret_val = ICF_FAILURE;
        }

		if (ICF_FAILURE != ret_val)
		{        
            /* 
             * If method is UPDATE set SDP in overlapped transaction
             * else set SDP in UAC transaction
             */
            if (0 == icf_port_strcmp(method,(icf_uint8_t *)"UPDATE"))
            {
                
                p_sip_msg = p_overlap_txn->pSipMsg;                
            }
            else
            {
                if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
                            p_call_obj->pUacTransaction, p_sdp_msg, 
                            &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
		/*Message body header changes Start*/
                else if ( Sdf_co_fail == 
                    icf_ssa_SetAllMsgBody(
                    p_call_obj->pUacTransaction,p_overlap_txn,/*OverlapTxn*/\
		    "application/sdp",&sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                        p_overlap_txn, (const icf_int8_t *)method,
                        Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
		/*Message body header changes Ends*/
                else
                {
                    p_sip_msg = p_call_obj->pUacTransaction->pSipMsg;
                }
            }

            /* Insert persistent header list in Re-INVITE or UPDATE for Session-Refresh */
            tmp_hdr_list.count = ICF_NULL;
            tmp_hdr_list.hdr = ICF_NULL;

            /* Insert P-Preferred-Identity header in Re-INVITE/UPDATE message */
            if (ICF_NULL != p_call_ctxt->p_preferred_identity)
            {
		        if (ICF_FAILURE == icf_ssa_form_and_insert_pref_identity_hdr(
			                        p_ssa_pdb, p_sip_msg))
		        {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
			        ret_val = ICF_FAILURE;
		        }
            }

			/* Fix for CSR 1-7397689: SPR 19747 */
			/* Copy the message body to persistent body list so
			 * that it can be used again to send SDP in request with
			 * credentials if the current request is challenged.
			 */
			if((ICF_SUCCESS == ret_val)&&
							(0 == icf_port_strcmp(method, (icf_uint8_t*)"INVITE")))
			{
				if ( Sdf_co_fail == sdf_listDeleteAll(
                            &p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList,
                           &sdf_error))
				{
					sdf_ivk_uaClearTransaction(p_call_obj,
                           p_overlap_txn, (const icf_int8_t *)method,
                           Sdf_en_uacTransaction);
					/*Raise a Major error here - UATK_PROCESSING FAILURE*/
					ret_val = ICF_FAILURE;
				}
				else if ( Sdf_co_fail == sdf_listInit(
								&(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
								__sdf_fn_uaFreeMsgBody,Sdf_co_false, &sdf_error))
				{
						sdf_ivk_uaClearTransaction(p_call_obj,
										p_overlap_txn, (const icf_int8_t *)method,
										Sdf_en_uacTransaction);
						/*Raise a Major error here - UATK_PROCESSING FAILURE*/
						ret_val = ICF_FAILURE;
				}
				else if (Sdf_co_fail == sdf_ivk_uaCopyMsgBodyToPersistent (
									&(p_ssa_ctxt->p_call_obj->pUacTransaction->slMsgBodyList),
									&(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
									(Sdf_ty_s8bit *)"persistent",&sdf_error))
				{
						sdf_ivk_uaClearTransaction(p_call_obj,
										p_overlap_txn, (const icf_int8_t *)method,
										Sdf_en_uacTransaction);
                        /*Raise a Major error here - UATK_PROCESSING FAILURE*/
						ret_val = ICF_FAILURE;
				}
			}

            if(ICF_FAILURE == ret_val)
            {
                /* do nothing */
            }
            /* Insert P-Access-Network-Info header in Re-INVITE/UPDATE message */
		    else if (ICF_FAILURE == icf_ssa_form_and_insert_access_nw_info_hdr(
				                    p_ssa_pdb, p_sip_msg))
		    {
                sdf_ivk_uaClearTransaction(p_call_obj,
                        p_overlap_txn, (const icf_int8_t *)method,
                        Sdf_en_uacTransaction);
			    ret_val = ICF_FAILURE;
		    }
            else if (ICF_FAILURE == icf_cmn_clone_header_list(
                                            p_ssa_pdb->p_glb_pdb,
                                            &(p_call_ctxt->persistent_header_list),
                                            &tmp_hdr_list,
                                            p_ssa_pdb->p_ecode))
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == icf_ssa_insert_header_list_in_sip_msg(
                                        p_ssa_pdb,
                                        &tmp_hdr_list,
                                        ICF_TRUE,
                                        p_sip_msg))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {                
                if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                            p_call_obj,
                            p_sip_msg, &dOptions,&sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* Stat collection of UPDATE and RE_INVITE sent*/
                    if(icf_port_strcmp(method,(icf_uint8_t *)"UPDATE") == 0)
                    {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_requests.stat_outbound_req.stats_UPDATE);
                    }

                    else if(icf_port_strcmp(method,(icf_uint8_t *)"INVITE") == 0)
                    {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_requests.stat_outbound_req.stats_ReINVITE);
                    }
                    sdf_ivk_uaEndTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method, &sdf_error);
                }
            }
        }
    }
    if(Sdf_co_null != p_sdp_msg)
    {
        sip_freeSdpMessage(p_sdp_msg);
    }

    if(ICF_SUCCESS == ret_val)
    {
        p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->signalling_ongoing = 
            ICF_INT_MODULE_CC;
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
#endif
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_redirect
 **
 * DESCRIPTION:     The following function sends 302 message to peer
 *                  corresponding to call redirection.
 *                  1.The diversion information along with the address
 *                  to whom the call needs to be forwarded is populated 
 *                  in the call context.
 *                  2.The Contact header in the 302 response is populated
 *                  on the basis of this information.If the addr type is 
 *                  E164 then the proxy address is appended to the addr.
 *                  3.The diversion headers are also populated and the response
 *                  sent to peer.  
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_redirect(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_line_data_st         *p_line_data = ICF_NULL;

    icf_glb_pdb_st             *p_glb_pdb=
        p_ssa_pdb->p_ssa_ctxt->p_glb_pdb;
    icf_diversion_info_st      *p_div_info = ICF_NULL;

    /*
     * Following holds the address to which the call is being forwarded/
     * redirected. This will be passed to SSA from the invoking SLP
     */
    Sdf_ty_slist            contact_list;

    SipOptions              dOptions;
    Sdf_st_error           sdf_error;
    Sdf_st_callObject       *p_call_obj =
        p_ssa_pdb->p_ssa_ctxt->p_call_obj;
    SipError                sip_error;
    SipHeader               *p_contact_hdr = SIP_NULL;
    SipAddrSpec             *p_addr_spec = SIP_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    p_div_info = p_glb_pdb->p_call_ctx->p_diversion_header_list;

    dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    /*Get line data, we may need it to get the proxy address*/
    if ( ICF_FAILURE == 
            icf_dbm_fetch_line_cntxt_blk(
                p_glb_pdb, p_glb_pdb->p_call_ctx->line_id,
                &p_line_data, p_ssa_pdb->p_ecode))
    {
        /*Raise a Major Error Here- CFG_DATA_NOT_FOUND*/
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failed to get line data from DBM"));
        return ICF_FAILURE;
    }
    else
    {
        p_ssa_pdb->p_line_data = p_line_data;
    }
    if ( ICF_NULL == p_div_info)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:No Diversion Info, returning Failure"));
        return ICF_FAILURE;
    }
    if ( Sdf_co_fail == sdf_listInit(&contact_list,
                __sdf_fn_uaFreeCompleteHeader,Sdf_co_false, &sdf_error))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInContactList Init"));
        ret_val = ICF_FAILURE;
    }
    else if (SipFail == sip_initSipHeader(&p_contact_hdr,
                SipHdrTypeContactNormal, &sip_error))
    {
        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInContactHeader Init"));
        ret_val = ICF_FAILURE;
    }
    else 
    {
        ICF_ASSERT(p_glb_pdb, (p_div_info->p_contact_addr))
        if ((ICF_ADDRESS_PLAN_SIP_URI == 
                p_div_info->p_contact_addr->addr_type) || 
            (ICF_ADDRESS_PLAN_E164 == 
                p_div_info->p_contact_addr->addr_type))
        {
            if ( SipFail == sip_initSipAddrSpec(&p_addr_spec, 
                        SipAddrSipUri, &(sip_error)))
            {
                ICF_PRINT(((icf_uint8_t *)"\nFailure sip_initSipAddrSpec for Contact"));
                sip_freeSipHeader(p_contact_hdr);
                ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_contact_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
                
                ret_val = ICF_FAILURE;
            }
            else
            {
                SipUrl          *p_sip_url = SIP_NULL;
                icf_uint8_t  *p_temp = ICF_NULL,
                    user_name[ICF_MAX_STR_LEN] = "\0", 
                    host_name[ICF_MAX_STR_LEN] = "\0",
                    str[ICF_MAX_STR_LEN] = "\0";
                Sdf_ty_u16bit      port_num = 0;

                if (ICF_ADDRESS_PLAN_E164 == p_div_info->p_contact_addr->addr_type)
                {
                    icf_port_strcpy(user_name, p_div_info->p_contact_addr->addr_val.str);
                    if (ICF_NULL == (ICF_CFG_LINE_DATA_PROXY_RECD && 
                                p_ssa_pdb->p_line_data->line_data_received))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:LineData Not Configured"));
                        sip_freeSipAddrSpec(p_addr_spec);
                        return ICF_FAILURE;
                    }
                    if ( ICF_FAILURE == 
                            icf_ssa_get_hostname_str_frm_trans_addr(p_ssa_pdb, 
                                &(p_ssa_pdb->p_line_data->proxy_addr), host_name))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in AddressConversion from Self"));
                        sip_freeSipHeader(p_contact_hdr);
                        ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_contact_hdr,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            ret_val)
                        ret_val = ICF_FAILURE;
                        sip_freeSipAddrSpec(p_addr_spec);
                    }
                    else
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Success in AddressConversion from Self"));
                        port_num = (Sdf_ty_u16bit)ICF_SSA_DEFAULT_SIP_PORT_NUM;
                    }
                }
                else
                {
                    /*SipUrl so we parse to get userid, hostname and portnum*/
                    icf_port_strcpy(str, p_div_info->p_contact_addr->addr_val.str);
                    p_temp = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)str,(const icf_int8_t *)"@");
                    if(ICF_NULL != p_temp)
					{
						icf_port_strcpy(user_name, p_temp);
                    	p_temp = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)0,(const icf_int8_t *)":");
					}

                    if ( ICF_NULL == p_temp)
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:InvalidContactAddress in OTG redirection"));
                        sip_freeSipHeader(p_contact_hdr);
                        ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_contact_hdr,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            ret_val)
                        sip_freeSipAddrSpec(p_addr_spec);
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        icf_port_strcpy(host_name, p_temp);
                        p_temp = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)0,(const icf_int8_t *)":");
                        if (ICF_NULL == p_temp)
                        {
                            port_num = (Sdf_ty_u16bit)ICF_SSA_DEFAULT_SIP_PORT_NUM;
                        }
                        else
                        {
                            port_num = (Sdf_ty_u16bit)(icf_port_atoi((const icf_int8_t *)p_temp));
                        }
                    }
                }
                if (SipFail == sip_initSipUrl(&p_sip_url, &sip_error))
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in SipUrl init"));
                    sip_freeSipHeader(p_contact_hdr);
                    ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_contact_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
                    sip_freeSipAddrSpec(p_addr_spec);
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    sip_setUserInUrl(p_sip_url, Sdf_mc_strdupSdp((const icf_int8_t *)user_name), 
                            &sip_error);
                    sip_setHostInUrl(p_sip_url, Sdf_mc_strdupSdp((const icf_int8_t *)host_name), 
                            &sip_error);
                    if (0 != port_num)
                    {
                        sip_setPortInUrl(p_sip_url, (Sdf_ty_u16bit)port_num, \
                                &sip_error);
                    }
                    if (SipFail == sdf_ivk_uaSetUrlInAddrSpec(p_addr_spec, 
                                        p_sip_url, &sdf_error))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure to set Url in addr"));
                        sip_freeSipHeader(p_contact_hdr);
                        ICF_MEMFREE(
                            p_ssa_pdb->p_glb_pdb,
                            p_contact_hdr,
                            ICF_MEM_COMMON,
                            p_ssa_pdb->p_ecode,
                            ret_val)
                        sip_freeSipAddrSpec(p_addr_spec);
                        ret_val = ICF_FAILURE;
                    }
                }
                sip_freeSipUrl(p_sip_url);
            }/*InitAddrSpec successful*/
        }/*block for handling of Sip Uri E164*/
        else if (ICF_ADDRESS_PLAN_TEL_URI ==
                p_div_info->p_contact_addr->addr_type)
        {
            icf_string_st    contact_addr_str;
            icf_cmn_init_string(&contact_addr_str);
            icf_port_strcpy(contact_addr_str.str,(icf_uint8_t *)"tel:");
            icf_port_strcat(contact_addr_str.str,
                    p_div_info->p_contact_addr->addr_val.str);
            contact_addr_str.str_len = (icf_uint16_t )icf_port_strlen(
                    contact_addr_str.str);

            if ( SipFail == sip_initSipAddrSpec(&p_addr_spec, 
                        SipAddrReqUri, &(sip_error)))
            {
                ICF_PRINT(((icf_uint8_t *)"\nFailure sip_initSipAddrSpec for Contact"));
                sip_freeSipHeader(p_contact_hdr);
                ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_contact_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
                ret_val = ICF_FAILURE;
            }
            else
            {
                if(SipFail == sip_setUriInAddrSpec(p_addr_spec,
                            Sdf_mc_strdupCallHandler(
                                (const icf_int8_t *)p_div_info->p_contact_addr->addr_val.str),
                            &sip_error))
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in Setting telURI in AddrSpec"));
                    sip_freeSipHeader(p_contact_hdr);
                    ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_contact_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
                    sip_freeSipAddrSpec(p_addr_spec);
                    ret_val = ICF_FAILURE;
                }
                p_call_obj->pCallInfo->dContactAddrType = SipAddrReqUri;
            }
        }
        else
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:NoValidContactAddressType!!"));
            sip_freeSipHeader(p_contact_hdr);
                ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_contact_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
            ret_val = ICF_FAILURE;
        }
        if ( ICF_FAILURE != ret_val)
        {
            /* Now insert the params into the addr spec */
            if (ICF_FAILURE == 
                    icf_ssa_insert_params_frm_address_st(
                        p_ssa_pdb,p_addr_spec,
                        p_div_info->p_contact_addr))
            {
                sip_freeSipHeader(p_contact_hdr);
                ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_contact_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == sip_setAddrSpecInContactHdr(p_contact_hdr, p_addr_spec, \
                        &sip_error))
            {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:NoValidContactAddressType!!"));
                sip_freeSipHeader(p_contact_hdr);
                ICF_MEMFREE(
                        p_ssa_pdb->p_glb_pdb,
                        p_contact_hdr,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
                ret_val = ICF_FAILURE;
            }
            sip_freeSipAddrSpec(p_addr_spec);
        }

        /*Now we add the contact header into the contact header list*/
       sdf_listAppend(&contact_list, p_contact_hdr, &sdf_error);
    }/*Contact Header addition ends*/
    if ( ICF_FAILURE != ret_val)
    {
        if(Sdf_co_fail == sdf_ivk_uaFormRedirectResponse( 302, "INVITE", \
                    &contact_list, p_call_obj, &sdf_error))
        {
            /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
            sip_freeSipHeader(p_contact_hdr);
            sdf_listDeleteAll(&contact_list, &sdf_error);
            ret_val = ICF_FAILURE;
        }
        else if ( ICF_FAILURE == icf_ssa_form_and_insert_div_hdr(
                    p_ssa_pdb,
                    p_call_obj->pUasTransaction->pSipMsg))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureInDiversionHeaderPopulation"));
            sip_freeSipHeader(p_contact_hdr);
            sdf_listDeleteAll(&contact_list, &sdf_error);
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,p_call_obj,
                    p_call_obj->pUasTransaction->pSipMsg, \
                    &dOptions,&sdf_error))
        {
            /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
            sip_freeSipHeader(p_contact_hdr);
            sdf_listDeleteAll(&contact_list, &sdf_error);
            ret_val = ICF_FAILURE;
        }
        else
        {
            sip_freeSipHeader(p_contact_hdr);
            sdf_listDeleteAll(&contact_list, &sdf_error);
        }
    }
#ifdef ICF_STAT_ENABLE
	if(ret_val != ICF_FAILURE)
	{
		icf_ssa_invite_resp_stats(p_ssa_pdb,302,1);
	}
#endif
    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_notify_resp
 * DESCRIPTION:     Currently this is used for sending response to 
 *                  Notify messages in context of Refer procedures only .
 *                  1.Fetch the overlapped transaction corresponding
 *                  to the NOTIFY message received.
 *                  2.Form the 200 OK response to the NOTIFY and send the 
 *                  same to peer.
 * 
 *****************************************************************************/
icf_return_t icf_ssa_process_notify_resp(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;

    Sdf_st_error                sdf_error;
    SipOptions                  options;
    Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;
    icf_nw_otg_notify_resp_st *p_notify_resp = ICF_NULL;
    icf_uint16_t              resp_code = ICF_NULL;
	Sdf_st_callObject       *p_call_obj = ICF_NULL;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

/*Check if NOTIFY resp is to be sent for REFER then fetch SSA
     * context from call context otherwise fetch it from SUBM context*/
    if (ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx)
    {
        p_ssa_ctxt = (icf_ssa_ctxt_st*)
            (p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx);

        resp_code = 200;
    }
    else
    {   
  /*NOTIFY_OUT_OF_DIALOG_START*/
		/*
        p_ssa_ctxt = ((icf_nw_otg_notify_resp_st *)
		(p_ssa_pdb->p_internal_msg->p_msg_data))->p_sic_ctx->p_ssa_ctx;
        
        resp_code = 200;
		*/

		p_notify_resp = (icf_nw_otg_notify_resp_st *)p_ssa_pdb->p_internal_msg->p_msg_data;
        p_ssa_ctxt = p_notify_resp->p_sic_ctx->p_ssa_ctx;
		resp_code = (icf_uint16_t)p_notify_resp->response_code;

/*NOTIFY_OUT_OF_DIALOG_END*/
    }

    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_ssa_ctxt)
	p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

		    /*
     * The overlapped transcations are maintained as a list
     * in the call object.Fetch the matching overlapped
     * transaction on the basis of the message type and
     * the transaction state
     */
    if(ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_notify,Sdf_en_transactionReceived,
                p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
       if((p_ssa_ctxt->bitmask_1 & ICF_MSG_RECVD_OUT_OF_DIALOG) &&
          (ICF_SUCCESS == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_notify,Sdf_en_idle,
                p_ssa_ctxt->p_call_obj,&p_ovtxn)))
       {
           /*Do nothing: txn matched*/ 
       }
       else
       {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA}:FailureInTransactionRetreivalForNOTIFY-RESP"));
           ret_val = ICF_FAILURE;
       }
    }
    
    if ((ICF_FAILURE == ret_val)||
        (Sdf_co_fail == sdf_ivk_uaFormResponse(
                /*200,*/
                resp_code,
                (Sdf_ty_s8bit *)"NOTIFY", 
                p_ssa_ctxt->p_call_obj,
                p_ovtxn,
                Sdf_co_false, 
                &sdf_error)))
    {
        /*Raise/SetError:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ovtxn->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Specify the stack options */
        options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        /*  (0 for UDP, 1 for TCP) */
        if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_ssa_ctxt->p_call_obj,
                    p_ovtxn->pSipMsg, 
                    &options,&sdf_error))
        {
            /*Raise/SetError:UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE;
        }
        else 
        {
#ifdef ICF_STAT_ENABLE
            icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"NOTIFY",resp_code,1);
#endif

            if ((ICF_NULL != p_ssa_ctxt->p_sic_ctxt) &&
                    (ICF_TRUE ==
                     ICF_IS_BIT_SET(p_ssa_ctxt->p_sic_ctxt->bitmask,ICF_SIC_METHOD_TYPE_OOD_NOTIFY)))
            {
                p_call_obj = p_ssa_ctxt->p_call_obj;
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
        }
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_message_resp
 
 *****************************************************************************/
icf_return_t icf_ssa_process_message_resp(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;

    Sdf_st_error                sdf_error;
    SipOptions                  options;
    Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    p_ssa_ctxt = ((icf_nw_otg_message_resp_st *)
			(p_ssa_pdb->p_internal_msg->p_msg_data))->p_sic_ctx->p_ssa_ctx;

    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, p_ssa_ctxt)

    if( Sdf_co_fail == sdf_listGetAt(
                &(p_ssa_ctxt->p_call_obj->slOverlapTransInfo),
                0,
                (Sdf_ty_pvoid)&p_ovtxn,
                &sdf_error))
    {
        /*Raise/SetError:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (Sdf_co_fail == sdf_ivk_uaFormResponse(
                200,
                (Sdf_ty_s8bit *)"MESSAGE", 
                p_ssa_ctxt->p_call_obj,
                p_ovtxn,
                Sdf_co_false, 
                &sdf_error))
    {
        /*Raise/SetError:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Specify the stack options */
        options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        /*  (0 for UDP, 1 for TCP) */
        if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_ssa_ctxt->p_call_obj,
                    p_ovtxn->pSipMsg, 
                    &options,&sdf_error))
        {
            /*Raise/SetError:UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE;
        }
        else
        {
                p_ssa_ctxt->p_sic_ctxt = ICF_NULL;
                /* Free the call object*/ 
                sdf_ivk_uaFreeCallObject(&(p_ssa_ctxt->p_call_obj));
        }

    }
#ifdef ICF_STAT_ENABLE
		if(ret_val!=ICF_FAILURE)
        {
            icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"MESSAGE",200,1);
        }
#endif

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_notify
  * DESCRIPTION:     This function is used to send the NOTIFY messages
 *                  to peer.These NOTIFY's are corresponding to REFER only.
 *                  1.The following need to be populated in the NOTIFY message
 *                      a.Subscription state header
 *                      b.Event header
 *                      c.Message fragment body
 *                  2.Depending on the status indicated by the SLP,the
 *                  SIP frag body is constructed with status code as 200
 *                  or 503(Service Unavailable)
 *                  3.Invoke the sdf_ivk_uaFormNotify API which takes the
 *                  event header(refer) and the SIP frag body as parameters.
 *                  This API populates the Subcription state header on the
 *                  basis of the resp code in the SIP frag body.
 *                  4.Send the NOTIFY message to peer.
 * 
 *****************************************************************************/
icf_return_t icf_ssa_process_notify(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_glb_pdb_st          *p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;

    Sdf_ty_u16bit           frag_body_resp_code = 200;
    Sdf_st_error            sdf_error;
    Sdf_st_overlapTransInfo *p_overlap_trans_info = Sdf_co_null;
    SipOptions              options;
    SipMessage              *pRespMessage=Sdf_co_null;
    SipHeader               *p_event_hdr = Sdf_co_null, \
                            *p_sub_state_hdr = Sdf_co_null, \
                            *p_content_type_hdr = Sdf_co_null;
    Sdf_ty_slist             slMsgBodyList;
    Sdf_ty_u32bit            dIndex = 0, dCount = 0;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_glb_pdb->p_call_ctx->p_sip_ctx);

    ICF_ASSERT(p_glb_pdb, p_ssa_ctxt)
    ICF_ASSERT(p_glb_pdb, (p_ssa_ctxt->p_call_obj))
  

    if ( ICF_FALSE == ((icf_nw_otg_notify_st*)
                (p_ssa_pdb->p_internal_msg->p_msg_data))->status)
    {
        frag_body_resp_code = 503;
    }
       /* Added for sending traces */
    p_ssa_pdb->resp_code=frag_body_resp_code;
    ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_NOTIFY_FOR_REFER_SENT)

    if(ICF_FAILURE == icf_ssa_formStatusLineMessage(
                &pRespMessage,frag_body_resp_code,
                &sdf_error))
    {
        ret_val = ICF_FAILURE ;
    }
    else if (Sdf_co_fail == sdf_ivk_uaStartTransaction(p_ssa_ctxt->p_call_obj,
                &p_overlap_trans_info, "NOTIFY", &sdf_error))
    {
        /*Raise Alarm/Error UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if(sdf_ivk_uaFormNotify(p_ssa_ctxt->p_call_obj, p_overlap_trans_info,
                (Sdf_ty_s8bit*)"refer",
                pRespMessage,&sdf_error)== Sdf_co_fail)
    {
        sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                p_overlap_trans_info,"NOTIFY",
                Sdf_en_uacTransaction);
        ret_val = ICF_FAILURE;
    }
    else 
    {
        /*  Fix for CSR 1-7618644 : SPR 19989 */
        /*  Copy the Event header, Subscription state header and Content-Tyype
         *  header into persistent header list of call object.
         */
        if(SipFail == sip_initSipHeader(
                    (SipHeader**)(&p_event_hdr),
                    SipHdrTypeAny,\
                    (SipError*)&(sdf_error.stkErrCode)))
        {
            /*Raise/Ser Error:UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE;
        }
        else if (SipFail == sip_getHeader(p_overlap_trans_info->pSipMsg,SipHdrTypeEvent, \
                p_event_hdr, (SipError *)&(sdf_error.stkErrCode)))
        {
            ret_val = ICF_FAILURE;
        }
        else if(Sdf_co_fail == icf_ssa_insert_hdr_in_persistent_list_in_call_obj(
                            p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                            p_event_hdr,
                            SipHdrTypeEvent,
                            (Sdf_ty_s8bit *)"uat-Event",
                            &sdf_error))
        {
            sip_freeSipHeader(p_event_hdr);
            ret_val = ICF_FAILURE;
        }
        else if(SipFail == sip_initSipHeader(
                    (SipHeader**)(&p_sub_state_hdr),
                     SipHdrTypeAny,\
                    (SipError*)&(sdf_error.stkErrCode)))
        {
            /*Raise/Ser Error:UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE;
        }
        else if (SipFail == sip_getHeader(p_overlap_trans_info->pSipMsg,\
                    SipHdrTypeSubscriptionState, \
                    p_sub_state_hdr, (SipError *)&(sdf_error.stkErrCode)))
        {
            sip_freeSipHeader(p_sub_state_hdr);
            ret_val = ICF_FAILURE;
        }
        else if(Sdf_co_fail == icf_ssa_insert_hdr_in_persistent_list_in_call_obj(
                            p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                            p_sub_state_hdr,
                            SipHdrTypeSubscriptionState,
                            (Sdf_ty_s8bit *)"uat-Subscriptionstate",
                            &sdf_error))
        {
            sip_freeSipHeader(p_sub_state_hdr);
            ret_val = ICF_FAILURE;
        }
        else if(SipFail == sip_initSipHeader(
                    (SipHeader**)(&p_content_type_hdr),
                     SipHdrTypeAny,\
                    (SipError*)&(sdf_error.stkErrCode)))
        {
            /*Raise/Ser Error:UATK_PROCESSING_FAILURE*/
            ret_val = ICF_FAILURE;
        }
        else if (SipFail == sip_getHeader(p_overlap_trans_info->pSipMsg,\
                    SipHdrTypeContentType, \
                    p_content_type_hdr, (SipError *)&(sdf_error.stkErrCode)))
        {
            sip_freeSipHeader(p_content_type_hdr);
            ret_val = ICF_FAILURE;
        }
        else if(Sdf_co_fail == icf_ssa_insert_hdr_in_persistent_list_in_call_obj(
                    p_ssa_pdb->p_ssa_ctxt->p_call_obj,
                    p_content_type_hdr,
                    SipHdrTypeContentType,
                    (Sdf_ty_s8bit *)"uat-contenttype",
                    &sdf_error))
        {
            sip_freeSipHeader(p_content_type_hdr);
            ret_val = ICF_FAILURE;
        }


        else
        {
            /*  Fix for CSR 1-7618644 : SPR 19989 */
            /*  get the message body from Sip message and insert it into a 
             *  message list and set this message list into persistent message 
             *  list present in call object.
             */
            if (sdf_listInit(&slMsgBodyList,\
                        __sip_freeSipMsgBody, Sdf_co_false, &sdf_error) == Sdf_co_fail)
            {
                sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                        p_overlap_trans_info, "NOTIFY", 
                        Sdf_en_uacTransaction);
                ret_val = ICF_FAILURE;
            }
            else
            {
                if (SipFail ==
                    sip_getMsgBodyCount(p_overlap_trans_info->pSipMsg, &dCount,
                            (SipError *)&(sdf_error.stkErrCode)))
                {
                    sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                            p_overlap_trans_info, "NOTIFY", 
                            Sdf_en_uacTransaction);
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    for (dIndex=0; dIndex<dCount; dIndex++)
                    {
                        SipMsgBody      *pMsgBody = Sdf_co_null;
                        if (SipFail ==
                            sip_getMsgBodyAtIndex(p_overlap_trans_info->pSipMsg,\
                            &pMsgBody, dIndex, \
                            (SipError *)&(sdf_error.stkErrCode)))
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }

                        if (sdf_listAppend(&slMsgBodyList, (Sdf_ty_pvoid)pMsgBody, \
                                    &sdf_error) == Sdf_co_fail)
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }

                    }
                    if (ICF_FAILURE == ret_val)
                    {
                        sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                                p_overlap_trans_info, "NOTIFY", 
                                Sdf_en_uacTransaction);
                    }
                    else
                    {
                        /* Copy the message body to persistent body list so
                         * that it can be used again to send Msg body in request with
                         * credentials if the current request is challenged.
                         */
                        if ( Sdf_co_fail == sdf_listDeleteAll(
                                    &p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList,
                                    &sdf_error))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else if ( Sdf_co_fail == sdf_listInit(
                                    &(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
                                    __sdf_fn_uaFreeMsgBody,Sdf_co_false, &sdf_error))
                        {
                            ret_val = ICF_FAILURE;
                        }
                        else if (Sdf_co_fail == sdf_ivk_uaCopyMsgBodyToPersistent (
                                    &slMsgBodyList,
                                    &(p_ssa_ctxt->p_call_obj->slPersistentMsgBodyList),
                                    (Sdf_ty_s8bit *)"persistent",&sdf_error))
                        {
                            ret_val = ICF_FAILURE;

                        }
                        sdf_listDeleteAll(&slMsgBodyList,
                                &sdf_error);
                        if (ICF_FAILURE == ret_val)
                        {
                            sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                                    p_overlap_trans_info, "NOTIFY", 
                                    Sdf_en_uacTransaction);
                        }
                    }
                }
            }
            if (ICF_SUCCESS == ret_val)
            {
        options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_ssa_ctxt->p_call_obj,
                    p_overlap_trans_info->pSipMsg,
                    &options,&sdf_error))
        {
            sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlap_trans_info,"NOTIFY",
                    Sdf_en_uacTransaction);
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in ssa_send_to_peer for NOTIFY"));
            ret_val = ICF_FAILURE;
        }
        else
        {
            sdf_ivk_uaEndTransaction(p_ssa_ctxt->p_call_obj,
                    p_overlap_trans_info, "NOTIFY",&sdf_error);
        }
    }
        }
    }
    if (Sdf_co_null != pRespMessage)
    {
        sip_freeSipMessage(pRespMessage);
    }
    /* Stat collection of NOTIFY sent*/ 
    if( ret_val != ICF_FAILURE)
	{
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_outbound_req.stats_NOTIFY)
	}
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_refer_resp
 **
 * DESCRIPTION:     This function sends the response to the REFER request
 *                  1.Fetch the overlapped transaction from the list of
 *                  overlapped transactions in the call object.
 *                  2.Map the response code if a failure response needs
 *                  to be sent to peer.
 *                  3.Construct the response and send the same to peer.
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_refer_resp(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;

    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st       *p_glb_pdb =
        p_ssa_pdb->p_glb_pdb;

    icf_internal_msg_st  *p_internal_msg =
        p_ssa_pdb->p_internal_msg;

    icf_nw_otg_refer_resp_st     *p_otg_refer_resp =
        ICF_NULL;

    Sdf_ty_u16bit           sdf_resp_code = 202;
    Sdf_st_error            error;
    SipOptions              options;
    Sdf_st_overlapTransInfo *p_ovtxn = Sdf_co_null;

    ICF_ASSERT(p_glb_pdb, p_internal_msg)

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb) /* function entry trace */
    p_glb_pdb = p_glb_pdb;

    p_ssa_ctxt = (icf_ssa_ctxt_st*)
        (p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx);
    /*SPR17667 : In case SSA CTX is null set ret_val as 
      failure for no more processing */
    if(ICF_NULL == p_ssa_ctxt)
    {
        ret_val = ICF_FAILURE;
    }
    p_otg_refer_resp = (icf_nw_otg_refer_resp_st*)
        (p_internal_msg->p_msg_data);
    
    if ( ICF_NULL != 
            (ICF_NW_OTG_REFER_RESP_REASON & 
             p_otg_refer_resp->bitmask))
    {
        if(ICF_NULL != p_otg_refer_resp->resp_code)
        {
            sdf_resp_code = (icf_uint16_t )p_otg_refer_resp->resp_code;
        }
        else if(p_otg_refer_resp->reason == 
                ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ)
        {
            sdf_resp_code = 403;
        }
        else if(p_otg_refer_resp->reason == 
                ICF_CAUSE_TIMER_EXPIRY)
        {
            sdf_resp_code = 486;
        }
        else
        {
            if (ICF_FAILURE ==  icf_ssa_map_cc_reason_code(
                        p_otg_refer_resp->reason, &sdf_resp_code))
            {
                /*Raise/SetError:REASON_CODE_NOT_MAPPING*/
                ret_val = ICF_FAILURE;
            }
        }
    }
    if ( ICF_FAILURE != ret_val)
    {
        /*
         * The overlapped transcations are maintained as a list
         * in the call object.Fetch the matching overlapped 
         * transaction on the basis of the message type and
         * the transaction state
         */
        if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                    p_ssa_pdb,Sdf_en_refer,Sdf_en_transactionReceived,
                    p_ssa_ctxt->p_call_obj,&p_ovtxn))
        {
            /*Raise MajorError - NO TXN TO BE RESPONDED TO*/
            ret_val = ICF_FAILURE ;
        } /* if (count == list_size) */
        else
        {
            if (Sdf_co_fail == sdf_ivk_uaFormResponse(
                sdf_resp_code,
                (Sdf_ty_s8bit *)"REFER", 
                p_ssa_ctxt->p_call_obj,
                p_ovtxn,
                Sdf_co_false, 
                &error))
            {
                /*Raise/SetError:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE == 
                icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                    (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                    p_ovtxn->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                /* Specify the stack options */
                options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
                /*  (0 for UDP, 1 for TCP) */
                if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                    p_ssa_ctxt->p_call_obj,
                    p_ovtxn->pSipMsg, 
                    &options,&error))
                {
                    /*Raise/SetError:UATK_PROCESSING_FAILURE*/
                    ret_val = ICF_FAILURE;
                }
            } /* else */
        }
    }
#ifdef ICF_STAT_ENABLE
		else
        {
            icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"REFER",sdf_resp_code,1);
        }
#endif
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_process_otg_reg_req
 *
 * DESCRIPTION:     This function processes request from REGM sub-module to 
 *                  send REG/DE-REG request on network.
 *                  1.If the regstn duration is not 0 and the SSA ctxt
 *                  present in the rgm_context is NULL,it is a fresh
 *                  registration request,else it is for de-regstn or
 *                  re-regstn
 *                  2.Initial registration request is never sent with
 *                  authentication.If the registrar responds with 401/407
 *                  we construct the regst message with authentication 
 *                  parameters.
 *                  3.If this is an initial regstn request
 *                      a.Allocate SSA ctxt.
 *                      b.Allocate new call object
 *                      c.Push the SSA ctxt into the AppData of
 *                        the call object
 *                    else
 *                      use the existing call object and SSA ctxt 
 *                  4.Depending on wheter REGISTER has to be sent with 
 *                  authentication or not,call the appropiate utility
 *                  function to do the same.
 *
 ******************************************************************************/
 icf_return_t icf_ssa_process_otg_reg_req(
            INOUT   icf_ssa_pdb_st       *p_ssa_pdb)
 {   
        icf_return_t                     ret_val = ICF_SUCCESS;
        icf_rgm_context_st               *p_rgm_ctxt = ICF_NULL;
        icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
        icf_line_id_t                    line_index = 0;
        Sdf_st_callObject                   *p_call_obj = Sdf_co_null;        
        icf_boolean_t                    is_auth_req = ICF_FALSE;

        if (ICF_NULL == p_ssa_pdb)
        {
            return ICF_FAILURE;
        }
        ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

            /* Copy the received RGM context pointer locally */
            p_rgm_ctxt = (icf_rgm_context_st *)
            (p_ssa_pdb->p_internal_msg->p_msg_data);

        /* 
         * Set the pdb bitmask to indicate that the user address is to be read from 
         * rgm ctxt
         */
        p_ssa_pdb->common_bitmask |= ICF_SSA_USER_ADDR_FRM_RGM;

        /* Get the line data pointer for the last line in the received msg array */
        line_index = (icf_uint16_t )(p_rgm_ctxt->num_of_lines - 1);
        if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_ssa_pdb->p_glb_pdb,
                    p_rgm_ctxt->rgm_list_of_lines[line_index],
                    &(p_ssa_pdb->p_line_data),
                    p_ssa_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                return ret_val;
        }

        /* ASSUMPTION: The registrar address resolves to one IP ADDRESS */

        /* ---------------------------------------------------------------------- */

        /* First check if the rgm ctxt has an ssa ctxt already */

        /* First time Registration: (duration not 0) and (ssa ctxt ptr is null) */
        if ((ICF_NULL != p_rgm_ctxt->rgm_duration) &&
                (ICF_NULL == p_rgm_ctxt->p_ssa_ctxt))
        {
            p_ssa_pdb->common_bitmask |= ICF_SSA_RGM_INITIAL_REQ;

            /* 
             * This is a fresh Registration request, allocate memory to the new ssa 
             * ctxt pointer
         */
        ICF_MEMGET(
                p_ssa_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_ssa_ctxt_st),
                ICF_MEM_COMMON,
                p_ssa_ctxt,
                ICF_RET_ON_FAILURE,
                p_ssa_pdb->p_ecode,
                ret_val)

        /* Initialize the elements of new ssa ctxt */
        if (ICF_NULL == p_ssa_ctxt)
        {
            return ICF_FAILURE;
        }
        icf_ssa_init_ssa_context(p_ssa_pdb,p_ssa_ctxt);

        /* Store the rgm ctxt pointer in ssa ctxt */
        p_ssa_ctxt->p_rgm_ctxt = p_rgm_ctxt;
        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
                (ICF_NULL != p_ssa_ctxt->p_rgm_ctxt))
        
        /* Store the ssa pdb pointer in ssa ctxt */
        p_ssa_ctxt->p_ssa_pdb = (struct icf_ssa_pdb_st *)p_ssa_pdb;
        /* Add the reverse mapping too */
        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
                (ICF_NULL != p_ssa_ctxt->p_ssa_pdb))
        
        /* Get the new registration call object */
        if (ICF_FAILURE == icf_ssa_get_new_call_obj(
                                    p_ssa_pdb,
                                    &(p_call_obj)))
        {
            ICF_MEMFREE(
                    p_ssa_pdb->p_glb_pdb,
                    p_ssa_ctxt,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    ret_val)
                
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            if (p_call_obj != ICF_NULL)
                sdf_ivk_uaFreeCallObject(&p_call_obj);

            return ret_val;        
        }
        
        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_call_obj))
        p_ssa_ctxt->p_call_obj= p_call_obj;

        /* Set the ssa ctxt pointer in call object's app data */
        p_ssa_ctxt->p_call_obj->pAppData->pData = (SIP_Pvoid)p_ssa_ctxt;
        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
                (ICF_NULL != p_ssa_ctxt->p_call_obj->pAppData->pData))

#ifdef ICF_SIGCOMP_SUPPORT
       /* We are going to Send the Register Request to network so 
        * CreateCompartment for Seding Register request compressed
        * Only if the INIT TIME SIGCOMP REQUIRED IS ON
        */
        /* PS: Actually this should be handlled by RGM, Which can 
         * be taken care on ICF_REL_PHASE_2 
         * Currnetly handled by SSA it self 
         */
        if(ICF_TRUE == 
            p_ssa_pdb->p_glb_cfg_data->sigcomp_required)
        {
            p_rgm_ctxt->p_ssa_ctxt = p_ssa_ctxt;
            p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
            ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
                (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt))
                
           ret_val = icf_ssa_send_create_comp_req_to_sigsa(p_ssa_pdb);
        }
#endif                
        

    }
    /* De-Reg without Reg: (duration is 0) and (ssa ctxt ptr is null) */
    else if ((ICF_NULL == p_rgm_ctxt->rgm_duration) && 
            (ICF_NULL == p_rgm_ctxt->p_ssa_ctxt))
    {
        /* return failure to REGM */
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }
    /* Re-Reg or De-Reg: (ssa ctxt ptr is not null) */
    else
    {
        /* 
         * This is a continued Registration request or a De-registration req, 
         * copy the ssa ctxt pointer from the received message
         */
        p_ssa_ctxt = (icf_ssa_ctxt_st *)(p_rgm_ctxt->p_ssa_ctxt);
        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_ssa_ctxt))

    }

    /* Copy the call object pointer from ssa ctxt to ssa pdb */
    p_ssa_pdb->p_ssa_ctxt->p_call_obj = p_ssa_ctxt->p_call_obj;
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_obj))

    /* ---------------------------------------------------------------------- */
    
    /*
     * Now we have the ssa ctxt ready, find the action to be taken for the 
     * request
     */
	 /* check if mode reversion for this outgoing register will be allowed or 
	  *	not If the mode is configured to BOTH reversion will be allowed else 
	  * not
	  */
    /*set the SSA context pointer to the ssa_pdb */
    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt ;
    p_ssa_pdb->p_ssa_ctxt->line_id =  p_rgm_ctxt->line_id;

    if(ICF_FAILURE == icf_ssa_check_for_trans_param(p_ssa_pdb,
                    p_ssa_pdb->p_line_data->reg_mode,
                    ICF_NULL,ICF_TRUE))
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        ret_val = ICF_FAILURE;
    }
    /* SPR 16019: Check whether the transaction clear bitmask is set in rgm_context
     * If set then Abort the  register transaction.
     */
    if(ICF_RGM_CLEAR_ONGOING_REGISTRATION == (ICF_RGM_CLEAR_ONGOING_REGISTRATION &
                p_rgm_ctxt->indicator))
    {
        ret_val = icf_ssa_process_register_abort_req(p_ssa_pdb);
        if(ICF_SUCCESS == ret_val)
        {
            p_rgm_ctxt->indicator &= ~(ICF_RGM_CLEAR_ONGOING_REGISTRATION);
            if((ICF_TRANSPORT_MODE_BOTH ==  p_ssa_pdb->p_ssa_ctxt->transport)||
                (ICF_TRANSPORT_TYPE_TCP == p_ssa_pdb->p_ssa_ctxt->transport))
            {
                /* Blocking Connect required when any reg-dereg transaction 
                 * cleared forcefully.
                 */
                p_ssa_pdb->p_glb_cfg_data->is_tcp_blocking_connect = ICF_TRUE;
            }
        }
    }

    if(ICF_FAILURE == ret_val)
    {
        /* Some thing Failed before, So no need to process Further */
    }
    else if (ICF_NULL == p_rgm_ctxt->rgm_duration)
    {
        /* Deregistration request to be sent */
        ICF_SSA_RESET_COMMON_BITMASK(p_ssa_pdb->common_bitmask, 
                ICF_SSA_RGM_REQ_TYPE)

ICF_4_0_CRM_START
        /*
         * If Authentication Info is present on this line, send REG req with 
         * Auth info else send REG req without Auth info
         */
		 if ((ICF_RGM_AUTH_SET & p_rgm_ctxt->indicator) && 
               (!(ICF_SSA_RGM_INITIAL_REQ & p_ssa_pdb->common_bitmask)))
         {
            is_auth_req = ICF_TRUE;
         }
        if ((ICF_RGM_AUTH_SET & p_rgm_ctxt->indicator) && 
                (ICF_SUCCESS == icf_cmn_check_if_auth_params_present(
                                  p_ssa_pdb->p_glb_pdb, 
                                  &p_rgm_ctxt->rgm_user_address, 
								  p_ssa_pdb->p_line_data->line_id)) &&
                (!(ICF_SSA_RGM_INITIAL_REQ & p_ssa_pdb->common_bitmask)))
        {
            is_auth_req = ICF_TRUE;
        }
ICF_4_0_CRM_END
        if (ICF_TRUE == is_auth_req)   
        {
            /* Registration Request with Authorization */
            ret_val = icf_ssa_send_register_req_with_auth(p_ssa_pdb);
        }
        else
        {

            /* Registration Request without Authorization Info */
            ret_val = icf_ssa_send_register_req(p_ssa_pdb);

        }

    } /* End of if for (p_rgm_ctxt->rgm_duration) */
    else
    {
        ICF_SSA_SET_COMMON_BITMASK(p_ssa_pdb->common_bitmask, 
                ICF_SSA_RGM_REQ_TYPE)        
        
ICF_4_0_CRM_START
#ifdef IMS_CLIENT              
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
        /*
         * If Authentication Info is present on this line, send REG req with 
         * Auth info else send REG req without Auth info
         */
		 if ((ICF_RGM_AUTH_SET & p_rgm_ctxt->indicator) && 
               (!(ICF_SSA_RGM_INITIAL_REQ & p_ssa_pdb->common_bitmask)))
         {
            is_auth_req = ICF_TRUE;
         }
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT              
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
        if ((ICF_RGM_AUTH_SET & p_rgm_ctxt->indicator) && 
                (ICF_SUCCESS == icf_cmn_check_if_auth_params_present(
                                p_ssa_pdb->p_glb_pdb, 
								&p_rgm_ctxt->rgm_user_address, 
								p_ssa_pdb->p_line_data->line_id)) &&
                (!(ICF_SSA_RGM_INITIAL_REQ & p_ssa_pdb->common_bitmask)))
        {
            is_auth_req = ICF_TRUE;
        }
ICF_CHECK_IMS_END
#endif
ICF_4_0_CRM_END
        if (ICF_TRUE == is_auth_req)
        {            /* Registration Request with Authorization */
            ret_val = icf_ssa_send_register_req_with_auth(p_ssa_pdb);
        }
        else
        {            /* Registration Request without Authorization Info */
            ret_val = icf_ssa_send_register_req(p_ssa_pdb);
        }
      /* Stat collection of REGISTER request sent*/
	if(ret_val != ICF_FAILURE)
	{
	 ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
			stat_requests.stat_outbound_req.stats_REGISTER);
	}

    } /* End of else for (p_rgm_ctxt->rgm_duration) */

    /* ---------------------------------------------------------------------- */

    /* 
     * Save the ssa ctxt pointer back in rgm ctxt (this will be needed in case 
     * the ssa ctxt pointer was newly created for this request)
     */
    if (ICF_SUCCESS == ret_val)
    {
        p_rgm_ctxt->p_ssa_ctxt = p_ssa_ctxt;
        ICF_ASSERT(p_ssa_pdb->p_glb_pdb, 
                (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt))

    }
    else
    {
        /* Free the ssa ctxt and call object */
	    Sdf_st_callObject	*p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
        p_rgm_ctxt->p_ssa_ctxt = ICF_NULL;
        sdf_ivk_uaFreeCallObject(&p_call_obj);
        p_ssa_pdb->p_ssa_ctxt = ICF_NULL;
        /* ssa ctxt will be freed in sdf_cbk_uaFreeApplicationData */
    }
    p_ssa_pdb->p_glb_cfg_data->is_tcp_blocking_connect = ICF_FALSE;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_ssa_process_otg_reg_req() */

/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_process_clear_reg_obj
 *
 * DESCRIPTION:     This function processes request from REGM sub-module to 
 *                  destroy the registration call object.
 *                  1.Invoke sdf_ivk_uaAbortCall 
 *                  2.Free call object
 ******************************************************************************/
icf_return_t icf_ssa_process_clear_reg_obj(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
#ifdef IMS_CLIENT
    icf_return_t                     dummy_ret_val = ICF_SUCCESS;
#endif
    icf_rgm_context_st               *p_rgm_ctxt = ICF_NULL;
    Sdf_st_error                        error;
    Sdf_st_callObject                   *p_call_obj = ICF_NULL;

    icf_internal_msg_st    			internal_msg = {{0,0,0,0,0,0},0};
    icf_return_t                     return_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* Copy the received RGM context pointer locally */
    p_rgm_ctxt = (icf_rgm_context_st *)
                        (p_ssa_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_rgm_ctxt))


#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
     if(ICF_NULL != p_rgm_ctxt->p_srv_rte)
    {
        
        sdf_listDeleteAll((Sdf_ty_slist*)p_rgm_ctxt->p_srv_rte,
            &error);
        sdf_listFree((Sdf_ty_slist*)p_rgm_ctxt->p_srv_rte, &error);
        ICF_MEMFREE(
            p_ssa_pdb->p_glb_pdb,
            p_rgm_ctxt->p_srv_rte,
            ICF_MEM_COMMON,
            p_ssa_pdb->p_ecode,
            dummy_ret_val)
    }
ICF_CHECK_IMS_END
#endif
/* Fix for CSR 1-8070301: SPR 20236 */ 
	internal_msg.msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    /* 
     * If the ssa ctxt ptr is null, simply return success to REGM, else clear 
     * the ctxt and call object
     */
    if (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt)
    {
        p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
        /*This field is set to clean TCM connection if transport mode is TCP*/
        ((icf_ssa_ctxt_st *)p_rgm_ctxt->p_ssa_ctxt)->bitmask |= 
                        ICF_SSA_CLEAN_TCP_CONN_FORCEFUL_CLR;

		if (ICF_NULL != ((icf_ssa_ctxt_st *)
								(p_rgm_ctxt->p_ssa_ctxt))->p_auth_params_list)
		{
			sdf_listDeleteAll(
				((icf_ssa_ctxt_st *)
								(p_rgm_ctxt->p_ssa_ctxt))->p_auth_params_list,
                &error);
		}
        /* Abort the Registration request */
        sdf_ivk_uaAbortCall(
                p_call_obj, 
                p_rgm_ctxt->p_ssa_ctxt, 
                &error);
        sdf_ivk_uaCloseAllSocketsForCall(p_call_obj, &error);
#ifdef ICF_SIGCOMP_SUPPORT
        /* Here we are going to clear the RegObcect 
         * So Delete the Compartment Created While Registering the user
         */
		if(ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required)
		{
			if(ICF_NULL != ((icf_ssa_ctxt_st *)
								(p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt)
			{
				p_ssa_pdb->p_ssa_ctxt = 
					(icf_ssa_ctxt_st *)(p_rgm_ctxt->p_ssa_ctxt);
				icf_ssa_send_delete_comp_ind_to_sigsa(p_ssa_pdb);
				((icf_ssa_ctxt_st *)
					(p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt = ICF_NULL;
			}
		}
#endif
     /*Fix for CSR 8070301 SPR 20236*/
     /* If any running timer exists, then fetch all the timers  
     * stop these timers and simulate timer expiry for each
     * of the timer
	 */ 

        if (ICF_TRUE == icf_ssa_check_if_any_timers_exist(p_ssa_pdb->p_ssa_ctxt))
    	{
       	 	icf_ssa_timer_list_st      *p_timer_list = p_ssa_pdb->p_ssa_ctxt->p_timer_list;

       	 	while (0 != p_ssa_pdb->p_ssa_ctxt->timer_count)
        	{
            	p_timer_list = p_ssa_pdb->p_ssa_ctxt->p_timer_list;

            	if (ICF_NULL != p_timer_list)
            	{
                	icf_timer_id_t timer_id = p_timer_list->timer_id;

                     if (ICF_NULL == p_timer_list->p_timer_data) 
                        {
                           return_val = ICF_FAILURE; 
                        }

                	if (ICF_FAILURE == return_val)
                	{
                    	ret_val = ICF_FAILURE;
                    	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb);
                    	return return_val;
                	}
                    ICF_STOP_TIMER(p_ssa_pdb->p_glb_pdb, timer_id,
                                &(internal_msg.msg_hdr.ecode), ret_val)

                    internal_msg.msg_hdr.msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;

                    internal_msg.msg_hdr.status = ICF_NULL;

                    internal_msg.p_msg_data =
                            (icf_void_t *)p_timer_list->p_timer_data;

                    ret_val = icf_ssa_process_timer_expiry(&internal_msg);
            	}
			}
        }
        p_ssa_pdb->p_ssa_ctxt->p_timer_list = ICF_NULL;
     	p_ssa_pdb->p_ssa_ctxt->timer_count = ICF_NULL;


        /* Cross reference validation, set the reg context as null
         * within ssa context so that there is no junk pointer is present 
         * within SSA context.
         */ 
        ((icf_ssa_ctxt_st *)p_rgm_ctxt->p_ssa_ctxt)->p_rgm_ctxt = ICF_NULL;

        /* Free the Registration Call Object */
        sdf_ivk_uaFreeCallObject(&(p_call_obj));

        /* Free the ssa ctxt in this rgm ctxt */
        /* This should be done in the callback for freeing of ApplicationData 
         * (thrown to us by UATK in the processing of FreeCallObject to free 
         * the pCallObj->pAppData->pData pointer). Here, we just set the ptr 
         * to null.
         */
        p_rgm_ctxt->p_ssa_ctxt = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
} /* End of icf_ssa_process_clear_reg_obj() */

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_reject_ack
 **
 ** DESCRIPTION:        This function is used to send the acknowledgement
 **                     (ACK)for 3xx/4xx/5xx/6xx messages to peer.
 * DESCRIPTION:        This function is used to send the acknowledgement
 *                     (ACK)for 3xx/4xx/5xx/6xx messages to peer.
 *                     1.Form the ACK message and send the same to peer.
 *                     2.After the ACK for the failure response has been sent,
 *                     the link between SSA and the call control needs to be 
 *                     broken as the call context for the same has been
 *                     destroyed.For this purpose set the bit 
 *                     ICF_SSA_APP_CTXT_TERMINATED in the SSA ctxt
 *                     which indicates that no further messages for this 
 *                     dialog needs to be sent to CC
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_reject_ack(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    SipOptions              options = {0};
    Sdf_st_error            error ={0,0,0,"\0"};
    icf_call_ctx_st      *p_call_ctx = p_ssa_pdb->p_glb_pdb->p_call_ctx;


    icf_ssa_ctxt_st *p_ssa_ctxt = 
			(icf_ssa_ctxt_st *)p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx;
    Sdf_st_callObject	*p_call_obj = p_ssa_ctxt->p_call_obj;

    /* 
     * Flush the message bodies (if any) stored in the 
     * UAC transaction of the call object
     */
    sdf_listDeleteAll(
            &(p_ssa_ctxt->p_call_obj->pUacTransaction->slMsgBodyList),&error);
    /*
     * Form an ACK request
     */
    if (Sdf_co_fail == 
            sdf_ivk_uaFormRequest((Sdf_ty_s8bit *)"ACK",p_ssa_ctxt->p_call_obj, 
                Sdf_co_null, &error))
    {
        /*Raise/Set Error: UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else
    {
			if ( ICF_FAILURE == 
				icf_ssa_form_and_insert_access_nw_info_hdr(
				p_ssa_pdb, p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
			{
				ret_val = ICF_FAILURE;
			}
            else
            {

                /* Specify the stack options */
                options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
/*#ifdef ICF_DNS_LOOKUP_ENABLED
                if (ICF_FAILURE == icf_ssa_send_to_peer_no_dns_lookup(p_ssa_pdb,
                            p_ssa_ctxt->p_call_obj,
                            p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg,
                            &options,&error))
#else*/			
                if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                            p_ssa_ctxt->p_call_obj,
                            p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg,
                            &options,&error))
/*#endif*/
                {
	               /*Raise/Set Error: UATK_PROCESSING_FAILURE */
        	        ret_val = ICF_FAILURE;
                }
                else if(ICF_CONTINUE_CALL !=
                    (ICF_CONTINUE_CALL & p_call_ctx->common_bitmask))
                {
                /*
                * This indicates to SSA that on reception of any further
                * msg from N/w for this dialog, the CM is NOT TO BE
                * INDICATED
                    */
                    p_ssa_ctxt->bitmask |= ICF_SSA_APP_CTXT_TERMINATED;
                    icf_ssa_del_all_other_call_obj(p_ssa_pdb);
                    sdf_ivk_uaFreeCallObject(&p_call_obj);
                }
                else
                {
                    icf_ssa_del_all_other_call_obj(p_ssa_pdb);
                }
                {
                    /* Reset Call Contiune bitmask in call context */
                    p_call_ctx->common_bitmask &= (~(ICF_CONTINUE_CALL));
                }
            }   
			

    }
   if(ICF_FAILURE == ret_val)
   {
       /* Delete all ssa call objects other than one for which
        * Final Response is recived */
       icf_ssa_del_all_other_call_obj(p_ssa_pdb);
   }
    else
    {
        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                stat_requests.stat_outbound_req.stats_ACK);
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 * FUNCTION:        icf_ssa_process_forceful_call_clear
 * DESCRIPTION:     This API is invoked by the Call Control to forcefully
 *                  clear all the contexts of the call.
 *                  1.Invoke sdf_ivk_uaAbortCall 
 *                  2.Free the call object
 ******************************************************************************/
icf_return_t icf_ssa_process_forceful_call_clear(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st                  *p_ssa_ctxt = ICF_NULL;
    Sdf_st_callObject                   *p_call_obj = Sdf_co_null;
    Sdf_st_error                sdf_error = {0,0,0,"\0"};
    
	
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if (( ICF_NULL == p_ssa_pdb->p_ssa_ctxt) || 
        ( ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_obj))
    {
        /*Raise Trace here*/
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
        return  ICF_SUCCESS;
    }

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_ssa_pdb->p_ssa_ctxt);

    /*no_retrans_req is also checked in sdf_cbk_uaIndicateTxnTimeOut */
    p_ssa_ctxt->no_retrans_req = ICF_TRUE;

    /*If any running timer exists, then fetch all the timers 
     * stop these timers and simulate timer expiry for each
     * of the timer*/

    if (ICF_TRUE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt))
    {
        icf_internal_msg_st    internal_msg;
        icf_ssa_timer_list_st      *p_timer_list = p_ssa_ctxt->p_timer_list;
       
        /*Create internal message*/

        internal_msg.msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

        while (0 != p_ssa_ctxt->timer_count)
        {
            p_timer_list = p_ssa_ctxt->p_timer_list;

            if (ICF_NULL != p_timer_list)
            {
                icf_timer_id_t timer_id = p_timer_list->timer_id;
                
                ICF_STOP_TIMER(p_ssa_pdb->p_glb_pdb, timer_id, 
                        &(internal_msg.msg_hdr.ecode), ret_val)
                
	    		internal_msg.msg_hdr.msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;

        		internal_msg.msg_hdr.status = ICF_NULL;

                internal_msg.p_msg_data = 
                    (icf_void_t *)p_timer_list->p_timer_data;
                
                ret_val = icf_ssa_process_timer_expiry(&internal_msg);
            } /* if (ICF_NULL != p_timer_lis... */            
         
		}       

        p_ssa_ctxt->p_ssa_pdb = (icf_ssa_pdb_st*)p_ssa_pdb;
    } /* if (ICF_FALSE == icf_ssa... */
    
    if(p_ssa_ctxt->bitmask_1 & ICF_MANDATORY_CONTACT_HDR_MISSING)
    {
        sdf_ivk_uaAbortCall(
                p_ssa_ctxt->p_call_obj, 
                (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
        p_ssa_ctxt->bitmask_1 &= ~(ICF_MANDATORY_CONTACT_HDR_MISSING);
    }
   
    /*This field is set to clean TCPM connection if transport mode is TCP*/
    p_ssa_ctxt->bitmask |= ICF_SSA_CLEAN_TCP_CONN_FORCEFUL_CLR;

	/*Fix for CSR 1-8081459. Set the call context in ssa ctxt to NULL.
	 * Since, this call ctxt is to be cleared by CC itself while invoking forcefull call
	 * clear.
	 */
	p_ssa_ctxt->p_call_ctxt = ICF_NULL;

    p_call_obj = p_ssa_ctxt->p_call_obj;
    /*Clean up the UATK for this callobject and all the related transactions*/
    sdf_ivk_uaFreeCallObject(&(p_call_obj));

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

 
}

/*******************************************************************************
 * FUNCTION:        icf_ssa_process_refer_abort_req
 * DESCRIPTION:     This function aborts the REFER transaction
 *                  1.Invoke the utility function which aborts the REFER trans.
 * 
 ******************************************************************************/
icf_return_t icf_ssa_process_refer_abort_req(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;
    Sdf_st_error                sdf_error;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if ( ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_obj)
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        /*Raise Trace here*/
        return  ICF_SUCCESS;
    }
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_refer,Sdf_en_transactionSent,
                p_ssa_pdb->p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    /*
     * Even if we can't fetch the matching overlap transaction,
     * we should return SUCCESS as it is possible that the
     * REFER transaction timed out and has been removed from
     * the list of overlapped transactions
     */
    if (ICF_FAILURE != ret_val)
    {
        sdf_ivk_uaAbortTransaction(p_ssa_pdb->p_ssa_ctxt->p_call_obj, 
                p_ovtxn->dTxnId, Sdf_co_null,&sdf_error);
    }
    /* Always return SUCCESS as it is best effort */
    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 * FUNCTION:        icf_ssa_process_options_resp
 *
 * DESCRIPTION:     This function sends the response to the OPTIONS request
 *                  corresponding to the response received from MMI
 *                  1. Fisrt fetch the SSA ctxt from the global map 
 *                  2. Stop the OPTIONS timer and flush entry from the map
 *                  3. If success response,form and insert the SDP body
 *                     to the SIP message,else send failure resp to peer
 *                  4. Insert the Accept,Accept encoding and Accept Language
 *                     headers to the outgoing SIP message
 *
 *****************************************************************************/
icf_return_t icf_ssa_process_options_resp(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    Sdf_st_overlapTransInfo         *p_ovtxn = Sdf_co_null;
    Sdf_st_error                    sdf_error;
    SipOptions                      options;
    SdpMessage                      *p_sdp_msg = Sdf_co_null;
    icf_nw_inc_opt_cfm_st    *p_inc_opt_cfm = 
        (icf_nw_inc_opt_cfm_st*)(p_ssa_pdb->
                                        p_internal_msg->p_msg_data);
    icf_ssa_ctxt_st              *p_ssa_ctxt = ICF_NULL;
    icf_uint16_t                 sdf_resp_code = 0;
	Sdf_st_callObject       	*p_call_obj = ICF_NULL;
    icf_header_list_st*           p_hdr_list  = ICF_NULL;
    icf_tag_list_st*              p_tag_list  = ICF_NULL;
    

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

	p_ssa_ctxt = p_inc_opt_cfm->p_sic_ctx->p_ssa_ctx;
	if( ICF_NULL == p_ssa_ctxt)
	{
		ret_val = ICF_FAILURE;
	}
        else
	{
	   p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
        if(ICF_NULL != p_inc_opt_cfm->header_list.count)
          {
            p_hdr_list = &(p_inc_opt_cfm->header_list);
          }
        if(ICF_NULL != p_inc_opt_cfm->tag_list.count)
          {
            p_tag_list = &(p_inc_opt_cfm->tag_list);
          }

    ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_INC_OPTIONS_RESP)


    /*
     * Check if failure response needs to be sent to peer.
     * If yes,map the response code and send failure 
     * response to peer.If no reason has been set,
     * default resp code of 500 is sent
     */
    if ( ICF_OPTIONS_CFM_ERROR_PRESENT == 
			(p_inc_opt_cfm->bit_mask &
			ICF_OPTIONS_CFM_ERROR_PRESENT))
    {
		if ( ICF_CAUSE_INTERNAL_ERROR == 
			p_inc_opt_cfm->error_code)
		{
			sdf_resp_code = 500;
		}
#ifdef ICF_LOAD_STAT
    p_ssa_pdb->p_glb_pdb->load_data.inc_options_failure++;
#endif
    }
    else
    {
        sdf_resp_code = 200;
#ifdef ICF_LOAD_STAT
    p_ssa_pdb->p_glb_pdb->load_data.inc_options_success++;
#endif
    }
    /*
     * Fetch the overlapped transaction pOverlapTransInfo 
     * stored in the call object corresponding to OPTIONS
     * request received
     */
    
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_options,Sdf_en_transactionReceived,
                p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (Sdf_co_fail == sdf_ivk_uaFormResponse(sdf_resp_code, "OPTIONS", 
                p_ssa_ctxt->p_call_obj,
                p_ovtxn,
                Sdf_co_false,
                &sdf_error))
    {
        /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == 
            icf_ssa_form_and_insert_unknown_hdr(p_ssa_pdb,
                (icf_uint8_t *)"User-Agent", p_ssa_pdb->p_ssa_glb_data->user_agent_str.str,
                p_ovtxn->pSipMsg))
    {
        ret_val = ICF_FAILURE;
    }
    if ((ICF_NULL != p_hdr_list) && (ICF_NULL < p_hdr_list->count) && (ICF_SUCCESS == ret_val))
        {
            if (ICF_FAILURE == icf_ssa_insert_header_list_in_sip_msg(
                        p_ssa_pdb, p_hdr_list, ICF_TRUE, p_ovtxn->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }
        }
    if ((ICF_NULL != p_tag_list) && (ICF_NULL < p_tag_list->count)  && (ICF_SUCCESS == ret_val))
        {
            if (ICF_FAILURE == icf_ssa_insert_tag_list_in_sip_msg(
                p_ssa_pdb, p_tag_list, ICF_TRUE, p_ovtxn->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }

        }


    if(ICF_FAILURE != ret_val)
    {
        if (200 == sdf_resp_code &&
	    ICF_NULL != p_inc_opt_cfm->p_sdp_list)
        {
            /*
             * Form the SDP body and append the same in the
             * message body list
             */          
            
            if (ICF_FAILURE == icf_ssa_form_sdp_for_options_resp( 
                        &p_sdp_msg,
                        p_ssa_pdb,
                        p_inc_opt_cfm->p_sdp_list,
                        &sdf_error))
            {
                ret_val = ICF_FAILURE;
            } /* &sdf_error)) */
            else if(Sdf_co_fail == sdf_ivk_uaSetSdpInOverlapTransaction(
                        p_ovtxn,
                        p_sdp_msg, 
                        &sdf_error))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
            
            
	    /*Message body header changes Start*/
            else if ( Sdf_co_fail == icf_ssa_SetAllMsgBody(
                        p_ssa_ctxt->p_call_obj->pUasTransaction,
                        p_ovtxn,/*OverlapTxn*/"application/sdp", &sdf_error))
            {
                /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
                ret_val = ICF_FAILURE;
            }
	    /*Message body header changes Ends*/
        }
        if (ICF_FAILURE != ret_val && (200 == sdf_resp_code))
        {
			icf_uint32_t iterator = 0;
			icf_string_list_node_st *p_list = ICF_NULL;
		    icf_app_conf_st              *p_app_conf;

                if(ICF_FAILURE == icf_cfg_fetch_app_conf(
                    p_ssa_pdb->p_glb_pdb,
                    p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->app_id,
                    &(p_ssa_pdb->p_app_conf),
                    p_ssa_pdb->p_ecode))
                {
                    /* Free Memory */
                    ret_val = ICF_FAILURE;
                }         
				p_app_conf = p_ssa_pdb->p_app_conf;
				p_list = 
				p_app_conf->app_profile.supported_content_types.p_list;
				for ( iterator = 0; 
					iterator < 
					p_app_conf->app_profile.supported_content_types.count &&
					p_list != ICF_NULL; 
					iterator++)
				{
				 	if (ICF_FAILURE == 
						icf_ssa_form_and_insert_unknown_hdr(
                    		p_ssa_pdb,(icf_uint8_t *)"Accept",
                    		p_list->str.str, 
                    		p_ovtxn->pSipMsg))
        		   	{
            	   	 	ret_val = ICF_FAILURE;
            		}
					 p_list = p_list->p_next;
				}
				p_list = 
				p_app_conf->app_profile.supported_encodings.p_list;
				for ( iterator = 0; 
					iterator < 
					p_app_conf->app_profile.supported_encodings.count &&
					p_list != ICF_NULL; 
					iterator++)
				{
			 		if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                	    p_ssa_pdb,(icf_uint8_t *)"Accept-Encoding",
                	    p_list->str.str, 
                	    p_ovtxn->pSipMsg))
            		{
                		ret_val = ICF_FAILURE;
            		}
			 		p_list = p_list->p_next;
				}
				p_list = p_app_conf->app_profile.supported_languages.p_list;
				for ( iterator = 0; 
					iterator < 
					p_app_conf->app_profile.supported_languages.count &&
					p_list != ICF_NULL; 
					iterator++)
				{
			 		if (ICF_FAILURE == icf_ssa_form_and_insert_unknown_hdr(
                    	p_ssa_pdb,(icf_uint8_t *)"Accept-Language",
                    	(icf_uint8_t *)p_list->str.str, 
                    	p_ovtxn->pSipMsg))
            		{
                		ret_val = ICF_FAILURE;
            		}
					 p_list = p_list->p_next;
				}

            /* Specify the stack options */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
		}
		if(ICF_FAILURE == ret_val)
		{
		}
        else 
		{
			if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
				p_ssa_ctxt->p_call_obj,
				p_ovtxn->pSipMsg,
				&options,&sdf_error))
			{
				/*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
				ret_val = ICF_FAILURE;
			}
		
#ifdef ICF_STAT_ENABLE	
			else
			{
			icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"OPTIONS",sdf_resp_code,1);
			}
#endif
		}
       
    }
    /* Free the SDP message */
    if (Sdf_co_null != p_sdp_msg)
    {
        sip_freeSdpMessage(p_sdp_msg);
    }
    /* 
     * Reset the mapping in the map maintained 
     * in the SSA global data.Also reset the bit OPTIONS_ONGOING
     */
    icf_ssa_flush_entry_frm_map(p_ssa_pdb,p_ssa_ctxt);

    if (ICF_NULL != 
            (p_ssa_ctxt->bitmask & ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG))
    {
        p_ssa_ctxt->p_sic_ctxt = ICF_NULL;
        /* If OPTIONS is recvd out of dialog free the callobject */
		p_call_obj = p_ssa_ctxt->p_call_obj;
        sdf_ivk_uaFreeCallObject(&p_call_obj);
    }
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;

}

/*****************************************************************************
 * FUNCTION:        icf_ssa_process_info
 * DESCRIPTION:     This function is used to send an INFO request
 *                  with/without payload to the peer.
 *                  1. The payload along with the content type is indicated
 *                  to SSA in the internal message
 *****************************************************************************/
icf_return_t icf_ssa_process_info(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_glb_pdb_st       *p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;

    Sdf_st_error            sdf_error;
    Sdf_st_overlapTransInfo *p_overlap_trans_info = Sdf_co_null;
    SipOptions              options;
    icf_int8_t						*p_content_type = ICF_NULL;
	icf_uint8_t						count = 0;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if (ICF_NULL == p_glb_pdb->p_call_ctx)
    {
        /* Request to send INFO for a non-existing call */
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }
    /* Extract the SSA ctxt from the call context */
    p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_glb_pdb->p_call_ctx->p_sip_ctx);

    ICF_ASSERT(p_glb_pdb, p_ssa_ctxt)
    ICF_ASSERT(p_glb_pdb, (p_ssa_ctxt->p_call_obj))

    /*
     * Invoke the Start Transaction and MakeInfo API's to make the 
     * INFO request
     */
    if (Sdf_co_fail == sdf_ivk_uaStartTransaction(p_ssa_ctxt->p_call_obj,
                &p_overlap_trans_info,"INFO", &sdf_error))
    {
        /*Raise Alarm/Error UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if(Sdf_co_fail == sdf_ivk_uaMakeInfo(
                p_ssa_ctxt->p_call_obj, p_overlap_trans_info,
                &sdf_error))
    {
        sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                p_overlap_trans_info,"INFO",
                Sdf_en_uacTransaction);
        ret_val = ICF_FAILURE;
    }
    else
    {
       /*Message body header changes Start*/
        /* Set the count to the number of message bodies in the request
        */
		count = (icf_uint8_t )p_ssa_ctxt->p_call_ctxt->body_list.count;

		if(0 == count)
		{
			Sdf_mc_strdup(p_content_type,(icf_uint8_t *)"application/sdp");
		}
		else
		{
			 if(p_ssa_ctxt->p_call_ctxt->body_list.content_type.str_len != 0)
			 {
				 p_content_type = (icf_int8_t *)p_ssa_ctxt->p_call_ctxt->body_list.content_type.str;
			 }
		}

        /*
         * If the bit corresponding to the presence of INFO
         * payload is set in the internal message,send the 
         * INFO payload,else send INFO w/o message body
         */
        if (ICF_FAILURE == icf_insert_header_tag_body_list(
            p_ssa_pdb, ICF_TRUE,ICF_NULL,
            p_overlap_trans_info))
        {
            
            sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                p_overlap_trans_info, "INFO",
                Sdf_en_uacTransaction);
            /*Raise a Major error here - UATK_PROCESSING FAILURE*/
            ret_val = ICF_FAILURE;                        
        }

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)   
  
		ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_call_ctxt->p_preferred_identity))
		if (ICF_FAILURE == icf_ssa_form_and_insert_pref_identity_hdr(
			p_ssa_pdb, p_overlap_trans_info->pSipMsg))
		{
			sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
				p_overlap_trans_info, "INFO", Sdf_en_uacTransaction);
			ret_val = ICF_FAILURE;
		}
		else if ( ICF_FAILURE == 
				icf_ssa_form_and_insert_access_nw_info_hdr(
				p_ssa_pdb, p_overlap_trans_info->pSipMsg))
		{
			sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
				p_overlap_trans_info, "INFO", Sdf_en_uacTransaction);
			ret_val = ICF_FAILURE;
		}

ICF_CHECK_IMS_END
#endif

        if (ICF_FAILURE != ret_val)
        {
            /* Now send the INFO request to peer */
            options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
           
            if (ICF_NULL == p_content_type)
            {
               ret_val = ICF_FAILURE;
            }    
	    else if ( Sdf_co_fail == 
                        icf_ssa_SetAllMsgBody(
                            p_ssa_ctxt->p_call_obj->pUacTransaction,\
							p_overlap_trans_info,/*OverlapTxn*/(icf_int8_t *)p_content_type,\
                            &sdf_error))
            {
                sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                        p_overlap_trans_info, "INFO",
                        Sdf_en_uacTransaction);
                /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                ret_val = ICF_FAILURE;
            }
			else if(ICF_FAILURE == icf_cfg_fetch_app_conf(
											p_ssa_pdb->p_glb_pdb,
											p_ssa_ctxt->p_call_ctxt->app_id,
											&(p_ssa_pdb->p_app_conf),
											p_ssa_pdb->p_ecode))
			{
				sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
					p_overlap_trans_info, "INFO", Sdf_en_uacTransaction);
				ret_val = ICF_FAILURE;
			}   
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                       	p_ssa_ctxt->p_call_obj,
                       	p_overlap_trans_info->pSipMsg,
                       	&options,&sdf_error))
            	{
                	sdf_ivk_uaClearTransaction(p_ssa_ctxt->p_call_obj,
                        p_overlap_trans_info,"INFO",
                        Sdf_en_uacTransaction);
                	ret_val = ICF_FAILURE;
            	}
            else
            {
#ifdef ICF_LOAD_DEBUG
                printf("\nINFO req sent for    %d\n", p_ssa_ctxt->p_call_ctxt->call_id);
#endif
#ifdef ICF_LOAD_STAT
                p_ssa_pdb->p_glb_pdb->load_data.info_attempts++;
#endif
                sdf_ivk_uaEndTransaction(p_ssa_ctxt->p_call_obj,
                      p_overlap_trans_info, "INFO",&sdf_error);

                /* Stat collection of INFO sent*/
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_outbound_req.stats_INFO);
            }
        }         
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


ICF_SIC_START


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_subs_req
 *
 * DESCRIPTION:     This function processes outgoing subscribe request
 *        1. Fetch sic context
 *        2. Mark in ssa_pdb that access user address from app ctx
 *        3. Check if it is initial subscribe request
 *        4. 
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_otg_subs_req(
        INOUT icf_ssa_pdb_st  *p_ssa_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st         *p_ssa_ctx = ICF_NULL;
    icf_glb_pdb_st          *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_sic_ctx_st         *p_sic_ctx = ICF_NULL;
    icf_nw_otg_subs_req_st  *p_subs_req = ICF_NULL;
	Sdf_st_callObject       *p_call_obj = ICF_NULL;

#if defined ICF_SIGCOMP_SUPPORT ||defined ICF_IPSEC_ENABLED
    icf_rgm_context_st   *p_rgm_ctxt = ICF_NULL;
#endif
   
    
    /*Fetch icf_nw_otg_subs_req_st from internal_msg received*/
    p_subs_req = 
        (icf_nw_otg_subs_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);
    
    /*Fetch sic context from subs_req*/
    p_sic_ctx = p_subs_req->p_sic_ctx;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_sic_ctx))
    
    /*Set common_bitmask to access user address from application
     * context i.e. sic_ctx*/
    p_ssa_pdb->common_bitmask |= ICF_SSA_USER_ADDR_FRM_SIC;

	/* For in call requests, fetch the ssa context from call context*/
	/* Currently in-call subscription is not supported */
	/*
    else if (ICF_NULL != p_sic_ctx->p_call_ctx)
    {
        p_ssa_ctx = p_sic_ctx->p_call_ctx->p_ssa_ctxt;
    }*/


    if (ICF_NULL == p_sic_ctx->p_ssa_ctx)
    {
        p_ssa_pdb->common_bitmask |= ICF_SSA_SIC_INITIAL_REQ;

        /*Allocate memory for p_ssa_ctx*/
        ICF_MEMGET(
                p_glb_pdb,
                ICF_PORT_SIZEOF(icf_ssa_ctxt_st),
                ICF_MEM_COMMON,
                p_ssa_ctx,
                ICF_RET_ON_FAILURE,
                p_ssa_pdb->p_ecode,
                ret_val)

        /*Initialize p_ssa_ctx*/
        icf_ssa_init_ssa_context(p_ssa_pdb, p_ssa_ctx);

        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctx;
		/* Store the ssa pdb pointer in ssa ctxt */
        p_ssa_ctx->p_ssa_pdb = (icf_ssa_pdb_st *)p_ssa_pdb;

        p_ssa_ctx->p_sic_ctxt = p_sic_ctx;


#if defined ICF_SIGCOMP_SUPPORT ||defined ICF_IPSEC_ENABLED
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
      if(ICF_FAILURE == 
            icf_rgm_fetch_ctxt_for_uri(p_glb_pdb,
                     (p_sic_ctx->p_preferred_identity),
                     &p_rgm_ctxt,
                     p_ssa_pdb->p_ecode))
        {
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctx, \
                    ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)
            ret_val = ICF_FAILURE;
        }
ICF_CHECK_IMS_END
#endif
#ifdef ICF_SIGCOMP_SUPPORT
		if((ICF_TRUE ==  p_ssa_pdb->p_glb_cfg_data->sigcomp_required) && 
					(ICF_SUCCESS == ret_val)) 
		{	
			if (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt)
			{
				p_ssa_ctx->p_sigsa_ctxt = ((icf_ssa_ctxt_st*)
					(p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt;
				if(ICF_NULL != p_ssa_ctx->p_sigsa_ctxt)
				{
					/* In crement the reference Counter */
					icf_sigsa_incr_ref_count_to_sigsa_ctxt(
						p_ssa_ctx->p_sigsa_ctxt);
				}
			}
			else
			{
				p_ssa_ctx->p_sigsa_ctxt = ICF_NULL;
			}
		}
#endif
#ifdef ICF_IPSEC_ENABLED
		/* if IPsec is configured, then set the SA context and the
		 * RGM context into the SSA context and SSA pdb structures 
		 */
			if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) && 
					(ICF_SUCCESS == ret_val))
			{
				if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context) && 
						/* SA exists */
					(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context->is_active))
				{
					/* save the SA context and the REGM context pointers in the
					 * SSA PDB for future reference
					 */
					p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

					/* also set these pointers in the SSA ctxt. Required incase of 
					 * transaction time-out scenarios */
					p_ssa_ctx->ssa_sa_context.p_sa_context = 
						((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = 
						p_rgm_ctxt;
				}
				else if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context) && 
						(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context->is_active))
				{
					/* save the SA context and the REGM context pointers in the
					 * SSA PDB for future reference
					 */
					p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

					/* also set these pointers in the SSA ctxt. Required incase of 
					 * transaction time-out scenarios */
					p_ssa_ctx->ssa_sa_context.p_sa_context = 
						((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = 
						p_rgm_ctxt;
				}
				else if (ICF_NULL == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context)
				{
					p_ssa_pdb->p_sa_context = ICF_NULL;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt;

					p_ssa_ctx->ssa_sa_context.p_sa_context = ICF_NULL;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = p_rgm_ctxt;
				}
			}
#endif
        if(ret_val == ICF_FAILURE)
        {
        }
        /* Get the new Subscription call object */
        else if (ICF_FAILURE == icf_ssa_get_new_call_obj(p_ssa_pdb,
                    &(p_ssa_ctx->p_call_obj)))
        {
            /*Clear p_ssa_ctx and return failure*/

            ICF_MEMFREE(
                    p_glb_pdb,
                    p_ssa_ctx,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    ret_val)

            ret_val = ICF_FAILURE;
        } /* &(p_ssa_ctx->p_call_obj))) */
        else
        {
            /* Set the ssa ctxt pointer in call object's app data */
            p_ssa_ctx->p_call_obj->pAppData->pData = (SIP_Pvoid)p_ssa_ctx;
        } /* else */        
    }
    /*Above both conditions are false means request is to re-subscribe
     * or unsubscribe*/
    else
    {
        p_ssa_ctx = (icf_ssa_ctxt_st *)(p_sic_ctx->p_ssa_ctx);
    }

    if (ICF_SUCCESS == ret_val)
    {
        
        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctx;

        /*Call fucntion to send subscription request*/
        ret_val = icf_ssa_send_subscribe_req(p_ssa_pdb);
                   /* Stat collection of SUBSCRIBE sent*/
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
			stat_requests.stat_outbound_req.stats_SUBSCRIBE)


        if (ICF_SUCCESS == ret_val)
        {
			if( ICF_SSA_SIC_INITIAL_REQ ==
				(p_ssa_pdb->common_bitmask & ICF_SSA_SIC_INITIAL_REQ) )
			{

				icf_ssa_add_callid_callobj_map(
						p_ssa_pdb,						
						p_ssa_ctx,
						p_ssa_pdb->p_ecode);
				p_sic_ctx->p_ssa_ctx = p_ssa_ctx;
			}
        }
        else
        {
			p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
            /* Free the ssa ctxt and call object */
            sdf_ivk_uaFreeCallObject(&p_call_obj);
        }
    } /* if (ICF_SUCCESS == ret_val) */

    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_delink_req
 
 *****************************************************************************/
icf_return_t   icf_ssa_process_delink_req(
        INOUT  icf_ssa_pdb_st    *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st   *p_ssa_ctxt = ICF_NULL;
    icf_sic_ctx_st  *p_sic_ctx = ICF_NULL;
	icf_void_t		*p_orig_call_obj=ICF_NULL;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /*Fetch application context pointer*/
    p_sic_ctx = 
        (icf_sic_ctx_st *)(((icf_delink_app_ctx_st *)(p_ssa_pdb->
                    p_internal_msg->p_msg_data))->p_sic_ctx);

    /*Fetch p_ssa_ctxt*/
    p_ssa_ctxt = p_sic_ctx->p_ssa_ctx;

    if(p_ssa_ctxt)
    {
        /*CSR 1-6179301*/ 
        /*SPR 18430 For out of dialog Notify when PA does not send
          notify_cfm sic context was deallocated and ssa ctx pointer
          inside sic ctx was set to null but sic context inside ssa ctx
          was not set to null immediately. In between another out of dialog
          notify is received by IPTK and system allocates same memory block
          again for new sic context. Now timer expiry for first SSA context
          comes which still has the sic context pointer, however the memory
          now belongs to new sic context therefore inside FreeAppData it sets
          the new SSA ctx to null and later crash occurs when accessing the
          pointer for new SSA ctx. Therefore to just initilaize the sic ctx
          pointer inside SSA ctx after sending notify response we used below
          flag which will be true when only pointer to be set and not forceful
          timer expiry to be done.*/
        if(ICF_TRUE == p_sic_ctx->detach_sic_ctx)
        {
                    p_ssa_ctxt->p_sic_ctxt = ICF_NULL;
            return ICF_SUCCESS;
        }

    	p_ssa_ctxt->p_ssa_pdb = p_ssa_pdb;

		/*Mark in ssa_ctx bitmask that application context is de-linked*/
		p_ssa_ctxt->bitmask |= ICF_SSA_APP_CTX_DELINKED;

		p_ssa_ctxt->p_sic_ctxt = ICF_NULL;

		p_orig_call_obj= p_ssa_ctxt->p_call_obj;
    /*no_retrans_req is also checked in sdf_cbk_uaIndicateTxnTimeOut */


        /*If any running timer exists, then fetch all the timers 
        * stop these timers and simulate timer expiry for each
        * of the timer*/
        if (ICF_TRUE == ((icf_delink_app_ctx_st *)
            (p_ssa_pdb->p_internal_msg->p_msg_data))->forceful_delete)
        {
            p_ssa_ctxt->no_retrans_req = ICF_TRUE;
            if (ICF_TRUE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt))
            {
                icf_internal_msg_st    internal_msg;
                icf_ssa_timer_list_st  *p_timer_list = p_ssa_ctxt->p_timer_list;
                
                do
                {
                    p_timer_list = p_ssa_ctxt->p_timer_list;
                    
                    if (ICF_NULL != p_timer_list)
                    {
                        icf_timer_id_t timer_id = p_timer_list->timer_id;
                        
                        ICF_STOP_TIMER(p_ssa_pdb->p_glb_pdb, timer_id, 
                            &(internal_msg.msg_hdr.ecode), ret_val)
                            
                            internal_msg.p_msg_data = 
                            (icf_void_t *)p_timer_list->p_timer_data;
                        /*Create internal message*/
                        internal_msg.msg_hdr.msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;
                        
                        internal_msg.msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;
                        
                        internal_msg.msg_hdr.status = ICF_NULL;
                        
                        
                        if(ICF_FAILURE == icf_ssa_process_timer_expiry(
                            &internal_msg))
                        {
                            ret_val = ICF_FAILURE;
                            /* Exit from while loop */
                            break;
                        }
                        
                    } /* if (ICF_NULL != p_timer_lis... */            
                    
                } while( ICF_NULL != p_timer_list);
                
                p_ssa_ctxt->p_ssa_pdb = p_ssa_pdb;
            }
        }
		/*Free call object*/
		sdf_ivk_uaFreeCallObject((Sdf_st_callObject **)&p_orig_call_obj);
	}
    

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_initiate_clean_up
 *
 * DESCRIPTION:     This function processes clean up request received from CFG
 *                  This API fetches all the ssa contexts from its map and
 *                  check if any timer exists then stops it, simulate timer
 *                  expiry and then free all the memory for it.
 *                  API: ICF_CFG_SSA_INITIATE_CLEAN_UP
 *
 ******************************************************************************/
icf_return_t  icf_ssa_process_initiate_clean_up(
        INOUT icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_glb_pdb_st        *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    /* icf_uint16_t           count = 0;*/
    icf_ssa_glb_data_st   *p_ssa_glb_data = p_ssa_pdb->p_ssa_glb_data;
    Sdf_st_callObject        *p_call_obj = Sdf_co_null;
    icf_ssa_ctxt_st       *p_ssa_ctxt = ICF_NULL;
    Sdf_st_error             sdf_error;
    icf_boolean_t         ssa_ctxt_freed = ICF_FALSE ;
	icf_list_st			*p_list = ICF_NULL;
	icf_list_st			*p_tmp_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* 
     * Set the bit ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS in the
     * global bitmask and delete the headers present in the global
     * profile.On the next invocation of OPEN NW SERVERS,these 
     * headers will be added again
     */
    p_ssa_glb_data->ssa_glb_bitmask |= ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS;
    sdf_listDeleteAll(
            &(p_ssa_glb_data->p_default_profile->slExtraHeadersList),
			&sdf_error);

	ret_val = icf_dbm_get_all_active_ssa_ctxt(p_glb_pdb,
				&p_list,p_ssa_pdb->p_ecode);
	if ( ICF_SUCCESS == ret_val)
	{
		for ( p_tmp_list = p_list; p_tmp_list != ICF_NULL; p_tmp_list = p_tmp_list->p_next)
		{
			p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_tmp_list->p_data);
        	p_ssa_ctxt->p_ssa_pdb = p_ssa_pdb;

	        /*If any running timer exists, then fetch all the timers 
	         * stop these timers and simulate timer expiry for each
	         * of the timer*/
    	    if (ICF_TRUE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt))
    	    {
    	        icf_internal_msg_st    internal_msg;
    	        icf_ssa_timer_list_st  *p_timer_list = p_ssa_ctxt->p_timer_list;
	
	            /*Create internal message*/
   		         internal_msg.msg_hdr.msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;

    	        internal_msg.msg_hdr.p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    	        internal_msg.msg_hdr.status = ICF_NULL;
	
        	    do
        	    {
        	        /*
            	     * If the call object refcount was 1 in the prvious
           		      * iteration of this loop ,then the call object and
                	 * the SSA ctxt must have been freed already.
                	 * Hence if the variable ssa_ctxt_freed is true
                	 * then we shall break from the loop
                	 */
               		 if (ICF_TRUE == ssa_ctxt_freed)
                	    break;

                	if (1  == p_ssa_ctxt->p_call_obj->dRefCount)
                	{
                	    /*
                    	 * It indicates that the SSA ctxt will be
                     	* freed in the next invocation
                     	*/
                    	ssa_ctxt_freed = ICF_TRUE;
                	}
                	else
                	{
                	    ssa_ctxt_freed = ICF_FALSE;
                	}
                	p_timer_list = p_ssa_ctxt->p_timer_list;

                	if (ICF_NULL != p_timer_list)
                	{
                	    icf_timer_id_t timer_id = p_timer_list->timer_id;

                	    ICF_STOP_TIMER(p_ssa_pdb->p_glb_pdb, timer_id, 
                            &(internal_msg.msg_hdr.ecode), ret_val)

                   		 internal_msg.p_msg_data = 
                   		 (icf_void_t *)p_timer_list->p_timer_data;

                    	ret_val = icf_ssa_process_timer_expiry(&internal_msg);
                	} /* if (ICF_NULL != p_timer_lis... */            

            	} while( ICF_NULL != p_timer_list);
       	 	} /* if (ICF_FALSE == icf_ssa... */

        	if ( ICF_FALSE == ssa_ctxt_freed)
        	{        
            	/*
            	 * This field is set to clean TCPM connection 
            	 * if transport mode is TCP
            	 */
            	p_ssa_ctxt->bitmask |= ICF_SSA_CLEAN_TCP_CONN_FORCEFUL_CLR;

            	p_ssa_ctxt->p_ssa_pdb = p_ssa_pdb;

            	p_call_obj = p_ssa_ctxt->p_call_obj;

            	sdf_ivk_uaAbortCall(
                    p_ssa_ctxt->p_call_obj,
                    (Sdf_ty_pvoid*)p_ssa_ctxt, &sdf_error);
            	sdf_ivk_uaFreeCallObject(&p_call_obj);
        	}
        
    	} /* for (count = 0; count < ICF... */    
		icf_dbm_free_active_ssa_ctxt_list(p_glb_pdb,&p_list,p_ssa_pdb->p_ecode);
	}

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
} /* icf_ssa_process_initiate_clean_up */

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_msg_req
 *
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_otg_msg_req(
        INOUT icf_ssa_pdb_st  *p_ssa_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st         *p_ssa_ctx = ICF_NULL;
    icf_glb_pdb_st          *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_sic_ctx_st         *p_sic_ctx = ICF_NULL;
    icf_nw_otg_msg_req_st  *p_msg_req = ICF_NULL;
    
#if defined ICF_SIGCOMP_SUPPORT ||defined ICF_IPSEC_ENABLED
    icf_rgm_context_st   *p_rgm_ctxt = ICF_NULL;
#endif


    /*Fetch icf_nw_otg_msg_req_st from internal_msg received*/
    p_msg_req = 
        (icf_nw_otg_msg_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);
   

    /*Fetch sic context from subs_req*/
    p_sic_ctx = p_msg_req->p_sic_ctx;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_sic_ctx))
    
    /*Set common_bitmask to access user address from application
     * context i.e. sic_ctx*/
    p_ssa_pdb->common_bitmask |= ICF_SSA_USER_ADDR_FRM_SIC;

    /*Allocate memory for p_ssa_ctx*/
    ICF_MEMGET(
            p_glb_pdb,
            ICF_PORT_SIZEOF(icf_ssa_ctxt_st),
            ICF_MEM_COMMON,
            p_ssa_ctx,
            ICF_RET_ON_FAILURE,
            p_ssa_pdb->p_ecode,
            ret_val)

    /*Initialize p_ssa_ctx*/
    icf_ssa_init_ssa_context(p_ssa_pdb, p_ssa_ctx);

	/* Store the ssa pdb pointer in ssa ctxt */
    p_ssa_ctx->p_ssa_pdb = (icf_ssa_pdb_st *)p_ssa_pdb;



    p_ssa_ctx->p_sic_ctxt = p_sic_ctx;



#if	defined ICF_SIGCOMP_SUPPORT || defined ICF_IPSEC_ENABLED
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
      if(ICF_FAILURE == 
            icf_rgm_fetch_ctxt_for_uri(p_glb_pdb,
                     (p_sic_ctx->p_preferred_identity),
                     &p_rgm_ctxt,
                     p_ssa_pdb->p_ecode))
        {
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctx, \
                    ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)
            ret_val = ICF_FAILURE;
            p_sic_ctx->error_code = ICF_CALL_CLEAR_REASON_USER_NOT_REGISTERED;
        }
ICF_CHECK_IMS_END
#endif
#ifdef ICF_SIGCOMP_SUPPORT
		if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required) &&
			(ICF_SUCCESS == ret_val))
		{
		    if (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt)
            {
                p_ssa_ctx->p_sigsa_ctxt = ((icf_ssa_ctxt_st*)
                                (p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt;
                if(ICF_NULL != p_ssa_ctx->p_sigsa_ctxt)
                {
                    /* In crement the reference Counter */
                    icf_sigsa_incr_ref_count_to_sigsa_ctxt(
                        p_ssa_ctx->p_sigsa_ctxt);
                }

            }
            else
            {
                p_ssa_ctx->p_sigsa_ctxt = ICF_NULL;
            }
		}
#endif		
#ifdef ICF_IPSEC_ENABLED
			/* set the SA context and REGM context pointers in the SSA ctxt
			 * and SSA pdb pointer structures
			 */
			/* if IPsec is configured, then set the SA context and the
			 * RGM context into the SSA context and SSA pdb structures 
			 */
			if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) && 
				(ICF_SUCCESS == ret_val))
			{
				if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context) && 
					/* SA exists */
					(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context->is_active))
				{
					/* save the SA context and the REGM context pointers in the
					 * SSA PDB for future reference
					 */
					p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

					/* also set these pointers in the SSA ctxt. Required incase of 
					 * transaction time-out scenarios */
					p_ssa_ctx->ssa_sa_context.p_sa_context = 
						((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = 
						p_rgm_ctxt;
				}
				else if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context) && 
					(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context->is_active))
				{
					/* save the SA context and the REGM context pointers in the
					 * SSA PDB for future reference
					 */
					p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

					/* also set these pointers in the SSA ctxt. Required incase of 
					 * transaction time-out scenarios */
					p_ssa_ctx->ssa_sa_context.p_sa_context = 
						((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = 
						p_rgm_ctxt;
				}
			}
#endif
        if(ret_val == ICF_FAILURE)
        {
        }
    /* Get the new MESSAGE call object */
    else if (ICF_FAILURE == icf_ssa_get_new_call_obj(p_ssa_pdb,
                &(p_ssa_ctx->p_call_obj)))
    {
        /*Clear p_ssa_ctx and return failure*/

        ICF_MEMFREE(
                p_glb_pdb,
                p_ssa_ctx,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                ret_val)

        ret_val = ICF_FAILURE;
    } /* &(p_ssa_ctx->p_call_obj))) */
    else
    {
        /* Set the ssa ctxt pointer in call object's app data */
        p_ssa_ctx->p_call_obj->pAppData->pData = (SIP_Pvoid)p_ssa_ctx;
    } /* else */        

    

    if (ICF_SUCCESS == ret_val)
    {
        p_ssa_pdb->p_ssa_ctxt->p_call_obj = p_ssa_ctx->p_call_obj;

        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctx;

        /*Call fucntion to send message request*/
        ret_val = icf_ssa_send_message_req(p_ssa_pdb);
        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
			stat_requests.stat_outbound_req.stats_MESSAGE)


        if (ICF_SUCCESS == ret_val)
        {
            icf_ssa_add_callid_callobj_map(
                    p_ssa_pdb,
                    p_ssa_ctx,
                    p_ssa_pdb->p_ecode);
            p_sic_ctx->p_ssa_ctx = p_ssa_ctx;
        }
        else
        {
              /* Free the ssa ctxt and call object */
		    Sdf_st_callObject	*p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
		    /* Free the ssa ctxt and call object */
		    sdf_ivk_uaFreeCallObject(&p_call_obj);
        }
    } /* if (ICF_SUCCESS == ret_val) */

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_refer_req
 *
 * DESCRIPTION:     REFER request can be initiated from APP as well as from
 *                  services modules. When REFER request is initiated from
 *                  services modules ssa context will be fetched from p_call_ctx
 *                  otherwise p_sic_ctx.
 ******************************************************************************/
icf_return_t icf_ssa_process_otg_refer_req(
        INOUT icf_ssa_pdb_st  *p_ssa_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st          *p_ssa_ctx = ICF_NULL;
    icf_glb_pdb_st           *p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_sic_ctx_st           *p_sic_ctx = ICF_NULL;
    icf_nw_otg_refer_req_st  *p_refer_req = ICF_NULL;

#if defined ICF_SIGCOMP_SUPPORT || defined ICF_IPSEC_ENABLED
    icf_address_st           *p_pref_id = ICF_NULL;
#endif
        
#if defined ICF_SIGCOMP_SUPPORT ||defined ICF_IPSEC_ENABLED
    icf_rgm_context_st   *p_rgm_ctxt = ICF_NULL;
#endif

    p_refer_req = 
        (icf_nw_otg_refer_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);
   
    /*Fetch sic context from subs_req*/
    p_sic_ctx = p_refer_req->p_sic_ctx;

    if (ICF_NULL != p_sic_ctx)
    {
       /*Set common_bitmask to access user address from application
        * context i.e. sic_ctx*/
        p_ssa_pdb->common_bitmask |= ICF_SSA_USER_ADDR_FRM_SIC;
        
        if( p_sic_ctx->p_call_ctx)
        {
            p_sic_ctx->bitmask |= ICF_SIC_REQUEST_WITHIN_DIALOG;
            p_ssa_ctx = p_sic_ctx->p_call_ctx->p_sip_ctx;
            
        }
        else
        {
            p_sic_ctx->bitmask &= ~ICF_SIC_REQUEST_WITHIN_DIALOG;
            /*Allocate memory for p_ssa_ctx*/
            ICF_MEMGET(
                p_glb_pdb,
                ICF_PORT_SIZEOF(icf_ssa_ctxt_st),
                ICF_MEM_COMMON,
                p_ssa_ctx,
                ICF_RET_ON_FAILURE,
                p_ssa_pdb->p_ecode,
                ret_val)
                
            /*Initialize p_ssa_ctx*/
            icf_ssa_init_ssa_context(p_ssa_pdb, p_ssa_ctx);
            
            /* Store the ssa pdb pointer in ssa ctxt */
            p_ssa_ctx->p_ssa_pdb = (icf_ssa_pdb_st *)p_ssa_pdb;
        }
        p_ssa_ctx->p_sic_ctxt = p_sic_ctx;
    }
    else
    {
        p_ssa_ctx = p_glb_pdb->p_call_ctx->p_sip_ctx;        
    }

/* RN*/
#if defined ICF_SIGCOMP_SUPPORT || defined ICF_IPSEC_ENABLED
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
    /*Fetch preferred-identity from call context if p_sic_ctx is null*/
    if (ICF_NULL == p_sic_ctx)
    {
        p_pref_id = p_glb_pdb->p_call_ctx->p_preferred_identity;
    }
    else
    {
        p_pref_id = p_sic_ctx->p_preferred_identity;
    }

    if(ICF_FAILURE == 
        icf_rgm_fetch_ctxt_for_uri(p_glb_pdb,
                 (p_pref_id),
                 &p_rgm_ctxt,
                 p_ssa_pdb->p_ecode))
    {
        ICF_MEMFREE(p_glb_pdb,p_ssa_ctx, \
                ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)
        ret_val = ICF_FAILURE;
    }
ICF_CHECK_IMS_END
#endif
#ifdef ICF_SIGCOMP_SUPPORT
	if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required) && 
		(ICF_SUCCESS == ret_val))
	{
        if (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt)
        {
            p_ssa_ctx->p_sigsa_ctxt = ((icf_ssa_ctxt_st*)
                            (p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt;
            if(ICF_NULL != p_ssa_ctx->p_sigsa_ctxt)
            {
                /* In crement the reference Counter */
                icf_sigsa_incr_ref_count_to_sigsa_ctxt(
                    p_ssa_ctx->p_sigsa_ctxt);
            }

        }
        else
        {
            p_ssa_ctx->p_sigsa_ctxt = ICF_NULL;
        }
	}
#endif
#ifdef ICF_IPSEC_ENABLED
		/* if IPsec is configured, then set the SA context and the
		 * RGM context into the SSA context and SSA pdb structures 
		 */
		if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) && 
				(ICF_SUCCESS == ret_val))
		{
			if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context) && 
					/* SA exists */
				(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context->is_active))
			{
				/* save the SA context and the REGM context pointers in the
				 * SSA PDB for future reference
				 */
				p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
				p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

				/* also set these pointers in the SSA ctxt. Required incase of 
				 * transaction time-out scenarios */
				p_ssa_ctx->ssa_sa_context.p_sa_context = 
					((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
				p_ssa_ctx->ssa_sa_context.p_rgm_context = 
					p_rgm_ctxt;
			}
			else if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context) && 
					(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context->is_active))
			{
				/* save the SA context and the REGM context pointers in the
				 * SSA PDB for future reference
				 */
				p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
				p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

				/* also set these pointers in the SSA ctxt. Required incase of 
				 * transaction time-out scenarios */
				p_ssa_ctx->ssa_sa_context.p_sa_context = 
					((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
				p_ssa_ctx->ssa_sa_context.p_rgm_context = 
					p_rgm_ctxt;
			}
			else if (ICF_NULL == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context)
			{
				p_ssa_pdb->p_sa_context = ICF_NULL;
				p_ssa_pdb->p_rgm_context = p_rgm_ctxt;

				p_ssa_ctx->ssa_sa_context.p_sa_context = ICF_NULL;
				p_ssa_ctx->ssa_sa_context.p_rgm_context = p_rgm_ctxt;
			}
		}
#endif

    if (ICF_NULL != p_sic_ctx)
    {
        if ( !( p_sic_ctx->bitmask & ICF_SIC_REQUEST_WITHIN_DIALOG) )
        {
            if(ret_val == ICF_FAILURE)
            {
            }
            /* Get the new REFER call object */
            else if (ICF_FAILURE == icf_ssa_get_new_call_obj(p_ssa_pdb,
                &(p_ssa_ctx->p_call_obj)))
            {
                /*Clear p_ssa_ctx and return failure*/
                
                ICF_MEMFREE(
                    p_glb_pdb,
                    p_ssa_ctx,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    ret_val)
                    
                    ret_val = ICF_FAILURE;
            } /* &(p_ssa_ctx->p_call_obj))) */
            else
            {
                /* Set the ssa ctxt pointer in call object's app data */
                p_ssa_ctx->p_call_obj->pAppData->pData = (SIP_Pvoid)p_ssa_ctx;
            } /* else */        
        }
        else
        {
            /* For in dialog refer */
            ret_val = icf_ssa_refresh_supported_content_and_events(p_ssa_pdb,
                &(p_sic_ctx->header_list),p_ssa_pdb->p_ecode);
        }
    }
  
    if (ICF_SUCCESS == ret_val)
    {
        p_ssa_pdb->p_ssa_ctxt->p_call_obj = p_ssa_ctx->p_call_obj;

        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctx;

        /*Call fucntion to send message request*/
        ret_val = icf_ssa_send_refer_req(p_ssa_pdb);

        if (ICF_NULL != p_sic_ctx)
        {
            if (ICF_SUCCESS == ret_val)
            {
                               /*Stat collection of REFER sent*/
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
				stat_requests.stat_outbound_req.stats_REFER);

                if ( !( p_sic_ctx->bitmask & ICF_SIC_REQUEST_WITHIN_DIALOG) )
                {
                    icf_ssa_add_callid_callobj_map(
                        p_ssa_pdb,						
                        p_ssa_ctx,
                        p_ssa_pdb->p_ecode);
                }
                p_sic_ctx->p_ssa_ctx = p_ssa_ctx;
                
            }
            else
            {
            /*we ar enot freein the callobject in case the request was indialog
            as : the refcount is increased in the ssa_send_refer function only when
            SendCallToPeer is executed successfully.if the above fn. returns failure
            and it is in dialog, we need not decrement refcount as we have not incre
            mented it at all. for out of dialog requests we need to free the call
                object as we allocted it in the flow of this API only*/
                /* Free the ssa ctxt and call object */
                if ( !( p_sic_ctx->bitmask & ICF_SIC_REQUEST_WITHIN_DIALOG) )
                {
                    /* Free the ssa ctxt and call object */
			        Sdf_st_callObject	*p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
			        sdf_ivk_uaFreeCallObject(&p_call_obj);
                }
            }
        }
    } /* if (ICF_SUCCESS == ret_val) */

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_publish_req
 *
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_otg_publish_req(
        INOUT icf_ssa_pdb_st  *p_ssa_pdb)
{
    icf_return_t					ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st				*p_ssa_ctx = ICF_NULL;
    icf_glb_pdb_st				*p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_sic_ctx_st				*p_sic_ctx = ICF_NULL;
    icf_nw_otg_publish_req_st	*p_pub_req = ICF_NULL;
	Sdf_st_callObject       	*p_call_obj = ICF_NULL;

#if defined ICF_SIGCOMP_SUPPORT ||defined ICF_IPSEC_ENABLED
    icf_rgm_context_st   *p_rgm_ctxt = ICF_NULL;
#endif

    /*Fetch icf_nw_otg_publish_req_st from internal_msg received*/
    p_pub_req = 
        (icf_nw_otg_publish_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);
    
    /*Fetch sic context from subs_req*/
    p_sic_ctx = p_pub_req->p_sic_ctx;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_sic_ctx))
    
    /*Set common_bitmask to access user address from application
     * context i.e. sic_ctx*/
    p_ssa_pdb->common_bitmask |= ICF_SSA_USER_ADDR_FRM_SIC;

  

    /*Allocate memory for p_ssa_ctx*/
    ICF_MEMGET(
            p_glb_pdb,
            ICF_PORT_SIZEOF(icf_ssa_ctxt_st),
            ICF_MEM_COMMON,
            p_ssa_ctx,
            ICF_RET_ON_FAILURE,
            p_ssa_pdb->p_ecode,
            ret_val)

    /*Initialize p_ssa_ctx*/
    icf_ssa_init_ssa_context(p_ssa_pdb, p_ssa_ctx);

	/* Store the ssa pdb pointer in ssa ctxt */
     p_ssa_ctx->p_ssa_pdb = (icf_ssa_pdb_st *)p_ssa_pdb;

    p_ssa_ctx->p_sic_ctxt = p_sic_ctx;

/* RN*/
#if defined ICF_SIGCOMP_SUPPORT || defined ICF_IPSEC_ENABLED
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
      if(ICF_FAILURE == 
            icf_rgm_fetch_ctxt_for_uri(p_glb_pdb,
                     (p_sic_ctx->p_preferred_identity),
                     &p_rgm_ctxt,
                     p_ssa_pdb->p_ecode))
        {
            ICF_MEMFREE(p_glb_pdb,p_ssa_ctx, \
                    ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)
            ret_val = ICF_FAILURE;
            p_sic_ctx->error_code = ICF_CALL_CLEAR_REASON_USER_NOT_REGISTERED;
        }
ICF_CHECK_IMS_END
#endif
#ifdef ICF_SIGCOMP_SUPPORT
		if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required) && 
			(ICF_SUCCESS == ret_val))
		{
            if (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt)
            {
                p_ssa_ctx->p_sigsa_ctxt = ((icf_ssa_ctxt_st*)
                                (p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt;
                if(ICF_NULL != p_ssa_ctx->p_sigsa_ctxt)
                {
                    /* In crement the reference Counter */
                    icf_sigsa_incr_ref_count_to_sigsa_ctxt(
                        p_ssa_ctx->p_sigsa_ctxt);
                }

            }
            else
            {
                p_ssa_ctx->p_sigsa_ctxt = ICF_NULL;
            }
		}
#endif
#ifdef ICF_IPSEC_ENABLED
		/* if IPsec is configured, then set the SA context and the
		 * RGM context into the SSA context and SSA pdb structures 
		 */
			if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) && 
					(ICF_TRUE == ret_val))
			{
				if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context) && 
						/* SA exists */
					(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context->is_active))
				{
					/* save the SA context and the REGM context pointers in the
					 * SSA PDB for future reference
					 */
					p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

					/* also set these pointers in the SSA ctxt. Required incase of 
					 * transaction time-out scenarios */
					p_ssa_ctx->ssa_sa_context.p_sa_context = 
						((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = 
						p_rgm_ctxt;
				}
				else if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context) && 
						(ICF_TRUE == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context->is_active))
				{
					/* save the SA context and the REGM context pointers in the
					 * SSA PDB for future reference
					 */
					p_ssa_pdb->p_sa_context = ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt ;

					/* also set these pointers in the SSA ctxt. Required incase of 
					 * transaction time-out scenarios */
					p_ssa_ctx->ssa_sa_context.p_sa_context = 
						((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_old_sa_context;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = 
						p_rgm_ctxt;
				}
				else if (ICF_NULL == ((icf_rgm_glb_data_st *)(p_ssa_pdb->p_glb_rgm_data))->p_new_sa_context)
				{
					p_ssa_pdb->p_sa_context = ICF_NULL;
					p_ssa_pdb->p_rgm_context = p_rgm_ctxt;

					p_ssa_ctx->ssa_sa_context.p_sa_context = ICF_NULL;
					p_ssa_ctx->ssa_sa_context.p_rgm_context = p_rgm_ctxt;
				}
			}
#endif
        if(ret_val == ICF_FAILURE)
        {
        }

    /* Get the new PUBLISH call object */
    else if (ICF_FAILURE == icf_ssa_get_new_call_obj(p_ssa_pdb,
                &(p_ssa_ctx->p_call_obj)))
    {
        /*Clear p_ssa_ctx and return failure*/

        ICF_MEMFREE(
                p_glb_pdb,
                p_ssa_ctx,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                ret_val)

        ret_val = ICF_FAILURE;
    } /* &(p_ssa_ctx->p_call_obj))) */
    else
    {
        /* Set the ssa ctxt pointer in call object's app data */
        p_ssa_ctx->p_call_obj->pAppData->pData = (SIP_Pvoid)p_ssa_ctx;
    } /* else */        

    

    if (ICF_SUCCESS == ret_val)
    {
        p_ssa_pdb->p_ssa_ctxt->p_call_obj = p_ssa_ctx->p_call_obj;

        p_ssa_pdb->p_ssa_ctxt = p_ssa_ctx;

        /*Call fucntion to send publish request*/
        ret_val = icf_ssa_send_publish_req(p_ssa_pdb);

        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
			stat_requests.stat_outbound_req.stats_PUBLISH)


        if (ICF_SUCCESS == ret_val)
        {
            icf_ssa_add_callid_callobj_map(
                    p_ssa_pdb,                    
                    p_ssa_ctx,
                    p_ssa_pdb->p_ecode);
            p_sic_ctx->p_ssa_ctx = p_ssa_ctx;
        }
        else
        {
			p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
            /* Free the ssa ctxt and call object */
            sdf_ivk_uaFreeCallObject(&p_call_obj);
        }
    } /* if (ICF_SUCCESS == ret_val) */

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_options_req
 *
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_otg_options_req(
        INOUT icf_ssa_pdb_st  *p_ssa_pdb)
{
    icf_return_t				ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st			*p_ssa_ctx = ICF_NULL;
    icf_glb_pdb_st			*p_glb_pdb = p_ssa_pdb->p_glb_pdb;
    icf_sic_ctx_st			*p_sic_ctx = ICF_NULL;
    icf_nw_otg_opt_req_st	*p_opt_req = ICF_NULL;
   Sdf_st_overlapTransInfo  *p_overlap_trans_info = Sdf_co_null;
   Sdf_st_error              sdf_error;
   SipOptions              options;

#if defined ICF_SIGCOMP_SUPPORT ||defined ICF_IPSEC_ENABLED
    icf_rgm_context_st   *p_rgm_ctxt = ICF_NULL;
#endif
	Sdf_st_callObject       *p_call_obj = ICF_NULL;

    /*Fetch icf_nw_otg_opt_req_st from internal_msg received*/
    p_opt_req = 
        (icf_nw_otg_opt_req_st *)(p_ssa_pdb->p_internal_msg->p_msg_data);
    
    /*Fetch sic context from subs_req*/
    p_sic_ctx = p_opt_req->p_sic_ctx;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_sic_ctx))
    
    /*Set common_bitmask to access user address from application
     * context i.e. sic_ctx*/
    p_ssa_pdb->common_bitmask |= ICF_SSA_USER_ADDR_FRM_SIC;

  
    if(ICF_FALSE == p_opt_req->is_option_in_dlg)
    {
        /*Allocate memory for p_ssa_ctx*/
        ICF_MEMGET(
                p_glb_pdb,
                ICF_PORT_SIZEOF(icf_ssa_ctxt_st),
                ICF_MEM_COMMON,
                p_ssa_ctx,
                ICF_RET_ON_FAILURE,
                p_ssa_pdb->p_ecode,
                ret_val)

            /*Initialize p_ssa_ctx*/
            icf_ssa_init_ssa_context(p_ssa_pdb, p_ssa_ctx);
        /* Store the ssa pdb pointer in ssa ctxt */
        p_ssa_ctx->p_ssa_pdb = (icf_ssa_pdb_st *)p_ssa_pdb;
        p_ssa_ctx->p_sic_ctxt = p_sic_ctx;

        /* RN*/
#if defined ICF_SIGCOMP_SUPPORT || defined ICF_IPSEC_ENABLED
        ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
            if(ICF_FAILURE == 
                    icf_rgm_fetch_ctxt_for_uri(p_glb_pdb,
                        (p_sic_ctx->p_preferred_identity),
                        &p_rgm_ctxt,
                        p_ssa_pdb->p_ecode))
            {
                ICF_MEMFREE(p_glb_pdb,p_ssa_ctx, \
                        ICF_MEM_COMMON, p_ssa_pdb->p_ecode, ret_val)
                    ret_val = ICF_FAILURE;
            }
        ICF_CHECK_IMS_END
#endif
#ifdef ICF_SIGCOMP_ENABLED
            if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->sigcomp_required) && 
                    (ICF_SUCCESS == ret_val))
            {
                if (ICF_NULL != p_rgm_ctxt->p_ssa_ctxt)
                {
                    p_ssa_ctx->p_sigsa_ctxt = ((icf_ssa_ctxt_st*)
                            (p_rgm_ctxt->p_ssa_ctxt))->p_sigsa_ctxt;
                    if(ICF_NULL != p_ssa_ctx->p_sigsa_ctxt)
                    {
                        /* In crement the reference Counter */
                        icf_sigsa_incr_ref_count_to_sigsa_ctxt(
                                p_ssa_ctx->p_sigsa_ctxt);
                    }

                }
                else
                {
                    p_ssa_ctx->p_sigsa_ctxt = ICF_NULL;
                }
            }
#endif
#ifdef ICF_IPSEC_ENABLED
        /* need to inser the Sec-Verify header send the message
         * protected if SA exists
         */
        if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
                /* if SA exits */
                (ICF_NULL != p_ssa_pdb->p_sa_context) && (ICF_TRUE == ret_val))
        {
            /* insert security-verify header in the otg INVITE */
            icf_uint8_t        *p_value = ICF_NULL;

            /* insert security-verify header in the otg INVITE */
            if (ICF_FAILURE == icf_ssa_get_sec_verify_string (p_ssa_pdb, &p_value,
                        p_ssa_pdb->p_sa_context))
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE ==     
                    icf_ssa_insert_unknown_hdr_in_sip_msg (
                        p_ssa_pdb,
                        (icf_uint8_t*)"Security-Verify",
                        p_value, 
                        p_ssa_pdb->p_ssa_ctxt->p_call_obj->pUacTransaction->pSipMsg))
            {
                ICF_PRINT(("\nFailed to insert unknown header \
                            (Security-Verify)"));
                ret_val = ICF_FAILURE;
            }
            if (ICF_NULL != p_value)
            {
                ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                        p_value,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
            }
            if (ICF_SUCCESS == ret_val )
            {
                ICF_PRINT(("\nSecurity Verify Header inserted to \
                            outgoing Options "));
            }
        }
#endif
        if(ret_val == ICF_FAILURE)
        {
        }

        /* Get the new OPTIONS call object */
        if (ICF_FAILURE == icf_ssa_get_new_call_obj(p_ssa_pdb,
                    &(p_ssa_ctx->p_call_obj)))
        {
            /*Clear p_ssa_ctx and return failure*/

            ICF_MEMFREE(
                    p_glb_pdb,
                    p_ssa_ctx,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    ret_val)

                ret_val = ICF_FAILURE;
        } /* &(p_ssa_ctx->p_call_obj))) */
        else
        {
            /* Set the ssa ctxt pointer in call object's app data */
            p_ssa_ctx->p_call_obj->pAppData->pData = (SIP_Pvoid)p_ssa_ctx;
        } /* else */        



        if (ICF_SUCCESS == ret_val)
        {
            p_ssa_pdb->p_ssa_ctxt->p_call_obj = p_ssa_ctx->p_call_obj;

            p_ssa_pdb->p_ssa_ctxt = p_ssa_ctx;

#ifdef ICF_TRACE_ENABLE
            p_ssa_pdb->dialog_status=0;   /* out of dialog */
            icf_port_strcpy(p_ssa_pdb->remote_addr.addr_val.str,p_sic_ctx->dest_addr.addr_val.str);
            ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_OTG_OPTIONS);
#endif 

            /*Call fucntion to send options request*/
            ret_val = icf_ssa_send_options_req(p_ssa_pdb);


            if (ICF_SUCCESS == ret_val)
            {
                /* Stat collection of OPTION sent out of dialog*/
		ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                      stat_requests.stat_outbound_req.stats_outdlg_OPTION);

                icf_ssa_add_callid_callobj_map(
                        p_ssa_pdb,
                        p_ssa_ctx,
                        p_ssa_pdb->p_ecode);
                p_sic_ctx->p_ssa_ctx = p_ssa_ctx;
            }
            else
            {
                p_call_obj = p_ssa_pdb->p_ssa_ctxt->p_call_obj;
                /* Free the ssa ctxt and call object */
                sdf_ivk_uaFreeCallObject(&p_call_obj);
            }
        } /* if (ICF_SUCCESS == ret_val) */
    } /*is_option_in_dlg*/
    else
    {

        /* in-dialog processing for OPTIONS */
        if (ICF_NULL == p_glb_pdb->p_call_ctx)
        {
            /* Request to send INFO for a non-existing call */
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;
        }
        else if (ICF_NULL == p_glb_pdb->p_call_ctx->p_sip_ctx)
        {
            /* Request to send OPTIONS for a detached call */
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;
        }        

        /* Extract the SSA ctxt from the call context */
#if 0
        if(ICF_NULL == p_ssa_ctx)
        {
            return ICF_FAILURE; 
        }    
#endif            
        p_ssa_ctx = (icf_ssa_ctxt_st*)(p_glb_pdb->p_call_ctx->p_sip_ctx);
        ICF_ASSERT(p_glb_pdb, p_ssa_ctx)
        ICF_ASSERT(p_glb_pdb, (p_ssa_ctx->p_call_obj))
        p_ssa_ctx->p_sic_ctxt = p_sic_ctx;
#ifdef ICF_TRACE_ENABLE	
	p_ssa_pdb->dialog_status=1;   /* in dialog */
        icf_port_strcpy(p_ssa_pdb->remote_addr.addr_val.str,p_sic_ctx->dest_addr.addr_val.str);
        ICF_SSA_APP_TRACE(p_ssa_pdb, ICF_TRACE_OTG_OPTIONS);
#endif

     /*
     * Invoke the Start Transaction and MakeOPTIONS API's to make the 
     * OPTIONS request
     */
    if (Sdf_co_fail == sdf_ivk_uaStartTransaction(p_ssa_ctx->p_call_obj,
                &p_overlap_trans_info,"OPTIONS", &sdf_error))
    {
        /*Raise Alarm/Error UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if(Sdf_co_fail == sdf_ivk_uaMakeOptions(
                p_ssa_ctx->p_call_obj, p_overlap_trans_info,
                &sdf_error))
    {
        sdf_ivk_uaClearTransaction(p_ssa_ctx->p_call_obj,
                p_overlap_trans_info,"OPTIONS",
                Sdf_en_uacTransaction);
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Insert Header/Tag/Body if any given by the application */

        if (ICF_NULL < p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->header_list.count)
        {
            if (ICF_FAILURE == icf_ssa_insert_header_list_in_sip_msg(
                        p_ssa_pdb, &(p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->header_list),
                        ICF_TRUE, p_overlap_trans_info->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }
        }

        if (ICF_NULL < p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->tag_list.count)
        {
            if (ICF_FAILURE == icf_ssa_insert_tag_list_in_sip_msg(
                        p_ssa_pdb, &(p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->tag_list),
                        ICF_TRUE, p_overlap_trans_info->pSipMsg))
            {
                ret_val = ICF_FAILURE;
            }
        }
#ifdef ICF_IPSEC_ENABLED
        /* if SA exists, send the Securiy-Verify Header in the outgoing MESSAGE */
        if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
                /* if SA exits */
                (ICF_NULL != p_ssa_pdb->p_sa_context))
        {
            /* insert security-verify header in the otg OPTIONS */
            if (ICF_FAILURE == icf_ssa_get_sec_verify_string (p_ssa_pdb,&p_value,
                        p_ssa_pdb->p_sa_context))
            {
                ret_val = ICF_FAILURE;
            }
            else if (ICF_FAILURE ==     
                    icf_ssa_insert_unknown_hdr_in_sip_msg (
                        p_ssa_pdb,
                        (icf_uint8_t*)"Security-Verify",
                        p_value, 
                        p_ssa_pdb->p_ssa_ctx->p_call_obj->pUacTransaction->pSipMsg))
            {
                ICF_PRINT(("\nFailed to insert unknown header \
                            (Security-Verify)"));
                ret_val = ICF_FAILURE;
            }
            if (ICF_NULL != p_value)
            {
                ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
                        p_value,
                        ICF_MEM_COMMON,
                        p_ssa_pdb->p_ecode,
                        ret_val)
            }
            if (ICF_SUCCESS == ret_val )
            {
                ICF_PRINT(("\nSecurity Verify Header inserted to \
                            outgoing MESSAGE "));
            }
        }
#endif

        if (ICF_NULL < p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->body_list.count)
        {
            if (ICF_FAILURE == icf_ssa_insert_body_list_in_sdf_list(
                        p_ssa_pdb,&(p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->body_list) , 
                        ICF_TRUE, &(p_overlap_trans_info->slMsgBodyList)))
            {
                sdf_ivk_uaClearTransaction(\
                        p_ssa_pdb->p_ssa_ctxt->p_call_obj,\
                        p_overlap_trans_info,\
                        "OPTIONS",\
                        Sdf_en_uacTransaction);

                return ICF_FAILURE;

            }
            /*Message body header changes Start*/
            else if ( ICF_FAILURE == \
                    icf_ssa_SetAllMsgBody(\
                        Sdf_co_null,p_overlap_trans_info,\
                        (icf_int8_t *)p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->\
                        body_list.content_type.str,\
                        &sdf_error))
            {
                sdf_ivk_uaClearTransaction(\
                        p_ssa_pdb->p_ssa_ctxt->p_call_obj,\
                        p_overlap_trans_info,\
                        "OPTIONS",\
                        Sdf_en_uacTransaction);

                return ICF_FAILURE;
            }
        }

       	options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
        if(ret_val == ICF_FAILURE)
        {
        }
        else if(ICF_FAILURE == icf_cfg_fetch_app_conf(
											p_ssa_pdb->p_glb_pdb,
											p_ssa_ctx->p_call_ctxt->app_id,
											&(p_ssa_pdb->p_app_conf),
											p_ssa_pdb->p_ecode))
			{
				sdf_ivk_uaClearTransaction(p_ssa_ctx->p_call_obj,
					p_overlap_trans_info, "OPTIONS", Sdf_en_uacTransaction);
				ret_val = ICF_FAILURE;
			}            
            else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                        p_ssa_ctx->p_call_obj,
                        p_overlap_trans_info->pSipMsg,
                        &options,&sdf_error))
            {
                sdf_ivk_uaClearTransaction(p_ssa_ctx->p_call_obj,
                        p_overlap_trans_info,"OPTIONS",
                        Sdf_en_uacTransaction);
                ret_val = ICF_FAILURE;
            }
            else
            {
		sdf_ivk_uaEndTransaction(p_ssa_ctx->p_call_obj,
                        p_overlap_trans_info, "OPTIONS",&sdf_error);
                	ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
			stat_requests.stat_outbound_req.stats_indlg_OPTION);

            }

    }
    }

    return ret_val;
}

ICF_SIC_END


ICF_HANDLE_REINVITE_START
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_modify_req
 **
 ** DESCRIPTION:        This function is called to send UPDATE/RE-INVITE to
 **                     the remote party.The decision as to whether to send
 **                     an UPDATE or RE-INVITE is made on the basis of a 
 **                     configuration parameter
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_call_modify_req(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;

    icf_glb_pdb_st               *p_glb_pdb = ICF_NULL;

    icf_call_ctx_st              *p_call_ctxt = ICF_NULL;

    icf_ssa_ctxt_st              *p_ssa_ctxt = ICF_NULL;

    icf_config_data_st           *p_glb_cfg_data = ICF_NULL;

    icf_internal_msg_st          *p_internal_msg = ICF_NULL;
    Sdf_ty_s8bit                    display_name[ICF_MAX_STR_LEN] = "\0"; 
    Sdf_ty_s8bit                    tpt_scheme[ICF_MAX_STR_LEN] = "\0";
    icf_uint8_t                     *p_contact_str = ICF_NULL;
    Sdf_st_initData                 *p_callobj_spec_profile = ICF_NULL; 
    icf_nw_otg_media_change_req_st *p_media_chng_req = ICF_NULL;

    icf_uint8_t                  method[ICF_MAX_SIZE_INT_USAGE_STR],
    hold_option = ICF_SSA_NO_HOLD;

    Sdf_st_callObject               *p_call_obj = ICF_NULL;
    Sdf_st_error                    sdf_error;
    Sdf_st_overlapTransInfo         *p_overlap_txn = Sdf_co_null;
    SipMessage	                    *p_sip_msg = Sdf_co_null;
    SdpMessage                      *p_sdp_msg =Sdf_co_null ;

    icf_int8_t						*p_content_type = ICF_NULL;
    icf_uint8_t						count = 0;
    /* Start: Addition for Per Line Features */
    icf_line_id_t                     line_id;
    icf_error_t                     ecode; 
    icf_line_data_st                *p_line_data = ICF_NULL;
    /* End: Addition for Per Line Features */


    if (ICF_NULL == p_ssa_pdb)
    {
        return ICF_FAILURE;
    }
    p_glb_pdb = p_ssa_pdb->p_glb_pdb;

    if (ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    } 
    p_call_ctxt = p_glb_pdb->p_call_ctx;

    if (ICF_NULL == p_call_ctxt)
    {
       return ICF_FAILURE; 
    }

    ICF_ASSERT(p_glb_pdb, p_call_ctxt)

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

     
 
     p_internal_msg = p_ssa_pdb->p_internal_msg;
     p_media_chng_req =
        (icf_nw_otg_media_change_req_st*)(p_internal_msg->p_msg_data);

     p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_call_ctxt->p_sip_ctx);
     p_call_obj = p_ssa_ctxt->p_call_obj;
     p_glb_cfg_data = p_ssa_pdb->p_glb_cfg_data;
     p_callobj_spec_profile = p_ssa_pdb->p_ssa_ctxt->p_call_obj->pInitData;
                                    
    /* Start: Added code for obsolete hold parameter */
    if (ICF_FAILURE == icf_ssa_get_line_id_from_glb_pdb( p_glb_pdb,
                                                         p_ssa_ctxt,
                                                         &line_id,
                                                         &ecode) )
     {
       ret_val = ICF_FAILURE;
     }
     if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk( p_glb_pdb,
                                                      line_id,
                                                      (icf_line_data_st **)(&p_line_data),
                                                      &ecode) )
     {
        ret_val = ICF_FAILURE;
     }
     if(ICF_SUCCESS == ret_val)
     p_glb_cfg_data->hold_option = p_line_data->hold_option;
     /* End: Added for Obsolete Hold Parameter */

#ifdef ICF_SESSION_TIMER
    if((ICF_TRUE == p_ssa_ctxt->p_call_ctxt->session_timer_enable) && \
	   (ICF_TRUE == p_ssa_ctxt->session_timer_expired_refresh))
    {
		p_ssa_pdb->p_internal_msg->msg_hdr.msg_id = 
			ICF_NW_OTG_SESSION_REFRESH_IND;
		p_ssa_pdb->p_internal_msg->p_msg_data = ICF_NULL;
		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
    }
#endif

    if(p_call_ctxt->common_bitmask & ICF_CALL_CONNECT_DONE)
	{
		switch(p_media_chng_req->mesg_type)
        {
            case ICF_NW_OTG_SEND_UPDATE:
                icf_port_strcpy(method,(icf_uint8_t *)"UPDATE");
                break;
            case ICF_NW_OTG_SEND_RE_INVITE:
                 icf_port_strcpy(method,(icf_uint8_t *)"INVITE");
                break;
            default:        
                if(ICF_CFG_CALL_MODIFY_METHOD_INVITE ==
                    p_glb_cfg_data->call_modify_req_method)
                {
                    icf_port_strcpy(method,(icf_uint8_t *)"INVITE");
                }
                else
                {
                    icf_port_strcpy(method,(icf_uint8_t *)"UPDATE");
                }
        }/* End of switch case.*/
	}
	else
	{
		icf_port_strcpy(method,(icf_uint8_t *)"UPDATE");
	}

   /*Check if Contact header is present in call context header list*/
    if (ICF_TRUE == icf_ssa_is_contact_present_in_hdr_list(
                             p_ssa_pdb, 
                             &p_contact_str, 
                             (icf_uint8_t *)display_name, 
                             (icf_uint8_t *)tpt_scheme))
    {
       /*If Contact header present then delete previous contact from init_data
        *and call AddDefaultContact to add application specific Contact header*/
        if (Sdf_co_fail == sdf_listDeleteAt(
                        &(p_callobj_spec_profile->slContact), 0, &sdf_error))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = (icf_return_t)sdf_ivk_uaAddDefaultContact(
                                 p_callobj_spec_profile,
                                 (icf_int8_t *)display_name,
                                 (icf_int8_t *)p_contact_str,
                                 ICF_NULL,
                                 (icf_int8_t *)tpt_scheme,
                                 &sdf_error);
         }
        /* spr16521 */
        ICF_MEMFREE(
            p_ssa_pdb->p_glb_pdb,
            p_contact_str,
            ICF_MEM_COMMON,
            p_ssa_pdb->p_ecode,
            ret_val)
     }
	
    /* Accept & Supported Header handling */
    /* Clear previous accept and supported from profile */
    /* Add new ones */
    if(ICF_FAILURE == ret_val)
    {
       /*Nothing to be done*/
    }
    else if(ICF_FAILURE == icf_ssa_refresh_supported_content_and_events(p_ssa_pdb,
        &(p_ssa_pdb->p_glb_pdb->p_call_ctx->header_list),
        p_ssa_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    

    else if (Sdf_co_fail == sdf_ivk_uaStartTransaction(
                p_call_obj,
                &p_overlap_txn,
                (const icf_int8_t *)method,
                &sdf_error))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (Sdf_co_fail == 
            sdf_ivk_uaMakeTransaction((icf_int8_t *)method, 
                p_call_obj, 
                p_overlap_txn, 
                &sdf_error))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        sdf_ivk_uaClearTransaction(
                p_call_obj,
                p_overlap_txn,
                (const icf_int8_t *)method,
                Sdf_en_uacTransaction);
        ret_val = ICF_FAILURE;
    }
    /*twc_update handling*/
    else if (!(ICF_CALL_MOD_REQ_WO_SDP &
             p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask))
    {
        SipOptions  dOptions;
        /* 
         * Flush the message bodies (if any) stored in the 
         * UAC transaction of the call object
         */
        sdf_listDeleteAll(
                &(p_call_obj->pUacTransaction->slMsgBodyList),&sdf_error);


        dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    if(ICF_CALL_TYPE_T38 & p_call_ctxt->call_type)
	{
		/* Update SSA Call type
		*/
		p_ssa_pdb->p_ssa_ctxt->call_type |= ICF_CALL_TYPE_T38;
	}

	if ( ICF_FAILURE == 
                icf_ssa_form_sdp_from_stream_sdp_info(
                    &p_sdp_msg,
                    p_ssa_pdb,
                    hold_option,
                    &sdf_error))
        {
            sdf_ivk_uaClearTransaction(
                    p_call_obj,
                    p_overlap_txn,
                    (const icf_int8_t *)method,
                    Sdf_en_uacTransaction);
            ret_val = ICF_FAILURE;
        }
		
        else
        {
			/*Message body header changes Start*/
				/*Set the count to the number of message bodies in the request
				*/
			count = (icf_uint8_t)p_call_ctxt->body_list.count;
			if(0 == count)
			{
			    /*Fix done for SPR-18722 : CSR-1-6068965*/
				Sdf_mc_strdup(p_content_type,(icf_uint8_t *)"application/sdp");
			}
			else
			{
				 if(p_call_ctxt->body_list.content_type.str_len != 0)
				 {
				     /*Fix done for SPR-18722 : CSR-1-6068965*/
                     Sdf_mc_strdup(p_content_type,
                                   (icf_uint8_t *)p_call_ctxt->body_list.content_type.str);
				 }
			}


            /* 
             * If method is UPDATE set SDP in overlapped transaction
             * else set SDP in UAC transaction
             */
            if (0 == icf_port_strcmp(method,(icf_uint8_t *)"UPDATE"))
            {
                if (Sdf_co_fail == sdf_ivk_uaSetSdpInOverlapTransaction(
                            p_overlap_txn, 
                            p_sdp_msg, &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
/*CALL_MODIFY_FIX_8JUNE_START*/
				else if (ICF_FAILURE == icf_insert_header_tag_body_list(
					p_ssa_pdb, ICF_TRUE,
					ICF_NULL, p_overlap_txn))
				{
            
					sdf_ivk_uaClearTransaction(p_call_obj,
						p_overlap_txn, 
						(const icf_int8_t *)method, Sdf_en_uacTransaction);
					ret_val = ICF_FAILURE;
				}
/*CALL_MODIFY_FIX_8JUNE_END*/
                else
                {
                    p_sip_msg = p_overlap_txn->pSipMsg;
                }
            }
            else
            {
                if(Sdf_co_fail == sdf_ivk_uaSetSdpInTransaction(
                            p_call_obj->pUacTransaction, p_sdp_msg, 
                            &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
/*CALL_MODIFY_FIX_8JUNE_START*/
				else if (ICF_FAILURE == icf_insert_header_tag_body_list(
					p_ssa_pdb, ICF_TRUE,
					p_call_obj->pUacTransaction, ICF_NULL))
				{
            
					sdf_ivk_uaClearTransaction(p_call_obj,
						p_overlap_txn, 
						(const icf_int8_t *)method, Sdf_en_uacTransaction);
					ret_val = ICF_FAILURE;
				}
/*CALL_MODIFY_FIX_8JUNE_END*/
                else
                {
                    p_sip_msg = p_call_obj->pUacTransaction->pSipMsg;
                }
            }

            if(ICF_FAILURE != ret_val)
            {
                /* Set the bitmask in SSA Context for update Send with SDP.
                 */
                p_ssa_ctxt->bitmask |= ICF_SSA_UPDATE_WITH_SDP;
				
				/* CSR 8061646 SPR 20231: Set the bitmask to indicate that Update sent due to 
				 * call modify
                 */
				p_ssa_ctxt->bitmask_1 |= ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY;
					
/*CALL_MODIFY_FIX_8JUNE_START*/

				

#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)     
				ICF_ASSERT(p_ssa_pdb->p_glb_pdb, (ICF_NULL != p_call_ctxt->p_preferred_identity))
				if (ICF_FAILURE == icf_ssa_form_and_insert_pref_identity_hdr(
					p_ssa_pdb, p_sip_msg))
				{
					sdf_ivk_uaClearTransaction(p_call_obj,
						p_overlap_txn, method, Sdf_en_uacTransaction);
					ret_val = ICF_FAILURE;
				}
				else if ( ICF_FAILURE == 
						icf_ssa_form_and_insert_access_nw_info_hdr(
						p_ssa_pdb, p_sip_msg))
				{
					sdf_ivk_uaClearTransaction(p_call_obj,
						p_overlap_txn, method, Sdf_en_uacTransaction);
					ret_val = ICF_FAILURE;
				}

ICF_CHECK_IMS_END
#endif


				

                /*The above API added the SDP msg body in the msg body list
                 * of the transaction. Now we need to add this to the 
                 * actual outgoing SIP Message*/
                /*
				if ( Sdf_co_fail == 
                        sdf_ivk_uaSetAllMsgBody(
                            p_call_obj->pUacTransaction,
                            p_overlap_txn,OverlapTxn
                            &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, method,
                            Sdf_en_uacTransaction);
                    ret_val = ICF_FAILURE;
                }
				*/
                if (ICF_NULL == p_content_type)
                {
                   ret_val = ICF_FAILURE;
                } 
                else if ( Sdf_co_fail == 
                        icf_ssa_SetAllMsgBody(
                            p_call_obj->pUacTransaction,\
							p_overlap_txn,/*OverlapTxn*/(icf_int8_t *)p_content_type,\
                            &sdf_error))
                {
                    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise a Major error here - UATK_PROCESSING FAILURE*/
                    ret_val = ICF_FAILURE;
                }
				/*Message body header changes end*/
				else if(ICF_FAILURE == icf_cfg_fetch_app_conf(
											p_ssa_pdb->p_glb_pdb,
											p_call_ctxt->app_id,
											&(p_ssa_pdb->p_app_conf),
											p_ssa_pdb->p_ecode))
				{
					sdf_ivk_uaClearTransaction(p_call_obj,
						p_overlap_txn, (const icf_int8_t *)method, Sdf_en_uacTransaction);
					ret_val = ICF_FAILURE;
				}
/*CALL_MODIFY_FIX_8JUNE_END*/
                else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                            p_call_obj,
                            p_sip_msg, &dOptions,&sdf_error))
                {
                     /*Reset the bitmask for Update with SDP
		     */
		    if(ICF_SSA_UPDATE_WITH_SDP & p_ssa_ctxt->bitmask)
		    {
			p_ssa_ctxt->bitmask &= ~(ICF_SSA_UPDATE_WITH_SDP);
		    }

		    sdf_ivk_uaClearTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method,
                            Sdf_en_uacTransaction);
                    /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* Stas collection of UPDATE and INVITE sent*/
                    if(icf_port_strcmp(method,(icf_uint8_t *)"UPDATE") == 0)
                    {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_requests.stat_outbound_req.stats_UPDATE);
                    }

                    else if(icf_port_strcmp(method,(icf_uint8_t *)"INVITE") == 0)
                    {
                        ICF_STATS(p_ssa_pdb->p_glb_pdb->p_stats_info->
                                stat_requests.stat_outbound_req.stats_ReINVITE);
                    }
                    sdf_ivk_uaEndTransaction(p_call_obj,
                            p_overlap_txn, (const icf_int8_t *)method, &sdf_error);
                }
            }
        }
    }
    else
    {
    /*twc_update handling*/
       SipOptions  dOptions;
       dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
       if (ICF_FAILURE == icf_insert_header_tag_body_list(
                          p_ssa_pdb, ICF_TRUE,
                          ICF_NULL, p_overlap_txn))
       {

           sdf_ivk_uaClearTransaction(p_call_obj,
                     p_overlap_txn,
                     (const icf_int8_t *)method, Sdf_en_uacTransaction);
           ret_val = ICF_FAILURE;
       }
       else
       {
          p_sip_msg = p_overlap_txn->pSipMsg;
       }
   
       if (ICF_FAILURE == ret_val)
       {}
       else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                                p_call_obj,
                                p_sip_msg, &dOptions,&sdf_error))
       {
           sdf_ivk_uaClearTransaction(p_call_obj,
           p_overlap_txn, (const icf_int8_t *)method,
           Sdf_en_uacTransaction);
           ret_val = ICF_FAILURE;
       }
       else
       {
           sdf_ivk_uaEndTransaction(p_call_obj,
           p_overlap_txn, (const icf_int8_t *)method, &sdf_error);
       }
     }
     /*twc_update handling*/

	/* Fix for Memleaks under SPR 11096
	 */
	if(ICF_NULL != p_content_type)
    {
       sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&\
                     (p_content_type),&sdf_error);
    }

    if(Sdf_co_null != p_sdp_msg)
    {
        sip_freeSdpMessage(p_sdp_msg);
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

ICF_HANDLE_REINVITE_END


/*******************************************************************************
 ** FUNCTION:    icf_ssa_process_info_cfm 
 ** DESCRIPTION:  This function process info_cfm or info_cfm err received from 
 **               CC and send response accordingly on n/w.
 **
 ******************************************************************************/
icf_return_t icf_ssa_process_info_cfm(icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    Sdf_st_overlapTransInfo         *p_ovtxn = Sdf_co_null;
    Sdf_st_error                    sdf_error;
    SipOptions                      options;
    icf_nw_inc_info_cfm_st          *p_inc_info_cfm = ICF_NULL;
                                    
    icf_ssa_ctxt_st              *p_ssa_ctxt = ICF_NULL;
    icf_uint16_t                 sdf_resp_code;

    Sdf_ty_slist*       p_slist = ICF_NULL;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    p_inc_info_cfm = (icf_nw_inc_info_cfm_st*)(p_ssa_pdb->p_internal_msg->p_msg_data);
    p_ssa_ctxt = (icf_ssa_ctxt_st*)
                 (p_ssa_pdb->p_glb_pdb->p_call_ctx->p_sip_ctx);

    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;

     sdf_resp_code = (icf_int16_t)p_inc_info_cfm->response_code;

    /*
     * Fetch the overlapped transaction pOverlapTransInfo 
     * stored in the call object corresponding to OPTIONS
     * request received
     */
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_info,Sdf_en_transactionReceived,
                p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (Sdf_co_fail == sdf_ivk_uaFormResponse(sdf_resp_code, "INFO", 
                p_ssa_ctxt->p_call_obj,
                p_ovtxn,
                Sdf_co_false,
                &sdf_error))
    {
        /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else
    { 
        if (p_inc_info_cfm->bit_mask & ICF_NW_INC_INFO_CFM_HEADER_LIST_PRESENT)
        {
           if (ICF_FAILURE == icf_ssa_insert_header_list_in_sip_msg(p_ssa_pdb,
                                                  &(p_inc_info_cfm->header_list),
                                                  ICF_FALSE,
                                                  p_ovtxn->pSipMsg))
           {
              ret_val = ICF_FAILURE;
           }
        }
        if ((ret_val != ICF_FAILURE) && (p_inc_info_cfm->bit_mask & ICF_NW_INC_INFO_CFM_TAG_LIST_PRESENT))
        {
           if (ICF_FAILURE == icf_ssa_insert_tag_list_in_sip_msg(p_ssa_pdb,
                                                  &(p_inc_info_cfm->tag_list),
                                                  ICF_FALSE,
                                                  p_ovtxn->pSipMsg))
           {
              ret_val = ICF_FAILURE;
           }
        }
        if ((ret_val != ICF_FAILURE) && (p_inc_info_cfm->bit_mask & ICF_NW_INC_INFO_CFM_BODY_LIST_PRESENT))
        {
           p_slist   = &(p_ovtxn->slMsgBodyList);
           if (ICF_FAILURE == icf_ssa_insert_body_list_in_sdf_list(p_ssa_pdb,
                                                  &(p_inc_info_cfm->body_list),
                                                  ICF_FALSE,
                                                  p_slist))
           {
              ret_val = ICF_FAILURE;
           }
           else if (ICF_FAILURE == icf_ssa_SetAllMsgBody(Sdf_co_null, p_ovtxn,
					(icf_int8_t *)p_inc_info_cfm->body_list.content_type.str,
					&sdf_error)) 
           {
              ret_val = ICF_FAILURE;
           }              
        }
        if (ret_val != ICF_FAILURE)
        {
	   if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
			p_ssa_ctxt->p_call_obj,
			p_ovtxn->pSipMsg,
			&options,&sdf_error))
	   {
	        /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
	        ret_val = ICF_FAILURE;
	   }
        } 
    }
#ifdef ICF_STAT_ENABLE
   if(ret_val == ICF_SUCCESS)
	{
		icf_ssa_resp_stats(p_ssa_pdb,(icf_uint8_t *)"INFO",sdf_resp_code,1);
	}
#endif
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 ** FUNCTION:    icf_ssa_process_detach_call_ctx_frm_sip_ctx 
 ** DESCRIPTION:  This function set NULL to ssa_ctxt->p_call_ctx
 **
 ******************************************************************************/
icf_return_t    icf_ssa_process_detach_call_ctx_frm_sip_ctx(
       icf_ssa_pdb_st     *p_ssa_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    if ((ICF_NULL != p_ssa_pdb->p_ssa_ctxt) && (ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_call_ctxt))
    {
        p_ssa_pdb->p_ssa_ctxt->p_call_ctxt = ICF_NULL;
    }
    return ret_val;
}


/*******************************************************************************
 * FUNCTION:        icf_ssa_process_options_abort_req
 * DESCRIPTION:     This function aborts the OPTIONS transaction
 *                  1.Invoke the utility function which aborts the OPTIONS trans.
 * 
 ******************************************************************************/
icf_return_t icf_ssa_process_options_abort_req(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;
    Sdf_st_error                sdf_error;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if ( (ICF_NULL == p_ssa_pdb->p_ssa_ctxt) || 
	(ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_obj))
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        /*Raise Trace here*/
        return  ICF_SUCCESS;
    }
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_options,Sdf_en_transactionSent,
                p_ssa_pdb->p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    /*
     * Even if we can't fetch the matching overlap transaction,
     * we should return SUCCESS as it is possible that the
     * OPTIONS transaction timed out and has been removed from
     * the list of overlapped transactions
     */
    if (ICF_FAILURE != ret_val)
    {
        sdf_ivk_uaAbortTransaction(p_ssa_pdb->p_ssa_ctxt->p_call_obj, 
                p_ovtxn->dTxnId, Sdf_co_null,&sdf_error);
    }
    /* Always return SUCCESS as it is best effort */
    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/* SPR 16019 */
/*******************************************************************************
 * FUNCTION:        icf_ssa_process_register_abort_req
 * DESCRIPTION:     This function aborts the REGISTER transaction
 *                  1.Invoke the utility function which aborts the REGISTER trans.
 * 
 ******************************************************************************/
icf_return_t icf_ssa_process_register_abort_req(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    Sdf_st_overlapTransInfo     *p_ovtxn = Sdf_co_null;
    Sdf_st_error                sdf_error;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if ( ICF_NULL == p_ssa_pdb->p_ssa_ctxt->p_call_obj)
    {
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        /*Raise Trace here*/
        return  ICF_SUCCESS;
    }
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_register,Sdf_en_transactionSent,
                p_ssa_pdb->p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    /*
     * Even if we can't fetch the matching overlap transaction,
     * we should return SUCCESS as it is possible that the
     * REGISTER transaction timed out and has been removed from
     * the list of overlapped transactions
     */
    if (ICF_FAILURE != ret_val)
    {
        sdf_ivk_uaAbortTransaction(p_ssa_pdb->p_ssa_ctxt->p_call_obj, 
                p_ovtxn->dTxnId, Sdf_co_null,&sdf_error);
    }
    /* Always return SUCCESS as it is best effort */
    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}
