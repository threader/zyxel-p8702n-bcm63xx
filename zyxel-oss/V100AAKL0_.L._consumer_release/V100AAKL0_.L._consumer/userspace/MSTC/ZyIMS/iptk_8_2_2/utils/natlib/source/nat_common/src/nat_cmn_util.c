/******************************************************************************
 * 
 * FILE NAME:   nat_glb_mgmt_utility.c
 * 
 * DESCRIPTION: This file contains the common utility functions needed by NAT  
 *				Client Manager
  *              
 * REVISION HISTORY:    
 *     Date            Name            Reference       Comments
 *   24-Nov-2006	Anuradha Gupta       LLD      Initial Version
 *
 ******************************************************************************/       

#include "nat_error_id.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_common_prototypes.h"
#include "nat_common_internal_api.h"
#include "nat_common_internal_api_id.h"
#include "nat_glb_mgmt_prototypes.h"

/*******************************************************************************
 *
 * FUNCTION:        nat_cmn_compare_address
 *
 * DESCRIPTION:     This function compares two IP + PORT address  and returns
 *				    the status accordingly.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of address match and 
 *                  NAT_RETURN_FAILURE in case of no match with proper ecode set.                 
 *
 ******************************************************************************/
 nat_return_t
 nat_cmn_compare_address(
      IN nat_ip_port_address_st   src_addr,
      IN nat_ip_port_address_st   dest_addr,
      INOUT nat_error_t              ecode)
 {
    nat_return_t ret_val = NAT_RETURN_SUCCESS;

    if ( (src_addr.ipv4_addr.octet_1 != 
                 dest_addr.ipv4_addr.octet_1) ||
                (src_addr.ipv4_addr.octet_2 != 
                 dest_addr.ipv4_addr.octet_2) ||
                (src_addr.ipv4_addr.octet_3 != 
                 dest_addr.ipv4_addr.octet_3) ||
                (src_addr.ipv4_addr.octet_4 != 
                 dest_addr.ipv4_addr.octet_4))
    {
        ecode = NAT_ERR_IP_MISMATCH;
        ret_val = NAT_RETURN_FAILURE;
    }
    else if(src_addr.port != dest_addr.port)
    {
        ecode = NAT_ERR_IP_MISMATCH;
        ret_val = NAT_RETURN_FAILURE;
    }

    return ret_val;
 }
      
/*******************************************************************************
 *
 * FUNCTION:        nat_cmn_convert_ipv4_addr_to_string
 *
 * DESCRIPTION:     This function Converts transport Address to a string
 *                  Currently handles only for IPV4_ADDR
 *
 ******************************************************************************/
nat_return_t 
nat_cmn_convert_ipv4_addr_to_string(
        IN nat_ipv4_address_st *p_address,
		OUT nat_uint8_t *p_addr_string)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    
    nat_port_sprintf((nat_int8_t *) p_addr_string,"%d.%d.%d.%d",
            p_address->octet_1,
            p_address->octet_2,
            p_address->octet_3,
            p_address->octet_4);

    return ret_val;
 }




/*******************************************************************************
 *
 * FUNCTION:        nat_cmn_convert_ip_string_to_ip4_addr
 *
 * DESCRIPTION:     This function converts the transport address to a string
 *                  to be  sored in the SDP body of the SIP message     
 *
 ******************************************************************************/
nat_return_t nat_cmn_convert_ip_string_to_ip4_addr(
        IN nat_uint8_t *p_addr_string,
        OUT nat_ipv4_address_st *p_address)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
	nat_int32_t octet_1,octet_2,octet_3,octet_4;

	p_addr_string[NAT_MAX_IPV4_ADDRESS_LENGTH] = '\0';

	nat_port_sscanf((const nat_int8_t *) p_addr_string,"%d.%d.%d.%d",
        &octet_1,&octet_2,&octet_3,&octet_4);

	p_address->octet_1 = (nat_uint8_t)octet_1;
	p_address->octet_2 = (nat_uint8_t)octet_2;
	p_address->octet_3 = (nat_uint8_t)octet_3;
	p_address->octet_4 = (nat_uint8_t)octet_4;

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        nat_timer_cb_fn
 *
 * DESCRIPTION:     The callback function to be called on timer expiry
 ******************************************************************************/
nat_void_t nat_timer_cb_fn(nat_void_t *p_buff)
{
    nat_internal_msg_st    internal_msg;
    stun_timer_expire_ind_st  timer_exp_msg;
    nat_timer_data_st *p_timer_data = (nat_timer_data_st *)p_buff;
    nat_error_t ecode = NAT_ERR_UNDEFINED;
   
    NAT_PRINT(("\n TIMER CALLBACK FUNCTION CALLED \n"));
 
    internal_msg.msg_hdr.p_glb_pdb = p_nat_global_pdb;
    internal_msg.msg_hdr.msg_id = NAT_INT_TIMER_EXPIRY_IND;
    internal_msg.p_msg_data = (nat_uint8_t *)(&timer_exp_msg);
    
    timer_exp_msg.context_id = p_timer_data->context_id;
    timer_exp_msg.timer_type = p_timer_data->timer_type;
    
    nat_process_internal_mesg(&internal_msg);
    
    if(NAT_NULL != p_buff)
    {	
    	nat_port_static_memfree(p_buff,&ecode);
    }	
    
    return;
    
}
