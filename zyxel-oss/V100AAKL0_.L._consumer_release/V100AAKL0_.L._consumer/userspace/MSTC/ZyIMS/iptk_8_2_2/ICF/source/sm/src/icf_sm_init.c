/*******************************************************************************
 *
 * FILE NAME :      icf_sm_init.c
 *
 * DESCRIPTION:     This file contains the code to initialize
 *                  and de-initialize the global data of the SM module.
 *                                    
 * REVISION HISTORY: 
 * 
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 09-Mar-2005  Jyoti Narula    	ICF LLD	Initial
 * 27-Jul-2005  Sumit Gupta                     Replace icf_cmn_memget with 
 *                                              icf_cmn_static_memget in 
 *                                              function icf_sm_init as this 
 *                                              function is executed prior to 
 *                                              creation of pools
 * 14-Mar-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 06-Oct-2006  Aman Arora       ICF Rel 6.1  for enhanced Licensing    
 * 08-JUN-2007  Alok Tiwari      ICF Rel Early 7.0   Code added for stat
 * 08-Apr-2008  Tarun Gupta      SPR 18355      Merged Windows Porting Changes
 * 02-Jun-2009  Preksha          Rel8.2         Asynchronus MESSAGE Enh 
 *   
 * Copyright (c) 2008, Aricent.
 ******************************************************************************/

#include "icf_common_prototypes.h"
#include "icf_sm_int_types.h"
#include "icf_sm_defs.h"
#include "icf_sm_prototypes.h"
#include "icf_sm_macro.h"
#include "icf_macro.h"
#include "icf_common_defs.h"
#include "icf_port_defs.h"
#include "icf_port_prototypes.h"
#include "icf_dbm_prototypes.h"



/*************************************************************************
 * FUNCTION:        icf_sm_init
 *
 * DESCRIPTION:     This function allocates static memory to the gloal data 
 *                  for the SM module and it initializes the members
 *                  with default values and the function pointer list 
 *                  with API handlers.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.Failure will happen when Static memory 
 *                  for the sm_global data could not be allocated.
 *                  Incase Defined LICENSE_ENABLE: 
 *                  FAILURE: memory allocation failed.
 *                           license integrity check failed.
 *                           license invalid or expired.
 *                           file wriitng failed for counter file
 *                  SUCCESS: license valid and counter file 
 *                           maintained.         
 ***************************************************************************/

icf_return_t icf_sm_init(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        OUT     icf_void_t **p_p_glb_sm_data,
        OUT     icf_error_t *p_ecode)
{

    icf_return_t ret_val = ICF_SUCCESS;

    icf_sm_glb_data_st  *p_sm_data = ICF_NULL;

    icf_uint32_t data_len = ICF_NULL;

    /*  macros to record module entry are not used here
     *  because the SET_MODULE/ FUNCTION_ENTER
     *  macros instead TRACE macro
     * and cmn_trace uses the sm_global data
     * -------------------------------------*/


    /* Allocate static memory to sm_glb_data */
    data_len = (icf_uint32_t)ICF_PORT_SIZEOF(icf_sm_glb_data_st);
    p_sm_data = icf_cmn_static_memget(p_glb_pdb, data_len, p_ecode);

    if (ICF_NULL != p_sm_data)
    {
		/* these SM apis are currently not supported in ICF */ 
		/* From the code that has been compiled out, we still need to retain
		 * this pointer to SM global data */
		*p_p_glb_sm_data = (icf_void_t *)(p_sm_data);

        /* Static Memory successfully allocated to SM global data: 
         * processing begins*/

        /* Set values to the function pointer list. */

#ifdef ICF_TRACE_ENABLE
        p_sm_data->api_hdlr_list[ICF_SM_SET_TRACE_LVL] = 
            icf_sm_set_trace_hdlr;
        
	p_sm_data->api_hdlr_list[ICF_SM_SET_TRACE_REQ] = 
            icf_sm_set_trace_req_hdlr;
#endif
	
#ifdef ICF_ERROR_ENABLE
        p_sm_data->api_hdlr_list[ICF_SM_SET_ERROR_LVL] = 
            icf_sm_set_error_hdlr;
#endif
         p_sm_data->api_hdlr_list[ICF_SM_GET_STATS] = 
            icf_sm_get_stats_hdlr;

        p_sm_data->api_hdlr_list[ICF_SM_CHK_MODULE] = 
            icf_sm_chk_module_status_hdlr;
#ifdef ICF_STAT_ENABLED
        p_sm_data->api_hdlr_list[ICF_SM_CALL_LOG] = 
            icf_sm_call_log_hdlr;
#endif

#ifdef ICF_STAT_ENABLE
	p_sm_data->api_hdlr_list[ICF_SM_STATS] =
		 icf_sm_stats_hdlr;
#endif

#ifdef ICF_REL_OLD
        p_sm_data->api_hdlr_list[ICF_SM_TIMER_EXPIRY] = 
                icf_sm_timer_expiry_hdlr;   
#endif

        /* Initializing members with default values.
         * The compile time flags used here are ICF_TRACE_ENABLE, 
         * as used in the definition of the macro ICF_TRACE. */

#ifdef ICF_ERROR_ENABLE
        p_sm_data->error_status =   ICF_TRUE;
        p_sm_data->error_level  =   ICF_ERROR_LEVEL_INFO;
        p_sm_data->error_type   =   0|
            ICF_ERROR_TYPE_SYSTEM|
            ICF_ERROR_TYPE_INTERFACE|
            ICF_ERROR_TYPE_SIP|
            ICF_ERROR_TYPE_CALL_CONTROL|
            ICF_ERROR_TYPE_MEDIA|
            ICF_ERROR_TYPE_CONFIG|
            ICF_ERROR_TYPE_REGISTRATION|
            ICF_ERROR_TYPE_SM|
            ICF_ERROR_TYPE_CALL_HOLD_RESUME|
            ICF_ERROR_TYPE_THREE_WAY_CALL|
            ICF_ERROR_TYPE_CALL_TRANSFER_UNATTENDED|
            ICF_ERROR_TYPE_CALL_TRANSFER_ATTENDED|
            ICF_ERROR_TYPE_REFER|
            ICF_ERROR_TYPE_CALL_FORWARDING|
            ICF_ERROR_TYPE_CALL_WAITING|
            ICF_ERROR_TYPE_GENERAL;
#endif

#ifdef ICF_TRACE_ENABLE
#ifdef ICF_TRACE_INIT_TIME_ENABLE
        p_sm_data->trace_status =   ICF_TRUE;
        p_sm_data->trace_level  =   ICF_TRACE_LEVEL_DETAILED
                                    | ICF_TRACE_LEVEL_BRIEF;
        p_sm_data->trace_type   =   0|
            ICF_TRACE_TYPE_SYSTEM|
            ICF_TRACE_TYPE_INTERFACE|
            ICF_TRACE_TYPE_SIP|
            ICF_TRACE_TYPE_CALL_CONTROL|
            ICF_TRACE_TYPE_MESSAGE_CONTROL|
            ICF_TRACE_TYPE_MEDIA|
            ICF_TRACE_TYPE_CONFIG|
            ICF_TRACE_TYPE_REGISTRATION|
            ICF_TRACE_TYPE_SM|
            ICF_TRACE_TYPE_CALL_HOLD_RESUME|
            ICF_TRACE_TYPE_THREE_WAY_CALL|
            ICF_TRACE_TYPE_CALL_TRANSFER_UNATTENDED|
            ICF_TRACE_TYPE_CALL_TRANSFER_ATTENDED|
            ICF_TRACE_TYPE_REFER|
            ICF_TRACE_TYPE_CALL_FORWARDING|
            ICF_TRACE_TYPE_CALL_WAITING|ICF_TRACE_TYPE_SIP_STACK |
            ICF_TRACE_TYPE_GENERAL;


#else
        p_sm_data->trace_status =   ICF_TRUE;
        p_sm_data->trace_level  =   ICF_TRACE_LEVEL_UNDEFINED;
        p_sm_data->trace_type   =   ICF_TRACE_TYPE_UNDEFINED;

#endif/* TRACE_INIT_TIME_ENABLE */

/*#else */
        p_sm_data->trace_status =   ICF_TRUE;
        p_sm_data->trace_level  =   ICF_TRACE_LEVEL_UNDEFINED;
        p_sm_data->trace_type   =   ICF_TRACE_TYPE_UNDEFINED|
                                    ICF_TRACE_TYPE_SYSTEM|
                                    ICF_TRACE_TYPE_SIP_INTERFACE|
                                    ICF_TRACE_TYPE_APP_INTERFACE|
                                    ICF_TRACE_TYPE_MEDIA_INTERFACE|
                                    ICF_TRACE_TYPE_CALL_CONTROL|
                                    ICF_TRACE_TYPE_MESSAGE_CONTROL|
                                    ICF_TRACE_TYPE_CONFIGURATION|
                                    ICF_TRACE_TYPE_SUBSCRIPTION|
                                    ICF_TRACE_TYPE_REGISTRATION|
                                    ICF_TRACE_TYPE_ERROR;

	p_sm_data->trace_criteria = ICF_TRACE_CRITERIA_UNDEFINED;
	p_sm_data->trace_rule   =   ICF_TRACE_RULE_OR;
       
#endif/* TRACE_ENABLE */

        /* Initializing the num_calls_to_log value. */
        p_sm_data->num_calls_to_log = 0;

        /* Initializing the call history pointers to NULL. */
        p_sm_data->p_dialed_calls  = ICF_NULL;
        p_sm_data->p_missed_calls  = ICF_NULL;
        p_sm_data->p_recd_calls    = ICF_NULL;       
        

        /*Initializing the count of logged calls to 0. */
        p_sm_data->num_dialed_calls  = 0;
        p_sm_data->num_recd_calls    = 0;
        p_sm_data->num_missed_calls  = 0;

        /*Initializing the moving index of the arrays to INVALID_INDEX.*/
        p_sm_data->dialed_calls_index  = ICF_SM_INVALID_INDEX;
        p_sm_data->recd_calls_index    = ICF_SM_INVALID_INDEX;
        p_sm_data->missed_calls_index  = ICF_SM_INVALID_INDEX;

        /* SM's global data pointer needs to be 
          * returned to the calling module 
         * This assignment has been shifted from
         * being the last statement to this point,
         * because during the license validation
         * checks, the errors/traces raised have to be reported
         * bask to external System Manager.*/

          *p_p_glb_sm_data = (icf_void_t *)(p_sm_data);


        /* Following statements to be needed for License Management */
    }/* Static Memory successfully allocated to SM global data:
        Processing Ends */

    else
    {

        /* Static memory could not be allocated for SM global data. */

        ret_val = ICF_FAILURE;
    }/* else ends here*/


    ICF_FUNCTION_EXIT(p_glb_pdb)

        /* For this was giving uninitialized memory read error while SI run
         *     ICF_RESET_MODULE(p_glb_pdb) */
    
    return ret_val;


}/*Function icf_sm_init_data() ends*/



/*******************************************************************************
 *
 * FUNCTION:        icf_sm_deinit
 *
 * DESCRIPTION:     This function is invoked at system termination and is used 
 *                  to free the memory allocated at init time.
 *
 ******************************************************************************/
icf_void_t icf_sm_deinit(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        OUT     icf_void_t               *p_data,
        OUT     icf_error_t              *p_ecode)
{
  
   icf_return_t ret_val = ICF_SUCCESS; 

     p_glb_pdb = p_glb_pdb;

    ICF_PRINT(((icf_uint8_t *)"\n[SM]: Entering function icf_sm_deinit()"));
    

    if (ICF_NULL != p_data) 
    {
#ifdef ICF_STAT_ENABLED
       /* Call deinit_call_history() to deinitialize the 
        * call history lists. */ 
        if (0 != ((icf_sm_glb_data_st *)p_data)->num_calls_to_log)
        {
        
            icf_sm_deinit_call_history(p_glb_pdb, p_ecode);
        }
#endif
        /* close the File descriptor of counter file if
         * LICENSE enabled  Also deallocate memory given to
         * license_file_name array. */
        
       /* Call icf memory free function */ 
       ret_val = icf_cmn_static_memfree(p_glb_pdb, p_data, p_ecode); 
    } 
      p_data = ICF_NULL; 
     
      ICF_PRINT(((icf_uint8_t *)"\n[SM]: Exiting Function icf_sm_deinit()"));
        return;
} /* End of icf_sm_deinit */


#ifdef ICF_STAT_ENABLED
/**************************************************************
 * FUNCTION:    icf_sm_init_call_history
 *
 * DESCRIPTION: This is a utility function invoked by the 
 *              CFG module to pass the num_calls_to_log 
 *              value to the SM.It also allocates the static 
 *              memory to the call history arrays in the SM
 *              Global data. It also makes the value of the 
 *              num_dialed/num_missed/num_recd as 0.
 *
 *
 * RETURNS:     ICF_SUCCESS if the value is correctly
 *              recorded in the SM global data.
 *              ICF_FAILURE 
 *                  -if the SM global data could 
 *                  not be retrieved successfully.
 *                  -if the memory could not be allocated 
 *                  for any of the 3 call history arrays.
 *************************************************************/

icf_return_t icf_sm_init_call_history(
        INOUT   icf_glb_pdb_st  *p_glb_pdb,
        IN      icf_uint8_t     num_calls_to_log)
{

    icf_sm_glb_data_st      *p_sm_data =  ICF_NULL;
    icf_uint16_t            array_len  =  ICF_NULL;   
    icf_error_t             ecode      =  ICF_NULL;

    /* Used to indicate the failure by DBM or MEMGET*/
#ifdef ICF_ERROR_ENABLE
    icf_error_data_st       error_data;
#endif

    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)


        ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_SM),
                (icf_void_t *)&p_sm_data,
                &(ecode));

    if (ICF_FAILURE != ret_val)
    {
    /*DBM returned the SM global data successfully: Processing Begins */

        /* Num_Calls_to_log initialized to 0.
         * Only after the static memory is successfully allocated,
         * then the num_calls_to_log is given the value is given
         * by the CFG. */
         p_sm_data->num_calls_to_log = 0;
         
        /*Initializing the count of logged calls to 0. */
        p_sm_data->num_dialed_calls  = 0;
        p_sm_data->num_recd_calls    = 0;
        p_sm_data->num_missed_calls  = 0;

        /*Initializing the moving index of the arrays to INVALID_INDEX*/
        p_sm_data->dialed_calls_index  = ICF_SM_INVALID_INDEX;
        p_sm_data->recd_calls_index    = ICF_SM_INVALID_INDEX;
        p_sm_data->missed_calls_index  = ICF_SM_INVALID_INDEX;


        if (0 != num_calls_to_log)
        {
            /* When num_calls_to_log is non-zero: Processing Begins */
        array_len    =  (num_calls_to_log*
                    ICF_PORT_SIZEOF(icf_sm_log_call_details_st));


        /* Memory allocation for dialed_call/missed_call/recd_call array. */

         p_sm_data->p_dialed_calls =
                 icf_cmn_static_memget(p_glb_pdb,(icf_uint32_t)array_len,
                                 &ecode); 

         if (ICF_NULL !=  p_sm_data->p_dialed_calls) 
         { 
             /* Dialed calls allocation Successful :Processing Begins*/

             p_sm_data->p_missed_calls =
                 icf_cmn_static_memget(p_glb_pdb,(icf_uint32_t)array_len,
                                 &ecode); 
             if (ICF_NULL !=  p_sm_data->p_missed_calls) 
             {
              /* Missed calls allocation Successful :Processing Begins*/

                 p_sm_data->p_recd_calls =
                 icf_cmn_static_memget(p_glb_pdb,(icf_uint32_t)array_len,
                                 &ecode); 
                 
                 if(ICF_NULL ==  p_sm_data->p_recd_calls) 
                 {
                     
                     ret_val = ICF_FAILURE; 
                     ICF_SET_ECODE(p_glb_pdb, ICF_NULL, 
                     ICF_ERROR_MEM_ALLOC_FAILURE, &ecode)       
                 }/* Received Calls Allocation Failure Handled.*/
                 
             }/* Missed calls allocation Successful :Processing ends*/

             else
             {
                 /* Missed Calls Allocation Failure.*/
                  ret_val = ICF_FAILURE; 
                     ICF_SET_ECODE(p_glb_pdb, ICF_NULL, 
                     ICF_ERROR_MEM_ALLOC_FAILURE, &ecode)      
             }
             
         }/* Dialed calls allocation Successful :Processing Ends*/

         else
         {
             /* Dialed Calls Allocation Failure */
             ret_val = ICF_FAILURE; 
             ICF_SET_ECODE(p_glb_pdb, ICF_NULL, 
                     ICF_ERROR_MEM_ALLOC_FAILURE, &ecode)      

         }

        }/* When num_calls_to_log is non-zero: Processing Ends */

       
         if (ICF_FAILURE != ret_val)
         {   
            /* Putting the value of num_calls_to_log
             * ret_val is checked for FAILURE, to
             * detect any memory failure incase the
             * num_calls_to_log was non zero */

             p_sm_data->num_calls_to_log = num_calls_to_log;

             /* To indicate the recording of num_calls_to_log value.*/
     
             ICF_SM_NON_API_TRACE(p_glb_pdb,
                p_sm_data,ICF_SM_TRACE_NUM_CALLS_RECORD)

         }/* The num_calls_to_log in SM Global
             data is given value as passed as parameter
             in the method call. */

        
    }/* DBM returned the SM global data successfully: Processing Ends */

    else
    {
        /* Raise an alarm in case of DBM failure. 
         * To indicate that the call Logging will be disabled.
         * Error data is being filled here because the SM_SET_ECODE
         * needs the sm_pdb as one argument, that is not availale here.
         * Also the num_calls_to_log is made 0
         * in the SM Global data, because this is the indication
         * that the call logging is disabled. */

        p_sm_data->num_calls_to_log = 0;
#ifdef ICF_ERROR_ENABLE
        error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
        error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#endif
        ICF_SET_ECODE(p_glb_pdb,&error_data,
                (icf_error_t)ICF_ERROR_CALL_LOGGING_FAILURE,
                &ecode)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
        


    return ret_val;


}/* Function icf_sm_init_call_history() ends here. */



/*******************************************************************************
 *
 * FUNCTION:        icf_sm_deinit_call_history
 *
 * DESCRIPTION:     This function is invoked to free
 *                  the static memory allocated to the call history arrays.
 *
 ******************************************************************************/
icf_void_t icf_sm_deinit_call_history(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        OUT     icf_error_t              *p_ecode)
{

    icf_return_t ret_val             =    ICF_SUCCESS;
    icf_sm_glb_data_st *p_sm_data    =    ICF_NULL;


    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_SM)
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_sm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_SM),
                (icf_void_t *)&p_sm_data);


    if (ICF_FAILURE !=  ret_val)
    {

        ICF_STATIC_MEMFREE(
                p_glb_pdb,
                p_sm_data->p_dialed_calls,
                p_ecode,
                ret_val);

    ICF_STATIC_MEMFREE(
                p_glb_pdb,
                p_sm_data->p_missed_calls,
                p_ecode,
                ret_val);

    ICF_STATIC_MEMFREE(
                p_glb_pdb,
                p_sm_data->p_recd_calls,
                p_ecode,
                ret_val);


        /* Setting the num_calls_to_log value as 0. */
        p_sm_data->num_calls_to_log = 0;


     /* Setting the count of logged calls to 0. */
        p_sm_data->num_dialed_calls  = 0;
        p_sm_data->num_recd_calls    = 0;
        p_sm_data->num_missed_calls  = 0;

        /* Setting the moving index of the arrays to INVALID_INDEX.*/
        p_sm_data->dialed_calls_index  = ICF_SM_INVALID_INDEX;
        p_sm_data->recd_calls_index    = ICF_SM_INVALID_INDEX;
        p_sm_data->missed_calls_index  = ICF_SM_INVALID_INDEX;
    }

    

        ICF_FUNCTION_EXIT(p_glb_pdb)
        ICF_RESET_MODULE(p_glb_pdb)

        return;
} /* End of icf_sm_deinit_call_history */
#endif
