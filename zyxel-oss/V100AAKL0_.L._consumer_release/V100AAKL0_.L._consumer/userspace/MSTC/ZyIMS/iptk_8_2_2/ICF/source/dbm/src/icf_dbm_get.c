/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_get.c
 *
 * Purpose          : This file contains functions that would implement 
 *                    functionality for searching and retreiving various data 
 *                    blocks
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  14 DEC 2004   Sumit Gupta                  File created and initial version
 *  06 APR 2005   Sumit Gupta      SPR No 7355 created a new function to extract
 *                                             user name from SIP URI if the 
 *                                             user name is a valid e164 number
 *                                             and no exact match for user with
 *                                             the SIP URI found on line while 
 *                                             retreiving line for incomming 
 *                                             call lines are searched again for
 *                                             matching user having extracted 
 *                                             E164 address
 *
 * 11 APR 2005  Sumit Gupta      SPR No 7372  Added a check for user name to be 
 *                                            present before copying it into 
 *                                            call context in function 
 *                                            icf_dbm_fill_local_user_info
 *
 * 11 APR 2005  Sumit Gupta      SPR No 7392 The function 
 *                                           icf_dbm_get_ong_call_ctx was 
 *                                           returning call id at the index next
 *                                           to what was should have been 
 *                                            returned
 *
 * 11 APR 2005  Sumit Gupta      SPR No 7392  Reinitialized retval to 
 *                                            ICF_FALSE after call function 
 *                                            for getting global data
 * 21 APR 2005   Sumit Gupta     SPR 7416    Replaced call to macro 
 *                                           ICF_MEMFREE with 
 *                                           ICF_STATIC_MEMFREE 
 * 22 APR 2005   Sumit Gupta     SPR 7435    Set release cause in case 
 *                                           validate line for ougoing call fail
 * 22 APR 2005  Sumit Gupta      SPR 7434    Updated the handling in function
 *                                           icf_dbm_get_line_for_inc_call
 * 27 APR 2005  Sumit Gupta      SPR 7458    DBM wouldn't return status 
 *                                           ICF_STATUS_CALL_WAITING inspite
 *                                           it would return status as
 *                                           ICF_STATUS_CALL_FWD_BUSY_N_CALL_
 *                                           WAITING in case eith call waiting 
 *                                           or call forwarding busy is active
 * 15 MAY 2005  Sumit Gupta      SPR 7559    Put an addition check in function 
 *                                           icf_dbm_find_call_cntxt_blk to 
 *                                           look into call cantext blk to see 
 *                                           if it contains INVALID call id. If 
 *                                           so fail should be returned
 * 20 MAY 2005 Sumit Gupta      SPR 7588     ADDED ASSERTS 
 * 10-Aug-2005	Rohit Aggarwal	ICF			 Code changes specific to ICF
 * 02-Feb-2006  Amit Sharma                  CFG merged with ICF 2.0
 * 08-Mar-2006     Aman Aggarwal             ICF merging with ICF2.0
 * 23-Mar-2006  Amit Sharma                  Code merged with ICF 4.2
 * 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
 *											 Multiple Public Users ICF5.0
 * 29-Aug-2007  Amit Mahajan    SPR 16005   IPTK should present incoming call received
 *                              BDFam33243  to PA as normal incoming call, if already existing
 *                                          call on the same line is in releasing state.         
 * 26-Jan-2008  Amit Mahajan    ICF Rel 8.0  Changes for reg event package support
 * 27-Jan-2008  Rajiv Kumar     ICF 8.0     Registration procedure
 *                                          Enhancements
 * 19-Mar-2008  Shruti Thakral  SPR 18252   Klocworks warning removal
 * 24-Mar-2008  Tarun Gupta     IPTK Rel 8.0  Merged User Param and
 *                                            Dynamic Payload Enhancements
 * 3-Apr-2008   Sumant Gupta    ICF 8.0       Rel7.1 Windows porting Merging
 * 17-May-2008  Amit Sharma     SPR:18577   Assigning line_id, p_sic_ctx and 
 *                                          p_rgm_ctx in p_glb_pdb
 * 18-Jun-2008  Anurag Khare    SPR 18684   Maddr Handling 
 * 04-Nov-2008  Tarun Gupta     SPR 19189   Merged Rel 7.1.2 SPR 18880
 * 08-Dec-2008  Rajiv Kumar     SPR 19218   CSR-1-6421580 Merged
 * 18-Dec-2008  Rajiv Kumar     SPR 19218   CSR-1-6102712 Merged
 * 10-Feb-2009  Tarun Gupta     Rel 8.1 LLD SDP Tunnelling Enhancements
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349       Klocworks fix
 * 03-Apr-2009 Kamal Ashraf     Rel 8.2     Klocwork Fix
 * 30-Apr-2009 Tarun Gupta      SPR 19532   Modified icf_dbm_decrease_call_waiting
 * 05-May-2009 Rajiv Kumar      Rel 8.2     Klocwork Fix
 * 11-Jun-2009 Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal
 * 27-Jul-2009 Tarun Gupta      SPR 19891   Modified icf_dbm_add_public_uri_for_line
 * 02-Aug-2009 Rajiv Kumar      SPR 19886   Merged SPR 19821 
 * 08-Sep-2009 Preksha          SPR 19999   Merged SPR 19803 
 * 08-Sep-2009 Rajiv Kumar      SPR 19999   Merged CSR 1-7586127 
 * 22-Sep-2009 Tarun Gupta      SPR 20063   Merged CSR 1-7644788
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/
/* Included for IPV6 */


#include"icf_dbm_types.h"
#include"icf_dbm_prototypes.h"
#include"icf_dbm_regm_prototypes.h"
#include"icf_dbm_sc_prototypes.h"
#include"icf_dbm_defs.h"
#include"icf_common_types.h"
#include"icf_common_defs.h"
#include"icf_port_prototypes.h"
#include"icf_macro.h"
#include"icf_ecode.h"

#include <ctype.h>


#ifdef ICF_SERVICE_ENABLE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_srvc_cntxt_blk
 *
 * DESCRIPTION: The function looks for the sevice context block for the call
 *              if present return the block address else allocates a new block   
 * INPUT:       The function expects a valid call ID to be passed for which a 
 *              call is in progress
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t  
icf_dbm_get_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        IN  icf_call_id_t                     call_id,
        OUT icf_service_ctx_st                **p_p_srvc_cntxt_blk,
        OUT icf_error_t                       *p_ecode)
{   
    icf_return_t                        ret_val = ICF_SUCCESS;
#ifdef ICF_TRACE_ENABLE
    icf_trace_data_st                   trace_data;
#endif

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
        
    ICF_FUNCTION_ENTER(p_glb_pdb)

	/* If service context block could not be found allocate one*/
    if(ICF_FAILURE == icf_dbm_find_srvc_cntxt_blk(
       p_glb_pdb,call_id,
       p_p_srvc_cntxt_blk,p_ecode))
    {     
        if(ICF_SUCCESS == icf_dbm_alloc_blk(p_glb_pdb,
        (icf_dbm_block_type_et)ICF_BLK_TYPE_SRVC_CTX,
        (icf_void_t **)p_p_srvc_cntxt_blk,
        p_ecode))
        {

#ifdef ICF_TRACE_ENABLE
            trace_data.int_val[0] = call_id;
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;     
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;  
        
            ICF_TRACE(p_glb_pdb,
               (icf_trace_id_t const)(ICF_SRVC_CTX_ALLOCATE),&trace_data) 
#endif
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
        
    return (ret_val);         
  
}/* End icf_dbm_get_srvc_cntxt_blk */
#endif /* #ifdef ICF_SERVICE_ENABLE */



#ifdef ICF_SERVICE_ENABLE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_find_srvc_cntxt_blk
 *
 * DESCRIPTION: Returns the address of service context block for specified call
 *              id
 *INPUT:       The function expects a valid call ID to be passed for which a 
 *              call is in progress
 * RETURNS: icf_return_t
 *
 *************************************************************************/
icf_return_t 
icf_dbm_find_srvc_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_call_id_t                   call_id,
       OUT   icf_service_ctx_st              **p_p_service_cntxt_blk,       
       OUT   icf_error_t                     *p_ecode)
{

     icf_call_ctx_st         *p_call_cntxt_blk = ICF_NULL;
     icf_return_t            ret_val = ICF_SUCCESS;
#ifdef ICF_TRACE_ENABLE
     icf_trace_data_st       trace_data;
#endif

     ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

     ICF_FUNCTION_ENTER(p_glb_pdb)
         
     if (ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                 p_glb_pdb,call_id, &p_call_cntxt_blk,
                 p_ecode))
     {
         ret_val = ICF_FAILURE;
     }
     else if(ICF_NULL == p_call_cntxt_blk->p_service_ctx)
     {
#ifdef ICF_ERROR_ENABLE
         ICF_SET_ECODE(p_glb_pdb,ICF_NULL,
                 ICF_NO_SRVC_CTX_EXIST, p_ecode)
#endif
         ret_val = ICF_FAILURE;
     }
     else
     {   
#ifdef ICF_TRACE_ENABLE 
         trace_data.int_val[0] = call_id;
         trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
         trace_data.number_of_string_values  = ICF_NO_STRING_PRESENT;      

         ICF_TRACE(p_glb_pdb,
                 (icf_trace_id_t const)(ICF_SRVC_CTX_FOUND),&trace_data)
#endif
         *p_p_service_cntxt_blk = p_call_cntxt_blk->p_service_ctx;
     }
       
     ICF_FUNCTION_EXIT(p_glb_pdb)

     ICF_RESET_MODULE(p_glb_pdb)     
         
     return (ret_val);

}/*End icf_dbm_find_srvc_cntxt_blk*/
#endif /* #ifdef ICF_SERVICE_ENABLE */

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_find_call_cntxt_blk
 *
 * DESCRIPTION: Returns the address of call context block for specified call id
 * INPUT:       The function expects a valid call ID to be passed for which a 
 *              call is in progress
 *
 * RETURNS: icf_return_t
 *        
 ************************************************************************/
icf_return_t 
icf_dbm_find_call_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_call_id_t                   call_id,
       OUT   icf_call_ctx_st                 **p_p_call_cntxt_blk,
       OUT   icf_error_t                     *p_ecode)
{
	icf_dbm_blk_id_t                call_cntxt_blk_id;

#ifdef ICF_TRACE_ENABLE
    icf_trace_data_st               trace_data;
#endif
#ifdef ICF_ERROR_ENABLE
    icf_error_data_st               error_data;
#endif
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_call_ctx_st                 *p_temp_call_ctx = ICF_NULL;
    icf_dbm_pool_info_st            *p_call_ctx_pool = ICF_NULL;
    /* Rel 8.2 Klocworks fix */
    icf_cmn_init_error_data_st(&error_data);
    /* Rel 8.2 Klocworks fix end */

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
     
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_glb_pdb))

#ifdef ICF_ERROR_ENABLE
    error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
    error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#endif     

   /*
     * Get the pool address for call context pool
     */
    p_call_ctx_pool =  icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_CALL_CTX);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_pool))

	ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_glb_pdb->p_glb_data))    

	if((ICF_DBM_OPT_CTX_ID_START < call_id) &&
		(ICF_DBM_OPT_CTX_ID_END > call_id))
	{
		return ICF_FAILURE;
	}

    /*Check for the MSB If it is a call Id for Incomming call then the 
     * value of the 15 LSB would give the value of Index of call context 
     * block Fetch the service context block from that call context block*/
    if(ICF_DBM_INC_CALL_ID_START == (call_id & ICF_DBM_INC_CALL_ID_START))
    {
        call_cntxt_blk_id =  (icf_call_id_t)(call_id & ~(ICF_DBM_INC_CALL_ID_START));
        
        if (call_cntxt_blk_id > p_call_ctx_pool->pool_size)
            return ICF_FAILURE;

        /*Fetch the call context block pointed by call_cntxt_blk_id to see if it
         * contains a valid call id. If it doesnot contain a valid call ID means          
         *there is no call going on for the call ID. Set ret_val to FAILURE*/
        p_temp_call_ctx = 
            ((icf_call_ctx_st *)
             p_call_ctx_pool->p_blk_array) +
             call_cntxt_blk_id;
        
        /*If call context block contains call Id as ICF_INVALID_CALL_ID
         * or it contains some call id other than the one we are looking for set
         * retval as FAILURE. This can happen in case call is terminated and
         * and some api comes from suppose PA or SSA like call term response*/
        if(ICF_INVALID_CALL_ID == p_temp_call_ctx->call_id ||
               call_id != p_temp_call_ctx->call_id )
        {
            ret_val = ICF_FAILURE;
        }
	else
	{
		*p_p_call_cntxt_blk = 
            		((icf_call_ctx_st *)
           		  p_call_ctx_pool->p_blk_array) +
           		  call_cntxt_blk_id;
	}

    }/*end if(ICF_DBM_INC_CALL_ID_START.. */
    /* 
     * If this is not an incomming call
     */
    else
    {
        /* Retreive the call context block id for outgoing call*/
	/* this function call is replaced by hash table function call */
	/*
        ret_val = 
            icf_dbm_get_idx_for_ogn_ccx(
                    p_glb_pdb,
                    call_id,
                    &call_cntxt_blk_id,
                    p_ecode);*/

		ret_val = icf_dbm_fetch_call_ctxt_from_hash_table(
			p_glb_pdb, call_id, p_p_call_cntxt_blk, p_ecode);

    }   

    
    if(ICF_SUCCESS == ret_val)
    {
        /*Return call context block address in the pointer passed as argument*/
        /* *p_p_call_cntxt_blk = 
            ((icf_call_ctx_st *)
             p_call_ctx_pool->p_blk_array) +
             call_cntxt_blk_id;
	`	 */
             
#ifdef ICF_TRACE_ENABLE
        trace_data.int_val[0] = call_id;
        trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
        trace_data.number_of_string_values  = ICF_NO_STRING_PRESENT;      
#endif

        ICF_TRACE(p_glb_pdb,
                (icf_trace_id_t const)(ICF_CALL_CTX_FOUND),&trace_data)

        p_glb_pdb->line_id = (*p_p_call_cntxt_blk)->line_id;
    }
    else
    {
#ifdef ICF_ERROR_ENABLE
        error_data.int_val[1] = call_id;   
        error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
        error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#endif
        ICF_SET_ECODE(p_glb_pdb,&error_data,
                ICF_NO_CALL_CTX_EXIST,p_ecode)
    }


    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    

    return (ret_val);

}/* End icf_dbm_find_call_cntxt_blk*/


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_find_sic_cntxt_blk
 *
 * DESCRIPTION: Returns the address of SIC context block for specified call id
 *
 * RETURNS: icf_return_t
 *        
 ************************************************************************/
icf_return_t 
icf_dbm_find_sic_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_call_id_t                   call_id,
       OUT   icf_sic_ctx_st                 **p_p_sic_cntxt_blk,
       OUT   icf_error_t                     *p_ecode)
{
    icf_dbm_sic_cntxt_blk_id_t      sic_cntxt_blk_id;
    icf_dbm_data_st                 *p_dbm_data = ICF_NULL;
    icf_return_t                    ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
     
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_glb_pdb))

    ICF_ASSERT(p_glb_pdb,(ICF_NULL != p_glb_pdb->p_glb_data))    

    ret_val = icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode);
         
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
 


#if 0
    /*Check for the MSB If it is a call Id for Incomming call then the 
     * value of the 15 LSB would give the value of Index of call context 
     * block Fetch the service context block from that call context block*/

    if(0x8000 == (call_id & 0x8000))
    {
        sic_cntxt_blk_id = (call_id & 0x7FFF);
        
        /*Fetch the call context block pointed by call_cntxt_blk_id to see if it
         * contains a valid call id. If it doesnot contain a valid call ID means          *there is no call going on for the call ID. Set ret_val to FAILURE*/
        p_temp_sic_ctx = ((icf_sic_ctx_st *)p_dbm_data->p_sic_cntxt_blk) +
             sic_cntxt_blk_id;
        
        /*If call context block contains call Id as ICF_INVALID_CALL_ID
         * set retval as FAILURE. This can happen in case call is terminated and
         * and some api comes from suppose PA or SSA like call term response*/
        if(ICF_INVALID_CALL_ID == p_temp_sic_ctx->call_id)
        {
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        ret_val = 
            icf_dbm_get_idx_for_inc_scx(
                    p_glb_pdb,
                    call_id,
                    &sic_cntxt_blk_id,
                    p_ecode);
    }
	
#endif

	
	/* Logic for SIC*/
	/* Currenyly SIC doesnt generate any Ids for Incoming request */
	/* Also for SIC there is no requirement for the MSBit to be 1 for requests from application */
	/* So in all cases we will search all the blocks and then find the SIC context block */
	
	ret_val = 
            icf_dbm_get_idx_for_inc_scx(
                    p_glb_pdb,
                    call_id,
                    &sic_cntxt_blk_id,
                    p_ecode);

    
    if(ICF_SUCCESS == ret_val)
    {
        /*Return call context block address in the pointer passed as argument*/
        *p_p_sic_cntxt_blk = 
            ((icf_sic_ctx_st *)p_dbm_data->p_sic_cntxt_blk) +
             sic_cntxt_blk_id;
             
        p_glb_pdb->line_id = (*p_p_sic_cntxt_blk)->line_id;

        p_glb_pdb->p_sic_ctx = *p_p_sic_cntxt_blk;
    }


    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    

    return (ret_val);

}/* End icf_dbm_find_sic_cntxt_blk*/

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_stats_glb_data
 *
 * DESCRIPTION: Finds out pointer to the stats data corresponding the stats id
 *
 * RETURNS: icf_return_t 
 *
 ************************************************************************/
icf_return_t
icf_dbm_get_stats_glb_data(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        OUT   icf_void_t            **p_p_stats_blk,
        OUT   icf_error_t           *p_ecode)
{
    icf_return_t        ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_ASSERT (p_glb_pdb,(ICF_NULL != 
                    p_glb_pdb->p_glb_data->p_stats_data))
    
    *p_p_stats_blk = p_glb_pdb->p_glb_data->p_stats_data;
    p_ecode = p_ecode;     /*To remove warning*/
    

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);

}/* end icf_dbm_get_stats_data_addr */

/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_get_line_cntxt_blk
 *
 *     DESCRIPTION      :   Return the address for line context block 
 *                          corresponding to the line ID. If no block exist 
 *                          allocates a new block and returns its address.
 *
 *     RETURNS          :  icf_return_t
 * 
 *************************************************************************/
icf_return_t
icf_dbm_get_line_cntxt_blk(
        INOUT icf_glb_pdb_st      *p_glb_pdb,
        IN    icf_line_id_t       line_id,
        OUT   icf_line_data_st    **p_p_line_data,
        OUT   icf_uint8_t         *p_line_status, 
        OUT   icf_error_t         *p_ecode)
{
    icf_line_data_st         **p_p_temp_line_data = ICF_NULL;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_config_data_st       *p_config_data = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)


    ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /*
     * Check if the line id given is a valid line id. If it is not a valid line
     * id set error and return failure. Line ids start from 0 to max_configured
     *  lines
     */
    if(p_config_data->max_lines <= line_id)
    {
        *p_ecode = ICF_STATUS_INVALID_LINE_ID;
        ret_val = ICF_FAILURE;
    }
    else
    {
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != 
                    p_glb_pdb->p_glb_data->p_p_line_data))
            
        p_p_temp_line_data = p_glb_pdb->p_glb_data->p_p_line_data + line_id;

        if(ICF_NULL != *p_p_temp_line_data)
        {
            *p_p_line_data = *p_p_temp_line_data;   
            *p_line_status = ICF_STATUS_LINE_OLD;                 
        }
        else 
        {
            ret_val = icf_dbm_get_module_glb_data(     
                    p_glb_pdb,
                    (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                    (icf_void_t *)&p_config_data,
                    p_ecode);

            ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))


            ICF_STATIC_MEMGET(p_glb_pdb,
                        sizeof(icf_line_data_st),        
                        *p_p_temp_line_data,
                        ICF_DONOT_RET_ON_FAILURE,
                        p_ecode, ret_val)

            if (ICF_FAILURE != ret_val)
            {
		        /* 
                 * Initialize the User adress list and user priority list 
                 * pointers to NULL
                 */ 
                (*p_p_temp_line_data)->p_user_db = ICF_NULL;
                
#ifdef ICF_MULTILINE    
                (*p_p_temp_line_data)->p_user_priority_list = ICF_NULL;
                    /* Allocate memory for maintaining user's priority*/
                ICF_STATIC_MEMGET(p_glb_pdb,
                    sizeof(icf_priority_id_t) * 
                    p_config_data->max_users_per_line ,     
                    (*p_p_temp_line_data)->p_user_priority_list,
                    ICF_DONOT_RET_ON_FAILURE,
                    p_ecode, ret_val)
#endif

                /* 
                 * If there was a failure in any of the memory allocations, 
                 * release all the allocated memory
				 */

                if(ICF_FAILURE == ret_val)
                {
                    ICF_STATIC_MEMFREE(
                            p_glb_pdb, 
                            *p_p_temp_line_data, 
                            p_ecode, 
                            ret_val) 

                        ret_val = ICF_FAILURE;   
                }
                else
                {
                    (*p_p_temp_line_data)->line_id = line_id;
					(*p_p_temp_line_data)->user_name.str_len = 0;
                    *p_p_line_data = *p_p_temp_line_data;
                    *p_line_status = ICF_STATUS_LINE_NEW; 
                }
            }/*End If*/
        }/* End Else*/
    }/*End Else*/
    ICF_RESET_MODULE(p_glb_pdb)    

    return(ret_val);

}/* End icf_dbm_get_line_cntxt_blk */

/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_fetch_line_cntxt_blk
 *
 *     DESCRIPTION      :   Return the address for line context block 
 *                          corresponding to the line ID. If no block exist 
 *                          returns failure.
 *		INPUT            :  The function expects a valid line ID lesser than or 
 *                          equal to max lines configured
 *
 *     RETURNS          :  icf_return_t
 * 
 *************************************************************************/
icf_return_t
icf_dbm_fetch_line_cntxt_blk(
        INOUT icf_glb_pdb_st      *p_glb_pdb,
        IN    icf_line_id_t       line_id,
        OUT   icf_line_data_st    **p_p_line_data,
        OUT   icf_error_t         *p_ecode)
{
    
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_config_data_st       *p_config_data = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

 
    ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))   
    
    /*
     * Check if the line id given is a valid line id. If it is not a valid line
     * id set error and return failure. Line ids start from 0 to max_configured
     *  lines
     */
    if(p_config_data->max_lines <= line_id)
    {
        *p_ecode = ICF_STATUS_INVALID_LINE_ID;
        ret_val = ICF_FAILURE;
        
    }
    else
    {
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != 
                    p_glb_pdb->p_glb_data->p_p_line_data))

        /* We should check if line id exist or not */
        if(ICF_NULL != *(p_glb_pdb->p_glb_data->p_p_line_data + line_id))
        {
            *p_p_line_data = *(p_glb_pdb->p_glb_data->p_p_line_data + line_id); 
        }
        else 
        {
            *p_ecode = ICF_STATUS_LINE_NOT_CONFIGURED;
            ret_val = ICF_FAILURE;
        }
    }
    

    ICF_RESET_MODULE(p_glb_pdb)    

    return(ret_val);

}/* End icf_dbm_fetch_line_cntxt_blk */



/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_ret_index_for_addr
 *
 * DESCRIPTION: Returns the index of the block in DBM's pool
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t
icf_dbm_ret_index_for_addr(
	INOUT icf_glb_pdb_st       *p_glb_pdb,
        IN  icf_void_t             *p_start_addr,
        IN  icf_void_t             *p_block_addr,
        IN  icf_uint16_t           block_size,
        OUT icf_uint16_t           *p_index,
        OUT icf_error_t            *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }
   

	p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_ASSERT (p_glb_pdb,(ICF_NULL != p_start_addr))   

    ICF_ASSERT (p_glb_pdb,(ICF_NULL != p_block_addr))  
  
    /*Determine the index of the block, it will be get by
     *taking the difference of cutrent block - start block
     *divided by the block size*/
    *p_index = (icf_uint16_t )(((icf_uint8_t *)p_block_addr - 
                (icf_uint8_t *)p_start_addr)/block_size);
    p_ecode = p_ecode; 

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_no_of_ong_calls
 *
 * DESCRIPTION: Returns the number of ongoing calls
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t
icf_dbm_get_no_of_ong_calls(
        INOUT icf_glb_pdb_st      *p_glb_pdb,
        OUT icf_uint16_t          *p_no_of_calls,
        OUT icf_error_t           *p_ecode)
{
    icf_uint16_t       counter_i = 0,current_index;
    icf_return_t       ret_val = ICF_SUCCESS;
	icf_dbm_la_st      *p_available_blk_lst = ICF_NULL;
    icf_dbm_pool_info_st *p_call_ctx_pool = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
/*
     * Get the pool address for call context pool
     */
    p_call_ctx_pool =  icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_CALL_CTX);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_pool))

    /* If next free id is equa to the pool size mean pool had all blocks 
     * allocated. Return max calls equal to the pool size*/
    if(p_call_ctx_pool->pool_size == 
             p_call_ctx_pool->next_free_id)
    {
        *p_no_of_calls = p_call_ctx_pool->pool_size;
    }
    /* 
     * If all the blocks are not allocated then traverse the free blocks list 
     * to identify the number of free blocks
     */
    else
    {
        p_available_blk_lst = 
            p_call_ctx_pool->p_available_blk_lst;
        
        current_index = p_call_ctx_pool->next_free_id;
        
        while(p_available_blk_lst[current_index].next_index
                != p_call_ctx_pool->pool_size) 
        {
           current_index = 
               p_available_blk_lst[current_index].next_index;
           counter_i += 1;
        }
        *p_no_of_calls = (icf_uint16_t )p_call_ctx_pool->pool_size -
                                     counter_i - 1;
    }/* End else */

    p_ecode = p_ecode; /* To remove warning*/

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
        
    return ret_val;
    
}/*End icf_get_no_of_ongoing_calls*/

/*************************************************************************
 *     FUNCTION NAME: icf_dbm_get_module_glb_data
 *
 *     DESCRIPTION : This function returns the address of data block for
 *                   the module whose id is been given
 *     INPUT       :  The function expects a valid module ID. An invalid module 
 *                    would be asking for trouble 
 *     RETURNS     : Success if data is retreived successfully else failure     
 *
 *************************************************************************/
icf_return_t
icf_dbm_get_module_glb_data(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        IN    icf_int_module_id_t   module_id,
        OUT   icf_void_t            **p_p_data,
        OUT   icf_error_t           *p_ecode)
{
    icf_return_t ret_val = ICF_FAILURE;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

#ifdef ICF_UT_TEST
    ICF_FUNCTION_ENTER(p_glb_pdb)
#endif
    
    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data[module_id])
    {
        *p_p_data = p_glb_pdb->p_glb_data->p_module_data[module_id];
        ret_val = ICF_SUCCESS;
    }
    else
    {
#ifdef ICF_ERROR_ENABLE
        ICF_SET_ECODE (p_glb_pdb,ICF_NULL,
			ICF_NO_MODULE_DATA_AVAILABLE, p_ecode)
#endif
    }
    p_ecode=p_ecode;


    ICF_RESET_MODULE(p_glb_pdb)    
    
    return ret_val;
}

/*************************************************************************
 *     FUNCTION NAME: icf_dbm_get_used_rgn_blk
 *
 *     DESCRIPTION : This function returns the address of registration block 
 *                   which is at the index marked by block_num in used 
 *                   registration list 
 *                   
 *     RETURNS     : Success if data is retreived successfully else failure     
 *
 *************************************************************************/
icf_return_t
icf_dbm_get_used_rgn_blk(
        INOUT icf_glb_pdb_st         *p_glb_pdb,
        IN    icf_uint16_t            block_num,
        OUT   icf_rgm_context_st     **p_p_rgn_cntxt_blk,
        OUT   icf_error_t            *p_ecode)
{
    icf_uint16_t           block_count = ICF_NULL;
    icf_dbm_data_st       *p_dbm_data = ICF_NULL;
    icf_dbm_blk_id_t      next_used_id;
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_dbm_pool_info_st  *p_rgn_pool_addr = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                p_ecode);
       
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /* 
     * Get the pool address for registration context pool
     */  
    p_rgn_pool_addr = icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_RGN_CTX);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgn_pool_addr))


    next_used_id = p_dbm_data->rgn_cntxt_blk_next_used_id;
    while(next_used_id != p_rgn_pool_addr->pool_size &&
            (block_count< block_num - 1) && (block_count <= p_rgn_pool_addr->block_size))
    {
        next_used_id = 
            p_rgn_pool_addr->p_available_blk_lst[next_used_id].next_index;
        block_count += 1;
    }
  
    if(next_used_id == p_rgn_pool_addr->pool_size)
    {
        ret_val = ICF_FAILURE;
    }else if(block_count >= p_rgn_pool_addr->block_size){

       printf("\r\nblock_count2 id is %x ",block_count);
        ret_val = ICF_FAILURE;
    }
    else
    {
         *p_p_rgn_cntxt_blk = 
                    ((icf_rgm_context_st *)p_rgn_pool_addr->p_blk_array) +
                     next_used_id;
          p_glb_pdb->p_rgm_ctx = *p_p_rgn_cntxt_blk;
          p_glb_pdb->line_id = (*p_p_rgn_cntxt_blk)->rgm_list_of_lines[0];
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    ICF_RESET_MODULE(p_glb_pdb)    
    
    return ret_val;
}


/****************************************************************************
 * FUNCTION:           icf_dbm_fetch_rgn_blk
 *
 * DESCRIPTION:        The function would be called to fetch the address of the 
 *                     the registration context block for the specified user    
 * INPUT               The function expects a valid line ID lesser than or 
 *                     equal to max lines configured
 * RETURNS:            Returns success if registration block for the user 
 *                     address exist
 *
 ***************************************************************************/
icf_return_t
icf_dbm_fetch_rgn_blk(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_user_addr,
        IN    icf_line_id_t  line_id,
        OUT   icf_rgm_context_st    **p_p_rgn_blk_addr,
        OUT   icf_error_t    *p_ecode)
{
    icf_dbm_blk_id_t           rgn_cntxt_blk_next_used_id;
    icf_return_t               ret_val = ICF_FAILURE;
    icf_dbm_data_st            *p_dbm_data = ICF_NULL;
    icf_rgm_context_st         *p_rgn_blk_addr = ICF_NULL;
    icf_dbm_pool_info_st       *p_rgn_pool_addr = ICF_NULL; 
    icf_line_id_t                i;
    int j=0;
 
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)     

    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                p_ecode);
       
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
 
    /*Reinitialize ret_val to ICF_FAILURE*/
    ret_val = ICF_FAILURE;
        

    /* 
    * Get the pool address for registration context pool
    */  
    p_rgn_pool_addr = icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_RGN_CTX);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgn_pool_addr))

   /* If next used id is equal to the pool size mean the pool is empty then 
    * return failure*/
   if(p_rgn_pool_addr->pool_size ==  
           p_dbm_data->rgn_cntxt_blk_next_used_id)
   {
#ifdef ICF_ERROR_ENABLE
       /*Error block does not exist*/
       ICF_SET_ECODE(p_glb_pdb,ICF_NULL,
               ICF_NO_RGN_CTX_EXIST,p_ecode)
#endif
   }/* End if(p_rgn_pool_addr->pool .. */
   /*If next used id is not equal to the pool size ie there are few used nodes*/

   else
    {
        rgn_cntxt_blk_next_used_id = p_dbm_data->rgn_cntxt_blk_next_used_id;
        j=0;
        /* traverse the used nodes*/ 
        while((p_rgn_pool_addr->pool_size != 
                rgn_cntxt_blk_next_used_id) && (ICF_FAILURE == ret_val))
        {
            p_rgn_blk_addr= 
                ((icf_rgm_context_st *)p_rgn_pool_addr->p_blk_array) + 
                  rgn_cntxt_blk_next_used_id;
            
             /* Compare Addresses*/ 
                if (ICF_TRUE == icf_dbm_util_cmp_addr(
                    p_glb_pdb,
                    p_user_addr,
                    &(p_rgn_blk_addr->rgm_user_address)))
                {
                    for(i = 0;(i<p_rgn_blk_addr->num_of_lines) && (i<ICF_MAX_NUM_OF_LINES) ;i++)
                    {
                        if(line_id == p_rgn_blk_addr->rgm_list_of_lines[i])
                        {
                            *p_p_rgn_blk_addr = p_rgn_blk_addr; 
                            p_glb_pdb->p_rgm_ctx = *p_p_rgn_blk_addr;
                            p_glb_pdb->line_id = line_id;
                            ret_val = ICF_SUCCESS;
                            break;
                        }
                    }

                }
            rgn_cntxt_blk_next_used_id = 
               p_rgn_pool_addr->p_available_blk_lst[rgn_cntxt_blk_next_used_id].
              next_index;
            j++;
            if(rgn_cntxt_blk_next_used_id >=p_rgn_pool_addr->block_size || j>= p_rgn_pool_addr->block_size ){/*minghung test*/

                printf("\r\nid is %x,%x",rgn_cntxt_blk_next_used_id,j);

                break;
                 
            }
        }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)    

    ICF_RESET_MODULE(p_glb_pdb)
    
    return ret_val;
}


/****************************************************************************
 * FUNCTION:           icf_dbm_check_other_call_act
 *
 * DESCRIPTION:        This is a utility function which would check if any call 
 *                     is active on the line on which current call is running
 *                     icf_address_st type addresses    
 *
 * RETURNS:            Returns success if check was made without any errors
 *
 ***************************************************************************/
icf_return_t icf_dbm_check_other_call_act(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        OUT   icf_boolean_t    *p_other_call_active,
        OUT   icf_error_t      *p_ecode )
{
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_call_ctx_list_st       *p_call_ctx_list = ICF_NULL;
    icf_line_id_t              line_id;
    /*Fix Merged for CSR_1_5400409*/
    icf_config_data_st         *p_config_data = ICF_NULL;
    icf_uint8_t                temp_count_active_calls =0;
    icf_uint8_t                max_limit_active_calls_per_line =0;
    *p_other_call_active = ICF_FALSE;    
    
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_call_ctx))
    
    /*Initialize line id with the line idfor current call context*/ 
    line_id = p_glb_pdb->p_call_ctx->line_id;
    /*Fix Merged for CSR_1_5400409*/
    /* Retreive the pointer for global CFG data*/
    ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    max_limit_active_calls_per_line = 
                p_config_data ->max_active_calls_per_line;

    
    /*Copy call context list pointer from line data structure*/
    p_call_ctx_list = p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list;

    /*
     * Iterate while either the list ends
     * or media_state of some other call is active
     */
    while(ICF_NULL != p_call_ctx_list)
    {
        /*
         * If call Id is same then leave it and go forward
         */
        if (p_glb_pdb->p_call_ctx->call_id ==
                p_call_ctx_list->p_call_ctx->call_id)
        {
            p_call_ctx_list = p_call_ctx_list->p_call_ctx_next;

            continue;
        } /* p_call_ctx_list->p_call_ctx->c... */

        /*
         * If any other call is active then break
         * the loop and set other_call_active as TRUE
         */
        /*if 
           ! ((release_type is valid) && 
              (PA_initiated release type or PA cleared call status) && 
              (call state not await DMS))

           and

            call_status local held

           then

            other call is active
         */
       if ((!((ICF_REL_TYPE_INVALID != 
             p_call_ctx_list->p_call_ctx->release_type) &&
             ((ICF_REL_TYPE_PA_INITIATED == p_call_ctx_list->p_call_ctx->release_type) ||
              (ICF_CALL_CLEAR_STATUS_PA_CLEARED & 
               p_call_ctx_list->p_call_ctx->call_clear_status)) &&
             (ICF_CC_07_AWT_DMS_RESP != p_call_ctx_list->p_call_ctx->call_state))) &&
               ((ICF_NULL == 
                (ICF_CALL_STATUS_LOCAL_HELD &
                 p_call_ctx_list->p_call_ctx->call_status)) ||
                   ((ICF_NULL != 
                     (ICF_CALL_STATUS_LOCAL_HELD & p_call_ctx_list->p_call_ctx->call_status)) &&
                     (ICF_NULL != p_call_ctx_list->p_call_ctx->p_service_ctx) &&
                     (ICF_NULL != 
                        ((icf_service_ctx_st *)(p_call_ctx_list->p_call_ctx->p_service_ctx)) \
                        ->active_service_list.p_sl_ctx \
                        [ICF_INT_MODULE_CHR - ICF_INT_MODULE_SL_START - 1]))))
       {
           /* First check imples that the call is not in releasing phase,
            * or media not cleared or call cleared from PA not set */
           /* It is assumed that CC is not waiting for media ongoing
            * in case of sending DMS. It sends DMS and goes to awt DMS
            * even if media ongoing is not invalid. If this assumption
            * is false, then also check that the call state should not
            * be wait for media ongoing with next state as awt DMS. 
            * Second check implies that call is not locally held. 
            * For both these cases, the call shall not be allowed. */
           
           /*Fix Merged for CSR_1_5400409*/
           /* Count the number of active calls on the line*/  
           temp_count_active_calls +=1;
            
           /* Check for the number of calls active on the line against
            *  the maximum limit configured*/

           if(max_limit_active_calls_per_line == temp_count_active_calls)
	       {
               *p_other_call_active = ICF_TRUE;
               break;
	       }
        } /* if (ICF_MEDIA_ACTIVE == p_pd... */

        p_call_ctx_list = p_call_ctx_list->p_call_ctx_next;
    } /* while(ICF_NULL != p_call_ct... */

    /*
     * If interation reaches at end then set
     * other_call_active as FALSE
     */
    if (ICF_NULL == p_call_ctx_list)
    {
        *p_other_call_active = ICF_FALSE;
    } /* if (ICF_NULL == p_call_ctx_l... */
    /*}*/ /* else */

   p_ecode = p_ecode;/*Just to remove warning*/ 

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
} /* End icf_dbm_check_other_call_act */


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_port_data
 *
 * DESCRIPTION:   This function returns the address of data block for
 *                porting module
 *
 * RETURNS:       Success or Failure
 *
 ************************************************************************/
icf_return_t
icf_dbm_get_port_data(
        INOUT icf_glb_pdb_st  *p_glb_pdb,
        OUT   icf_void_t      **p_p_port_data,
        OUT   icf_error_t     *p_ecode)
{
    
    icf_return_t        ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_ASSERT (p_glb_pdb,(ICF_NULL != 
                    p_glb_pdb->p_glb_data->p_glb_port_info))
    
    *p_p_port_data = p_glb_pdb->p_glb_data->p_glb_port_info;
    p_ecode = p_ecode;     /*To remove warning*/
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);

}/* end icf_dbm_get_port_data */

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_line_for_inc_call
 *
 *  DESCRIPTION:   The function finds a free line or an alternate line for 
 *                presenting an incomming call depending on the value of boolean
 *                parameter search_alt_line. If this parameter is true, the 
 *                function would try and find out a line other than the one in 
 *                call context for presenting the incomming call on.
 *                Else would try and find out any free line or a line with 
 *                minimum call waiting calls
 *
 *                IF THE CODE IS COMPILED WITHOUT ICF_MULTILINE FLAG
 *                   The algorithm for seaching a line for presenting incomming 
 *                   call on would go as follows:
 *                   
 *                   Try and find out line for presenting incomming call on. If 
 *                   found returns the line id and status to be idle.Else
 *                   returns invalid line id.
 *                   Intension is give highest priority to line which is idle
 *                   Next priority - line with all calls in releasing phase
 *                   Next priority - line which is not in setup phase and call
 *                                   waiting enabled
 *                   Else returned as busy.
 *                   
 *               IF THE CODE IS COMPILED WITH ICF_MULTILINE FLAG
 *                  The algorithm for seaching a line for presenting incomming 
 *                  call on would go as follows 
 *                  We would additionally take case of priorities in this case.
 *                  So we would be continuing the seach for a line till either 
 *                  we get a line with user having highest priority or there 
 *                  are no more lines to be searched for.This would ensure the 
 *                  line having user with highest priority amongst all free 
 *                  lines is selected for presenting the incomming call. If the 
 *                  call has to be presented as a call waiting call that is no 
 *                  free lines could be found, the line with minimum call 
 *                  waiting calls would be selected
 *               
 *                
 * RETURNS:       Success if the operation is completed successfully 
 *
 ************************************************************************/

icf_return_t
icf_dbm_get_line_for_inc_call(
    INOUT icf_glb_pdb_st *p_glb_pdb,
	IN    icf_boolean_t  search_alt_line,
    INOUT    icf_address_st *p_user_address,
    OUT   icf_line_id_t  *p_line_id,
    OUT   icf_uint32_t   *p_status,
    OUT   icf_error_t    *p_ecode)
{
    icf_uint32_t  line_count = 0, count = 0;
    
    /* 
     * Since min value is to be set, set to max value. 
     */
    icf_uint8_t   current_min_call_waiting = 0xff; 
    icf_boolean_t all_call_in_release_state = ICF_TRUE;
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_line_data_st *p_line_blk = ICF_NULL;
    icf_call_ctx_list_st         *p_call_ctx_list = ICF_NULL;
    icf_config_data_st           *p_config_data = ICF_NULL;
    icf_call_ctx_st              *p_call_ctx = ICF_NULL;
    icf_address_st               search_address = {{0,"\0"},0};
    icf_uint8_t                  search_count = 0;
    icf_uint8_t                  user_found = ICF_FALSE;
    icf_boolean_t                user_name_e164 = ICF_FALSE;
    icf_uint8_t                  max_search_count = 3;

    icf_boolean_t                some_call_in_setup_phase = ICF_FALSE;
    icf_boolean_t                line_with_rel_phs_found = ICF_FALSE;
    icf_boolean_t                line_with_local_held_found = ICF_FALSE;
    /*Fix Merged for CSR_1_5400409*/
    icf_uint8_t                  temp_active_calls_on_line = 0;
    icf_uint8_t                  min_active_calls_on_line =255;
    icf_boolean_t                line_avail = ICF_FALSE;
    icf_uint8_t                  Max_limit_of_active_calls_per_line =0;    
#ifdef ICF_MULTILINE
    icf_boolean_t                free_line_found = ICF_FALSE;
    icf_uint8_t                  priority = ICF_NULL,
                                    highest_priority = ICF_NULL;
#endif

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    *p_line_id = (icf_line_id_t)ICF_INVALID_LINE_ID;
    *p_status = (icf_uint32_t)ICF_STATUS_CONTINUE;

	/* Retreive the pointer for global CFG data*/
    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))   
	/* Set the minimum call waiting calls as max number of call waiting calls 
     * allowed per line
     */
    current_min_call_waiting =
                p_config_data->max_num_of_calls_in_call_waiting; 

    line_count = p_config_data->max_lines;
    /*Fix Merged for CSR_1_5400409*/
    /* Set the Max_limit_of_active_calls_per_line as max number of 
     * active calls allowed per line*/
 
    Max_limit_of_active_calls_per_line =
                p_config_data->max_active_calls_per_line;

   
    min_active_calls_on_line =  Max_limit_of_active_calls_per_line;    

    /*Copy the user address in search_address*/
    icf_port_memcpy(&search_address,p_user_address,
            (icf_uint32_t const)sizeof(icf_address_st));

	/* traverse all the lines*/
    do
    {
        line_with_rel_phs_found = ICF_FALSE;
        line_with_local_held_found = ICF_FALSE;
        for (count = 0; count < line_count; count++)
        {
            /* If the request was for searching an alternate line other than the
             * one on which the call was presented earlier then don't look into 
             * the line present in the call context */
            if(ICF_TRUE == search_alt_line && count == 
                    p_glb_pdb->p_call_ctx->line_id)
            {
                continue;
            }
            else if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
                        (icf_line_id_t const)(count),
                        &p_line_blk,
                        p_ecode))
            {
                /* Check for next line block. It is possible that there
                 * may be holes in line data configuration. So leave
                 * that. */
                continue;
            }

#ifdef ICF_MULTILINE
            /* 
             * Get the user's priority on hte line
             */ 
            priority = icf_dbm_get_user_priority(
                    p_glb_pdb,
                    &search_address,
                    p_line_blk);
            /* 
             * If priority is null means the user could not be found on the line
             * so continue searching user on other lines
             */
            if(ICF_NULL == priority)
            {
               continue;  
            }
            /* If the highest priority is not yet known set this to some 
             * priority greater than the priority of user found
             */
            else if(ICF_NULL == highest_priority)
            {
                highest_priority = priority + 1;
            }
#else            

			if(2 == search_count)
			{
				if(ICF_FAILURE == icf_dbm_check_if_user_exits_in_line(
                        p_glb_pdb,
                        p_user_address,
                        p_line_blk,
						(icf_boolean_t)ICF_TRUE,
                        p_ecode))
				{
					/* Check next line block where this user address exist
					* in that line. 
					*/
					continue;
				}
#endif
				else
				{
					user_found = ICF_TRUE;
				}
			}

#ifndef ICF_MULTILINE
			if(2 != search_count)
			{
				if (
					(ICF_FAILURE == icf_dbm_check_if_user_exits_in_line(
							p_glb_pdb,
							&search_address,
							p_line_blk,
							(icf_boolean_t)ICF_FALSE,
							p_ecode)))

				{
					/* Check next line block where this user address exist
					 * in that line. 
					 */
					continue;
				}
				else
				{
					user_found = ICF_TRUE;
				}
			}
#endif

#ifdef ICF_MULTILINE
            /* If call context list is null means there are no active calls on 
             * line and priority of user on line is greater then priority of 
             * user found on other lines
             */  
            if ((ICF_NULL == p_line_blk->p_call_ctx_list &&
                    highest_priority > priority))
#else
            /* If call context list is null means there are no active calls on 
             * line then return the block*/
            if (ICF_NULL == p_line_blk->p_call_ctx_list)  
#endif
			{
				/* Choose the first free line with the user address. */
                *p_status = (icf_uint32_t)ICF_LINE_STATUS_FREE;
                *p_line_id = (icf_line_id_t )count;
#ifdef ICF_MULTILINE
                free_line_found = ICF_TRUE;
                /* 
                 * Set the priority to the priority of user found 
                 */
                highest_priority = priority;

                /* Check if the free line was the one having user with highest
                 * priority then we would stop the seach else we would continue
                 * the search to find out a line with higher priority*/
                if(ICF_USER_HIGHEST_PRIORITY == priority)
                {
#endif
                    break;
#ifdef ICF_MULTILINE
                }
                else
                {
                    /* 
                     * Continue searching to find if there is a line with user 
                     * configured with a higher priority
                     */  
                    continue;
                }
#endif
            }
#ifdef ICF_MULTILINE
            /* 
             * If we haven't found a free line yet we should try and find out 
             * line which has all calls in release phase or a line with minimum 
             * call waiting calls
             */
            else if(ICF_FALSE == free_line_found)
#else
             /* 
              * If the line had calls in execution the try and find if it has 
              * all calls in release phase or if it has call waiting service 
              * active
              */    
            else
#endif
            {
                p_call_ctx_list =  p_line_blk->p_call_ctx_list;

                all_call_in_release_state = ICF_TRUE;
                some_call_in_setup_phase = ICF_FALSE;
                
                while (ICF_NULL != p_call_ctx_list)
                {
                    p_call_ctx = p_call_ctx_list->p_call_ctx;

					 if (ICF_REL_TYPE_INVALID == p_call_ctx->release_type)
                     {
                         /* It imples that the call is not in releasing phase,
                         * or media not cleared or call cleared from PA not done
                         */
                         all_call_in_release_state = ICF_FALSE;
                         /* It imples it is either in setup phase or active
                         * phase */
                        if (ICF_NULL == (ICF_CALL_STATUS_LOCAL_HELD & 
                                    p_call_ctx->call_status))
                        {
                            /*Fix Merged for CSR_1_5400409*/
                            temp_active_calls_on_line ++;                            
                            
                            if (!(ICF_CALL_CONNECT_DONE & 
                                    p_call_ctx->common_bitmask))
                            {
                               some_call_in_setup_phase = ICF_TRUE;
                               /* Implies in setup phase. Hence this line 
                                * cannot be chosen and also implies that 
                                * call cannot be presented due to held 
                                * status.
                                */
                               /*Fix Merged for CSR_1_5400409*/
                               /*break;*/
                            }
                        }
                    } /* End of if not in releasing state. */
                    p_call_ctx_list = p_call_ctx_list->p_call_ctx_next;
                } /* End of while loop searching in call blocks. */

                if (ICF_TRUE == all_call_in_release_state)
                {
                    *p_status = (icf_uint32_t)ICF_LINE_STATUS_FREE;
                    *p_line_id = (icf_line_id_t)count;
                    /* Go on searching for next line, as there may a line
                     * on which there are no held calls or may not be 
                     * in releasing state as well. */
                    line_with_rel_phs_found = ICF_TRUE;
                }

                /*Fix for CSR-1-5599808. In this case, IPTK willl check for the value 
                 *Max_limit_of_active_calls_per_line. If it is 1, then IPTK shall treat 
                 * Locally held call as Active Dialog. This is in accordance with IPTK design 
                 *before Multiple active dialogs patch.  
                 */
                else if((1 == Max_limit_of_active_calls_per_line) &&
                                (ICF_NULL != (ICF_CALL_STATUS_LOCAL_HELD & 
                                 p_call_ctx->call_status))
                                          &&
                         (ICF_NULL == (ICF_SERVICE_ACT_CALL_WAITING & 
                                    p_line_blk->service_activation_flag)))
                {
                     *p_status = (icf_uint32_t)ICF_LINE_STATUS_BUSY;
                     *p_line_id = count;
                }

                /*Fix Merged for CSR_1_5400409*/
                
                /* Here is the logic of uniform distribution over the calls for an user
                 * is implemented.If the user is configured on multiple line then it will
                 * check for that line which has minimum number of active calls and return.*/
                     
                else if(min_active_calls_on_line >temp_active_calls_on_line)
                {
                   min_active_calls_on_line = temp_active_calls_on_line;
                   
                   if(min_active_calls_on_line < 
                          Max_limit_of_active_calls_per_line)
                    {
                       *p_status = (icf_uint32_t)ICF_LINE_STATUS_FREE;
                       *p_line_id = count;
                        line_avail = ICF_TRUE;
                    }
                }                

#ifdef ICF_MULTILINE
                /*If the request was not for searching an alternate line the we 
                 * would do the following
                 * If there was no call in setup phase then we can offer a call 
                 *waiting call on this line
                 *For selecting the line for call waiting call check to see if
                 *the line had call waiting service active and number of call 
                 *waiting calls on line are lesser the previously selected line
                 *for offering call waiting call or has equal number of call 
                 *waiting call but the user with a higher priority
                 */
                else if (ICF_FALSE == search_alt_line &&
                        ((ICF_FALSE == some_call_in_setup_phase) &&
                         (ICF_FALSE == line_with_rel_phs_found) &&
                        ((ICF_SERVICE_ACT_CALL_WAITING & 
                            p_line_blk->service_subscription_flag) &&
                        ((current_min_call_waiting >
                         p_line_blk->num_call_waiting_calls) || 
                         (current_min_call_waiting ==
                         p_line_blk->num_call_waiting_calls &&
                         highest_priority > priority)))))
#else

                /*If there was no call in setup phase then we can offer a call 
                 *waiting call on this line*/
                /*Fix Merged for CSR_1_5400409*/    
                else if (ICF_FALSE == search_alt_line &&
                        ((ICF_FALSE == some_call_in_setup_phase) && 
                         (ICF_FALSE == line_with_rel_phs_found) &&
                        ((ICF_SERVICE_SUBS_CALL_WAITING & 
                            p_line_blk->service_subscription_flag) &&
                         (ICF_SERVICE_ACT_CALL_WAITING & 
                            p_line_blk->service_activation_flag) &&
                        (current_min_call_waiting >
                         p_line_blk->num_call_waiting_calls))) &&
                        (ICF_FALSE == line_avail))
#endif
                {
                    /* This implies active call and has lesser no. of 
                     * call waiting calls as compared to previous found.
                     */
                    *p_status = (icf_uint32_t)ICF_LINE_STATUS_BUSY;
                    *p_line_id = count;
                    current_min_call_waiting = 
                        p_line_blk->num_call_waiting_calls;
                    /* Go on searching for next line, as there may a 
                     * line which is free or all releasing 
                     * or having lesser no of call waiting call. */
                }
                else if (ICF_INVALID_LINE_ID == *p_line_id)
                {
                     *p_status = (icf_uint32_t)ICF_LINE_STATUS_BUSY;
                     *p_line_id = (icf_line_id_t)count;
                }

            } /* End of else where call blocks exist in line blocks. */
            /*Fixed merged for CSR_1_5400409*/
            /* ASB : Fix for the case 22 */
	        temp_active_calls_on_line = 0;
        } /* End of For loop for max line count */

        if(0 == search_count && (ICF_FALSE == user_found && 
                ICF_ADDRESS_PLAN_SIP_URI == search_address.addr_type))
        {
            /*If it is a SIP URI extract the username and check it for being a
             * valid E164 address.*/
            user_name_e164 = icf_dbm_ext_usrnam_n_val_e164(
                    p_glb_pdb,
                    p_user_address,
                    &search_address);
            
        }  
		if(ICF_TRUE != user_name_e164)
		{
			search_count += 1;
		}
		if(1 == search_count && (ICF_FALSE == user_found && 
                ICF_ADDRESS_PLAN_SIP_URI == p_user_address->addr_type))
		{
			if(ICF_SUCCESS == icf_dbm_chk_if_dom_is_local_ip(p_glb_pdb,p_user_address))
			{
                search_count += 1;
				continue;
			}
		}
        search_count++;    
	}while(search_count < max_search_count);

/* code for early call indication */
    if(ICF_FALSE == user_found)
    {
        *p_status = ICF_NO_USER_ON_LINE;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return (ret_val);

} /* End of icf_dbm_get_line_for_inc_call */


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_validate_line_for_ogn_call
 *
 * DESCRIPTION:   The function checks for the line to be free for making an 
 *                outgoing call.
 * INPUT      :   The function expects a valid line ID lesser than or 
 *                         equal to max lines configured             
 * RETURNS:       Success if the line is free else returns failure.
 *
 ************************************************************************/
icf_return_t
icf_dbm_validate_line_for_ogn_call(
    INOUT icf_glb_pdb_st *p_glb_pdb,
    IN    icf_line_id_t  line_id,
    OUT   icf_error_t    *p_ecode)
{
    /*Fix merged for CSR_1_5400409*/
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_line_data_st         *p_line_blk = ICF_NULL;
    icf_call_ctx_st          *p_call_ctx = ICF_NULL;
    icf_call_ctx_list_st     *p_call_ctx_list = ICF_NULL;
    icf_uint8_t               count_active_calls =0;
    icf_config_data_st       *p_config_data = ICF_NULL;
    icf_uint8_t               max_limit_of_active_calls =0 ;

    
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Fix Merged for CSR_1_5400409*/
    /* Retreive the pointer for global CFG data*/
    ret_val = icf_dbm_get_module_glb_data(
               p_glb_pdb,
               (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
               (icf_void_t *)&p_config_data,
               p_ecode);
    /* Assign the value of limit of maximum active calls per line
       that is configured*/

    max_limit_of_active_calls = p_config_data ->max_active_calls_per_line;

    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
                              line_id,
                              &p_line_blk,
                              p_ecode))
    {
        p_glb_pdb->p_call_ctx->release_cause = 
            ICF_CALL_CLEAR_REASON_LINE_NOT_CONFIGURED; 
        ret_val = ICF_FAILURE;
            
    }
    else
    {
       p_call_ctx_list = p_line_blk->p_call_ctx_list;

     /*  if (ICF_NULL != p_call_ctx_list)
       {
           p_call_ctx =  p_call_ctx_list->p_call_ctx;*/

       while (ICF_NULL != p_call_ctx_list)
       {               
           p_call_ctx =  p_call_ctx_list->p_call_ctx;
           /* Modified below if condition for the scenario when call is 
              on hold and resume in going on then call shall not be allowed */ 

           if ((!((ICF_REL_TYPE_INVALID != 
                 p_call_ctx->release_type) &&
                 ((ICF_REL_TYPE_PA_INITIATED == p_call_ctx->release_type) ||
                  (ICF_CALL_CLEAR_STATUS_PA_CLEARED & 
                   p_call_ctx->call_clear_status)) &&
                 (ICF_CC_07_AWT_DMS_RESP != p_call_ctx->call_state))) &&
                 ((ICF_NULL == 
                     (ICF_CALL_STATUS_LOCAL_HELD & p_call_ctx->call_status)) ||
                   ((ICF_NULL != 
                     (ICF_CALL_STATUS_LOCAL_HELD & p_call_ctx->call_status)) &&
                     (ICF_NULL != p_call_ctx->p_service_ctx) &&
                     (ICF_NULL != 
                        ((icf_service_ctx_st *)(p_call_ctx->p_service_ctx)) \
                        ->active_service_list.p_sl_ctx \
                        [ICF_INT_MODULE_CHR - ICF_INT_MODULE_SL_START - 1]))))
           {
               /* First check imples that the call is not in releasing phase,
                * or media not cleared or call cleared from PA not set */
               /* It is assumed that CC is not waiting for media ongoing
                * in case of sending DMS. It sends DMS and goes to awt DMS
                * even if media ongoing is not invalid. If this assumption
                * is false, then also check that the call state should not
                * be wait for media ongoing with next state as awt DMS. 
                * Second check implies that call is not locally held. 
                * For both these cases, the call shall not be allowed. */
               
               /*Fix Merged for CSR_1_5400409*/
               /* count the active calls on the line */	       
               count_active_calls ++;
       
               /* If the number of calls active on the line is already 
                * achieved max limit then return failure. */
               
               if(count_active_calls == max_limit_of_active_calls)
               {
                   ret_val = ICF_FAILURE;
                   break;
               }   
           }
           p_call_ctx_list = p_call_ctx_list->p_call_ctx_next;
       } /* End of while loop searching in call blocks. */

      /* } End if*/
    } /* End of else where call blocks exist in line block. */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
    
    return (ret_val);

} /* End of icf_dbm_validate_line_for_ogn_call */


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_check_if_user_exits_in_line
 *
 * DESCRIPTION:   The function checks for the line to find if a user exists 
 *                on the line .
 *                
 * RETURNS:       Success if the user exist on the line else returns failure.
 *
 ************************************************************************/      
icf_return_t
icf_dbm_check_if_user_exits_in_line(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        INOUT    icf_address_st   *p_user_addr,
        IN    icf_line_data_st *p_line_blk,
		IN    icf_boolean_t    match_only_user_name,
        INOUT    icf_error_t      *p_ecode)
{
    icf_return_t         ret_val = ICF_FAILURE ;
	icf_user_db_node_st  *p_user_db_node = ICF_NULL;
	icf_uint8_t          search_bitmask = 0;
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    
    if(ICF_TRUE == match_only_user_name)
	{
	    search_bitmask |= ICF_USER_DB_MATCH_ONLY_USER_NAME;
	}
    
    /***************ICF 8.0********************/
    /*Search bitmask are set to , public uri.contact uri 
      and associated uri.The change is done for the support for 
      implicit registerd uri*/
    
    search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;
    search_bitmask |= ICF_USER_DB_USE_CONTACT_URIS;
    search_bitmask |= ICF_USER_DB_USE_ASSOCIATED_URIS;
	
	
	if (ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line (
                                              p_glb_pdb,
                                              p_line_blk->line_id,
                                              p_user_addr,
                                              search_bitmask,
                                              &p_user_db_node,
                                              p_ecode))
    {
	    if(ICF_NULL != p_user_db_node)
		{
		    ret_val = ICF_SUCCESS;
		}
	}

    p_ecode = p_ecode;/*Just to remove warning*/
    ICF_FUNCTION_EXIT(p_glb_pdb)
            
    return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_fill_local_user_info
 *
 * DESCRIPTION:   The function copies the user name and address information in 
 *                the current call context from the line data of the line the 
 *                call is going on.
 *                
 * RETURNS:       Success after filling up the local user and name in curent 
 *                call context.
 *
 ************************************************************************/ 
icf_return_t 
icf_dbm_fill_local_user_info(
        INOUT          icf_glb_pdb_st     *p_glb_pdb)
{
    icf_line_data_st *p_line_data = ICF_NULL;
    icf_return_t ret_val =ICF_SUCCESS;
    icf_address_st *p_uri=ICF_NULL;
    icf_error_t ecode;
    
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Retreive line data block for the line id in current call context
     */
    p_line_data = *(p_glb_pdb->p_glb_data->p_p_line_data + 
            p_glb_pdb->p_call_ctx->line_id);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_line_data))
 
    p_glb_pdb->p_call_ctx->common_bitmask &= (~(ICF_LOCAL_NAME_PRESENT));
    /*
     * If string length for username is not null ie user name exist
     * Copy the user name in call context block attached to line
     */
    if(ICF_NULL != 
            p_line_data->user_name.str_len)
    {
        /*
         * Copy the first name in user name list in current call context
         */
        icf_port_memcpy(
                &p_glb_pdb->p_call_ctx->local_name,
                &p_line_data->user_name,
                (icf_uint32_t const)sizeof(icf_name_st));
        
        p_glb_pdb->p_call_ctx->common_bitmask |= ICF_LOCAL_NAME_PRESENT;
    }

    /*
     * Here we would be checking number of users on line as this function would 
     * call for an ongoing call which would mean user must have been configured.
     * Copy the first address in user address list in current call context
     */
    if(ICF_FAILURE == icf_dbm_get_default_uri_for_line (
        p_glb_pdb,
        p_line_data->line_id,
        &p_uri,
        &ecode
        ))
    {
        ret_val = ICF_SUCCESS;
    }
    else
    {
    /* Here we would be checking number of users on line as this function would 
    * call for an ongoing call which would mean user must have been configured.
    * Copy the first address in user address list in current call context
        */
        icf_port_memcpy(
            &p_glb_pdb->p_call_ctx->local_address,
            p_uri,
            (icf_uint32_t const)sizeof(icf_address_st));
    }


   
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    ICF_RESET_MODULE(p_glb_pdb)   

    return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_idx_for_ogn_ccx
 *
 * DESCRIPTION: The function looks in the call context pool to find a call 
 *              context block index for an outgoing call .
 *              if present return the block index
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_get_idx_for_ogn_ccx(
        icf_glb_pdb_st *p_glb_pdb,
        icf_call_id_t  call_id,
        icf_uint16_t   *p_index,
        icf_error_t    *p_ecode)
{
    icf_return_t       ret_val = ICF_FAILURE;
    icf_uint16_t       temp_index = 0;
    icf_dbm_pool_info_st *p_call_ctx_pool = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* 
     * Get the pool address for call context pool
     */ 
    p_call_ctx_pool =  icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_CALL_CTX);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_pool))
    
    while(temp_index < p_call_ctx_pool->pool_size) 
    {
      if(call_id == (((icf_call_ctx_st *)p_call_ctx_pool->p_blk_array) +
               temp_index)->call_id)
      {
          *p_index = temp_index;
          ret_val = ICF_SUCCESS;
      }
      temp_index += 1;
    }

    p_ecode = p_ecode;

    ICF_FUNCTION_EXIT(p_glb_pdb)    

    return ret_val;
}/*End icf_dbm_get_idx_for_ogn_ccx*/

#if 0
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_idx_for_inc_ccx
 *
 * DESCRIPTION: The function looks in the call context pool to find a call 
 *              context block index for an incomming call .
 *              if present return the block index
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_get_idx_for_inc_ccx(
        icf_glb_pdb_st *p_glb_pdb,
        icf_call_id_t  call_id,
        icf_uint16_t   *p_index,
        icf_error_t    *p_ecode)
{
    icf_config_data_st *p_config_data = ICF_NULL;
    icf_dbm_data_st    *p_dbm_data = ICF_NULL;
    icf_return_t       ret_val = ICF_FAILURE;
    icf_uint16_t       temp_index = 0;
    icf_uint16_t       max_num_of_calls = 0;

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

    /*Reinitialize retval to default value*/
    ret_val = ICF_FAILURE;

    max_num_of_calls = p_config_data->max_audio_calls +  
                       p_config_data->max_video_calls;

    while(temp_index < max_num_of_calls) 
    {
      if(call_id == 
              (((icf_call_ctx_st *)(p_dbm_data->p_call_cntxt_blk)) + 
               temp_index)->call_id)
      {
          *p_index = temp_index;
          ret_val = ICF_SUCCESS;
      }
      temp_index += 1;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)    

    return ret_val;
}
#endif

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_idx_for_inc_scx
 *
 * DESCRIPTION: The function looks in the SIC context pool to find a sic 
 *              context block index for an incomming call/transaction .
 *              if present return the block index
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_get_idx_for_inc_scx(
        icf_glb_pdb_st *p_glb_pdb,
        icf_call_id_t  call_id,
        icf_uint16_t   *p_index,
        icf_error_t    *p_ecode)
{    
    icf_dbm_data_st    *p_dbm_data = ICF_NULL;
    icf_return_t       ret_val = ICF_FAILURE;
    icf_uint16_t       temp_index = 0;
    icf_uint16_t       max_num_of_calls = 0;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /*Reinitialize retval to default value*/
    ret_val = ICF_FAILURE;

    max_num_of_calls = ICF_CMN_SIC_MAX_CALL_CONTEXTS;

    while(temp_index < max_num_of_calls) 
    {
      if(call_id == 
              (((icf_sic_ctx_st *)(p_dbm_data->p_sic_cntxt_blk)) + 
               temp_index)->call_id)
      {
          *p_index = temp_index;
          ret_val = ICF_SUCCESS;
          break;
      }
      temp_index += 1;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)    

    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_ong_call_ctx
 *
 *  DESCRIPTION : This function returns the address of data block for
 *                   the module whose id is been given   
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t
icf_dbm_get_ong_call_ctx(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_uint16_t   call_index,
        OUT   icf_call_ctx_st **p_p_call_ctx,
        OUT   icf_error_t     *p_ecode)
{
    icf_uint16_t counter = 0,temp_index = 0;
   icf_return_t ret_val = ICF_FAILURE;
    icf_dbm_pool_info_st *p_call_ctx_pool = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* 
     * Get the pool address for call context pool
     */ 
    p_call_ctx_pool =  icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_CALL_CTX);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_pool))

    /*
     * Traverse the all call context in pool to find the blocks with valid call 
     * id. These would be the blocks for which call is going on. Break when nth 
     * such call call context block is found where n is identified by the value 
     * of call_index passed as argument.
     */
    while(temp_index < p_call_ctx_pool->pool_size) 
    {
      if(ICF_INVALID_CALL_ID !=
              (((icf_call_ctx_st *)(p_call_ctx_pool->p_blk_array)) + 
               temp_index)->call_id)
      {
          /*
           * Would keep a could of the number of call context with a valid call 
           * id found
           */
          counter += 1; 
          /*If the call context block found for count_index then return the call
           * context block address */
          if(call_index == counter)
          {
              *p_p_call_ctx = 
                  ((icf_call_ctx_st *)p_call_ctx_pool->p_blk_array) + 
                  temp_index;
              
              ret_val = ICF_SUCCESS;
              break;
          }
      }
      /*
       * Keeps a track of current index in DBM pool
       */ 
      temp_index += 1;
    }
	
	p_ecode =p_ecode;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val; 
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_chk_line_service
 *
 * DESCRIPTION: The function looks for various services being activated on the 
 *              at various trigger points and returns status accordingly
 * INPUT:        The function expects a valid line ID lesser than or 
 *               equal to max lines configured            
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_chk_line_service(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_uint8_t   trigger_cause,
        IN    icf_line_id_t  line_id,
        OUT   icf_uint32_t   *p_status,
        OUT   icf_error_t    *p_ecode)
{
    icf_line_data_st *p_line_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_config_data_st  *p_config_data = ICF_NULL;
#ifdef ICF_MULTILINE
    icf_priority_id_t      priority = ICF_NULL,
                           highest_priority = ICF_NULL;
    icf_line_id_t       line_with_highest_priority = line_id;
    icf_uint8_t         count = ICF_NULL, line_count = ICF_NULL;
#endif

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    /* SDP Tunnelling Enhancements */
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_config_data,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_PRINT(((icf_uint8_t *)"[DBM] : p_config_data is NULL.\n"))
        ICF_FUNCTION_EXIT(p_glb_pdb)
        ICF_RESET_MODULE(p_glb_pdb)
        return ret_val;
    }

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
 
    p_line_data = *(p_glb_pdb->p_glb_data->p_p_line_data + line_id);

    /* SDP Tunnelling Enhancements */
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_line_data,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_PRINT(((icf_uint8_t *)"[DBM] : p_line_data is NULL.\n"))
        ICF_FUNCTION_EXIT(p_glb_pdb)
        ICF_RESET_MODULE(p_glb_pdb)
        return ret_val;
    }

    *p_status = (icf_uint32_t)ICF_STATUS_CALL_R_SRV_TERMINATE;

    /* In tunnel mode, we are not supporting call waiting and call forwarding 
       services. Thus even if the application has subscribed and activated these
       services, internally we will disable them by resetting the respective
       service activation flags and by setting the maximum number of calls in
       waiting to 0.
    */
ICF_CHECK_TUNNEL_MODE_START(p_glb_pdb,ICF_TRUE)
    
    /* Diable call waiting service */
    p_config_data->max_num_of_calls_in_call_waiting = ICF_NULL;
    p_line_data->service_activation_flag &= ~ICF_SERVICE_ACT_CALL_WAITING;

    /* Disable call forwarding service */
    p_line_data->service_activation_flag &= ~ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL;
    p_line_data->service_activation_flag &= ~ICF_SERVICE_ACT_CALL_FWD_BUSY;
    p_line_data->service_activation_flag &= ~ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER;

ICF_CHECK_TUNNEL_MODE_END        
    

    /*
     * If line is idle check for call forwarding unconditional to be active. If 
     * it is active return status as ICF_STATUS_CALL_FWD_UNCOND
     */

     /*
	  * Fix for CSR 1-7644788 SPR 19926. IPTK shall send 302 response when CFU service
	  * is configured on that line even if any call is going on that line.
	  */
    if(ICF_NULL != 
            (ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL & 
                    p_line_data->service_activation_flag))
    {
        *p_status = (icf_uint32_t)ICF_STATUS_CALL_FWD_UNCOND;
    }

#ifdef ICF_MULTILINE
    line_count = p_config_data->max_lines;
	
    /* Initialize higest priority to that of current line ie the the line id
	* passed as argument*/
    highest_priority = icf_dbm_get_user_priority(
		p_glb_pdb,
		&p_glb_pdb->p_call_ctx->local_address,
		p_line_data);
	
		/*
		*  If the user was not configured with highest priority on the line then 
		* try to find out a line with higher priority 
	*/
    for (count = 0; ICF_USER_HIGHEST_PRIORITY != highest_priority &&
		count <= line_count; count++)
    {
        if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
			(icf_line_id_t const)(count),
			&p_line_data,
			p_ecode))
        {
		/* Check for next line block. It is possible that there
		* may be holes in line data configuration. So leave
			* that. */
            continue;
        }
        else
        {
            priority = icf_dbm_get_user_priority(
				p_glb_pdb,
				&p_glb_pdb->p_call_ctx->local_address,
				p_line_data);
            
            if(ICF_NULL == priority)
            { 
			/* Check next line block where this user address exist
			* in that line. 
				*/
                continue;
            }
            else if (priority < highest_priority)
            {
                highest_priority = priority;
                line_with_highest_priority = count;
            }
        }
    }
	
    p_line_data = *(p_glb_pdb->p_glb_data->p_p_line_data + 
		line_with_highest_priority);
	
		/*
		* If line is busy check for call forwarding busy or call waiting to be 
		* active. If it is active return status as 
		* ICF_STATUS_CALL_FWD_BUSY_N_CALL_WAITING. If only call waiting is 
		* active check for number of call waiting calls on line to be lesser the
		* max_call_waiting_call else dont return this status
	*/
    if(ICF_CAUSE_LINE_BUSY == trigger_cause &&  (ICF_NULL != 
		(ICF_SERVICE_ACT_CALL_FWD_BUSY & 
		p_line_data->service_activation_flag) || 
		(ICF_NULL != 
		(ICF_SERVICE_ACT_CALL_WAITING & 
		p_line_data->service_activation_flag) && 
		p_line_data->num_call_waiting_calls < 
		p_config_data->max_num_of_calls_in_call_waiting)))
#else

    /*
     * If line is busy check for call forwarding busy or call waiting to be 
     * active. If it is active return status as 
     * ICF_STATUS_CALL_FWD_BUSY_N_CALL_WAITING. If only call waiting is 
     * active check for number of call waiting calls on line to be lesser the
     * max_call_waiting_call else dont return this status
     */
    else if(ICF_CAUSE_LINE_BUSY == trigger_cause &&  (ICF_NULL != 
            (ICF_SERVICE_ACT_CALL_FWD_BUSY & 
                    p_line_data->service_activation_flag) || 
            (ICF_NULL != 
            (ICF_SERVICE_ACT_CALL_WAITING & 
                    p_line_data->service_activation_flag) && 
            p_line_data->num_call_waiting_calls < 
            p_config_data->max_num_of_calls_in_call_waiting)))

    {
        *p_status = (icf_uint32_t)ICF_STATUS_CALL_FWD_BUSY_N_CALL_WAITING;
    }
#endif
    /*
     * If alerting timer expired check for call forwarding no answer to be 
     * active. If it is active return status as ICF_STATUS_CALL_FWD_NO_ANS
     */
    else if(ICF_CAUSE_NO_ANSWER == trigger_cause &&  ICF_NULL != 
            (ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER & 
                    p_line_data->service_activation_flag))
    {
        *p_status = (icf_uint32_t)ICF_STATUS_CALL_FWD_NO_ANS;
    }
    /*
     * If the call was rejected by user check call forwarding busy to be  
     * active. If it is active return status as ICF_STATUS_CALL_FWD_BUSY_LINE
     */
     else if((ICF_CAUSE_CALL_REJECT_BY_USER == trigger_cause || 
             ICF_CAUSE_RESOURCE_NOT_AVAILABLE == trigger_cause)&&  
            (ICF_NULL != 
            (ICF_SERVICE_ACT_CALL_FWD_BUSY & 
                    p_line_data->service_activation_flag))) 
    {
        *p_status = (icf_uint32_t)ICF_STATUS_CALL_FWD_BUSY_LINE;
    }
    /*
     * Else if user is not subscribed for above service and line is idle
     * return status as ICF_STATUS_CONTINUE
     */
    else if(ICF_CAUSE_LINE_FREE == trigger_cause && 
            ICF_STATUS_CALL_R_SRV_TERMINATE == *p_status)
    {
        *p_status = (icf_uint32_t)ICF_STATUS_CONTINUE;
    }

#ifdef ICF_MULTILINE
    if(ICF_STATUS_CONTINUE != *p_status)
    {
        p_glb_pdb->p_call_ctx->service_line_id = line_with_highest_priority;
    }
#else
    if((ICF_STATUS_CONTINUE != *p_status) && (ICF_NULL != p_glb_pdb->p_call_ctx))
    {
        p_glb_pdb->p_call_ctx->service_line_id = line_id;
    } 

#endif

    p_ecode = p_ecode;/*To remove warning*/

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
}

#ifdef ICF_MULTILINE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_user_priority
 *
 * DESCRIPTION:   The function checks for the line to find if a user exists 
 *                on the line and returns its priority.In case the user was not
 *                 present it returns the priority as ICF_NULL
 *                
 * RETURNS:       Returns a valid user priority is user was found on line else 
 *                returns ICF_NULL.
 *
 ************************************************************************/      
icf_priority_id_t
icf_dbm_get_user_priority(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        IN    icf_address_st   *p_user_addr,
        IN    icf_line_data_st *p_line_blk)
{
    icf_uint8_t user_count = 0;
    icf_priority_id_t priority = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_boolean_t user_match = ICF_TRUE;
    /* THese two variables would be contain value 1 if the address is preceeded 
     * by '+' else would contain value 0 */
    icf_uint8_t   user_len_diff = 0,
                     line_len_diff = 0;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    if('+' == p_user_addr->addr_val.str[0])
    {
        /* the address is preceeded by '+'*/
        user_len_diff = 1;
    }
    
    while(user_count < p_line_blk->num_of_user_addr)
    {

        if('+' == p_line_blk->p_user_addr_list[user_count].addr_val.str[0])
        {
            /* the address is preceeded by '+'*/
            line_len_diff = 1;
        }

        /* 
         * Compare the user adress passed as argument with the user in the list.
         * If it matches return the priority for the user from the priority list
         */

         /* 
         * Check if either the string length of address mismatch or the address 
         * type of two addresses mismatch and either of the address type is 
         * SIP_URI then set user_match to false
         */ 
        if ((p_line_blk->p_user_addr_list[user_count].addr_val.str_len - 
                    line_len_diff) != 
                (p_user_addr->addr_val.str_len - user_len_diff) || 
                (p_line_blk->p_user_addr_list[user_count].addr_type != 
                 p_user_addr->addr_type && 
                    (ICF_ADDRESS_PLAN_SIP_URI == 
                     p_line_blk->p_user_addr_list[user_count].addr_type || 
                    ICF_ADDRESS_PLAN_SIP_URI == p_user_addr->addr_type))) 
        {
            user_match = ICF_FALSE;
        }
        else if(0 != icf_port_strcmp(
                    &(p_line_blk->p_user_addr_list[user_count].addr_val.str
                    [line_len_diff]), 
                    &(p_user_addr->addr_val.str[user_len_diff])))
        {
            user_match = ICF_FALSE;
        }


        if(ICF_TRUE == user_match)
        {
           priority = p_line_blk->p_user_priority_list[user_count];
           break;
        }
        else 
        {
            /* Reinitialize user_match to true*/
            user_match = ICF_TRUE;
        }

        user_count += 1;
        line_len_diff = 0;
    }/* while(user_count */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return priority;
}/*End icf_dbm_get_user_priority*/
#endif

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_ctx_id_for_opt
 *
 * DESCRIPTION:   The function would return a context identifier. The function 
 *                would be called by SSA when it gets options request from 
 *                network. SSA would pass on this identifier to MMI when 
 *                making media capability request.
 *                 
 * RETURNS:       Returns success
 *
 ************************************************************************/

icf_boolean_t
icf_dbm_get_ctx_id_for_opt (
    INOUT icf_glb_pdb_st *p_glb_pdb,
    OUT    icf_call_id_t *p_call_id,
    OUT   icf_error_t    *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_dbm_data_st *p_dbm_data = ICF_NULL;
     
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /* 
     * If the is the last options context identifier in the range the reset the
     * last_opt_ctx_id_ret to range start
     */  
    if(ICF_DBM_OPT_CTX_ID_END == p_dbm_data->last_opt_ctx_id_ret)
    {
        p_dbm_data->last_opt_ctx_id_ret = ICF_DBM_OPT_CTX_ID_START;
    }
    /* Increment the options identifier*/
    else
    {
        p_dbm_data->last_opt_ctx_id_ret += 1;
    }
    
    *p_call_id = p_dbm_data->last_opt_ctx_id_ret;
        
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return(ret_val);

}/*End icf_dbm_get_ctx_id_for_opt*/

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_alert_timer_dur
 *
 * DESCRIPTION: The function looks for the call being a call waiting call and 
 *              various services being active on the line. It would retreive the
 *              call waiting status and line id from the current call context 
 *              and return the alering timer duration depending on call type 
 *              and service active on line
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_get_alert_timer_dur(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_duration_t *p_alert_timer_dur,
        OUT   icf_error_t    *p_ecode)
{
    icf_line_data_st *p_line_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_config_data_st *p_config_data = ICF_NULL;
    icf_return_t return_val = ICF_SUCCESS;

    /* SPR 18880 : Added Null checks */
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"[DBM] : p_glb_pdb is NULL.\n"))
        return ICF_FAILURE;
    }

    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_glb_pdb,
                                          p_glb_pdb->p_glb_data,
                                          (*(p_glb_pdb->p_glb_data->p_p_line_data)),
                                          p_glb_pdb->p_call_ctx,
                                          return_val)
    if(ICF_FAILURE == return_val)    
    {
        /* return failure. Not enough data to proceed.*/
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }
    
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ret_val = icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_config_data,
                             return_val)
    if(ICF_FAILURE == return_val)    
    {
        /* return failure. Not enough data to proceed.*/
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    p_line_data = *(p_glb_pdb->p_glb_data->p_p_line_data + 
            p_glb_pdb->p_call_ctx->line_id);

    /*
     * If current call is a call waiting call
     */
    if(ICF_TRUE == p_glb_pdb->p_call_ctx->call_waiting_call)
    {
           /*
            * If call forwarding is active on the line
            */ 
           if(ICF_NULL != (ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER & 
                    p_line_data->service_activation_flag))
           {
               *p_alert_timer_dur = p_config_data->cw_n_cnd_alert_timer;
           }
           else
           {
               *p_alert_timer_dur = p_config_data->cw_alert_timer;              
           }
    }
    /*
     * The call is not a call waiting call
     */
    else
    {      
           /*
            * If call forwarding is active on the line
            */ 
           if(ICF_NULL != (ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER & 
                    p_line_data->service_activation_flag))
           {
               #if 1 /*ZyXEL , Jason , [RD Internal #231]*/
				/*BL2348_iptk let's noans fwd timer per-line*/
           		*p_alert_timer_dur = p_line_data->cnd_alert_timer;
		  		/*BL2348_iptk end*/
		#else
               	*p_alert_timer_dur = p_config_data->cnd_alert_timer;
		#endif
           }
           else
           {
               *p_alert_timer_dur = p_config_data->alerting_timer;
           }
    }
    
    p_ecode = p_ecode;/*To remove warning*/

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_decrease_call_waiting
 *
 * DESCRIPTION: The function decrements the number of call waiting calls on 
 *              the line
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t
icf_dbm_decrease_call_waiting(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_error_t *p_ecode)
{
    icf_line_data_st *p_line_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(ICF_INVALID_LINE_ID != line_id)
    {
	    /*
	     * Get line pointer for the line_id
    	 */
	    p_line_data = *(p_glb_pdb->p_glb_data->p_p_line_data + line_id);

        /* SPR 19532 */
	    if ((ICF_NULL != p_line_data) &&
            (ICF_NULL < p_line_data->num_call_waiting_calls))
	    {
	    	/*
	     	 * Decrease number of call waiting calls
      	 	 */
    		p_line_data->num_call_waiting_calls -= 1;
		}
	}

    p_ecode = p_ecode;/*To remove warning*/

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
    
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_ext_usrnam_n_val_e164
 *
 * DESCRIPTION: The function takes as an argument a SIP URI and extracts the 
 *              username from it and if the user name is a valid E164 number 
 *              then returns the E164 number in OUT parameter p_e164 and 
 *              ICF_TRUE as return value
 *
 * RETURNS: Returns ICF_TRUE if user name in a SIP URI is a valid E164 Number
 *          and returns the e164 number in OUT parameter p_e164
 *
************************************************************************/
icf_boolean_t
icf_dbm_ext_usrnam_n_val_e164(
       
 	INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_address,
        OUT   icf_address_st *p_e164)
{
   icf_return_t ret_val = ICF_FALSE;
    icf_uint8_t counter = 0;
    icf_boolean_t  e164_number = ICF_TRUE;
    icf_uint8_t    sip_seperator = '@';


    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }

	p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
       
    /*Copy the string before the @ symbol*/  
    while((counter<p_address->addr_val.str_len ) && (counter < ICF_MAX_STR_LEN ) 
            && (p_address->addr_val.str[counter] != sip_seperator))
    {
/*        if(isxdigit(p_address->addr_val.str[counter]))*/
		if (((p_address->addr_val.str[counter] >= 48) && (p_address->addr_val.str[counter] <= 57)
			)|| ((p_address->addr_val.str[counter] >= 65) && (p_address->addr_val.str[counter] <= 70)
			)|| ((p_address->addr_val.str[counter] >= 'a') && (p_address->addr_val.str[counter] <= 'f')))
        {
            p_e164->addr_val.str[counter] = p_address->addr_val.str[counter];
        }
        else
        {
            /* If number does not contain [0-f]
             *  it is not valid number  */
            e164_number = ICF_FALSE;
            break;
        }
        counter++;
    }

    if((ICF_TRUE == e164_number) && (counter < ICF_MAX_STR_LEN))
    {
        /*Concatenate the string termination character*/
        p_e164->addr_val.str[counter] = '\0';

        /* Null Char not include in Length*/
        p_e164->addr_val.str_len = counter;     
        
        p_e164->addr_type = ICF_ADDRESS_PLAN_E164;
    }
    
    ret_val = ret_val;

	ret_val = ret_val;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return e164_number;    
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_chk_if_local_usr
 *
 * DESCRIPTION: The function would traverse all the line to identify if the user
 *              exists on any of the line. If it does exist meaning this is a 
 *              local user would return ICF_SUCCESS else would return 
 *              ICF_FAILURE
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t
icf_dbm_chk_if_local_usr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT    icf_address_st *p_user_address,
        OUT   icf_error_t    *p_ecode)
{
    icf_line_data_st *p_line_blk = ICF_NULL;
    icf_return_t ret_val = ICF_FAILURE;
    icf_uint32_t max_line_count = 0;   
    icf_uint32_t count = 0;
    icf_boolean_t user_name_e164 = ICF_FALSE;
    icf_config_data_st *p_config_data = ICF_NULL;
    icf_address_st e164_address;
    /* CSR 1-6013602 Merge */
    icf_address_st temp_addr = {{0,"\0"},0};;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);
    
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))   

    /*Reset ret_val to default value*/
    ret_val = ICF_FAILURE;    
    /* CSR 1-6013602 Merge  */
    /* take the address in local variable to use it for 
     * address matching 
     */
    icf_port_memcpy(&temp_addr,p_user_address,ICF_PORT_SIZEOF(icf_address_st));


    max_line_count = p_config_data->max_lines;

    /*
     *If the address happens to be SIP URI . Extract the user name and check if 
     *it is a valid E164 number. If valid E164 number is found then search for 
     *the E164 number as well. 
     */
    if(ICF_ADDRESS_PLAN_SIP_URI == temp_addr.addr_type)
    {
        /*If it is a SIP URI extract the username and check it for 
         * being a valid E164 address.*/
       /* CSR 1-6013602 Merge */
        user_name_e164 = icf_dbm_ext_usrnam_n_val_e164(
                p_glb_pdb,
                &temp_addr,
                &e164_address);
    }

    /*Traverse all lines to find the user*/
    for (count = 0; count < max_line_count; count++)
    {
        if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
                    (icf_line_id_t const)(count),
                    &p_line_blk,
                    p_ecode))
        {
            /* CSR 1-6013602 Merge */
            if (ICF_FAILURE == icf_dbm_check_if_user_exits_in_line(
                        p_glb_pdb,
                        &temp_addr,
                        p_line_blk,
						(icf_boolean_t) ICF_FALSE,
                        p_ecode))
            {
                /*If a valid E164 user name was found in SIP URI*/
                if(ICF_TRUE == user_name_e164)
                {
                    /*Check if a user with this E164 address found on line*/   
                    if (ICF_SUCCESS == 
                            icf_dbm_check_if_user_exits_in_line(
                                p_glb_pdb,
                                &e164_address,
                                p_line_blk,
						(icf_boolean_t) ICF_FALSE,
                                p_ecode))
                    {
                        /*User Found*/
                        ret_val = ICF_SUCCESS;
                        break;
                    }
                }
            }
            else
            {
                /*User Found*/
                ret_val = ICF_SUCCESS;
                break;
            }
        } /* EndIf line is configured */

    } /* End of For loop for max line count */


    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
    
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_ext_usrnam
 *
 * DESCRIPTION: The function takes as an argument a SIP URI and extracts the 
 *              username from it 
 *
 * RETURNS: Returns ICF_TRUE if user name in a SIP URI is a valid E164 Number
 *          and returns the e164 number in OUT parameter p_e164
 *
************************************************************************/
icf_return_t
icf_dbm_ext_usrnam(
	INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_address,
        OUT   icf_address_st *p_user_name)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint16_t counter = 0;
    icf_uint8_t  seperator = '@';


    if(ICF_NULL == p_glb_pdb)
    {
        return ICF_FAILURE;
    }    
	if ( ICF_NULL == p_address) return ICF_FAILURE; 
	p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
       
    if(ICF_ADDRESS_PLAN_TEL_URI == p_address->addr_type)
    {
        seperator = ';';
    }     
    /*Copy the string before the @ symbol*/  
    while((counter<p_address->addr_val.str_len) && (counter < ICF_MAX_STR_LEN)   
            && (p_address->addr_val.str[counter] != seperator))
    {

        p_user_name->addr_val.str[counter] = p_address->addr_val.str[counter];
        counter++;
        if ((ICF_MAX_STR_LEN - 1) == counter)
            break;
    }
    if ( counter < ICF_MAX_STR_LEN) 
	{
		p_user_name->addr_val.str[counter] = '\0';
		p_user_name->addr_val.str_len = counter;
    	p_user_name->addr_type = p_address->addr_type;
	}
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;    
}

/*******************************************************************************
 *
 * FUNCTION:        icf_dbm_convert_to_tran_addr
 *
 * DESCRIPTION:     This function converts the IP address
 *                  stored in the SDP body of SIP message
 *                  to the transport address understandable by the
 *                  ICF
 *
 ******************************************************************************/
icf_return_t icf_dbm_chk_if_dom_is_local_ip(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_address)
{
    icf_uint8_t domain[ICF_MAX_STR_LEN] = "\0";
	icf_uint8_t *p_temp = ICF_NULL;
    icf_uint8_t str_tok[ICF_MAX_STR_LEN] = "\0";
	icf_transport_address_st ip_address = {{0,{{0,"\0"}}},0};
	icf_config_data_st *p_config_data = ICF_NULL;
	icf_return_t ret_val = ICF_SUCCESS;
	icf_error_t ecode = ICF_ERROR_NONE;
	
    p_temp = icf_port_strchr(p_address->addr_val.str,'@');

    if ((p_temp == ICF_NULL) && (p_address->addr_val.str[0] != '@'))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        p_temp += 1;
        /*Fix merged for CSR_1-5409550 */
        /* If p_address was of the form user@ip;transport=xxx or 
         * user@ip:port;transport=xxx, remove transport param from p_temp.
         * This will never happen for incoming call as Req-Uri and To will not have
         * transp-param. But it may be required for a direct-routed outgoing call.
         * So just a defensive code for now.
         */
        if (ICF_NULL != icf_port_strchr(p_temp, ';'))
        {
            p_temp = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)p_temp,";");
        }   
        if(ICF_NULL != p_temp)
        {
            icf_port_strcpy(str_tok,p_temp);
            /* If p_address was of the form user@ip:port, remove port from p_temp */
            if (ICF_NULL != icf_port_strchr(p_temp, ':'))
            {
                p_temp = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)str_tok,  (const icf_int8_t *)":");
            }
        }

        if (ICF_NULL != p_temp)
        {
            icf_port_strcpy(domain,p_temp);

            if(ICF_SUCCESS == icf_dbm_check_if_ip(domain))
            {
                /*
                 * Storing in temporary variables because of
                 * mismatch of data types
                 */
                if(icf_port_str_to_trans_addr(domain, &(ip_address.addr)) == ICF_FAILURE){
                    ret_val = ICF_FAILURE;
                }
                ret_val = icf_dbm_get_module_glb_data(     
                        p_glb_pdb,
                        (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                        (icf_void_t *)&p_config_data,
                        &ecode);

                ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))


                if(ICF_TRUE != icf_dbm_util_cmp_tran_addr_wo_port(
                            p_glb_pdb,
                            &p_config_data->self_ip_address,
                            &ip_address))
                {
                    ret_val = ICF_FAILURE;
                }
            }
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    return ret_val;
}
/***************************************************************************
 **
 ** FUNCTION:     icf_dbm_check_if_ip
 ** DESCRIPTION:  This function checks if the string passed is an IP address
 **               
 **
 ***************************************************************************/
icf_return_t icf_dbm_check_if_ip(icf_uint8_t *p_ip_address)
{
	return icf_port_check_if_ip(p_ip_address);
}


ICF_SESSION_CONTROL_START
ICF_CRM_START
/************************************************************************
 *
 * FUNCTION:     icf_dbm_get_preferred_identity
 * DESCRIPTION: The function would return the preferred identity for a 
 *              requested line. It is upto the calling function to allocate
 *              memory to the address pointer and free it later. If 
 *				no such user exists, this function will return failure with 
 *				ecode ICF_ECODE_NO_PREF_ID_EXISTS.
 *
 ************************************************************************/
icf_return_t icf_dbm_get_preferred_identity(
        INOUT      icf_glb_pdb_st       *p_glb_pdb,
        IN      icf_line_id_t        line_id,
        INOUT   icf_address_st       *p_pref_addr,
        OUT     icf_error_t          *p_ecode)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_address_st  *p_uri=ICF_NULL;
    icf_error_t     ecode;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* retrieve the preffered identity */
    if(ICF_FAILURE == icf_dbm_get_default_uri_for_line (
        p_glb_pdb,
        line_id,
        &p_uri,
        &ecode
        ))
    {
        ret_val = ICF_FAILURE; 
        *p_ecode = ICF_ECODE_NO_PREF_ID_EXISTS; 
    }
    else
    {
		if ( p_pref_addr ) {
        p_pref_addr->addr_type = (p_uri)->addr_type;
        icf_port_strcpy(p_pref_addr->addr_val.str, 
            (p_uri)->addr_val.str);
        p_pref_addr->addr_val.str_len = 
            (p_uri)->addr_val.str_len;
			}
    }


    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_RESET_MODULE(p_glb_pdb)
    return ret_val;
}

ICF_CRM_END
ICF_SESSION_CONTROL_END


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_pool_addr
 *
 * DESCRIPTION: This is a utility function to retreive the pool address for the 
 *               block types identified by block_type passed as argument
 *
 * RETURNS:    returns address of pool
 *
 ************************************************************************/
icf_dbm_pool_info_st * 
icf_dbm_get_pool_addr(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_dbm_block_type_et           block_type)
{
    icf_dbm_data_st *p_dbm_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_error_t  ecode;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Get the poiter to global DBM data */
    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                &ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
        
    ICF_FUNCTION_EXIT(p_glb_pdb)

    /* 
     * Return the block address for the pool 
     */
    return &p_dbm_data->pool[block_type];
}

/*******************************************************************************
 *
 * FUNCTION:       icf_cc_validate_mode_ogn_call
 *
 * DESCRIPTION:    This function would validate the self mode against the proxy 
 *                 mode to identify if the otg call could be initiated 
 *
 ******************************************************************************/
icf_return_t 
icf_dbm_validate_mode_ogn_call(
        INOUT icf_glb_pdb_st      *p_glb_pdb,        
        IN    icf_uint8_t    *p_transport_val,
        IN    icf_line_id_t  line_id,
        IN    icf_boolean_t  proxy_routed_call,
        OUT   icf_error_t *p_ecode)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_config_data_st       *p_config_data = ICF_NULL;
    icf_line_data_st         *p_line_data = ICF_NULL;
    icf_transport_mode_t     transport;

    ICF_FUNCTION_ENTER(p_glb_pdb)
	/* CSR 1-6421580 : SPR 19024 */
    /* This comparison should be case insensitive */
    if (!icf_port_strcmpi((icf_uint8_t*)p_transport_val, (icf_uint8_t *)"tcp"))
        transport = ICF_TRANSPORT_TYPE_TCP;
    else if (!icf_port_strcmpi((icf_uint8_t*)p_transport_val, (icf_uint8_t*)"tls"))
        transport = ICF_TRANSPORT_TYPE_TLS;        
    else
        transport = ICF_TRANSPORT_TYPE_UDP;

    if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
        /* Check if there is a mode mismatch in called party address mode and self 
     * mode and none of them is BOTH*/
    else if(p_config_data->self_mode != transport &&
          (ICF_TRANSPORT_MODE_BOTH != transport && 
           ICF_TRANSPORT_MODE_BOTH != p_config_data->self_mode))
    {
        ret_val = ICF_FAILURE;
    }
    /* Check if there is a mode mismatch in called party address mode and proxy 
     * address mode and node of them is BOTH*/
    else if(ICF_TRUE == proxy_routed_call)
    {
        /* Fetch the line context block */
        if(ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_glb_pdb,
                    line_id,
                    &p_line_data,
                    p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else if(p_line_data->transport_mode != transport &&
                (ICF_TRANSPORT_MODE_BOTH != transport &&
                 ICF_TRANSPORT_MODE_BOTH != p_line_data->transport_mode))
        {
            ret_val = ICF_FAILURE;
        }
    }/* End else if(ICF_TRUE = ... */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}/* End icf_dbm_validate_mode_ogn_call */

/*******************************************************************************
 *
 * FUNCTION:       icf_cc_validate_mode_ogn_call
 *
 * DESCRIPTION:    This function would look into the global default setting to 
 *                 identify if outgoing calls need to be initiated through proxy
 *
 ******************************************************************************/
icf_boolean_t 
icf_dbm_chk_ogn_call_thru_proxy(
        INOUT icf_glb_pdb_st      *p_glb_pdb,        
        OUT   icf_error_t *p_ecode)
{
    icf_boolean_t            ret_val = ICF_FALSE;
    icf_config_data_st       *p_config_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode))
    {
        ret_val = ICF_FALSE;
    }
    else if(ICF_CFG_CC_CALL_THRU_SIP_PROXY & p_config_data->default_settings)
    {
        ret_val = ICF_TRUE;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}/* End icf_dbm_validate_mode_ogn_call */

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_ong_sic_ctx
 *
 *  DESCRIPTION : This function returns the address of data block for
 *                   the module whose id is been given   
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t
icf_dbm_get_ong_sic_ctx(
        INOUT icf_glb_pdb_st		*p_glb_pdb,
        IN    icf_uint16_t		call_index,
        OUT   icf_sic_ctx_st		**p_p_sic_cntxt_blk,
        OUT   icf_error_t		*p_ecode)
{
    icf_uint8_t counter = 0;
    icf_dbm_data_st    *p_dbm_data = ICF_NULL;
    icf_return_t ret_val = ICF_FAILURE;
    icf_uint16_t temp_index = 0;
    icf_uint16_t max_num_of_calls;
    
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    max_num_of_calls = ICF_CMN_SIC_MAX_CALL_CONTEXTS;

    /*Set ret_val to default value ie ICF_FAILURE*/
    ret_val = ICF_FAILURE;
    /*
     * Traverse the all call context in pool to find the blocks with valid call 
     * id. These would be the blocks for which call is going on. Break when nth 
     * such call call context block is found where n is identified by the value 
     * of call_index passed as argument.
     */
    while(temp_index < max_num_of_calls) 
    {
      if(ICF_INVALID_CALL_ID !=
              (((icf_sic_ctx_st *)(p_dbm_data->p_sic_cntxt_blk)) + 
               temp_index)->call_id)
      {
          /*
           * Would keep a could of the number of call context with a valid call 
           * id found
           */
          counter += 1; 
          /*If the call context block found for count_index then return the call
           * context block address */
          if(call_index == counter)
          {
              *p_p_sic_cntxt_blk = 
                  ((icf_sic_ctx_st *)p_dbm_data->p_sic_cntxt_blk) + 
                  temp_index;

               p_glb_pdb->p_sic_ctx = *p_p_sic_cntxt_blk;
               p_glb_pdb->line_id = (*p_p_sic_cntxt_blk)->line_id;
              
              ret_val = ICF_SUCCESS;
              break;
          }
      }
      /*
       * Keeps a track of current index in DBM pool
       */ 
      temp_index += 1;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val; 
}



/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_add_public_uri_for_line
 *
 *  DESCRIPTION : This function adds a public user address in the user 
 *                database of the given line.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_add_public_uri_for_line (
                                    icf_glb_pdb_st	*p_glb_pdb,
                                    icf_line_id_t	line_id,
                                    icf_address_st 	*p_public_uri,
                                    icf_error_t 	    *p_ecode)
{
      icf_return_t           ret_val = ICF_FAILURE;
      icf_line_data_st       *p_line_data = ICF_NULL;
      icf_user_db_node_st    **p_p_curr_node;
   /* Get line data */
      if ( ICF_SUCCESS == 
          icf_dbm_fetch_line_cntxt_blk(
          p_glb_pdb, line_id,
          (icf_void_t *)&(p_line_data),
          p_ecode))
      {
          for(p_p_curr_node = &(p_line_data->p_user_db);
          ICF_NULL != *p_p_curr_node;
          p_p_curr_node=&((*p_p_curr_node)->p_next))
          {
              if (
                  ((*p_p_curr_node)->public_user.addr_type == p_public_uri->addr_type)
                  &&(0 == icf_port_strcmp((*p_p_curr_node)->public_user.addr_val.str, 
                  p_public_uri->addr_val.str))
                  )
 
              {
                  /* public_address of node is same as p_public_uri */
                  /**p_ecode = ICF_USER_ALREADY_PRESENT_ON_LINE; */
                  break;
              }
          }
          if(ICF_NULL == *p_p_curr_node)
          {
              /* Make new node */
              ICF_MEMGET(p_glb_pdb,
                  sizeof(icf_user_db_node_st),
                  ICF_MEM_COMMON,
                  *p_p_curr_node,
                  ICF_RET_ON_FAILURE,
                  p_ecode,
                  ret_val);

              /* SPR 19891 : Memset all variables to 0. This is to avoid
                 any junk pointer access while validation of subsequent 
                 users added on the same line.
                 If this memset operation is not done, the 
                 (*p_p_curr_node)->p_next variable remains uninitialised 
                 (with junk values) and IPTK resets while traversing 
                 through the User DB for this line in the function 
                 icf_cfg_validate_add_self_id_prms.
              */   
              icf_port_memset(*p_p_curr_node,0,sizeof(icf_user_db_node_st));

              /* Copy the public user id */
              icf_port_memcpy(&((*p_p_curr_node)->public_user),
                  p_public_uri,
                  sizeof(icf_address_st));
              ret_val = ICF_SUCCESS;
          }
      }
      return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_delete_public_uri_for_line 
 *
 *  DESCRIPTION : This function Deletes the given public user address 
 *                from the user database of the given line.
 *                Returns failure if given public uri is not found.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_delete_public_uri_for_line  (
                                    icf_glb_pdb_st	*p_glb_pdb,
                                    icf_line_id_t	line_id,
                                    icf_address_st 	*p_public_uri,
                                    icf_error_t 	    *p_ecode)
{
      icf_return_t           ret_val = ICF_FAILURE;
      icf_line_data_st       *p_line_data = ICF_NULL;
      icf_user_db_node_st    **p_p_curr_node;
   /* Get line data */
      if ( ICF_SUCCESS == 
          icf_dbm_fetch_line_cntxt_blk(
          p_glb_pdb, line_id,
          (icf_void_t *)&(p_line_data),
          p_ecode))
      {
          icf_dbm_clear_public_uri_node_for_line (p_glb_pdb, line_id, p_public_uri,
                          ICF_USER_DB_USE_ASSOCIATED_URIS | 
                          ICF_USER_DB_USE_CONTACT_URIS,
                          p_ecode);
          for(p_p_curr_node = &(p_line_data->p_user_db);
          ICF_NULL != *p_p_curr_node;
          p_p_curr_node=&((*p_p_curr_node)->p_next))
          {
              if (
                  ((*p_p_curr_node)->public_user.addr_type == p_public_uri->addr_type)
                  &&(0 == icf_port_strcmp((*p_p_curr_node)->public_user.addr_val.str, 
                  p_public_uri->addr_val.str))
                  )
 
              {
                  /* public_address found */
                  icf_user_db_node_st    *p_del_node = *p_p_curr_node;

                  *p_p_curr_node = (*p_p_curr_node)->p_next;

                  ICF_MEMFREE(p_glb_pdb, p_del_node, ICF_MEM_COMMON, p_ecode, ret_val);
                  ret_val = ICF_SUCCESS;
                  break;
              }
          }
      }
      return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_default_uri_for_line 
 *
 * DESCRIPTION : This function returns the pointer to the first 
 *               non-barred uri on the line.
 *               Returns failure if no non-barred uri exists on the line
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_default_uri_for_line (
                                                     icf_glb_pdb_st	*p_glb_pdb,
                                                     icf_line_id_t	line_id,
                                                     icf_address_st	**p_p_uri,
                                                     icf_error_t     *p_ecode
                                                     )
{
      icf_return_t           ret_val = ICF_FAILURE;
      icf_line_data_st       *p_line_data = ICF_NULL;
#ifdef IMS_CLIENT
      icf_user_db_node_st    *p_curr_node;
#endif
	  /* Get line data */
      if ( ICF_SUCCESS == 
          icf_dbm_fetch_line_cntxt_blk(
          p_glb_pdb, line_id,
          (icf_void_t *)&(p_line_data),
          p_ecode))
      {
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
          for(p_curr_node = p_line_data->p_user_db;
          ICF_NULL != p_curr_node;
          p_curr_node=p_curr_node->p_next)
          {
			  /* send the first assoc uri found */
			  if (ICF_NULL != p_curr_node->p_assoc_uri_list)
              {
                    
                  
                  *p_p_uri = &(p_curr_node->p_assoc_uri_list->user_addr);
                  ret_val = ICF_SUCCESS;
                  break;
              }
          }
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
/*ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)*/
		  /* The first uri on the line */
		  if(ICF_NULL != p_line_data->p_user_db)
		  {
           
			  *p_p_uri = &(p_line_data->p_user_db->public_user);
			  ret_val = ICF_SUCCESS;
		  }
/*ICF_CHECK_IMS_END*/
#endif
      }
      return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_search_address_list
 *
 * DESCRIPTION : This function searches the address list passed for 
 *               the given input uri.
 *				 Search bitmask would tell if full address is to be compared or
 *				 just user_name
 *               Returns ICF_SUCCESS if found, ICF_FAILURE otherwise
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_search_address_list(icf_glb_pdb_st            *p_glb_pdb,
                                         icf_address_list_node_st     *p_list,
                                         icf_address_st               *p_uri,
										 icf_uint8_t				  search_bitmask)
{

	icf_address_st user_name,
					user_on_line;
	icf_return_t ret_val = ICF_FAILURE;

	/* 
	 * If the search bitmask has ICF_USER_DB_MATCH_ONLY_USER_NAME bit set then we would 
	 * be comparing the user names only so extract username from the URI
	 */
	if(0 !=(ICF_USER_DB_MATCH_ONLY_USER_NAME & search_bitmask))
	{
		if (ICF_FAILURE == icf_dbm_ext_usrnam(p_glb_pdb,
			p_uri,
			&user_name))
		{
			ret_val = ICF_FAILURE;
		}
	}
	
    while(p_list)
    {
		/* 
	  	 * If the search bitmask has ICF_USER_DB_MATCH_ONLY_USER_NAME bit set then we would 
		 * be comparing the user names only so extract username from the URI
		 */
		if(0 !=(ICF_USER_DB_MATCH_ONLY_USER_NAME & search_bitmask))
		{
			if (ICF_FAILURE == icf_dbm_ext_usrnam(p_glb_pdb,
				&(p_list->user_addr),
				&user_on_line))
			{
				ret_val = ICF_FAILURE;
				break;
			}
			else if (ICF_TRUE == icf_dbm_util_cmp_addr(
				p_glb_pdb,
				&user_name,
				&user_on_line))
			{
				ret_val = ICF_SUCCESS;
				break;
			}
		}
		else if(ICF_TRUE == icf_dbm_cmp_addr_ignr_pre_plus(
			p_glb_pdb,
			&(p_list->user_addr),
			p_uri))
		{
			ret_val = ICF_SUCCESS;
			break;
		}
        p_list = p_list->p_next;
    }
    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_append_uri_to_address_list
 *
 * DESCRIPTION : This function appends a uri to the address list passed
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t 
icf_dbm_append_uri_to_address_list(
                                   icf_glb_pdb_st	*p_glb_pdb,
                                   icf_address_st	*p_uri,
                                   icf_address_list_node_st **p_p_node,
                                   icf_error_t     *p_ecode
                                   )
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Go to the last node */
    while(ICF_NULL != *p_p_node)
        p_p_node = &((*p_p_node)->p_next);

    /* Make new node */
    ICF_MEMGET(p_glb_pdb,
        sizeof(icf_address_list_node_st),
        ICF_MEM_COMMON,
        *p_p_node,
        ICF_RET_ON_FAILURE,
        p_ecode,
        ret_val);
    (*p_p_node)->p_next = ICF_NULL;
    
    /* Copy the uri */
    icf_port_memcpy(&((*p_p_node)->user_addr),
            p_uri,sizeof(icf_address_st));
    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_public_uri_node_from_uri_for_line
 *
 * DESCRIPTION : This function searches the user database for the given 
 *               line_id and returns the public uri list node pointer of 
 *               the node where the address given in p_uri is first found  
 *               in either the p-associated uri list or the contact list.
 *               The searching is controlled by the field search_bitmask
 *               if ICF_USER_DB_SEARCH_ASSOCIATED_URIS is set then the 
 *               p-associated uri lists are searched.
 *               if ICF_USER_DB_SEARCH_CONTACT_URIS is set then the 
 *               contact lists are searched.
 *               
 *               If both the bits are set then both the lists are searched
 *               If none of the bits are set then only the top 
 *               level public uri list is searched.
 *
 *               Returns failure if uri is not found in the user db
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t 
icf_dbm_get_public_uri_node_from_uri_for_line (
                                                  icf_glb_pdb_st	*p_glb_pdb,
                                                  icf_line_id_t	line_id,
                                                  icf_address_st	*p_uri,
                                                  icf_uint8_t search_bitmask,
                                                  icf_user_db_node_st	**p_p_node,
                                                  icf_error_t     *p_ecode
                                                  )
{
	icf_return_t           ret_val = ICF_FAILURE;
	icf_line_data_st       *p_line_data = ICF_NULL;
	icf_user_db_node_st    *p_curr_node;
	icf_address_st user_on_line, user_name;
    
      /* Get line data */
	if ( ICF_SUCCESS == 
		icf_dbm_fetch_line_cntxt_blk(
		p_glb_pdb, line_id,
		(icf_void_t *)&(p_line_data),
		p_ecode))
	{
        ret_val = ICF_SUCCESS;

		/* 
		 * If the bitmask for matching user name only is set the extract username from URI
		 */
		if(0 !=(ICF_USER_DB_MATCH_ONLY_USER_NAME & search_bitmask))
		{
			ret_val = icf_dbm_ext_usrnam(p_glb_pdb,
				p_uri,
				&user_name);
		}

		if(ICF_SUCCESS == ret_val)
		{
			/* Initialise ret_val to failure */
			ret_val = ICF_FAILURE;
			for(p_curr_node = p_line_data->p_user_db;
			ICF_NULL != p_curr_node;
			p_curr_node=p_curr_node->p_next)
			{					
			    /* If assoc_uri_list is present in user db node first check
                   the associated uris */
                if((ICF_NULL != p_curr_node->p_assoc_uri_list) && 
                        (ICF_USER_DB_USE_ASSOCIATED_URIS & search_bitmask))
                {
					ret_val = icf_dbm_search_address_list(p_glb_pdb,
						p_curr_node->p_assoc_uri_list,
						p_uri,
						search_bitmask);
                    if(ICF_SUCCESS == ret_val)
				    {
					    *p_p_node = p_curr_node;
					    break;
				    }
				}
                if(0 != (ICF_USER_DB_USE_PUBLIC_URIS & search_bitmask))
                {
                    /* Search Public Uri */
                    if(0 !=(ICF_USER_DB_MATCH_ONLY_USER_NAME & 
                               search_bitmask))
                    {
                        if (ICF_FAILURE == icf_dbm_ext_usrnam(p_glb_pdb,
                            &(p_curr_node->public_user),
                            &user_on_line))
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        else if (ICF_TRUE == icf_dbm_util_cmp_addr(
                            p_glb_pdb,
                            &user_name,
                            &user_on_line))
                        {
                            ret_val = ICF_SUCCESS;
                        }
                    }
                    else if(ICF_TRUE == icf_dbm_cmp_addr_ignr_pre_plus(
                        p_glb_pdb,
                        &(p_curr_node->public_user),
                        p_uri))
                    {
                        ret_val = ICF_SUCCESS;
                    }
                }
				if(ICF_SUCCESS != ret_val)
				{
				    /* URI not found till now , check  the contact list */
					if( (ICF_FAILURE == ret_val) && 
					    ( 0 != (ICF_USER_DB_USE_CONTACT_URIS & search_bitmask)))
					{
					    ret_val = icf_dbm_search_address_list(p_glb_pdb,
						           p_curr_node->p_contact_uri_list,
							       p_uri,
							       search_bitmask);
                    }
				}
				if(ICF_SUCCESS == ret_val)
				{
				    *p_p_node = p_curr_node;
					break;
                }
		    }    
        }
	}
    
	return ret_val;
}





icf_void_t icf_dbm_delete_address_list(icf_glb_pdb_st	*p_glb_pdb,
                               icf_address_list_node_st     **p_p_node,
                               icf_error_t     *p_ecode)
{
   icf_address_list_node_st  *p_next_node,*p_cur_node;
   icf_return_t ret_val;

    for(p_cur_node = *p_p_node;ICF_NULL != p_cur_node;p_cur_node = p_next_node)
    {
        p_next_node = p_cur_node->p_next;
        /* Free this node */
        ICF_MEMFREE(p_glb_pdb,
            p_cur_node,
            ICF_MEM_COMMON,
            p_ecode,
            ret_val);
    }
	*p_p_node = ICF_NULL;
}
    

icf_void_t icf_dbm_delete_public_uri_list(icf_glb_pdb_st	*p_glb_pdb,
                               icf_user_db_node_st	  **p_p_node,
                               icf_error_t     *p_ecode)
{
    icf_user_db_node_st	  **p_p_next_node;
    icf_return_t ret_val;
    while(*p_p_node)
    {
        p_p_next_node = &((*p_p_node)->p_next);
        icf_dbm_delete_address_list(p_glb_pdb,&((*p_p_node)->p_assoc_uri_list),p_ecode);
        icf_dbm_delete_address_list(p_glb_pdb,&((*p_p_node)->p_contact_uri_list),p_ecode);
        
        ICF_MEMFREE(p_glb_pdb,
            *p_p_node,
            ICF_MEM_COMMON,
            p_ecode,
            ret_val);
        
        p_p_node = p_p_next_node;
    }

}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_delete_public_uri_list_for_line
 *
 * DESCRIPTION : This function removed the p-associated uri list and/or 
 *               the contact list for a given line id.
 *               if ICF_USER_DB_USE_ASSOCIATED_URIS is set then the 
 *               p-associated uri list is deleted.
 *               if ICF_USER_DB_USE_CONTACT_URIS is set then the 
 *               contact list is deleted.
 *               If both the bits are set then both the lists are deleted
 *
 *               Returns failure if public uri is not found in the user db
 *               or none of the nits in the bitmask are set.
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_delete_public_uri_list_for_line (
                                            icf_glb_pdb_st	*p_glb_pdb,
                                            icf_line_id_t	line_id,
                                            icf_error_t     *p_ecode
                                            )
{
    icf_line_data_st       *p_line_data = ICF_NULL;
    icf_return_t   ret_val = ICF_FAILURE;

    if ( ICF_SUCCESS == 
        icf_dbm_fetch_line_cntxt_blk(
        p_glb_pdb, line_id,
        (icf_void_t *)&(p_line_data),
        p_ecode))
    {
        icf_dbm_delete_public_uri_list(p_glb_pdb,
            &(p_line_data->p_user_db),p_ecode);
        ret_val = ICF_SUCCESS;
    }
    return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_clear_public_uri_node_for_line 
 *
 * DESCRIPTION : This function removes the p-associated uri list and/or 
 *               the contact list for a given line id.
 *               if ICF_USER_DB_USE_ASSOCIATED_URIS is set then the 
 *               p-associated uri list is deleted.
 *               if ICF_USER_DB_USE_CONTACT_URIS is set then the 
 *               contact list is deleted.
 *               If both the bits are set then both the lists are deleted
 *
 *               Returns failure if public uri is not found in the user db
 *               or none of the bits in the bitmask are set.
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/


icf_return_t 
icf_dbm_clear_public_uri_node_for_line (
                                                  icf_glb_pdb_st	*p_glb_pdb,
                                                  icf_line_id_t	line_id,
                                                  icf_address_st	*p_uri,
                                                  icf_uint8_t delete_bitmask,
                                                  icf_error_t     *p_ecode
                                                  )
{
    icf_return_t           ret_val = ICF_FAILURE;
    icf_line_data_st       *p_line_data = ICF_NULL;
    icf_user_db_node_st	  *p_node=ICF_NULL;
    /* Get line data */
    if ( ICF_SUCCESS == 
        icf_dbm_fetch_line_cntxt_blk(
        p_glb_pdb, line_id,
        (icf_void_t *)&(p_line_data),
        p_ecode))
    {
        if( ICF_FAILURE == icf_dbm_get_public_uri_node_from_uri_for_line (
            p_glb_pdb,
            line_id,
            p_uri,
            ICF_USER_DB_USE_PUBLIC_URIS,
            &p_node,
            p_ecode
            ))
        {
            ret_val =ICF_FAILURE;
        }
        else
        {
            /* Uri found */
            if(delete_bitmask & ICF_USER_DB_USE_ASSOCIATED_URIS)
            {
                icf_dbm_delete_address_list(p_glb_pdb,
                    &((p_node)->p_assoc_uri_list),
                    p_ecode);
            }
            if(delete_bitmask & ICF_USER_DB_USE_CONTACT_URIS)
            {
                icf_dbm_delete_address_list(p_glb_pdb,
                    &((p_node)->p_contact_uri_list),
                    p_ecode);
            }
        }
    }
    return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dup_address_list
 *
 * DESCRIPTION : This function duplicates an address list structure
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_dup_address_list (
                             icf_glb_pdb_st	*p_glb_pdb,
                             icf_address_list_node_st  *p_src,
                             icf_address_list_node_st  **p_p_dest,
                             icf_error_t     *p_ecode
                             )
{
    icf_return_t ret_val;
    
    while(ICF_NULL != p_src)
    {
        /* Make new node */
        ICF_MEMGET(p_glb_pdb,
            sizeof(icf_address_list_node_st),
            ICF_MEM_COMMON,
            *p_p_dest,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val);
        /* Copy the public user id */
        icf_port_memcpy(&((*p_p_dest)->user_addr),
            &(p_src->user_addr),
                  sizeof(icf_address_st));
        
        /* Increment pointers */
        p_p_dest = &((*p_p_dest)->p_next);
        p_src = p_src->p_next;
    }
    *p_p_dest = ICF_NULL;
    return ICF_SUCCESS;

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_copy_user_list_to_pub_uri_list 
 *
 * DESCRIPTION : This function copies the a user database in a public uri 
 *               list structure. p-associated uri lists are copied based on 
 *               the copy_bitmask.
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

/* */
icf_return_t 
icf_dbm_copy_user_list_to_pub_uri_list (
                             icf_glb_pdb_st	*p_glb_pdb,
                             icf_user_db_node_st  *p_src,
                             icf_pub_uri_list_node_st  **p_p_dest,
                             icf_uint8_t copy_bitmask,
                             icf_error_t *p_ecode
                             )
{
    icf_return_t  ret_val;

    while(ICF_NULL != p_src)
    {
        /* Make new node */
        ICF_MEMGET(p_glb_pdb,
            sizeof(icf_pub_uri_list_node_st),
            ICF_MEM_COMMON,
            *p_p_dest,
            ICF_RET_ON_FAILURE,
            p_ecode,
            ret_val);
        /* Copy the public user id */
        icf_port_memcpy(&((*p_p_dest)->pub_uri),
            &(p_src->public_user),
            sizeof(icf_address_st));
        (*p_p_dest)->p_assoc_uri_list = ICF_NULL;

        if(copy_bitmask & ICF_USER_DB_USE_ASSOCIATED_URIS)
        {
            icf_dbm_dup_address_list (
                p_glb_pdb,
                p_src->p_assoc_uri_list,
                &((*p_p_dest)->p_assoc_uri_list),
                p_ecode
                );
        }
     
        /* Increment pointers */
        p_p_dest = &((*p_p_dest)->p_next);
        p_src = (icf_user_db_node_st  *)p_src->p_next;
    }
    *p_p_dest = ICF_NULL;
   
    return ICF_SUCCESS;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_public_user_count
 *
 * DESCRIPTION : This function returns the public user count in a 
 *               user database
 *              
 * RETURNS: icf_uint8_t
 *
 ************************************************************************/
icf_uint8_t icf_dbm_get_public_user_count(icf_user_db_node_st  *p_user_db)
{
    icf_uint8_t count = 0;
    while(ICF_NULL != p_user_db)
    {
        count++;
        p_user_db=p_user_db->p_next;
    }
    return count;
}

#ifdef IMS_CLIENT
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_next_buffered_reg_ctxt
 *
 * DESCRIPTION : This function gets the next buffered registration context
 *              
 * RETURNS: icf_return_t 
 *
 ************************************************************************/

icf_return_t icf_dbm_get_next_buffered_reg_ctxt(icf_glb_pdb_st *p_glb_pdb,
                                                icf_uint8_t current_buf_id,
                                                icf_rgm_context_st **p_p_buffered_rgm_block,
                                                icf_error_t *p_ecode)
{
    icf_dbm_blk_id_t           rgn_cntxt_blk_next_used_id;
    icf_return_t               ret_val = ICF_FAILURE;
    icf_dbm_data_st            *p_dbm_data = ICF_NULL;
    icf_config_data_st         *p_cfg_data = ICF_NULL;
    icf_rgm_context_st         *p_rgn_blk_addr = ICF_NULL;
    icf_dbm_pool_info_st       *p_rgn_pool_addr = ICF_NULL; 
    icf_uint8_t                target_buf_id=0;
 
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
                (icf_void_t *)&p_cfg_data,
                p_ecode);
       
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
 
    /* 
     * Get the pool address for registration context pool
     */  
    p_rgn_pool_addr = icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_RGN_CTX);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgn_pool_addr))

    /*If next used id is not equal to the pool size ie there are few used nodes*/
    rgn_cntxt_blk_next_used_id = p_dbm_data->rgn_cntxt_blk_next_used_id;
    
    if(current_buf_id == p_cfg_data->max_registration_req)
    {
        target_buf_id = 1;
    }
    else
    {
        target_buf_id = current_buf_id + 1;
    }
    
    /* traverse the used nodes*/ 
    while(p_rgn_pool_addr->pool_size != 
        rgn_cntxt_blk_next_used_id)
    {
        p_rgn_blk_addr= 
            ((icf_rgm_context_st *)p_rgn_pool_addr->p_blk_array) + 
            rgn_cntxt_blk_next_used_id;
        /* Compare Addresses*/
        if(target_buf_id == p_rgn_blk_addr->buffer_id)
        {
            *p_p_buffered_rgm_block = p_rgn_blk_addr;
            p_glb_pdb->p_rgm_ctx = *p_p_buffered_rgm_block;
            p_glb_pdb->line_id = p_glb_pdb->p_rgm_ctx->rgm_list_of_lines[0];
            break;
        }
        rgn_cntxt_blk_next_used_id =
            (((icf_dbm_la_st *)(p_rgn_pool_addr->p_available_blk_lst)) + 
            rgn_cntxt_blk_next_used_id)->next_index;

    }
    ICF_FUNCTION_EXIT(p_glb_pdb)    
    ICF_RESET_MODULE(p_glb_pdb)
        
    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_buffered_block_by_address
 *
 * DESCRIPTION : This function gets a buffered registration context for 
 *                the given uri
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_buffered_block_by_address(icf_glb_pdb_st *p_glb_pdb,
                                                   icf_address_st *p_user_addr,
                                                   icf_rgm_context_st **p_p_buffered_rgm_block,
                                                   icf_error_t *p_ecode)
{
    icf_dbm_blk_id_t           rgn_cntxt_blk_next_used_id;
    icf_return_t               ret_val = ICF_FAILURE;
    icf_dbm_data_st            *p_dbm_data = ICF_NULL;
    icf_config_data_st         *p_cfg_data = ICF_NULL;
    icf_rgm_context_st         *p_rgn_blk_addr = ICF_NULL;
    icf_dbm_pool_info_st       *p_rgn_pool_addr = ICF_NULL; 
 
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)    
    
    *p_p_buffered_rgm_block = ICF_NULL;

    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
                (icf_void_t *)&p_dbm_data,
                p_ecode);
       
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_cfg_data,
                p_ecode);
       
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
 
    /* 
     * Get the pool address for registration context pool
     */  
    p_rgn_pool_addr = icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_RGN_CTX);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgn_pool_addr))

    /*If next used id is not equal to the pool size ie there are few used nodes*/
    rgn_cntxt_blk_next_used_id = p_dbm_data->rgn_cntxt_blk_next_used_id;
    
    
    /* traverse the used nodes*/ 
    while(p_rgn_pool_addr->pool_size != 
        rgn_cntxt_blk_next_used_id)
    {
        p_rgn_blk_addr= 
            ((icf_rgm_context_st *)p_rgn_pool_addr->p_blk_array) + 
            rgn_cntxt_blk_next_used_id;
        /* Compare Addresses*/
        if(ICF_TRUE == 
            icf_dbm_util_cmp_addr(p_glb_pdb,p_user_addr,&(p_rgn_blk_addr->rgm_user_address)))
        {
            *p_p_buffered_rgm_block = p_rgn_blk_addr;
            p_glb_pdb->p_rgm_ctx = *p_p_buffered_rgm_block;
            p_glb_pdb->line_id = p_glb_pdb->p_rgm_ctx->rgm_list_of_lines[0];
            break;
        }
        rgn_cntxt_blk_next_used_id =
            (((icf_dbm_la_st *)(p_rgn_pool_addr->p_available_blk_lst)) + 
            rgn_cntxt_blk_next_used_id)->next_index;

    }
    ICF_FUNCTION_EXIT(p_glb_pdb)    
    ICF_RESET_MODULE(p_glb_pdb)
        
    return ret_val;
}

#endif

/************************************************************************
 *
 * FUNCTION NAME : icf_dbm_copy_public_user_node_to_pub_uri_list 
 *
 * DESCRIPTION   : This function copies a public user node to a public uri 
 *               list structure. p-associated uri lists are copied based on 
 *               the copy_bitmask.
 *              
 * RETURNS       : icf_return_t
 *
 ************************************************************************/


icf_return_t 
icf_dbm_copy_public_user_node_to_pub_uri_list (
                             icf_glb_pdb_st	           *p_glb_pdb,
                             icf_user_db_node_st       *p_src,
                             icf_pub_uri_list_node_st  **p_p_dest,
                             icf_uint8_t               copy_bitmask,
                             icf_error_t               *p_ecode)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    /* Make new node */
    ICF_MEMGET(p_glb_pdb,
        sizeof(icf_pub_uri_list_node_st),
        ICF_MEM_COMMON,
        *p_p_dest,
        ICF_RET_ON_FAILURE,
        p_ecode,
        ret_val);
    
    /* Copy the public user id */
    icf_port_memcpy(&((*p_p_dest)->pub_uri),
        &(p_src->public_user),
        sizeof(icf_address_st));
    
    (*p_p_dest)->p_assoc_uri_list = ICF_NULL;
    
    if(copy_bitmask & ICF_USER_DB_USE_ASSOCIATED_URIS)
    {
        /*If the copy_bitmask is set to ICF_USER_DB_USE_ASSOCIATED_URIS
          also copy the assoc uri list to user db node */
        icf_dbm_dup_address_list (
            p_glb_pdb,
            p_src->p_assoc_uri_list,
            &((*p_p_dest)->p_assoc_uri_list),
            p_ecode
            );
        /*Fix for CSR-1-5556294.If there is no p-assoc-uri list,
         * IPTK shall not send any list to the application.Free the
         * allocated memory.
         */
          if(ICF_NULL == (*p_p_dest)->p_assoc_uri_list)
          {
              ICF_MEMFREE(p_glb_pdb, *p_p_dest,ICF_MEM_COMMON, p_ecode,
                                                  ret_val);
          }
    }/*if(copy_bitmask & ICF_USER_DB_USE_ASSOCIATED_URIS)*/
    
    return ICF_SUCCESS;
}




icf_return_t
icf_dbm_get_line_id_for_user(
    INOUT icf_glb_pdb_st *p_glb_pdb,
    INOUT    icf_address_st *p_user_address,
    OUT   icf_line_id_t  *p_line_id,
    OUT   icf_error_t    *p_ecode)
{
    icf_uint32_t  line_count = 0, count = 0;

    icf_return_t  ret_val = ICF_SUCCESS;
    icf_line_data_st *p_line_blk = ICF_NULL;
    icf_config_data_st           *p_config_data = ICF_NULL;
    icf_address_st               search_address;
    icf_uint8_t                  user_found = ICF_FALSE;


    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    *p_line_id = (icf_line_id_t)ICF_INVALID_LINE_ID;

	/* Retreive the pointer for global CFG data*/
    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))   

    line_count = p_config_data->max_lines;

    /*Copy the user address in search_address*/
    icf_port_memcpy(&search_address,p_user_address,
            (icf_uint32_t const)sizeof(icf_address_st));

	/* traverse all the lines*/
    for (count = 0; count < line_count; count++)
    {
        if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
                    (icf_line_id_t const)(count),
                    &p_line_blk,
                    p_ecode))
        {
            /* Check for next line block. It is possible that there
             * may be holes in line data configuration. So leave
             * that. */
            continue;
        }

		if(ICF_FAILURE == icf_dbm_check_if_user_exits_in_line(
                p_glb_pdb,
                p_user_address,
                p_line_blk,
				(icf_boolean_t)ICF_TRUE,
                p_ecode))
		{
			/* Check next line block where this user address exist
			* in that line. 
			*/
			continue;
		}
		else
		{
			user_found = ICF_TRUE;
			*p_line_id = (icf_line_id_t const)(count);
			break;
		}
      
    } /* End of For loop for max line count */    
    

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return (ret_val);

} /* End of icf_dbm_get_line_id_for_user */

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_fetch_call_ctxt_from_hash_table 
 * DESCRIPTION : This function gets the call context from hash table 
 *               using call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_fetch_call_ctxt_from_hash_table(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint32_t call_id,
	icf_call_ctx_st **p_call_ctxt,
	icf_error_t *p_ecode)
{
	icf_return_t 	ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

    if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		*p_call_ctxt = (icf_call_ctx_st*)icf_port_numeric_hash_fetch(
							ICF_INT_MODULE_CC, p_dbm_data->call_ctxt_hash_table_id,
							call_id, p_ecode);
		if ( ICF_NULL == *p_call_ctxt)
		{
			ret_val = ICF_FAILURE;
		}
	} 
	
	ICF_PRINT(((icf_uint8_t*)"\n{DBM]: <NUM_FETCH>: FETCHED KEY: %d, RESULT: %d\n",call_id,ret_val));	
	
	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_fetch_ssa_ctxt_from_hash_table 
 * DESCRIPTION : This function gets the ssa context from hash table 
 *               using n/w call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_fetch_ssa_ctxt_from_hash_table(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint8_t *nw_call_id,
	icf_void_t **p_ssa_ctxt,
	icf_error_t *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		*p_ssa_ctxt = icf_port_string_hash_fetch(
						ICF_INT_MODULE_SSA, p_dbm_data->ssa_ctxt_hash_table_id,
						nw_call_id,p_ecode);
		if ( ICF_NULL == *p_ssa_ctxt)
		{
			ret_val = ICF_FAILURE;
		}
	}

	ICF_PRINT(((icf_uint8_t*)"\n{DBM]: <STRING_FETCH>: FETCHED KEY: %s, RESULT: %d\n",nw_call_id,ret_val));

	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}

icf_list_st *icf_dbm_fetch_all_ssa_ctxt_from_hash_table(
	icf_glb_pdb_st *p_glb_pdb)
{
    icf_list_st     *p_head = ICF_NULL;
    icf_list_st     *p_temp = ICF_NULL;
    icf_list_st     *p_new = ICF_NULL;
    icf_uint8_t     *p_key = ICF_NULL;
    icf_uint8_t     *p_new_key = ICF_NULL;
    icf_error_t      err;
    icf_void_t      *p_val = ICF_NULL;
    icf_return_t    ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

    if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        &err))
	{
		ret_val = ICF_FAILURE;
	}

    if (ICF_NULL == (p_val = icf_port_get_first_entry_from_string_hash_table(
                                ICF_INT_MODULE_SSA, p_dbm_data->ssa_ctxt_hash_table_id,
                                &p_key)))
    {
        ICF_PRINT(((icf_uint8_t*)"No data found.\n"));
        return ICF_NULL;
    }

    /*
    ICF_MEMGET(p_glb_pdb,
            sizeof(icf_list_st),
            ICF_MEM_COMMON,
            p_head,
            ICF_DONOT_RET_ON_FAILURE,
            &err,
            ret_val)
    */
    p_head = (icf_list_st *)icf_cmn_memget(p_glb_pdb,
                   sizeof(icf_list_st), ICF_MEM_COMMON,
                   &err);
    if(ICF_NULL == p_head)
    {
         return ICF_NULL;
    }

    p_head->p_data = p_val;
    p_head->p_next = ICF_NULL;
    p_temp = p_head;
    for (; ICF_NULL != (p_val = icf_port_get_next_entry_from_string_hash_table(
                    ICF_INT_MODULE_SSA, p_dbm_data->ssa_ctxt_hash_table_id,
                    p_key, &p_new_key));)
    {
        ICF_PRINT(((icf_uint8_t*)"Getting ssa context from hash table\n"))
        /*
        ICF_MEMGET(p_glb_pdb,
                sizeof(icf_list_st),
                ICF_MEM_COMMON,
                p_new,
                ICF_DONOT_RET_ON_FAILURE,
                &err,
                ret_val)
        */
        p_new = (icf_list_st *)icf_cmn_memget(p_glb_pdb, 
                   sizeof(icf_list_st), ICF_MEM_COMMON,
                   &err);
        if(ICF_NULL == p_new)
        {
           break; 
        }

        p_temp->p_next = p_new;
        p_new->p_data = p_val;
        p_new->p_next = ICF_NULL;
        p_temp = p_new;
        p_key = p_new_key;
          
    }

    if(ICF_FAILURE == ret_val)
    {
       while(ICF_NULL != p_head)
       {

          p_temp = p_head->p_next;

		  ICF_MEMFREE(p_glb_pdb,
            p_head,
            ICF_MEM_COMMON,
            &err,
            ret_val)
         
          p_head = p_temp;
       }
       p_head = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return p_head;
}
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_add_call_ctxt 
 *
 * DESCRIPTION : This function add the call context into hash table 
 *               using call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_add_call_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint32_t call_id,
	icf_call_ctx_st *p_call_ctxt,
	icf_error_t *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		ret_val = icf_port_numeric_hash_add(
					ICF_INT_MODULE_CC,p_dbm_data->call_ctxt_hash_table_id,
					call_id,p_call_ctxt,p_ecode);
	}
	ICF_PRINT(((icf_uint8_t*)"\n{DBM]: <NUM_ADD>: ADDED KEY: %d, RESULT: %d\n",call_id,ret_val));		

	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_add_ssa_ctxt 
 *
 * DESCRIPTION : This function adds the ssa context into hash table 
 *               using n/w call_id as key 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_add_ssa_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint8_t *nw_call_id,
	icf_void_t *p_ssa_ctxt,
	icf_error_t *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		ret_val = icf_port_string_hash_add(
					ICF_INT_MODULE_SSA,p_dbm_data->ssa_ctxt_hash_table_id,
					nw_call_id,p_ssa_ctxt,p_ecode);
	}

	
	ICF_PRINT(((icf_uint8_t*)"\n{DBM]: <STRING_ADD>: ADDED KEY: %s, RESULT: %d\n",nw_call_id,ret_val));		
	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}



/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_remove_call_ctxt 
 *
 * DESCRIPTION : This function removes the call context from hash table 
 *               using call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_remove_call_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint32_t call_id,
	icf_call_ctx_st *p_call_ctxt,
	icf_error_t *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		p_call_ctxt = (icf_call_ctx_st*)icf_port_numeric_hash_remove(
					ICF_INT_MODULE_CC,p_dbm_data->call_ctxt_hash_table_id,
					call_id,p_ecode);
		if ( ICF_NULL == p_call_ctxt)
		{
			ret_val = ICF_FAILURE;
		}
	}

	ICF_PRINT(((icf_uint8_t*)"\n{DBM]: <NUM_REMOVE>: REMOVED KEY: %d, RESULT: %d\n",call_id,ret_val));

	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_remove_ssa_ctxt 
 *
 * DESCRIPTION : This function removes the ssa context from hash table 
 *               using n/w call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_remove_ssa_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint8_t *nw_call_id,
	icf_void_t *p_ssa_ctxt,
	icf_error_t *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		p_ssa_ctxt = icf_port_string_hash_remove(
					ICF_INT_MODULE_SSA,p_dbm_data->ssa_ctxt_hash_table_id,
					nw_call_id,p_ecode);
		if ( ICF_NULL == p_ssa_ctxt)
		{
			ret_val = ICF_FAILURE;
		}
	}

	ICF_PRINT(((icf_uint8_t*)"\n{DBM]: <STRING_REMOVE>: REMOVED KEY: %s, RESULT: %d\n",nw_call_id,ret_val));

	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_all_active_ssa_ctxt 
 *
 * DESCRIPTION : This function gets the all ssa contexts from hash table 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_all_active_ssa_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_list_st **p_p_ssa_list,
	icf_error_t *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if ( ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		*p_p_ssa_list = icf_port_get_iterator_from_hash_table(
							ICF_INT_MODULE_SSA,p_dbm_data->ssa_ctxt_hash_table_id,
							p_ecode);
		if ( ICF_NULL == *p_p_ssa_list)
		{
			ret_val = ICF_FAILURE;
		}
	}


	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_free_active_ssa_ctxt_list 
 *
 * DESCRIPTION : This function frees the list created by 
 *				 icf_dbm_get_all_active_ssa_ctxt. this will not free
 *				 p_data in the list. 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_free_active_ssa_ctxt_list(
	icf_glb_pdb_st *p_glb_pdb,
	icf_list_st **p_p_ssa_list,
	icf_error_t *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_list_st *p_tmp_node = ICF_NULL;
	icf_list_st *p_current_node = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)
	
	p_current_node = *p_p_ssa_list;
	
	while(ICF_NULL != p_current_node)
	{
		p_tmp_node = p_current_node->p_next;
		ICF_MEMFREE(p_glb_pdb,
            p_current_node,
            ICF_MEM_COMMON,
            p_ecode,
            ret_val);
		p_current_node = p_tmp_node;
	}
	*p_p_ssa_list = ICF_NULL;

	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_non_wildcarded_uri_for_line_if_pressent
 *
 * DESCRIPTION : This function returns the pointer to the first
 *               non-barred and non wildcarded uri on the line.
                 if ONLY wildcarded user exists on line,then
                 only pointer to it is returned.
 *               Returns failure if no non-barred uri exists on the line
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_non_wildcarded_uri_for_line_if_pressent (
                                                     icf_glb_pdb_st     *p_glb_pdb,
                                                     icf_line_id_t      line_id,
                                                     icf_address_st     **p_p_uri,
                                                     icf_error_t     *p_ecode
                                                     )
{
      icf_return_t           ret_val = ICF_FAILURE;
      icf_line_data_st       *p_line_data = ICF_NULL;
#ifdef IMS_CLIENT
      icf_user_db_node_st    *p_curr_node;
#endif
          /* Get line data */
      if ( ICF_SUCCESS ==
          icf_dbm_fetch_line_cntxt_blk(
          p_glb_pdb, line_id,
          (icf_void_t *)&(p_line_data),
          p_ecode))
      {
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
          for(p_curr_node = p_line_data->p_user_db;
          ICF_NULL != p_curr_node;
          p_curr_node=p_curr_node->p_next)
          {
                          /* send the first assoc uri found */
                          if (ICF_NULL != p_curr_node->p_assoc_uri_list)
              {


                  *p_p_uri = &(p_curr_node->p_assoc_uri_list->user_addr);
                  ret_val = ICF_SUCCESS;
                  break;
              }
          }
ICF_CHECK_IMS_END
#endif
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_FALSE)
                  /* The first uri on the line */
                  if(ICF_NULL != p_line_data->p_user_db)
                  {
                      /* If ONLY wildcarded user is present on line,fill the address of
                         wildcarded user.*/
                      if (((!icf_port_strncmp(p_line_data->p_user_db->public_user.addr_val.str, \
                                  (icf_uint8_t *)ICF_WILDCARD_USER, icf_port_strlen( (icf_uint8_t *)ICF_WILDCARD_USER)))&&
                          (ICF_NULL == p_line_data->p_user_db->p_next)) ||
                          /*If a valid user is present as first user on line,fill the
                            address of tgis particular user.*/
                          (icf_port_strncmp(p_line_data->p_user_db->public_user.addr_val.str, \
                                  (icf_uint8_t *)ICF_WILDCARD_USER, icf_port_strlen( (icf_uint8_t *)ICF_WILDCARD_USER))))
                           { 
                             *p_p_uri = &(p_line_data->p_user_db->public_user);
                              ret_val = ICF_SUCCESS;
                           }
                      /* If wildcarded user is present as a first user on line and a valid
                         user also exists next to it on the same line,then fill address of
                         first valid user.*/
                      else if((!icf_port_strncmp(p_line_data->p_user_db->public_user.addr_val.str, \
                                  (icf_uint8_t *)ICF_WILDCARD_USER, icf_port_strlen( (icf_uint8_t *)ICF_WILDCARD_USER))) &&
                             (ICF_NULL != p_line_data->p_user_db->p_next))
                           {
                             *p_p_uri = &(p_line_data->p_user_db->p_next->public_user);
                              ret_val = ICF_SUCCESS;
                           }

                  }
ICF_CHECK_IMS_END
#endif
      }
      return ret_val;
}
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_check_if_user_exits_on_line
 *
 * DESCRIPTION:   The function checks for the line to find if a user exists
 *                on the line .
 *
 * RETURNS:       Success if the user exist on the line else returns failure.
 *
 ************************************************************************/
icf_return_t
icf_dbm_check_if_user_exits_on_line(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        INOUT    icf_address_st   *p_user_addr,
        IN    icf_line_data_st *p_line_blk,
        INOUT    icf_error_t      *p_ecode)
{
    icf_return_t               ret_val = ICF_FAILURE;
    /*Klocwork warning fix , variable search_address initialized*/
    icf_address_st             search_address = {{0,"\0"},0};
    icf_uint8_t                user_found = ICF_FALSE;
    icf_boolean_t              user_name_e164 = ICF_FALSE;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Copy the user address in search_address*/
    icf_port_memcpy(&search_address,p_user_addr,
            (icf_uint32_t const)sizeof(icf_address_st));


    if(ICF_FAILURE != icf_dbm_check_if_user_exits_in_line(
                        p_glb_pdb,
                        &search_address,
                        p_line_blk,
                        (icf_boolean_t)ICF_FALSE,
                        p_ecode))

      {
        user_found = ICF_TRUE;
        ret_val = ICF_SUCCESS;
      }
    if((ICF_FALSE == user_found) &&
                (ICF_ADDRESS_PLAN_SIP_URI == search_address.addr_type))
        {
            /*If it is a SIP URI extract the username and check it for being a
             * valid E164 address.*/
            user_name_e164 = icf_dbm_ext_usrnam_n_val_e164(
                    p_glb_pdb,
                    p_user_addr,
                    &search_address);

        }
          if(ICF_TRUE == user_name_e164)
                {
                   if(ICF_FAILURE != icf_dbm_check_if_user_exits_in_line(
                        p_glb_pdb,
                        &search_address,
                        p_line_blk,
                        (icf_boolean_t)ICF_FALSE,
                        p_ecode))
                     {
                       user_found = ICF_TRUE;
                       ret_val = ICF_SUCCESS;
                     }
                        
                }
    if((ICF_FALSE == user_found) &&
                (ICF_ADDRESS_PLAN_SIP_URI == p_user_addr->addr_type))
                {               
                /*  if(ICF_SUCCESS == icf_dbm_chk_if_dom_is_local_ip(p_glb_pdb,p_user_addr))
                    { */
                       if(ICF_FAILURE != icf_dbm_check_if_user_exits_in_line(
                            p_glb_pdb,
                            p_user_addr,
                            p_line_blk,
                            (icf_boolean_t)ICF_TRUE,
                            p_ecode))
                         {
                           user_found = ICF_TRUE;
                           ret_val = ICF_SUCCESS;
                         }
                /*  } */
                }


    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}


#ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING
/******************************************************************************
 * FUNCTION:           	icf_dbm_compare_alternate_addr
 *
 * DESCRIPTION:        	This is a utility function to derive an alternate addr
 *						format from address1 carrying phone-context parameter 
 *						and compare different combinations with address2
 *
 * RETURNS:             Returns pointer indicating match status:
 *                           ICF_TRUE if addresses match else returns ICF_FALSE
 *
 *****************************************************************************/
icf_return_t icf_dbm_compare_alternate_addr(
        	INOUT 	icf_glb_pdb_st 		*p_glb_pdb,
        	IN 		icf_address_st 		*p_address_1,
        	IN 		icf_address_st 		*p_address_2,
			OUT		icf_boolean_t		*p_match_found)
{
	icf_return_t						ret_val = ICF_SUCCESS;

    icf_address_st						user_info_full_1 = {{0,"\0"},0};
    icf_address_st						user_info_full_2 = {{0,"\0"},0};
    icf_address_st						user_info_full_derived_1 = {{0,"\0"},0};


	icf_uint8_t							*p_user_num_1 = ICF_NULL;
	icf_uint8_t							*p_dial_string_1 = ICF_NULL;
	icf_uint8_t             			temp_str[ICF_MAX_STR_LEN] = "\0";
	icf_uint8_t             			user_derived_1[ICF_MAX_STR_LEN] = "\0";

	ICF_FUNCTION_ENTER(p_glb_pdb)

	ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_match_found))

	icf_cmn_init_address(&user_info_full_1);
	icf_cmn_init_address(&user_info_full_2);
	icf_cmn_init_address(&user_info_full_derived_1);

	/* ---------------------------------------------------------------------- */
    /* In case the req-uri is of the form
     *      sip:3659220;phone-context=+1408@20.20.20.20;user=phone, or
     *      sip:+14083659220@20.20.20.20;user=phone
     * user search in local database should be done for both
     * (a) '3659220' - user part of address, and
     * (b) '+14083659220' - phone-context prefixed to user part
     * (c) '14083659220' - phone-context prefixed to user part without '+' sign
     */
	/*ICF_PRINT(((icf_uint8_t*)"\n[DBM]: icf_dbm_compare_alternate_addr will compare:"));
    ICF_PRINT(((icf_uint8_t*)"\n\tAddress 1: %s", p_address_1->addr_val.str));
    ICF_PRINT(((icf_uint8_t*)"\n\tAddress 2: %s", p_address_2->addr_val.str));
	ICF_PRINT(((icf_uint8_t*)"\n\tAlternate addresses may be derived from Address 1\n"));*/


	/* ------------------- STEP :: 1 :: VALIDATION -------------------------- */
	/* Check if the address to be parsed is a local IP address */
	/* --------------------------------------------------------------------- */

	if((ICF_ADDRESS_PLAN_E164 != p_address_1->addr_type) &&
       (ICF_ADDRESS_PLAN_TEL_URI != p_address_1->addr_type))
	{
    	if ((ICF_NULL != icf_port_strchr(p_address_1->addr_val.str, '@')) &&
	   		(ICF_FAILURE == icf_dbm_chk_if_dom_is_local_ip(p_glb_pdb, p_address_1)))
		{
			/*ICF_PRINT(((icf_uint8_t*)"\n[DBM]: Address 1 is not local address\n"));*/
			ret_val = ICF_FAILURE;
		}
	}
	
	if (ICF_FAILURE == ret_val)
	{
		/* do nothing */
	}
	/* ------------------- STEP :: 2 :: EXTRACTION -------------------------- */
    /* Get the userinfo (i.e. abc in abc@ip) */
	/* 		3659220;phone-context=+1408@47.100.105.142
	 *			gives
	 * 		3659220;phone-context=+1408
	 */
	/* --------------------------------------------------------------------- */
	else if (ICF_FAILURE == icf_dbm_ext_usrnam(
							p_glb_pdb,
							p_address_1,
							&user_info_full_1))
	{
		ret_val = ICF_FAILURE;
	}
	else if (ICF_FAILURE == icf_dbm_ext_usrnam(
							p_glb_pdb,
							p_address_2,
							&user_info_full_2))
	{
		ret_val = ICF_FAILURE;
	}

	/* ------------------- STEP :: 3 :: COMPARISON -------------------------- */

	/* --------------------------------------------------------------------- */
	/* Address-1 without phone-context parameter and without '+' sign */
	/* --------------------------------------------------------------------- */
    /* Fix for SPR: 19886 (Merged SPR:19821)*/
    /*Semicolon is added in string to identify phone-context-tag*/
	else if ((ICF_NULL == icf_port_strstr(
                                 (const icf_int8_t *)user_info_full_1.addr_val.str,
                                 (const icf_int8_t *)";phone-context=")) &&
			 ('+' != user_info_full_1.addr_val.str[0]))
	{
		/* Address-2 without '+' sign */
		if ('+' != user_info_full_2.addr_val.str[0])
		{
			if (ICF_NULL == icf_port_strcmp(
								user_info_full_1.addr_val.str,
								user_info_full_2.addr_val.str))
			{
				*p_match_found = ICF_TRUE;
			}
		}
		/* Address-2 with '+' sign */
		else if ('+' == user_info_full_2.addr_val.str[0])
		{
			if (ICF_NULL == icf_port_strcmp(
								user_info_full_1.addr_val.str,
								&user_info_full_2.addr_val.str[1]))
			{
				*p_match_found = ICF_TRUE;
			}
		}
	}

	/* --------------------------------------------------------------------- */
	/* Address-1 without phone-context parameter and with '+' sign */
	/* --------------------------------------------------------------------- */
    /* Fix for SPR: 19886 (Merged SPR:19821)*/
    /*Semicolon is added in string to identify phone-context-tag*/
	else if ((ICF_NULL == icf_port_strstr(
                                 (const icf_int8_t *)user_info_full_1.addr_val.str,
                                 (const icf_int8_t *)";phone-context=")) &&
			 ('+' == user_info_full_1.addr_val.str[0]))
	{
		/* Address-2 without '+' sign */
		if ('+' != user_info_full_2.addr_val.str[0])
		{
			if (ICF_NULL == icf_port_strcmp(
								&user_info_full_1.addr_val.str[1],
								user_info_full_2.addr_val.str))
			{
				*p_match_found = ICF_TRUE;
			}
		}
		/* Address-2 with '+' sign */
		else if ('+' == user_info_full_2.addr_val.str[0])
		{
			if (ICF_NULL == icf_port_strcmp(
								user_info_full_1.addr_val.str,
								user_info_full_2.addr_val.str))
			{
				*p_match_found = ICF_TRUE;
			}
		}
	}
	/* --------------------------------------------------------------------- */
	/* Address-1 with phone-context parameter */
	/* --------------------------------------------------------------------- */
    /* Fix for SPR: 19886 (Merged SPR:19821)*/
    /*Semicolon is added in string to identify phone-context-tag*/
	else if (ICF_NULL != icf_port_strstr(
                                (const icf_int8_t *) user_info_full_1.addr_val.str,
                                 (const icf_int8_t *)";phone-context="))
	{
        /* Get p_user_num_1 = '3659220' */
        icf_port_strcpy(temp_str, user_info_full_1.addr_val.str);
        p_user_num_1 = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)temp_str, ";");


        /* Klocwork warning removal.*/
        if (ICF_NULL != p_user_num_1)
        {
             /* Get p_dial_string_1 = '+1408' */
             p_dial_string_1 = user_info_full_1.addr_val.str +
						  	icf_port_strlen(p_user_num_1) +
						  	icf_port_strlen((icf_uint8_t *)";") +
						  	icf_port_strlen((icf_uint8_t *)"phone-context=");

		/* Developer debug trace - commented for the time being */
		/*ICF_PRINT(((icf_uint8_t*)"\n[DBM]: user_info_full_1 = %s", \
							user_info_full_1.addr_val.str));
		ICF_PRINT(((icf_uint8_t*)"\n[DBM]: user_info_full_2 = %s", \
							user_info_full_2.addr_val.str));
		ICF_PRINT(((icf_uint8_t*)"\n[DBM]: p_user_num_1 = %s", p_user_num_1));
		ICF_PRINT(((icf_uint8_t*)"\n[DBM]: p_dial_string_1 = %s\n", p_dial_string_1));*/



		/* ----------------------------------------------------------------- */
		/* Compare number but ignore the phone-context */
		/* 		3659220;phone-context=+1408@47.100.105.142
		 *			matches
		 * 		3659220
		 */
    
		if (ICF_NULL == icf_port_strcmp(
							p_user_num_1,
							user_info_full_2.addr_val.str))
		{
			*p_match_found = ICF_TRUE;
		}
		/* ----------------------------------------------------------------- */
		/* Compare number prefixed with phone-context with or without '+' */
		/* 		3659220;phone-context=+1408@47.100.105.142
		 *			matches
		 * 		+14083659220
		 *
		 * 		3659220;phone-context=+1408@47.100.105.142
		 *			matches
		 * 		14083659220
		 *
		 * 		3659220;phone-context=1408@47.100.105.142
		 *			matches
		 * 		+14083659220
		 *
		 * 		3659220;phone-context=1408@47.100.105.142
		 *			matches
		 * 		14083659220
		 */
		else if ((ICF_TRUE == icf_port_isnumeric(p_dial_string_1)) ||
				 (('+' == *p_dial_string_1) &&
				  (ICF_TRUE == icf_port_isnumeric(p_dial_string_1 + 1))))
		{
			/* Get user_derived_1 = '+14083659220' or '14083659220' */
			icf_port_strcpy(user_derived_1, p_dial_string_1);
			icf_port_strcat(user_derived_1, p_user_num_1);

			user_info_full_derived_1.addr_type = 
					user_info_full_1.addr_type;
			icf_port_strcpy(
					user_info_full_derived_1.addr_val.str,
					user_derived_1);
			user_info_full_derived_1.addr_val.str_len =(icf_uint16_t) 
					icf_port_strlen(user_info_full_derived_1.addr_val.str);

			/* Developer debug trace - commented for the time being */
			/*ICF_PRINT(((icf_uint8_t*)"\n[DBM]: user_info_full_derived_1 = %s", \
					user_info_full_derived_1.addr_val.str));*/

			*p_match_found = icf_dbm_cmp_addr_ignr_pre_plus(
									p_glb_pdb,	
									&user_info_full_derived_1,
									&user_info_full_2);
		}
           }
           else
           {
               ret_val = ICF_FAILURE;
           } 
	} /* End else (STEP 3) */

/*
	if (ICF_TRUE == *p_match_found)
	{
		ICF_PRINT(((icf_uint8_t*)"\n[DBM]: icf_dbm_compare_alternate_addr - Match found\n"));
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"\n[DBM]: icf_dbm_compare_alternate_addr - No match\n"));
	}
*/

	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;

} /* End icf_dbm_compare_alternate_addr */
#endif /* #ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING */


/******************************************************************************
 * FUNCTION:           	icf_dbm_delete_uri_from_uri_list 
 *
 * DESCRIPTION:        	This is a utility function to deletes a particular URI
 *                      from  URI list based on search bitmask ie either
 *                      contact uri list or assoc uri list. 
 *
 * RETURNS:             ICF_SUCCESS if uri is deleted from user db node else
 *                      returns ICF_FAILURE
 *
 *****************************************************************************/

icf_return_t icf_dbm_delete_uri_from_uri_list (
    icf_glb_pdb_st	  *p_glb_pdb,
    icf_address_st 	  *p_public_uri,
    icf_line_id_t	  line_id,
    icf_address_st    *p_uri_to_delete,
    icf_uint8_t       search_bitmask,
    icf_error_t 	  *p_ecode)
{
    icf_return_t			 ret_val = ICF_SUCCESS;
    icf_line_data_st         *p_line_blk = ICF_NULL;
    icf_user_db_node_st	     *p_node=ICF_NULL;
   
    ICF_FUNCTION_ENTER(p_glb_pdb) 

    /*Get the line context block corresponding to the line ID*/
    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
                              line_id,
                              &p_line_blk,
                              p_ecode))
    {
        ret_val = ICF_FAILURE;
            
    }
    else
    {
        /*Searches the user database for the given line_id and
         * returns the public uri list node pointer of the node where the
         * address given in p_public_uri is first found in either the
         * p-associated uri list or in the contact list */
        if(ICF_FAILURE == icf_dbm_get_public_uri_node_from_uri_for_line (
	         p_glb_pdb, line_id, p_public_uri,
      	     ICF_USER_DB_USE_ASSOCIATED_URIS | 
      	     ICF_USER_DB_USE_CONTACT_URIS,
             &p_node, p_ecode))
        {
            ret_val = ICF_FAILURE;
        }    
        else
        {
            if (0 != (search_bitmask & ICF_USER_DB_USE_ASSOCIATED_URIS))
            {    
                /*Delete the contact uri from the associated uri list
                 * coressponding to the node found above */
                if (ICF_FAILURE == icf_dbm_clear_uri_for_user_node (
                      p_glb_pdb, p_node, p_uri_to_delete,
                      ICF_USER_DB_USE_ASSOCIATED_URIS,
                      p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }    
            }
            else if(0 !=(search_bitmask & ICF_USER_DB_USE_CONTACT_URIS))
            {
                /*Delete the contact uri from the associated uri list
                 * coressponding to the node found above */
                if (ICF_FAILURE == icf_dbm_clear_uri_for_user_node (
                      p_glb_pdb, p_node, p_uri_to_delete,
                      ICF_USER_DB_USE_CONTACT_URIS,
                      p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }    

            }    
 	    }
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}


/******************************************************************************
 * FUNCTION:           	icf_dbm_clear_uri_for_user_node 
 *
 * DESCRIPTION:        	This is a utility function to deletes a particular
 *                      node from the  uri list based on the value of delete
 *                      bitmask and p_uri to be deleted  
 *
 * RETURNS:             ICF_SUCCESS if uri is deleted from the assoc uri list
 *                      else returns ICF_FAILURE
 *
 *****************************************************************************/

icf_return_t icf_dbm_clear_uri_for_user_node (
    icf_glb_pdb_st	       *p_glb_pdb,
    icf_user_db_node_st    *p_node,
    icf_address_st         *p_uri,
	icf_uint8_t	           delete_bitmask,
    icf_error_t 	       *p_ecode)
{
    icf_return_t		      ret_val = ICF_FAILURE,mem_ret_val = ICF_SUCCESS;
    icf_address_list_node_st  **p_p_node = ICF_NULL;
    icf_address_list_node_st  *p_next_node = ICF_NULL,
                              *p_cur_node = ICF_NULL,
                              *p_previous_node = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*If the delete bitmask is set to ICF_USER_DB_USE_ASSOCIATED_URIS
     *and p_assoc_uri_list is NULL set ret_val to failure */
    if ((ICF_USER_DB_USE_ASSOCIATED_URIS & delete_bitmask) && 
            (ICF_NULL != p_node->p_assoc_uri_list))
    {
        /*Get the starting node(header) of assoc uri list */
        p_p_node = &(p_node->p_assoc_uri_list); 

    }  
    /*If the delete bitmask is set to ICF_USER_DB_USE_CONTACT_URIS
     *and p_contact_uri_list is NULL set ret_valto failure */
    else if ((ICF_USER_DB_USE_CONTACT_URIS & delete_bitmask) && 
            (ICF_NULL != p_node->p_contact_uri_list))
    {
        /*Get the starting node(header) of contact uri list */
        p_p_node = &(p_node->p_contact_uri_list);

    }   
    
    if (ICF_NULL != p_p_node)
    {
        for(p_cur_node = *p_p_node;ICF_NULL !=
            p_cur_node;p_previous_node = p_cur_node,p_cur_node = 
            p_next_node)
        {
           /* Check the uri to be deleted against the user address present
              in current node*/
           
           /*Function call icf_dbm_util_cmp_addr is replaced by 
            *icf_dbm_cmp_addr_in_aor.Here we are doing the strict comparsion
            *for the two addresses of type icf_address_st.For example if the
            *two address structure are of type User1@172.16.105.152:33287 and
            *User1@172.16.105.152:33287 then only this function return
            *SUCCESS.For the case when two address are of type User1@172.16.
            *105.152:33287 and User1@172.16.105.152 this function will return
            *failure and no deletion from contact uri list or assoc uri list
            *is done.*/ 
            
           if(ICF_TRUE ==
              icf_dbm_cmp_addr_in_aor(p_glb_pdb,
                                    &(p_cur_node->user_addr),
                                    p_uri))
           {             
               /* If the user address is present at the first node(if the
                * headd node and current node are same) , delete
                * the node and reassign the Header node of link list*/
               if (*p_p_node == p_cur_node)
               {
                   *p_p_node = ICF_NULL;
                   ICF_MEMFREE(p_glb_pdb,
                                    p_cur_node,
                                    ICF_MEM_COMMON,
                                    p_ecode,
                                    mem_ret_val);
                   /*Do only if the MEMFREE operation is successfully 
                     done*/
                   if(ICF_SUCCESS == mem_ret_val)
                   {    
                       ret_val = ICF_SUCCESS;
                   }    
                   
                   break;
                }
                /* If the user address is present at the last node , delete
                 * the node and reassign the last node of link list*/
               if (ICF_NULL == p_cur_node->p_next)
               {
                   
                   ICF_MEMFREE(p_glb_pdb,
                                   p_cur_node,
                                   ICF_MEM_COMMON,
                                   p_ecode,
                                   mem_ret_val);
                   /*Do only if the MEMFREE operation is successfully 
                     done*/
                   if(ICF_SUCCESS == mem_ret_val)
                   {    
                       ret_val = ICF_SUCCESS;
                       p_previous_node->p_next = ICF_NULL;
                   }    
                   
                   break;
               }
               else
               {
                   
                   /*Klocwork warning removal :reassign the p_next element of the 
                    * prevoius node before freeing the current node */
                   p_previous_node->p_next = p_cur_node->p_next;
                   
                   /* If the user address is present in between  , delete
                    * the node and reassign the p_next element of the 
                    * prevoius node*/
                   
                   ICF_MEMFREE(p_glb_pdb,
                                        p_cur_node,
                                        ICF_MEM_COMMON,
                                        p_ecode,
                                        mem_ret_val);
                   /*Do only if the MEMFREE operation is successfully 
                     done*/
                   if(ICF_SUCCESS == mem_ret_val)
                   {    
                       ret_val = ICF_SUCCESS;
                   }    
                   
                   break;
               }   
           }/*if(ICF_TRUE == icf_dbm_cmp_addr_in_aor...*/
           
           p_next_node = p_cur_node->p_next;
        }/*for(p_cur_node = *p_p_node;ICF_NULL....*/

    }/*if (ICF_FAILURE != ret_val)*/   
        
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}    

/************************************************************************
 * FUNCTION:    icf_dbm_get_default_uri_for_user_on_line   
 * 
 * DESCRIPTION: This function will return ICF_SUCCESS, if it on a
 *              given line for the particular user assoc uri or 
 *              public uri is found.If neither assoc uri nor
 *              public uri is present it will return ICF_FAILURE. In case of 
 *              success, p_uri pointer will be updated with the fetched assoc
 *              uri or public uri.
 * 
 ************************************************************************/
icf_return_t icf_dbm_get_default_uri_for_user_on_line(                                       
                     icf_glb_pdb_st	*p_glb_pdb,                                          
                     icf_line_id_t	line_id,
                     icf_address_st     rgm_user_address, 
                     icf_address_st	*p_uri,                                          
                     icf_error_t        *p_ecode)
{
   /* This function will first get user_db_node on this line
    * user address will match to the rgm_user_address, only 
    * public users will be searched, this search will be 
    * completed by calling
    * icf_dbm_get_public_uri_node_from_uri_for_line().
    * After this we will have user db node, it?™s first Assoc   
    * URI will give us the default public uri for 
    * Subscription.If assoc uri is not present public uri is returned as
    * default public uri for Subscription*/  

   icf_return_t         ret_val = ICF_FAILURE;   
   icf_user_db_node_st	*p_node = ICF_NULL;
   icf_uint8_t          search_bitmask = 0;

   ICF_FUNCTION_ENTER(p_glb_pdb)

   search_bitmask |= ICF_USER_DB_USE_PUBLIC_URIS;

   if(ICF_FAILURE != icf_dbm_get_public_uri_node_from_uri_for_line(
                                        p_glb_pdb,line_id,&rgm_user_address,
                                        search_bitmask,&p_node,p_ecode))
   {
      if (ICF_NULL != p_node)
      {    
          /*Copy the first assoc uri as default uri if present*/
          if (ICF_NULL != p_node->p_assoc_uri_list)
          {
              icf_port_memcpy(p_uri,
			     &(p_node->p_assoc_uri_list->user_addr),
			     sizeof(icf_address_st));

              ret_val = ICF_SUCCESS; 
          }
          /*Copy the public uri as default uri */
          else
          {
              icf_port_memcpy(p_uri,
			     &(p_node->public_user),
			     sizeof(icf_address_st));
              
              ret_val = ICF_SUCCESS;
          }    
      }    

   }
   ICF_FUNCTION_EXIT(p_glb_pdb)
   return ret_val;
}

/*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_fetch_ssa_ctxt_from_via_mapped_hash_table 
 * DESCRIPTION : This function gets the ssa context from hash table 
 *               using n/w via 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_fetch_ssa_ctxt_from_via_mapped_hash_table(
	INOUT   icf_glb_pdb_st *p_glb_pdb,
	IN      icf_uint8_t    *p_nw_via,
	INOUT   icf_void_t     **p_p_ssa_ctxt,
	INOUT   icf_error_t    *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if (ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		*p_p_ssa_ctxt = icf_port_string_hash_fetch(
						ICF_INT_MODULE_SSA,
                        p_dbm_data->ssa_ctxt_via_based_hash_table_id,
						p_nw_via,p_ecode);
		if ( ICF_NULL == *p_p_ssa_ctxt)
		{
			ret_val = ICF_FAILURE;
		}
	}

	ICF_PRINT(((icf_uint8_t *)"\n{DBM]: <STRING_FETCH>: FETCHED KEY: %s, RESULT: %d\n",p_nw_via,ret_val));

	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}
/*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_add_ssa_ctxt_mapped_with_via 
 *
 * DESCRIPTION : This function adds the ssa context into hash table 
 *               using n/w via as key 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_add_ssa_ctxt_mapped_with_via(
	INOUT    icf_glb_pdb_st    *p_glb_pdb,
	IN       icf_uint8_t       *p_nw_via,
	INOUT    icf_void_t        *p_ssa_ctxt,
	INOUT    icf_error_t       *p_ecode)
{
	icf_return_t ret_val = ICF_SUCCESS;
	icf_dbm_data_st	*p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if (ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		ret_val = icf_port_string_hash_add(
					ICF_INT_MODULE_SSA,
                    p_dbm_data->ssa_ctxt_via_based_hash_table_id,
					p_nw_via,p_ssa_ctxt,p_ecode);
	}
	
	ICF_PRINT(((icf_uint8_t *)"\n{DBM]: <STRING_ADD>: ADDED KEY: %s, RESULT: %d\n",p_nw_via,ret_val));		
	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}
/*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_remove_via_mapped_ssa_ctxt 
 *
 * DESCRIPTION : This function removes the ssa context from hash table 
 *               using n/w via 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_remove_via_mapped_ssa_ctxt(
	            INOUT   icf_glb_pdb_st *p_glb_pdb,
	            INOUT   icf_uint8_t    *p_nw_via,
	            INOUT   icf_void_t     *p_ssa_ctxt,
	            INOUT   icf_error_t    *p_ecode)
{
	icf_return_t       ret_val = ICF_SUCCESS;
	icf_dbm_data_st	   *p_dbm_data = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	if (ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode))
	{
		ret_val = ICF_FAILURE;
	}
	else 
	{
		p_ssa_ctxt = icf_port_string_hash_remove(
					ICF_INT_MODULE_SSA,
                    p_dbm_data->ssa_ctxt_via_based_hash_table_id,
					p_nw_via,p_ecode);

		if (ICF_NULL == p_ssa_ctxt)
		{
			ret_val = ICF_FAILURE;
		}
	}

	ICF_PRINT(((icf_uint8_t *)"\n{DBM]: <STRING_REMOVE>: REMOVED KEY: %s, RESULT: %d\n",p_nw_via,ret_val));

	ICF_FUNCTION_EXIT(p_glb_pdb)

	return ret_val;
}

