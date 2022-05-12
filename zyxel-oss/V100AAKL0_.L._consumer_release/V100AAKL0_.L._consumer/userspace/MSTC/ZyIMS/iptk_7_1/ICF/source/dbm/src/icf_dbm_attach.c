/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_attach.c
 *
 * Purpose          : This file contains functions that would implement 
 *                    functionality to attach existing blocks
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  14 DEC 2004   Sumit Gupta                File created and initial version
 *  11 APR 2005   Sumit Gupta   SPR 7372     Added two statements in function 
 *                                           icf_dbm_attach_cc_to_line to 
 *                                           copy user name and user address in 
 *                                           call context block
 *                                           
 *  22 APR 2005   Sumit Gupta   SPR 7435     DBM would set release cause as
 *                                           ICF_CALL_CLEAR_REASON_RESOURCES_
 *                                           NOT_AVAILABLE in case of failure to
 *                                           get memory in attach_cc_to_line
 *  04 MAY 2005   Sumit Gupta   SPR 7476     The pointer was getting overwitten
 *                                           so there was only one element in 
 *                                           list no matter how call were there 
 *                                           on line                           
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
 *											 Multiple Public Users ICF5.0
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_dbm_defs.h"
#include "icf_dbm_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_sc_prototypes.h"
#include "icf_macro.h"


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_attach_sc_to_cc
 * 
 * DESCRIPTION:   Attaches an existing Sevice Context block to an existing Call
 *                Context Block
 *
 * RETURNS:       icf_return_t 
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_attach_sc_to_cc(            
      INOUT  icf_glb_pdb_st                         *p_glb_pdb, 
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      INOUT  icf_service_ctx_st                     *p_srvc_cntxt,
      OUT    icf_error_t                            *p_ecode)
{
   
    icf_return_t ret_val = ICF_SUCCESS;
    
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_DBM)

	p_glb_pdb = p_glb_pdb;
	
    ICF_FUNCTION_ENTER(p_glb_pdb)        
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_cntxt))

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_srvc_cntxt))

    p_call_cntxt->p_service_ctx = p_srvc_cntxt;
    p_ecode = p_ecode; /*To remove Warning*/ 
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    

    return (ret_val);
}
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_attach_cc_to_line
 * 
 * DESCRIPTION:   Attaches an existing Call Context block to an existing line 
 *                on which the call is going
 *
 * RETURNS:       icf_return_t 
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_attach_cc_to_line(            
      INOUT  icf_glb_pdb_st                         *p_glb_pdb, 
      INOUT  icf_line_id_t                          line_id,
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      OUT    icf_error_t                            *p_ecode)
{
    
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_call_ctx_list_st     *p_temp_call_ctx_lst = ICF_NULL;
    icf_call_ctx_list_st     **p_p_temp_call_ctx_lst = ICF_NULL;
	icf_address_st			*p_uri = ICF_NULL;
        
    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_cntxt))
       
    /*
     * If there is no user configured on line set call clear reason in call 
     * context and return failure
     */ 
   
    if(ICF_NULL == 
            p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_user_db)
    {
        /*Fill the release cause as no user configured on line*/
        p_call_cntxt->release_cause = 
            ICF_CALL_CLEAR_REASON_SELF_ADDR_NOT_CFGRD_FOR_LINE;

        ret_val = ICF_FAILURE;
    }    
    else
    {
        p_temp_call_ctx_lst = 
            p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list;
        p_p_temp_call_ctx_lst =
            &p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list;

        /*  
         * Traverse the list till last node to add node at the end of the list
         */
        while (ICF_NULL != p_temp_call_ctx_lst)
        {        
            p_p_temp_call_ctx_lst = &(p_temp_call_ctx_lst->p_call_ctx_next);
            p_temp_call_ctx_lst = p_temp_call_ctx_lst->p_call_ctx_next;
        }

        if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
            p_glb_pdb,
            (icf_pool_id_t const)ICF_MEM_CALL_CTX_LIST,
            (icf_void_t **)p_p_temp_call_ctx_lst,
            p_ecode))
         {
             /*Set release cause as resource not available*/
             p_call_cntxt->release_cause =
             ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE;
             ret_val = ICF_FAILURE;
         }        
    }
   
    if(ICF_SUCCESS == ret_val)
    { 
       p_temp_call_ctx_lst = 
            (*((icf_call_ctx_list_st **)p_p_temp_call_ctx_lst));
        p_call_cntxt->line_id = line_id;
        p_temp_call_ctx_lst->p_call_ctx = 
            p_call_cntxt;
        p_temp_call_ctx_lst->p_call_ctx_next =
            ICF_NULL;

        (p_glb_pdb->p_glb_data->p_p_line_data[line_id])->num_of_calls += 1;

        /* The local_addr in call_ctx may be overwritten for outgoing 
		 * call, but it should not be done for incoming call. 
		 * Isn't it ??
		 */
		if (ICF_NULL == (p_call_cntxt->common_bitmask & ICF_CALL_DIR))
		{

        /*
         * If string length for username is not null ie user name exist
         * Copy the user name in call context block attached to line
         */
        if((ICF_NULL != 
               p_glb_pdb->p_glb_data->p_p_line_data[line_id]->user_name.str_len)  &&
           (icf_port_strncmp(p_glb_pdb->p_glb_data->p_p_line_data[line_id]->user_name.str, \
                                     (icf_uint8_t *)ICF_WILDCARD_USER, icf_port_strlen((icf_uint8_t *)ICF_WILDCARD_USER))))
        {
            /*
             * Copy the first name in user name list in current call context
             */
            icf_port_memcpy(
                    &p_call_cntxt->local_name,
                    &p_glb_pdb->p_glb_data->p_p_line_data[line_id]->user_name,
                    (icf_uint32_t const)sizeof(icf_name_st));
            
            /*Set the user name present flag to true*/
            p_call_cntxt->common_bitmask |= ICF_LOCAL_NAME_PRESENT;
        }
           
        /*
         * Copy the first address in user address list in current call 
         * context
         */

		ret_val = icf_dbm_get_default_uri_for_line (
                                                     p_glb_pdb,
                                                     line_id,
                                                     &p_uri,
                                                     p_ecode);
		if(ICF_SUCCESS == ret_val)
		{
			icf_port_memcpy(
					&p_call_cntxt->local_address,
					p_uri,
					(icf_uint32_t const)sizeof(icf_address_st));
		}
        }/* if (ICF_NULL == (p_call_cntxt->common_bitmask & ICF_CALL_DIR)) */
    }
	 /* 
     * If call context is successfully attached to line the fill in the line 
     * related data in call context from the line
     */     
    if(ICF_SUCCESS == ret_val)
    {
        /* 
         * Read the presentation indication value from the line and fill it in 
         * the call context
         */ 
        /*If presentation indicator is setfor the line then set presentation 
         * indicator on in call context*/
        if(ICF_SETTINGS_CONFIG_PRESENTATION_IND & 
                p_glb_pdb->p_glb_data->p_p_line_data[line_id]->default_settings)
        {
            p_call_cntxt->common_bitmask |= ICF_TOGGLE_PI;
        }

    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return (ret_val);
}








/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_attach_cc_to_line_wo_user
 * 
 * DESCRIPTION:   Attaches an existing Call Context block to an existing line 
 *                on which the call is going but there is not user on the same
 *
 * RETURNS:       icf_return_t 
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_attach_cc_to_line_wo_user(            
      INOUT  icf_glb_pdb_st                         *p_glb_pdb, 
      INOUT  icf_line_id_t                          line_id,
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      OUT    icf_error_t                            *p_ecode)
{
    
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_call_ctx_list_st     *p_temp_call_ctx_lst = ICF_NULL;
    icf_call_ctx_list_st     **p_p_temp_call_ctx_lst = ICF_NULL;
        
    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_cntxt))
       

    p_temp_call_ctx_lst = 
    p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list;
    p_p_temp_call_ctx_lst =
        &p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list;

    /*  
     * Traverse the list till last node to add node at the end of the list
     */
    while (ICF_NULL != p_temp_call_ctx_lst)
    {        
        p_p_temp_call_ctx_lst = &(p_temp_call_ctx_lst->p_call_ctx_next);
        p_temp_call_ctx_lst = p_temp_call_ctx_lst->p_call_ctx_next;
    }

    if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_CALL_CTX_LIST,
                (icf_void_t **)p_p_temp_call_ctx_lst,
                p_ecode))
    {
        /*Set release cause as resource not available*/
        p_call_cntxt->release_cause =
            ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE;
        ret_val = ICF_FAILURE;
    }        

   
    if(ICF_SUCCESS == ret_val)
    { 
       p_temp_call_ctx_lst = 
            (*((icf_call_ctx_list_st **)p_p_temp_call_ctx_lst));
        p_call_cntxt->line_id = line_id;
        p_temp_call_ctx_lst->p_call_ctx = 
            p_call_cntxt;
        p_temp_call_ctx_lst->p_call_ctx_next =
            ICF_NULL;

        (p_glb_pdb->p_glb_data->p_p_line_data[line_id])->num_of_calls += 1;

    }

	 /* 
     * If call context is successfully attached to line the fill in the line 
     * related data in call context from the line
     */     
    if(ICF_SUCCESS == ret_val)
    {
        /* 
         * Read the presentation indication value from the line and fill it in 
         * the call context
         */ 
        /*If presentation indicator is setfor the line then set presentation 
         * indicator on in call context*/
        if(ICF_SETTINGS_CONFIG_PRESENTATION_IND & 
                p_glb_pdb->p_glb_data->p_p_line_data[line_id]->default_settings)
        {
            p_call_cntxt->common_bitmask |= ICF_TOGGLE_PI;
        }

    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return (ret_val);
}
