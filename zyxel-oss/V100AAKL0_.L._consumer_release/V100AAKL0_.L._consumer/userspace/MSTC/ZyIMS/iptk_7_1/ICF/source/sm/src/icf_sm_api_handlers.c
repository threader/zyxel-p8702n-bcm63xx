/*********************************************************************
 *
 * FILE NAME :      icf_sm_api_handlers.c
 *
 * DESCRIPTION:     This file contains the code of the API handlers in the
 *                  SM module of the Packet Phone.
 *
 * REVISION HISTORY:
 *
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 10-Mar-2005  Jyoti Narula    ICF LLD  Initial
 * 06-Apr-2005  Jyoti Narula    Code Review Comments Updation
 * 14-Mar-2006  Amit Sharma                 CFG merged with ICF 2.0
 * 06-Oct-2006  Aman Arora      ICF Rel 6.1  Enhanced licensing    
 *
 * 08-JUN-2007  Alok Tiwari     ICF Early 7.0  Enhanced code for Stat
 * Copyright (c) 2006, Aricent.
 *********************************************************************/


#include<stdio.h>
#include<stdlib.h>
#include "icf_macro.h"
#include "icf_sm_prototypes.h"
#include "icf_sm_defs.h"
#include "icf_sm_macro.h"
#include "icf_sm_int_types.h"
#ifdef ICF_STAT_ENABLED
#include "icf_stats_defs.h"
#endif
#include "icf_dbm_prototypes.h"
#include "icf_common_types.h"


#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 * FUNCTION:        icf_sm_set_trace_hdlr
 *
 * DESCRIPTION:     This function is called when ever the API 
 *                  ICF_SM_CC_SET_TRACE_LEVEL_REQ is received.
 *                  It sets the value of trace_level and trace_type
 *                  in the global data structure as given in the
 *                  payload of incoming API.This function sets the
 *                  values in the global data structure for 
 *                  trace_status,trace_type, trace_level
 *                  in the module data of SM in the glb_pdb.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and 
 *                  ICF_FAILURE in case of failure.
 *                  Failure occurs:
 *                      -When invalid trace_details are received in the 
 *                      API.
 *
 *********************************************************************/

icf_return_t icf_sm_set_trace_hdlr(
        INOUT   icf_sm_pdb_st *p_sm_pdb)
{

#ifdef ICF_TRACE_ENABLE
 
    icf_return_t ret_val  =  ICF_SUCCESS; 
    icf_sm_cc_set_trace_level_req_st *p_recd_msg_payload =  ICF_NULL;
    icf_sm_glb_data_st *p_sm_data =  ICF_NULL;
    icf_glb_pdb_st *p_glb_pdb =  ICF_NULL;

    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        p_sm_pdb->p_ecode = &(p_sm_pdb->p_internal_msg->
                msg_hdr.ecode);


    p_recd_msg_payload = (icf_sm_cc_set_trace_level_req_st *)
        (p_sm_pdb->p_api_payload);

    /* No need to validate the level and type parameters if the application
     * is trying to turn OFF the logs
     */
    if (ICF_TRUE == p_recd_msg_payload->status)
    {
        icf_sm_validate_trace_details(p_sm_pdb,
            (icf_sm_cc_set_trace_level_req_st *)p_recd_msg_payload);
    }
    else
    {
        p_sm_pdb->param_validity = ICF_TRUE;
    }

    ICF_SM_TRACE(p_sm_pdb,ICF_SM_TRACE_VALIDATE_PARAM)

        if (ICF_TRUE == p_sm_pdb->param_validity)
        {/* The recd. trace_details are valid.processing begins */

            p_sm_data = (icf_sm_glb_data_st *)
                (p_sm_pdb->p_sm_data);

            p_sm_data->trace_status = p_recd_msg_payload->status;
            p_sm_data->trace_level = p_recd_msg_payload->trace_level;
            p_sm_data->trace_type = p_recd_msg_payload->trace_type;

            p_glb_pdb->trace_info.status = p_recd_msg_payload->status;
            p_glb_pdb->trace_info.level = p_recd_msg_payload->trace_level;
            p_glb_pdb->trace_info.type = p_recd_msg_payload->trace_type;

            /* For UT: to be removed later: */

            ICF_PRINT(("\n[SM]: The trace details set in the global data: \
                         trace_status = %d , trace_level = %d , \
                         trace_type = %d ",
                         p_sm_data->trace_status,
                         p_sm_data->trace_level,
                         p_sm_data->trace_type));

        }/* The recd. trace details are valid.processing ends */

        else
        {/* Invalid parameter recd. */
            ICF_SM_SET_ECODE(p_sm_pdb,ICF_ERROR_INVALID_PARAM)
                ret_val = ICF_FAILURE;
        }
    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    p_sm_pdb = p_sm_pdb;

    return ret_val;

#endif

}/* Function icf_sm_set_trace_hdlr() ends here. */



/**********************************************************************
 * FUNCTION:        icf_sm_set_trace_req_hdlr
 *
 * DESCRIPTION:     This function is called when ever the API 
 *                  ICF_SM_CC_SET_TRACE_REQ is received.
 *                  It sets the value of trace_criteria and trace_type
 *                  in the global data structure as given in the
 *                  payload of incoming API.This function sets the
 *                  values in the global data structure for 
 *                  trace_status,trace_type, trace_level
 *                  in the module data of SM in the glb_pdb.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and 
 *                  ICF_FAILURE in case of failure.
 *                  Failure occurs:
 *                      -When invalid trace_details are received in the 
 *                      API.
 *
 *********************************************************************/

icf_return_t icf_sm_set_trace_req_hdlr(
        INOUT   icf_sm_pdb_st *p_sm_pdb)
{

#ifdef ICF_TRACE_ENABLE
 
    icf_return_t ret_val  =  ICF_SUCCESS; 
    icf_set_trace_req_st *p_recd_msg_payload =  ICF_NULL;
    icf_sm_glb_data_st *p_sm_data =  ICF_NULL;
    icf_glb_pdb_st *p_glb_pdb =  ICF_NULL;

    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        p_sm_pdb->p_ecode = &(p_sm_pdb->p_internal_msg->
                msg_hdr.ecode);


    p_recd_msg_payload = (icf_set_trace_req_st *)
        (p_sm_pdb->p_api_payload);

    /* No need to validate the level and type parameters if the application
     * is trying to turn OFF the logs
     */
        p_sm_pdb->param_validity = ICF_TRUE;
    /*
    if (ICF_TRUE == p_recd_msg_payload->status)
    {
        icf_sm_validate_trace_details(p_sm_pdb,
            (icf_set_trace_req_st *)p_recd_msg_payload);
    }
    else
    {
        p_sm_pdb->param_validity = ICF_TRUE;
    }
    */

        if (ICF_TRUE == p_sm_pdb->param_validity)
        {/* The recd. trace_details are valid.processing begins */

            p_sm_data = (icf_sm_glb_data_st *)
                (p_sm_pdb->p_sm_data);

            p_sm_data->trace_status = p_recd_msg_payload->status;

            if(ICF_TRACE_CRITERIA_PRESENT & p_recd_msg_payload->bitmask)
            {
                p_sm_data->trace_criteria = p_recd_msg_payload->trace_criteria;
            }

            if(ICF_TRACE_TYPE_PRESENT & p_recd_msg_payload->bitmask)
            {
                 p_sm_data->trace_type = p_recd_msg_payload->trace_type;
            }

            if(ICF_TRACE_RULE_PRESENT & p_recd_msg_payload->bitmask)
            {
	         p_sm_data->trace_rule = p_recd_msg_payload->trace_rule;
            }

        }/* The recd. trace details are valid.processing ends */

        else
        {/* Invalid parameter recd. */
            ICF_SM_SET_ECODE(p_sm_pdb,ICF_ERROR_INVALID_PARAM)
                ret_val = ICF_FAILURE;
        }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    p_sm_pdb = p_sm_pdb;

    return ret_val;

#endif

}/* Function icf_sm_set_trace_req_hdlr() ends here. */


#endif


/***********************************************************************
 * FUNCTION:        icf_sm_set_error_hdlr
 *
 * DESCRIPTION:     This function is called when ever the API 
 *                  ICF_SM_CC_SET_ERROR_LEVEL_REQ is received.
 *                  It sets the value of error_status, error_level and 
 *                  error_type in the global data structure as given 
 *                  in the payload of incoming API.
 *
 * RETURNS:         ICF_SUCCESS in case of success and 
 *                  ICF_FAILURE in case of failure. Failure
 *                  occurs when
 *                  -Invalid error_details are entered.
 *
 *********************************************************************/

icf_return_t icf_sm_set_error_hdlr(
        INOUT    icf_sm_pdb_st *p_sm_pdb)
{
#ifdef ICF_ERROR_ENABLE
    icf_return_t ret_val = ICF_SUCCESS;
    icf_sm_cc_set_error_report_level_req_st 
                    *p_recd_msg_payload =  ICF_NULL;
    icf_glb_pdb_st *p_glb_pdb =  ICF_NULL;
    icf_sm_glb_data_st *p_sm_data =  ICF_NULL;

    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        p_sm_pdb->p_ecode = &(p_sm_pdb->p_internal_msg->
                msg_hdr.ecode);


    p_recd_msg_payload = (icf_sm_cc_set_error_report_level_req_st *)
        (p_sm_pdb->p_api_payload);

    icf_sm_validate_error_details(p_sm_pdb,\
            (icf_sm_cc_set_error_report_level_req_st *)\
            p_recd_msg_payload);

    ICF_SM_TRACE(p_sm_pdb,ICF_SM_TRACE_VALIDATE_PARAM)

        if (ICF_TRUE == p_sm_pdb->param_validity)
        {/* The recd. error_details are valid.processing begins */                  
            p_sm_data = (icf_sm_glb_data_st *)(p_sm_pdb->p_sm_data);

            p_sm_data->error_status = p_recd_msg_payload->status;
            p_sm_data ->error_level = p_recd_msg_payload->error_level;
            p_sm_data->error_type =   p_recd_msg_payload->error_type;

            p_glb_pdb->error_info.status = p_recd_msg_payload->status;
            p_glb_pdb->error_info.level = p_recd_msg_payload->error_level;
            p_glb_pdb->error_info.type = p_recd_msg_payload->error_type;

            /* For UT: to be removed later: */

            ICF_PRINT(("\n[SM]: The error details set in the global data: \
                         error_status = %d , error_level = %d ,\
                         error_type = %d ",
                         p_sm_data->error_status,
                         p_sm_data->error_level,
                         p_sm_data->error_type));



        }/* The recd. error_details are valid.processing ends. */

        else
        {/* Invalid parameters recd. */
            ICF_SM_SET_ECODE(p_sm_pdb,ICF_ERROR_INVALID_PARAM)

                ret_val = ICF_FALSE;
        }

    ICF_FUNCTION_EXIT(p_glb_pdb)

        return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    p_sm_pdb = p_sm_pdb;

    return ret_val;

#endif

    
}/* Function icf_sm_set_error_hdlr() ends here. */


/**********************************************************************
 * FUNCTION:        icf_sm_get_stats_hdlr
 * 
 * DESCRIPTION:     This function is called by the function 
 *                  icf_process_mesg whenever the
 *                  API ICF_GET_STAT_REQ is 
 *                  received. This function reads the value of 
 *                  stat id that is received with the API and 
 *                  fills the response API structure with 
 *                  the appropriate values of the 
 *                  stats/members of the stat group.
 *
 * RETUENS:         ICF_SUCCESS in case of success and ICF_FAILURE
 *                  otherwise.Failure occurs when:
 *                  -Memory allocation for p_sm_msg fails.
 *                  -SEND Macro returned failure. 
 *********************************************************************/

icf_return_t icf_sm_get_stats_hdlr(
        INOUT    icf_sm_pdb_st *p_sm_pdb)
{
#ifdef ICF_STAT_ENABLED

    icf_return_t ret_val = ICF_SUCCESS;
    icf_stats_id_t recd_stats_id =  ICF_NULL;

    icf_get_stats_resp_st *p_sm_msg_payload = ICF_NULL ;

    icf_get_icf_stats_req_st *p_recd_msg_payload =  ICF_NULL;
    icf_glb_pdb_st *p_glb_pdb =  ICF_NULL;

    icf_msg_st *p_sm_msg = ICF_NULL ; 

    icf_uint8_t stat_index =  ICF_NULL;
    icf_uint8_t resp_stat_index =  ICF_NULL;
    icf_uint16_t api_len =  ICF_NULL;

    icf_sm_stats_index_st recd_stats_index;

    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /*To raise the trace for function entry.*/

    ICF_FUNCTION_ENTER(p_glb_pdb)


        p_recd_msg_payload = (icf_get_icf_stats_req_st *)
        (p_sm_pdb->p_api_payload);

    /* To see the values during UT: to be removed later:*/
    ICF_PRINT(("\n[SM]:The values recd in get_stats_req: \
                     stats_id = %d, reset_flag = %d ",
                     p_recd_msg_payload->stats_id,
                     p_recd_msg_payload->reset));

    /* This function gives the value to reset flag as TRUE
     * if the recd_reset is any non-zero value. */
    icf_sm_validate_get_stats(p_sm_pdb,\
            (icf_get_icf_stats_req_st *)p_recd_msg_payload);

    ICF_SM_TRACE(p_sm_pdb,ICF_SM_TRACE_VALIDATE_PARAM)


        api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
        ICF_PORT_SIZEOF(icf_get_stats_resp_st);

    /* Memory allocation for p_sm_msg. */

    ICF_MEMGET(p_glb_pdb,api_len,ICF_MEM_COMMON,\
            p_sm_msg,ICF_RET_ON_FAILURE,\
            p_sm_pdb->p_ecode,ret_val)


        if (ICF_FAILURE != ret_val)
        {
            /* Successful memory allocation for p_sm_msg : processing begins. */

            /* Filling the header fields for the response */


            p_sm_msg->hdr.api_id = ICF_GET_ICF_STATS_RESP;
            p_sm_msg->hdr.source_id = ICF_MODULE_ICF;
            p_sm_msg->hdr.destination_id = ICF_MODULE_SM;
            p_sm_msg->hdr.api_length =  ICF_PORT_SIZEOF(
                    icf_api_header_st) +
                ICF_PORT_SIZEOF(icf_get_stats_resp_st);


            /* Filling the payload */

            p_sm_msg_payload = (icf_get_stats_resp_st *)
                (p_sm_msg->payload);



            /*Retrieving the stats id from the get_stats_request API*/

            recd_stats_id = p_recd_msg_payload->stats_id;

            p_sm_msg_payload->stats_id = recd_stats_id;
            
            

            if (ICF_TRUE == p_sm_pdb->param_validity)
            {
                /* Received stats_id is within valid range.Processing Begins */

                if (ICF_SUCCESS == icf_sm_get_stats_index(\
                            recd_stats_id,&(recd_stats_index)))
                {

                    /* For all the members lying between these 2 
                     * indices of current group and next group, 
                     * the value has to be copied into the response structure.*/

                    resp_stat_index=0;

                    for (stat_index = recd_stats_index.curr_stats_group +1; 
                            stat_index < recd_stats_index.next_stats_group; 
                            stat_index++, resp_stat_index++)
                    {
                                                
                        p_sm_msg_payload->stats_data[resp_stat_index] =
                            ((icf_stat_glb_st *)
                             (p_glb_pdb->p_glb_data->
                              p_stats_data))->stat_counter_list[stat_index];
                    }

                    p_sm_msg_payload->result = ICF_SUCCESS ;
                    p_sm_msg_payload->num_of_stats_elements = resp_stat_index ;

                    if (ICF_TRUE == p_recd_msg_payload->reset)
                    {
                        /* After filling the response structure the stats in the
                           counter list have to be reset. */

                        for (stat_index = recd_stats_index.curr_stats_group+1;
                                stat_index < 
                                recd_stats_index.next_stats_group; 
                                stat_index++)
                        {
                            ((icf_stat_glb_st *)
                             (p_glb_pdb->p_glb_data->
                              p_stats_data))->stat_counter_list[stat_index] =0 ;
                            
                        }


                    }/* Resetting done for the whole group. */

                }/* All members of the recd group id have been copied. */


                else
                {
                    /* When the recd_stats_id is an individual stats 
                     * to be updated.To indicate this the function
                     * icf_sm_get_stats_index() returns ICF_FAILURE */

                   
                    p_sm_msg_payload->stats_data[0]= 
                        ((icf_stat_glb_st *)\
                         (p_glb_pdb->p_glb_data->p_stats_data))->
                        stat_counter_list[recd_stats_id];

                   
                    p_sm_msg_payload->result=ICF_SUCCESS;
                    p_sm_msg_payload->num_of_stats_elements=1;

                    if (ICF_TRUE == p_recd_msg_payload->reset)
                    {
                        /* This stat has to be reset in the counter list. */
                        ((icf_stat_glb_st *)
                         (p_glb_pdb->p_glb_data->p_stats_data))->
                            stat_counter_list[recd_stats_id] = 0 ;

                        

                    }
                }/*Individual stats_id: processing ends. */

            }/* Valid stats_id received. Processing ends */

            else
            {
                /* Received Stats Id is out of range */
                p_sm_msg_payload->result=ICF_FAILURE;
                p_sm_msg_payload->error_cause=ICF_CAUSE_INVALID_STATS_ID;
                p_sm_msg_payload->num_of_stats_elements=0;


            }/* Received stats_id out of range.*/


            /* Value printing for UT: to be removed later.*/
           ICF_PRINT(("\n[SM]:Values filled in the response \
                                structure of get_stats: stats_id = %d ,\
                                num_stats = %d ",
                                p_sm_msg_payload->stats_id,
                                p_sm_msg_payload->num_of_elements));
           ICF_PRINT(("\n"));


            for (resp_stat_index = 0; resp_stat_index < 
                    p_sm_msg_payload->num_of_stats_elements; resp_stat_index++)
            {
                ICF_PRINT(("%d\t", 
                            p_sm_msg_payload->stats_data[resp_stat_index]));
            }
                       
            /*-----------UT printing ends here-----------*/

            /* Sending the message: in all cases: 
             *      valid/invalid stats_id
             *      Group stats/ individual stats_id */
            ICF_SEND(p_glb_pdb,p_sm_msg,ret_val, p_sm_pdb->p_ecode)

                if (ICF_SUCCESS == ret_val)
                {
                /*  Because this payload gets used inside TRACE:*/
                    p_sm_pdb->p_api_payload = (icf_get_stats_resp_st *)
                                         (p_sm_msg_payload);
                
                    ICF_SM_TRACE(p_sm_pdb, 
                                ICF_SM_TRACE_GET_STATS_RESP_SENT)
                        /* Free memory */
                        ICF_MEMFREE(p_glb_pdb,p_sm_msg,ICF_MEM_COMMON,\
                                p_sm_pdb->p_ecode,ret_val)
                }
                else
                {
                    ICF_MEMFREE(p_glb_pdb,p_sm_msg,ICF_MEM_COMMON,\
                            p_sm_pdb->p_ecode,ret_val)
                        ret_val = ICF_FAILURE;

                }


        }/* Successful memory allocation for p_sm_msg : processing ends. */

        else
        {

            /* Memory could not be allocated for p_sm_msg. */
            /* ret_val contains FAILURE.  Nothing to do here.*/
        }



    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    p_sm_pdb = p_sm_pdb;

    return ret_val;

#endif


}/* Function icf_sm_get_stats_hdlr() ends here. */


/*********************************************************************
 * FUNCTION:     icf_sm_chk_module_status_hdlr
 * 
 * DESCRIPTION : This function is called by the function 
 *               icf_process_mesg whenever the API 
 *               ICF_SM_CC_CHECK_MODULE_STATUS_REQ is 
 *               received. The response API is sent to indicate that
 *               the module is alive. It has no parameters.
 *
 * RETURNS:      ICF_SUCCESS in case of success and ICF_FAILURE
 *               otherwise.Failure occurs when:
 *                  -The memory fails to be allocated 
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND Macro returned failure.
 *                   
 *************************************************************/

icf_return_t icf_sm_chk_module_status_hdlr(
        INOUT    icf_sm_pdb_st *p_sm_pdb)
{

    icf_return_t ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb =  ICF_NULL;
    icf_msg_st *p_sm_msg = ICF_NULL ; 
    icf_uint8_t api_len =  ICF_NULL;

    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        p_sm_pdb->p_ecode = &(p_sm_pdb->p_internal_msg->msg_hdr.ecode);

    api_len = ICF_PORT_SIZEOF(icf_msg_st);

    /* Allocate memory to the p_sm_msg. */

    ICF_MEMGET(p_glb_pdb,api_len,ICF_MEM_COMMON,\
            p_sm_msg,ICF_RET_ON_FAILURE,\
            p_sm_pdb->p_ecode,ret_val)

     if (ICF_FAILURE != ret_val)
     {
         /* Memory successfully allocated for p_sm_msg : processing begins */

        /* Filling the header fields for the response */

        p_sm_msg->hdr.api_id = ICF_CC_SM_CHECK_MODULE_STATUS_RESP;
        p_sm_msg->hdr.source_id = ICF_MODULE_ICF;
        p_sm_msg->hdr.destination_id = ICF_GENERIC_APP_ID;
        p_sm_msg->hdr.api_length = ICF_PORT_SIZEOF(
            icf_msg_st); 

    ICF_SEND(p_glb_pdb,p_sm_msg,ret_val,\
            p_sm_pdb->p_ecode)

        if (ICF_SUCCESS == ret_val)
        {
            ICF_SM_TRACE(p_sm_pdb,ICF_SM_TRACE_CHK_MODULE_RESP_SENT)
                /* Free memory */
                ICF_MEMFREE(p_glb_pdb,p_sm_msg,ICF_MEM_COMMON,\
                        p_sm_pdb->p_ecode,ret_val)
        }
        else
        {
            ICF_MEMFREE(p_glb_pdb,p_sm_msg,ICF_MEM_COMMON,\
                    p_sm_pdb->p_ecode,ret_val)

                ret_val = ICF_FAILURE;
        }
     }/* Memory allocated for p_sm_msg: processing ends*/

    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;


}/* Function icf_sm_chk_module_status_hdlr() ends here. */


#ifdef ICF_STAT_ENABLED
/**********************************************************************
 * FUNCTION:     icf_sm_call_log_hdlr
 *
 * DESCRIPTION:  This function is called by the function 
 *               icf_process_mesg
 *               whenever the API ICF_SM_CC_GET_CALL_LOG_REQ 
 *               is received.
 *               The response API is sent after being filled 
 *               with the values.
 *
 * RETURNS:      ICF_SUCCESS in case of success and ICF_FAILURE
 *               otherwise.Failure occurs when:
 *                  -The memory fails to be allocated or freed
 *                   for icf_msg_st, that has to be sent to SM.
 *                  
 ***********************************************************************/
icf_return_t icf_sm_call_log_hdlr(
        INOUT    icf_sm_pdb_st *p_sm_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    icf_glb_pdb_st *p_glb_pdb =  ICF_NULL;
    icf_msg_st *p_sm_msg = ICF_NULL ; 

    icf_call_log_resp *p_sm_msg_payload = ICF_NULL ;

    icf_uint16_t api_len =  ICF_NULL;
    icf_sm_glb_data_st *p_sm_data =  ICF_NULL;

    icf_uint8_t call_index =  ICF_NULL;
    icf_uint8_t logged_num_of_calls =  ICF_NULL;

    /* Incase the response API has to be filled only with default_values.
     * This happens when the DBM fails to return the CFG data. */

    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /* To raise the trace for function entry. */

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_sm_pdb->p_ecode =&(p_sm_pdb->p_internal_msg->msg_hdr.ecode);

    p_sm_data = (icf_sm_glb_data_st *)
        (p_sm_pdb->p_sm_data);


    api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
        ICF_PORT_SIZEOF(icf_call_log_resp);

    ICF_MEMGET(p_glb_pdb,api_len,ICF_MEM_COMMON,p_sm_msg,\
            ICF_RET_ON_FAILURE,p_sm_pdb->p_ecode,ret_val)

        if (ICF_FAILURE != ret_val)
        {
            /* Memory allocation for p_sm_msg successful: processing begins.*/

            /* Filling the header fields for the response */

            p_sm_msg->hdr.api_id = ICF_CC_PA_CALL_LOG_RESP;
            p_sm_msg->hdr.source_id = ICF_MODULE_ICF;
            p_sm_msg->hdr.destination_id = ICF_MODULE_PA;
            p_sm_msg->hdr.api_length =  ICF_PORT_SIZEOF(
                            icf_api_header_st) +
                ICF_PORT_SIZEOF(icf_call_log_resp);


            /* Filling the payload */

            p_sm_msg_payload = (icf_call_log_resp *)\
                (p_sm_msg->payload);

            if (0 == p_sm_data->num_calls_to_log)
            {
                /* Incase the num_calls_to_log has the value 0.
                 * The num_calls will indicate that the
                 * response contains no value in the call
                 * history arrays. */

                 
                p_sm_msg_payload->num_of_dialed_calls = 0;        
                p_sm_msg_payload->num_of_missed_calls = 0;
                p_sm_msg_payload->num_of_received_calls = 0;
                

            }

            else
            {
                 /* The num_calls_to_log is non-zero
                  * The Response can be filled with 
                  * actual logged history.*/

                /* Filling the num_calls members in the 
                 * respose API:*/


                p_sm_msg_payload->num_of_dialed_calls =
                                 p_sm_data->num_dialed_calls;        
                p_sm_msg_payload->num_of_missed_calls =
                                p_sm_data->num_missed_calls;
                p_sm_msg_payload->num_of_received_calls = 
                                p_sm_data->num_recd_calls;
                

                /* Filling the Dialed Call Info into the 
                 * response API Payload. */

                call_index = p_sm_data->dialed_calls_index;
                logged_num_of_calls = p_sm_data->num_dialed_calls;


                icf_sm_fill_call_details(\
                        p_sm_msg_payload->dialed_call_info,\
                        p_sm_data->p_dialed_calls,\
                        logged_num_of_calls,\
                        call_index,\
                        p_sm_data->num_calls_to_log);


                /* Filling the Missed Call Info into the response
                 *  API Payload. */

                call_index = p_sm_data->missed_calls_index;
                logged_num_of_calls = p_sm_data->num_missed_calls;

                icf_sm_fill_call_details(\
                        p_sm_msg_payload->missed_call_info,\
                        p_sm_data->p_missed_calls,\
                        logged_num_of_calls,\
                        call_index,\
                        p_sm_data->num_calls_to_log);


                /* Filling the Received Call Info into the response
                 *  API Payload. */

                call_index = p_sm_data->recd_calls_index;
                logged_num_of_calls = p_sm_data->num_recd_calls;

                icf_sm_fill_call_details(\
                        p_sm_msg_payload->received_call_info,\
                        p_sm_data->p_recd_calls,\
                        logged_num_of_calls,\
                        call_index,\
                        p_sm_data->num_calls_to_log);

                /* Send the message */

            }/* else ends: to fill the response with logged call history.*/


            ICF_SEND(p_glb_pdb,p_sm_msg,ret_val,\
                    p_sm_pdb->p_ecode)

                if (ICF_SUCCESS == ret_val)
                {

                    ICF_SM_TRACE(p_sm_pdb,ICF_SM_TRACE_CALL_LOG_RESP_SENT)
                        /* Free memory */
                        ICF_MEMFREE(p_glb_pdb,p_sm_msg,ICF_MEM_COMMON,\
                                p_sm_pdb->p_ecode,ret_val)
                }
                else
                {

                    ICF_MEMFREE(p_glb_pdb,p_sm_msg,ICF_MEM_COMMON,\
                            p_sm_pdb->p_ecode,ret_val)
                        ret_val = ICF_FAILURE;
                }

        }/* Memory allocation for p_sm_msg successful: processing ends.*/


    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;


}/* Function icf_sm_call_log_hdlr() ends here. */
#endif

#ifdef ICF_REL_OLD
/*----------------------------------------------------------------
 * FUNCTION NAME:  icf_sm_timer_expiry_hdlr
 * 
 * DESCRIPTION:    This function performs the checks 
 *                 related to the 24 hour interval
 *                 of the license management scenario.
 *                
 * RETURNS :        SUCCESS: Incase all checks are successful
 * Also: what happens when the license expiry reached?
 *                  FAILURE: In the following scenarios:
 *                  -->Counter file not read.
 *                  -->DBM fails to give SM global data
 *                  -->Failed to get system time.
 *                  -->Failed to get the modify time of the license file.
 *                  -->Failed to write the updated license file.
 *                                   
 *
 * CALLED ON:       Expiration of 24 hour timer run by SM.
 *-----------------------------------------------------------------------*/
icf_return_t icf_sm_timer_expiry_hdlr(
        INOUT     icf_sm_pdb_st *p_sm_pdb)
{
    icf_glb_pdb_st           *p_glb_pdb =  ICF_NULL;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_sm_glb_data_st       *p_sm_data = ICF_NULL;
    icf_sm_counter_data_st   counter_data;
    icf_uint32_t             current_timestamp = ICF_NULL;
    icf_uint32_t             temp_timestamp = ICF_NULL;

    
    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /* To raise the trace for function entry. */

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_sm_pdb->p_ecode =&(p_sm_pdb->p_internal_msg->msg_hdr.ecode);

    p_sm_data = (icf_sm_glb_data_st *)
                            (p_sm_pdb->p_sm_data);
    

    p_sm_data->detection_mask = 0;

   /* Checking the time_remaining field */

    if (ICF_NULL == (p_sm_data->time_remaining - (ICF_SM_HOUR_INTERVAL * 
                                       ICF_SM_SECONDS_PER_HOUR)))
    {
        /* License expiry reached: Send the appropriate indications */
        p_sm_data->detection_mask |= ICF_SM_LICENSE_EXPIRED;
        ret_val = icf_sm_handle_lic_error(p_glb_pdb, 
                p_sm_data, ICF_TRUE);
        ret_val = ICF_FAILURE;
    }

    if (ICF_FAILURE != ret_val)
    {
        /* ret_val != FAILURE: scenario 1: Processing Begins */

        /* Reading the counter data */
        icf_port_strcpy(counter_data.reference_id,"\0"); 
        counter_data.last_known_time = ICF_NULL;
        counter_data.last_modified_time = ICF_NULL;
        if (ICF_FAILURE == icf_sm_read_counter_file(
                                    p_sm_data,
                                    &counter_data))
        {
            p_sm_data->detection_mask |= ICF_SM_FILE_FAILURE;
            ret_val = icf_sm_handle_lic_error(p_glb_pdb, 
                                p_sm_data, ICF_TRUE);
            ret_val =  ICF_FAILURE;
        }
    }/* ret_val != FAILURE: scenario 1: Processing Ends */


    if (ICF_FAILURE != ret_val) 
    {
        /* ret_val != FAILURE: scenario 2: Processing Begins */

        /* To obtain the current system time */
        temp_timestamp = ICF_NULL;
        if (ICF_FAILURE == icf_sm_validate_n_get_time(
                           &temp_timestamp,
                                &current_timestamp))
        {
            p_sm_data->detection_mask |= ICF_SM_TIME_CALL_FAILED;
            ret_val = icf_sm_handle_lic_error(p_glb_pdb,
                             p_sm_data, ICF_TRUE);
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* current timestamp obtained: verify counter
             * data : processing begins */
            
            temp_timestamp = counter_data.last_known_time + (
                                       ICF_SM_HOUR_INTERVAL * 
                                       ICF_SM_SECONDS_PER_HOUR);
            
            if (temp_timestamp != current_timestamp)
            {
                p_sm_data->detection_mask |= 
                        ICF_SM_SYS_TIME_CHANGED;
                ret_val = icf_sm_handle_lic_error(p_glb_pdb, 
                                        p_sm_data, ICF_TRUE);
                ret_val = ICF_FAILURE;
            }
        } /* current timestamp obtained: verify counter
                data : processing ends */

    }/* ret_val != FAILURE: scenario 2: Processing Ends */

    

    if (ICF_FAILURE != ret_val)
    {
        /* ret_val != FAILURE: scenario 3: Processing Begins */

        /* Check for the last modified time of the license file */
        temp_timestamp = ICF_NULL;
        if (ICF_FAILURE == icf_sm_get_file_modify_time(
                        p_sm_data->p_license_file_name,
                        &temp_timestamp))
        {
            ICF_PRINT(("\n[ICF]:[SM]:Unable to get the last modified \
                            time for the license file \n"));
            p_sm_data->detection_mask |=  ICF_SM_STAT_CALL_FAILED;
            ret_val = icf_sm_handle_lic_error(p_glb_pdb, 
                                p_sm_data, ICF_TRUE);
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Modification time obtained: verify the 
             * counter data: Processing Begins */

            if (temp_timestamp != counter_data.last_modified_time)
            {
                /* New License File detected: Processing Begins */
                ret_val = icf_sm_init_license(p_sm_data,
                                p_sm_pdb->p_ecode);
                if (ICF_FAILURE == ret_val)
                {
                    /* init_license() returned FAILURE */
                    ret_val = icf_sm_handle_lic_error(p_glb_pdb, 
                                p_sm_data, ICF_TRUE);
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* Start timer for new valid license
                     * and update the counter
                     * file accordingly: processing begins */
                    if (ICF_NULL != p_sm_data->expiry_time)
                    {
                        if(ICF_FAILURE == icf_sm_start_timer(p_glb_pdb,
                                            p_sm_data,p_sm_pdb->p_ecode,
                                            ICF_TRUE))
                        {
                            ret_val = icf_sm_handle_lic_error(p_glb_pdb,
                                            p_sm_data, ICF_TRUE);
                            ret_val = ICF_FAILURE;
                        }
                    }
                    else
                    {
                        /* No timer to be started */
                        p_sm_data->timer_id = ICF_NULL;
                    }
                    
                }/* Start timer for new valid license
                     * and update the counter
                     * file accordingly: processing ends */

            }/* New License File detected: Processing Ends */

            else
            {
                /* old license file: so timer to be restarted for
                 * next 24 hrs or whatever time is remaining;
                 * if the timer_id is NON_NULL in p_sm_data  */
                if (ICF_NULL != p_sm_data->timer_id)
                {
                    if(ICF_FAILURE == icf_sm_start_timer(p_glb_pdb,
                                            p_sm_data,p_sm_pdb->p_ecode,
                                            ICF_FALSE))
                        {
                            ret_val = icf_sm_handle_lic_error(p_glb_pdb,
                                            p_sm_data, ICF_TRUE);
                            ret_val = ICF_FAILURE;
                        }
                }/* timer was already running: update it */
                
            }
        
        }/* Modification time obtained: verify the 
          * counter data: Processing Ends */

    } /* ret_val != FAILURE: scenario 3: Processing Ends */

    
             
     return ret_val;

}/* Function icf_sm_timer_expiry_hdlr() ends here */
#endif

#ifdef ICF_STAT_ENABLE
/**********************************************************************
 * FUNCTION:        icf_sm_stats_hdlr
 * 
 * DESCRIPTION:     This function is called by the function 
 *                  icf_process_mesg whenever the
 *                  API ICF_STATS_REQ is 
 *                  received. This function reads the value of 
 *                  rest_stats that is received with the API and 
 *                  populates the response API structure with 
 *                  the appropriate values of stats.
 *
 * RETUENS:         ICF_SUCCESS in case of success and ICF_FAILURE
 *                  otherwise.Failure occurs when:
 *                  -Memory allocation for p_sm_msg fails.
 *                  -function icf_adm_process_msg  returned failure. 
 *********************************************************************/


icf_return_t icf_sm_stats_hdlr(
        INOUT    icf_sm_pdb_st *p_sm_pdb)
{

    icf_return_t ret_val = ICF_SUCCESS;

    icf_stats_resp_st *p_sm_msg_payload = ICF_NULL ;

    icf_stats_req_st *p_recd_msg_payload =  ICF_NULL;
    icf_glb_pdb_st *p_glb_pdb =  ICF_NULL;

    icf_msg_st *p_sm_msg = ICF_NULL ;

    icf_uint16_t api_len =  ICF_NULL;

    p_glb_pdb = p_sm_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /*To raise the trace for function entry.*/

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*The payload received in ICF_STATS_REQ API is extracted*/
    p_recd_msg_payload = (icf_stats_req_st *)
        (p_sm_pdb->p_api_payload);

    

    /* This function gives the value to reset flag as TRUE
     * if the recd_reset is any non-zero value. */

    ICF_SM_TRACE(p_sm_pdb,ICF_SM_TRACE_VALIDATE_PARAM)


   api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
        ICF_PORT_SIZEOF(icf_stats_resp_st);


   /* Memory allocation for p_sm_msg. */

    ICF_MEMGET(p_glb_pdb,api_len,ICF_MEM_COMMON,\
            p_sm_msg,ICF_RET_ON_FAILURE,\
            p_sm_pdb->p_ecode,ret_val)


   if (ICF_FAILURE != ret_val)
   {
            /* Successful memory allocation for p_sm_msg : processing begins. */

            /* Filling the header fields for the response */


            p_sm_msg->hdr.api_id = ICF_STATS_RESP;
            p_sm_msg->hdr.source_id = ICF_MODULE_ICF;
            p_sm_msg->hdr.destination_id = ICF_MODULE_PA;
            p_sm_msg->hdr.api_length =  
                ICF_PORT_SIZEOF(icf_api_header_st) +
                ICF_PORT_SIZEOF(icf_stats_resp_st);

            p_sm_msg->hdr.app_id = p_glb_pdb->p_recv_msg->hdr.app_id;
            p_sm_msg->hdr.call_id = p_glb_pdb->p_recv_msg->hdr.call_id;
            /* Filling the payload */

            p_sm_msg_payload = (icf_stats_resp_st *)
                (p_sm_msg->payload);

            p_sm_msg_payload->stats_data = *(p_glb_pdb->p_stats_info);

            if(ICF_TRUE == p_recd_msg_payload->reset_stats)
             {
               icf_port_get_current_timestamp(
                         &(p_glb_pdb->last_reset_time));
               icf_port_memset((p_glb_pdb->p_stats_info),0,
                                  ICF_PORT_SIZEOF(icf_stats_st));
	     }

            p_sm_msg_payload->last_reset_time = p_glb_pdb->last_reset_time;

            p_sm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_STATS_RESP;

            p_sm_pdb->p_internal_msg->msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_api_header_st) +
                       ICF_PORT_SIZEOF(icf_stats_resp_st);
            p_sm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t*)p_sm_msg;

            icf_adm_process_mesg(p_sm_pdb->p_internal_msg);

            
           }

         /* Free the memory allocated to the response message */
              ICF_MEMFREE(
                     p_sm_pdb->p_glb_pdb,
                     p_sm_msg,
                     ICF_MEM_COMMON,
                     p_sm_pdb->p_ecode,
                     ret_val)
       
 
    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;

} 


#endif
