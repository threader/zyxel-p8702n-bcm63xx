/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_deinit.c
 *
 * Purpose          : This file contains functions that would implement 
 *                    functionality deinitailize and reinitialize existing 
 *                    structures and modules
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  14 DEC 2004   Sumit Gupta                File created and initial version
 *	06 MAY 2004	  Vidyut Gupta				Block the initialization of scm module
 *											(ICF ported on WINDOWS for IMS Client)
 *  12 Apr 2005   Amit Sharma                Updated rfn_deinit to rnw_deinit
 *  12 APR 2005   Jyoti Narula               Added deinit for SM Handler
 *  04 MAY 2005   Sumit Gupta                Remove commented code and insure 
 *                                           warning
 *    
 *  20 MAY 2005   Sumit Gupta    SPR 7584    Stats data would be deleted at the 
 *                                           time of delete_configuration
 *  23-Aug-2005	  Ashutos Rath  	 ICF	 Added Deinit function for SIGSA 
 *	08-Mar-2006     Aman Aggarwal            ICF merging with ICF2.0
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#include"icf_dbm_types.h"
#include"icf_dbm_prototypes.h"
#include"icf_ssa_common_types.h"
#include"icf_macro.h"
/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_restart
 *
 *     DESCRIPTION      :    Calls DBM's stop function to clear all memory 
 *                           allocated by DBM and then call init function to 
 *                           reallocate memory.
 *
 *     RETURNS          :  icf_return_t
 *
 *************************************************************************/
icf_return_t 
icf_dbm_restart(
        OUT icf_glb_pdb_st                       *p_glb_pdb,
        IN  icf_dbm_scale_data_init_st     *p_init_param,
        OUT icf_error_t                          *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Call DBM's Stop function to deallocate all memory held by dbm */   
    if(ICF_FAILURE == icf_dbm_stop(
                                    p_glb_pdb,
                                    p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /* Call DBM's init function to initialize DBM */
    else if(ICF_FAILURE == icf_dbm_init_scalable_param(
                                   p_glb_pdb,
                                   p_init_param, 
                                   p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
   /* else if call cfg init data function
    {
    }*/
    /* else if call sm init data function
    {
    }*/
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
            
    return (ret_val);

}/* end icf_dbm_restart */

/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_stop
 *
 *     DESCRIPTION      :    Clears all the memory held by DBM .
 *
 *     RETURNS          :    icf_return_t 
 *
 *************************************************************************/
icf_return_t 
icf_dbm_stop(
       INOUT icf_glb_pdb_st           *p_glb_pdb,
       OUT   icf_error_t              *p_ecode)
{
   
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_uint16_t               counter_i = 0;
    icf_config_data_st         *p_config_data = ICF_NULL;
    icf_dbm_data_st            *p_dbm_data = ICF_NULL;
    
    
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
        
    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    ret_val = icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode);
    
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
 
    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
        
    
    /* Free all the memory held by various line blocks pointers */
    while(counter_i < p_config_data->max_lines)
    {
        if(ICF_NULL != 
                *(p_glb_pdb->p_glb_data->p_p_line_data + counter_i))
        {
            icf_dbm_dealloc_line_blk( p_glb_pdb,
                    (icf_line_id_t const)counter_i,p_ecode);
        }
        counter_i += 1;
    }

    /* Free array of line pointers*/   
    ICF_STATIC_MEMFREE( p_glb_pdb,
            p_glb_pdb->p_glb_data->p_p_line_data,
            p_ecode, ret_val)

        icf_dbm_deinit_scalable_param(
                p_glb_pdb,
                p_ecode);

    ret_val = ICF_SUCCESS;


    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return(ret_val);
}/* end icf_dbm_stop */

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_deinit
 *
 * DESCRIPTION: This the function which would call other modules free function 
 *              to free all global data
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_deinit(
        INOUT icf_glb_pdb_st            *p_glb_pdb,
        OUT   icf_error_t               *p_ecode)
{

    icf_result_t          ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

ICF_CRM_START
	/* ICF_INT_MODULE_CFG module to be freed in last, as the other modules
	 * uses ICF_INT_MODULE_CFG for freeing scalable parameters
	 */
ICF_CRM_END

#if 0
	 if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_MWI])
    {
        icf_mwi_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_MWI],
                p_ecode);   
    }
#endif

#ifdef ICF_SUBSCRIBE_FEATURE
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_SUBM])
    {

        icf_subm_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SUBM],
                p_ecode);   
    }
#endif

    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_REGM])
    {
ICF_CRM_START


		icf_uint32_t			i; /* temp variable */
		icf_config_data_st   *p_config_data; 
		icf_rgm_context_st	*p_regm_data; 	
		icf_dbm_data_st   *p_dbm_data = ICF_NULL;
		
		/* get the config data pointer from DBM */
		ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_config_data),
                p_ecode);
		/* get the regm data pointer from DBM */
		ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_REGM,
                (icf_void_t *)&(p_regm_data),
                p_ecode);

		/* get the DBM data pointer from DBM */
		ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_DBM,
                (icf_void_t *)&(p_dbm_data),
                p_ecode);
				   
		/* de-allocate the static memory for p_rgn_uris and p_app_data_list; 
		* in each of the registration context */
		for (i=0; 
			i<p_config_data->max_registration_req;
			i++)
		{
			if (p_regm_data[i].p_app_data_list)
			{
				ICF_STATIC_MEMFREE( p_glb_pdb,
					(p_regm_data)[i].p_app_data_list,
					p_ecode, ret_val)
			}
		}
        icf_regm_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_REGM],
                p_ecode);   
    }
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_MPH])
    {

        icf_mph_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_MPH],
                p_ecode);   
    }
ICF_CHECK_IMS_END
#endif	
	if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_MMI])
    {
        icf_mmi_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_MMI],
                p_ecode);   
    }


    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_CFG])
    {

        icf_cfg_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG],
                p_ecode);   
    }

    
#ifdef ICF_SERVICE_ENABLE
#if !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_SYMBIAN)
	/* This to block the initialization of SCM module
	as IMS Client will not SCM */

    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_SCM])
    {

        icf_scm_deinit(p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SCM],
                p_ecode);
    }
#endif

#if defined ICF_CW_SERVICE || ICF_CHR_SERVICE || ICF_TWC_SERVICE || ICF_CTA_SERVICE || ICF_CTU_SERVICE

    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_CHR])
    {

        icf_chr_deinit(p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CHR],
                p_ecode);
    }

#endif
#endif
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_CC])
    {
        icf_cc_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CC],
                p_ecode);
    }

    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_CM])
    {  
        icf_cm_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CM],
                p_ecode);
    }

    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_SSA])
    {  
        icf_ssa_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SSA],
                p_ecode);
    }
#ifdef ICF_SIGCOMP_SUPPORT
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_SIGSA])
    {  
        icf_sigsa_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SSA],
                p_ecode);
    }
#endif
#ifdef ICF_SERVICE_ENABLE
#ifdef ICF_TWC_SERVICE
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_TWC])
    {
        icf_twc_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_TWC],
                p_ecode);
    }

#endif

#ifdef ICF_CTA_SERVICE
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_CTA])
    {
        icf_cta_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CTA],
                p_ecode);
    }
#endif

#ifdef ICF_CTU_SERVICE
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_CTU])
    {
        icf_ctu_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CTU],
                p_ecode);
    }
#endif

#ifdef ICF_RNW_SERVICE
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_RNW])
    {
        icf_rnw_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_RNW],
                p_ecode);
    }
#endif


#ifdef ICF_CFW_SERVICE
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_CFW])
    {
        icf_cfw_deinit(p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFW],
                p_ecode);
    }
#endif
#endif
#if defined ICF_TRACE_ENABLE || ICF_ERROR_ENABLE || ICF_STAT_ENABLED
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_SM])
    {
        icf_sm_deinit(
                p_glb_pdb,
                p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SM],
                p_ecode);   
    }
#endif


    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data
            [ICF_INT_MODULE_DBM])
    {
        icf_dbm_deinit_dbm_data(
                p_glb_pdb,
                (icf_dbm_data_st *)p_glb_pdb->p_glb_data->p_module_data[
                ICF_INT_MODULE_DBM],
                p_ecode);  
    }
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_glb_port_info)
    {
        icf_es_deinit(
                p_glb_pdb->p_glb_data->p_glb_port_info,
                p_ecode); 
   }

   ret_val = icf_cmn_static_memfree(p_glb_pdb,
                   p_glb_pdb->p_glb_data,p_ecode);
   p_glb_pdb->p_glb_data = ICF_NULL;

   ICF_FUNCTION_EXIT(p_glb_pdb)

   ICF_RESET_MODULE(p_glb_pdb)

   return (ret_val);

}/* End icf_dbm_deinit */


/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_deinit_scalable_param
 *
 *     DESCRIPTION      :    Clears all the memory pools held by DBM .
 *
 *     RETURNS          :    icf_return_t 
 *
 *************************************************************************/
icf_void_t
icf_dbm_deinit_scalable_param(
      INOUT icf_glb_pdb_st          *p_glb_pdb,
      OUT  icf_error_t              *p_ecode)
{
 	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_block_type_et           blk_type = 0;
    icf_dbm_pool_info_st  *p_pool_addr = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Clean up the stats data as it would be created afresh when system is 
     * reconfigured*/
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_stats_data)
    {
        icf_cmn_deinit_stats(
                p_glb_pdb,
                p_ecode);
    }

   /* While there are blocks left for which pool is to be created*/
    while(ICF_MAX_BLK_TYPE > blk_type)
    { 
        /*
         * Get the pool address for different pool
         */
        p_pool_addr = icf_dbm_get_pool_addr(
                p_glb_pdb,
                blk_type);

        /* use MEMGET function to allocate memory memory in advance for 
         * pool */
        ICF_STATIC_MEMFREE( p_glb_pdb,
                p_pool_addr->p_blk_array,
                p_ecode, ret_val)

            /* use MEMGET function to allocate memory in advance for storing
             * linked array for maintaining information about uused SC blocks */
        ICF_STATIC_MEMFREE( p_glb_pdb,
                p_pool_addr->p_available_blk_lst,
                p_ecode, ret_val)
        /*Initialize the unused block linked array*/
        
        blk_type += 1;
	}/* End while(ICF_MAX_BLK_TYPE >*/

	ICF_FUNCTION_EXIT(p_glb_pdb)

}/*End icf_dbm_init_dbm_data */

/*************************************************************************
 *
 *     FUNCTION NAME    :   icf_dbm_deinit_dbm_data
 *
 *     DESCRIPTION      :   This function frees global DBM's data
 *                          block.
 *
 *     RETURNS          :  icf_return_t
 * 
 *************************************************************************/
icf_void_t
icf_dbm_deinit_dbm_data(
      INOUT icf_glb_pdb_st          *p_glb_pdb,
      OUT  icf_dbm_data_st          *p_dbm_data,
      OUT  icf_error_t              *p_ecode)
{

#ifdef ICF_TRACE_ENABLE
    icf_return_t    ret_val = ICF_SUCCESS;
#endif
	
   	p_glb_pdb = p_glb_pdb; 

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_glb_pdb = p_glb_pdb;
    icf_cmn_static_memfree(p_glb_pdb,p_dbm_data,p_ecode);
    p_dbm_data = ICF_NULL;

	ICF_FUNCTION_EXIT(p_glb_pdb)
}/*End icf_dbm_init_dbm_data */
