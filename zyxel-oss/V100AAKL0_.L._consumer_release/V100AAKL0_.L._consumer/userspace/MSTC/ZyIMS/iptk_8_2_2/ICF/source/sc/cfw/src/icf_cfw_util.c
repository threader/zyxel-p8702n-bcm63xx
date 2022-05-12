/*******************************************************************************
 *
 * FILE NAME :      icf_cfw_util.c
 *
 * DESCRIPTION:     This file contains the code of the utility functions
 *                  of the Call Forwarding logic.
 *                                    
 * REVISION HISTORY: 
 * 
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 11-Mar-2005  Shefali Aggarwal	        	Initial
 * 
 * 02-May-2005  Shefali Aggarwal                SPR 7450 - Function 
 *                                              icf_cfw_compare_addr will 
 *                                              compare addresses excluding 
 *                                              the port no.
 *
 * 02-May-2005  Shefali Aggarwal                SPR 7452 - Function 
 *                                              icf_cfw_trigger_ssa will 
 *                                              append the local address in the 
 *                                              begining of the linked list
 * 
 * 09-May-2005  Shefali Aggarwal                SPR 7501 - Max number of hops 
 *                                              will be checked against the
 *                                              configured value of max hops
 * 24-Aug-2007  Amit Sharma                     IPTK merge from PQT6.5
 *  8-Apr-2008  Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 04-Mar-2009 Saurabh Gupta     SPR 19349      Klocworks fix
 *                                              
 * Copyright (c) 2006, Aricent 
 ******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_cfw_prototypes.h"
#include "icf_cfw_macro.h"
#include "icf_cfw_types.h"
#include "icf_cfw_defs.h"
#include "icf_dbm_prototypes.h"
#include "icf_common_prototypes.h"
#include "icf_internal_api_id.h"

/*#include "icf_api_id.h"*/


/******************************************************************************
 *
 * FUNCTION:        icf_cfw_init
 *
 * DESCRIPTION:     This function is called by DBM to initialize the global 
 *                  data of CFW module. Currently this is an empty function as 
 *                  CFW has no global data.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_init(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        OUT     icf_void_t               **p_p_cfw_data,
        OUT     icf_error_t              *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

	p_glb_pdb = p_glb_pdb; 
    
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CFW)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* To remove warning of unused param */
    p_p_cfw_data = p_p_cfw_data;
    p_ecode = p_ecode;
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CFW)
    return ret_val;
}   

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_deinit
 *
 * DESCRIPTION:     This function is called by DBM to initialize the global 
 *                  data of CFW module. Currently this is an empty function as 
 *                  CFW has no global data.
 *                         
 ******************************************************************************/
icf_void_t icf_cfw_deinit(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        INOUT   icf_void_t               *p_cfw_data,
        OUT     icf_error_t              *p_ecode)
{
#ifdef ICF_TRACE_ENABLE
    icf_return_t ret_val = ICF_SUCCESS;
#endif    

    if(ICF_NULL == p_glb_pdb)
    {
       return;
    }

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CFW)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /* To remove warning of unused param */
    p_cfw_data = p_cfw_data;
    p_ecode = p_ecode;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CFW)
}    

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_forward_call
 *
 * DESCRIPTION:     This function is called by the entry point function to 
 *                  process the message received to forward the call after the
 *                  validation of msg_id and status.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_forward_call(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_boolean_t        unsucc_cfw = ICF_FALSE;
   
    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)
        
    /* Validate the request */
    ret_val = icf_cfw_validate_req(p_cfw_pdb);
    
    if((ICF_SUCCESS == ret_val) && (ICF_TRUE == p_cfw_pdb->is_valid_req))
    {
        ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_VALIDITY)

        /* Trigger SSA to send message 302 on the network */
        ret_val = icf_cfw_trigger_ssa(p_cfw_pdb);
        
        /* If the msg is successfully send */
        if(ICF_SUCCESS == ret_val)
        {
            ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_MSG_SEND)

            /* This message will tell SCM to return the status to CM,
             * type of CFW can be found from p_msg_data in the internal
             * msg and CM can then send appropriate API to CC
             */
            ICF_CFW_SET_STATUS(
                    p_cfw_pdb,
                    ICF_STATUS_SERVICE_ENDED_ERROR)
            
            /* Set release type and release cause */
            ICF_CFW_SET_REL_CAUSE_N_TYPE(
                p_cfw_pdb,
                ICF_REL_TYPE_INTERNAL_INITIATED,
                ICF_CALL_CLEAR_REASON_CALL_FORWARDED)   
           
           /* Here call is forwarded successfully so the stat shall be 
            * updated 
            */
            if (ICF_STATUS_CALL_FWD_UNCOND == p_cfw_pdb->cfw_type)
            {
                ICF_STAT(p_cfw_pdb->p_glb_pdb,
                        ICF_STAT_SUCC_CALL_FWD_UNCOND)
            }

            else if (ICF_STATUS_CALL_FWD_NO_ANS == p_cfw_pdb->cfw_type)
            {  
                ICF_STAT(p_cfw_pdb->p_glb_pdb,
                        ICF_STAT_SUCC_CALL_FWD_NOANSWER)
            }            
           
            else if (ICF_STATUS_CALL_FWD_BUSY_LINE == p_cfw_pdb->cfw_type)
            {
                ICF_STAT(p_cfw_pdb->p_glb_pdb,
                        ICF_STAT_SUCC_CALL_FWD_USER_BUSY)
            }
            /*Send call forward indication to application*/
            ret_val = icf_cfw_send_call_forward_ind(p_cfw_pdb);
        }
    }/* if((ICF_SUCCESS == .. */  
   
    /* If the request is not valid */
    else if((ICF_SUCCESS == ret_val) && 
            (ICF_FALSE == p_cfw_pdb->is_valid_req))
    {
        ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_VALIDITY)

        /* This message will tell SCM to return the status to CM,
         * type of CFW can be found from p_msg_data in the internal
         * msg and CM can then send appropriate API to CC
         */

        ICF_CFW_SET_STATUS(
                p_cfw_pdb,
                ICF_STATUS_SERVICE_ENDED_ERROR)
        
        /* Set the release cause in call ctx */
        if(ICF_STATUS_CALL_FWD_UNCOND == p_cfw_pdb->cfw_type)
        {
            ICF_CFW_SET_REL_CAUSE_N_TYPE(
                p_cfw_pdb,
                ICF_REL_TYPE_INTERNAL_INITIATED,
                ICF_CALL_CLEAR_REASON_SERVICE_FAILURE)
        } 

        if(ICF_STATUS_CALL_FWD_BUSY_LINE == p_cfw_pdb->cfw_type)
        {
            ICF_CFW_SET_REL_CAUSE_N_TYPE(
                p_cfw_pdb,
                ICF_REL_TYPE_INTERNAL_INITIATED,
                ICF_CALL_CLEAR_REASON_USER_BUSY)
        }  
            
        if(ICF_STATUS_CALL_FWD_NO_ANS == p_cfw_pdb->cfw_type)
        {
            ICF_CFW_SET_REL_CAUSE_N_TYPE(
                p_cfw_pdb,
                ICF_REL_TYPE_INTERNAL_INITIATED,
                ICF_CALL_CLEAR_REASON_NOANSWER)
        } 
        
        unsucc_cfw = ICF_TRUE;
    }
    
    /* If failure is returned by the validate function then it is due to
     * DBM failure and failure will be returned by CFW 
     */
    else if(ICF_FAILURE == ret_val)
    {
        unsucc_cfw = ICF_TRUE;
    }

    /* If CFW was unsucc then update the stat */
    if(ICF_TRUE == unsucc_cfw)
    {
        if (ICF_STATUS_CALL_FWD_UNCOND == p_cfw_pdb->cfw_type)
        {
            ICF_STAT(p_cfw_pdb->p_glb_pdb,
                    ICF_STAT_UNSUCC_CALL_FWD_UNCOND)
        }

        else if (ICF_STATUS_CALL_FWD_NO_ANS == p_cfw_pdb->cfw_type)
        {  
            ICF_STAT(p_cfw_pdb->p_glb_pdb,
                    ICF_STAT_UNSUCC_CALL_FWD_NOANSWER)
        }            
           
        else if (ICF_STATUS_CALL_FWD_BUSY_LINE == p_cfw_pdb->cfw_type)
        {
            ICF_STAT(p_cfw_pdb->p_glb_pdb,
                    ICF_STAT_UNSUCC_CALL_FWD_USER_BUSY)
        }         
    }    
   
    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
    return ret_val;        
}    


/******************************************************************************
 *
 * FUNCTION:        icf_cfw_validate_req
 *
 * DESCRIPTION:     This function is called to validate the request of CFW.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_validate_req(
       INOUT icf_cfw_pdb_st      *p_cfw_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_config_data_st       *p_config_data = ICF_NULL;
    icf_line_id_t            line_id = 0;
    icf_line_id_t            curr_line_id = ICF_NULL;
    icf_uint8_t              div_index;
    icf_diversion_data_st    *p_temp_div_data =ICF_NULL;
    icf_line_data_st         *p_line_data = ICF_NULL;
    icf_error_t              ecode;
    
    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)
        
    /* Get the curr line id from the call ctx. this is the line on which the 
     * call is to be forwarded
     */            
    curr_line_id = p_cfw_pdb->p_call_ctx->service_line_id;
    
     /* Request the dbm to get the pointer for config_data_st in p_config_data
     */ 
    if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_cfw_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_cfw_pdb->p_config_data),
                &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode)))
    {
        ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_FAIL_TO_GET_CFG_DATA)
        ret_val = ICF_FAILURE;
    }        
        
    else
    {
        p_config_data = p_cfw_pdb->p_config_data;

        /* Request dbm to get line data pointer for all the line ids
         * Take the pointer in p_line_data.
         */
        for(line_id = 0; (line_id < (p_config_data->max_lines)) && (line_id < ICF_MAX_NUM_OF_LINES); line_id++)
        {
            p_cfw_pdb->line_id = line_id;
            ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_LINE_ID)
                
            /* There is no check in CFW SLP that if no line is configured or
             * no user is present then it should fail the request. 
             * It is assumed that CC will fail the call in these cases and we
             * will not get the request for CFW
             */

            if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_cfw_pdb->p_glb_pdb,
                    line_id,
                    &(p_cfw_pdb->p_line_data[line_id]),
                    &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode)))
            {
                ICF_CFW_TRACE(
                        p_cfw_pdb, 
                        ICF_CFW_TRACE_FAIL_TO_GET_LINE_DATA)
            }
        }        
  
        if (curr_line_id < ICF_MAX_NUM_OF_LINES) p_line_data = p_cfw_pdb->p_line_data[curr_line_id];

        /* Fill the address to which the call is to be forwarded in the pdb */
        if(ICF_STATUS_CALL_FWD_UNCOND == p_cfw_pdb->cfw_type)    
        {
            /* check if the address is present on the line or not */
            if( (p_line_data) && (ICF_NULL == p_line_data->addr_for_cfu.addr_val.str_len))
            {
                p_cfw_pdb->is_valid_req = ICF_FALSE;
            }
            
            else
            {
                icf_port_memcpy(
                    (icf_void_t*)&(p_cfw_pdb->addr_for_cfw),
                    (icf_void_t*)&(p_line_data->addr_for_cfu),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
            }
        }        
        
        else if(ICF_STATUS_CALL_FWD_NO_ANS == p_cfw_pdb->cfw_type)
        {
            if((p_line_data) && (ICF_NULL == p_line_data->addr_for_cfd.addr_val.str_len))
            {
                p_cfw_pdb->is_valid_req = ICF_FALSE;
            }
            else
            {
                icf_port_memcpy(
                    (icf_void_t*)&(p_cfw_pdb->addr_for_cfw),
                    (icf_void_t*)&(p_line_data->addr_for_cfd),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
            }
        }        

        else if(ICF_STATUS_CALL_FWD_BUSY_LINE == p_cfw_pdb->cfw_type)
        {        
            if((p_line_data) && (ICF_NULL == p_line_data->addr_for_cfb.addr_val.str_len))
            {
                p_cfw_pdb->is_valid_req = ICF_FALSE;
            }
            else
            {
                icf_port_memcpy(
                    (icf_void_t*)&(p_cfw_pdb->addr_for_cfw),
                    (icf_void_t*)&(p_line_data->addr_for_cfb),
                    (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
            }
        }       
        
        /* If the address to which the call is to be forwarded is not present 
         * on the line then return with is_valid_req as false. There is
         * no need to check furthur
         */

        if(ICF_FALSE == p_cfw_pdb->is_valid_req)
        {
            return ret_val;
        }

        /* Check that calling party address is not same as the address
         * to which the call is to be forwarded as this is to be checked in 
         * both the cases if diversion info is present or not.
         */
    
        if(ICF_SUCCESS == icf_cfw_compare_addr(
                    p_cfw_pdb,
                    &(p_cfw_pdb->addr_for_cfw),
                    &(p_cfw_pdb->p_call_ctx->remote_party_addr)))
        {
            ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_CALLING_CFW_ADDR_MATCH)
            p_cfw_pdb->is_valid_req = ICF_FALSE;
        }        
        
        /* Check if diversion info is present and the above check is passed */
        if ((ICF_NULL != p_cfw_pdb->p_diversion_info) && 
                (ICF_TRUE == p_cfw_pdb->is_valid_req))
        {
            /* Do rest of the checks */
            /* If the number of hops is more than the max num of call 
             * forwarding hops    
             */
            if(p_cfw_pdb->p_diversion_info->no_of_hops >= 
                    p_config_data->max_num_of_call_forwarding_hops)
            {
                ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_INVALID_HOPS)
                p_cfw_pdb->is_valid_req = ICF_FALSE;
            }               
        
            else
            {
                p_temp_div_data = p_cfw_pdb->p_diversion_info->p_diversion_data;
                
                /* Traverse the diversion data to searh for self address
                 * and the address to which the call is to be forwarded
                 */
                
                for(div_index = 0; div_index < (p_cfw_pdb->p_diversion_info->
                            no_of_diversion_data); div_index++)
                {
                    /* Compare the address to which the call is to be 
                     * forwarded to the address in the diversion data
                     */
                    if(ICF_SUCCESS == icf_cfw_compare_addr(
                                p_cfw_pdb,
                                &(p_temp_div_data->diverted_frm_addr),
                                &(p_cfw_pdb->addr_for_cfw)))
                    {
                        ICF_CFW_TRACE(p_cfw_pdb,
                                ICF_CFW_TRACE_CFW_DIV_ADDR_MATCH)
                        p_cfw_pdb->is_valid_req = ICF_FALSE;
                        break;
                    }               
                    
                    /* Traverse all the lines and compare the self-addresses
                     * on all the lines to the address to which the call
                     * is to be forwarded
                     */
                    
                    for(line_id = 0; (line_id < p_config_data->max_lines) && (line_id < ICF_MAX_NUM_OF_LINES); 
                            line_id++)
                    {
                        p_line_data = p_cfw_pdb->p_line_data[line_id];

                        if(ICF_NULL != p_line_data)
                        {
                            icf_user_db_node_st	*p_node;
                            p_cfw_pdb->line_id = line_id;

                            if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
                                                  p_cfw_pdb->p_glb_pdb,
                                                  line_id,
                                                  &(p_temp_div_data->diverted_frm_addr),
                                                  ICF_USER_DB_USE_PUBLIC_URIS | 
							ICF_USER_DB_USE_ASSOCIATED_URIS | ICF_USER_DB_USE_CONTACT_URIS,
                                                  &p_node,
                                                  &ecode
                                                  ))
                            {
                                    ICF_CFW_TRACE(p_cfw_pdb, 
                                        ICF_CFW_TRACE_SELF_CFW_ADDR_MATCH)
                                    p_cfw_pdb->is_valid_req = ICF_FALSE;
                            }
                            
                  
                            /* If the request is already invalid then exit the
                             * loop for max_lines
                             */
                            if(ICF_FALSE == p_cfw_pdb->is_valid_req)
                            {
                                break;
                            }          
                        }/* if(ICF_NULL != */
                    }/* for(line_id = 0; */                        
                    
                    /* If the request is already invalid then exit the
                     * loop for number of diversion data
                     */

                    if(ICF_FALSE == p_cfw_pdb->is_valid_req)
                    {
                        break;
                    }          
               
                    if(ICF_NULL != p_temp_div_data->p_next)
                    {
                        p_temp_div_data = p_temp_div_data->p_next;
                    }
                    else
                    {
                        break;
                    }
                }/* End loop */
            }/* End else */
        }/* if((ICF_NULL != .. */      
    }/* if(ICF_SUCCESS == .. */
  
    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
    return ret_val;        
}    


/******************************************************************************
 *
 * FUNCTION:        icf_cfw_trigger_ssa
 *
 * DESCRIPTION:     This function is called to trigger SSA to send 302 on the
 *                  network.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_trigger_ssa(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_nw_otg_call_fwd_st   call_fwd_req;
    icf_diversion_data_st    *p_temp_div_data = ICF_NULL;
    icf_uint8_t              div_info = 0;
    icf_diversion_data_st    *p_curr_div_data = ICF_NULL;
    icf_config_data_st       *p_config_data = ICF_NULL;
    icf_line_data_st         *p_line_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)
    
    call_fwd_req.bitmask = ICF_NULL;
    
    /* Update the diversion data if present */
    if(ICF_NULL != p_cfw_pdb->p_diversion_info)
    {
	    ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_DIV_INFO_PRESENT)
            
        p_cfw_pdb->p_diversion_info->no_of_hops++;
    
        /* Allocate memory for the new data to be added */
        ICF_MEMGET(
            p_cfw_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_diversion_data_st),
            ICF_MEM_COMMON,
            p_temp_div_data,
            ICF_RET_ON_FAILURE,
            &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val)

        p_temp_div_data->p_next = ICF_NULL;

        /* Copy the local address in the diversion data */
        icf_port_memcpy(
            (icf_void_t*)
                &(p_temp_div_data->diverted_frm_addr),
            (icf_void_t*)
                &(p_cfw_pdb->p_call_ctx->local_address),
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
        
        /* Add the node in the beginning of the diversion data linked list */
        p_temp_div_data->bitmask = ICF_NULL;
        p_temp_div_data->p_next = p_cfw_pdb->p_diversion_info->p_diversion_data;        
        p_cfw_pdb->p_diversion_info->p_diversion_data = p_temp_div_data;
        
        /* Increase the number of elements in the diversion data */
        p_cfw_pdb->p_diversion_info->no_of_diversion_data++;
    }
    
    /* If diversion data is not present then allocate memory to the
     * diversion info and then populate the diversion data to be send in
     * 302 message
     */
    
    else
    {
        p_config_data = p_cfw_pdb->p_config_data;
        
        /* Get line data pointer for the service line id in the call ctx */
        if ( p_cfw_pdb->p_call_ctx->service_line_id < ICF_MAX_NUM_OF_LINES) p_line_data = 
           p_cfw_pdb->p_line_data[p_cfw_pdb->p_call_ctx->service_line_id];
        
        ICF_MEMGET(
            p_cfw_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_diversion_info_st),
            ICF_MEM_COMMON,
            p_cfw_pdb->p_call_ctx->p_diversion_header_list,
            ICF_RET_ON_FAILURE,
            &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val)
            
        p_cfw_pdb->p_diversion_info = p_cfw_pdb->p_call_ctx->
            p_diversion_header_list;
        
        p_cfw_pdb->p_diversion_info->no_of_diversion_data = 1;
        p_cfw_pdb->p_diversion_info->no_of_hops = 
            p_config_data->max_num_of_call_forwarding_hops;
        
        ICF_MEMGET(
            p_cfw_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_diversion_data_st),
            ICF_MEM_COMMON,
            p_cfw_pdb->p_diversion_info->p_diversion_data,
            ICF_RET_ON_FAILURE,
            &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val)

        /* Copy the local address in the diversion data */
        icf_port_memcpy(
            (icf_void_t*)
                &(p_cfw_pdb->p_diversion_info->p_diversion_data->
                    diverted_frm_addr),
            (icf_void_t*)
                &(p_cfw_pdb->p_call_ctx->local_address),
            (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
        
        /* Check if name is present */
        p_cfw_pdb->p_diversion_info->p_diversion_data->bitmask = ICF_NULL;
        
        if((p_line_data) && (ICF_NULL != p_line_data->user_name.str_len))
        {
            icf_port_memcpy(
                (icf_void_t*)&(p_cfw_pdb->p_diversion_info->
                                  p_diversion_data->diverted_frm_name),
                (icf_void_t*)&(p_line_data->user_name),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_name_st));
            
           p_cfw_pdb->p_diversion_info->p_diversion_data->bitmask |=  
               ICF_DIVERTERD_NAME_PRESENT;
        }            
        p_cfw_pdb->p_diversion_info->p_diversion_data->p_next = ICF_NULL;
    }        

    /* Copy the contact address in the diversion info
     */
    
    ICF_MEMGET(
            p_cfw_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_address_st),
            ICF_MEM_COMMON,
            p_cfw_pdb->p_diversion_info->p_contact_addr,
            ICF_RET_ON_FAILURE,
            &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode),
            ret_val)

    icf_port_memcpy(
                (icf_void_t*)(p_cfw_pdb->p_diversion_info->p_contact_addr),
                (icf_void_t*)&(p_cfw_pdb->addr_for_cfw),
                (icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
    
    p_cfw_pdb->p_diversion_info->p_contact_name = ICF_NULL;
    p_cfw_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_CALL_REDIRECT;

    /* Traces for data send to SSA */
    p_curr_div_data = p_cfw_pdb->p_diversion_info->p_diversion_data; 
        
    for(div_info = 0; div_info < (p_cfw_pdb->p_diversion_info->
                no_of_diversion_data); div_info++)
    {
        p_cfw_pdb->p_temp_div_data = p_curr_div_data;

        ICF_CFW_TRACE(p_cfw_pdb, ICF_CFW_TRACE_DATA_SEND)
            
        if(ICF_NULL != p_curr_div_data->p_next)
        {
            p_curr_div_data = p_curr_div_data->p_next;
        }                
        else
        {
            break;
        }
    }            
   
    /* Call SSA process msg */
    ret_val = icf_ssa_process_mesg(p_cfw_pdb->p_internal_msg);        
        
    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
    return ret_val;        
} 

/******************************************************************************
 *
 * FUNCTION:        icf_cfw_compare_addr
 *
 * DESCRIPTION:     This function compares the addresses passed to it and
 *                  returns success if they match else return failure.
 *                         
 ******************************************************************************/
icf_return_t icf_cfw_compare_addr(
        INOUT icf_cfw_pdb_st     *p_cfw_pdb,
        IN    icf_address_st     *p_addr_a,
        IN    icf_address_st     *p_addr_b)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      *p_str_a = ICF_NULL;
    icf_uint8_t      *p_str_b = ICF_NULL;
    icf_uint8_t      str_a[ICF_MAX_STR_LEN];
    icf_uint8_t      str_b[ICF_MAX_STR_LEN];
    
    if(ICF_NULL == p_cfw_pdb)
    {
       return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)
        
    /* Extract the address excluding the port number from the string */
    icf_port_strcpy(str_a, p_addr_a->addr_val.str);
    icf_port_strcpy(str_b, p_addr_b->addr_val.str);
        
    p_str_a = icf_port_strchr(str_a,(icf_uint8_t)':');
    p_str_b = icf_port_strchr(str_b,(icf_uint8_t)':');
    
    /* If the string does not contain the port number then the pointer
     * is returned as null. Check if the pointer is null then use the
     * actual string.
     */
    
    if(ICF_NULL == p_str_a)
    {
        icf_port_strcpy(str_a,p_addr_a->addr_val.str);
    }      
    else
    {
        icf_port_strcpy(str_a,p_addr_a->addr_val.str);
        p_str_a = (icf_uint8_t *)icf_port_strtok((char *)str_a,":");
	if(ICF_NULL != p_str_a)
	{
        	icf_port_strcpy(str_a,p_str_a);        
	}
    }        
   
    if(ICF_NULL == p_str_b)
    {
        icf_port_strcpy(str_b,p_addr_b->addr_val.str);
    }   
    else
    {
        icf_port_strcpy(str_b,p_addr_b->addr_val.str);
        p_str_b = (icf_uint8_t *)icf_port_strtok((char *)str_b,":");
	if(ICF_NULL != p_str_b)
	{
        	icf_port_strcpy(str_b,p_str_b);        
	}
    }  
        
    /* If the addresses are same then return success */
    if ((p_addr_a->addr_type == p_addr_b->addr_type) &&
            (0 == icf_port_strcmp(str_a,str_b)))
    {
        ret_val = ICF_SUCCESS;
    }
    
    /* If the addresses are different then return failure */
    else
    {
        ret_val = ICF_FAILURE;
    }
        
    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
    return ret_val;
}   

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfw_trace
 *
 * DESCRIPTION:     This function checks the trace id and calls the macro for
 *                  trace.
 *
 ******************************************************************************/
icf_void_t icf_cfw_trace(
        INOUT   icf_cfw_pdb_st           *p_cfw_pdb,
        IN      icf_trace_id_t           trace_id)
{
    icf_trace_data_st                    trace_data;
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)

    switch(trace_id)
    {
        case ICF_CFW_TRACE_CFW_TYPE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_cfw_pdb->cfw_type);
            break;
            
        case ICF_CFW_TRACE_LINE_ID:
        case ICF_CFW_TRACE_FAIL_TO_GET_LINE_DATA:               
        case ICF_CFW_TRACE_SELF_CFW_ADDR_MATCH:            
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = (icf_int32_t)(p_cfw_pdb->line_id);
            break;
            
        case ICF_CFW_TRACE_VALIDITY:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_int32_t)(p_cfw_pdb->is_valid_req);
            break;
            
        case ICF_CFW_TRACE_DATA_SEND:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            
            if(ICF_NULL == p_cfw_pdb->p_diversion_info)
            {
                trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
                trace_data.int_val[0] = 
                    (icf_int32_t)(((icf_nw_otg_call_fwd_st *)
                                (p_cfw_pdb->p_internal_msg->p_msg_data))->
                                     bitmask);
            }
            
            else
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

                trace_data.int_val[0] = 
                    (icf_int32_t)(p_cfw_pdb->p_diversion_info->
                                     no_of_diversion_data);
                trace_data.int_val[1] = 
                    (icf_int32_t)(p_cfw_pdb->p_diversion_info->no_of_hops);
                trace_data.int_val[2] = 
                    (icf_int32_t)(p_cfw_pdb->p_temp_div_data->bitmask);
                trace_data.int_val[3] = 
                    (icf_int32_t)(p_cfw_pdb->p_temp_div_data->diverted_frm_addr.
                                     addr_type);
            
                icf_port_strcpy(trace_data.string_val[0].str,
                        (p_cfw_pdb->p_temp_div_data->diverted_frm_addr.addr_val.str));
                    
                trace_data.string_val[0].str_len =(icf_uint16_t) 
                    icf_port_strlen(trace_data.string_val[0].str);
            }                
            break;
            
        case ICF_CFW_TRACE_REL_TYPE_N_CAUSE:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_int32_t)(p_cfw_pdb->p_call_ctx->release_type);
            trace_data.int_val[1] = 
                (icf_int32_t)(p_cfw_pdb->p_call_ctx->release_cause);
            break;
            
        case ICF_CFW_TRACE_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_int32_t)(p_cfw_pdb->p_internal_msg->msg_hdr.status);
            break;
                
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    }        
 
    /* Finally, invoke the common macro for trace handling */
    ICF_TRACE(p_cfw_pdb->p_glb_pdb, trace_id, &trace_data)

    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        icf_cfw_app_trace
 *
 * DESCRIPTION:     This function checks the trace id and calls the macro for
 *                  trace.
 *
 ******************************************************************************/
icf_return_t icf_cfw_app_trace(
        INOUT   icf_cfw_pdb_st           *p_cfw_pdb,
        IN      icf_trace_id_t           trace_id)
{
    icf_trace_data_st                    trace_data;
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_trace_criteria_data_st    *p_criteria_data = p_cfw_pdb->p_internal_msg->
                           msg_hdr.p_glb_pdb->p_call_ctx->p_trace_criteria_data;
   /* Fix for CSR-1-5249829 */
    ret_val = icf_cmn_check_app_trace(p_cfw_pdb->p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
       return ret_val;
    }
    else
    {
 
    p_cfw_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_cfw_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->app_id;

    p_cfw_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_cfw_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id;

    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)

    switch(trace_id)
    {
        case ICF_TRACE_CALL_WAITING_IND:

            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            
            /*
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT; 
      
            trace_data.int_val[0] = call_waiting_count;
  
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
              
            

            break;
                      */
        default:
 
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;
    }

    /* Finally, invoke the common macro for trace handling */
    ICF_APP_TRACE(p_cfw_pdb->p_glb_pdb, trace_id, &trace_data, p_criteria_data)

    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
 }
    return ret_val;
}/* End function */
#endif

#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cfw_ecode
 *
 * DESCRIPTION:     This function sets the ecode
 *
 ******************************************************************************/
icf_void_t icf_cfw_ecode(
        INOUT   icf_cfw_pdb_st           *p_cfw_pdb,
        IN      icf_error_id_t           ecode)
{
    icf_error_data_st                    error_data;
    icf_error_t                          *p_ecode = ICF_ERROR_NONE;
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)
    p_cfw_pdb = p_cfw_pdb;
    p_ecode = &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode);

    error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
    error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
    
    /* Finally, invoke the common macro for error handling */
    ICF_SET_ECODE(p_cfw_pdb->p_glb_pdb, &error_data, ecode, p_ecode)

    ret_val = ICF_SUCCESS;
    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
}    
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cfw_send_call_forward_ind
 *
 * DESCRIPTION:     This function send call forward indication to application
 *
 ******************************************************************************/
icf_return_t   icf_cfw_send_call_forward_ind(
            INOUT   icf_cfw_pdb_st      *p_cfw_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_msg_st                  *p_msg = ICF_NULL;
    icf_error_t                 *p_ecode = ICF_NULL;
    icf_uint32_t                api_len = 0;
    icf_call_forwarded_ind_st   *p_call_fwd_ind = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cfw_pdb->p_glb_pdb)

    p_ecode =  &(p_cfw_pdb->p_internal_msg->msg_hdr.ecode);

    api_len = 
    sizeof(icf_api_header_st) + sizeof(icf_call_forwarded_ind_st);

    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_cfw_pdb->p_glb_pdb, api_len, ICF_MEM_COMMON,
            p_msg, ICF_RET_ON_FAILURE, 
            p_ecode, ret_val)

    if(ICF_NULL != p_msg)
    {
        /*API-ID to send */
        p_msg->hdr.api_id = ICF_CALL_FORWARDED_IND;
        
        /*Source module is ICF*/
        p_msg->hdr.source_id = ICF_MODULE_ICF;
        
        /*Destination is the module to which response targetted*/
        p_msg->hdr.destination_id = ICF_MODULE_PA;
        
        /*Payload length is size of api header + response structure*/
        p_msg->hdr.api_length = (icf_uint16_t)api_len;
        
        if((ICF_NULL != p_cfw_pdb->p_glb_pdb->p_call_ctx))
        {
            /*Call ID from glb_pdb*/
            p_msg->hdr.call_id = p_cfw_pdb->p_glb_pdb->p_call_ctx->call_id;
            p_msg->hdr.app_id = p_cfw_pdb->p_glb_pdb->p_call_ctx->app_id;
        }

        p_call_fwd_ind = (icf_call_forwarded_ind_st*)&(p_msg->payload[0]);
        p_call_fwd_ind->bit_mask = ICF_NULL;
        p_call_fwd_ind->call_forwarded_type = ICF_NULL;

        /*Fill call forward type*/
        if (ICF_STATUS_CALL_FWD_UNCOND == p_cfw_pdb->cfw_type)
        {
            p_call_fwd_ind->call_forwarded_type = ICF_CALL_FORWARD_TYPE_UNCOND;
        }        
        else if (ICF_STATUS_CALL_FWD_NO_ANS == p_cfw_pdb->cfw_type)
        {
            p_call_fwd_ind->call_forwarded_type = ICF_CALL_FORWARD_TYPE_NO_ANSWER;
        }
        else if (ICF_STATUS_CALL_FWD_BUSY_LINE == p_cfw_pdb->cfw_type)
        {
            p_call_fwd_ind->call_forwarded_type = ICF_CALL_FORWARD_TYPE_BUSY_LINE;
        }
        
        /*Fill remote party address from call context*/
        icf_port_memcpy(&(p_call_fwd_ind->remote_party_addr), 
            &(p_cfw_pdb->p_call_ctx->remote_party_addr), 
            sizeof(icf_address_st));

        /*Fill remote party name if present in call context*/
        if (ICF_REMOTE_NAME_PRESENT & p_cfw_pdb->p_call_ctx->common_bitmask)
        {
            icf_port_memcpy(&(p_call_fwd_ind->remote_party_name), 
                &(p_cfw_pdb->p_call_ctx->remote_party_name),
                sizeof(icf_name_st));

            p_call_fwd_ind->bit_mask = ICF_REMOTE_PARTY_NAME;
        }

        /*Assign p_msg to p_msg_data of icf-internal_msg_st*/
        p_cfw_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CALL_FORWARDED_IND;
        /* Fix for CSR-1-5249829 */
        p_cfw_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_cfw_pdb->p_call_ctx->call_id;


        p_cfw_pdb->p_internal_msg->msg_hdr.payload_length = (icf_uint16_t)api_len;

        p_cfw_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        if (ICF_FAILURE == 
                icf_adm_process_mesg(p_cfw_pdb->p_internal_msg))
        { 
            ICF_MEMFREE(p_cfw_pdb->p_glb_pdb, p_msg, 
                    ICF_MEM_COMMON, p_ecode, ret_val)

            ret_val = ICF_FAILURE;
        } /* icf_adm_process_mesg(p_pdb-... */
        else
        {
            ICF_MEMFREE(p_cfw_pdb->p_glb_pdb, p_msg, 
                    ICF_MEM_COMMON, p_ecode, ret_val)
        } /* else */           
    } /* if(ICF_NULL != p_msg) */


    ICF_FUNCTION_EXIT(p_cfw_pdb->p_glb_pdb)
    return ret_val;
}
#endif
