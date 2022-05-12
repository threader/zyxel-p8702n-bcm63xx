/*------------------------------------------------------------------------------
 *     
 * File name        : icf_dbm_types.h
 *
 * Purpose          : This file contains the datatypes commonly used by DBM
 *
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  07 DEC 2004   Sumit Gupta                File created and initial version
 *  15 DEC 2004   Sumit Gupta                Added structure icf_dbm_data_st 
 *                                           and removed structure 
 *                                           icf_stats_data_st
 *
 *	08-Mar-2006     Aman Aggarwal            ICF merging with ICF2.0
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_DBM_TYPES_H__ 
#define __ICF_DBM_TYPES_H__ 


#include"icf_common_types.h"
#include"icf_port_types.h"


/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

/************************************************************************
 *
 * TYPEDEF NAME: icf_blk_id_t
 *
 * DESCRIPTION:  Would represent a block in DBM pool
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_blk_id_t;

/************************************************************************
 *
 * TYPEDEF NAME: icf_dbm_pool_size_t
 *
 * DESCRIPTION:  Would represent a pool size
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_pool_size_t;

/************************************************************************
 *
 * TYPEDEF NAME: icf_dbm_blk_size_t
 *
 * DESCRIPTION:  Would represent a block in DBM pool
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_blk_size_t;

/************************************************************************
 *
 * TYPEDEF NAME: icf_call_cntxt_blk_id_t
 *
 * DESCRIPTION:  Would represent a call context block
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_call_cntxt_blk_id_t;

/************************************************************************
 *
 * TYPEDEF NAME: icf_sic_cntxt_blk_id_t
 *
 * DESCRIPTION:  Would represent a sic context block
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_sic_cntxt_blk_id_t;

/************************************************************************
 *
 * TYPEDEF NAME: icf_dbm_srvc_cntxt_blk_id_t
 *
 * DESCRIPTION:  Would represent a service context block
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_srvc_cntxt_blk_id_t;

/************************************************************************
 *
 * TYPEDEF NAME: icf_dbm_srvc_cntxt_lgc_blk_id_t
 *
 * DESCRIPTION:  Would represent a service context logic block
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_srvc_cntxt_lgc_blk_id_t;

/************************************************************************
 *
 * TYPEDEF NAME: icf_dbm_rgn_cntxt_blk_id_t
 *
 * DESCRIPTION:  Would represent a registration context block
 *   
 ************************************************************************/
typedef icf_uint16_t icf_dbm_rgn_cntxt_blk_id_t;


/************************************************************************
 *
 * STRUCTURE NAME: icf_dbm_la_st
 *
 * DESCRIPTION:  This structure is used to implement a linked array
 *   
 ************************************************************************/
typedef struct
{
    
    /* Stores the following index in the linked array */
    icf_uint16_t next_index;
    
}icf_dbm_la_st;

/************************************************************************
 *
 * STRUCTURE NAME: icf_call_cntxt_la_st
 *
 * DESCRIPTION:  This structure is used to implement a free call context blocks 
 *               linked array
 *   
 ************************************************************************/
typedef icf_dbm_la_st icf_dbm_call_cntxt_la_st;

/************************************************************************
 *
 * STRUCTURE NAME: icf_sic_cntxt_la_st
 *
 * DESCRIPTION:  This structure is used to implement a free sic context blocks 
 *               linked array
 *   
 ************************************************************************/
typedef icf_dbm_la_st icf_dbm_sic_cntxt_la_st;

/************************************************************************
 *
 * STRUCTURE NAME: icf_srvc_cntxt_la_st
 *
 * DESCRIPTION:  This structure is used to implement a free service context 
 *               blocks linked array
 *   
 ************************************************************************/
typedef icf_dbm_la_st icf_dbm_srvc_cntxt_la_st;

/************************************************************************
 *
 * STRUCTURE NAME: icf_srvc_cntxt_lgc_la_st
 *
 * DESCRIPTION:  This structure is used to implement a free service context 
 *               logic blocks linked array
 *   
 ************************************************************************/
typedef icf_dbm_la_st icf_dbm_srvc_cntxt_lgc_la_st;

/************************************************************************
 *
 * STRUCTURE NAME: icf_rgn_cntxt_la_st
 *
 * DESCRIPTION:  This structure is used to implement a free registration context *               blocks linked array
 *   
 ************************************************************************/
typedef icf_dbm_la_st icf_dbm_rgn_cntxt_la_st;


/************************************************************************
 *
 * STRUCTURE NAME: icf_dbm_pool_info_st
 *
 * DESCRIPTION:  The data type would hold information corresponding to a pool 
 *               in DBM data
 *  
 ************************************************************************/
typedef struct
{
    /* This will hold an array pool of structures */
   icf_void_t *p_blk_array;

   /* Maintains list of available blocks */
   icf_dbm_la_st *p_available_blk_lst;
  
   /* Stores the index of next free call context blk */
   icf_dbm_blk_id_t next_free_id; 

   icf_dbm_pool_size_t  pool_size;

   icf_dbm_blk_size_t   block_size;

}icf_dbm_pool_info_st;


/************************************************************************
 *
 * STRUCTURE NAME: icf_dbm_data_st
 *
 * DESCRIPTION:  The structure the data specific to DBM 
 *  
 ************************************************************************/
typedef struct 
{
	/* Contain the infomation regarding various DBM pools*/ 
	icf_dbm_pool_info_st pool[ICF_MAX_BLK_TYPE];
	
	/* Stores the index of next used registration context blk */
	icf_dbm_blk_id_t rgn_cntxt_blk_next_used_id;  
	
	/* 
	* This would tell the last context identifier returned. This would be used to
	* idnetify what should be the context identifier to be returned for 
	* subequesnt calls
	*/
	icf_call_id_t   last_opt_ctx_id_ret;
	
	/* This will hold an array pool of structures that will hold sic context 
    * data for different calls */
	icf_void_t *p_sic_cntxt_blk;
	
	/* Maintains list of available sic context blocks */
	icf_dbm_call_cntxt_la_st *p_available_sic_cntxt_lst;
	
	/* Stores the index of next free call context blk */
	icf_dbm_sic_cntxt_blk_id_t sic_cntxt_blk_next_free_id; 
	
	ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
	/* This will hold an array pool of structures that will hold network 
    * control context data. Size of this array depends upon the max_network
    * parameter in ippstl_config_data_st */
	icf_void_t *p_netact_ctxt_blk ;

	/* Identifier of the retry timer which is running on network_activation */
    icf_timer_id_t timer_id;

	/* Pointer to the retry timer which is running on network_activation */
    icf_timer_data_st *p_timer_data;
#endif
	
	/* This is a scalable array, allocated at the init-time, when CFG module 
    * gets mesg_id = ICF_CFG_INTERNAL_INIT. The size of this array depends
	upon the max_applications parameter in the ippstl_config_data_st. */ 
	icf_void_t     *p_app_conf;
ICF_CRM_END

	/*Call context hash table identifier. This is used to add/fetch/remove
	 * elements from the hash table
	 */
	icf_hash_table_id_t call_ctxt_hash_table_id;

	/*ssa context hash table identifier. This is used to add/fetch/remove
	 * elements from the hash table
	 */
	icf_hash_table_id_t ssa_ctxt_hash_table_id;
  

}icf_dbm_data_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_DBM_TYPES_H__ */



