/*******************************************************************************
*										
* FILE NAME    :icf_util.c
*										
* DESCRIPTION  :The file contains some utillity functions required by the encoder
*               and decoder functions of the library
*										
* Revision History :				
*				
*    DATE            NAME         REFERENCE       REASON
* --------    ------------        ----         ----------
* 15-JUL-2005   Ram Narayan        None         Initial
* 02-Feb-2006  Amit Sharma                      CFG merged with ICF 2.0
* 07-Mar-2006	Anuradha Gupta	   None			Added code changes for Re-invite handling
* 30-Mar-2006	Anuradha Gupta	   ICF 5.0	Call Modify Feature changes
* 31-Mar-2006   Deepti Goyal       ICF_5_0     	Changes for Message Body Header
* 03-Jan-2007   Amit Mahajan                    changes related to INFO confirmation from app.
* 26-Mar-2007   Deepti Goyal       ICF 6.4      Changes for media connect req API.
* 									
* 11-Apr-2007   Kamini Gangwani    CSR1-4379017/SPR 14897
* 09-Apr-2010  Alok Tiwari        SPR 20362  Modified Function icf_free_msg
*                                 to free remove_hdr_list in options request.
*
* Copyright 2007, Aricent.   					
*										
*******************************************************************************/



#include "icf_clib.h"
#include "icf_clib_mm.h"
#include "icf_api.h"
#include "icf_trace_error_types.h"
#include "icf_utils.h"
#include "icf_utils_mm.h"
#ifdef ICF_PLTFM_TEST_ENABLE
#include "icf_common_types.h"
#endif

#ifdef ICF_STAT_ENABLE
#include "icf_stats_types.h"
#endif
#include "icf_trace_id.h"

icf_return_t icf_make_hdr_node(icf_header_st** start, icf_uint8_t count)
{
    
    icf_header_st* p_hdr = ICF_NULL, 
                  *p_tmp = ICF_NULL;
    
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }

    if (*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_node - Corrupt or Invalid List Start Pointer\n"));
        return ICF_FAILURE;
    }

    while (count--) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_node - Getting memory for header node (p_hdr) %d\n", count));
        p_hdr = ICF_NULL;
        p_hdr = (icf_header_st*)ICF_CLIB_MEMGET(sizeof(icf_header_st));

        if (!p_hdr) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_node - ICF_CLIB_MEMGET Failed for header node (p_hdr)\n"));
            return ICF_FAILURE;
        }

        if (!(*start)) {
            *start = p_hdr;
            p_tmp = p_hdr;
        } 

        p_tmp->next = p_hdr;
        p_hdr = (icf_header_st*)p_hdr->next;
		p_tmp = p_tmp->next;
    }
	p_tmp->next = ICF_NULL;

    return ICF_SUCCESS;
}



icf_return_t icf_make_tag_node(icf_tag_st** start, icf_uint8_t count)
{
    
    icf_tag_st* p_tag = ICF_NULL, 
               *p_tmp = ICF_NULL;
    
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_tag_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }

    if (*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_tag_node - Corrupt or Invalid List Start Pointer\n"));
        return ICF_FAILURE;
    }

    while (count--) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_tag_node - Getting memory for tag node  (p_tag)  %d\n", count));

        p_tag = ICF_NULL;
        p_tag = (icf_tag_st*)ICF_CLIB_MEMGET(sizeof(icf_tag_st));

        if (!p_tag) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_tag_node - ICF_CLIB_MEMGET Failed for tag node (p_tag)\n"));
            return ICF_FAILURE;
        }

        if (!(*start)) {
            *start = p_tag;
            p_tmp = p_tag;
        } 
        p_tmp->next = p_tag;
        p_tag = (icf_tag_st*)p_tag->next;
		p_tmp = p_tmp->next;
    }
	p_tmp->next = ICF_NULL;

    return ICF_SUCCESS;
}


icf_return_t icf_make_msg_body_node(icf_msg_body_st** start, icf_uint8_t count)
{
    
    icf_msg_body_st* p_body = ICF_NULL, 
		           *p_tmp = ICF_NULL;
    
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_msg_body_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }

    if (*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_msg_body_node - Corrupt or Invalid List Start Pointer\n"));
        return ICF_FAILURE;
    }

    while (count--) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_msg_body_node - Getting memory for msg_body node (p_body) %d\n", count));

        p_body = ICF_NULL;
        p_body = (icf_msg_body_st*)ICF_CLIB_MEMGET(sizeof(icf_msg_body_st));

        if (!p_body) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_msg_body_node - ICF_CLIB_MEMGET Failed for body node (p_body)\n"));
            return ICF_FAILURE;
        }

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_msg_body_node - Getting memory for msg_body node (p_body->value) %d\n", count));
        p_body->value = ICF_NULL;
        p_body->value = (icf_uint8_t*)ICF_CLIB_MEMGET(40 * ICF_MAX_STR_LEN);

        if (!p_body->value) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_msg_body_node - ICF_CLIB_MEMGET Failed for Body Value node (p_body->value)\n"));
            return ICF_FAILURE;
        }

        if (!(*start)) {
            *start = p_body;
            p_tmp = p_body;
        } 

        p_tmp->next = p_body;
        p_body = (icf_msg_body_st*)p_body->next;
		p_tmp = p_tmp->next;
    }
	p_tmp->next = ICF_NULL;

    return ICF_SUCCESS;

}


icf_return_t icf_make_hdr_name_node(icf_header_name_st** start, icf_uint8_t count)
{
   

    icf_header_name_st* p_hdr_name = ICF_NULL, 
 	                   *p_tmp = ICF_NULL;
  
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_name_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }
   
    if (*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_name_node - Corrupt or Invalid List Start Pointer\n"));
        return ICF_FAILURE;
    }

    while (count--) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_name_node - Getting memory for header_name node  (p_hdr_name) %d\n", count));

        p_hdr_name = ICF_NULL;
        p_hdr_name = (icf_header_name_st*)ICF_CLIB_MEMGET(sizeof(icf_header_name_st));

        if (!p_hdr_name) {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_hdr_name_node - ICF_CLIB_MEMGET Failed for hdr_name node (p_hdr_name)\n"));
            return ICF_FAILURE;
        }

        if (!(*start)) {
            *start = p_hdr_name;
            p_tmp = p_hdr_name;
        } 

        p_tmp->next = p_hdr_name;
        p_hdr_name = (icf_header_name_st*)p_hdr_name->next;
		p_tmp = p_tmp->next;
    }
	p_tmp->next = ICF_NULL;

    return ICF_SUCCESS;
}

icf_return_t icf_make_string_node(icf_string_list_node_st **p_p_start_node, icf_uint32_t count)
{
   
    icf_string_list_node_st **p_p_curr_node;
    icf_return_t       ret_val = ICF_SUCCESS;
  
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) 
    {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_string_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }
   
    for(p_p_curr_node = p_p_start_node; count--; p_p_curr_node = &((*p_p_curr_node)->p_next)) 
    {
        
        *p_p_curr_node = ICF_NULL; 
        *p_p_curr_node = (icf_string_list_node_st*)ICF_CLIB_MEMGET(sizeof(icf_header_name_st));

        if (ICF_NULL == *p_p_curr_node) 
        {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_string_node - ICF_CLIB_MEMGET Failed \n"));
            ret_val = ICF_FAILURE;
            break;
        }
        (*p_p_curr_node)->p_next = ICF_NULL;
    }
        
    /* We are not freeing the allocaed memory in case of failure 
       as there can be no true recovery if memget fails */
    return ret_val;
}


icf_return_t icf_free_hdr_node(icf_header_st** start, icf_uint8_t count)
{

    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_hdr_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }

    if (!*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_hdr_node - Start of List Is a NULL Pointer\n"));
		return ICF_FAILURE;
    }

    if ((*start)->next) 
		icf_free_hdr_node((icf_header_st**)&((*start)->next), count);

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_hdr_node - Freeing memory for header node\n"));
	ICF_CLIB_MEMFREE(*start);
	*start = ICF_NULL;

    return ICF_SUCCESS;
}



icf_return_t icf_free_tag_node(icf_tag_st** start, icf_uint8_t count)
{

    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_tag_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }

    if (!*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_tag_node - Start of List Is a NULL Pointer\n"));
		return ICF_FAILURE;
    }

    if ((*start)->next) 
		icf_free_tag_node((icf_tag_st**)&((*start)->next), count);


    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_tag_node - Freeing memory for tag node\n"));
	ICF_CLIB_MEMFREE(*start);
	*start = ICF_NULL;

    return ICF_SUCCESS;
}


icf_return_t icf_free_msg_body_node(icf_msg_body_st** start, icf_uint8_t count)
{
    icf_return_t    ret_val=ICF_SUCCESS;
    
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg_body_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }

    if (!*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg_body_node - Start of List Is a NULL Pointer\n"));
		return ICF_FAILURE;
    }
    /*Message body header changes Start*/
    /* Invoke function to free the message body header nodes.
     */
    ret_val = icf_free_hdr_node(&((icf_msg_body_st *)*start)->\
		    msg_body_hdr_list.hdr, (icf_uint8_t)((icf_msg_body_st*)\
		    *start)->msg_body_hdr_list.count);
    /*Message body header changes End*/
    if ((*start)->next) 
		icf_free_msg_body_node((icf_msg_body_st**)&((*start)->next), count);


    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg_body_node - Freeing memory for body node value\n"));
	ICF_CLIB_MEMFREE((*start)->value);

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg_body_node - Freeing memory for body node\n"));
	ICF_CLIB_MEMFREE(*start);
	*start = ICF_NULL;

    return ICF_SUCCESS;
}


icf_return_t icf_free_string_list(icf_string_list_st *p_str_list)
{
    icf_string_list_node_st *p_cur_node,*p_next_node;
    
    p_cur_node = p_str_list->p_list;
    
    while(p_str_list->count-- )
    {
        p_next_node = p_cur_node->p_next;
        ICF_CLIB_MEMFREE(p_cur_node);
        p_cur_node = p_next_node;
    }
    return ICF_SUCCESS;
}

icf_return_t icf_free_hdr_name_node(icf_header_name_st** start, icf_uint8_t count)
{
    
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_hdr_name_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }

    if (!*start) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_hdr_name_node - Start of List Is a NULL Pointer\n"));
		return ICF_FAILURE;
    }

    if ((*start)->next)
		icf_free_hdr_name_node((icf_header_name_st**)&((*start)->next), count);

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg_body_node - Freeing memory for hdr_name node\n"));
	ICF_CLIB_MEMFREE(*start);
	*start = ICF_NULL;

    return ICF_SUCCESS;
}

/*******************************************************************************
 * FUNCTION:        icf_free_app_filter_list
 *
 * DESCRIPTION:     Function to free Internal Filter List strucure 
 *
 ******************************************************************************/

icf_return_t icf_free_app_filter_list(icf_filter_list_st *p_filter_list)
{

	icf_filter_st *p_curr_node;
	icf_filter_st *p_next_node;
	icf_uint8_t i;

	p_curr_node = p_filter_list->p_filter_list;
	
	for(i=0 ; (i<p_filter_list->count) && (ICF_NULL != p_curr_node);i++)
	{
		p_next_node = (icf_filter_st *)p_curr_node->p_next;

		ICF_CLIB_MEMFREE(p_curr_node);
		p_curr_node = p_next_node;
	}
	return ICF_SUCCESS;
}

icf_return_t icf_free_msg(icf_msg_st* p_msg)
{
	/*icf_error_t	ecode = 0;*/
    icf_return_t ret_val = ICF_SUCCESS;
#ifdef ICF_NW_ACTIVATE
	icf_uint8_t num_of_elements;
#endif

	switch (p_msg->hdr.api_id) {
	case ICF_CREATE_CALL_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_CREATE_CALL_REQ\n"));

		if (((icf_create_call_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_create_call_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_create_call_req_st*)p_msg->payload)->header_list.count);

		if (((icf_create_call_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_create_call_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_create_call_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_create_call_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_create_call_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_create_call_req_st*)p_msg->payload)->body_list.count);
		
		if (((icf_create_call_req_st*)p_msg->payload)->bit_mask & ICF_PREF_MEDIA_PROFILE_PRESENT)
			icf_free_icf_stream_capab_list_st(((icf_create_call_req_st*)p_msg->payload)->p_pref_media_profile);

		break;
		
	case ICF_LOCAL_USER_ALERTED_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_LOCAL_USER_ALERTED_REQ\n"));

		if (((icf_local_user_alerted_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_local_user_alerted_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_local_user_alerted_req_st*)p_msg->payload)->header_list.count);

		if (((icf_local_user_alerted_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_local_user_alerted_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_local_user_alerted_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_local_user_alerted_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_local_user_alerted_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)(icf_uint8_t)((icf_local_user_alerted_req_st*)p_msg->payload)->body_list.count);
	
		if (((icf_local_user_alerted_req_st*)p_msg->payload)->bit_mask & ICF_STREAM_LIST_PRESENT)
			icf_free_icf_stream_capab_list_st(((icf_local_user_alerted_req_st*)p_msg->payload)->p_stream_list);

		break;


    case ICF_MEDIA_CONNECT_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_MEDIA_CONNECT_REQ\n"));

		if (((icf_media_connect_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_media_connect_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_media_connect_req_st*)p_msg->payload)->header_list.count);

		if (((icf_media_connect_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_media_connect_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_media_connect_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_media_connect_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_media_connect_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)(icf_uint8_t)((icf_media_connect_req_st*)p_msg->payload)->body_list.count);
	
		if (((icf_media_connect_req_st*)p_msg->payload)->bit_mask & ICF_STREAM_LIST_PRESENT)
			icf_free_icf_stream_capab_list_st(((icf_media_connect_req_st*)p_msg->payload)->p_stream_list);

		break;
					
	case ICF_CONNECT_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_CONNECT_REQ\n"));

		if (((icf_connect_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_connect_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_connect_req_st*)p_msg->payload)->header_list.count);

		if (((icf_connect_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_connect_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_connect_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_connect_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_connect_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_connect_req_st*)p_msg->payload)->body_list.count);
	
		if (((icf_connect_req_st*)p_msg->payload)->bit_mask & ICF_STREAM_LIST_PRESENT)
			icf_free_icf_stream_capab_list_st(((icf_connect_req_st*)p_msg->payload)->p_stream_list);


		break;

	case ICF_INCOMING_CALL_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_INCOMING_CALL_IND\n"));

		if (((icf_incoming_call_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_incoming_call_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_incoming_call_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_incoming_call_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_incoming_call_ind_st*)p_msg->payload)->body_list.body,
            (icf_uint8_t)((icf_incoming_call_ind_st*)p_msg->payload)->body_list.count);

	
		if(ICF_NULL != ((icf_incoming_call_ind_st*)p_msg->payload)->p_accepted_stream_list)
		{
			icf_free_icf_stream_capab_list_st(((icf_incoming_call_ind_st*)p_msg->payload)->p_accepted_stream_list);
		}

		break;

	case ICF_REMOTE_USER_ALERTED_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_REMOTE_USER_ALERTED_IND\n"));

		if (((icf_remote_user_alerted_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_remote_user_alerted_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_remote_user_alerted_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_remote_user_alerted_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_remote_user_alerted_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_remote_user_alerted_ind_st*)p_msg->payload)->body_list.count);

		if(ICF_NULL !=((icf_remote_user_alerted_ind_st*)p_msg->payload)->p_accepted_stream_list)
		{
			icf_free_icf_stream_capab_list_st(((icf_remote_user_alerted_ind_st*)p_msg->payload)->p_accepted_stream_list);
		}

		break;

	case ICF_CONNECT_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_CONNECT_IND\n"));

		if (((icf_connect_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_connect_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_connect_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_connect_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_connect_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_connect_ind_st*)p_msg->payload)->body_list.count);
		
		if(ICF_NULL != ((icf_connect_ind_st*)p_msg->payload)->p_accepted_stream_list)
		{
			icf_free_icf_stream_capab_list_st(((icf_connect_ind_st*)p_msg->payload)->p_accepted_stream_list);
		}

		break;

	case ICF_CONNECT_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_CONNECT_RESP\n"));
        if (((icf_connect_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_connect_resp_st*)p_msg->payload)->header_list.hdr,
					  (icf_uint8_t)((icf_connect_resp_st*)p_msg->payload)->header_list.count);
		if (ICF_NULL != ((icf_connect_resp_st*)p_msg->payload)->p_accepted_stream_list)
			icf_free_icf_stream_capab_list_st(((icf_connect_resp_st*)p_msg->payload)->p_accepted_stream_list);

		break;


/**************************************
/ICF REL 6.0
****************************************/

	case ICF_MEDIA_CONNECTED_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_MEDIA_CONNECTED_IND\n"));
        if (ICF_NULL != ((icf_media_connected_ind_st*)p_msg->payload)->p_accepted_stream_list)
        {
	    	icf_free_icf_stream_capab_list_st(((icf_media_connected_ind_st*)p_msg->payload)->p_accepted_stream_list);
        }

		break;

	case ICF_INCOMING_CALL_MODIFY_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_INCOMING_CALL_IND\n"));

		if (((icf_incoming_call_modify_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
		ret_val = icf_free_hdr_node(&((icf_incoming_call_modify_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_incoming_call_modify_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_incoming_call_modify_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
		ret_val = icf_free_msg_body_node(&((icf_incoming_call_modify_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_incoming_call_modify_ind_st*)p_msg->payload)->body_list.count);
		
		
        if (ICF_NULL != ((icf_incoming_call_modify_ind_st*)p_msg->payload)->p_active_stream_list)
		{
			icf_free_call_modify_media_data_list_st(((icf_incoming_call_modify_ind_st*)p_msg->payload)->p_active_stream_list);
		}
		break;

	case ICF_CALL_MODIFY_CFM:
		ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_CALL_MODIFY_CFM\n"));

        if(((icf_call_modify_cfm_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
        {
		  ret_val = icf_free_hdr_node(&((icf_call_modify_cfm_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_call_modify_cfm_st*)p_msg->payload)->header_list.count);
        }    
		if(((icf_call_modify_cfm_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
	    {
		  ret_val = icf_free_tag_node(&((icf_call_modify_cfm_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_call_modify_cfm_st*)p_msg->payload)->tag_list.count);
        }
        if(((icf_call_modify_cfm_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
        {
		  ret_val = icf_free_msg_body_node(&((icf_call_modify_cfm_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_call_modify_cfm_st*)p_msg->payload)->body_list.count);
        }

        if (((icf_call_modify_cfm_st*)p_msg->payload)->bit_mask &  ICF_STREAM_LIST_PRESENT)
        {     
            icf_free_icf_stream_capab_list_st(((icf_call_modify_cfm_st*)p_msg->payload)->p_stream_list);
        }
		break;

	case ICF_CALL_MODIFY_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_CALL_MODIFY_REQ\n"));
        
        if(((icf_call_modify_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
        {
            ret_val = icf_free_hdr_node(&((icf_call_modify_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_call_modify_req_st*)p_msg->payload)->header_list.count);
        }    
	if(((icf_call_modify_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)	
	{	
			ret_val = icf_free_tag_node(&((icf_call_modify_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_call_modify_req_st*)p_msg->payload)->tag_list.count);
	}

        if(((icf_call_modify_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
        {
		  ret_val = icf_free_msg_body_node(&((icf_call_modify_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_call_modify_req_st*)p_msg->payload)->body_list.count);
        }    

        if(((icf_call_modify_req_st*)p_msg->payload)->bit_mask & ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT)
        icf_free_call_modify_media_data_list_st(((icf_call_modify_req_st*)p_msg->payload)->p_media_modify_list);;
		
        break;

        case ICF_CALL_MODIFY_RESP:
		ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_CALL_MODIFY_RESP\n"));
        if(((icf_call_modify_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
        {
		   ret_val = icf_free_hdr_node(&((icf_call_modify_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_call_modify_resp_st*)p_msg->payload)->header_list.count);
        }    
	    if(((icf_call_modify_resp_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
        {
		   ret_val = icf_free_msg_body_node(&((icf_call_modify_resp_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_call_modify_resp_st*)p_msg->payload)->body_list.count);
        }    
		if(((icf_call_modify_resp_st*)p_msg->payload)->bit_mask & ICF_CALL_MOD_ACCEPT_STREAM_LIST_PRESENT)
			icf_free_call_modify_media_data_list_st(((icf_call_modify_resp_st*)p_msg->payload)->p_active_stream_list);;
		
		break;

/**************************************

****************************************/

	case ICF_REGISTER_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_REGISTER_REQ\n"));  
        
		if (((icf_register_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_register_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_register_req_st*)p_msg->payload)->header_list.count);

		if (((icf_register_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_register_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_register_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_register_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_register_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_register_req_st*)p_msg->payload)->body_list.count);
		break;

	case ICF_REGISTER_CFM:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_REGISTER_CFM\n"));   
        
		if (((icf_register_cfm_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_register_cfm_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_register_cfm_st*)p_msg->payload)->header_list.count);

		if (((icf_register_cfm_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_register_cfm_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_register_cfm_st*)p_msg->payload)->body_list.count);

        if (((icf_register_cfm_st*)p_msg->payload)->bit_mask & ICF_REG_CFM_ASSO_URI_PRESENT)
            icf_free_pub_uri_list(((icf_register_cfm_st*)p_msg->payload)->p_uri_list);

		break;

/* RN*/
	case ICF_REGISTER_STATUS_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_REGISTER_STATUS_IND\n"));  
        
		if (((icf_register_status_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)		
			ret_val = icf_free_hdr_node(&((icf_register_status_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_register_status_ind_st*)p_msg->payload)->header_list.count);

        if (((icf_register_status_ind_st*)p_msg->payload)->bit_mask & ICF_REG_IND_ASSO_URI_PRESENT)
            icf_free_pub_uri_list(((icf_register_status_ind_st *)p_msg->payload)->p_uri_list);

		break;

	case ICF_SUBSCRIBE_REQ:	 
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_SUBSCRIBE_REQ\n"));        
		if (((icf_subscribe_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_subscribe_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_subscribe_req_st*)p_msg->payload)->header_list.count);

		if (((icf_subscribe_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_subscribe_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_subscribe_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_subscribe_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_subscribe_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_subscribe_req_st*)p_msg->payload)->body_list.count);
		break;

	case ICF_SUBSCRIPTION_STATUS_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_SUBSCRIPTION_STATUS_IND\n"));        
		if (((icf_subscription_status_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_subscription_status_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_subscription_status_ind_st*)p_msg->payload)->header_list.count);
		break;

	case ICF_NOTIFY_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_NOTIFY_IND\n"));        
		if (((icf_notify_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_notify_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_notify_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_notify_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_notify_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_notify_ind_st*)p_msg->payload)->body_list.count);
		break;

	case ICF_PUBLISH_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_PUBLISH_REQ\n"));        
		if (((icf_publish_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_publish_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_publish_req_st*)p_msg->payload)->header_list.count);

        if (((icf_publish_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_publish_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_publish_req_st*)p_msg->payload)->tag_list.count);
		
		if (((icf_publish_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_publish_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_publish_req_st*)p_msg->payload)->body_list.count);
		break;

	case ICF_PUBLISH_RESP:	  
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_PUBLISH_RESP\n"));
        
		if (((icf_publish_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_publish_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_publish_resp_st*)p_msg->payload)->header_list.count);
		break;

	case ICF_MESSAGE_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_MESSAGE_REQ\n"));
        
		if (((icf_message_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_message_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_message_req_st*)p_msg->payload)->header_list.count);

		if (((icf_message_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_message_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_message_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_message_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_message_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_message_req_st*)p_msg->payload)->body_list.count);
		break;

	case ICF_MESSAGE_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_MESSAGE_RESP\n"));        
		if (((icf_message_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_message_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_message_resp_st*)p_msg->payload)->header_list.count);
		break;

	case ICF_MESSAGE_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_MESSAGE_IND\n"));
        
		if (((icf_message_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_message_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_message_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_message_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_message_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_message_ind_st*)p_msg->payload)->body_list.count);
		break;

	case ICF_REFER_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_REFER_REQ\n"));
        
		if (((icf_refer_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_refer_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_refer_req_st*)p_msg->payload)->header_list.count);

		if (((icf_refer_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&(((icf_refer_req_st*)p_msg->payload)->tag_list.tag), 
            (icf_uint8_t)((icf_refer_req_st*)p_msg->payload)->tag_list.count);		

		if (((icf_refer_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_refer_req_st*)p_msg->payload)->body_list.body, (icf_uint8_t)((icf_refer_req_st*)p_msg->payload)->body_list.count);
		break;

	case ICF_REFER_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_REFER_RESP\n"));
        
		if (((icf_refer_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_refer_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_refer_resp_st*)p_msg->payload)->header_list.count);
		break;

	case ICF_APP_ADD_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_APP_ADD_REQ\n"));
        
		ret_val = icf_free_hdr_name_node(&((icf_app_add_req_st*)p_msg->payload)->header_name_list.hdr, 
            (icf_uint8_t)((icf_app_add_req_st*)p_msg->payload)->header_name_list.count);
		ICF_CLIB_MEMFREE(((icf_app_add_req_st*)p_msg->payload)->routing_info.ptr);
		if( ICF_NULL != ((icf_app_add_req_st*)p_msg->payload)->filter.p_filter_list)
		{
			if(ICF_FAILURE == 
				icf_free_app_filter_list(
				&((icf_app_add_req_st*)p_msg->payload)->filter))
			{
				ret_val = ICF_FAILURE;
			}
		}
        if(ICF_FAILURE == icf_free_string_list(&((icf_app_add_req_st*)p_msg->payload)->supported_methods))
            ret_val = ICF_FAILURE;
            
        if(ICF_FAILURE == icf_free_string_list(&((icf_app_add_req_st*)p_msg->payload)->supported_events))
            ret_val = ICF_FAILURE;

        if(ICF_FAILURE == icf_free_string_list(&((icf_app_add_req_st*)p_msg->payload)->supported_content_types))
            ret_val = ICF_FAILURE;

        if(ICF_FAILURE == icf_free_string_list(&((icf_app_add_req_st*)p_msg->payload)->supported_encodings))
            ret_val = ICF_FAILURE;

        if(ICF_FAILURE == icf_free_string_list(&((icf_app_add_req_st*)p_msg->payload)->supported_languages))
            ret_val = ICF_FAILURE;
		
		/* we will not free the media profile here, as the memory is being re-used within CFG.
         * the memory will be deleted within ICF/CFG when the application is removed */

		break;
	case ICF_GET_SYSTEM_SETTINGS_REQ:
			
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_GET_SYSTEM_SETTINGS_REQ\n"));        

		ICF_CLIB_MEMFREE(((icf_get_system_settings_req_st*)p_msg->payload)->routing_info.ptr);

        break;
	case ICF_GET_LINE_SETTINGS_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_GET_LINE_SETTINGS_REQ\n"));        
        ICF_CLIB_MEMFREE(((icf_get_line_settings_req_st*)p_msg->payload)->routing_info.ptr);
        break;

#ifdef ICF_NW_ACTIVATE
 	case ICF_NETWORK_ACTIVATE_REQ:

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_NETWORK_ACTIVATE_REQ\n"));        
	    if (((icf_network_activate_req_st*)p_msg->payload)->bit_mask & ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT)
	    {	
	        ICF_CLIB_MEMFREE(((icf_network_activate_req_st*)p_msg->payload)->routing_info.ptr);
	    }        
	    break;

    case ICF_NETWORK_DEACTIVATE_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_NETWORK_DEACTIVATE_REQ\n"));        
        if (((icf_network_deactivate_req_st*)p_msg->payload)->bit_mask & ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT)     	       
	        ICF_CLIB_MEMFREE(((icf_network_deactivate_req_st*)p_msg->payload)->routing_info.ptr);
        break;

	case ICF_NETWORK_ACTIVATE_CFM:
	
	if(((icf_network_activate_cfm_st*)p_msg->payload)->bit_mask & ICF_NET_ACT_CFM_NW_INFO_PRESENT)
	{	
	num_of_elements = ((icf_network_activate_cfm_st*)p_msg->payload)->num_nw;
		while(num_of_elements--)
        	{
			if (ICF_NET_INFO_ASSO_URI_PRESENT & 
				((icf_network_activate_cfm_st*)p_msg->payload)->nw_info[num_of_elements].bit_mask)
			{
				icf_free_pub_uri_list(((icf_network_activate_cfm_st*)p_msg->payload)->nw_info[num_of_elements].p_uri_list);
			}
		}
	}
		break;
	case ICF_NETWORK_DEACTIVATE_CFM:

	if(((icf_network_deactivate_cfm_st*)p_msg->payload)->bit_mask & ICF_NET_DEACT_CFM_NW_INFO_PRESENT)
        { 		

	num_of_elements = ((icf_network_deactivate_cfm_st*)p_msg->payload)->num_nw;
		while(num_of_elements--)
        	{
			if (ICF_NET_INFO_ASSO_URI_PRESENT & 
                                ((icf_network_deactivate_cfm_st*)p_msg->payload)->nw_info[num_of_elements].bit_mask)
			{
			icf_free_pub_uri_list(((icf_network_deactivate_cfm_st*)p_msg->payload)->nw_info[num_of_elements].p_uri_list);
			}
		}
	}
		break;
#endif

    case ICF_GET_SYSTEM_SETTINGS_RESP:
		ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_GET_SYSTEM_SETTINGS_RESP\n"));
		{
			icf_get_system_settings_resp_st *p_resp=
					(icf_get_system_settings_resp_st*)p_msg->payload;
			if(p_resp->apn_list.count >0)
			{
				ICF_CLIB_MEMFREE(p_resp->apn_list.p_list_of_apn);
            }
            if(((icf_get_system_settings_resp_st*)p_msg->payload)->bit_mask & ICF_REQD_ASSO_URI_LIST)
			     icf_free_pub_uri_list(p_resp->p_uri_list);
		}
        break;

#ifdef ICF_NW_ACTIVATE
    case ICF_APP_ADD_CFM: 
      	if (((icf_app_add_cfm_st*)p_msg->payload)->bit_mask & ICF_APP_ADD_CFM_NET_INFO_PRESENT) 
        {
            num_of_elements = ((icf_app_add_cfm_st*)p_msg->payload)->num_nw;
		    while(num_of_elements--)
            {
				if (ICF_APP_ADD_CFM_NET_INFO_PRESENT &
					((icf_app_add_cfm_st*)p_msg->payload)->nw_info[num_of_elements].bit_mask)
				{
					icf_free_pub_uri_list(((icf_app_add_cfm_st*)p_msg->payload)->nw_info[num_of_elements].p_uri_list);
				}
            }
        }
#endif
        break;

    case ICF_CALL_RESUME_RESP:        
    case ICF_CALL_HOLD_RESP:        

		if (((icf_api_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_api_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_api_resp_st*)p_msg->payload)->header_list.count);

		if (((icf_api_resp_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_api_resp_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_api_resp_st*)p_msg->payload)->body_list.count);
		break;

    /*  Changes for REL 7.0 XCONF */
    case  ICF_CREATE_XCONF_REQ:

    	if(((icf_create_ext_xconf_req_st*)p_msg->payload)->bitmask & ICF_HDR_LIST_PRESENT)	
	{
	    ret_val = icf_free_hdr_node(&((icf_create_ext_xconf_req_st*)p_msg->payload)->header_list.hdr,
            (icf_uint8_t)((icf_create_ext_xconf_req_st*)p_msg->payload)->header_list.count);

	}   
	if(((icf_create_ext_xconf_req_st*)p_msg->payload)->bitmask & ICF_TAG_LIST_PRESENT)	
	{
	    ret_val = icf_free_tag_node(&(((icf_create_ext_xconf_req_st*)p_msg->payload)->tag_list.tag),
            (icf_uint8_t)((icf_create_ext_xconf_req_st*)p_msg->payload)->tag_list.count);
	}
	if(((icf_create_ext_xconf_req_st*)p_msg->payload)->bitmask & ICF_CREATE_XCONF_PREF_MEDIA_PROFILE_PRESENT)
	{

	    icf_free_icf_stream_capab_list_st(((icf_create_ext_xconf_req_st*)p_msg->payload)->p_pref_media_profile);

	}	
    	break;

    case ICF_ADD_XCONF_PARTY_REQ:
	
	if(((icf_add_xconf_party_req_st*)p_msg->payload)->bitmask & ICF_HDR_LIST_PRESENT)	
	{
	    ret_val = icf_free_hdr_node(&((icf_add_xconf_party_req_st*)p_msg->payload)->header_list.hdr,
            (icf_uint8_t)((icf_add_xconf_party_req_st*)p_msg->payload)->header_list.count);

	}   
	if(((icf_add_xconf_party_req_st*)p_msg->payload)->bitmask & ICF_TAG_LIST_PRESENT)	
	{
	    ret_val = icf_free_tag_node(&(((icf_add_xconf_party_req_st*)p_msg->payload)->tag_list.tag),
            (icf_uint8_t)((icf_add_xconf_party_req_st*)p_msg->payload)->tag_list.count);
	}
	break;

    case ICF_DELETE_XCONF_PARTY_REQ:
	
	if(((icf_delete_xconf_party_req_st*)p_msg->payload)->bitmask & ICF_HDR_LIST_PRESENT)	
	{
	    ret_val = icf_free_hdr_node(&((icf_delete_xconf_party_req_st*)p_msg->payload)->header_list.hdr,
            (icf_uint8_t)((icf_delete_xconf_party_req_st*)p_msg->payload)->header_list.count);

	}   
	if(((icf_delete_xconf_party_req_st*)p_msg->payload)->bitmask & ICF_TAG_LIST_PRESENT)	
	{
	    ret_val = icf_free_tag_node(&(((icf_delete_xconf_party_req_st*)p_msg->payload)->tag_list.tag),
            (icf_uint8_t)((icf_delete_xconf_party_req_st*)p_msg->payload)->tag_list.count);
	}
	break;

    case ICF_RELEASE_XCONF_REQ:
	
	if(((icf_release_xconf_req_st*)p_msg->payload)->bitmask & ICF_HDR_LIST_PRESENT)	
	{
	    ret_val = icf_free_hdr_node(&((icf_release_xconf_req_st*)p_msg->payload)->header_list.hdr,
            (icf_uint8_t)((icf_release_xconf_req_st*)p_msg->payload)->header_list.count);

	}   
	if(((icf_release_xconf_req_st*)p_msg->payload)->bitmask & ICF_TAG_LIST_PRESENT)	
	{
	    ret_val = icf_free_tag_node(&(((icf_release_xconf_req_st*)p_msg->payload)->tag_list.tag),
            (icf_uint8_t)((icf_release_xconf_req_st*)p_msg->payload)->tag_list.count);
	}
	break;

       /*  REL 7.0 XCONF END*/
	
    case ICF_CALL_TRANSFER_UNATTENDED_REQ:

        if (((icf_call_transfer_unattended_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
            ret_val = icf_free_hdr_node(&((icf_call_transfer_unattended_req_st*)p_msg->payload)->header_list.hdr,
            (icf_uint8_t)((icf_call_transfer_unattended_req_st*)p_msg->payload)->header_list.count);

        if (((icf_call_transfer_unattended_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
            ret_val = icf_free_tag_node(&(((icf_call_transfer_unattended_req_st*)p_msg->payload)->tag_list.tag),
            (icf_uint8_t)((icf_call_transfer_unattended_req_st*)p_msg->payload)->tag_list.count);	
        if (((icf_call_transfer_unattended_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
            ret_val = icf_free_msg_body_node(&((icf_call_transfer_unattended_req_st *)p_msg->payload)->body_list.body,
            (icf_uint8_t)((icf_call_transfer_unattended_req_st*)p_msg->payload)->body_list.count);

	    if (((icf_call_transfer_unattended_req_st*)p_msg->payload)->bit_mask & ICF_ADDRESS_ADDNL_URI_PARAMTR_PRES)
        {
	       ICF_CLIB_MEMFREE(((icf_call_transfer_unattended_req_st*)p_msg->payload)->p_uri_parameter);
        }
	    break;

	case ICF_CALL_TRANSFER_UNATTENDED_RESP:
        if (((icf_ctu_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_ctu_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_ctu_resp_st*)p_msg->payload)->header_list.count);
        break;
	case ICF_CALL_TRANSFER_ATTENDED_REQ:

    if (((icf_call_transfer_attended_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
        ret_val = icf_free_hdr_node(&((icf_call_transfer_attended_req_st*)p_msg->payload)->header_list.hdr,
        (icf_uint8_t)((icf_call_transfer_attended_req_st*)p_msg->payload)->header_list.count);

    if (((icf_call_transfer_attended_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
        ret_val = icf_free_tag_node(&(((icf_call_transfer_attended_req_st*)p_msg->payload)->tag_list.tag),
        (icf_uint8_t)((icf_call_transfer_attended_req_st*)p_msg->payload)->tag_list.count);	
    if (((icf_call_transfer_attended_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
        ret_val = icf_free_msg_body_node(&((icf_call_transfer_attended_req_st *)p_msg->payload)->body_list.body,
        (icf_uint8_t)((icf_call_transfer_attended_req_st*)p_msg->payload)->body_list.count);
     break;
    case ICF_CALL_TRANSFER_ATTENDED_RESP:
        if (((icf_cta_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_cta_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_cta_resp_st*)p_msg->payload)->header_list.count);
        break;

    case ICF_REMOTE_CALL_TRANSFER_INITIATED_IND:
        if (((icf_remote_call_transfer_initiated_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_remote_call_transfer_initiated_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_remote_call_transfer_initiated_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_remote_call_transfer_initiated_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_remote_call_transfer_initiated_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_remote_call_transfer_initiated_ind_st*)p_msg->payload)->body_list.count);
        break;
    case ICF_INFO_REQ:
        if (((icf_info_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_info_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_info_req_st*)p_msg->payload)->header_list.count);

        if (((icf_info_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&(((icf_info_req_st*)p_msg->payload)->tag_list.tag), 
            (icf_uint8_t)((icf_info_req_st*)p_msg->payload)->tag_list.count);		

		if (((icf_info_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_info_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_info_req_st*)p_msg->payload)->body_list.count);
        break;

	case ICF_INFO_RESP:
        if (((icf_info_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_info_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_info_resp_st*)p_msg->payload)->header_list.count);

		if (((icf_info_resp_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_info_resp_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_info_resp_st*)p_msg->payload)->body_list.count);
        break;

	case ICF_INFO_IND:
		if (((icf_info_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_info_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_info_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_info_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_info_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_info_ind_st*)p_msg->payload)->body_list.count);
		break;

    case ICF_CALL_HOLD_IND:        
    case ICF_CALL_RESUME_IND:
        if (((icf_api_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_api_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_api_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_api_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_api_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_api_ind_st*)p_msg->payload)->body_list.count);
		break;
        
    case ICF_CALL_REDIRECTION_IND:

        break;

    case ICF_CALL_HOLD_REQ:
        if (((icf_call_hold_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_call_hold_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_call_hold_req_st*)p_msg->payload)->header_list.count);

        if (((icf_call_hold_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&(((icf_call_hold_req_st*)p_msg->payload)->tag_list.tag), 
            (icf_uint8_t)((icf_call_hold_req_st*)p_msg->payload)->tag_list.count);		

		if (((icf_call_hold_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_call_hold_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_call_hold_req_st*)p_msg->payload)->body_list.count);
        break;

    case ICF_CALL_RESUME_REQ:
        if (((icf_call_resume_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_call_resume_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_call_resume_req_st*)p_msg->payload)->header_list.count);

        if (((icf_call_resume_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&(((icf_call_resume_req_st*)p_msg->payload)->tag_list.tag), 
            (icf_uint8_t)((icf_call_resume_req_st*)p_msg->payload)->tag_list.count);		

		if (((icf_call_resume_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_call_resume_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_call_resume_req_st*)p_msg->payload)->body_list.count);
        break;

    case ICF_CONFERENCE_REQ:
        if (((icf_conference_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_conference_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_conference_req_st*)p_msg->payload)->header_list.count);

        if (((icf_conference_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&(((icf_conference_req_st*)p_msg->payload)->tag_list.tag), 
            (icf_uint8_t)((icf_conference_req_st*)p_msg->payload)->tag_list.count);		

        break;

	case ICF_OPTIONS_REQ:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_OPTIONS_REQ\n"));
        
		if (((icf_options_req_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_options_req_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_options_req_st*)p_msg->payload)->header_list.count);

		if (((icf_options_req_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_options_req_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_options_req_st*)p_msg->payload)->tag_list.count);

		if (((icf_options_req_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_options_req_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_options_req_st*)p_msg->payload)->body_list.count);

        /*Fix for CSR 1-8223807 : SPR-20362*/
        if (((icf_options_req_st*)p_msg->payload)->bit_mask & 
                                       ICF_OPTIONS_REQ_REMOVE_HDR_LIST_PRESENT)
        {
            ret_val = icf_free_hdr_name_node(
               &((icf_options_req_st*)p_msg->payload)->remove_hdr_list.hdr, 
               (icf_uint8_t)((icf_options_req_st*)p_msg->payload)->remove_hdr_list.count);
        }

		break;

	case ICF_OPTIONS_RESP:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_OPTIONS_RESP\n"));
		
		if (((icf_options_resp_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_options_resp_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_options_resp_st*)p_msg->payload)->header_list.count);

		if (((icf_options_resp_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_options_resp_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_options_resp_st*)p_msg->payload)->body_list.count);

		if (((icf_options_resp_st*)p_msg->payload)->bit_mask & ICF_OPTIONS_REMOTE_STREAM_LIST_PRESENT)
			icf_free_icf_stream_capab_list_st(
			((icf_options_resp_st*)p_msg->payload)->p_remote_stream_list);

		/* if (((icf_options_resp_st*)p_msg->payload)->bit_mask & ICF_STREAM_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_options_resp_st*)p_msg->payload)->stream_list.tag, 
            (icf_uint8_t)((icf_options_resp_st*)p_msg->payload)->stream_list.count); */
		break;
	case ICF_INC_OPTIONS_IND:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_INC_OPTIONS_IND\n"));
        
		if (((icf_inc_options_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_inc_options_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_inc_options_ind_st*)p_msg->payload)->header_list.count);

		if (((icf_inc_options_ind_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_inc_options_ind_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_inc_options_ind_st*)p_msg->payload)->tag_list.count);

		if (((icf_inc_options_ind_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_inc_options_ind_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_inc_options_ind_st*)p_msg->payload)->body_list.count);
		if (((icf_inc_options_ind_st*)p_msg->payload)->bit_mask & ICF_INC_OPTIONS_IND_MEDIA_PRESENT)
			icf_free_icf_stream_capab_list_st(((icf_inc_options_ind_st*)p_msg->payload)->p_app_sdp_list);
		break;
	case ICF_OPTIONS_CFM:
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_msg - ICF_OPTIONS_CFM\n"));
        
		if (((icf_options_cfm_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_options_cfm_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_options_cfm_st*)p_msg->payload)->header_list.count);

		if (((icf_options_cfm_st*)p_msg->payload)->bit_mask & ICF_TAG_LIST_PRESENT)
			ret_val = icf_free_tag_node(&((icf_options_cfm_st*)p_msg->payload)->tag_list.tag, 
            (icf_uint8_t)((icf_options_cfm_st*)p_msg->payload)->tag_list.count);

		if (((icf_options_cfm_st*)p_msg->payload)->bit_mask & ICF_BODY_LIST_PRESENT)
			ret_val = icf_free_msg_body_node(&((icf_options_cfm_st*)p_msg->payload)->body_list.body, 
            (icf_uint8_t)((icf_options_cfm_st*)p_msg->payload)->body_list.count);
		if (((icf_options_cfm_st*)p_msg->payload)->bit_mask & ICF_OPTIONS_CFM_MEDIA_PRESENT)
			icf_free_icf_stream_capab_list_st(((icf_options_cfm_st*)p_msg->payload)->p_app_sdp_list);
		break;


       case ICF_RECV_UDP_DATA_IND:
            ICF_CLIB_MEMFREE(((icf_recv_udp_data_ind_st *)p_msg->payload)->p_app_data);
        break;
		
       case ICF_SEND_UDP_DATA_REQ:
            ICF_CLIB_MEMFREE(((icf_send_udp_data_req_st *)p_msg->payload)->p_app_data);
        break;
       case ICF_EARLY_INC_CALL_IND :
        	if (((icf_early_incoming_call_ind_st*)p_msg->payload)->bit_mask & ICF_HDR_LIST_PRESENT)
			ret_val = icf_free_hdr_node(&((icf_early_incoming_call_ind_st*)p_msg->payload)->header_list.hdr, 
            (icf_uint8_t)((icf_early_incoming_call_ind_st*)p_msg->payload)->header_list.count);
        break;
       case ICF_GET_REGISTER_STATUS_RESP:
            icf_free_user_reg_status_st(((icf_get_register_status_resp_st*)p_msg              ->payload)->p_user_reg_status);

            break;

       case ICF_SET_LINE_PARAM_REQ:
             if (((icf_set_line_param_req_st*)p_msg->payload)->bitmask & ICF_SET_LINE_AUDIO_CODEC)
             {
                icf_free_codec_attrib_st(((icf_set_line_param_req_st*)p_msg->payload)->p_codec_attributes_audio);
             }
 
              if (((icf_set_line_param_req_st*)p_msg->payload)->bitmask & ICF_SET_LINE_VIDEO_CODEC)
             {
                icf_free_codec_attrib_st(((icf_set_line_param_req_st*)p_msg->payload)->p_codec_attributes_video  );
             }

          break;
        #ifdef ICF_TRACE_ENABLE
        case ICF_REPORT_TRACE_IND:
           {
               icf_trace_data_st *p_trace_var_data =
                   &(((icf_report_trace_ind_st*)
                           p_msg->payload)->trace_data);

           switch(((icf_report_trace_ind_st*)p_msg->payload)->trace_id)
           {
           case ICF_TRACE_MEDIA_CAPABILITY:
           case ICF_TRACE_CODEC_RESERVE:
           case ICF_TRACE_STREAM_CREATE:
           case ICF_TRACE_STREAM_MODIFY:
           case ICF_TRACE_CODEC_RELEASE:
            {
               icf_list_st *p_codec_info = (icf_list_st *)p_trace_var_data->p_var_data;

                if(ICF_NULL != p_codec_info)
		           icf_free_codec_attrib_st(p_codec_info);
             
           }
           break;
       
           case ICF_TRACE_APP_MESG:
           case ICF_TRACE_MEDIA_MESG:
           case ICF_TRACE_SIP_PDU:
           {
              icf_generic_string_st *p_msg_data = (icf_generic_string_st *)
                        p_trace_var_data->p_var_data;

              if(ICF_NULL != p_msg_data)
              {
                 if(ICF_NULL != p_msg_data->p_buff)
                    ICF_CLIB_MEMFREE(p_msg_data->p_buff);
                 ICF_CLIB_MEMFREE(p_msg_data);
              }
           }
           break;
	
        case ICF_TRACE_CODEC_COMMIT:
          {
              icf_codec_attrib_st *p_codec = 
                      (icf_codec_attrib_st *)p_trace_var_data->p_var_data;

              ICF_CLIB_MEMFREE(p_codec);
          }
          break;

        case ICF_TRACE_LOCAL_SDP_OFFER:
        case ICF_TRACE_REMOTE_SDP_OFFER:
        case ICF_TRACE_REMOTE_SDP_ANSWER:
        case ICF_TRACE_LOCAL_SDP_ANSWER:
           {
                icf_list_st *p_sdp_list = (icf_list_st *)p_trace_var_data->p_var_data;

                icf_free_call_modify_media_data_list_st(p_sdp_list);
           }
           break;

        case ICF_TRACE_CALL_CONNECTED:
          {
              icf_call_connect_data_st *p_call_data = (icf_call_connect_data_st *)
                             p_trace_var_data->p_var_data;

              /* check for stream data list */
              icf_list_st *p_stream_data = p_call_data->p_stream_data;
              icf_list_st *p_temp_list =ICF_NULL;

              while(ICF_NULL != p_stream_data)
              {
                  icf_stream_data_st *p_data = 
                     (icf_stream_data_st *)p_stream_data->p_data;
                   
                  if(ICF_NULL != p_data->p_offered_codec)
                   icf_free_codec_attrib_st(p_data->p_offered_codec);

                  ICF_CLIB_MEMFREE(p_data->p_negotiated_codec);
                  ICF_CLIB_MEMFREE(p_data);

                  p_temp_list = p_stream_data;
                  p_stream_data = p_stream_data->p_next;
                  ICF_CLIB_MEMFREE(p_temp_list);
              }
              ICF_CLIB_MEMFREE(p_call_data);
          }
          break;
	
	default:
        break;
	}
        break;
    }
#endif
}

    if (ICF_MSG_FRM_PLTFM != p_msg->hdr.api_id)
        ICF_CLIB_MEMFREE(p_msg);

    return ret_val;
}




icf_uint32_t icf_get_mm_api_length(icf_uint16_t api_id,icf_boolean_t *p_is_valid_api)
{

    icf_uint32_t api_length = 0;
    /********************************************/
    /* ICF_NS_MEDIA_PLANE_API_START */
	switch (api_id) {
	
    case ICF_CFG_CODEC_INFO_REQ:
        api_length = sizeof(icf_cfg_codec_info_req_st);
        break;

    case ICF_CFG_RINGER_TONES_REQ:
        api_length = sizeof(icf_cfg_ringer_tones_req_st);
        break;

#ifdef ICF_REL_OLD    
    case ICF_CFG_TONES_REQ:
        api_length = sizeof(icf_cfg_tones_req_st);
        break;
#endif

    case ICF_CFG_JBM_MGMT_REQ:
        api_length = sizeof(icf_cfg_jbm_mgmt_req_st);
        break;

    case ICF_CFG_MAX_EARP_VOL_REQ:
        api_length = sizeof(icf_cfg_max_earp_vol_req_st);
        break;

    case ICF_CFG_MAX_SPEAKER_VOL_REQ:
        api_length = sizeof(icf_cfg_max_speaker_vol_req_st);
        break;

    case ICF_CFG_SET_PORT_RANGE_REQ:
        api_length = sizeof(icf_cfg_set_port_range_req_st);
        break;

    case ICF_CFG_SET_SELF_IP_REQ:
        api_length = sizeof(icf_cfg_set_self_ip_req_st);
        break;

    case ICF_CFG_SET_DIGIT_TRANS_REQ:
        api_length = sizeof(icf_cfg_set_digit_trans_req_st);
        break;
/*ICF_Rel 6.0*/
	case ICF_CFG_MEDIA_REQ:
        api_length = sizeof(icf_cfg_media_req_st);
        break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
        api_length = sizeof(icf_cfg_set_remote_video_display_st);
        break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
        api_length = sizeof(icf_cfg_set_preview_video_display_st);
        break;

	case ICF_PLAY_MEDIA_REQ:
        api_length = sizeof(icf_play_media_req_st);
        break;

	case ICF_STOP_MEDIA_PLAY_REQ:
        api_length = sizeof(icf_stop_media_play_req_st);
        break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
        api_length = sizeof(icf_set_media_playback_device_req_st);
        break;

	case ICF_CONNECT_MEDIA_DEVICE_REQ:
        api_length = sizeof(icf_connect_media_device_req_st);
        break;

	case ICF_MEDIA_RECORD_REQ:
        api_length = sizeof(icf_media_record_req_st);
        break;

	case ICF_STOP_MEDIA_RECORD_REQ:
        api_length = sizeof(icf_stop_media_record_req_st);
        break;

	case ICF_PAUSE_MEDIA_REQ:
        api_length = sizeof(icf_pause_media_req_st);
        break;

	case ICF_OPEN_MEDIA_CHANNEL_REQ:
        api_length = sizeof(icf_open_media_channel_req_st);
        break;

	case ICF_CLOSE_MEDIA_CHANNEL_REQ:
        api_length = sizeof(icf_close_media_channel_req_st);
        break;

	case ICF_SEND_UDP_DATA_REQ:
        api_length = sizeof(icf_send_udp_data_req_st);
        break;

    case ICF_START_RINGING_REQ:
        api_length = sizeof(icf_start_ringing_req_st);
        break;

    case ICF_VIBRATION_REQ:
        api_length = sizeof(icf_vibration_req_st);
        break;

#ifdef ICF_REL_OLD
    case ICF_MEDIA_SESSION_REQ:
        api_length = sizeof(icf_media_session_req_st);
        break;
#endif

    case ICF_ECHO_CANC_REQ:
        api_length = sizeof(icf_echo_canc_req_st);
        break;
    case ICF_VAD_SS_CNG_REQ:
        api_length = sizeof(icf_vad_ss_cng_req_st);
        break;
    
    case ICF_GET_STATS_REQ:
        api_length = sizeof(icf_get_stats_req_st);
        break;
#ifdef ICF_REL_OLD
    case ICF_PLAY_FILE_REQ:
        api_length = sizeof(icf_play_file_req_st);
        break;

    case ICF_PLAY_FILE_RESP:
        api_length = sizeof(icf_play_file_resp_st);
        break;

    case ICF_PLAY_TONE_REQ:
        api_length = sizeof(icf_play_tone_req_st);
        break;

    case ICF_STOP_TONE_REQ:
        api_length = sizeof(icf_stop_tone_req_st);
        break;
    
    case ICF_STOP_FILE_REQ:
        api_length = sizeof(icf_stop_file_req_st);
        break;

    case ICF_STOP_PLAY_FILE_IND:
        api_length = sizeof(icf_stop_media_play_ind_st);
        break;
#endif

    case ICF_STOP_RINGING_REQ:
        api_length = 0;
        break;

    case ICF_STOP_VIBRATION_REQ:
        api_length = 0;
        break;

#ifdef ICF_REL_OLD
    case ICF_STOP_TONE_IND:
        api_length = sizeof(icf_stop_tone_ind_st);
        break;
#endif

    case ICF_STOP_RINGING_IND:
        api_length = sizeof(icf_stop_ringing_ind_st);
        break;

    case ICF_MUTE_MEDIA_REQ:
        api_length = sizeof(icf_mute_media_req_st);
        break;

#ifdef ICF_REL_OLD
    case ICF_PLAY_MEDIA_SPKR_REQ:
        api_length = sizeof(icf_play_media_spkr_req_st);
        break;

    case ICF_RECORD_VOICE_REQ:
        api_length = sizeof(icf_record_voice_req_st);
        break;
#endif

    case ICF_SEND_DGTS_REQ:
        api_length = sizeof(icf_send_dgts_st);
        break;

    case ICF_SET_EARP_VOLUME_REQ:
        api_length = sizeof(icf_set_earp_volume_req_st);
        break;

    case ICF_SET_SPEAKER_VOLUME_REQ:
        api_length = sizeof(icf_set_speaker_volume_req_st);
        break;

#ifdef ICF_REL_OLD
    case ICF_STOP_RECORD_VOICE_REQ:
        api_length = sizeof(icf_stop_record_voice_req_st);
        break;
#endif

    case ICF_SEND_APP_DATA_REQ:
        api_length = sizeof(icf_send_app_data_req_st);
        break;

    case ICF_RCV_APP_DATA_IND:
        api_length = sizeof(icf_rcv_app_data_ind_st);
        break;

    case ICF_SEND_STATS_REQ:
        api_length = sizeof(icf_send_stats_req_st);
        break;

    case ICF_STOP_STATS_REQ:
        api_length = sizeof(icf_stop_stats_req_st);
        break;

    case ICF_CFG_MAX_MIC_GAIN_REQ:
        api_length = sizeof(icf_cfg_max_mic_gain_req_st);
        break;

    case ICF_SET_MIC_GAIN_REQ:
        api_length = sizeof(icf_set_mic_gain_req_st);
        break;

    case ICF_RTP_START_IND:
        api_length = sizeof(icf_rtp_start_ind_st);
        break;

	case ICF_STOP_MEDIA_PLAY_IND:
        api_length = sizeof(icf_stop_media_play_ind_st);
        break;

	case ICF_MUTE_MEDIA_RESP:
        api_length = sizeof(icf_mute_media_resp_st);
        break;

	case ICF_OPEN_MEDIA_CHANNEL_RESP:
        api_length = sizeof(icf_open_media_channel_resp_st);
        break;

	case ICF_RECV_UDP_DATA_IND:
        api_length = sizeof(icf_recv_udp_data_ind_st);
        break;
	
	case ICF_CFG_SET_VIDEO_ATTRIB_REQ:
	api_length = sizeof(icf_cfg_set_video_attrib_st);
	break;

    case ICF_CFG_CODEC_INFO_RESP:
    case ICF_CFG_RINGER_TONES_RESP:
#ifdef ICF_REL_OLD 
    case ICF_CFG_TONES_RESP:
#endif
    case ICF_CFG_JBM_MGMT_RESP:
    case ICF_CFG_MAX_EARP_VOL_RESP:
    case ICF_CFG_MAX_SPEAKER_VOL_RESP:
    case ICF_CFG_SET_SELF_IP_RESP:
    case ICF_CFG_MAX_MIC_GAIN_RESP:
    case ICF_CFG_SET_DIGIT_TRANS_RESP:
    case ICF_START_RINGING_RESP:
    case ICF_VIBRATION_RESP:
    case ICF_MEDIA_SESSION_RESP:
    case ICF_ECHO_CANC_RESP:
    case ICF_VAD_SS_CNG_RESP:
    case ICF_GET_STATS_RESP:
#ifdef ICF_REL_OLD
    case ICF_PLAY_TONE_RESP:
    case ICF_STOP_TONE_RESP:
    case ICF_STOP_FILE_RESP:
#endif
    case ICF_STOP_RINGING_RESP:
    case ICF_STOP_VIBRATION_RESP:
    case ICF_DGTS_IND:
#ifdef ICF_REL_OLD
    case ICF_PLAY_MEDIA_SPKR_RESP:
#endif

#ifdef ICF_REL_OLD
    case ICF_RECORD_VOICE_RESP:
#endif

    case ICF_SEND_DGTS_RESP:
    case ICF_SET_EARP_VOLUME_RESP:
    case ICF_SET_SPEAKER_VOLUME_RESP:

#ifdef ICF_REL_OLD
    case ICF_STOP_RECORD_VOICE_RESP:
#endif

    case ICF_SEND_APP_DATA_RESP:
    case ICF_SEND_STATS_RESP:
    case ICF_STOP_STATS_RESP:
	case ICF_CFG_MEDIA_RESP:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_PORT_RANGE_RESP:
	case ICF_PLAY_MEDIA_RESP:
	case ICF_STOP_MEDIA_PLAY_RESP:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
	case ICF_CONNECT_MEDIA_DEVICE_RESP:
	case ICF_MEDIA_RECORD_RESP:
	case ICF_STOP_MEDIA_RECORD_RESP:
	case ICF_PAUSE_MEDIA_RESP:
	case ICF_CFG_SET_VIDEO_ATTRIB_RESP:
	case ICF_MM_INIT_COMPLETE:
		api_length = sizeof(icf_msg_resp_st);
        break;

    case ICF_SET_MIC_GAIN_RESP:
        api_length = sizeof(icf_msg_resp_st);
        break;


	/* ICF_NS_MEDIA_PLANE_API_END */
    /********************************************/


    /********************************************/
    /* APIs between ICF and MM - ICF_CC_MM_API_START */

    case ICF_GET_CODEC_LIST_REQ:
        api_length = sizeof(icf_get_codec_list_req_st);
        break;

    case ICF_GET_CODEC_LIST_RESP:
        api_length = sizeof(icf_get_codec_list_resp_st);
        break;
        
    case ICF_CODEC_COMMIT_REQ:
        api_length = sizeof(icf_codec_commit_req_st);
        break;

    case ICF_REL_RES_REQ:
        api_length = sizeof(icf_rel_res_req_st);
        break;

    case ICF_MEDIA_CAPABILITIES_REQ:
        api_length = sizeof(icf_media_capabilities_req_st);
        break;
        
    case ICF_MEDIA_CAPABILITIES_RESP:
        api_length = sizeof(icf_media_capabilities_resp_st);
        break;

    case ICF_CREATE_MEDIA_SESSION_REQ:
        api_length = sizeof(icf_create_media_session_req_st);
        break;

    case ICF_CREATE_MEDIA_SESSION_RESP:
        api_length = sizeof(icf_create_media_session_resp_st);
        break;
        
    case ICF_MODIFY_MEDIA_SESSION_REQ:
        api_length = sizeof(icf_modify_media_session_req_st);
        break;

    case ICF_MERGE_MEDIA_SESSION_REQ:
        api_length = sizeof(icf_merge_media_session_req_st);
        break;

    case ICF_DELETE_MEDIA_SESSION_IND:
        api_length = sizeof(icf_delete_media_session_ind_st);
        break;

    case ICF_DELETE_MEDIA_SESSION_REQ:
        api_length = sizeof(icf_delete_media_session_req_st);
        break;

    case ICF_MM_UNEXPECTED_MSG_IND:
        api_length = sizeof(icf_mm_unexpected_msg_ind_st);
        break;

#ifdef ICF_REL_OLD
    case ICF_REL_RES_RESP:
#endif

    case ICF_CODEC_COMMIT_RESP:
		api_length = sizeof(icf_codec_commit_resp_st);
        break;

    case ICF_MODIFY_MEDIA_SESSION_RESP:
		 api_length = sizeof(icf_modify_media_session_resp_st);
		break;

    case ICF_MERGE_MEDIA_SESSION_RESP:
    case ICF_DELETE_MEDIA_SESSION_RESP:
        api_length = sizeof(icf_msg_resp_st);
        break;
    /********************************************/
#ifdef ICF_PLTFM_TEST_ENABLE
/***************************************
*	       PLATFORM APIs
****************************************/
#ifdef ICF_NW_ACTIVATE
	case ICF_GPRS_SEC_PDP_ACTIVATE_REQ:
		api_length = sizeof(icf_gprs_sec_pdp_activate_req_st);
		break;

	case ICF_GPRS_PDP_DEACTIVATE_REQ:
		api_length = sizeof(icf_gprs_pdp_deactivate_req_st);
		break;
#endif

	case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP:
		api_length = sizeof(icf_port_pltfm_pdp_def_resp_st);
		break;

	case ICF_PORT_PLTFM_PDP_MODIFY_IND:
		api_length = sizeof(icf_port_pltfm_pdp_mod_ind_st);
		break;

	case ICF_PORT_PLTFM_PDP_DEACT_IND:
		api_length = sizeof(icf_port_pltfm_pdp_deact_ind_st);
		break;

	case ICF_PORT_PLTFM_PLATFORM_EVENT_IND:
		api_length = sizeof(icf_port_pltfm_event_ind_st);
		break;

#endif

	case ICF_MM_SET_NW_STATUS:
		api_length = sizeof(icf_nw_status_st);
		break;

    default:
		*p_is_valid_api = ICF_FALSE;
		/* This should be 0 */
		/* api_length = sizeof(icf_uint32_t);*/
		 break;
	}
	return api_length;
}



icf_uint32_t icf_get_api_length(icf_uint16_t api_id)
{

    icf_uint32_t api_length = 0;
	icf_boolean_t is_valid_api = ICF_TRUE;
	switch (api_id) {
	case ICF_APP_ADD_REQ:	
		api_length = sizeof(icf_app_add_req_st);
        break;

	case ICF_GET_SYSTEM_SETTINGS_REQ:
		api_length = sizeof(icf_get_system_settings_req_st);
		break;
	
	case ICF_GET_SYSTEM_SETTINGS_RESP:
		api_length = sizeof(icf_get_system_settings_resp_st);
		break;

	case ICF_GET_LINE_SETTINGS_REQ:
		api_length = sizeof(icf_get_line_settings_req_st);
		break;
	
	case ICF_GET_LINE_SETTINGS_RESP:
		api_length = sizeof(icf_get_line_settings_resp_st);
		break;

	
	case ICF_APP_ADD_CFM:											
		api_length = sizeof(icf_app_add_cfm_st);
        break;

	case ICF_SET_TRANSPORT_REQ:									
		api_length = sizeof(icf_set_transport_req_st);
        break;

	case ICF_SET_CALL_PARAM_REQ:								
		api_length = sizeof(icf_set_call_params_req_st);
        break;

	case ICF_SET_SELF_ID_REQ:									
		api_length = sizeof(icf_set_self_id_req_st);
        break;

	case ICF_SET_LINE_PARAM_REQ:
		api_length = sizeof(icf_set_line_param_req_st);
	break;

#if 0
	case ICF_SET_SCALE_PARAMS_REQ:
		api_length = sizeof(icf_set_scale_params_req_st);
        break;
#endif
	case ICF_SET_REGISTRATION_REQ:								
		api_length = sizeof(icf_set_registration_req_st);
        break;


	case ICF_CONFIGURE_CFM:
		api_length = sizeof(icf_configure_cfm_st);
        break;

	case ICF_CREATE_CALL_REQ:
		api_length = sizeof(icf_create_call_req_st);
        break;

	case ICF_LOCAL_USER_ALERTED_REQ:
		api_length = sizeof(icf_local_user_alerted_req_st);
        break;

	case ICF_MEDIA_CONNECT_REQ:
		api_length = sizeof(icf_media_connect_req_st);
        break;
 
        case ICF_UNEXPECTED_MSG_IND:
                api_length = sizeof(icf_unexpected_msg_ind_st);
        break;

	case ICF_TERMINATE_CALL_REQ:
		api_length = sizeof(icf_terminate_call_req_st);
        break;

	case ICF_CONNECT_REQ:
		api_length = sizeof(icf_connect_req_st);
        break;

	case ICF_INCOMING_CALL_IND:
		api_length = sizeof(icf_incoming_call_ind_st);
        break;

/*ICF_HANDLE_REINVITE_START*/
	case ICF_INCOMING_CALL_MODIFY_IND:
		api_length = sizeof(icf_incoming_call_modify_ind_st);
        break;

	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND:
		api_length = 0;
        break;

	case ICF_CALL_MODIFY_CFM:
		api_length = sizeof(icf_call_modify_cfm_st);
        break;

	case ICF_CALL_MODIFY_REQ:
		api_length = sizeof(icf_call_modify_req_st);
		break;

	case ICF_CALL_MODIFY_RESP:
		api_length = sizeof(icf_call_modify_resp_st);
		break;

/*ICF_HANDLE_REINVITE_END*/

	case ICF_REMOTE_USER_ALERTED_IND:
		api_length = sizeof(icf_remote_user_alerted_ind_st);
        break;

	case ICF_TERMINATE_CALL_IND:
		api_length = sizeof(icf_terminate_call_ind_st);
        break;

	case ICF_CONNECT_IND:
		api_length = sizeof(icf_connect_ind_st);
        break;


	case ICF_REGISTER_REQ:										
		api_length = sizeof(icf_register_req_st);
        break;

	case ICF_REGISTER_CFM:										
		api_length = sizeof(icf_register_cfm_st);
        break;

	case ICF_REGISTER_STATUS_IND:										
		api_length = sizeof(icf_register_status_ind_st);
        break;

#ifdef ICF_NW_ACTIVATE
	case ICF_NETWORK_ACTIVATE_REQ:								
		api_length = sizeof(icf_network_activate_req_st);
        break;

	case ICF_EVENT_INDICATION:								
		api_length = sizeof(icf_event_indication_st);
        break;

	case ICF_NETWORK_ACTIVATE_CFM:								
		api_length = sizeof(icf_network_activate_cfm_st);
        break;

	case ICF_NETWORK_DEACTIVATE_REQ:							
		api_length = sizeof(icf_network_deactivate_req_st);
        break;

	case ICF_NETWORK_DEACTIVATE_CFM:								
		api_length = sizeof(icf_network_deactivate_cfm_st);
        break;
#endif

	case ICF_APP_REMOVE_REQ:										
		api_length = sizeof(icf_app_remove_req_st);
        break;

	case ICF_APP_REMOVE_CFM:										
		api_length = sizeof(icf_app_remove_cfm_st);
        break;

	case ICF_SUBSCRIBE_REQ:										
		api_length = sizeof(icf_subscribe_req_st);
        break;

	case ICF_SUBSCRIPTION_STATUS_IND:										
		api_length = sizeof(icf_subscription_status_ind_st);
        break;

	case ICF_NOTIFY_IND:
		api_length = sizeof(icf_notify_ind_st);
        break;

/*NOTIFY_OUT_OF_DIALOG_START*/
	case ICF_NOTIFY_CFM:
		api_length = sizeof(icf_notify_cfm_st);
        break;
/*NOTIFY_OUT_OF_DIALOG_END*/

	case ICF_PUBLISH_REQ:
		api_length = sizeof(icf_publish_req_st);
        break;

	case ICF_PUBLISH_RESP:										
		api_length = sizeof(icf_publish_resp_st);
        break;

	case ICF_MESSAGE_REQ:											
		api_length = sizeof(icf_message_req_st);
        break;

	case ICF_MESSAGE_RESP:										
		api_length = sizeof(icf_message_resp_st);
        break;

	case ICF_MESSAGE_IND:											
		api_length = sizeof(icf_message_ind_st);
        break;

	case ICF_REFER_REQ:	
		api_length = sizeof(icf_refer_req_st);
        break;

	case ICF_REFER_RESP:
		api_length = sizeof(icf_refer_resp_st);
        break;

    case ICF_MEDIA_CONNECTED_IND:
        api_length = sizeof(icf_media_connected_ind_st);
        break;

	case ICF_SET_MGMT_PLANE_AUTH_PARAM:
		api_length = sizeof(icf_set_mgmt_plane_auth_param_req_st);
		break;
		
	case ICF_CREATE_SECURE_CONN_REQ:
		api_length = sizeof(icf_create_secure_conn_req_st);
		break;

	case ICF_CREATE_SECURE_CONN_RESP:
		api_length = sizeof(icf_create_secure_conn_resp_st);
		break;

	case ICF_CLOSE_SECURE_CONN_REQ:
		api_length = sizeof(icf_close_secure_conn_req_st);
		break;

	case ICF_CLOSE_SECURE_CONN_RESP:
		api_length = sizeof(icf_close_secure_conn_resp_st);
		break;

	case ICF_SECURE_CONN_STATUS_IND:
		api_length = sizeof(icf_secure_conn_status_ind_st);
		break;

	case ICF_XCAP_CREATE_REPLACE_RES:
		api_length = sizeof(icf_create_replace_res_req_st);
		break;

	case ICF_XCAP_DELETE_RES:
		api_length = sizeof(icf_delete_res_req_st);
		break;

	case ICF_XCAP_FETCH_RES:
		api_length = sizeof(icf_fetch_res_req_st);
		break;

	case ICF_SYNCML_SEND_REQ:
		api_length = sizeof(icf_syncml_send_req_st);
		break;

	case ICF_MGMT_PLANE_API_PROC_FAILURE:
		api_length = sizeof(icf_api_proc_failure_st);
		break;

	case ICF_MGMT_PLANE_NW_RESP:
		api_length = sizeof(icf_mgmt_plane_nw_resp_st);
		break;
		
	case ICF_SET_SERVICE_PARAMS_REQ:
        api_length = sizeof(icf_set_service_params_req_st);
        break;

    /*  Changes Required for REL 7.0 for XCONF Feature */
    case ICF_CREATE_XCONF_REQ:
	api_length = sizeof(icf_create_ext_xconf_req_st);
	break;

    case ICF_ADD_XCONF_PARTY_REQ:
	api_length = sizeof(icf_add_xconf_party_req_st);
	break;

    case ICF_DELETE_XCONF_PARTY_REQ:
	api_length = sizeof(icf_delete_xconf_party_req_st);
	break;

    case ICF_RELEASE_XCONF_REQ:
	api_length = sizeof(icf_release_xconf_req_st);
	break;

    case ICF_CREATE_XCONF_RESP:
	api_length = sizeof(icf_create_ext_xconf_resp_st);
	break;
    
    case ICF_RELEASE_XCONF_RESP:
	api_length = sizeof(icf_release_xconf_resp_st);
	break;

    case ICF_ADD_XCONF_PARTY_RESP:
	api_length = sizeof(icf_add_xconf_party_resp_st);
	break;

    case ICF_DELETE_XCONF_PARTY_RESP:
	api_length = sizeof(icf_delete_xconf_party_resp_st);
	break;
    
    case ICF_RELEASE_XCONF_IND:
	api_length = sizeof(icf_xconf_release_ind_st);
	break;

    /*  REl 7.0 XCONF Feature change end */

    case ICF_CALL_HOLD_RESP:
    case ICF_CALL_RESUME_RESP:
        api_length = sizeof(icf_api_resp_st);
        break;

    case ICF_CALL_TRANSFER_ATTENDED_RESP:
        api_length = sizeof(icf_cta_resp_st);
        break;
    
	case ICF_CALL_TRANSFER_UNATTENDED_RESP:
        api_length = sizeof(icf_ctu_resp_st);
        break;
    
	case ICF_CONFERENCE_RESP:
        api_length = sizeof(icf_msg_resp_st);
        break;

    case ICF_CALL_TRANSFER_UNATTENDED_REQ:
        api_length = sizeof(icf_call_transfer_unattended_req_st);
        break;
    
	case ICF_CONFERENCE_REQ:
        api_length = sizeof(icf_conference_req_st);
        break;
    
	case ICF_REMOTE_CALL_TRANSFER_INITIATED_IND:
        api_length = sizeof(icf_remote_call_transfer_initiated_ind_st);
        break;

    case ICF_CALL_MERGED_IND:
        api_length = sizeof(icf_call_merged_ind_st);
        break;        

    case ICF_CALL_TRANSFER_ATTENDED_REQ:
        api_length = sizeof(icf_call_transfer_attended_req_st);
        break;

    case ICF_CALL_HOLD_REQ:
        api_length = sizeof(icf_call_hold_req_st);
        break;

    case ICF_CALL_RESUME_REQ:
        api_length = sizeof(icf_call_resume_req_st);
        break;

	case ICF_RES_UNAVAIL_IND:
		api_length = sizeof(icf_res_unavail_ind_st);
		break;		
#ifdef ICF_STAT_ENABLED
    case ICF_GET_ICF_STATS_REQ:
        api_length = sizeof(icf_get_icf_stats_req_st);
		break;

    case ICF_GET_ICF_STATS_RESP:
        api_length = sizeof(icf_stats_resp_st);
		break;
#endif

    case ICF_INFO_REQ:
        api_length = sizeof(icf_info_req_st);
		break;

  	case ICF_INFO_RESP:
        api_length = sizeof(icf_info_resp_st);
		break;

	case ICF_INFO_IND:
        api_length = sizeof(icf_info_ind_st);
		break;
       
        case ICF_INFO_CFM:
        api_length = sizeof(icf_info_cfm_st);
		break;     

    case ICF_CALL_HOLD_IND:
    case ICF_CALL_RESUME_IND:
        api_length = sizeof(icf_api_ind_st);
		break;

    case ICF_PARTY_REPLACED_IND:
        api_length = sizeof(icf_party_replaced_ind_st);
		break;
	case ICF_CALL_REDIRECTION_IND :
		api_length = sizeof(icf_call_redirection_ind_st);
		break;

   case ICF_TRACE_NW_BUFF_IND :
		api_length = sizeof(icf_trace_nw_buff_ind_st);
		break;
		
    case ICF_LOCAL_USER_TERMINATE_CALL_RESP:
        api_length = 0;
        break;

    case ICF_REMOTE_USER_TERMINATE_CALL_RESP:
        api_length = 0;
        break;

	case ICF_START_FAX_REQ:
		api_length = sizeof(icf_start_fax_req_st);
		break;

	case ICF_START_FAX_RESP:
		api_length = sizeof(icf_start_fax_resp_st);
		break;

	case ICF_START_FAX_IND:
		api_length = sizeof(icf_start_fax_ind_st);
		break;

	case ICF_STOP_FAX_RESP:
		api_length = sizeof(icf_stop_fax_resp_st);
		break;

	case ICF_CALL_MODIFY_CANCEL_RESP:
		api_length = sizeof(icf_call_modify_cancel_resp_st);
		break;

	case ICF_STOP_FAX_IND:
		api_length = sizeof(icf_stop_fax_ind_st);
		break;

        case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
       		 api_length = sizeof(icf_remote_call_transfer_initiated_cfm_st);
       		 break;

	case ICF_CONNECT_RESP:
		api_length = sizeof(icf_connect_resp_st);
		break;

	case ICF_CALL_FORWARDED_IND:
		api_length = sizeof(icf_call_forwarded_ind_st);
		break;

	case ICF_OPTIONS_REQ:											
		api_length = sizeof(icf_options_req_st);
        break;

	case ICF_OPTIONS_RESP:										
		api_length = sizeof(icf_options_resp_st);
        break;
/* ICF_6_1_OPTIONS_SUPPORT start */
	case ICF_INC_OPTIONS_IND:											
		api_length = sizeof(icf_inc_options_ind_st);
        break;

	case ICF_OPTIONS_CFM:										
		api_length = sizeof(icf_options_cfm_st);
        break;
#ifdef ICF_TRACE_ENABLE
        case ICF_SET_TRACE_LEVEL_REQ:
                api_length = sizeof(icf_set_trace_level_req_st);
                break;

        case ICF_SET_TRACE_REQ:
                api_length = sizeof(icf_set_trace_req_st);
                break;

        case ICF_REPORT_TRACE_IND:
                api_length = sizeof(icf_report_trace_ind_st);
                break;

#endif
#ifdef ICF_ERROR_ENABLE
        case ICF_SET_ERROR_REPORT_LEVEL_REQ:
                api_length = sizeof(icf_set_error_report_level_req_st);
                break;
        case ICF_REPORT_ERROR_IND:
                api_length = sizeof(icf_report_error_ind_st);
                break;
#endif
        case ICF_EARLY_INC_CALL_IND :
                api_length = sizeof(icf_early_incoming_call_ind_st);
                break;
        case ICF_EARLY_INC_CALL_CFM :
                api_length = sizeof(icf_early_incoming_call_cfm_st);
                break;
/* ICF_6_1_OPTIONS_SUPPORT end */
        case ICF_PROGRESS_IND:
		api_length = sizeof(icf_progress_ind_st);
		break;

#ifdef ICF_STAT_ENABLE
        case ICF_STATS_REQ:
         	 api_length = sizeof(icf_stats_req_st);
               	 break;

        case ICF_STATS_RESP:
       		 api_length = sizeof(icf_stats_resp_st);
	       	 break;

#endif
        case ICF_GET_REGISTER_STATUS_REQ:
                 api_length = sizeof(icf_get_register_status_req_st);
                 break;

        case ICF_GET_REGISTER_STATUS_RESP:
                 api_length = sizeof(icf_get_register_status_resp_st);
                 break;

       /*Restart/Reinitialise  API */

       case ICF_RESTART_REQ:
               api_length = 0;
       break;
       case ICF_RESTART_RESP:
	       api_length = 0;
	break;
 	case ICF_RESTART_IND:
               api_length = 0;
        break;
 	case ICF_REINITIALISE_REQ:
               api_length = 0;
        break;      
  	case ICF_REINIT_RESP:
               api_length = 0;
        break;
	 case ICF_REINIT_IND:
               api_length = 0;
        break;
     case ICF_STOP_FAX_REQ:
               api_length = 0;
        break;      
     case ICF_CALL_MODIFY_CANCEL_REQ:
               api_length = 0;
        break; 
    default:
		api_length = icf_get_mm_api_length(api_id,&is_valid_api);
		break;
	}
	if(ICF_TRUE == is_valid_api)
	{
		api_length += ICF_API_HEADER_LENGTH;
	}
	/* else api_length is 0 already initialised above */

	return api_length;
}



icf_void_t icf_free_pub_uri_list(icf_pub_uri_list_node_st *p_uri_list)
{
    icf_pub_uri_list_node_st *p_next_node;
    
    for(;ICF_NULL != p_uri_list; p_uri_list = p_next_node)
    {
        p_next_node = p_uri_list->p_next;
        icf_free_address_list(p_uri_list->p_assoc_uri_list);
        ICF_CLIB_MEMFREE(p_uri_list);
    }
}


icf_void_t icf_free_address_list(icf_address_list_node_st *p_list)
{
    icf_address_list_node_st *p_next_node;
    
    for(;ICF_NULL != p_list; p_list = p_next_node)
    {
        p_next_node = p_list->p_next;
        ICF_CLIB_MEMFREE(p_list);
    }
}

/**************************************
/ICF REL 6.0
****************************************/

icf_void_t	icf_free_call_modify_media_data_list_st(icf_list_st *p_list)
{
	icf_list_st *p_next_node;
        icf_call_modify_media_data_st   *p_call_modify_media_data = ICF_NULL;

	for(;p_list != ICF_NULL; p_list = p_next_node)
	{
		p_next_node = p_list->p_next;
                p_call_modify_media_data = (icf_call_modify_media_data_st*)p_list->p_data;
		
               if(p_call_modify_media_data->bit_mask & ICF_CALL_MOD_CODEC_INFO_PRESENT)
               { 
		icf_free_codec_attrib_st(p_call_modify_media_data->p_codec_info_list);
               }
               if(p_call_modify_media_data->bit_mask & ICF_CALL_MOD_ENC_ATTRIB_PRESENT)
               {
		icf_free_list(p_call_modify_media_data->p_encoded_attributes);
               }
		ICF_CLIB_MEMFREE(p_list->p_data);
		ICF_CLIB_MEMFREE(p_list);
	}
}

icf_void_t	icf_free_codec_attrib_st(icf_list_st *p_list)
{
	icf_list_st *p_next_node;
    icf_codec_attrib_st *p_icf_codec_attrib = ICF_NULL;

	for(;p_list != ICF_NULL; p_list = p_next_node)
	{
		p_next_node = p_list->p_next;
        p_icf_codec_attrib = (icf_codec_attrib_st*)p_list->p_data;
        if(p_icf_codec_attrib->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)
        {
		    icf_free_list(((icf_codec_attrib_st*)p_list->p_data)->p_encoded_attributes);
        }
		ICF_CLIB_MEMFREE(p_list->p_data);
		ICF_CLIB_MEMFREE(p_list);
	}
}

icf_void_t  icf_free_icf_stream_capab_list_st(icf_list_st *p_list)
{
	icf_list_st *p_next_node;
    icf_stream_capab_st *p_icf_stream_capab_lst = ICF_NULL;

	for(;p_list != ICF_NULL; p_list = p_next_node)
	{
		p_next_node = p_list->p_next;        
        p_icf_stream_capab_lst = (icf_stream_capab_st*)p_list->p_data;
        if(p_icf_stream_capab_lst->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
        {
		icf_free_codec_attrib_st(((icf_stream_capab_st*)p_list->p_data)->p_codec_info_list);      
        }
        if(p_icf_stream_capab_lst->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
        {
            icf_free_list(((icf_stream_capab_st*)p_list->p_data)->p_encoded_attributes);
        }
		ICF_CLIB_MEMFREE(p_list->p_data);
		ICF_CLIB_MEMFREE(p_list);
	}
}




icf_void_t  icf_free_user_reg_status_st(icf_list_st *p_list)
{
    icf_list_st *p_next_node;

    for(;p_list != ICF_NULL; p_list = p_next_node)
    {
        p_next_node = p_list->p_next;  

        ICF_CLIB_MEMFREE(p_list->p_data);
        ICF_CLIB_MEMFREE(p_list); 
    }
 
}


icf_return_t icf_make_list_node(icf_list_st **p_p_start_node, icf_uint32_t count, icf_uint32_t size)
{
   
    icf_list_st			**p_p_curr_node;
    icf_return_t		ret_val = ICF_SUCCESS;
  
    if (count <= 0 || count > ICF_CLIB_MAX_LIST_NODES) 
    {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_string_node - Node Count Exceeds Limits (%d)\n", count));
        return ICF_FAILURE;
    }
   
    for(p_p_curr_node = p_p_start_node; count--; p_p_curr_node = &((*p_p_curr_node)->p_next)) 
    {
        *p_p_curr_node = ICF_NULL; 
        *p_p_curr_node = (icf_list_st*)ICF_CLIB_MEMGET(sizeof(icf_list_st));

        if (ICF_NULL == *p_p_curr_node) 
        {
            ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_make_string_node - ICF_CLIB_MEMGET Failed \n"));
            ret_val = ICF_FAILURE;
            break;
        }
	(*p_p_curr_node)->p_data = ICF_CLIB_MEMGET(size);
        if (ICF_NULL == (*p_p_curr_node)->p_data)
        {
           ICF_CLIB_MEMFREE(*p_p_curr_node);
           *p_p_curr_node = ICF_NULL;
           ret_val = ICF_FAILURE;
           break;
        } 
        (*p_p_curr_node)->p_next = ICF_NULL;
    }
        
    /* We are not freeing the allocaed memory in case of failure 
       as there can be no true recovery if memget fails */
    return ret_val;
}

