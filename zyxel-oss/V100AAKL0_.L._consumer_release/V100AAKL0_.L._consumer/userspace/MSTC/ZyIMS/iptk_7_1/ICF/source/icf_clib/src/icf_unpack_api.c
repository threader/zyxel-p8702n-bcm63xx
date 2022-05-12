/*******************************************************************************
*                                        
* FILE NAME    : icf_unpack_api.c
*                                        
* DESCRIPTION  :The file contains the routines for the buffer Unpacking code.
*               Fraction of this code is automatically generated.
*                                        
* Revision History :                
*                
*    DATE         NAME           REFERENCE     REASON
* --------      ------------     ---------     --------
* 27-JUL-2005    Ram Narayan       None        Initial
* 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
* 08-Feb-2006  Amit Sharma               Network PDU Logging API from ICF
* 09-FEB-2006   Rohit Aggarwal              Fixed SPR 10093
* 07-Mar-2006	Anuradha Gupta	  Nee	       Added code changes for Re-invite
*                                              handling
* 30-Mar-2006	Anuradha Gupta	  ICF 5.0      Call Modify Feature changes
* 31-Mar-2006   Deepti Goyal      ICF_5_0      Changes for unpacking of message
*                                              body headers.
* 02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments 
*											 Multiple Public Users ICF5.0
* 27-June-2006  Raveen Sharma	  ICF 6.0	 Added code changes for list handling	
* 29-Nov-2006   Deepti Goyal        ICF 6.2     SET CALL PARAMS API changes
* 03-Jan-2007   Amit Mahajan                   INFO confirmation from application.  
* 15-Jan-2007   Abhishek Dhammawat ICF 6.3    Additions for Per Line Feature  
* 23-Mar-2007   Deepti Goyal        ICF 6.4   Additions for Media_Connect Req
*                                             API.
* 10-July-2007  Abhishek Dutta                Additions for call modify cancel req
*                                             API.
*
* 04-Apr-2007   Deepti Goyal        ICF 6.3   Additions for Media_Connect Req
*                                             API.
* 27-Aug-2007   Deepti Goyal        SPR 15997   Line Id in configure_cfm API.
*
* 07-Jan-2008   Amit Sikka          SPR 17945 Support for PUBLISH/MESSAGE
* 11-May-2008   Alok Tiwari  CSR:1-5905629/SPR:18538 Modified function
*                                             icf_unpack_icf_set_trace_req_st
* 07-Nov-2008   Anurag Khare        SPR 19168 Modified function
*                                             icf_unpack_icf_set_call_params_req_st
* 07-Jan-2009   Vinay Jindal       SPR 19250  Modified Function 
*                                             icf_unpack_icf_set_service_params_req_st
* 05-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
* 19-Dec-2009  Rohit Aggarwal   SPR 20246  Configuration to strip Route header
* 23-Mar-2010  Rohit Aggarwal   SPR 20343  Event name in subscription ind
* 29-Mar-2010  Kamal Ashraf                   CSR_1-8389294(Registaration Enh)
* 09-Apr-2010  Alok Tiwari      SPR 20362     Modified Function
*                                             icf_unpack_icf_options_req_st
*
* Copyright 2010, Aricent.                       
*******************************************************************************/




#include "icf_clib.h" 
#include "icf_utils.h"
#include "icf_api.h"
#include "icf_trace_error_types.h"
#include "icf_trace_id.h"
#include "icf_stats_types.h"
#include "icf_unpack_api.h"


#ifdef ICF_LOAD_STAT
icf_uint32_t    g_create_call_req=0;
#endif

icf_uint32_t
icf_unpack_icf_set_call_params_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_call_params_req_st    *p_icf_set_call_params_req_st = ICF_NULL;
    
    p_icf_set_call_params_req_st    = (icf_set_call_params_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_call_params_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_call_params_req_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_DEFAULT_SETTINGS)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_set_call_params_req_st->default_settings), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_QOS_MARK)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_call_params_req_st->qos_mark), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SETUP_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->setup_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_ALERTING_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->alerting_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_RELEASE_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->release_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MODIFY_MEDIA_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->modify_media_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_REGISTRATION_RESP_DURATION)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->registration_resp_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_REGISTRATION_RETRY_DURATION)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->registration_retry_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_SESSION_TIMER
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->session_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->min_se), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

   if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_ENABLE)
    {
       icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->\
                session_timer_enable), p_src + (*p_length), &offset);
         
        *p_length    += offset;
    }
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_REFRESHER)
    {
        icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->\
                session_refresher), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_LOGIC)
    {
       icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->\
                session_timer_logic), p_src + (*p_length), &offset);
         
        *p_length    += offset;
    }
#endif

	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_INFO_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->info_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_OPTIONS_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->options_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_PHONE_INFO)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_call_params_req_st->phone_info), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_NAT_RPORT_SUPPORT
	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_BINDING_REFRESH_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->\
                    binding_refresh_timer), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->\
                binding_query_enable), p_src + (*p_length), &offset);

    *p_length    += offset;
#endif
     if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_MULTIPLE_ANSWER_ENABLE)
     {
    icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->\
                multiple_answer_enable), p_src + (*p_length), &offset);

    *p_length    += offset;
     }
	if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_UPDATE_REQD) 
    {
    icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->\
                update_reqd), p_src + (*p_length), &offset);

    *p_length    += offset;
    }

        if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_EARLY_INC_IND_FOR_ALL_USERS)
    {
    icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->\
                early_inc_ind_for_all_users), p_src + (*p_length), &offset);

    *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_DNS_MIN_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->dns_min_retrans), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_DNS_MAX_TIMER)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_call_params_req_st->dns_max_retrans), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
    /*  Fix for SPR 19168 */
    if(p_icf_set_call_params_req_st->bit_mask & ICF_SET_PORT_IN_FROM_HDR)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_call_params_req_st->port_to_be_sent_in_from_hdr),p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    /* Fix for CSR 1-6727350: SPR 19316 */
	if (p_icf_set_call_params_req_st->bit_mask & ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES)
    {
        icf_unpack_U8((icf_uint8_t *)&(p_icf_set_call_params_req_st->app_connect_reqd_for_replaces), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}




icf_uint32_t
icf_unpack_icf_set_scale_params_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_scale_params_req_st    *p_icf_set_scale_params_req_st = ICF_NULL;
    
    p_icf_set_scale_params_req_st    = (icf_set_scale_params_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_call_params_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_scale_params_req_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_set_scale_params_req_st->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;
    
    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_LINES)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_lines), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_AUDIO_CALLS)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_audio_calls), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_VIDEO_CALLS)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_video_calls), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_SERVICE_CALLS)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_service_calls), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_REGISTRATION_CALLS)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_registration_calls), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_USERS_PER_LINE)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_users_per_line), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_NUM_OF_CALLS_TO_LOG)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_scale_params_req_st->num_of_calls_to_log), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_set_scale_params_req_st->bit_mask & ICF_SET_SCALE_PRM_MAX_NUM_OF_APPS)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_scale_params_req_st->max_num_of_apps), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_req_uri_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t *p_length
)
{
    
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    
    icf_req_uri_st    *p_icf_req_uri_st = ICF_NULL;
    
    p_icf_req_uri_st    = (icf_req_uri_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_string_st */
    ICF_CLIB_TRACE(("Unpacking p_icf_req_uri_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_req_uri_st->str_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if (0 >= p_icf_req_uri_st->str_len || p_icf_req_uri_st->str_len > ICF_MAX_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_req_uri_st - Invalid String Length (%d) Bytes\n", p_icf_req_uri_st->str_len));
        return ICF_FAILURE;
    } else 
    {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_req_uri_st - Unpacking String of Length (%d) Bytes\n", p_icf_req_uri_st->str_len));

    }

    /* This piece of code unpacks variable length octet string */

    count = 0;
    /* RN Unpack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_req_uri_st->str_len + 1)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_req_uri_st->str[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}





icf_uint32_t
icf_unpack_icf_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    
    icf_uint32_t        offset = 0;
    
    icf_uint32_t        count = 0;
    icf_string_st    *p_icf_string_st = ICF_NULL;
    
    p_icf_string_st    = (icf_string_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_string_st */
    ICF_CLIB_TRACE(("Unpacking icf_string_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_string_st->str_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if (0 >= p_icf_string_st->str_len || p_icf_string_st->str_len > ICF_MAX_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_string_st - Invalid String Length (%d) Bytes\n", p_icf_string_st->str_len));
        return ICF_FAILURE;
    } else
    { 
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_string_st - Unpacking String of Length (%d) Bytes\n", p_icf_string_st->str_len));

    }

    /* This piece of code unpacks variable length octet string */

    count = 0;
    /* RN Unpack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_string_st->str_len + 1)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_string_st->str[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_large_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    
    icf_large_string_st    *p_icf_large_string_st = ICF_NULL;
    
    p_icf_large_string_st    = (icf_large_string_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_large_string_st */
    ICF_CLIB_TRACE(("Unpacking icf_large_string_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_large_string_st->str_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if (0 >= p_icf_large_string_st->str_len || p_icf_large_string_st->str_len > ICF_MAX_LARGE_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_large_string_st - Invalid String Length (%d) Bytes\n", p_icf_large_string_st->str_len));
        return ICF_FAILURE;
    } else { 
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_large_string_st - Unpacking String of Length (%d) Bytes\n", p_icf_large_string_st->str_len));
    } 


    /* This piece of code unpacks variable length octet string */

    count = 0;
    /* Unpack the NULL Character also */
    while((icf_uint16_t)count < p_icf_large_string_st->str_len + 1)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_large_string_st->str[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

/* Added code in ICF REL 6.0 for unpacking short string*/
icf_uint32_t
icf_unpack_icf_short_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;
    
    icf_short_string_st    *p_icf_short_string_st = ICF_NULL;
    
    p_icf_short_string_st    = (icf_short_string_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_short_string_st */
    ICF_CLIB_TRACE(("Unpacking icf_short_string_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_short_string_st->str_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if (0 >= p_icf_short_string_st->str_len || p_icf_short_string_st->str_len > ICF_MAX_SHORT_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_short_string_st - Invalid String Length (%d) Bytes\n", p_icf_short_string_st->str_len));
        return ICF_FAILURE;
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_short_string_st - Unpacking String of Length (%d) Bytes\n", p_icf_short_string_st->str_len));
   }


    /* This piece of code unpacks variable length octet string */

    count = 0;
    /* RN Unpack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_short_string_st->str_len + 1)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_short_string_st->str[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

/* Added code in ICF REL 6.0 for unpacking long string*/
icf_uint32_t
icf_unpack_icf_long_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    
    icf_long_string_st    *p_icf_long_string_st = ICF_NULL;
    
    p_icf_long_string_st    = (icf_long_string_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_long_string_st */
    ICF_CLIB_TRACE(("Unpacking icf_long_string_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_long_string_st->str_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if (0 >= p_icf_long_string_st->str_len || p_icf_long_string_st->str_len > ICF_MAX_LONG_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_long_string_st - Invalid String Length (%d) Bytes\n", p_icf_long_string_st->str_len));
        return ICF_FAILURE;
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_long_string_st - Unpacking String of Length (%d) Bytes\n", p_icf_long_string_st->str_len));
   }


    /* This piece of code unpacks variable length octet string */

    count = 0;
    /* RN Unpack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_long_string_st->str_len + 1)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_long_string_st->str[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_address_st    *p_icf_address_st = ICF_NULL;
    
    p_icf_address_st    = (icf_address_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_address_st */
    ICF_CLIB_TRACE(("Unpacking icf_address_st\n"));

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_address_st->addr_val), p_src + (*p_length), &offset);
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_address_st->addr_type), p_src + (*p_length), &offset);
    *p_length    += offset;
  
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_config_self_addr_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_self_addr_st    *p_icf_config_self_addr_st = ICF_NULL;
    
    p_icf_config_self_addr_st    = (icf_config_self_addr_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_self_addr_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_self_addr_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_self_addr_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_self_addr_st->action), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_config_self_addr_st->addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_config_self_addr_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_config_self_addr_list_st    *p_icf_config_self_addr_list_st = ICF_NULL;
    
    p_icf_config_self_addr_list_st    = (icf_config_self_addr_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_self_addr_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_self_addr_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_self_addr_list_st->all_lines), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_self_addr_list_st->num_of_elements), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_config_self_addr_list_st->num_of_elements)
    {
        icf_unpack_icf_config_self_addr_st((icf_uint8_t*)&(p_icf_config_self_addr_list_st->self_addr[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_config_self_name_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_self_name_st    *p_icf_config_self_name_st = ICF_NULL;
    
    p_icf_config_self_name_st    = (icf_config_self_name_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_self_name_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_self_name_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_self_name_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_self_name_st->action), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_config_self_name_st->name), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_config_self_name_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_config_self_name_list_st    *p_icf_config_self_name_list_st = ICF_NULL;
    
    p_icf_config_self_name_list_st    = (icf_config_self_name_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_self_name_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_self_name_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_self_name_list_st->all_lines), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_self_name_list_st->num_of_elements), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_config_self_name_list_st->num_of_elements)
    {
        icf_unpack_icf_config_self_name_st((icf_uint8_t*)&(p_icf_config_self_name_list_st->self_name[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_config_default_settings_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_default_settings_st    *p_icf_config_default_settings_st = ICF_NULL;
    
    p_icf_config_default_settings_st    = (icf_config_default_settings_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_default_settings_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_default_settings_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_default_settings_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_default_settings_st->default_settings), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_config_default_settings_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_config_default_settings_list_st    *p_icf_config_default_settings_list_st = ICF_NULL;
    
    p_icf_config_default_settings_list_st    = (icf_config_default_settings_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_default_settings_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_default_settings_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_default_settings_list_st->all_lines), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_default_settings_list_st->num_of_elements), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_config_default_settings_list_st->num_of_elements)
    {
        icf_unpack_icf_config_default_settings_st((icf_uint8_t*)&(p_icf_config_default_settings_list_st->default_settings[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_config_status_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_status_st    *p_icf_config_status_st = ICF_NULL;
    
    p_icf_config_status_st    = (icf_config_status_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_status_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_status_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_status_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U16((icf_uint8_t*)&(p_icf_config_status_st->status), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_config_status_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_config_status_list_st    *p_icf_config_status_list_st = ICF_NULL;
    
    p_icf_config_status_list_st    = (icf_config_status_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_status_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_status_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_status_list_st->all_lines), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_status_list_st->num_of_elements), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_config_status_list_st->num_of_elements)
    {
        icf_unpack_icf_config_status_st((icf_uint8_t*)&(p_icf_config_status_list_st->status[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_config_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_address_st    *p_icf_config_address_st = ICF_NULL;
    
    p_icf_config_address_st    = (icf_config_address_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_address_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_address_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_address_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_config_address_st->address), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_config_address_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_config_address_list_st    *p_icf_config_address_list_st = ICF_NULL;
    
    p_icf_config_address_list_st    = (icf_config_address_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_address_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_address_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_address_list_st->all_lines), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_address_list_st->num_of_elements), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_config_address_list_st->num_of_elements)
    {
        icf_unpack_icf_config_address_st((icf_uint8_t*)&(p_icf_config_address_list_st->address[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_ipv4_int_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_ipv4_int_address_st    *p_icf_ipv4_int_address_st = ICF_NULL;
    
    p_icf_ipv4_int_address_st    = (icf_ipv4_int_address_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_ipv4_int_address_st */
    ICF_CLIB_TRACE(("Unpacking icf_ipv4_int_address_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_1), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_2), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_3), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_ipv4_int_address_st->octet_4), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_transport_addr_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_transport_addr_st    *p_icf_transport_addr_st = ICF_NULL;
    
    p_icf_transport_addr_st    = (icf_transport_addr_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_transport_addr_st */
    ICF_CLIB_TRACE(("Unpacking icf_transport_addr_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_transport_addr_st->addr_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    switch (p_icf_transport_addr_st->addr_type)
    {
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
    
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_transport_addr_st->addr.domain), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
    
        icf_unpack_icf_ipv4_int_address_st((icf_uint8_t*)&(p_icf_transport_addr_st->addr.ipv4_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        break;
#ifdef ICF_IPV6_ENABLED
    case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_transport_addr_st-> addr.ipv6_addr), p_src +(*p_length), &offset);     
        *p_length += offset;
        break;
#endif
        
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_transport_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_transport_address_st    *p_icf_transport_address_st = ICF_NULL;
    
    p_icf_transport_address_st    = (icf_transport_address_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_transport_address_st */
    ICF_CLIB_TRACE(("Unpacking icf_transport_address_st\n"));
    icf_unpack_icf_transport_addr_st((icf_uint8_t*)&(p_icf_transport_address_st->addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U16((icf_uint8_t*)&(p_icf_transport_address_st->port_num), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}

#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_unpack_icf_network_activate_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_network_activate_req_st    *p_icf_network_activate_req_st = ICF_NULL;
    
    p_icf_network_activate_req_st    = (icf_network_activate_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_network_activate_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_network_activate_req_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_network_activate_req_st->access_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_network_activate_req_st->access_info), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_icf_network_activate_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if (ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT & p_icf_network_activate_req_st->bit_mask)
    {
        icf_unpack_icf_void_st((icf_uint8_t*)&(p_icf_network_activate_req_st->routing_info), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    
    return ICF_SUCCESS;
}
#endif

icf_uint32_t
icf_unpack_icf_void_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_void_st    *p_icf_void_st = ICF_NULL;
    
    p_icf_void_st    = (icf_void_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_void_st */
    ICF_CLIB_TRACE(("Unpacking icf_void_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_void_st->len), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    p_icf_void_st->ptr = ICF_NULL;
    /* This piece of code unpacks variable length octet string */

    if (0 < p_icf_void_st->len  && p_icf_void_st->len < ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: Getting memory for icf_void_st\n"));
        p_icf_void_st->ptr = ICF_CLIB_MEMGET(p_icf_void_st->len);

        if (!p_icf_void_st->ptr) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: Failed In Allocating Memory for icf_void_st->ptr\n"));
            return ICF_FAILURE;
        }
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_void_st - Invalid Structure Length!! Quitting! (%d) Bytes\n"));
        return ICF_FAILURE;
    }

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_void_st - Unpacking Structure of Length (%d)\n", p_icf_void_st->len));

    count = 0;
    while(count < p_icf_void_st->len && p_icf_void_st->ptr)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_void_st->ptr[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_unpack_icf_event_indication_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_event_indication_st    *p_icf_event_indication_st = ICF_NULL;
    
    p_icf_event_indication_st    = (icf_event_indication_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_event_indication_st */
    ICF_CLIB_TRACE(("Unpacking icf_event_indication_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_event_indication_st->event_code), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_void_st((icf_uint8_t*)&(p_icf_event_indication_st->event_data), p_src + (*p_length), &offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_network_activate_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_network_activate_cfm_st    *p_icf_network_activate_cfm_st = ICF_NULL;
    
    p_icf_network_activate_cfm_st    = (icf_network_activate_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_network_activate_cfm_st */
    ICF_CLIB_TRACE(("Unpacking icf_network_activate_cfm_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_network_activate_cfm_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_network_activate_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_network_activate_cfm_st->bit_mask & ICF_NET_ACT_CFM_ERR_CAUSE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_network_activate_cfm_st->error_cause), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_activate_cfm_st->bit_mask & ICF_NET_ACT_CFM_NUM_NW_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_network_activate_cfm_st->num_nw), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_activate_cfm_st->bit_mask & ICF_NET_ACT_CFM_NW_INFO_PRESENT)
    {
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_network_activate_cfm_st->num_nw)
    {
        icf_unpack_icf_network_info_st((icf_uint8_t*)&(p_icf_network_activate_cfm_st->nw_info[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    }

    return ICF_SUCCESS;
}
#endif/*#ifdef ICF_NW_ACTIVATE*/

icf_uint32_t
icf_unpack_icf_pub_uri_list_st
(
    icf_pub_uri_list_node_st **p_p_icf_uri_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_uri_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_pub_uri_list_node_st \n"));
    icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    while(count--)
    {
        *p_p_icf_uri_list_st = (icf_pub_uri_list_node_st *)ICF_CLIB_MEMGET(sizeof(icf_pub_uri_list_node_st));
      
        icf_unpack_icf_address_st((icf_uint8_t*)&((*p_p_icf_uri_list_st)->pub_uri), p_src + (*p_length), &offset);
        *p_length    += offset;

        icf_unpack_icf_address_list_node_st(&((*p_p_icf_uri_list_st)->p_assoc_uri_list), p_src + (*p_length), &offset);
        *p_length    += offset;
        p_p_icf_uri_list_st = &((*p_p_icf_uri_list_st)->p_next);
    }
    (*p_p_icf_uri_list_st) = ICF_NULL;

    return ICF_SUCCESS;
}

icf_uint32_t 
icf_unpack_icf_address_list_node_st
(
    icf_address_list_node_st    **p_p_icf_address_list_node_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_uri_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_address_list_node_st\n"));
    icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
    *p_length    += offset;
    

    while(count--)
    {
        *p_p_icf_address_list_node_st = (icf_address_list_node_st *)ICF_CLIB_MEMGET(sizeof(icf_address_list_node_st))

        icf_unpack_icf_address_st((icf_uint8_t*)&((*p_p_icf_address_list_node_st)->user_addr), p_src + (*p_length), &offset);
        *p_length    += offset;

        p_p_icf_address_list_node_st = &((*p_p_icf_address_list_node_st)->p_next);
    }
    *p_p_icf_address_list_node_st = ICF_NULL;
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_transport_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_transport_list_st    *p_icf_transport_list_st = ICF_NULL;
    
    p_icf_transport_list_st    = (icf_transport_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_transport_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_transport_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_transport_list_st->num_of_elements), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_transport_list_st->num_of_elements)
    {
        icf_unpack_icf_transport_addr_st((icf_uint8_t*)&(p_icf_transport_list_st->addr[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;

}

/************************************************************************
*
* FUNCTION: icf_unpack_icf_user_reg_status_st 
*
******************************************************************/



icf_uint32_t
icf_unpack_icf_user_reg_status_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
        icf_uint32_t        offset = 0;

    icf_user_reg_status_st    *p_icf_user_reg_status_st = ICF_NULL;

    p_icf_user_reg_status_st    = (icf_user_reg_status_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_user_reg_status_st */
    ICF_CLIB_TRACE(("Unpacking icf_user_reg_status_st\n"));

    icf_unpack_icf_address_st((icf_uint8_t*)&
        (p_icf_user_reg_status_st->user), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&
        (p_icf_user_reg_status_st->reg_status), p_src + (*p_length), &offset);

    *p_length    += offset;


     return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_user_reg_status_list_st
(
    icf_list_st    **p_p_icf_user_reg_status_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint8_t        count = 0;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_user_reg_status_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_user_reg_status_list_st\n"));

    icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);

    *p_length    += offset;


    while(count--)
    {

             *p_p_icf_user_reg_status_list_st = 
                   (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

             /* get memory for icf_user_reg_status_st*/

            (*p_p_icf_user_reg_status_list_st)->p_data = 
                         (icf_user_reg_status_st*)ICF_CLIB_MEMGET
                                     (sizeof(icf_user_reg_status_st));



            icf_unpack_icf_user_reg_status_st((icf_uint8_t*)
                  ((*p_p_icf_user_reg_status_list_st)->p_data), 
                                        p_src + (*p_length), &offset);

                *p_length    += offset;

           p_p_icf_user_reg_status_list_st = 
                         &((*p_p_icf_user_reg_status_list_st)->p_next);

       
    }

    *p_p_icf_user_reg_status_list_st = ICF_NULL;

    return ICF_SUCCESS;
}

#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_unpack_icf_network_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{

    icf_uint32_t        offset = 0;

    icf_network_info_st    *p_icf_network_info_st = ICF_NULL;
    
    p_icf_network_info_st    = (icf_network_info_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_network_info_st */
    ICF_CLIB_TRACE(("Unpacking icf_network_info_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_network_info_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_network_info_st->ctxt_id), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_SELF_IP_PRESENT)
    {
        icf_unpack_icf_transport_addr_st((icf_uint8_t*)&(p_icf_network_info_st->self_ip), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_network_info_st->access_type), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_network_info_st->access_info), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
 
    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT)
    {
		icf_unpack_icf_pub_uri_list_st(&(p_icf_network_info_st->p_uri_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
    }

    if(p_icf_network_info_st->bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT)
    {
        icf_unpack_icf_transport_list_st((icf_uint8_t*)&(p_icf_network_info_st->out_proxy_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }


    return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_network_deactivate_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_network_deactivate_req_st    *p_icf_network_deactivate_req = ICF_NULL;
    
    p_icf_network_deactivate_req    = (icf_network_deactivate_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_network_deactivate_req */
    ICF_CLIB_TRACE(("Unpacking icf_network_deactivate_req\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_network_deactivate_req->ctxt_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_network_deactivate_req->bit_mask), &offset);
    
    *p_length    += offset;
    
    if (ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT & p_icf_network_deactivate_req->bit_mask)
    {
        
        icf_unpack_icf_void_st((icf_uint8_t*)&(p_icf_network_deactivate_req->routing_info), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    
    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_network_deactivate_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_network_deactivate_cfm_st    *p_icf_network_deactivate_cfm_st = ICF_NULL;
    
    p_icf_network_deactivate_cfm_st    = (icf_network_deactivate_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_network_deactivate_cfm_st */
    ICF_CLIB_TRACE(("Unpacking icf_network_deactivate_cfm_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    
    if(p_icf_network_deactivate_cfm_st->bit_mask & ICF_NET_DEACT_CFM_ERR_CAUSE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->error_cause), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_deactivate_cfm_st->bit_mask & ICF_NET_DEACT_CFM_NUM_NW_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->num_nw), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_network_deactivate_cfm_st->bit_mask & ICF_NET_DEACT_CFM_NW_INFO_PRESENT)
    {
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_network_deactivate_cfm_st->num_nw)
    {
        icf_unpack_icf_network_info_st((icf_uint8_t*)&(p_icf_network_deactivate_cfm_st->nw_info[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    }

    return ICF_SUCCESS;
}
#endif/*#ifdef ICF_NW_ACTIVATE*/

icf_uint32_t
icf_unpack_icf_filter_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_filter_st    *p_icf_filter_st = ICF_NULL;
    
    p_icf_filter_st = (icf_filter_st*)p_dest;

    /* Initialize length to 0 */
    *p_length = 0;


    ICF_CLIB_TRACE(("Unpacking icf_filter_st\n"));
    /* filter_type */
    icf_unpack_U8((icf_uint8_t*)&(p_icf_filter_st->filter_type),p_src + (*p_length), &offset);
    *p_length    += offset;
    /* match_type */
    icf_unpack_U8((icf_uint8_t*)&(p_icf_filter_st->match_type),p_src + (*p_length), &offset);
    *p_length    += offset;
    /*target_header*/
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_filter_st->target_header),p_src + (*p_length), &offset);
    *p_length    += offset;
    /*expr_str*/
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_filter_st->expr_str),p_src + (*p_length), &offset);
    *p_length    += offset;

    return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_filter_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        i;

    icf_filter_st       **p_p_filter;

    icf_filter_list_st    *p_icf_filter_list_st = ICF_NULL;
    
    p_icf_filter_list_st = (icf_filter_list_st*)p_dest;

    /* Initialize length to 0 */
    *p_length    = 0;

    ICF_CLIB_TRACE(("Unpacking icf_filter_list_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_filter_list_st->count),p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_filter_list_st->match_all),p_src + (*p_length), &offset);

    *p_length    += offset;
    
    for(i=0,p_p_filter = &(p_icf_filter_list_st->p_filter_list);
        i < p_icf_filter_list_st->count;
        i++,p_p_filter = &((*p_p_filter)->p_next))
    {
        /* get memory */
        *p_p_filter = (icf_filter_st*)ICF_CLIB_MEMGET(sizeof(icf_filter_st));
        icf_unpack_icf_filter_st((icf_uint8_t*)(*p_p_filter),p_src + (*p_length), &offset);
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_apn_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_uint32_t        count = 0;
    icf_apn_list_st    *p_icf_apn_list_st = ICF_NULL;
    
    p_icf_apn_list_st    = (icf_apn_list_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_apn_list_st */
    ICF_CLIB_TRACE(("Packing icf_apn_list_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_apn_list_st->count),p_src + (*p_length), &offset);
        
    *p_length    += offset;

    /* This piece of code packs a variable length octet string */
    count = 0;
    if(p_icf_apn_list_st->count > 0)
    { 
      p_icf_apn_list_st->p_list_of_apn = 
         (icf_string_st*)ICF_CLIB_MEMGET(sizeof(icf_string_st)*(p_icf_apn_list_st->count));
    }

    while(count < p_icf_apn_list_st->count)
    {
        offset = 0;
        if( ICF_NULL != p_icf_apn_list_st->p_list_of_apn )
        {
            icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_apn_list_st->p_list_of_apn[count]), p_src + (*p_length), &offset);
        }
        *p_length    += offset;
        
        count++;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_get_line_settings_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_get_line_settings_req_st    *p_icf_get_line_settings_req_st = ICF_NULL;
    
    p_icf_get_line_settings_req_st    = (icf_get_line_settings_req_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_line_settings_req_st */
    ICF_CLIB_TRACE(("Packing icf_get_line_settings_req_st\n"));

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_line_settings_req_st->bit_mask), p_src + (*p_length), &offset);
        
    *p_length    += offset;
    
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_get_line_settings_req_st->line_id), p_src + (*p_length), &offset);
        
    *p_length    += offset;
/* FIX for spr 17944 */ 
    if(ICF_GET_SETTINGS_ROUTE_INFO_PRESENT & p_icf_get_line_settings_req_st->bit_mask)
    {
    icf_unpack_icf_void_st((icf_uint8_t*)&(p_icf_get_line_settings_req_st->routing_info), p_src + (*p_length), &offset);
        
    *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_get_line_settings_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    
    icf_get_line_settings_resp_st    *p_icf_get_line_settings_resp_st = ICF_NULL;
    
    p_icf_get_line_settings_resp_st    = (icf_get_line_settings_resp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_line_settings_resp_st */
    ICF_CLIB_TRACE(("Packing icf_get_line_settings_resp_st\n"));

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_line_settings_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_get_line_settings_resp_st->proxy_address), p_src + (*p_length), &offset);
            
    *p_length    += offset;

    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_get_line_settings_resp_st->registrar_address), p_src + (*p_length), &offset);
            
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t *)&(p_icf_get_line_settings_resp_st->proxy_transport_mode), p_src + (*p_length), &offset);
        
    *p_length    += offset;

    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_get_line_settings_resp_st->self_address_list), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_icf_string_st((icf_uint8_t *)&(p_icf_get_line_settings_resp_st->self_name), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t *)&(p_icf_get_line_settings_resp_st->presentation_allowed), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_icf_string_st((icf_uint8_t *)&(p_icf_get_line_settings_resp_st->auth_key), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_icf_string_st((icf_uint8_t *)&(p_icf_get_line_settings_resp_st->auth_password), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_line_settings_resp_st->reg_mode), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_icf_get_line_settings_resp_st->reg_duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_get_line_settings_resp_st->reg_route_addr), p_src + (*p_length), &offset);
            
    *p_length    += offset;


    icf_unpack_U32((icf_uint8_t*)&
            (p_icf_get_line_settings_resp_st->sip_T1_timer),
                              p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&
            (p_icf_get_line_settings_resp_st->sip_B_timer), 
                              p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&
            (p_icf_get_line_settings_resp_st->sip_F_timer), 
                              p_src + (*p_length), &offset);

    *p_length    += offset;

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_get_sys_settings_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;



    icf_get_system_settings_req_st    *p_icf_get_sys_settings_req_st = ICF_NULL;
    
    p_icf_get_sys_settings_req_st    = (icf_get_system_settings_req_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_system_settings_req_st */
    ICF_CLIB_TRACE(("Packing icf_get_system_settings_req_st\n"));
        
    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_req_st->bit_mask), p_src + (*p_length), &offset);

    *p_length    += offset;
    
    icf_unpack_icf_void_st((icf_uint8_t*)&(p_icf_get_sys_settings_req_st->routing_info), p_src + (*p_length), &offset);
    
    *p_length    += offset;



return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_get_sys_settings_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    
    icf_get_system_settings_resp_st    *p_icf_get_sys_settings_resp_st = ICF_NULL;
    
    p_icf_get_sys_settings_resp_st    = (icf_get_system_settings_resp_st *)p_dest;

/*       icf_uint32_t                     bit_mask;

    icf_boolean_t                         prack_enabled;
    icf_uint16_t                     auth_type;
    icf_boolean_t                     privacy_supported;
    icf_boolean_t                        max_lines;
    icf_duration_t                   setup_timer;
    icf_duration_t                   alerting_timer;
    icf_duration_t                       release_timer;
    icf_duration_t                   modify_media_timer;
    icf_duration_t                    registration_resp_timer;
    icf_duration_t                   registration_retry_timer;
    icf_duration_t                   subs_retry_timer;
    icf_duration_t                     subs_resp_timer;
    icf_boolean_t                         sig_comp_enabled;
    icf_boolean_t                         security_enabled;
    icf_transport_address_st         self_ip_address;
    icf_uri_list_st                     asso_uri_list;
    icf_apn_list_st                     apn_list;
#ifdef ICF_NAT_RPORT_SUPPORT
     icf_duration_t       binding_refresh_timer;
     icf_boolean_t        binding_query_enable;
#endif


} icf_get_system_settings_resp_st; */

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_system_settings_resp_st */
    ICF_CLIB_TRACE(("Packing icf_get_system_settings_resp_st\n"));

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->bit_mask), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->prack_enabled), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->auth_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->privacy_supported), p_src + (*p_length), &offset);
        
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->max_lines), p_src + (*p_length), &offset);
        
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->setup_timer), p_src + (*p_length), &offset);
            
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->alerting_timer), p_src + (*p_length), &offset);
            
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->release_timer), p_src + (*p_length), &offset);    
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->modify_media_timer), p_src + (*p_length), &offset);    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->registration_resp_timer), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->registration_retry_timer), p_src + (*p_length), &offset);    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->subs_retry_timer), p_src + (*p_length), &offset);    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->subs_resp_timer), p_src + (*p_length), &offset);    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->sig_comp_enabled), p_src + (*p_length), &offset);    
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t *)&(p_icf_get_sys_settings_resp_st->security_enabled), p_src + (*p_length), &offset);    
        
    *p_length    += offset;
    
    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->self_ip_address), p_src + (*p_length), &offset);
            
    *p_length    += offset;
    
    icf_unpack_icf_pub_uri_list_st(&(p_icf_get_sys_settings_resp_st->p_uri_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_apn_list_st((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->apn_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->self_transport_mode), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->self_port), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->call_thru_proxy), p_src + (*p_length),&offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->sip_compact_header), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->session_interval_timer), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->min_session_interval_timer), p_src + (*p_length), &offset);
    
    *p_length    += offset;

#ifdef ICF_NAT_RPORT_SUPPORT

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->binding_query_enable), p_src + (*p_length), &offset);

    *p_length    += offset;

 icf_unpack_U32((icf_uint8_t*)&(p_icf_get_sys_settings_resp_st->binding_refresh_timer), p_src + (*p_length), &offset);
    
    *p_length    += offset;

#endif        

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_string_list_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t *p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		i;

    icf_string_list_node_st	**p_p_list;

	icf_string_list_st	*p_icf_string_list_st = ICF_NULL;
	
	p_icf_string_list_st	= (icf_string_list_st*)p_dest;

    /* Initialize length to 0 */
	*p_length	= 0;

	ICF_CLIB_TRACE(("Unpacking icf_string_list_st\n"));
	icf_unpack_U32((icf_uint8_t*)&(p_icf_string_list_st->count), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	
    for(i=0,p_p_list = &(p_icf_string_list_st->p_list);
        i < p_icf_string_list_st->count;
        i++,p_p_list = &((*p_p_list)->p_next))
	{
        /* get memory */
        *p_p_list = (icf_string_list_node_st*)ICF_CLIB_MEMGET(sizeof(icf_string_list_node_st));
        icf_unpack_icf_string_st((icf_uint8_t*)(*p_p_list),p_src + (*p_length), &offset);
		*p_length	+= offset;
	}
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_app_add_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_app_add_req_st    *p_icf_app_add_req_st = ICF_NULL;

	p_icf_app_add_req_st    = (icf_app_add_req_st*)p_dest;

	/* Initialise length to 0 */
	*p_length    = 0;

	/* This function is for unpacking icf_app_add_req_st */
	ICF_CLIB_TRACE(("Unpacking icf_app_add_req_st\n"));

	icf_unpack_U8((icf_uint8_t*)&(p_icf_app_add_req_st->bit_mask), p_src + (*p_length), &offset);

	*p_length += offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_app_add_req_st->app_name), p_src + (*p_length), &offset);

	*p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_app_add_req_st->routing_info.len), p_src + (*p_length), &offset);

	*p_length += offset;

	p_icf_app_add_req_st->routing_info.ptr = ICF_NULL;

	p_icf_app_add_req_st->routing_info.ptr = ICF_CLIB_MEMGET(sizeof(icf_route_info_st));

	if ( ICF_NULL == p_icf_app_add_req_st->routing_info.ptr)
		return ICF_FAILURE;

	icf_unpack_icf_route_info_st((icf_uint8_t*)(p_icf_app_add_req_st->routing_info.ptr), p_src + (*p_length), &offset);

	*p_length    += offset;

#ifdef ICF_REL_OLD
	icf_unpack_U8((icf_uint8_t *)&(p_icf_app_add_req_st->call_type), p_src + (*p_length), &offset);

	*p_length    += offset;
#endif

	icf_unpack_icf_filter_list_st((icf_uint8_t*)&(p_icf_app_add_req_st->filter), p_src + (*p_length), &offset);

	*p_length    += offset;

	icf_unpack_icf_header_name_list_st((icf_uint8_t*)&(p_icf_app_add_req_st->header_name_list), p_src + (*p_length), &offset);

	*p_length    += offset;

	icf_unpack_icf_string_list_st((icf_uint8_t*)&(p_icf_app_add_req_st->supported_methods), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_icf_string_list_st((icf_uint8_t*)&(p_icf_app_add_req_st->supported_events), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_icf_string_list_st((icf_uint8_t*)&(p_icf_app_add_req_st->supported_content_types), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_icf_string_list_st((icf_uint8_t*)&(p_icf_app_add_req_st->supported_encodings), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_icf_string_list_st((icf_uint8_t*)&(p_icf_app_add_req_st->supported_languages), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	
/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_app_add_req_st->bit_mask & ICF_APP_ADD_MEDIA_PROFILE_PRESENT)
	{
		icf_unpack_icf_stream_capab_list_st(&(p_icf_app_add_req_st->p_media_profile), p_src + (*p_length), &offset);
	
		*p_length	+= offset;
	}

	icf_unpack_U8((icf_uint8_t *)&(p_icf_app_add_req_st->rtp_detection_reqd), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t *)&(p_icf_app_add_req_st->icf_simul_call_line_arbitration), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

        icf_unpack_U8(
        (icf_uint8_t *)&(p_icf_app_add_req_st->merge_req_wo_rem_sdp),
        p_src + (*p_length), &offset);
	
        *p_length       += offset;

    return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_header_name_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_name_st    *p_icf_header_name_st = ICF_NULL;
    
    p_icf_header_name_st    = (icf_header_name_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_header_name_st */
    ICF_CLIB_TRACE(("Unpacking icf_header_name_st\n"));

    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_header_name_st->hdr_name), p_src + (*p_length), &offset);

    *p_length    += offset;

    /* RN -- Manually added code*/
    if (p_icf_header_name_st->next) {
        icf_unpack_icf_header_name_st((icf_uint8_t*)p_icf_header_name_st->next, p_src + (*p_length), &offset);
        *p_length += offset;
    }

    
    return ICF_SUCCESS;
}
icf_uint32_t
icf_unpack_icf_header_name_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_name_list_st    *p_icf_header_name_list_st = ICF_NULL;
    
    p_icf_header_name_list_st    = (icf_header_name_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_header_name_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_header_name_list_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_header_name_list_st->count), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    

    /* RN -- Manually added code*/
    if (!(p_icf_header_name_list_st->count))
        return ICF_SUCCESS;

    p_icf_header_name_list_st->hdr = ICF_NULL;

    if (icf_make_hdr_name_node(&(p_icf_header_name_list_st->hdr), 
        (icf_uint8_t)p_icf_header_name_list_st->count) == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_header_name_list_st() Aborted\n"));
        return ICF_FAILURE;
    }



    icf_unpack_icf_header_name_st((icf_uint8_t*)(p_icf_header_name_list_st->hdr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_app_add_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
#ifdef ICF_NW_ACTIVATE
    icf_uint32_t        count = 0;
#endif
    icf_app_add_cfm_st    *p_icf_app_add_cfm_st = ICF_NULL;
    
    p_icf_app_add_cfm_st    = (icf_app_add_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_app_add_cfm_st */
    ICF_CLIB_TRACE(("Unpacking icf_app_add_cfm_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_app_add_cfm_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_app_add_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_app_add_cfm_st->bit_mask & ICF_APP_ADD_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_app_add_cfm_st->error_cause), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_NW_ACTIVATE
    if(p_icf_app_add_cfm_st->bit_mask & ICF_APP_ADD_CFM_NUM_NW_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_app_add_cfm_st->num_nw), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_app_add_cfm_st->bit_mask & ICF_APP_ADD_CFM_NET_INFO_PRESENT)
    {
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_app_add_cfm_st->num_nw)
    {
        icf_unpack_icf_network_info_st((icf_uint8_t*)&(p_icf_app_add_cfm_st->nw_info[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    }
#endif

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_app_remove_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_app_remove_req_st    *p_icf_app_remove_req_st = ICF_NULL;
    
    p_icf_app_remove_req_st    = (icf_app_remove_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_app_remove_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_app_remove_req_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_app_remove_req_st->option), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}
icf_uint32_t
icf_unpack_icf_app_remove_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_app_remove_cfm_st    *p_icf_app_remove_cfm_st = ICF_NULL;
    
    p_icf_app_remove_cfm_st    = (icf_app_remove_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_app_remove_cfm_st */
    ICF_CLIB_TRACE(("Unpacking icf_app_remove_cfm_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_app_remove_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U16((icf_uint8_t*)&(p_icf_app_remove_cfm_st->error_cause), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_set_self_id_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_self_id_req_st    *p_icf_set_self_id_req_st = ICF_NULL;
    
    p_icf_set_self_id_req_st    = (icf_set_self_id_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_self_id_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_self_id_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_set_self_id_req_st->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_set_self_id_req_st->line_id), p_src + (*p_length), &offset);
    *p_length    += offset;

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_ADDR_ACTION)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_self_id_req_st->action), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
    
    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_ADDR)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_set_self_id_req_st->self_addr), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_PRIORITY)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_self_id_req_st->priority), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_NAME)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_self_id_req_st->self_name), p_src + (*p_length), &offset);
        *p_length    += offset;        
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_self_id_req_st->default_settings), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_AUTH_KEY)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_self_id_req_st->authentication_key), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_AUTH_PASSWORD)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_self_id_req_st->authentication_password), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_self_id_req_st->bit_mask & ICF_SET_SLF_ID_PRM_REALM)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_self_id_req_st->configured_realm), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_set_transport_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_transport_req_st    *p_icf_set_transport_req_st = ICF_NULL;
    
    p_icf_set_transport_req_st    = (icf_set_transport_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_transport_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_transport_req_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_set_transport_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_PROXY_ADDR)
    {
        icf_unpack_icf_config_proxy_transport_address_st((icf_uint8_t*)&(p_icf_set_transport_req_st->proxy_addr), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_TRANSPORT_ADDRESS)
    {
        icf_unpack_icf_transport_addr_st((icf_uint8_t*)&(p_icf_set_transport_req_st->self_transport_address), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_MODE)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_transport_req_st->self_mode), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_PORT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_set_transport_req_st->self_port), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE)
    {
        icf_unpack_icf_config_transport_address_st(
          (icf_uint8_t*)&(p_icf_set_transport_req_st->self_addr_per_line), 
          p_src + (*p_length), &offset);
        *p_length    += offset;
    }
     if(p_icf_set_transport_req_st->bit_mask & ICF_SET_TRP_REQ_DNS_SRC_PORT)
    {
           icf_unpack_U16((icf_uint8_t*)&(p_icf_set_transport_req_st->dns_source_port), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
    
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_config_proxy_transport_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_proxy_transport_address_st    *p_icf_config_transport_address_st = ICF_NULL;
    
    p_icf_config_transport_address_st    = (icf_config_proxy_transport_address_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_transport_address_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_proxy_transport_address_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_transport_address_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_transport_address_st->action), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_config_transport_address_st->transport_addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_transport_address_st->transport_mode), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_transport_address_st->sigcomp_options), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_config_transport_address_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_transport_address_st    
                         *p_icf_config_transport_address_st = ICF_NULL;

    p_icf_config_transport_address_st    = (icf_config_transport_address_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_transport_address_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_transport_address_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_transport_address_st->line_id), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_transport_address_st->action), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_config_transport_address_st->transport_addr), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_transport_address_st->transport_mode), p_src + (*p_length), &offset);

    *p_length    += offset;

    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_config_transport_address_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_config_transport_address_list_st    *p_icf_config_transport_address_list_st = ICF_NULL;
    
    p_icf_config_transport_address_list_st    = (icf_config_transport_address_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_transport_address_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_transport_address_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_transport_address_list_st->all_lines), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_transport_address_list_st->num_of_elements), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_config_transport_address_list_st->num_of_elements)
    {
        icf_unpack_icf_config_transport_address_st((icf_uint8_t*)&(p_icf_config_transport_address_list_st->transport_addr[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_set_registration_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_set_registration_req_st    *p_icf_set_registration_req_st = ICF_NULL;
    
    p_icf_set_registration_req_st    = (icf_set_registration_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_registration_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_registration_req_st\n"));

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_set_registration_req_st->line_id), p_src + (*p_length), &offset);
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_set_registration_req_st->action), p_src + (*p_length), &offset);
    *p_length    += offset;
    
    
    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_set_registration_req_st->registrar_addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    

    icf_unpack_U8((icf_uint8_t*)&(p_icf_set_registration_req_st->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;

     if(p_icf_set_registration_req_st->bit_mask 
        & ICF_SET_REG_HS_DURATION)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_registration_req_st-> register_head_start_timer), p_src + (*p_length), &offset);
        *p_length    += offset;
    }


    if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_REQ_MODE)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_registration_req_st->reg_mode), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_REQ_DURATION)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_set_registration_req_st->registration_duration), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
 
    if(p_icf_set_registration_req_st->bit_mask & ICF_SET_REG_REQ_ROUTE_ADDR)
    {
        icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_set_registration_req_st->reg_route_addr), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
      
    return(ICF_SUCCESS);
}


#ifdef ICF_SERVICE_ENABLE


icf_uint32_t
icf_unpack_icf_set_service_params_req_st
( 
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length)
{
	icf_uint32_t        offset = 0;

	icf_set_service_params_req_st *p_icf_set_service_params_st = ICF_NULL; 
    
    p_icf_set_service_params_st   = (icf_set_service_params_req_st *)p_dest;
 
	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_service_params_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_service_params_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->bit_mask), p_src + (*p_length), &offset);
	*p_length    += offset;
	
	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_SUBSCRIPTION_FLAG)
	{
		icf_unpack_icf_config_status_st((icf_uint8_t*)&(p_icf_set_service_params_st->service_subscription_flag), p_src + (*p_length), &offset);
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_ACTIVATION_FLAG)
	{
		icf_unpack_icf_config_status_st((icf_uint8_t*)&(p_icf_set_service_params_st->service_activation_flag), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}	

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CALL_HOLD_OPTION)
	{
		/*icf_unpack_U8((icf_uint8_t*)&(p_icf_set_service_params_st->call_hold_option), p_src + (*p_length), &offset);	*/
		icf_unpack_icf_hold_option_st((icf_uint8_t*)&(p_icf_set_service_params_st->call_hold_option),
                                      p_src + (*p_length), &offset);
        *p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_MAX_NUM_OF_HOPS)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_set_service_params_st->max_num_of_call_forwarding_hops), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_MAX_CALL_WAITING)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_set_service_params_st->max_num_of_calls_in_call_waiting), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CFU_ADDR)
	{
		icf_unpack_icf_config_address_st((icf_uint8_t*)&(p_icf_set_service_params_st->addr_for_cfu), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CFB_ADDR)
	{
		icf_unpack_icf_config_address_st((icf_uint8_t*)&(p_icf_set_service_params_st->addr_for_cfb), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CFD_ADDR)
	{
		icf_unpack_icf_config_address_st((icf_uint8_t*)&(p_icf_set_service_params_st->addr_for_cfd), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CHR_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->call_hold_resume_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_REINVITE_RACE_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->reinvite_race_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_TWC_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->twc_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CTU_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->ctu_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CTA_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->cta_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_RNW_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->rnw_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CW_ALERT_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st-> cw_alert_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CND_ALERT_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st-> cnd_alert_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CW_N_CND_ALERT_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->cw_n_cnd_alert_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_SUBS_RESP_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st->subs_resp_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}

	if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_SUBS_RETRY_TIMER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st-> subs_retry_timer), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}
    if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CONF_REQD_AT_TRANFEREE)
    {

        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_service_params_st->conf_reqd_at_transferee),
                p_src + (*p_length), &offset);	
        *p_length    += offset;
    }
    if(p_icf_set_service_params_st->bit_mask & ICF_SET_SER_PRM_CONF_CFM_REQD_AT_TRANSFEREE)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_set_service_params_st->conf_cfm_reqd_at_transferee),
                p_src + (*p_length), &offset);	
        *p_length    += offset;
    }
	if(p_icf_set_service_params_st->bit_mask & 
            ICF_SET_SER_PRM_CONFERENCE_URI_PRESENT)
	{
		icf_unpack_icf_config_address_st((icf_uint8_t*)&(p_icf_set_service_params_st->conference_uri), p_src + (*p_length), &offset);	
		*p_length    += offset;
	}
	if(p_icf_set_service_params_st->bit_mask & 
            ICF_SET_SER_PRM_SUBSCRIPTION_REQD_IN_EXT_CONF)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_set_service_params_st-> \
          xconf_subs_duration), p_src + (*p_length), &offset);
		*p_length    += offset;
	}
	if(p_icf_set_service_params_st->bit_mask & 
						ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD)
    {
			icf_unpack_U8(
               (icf_uint8_t*)&(p_icf_set_service_params_st->conf_fail_call_clear_not_reqd),
				p_src + (*p_length), &offset);	
			*p_length    += offset;
    }


    return(ICF_SUCCESS);
}


/**************************************

			ICF REL 7.0

****************************************/

icf_uint32_t
icf_unpack_icf_create_xconf_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_create_ext_xconf_req_st    *p_icf_create_ext_xconf_req_st = ICF_NULL;
    
    p_icf_create_ext_xconf_req_st    = (icf_create_ext_xconf_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_create_ext_xconf_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_create_ext_xconf_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->bitmask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_CALLING_PARTY_ADDR_PRESENT)
    {   
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->calling_party_address), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_CALLING_PARTY_NAME_PRESENT)
    {
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->calling_party_name), p_src + (*p_length), &offset);
        *p_length    += offset;
    }    
     if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_TOGGLE_PRES_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->toggle_presentation_indicator), p_src + (*p_length), &offset);
        
        *p_length    += offset;

    }
    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_CONF_URI_PRESENT)
     { 
    	icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->conference_uri), p_src + (*p_length), &offset);

        *p_length    += offset;
     }
     
    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_EXIST_CALL_ID_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->existing_call_id),p_src + (*p_length), &offset);

        *p_length    += offset;
    }

    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
	icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st-> header_list), p_src + (*p_length), &offset);

        *p_length	+= offset;
    }
    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    { 
	icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_create_ext_xconf_req_st->tag_list), p_src + (*p_length), &offset); 
    
        *p_length	+= offset;
    }

    if(p_icf_create_ext_xconf_req_st->bitmask & ICF_CREATE_XCONF_PREF_MEDIA_PROFILE_PRESENT)
    {
	icf_unpack_icf_stream_capab_list_st(&(p_icf_create_ext_xconf_req_st->p_pref_media_profile), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_add_xconf_party_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_add_xconf_party_req_st    *p_icf_add_xconf_party_req_st = ICF_NULL;
    
    p_icf_add_xconf_party_req_st    = (icf_add_xconf_party_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_add_xconf_party_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_add_xconf_party_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_add_xconf_party_req_st->bitmask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
     
    
    if(p_icf_add_xconf_party_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_add_xconf_party_req_st-> header_list),p_src + (*p_length), &offset);

        *p_length       += offset;
    }
    if(p_icf_add_xconf_party_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_add_xconf_party_req_st-> tag_list),p_src + (*p_length), &offset);

        *p_length       += offset;
    }
    
    icf_unpack_U16((icf_uint8_t*)&(p_icf_add_xconf_party_req_st-> add_party_id),p_src + (*p_length),  &offset);
    *p_length       += offset;
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_delete_xconf_party_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_delete_xconf_party_req_st    *p_icf_delete_xconf_party_req_st = ICF_NULL;

    p_icf_delete_xconf_party_req_st    = (icf_delete_xconf_party_req_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_delete_xconf_party_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_delete_xconf_party_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_delete_xconf_party_req_st->bitmask),p_src + (*p_length), &offset);
    *p_length    += offset;
  
    if(p_icf_delete_xconf_party_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_delete_xconf_party_req_st-> header_list),p_src + (*p_length), &offset);

        *p_length       += offset;
    }
    if(p_icf_delete_xconf_party_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_delete_xconf_party_req_st-> tag_list),p_src + (*p_length), &offset);

        *p_length       += offset;
    }
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_delete_xconf_party_req_st
           ->del_party_uri), p_src + (*p_length), &offset);

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_release_xconf_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_release_xconf_req_st    *p_icf_release_xconf_req_st = ICF_NULL;

    p_icf_release_xconf_req_st    = (icf_release_xconf_req_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_release_xconf_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_release_xconf_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_release_xconf_req_st->bitmask),p_src +      (*p_length), &offset);
    *p_length    += offset;

    if(p_icf_release_xconf_req_st->bitmask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_release_xconf_req_st-> header_list),p_src + (*p_length), &offset);

        *p_length       += offset;
    }
    
    if(p_icf_release_xconf_req_st->bitmask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_release_xconf_req_st-> tag_list),p_src + (*p_length), &offset);

        *p_length       += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_create_xconf_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_create_ext_xconf_resp_st    *p_icf_create_ext_xconf_resp_st = ICF_NULL;

    p_icf_create_ext_xconf_resp_st    = (icf_create_ext_xconf_resp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_create_ext_xconf_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_create_ext_xconf_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_create_ext_xconf_resp_st->bitmask),p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_create_ext_xconf_resp_st->result), p_src + (*p_length),&offset);
	
    *p_length	+= offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_create_ext_xconf_resp_st->error_cause), p_src + (*p_length),&offset);
	
    *p_length	+= offset;

     return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_add_xconf_party_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_add_xconf_party_resp_st    *p_icf_add_xconf_party_resp_st = ICF_NULL;

    p_icf_add_xconf_party_resp_st    = (icf_add_xconf_party_resp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_add_xconf_party_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_add_xconf_party_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_add_xconf_party_resp_st->result),p_src + (*p_length), &offset);
	
    *p_length	+= offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_add_xconf_party_resp_st->error_cause),p_src + (*p_length),  &offset);
	
    *p_length	+= offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_delete_xconf_party_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_delete_xconf_party_resp_st    *p_icf_delete_xconf_party_resp_st = ICF_NULL;

    p_icf_delete_xconf_party_resp_st    = (icf_delete_xconf_party_resp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_delete_xconf_party_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_delete_xconf_party_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_delete_xconf_party_resp_st->result),p_src + (*p_length), &offset);
	
    *p_length	+= offset;

    icf_unpack_U16(
            (icf_uint8_t*)&(p_icf_delete_xconf_party_resp_st->error_code),p_src + (*p_length), &offset);
	
    *p_length	+= offset;

     icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_delete_xconf_party_resp_st->deleted_party_uri), p_src + (*p_length),&offset);
     *p_length	+= offset;

     return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_release_xconf_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_release_xconf_resp_st    *p_icf_xconf_release_resp_st = ICF_NULL;

    p_icf_xconf_release_resp_st    = (icf_release_xconf_resp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_xconf_release_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_xconf_release_resp_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_xconf_release_resp_st->result),p_src + (       *p_length), &offset);

    *p_length   += offset;
    icf_unpack_U16(
            (icf_uint8_t*)&(p_icf_xconf_release_resp_st->error_code), p_src + (*      p_length), &offset);
       
 

    *p_length   += offset;

     return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_release_xconf_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_xconf_release_ind_st    *p_icf_xconf_release_ind_st = ICF_NULL;

    p_icf_xconf_release_ind_st    = (icf_xconf_release_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_xconf_release_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_xconf_release_ind_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_xconf_release_ind_st->call_clear_reason),
            p_src + (*p_length), &offset);
	
    *p_length	+= offset;

     return ICF_SUCCESS;
}





#endif

icf_uint32_t
icf_unpack_icf_call_redirection_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_redirection_ind_st    *p_icf_call_redirection_ind_st = ICF_NULL;
    
    p_icf_call_redirection_ind_st    = (icf_call_redirection_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_call_redirection_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_redirection_ind_st\n"));

	icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_call_redirection_ind_st->redirected_party_addr), p_src + (*p_length), &offset);	
	*p_length    += offset;
    
	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_call_redirection_ind_st->redirected_party_name), p_src + (*p_length), &offset);	
	*p_length    += offset;

	return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_conference_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_conference_req_st    *p_icf_conference_req_st = ICF_NULL;
    
    p_icf_conference_req_st    = (icf_conference_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_conference_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_conference_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_conference_req_st->bit_mask), 
                   p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_conference_req_st->merge_call_id), 
        p_src + (*p_length), &offset);	
    *p_length    += offset;

    if(p_icf_conference_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st(
           (icf_uint8_t*)&(p_icf_conference_req_st->header_list), 
           p_src + (*p_length), &offset);

        *p_length       += offset;
    }

    if(p_icf_conference_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st(
           (icf_uint8_t*)&(p_icf_conference_req_st->tag_list), 
           p_src + (*p_length), &offset);

        *p_length       += offset;
    }

    return(ICF_SUCCESS);

}






icf_uint32_t
icf_unpack_icf_call_merged_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_merged_ind_st    *p_icf_call_merged_ind_st = ICF_NULL;
    
    p_icf_call_merged_ind_st    = (icf_call_merged_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_call_merged_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_merged_ind_st\n"));

	icf_unpack_U16((icf_uint8_t*)&(p_icf_call_merged_ind_st->merge_call_id), p_src + (*p_length), &offset);	
	*p_length    += offset;
  
	return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_party_replaced_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_party_replaced_ind_st    *p_icf_party_replaced_ind_st = ICF_NULL;
    
    p_icf_party_replaced_ind_st    = (icf_party_replaced_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_party_replaced_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_party_replaced_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_party_replaced_ind_st->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;

    
    icf_unpack_U16((icf_uint8_t*)&(p_icf_party_replaced_ind_st->replacing_call_id), p_src + (*p_length), &offset);	
    *p_length    += offset;
    
    if (ICF_REPLACING_PARTY_ADDR_PRESENT & p_icf_party_replaced_ind_st->bit_mask)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_party_replaced_ind_st->replacing_party_addr), p_src + (*p_length), &offset);	
        *p_length    += offset;
    }

    if (ICF_PARTY_REPLACED_NAME & p_icf_party_replaced_ind_st->bit_mask)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_party_replaced_ind_st->replacing_party_name), p_src + (*p_length), &offset);	
        *p_length    += offset;
    }

    if(p_icf_party_replaced_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_party_replaced_ind_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
	if(p_icf_party_replaced_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_party_replaced_ind_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
    if(p_icf_party_replaced_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_party_replaced_ind_st->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
   	
	return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_config_registration_params_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_config_registration_params_st    *p_icf_config_registration_params_st = ICF_NULL;
    
    p_icf_config_registration_params_st    = (icf_config_registration_params_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_registration_params_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_registration_params_st\n"));
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_config_registration_params_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_registration_params_st->action), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_registration_params_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_RG_ADDR)
    {
        icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_config_registration_params_st->registrar_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_RG_MODE)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_config_registration_params_st->reg_mode), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_RG_DURATION)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_config_registration_params_st->registration_duration), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_AUTH_KEY)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_config_registration_params_st->authentication_key), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_config_registration_params_st->bit_mask & ICF_CONFIG_RG_PRM_AUTH_PASSWORD)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_config_registration_params_st->authentication_password), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_configure_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_configure_cfm_st    *p_icf_configure_cfm_st = ICF_NULL;
    
    p_icf_configure_cfm_st    = (icf_configure_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_configure_cfm_st */
    ICF_CLIB_TRACE(("Unpacking icf_configure_cfm_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_configure_cfm_st->api_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    /* SPR 15997 : Line id in configure confirm API */ 
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_configure_cfm_st->line_id), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_configure_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U16((icf_uint8_t*)&(p_icf_configure_cfm_st->error_cause), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_register_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_register_req_st    *p_icf_register_req_st = ICF_NULL;
    
    p_icf_register_req_st    = (icf_register_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_register_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_register_req_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_register_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_register_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_TO_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_register_req_st->to), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_EXPIRY_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_register_req_st->expiry), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_register_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_register_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_register_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    /* CSR_1-8389294 changes start */
    if(p_icf_register_req_st->bit_mask & ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_register_req_st->dereg_specific_contact), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    /* CSR_1-8389294 changes end */


    return ICF_SUCCESS;

}
icf_uint32_t
icf_unpack_icf_header_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_st    *p_icf_header_st = ICF_NULL;
    
    p_icf_header_st    = (icf_header_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_header_st */
    ICF_CLIB_TRACE(("Unpacking icf_header_st\n"));
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_header_st->hdr_name), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_large_string_st((icf_uint8_t*)&(p_icf_header_st->hdr_value), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    

    /* RN -- Manually added code*/
    if (p_icf_header_st->next) {
        icf_unpack_icf_header_st((icf_uint8_t*)p_icf_header_st->next, p_src + (*p_length), &offset);
        *p_length += offset;
    }


    return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_header_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_header_list_st    *p_icf_header_list_st = ICF_NULL;
    
    p_icf_header_list_st    = (icf_header_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_header_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_header_list_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_header_list_st->count), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    

    /* RN -- Manually added code*/
    if (!(p_icf_header_list_st->count))
        return ICF_SUCCESS;

    p_icf_header_list_st->hdr = ICF_NULL;

    if (icf_make_hdr_node(&(p_icf_header_list_st->hdr), 
        (icf_uint8_t)p_icf_header_list_st->count) == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_header_list_st() Aborted\n"));
        return ICF_FAILURE;
    }


    icf_unpack_icf_header_st((icf_uint8_t*)(p_icf_header_list_st->hdr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_tag_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_tag_st    *p_icf_tag_st = ICF_NULL;

	p_icf_tag_st    = (icf_tag_st*)p_dest;

	/* Initialise length to 0 */
	*p_length    = 0;

	/* This function is for unpacking icf_tag_st */
	ICF_CLIB_TRACE(("Unpacking icf_tag_st\n"));

	icf_unpack_U8((icf_uint8_t*)&(p_icf_tag_st->tag_type),p_src + (*p_length),&offset);

	*p_length += offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_tag_st->hdr_name), p_src + (*p_length), &offset);

	*p_length    += offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_tag_st->tag_value), p_src + (*p_length), &offset);

	*p_length    += offset;

	/* RN -- Manually added code*/
	if (p_icf_tag_st->next) {
		icf_unpack_icf_tag_st((icf_uint8_t*)p_icf_tag_st->next, p_src + (*p_length), &offset);
		*p_length += offset;
	}

	return ICF_SUCCESS;
}




icf_uint32_t
icf_unpack_icf_tag_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_tag_list_st    *p_icf_tag_list_st = ICF_NULL;
    
    p_icf_tag_list_st    = (icf_tag_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_tag_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_tag_list_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_tag_list_st->count), p_src + (*p_length), &offset);
    
    *p_length    += offset;



    /* RN -- Manually added code*/
    if (!(p_icf_tag_list_st->count))
        return ICF_SUCCESS;

    p_icf_tag_list_st->tag = ICF_NULL;

    if (icf_make_tag_node(&(p_icf_tag_list_st->tag), 
        (icf_uint8_t)p_icf_tag_list_st->count) == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_tag_list_st() Aborted\n"));
        return ICF_FAILURE;
    }
    

        
    icf_unpack_icf_tag_st((icf_uint8_t*)(p_icf_tag_list_st->tag), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}




icf_uint32_t
icf_unpack_icf_msg_body_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_msg_body_st    *p_icf_msg_body_st = ICF_NULL;
    
    p_icf_msg_body_st    = (icf_msg_body_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_msg_body_st */
    ICF_CLIB_TRACE(("Unpacking icf_msg_body_st\n"));
    
    /*Message body header changes Start*/
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_msg_body_st->content_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* Unpack the body header list, if any in the message body list.
     */
    icf_unpack_icf_header_list_st((icf_uint8_t *)&(p_icf_msg_body_st->\
			    msg_body_hdr_list), p_src + (*p_length) , &offset);
    *p_length += offset;
    /*Message body header changes Ends*/
    /* Unpack the message body length
     */
    icf_unpack_U32((icf_uint8_t*)&(p_icf_msg_body_st->length), p_src +\
		    (*p_length),&offset);
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    /* Unpack the NULL Character*/
    while(count < p_icf_msg_body_st->length + 1)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_msg_body_st->value[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }

    /* RN -- Manually added code*/
    if (p_icf_msg_body_st->next) {
        icf_unpack_icf_msg_body_st((icf_uint8_t*)p_icf_msg_body_st->next, p_src + (*p_length), &offset);
        *p_length += offset;
    }
   	
    return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_msg_body_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_msg_body_list_st    *p_icf_msg_body_list_st = ICF_NULL;
    
    p_icf_msg_body_list_st    = (icf_msg_body_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_msg_body_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_msg_body_list_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_msg_body_list_st->count), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    /*Message body header changes Start*/
    /* Unpack the content-type in the message body list structure
     */
     icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_msg_body_list_st->\
			     content_type), p_src + (*p_length), &offset);
     *p_length    += offset;
     /*Message body header changes Ends*/

    /* RN -- Manually added code*/
    if (!(p_icf_msg_body_list_st->count))
        return ICF_SUCCESS;

    p_icf_msg_body_list_st->body = ICF_NULL;

    if (icf_make_msg_body_node(&(p_icf_msg_body_list_st->body), 
        (icf_uint8_t)p_icf_msg_body_list_st->count) == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_msg_body_list_st() Aborted\n"));
        return ICF_FAILURE;
    }

/*    icf_make_msg_body_node(&(p_icf_msg_body_list_st->body), (icf_uint8_t)p_icf_msg_body_list_st->count);*/

    icf_unpack_icf_msg_body_st((icf_uint8_t*)(p_icf_msg_body_list_st->body), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}




icf_uint32_t
icf_unpack_icf_create_call_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_create_call_req_st    *p_icf_create_call_req_st = ICF_NULL;
    
    p_icf_create_call_req_st    = (icf_create_call_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_create_call_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_create_call_req_st\n"));

#ifdef ICF_LOAD_STAT
    g_create_call_req++;
#endif

    icf_unpack_U32((icf_uint8_t*)&(p_icf_create_call_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_create_call_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

#ifdef ICF_REL_OLD
    icf_unpack_U8((icf_uint8_t*)&(p_icf_create_call_req_st->call_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;
#endif
    
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_create_call_req_st->called_party_addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_create_call_req_st->bit_mask & ICF_CALLED_PARTY_NAME_PRESENT)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_create_call_req_st->called_party_name), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_TOGGLE_PRES_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_create_call_req_st->toggle_presentation_indicator), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_QOS_MARK_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_create_call_req_st->qos_mark), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_create_call_req_st->pref_identity), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if (p_icf_create_call_req_st->bit_mask & ICF_CALLING_PARTY_ADDR)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_create_call_req_st->calling_party_address), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    
    if(p_icf_create_call_req_st->bit_mask & ICF_CALLING_PARTY_NAME)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_create_call_req_st->calling_party_name), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_create_call_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_create_call_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_create_call_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_create_call_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_create_call_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_create_call_req_st->stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
#endif

	if(p_icf_create_call_req_st->bit_mask & ICF_PREF_MEDIA_PROFILE_PRESENT)
    {
        icf_unpack_icf_stream_capab_list_st(&(p_icf_create_call_req_st->p_pref_media_profile), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
	else
	{
		p_icf_create_call_req_st->p_pref_media_profile = ICF_NULL;
	}

    if(p_icf_create_call_req_st->bit_mask & ICF_PRIVACY_HDR_FLAG_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_create_call_req_st->privacy_hdr_all_req), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_create_call_req_st->bit_mask & ICF_RESOURCE_RESERV_FLAG_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_create_call_req_st->resource_reservation_reqd), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}



#ifdef ICF_REL_OLD
icf_uint32_t
icf_unpack_icf_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_stream_st    *p_icf_stream_st = ICF_NULL;
    
    p_icf_stream_st    = (icf_stream_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stream_st */
    ICF_CLIB_TRACE(("Unpacking icf_stream_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_st->type), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_stream_st->codec), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_st->att_count), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

    count = 0;
    while(count < p_icf_stream_st->att_count)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_stream_st->attributes[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}
#endif


icf_uint32_t
icf_unpack_icf_stream_id_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_stream_id_list_st    *p_icf_stream_id_list_st = ICF_NULL;
    
    p_icf_stream_id_list_st    = (icf_stream_id_list_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stream_id_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_stream_id_list_st\n"));
    icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_id_list_st->count), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    /* This piece of code unpacks variable length octet string */

   while(count < p_icf_stream_id_list_st->count)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_id_list_st->stream_id[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_stream_list_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_stream_list_st *p_icf_stream_list_st = ICF_NULL;

	p_icf_stream_list_st = (icf_stream_list_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stream_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_stream_list_st\n"));

	icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_list_st->stream_id), p_src + (*p_length), &offset);

	*p_length += offset;
	
	icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_list_st->stream_type), p_src + (*p_length), &offset);
		
	*p_length += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_local_user_alerted_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_local_user_alerted_req_st    *p_icf_local_user_alerted_req_st = ICF_NULL;
    
    p_icf_local_user_alerted_req_st    = (icf_local_user_alerted_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_local_user_alerted_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_local_user_alerted_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->early_media_reqd), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->inband_info_status), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->privacy_ind), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_local_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_ALERT_TIMER_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->alert_timer_duration), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
#endif

/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_local_user_alerted_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_capab_list_st(&(p_icf_local_user_alerted_req_st->p_stream_list), p_src + (*p_length), &offset);
	
		*p_length	+= offset;
	}

	if(p_icf_local_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_DELETED_STREAM_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;

		while(count<p_icf_local_user_alerted_req_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->deleted_media_streams[count]), p_src + (*p_length), &offset);
        
		    *p_length    += offset;

			count++;
		}
    }

    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED)
    {
      icf_unpack_U8((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->early_mode), p_src + (*p_length),
                                             &offset);
      *p_length    += offset;
    }
   
    if(p_icf_local_user_alerted_req_st->bit_mask & ICF_USER_ALERTED_MEDIA_MODE_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_local_user_alerted_req_st->media_mode), p_src\
                + (*p_length), &offset);
                
        *p_length    += offset;
    }
   
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_call_forwarded_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_forwarded_ind_st    *p_icf_call_forwarded_ind_st = ICF_NULL;
    
    p_icf_call_forwarded_ind_st    = (icf_call_forwarded_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_call_forwarded_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_forwarded_ind_st\n"));
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_call_forwarded_ind_st->remote_party_addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_call_forwarded_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_call_forwarded_ind_st->bit_mask & ICF_REMOTE_PARTY_NAME)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_call_forwarded_ind_st->remote_party_name), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	icf_unpack_U8((icf_uint8_t*)&(p_icf_call_forwarded_ind_st->call_forwarded_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_terminate_call_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_terminate_call_req_st    *p_icf_terminate_call_req_st = ICF_NULL;
    
    p_icf_terminate_call_req_st    = (icf_terminate_call_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_terminate_call_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_terminate_call_req_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_terminate_call_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_terminate_call_req_st->call_clear_reason), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_terminate_call_req_st->bit_mask & ICF_LINE_ID_PRESENT)
    {
        icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_terminate_call_req_st->line_id), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

 if(p_icf_terminate_call_req_st->bit_mask & ICF_CALL_CLEAR_RESPONSE_CODE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_terminate_call_req_st->response_code), p_src + (*p_length), &offset);

        *p_length    += offset;

    }
    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_connect_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_connect_req_st    *p_icf_connect_req_st = ICF_NULL;
    
    p_icf_connect_req_st    = (icf_connect_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_connect_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_connect_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_connect_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_connect_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_connect_req_st->privacy_ind), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_connect_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_connect_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_connect_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_connect_req_st->stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
#endif

/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_capab_list_st(&(p_icf_connect_req_st->p_stream_list), p_src + (*p_length), &offset);
	
		*p_length	+= offset;
	}

	if(p_icf_connect_req_st->bit_mask & ICF_CONNECT_DELETED_STREAM_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_connect_req_st->count), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		while(count<p_icf_connect_req_st->count)
		{		
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_connect_req_st->deleted_media_streams[count]), p_src + (*p_length), &offset);

			*p_length    += offset;

			count++;
		}
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_subscribe_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_subscribe_req_st    *p_icf_subscribe_req_st = ICF_NULL;
    
    p_icf_subscribe_req_st    = (icf_subscribe_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_subscribe_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_subscribe_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_subscribe_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_subscribe_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_subscribe_req_st->bit_mask & ICF_SUB_REQ_DEST_INFO_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_subscribe_req_st->destination_info), p_src + (*p_length), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_SUB_REQ_EVENT_PRESENT)
    {
    
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_subscribe_req_st->event_pkg_name), p_src + (*p_length), &offset);
    
        *p_length    += offset;
    }
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_subscribe_req_st->duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_subscribe_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_subscribe_req_st->pref_identity), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_subscribe_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_subscribe_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_subscribe_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_publish_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_publish_req_st    *p_icf_publish_req_st = ICF_NULL;
    
    p_icf_publish_req_st    = (icf_publish_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_publish_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_publish_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_publish_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_publish_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_publish_req_st->destination_info), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_publish_req_st->event_pkg_name), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_publish_req_st->duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_publish_req_st->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_publish_req_st->user_address), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    
    if(p_icf_publish_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_publish_req_st->pref_identity), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_publish_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_publish_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_publish_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_publish_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_publish_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_publish_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_message_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_message_req_st    *p_icf_message_req_st = ICF_NULL;
    
    p_icf_message_req_st    = (icf_message_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_message_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_message_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_message_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_message_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_message_req_st->destination_info), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_message_req_st->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_message_req_st->user_address), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    
    if(p_icf_message_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_message_req_st->pref_identity), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_message_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_message_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_message_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_refer_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_refer_req_st    *p_icf_refer_req_st = ICF_NULL;
    
    p_icf_refer_req_st    = (icf_refer_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_refer_req_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_refer_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_refer_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_refer_req_st->bit_mask & ICF_REF_REQ_DEST_INFO_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_refer_req_st->destination_info), p_src + (*p_length), &offset);
    
        *p_length    += offset;
    }
    
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_refer_req_st->refer_to), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_refer_req_st->bit_mask & ICF_REF_REQ_SUBS_REQD_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_refer_req_st->subscription_required), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_refer_req_st->pref_identity), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_refer_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_refer_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_refer_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_register_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_register_cfm_st    *p_icf_register_cfm_st = ICF_NULL;
    
    p_icf_register_cfm_st    = (icf_register_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;


    /* This function is for unpacking icf_register_cfm_st */
    ICF_CLIB_TRACE(("Unpacking icf_register_cfm_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_register_cfm_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_register_cfm_st->request_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_register_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_register_cfm_st->error_cause), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    
    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_ASSO_URI_PRESENT)
    {
        icf_unpack_icf_pub_uri_list_st(&(p_icf_register_cfm_st->p_uri_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_register_cfm_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;


    if(p_icf_register_cfm_st->bit_mask & ICF_REG_CFM_TO_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_register_cfm_st->to), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_register_cfm_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_register_cfm_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }


    return ICF_SUCCESS;
}

/* RN*/
icf_uint32_t
icf_unpack_icf_register_status_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_register_status_ind_st    *p_icf_register_status_ind_st = ICF_NULL;
    
    p_icf_register_status_ind_st    = (icf_register_status_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;


    /* This function is for unpacking icf_register_status_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_register_status_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_register_status_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_register_status_ind_st->request_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_register_status_ind_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_register_status_ind_st->error_cause), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    
    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_CFM_ASSO_URI_PRESENT)
    {
        icf_unpack_icf_pub_uri_list_st(&(p_icf_register_status_ind_st->p_uri_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_register_status_ind_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;


    if(p_icf_register_status_ind_st->bit_mask & ICF_REG_CFM_TO_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_register_status_ind_st->to), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_register_status_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_register_status_ind_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return ICF_SUCCESS;
}




icf_uint32_t
icf_unpack_icf_incoming_call_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_incoming_call_ind_st    *p_icf_incoming_call_ind_st = ICF_NULL;
    
    p_icf_incoming_call_ind_st    = (icf_incoming_call_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_incoming_call_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_incoming_call_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_incoming_call_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_incoming_call_ind_st->bit_mask & ICF_LINE_ID_PRESENT)
    {
        icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_incoming_call_ind_st->line_id), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_incoming_call_ind_st->bit_mask & ICF_TYPE_OF_CALL_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_incoming_call_ind_st->call_type), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALL_WAITING_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_incoming_call_ind_st->call_waiting_ind), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLING_ADD_ASRTD_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_incoming_call_ind_st->calling_party_addr_asserted), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLING_PARTY_ADD_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->calling_party_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLING_PARTY_NAME_PRESENT)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->calling_party_name), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_CALLED_PARTY_ADD_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->called_party_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_TRANSF_PARTY_ADD_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->transferring_party_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_TRANSF_PARTY_NAME_PRESENT)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->transferring_party_name), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_incoming_call_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_incoming_call_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
#endif

	icf_unpack_icf_stream_capab_list_st(&(p_icf_incoming_call_ind_st->p_accepted_stream_list), p_src + (*p_length), &offset);
        
    *p_length    += offset;
    	
	if(p_icf_incoming_call_ind_st->bit_mask & ICF_REJECTED_MEDIA_STREAM_PRESENT)
    {
		icf_unpack_U8((icf_uint8_t*)&(p_icf_incoming_call_ind_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;

		while(count<p_icf_incoming_call_ind_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_incoming_call_ind_st->deleted_stream_list[count]), p_src + (*p_length), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

    icf_unpack_U16((icf_uint8_t*)&(p_icf_incoming_call_ind_st->call_id_to_be_joined),p_src + (*p_length),  &offset);
    *p_length       += offset;


    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_remote_user_alerted_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_remote_user_alerted_ind_st    *p_icf_remote_user_alerted_ind_st = ICF_NULL;
    
    p_icf_remote_user_alerted_ind_st    = (icf_remote_user_alerted_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_remote_user_alerted_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_remote_user_alerted_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->inband_info_status), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
#endif

    icf_unpack_icf_stream_capab_list_st(&(p_icf_remote_user_alerted_ind_st->p_accepted_stream_list), p_src + (*p_length), &offset);
        
    *p_length    += offset;
  
	if(p_icf_remote_user_alerted_ind_st->bit_mask & ICF_REM_USER_ALRTED_DELETED_STREAM_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;
		
		while(count<p_icf_remote_user_alerted_ind_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_remote_user_alerted_ind_st->deleted_media_streams[count]), p_src + (*p_length), &offset);
        
			*p_length    += offset;

			count++;
		}
    }


    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_terminate_call_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_terminate_call_ind_st    *p_icf_terminate_call_ind_st = ICF_NULL;
    
    p_icf_terminate_call_ind_st    = (icf_terminate_call_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_terminate_call_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_terminate_call_ind_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_terminate_call_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_terminate_call_ind_st->call_clear_reason), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_terminate_call_ind_st->bit_mask & ICF_ADDNL_INFO_PRESENT)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_terminate_call_ind_st->additional_info), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    if(p_icf_terminate_call_ind_st->bit_mask & ICF_REMOTE_CALL_CLEAR_RESPONSE_CODE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_terminate_call_ind_st->response_code), p_src + (*p_length), &offset);
   
        *p_length    += offset;
    }

    if(p_icf_terminate_call_ind_st->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_unpack_U32(
         (icf_uint8_t*)&(p_icf_terminate_call_ind_st->retry_after_duration), 
                                              p_src + (*p_length), &offset);

        *p_length    += offset;
    }

     return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_connect_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_connect_ind_st    *p_icf_connect_ind_st = ICF_NULL;
    
    p_icf_connect_ind_st    = (icf_connect_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_connect_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_connect_ind_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_connect_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_connect_ind_st->bit_mask & ICF_ASSERTED_ID_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_connect_ind_st->asserted_identity), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_connect_ind_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_connect_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_connect_ind_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

#ifdef ICF_REL_OLD
    if(p_icf_connect_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_connect_ind_st->stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
#endif

    icf_unpack_icf_call_modify_media_data_list_st(&(p_icf_connect_ind_st->p_accepted_stream_list), p_src + (*p_length), &offset);
        
    *p_length    += offset;
 
	if(p_icf_connect_ind_st->bit_mask & ICF_CONNECT_IND_DELETED_STREAM_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_connect_ind_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;
		
		while(count<p_icf_connect_ind_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_connect_ind_st->deleted_media_streams[count]), p_src + (*p_length), &offset);
        
	        *p_length    += offset;
		
			count++;
		}
    }

	icf_unpack_U8((icf_uint8_t*)&(p_icf_connect_ind_st->inband_dtmf_supported), p_src + (*p_length), &offset);

	*p_length    += offset;

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_subscribe_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_subscription_status_ind_st    *p_icf_subscribe_resp_st = ICF_NULL;
    
    p_icf_subscribe_resp_st    = (icf_subscription_status_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_subscribe_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_subscribe_resp_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_subscribe_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_subscribe_resp_st->error_code), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_subscribe_resp_st->status), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_subscribe_resp_st->duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_subscribe_resp_st->bit_mask & ICF_SUBS_ST_IND_RESP_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_subscribe_resp_st->response_code), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_subscribe_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_subscribe_resp_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	/* Fix for CSR 1-8420460: SPR 20343 */
	/* Event package name for which subscribe request was sent */
    icf_unpack_icf_string_st(
		(icf_uint8_t *)&(p_icf_subscribe_resp_st->event_pkg_name),
		p_src + (*p_length),
		&offset);
    
    *p_length    += offset;
    
    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_notify_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_notify_ind_st    *p_icf_notify_ind_st = ICF_NULL;
    
    p_icf_notify_ind_st    = (icf_notify_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_notify_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_notify_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_notify_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
	icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_notify_ind_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_notify_ind_st->event_pkg_name), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
/*NOTIFY_OUT_OF_DIALOG_START*/
	icf_unpack_U8((icf_uint8_t*)&(p_icf_notify_ind_st->subs_state), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_notify_ind_st->reason_code), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_notify_ind_st->bit_mask & ICF_SUBS_DURATION_PRESENT)
    {
		icf_unpack_U32((icf_uint8_t*)&(p_icf_notify_ind_st->duration), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_notify_ind_st->bit_mask & ICF_NOTIFY_IND_SENDER_PRESENT)
    {
		icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_notify_ind_st->sender), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_notify_ind_st->bit_mask & ICF_NOTIFY_IND_USER_ADDR_PRESENT)
    {
		icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_notify_ind_st->user_address), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
/*NOTIFY_OUT_OF_DIALOG_END*/

/*    icf_unpack_U32((icf_uint8_t*)&(p_icf_notify_ind_st->error_code), p_src + (*p_length), &offset);*/
    
/*    *p_length    += offset;*/
    
/*    icf_unpack_U32((icf_uint8_t*)&(p_icf_notify_ind_st->duration), p_src + (*p_length), &offset);*/
    
/*    *p_length    += offset;*/
    

    if(p_icf_notify_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_notify_ind_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_notify_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_notify_ind_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_publish_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_publish_resp_st    *p_icf_publish_resp_st = ICF_NULL;
    
    p_icf_publish_resp_st    = (icf_publish_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_publish_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_publish_resp_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_publish_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_publish_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_publish_resp_st->bit_mask & ICF_PUB_RESP_ERROR_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_publish_resp_st->error_code), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_publish_resp_st->duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_publish_resp_st->bit_mask & ICF_PUB_RESP_RESP_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_publish_resp_st->response_code), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_publish_resp_st->entity_tag), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_publish_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_publish_resp_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_message_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_message_resp_st    *p_icf_message_resp_st = ICF_NULL;
    
    p_icf_message_resp_st    = (icf_message_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_message_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_message_resp_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_message_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_message_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_message_resp_st->bit_mask & ICF_MSG_RESP_ERROR_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_message_resp_st->error_code), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
    
    if(p_icf_message_resp_st->bit_mask & ICF_MSG_RESP_RESP_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_message_resp_st->response_code), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_message_resp_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_message_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_message_ind_st    *p_icf_message_ind_st = ICF_NULL;
    
    p_icf_message_ind_st    = (icf_message_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_message_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_message_ind_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_message_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_message_ind_st->sender), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_message_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_message_ind_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_message_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_message_ind_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}
icf_uint32_t
icf_unpack_icf_refer_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_refer_resp_st    *p_icf_refer_resp_st = ICF_NULL;
    
    p_icf_refer_resp_st    = (icf_refer_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_refer_resp_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_refer_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_refer_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_refer_resp_st->bit_mask & ICF_REF_RESP_RESP_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_refer_resp_st->response_code), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_refer_resp_st->bit_mask & ICF_REF_RESP_ERROR_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_refer_resp_st->error_code), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_refer_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_refer_resp_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_nw_buff_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_trace_nw_buff_ind_st    *p_icf_nw_buff_ind_st = ICF_NULL;
    
    p_icf_nw_buff_ind_st    = (icf_trace_nw_buff_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_trace_nw_buff_ind_st\n"));
    icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_nw_buff_ind_st->nw_address), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_nw_buff_ind_st->nw_buffer.len), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    p_icf_nw_buff_ind_st->nw_buffer.p_buff = p_src + (*p_length);

    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_api_header_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{

    icf_uint32_t        offset = 0;

    icf_api_header_st    *p_icf_api_header_st = ICF_NULL;
    
    p_icf_api_header_st    = (icf_api_header_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_api_header_st */
    ICF_CLIB_TRACE(("Unpacking icf_api_header_st\n"));

    icf_unpack_U16((icf_uint8_t *)&(p_icf_api_header_st->api_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t *)&(p_icf_api_header_st->version), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t *)&(p_icf_api_header_st->source_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t *)&(p_icf_api_header_st->destination_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U16((icf_uint8_t *)&(p_icf_api_header_st->call_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U32((icf_uint8_t *)&(p_icf_api_header_st->app_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U16((icf_uint8_t *)&(p_icf_api_header_st->api_length), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_unknown_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{


    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    
    while(count < *p_length)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_dest[count]), (icf_uint8_t*)&(p_src[count]), &offset);

        count++;
    }


    return ICF_SUCCESS;
}





/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/



icf_uint32_t
icf_unpack_U8
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{

    *p_length    = sizeof(icf_uint8_t);

    *p_dest = *p_src;

#if ICF_CLIB_TRACE_LEVEL == 2    
    ICF_CLIB_TRACE(("Value of icf_uint8_t = %d\n", *((icf_uint8_t *)p_dest)));
#endif
    
    return ICF_SUCCESS;

}


icf_uint32_t
icf_unpack_U16
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    /* Unpacking U16 */
     
    *(icf_uint16_t*)p_dest    = 0;
    
    *(icf_uint16_t*)p_dest    |= (icf_uint16_t)p_src[0] << 8;
    *(icf_uint16_t*)p_dest    |= (icf_uint16_t)p_src[1];
    
    *p_length    = sizeof(icf_uint16_t);

#if ICF_CLIB_TRACE_LEVEL == 2
    ICF_CLIB_TRACE(("Value of icf_uint16_t = %d\n", *((icf_uint16_t*)p_dest)));
#endif
    
    return ICF_SUCCESS;

}


icf_uint32_t
icf_unpack_U32
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    /* Unpacking icf_uint32_t */
     
    *(icf_uint32_t*)p_dest    = 0;

    *(icf_uint32_t*)p_dest    |= (icf_uint32_t)p_src[0] << 24;
    *(icf_uint32_t*)p_dest    |= (icf_uint32_t)p_src[1] << 16;
    *(icf_uint32_t*)p_dest    |= (icf_uint32_t)p_src[2] << 8;
    *(icf_uint32_t*)p_dest    |= (icf_uint32_t)p_src[3];
    
    *p_length    = sizeof(icf_uint32_t);

#if ICF_CLIB_TRACE_LEVEL == 2
    ICF_CLIB_TRACE(("Value of icf_uint32_t = %d\n", *((icf_uint32_t*)p_dest)));
#endif
    
    return ICF_SUCCESS;

}

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Starts here *************/
/*****************************************************************************/


icf_uint32_t
icf_unpack_icf_stretchable_hdr_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;

	icf_stretchable_hdr_st	*p_icf_stretchable_hdr_st = ICF_NULL;
	
	p_icf_stretchable_hdr_st	= (icf_stretchable_hdr_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function packs icf_transport_address_st */
	ICF_CLIB_TRACE(("Packing icf_stretchable_hdr_st\n"));

	/*  icf_uint8_t   hdr_len;
		icf_uint8_t  *p_hdr; */

	icf_unpack_U8((icf_uint8_t *)&(p_icf_stretchable_hdr_st->hdr_len), p_src + (*p_length), &offset);
	*p_length	+= offset;

    p_icf_stretchable_hdr_st->p_hdr = ICF_NULL;
	/* This piece of code unpacks variable length octet string */
  /* non-required comparison - comparison is always true due to limited range of data type */
 /*   if (0 < p_icf_stretchable_hdr_st->hdr_len && p_icf_stretchable_hdr_st->hdr_len < ICF_CLIB_MAX_API_LENGTH) {
*/
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: Getting memory for icf_stretchable_hdr_st\n"));
        p_icf_stretchable_hdr_st->p_hdr = ICF_CLIB_MEMGET(p_icf_stretchable_hdr_st->hdr_len);

        if (!p_icf_stretchable_hdr_st->p_hdr) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: Failed In Allocating Memory for p_icf_stretchable_hdr_st->p_hdr\n"));
            return ICF_FAILURE;
        }
  /*  } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_stretchable_hdr_st - Invalid Structure Length!! Quitting! (%d) Bytes\n",p_icf_stretchable_hdr_st->hdr_len));
        return ICF_FAILURE;
    }
*/
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_stretchable_hdr_st - Unpacking Structure of Length (%d)\n", p_icf_stretchable_hdr_st->hdr_len));

	count = 0;
	while(count < p_icf_stretchable_hdr_st->hdr_len && p_icf_stretchable_hdr_st->p_hdr)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_stretchable_hdr_st->p_hdr[count]), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
		
		count++;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_set_mgmt_plane_auth_param_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_set_mgmt_plane_auth_param_req_st	*p_icf_set_mgmt_plane_auth_param_req_st = ICF_NULL;
	p_icf_set_mgmt_plane_auth_param_req_st	= (icf_set_mgmt_plane_auth_param_req_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_unpack_icf_set_mgmt_plane_auth_param_req_st\n"));

	/* #define ICF_MGMT_PLANE_AUTH_DATA_PRESENT 0x01
	icf_uint16_t				bitmask;
	icf_boolean_t				remove_flag;
	icf_uint8_t					server_type;
	icf_transport_address_st	server_addr;
	icf_uint8_t					auth_method;
	icf_string_st				auth_username;
	icf_string_st				auth_password; */

	icf_unpack_U16((icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->bitmask), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->remove_flag), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->server_type), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_set_mgmt_plane_auth_param_req_st->server_addr), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t *)&(p_icf_set_mgmt_plane_auth_param_req_st->auth_method), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_mgmt_plane_auth_param_req_st->auth_username), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_mgmt_plane_auth_param_req_st->auth_password), p_src + (*p_length), &offset);
	*p_length	+= offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_create_secure_conn_req_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_create_secure_conn_req_st	*p_icf_create_secure_conn_req_st = ICF_NULL;
	p_icf_create_secure_conn_req_st	= (icf_create_secure_conn_req_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_create_secure_conn_req_st\n"));

	/*icf_uint16_t				 exchange_id;
#define ICF_XCAP_USAGE	0x01
#define ICF_SYNCML_USAGE 0x02
	icf_uint8_t					 server_type;
	icf_transport_address_st	 server_addr; */

	icf_unpack_U16((icf_uint8_t *)&(p_icf_create_secure_conn_req_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t *)&(p_icf_create_secure_conn_req_st->server_type), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_create_secure_conn_req_st->server_addr), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_create_secure_conn_resp_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_create_secure_conn_resp_st	*p_icf_create_secure_conn_resp_st = ICF_NULL;
	p_icf_create_secure_conn_resp_st	= (icf_create_secure_conn_resp_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_create_secure_conn_resp_st\n"));

	/*	icf_uint16_t	exchange_id;
		icf_result_t	conn_req_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */

	icf_unpack_U16((icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->conn_req_status), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_create_secure_conn_resp_st->error_cause), p_src + (*p_length), &offset);
	*p_length	+= offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_close_secure_conn_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_close_secure_conn_req_st	*p_icf_close_secure_conn_req_st = ICF_NULL;
	p_icf_close_secure_conn_req_st	= (icf_close_secure_conn_req_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_close_secure_conn_req_st\n"));

	/*	icf_uint16_t	exchange_id;
		icf_uint32_t	conn_handle; */

	icf_unpack_U16((icf_uint8_t *)&(p_icf_close_secure_conn_req_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_close_secure_conn_req_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_close_secure_conn_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_close_secure_conn_resp_st	*p_icf_close_secure_conn_resp_st = ICF_NULL;
	p_icf_close_secure_conn_resp_st	= (icf_close_secure_conn_resp_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_close_secure_conn_resp_st\n"));

	/*	icf_uint16_t	exchange_id;
		icf_result_t	close_conn_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->close_conn_status), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_close_secure_conn_resp_st->error_cause), p_src + (*p_length), &offset);
	*p_length	+= offset;
	  	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_secure_conn_status_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_secure_conn_status_ind_st	*p_icf_secure_conn_status_ind_st = ICF_NULL;
	p_icf_secure_conn_status_ind_st	= (icf_secure_conn_status_ind_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_secure_conn_status_ind_st\n"));

	/*	icf_uint8_t		conn_status;
		icf_uint32_t	conn_handle; 
        icf_uint32_t	old_conn_handle */

	icf_unpack_U8((icf_uint8_t *)&(p_icf_secure_conn_status_ind_st->conn_status), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_secure_conn_status_ind_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_secure_conn_status_ind_st->old_conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_create_replace_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;
	
	icf_create_replace_res_req_st	*p_icf_create_replace_res_req_st = ICF_NULL;
	p_icf_create_replace_res_req_st	= (icf_create_replace_res_req_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_create_replace_res_req_st\n"));

	/* 
#define ICF_NO_RESP_DURATION_PRESENT	0x01
#define ICF_ACCEPT_PRESENT				0x02
#define ICF_IF_MATCH_PRESENT			0x04
#define ICF_IF_NONE_MATCH_PRESENT		0x08
#define ICF_ACCEPT_ENCODING_PRESENT		0x10
#define ICF_CONTENT_ENCODING_PRESENT	0x20
#define ICF_HIGH_PRIORITY_REQ			0x40
	icf_uint8_t					bitmask;
	icf_uint16_t				exchange_id;
	icf_uint32_t				conn_handle;
	icf_req_uri_st				request_uri;
	icf_transport_address_st	host_name;
	icf_xcap_res_et				res_type;
	icf_string_st				content_type;
	icf_duration_t				timer_duration;
	icf_stretchable_hdr_st		accept;
	icf_stretchable_hdr_st		if_match;
	icf_stretchable_hdr_st		if_none_match;
	icf_stretchable_hdr_st		accept_encoding;
	icf_stretchable_hdr_st		content_encoding;
	icf_uint32_t				payload_length;
	icf_uint8_t 				*payload; */
	
	icf_unpack_U8((icf_uint8_t *)&(p_icf_create_replace_res_req_st->bitmask), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_create_replace_res_req_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_create_replace_res_req_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_icf_req_uri_st((icf_uint8_t*)&(p_icf_create_replace_res_req_st->request_uri), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_create_replace_res_req_st->host_name), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_create_replace_res_req_st->res_type), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_create_replace_res_req_st->content_type), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_create_replace_res_req_st->timer_duration), p_src + (*p_length), &offset);  
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_create_replace_res_req_st->accept), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_create_replace_res_req_st->if_match), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_create_replace_res_req_st->if_none_match), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_create_replace_res_req_st->accept_encoding), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_create_replace_res_req_st->content_encoding), p_src + (*p_length), &offset);
	*p_length	+= offset;
  	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_create_replace_res_req_st->payload_length), p_src + (*p_length), &offset);
	*p_length	+= offset;

    p_icf_create_replace_res_req_st->payload = ICF_NULL;
	/* This piece of code unpacks variable length octet string */

    if (0 < p_icf_create_replace_res_req_st->payload_length  && p_icf_create_replace_res_req_st->payload_length < ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: Getting memory for icf_unpack_icf_create_replace_res_req_st\n"));
        p_icf_create_replace_res_req_st->payload = ICF_CLIB_MEMGET(p_icf_create_replace_res_req_st->payload_length);

        if (!p_icf_create_replace_res_req_st->payload) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: Failed In Allocating Memory for p_icf_create_replace_res_req_st->payload\n"));
            return ICF_FAILURE;
        }
    } else {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_create_replace_res_req_st - Invalid Structure Length!! Quitting! (%d) Bytes\n",p_icf_create_replace_res_req_st->payload_length));
        return ICF_FAILURE;
    }

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_create_replace_res_req_st - Unpacking Structure of Length (%d)\n", p_icf_create_replace_res_req_st->payload_length));

	count = 0;
	while(count < p_icf_create_replace_res_req_st->payload_length && p_icf_create_replace_res_req_st->payload)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_create_replace_res_req_st->payload[count]), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
		
		count++;
	}
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_delete_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_delete_res_req_st	*p_icf_delete_res_req_st = ICF_NULL;
	p_icf_delete_res_req_st	= (icf_delete_res_req_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_delete_res_req_st\n"));

	/*  icf_uint8_t					bitmask;
		icf_uint16_t				exchange_id;
		icf_uint32_t				conn_handle;
		icf_req_uri_st				request_uri;
		icf_transport_address_st	host_name;
		icf_xcap_res_et				res_type;
		icf_duration_t				timer_duration;
		icf_stretchable_hdr_st		accept;
		icf_stretchable_hdr_st		if_match;
		icf_stretchable_hdr_st		if_none_match; */
	
	icf_unpack_U8((icf_uint8_t *)&(p_icf_delete_res_req_st->bitmask), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_delete_res_req_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_delete_res_req_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_icf_req_uri_st((icf_uint8_t*)&(p_icf_delete_res_req_st->request_uri), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_delete_res_req_st->host_name), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_delete_res_req_st->res_type), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_delete_res_req_st->timer_duration), p_src + (*p_length), &offset);  
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_delete_res_req_st->accept), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_delete_res_req_st->if_match), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_delete_res_req_st->if_none_match), p_src + (*p_length), &offset);
	*p_length	+= offset;
	  	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_fetch_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_fetch_res_req_st	*p_icf_fetch_res_req_st = ICF_NULL;
	p_icf_fetch_res_req_st	= (icf_fetch_res_req_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_fetch_res_req_st\n"));

	/*  
ICF_IF_MATCH_PRESENT 
ICF_IF_NONE_MATCH_PRESENT
ICF_NO_RESP_DURATION_PRESENT 
	icf_uint8_t					bitmask;
	icf_uint16_t				exchange_id;
	icf_uint32_t				conn_handle;
	icf_req_uri_st				request_uri;
	icf_transport_address_st	host_name;
	icf_xcap_res_et				res_type;
	icf_duration_t				timer_duration;
	icf_stretchable_hdr_st		if_match;
	icf_stretchable_hdr_st		if_none_match; */
	
	icf_unpack_U8((icf_uint8_t *)&(p_icf_fetch_res_req_st->bitmask), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_fetch_res_req_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_fetch_res_req_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_icf_req_uri_st((icf_uint8_t*)&(p_icf_fetch_res_req_st->request_uri), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_fetch_res_req_st->host_name), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_fetch_res_req_st->res_type), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_fetch_res_req_st->timer_duration), p_src + (*p_length), &offset);  
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_fetch_res_req_st->if_match), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_fetch_res_req_st->if_none_match), p_src + (*p_length), &offset);
	*p_length	+= offset;
	 	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_syncml_send_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;
	
	icf_syncml_send_req_st	*p_icf_syncml_send_req_st = ICF_NULL;
	p_icf_syncml_send_req_st	= (icf_syncml_send_req_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_syncml_send_req_st\n"));

	/*  
#define ICF_HEADER_LIST_PRESENT 0x80
	icf_uint8_t		bitmask;
	icf_uint16_t	exchange_id;
	icf_uint32_t	conn_handle;
	icf_string_st	content_type;
	icf_string_st	cache_control;
	icf_string_st	transfer_encoding;
	icf_string_st	user_agent;
	icf_string_st	accept;
	icf_string_st	accept_char;
	icf_duration_t	timer_duration;
	icf_header_list_st  header_list;
	icf_uint32_t		payload_length;
	icf_uint8_t 	*payload; */
	
	icf_unpack_U8((icf_uint8_t *)&(p_icf_syncml_send_req_st->bitmask), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_syncml_send_req_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_syncml_send_req_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_syncml_send_req_st->content_type), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_syncml_send_req_st->cache_control), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_syncml_send_req_st->transfer_encoding), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_syncml_send_req_st->user_agent), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_syncml_send_req_st->accept), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_syncml_send_req_st->accept_char), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_syncml_send_req_st->timer_duration), p_src + (*p_length), &offset);  
	*p_length	+= offset;

	icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_syncml_send_req_st->header_list), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_syncml_send_req_st->payload_length), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
    p_icf_syncml_send_req_st->payload = ICF_NULL;
	/* This piece of code unpacks variable length octet string */

    if (0 < p_icf_syncml_send_req_st->payload_length  && p_icf_syncml_send_req_st->payload_length < ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: Getting memory for icf_unpack_icf_syncml_send_req_st\n"));
        p_icf_syncml_send_req_st->payload = ICF_CLIB_MEMGET(p_icf_syncml_send_req_st->payload_length);

        if (!p_icf_syncml_send_req_st->payload) 
		{
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: Failed In Allocating Memory for p_icf_syncml_send_req_st->payload\n"));
            return ICF_FAILURE;
        }
    } 
	else
	{
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_syncml_send_req_st - Invalid Structure Length!! Quitting! (%d) Bytes\n",p_icf_syncml_send_req_st->payload_length));
		return ICF_FAILURE;
	}
	ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_syncml_send_req_st - Unpacking Structure of Length (%d)\n", p_icf_syncml_send_req_st->payload_length));

	count = 0;
	while(count < p_icf_syncml_send_req_st->payload_length && p_icf_syncml_send_req_st->payload)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_syncml_send_req_st->payload[count]), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
		
		count++;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_api_proc_failure_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	
	icf_api_proc_failure_st	*p_icf_api_proc_failure_st = ICF_NULL;
	p_icf_api_proc_failure_st	= (icf_api_proc_failure_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_api_proc_failure_st\n"));

	/*  icf_uint16_t	exchange_id;
		icf_uint32_t	conn_handle;
		icf_api_id_t	api_id;
		icf_error_t		error_cause;*/
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_api_proc_failure_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_api_proc_failure_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U16((icf_uint8_t *)&(p_icf_api_proc_failure_st->api_id), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_api_proc_failure_st->error_cause), p_src + (*p_length), &offset);
    *p_length	+= offset;
	  	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_mgmt_plane_nw_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;
	
	icf_mgmt_plane_nw_resp_st	*p_icf_mgmt_plane_nw_resp_st = ICF_NULL;
	p_icf_mgmt_plane_nw_resp_st	= (icf_mgmt_plane_nw_resp_st *)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;
	/* This function packs icf_api_header_st */
	ICF_CLIB_TRACE(("Packing icf_mgmt_plane_nw_resp_st\n"));

	/* 
#define ICF_ENTITY_TAG_PRESENT		0x01
#define ICF_CONTENT_TYPE_PRESENT	0x02
#define ICF_CONNECTION_HEADER_VAL_PRESENT       0x04
	icf_uint8_t				bitmask;
	icf_api_id_t			req_api;
	icf_uint16_t			exchange_id;
	icf_uint32_t			conn_handle;
	icf_stretchable_hdr_st  entity_tag;
	icf_string_st			content_type;
#define ICF_CONN_HDR_VAL_INVALID                    0x00
#define ICF_CONN_HDR_VAL_CLOSE_WITH_REATTEMPT       0x01
#define ICF_CONN_HDR_VAL_CLOSE_WITH_NO_REATTEMPT    0x02
    icf_uint8_t             connection_header_val;
	icf_header_list_st		header_list;
	icf_uint32_t			response_code;
	icf_uint32_t			payload_length;
	icf_uint8_t				*payload; */
	
	icf_unpack_U8((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->bitmask), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->req_api), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U16((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->exchange_id), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_U32((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->conn_handle), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
	icf_unpack_icf_stretchable_hdr_st((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->entity_tag), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_mgmt_plane_nw_resp_st->content_type), p_src + (*p_length), &offset);
	*p_length	+= offset;

    icf_unpack_U8((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->connection_status), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_mgmt_plane_nw_resp_st->header_list), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->response_code), p_src + (*p_length), &offset);
	*p_length	+= offset;

	icf_unpack_U32((icf_uint8_t *)&(p_icf_mgmt_plane_nw_resp_st->payload_length), p_src + (*p_length), &offset);
	*p_length	+= offset;
	
    p_icf_mgmt_plane_nw_resp_st->payload = ICF_NULL;
	/* This piece of code unpacks variable length octet string */

    if (0 < p_icf_mgmt_plane_nw_resp_st->payload_length  && 
		p_icf_mgmt_plane_nw_resp_st->payload_length < ICF_CLIB_MAX_API_LENGTH) 
	{
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: Getting memory for icf_unpack_icf_mgmt_plane_nw_resp_st\n"));
        p_icf_mgmt_plane_nw_resp_st->payload = ICF_CLIB_MEMGET(p_icf_mgmt_plane_nw_resp_st->payload_length);
		
        if (!p_icf_mgmt_plane_nw_resp_st->payload) 
		{
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: Failed In Allocating Memory for p_icf_mgmt_plane_nw_resp_st->payload\n"));
            return ICF_FAILURE;
        }
		else
		{
			ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_mgmt_plane_nw_resp_st - Unpacking Structure of Length (%d)\n", p_icf_mgmt_plane_nw_resp_st->payload_length));
			count = 0;
			while(count < p_icf_mgmt_plane_nw_resp_st->payload_length && p_icf_mgmt_plane_nw_resp_st->payload)
			{
				icf_unpack_U8((icf_uint8_t*)&(p_icf_mgmt_plane_nw_resp_st->payload[count]), p_src + (*p_length), &offset);
				*p_length	+= offset;

				count++;
			}
			
		}
    }
	else
	{
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_mgmt_plane_nw_resp_st - 0 payload\n"));
    }


	return ICF_SUCCESS;
}

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Ends here ***************/
/*****************************************************************************/

/*ICF_HANDLE_REINVITE_START*/

icf_uint32_t
icf_unpack_icf_call_modify_cfm_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
    icf_uint8_t         count = 0;

	icf_call_modify_cfm_st	*p_icf_reinvite_cfm_st = ICF_NULL;
	
	p_icf_reinvite_cfm_st	= (icf_call_modify_cfm_st*)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function is for unpacking icf_call_modify_cfm_st */
	ICF_CLIB_TRACE(("Unpacking icf_call_modify_cfm_st\n"));

	icf_unpack_U32((icf_uint8_t*)&(p_icf_reinvite_cfm_st->bit_mask), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_cfm_st->result),p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_reinvite_cfm_st->cause),p_src + (*p_length), &offset);
	
	*p_length	+= offset;


	if(p_icf_reinvite_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_reinvite_cfm_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_reinvite_cfm_st->tag_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_reinvite_cfm_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_cfm_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_cfm_st->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
#endif


/*	if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MOD_CFM_DELETED_STREAM_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_reinvite_cfm_st->count), p_src + (*p_length), &offset);
	
		*p_length	+= offset;

		while(count<p_icf_reinvite_cfm_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_cfm_st->deleted_media_streams[count]), p_src + (*p_length), &offset);
        
			*p_length    += offset;

			count++;
		}
    }*/
	if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MODIFY_RESPONSE_CODE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_reinvite_cfm_st->nw_response_code), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
	
    if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MODIFY_CFM_REJECTED_STREAM_PRESENT)
    {
		icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_cfm_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;
		
		while(count<p_icf_reinvite_cfm_st->count)
		{
			icf_unpack_icf_stream_list_st(
              (icf_uint8_t*)&(p_icf_reinvite_cfm_st->deleted_stream_list[count]), 
               p_src + (*p_length), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

    if(p_icf_reinvite_cfm_st->bit_mask & ICF_CALL_MODIFY_RESPONSE_SIGNALING_MODE_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_cfm_st->signaling_mode), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_incoming_call_modify_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint8_t			count = 0;

	icf_incoming_call_modify_ind_st	*p_icf_reinvite_ind_st = ICF_NULL;
	
	p_icf_reinvite_ind_st	= (icf_incoming_call_modify_ind_st*)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function is for unpacking icf_incoming_call_modify_ind_st */
	ICF_CLIB_TRACE(("Unpacking icf_incoming_call_modify_ind_st\n"));

	icf_unpack_U32((icf_uint8_t*)&(p_icf_reinvite_ind_st->bit_mask), p_src + (*p_length), &offset);
	
	*p_length	+= offset;
  
        icf_unpack_U8((icf_uint8_t *)&(p_icf_reinvite_ind_st->confirm_required), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

    if(p_icf_reinvite_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_reinvite_ind_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_reinvite_ind_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_ind_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_ind_st->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_ind_st->bit_mask & ICF_INC_CALL_ACCEPT_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_call_modify_media_data_list_st(&(p_icf_reinvite_ind_st->p_active_stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
	else
	{
		p_icf_reinvite_ind_st->p_active_stream_list = ICF_NULL;
	}
	
	if(p_icf_reinvite_ind_st->bit_mask & ICF_INC_CALL_MOD_DELETED_STREAM_PRESENT)
    {
		icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_ind_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;
		
		while(count<p_icf_reinvite_ind_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_ind_st->deleted_stream_list[count]), p_src + (*p_length), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_incoming_call_modify_cancel_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
    /* To remove warnings */
     p_dest=p_dest;
     p_src=p_src;
     p_length=p_length;
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_call_modify_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint32_t		count = 0;

	icf_call_modify_req_st	*p_icf_reinvite_req_st = ICF_NULL;
	
	p_icf_reinvite_req_st	= (icf_call_modify_req_st*)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function is for unpacking icf_call_modify_req_st */
	ICF_CLIB_TRACE(("Unpacking icf_call_modify_req_st\n"));

	icf_unpack_U32((icf_uint8_t*)&(p_icf_reinvite_req_st->bit_mask), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_req_st->bit_mask & ICF_CALL_MODIFY_MEDIA_MODE_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_req_st->media_mode),p_src + (*p_length), &offset);
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_reinvite_req_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_reinvite_req_st->tag_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_reinvite_req_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_req_st->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_req_st->bit_mask & ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT)
    {
        icf_unpack_icf_call_modify_media_data_list_st(&(p_icf_reinvite_req_st->p_media_modify_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
	else
	{
		p_icf_reinvite_req_st->p_media_modify_list = ICF_NULL;
	}

	if(p_icf_reinvite_req_st->bit_mask & ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_req_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;
		
		while(count<p_icf_reinvite_req_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_req_st->deleted_media_streams[count]), p_src + (*p_length), &offset);
        
			*p_length    += offset;

			count++;
		}
    }


	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_call_modify_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;
	icf_uint8_t			count = 0;

	icf_call_modify_resp_st	*p_icf_reinvite_resp_st = ICF_NULL;
	
	p_icf_reinvite_resp_st	= (icf_call_modify_resp_st*)p_dest;

	/* Initialise length to 0 */
	*p_length	= 0;

	/* This function is for unpacking icf_call_modify_resp_st */
	ICF_CLIB_TRACE(("Unpacking icf_call_modify_resp_st\n"));

	icf_unpack_U32((icf_uint8_t*)&(p_icf_reinvite_resp_st->bit_mask), p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_resp_st->result),p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_reinvite_resp_st->error_cause),p_src + (*p_length), &offset);
	
	*p_length	+= offset;

	if(p_icf_reinvite_resp_st->bit_mask & ICF_RESPONSE_CODE_PRESENT)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_reinvite_resp_st->response_code),p_src + (*p_length), &offset);
	
		*p_length	+= offset;
	}

	if(p_icf_reinvite_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_reinvite_resp_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_reinvite_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_reinvite_resp_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

#ifdef ICF_REL_OLD
	if(p_icf_reinvite_resp_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_resp_st->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
#endif

	if(p_icf_reinvite_resp_st->bit_mask & ICF_CALL_MOD_ACCEPT_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_call_modify_media_data_list_st(&(p_icf_reinvite_resp_st->p_active_stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
	else
	{
		p_icf_reinvite_resp_st->p_active_stream_list = ICF_NULL;
	}
		
	if(p_icf_reinvite_resp_st->bit_mask & ICF_CALL_MOD_RESP_REJECT_STREAM_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_reinvite_resp_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;
		
		while(count<p_icf_reinvite_resp_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_reinvite_resp_st->deleted_stream_list[count]), p_src + (*p_length), &offset);
        
			*p_length    += offset;

			count++;
		}
    }

   if(p_icf_reinvite_resp_st->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_reinvite_resp_st->retry_after_duration), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    
	return ICF_SUCCESS;
}

/*ICF_HANDLE_REINVITE_END*/
#ifdef ICF_SERVICE_ENABLE

icf_uint32_t
icf_unpack_icf_cta_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_transfer_attended_req_st    *p_icf_call_transfer_attended_req_st = ICF_NULL;
    
    p_icf_call_transfer_attended_req_st    = (icf_call_transfer_attended_req_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_call_transfer_attended_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_transfer_attended_req_st\n"));

	icf_unpack_U16((icf_uint8_t*)&(p_icf_call_transfer_attended_req_st->transferred_party_id), p_src + (*p_length), &offset);	
	*p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_icf_call_transfer_attended_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;


    if(p_icf_call_transfer_attended_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_call_transfer_attended_req_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_call_transfer_attended_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_call_transfer_attended_req_st->tag_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_call_transfer_attended_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_call_transfer_attended_req_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
  
   	return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_ctu_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_transfer_unattended_req_st    *p_icf_ctu_req_st = ICF_NULL;
    
    p_icf_ctu_req_st    = (icf_call_transfer_unattended_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_transfer_unattended_req_st\n"));

    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_ctu_req_st->transferred_party_addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_icf_ctu_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;


    if(p_icf_ctu_req_st->bit_mask & ICF_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_ctu_req_st->toggle_presentation_indicator), p_src + (*p_length), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_ctu_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_ctu_req_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_ctu_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_ctu_req_st->tag_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_ctu_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_ctu_req_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

   	if(p_icf_ctu_req_st->bit_mask & ICF_ADDRESS_ADDNL_URI_PARAMTR_PRES)
	{
        p_icf_ctu_req_st->p_uri_parameter = (icf_large_string_st *)ICF_CLIB_MEMGET(sizeof(icf_large_string_st));
    	icf_unpack_icf_large_string_st((icf_uint8_t*)(p_icf_ctu_req_st->p_uri_parameter), p_src + (*p_length), &offset);
        *p_length    += offset;
	}
        
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_call_hold_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_hold_req_st *p_icf_chd_req_st = ICF_NULL;
    
    p_icf_chd_req_st    = (icf_call_hold_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_hold_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_chd_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_chd_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_chd_req_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_chd_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_chd_req_st->tag_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_chd_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_chd_req_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
    if(p_icf_chd_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_chd_req_st->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_call_resume_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_resume_req_st *p_icf_crm_req_st = ICF_NULL;
    
    p_icf_crm_req_st    = (icf_call_resume_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_resume_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_crm_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_crm_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_crm_req_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_crm_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_crm_req_st->tag_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_crm_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_crm_req_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
    if(p_icf_crm_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_crm_req_st->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}


#endif

icf_uint32_t
icf_unpack_icf_res_unavail_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
)
{
	icf_uint32_t		offset = 0;

	icf_res_unavail_ind_st	*p_res_unavail_ind = ICF_NULL;
	
	p_res_unavail_ind	= (icf_res_unavail_ind_st *)p_dest;

	*p_length = 0;

	icf_unpack_U16((icf_uint8_t*)&(p_res_unavail_ind->req_api_id), p_src + (*p_length), &offset);

	*p_length += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_res_unavail_ind->result), p_src + (*p_length), &offset);

	*p_length += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_res_unavail_ind->cause), p_src + (*p_length), &offset);

	*p_length += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_res_unavail_ind->retry_after), p_src + (*p_length), &offset);
	
	*p_length +=offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_info_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_req_st *p_icf_info_req_st = ICF_NULL;
    
    p_icf_info_req_st    = (icf_info_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_info_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_info_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_info_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_info_req_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_info_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
	{
		icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_info_req_st->tag_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_info_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_info_req_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_api_mesg_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_api_resp_st *p_icf_api_resp = ICF_NULL;
    
    p_icf_api_resp    = (icf_api_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_api_resp_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_api_resp->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_api_resp->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_api_resp->error_cause), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_api_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_api_resp->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
	if(p_icf_api_resp->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_api_resp->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
    if(p_icf_api_resp->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_api_resp->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    if(p_icf_api_resp->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_unpack_U32(
          (icf_uint8_t*)&(p_icf_api_resp->retry_after_duration),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;
    }

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_ctu_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_ctu_resp_st *p_icf_ctu_resp = ICF_NULL;
    
    p_icf_ctu_resp    = (icf_ctu_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_ctu_resp_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_ctu_resp->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_ctu_resp->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_ctu_resp->error_cause), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_ctu_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_ctu_resp->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    if(p_icf_ctu_resp->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_unpack_U32(
          (icf_uint8_t*)&(p_icf_ctu_resp->retry_after_duration),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;
    }

 
    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_cta_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_cta_resp_st *p_icf_cta_resp = ICF_NULL;
    
    p_icf_cta_resp    = (icf_cta_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cta_resp_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_cta_resp->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_cta_resp->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_cta_resp->error_cause), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_cta_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_cta_resp->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
 
    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_remote_call_transfer_initiated_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_remote_call_transfer_initiated_ind_st    *p_icf_remote_call_transfer_initiated_ind_st = ICF_NULL;
    
    p_icf_remote_call_transfer_initiated_ind_st    = (icf_remote_call_transfer_initiated_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_remote_call_transfer_initiated_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_remote_call_transfer_initiated_ind_st\n"));

	icf_unpack_U16((icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->new_call_id), p_src + (*p_length), &offset);	
	*p_length    += offset;
  
	icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->referred_party_addr), p_src + (*p_length), &offset);	
	*p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->bit_mask), p_src + (*p_length), &offset);	
	*p_length    += offset;

    if(p_icf_remote_call_transfer_initiated_ind_st->bit_mask & ICF_REFER_INITIATED_REFERRED_PARTY_NAME)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->referred_party_name), p_src + (*p_length), &offset);	
        *p_length    += offset;
    }

    if(p_icf_remote_call_transfer_initiated_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_remote_call_transfer_initiated_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_remote_call_transfer_initiated_ind_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
   	
	return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_api_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_api_ind_st *p_icf_api_resp = ICF_NULL;
    
    p_icf_api_resp    = (icf_api_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_api_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_api_resp->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_api_resp->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_api_resp->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
	if(p_icf_api_resp->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_api_resp->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
    if(p_icf_api_resp->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_api_resp->stream_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_redirection_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_call_redirection_ind_st *p_icf_red_ind = ICF_NULL;
    
    p_icf_red_ind    = (icf_call_redirection_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_redirection_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_red_ind->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_red_ind->bit_mask & ICF_CALL_RDR_IND_PTY_ADDR)
	{
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_red_ind->redirected_party_addr), p_src + (*p_length), &offset);		
		
		*p_length	+= offset;
	}
    
	if(p_icf_red_ind->bit_mask & ICF_CALL_RDR_IND_PTY_NAME)
	{
		icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_red_ind->redirected_party_name), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}
    
    if(p_icf_red_ind->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_red_ind->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_route_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

	icf_route_info_st *p_route_info = ICF_NULL;

	p_route_info = (icf_route_info_st*)p_dest;

	icf_unpack_U8((icf_uint8_t*)&(p_route_info->type), p_src + (*p_length), &offset);

	*p_length += offset;

	if ( ICF_IPC_UDP == p_route_info->type)
	{
		icf_unpack_icf_transport_addr_st((icf_uint8_t*)&(p_route_info->ipc.udp.ip), p_src + (*p_length), &offset);

		*p_length += offset;

		icf_unpack_U16((icf_uint8_t*)&(p_route_info->ipc.udp.port), p_src + (*p_length), &offset);

		*p_length += offset;
	}
	else if ( ICF_IPC_TCP == p_route_info->type)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_route_info->ipc.tcp.type), p_src + (*p_length), &offset);

		*p_length += offset;

		icf_unpack_icf_transport_addr_st((icf_uint8_t*)&(p_route_info->ipc.tcp.ip), p_src + (*p_length), &offset);

		*p_length += offset;

		icf_unpack_U16((icf_uint8_t*)&(p_route_info->ipc.tcp.port), p_src + (*p_length), &offset);

		*p_length += offset;

	}
	else if ( ICF_IPC_MSGQ == p_route_info->type)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_route_info->ipc.msgq.mtype), p_src + (*p_length), &offset);

		*p_length += offset;

		icf_unpack_U32((icf_uint8_t*)&(p_route_info->ipc.msgq.mkey), p_src + (*p_length), &offset);

		*p_length += offset;
	}
	else
	{
		return ICF_FAILURE;
	}

	return ICF_SUCCESS;
}




/***********************************

		ICF REL 6.0

************************************/




icf_uint32_t
icf_unpack_icf_call_modify_media_data_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;


	icf_call_modify_media_data_st *p_icf_call_modify_media_data_st = ICF_NULL;

	p_icf_call_modify_media_data_st = (icf_call_modify_media_data_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_call_modify_media_data_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_modify_media_data_st\n"));

	icf_unpack_U8((icf_uint8_t*)&(p_icf_call_modify_media_data_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_call_modify_media_data_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_call_modify_media_data_st->stream_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_CODEC_INFO_PRESENT)
	{
		icf_unpack_icf_codec_attrib_list_st(&(p_icf_call_modify_media_data_st->p_codec_info_list), p_src + (*p_length), &offset, p_icf_call_modify_media_data_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT)
	{
		switch(p_icf_call_modify_media_data_st->stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:

				  icf_unpack_icf_audio_profile_st((icf_uint8_t*)&(p_icf_call_modify_media_data_st->media_stream_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_STREAM_TYPE_VIDEO:

				  icf_unpack_icf_video_profile_st((icf_uint8_t*)&(p_icf_call_modify_media_data_st->media_stream_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;
		}

	}

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_ENC_ATTRIB_PRESENT)
	{
	icf_unpack_icf_encoded_attributes_list_st(&(p_icf_call_modify_media_data_st->p_encoded_attributes), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_call_modify_media_data_st->bit_mask & ICF_CALL_MOD_MEDIA_MODE_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_call_modify_media_data_st->media_mode), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_call_modify_media_data_list_st
(
	icf_list_st    **p_p_icf_call_modify_media_data_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_call_modify_media_data_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_call_modify_media_data_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_call_modify_media_data_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_call_modify_media_data_st*/
		(*p_p_icf_call_modify_media_data_list_st)->p_data = (icf_call_modify_media_data_st*)ICF_CLIB_MEMGET(sizeof(icf_call_modify_media_data_st));
		
		icf_unpack_icf_call_modify_media_data_st((icf_uint8_t*)((*p_p_icf_call_modify_media_data_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_call_modify_media_data_list_st = &((*p_p_icf_call_modify_media_data_list_st)->p_next);

	}

	(*p_p_icf_call_modify_media_data_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_media_connected_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;

	icf_media_connected_ind_st *p_icf_media_connected_ind_st = ICF_NULL;

	p_icf_media_connected_ind_st = (icf_media_connected_ind_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_media_connected_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_media_connected_ind_st\n"));
	
	icf_unpack_U8((icf_uint8_t*)&(p_icf_media_connected_ind_st->bit_mask), p_src + (*p_length), &offset);
    
	*p_length    += offset;
	
    icf_unpack_icf_stream_capab_list_st(&(p_icf_media_connected_ind_st->p_accepted_stream_list), p_src + (*p_length), &offset);

	*p_length    += offset;
    
	if(p_icf_media_connected_ind_st->bit_mask & ICF_MEDIA_CONECT_DELETED_STREAM_PRESENT)
    {
        
		icf_unpack_U8((icf_uint8_t*)&(p_icf_media_connected_ind_st->count), p_src + (*p_length), &offset);

		*p_length    += offset;
		
		
		while(count<p_icf_media_connected_ind_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_media_connected_ind_st->deleted_media_streams[count]), p_src + (*p_length), &offset);

			*p_length    += offset;

			count++;
		}
    }

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_connect_resp_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

	icf_connect_resp_st *p_icf_connect_resp_st = ICF_NULL;

	p_icf_connect_resp_st = (icf_connect_resp_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_connect_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_connect_resp_st\n"));
	
	icf_unpack_U32((icf_uint8_t*)&(p_icf_connect_resp_st->bit_mask), p_src + (*p_length), &offset);
    
	*p_length    += offset;
	
    icf_unpack_icf_call_modify_media_data_list_st(&(p_icf_connect_resp_st->p_accepted_stream_list), p_src + (*p_length), &offset);

	*p_length    += offset;

	if(p_icf_connect_resp_st->bit_mask & ICF_CONNECT_RESP_DELETED_STREAM_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_connect_resp_st->count), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		while(count<p_icf_connect_resp_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_connect_resp_st->deleted_media_streams[count]), p_src + (*p_length), &offset);

			*p_length    += offset;

			count++;
		}
	}
 if(p_icf_connect_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_connect_resp_st->header_list), p_src + (*p_length), &offset);

        *p_length   += offset;
    }

	return ICF_SUCCESS;
}


/*Functions for fax support*/


#ifdef ICF_FAX_SUPPORT

icf_uint32_t
icf_unpack_icf_start_fax_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_start_fax_req_st *p_icf_start_fax_req_st = ICF_NULL;

	p_icf_start_fax_req_st = (icf_start_fax_req_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_start_fax_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_start_fax_req_st\n"));
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_start_fax_req_st->bitmask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
		
    icf_unpack_U8((icf_uint8_t*)&(p_icf_start_fax_req_st->fax_type), p_src + (*p_length), &offset);
    
     *p_length    += offset;
	
    icf_unpack_U8(&(p_icf_start_fax_req_st->fax_pt_codec), p_src + (*p_length), &offset);

     *p_length    += offset;
   
    icf_unpack_U8((icf_uint8_t*)&(p_icf_start_fax_req_st->fax_method), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_start_fax_req_st->pt_param), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_start_fax_resp_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_start_fax_resp_st *p_icf_start_fax_resp_st = ICF_NULL;

	p_icf_start_fax_resp_st = (icf_start_fax_resp_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_start_fax_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_start_fax_resp_st\n"));
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_start_fax_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
	
	icf_unpack_U8((icf_uint8_t*)&(p_icf_start_fax_resp_st->result), p_src + (*p_length), &offset);
    
	*p_length    += offset;
	
    if(!p_icf_start_fax_resp_st->result)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_start_fax_resp_st->error_cause), p_src + (*p_length), &offset);

		*p_length    += offset;
	}

	if(p_icf_start_fax_resp_st->result)
	{
		icf_unpack_U8(&(p_icf_start_fax_resp_st->fax_type), p_src + (*p_length), &offset);

		*p_length    += offset;
	}
   
    if(p_icf_start_fax_resp_st->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_start_fax_resp_st->retry_after_duration), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_start_fax_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_start_fax_ind_st *p_icf_start_fax_ind_st = ICF_NULL;

	p_icf_start_fax_ind_st = (icf_start_fax_ind_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_start_fax_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_start_fax_ind_st\n"));
	
	icf_unpack_U8((icf_uint8_t*)&(p_icf_start_fax_ind_st->fax_type), p_src + (*p_length), &offset);
    
	*p_length    += offset;
	
    return ICF_SUCCESS;
}

#endif


icf_uint32_t
icf_unpack_icf_info_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_resp_st *p_icf_info_resp_st = ICF_NULL;
    
    p_icf_info_resp_st    = (icf_info_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_refer_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_info_resp_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_info_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_info_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_info_resp_st->error_cause), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_info_resp_st->response_code), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_info_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_info_resp_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_info_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_info_resp_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}



/*NOTIFY_OUT_OF_DIALOG_START*/
icf_uint32_t
icf_unpack_icf_notify_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_notify_cfm_st    *p_icf_notify_cfm_st = ICF_NULL;
    
    p_icf_notify_cfm_st    = (icf_notify_cfm_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_notify_cfm_st */
    ICF_CLIB_TRACE(("Packing icf_notify_cfm_st\n"));
    
    icf_unpack_U32((icf_uint8_t*)&(p_icf_notify_cfm_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;


	icf_unpack_U8((icf_uint8_t*)&(p_icf_notify_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_notify_cfm_st->bit_mask & ICF_NOTIFY_ERROR_CAUSE_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_notify_cfm_st->error_code), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_notify_cfm_st->bit_mask & ICF_NOTIFY_RESP_CODE_PRESENT)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_notify_cfm_st->response_code), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

    return(ICF_SUCCESS);
}
/*NOTIFY_OUT_OF_DIALOG_END*/

/*HANDLE_INFO_IND_START*/

icf_uint32_t
icf_unpack_icf_info_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_ind_st *p_icf_info_ind_st = ICF_NULL;
    
    p_icf_info_ind_st    = (icf_info_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_info_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_info_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_info_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_info_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
	{
		icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_info_ind_st->header_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

	if(p_icf_info_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
	{
		icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_info_ind_st->body_list), p_src + (*p_length), &offset);
		
		*p_length	+= offset;
	}

    return ICF_SUCCESS;
}

/*HANDLE_INFO_IND_END*/

/* OPTIONS handling - start*/

icf_uint32_t
icf_unpack_icf_options_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_options_req_st    *p_icf_options_req_st = ICF_NULL;
    
    p_icf_options_req_st    = (icf_options_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_options_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_options_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_options_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_options_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_options_req_st->destination_info), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_options_req_st->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_options_req_st->pref_identity), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_options_req_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_options_req_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_options_req_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    /* Fix for CSR 1-8223807 : SPR-20362*/
    if(p_icf_options_req_st->bit_mask & ICF_OPTIONS_REQ_REMOVE_HDR_LIST_PRESENT)
    {
       icf_unpack_icf_header_name_list_st((icf_uint8_t*)&(p_icf_options_req_st->remove_hdr_list), p_src + (*p_length), &offset);

	   *p_length    += offset;
    }

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_options_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_options_resp_st    *p_icf_options_resp_st = ICF_NULL;
    
    p_icf_options_resp_st    = (icf_options_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_options_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_options_resp_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_options_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    icf_unpack_U8((icf_uint8_t*)&(p_icf_options_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    if(p_icf_options_resp_st->bit_mask & ICF_OPTIONS_RESP_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_icf_options_resp_st->response_code_for_options), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
    
    if(p_icf_options_resp_st->bit_mask & ICF_OPTIONS_ERROR_CODE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_icf_options_resp_st->error_code_for_options), p_src + (*p_length), &offset);

        *p_length    += offset;
    }

    if(p_icf_options_resp_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_options_resp_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_resp_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_options_resp_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_resp_st->bit_mask & ICF_OPTIONS_REMOTE_STREAM_LIST_PRESENT)
    {
        icf_unpack_icf_stream_capab_list_st(&(p_icf_options_resp_st->p_remote_stream_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }


    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_inc_option_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_inc_options_ind_st    *p_icf_inc_options_ind_st = ICF_NULL;
    
    p_icf_inc_options_ind_st    = (icf_inc_options_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_inc_option_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_inc_option_ind_st\n"));
    icf_unpack_U32((icf_uint8_t*)&(p_icf_inc_options_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_inc_options_ind_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_inc_options_ind_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_inc_options_ind_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_inc_options_ind_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_inc_options_ind_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_inc_options_ind_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_inc_options_ind_st->bit_mask & ICF_INC_OPTIONS_IND_MEDIA_PRESENT)
    {
        icf_unpack_icf_stream_capab_list_st(&(p_icf_inc_options_ind_st->p_app_sdp_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_icf_options_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_options_cfm_st    *p_icf_options_cfm_st = ICF_NULL;
    
    p_icf_options_cfm_st    = (icf_options_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_icf_options_cfm_st */
    ICF_CLIB_TRACE(("Unpacking icf_icf_options_cfm_st\n"));
    
	icf_unpack_U32((icf_uint8_t*)&(p_icf_options_cfm_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
	icf_unpack_U8((icf_uint8_t*)&(p_icf_options_cfm_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    if(p_icf_options_cfm_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_options_cfm_st->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
	
	if(p_icf_options_cfm_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_options_cfm_st->tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_options_cfm_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_options_cfm_st->body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_options_cfm_st->bit_mask & ICF_OPTIONS_CFM_MEDIA_PRESENT)
    {
        icf_unpack_icf_stream_capab_list_st(&(p_icf_options_cfm_st->p_app_sdp_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

	if(p_icf_options_cfm_st->bit_mask & ICF_OPTIONS_CFM_DEL_MEDIA_PRESENT)
    {
	     icf_unpack_U8((icf_uint8_t*)&(p_icf_options_cfm_st->count), p_src + (*p_length), &offset);
        
        *p_length    += offset;

		while(count<p_icf_options_cfm_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&(p_icf_options_cfm_st->deleted_media_streams[count]), p_src + (*p_length), &offset);
        
		    *p_length    += offset;

			count++;
		}
    }

    return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_set_line_param_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_set_line_param_req_st   
 	*p_icf_set_line_param_req_st = ICF_NULL;
    
    p_icf_set_line_param_req_st    = 
	(icf_set_line_param_req_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_transport_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_line_param_req_st\n"));
    
    icf_unpack_icf_line_id_t((icf_uint8_t*)
	&(p_icf_set_line_param_req_st ->line_id), 
	p_src + (*p_length), &offset);
    *p_length    += offset;
    
    icf_unpack_U16((icf_uint8_t*)
	&(p_icf_set_line_param_req_st->bitmask), 
	p_src + (*p_length), &offset);
    *p_length    += offset;
			    if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_SUBS_HS_TIMER)
    {
    	icf_unpack_U32((icf_uint8_t*)
	&(p_icf_set_line_param_req_st -> 
	subscribe_head_start_timer), 
	p_src + (*p_length), &offset);
    	*p_length    += offset;
    }
   			     if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_T1_TIMER)
    {
    	icf_unpack_U32((icf_uint8_t*)
	&(p_icf_set_line_param_req_st -> sip_timer_t1),
 
	p_src + (*p_length), &offset);
    	*p_length    += offset;
    }
    			    if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_T2_TIMER)
    {
    	icf_unpack_U32((icf_uint8_t*)
	&(p_icf_set_line_param_req_st -> sip_timer_t2), 
	p_src + (*p_length), &offset);
    	*p_length    += offset;
    }
    	if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_INV_TIMER)
    {
    	icf_unpack_U32((icf_uint8_t*)
	&(p_icf_set_line_param_req_st ->sip_timer_invite), 
	p_src + (*p_length), &offset);
    	*p_length    += offset;
    }    
    	if (p_icf_set_line_param_req_st ->bitmask &
	ICF_SET_LINE_PRM_NON_INV_TIMER)
    {
      	icf_unpack_U32((icf_uint8_t*)
	&(p_icf_set_line_param_req_st -> 
	sip_timer_noninvite), 
	p_src + (*p_length), &offset);
    	*p_length    += offset;
    }
    /* Start : Addition for Per Line Feature */
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_SIP_DSCP)
    {
      icf_unpack_U8((icf_uint8_t*)
      &(p_icf_set_line_param_req_st->sip_dscp),
      p_src + (*p_length), &offset);
      *p_length    += offset;
    }
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_RTP_DSCP)
    {
     icf_unpack_U8((icf_uint8_t*)
     &(p_icf_set_line_param_req_st->rtp_dscp),
     p_src + (*p_length), &offset);
     *p_length    += offset;
    }
    #ifdef ICF_SESSION_TIMER
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_MIN_SE)
    {
     icf_unpack_U32((icf_uint8_t*)
     &(p_icf_set_line_param_req_st->min_se),
     p_src + (*p_length), &offset);
     *p_length    += offset;
   }
   if (p_icf_set_line_param_req_st->bitmask &
       ICF_SET_LINE_SESSION_TIMER)
   {
    icf_unpack_U32((icf_uint8_t*)
    &(p_icf_set_line_param_req_st->session_timer),
    p_src + (*p_length), &offset);
    *p_length    += offset;
   }
   if (p_icf_set_line_param_req_st->bitmask &
       ICF_SET_LINE_SESSION_TIMER_REFRESHER)
   {
    icf_unpack_U8((icf_uint8_t*)
    &(p_icf_set_line_param_req_st->session_timer_refresher), 
    p_src + (*p_length), &offset);
    *p_length    += offset;
   }
   #endif
   if (p_icf_set_line_param_req_st->bitmask &
       ICF_SET_LINE_AUDIO_CODEC)
    {
      icf_unpack_icf_codec_attrib_list_st(&(p_icf_set_line_param_req_st->p_codec_attributes_audio),  
      p_src + (*p_length),  &offset,  ICF_STREAM_TYPE_AUDIO);
      *p_length    += offset;
    }
    if (p_icf_set_line_param_req_st->bitmask &
        ICF_SET_LINE_VIDEO_CODEC)
     {
       icf_unpack_icf_codec_attrib_list_st(&(p_icf_set_line_param_req_st->p_codec_attributes_video),
       p_src + (*p_length),  &offset,  ICF_STREAM_TYPE_VIDEO);
       *p_length    += offset;
     }
	if(p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_CONTACT_USER_INFO)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_set_line_param_req_st->contact_user_info), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }
    /* End : Addition for Per Line Feature */ 

	/* Fix for CSR 1-8047728: SPR 20246 */
	if (p_icf_set_line_param_req_st->bitmask & ICF_SET_LINE_REMOVE_ROUTE_HDR)
    {
       icf_unpack_U8(
			(icf_uint8_t *) &(p_icf_set_line_param_req_st->remove_route_hdr),
			p_src + (*p_length),
			&offset);

       *p_length    += offset;
    }

    return ICF_SUCCESS;
}

/* OPTIONS handling - end*/

icf_uint32_t
icf_unpack_icf_info_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_info_cfm_st    *p_info_cfm_st = ICF_NULL;
    
    p_info_cfm_st    = (icf_info_cfm_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_info_cfm_st */
    ICF_CLIB_TRACE(("UnPacking icf_info_cfm_st\n"));
   
    icf_unpack_U8((icf_uint8_t*)&(p_info_cfm_st->result), p_src + (*p_length), &offset);    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_info_cfm_st->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;    

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_ERROR_CAUSE_PRESENT)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_info_cfm_st->error_cause), p_src + (*p_length),  &offset);
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_RESPONSE_CODE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_info_cfm_st->response_code),p_src + (*p_length),  &offset);
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_HEADER_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_info_cfm_st->header_list), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_info_cfm_st->tag_list), p_src + (*p_length), &offset); 
        *p_length    += offset;
    }

    if(p_info_cfm_st->bit_mask & ICF_INFO_CFM_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_info_cfm_st->body_list), p_src + (*p_length), &offset);
        *p_length    += offset;
    }
 

    return ICF_SUCCESS;
}


/* Start: Additions for per Line Feature */
icf_uint32_t icf_unpack_icf_hold_option_st( icf_uint8_t    *p_dest,
                                            icf_uint8_t    *p_src,
                                            icf_uint32_t   *p_length )
{
  icf_uint32_t        offset = 0;
  icf_hold_option_st  * p_icf_hold_option_st = ICF_NULL;
  p_icf_hold_option_st = ( icf_hold_option_st *)p_dest;
  /* Initialise length to 0 */
  *p_length    = 0;
  /* This function is for unpacking icf_hold_option_st */
  ICF_CLIB_TRACE(("Unpacking icf_hold_option_st \n"));
  icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_hold_option_st->line_id),
  p_src + (*p_length), &offset);
  *p_length    += offset;
  icf_unpack_U8((icf_uint8_t*)&(p_icf_hold_option_st->hold_option), 
  p_src + (*p_length), &offset);
  *p_length    += offset;
  return(ICF_SUCCESS);
}
/* End: Additions for Per Line Feature */


icf_uint32_t 
icf_unpack_icf_line_id_t
( 
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
)
{
  icf_uint32_t        offset = 0;
  *p_length    = 0;
  icf_unpack_U16((icf_uint8_t*)(p_dest),p_src + (*p_length), &offset);
  *p_length    += offset;
  return(ICF_SUCCESS);
}


icf_uint32_t
icf_unpack_icf_early_incoming_call_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_early_incoming_call_ind_st    *p_icf_early_call_ind = ICF_NULL;
    
    p_icf_early_call_ind    = (icf_early_incoming_call_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_incoming_call_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_early_call_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_early_call_ind->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_early_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_ADDR)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_early_call_ind->calling_party_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_early_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_NAME)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_icf_early_call_ind->calling_party_name), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_early_call_ind->bit_mask & ICF_EARLY_CALL_CALLED_PTY_ADDR)
    {
        icf_unpack_icf_address_st((icf_uint8_t*)&(p_icf_early_call_ind->called_party_addr), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    
    if(p_icf_early_call_ind->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_early_call_ind->header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }


    return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_early_incoming_call_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t				offset = 0;

    icf_early_incoming_call_cfm_st		*p_early_call_cfm = ICF_NULL;
    
    p_early_call_cfm    = (icf_early_incoming_call_cfm_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_info_cfm_st */
    ICF_CLIB_TRACE(("Un-Packing icf_early_incoming_call_cfm_st\n"));
    
    icf_unpack_U8((icf_uint8_t*)&(p_early_call_cfm->result), p_src + (*p_length), &offset);    
    *p_length    += offset;

	icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_early_call_cfm->line_id), p_src + (*p_length), &offset);
	*p_length    += offset;

    return ICF_SUCCESS;
}


#ifdef ICF_TRACE_ENABLE
icf_uint32_t icf_unpack_icf_trace_level_req_st( icf_uint8_t    *p_dest,
                                            icf_uint8_t    *p_src,
                                            icf_uint32_t   *p_length )
{
  icf_uint32_t        offset = 0;
  icf_set_trace_level_req_st *p_trace_level = ICF_NULL;
  p_trace_level = (icf_set_trace_level_req_st *)p_dest;
  /* Initialise length to 0 */
  *p_length    = 0;
  /* This function is for unpacking icf_set_trace_level_req_st */
  ICF_CLIB_TRACE(("Unpacking icf_set_trace_level_req_st \n"));
  icf_unpack_U8((icf_uint8_t*)&(p_trace_level->status),
 p_src + (*p_length), &offset);
  *p_length    += offset;
  icf_unpack_U8((icf_uint8_t*)&(p_trace_level->trace_level),
  p_src + (*p_length), &offset);
  *p_length    += offset;

  icf_unpack_U32((icf_uint8_t*)&(p_trace_level->trace_type),
  p_src + (*p_length), &offset);
  *p_length    += offset;

  return(ICF_SUCCESS);
}
#endif

#ifdef ICF_ERROR_ENABLE
icf_uint32_t icf_unpack_icf_error_report_level_req_st( icf_uint8_t    *p_dest,
                                            icf_uint8_t    *p_src,
                                            icf_uint32_t   *p_length )
{
  icf_uint32_t        offset = 0;
  icf_set_error_report_level_req_st *p_error_report_level = ICF_NULL;
  p_error_report_level = (icf_set_error_report_level_req_st *)p_dest;
  /* Initialise length to 0 */
  *p_length    = 0;
  /* This function is for unpacking icf_set_trace_level_req_st */
  ICF_CLIB_TRACE(("Unpacking icf_set_error_report_level_req_st \n"));
  icf_unpack_U8((icf_uint8_t*)&(p_error_report_level->status),
 p_src + (*p_length), &offset);
  *p_length    += offset;
  icf_unpack_U8((icf_uint8_t*)&(p_error_report_level->error_level),
  p_src + (*p_length), &offset);
  *p_length    += offset;

  icf_unpack_U32((icf_uint8_t*)&(p_error_report_level->error_type),
  p_src + (*p_length), &offset);
  *p_length    += offset;

  return(ICF_SUCCESS);
}

icf_uint32_t icf_unpack_icf_report_error_ind_st( icf_uint8_t    *p_dest,
                                            icf_uint8_t    *p_src,
                                            icf_uint32_t   *p_length )
{

    icf_uint32_t        offset = 0;
    icf_uint8_t        count = 0;
    icf_report_error_ind_st *p_error_report = ICF_NULL;
    p_error_report = (icf_report_error_ind_st*)p_dest;
    /* Initialise length to 0 */
    *p_length    = 0;
    /* This function packs icf_report_error_ind_st */
    ICF_CLIB_TRACE(("Packing icf_report_error_ind_st\n"));
 
    icf_unpack_U32((icf_uint8_t*)&(p_error_report->error_id),p_src + (*p_length), &offset);
    *p_length += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_error_report->error_data.num_of_integer_values),p_src + (*p_length), &offset);
    *p_length += offset;
    /* Pack the integer values
     */
    for(count = 0; count < p_error_report->error_data.num_of_integer_values;\
            count++)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_error_report->error_data.int_val[count]),p_src + (*p_length), &offset);
        *p_length += offset;
    }

    icf_unpack_U8((icf_uint8_t*)&(p_error_report->error_data.number_of_string_values),p_src + (*p_length), &offset);
    *p_length += offset;
    /* Pack the munber of string values present in the array
     */
    for(count = 0; count < p_error_report->error_data.number_of_string_values;\
            count++)
    {
        icf_unpack_icf_string_st((icf_uint8_t*)&(p_error_report->error_data.string_val[count]),\
                p_src + (*p_length),&offset);
        *p_length += offset;
    }


    icf_unpack_icf_string_st((icf_uint8_t*)&(p_error_report->error_data.err_string), \
            p_src + (*p_length),&offset);
    *p_length += offset;


  return(ICF_SUCCESS);
}
#endif


icf_uint32_t
icf_unpack_icf_media_connect_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;

    icf_media_connect_req_st    *p_icf_media_connect_req_st = ICF_NULL;
    
    p_icf_media_connect_req_st    = (icf_media_connect_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_media_connect_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_media_connect_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_media_connect_req_st->bit_mask), p_src\
            + (*p_length), &offset);
    
    *p_length    += offset;
    
    if(p_icf_media_connect_req_st->bit_mask & ICF_PRIVACY_IND_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_media_connect_req_st->privacy_ind),\
                p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_media_connect_req_st->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        icf_unpack_icf_header_list_st((icf_uint8_t*)&(p_icf_media_connect_req_st->\
                    header_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_media_connect_req_st->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        icf_unpack_icf_tag_list_st((icf_uint8_t*)&(p_icf_media_connect_req_st->\
                    tag_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    if(p_icf_media_connect_req_st->bit_mask & ICF_BODY_LIST_PRESENT)
    {
        icf_unpack_icf_msg_body_list_st((icf_uint8_t*)&(p_icf_media_connect_req_st->\
                    body_list), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

/*ICF REL 6.0 : multiple m lines*/
	if(p_icf_media_connect_req_st->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_capab_list_st(&(p_icf_media_connect_req_st->\
                    p_stream_list), p_src + (*p_length), &offset);
	
		*p_length	+= offset;
	}

    if(ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT & p_icf_media_connect_req_st->bit_mask)
    {
      icf_unpack_U8(&(p_icf_media_connect_req_st->media_mode), \
          p_src + (*p_length) , &offset);
      *p_length    += offset;
    }

	if(p_icf_media_connect_req_st->bit_mask & ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_media_connect_req_st->count), p_src\
                + (*p_length), &offset);
        
        *p_length    += offset;

		while(count<p_icf_media_connect_req_st->count)
		{
			icf_unpack_icf_stream_list_st((icf_uint8_t*)&\
                    (p_icf_media_connect_req_st->deleted_media_streams[count])\
                    , p_src + (*p_length), &offset);
        
		    *p_length    += offset;

			count++;
		}
    }
    icf_unpack_U8(
            (icf_uint8_t *)&(p_icf_media_connect_req_st->send_without_sdp), 
            p_src + (*p_length), 
            &offset);

    *p_length    += offset;
    

    return ICF_SUCCESS;
}
icf_uint32_t
icf_unpack_icf_unexpected_msg_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_unexpected_msg_ind_st    *p_icf_unexpected_msg_ind_st = ICF_NULL;

    p_icf_unexpected_msg_ind_st    = (icf_unexpected_msg_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_unexpected_msg_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_unexpected_msg_ind_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_unexpected_msg_ind_st->error_cause), p_src + (*p_length), &offset);

    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_unexpected_msg_ind_st->api_id), p_src + (*p_length), &offset);

    *p_length    += offset;
 
    return ICF_SUCCESS;

}

icf_uint32_t icf_unpack_icf_stop_fax_resp_st(icf_uint8_t    *p_dest,
                                             icf_uint8_t    *p_src,
                                             icf_uint32_t   *p_length)
{
    icf_uint32_t        offset = 0;
    icf_stop_fax_resp_st *p_stop_fax_resp = ICF_NULL;
    p_stop_fax_resp = (icf_stop_fax_resp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;
    /* This function is for unpacking icf_stop_fax_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_stop_fax_resp_st \n"));
    
    icf_unpack_U32((icf_uint8_t*)&(p_stop_fax_resp->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;
	

    icf_unpack_U8((icf_uint8_t*)&(p_stop_fax_resp->result), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_stop_fax_resp->error_cause), p_src + (*p_length), &offset);
    *p_length    += offset;
   
    if(p_stop_fax_resp->bit_mask & ICF_RETRY_AFTER_DURATION_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_stop_fax_resp->retry_after_duration), p_src + (*p_length), &offset);
        
        *p_length    += offset;
    }

    return(ICF_SUCCESS);
}

icf_uint32_t icf_unpack_icf_stop_fax_ind_st(icf_uint8_t    *p_dest,
                                             icf_uint8_t    *p_src,
                                             icf_uint32_t   *p_length)
{
    icf_uint32_t        offset = 0;
    icf_stop_fax_ind_st *p_stop_fax_ind = ICF_NULL;
    p_stop_fax_ind = (icf_stop_fax_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;
    /* This function is for unpacking icf_stop_fax_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_stop_fax_ind_st \n"));

    icf_unpack_U8((icf_uint8_t*)&(p_stop_fax_ind->fax_type), p_src + (*p_length), &offset);
    *p_length    += offset;
    
	return(ICF_SUCCESS);
}

/*call_mod_cancel*/
icf_uint32_t icf_unpack_icf_call_mod_cancel_resp_st(icf_uint8_t    *p_dest,
                                             icf_uint8_t    *p_src,
                                             icf_uint32_t   *p_length)
{
    icf_uint32_t        offset = 0;
    icf_call_modify_cancel_resp_st *p_call_modify_cancel_resp = ICF_NULL;
    p_call_modify_cancel_resp = (icf_call_modify_cancel_resp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;
    /* This function is for unpacking icf_call_modify_cancel_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_call_modify_cancel_resp_st \n"));

    icf_unpack_U8((icf_uint8_t*)&(p_call_modify_cancel_resp->result), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_call_modify_cancel_resp->error_cause), p_src + (*p_length), &offset);
    *p_length    += offset;
    return(ICF_SUCCESS);
}

icf_uint32_t icf_unpack_icf_remote_call_transfer_initiated_cfm_st(icf_uint8_t    *p_dest,
                                             icf_uint8_t    *p_src,
                                             icf_uint32_t   *p_length)
{
    icf_uint32_t        offset = 0;
    icf_remote_call_transfer_initiated_cfm_st *p_cti_cfm = ICF_NULL;
    p_cti_cfm = (icf_remote_call_transfer_initiated_cfm_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;
    /* This function is for unpacking icf_stop_fax_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_remote_call_transfer_initiated_cfm_st \n"));

    icf_unpack_U32((icf_uint8_t*)&(p_cti_cfm->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_cti_cfm->result), p_src + (*p_length), &offset);
    *p_length    += offset;

    if (ICF_CALL_TRANS_INIT_ERROR_CAUSE_PRESENT & p_cti_cfm->bit_mask)
    {
        icf_unpack_U16((icf_uint8_t*)&(p_cti_cfm->error_code), p_src + (*p_length), &offset);
        *p_length    += offset;
    }

    if (ICF_CALL_TRANS_INIT_CFM_CODE_PRESENT & p_cti_cfm->bit_mask)
    {
        icf_unpack_U32((icf_uint8_t*)&(p_cti_cfm->response_code), p_src + (*p_length), &offset);
        *p_length    += offset;
    }


    return(ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_time_stamp_st
(
     icf_uint8_t    *p_dest,
     icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_time_stamp_st    *p_time_stamp = ICF_NULL;

    p_time_stamp    = (icf_time_stamp_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_timestamp_st */
    ICF_CLIB_TRACE(("Unpacking icf_time_stamp_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_time_stamp->time_in_secs), 
		p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_time_stamp->time_in_millisecs), 
		p_src + (*p_length), &offset);
    *p_length    += offset;

    return ICF_SUCCESS;
}



#ifdef ICF_TRACE_ENABLE
icf_uint32_t
icf_unpack_icf_trace_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;

    icf_trace_data_st    *p_trace_data = ICF_NULL;

    p_trace_data    = (icf_trace_data_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

   /* This function unpacks icf_trace_data_st */
     ICF_CLIB_TRACE(("Unpacking icf_trace_data_st\n"));

     icf_unpack_icf_time_stamp_st(
		(icf_uint8_t*)&(p_trace_data->trace_timestamp),
		p_src + (*p_length),  &offset);
     *p_length    += offset;


     icf_unpack_U8(
		(icf_uint8_t*)&(p_trace_data->num_of_integer_values),
		p_src + (*p_length),  &offset);
     *p_length    += offset;

    for(count = 0;count <p_trace_data->num_of_integer_values;
                count++)
     {
        icf_unpack_U32((icf_uint8_t*)&(p_trace_data->int_val[count]),
			p_src + (*p_length),  &offset);

        *p_length    += offset;

     }


     icf_unpack_U8(
		(icf_uint8_t*)&(p_trace_data->number_of_string_values),
		p_src + (*p_length),  &offset);

     *p_length    += offset;

     for(count = 0;count <p_trace_data->number_of_string_values;
                count++)
     {
          icf_unpack_icf_string_st(
			(icf_uint8_t*)&(p_trace_data->string_val[count]), 
			p_src + (*p_length),  &offset);

          *p_length    += offset;

      }

     icf_unpack_icf_string_st(
		(icf_uint8_t*)&(p_trace_data->trc_string), 
		p_src + (*p_length),  &offset);

     *p_length    += offset;

     return ICF_SUCCESS;
}





icf_uint32_t 
icf_unpack_icf_report_trace_ind_st
(
		icf_uint8_t    *p_dest,
        icf_uint8_t    *p_src,
        icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0, count =0;
    icf_report_trace_ind_st *p_report_trace_ind = ICF_NULL;

    p_report_trace_ind = (icf_report_trace_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_report_trace_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_report_trace_ind_st \n"));

    icf_unpack_U32((icf_uint8_t*)&(p_report_trace_ind->trace_id), 
		p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_report_trace_ind->trace_type), 
		p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_icf_trace_data_st(
		(icf_uint8_t*)&(p_report_trace_ind->trace_data), 
		p_src + (*p_length), &offset);
    *p_length    += offset;

        switch(p_report_trace_ind->trace_id)
        {
        case ICF_TRACE_MEDIA_CAPABILITY:
        case ICF_TRACE_CODEC_RESERVE:
        case ICF_TRACE_STREAM_CREATE:
        case ICF_TRACE_STREAM_MODIFY:
        case ICF_TRACE_CODEC_RELEASE:
        {
                icf_list_st *p_codec_list = ICF_NULL; 

                icf_unpack_icf_codec_attrib_list_st(
                  &p_codec_list,
                  p_src + (*p_length), 
                  &offset, (icf_uint8_t) p_report_trace_ind->trace_data.int_val[0]);
                p_report_trace_ind->trace_data.p_var_data = (icf_void_t *)
                         p_codec_list;
                break;
        }

       case ICF_TRACE_CODEC_COMMIT:
          {
              icf_codec_attrib_st *p_codec_info = (icf_codec_attrib_st *)
                    ICF_CLIB_MEMGET(sizeof(icf_codec_attrib_st));

              icf_unpack_icf_codec_attrib_st(
                  (icf_uint8_t*)(p_codec_info),
                  p_src + (*p_length),
                  &offset, (icf_uint8_t) p_report_trace_ind->trace_data.int_val[0]);
               
               p_report_trace_ind->trace_data.p_var_data = (icf_void_t *)
                         p_codec_info;
               break;
          }
       case ICF_TRACE_LOCAL_SDP_OFFER:
       case ICF_TRACE_REMOTE_SDP_OFFER:
       case ICF_TRACE_LOCAL_SDP_ANSWER:
       case ICF_TRACE_REMOTE_SDP_ANSWER:
       {
            icf_list_st *p_sdp_list = ICF_NULL;

                icf_unpack_icf_call_modify_media_data_list_st(
                      &p_sdp_list,
                      p_src + (*p_length),
                      &offset);
 
                p_report_trace_ind->trace_data.p_var_data = p_sdp_list;
                break;
       }
       case ICF_TRACE_SIP_PDU:
       case ICF_TRACE_APP_MESG:
       case ICF_TRACE_MEDIA_MESG:
           {
              icf_generic_string_st *p_msg_data = ICF_NULL;

              p_msg_data = 
                    ICF_CLIB_MEMGET(sizeof(icf_generic_string_st));

              p_report_trace_ind->trace_data.p_var_data = p_msg_data;

              icf_unpack_U32((icf_uint8_t *)&(p_msg_data->len), 
                 p_src + (*p_length), &offset);

              *p_length       += offset;
              
               p_msg_data->p_buff = 
                       ICF_CLIB_MEMGET(p_msg_data->len + 1);

              for(count = 0;count < p_msg_data->len; count++)
              {
                  icf_unpack_U8(
                      (icf_uint8_t*)&(p_msg_data->p_buff[count]),
                       p_src + (*p_length),
                       &offset);
                  *p_length       += offset;
              }
              p_msg_data->p_buff[p_msg_data->len] = '\0';
              break;
           }  
        case ICF_TRACE_CALL_CONNECTED:
          {
                icf_call_connect_data_st *p_call_connect_data =
                    (icf_call_connect_data_st *)ICF_CLIB_MEMGET(
                        sizeof(icf_call_connect_data_st));

                icf_unpack_icf_call_connect_data_st(
                      (icf_uint8_t *)p_call_connect_data,
                      p_src + (*p_length),
                      &offset);

              p_report_trace_ind->trace_data.p_var_data = p_call_connect_data;
              break;
          }
        }
    *p_length    += offset;
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stream_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
        icf_uint32_t        offset = 0;

        icf_stream_data_st  *p_icf_stream_data_st = (icf_stream_data_st *)p_dest;

        /* This function packs icf_stream_data_st */
        ICF_CLIB_TRACE(("Packing icf_stream_data_st\n"));

        /* Initialise length to 0 */
        *p_length    = 0;

        icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_data_st->stream_id), 
                  p_src + (*p_length), &offset);

        *p_length += offset;

        icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_data_st->stream_type), 
                  p_src + (*p_length), &offset);

        *p_length += offset;

        if(ICF_STREAM_TYPE_TBCP != p_icf_stream_data_st->stream_type)
        {
            icf_unpack_icf_codec_attrib_list_st(&(p_icf_stream_data_st->p_offered_codec),
                    p_src + (*p_length),
                    &offset, p_icf_stream_data_st->stream_type);

            *p_length += offset;

            p_icf_stream_data_st->p_negotiated_codec = 
              (icf_codec_attrib_st *)ICF_CLIB_MEMGET(sizeof(icf_codec_attrib_st));

            icf_unpack_icf_codec_attrib_st(
                    (icf_uint8_t*)(p_icf_stream_data_st->p_negotiated_codec),
                     p_src + (*p_length),
                    &offset, p_icf_stream_data_st->stream_type);

            *p_length += offset;
        }

        icf_unpack_icf_transport_address_st(
                     (icf_uint8_t*)&(p_icf_stream_data_st->local_rtp_address), 
                       p_src + (*p_length), &offset);

        *p_length    += offset;

         icf_unpack_icf_transport_address_st(
                     (icf_uint8_t*)&(p_icf_stream_data_st->remote_rtp_address), 
                       p_src + (*p_length), &offset);

        *p_length    += offset;

        return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stream_data_list_st
(
        icf_list_st    **p_p_icf_stream_data_list_st,
        icf_uint8_t    *p_src,
        icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t        count = 0;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_codec_attrib_st*/
    ICF_CLIB_TRACE(("Packing list of icf_codec_attrib_st\n"));

    /*Pack count = 0 initially*/
    icf_unpack_U8((icf_uint8_t*)&count,  p_src + (*p_length), &offset);

    *p_length    += offset;

    while(count--)
    {
   
        *p_p_icf_stream_data_list_st = 
              (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

        /* get memory for icf_stream_data_st*/
        (*p_p_icf_stream_data_list_st)->p_data = 
              (icf_stream_data_st*)ICF_CLIB_MEMGET(sizeof(icf_stream_data_st));
        
        icf_unpack_icf_stream_data_st(
                (icf_uint8_t*)((*p_p_icf_stream_data_list_st)->p_data), 
                  p_src + (*p_length), &offset);

        *p_length    += offset;

        p_p_icf_stream_data_list_st = &((*p_p_icf_stream_data_list_st)->p_next);

    }

        *p_p_icf_stream_data_list_st = ICF_NULL;
        return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_call_connect_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
        icf_uint32_t        offset = 0;

        icf_call_connect_data_st  *p_call_connect_data_st = 
                  (icf_call_connect_data_st *)p_dest;

        /* This function unpacks icf_call_connect_data_st */
        ICF_CLIB_TRACE(("UnPacking icf_call_connect_data_st\n"));

        /* Initialise length to 0 */
        *p_length    = 0;

        icf_unpack_icf_address_st(
               (icf_uint8_t*)&(p_call_connect_data_st->calling_party_address), 
                p_src + (*p_length), &offset);

        *p_length    += offset;

        icf_unpack_icf_address_st(
               (icf_uint8_t*)&(p_call_connect_data_st->called_party_address), 
                p_src + (*p_length), &offset);

        *p_length    += offset;

        icf_unpack_icf_line_id_t(
             (icf_uint8_t*)&(p_call_connect_data_st->line_id), 
             p_src + (*p_length), &offset);

        *p_length    += offset;

        icf_unpack_U8(
                 (icf_uint8_t*)&(p_call_connect_data_st->call_direction), 
                 p_src + (*p_length), &offset);

        *p_length += offset;

        icf_unpack_icf_stream_data_list_st(
               &(p_call_connect_data_st->p_stream_data), 
               p_src + (*p_length), &offset);

        *p_length += offset;

        icf_unpack_icf_transport_address_st(
               (icf_uint8_t*)&(p_call_connect_data_st->local_sip_ip_address),
               p_src + (*p_length), &offset);

        *p_length    += offset;

         icf_unpack_U8(
               (icf_uint8_t*)&(p_call_connect_data_st->call_transport_mode),
                p_src + (*p_length), &offset);

        *p_length += offset;


        return ICF_SUCCESS;
}



icf_uint32_t 
icf_unpack_icf_set_trace_req_st
( 
	icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length )
{
    icf_uint32_t           offset = 0;
    icf_set_trace_req_st  *p_set_trace_req = ICF_NULL;

    p_set_trace_req = (icf_set_trace_req_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;
    /* This function unpacking icf_set_trace_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_trace_req_st \n"));

    icf_unpack_U8((icf_uint8_t *)&(p_set_trace_req->bitmask), 
                 p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_set_trace_req->status), 
			p_src + (*p_length), &offset);
    *p_length    += offset;

    if(p_set_trace_req->bitmask & ICF_TRACE_TYPE_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t *)&(p_set_trace_req->trace_type), 
                 p_src + (*p_length), &offset);

        *p_length       += offset;
    }

    if(p_set_trace_req->bitmask & ICF_TRACE_CRITERIA_PRESENT)
    {
        icf_unpack_U32((icf_uint8_t *)&(p_set_trace_req->trace_criteria), 
                 p_src + (*p_length), &offset);

        *p_length       += offset;
    }

   if(p_set_trace_req->bitmask & ICF_TRACE_RULE_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t *)&(p_set_trace_req->trace_rule), 
                 p_src + (*p_length), &offset);

        *p_length       += offset;
    }
   if(p_set_trace_req->bitmask & ICF_TRACE_DEBUG_TRACE_STATUS_PRESENT)
    {
        icf_unpack_U8((icf_uint8_t *)&(p_set_trace_req->debug_trace_status),
                                       p_src + (*p_length), &offset);

        *p_length       += offset;
    }

    return ICF_SUCCESS;
}

#endif

#ifdef ICF_STAT_ENABLE

/****************************************************************************
*
* FUNCTION: icf_unpack_icf_stats_req_st
*
* DESCRIPTION: This function is used to unpack the payload part of 
*              API ICF_STATS_REQ.
*****************************************************************************/

icf_uint32_t icf_unpack_icf_stats_req_st(icf_uint8_t    *p_dest,
                                            icf_uint8_t    *p_src,
                                            icf_uint32_t    *p_length)
{

        icf_uint32_t       offset = 0;


        icf_stats_req_st  *p_stats_req = ICF_NULL;
        p_stats_req = (icf_stats_req_st *)p_dest;


         /* Initialise length to 0 */
        *p_length    = 0;
        /* This function is for unpacking icf_stats_req_st */
        ICF_CLIB_TRACE(("Unpacking icf_stats_req_st \n"));

        icf_unpack_U8((icf_uint8_t*)&(p_stats_req->reset_stats),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;

        return(ICF_SUCCESS);
}


/****************************************************************************
*
* FUNCTION: icf_unpack_icf_stats_resp_st
*
* DESCRIPTION: This function is used to unpack the payload part of 
*              API ICF_STATS_RESP.
*****************************************************************************/


icf_uint32_t icf_unpack_icf_stats_resp_st(icf_uint8_t    *p_dest,
                             icf_uint8_t    *p_src,
                             icf_uint32_t    *p_length)
{

        icf_uint32_t        offset = 0;

        icf_stats_resp_st  *p_stats_resp = ICF_NULL;
        p_stats_resp = (icf_stats_resp_st *)p_dest;

          /* This function is for unpacking icf_stats_resp_st */
        ICF_CLIB_TRACE(("Unpacking icf_stats_resp_st\n"));

         /* Initialise length to 0 */
        *p_length    = 0;

/* UNPACKING OF TCP CONNECTIONS STAT*/

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
                  stat_tcp_connections.stats_active_conn),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_tcp_connections.total_tcp_conn_since_last_reboot),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_tcp_connections.stats_send_failures),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_tcp_connections.stats_conn_failures),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_tcp_connections.stats_remote_closure_for_local_conn),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;

/* UNPACKING OF UDP CONNECTION STAT*/


     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_udp_connections.stats_send_failures),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;


/* UNPACKING OF TLS CONNECTION STAT*/

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_tls_connections.stats_active_conn),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_tls_connections.total_tls_conn_since_last_reboot),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_tls_connections.stats_send_failures), 
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_tls_connections.stats_conn_failures),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_tls_connections.stats_remote_closure_for_local_conn),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_tls_connections.stats_handshake_errors),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;


/* UNPACKING OF REGISTER STAT*/

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_register.stats_active_reg),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_register.stats_failed_reg),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_register.stats_dereg_done),
                              p_src + (*p_length), &offset);


        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_register.stats_user_registered),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_register.stats_registrar_addr),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_register.stats_rereg_done),
                              p_src + (*p_length), &offset);

        *p_length    += offset;



/*UNPACKING OF REQUEST MESSAGES STAT*/


     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_req.stats_INVITE),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_req.stats_PRACK),
                             p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_UPDATE),
                            p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_req.stats_ReINVITE),
                            p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_INFO),
                            p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_outdlg_OPTION),
                             p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_indlg_OPTION), 
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_CANCEL),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_ACK),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_BYE),
                              p_src + (*p_length), &offset);


        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_REGISTER),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_req.stats_SUBSCRIBE),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_req.stats_PUBLISH),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_req.stats_MESSAGE),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_req.stats_REFER),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_req.stats_NOTIFY),
                               p_src + (*p_length), &offset);

        *p_length    += offset;


     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_retrans_req.stats_INVITE),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_retrans_req.stats_PRACK),
                             p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_retrans_req.stats_UPDATE),
                            p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_retrans_req.stats_INFO),
                            p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_retrans_req.stats_OPTION),
                             p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_retrans_req.stats_CANCEL),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_retrans_req.stats_BYE),
                              p_src + (*p_length), &offset);


        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_retrans_req.stats_REGISTER),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_requests.stat_outbound_retrans_req.stats_SUBSCRIBE),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_retrans_req.stats_PUBLISH),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_retrans_req.stats_REFER),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_retrans_req.stats_NOTIFY),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_outbound_retrans_req.stats_MESSAGE),
                               p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_INVITE),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_PRACK),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_UPDATE),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_requests.stat_inbound_req.stats_ReINVITE),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_requests.stat_inbound_req.stats_INFO),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_MESSAGE),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_outdlg_OPTION),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_indlg_OPTION),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_CANCEL),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_requests.stat_inbound_req.stats_ACK),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_BYE),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_NOTIFY),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_requests.stat_inbound_req.stats_REFER),
                                p_src + (*p_length), &offset);

        *p_length    += offset;


/* PACKING OF STATS OF RESPONSES */


      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_100),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_2xx),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_180),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_181),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_182),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_183),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_3xx),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_486),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        stat_responses.stat_outbound_invite_resp.stats_404),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
       stat_responses.stat_outbound_invite_resp.stats_415),
                               p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
       stat_responses.stat_outbound_invite_resp.stats_407),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
       stat_responses.stat_outbound_invite_resp.stats_401),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_491),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_481),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_402),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_406),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_408),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_409),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_410),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_413),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_482),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_483),
                                p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_489),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_503),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_488),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_480),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_500),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_403),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_487),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_400),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_420),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_423),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_484),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_485),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_501),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_502),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_504), 
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_505), 
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_invite_resp.stats_580),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_600),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

       icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_603),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_604),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;
icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_606),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_rest_4xx),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_rest_5xx),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_invite_resp.stats_rest_6xx),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;





      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_cancel_resp.stats_200),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_cancel_resp.stats_other_resp),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_prack_resp.stats_200),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_prack_resp.stats_other_resp),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_refer_resp.stats_202),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_refer_resp.stats_400),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_refer_resp.stats_481),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_refer_resp.stats_other_resp),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_subscribe_resp.stats_200),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_subscribe_resp.stats_401),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_subscribe_resp.stats_407),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_subscribe_resp.stats_489),
                                        p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_subscribe_resp.stats_other_resp),
                                         p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_info_resp.stats_200),
                                         p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_info_resp.stats_other_resp),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_bye_resp.stats_200),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_bye_resp.stats_other_resp),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_notify_resp.stats_200),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_notify_resp.stats_other_resp),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;



       icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_outbound_update_resp.stats_200),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;

       icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_outbound_update_resp.stats_other_resp),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_register_resp.stats_200),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_register_resp.stats_401),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_register_resp.stats_407),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         stat_responses.stat_outbound_register_resp.stats_other_resp),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;




      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_options_resp.stats_200),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_outbound_options_resp.stats_other_resp),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_publish_resp.stats_200),
                                         p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_publish_resp.stats_other_resp),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_message_resp.stats_200),
                                         p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_outbound_message_resp.stats_other_resp),
                                       p_src + (*p_length), &offset);

        *p_length    += offset;


      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_100),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_2xx),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_180),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_181),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_182),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_183),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_3xx),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_400),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;


      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_486),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_404),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_415),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_407),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_401),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_491),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_481),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_402),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_406),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_408),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_409),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_410),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_413),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_482),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_483),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;
 icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_489),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_503),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_488),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_480),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_500),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_403),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_invite_resp.stats_487),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_420),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_423),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_484),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_485),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_501),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_502),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_invite_resp.stats_504),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_505),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_580),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_600),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_603),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_604),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_606),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_rest_4xx),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_rest_5xx),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_invite_resp.stats_rest_6xx),
                                      p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_cancel_resp.stats_200),
                                     p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_cancel_resp.stats_481),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_cancel_resp.stats_400),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_prack_resp.stats_200),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_prack_resp.stats_500),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_prack_resp.stats_481),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_prack_resp.stats_400),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_refer_resp.stats_202),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_refer_resp.stats_481),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_refer_resp.stats_400),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_refer_resp.stats_403),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_refer_resp.stats_failure_resp),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_notify_resp.stats_200),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
            stat_responses.stat_inbound_notify_resp.stats_481),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_notify_resp.stats_400),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;




      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_info_resp.stats_200),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_info_resp.stats_481),
                                    p_src + (*p_length), &offset);


        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_info_resp.stats_400),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_bye_resp.stats_200),
                                    p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_bye_resp.stats_481),
                                   p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_bye_resp.stats_400),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_update_resp.stats_200),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_update_resp.stats_500),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;


      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_update_resp.stats_481),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_update_resp.stats_400),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_message_resp.stats_200),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_message_resp.stats_500),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;


      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_message_resp.stats_481),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_message_resp.stats_400),
                                 p_src + (*p_length), &offset);

        *p_length    += offset;



      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_options_resp.stats_200),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
           stat_responses.stat_inbound_options_resp.stats_481),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

      icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          stat_responses.stat_inbound_options_resp.stats_other_resp),
                                  p_src + (*p_length), &offset);

        *p_length    += offset;




     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          misc_stats.stats_AAA_DNS_QUERY), 
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          misc_stats.stats_SRV_DNS_QUERY), 
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

     icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         misc_stats.stats_AAA_DNS_QUERY_SUCCESS_RESP), 
                                  p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
          misc_stats.stats_SRV_DNS_SUCCESS_RESP), 
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         misc_stats.stats_AAA_DNS_QUERY_FAILURE_RESP), 
                                 p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
         misc_stats.stats_SRV_DNS_FAILURE_RESP),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

   icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        misc_stats.stats_active_timers),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        misc_stats.inbound_req_405),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

    icf_unpack_U32((icf_uint8_t*)&(p_stats_resp->stats_data.
        misc_stats.outbound_req_405),
                                p_src + (*p_length), &offset);

        *p_length    += offset;

   icf_unpack_icf_time_stamp_st((icf_uint8_t*)&(p_stats_resp->
                 last_reset_time), p_src + (*p_length), &offset);

        *p_length    += offset;

        return ICF_SUCCESS;
}

#endif

/****************************************************************************
*
* FUNCTION: icf_unpack_icf_get_register_status_req_st
*
* DESCRIPTION: This function is used to unpack the payload part of 
*              API ICF_GET_REGISTER_STATUS_REQ.
*****************************************************************************/


icf_uint32_t icf_unpack_icf_get_register_status_req_st(icf_uint8_t    *p_dest,
                                                       icf_uint8_t    *p_src,
                                                       icf_uint32_t   *p_length)
{

        icf_uint32_t       offset = 0;


        icf_get_register_status_req_st  *p_get_register_status_req = ICF_NULL;
        p_get_register_status_req = (icf_get_register_status_req_st *)p_dest;


         /* Initialise length to 0 */
        *p_length    = 0;
        /* This function is for unpacking icf_get_register_status_req_st */
        ICF_CLIB_TRACE(("Unpacking icf_get_register_status_req_st \n"));

        icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_get_register_status_req->
                                 line_id), p_src + (*p_length), &offset);

        *p_length    += offset;

        return(ICF_SUCCESS);
}



/****************************************************************************
*
* FUNCTION: icf_unpack_icf_get_register_status_resp_st
*
* DESCRIPTION: This function is used to unpack the payload part of 
*              API ICF_GET_REGISTER_STATUS_RESP.
*****************************************************************************/



icf_uint32_t icf_unpack_icf_get_register_status_resp_st(icf_uint8_t    *p_dest,
                                                      icf_uint8_t    *p_src,
                                                      icf_uint32_t    *p_length)
{

        icf_uint32_t       offset = 0;


        icf_get_register_status_resp_st  *p_get_register_status_resp = ICF_NULL;
        p_get_register_status_resp = (icf_get_register_status_resp_st *)p_dest;

        /* Initialise length to 0 */
        *p_length    = 0;
        /* This function is for unpacking icf_get_register_status_resp_st */
        ICF_CLIB_TRACE(("Unpacking icf_get_register_status_resp_st \n"));

        icf_unpack_U8((icf_uint8_t*)&
                      (p_get_register_status_resp->result),
                              p_src + (*p_length), &offset);

        *p_length    += offset;

         
        if(p_get_register_status_resp->result == ICF_FAILURE)
         {
           icf_unpack_U16((icf_uint8_t*)&
               (p_get_register_status_resp->error_cause), 
                              p_src + (*p_length), &offset);

           *p_length    += offset;
         }

        else
         {

           icf_unpack_icf_user_reg_status_list_st(
             &(p_get_register_status_resp->p_user_reg_status),
                                p_src + (*p_length), &offset);

           *p_length    += offset;

           icf_unpack_icf_transport_address_st((icf_uint8_t*)&
              (p_get_register_status_resp->registrar_address),
                                   p_src+(*p_length),&offset);

           *p_length    += offset;


           icf_unpack_U32((icf_uint8_t*)&
              (p_get_register_status_resp->expires_duration),
                               p_src + (*p_length), &offset);

           *p_length    += offset;
  
      }

        return (ICF_SUCCESS);
}

icf_uint32_t
icf_unpack_icf_progress_ind_st
(
    icf_uint8_t    	*p_dest,
    icf_uint8_t    	*p_src,
    icf_uint32_t 	*p_length
)
{
    icf_uint32_t        			offset = 0;

    icf_progress_ind_st    			*p_icf_progress_ind_st = ICF_NULL;
    
    p_icf_progress_ind_st = (icf_progress_ind_st *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_progress_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_progress_ind_st\n"));

    icf_unpack_U16(
		(icf_uint8_t *)&(p_icf_progress_ind_st->response_code),
		p_src + (*p_length),
		&offset);
    
    *p_length    += offset;
    
    return ICF_SUCCESS;
}
