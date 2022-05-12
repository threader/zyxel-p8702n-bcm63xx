/*******************************************************************************
 *
 * FILE NAME :      icf_sm_entry_pt.c
 *
 * DESCRIPTION:     This file contains the code of the entry point function 
 *                  of the SM module of the Packet Phone.
 *                                    
 * REVISION HISTORY: 
 * 
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 09-Mar-2005  Jyoti Narula    	ICF LLD	Initial
 * 06-Apr-2005  Jyoti Narula        Code Review Comments Updation
 * 14-Mar-2006  Amit Sharma                 CFG merged with ICF 2.0
 * 08-JUN-2007  Alok Tiwari                 Code added for handling stat APIs.
 *
 * Copyright (c) 2006, Aricent.
 ******************************************************************************/

#include "icf_macro.h"
#include "icf_common_prototypes.h"
#include "icf_sm_int_types.h"
#include "icf_sm_defs.h"
#include "icf_sm_macro.h"
#include "icf_sm_prototypes.h"
#include "icf_dbm_prototypes.h"
/*#include "icf_cfg_prototypes.h"*/
#include "icf_internal_api_id.h"



/***********************************************************************
 *
 * FUNCTION:        icf_sm_api_to_event
 *
 * DESCRIPTION:     It performs the mapping of received API Ids 
 *                  to SM events.These events are used to call 
 *                  the appropriate API handlers.
 *                  It checks the API ID received and updates 
 *                  the appropriate statistic value using ICF_STAT.
 *
 * RETURNS:         ICF_SUCCESS in case of success
 *                  and ICF_FAILURE in
 *                  case of failure. Failure occurs when an invalid
 *                  API ID is received.
 ***********************************************************************/
icf_return_t icf_sm_api_to_event(
        INOUT    icf_sm_pdb_st *p_sm_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_sm_pdb->p_glb_pdb)


    switch(p_sm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id)
    {
        case ICF_TIMER_EXPIRY_IND:
            /* Timer Expiry indication received */
            ICF_SM_TRACE(p_sm_pdb,
                        ICF_TRACE_TIMER_EXPIRED)
            /* Setting the value of curr_event in the sm_pdb. */
            p_sm_pdb->curr_event = ICF_SM_TIMER_EXPIRY;
            break;

        case ICF_SM_CC_SET_TRACE_LEVEL_REQ:

            /* Copying the received API's payload. */
            p_sm_pdb->p_api_payload = p_sm_pdb->p_glb_pdb->
                p_recv_msg->payload;

            /* using relevant Macros. */
            ICF_ASSERT(p_sm_pdb->p_glb_pdb,
                    (ICF_NULL != p_sm_pdb->p_api_payload))

            ICF_SM_TRACE(p_sm_pdb,
                        ICF_SM_TRACE_TRACE_LVL_REQ_RECD) 
            ICF_STAT(p_sm_pdb->p_glb_pdb,ICF_STAT_SM_CC_SET_TRC_LVL_REQ)

            /* Setting the value of curr_event in the sm_pdb. */
            p_sm_pdb->curr_event = ICF_SM_SET_TRACE_LVL;
            break;

	case ICF_SET_TRACE_REQ:
	    /* Copying the received API's payload. */
            p_sm_pdb->p_api_payload = p_sm_pdb->p_glb_pdb->
                p_recv_msg->payload;

            /* using relevant Macros. */
            ICF_ASSERT(p_sm_pdb->p_glb_pdb,
                    (ICF_NULL != p_sm_pdb->p_api_payload))

	    /* Setting the value of curr_event in the sm_pdb. */
            p_sm_pdb->curr_event = ICF_SM_SET_TRACE_REQ;
            break;
	
        case ICF_SM_CC_SET_ERROR_REPORT_LEVEL_REQ:

            /* Copying the received API's payload. */
            p_sm_pdb->p_api_payload = p_sm_pdb->p_glb_pdb->
                p_recv_msg->payload;

            /* using relevant Macros. */
            ICF_ASSERT(p_sm_pdb->p_glb_pdb,
                    (ICF_NULL != p_sm_pdb->p_api_payload))

             ICF_SM_TRACE(p_sm_pdb,
                        ICF_SM_TRACE_ERROR_LVL_REQ_RECD)  
             ICF_STAT(p_sm_pdb->p_glb_pdb,
                        ICF_STAT_SM_CC_SET_ERR_REP_LVL_REQ)

            /* Setting the value of curr_event in sm_pdb. */
             p_sm_pdb->curr_event = ICF_SM_SET_ERROR_LVL;
            break;

        case ICF_GET_ICF_STATS_REQ:

            /* Copying the received API's payload. */
            p_sm_pdb->p_api_payload = p_sm_pdb->p_glb_pdb->
                p_recv_msg->payload;

            /* using relevant Macros */
            ICF_ASSERT(p_sm_pdb->p_glb_pdb,
                    (ICF_NULL != p_sm_pdb->p_api_payload))

            ICF_SM_TRACE(p_sm_pdb,
                        ICF_SM_TRACE_GET_STATS_REQ_RECD)
            ICF_STAT(p_sm_pdb->p_glb_pdb,
                    ICF_STAT_SM_CC_GET_STATS_REQ)

            /* Setting the value of curr_event in sm_pdb. */
            p_sm_pdb->curr_event = ICF_SM_GET_STATS;
            break;

        case ICF_SM_CC_CHECK_MODULE_STATUS_REQ:

            /* using relevant Macros. */
            ICF_SM_TRACE(p_sm_pdb,
                    ICF_SM_TRACE_CHK_MODULE_REQ_RECD)
            ICF_STAT(p_sm_pdb->p_glb_pdb,
                    ICF_STAT_SM_CC_CHK_MOD_STS_REQ)

            /* Setting the value of curr_event in sm_pdb. */ 
            p_sm_pdb->curr_event = ICF_SM_CHK_MODULE;
            break;

        case ICF_PA_CC_GET_CALL_LOG_REQ:

            /* Using relevant macros. */
            ICF_SM_TRACE(p_sm_pdb,
                    ICF_SM_TRACE_CALL_LOG_REQ_RECD)
            ICF_STAT(p_sm_pdb->p_glb_pdb,
                    ICF_STAT_PA_CC_GET_CALL_LOG_REQ)

            /* Setting the value of curr_event in sm_pdb. */
            p_sm_pdb->curr_event = ICF_SM_CALL_LOG ;
            break;

	case ICF_STATS_REQ:

            /* Copying the received API's payload. */
            p_sm_pdb->p_api_payload = p_sm_pdb->p_glb_pdb->
                p_recv_msg->payload;

            /* using relevant Macros */
            ICF_ASSERT(p_sm_pdb->p_glb_pdb,
                    (ICF_NULL != p_sm_pdb->p_api_payload))

            /* Setting the value of curr_event in sm_pdb. */
            p_sm_pdb->curr_event = ICF_SM_STATS;
            break;


        default:
            ret_val = ICF_FAILURE;
            break;

    }/* Switch ends here */

    ICF_FUNCTION_EXIT(p_sm_pdb->p_glb_pdb)
    return ret_val;  

}/* Function icf_sm_api_to_event() ends here. */


/****************************************************************************
 * FUNCTION:        icf_sm_process_mesg
 *
 * DESCRIPTION:     This is the entry point function for SM Handler.
 *                  It puts values into required fields into 
 *                  icf_sm_pdb_st's variable.
 *                  It checks the API ID received and then calls
 *                  the appropriate handler to process the API. 
 *
 * RETURNS:         ICF_SUCCESS always:
 *                      -When Invalid_api_id recd: ECODE set.
 *                      -DBM returns failure: no action.
 *                      -The API Handler returns Failure: no action
 *
 *
 *************************************************************************/
icf_return_t icf_sm_process_mesg( 
        INOUT   icf_internal_msg_st  *p_internal_msg)
{
    icf_sm_pdb_st  sm_pdb;
    icf_sm_pdb_st *p_sm_pdb =  ICF_NULL;
    icf_sm_glb_data_st *p_sm_data  =  ICF_NULL;

    icf_return_t  ret_val = ICF_SUCCESS;

    /* Using relevant Macros. */
    ICF_SET_MODULE(p_internal_msg-> 
            msg_hdr.p_glb_pdb,ICF_INT_MODULE_SM)

    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)
    
    /* Copy the global pdb pointer locally in pdb */
    sm_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    /* Copy the received internal message pointer locally in pdb */
    sm_pdb.p_internal_msg = p_internal_msg;


    /* initializing the p_ecode member of sm_pdb.*/        
    sm_pdb.p_ecode = &(sm_pdb.p_internal_msg->msg_hdr.ecode);

    /* Initializing the param_validity member of sm_pdb. */
    sm_pdb.param_validity = ICF_FALSE;

    ret_val = icf_dbm_get_module_glb_data(sm_pdb.p_glb_pdb,
            (icf_module_id_t)(ICF_INT_MODULE_SM),
            (icf_void_t *)&(sm_pdb.p_sm_data),
            sm_pdb.p_ecode);
    
    p_sm_data = (icf_sm_glb_data_st *)(sm_pdb.p_sm_data);


    if (ICF_FAILURE != ret_val)
    {
        /* DBM successfully returned global data: processing begins. */
        
        ret_val = icf_sm_api_to_event(&sm_pdb);

        if (ICF_SUCCESS == ret_val)
        {
            /* SM related API recd: Processing Begins */

            ret_val = p_sm_data->api_hdlr_list
                        [sm_pdb.curr_event](&sm_pdb);
            if(ICF_FAILURE == ret_val)
            {
                                
                ret_val = ICF_SUCCESS;
            }
        }/* SM related API recd: Processing Ends */
        
        else
        {
            /* Incase non SM API is recd. Processing Begins.*/

            if (ICF_SM_CC_RESTART_REQ ==     
                    sm_pdb.p_glb_pdb->p_recv_msg->hdr.api_id ||
               ICF_SM_CC_DELETE_CONFIG_PARAMS_REQ ==
                    sm_pdb.p_glb_pdb->p_recv_msg->hdr.api_id )
            {
                /* These APIs.have to be passed to the CFG.*/
                ret_val = icf_cfg_process_mesg(p_internal_msg);

                ret_val = ICF_SUCCESS;
                
            }
            else
            {
                
            /* This is an invalid API, raise error_n_trace */
            p_sm_pdb = &sm_pdb;
            ICF_SM_SET_ECODE(p_sm_pdb,ICF_ERROR_INVALID_API_ID)
           
            ret_val = ICF_SUCCESS;
            }
            
        }/* Incase non SM API is recd. Processing ends.*/
        
    }/* DBM successfully returned global data : processing ends. */


    else
    {
        /*DBM failed to return SM Global data.
         * Still the value returned to MRM is SUCCESS. */
        
        ret_val = ICF_SUCCESS;

    }


    ICF_FUNCTION_EXIT(sm_pdb.p_glb_pdb)
        ICF_RESET_MODULE(sm_pdb.p_glb_pdb)
        return ret_val;


}/* Function icf_sm_process_mesg() ends here.*/



