/*-----------------------------------------------------------------------------
 *
 * File name        : icf_mrm.c
 *
 * Purpose          : The file contains the code to implement the functionality
 *                    of MRM 
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  18 DEC 2004   Sumit Gupta                File created and initial version
 *  09 MAY 2005   Sumit Gupta      SPR 7516  MRM would dump all the messages 
 *                                           other than those comming from 
 *                                           external module conf or external 
 *                                           module SM in case init complete is  
 *                                           not done. We would also not dump 
 *                                           any of the timer messages
 *  11 MAY 2005  Sumit Gupta       SPR 7542  Handle GET_CALL_LOG API from PA.
 *                                           Would be given to SM not PA like 
 *                                           other API from PA
 *  08 DEC 2005  Rohit Aggarwal    ICF CDD   Code changes for routing platform
 *                                           messages from ES to NM
 *  08 Feb 2006	 Aman Arora		   ICF rel 2.1.2 fixed for configuring AMR mode
 *											     in MM at init time
 *  20 Jun 2007  Tarun Gupta       SPR 15133 Change in function icf_process_message:
 *                                           Added a check to prevent IPTK from sending
 *                                           unexpected_msg_ind to application in case
 *                                           of SIC and SM messages.Eg->restart_req
 *  13-Jul-2007  Abhishek Dhammawat Rel7.0   Added functionality for XCONF ftr
 *  20-sep-2007  Abhishek Dhammawat SPR 16566 Modified icf_process_mesg that if
 *                                            create_xconf_req comes and
 *                                            call_ctx is not null,
 *                                            then do not proceed further.
 *  8-Dec-2007  Abhishek Dhammawat  SPR 17668 Modified icf_process_mesg for
 *                                            ignore_msg_flag handling.
 * 17-Jan-2008  Neha Gupta          SPR 18033 Added support for func failure 
 *					      simulation for UT.
 * 24-Jan-2008  Kamini Gangwani     Rel 8.0   Modified function icf_init
 * 11-Mar-2008  Vipul Varshney      SPR 18122   AKA Enhancement
 * 28-Mar-2008  Tarun Gupta         SPR 18311 Merged Publish-Message Enhancements
 * 8-Apr-2008   Sumant Gupta        SPR:18355 Windows porting Merging
 * 12-Apr-2008  Alok Tiwari         SPR-18358 Added code for klocwork warning
                                              of type FALSE-POSITIVE.  
 * 28-Apr-2008  Kamini Gangwani     SPR-18468 Changes for icf.cfg cleanup
 * 11-May-2008  Rajiv Kumar         SPR-17506 Rejecting the REFER REQ received
 *                                            from application
 * 17-May-2008  Amit Sharma         SPR:18577 Initializing sic,rgm ctx and line_id in p_glb_pdb
 * 04-Nov-2008  Tarun Gupta         SPR 19189 Merged Rel 7.1.2 SPR 18880
 * 04-Nov-2008  Rajiv Kumar         SPR 19188 Rel 7.1.2 SPR merging (SPR
 *                                            19156)
 * 30-Jan-2009  Rajiv Kumar         IPTK Rel8.1 Changes done for testing UATK
 *                                            API under ICF_UT_TEST flag
 * 10-feb-2009  Anurag Khare        ICF Rel 8.1 changes done for OPTIONS \
 *                                              enhancements
 * 10-Feb-2009  Tarun Gupta       Rel 8.1 LLD SDP Tunnelling Enhancements
 * 27-Feb-2009  Preksha           Rel 8.1 Modified icf_process_mesg for ICF_UT_SIMULATE .
 * 04-Mar-2009 Saurabh Gupta        SPR 19349   Klocworks fix
 * 14-Mar-2009 Alok Tiwari          IPTK Rel8.1 modified function
 *                                         icf_mrm_recv_msg to fix the 
 *                                         crash if p_glb_port_info is NULL.
 * 31-Mar-2009  Tarun Gupta         Rel 8.2     REFER/NOTIFY Enhancements
 * 01-Apr-2009  Kamal Ashraf        Rel 8.2    Klocwork Fix
 * 06-Apr-2009  Rajiv Kumar         SPR 19423  Klocwork Warning Fix.
 * 08-Apr-2009  Tarun Gupta         SPR 19433  Modified icf_process_mesg
 * 28-May-2009  Tarun Gupta         Rel 8.2    Async NOTIFY Support
 * 02-Jun-2009  Preksha             Rel 8.2    Asynchronus Message enh
 * 11-Jun-2009  Ashutosh Mohan      SPR 19737  Changes done for GCC 4.1 warnings removal
 * 12-Sep-2009  Rajiv Kumar         SPR 19999  Merged CSR 1-7467002
 * 08-Oct-2009 Abhishek Dhammawat   SPR 20063  Merged CSR 1-7407479 
 * 27-May-2010 Preksha              SPR 20412  Merged CSR 1-7873034 
 * 29-May-2010 Alok Tiwari          SPR 20412  Merged CSR 1-8223807 
 * Copyright (c) 2010, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_common_defs.h"
#include "icf_mrm_prototypes.h"
#include "icf_mrm_defs.h"
#include "icf_mrm_types.h"
#include "icf_macro.h"
#include "icf_port_types.h"
#include "icf_port_defs.h"
#include "icf_port_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#include "icf_clib.h"
#include "icf_mmi_types.h"
#include "icf_ecode.h"


#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif

/************************************************************************
 *
 * FUNCTION NAME: icf_mrm_init
 *
 * DESCRIPTION:  This function would create allocate memory for global PDB and 
 *               initialize global data non-scalable data structures
 *
 * RETURNS:      Returns Success or Failure 
 *
 ************************************************************************/
#if defined ICF_PORT_WINCE
icf_return_t 
icf_mrm_init(
		IN	icf_uint8_t sys_resource_limit,
		IN	icf_uint8_t logs_indication,
        OUT icf_void_t **p_p_glb_pdb)
#else
icf_return_t 
icf_mrm_init(
		IN	icf_uint8_t sys_resource_limit,
        OUT icf_void_t **p_p_glb_pdb)
#endif
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_error_t   error = ICF_ERROR_NONE;

	ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Entering function <icf_mrm_init>\n"));
        
    p_glb_pdb = (icf_glb_pdb_st *)icf_cmn_static_memget(p_glb_pdb,
            sizeof(icf_glb_pdb_st),&error);

    if(ICF_NULL == p_glb_pdb)
	return ICF_FAILURE;

    icf_mrm_init_glb_pdb(p_glb_pdb);
	*p_p_glb_pdb = p_glb_pdb;
     
	/* system memory and timer scaling factor */
	p_glb_pdb->sys_resource_limit = sys_resource_limit;
	
#if defined ICF_PORT_WINCE
	p_glb_pdb->logs_indication = logs_indication;
#endif

    /*Call dbm_init for creation of global data and initializing 
     *non-scalable datastructures;*/
    if (ICF_FAILURE == 
            icf_dbm_init(p_glb_pdb,&error))
    {
		ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Exitting function <icf_mrm_init> with FAILURE\n"));
        icf_mrm_static_deinit(*p_p_glb_pdb);
        ret_val = ICF_FAILURE;
    }
	else
	{
		ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Exitting function <icf_mrm_init> with SUCCESS\n"));
	}
	
    return ret_val;
    
}

/************************************************************************
 *
 * FUNCTION NAME: icf_mrm_static_deinit
 *
 * DESCRIPTION:  This function would free memory allocated for global PDB
 *
 * RETURNS:      Nothing 
 *
 ************************************************************************/
icf_return_t 
icf_mrm_static_deinit(
        INOUT icf_void_t *p_glb_pdb)
{
    icf_return_t ret_val=ICF_TRUE;
    icf_error_t ecode;
 
    if (ICF_NULL == p_glb_pdb)
    {
      return ICF_FAILURE;
    } 
    
    /*Later we could call DBM's function to free all static allocations in 
     * global data and the global data block thereafter*/
#ifdef ICF_STAT_ENABLE
    ICF_STATIC_MEMFREE( (icf_glb_pdb_st *)p_glb_pdb,
           ((icf_glb_pdb_st *)p_glb_pdb)->p_stats_info,
           &ecode,ret_val)
#endif           

    ICF_STATIC_MEMFREE(p_glb_pdb,p_glb_pdb,
            &ecode,ret_val)
   return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_mrm_init_glb_pdb
 *
 * DESCRIPTION:  This function would initailize pdb
 *
 * RETURNS:      Nothing
 *
 ************************************************************************/
icf_void_t
icf_mrm_init_glb_pdb(
        INOUT icf_glb_pdb_st *p_glb_pdb)

{
#ifdef ICF_STAT_ENABLE
   icf_error_t error = ICF_ERROR_NONE;
   p_glb_pdb->p_stats_info =
          (icf_stats_st *)icf_cmn_static_memget(p_glb_pdb,
            sizeof(icf_stats_st),&error);
#endif
    p_glb_pdb->p_call_ctx = ICF_NULL;
    p_glb_pdb->p_sic_ctx = ICF_NULL;
    p_glb_pdb->p_rgm_ctx = ICF_NULL;
    p_glb_pdb->line_id = ICF_INVALID_LINE_ID;
    p_glb_pdb->p_recv_msg = ICF_NULL;
    p_glb_pdb->p_glb_data = ICF_NULL;
    p_glb_pdb->p_msg_lst = ICF_NULL;
    p_glb_pdb->curr_module_index = ICF_NULL;

	p_glb_pdb->sys_resource_limit = ICF_DEFAULT_SYS_RESOURCE_LIMIT;
#ifdef ICF_TRACE_ENABLE
    p_glb_pdb->trace_info.status = ICF_FALSE;
    p_glb_pdb->trace_info.level = ICF_TRACE_LEVEL_DETAILED | 
								  ICF_TRACE_LEVEL_BRIEF;
    p_glb_pdb->trace_info.type = 0 |
            ICF_TRACE_TYPE_SYSTEM|
            ICF_TRACE_TYPE_INTERFACE|
            ICF_TRACE_TYPE_SIP|
            ICF_TRACE_TYPE_CALL_CONTROL|
            ICF_TRACE_TYPE_MESSAGE_CONTROL|
            ICF_TRACE_TYPE_MEDIA|
            ICF_TRACE_TYPE_CONFIG|
            ICF_TRACE_TYPE_REGISTRATION|
            ICF_TRACE_TYPE_SM|
            ICF_TRACE_TYPE_CALL_HOLD_RESUME|
            ICF_TRACE_TYPE_THREE_WAY_CALL|
            ICF_TRACE_TYPE_CALL_TRANSFER_UNATTENDED|
            ICF_TRACE_TYPE_CALL_TRANSFER_ATTENDED|
            ICF_TRACE_TYPE_REFER|
            ICF_TRACE_TYPE_CALL_FORWARDING|
            ICF_TRACE_TYPE_CALL_WAITING|ICF_TRACE_TYPE_SIP_STACK |
            ICF_TRACE_TYPE_GENERAL;
#endif
#ifdef ICF_ERROR_ENABLE

#ifndef ICF_LOAD_STAT
	p_glb_pdb->error_info.status = ICF_TRUE;
#else
        p_glb_pdb->error_info.status = ICF_FALSE;
#endif
    p_glb_pdb->error_info.level = ICF_ERROR_LEVEL_INFO;
    p_glb_pdb->error_info.type = 0 |
            ICF_ERROR_TYPE_SYSTEM|  
            ICF_ERROR_TYPE_INTERFACE|
            ICF_ERROR_TYPE_SIP|
            ICF_ERROR_TYPE_CALL_CONTROL|
            ICF_ERROR_TYPE_MEDIA|
            ICF_ERROR_TYPE_CONFIG|
            ICF_ERROR_TYPE_REGISTRATION|
            ICF_ERROR_TYPE_SM|
            ICF_ERROR_TYPE_CALL_HOLD_RESUME|
            ICF_ERROR_TYPE_THREE_WAY_CALL|
            ICF_ERROR_TYPE_CALL_TRANSFER_UNATTENDED|
            ICF_ERROR_TYPE_CALL_TRANSFER_ATTENDED|
            ICF_ERROR_TYPE_REFER|
            ICF_ERROR_TYPE_CALL_FORWARDING|
            ICF_ERROR_TYPE_CALL_WAITING|
            ICF_ERROR_TYPE_GENERAL;
#endif
#ifndef ICF_KLOCWORK_SHOW_FALSE_POSITIVE
        p_glb_pdb->junk.p_next = &(p_glb_pdb->junk_1);
        p_glb_pdb->junk_1.p_next = &(p_glb_pdb->junk);
        p_glb_pdb->p_junk_temp = &(p_glb_pdb->junk);
#endif
        /*CSR 1-7407479 merge */
        p_glb_pdb->no_of_call_obj = 0;
}
       
#ifndef ICF_PORT_SYMBIAN
 /************************************************************************
 *
 * FUNCTION NAME: icf_mrm_recv_msg
 *
 * DESCRIPTION:  The function would call the porting layer's recv function 
 *               for reading data from socetets , pipes etc
 *
 * RETURNS:      Returns Success or Failure
 *          
 ************************************************************************/
icf_return_t
icf_mrm_recv_msg(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_error_t *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_void_t *p_port_data = ICF_NULL;
    #ifdef ICF_TRACE_ENABLE
    icf_global_port_info_st *p_glb_port_data = ICF_NULL;
    icf_uint8_t     p_transp_addr_str[100] = "";
    icf_config_data_st      *p_config_data = ICF_NULL;

    if((ICF_NULL == p_glb_pdb) ||
       (ICF_NULL == p_glb_pdb->p_glb_data) ||
       (ICF_NULL == p_glb_pdb->p_glb_data->p_glb_port_info))
    {
      ICF_PRINT(((icf_uint8_t *)"\n[Function:icf_mrm_recv_msg]:"
                   "p_glb_data or p_glb_port_info is NULL"))
      return  ICF_SUCCESS;
    }

    p_glb_port_data =
        (icf_global_port_info_st *)p_glb_pdb->p_glb_data->p_glb_port_info;
    #endif


     ICF_FUNCTION_ENTER(p_glb_pdb)
         
     if(ICF_FAILURE == icf_dbm_get_port_data(p_glb_pdb,
                    &p_port_data,p_ecode)) 
     {
         ret_val = ICF_FAILURE;
     }
     else
     {
         /* Keep on checking till a message is received*/
         while(ICF_NULL == p_glb_pdb->p_msg_lst->p_msg)
         {
#ifdef ICF_LOAD_STAT
             
             /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19156)*/
             /* SPR 19156 : Passing a local variable to the port function */
             icf_time_t new_time = (icf_time_t)p_glb_pdb->load_data.time_new;
             icf_port_get_current_time(&new_time);
             p_glb_pdb->load_data.time_new = (icf_time_t)new_time;

             if(!p_glb_pdb->load_data.init_time)
             {
                 p_glb_pdb->load_data.init_time = p_glb_pdb->load_data.time_new;
             }
             if((p_glb_pdb->load_data.time_new - p_glb_pdb->load_data.time_old) >=
                     ICF_LOAD_STAT_TIMER)
             {
                 icf_cmn_print_load_stats(p_glb_pdb);
                 p_glb_pdb->load_data.time_old=p_glb_pdb->load_data.time_new;
             }  
#endif
             /*Function to check timer expiry*/  
             icf_port_check_timer_expiry(p_port_data,p_ecode);

			 /* If memory is allocated by timer exp function, return */
			 if(ICF_NULL != p_glb_pdb->p_msg_lst->p_msg)
				 return ret_val;


             /* call the Encapsulating Shell's receive function for polling for 
              * message on the Pipes and Sockets */ 

             if (ICF_FAILURE == 
                     icf_es_recv(
                         p_port_data,
                         (icf_uint16_t const)ICF_SELECT_TIMER,
                         (icf_msg_list_st *)p_glb_pdb->p_msg_lst,p_ecode))
             {
#ifdef ICF_TRACE_ENABLE
        ret_val = icf_dbm_get_module_glb_data(
                                   p_glb_pdb,
                                   (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                   (icf_void_t *)&p_config_data, p_ecode);
        if( ICF_SUCCESS == ret_val)
         {
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
             if ( ICF_TRANSPORT_TYPE_UDP == p_glb_port_data->icf_trace_ipc_info.self_mode )
             {
                ICF_MRM_APP_TRACE( p_glb_pdb, ICF_TRACE_UDP_RECV_ERROR)
             }
            else if( ICF_TRANSPORT_TYPE_TCP == p_glb_port_data->icf_trace_ipc_info.self_mode)
             {
                ICF_MRM_APP_TRACE( p_glb_pdb, ICF_TRACE_TCP_RECV_ERROR) 
             }
            else if( ICF_TRANSPORT_TYPE_TLS == p_glb_port_data->icf_trace_ipc_info.self_mode )
             {
                ICF_MRM_APP_TRACE( p_glb_pdb, ICF_TRACE_TLS_RECV_ERROR)
             }
          }
#endif          
#ifdef ICF_STAT_ENABLED
                 ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MSG_RECV_FAIL)
#endif
#ifdef MRM_UT  
                 ICF_SET_ECODE(p_glb_pdb,ICF_NULL,
                         ICF_MSG_RECEIVE_ERROR,p_ecode)

  
                  ret_val = ICF_FAILURE;     
                 return ret_val;
#endif
              }
#ifdef ICF_TRACE_ENABLE
       else
       {
         ret_val = icf_cmn_trace_msg(p_glb_pdb,p_ecode);
         /* FAILURE in reporting trace is not fatal, 
              * so ignore the ret_val.
              */
             ret_val = ICF_SUCCESS;
       }
#endif       
         }/*End While*/
     }/*End else */

     ICF_FUNCTION_EXIT(p_glb_pdb)

     return ret_val;
}
#endif
/************************************************************************
 *
 * FUNCTION NAME: icf_process_message
 *
 * DESCRIPTION:  Entry point function for MRM module
 *
 * RETURNS:      Success or Failure
 *
 ************************************************************************/
icf_return_t
icf_process_message(
        INOUT icf_void_t *p_data
#ifdef ICF_PORT_SYMBIAN
		,INOUT icf_msg_list_st *p_msg_lst
#endif 
        )
{
    icf_msg_st *p_msg = ICF_NULL;
    icf_module_id_t source_id = ICF_MAX_MODULE;
    icf_msg_list_st *p_temp_msg_blk = ICF_NULL;
    icf_internal_msg_st internal_msg = {{0,0,0,0,0,0},0};
    icf_error_t ecode = ICF_ERROR_NONE;
    icf_mrm_func_ptr_t func_ptr[ICF_INT_MAX_MODULE]/*={0}*/;
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_boolean_t  ignore_msg_flag = ICF_FALSE;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;
    icf_int_module_id_t dest_int_module = ICF_INT_MODULE_INVALID;
    icf_mmi_data_st			*p_mmi_data = ICF_NULL;
    icf_boolean_t   service_call_api = ICF_FALSE;
    icf_uint8_t index = 0;
 
#ifdef ICF_TRACE_ENABLE
    icf_uint8_t*   func_name_trace[ICF_INT_MAX_MODULE];
#endif

    icf_api_id_t   api_id = ICF_API_ID_UNDEFINED;
#ifndef ICF_PORT_SYMBIAN
    icf_msg_list_st msg_lst;
    icf_msg_list_st *p_msg_lst = &msg_lst;
#endif
#ifdef ICF_NW_ACTIVATE 
    icf_boolean_t       gprs_status = ICF_TRUE;
#endif
#ifdef ICF_UT_TEST
    icf_uint8_t i = 0; 
#endif

    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_data))
    
    p_glb_pdb = (icf_glb_pdb_st *)p_data;

    icf_port_memset(func_ptr, ICF_NULL,\
         sizeof(icf_mrm_func_ptr_t) * ICF_INT_MAX_MODULE);

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_MRM)

	ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Entering function <icf_mrm_process_msg>\n"));
    ICF_FUNCTION_ENTER(p_glb_pdb)

     /*Create the head node of mesg list in global PDB*/
#ifndef ICF_PORT_SYMBIAN
    p_msg_lst->p_msg = ICF_NULL;
    p_msg_lst->p_msg_next = ICF_NULL;
#endif 
    p_glb_pdb->p_msg_lst = p_msg_lst;
    
    /*Klocwork warning removal.*/
    /*Initialising functional pointer table*/
    for (index=0;index<ICF_INT_MAX_MODULE;index++)
    {
        func_ptr[index] = icf_invalid_module_handling;
    }

    /*Initialize the function pointer table*/
    func_ptr[ICF_INT_MODULE_MRM] = icf_mrm_icf_process_msg;
    func_ptr[ICF_INT_MODULE_CM]  = icf_cm_process_mesg;
    func_ptr[ICF_INT_MODULE_SSA] = icf_ssa_process_mesg;
    func_ptr[ICF_INT_MODULE_CFG] = icf_cfg_process_mesg;
    func_ptr[ICF_INT_MODULE_MMI] = icf_mmi_process_mesg;
#if defined(ICF_STAT_ENABLED) || defined (ICF_TRACE_ENABLE) ||  defined(ICF_ERROR_ENABLE)
    func_ptr[ICF_INT_MODULE_SM] = icf_sm_process_mesg;
#endif
    func_ptr[ICF_INT_MODULE_NM]  = icf_nm_process_mesg;
#ifdef IMS_CLIENT
    func_ptr[ICF_INT_MODULE_MPH]  = icf_mph_process_mesg;
#endif
    
#ifdef ICF_TRACE_ENABLE
    func_name_trace[ICF_INT_MODULE_MRM] = (icf_uint8_t *)"icf_mrm_icf_process_msg";
    func_name_trace[ICF_INT_MODULE_CM]  = (icf_uint8_t *)"icf_cm_process_mesg";
    func_name_trace[ICF_INT_MODULE_SSA] = (icf_uint8_t *)"icf_ssa_process_mesg";
    func_name_trace[ICF_INT_MODULE_CFG] = (icf_uint8_t *)"icf_cfg_process_mesg";
    func_name_trace[ICF_INT_MODULE_MMI] = (icf_uint8_t *)"icf_mmi_process_mesg";
    func_name_trace[ICF_INT_MODULE_NM]  = (icf_uint8_t *)"icf_nm_process_mesg";
#ifdef IMS_CLIENT
    func_name_trace[ICF_INT_MODULE_MPH]  = "icf_mph_process_mesg";
#endif

#endif
#ifdef ICF_TRACE_ENABLE
#if defined(ICF_STAT_ENABLED) ||  defined(ICF_ERROR_ENABLE)
    func_name_trace[ICF_INT_MODULE_SM]  = (icf_uint8_t *)"icf_sm_process_mesg";
#endif
#endif

#ifndef ICF_PORT_SYMBIAN
    /*Loop infinitely for messages till the system is up*/
/*    while (1) */
 for(;;)
    {
        /*If receive was successful then process this block*/
        if(ICF_SUCCESS == icf_mrm_recv_msg(
                    p_glb_pdb,
                    &ecode))
        {
            if(ICF_ECODE_NAT_MESSAGE_RECEIVED == ecode)
            {
                continue;
            }
#endif
            /* Loop until there are messages left to be processed*/
            do
            {

                ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MSG_RECV_SUCC)
                p_msg = p_msg_lst->p_msg;

                /* ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_msg)) */
                if (ICF_NULL == p_msg)
                {
                    ICF_FUNCTION_EXIT(p_glb_pdb)              
                    return ICF_FAILURE;
                }

                /*Retreive the source Id*/
                source_id = p_msg->hdr.source_id;
                api_id = p_msg->hdr.api_id;
                /* app_id    = p_msg->hdr.app_id; */

#ifdef ICF_UT_TEST
		        /*if api recvd is ICF_UT_SIMULATE, then fill the respective glb_pdb 
		          fields by fetching values from p_msg */ 
                if (ICF_UT_SIMULATE == api_id)
                {
                    p_glb_pdb->func_num = ((icf_ut_simulate_st *)(p_msg->
                                payload))->func_num;
                    for (i=0; i<p_glb_pdb->func_num; i++)
                    {
                        icf_port_strcpy(p_glb_pdb->func_fail_info[i].func_name.
                                str, ((icf_ut_simulate_st *)(p_msg->payload))->
                                func_fail[i].func_name.str);
                        p_glb_pdb->func_fail_info[i].func_ims_count = 
                            ((icf_ut_simulate_st *)(p_msg->payload))->
                            func_fail[i].count;
                        p_glb_pdb->func_fail_info[i].func_icf_count = 0;
                        p_glb_pdb->func_fail_info[i].ecode =  
                            ((icf_ut_simulate_st *)(p_msg->payload))->
                            func_fail[i].ecode;
                        icf_port_strcpy(p_glb_pdb->func_fail_info[i].func_level_name.
                                str, ((icf_ut_simulate_st *)(p_msg->payload))->
                                func_fail[i].func_level_name.str);
                        p_glb_pdb->func_fail_info[i].func_visited = ICF_FALSE;
                    }
                    icf_free_decoded_msg(p_msg_lst->p_msg, &ecode);
                    p_msg_lst->p_msg = ICF_NULL;
                    continue;
                }
                /*Populate the global pdb with data used while testing any
                 * UATK API*/
                if (ICF_UATK_API_TEST == api_id)
                {
                    p_glb_pdb->dVar1 = ((icf_uatk_api_test_st *)(p_msg->
                                payload))->dVar1;
                    p_glb_pdb->dVar2 = ((icf_uatk_api_test_st *)(p_msg->
                                payload))->dVar2;
                    icf_port_strcpy(p_glb_pdb->string1.str,((icf_uatk_api_test_st *)
                                (p_msg->payload))->string1.str);
                    p_glb_pdb->string1.str_len = (icf_int16_t )icf_port_strlen(
                            p_glb_pdb->string1.str);

                }
#endif
                /* If API recvd is ICF_REFER_REQ,reject the API with ecode as
                 * ICF_CAUSE_INVALID_REQUEST and send the unexpected message
                 * indication to application as ICF_REFER_REQ from application
                 * is not supported by IPTK*/
                if(ICF_REFER_REQ == p_msg->hdr.api_id)
                {  
                    ecode = ICF_CAUSE_INVALID_REQUEST;
                    ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_glb_pdb,
                            p_glb_pdb->p_msg_lst->p_msg, &ecode);
                    icf_free_decoded_msg(p_msg_lst->p_msg, &ecode);
                    p_msg_lst->p_msg = ICF_NULL;
                    continue;
                }   
                
				icf_dbm_get_module_glb_data(p_glb_pdb,
					(icf_module_id_t)(ICF_INT_MODULE_MMI),
					(icf_void_t *)&p_mmi_data,&ecode);				
                
                if(ICF_FAILURE == icf_mrm_validate_hdr_and_get_dest_module(
                    p_glb_pdb,
                    &p_msg->hdr,
		            &dest_int_module,
                    &ecode))
                {
                    if(ICF_MODULE_MM == p_msg->hdr.source_id)
                      {
                        ret_val = icf_cmn_send_mmi_unexpected_msg_ind(p_glb_pdb,
                                      p_glb_pdb->p_msg_lst->p_msg, &ecode);
                      }
                    else if(ICF_MODULE_PA == p_msg->hdr.source_id)
                      {
                        ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_glb_pdb,
                                      p_glb_pdb->p_msg_lst->p_msg, &ecode);
                      }

                    ICF_ALARM(((icf_uint8_t *)"Header Validation failed. Message will be ignored.\n"));

                    ICF_SET_ECODE(p_glb_pdb,ICF_NULL,
                            ICF_INVALID_SRC_MODULE_ID,&ecode)
                }

                /*
                 * If init complete is not done or the system has beein deinit
                 *then dump all the messages other than those comming from 
                 *external module CONF or external module SM
                 */

#if 0
                else if( ((ICF_FALSE == 
							p_glb_pdb->p_glb_data->global_data_init_complete
                            && ICF_INT_MODULE_CFG != dest_int_module &&
                            ICF_TIMER_MSG != p_msg->hdr.api_id) && 
							!(ICF_NULL != p_mmi_data && 
							ICF_FALSE == p_mmi_data->init_complete &&
							ICF_MODULE_MM == p_msg->hdr.source_id)) )
                {
	                ICF_SET_ECODE(p_glb_pdb,ICF_NULL,
                        ICF_MSG_PROC_INIT_NOT_COMPLETE,&ecode)
                }
#endif
#ifdef ICF_PORT_SYMBIAN
                else if ( ((ICF_FAILURE == 
                    icf_port_is_mem_available(p_glb_pdb)) || 
                    (ICF_FAILURE ==
                    icf_port_is_timer_available(p_glb_pdb)))
                    && ICF_NW_API_MSG_FROM_PEER != api_id
					&& ICF_TIMER_MSG !=api_id
                    && dest_int_module != ICF_INT_MODULE_MMI)
                {
                    icf_msg_st	*p_res_unavail_ind = ICF_NULL;
                    icf_res_unavail_ind_st *p_payload = ICF_NULL;
                    ICF_MEMGET(
                        p_glb_pdb,
                        sizeof(icf_api_header_st) + sizeof(icf_res_unavail_ind_st),
                        ICF_MEM_COMMON,
                        p_res_unavail_ind,
                        ICF_RET_ON_FAILURE,
                        &ecode,
                        ret_val)
                        p_payload = (icf_res_unavail_ind_st*)
                        p_res_unavail_ind->payload;

                    p_res_unavail_ind->hdr.api_id = 
                        ICF_RES_UNAVAIL_IND;
                    p_res_unavail_ind->hdr.api_length =
                        sizeof(icf_api_header_st) + sizeof(icf_res_unavail_ind_st);
                    p_res_unavail_ind->hdr.app_id = 
                        p_msg->hdr.app_id;
                    p_res_unavail_ind->hdr.call_id = 
                        p_msg->hdr.call_id;
                    p_res_unavail_ind->hdr.destination_id = 
                        p_msg->hdr.source_id;
                    p_res_unavail_ind->hdr.source_id = 
                        p_msg->hdr.destination_id;
                    p_res_unavail_ind->hdr.version =
                        p_msg->hdr.version;

                    p_payload->cause = ICF_RES_THRESHOLD_REACHED;
                    p_payload->req_api_id = p_msg->hdr.api_id;
                    p_payload->result = ICF_FAILURE;
                    p_payload->retry_after = 130;
                    icf_free_decoded_msg(p_msg_lst->p_msg, &ecode);
                    p_glb_pdb->p_recv_msg = p_res_unavail_ind;
                    
                    internal_msg.msg_hdr.msg_id = ICF_MSG_FROM_MRM;     
                    internal_msg.msg_hdr.payload_length = 0;
                    internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE;
                    /*Assigning the global pdb pointer received to global 
                    * pdb pointer in the internal message structure*/
                    internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;
                    ret_val = icf_cfg_process_mesg(&internal_msg);
                    ICF_MEMFREE(
                        p_glb_pdb,
                        p_res_unavail_ind, 
                        (icf_pool_id_t const)ICF_MEM_COMMON,
                        &ecode, 
                        ret_val)
					return ICF_SUCCESS;
                }
#endif /* ICF_PORT_SYMBIAN */

                /* If module id is valid*/
               
                /* This check is done for GPRS status and for 
                 * some APIs that expect to send message on network. */
#ifdef ICF_NW_ACTIVATE
                else if (
                    (ICF_FAILURE != icf_cmn_chk_gprs_status(
                        p_glb_pdb, &gprs_status, &ecode)) &&
                        (ICF_FALSE == gprs_status) &&
                    (   ICF_TRUE == icf_cmn_chk_api_for_gprs(p_glb_pdb,
                                p_msg, &ecode))
                            )
                {
                    icf_cmn_handle_gprs_fail(p_glb_pdb,
                            p_msg, &ecode);                    
                }
#endif
                else 
                {
                    p_glb_pdb->p_call_ctx = ICF_NULL;
					if((p_msg->hdr.api_id > ICF_CONTROL_PLANE_API_BASE ) &&
                        (ICF_INVALID_CALL_ID != p_msg->hdr.call_id) &&
                        (ICF_INT_MODULE_CFG != dest_int_module))
                    {
                        if(ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                                    p_glb_pdb,
                                    p_msg->hdr.call_id,
                                    &p_glb_pdb->p_call_ctx,
                                    &ecode))
                        {
                            p_glb_pdb->p_call_ctx = ICF_NULL;
                            switch(p_msg->hdr.api_id)
                                  {
                                     case ICF_CALL_HOLD_REQ:
                                     case ICF_CALL_RESUME_REQ:
                                     case ICF_CALL_TRANSFER_UNATTENDED_REQ:
                                     case ICF_CALL_TRANSFER_ATTENDED_REQ:
                                     case ICF_CONFERENCE_REQ:
                                                              service_call_api = ICF_TRUE;
                                     break;
                                     default:
                                     break;
                                  }
                            /* Added check for create_xconf api because
                               in this case also call_ctx will be null 
                               We don't need to send unexpected msg indication
                             to PA but process it regularly.*/
                            if((ICF_PA_CC_CREATE_CALL_REQ !=
                                        p_msg->hdr.api_id) &&
                               (ICF_CREATE_XCONF_REQ != p_msg->hdr.api_id) &&
                               (ICF_FALSE == service_call_api))     
                              {
                                /*It is rewuired to have sic ctx to check the tunnel mode  */
                                if(ICF_FAILURE == icf_dbm_find_sic_cntxt_blk(
                                    p_glb_pdb,
                                    p_msg->hdr.call_id,
                                   (icf_sic_ctx_st **)(&(p_glb_pdb->p_sic_ctx)),
                                    &ecode))
                                {
                                    /*  Do nothing */
                                }

                                ecode = ICF_CAUSE_INVALID_CALL_ID;
                                if((p_msg->hdr.api_id > ICF_CC_MM_API_START) &&
                                   (p_msg->hdr.api_id <= ICF_CC_MM_API_END))
                                {
                                    /* call_id validity for MM messages is checked in MMI - 
                                        hence do nothing */
                                   }
                                    /*  the following is the check for SM messages */
                                else if((p_msg->hdr.api_id > ICF_SM_API_START) &&
                                   (p_msg->hdr.api_id <= ICF_SM_API_END))
                                {
                                    /* Since SM messages have no Call-ID no validity is
                                    required */
                                }
                                 else
                                   {
                                      if(ICF_INT_MODULE_SIC != dest_int_module)
                                      {
                                          ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_glb_pdb,
                                          p_glb_pdb->p_msg_lst->p_msg, &ecode);
                                          /* SPR 17668: Set the flag to ignore
                                           * the message for further processing 
                                           * after sending unexpected_msg_ind */
                                          ignore_msg_flag = ICF_TRUE;
                                      }    
                                   }
                              }
                        }
                        else if(ICF_CREATE_XCONF_REQ == p_msg->hdr.api_id)
                        {  
                        /* if call ctx is present for the call id coming in create
                           xconf request then send PA unexpected mesg indication 
                           as this is not valid */
                           ecode = ICF_CAUSE_INVALID_CALL_ID;
                           ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_glb_pdb,
                                   p_glb_pdb->p_msg_lst->p_msg, &ecode);
                        }
                        /* SPR 19433 : In case a API has already been rejected with 
                           ICF_UNEXPECTED_MSG_IND, ignore_msg_flag is set to ICF_TRUE.
                           In such a case, do not send the rejection API again, irrespective
                           of mode of operation (ie tunnel or non-tunnel mode)
                        */   

                        if(ICF_FALSE == ignore_msg_flag)
                        {
                            /* In tunnel mode, that is when the application does not process 
                               media locally, we shall not support any FAX or SERVICE APIs.They
                               will be rejected by API ICF_UNEXPECTED_MSG_IND.
                            */
ICF_CHECK_TUNNEL_MODE_START(p_glb_pdb,ICF_TRUE)
                        
                            switch(p_msg->hdr.api_id)
                            {
                                case ICF_START_FAX_REQ:
                                case ICF_STOP_FAX_REQ:
                                case ICF_CALL_HOLD_REQ:
                                case ICF_CALL_RESUME_REQ:
                                case ICF_CONFERENCE_REQ:
                                case ICF_CALL_TRANSFER_ATTENDED_REQ:
                                case ICF_CALL_TRANSFER_UNATTENDED_REQ:
    
                                     ecode = ICF_CAUSE_INVALID_REQUEST;
                                     ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_glb_pdb,
                                     p_glb_pdb->p_msg_lst->p_msg, &ecode);
                                     icf_free_decoded_msg(p_msg_lst->p_msg, &ecode);
                                     p_msg_lst->p_msg = ICF_NULL;
                                     /* No further processing required. Continue. */
                                     continue;
                            }

ICF_CHECK_TUNNEL_MODE_END
                            /* In non tunnel mode, we do not expect APIs ICF_GENERIC_MSG_REQ and 
                               ICF_GENERIC_MSG_CFM.They will be rejected by API ICF_UNEXPECTED_MSG_IND.
                            */
ICF_CHECK_TUNNEL_MODE_START(p_glb_pdb,ICF_FALSE)
                            switch(p_msg->hdr.api_id)
                            {
                                case ICF_GENERIC_MSG_REQ:
                                case ICF_GENERIC_MSG_CFM:
                                     ecode = ICF_CAUSE_INVALID_REQUEST;
                                     ret_val = icf_cmn_send_pa_unexpected_msg_ind(p_glb_pdb,
                                     p_glb_pdb->p_msg_lst->p_msg, &ecode);
                                     icf_free_decoded_msg(p_msg_lst->p_msg, &ecode);
                                     p_msg_lst->p_msg = ICF_NULL;
                                     /* No further processing required. Continue. */
                                     continue;
                            }
ICF_CHECK_TUNNEL_MODE_END
                        } /* end of ignore_msg_flag is ICF_FALSE */
                    }
                    

                    /* 
                     * If it is a not a create call request or the call context 
                     * is null continue with the processing. The intent of this 
                     * check is to ensure any duplicate 
                     * ICF_PA_CC_CREATE_CALL_REQ is DUMPED. Other API would 
                     * would wither be handled by CM like few service API if 
                     * they come and the is no call ongoing for the call id 
                     * given. Rest API if come when there is no call context for
                     * call id would be DUMPED by CC
                     */
                    if((ICF_TRUE == ignore_msg_flag) || 
                        ((ICF_NULL != p_glb_pdb->p_call_ctx) &&
                        (dest_int_module != ICF_INT_MODULE_MMI) &&
                        (ICF_PA_CC_CREATE_CALL_REQ != p_msg->hdr.api_id) &&
                        (p_glb_pdb->p_call_ctx->app_id != p_msg->hdr.app_id )
                        ))
                    {
                        ICF_ALARM(((icf_uint8_t *)"Invalid Call-id for the application. Message will be ignored.\n"));
                        /* SPR17668: Reset the flag for further processing of
                         * other messages, else all messages will be
                         * ignored */
                        ignore_msg_flag = ICF_FALSE;
                    }
                    /* Added the CREATE_XCONF_REQ check in else we do not 
                       want to go further if xconf_request comes again and
                       call ctx is non null */

                    else if(((ICF_PA_CC_CREATE_CALL_REQ != p_msg->hdr.api_id)
                            &&(ICF_CREATE_XCONF_REQ != p_msg->hdr.api_id)) ||
                        (ICF_NULL == p_glb_pdb->p_call_ctx))
                    {
                        p_glb_pdb->p_recv_msg = p_msg;
                                             
                        internal_msg.msg_hdr.msg_id = ICF_MSG_FROM_MRM;     
                        internal_msg.msg_hdr.payload_length = 0;
                        internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE;
                        /*Assigning the global pdb pointer received to global 
                         * pdb pointer in the internal message structure*/
                        internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;

					
						if(ICF_INT_MODULE_SIC == dest_int_module)
						{
							/*Message is destined for SIC module*/
							icf_mrm_map_and_process_external_api_for_sic(&internal_msg);
						}
						else 
						{
							/* Access the function pointer table to call the 
							 * function which would process the message */      
							if( ICF_FAILURE == func_ptr[dest_int_module]
									(&internal_msg)
									&& ICF_MODULE_ICF != source_id )
							{
								if(ICF_INT_MODULE_NM != dest_int_module)
								icf_mrm_handle_process_fail(&internal_msg,
										(icf_uint8_t)dest_int_module);
							}
						}
                    }
                }
               /* Free memory held by list node for the message */             

				if( api_id == ICF_NW_API_MSG_FROM_PEER)
				{
					/*Free memory held by list node for the message */             
					ICF_MEMFREE(
							p_glb_pdb,
							p_msg_lst->p_msg, 
							(icf_pool_id_t const)ICF_MEM_COMMON,
							&ecode, 
							ret_val)
				}
#ifdef FSS_EMUZED_MEDIA_MANAGER
				else if( dest_int_module == ICF_INT_MODULE_MMI)
				{
					if ( ICF_CFG_MAX_MIC_GAIN_REQ == p_msg->hdr.api_id)
					{
						p_msg->hdr.api_id = ICF_CFG_MAX_MIC_GAIN_REQ;
					}
					else if ( ICF_SET_MIC_GAIN_REQ == p_msg->hdr.api_id )
					{
						p_msg->hdr.api_id = ICF_SET_MIC_GAIN_REQ;
					}
					icf_free_decoded_msg(p_msg_lst->p_msg, &ecode);
					/*
					if ( ICF_CFG_MAX_MIC_GAIN_REQ == p_msg->hdr.api_id ||
						 ICF_SET_MIC_GAIN_REQ == p_msg->hdr.api_id)
					{
					}*/
						
					/*In case of media manager, No encoding/decoding
					  is done and hence no memory free is required.
					 */   
				}
#endif
				else
				{
					/*call the client library function to free this message*/
					icf_free_decoded_msg(p_msg_lst->p_msg, &ecode);
				}

                p_msg_lst->p_msg = ICF_NULL;
                if(ICF_NULL != p_msg_lst->p_msg_next)    
                {
                    p_msg_lst->p_msg = (p_msg_lst->p_msg_next)->p_msg;
                    p_temp_msg_blk = p_msg_lst->p_msg_next;
                    p_msg_lst->p_msg_next = p_temp_msg_blk->p_msg_next;

                    ICF_MEMFREE(
                            p_glb_pdb,
                            p_temp_msg_blk, 
                            (icf_pool_id_t const)ICF_MEM_COMMON,
                            &ecode, 
                            ret_val)                  
                }/*End If*/
                /*
                 * Set call context and recv message pointers to NULL at the end
                 * of message processing
                 */
                p_glb_pdb->p_call_ctx = ICF_NULL;
                p_glb_pdb->p_rgm_ctx = ICF_NULL;
                p_glb_pdb->p_sic_ctx = ICF_NULL;
                p_glb_pdb->line_id = ICF_INVALID_LINE_ID;
                p_glb_pdb->p_recv_msg = ICF_NULL;
                
            }while(ICF_NULL != p_msg_lst->p_msg);
#ifndef ICF_PORT_SYMBIAN
        }/*End If*/
        else
        {
            break;
        }
    }/*End While*/
#endif

	ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Exitting function <icf_mrm_process_msg>\n"));
    ICF_FUNCTION_EXIT(p_glb_pdb)

#ifdef ICF_PORT_SYMBIAN
	p_glb_pdb->p_msg_lst = ICF_NULL;
#endif

    ICF_RESET_MODULE(p_glb_pdb)
    return ret_val;
}/*End icf_mrm_process_msg*/


/****************************************************************************
*
* FUNCTION NAME:  icf_mrm_timer_cb_fn
*
* DESCRIPTION :   This function reports the genlib timer expiry to a module.
*
* RETURNS  : Nothing
*
*****************************************************************************/

#ifdef ICF_PORT_SYMBIAN
icf_void_t
icf_mrm_timer_cb_fn(
        INOUT icf_void_t *p_buf)
{
    icf_timer_data_st    *p_timer_data = ICF_NULL;
    icf_msg_st           *p_msg = ICF_NULL;  
    icf_msg_list_st      temp_msg_lst;
    icf_error_t          ecode;
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Entering function <icf_mrm_timer_cb_fn>\n"));
   
    if(ICF_NULL == p_buff)
        return;

	ICF_FUNCTION_ENTER(((icf_timer_data_st *)p_buf)->p_glb_pdb)
        
    p_timer_data=( icf_timer_data_st *)p_buf;
    
    p_glb_pdb = p_timer_data->p_glb_pdb;

    p_msg = (icf_msg_st *)icf_cmn_memget(
            p_glb_pdb,
            (icf_uint32_t const)(sizeof(icf_api_header_st) 
             + sizeof(icf_timer_data_st *)),
            (icf_pool_id_t const)ICF_MEM_COMMON, 
            &ecode); 
    if (ICF_NULL == p_msg) 
    { 
       ICF_SET_ECODE(p_glb_pdb, 
               ICF_NULL, 
               ICF_ERROR_MEM_ALLOC_FAILURE,               
               &ecode)  
    }
    else       
    /* if memory is allocated successfully process the block*/
    {
        p_msg->hdr.api_id = ICF_TIMER_MSG;
        p_msg->hdr.destination_id = p_timer_data->module_id;
        p_msg->hdr.source_id = ICF_MODULE_ICF;
        p_msg->hdr.call_id   = ICF_INVALID_CALL_ID;
        p_msg->hdr.api_length = sizeof(icf_timer_data_st *);
        /*copy the timer_data in payload field of msg */
        icf_port_memcpy(&p_msg->payload,&p_timer_data,
                (icf_uint32_t const)sizeof(icf_timer_data_st *));     

		temp_msg_lst.p_msg = p_msg;
		temp_msg_lst.p_msg_next= ICF_NULL;

		icf_process_message(p_glb_pdb, &temp_msg_lst);
        
    }/*End Else*/
  
	ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Exitting function <icf_mrm_timer_cb_fn>\n"));
    ICF_FUNCTION_EXIT(p_glb_pdb)
}
#else
icf_void_t
icf_mrm_timer_cb_fn(
        INOUT icf_void_t *p_buf)
{
    icf_timer_data_st    *p_timer_data = ICF_NULL;
    icf_msg_st           *p_msg = ICF_NULL;  
    icf_msg_list_st      *p_temp_msg_lst = ICF_NULL;
    icf_error_t          ecode = ICF_ERROR_NONE;
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

    /* SPR 18880 : Added NULL checks **/
    if((ICF_NULL == p_buf) ||
       ((ICF_NULL == ((icf_timer_data_st *)p_buf)->p_glb_pdb)))
    {
        /* Return failure. Not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[MRM]: p_glb_pdb is NULL.Exitting function <icf_mrm_timer_cb_fn>\n"))
        return;
    }

    ICF_FUNCTION_ENTER(((icf_timer_data_st *)p_buf)->p_glb_pdb)
        
    p_timer_data=( icf_timer_data_st *)p_buf;
    
    p_glb_pdb = p_timer_data->p_glb_pdb;

    p_msg = (icf_msg_st *)icf_cmn_memget(
            p_glb_pdb,
            (icf_uint32_t const)(sizeof(icf_api_header_st) 
             + sizeof(icf_timer_data_st *)),
            (icf_pool_id_t const)ICF_MEM_COMMON, 
            &ecode); 
    if (ICF_NULL == p_msg) 
    { 
       ICF_SET_ECODE(p_glb_pdb, 
               ICF_NULL, 
               ICF_ERROR_MEM_ALLOC_FAILURE,               
               &ecode)  
    }
    else       
    /* if memory is allocated successfully process the block*/
    {
        p_msg->hdr.api_id = ICF_TIMER_MSG;
        p_msg->hdr.destination_id = p_timer_data->module_id;
        p_msg->hdr.source_id = ICF_MODULE_ICF;
        p_msg->hdr.call_id   = ICF_INVALID_CALL_ID;
        p_msg->hdr.api_length = sizeof(icf_timer_data_st *);
        /*copy the timer_data in payload field of msg */
        icf_port_memcpy(&p_msg->payload,&p_timer_data,
                (icf_uint32_t const)sizeof(icf_timer_data_st *));     
        
        p_temp_msg_lst = p_glb_pdb->p_msg_lst;

        /*  Traverse the link list till last node */
        while ((ICF_NULL != p_temp_msg_lst) &&
               (ICF_NULL != p_temp_msg_lst->p_msg_next))
        {        
            p_temp_msg_lst = p_temp_msg_lst->p_msg_next;
        }
        /*If already there are messages in the list*/
        if( p_temp_msg_lst && (ICF_NULL != p_temp_msg_lst->p_msg))
        {
            /*Create a new node*/
            p_temp_msg_lst->p_msg_next = (icf_msg_list_st *)
                icf_cmn_memget(
                    p_glb_pdb,
                    (icf_uint32_t const)sizeof(icf_msg_list_st),
                    (icf_pool_id_t const)ICF_MEM_COMMON, &ecode); 

            /*If there's a memory allocation failure*/
            if (ICF_NULL == p_temp_msg_lst->p_msg_next) 
            { 
                ICF_SET_ECODE(p_glb_pdb, 
                        ICF_NULL, 
                        ICF_ERROR_MEM_ALLOC_FAILURE,               
                        &ecode)  

                    if (ICF_FAILURE ==
                            icf_cmn_memfree(p_glb_pdb,
                                p_msg,
                                (icf_pool_id_t const)ICF_MEM_COMMON, 
                                &ecode))
                    {
                        /*If call to MEMFREE returns failure*/ 
                        ICF_SET_ECODE(
                                p_glb_pdb, 
                                ICF_NULL, 
                                ICF_ERROR_MEM_FREE_FAILURE,  
                                &ecode) 
                    } 
                    else
                    {
                        p_msg = ICF_NULL; 
                    }
            }/*End If*/
            else
            {
                p_temp_msg_lst->p_msg_next->p_msg = p_msg;
                p_temp_msg_lst->p_msg_next->p_msg_next = ICF_NULL;
            }
        }/*The message is the first message in the list*/
        else
        {
            if (p_temp_msg_lst) p_temp_msg_lst->p_msg = p_msg;
        }
    }/*End Else*/
#ifdef ICF_STAT_ENABLE
    /* SPR 18880 : Added NULL check */
    if(ICF_NULL != p_glb_pdb->p_stats_info)
    {
        (p_glb_pdb->p_stats_info->misc_stats.stats_active_timers)--;
    }
#endif
    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_glb_pdb)
}
#endif
/****************************************************************************
 *
 * FUNCTION NAME:  icf_mrm_process_ipsstk_msg
 *
* DESCRIPTION :   This is utility function to route messages from ICF.
*
* RETURNS  : Nothing
* 
*****************************************************************************/


icf_return_t 
icf_mrm_icf_process_msg(
        INOUT icf_internal_msg_st *p_internal_msg)
{
    icf_timer_data_st *p_timer_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    

    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)
        
    p_timer_data = *((icf_timer_data_st **)
                    p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->payload);

    /*Route the message based on the module id of of timer expiry raising module
     * */
    switch (p_timer_data->module_id)
    {  
        case ICF_INT_MODULE_CFG:
			ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Calling icf_cfg_process_mesg\n"));
            icf_cfg_process_mesg(p_internal_msg);
            break;
#if defined(ICF_STAT_ENABLED) || defined (ICF_TRACE_ENABLE) ||  defined(ICF_ERROR_ENABLE)
        case ICF_INT_MODULE_SM:
            icf_sm_process_mesg(p_internal_msg);
            break;
#endif
        case ICF_INT_MODULE_MMI:
			ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Calling icf_mmi_process_mesg\n"));
            if(ICF_FAILURE ==
                      icf_mmi_process_mesg(p_internal_msg))
            {
                icf_mrm_handle_process_fail(p_internal_msg,
                        (icf_uint8_t)ICF_INT_MODULE_MMI);
            }
            break;

        case ICF_INT_MODULE_SCM:
        case ICF_INT_MODULE_ADM:
        case ICF_INT_MODULE_CHR:
        case ICF_INT_MODULE_RNW:
        case ICF_INT_MODULE_TWC:
        case ICF_INT_MODULE_CTA:
        case ICF_INT_MODULE_CTU:
        case ICF_INT_MODULE_CFW:
        case ICF_INT_MODULE_CC:
        case ICF_INT_MODULE_CM:
			ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Calling icf_cm_process_mesg\n"));
           if(ICF_FAILURE == 
            icf_cm_process_mesg(p_internal_msg))
           {
               icf_mrm_handle_process_fail(p_internal_msg,
                        (icf_uint8_t)ICF_INT_MODULE_CM);
           }
           break;

		case ICF_INT_MODULE_SIC:
            icf_sic_process_mesg(p_internal_msg);
			break;

        case ICF_INT_MODULE_SSA:
        case ICF_INT_MODULE_SIP_STACK:
        case ICF_INT_MODULE_SIP_UA:
            if(ICF_FAILURE ==
                    icf_ssa_process_mesg(p_internal_msg))
            {
                icf_mrm_handle_process_fail(p_internal_msg,
                        (icf_uint8_t)ICF_INT_MODULE_SSA);
            }
            break;
#ifdef ICF_NAT_MEDIA_TRAVERSAL
        case ICF_MODULE_NAT:
            {
                icf_msg_st                   *p_msg = ICF_NULL;
                icf_uint16_t			api_len = 0;
                nat_timer_expiry_ind_st		*p_payload = ICF_NULL;
                icf_error_t                 ecode;

                ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Calling NAT timer expiry message \n"));
        
                api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
                                ICF_PORT_SIZEOF(nat_timer_expiry_ind_st);
    
	            /* Allocate memory to the response message strucure */
                ICF_MEMGET(
                        p_internal_msg->msg_hdr.p_glb_pdb,
                        api_len,
                        ICF_MEM_COMMON,
                        p_msg,
                        ICF_RET_ON_FAILURE,
                        &ecode,
                        ret_val)
	            
                if(ICF_NULL != p_msg)
                {        
                    /* Fill the header of the message to be send */
                    p_msg->hdr.api_id = ICF_NAT_TIMER_EXP_IND;
                    p_msg->hdr.source_id = ICF_MODULE_ICF;
                    p_msg->hdr.destination_id = ICF_MODULE_NAT;
    
	                p_msg->hdr.api_length = api_len;
                    p_msg->hdr.call_id = ICF_INVALID_CALL_ID;    
	                p_msg->hdr.version = ICF_VERSION_1_0;
	                p_msg->hdr.app_id = ICF_GENERIC_APP_ID;

	                p_payload = (nat_timer_expiry_ind_st *)p_msg->payload;

                    icf_port_memcpy(&p_payload->timer_data,
				p_timer_data->p_timer_buffer,
                        	ICF_PORT_SIZEOF(nat_timer_data_st));

					p_payload->timer_id = p_timer_data->timer_id;

                    ICF_SEND(p_internal_msg->msg_hdr.p_glb_pdb,p_msg,ret_val, 
                            &ecode)

	                ICF_MEMFREE(p_internal_msg->msg_hdr.p_glb_pdb,p_msg,
                            ICF_MEM_COMMON,&ecode,ret_val)
                    /* Free the timer data in case of NAT, NAT is not
                     * able to free this memory.*/
                    icf_dbm_ret_mem_to_pool(p_internal_msg->msg_hdr.p_glb_pdb, ICF_MEM_TIMER,
                            p_timer_data, &ecode);
	            }

            
            }
             break;
#endif

        default:
            ICF_SET_ECODE(p_internal_msg->msg_hdr.p_glb_pdb,ICF_NULL,
                         ICF_INVALID_SRC_MODULE_ID,
                         &p_internal_msg->msg_hdr.ecode)
            break;
    }/* End Switck*/
    ICF_FUNCTION_EXIT(p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
}

/****************************************************************************
 *
 * FUNCTION NAME:  icf_mrm_handle_process_fail
 *
 * DESCRIPTION :   This is utility function to send appropriate message to 
 *                modules in case the message was not successfully processed.
 *
 * RETURNS  : Nothing
 * 
 *****************************************************************************/
icf_void_t 
icf_mrm_handle_process_fail(
        icf_internal_msg_st *p_internal_msg,
        icf_uint8_t failure_ret_module)
{
    switch(failure_ret_module)
    {

        case ICF_INT_MODULE_MMI:
        case ICF_INT_MODULE_CM:
        case ICF_INT_MODULE_SSA:

            p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND; 
            p_internal_msg->msg_hdr.payload_length = 0;
            icf_mmi_process_mesg(p_internal_msg);   
            
            p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND; 
            p_internal_msg->msg_hdr.payload_length = 0;
            icf_cm_process_mesg(p_internal_msg);

            p_internal_msg->msg_hdr.msg_id = 
                ICF_FORCEFUL_CALL_CLEAR_IND; 
            p_internal_msg->msg_hdr.payload_length = 0;
            icf_ssa_process_mesg(p_internal_msg);

            /*Currently there is no handling for process message failure from 
             * module CFG and SM*/

        default:
            break;
    }

}


icf_void_t 
icf_mrm_map_and_process_external_api_for_sic(
        icf_internal_msg_st *p_internal_msg)
{
	icf_msg_st		             *p_msg=ICF_NULL;

	icf_sic_publish_req_st		 pub_req;
	icf_publish_req_st			 *p_pub_req=ICF_NULL;

	icf_sic_subs_req_st			 subs_req = {0,{0,"\0"},0,0,0,ICF_NULL,{{0,"\0"},0},
                                      0,{{0,"\0"},0},{0,0},{0,0},{0,{0,{0}},0},0};
	icf_subscribe_req_st		 *p_sub_req=ICF_NULL;

	icf_sic_message_req_st		 msg_req;
	icf_message_req_st			 *p_msg_req=ICF_NULL;

	icf_sic_refer_req_st		 refer_req;
	icf_refer_req_st			 *p_refer_req=ICF_NULL;

	icf_sic_notify_cfm_st		 notify_cfm = {0,0,0,0,{0,0},{0,0},{0,{0,{0}},0}};
	icf_notify_cfm_st			 *p_notify_cfm = ICF_NULL;
	icf_sic_options_req_st		 options_req;
	icf_options_req_st			 *p_options_req=ICF_NULL;

	icf_sic_message_cfm_st		message_cfm=
                        {0,0,0,0,{0,0},{0,0}};
	icf_message_cfm_st	*p_message_cfm = ICF_NULL;
	/* ICF_6_1_OPTIONS_SUPPORT start */
	icf_options_cfm_st	         *p_options_cfm = ICF_NULL;
	icf_sic_options_cfm_st	     options_cfm = 
                        {0,0,0,0,0,{0,0},{0,0},{0,{0,{0}},0},0,0,{{0,0}},0};
    /* Rel 8.1
     * variables required to convey the ICF_EARLY_SESSION_CFM
     * data to SIC
     */
    icf_early_session_cfm_st     *p_early_session_cfm = ICF_NULL;
    icf_sic_early_session_cfm_st early_session_cfm = 
                                         {0,0,0,0,0,0};
	/* ICF_6_1_OPTIONS_SUPPORT end */	

    icf_header_list_st		     *p_header_list = ICF_NULL;
    icf_tag_list_st			     *p_tag_list = ICF_NULL;
	icf_msg_body_list_st		 *p_body_list = ICF_NULL;
	icf_list_st			         *p_app_stream_list = ICF_NULL;

    /*Fix for SPR 20412: CSR 1-8223807 merged*/
	/*Fix for CSR 1-8223807 : SPR-20362*/
    icf_header_name_list_st  *p_remove_hdr_list = ICF_NULL;	
		
	p_msg = (icf_msg_st*) p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg;

	switch( p_msg->hdr.api_id)
	{
		case ICF_SUBSCRIBE_REQ :
			{
				p_internal_msg->msg_hdr.msg_id = ICF_SIC_SUBSCRIBE_REQ;
				p_internal_msg->p_msg_data = (icf_uint8_t*)&subs_req;

				p_sub_req = (icf_subscribe_req_st*)
					(p_msg->payload);
				
				subs_req.call_id = p_msg->hdr.call_id;
				subs_req.app_id = p_msg->hdr.app_id;
                                subs_req.bitmask = p_sub_req->bit_mask;

				if(p_sub_req->bit_mask & ICF_SUB_REQ_DEST_INFO_PRESENT)
				{
					icf_port_memcpy(
						&(subs_req.destination_info),
						&(p_sub_req->destination_info),
						sizeof(icf_address_st) );
				}
				else
				{
					subs_req.destination_info.addr_type = ICF_ADDRESS_PLAN_UNDEFINED;
					subs_req.destination_info.addr_val.str_len = 0;
					subs_req.destination_info.addr_val.str[0] = 0;
				}
					
				
				subs_req.duration = p_sub_req->duration;

				if(p_sub_req->bit_mask & ICF_SUB_REQ_EVENT_PRESENT)
				{
					/*
					if( icf_port_strcmp( p_sub_req->event_pkg_name.str,"conference") == 0)
						subs_req.event_package = ICF_EVENT_PKG_CONFERENCE;

					if( icf_port_strcmp( p_sub_req->event_pkg_name.str,"message-summary") == 0)
						subs_req.event_package = ICF_EVENT_PKG_MESSAGE_SUMMARY;

					if( icf_port_strcmp( p_sub_req->event_pkg_name.str,"reg") == 0)
						subs_req.event_package = ICF_EVENT_PKG_REG;
						*/

					icf_port_strcpy( subs_req.event_package.str, 
						p_sub_req->event_pkg_name.str);
				}
				else
					subs_req.event_package.str[0] = ICF_NULL;


				subs_req.line_id = p_sub_req->line_id;;
				subs_req.p_key= ICF_NULL;

				if(p_sub_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
				{
					icf_port_memcpy(
						&(subs_req.pref_identity),
						&(p_sub_req->pref_identity),
						sizeof(icf_address_st) );
				}
				else
				{
					subs_req.pref_identity.addr_type =ICF_ADDRESS_PLAN_UNDEFINED;
				}

                /*Fix for SPR: 19999 (Merged CSR 1-7467002 : SPR 19863)*/
                if(p_sub_req->bit_mask & 
                         ICF_SUB_REQ_SUBS_HEAD_START_DURATION_PRESENT)
                {
                    subs_req.subs_head_start_duration = 
                               p_sub_req->subs_head_start_duration;
                }				

				if( p_sub_req->header_list.count )
				{
					icf_cmn_clone_header_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_sub_req->header_list),
						&(subs_req.header_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_header_list = &(subs_req.header_list);
				}
				else
					subs_req.header_list.count=0;

				if( p_sub_req->tag_list.count )
				{
					icf_cmn_clone_tag_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_sub_req->tag_list),
						&(subs_req.tag_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_tag_list = &(subs_req.tag_list);
				}
				else
					subs_req.tag_list.count=0;

				if( p_sub_req->body_list.count )
				{
					icf_cmn_clone_body_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_sub_req->body_list),
						&(subs_req.body_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_body_list = &(subs_req.body_list);
				}
				else
					subs_req.body_list.count=0;
				
       
				break;
			}
		case ICF_PUBLISH_REQ :
			{
				p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_REQ;
				p_internal_msg->p_msg_data = (icf_uint8_t*)&pub_req;

				p_pub_req = (icf_publish_req_st*)
					(p_msg->payload);
				
				pub_req.call_id = p_msg->hdr.call_id;
				pub_req.app_id = p_msg->hdr.app_id;
				icf_port_memcpy(
					&(pub_req.destination_info),
					&(p_pub_req->destination_info),
					sizeof(icf_address_st) );
					;
				pub_req.duration = p_pub_req->duration;

			/*	if( icf_port_strcmp( p_pub_req->event_pkg_name.str,"poc-settings") == 0)*/
			/*		pub_req.event_package = ICF_EVENT_PKG_POC_SETTINGS;*/

				icf_port_strcpy( pub_req.event_package.str, 
					p_pub_req->event_pkg_name.str);
			
				pub_req.event_package.str_len = 
					p_pub_req->event_pkg_name.str_len;

				pub_req.line_id = p_pub_req->line_id;;
				pub_req.p_key= ICF_NULL;
                /* code change for CSR_1-4816176/SPR 16075 - not tested */ 
                pub_req.bit_mask = p_pub_req->bit_mask;
                /* end of code change for CSR_1-4816176/SPR 16075 - not tested */ 
                
                if(p_pub_req->bit_mask & ICF_USER_ADDRESS_PRESENT)
				{
					icf_port_memcpy(
						&(pub_req.user_address),
						&(p_pub_req->user_address),
						sizeof(icf_address_st) );
				}
				else
				{
					pub_req.user_address.addr_type =ICF_ADDRESS_PLAN_UNDEFINED;
				}

				if(p_pub_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
				{
					icf_port_memcpy(
						&(pub_req.pref_identity),
						&(p_pub_req->pref_identity),
						sizeof(icf_address_st) );
				}
				else
				{
					pub_req.pref_identity.addr_type =ICF_ADDRESS_PLAN_UNDEFINED;
				}
				

				if( p_pub_req->header_list.count )
				{
					icf_cmn_clone_header_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_pub_req->header_list),
						&(pub_req.header_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_header_list = &(pub_req.header_list);
				}
				else
					pub_req.header_list.count=0;

				if( p_pub_req->tag_list.count )
				{
					icf_cmn_clone_tag_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_pub_req->tag_list),
						&(pub_req.tag_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_tag_list = &(pub_req.tag_list);
				}
				else
					pub_req.tag_list.count=0;

				if( p_pub_req->body_list.count )
				{
					icf_cmn_clone_body_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_pub_req->body_list),
						&(pub_req.body_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_body_list = &(pub_req.body_list);
				}
				else
					pub_req.body_list.count=0;

				break;
			}
		case ICF_MESSAGE_REQ :
			{
				p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_REQ;
				p_internal_msg->p_msg_data = (icf_uint8_t*)&msg_req;

				p_msg_req = (icf_message_req_st*)
					(p_msg->payload);
				
				msg_req.call_id = p_msg->hdr.call_id;
				msg_req.app_id = p_msg->hdr.app_id;
				icf_port_memcpy(
					&(msg_req.destination_info),
					&(p_msg_req->destination_info),
					sizeof(icf_address_st) );
					;
				
				msg_req.line_id = p_msg_req->line_id;;
				msg_req.p_key= ICF_NULL;
                /* code change for CSR_1-4816176/SPR 16075 - not tested */ 
                msg_req.bit_mask = p_msg_req->bit_mask;
                /* end of code change for CSR_1-4816176/SPR 16075 - not tested */

                if(p_msg_req->bit_mask & ICF_USER_ADDRESS_PRESENT)
				{

					icf_port_memcpy(
						&(msg_req.user_address),
						&(p_msg_req->user_address),
						sizeof(icf_address_st) );
				}
				else
				{
					msg_req.user_address.addr_type =ICF_ADDRESS_PLAN_UNDEFINED;
				}
                
				if(p_msg_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
				{

					icf_port_memcpy(
						&(msg_req.pref_identity),
						&(p_msg_req->pref_identity),
						sizeof(icf_address_st) );
				}
				else
				{
					msg_req.pref_identity.addr_type =ICF_ADDRESS_PLAN_UNDEFINED;
				}
				

				if( p_msg_req->header_list.count )
				{
					icf_cmn_clone_header_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_msg_req->header_list),
						&(msg_req.header_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_header_list = &(msg_req.header_list);
				}
				else
					msg_req.header_list.count=0;

				if( p_msg_req->tag_list.count )
				{
					icf_cmn_clone_tag_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_msg_req->tag_list),
						&(msg_req.tag_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_tag_list = &(msg_req.tag_list);
				}
				else
					msg_req.tag_list.count=0;

				if( p_msg_req->body_list.count )
				{
					icf_cmn_clone_body_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_msg_req->body_list),
						&(msg_req.body_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_body_list = &(msg_req.body_list);
				}
				else
					msg_req.body_list.count=0;
				break;
			}
                case ICF_OPTIONS_REQ:
                       {
                           p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_REQ;
                           p_internal_msg->p_msg_data = (icf_uint8_t*)&options_req;

                           p_options_req = (icf_options_req_st*)
                                            (p_msg->payload);
				
                           options_req.call_id = p_msg->hdr.call_id;
                           options_req.app_id = p_msg->hdr.app_id;
                           icf_port_memcpy(
                             &(options_req.destination_info),
                             &(p_options_req->destination_info),
                             sizeof(icf_address_st) );
									
                           options_req.line_id = p_options_req->line_id;
                           options_req.p_key= ICF_NULL;
                           options_req.bit_mask = p_options_req->bit_mask;
                           if(p_options_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
                           {
                               icf_port_memcpy(
                                      &(options_req.pref_identity),
                                      &(p_options_req->pref_identity),
                                      sizeof(icf_address_st) );
                           }
                           else
                           {
                               options_req.pref_identity.addr_type =ICF_ADDRESS_PLAN_UNDEFINED;
                           }
				

                           if( p_options_req->header_list.count )
                           {
                               icf_cmn_clone_header_list(
                                 p_internal_msg->msg_hdr.p_glb_pdb,
                                 &(p_options_req->header_list),
                                 &(options_req.header_list),
                                 &(p_internal_msg->msg_hdr.ecode) );
                               p_header_list = &(options_req.header_list);
                           }
                           else
                               options_req.header_list.count=0;

                           if( p_options_req->tag_list.count )
                           {
                               icf_cmn_clone_tag_list(
                                   p_internal_msg->msg_hdr.p_glb_pdb,
                                   &(p_options_req->tag_list),
                                   &(options_req.tag_list),
                                   &(p_internal_msg->msg_hdr.ecode) );
                               p_tag_list = &(options_req.tag_list);
                           }
                           else
                               options_req.tag_list.count=0;

                           if( p_options_req->body_list.count )
                           {
                               icf_cmn_clone_body_list(
                                   p_internal_msg->msg_hdr.p_glb_pdb,
                                   &(p_options_req->body_list),
                                   &(options_req.body_list),
                                   &(p_internal_msg->msg_hdr.ecode) );
                                   p_body_list = &(options_req.body_list);
                           }
                           else
                               options_req.body_list.count=0;
                           /* Rel 8.1
                            * get the tunnel mode from ICF_OPTIONS_REQ
                            * and fill it in internal message
                            * to communicate it to SIC
                            */
                           if(ICF_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT ==
                              (ICF_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT & 
                              p_options_req->bit_mask)) 
                           {
                               options_req.bit_mask |= 
                               ICF_SIC_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT;
                               options_req.tunnel_mode = p_options_req->tunnel_mode;
                               
                           }
                           /* Fix for SPR 20412: CSR 1-8223807 merged*/
                           /*Fix for CSR 1-8223807 : SPR-20362*/
                           if( p_options_req->remove_hdr_list.count )
				           {
					         icf_cmn_clone_header_name_list(
						               p_internal_msg->msg_hdr.p_glb_pdb,
						               &(p_options_req->remove_hdr_list),
						               &(options_req.remove_hdr_list),
						               &(p_internal_msg->msg_hdr.ecode) );

					         p_remove_hdr_list = &(options_req.remove_hdr_list);
				           }
				           else
                           {
					          options_req.remove_hdr_list.count=0;
                           }

                           break;
                       }
	
		case ICF_REFER_REQ :
			{
				p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_REQ;
				p_internal_msg->p_msg_data = (icf_uint8_t*)&refer_req;

				p_refer_req = (icf_refer_req_st*)
					(p_msg->payload);
				
				refer_req.call_id = p_msg->hdr.call_id;
				refer_req.app_id = p_msg->hdr.app_id;

				if(p_refer_req->bit_mask & ICF_REF_REQ_DEST_INFO_PRESENT)
				{
					icf_port_memcpy(
						&(refer_req.destination_info),
						&(p_refer_req->destination_info),
						sizeof(icf_address_st) );
				}
				else
				{
					refer_req.destination_info.addr_type = ICF_ADDRESS_PLAN_UNDEFINED;
					refer_req.destination_info.addr_val.str_len = 0;
					refer_req.destination_info.addr_val.str[0] = 0;
				}

							
				
				refer_req.line_id = p_refer_req->line_id;;
				refer_req.p_key= ICF_NULL;

				
				icf_port_memcpy(
					&(refer_req.refer_to),
					&(p_refer_req->refer_to),
					sizeof(icf_address_st) );
				
				if(p_refer_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
				{
					icf_port_memcpy(
						&(refer_req.pref_identity),
						&(p_refer_req->pref_identity),
						sizeof(icf_address_st) );
				}
				else
				{
					refer_req.pref_identity.addr_type =ICF_ADDRESS_PLAN_UNDEFINED;
				}

				if(p_refer_req->bit_mask & ICF_REF_REQ_SUBS_REQD_PRESENT)
				{
					refer_req.subs_reqd = p_refer_req->subscription_required;
				}
				else
					refer_req.subs_reqd = 0;

				if( p_refer_req->header_list.count )
				{
					icf_cmn_clone_header_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_refer_req->header_list),
						&(refer_req.header_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_header_list = &(refer_req.header_list);
				}
				else
					refer_req.header_list.count=0;

				if( p_refer_req->tag_list.count )
				{
					icf_cmn_clone_tag_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_refer_req->tag_list),
						&(refer_req.tag_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_tag_list = &(refer_req.tag_list);
				}
				else
					refer_req.tag_list.count=0;

				if( p_refer_req->body_list.count )
				{
					icf_cmn_clone_body_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_refer_req->body_list),
						&(refer_req.body_list),
						&(p_internal_msg->msg_hdr.ecode) );
					p_body_list = &(refer_req.body_list);
				}
				else
					refer_req.body_list.count=0;
				
				break;
			}
/*NOTIFY_OUT_OF_DIALOG_START*/
		case ICF_NOTIFY_CFM:
			{
				p_internal_msg->msg_hdr.msg_id = ICF_SIC_NOTIFY_CFM;
				p_internal_msg->p_msg_data = (icf_uint8_t*)&notify_cfm;

				p_notify_cfm = (icf_notify_cfm_st*)
					(p_msg->payload);

				notify_cfm.call_id = p_msg->hdr.call_id;
				notify_cfm.result = p_notify_cfm->result;

				if(p_notify_cfm->bit_mask & ICF_NOTIFY_ERROR_CAUSE_PRESENT)
				{
					notify_cfm.error_code = p_notify_cfm->error_code;
				}
				else
				{
					notify_cfm.error_code = 0;
				}

				if(p_notify_cfm->bit_mask & ICF_NOTIFY_RESP_CODE_PRESENT)
				{
					notify_cfm.response_code = p_notify_cfm->response_code;
				}
				else
				{
					notify_cfm.response_code = 0;
				}

                /* Asynchronous NOTIFY support */
                /* Copy header,tag and body list */
                if(p_notify_cfm->header_list.count)
				{
					icf_cmn_clone_header_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_notify_cfm->header_list),
						&(notify_cfm.header_list),
						&(p_internal_msg->msg_hdr.ecode));
					p_header_list = &(notify_cfm.header_list);
				}
				else
                {
					notify_cfm.header_list.count = 0;
                }

				if(p_notify_cfm->tag_list.count)
				{
					icf_cmn_clone_tag_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_notify_cfm->tag_list),
						&(notify_cfm.tag_list),
						&(p_internal_msg->msg_hdr.ecode));
					p_tag_list = &(notify_cfm.tag_list);
				}
				else
                {
					notify_cfm.tag_list.count = 0;
                }

				if(p_notify_cfm->body_list.count)
				{
					icf_cmn_clone_body_list(
						p_internal_msg->msg_hdr.p_glb_pdb,
						&(p_notify_cfm->body_list),
						&(notify_cfm.body_list),
						&(p_internal_msg->msg_hdr.ecode));
					p_body_list = &(notify_cfm.body_list);
				}
				else
                {
					notify_cfm.body_list.count = 0;
                }

				break;
			}
/*NOTIFY_OUT_OF_DIALOG_END*/
		/* ICF_6_1_OPTIONS_SUPPORT start */
		case ICF_OPTIONS_CFM:
		{
			icf_uint32_t iterator = 0;
			p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTIONS_CFM;
			p_internal_msg->p_msg_data = (icf_uint8_t*)&options_cfm;

			p_options_cfm = (icf_options_cfm_st*)
				(p_msg->payload);
			
			options_cfm.call_id = p_msg->hdr.call_id;
			options_cfm.app_id = p_msg->hdr.app_id;
			options_cfm.result = p_options_cfm->result;
			/*options_cfm.app_id = p_options_cfm->app_id;*/

			options_cfm.header_list.hdr = ICF_NULL;
			options_cfm.header_list.count=0;
			options_cfm.tag_list.tag = ICF_NULL;
			options_cfm.tag_list.count=0;
			options_cfm.body_list.body = ICF_NULL;
			options_cfm.body_list.count=0;
			options_cfm.p_app_sdp_list = ICF_NULL;
			options_cfm.count = 0;
			
			if( p_options_cfm->header_list.count )
			{
				icf_cmn_clone_header_list(
					p_internal_msg->msg_hdr.p_glb_pdb,
					&(p_options_cfm->header_list),
					&(options_cfm.header_list),
					&(p_internal_msg->msg_hdr.ecode) );
				p_header_list = &(options_cfm.header_list);
			}
			if( p_options_cfm->tag_list.count )
			{
				icf_cmn_clone_tag_list(
					p_internal_msg->msg_hdr.p_glb_pdb,
					&(p_options_cfm->tag_list),
					&(options_cfm.tag_list),
					&(p_internal_msg->msg_hdr.ecode) );
				p_tag_list = &(options_cfm.tag_list);
			}
			if( p_options_cfm->body_list.count )
			{
				icf_cmn_clone_body_list(
					p_internal_msg->msg_hdr.p_glb_pdb,
					&(p_options_cfm->body_list),
					&(options_cfm.body_list),
					&(p_internal_msg->msg_hdr.ecode) );
				p_body_list = &(options_cfm.body_list);
			}
			if ( ICF_NULL != (p_options_cfm->bit_mask & 
				ICF_OPTIONS_CFM_MEDIA_PRESENT))
			{
				icf_cmn_clone_stream_capab_list(
					p_internal_msg->msg_hdr.p_glb_pdb,
					&(options_cfm.p_app_sdp_list),
					p_options_cfm->p_app_sdp_list);
				p_app_stream_list = options_cfm.p_app_sdp_list;
			}
			if ( ICF_NULL != (p_options_cfm->bit_mask & 
				ICF_OPTIONS_CFM_DEL_MEDIA_PRESENT))
			{
				options_cfm.count = p_options_cfm->count;
				for ( iterator = 0; (iterator < options_cfm.count) && (iterator < ICF_MAX_STREAM_PER_CALL); iterator++)
				{
					options_cfm.deleted_media_streams[iterator].stream_id = 
						p_options_cfm->deleted_media_streams[iterator].stream_id;
					options_cfm.deleted_media_streams[iterator].stream_type = 
						p_options_cfm->deleted_media_streams[iterator].stream_type;
				}
			}
            if(ICF_OPTIONS_CFM_FAILURE_RESP_PRESENT & p_options_cfm->bit_mask)
            {
                options_cfm.response_code = p_options_cfm->response_code;
                options_cfm.bitmask |= ICF_SIC_OPTIONS_FAILURE_RESP_PRESENT;
            }
			break;
		}
                /* Rel 8.1
                 * Early_session_cfm handling for
                 * communicating the data through  
                 * internal message to SIC
                 */
                case ICF_EARLY_SESSION_CFM:
                {
                    /* Rel 8.1
                     * make the internal message to send to SIC 
                     */
                    p_internal_msg->msg_hdr.msg_id = ICF_SIC_EARLY_SESSION_CFM;
                    p_internal_msg->p_msg_data = (icf_uint8_t*)&early_session_cfm;

                    p_early_session_cfm = (icf_early_session_cfm_st*)
                                (p_msg->payload);

                    early_session_cfm.call_id = p_msg->hdr.call_id;
                    early_session_cfm.result = p_early_session_cfm->result;
                    early_session_cfm.line_id = p_early_session_cfm->line_id;
                    if (ICF_NULL != (p_early_session_cfm->bit_mask &
                                     ICF_EARLY_SESSION_CFM_TUNNEL_MODE_PRESENT))
                    {
                         early_session_cfm.tunnel_mode =  
                               p_early_session_cfm->tunnel_mode;
                         early_session_cfm.bit_mask |= 
                               ICF_SIC_EARLY_SESSION_TUNNEL_MODE_OPERATION_PRESENT;
                    }
                    if (ICF_NULL != (p_early_session_cfm->bit_mask &
                                 ICF_EARLY_SESSION_CFM_RESPONSE_CODE_PRESENT))
                    {
                        early_session_cfm.response_code=  
                        p_early_session_cfm->response_code;
                        early_session_cfm.bit_mask |= 
                              ICF_SIC_EARLY_SESSION_RESPONSE_CODE_PRESENT;
                    }
                    break;
                }
		
		/* ICF_6_1_OPTIONS_SUPPORT end */
        /* ASYNCHRONUS_MESSAGE support starts */
                case ICF_MESSAGE_CFM:
                {
                    p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_CFM;
                    p_internal_msg->p_msg_data = (icf_uint8_t*)&message_cfm;

                    p_message_cfm = (icf_message_cfm_st*)
                        (p_msg->payload);

                    message_cfm.call_id = p_msg->hdr.call_id;
                    message_cfm.result = p_message_cfm->result;
                    message_cfm.header_list.hdr = ICF_NULL;
                    message_cfm.header_list.count=0;
                    message_cfm.tag_list.tag = ICF_NULL;
                    message_cfm.tag_list.count=0;

                    if( p_message_cfm->header_list.count )
                    {
                        icf_cmn_clone_header_list(
                                p_internal_msg->msg_hdr.p_glb_pdb,
                                &(p_message_cfm->header_list),
                                &(message_cfm.header_list),
                                &(p_internal_msg->msg_hdr.ecode) );
                        p_header_list = &(message_cfm.header_list);
                    }
                    if( p_message_cfm->tag_list.count )
                    {
                        icf_cmn_clone_tag_list(
                                p_internal_msg->msg_hdr.p_glb_pdb,
                                &(p_message_cfm->tag_list),
                                &(message_cfm.tag_list),
                                &(p_internal_msg->msg_hdr.ecode) );
                        p_tag_list = &(message_cfm.tag_list);
                    }
                    if(ICF_NULL != (p_message_cfm->bit_mask & ICF_APP_CONFIRMATION_RESP_CODE_PRESENT))
                    {
                        message_cfm.response_code = p_message_cfm->response_code;
                        message_cfm.bitmask |= 
                              ICF_SIC_APP_CONFIRMATION_RESP_CODE_PRESENT;

                    }
                    break;
                }
        /* ASYNCHRONUS_MESSAGE support ends */
		
	}

	icf_sic_process_mesg(p_internal_msg);
	if ( ICF_NULL != p_header_list)
	{
		icf_cmn_free_header_list(p_internal_msg->msg_hdr.p_glb_pdb,
			p_header_list, &(p_internal_msg->msg_hdr.ecode));
	}
	if ( ICF_NULL != p_tag_list)
	{
		icf_cmn_free_tag_list(p_internal_msg->msg_hdr.p_glb_pdb,
			p_tag_list, &(p_internal_msg->msg_hdr.ecode));
	}
	if ( ICF_NULL != p_body_list)
	{
		icf_cmn_free_msg_body_list(p_internal_msg->msg_hdr.p_glb_pdb,
			p_body_list, &(p_internal_msg->msg_hdr.ecode));
	}
	if ( ICF_NULL != p_app_stream_list)
	{
		icf_cmn_free_icf_stream_capab_list(
			p_internal_msg->msg_hdr.p_glb_pdb,
			&(p_app_stream_list));
	}

    /*Fix for SPR 20412: CSR 1-8223807 merged*/
    /*Fix for CSR 1-8223807 : SPR-20362*/
    if(ICF_NULL != p_remove_hdr_list)
    {
        icf_cmn_free_header_name_list (
            p_internal_msg->msg_hdr.p_glb_pdb,
            p_remove_hdr_list, &(p_internal_msg->msg_hdr.ecode));
    }

}

icf_return_t 
icf_mrm_validate_hdr_and_get_dest_module(icf_glb_pdb_st    *p_glb_pdb,
                                            icf_api_header_st *p_hdr, 
                                            icf_int_module_id_t *p_dest_int_module,
                                            icf_error_t *p_ecode )
{
    icf_boolean_t   validate_app_id = ICF_FALSE;
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_module_id_t recvd_src_id = p_hdr->source_id;
    icf_dbm_data_st      *p_dbm_data = ICF_NULL;
    icf_app_conf_st      *p_app_conf = ICF_NULL;
    icf_uint8_t          app_index=(icf_uint8_t)-1;
    icf_config_data_st   *p_config_data = ICF_NULL;

    if ((ICF_APP_ADD_REQ != p_hdr->api_id)
		&& (ICF_GET_LINE_SETTINGS_REQ != p_hdr->api_id)
		&&(ICF_GET_SYSTEM_SETTINGS_REQ != p_hdr->api_id) 
		&& (ICF_MODULE_PA == p_hdr->source_id 
		|| ICF_GENERIC_APP_MODULE_ID == p_hdr->source_id))
    {
	icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                p_ecode);
        
	icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t **)(&(p_config_data)),
                p_ecode);	
     	    p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;

            for(app_index=0;app_index<p_config_data->max_applications;app_index++)
    	    {
                if( ICF_TRUE==p_app_conf[app_index].is_valid)
                {
            	    if(p_hdr->app_id == p_app_conf[app_index].app_id)
            	    {
                      	validate_app_id = ICF_TRUE;
		    
            	    }
		    
                }
            }
	if( ICF_TRUE == validate_app_id)
        {
            ret_val = ICF_SUCCESS;
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
   
        
     }

    if(((ICF_MPH_API_BASE <= p_hdr->api_id)
			    && (ICF_MPH_API_END  >= p_hdr->api_id)) ||
		    (ICF_MODULE_MGMT_SERVER == p_hdr->source_id))
    {
		   *p_dest_int_module = ICF_INT_MODULE_MPH;
    }
	else if((ICF_APP_SETUP_API_BASE <= p_hdr->api_id) && 
        ( ICF_CFG_API_END >= p_hdr->api_id))
    {
        /* Destination module is CFG */
        *p_dest_int_module = ICF_INT_MODULE_CFG;
        
        /* For requests other than app_add, validate the src id */
        
        switch(p_hdr->api_id)
        {
            case ICF_APP_ADD_REQ : /* no validation required */
                break;

            default : /* Either app-id and src id are generic */
                {
                    validate_app_id = ICF_TRUE;
                    break;
                }
        }
    }
    else if((ICF_CONTROL_PLANE_API_BASE <= p_hdr->api_id)
        && (ICF_CONTROL_PLANE_API_END  > p_hdr->api_id))
    {
        validate_app_id = ICF_TRUE;
        
    	if((ICF_REGISTER_REQ == p_hdr->api_id)||(ICF_AKA_AUTH_RESP == p_hdr->api_id))
        {
           *p_dest_int_module = ICF_INT_MODULE_CFG;
        }   
        else if(
            ((ICF_SUBSCRIBE_REQ <= p_hdr->api_id) && 
             (ICF_REFER_REQ >= p_hdr->api_id)) ||
	  	    ((ICF_SUBSCRIPTION_STATUS_IND <= p_hdr->api_id) && 
             (ICF_REFER_RESP >= p_hdr->api_id))||
             (ICF_NOTIFY_CFM == p_hdr->api_id) ||
	/* ICF_6_1_OPTIONS_SUPPORT start */
	     ( ICF_OPTIONS_REQ == p_hdr->api_id) ||
	     ( ICF_OPTIONS_CFM == p_hdr->api_id) ||
	     ( ICF_MESSAGE_CFM == p_hdr->api_id) ||
             ( ICF_EARLY_SESSION_CFM == p_hdr->api_id)
            
	/* ICF_6_1_OPTIONS_SUPPORT end */

          )
        { 
            /* dest module is sic */
            *p_dest_int_module = ICF_INT_MODULE_SIC;
        }
        
        else
        {
            *p_dest_int_module = ICF_INT_MODULE_CM;
        }
    }    
    else if((ICF_SM_API_START <= p_hdr->api_id)
        && (ICF_SM_API_END  > p_hdr->api_id))
    {
        validate_app_id = ICF_TRUE;
        *p_dest_int_module = ICF_INT_MODULE_SM;
    }
    else 
    {
        /* Media API's */
        switch(p_hdr->api_id)
        {
            case ICF_TIMER_MSG : 
                *p_dest_int_module = ICF_INT_MODULE_MRM;
                break;

            case ICF_CFG_CODEC_INFO_REQ :
            case ICF_CFG_RINGER_TONES_REQ:
		
			case ICF_CFG_MEDIA_REQ:
			case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
			case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
		
			case ICF_CFG_JBM_MGMT_REQ :
            case ICF_CFG_MAX_EARP_VOL_REQ : 
            case ICF_CFG_MAX_SPEAKER_VOL_REQ :
            case ICF_CFG_SET_PORT_RANGE_REQ :
			case ICF_CFG_SET_SELF_IP_REQ:
            case ICF_START_RINGING_REQ :
            case ICF_MEDIA_SESSION_REQ :
			case ICF_ECHO_CANC_REQ:
			case ICF_VAD_SS_CNG_REQ:
            case ICF_GET_STATS_REQ :

			case ICF_PLAY_MEDIA_REQ:
			case ICF_STOP_MEDIA_PLAY_REQ:		         
			case ICF_STOP_RINGING_REQ :

			case ICF_MUTE_MEDIA_REQ:

            case ICF_SET_EARP_VOLUME_REQ :
            case ICF_SET_SPEAKER_VOLUME_REQ :
            case ICF_SEND_APP_DATA_REQ :
            case ICF_SEND_STATS_REQ :
            case ICF_STOP_STATS_REQ : 
			case ICF_CFG_MAX_MIC_GAIN_REQ:
			case ICF_SET_MIC_GAIN_REQ:
			case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
			case ICF_CONNECT_MEDIA_DEVICE_REQ:
			case ICF_MEDIA_RECORD_REQ:
			case ICF_STOP_MEDIA_RECORD_REQ:
			case ICF_PAUSE_MEDIA_REQ:

                {
                    p_hdr->source_id = ICF_MODULE_PA;
                    p_hdr->destination_id = ICF_MODULE_MM;
                    validate_app_id = ICF_TRUE;
                    *p_dest_int_module = ICF_INT_MODULE_MMI;
                     break;
                }
            case ICF_CFG_CODEC_INFO_RESP:
			case ICF_CFG_RINGER_TONES_RESP:

			case ICF_CFG_JBM_MGMT_RESP:
			case ICF_CFG_MAX_EARP_VOL_RESP:
			case ICF_CFG_MAX_SPEAKER_VOL_RESP:
			case ICF_CFG_SET_PORT_RANGE_RESP:
			case ICF_CFG_SET_SELF_IP_RESP:
			case ICF_CFG_MEDIA_RESP:
			case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:	
			case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
			case ICF_START_RINGING_RESP:
			case ICF_MEDIA_SESSION_RESP:
			case ICF_ECHO_CANC_RESP:
			case ICF_VAD_SS_CNG_RESP:
			case ICF_GET_STATS_RESP:
			case ICF_PLAY_MEDIA_RESP:
			case ICF_STOP_MEDIA_PLAY_RESP:		
			case ICF_STOP_RINGING_RESP:
			case ICF_STOP_MEDIA_PLAY_IND:		
			case ICF_STOP_RINGING_IND:
			case ICF_MUTE_MEDIA_RESP:
			case ICF_SET_EARP_VOLUME_RESP:
			case ICF_SET_SPEAKER_VOLUME_RESP:
			case ICF_SEND_APP_DATA_RESP:
			case ICF_RCV_APP_DATA_IND:
			case ICF_SEND_STATS_RESP:
			case ICF_STOP_STATS_RESP:
			case ICF_MM_INIT_COMPLETE:
			case ICF_CFG_MAX_MIC_GAIN_RESP:
			case ICF_SET_MIC_GAIN_RESP:		
            case ICF_RTP_START_IND:


			case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
			case ICF_CONNECT_MEDIA_DEVICE_RESP:
			case ICF_MEDIA_RECORD_RESP:
			case ICF_STOP_MEDIA_RECORD_RESP:
			case ICF_PAUSE_MEDIA_RESP:
                {
                    p_hdr->source_id = ICF_MODULE_MM;
                    p_hdr->destination_id = ICF_MODULE_PA;
                    *p_dest_int_module = ICF_INT_MODULE_MMI;
                    break;
                }
            case ICF_GET_CODEC_LIST_REQ :          
            case ICF_CODEC_COMMIT_REQ :
			case ICF_REL_RES_REQ:
			case ICF_MEDIA_CAPABILITIES_REQ:
			case ICF_OPEN_MEDIA_CHANNEL_REQ:
			case ICF_CLOSE_MEDIA_CHANNEL_REQ:
            case ICF_CREATE_MEDIA_SESSION_REQ :    
            case ICF_MODIFY_MEDIA_SESSION_REQ :    
            case ICF_DELETE_MEDIA_SESSION_REQ :    
            case ICF_MERGE_MEDIA_SESSION_REQ :
                 /* Tunnel mode specific MM APIs */
            case ICF_TUNNELLED_CREATE_MEDIA_SESSION_REQ:
            case ICF_TUNNELLED_MODIFY_MEDIA_SESSION_REQ:    
                *p_dest_int_module = ICF_INT_MODULE_MMI;
                break;
        #ifdef ICF_NAT_MEDIA_TRAVERSAL        
            case ICF_SEND_UDP_DATA_REQ:
                {
                    p_hdr->source_id = ICF_MODULE_NAT;
                    p_hdr->destination_id = ICF_MODULE_MM;
                    *p_dest_int_module = ICF_INT_MODULE_MMI;
                    break;
                }

			case ICF_RECV_UDP_DATA_IND:
                {
                    p_hdr->source_id = ICF_MODULE_MM;
                    p_hdr->destination_id = ICF_MODULE_NAT;
                    *p_dest_int_module = ICF_INT_MODULE_MMI;
                    break;
                }
    #endif
            case ICF_CODEC_COMMIT_RESP : 
            case ICF_CREATE_MEDIA_SESSION_RESP :   
            case ICF_MODIFY_MEDIA_SESSION_RESP :   
            case ICF_DELETE_MEDIA_SESSION_RESP :   
            case ICF_DELETE_MEDIA_SESSION_IND  :
            case ICF_MERGE_MEDIA_SESSION_RESP :    
            case ICF_GET_CODEC_LIST_RESP :        
            case ICF_MEDIA_CAPABILITIES_RESP :
            case ICF_OPEN_MEDIA_CHANNEL_RESP:
            /* Rel 8.1
             * If ICF_TUNNELLED_MEDIA_CAPABILITIES_RESP received 
             * then it must be sent to MMI
             */
            case ICF_TUNNELLED_MEDIA_CAPABILITIES_RESP:
                 /* Tunnel mode specific MM APIs */
            case ICF_TUNNELLED_CREATE_MEDIA_SESSION_RESP:
            case ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP:    

#ifdef ICF_NAT_MEDIA_TRAVERSAL
            case ICF_NAT_STUN_INIT_RESP:
            case ICF_NAT_GET_PUBLIC_IP_PORT_RESP:
            case ICF_NAT_ERROR_IND:
            case ICF_NAT_STUN_EXEC_PROC_RESP:
            case ICF_NAT_STUN_PROC_STATUS_IND:
#endif
                *p_dest_int_module = ICF_INT_MODULE_MMI;
                break;

            /* Network API's */
            case ICF_NW_API_MSG_FROM_PEER :
            case ICF_NW_API_OPEN_CONN_IND :
            case ICF_NW_API_CONNECT_SUCCESS :
            case ICF_NW_API_CONNECT_FAILURE :
            case ICF_NW_API_CLOSE_CONN_IND :
                *p_dest_int_module = ICF_INT_MODULE_SSA;
                break;
            /* Platform API's */
            case ICF_PORT_PLTFM_SA_CREATE_RESP : 
			case ICF_PORT_PLTFM_SA_MODIFY_RESP :
			case ICF_PORT_PLTFM_SA_DELETE_RESP :
			case ICF_PORT_PLTFM_SA_EXPIRY_IND  :
			case ICF_PORT_PLTFM_SA_FETCH_ENC_ALGO_RESP :
			case ICF_PORT_PLTFM_SA_FETCH_AUTH_ALGO_RESP :
#ifdef ICF_NW_ACTIVATE
			case ICF_PORT_PLTFM_PRIM_PDP_ACT_RSP:
			case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP :
			case ICF_PORT_PLTFM_PDP_DEACT_RSP :
			case ICF_PORT_PLTFM_PDP_DEACT_IND :
			case ICF_PORT_PLTFM_PDP_MODIFY_IND :
#endif
			case ICF_PORT_PLTFM_QOS_RES_NON_IMS_RESP:
			case ICF_PORT_PLTFM_QOS_REL_NON_IMS_RESP:
			case ICF_PORT_PLTFM_GET_DEVICE_PARAMS_RSP :
			case ICF_PORT_PLTFM_INIT_RSP :
			case ICF_PORT_PLTFM_DEINIT_RSP :
			case ICF_PORT_PLTFM_GET_ACCESS_NW_INFO_RSP :
			case ICF_PORT_PLTFM_GET_PCSCF_ADDRESS_RSP :
			case ICF_PORT_PLTFM_DNS_SERVICE_RSP :
			case ICF_PORT_PLTFM_AKA_CHALLENGE_RSP :
			case ICF_PORT_PLTFM_PLATFORM_EVENT_IND :
			case ICF_PORT_PLTFM_HOST_TO_IP_RSP :
				*p_dest_int_module = ICF_INT_MODULE_NM;
				break;
            default : 
                {
                    ret_val = ICF_FAILURE;
                    *p_ecode = ICF_CAUSE_INVALID_API_ID;
                    break;
                }

        }
    }
	
	if ((ICF_DUMMY_APP_ID == p_hdr->app_id) &&
	   (ICF_CFG_CODEC_INFO_REQ == p_hdr->api_id))
	{
		return ret_val;
	}
    else
	if(ICF_TRUE == validate_app_id)
    {
        icf_app_conf_st *p_app_conf = ICF_NULL;
        icf_error_t  ecode;
        
        /* For generic App id */
        if(ICF_GENERIC_APP_ID == p_hdr->app_id)
        {
            if(ICF_GENERIC_APP_MODULE_ID != recvd_src_id)
            {
                ret_val = ICF_FAILURE;
            }
        }
		/* Non-gneric APP id */
        else if(ICF_FAILURE == icf_cfg_fetch_app_conf(p_glb_pdb,
            p_hdr->app_id,
            &p_app_conf,
            &ecode))
        {
            /* No application record exists */
            ret_val = ICF_FAILURE;
        }
        /* app record exists */
        else if(p_app_conf->app_source_id != recvd_src_id)
        {
            ret_val = ICF_FAILURE;
        }   
    }
    return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_init
 *
 * DESCRIPTION:  This function will initialize the entire ICF
 *
 * RETURNS:      Returns Success or Failure 
 *
 ************************************************************************/
icf_return_t 
icf_init(
        INOUT	icf_init_param_st	*p_init_param,
		OUT icf_void_t			**p_p_icf_glb_data)
{
    icf_return_t				ret_val = ICF_SUCCESS;
    icf_internal_msg_st			icf_internal_msg = {{0,0,0,0,0,0},0} ;
	icf_glb_pdb_st				*p_glb_pdb = ICF_NULL;
	icf_uint8_t					sys_resource_limit = 0;
#ifndef ICF_NW_ACTIVATE
	icf_cfg_pcscf_return_val_st pcscf_return_val = {0,0};
    pcscf_return_val.p_init_param = ICF_NULL;
    pcscf_return_val.pcscf_return = ICF_FALSE;
#endif
	if(ICF_NULL == p_init_param)
	{
		return ICF_FAILURE;
	}
	else if(ICF_NULL == p_init_param->p_generic_rt_info	||
			((ICF_NULL == p_init_param->p_scalable_info) &&
			 (0 == p_init_param->auto_scale)))
	{
		return ICF_FAILURE;
	}
	else if ((ICF_BLOCKING_CONNECT != (p_init_param->bitmask & ICF_BLOCKING_CONNECT)) &&
		(ICF_NULL != p_init_param->p_tls_init_data))
	{
		return ICF_FAILURE;
	}


		if (!p_init_param->auto_scale)
		{
			sys_resource_limit = p_init_param->p_scalable_info->sys_resource_limit;
		}
		else
		{
			sys_resource_limit = ICF_DEFAULT_SYS_RESOURCE_LIMIT;
		}

#if defined ICF_PORT_WINCE
		if (ICF_FAILURE == icf_mrm_init(sys_resource_limit, 
										p_init_param->logs_indication,
										p_p_icf_glb_data))
#else
		if (ICF_FAILURE == icf_mrm_init(sys_resource_limit, 
										p_p_icf_glb_data))
#endif
		{
			ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[ICF INIT]: Exitting function <icf_init> with FAILURE\n"));
			ret_val = ICF_FAILURE;
		}
		else
		{
			p_glb_pdb = *p_p_icf_glb_data;

			icf_internal_msg.msg_hdr.msg_id = ICF_CFG_INTERNAL_INIT; 
			icf_internal_msg.msg_hdr.payload_length = 0; 
			icf_internal_msg.msg_hdr.p_glb_pdb = (icf_glb_pdb_st*)p_glb_pdb; 
			icf_internal_msg.msg_hdr.ecode = 0; 
			icf_internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE; 
			icf_internal_msg.p_msg_data= (icf_uint8_t *) p_init_param; 
			
			ret_val = icf_cfg_process_mesg(&icf_internal_msg);

		}
  
  return ret_val;
}


#ifdef ICF_NW_ACTIVATE
/****************************************************************************
** Function: icf_auto_activate
**
****************************************************************************
** DESCRIPTION: This function scales the database as per the initial parameters 
**            
** 
** 
**
***************************************************************************/
icf_return_t icf_auto_activate (
					INOUT	icf_glb_pdb_st		*p_glb_pdb, 
					IN		icf_init_param_st		*p_init_param,
					INOUT	icf_error_t			*p_ecode)
{
	icf_internal_msg_st			icf_internal_msg; 
	icf_return_t					ret_val = ICF_SUCCESS, mem_ret_val = ICF_SUCCESS; 
	icf_network_activate_req_st		*p_payload; 


	ICF_FUNCTION_ENTER(p_glb_pdb)
	ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_init_param))
	ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_ecode))
	if (  p_init_param->apn.str_len != 0 && 
			(ICF_ACCESS_NET_TYPE_GPRS == p_init_param->access_net_type)) 
	{
	
		icf_internal_msg.msg_hdr.msg_id = ICF_CFG_AUTO_ACTIVATE; 
		icf_internal_msg.msg_hdr.payload_length = 0; 
		icf_internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb; 
		icf_internal_msg.msg_hdr.ecode = 0; 
		icf_internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE; 
		icf_internal_msg.p_msg_data= (icf_uint8_t *) ICF_NULL; 
	
		/* this function simulates as if API is coming from Application, 
		   thus p_recv_msg is populated in the p_glb_data */
		ICF_ASSERT(p_glb_pdb,(ICF_NULL == p_glb_pdb->p_recv_msg))

		ICF_MEMGET(p_glb_pdb, 
					sizeof(icf_msg_st) + sizeof(icf_network_activate_req_st) , 
					ICF_MEM_COMMON,
					p_glb_pdb->p_recv_msg, 
					ICF_RET_ON_FAILURE, 
					p_ecode, 
					mem_ret_val)


		/* memory allocated for p_recv_msg and payload */
		p_glb_pdb->p_recv_msg->hdr.api_id = ICF_NETWORK_ACTIVATE_REQ; 
		p_glb_pdb->p_recv_msg->hdr.version = ICF_VERSION_1_0; 
		p_glb_pdb->p_recv_msg->hdr.source_id = ICF_DUMMY_MODULE_ID;
		p_glb_pdb->p_recv_msg->hdr.destination_id = ICF_MODULE_ICF;
		p_glb_pdb->p_recv_msg->hdr.call_id = (icf_uint16_t )ICF_INVALID_CALL_ID;
		p_glb_pdb->p_recv_msg->hdr.app_id = ICF_DUMMY_APP_ID;
		p_glb_pdb->p_recv_msg->hdr.api_length = sizeof(icf_msg_st) + 
			sizeof(icf_network_activate_req_st);
		p_payload = (icf_network_activate_req_st *)
				(p_glb_pdb->p_recv_msg->payload); 

		p_payload->access_type = ICF_GPRS; 
		p_payload->access_info.str_len = p_init_param->apn.str_len; 
		icf_port_strcpy(p_payload->access_info.str, p_init_param->apn.str);
		p_payload->routing_info.len = 0; 
		p_payload->routing_info.ptr = ICF_NULL; 

		ret_val = icf_cfg_process_mesg(&icf_internal_msg);
		/* return type is not checked */

		/* free the memory allocated */
		ICF_MEMFREE(p_glb_pdb, 
					p_glb_pdb->p_recv_msg,
					ICF_MEM_COMMON,
					p_ecode, 
					mem_ret_val)
	}
	else
	{
		ret_val = ICF_FAILURE;
	}
	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val; 
}
#endif

/****************************************************************************
** Function: icf_auto_register
**
** DESCRIPTION: This function triggers a REGISTER message on the network 
**				to automatically register the 
**            
** 
*****************************************************************************/
icf_return_t icf_auto_register(
			INOUT	icf_glb_pdb_st     *p_glb_pdb, 
			IN		icf_init_param_st  *p_init_param,
			INOUT	icf_error_t        *p_ecode)
{
#ifdef ICF_NW_ACTIVATE
	icf_internal_msg_st	icf_internal_msg; 
        icf_return_t mem_ret_val= ICF_SUCCESS;
	icf_register_req_st	*p_payload = ICF_NULL; 
#endif
	icf_return_t		ret_val = ICF_SUCCESS; 
	
    	if(ICF_NULL == p_glb_pdb)
    	{
       	   return ICF_FAILURE;
    	}
	ICF_FUNCTION_ENTER(p_glb_pdb)
	ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_init_param))
	ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_ecode))

#ifndef ICF_NW_ACTIVATE
/* To remove warnings */
   p_init_param=p_init_param;
   p_ecode=p_ecode;
#endif
#ifdef ICF_NW_ACTIVATE
	if ( ICF_ACCESS_NET_TYPE_GPRS == p_init_param->access_net_type ) 
	{
	
		icf_internal_msg.msg_hdr.msg_id = ICF_CFG_AUTO_REGISTER; 
		icf_internal_msg.msg_hdr.payload_length = 0; 
		icf_internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb; 
		icf_internal_msg.msg_hdr.ecode = 0; 
		icf_internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE; 
		icf_internal_msg.p_msg_data= (icf_uint8_t *) ICF_NULL; 
	
		/* this function simulates as if API is coming from Application, 
		   thus p_recv_msg is populated in the p_glb_data */
		ICF_ASSERT(p_glb_pdb,(ICF_NULL == p_glb_pdb->p_recv_msg))

		ICF_MEMGET(p_glb_pdb, 
			sizeof(icf_msg_st) + sizeof(icf_register_req_st) , 
			ICF_MEM_COMMON,
			p_glb_pdb->p_recv_msg, 
			ICF_RET_ON_FAILURE, 
			p_ecode, 
			mem_ret_val)

       
       icf_port_memset(p_glb_pdb->p_recv_msg, 0,
					sizeof(icf_msg_st) + sizeof(icf_register_req_st));

		/* memory allocated for p_recv_msg and payload */
		p_glb_pdb->p_recv_msg->hdr.api_id = ICF_REGISTER_REQ;
		p_glb_pdb->p_recv_msg->hdr.version = ICF_VERSION_1_0; 
		p_glb_pdb->p_recv_msg->hdr.source_id = ICF_DUMMY_MODULE_ID;
		p_glb_pdb->p_recv_msg->hdr.destination_id = ICF_MODULE_ICF;
		p_glb_pdb->p_recv_msg->hdr.call_id = (icf_uint16_t 
					)ICF_INVALID_CALL_ID;
		p_glb_pdb->p_recv_msg->hdr.app_id = ICF_DUMMY_APP_ID;
		p_glb_pdb->p_recv_msg->hdr.api_length = sizeof(icf_msg_st) + 
												sizeof(icf_register_req_st);
		p_payload = (icf_register_req_st *)(p_glb_pdb->
					p_recv_msg->payload); 
		p_payload->bit_mask = ICF_NULL;

		/* deafult line id being filled */
		p_payload->line_id = ICF_USE_LINE1;
		p_payload->header_list.count = ICF_NULL;
		p_payload->tag_list.count = ICF_NULL;


		ret_val = icf_cfg_process_mesg(&icf_internal_msg);
		/* return type is not checked */

		/* free the memory allocated */
		ICF_MEMFREE(p_glb_pdb, 
				p_glb_pdb->p_recv_msg,
				ICF_MEM_COMMON,
				p_ecode, 
				mem_ret_val)
	}
	else
	{
		ret_val = ICF_FAILURE;
	}
#endif	
	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val; 
}/* Function icf_auto_register() ends here  */
/******************************************************************************
*
* FUNCTION NAME    :   icf_mrm_app_trace
*
* DESCRIPTION  : This function is used to initialize trace data
*
* RETURNS:
******************************************************************************/
#ifdef ICF_TRACE_ENABLE
icf_return_t icf_mrm_app_trace(
        INOUT  icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_trace_id_t trace_id)
{
    icf_global_port_info_st *p_glb_port_data =
      (icf_global_port_info_st *)p_glb_pdb->p_glb_data->p_glb_port_info;
    icf_trace_data_st      trace_data;
    icf_trace_criteria_data_st *p_criteria_data = ICF_NULL;
    /* Fix for CSR1-5249829 */
    icf_return_t ret_val = ICF_SUCCESS;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */
    ret_val = icf_cmn_check_app_trace(p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
       return ret_val;
    }
    else
    {
    if( ICF_NULL == p_glb_pdb->p_recv_msg)
    {
       return ret_val;
    }
    else
    {
       p_glb_pdb->p_glb_data->app_trace_info.app_id =
         p_glb_pdb->p_recv_msg->hdr.app_id;
       p_glb_pdb->p_glb_data->app_trace_info.call_id =
         p_glb_pdb->p_recv_msg->hdr.call_id;
    }
    switch(trace_id)
    {
      case ICF_TRACE_UDP_RECV_ERROR:
         trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
         trace_data.int_val[0] =
                     (icf_uint32_t)(p_glb_port_data->icf_trace_ipc_info.remote_port);
         trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
         icf_port_strcpy(trace_data.string_val[0].str,
                 p_glb_port_data->icf_trace_ipc_info.remote_ip);
         trace_data.string_val[0].str_len =(icf_uint16_t)
                    icf_port_strlen(trace_data.string_val[0].str);
         break;
      case ICF_TRACE_TCP_RECV_ERROR:
      case ICF_TRACE_TLS_RECV_ERROR:
         trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
         trace_data.int_val[0] =
                        (icf_uint32_t)(p_glb_port_data->icf_trace_ipc_info.local_port);
         trace_data.int_val[1] =
                        (icf_uint32_t)(p_glb_port_data->icf_trace_ipc_info.remote_port);
         trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
         icf_port_strcpy(trace_data.string_val[0].str,
                                 p_glb_port_data->icf_trace_ipc_info.local_ip);
         trace_data.string_val[0].str_len =(icf_uint16_t)
                     icf_port_strlen(trace_data.string_val[0].str);
         icf_port_strcpy(trace_data.string_val[1].str,
                                   p_glb_port_data->icf_trace_ipc_info.remote_ip);
         trace_data.string_val[1].str_len =(icf_uint16_t)
                   icf_port_strlen(trace_data.string_val[0].str);
         break;
      default:
         break;
    }
    /* Finally, invoke the common macro for trace handling */
    ICF_APP_TRACE(p_glb_pdb, trace_id, &trace_data,
                           p_criteria_data)
}
    return ret_val;
}
#endif

/******************************************************************************
*
* FUNCTION NAME    :   icf_invalid_module_handling
*
* DESCRIPTION      :   This is a dummy function for removing the klocwork
*                      warning, for initialising the function pointer table
*                      icf_mrm_func_ptr_t in function icf_process_message.    
*
* RETURNS:         :   Always return SUCCESS   
******************************************************************************/
icf_return_t icf_invalid_module_handling(
              INOUT icf_internal_msg_st        *p_internal_msg)
{
        icf_return_t ret_val = ICF_SUCCESS;
        p_internal_msg = p_internal_msg;
        ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"[MRM]: Function icf_process_message get INVALID Module\n"));        
        return ret_val;
}
