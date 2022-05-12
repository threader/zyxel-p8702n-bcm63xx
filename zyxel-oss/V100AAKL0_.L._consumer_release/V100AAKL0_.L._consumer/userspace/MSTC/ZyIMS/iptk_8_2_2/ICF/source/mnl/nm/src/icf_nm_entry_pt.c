/*------------------------------------------------------------------------------
 *
 * File name        : icf_nm_entry_pt.c
 *
 * Purpose          : This file contains the entry point function of Network 
 *						Manager(NM) that is invoked by other modules of
 *						the toolkit.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 02-Aug-2005  Irfan Khan	    LLD			Initial
 * 08-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD Coding for IPSec support
 * 08-Apr-2008  Tarun Gupta      SPR 18355   Merged Windows Porting Changes
 *
 * Copyright (c) 2008, Aricent.
 *----------------------------------------------------------------------------*/
#include "icf_map_extif_feature_flags.h"
#include "icf_common_prototypes.h" 
#include "icf_port_prototypes.h"
#include "icf_dbm_prototypes.h" 
#include "icf_port_prototypes.h" 
#include "icf_macro.h"
#include "icf_internal_api_id.h"
#include "icf_nm_prototypes.h"
#include "icf_dbm_prototypes.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif



icf_return_t 	icf_nm_allocate_mem_for_nm_cntxt
					(INOUT   icf_glb_pdb_st        *p_glb_pdb,
					 INOUT   icf_nm_global_st      **p_p_nm_glb,
					 OUT    icf_error_t            *p_ecode);

icf_return_t icf_nm_deallocate_mem_for_nm_cntxt
				(INOUT   icf_nm_global_st        *p_nm_glb,
				 OUT    icf_error_t            *p_ecode);

#ifndef ICF_QOS_STUBBED_OUT
extern icf_return_t icf_port_pltfm_init_send(icf_return_t (*asmcWrprSend)(void *msg));
#endif

/*******************************************************************************
 *
 * FUNCTION	   :     icf_nm_get_icf_messg_for_pltfm
 *
 * DESCRIPTION :     This function will allocate mem for icf_msg_st and assign 
 *					  values to hdrs which need to be sent to Pltfm
 *					
 *
 * RETURN	   :  	 ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/
icf_return_t icf_nm_get_icf_messg_for_pltfm(INOUT   icf_glb_pdb_st       *p_glb_pdb,
												  OUT icf_msg_st ** p_p_icf_msg,
												  IN icf_int32_t payload_size)

{
	icf_return_t			  ret_val = ICF_SUCCESS;
	icf_error_t				  ecode;
	icf_uint32_t			  errhndlr = ICF_DONOT_RET_ON_FAILURE;


	ICF_FUNCTION_ENTER(p_glb_pdb)

	ICF_STATIC_MEMGET(p_glb_pdb,
						 sizeof(icf_api_header_st) + payload_size,
						 *p_p_icf_msg, (icf_uint16_t) errhndlr ,&ecode, ret_val);

	if( ICF_SUCCESS == ret_val )
	{

		(*p_p_icf_msg)->hdr.version = ICF_VERSION_1_0;
        (*p_p_icf_msg)->hdr.source_id = 0;
        (*p_p_icf_msg)->hdr.destination_id = 0;
        (*p_p_icf_msg)->hdr.call_id = 0;
        (*p_p_icf_msg)->hdr.app_id =  0;
        (*p_p_icf_msg)->hdr.api_length = \
            (icf_uint16_t ) (sizeof(icf_api_header_st) + payload_size);
	}
	
	return ret_val;

	ICF_FUNCTION_EXIT(p_glb_pdb)

}

/*******************************************************************************
 *
 * FUNCTION	   :     icf_nm_free_icf_messg_for_pltfm
 *
 * DESCRIPTION :     This function will free memory allocated for icf_msg_st
 *					
 *
 * RETURN	   :  	 ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/

icf_return_t icf_nm_free_icf_messg_for_pltfm(INOUT   icf_glb_pdb_st       *p_glb_pdb,
												  IN icf_msg_st * p_icf_msg)

{
	icf_return_t			  ret_val = ICF_SUCCESS;
	icf_error_t            ecode;
	
	ICF_FUNCTION_ENTER(p_glb_pdb)

	ICF_STATIC_MEMFREE(
                p_glb_pdb,
                p_icf_msg,
                &ecode,
                ret_val)

	return ret_val;

	ICF_FUNCTION_EXIT(p_glb_pdb)

}


 /*******************************************************************************
 *
 * FUNCTION:        icf_nm_process_mesg
 *
 * DESCRIPTION:     This is the only entry point of Network Manager. It will be
 *					used for all the requests/messages to NM. It will call 
 *					internal functions as per the request parameters and  may 
 *					send messages to other modules to notify about platform/
 *					network events.
 *					
 * RETURN			ICF_SUCCESS/ICF_FAILURE.
 *
******************************************************************************/
icf_return_t icf_nm_process_mesg(INOUT icf_internal_msg_st	*p_msg)
{
	icf_return_t                     ret_val = ICF_SUCCESS;
	icf_msg_st                       *p_ext_msg = ICF_NULL;
    icf_nm_pdb_st                    nm_pdb;
    
   	ICF_SET_MODULE(p_msg->msg_hdr.p_glb_pdb,ICF_INT_MODULE_NM);

	ICF_FUNCTION_ENTER(p_msg->msg_hdr.p_glb_pdb)
		
		/* initialize NM pdb */
		nm_pdb.p_glb_pdb = p_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(p_msg->msg_hdr.p_glb_pdb, nm_pdb.p_glb_pdb)
		
		nm_pdb.p_internal_msg = p_msg;
    ICF_ASSERT(p_msg->msg_hdr.p_glb_pdb, nm_pdb.p_internal_msg)
		
		nm_pdb.p_ecode = &(p_msg->msg_hdr.ecode);
	
    /* copy pointer to NM global data in NM pdb */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
		nm_pdb.p_glb_pdb,
		(icf_int_module_id_t)ICF_INT_MODULE_NM,
		(icf_void_t *)(&nm_pdb.p_nm_glb_data),
		nm_pdb.p_ecode))
    {
        ret_val = ICF_FAILURE;

        ICF_FUNCTION_EXIT(nm_pdb.p_glb_pdb)
        ICF_RESET_MODULE(nm_pdb.p_glb_pdb);

		return ret_val;
    }
	
	switch(p_msg->msg_hdr.msg_id)
	{

#ifdef ICF_NW_ACTIVATE
	case ICF_NM_NW_INFO:
		{
			ret_val = icf_nm_get_nw_info(
				(icf_nm_nw_info_req_st*)p_msg->p_msg_data,
				&(p_msg->msg_hdr));
			break;
		}
#endif

/*#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(nm_pdb.p_glb_pdb,ICF_TRUE)*/
	case ICF_NM_ACS_INFO:
		{
			ret_val = icf_nm_get_acs_info((icf_nm_acs_info_st*)
				p_msg->p_msg_data,&(p_msg->msg_hdr));
			break;
		}
/*
ICF_CHECK_IMS_END
#endif*/
	case ICF_NM_P_NW_INFO:
		{
			ret_val = icf_nm_get_pacs_info((icf_nm_pnwacs_info_st*)
				p_msg->p_msg_data,&(p_msg->msg_hdr));
			break;
		}

#ifdef ICF_NW_ACTIVATE
	case ICF_NM_ACT:
		{
			ret_val = icf_nm_activate((icf_nm_pdp_st*)p_msg->p_msg_data, 
				&p_msg->msg_hdr);
			break;
		}
		
	case ICF_NM_DEACT:
		{
			ret_val = icf_nm_deactivate((icf_nm_deactivate_st*)
				p_msg->p_msg_data, &p_msg->msg_hdr);
			break;
		}
#endif
	
	case ICF_NM_AUTH:
		{
			ret_val = icf_nm_auth((icf_nm_aka_info_st*)p_msg->p_msg_data,
				&p_msg->msg_hdr);
			break;
		}

#ifdef ICF_QOS_SUPPORT

#ifdef ICF_NW_ACTIVATE

		case ICF_NM_MULTIPLE_SEC_ACT :
		{
			ret_val = icf_nm_activate_multiple_sec_pdps(
			(icf_nm_multiple_sec_pdp_activate_st*)p_msg->p_msg_data,
			&p_msg->msg_hdr);
			break;
		}
		
		case ICF_NM_MULTIPLE_SEC_DEACT :
		{
			ret_val = icf_nm_deactivate_multiple_sec_pdps(
			(icf_nm_multiple_sec_pdp_deactivate_st*)p_msg->p_msg_data,
			&p_msg->msg_hdr);
			break;
		}

#endif

	case ICF_NM_QOS_RESERVE_FOR_NON_IMS:
		{
			ret_val = icf_nm_qos_res_for_non_ims(
			(icf_nm_non_ims_qos_reserve_st*)p_msg->p_msg_data,
			&p_msg->msg_hdr, &nm_pdb);
			break;
		}

	case ICF_NM_QOS_RELEASE_FOR_NON_IMS:
		{
			ret_val = icf_nm_qos_rel_for_non_ims(
			(icf_nm_non_ims_qos_release_st*)p_msg->p_msg_data,
			&p_msg->msg_hdr, &nm_pdb);
			break;
		}
#endif


	   case ICF_MSG_FROM_MRM:
        {
            /* Message received from Wrapper */
            p_ext_msg = (icf_msg_st *)(nm_pdb.p_glb_pdb->p_recv_msg);
                 
                
				
				switch (p_ext_msg->hdr.api_id)
            {
#ifdef ICF_IPSEC_ENABLED
				
	case ICF_PORT_PLTFM_SA_CREATE_RESP:
		ret_val = icf_nm_sa_create_resp(&nm_pdb);
		break;
		
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT                   
	 case ICF_PORT_PLTFM_SA_MODIFY_RESP:
		ret_val = icf_nm_sa_modify_resp(&nm_pdb);
		break;
#endif
		
	  case ICF_PORT_PLTFM_SA_DELETE_RESP:
		ret_val = icf_nm_sa_delete_resp(&nm_pdb);
		break;
		
	   case ICF_PORT_PLTFM_SA_EXPIRY_IND:
		   ret_val = icf_nm_sa_expiry_ind(&nm_pdb);
           break;
#endif /* ICF_IPSEC_ENABLED */

#ifdef ICF_NW_ACTIVATE
				case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP:
					ret_val=icf_nm_process_pdp_activate_rsp(
						&nm_pdb);
					break;
#if 0					
				case ICF_PORT_PLTFM_PDP_DEACT_RSP:
					ret_val=icf_nm_process_pdp_deactivate_rsp(
						&nm_pdb);
					break;
#endif					
				case ICF_PORT_PLTFM_PDP_DEACT_IND:
					ret_val=icf_nm_process_pdp_deactivate_ind(
						&nm_pdb);					
					break;
					
				case ICF_PORT_PLTFM_PDP_MODIFY_IND:
					ret_val=icf_nm_process_pdp_modify_ind(
						&nm_pdb);
					break;
					
				case ICF_PORT_PLTFM_PLATFORM_EVENT_IND:
					ret_val=icf_nm_send_platform_event_to_cfg(
						&nm_pdb);
					break;
#endif

				case ICF_PORT_PLTFM_QOS_RES_NON_IMS_RESP:
#ifdef ICF_QOS_SUPPORT
					ret_val = icf_nm_send_qos_reserve_non_ims_resp(&nm_pdb);
#endif
					break;
#if 0
				case ICF_PORT_PLTFM_QOS_REL_NON_IMS_RESP:
#ifdef ICF_QOS_SUPPORT
					ret_val = icf_nm_send_qos_release_non_ims_resp(&nm_pdb);
#endif
					break;
#endif
                default:
                    p_msg->msg_hdr.ecode = ICF_INVALID_MSG_ID_RECD;
					ret_val = ICF_FAILURE;
                    break;
            } /* End switch(api_id) */
		}
        break;
		
#ifdef ICF_IPSEC_ENABLED
    case ICF_RGM_NM_SA_FETCH_ALGO:
		{
			ret_val = icf_nm_sa_fetch_algo(&nm_pdb);
			break;
		}
    case ICF_RGM_NM_SA_CREATE_REQ:
		{
			ret_val = icf_nm_sa_create_req(&nm_pdb);
			break;
		}
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
    case ICF_RGM_NM_SA_MODIFY_REQ:
		{
			ret_val = icf_nm_sa_modify_req(&nm_pdb);
			break;
		}
#endif
    case ICF_RGM_NM_SA_DELETE_REQ:
		{
			ret_val = icf_nm_sa_delete_req(&nm_pdb);
			break;
		}
#endif /* ICF_IPSEC_ENABLED */
		
	case ICF_NM_SCALABLE_PARAMS_REQ:
		{
			ret_val = icf_nm_process_scalable_params_req(&nm_pdb);
			break;
		}

	default:
		{
			p_msg->msg_hdr.ecode = ICF_ERROR_UNDEFINED;
			ret_val = ICF_FAILURE;
		}
		
    } /* End switch(msg_id) */
	
	ICF_FUNCTION_EXIT(p_msg->msg_hdr.p_glb_pdb)
    ICF_RESET_MODULE(nm_pdb.p_glb_pdb);

		
		return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_allocate_mem_for_nm_cntxt
 *
 * DESCRIPTION:     This function will allocate memory for global data structure 
 *					of NM.
 *
 * RETURN			:ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/

icf_return_t 	icf_nm_allocate_mem_for_nm_cntxt
					(INOUT   icf_glb_pdb_st        *p_glb_pdb,
					 INOUT   icf_nm_global_st      **p_p_nm_glb,
					 OUT    icf_error_t            *p_ecode)
{
	icf_nm_global_st			 *p_nm_glb = ICF_NULL;
	icf_uint32_t				 errhndlr = ICF_DONOT_RET_ON_FAILURE;
	icf_return_t			 	 ret_val = ICF_SUCCESS;
	ICF_FUNCTION_ENTER(p_glb_pdb)

	/*We need to allocate memory for the following pointers */
	
	/**
	Pointer to global structure for NM 
	icf_nm_global_st 			*p_nm_glb = ICF_NULL;
	Pointer to the list of Network Informations
		icf_nm_nw_info_st			*p_n_info_list;
	Pointer to list having all the information related to Network Access
		icf_nm_access_info_st		*p_acc_info;
	**/

	ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_nm_global_st),
						 p_nm_glb, (icf_uint16_t) errhndlr ,p_ecode, ret_val);

	if(ICF_SUCCESS == ret_val)
	{

		icf_port_memset(p_nm_glb,0,sizeof(icf_nm_global_st));
		ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_nm_access_info_st),
       	     							p_nm_glb->p_acc_info, (icf_uint16_t) errhndlr ,p_ecode, ret_val);
		if(ICF_SUCCESS==ret_val)
		{

			icf_port_memset(p_nm_glb->p_acc_info,0,sizeof(icf_nm_access_info_st));

#ifdef ICF_NW_ACTIVATE
			ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_nm_nw_info_st),
       	     							p_nm_glb->p_n_info_list, errhndlr ,p_ecode, ret_val);
			if(ICF_SUCCESS==ret_val)
			{
				icf_port_memset(p_nm_glb->p_n_info_list,0,sizeof(icf_nm_nw_info_st));

				p_nm_glb->p_n_info_list->pdp_cntxt_info.prim_pdp_id=
					ICF_NM_INVALID_PRIMARY_PDP_CTXT_ID;
			}
			p_nm_glb->p_n_info_list->pdp_cntxt_info.p_callid_sec_pdp_ctxts_map=ICF_NULL;
#endif
		}
		
		p_nm_glb->p_icf_glb_ptr = p_glb_pdb;
		*p_p_nm_glb=p_nm_glb;
	}
	
	ICF_FUNCTION_EXIT(p_glb_pdb)
		
	return ret_val;
	
}


icf_return_t icf_nm_deallocate_mem_for_nm_cntxt
				(INOUT   icf_nm_global_st        *p_nm_glb,
				 OUT    icf_error_t            *p_ecode)
{
	icf_return_t				ret_val = ICF_SUCCESS;
	icf_nm_access_info_st    *p_acc_info= ICF_NULL;
#ifdef ICF_NW_ACTIVATE
	icf_nm_nw_info_st	    *p_n_info_list=ICF_NULL;
#endif

	/*
	icf_glb_pdb_st	*p_icf_glb_ptr=p_nm_glb->p_icf_glb_ptr;


	ICF_FUNCTION_ENTER(p_icf_glb_ptr)
	*/

	if(ICF_NULL!=p_nm_glb)
	{
		ICF_FUNCTION_ENTER(p_nm_glb->p_icf_glb_ptr)
		p_acc_info=p_nm_glb->p_acc_info;
#ifdef ICF_NW_ACTIVATE
		p_n_info_list=p_nm_glb->p_n_info_list;
#endif
#ifdef ICF_IPSEC_ENABLED
        /* free the security algorithm list stored in NM global data */
        if (ICF_NULL != p_nm_glb->p_ue_algo_list)
        {
            ICF_STATIC_MEMFREE(
                p_nm_glb->p_icf_glb_ptr,
                p_nm_glb->p_ue_algo_list,
                p_ecode,
                ret_val)
        }
#endif /* ICF_IPSEC_ENABLED */

        if(ICF_NULL!=p_acc_info)
        {
#ifdef ICF_NW_ACTIVATE
			if(ICF_NULL != p_acc_info->p_apn_list)
				ICF_STATIC_MEMFREE(p_nm_glb->p_icf_glb_ptr,
				p_acc_info->p_apn_list,	
				p_ecode,ret_val);
#endif			
			if( 0 != p_acc_info->nofpbuids)
				ICF_STATIC_MEMFREE(p_nm_glb->p_icf_glb_ptr,
				p_acc_info->p_public_usr_id,
				p_ecode,ret_val);
			
			if( 0 != p_acc_info->nofprids)
				ICF_STATIC_MEMFREE(p_nm_glb->p_icf_glb_ptr,
				p_acc_info->p_private_usr_id,
				p_ecode,ret_val);

			ICF_STATIC_MEMFREE(p_nm_glb->p_icf_glb_ptr,
			p_acc_info,
			p_ecode,ret_val);
		}

#ifdef ICF_NW_ACTIVATE
		if(ICF_NULL!=p_n_info_list)
		{
			if(ICF_NULL!=p_nm_glb->p_n_info_list->pdp_cntxt_info.p_callid_sec_pdp_ctxts_map)
			{
				ICF_STATIC_MEMFREE(p_nm_glb->p_icf_glb_ptr,
				p_nm_glb->p_n_info_list->pdp_cntxt_info.p_callid_sec_pdp_ctxts_map,
				p_ecode,ret_val);
			}

			ICF_STATIC_MEMFREE(p_nm_glb->p_icf_glb_ptr,
			p_n_info_list,
			p_ecode,ret_val);
		
		}
#endif
		ICF_FUNCTION_EXIT(p_nm_glb->p_icf_glb_ptr)
		if(ICF_NULL!=p_nm_glb)
		{
			ICF_STATIC_MEMFREE(p_nm_glb->p_icf_glb_ptr,
			p_nm_glb,
			p_ecode,ret_val);
		}
		
	}

	/* ICF_FUNCTION_EXIT(p_icf_glb_ptr) */

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_init
 *
 * DESCRIPTION:     This function will allocate memory for global data structure 
 *				   of NM.It initializes the NM module by getting the information
 *				   from the platform.
 * RETURN			:ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/
icf_return_t icf_nm_init(INOUT   icf_glb_pdb_st        *p_glb_pdb,
								OUT    icf_void_t           **p_p_glb_data,
								OUT    icf_error_t          *p_ecode) 
{
	icf_return_t							 ret_val= ICF_SUCCESS;
	icf_nm_global_st						 *p_nm_glb = ICF_NULL;
	icf_msg_st								 *p_port_pltfm_req=ICF_NULL,\
											 *p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t				 port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;
#ifdef ICF_NW_ACTIVATE
	icf_string_st							 *p_apnlist=ICF_NULL;
#endif
#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT || defined ICF_IPSEC_ENABLED
	icf_int32_t								 temp;
#endif
#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
	icf_port_pltfm_device_params_resp_st	 *p_dev_params_resp;
	icf_uint32_t							 errhndlr = ICF_DONOT_RET_ON_FAILURE;
#endif
#ifdef ICF_IPSEC_ENABLED
	icf_port_pltfm_fetch_algo_resp_st		 *p_ipsec_algos;
#endif



   	ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_NM);

	ICF_FUNCTION_ENTER(p_glb_pdb)

	
	/*Allocates memory for NM's global structure. Return if error*/
	
	ret_val = icf_nm_allocate_mem_for_nm_cntxt(p_glb_pdb,&p_nm_glb,p_ecode);

	if(ICF_SUCCESS==ret_val)
	{

		icf_nm_get_icf_messg_for_pltfm(p_glb_pdb,
										  &p_port_pltfm_req,
										  sizeof(icf_port_pltfm_device_params_req_st));

		icf_nm_get_icf_messg_for_pltfm(p_glb_pdb,
										  &p_port_pltfm_rsp,
										  sizeof(icf_port_pltfm_device_params_resp_st));

		/* Initialize the icf global pointer in NM Global Pointer */
		p_nm_glb->p_icf_glb_ptr = p_glb_pdb;

		/* Init the Wrapper */
		p_port_pltfm_req->hdr.api_id=ICF_PORT_PLTFM_INIT_REQ;

		icf_port_pltfm_init(p_port_pltfm_req,
						  &port_pltfm_retval,
						  p_port_pltfm_rsp,
						  icf_port_cbk_RsporIndCallBack);

if(ICF_PORT_PLTFM_SUCCESS == port_pltfm_retval)
{
#ifndef ICF_QOS_STUBBED_OUT
	icf_port_pltfm_init_send(icf_port_pltfm_send);
#endif
#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
			/* Init the Wrapper */
			p_port_pltfm_req->hdr.api_id=ICF_PORT_PLTFM_GET_DEVICE_PARAMS_REQ;
			ret_val=icf_port_pltfm_get_device_params(
						  p_port_pltfm_req,
						  &port_pltfm_retval,
						  p_port_pltfm_rsp);

			if(ICF_PORT_PLTFM_SUCCESS == port_pltfm_retval)
			{
				p_dev_params_resp=(icf_port_pltfm_device_params_resp_st*)p_port_pltfm_rsp->payload;
				/*Copying imsi and SIM type*/
				p_nm_glb->simType=p_dev_params_resp->sim_type;
				p_nm_glb->imsi=p_dev_params_resp->imsi;
				if (ICF_PORT_PLTFM_EMIM3GISIM == p_dev_params_resp->sim_type)
				{
					p_nm_glb->p_acc_info->home_nw_dmn=p_dev_params_resp->home_nw_domain;

	#ifdef ICF_NW_ACTIVATE
					p_nm_glb->p_acc_info->access_type=ICF_GPRS;
					p_nm_glb->p_acc_info->nofapns=p_dev_params_resp->num_of_apns;
	    			temp = p_nm_glb->p_acc_info->nofapns;
					/*Memory allocation for APN list*/
					if(0 != temp)
					{
						ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_string_st)*temp,
							 			 p_apnlist,errhndlr ,p_ecode, ret_val);
						/*copying the list of APN to access_info*/
						while(0 <= --temp)
							p_apnlist[temp]= p_dev_params_resp->apn_list[temp];

	 					p_nm_glb->p_acc_info->p_apn_list=p_apnlist;
					}
	#endif

					p_nm_glb->p_acc_info->nofpbuids=p_dev_params_resp->num_public_uris;
					temp = p_nm_glb->p_acc_info->nofpbuids;

					p_nm_glb->p_acc_info->p_public_usr_id = ICF_NULL;

					if(0 != temp)
					{
						ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_string_st)* temp,
						p_nm_glb->p_acc_info->p_public_usr_id, errhndlr ,p_ecode, ret_val);

						/*copying the list of public user ids */
						while(0 <= --temp)
							p_nm_glb->p_acc_info->p_public_usr_id[temp]=p_dev_params_resp->public_user_id[temp];
					}

					/*allocating memory for private user id */
					ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_private_id_st),
	 									 p_nm_glb->p_acc_info->p_private_usr_id, 
										 errhndlr ,p_ecode, ret_val); 

					if(ICF_SUCCESS != ret_val)
					{
						*p_p_glb_data = ICF_NULL;
					}
					else
					{
						p_nm_glb->p_acc_info->nofprids = 1; 

						if(0==(p_dev_params_resp->private_user_id.str_len))
						{
							p_nm_glb->p_acc_info->p_private_usr_id[temp].type=
									ICF_PUID_TEMPORARY;
							/*calculate the temperary private user id and assign */
			
						}
						else
						{
							temp = 1;
							p_nm_glb->p_acc_info->p_private_usr_id->type = ICF_PUID_STORED;
							p_nm_glb->p_acc_info->p_private_usr_id->value=
								p_dev_params_resp->private_user_id;
						}

						*p_ecode = ICF_SUCCESS;
						*p_p_glb_data	= (icf_void_t*) p_nm_glb;
					}
				}
				else if (ICF_PORT_PLTFM_EMIM3GUSIM == p_dev_params_resp->sim_type)
				{
					if (ICF_SUCCESS == icf_nm_get_usim_info(p_glb_pdb, p_dev_params_resp, &p_nm_glb, p_ecode))
					{
						/*USIM data extracted*/
						*p_ecode = ICF_SUCCESS;
						*p_p_glb_data	= (icf_void_t*) p_nm_glb;
					}
					else
					{
						/*Raise alarm "USIM data not extracted"*/
					}
				}
				else
				{
					/*For other types of SIM applications we will */
					/* configure public id , private id and hm nw domain */
					/*  from Application*/
					*p_ecode = ICF_SUCCESS;
					*p_p_glb_data	= (icf_void_t*) p_nm_glb;
				}
			
			}
#else
#ifdef ICF_QOS_SUPPORT
		*p_ecode = ICF_SUCCESS;
		*p_p_glb_data =  p_nm_glb;
        /*initialize QOS call index*/
        p_nm_glb->call_index = ICF_NULL;

#endif
#endif
		}

		icf_nm_free_icf_messg_for_pltfm(p_glb_pdb,
			  						    p_port_pltfm_req);

		icf_nm_free_icf_messg_for_pltfm(p_glb_pdb,
										p_port_pltfm_rsp);
	}
	else
	{
		/* No need to call deallocate mem_for_nm_cntxt */
		*p_p_glb_data = ICF_NULL;
	}

			               
#ifdef ICF_IPSEC_ENABLED
	if(ICF_PORT_PLTFM_SUCCESS==port_pltfm_retval && *p_p_glb_data!=ICF_NULL)
	{

        /* fetch the list of locally supported security algorithms */
        p_nm_glb->p_ue_algo_list = ICF_NULL;

		ICF_STATIC_MEMGET(
            p_glb_pdb,
            ICF_PORT_SIZEOF(icf_ipsec_self_algo_st),
            p_nm_glb->p_ue_algo_list,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val);

        p_nm_glb->p_ue_algo_list->num_enc_algo = ICF_NULL;
        p_nm_glb->p_ue_algo_list->num_auth_algo = ICF_NULL;

		icf_nm_get_icf_messg_for_pltfm(p_glb_pdb,
										  &p_port_pltfm_req,
										  sizeof(icf_port_pltfm_fetch_enc_algo_req_st));

		icf_nm_get_icf_messg_for_pltfm(p_glb_pdb,
										  &p_port_pltfm_rsp,
										  sizeof(icf_port_pltfm_fetch_algo_resp_st));

		/* Get the IP Sec ALgos */
		/** Fetch local Encryption algos synchronously from Wrapper **/
		p_port_pltfm_req->hdr.api_id=ICF_PORT_PLTFM_SA_FETCH_ENC_ALGO_REQ;

		icf_port_pltfm_fetch_enc_algo_req(p_port_pltfm_req,
										 &port_pltfm_retval,
										 p_port_pltfm_rsp);

		p_ipsec_algos=(icf_port_pltfm_fetch_algo_resp_st*)p_port_pltfm_rsp->payload;

        if (ICF_PORT_PLTFM_FAILURE == port_pltfm_retval)
	    {
			ret_val=ICF_FAILURE;
        }
		else
		{
			temp=p_nm_glb->p_ue_algo_list->num_enc_algo=\
				p_ipsec_algos->num_enc_algo;

			while(0 <= --temp)
			p_nm_glb->p_ue_algo_list->encryption_algo[temp]=\
				p_ipsec_algos->encryption_algo[temp];
			/* Get the IP Sec ALgos */
			/** Fetch local Encryption algos synchronously from Wrapper **/
			p_port_pltfm_req->hdr.api_id=ICF_PORT_PLTFM_SA_FETCH_ENC_ALGO_REQ;
		
			icf_port_pltfm_fetch_auth_algo_req(p_port_pltfm_req,
										 &port_pltfm_retval,
										 p_port_pltfm_rsp);
       
			if (ICF_PORT_PLTFM_FAILURE == port_pltfm_retval)
			{
				ret_val=ICF_FAILURE;
		    }
			else
			{
				temp=p_nm_glb->p_ue_algo_list->num_auth_algo=\
					p_ipsec_algos->num_auth_algo;

				while(0 <= --temp)
				p_nm_glb->p_ue_algo_list->integrity_algo[temp]=\
					p_ipsec_algos->integrity_algo[temp];
			}
		}

			icf_nm_free_icf_messg_for_pltfm(p_glb_pdb,
						  				    p_port_pltfm_req);

			icf_nm_free_icf_messg_for_pltfm(p_glb_pdb,
					       				  p_port_pltfm_rsp);
		
}

#endif /* ICF_IPSEC_ENABLED */

if(*p_p_glb_data==ICF_NULL)
{
	icf_nm_deallocate_mem_for_nm_cntxt(p_nm_glb,p_ecode);
	ret_val=ICF_FAILURE;
}


	ICF_FUNCTION_EXIT(p_glb_pdb)
	ICF_RESET_MODULE(p_glb_pdb);

	return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_deinit
 *
 * DESCRIPTION:     It will clean up all the memory allocated by NM and reset 
 *					the	contexts 
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

icf_void_t icf_nm_deinit(
	INOUT   icf_glb_pdb_st       *p_glb_pdb,
        INOUT     icf_void_t         *p_nm_glb_data,
        OUT     icf_error_t          *p_ecode)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_nm_global_st				*p_nm_data=(icf_nm_global_st*)p_nm_glb_data;

	if(ICF_NULL == p_glb_pdb)
  	{
            return;
        }

   	ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_NM);
        ICF_FUNCTION_ENTER(p_glb_pdb)

	p_glb_pdb=p_glb_pdb;
	icf_port_pltfm_deinit();

	icf_nm_deallocate_mem_for_nm_cntxt(p_nm_data,p_ecode);

	ret_val = ICF_SUCCESS;

	ICF_FUNCTION_EXIT(p_glb_pdb)
   	ICF_RESET_MODULE(p_glb_pdb);

}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_usim_info
 *
 * DESCRIPTION:     It will calculate the information from IMSI no. in case of 
 *					USIM application
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

icf_return_t icf_nm_get_usim_info(
			INOUT   icf_glb_pdb_st					*p_glb_pdb,
			IN		icf_port_pltfm_device_params_resp_st	*p_plat_info, 
			INOUT   icf_nm_global_st				**p_p_nm_glb,
			INOUT   icf_error_t					    *p_ecode)
{
	icf_return_t					ret_val  = ICF_SUCCESS;
	icf_uint32_t					errhndlr = ICF_DONOT_RET_ON_FAILURE;
	
	icf_uint8_t		hnd_name[34];
	icf_uint8_t		pri_id[50];
	icf_uint8_t		pub_id[54];
	icf_uint8_t		mcc[8];
	icf_uint8_t		mnc[8];
	icf_uint8_t		temp[4];
	icf_uint32_t	i,j;
#ifdef ICF_NW_ACTIVATE
	icf_int32_t		nofAPNs;
	icf_string_st	*p_apnlist	= ICF_NULL;
#endif

	ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_NM);
	ICF_FUNCTION_ENTER(p_glb_pdb)

	
	/********* Deriving Home Network Domain ************/
	icf_port_strcpy(hnd_name, (icf_uint8_t *) "ims.");
	
	icf_port_strcpy(mcc, (icf_uint8_t *)"mcc");
	for (i=0; i<3; i++)
	{
		temp[i] = (*p_p_nm_glb)->imsi.str[i];
	}
	temp[3] = '\0';
	icf_port_strcat(mcc, temp);
	icf_port_strcat(mcc,(icf_uint8_t *)".");
	
	icf_port_strcpy(mnc, (icf_uint8_t *)"mnc");
	if (ICF_MNC_LEN_TWO == p_plat_info->mncLen)
	{
		temp[0] = '0';
		for (i=1, j=3; i<3; i++)
		{
			temp[i] = (*p_p_nm_glb)->imsi.str[j]; 
			j++;
		}
		temp[3] = '\0';
	}
	else if (ICF_MNC_LEN_THREE == p_plat_info->mncLen)
	{
		for (i=0,j=3; i<3; i++,j++)
			temp[i] = (*p_p_nm_glb)->imsi.str[j]; 
		temp[3] = '\0';
	}
	else
	{
		return ICF_FAILURE;
	}

	icf_port_strcat(mnc, temp);
	icf_port_strcat(mnc,(icf_uint8_t *)".");
	
	icf_port_strcat(hnd_name, mnc);
	icf_port_strcat(hnd_name, mcc);
	icf_port_strcat(hnd_name, (icf_uint8_t *)"3gppnetwork.org");

	(*p_p_nm_glb)->p_acc_info->home_nw_dmn.str_len = (icf_uint16_t) (icf_port_strlen(hnd_name));
	icf_port_strcpy(((*p_p_nm_glb)->p_acc_info->home_nw_dmn.str),hnd_name);


	/********* Deriving Private User Id ************/
	(*p_p_nm_glb)->p_acc_info->nofprids = 1;
	
	icf_port_strcpy(pri_id, (*p_p_nm_glb)->imsi.str);
	icf_port_strcat(pri_id, (icf_uint8_t*) "@");
	icf_port_strcat(pri_id, hnd_name);
	
	ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_private_id_st),
	 					 (*p_p_nm_glb)->p_acc_info->p_private_usr_id, 
						 (icf_uint16_t) errhndlr ,p_ecode, ret_val);

	if(ICF_SUCCESS == ret_val && ICF_NULL != 
		(*p_p_nm_glb)->p_acc_info->p_private_usr_id)
 	{	
		(*p_p_nm_glb)->p_acc_info->p_private_usr_id->type = ICF_PUID_CALCULATED;
		(*p_p_nm_glb)->p_acc_info->p_private_usr_id->value.str_len = 
												(icf_uint16_t) icf_port_strlen(pri_id);
		icf_port_strcpy(((*p_p_nm_glb)->p_acc_info->p_private_usr_id->value.str),
					pri_id);
		

		/********* Deriving Public User Id ************/
		(*p_p_nm_glb)->p_acc_info->nofpbuids = 1;
	
		icf_port_strcpy(pub_id, (icf_uint8_t*)"sip:");
		icf_port_strcat(pub_id, pri_id);

		ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_string_st),
						(*p_p_nm_glb)->p_acc_info->p_public_usr_id, 
						(icf_uint16_t) errhndlr ,p_ecode, ret_val);

		if(ICF_SUCCESS == ret_val)
		{
			(*p_p_nm_glb)->p_acc_info->p_public_usr_id->str_len = (icf_uint16_t)
												icf_port_strlen(pub_id);
			icf_port_strcpy(((*p_p_nm_glb)->p_acc_info->p_public_usr_id->str),
					pub_id);
		}
		else
		{
			ICF_STATIC_MEMFREE(p_glb_pdb,
				(*p_p_nm_glb)->p_acc_info->p_private_usr_id,
				p_ecode,ret_val)

			return ICF_FAILURE;
		}
	}
	else
	{
		return ICF_FAILURE;
	}

#ifdef ICF_NW_ACTIVATE
	/*****  Copying other information ********/
	(*p_p_nm_glb)->simType=p_plat_info->sim_type;
	(*p_p_nm_glb)->imsi=p_plat_info->imsi;
	(*p_p_nm_glb)->p_acc_info->access_type=ICF_GPRS;
	
	(*p_p_nm_glb)->p_acc_info->nofapns=p_plat_info->num_of_apns;
	nofAPNs = (*p_p_nm_glb)->p_acc_info->nofapns;
	/*Memory allocation for APN list*/
	if(0 != temp)
	{
		ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_string_st)*nofAPNs,
			 			 p_apnlist,errhndlr ,p_ecode, ret_val);
		/*copying the list of APN to access_info*/
		while(0 <= --nofAPNs)
			p_apnlist[nofAPNs]= p_plat_info->apn_list[nofAPNs];

		(*p_p_nm_glb)->p_acc_info->p_apn_list=p_apnlist;
	}
#endif

	ICF_FUNCTION_EXIT(p_glb_pdb)
   	ICF_RESET_MODULE(p_glb_pdb);

	return ret_val;
}
/*End of File*/
