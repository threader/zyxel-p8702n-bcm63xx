/*------------------------------------------------------------------------------
 *
 * File name        : icf_cm_init.c
 *
 * Purpose          : This file contains the functions to initialize and 
 *                    de-initialize the global database of CM.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 25-Nov-2004  R Ezhirpavai    ICF LLD      Initial
 * 21-Jan-2005  Rohit Aggarwal                  Beginning of coding phase
 * 31-Jan-2005  Rohit Aggarwal                  Added deinit function
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_cm_prototypes.h"
#include "icf_cm_macro.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_init
 *
 * DESCRIPTION:     This function is invoked at startup and is used to take the
 *                  default data values that would help in having a data driven
 *                  approach for routing messages.
 *                  This function would be initially called so that the data
 *                  driven variables can be stored. This function mallocs the
 *                  CM data and returns back this data to the calling function
 *                  so that this data can be stored in global data.
 *                  It would be retrieved from global data on each invocation.
 *                  There are different functions for forward rules setting as
 *                  compared to backward rules setting. Hence this calls those
 *                  functions to set these values.
 *
 ******************************************************************************/
icf_return_t icf_cm_init(
        INOUT   icf_glb_pdb_st       *p_glb_pdb,
        OUT     icf_void_t           **p_p_cm_data,
        OUT     icf_error_t          *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t                      num_checks = ICF_NULL;
    icf_cm_pdb_st                    cm_pdb, *p_cm_pdb = &cm_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CM)
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Populate the CM pdb */
    
    /* pointer to action_result in CM's local pdb is null */
    cm_pdb.p_action_result = ICF_NULL;

    /* pointer to received internal message in CM's local pdb is null */
    cm_pdb.p_recv_int_msg = ICF_NULL;

    /* pointer to sent internal message in CM's local pdb is null */
    cm_pdb.p_sent_int_msg = ICF_NULL;

    /* allocate memory for CM global data */
    ICF_STATIC_MEMGET(p_glb_pdb, 
            ICF_PORT_SIZEOF(icf_cm_data_st), 
            cm_pdb.p_cm_data,
            ICF_RET_ON_FAILURE, 
            p_ecode, 
            ret_val)
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != cm_pdb.p_cm_data))
   
    /* initialize global data pointers */
    for (num_checks=0; num_checks<ICF_CM_CHECK_TYPE_MAX; num_checks++)
    {
        cm_pdb.p_cm_data->p_new_check[num_checks] = ICF_NULL;
    }
    
    /* copy global pdb pointer into CM's local pdb */
    cm_pdb.p_glb_pdb = p_glb_pdb;
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != cm_pdb.p_glb_pdb))
    
    /* reset the common bitmask */
    cm_pdb.common_bitmask = 0;
   
    /* pointer to value_action in CM's local pdb is null */
    cm_pdb.p_value_action = ICF_NULL;

    /* pointer to action in CM's local pdb is null */
    cm_pdb.p_action = ICF_NULL;

    
    /* CM's global data pointer needs to be returned to the calling module */
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_p_cm_data))
    *p_p_cm_data = (icf_void_t *)(cm_pdb.p_cm_data);

#ifdef ICF_SERVICE_ENABLE       
    /* Invoke functions to populate the rules in CM's global data */
    if (ICF_FAILURE == icf_cm_db_set_forward_data(p_cm_pdb))
    {
        /* setting of rules for forward data into CM's global data */
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_cm_db_set_sc_backward_data(p_cm_pdb))
    {
        /* setting of rules for backward data from SC into CM's global data */
        ret_val = ICF_FAILURE;
    }
    else 
    {
#endif /* #ifdef ICF_SERVICE_ENABLE */

        /* setting of rules for backward data from CC into CM's global data */
        ret_val = icf_cm_db_set_cc_backward_data(p_cm_pdb);

#ifdef ICF_SERVICE_ENABLE
    }
#endif /* #ifdef ICF_SERVICE_ENABLE */

    /* Free the allocated memory in case of any failure */
    if (ICF_FAILURE == ret_val)
    {
        icf_cm_deinit(p_glb_pdb, *p_p_cm_data, p_ecode);
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_RESET_MODULE(p_glb_pdb)
    return ret_val;
} /* End of icf_cm_init */


/*******************************************************************************
 *
 * FUNCTION:        icf_cm_deinit
 *
 * DESCRIPTION:     This function is invoked at system termination and is used 
 *                  to free the global data values set at startup.
 *                  The following figure illustrates the hierarchy to be freed:
 *
 *                  icf_cm_init()-> icf_cm_util_delete_nodes()
 *                  |
 *                  ------------------------------------------------
 *                  p_new_check (list)
 *                  |
 *                  ----p_field_check
 *                      |
 *                      ----p_value_action_list (list)
 *                          |
 *                          ----p_action
 *                              |
 *                              ----p_new_check -> icf_cm_util_delete_nodes()
 *
 ******************************************************************************/
icf_void_t icf_cm_deinit(
        INOUT   icf_glb_pdb_st       *p_glb_pdb,
        OUT     icf_void_t           *p_data,
        OUT     icf_error_t          *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t                      num_checks = ICF_NULL;
    icf_cm_data_st                   *p_cm_data = ICF_NULL;
    icf_cm_new_check_st              *p_new_check = ICF_NULL;
    icf_cm_new_check_st              *p_next_new_check = ICF_NULL;
    icf_cm_pdb_st                    cm_pdb;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CM)
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* cast the global data into module specific type */
    p_cm_data = (icf_cm_data_st *)p_data;

    /* Populate the CM pdb */
    cm_pdb.p_action_result = ICF_NULL;
    cm_pdb.p_recv_int_msg = ICF_NULL;
    cm_pdb.p_sent_int_msg = ICF_NULL;
    cm_pdb.p_cm_data = (icf_cm_data_st *)p_data;
    cm_pdb.p_glb_pdb = p_glb_pdb;
    cm_pdb.common_bitmask = 0;
    cm_pdb.p_value_action = ICF_NULL;
    cm_pdb.p_action = ICF_NULL;

    /* Free the forward data, SC backward data and CC backward data */
    for (num_checks=0; num_checks<ICF_CM_CHECK_TYPE_MAX; num_checks++)
    {
        /* Free the three global arrays for three data(s) in turn */
        p_new_check = (icf_cm_new_check_st *)(p_cm_data->
                                                    p_new_check[num_checks]);

        /* Free all the root level new_check nodes in the list */
        while (ICF_NULL != p_new_check)
        {
            /* hold the pointer to next new_check */
            p_next_new_check = p_new_check->p_next;

            /* Invoke recursive function to delete all checks */
            ret_val = icf_cm_util_delete_nodes(
                                &cm_pdb, 
                                p_new_check, 
                                p_ecode);
            
            /* point to the next new_check to be freed */
            p_new_check = p_next_new_check;
        }
                
    } /* End of for (num_checks) */

    /* Finally, free the memory allocated to CM global data */
    ICF_STATIC_MEMFREE(
            cm_pdb.p_glb_pdb,
            p_data,
            p_ecode,
            ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_RESET_MODULE(p_glb_pdb)
    return;
} /* End of icf_cm_deinit */
