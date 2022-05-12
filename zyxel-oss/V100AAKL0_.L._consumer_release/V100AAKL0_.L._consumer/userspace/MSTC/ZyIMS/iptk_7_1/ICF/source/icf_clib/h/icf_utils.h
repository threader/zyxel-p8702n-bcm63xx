/*****************************************************************************
*										
* FILE NAME    :icf_utils.h
*										
* DESCRIPTION  :The file contains utility functions used by packing/
*               unpacking routines
*										
* Revision History :				
*				
*    DATE            NAME         REFERENCE       REASON
* --------    ------------        ----         ----------
* 8-JUL-2005   Ram Narayan        None         Initial
*   										
* Copyright 2006, Aricent.   					
*										
*****************************************************************************/



#ifndef __ICF_UTILS_H__
#define __ICF_UTILS_H__



/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */




#include "icf_pack_api.h"
#include "icf_unpack_api.h"
#include "icf_pack_mm_api.h"
#include "icf_unpack_mm_api.h"



/* To Disable Traces Comment the following line*/
/*#define ICF_CLIB_TRACE_ENABLE*/



/* For Detailed traces Set this Trace Level to 2*/
#define ICF_CLIB_TRACE_LEVEL 1



#ifdef ICF_CLIB_TRACE_ENABLE
#define ICF_CLIB_TRACE(arg) printf arg
#else
#define ICF_CLIB_TRACE(arg)
#endif



typedef enum _ICF_ERROR {
    ICF_ERR_UNKNOWN,
    ICF_ERR_NULL_POINTER,
    ICF_ERR_MEMGET_FAILED,
    ICF_ERR_MEMFREE_FAILED,
    ICF_ERR_ENCODE_FAILED,
    ICF_ERR_DECODE_FAILED,
    ICF_ERR_INVALID_LENGTH
} _ICF_ERROR;




/* Specifies the maximum number of linked list nodes that can be created*/
#define ICF_CLIB_MAX_LIST_NODES (30)

/* Defines the Maximum String length*/
#define ICF_CLIB_MAX_STR_LEN (512)

/* API length should not be greater then to this*/
#define ICF_CLIB_MAX_API_LENGTH (15000)

/* Temporary Buffer For storing the arbitrary length API*/
#define ICF_CLIB_MAX_TEMP_ARBI_BUFF_LENGTH (10000)


/*--------------------------- Prototypes ----------------------------*/

/* Create count Linked list nodes of type icf_header_st*/
icf_return_t icf_make_hdr_node(icf_header_st** start, icf_uint8_t count);

/* Create count Linked list nodes of type icf_tag_st*/
icf_return_t icf_make_tag_node(icf_tag_st** start, icf_uint8_t count);

/* Create count Linked list nodes of type icf_msg_body_st*/
icf_return_t icf_make_msg_body_node(icf_msg_body_st** start, icf_uint8_t count);

/* Create count Linked list nodes of type icf_header_name_st*/
icf_return_t icf_make_hdr_name_node(icf_header_name_st** start, icf_uint8_t count);

icf_return_t icf_make_string_node(icf_string_list_node_st **p_p_start_node, icf_uint32_t count);

/* Free the memory of Linked list nodes of type icf_header_st*/
icf_return_t icf_free_hdr_node(icf_header_st** start, icf_uint8_t count);

/* Free the memory of Linked list nodes of type icf_tag_st*/
icf_return_t icf_free_tag_node(icf_tag_st** start, icf_uint8_t count);

/* Free the memory of Linked list nodes of type icf_mdg_body_st*/
icf_return_t icf_free_msg_body_node(icf_msg_body_st** start, icf_uint8_t count);

/* Free the memory of Linked list nodes of type icf_hdr_name_st*/
icf_return_t icf_free_hdr_name_node(icf_header_name_st** start, icf_uint8_t count);

/* Free the memory of icf_msg_st*/
icf_return_t icf_free_msg(icf_msg_st* p_msg);


icf_uint32_t icf_get_api_length(icf_uint16_t api_id);

icf_return_t icf_free_string_list(icf_string_list_st *p_str_list);

icf_void_t icf_free_pub_uri_list(icf_pub_uri_list_node_st *p_uri_list);

icf_void_t icf_free_address_list(icf_address_list_node_st *p_list);

icf_void_t	icf_free_call_modify_media_data_list_st(icf_list_st *p_list);

icf_void_t	icf_free_codec_attrib_st(icf_list_st *p_list);

icf_void_t  icf_free_icf_stream_capab_list_st(icf_list_st *p_list);

icf_return_t icf_make_list_node(icf_list_st **p_p_start_node, icf_uint32_t count, icf_uint32_t size);


/*******************************************************************************
 * FUNCTION:        icf_free_app_filter_list
 * 
 * DESCRIPTION:     Function to free Internal Filter List strucure 
 *  
 ******************************************************************************/

icf_return_t icf_free_app_filter_list(icf_filter_list_st *p_filter_list);

icf_uint32_t icf_get_mm_api_length(icf_uint16_t api_id,icf_boolean_t *p_is_valid_api);

/*******************************************************************************
 * FUNCTION:        icf_free_user_reg_status_st(icf_list_st *p_list
 *
 * DESCRIPTION:     Function to free user reg  List strucure
 *
 ******************************************************************************/

icf_void_t  icf_free_user_reg_status_st(icf_list_st *p_list);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /*__ICF_UTILS_H__*/
