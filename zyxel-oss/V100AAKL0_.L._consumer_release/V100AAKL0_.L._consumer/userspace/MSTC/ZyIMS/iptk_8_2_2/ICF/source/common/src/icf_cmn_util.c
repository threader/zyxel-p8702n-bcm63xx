/*******************************************************************************
 *
 * DESCRIPTION      :
 *
 * Revision History :
 *
 * DATE         NAME                REFERENCE       REASON
 * -----------  -------------       ------------    --------------
 * 04-Jan-2005  Aashish Suchdev                     Definition for cmn memory
 *                                                  utils
 * 07-Jan-2005  Aashish Suchdev                     Updated the interface for
 *                                                  trace/error logging
 * 10-Jan-2005  Aashish Suchdev                     Logging interface updated
 * 21-Feb-2005  Amit Sharma                         Added a function to send common
 *                                                  response 
 * 12-Apr-2005  Jyoti Narula                        Changes in cmn_trace()
 *                                                  and raise_error_n_trace()
 * 29-May-2005   Amit Sharma       SPR:7623         p_ecode is assigned
 * 10-Aug-2005	Rohit Aggarwal		ICF		        Added new functions
 *
 * 12-Dec-2005  Ashutos Rath		ICF 4.0			XML parser related functions added
 *
 * 28-Dec-2005	Jyoti Narula		SPR 9067		Proper test condition for parse fail detection
 * 06-Jan-2006 Jyoti Narula			SPR 9124		change in get_network_status
 * 02-Feb-2006  Amit Sharma                         CFG merged with ICF 2.0
 * 30-Mar-2006  Deepti Goyal            ICF_5_0     Update the ippstk_cmn_clone_
 *                                                  body_list function to copy
 *                                                  the body headers, if any.
 * 09-Jul-2007  Neha Choraria           ICF 7.0     Changes Related to Multiple Answer & Forking
 * 11-Jul-2007  Neha Choraria           ICF 7.0     Added icf_cmn_generate_timer
 *                                                  for changes in Retry-After 
 * 7-Aug-2007   Amit Mahajan                    Klocwork warning removal
 * 10-Dec-2007  Jagmohan Chauhan        SPR 17657  Change in icf_cmn_validate_trnspt_addr
 * 18-Dec-2007  Abhishek Dhammawat      SPR17811   Modified
 *                                                 icf_cmn_capture_app_trace.
 * 15-Jan-2008  Shruti Thakral                     Changes for secondary server
 *                                                 support in Rel 8.0
 * 24-Jan-2008  Tarun Gupta          IPTK Rel 8.0 LLD Added functions for AKA handling 
 * 11-Mar-2008  Divij Agarwal       SPR 18122       Suport for async AKA negotiation
 * 3-Apr-2008   Sumant Gupta        ICF 8.0         Rel7.1 Windows porting Merging
 * 11-May-2008 Abhishek Dhammawat SPR 18535     Modified 
 *                                              icf_cmn_compare_transport_addresses
 * 30-Jun-2008  Tarun Gupta       SPR 18579     Modified icf_cmn_copy_transport_address
 * 04-Nov-2008  Tarun Gupta       SPR 19189     Merged Rel 7.1.2 SPR 18880
 * 18-Nov-2008  Rajiv Kumar       SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                              18997)
 * 25-Nov-2008 Abhishek Dhammawat SPR 19189     Merging of SPR 18882 cycle3
 * 15-Dec-2008 Anurag Khare       SPR 19218     SPR 18901 Merged
 * 09-Feb-2009 Tarun Gupta        Rel 8.1 LLD   SDP Tunnelling Enhancements
 * 02-Mar-2009 Kamal Ashraf       Rel 8.1       Adding function icf_cmn_convert_ip_str_
 *                                              to_transport_addr for open source
 *                                              replacement
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349       Klocworks fix
 * 05-Mar-2009 Tarun Gupta        Rel 8.1       Modified icf_cmn_send
 * 14-Mar-2009 Alok Tiwari        IPTK Rel8.1   Modified function icf_cmn_send
 *                                              to fix the crash if 
 *                                              p_glb_port_info is NULL.
 * 31-Mar-2009 Anurag Khare       Rel 8.2       changes done for PRACK Enh
 * 31-Mar-2009 Alok Tiwari        IPTK Rel 8.2  Modified Function:
 *                                              icf_cmn_process_aka_challenge
 * 01-Apr-2009 Kamal Ashraf       Rel 8.2        Klocworks fix 
 * 18-Apr-2009 Tarun Gupta        SPR 19478     Modified icf_cmn_process_aka_challenge
 * 23-Apr-2009 Abhishek Dhammawat SPR 19502     Moved icf_ssa_map_interface_reason_code
 *                                              as icf_cmn_map_interface_reason_code
 * 25-May-2009 Rajiv Kumar        SPR 19666     Load stats added for 302
 *                                              response.
 * 08-Jun-2009 Abhishek Dhammawat SPR 19590     Added icf_cmn_hdr_list_node
 * 11-Jun-2009 Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal
 * 19-Jun-2009 Abhishek Dhammawat SPR 19590     Modified
 *                                              icf_cmn_make_hdr_list_node
 * 28-Jul-2009 Rajiv Kumar        SPR 19886     Fix merged for
 *                                              CSR-1-7302347
 * 20-Aug-2009 Anuradha Gupta     SPR 19927     GCC Warning fix
 * 29-May-2010 Alok Tiwari        SPR 20412     Merged CSR 1-8223807
 *
 * 
 * Copyright (c) 2009, Aricent.
 ******************************************************************************/

#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_port_macros.h"
#include "icf_port_defs.h"
#include "icf_port.h"
#include "icf_macro.h"
#include "icf_sm_prototypes.h"
#include "icf_dbm_types.h"
#include "icf_dbm_prototypes.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif

#ifdef ICF_LOAD_STAT
extern icf_uint32_t g_create_call_req;
#endif

/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_memget
*
*  DESCRIPTION    :   This function is called to allocate buffers in the system.
*  Arguments      :   size      - Size of memory buffer required
*                     pool_id   - Pool from which memory is required
*                     p_eocde   - pointer to variable to return error code
*
*  RETURNS        :   Pointer to the allocated buffer if there is available 
*                     memory in the system, else ICF_NULL with ecode as 
*                     ICF_OUT_OF_MEM.
*
 *************************************************************************/

icf_void_t* 
icf_cmn_memget(
        INOUT  icf_glb_pdb_st    *p_glb_pdb,
        IN  icf_uint32_t         size,
        IN  icf_pool_id_t        pool_id,
        OUT icf_error_t          *p_ecode )
{
    icf_void_t  *p_buff = ICF_NULL;
    p_buff = icf_port_memget(p_glb_pdb->p_glb_data->p_glb_port_info,
           size, pool_id, p_ecode); 
   if (ICF_NULL == p_buff) 
   {
       ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEMGET_ERROR,ICF_NULL)
 
#ifdef ICF_STAT_ENABLED
       ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_ALLOC_FAIL) 
#endif
       ICF_SET_ECODE(p_glb_pdb, ICF_NULL, ICF_ERROR_MEM_ALLOC_FAILURE,
                                                p_ecode)     
   } 
   else 
   {
#ifdef ICF_STAT_ENABLED	   
       ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_ALLOC_SUCC) 
       ICF_STAT_COUNT(p_glb_pdb, ICF_STAT_BYTES_OF_MEM_ALLOCATED, size);
#endif
   }
   return p_buff;   
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_memfree
*
*  DESCRIPTION    :   This function is called to deallocate buffers.
*  RETURNS        :   void.
*
 *************************************************************************/

icf_return_t
icf_cmn_memfree(
        INOUT  icf_glb_pdb_st        *p_glb_pdb,
        INOUT  icf_void_t*           p_buf,
        IN  icf_pool_id_t     pool_id,
        OUT icf_error_t    *p_ecode )
{
    icf_return_t ret_val = ICF_SUCCESS;
    if(ICF_FAILURE ==  icf_port_memfree(
          p_glb_pdb->p_glb_data->p_glb_port_info, p_buf, pool_id, p_ecode))
    {
       ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEMFREE_ERROR,ICF_NULL)

#ifdef ICF_STAT_ENABLED
        ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_DEALLOC_FAIL) 
#endif
        ICF_SET_ECODE(p_glb_pdb, ICF_NULL, 
                ICF_ERROR_MEM_FREE_FAILURE, p_ecode)
        ret_val = ICF_FAILURE;
    }
    else
    {
#ifdef ICF_STAT_ENABLED
         ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_DEALLOC_SUCC) 
#endif
    }
    return ret_val;
               
}



/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_static_memget
*
*  DESCRIPTION    :   This function is called to allocate static buffers
*                     in the system.
*
*
*  RETURNS        :   Pointer to the allocated buffer if there is available
*                     memory in the system, else ICF_NULL with ecode as
*                     ICF_OUT_OF_MEM.
*
 *************************************************************************/

icf_void_t*
icf_cmn_static_memget(
        INOUT  icf_glb_pdb_st   *p_glb_pdb,
        IN  icf_uint32_t          size,
        OUT icf_error_t    *p_ecode )
{
    icf_void_t*  p_buff = ICF_NULL;
    p_buff = icf_port_static_memget( size, p_ecode);
    
    if(ICF_NULL != p_glb_pdb)
    {
	    if (ICF_NULL == p_buff)
	    {
#ifdef ICF_STAT_ENABLED
		    ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_ALLOC_FAIL) 
#endif
			    ICF_SET_ECODE(p_glb_pdb, ICF_NULL,
					    ICF_ERROR_MEM_ALLOC_FAILURE, p_ecode)
	    }
	    else 
	    { 
#ifdef ICF_STAT_ENABLED
		    ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_ALLOC_SUCC) 
			    ICF_STAT_COUNT(p_glb_pdb,ICF_STAT_BYTES_OF_MEM_ALLOCATED, size);
#endif
	    }        
    }    
    return p_buff;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_static_memfree
*
*  DESCRIPTION    :   This function is called to free static buffers
*                     in the system.
*
*
*  RETURNS        :   ICF_SUCCESS/ICF_FAILURE
*
 *************************************************************************/

icf_return_t
icf_cmn_static_memfree(
        INOUT  icf_glb_pdb_st   *p_glb_pdb,
        IN  icf_void_t     *p_buf,
        OUT icf_error_t    *p_ecode )
{
    icf_return_t  ret_val = ICF_SUCCESS;
    p_glb_pdb=p_glb_pdb;
    if(ICF_FAILURE == icf_port_static_memfree(p_buf, p_ecode))
    {
#ifdef ICF_STAT_ENABLED
        ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_DEALLOC_FAIL) 
#endif
        ICF_SET_ECODE(p_glb_pdb, ICF_NULL, ICF_ERROR_MEM_FREE_FAILURE, 
                                         p_ecode)
        ret_val = ICF_FAILURE;
    }
    else
    { 
#ifdef ICF_STAT_ENABLED
        ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MEM_DEALLOC_SUCC) 
#endif
     } 
    return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_cmn_start_timer
 *
 *  DESCRIPTION     :    This function starts the ICF timer through a 
 *                      call to port interface
 *
 *  RETURNS         :    ICF_SUCCESS if successful else ICF_FAILURE 
 *************************************************************************/

icf_return_t 
icf_cmn_start_timer(
 INOUT    icf_glb_pdb_st	   *p_glb_pdb,
 IN    icf_timer_data_st    *p_buff,
 IN	   icf_uint32_t         duration,
 OUT   icf_timer_id_t       *p_timer_id,
 OUT   icf_error_t  	       *p_ecode)
{
    icf_return_t  ret_val = ICF_SUCCESS;
    
    /* SPR 18880 : Added NULL checks */
    if((ICF_NULL == p_glb_pdb) ||
       (ICF_NULL == p_glb_pdb->p_glb_data) ||
       (ICF_NULL == p_glb_pdb->p_glb_data->p_glb_port_info) ||
       (ICF_NULL == p_buff))
    {
        /* Return failure as not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t*)"[CMN] : p_glb_pdb or p_glb_data or p_glb_port_info is NULL\n"))
        return ICF_FAILURE;
    }

    if(ICF_FAILURE == icf_port_start_timer(
         p_glb_pdb->p_glb_data->p_glb_port_info,
             (icf_timer_data_st *)p_buff, duration,  p_timer_id, p_ecode))
    {
          ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_START_TIMER_ERROR,ICF_NULL)

          ICF_SET_ECODE(p_glb_pdb, ICF_NULL,
                  ICF_ERROR_TMR_START_FAILURE, p_ecode) 
          ret_val = ICF_FAILURE;
    }

#ifdef ICF_STAT_ENABLE    
    if(ICF_FAILURE != ret_val)
    {
       /* SPR 18880 : Added NULL checks */
       if(ICF_NULL != p_glb_pdb->p_stats_info)
       {
           ICF_STATS(p_glb_pdb->p_stats_info->misc_stats.
                                           stats_active_timers)
       }
    }
#endif

    return ret_val;

}

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_cmn_stop_timer
 *
 *  DESCRIPTION     :    This function is the port function for stopping 
 *                       a started timer. It has checks for timers not
 *                       started.
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
 *************************************************************************/
icf_return_t
icf_cmn_stop_timer(
        INOUT       icf_glb_pdb_st   *p_glb_pdb,
        INOUT       icf_timer_id_t   timer_id,
        OUT         icf_error_t      *p_ecode)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    /* SPR 18880 : Added NULL checks */
    if((ICF_NULL == p_glb_pdb) ||
       (ICF_NULL == p_glb_pdb->p_glb_data) ||
       (ICF_NULL == p_glb_pdb->p_glb_data->p_glb_port_info))
    {
        /* Return failure as not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t*)"[CMN] : p_glb_pdb or p_glb_data or p_glb_port_info is NULL\n"))
        return ICF_FAILURE;
    }
    
    if(ICF_FAILURE == icf_port_stop_timer(
           p_glb_pdb->p_glb_data->p_glb_port_info, timer_id, p_ecode))
        {

          ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_STOP_TIMER_ERROR,ICF_NULL)
          ICF_SET_ECODE(p_glb_pdb, ICF_NULL,
                  ICF_ERROR_TMR_STOP_FAILURE, p_ecode) 
          ret_val = ICF_FAILURE;
    }

#ifdef ICF_STAT_ENABLE
   if(ICF_FAILURE != ret_val)
   {
     /* SPR 18880 : Added NULL checks */
       if(ICF_NULL != p_glb_pdb->p_stats_info)
       {
           (p_glb_pdb->p_stats_info->misc_stats.stats_active_timers)--;
       }
   }
#endif

    return ret_val;
}


#ifdef ICF_ERROR_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_raise_error_n_trace
*
*  DESCRIPTION    :   This routine checks the error level and error type 
*                     passed against the ones configured by SM.  If it is 
*                     allowed this function will do the necessary oprn 
*                     to log the error/alarm with the SM, otherwise it 
*                     will just print("raise trace") the error string
*
*  RETURNS        :     
*
 *************************************************************************/

void icf_cmn_raise_error_n_trace(
        INOUT icf_glb_pdb_st *p_glb_pdb, 
        INOUT icf_error_t *p_ecode,
        IN icf_error_t error_id,
        INOUT icf_error_data_st *p_error_data)
{
#ifdef ICF_ERROR_ENABLE
    icf_error_internal_data_st   *p_intrnl_error_data =  ICF_NULL;
    icf_return_t error_reporting = ICF_FAILURE ;
    icf_return_t report_status = ICF_FAILURE;
    icf_uint8_t *p_error_string = ICF_NULL;



    if (ICF_NULL != p_glb_pdb->p_glb_data)
    {
        /* Incase the p_glb_data is not NULL : Processing Begins*/

    error_reporting = icf_sm_check_error(p_glb_pdb, 
                        (icf_error_id_t)error_id);

    *p_ecode = error_id;

         if (ICF_SUCCESS == error_reporting)
         {
           /* The error_id has been found to be a valid error_id.
            * The error_status in global data is TRUE, hence error reporting
            * is enabled.
            * Also the associated type and level of error_id falls
            * within valid range as specified in global data. 
            * FAILURE is received if any of the above conditions
            * are not satisfied or the Global SM data could not
            * be retrieved.*/


           /* Sending the error indication to SM */
           report_status = icf_sm_report_error(p_glb_pdb,
                                (icf_error_id_t)error_id,
                                p_error_data,
                                (icf_error_t *)p_ecode);


           p_ecode = p_ecode;
    
          }/* Error reporting completed*/
         if(error_id < ICF_MAX_NUM_OF_ERROR_IDS)
         {
             p_intrnl_error_data = &(p_glb_pdb->p_glb_data->error_data[error_id]);
             p_error_string = p_intrnl_error_data->p_error_string;
         }
         else
         {
             
             if((ICF_NULL != p_error_data) &&
                (p_error_data->number_of_string_values >0 ) &&
                (p_error_data->err_string.str_len > 0))
             {
                 p_error_string = (p_error_data->err_string.str);
             }

         }
          /* Sending the error to port for printing. */
          if ( p_glb_pdb->curr_module_index < (ICF_MAX_MODULE_TVSL+1)) {
		  	icf_port_log_error(
               p_glb_pdb->p_glb_data->p_glb_port_info, 
               p_error_string,
               error_id,
               p_glb_pdb->module_hist[p_glb_pdb->curr_module_index],
               p_error_data);
    	}
    }/* Incase the p_glb_data is not NULL : Processing Ends*/



#else
    p_glb_pdb = p_glb_pdb;
#endif
}
#endif /* #ifdef ICF_ERROR_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_trace
*
*  DESCRIPTION    :    
*
*  RETURNS        :     
*
*************************************************************************/
void icf_cmn_trace( 
        INOUT icf_glb_pdb_st *p_glb_pdb, 
        IN    icf_trace_id_t  trace_id, 
        IN    icf_trace_data_st *p_trace_data)
{
#ifdef ICF_TRACE_ENABLE
    icf_trace_internal_data_st   *p_intrnl_trace_data; 
    icf_return_t trace_reporting = ICF_FAILURE ;
    icf_return_t report_status = ICF_FAILURE;

    if ( p_glb_pdb && (ICF_NULL != p_glb_pdb->p_glb_data ))
    {
        /* When p_glb_data is not NULL : Processing Begins */

	p_intrnl_trace_data = &(p_glb_pdb->p_glb_data->trace_data[trace_id]);
    trace_reporting = icf_sm_check_trace(p_glb_pdb, trace_id);

        if (ICF_SUCCESS == trace_reporting)
        {
        /* The trace_id has been found to be a valid trace_id.
         * The trace_status in global data is TRUE, hence traces are enabled.
         * Also the associated type and level of trace falls
         * within valid range as specified in global data. 
         * FAILURE is received if any of the above conditions
         * are not satisfied or the Global SM data could not
         * be retrieved.*/

        /* Sending Trace indication to SM. */

            report_status = icf_sm_report_trace(p_glb_pdb,
                              trace_id,p_trace_data);


         }/* Trace reporting completed. */
        /* Sending the trace to port for printing. */

         if ( p_glb_pdb->curr_module_index < (ICF_MAX_MODULE_TVSL+1)) {
         icf_port_log_trace(
                p_glb_pdb->p_glb_data->p_glb_port_info,
                p_intrnl_trace_data->p_trace_string,
                trace_id,
                p_glb_pdb->module_hist[p_glb_pdb->curr_module_index],
                p_trace_data);
		}
    }/* When p_glb_data is not NULL : Processing Ends */


#else
    p_glb_pdb = p_glb_pdb;
#endif
}

#ifdef ICF_TRACE_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_capture_app_trace
*
*  DESCRIPTION    :
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t icf_cmn_capture_app_trace(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_trace_id_t  trace_id,
        IN    icf_msg_st      *p_msg_to_be_traced)
{

    icf_global_port_info_st          *p_glb_port_data =
           (icf_global_port_info_st *)p_glb_pdb->p_glb_data->p_glb_port_info;
    icf_trace_data_st                *p_trace_data = ICF_NULL;
     icf_return_t    ret_val = ICF_SUCCESS;
    icf_error_t     ecode = ICF_ERROR_NONE;
    icf_generic_string_st *p_msg = ICF_NULL;
    icf_trace_criteria_data_st    *p_criteria_data = ICF_NULL;

        ICF_MEMGET(p_glb_pdb,
                ICF_PORT_SIZEOF(icf_trace_data_st),
                ICF_MEM_COMMON,
                p_trace_data,
                ICF_RET_ON_FAILURE,
                &ecode,
                ret_val)
    switch(trace_id)
        {
            case ICF_TRACE_UDP_SEND_ERROR:
            case ICF_TRACE_TCP_SEND_ERROR:
            case ICF_TRACE_TLS_SEND_ERROR:
                p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
                p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_glb_port_data->icf_trace_ipc_info.local_port);
                p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_glb_port_data->icf_trace_ipc_info.remote_port);

                p_trace_data->number_of_string_values = ICF_TWO_STRING_PRESENT;
                icf_port_strcpy(p_trace_data->string_val[0].str,
                                        p_glb_port_data->icf_trace_ipc_info.local_ip);
                p_trace_data->string_val[0].str_len =
                   (icf_uint16_t)icf_port_strlen(p_trace_data->string_val[0].str);
                icf_port_strcpy(p_trace_data->string_val[1].str,
                                        p_glb_port_data->icf_trace_ipc_info.remote_ip);
                p_trace_data->string_val[1].str_len =
                    (icf_uint16_t)icf_port_strlen(p_trace_data->string_val[1].str);
             break;

         case ICF_TRACE_MEMGET_ERROR:
         case ICF_TRACE_MEMFREE_ERROR:
         case ICF_TRACE_START_TIMER_ERROR:
         case ICF_TRACE_STOP_TIMER_ERROR:

              p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
              p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
              break;

         case ICF_TRACE_APP_MESG:
              {
                  /* Null check handling similar to default-case */
                  if (ICF_NULL == p_msg_to_be_traced)
                  {
                      ICF_MEMFREE(p_glb_pdb, p_trace_data, ICF_MEM_COMMON, &ecode, ret_val)
                          return ICF_SUCCESS;
                  }
                     ICF_MEMGET(p_glb_pdb,
                     ICF_PORT_SIZEOF(icf_generic_string_st),
                     ICF_MEM_COMMON,
                     p_msg,
                     ICF_DONOT_RET_ON_FAILURE,
                     &ecode,
                     ret_val)
                 if (ICF_FAILURE == ret_val)
                 {
                   ICF_MEMFREE(p_glb_pdb, 
                             p_trace_data, 
                             ICF_MEM_COMMON, 
                             &ecode, 
                             ret_val)
                   return ICF_FAILURE; 
                 }
                 /* SPR17811: p_glb_pdb->p_call_ctx was set to null on
                  * fetching call ctx using DBM function because the call ctx
                  * is removed from hash table, after call release.
                  * However at this stage already we do have the call ctx so 
                  * no need to call the dbm function if call ctx is non
                  * null or present */
                 if((ICF_NULL != p_glb_pdb->p_call_ctx) || 
                         (ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                                    p_glb_pdb,
                                    p_msg_to_be_traced->hdr.call_id,
                                    &p_glb_pdb->p_call_ctx,
                                    &ecode)))
                 {
                    /* This check is  added for condition id after calling 
                       dbm function also we do not have call ctx then
                       set default values in app_trace_info and do not
                       use the call ctx */
                    if(ICF_NULL != p_glb_pdb->p_call_ctx)
                    {   
                       p_criteria_data = 
                           p_glb_pdb->p_call_ctx->p_trace_criteria_data;
                       p_glb_pdb->p_glb_data->app_trace_info.app_id =
                                         p_glb_pdb->p_call_ctx->app_id;
                    }
                    else
                    {
                       p_glb_pdb->p_glb_data->app_trace_info.app_id =
                           ICF_GENERIC_APP_ID;
		               p_glb_pdb->p_glb_data->app_trace_info.call_id =
                           ICF_INVALID_CALL_ID;
                    }  
                 }


                 /*CSR-5286119*/

                 if ((ICF_GENERIC_APP_ID != p_msg_to_be_traced->hdr.app_id) &&
			(ICF_INVALID_APP_ID != p_msg_to_be_traced->hdr.app_id))

		{
		    p_glb_pdb->p_glb_data->app_trace_info.app_id = 
			p_msg_to_be_traced->hdr.app_id;

		    p_glb_pdb->p_glb_data->app_trace_info.call_id = 
			p_msg_to_be_traced->hdr.call_id;
		}

                 p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;

                 p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

                 p_trace_data->p_var_data = p_msg;

                 p_msg->len = icf_port_strlen(p_glb_pdb->p_interface_trace_info);

                 p_msg->p_buff = p_glb_pdb->p_interface_trace_info;

                 break;
              }

         case ICF_TRACE_MEDIA_MESG:
               {
                  /* CSR-5286119*/
                  /* Null check handling similar to default-case */
		   if (ICF_NULL == p_msg_to_be_traced)
		   {
               	     ICF_MEMFREE(p_glb_pdb, p_trace_data, ICF_MEM_COMMON, &ecode, ret_val)
               	     return ICF_SUCCESS;
		   }

                 ICF_MEMGET(p_glb_pdb,
                     ICF_PORT_SIZEOF(icf_generic_string_st),
                     ICF_MEM_COMMON,
                     p_msg,
                     ICF_DONOT_RET_ON_FAILURE,
                     &ecode,
                     ret_val)

                 if (ICF_FAILURE == ret_val)
                 {
                   ICF_MEMFREE(p_glb_pdb, 
                             p_trace_data, 
                             ICF_MEM_COMMON, 
                             &ecode, 
                             ret_val)
                   return ICF_FAILURE; 
                 }

                 if (ICF_NULL != p_glb_pdb->p_call_ctx)
                 {
                     p_glb_pdb->p_glb_data->app_trace_info.app_id = 
                            p_glb_pdb->p_call_ctx->app_id;
                     p_glb_pdb->p_glb_data->app_trace_info.call_id = 
                            p_glb_pdb->p_call_ctx->call_id;
                 }
                 else if(ICF_SUCCESS == icf_dbm_find_call_cntxt_blk(
                                    p_glb_pdb,
                                    p_msg_to_be_traced->hdr.call_id,                                    
                                    &p_glb_pdb->p_call_ctx,
                                    &ecode))
                 {
                        p_criteria_data = p_glb_pdb->p_call_ctx->p_trace_criteria_data;
                        p_glb_pdb->p_glb_data->app_trace_info.app_id =
                            p_glb_pdb->p_call_ctx->app_id;

                        p_glb_pdb->p_glb_data->app_trace_info.call_id =
                           p_glb_pdb->p_msg_lst->p_msg->hdr.call_id;
                 }
                 
                 else if ((ICF_GENERIC_APP_ID != p_msg_to_be_traced->hdr.app_id) &&
			(ICF_INVALID_APP_ID != p_msg_to_be_traced->hdr.app_id))
		{

		     p_glb_pdb->p_glb_data->app_trace_info.app_id = 
				p_msg_to_be_traced->hdr.app_id;

                     p_glb_pdb->p_glb_data->app_trace_info.call_id = 
				p_msg_to_be_traced->hdr.call_id;
		 }         
                 p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;

                 p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

                 p_trace_data->p_var_data = p_msg;

                 p_msg->len = icf_port_strlen(p_glb_pdb->p_interface_trace_info);

                 p_msg->p_buff = p_glb_pdb->p_interface_trace_info;

                 break;
               }
         default:

               ICF_MEMFREE(p_glb_pdb, p_trace_data, ICF_MEM_COMMON, &ecode, ret_val)
               return ICF_SUCCESS;


         } /* end of switch-case for trace_id */

          ret_val = icf_cmn_check_app_trace(p_glb_pdb, trace_id,
		                       p_criteria_data);
	  if(ICF_FAILURE == ret_val)
	  {
	         /* CSR-5286119 */
                 if(ICF_NULL != p_msg)
                  ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON, &ecode, ret_val)
	
	       ICF_MEMFREE(p_glb_pdb, p_trace_data, ICF_MEM_COMMON, &ecode, ret_val)

               return ret_val;
	  }

           /* Finally, invoke the common macro for trace handling */
           if( ICF_NULL == p_criteria_data)
	   {
                ICF_APP_TRACE(p_glb_pdb, trace_id, p_trace_data,
                        ICF_NULL)
	   }
	   else
	   {
		ICF_APP_TRACE(p_glb_pdb, trace_id, p_trace_data,
                        p_criteria_data)
	   }

 
        if(ICF_NULL != p_msg)
            ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON, &ecode, ret_val)

        ICF_MEMFREE(p_glb_pdb, p_trace_data, ICF_MEM_COMMON, &ecode, ret_val)

    return ICF_SUCCESS;
} /* end of icf_cmn_capture_app_trace() */

#endif


/*************************************************************************
*
*  FUNCTION NAME  :   icf_cmn_app_trace
*
*  DESCRIPTION    :
*
*  RETURNS        :
*
*************************************************************************/
void icf_cmn_app_trace(
         icf_glb_pdb_st *p_glb_pdb,
         icf_trace_id_t  trace_id,
         icf_trace_data_st *p_trace_data,
         icf_trace_criteria_data_st *p_criteria_data)
{
#ifdef ICF_TRACE_ENABLE

    icf_return_t report_status = ICF_FAILURE;
    p_criteria_data = p_criteria_data;

    if (ICF_NULL != p_glb_pdb->p_glb_data )
    {
        /* When p_glb_data is not NULL : Processing Begins */

      /* CSR1-5249829 */
      /* trace_reporting = icf_cmn_check_app_trace(p_glb_pdb, trace_id,
                        p_criteria_data);

        if (ICF_SUCCESS == trace_reporting)
        {*/
        /* The trace_id has been found to be a valid trace_id.
         * The trace_status in global data is TRUE, hence traces are enabled.
         * Also the associated type and level of trace falls
         * within valid range as specified in global data.
         * FAILURE is received if any of the above conditions
         * are not satisfied or the Global SM data could not
         * be retrieved.*/

        /* Sending Trace indication to SM. */

           icf_port_get_current_timestamp(&(p_trace_data->trace_timestamp));
            report_status = icf_sm_report_app_trace(p_glb_pdb,
                              trace_id,p_trace_data);

         /* CSR1-5249829 */
         /*}*/ /* Trace reporting completed. */

    }/* When p_glb_data is not NULL : Processing Ends */


#else
    p_glb_pdb = p_glb_pdb;
    p_criteria_data = p_criteria_data;
#endif
}

#endif /* #ifdef ICF_TRACE_ENABLE */


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_resp_msg 
*
*  DESCRIPTION    :  This function is used to fill common response structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t     icf_cmn_fill_resp_msg
(
        INOUT        icf_glb_pdb_st        *p_glb_pdb,
        INOUT        icf_msg_st            *p_msg,
        IN           icf_module_id_t       dest_id,
        IN           icf_api_id_t          resp_api_id,
        IN           icf_result_t          result,
        IN           icf_error_t           error_cause
)
{

    /*API-ID to send */
    p_msg->hdr.api_id = resp_api_id;

    /*Source module is ICF*/
    p_msg->hdr.source_id = ICF_MODULE_ICF;

    /*Destination is the module to which response targetted*/
    p_msg->hdr.destination_id = dest_id;

    /*Payload length is size of api header + response structure*/
    p_msg->hdr.api_length = ICF_PORT_SIZEOF(icf_api_header_st) +
        ICF_PORT_SIZEOF(icf_msg_resp_st);

    if((ICF_NULL != p_glb_pdb->p_call_ctx))
    {
        /*Call ID from glb_pdb*/
        p_msg->hdr.call_id = p_glb_pdb->p_call_ctx->call_id;
        p_msg->hdr.app_id = p_glb_pdb->p_call_ctx->app_id;
    }
    else if (ICF_NULL != p_glb_pdb->p_recv_msg)
    {
        p_msg->hdr.call_id = p_glb_pdb->p_recv_msg->hdr.call_id;	
        p_msg->hdr.app_id = p_glb_pdb->p_recv_msg->hdr.app_id;	
    }
    else
    {
        p_msg->hdr.call_id = ICF_INVALID_CALL_ID;
        p_msg->hdr.app_id = ICF_INVALID_APP_ID;
    }
    
    if ((ICF_CALL_HOLD_RESP== resp_api_id) ||
        (ICF_CALL_RESUME_RESP == resp_api_id))
    {
        /*Result of request received success or failure*/
        ((icf_api_resp_st *)&(p_msg->payload[0]))->result = result;
        
        /*error_cause, valid if result is failure*/
        ((icf_api_resp_st *)&(p_msg->payload[0]))->error_cause = error_cause;

    }
    else if (ICF_CALL_TRANSFER_ATTENDED_RESP== resp_api_id)
    {
        /*Result of request received success or failure*/
        ((icf_cta_resp_st *)&(p_msg->payload[0]))->result = result;
        
        /*error_cause, valid if result is failure*/
        ((icf_cta_resp_st *)&(p_msg->payload[0]))->error_cause = error_cause;
        
    }/* CTA: */
    else if (ICF_CALL_TRANSFER_UNATTENDED_RESP== resp_api_id)
    {
        /*Result of request received success or failure*/
        ((icf_ctu_resp_st *)&(p_msg->payload[0]))->result = result;
        
        /*error_cause, valid if result is failure*/
        ((icf_ctu_resp_st *)&(p_msg->payload[0]))->error_cause = error_cause;
        
    }/* CTU*/
    else if(ICF_CREATE_XCONF_RESP == resp_api_id)
    {
        /*Result of request received success or failure*/
        ((icf_create_ext_xconf_resp_st*)&(p_msg->payload[0]))->result = result;
        
        /*error_cause, valid if result is failure*/
        ((icf_create_ext_xconf_resp_st*)&(p_msg->payload[0]))->error_cause = 
            error_cause;
    }
    else if(ICF_ADD_XCONF_PARTY_RESP == resp_api_id) 
    {
        /*Result of request received success or failure*/
        ((icf_add_xconf_party_resp_st*)&(p_msg->payload[0]))->result = result;
        
        /*error_cause, valid if result is failure*/
        ((icf_add_xconf_party_resp_st*)&(p_msg->payload[0]))->error_cause = 
            error_cause;
    }
    else if(ICF_DELETE_XCONF_PARTY_RESP == resp_api_id)
     {
        /*Result of request received success or failure*/
        ((icf_delete_xconf_party_resp_st*)&(p_msg->payload[0]))->result = 
            result;
        
        /*error_cause, valid if result is failure*/
        ((icf_delete_xconf_party_resp_st*)&(p_msg->payload[0]))->error_code = 
            error_cause;
     }
    else if(ICF_CC_PA_RELEASE_XCONF_RESP == resp_api_id) 
    {
        /*Result of request received success or failure*/
        ((icf_release_xconf_resp_st*)&(p_msg->payload[0]))->result 
            = result;
        
        /*error_cause, valid if result is failure*/
        ((icf_release_xconf_resp_st*)&(p_msg->payload[0]))->error_code = 
            error_cause;
    }
    else
    {
        
        /*Result of request received success or failure*/
        ((icf_msg_resp_st *)&(p_msg->payload[0]))->result = result;
        
        /*error_cause, valid if result is failure*/
        ((icf_msg_resp_st *)&(p_msg->payload[0]))->error_cause = error_cause;
        
    }/* normal resp_msg_st */


} /* icf_cmn_fill_resp_msg */

/************************************************************************
 * NAME:          icf_cmn_send 
 * DESCRIPTION:   Function is used to send message to PA/RM/RTP
 * 
 ***********************************************************************/

icf_return_t icf_cmn_send(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        INOUT icf_msg_st *p_msg_data,
        OUT   icf_error_t *p_ecode)
{

    icf_return_t ret_val = ICF_SUCCESS;
#ifdef ICF_TRACE_ENABLE
    icf_global_port_info_st *p_glb_port_data = ICF_NULL;
    icf_config_data_st      *p_config_data = ICF_NULL;
    icf_uint8_t     p_transp_addr_str[100] = "";
	icf_return_t		ret_val_trace = ICF_SUCCESS;

    if((ICF_NULL == p_glb_pdb) ||
       (ICF_NULL == p_glb_pdb->p_glb_data) ||
       (ICF_NULL == p_glb_pdb->p_glb_data->p_glb_port_info))
    {
      ICF_PRINT(((icf_uint8_t *)"\n[Function:icf_cmn_send]:"
                   "p_glb_data or p_glb_port_info is NULL"))
      return  ICF_FAILURE;
    }
    p_glb_port_data =
        (icf_global_port_info_st *)p_glb_pdb->p_glb_data->p_glb_port_info;
#endif

#ifdef ICF_TRACE_ENABLE
   #ifdef ICF_NAT_TRAVERSAL
    if(ICF_MODULE_NAT != p_msg_data->hdr.destination_id)
    {
    #endif
       ret_val_trace = icf_cmn_trace_api_from_api_id(p_glb_pdb,p_msg_data,p_ecode);
    #ifdef ICF_NAT_TRAVERSAL
    }
    #endif
#endif
 
    if(ICF_SUCCESS == icf_send(p_glb_pdb->p_glb_data->p_glb_port_info,
            p_msg_data, p_ecode))
    { 
         ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MSG_SEND_SUCC) 
    } 
    else 
    { 
        ret_val = ICF_FAILURE;
#ifdef ICF_TRACE_ENABLE
        ret_val_trace = icf_dbm_get_module_glb_data(
                                        p_glb_pdb,
                                        (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                        (icf_void_t *)&p_config_data, p_ecode);
        if( ICF_SUCCESS == ret_val_trace)
        {
            p_glb_port_data->icf_trace_ipc_info.local_port =
                    p_config_data->self_ip_address.port_num;
            /*  Fix Merge for SPR 18901 */
            /* Rel 8.1 : SDP Tunnelling */
            /* The following code snippet wherein we are extracting remote port
               and remote IP address from the message payload is relevant
               only when we receive an incoming message from the network. Thus 
               a check for API ID ICF_NW_API_MSG_FROM_PEER has been put. 
               Extraction of remote port and remote IP in a generic way is incorrect.
               Consider the following scenario:
               MMI creates a DMS request and sends it to ES. ES fails to send the message.
               Flow reaches this point and p_msg_data->payload can be NULL. 
               Even if it is not NULL, extracting remote port and remote IP
               from MMI message is incorrect.
            */
            if(ICF_NW_API_MSG_FROM_PEER == p_msg_data->hdr.api_id)
            {
                p_glb_port_data->icf_trace_ipc_info.remote_port =
                    ((icf_nw_interface_info_st*)(p_msg_data->payload))
                                                 ->remote_port_num;
 
                icf_port_strcpy(p_glb_port_data->icf_trace_ipc_info.remote_ip,
                    ((icf_nw_interface_info_st*)
                               (p_msg_data->payload))->remote_ip_addr);
            }

            if( ICF_TRANSPORT_ADDRESS_DOMAIN_NAME
                       == p_config_data->self_ip_address.addr.addr_type)
            {
                icf_port_strcpy(p_glb_port_data->icf_trace_ipc_info.local_ip,
                                        p_config_data->self_ip_address.addr.addr.domain.str);
            }
            else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR
                       == p_config_data->self_ip_address.addr.addr_type)
            {
                icf_cmn_convert_transport_addr_to_ip_string(
                              &(p_config_data->self_ip_address.addr),
                               p_transp_addr_str);
                icf_port_strcpy(p_glb_port_data->icf_trace_ipc_info.local_ip,
                                        p_transp_addr_str);
            }
            else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR
                             == p_config_data->self_ip_address.addr.addr_type)
            {
                icf_port_strcpy(p_glb_port_data->icf_trace_ipc_info.local_ip,
                                p_config_data->self_ip_address.addr.addr.ipv6_addr.str);
            }

            if( ICF_TRANSPORT_TYPE_UDP == p_glb_port_data->icf_trace_ipc_info.self_mode)
            {
                /*CSR-5286119 */
                ICF_CMN_APP_TRACE(p_glb_pdb, ICF_TRACE_UDP_SEND_ERROR,p_msg_data)
            }
            else if( ICF_TRANSPORT_TYPE_TCP == p_glb_port_data->icf_trace_ipc_info.self_mode)
            {
                /*CSR-5286119 */
                ICF_CMN_APP_TRACE(p_glb_pdb, ICF_TRACE_TCP_SEND_ERROR,p_msg_data)
            }
            else if( ICF_TRANSPORT_TYPE_TLS == p_glb_port_data->icf_trace_ipc_info.self_mode)
            {
                /*CSR-5286119  */
                ICF_CMN_APP_TRACE(p_glb_pdb, ICF_TRACE_TLS_SEND_ERROR,p_msg_data)
            }
        }
#endif

         ICF_STAT(p_glb_pdb,ICF_STAT_NUM_OF_MSG_SEND_FAIL) 
#ifdef ICF_TRACE_ENABLE
         ret_val_trace = ICF_SUCCESS;
#endif
    }
    return ret_val;
}


#ifdef ICF_STAT_ENABLED
icf_void_t icf_stat_count(
        IN   icf_glb_pdb_st        *p_glb_pdb,
        IN   icf_stats_id_t stat_id,
        IN   icf_uint32_t count)
{ 
    if(ICF_NULL != p_glb_pdb && ICF_NULL != p_glb_pdb->p_glb_data) 
    {
       if(ICF_NULL != p_glb_pdb->p_glb_data->p_stats_data)        
        {
            ((icf_stat_glb_st*)(p_glb_pdb->p_glb_data->p_stats_data))->
              stat_counter_list[stat_id] += count;                       
           
            ICF_DEBUG_PRINT(("\nICF STAT ID: %d, count and value %d", 
                          stat_id , count, 
                  ((icf_stat_glb_st*)(p_glb_pdb->p_glb_data->p_stats_data))
                  ->stat_counter_list[stat_id]));
        }
    }
}
#endif


/* this function is added by vidyut for strtok_r*/
#if defined (ICF_PORT_WINDOWS) || defined (ICF_PORT_LINUX)

char * self_implemented_strtok_r(char *s, const char *delim, char **last)
 {
   char *spanp;
   int c, sc;
   char *tok;
 
   if (s == NULL && (s = *last) == NULL)
   {
     return NULL;
   }
 
   /*
    * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
    */
 cont:
   c = *s++;
   for (spanp = (char *)delim; (sc = *spanp++) != 0; )
   {
     if (c == sc)
     {
       goto cont;
     }
   }
 
   if (c == 0)  /* no non-delimiter characters */
   {
     *last = NULL;
     return NULL;
   }
   tok = s - 1;
 
   /*
    * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
    * Note that delim must have one NUL; we stop if we see that, too.
    */
   for (;;)
   {
     c = *s++;
     spanp = (char *)delim;
     do
     {
       if ((sc = *spanp++) == c)
       {
         if (c == 0)
         {
           s = NULL;
         }
         else
         {
           char *w = s - 1;
           *w = '\0';
         }
         *last = s;
         return tok;
       }
     }
     while (sc != 0);
   }
   /* NOTREACHED */
 }
#endif





ICF_SESSION_CONTROL_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_header_list
 *
 * DESCRIPTION:     This is a utility function that frees the header list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_header_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_header_list_st	*p_header_list,
		OUT		icf_error_t			*p_ecode)
{
    icf_return_t					ret_val = ICF_SUCCESS;
	icf_header_st					*p_hdr = ICF_NULL, *p_next = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Rel 8.2 Klocworks fix */    
    if (ICF_NULL == p_header_list)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
    }
    /* Rel 8.2 Klocworks fix end */
    
	p_hdr = p_header_list->hdr;
	
	/* free the elements in the list */
	while (ICF_NULL != p_hdr)
	{
		p_next = p_hdr->next;
        ICF_MEMFREE(p_glb_pdb, p_hdr, ICF_MEM_COMMON, p_ecode, ret_val)
		p_hdr = p_next;
	}

	/* reset the elements in the list */
	p_header_list->count = ICF_NULL;
	p_header_list->hdr = ICF_NULL;

	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;
} /* icf_cmn_free_header_list() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_tag_list
 *
 * DESCRIPTION:     This is a utility function that frees the tag list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_tag_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_tag_list_st		*p_tag_list,
		OUT		icf_error_t			*p_ecode)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_tag_st						*p_tag = ICF_NULL, *p_next = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)
    /* Rel 8.2 Klocworks fix */    
    if (ICF_NULL == p_tag_list)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
    }
    /* Rel 8.2 Klocworks fix end */
	p_tag = p_tag_list->tag;
	
	/* free the elements in the list */
	while (ICF_NULL != p_tag)
	{
		p_next = p_tag->next;
        ICF_MEMFREE(p_glb_pdb, p_tag, ICF_MEM_COMMON, p_ecode, ret_val)
		p_tag = p_next;
	}

	/* reset the elements in the list */
	p_tag_list->count = ICF_NULL;
	p_tag_list->tag = ICF_NULL;

	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;
} /* icf_cmn_free_tag_list() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_msg_body_list
 *
 * DESCRIPTION:     This is a utility function that frees the body list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_msg_body_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_msg_body_list_st	*p_body_list,
		OUT		icf_error_t			*p_ecode)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_msg_body_st					*p_body = ICF_NULL, *p_next = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)
    /* Rel 8.2 Klocworks fix */    
    if (ICF_NULL == p_body_list)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
    }
    /* Rel 8.2 Klocworks fix end */

    /*Message Body Header Changes*/
	p_body = p_body_list->body;
	
	/* free the elements in the list */
	while (ICF_NULL != p_body)
	{
		p_next = p_body->next;
               /* Free the message body header if any are present
		*/
		if(p_body->msg_body_hdr_list.count)
		{
			icf_cmn_free_header_list(p_glb_pdb,&(p_body->
					msg_body_hdr_list),p_ecode);
		}
		
		/* Free the memory allocated to the body value
		 */
		if (ICF_NULL != p_body->value)
		{
			ICF_MEMFREE(p_glb_pdb, p_body->value, ICF_MEM_COMMON, 
				p_ecode, ret_val)
		}
        ICF_MEMFREE(p_glb_pdb, p_body, ICF_MEM_COMMON, p_ecode, ret_val)
		p_body = p_next;
	}

	/* reset the elements in the list */
	p_body_list->count = ICF_NULL;
	p_body_list->body = ICF_NULL;

	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;
} /* icf_cmn_free_msg_body_list() */


#ifdef ICF_REL_OLD
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_stream_list
 *
 * DESCRIPTION:     This is a utility function that frees the stream list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_stream_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_stream_list_st	*p_stream_list,
		OUT		icf_error_t			*p_ecode)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_uint8_t						ctr = ICF_NULL;
	icf_uint8_t						count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)
	
	/* reset the elements in the list */
	for (ctr = 0; ctr < (p_stream_list->count); ctr++)
    {
        p_stream_list->stream[ctr].type = ICF_MEDIA_TYPE_INVALID;

		p_stream_list->stream[ctr].stream_id = ICF_STREAM_ID_INVALID;
		
		p_stream_list->stream[ctr].codec.str_len = ICF_NULL;
		icf_port_strcpy(p_stream_list->stream[ctr].codec.str,"\0");
		
		for (count = 0; count < (p_stream_list->stream[ctr].att_count); count++)
		{	
			p_stream_list->stream[ctr].attributes[count].str_len = 
				ICF_NULL;
			icf_port_strcpy(p_stream_list->stream[ctr].
				attributes[count].str, "\0");
		}
		p_stream_list->stream[ctr].att_count = ICF_NULL;
    }

	/* reset the count in the list */
	p_stream_list->count = ICF_NULL;

    (void*)p_ecode;

	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;
} /* icf_cmn_free_stream_list() */
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_header_list
 *
 * DESCRIPTION:     This function makes a copy of header list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_header_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
		IN	icf_header_list_st	*p_src_header_list,
                OUT	icf_header_list_st	*p_dst_header_list,
		OUT	icf_error_t		*p_ecode)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_header_st					*p_src_hdr = ICF_NULL;
	icf_header_st					*p_dst_hdr = ICF_NULL;
	icf_header_st					*p_dst_hdr_last = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	/* return if the source list is empty */
	if (ICF_NULL == p_src_header_list->count)
	{
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	}
	else if(ICF_NULL == p_src_header_list->hdr)
 	{
  		ICF_FUNCTION_EXIT(p_glb_pdb)
  		return ret_val;
 	}

	/* copy the size of the list */
	p_dst_header_list->count = p_src_header_list->count;

	/* copy all the elements of the received list */
	
	/* allocate memory for the first header in the list */
	ICF_MEMGET(p_glb_pdb,
		ICF_PORT_SIZEOF(icf_header_st),
		ICF_MEM_COMMON,
		p_dst_header_list->hdr,
		ICF_RET_ON_FAILURE,
		p_ecode,
		ret_val)

	/* copy the contents of first element in the list into call ctx */
	p_dst_header_list->hdr->hdr_name.str_len = 
		p_src_header_list->hdr->hdr_name.str_len;
	icf_port_strcpy(p_dst_header_list->hdr->hdr_name.str,
		p_src_header_list->hdr->hdr_name.str);

	p_dst_header_list->hdr->hdr_value.str_len = 
		p_src_header_list->hdr->hdr_value.str_len;
	icf_port_strcpy(p_dst_header_list->hdr->hdr_value.str,
		p_src_header_list->hdr->hdr_value.str);

	p_dst_header_list->hdr->next = ICF_NULL;
	
	p_src_hdr = p_src_header_list->hdr->next;
	p_dst_hdr = p_dst_header_list->hdr->next;
	p_dst_hdr_last = p_dst_header_list->hdr;

	while (ICF_NULL != p_src_hdr)
	{
		/* allocate memory for the next header in the list */
		ICF_MEMGET(p_glb_pdb,
			ICF_PORT_SIZEOF(icf_header_st),
			ICF_MEM_COMMON,
			p_dst_hdr,
			ICF_RET_ON_FAILURE,
			p_ecode,
			ret_val)

		p_dst_hdr->hdr_name.str_len = p_src_hdr->hdr_name.str_len;
		icf_port_strcpy(p_dst_hdr->hdr_name.str, p_src_hdr->hdr_name.str);

		p_dst_hdr->hdr_value.str_len = p_src_hdr->hdr_value.str_len;
		icf_port_strcpy(p_dst_hdr->hdr_value.str, p_src_hdr->hdr_value.str);
		
		p_dst_hdr->next = ICF_NULL;
		p_dst_hdr_last->next = p_dst_hdr;
		p_dst_hdr_last = p_dst_hdr;

		p_src_hdr = p_src_hdr->next;
		p_dst_hdr = p_dst_hdr->next;
	} /* end while */
	
	ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_cmn_clone_header_list() */


ICF_CRM_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_header_name_list
 *
 * DESCRIPTION:     This function makes a copy of header list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_header_name_list(
		INOUT		icf_glb_pdb_st		    *p_glb_pdb,
		IN		icf_header_name_list_st	*p_src_header_name_list,
        OUT		icf_header_name_list_st	*p_dst_header_name_list,
		OUT		icf_error_t	    		*p_ecode)
{
    icf_return_t						    ret_val = ICF_SUCCESS;
	icf_header_name_st					*p_src_hdr = ICF_NULL;
	icf_header_name_st					*p_dst_hdr = ICF_NULL;
	icf_header_name_st					*p_dst_hdr_last = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	/* return if the source list is empty */
	if (ICF_NULL == p_src_header_name_list->count)
	{
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	}

	/* copy the size of the list */
	p_dst_header_name_list->count = p_src_header_name_list->count;

	/* copy all the elements of the received list */
	
	/* allocate memory for the first header in the list */
	ICF_MEMGET(p_glb_pdb,
		ICF_PORT_SIZEOF(icf_header_name_st),
		ICF_MEM_COMMON,
		p_dst_header_name_list->hdr,
		ICF_RET_ON_FAILURE,
		p_ecode,
		ret_val)

	/* copy the contents of first element in the list into call ctx */
	p_dst_header_name_list->hdr->hdr_name.str_len = 
		p_src_header_name_list->hdr->hdr_name.str_len;
	icf_port_strcpy(p_dst_header_name_list->hdr->hdr_name.str,
		p_src_header_name_list->hdr->hdr_name.str);

	p_dst_header_name_list->hdr->next = ICF_NULL;
	

	p_src_hdr = p_src_header_name_list->hdr->next;
	p_dst_hdr = p_dst_header_name_list->hdr->next;
	p_dst_hdr_last = p_dst_header_name_list->hdr;

	while (ICF_NULL != p_src_hdr)
	{
		/* allocate memory for the next header in the list */
		ICF_MEMGET(p_glb_pdb,
			ICF_PORT_SIZEOF(icf_header_name_st),
			ICF_MEM_COMMON,
			p_dst_hdr,
			ICF_RET_ON_FAILURE,
			p_ecode,
			ret_val)

		p_dst_hdr->hdr_name.str_len = p_src_hdr->hdr_name.str_len;
		icf_port_strcpy(p_dst_hdr->hdr_name.str, p_src_hdr->hdr_name.str);

		p_dst_hdr->next = ICF_NULL;
		p_dst_hdr_last->next = p_dst_hdr;
		p_dst_hdr_last = p_dst_hdr;

		p_src_hdr = p_src_hdr->next;
		p_dst_hdr = p_dst_hdr->next;
	} /* end while */
	
	ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_cmn_clone_header_name_list() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_header_name_list
 *
 * DESCRIPTION:     This is a utility function that frees the header list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_header_name_list(
		INOUT	icf_glb_pdb_st   		*p_glb_pdb,
        INOUT	icf_header_name_list_st	*p_header_list,
		OUT		icf_error_t		    	*p_ecode)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_header_name_st					*p_hdr = ICF_NULL, *p_next = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	p_hdr = p_header_list->hdr;
	
	/* free the elements in the list */
	while (ICF_NULL != p_hdr)
	{
		p_next = p_hdr->next;
        ICF_MEMFREE(p_glb_pdb, p_hdr, ICF_MEM_COMMON, p_ecode, ret_val)
		p_hdr = p_next;
	}

	/* reset the elements in the list */
	p_header_list->count = ICF_NULL;
	p_header_list->hdr = ICF_NULL;

	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;
} /* icf_cmn_free_header_name_list() */

ICF_CRM_END


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_tag_list
 *
 * DESCRIPTION:     This function makes a copy of tag list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_tag_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN		icf_tag_list_st		*p_src_tag_list,
		OUT		icf_tag_list_st		*p_dst_tag_list,
		OUT		icf_error_t			*p_ecode)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_tag_st						*p_src_tag = ICF_NULL;
	icf_tag_st						*p_dst_tag = ICF_NULL;
	icf_tag_st						*p_dst_tag_last = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	/* return if the source list is empty */
	if (ICF_NULL == p_src_tag_list->count)
	{
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	}
	else if(ICF_NULL == p_src_tag_list->tag)
 	{
		ICF_FUNCTION_EXIT(p_glb_pdb)
  		return ret_val;
 	}

	/* copy the size of the list */
	p_dst_tag_list->count = p_src_tag_list->count;

	/* copy all the elements of the received list */
	
	/* allocate memory for the first element in the list */
	ICF_MEMGET(p_glb_pdb,
		ICF_PORT_SIZEOF(icf_tag_st),
		ICF_MEM_COMMON,
		p_dst_tag_list->tag,
		ICF_RET_ON_FAILURE,
		p_ecode,
		ret_val)

	/* copy the contents of first element in the list into call ctx */
	p_dst_tag_list->tag->tag_type = 
		p_src_tag_list->tag->tag_type;
		
	p_dst_tag_list->tag->hdr_name.str_len = 
		p_src_tag_list->tag->hdr_name.str_len;
	icf_port_strcpy(p_dst_tag_list->tag->hdr_name.str,
		p_src_tag_list->tag->hdr_name.str);

	p_dst_tag_list->tag->tag_value.str_len = 
		p_src_tag_list->tag->tag_value.str_len;
	icf_port_strcpy(p_dst_tag_list->tag->tag_value.str,
		p_src_tag_list->tag->tag_value.str);

	p_dst_tag_list->tag->next = ICF_NULL;
	
	p_src_tag = p_src_tag_list->tag->next;
	p_dst_tag = p_dst_tag_list->tag->next;
	p_dst_tag_last = p_dst_tag_list->tag;

	while (ICF_NULL != p_src_tag)
	{
		/* allocate memory for the next element in the list */
		ICF_MEMGET(p_glb_pdb,
			ICF_PORT_SIZEOF(icf_tag_st),
			ICF_MEM_COMMON,
			p_dst_tag,
			ICF_RET_ON_FAILURE,
			p_ecode,
			ret_val)

		p_dst_tag->tag_type = p_src_tag->tag_type;

		p_dst_tag->hdr_name.str_len = p_src_tag->hdr_name.str_len;
		icf_port_strcpy(p_dst_tag->hdr_name.str, p_src_tag->hdr_name.str);

		p_dst_tag->tag_value.str_len = p_src_tag->tag_value.str_len;
		icf_port_strcpy(p_dst_tag->tag_value.str, p_src_tag->tag_value.str);
		
		p_dst_tag->next = ICF_NULL;
		p_dst_tag_last->next = p_dst_tag;
		p_dst_tag_last = p_dst_tag;

		p_src_tag = p_src_tag->next;
		p_dst_tag = p_dst_tag->next;
	} /* end while */
	
	ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_cmn_clone_tag_list() */

/*Message body header changes*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_body_list
 *
 * DESCRIPTION:     This function makes a copy of message body list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_body_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN	icf_msg_body_list_st	*p_src_body_list,
		OUT	icf_msg_body_list_st	*p_dst_body_list,
		OUT	icf_error_t		*p_ecode)
{
    icf_return_t		ret_val = ICF_SUCCESS;
	icf_msg_body_st	*p_src_body = ICF_NULL;
	icf_msg_body_st	*p_dst_body = ICF_NULL;
	icf_msg_body_st	*p_dst_body_last = ICF_NULL;
   

	ICF_FUNCTION_ENTER(p_glb_pdb)

	/* return if the source list is empty */
	if (ICF_NULL == p_src_body_list->count)
	{
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	}

	/* copy the size of the list */
	p_dst_body_list->count = p_src_body_list->count;
        
	/* Copy the Content_Type Header from source list to destination list
	 * present in the message body list.
	 */
	p_dst_body_list->content_type.str_len =  p_src_body_list->content_type.
		str_len;
	
        icf_port_strcpy(p_dst_body_list->content_type.str,
		p_src_body_list->content_type.str);
	

	/* copy all the elements of the received list */
	
	/* allocate memory for the first element in the list */
	ICF_MEMGET(p_glb_pdb,
		ICF_PORT_SIZEOF(icf_msg_body_st),
		ICF_MEM_COMMON,
		p_dst_body_list->body,
		ICF_RET_ON_FAILURE,
		p_ecode,
		ret_val)

	/* copy the contents of first element in the list into call ctx */
	p_dst_body_list->body->content_type.str_len = 
		p_src_body_list->body->content_type.str_len;
	icf_port_strcpy(p_dst_body_list->body->content_type.str,
		p_src_body_list->body->content_type.str);

	/* Copy the body headers count.
	 */
	p_dst_body_list->body->msg_body_hdr_list.count =
	       p_src_body_list->body->msg_body_hdr_list.count;

	/* Copy the body headers if present.
	 */
	if(0 < p_src_body_list->body->msg_body_hdr_list.count)
	{
              ret_val = icf_cmn_clone_header_list(p_glb_pdb,\
			      &(p_src_body_list->body->msg_body_hdr_list),\
			      &(p_dst_body_list->body->msg_body_hdr_list),\
			      p_ecode);
	      if(ICF_FAILURE == ret_val)
	      {
		       /* Deallocate the memory allocated to the body 
			* node
			*/
		       ICF_MEMFREE(p_glb_pdb,p_dst_body,
				       ICF_MEM_COMMON,
				       p_ecode, ret_val);
		       return ret_val;  
	       }
	}

	p_dst_body_list->body->length = p_src_body_list->body->length;
	
	if (ICF_NULL != p_src_body_list->body->length)
	{
		ICF_MEMGET(p_glb_pdb,
			(p_src_body_list->body->length + 1),
			ICF_MEM_COMMON,
			p_dst_body_list->body->value,
			ICF_RET_ON_FAILURE,
			p_ecode,
			ret_val)

		icf_port_memcpy(p_dst_body_list->body->value,
			p_src_body_list->body->value,p_src_body_list->body->length);
		p_dst_body_list->body->value[p_dst_body_list->body->length] = '\0';
	}

	p_dst_body_list->body->next = ICF_NULL;
	
	p_src_body = p_src_body_list->body->next;
	p_dst_body = p_dst_body_list->body->next;
	p_dst_body_last = p_dst_body_list->body;

	while (ICF_NULL != p_src_body)
	{
		/* allocate memory for the next element in the list */
		ICF_MEMGET(p_glb_pdb,
			ICF_PORT_SIZEOF(icf_msg_body_st),
			ICF_MEM_COMMON,
			p_dst_body,
			ICF_RET_ON_FAILURE,
			p_ecode,
			ret_val)
  
                if (ICF_NULL == p_dst_body)
                {
                    return ICF_FAILURE;
                }  

		/* copy the contents of next element in the list into call ctx */
		p_dst_body->content_type.str_len = p_src_body->content_type.str_len;
		icf_port_strcpy(p_dst_body->content_type.str,
			p_src_body->content_type.str);
	
		p_dst_body->length = p_src_body->length;
	
               if(0 < p_src_body->msg_body_hdr_list.count)
	       {
		       ret_val = icf_cmn_clone_header_list(p_glb_pdb,
				       &(p_src_body->msg_body_hdr_list), 
			       &(p_dst_body->msg_body_hdr_list),p_ecode);
		       if(ICF_FAILURE == ret_val)
		       {
			       /* Deallocate the memory allocated to the body 
				* node
				*/
			       ICF_MEMFREE(p_glb_pdb,p_dst_body,
					       ICF_MEM_COMMON,
					       p_ecode, ret_val);
			       return ret_val;  
		       }
	       }
		
		if (ICF_NULL != p_src_body->length)
		{
			ICF_MEMGET(p_glb_pdb,
					(p_src_body->length + 1),
					ICF_MEM_COMMON,
					p_dst_body->value,
					ICF_DONOT_RET_ON_FAILURE,
					p_ecode,
					ret_val)

                        if (ICF_FAILURE == ret_val || ICF_NULL == p_dst_body->value)
                        {
                            if (ICF_NULL != p_dst_body)
                            {
			       /* Deallocate the memory allocated to the body 
				* node
				*/
			       ICF_MEMFREE(p_glb_pdb,p_dst_body,
					       ICF_MEM_COMMON,
					       p_ecode, ret_val)
                               return ICF_FAILURE;                                 
                            }
                        } 

			icf_port_memcpy(p_dst_body->value,
						p_src_body->value,p_src_body->length);
			p_dst_body->value[p_src_body->length] = '\0';

		}

		p_dst_body->next = ICF_NULL;
		p_dst_body_last->next = p_dst_body;
		p_dst_body_last = p_dst_body;

		p_src_body = p_src_body->next;
		p_dst_body = p_dst_body->next;
	} /* end while */
	
	ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_cmn_clone_body_list() */


#ifdef ICF_REL_OLD
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_stream_list
 *
 * DESCRIPTION:     This function makes a copy of stream list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_stream_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN		icf_stream_list_st	*p_src_stream_list,
		OUT		icf_stream_list_st	*p_dst_stream_list,
		OUT		icf_error_t			*p_ecode)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_uint8_t						ctr = ICF_NULL;

	ICF_FUNCTION_ENTER(p_glb_pdb)

	/* return if the source list is empty */
	if (ICF_NULL == p_src_stream_list->count)
	{
		ICF_FUNCTION_EXIT(p_glb_pdb)
		return ret_val;
	}

	/* copy the size of the list */
	p_dst_stream_list->count = p_src_stream_list->count;

	/* copy the received list into call ctx */
	for (ctr = 0; ctr < (p_src_stream_list->count); ctr++)
	{
		icf_port_memcpy(
			(icf_void_t *)&(p_dst_stream_list->stream[ctr]),
			(icf_void_t *)&(p_src_stream_list->stream[ctr]),
			(icf_uint32_t)ICF_PORT_SIZEOF(icf_stream_st));
	} /* end for(ctr) */
		
	ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* icf_cmn_clone_stream_list() */
#endif


ICF_SESSION_CONTROL_END


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_ip_string_to_transport_addr
 *
 * DESCRIPTION:     This function Converts IP string to Transport Address
 *                  Currently handles only for ICF_TRANSPORT_ADDRESS_IPV4_ADDR
 ******************************************************************************/
icf_return_t icf_cmn_convert_ip_string_to_transport_address(
        IN icf_uint8_t *p_IP,
        OUT icf_transport_address_st *p_address)
{
    if(ICF_NULL == p_IP)
    {
        return ICF_FAILURE;
    }
    else
    {
        return icf_port_str_to_trans_addr(p_IP,&(p_address->addr));
    }
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_ip_string_to_transport_addr
 *
 * DESCRIPTION:     This function Converts IP string to Transport Address
 *                  Currently handles only for ICF_TRANSPORT_ADDRESS_IPV4_ADDR
 ******************************************************************************/
icf_return_t icf_cmn_convert_ip_string_to_transport_addr(
        IN icf_uint8_t *p_IP,
        OUT icf_transport_addr_st *p_address)
{
    if(ICF_NULL == p_IP)
    {
        return ICF_FAILURE;
    }
    else
    {		
		if(icf_port_str_to_trans_addr(p_IP,p_address) == ICF_FAILURE){
			p_address->addr.domain.str_len = 
                  (icf_uint16_t) icf_port_strlen(p_IP);
            icf_port_strcpy(p_address->addr.domain.str,p_IP);
            p_address->addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
        }
    }
    
    return ICF_SUCCESS;
}

/*******************************************************************************
 * FUNCTION:        icf_cmn_is_null_ip_address
 *
 * DESCRIPTION:     This function checks for NULL ip address
 *                  
 * RETURNS          ICF_SUCCESS: In case ip address is NULL
 *					ICF_FAILURE: In case ip address is not NULL
 ******************************************************************************/
icf_return_t icf_cmn_is_null_ip_address( 
IN		icf_transport_addr_st ip_address){
	icf_uint32_t temp_ip_addr = 0;
	if(ip_address.addr_type == ICF_TRANSPORT_ADDRESS_IPV4_ADDR){
		temp_ip_addr = ICF_MAKE_32BIT_INT(ip_address.addr.ipv4_addr.octet_1,
	                    ip_address.addr.ipv4_addr.octet_2,
	                    ip_address.addr.ipv4_addr.octet_3,
	                    ip_address.addr.ipv4_addr.octet_4);
	
		if(ICF_NULL != temp_ip_addr)
		{
			return ICF_FAILURE;
		}
	}
#ifdef ICF_IPV6_ENABLED
	else if(ip_address.addr_type == ICF_TRANSPORT_ADDRESS_IPV6_ADDR){
		if(ip_address.addr.ipv6_addr.str != NULL || ip_address.addr.ipv6_addr. str_len != 0){
			return ICF_FAILURE;
		}
	}
#endif
	return ICF_SUCCESS;

}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_transport_addr_to_ip_string
 *
 * DESCRIPTION:     This function Converts IP string to Transport Address
 *                  Currently handles only for ICF_TRANSPORT_ADDRESS_IPV4_ADDR
 ******************************************************************************/
icf_return_t icf_cmn_convert_transport_addr_to_ip_string(
        IN icf_transport_addr_st *p_address,
		OUT icf_uint8_t *p_IP
)
{
    icf_uint8_t temp1=0,temp2=0,temp3=0,temp4=0;
    icf_return_t ret_val = ICF_SUCCESS;

    if(ICF_NULL == p_address)
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
		switch(p_address->addr_type)
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
			{
				/*convert domain name to ip
				gethostbyname(p_address);*/
			}
				break;

			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
			{
				temp1 = p_address->addr.ipv4_addr.octet_1;
				temp2 = p_address->addr.ipv4_addr.octet_2;
				temp3 = p_address->addr.ipv4_addr.octet_3;
				temp4 = p_address->addr.ipv4_addr.octet_4;
				icf_port_sprintf((icf_int8_t *)p_IP,"%d.%d.%d.%d",temp1,temp2,temp3,temp4);
			}
				break;

			case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
			
			{
				icf_port_strcpy(
					(icf_void_t *)p_IP,
					(icf_void_t *)&(p_address->addr.ipv6_addr.str));
			}
				break;
		}
    }
    
    return ret_val;
}


ICF_4_0_CRM_START

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_transport_addr_to_string
 *
 * DESCRIPTION:     This function Converts transport Address to a string
 *                  Currently handles only for ICF_TRANSPORT_ADDRESS_IPV4_ADDR
 *					And domain name
 ******************************************************************************/
icf_return_t icf_cmn_convert_transport_addr_to_string(
        IN icf_transport_addr_st *p_address,
		OUT icf_uint8_t *p_addr_string)
{
    icf_uint32_t temp1=0,temp2=0,temp3=0,temp4=0;
    icf_return_t ret_val = ICF_SUCCESS;

    if(ICF_NULL == p_address)
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
		if(p_address->addr_type == ICF_TRANSPORT_ADDRESS_IPV4_ADDR)
		{
            temp1 = p_address->addr.ipv4_addr.octet_1;
            temp2 = p_address->addr.ipv4_addr.octet_2;
            temp3 = p_address->addr.ipv4_addr.octet_3;
			temp4 = p_address->addr.ipv4_addr.octet_4;
			icf_port_sprintf((icf_int8_t *)p_addr_string,"%d.%d.%d.%d",temp1,temp2,temp3,temp4);
		}
		else if (ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_address->addr_type)
		{
			/*convert domain name to string */
			icf_port_strcpy(p_addr_string, p_address->addr.domain.str);
		}
#ifdef ICF_IPV6_ENABLED
		else if( ICF_TRANSPORT_ADDRESS_IPV6_ADDR == p_address->addr_type)
		{
			icf_port_strcpy(p_addr_string,p_address->addr.ipv6_addr.str);
		}
#endif
    }/* Non NULL address: ends */
    
    return ret_val;
}/* Function convert_transport_addr_to_string() ends here */

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_transport_address_to_ip_string
 *
 * DESCRIPTION:     This function converts the transport address to a string
 *                  to be  sored in the SDP body of the SIP message     
 *
 ******************************************************************************/
icf_return_t icf_cmn_convert_transport_address_to_ip_string(
        IN icf_transport_address_st *p_rtp_address,
        OUT icf_uint8_t *p_remoteIP)
{
	if(p_rtp_address->addr.addr_type == ICF_TRANSPORT_ADDRESS_IPV4_ADDR){
		icf_port_sprintf((icf_int8_t *)p_remoteIP,"%d.%d.%d.%d",
            p_rtp_address->addr.addr.ipv4_addr.octet_1,
            p_rtp_address->addr.addr.ipv4_addr.octet_2,
            p_rtp_address->addr.addr.ipv4_addr.octet_3,
            p_rtp_address->addr.addr.ipv4_addr.octet_4);
	}
#ifdef ICF_IPV6_ENABLED
	else if( p_rtp_address->addr.addr_type == ICF_TRANSPORT_ADDRESS_IPV6_ADDR){
		strcpy(p_remoteIP, p_rtp_address->addr.addr.ipv6_addr.str);
	}
#endif
    return ICF_SUCCESS;
}

ICF_4_0_CRM_END

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_compare_transport_addresses
 *
 * DESCRIPTION:     This function compares two transport addresses and returns
 *                  success if they are same, else returns failure     
 *
 ******************************************************************************/
icf_return_t icf_cmn_compare_transport_addresses(
        IN icf_transport_address_st    *p_address_1,
        IN icf_transport_address_st    *p_address_2,
        INOUT icf_glb_pdb_st              *p_glb_pdb)
{
    icf_boolean_t    transport_addr_same = ICF_FALSE;
    icf_return_t     ret_val             = ICF_SUCCESS;

    /*SPR18535*/
    p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if ((ICF_NULL == p_address_1) || (ICF_NULL == p_address_2))
    {
        ret_val = ICF_FAILURE;
    }
    else if (p_address_1->addr.addr_type == p_address_2->addr.addr_type)
    {
        /* Check if transport addr is same */
        switch (p_address_1->addr.addr_type)
        {
            case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
            {    
                if (p_address_1->addr.addr.domain.str_len == 
                    p_address_2->addr.addr.domain.str_len)
                {
                    if (ICF_NULL == icf_port_strcmp(
                          p_address_1->addr.addr.domain.str,
                          p_address_2->addr.addr.domain.str))
                    {
                        transport_addr_same = ICF_TRUE;
                    }
                }
            }
            break; /* end of domain case */
            case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
            {
                if ((p_address_1->addr.addr.ipv4_addr.octet_1 == 
                     p_address_2->addr.addr.ipv4_addr.octet_1) 
                     && (p_address_1->addr.addr.ipv4_addr.octet_2 == 
                         p_address_2->addr.addr.ipv4_addr.octet_2) 
                     && (p_address_1->addr.addr.ipv4_addr.octet_3 == 
                         p_address_2->addr.addr.ipv4_addr.octet_3) 
                     && (p_address_1->addr.addr.ipv4_addr.octet_4 == 
                         p_address_2->addr.addr.ipv4_addr.octet_4))
                {
                    transport_addr_same = ICF_TRUE;
                }
            }
            break; /* End of ipv4 case */
            case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
            {
                if (p_address_1->addr.addr.ipv6_addr.str_len == 
                    p_address_2->addr.addr.ipv6_addr.str_len)
                {
                    if (ICF_NULL == icf_port_strcmp (
                                p_address_1->addr.addr.ipv6_addr.str, 
                                p_address_2->addr.addr.ipv6_addr.str))
                    {
                        transport_addr_same = ICF_TRUE;
                    }
                }
            }
            break; /* end of ipv6 case */
            default:
            break;
        }/* End of switch */
        /* Now check if ports are also same */
        if ((ICF_TRUE == transport_addr_same) && 
            (p_address_1->port_num ==
                p_address_2->port_num))
        {
           ret_val = ICF_SUCCESS; /* Primary and secondary same*/
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    } /* end if addr types are same */
    else
    {
        ret_val = ICF_FAILURE;
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_copy_transport_address
 *
 * DESCRIPTION:     This function copies transport address from p_address_2 
 *                  to p_address_1
 *
 ******************************************************************************/
icf_return_t icf_cmn_copy_transport_address(
        INOUT icf_transport_address_st    *p_address_1,
        IN    icf_transport_address_st    *p_address_2,
        INOUT icf_glb_pdb_st              *p_glb_pdb,
        INOUT icf_error_t                 *p_ecode)
{
    icf_return_t     ret_val             = ICF_SUCCESS;
   /*SPR18535*/ 
    p_glb_pdb = p_glb_pdb;

    /* To remove compile time warning */
    p_ecode = p_ecode;
    ICF_FUNCTION_ENTER(p_glb_pdb)

    if ((ICF_NULL == p_address_1) || (ICF_NULL == p_address_2))
    {
        ICF_SET_ECODE(p_glb_pdb, ICF_NULL, ICF_CAUSE_INTERNAL_FAILURE,
                                                p_ecode)
        ret_val = ICF_FAILURE;
    }
    else
    {
        p_address_1->port_num = p_address_2->port_num;
        p_address_1->addr.addr_type = p_address_2->addr.addr_type;
        switch (p_address_2->addr.addr_type)
        {
            case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
                {    
                    p_address_1->addr.addr.domain.str_len = 
                        p_address_2->addr.addr.domain.str_len;
                    icf_port_strcpy(p_address_1->addr.addr.domain.str,
                            p_address_2->addr.addr.domain.str);
                }
                break; /* end of domain case */
            case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
                {
                    p_address_1->addr.addr.ipv4_addr.octet_1 = 
                        p_address_2->addr.addr.ipv4_addr.octet_1;
                    p_address_1->addr.addr.ipv4_addr.octet_2 = 
                        p_address_2->addr.addr.ipv4_addr.octet_2;
                    p_address_1->addr.addr.ipv4_addr.octet_3 =
                        p_address_2->addr.addr.ipv4_addr.octet_3;
                    p_address_1->addr.addr.ipv4_addr.octet_4 =
                        p_address_2->addr.addr.ipv4_addr.octet_4;
                }
                break; /* End of ipv4 case */
            case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
                {
                    p_address_1->addr.addr.ipv6_addr.str_len = 
                        p_address_2->addr.addr.ipv6_addr.str_len;
                    icf_port_strcpy (p_address_1->addr.addr.ipv6_addr.str, 
                            p_address_2->addr.addr.ipv6_addr.str);
                }
                break; /* end of ipv6 case */
            default:
                break;
        }/* End of switch */
        ret_val = ICF_SUCCESS;
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

#ifdef ICF_NW_ACTIVATE
/***************************************************************************
 *
 * FUNCTION:        icf_cfg_get_network_status
 *
 * DESCRIPTION:     This fn can be called by any module to check the 
 *                  status of network associated with this app_id
 * 
 **************************************************************************/
icf_result_t  icf_cfg_get_network_status(
                 INOUT    icf_glb_pdb_st    *p_glb_pdb,
                 IN 	  icf_app_id_t        app_id)
{
    icf_result_t			ret_val = ICF_FAILURE;
    icf_dbm_data_st      *p_dbm_data = ICF_NULL;
    icf_uint8_t           nw_index = 0;
	icf_uint8_t           app_index = 0;
    icf_error_t           *p_ecode = ICF_ERROR_NONE;
	icf_netact_ctxt_st   *p_netact_ctxt = ICF_NULL;
    icf_config_data_st   *p_config_data = ICF_NULL;
    ICF_FUNCTION_ENTER(p_glb_pdb)


        if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
			p_glb_pdb,
			(icf_int_module_id_t const)ICF_INT_MODULE_DBM,
			(icf_void_t *)&p_dbm_data,
			p_ecode))
        {
            p_netact_ctxt = (icf_netact_ctxt_st *)
                p_dbm_data->p_netact_ctxt_blk;
			
            if(ICF_SUCCESS == icf_dbm_get_module_glb_data(     
				p_glb_pdb,
				(icf_int_module_id_t const)ICF_INT_MODULE_CFG,
				(icf_void_t *)&p_config_data,
				p_ecode))
            {
                /* search the list of networks */
                for(nw_index = 0; 
					nw_index < p_config_data->max_networks;
					nw_index++)
                {
                    if ((p_netact_ctxt[nw_index].is_valid
						== ICF_TRUE) 
						&& ((p_netact_ctxt[nw_index].activate_state
									== ICF_NETACT_02_ACTIVATED) ||
                                    /*SPR 10950 fix*/
                                    (p_netact_ctxt[nw_index].activate_state
									== ICF_NETACT_05_NW_SUSPENDED)))
		    
		    {
			    /* Check whether the APP id is ICF_DUMMY_APP_ID or not
			     * if yes then return success 
			     */
			    if (ICF_DUMMY_APP_ID == app_id)
			    {
				    /* This for the case when auto_activate OFF and auto_reg OFF */
				    ret_val = ICF_SUCCESS;
				    break;
			    }

			    /* search the list of 
			     * applications associated with the  network */
			    for(app_index = 0;
					    app_index < p_config_data->max_applications;
					    app_index++)
			    {
				    if ((p_netact_ctxt[nw_index].p_app_ctxt[app_index].is_valid
							    == ICF_TRUE)
						    && (p_netact_ctxt[nw_index].p_app_ctxt[app_index].app_id
							    == app_id))
				    {
					    ret_val = ICF_SUCCESS;
					    break;
				    }
			    } /* end of for loop */

			    if (ret_val == ICF_SUCCESS)
			    {
				    break;
			    }
			    else
			    {
				    /* Return status of topmost auto_activated n/w */
				    if (p_netact_ctxt[nw_index].auto_activated == \
						    ICF_TRUE)
				    {
					    ret_val = ICF_SUCCESS;
					    break; 
				    }
			    }

		    } /* end of if(isvaild= true) */
				}/* end of for loop for nw_index */
			}
        }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
#endif

/***************************************************************************
 *
 * FUNCTION:        icf_cfg_get_app_id_validity
 *
 * DESCRIPTION:     This fn can be called by any module to check the 
 *                  validity of given app_id
 * 
 **************************************************************************/
icf_result_t  icf_cfg_get_app_id_validity(
                 INOUT    icf_glb_pdb_st    *p_glb_pdb,
                 IN 	  icf_app_id_t      app_id)
{
    icf_boolean_t        ret_val = ICF_FAILURE;
    icf_dbm_data_st      *p_dbm_data = ICF_NULL;
    icf_app_conf_st      *p_app_conf_1 = ICF_NULL;
    icf_error_t          ecode = ICF_ERROR_NONE;
    icf_app_conf_st      *p_app_conf = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
  
	  /* Get DBM data to fetch network access context */
	  if(ICF_SUCCESS == icf_dbm_get_module_glb_data(p_glb_pdb,\
		  (icf_int_module_id_t const)\
		  ICF_INT_MODULE_DBM,
		  (icf_void_t *)&p_dbm_data,
		  &ecode))
	  {
		  p_app_conf = (icf_app_conf_st *)p_dbm_data->p_app_conf;
		  
		  if(ICF_SUCCESS == icf_cfg_fetch_app_conf(p_glb_pdb,\
			  app_id, 
			  &p_app_conf_1,\
			  &ecode))
		  {
			  ret_val = ICF_SUCCESS;
		  }
		  else
		  {
			  ret_val = ICF_FAILURE;
		  }
	  }

      
  ICF_FUNCTION_EXIT(p_glb_pdb)
  return ret_val;
}


ICF_4_0_CRM_START
/******************************************************************************
 * 
 *              XML PARSER : Currently Used for Parsing RegEvent XML Body
 *
 *****************************************************************************/

 /****************************************************************************
 *          UTILITY Functions of XML parser
 ****************************************************************************/

/**************************************************************** 
** FUNCTION:  icf_cmn_skip_chars_forward
**
** DESCRIPTION: Skips caharcters till a matching pattern
**
***************************************************************/ 
icf_boolean_t icf_cmn_skip_chars_forward
                    	(IN    icf_int8_t* pInput,
	                     INOUT icf_int32_t* pPosition,
	                     IN    icf_int8_t* pPattern)
{
	icf_int32_t matched=0,len=0;
	len=icf_port_strlen((icf_uint8_t *)pPattern);

	/* 
	 * While not end of buffer input, skip data 
	 * till the pattern is found
	 */
	while((pInput[(*pPosition)++]!='\0')&&(!matched))
	{
		if(!icf_port_strncmp((icf_uint8_t *)&pInput[*pPosition],(icf_uint8_t *)pPattern,len))
		{
			*pPosition=*pPosition+icf_port_strlen((icf_uint8_t *)pPattern)-1;

			matched=1;
		}
	}
	if(!matched)			
		return ICF_FALSE;

	return ICF_TRUE;

}/* Function icf_cmn_skip_chars_forward() ends */

/***************************************************************
** FUNCTION:  icf_cmn_skip_spaces
**
** DESCRIPTION: This function skips spaces in given buffer and at a 
**				given position. It advances the pointer to the next 
**				non-space character.
***************************************************************/ 
icf_void_t icf_cmn_skip_spaces
	            (IN    icf_int8_t* pInput,
	             INOUT icf_int32_t* pPosition)
{

	while ((pInput[*pPosition]==' ')||(pInput[*pPosition]=='\n')||\
		(pInput[*pPosition]=='\b')||(pInput[*pPosition]=='\t')||\
		(pInput[*pPosition]=='\r'))
	{
		*pPosition=*pPosition+1;
	}

}/* Function icf_cmn_skip_spaces((icf_int8_t *)) ends */

/***************************************************************
 ** FUNCTION:  icf_cmn_get_valid_node
 **
 ** DESCRIPTION: This function advances the pointer to the next 
 **	non-space character, and put the word to the output
 **************************************************************/ 
icf_return_t icf_cmn_get_valid_node
   (IN    icf_int8_t*  pInput,
	INOUT icf_int32_t* pPosition,
	INOUT icf_int8_t*  pOutPut)
{
	
	icf_int32_t counter =0;
	while (!((pInput[*pPosition] ==' ')||(pInput[*pPosition]=='\n')||\
		(pInput[*pPosition]=='\b')||(pInput[*pPosition]=='\t')||\
		(pInput[*pPosition]=='\r') ||(pInput[*pPosition]=='>')||\
		(pInput[*pPosition]=='<') || (pInput[*pPosition]=='\\')\
		/* ||(pInput[*pPosition]=='//')*/))
	{
		pOutPut[counter++] = pInput[(*pPosition)++];
	}
    return ICF_SUCCESS;
}
/***************************************************************
** FUNCTION:  icf_cmn_xml_parse_attribute
**
** DESCRIPTION: This function parse Attribute of the Node and 
**              calls the callback function towards the applcation
**              context
***************************************************************/ 
icf_return_t icf_cmn_xml_parse_attribute 
                     (IN icf_uint8_t  *pInput, 
                     INOUT icf_uint32_t *pPosition,
                     INOUT icf_uint8_t  *pNode,
                     INOUT icf_void_t   *p_app_ctxt,
                     INOUT icf_error_t  *pErr)

{
    icf_int8_t attribute[icf_maxConfigFileLen] = {'\0'};
    icf_int8_t value[icf_maxConfigFileLen]={'\0'};
    icf_uint32_t counter = 0;
	icf_uint32_t BreakVar=0;
    
    /*ICF_PRINT(((icf_uint8_t*)"\nNODE ATTRIVUTE:\n",attribute));*/
	icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t*)pPosition);
	
	while(pInput[(*pPosition)]!='>')
	{
    	/* get some x="y" into att */
        counter = 0;
		while(pInput[*pPosition] !='=')
        {
           
			if((pInput[(*pPosition)] =='<') ||
				(pInput[(*pPosition)] =='>'))				
			{
				/* SPR 9067: fix ends */
				ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag():"
                "XML invalid syntax -- = or > expected\n"));
				BreakVar=1;
				break;
			}            

			 attribute[counter++]= pInput[(*pPosition)++];
        }
        if(BreakVar==1)
        {
            break;
        }
        attribute[counter] = '\0';
		*pPosition = *pPosition +1;		
		icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t*)pPosition);
		
		
		if(pInput[(*pPosition)++]!='"')
		{

            ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag():"
                "XML invalid syntax -- \" expected\n"));
			BreakVar=1;
			break;
		}
		
		counter = 0;
		while(pInput[*pPosition] !='"')
		{
			
			if((pInput[(*pPosition)] =='<') ||
				(pInput[(*pPosition)] =='>'))
				
			{
				
				ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag():"\
					"XML invalid syntax -- > or \" expected\n"));
				BreakVar=1;
				break;
			}            
			value[counter++]= pInput[(*pPosition)++];
		}

		

        if(BreakVar==1)
        {
            break;
        }
        value[counter]='\0';
        *pPosition = *pPosition +1;
		icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t*)pPosition);
		/****************AttributeForNode CALLBACK TO APPLICATION ****************/
		if(ICF_FAILURE == 
            icf_cmn_xmlparser_cbk_node_attribute(p_app_ctxt,
                                    pNode,(icf_uint8_t *)attribute,(icf_uint8_t*)value,pErr))
        {
            BreakVar=1;
			break;
        }
  	}

    if(BreakVar==1)
    {
		return ICF_FAILURE;	
    }
	else
	{
		icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t *)pPosition);

	}
    return ICF_SUCCESS;
}/* function icf_cmn_xml_parse_attribute() ends */


/***************************************************************
** FUNCTION:  icf_cmn_xml_parse_tag
**
** DESCRIPTION: This function parse Xml Tag and 
**              calls the callback function towards the applcation
**              context
***************************************************************/ 

icf_return_t icf_cmn_xml_parse_tag(IN icf_uint8_t	*pInput,
                                    INOUT icf_uint32_t   *pPosition,
                                    INOUT icf_void_t     *p_app_ctxt,
                                    INOUT icf_error_t    *p_ecode)
{
	/* Flag to exit the Parsing */
	icf_uint32_t ExitVar=0;

   	icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t *)pPosition);

   	/* 
	 * Special tag (like the <?xml ...?> tag at the beginning of a document)
	 */
    if(!icf_port_strncmp(&pInput[*pPosition],(icf_uint8_t *)"<?",2))
	{
	   	if(!icf_cmn_skip_chars_forward((icf_int8_t*)pInput,(icf_int32_t*)pPosition,(icf_int8_t *)"?>"))
		{
            ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag(): ?> not found\n"));
            return ICF_FAILURE;
		}
	}

    /* Skip comments */
    if(!icf_port_strncmp(&pInput[*pPosition],(icf_uint8_t *)"<!--",4))
	{
		if(!icf_cmn_skip_chars_forward((icf_int8_t*)pInput,(icf_int32_t *)pPosition,(icf_int8_t *)"-->"))
		{
			ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag(): \"-->\" not found\n"));
			return ICF_FAILURE;
		}
	}
    /* Check For EndTag */
    if(!icf_port_strncmp(&pInput[*pPosition],(icf_uint8_t *) "</",2))
	{
		ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag(): "\
            "XML invalid syntax--Improper Close tag\n\n"));
		return ICF_FAILURE;
	}

    /* Opening tag */
    if(!icf_port_strncmp(&pInput[*pPosition],(icf_uint8_t *)"<",1))
    {
		icf_int8_t nodeName[icf_maxConfigFileLen]={'\0'},
                      endTagExpected[icf_maxConfigFileLen]={'\0'}; 

		*pPosition=*pPosition+1;

		/* Retrieve the node name and store it in the Nodes tagName files */
		icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t *)pPosition);

		/*icf_port_sscanf(&pInput[*pPosition],"%[a-z]",nodeName);*/
		/* *pPosition =*pPosition +icf_port_strlen(nodeName);*/
		icf_cmn_get_valid_node((icf_int8_t *)pInput,(icf_int32_t *)pPosition,nodeName);


		/****************NODE_START CALLBACK TO APPLICATION ****************/
		if(ICF_FAILURE == 
            icf_cmn_xmlparser_cbk_node_start(p_app_ctxt,(icf_uint8_t*)nodeName,p_ecode))
        {
		    return ICF_FAILURE;
        }
		/* The end tag expected for this particular node */
		icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t *)pPosition);


		/* Check for the attributes */
		if(pInput[*pPosition]!='>')
		{
			if(icf_cmn_xml_parse_attribute
                ((icf_uint8_t *)pInput,pPosition,(icf_uint8_t*)nodeName,p_app_ctxt,p_ecode)== ICF_FAILURE)
            {
				return ICF_FAILURE;
            }
           /* ICF_PRINT(((icf_uint8_t*)"NODE ATTRIVUTE: %s \n",attribute));*/
		}
		icf_port_strcpy((icf_uint8_t *)endTagExpected,(icf_uint8_t *)"</");
		icf_port_strcat((icf_uint8_t *)endTagExpected,(icf_uint8_t *)nodeName);
		icf_port_strcat((icf_uint8_t *)endTagExpected,(icf_uint8_t *)">");
		icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t *)pPosition);

    	while (icf_port_strncmp(&pInput[*pPosition], (icf_uint8_t *)endTagExpected,\
			icf_port_strlen((icf_uint8_t *)endTagExpected)))
        {
			icf_int32_t len=0,scanf_retval = 0;

			icf_int8_t tagData[icf_maxConfigFileLen]={'\0'};

			if(!icf_port_strncmp(&pInput[*pPosition],(icf_uint8_t *)"</",2))
			{
               ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag()"
						"XML invalid syntax--invalid end tag expecting node-name\n"));
				ExitVar=1;
				break;
			}
			if(pInput[*pPosition]== '<')
			{
				if(ICF_FAILURE == icf_cmn_xml_parse_tag(pInput,
                            pPosition,p_app_ctxt, \
							p_ecode))
				{
					ExitVar=1;
					break;	
				}
				icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t *)pPosition);
				continue;
			}
            if(pInput[*pPosition] == '>')
            {
                *pPosition=*pPosition+1;
            }
			/* Get the total string */
			scanf_retval = icf_port_sscanf((const icf_int8_t *)&pInput[*pPosition],"%[^<]",tagData);
            if(scanf_retval > 0)
            {
                len=icf_port_strlen((icf_uint8_t *)tagData);
                if(len!=0)
                {
                    while(tagData[len-1]==' ')
                        len--;
                }
                tagData[len]='\0';
                /* Copy the tag Data in the Node tagData field */
                *pPosition=*pPosition+icf_port_strlen((icf_uint8_t *)tagData);
                /****************NODE_END CALLBACK TO APPLICATION ****************/
				if(ICF_FAILURE == 
                    icf_cmn_xmlparser_cbk_node_tagdata(p_app_ctxt,
                    (icf_uint8_t*)nodeName,(icf_uint8_t*)tagData,p_ecode))
                {
                    ExitVar=1;
                    break;	
                }
            }
            else if(scanf_retval < 0)
            {
                ICF_PRINT(((icf_uint8_t*)"icf_cmn_xml_parse_tag()"
                    "XML invalid syntax--invalid end tag expecting node-name\n"));
                ExitVar=1;
                break;

            }

    		icf_cmn_skip_spaces((icf_int8_t *)pInput,(icf_int32_t *)pPosition);
        }
        if(ExitVar==1)
		{
			return ICF_FAILURE;
		}
		*pPosition=*pPosition+icf_port_strlen((icf_uint8_t *)endTagExpected);

		/****************NODE_END CALLBACK TO APPLICATION ****************/
		if(ICF_FAILURE == 
            icf_cmn_xmlparser_cbk_node_end(p_app_ctxt,(icf_uint8_t*)nodeName,p_ecode))
        {
			return ICF_FAILURE;
        }

    }

    return ICF_SUCCESS;
}/* Function icf_cmn_xml_parse_tag() ends */


/***************************************************************
** FUNCTION:  icf_cmn_parse_xml_body
**
** DESCRIPTION: This function parse XML body and 
**              calls the callback function towards the applcation
**              context
***************************************************************/ 
icf_return_t icf_cmn_parse_xml_body(
                                    IN      icf_uint8_t		  *pbuf,
                                    IN      icf_uint32_t		  buf_length,
                                    INOUT   icf_void_t		  *p_reginfo,
                                    INOUT   icf_error_t	      *p_ecode)
{
    icf_uint32_t position=0;
    icf_boolean_t BreakVar =0;
    /* If first character is not a < then it is not a xml body
       and we do not parse and return failure*/
    if(pbuf[position] != '<')
    {
        BreakVar = 1;
    }
/*    pbuf[buf_length]='\0';
      printf("%s",pbuf);*/
    else
    {
	while(position<buf_length)
	{
		icf_cmn_skip_spaces((icf_int8_t *)pbuf,(icf_int32_t *)&position);
		if(pbuf[position]=='<' )
		{
        	if(ICF_FAILURE == 
                icf_cmn_xml_parse_tag(pbuf,&position,p_reginfo,p_ecode))
			 {
				BreakVar=1;
				break;
			}
		}
	}
    }
    /* Check for XML Version : Proceed Further if Version is 1.0 only*/
    if(BreakVar == 1)
    {
        return ICF_FAILURE;
    }
    return ICF_SUCCESS;

}/* Function icf_cmn_parse_xml_body() ends */


/******************************************************************************
 * 
 *           END   XML PARSER : Currently Used for Parsing RegEvent XML Body
 *
 *****************************************************************************/

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_trnspt_addr
 *
 * DESCRIPTION:     This function validate the transport address recevied
 *                  from PA for address type
 *                  Validation Critteria:
 *                  
 *                  E164: An E164 number may or maynot start with '+' and must 
 *                  have only hexadecimal digits in it 
 *
 *                  Tel URI: A Tel URI may or may not start with '+' and must 
 *                  have only hexadecimal numbers with atleast on '-' seperator
 *
 *                  SIP URI: A number which is niether a tel uri or tel uri 
 *                  would be SIP URI if it has domain seperated by '@'
 *
 *
 *****************************************************************************/
icf_return_t icf_cmn_validate_trnspt_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode)
 {
    icf_return_t               ret_val = ICF_SUCCESS;

    /* This variable is used to calculate address type sent by remote
     * If address type is same as provided by user, then this function
     * return success else return failure */

    icf_address_type_t         addr_type = ICF_ADDRESS_PLAN_UNDEFINED;

    icf_uint8_t *p_addr_data = ICF_NULL;

    /* This variable is used to identfiy number of sip uri format */
    icf_uint8_t *p_sipnumber = ICF_NULL;
    /* This variable is used to compare first char iof number as teluri number
     * start with '+' char */
    icf_uint8_t telurl_start = '+';
    /* This variable is used to check presence of @ in string received from PA */
    icf_uint8_t sipuri_num = '@';
    /* This var is used in loops */
    icf_uint8_t counter =0 ;
    /* This varialbe is used to identfiy e164 number */
    icf_boolean_t e164_number = ICF_TRUE;
    /* This varialbe is used to identfiy tel number */
    icf_boolean_t tel_number =  ICF_TRUE;

    p_glb_pdb = p_glb_pdb;

    if(ICF_NULL == p_glb_pdb)
    {
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb) /*function entry trace */

    p_addr_data = (icf_uint8_t *)p_addr->addr_val.str;
    p_sipnumber = icf_port_strchr(p_addr_data,sipuri_num);

    /* if @ icf_uint8_t is not found, p_sipnumber will be null and
     * number can be telurl or e164 number */
    if(ICF_NULL == p_sipnumber)
    {
        /* Check if the address starts with a valid hexadecimal digit or '+'
         * If it doesn't it cant be E164 or Tel URI*/
        if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]) || (ICF_ISSPECIALDIGIT(p_addr_data[0])))
        {
            for(counter = 1; counter<icf_port_strlen(p_addr_data); counter++)
            {  
                /* A tel URI may have special digits like * and
                #.We are validating them.(Ref RFC 3966) */
                if(!ICF_ISXDIGIT(p_addr_data[counter])) 
                {
                    /* If this was not a hexadecimal digit. the number cant be 
                     * an E164 Number*/
                    e164_number = ICF_FALSE;

		       if(('-' != p_addr_data[counter]) &&
                       (!ICF_ISSPECIALDIGIT(p_addr_data[counter])))
                    {
                        /* If number does not contain [0-f] or '-'
                         *  it is not valid number  */
                        tel_number = ICF_FALSE;
                        break;
                    }
                }

            }/*End of for loop */

            if(ICF_TRUE == e164_number)
            {
                /* it is an E164 url number */
                addr_type = ICF_ADDRESS_PLAN_E164;
            }
            else if(ICF_TRUE == tel_number)
            {
                /* it is a tel url number */
                addr_type = ICF_ADDRESS_PLAN_TEL_URI;
            }

        }/*End p_addr_data[0] == telurl_start...*/
    }
    else
    {
        /* The previous check to see if there is a port also in sip_uri in self_address has been removed 
           and shifted to vldt module. SPR fix 17657 */
        if ( ICF_ADDRESS_PLAN_SIPS_URI == p_addr->addr_type)
        {
            addr_type = ICF_ADDRESS_PLAN_SIPS_URI;
        }
        else
        {
            addr_type = ICF_ADDRESS_PLAN_SIP_URI;
        }
    }

    /* if calculated address type is invalid return failure */
    if(ICF_ADDRESS_PLAN_UNDEFINED == addr_type) 
    {
        /* Find the address type
         * if it contain @ and no + in first char then it is SIP URL
         * IF address start with + and no @ then it is TELURL
         * else if it contain alphanumeric values
         * then it is e164 value else return error */
        ret_val = ICF_FAILURE;

    }
    else if((p_addr->addr_type != addr_type && 
                (ICF_ADDRESS_PLAN_SIP_URI == p_addr->addr_type || 
                ICF_ADDRESS_PLAN_SIP_URI == addr_type)) && 
            p_addr->addr_type!=ICF_ADDRESS_PLAN_UNDEFINED)
    {
         /* if calculated value is not same as value recevied in remote
          *  address, validation failed. Return failure */
          ret_val = ICF_FAILURE;
    }
    else if(ICF_ADDRESS_PLAN_UNDEFINED == p_addr->addr_type)
    {
        /* Update address type if it is undefined*/
        p_addr->addr_type = addr_type;
    }
    *p_ecode = *p_ecode;

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_called_party_addr
 *
 * DESCRIPTION:     This function validate the called party address recevied
 *                  from PA for address type
 *                  Validation Critteria:
 *
 *                  E164: An E164 number may or maynot start with '+' and must
 *                  have only hexadecimal digits in it
 *
 *                  Tel URI: A Tel URI may or may not start with '+' and must
 *                  have only hexadecimal numbers with atleast on '-' seperator
 *
 *                  SIP URI: A number which is niether a tel uri or tel uri
 *                  would be SIP URI if it has domain seperated by '@'
 *
 *
 *****************************************************************************/


icf_return_t icf_cmn_validate_called_party_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode)
 {
    icf_return_t               ret_val = ICF_SUCCESS;

    /* This variable is used to calculate address type sent by remote
     * If address type is same as provided by user, then this function
     * return success else return failure */

    icf_address_type_t         addr_type = ICF_ADDRESS_PLAN_UNDEFINED;

    icf_uint8_t *p_addr_data = ICF_NULL;

    /* This variable is used to identfiy number of sip uri format */
    icf_uint8_t *p_sipnumber = ICF_NULL;
    /* This variable is used to compare first char iof number as teluri number
     * start with '+' char */
    icf_uint8_t telurl_start = '+';
    /* This variable is used to check presence of @ in string received from PA */
    icf_uint8_t sipuri_num = '@';
    /* This var is used in loops */
    icf_uint8_t counter =0 ;
    /* This varialbe is used to identfiy e164 number */
    icf_boolean_t e164_number = ICF_TRUE;
    /* This varialbe is used to identfiy tel number */
    icf_boolean_t tel_number =  ICF_TRUE;

    p_glb_pdb = p_glb_pdb;

    if(ICF_NULL == p_glb_pdb)
    {
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb) /*function entry trace */

    p_addr_data = (icf_uint8_t *)p_addr->addr_val.str;
    p_sipnumber = icf_port_strchr(p_addr_data,sipuri_num);

    /* if @ icf_uint8_t is not found, p_sipnumber will be null and
     * number can be telurl or e164 number */
    if(ICF_NULL == p_sipnumber)
    {
        /* Check if the address starts with a valid hexadecimal digit or '+'
         * If it doesn't it cant be E164 or Tel URI*/
        if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]) || (ICF_ISSPECIALDIGIT(p_addr_data[0])))
        {
            for(counter = 1; counter<icf_port_strlen(p_addr_data); counter++)
            {
                if(!ICF_ISXDIGIT(p_addr_data[counter]))
                {
                    /* If this was not a hexadecimal digit. the number cant be
                     * an E164 Number*/
                    e164_number = ICF_FALSE;

                    if('-' != p_addr_data[counter])
                    {
                        /* If number does not contain [0-f] or '-'
                         *  it is not valid number  */
                        tel_number = ICF_FALSE;
                        break;
                    }
                }

            }/*End of for loop */

            if(ICF_TRUE == e164_number)
            {
                /* it is an E164 url number */
                addr_type = ICF_ADDRESS_PLAN_E164;
            }
            else if(ICF_TRUE == tel_number)
            {
                /* it is a tel url number */
                addr_type = ICF_ADDRESS_PLAN_TEL_URI;
            }

        }/*End p_addr_data[0] == telurl_start...*/
    }
    else
    {
        /*fix merged for CSR CSR_1-5409550 */
        if ( ICF_ADDRESS_PLAN_SIPS_URI == p_addr->addr_type)
        {
            addr_type = ICF_ADDRESS_PLAN_SIPS_URI;
        }
        else
        {
            addr_type = ICF_ADDRESS_PLAN_SIP_URI;
        }
    }

    /* if calculated address type is invalid return failure */
    if(ICF_ADDRESS_PLAN_UNDEFINED == addr_type)
    {
        /* Find the address type
         * if it contain @ and no + in first char then it is SIP URL
         * IF address start with + and no @ then it is TELURL
         * else if it contain alphanumeric values
         * then it is e164 value else return error */
        ret_val = ICF_FAILURE;

    }
    else if((p_addr->addr_type != addr_type &&
                (ICF_ADDRESS_PLAN_SIP_URI == p_addr->addr_type ||
                ICF_ADDRESS_PLAN_SIP_URI == addr_type)) &&
	  p_addr->addr_type!=ICF_ADDRESS_PLAN_UNDEFINED)
    {
         /* if calculated value is not same as value recevied in remote
          *  address, validation failed. Return failure */
          ret_val = ICF_FAILURE;
    }
    else if(ICF_ADDRESS_PLAN_UNDEFINED == p_addr->addr_type)
    {
        /* Update address type if it is undefined*/
        p_addr->addr_type = addr_type;
    }
    *p_ecode = *p_ecode;

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_string
 *
 * DESCRIPTION:    Initializes a structure of type icf_string_st 
 *
 *
 *****************************************************************************/
icf_void_t icf_cmn_init_string(
        INOUT icf_string_st *p_address)
{
    /* specifies the length of the string including '\0' character */ 
    p_address->str_len = 0;
    
    /* array of characters */ 
    icf_port_strcpy(p_address->str,(icf_uint8_t *)"\0");
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_transport_address
 *
 * DESCRIPTION:    Initializes a structure of type icf_transport_address_st 
 *
 *
 *****************************************************************************/
icf_void_t icf_cmn_init_transport_address(
        INOUT icf_transport_address_st *p_transport_address)
{
    /* set the port number of the transport address to 0 */ 
    p_transport_address->port_num = 0;
 
    /*Initialize as Domain name*/   
    p_transport_address->addr.addr.domain.str_len = 0;
    icf_port_strcpy(p_transport_address->addr.addr.domain.str,(icf_uint8_t *)"\0");

    /*Initialize as IP Address*/
    p_transport_address->addr.addr.ipv4_addr.octet_1 = 0;
    p_transport_address->addr.addr.ipv4_addr.octet_2 = 0;
    p_transport_address->addr.addr.ipv4_addr.octet_3 = 0;
    p_transport_address->addr.addr.ipv4_addr.octet_4 = 0;

    /*Initialize as IPv6 Address*/
    p_transport_address->addr.addr.ipv6_addr.str_len = 0;
    icf_port_strcpy(p_transport_address->addr.addr.ipv6_addr.str,(icf_uint8_t *)"\0");

     p_transport_address->addr.addr_type = 0;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_address
 *
 * DESCRIPTION:    Initializes a structure of type icf_address_st 
 *
 *
 *****************************************************************************/
icf_void_t icf_cmn_init_address(
        INOUT icf_address_st *p_address)
{
    /* specifies the string buffer containing the user address in a format 
     * defined by the addr_type field */
    icf_cmn_init_string(&p_address->addr_val);

    /* specifies the type of format for the user address information present in 
     * the addr_val parameter*/
    p_address->addr_type = ICF_ADDRESS_PLAN_UNDEFINED;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_check_if_param_present
 *
 * DESCRIPTION:    Function parses the given string for given parameter and
 *                 returns true if param present and value else false
 *
 *****************************************************************************/
icf_boolean_t icf_cmn_check_if_param_present(
                                                   icf_string_st  *p_addr,
                                                   icf_uint8_t    *p_param,
                                                   icf_uint8_t    *p_param_val,
                                                   icf_uint8_t    *p_hdr_name,
                                                   icf_tag_list_st   *p_tag_list)
{
    icf_boolean_t   ret_val = ICF_FALSE;
    icf_string_st   temp_addr;
    icf_int8_t     *p_rem_str_scln = ICF_NULL;
    icf_int8_t     *p_rem_str_eql = ICF_NULL;
    icf_uint8_t     *p_str = ICF_NULL;
    icf_uint8_t     *p_str1 = ICF_NULL;
    icf_uint8_t     cnt = 0;
    icf_tag_st         *p_tag = ICF_NULL;

    icf_port_strcpy(temp_addr.str, p_addr->str);

    do
    {        
        p_str = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)temp_addr.str, " ;", &p_rem_str_scln);
       
	if(ICF_NULL == p_str)
	{
	    ret_val = ICF_FALSE;
	    break;
	}
 
        p_str1 = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)p_str, "=", &p_rem_str_eql);

        if (!icf_port_strcmp(p_param, p_str))
        {
            if (p_rem_str_eql) icf_port_strcpy(p_param_val,(icf_uint8_t *) p_rem_str_eql);
            ret_val = ICF_TRUE;
            break;
        }
        if (ICF_NULL != p_rem_str_scln)
        {
            icf_port_strcpy(temp_addr.str,(icf_uint8_t *) p_rem_str_scln);
        }
        
    }while (ICF_NULL != p_rem_str_scln);

    if (ICF_TRUE != ret_val)
    {
        if (ICF_NULL != p_tag_list)
        {
            /*Search tag_list for the parameter
              if found then return true and also
              return param_val*/
            p_tag = p_tag_list->tag;

            while (cnt < p_tag_list->count)
            {
                if (!icf_port_strcmp(p_tag->hdr_name.str, p_hdr_name))
                {
                    icf_port_strcpy(temp_addr.str, p_tag->tag_value.str);
                    p_str = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)temp_addr.str, "=",&p_rem_str_eql);
		    if(ICF_NULL == p_str)
		    {
			ret_val = ICF_FALSE;
			break;
		    }
	
                    if (icf_port_strcmp(p_param, p_str))
                    {
                        if (ICF_NULL != p_rem_str_eql)
                        {
                            icf_port_strcpy(p_param_val,(icf_uint8_t *) p_rem_str_eql);
                            ret_val = ICF_TRUE;
                            break;
                        }
                    }
                }
                p_tag = p_tag->next;
                cnt++;
            }
        }
    }

    return ret_val;
}

ICF_4_0_CRM_END
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_tokenize_string_list
 *
 * DESCRIPTION:     This function takes a string list as input and breaks up 
 *                  all the comma separated entries into single string nodes
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cmn_tokenize_string_list(
                icf_glb_pdb_st       *p_glb_pdb,
                icf_string_list_st   *p_str_list)
{
    icf_return_t                     ret_val= ICF_SUCCESS;
    icf_error_t                      ecode = ICF_ERROR_NONE;
    icf_string_list_node_st          *p_new_node = ICF_NULL,
                                        *p_cur_node = ICF_NULL;
    icf_uint8_t                      count = ICF_NULL;
    icf_uint8_t                      *p_ch = ICF_NULL;

    for(count = (icf_uint8_t)p_str_list->count,p_cur_node = p_str_list->p_list;
    count--;
    p_cur_node= p_cur_node->p_next)
    {
        while((p_ch = icf_port_strchr(p_cur_node->str.str,',')))
        {
            *p_ch = '\0';
            p_ch++;

			/* fix - strlen was not getting updated */
			p_cur_node->str.str_len = (icf_uint16_t)icf_port_strlen(p_cur_node->str.str);

            ICF_MEMGET(
                p_glb_pdb,
                ICF_PORT_SIZEOF(icf_string_list_node_st),
                ICF_MEM_COMMON,
                p_new_node,
                ICF_RET_ON_FAILURE,
                &ecode,
                ret_val)
            p_new_node->p_next = p_cur_node->p_next;
            p_cur_node->p_next = p_new_node;
            icf_port_strcpy(p_new_node->str.str,p_ch);
            p_new_node->str.str_len = (icf_uint16_t)icf_port_strlen(p_new_node->str.str);
            p_cur_node = p_new_node;
            p_str_list->count ++;
        }
    }
    return ICF_SUCCESS;
} /* End icf_cmn_tokenize_string_list() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_strlist_from_hdrlist
 *
 * DESCRIPTION:     This function takes a header list and a header name as input
 *                  and returns a string list corresponding to that header name 
 *                  from the header list. It also breaks up all the comma 
 *                  separated entries in the string list into single string 
 *                  nodes before returning it to the calling function.
 *                  CAUTION: In case a node corresponding to the input header 
 *                  name is found, this function will delete that node from the 
 *                  passed header list. This will be useful so that the same 
 *                  header is not inserted twice in SIP message, one from 
 *                  SIP profile and other while copying header list.
 *
 * RETURNS:         nothing 
 *
 ******************************************************************************/
icf_return_t icf_cmn_get_strlist_from_hdrlist(
                icf_glb_pdb_st       *p_glb_pdb,
                icf_header_list_st   *p_hdr_list,
                icf_uint8_t          *p_hdr_name,
                icf_string_list_st   **p_p_str_list)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_error_t                      ecode = ICF_ERROR_NONE;
	/* this variable denotes the number of nodes corresponding to p_hdr_name in 
	 * the p_hdr_list. This is needed because a header list can have multiple 
	 * nodes with the same header name */
    icf_header_st                    **p_p_cur_hdr_node = ICF_NULL;
    icf_header_st                    *p_del_hdr_node= ICF_NULL;
    icf_string_list_st              temp_str_list;
    icf_string_list_node_st         **p_p_cur_str_node;
    icf_uint8_t                     hdr_count = ICF_NULL;


    ICF_FUNCTION_ENTER(p_glb_pdb) 

    *p_p_str_list    = ICF_NULL;
    p_p_cur_hdr_node = &(p_hdr_list->hdr);
    hdr_count = (icf_uint8_t)p_hdr_list->count;
    p_p_cur_str_node = &(temp_str_list.p_list);
    temp_str_list.p_list = ICF_NULL;
    temp_str_list.count = 0; 
    
    while((hdr_count)&&(ICF_NULL  != *p_p_cur_hdr_node))
    {
        if (0 == icf_port_strcmp(p_hdr_name, (*p_p_cur_hdr_node)->hdr_name.str))
        {
            /* Make new string node */
            ICF_MEMGET(
                p_glb_pdb,
                ICF_PORT_SIZEOF(icf_string_list_node_st),
                ICF_MEM_COMMON,
                *p_p_cur_str_node,
                ICF_RET_ON_FAILURE,
                &ecode,
                ret_val)
                
            (*p_p_cur_str_node)->p_next = ICF_NULL;

            icf_port_strcpy(
                (*p_p_cur_str_node)->str.str,
                (*p_p_cur_hdr_node)->hdr_value.str);
            (*p_p_cur_str_node)->str.str_len = (*p_p_cur_hdr_node)->hdr_value.str_len;
            temp_str_list.count++;

            p_p_cur_str_node = &((*p_p_cur_str_node)->p_next);
            
            /* Delete the current node from the header list */
            p_del_hdr_node = *p_p_cur_hdr_node;
            (*p_p_cur_hdr_node) = (*p_p_cur_hdr_node)->next;

            ICF_MEMFREE(
                p_glb_pdb,
                p_del_hdr_node, 
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
            p_hdr_list->count--;
        }
        else
        {
            /* Move to the next node */
            p_p_cur_hdr_node = &((*p_p_cur_hdr_node)->next);
        }

        hdr_count--;
    }

	if (0 != temp_str_list.count)
	{
        ICF_MEMGET(
            p_glb_pdb,
            ICF_PORT_SIZEOF(icf_string_list_st),
            ICF_MEM_COMMON,
            *p_p_str_list,
            ICF_RET_ON_FAILURE,
            &ecode,
            ret_val)
        (*p_p_str_list)->count = temp_str_list.count;
        (*p_p_str_list)->p_list = temp_str_list.p_list;


        /* tokenize the string list */
        icf_cmn_tokenize_string_list(p_glb_pdb, *p_p_str_list);
	}

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

} /* End icf_cmn_get_strlist_from_hdrlist() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_string_list
 *
 * DESCRIPTION:     This function frees a string list structure
 *
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_string_list(
                icf_glb_pdb_st	    *p_glb_pdb,
                icf_string_list_st   *p_str_list,
                icf_error_t          *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_string_list_node_st          *p_cur_node = ICF_NULL,
                                        *p_next_node = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_cur_node = p_str_list->p_list;

    if(ICF_NULL  == p_cur_node)
    {
	return ICF_SUCCESS;
    }
 
    while((p_str_list->count--) && (ICF_NULL != p_cur_node))
    {
        p_next_node = p_cur_node->p_next;
        
        ICF_MEMFREE(
            p_glb_pdb, 
            p_cur_node,
            ICF_MEM_COMMON,
            p_ecode, 
            ret_val)

        p_cur_node = p_next_node;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* End icf_cmn_free_string_list() */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_apiProcFailure 
*
*  DESCRIPTION    :  This function is used to fill 
*					 pcor_failure_st structure for 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t     icf_cmn_fill_apiProcFailure
(
        INOUT	icf_glb_pdb_st			      *p_glb_pdb,
        INOUT	icf_void_t					  *p_recv_payload,
		INOUT	icf_api_proc_failure_st        *p_resp_payload,
        IN		icf_api_id_t					   api_id,    
        IN      icf_error_t					   error_cause
)
{

	icf_delete_res_req_st		*p_delete_res_req = ICF_NULL;
	icf_fetch_res_req_st			*p_fetch_res_req = ICF_NULL;
	icf_syncml_send_req_st 		*p_syncml_send_req = ICF_NULL;

    if(ICF_NULL == p_glb_pdb)
    {
       return;
    }
	
	p_resp_payload->error_cause = error_cause;
	p_resp_payload->api_id = api_id;

	/* Unless the exchange Id and connection handle can
	 * be used from the request payload, the following
	 * code can be called useless */
	switch (api_id)
	{
	case ICF_XCAP_DELETE_RES:
		p_delete_res_req = (icf_delete_res_req_st *)
								(p_recv_payload);
        p_resp_payload->exchange_id =
                    p_delete_res_req->exchange_id;
		
		break;
    case ICF_XCAP_FETCH_RES:
		p_fetch_res_req = (icf_fetch_res_req_st *)
								(p_recv_payload);
        p_resp_payload->exchange_id =
                    p_fetch_res_req->exchange_id;
		break;
    case ICF_SYNCML_SEND_REQ:
		p_syncml_send_req = (icf_syncml_send_req_st *)
								(p_recv_payload);
        p_resp_payload->exchange_id =
                    p_syncml_send_req->exchange_id;

		break;		
	}/* switch on api id ends here */



}/* Function icf_cmn_fill_apiProcFailure() ends here */


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_register_cfm 
*
*  DESCRIPTION    :  This function is used to fill 
*					 register_cfm structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t     icf_cmn_fill_register_cfm
(
        INOUT	icf_glb_pdb_st			      *p_glb_pdb,
        INOUT	icf_register_req_st			  *p_register_req,
		INOUT	icf_register_cfm_st		      *p_register_cfm,
        IN		icf_return_t					   result,    
        IN      icf_error_t					   error_cause
)					
{
	icf_error_t		ecode = ICF_NULL;
	
	   
    if ((ICF_NULL != p_register_req) &&
		(ICF_NULL != p_register_cfm))
	{
		p_register_cfm->bit_mask = ICF_NULL;
		p_register_cfm->result = result;
		p_register_cfm->line_id =
					p_register_req->line_id;

		if (ICF_REG_REQ_TO_PRESENT & 
				p_register_req->bit_mask)
		{
			icf_port_memcpy(
				(icf_void_t *)&(p_register_cfm->to),
				(icf_void_t *)&(p_register_req->to),
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_address_st));
			p_register_cfm->bit_mask |= 
					ICF_REG_CFM_TO_PRESENT;			
					
		}

		if (ICF_NULL != error_cause)
		{
			p_register_cfm->error_cause =
							error_cause;
			p_register_cfm->bit_mask |= 
				ICF_REG_CFM_ERROR_CAUSE_PRESENT;
		}
		if ((ICF_HDR_LIST_PRESENT & 
					p_register_req->bit_mask) &&
				(ICF_SUCCESS == icf_cmn_clone_header_list(
                                        p_glb_pdb,
                                        &(p_register_req->header_list),
                                        &(p_register_cfm->header_list),
                                        &ecode)))
		{
			p_register_cfm->bit_mask |= 
					ICF_HDR_LIST_PRESENT;

		}
		if ((ICF_BODY_LIST_PRESENT &
				p_register_req->bit_mask) &&
				(ICF_SUCCESS == icf_cmn_clone_body_list(
                                p_glb_pdb, 
                                &(p_register_req->body_list),
                                &(p_register_cfm->body_list),
                                &ecode)))
		{
			p_register_cfm->bit_mask |= 
					ICF_BODY_LIST_PRESENT;
			
		}

	}/* Non null payloads: ends here */

    
}/* Function icf_cmn_fill_register_cfm() ends here */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_subs_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 subscription status ind structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_subs_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_subscribe_req_st			*p_subs_req,
		INOUT	icf_subscription_status_ind_st	*p_subs_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause)
{

	icf_error_t		ecode = ICF_NULL;
	
	   
    if ((ICF_NULL != p_subs_req) &&
		(ICF_NULL != p_subs_resp))
	{
		p_subs_resp->bit_mask = ICF_NULL;
		p_subs_resp->duration =
					p_subs_req->duration;
	
		if (ICF_FAILURE == result)
		{
			p_subs_resp->status = 
					ICF_SUBS_FAIL_NO_RETRY;
		}
		else
		{/* will not come in this code leg
		  * as this function is currently being used
		  * only for Failure */
			p_subs_resp->status = 
					ICF_SUBS_ACCEPTED;
		}
		if (ICF_NULL != error_cause)
		{
			p_subs_resp->error_code =
							error_cause;
			p_subs_resp->bit_mask |= 
				ICF_SUBS_ST_IND_ERROR_CODE_PRESENT;
		}
		if ((ICF_HDR_LIST_PRESENT & 
					p_subs_req->bit_mask) &&
				(ICF_SUCCESS == icf_cmn_clone_header_list(
                                        p_glb_pdb,
                                        &(p_subs_req->header_list),
                                        &(p_subs_resp->header_list),
                                        &ecode)))
		{
			p_subs_resp->bit_mask |= 
					ICF_HDR_LIST_PRESENT;

		}

	}/* Non null payloads: ends here */

    
}/* Function icf_cmn_fill_subs_resp() ends here */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_message_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 icf_message_resp_st structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_message_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_message_req_st				*p_message_req,
		INOUT	icf_message_resp_st				*p_message_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause)
{
	
	icf_error_t		ecode = ICF_NULL;
	
	    
    if ((ICF_NULL != p_message_req) &&
		(ICF_NULL != p_message_resp))
	{
		p_message_resp->bit_mask = ICF_NULL;
		p_message_resp->result = result;			
		
		if (ICF_NULL != error_cause)
		{
			p_message_resp->error_code =
							error_cause;
			p_message_resp->bit_mask |= 
				ICF_MSG_RESP_ERROR_CODE_PRESENT;
		}
		if ((ICF_HDR_LIST_PRESENT & 
					p_message_req->bit_mask) &&
				(ICF_SUCCESS == icf_cmn_clone_header_list(
                                        p_glb_pdb,
                                        &(p_message_req->header_list),
                                        &(p_message_resp->header_list),
                                        &ecode)))
		{
			p_message_resp->bit_mask |= 
					ICF_HDR_LIST_PRESENT;

		}

	}/* Non null payloads: ends here */

    
}/* Function icf_cmn_fill_message_resp() ends here */


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_publish_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 publish_resp structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_publish_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_publish_req_st				*p_publish_req,
		INOUT	icf_publish_resp_st				*p_publish_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause)
{
	icf_error_t		ecode = ICF_NULL;
	
	    
    if ((ICF_NULL != p_publish_req) &&
		(ICF_NULL != p_publish_resp))
	{
		p_publish_resp->bit_mask = ICF_NULL;
		p_publish_resp->result = result;	
		p_publish_resp->duration =
				p_publish_req->duration;
		
		if (ICF_NULL != error_cause)
		{
			p_publish_resp->error_code =
							error_cause;
			p_publish_resp->bit_mask |= 
				ICF_PUB_RESP_ERROR_CODE_PRESENT;
		}
		if ((ICF_HDR_LIST_PRESENT & 
					p_publish_req->bit_mask) &&
				(ICF_SUCCESS == icf_cmn_clone_header_list(
                                        p_glb_pdb,
                                        &(p_publish_req->header_list),
                                        &(p_publish_resp->header_list),
                                        &ecode)))
		{
			p_publish_resp->bit_mask |= 
					ICF_HDR_LIST_PRESENT;

		}

	}/* Non null payloads: ends here */

    
}/* Function icf_cmn_fill_publish_resp() ends here */


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_refer_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 refer_resp structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_refer_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_refer_req_st				*p_refer_req,
		INOUT	icf_refer_resp_st				*p_refer_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause)
{
	icf_error_t		ecode = ICF_NULL;
	
	    
    if ((ICF_NULL != p_refer_req) &&
		(ICF_NULL != p_refer_resp))
	{
		p_refer_resp->bit_mask = ICF_NULL;
		p_refer_resp->result = result;	
		
		if (ICF_NULL != error_cause)
		{
			p_refer_resp->error_code =
							error_cause;
			p_refer_resp->bit_mask |= 
				ICF_REF_RESP_ERROR_CODE_PRESENT;
		}
		if ((ICF_HDR_LIST_PRESENT & 
					p_refer_req->bit_mask) &&
				(ICF_SUCCESS == icf_cmn_clone_header_list(
                                        p_glb_pdb,
                                        &(p_refer_req->header_list),
                                        &(p_refer_resp->header_list),
                                        &ecode)))
		{
			p_refer_resp->bit_mask |= 
					ICF_HDR_LIST_PRESENT;

		}

	}/* Non null payloads: ends here */

    
}/* Function icf_cmn_fill_refer_resp() ends here */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_call_term_ind
*
*  DESCRIPTION    :  This function is used to fill 
*					 call_term_ind structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_call_term_ind(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_terminate_call_ind_st	*p_term_call_ind,		
		IN		icf_call_clear_reason_t		clear_reason)
{

    if(ICF_NULL == p_glb_pdb)
    {
       return;
    }

    

    if (ICF_NULL != p_term_call_ind)
    {
        p_term_call_ind->bit_mask = ICF_NULL;
        p_term_call_ind->call_clear_reason =
                        clear_reason;

    }/* non null payloads: ends here */

    

}/* Function icf_cmn_fill_call_term_ind() ends here */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_create_conn_resp
*
*  DESCRIPTION    :  This function is used to fill 
*					 create_conn_resp structure
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_create_conn_resp(
		INOUT	icf_glb_pdb_st			    	*p_glb_pdb,
		INOUT	icf_create_secure_conn_req_st	*p_create_conn_req,		
		INOUT   icf_create_secure_conn_resp_st	*p_create_conn_resp,
        IN      icf_result_t                    conn_req_status,
        IN      icf_error_t                     error_cause)
{

    if(ICF_NULL == p_glb_pdb)
    {
       return;
    }

    if ((ICF_NULL != p_create_conn_req) &&
            (ICF_NULL != p_create_conn_resp))
    {
        p_create_conn_resp->exchange_id =
                p_create_conn_req->exchange_id;
        p_create_conn_resp->conn_req_status =
                    conn_req_status;
        if (ICF_NULL != error_cause)
        {
        p_create_conn_resp->error_cause =
                    error_cause;
        }
    }/* Non null payloads: ends here */

}/* Function icf_cmn_fill_create_conn_resp() ends here */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_chk_n_free_lists 
*
*  DESCRIPTION    :  This function is used to free the lists 
*					 depending on the api for which the response
*					 payload has to be freed.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_chk_n_free_lists(
		INOUT	icf_glb_pdb_st	*p_glb_pdb,
		INOUT	icf_msg_st		*p_msg,		
		INOUT	icf_error_t		*p_ecode)
{
	icf_register_cfm_st		        *p_register_cfm = ICF_NULL;
    icf_subscription_status_ind_st  *p_subs_ind = ICF_NULL;
    icf_publish_resp_st             *p_publish_resp = ICF_NULL;
    icf_message_resp_st             *p_message_resp = ICF_NULL;
    icf_refer_resp_st               *p_refer_resp = ICF_NULL;
	icf_return_t				    ret_val = ICF_SUCCESS;
    icf_header_list_st              *p_hdr_list = ICF_NULL;
    icf_msg_body_list_st            *p_body_list = ICF_NULL;
    icf_boolean_t                   hdr_list = ICF_FALSE;
    icf_boolean_t                   body_list = ICF_FALSE;
    


	ICF_FUNCTION_ENTER(p_glb_pdb)
    
    switch (p_msg->hdr.api_id)
	{
	case ICF_REGISTER_CFM:
		p_register_cfm = (icf_register_cfm_st*)(
								p_msg->payload);
		if (ICF_BODY_LIST_PRESENT &
				p_register_cfm->bit_mask)
		{
            p_body_list = &(p_register_cfm->body_list);
            body_list = ICF_TRUE;	
		}
		if (ICF_HDR_LIST_PRESENT &
				p_register_cfm->bit_mask)
		{
            p_hdr_list = &(p_register_cfm->header_list);
            hdr_list = ICF_TRUE;
        }
		break;
    case ICF_SUBSCRIPTION_STATUS_IND:
        p_subs_ind = (icf_subscription_status_ind_st*)(
								p_msg->payload);
        	if (ICF_HDR_LIST_PRESENT &
				p_subs_ind->bit_mask)
            {
                p_hdr_list = &(p_subs_ind->header_list);
                hdr_list = ICF_TRUE;

            }


        break;
    case ICF_PUBLISH_RESP:
        p_publish_resp = (icf_publish_resp_st*)(
								p_msg->payload);
        if (ICF_HDR_LIST_PRESENT &
				p_publish_resp->bit_mask)
            {
                p_hdr_list = &(p_publish_resp->header_list);
                hdr_list = ICF_TRUE;

            }
        break;
    case ICF_MESSAGE_RESP:
        p_message_resp = (icf_message_resp_st*)(
								p_msg->payload);
        if (ICF_HDR_LIST_PRESENT &
				p_message_resp->bit_mask)
            {
                p_hdr_list = &(p_message_resp->header_list);
                hdr_list = ICF_TRUE;

            }
        break;
    case ICF_REFER_RESP:
        p_refer_resp = (icf_refer_resp_st*)(
								p_msg->payload);
        if (ICF_HDR_LIST_PRESENT &
				p_refer_resp->bit_mask)
            {
                p_hdr_list = &(p_refer_resp->header_list);
                hdr_list = ICF_TRUE;

            }
        break;

	}/* switch on api id ends here */

    if (ICF_TRUE == body_list)
    {
    ret_val = icf_cmn_free_msg_body_list(
                              p_glb_pdb,
	                          p_body_list,					
                               p_ecode);
    }
    if (ICF_TRUE == hdr_list)
    {
    	ret_val = icf_cmn_free_header_list(
                              p_glb_pdb,
							  p_hdr_list,
                              p_ecode);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

}/* function icf_cmn_chk_n_free_lists() ends here */



#ifdef ICF_NW_ACTIVATE
/*************************************************************
 *
 * FUNCTION:    icf_cmn_chk_gprs_status
 *
 * DESCRIPTION: This will be a utility function 
 *              offered by CFG towards other internal modules, CC,
 *              REGM and SIC.
 *              This function will be 
 *              invoked in all API handlers 
 *              wherein before creating the contexts , 
 *              this function will be invoked.
 *              The function parameter p_status will 
 *              contain the value ICF_FALSE if
 *              the GPRS state is suspended.
 *
 *  RETURNS:    Success/ Failure and the status of GPRS:
 *              False: Suspended; True: Active
 *
*******************************************************************/
icf_return_t	icf_cmn_chk_gprs_status(
            INOUT   icf_glb_pdb_st       *p_glb_pdb,
            INOUT   icf_boolean_t        *p_status,
            INOUT   icf_error_t          *p_ecode)
{

    
     icf_return_t                ret_val = ICF_SUCCESS;
     icf_config_data_st          *p_config_data = ICF_NULL;    


    ICF_FUNCTION_ENTER(p_glb_pdb)

     
    if (ICF_SUCCESS == 
        icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)
        ICF_INT_MODULE_CFG,
        (icf_void_t *)
        (&p_config_data),
        p_ecode))
    {
        
        if (ICF_FALSE == 
            p_config_data->gprs_suspended)
        {
            *p_status = ICF_TRUE;      
            
        }
        else
        {
            *p_status = ICF_FALSE;
        }
        ret_val = ICF_SUCCESS;
    }
    else
    {
        ret_val = ICF_FAILURE;
    }


    

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

}/* Function icf_cmn_chk_gprs_status () ends here */


/*********************************************************************
 *
 * FUNCTION:        icf_cmn_chk_api_for_gprs
 *
 * DESCRIPTION:     This function returns TRUE if the API id is any
 *                  one of those that need not be processed if GPRS
 *                  is suspended
 *
 * RETURNS:         ICF_TRUE/ ICF_FALSE 
 *
 ************************************************************************/
icf_boolean_t icf_cmn_chk_api_for_gprs(
            INOUT   icf_glb_pdb_st       *p_glb_pdb,
            INOUT   icf_msg_st           *p_msg,
            INOUT   icf_error_t          *p_ecode)
{
    icf_boolean_t  api_status = ICF_FALSE;
    icf_register_req_st     *p_register_req = ICF_NULL;
    icf_subscribe_req_st    *p_subs_req = ICF_NULL;

    /* To remove warnings */
    p_glb_pdb=p_glb_pdb;
    p_ecode=p_ecode;

    switch (p_msg->hdr.api_id)
    {
    case ICF_REGISTER_REQ:
        p_register_req = (icf_register_req_st *)(p_msg->payload);
        if ((ICF_REG_REQ_EXPIRY_PRESENT & p_register_req->bit_mask) &&
                (ICF_NULL == p_register_req->expiry))
                api_status = ICF_FALSE;
        else
            api_status = ICF_TRUE;
        break;    

    case ICF_SUBSCRIBE_REQ:
        p_subs_req = (icf_subscribe_req_st *)(p_msg->payload);
        if (ICF_NULL == p_subs_req->duration)
                api_status = ICF_FALSE;
        else
            api_status = ICF_TRUE;
        break;
    case ICF_CREATE_CALL_REQ:
    case ICF_PUBLISH_REQ:
    case ICF_MESSAGE_REQ:
    case ICF_REFER_REQ:
    case ICF_INFO_REQ:
    case ICF_CALL_TRANSFER_ATTENDED_REQ:
    case ICF_CONFERENCE_REQ:
    case ICF_CALL_TRANSFER_UNATTENDED_REQ:
    case ICF_CREATE_SECURE_CONN_REQ:
    case ICF_XCAP_DELETE_RES:
    case ICF_XCAP_FETCH_RES:
    case ICF_SYNCML_SEND_REQ:
             /* SPR: 10952 fix */
    case ICF_CALL_HOLD_REQ:
    case ICF_CALL_RESUME_REQ:
        api_status = ICF_TRUE;
        break;        
    default:
        api_status = ICF_FALSE;
        break;

    }/* switch ends */


    return api_status;

}/* Function  icf_cmn_chk_api_for_gprs() ends here */


/*******************************************************************
 *
 * FUNCTION:        icf_cmn_handle_gprs_fail
 *
 * DESCRIPTION:     This function returns send response to any API
 *                  that need not be processed if GPRS
 *                  is suspended. The status is FAILURE and error_cause
 *                  is ICF_CAUSE_GPRS_SUSPENDED.
 *
 * RETURNS:         ICF_SUCCESS/ ICF_FAILURE
 *
 ********************************************************************/
icf_return_t icf_cmn_handle_gprs_fail(
            INOUT   icf_glb_pdb_st       *p_glb_pdb,
            INOUT   icf_msg_st           *p_msg,
            INOUT   icf_error_t          *p_ecode)
{
	 icf_return_t                ret_val = ICF_SUCCESS;
     icf_return_t                dummy_ret_val = ICF_SUCCESS;
	 icf_api_id_t				api_id = ICF_NULL;
	 icf_msg_st					*p_msg_resp = ICF_NULL;
	 icf_uint32_t				api_len = ICF_NULL;
     icf_internal_msg_st            internal_msg;

	

    ICF_FUNCTION_ENTER(p_glb_pdb)

	api_id = p_msg->hdr.api_id;

	/* switch case to find the api_len */
	switch (api_id)
	{	
	case ICF_CALL_TRANSFER_ATTENDED_REQ:
          api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_cta_resp_st);
          break;
    case ICF_CONFERENCE_REQ:
    case ICF_INFO_REQ:
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_msg_resp_st);
		
		break;
    case ICF_CALL_TRANSFER_UNATTENDED_REQ:
          api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_ctu_resp_st);
          break;
            /* SPR: 10952 fix */
    case ICF_CALL_HOLD_REQ:        
    case ICF_CALL_RESUME_REQ:  
          /* get the size of external msg */
        api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
            ICF_PORT_SIZEOF(icf_api_resp_st);
        break;

		
	case ICF_XCAP_DELETE_RES:
    case ICF_XCAP_FETCH_RES:
    case ICF_SYNCML_SEND_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_api_proc_failure_st);
		break;
	case ICF_REGISTER_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_register_cfm_st);
		break;
	case ICF_CREATE_CALL_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_cc_pa_terminate_call_ind_st);
		break;
	case ICF_SUBSCRIBE_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_subscription_status_ind_st);
		break;
    case ICF_PUBLISH_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_publish_resp_st);
		break;
    case ICF_MESSAGE_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_message_resp_st);
		break;
    case ICF_REFER_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
					ICF_PORT_SIZEOF(icf_refer_resp_st);
		break;
	case ICF_CREATE_SECURE_CONN_REQ:
		api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
				ICF_PORT_SIZEOF(icf_create_secure_conn_resp_st);
		break;
	default:
		ret_val = ICF_FAILURE;
		break;

	}/* switch(for api_len) on API id ends here */

	if (ICF_FAILURE != ret_val)
	{
		/* allocate memory to p_msg */
        

		ICF_MEMGET(
		p_glb_pdb,
		api_len,
		ICF_MEM_COMMON,
		(icf_void_t *)p_msg_resp,
		ICF_RET_ON_FAILURE,
		p_ecode,
		ret_val)
	}
	else
	{
		p_msg = ICF_NULL;
	}

	if (ICF_NULL != p_msg)
	{
		/* some values of the header are common
		 * to all APIs */
		
		p_msg_resp->hdr.api_length = api_len;		
		p_msg_resp->hdr.call_id = p_msg->hdr.call_id;
		p_msg_resp->hdr.app_id = p_msg->hdr.app_id;
		p_msg_resp->hdr.version = ICF_VERSION_1_0;
		p_msg_resp->hdr.destination_id = ICF_MODULE_PA;


		/* switch case to make the responses */
		switch (api_id)
		{
		case ICF_INFO_REQ:
            	icf_cmn_fill_resp_msg(p_glb_pdb, p_msg_resp,
						ICF_MODULE_PA,ICF_INFO_RESP,ICF_FAILURE,
						ICF_CAUSE_GPRS_SUSPENDED); 
                break;
		case ICF_CALL_TRANSFER_ATTENDED_REQ:
            icf_cmn_fill_resp_msg(p_glb_pdb, p_msg_resp,
						ICF_MODULE_PA,ICF_CALL_TRANSFER_ATTENDED_RESP,
                        ICF_FAILURE,
					ICF_CAUSE_GPRS_SUSPENDED);		
                 p_msg_resp->hdr.api_length = api_len;
			break;

		case ICF_CONFERENCE_REQ:
            icf_cmn_fill_resp_msg(p_glb_pdb, p_msg_resp,
						ICF_MODULE_PA,ICF_CONFERENCE_RESP,ICF_FAILURE,
						ICF_CAUSE_GPRS_SUSPENDED);			
			break;
		case ICF_CALL_TRANSFER_UNATTENDED_REQ:
            icf_cmn_fill_resp_msg(p_glb_pdb, p_msg_resp,
						ICF_MODULE_PA,ICF_CALL_TRANSFER_UNATTENDED_RESP,
                        ICF_FAILURE,
						ICF_CAUSE_GPRS_SUSPENDED);	
                 p_msg_resp->hdr.api_length = api_len;
			break;
            /* SPR: 10952 fix */
    case ICF_CALL_HOLD_REQ:
        icf_cmn_fill_resp_msg(p_glb_pdb, p_msg_resp,
						ICF_MODULE_PA,ICF_CALL_HOLD_RESP,
                        ICF_FAILURE,
						ICF_CAUSE_GPRS_SUSPENDED);		
                 p_msg_resp->hdr.api_length = api_len;

        break;
    case ICF_CALL_RESUME_REQ:
        icf_cmn_fill_resp_msg(p_glb_pdb, p_msg_resp,
						ICF_MODULE_PA,ICF_CALL_RESUME_RESP,
                        ICF_FAILURE,
						ICF_CAUSE_GPRS_SUSPENDED);	
                p_msg_resp->hdr.api_length = api_len;
        break;
		case ICF_XCAP_DELETE_RES:
		case ICF_XCAP_FETCH_RES:
		case ICF_SYNCML_SEND_REQ:
            p_msg_resp->hdr.api_id= ICF_MGMT_PLANE_API_PROC_FAILURE;
			icf_cmn_fill_apiProcFailure(p_glb_pdb, 
					(icf_void_t *)(p_msg->payload), 
					(icf_api_proc_failure_st *)(p_msg_resp->payload),
					api_id, ICF_CAUSE_GPRS_SUSPENDED);			
			break;
		case ICF_REGISTER_REQ:
            p_msg_resp->hdr.api_id = ICF_REGISTER_CFM;
			icf_cmn_fill_register_cfm(p_glb_pdb, 
					(icf_register_req_st *)(p_msg->payload), 
					(icf_register_cfm_st *)(p_msg_resp->payload),
					ICF_FAILURE, ICF_CAUSE_GPRS_SUSPENDED);			
			break;
		case ICF_CREATE_CALL_REQ:
            p_msg_resp->hdr.api_id = ICF_TERMINATE_CALL_IND;
            icf_cmn_fill_call_term_ind(p_glb_pdb,
                (icf_terminate_call_ind_st *)(p_msg_resp->payload),
                ICF_CALL_CLEAR_REASON_GPRS_SUSPENDED);			
			break;
		case ICF_SUBSCRIBE_REQ:			
            p_msg_resp->hdr.api_id = ICF_SUBSCRIPTION_STATUS_IND;
            icf_cmn_fill_subs_resp(p_glb_pdb,
                (icf_subscribe_req_st *)(p_msg->payload),
                (icf_subscription_status_ind_st *)(p_msg_resp->payload),
                ICF_FAILURE,
                ICF_CAUSE_GPRS_SUSPENDED);
			break;
		case ICF_PUBLISH_REQ:			
            p_msg_resp->hdr.api_id = ICF_PUBLISH_RESP;
            icf_cmn_fill_publish_resp(p_glb_pdb,
                (icf_publish_req_st *)(p_msg->payload),
                (icf_publish_resp_st *)(p_msg_resp->payload),
                ICF_FAILURE,
                ICF_CAUSE_GPRS_SUSPENDED);
            break;
		case ICF_MESSAGE_REQ:	
            p_msg_resp->hdr.api_id = ICF_MESSAGE_RESP;
            icf_cmn_fill_message_resp(p_glb_pdb,
                (icf_message_req_st *)(p_msg->payload),
                (icf_message_resp_st *)(p_msg_resp->payload),
                ICF_FAILURE,
                ICF_CAUSE_GPRS_SUSPENDED);
			break;
		case ICF_REFER_REQ:			
            p_msg_resp->hdr.api_id = ICF_REFER_RESP;
            icf_cmn_fill_refer_resp(p_glb_pdb,
                (icf_refer_req_st *)(p_msg->payload),
                (icf_refer_resp_st *)(p_msg_resp->payload),
                ICF_FAILURE,
                ICF_CAUSE_GPRS_SUSPENDED);
			break;
		case ICF_CREATE_SECURE_CONN_REQ:
            p_msg_resp->hdr.api_id = ICF_CREATE_SECURE_CONN_RESP;
            icf_cmn_fill_create_conn_resp(p_glb_pdb,
                (icf_create_secure_conn_req_st *)(p_msg->payload),
                (icf_create_secure_conn_resp_st *)(p_msg_resp->payload),
                ICF_FALSE,
                ICF_CAUSE_GPRS_SUSPENDED);			
			break;
		default:
			ret_val = ICF_FAILURE;
			break;
			
		}/* switch(for response making) on API id ends here */
      
        /* Form an internal message with p_msg_resp as p_msg_data*/
         internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;
         internal_msg.msg_hdr.msg_id = p_msg_resp->hdr.api_id;
         internal_msg.p_msg_data = (icf_uint8_t *)p_msg_resp;
         /* invoke ADM process message */
         ret_val = icf_adm_process_mesg(&internal_msg);
         
        /* Free the lists if any from the response message structure*/
        icf_cmn_chk_n_free_lists(p_glb_pdb, p_msg_resp,p_ecode);
        
        /* Free the message sent as response to application */
        ICF_MEMFREE(
            p_glb_pdb,
            p_msg_resp,
            ICF_MEM_COMMON,
            p_ecode,
            dummy_ret_val)

	}/* non null p_msg: ends here */
    else
    {
        ret_val = ICF_FAILURE;
    }

	ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;


}/* Function icf_cmn_handle_gprs_fail() ends here */
#endif/* NW_ACTIVATE */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_api_resp 
*
*  DESCRIPTION    :  This function is used to fill common response structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t     icf_cmn_fill_api_resp
(
        INOUT        icf_glb_pdb_st        *p_glb_pdb,
        INOUT        icf_msg_st            *p_msg,
        IN           icf_module_id_t       dest_id,
        IN           icf_api_id_t          resp_api_id,
        IN           icf_result_t          result,
        IN           icf_error_t           error_cause,
        INOUT        icf_error_t           *p_ecode
)
{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_api_resp_st       *p_api_resp = ICF_NULL;
    icf_list_st           *p_temp_list_node =  ICF_NULL;
    /*API-ID to send */
    p_msg->hdr.api_id = resp_api_id;

    /*Source module is ICF*/
    p_msg->hdr.source_id = ICF_MODULE_ICF;

    /*Destination is the module to which response targetted*/
    p_msg->hdr.destination_id = dest_id;

    /*Payload length is size of api header + response structure*/
    p_msg->hdr.api_length = ICF_PORT_SIZEOF(icf_api_header_st) +
        ICF_PORT_SIZEOF(icf_api_resp_st);

    if((ICF_NULL != p_glb_pdb->p_call_ctx))
    {
        /*Call ID from glb_pdb*/
        p_msg->hdr.call_id = p_glb_pdb->p_call_ctx->call_id;
        p_msg->hdr.app_id = p_glb_pdb->p_call_ctx->app_id;
    }
    else if (ICF_NULL != p_glb_pdb->p_recv_msg)
    {
        p_msg->hdr.call_id = p_glb_pdb->p_recv_msg->hdr.call_id;	
        p_msg->hdr.app_id = p_glb_pdb->p_recv_msg->hdr.app_id;	
    }
    else
    {
        p_msg->hdr.call_id = ICF_INVALID_CALL_ID;
        p_msg->hdr.app_id = ICF_INVALID_APP_ID;
    }

    p_api_resp = (icf_api_resp_st *)&(p_msg->payload[0]);
    /*Result of request received success or failure*/
    p_api_resp->result = result;

    /*error_cause, valid if result is failure*/
    p_api_resp->error_cause = error_cause;

    if(ICF_NULL != p_glb_pdb->p_call_ctx)
    {
    /* clone the header list */
        /*SPR 19590 change start*/
        ret_val = icf_cmn_fetch_curr_hdr_list_node_from_call_ctx(
                     p_glb_pdb,
                     &p_temp_list_node);
        /*SPR 19590 change end*/

        if((ICF_NULL != p_temp_list_node) &&
           (ICF_NULL != p_temp_list_node->p_data) &&     
           (ICF_NULL != ((icf_header_link_list_st*)(p_temp_list_node->p_data))
                ->header_list.count))
        {
            if (ICF_SUCCESS == icf_cmn_clone_header_list(
                p_glb_pdb,
                &(((icf_header_link_list_st*)(p_temp_list_node->p_data))->header_list),
                &(p_api_resp->header_list),
                p_ecode))
            {
                p_api_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
                /*SPR 19590 change start*/
                /* free the list in call ctx */
                ret_val = icf_cmn_del_header_list_node_in_call_ctx(
                            p_glb_pdb,
                            p_temp_list_node);
                /*SPR 19590 change end*/
            }
            else
            {
                ret_val = ICF_FAILURE;
            }
        }
    else{
        p_api_resp->header_list.hdr = ICF_NULL;
        p_api_resp->header_list.count = 0;
    }
    /* clone the body list */
    /*SPR 19590 change start*/
    ret_val = icf_cmn_fetch_curr_body_list_node_from_call_ctx(
                     p_glb_pdb,
                     &p_temp_list_node);
    /*SPR 19590 change end*/
    if ((ICF_SUCCESS == ret_val) &&
        (ICF_NULL != p_temp_list_node) && 
        (ICF_NULL != p_temp_list_node->p_data) &&
        (ICF_NULL != ((icf_msg_body_link_list_st*)(p_temp_list_node->p_data))
                ->body_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_body_list(
            p_glb_pdb,
            &(((icf_msg_body_link_list_st*)(p_temp_list_node->p_data))->body_list),
            &(p_api_resp->body_list),
            p_ecode))
        {
            p_api_resp->bit_mask |= ICF_BODY_LIST_PRESENT;
            
            /*SPR 19590 change start*/
            /* free the list in call ctx */
            ret_val = icf_cmn_del_body_list_node_in_call_ctx(
                            p_glb_pdb,
                            p_temp_list_node);
            /*SPR 19590 change end*/
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_api_resp->body_list.body = ICF_NULL;
        p_api_resp->body_list.count = 0;
    }
   /*SPR 19835 reset the txn_id in call ctx as the node is deleted for
    * the header/body list.*/
   ICF_RESET_TXN_ID_IN_CALL_CTX(p_glb_pdb)
#ifdef ICF_REL_OLD    
    /* clone the stream list */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_NULL != p_glb_pdb->p_call_ctx->stream_list.count))
    {
        if (ICF_SUCCESS == icf_cmn_clone_stream_list(
            p_glb_pdb,
            &(p_glb_pdb->p_call_ctx->stream_list),
            &(p_api_resp->stream_list),
            p_ecode))
        {
            p_api_resp->bit_mask |= ICF_STREAM_LIST_PRESENT;
            
            /* free the list in call ctx */
            ret_val = icf_cmn_free_stream_list(p_glb_pdb,
                &(p_glb_pdb->p_call_ctx->stream_list),
                p_ecode);
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else{
        p_api_resp->stream_list.count = 0;
    }
#endif
   }

    return ret_val;

} /* icf_cmn_fill_resp_msg */


/* ICF REL 6.0: multiple m lines
 * Following are a set of functions that would be used to manipulate the 
 * generic list structures icf_internal_list_st and icf_list_st
 */
/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_internal_list_count 
*
*  DESCRIPTION    :  This function is used to get the count/size of the 
*					 list. 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_get_internal_list_count 
(
	INOUT	icf_internal_list_st	*p_list,
	INOUT	icf_uint8_t		*p_count
)
{
	/* local variable */
	icf_internal_list_st	*p_tmp_list = ICF_NULL;

	*p_count = 0;
	p_tmp_list = p_list;

	while( ICF_NULL != p_tmp_list )
	{
		p_tmp_list = p_tmp_list->p_next;
		*p_count += 1;
	}
	return ;
}

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_list_count 
*
*  DESCRIPTION    :  This function is used to get the count/size of the 
*					 list. 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_get_list_count 
(
	INOUT	icf_list_st		*p_list,
	INOUT	icf_uint8_t		*p_count
)
{
	/* local variable */
	icf_list_st	*p_tmp_list = ICF_NULL;

	*p_count = 0;
	p_tmp_list = p_list;
	while( ICF_NULL != p_tmp_list )
	{
		p_tmp_list = p_tmp_list->p_next;
		*p_count += 1;
	}
	return ;
}

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_add_node_to_internal_list 
*
*  DESCRIPTION    :  This function will be used to insert a node into the 
*					 list of the type icf_internal_list_st. The function will 
*					 always insert the node at the end of the list
*					 If the identifier is of the type icf_uint8_t/icf_uint16_t/
*					 icf_uint32_t, the calling function can pass the identifier
*					 and the comparison function ptr as NULL to this function.
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_add_node_to_internal_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	**p_p_list_start, 
									  /* incase we r adding the first node 
									   * in the list then we will be allocating
									   * memory for the node, that will be
									   * returned to the calling function
									   */
	INOUT		icf_void_t		*p_payload,
	INOUT		icf_void_t		*p_identifier,
	IN		icf_void_t		*p_func_ptr
)
{
	/* the function will allocate memory for the node and assign the node
	 * to the list
	 */
	/* local variable */
	icf_internal_list_st *p_tmp_list = ICF_NULL;
    icf_internal_list_st *p_pre_tmp_list = ICF_NULL;
	icf_internal_list_st *p_tmp_node = ICF_NULL;
	icf_return_t		 ret_val	= ICF_FAILURE;
	icf_error_t			 ecode = ICF_ERROR_NONE;


    /* SDP Tunnelling */
	ICF_FUNCTION_ENTER(p_glb_pdb)

	/* allocate memory for the node */
	ICF_MEMGET (p_glb_pdb,
				ICF_PORT_SIZEOF(icf_internal_list_st),
				ICF_MEM_COMMON,
				p_tmp_node,
				ICF_RET_ON_FAILURE,
				&ecode,
				ret_val)

	if (ICF_SUCCESS == ret_val)
	{
		p_tmp_node->p_payload = p_payload;
		if (ICF_NULL == p_func_ptr)
		{
			/* this function will allocate the req. memory for the identifier
		 	 * as per the id_type and then save the identifier in the node */
			icf_set_identifier_in_node(p_glb_pdb,
								   p_tmp_node,
								   p_identifier);
			p_tmp_node->func_ptr = icf_cmn_compare_int_identifier;
		}
		else
		{
		/* the calling module needs to pass the allocated memory for the
		 * identifier, just like the payload. Since the generic list function
		 * is not aware as to what is the identifier and what is the size of 
		 * the identifier */
			p_tmp_node->p_identifier = p_identifier;
			p_tmp_node->func_ptr = (icf_cmn_compare_identifier_fn_ptr_t)
				             p_func_ptr;
		}
			
		p_tmp_node->p_next = ICF_NULL;
        p_tmp_list = *p_p_list_start; /* Head of the list */

		/* if this is the first node of the list */
		if ( ICF_NULL == p_tmp_list )
		{
			(*p_p_list_start) = p_tmp_node;
		}
		else /* traverse to the end of the list to insert the node */
		{
			while (ICF_NULL != p_tmp_list)
			{
				p_pre_tmp_list = p_tmp_list;
                p_tmp_list = p_tmp_list->p_next;
			}
			p_pre_tmp_list->p_next = p_tmp_node;
		}
	}
    ICF_FUNCTION_EXIT(p_glb_pdb)
	return ret_val;
}	

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_add_node_to_list 
*
*  DESCRIPTION    :  This function will be used to insert a node into the 
*					 list of the type icf_list_st. The function will always
*					 insert the node at the end of the list
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_add_node_to_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_p_list_start, 
									  /* incase we r adding the first node 
									   * in the list then we will be allocating
									   * memory for the node, that will be
									   * returned to the calling function
									   */
	IN		icf_void_t		*p_payload
)
{
	/* the function will allocate memory for the node and assign the node
	 * to the list
	 */
	/* local variable */
	icf_list_st			*p_tmp_list = ICF_NULL;
    icf_list_st			*p_pre_tmp_list = ICF_NULL;
	icf_list_st			*p_tmp_node = ICF_NULL;
	icf_return_t		ret_val	= ICF_FAILURE;
	icf_error_t			ecode = ICF_ERROR_NONE;

	p_tmp_list = *p_p_list_start; /* Head of the list */
	
	/* allocate memory for the node */
	ICF_MEMGET (p_glb_pdb,
				ICF_PORT_SIZEOF(icf_list_st),
				ICF_MEM_COMMON,
				p_tmp_node,
				ICF_RET_ON_FAILURE,
				&ecode,
				ret_val)

	if (ICF_SUCCESS == ret_val)
	{
		p_tmp_node->p_data = (icf_void_t*)p_payload;
			
		p_tmp_node->p_next = ICF_NULL;
		p_tmp_list = *p_p_list_start;

		/* if this is the first node of the list */
		if ( ICF_NULL == p_tmp_list )
		{
			(*p_p_list_start) = p_tmp_node;
		}
		else /* traverse to the end of the list to insert the node */
		{
			while (ICF_NULL != p_tmp_list)
			{
				p_pre_tmp_list = p_tmp_list;
                p_tmp_list = p_tmp_list->p_next;
			}
			p_pre_tmp_list->p_next = p_tmp_node;
		}
	}
	return ret_val;
}	

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_node_frm_list 
*
*  DESCRIPTION    :  This function will be used to delete a node from the 
*					 list of the type icf_list_st. The function will return
*					 the pointer to the payload, so that the calling module
*					 can free that memory as the payload was allocated mem.
*					 by the calling module.
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t icf_cmn_delete_node_frm_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_list_start,
	INOUT	icf_void_t		**p_payload
)
{
	/* local variables */
	icf_list_st		*p_tmp_list = ICF_NULL,
					*p_prev_node = ICF_NULL;
	icf_error_t		ecode;
	icf_return_t	ret_val = ICF_FAILURE;
	

	p_tmp_list = *p_list_start;
	/* if the node is present in the function argument, it means that the
	 * calling function is aware of the payload node, so the list will be 
	 * traversed based on the node pointer value */
	if ( ICF_NULL != p_payload )
	{
		while ( ICF_NULL != p_tmp_list )
		{
			if (*p_payload == p_tmp_list->p_data)
			{
				break;
			}
			else
			{
				p_prev_node = p_tmp_list ;
				p_tmp_list = p_tmp_list->p_next;
			}
		}
	}

	/* if a match is found, then delete the node */
	if ( ICF_NULL != p_tmp_list )
	{
		/* if this is not the first/last node then we need to move the pointers
		 * here and remove this node from the list */ 
		if ((p_prev_node) &&
			(p_tmp_list->p_next))
		{
			p_prev_node->p_next = p_tmp_list->p_next;
		}
		else 
		if ((p_prev_node) &&
			(ICF_NULL == p_tmp_list->p_next))
		{
			/* if this is the last node and not the fist node of the list, then
			 * we need to set the pointer of the prev. node to NULL */
			p_prev_node->p_next = ICF_NULL;
		}
		else if (ICF_NULL == p_prev_node)
		{
			/* if this is the first node of the list then the list header
			 * needs to be updated */
			*p_list_start = p_tmp_list->p_next;
		}
		else if ((ICF_NULL == p_prev_node) &&
				(ICF_NULL == p_tmp_list->p_next))
		{
			/* if this is the only node in the list, then we need to make the
			 * list header NULL */
			*p_list_start = ICF_NULL;
		}

		/* return this payload, so that the calling function can delete this
		 * node. The calling module has allocated memory for the payload */
		if ( ICF_NULL != p_payload ) *p_payload = p_tmp_list->p_data;

		/* Now free the memory for the complete node */
		ICF_MEMFREE(p_glb_pdb,
					p_tmp_list,
					ICF_MEM_COMMON,
					&ecode,
					ret_val)
	}
	
	return ret_val;
}	
/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_internal_list 
*
*  DESCRIPTION    :  This function will be used to delete all nodes in the 
*					 list of the type icf_internal_list_st. This function will 
*					 also delete the payload in the list, based on the pool_id
*					 passed by the calling function/module
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_delete_internal_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	**p_list_start,
	IN		icf_pool_id_t	pool_id
)
{
	/* local variables */
	icf_internal_list_st	*p_hdr = ICF_NULL, 
							*p_tmp_list = ICF_NULL;
	icf_return_t			ret_val = ICF_FAILURE;
	icf_error_t				ecode;

	p_hdr = *p_list_start;
	/* free the elements in the list */
	while ( ICF_NULL != p_hdr )
	{
		p_tmp_list = p_hdr->p_next;
		/* free memory for the identifier. Currently we assume that the 
		 * identifier as allocated by the common list function*/
		ICF_MEMFREE(p_glb_pdb, 
					p_hdr->p_identifier, 
					ICF_MEM_COMMON, 
					&ecode, 
					ret_val)
		p_hdr->p_identifier = ICF_NULL;

        /* delete the payload from the pool id */
		ICF_MEMFREE(p_glb_pdb, 
					p_hdr->p_payload, 
					pool_id, 
					&ecode, 
					ret_val)
		p_hdr->p_payload = ICF_NULL;
		
		/* now free the node */
		ICF_MEMFREE(p_glb_pdb, 
					p_hdr, 
					ICF_MEM_COMMON, 
					&ecode, 
					ret_val)
		p_hdr = p_tmp_list;
	}
	
	*p_list_start = ICF_NULL;

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_list 
*
*  DESCRIPTION    :  This function will be used to delete all nodes in the 
*					 list of the type icf_list_st. This function will also
*					 delete the payload in the list, based on the pool_id
*					 passed by the calling function/module
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_delete_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_list_start,
	IN		icf_pool_id_t	pool_id
)
{
	/* local variables */
	icf_list_st			*p_hdr = ICF_NULL, 
						*p_tmp_list = ICF_NULL;
	icf_return_t		ret_val = ICF_FAILURE;
	icf_error_t			ecode;

	p_hdr = *p_list_start;
	/* free the elements in the list */
	while ( ICF_NULL != p_hdr )
	{
		p_tmp_list = p_hdr->p_next;

        /* delete the payload from the pool id */
		ICF_MEMFREE(p_glb_pdb, 
					p_hdr->p_data, 
					pool_id, 
					&ecode, 
					ret_val)
		p_hdr->p_data = ICF_NULL;
		
		/* now free the node */
		ICF_MEMFREE(p_glb_pdb, 
					p_hdr, 
					ICF_MEM_COMMON, 
					&ecode, 
					ret_val)
		p_hdr = p_tmp_list;
	}
	
	*p_list_start = ICF_NULL;

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_node_in_internal_list 
*
*  DESCRIPTION    :  This function will return a node from the list that 
*					 matches the identifier in the list node in a  
*					 list of the type icf_internal_list_st. The function returns
*					 the node in the list 
*
*  RETURNS        :
*
*************************************************************************/
icf_internal_list_st*	icf_cmn_get_node_in_internal_list 
(
	INOUT	icf_internal_list_st	*p_list_start,
	IN		icf_void_t				*p_identifier
)
{
	/* local variables */
	icf_internal_list_st	*p_tmp_node = ICF_NULL;
	
	
	p_tmp_node = p_list_start;

	while (ICF_NULL != p_tmp_node)
	{
		/* compare the identifier with the identifier in the node */
		if (ICF_SUCCESS == p_tmp_node->func_ptr (p_tmp_node,
											(icf_void_t*) p_identifier))
		{
			break;
		}
		else
		{
			p_tmp_node = p_tmp_node->p_next;
		}
	}

	return p_tmp_node;
}

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_replace_node_in_internal_list 
*
*  DESCRIPTION    :  This function will be used to replace an existing node 
*  					 with a new node. the calling function will give the new
*  					 node to be inserted in the list. Also the calling funct.
*  					 will delete the old node, as the calling function had
*  					 allocated memory for it. The old payload addrs will be 
*  					 returned in the p_old_payload ptr.
*  					 NOTE: Incase the calling function is passing a valid addr
*  					 in the old_payload ptr, at the time of calling this funct
*  					 it shud pass the p_identifier as ICF_NULL
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_replace_node_in_internal_list 
(
	INOUT	icf_internal_list_st	*p_list_start,
	INOUT	icf_void_t		**p_old_payload,
	INOUT	icf_void_t		*p_rep_payload,
	IN		icf_void_t		*p_identifier
)
{
	/* local variables */
	icf_internal_list_st		*p_hdr = ICF_NULL,
					*p_node = ICF_NULL;
	icf_return_t	ret_val = ICF_FAILURE;
	

	if (ICF_NULL != p_identifier)
	{
		/* we will use identifier as the index for fetching the node */
		p_node = (icf_internal_list_st *)icf_cmn_get_node_in_internal_list(
														p_list_start,
														p_identifier);

		if (ICF_NULL == p_node)
		{
			ICF_PRINT(((icf_uint8_t*)"Invalid Identifier\n"));
		}
		else
		{
			/* replace the old node payload with the new payload */
			*p_old_payload = p_node->p_payload;
			/* assign the new payload to the node */
			p_node->p_payload = p_rep_payload;
			/* the identifier will remain the same */

			ret_val = ICF_SUCCESS;
		}
	}
	else if (ICF_NULL != (*p_old_payload))
	{
		/* fetch the list for the node */
		p_hdr = p_list_start;
		while (ICF_NULL != p_hdr)
		{
			if (p_old_payload == p_hdr->p_payload)
			{
				/* the calling function/module needs to delete the pointer
				 * in the old payload pointer on return from the function */
				p_hdr->p_payload = p_rep_payload;
				ret_val = ICF_SUCCESS;
				break;
			}
			else
			{
				p_hdr = p_hdr->p_next;
			}
		}		
	}

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_next_node_in_internal_list 
*
*  DESCRIPTION    :  This function will be used to fetch the next node in the
*  					 list. The calling function will pass the payload of the
*  					 current node. This function will return the pointer to the
*  					 payload of the next node.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t*	icf_cmn_get_next_node_in_internal_list 
(
	INOUT	icf_internal_list_st	*p_list_start,
	IN		icf_void_t		*p_payload /* current payload */
)
{
	/* local variables */
	icf_internal_list_st			*p_hdr = ICF_NULL;
	icf_void_t			*p_node = ICF_NULL;

	p_hdr = p_list_start;

	while (ICF_NULL != p_hdr)
	{
		if (p_payload == p_hdr->p_payload)
		{
			p_hdr = p_hdr->p_next;
			/* if this is the last node of the list, it shall return NULL */
			if (p_hdr)
			{
				p_node = p_hdr->p_payload;
			}
			break;
		}
		else
		{
			p_hdr = p_hdr->p_next;
		}
	}

	/* the function will return the pointer to the payload of the next node
	 * in the list */
	return p_node;
}


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_next_node_in_list 
*
*  DESCRIPTION    :  This function will be used to fetch the next node in the
*  					 list. The calling function will pass the payload of the
*  					 current node. This function will return the pointer to the
*  					 payload of the next node.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t*	icf_cmn_get_next_node_in_list 
(
	INOUT	icf_list_st	*p_list_start,
	IN		icf_void_t		*p_payload /* current payload */
)
{
	/* local variables */
	icf_list_st			*p_hdr = ICF_NULL;
	icf_void_t			*p_node = ICF_NULL;

	p_hdr = p_list_start;

	while (ICF_NULL != p_hdr)
	{
		if (p_payload == p_hdr->p_data)
		{
			p_hdr = p_hdr->p_next;
			/* if this is the last node of the list, it shall return NULL */
			if (p_hdr)
			{
				p_node = p_hdr->p_data;
			}
			break;
		}
		else
		{
			p_hdr = p_hdr->p_next;
		}
	}

	/* the function will return the pointer to the payload of the next node
	 * in the list */
	return p_node;
}

/*************************************************************************
*
*  FUNCTION NAME  :  icf_set_identifier_in_node
*
*  DESCRIPTION    :  This function will allocate memory for the identifier
*					 and then save the identifier in the node as per the 
*					 type of the identifier.
*
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_set_identifier_in_node
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	*p_tmp_node,
	IN		icf_void_t		*p_identifier
)
{
	/* local variables */
	icf_uint8_t		size =0;
	icf_return_t	ret_val	= ICF_SUCCESS;
	icf_error_t		ecode;
	icf_uint32_t    *p_tmp_id = ICF_NULL;

	size = ICF_PORT_SIZEOF(icf_uint32_t);
	ICF_MEMGET (p_glb_pdb,
				size,
				ICF_MEM_COMMON,
				p_tmp_id,
				ICF_RET_ON_FAILURE,
				&ecode,
				ret_val)

	/* assign the variable to this new temp id */
	*p_tmp_id = *(icf_uint32_t*)p_identifier;
	p_tmp_node->p_identifier = p_tmp_id;
	
	return ret_val;
}


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_compare_int_identifier
*
*  DESCRIPTION    :  This utility function will compare with received 
*					 identifier with the identifier saved within the node. 
*					 This is a default comparison function to compare interger
*					 type identifier's
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_compare_int_identifier
(		
	INOUT	icf_void_t	*p_node,
	INOUT	icf_void_t	*p_identifier
)
{
	/* local variables */
	icf_return_t		ret_val = ICF_FAILURE;
	icf_internal_list_st *p_tmp = (icf_internal_list_st *) p_node;

	if (*(icf_uint32_t*)(p_identifier) == 
					*(icf_uint32_t*)(p_tmp->p_identifier))
	{
		ret_val = ICF_SUCCESS;
	}

	return ret_val;
}

/*SPR 13978 Aman 05-01-07*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_open_nw_server
 *
 * DESCRIPTION:     This fucntion checks that whether the sockets are open or 
 *                  not.It checks that if the call is on UDP, then UDP socket 
 *                  should be opened or if the call is on TCP, then TCP socket
 *                  should be opened.   
 *                  
 ******************************************************************************/
icf_return_t icf_cmn_validate_open_nw_server(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        INOUT   icf_line_id_t  line_id)
{
    icf_return_t ret_val = ICF_SUCCESS;
     icf_config_data_st *p_config_data = ICF_NULL;
     icf_line_data_st   *p_line_data = ICF_NULL;
     icf_error_t        ecode = ICF_ERROR_NONE;


     ICF_FUNCTION_ENTER(p_glb_pdb)

     ret_val = icf_dbm_get_module_glb_data(
                     p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, &ecode);

     if (ICF_CFG_SELF_ADDR_PER_LINE == p_config_data->self_addr_per_line)
     {
        icf_dbm_fetch_line_cntxt_blk(
                p_glb_pdb, line_id,
                &p_line_data, &ecode);

        if((p_line_data->self_mode == ICF_TRANSPORT_TYPE_UDP) &&
                (p_line_data->server_index[0] == ICF_MAX_NUM_OF_SERVERS))
        {
                ret_val = ICF_FAILURE;
        }
        else if((p_line_data->self_mode == ICF_TRANSPORT_TYPE_TCP) &&
                (p_line_data->server_index[1] == ICF_MAX_NUM_OF_SERVERS))
        {
                ret_val = ICF_FAILURE;
        }
        else if((p_line_data->self_mode == ICF_TRANSPORT_TYPE_TLS) &&
                (p_line_data->server_index[2] == ICF_MAX_NUM_OF_SERVERS))
        {
                ret_val = ICF_FAILURE;
        }
        else if(p_line_data->self_mode == ICF_TRANSPORT_MODE_BOTH)
        {
                if((p_line_data->server_index[0] == ICF_MAX_NUM_OF_SERVERS) &&
                        (p_line_data->server_index[1] == ICF_MAX_NUM_OF_SERVERS))
                {
                   ret_val = ICF_FAILURE;
                }
        }
     }
     else
     {
             if((p_config_data->self_mode == ICF_TRANSPORT_TYPE_UDP) &&
                (p_config_data->server_index[0] == ICF_MAX_NUM_OF_SERVERS))
             {
                     ret_val = ICF_FAILURE;
             }
             else if((p_config_data->self_mode == ICF_TRANSPORT_TYPE_TCP) &&
                (p_config_data->server_index[1] == ICF_MAX_NUM_OF_SERVERS))
             {
                     ret_val = ICF_FAILURE;
             }
             else if((p_config_data->self_mode == ICF_TRANSPORT_TYPE_TLS) &&
                        (p_config_data->server_index[2] == ICF_MAX_NUM_OF_SERVERS))
             {
                     ret_val = ICF_FAILURE;
             }
             else if(p_config_data->self_mode == ICF_TRANSPORT_MODE_BOTH)
             {
                     if((p_config_data->server_index[0] == ICF_MAX_NUM_OF_SERVERS) &&
                             (p_config_data->server_index[1] == ICF_MAX_NUM_OF_SERVERS))           
                     {
                             ret_val = ICF_FAILURE;
                     }
             }
     }

     ICF_FUNCTION_EXIT(p_glb_pdb)
     return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_send_pa_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is used to send an unexpected
 *                  event indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cmn_send_pa_unexpected_msg_ind(
              INOUT   icf_glb_pdb_st       *p_glb_pdb,
              INOUT   icf_msg_st           *p_msg,
              INOUT   icf_error_t          *p_ecode )
{
      icf_return_t                  ret_val = ICF_SUCCESS;
      icf_return_t                  dummy_ret_val = ICF_SUCCESS;
      icf_msg_st                    *p_msg_resp = ICF_NULL;
      icf_uint32_t                  api_len = ICF_NULL;
      icf_unexpected_msg_ind_st   *p_unexpected_msg_ind = ICF_NULL;
      icf_internal_msg_st           internal_msg;

      ICF_FUNCTION_ENTER(p_glb_pdb)
  
      api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
            ICF_PORT_SIZEOF(icf_unexpected_msg_ind_st);

       /* allocate memory to p_msg */

           ICF_MEMGET(
                     p_glb_pdb,
                     api_len,
                     ICF_MEM_COMMON,
                     p_msg_resp,
                     ICF_RET_ON_FAILURE,
                     p_ecode,
                     ret_val)
        
     if (ICF_NULL != p_msg && ICF_NULL != p_msg_resp)
       {
         p_msg_resp->hdr.api_id = ICF_UNEXPECTED_MSG_IND;
         p_msg_resp->hdr.api_length = (icf_uint8_t)api_len;
         p_msg_resp->hdr.call_id = p_msg->hdr.call_id;
         p_msg_resp->hdr.version = ICF_VERSION_1_0;
         p_msg_resp->hdr.app_id = p_msg->hdr.app_id;

         /* Fill the payload for ICF_UNEXPECTED_MSG_IND */
         p_unexpected_msg_ind = (icf_unexpected_msg_ind_st *)(p_msg_resp->payload);
         /* Initialize the error cause */
         p_unexpected_msg_ind->error_cause = *p_ecode;
         /* Filling in api_id ...... */
         p_unexpected_msg_ind->api_id = p_msg->hdr.api_id;
   
         /* Form an internal message with p_msg_resp as p_msg_data*/
         internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;
         internal_msg.msg_hdr.msg_id = p_msg_resp->hdr.api_id;
         internal_msg.msg_hdr.payload_length = (icf_uint8_t)api_len; 
         internal_msg.p_msg_data = (icf_uint8_t *)p_msg_resp;
         /* invoke ADM process message */
         ret_val = icf_adm_process_mesg(&internal_msg);

         /* Free the message sent as response to application */
          ICF_MEMFREE(
               p_glb_pdb,
               p_msg_resp,
               ICF_MEM_COMMON,
               p_ecode,
               dummy_ret_val)
       }
     else
       { 
         /* Free the message sent as response to application */
          ICF_MEMFREE(
               p_glb_pdb,
               p_msg_resp,
               ICF_MEM_COMMON,
               p_ecode,
               dummy_ret_val) 
         ret_val = ICF_FAILURE;
       }

     ICF_FUNCTION_EXIT(p_glb_pdb)
     return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_send_mmi_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is used to send an unexpected
 *                  event indication to the MMI
 *
 *
 ******************************************************************************/
icf_return_t icf_cmn_send_mmi_unexpected_msg_ind(
              INOUT   icf_glb_pdb_st       *p_glb_pdb,
              INOUT   icf_msg_st           *p_msg,
              INOUT   icf_error_t          *p_ecode )
{
      icf_return_t                  ret_val = ICF_SUCCESS;
      icf_return_t                  dummy_ret_val = ICF_SUCCESS;
      icf_msg_st                    *p_msg_resp = ICF_NULL;
      icf_uint32_t                  api_len = ICF_NULL;
      icf_mm_unexpected_msg_ind_st   *p_mm_unexpected_msg_ind = ICF_NULL;
      icf_internal_msg_st           internal_msg;

      ICF_FUNCTION_ENTER(p_glb_pdb)

          api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
            ICF_PORT_SIZEOF(icf_mm_unexpected_msg_ind_st);

           /* allocate memory to p_msg */

          ICF_MEMGET(
                     p_glb_pdb,
                     api_len,
                     ICF_MEM_COMMON,
                     p_msg_resp,
                     ICF_RET_ON_FAILURE,
                     p_ecode,
                     ret_val)

     if (ICF_NULL != p_msg && ICF_NULL != p_msg_resp)
       {
         p_msg_resp->hdr.api_id = ICF_MM_UNEXPECTED_MSG_IND;
         p_msg_resp->hdr.api_length = (icf_uint8_t)api_len;
         p_msg_resp->hdr.call_id = p_msg->hdr.call_id;
         p_msg_resp->hdr.version = ICF_VERSION_1_0;
        p_msg_resp->hdr.app_id = p_msg->hdr.app_id;

         /* Fill the payload for ICF_UNEXPECTED_MSG_IND */
         p_mm_unexpected_msg_ind = (icf_mm_unexpected_msg_ind_st *)(p_msg_resp->payload);
         /* Initialize the error cause */
         p_mm_unexpected_msg_ind->error_cause = *p_ecode;
         /* Filling in api_id ...... */
         p_mm_unexpected_msg_ind->api_id = p_msg->hdr.api_id;

         /* Form an internal message with p_msg_resp as p_msg_data*/
          p_glb_pdb->p_recv_msg = p_msg;
         internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;
         internal_msg.msg_hdr.msg_id = p_msg_resp->hdr.api_id;
         internal_msg.msg_hdr.payload_length = (icf_uint8_t)api_len;
         internal_msg.p_msg_data = (icf_uint8_t *)p_msg_resp;
         /* invoke mmi process message */
         ret_val = icf_mmi_process_mesg(&internal_msg);

         /* Free the message sent as response to application */
          ICF_MEMFREE(
               p_glb_pdb,
               p_msg_resp,
               ICF_MEM_COMMON,
               p_ecode,
               dummy_ret_val)
       }
    else
       { 
         /* Free the message sent as response to application */
         ICF_MEMFREE(
               p_glb_pdb,
               p_msg_resp,
               ICF_MEM_COMMON,
               p_ecode,
               dummy_ret_val)
         ret_val = ICF_FAILURE;
       }

     ICF_FUNCTION_EXIT(p_glb_pdb)
     return ret_val;
}

/******************************************************************************
 *
 *  FUNCTION:        icf_cmn_validate_sip_addr
 *
 *
 *
 *      DESCRIPTION:     This function validate the transport address recevied
 *                  from PA for address type
 *                  Validation Critteria:
 *
 *                  E164: An E164 number may or maynot start with '+' and must
 *                  have only hexadecimal digits in it
 *
 *                  Tel URI: A Tel URI may or may not start with '+' and must
 *                  have only hexadecimal numbers with atleast on '-' seperator
 *
 *                  SIP URI: A number which is niether a tel uri or tel uri
 *                  would be SIP URI if it has domain seperated by '@'
 *
 *                  Also we would not validate anything after ';' in the called
 *                                      party address.It is the application's responsibility.
 *
 *****************************************************************************/

icf_return_t icf_cmn_validate_sip_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode)
{
    icf_return_t               ret_val = ICF_SUCCESS;

    /* This variable is used to calculate address type sent by remote
     * If address type is same as provided by user, then this function
     * return success else return failure */

    icf_address_type_t         addr_type = ICF_ADDRESS_PLAN_UNDEFINED;

    icf_uint8_t *p_addr_data = ICF_NULL;

    /* This variable is used to identfiy number of sip uri format */
    icf_uint8_t *p_sipnumber = ICF_NULL;
    /* This variable is used to compare first char if number as teluri number
     * start with '+' char */
    icf_uint8_t telurl_start = '+';
    /* This variable is used to check presence of @ in string received from PA */
    icf_uint8_t sipuri_num = '@';

    /* This var is used in loops */
    icf_uint8_t counter =0 ;
    /* This varialbe is used to identfiy e164 number */
    icf_boolean_t e164_number = ICF_TRUE;
    /* This varialbe is used to identfiy tel number */
    icf_boolean_t tel_number =  ICF_TRUE;

    icf_uint8_t        temp_addr_str[ICF_MAX_STR_LEN] = "\0",
                       str_count = 0;

        p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb) /*function entry trace */

    p_addr_data = (icf_uint8_t *)p_addr->addr_val.str;

    /*************************************************************************
    *      fix for CSR 1-4627436                                             *
    *      eg:    tel:863-1234;phone-context=+1-914-555                      *
    *     In the above address we would ignore the phone context parameter    *
    *     and would not validate it                                          *
    ************************************************************************ */

        /* getting the called party address before ';' */

    while(('\0' != *p_addr_data) && (str_count < ICF_MAX_STR_LEN))
    {
        if(';' == *p_addr_data)
        {
            temp_addr_str[str_count] = '\0';
            break;
        }
        else
        {
            temp_addr_str[str_count] = *p_addr_data;
        }
        str_count++;
        p_addr_data++;
    }
    if (str_count < ICF_MAX_STR_LEN)
	{ 
		temp_addr_str[str_count] = '\0';
	}
    p_addr_data = (icf_uint8_t *)temp_addr_str;

        /*
       now p_addr_data has the address before ';' and p_rem has the phone-context 
       parameter(if present).If it is not present p_rem will be null
    */

                p_sipnumber = icf_port_strchr(p_addr_data,sipuri_num);

    /* if @ icf_uint8_t is not found, p_sipnumber will be null and
     * number can be telurl or e164 number */
    if(ICF_NULL == p_sipnumber)
    {
        /* Check if the address starts with a valid hexadecimal digit or '+'
         * If it doesn't it cant be E164 or Tel URI*/
        if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]))
        {
            for(counter = 1; (counter<icf_port_strlen(p_addr_data)) && (counter < ICF_MAX_STR_LEN); counter++)
            {
                if(!ICF_ISXDIGIT(p_addr_data[counter]))
                {
                    /* If this was not a hexadecimal digit. the number cant be
                     * an E164 Number*/
                    e164_number = ICF_FALSE;

                    if('-' != p_addr_data[counter])
                    {
                        /* If number does not contain [0-f] or '-'
                         *  it is not valid number  */
                        tel_number = ICF_FALSE;
                        break;
                    }
                }

            }/*End of for loop */

            if(ICF_TRUE == e164_number)
            {
                /* it is an E164 url number */
                addr_type = ICF_ADDRESS_PLAN_E164;
            }
            else if(ICF_TRUE == tel_number)
            {
                /* it is a tel url number */
                addr_type = ICF_ADDRESS_PLAN_TEL_URI;
            }

        }/*End p_addr_data[0] == telurl_start...*/
    }
    else
    {
        if ( ICF_ADDRESS_PLAN_SIPS_URI == p_addr->addr_type)
        {
            addr_type = ICF_ADDRESS_PLAN_SIPS_URI;
        }
        else
        {
            addr_type = ICF_ADDRESS_PLAN_SIP_URI;
        }
    }

    /* if calculated address type is invalid return failure */
    if(ICF_ADDRESS_PLAN_UNDEFINED == addr_type)
    {
        /* Find the address type
         * if it contain @ and no + in first char then it is SIP URL
         * IF address start with + and no @ then it is TELURL
         * else if it contain alphanumeric values
         * then it is e164 value else return error */
        ret_val = ICF_FAILURE;

    }
    else if((p_addr->addr_type != addr_type &&
                (ICF_ADDRESS_PLAN_SIP_URI == p_addr->addr_type ||
                ICF_ADDRESS_PLAN_SIP_URI == addr_type)) &&
            p_addr->addr_type!=ICF_ADDRESS_PLAN_UNDEFINED)
    {
         /* if calculated value is not same as value recevied in remote
          *  address, validation failed. Return failure */
          ret_val = ICF_FAILURE;
    }
    else if(ICF_ADDRESS_PLAN_UNDEFINED == p_addr->addr_type)
    {
        /* Update address type if it is undefined*/
        p_addr->addr_type = addr_type;
    }
    *p_ecode = *p_ecode;

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cmn_validate_sip_addr(...) */


#ifdef ICF_LOAD_STAT
/*******************************************************************************
 * FUNCTION:        icf_cmn_print_load_stats
 * DESCRIPTION:      
 *                  
 ******************************************************************************/
icf_void_t icf_cmn_print_load_stats
(
    INOUT   icf_glb_pdb_st *p_glb_pdb
)
{
    icf_uint32_t    time_sec=0,
                    time_min=0,
                    time_hr=0,
                    attempted_calls,
                    connected_calls=0,
                    planned_fail_calls=0;

    time_sec= p_glb_pdb->load_data.time_new -
              p_glb_pdb->load_data.init_time;

    if(time_sec)
    {
        if(time_sec > 60)
        {
            time_min = time_sec/60;
            time_sec = time_sec%60;
            if(time_min > 60)
            {
                time_hr = time_min/60;
                time_min = time_min%60;
            }
        }

    printf("###############################################################\n");
    printf("IPTK time elapsed Hr: %3d\tMin: %3d\tSec: %3d\n",time_hr,time_min,time_sec);
    printf("###############################################################\n");
    if(g_create_call_req)
    printf("Calls  from  APP     %d\n",g_create_call_req);
    if(p_glb_pdb->load_data.otg_call_attempts)
    printf("otg_call_attempts    %d\n",p_glb_pdb->load_data.otg_call_attempts);

    if(p_glb_pdb->load_data.otg_call_connects)
    printf("otg_call_connects    %d\n",p_glb_pdb->load_data.otg_call_connects);

    if(p_glb_pdb->load_data.inc_call_attempts)
    printf("inc_call_attempts    %d\n",p_glb_pdb->load_data.inc_call_attempts);

    if(p_glb_pdb->load_data.inc_early_call_attempts)
    printf("inc_early_call_attempts  %d\n",p_glb_pdb->load_data.inc_early_call_attempts);

    if(p_glb_pdb->load_data.inc_call_connects)
    printf("inc_call_connects    %d\n",p_glb_pdb->load_data.inc_call_connects);

    if(p_glb_pdb->load_data.otg_call_term_req)
    printf("otg_call_term_req    %d\n",p_glb_pdb->load_data.otg_call_term_req);

    if(p_glb_pdb->load_data.term_call_ind_bye)
    printf("term_call_ind_bye    %d\n",p_glb_pdb->load_data.term_call_ind_bye);

    if(p_glb_pdb->load_data.term_call_ind_404)
    printf("term_call_ind_404    %d\n",p_glb_pdb->load_data.term_call_ind_404);

    if(p_glb_pdb->load_data.term_call_ind_486)
    printf("term_call_ind_486    %d\n",p_glb_pdb->load_data.term_call_ind_486);

    if(p_glb_pdb->load_data.term_call_ind_default)
    printf("term_ind_default     %d\n",p_glb_pdb->load_data.term_call_ind_default);

    if(p_glb_pdb->load_data.call_hold_attempts)
    printf("call_hold_attempts   %d\n",p_glb_pdb->load_data.call_hold_attempts);

    if(p_glb_pdb->load_data.call_hold_success)
    printf("call_hold_success    %d\n",p_glb_pdb->load_data.call_hold_success);

    if(p_glb_pdb->load_data.call_resume_attempts)
    printf("call_resume_attempts %d\n",p_glb_pdb->load_data.call_resume_attempts);

    if(p_glb_pdb->load_data.call_resume_success)
    printf("call_resume_success  %d\n",p_glb_pdb->load_data.call_resume_success);

    if(p_glb_pdb->load_data.options_rcvd_from_app)
    printf("OPTIONS Recvd from app       %d\n",p_glb_pdb->load_data.options_rcvd_from_app);

    if(p_glb_pdb->load_data.otg_options_attempts)
    printf("OTG OPTIONS Attempts        %d\n",p_glb_pdb->load_data.otg_options_attempts);

    if(p_glb_pdb->load_data.otg_options_success)
    printf("OTG OPTIONS Success      %d\n",p_glb_pdb->load_data.otg_options_success);

    if(p_glb_pdb->load_data.inc_options_attempts)
    printf("INC OPTIONS Recvd        %d\n",p_glb_pdb->load_data.inc_options_attempts);

    if(p_glb_pdb->load_data.inc_options_success)
    printf("INC OPTIONS Success      %d\n",p_glb_pdb->load_data.inc_options_success);

    if(p_glb_pdb->load_data.inc_options_failure)
    printf("INC OPTIONS failure      %d\n",p_glb_pdb->load_data.inc_options_failure);

    if(p_glb_pdb->load_data.otg_options_failure)
    printf("OTG OPTIONS failure      %d\n",p_glb_pdb->load_data.otg_options_failure);

    if(p_glb_pdb->load_data.inc_options_failure_wo_app)
    printf("INC OPTIONS failure wo app     %d\n",p_glb_pdb->load_data.inc_options_failure_wo_app);

    if(p_glb_pdb->load_data.info_attempts)
    printf("info_attempts        %d\n",p_glb_pdb->load_data.info_attempts);

    if(p_glb_pdb->load_data.info_success)
    printf("info_success         %d\n",p_glb_pdb->load_data.info_success);

    if(p_glb_pdb->load_data.register_req)
    printf("Register request     %d\n",p_glb_pdb->load_data.register_req);

    if(p_glb_pdb->load_data.register_refresh)
    printf("Refresh Register     %d\n",p_glb_pdb->load_data.register_refresh);

    if(p_glb_pdb->load_data.register_cfm)
    printf("Register succ     %d\n",p_glb_pdb->load_data.register_cfm);

    if(p_glb_pdb->load_data.register_failure)
    printf("Register failure     %d\n",p_glb_pdb->load_data.register_failure);

    if(p_glb_pdb->load_data.subscribe_req)
    printf("Subscribe requests   %d\n",p_glb_pdb->load_data.subscribe_req);

    if(p_glb_pdb->load_data.subscribe_fail)
    printf("Subscribe req fail   %d\n",p_glb_pdb->load_data.subscribe_fail);

    if(p_glb_pdb->load_data.subscribe_refresh)
    printf("Refresh Subscribe    %d\n",p_glb_pdb->load_data.subscribe_refresh);

    if(p_glb_pdb->load_data.subscribe_cfm)
    printf("Subscribe cfm   %d\n",p_glb_pdb->load_data.subscribe_cfm);

    /* Changes for printing the load stats start*/
    if(p_glb_pdb->load_data.otg_message_attempts)
    printf("OTG MESSAGE Attempts  %d\n",p_glb_pdb->load_data.otg_message_attempts);

    if(p_glb_pdb->load_data.otg_message_success)
     printf("OTG MESSAGE success %d\n",p_glb_pdb->load_data.otg_message_success); 

    if(p_glb_pdb->load_data.inc_message_attempts)
    printf("INC MESSAGE Attempts %d\n",p_glb_pdb->load_data.inc_message_attempts);

    if(p_glb_pdb->load_data.inc_message_success)
     printf("INC MESSAGE success  %d\n",p_glb_pdb->load_data.inc_message_success);

    if(p_glb_pdb->load_data.otg_publish_attempts)
      printf("OTG PUBLISH Attempts %d\n",p_glb_pdb->load_data.otg_publish_attempts);
    
    if(p_glb_pdb->load_data.otg_publish_success)
     printf("OTG PUBLISH success %d\n",p_glb_pdb->load_data.otg_publish_success);
    
    if(p_glb_pdb->load_data.otg_cta_attempts)
     printf("OTG CTA attempts %d\n",p_glb_pdb->load_data.otg_cta_attempts);

    if(p_glb_pdb->load_data.otg_cta_success)
     printf("OTG CTA success %d\n",p_glb_pdb->load_data.otg_cta_success);
    
    /*Changes for printing the load stats end*/
                            
    if(p_glb_pdb->load_data.load_force_cleared_calls)
    printf("LOAD FORCE CLEARED CALLS   %d\n",p_glb_pdb->load_data.load_force_cleared_calls);
    
    if(p_glb_pdb->load_data.otg_session_refresh_update)
    printf("SESSION REFRESH UPDATE   %d\n",p_glb_pdb->load_data.otg_session_refresh_update);
    
    if(p_glb_pdb->load_data.otg_session_refresh_invite)
    printf("SESSION REFRESH INVITE   %d\n",p_glb_pdb->load_data.otg_session_refresh_invite);

    if(p_glb_pdb->load_data.num_of_302_reponse_recv)
    printf("Num of 302 response received %d\n",p_glb_pdb->load_data.num_of_302_reponse_recv);    

    printf("###############################################################\n");
    attempted_calls=p_glb_pdb->load_data.otg_call_attempts+
                     p_glb_pdb->load_data.inc_call_attempts;

    connected_calls=p_glb_pdb->load_data.otg_call_connects + 
                     p_glb_pdb->load_data.inc_call_connects;

    planned_fail_calls=p_glb_pdb->load_data.term_call_ind_486 +
                        p_glb_pdb->load_data.term_call_ind_404;

#if 0
    printf("Calls/min            %d\n",attempted_calls/time_min);
    printf("%Success Calls       %d\n",(100*connected_calls)/attempted_calls);
    printf("%Planned Fail Calls  %d\n",(100*planned_fail_calls)/attempted_calls);
    printf("%Unpland Fail Calls  %d\n",(100*p_glb_pdb->load_data.term_call_ind_default)/
                                       attempted_calls);
#endif
    printf("###############################################################\n");
    }
}


#endif

/************* Changes for Forking & Multiple Answer Start (Rel7.0) *************/

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_node_in_list 
*
*  DESCRIPTION    :  This function will return a node from the list that 
*		     matches the identifier in the list node.The function 
*                    returns the node in the list
*
*************************************************************************/

icf_list_st*	icf_cmn_get_node_in_list 
(
	INOUT	icf_list_st	*p_list_start,
	IN	icf_uint8_t	identifier
)
{
	/* local variables */
	icf_list_st	*p_tmp_node = ICF_NULL;
	
	p_tmp_node = p_list_start;

	while (ICF_NULL != p_tmp_node)
	{
		/* compare the identifier with the identifier in the node */
		if (identifier == ((icf_forked_resp_data_st *)p_tmp_node->p_data)->forked_resp_id)
		{
			break;
		}
		else
		{
			p_tmp_node = p_tmp_node->p_next;
		}
	}

	return p_tmp_node;
}/*end of icf_cmn_get_node_in_list()*/

/*************** Changes for Forking & Multiple Answer End (Rel7.0) ***************/

/********** Changes for Retry-After Start (Rel7.0) **********/

/******************************************************************************
*
* FUNCTION NAME    : icf_cmn_generate_timer
*
* DESCRIPTION  : This function is used to generate random timer
*
* RETURNS:
******************************************************************************/

icf_return_t icf_cmn_generate_timer(
        INOUT icf_duration_t     *p_timer_val,
        IN    icf_uint16_t     st_val,
        IN    icf_uint16_t     end_val)
{
    icf_return_t       ret_val = ICF_SUCCESS;

    /*Get random timer value in specified range*/
    *p_timer_val = icf_port_random(st_val, end_val);

    /*If returned timer value is not in specified range*/
    if (*p_timer_val < st_val || *p_timer_val>end_val)
    {
        ret_val = ICF_FAILURE;
    } /* if (*timer_val < st_val || *tim... */

    return ret_val;
} /* IN    icf_uint8_t     end_v... */

/*********** Changes for Retry-After End (Rel7.0) ***********/

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_check_if_auth_params_present
 *
 * Description:		This function return success if the user/line combination
 *					has auth key and auth password present.
 *******************************************************************************/
icf_return_t icf_cmn_check_if_auth_params_present
(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st *p_addr,
		IN	  icf_line_id_t  line_id)
{
    icf_return_t             ret_val = ICF_FAILURE;
    icf_line_data_st         *p_line_data = ICF_NULL;
    icf_user_db_node_st      *p_user_db = ICF_NULL;
    icf_error_t              ecode = 0;

    if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line(
                          p_glb_pdb, line_id, p_addr,
                          ICF_USER_DB_USE_PUBLIC_URIS,
                          &p_user_db, &ecode)&&
                	  (p_user_db->flags & ICF_USER_DB_AUTH_PRMS_PER_USER))
	{
		if((p_user_db->flags & ICF_USER_DB_AUTH_KEY_RECD)&&
		(p_user_db->flags & ICF_USER_DB_AUTH_PASSWD_RECD))
		{
		    ret_val = ICF_SUCCESS;
		}
    }
	else if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
							p_glb_pdb, line_id,
							(icf_void_t *)&(p_line_data),
							&ecode))
	{
        if((p_line_data->line_data_received & 
		ICF_CFG_LINE_DATA_AUTH_KEY_RECD)&&
		(p_line_data->line_data_received & 
		ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD))
		{
		    ret_val = ICF_SUCCESS;
		}
	}

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_auth_params
 *
 * Description:		This function returns the auth params that are configured
 *					in user db or line depending on the PA specification.					
 ******************************************************************************/
icf_return_t icf_cmn_get_auth_params
(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st *p_addr,
		IN	  icf_line_id_t  line_id,
		INOUT icf_string_st  *p_key,
		INOUT icf_string_st  *p_passwd)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_line_data_st         *p_line_data = ICF_NULL;
    icf_user_db_node_st      *p_user_db = ICF_NULL;
    icf_error_t		         ecode = 0;

    if(ICF_SUCCESS == icf_dbm_get_public_uri_node_from_uri_for_line(
                          p_glb_pdb, line_id, p_addr,
                          ICF_USER_DB_USE_PUBLIC_URIS,
                          &p_user_db, &ecode)&&
    	              (p_user_db->flags & ICF_USER_DB_AUTH_PRMS_PER_USER))
	{
		if(p_user_db->flags & ICF_USER_DB_AUTH_KEY_RECD)
		{
		    icf_port_memcpy(
		        p_key,
		   	    &p_user_db->authentication_key,
			    (icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));		
		}
		
		if(p_user_db->flags & ICF_USER_DB_AUTH_PASSWD_RECD)
		{
		    icf_port_memcpy(
		        p_passwd,
			    &p_user_db->authentication_password,
			   	(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
		}
    }
	else if(ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
							p_glb_pdb, line_id,
							(icf_void_t *)&(p_line_data),
							&ecode))
	{
		if(p_line_data->line_data_received & 
			ICF_CFG_LINE_DATA_AUTH_KEY_RECD)
		{
		    icf_port_memcpy(
				p_key,
				&p_line_data->authentication_key,
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));		    
		}

		if(p_line_data->line_data_received & 
			ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD)
		{
		    icf_port_memcpy(
				p_passwd,
				&p_line_data->authentication_password,
				(icf_uint32_t)ICF_PORT_SIZEOF(icf_string_st));
		}		
	}

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
}
/******************************************************************************
 *
 * FUNCTION:        icf_cmn_parse_transport_addr
 *
 * DESCRIPTION:     This function parses the transport address recevied
 *                  from PA and determines its address type.
 *                  Validation Critteria:
 *                  
 *                  E164: An E164 number may or maynot start with '+' and must 
 *                  have only hexadecimal digits in it 
 *
 *                  Tel URI: A Tel URI may or may not start with '+' and must 
 *                  have only hexadecimal numbers with atleast on '-' seperator
 *
 *                  SIP URI: A number which is niether a tel uri or tel uri 
 *                  would be SIP URI if it has domain seperated by '@'
 *
 *
 *****************************************************************************/
icf_return_t icf_cmn_parse_transport_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode)
 {
    icf_return_t               ret_val = ICF_SUCCESS;

    /* This variable is used to calculate address type sent by remote
     * If address type is same as provided by user, then this function
     * return success else return failure */

    icf_uint8_t *p_addr_data = ICF_NULL;

    /* This variable is used to identfiy number of sip uri format */
    icf_uint8_t *p_sipnumber = ICF_NULL;
    /* This variable is used to compare first char iof number as teluri number
     * start with '+' char */
    icf_uint8_t telurl_start = '+';
    /* This variable is used to check presence of @ in string received from PA */
    icf_uint8_t sipuri_num = '@';
    /* This var is used in loops */
    icf_uint8_t counter =0 ;
    /* This varialbe is used to identfiy e164 number */
    icf_boolean_t e164_number = ICF_TRUE;
    /* This varialbe is used to identfiy tel number */
    icf_boolean_t tel_number =  ICF_TRUE;

    if(ICF_NULL == p_glb_pdb)
    {
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb) /*function entry trace */

    p_addr_data = (icf_uint8_t *)p_addr->addr_val.str;
    p_sipnumber = icf_port_strchr(p_addr_data,sipuri_num);

    /* if @ icf_uint8_t is not found, p_sipnumber will be null and
     * number can be telurl or e164 number */
    if(ICF_NULL == p_sipnumber)
    {
        /* Check if the address starts with a valid hexadecimal digit or '+'
         * If it doesn't it cant be E164 or Tel URI*/
        if(p_addr_data[0] == telurl_start || ICF_ISXDIGIT(p_addr_data[0]))
        {
            for(counter = 1; counter<icf_port_strlen(p_addr_data); counter++)
            {
                if(!ICF_ISXDIGIT(p_addr_data[counter])) 
                {
                    /* If this was not a hexadecimal digit. the number cant be 
                     * an E164 Number*/
                    e164_number = ICF_FALSE;

                    if('-' != p_addr_data[counter])
                    {
                        /* If number does not contain [0-f] or '-'
                         *  it is not valid number  */
                        tel_number = ICF_FALSE;
                        break;
                    }
                }

            }/*End of for loop */

            if(ICF_TRUE == e164_number)
            {
                /* it is an E164 url number */
                p_addr->addr_type = ICF_ADDRESS_PLAN_E164;
            }
            else if(ICF_TRUE == tel_number)
            {
                /* it is a tel url number */
                p_addr->addr_type = ICF_ADDRESS_PLAN_TEL_URI;
            }

        }/*End p_addr_data[0] == telurl_start...*/
    }
    else
    {
        /* THis can be sip uri, if it does not start with +" */
        if(p_addr_data[0] != telurl_start)
        {
            if ( ICF_ADDRESS_PLAN_SIPS_URI == p_addr->addr_type)
            {
                p_addr->addr_type = ICF_ADDRESS_PLAN_SIPS_URI;
            }
            else
            {
                p_addr->addr_type = ICF_ADDRESS_PLAN_SIP_URI;
            }
        }
    }

    /* if calculated address type is invalid return failure */
    if(ICF_ADDRESS_PLAN_UNDEFINED == p_addr->addr_type) 
    {
        /* Find the address type
         * if it contain @ and no + in first char then it is SIP URL
         * IF address start with + and no @ then it is TELURL
         * else if it contain alphanumeric values
         * then it is e164 value else return error */
        ret_val = ICF_FAILURE;

    }
   *p_ecode = *p_ecode;

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end if icf_cmn_parse_transport_addr*/

/**************************************************************************
 *
 * FUNCTION:        icf_cmn_process_aka_challenge
 * 
 * DESCRIPTION:     This function is a utility function which initiates the
 *                  AKA challenge processing.It receives as input,the auth data
 *                  received from network.It operates on it and populates
 *                  auth data sent.It also sets appropriate error codes
 *                  on the basis of which the calling function can take
 *                  decisions.
 *                  The method name is passed as IN to port. Port returns
 *                  status (sync or async as output).
 *                  In case of MAC or SQN failures, port returns AKA algo
 *                  failure. However internally IPTK interprets these as
 *                  successful as they result in re-sending of method.
 *                   
 * RETURNS:         ICF_SUCCESS/ICF_FAILUE.
 *************************************************************************/
icf_return_t icf_cmn_process_aka_challenge(
	INOUT icf_glb_pdb_st          *p_glb_pdb,
	INOUT icf_cmn_auth_data_st    *p_auth_data_recd,
	INOUT icf_cmn_auth_data_st    *p_auth_data_sent,
    INOUT icf_address_st          *p_addr,
    IN    icf_line_id_t           line_id,
    IN    icf_app_id_t            app_id,
    INOUT icf_large_string_st     *p_security_client,
    INOUT icf_large_string_st     *p_security_verify,
    INOUT icf_large_string_st     *p_security_server,
    IN    icf_uint32_t            security_association_status,        
    IN    icf_uint8_t             sip_method_name,
    OUT   icf_boolean_t           *p_resp_sync_flag,
    OUT   icf_error_t             *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* These are pointers to input and ouput aka info structures
     * which are passed to the porting function.
     */
    icf_aka_auth_input_st    *p_aka_auth_input = ICF_NULL;
    icf_aka_auth_output_st   *p_aka_auth_output = ICF_NULL;
    icf_uint32_t             temp_len = ICF_NULL;
    icf_return_t             dummy_ret_val = ICF_SUCCESS;

    /* Allocating memory for aka specific structures.This will be passed
     * to the entry level function for port procedure
     */
    ICF_MEMGET(p_glb_pdb, 
            sizeof(icf_aka_auth_input_st),
            ICF_MEM_COMMON, 
            (p_aka_auth_input),
            ICF_RET_ON_FAILURE, 
            p_ecode, ret_val)

    ICF_MEMGET(p_glb_pdb, 
            sizeof(icf_aka_auth_output_st),
            ICF_MEM_COMMON, 
            (p_aka_auth_output),
            ICF_DONOT_RET_ON_FAILURE, 
            p_ecode, ret_val)
    
    if(ICF_FAILURE != ret_val)
    {
        /* Initializing the aka auth structures */
        icf_port_memset(p_aka_auth_input,ICF_NULL,
                    sizeof(icf_aka_auth_input_st));
        icf_port_memset(p_aka_auth_output,ICF_NULL,
                    sizeof(icf_aka_auth_output_st));

        /* Clear old contents of auth_data_sent */

        ICF_MEMFREE(p_glb_pdb,
                    p_auth_data_sent->p_nonce,
                    ICF_MEM_COMMON,
                    p_ecode,
                    dummy_ret_val)

        icf_port_memset(p_auth_data_sent,
                    ICF_NULL,
                    sizeof(icf_cmn_auth_data_st));
    
        /* now we will copy fields from auth data recd to auth data sent */

        /* Copy the value of nonce */
        temp_len = p_auth_data_recd->nonce_len;
            
        ICF_MEMGET(p_glb_pdb, 
                   temp_len + 1,
                   ICF_MEM_COMMON,
                   p_auth_data_sent->p_nonce,
                   ICF_DONOT_RET_ON_FAILURE,
                   p_ecode,
                   ret_val)
    
        if(ICF_FAILURE != ret_val)
        {
            /* Start copying fileds */

            icf_port_memcpy(p_auth_data_sent->p_nonce,
                    p_auth_data_recd->p_nonce,
                    temp_len + 1);
            p_auth_data_sent->nonce_len = temp_len;

            /* Copy the value of scheme */
            icf_port_memcpy(&(p_auth_data_sent->scheme),
                    &(p_auth_data_recd->scheme),
                    ICF_PORT_SIZEOF(icf_string_st));
            
            /* Copy the value of algo */                
            icf_port_memcpy(&(p_auth_data_sent->algo),
                    &(p_auth_data_recd->algo),
                    ICF_PORT_SIZEOF(icf_string_st));
            
            /* Copy the realm value */
            icf_port_memcpy(&(p_auth_data_sent->realm),
                    &(p_auth_data_recd->realm),
                    ICF_PORT_SIZEOF(icf_string_st));

            /* Set the value for URI */
            icf_port_strcpy(p_auth_data_sent->uri.str,(icf_uint8_t *)"sip:");
            icf_port_strcat(p_auth_data_sent->uri.str,
                    p_auth_data_sent->realm.str);
            p_auth_data_sent->uri.str_len = 
                    icf_port_strlen(p_auth_data_sent->uri.str);
             
            /* Copy the Opaque value */
            icf_port_memcpy(&(p_auth_data_sent->opaque),
                    &(p_auth_data_recd->opaque),
                    ICF_PORT_SIZEOF(icf_string_st));

            /* Setting the bitmask same as auth_data_recd */
            p_auth_data_sent->auth_data_present =
                p_auth_data_recd->auth_data_present;

            p_auth_data_sent->auth_data_present |=
                    ICF_AUTH_URI_PRESENT;

            /* Copying the value of sequence number */
            p_auth_data_sent->sequence_num = 
                p_auth_data_recd->sequence_num;
                    
            /* Populating the input structure fileds that will be passed
             * to the porting function.
             */
            
            /* filling call_id.As of now we are filling it with a dummy
             * value.This field is for future use.
             */
            p_aka_auth_input->call_id = ICF_INVALID_CALL_ID;

            /* filling line_id and app_id as received. */
            p_aka_auth_input->app_id = app_id;
            p_aka_auth_input->line_id = line_id;

             /* filling the value of algo */                
            icf_port_memcpy(&(p_aka_auth_input->algo),
                    &(p_auth_data_sent->algo),
                    ICF_PORT_SIZEOF(icf_short_string_st));
            
            /* Copy the realm value */
            icf_port_memcpy(&(p_aka_auth_input->realm),
                    &(p_auth_data_sent->realm),
                    ICF_PORT_SIZEOF(icf_string_st));

            /* filling the value for URI */
            icf_port_strcpy(p_aka_auth_input->uri.str,(icf_uint8_t *)"sip:");
            icf_port_strcat(p_aka_auth_input->uri.str,
                    p_auth_data_sent->realm.str);
            p_aka_auth_input->uri.str_len = 
                    icf_port_strlen(p_auth_data_sent->uri.str);

            /* filling nonce value */
            icf_port_memcpy(&(p_aka_auth_input->nonce.str),
                    p_auth_data_sent->p_nonce,
                    p_auth_data_sent->nonce_len + 1);
            p_aka_auth_input->nonce.str_len = p_auth_data_sent->nonce_len;

            /* filling values of Ik and Ck as NULL.Currently these are
             * not being extracted from the challenge and are reseved for
             * future use.
             */
            p_aka_auth_input->Ik.str[0] = '\0';
            p_aka_auth_input->Ik.str_len = 0;
            p_aka_auth_input->Ck.str[0] = '\0';
            p_aka_auth_input->Ck.str_len = 0;

            /* filling the value of last sequence number stored */
            p_aka_auth_input->sequence_num = p_auth_data_sent->sequence_num;

            /* filling the sip method name and user address 
             * as input to be passed to port */
            p_aka_auth_input->sip_method_name = sip_method_name;
            icf_port_memcpy(&(p_aka_auth_input->user_address),
                    p_addr, ICF_PORT_SIZEOF(icf_address_st));
           
            /* Filling security_client and security_server. */

            /* The security headers is not communicated to application
             * if Security-Server header is not received in 401/407.
             */  

            if((ICF_NULL != p_security_client) &&
               ((ICF_NULL != p_security_server) ||
                (ICF_NULL != p_security_verify)))
            {
              icf_port_memcpy(&(p_aka_auth_input->security_client),
                                p_security_client,
                                ICF_PORT_SIZEOF(icf_large_string_st));
            }

            /* If REGISTER request is sent with Security-Client
             * header and 401/407 is received with Secuirty-Server header,
             * then Security-Server header value will be present in 
             * p_security-verify field in regm context if the security
             * association does not exist else the Security-Server
             * header value will be present in p_security_server field
             * in regm context.
             */


            /* This is the case when the security association is done
             * and 401/407 response is received for Re-Register with 
             * security-server header.
             */  

            if((ICF_NULL != 
               (ICF_RGM_SECURITY_ASSOCIATION_DONE &
                              security_association_status)) &&
               (ICF_NULL != p_security_server))
            { 
              icf_port_memcpy(&(p_aka_auth_input->security_server),
                                p_security_server,
                                ICF_PORT_SIZEOF(icf_large_string_st));
            }

            /* This is the case when the security association is not done
             * and 401/407 response is received for Register with 
             * security-server header.
             */

            else if((ICF_NULL == 
                     (ICF_RGM_SECURITY_ASSOCIATION_DONE &
                                        security_association_status)) &&
                     (ICF_NULL != p_security_verify))
            {
              icf_port_memcpy(&(p_aka_auth_input->security_server),
                                p_security_verify,
                                ICF_PORT_SIZEOF(icf_large_string_st));
            }
 
            /* filling username(auth_key) and auth password. */
            if(ICF_FAILURE == icf_cmn_get_auth_params(
                    p_glb_pdb,
                    p_addr,
                    line_id,
                    &(p_aka_auth_input->username),
                    &(p_aka_auth_input->password)))
            {
                ret_val = ICF_FAILURE;
                *p_ecode = ICF_CAUSE_INVALID_AUTH_PARAM;
            }
            /* Call the entry level function for AKA */
            else
            { 
                ret_val = icf_port_run_aka_procedure(
                              p_aka_auth_input,
                              p_aka_auth_output,
                              p_resp_sync_flag);
                
                /* Check whether port has returned async response for methods 
                 * other than REGISTER */
                if((ICF_METHOD_REGISTER != p_aka_auth_input->sip_method_name) &&
                        (ICF_FALSE == *p_resp_sync_flag))
                {
                    ret_val = ICF_FAILURE;
                    *p_ecode = ICF_CAUSE_INVALID_AUTH_RESP_STATUS;
                    ICF_MEMFREE(p_glb_pdb,
                            (p_aka_auth_output),
                            ICF_MEM_COMMON,
                            p_ecode,
                            dummy_ret_val)

                    ICF_MEMFREE(p_glb_pdb,
                            (p_aka_auth_input),
                            ICF_MEM_COMMON,
                            p_ecode,
                            dummy_ret_val)

                    return ret_val;
                }

                /* Fix for SPR 19478:
                   If the porting function has returned failure or has
                   indicated synchronous support for REGISTER method, 
                   we will free the memory allocated for the security
                   related pointers in the regm context. However instead 
                   of freeing them in this function, the freeing will
                   be done in the calling function icf_rgm_aka_auth_process.
                   This is to avoid FUMs in case the porting function returns 
                   failure and the security related pointers are again
                   freed in the function icf_rgm_clear_reg_context.
                */   

                /* Assigning the error code as set by the port function */
                *p_ecode = p_aka_auth_output->ecode;

                /* first copying the sequence number as computed by 
                 * the porting procedure from the nonce value.
                 */
                p_auth_data_recd->sequence_num = 
                    p_aka_auth_output->sequence_num;
                
                if(ICF_FAILURE == ret_val)
                {
                    /* Check whether the resp is sync or async */
                    if(ICF_TRUE == *p_resp_sync_flag)
                    {
                        /* Check error response */
                        if(ICF_CAUSE_SQN_OUT_OF_RANGE == p_aka_auth_output->ecode)
                        {
                            /* AKA procedures returned AUTS to be used */
                            ret_val = ICF_SUCCESS;
                            p_auth_data_sent->auts.str_len = 
                                p_aka_auth_output->auts.str_len;
                            icf_port_memcpy(&(p_auth_data_sent->auts.str),
                                    &(p_aka_auth_output->auts.str),
                                    p_auth_data_sent->auts.str_len);
                            if (p_auth_data_sent->auts.str_len < ICF_MAX_STR_LEN) 
							{
								p_auth_data_sent->auts.str[p_auth_data_sent->auts.str_len]
                                = '\0';
							}
                            p_auth_data_sent->auth_data_present |=
                                ICF_AUTH_AUTS_PRESENT;
                        }
                        else if (ICF_CAUSE_HOME_NETWORK_NOT_AUTH == 
                                p_aka_auth_output->ecode)
                        {
                            /* AKA procedures returned MAC failure.
                             * New method is to be sent with no AUTS
                             * or RES */
                            ret_val = ICF_SUCCESS;
                        }
                        else
                        {
                            /* Do nothing.*/
                        }
                    }
                    else
                    {
                        /* Do nothing.*/
                    }
                }
                else
                {
                    /* Check whether the resp is sync or async */
                    if(ICF_TRUE == *p_resp_sync_flag)
                    {
                        /* AKA procedure successful, copy res */
                        p_auth_data_sent->res.str_len = 
                            p_aka_auth_output->res.str_len;
                        icf_port_memcpy(&(p_auth_data_sent->res.str),
                            &(p_aka_auth_output->res.str),
                            p_auth_data_sent->res.str_len);
                        if (p_auth_data_sent->res.str_len < ICF_MAX_STR_LEN) 
						{
							p_auth_data_sent->res.str[p_auth_data_sent->res.str_len]
                            = '\0';
						}
                        p_auth_data_sent->auth_data_present |=
                            ICF_AUTH_RES_PRESENT;

                       /* Copying Integrity key and Cipher key generated by AKA 
                        * procedure.
                        */
                        p_auth_data_sent->enc_key.str_len = 
                            p_aka_auth_output->Ck.str_len;

                        icf_port_memcpy(&(p_auth_data_sent->enc_key.str),
                            &(p_aka_auth_output->Ck.str),
                            p_auth_data_sent->enc_key.str_len);
                        if (p_auth_data_sent->enc_key.str_len < ICF_MAX_SHORT_STR_LEN) 
						{
							p_auth_data_sent->enc_key.str[p_auth_data_sent->enc_key.str_len]
                            = '\0';
						}

                        p_auth_data_sent->auth_passwd.str_len = 
                            p_aka_auth_output->Ik.str_len;

                        icf_port_memcpy(&(p_auth_data_sent->auth_passwd.str),
                            &(p_aka_auth_output->Ik.str),
                            p_auth_data_sent->auth_passwd.str_len);
                        if (p_auth_data_sent->auth_passwd.str_len < ICF_MAX_SHORT_STR_LEN)
						{
							p_auth_data_sent->auth_passwd.str[p_auth_data_sent->auth_passwd.str_len]
                            = '\0';
						}
                    }
                    else
                    {
                        /* Do not process further, wait for resp 
                         * from application */
                    } 
                }
            }/* end successful extraction of nonce */
        }/* end suuccessful second memget */
        
        ICF_MEMFREE(p_glb_pdb,
               (p_aka_auth_output),
               ICF_MEM_COMMON,
               p_ecode,
               dummy_ret_val)

    }/* end successful memget of p_aka_auth_output */

    ICF_MEMFREE(p_glb_pdb,
               (p_aka_auth_input),
               ICF_MEM_COMMON,
               p_ecode,
               dummy_ret_val)


    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
}

/**************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_challenge_algo
 * 
 * DESCRIPTION:     This function is a utility function which validates 
 *                  the challenge algo supported.The algos supported
 *                  are MD5,md5,MD5-sess and AKAv1-MD5.
 *
 * RETURNS:         ICF_SUCCESS/ICF_FAILURE.
 *************************************************************************/
icf_return_t icf_cmn_validate_challenge_algo(
	IN icf_uint8_t     *p_algo)
{
    icf_return_t ret_val = ICF_FAILURE;
 
    /* check for algo */      
    if ((ICF_NULL == icf_port_strcasecmp((const icf_int8_t *)p_algo,"MD5")) ||
        (ICF_NULL == icf_port_strcasecmp((const icf_int8_t *)p_algo,"MD5-sess"))||
        (ICF_NULL == icf_port_strcasecmp((const icf_int8_t *)p_algo,"AKAv1-MD5")))
    {
        ret_val = ICF_SUCCESS;
    }
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_reg_auth_data
 *
 * DESCRIPTION:     This function initializes the RGM Authentication data
 * 
 *****************************************************************************/
icf_return_t icf_cmn_init_reg_auth_data(
        INOUT icf_cmn_auth_data_st *p_reg_auth_data)    
 {
    icf_return_t               ret_val = ICF_SUCCESS;

    p_reg_auth_data->auth_data_present = ICF_NULL;
    p_reg_auth_data->p_nonce = ICF_NULL;
    p_reg_auth_data->nonce_len = ICF_NULL;
    p_reg_auth_data->nonce_count = ICF_NULL;

    ICF_CMN_INIT_STRING(p_reg_auth_data->scheme)
    ICF_CMN_INIT_STRING(p_reg_auth_data->algo)
    ICF_CMN_INIT_STRING(p_reg_auth_data->realm)
    ICF_CMN_INIT_STRING(p_reg_auth_data->res)
    ICF_CMN_INIT_STRING(p_reg_auth_data->opaque)
    ICF_CMN_INIT_STRING(p_reg_auth_data->auts)
    ICF_CMN_INIT_STRING(p_reg_auth_data->uri)
    ICF_CMN_INIT_STRING(p_reg_auth_data->qop)
    ICF_CMN_INIT_STRING(p_reg_auth_data->cnonce)
    ICF_CMN_INIT_STRING(p_reg_auth_data->enc_key)
    ICF_CMN_INIT_STRING(p_reg_auth_data->auth_passwd)
    p_reg_auth_data->sequence_num = ICF_NULL;

    return ret_val;
}/* end if icf_cmn_init_reg_auth_data*/

/******************************************************************************
 * FUNCTION:        icf_cmn_append_msg_to_list
 * DESCRIPTION:     This function appends a message to MRM's message list.
 *****************************************************************************/
icf_return_t icf_cmn_append_msg_to_list(
              icf_msg_st        *p_msg_data)
{
    icf_msg_st           *p_msg = ICF_NULL;  
    icf_msg_list_st      *p_temp_msg_lst = ICF_NULL;
    icf_error_t          ecode = ICF_ERROR_NONE;
    icf_glb_pdb_st       *p_glb_pdb = p_persistent_glb_pdb;
    icf_return_t         ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_msg_data,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    	return ret_val;
    }

    /* Invoking this function to fill the API header and payload */
    ret_val = icf_cmn_fill_ext_msg(p_msg_data->hdr.api_id,
                                   p_msg_data->hdr.call_id,
                                   p_msg_data->hdr.app_id,
                                   &p_msg);
	if(ICF_FAILURE == ret_val)
	{
        icf_cmn_memfree(p_glb_pdb,
		                p_msg,
			            ICF_MEM_COMMON,
                        &ecode);
        p_msg = ICF_NULL;
		ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
		return ret_val;
	}

    p_temp_msg_lst = p_glb_pdb->p_msg_lst;

    /*  Traverse the link list till last node */
    while ((ICF_NULL != p_temp_msg_lst) &&
           (ICF_NULL != p_temp_msg_lst->p_msg_next))
    {        
        p_temp_msg_lst = p_temp_msg_lst->p_msg_next;
    }
    /* If already there are messages in the list */
    if((ICF_NULL != p_temp_msg_lst) &&
       (ICF_NULL != p_temp_msg_lst->p_msg))
    {
        /* Create a new node */
        p_temp_msg_lst->p_msg_next = (icf_msg_list_st *)
                                     icf_cmn_memget(
                                     p_glb_pdb,
                                     ICF_PORT_SIZEOF(icf_msg_list_st),
                                     ICF_MEM_COMMON,
                                     &ecode); 
        /* If there's a memory allocation failure */
        if (ICF_NULL == p_temp_msg_lst->p_msg_next) 
        { 
            icf_cmn_memfree(p_glb_pdb,
			                p_msg,
				            ICF_MEM_COMMON,
                            &ecode);
            p_msg = ICF_NULL;
            ret_val = ICF_FAILURE;
        } 
        else
        {
            if((ICF_NULL != p_temp_msg_lst) &&
               (ICF_NULL != p_temp_msg_lst->p_msg_next))
            {
                p_temp_msg_lst->p_msg_next->p_msg = p_msg;
                p_temp_msg_lst->p_msg_next->p_msg_next = ICF_NULL;
            }
        }
    } /* The message is the first message in the list */
    else
    {
        if(ICF_NULL != p_temp_msg_lst)
        {
            p_temp_msg_lst->p_msg = p_msg;
        }
    }
    /* Klokworks warning resolution */
    /* The memory acquired for p_msg through the function icf_cmn_fill_ext_msg
       will be freed in MRM module in the function icf_process_message. The message
       node created here has been appended to GLB PDB's message list. In MRM, we
       are polling on the message list. The mode created here will be freed once
       that particular node has been processed by MRM (here it is the dummy
       DMS response that has been appened to the global message list).
       Thus this is a false positive warning. 
    */
    ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_msg)
    /* end Klokworks warning resolution */

	ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
	return ret_val;
}

/******************************************************************************
 * FUNCTION:        icf_cmn_fill_ext_msg
 * DESCRIPTION:     This function creates an internal message for the
 *                  API ID passed to it.
 *****************************************************************************/
icf_return_t icf_cmn_fill_ext_msg(
		     icf_api_id_t    api_id,
             icf_call_id_t   call_id,
             icf_app_id_t    app_id,
             icf_msg_st      **p_p_dummy_resp)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_msg_resp_st *p_dms_resp = ICF_NULL;
    icf_error_t     ecode = ICF_ERROR_NONE;
	
	ICF_FUNCTION_ENTER(p_persistent_glb_pdb) /* function entry trace */

    switch(api_id)
    {
        case ICF_DELETE_MEDIA_SESSION_REQ:

			 /* Make dummy DMS success response */
             (*p_p_dummy_resp) = icf_cmn_memget(p_persistent_glb_pdb, 
                                     (ICF_PORT_SIZEOF(icf_api_header_st) +
    					             ICF_PORT_SIZEOF(icf_msg_resp_st)),
                                     ICF_MEM_COMMON,
                                     &ecode);
			 /* If there is memory allocation failure, return failure */
			 if(ICF_NULL ==  *p_p_dummy_resp)
			 {
				 ret_val = ICF_FAILURE;
				 ICF_FUNCTION_EXIT(p_persistent_glb_pdb) /* function exit trace */
				 return ret_val;
			 }
             (*p_p_dummy_resp)->hdr.api_id = ICF_DELETE_MEDIA_SESSION_RESP;  
             (*p_p_dummy_resp)->hdr.version = ICF_VERSION_1_0;
             (*p_p_dummy_resp)->hdr.source_id = ICF_MODULE_MM;
             (*p_p_dummy_resp)->hdr.destination_id = ICF_MODULE_ICF;
             (*p_p_dummy_resp)->hdr.call_id = call_id;
             (*p_p_dummy_resp)->hdr.app_id = app_id;
             (*p_p_dummy_resp)->hdr.api_length = 
                 ICF_PORT_SIZEOF(icf_api_header_st) + ICF_PORT_SIZEOF(icf_msg_resp_st);
             p_dms_resp = (icf_msg_resp_st*)((*p_p_dummy_resp))->payload;
             p_dms_resp->result = ICF_SUCCESS;
             break;
        
    }
	ICF_FUNCTION_EXIT(p_persistent_glb_pdb) /* function exit trace */
    return ret_val;    
}

#ifdef ICF_DNS_LOOKUP_ENABLED
/*-----Function added for open source replacement(IPTK REL 8.1)------START---*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_ip_str_to_transport_addr
 *
 * DESCRIPTION:     This function Converts IP string to Transport Address
 *                  Currently used at init time when global process data block
 *                  has not been initialized. It currently handles only for
 *                  ICF_TRANSPORT_ADDRESS_IPV4_ADDR ie. IPV4 format
 ******************************************************************************/
icf_return_t icf_cmn_convert_ip_str_to_transport_addr(
IN               icf_uint8_t *ip_address,
OUT              icf_transport_addr_st *ip_trans_addr)
{
    /* return value from function */
    icf_return_t                ret_val = ICF_SUCCESS;
    /* temporary string used in icf_port_strtokr */
    icf_int8_t 		        *pTemp = ICF_NULL;
    /* returned octet as string from icf_port_strtokr */
    icf_uint8_t                 *pOctet = ICF_NULL;
    /* temporary string passed in icf_port_strtokr */
    icf_uint8_t                 ip_address1[ICF_MAX_SHORT_STR_LEN] = "";
    /* used for storing octet as numeric value */
    icf_uint8_t   	        dOctet = 0;

    /* NULL check and length check for the parameter ip address */ 
    if ((ICF_NULL == ip_address) ||
                    ( 0 == icf_port_strlen(ip_address))) 
        return ICF_FAILURE;
    /* checking '.' at the end of string */
    if('.' == ip_address[icf_port_strlen(ip_address)-1])
    {
        ret_val = ICF_FAILURE;
    }
    /* copying passed ip address into local variable */
    icf_port_strcpy(ip_address1, ip_address);
    pOctet = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)ip_address1,
             (const icf_int8_t *)".",&pTemp);
    if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
    {
        /* placing the first octet value value in the
           structure corresponding field */
        ip_trans_addr->addr.ipv4_addr.octet_1 = dOctet;
    }
    else
    {
        ret_val = ICF_FAILURE;
    }
        pOctet =ICF_NULL;
        /* extracting octet value */
        pOctet = (icf_uint8_t *)icf_port_strtokr(pTemp,".",&pTemp);
    /* validating octet value */
    if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
    {
        /* placing second octet value value in the
           structure corresponding field */

        ip_trans_addr->addr.ipv4_addr.octet_2 = dOctet;
    }
    else
    {
        ret_val = ICF_FAILURE;
    }

    pOctet =ICF_NULL;
    /* extracting octet value */
    pOctet = (icf_uint8_t *)icf_port_strtokr(pTemp,".",&pTemp);
    /* validating octet value */
    if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
    {
        /* placing third octet value value in the
           structure corresponding field */

        ip_trans_addr->addr.ipv4_addr.octet_3 = dOctet;
    }	
    else
    {
        ret_val = ICF_FAILURE;
    }

    pOctet =ICF_NULL;
    /* extracting octet value */
    pOctet = (icf_uint8_t *)icf_port_strtokr(pTemp,".",&pTemp);
    /* validating octet value */
    if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
    {
        /* placing fourth octet value value in the
           structure corresponding field */

        ip_trans_addr->addr.ipv4_addr.octet_4 = dOctet;
    }
    else
    {
        ret_val = ICF_FAILURE;
    }
    /* assigning ip address type in the correponding field of the 
       structure */
    if (ICF_FAILURE != ret_val)
    {
        ip_trans_addr->addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
    }

    return ret_val;
}
/*-----Function added for open source replacement(IPTK REL 8.1)------END---*/
#endif

/*****************************************************************************
 ** FUNCTION:           icf_cmn_ssa_map_cc_reason_code
 **
 ** DESCRIPTION:        This function maps the CC reason codes to
 **                     SIP response codes. Previously this function was
 **                     in SSA and we need to call it from CC.
 **
 *****************************************************************************/
icf_return_t icf_cmn_ssa_map_cc_reason_code(
        icf_ssa_reason_t reason_code,
        icf_uint16_t *p_respcode)
{
    icf_return_t             ret_val = ICF_SUCCESS;

    switch(reason_code)
    {
        case ICF_CALL_CLEAR_REASON_CALL_ABANDONED :
        case ICF_CALL_CLEAR_REASON_USER_BUSY :
        case ICF_CALL_CLEAR_REASON_ATTEMPT_ON_ALTERNATE_LINE :
        case ICF_CALL_CLEAR_REASON_LINE_BUSY :
            *p_respcode = 486;
            break;
ICF_HANDLE_REINVITE_START
        case ICF_CALL_MODIFY_REASON_USER_BUSY:
ICF_HANDLE_REINVITE_END
            *p_respcode = 504;
            break;

        case ICF_CALL_CLEAR_REASON_SYSTEM_ERROR :
        case ICF_CALL_CLEAR_REASON_SERVICE_FAILURE :
        case ICF_CALL_CLEAR_REASON_MAX_AUDIO_CALLS_EXCEEDED :
        case ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE :
        case ICF_CALL_CLEAR_REASON_UNDEFINED :
        case ICF_SA_SERVER_INTERNAL_ERROR :
        case ICF_CALL_CLEAR_REASON_LINE_NOT_CONFIGURED:
            *p_respcode = 500;
            break;

        case ICF_FAX_ERR_CALL_NOT_ALLOWED:
        case ICF_FAX_ERR_INVALID_REQUEST:
        case ICF_CAUSE_FAX_PROFILE_NOT_CONFIGURED:
        /*  CSR 1-5990682 Merge */
        /*  New reason is added for stop fax failure */
        case ICF_CALL_CLEAR_REASON_STOP_FAX_FAILURE:
ICF_HANDLE_REINVITE_START
        case ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE:
ICF_HANDLE_REINVITE_END
            *p_respcode = 488;
            break;

        case ICF_CALL_CLEAR_REASON_USER_NOT_FOUND :
            *p_respcode = 404;
            break;

        case ICF_CALL_CLEAR_REASON_NOANSWER :
        case ICF_CALL_CLEAR_APP_REJECT:
            *p_respcode = 480;
            break;

        case ICF_CALL_CLEAR_REASON_REQUEST_PENDING :
        case ICF_REQUEST_PENDING :
            *p_respcode = 491;
            break;

ICF_HANDLE_REINVITE_START
        case ICF_CALL_MODIFY_REASON_REQUEST_TERMINATED:
        case ICF_CALL_CLEAR_REASON_REQUEST_TERMINATED:
            *p_respcode = 487;
            break;
ICF_HANDLE_REINVITE_END
        case ICF_CALL_CLEAR_REASON_REMOTE_MEDIA_NOT_RECEIVED:
            *p_respcode = 508;
            break;
        case ICF_CALL_CLEAR_REASON_NW_ERROR:
            *p_respcode = 400;
            break;

    case ICF_CALL_CLEAR_MEDIA_MISMATCH :
        *p_respcode = 488;
        break;

        case ICF_CALL_CLEAR_REASON_MEDIA_TRANSPORT_FAILURE :
        case ICF_CALL_CLEAR_REASON_UNABLE_TO_CREATE_MEDIA :
             *p_respcode = 503;
             break;
        case ICF_CALL_CLEAR_REASON_PRECOND_FAILURE :
             *p_respcode = 580;
             break;
        
        case ICF_CALL_CLEAR_REASON_PRACK_CALL_NOT_ALLOWED:
              *p_respcode = 420;
             break;
        default :
            *p_respcode = 500;
            break;
    }
    return ret_val;
}
/* Rel 8.2 Klocworks fix */
#ifdef ICF_TRACE_ENABLE
/******************************************************************************
 * FUNCTION:        icf_cmn_init_trace_data_st
 * DESCRIPTION:     This function is used to initialize icf_trace_data_st
 *                  type variable.
 *****************************************************************************/
icf_void_t icf_cmn_init_trace_data_st(icf_trace_data_st *p_trace_data)
{
    icf_uint32_t index = 0;
    p_trace_data->trace_timestamp.time_in_secs = 0;
    p_trace_data->trace_timestamp.time_in_millisecs = 0;
    p_trace_data->num_of_integer_values = 0;
    for (index = 0; index < ICF_MAX_INT_TRACE_DATA; index++)
    { 
        p_trace_data->int_val[index] = 0;
    }
    p_trace_data->number_of_string_values = 0;
    for (index = 0; index < ICF_MAX_STR_TRACE_DATA; index++)
    {
        p_trace_data->string_val[index].str_len = 0;
        p_trace_data->string_val[index].str[0] = '\0';
    }
    p_trace_data->trc_string.str_len = 0;
    p_trace_data->trc_string.str[0] = '\0';
    p_trace_data->p_var_data = ICF_NULL;
}
#endif
/* Rel 8.2 Klocworks fix end*/

/* Rel 8.2 Klocworks fix */
#ifdef ICF_ERROR_ENABLE
/******************************************************************************
 * FUNCTION:        icf_cmn_init_error_data_st
 * DESCRIPTION:     This function is used to initialize icf_error_data_st
 *                  type variable.
 *****************************************************************************/
icf_void_t icf_cmn_init_error_data_st(icf_error_data_st *p_error_data)
{
    icf_uint32_t index = 0;
    p_error_data->num_of_integer_values = 0;
    for (index = 0; index < ICF_MAX_INT_ERROR_DATA; index++)
    { 
        p_error_data->int_val[index] = 0;
    }
    p_error_data->number_of_string_values = 0;
    for (index = 0; index < ICF_MAX_STR_ERROR_DATA; index++)
    {
        p_error_data->string_val[index].str_len = 0;
        p_error_data->string_val[index].str[0] = '\0';
    }
    p_error_data->err_string.str_len = 0;
    p_error_data->err_string.str[0] = '\0';
}
#endif
/* Rel 8.2 Klocworks fix end*/
/*SPR 19502 Moved icf_ssa_map_interface_reason_code as 
  icf_cmn_map_interface_reason_code*/
/*****************************************************************************
 ** FUNCTION:           icf_cmn_map_interface_reason_code
 **
 ** DESCRIPTION:        This function maps the reason codes of interface messages 
 **                     to SIP response codes
 **
 *****************************************************************************/
icf_return_t icf_cmn_map_interface_reason_code(
        icf_ssa_reason_t reason_code,
        icf_uint16_t *p_respcode)
{
    icf_return_t             ret_val = ICF_SUCCESS;

    switch(reason_code)
    {
        case ICF_SA_REJECT:
            *p_respcode = 488;
            break;

        case ICF_SA_SERVER_INTERNAL_ERROR:
            *p_respcode = 500;
            break;

        case ICF_REQUEST_PENDING:
            *p_respcode = 491;
            break;

        case ICF_SA_PRECONDITION_FAILURE:
            *p_respcode = 580;
            break;

        case ICF_SA_NETWORK_ERROR:
            *p_respcode = 415;
            break;
    
    default:
        *p_respcode = 500;
    break;
    }

    return ret_val;
}
/*SPR 19590 Code changes start*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_make_hdr_list_node
 *
 * DESCRIPTION:     This function takes as input pointer to header list and
 *                  creates a nodse for the same in list of header lists in
 *                  call context.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_make_hdr_list_node(
        INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN   icf_header_list_st  *p_header_list)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_header_link_list_st *p_temp_hdr_list = ICF_NULL;
    icf_return_t            dummy_ret_val = ICF_SUCCESS;
    icf_error_t             ecode = ICF_ERROR_NONE;

	ICF_FUNCTION_ENTER(p_glb_pdb)
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, p_glb_pdb->p_call_ctx,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }   
   /*Allocate memory for the header list node*/
    ICF_MEMGET(p_glb_pdb,
            ICF_PORT_SIZEOF(icf_header_link_list_st),ICF_MEM_COMMON,\
            p_temp_hdr_list,\
            ICF_RET_ON_FAILURE,\
            &ecode, ret_val);
    /*Add the input header list in this newly created node p_data*/
    if(ICF_FAILURE == icf_cmn_clone_header_list(
                 p_glb_pdb,
                 p_header_list,
         &((p_temp_hdr_list)->header_list),
                 &ecode))
    {
       ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CMN]: \
                   icf_cmn_clone_header_list returned Failure "));
       ICF_MEMFREE(p_glb_pdb, 
                   p_temp_hdr_list, 
                   ICF_MEM_COMMON, 
                   &ecode, 
                   dummy_ret_val) 
       /*Assign ret_val as ICF_FAILURE for correct return value*/    
       ret_val = ICF_FAILURE;    
	   ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }

    /*Associate the current call context transaction id which denotes the
    * current SIP message or application API to the new node*/ 
    p_temp_hdr_list->txn_id = p_glb_pdb->p_call_ctx->txn_id;

    /*Add this node containg the txn_id and current API header list to the
      call context lists of header list*/
    ret_val = icf_cmn_add_node_to_list(p_glb_pdb,\
            &(p_glb_pdb->p_call_ctx->p_list_of_header_list),\
            p_temp_hdr_list);

    if(ICF_FAILURE == ret_val)
    {
       /* in case the node cannot be successfully added to call ctx list 
          free the temp node memory*/
       ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CMN]: \
                   icf_cmn_add_node_to_list returned Failure "));
       ICF_MEMFREE(p_glb_pdb, 
                   p_temp_hdr_list, 
                   ICF_MEM_COMMON, 
                   &ecode, 
                   dummy_ret_val) 
    }
    
	ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_make_body_list_node
 *
 * DESCRIPTION:     This function takes as input pointer to body list and
 *                  creates a nodse for the same in list of body lists in
 *                  call context.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_make_body_list_node(
        INOUT	icf_glb_pdb_st		  *p_glb_pdb,
        IN   icf_msg_body_list_st  *p_body_list)
{
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_return_t              dummy_ret_val = ICF_SUCCESS;
    icf_msg_body_link_list_st *p_temp_body_list = ICF_NULL;
    icf_error_t               ecode = ICF_ERROR_NONE;

	ICF_FUNCTION_ENTER(p_glb_pdb)
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, p_glb_pdb->p_call_ctx,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }   

   /*Allocate memory for the body list node*/
    ICF_MEMGET(p_glb_pdb,
            ICF_PORT_SIZEOF(icf_msg_body_link_list_st),ICF_MEM_COMMON,\
            p_temp_body_list,\
            ICF_RET_ON_FAILURE,\
            &ecode, ret_val);

    /*Add the input header list in this newly created node p_data*/
    if(ICF_FAILURE == icf_cmn_clone_body_list(
                 p_glb_pdb,
                 p_body_list,
      &((p_temp_body_list)->body_list),
                 &ecode))
    {
       ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CMN]: \
                   icf_cmn_clone_body_list returned Failure "));
       ICF_MEMFREE(p_glb_pdb, 
                   p_temp_body_list, 
                   ICF_MEM_COMMON, 
                   &ecode, 
                   dummy_ret_val) 
       ret_val = ICF_FAILURE;    
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }
    /*Associate the current call context transaction id which denotes the
    * current SIP message or application API to the new node*/ 
    p_temp_body_list->txn_id = p_glb_pdb->p_call_ctx->txn_id;

    /*Add node to the lists of list in the call context which shall be later
     retrieved using the txn id*/
    ret_val = icf_cmn_add_node_to_list(p_glb_pdb,\
                &(p_glb_pdb->p_call_ctx->p_list_of_body_list),\
                p_temp_body_list);
	
    if(ICF_FAILURE == ret_val)
    {
       /* in case the node cannot be successfully added to call ctx list 
          free the temp node memory*/
       ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CMN]: \
                   icf_cmn_add_node_to_list returned Failure "));
       ICF_MEMFREE(p_glb_pdb, 
                   p_temp_body_list, 
                   ICF_MEM_COMMON, 
                   &ecode, 
                   dummy_ret_val) 
    }
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_make_tag_list_node
 *
 * DESCRIPTION:     This function takes as input pointer to tag list and
 *                  creates a nodse for the same in list of tag lists in
 *                  call context.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_make_tag_list_node(
        INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN   icf_tag_list_st     *p_tag_list)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             dummy_ret_val = ICF_SUCCESS;
    icf_tag_link_list_st     *p_temp_tag_list = ICF_NULL;
    icf_error_t               ecode = ICF_ERROR_NONE;

	ICF_FUNCTION_ENTER(p_glb_pdb)
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, p_glb_pdb->p_call_ctx,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }   

   /*Allocate memory for the body list node*/
    ICF_MEMGET(p_glb_pdb,
            ICF_PORT_SIZEOF(icf_tag_link_list_st),ICF_MEM_COMMON,\
            p_temp_tag_list,\
            ICF_RET_ON_FAILURE,\
            &ecode, ret_val);
    
    /*Add the input header list in this newly created node p_data*/
    if(ICF_FAILURE == icf_cmn_clone_tag_list(
                 p_glb_pdb,
                 p_tag_list,
            &((p_temp_tag_list)->tag_list),
                 &ecode))
    {
       ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CMN]: \
                   icf_cmn_clone_tag_list returned Failure "));
       ICF_MEMFREE(p_glb_pdb, 
                   p_temp_tag_list, 
                   ICF_MEM_COMMON, 
                   &ecode, 
                   dummy_ret_val) 
       ret_val = ICF_FAILURE; 
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    } 
    /*Associate the current call context transaction id which denotes the
    * current SIP message or application API to the new node*/ 
    p_temp_tag_list->txn_id = p_glb_pdb->p_call_ctx->txn_id;

    ret_val = icf_cmn_add_node_to_list(p_glb_pdb,\
                 &(p_glb_pdb->p_call_ctx->p_list_of_tag_list),\
                 p_temp_tag_list);
    
    if(ICF_FAILURE == ret_val)
    {
       /* in case the node cannot be successfully added to call ctx list 
          free the temp node memory*/
       ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CMN]: \
                   icf_cmn_add_node_to_list returned Failure "));
       ICF_MEMFREE(p_glb_pdb, 
                   p_temp_tag_list, 
                   ICF_MEM_COMMON, 
                   &ecode, 
                   dummy_ret_val) 
    }      
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_del_header_list_node_in_call_ctx
 *
 * DESCRIPTION:     This function deletes the input node from the call ctx
 *                  list of header lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_del_header_list_node_in_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        INOUT   icf_list_st     *p_temp_list_node)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             dummy_ret_val = ICF_SUCCESS;
    icf_error_t               ecode = ICF_ERROR_NONE;
    
	ICF_FUNCTION_ENTER(p_glb_pdb)

    if((ICF_NULL != p_temp_list_node) &&
       (ICF_NULL != p_temp_list_node->p_data) &&     
       (ICF_NULL != p_glb_pdb->p_call_ctx))
    {    
        /* free the hdr list in the input node */
        ret_val = icf_cmn_free_header_list(p_glb_pdb,
          &((icf_header_link_list_st*)(p_temp_list_node->p_data))->header_list,
                 &ecode);

        if((ICF_SUCCESS == ret_val) && (ICF_NULL != p_temp_list_node->p_data))
        {
           ICF_MEMFREE(p_glb_pdb, 
              p_temp_list_node->p_data,ICF_MEM_COMMON, 
              &ecode, dummy_ret_val)
        }
        /*Delete the node from the call ctx list of hdr list*/
        ret_val = icf_cmn_delete_node_frm_list(p_glb_pdb, \
                 &(p_glb_pdb->p_call_ctx->p_list_of_header_list), 
		         &(p_temp_list_node->p_data));
        /*SPR 19835: Mark the reset of txn id in call ctx so that the same can
         * be used to mark the txn_id in call ctx as invalid.*/
        p_glb_pdb->p_call_ctx->reset_txn_id = ICF_TRUE;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_del_body_list_node_in_call_ctx
 *
 * DESCRIPTION:     This function deletes the input node from the call ctx
 *                  list of body lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_del_body_list_node_in_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        INOUT   icf_list_st     *p_temp_list_node)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             dummy_ret_val = ICF_SUCCESS;
    icf_error_t               ecode = ICF_ERROR_NONE;
    
	ICF_FUNCTION_ENTER(p_glb_pdb)

    if((ICF_NULL != p_temp_list_node) &&
       (ICF_NULL != p_temp_list_node->p_data) &&     
       (ICF_NULL != p_glb_pdb->p_call_ctx))
    {    
        /* free the body list in the input node */
        ret_val = icf_cmn_free_msg_body_list(p_glb_pdb,
          &((icf_msg_body_link_list_st*)(p_temp_list_node->p_data))->body_list,
                 &ecode);

        if((ICF_SUCCESS == ret_val) && (ICF_NULL != p_temp_list_node->p_data))
        {
           ICF_MEMFREE(p_glb_pdb, 
              p_temp_list_node->p_data,ICF_MEM_COMMON, 
              &ecode, dummy_ret_val)
        }
        /*Delete the node from the call ctx list of body list*/
        ret_val = icf_cmn_delete_node_frm_list(p_glb_pdb, \
                 &(p_glb_pdb->p_call_ctx->p_list_of_body_list), 
		         &(p_temp_list_node->p_data));
        /*SPR 19835: Mark the reset of txn id in call ctx so that the same can
         * be used to mark the txn_id in call ctx as invalid.*/
        p_glb_pdb->p_call_ctx->reset_txn_id = ICF_TRUE;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_del_tag_list_node_in_call_ctx
 *
 * DESCRIPTION:     This function deletes the input node from the call ctx
 *                  list of tag lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_del_tag_list_node_in_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        INOUT   icf_list_st     *p_temp_list_node)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_return_t             dummy_ret_val = ICF_SUCCESS;
    icf_error_t              ecode = ICF_ERROR_NONE;
    
	ICF_FUNCTION_ENTER(p_glb_pdb)
    
    if((ICF_NULL != p_temp_list_node) &&
       (ICF_NULL != p_temp_list_node->p_data) &&     
       (ICF_NULL != p_glb_pdb->p_call_ctx)) 
    {    
        /* free the tag list in the input node */
        ret_val = icf_cmn_free_tag_list(p_glb_pdb,
          &((icf_tag_link_list_st*)(p_temp_list_node->p_data))->tag_list,
                 &ecode);

        if((ICF_SUCCESS == ret_val) && (ICF_NULL != p_temp_list_node->p_data))
        {
           ICF_MEMFREE(p_glb_pdb, 
              p_temp_list_node->p_data,ICF_MEM_COMMON, 
              &ecode, dummy_ret_val)
        }
        /*Delete the node from the call ctx list of tag list*/
        ret_val = icf_cmn_delete_node_frm_list(p_glb_pdb, \
                 &(p_glb_pdb->p_call_ctx->p_list_of_tag_list), 
		         &(p_temp_list_node->p_data));
        /*SPR 19835: Mark the reset of txn id in call ctx so that the same can
         * be used to mark the txn_id in call ctx as invalid.*/
        p_glb_pdb->p_call_ctx->reset_txn_id = ICF_TRUE;

    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_curr_hdr_list_node_from_call_ctx
 *
 * DESCRIPTION:     This function fetches the current node from the call ctx
 *                  list of hdr lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_fetch_curr_hdr_list_node_from_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        OUT   icf_list_st     **p_p_temp_list_node)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, p_glb_pdb->p_call_ctx,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }   
    *p_p_temp_list_node = p_glb_pdb->p_call_ctx->p_list_of_header_list;

    if((ICF_NULL != *p_p_temp_list_node) &&
       (ICF_NULL != ((icf_header_link_list_st*)((*p_p_temp_list_node)->p_data))))     
    {    
        while(p_glb_pdb->p_call_ctx->txn_id != 
                ((icf_header_link_list_st*)((*p_p_temp_list_node)->p_data))->txn_id)
        {
            *p_p_temp_list_node = (*p_p_temp_list_node)->p_next;
            if((ICF_NULL == *p_p_temp_list_node) ||
               (ICF_NULL == ((*p_p_temp_list_node)->p_data)))
                break;
        }
    }    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;

}
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_curr_body_list_node_from_call_ctx
 *
 * DESCRIPTION:     This function fetches the current node from the call ctx
 *                  list of body lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_fetch_curr_body_list_node_from_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        OUT   icf_list_st     **p_p_temp_list_node)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, p_glb_pdb->p_call_ctx,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }   
    
    *p_p_temp_list_node = p_glb_pdb->p_call_ctx->p_list_of_body_list;

    if((ICF_NULL != *p_p_temp_list_node) &&
       (ICF_NULL != ((icf_msg_body_link_list_st*)((*p_p_temp_list_node)->p_data))))     
    {    
        while(p_glb_pdb->p_call_ctx->txn_id != 
                ((icf_msg_body_link_list_st*)((*p_p_temp_list_node)->p_data))->txn_id)
        {
            *p_p_temp_list_node = (*p_p_temp_list_node)->p_next;
            if((ICF_NULL == *p_p_temp_list_node) ||
               (ICF_NULL == ((*p_p_temp_list_node)->p_data)))
                break;
        }
    }    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_curr_tag_list_node_from_call_ctx
 *
 * DESCRIPTION:     This function fetches the current node from the call ctx
 *                  list of tag lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_fetch_curr_tag_list_node_from_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        OUT   icf_list_st     **p_p_temp_list_node)
{
    icf_return_t             ret_val = ICF_SUCCESS;
   
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb, p_glb_pdb->p_call_ctx,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
       ICF_FUNCTION_EXIT(p_glb_pdb)
       return ret_val;
    }   
    
    *p_p_temp_list_node = p_glb_pdb->p_call_ctx->p_list_of_tag_list;

    if((ICF_NULL != *p_p_temp_list_node) &&
       (ICF_NULL != ((icf_tag_link_list_st*)((*p_p_temp_list_node)->p_data))))     
    {    
        while(p_glb_pdb->p_call_ctx->txn_id != 
                ((icf_tag_link_list_st*)((*p_p_temp_list_node)->p_data))->txn_id)
        {
            *p_p_temp_list_node = (*p_p_temp_list_node)->p_next;
            if((ICF_NULL == *p_p_temp_list_node) ||
               (ICF_NULL == ((*p_p_temp_list_node)->p_data)))
                break;
        }
    }    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}
/*SPR19590 code changes End*/

/*Fix for SPR:19886 (Merged CSR-1-7302347)*/
/****************************************************************************
 * FUNCTION:           icf_cmn_compare_address
 *
 * DESCRIPTION:        This is a utility function to compare two 
 *                     icf_address_st type addresses    
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/
icf_boolean_t
icf_cmn_compare_address(
        IN icf_address_st *p_address1,
        IN icf_address_st *p_address2)
{
    icf_boolean_t   match = ICF_FALSE;
    icf_address_st  temp_addr1, temp_addr2;
    icf_uint8_t    *p_temp_str1 = ICF_NULL;
    icf_uint8_t     addr1_len_diff = 0,
                    addr2_len_diff = 0;


    /*  If any of the two address is NULL
     *  then set match to false
     */
    if((ICF_NULL == p_address1) || 
       (ICF_NULL == p_address2))
    {
        match = ICF_FALSE;
    }
    else
    {
        if('+' == p_address1->addr_val.str[0])
        {
                /* the address is preceeded by '+'*/
                addr1_len_diff = 1;
        }
        if('+' == p_address2->addr_val.str[0])
        {
                /* the address is preceeded by '+'*/
                addr2_len_diff = 1;
        }

        /* if one address is E164 and 
         * other one is SIP or one of the address
         * is Tel and other one is SIP
         * compare the user portion of SIP 
         * address with complete E164/Tel address.
         */
        icf_port_memcpy(&temp_addr1,p_address1, sizeof(icf_address_st));
        icf_port_memcpy(&temp_addr2,p_address2, sizeof(icf_address_st));

        icf_cmn_remove_parameter_and_port_from_address(&temp_addr1);
        icf_cmn_remove_parameter_and_port_from_address(&temp_addr2);

        if((((p_address1->addr_type == ICF_ADDRESS_PLAN_SIP_URI) && 
           (p_address2->addr_type == ICF_ADDRESS_PLAN_E164)) ||
           ((p_address2->addr_type == ICF_ADDRESS_PLAN_SIP_URI) && 
           (p_address1->addr_type == ICF_ADDRESS_PLAN_E164))) ||
           (((p_address1->addr_type == ICF_ADDRESS_PLAN_SIP_URI) &&
           (p_address2->addr_type == ICF_ADDRESS_PLAN_TEL_URI)) ||
           ((p_address2->addr_type == ICF_ADDRESS_PLAN_SIP_URI) &&
           (p_address1->addr_type == ICF_ADDRESS_PLAN_TEL_URI))))
        {
            /* If first address is SIP then extarct
             * the user portion from SIP address
             * and compare with complete E164 
             */
            if(ICF_ADDRESS_PLAN_SIP_URI == p_address1->addr_type)
            {
                p_temp_str1 = (icf_uint8_t *)icf_port_strtok(\
                        (icf_int8_t *)temp_addr1.addr_val.str,"@");
                if(ICF_NULL != p_temp_str1)
                {
                    if(ICF_NULL == icf_port_strcmp(&(p_temp_str1[addr1_len_diff]),
                                &(temp_addr2.addr_val.str[addr2_len_diff])))
                    {
                        match = ICF_TRUE;
                    }
                }/* ICF_NULL != p_temp_str1 */
            }/*ICF_ADDRESS_PLAN_SIP_URI == p_address1->addr_type*/
            else
            {
                /* If second address is SIP then extarct
                 * the user portion from SIP address
                 * and compare with complete E164 
                 */
                p_temp_str1 = (icf_uint8_t *)icf_port_strtok((icf_int8_t*)temp_addr2.addr_val.str,"@");
                if(ICF_NULL != p_temp_str1)
                {
                    if(ICF_NULL == icf_port_strcmp(&(p_temp_str1[addr2_len_diff]),
                                &(temp_addr1.addr_val.str[addr1_len_diff])))
                    {
                        match = ICF_TRUE;
                    }
                }/* ICF_NULL != p_temp_str1*/
            }/*ICF_ADDRESS_PLAN_SIP_URI == p_adress2->addr_type*/
        }/*  If one address is SIPS and other one is E16/TEL4 */
              
        /* If one address is E164 and other one is SIPS or one of the address is
         * Tel and other one is SIPS then compare the user portion of SIPS 
         * address with complete E164/Tel address.
         */
        else if((((p_address1->addr_type == ICF_ADDRESS_PLAN_SIPS_URI) && 
           (p_address2->addr_type == ICF_ADDRESS_PLAN_E164)) ||
           ((p_address2->addr_type == ICF_ADDRESS_PLAN_SIPS_URI) && 
           (p_address1->addr_type == ICF_ADDRESS_PLAN_E164))) ||
           (((p_address1->addr_type == ICF_ADDRESS_PLAN_SIPS_URI) &&
           (p_address2->addr_type == ICF_ADDRESS_PLAN_TEL_URI)) ||
           ((p_address2->addr_type == ICF_ADDRESS_PLAN_SIPS_URI) &&
           (p_address1->addr_type == ICF_ADDRESS_PLAN_TEL_URI))))
        {
            /* If first address is SIPS then extarct
             * the user portion from SIP address
             * and compare with complete E164 
             */
            if(ICF_ADDRESS_PLAN_SIPS_URI == p_address1->addr_type)
            {
                p_temp_str1 = (icf_uint8_t *)icf_port_strtok(\
                        (icf_int8_t *)temp_addr1.addr_val.str,"@");
                if(ICF_NULL != p_temp_str1)
                {
                    if(ICF_NULL == icf_port_strcmp(&(p_temp_str1[addr1_len_diff]),
                                &(temp_addr2.addr_val.str[addr2_len_diff])))
                    {
                        match = ICF_TRUE;
                    }
                }/* ICF_NULL != p_temp_str1 */
            }/*ICF_ADDRESS_PLAN_SIPS_URI == p_address1->addr_type*/
            else
            {
                /* If second address is SIPS then extarct
                 * the user portion from SIP address
                 * and compare with complete E164 
                 */
                p_temp_str1 = (icf_uint8_t *)icf_port_strtok(\
                        (icf_int8_t*)temp_addr2.addr_val.str,"@");
                if(ICF_NULL != p_temp_str1)
                {
                    if(ICF_NULL == icf_port_strcmp(&(p_temp_str1[addr2_len_diff]),
                                &(temp_addr1.addr_val.str[addr1_len_diff])))
                    {
                        match = ICF_TRUE;
                    }
                }/* ICF_NULL != p_temp_str1*/
            }/*ICF_ADDRESS_PLAN_SIPS_URI == p_adress2->addr_type*/
        }/*  If one address is SIPS and other one is E164/TEL */
        else
        {
            if(ICF_NULL == icf_port_strcmp(&(temp_addr1.addr_val.str[addr1_len_diff]), 
                        &(temp_addr2.addr_val.str[addr2_len_diff])))
            {
                match = ICF_TRUE;
            }
        }
    }
    return match;
}


/****************************************************************************
 * FUNCTION:           icf_cmn_remove_parameter_and_port_from_address
 *
 * DESCRIPTION:        This is a utility function to that
 *                     will return the string that does not contain the 
 *                     semicolon and colon
 *
 *
 ***************************************************************************/
icf_void_t
icf_cmn_remove_parameter_and_port_from_address(icf_address_st *p_address)
{
    icf_uint8_t     temp_addr[ICF_MAX_STR_LEN];
    icf_uint8_t      *p_str1 = ICF_NULL;
    icf_int8_t       *p_temp = ICF_NULL;
    
    if(ICF_NULL == p_address)
    {
        ICF_PRINT(((icf_uint8_t*)"[CMN] : p_address is NULL\n"))
        return ;
    }
    icf_port_strcpy (temp_addr, p_address->addr_val.str);
    /*Check if port is present*/
    p_str1 = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)temp_addr," :", &p_temp);
    p_temp = ICF_NULL;
    /*Check if any sip param is present*/
    p_str1 = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)temp_addr," ;", &p_temp);
    if(ICF_NULL != p_str1)
    {
        /*Copy the string without port and sip param*/
        icf_port_strcpy(p_address->addr_val.str, p_str1);
        p_address->addr_val.str_len = 
                   (icf_uint16_t)icf_port_strlen(p_str1);
    }

}

/* Fix for SPR 20412: CSR 1-8223807 merged*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_hdr_name_list_into_str_buffer
 *
 * DESCRIPTION:     The function converts the header list present in 
 *                  icf_header_name_list_st into string buffer by appending
 *                  all the headers and seperated by comma(,).
 *
 * This function is written as per UATK requirement for CSR 1-8290042.
 * The list of headers passed to the microua APIs from the application
 * should be a character array
 * Example: Char buffer[] = "allow ,supported ,session-expires ,unknown "
 * Kindly also note that in the above example each header name contains one
 * space at the end to store a null character when tokenized.
 ******************************************************************************/
icf_void_t icf_cmn_convert_hdr_name_list_into_str_buffer(
           OUT icf_uint8_t              **p_hdr_name_str_buffer,
           IN  icf_header_name_list_st  *p_hdr_name_list)
{
        icf_uint8_t         count = 0;
        icf_uint8_t         hdr_count = 0;
        icf_header_name_st  *p_current_hdr = ICF_NULL; 


        if((0 == p_hdr_name_list->count) ||
           (ICF_NULL == p_hdr_name_list) ||
           (ICF_NULL == p_hdr_name_str_buffer))
        {
            return ;
        }

        hdr_count = p_hdr_name_list->count;
        p_current_hdr = p_hdr_name_list->hdr;
        
        /* Copy the first header name in p_hdr_name_str_buffer.*/
        icf_port_strncpy(*p_hdr_name_str_buffer,
                            p_current_hdr->hdr_name.str,ICF_MAX_STR_LEN);

        /* If sigle header is present then return aftere addiing a space
         * else append the reamining headers also seperated by " ,".
         */
        if(1 == hdr_count)
        {
            icf_port_strcat(*p_hdr_name_str_buffer, " ");
            return;
        }
        else
        {
            icf_port_strcat(*p_hdr_name_str_buffer, " ,");
            p_current_hdr = p_current_hdr->next;
            hdr_count--;
        }

        /* Append the remaining headers except last*/
        for(count = hdr_count;(hdr_count > 1) && (ICF_NULL != p_current_hdr) ;
                                                                 hdr_count--)
        {
          icf_port_strcat(*p_hdr_name_str_buffer, p_current_hdr->hdr_name.str);
          icf_port_strcat(*p_hdr_name_str_buffer, " ,");
          p_current_hdr = p_current_hdr->next;
        }

        /* Append the last header*/
        icf_port_strcat(*p_hdr_name_str_buffer, p_current_hdr->hdr_name.str);
        icf_port_strcat(*p_hdr_name_str_buffer, " ");

}

