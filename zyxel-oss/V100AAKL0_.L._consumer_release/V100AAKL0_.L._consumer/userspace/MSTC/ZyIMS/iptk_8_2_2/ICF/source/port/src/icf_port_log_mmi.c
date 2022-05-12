/*************************************************************************

 *

 *     FILENAME           :  icf_port_log_mmi.c

 *

 *     DESCRIPTION        :  This file contains error/trace printing 

 *                           routines for CC

 *

 *     Revision History   :

 *         DATE        NAME                REFERENCE            REASON

 *         ----------------------------------------------------------

 *         07/01/05    Aashish Suchdev                        Initial

 *         28/03/05    Shefali Aggarwal                       Add Traces

 *         07/04/08  Sumant Gupta    ICF 8.0         Rel7.1 Windows porting
 *       04-Mar-2009 Saurabh Gupta         SPR 19349      Klocworks fix
 *     Copyright 2008, Aricent.

 *

 *************************************************************************/

#ifdef ICF_PORT_SOLARIS

#include <stdio.h>

#endif



#include "icf_common_types.h"

#include "icf_mmi_defs.h"

#include "icf_port_prototypes.h"

#include "icf_port_defs.h"





#ifdef ICF_ERROR_ENABLE

#ifndef ICF_SM_LOG_ENABLE

/*************************************************************************

*

*  FUNCTION NAME  :   icf_port_log_mmi_error

*

*  DESCRIPTION    :  This function is used to print the error data from CC  

*

*  RETURNS        :     

*

 *************************************************************************/



icf_void_t icf_port_log_mmi_error(

        IN icf_error_id_t    error_id,

        IN icf_error_data_st *p_error_data)

{

#ifdef ICF_DETAILED_DEBUG_TRACE	

    icf_uint8_t  count = 0, max_count = 0;    

#endif

    switch (error_id){

        default:

            {

#ifdef ICF_DETAILED_DEBUG_TRACE

                max_count = p_error_data->num_of_integer_values;

                ICF_PRINT(((icf_uint8_t*)"\tDATA:integer(s):"));

                for ( count = 0; (count < max_count) && (count < ICF_MAX_INT_ERROR_DATA) ; count ++)

                {

                    ICF_PRINT(((icf_uint8_t*)" %lu",p_error_data->int_val[count]));

                }



                max_count = p_error_data->number_of_string_values;

                ICF_PRINT(((icf_uint8_t*)"\nDATA:string(s):"));

                for ( count = 0; (count < max_count) && (count < ICF_MAX_STR_ERROR_DATA) ; count ++)
                {
                    ICF_PRINT(((icf_uint8_t*)"\t %s",p_error_data->string_val[count].str));
                }
                ICF_PRINT(((icf_uint8_t*)"\n"));

#else

		if(ICF_NULL == p_error_data)
                {
                }

#endif

	    }

            break;

    }

}

#endif /*ICF_ERROR_ENABLE*/

#endif



#ifdef ICF_TRACE_ENABLE

/*******************************************************************************

 *

 * FUNCTION:        icf_port_log_mmi_event_to_str

 *

 * DESCRIPTION:     This function returns a string corresponding to an event

 *                  and is used only for traces.

 *

 ******************************************************************************/

icf_void_t icf_port_log_mmi_event_to_str(

        IN icf_mmi_event_t event_val,

        OUT icf_uint8_t *p_event_str)

{

    switch(event_val)

    {

        case ICF_MMI_CMS_REQ: 

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_CMS_REQ");

            break;



        case  ICF_MMI_MMS_REQ:    

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_MMS_REQ");

            break;



        case ICF_MMI_DMS_REQ:

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_DMS_REQ");

            break;



        case ICF_MMI_MERGE_REQ:

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_MERGE_REQ");

            break;



        case ICF_MMI_GCL_RESP: 

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_GCL_RESP");

            break;



        case ICF_MMI_COMMIT_RESP:

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_COMMIT_RESP");

            break;


/*
        case ICF_MMI_REL_RES_RESP:

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_REL_RES_RESP");

            break;

*/

        case ICF_MMI_CMS_RESP :

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_CMS_RESP");

            break;



        case ICF_MMI_MMS_RESP :

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_MMS_RESP");

            break;



        case ICF_MMI_DMS_RESP :

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_DMS_RESP");

            break;



        case ICF_MMI_DMS_IND:

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_DMS_IND");

            break;



        case ICF_MMI_MERGE_RESP:

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_MERGE_RESP");

            break;



        case ICF_MMI_CALL_CLEAR :

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_CALL_CLEAR");

            break;



        default:

            icf_port_strcpy(p_event_str,

                    (icf_uint8_t *)"ICF_MMI_INVALID_EVENT");

            break;



    } /* end of switch case for event_val */

} /* end of icf_port_log_mmi_event_to_str() */

#endif /* end of #ifdef ICF_TRACE_ENABLE */









#ifdef ICF_TRACE_ENABLE

/*******************************************************************************

 *

 * FUNCTION:        icf_port_log_mmi_state_to_str

 *

 * DESCRIPTION:     This function returns a string corresponding to a state and

 *                  is used only for traces.

 *

 ******************************************************************************/

icf_void_t icf_port_log_mmi_state_to_str(

        IN icf_mmi_state_t state_val,

        OUT icf_uint8_t *p_state_str)

{

    switch(state_val)

    {

        case ICF_MMI_00_IDLE: 

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_00_IDLE");

            break;


/*
        case ICF_MMI_01_AWT_RM_GCL_RESP:

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_01_AWT_RM_GCL_RESP");

            break;



        case ICF_MMI_02_AWT_MM_CMS_RESP: 

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_02_AWT_MM_CMS_RESP");

            break;



        case ICF_MMI_03_AWT_RM_COMMIT_RESP: 

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_03_AWT_RM_COMMIT_RESP");

            break;



        case ICF_MMI_04_AWT_MM_MMS_RESP: 

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_04_AWT_MM_MMS_RESP");

            break;



        case ICF_MMI_05_AWT_MM_DMS_RESP:

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_05_AWT_MM_DMS_RESP");

            break;



        case ICF_MMI_06_AWT_RM_REL_RES_RESP:

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_06_AWT_REL_RES_RESP");

            break;



        case ICF_MMI_07_AWT_MM_MERGE_RESP:

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_07_AWT_MM_MERGE_RESP");

            break;

*/



        default:

            icf_port_strcpy(p_state_str,

                    (icf_uint8_t *)"ICF_MMI_STATE_INVALID");

            break;



    } /* end of switch case for state_val */

} /* end of icf_port_log_mmi_state_to_str() */

#endif /* end of #ifdef ICF_TRACE_ENABLE */



#ifdef ICF_TRACE_ENABLE

/*******************************************************************************

 *

 * FUNCTION:        icf_port_log_mmi_api_to_str

 *

 * DESCRIPTION:     This function returns a string corresponding to an API ID

 *                  and is used only for traces.

 *

 ******************************************************************************/

icf_void_t icf_port_log_mmi_api_to_str(

        IN icf_api_id_t api_val,

        OUT icf_uint8_t *p_api_str)

{

 

 switch(api_val)   

 {

     case ICF_GET_CODEC_LIST_REQ:

         icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"GET_CODEC_LIST_REQ");

         break;



     case ICF_CREATE_MEDIA_SESSION_REQ:

         icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"ICF_CC_MM_CREATE_MEDIA_SESSION_REQ");

         break;



     case ICF_CODEC_COMMIT_REQ:

         icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"ICF_CC_RM_CODEC_COMMIT_REQ");

         break;



     case ICF_MODIFY_MEDIA_SESSION_REQ:

         icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"ICF_CC_MM_MODIFY_MEDIA_SESSION_REQ");

         break;



     case ICF_DELETE_MEDIA_SESSION_REQ:

         icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"ICF_CC_MM_DELETE_MEDIA_SESSION_REQ");

         break;


/*
     case ICF_REL_RES_REQ:

         icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"ICF_CC_RM_REL_RES_REQ");

         break;

*/

     case ICF_MERGE_MEDIA_SESSION_REQ:

         icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"ICF_CC_MM_MERGE_MEDIA_SESSION_REQ");

         break;



    default:

            icf_port_strcpy(p_api_str,

                    (icf_uint8_t *)"ICF_API_ID_INVALID");

            break;





     }/*switch(api_val) ends here*/

}/*Function icf_port_log_mmi_api_to_str ends here*/

#endif



#ifdef ICF_TRACE_ENABLE

#ifndef ICF_SM_LOG_ENABLE

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_log_mmi_trace
*
*  DESCRIPTION    :  This function is used to print the trace data from MMI
*
*  RETURNS        :     
**************************************************************************/

icf_void_t icf_port_log_mmi_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data)
{
    switch (trace_id)
    {
/*######################################################################*/
        case ICF_EVENT_RECVD:
            ICF_PRINT(((icf_uint8_t*)"\n Event_id = %d", p_trace_data->int_val[0]));
            break;
    
        case ICF_MMI_TRACE_CURRENT_STATE:
            ICF_PRINT(((icf_uint8_t*)"\n State = %d", p_trace_data->int_val[0]));
            break;
          
        case ICF_TRACE_API_SEND:
            ICF_PRINT(((icf_uint8_t*)"\n Api_id = %d", p_trace_data->int_val[0]));
            break;

        case ICF_TRACE_HDR_SEND:
            ICF_PRINT(((icf_uint8_t*)"\n api_id = %d",p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)"\n source_id = %d",p_trace_data->int_val[1]));
            ICF_PRINT(((icf_uint8_t*)"\n destination_id = %d",p_trace_data->int_val[2]));
            ICF_PRINT(((icf_uint8_t*)"\n call_id = %d",p_trace_data->int_val[3]));
            break;

        case ICF_TRACE_CMS_DATA_SEND:
        case ICF_TRACE_GCL_DATA_SEND:
            ICF_PRINT(((icf_uint8_t*)"\n line_id = %d", p_trace_data->int_val[0]));
            break;
            
        case ICF_TRACE_MMS_DATA_SEND:
            break;

        case ICF_TRACE_COMMIT_DATA_SEND:            
        case ICF_TRACE_MMS_SDP_SEND:
        case ICF_TRACE_CMS_SDP_SEND:
        case ICF_TRACE_CMS_SDP_RECVD:
            break;

        case ICF_TRACE_MERGE_DATA_SEND:
            ICF_PRINT(((icf_uint8_t*)"\n merge_call_id = %d",p_trace_data->int_val[0]));
            break;

/*######################################################################*/

		case ICF_TRACE_STREAM_ID:
            ICF_PRINT(((icf_uint8_t*)"\n Stream ID = %d", p_trace_data->int_val[0]));
            break;
            
		case ICF_TRACE_STREAM_TYPE:
            ICF_PRINT(((icf_uint8_t*)"\n Stream TYPE = %s", p_trace_data->string_val[0].str));
            break;

        case ICF_TRACE_NUM_OF_LOCAL_CODECS:
            ICF_PRINT(((icf_uint8_t*)"\n number = %d",p_trace_data->int_val[0]));
            break;  

        case ICF_TRACE_NUM_OF_CODECS:
            ICF_PRINT(((icf_uint8_t*)"\n codec_count = %d", p_trace_data->int_val[0]));
            break;
           
        case ICF_TRACE_CODEC:
            if(ICF_ONE_INT_PRESENT == p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t*)"\n codec_num = %d", p_trace_data->int_val[0]));
            }
            if(ICF_ONE_STRING_PRESENT == 
                    p_trace_data->number_of_string_values)
            {
				ICF_PRINT(((icf_uint8_t*)"\n codec_name = %s", p_trace_data->string_val[0].str));
			}
            break;

        case ICF_TRACE_ADDR:
            if(ICF_ONE_STRING_PRESENT == 
                    p_trace_data->number_of_string_values)
            {
                ICF_PRINT(((icf_uint8_t*)"\n address = %s",p_trace_data->string_val[0].str));
            }
            
            else if(ICF_FOUR_INT_PRESENT == 
                    p_trace_data->num_of_integer_values)
            {
                ICF_PRINT(((icf_uint8_t*)"\n IP = %d.%d.%d.%d",
                            p_trace_data->int_val[0],
                            p_trace_data->int_val[1],
                            p_trace_data->int_val[2],
                            p_trace_data->int_val[3]));
            }
            break;
/*######################################################################*/        

		case ICF_TRACE_MMS_DATA_RECVD:
        case ICF_TRACE_GCL_DATA_RECVD:
        case ICF_TRACE_CMS_DATA_RECVD:
            ICF_PRINT(((icf_uint8_t*)"\n result = %d",p_trace_data->int_val[0]));
            ICF_PRINT(((icf_uint8_t*)"   presence_flag = %d",p_trace_data->int_val[1]));
            break;
               
        default:
            ICF_PRINT(((icf_uint8_t*)"\n"));
            break;
    }

}/*Function icf_port_log_mmi_trace() ends here*/

#endif/*ICF_TRACE_ENABLE*/

#endif

