
/*------------------------------------------------------------------------------
 *
 * File name        : icf_nm_servies.c
 *
 * Purpose          : This file contains the service function of Network 
 *						Manager(NM) 
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 02-Aug-2005  Irfan Khan	    LLD			    Initial
 * 10-Dec-2005  Rohit Aggarwal  ICF 4.0 CDD     Code changes for handling
 *                                              platform req/resp
 *
 * 06-Jan-2006	Jyoti Narula	SPR 8063/ 9043	changes in icf_nm_auth() for AKA
 * 15-Mar-2008  Abhishek Dhammawat REL8.0       MUL call support for QOS
 * 26-Mar-2008  Shruti Thakral  SPR 18299       Klocworks warning removal
 * 08-Apr-2008  Tarun Gupta     SPR 18355       Merged Windows Porting Changes
 * 08-Apr-2008  Amit Sharma     SPR 18387       Freeing Memory allocated for
 *                                              p_port_pltfm_req and
 *                                              p_port_pltfm_rsp in faliure leg also. 
 *
 * Copyright (c) 2008, Aricent.
 *----------------------------------------------------------------------------*/
#include "icf_map_extif_feature_flags.h"
#include "icf_port_prototypes.h" 
#include "icf_macro.h"
#include "icf_internal_api_id.h"
#include "icf_nm_prototypes.h"
#include "icf_nm_macro.h"
#include "icf_cfg_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_port_pltfm_defs.h"

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_acs_info
 *
 * DESCRIPTION:     It will return the network access information which is stored
 *				   locally in the NM. If required information is not present
 *				   then return failure
 *					
 * RETURN:			ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_get_acs_info(INOUT icf_nm_acs_info_st	*p_na_info,
									   INOUT icf_internal_msg_hdr_st *p_msg_hdr)
{
	icf_return_t		ret_val = ICF_SUCCESS;
	
	ICF_FUNCTION_ENTER(p_na_info->p_nm_ptr->p_icf_glb_ptr)  

	if(ICF_NULL == p_na_info->p_nm_ptr->p_acc_info)
	{
		return ICF_FAILURE;
	}
	
	p_na_info->p_ainfo = p_na_info->p_nm_ptr->p_acc_info;
	
	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_COMPLETED;
	
	ICF_FUNCTION_EXIT(p_na_info->p_nm_ptr->p_icf_glb_ptr)

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_inttostr
 *
 * DESCRIPTION:     Converts integer to string
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

icf_int32_t icf_nm_inttostr(icf_int32_t p_int,icf_uint8_t *p_string)
{
	icf_uint32_t count=3; /*As the length of mnc and mcc would be max 3*/
	while(0 != p_int%10)
	{
		p_string[--count] = (icf_uint8_t)(p_int%10);
		p_int /= 10;
	}
	return (3-count);

}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_pacs_info
 *
 * DESCRIPTION:     It will provide the location information like mobile country
 *					code(mcc), mobile network code(mnc),location area code and
 *					cell id. 
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_get_pacs_info(icf_nm_pnwacs_info_st	*p_info,
									icf_internal_msg_hdr_st *p_msg_hdr)
 {
	icf_return_t					ret_val = ICF_SUCCESS;
    icf_msg_st						*p_port_pltfm_req=ICF_NULL,
									*p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t		port_pltfm_ret_val;
	icf_port_pltfm_access_network_resp_st	*p_access_nw_resp;
	

	ICF_FUNCTION_ENTER(p_info->p_nm_ptr->p_icf_glb_ptr)

	icf_nm_get_icf_messg_for_pltfm(p_info->p_nm_ptr->p_icf_glb_ptr,
										  &p_port_pltfm_req,
										  sizeof(icf_port_pltfm_access_network_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_info->p_nm_ptr->p_icf_glb_ptr,
										  &p_port_pltfm_rsp,
										  sizeof(icf_port_pltfm_access_network_resp_st));

	p_access_nw_resp=(icf_port_pltfm_access_network_resp_st *) p_port_pltfm_rsp->payload;

	
	/*Call to platform wrapper*/
    if ((ICF_NULL == p_port_pltfm_req) || (ICF_NULL == p_port_pltfm_rsp))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_GET_ACCESS_NW_INFO_REQ;
        icf_port_pltfm_get_access_nw_info(p_port_pltfm_req,
                &port_pltfm_ret_val,
                p_port_pltfm_rsp);

        /*Filling up the response to the user's structure*/
        if(ICF_PORT_PLTFM_FAILURE!=port_pltfm_ret_val)
        {
            p_info->access_type	=	p_access_nw_resp->nw_type;

            icf_nm_inttostr(p_access_nw_resp->mcc,p_info->p_mcc);

            p_info->mnc.len =(icf_uint8_t) icf_nm_inttostr(p_access_nw_resp->mnc,
                    p_info->mnc.p_mnc);
            p_info->lac=p_access_nw_resp->lac;
            /*ICF_RELEASE4_TBD */
            /* What to do with Cell ID here */
            p_info->umts_c		=	0;	/*for phase 1*/
            /*ICF_RELEASE4_TBD_END */
        }
        else
            ret_val=ICF_FAILURE;

        p_msg_hdr->status = ICF_STATUS_NM_REQUEST_COMPLETED;

        icf_nm_free_icf_messg_for_pltfm(p_info->p_nm_ptr->p_icf_glb_ptr,
                p_port_pltfm_req);

        icf_nm_free_icf_messg_for_pltfm(p_info->p_nm_ptr->p_icf_glb_ptr,
                p_port_pltfm_rsp);
    }

	ICF_FUNCTION_EXIT(p_info->p_nm_ptr->p_icf_glb_ptr)

	return ret_val;
 }

#ifdef ICF_NW_ACTIVATE
#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        		icf_nm_populate_default_qos
 *
 * DESCRIPTION:    		Populate Default Qos for POC
 *
 *					
 * RETURN:				ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/

icf_return_t icf_nm_populate_default_qos (INOUT icf_nm_qos_st *p_qos)
{
	/* This is the default QOS values for a Signalling PDP Context */
	p_qos->traffic_class=ICF_QOS_INTERACTIVE_TRAFFIC_CLASS;
	p_qos->delivery_order =0;
	p_qos->max_sdu_size=1500; 
	p_qos->residual_ber =0x07; /* 10pow-5 is coded as binary 0111 */ \
	p_qos->sdu_error_ratio =0x04; /* 10pow-4 is coded as binary 0100 */ \
	p_qos->transfer_delay =0;
	p_qos->guaranteed_bit_rate_ul=0;
	p_qos->guaranteed_bit_rate_dl=0;
	p_qos->traffic_handling_priority=1;
	p_qos->signaling_indication=1;
	p_qos->source_statistics_descriptor=0;
	p_qos->maximum_bit_rate_ul=8; /* 8 Kbps */
	p_qos->maximum_bit_rate_dl=8; /* 8 Kbps */

	return ICF_SUCCESS;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_activate_sec
 *
 * DESCRIPTION:     This will activate the secondary PDP context of a given 
 *					primary PDP context. Qos and TFT parameters may be requested
 *					for secondary PDP context.
 *					1)p_act->prim_id = id of primary PDP context from which 
 *					secondary has to derive, for this function.
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t  icf_nm_activate_sec(icf_nm_global_st	*p_glb_nm, 
										icf_nm_pdp_context_st *p_pdp_ctxt)
{

	icf_msg_st							 *p_port_pltfm_req=ICF_NULL,\
										 *p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t			 port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;	
	icf_port_pltfm_pdp_def_req_st		 *p_pdp_def_req;
	icf_port_pltfm_pdp_def_resp_st		 *p_pdp_def_rsp;
	icf_return_t						 ret_val= ICF_SUCCESS;
	icf_nm_nw_info_st					 *p_info;
	
	ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr) 
	p_info =p_glb_nm->p_n_info_list;

	/* Check to see if no Primary PDP Context is present */
	
	if(ICF_NM_INVALID_PRIMARY_PDP_CTXT_ID==

		p_info->pdp_cntxt_info.prim_pdp_id)
	{
		return ICF_FAILURE;
	}

	icf_nm_get_icf_messg_for_pltfm(p_glb_nm->p_icf_glb_ptr,
										  &p_port_pltfm_req,
										  sizeof(icf_port_pltfm_pdp_def_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_glb_nm->p_icf_glb_ptr,
										  &p_port_pltfm_rsp,
										  sizeof(icf_port_pltfm_pdp_def_resp_st));

	p_pdp_def_req=(icf_port_pltfm_pdp_def_req_st *) p_port_pltfm_req->payload;
	p_pdp_def_rsp=(icf_port_pltfm_pdp_def_resp_st *) p_port_pltfm_rsp->payload;
	
	p_pdp_def_req->pdp_context_type=ICF_NM_SECONDARY_PDP_CNTXT;
	p_pdp_def_req->pdp_context_info.secondary.primary_id=p_pdp_ctxt->primay_pdp_id;
	p_pdp_def_req->pdp_context_info.secondary.context_id=p_pdp_ctxt->cntxt_id;
/*
	p_pdp_def_req->pdp_context_info.secondary.context_id=p_info->pdp_cntxt_info.sec_pdp_id_counter;
	p_info->pdp_cntxt_info.sec_pdp_id_counter=(p_info->pdp_cntxt_info.sec_pdp_id_counter+1)%255;
*/
	/*p_pdp_def_req->pdp_context_info.secondary.tft=p_pdp_ctxt->tft;*/

	icf_port_memcpy(
                    (icf_void_t*)(&p_pdp_def_req->pdp_context_info.secondary.tft),
                    (icf_void_t*)(&p_pdp_ctxt->tft),
                    (icf_uint32_t)sizeof(icf_nm_tft_st));

	p_pdp_def_req->qos_profile_present=ICF_TRUE;

	/*p_pdp_def_req->qos_profile=p_pdp_ctxt->qos;*/
	
	icf_port_memcpy(
                    (icf_void_t*)(&p_pdp_def_req->qos_profile),
                    (icf_void_t*)(&p_pdp_ctxt->qos),
                    (icf_uint32_t)sizeof(icf_port_pltfm_qos_profile_st));

	p_pdp_def_req->min_qos_present=ICF_TRUE;

	/*p_pdp_def_req->min_qos_profile=p_pdp_ctxt->qos;*/
	
	icf_port_memcpy(
                    (icf_void_t*)(&p_pdp_def_req->min_qos_profile),
                    (icf_void_t*)(&p_pdp_ctxt->qos),
                    (icf_uint32_t)sizeof(icf_port_pltfm_qos_profile_st));

	p_pdp_def_req->dcomp_present=ICF_TRUE;
	p_pdp_def_req->dcomp=ICF_NM_DCOMP_DEFAULT;
	p_pdp_def_req->hcomp_present=ICF_TRUE;
	p_pdp_def_req->hcomp=ICF_NM_HCOMP_DEFAULT;

	/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_SEC_PDP_ACT_REQ;

	icf_port_pltfm_pdp_activate_req(p_port_pltfm_req,
									&port_pltfm_retval,
									p_port_pltfm_rsp);

	if(ICF_PORT_PLTFM_FAILURE==port_pltfm_retval)
		ret_val=ICF_FAILURE;

	icf_nm_free_icf_messg_for_pltfm(p_glb_nm->p_icf_glb_ptr,
										  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_glb_nm->p_icf_glb_ptr,
										  p_port_pltfm_rsp);
	
	ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_activate_multiple_sec_pdps
 *
 * DESCRIPTION	:     It will activate Multiple Secondary PDP contexts 
 *										
 * RETURN:			  ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
 
icf_return_t  icf_nm_activate_multiple_sec_pdps(
	INOUT		icf_nm_multiple_sec_pdp_activate_st		*p_mult_sec_act,
	INOUT	icf_internal_msg_hdr_st 				*p_msg_hdr)
{
	icf_uint8_t						counter,idx = ICF_NULL;
	icf_uint16_t					ts = ICF_NULL;
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_nm_prim_sec_pdp_map_st		*p_pdp_ctxt_info=
								&(p_mult_sec_act->p_nm_ptr->p_n_info_list->pdp_cntxt_info);
	
	icf_callid_sec_pdp_ctxts_map_st *p_callid_sec_pdp_ctxts_map;

	ret_val = icf_nm_get_callid_index(p_pdp_ctxt_info,p_mult_sec_act,&idx);

	if(ICF_FAILURE == ret_val)
	{
		ret_val = icf_nm_get_vacant_index(p_pdp_ctxt_info,&idx);
	}

	p_callid_sec_pdp_ctxts_map =
			&(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[p_pdp_ctxt_info->number_of_calls]);

    ICF_FUNCTION_ENTER(p_mult_sec_act->p_nm_ptr->p_icf_glb_ptr)

	p_pdp_ctxt_info->prim_pdp_id = ICF_NM_DEFAULT_PRIMARY_PDP_CTXT_ID;

	p_callid_sec_pdp_ctxts_map->call_id=p_mult_sec_act->call_id;
	
	p_callid_sec_pdp_ctxts_map->sec_pdps_count=p_mult_sec_act->no_of_cntxts;

	for(counter=0;counter<p_mult_sec_act->no_of_cntxts;counter++)
	{
		ts = icf_port_random((icf_uint16_t)ICF_QOS_MIN_SEC_PDP_CTX_ID, 
							(icf_uint16_t)ICF_QOS_MAX_SEC_PDP_CTX_ID);
		
		/* Since, the above function generates same random number,
		   if called within short span, We increment ts by counter */
		
		/* sleep(10); */

		p_mult_sec_act->p_pdp_contexts[counter].cntxt_id = ts + counter;

		p_mult_sec_act->p_pdp_contexts[counter].primay_pdp_id = ICF_NM_DEFAULT_PRIMARY_PDP_CTXT_ID;
	
		icf_nm_activate_sec(p_mult_sec_act->p_nm_ptr,
								&(p_mult_sec_act->p_pdp_contexts[counter]));

		p_callid_sec_pdp_ctxts_map->sec_pdp_ids[counter] = ts + counter;

		p_callid_sec_pdp_ctxts_map->sec_pdp_states[counter]=ICF_NM_SEC_PDP_STATE_PENDING_ACTIVATION;
	}

	p_pdp_ctxt_info->number_of_calls++;

	p_pdp_ctxt_info->sec_pdp_id_counter = p_pdp_ctxt_info->sec_pdp_id_counter + counter;


	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_PENDING;

	ICF_FUNCTION_EXIT(p_mult_sec_act->p_nm_ptr->p_icf_glb_ptr)

	return ICF_SUCCESS;
	
}

/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_deactivate_multiple_sec_pdps
 *
 * DESCRIPTION	:     It will deactivate All Secondary PDP contexts associated 
 *					  with a particular Call ID
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_deactivate_multiple_sec_pdps( 
							IN icf_nm_multiple_sec_pdp_deactivate_st *p_mult_sec_deact,
							INOUT icf_internal_msg_hdr_st *p_msg_hdr)
{

	icf_nm_deactivate_st       		pdp_deact;
	icf_uint16_t						counter;
	icf_return_t		       			ret_val = ICF_SUCCESS;
	icf_callid_sec_pdp_ctxts_map_st 	* p_callid_sec_pdp_ctxts_map=ICF_NULL;
	icf_nm_prim_sec_pdp_map_st		*p_pdp_ctxt_info=
								&(p_mult_sec_deact->p_nm_ptr->p_n_info_list->pdp_cntxt_info);

	pdp_deact.p_nm_ptr=p_mult_sec_deact->p_nm_ptr;
	pdp_deact.tree=ICF_FALSE;
	pdp_deact.p_caller_data=ICF_NULL;

	ICF_FUNCTION_ENTER(p_mult_sec_deact->p_nm_ptr->p_icf_glb_ptr)

	for(counter=0;counter<p_pdp_ctxt_info->number_of_calls;counter++)
	{
		if(p_mult_sec_deact->call_id==p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[counter].call_id)
		{
			p_callid_sec_pdp_ctxts_map=&(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[counter]);
			break;
		}
	}

	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_PENDING;

	if(ICF_NULL!=p_callid_sec_pdp_ctxts_map)
	{
		for(counter=0;counter<p_callid_sec_pdp_ctxts_map->sec_pdps_count;counter++)
		{
			if((ICF_NM_SEC_PDP_STATE_PENDING_ACTIVATION==\
				p_callid_sec_pdp_ctxts_map->sec_pdp_states[counter]) || 
				(ICF_NM_SEC_PDP_STATE_ACTIVE==\
				p_callid_sec_pdp_ctxts_map->sec_pdp_states[counter]))
			{
				pdp_deact.cid=p_callid_sec_pdp_ctxts_map->sec_pdp_ids[counter];
				p_callid_sec_pdp_ctxts_map->sec_pdp_states[counter]=\
				ICF_NM_SEC_PDP_STATE_INACTIVE;
				icf_nm_deactivate(&pdp_deact,p_msg_hdr);
			}
		}

		icf_port_memset(p_callid_sec_pdp_ctxts_map,0,sizeof(icf_callid_sec_pdp_ctxts_map_st));

		p_callid_sec_pdp_ctxts_map->call_id = ICF_INVALID_CALL_ID;

		p_msg_hdr->status = ICF_STATUS_NM_REQUEST_COMPLETED;
	}
	
	ICF_FUNCTION_EXIT(p_mult_sec_deact->p_nm_ptr->p_icf_glb_ptr)

	return ret_val;
}

/*ICF_RELEASE4_TBD */

/* Move this to a utility file if possible */

/******************************************************************************
 *
 * FUNCTION:        icf_nm_check_all_sec_deactivated
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  check if all sec pdps for a call id are deactivated 
 *					or not 
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_check_all_sec_deactivated(
         INOUT   icf_nm_global_st	*p_glb_nm,
         IN	     icf_call_id_t		callid)
     
{
	icf_uint16_t						counter;
	icf_return_t		       			ret_val = ICF_SUCCESS;
	icf_callid_sec_pdp_ctxts_map_st 	* p_callid_sec_pdp_ctxts_map=ICF_NULL;
	icf_nm_prim_sec_pdp_map_st	  *p_pdp_ctxt_info=
								&(p_glb_nm->p_n_info_list->pdp_cntxt_info);

	ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)

	for(counter=0;counter<p_pdp_ctxt_info->number_of_calls;counter++)
	{
		if(callid==p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[counter].call_id)
		{
			p_callid_sec_pdp_ctxts_map=&(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[counter]);
			
		}
	}

	if(ICF_NULL!=p_callid_sec_pdp_ctxts_map)
	{
		for(counter=0;counter<p_callid_sec_pdp_ctxts_map->sec_pdps_count;counter++)
		{
			if(ICF_NM_SEC_PDP_STATE_ACTIVE==p_callid_sec_pdp_ctxts_map->sec_pdp_states[counter])
			{
				ret_val=ICF_FAILURE;
				break;
			}
		
		}	
	}
	else
	{
		ret_val = ICF_FAILURE;
	}	

	ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_nm_check_all_sec_activated
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  check if all sec pdps for a call id are activated 
 *					or not 
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_check_all_sec_activated(
         INOUT   icf_nm_global_st	*p_glb_nm,
         IN	     icf_call_id_t		callid)
         
{
	icf_uint16_t						counter;
	icf_return_t		       			ret_val = ICF_SUCCESS;
	icf_callid_sec_pdp_ctxts_map_st 	* p_callid_sec_pdp_ctxts_map=ICF_NULL;
	icf_nm_prim_sec_pdp_map_st	  *p_pdp_ctxt_info=
								&(p_glb_nm->p_n_info_list->pdp_cntxt_info);

	ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)

	for(counter=0;counter<p_pdp_ctxt_info->number_of_calls;counter++)
	{
		if(callid==p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[counter].call_id)
		{
			p_callid_sec_pdp_ctxts_map=&(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[counter]);
			
		}
	}

	if(ICF_NULL!=p_callid_sec_pdp_ctxts_map)
	{
		for(counter=0;counter<p_callid_sec_pdp_ctxts_map->sec_pdps_count;counter++)
		{
			if( (ICF_NM_SEC_PDP_STATE_INACTIVE==p_callid_sec_pdp_ctxts_map->sec_pdp_states[counter]) ||
			(ICF_NM_SEC_PDP_STATE_PENDING_ACTIVATION==p_callid_sec_pdp_ctxts_map->sec_pdp_states[counter]) )
			{
				ret_val=ICF_FAILURE;
				break;
			}
		
		}	
	}
	else
	{
		ret_val = ICF_FAILURE;
	}	


	ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_sec_deactivate_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the deactivate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_sec_deactivate_resp(
		    INOUT icf_nm_pdb_st		*p_nm_pdb,
			IN    icf_call_id_t		callid,
			IN    icf_result_t		result)
		
{
   icf_return_t                         ret_val = ICF_SUCCESS;
   icf_nm_global_st						*p_glb_nm=p_nm_pdb->p_nm_glb_data;
   icf_internal_msg_st                  *p_nm_internal_msg = p_nm_pdb->p_internal_msg;
   icf_nm_deactivate_rsp_st				msg_payload;
   
    
   ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)
   /* Fill the header of the message to be send */
    p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MMI_SEC_PDP_DEACT_RESP ;
	p_nm_internal_msg->msg_hdr.payload_length = 0;
	p_nm_internal_msg->msg_hdr.p_glb_pdb = p_glb_nm->p_icf_glb_ptr ;
    /* Filling the Call Id in the payload */
	(icf_nm_deactivate_rsp_st *)p_nm_internal_msg->p_msg_data = &msg_payload;
	msg_payload.result = result;

	/* Set the call Context for this call id in global pdb */

	ret_val=icf_nm_set_call_ctxt_in_glb_db(p_glb_nm,callid);

	if(ICF_SUCCESS==ret_val)
		ret_val = icf_mmi_process_mesg(p_nm_internal_msg);

          
    ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

    return ret_val; 
}

/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_sec_deactivate_ind
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the unsolicited deactivate indication with cause
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_sec_deactivate_ind(
        INOUT icf_nm_pdb_st	*p_nm_pdb,
        IN 	     icf_call_id_t   callid,
		IN    icf_result_t		result)
       
{
   icf_return_t                         ret_val = ICF_SUCCESS;
   icf_nm_global_st						*p_glb_nm=p_nm_pdb->p_nm_glb_data;
   icf_internal_msg_st                  *p_nm_internal_msg = p_nm_pdb->p_internal_msg;
    icf_nm_deactivate_ind_st				msg_payload;
   
    
   ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)
   /* Fill the header of the message to be send */
    p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MMI_SEC_PDP_DEACT_IND ;
	p_nm_internal_msg->msg_hdr.payload_length = 0;
	p_nm_internal_msg->msg_hdr.p_glb_pdb = p_glb_nm->p_icf_glb_ptr ;
    /* Filling the Call Id in the payload */
	(icf_nm_deactivate_ind_st *)p_nm_internal_msg->p_msg_data = &msg_payload;
	msg_payload.result = result;

	/* Set the call Context for this call id in global pdb */

	ret_val=icf_nm_set_call_ctxt_in_glb_db(p_glb_nm,callid);

	if(ICF_SUCCESS==ret_val)
	    ret_val = icf_mmi_process_mesg(p_nm_internal_msg);
         
   ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

    return ret_val; 
}

/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_sec_activate_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the activate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_sec_activate_resp(
        INOUT icf_nm_pdb_st	*p_nm_pdb,
        IN	     icf_call_id_t		callid,
		IN    icf_result_t		result)
     
{

   icf_return_t                         ret_val = ICF_SUCCESS;
   icf_nm_global_st					   *p_glb_nm=p_nm_pdb->p_nm_glb_data;
   icf_internal_msg_st                  *p_nm_internal_msg = p_nm_pdb->p_internal_msg;
   icf_nm_activate_rsp_st				msg_payload;
   
    
   ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)
   /* Fill the header of the message to be send */
    p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MMI_SEC_PDP_ACT_RESP ;
	p_nm_internal_msg->msg_hdr.payload_length = 0;
	p_nm_internal_msg->msg_hdr.p_glb_pdb = p_glb_nm->p_icf_glb_ptr ;
    /* Filling the Call Id in the payload */
	(icf_nm_activate_rsp_st *)p_nm_internal_msg->p_msg_data = &msg_payload;
	msg_payload.result = result;

	ret_val=icf_nm_set_call_ctxt_in_glb_db(p_glb_nm,callid);
	if(ICF_SUCCESS==ret_val)
	    ret_val = icf_mmi_process_mesg(p_nm_internal_msg);
          
    ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

    return ret_val; 

}

/******************************************************************************
 *
 * FUNCTION:            icf_nm_send_sec_modify_ind
 *
 * DESCRIPTION:      This function is invoked when ever any function needs to 
 *                  		    send the unsolicited modify indication with cause
 *                   
 * RETURNS:              ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		     case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_send_sec_modify_ind(
        INOUT icf_nm_pdb_st	*p_nm_pdb,
        IN	     icf_call_id_t		callid,
		IN    icf_result_t		result)
        
{
   icf_return_t                         ret_val = ICF_SUCCESS;
   icf_nm_global_st					   *p_glb_nm=p_nm_pdb->p_nm_glb_data;
   icf_internal_msg_st                  *p_nm_internal_msg = p_nm_pdb->p_internal_msg;
   icf_nm_modify_ind_st				    msg_payload;
   
    
   ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)

   /* Fill the header of the message to be send */
    p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MMI_SEC_PDP_MODIFY_IND ;
	p_nm_internal_msg->msg_hdr.payload_length = 0;
	p_nm_internal_msg->msg_hdr.p_glb_pdb = p_glb_nm->p_icf_glb_ptr ;
    /* Filling the Call Id in the payload */
	(icf_nm_modify_ind_st *)p_nm_internal_msg->p_msg_data = &msg_payload;
	msg_payload.result = result;

    ret_val=icf_nm_set_call_ctxt_in_glb_db(p_glb_nm,callid);

	if(ICF_SUCCESS==ret_val)
	    ret_val = icf_mmi_process_mesg(p_nm_internal_msg);

   ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

    return ret_val; 

}

/******************************************************************************
 *
 * FUNCTION:         icf_nm_get_callid_index
 *
 * DESCRIPTION:      
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_get_callid_index(icf_nm_prim_sec_pdp_map_st				*p_pdp_ctxt_info,
									 icf_nm_multiple_sec_pdp_activate_st	*p_mult_sec_act,
									 icf_uint8_t							*idx)
{
	icf_uint8_t					i = 0;
	icf_return_t				ret_val = ICF_FAILURE;

	for(i=0;i<ICF_MAX_NUM_CALL;i++)
	{
		if(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[i].call_id != ICF_INVALID_CALL_ID)
		{
			if(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[i].call_id == p_mult_sec_act->call_id)
			{
				*idx = i;
				ret_val = ICF_SUCCESS;
				break;
			}
		}
	}

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:         icf_nm_get_vacant_index
 *
 * DESCRIPTION:      
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_get_vacant_index(icf_nm_prim_sec_pdp_map_st				*p_pdp_ctxt_info,
									 icf_uint8_t							*idx)
{
	icf_uint8_t					i = 0;
	icf_return_t				ret_val = ICF_FAILURE;

	for(i=0;i<ICF_MAX_NUM_CALL;i++)
	{
		if(ICF_INVALID_CALL_ID == p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[i].call_id)
		{
			*idx = i;
			ret_val = ICF_SUCCESS;
			break;
		}

	}
	return ret_val;
}

#endif /* #ifdef ICF_QOS_SUPPORT */

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_nw_info
 *
 * DESCRIPTION:     It will return the pointer to icf_nm_nw_info structure 
 *					which is stored localy in the NM.NM gets this information in
 *					NM initialization and maitains it.
 *					
 * RETURN:			ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_get_nw_info(INOUT icf_nm_nw_info_req_st *p_nw_info,
									  INOUT icf_internal_msg_hdr_st *p_msg_hdr)
{
	icf_return_t		ret_val = ICF_SUCCESS;
	
	ICF_FUNCTION_ENTER(p_nw_info->p_nm_ptr->p_icf_glb_ptr)

	if(ICF_NM_PRIMARY_PDP_CTXT_DEACTIVATED_ID != 
				p_nw_info->p_nm_ptr->p_n_info_list->pdp_cntxt_info.prim_pdp_id)
	{
		if(p_nw_info->p_nm_ptr->p_n_info_list && (icf_port_strcmp(p_nw_info->
                        p_nm_ptr->p_n_info_list->nm_access_info.str,
                        p_nw_info->apn.str) ==0) )
		{
				/*Assign the pointer to list of primary PDP context */
				p_nw_info->p_ninfo = p_nw_info->p_nm_ptr->p_n_info_list;
		}
		else
		ret_val=ICF_FAILURE;
	}
	else
	ret_val=ICF_FAILURE;

	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_COMPLETED;

    ICF_FUNCTION_EXIT(p_nw_info->p_nm_ptr->p_icf_glb_ptr)

	return ret_val;
}

 /*******************************************************************************
 *
 * FUNCTION		:    icf_nm_activate_primary_pdp
 *
 * DESCRIPTION	:     This will send the activate PDP Req to nw for a given APN, Qos and
 *					TFT parameters.
 *					1)p_act->prim_id =0, for this function.
 *					2)In phase-1 only one primary PDP context will be allowed,
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

 icf_return_t icf_nm_activate_primary_pdp(icf_nm_pdp_st	 *p_act,
												icf_uint16_t	     prim_pdp_id,
												icf_port_pltfm_pdp_def_resp_st *p_pdp_def_rsp )
{
	icf_msg_st							 *p_port_pltfm_req=ICF_NULL,\
										 *p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t			 port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;	
	icf_port_pltfm_pdp_def_req_st		 *p_pdp_def_req=ICF_NULL;
	icf_return_t						 ret_val= ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_act->p_nm_ptr->p_icf_glb_ptr)

	icf_nm_get_icf_messg_for_pltfm(p_act->p_nm_ptr->p_icf_glb_ptr,
										  &p_port_pltfm_req,
										  sizeof(icf_port_pltfm_pdp_def_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_act->p_nm_ptr->p_icf_glb_ptr,
										  &p_port_pltfm_rsp,
										  sizeof(icf_port_pltfm_pdp_def_resp_st));

	p_pdp_def_req=(icf_port_pltfm_pdp_def_req_st *) p_port_pltfm_req->payload;

	p_pdp_def_req->pdp_context_type=ICF_NM_PRIMARY_PDP_CNTXT;
	p_pdp_def_req->pdp_context_info.primary.context_id=prim_pdp_id;

	/*ICF_RELEASE4_TBD*/
	/* This must be made runtime */
	p_pdp_def_req->pdp_context_info.primary.pdp_type=ICF_NM_PDP_TYPE_DEFAULT;
	/*ICF_RELEASE4_TBD_END*/

	p_pdp_def_req->pdp_context_info.primary.apn=p_act->apn;

	/* Let PDP Address be all 0s as we are using dynamic IP
	   Address allocation as of now */

	p_pdp_def_req->dcomp_present=ICF_TRUE;
	p_pdp_def_req->dcomp=ICF_NM_DCOMP_DEFAULT;
	p_pdp_def_req->hcomp_present=ICF_TRUE;
	p_pdp_def_req->hcomp=ICF_NM_HCOMP_DEFAULT;

#ifdef ICF_QOS_SUPPORT
	/* Fill the default Qos to be used for Signalling */
	icf_nm_populate_default_qos(&(p_act->qos));
#endif

	p_pdp_def_req->qos_profile_present=ICF_TRUE;
	p_pdp_def_req->qos_profile=p_act->qos;
	p_pdp_def_req->min_qos_present=ICF_TRUE;
	p_pdp_def_req->min_qos_profile=p_act->qos;
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_act->p_nm_ptr->p_icf_glb_ptr,ICF_TRUE)
	p_pdp_def_req->num_pdp_var_params=2;
	p_pdp_def_req->pdp_var_params[0].type=ICF_NM_PCSCF_ADDR;
	p_pdp_def_req->pdp_var_params[1].type=ICF_NM_IM_CN_SIG_FLAG;
ICF_CHECK_IMS_END
#endif
	/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_PRIM_PDP_ACT_REQ;

	icf_port_pltfm_pdp_activate_req(p_port_pltfm_req,
									 &port_pltfm_retval,
									 p_port_pltfm_rsp);
	
	if(ICF_PORT_PLTFM_FAILURE==port_pltfm_retval)
		ret_val=ICF_FAILURE;

	if(ICF_PORT_PLTFM_SUCCESS==port_pltfm_retval)
		icf_port_memcpy(p_pdp_def_rsp,
						p_port_pltfm_rsp->payload,
					    sizeof(icf_port_pltfm_pdp_def_resp_st));	
					  
	icf_nm_free_icf_messg_for_pltfm(p_act->p_nm_ptr->p_icf_glb_ptr,
										  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_act->p_nm_ptr->p_icf_glb_ptr,
										  p_port_pltfm_rsp);

	ICF_FUNCTION_EXIT(p_act->p_nm_ptr->p_icf_glb_ptr)

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION		:    icf_nm_activate
 *
 * DESCRIPTION	:   This will activate the PDP context for a given APN, Qos and
 *					TFT parameters.
 *					1)p_act->prim_id =0, for this function.
 *					2)In phase-1 only one primary PDP context will be allowed,
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t  icf_nm_activate(icf_nm_pdp_st	*p_act, 
									icf_internal_msg_hdr_st *p_msg_hdr)
{

	icf_return_t					 ret_val= ICF_SUCCESS;
	icf_nm_nw_info_st			 *p_info;
	icf_port_pltfm_pdp_def_resp_st	 pdp_def_rsp;

	icf_port_memset(&pdp_def_rsp,0,sizeof(icf_pdp_def_resp_st));
	p_info = p_act->p_nm_ptr->p_n_info_list;
	
	ICF_FUNCTION_ENTER(p_act->p_nm_ptr->p_icf_glb_ptr);
	
	/* Check if the Primary PDP Context is active 
	   If  Primary PDP Context is already active we cannot activate 
  	   one more as of now */
  	      
	if(ICF_NM_INVALID_PRIMARY_PDP_CTXT_ID != 
	    p_act->p_nm_ptr->p_n_info_list->pdp_cntxt_info.prim_pdp_id)
	{
		return ICF_FAILURE;
	}
	
	/*assigning values to parameters to call the wrapper*/
	ret_val=icf_nm_activate_primary_pdp(p_act,
 				   				    	ICF_NM_DEFAULT_PRIMARY_PDP_CTXT_ID,
										&pdp_def_rsp);
	if(ICF_SUCCESS==ret_val)
	{
		/*storing the information into NM's structure*/
		p_info->access_type = 0; /*GPRS */
		p_info->nm_access_info= p_act->apn;
		p_info->pdp_cntxt_info.prim_pdp_id= ICF_NM_DEFAULT_PRIMARY_PDP_CTXT_ID;
		p_info->pdp_cntxt_info.sec_pdp_id_counter=p_info->pdp_cntxt_info.prim_pdp_id+1;
		p_info->pdp_cntxt_info.number_of_calls=0;
                p_info->outbound_proxies.num_of_elements = 0;
#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
                p_info->dns_addr.num_of_elements = 0;
                p_info->dhcp_addr.num_of_elements = 0;
#endif

		/* Common Function Used */
		icf_cmn_convert_ip_string_to_transport_addr(
			pdp_def_rsp.pdp_addr.str,
			&(p_info->self_ip));

		if(0 < pdp_def_rsp.pcscf_addr.str_len)
                {
                    icf_cmn_convert_ip_string_to_transport_addr (
			pdp_def_rsp.pcscf_addr.str,
			&(p_info->outbound_proxies.addr[0]));
                        p_info->outbound_proxies.num_of_elements = 1;
                }
#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
		if(0 < pdp_def_rsp.dns_addr.str_len)
                {
                    icf_cmn_convert_ip_string_to_transport_addr(
			pdp_def_rsp.dns_addr.str,
			&(p_info->dns_addr.addr[0]));
                    p_info->dns_addr.num_of_elements = 1;
                }
		if(0 < pdp_def_rsp.dhcp_addr.str_len)
                {
                    icf_cmn_convert_ip_string_to_transport_addr(
			pdp_def_rsp.dhcp_addr.str,
			&(p_info->dhcp_addr.addr[0]));
                    p_info->dhcp_addr.num_of_elements = 1;
                }
#endif
	}

    p_msg_hdr->status = ICF_STATUS_NM_REQUEST_COMPLETED;
	
	ICF_FUNCTION_EXIT(p_act->p_nm_ptr->p_icf_glb_ptr);
	
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_deactivate
 *
 * DESCRIPTION:     It will deactivate the given PDP context 
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_deactivate(INOUT icf_nm_deactivate_st	*p_deact,
									 icf_internal_msg_hdr_st *p_msg_hdr)
{

	icf_return_t					 ret_val = ICF_SUCCESS;
	icf_msg_st						 *p_port_pltfm_req=ICF_NULL,\
									 *p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_pdp_deact_req_st	 *p_pdp_deact_req;
	icf_port_pltfm_ret_val_t		 port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;
	icf_nm_nw_info_st			 *p_info;

	ICF_FUNCTION_ENTER(p_deact->p_nm_ptr->p_icf_glb_ptr)

	p_info=p_deact->p_nm_ptr->p_n_info_list;

	icf_nm_get_icf_messg_for_pltfm(p_deact->p_nm_ptr->p_icf_glb_ptr,
									  &p_port_pltfm_req,
									  sizeof(icf_port_pltfm_pdp_deact_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_deact->p_nm_ptr->p_icf_glb_ptr,
									  &p_port_pltfm_rsp,
									  sizeof(icf_port_pltfm_pdp_deact_rsp_st));

	p_pdp_deact_req=(icf_port_pltfm_pdp_deact_req_st *) p_port_pltfm_req->payload;
		
	if(ICF_NM_INVALID_PRIMARY_PDP_CTXT_ID!=
		p_info->pdp_cntxt_info.prim_pdp_id)
	{
		/*Call to platform wrapper*/
		p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_PDP_DEACT_REQ;
		p_pdp_deact_req->context_id=p_deact->cid;
		icf_port_pltfm_pdp_deactivate_req(p_port_pltfm_req,
										&port_pltfm_retval,
										p_port_pltfm_rsp);

		if(ICF_PORT_PLTFM_FAILURE==port_pltfm_retval)
			ret_val=ICF_FAILURE;
		else
		{
			if(p_info->pdp_cntxt_info.prim_pdp_id==p_deact->cid)
			/* This fix is incorrect ..this call should be made asynchronous */
			p_info->pdp_cntxt_info.prim_pdp_id=ICF_NM_INVALID_PRIMARY_PDP_CTXT_ID;
		}
		
	}

	icf_nm_free_icf_messg_for_pltfm(p_deact->p_nm_ptr->p_icf_glb_ptr,
										  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_deact->p_nm_ptr->p_icf_glb_ptr,
										  p_port_pltfm_rsp);

	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_PENDING;
	ICF_FUNCTION_EXIT(p_deact->p_nm_ptr->p_icf_glb_ptr) 
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_send_platform_even_to_cfg
 *
 * DESCRIPTION:     It will notify CFG module about network events.
 *										
 * RETURN			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_send_platform_event_to_cfg(
				INOUT icf_nm_pdb_st		*p_nm_pdb)
													 
{
	icf_return_t		ret_val = ICF_SUCCESS;
	icf_internal_msg_st	msg_to_s;
	icf_msg_st                       *p_ext_msg = ICF_NULL;
	icf_port_pltfm_event_ind_st			* p_port_pltfm_event_ind;	
    icf_nm_nw_event_ind_st			event_ind; 
	icf_nm_nw_info_st       *p_nw_info=\
			p_nm_pdb->p_nm_glb_data->p_n_info_list;
	
	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	icf_port_memset(&msg_to_s,0,sizeof(&msg_to_s));

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    p_port_pltfm_event_ind = (icf_port_pltfm_event_ind_st *)(p_ext_msg->payload);

	/* In case of Nw failure clear all PDP related info */
	if(ICF_NM_NW_FAILURE==p_port_pltfm_event_ind->event)
	{
		icf_port_memset(p_nw_info,0,sizeof(icf_nm_nw_info_st));
		p_nw_info->pdp_cntxt_info.prim_pdp_id=ICF_NM_INVALID_PRIMARY_PDP_CTXT_ID;
	}



	msg_to_s.msg_hdr.msg_id=ICF_NM_CFG_EVENT_IND;
	msg_to_s.msg_hdr.p_glb_pdb = p_nm_pdb->p_glb_pdb;
	msg_to_s.msg_hdr.payload_length = sizeof(icf_nm_nw_event_ind_st);
	event_ind.event=p_port_pltfm_event_ind->event;
	event_ind.event_data=ICF_NULL;
	msg_to_s.p_msg_data=(icf_uint8_t*)&event_ind;
	ret_val = icf_cfg_process_mesg(&msg_to_s);

	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_nm_get_cntxt_from_cntxt_id
 *
 * DESCRIPTION:     This function is invoked to find the call index and sec 
 *					pdp index
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_get_call_id_from_cntxt_id(
         INOUT   icf_nm_global_st	*p_glb_nm,
         IN	     icf_uint16_t		 cntxt_id,
		 INOUT	     icf_boolean_t		*primary,
		 OUT	 icf_uint16_t	*call_index,
		 OUT	 icf_uint16_t	*sec_pdp_index)
    
{
	icf_uint16_t						outer_counter,inner_counter;
	icf_return_t		       			ret_val = ICF_SUCCESS;
	icf_nm_prim_sec_pdp_map_st	  *p_pdp_ctxt_info=
								&(p_glb_nm->p_n_info_list->pdp_cntxt_info);

	ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)

	*primary=ICF_FALSE;

	if(cntxt_id==p_pdp_ctxt_info->prim_pdp_id)
		*primary=ICF_TRUE;
		
	if(ICF_TRUE!=*primary)
	{
		ret_val=ICF_FAILURE;

		for(outer_counter=0;outer_counter<p_pdp_ctxt_info->number_of_calls;outer_counter++)
		{
			for(inner_counter=0;\
				inner_counter<p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[outer_counter].sec_pdps_count;\
				inner_counter++)
			{

				if((p_pdp_ctxt_info)->p_callid_sec_pdp_ctxts_map[outer_counter].sec_pdp_ids[inner_counter]==\
					cntxt_id)
				{
					*call_index=outer_counter;
					*sec_pdp_index=inner_counter;
					*primary=ICF_FALSE;
					ret_val=ICF_SUCCESS;
					break;
				}
			}

			if(ICF_SUCCESS==ret_val)
				break;
		}
    }

	ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_modify_ind
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   unsolicited modify indication is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_modify_ind(
        INOUT icf_nm_pdb_st		*p_nm_pdb)
{
	icf_uint16_t				call_index,sec_pdp_index;
	icf_boolean_t			primary;
    icf_return_t				ret_val = ICF_SUCCESS;

#ifdef ICF_QOS_SUPPORT
	icf_nm_prim_sec_pdp_map_st	  *p_pdp_ctxt_info=
										&(p_nm_pdb->p_nm_glb_data->p_n_info_list->pdp_cntxt_info);
#endif
	icf_msg_st                       *p_ext_msg = ICF_NULL;
   	icf_uint16_t					 	cntxt_id;
	icf_result_t						result;

	icf_port_pltfm_pdp_mod_ind_st		port_pltfm_pdp_mod_ind;

	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    port_pltfm_pdp_mod_ind = * (icf_port_pltfm_pdp_mod_ind_st *)(p_ext_msg->payload);

	cntxt_id=port_pltfm_pdp_mod_ind.context_id;

	result=(ICF_PORT_PLTFM_GPRS_NOERR==\
					port_pltfm_pdp_mod_ind.status)?(ICF_SUCCESS):(ICF_FAILURE);

	ret_val=icf_nm_get_call_id_from_cntxt_id(p_nm_pdb->p_nm_glb_data,
											    cntxt_id,
												&primary,
												&call_index,
												&sec_pdp_index);
#ifdef ICF_QOS_SUPPORT
	if((ICF_SUCCESS==ret_val) && (ICF_FALSE==primary))
	{
		
		if(ICF_SUCCESS!=icf_nm_send_sec_modify_ind(p_nm_pdb,
									  p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].call_id,
									  result))
		{
			ret_val=ICF_FAILURE;
		}
		
	}
#endif

	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_deactivate_ind
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   unsolicited deactivate indication is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_deactivate_ind(
        INOUT icf_nm_pdb_st		*p_nm_pdb)
{

	icf_uint16_t		call_index,sec_pdp_index;

	icf_boolean_t		primary;
    icf_return_t		    ret_val = ICF_SUCCESS;

#ifdef ICF_QOS_SUPPORT
	icf_nm_prim_sec_pdp_map_st	  *p_pdp_ctxt_info=
								&(p_nm_pdb->p_nm_glb_data->p_n_info_list->pdp_cntxt_info);
#endif
	icf_msg_st                       *p_ext_msg = ICF_NULL;
   	icf_uint16_t					 	 cntxt_id;
	icf_result_t						result;
	icf_port_pltfm_pdp_deact_ind_st		port_pltfm_pdp_deact_ind;

	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    port_pltfm_pdp_deact_ind = *(icf_port_pltfm_pdp_deact_ind_st *)(p_ext_msg->payload);

	cntxt_id=port_pltfm_pdp_deact_ind.context_id;

	result=(ICF_PORT_PLTFM_GPRS_NOERR==\
		port_pltfm_pdp_deact_ind.status)?(ICF_SUCCESS):(ICF_FAILURE);

	ret_val=icf_nm_get_call_id_from_cntxt_id(p_nm_pdb->p_nm_glb_data,
											    cntxt_id,
												&primary,
												&call_index,
												&sec_pdp_index);

#ifdef ICF_QOS_SUPPORT
	if((ICF_SUCCESS==ret_val) && (ICF_FALSE==primary))
	{
		/* Set the state to INACTIVE now */
			p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].sec_pdp_states[sec_pdp_index]=\
			ICF_NM_SEC_PDP_STATE_INACTIVE;
			
		if(ICF_SUCCESS!=icf_nm_send_sec_deactivate_ind(p_nm_pdb,
									  p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].call_id,
									  result))
			ret_val=ICF_FAILURE;		
	}
#endif

	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)

	return ret_val;
}

#if 0
/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_deactivate_rsp
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   deactivate response is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_deactivate_rsp(
        INOUT icf_nm_pdb_st		*p_nm_pdb)
{

	icf_uint16_t		call_index,sec_pdp_index;

	icf_boolean_t		primary;
    icf_return_t		    ret_val = ICF_SUCCESS;

	icf_nm_prim_sec_pdp_map_st	  *p_pdp_ctxt_info=\
					&(p_nm_pdb->p_nm_glb_data->p_n_info_list->pdp_cntxt_info);

	icf_msg_st                       *p_ext_msg = ICF_NULL;
    icf_uint16_t					 	 cntxt_id;
	icf_result_t						result;

	icf_port_pltfm_pdp_deact_rsp_st		port_pltfm_pdp_deact_ind;

	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    port_pltfm_pdp_deact_ind = *(icf_port_pltfm_pdp_deact_rsp_st *)(p_ext_msg->payload);

	cntxt_id=port_pltfm_pdp_deact_ind.context_id;

	result=(ICF_PORT_PLTFM_GPRS_NOERR==\
			port_pltfm_pdp_deact_ind.status)?(ICF_SUCCESS):(ICF_FAILURE);

	ret_val=icf_nm_get_call_id_from_cntxt_id(p_nm_pdb->p_nm_glb_data,
											    cntxt_id,
												&primary,
												&call_index,
												&sec_pdp_index);

	if((ICF_SUCCESS==ret_val) && (ICF_TRUE==primary))
	{
		p_pdp_ctxt_info->prim_pdp_id=ICF_NM_INVALID_PRIMARY_PDP_CTXT_ID;
	}

#ifdef ICF_QOS_SUPPORT
	if((ICF_SUCCESS==ret_val) && (ICF_FALSE==primary))
	{
		/* Set the state to INACTIVE now */
		p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].sec_pdp_states[sec_pdp_index]=\
															ICF_NM_SEC_PDP_STATE_INACTIVE;
		ret_val=icf_nm_check_all_sec_deactivated(p_nm_pdb->p_nm_glb_data,
				p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].call_id);

		if(ICF_SUCCESS==ret_val)
		{
			/* Shift all calls one index up */
			icf_callid_sec_pdp_ctxts_map_st * p_callid_sec_pdp_ctxts_map=
				&(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index]);
			
			icf_uint16_t	size=(&(p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[p_pdp_ctxt_info->number_of_calls-1])-\
								  p_callid_sec_pdp_ctxts_map);

			icf_nm_send_sec_deactivate_resp(p_nm_pdb,
							p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].call_id,result);
	
			if(size)
			{
				icf_port_memcpy(
					p_callid_sec_pdp_ctxts_map,
					p_callid_sec_pdp_ctxts_map+1,
					(size * sizeof(icf_callid_sec_pdp_ctxts_map_st)));
			}
			
			p_pdp_ctxt_info->number_of_calls--;

			if(0==p_pdp_ctxt_info->number_of_calls)
				p_pdp_ctxt_info->sec_pdp_id_counter=(p_pdp_ctxt_info->prim_pdp_id)+1;
			
		}

	}
#endif
	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)

	return ret_val;
}
#endif

/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_activate_rsp
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   activate response is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_activate_rsp(
        INOUT icf_nm_pdb_st		*p_nm_pdb)
{

	icf_uint16_t		call_index,sec_pdp_index;

	icf_boolean_t		primary;
    icf_return_t		    ret_val = ICF_SUCCESS;

#ifdef ICF_QOS_SUPPORT
	icf_nm_prim_sec_pdp_map_st	  *p_pdp_ctxt_info=\
					&(p_nm_pdb->p_nm_glb_data->p_n_info_list->pdp_cntxt_info);
#endif

	icf_msg_st                       *p_ext_msg = ICF_NULL;
   	icf_uint16_t					 	 cntxt_id;
	icf_result_t						result;
	icf_port_pltfm_pdp_deact_rsp_st		port_pltfm_pdp_deact_rsp;

	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    port_pltfm_pdp_deact_rsp = *(icf_port_pltfm_pdp_deact_rsp_st *)(p_ext_msg->payload);

	cntxt_id=port_pltfm_pdp_deact_rsp.context_id;

	result=(ICF_PORT_PLTFM_GPRS_NOERR==\
		port_pltfm_pdp_deact_rsp.status)?(ICF_SUCCESS):(ICF_FAILURE);


	ret_val=icf_nm_get_call_id_from_cntxt_id(p_nm_pdb->p_nm_glb_data,
											    cntxt_id,
												&primary,
												&call_index,
												&sec_pdp_index);
#ifdef ICF_QOS_SUPPORT
	if(ICF_SUCCESS==result)
	{
		if((ICF_SUCCESS==ret_val) && (ICF_FALSE==primary))
		{
			/* Set the state to ACTIVE now */
			p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].sec_pdp_states[sec_pdp_index]=\
															ICF_NM_SEC_PDP_STATE_ACTIVE;
			ret_val=icf_nm_check_all_sec_activated(p_nm_pdb->p_nm_glb_data,
				p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].call_id);

			if(ICF_SUCCESS==ret_val)
				icf_nm_send_sec_activate_resp(p_nm_pdb,
						p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].call_id,result);

			ret_val=ICF_SUCCESS;
		}
	}
	else
	{
		/* Set the state to INACTIVE now */
		p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].sec_pdp_states[sec_pdp_index]=\
															ICF_NM_SEC_PDP_STATE_INACTIVE;
		p_pdp_ctxt_info->sec_pdp_id_counter=(p_pdp_ctxt_info->sec_pdp_id_counter-1)%255;

		icf_nm_send_sec_activate_resp(p_nm_pdb,
						p_pdp_ctxt_info->p_callid_sec_pdp_ctxts_map[call_index].call_id,result);
	}
#endif

	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)

	return ret_val;
}

#endif/*#ifdef ICF_NW_ACTIVATE*/

#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_qos_res_for_non_ims
 *
 * DESCRIPTION	:     It will activate Multiple Secondary PDP contexts 
 *										
 * RETURN:			  ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
 
icf_return_t  icf_nm_qos_res_for_non_ims(
			IN		icf_nm_non_ims_qos_reserve_st		*p_non_ims_qos_reserve,
			INOUT	icf_internal_msg_hdr_st 			*p_msg_hdr,
			INOUT	icf_nm_pdb_st						*p_nm_pdb)
{
	icf_msg_st									*p_port_pltfm_req=ICF_NULL,\
												*p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t					port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;	
	icf_port_pltfm_qos_res_non_ims_req_st		*p_qos_res_req=ICF_NULL;
	icf_return_t								ret_val= ICF_SUCCESS;
	icf_nm_non_ims_qos_map_st					*p_non_ims_qos_map = ICF_NULL;
    icf_uint16_t                                count = 0;

	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  &p_port_pltfm_req,
										  sizeof(icf_port_pltfm_qos_res_non_ims_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  &p_port_pltfm_rsp,
										  sizeof(icf_port_pltfm_qos_res_non_ims_resp_st));

    /*Iterate thru the non_ims_qos_map and find the unused block*/
    for (count=0; count < ICF_MAX_NUM_OF_QOS_MAPS; count++)
    {
        if (ICF_NULL == p_nm_pdb->p_nm_glb_data->p_non_ims_qos_map[count])
        {
            break;
        }
    }
    /*If count is equal to ICF_MAX_NUM_OF_QOS_MAPS, means no unused block is
     *there so return failure, QOS call can not succeed*/
    if (ICF_MAX_NUM_OF_QOS_MAPS == count)
    {
        return ICF_FAILURE;
    }
    /*Converted p_non_ims_qos_map to array so that multiple calls
      for QOS can be run. If this is not done then memory overwritting
      happens.*/
	ICF_STATIC_MEMGET(
		p_nm_pdb->p_glb_pdb,
		sizeof(icf_nm_non_ims_qos_map_st),
       	p_nm_pdb->p_nm_glb_data->p_non_ims_qos_map[count],
		ICF_RET_ON_FAILURE,
        p_nm_pdb->p_ecode,
        ret_val);

    /*Get local reference of allocated map entry*/
	p_non_ims_qos_map = 
        p_nm_pdb->p_nm_glb_data->p_non_ims_qos_map[count];

	p_non_ims_qos_map->id = ICF_DUMMY_ID_FOR_NON_IMS;
	p_non_ims_qos_map->call_id = p_non_ims_qos_reserve->call_id;

	p_qos_res_req=(icf_port_pltfm_qos_res_non_ims_req_st *) p_port_pltfm_req->payload;
    /*instead of using DUMMY id as done before we will pass the
      call id so that resp can be identified when multiple calls
      happen for QOS */
  	p_qos_res_req->id = p_non_ims_qos_reserve->call_id;
	p_qos_res_req->payload = ICF_NULL;

	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_PENDING;

	/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_QOS_RES_REQ;

	icf_port_pltfm_qos_reserve_req_non_ims(
								p_port_pltfm_req,
								&port_pltfm_retval,
								p_port_pltfm_rsp);
	
	if(ICF_PORT_PLTFM_FAILURE==port_pltfm_retval)
		ret_val=ICF_FAILURE;

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  p_port_pltfm_rsp);

    /* increment global call_index for identified during next res
      that which index to use */
    p_nm_pdb->p_nm_glb_data->call_index++;
	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)

	return ret_val;

}

/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_qos_rel_for_non_ims
 *
 * DESCRIPTION	:     It will deactivate All Secondary PDP contexts associated 
 *					  with a particular Call ID
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

icf_return_t icf_nm_qos_rel_for_non_ims(
			IN		icf_nm_non_ims_qos_release_st	*p_non_ims_qos_release,
			INOUT	icf_internal_msg_hdr_st			*p_msg_hdr,
			INOUT	icf_nm_pdb_st					*p_nm_pdb)
{

	icf_return_t							ret_val = ICF_SUCCESS;
	icf_msg_st								*p_port_pltfm_req=ICF_NULL,\
											*p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_qos_rel_non_ims_req_st	*p_qos_rel_req;
	icf_port_pltfm_ret_val_t				port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;
	icf_nm_non_ims_qos_map_st				*p_non_ims_qos_map = ICF_NULL;
	icf_uint16_t							count =0;

	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_req,
									  sizeof(icf_port_pltfm_qos_rel_non_ims_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_rsp,
									  sizeof(icf_port_pltfm_qos_rel_non_ims_rsp_st));

        
    /*Search non_ims_qos_map for given call_id*/
    for(count = 0; count < ICF_MAX_NUM_OF_QOS_MAPS; count++)
    {
       if((ICF_NULL != p_nm_pdb->p_nm_glb_data->p_non_ims_qos_map[count]) &&
          (p_non_ims_qos_release->call_id == 
           p_nm_pdb->p_nm_glb_data->p_non_ims_qos_map[count]->call_id))
       {
           p_non_ims_qos_map = p_nm_pdb->p_nm_glb_data->p_non_ims_qos_map[count];
           p_nm_pdb->p_nm_glb_data->p_non_ims_qos_map[count] = ICF_NULL;
           break;
       }
    }
    /*If found then free otherwise return failure*/
	if (ICF_NULL != p_non_ims_qos_map)
	{
	    ICF_STATIC_MEMFREE(
                p_nm_pdb->p_glb_pdb,
                p_non_ims_qos_map,
                p_nm_pdb->p_ecode,
                ret_val)
    }
    else
    {
        /*Freeing the memory allocated for p_port_pltfm_req and
         * p_port_pltfm_rsp in Failure leg*/
	    icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  p_port_pltfm_req);

    	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  p_port_pltfm_rsp);
        return ICF_FAILURE;
    }

	p_qos_rel_req=(icf_port_pltfm_qos_rel_non_ims_req_st *) p_port_pltfm_req->payload;
		
	/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_QOS_REL_REQ;
	/* ICF_DUMMY_ID_FOR_NON_IMS is used temporarily for non ims */
	p_qos_rel_req->id=ICF_DUMMY_ID_FOR_NON_IMS;
	icf_port_pltfm_qos_release_req_non_ims(p_port_pltfm_req,
									&port_pltfm_retval,
									p_port_pltfm_rsp);

	if(ICF_PORT_PLTFM_FAILURE==port_pltfm_retval)
		ret_val=ICF_FAILURE;
		
	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
										  p_port_pltfm_rsp);

	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_PENDING;
	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb) 
	return ret_val;
}

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_qos_release_non_ims_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the deactivate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_qos_release_non_ims_resp(
		    INOUT icf_nm_pdb_st		*p_nm_pdb)
		
{
   icf_return_t                         ret_val = ICF_SUCCESS;
   icf_nm_global_st						*p_glb_nm=p_nm_pdb->p_nm_glb_data;
   icf_internal_msg_st                  *p_nm_internal_msg = p_nm_pdb->p_internal_msg;
   icf_nm_qos_release_rsp_st			msg_payload;
   
    
   ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)
   /* Fill the header of the message to be send */
    p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MMI_QOS_RELEASE_NON_IMS_RESP ;
	p_nm_internal_msg->msg_hdr.payload_length = 0;
	p_nm_internal_msg->msg_hdr.p_glb_pdb = p_glb_nm->p_icf_glb_ptr ;
    /* Filling the Call Id in the payload */
	(icf_nm_qos_release_rsp_st *)p_nm_internal_msg->p_msg_data = &msg_payload;
	msg_payload.result = ICF_PORT_PLTFM_GPRS_NOERR;

	/* Set the call Context for this call id in global pdb */

	ret_val=icf_nm_set_call_ctxt_in_glb_db(p_glb_nm,callid);

	if(ICF_SUCCESS==ret_val)
		ret_val = icf_mmi_process_mesg(p_nm_internal_msg);

          
    ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

    return ret_val; 
}
#endif


/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_qos_reserve_non_ims_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the activate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_qos_reserve_non_ims_resp(
        INOUT icf_nm_pdb_st		*p_nm_pdb)
{

	icf_return_t                        ret_val = ICF_SUCCESS;
	icf_msg_st							*p_ext_msg = ICF_NULL;
    icf_internal_msg_st                 *p_nm_internal_msg = p_nm_pdb->p_internal_msg;
	icf_result_t						result;
	icf_nm_qos_reserve_rsp_st			msg_payload;
	icf_port_pltfm_qos_res_non_ims_resp_st		qos_res_non_ims_resp_st;

	ICF_FUNCTION_ENTER(p_nm_pdb->p_nm_glb_data->p_icf_glb_ptr)

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    qos_res_non_ims_resp_st = *(icf_port_pltfm_qos_res_non_ims_resp_st *)(p_ext_msg->payload);
	result=(ICF_PORT_PLTFM_GPRS_NOERR==\
			qos_res_non_ims_resp_st.status)?(ICF_SUCCESS):(ICF_FAILURE);

   /* Fill the header of the message to be send */
    p_nm_internal_msg->msg_hdr.msg_id = ICF_NM_MMI_QOS_RESERVE_NON_IMS_RESP ;
	p_nm_internal_msg->msg_hdr.payload_length = 0;
	p_nm_internal_msg->msg_hdr.p_glb_pdb = p_nm_pdb->p_nm_glb_data->p_icf_glb_ptr ;

    /*qos_res_non_ims_resp_st.id was populated with call_id so populate
     *call_id field in icf_msg_st from it*/
	p_nm_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->hdr.call_id = 
            qos_res_non_ims_resp_st.id;

    /* Filling the Call Id in the payload */
	p_nm_internal_msg->p_msg_data = (icf_uint8_t *)&msg_payload;
	msg_payload.result = result;

/*	ret_val=icf_nm_set_call_ctxt_in_glb_db(p_nm_pdb->p_nm_glb_data,callid);*/

	if(ICF_SUCCESS==ret_val)
	    ret_val = icf_mmi_process_mesg(p_nm_internal_msg);
          
    ICF_FUNCTION_EXIT(p_nm_pdb->p_nm_glb_data->p_icf_glb_ptr)

    return ret_val; 

}
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_auth
 *
 * DESCRIPTION:     This will be used to do authentication operation and return
 *					the result parameters along with the status.
 *																				
 * RETURN			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_auth(icf_nm_aka_info_st  *p_aka, 
							   icf_internal_msg_hdr_st *p_msg_hdr )
{
	icf_return_t						ret_val = ICF_SUCCESS;
	icf_msg_st							*p_port_pltfm_req=ICF_NULL,\
										*p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t			port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;
	icf_port_pltfm_aka_challenge_req_st *p_port_aka_chg_req;
	icf_port_pltfm_aka_response_st		*p_port_aka_chg_rsp;
	
	ICF_FUNCTION_ENTER(p_aka->p_nm_ptr->p_icf_glb_ptr)

	icf_nm_get_icf_messg_for_pltfm(p_aka->p_nm_ptr->p_icf_glb_ptr,
									  &p_port_pltfm_req,
									  sizeof(icf_port_pltfm_aka_challenge_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_aka->p_nm_ptr->p_icf_glb_ptr,
									  &p_port_pltfm_rsp,
									  sizeof(icf_port_pltfm_aka_response_st));

	p_port_aka_chg_req=(icf_port_pltfm_aka_challenge_req_st *)p_port_pltfm_req->payload;
	p_port_aka_chg_rsp=(icf_port_pltfm_aka_response_st *)p_port_pltfm_rsp->payload;

	if (ICF_NULL != p_port_aka_chg_req)
    {
        p_port_aka_chg_req->autn=p_aka->autn;
        p_port_aka_chg_req->rand=p_aka->rand;
    }

		/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_AKA_CHALLENGE_REQ;

	icf_port_pltfm_run_aka_procedure(p_port_pltfm_req,
									&port_pltfm_retval,
									p_port_pltfm_rsp);

	
	
	if(ICF_PORT_PLTFM_FAILURE != port_pltfm_retval)
	{
		if (ICF_PORT_PLTFM_AKA_SUCCESSFUL == p_port_aka_chg_rsp->status)
		{
			icf_port_memcpy((icf_void_t *)(&(p_aka->res)),
								(icf_void_t *)(&(p_port_aka_chg_rsp->res)),
								(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));

			icf_port_memcpy((icf_void_t *)(&(p_aka->imsCk)),
								(icf_void_t *)(&(p_port_aka_chg_rsp->imsCk)),
								(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));

			icf_port_memcpy((icf_void_t *)(&(p_aka->imsIk)),
								(icf_void_t *)(&(p_port_aka_chg_rsp->imsIk)),
								(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
						


			p_aka->auts.str_len = 0;

		}
		else if (ICF_PORT_PLTFM_AKA_SEQRANGEERR == p_port_aka_chg_rsp->status)
		{
			/* SQN out of Range */
			icf_port_memcpy((icf_void_t *)(&(p_port_aka_chg_rsp->auts)),
								(icf_void_t *)(&(p_port_aka_chg_rsp->auts)),
								(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
			p_aka->res.str_len = 0;
		}
		else if (ICF_PORT_PLTFM_AKA_UNAUTHNWERR == p_port_aka_chg_rsp->status)
		{
			/* MAC failure */
			p_aka->res.str_len = 0;
			p_aka->auts.str_len = 0;

		}	
		else
		{
			ret_val = ICF_FAILURE;
		}
		
	}
	else
	{
		ret_val = ICF_FAILURE;
	}

	p_msg_hdr->status = ICF_STATUS_NM_REQUEST_COMPLETED;

	icf_nm_free_icf_messg_for_pltfm(p_aka->p_nm_ptr->p_icf_glb_ptr,
										  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_aka->p_nm_ptr->p_icf_glb_ptr,
										  p_port_pltfm_rsp);
	
	ICF_FUNCTION_EXIT(p_aka->p_nm_ptr->p_icf_glb_ptr)
		
	return ret_val;
}

/******************************************************************************
*
* FUNCTION:        icf_nm_set_call_ctxt_in_glb_db
*
* DESCRIPTION:     This function sets the local call context in 
*					the global database
*
*                   
* RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
*                  		case of any failure.       
*
******************************************************************************/
icf_return_t icf_nm_set_call_ctxt_in_glb_db(
		    INOUT icf_nm_global_st	*p_glb_nm,
			IN    icf_call_id_t		callid)
		
		
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_error_t			ecode;

	ICF_FUNCTION_ENTER(p_glb_nm->p_icf_glb_ptr)

	if(callid<ICF_INVALID_CALL_ID)
	{
		/* Message is from MMI AL so ask dbm to fetch call context */
		/* Request DBM for the ctx with the call id in the recvd msg */

		ret_val= icf_dbm_find_call_cntxt_blk(
                 p_glb_nm->p_icf_glb_ptr,
                 callid,
                 &(p_glb_nm->p_icf_glb_ptr->p_call_ctx),
                 &ecode);

		if(ICF_SUCCESS == ret_val)
		{
			if(p_glb_nm->p_icf_glb_ptr->p_call_ctx->call_id != callid)
				ret_val=ICF_FAILURE;
		}
	}

	ICF_FUNCTION_EXIT(p_glb_nm->p_icf_glb_ptr)

	return ret_val;
 }

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_process_scalable_params_req
 *
 * DESCRIPTION:     This function will process scalable parameter req of nm
 *					
 *
 * RETURN			:ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/
icf_return_t 	icf_nm_process_scalable_params_req
					(INOUT icf_nm_pdb_st		*p_nm_pdb)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_config_data_st      *p_config_data = ICF_NULL;
	icf_uint16_t			max_calls = ICF_NULL;
#ifdef ICF_NW_ACTIVATE
        icf_uint16_t 			call_count = ICF_NULL;
	icf_uint32_t			errhndlr = ICF_DONOT_RET_ON_FAILURE;
#endif

	ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	ret_val = icf_dbm_get_module_glb_data(
                      p_nm_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data,p_nm_pdb->p_ecode);

	ICF_ASSERT(p_nm_pdb->p_glb_pdb, (ICF_NULL != p_config_data))

	max_calls=(icf_uint16_t)((p_config_data->max_audio_calls+p_config_data->max_video_calls+
				p_config_data->max_service_calls));

#ifdef ICF_NW_ACTIVATE

	ICF_STATIC_MEMGET(p_nm_pdb->p_glb_pdb,
						(max_calls*sizeof(icf_callid_sec_pdp_ctxts_map_st)),\
       	     			 p_nm_pdb->p_nm_glb_data->p_n_info_list->pdp_cntxt_info.\
						 p_callid_sec_pdp_ctxts_map,errhndlr ,p_nm_pdb->p_ecode, 
						 ret_val);

	if(ICF_SUCCESS==ret_val)
	{
		icf_port_memset(p_nm_pdb->p_nm_glb_data->p_n_info_list->pdp_cntxt_info.\
			p_callid_sec_pdp_ctxts_map,	0,\
			(max_calls*sizeof(icf_callid_sec_pdp_ctxts_map_st)));

		for(call_count = 0; call_count < max_calls; call_count++)
		{
			p_nm_pdb->p_nm_glb_data->p_n_info_list->pdp_cntxt_info.\
			p_callid_sec_pdp_ctxts_map[call_count].call_id = ICF_INVALID_CALL_ID;
		}
	}
#endif
	ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)

	return ret_val;
}

#ifdef ICF_IPSEC_ENABLED
/* -------------------------------------------------------------------------- */
/* **** NM IPSec Functions **** */
/* -------------------------------------------------------------------------- */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_fetch_algo
 *
 * DESCRIPTION:     This is a utility function that returns the list of locally
 *                  supported security algorithms in internal message payload.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_fetch_algo(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_ipsec_self_algo_st           *p_algo_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

    /* allocate memory for new algorithm structure */
    ICF_STATIC_MEMGET(
        p_nm_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_ipsec_self_algo_st),
        p_algo_list,
        ICF_RET_ON_FAILURE,
        p_nm_pdb->p_ecode,
        ret_val)
        
    /* copy the algorithm list from NM data to REGM data */
    icf_port_memcpy(
        (icf_void_t *)(p_algo_list),
        (icf_void_t *)(p_nm_pdb->p_nm_glb_data->p_ue_algo_list),
        ICF_PORT_SIZEOF(icf_ipsec_self_algo_st));
    
    /* return the newly allocated list to RGM */
    p_nm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_algo_list;
    
    if (ICF_NULL == p_algo_list)
    {
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_nm_sa_fetch_algo() */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_create_req
 *
 * DESCRIPTION:     This is a utility function that handles SA creation request
 *                  from REGM.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_create_req(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_rgm_nm_sa_create_req_st      *p_rgm_req_pld = ICF_NULL;
	icf_msg_st							*p_port_pltfm_req=ICF_NULL,
										*p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t			port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;
	icf_port_pltfm_sa_create_req_st		*p_sa_create_req;
		
    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

    /* get the payload from RGM */
    p_rgm_req_pld = (icf_rgm_nm_sa_create_req_st *)
                    (p_nm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_nm_pdb->p_glb_pdb, p_rgm_req_pld);

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_req,
									  sizeof(icf_port_pltfm_sa_create_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_rsp,
									  sizeof(icf_port_pltfm_sa_cmd_resp_st));

	p_sa_create_req=(icf_port_pltfm_sa_create_req_st*)p_port_pltfm_req->payload;
	   
    icf_port_memcpy(
      (icf_void_t *)&(p_sa_create_req->ue_ip_port),
      (icf_void_t *)&(p_rgm_req_pld->ue_ip_port),
      ICF_PORT_SIZEOF(icf_transport_address_st));

    icf_port_memcpy(
      (icf_void_t *)&(p_sa_create_req->proxy_ip_port),
      (icf_void_t *)&(p_rgm_req_pld->proxy_ip_port),
      ICF_PORT_SIZEOF(icf_transport_address_st));

    p_sa_create_req->spi = p_rgm_req_pld->spi;

    p_sa_create_req->lifetime = p_rgm_req_pld->lifetime;

    p_sa_create_req->direction = p_rgm_req_pld->direction;

    icf_port_strcpy(
        p_sa_create_req->enc_algo,
        p_rgm_req_pld->enc_algo);

    icf_port_memcpy(
      (icf_void_t *)&(p_sa_create_req->enc_key),
      (icf_void_t *)&(p_rgm_req_pld->enc_key),
      ICF_PORT_SIZEOF(icf_string_st));
    
    icf_port_strcpy(
        p_sa_create_req->auth_algo,
        p_rgm_req_pld->auth_algo);

    icf_port_memcpy(
      (icf_void_t *)&(p_sa_create_req->auth_passwd),
      (icf_void_t *)&(p_rgm_req_pld->auth_passwd),
      ICF_PORT_SIZEOF(icf_string_st));

    p_sa_create_req->protocol = ICF_PORT_PLTFM_IPSEC_PROTO_ESP;

    p_sa_create_req->protocol_mode = 
        ICF_PORT_PLTFM_IPSEC_PROT_MODE_TUNNEL;

    p_sa_create_req->enc_mode = 
        ICF_PORT_PLTFM_IPSEC_ENC_MODE_ECBC;

    /* not sure about this */
    p_sa_create_req->key_life = p_rgm_req_pld->lifetime;

    /* not sure about this */
    p_sa_create_req->mask_bits = ICF_NULL;

	/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_SA_CREATE_REQ;
	icf_port_pltfm_sa_create_req(p_port_pltfm_req,
								 &port_pltfm_retval,
								 p_port_pltfm_rsp);

    if (ICF_PORT_PLTFM_FAILURE == port_pltfm_retval)
    {
        ret_val = ICF_FAILURE;

    }

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  p_port_pltfm_rsp);
									  
    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)

    return ret_val;
} /* End function icf_nm_sa_create_req() */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_create_resp
 *
 * DESCRIPTION:     This is a utility function that handles SA creation response
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_create_resp(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        rgm_resp_pld;
	icf_port_pltfm_sa_cmd_resp_st		* p_sa_cmd_resp;
	icf_msg_st                       *p_ext_msg = ICF_NULL;

    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);

	p_sa_cmd_resp=(icf_port_pltfm_sa_cmd_resp_st *) p_ext_msg->payload;
    /* construct the internal message to be sent to RGM */

    /* fill the message header */
    p_nm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NM_RGM_SA_CREATE_RESP;
    p_nm_pdb->p_internal_msg->msg_hdr.payload_length = 
        ICF_PORT_SIZEOF(icf_nm_rgm_sa_cmd_resp_st);

    /* fill the message payload */
    p_nm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&rgm_resp_pld;
    rgm_resp_pld.spi = p_sa_cmd_resp->spi;
    rgm_resp_pld.response = p_sa_cmd_resp->response;


    /* invoke CFG entry point to send the message to RGM */
    ret_val = icf_cfg_process_mesg(p_nm_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_nm_sa_create_resp() */


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_modify_req
 *
 * DESCRIPTION:     This is a utility function that handles SA modify request
 *                  from REGM.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_modify_req(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_rgm_nm_sa_modify_req_st      *p_rgm_req_pld = ICF_NULL;
    icf_msg_st							*p_port_pltfm_req=ICF_NULL,\
										*p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t			port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;
	icf_port_pltfm_sa_modify_req_st		*p_sa_modify_req;

    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_req,
									  sizeof(icf_port_pltfm_sa_modify_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_rsp,
									  sizeof(icf_port_pltfm_sa_cmd_resp_st));

	p_sa_modify_req=(icf_port_pltfm_sa_modify_req_st*)p_port_pltfm_req->payload;
	

    /* get the payload from RGM */
    p_rgm_req_pld = (icf_rgm_nm_sa_modify_req_st *)
                    (p_nm_pdb->p_internal_msg->p_msg_data);


    ICF_ASSERT(p_nm_pdb->p_glb_pdb, p_rgm_req_pld);

	p_sa_modify_req->spi = p_rgm_req_pld->spi;
    p_sa_modify_req->lifetime = p_rgm_req_pld->lifetime;

	/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_SA_MODIFY_REQ;

	icf_port_pltfm_sa_modify_req(p_port_pltfm_req,
								 &port_pltfm_retval,
								 p_port_pltfm_rsp);

    if (ICF_PORT_PLTFM_FAILURE == port_pltfm_retval)
    {
        ret_val = ICF_FAILURE;
    }

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  p_port_pltfm_rsp);

    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_nm_sa_modify_req() */
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_modify_resp
 *
 * DESCRIPTION:     This is a utility function that handles SA modify response
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_modify_resp(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        rgm_resp_pld;
	icf_msg_st                       *p_ext_msg = ICF_NULL;
	icf_port_pltfm_sa_cmd_resp_st		* p_sa_cmd_resp;

    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    p_sa_cmd_resp=(icf_port_pltfm_sa_cmd_resp_st *) p_ext_msg->payload;
    /* construct the internal message to be sent to RGM */

    /* fill the message header */
    p_nm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NM_RGM_SA_MODIFY_RESP;
    p_nm_pdb->p_internal_msg->msg_hdr.payload_length = 
        ICF_PORT_SIZEOF(icf_nm_rgm_sa_cmd_resp_st);

    /* fill the message payload */
     p_nm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&rgm_resp_pld;
    rgm_resp_pld.spi = p_sa_cmd_resp->spi;
    rgm_resp_pld.response = p_sa_cmd_resp->response;


    /* invoke CFG entry point to send the message to RGM */
    ret_val = icf_cfg_process_mesg(p_nm_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_nm_sa_modify_resp() */
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_delete_req
 *
 * DESCRIPTION:     This is a utility function that handles SA deletion request
 *                  from REGM.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_delete_req(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_rgm_nm_sa_delete_req_st      *p_rgm_req_pld = ICF_NULL;
    icf_msg_st							*p_port_pltfm_req=ICF_NULL,\
										*p_port_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_ret_val_t			port_pltfm_retval = ICF_PORT_PLTFM_SUCCESS;
	icf_port_pltfm_sa_delete_req_st		*p_sa_delete_req;

    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

    /* get the payload from RGM */
    p_rgm_req_pld = (icf_rgm_nm_sa_delete_req_st *)
                    (p_nm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_nm_pdb->p_glb_pdb, p_rgm_req_pld);

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_req,
									  sizeof(icf_port_pltfm_sa_delete_req_st));

	icf_nm_get_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  &p_port_pltfm_rsp,
									  sizeof(icf_port_pltfm_sa_cmd_resp_st));


	p_sa_delete_req = (icf_port_pltfm_sa_delete_req_st *)p_port_pltfm_req->payload;

    p_sa_delete_req->spi = p_rgm_req_pld->spi;

	/*Call to platform wrapper*/
	p_port_pltfm_req->hdr.api_id= ICF_PORT_PLTFM_SA_DELETE_REQ;

	icf_port_pltfm_sa_delete_req(p_port_pltfm_req,
								 &port_pltfm_retval,
								 p_port_pltfm_rsp);

    if (ICF_PORT_PLTFM_FAILURE == port_pltfm_retval)
    {
        ret_val = ICF_FAILURE;
    }

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  p_port_pltfm_req);

	icf_nm_free_icf_messg_for_pltfm(p_nm_pdb->p_glb_pdb,
									  p_port_pltfm_rsp);

    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_nm_sa_delete_req() */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_delete_resp
 *
 * DESCRIPTION:     This is a utility function that handles SA deletion response
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_delete_resp(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        rgm_resp_pld;
	icf_msg_st                       *p_ext_msg = ICF_NULL;
	icf_port_pltfm_sa_cmd_resp_st		* p_sa_cmd_resp;
	
    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)


    /* construct the internal message to be sent to RGM */

    /* fill the message header */
    p_nm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NM_RGM_SA_DELETE_RESP;
    p_nm_pdb->p_internal_msg->msg_hdr.payload_length = 
        ICF_PORT_SIZEOF(icf_nm_rgm_sa_cmd_resp_st);

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    p_sa_cmd_resp=(icf_port_pltfm_sa_cmd_resp_st *) p_ext_msg->payload;

    /* fill the message payload */
    p_nm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&rgm_resp_pld;
    rgm_resp_pld.spi = p_sa_cmd_resp->spi;
    rgm_resp_pld.response = p_sa_cmd_resp->response;

    /* invoke CFG entry point to send the message to RGM */
    ret_val = icf_cfg_process_mesg(p_nm_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_nm_sa_delete_resp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_expiry_ind
 *
 * DESCRIPTION:     This is a utility function that handles SA expiry indication
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_expiry_ind(
        INOUT   icf_nm_pdb_st        *p_nm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
	icf_msg_st                       *p_ext_msg = ICF_NULL;
    icf_port_pltfm_sa_expiry_ind_st     *p_sa_expiry_ind = ICF_NULL;
    icf_nm_rgm_sa_expiry_ind_st      rgm_resp_pld;

    ICF_FUNCTION_ENTER(p_nm_pdb->p_glb_pdb)

    /* construct the internal message to be sent to RGM */

    /* fill the message header */
    p_nm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NM_RGM_SA_EXPIRY_IND;
    p_nm_pdb->p_internal_msg->msg_hdr.payload_length = 
        ICF_PORT_SIZEOF(icf_nm_rgm_sa_expiry_ind_st);

	p_ext_msg = (icf_msg_st *)(p_nm_pdb->p_glb_pdb->p_recv_msg);
    p_sa_expiry_ind=(icf_port_pltfm_sa_expiry_ind_st *) p_ext_msg->payload;

    /* fill the message payload */
    p_nm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&rgm_resp_pld;
    rgm_resp_pld.spi = p_sa_expiry_ind->spi;
    
    /* invoke CFG entry point to send the message to RGM */
    ret_val = icf_cfg_process_mesg(p_nm_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_nm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_nm_sa_expiry_ind() */


/* ------------------ End: New functions for IP Security Support ------------ */
#endif /* ICF_IPSEC_ENABLED */

 /*End of file*/


