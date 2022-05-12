/*********************************************************************** *
 * FILE NAME :      icf_sm_util.c
 *
 * DESCRIPTION:     This file contains the code of the utility 
 *                  functions of the SM handler(SM)
 *                  of the Packet Phone.
 *
 * REVISION HISTORY:
 *
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 10-Mar-2005  Jyoti Narula        SM LLD      Initial
 * 06-Apr-2005  Jyoti Narula        Code Review Comments Updation
 * 11-May-2005  Jyoti Narula        LicMgmt LLD New feature
 * 14-Mar-2006  Amit Sharma                 CFG merged with ICF 2.0
 * 08-Apr-2008  Tarun Gupta         SPR 18355   Merged Windows Porting Changes
 * 03-Jul-2008  Anurag Khare        SPR 18684   CSR 1-6044375 Merge
 * 04-Mar-2009 Saurabh Gupta         SPR 19349      Klocworks fix
 *
 * Copyright (c) 2008, Aricent.
 *********************************************************************/


/*--Header files needed during license_mgmt--------------------------*/
#include "icf_feature_flags.h"
#include <ctype.h>
#ifdef ICF_PORT_WINDOWS
#include <winsock2.h>

#else
#include <unistd.h>
#endif


#if !defined (ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_SYMBIAN) && !defined(ICF_PORT_VXWORKS)
#include <strings.h>
#endif

#ifdef ICF_PORT_SYMBIAN
#include <string.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#ifndef ICF_PORT_WINCE
#include <sys/types.h>
#include <sys/stat.h>
#endif


/*-------Header Files that are from ICF-----------*/
#include "icf_internal_api_id.h"
#include "icf_common_interface_types.h"
#include "icf_common_defs.h"
#include "icf_sm_prototypes.h"
#include "icf_sm_defs.h"
#include "icf_dbm_prototypes.h"
#include "icf_macro.h"
#include "icf_sm_macro.h"
#include "icf_port_defs.h"
#include "icf_port_prototypes.h"

extern icf_return_t icf_send_to_app(icf_glb_pdb_st*    p_glb_pdb,
                icf_msg_st*    p_msg_data,
                icf_void_st*    p_routing_info,
                icf_error_t*    p_ecode);


/*************************************************************************
 *     FUNCTION NAME: icf_sm_get_module_glb_data
 *
 *     DESCRIPTION : This function returns the address of data block for
 *                   the SM Module.Called by sm_check_trace/error to
 *                   avoid recursive calling when global data was
 *                   asked from DBM directly.
 *
 *     RETURNS     : Success if data is retreived successfully else failure     
 *
 *************************************************************************/
icf_return_t
icf_sm_get_module_glb_data(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        IN    icf_int_module_id_t   module_id,
        OUT   icf_void_t            **p_p_data)
{
    icf_return_t ret_val = ICF_FAILURE;

/* ICF_PRINT(("\n[SM] Entering function icf_sm_get_module_glb_data"));*/
    if(ICF_NULL != p_glb_pdb)
    {
       if(ICF_NULL != p_glb_pdb->p_glb_data)
       {
	    if(ICF_NULL != p_glb_pdb->p_glb_data->p_module_data[module_id])
	    {
		    *p_p_data = p_glb_pdb->p_glb_data->p_module_data[module_id];
		    ret_val = ICF_SUCCESS;
	    }
	    else
	    {
		    ICF_PRINT(((icf_uint8_t *)"\n[SM] No Module Data availale for SM Handler"));
	    }
       }
       else
       {
           ICF_PRINT(((icf_uint8_t *)"\n[SM] No Global Data availale for SM Handler"));
       }
   }
   else
   {
      ICF_PRINT(((icf_uint8_t *)"\n[SM] No Global Pdb"));
  }

/*  ICF_PRINT(("\n[SM] Exiting function icf_sm_get_module_glb_data \
                with ret_val = %d",ret_val));*/


        return ret_val;
}

#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 * FUNCTION:    icf_sm_check_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              to check whether the trace level and the type requested
 *              is enabled and is to be reported or not. 
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received trace_id 
 *              can be reported.
 *              ICF_FAILURE to indicate:
 *                  -Received trace_id is INAVLID
 *                  -DBM failure to obtain SM Global data.
 *                  -Received trace_id cannot be reported.
 *                  
 *              
 **********************************************************************/
icf_return_t icf_sm_check_trace(
        INOUT    icf_glb_pdb_st *p_glb_pdb,
        IN       icf_trace_id_t trace_id)
{

#ifdef ICF_TRACE_ENABLE
    icf_return_t ret_val = ICF_SUCCESS;

    /* Variables used to store the 2 categories of traces 
     * to compare: recd_ and global_. */

    icf_trace_type_t recd_trace_type =  ICF_NULL;
    icf_trace_type_t glb_trace_type =  ICF_NULL;
    icf_trace_level_t recd_trace_level =  ICF_NULL;
    icf_trace_level_t glb_trace_level =  ICF_NULL;

    icf_boolean_t glb_trace_status =  ICF_FALSE;

    icf_sm_glb_data_st *p_sm_data =  ICF_NULL;


    /* To raise the trace for function entry. */
/*   ICF_PRINT(("\n[SM] Entering function icf_sm_check_trace"));*/


    /*Checking for Received trace_id's range.Processing begins*/

    if (ICF_TRACE_NORMAL_START <= trace_id  &&
            ICF_MAX_NUM_OF_TRACE_IDS > trace_id)
        /* Received trace_id is within range.Processing begins*/
    {/* Received trace_id is within range.Processing begins*/

        /* Received trace_id is within range.Processing begins*/

        ret_val = icf_sm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_SM),
                (icf_void_t *)&p_sm_data);


        if (ICF_FAILURE !=  ret_val)
        {

            /* Successul retrieval of SM data: processing begins. */
            glb_trace_status = p_sm_data->trace_status;
            if (ICF_FALSE == glb_trace_status)
            { 
                ret_val = ICF_FAILURE;

            }
            else
            {

                /* Processing of trace_status=TRUE begins. */

                recd_trace_level = p_glb_pdb->p_glb_data->
                    trace_data[trace_id].trace_level;
                recd_trace_type = p_glb_pdb->p_glb_data->
                    trace_data[trace_id].trace_type;
                glb_trace_type = p_sm_data->trace_type;
                glb_trace_level = p_sm_data->trace_level;

                
                if ( (recd_trace_level <= glb_trace_level) &&
                        (recd_trace_type & glb_trace_type))
                {
                    ret_val = ICF_SUCCESS;
                }
                else
                {
                    ret_val = ICF_FAILURE;
                }

            }/* Processing of trace_status=TRUE ends. */

        }/* DBM successul with SM data: processing ends. */


    }/* received trace_id was within valid range: processing ends. */

    else
    {

        ret_val = ICF_FAILURE;
    }/* Received trace_id found to be INVALID.*/

    /*Checking for Received trace_id's range.Processing Ends.*/


/*  ICF_PRINT(("\n[SM] Exit function icf_sm_check_trace with retval %d",\
                ret_val));*/


    return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    p_glb_pdb = p_glb_pdb;
    trace_id = trace_id;
    return ret_val;

#endif


}/* Function icf_sm_check_trace() ends here. */

/**********************************************************************
 * FUNCTION:    icf_sm_report_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              if the trace level and the type requested is enabled 
 *              and is to be reported. It sends the trace data to
 *              the SM.
 *
 * RETURNS:     ICF_SUCCESS in case of success and ICF_FAILURE
 *              otherwise.Failure occurs when:
 *                  -The memory fails to be allocated 
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND macro returned failure.
 *              
 ********************************************************************/
icf_return_t icf_sm_report_trace(
        INOUT    icf_glb_pdb_st *p_glb_pdb,
        IN       icf_trace_id_t trace_id,
        IN       icf_trace_data_st *p_trace_data)
{
#ifdef ICF_TRACE_ENABLE
#ifdef ICF_REPORT_ENABLE

    icf_return_t  ret_val = ICF_SUCCESS;
    icf_error_t ecode = ICF_NULL;
    icf_msg_st *p_sm_msg = ICF_NULL;

    /* Variable used to fill values in int_val and string_val of trace_data*/
    icf_uint8_t   value_index =  ICF_NULL;

    icf_uint16_t api_len =  ICF_NULL;
    icf_cc_sm_report_trace_ind_st *p_sm_msg_payload = ICF_NULL;


    /* To raise the trace for function entry. */
/*   ICF_PRINT(("\n[SM] Entering Function icf_sm_report_trace"));*/

    api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
        ICF_PORT_SIZEOF(icf_cc_sm_report_trace_ind_st);

    /* To obtain memory for p_sm_msg. */

    p_sm_msg = p_sm_msg = icf_port_memget(p_glb_pdb->p_glb_data->p_glb_port_info,
		     (icf_uint32_t)api_len, 
            (icf_pool_id_t)ICF_MEM_COMMON, &(ecode)); 

    if (ICF_NULL != p_sm_msg)
    {
        /* When the memory is successfully allocated to 
         * the p_sm_msg: processing begins */

        /* Fill the header of the message to be send */

        p_sm_msg->hdr.api_id = ICF_CC_SM_REPORT_TRACE_IND;
        p_sm_msg->hdr.source_id = ICF_MODULE_ICF;
        p_sm_msg->hdr.destination_id = ICF_MODULE_PA;
        p_sm_msg->hdr.api_length = ICF_PORT_SIZEOF(
                            icf_api_header_st)+
            ICF_PORT_SIZEOF(icf_cc_sm_report_trace_ind_st);

        /* Fill the payload */

        p_sm_msg_payload = (icf_cc_sm_report_trace_ind_st *)
            (p_sm_msg->payload);

        /* Copy the trace id in the report API */

        p_sm_msg_payload->trace_id = trace_id;

        if (ICF_NULL != p_trace_data)
        {
            /* Incase the trace_data is NOT NULL: Processing Begins. */

            /* Copy the data from p_trace_data to trace_data in the report API */

            p_sm_msg_payload->trace_data.num_of_integer_values = 
                p_trace_data->num_of_integer_values;

            for(value_index=0;value_index < p_trace_data->num_of_integer_values;
                    value_index++)
            {
                p_sm_msg_payload->trace_data.int_val[value_index] =
                    p_trace_data->int_val[value_index];
            }

            p_sm_msg_payload->trace_data.number_of_string_values = 
                p_trace_data->number_of_string_values; 

            for(value_index=0;value_index < p_trace_data->number_of_string_values;
                    value_index++)
            {
                ICF_SM_STRCPY(p_sm_msg_payload->trace_data.
                        string_val[value_index].str,
                        p_trace_data->string_val[value_index].str)

            }
        } /* Incase the trace_data is NOT NULL: Processing Ends. */

        else
        {
            /* trace_data is NULL. */
            p_sm_msg_payload->trace_data.num_of_integer_values = 0;
            p_sm_msg_payload->trace_data.number_of_string_values = 0;

        }



        /* Send the message */
        ICF_SEND(p_glb_pdb,p_sm_msg,ret_val,&(ecode))

            if(ICF_SUCCESS == ret_val)
            {      /* Free memory */
                ret_val =  ret_val = icf_port_memfree(p_glb_pdb->p_glb_data->p_glb_port_info,
					     p_sm_msg, 
                        (icf_pool_id_t)ICF_MEM_COMMON, &(ecode)); 

                p_sm_msg = ICF_NULL;

            }/* Memory freed when SEND successful. */
            else
            {
                ret_val = icf_port_memfree(p_glb_pdb->p_glb_data->p_glb_port_info,
					     p_sm_msg, 
                        (icf_pool_id_t)ICF_MEM_COMMON, &(ecode)); 

                p_sm_msg = ICF_NULL;

                ret_val = ICF_FAILURE;
            }/* Memory freed when SEND failed. */

    }/* memory allocated for p_sm_msg: processing ends.*/

    else
    {/* cmn_memget returned a NULL Pointer for p_sm_msg: 
        Memory allocation failed. */
        ret_val = ICF_FAILURE;
    }


    /* To raise the trace for function exit. */
/*   ICF_PRINT(("\n[SM] Exiting Function icf_sm_report_trace\
                with ret_val = %d",ret_val));*/

        return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    icf_trace_id_t use_trace_id;
    
    p_glb_pdb = p_glb_pdb;
    use_trace_id = trace_id;
    p_trace_data = p_trace_data;
    return ret_val;

#endif /* REPORT_ENABLE */

    
#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    icf_trace_id_t use_trace_id;
    
    p_glb_pdb = p_glb_pdb;
    use_trace_id = trace_id;
    p_trace_data = p_trace_data;
    return ret_val;

#endif /* TRACE_ENABLE */



}/* Function icf_report_trace() ends here. */


/**********************************************************************
 * FUNCTION:    icf_sm_report_app_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              if the trace level and the type requested is enabled
 *              and is to be reported. It sends the trace data to
 *              the SM.
 *
 * RETURNS:     ICF_SUCCESS in case of success and ICF_FAILURE
 *              otherwise.Failure occurs when:
 *                  -The memory fails to be allocated
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND macro returned failure.
 *
 ********************************************************************/
 icf_return_t icf_sm_report_app_trace(
           INOUT    icf_glb_pdb_st *p_glb_pdb,
           IN       icf_trace_id_t trace_id,
           IN       icf_trace_data_st *p_trace_data)
 {
#ifdef ICF_TRACE_ENABLE
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_error_t ecode = ICF_NULL;
    icf_msg_st *p_sm_msg = ICF_NULL;

    /* Variable used to fill values in int_val and string_val of trace_data*/
    icf_uint8_t   value_index =  ICF_NULL;
    icf_uint16_t api_len =  ICF_NULL;
    icf_cc_sm_report_trace_ind_st *p_sm_msg_payload = ICF_NULL;
    icf_void_st *p_routing_info = ICF_NULL;
    icf_app_conf_st *p_app_conf = ICF_NULL;
     /* To raise the trace for function entry. */
    ICF_PRINT(((icf_uint8_t *)"\n[SM] Entering Function icf_sm_report_app_trace"));
     /* To obtain memory for p_sm_msg. */
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
                    ICF_PORT_SIZEOF(icf_cc_sm_report_trace_ind_st) +
                    ICF_PORT_SIZEOF(icf_trace_var_data_st);
     p_sm_msg = icf_port_memget(p_glb_pdb->p_glb_data->p_glb_port_info,
              (icf_uint32_t)api_len,
              (icf_pool_id_t)ICF_MEM_COMMON, &(ecode));
     if (ICF_NULL != p_sm_msg)
     {
        /* When the memory is successfully allocated to
         * the p_sm_msg: processing begins */
        /* Fill the header of the message to be send */  
         p_sm_msg->hdr.api_id = ICF_CC_SM_REPORT_TRACE_IND;
         p_sm_msg->hdr.source_id = ICF_MODULE_ICF;
         p_sm_msg->hdr.destination_id = ICF_MODULE_PA;
         p_sm_msg->hdr.app_id =
                 p_glb_pdb->p_glb_data->app_trace_info.app_id;
         p_sm_msg->hdr.call_id =
                 p_glb_pdb->p_glb_data->app_trace_info.call_id;
         p_sm_msg->hdr.api_length = ICF_PORT_SIZEOF(
                     icf_api_header_st)+
                     ICF_PORT_SIZEOF(icf_cc_sm_report_trace_ind_st);
         /* Fill the payload */
         p_sm_msg_payload = (icf_cc_sm_report_trace_ind_st *)
                                         (p_sm_msg->payload);
         /* Copy the trace id in the report API */
         p_sm_msg_payload->trace_id = trace_id;
         p_sm_msg_payload->trace_type =
               p_glb_pdb->p_glb_data->app_trace_data[trace_id].trace_type;
         if (ICF_NULL != p_trace_data)      
         {
            /* Incase the trace_data is NOT NULL: Processing Begins. */
            /* Copy the data from p_trace_data to trace_data in the report API */
            p_sm_msg_payload->trace_data.trace_timestamp =
                  p_trace_data->trace_timestamp;
            p_sm_msg_payload->trace_data.num_of_integer_values =
                  p_trace_data->num_of_integer_values;
            for(value_index=0;
				(value_index < p_trace_data->num_of_integer_values) && 
				(value_index < ICF_MAX_INT_TRACE_DATA); 
				value_index++)
            {
               p_sm_msg_payload->trace_data.int_val[value_index] =
                      p_trace_data->int_val[value_index];
            }
            p_sm_msg_payload->trace_data.number_of_string_values =
                    p_trace_data->number_of_string_values;
            for(value_index=0;
				(value_index < p_trace_data->number_of_string_values) && 
				(value_index < ICF_MAX_STR_TRACE_DATA);
                  value_index++)
            {
               ICF_SM_STRCPY(p_sm_msg_payload->trace_data.string_val[value_index].str,
                          p_trace_data->string_val[value_index].str)
               p_sm_msg_payload->trace_data.string_val[value_index].str_len =
                     (icf_uint16_t)icf_port_strlen(p_trace_data->string_val[value_index].str);

            }
            ICF_SM_STRCPY(p_sm_msg_payload->trace_data.trc_string.str,
                      p_glb_pdb->p_glb_data->app_trace_data[trace_id].p_trace_string)
            p_sm_msg_payload->trace_data.trc_string.str_len =
                       (icf_uint16_t) icf_port_strlen(p_sm_msg_payload->trace_data.trc_string.str);
            p_sm_msg_payload->trace_data.p_var_data =
                       p_trace_data->p_var_data;
         }/* Incase the trace_data is NOT NULL: Processing Ends. */
         else
         {
            /* trace_data is NULL. */
            p_sm_msg_payload->trace_data.num_of_integer_values = 0;
            p_sm_msg_payload->trace_data.number_of_string_values = 0;
         }

         if(ICF_SUCCESS == 
           icf_cfg_fetch_app_conf(p_glb_pdb, 
                     p_glb_pdb->p_glb_data->app_trace_info.app_id,
                     &p_app_conf,
                     &ecode))
         {
           if((ICF_NULL != p_app_conf) &&
                   (ICF_TRUE == p_app_conf->is_valid))
            {
               p_routing_info = &(p_app_conf->routing_info);
               p_sm_msg->hdr.destination_id = p_app_conf->app_source_id;
            }
         }
         
         if(ICF_NULL == p_routing_info)
         {
            /* Raise Am */
            ICF_ALARM(((icf_uint8_t *)" Not able to fetch Routing info For APP_ID %d",p_sm_msg->hdr.app_id));
         }
         else
         {
             ret_val = icf_send_to_app(p_glb_pdb, (icf_msg_st *)p_sm_msg,
                       p_routing_info, &ecode);
         }

         if(ICF_SUCCESS == ret_val)
         {
           /* Free memory */
           ret_val = icf_port_memfree(
                    p_glb_pdb->p_glb_data->p_glb_port_info,
                    p_sm_msg,
                    (icf_pool_id_t)ICF_MEM_COMMON, &(ecode));
           p_sm_msg = ICF_NULL;         
          }/* Memory freed when SEND successful. */
          else
          {
             ret_val = icf_port_memfree(p_glb_pdb->p_glb_data->p_glb_port_info,
                       p_sm_msg,
                       (icf_pool_id_t)ICF_MEM_COMMON, &(ecode));
             p_sm_msg = ICF_NULL;
             ret_val = ICF_FAILURE;
          }/* Memory freed when SEND failed. */

     }/* memory allocated for p_sm_msg: processing ends.*/
     else
     {
         /* cmn_memget returned a NULL Pointer for p_sm_msg:
            Memory allocation failed. */
         ret_val = ICF_FAILURE;   
     }
    
    return ret_val;
#else
     icf_return_t ret_val  =  ICF_SUCCESS;
     icf_trace_id_t use_trace_id;
     p_glb_pdb = p_glb_pdb;
     use_trace_id = trace_id;
     p_trace_data = p_trace_data;
     return ret_val;
#endif /* TRACE_ENABLE */

}

#endif

#ifdef ICF_ERROR_ENABLE
/**********************************************************************
 * FUNCTION:    icf_sm_check_error
 *
 * DESCRIPTION: This function is called by the function 
 *              icf_cmn_raise_error_n_trace
 *              to check whether the error level and the type requested
 *              is enabled and is to be reported or not. 
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received error_id 
 *              can be reported.
 *              ICF_FAILURE to indicate:
 *                  -received error_id is out of range.
 *                  -Failure to obtain SM Global data.
 *                  -Received error_id cannot be reported.
 *              
 *********************************************************************/
icf_return_t icf_sm_check_error(
        INOUT    icf_glb_pdb_st *p_glb_pdb,
        IN       icf_error_id_t error_id)
{
#ifdef ICF_ERROR_ENABLE


    icf_return_t        ret_val = ICF_SUCCESS;

    /* Variables used to store the 2 categories of traces 
     * to compare: recd_ and global_. */

    icf_error_type_t    recd_error_type =  ICF_NULL;
    icf_error_type_t   glb_error_type =  ICF_NULL;
    icf_error_level_t   recd_error_level =  ICF_NULL;
    icf_error_level_t glb_error_level =  ICF_NULL;
    icf_boolean_t       glb_error_status =  ICF_FALSE;

    icf_sm_glb_data_st  *p_sm_data =  ICF_NULL;

    /* To raise the trace for function entry. */
/*  ICF_PRINT(("\n[SM] Entering function icf_sm_check_error \
                with the error_id as = %d",error_id));*/

        if(error_id > ICF_ERROR_START &&
                error_id < ICF_MAX_NUM_OF_ERROR_IDS)
        {
            
            /* received error_id found within range: Processing Begins */      

            ret_val = icf_sm_get_module_glb_data(p_glb_pdb,
                    (icf_module_id_t)(ICF_INT_MODULE_SM),
                    (icf_void_t *)&p_sm_data);


            if (ICF_FAILURE != ret_val)
            {
                

                /* SM GLobal Data Retrieved Successfully. Processing Begins*/

                glb_error_status = p_sm_data->error_status;

                if (ICF_FALSE == glb_error_status)
                {
                    ret_val = ICF_FAILURE;
                }

                else
                {
                    

                    recd_error_level = p_glb_pdb->p_glb_data->
                        error_data[error_id].error_level;
                    recd_error_type = p_glb_pdb->p_glb_data->
                        error_data[error_id].error_type;
                    glb_error_type = p_sm_data->error_type;
                    glb_error_level = p_sm_data->error_level;

                    

                    if ( (recd_error_level <= glb_error_level) && 
                            (recd_error_type & glb_error_type))
                    {
                        ret_val = ICF_SUCCESS;
                    }
                    else
                    {
                        ret_val = ICF_FAILURE;
                    }
                }/* Processing glb_error_status=TRUE ends. */

            } /* SM GLobal Data Retrieved Successfully. Processing Ends */


        }/* Received error_id within range Processing ends*/

        else
        {
            /* received error_id out of range.*/

            ret_val = ICF_FAILURE;
        }/* Received error_id found to be out of range. */


/*  ICF_PRINT(("\n[SM] Exiting function icf_sm_check_error \
                with ret_val = %d",ret_val));*/

        return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    p_glb_pdb = p_glb_pdb;
    error_id = error_id;
    return ret_val;

#endif

}/* Function icf_sm_check_error() ends here. */

/**********************************************************************
 * FUNCTION:    icf_sm_report_error
 * 
 * DESCRIPTION: This function is called by the function 
 *              icf_cmn_rasie_error_n_trace if the error level
 *              and the type requested is enabled and is to be reported.
 *              It sends the error_data to the SM.
 *
 * RETURNS:     ICF_SUCCESS in case of success and 
 *              ICF_FAILURE otherwise.Failure occurs when:
 *                  -The memory fails to be allocated 
 *                   for icf_msg_st, that has to be sent to SM.
 *                  -SEND Macro returned Failure.
 *              
 ***********************************************************************/
icf_return_t icf_sm_report_error(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        IN      icf_error_id_t error_id,
        IN      icf_error_data_st *p_error_data,
        INOUT   icf_error_t *p_ecode)

{
#ifdef ICF_ERROR_ENABLE

    icf_return_t  ret_val = ICF_SUCCESS;
    icf_msg_st *p_sm_msg =  ICF_NULL;

    /* To be used in copying int_val and string_val of error_data. */
    icf_uint8_t val_index =  ICF_NULL;

    icf_uint16_t api_len =  ICF_NULL;

    icf_cc_sm_report_error_ind_st
        *p_sm_msg_payload = ICF_NULL;
	icf_void_st				*p_routing_info = ICF_NULL;
    icf_app_conf_st			*p_app_conf = ICF_NULL;
    /* To print for function entry. */
/*  ICF_PRINT(("\n[SM] Entering Function icf_sm_report_error"));*/
	if(ICF_TRUE == p_glb_pdb->error_info.status)
	{

		api_len = ICF_PORT_SIZEOF(icf_api_header_st) +
			ICF_PORT_SIZEOF(icf_cc_sm_report_error_ind_st);

		/* To get memory for p_sm_msg: CHECK needed here.*/

    p_sm_msg = icf_port_memget(p_glb_pdb->p_glb_data->p_glb_port_info,
		(icf_uint32_t)api_len, 
            (icf_pool_id_t)ICF_MEM_COMMON, p_ecode); 

    if (ICF_NULL != p_sm_msg)
    {
        /* Memory successfully allocated for p_sm_msg
         * Processing Begins */

        /* Fill the header of the message to be send */
        p_sm_msg->hdr.api_id = ICF_CC_SM_REPORT_ERROR_IND; 
        p_sm_msg->hdr.source_id = ICF_MODULE_ICF;
        p_sm_msg->hdr.destination_id = ICF_MODULE_PA;
/*        
        p_sm_msg->hdr.app_id = p_error_data->int_val[ICF_MAX_INT_ERROR_DATA-1];
        p_sm_msg->hdr.call_id = p_error_data->int_val[ICF_MAX_INT_ERROR_DATA-2];
*/        
        p_sm_msg->hdr.api_length = ICF_PORT_SIZEOF(
                        icf_api_header_st)+
            ICF_PORT_SIZEOF(icf_cc_sm_report_error_ind_st);

        /* Fill the payload */
        p_sm_msg_payload = (icf_cc_sm_report_error_ind_st *)
            (p_sm_msg->payload);


        /* Copy the error id in the report API */

        p_sm_msg_payload->error_id = error_id;

        if (ICF_NULL != p_error_data)
        {
            /* Incase the p_error_data is not null. Processing begins*/
            p_sm_msg->hdr.app_id = p_error_data->int_val[ICF_MAX_INT_ERROR_DATA-1];
            /*  CSR 1-6044375 Merge */
            p_sm_msg->hdr.call_id = (icf_call_id_t)(p_error_data->int_val[ICF_MAX_INT_ERROR_DATA-2]);
            /* Copy the data from p_error_data to error_data
             *  in the report API */

            p_sm_msg_payload->error_data.num_of_integer_values = 
                p_error_data->num_of_integer_values;

            for (val_index = 0; 
				(val_index < p_error_data->num_of_integer_values) && 
				(val_index < ICF_MAX_INT_ERROR_DATA);
                    val_index++)
            {
                p_sm_msg_payload->error_data.int_val[val_index] = 
                    p_error_data->int_val[val_index];

            }

            p_sm_msg_payload->error_data.number_of_string_values = 
                p_error_data->number_of_string_values; 

            for (val_index = 0;
				(val_index < p_error_data->number_of_string_values) && 
				(val_index < ICF_MAX_STR_ERROR_DATA);
                    val_index++)
            {
                ICF_SM_STRCPY(p_sm_msg_payload->error_data.
                        string_val[val_index].str,
                        p_error_data->string_val[val_index].str)
				p_sm_msg_payload->error_data.string_val[val_index].str_len\
                    = p_error_data->string_val[val_index].str_len;
            }
            ICF_SM_STRCPY(p_sm_msg_payload->error_data.
                    err_string.str,
                        p_glb_pdb->p_glb_data->error_data[error_id].p_error_string)
            p_sm_msg_payload->error_data.err_string.str_len = 
		(icf_uint16_t)icf_port_strlen(p_glb_pdb->p_glb_data->error_data[error_id].p_error_string);
        }/* Incase the p_error_data is not null. Processing Ends. */
        else
        {
            /* Incase the error_data is NULL.*/

            p_sm_msg_payload->error_data.num_of_integer_values = 0;
            p_sm_msg_payload->error_data.number_of_string_values = 0;

        }


        /* Fetch routing info for the app_id
         */
        if(ICF_SUCCESS == 
                icf_cfg_fetch_app_conf(p_glb_pdb,p_sm_msg->hdr.app_id,&p_app_conf,
                    p_ecode))
        {
            {
                if((ICF_NULL != p_app_conf) &&
                        (ICF_TRUE == p_app_conf->is_valid))
                {
                    p_routing_info = &(p_app_conf->routing_info);
                    p_sm_msg->hdr.destination_id = p_app_conf->app_source_id;
                }
            }
        }
        if(ICF_NULL == p_routing_info)
        {
            /* Raise Alarm */
            ICF_ALARM(((icf_uint8_t *)" Not able to fetch Routing info For APP_ID %d",p_sm_msg->hdr.app_id));

        }
        else
        {
            ret_val = icf_send_to_app(p_glb_pdb, (icf_msg_st *)p_sm_msg,
                            p_routing_info, p_ecode);
        }

            if(ICF_SUCCESS == ret_val)
            {      /* Free memory */
                ret_val = icf_port_memfree(p_glb_pdb->p_glb_data->p_glb_port_info,
					     p_sm_msg, 
                        (icf_pool_id_t)ICF_MEM_COMMON, p_ecode); 

                p_sm_msg = ICF_NULL;

            }/* Memory freed when SEND successful. */
            else
            {
                ret_val = icf_port_memfree(p_glb_pdb->p_glb_data->p_glb_port_info,
					     p_sm_msg,
                        (icf_pool_id_t)ICF_MEM_COMMON, p_ecode); 

                p_sm_msg = ICF_NULL;

                ret_val = ICF_FAILURE;
            }/* Memory freed when SEND failed. */


    }/* Memory successfully allocated for p_sm_msg
      * Processing Ends */


    else
    {/* cmn_memget returned a NULL Pointer for p_sm_msg: 
        Memory allocation failed. */
        ret_val = ICF_FAILURE;
    }

     /* To print for function exit. */
    return ret_val;
}
else
    return ICF_SUCCESS;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    icf_error_id_t use_error_id;
    
    p_glb_pdb = p_glb_pdb;
    use_error_id  = error_id;
    p_error_data = p_error_data;
    p_ecode = p_ecode;

    return ret_val;

#endif /* ERROR_ENABLE */


}/* Function icf_sm_report_error() ends here. */
#endif


#ifdef ICF_STAT_ENABLED
/*********************************************************************
 * FUNCTION:    icf_sm_get_stats_index
 *
 * DESCRIPTION: This function is called by the function 
 *              icf_get_stats_hdlr() and icf_set_stats_hdlr()
 *              to retrieve the  of current stats group and the
 *              next stats group.
 *
 * RETURNS:     ICF_SUCCESS in case of success and
 *              ICF_FAILURE when the recd_stats_id is not a group id.
 *              
 *************************************************************/

icf_return_t icf_sm_get_stats_index(
        IN      icf_stats_id_t recd_stats_id,
        INOUT   icf_sm_stats_index_st *p_sm_stats_index)
{
#ifdef ICF_STAT_ENABLED

    icf_return_t ret_val = ICF_SUCCESS;

    icf_uint8_t array_index =  ICF_NULL;

    /* Array used to store the stats_id for the stat groups.
     * To be used in the loop to retrieve the values of
     * all members of one group. 
     * Uptil Now(April 2005) , the toolkit is only supporting
     * following group_stats_ids. */

    icf_uint16_t index_stats_group[ICF_SM_MAX_STATS_GROUP_COUNT]=
    {
        ICF_STAT_GROUP_CALL_RELATED,
        ICF_STAT_GROUP_SERV_RELATED,
        ICF_STAT_GROUP_REG_RELATED,
        ICF_STAT_GROUP_SYS_RELATED,
        ICF_STAT_GROUP_TMR_RELATED};



    ICF_PRINT(("\n[SM] Entering Function icf_sm_get_stats_index. "));
    /* Checking if the recd_stats_id is a group_id */

    for(array_index=0;array_index<ICF_SM_MAX_STATS_GROUP_COUNT;array_index++)
    {
        if(recd_stats_id==index_stats_group[array_index])
        {
            break;
        }
    }


    if(array_index<ICF_SM_MAX_STATS_GROUP_COUNT)
    {
        /* When the recd_stats_id is a group_id */

        p_sm_stats_index->curr_stats_group = index_stats_group[array_index];
        p_sm_stats_index->next_stats_group = index_stats_group[array_index+1];

    }
    else
    {
        /* When the recd_stats_id is an individual stats_id */
        ret_val=ICF_FAILURE;

    }

    ICF_PRINT(("\n[SM] Exiting Function icf_sm_get_stats_index \
                with ret_val = %d",ret_val));
        return ret_val;

#else
    icf_return_t ret_val  =  ICF_SUCCESS; 
    icf_stats_id_t   use_stats_id;

    /* To remove unused parameter warning.*/
    use_stats_id = recd_stats_id;
    p_sm_stats_index = p_sm_stats_index;

    return ret_val;

#endif

}/* Function icf_sm_get_stats_index() ends here. */



/**************************************************************
 * FUNCTION:    icf_sm_log_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *                  -icf_sm_set_dialed_call()
 *                  -icf_sm_set_missed_call()
 *                  -icf_sm_set_recd_call()
 *              to log the call details into the SM Global
 *              data.
 *
 * RETURNS:     Nothing. 
 *                   
 *************************************************************/
icf_void_t icf_sm_log_call_details(
        INOUT   icf_sm_log_call_details_st *p_call_details,
        INOUT   icf_glb_pdb_st  *p_glb_pdb,
        INOUT   icf_time_n_date_st *p_time_details,
        IN      icf_uint8_t  call_index)
{

    ICF_PRINT(("\n[SM] Entering Function icf_sm_log_call_details"));
    /*Copying values of remote_addr from the call_ctx*/

    ICF_SM_STRCPY(p_call_details[call_index].
            remote_party_addr.addr_val.str,
            p_glb_pdb->p_call_ctx->remote_party_addr.addr_val.str)

        p_call_details[call_index].remote_party_addr.addr_type =
        p_glb_pdb->p_call_ctx->remote_party_addr.addr_type;

    /*Copying the value of current time into time_stamp.*/

    p_call_details[call_index].time_stamp.seconds =
        p_time_details->seconds;

    p_call_details[call_index].time_stamp.minutes =
        p_time_details->minutes;

    p_call_details[call_index].time_stamp.hour =
        p_time_details->hour;

    p_call_details[call_index].time_stamp.date =
        p_time_details->date;

    p_call_details[call_index].time_stamp.month =
        p_time_details->month;

    p_call_details[call_index].time_stamp.year =
        p_time_details->year;


    /* Putting value into the call_id */

    p_call_details[call_index].call_id =
        p_glb_pdb->p_call_ctx->call_id;

    ICF_PRINT(("\n[SM] Exiting function icf_sm_log_call_details."));

}/* Function icf_sm_log_call_details() ends here*/


/**************************************************************
 * FUNCTION:    icf_sm_set_dialed_call
 *
 * DESCRIPTION: This is a utility function invoked by the CC
 *              whenever the CREATE_CALL is received from the PA.
 *
 * RETURNS:     ICF_SUCCESS to indicate that the dialed call
 *              has been  logged properly. ICF_FAILURE is 
 *              returned when:
 *                  -DBM returns failure.
 *                   
 *************************************************************/
icf_return_t icf_sm_set_dialed_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb)
{

    icf_error_t ecode =  ICF_NULL;

    icf_time_n_date_st sm_time_details;
    icf_sm_glb_data_st *p_sm_data =  ICF_NULL;

    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_SM),
                (icf_void_t *)&p_sm_data,
                &(ecode));

    if(ICF_FAILURE!= ret_val)
    {

            /*DBM returned the SM global data successfully */
            if (0 != p_sm_data->num_calls_to_log)
            {
                ICF_SM_NON_API_TRACE(p_glb_pdb,
                p_sm_data,ICF_SM_TRACE_DIALED_CALL_LOG_REQ)

                /* Call Logging Enabled: Processing Begins*/

                /* Use Macro to obtain the timestamp*/
                ICF_SM_TIME(sm_time_details,ret_val)

                    if (ICF_FAILURE == ret_val)
                    {
                        /* The SM_TIME macro evaluated FAILURE, 
                         * hence time_n_date_st will now 
                         * be given default values.*/

                        sm_time_details.seconds = 0;
                        sm_time_details.minutes = 0;
                        sm_time_details.hour = 0;
                        sm_time_details.date = 0;
                        sm_time_details.month = 0;
                        sm_time_details.year = 0;
                        ret_val = ICF_SUCCESS;

                    }


                /* Setting the value of index to do correct logging. */

                if (ICF_SM_INVALID_INDEX == p_sm_data->dialed_calls_index )
                {
                    /* This is the first call to be logged. */
                    p_sm_data->num_dialed_calls = 1;
                    p_sm_data->dialed_calls_index = 0;
                }

                else
                {
                    /* Not the first call to log : Processing Begins*/

                    if (p_sm_data->num_dialed_calls < 
                            p_sm_data->num_calls_to_log)
                    {
                        p_sm_data->dialed_calls_index += 1;
                        p_sm_data->num_dialed_calls+=1;
                    }/* if ends here */ 
                    else
                    {
                        /* num_calls_to_log has been reached. 
                         * p_sm_data->num_dialed_calls is now 
                         * num_calls_to_log. */

                        if ((p_sm_data->num_dialed_calls - 1)==
                                p_sm_data->dialed_calls_index )
                        {
                            p_sm_data->dialed_calls_index = 0;

                        }/*inner if ends here*/

                        else
                        {
                            p_sm_data->dialed_calls_index+=1;

                        }/*inner else ends here*/

                    }/* Else: num_calls_to_log reached: ends here*/

                }/* Not the first call to log : Processing Ends*/



                /*Logging dialed calls*/
                icf_sm_log_call_details(
                        p_sm_data->p_dialed_calls,
                        p_glb_pdb,
                        &(sm_time_details),
                        p_sm_data->dialed_calls_index);

              ICF_SM_NON_API_TRACE(p_glb_pdb,
                        p_sm_data,ICF_SM_TRACE_DIALED_CALL_LOG_RESP)


               
            }/* Call Logging enabled: Processing Ends.*/

            else
            {
                /* Call logging disabled*/
                /*---UT Printing------*/
                ICF_PRINT(("\n[SM]Call Logging is disabled."));

            }

     

    }/*DBM returned the SM global data successfully */

    else
    {
        /* DBM returned failure: Nothing to do here. */

    }


    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;

}/* Function icf_set_dialed_call() ends here.*/

/**************************************************************
 * FUNCTION:    icf_sm_set_missed_call
 *
 * DESCRIPTION: This is a utility function invoked by the CC
 *              whenever the USER_ALERTED is received from the PA. 
 *              Once the CONNECT_REMOTE/TERMINATE_CALL is received 
 *              the call details are moved from missed_calls to recd_calls.
 *              
 * RETURNS:     ICF_SUCCESS to indicate that the missed call
 *              has been  logged properly. ICF_FAILURE is 
 *              returned when:
 *                  -DBM returns failure.
 *                   
 *************************************************************/
icf_return_t icf_sm_set_missed_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb)
{

    icf_error_t ecode =  ICF_NULL;
    icf_time_n_date_st sm_time_details;
    icf_sm_glb_data_st *p_sm_data =  ICF_NULL;

    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)
        ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_SM),
                (icf_void_t *)&p_sm_data,
                &ecode);

    if(ICF_FAILURE != ret_val)
    {
        

        /* DBM successfully returned SM global data. */
        if (0 != p_sm_data->num_calls_to_log)
        {
            
            /* Call Logging Enabled: Processing Begins*/
            ICF_SM_NON_API_TRACE(p_glb_pdb,
                    p_sm_data,ICF_SM_TRACE_MISSED_CALL_LOG_REQ)


                /* Use Macro to obtain the timestamp*/
                ICF_SM_TIME(sm_time_details,ret_val)

                if (ICF_FAILURE == ret_val)
                {
                    /* The SM_TIME macro evaluated FAILURE, 
                     * hecne time_n_date_st will now be given default values.*/

                    sm_time_details.seconds = 0;
                    sm_time_details.minutes = 0;
                    sm_time_details.hour = 0;
                    sm_time_details.date = 0;
                    sm_time_details.month = 0;
                    sm_time_details.year = 0;
                    ret_val = ICF_SUCCESS;

                }


            /* Setting the value of index to do correct logging. */

            if (ICF_SM_INVALID_INDEX == p_sm_data->missed_calls_index )
            {
                /* This is the first call to be logged. */
                p_sm_data->num_missed_calls = 1;
                p_sm_data->missed_calls_index = 0;
            }

            else
            {
                /* Not the first call to log : Processing Begins*/

                if (p_sm_data->num_missed_calls < p_sm_data->num_calls_to_log)
                {
                    p_sm_data->missed_calls_index += 1;
                    p_sm_data->num_missed_calls+=1;
                }/* if ends here */ 
                else
                {
                    /* num_calls_to_log has been reached. 
                     * p_sm_data->num_missed_calls is now num_calls_to_log. */

                    if ((p_sm_data->num_missed_calls - 1)==
                            p_sm_data->missed_calls_index )
                    {
                        p_sm_data->missed_calls_index = 0;

                    }/*inner if ends here*/

                    else
                    {
                        p_sm_data->missed_calls_index+=1;

                    }/*inner else ends here*/

                }/* Else: num_calls_to_log reached: ends here*/

            }/* Not the first call to log : Processing Ends*/



            /*Logging missed calls*/
            icf_sm_log_call_details(
                    p_sm_data->p_missed_calls,
                    p_glb_pdb,
                    &(sm_time_details),
                    p_sm_data->missed_calls_index);

            
        ICF_SM_NON_API_TRACE(p_glb_pdb,
                    p_sm_data,ICF_SM_TRACE_MISSED_CALL_LOG_RESP)

           
        }/* Call Logging enabled: Processing Ends.*/

            
            else
            {
                /* Call logging disabled*/
                /*---UT Printing------*/
                ICF_PRINT(("\n[SM]Call Logging is disabled."));

            }



    }/* DBM successfully returned SM global data. */


    else
    {
        /* DBM returned failure. Nothing to do here.*/
    }


    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;

}/* Function icf_set_missed_call() ends here.*/

/**************************************************************
 * FUNCTION:    icf_sm_copy_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *                  -icf_sm_remv_missed_call()
 *              to copy the call details from one member onto 
 *              another.
 *
 * RETURNS:     Nothing. 
 *                   
 *************************************************************/
icf_void_t icf_sm_copy_call_details(
        INOUT   icf_sm_log_call_details_st *p_dest_call_details,
        INOUT   icf_sm_log_call_details_st *p_src_call_details)
{
    ICF_PRINT(("\n[SM] Entering function icf_sm_copy_call_details."));

    /*Copying values for the remote_party_addr*/
    ICF_SM_STRCPY(p_dest_call_details->remote_party_addr.addr_val.str,
            p_src_call_details->remote_party_addr.addr_val.str)


        p_dest_call_details->remote_party_addr.addr_type =
        p_src_call_details->remote_party_addr.addr_type;

    /* Copying the value of time_stamp. */
    p_dest_call_details->time_stamp.seconds =
        p_src_call_details->time_stamp.seconds;

    p_dest_call_details->time_stamp.minutes =
        p_src_call_details->time_stamp.minutes;

    p_dest_call_details->time_stamp.hour =
        p_src_call_details->time_stamp.hour;

    p_dest_call_details->time_stamp.date =
        p_src_call_details->time_stamp.date;

    p_dest_call_details->time_stamp.month =
        p_src_call_details->time_stamp.month;

    p_dest_call_details->time_stamp.year =
        p_src_call_details->time_stamp.year;


    /* Copying value into the call_id */
    p_dest_call_details->call_id =
        p_src_call_details->call_id;

    ICF_PRINT(("\n[SM] Exiting function icf_sm_copy_call_details"));

}/* Function icf_sm_copy_call_details() ends here.*/

/**************************************************************
 * FUNCTION:    icf_sm_remv_missed_call
 * 
 * DESCRIPTION: This utility function is invoked by 
 *              icf_sm_set_recd_call() when it has
 *              recorded the call details in the sm_recd_calls[]
 *              list and now that call has to be removed from
 *              the sm_missed_calls[] list. 
 *              
 * RETURNS:     SUCCESS to indicate that the call was removed from
 *              missed_calls[]. FAILURE indicating that the call
 *              was not in the missed_calls[] list and hence not
 *              removed.
 * 
 *************************************************************/

icf_return_t icf_sm_remv_missed_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        IN      icf_call_id_t remv_call_id,
        INOUT   icf_sm_glb_data_st *p_sm_data)
{
    icf_uint16_t count_calls = ICF_NULL;
    icf_uint8_t call_index = ICF_NULL;
    icf_uint8_t chk_call_index = ICF_NULL;

    icf_return_t ret_val = ICF_SUCCESS;

    icf_uint8_t remv_index = ICF_SM_INVALID_INDEX ;


    ICF_FUNCTION_ENTER(p_glb_pdb)

        count_calls = p_sm_data->num_missed_calls;
    call_index = p_sm_data->missed_calls_index;

    while (count_calls > 0)
    {
        /* To change the call_index if the 0th element has been seen. */
        if (1 == chk_call_index) 
        {
            call_index = p_sm_data->num_missed_calls-1;
            chk_call_index = 0;
        }

        /* To set indication when the 0th element is being seen. */
        if (0 == call_index)
        {
            chk_call_index = 1;
        }

        if (remv_call_id == p_sm_data->p_missed_calls[call_index].call_id)
        {
            remv_index = call_index;
            break;
        }

        count_calls--;
        call_index--;

    }/* While ends here */


    if (ICF_SM_INVALID_INDEX != remv_index)
    {

        /* recd_call_id found in the list: processing begins. */

        if (remv_index < p_sm_data->missed_calls_index)
        {
            /* When the remv_index is less than the top_index. History
             * is being maintained as a circular list, hence the
             * treatment differs for different positions of the node
             * to be removed. */

            for (call_index = remv_index; call_index < 
                    p_sm_data->missed_calls_index; call_index++)
            {
                /* copying the elements in reverse order. */
                icf_sm_copy_call_details(
                        &(p_sm_data->p_missed_calls[call_index]),
                        &(p_sm_data->p_missed_calls[call_index+1]));

            }/*for ends here*/


            p_sm_data->missed_calls_index = call_index-1;

        }/*if ends here*/

        else
        {

            if (remv_index == p_sm_data->missed_calls_index)
            {
                /* when the call at the main index is supposed to be removed */
                p_sm_data->missed_calls_index-=1;

            }/*inner if ends here*/

            else
            { 
                /* When the remv_index> main index of the list */
                call_index = remv_index -1;

                while (call_index != p_sm_data->missed_calls_index)
                {
                    /* copy the elements in reverse order until 
                     * the main index is reached */

                    icf_sm_copy_call_details(
                            &(p_sm_data->p_missed_calls[call_index+1]),
                            &(p_sm_data->p_missed_calls[call_index]));


                    call_index--;

                }/* while ends here */


            }/* inner else ends */

        }/* outer else ends here */


        p_sm_data->num_missed_calls--;

    }/* recd_call_id found in the list: processing ends. */


    else
    {

        /* Recd_call_id not found in the missed_call_list. */
        ret_val = ICF_FAILURE;
    }


    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;

}/* Function icf_sm_remv_missed_call() ends here. */

/**************************************************************
 * FUNCTION:    icf_sm_set_recd_call
 *
 * DESCRIPTION: This is a utility function invoked by the CC
 *              whenever CONNECT_REMOTE/TERMINATE_CALL is 
 *              received from the PA, to log the call details in
 *              the received_call history. 
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received call
 *              has been  logged properly. ICF_FAILURE is 
 *              returned when:
 *                  -DBM returns failure.
 *                   
 *************************************************************/
icf_return_t icf_sm_set_recd_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb)
{

    icf_error_t ecode = ICF_NULL;

    icf_call_id_t recd_call_id = ICF_NULL;

    icf_time_n_date_st sm_time_details;
    icf_sm_glb_data_st *p_sm_data = ICF_NULL;

    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_SM),
                (icf_void_t *)&p_sm_data,
                &(ecode));

    if(ICF_FAILURE != ret_val)
    {
        /* DBM successfully returned SM Global data: processing begins.*/ 
        
      
        if (0 != p_sm_data->num_calls_to_log)
        {
             ICF_SM_NON_API_TRACE(p_glb_pdb,
                    p_sm_data,ICF_SM_TRACE_RECD_CALL_LOG_REQ)


            /* Call Logging Enabled: Processing Begins */

                /* Use Macro to obtain the timestamp*/
                ICF_SM_TIME(sm_time_details,ret_val)

                if (ICF_FAILURE == ret_val)
                {
                    /* The SM_TIME macro evaluated FAILURE, 
                     * hence time_n_date_st will now be given default values.*/

                    sm_time_details.seconds = 0;
                    sm_time_details.minutes = 0;
                    sm_time_details.hour = 0;
                    sm_time_details.date = 0;
                    sm_time_details.month = 0;
                    sm_time_details.year = 0;
                    ret_val = ICF_SUCCESS;

                }

            recd_call_id = p_glb_pdb->p_call_ctx->call_id;

            /* Setting the value of index to do correct logging. */

            if (ICF_SM_INVALID_INDEX == p_sm_data->recd_calls_index )
            {
                /* This is the first call to be logged. */
                p_sm_data->num_recd_calls = 1;
                p_sm_data->recd_calls_index = 0;
            }

            else
            {
                /* Not the first call to log : Processing Begins*/

                if (p_sm_data->num_recd_calls < p_sm_data->num_calls_to_log)
                {
                    p_sm_data->recd_calls_index += 1;
                    p_sm_data->num_recd_calls+=1;
                }/* if ends here */ 
                else
                {
                    /* num_calls_to_log has been reached. 
                     * p_sm_data->num_recd_calls is now num_calls_to_log. */

                    if ((p_sm_data->num_recd_calls - 1)==
                            p_sm_data->recd_calls_index )
                    {
                        p_sm_data->recd_calls_index = 0;

                    }/*inner if ends here*/

                    else
                    {
                        p_sm_data->recd_calls_index+=1;

                    }/*inner else ends here*/

                }/* Else: num_calls_to_log reached: ends here*/

            }/* Not the first call to log : Processing Ends*/

            /*Logging recd calls*/
            icf_sm_log_call_details(
                    p_sm_data->p_recd_calls,
                    p_glb_pdb,
                    &(sm_time_details),
                    p_sm_data->recd_calls_index);


            /* Removing the call details from missed_call[] history. */
            ret_val = icf_sm_remv_missed_call(p_glb_pdb,
                             recd_call_id,p_sm_data);

            /* Because even if call not found in p_missed_calls[] 
             * list, it does not affect the recd_call addition. */
            ret_val = ICF_SUCCESS;

            ICF_SM_NON_API_TRACE(p_glb_pdb,
                    p_sm_data,ICF_SM_TRACE_RECD_CALL_LOG_RESP)


           
        }/* Call Logging enabled: Processing Ends.*/
                   
            else
            {
                /* Call logging disabled*/
                /*---UT Printing------*/
                ICF_PRINT(("\n[SM]Call Logging is disabled."));

            }





    }/* DBM successfully returned SM Global data: processing ends.*/

    else
    {
        /* DBM Failed to return SM Global Data. Nothing to do here. */

    }


    ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;


}/* Function icf_sm_set_recd_call() ends here. */

/**************************************************************
 * FUNCTION:    icf_sm_fill_def_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *              icf_sm_call_log_hdlr() to fill the NULL
 *              call history into the response API for
 *              dialed/missed and received calls, when DBM
 *              failed to give the config data.
 *              It is also called by fill_call_details() to
 *              fill the elements which are extra than the 
 *              num_calls_to_log value.
 *
 * RETURNS:     Nothing.
 *                   
 *************************************************************/
icf_void_t icf_sm_fill_def_call_details(
        INOUT   icf_call_info_st *p_call_info,
        IN      icf_uint8_t  recd_call_index)
{
    icf_string_st null_addr_val;
    icf_uint8_t extra_index = ICF_NULL;


    ICF_PRINT(("\n[SM] Entering Function icf_fill_def_call_details"));

    ICF_SM_STRCPY(null_addr_val.str,(icf_uint8_t *)("Null address"))

        /* Starting with recd_call_index, upto the last element, fill with 
         * default values. */
        for (extra_index = recd_call_index;
                extra_index < ICF_MAX_NUM_OF_CALLS_TO_LOG;
                extra_index++)
        {

            /* For loop to fill the extra entries of call history: BEGINS*/


            ICF_SM_STRCPY(p_call_info[extra_index].
                    party_addr.addr_val.str,
                    null_addr_val.str)

                p_call_info[extra_index].
                party_addr.addr_type = ICF_ADDRESS_PLAN_UNDEFINED;


            /*time_stamp field being copied.*/ 
            p_call_info[extra_index].
                time_stamp.seconds = 0;

            p_call_info[extra_index].
                time_stamp.minutes = 0;

            p_call_info[extra_index].
                time_stamp.hour = 0;

            p_call_info[extra_index].
                time_stamp.date = 0;

            p_call_info[extra_index].
                time_stamp.month = 0;

            p_call_info[extra_index].
                time_stamp.year = 0;



        }/* For loop to fill the extra entries of call history: ENDS*/

    ICF_PRINT(("\n[SM] Exiting function icf_fill_def_call_details"));


}/* Function icf_fill_def_call_details() ends here.*/

/**************************************************************
 * FUNCTION:    icf_sm_fill_call_details
 *
 * DESCRIPTION: This is a utility function invoked by the
 *              icf_sm_call_log_hdlr() to fill the 
 *              call history into the response API for
 *              dialed/missed and received calls.
 *
 * RETURNS:     Nothing.
 *                   
 *************************************************************/
icf_void_t icf_sm_fill_call_details(
        INOUT   icf_call_info_st             *p_call_info,
        INOUT   icf_sm_log_call_details_st   *p_logged_calls,
        IN      icf_uint8_t                  logged_num_of_calls,
        IN      icf_uint8_t                  recd_call_index,
        IN      icf_uint8_t                  num_calls_to_log)
{

    icf_uint8_t record_count = ICF_NULL;
    icf_uint8_t call_index = ICF_NULL;
    icf_uint8_t check_call_index  = ICF_NULL;
    icf_uint8_t record_index  = ICF_NULL;


    /* Call_log handler has given us the num_calls_to_log
     * We need to display call history according to the following 
     * logic: if logged_num_of_calls is lesser than num_calls_to_log
     * then this is the size of valid history. Else num_calls_to_log
     * is the size of valid history.*/

    if (logged_num_of_calls < num_calls_to_log)
    {
        record_count = logged_num_of_calls;
    }
    else
    {
        record_count = num_calls_to_log;
    }

    /* This function fills the default values in the entries that
     * exceed the valid_call_history count. */
    icf_sm_fill_def_call_details(p_call_info,record_count);

    /* Processing for the record_count number of entries in Call History. */

    call_index = recd_call_index;

    while (record_count > 0)
    {
        if (1 == check_call_index)
        {
            /* Incase the 0th index element has been handled
             * so the next element is read from the last position 
             * in the array. */

            call_index = logged_num_of_calls-1;
            check_call_index = 0;
        }

        if (0 == call_index)
        {
            /* To indicate that for next iteration, the call_index
             * should be taken to the last element. */

            check_call_index = 1;
        }

        ICF_SM_STRCPY(p_call_info[record_index].
                party_addr.addr_val.str,
                p_logged_calls[call_index].
                remote_party_addr.addr_val.str)


            p_call_info[record_index].
            party_addr.addr_type = p_logged_calls[call_index].
            remote_party_addr.addr_type;


        /* The timestamp field of call_info_st being copied. */ 

        p_call_info[record_index].
            time_stamp.seconds = 
            p_logged_calls[call_index].time_stamp.seconds;

        p_call_info[record_index].
            time_stamp.minutes = 
            p_logged_calls[call_index].time_stamp.minutes;

        p_call_info[record_index].
            time_stamp.hour = 
            p_logged_calls[call_index].time_stamp.hour;

        p_call_info[record_index].
            time_stamp.date = 
            p_logged_calls[call_index].time_stamp.date;

        p_call_info[record_index].
            time_stamp.month = 
            p_logged_calls[call_index].time_stamp.month;

        p_call_info[record_index].
            time_stamp.year = 
            p_logged_calls[call_index].time_stamp.year;


        record_count--;

        call_index--;
        record_index++;

    }/* while ends for recording all calls */

    ICF_PRINT(("\n[SM] Exiting function icf_sm_fill_call_details"));


}/* Function icf_sm_fill_call_details() ends here. */
#endif /* #ifdef ICF_STAT_ENABLED */



#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 *
 * FUNCTION:        icf_sm_trace
 *
 * DESCRIPTION:     This function checks the trace id and 
 *                  calls the macro for trace. It receives
 *                  the trace_id from API_Handlers of SM.
 *                     
 * RETURNS:         ICF_SUCCESS in case of success
 *                  and ICF_FAILURE in case of failure.
 *
 ******************************************************************/
icf_void_t icf_sm_trace(
        INOUT   icf_sm_pdb_st           *p_sm_pdb,
        IN      icf_trace_id_t           trace_id)
{
    icf_trace_data_st                    trace_data;

    switch(trace_id)
    {
        case ICF_SM_TRACE_TRACE_LVL_REQ_RECD:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] = 
                ((icf_sm_cc_set_trace_level_req_st *)
                 (p_sm_pdb->p_api_payload))->status;
            trace_data.int_val[1] = 
                ((icf_sm_cc_set_trace_level_req_st *)
                 (p_sm_pdb->p_api_payload))->trace_level;
            trace_data.int_val[2] = 
                ((icf_sm_cc_set_trace_level_req_st *)
                 (p_sm_pdb->p_api_payload))->trace_type;

            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;
            break;

        case ICF_SM_TRACE_ERROR_LVL_REQ_RECD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = 
                ((icf_sm_cc_set_error_report_level_req_st *)
                 (p_sm_pdb->p_api_payload))->status;
            /* Fix for SPR 17651 .Only status is to be printed */ 
            /*
            trace_data.int_val[1] = 
                ((icf_sm_cc_set_error_report_level_req_st *)
                 (p_sm_pdb->p_api_payload))->error_level;
            trace_data.int_val[2] = 
                ((icf_sm_cc_set_error_report_level_req_st *)
                 (p_sm_pdb->p_api_payload))->error_type;
             */
            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;
            break;

#if 0
        case ICF_SM_TRACE_GET_STATS_REQ_RECD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = 
                ((icf_sm_cc_get_stats_req_st *)
                 (p_sm_pdb->p_api_payload))->stats_id;
            trace_data.int_val[1] = 
                ((icf_sm_cc_get_stats_req_st *)
                 (p_sm_pdb->p_api_payload))->reset; 

            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;
            break;


        case ICF_SM_TRACE_GET_STATS_RESP_SENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] = 
                ((icf_cc_sm_get_stats_resp_st *)
                 (p_sm_pdb->p_api_payload))->stats_id;
            trace_data.int_val[1] = 
                ((icf_cc_sm_get_stats_resp_st *)
                 (p_sm_pdb->p_api_payload))->num_of_elements; 

            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;
            break;
#endif

        case ICF_SM_TRACE_CHK_MODULE_REQ_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;

            break;


        case ICF_SM_TRACE_CHK_MODULE_RESP_SENT:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;
            break;

        case ICF_SM_TRACE_CALL_LOG_REQ_RECD:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;

            break;


        case ICF_SM_TRACE_CALL_LOG_RESP_SENT:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;


            break;

        case ICF_SM_TRACE_VALIDATE_PARAM:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = p_sm_pdb->param_validity;
            trace_data.number_of_string_values = 
                ICF_NO_STRING_PRESENT;
            
        case ICF_TRACE_TIMER_EXPIRED:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = 
                ICF_ONE_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    (icf_uint8_t *)"Timer expired for License\
                            Management");
             break;



        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
    ICF_TRACE(p_sm_pdb->p_glb_pdb, trace_id, &trace_data)

}/* End function :icf_sm_trace() */

/**********************************************************************
 *
 * FUNCTION:        icf_sm_non_api_trace
 *
 * DESCRIPTION:     This function checks the trace id and 
 *                  calls the macro for trace.It receives the 
 *                  trace_id that are relevant to non_api
 *                  functions of SM.
 *                     
 * RETURNS:         ICF_SUCCESS in case of success
 *                  and ICF_FAILURE in case of failure.
 *
 ******************************************************************/
icf_void_t icf_sm_non_api_trace(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        INOUT   icf_sm_glb_data_st       *p_sm_data,
        IN      icf_trace_id_t           trace_id)
{
    icf_trace_data_st                    trace_data;

    switch(trace_id)
    {
        case ICF_SM_TRACE_NUM_CALLS_RECORD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] = p_sm_data->num_calls_to_log;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;


        case ICF_SM_TRACE_DIALED_CALL_LOG_REQ:
            if (0 != p_sm_data->num_dialed_calls)
            {
                /* Only if the dialed_calls_list is non empty. */
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = p_sm_data->p_dialed_calls[
                    p_sm_data->dialed_calls_index].call_id;
                trace_data.int_val[1] = p_sm_data->p_dialed_calls[
                    p_sm_data->dialed_calls_index].time_stamp.date;
                trace_data.int_val[2] = p_sm_data->p_dialed_calls[
                    p_sm_data->dialed_calls_index].time_stamp.hour;
                trace_data.int_val[3] = p_sm_data->p_dialed_calls[
                    p_sm_data->dialed_calls_index].time_stamp.minutes;

                trace_data.number_of_string_values = 
                    ICF_ONE_STRING_PRESENT;
                ICF_SM_STRCPY(trace_data.string_val[0].str,
                        p_sm_data->p_dialed_calls[
                        p_sm_data->dialed_calls_index].
                        remote_party_addr.addr_val.str)
            }
            else
            {
                trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
                trace_data.number_of_string_values = 
                    ICF_NO_STRING_PRESENT;


            }
            break;


        case ICF_SM_TRACE_DIALED_CALL_LOG_RESP:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = p_sm_data->p_dialed_calls[
                p_sm_data->dialed_calls_index].call_id;
            trace_data.int_val[1] = p_sm_data->p_dialed_calls[
                p_sm_data->dialed_calls_index].time_stamp.date;
            trace_data.int_val[2] = p_sm_data->p_dialed_calls[
                p_sm_data->dialed_calls_index].time_stamp.hour;
            trace_data.int_val[3] = p_sm_data->p_dialed_calls[
                p_sm_data->dialed_calls_index].time_stamp.minutes;

            trace_data.number_of_string_values = 
                ICF_ONE_STRING_PRESENT;
            ICF_SM_STRCPY(trace_data.string_val[0].str,
                    p_sm_data->p_dialed_calls[
                    p_sm_data->dialed_calls_index].
                    remote_party_addr.addr_val.str)


                break;


        case ICF_SM_TRACE_MISSED_CALL_LOG_REQ:
            if (0 != p_sm_data->num_missed_calls)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = p_sm_data->p_missed_calls[
                    p_sm_data->missed_calls_index].call_id;
                trace_data.int_val[1] = p_sm_data->p_missed_calls[
                    p_sm_data->missed_calls_index].time_stamp.date;
                trace_data.int_val[2] = p_sm_data->p_missed_calls[
                    p_sm_data->missed_calls_index].time_stamp.hour;
                trace_data.int_val[3] = p_sm_data->p_missed_calls[
                    p_sm_data->missed_calls_index].time_stamp.minutes;

                trace_data.number_of_string_values = 
                    ICF_ONE_STRING_PRESENT;
                ICF_SM_STRCPY(trace_data.string_val[0].str,
                        p_sm_data->p_missed_calls[
                        p_sm_data->missed_calls_index].
                        remote_party_addr.addr_val.str)
            }
            else
            {
                trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
                trace_data.number_of_string_values = 
                    ICF_NO_STRING_PRESENT;

            }

            break;


        case ICF_SM_TRACE_MISSED_CALL_LOG_RESP:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = p_sm_data->p_missed_calls[
                p_sm_data->missed_calls_index].call_id;
            trace_data.int_val[1] = p_sm_data->p_missed_calls[
                p_sm_data->missed_calls_index].time_stamp.date;
            trace_data.int_val[2] = p_sm_data->p_missed_calls[
                p_sm_data->missed_calls_index].time_stamp.hour;
            trace_data.int_val[3] = p_sm_data->p_missed_calls[
                p_sm_data->missed_calls_index].time_stamp.minutes;

            trace_data.number_of_string_values = 
                ICF_ONE_STRING_PRESENT;
            ICF_SM_STRCPY(trace_data.string_val[0].str,
                    p_sm_data->p_missed_calls[
                    p_sm_data->missed_calls_index].
                    remote_party_addr.addr_val.str)


                break;


        case ICF_SM_TRACE_RECD_CALL_LOG_REQ:
            if (0 != p_sm_data->num_recd_calls)
            {
                trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
                trace_data.int_val[0] = p_sm_data->p_recd_calls[
                    p_sm_data->recd_calls_index].call_id;
                trace_data.int_val[1] = p_sm_data->p_recd_calls[
                    p_sm_data->recd_calls_index].time_stamp.date;
                trace_data.int_val[2] = p_sm_data->p_recd_calls[
                    p_sm_data->recd_calls_index].time_stamp.hour;
                trace_data.int_val[3] = p_sm_data->p_recd_calls[
                    p_sm_data->recd_calls_index].time_stamp.minutes;

                trace_data.number_of_string_values = 
                    ICF_ONE_STRING_PRESENT;
                ICF_SM_STRCPY(trace_data.string_val[0].str,
                        p_sm_data->p_recd_calls[
                        p_sm_data->recd_calls_index].
                        remote_party_addr.addr_val.str)
            }

            else
            {
                trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
                trace_data.number_of_string_values = 
                    ICF_NO_STRING_PRESENT;

            }

            break;


        case ICF_SM_TRACE_RECD_CALL_LOG_RESP:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = p_sm_data->p_recd_calls[
                p_sm_data->recd_calls_index].call_id;
            trace_data.int_val[1] = p_sm_data->p_recd_calls[
                p_sm_data->recd_calls_index].time_stamp.date;
            trace_data.int_val[2] = p_sm_data->p_recd_calls[
                p_sm_data->recd_calls_index].time_stamp.hour;
            trace_data.int_val[3] = p_sm_data->p_recd_calls[
                p_sm_data->recd_calls_index].time_stamp.minutes;

            trace_data.number_of_string_values = 
                ICF_ONE_STRING_PRESENT;
            ICF_SM_STRCPY(trace_data.string_val[0].str,
                    p_sm_data->p_recd_calls[
                    p_sm_data->recd_calls_index].
                    remote_party_addr.addr_val.str)


                break;



        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
    ICF_TRACE(p_glb_pdb, trace_id, &trace_data)

}/* End function: icf_sm_non_api_trace() */
#endif /* End of #ifdef ICF_TRACE_ENABLE */

#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_sm_error
 *
 * DESCRIPTION:     This function checks the error id and calls the macro for
 *                  error reporting.
 *
 * RETURNS:         Nothing.
 *
 ******************************************************************************/
icf_void_t icf_sm_error(
        INOUT   icf_sm_pdb_st           *p_sm_pdb,
        IN      icf_error_id_t              ecode)

{
    icf_error_data_st                    error_data;

    switch(ecode)
    {
        case ICF_ERROR_INVALID_API_ID:
            error_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            error_data.int_val[0] =
                (icf_int32_t)(p_sm_pdb->p_internal_msg->msg_hdr.msg_id);
            error_data.int_val[1] =
                (icf_int32_t)(p_sm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id);
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_ERROR_INVALID_PARAM:
            error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;

            error_data.int_val[0] =
                (icf_int32_t)(p_sm_pdb->p_glb_pdb->p_recv_msg->hdr.api_id);
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        default:
            error_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

    } /* end of switch-case for ecode */

    /* Finally, invoke the common macro for error handling */
    ICF_SET_ECODE(p_sm_pdb->p_glb_pdb, 
            &error_data, ecode, p_sm_pdb->p_ecode)

}/* End function : icf_sm_error()*/
#endif /* End of #ifdef ICF_ERROR_ENABLE */




/*------------------------------------------------------------------------
 *          Fucntions to check parameter validity.
 *---------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
/**********************************************************************
 *
 * FUNCTION:        icf_sm_validate_trace_details
 *
 * DESCRIPTION:     This function checks the trace details 
 *                  received in the API: SET_TRACE_LEVEL_REQ.
 *                     
 * RETURNS:         ICF_SUCCESS in case of trace details are valid,
 *                  and ICF_FAILURE in case invalid values have been given.
 *
 ******************************************************************/
icf_void_t icf_sm_validate_trace_details(
        INOUT   icf_sm_pdb_st                *p_sm_pdb,
        INOUT   icf_sm_cc_set_trace_level_req_st  *p_trace_details)
{
#ifdef ICF_TRACE_ENABLE
    ICF_PRINT(((icf_uint8_t *)"\n[SM]Entering function icf_sm_validate_trace_details"));


    if(ICF_FALSE<p_trace_details->status)
    {
        p_trace_details->status = ICF_TRUE;

    }
    if((ICF_SM_TRACE_LEVEL_END > p_trace_details->trace_level) &&
            (ICF_SM_TRACE_LEVEL_START < p_trace_details->trace_level))
    {
        p_sm_pdb->param_validity = ICF_TRUE;
    }
    else
    {
        p_sm_pdb->param_validity = ICF_FALSE;

    }
    
    ICF_PRINT(((icf_uint8_t *)"\n[SM]Exiting function icf_sm_validate_trace_details"));
#else
    p_sm_pdb = p_sm_pdb;
    p_trace_details = p_trace_details;
#endif


}/* Function icf_sm_validate_trace_details ends here.*/
#endif


#ifdef ICF_ERROR_ENABLE
/**********************************************************************
 *
 * FUNCTION:        icf_sm_validate_error_details
 *
 * DESCRIPTION:     This function checks the error details 
 *                  received in the API: SET_ERROR_LEVEL_REQ.
 *                     
 * RETURNS:         ICF_SUCCESS in case of error details are valid,
 *                  and ICF_FAILURE in case invalid values have been given.
 *
 ******************************************************************/
icf_void_t icf_sm_validate_error_details(
        INOUT   icf_sm_pdb_st                       *p_sm_pdb,
        INOUT   icf_sm_cc_set_error_report_level_req_st
        *p_error_details)
{

#ifdef ICF_ERROR_ENABLE
    ICF_PRINT(((icf_uint8_t *)"\n[SM]Entering function icf_sm_validate_error_details"));


    if (ICF_FALSE < p_error_details->status)
    {
        p_error_details->status = ICF_TRUE;

    }

    if((ICF_SM_ERROR_LEVEL_END > p_error_details->error_level) &&
            (ICF_SM_ERROR_LEVEL_START < p_error_details->error_level))
    {
        p_sm_pdb->param_validity = ICF_TRUE;
    }
    else
    {
        p_sm_pdb->param_validity = ICF_FALSE;
    }

    
    ICF_PRINT(((icf_uint8_t *)"\n[SM]Exiting function icf_sm_validate_error_details"));

#else
    p_sm_pdb = p_sm_pdb;
    p_error_details = p_error_details;
#endif
}/* Function icf_sm_validate_error_details ends here.*/
#endif

#ifdef ICF_STAT_ENABLED
/**********************************************************************
 *
 * FUNCTION:        icf_sm_validate_get_stats
 *
 * DESCRIPTION:     This function checks the stats details 
 *                  received in the API: GET_STATS_REQ. 
 *                  Only the reset flag is checked here, because the
 *                  INVALID stats_id is to be reported in the response 
 *                  to this API.
 *                     
 * RETURNS:         ICF_SUCCESS in case of parameter values are valid,
 *                  and ICF_FAILURE in case invalid values have been given.
 *
 ******************************************************************/
icf_void_t icf_sm_validate_get_stats(
        INOUT   icf_sm_pdb_st          *p_sm_pdb,
        INOUT   icf_get_icf_stats_req_st  *p_stats_details)
{
#ifdef ICF_STAT_ENABLED

    ICF_PRINT(("\n[SM]Entering function icf_sm_validate_get_stats"));


    if(ICF_FALSE<p_stats_details->reset)
    {
        p_stats_details->reset = ICF_TRUE;

    }

    if(ICF_STAT_UNDEFINED < p_stats_details->stats_id &&
            ICF_SM_STAT_MAX_ID_VAL > p_stats_details->stats_id)
    {
        p_sm_pdb->param_validity = ICF_TRUE;
    }

    else
    {
        p_sm_pdb->param_validity = ICF_FALSE;
    }

    
    ICF_PRINT(("\n[SM] Exiting function icf_sm_validate_get_stats"));

#else
    p_sm_pdb = p_sm_pdb;
    p_stats_details = p_stats_details;
#endif

}/* Function icf_sm_validate_get_stats ends here.*/
#endif


/********************************************************
 *      Functions for License Management 
 ********************************************************/








#ifdef ICF_REL_OLD
/*----------------------------------------------------------------
 * FUNCTION NAME:  icf_sm_validate_expiry_time
 * 
 * DESCRIPTION:    Validates the host_id of the license_data
 *                
 * RETURNS :        SUCCESS:Incase of no expiry time or
 *                  when the current time is less than expiry time.
 *                  FAILURE: Incase the current time could not
 *                  be obtained or the expiry time is less than
 *                  or equal to current time.
 *
 * CALLED BY:       icf_sm_init_license()
 *-----------------------------------------------------------------------*/
icf_return_t icf_sm_validate_expiry_time(
        INOUT     icf_sm_license_data_st       *p_license_data)

{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint32_t current_timestamp = ICF_NULL;
    icf_uint32_t temp_time = ICF_NULL;
    
    if (!(ICF_SM_EXPIRY_TIME & p_license_data->bit_mask))
    {
        ret_val = ICF_SUCCESS;
    }
    else
    {
       /* expiry time bit set : processing begins */
        ret_val = icf_sm_validate_n_get_time(
                    &temp_time,
                    &current_timestamp);
            
        if (ICF_FAILURE != ret_val)
        {
            /* current time obtained successfully: processing begins */
            if (current_timestamp >= p_license_data->
                                    expiry_time)
            {
                ret_val = ICF_FAILURE;
            }
            else
            {
                ret_val = ICF_SUCCESS;
            }
        }/* current time obtained successfully: processing ends */
    }/* expiry time bit set : processing ends */
    
    return ret_val;
    
}/* Function icf_sm_validate_expiry_time() Ends here */
#endif


#ifdef ICF_REL_OLD
/*----------------------------------------------------------------
 * FUNCTION NAME:  icf_sm_validate_hostid
 * 
 * DESCRIPTION:    Validates the host_id of the license_data
 *                
 * RETURNS :        SUCCESS: Incase of valid host id or no host Id
 *                  FAILURE: Incase of mismatched host id.
 *
 * CALLED BY:       icf_sm_init_license()
 *-----------------------------------------------------------------------*/
icf_return_t icf_sm_validate_hostid(
        INOUT     icf_sm_license_data_st       *p_license_data)

{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint32_t temp_hostid = ICF_NULL;

    
    if (!(ICF_SM_HOST_ID & p_license_data->bit_mask))
    {
        ret_val = ICF_SUCCESS;
    }
    else
    {
        temp_hostid = (icf_uint32_t)ICF_PORT_GETHOSTID();
        if (temp_hostid != p_license_data->host_id)
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = ICF_SUCCESS;
        }
    }
    return ret_val;
    
}/* Function icf_sm_validate_hostid() Ends here */
#endif

#ifdef ICF_REL_OLD
/*----------------------------------------------------------------
 * FUNCTION NAME:  icf_sm_validate_prev_ref_id
 * 
 * DESCRIPTION:    Validates the prev_ref_id of the license_data
 *                
 * RETURNS :        SUCCESS: Incase of no prev_ref_id or
 *                  when the stored ref_id in counter data
 *                  matches the new
 *                  FAILURE: Incase of mismatched host id.
 *
 * CALLED BY:       icf_sm_init_license()
 *-----------------------------------------------------------------------*/
icf_return_t icf_sm_validate_prev_ref_id(
        IN     icf_sm_license_data_st       *p_license_data,
        IN     icf_sm_counter_data_st       *p_counter_data)

{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Default value of reference ID stored in the counter_file
     * is looked for and if it is not present then the 
     * license data should contain the prev_ref_id -- SUMIT*/
    if (ICF_NULL != icf_port_strcmp(p_counter_data->reference_id,
                (icf_uint8_t *)"\0"))
    {
        /* This would ensure if the system was shut down and restarted with same 
         * licence file and counter file then it should check for the existing 
         * ref_id in the licence file to be as the on in counter file and if so
         * should return success -- SUMIT*/
        if((ICF_NULL == icf_port_strcmp(
                                           p_license_data->reference_id, 
                                           p_counter_data->reference_id)))
        {
          ret_val = ICF_SUCCESS;
        }
        /* Else it should check if there exist a previous reference id in file.*/ 
        else if (!(ICF_SM_PREV_REFERENCE_ID & p_license_data->bit_mask))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /*Check if the prevoius reference id matches with the ref id in 
             *counter file if it does return success. This is the case where in 
             *licence file is updated*/
            if (ICF_NULL == icf_port_strcmp( 
                    p_license_data->previous_reference_id,
                    p_counter_data->reference_id))
            {
                ret_val = ICF_SUCCESS;
            }
            else
            {
                ret_val = ICF_FAILURE;
            }
        }
    }/* reference_id stored in the counter data is not the default one */
    else
    {
        ret_val = ICF_SUCCESS;
    }
    return ret_val;
    
}/* Function icf_sm_validate_prev_ref_id() Ends here */
#endif


#ifdef ICF_REL_OLD
/*----------------------------------------------------------------
 * FUNCTION NAME:  icf_sm_get_file_modify_time
 * 
 * DESCRIPTION:    Obtains the last modified time
 *                 of the file specified as the parameter.
 *                
 * RETURNS :        SUCCESS: Incase value could be obtained
 *                  FAILURE: Incase stat() call failed.
 *
 * CALLED BY:       icf_sm_init_license()
 *-----------------------------------------------------------------------*/
icf_return_t icf_sm_get_file_modify_time(
        IN      icf_uint8_t      *p_license_file_name,
        OUT     icf_uint32_t     *p_modified_time)

{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_stat_t   file_stat;
    
    
    if (ICF_NULL == ICF_PORT_STAT(p_license_file_name,
                        &file_stat))
    {
        ret_val = ICF_SUCCESS;
        
        /*    From  *p_modified_time = (icf_uint32_t)(file_stat.st_mtime) (RN);*/

        *p_modified_time = (icf_uint32_t)mktime(gmtime(&file_stat.st_mtime));
        
    }
    else
    {
        ret_val = ICF_FAILURE;
    }
    return ret_val;
    
}/* Function icf_sm_get_file_modify_time() Ends here */
#endif



#ifdef ICF_REL_OLD
/*******************************************************************************
 *
 * FUNCTION:        icf_sm_start_timer
 *
 * DESCRIPTION:     This function starts the 24-Hour timer.
 *                  This function is invoked only if the expiry_time is
 *                  not null.
 *
 ******************************************************************************/
icf_return_t icf_sm_start_timer(
        INOUT icf_glb_pdb_st     *p_glb_pdb,
        INOUT icf_sm_glb_data_st *p_sm_data,
        INOUT icf_error_t        *p_ecode,
        IN    icf_boolean_t       init_flag)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;
    icf_sm_counter_data_st           counter_data;
    icf_uint32_t                     temp_timestamp = ICF_NULL;
    icf_uint32_t                     current_timestamp = ICF_NULL;
    

    ICF_FUNCTION_ENTER(p_glb_pdb)  /* function entry trace */


    /* Initializing members of counter_data and the 
     * detection flag of p_sm_data */
    icf_port_strcpy((icf_uint8_t *)(counter_data.reference_id),
                                  (icf_uint8_t *)"\0"); 
    
    counter_data.last_known_time = ICF_NULL;
    counter_data.last_modified_time = ICF_NULL;

    p_sm_data->detection_mask = ICF_NULL;
    
    /* Read the counter file */
    if (ICF_FAILURE == icf_sm_read_counter_file(p_sm_data, 
                    &counter_data))
    {
        p_sm_data->detection_mask |= ICF_SM_FILE_FAILURE;
        ret_val = ICF_FAILURE;
    }

    /* Trying to obtain the current_timestamp */
    else if (ICF_FAILURE == icf_sm_validate_n_get_time(
                            &temp_timestamp,
                            &current_timestamp))
    {
         p_sm_data->detection_mask |=  ICF_SM_TIME_CALL_FAILED;  
         ret_val = ICF_FAILURE;

    }
    else
    {
        /* current_timestamp successfully found 
         * Processing Begins: time_remaining and counter_data
         * given new values. Also counter data to be written */
        if (ICF_TRUE == init_flag)
        {
            /* timer is to be started for the first time:
             * after the license has been found to be valid  */
            p_sm_data->time_remaining = p_sm_data->expiry_time -
                                           current_timestamp;
            counter_data.last_known_time = current_timestamp;
        }                
        else
        {
            /* timer to be started at the 24 hour interval */
            p_sm_data->time_remaining -= (ICF_SM_HOUR_INTERVAL * 
                                          ICF_SM_SECONDS_PER_HOUR);
            counter_data.last_known_time = current_timestamp;
        }
        /* Write the counter data on the file*/
        if (ICF_FAILURE == icf_sm_write_counter_data(
                                             p_sm_data->p_counter_file,
                                            &counter_data))
        {
            p_sm_data->detection_mask |= ICF_SM_FILE_FAILURE;
            ret_val = ICF_FAILURE;
        }
    }/* current_timestamp successfully found 
        time_remaining and counter_data
        given new values. Also counter data to be written 
        Processing Ends */

    if (ICF_FAILURE != ret_val)
    {
        /* Timer Handling: Processing begins */
         
         /* Obtain the timer_interval depending on 
          * the value of time_remaining */
        if (p_sm_data->time_remaining > (ICF_SM_HOUR_INTERVAL * 
                                       ICF_SM_SECONDS_PER_HOUR))
        {
            p_sm_data->timer_interval = (ICF_SM_HOUR_INTERVAL * 
                                       ICF_SM_SECONDS_PER_HOUR);
        }
        else
        {
            p_sm_data->timer_interval = p_sm_data->time_remaining;
        }
            
        /* Get a timer block from DBM, this will be freed later */
        ret_val = icf_dbm_create_timer_block(p_glb_pdb,(icf_void_t **)
                                &p_timer_data, p_ecode);
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_timer_data))
        if (ICF_SUCCESS == ret_val)
        {
            /* timer_data obtained: processing begins */
            /* fill the timer data structure */
            p_timer_data->timer_type = ICF_TIMER_TYPE_LICENSE_CHK;
            p_timer_data->module_id = ICF_INT_MODULE_SM;
            p_timer_data->p_timer_buffer = ICF_NULL;
            p_timer_data->p_glb_pdb = p_glb_pdb;
            /* Now, start the  timer */
            ICF_START_TIMER(p_glb_pdb, p_timer_data, timer_id,
                    p_sm_data->timer_interval, p_ecode, ret_val)
            if (ICF_SUCCESS == ret_val)
            {
                /* save the timer id returned by macro */
                p_sm_data->timer_id  = timer_id;
                ret_val = ICF_SUCCESS;
                /* Timer started successfully */
            }
            else
            {
                /* Timer could not be started */
                 p_sm_data->timer_id  = ICF_NULL;
                 p_sm_data->detection_mask |= ICF_SM_TIMER_FAILED;
                 ret_val = ICF_FAILURE;
            }
            icf_dbm_free_timer_block(p_glb_pdb,
                               p_timer_data, p_ecode); 
        
        }/* timer_data obtained: processing ends */
        else
        {
            /* timer_data could not be obtained */
            p_sm_data->detection_mask |= ICF_SM_TIMER_FAILED;
            ret_val = ICF_FAILURE;
        }
        
    } /* Timer Handling: Processing Ends */


    else
    {
        /* Some failure occurred:
         * while reading the counter file or
         * by time() call or
         * while writing the counter file */
        ret_val = ICF_FAILURE;
            
    }
        
    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_sm_start_timer() */
#endif


