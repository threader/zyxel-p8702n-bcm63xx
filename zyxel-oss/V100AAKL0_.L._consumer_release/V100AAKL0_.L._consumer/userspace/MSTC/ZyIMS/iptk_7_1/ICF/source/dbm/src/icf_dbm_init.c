/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_init.c
 *
 * Purpose          : This file contains functions that would implement 
 *                    functionality for initializing of data blocks blocks
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  14 DEC 2004   Sumit Gupta                File created and initial version
 *  12 APR 2005   Amit Sharma                Updated rfn_deinit to rnw_deinit
 *  19 APR 2005   Sumit Gupta     SPR 7402   Initialized the leftover 
 *                                           uninitialized fields in call ctx
 *                                           in the function 
 *                                           icf_dbm_init_call_ctx 
 *  12 APR 2005   Jyoti Narula               Added the init() for SM handler.
 *  27 APR 2005   Sumit Gupta                Updation for changes in 
 *                                           icf_common_types.h
 *  20 MAY 2005   Sumit Gupta    SPR 7584    Stats data would be created at the 
 *                                           time of init_scale_params
 *  24 MAY 2005   Sumit Gupta    SPR 7610    Initialized the newly added 
 *                                           rem_committed_mode field in 
 *                                           icf_call_ctx_st  
 * 13 JUN 2005    Sumit Gupta    SPR 7662    Initialized newly added pointer in
 *                                           call context
 * 23 JUN 2005   Jalaj Negi     IMS CLIENT
 * 25 JUL 2005	 Rohit Aggarwal  ICF		 Initialized new members of call_ctx
 * 17-Aug-2005	Rohit Aggarwal	ICF		     Init p_called_party_id in call ctx
 * 23-Aug-2005	Ashutos Rath  	ICF		     Added Init function for SIGSA 
 * 24-Aug-2005	Syed Ali Rizvi  ICF		     Added Init function for SIC 
 * 15-Sept-2005	Ashutos Rath  	ICF		     Added Memory Pool for SIGSA DICTIONARY
 * 07-Mar-2006	Anuradha Gupta	ICF			 changed the Invalid Event number
 * 08-Mar-2006     Aman Aggarwal             ICF merging with ICF2.0
 * 23-Mar-2006  Amit Sharma                  Code merged with ICF 4.2
 * 30-Mar-2006    Anuradha Gupta	ICF 5.0   Call Modify Feature changes
 * 15-Jul-2007    Neha Choraria         ICF 7.0   Initialized forked_resp_counter
 *                                                in call_ctx 
 * 31-Jul-2007   Amit Mahajan       SPR 15478
 * 20-Aug-2008    Puneet Sharma     CSR 1-6179301
 * 01-Jul-2009	 Aman Aggarwal		CSR 1-7294946 Changes for P-Early-Media mode
 * 29-Mar-2010  Kamal Ashraf                         CSR_1-8389294(Registaration Enh)
 * 09-Apr-2010   Alok Tiwari        SPR 20362     Modified Function
 *                                                icf_dbm_init_sic_ctx
 *
 * Copyright (c) 2010 Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_port.h"
#include "icf_common_types.h"
#include "icf_ssa_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_macro.h"

#include "icf_dbm_defs.h"
#include "icf_dbm_macro.h"
#include "icf_dbm_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_sc_prototypes.h"
#include "icf_dbm_regm_prototypes.h"
#include "icf_regm_defs.h"

#include "icf_port_prototypes.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif 


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_init
 *
 * DESCRIPTION: This the function which creates the non scalable global data   
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_init(
        INOUT icf_glb_pdb_st            *p_glb_pdb,
        OUT   icf_error_t               *p_ecode)
{

   icf_result_t          ret_val = ICF_SUCCESS;

       
   ICF_STATIC_MEMGET(
            p_glb_pdb,sizeof(icf_glb_data_st),p_glb_pdb->p_glb_data,
            ICF_RET_ON_FAILURE,p_ecode,ret_val)
    
   icf_cmn_trace_init(p_glb_pdb->p_glb_data);
   icf_cmn_app_trace_init(p_glb_pdb->p_glb_data);
   icf_dbm_init_glb_data(p_glb_pdb,p_ecode);

#ifdef ICF_ERROR_ENABLE
   if(ICF_FAILURE ==  icf_cmn_error_init(p_glb_pdb->p_glb_data))
   {
       ret_val = ICF_FAILURE;
   }
   else 
#endif
	   if (ICF_FAILURE == 
           icf_dbm_init_dbm_data(
               p_glb_pdb,
               (icf_dbm_data_st **)&p_glb_pdb->p_glb_data->p_module_data[
               (icf_int_module_id_t const)ICF_INT_MODULE_DBM],
               p_ecode))   
   {
       ret_val = ICF_FAILURE;  
   }
#if defined ICF_TRACE_ENABLE || ICF_ERROR_ENABLE || ICF_STAT_ENABLED || ICF_LICENSE_ENABLE
   else if (ICF_FAILURE == icf_sm_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SM],
               p_ecode) )
   {
       ret_val = ICF_FAILURE;
   }
#endif

   else if (ICF_FAILURE == 
                icf_cfg_init(
                    p_glb_pdb,
                   &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG],
                    p_ecode))   
   {
       ret_val = ICF_FAILURE;  
   }
#ifdef IMS_CLIENT
    else if ((((icf_config_data_st*)(p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG]))->ims_oprn_flag) && 
		(ICF_FAILURE == 
       icf_mph_init(
       p_glb_pdb,
       &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_MPH],
       p_ecode)))
   {
       ret_val = ICF_FAILURE;  
   }
#endif
   else if (ICF_FAILURE == 
       icf_regm_init(
       p_glb_pdb,
       &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_REGM],
       p_ecode))   
   {
       ret_val = ICF_FAILURE;  
   }
   else if (ICF_FAILURE == icf_es_init(
                &p_glb_pdb->p_glb_data->p_glb_port_info,
                p_ecode))   
   {
       ret_val = ICF_FAILURE;  
   }
   else if (ICF_FAILURE == icf_dbm_create_cmn_pool(
               p_glb_pdb,
               p_ecode))
   {
       ret_val = ICF_FAILURE;  
   }


#ifdef ICF_SERVICE_ENABLE
   /* this blocked because IMS Client is not using SCM */
   else if(ICF_FAILURE ==  icf_scm_init(p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SCM],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }


#if defined(ICF_CW_SERVICE) || defined(ICF_CHR_SERVICE) || defined(ICF_TWC_SERVICE) || defined(ICF_CTA_SERVICE) || defined(ICF_CTU_SERVICE)

   else if(ICF_FAILURE ==  icf_chr_init(p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CHR],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
#endif
#endif

   else if(ICF_FAILURE ==  icf_mmi_init(p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_MMI],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }

   else if(ICF_FAILURE ==  icf_cc_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CC],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }

   else if(ICF_FAILURE ==  icf_cm_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CM],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
   else if(ICF_FAILURE ==  icf_ssa_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SSA],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
   else if(ICF_FAILURE ==  icf_sic_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SIC],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT || defined ICF_QOS_SUPPORT
   else if(ICF_FAILURE ==  icf_nm_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_NM],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
#endif

#ifdef ICF_SIGCOMP_SUPPORT
   else if(ICF_FAILURE ==  icf_sigsa_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SIGSA],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
#endif
#ifdef ICF_SERVICE_ENABLE
/*  Changes For XCONF  */

#ifdef ICF_XCONF_SERVICE
    else if(ICF_FAILURE == icf_xconf_init(p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_XCONF],
               p_ecode))
    {
    	ret_val = ICF_FAILURE;
    }
#endif
/* CHANGES for XCONF End */

#ifdef ICF_TWC_SERVICE
   else if(ICF_FAILURE ==  icf_twc_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_TWC],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }

#endif

#ifdef ICF_CTA_SERVICE
   else if(ICF_FAILURE ==  icf_cta_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CTA],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
#endif

#ifdef ICF_CTU_SERVICE
   else if(ICF_FAILURE ==  icf_ctu_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CTU],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
#endif

#ifdef ICF_RNW_SERVICE
   else if(ICF_FAILURE ==  icf_rnw_init(
               p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_RNW],
               p_ecode))
   {
       ret_val = ICF_FAILURE;
   }
#endif

#ifdef ICF_CFW_SERVICE
   else if(ICF_FAILURE ==  icf_cfw_init(p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFW],
               p_ecode))
   {
      
       ret_val = ICF_FAILURE;
   }
#endif
#endif
#ifdef ICF_SUBSCRIBE_FEATURE
   else if(ICF_FAILURE ==  icf_subm_init(p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SUBM],
               p_ecode))
   {
      
       ret_val = ICF_FAILURE;
   }

#endif

#ifdef ICF_FEATURE_ABANDON
#if 0
   else if(ICF_FAILURE ==  icf_mwi_init(p_glb_pdb,
               &p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_MWI],
               p_ecode))
   {
      
       ret_val = ICF_FAILURE;
   }
#endif
#endif

	else if ( ICF_FAILURE == icf_port_hash_table_init())
	{
		ret_val = ICF_FAILURE;
	} 	
#if 0
	else if ( ICF_FAILURE == icf_port_create_numeric_hash_table(
                   ICF_INT_MODULE_CC,
                   &(((icf_dbm_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_DBM])
                   ->call_ctxt_hash_table_id),
                   ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_num_of_calls,
                   ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_num_of_calls,
                   p_ecode))
	{
		ret_val = ICF_FAILURE;
	}

	else if ( ICF_FAILURE == icf_port_create_string_hash_table(
                  ICF_INT_MODULE_SSA,
                  &(((icf_dbm_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_DBM])
                  ->ssa_ctxt_hash_table_id),
                  ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_sip_call_objs,
                  ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_sip_call_objs,
                  p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	
#endif
   if(ICF_FAILURE == ret_val)
   {
       /*Call deinit function which would call all module's deinit functions for
        * clearing up the memory*/
        icf_dbm_deinit(p_glb_pdb,p_ecode);
   }

   ICF_FUNCTION_EXIT(p_glb_pdb)

   return (ret_val);

}/* End icf_dbm_init */


/************************************************************************
 *
 * FUNCTION NAME:   icf_dbm_get_sys_scale_factor
 *
 * DESCRIPTION:     This function returns the system scaling factor value
 *                  corresponding to the configured resource limit.
 *
 * RETURNS:         System resource limit scaling factor
 *
 ************************************************************************/
icf_uint32_t icf_dbm_get_sys_scale_factor(
        IN    icf_glb_pdb_st                     *p_glb_pdb)
{
    icf_uint32_t            sys_scaling_factor = 
                                ICF_SYS_SCALING_FACTOR_DEFAULT;

    switch (p_glb_pdb->sys_resource_limit)
    {
        case 1:
            /* scaling factor will be 1 */
            sys_scaling_factor = ICF_SYS_SCALING_FACTOR_1_0;
            break;
            
        case 2:
            /* scaling factor will be 1.5 */
            sys_scaling_factor = ICF_SYS_SCALING_FACTOR_1_5;
            break;

        case 3:
            /* scaling factor will be 2 */
            sys_scaling_factor = ICF_SYS_SCALING_FACTOR_2_0;
            break;

        case 4:
            /* scaling factor will be 3 */
            sys_scaling_factor = ICF_SYS_SCALING_FACTOR_3_0;
            break;

        default:
            /* scaling factor will be 1 */
            sys_scaling_factor = ICF_SYS_SCALING_FACTOR_DEFAULT;
    } /* switch-case */

    return sys_scaling_factor;
} /* End icf_dbm_get_sys_scale_factor */


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_init_scalable_param
 *
 * DESCRIPTION: This the function which creates the global data structures  
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t 
icf_dbm_init_scalable_param(
        INOUT icf_glb_pdb_st                     *p_glb_pdb,
        IN    icf_dbm_scale_data_init_st         *p_init_param, 
        OUT   icf_error_t                        *p_ecode)

{
    icf_result_t      ret_val = ICF_SUCCESS;
    icf_dbm_data_st   *p_dbm_data = ICF_NULL;
    icf_uint16_t       blk_count = 0;
    icf_void_t        *p_blk_addr = ICF_NULL;
    icf_dbm_pool_info_st  *p_pool = ICF_NULL;
	icf_config_data_st     *p_config_data; 
	icf_uint32_t      sys_scaling_factor = ICF_SYS_SCALING_FACTOR_DEFAULT;	

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)       

    ICF_FUNCTION_ENTER(p_glb_pdb)

     ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
	
	sys_scaling_factor = icf_dbm_get_sys_scale_factor(p_glb_pdb);
	 
	 ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                p_ecode);

	 ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

     /* 
      * The function would set various pool sizes and block sizes for various
      * DBM pools in global DBM data
      */ 
     if(ICF_FAILURE == icf_dbm_set_blk_n_pool_size(
        p_glb_pdb,
        p_init_param, 
        p_ecode))
     {
         ret_val = ICF_FAILURE;
     }

	/*SIC  context init function to reseving blocks for storing sic
     * contexts*/
    if (ICF_FAILURE == icf_dbm_init_sic_cntxt_blk(
                p_glb_pdb,
                ICF_CMN_SIC_MAX_CALL_CONTEXTS,
                &p_dbm_data->p_available_sic_cntxt_lst,
                (icf_sic_ctx_st **)&p_dbm_data->p_sic_cntxt_blk,
                &p_dbm_data->sic_cntxt_blk_next_free_id,
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
	

ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
	 /*Call network access context init function to reserve blocks for storing 
	 * network access contexts*/
	 else if (ICF_FAILURE == icf_dbm_init_netact_cntxt_blk(
														p_glb_pdb,
														p_config_data->max_networks,
	 /* + 1 done so that ICF_GENERIC_APP_ID can also call 
	 * network_activate/network_deactivate */
	 (icf_uint16_t)(p_config_data->max_applications + 1),
	 (icf_netact_ctxt_st **)&p_dbm_data->p_netact_ctxt_blk,
								p_ecode))
	 {
		 ret_val = ICF_FAILURE;
	 }
#endif
	 /*Call application context init function to reserve blocks for storing 
     * application contexts*/
	 else if (ICF_FAILURE == 
				icf_dbm_init_app_conf_blk(
									p_glb_pdb,
									p_config_data->max_applications,
									(icf_app_conf_st **)&p_dbm_data->p_app_conf,
									p_ecode))
	 {	
		 ret_val = ICF_FAILURE;
	 }
ICF_CRM_END


     /* 
      * Once the block sizes and pools sizes are initialized into DBM's global
      * data, this function would create the actual pools
      */ 
     else if(ICF_FAILURE == icf_dbm_create_pools(
                 p_glb_pdb,
                 p_ecode))
     {
         ret_val = ICF_FAILURE; 
     }

    /*Call line block init function to reseving pointers for storing
     *line data*/
    else if(ICF_FAILURE == icf_dbm_init_line_blk(
        p_glb_pdb,
        (icf_uint16_t const)p_init_param->max_num_of_lines,
        &p_glb_pdb->p_glb_data->p_p_line_data,
        p_ecode))
    {
        ret_val = ICF_FAILURE;
    }

    else if(ICF_FAILURE == icf_dbm_create_task_pool(
                p_glb_pdb,
                p_init_param,        
                p_ecode))
    {
        ret_val = ICF_FAILURE;          
    }
    else if(ICF_FAILURE ==
            icf_port_timer_init(
               p_glb_pdb->p_glb_data->p_glb_port_info,         
               (icf_timer_cb_fn_t)icf_mrm_timer_cb_fn,
               (icf_uint8_t)ICF_NUM_OF_TIMER_LIST,
               (icf_uint16_t)((p_init_param->max_num_of_timers * sys_scaling_factor)/2),
               p_ecode))
    {
        ret_val = ICF_FAILURE;          
    }
    /*Create Stats data*/
    else if(ICF_FAILURE ==  icf_cmn_init_stats(
                p_glb_pdb,
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Initialize the head pointer of used node list for registration block
         * It should be equal to max_number_of registrations*/
        p_dbm_data->rgn_cntxt_blk_next_used_id = 
            p_init_param->max_num_of_registrations;
		
		/* 
         * Get the address of the pool for call context
         */
        p_pool =  icf_dbm_get_pool_addr(
                p_glb_pdb,
                ICF_BLK_TYPE_CALL_CTX);
        
		ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pool))

        /*
         *  Initialize all the call context blocks with INVALID_CALL_ID
         */
        while(blk_count < p_init_param->max_num_of_calls)
        {
            p_blk_addr = 
                (icf_call_ctx_st *)(p_pool->p_blk_array) +  blk_count;
            
           ((icf_call_ctx_st *)p_blk_addr)->call_id = ICF_INVALID_CALL_ID;
            blk_count += 1;
        }
    }

   if ( ICF_FAILURE == icf_port_create_numeric_hash_table(
                   ICF_INT_MODULE_CC,
                   &(((icf_dbm_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_DBM])
                   ->call_ctxt_hash_table_id),
                   ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_num_of_calls,
                   ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_num_of_calls,
                   p_ecode))
	{
		ret_val = ICF_FAILURE;
	}

	else if ( ICF_FAILURE == icf_port_create_string_hash_table(
                  ICF_INT_MODULE_SSA,
                  &(((icf_dbm_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_DBM])
                  ->ssa_ctxt_hash_table_id),
                  ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_sip_call_objs,
                  ((icf_config_data_st*)p_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG])->max_sip_call_objs,
                  p_ecode))
	{
		ret_val = ICF_FAILURE;
	}

    if(ICF_FAILURE == ret_val)
    {
        icf_dbm_deinit_scalable_param(
                p_glb_pdb,
                p_ecode);
    }
 
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return(ret_val);
    
}/* End icf_dbm_init_scalable_param */

#if 0
/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_init_srvc_cntxt_blk
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the service context blocks.
 *
 *     RETURNS          :   icf_return_t  
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        IN    icf_uint16_t                    num_of_blks,
        OUT   icf_void_t                      **p_p_unused_srvc_cntxt_blk,
        OUT   icf_dbm_srvc_cntxt_la_st        **p_p_srvc_cntxt_blk_la,
        OUT   icf_dbm_srvc_cntxt_blk_id_t     *p_srvc_cntxt_blk_next_free_id,
        OUT   icf_error_t                     *p_ecode)
{
 
    icf_return_t          ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* use MEMGET function to allocate memory memory in advance for storing
     * service context data block */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_service_ctx_st) * num_of_blks,        
            (icf_void_t *)*p_p_unused_srvc_cntxt_blk,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)

    /* use MEMGET function to allocate memory memory in advance for storing
    * linked array for maintaining information about uused SC blocks */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_dbm_la_st) * num_of_blks,
           (icf_dbm_la_st *)*p_p_srvc_cntxt_blk_la,
            ICF_DONOT_RET_ON_FAILURE,
            p_ecode, ret_val)
    
    if(ICF_FAILURE == ret_val)    
    {
        ICF_STATIC_MEMFREE( p_glb_pdb,
             *p_p_unused_srvc_cntxt_blk,
             p_ecode, ret_val)
        ret_val =ICF_FAILURE;
    }
    else
    {
        /*Initialize the unused block linked array*/
        if ( ICF_FAILURE == 
                icf_dbm_la_init(num_of_blks,*p_p_srvc_cntxt_blk_la,
          p_srvc_cntxt_blk_next_free_id, p_ecode))         
        {
            ret_val= ICF_FAILURE;
        }/*End if ( ICF_FAILURE == ret_val)*/
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return (ret_val);

}/* End icf_dbm_init_srvc_cntxt_blk */



/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_init_srvc_cntxt_lgc_blk
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the service context logic blocks.
 *
 *     RETURNS          :  icf_return_t  
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_srvc_cntxt_lgc_blk(
    INOUT icf_glb_pdb_st                  *p_glb_pdb,
    IN    icf_uint16_t                    num_of_blks,
    OUT   icf_void_t                      **p_p_unused_srvc_cntxt_lgc_blk,
    OUT   icf_dbm_srvc_cntxt_lgc_la_st    **p_p_srvc_cntxt_lgc_blk_la,
    OUT   icf_dbm_srvc_cntxt_lgc_blk_id_t *p_srvc_cntxt_lgc_blk_next_free_id,
    OUT   icf_error_t                     *p_ecode)
{

    icf_return_t        ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)  

    /* use MEMGET function to allocate memory in advance for storing
     * service context logic data block */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_srvc_ctx_lgc_ut) * num_of_blks,        
            *p_p_unused_srvc_cntxt_lgc_blk,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)

    /* use MEMGET function to allocate  memory in advance for storing    
     *  linked array for maintaining information about uused SCL blocks */
    
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_dbm_la_st) * num_of_blks,
           (icf_dbm_la_st *)*p_p_srvc_cntxt_lgc_blk_la,
            ICF_DONOT_RET_ON_FAILURE,
            p_ecode, ret_val)

    if(ICF_FAILURE == ret_val)
    {
         ICF_STATIC_MEMFREE( p_glb_pdb,
             *p_p_unused_srvc_cntxt_lgc_blk,
             p_ecode, ret_val)

         ret_val = ICF_FAILURE;
    }
    else     
    {
        /*Initialize the unused block linked array*/
        if(ICF_FAILURE ==
                icf_dbm_la_init(num_of_blks,*p_p_srvc_cntxt_lgc_blk_la,
           p_srvc_cntxt_lgc_blk_next_free_id, p_ecode))
        {
            ret_val = ICF_FAILURE;
            /*Function assumed to print trace*/
        }
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return (ret_val);

}/*End icf_dbm_init_srvc_cntxt_lgc_blk */




/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_init_rgn_cntxt_blk
 *
 *     DESCRIPTION      :    This function allocates pool for registration 
 *                           context blocks and initializes the linked array 
 *                           structure for the service context logic blocks..
 *
 *     RETURNS          :  icf_return_t 
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_rgn_cntxt_blk(
  INOUT icf_glb_pdb_st                        *p_glb_pdb,
  IN    icf_uint16_t                          num_of_blks,
  OUT   icf_void_t                            **p_p_rgn_cntxt_blk,
  OUT                   **p_p_rgn_cntxt_blk_la,
  OUT   icf_dbm_rgn_cntxt_blk_id_t            *p_rgn_cntxt_blk_next_free_id,
  OUT   icf_error_t                           *p_ecode)
{

    icf_return_t           ret_val = ICF_SUCCESS;
ICF_CRM_START
	icf_uint32_t			  i; /* temp variable */
	icf_config_data_st     *p_config_data; 
	icf_rgm_context_st	  *p_rgn_cntxt; 
ICF_CRM_END

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* use MEMGET function to allocate memory memory in advance for storing
     * call context data block */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_rgm_context_st) * num_of_blks,        
            *p_p_rgn_cntxt_blk,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)


    /* use MEMGET function to allocate memory memory in advance for storing    
     * linked array for maintaining information about uused CC blocks */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_dbm_la_st) * num_of_blks,
           (icf_dbm_la_st *)*p_p_rgn_cntxt_blk_la,
            ICF_DONOT_RET_ON_FAILURE,
            p_ecode, ret_val)

	if ( ret_val == ICF_SUCCESS )
	{
		/*Initialize the unused block linked array*/    
        if (ICF_FAILURE ==
                icf_dbm_la_init(num_of_blks,*p_p_rgn_cntxt_blk_la,
           p_rgn_cntxt_blk_next_free_id, p_ecode))
        {
                ret_val = ICF_FAILURE;
        }
	}
	else
	{
		ICF_STATIC_MEMFREE( p_glb_pdb,
			*p_p_rgn_cntxt_blk,
            p_ecode, ret_val)
       
		ret_val = ICF_FAILURE;
	}
    
ICF_CRM_START
	p_rgn_cntxt = (icf_rgm_context_st	  *)*p_p_rgn_cntxt_blk; 
	if ( ret_val == ICF_SUCCESS )
	{

		/* get the config data pointer from DBM */
		ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&(p_config_data),
                p_ecode);

			
		/* allocate the static memory for p_rgn_uris and p_app_data_list; 
		* in each of the registration context */
		for (i=0; i< num_of_blks && ret_val != ICF_FAILURE; i++)
		{
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
			(p_rgn_cntxt)[i].p_rgm_uris = ICF_NULL; 
			ICF_STATIC_MEMGET( p_glb_pdb,
				sizeof(icf_address_st) * p_config_data->max_users_per_line,
				((p_rgn_cntxt)[i].p_rgm_uris),
				ICF_DONOT_RET_ON_FAILURE,
				p_ecode, ret_val)
ICF_CHECK_IMS_END
#endif
			(p_rgn_cntxt)[i].p_app_data_list = ICF_NULL; 
			ICF_STATIC_MEMGET( p_glb_pdb,
				sizeof(icf_rgm_app_ctxt_st) * p_config_data->max_applications,
				((p_rgn_cntxt)[i].p_app_data_list),
				ICF_DONOT_RET_ON_FAILURE,
				p_ecode, ret_val)
	
		}
		if ( ret_val == ICF_FAILURE )
		{
			/* free p_rgn_uris and p_app_data_list */
			for (i=0; i<num_of_blks; i++)
			{
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
				if ( (p_rgn_cntxt)[i].p_rgm_uris )
				{
					ICF_STATIC_MEMFREE( p_glb_pdb,
						(p_rgn_cntxt)[i].p_rgm_uris,
						p_ecode, ret_val)
				}
ICF_CHECK_IMS_END
#endif
				if ((p_rgn_cntxt)[i].p_app_data_list)
				{
					ICF_STATIC_MEMFREE( p_glb_pdb,
						(p_rgn_cntxt)[i].p_app_data_list,
						p_ecode, ret_val)
				}
			}
			ICF_STATIC_MEMFREE( p_glb_pdb,
				p_rgn_cntxt,
				p_ecode, ret_val)
			ICF_STATIC_MEMFREE( p_glb_pdb,
				*p_p_rgn_cntxt_blk_la,
				p_ecode, ret_val)
			ret_val = ICF_FAILURE; 
		}
	}
ICF_CRM_END

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return (ret_val);
}/* End icf_dbm_init_rgn_cntxt_blk */
#endif

/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_init_line_blk
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the registration blocks.
 *
 *     RETURNS          :  icf_return_t  
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_line_blk(
       INOUT icf_glb_pdb_st                     *p_glb_pdb,
       IN    icf_uint16_t                       num_of_blks,
       OUT   icf_line_data_st                   ***p_p_p_line_data,
       OUT   icf_error_t                        *p_ecode)
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_uint16_t counter_i = 0;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Create pointer array for maximum_num_of_lines_possible*/
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_line_data_st **) * num_of_blks,        
            *p_p_p_line_data,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)

    /*Initialize all pointers to Null value*/
    while(counter_i < num_of_blks)
    {
        p_glb_pdb->p_glb_data->p_p_line_data[counter_i] = ICF_NULL;
        counter_i += 1;
    }


    ICF_FUNCTION_EXIT(p_glb_pdb)

    return (ret_val) ;

}/* End icf_dbm_init_line_blk */


/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_init_call_cntxt_blk
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the call context blocks.
 *
 *     RETURNS          :   icf_return_t 
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_call_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_uint16_t                    num_of_blks,
       INOUT    icf_dbm_call_cntxt_la_st        **p_p_call_cntxt_blk_la,
       OUT   icf_call_ctx_st                 **p_p_call_context_data, 
       OUT   icf_dbm_call_cntxt_blk_id_t     *p_call_cntxt_blk_next_free_id,
       OUT   icf_error_t                     *p_ecode)
{

    icf_return_t       ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* use MEMGET function to allocate memory memory in advance for storing
     * call context data block */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_call_ctx_st) * num_of_blks,        
            *p_p_call_context_data,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)

    /* use MEMGET function to allocate memory memory in advance for storing    
     * linked array for maintaining information about uused CC blocks */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_dbm_la_st) * num_of_blks,
           *p_p_call_cntxt_blk_la,
            ICF_DONOT_RET_ON_FAILURE,
            p_ecode, ret_val)
    
    
    if (ICF_FAILURE == ret_val)
    {
        ICF_STATIC_MEMFREE( p_glb_pdb,
             *p_p_call_context_data,
             p_ecode, ret_val)

        ret_val = ICF_FAILURE;
    }
    else
    {
        /*Initialize the unused block linked array*/    
        if (ICF_FAILURE == 
                icf_dbm_la_init(num_of_blks,
                (icf_dbm_la_st *)*p_p_call_cntxt_blk_la,
                p_call_cntxt_blk_next_free_id, p_ecode))
        {
            ret_val =ICF_FAILURE;
        }
		else
		{
			icf_uint16_t			i =0; 
			icf_call_ctx_st      *p_call_ctx = (*p_p_call_context_data);
			for(i=0;i<num_of_blks;i++)
			{
				icf_dbm_init_call_ctx(p_glb_pdb,p_call_ctx);
				p_call_ctx++;
			}
		}

    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return (ret_val);
}/* End icf_dbm_init_call_cntxt_blk */


/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_init_sic_cntxt_blk
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the sic context blocks.
 *
 *     RETURNS          :   icf_return_t 
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_sic_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_uint16_t                    num_of_blks,
       INOUT    icf_dbm_sic_cntxt_la_st        **p_p_sic_cntxt_blk_la,
       OUT   icf_sic_ctx_st                 **p_p_sic_context_data, 
       OUT   icf_dbm_sic_cntxt_blk_id_t     *p_sic_cntxt_blk_next_free_id,
       OUT   icf_error_t                     *p_ecode)
{

    icf_return_t       ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* use MEMGET function to allocate memory memory in advance for storing
     * call context data block */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_sic_ctx_st) * num_of_blks,        
            *p_p_sic_context_data,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)

    /* use MEMGET function to allocate memory memory in advance for storing    
     * linked array for maintaining information about uused CC blocks */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_dbm_la_st) * num_of_blks,
           *p_p_sic_cntxt_blk_la,
            ICF_DONOT_RET_ON_FAILURE,
            p_ecode, ret_val)
    
    
    if (ICF_FAILURE == ret_val)
    {
        ICF_STATIC_MEMFREE( p_glb_pdb,
             *p_p_sic_context_data,
             p_ecode, ret_val)

        ret_val = ICF_FAILURE;
    }
    else
    {
        /*Initialize the unused block linked array*/    
        if (ICF_FAILURE == 
                icf_dbm_la_init(num_of_blks,
                (icf_dbm_la_st *)*p_p_sic_cntxt_blk_la,
                p_sic_cntxt_blk_next_free_id, p_ecode))
        {
            ret_val =ICF_FAILURE;
        }
		else
		{
			icf_uint16_t i =0; 
			icf_sic_ctx_st          *p_sic_ctx= (*p_p_sic_context_data);
			for(i=0;i<num_of_blks;i++)
			{
				icf_dbm_init_sic_ctx(p_glb_pdb,p_sic_ctx);
				p_sic_ctx++;
			}	
		}
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return (ret_val);
}/* End icf_dbm_init_sic_cntxt_blk */


/*************************************************************************
 *
 *     FUNCTION NAME    :   icf_dbm_init_dbm_data
 *
 *     DESCRIPTION      :   This function initializes global DBM's data
 *                          block.
 *
 *     RETURNS          :  icf_return_t
 * 
 *************************************************************************/
icf_return_t
icf_dbm_init_dbm_data(
      INOUT icf_glb_pdb_st          *p_glb_pdb,
      OUT  icf_dbm_data_st          **p_p_dbm_data,
      OUT  icf_error_t              *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
	icf_void_t *p_buf = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Allocate memory for DBM data block */
    ICF_STATIC_MEMGET(p_glb_pdb,
            sizeof(icf_dbm_data_st),
            p_buf,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)
   *p_p_dbm_data = (icf_dbm_data_st *)p_buf;
    /*
     * Initialize all bytes in dbm data to 0
     */
    icf_port_memset(p_buf,(icf_uint8_t const)0,
            (icf_uint32_t const)sizeof(icf_dbm_data_st));

    (*p_p_dbm_data)->last_opt_ctx_id_ret = ICF_DBM_OPT_CTX_ID_START;

    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return(ret_val);

}/*End icf_dbm_init_dbm_data */



/*************************************************************************
 *
 *     FUNCTION NAME    :   icf_dbm_init_glb_data
 *
 *     DESCRIPTION      :   This function initializes global data
 *                          block to default values .
 *
 *     RETURNS          :  Nothing
 * 
 *************************************************************************/
icf_void_t
icf_dbm_init_glb_data(
      INOUT icf_glb_pdb_st          *p_glb_pdb,
      OUT  icf_error_t              *p_ecode)
{
    icf_uint8_t count_module = 0;
#ifdef ICF_TRACE_ENABLE     
    icf_return_t ret_val = ICF_SUCCESS;
#endif
 
    /* This is not invoked as sm data has not been initalized yet */  
    /* ICF_FUNCTION_ENTER(p_glb_pdb)*/
     
    p_glb_pdb->p_glb_data->p_p_line_data = ICF_NULL;

    p_glb_pdb->p_glb_data->p_stats_data = ICF_NULL;
    
    while(ICF_INT_MAX_MODULE > count_module)
    {
        p_glb_pdb->p_glb_data->p_module_data[count_module] = ICF_NULL;
        count_module += 1;
    }
      
    p_glb_pdb->p_glb_data->global_data_init_complete = ICF_FALSE;
 
    p_glb_pdb->p_glb_data->p_glb_port_info = ICF_NULL;   

    p_ecode = p_ecode; /*To remove warning*/ 

    ICF_FUNCTION_EXIT(p_glb_pdb)    

}

/******************************************************************************  *
 *
 * FUNCTION:        icf_dbm_init_call_ctx
 *
 * DESCRIPTION:     This is a utility function that initializes call context
 *                  with default values.
 *
 ******************************************************************************/
icf_return_t icf_dbm_init_call_ctx(
        INOUT icf_glb_pdb_st    *p_glb_pdb,
        INOUT icf_call_ctx_st   *p_call_ctx)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t                      ctr;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_error_t                      ecode;
 
    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    ret_val = icf_dbm_get_module_glb_data(     
            p_glb_pdb,
            (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
            (icf_void_t *)&p_config_data,
            &ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /* **** Set the fields of call context **** */

    p_call_ctx->line_id = ICF_INVALID_LINE_ID;
 
	p_call_ctx->service_line_id = ICF_INVALID_LINE_ID;

    /* reset call state to idle */
    p_call_ctx->call_state = ICF_CC_00_IDLE;  
    
    /* reset the next state */
    p_call_ctx->next_state = ICF_CC_18_CALL_STATE_INVALID;

    /* reset the previous state */
    p_call_ctx->prev_state = ICF_CC_18_CALL_STATE_INVALID;
    
    /* reset the replace init flag */
    p_call_ctx->replace_initiated_call_id = ICF_INVALID_CALL_ID;

    /* list of stream-level sdp */
    p_call_ctx->p_stream_sdp_info = ICF_NULL;

    /*  the referred party add */
    p_call_ctx->p_referred_party_addr = ICF_NULL;

    /*  the referred party name */
    p_call_ctx->p_referred_party_name = ICF_NULL;
   
    /*  the nw call id recevied in replace header in refer */
    p_call_ctx->p_replaces_header = ICF_NULL;

    /*  the remote received sdp */
    p_call_ctx->p_diversion_header_list = ICF_NULL;

    /*  the sip ctx */
    p_call_ctx->p_sip_ctx = ICF_NULL;

    /*  the timer data block  */
    p_call_ctx->p_timer_data = ICF_NULL;
    p_call_ctx->p_info_timer_data = ICF_NULL;
    p_call_ctx->p_info_cfm_timer_data = ICF_NULL;
    p_call_ctx->p_retry_after_timer_data = ICF_NULL;

    /*  the service  data block  */
    p_call_ctx->p_service_ctx = ICF_NULL;

    /*Get the qos value from config data */
    p_call_ctx->qos_mark = p_config_data->qos_mark;

    /*get the value from config data */
    p_call_ctx->common_bitmask = ICF_NULL;
    p_call_ctx->common_bitmask_2 = ICF_NULL;
    p_call_ctx->call_to_be_joined = ICF_INVALID_CALL_ID;

    /* method for which Re-invite was sent */
    p_call_ctx->call_modify_method_bitmask = ICF_NULL;

    /*Init remote party address */
    icf_cmn_init_address(&p_call_ctx->remote_party_addr);
        
    /*Init remote party name */
    icf_cmn_init_string((icf_string_st *)
            (&p_call_ctx->remote_party_name));
    
    /*Init local address*/
    icf_cmn_init_address(&p_call_ctx->local_address);

    /*Init local name */
    icf_cmn_init_string((icf_string_st *)(&p_call_ctx->local_name));

    /* reset the timer id for all timers in the list */
    for (ctr=0; ctr<ICF_CC_NUMBER_OF_TIMERS; ctr++)
    {
        p_call_ctx->timer_list.timer_id[ctr] =
            ICF_TIMER_ID_INVALID;
    }

   p_call_ctx->info_timer_id = ICF_TIMER_ID_INVALID;
   p_call_ctx->info_cfm_timer_id = ICF_TIMER_ID_INVALID;
   p_call_ctx->retry_after_timer_id = ICF_TIMER_ID_INVALID;

    /* reset the release type bitmask */
    p_call_ctx->release_type = ICF_NULL;

    /* reset the release cause */
    p_call_ctx->release_cause = ICF_REL_CAUSE_INVALID;
    /* reset PA initiated call clear response code */
    p_call_ctx->response_code = ICF_NULL;  
    /* reset Remote initiated call clear response code */
    p_call_ctx->inc_response_code = ICF_NULL;

    p_call_ctx->p_response_str = ICF_NULL;
    
    p_call_ctx->call_clear_status = ICF_NULL;

    /*Set call clear status mark MM,PA and NW cleared*/

    /*Mark PA cleared in call clear status*/
    p_call_ctx->call_clear_status |= 
                                    ICF_CALL_CLEAR_STATUS_PA_CLEARED;
    
    /*Mark MM cleared in call clear status*/
    p_call_ctx->call_clear_status |= 
                                    ICF_CALL_CLEAR_STATUS_MM_CLEARED;

    /*Mark NW cleared in call clear status*/
    p_call_ctx->call_clear_status |= 
                                    ICF_CALL_CLEAR_STATUS_NW_CLEARED;


    /* reset the media ongoing flag */
    p_call_ctx->media_ongoing = ICF_INT_MODULE_INVALID;

    /* reset the signaling ongoing flag */
    p_call_ctx->signalling_ongoing = ICF_INT_MODULE_INVALID;

    /* reset the pending event bitmask */
    p_call_ctx->pending_event = ICF_NULL;

    /* reset the ongoing event */
    p_call_ctx->ongoing_event = ICF_NULL;

    /* reset the media committed flag */
    p_call_ctx->media_committed = ICF_FALSE;

    /* reset the media updated flag */
    p_call_ctx->media_updated = ICF_FALSE;

    /* reset the media state bitmask */
    p_call_ctx->media_state = ICF_NULL;

    /* reset the call  state bitmask */
    p_call_ctx->call_status = ICF_NULL;

    /* reset the remote mode change ind */
    /*p_call_ctx->remote_mode_change_ind = ICF_FALSE;*/

    /* reset the inband info flag */
    p_call_ctx->inband_info_present = ICF_FALSE;

    /* reset the pointer to timer data */
    p_call_ctx->p_timer_data = ICF_NULL;

    /* reset the remote assert flag */
    p_call_ctx->remote_aserted_address = ICF_FALSE;
    
    /* reset the inband info flag */
    p_call_ctx->inband_info_present = ICF_FALSE;
    
    /* reset the inband info flag */
    p_call_ctx->call_waiting_call = ICF_FALSE;

    /*Set mmi_bitmask in mmi context to NULL*/
    p_call_ctx->mmi_context.mmi_bitmask = ICF_NULL;
    p_call_ctx->mmi_context.merge_call_id = ICF_INVALID_CALL_ID;
    /*p_call_ctx->mmi_context.p_codec_for_commit = ICF_NULL;*/
    p_call_ctx->mmi_context.mmi_state = ICF_NULL;
    p_call_ctx->mmi_context.mmi_event = ICF_NULL;
    p_call_ctx->mmi_context.p_remote_recvd_sdp = ICF_NULL;
    p_call_ctx->mmi_context.p_local_stream_addr = ICF_NULL;
    p_call_ctx->mmi_context.p_media_session_req = ICF_NULL;

    /*Set other conference call pointer to NULL*/
    p_call_ctx->p_conf_other_call_ctx = ICF_NULL;
	
    p_call_ctx->pending_info_resp = ICF_FALSE;
	
    /* Initialize contact address as ICF_NULL */
    p_call_ctx->p_contact_address = ICF_NULL;
	
    p_call_ctx->prov_resp_status = ICF_NULL;
	/* reset the call type */
	p_call_ctx->call_type = ICF_NULL; 

    p_call_ctx->update_reqd = ICF_FALSE;

#ifdef ICF_QOS_SUPPORT
    p_call_ctx->received_successful_prack_resp = ICF_FALSE;
    p_call_ctx->resource_reservation_completed = ICF_FALSE;
    
    p_call_ctx->percent_alloc_to_rs = ICF_PERCENT_BW_ALLOC_TO_RS;
    p_call_ctx->percent_alloc_to_rr = ICF_PERCENT_BW_ALLOC_TO_RR;
#endif

ICF_SESSION_CONTROL_START
	/* reset the app id */
	p_call_ctx->app_id = ICF_APP_ID_INVALID;


	/* reset the privacy header indication flag */
	p_call_ctx->send_privacy_hdr = ICF_FALSE;

	/* reset the preferred identity */
	p_call_ctx->p_preferred_identity = ICF_NULL;

	/* reset the asserted identity */
	p_call_ctx->p_asserted_identity = ICF_NULL;

	/* reset the called party identity */
	p_call_ctx->p_called_party_id = ICF_NULL;

	/* reset the list of additional headers */
	p_call_ctx->header_list.count = ICF_NULL;
	p_call_ctx->header_list.hdr = ICF_NULL;

	/* reset the list of additional tags */
	p_call_ctx->tag_list.count = ICF_NULL;
	p_call_ctx->tag_list.tag = ICF_NULL;

	/* reset the list of additional message bodies */
	p_call_ctx->body_list.count = ICF_NULL;
	p_call_ctx->body_list.body = ICF_NULL;

ICF_SESSION_CONTROL_END
      
	/* reset the list of persistent headers */
	p_call_ctx->persistent_header_list.count = ICF_NULL;
	p_call_ctx->persistent_header_list.hdr = ICF_NULL;

        p_call_ctx->rejected_stream_count = ICF_NULL;

#ifdef ICF_FAX_SUPPORT
	p_call_ctx->fax_type = ICF_NULL;
	p_call_ctx->pref_codec = ICF_NULL;
#endif

	p_call_ctx->sdp_changed = ICF_FALSE;
	p_call_ctx->update_reqd = ICF_FALSE;

	p_call_ctx->offer_failure_reason = ICF_NULL;
	p_call_ctx->app_failure_cause = ICF_NULL;

    /* initialize the forceful call clear flag */
	p_call_ctx->force_clr_rcvd = ICF_FALSE;
    /* initialize the modify session request method allowed by remote flag*/
        p_call_ctx->allowed_value =  ICF_NULL; 

        p_call_ctx->echo_canc = ICF_FALSE;

	p_call_ctx->stop_fax_resp_recvd = ICF_FALSE;

        p_call_ctx->forked_resp_counter = ICF_NULL;

        /* SPR 17626: Initialize p_initial_sip_ctx to NULL in call_context.
        */
        p_call_ctx->p_initial_sip_ctx = ICF_NULL;

#ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING
    /* Init alternate called party address for an incoming call */
    icf_cmn_init_address(&p_call_ctx->alternate_local_address);
#endif

#ifdef ICF_TRACE_ENABLE
    p_call_ctx->p_trace_criteria_data = ICF_NULL;
#endif

    p_call_ctx->retry_after_duration = ICF_NULL;
    p_call_ctx->fax_method = ICF_FAX_METHOD_REPLACE_STREAM;

#ifdef ICF_SESSION_TIMER
       if (ICF_TRUE == p_config_data->session_timer_enable)
       {
          p_call_ctx->session_timer_enable = ICF_TRUE; 
       }
       else
       {
          p_call_ctx->session_timer_enable = ICF_FALSE;
       }
       /* If session_timer_enable is TRUE then p_seesion_timer_data
        * should be allocated memory during incoming and outgoing call
        * setup.*/
       p_call_ctx->p_session_timer_data = ICF_NULL;
#endif
	/*Initialize the back up mode*/
 	p_call_ctx->recvd_media_attrib_backup = ICF_NULL;

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
        
    return ret_val;
}


/******************************************************************************  *
 *
 * FUNCTION:        icf_dbm_init_sic_ctx
 *
 * DESCRIPTION:     This is a utility function that initializes call context
 *                  with default values.
 *
 ******************************************************************************/
icf_return_t icf_dbm_init_sic_ctx(
	INOUT icf_glb_pdb_st    *p_glb_pdb,
        INOUT icf_sic_ctx_st   *p_sic_ctx)
{
     icf_return_t                     ret_val = ICF_SUCCESS;
 
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }   

	p_glb_pdb = p_glb_pdb;   

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /* **** Set the fields of call context **** */
    /*CSR 1-6179301: reset bitmask and other uninitialised parameters*/
    p_sic_ctx->bitmask = ICF_NULL;
    p_sic_ctx->refer_to.addr_type = 0;
    p_sic_ctx->refer_to.addr_val.str_len = 0;
    p_sic_ctx->refer_to.addr_val.str[0] = 0;
    p_sic_ctx->subs_reqd = ICF_FALSE;
    p_sic_ctx->persistent_header_list.count = ICF_NULL;
    p_sic_ctx->persistent_header_list.hdr = ICF_NULL;
    p_sic_ctx->p_sdp_list = ICF_NULL;
    p_sic_ctx->options_timer_id = ICF_TIMER_ID_INVALID;
    p_sic_ctx->p_options_timer = ICF_NULL;
    p_sic_ctx->p_mm_media_capab= ICF_NULL;
    p_sic_ctx->pending_event=ICF_NULL;
    p_sic_ctx->retry_after_duration = ICF_NULL;
    p_sic_ctx->method = ICF_NULL;

	p_sic_ctx->call_id = ICF_INVALID_CALL_ID;
	p_sic_ctx->event_buffer = ICF_FALSE;
    p_sic_ctx->line_id = (icf_line_id_t)ICF_INVALID_LINE_ID;
    p_sic_ctx->sic_state = ICF_SIC_00_IDLE;  
    p_sic_ctx->p_timer_data = ICF_NULL;
	p_sic_ctx->timer_type = ICF_NULL;
    p_sic_ctx->duration = ICF_NULL;
    p_sic_ctx->app_requested_duration = ICF_NULL;
    p_sic_ctx->event_package.str[0] = ICF_NULL;
    p_sic_ctx->timer_id = ICF_TIMER_ID_INVALID;
    p_sic_ctx->p_app_ctx = ICF_NULL;
    p_sic_ctx->p_ssa_ctx = ICF_NULL;
    p_sic_ctx->p_timer_data = ICF_NULL;
    p_sic_ctx->p_user_address = ICF_NULL;
	p_sic_ctx->dest_addr.addr_type = 0;
    p_sic_ctx->dest_addr.addr_val.str_len = 0;
	p_sic_ctx->dest_addr.addr_val.str[0] = 0;
	p_sic_ctx->app_id = ICF_APP_ID_INVALID;
	p_sic_ctx->p_preferred_identity = ICF_NULL;

	p_sic_ctx->header_list.count = ICF_NULL;
	p_sic_ctx->header_list.hdr = ICF_NULL;

	p_sic_ctx->tag_list.count = ICF_NULL;
	p_sic_ctx->tag_list.tag = ICF_NULL;

	p_sic_ctx->body_list.count = ICF_NULL;
	p_sic_ctx->body_list.body = ICF_NULL;
	p_sic_ctx->p_call_ctx = ICF_NULL;
	p_sic_ctx->entity_tag.str[0] = 0;
	p_sic_ctx->entity_tag.str_len=0;

	p_sic_ctx->header_list.count = ICF_NULL;
    p_sic_ctx->body_list.count = ICF_NULL;
    p_sic_ctx->tag_list.count = ICF_NULL;
    p_sic_ctx->error_code = ICF_NULL;

    /*CSR 1-6179301*/
    /*SPR18430 initialize the flag for detaching
      sic ctx from ssa ctx by FALSE */
    p_sic_ctx->detach_sic_ctx = ICF_FALSE;

    /*Fix for CSR 1-8223807 : SPR 20362*/
    p_sic_ctx->p_remove_hdrs = ICF_NULL;

    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_dbm_init_rgn_ctx
 *
 * DESCRIPTION:     This is a utility function that initializes registration 
 *                  context with default values.
 *
 ******************************************************************************/
icf_return_t 
icf_dbm_init_rgn_ctx(
	INOUT icf_glb_pdb_st       *p_glb_pdb,
        INOUT icf_rgm_context_st   *p_rgn_ctx)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
 
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }   

	p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    p_rgn_ctx->p_timer_buff = ICF_NULL;
    p_rgn_ctx->p_ssa_ctxt = ICF_NULL;
    p_rgn_ctx->num_of_lines = 0;
    p_rgn_ctx->indicator = 0;
#ifdef IMS_CLIENT
    p_rgn_ctx->p_srv_rte = ICF_NULL;
	
ICF_CRM_START
    p_rgn_ctx->retry_count = ICF_NULL;
    p_rgn_ctx->rgm_duration = ICF_NULL;
    p_rgn_ctx->rgm_state = ICF_RGM_00_IDLE;
    p_rgn_ctx->buffer_id = 0;
ICF_CRM_END
#endif

    p_rgn_ctx->p_contact_uri_list = ICF_NULL;

/* CSR_1-8389294 changes start */
    /* Initialize dereg_specific_contact with default value as false */
    p_rgn_ctx->dereg_specific_contact = ICF_FALSE;
/* CSR_1-8389294 changes end */

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
        
    return ret_val;
}

ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
/*************************************************************************
 *
 * FUNCTION:        icf_dbm_init_netact_cntxt_blk
 *
 * DESCRIPTION:     This function initializes the linked array 
 *                  structure for network access context blocks.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_netact_cntxt_blk(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        IN    icf_uint16_t                    num_of_blks,
        IN    icf_uint16_t                    num_of_apps,
        OUT   icf_netact_ctxt_st              **p_p_netact_cntxt_blk, 
        OUT   icf_error_t                     *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint16_t blk_index = 0;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* use MEMGET function to allocate memory in advance for storing
     * network access context data block */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_netact_ctxt_st) * num_of_blks,        
            *p_p_netact_cntxt_blk,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)
    
    /* use MEMGET function to allocate memory in advance for storing
     * application context data block associated with each network */
    for(blk_index = 0; blk_index < num_of_blks; blk_index++)
    {
        ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_netact_app_ctxt_st) * num_of_apps,        
            (*p_p_netact_cntxt_blk)[blk_index].p_app_ctxt,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return (ret_val);
}/* End icf_dbm_init_netact_cntxt_blk */
#endif

/*************************************************************************
 *
 * FUNCTION:        icf_dbm_init_app_conf_blk
 *
 * DESCRIPTION:     This function initializes the linked array 
 *                  structure for application context blocks.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_app_conf_blk(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        IN    icf_uint16_t                    num_of_blks,
        OUT   icf_app_conf_st                 **p_p_app_conf_blk, 
        OUT   icf_error_t                     *p_ecode)
{


    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* use MEMGET function to allocate memory in advance for storing
     * application context data block */
    ICF_STATIC_MEMGET( p_glb_pdb,
            sizeof(icf_app_conf_st) * num_of_blks,        
            *p_p_app_conf_blk,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return (ret_val);
}/* End icf_dbm_init_app_conf_blk */
ICF_CRM_END

/******************************************************************************  *
 *
 * FUNCTION:        icf_dbm_init_sl_ctx
 *
 * DESCRIPTION:     This is a utility function that initializes service logic 
 *                  context with default values.
 *
 ******************************************************************************/
icf_return_t 
icf_dbm_init_sl_ctx(
	INOUT icf_glb_pdb_st    *p_glb_pdb,
        INOUT icf_srvc_ctx_lgc_ut   *p_sl_ctx)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }
   
    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

	p_glb_pdb = p_glb_pdb;

    /*
     * Initialize all bytes in service logic context block to 0
     */
    icf_port_memset(p_sl_ctx,(icf_uint8_t const)0,
            (icf_uint32_t const)sizeof(icf_srvc_ctx_lgc_ut));
    
    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
        
    return ret_val;
}

/*************************************************************************
 *
 *     FUNCTION NAME    :   icf_dbm_create_task_pool
 *
 *     DESCRIPTION      :   Would fill in the information about different block 
 *                          sizes and the number of blocks of the size to be 
 *                          created for task specific pool and call the port 
 *                          function to create the pools
 *
 *     RETURNS          :  SUCCESS or FAILURE depending on the value returned by
 *                         port
 * 
 *************************************************************************/
icf_return_t
icf_dbm_create_task_pool(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_dbm_scale_data_init_st   *p_init_param,        
        OUT   icf_error_t *p_ecode)
{
    icf_memory_pool_table_st pool_data;
    icf_return_t            ret_val = ICF_SUCCESS; 
	icf_uint32_t      		sys_scaling_factor = ICF_SYS_SCALING_FACTOR_DEFAULT;	
    
    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

	sys_scaling_factor = icf_dbm_get_sys_scale_factor(p_glb_pdb);			
     
    /*
     * Fill SDP block info
     */ 
    pool_data.pools[0].pool_buffer_size = 
                        (icf_uint32_t)sizeof(icf_internal_sdp_stream_st);
    
    pool_data.pools[0].num_buffers = (icf_uint32_t)
	    	((ICF_SDP_BLK_MUL_FACTOR * p_init_param->max_num_of_calls * 
			  sys_scaling_factor) / 2);

    pool_data.pools[0].pool_id = ICF_MEM_SDP;

    /*
     * Fill TIMER block info
     */ 
    pool_data.pools[1].pool_buffer_size = 
                        (icf_uint32_t)sizeof(icf_timer_data_st);
    
    pool_data.pools[1].num_buffers = 
			(ICF_TIMER_BLK_MUL_FACTOR * sys_scaling_factor) / 2;

    pool_data.pools[1].pool_id = ICF_MEM_TIMER;

    /*
     * Fill CALL_CTX_LIST pool block info
     */ 
    pool_data.pools[2].pool_buffer_size = 
                        (icf_uint32_t)sizeof(icf_call_ctx_list_st);
    
    pool_data.pools[2].num_buffers = 
			(p_init_param->max_num_of_calls * sys_scaling_factor) / 2;

    pool_data.pools[2].pool_id = ICF_MEM_CALL_CTX_LIST;

    /*
     * Fill ADDRESS pool block info
     */ 
    pool_data.pools[3].pool_buffer_size = 
                        (icf_uint32_t)sizeof(icf_address_st);
    
    pool_data.pools[3].num_buffers = (icf_uint32_t)
    	((ICF_ADDRESS_BLK_MUL_FACTOR * p_init_param->max_num_of_calls * 
			  sys_scaling_factor) / 2);

    pool_data.pools[3].pool_id = ICF_MEM_ADDRESS;

    pool_data.pools[4].pool_buffer_size = 
                        (icf_uint32_t)sizeof(icf_ssa_timer_data_st);
   /*TO DO The usageof numeric to be changed to some defined value here*/ 
    pool_data.pools[4].num_buffers = 
			(ICF_MAX_NUM_OF_SSA_TIMER_DATA * sys_scaling_factor) / 2;

    pool_data.pools[4].pool_id = ICF_MEM_SSA_TIMER;

    /*
     * Fill the count of number of pools to be created
     */  
    pool_data.pool_count = 5;
    
    if(ICF_FAILURE == icf_port_task_memory_init(
            p_glb_pdb->p_glb_data->p_glb_port_info,
            &pool_data,
            p_ecode ))
    {
        ret_val = ICF_FAILURE;
    }            
        
    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */

    return ret_val;
}

/*************************************************************************
 *
 *     FUNCTION NAME    :   icf_dbm_create_cmn_pool
 *
 *     DESCRIPTION      :   Would fill in the information about different block 
 *                          sizes and the number of blocks of the size to be 
 *                          created for common pool and call the port function 
 *                          to create the pool
 *
 *     RETURNS          :  SUCCESS or FAILURE depending on the value returned by
 *                         port
 * 
 *************************************************************************/
icf_return_t
icf_dbm_create_cmn_pool(
        INOUT icf_glb_pdb_st              *p_glb_pdb,
        OUT   icf_error_t                  *p_ecode)
{
    icf_memory_pool_table_st pool_data;
    icf_return_t            ret_val = ICF_SUCCESS; 
	icf_uint32_t            sys_scaling_factor = ICF_SYS_SCALING_FACTOR_DEFAULT;
    
    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

	sys_scaling_factor = icf_dbm_get_sys_scale_factor(p_glb_pdb);
			
    pool_data.pools[ICF_INDX_BLK_SIZE_8].pool_buffer_size =ICF_BLK_SIZE_8;
    pool_data.pools[ICF_INDX_BLK_SIZE_8].num_buffers = 
			(ICF_NUM_BLK_SIZE_8 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_8].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_16].pool_buffer_size =ICF_BLK_SIZE_16;
    pool_data.pools[ICF_INDX_BLK_SIZE_16].num_buffers = 
			(ICF_NUM_BLK_SIZE_16 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_16].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_32].pool_buffer_size =ICF_BLK_SIZE_32;
    pool_data.pools[ICF_INDX_BLK_SIZE_32].num_buffers = 
			(ICF_NUM_BLK_SIZE_32 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_32].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_64].pool_buffer_size =ICF_BLK_SIZE_64;
    pool_data.pools[ICF_INDX_BLK_SIZE_64].num_buffers = 
			(ICF_NUM_BLK_SIZE_64 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_64].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_128].pool_buffer_size =ICF_BLK_SIZE_128;
    pool_data.pools[ICF_INDX_BLK_SIZE_128].num_buffers = 
			(ICF_NUM_BLK_SIZE_128 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_128].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_256].pool_buffer_size =ICF_BLK_SIZE_256;
    pool_data.pools[ICF_INDX_BLK_SIZE_256].num_buffers = 
			(ICF_NUM_BLK_SIZE_256 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_256].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_512].pool_buffer_size =ICF_BLK_SIZE_512;
    pool_data.pools[ICF_INDX_BLK_SIZE_512].num_buffers = 
			(ICF_NUM_BLK_SIZE_512 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_512].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_1024].pool_buffer_size =ICF_BLK_SIZE_1024;
    pool_data.pools[ICF_INDX_BLK_SIZE_1024].num_buffers = 
			(ICF_NUM_BLK_SIZE_1024 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_1024].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_2048].pool_buffer_size =ICF_BLK_SIZE_2048;
    pool_data.pools[ICF_INDX_BLK_SIZE_2048].num_buffers = 
			(ICF_NUM_BLK_SIZE_2048 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_2048].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_4096].pool_buffer_size =ICF_BLK_SIZE_4096;
    pool_data.pools[ICF_INDX_BLK_SIZE_4096].num_buffers = 
			(ICF_NUM_BLK_SIZE_4096 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_4096].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_8192].pool_buffer_size =ICF_BLK_SIZE_8192;
    pool_data.pools[ICF_INDX_BLK_SIZE_8192].num_buffers = 
			(ICF_NUM_BLK_SIZE_8192 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_8192].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_10240].pool_buffer_size =ICF_BLK_SIZE_10240;
    pool_data.pools[ICF_INDX_BLK_SIZE_10240].num_buffers = 
			(ICF_NUM_BLK_SIZE_10240 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_10240].pool_id = ICF_MEM_COMMON;

    pool_data.pools[ICF_INDX_BLK_SIZE_20480].pool_buffer_size =ICF_BLK_SIZE_20480;
    pool_data.pools[ICF_INDX_BLK_SIZE_20480].num_buffers = 
			(ICF_NUM_BLK_SIZE_20480 * sys_scaling_factor) / 2;
    pool_data.pools[ICF_INDX_BLK_SIZE_20480].pool_id = ICF_MEM_COMMON;

    pool_data.pool_count = ICF_MAX_NUM_COMMON_POOL;


    if(ICF_FAILURE == icf_port_memory_init(
            p_glb_pdb->p_glb_data->p_glb_port_info,
            &pool_data,
            p_ecode ))
    {
        ret_val = ICF_FAILURE;
    }            
        
    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */

    return ret_val;

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_set_blk_n_pool_size
 *
 * DESCRIPTION: The function initialize the pool size and block size in DBM 
 *              global data block for different pool ids based on which DBM 
 *              creates it pools   
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_set_blk_n_pool_size(
        INOUT icf_glb_pdb_st                     *p_glb_pdb,
        IN    icf_dbm_scale_data_init_st         *p_init_param, 
        OUT   icf_error_t                        *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_dbm_data_st *p_dbm_data = ICF_NULL;
    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
        
   if(ICF_SUCCESS == ret_val)
   {
       p_dbm_data->pool[ICF_BLK_TYPE_CALL_CTX].pool_size = 
           p_init_param->max_num_of_calls;
       p_dbm_data->pool[ICF_BLK_TYPE_SRVC_CTX].pool_size = 
           p_init_param->max_num_of_service_calls;

       p_dbm_data->pool[ICF_BLK_TYPE_SL_CTX].pool_size = 
           p_init_param->max_num_of_service_calls;
       p_dbm_data->pool[ICF_BLK_TYPE_RGN_CTX].pool_size = 
           p_init_param->max_num_of_registrations;

       p_dbm_data->pool[ICF_BLK_TYPE_CALL_CTX].block_size = 
           sizeof(icf_call_ctx_st);
       p_dbm_data->pool[ICF_BLK_TYPE_SRVC_CTX].block_size = 
           sizeof(icf_service_ctx_st);
       
       p_dbm_data->pool[ICF_BLK_TYPE_SL_CTX].block_size =
           sizeof(icf_srvc_ctx_lgc_ut); 
       p_dbm_data->pool[ICF_BLK_TYPE_RGN_CTX].block_size = 
           sizeof(icf_rgm_context_st);
#if 0       
       p_dbm_data->pool[ICF_BLK_TYPE_SUBM_CTX].block_size = 
           sizeof(icf_subm_ctx_st);
       p_dbm_data->pool[ICF_BLK_TYPE_SUBM_CTX].pool_size = 
           p_init_param->max_num_of_subm_ctx;
#endif
	}

    return ret_val;
  
}/*End icf_dbm_set_blk_n_pool_size*/


/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_create_pools
 *
 *     DESCRIPTION      :    This function would be used to create various pools
 *                           for dbm and would also initialize the linked array 
 *                           of used nodes in pool.
 *
 *     RETURNS          :   icf_return_t  
 *
 *************************************************************************/
icf_return_t 
icf_dbm_create_pools(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        OUT   icf_error_t                     *p_ecode)
{
    icf_return_t          ret_val= ICF_SUCCESS;
    icf_dbm_pool_info_st  *p_pool = ICF_NULL;
    icf_dbm_block_type_et blk_type = 0;
ICF_CRM_START
	icf_uint32_t			  i; /* temp variable */
	icf_config_data_st     *p_config_data; 
	icf_rgm_context_st	  *p_rgn_cntxt; 
ICF_CRM_END


    ICF_FUNCTION_ENTER(p_glb_pdb)

    while(ICF_MAX_BLK_TYPE > blk_type)
    {

        /* 
         * Get the address of the pool pointed by the block type
         */
        p_pool =  icf_dbm_get_pool_addr(
                p_glb_pdb,
                blk_type);
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pool))
       
        /* 
         * If the pool size and the block size are greater than 0 then allocate
         * memory for pool and initialize the pool   
         */ 
        if(0 < p_pool->pool_size && 0 < p_pool->block_size)
        { 
            /* 
             * use STATIC_MEMGET to allocate memory
             */
            ICF_STATIC_MEMGET( p_glb_pdb,
                    (icf_uint32_t)(p_pool->block_size * p_pool->pool_size *
                                      sizeof(icf_uint8_t)),
                    p_pool->p_blk_array,
                    ICF_RET_ON_FAILURE,
                    p_ecode, ret_val)

            /* 
             * use STATIC_MEMGET to deallocate memory 
             */
             ICF_STATIC_MEMGET( p_glb_pdb,
                     (icf_uint32_t)(p_pool->pool_size * 
                                       sizeof(icf_dbm_la_st *)),
                     p_pool->p_available_blk_lst,
                     ICF_RET_ON_FAILURE,
                     p_ecode, ret_val)

            /*Initialize the unused block linked array*/
             if( ICF_FAILURE == 
                     icf_dbm_la_init(p_pool->pool_size,
                         p_pool->p_available_blk_lst,
                         &p_pool->next_free_id, 
                         p_ecode))         
             {
                 ret_val= ICF_FAILURE;
                 break;
             }/*End if ( ICF_FAILURE == ret_val)*/
							
        }/* End if(0 < p_pool->pool_size ... */

		if (ICF_BLK_TYPE_RGN_CTX == blk_type)
		{
ICF_CRM_START
			p_rgn_cntxt = (icf_rgm_context_st*)p_pool->p_blk_array; 
			if ( ret_val == ICF_SUCCESS )
			{
				/* get the config data pointer from DBM */
				ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
							(icf_int_module_id_t)ICF_INT_MODULE_CFG,
							(icf_void_t *)&(p_config_data),
							p_ecode);
	
			/* allocate the static memory for p_rgn_uris and p_app_data_list; 
			 * in each of the registration context */
				for (i=0; i< p_pool->pool_size && ret_val != ICF_FAILURE; i++)
				{
                    (p_rgn_cntxt)[i].p_app_data_list = ICF_NULL; 
                    ICF_STATIC_MEMGET( p_glb_pdb,
					sizeof(icf_rgm_app_ctxt_st) * p_config_data->max_applications,
						((p_rgn_cntxt)[i].p_app_data_list),
						ICF_DONOT_RET_ON_FAILURE,
						p_ecode, ret_val)
				
				}
				if ( ret_val == ICF_FAILURE )
				{
					/* free p_rgn_uris and p_app_data_list */
					for (i=0; i < p_pool->pool_size; i++)
					{
						if ((p_rgn_cntxt)[i].p_app_data_list)
						{
							ICF_STATIC_MEMFREE( p_glb_pdb,
							(p_rgn_cntxt)[i].p_app_data_list,
							p_ecode, ret_val)
						}
					}
					ICF_STATIC_MEMFREE( p_glb_pdb,
							p_rgn_cntxt,
							p_ecode, ret_val)
					break;
				}
			}
ICF_CRM_END

		
		}
        
        blk_type += 1;
    }/* End while(ICF_MAX_BLK_TYP ... */
    

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return (ret_val);

}/* End icf_dbm_create_pools */
