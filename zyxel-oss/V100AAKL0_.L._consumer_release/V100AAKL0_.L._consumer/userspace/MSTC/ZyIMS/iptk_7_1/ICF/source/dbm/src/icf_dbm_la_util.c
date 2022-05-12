/*************************************************************************
 *
 *     FILENAME     :    icf_dbm_la_util.c
 *
 *     DESCRIPTION  :  This file defines the database organization for linking
 *                     the control block structures ."la" here stands for linked *                     array i.e. the structure used for linking the array 
 *                     elements.
 *                     
 *     Revision History    :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *       14-Dec-2004   Sumit Gupta                         Initial Version    
 *		 08-Mar-2006   Aman Aggarwal					   ICF merging with ICF2.0		
 *	
 *     Copyright 2006, Aricent.
 *
 *************************************************************************/


#include "icf_common_types.h"
#include "icf_dbm_types.h"
#include "icf_dbm_defs.h"
#include "icf_dbm_prototypes.h"
#include "icf_port_defs.h"
#include "icf_port_prototypes.h"




/*************************************************************************
 *
 * FUNCTION NAME    :     icf_dbm_la_init
 *
 * DESCRIPTION      :    This function initializes the linked array pool.
 *
 * RETURNS          :    ICF_SUCCESS    
 *
*************************************************************************/

icf_return_t
icf_dbm_la_init(
        IN       icf_uint16_t        la_pool_size,
        INOUT    icf_dbm_la_st       *p_la_pool,
        OUT      icf_uint16_t        *p_next_free_index,
        OUT      icf_error_t         *p_ecode)
{
    icf_uint16_t        counter_i;
    icf_return_t        ret_val = ICF_SUCCESS;

    *p_next_free_index = 0;
    for( counter_i=0; counter_i< la_pool_size; counter_i++ )
    {
        p_la_pool[ counter_i ].next_index = (icf_uint16_t)(counter_i+1);
    }
    p_la_pool[ la_pool_size - 1 ].next_index = la_pool_size;
                                                /* Invalid index */
                
    p_ecode = p_ecode;
    return ret_val;
}

/*************************************************************************
 *
 *     FUNCTION NAME    :  ippst_dbm_la_allocate
 *
 *     DESCRIPTION      :  This function manipulates the linked array 
 *                         structure after a control block has been
 *                         allocated from the pool.
 *
 *  RETURNS             : ICF_SUCCESS,in case there are blocks to allocate,
 *                        else ICF_FAILURE with the corrresponding ecode.
 *
*************************************************************************/

icf_return_t
icf_dbm_la_allocate (
        IN      icf_uint16_t     la_pool_size,
        INOUT   icf_dbm_la_st    *p_la_pool,
        INOUT   icf_uint16_t     *p_next_free_index,
        OUT     icf_uint16_t     *p_new_index,
        OUT     icf_uint16_t    *p_status,
        OUT     icf_error_t      *p_ecode )
{
    icf_dbm_la_st         *p_temp_la = ICF_NULL;
    icf_uint16_t          next_free_index;
    icf_return_t          ret_val = ICF_SUCCESS;
	
    next_free_index = *p_next_free_index;

    ICF_PRINT(((icf_uint8_t *)"\n[DBM]: Invoked icf_dbm_la_allocate for pool-size = %d and " \
			"next-free-index = %d", la_pool_size, next_free_index));

    if( next_free_index == la_pool_size )
    {
        *p_status = ICF_DBM_CTRL_BLK_EXHAUSTED;
    }
    else
    {
        *p_new_index = next_free_index;
        p_temp_la = &p_la_pool[ next_free_index ];

        *p_next_free_index = p_temp_la->next_index;
	
        p_la_pool[ *p_new_index ].next_index = la_pool_size; /* Invalid */
        
    }
    p_ecode = p_ecode; /*To remove warning*/
    return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME    :  icf_dbm_la_deallocate
 *
 *  DESCRIPTION      :  This function manipulates the linked array 
 *                      structure after a control block has been
 *                      returned to the pool.
 *
 *  RETURNS          :  ICF_SUCCESS    
 *
 *************************************************************************/

icf_return_t
icf_dbm_la_deallocate (
        INOUT   icf_dbm_la_st       *p_la_pool,
        INOUT   icf_uint16_t        *p_next_free_index,
        OUT     icf_uint16_t        p_index_to_free,
        OUT     icf_error_t         *p_ecode )
{
    icf_dbm_la_st       *p_temp_la;
    icf_return_t        ret_val = ICF_SUCCESS;    

    p_temp_la = &p_la_pool[ p_index_to_free ];


    p_temp_la->next_index = *p_next_free_index;
    *p_next_free_index = p_index_to_free;

     p_ecode = p_ecode; /*To remove warning*/
                
    return ret_val;
}

