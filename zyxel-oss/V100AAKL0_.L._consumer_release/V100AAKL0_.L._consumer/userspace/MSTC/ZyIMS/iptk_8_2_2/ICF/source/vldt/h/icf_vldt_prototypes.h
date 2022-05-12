/*------------------------------------------------------------------------------
 *
 * File name        : icf_vldt_prototypes.h
 *
 * Purpose          : Contains the definitions for the structures used in 
 *                    PA Validation. 
 *
 * Revision History :
 *
 * Date         Name              Ref#            Comments
 * --------     ------------      ----            ---------------
 * 13-JUN-2007  Jagmohan Chauhan   No              Initial
                Rishi Saxena
 * 10-Dec-2007  Jagmohan Chauhan   No            New function icf_vldt_icf_address_with_port_st
 *                                               added for SPR fix 17567
 * 17-Jan-2008  Shruti Thakral                   Enhancements for Rel 8.0
 * 28-Jan-2008  Kamini Gangwani   Rel 8.0        Added prototype for function
 *                                               icf_vldt_icf_media_transport_type_et 
 * 07-MAR-2008  Vipul Varshney    SPR 18122      AKA Enhancement
 * 21-Apr-2008  Tarun Gupta       SPR 18126      Modified function prototype of 
 *                                               icf_vldt_icf_address_st
 * 25-Jun-2008  Abhishek Dhammawat IPTK 8.0      Added prototype for
 *                                               icf_vldt_allow_null_icf_string_st
 * 11-Feb-2009  Ashutosh Mohan    Rel8.1         Added prototype for:
 *                                               icf_vldt_icf_tunnelled_sdp_st
 *                                               icf_vldt_icf_sdf_address_st
 *                                               icf_vldt_icf_sdf_string_st
 *                                               icf_vldt_icf_tunnelled_media_capabilities_resp_st
 *                                               icf_vldt_tunnelled_modify_media_session_resp_st
 *                                               icf_vldt_icf_early_session_cfm_st
 * 15-Mar-2009  Anuradha Gupta     Rel 8.1       Added prototypes for Timer Enh
 * 26-May-2009  Preksha            Rel 8.2       Asynchronus Message enhancements
 * 17-Jun-2009  Tarun Gupta       SPR 19742      Added icf_vldt_port_strcmpi prototype
 * 08-Oct-2009  Abhishek Dhammawat SPR 20063     Merged 1-7718121 
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_VLDT_PROTOTYPES_H__
#define __ICF_VLDT_PROTOTYPES_H__

#include "icf_error_id.h"
#include "icf_mm_error_id.h"
#include "icf_types.h"
#include "icf_defs.h"
#include "icf_trace_error_types.h"
#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_port_macros.h"
#include "icf_port_defs.h"
#include "icf_port.h"
#include "icf_macro.h"

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
    */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
*Use standard C.
* */

extern "C"
{
#endif /* __cplusplus */
/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strcpy
*
*  DESCRIPTION    :   Utility function for string copy.
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_vldt_port_strcpy(
        INOUT icf_uint8_t   *p_dest,
        IN  icf_uint8_t   *p_src);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strcat
*
*  DESCRIPTION    :   Utility function for string cat.
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_vldt_port_strcat(
         icf_uint8_t   *p_dest,
          icf_uint8_t   *p_src);

/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strlen
*
*  DESCRIPTION    :   Utility function for calculating string length.
*
*  RETURNS        :   A U32 holding value of the length of the string
*
 *************************************************************************/
icf_uint32_t 
icf_vldt_port_strlen(icf_uint8_t    *p_s);

/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strcmp
*
*  DESCRIPTION    :   Utility function for string comparison.
*
*  RETURNS        :  0 if strings match, non zero if they don't
*
 *************************************************************************/
icf_int32_t icf_vldt_port_strcmp(
         icf_uint8_t   *p_str1,
          icf_uint8_t   *p_str2);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strchr
*
*  DESCRIPTION    : Returns pointer to first occurrrnce of a character in a
*                    given string.
*
*  RETURNS        :   Returns pointer to first occurrrnce of a character in a
*                    given string.
*
*************************************************************************/
icf_uint8_t *
icf_vldt_port_strchr (
         icf_uint8_t                 *p_string,
         icf_uint8_t                  character);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_body_list_st
 *
 * DESCRIPTION: This function is to validate icf_body_list_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_msg_body_list_st(
                  IN icf_msg_body_list_st *p_body , 
                  INOUT icf_vldt_err_st *p_vldt_err);
#if 0
/*******************************************************************************
*
* FUNCTION:        icf_vldt_body
*
* DESCRIPTION:This function is invoked to validate icf_msg_body_st.
*
*
******************************************************************************/

icf_return_t icf_vldt_body(
                      IN icf_msg_body_st *p_message,
                      INOUT icf_vldt_err_st *p_vldt_err);
#endif


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_list_st
 *
 * DESCRIPTION: This function is to validate icf_list_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_list_st(
                    IN icf_list_st *p_list , 
                    INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_stream_id_list_st
 *
 * DESCRIPTION: This function is to validate icf_stream_id_list_st

 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_stream_id_list_st(
                        IN icf_stream_id_list_st *p_list , 
                        INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_app_add_req_st
 *
 * DESCRIPTION: This function is to validate icf_app_add_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_app_add_req_st(
                      IN icf_uint8_t *p_payload , 
                      INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_create_call_req_st
 *
 * DESCRIPTION: This function is to validate icf_address_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_create_call_req_st(
                          IN icf_uint8_t *p_payload , 
                          INOUT icf_vldt_err_st *p_vldt_err);

#if 0
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_header
*
* DESCRIPTION:     This function is invoked to validate icf_header_st.
*
*
******************************************************************************/
icf_return_t icf_vldt_icf_header(
                      IN icf_header_st *p_header,
                      INOUT icf_vldt_err_st *p_vldt_err);
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_header_name
*
* DESCRIPTION:     This function is invoked to validate icf_header_name_st.
*
*
******************************************************************************/

icf_return_t icf_vldt_icf_header_name(
                      IN icf_header_name_st *p_hdr_name,
                      INOUT icf_vldt_err_st *p_vldt_err);


/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_tag
*
* DESCRIPTION:     This function is invoked to validate icf_header_st.
*
*
******************************************************************************/
icf_return_t icf_vldt_icf_tag(
                      IN icf_tag_st *p_header,
                      INOUT icf_vldt_err_st *p_vldt_err);

#endif




/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_publich_req_st
 *
 * DESCRIPTION: This function is to validate icf_publish_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_publish_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_message_req_st
 *
 * DESCRIPTION: This function is to validate icf_message_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_message_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err)  ;


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_conference_req_st
 *
 * DESCRIPTION: This function is to validate icf_conference_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_conference_req_st(
                            IN icf_uint8_t *p_payload , 
                            INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_refer_req_st
 *
 * DESCRIPTION: This function is to validate icf_refer_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_refer_req_st(
                             IN icf_uint8_t *p_payload , 
                             INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_call_hold_req_st
 *
 * DESCRIPTION: This function is to validate icf_call_hold_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_call_hold_req_st(
                             IN icf_uint8_t *p_payload , 
                             INOUT icf_vldt_err_st *p_vldt_err);



/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_call_resume_req_st
 *
 * DESCRIPTION: This function is to validate icf_call_resume_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_call_resume_req_st(
                            IN icf_uint8_t *p_payload , 
                            INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_call_transfer_unattended_req_st
 *
 * DESCRIPTION: This function is to validate icf_call_transfer_unattended_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_ctu_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_call_transfer_attended_req_st
 *
 * DESCRIPTION: This function is to validate icf_call_transfer_attended_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_cta_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_info_req_st
 *
 * DESCRIPTION: This function is to validate icf_info_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_info_req_st(
                          IN icf_uint8_t *p_payload , 
                          INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_call_modify_ireq_st
 *
 * DESCRIPTION: This function is to validate icf_call_modify_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_call_modify_req_st(
                          IN icf_uint8_t *p_payload , 
                          INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_call_modify_cfm_st
 *
 * DESCRIPTION: This function is to validate icf_call_modify_cfm_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_call_modify_cfm_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_start_fax_req_st
 *
 * DESCRIPTION: This function is to validate icf_start_fax_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_start_fax_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_notify_cfm_st
 *
 * DESCRIPTION: This function is to validate icf_notify_cfm_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_notify_cfm_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_options_req_st
 *
 * DESCRIPTION: This function is to validate icf_options_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_options_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_options_cfm_st
 *
 * DESCRIPTION: This function is to validate icf_options_cfm_st_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_options_cfm_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_info_cfm_st
 *
 * DESCRIPTION: This function is to validate icf_info_cfm_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

  icf_return_t icf_vldt_icf_info_cfm_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_early_incoming_call_cfm_st
 *
 * DESCRIPTION: This function is to validate icf_early_incoming_call_cfm_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
 icf_return_t icf_vldt_icf_early_incoming_call_cfm_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_app_remove_req_st
*       
* DESCRIPTION:     This function is invoked  to validate the 
*                  icf_app_remove_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_app_remove_req_st(
                        IN icf_uint8_t *p_payload, 
                        INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_get_line_settings_req_st
 *
 * DESCRIPTION: This function is to validate icf_get_line_settings_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_get_line_settings_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_trace_nw_buff_ind_st
 *
 * DESCRIPTION: This function is to validate icf_trace_nw_buff_ind_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_trace_nw_buff_ind_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);

#ifdef ICF_STAT_ENABLED
/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_icf_stats_req_st
*   
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_get_icf_stats_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_get_icf_stats_req_st(
                              icf_uint8_t *p_payload,
                              icf_vldt_err_st *p_vldt_err);
#endif


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_set_transport_req_st
 *
 * DESCRIPTION: This function is to validate icf_set_transport_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_set_transport_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);



/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_set_self_id_req_st
 *
 * DESCRIPTION: This function is to validate icf_set_self_id_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or
 *          failure of the validation
 *
 ***************************************************************************/
 icf_return_t icf_vldt_icf_set_self_id_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_set_registration_req_st
 *
 * DESCRIPTION: This function is to validate icf_set_registration_req
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_set_registration_req_st(
                           IN icf_uint8_t *p_payload , 
                           INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_set_line_param_req_st
 *
 * DESCRIPTION: This function is to validate icf_set_line_param_req
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_set_line_param_req(
                            IN icf_uint8_t *p_payload , 
                            INOUT icf_vldt_err_st *p_vldt_err);

#if 0
/**************************************************************************
*
* FUNCTION:        icf_vldt_content_type
*
* DESCRIPTION:     This function is invoked to validate string.
*
**************************************************************************/
icf_return_t icf_vldt_content_type(
             IN icf_string_st *p_str,
             INOUT icf_vldt_err_st *p_vldt_err);




/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_string
* DESCRIPTION: This function is used to validate icf_string_st
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation

*****************************************************************************/

icf_return_t icf_vldt_icf_string(
                             IN icf_string_st *p_str,
                             INOUT icf_vldt_err_st *p_vldt_err);
#endif

/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_string_st	
* DESCRIPTION: This function is used to validate icf_string_st 
*              str_len, should be less than max. length of string allowed
*              str_len, character at str[str_len] is '\0'  
*              str_len, str_len !=0   
*              str[ICF_MAX_STR_LEN], string operation!----> string length of
*              string should come 			
*              same as that of the value of str_len
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
					
*****************************************************************************/

icf_return_t icf_vldt_icf_string_st(
                             IN icf_string_st *p_str, 
                             INOUT icf_vldt_err_st *p_vldt_err);

/****************************************************************************
* FUNCTION NAME: icf_vldt_allow_null_icf_string_st	
* DESCRIPTION: This function is used to validate icf_string_st 
*              str_len, should be less than max. length of string allowed
*              str[ICF_MAX_STR_LEN], string operation!----> string length of
*              string should come same as that of the value of str_len
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
					
*****************************************************************************/

icf_return_t icf_vldt_allow_null_icf_string_st(
                             IN icf_string_st *p_str, 
                             INOUT icf_vldt_err_st *p_vldt_err);


/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_large_string_st
* DESCRIPTION: This function is used to validate icf_large_string_st
*              str_len, should be less than max. length of string allowed
*              str_len, character at str[str_len] is '\0'
*              str_len, str_len !=0
*              str[ICF_MAX_LARGE_STR_LEN], string operation!----> string length *              of string should come
*              same as that of the value of str_len
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation

*****************************************************************************/

icf_return_t icf_vldt_icf_large_string_st(
                             IN icf_large_string_st *p_str,
                             INOUT icf_vldt_err_st *p_vldt_err);




/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_line_id_t
* DESCRIPTION: This function is used to validate icf_line_id_st  
*              line_id should be less than ICF_MAX_NUM_OF_LINES          
* 
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation					
*****************************************************************************/
icf_return_t icf_vldt_icf_line_id_t(
                             IN icf_line_id_t lid, 
                             INOUT icf_vldt_err_st *p_vldt_err,
                             IN icf_boolean_t is_config_api);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_header_st	
* DESCRIPTION: This function is used to validate icf_header_st   
*              hdr_name and hdr_value should be validated against 
*              icf_vldt_icf_string_st          
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_header_st(
                             IN icf_header_st *p_hdr, 
                             INOUT icf_vldt_err_st *p_vldt_err);


/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_tag_st
* DESCRIPTION: This function is used to validate icf_tag_st   
*              hdr_name and tag_value should be validated against 
*              icf_vldt_icf_string_st.         
*              tag_type should have a check of MAXIMUM and MINIMUM value
* 
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation					
*****************************************************************************/
icf_return_t icf_vldt_icf_tag_st(
                             IN icf_tag_st *p_tag, 
                             INOUT icf_vldt_err_st *p_vldt_err);


/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_msg_body_st	
* DESCRIPTION: This function is used to validate icf_msg_body_st   
*              content_type should be validated against icf_vldt_icf_string_st  
*              msg_body_hdr_list should be validated against 
*              icf_vldt_icf_header_list_st 
*              length should not be NULL 
*              value, character at length + 1  should be '\0'
*              value, string operation!--->string length of string should come  
*              same as that of the value of str_len  
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validatio
*****************************************************************************/
icf_return_t icf_vldt_icf_msg_body_st(
                              IN icf_msg_body_st *p_msg, 
                              INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_header_name_st
* DESCRIPTION: This function is used to validate icf_header_name_st   
*              hdr_name should be validated against icf_vldt_icf_string_st
* 
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_header_name_st(
                                 IN icf_header_name_st *p_hdr_name,
                                 INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_string_list_node_st	
* DESCRIPTION: This function is used to validate icf_string_list_node_st  
*              str should be validated against icf_vldt_icf_string_st.
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation          
*****************************************************************************/
icf_return_t icf_vldt_icf_string_list_node_st(
                              IN icf_string_list_node_st *p_str_node, 
                              INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_media_stream_param_ut	
* DESCRIPTION: This function is used to validate icf_media_stream_param_ut  
*              audio_profile should be validated against 
*              icf_vldt_icf_audio_profile_st
*              video_profile should be validated against 
*              icf_vldt_icf_video_profile_st
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
/* icf_return_t icf_vldt_icf_media_stream_param_ut(
                             IN icf_media_stream_param_ut *p_msparam, 
                             INOUT icf_media_type str,icf_vldt_err_st *p_vldt_err);*/



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_video_profile_st	
* DESCRIPTION: This function is used to validate icf_video_profile_st  
*              encoding_quality should have a check that its value can only be
*              in between 0 to 10.
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_video_profile_st(
                            IN icf_video_profile_st *p_vprofile, 
                            INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_tcp_st	
* DESCRIPTION: This function is used to validate icf_tcp_st into a byte buffer  
*              type can have only two values ICF_TCP_SERVER or ICF_TCP_CLIENT
*              port should have a check of its range
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation 
*****************************************************************************/
icf_return_t icf_vldt_icf_tcp_st(
                            IN icf_tcp_st *p_tcp, 
                            INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_t38_attrib_param_st
* DESCRIPTION: This function is used to validate icf_t38_attrib_param_st  
*              fax_version can have values ICF_T38_1998, ICF_T38_2000 or 
*              ICF_T38_2002
*              bitrate can have values such as ICF_T38_2400_BIT_PER_SEC, 
*              ICF_T38_4800_BIT_PER_SEC, ICF_T38_7200_BIT_PER_SEC,
*              ICF_T38_9600_BIT_PER_SEC, ICF_T38_14400_BIT_PER_SEC, or
*              ICF_T38_33600_BIT_PER_SEC.
*              rate_management can have following values 
*              ICF_T38_RATE_MANAGEMENT_LOCAL or 
*              ICF_T38_RATE_MANAGEMENT_TRANSFERRED
*              err_correction can have values such as 
*              ICF_T38_ERROR_CORR_REDUNDANCY or
*              ICF_T38_ERROR_CORR_FEC
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_t38_attrib_param_st(
                            IN icf_t38_attrib_param_st *p_t38aparam, 
                            INOUT icf_vldt_err_st *p_vldt_err);




/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_app_add_req_st
* DESCRIPTION: This function is used to validate icf_transport_mode_t   
*              transport_mode_t can have values such as ICF_TRANSPORT_TYPE_UDP, 
*              ICF_TRANSPORT_TYPE_TCP, ICF_TRANSPORT_MODE_BOTH
*              or ICF_TRANSPORT_TYPE_TLS
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*
*****************************************************************************/
icf_return_t icf_vldt_icf_transport_mode_t(
                             IN icf_transport_mode_t tmode, 
                             INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_media_type_t

* DESCRIPTION: This function is used to validate icf_media_type_t  
*              stream_type can have values as ICF_STREAM_TYPE_INVALID,
*              ICF_STREAM_TYPE_AUDIO, ICF_STREAM_TYPE_VIDEO,
*              ICF_STREAM_TYPE_TBCP, ICF_STREAM_TYPE_FAX
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_media_type_t(
                             IN icf_media_type_t stream_type, 
                             INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_duration_t	
* DESCRIPTION: This function is used to validate icf_duration_t   
*              all icf_duration_t either 0 or in multiples of 1000s
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_duration_t(
                              IN icf_duration_t dur, 
                              INOUT icf_vldt_err_st *p_vldt_err);



/****************************************************************************
* FUNCTION NAME: icf_vldt_port	
* DESCRIPTION: This function is used to validate icf_port  
*              port should be checked against its range i.e. it should be
*             in between ICF_CFG_MIN_PORT_VALUE and ICF_CFG_MAX_PORT_VALUE
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_port(
                          IN icf_uint16_t port, 
                          INOUT icf_vldt_err_st *p_vldt_err);

/****************************************************************************
* FUNCTION NAME: icf_vldt_port_reg
* DESCRIPTION: This function is used to validate icf_port  
*              port i.e. it should be
*              greater than ICF_CFG_MIN_PORT_VALUE or 0 
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_port_reg(
                          IN icf_uint16_t port,
                          INOUT icf_vldt_err_st *p_vldt_err);


/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_transport_addr_st
* DESCRIPTION: This function is used to validate icf_transport_addr_st  
*              addr_type can have following values such as 
*              ICF_TRANSPORT_ADDRESS_INVALID, ICF_TRANSPORT_ADDRESS_DOMAIN_NAME,
*              ICF_TRANSPORT_ADDRESS_IPV4_ADDR or ICF_TRANSPORT_ADDRESS_IPV6_ADDR 
*              addr should be checked against union address
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_transport_addr_st(
                          IN icf_transport_addr_st *p_taddr, 
                          INOUT icf_vldt_err_st *p_vldt_err,
                          IN icf_boolean_t self_ip_null);



/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_stream_capab_st	
* DESCRIPTION: This function is used to validate icf_stream_capab_st   
*              stream_id should be validated against stream_id in utilities sheet
*              stream_type should be validated against stream_type in utilities 
*              sheet
*              p_codec_info_list is of type icf_list_st and each node of this is 
*              of type icf_codec_attrib_st 
*              media_stream_param should be validated against 
*              icf_vldt_icf_media_stream_param_ut
*              qos_mark can have values as ICF_TRUE or ICF_FALSE
*
*RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_stream_capab_st(
                             IN icf_stream_capab_st *p_scapab, 
                             INOUT icf_vldt_err_st *p_vldt_err);

/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_audio_codec_param_st	
* DESCRIPTION: This function is used to validate icf_audio_codec_param_st  
*              amr_mode_count should be checked as its values can be 
*              in between 1 and 
*              ICF_MAX_AMR_MODE
*              crc_enabled can have values between ICF_TRUE and ICF_FALSE
*
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
*****************************************************************************/
icf_return_t icf_vldt_icf_audio_codec_param_st(
                          IN icf_audio_codec_param_st *p_acp,
                          INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_address_st
 *
 * DESCRIPTION: This function is to validate icf_address_st
 * 	
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_address_st(
                          INOUT icf_address_st *p_addr, 
                          INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_header_list_st
 *
 * DESCRIPTION: This function is to validate icf_header_list_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_header_list_st(
                          IN icf_header_list_st *p_header , 
                          INOUT icf_vldt_err_st *p_vldt_err) ;


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_tag_list_st
 *
 * DESCRIPTION: This function is to validate icf_tag_list_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_tag_list_st(
                           IN icf_tag_list_st *p_tag , 
                           INOUT icf_vldt_err_st *p_vldt_err);



/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_header_name_list_st
 *
 * DESCRIPTION: This function is to validate icf_header_name_list_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_header_name_list_st(
                           IN icf_header_name_list_st *p_header_name , 
                           INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_string_list_st
 *
 * DESCRIPTION: This function is to validate icf_string_list_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/
icf_return_t icf_vldt_icf_string_list_st(
                            IN icf_string_list_st *p_string_l , 
                            INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_transport_address_st
 *
 * DESCRIPTION: This function is to validate icf_transport_address_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_transport_address_st(
                            IN icf_transport_address_st *p_trans_addr , 
                            INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_transport_addr_reg_st
 *
 * DESCRIPTION: This function is to validate icf_transport_address_st. This
 *              funtion calls icf_vldt_port_reg which allows the port to be 0
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation.
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_transport_addr_reg_st(
                            IN icf_transport_address_st *p_trans_addr ,
                            INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf__ipv4_int_address_st
 *
 * DESCRIPTION: This function is to validate icf_ipv4_int_address_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_ipv4_int_address_st(
                            IN icf_ipv4_int_address_st *p_ipv4_addr , 
                            INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf__ipv6_int_address_st
 *
 * DESCRIPTION: This function is to validate icf_ipv6_int_address_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_ipv6_int_address_st(
                            IN icf_ipv6_int_address_st *p_ipv6_addr , 
                            INOUT  icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_codec_attrib_st
*
* DESCRIPTION:     This function is invoked to validate icf_codec_attrib_st.
*
*
******************************************************************************/
         
icf_return_t icf_vldt_icf_codec_attrib_st(
                                IN icf_codec_attrib_st *p_codec_attrib,
                                IN icf_media_type_t str_type,
                                INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_stream_list_st
 *
 * DESCRIPTION: This function is to validate icf_stream_list_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_stream_list_st(
                                  IN icf_stream_list_st *stream_l , 
                                  INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_stream_type_t
 *
 * DESCRIPTION: This function is to validate icf_stream_type_t
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_stream_type_t(
                                  IN icf_media_type_t stream_type, 
                                  INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_call_id_t
 *
 * DESCRIPTION: This function is to validate icf_call_id_t
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_call_id_t(
                     IN icf_call_id_t call_id, 
                     INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_config_action_t
 *
 * DESCRIPTION: This function is to validate icf_config_action_t
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_config_action_t(
                      IN icf_config_action_t c_action , 
                      INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_udp_t
 *
 * DESCRIPTION: This function is to validate icf_udp_t
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_udp_st(
                      IN icf_udp_st *udp, 
                      INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_config_transport_address_st
 *
 * DESCRIPTION: This function is to validate icf_config_transport_address_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_config_transport_address_st(
                      IN icf_config_transport_address_st *config_trans , 
                      INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_audio_profile_st
 *
 * DESCRIPTION: This function is to validate icf_audio_profile_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_audio_profile_st(
                       IN icf_audio_profile_st *p_audio_pro ,
                       INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_route_info_st
 *
 * DESCRIPTION: This function is to validate icf_route_info_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_route_info_st(
                       IN icf_route_info_st *p_rt, 
                       INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_amr_params_st
 *
 * DESCRIPTION: This function is to validate icf_amr_params_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_amr_params_st(
                       IN icf_amr_params_st *amrprm, 
                       INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_short_string_st
 *
 * DESCRIPTION: This function is to validate icf_short_string_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_short_string_st(
                       IN icf_short_string_st *p_s_str, 
                       INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_register_req_st
 *
 * DESCRIPTION: This function is to validate icf_register_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_register_req_st(
                        IN icf_uint8_t *p_payload, 
                        INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_terminate_call_req_st
 *
 * DESCRIPTION: This function is to validate icf_terminate_call_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_terminate_call_req_st(
                       IN icf_uint8_t *p_payload , 
                       INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_connect_req_st
 *
 * DESCRIPTION: This function is to validate icf_connect_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_connect_req_st(
                        IN icf_uint8_t *p_payload , 
                        INOUT icf_vldt_err_st *p_vldt_err);


/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_subscribe_req_st
 *
 * DESCRIPTION: This function is to validate icf_subscribe_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_subscribe_req_st(
                        IN icf_uint8_t *p_payload , 
                        INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_get_sys_settings_req_st
 *
 * DESCRIPTION: This function is to validate icf_get_sys_settings_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_get_sys_settings_req_st(
                        IN icf_uint8_t *p_payload , 
                        INOUT icf_vldt_err_st *p_vldt_err);




/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_config_transport_address_st
 *
 * DESCRIPTION: This function is to validate icf_connect_req_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_config_transport_address_st(
                        IN icf_config_transport_address_st *p_config_trans_addr, 
                        INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_config_proxy_transport_address_st
 *
 * DESCRIPTION: This function is to validate icf_config_proxy_transport_address_st
 *
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

 icf_return_t icf_vldt_icf_config_proxy_transport_address_st(
                IN icf_config_proxy_transport_address_st *p_config_trans_addr,
                INOUT icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_config_sec_transport_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  icf_config_sec_transport_address_st structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_config_sec_transport_address_st(
	IN 		icf_config_sec_transport_address_st 	*config_sec_trans, 
    INOUT 	icf_vldt_err_st 						*p_vldt_err);

 /*******************************************************************************
*
* FUNCTION:        icf_vldt_api
*
* DESCRIPTION:     This function is invoked to validate each API field 
*                  depending on the api-id 
*
*
******************************************************************************/
 icf_return_t icf_vldt_api(
                 icf_msg_st* p_msg,
                 icf_vldt_err_st *p_vldt_err);

 
/*******************************************************************************
*
* FUNCTION:        icf_vldt_mm_api
*
* DESCRIPTION:     This function is invoked to validate each API field depending*                  on the api-id 
*
*
******************************************************************************/
icf_return_t icf_vldt_mm_api(
                   icf_msg_st* p_msg,
                   icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_codec_list_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_get_codec_list_req_st api.
*
**************************************************************************/

icf_return_t icf_vldt_icf_get_codec_list_req_st(
                  IN icf_uint8_t *p_payload ,
                  INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_codec_list_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_get_codec_list_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_get_codec_list_resp_st(
                  IN icf_uint8_t *p_payload ,
                  INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_codec_commit_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_codec_commit_req_st api.
*
**************************************************************************/

icf_return_t icf_vldt_icf_codec_commit_req_st(
                       IN icf_uint8_t *p_payload ,
                       INOUT icf_vldt_err_st *p_vldt_err);

/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_codec_commit_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_codec_commit_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_codec_commit_resp_st(
                       IN icf_uint8_t *p_payload ,
                       INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_rel_res_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_rel_res_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_rel_res_req_st(
                       IN icf_uint8_t *p_payload ,
                       INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_open_media_channel_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_open_media_channel_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_open_media_channel_req_st(
                       IN icf_uint8_t *p_payload ,
                       INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_open_media_channel_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_open_media_channel_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_open_media_channel_resp_st(
                                    IN icf_uint8_t *p_payload ,
                                    INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_create_media_session_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_create_media_session_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_create_media_session_req_st(
                           IN icf_uint8_t *p_payload ,
                           INOUT icf_vldt_err_st *p_vldt_err);



/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_create_media_session_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_create_media_session_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_create_media_session_resp_st(
                                IN  icf_uint8_t *p_payload ,
                                INOUT  icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_modify_media_session_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_modify_media_session_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_modify_media_session_req_st(
                                 IN icf_uint8_t *p_payload ,
                                 INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_modify_media_session_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_modify_media_session_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_modify_media_session_resp_st(
                                 IN icf_uint8_t *p_payload ,
                                 INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_delete_media_session_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_delete_media_session_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_delete_media_session_req_st(
                                IN  icf_uint8_t *p_payload ,
                                INOUT  icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_close_media_channel_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_close_media_channel_req_st api.
*
**************************************************************************/

icf_return_t icf_vldt_icf_close_media_channel_req_st(
                                  IN icf_uint8_t *p_payload ,
                                  INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_delete_media_session_ind_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_delete_media_session_ind_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_delete_media_session_ind_st(
                                   IN icf_uint8_t *p_payload ,
                                   INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_merge_media_session_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_merge_media_session_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_merge_media_session_req_st(
                                   IN icf_uint8_t *p_payload ,
                                   INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_capabilities_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_media_capabilities_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_media_capabilities_req_st(
                                    IN icf_uint8_t *p_payload ,
                                    INOUT icf_vldt_err_st *p_vldt_err);



/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_capabilities_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_media_capabilities_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_media_capabilities_resp_st(
                                    IN icf_uint8_t *p_payload ,
                                    INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_recv_udp_data_ind_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_recv_udp_data_ind_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_recv_udp_data_ind_st(
                                     IN icf_uint8_t *p_payload ,
                                     INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_send_udpdata_req_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_send_udpdata_req_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_send_udpdata_req_st(
                                      IN  icf_uint8_t *p_payload ,
                                      INOUT  icf_vldt_err_st *p_vldt_err);


                                                     
/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_channel_st
*
* DESCRIPTION:     This function is invoked to validate icf_channel_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_channel_st(
                       IN icf_channel_st *p_channel,
                       INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_channel_addr_st
*
* DESCRIPTION:     This function is invoked to validate icf_channel_addr_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_channel_addr_st(
                       IN icf_channel_addr_st *p_channel_addr,
                       INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_req_stream_st
*
* DESCRIPTION:     This function is invoked to validate icf_req_stream_st.
*
*
**************************************************************************/

icf_return_t icf_vldt_icf_req_stream_st(
                       IN  icf_req_stream_st *p_req_stream,
                       INOUT icf_vldt_err_st *p_vldt_err);



/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_sdp_stream_st
*
* DESCRIPTION:     This function is invoked to validate icf_sdp_stream_st.
*
*
**************************************************************************/

icf_return_t icf_vldt_icf_sdp_stream_st(
                       IN icf_sdp_stream_st *p_sdp_stream,
                       INOUT icf_media_type_t stream_type,
                       INOUT icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_transport_mode_et
*
* DESCRIPTION:     This function is invoked to validate icf_transport_mode_et.
*
*
******************************************************************************/
icf_return_t icf_vldt_icf_transport_mode_et(
                      IN icf_transport_mode_t tmode,
                      INOUT icf_vldt_err_st *p_vldt_err);





/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_resp_stream_st
*
* DESCRIPTION:     This function is invoked to validate icf_resp_stream_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_resp_stream_st(
                       IN icf_resp_stream_st *p_resp_stream,
                       INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_stream_commit_st
*
* DESCRIPTION:     This function is invoked to validate icf_stream_commit_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_stream_commit_st(
                       IN icf_stream_commit_st *p_stream_commit,
                       INOUT icf_vldt_err_st *p_vldt_err);


/*************************************************************************
*
* FUNCTION:        icf_vldt_boolean
*
* DESCRIPTION:     This function is invoked to validate boolean.
*
*
**************************************************************************/
icf_return_t icf_vldt_boolean(
                       IN icf_boolean_t result, 
                       INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_result
*
* DESCRIPTION:     This function is invoked to validate result.
*
*
**************************************************************************/


icf_return_t icf_vldt_icf_result(
                       IN icf_boolean_t result, 
                       INOUT icf_vldt_err_st *p_vldt_err);



/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_codec_id_st
*
* DESCRIPTION:     This function is invoked to validate icf_codec_id_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_codec_id_st(
                                     IN icf_codec_id_st *p_codec_id,
                                     INOUT icf_vldt_err_st *p_vldt_err);


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_stream_id_list_st
*
* DESCRIPTION:     This function is invoked to validate icf_stream_id_list_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_stream_id_list_st(
                               IN icf_stream_id_list_st *p_stream_id_list,
                               INOUT icf_vldt_err_st *p_vldt_err);



/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_modify_stream_st
*
* DESCRIPTION:     This function is invoked to validate icf_modify_stream_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_modify_stream_st(
                               IN icf_modify_stream_st *p_modify_stream,
                               INOUT icf_vldt_err_st *p_vldt_err);



/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_transport_address_st
*
* DESCRIPTION:     This function is invoked to validate icf_transport_address_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_transpor_address_st(
                               IN icf_transport_address_st *p_transport_address,
                               INOUT icf_vldt_err_st *p_vldt_err);



/**************************************************************************************
*
* FUNCTION:      icf_vldt_icf_get_system_settings_req_st  
*
* DESCRIPTION:     This function is invoked to validate icf_get_system_settings_req_st 
*
*
***************************************************************************************/
icf_return_t icf_vldt_icf_get_system_settings_req_st(
                             IN icf_uint8_t *p_payload,
                             INOUT icf_vldt_err_st *p_vldt_err);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strcpy
*
*  DESCRIPTION    :   Utility function for string copy.
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_vldt_port_strcpy(
        INOUT icf_uint8_t   *p_dest,
        IN  icf_uint8_t   *p_src);


icf_return_t icf_vldt_icf_call_modify_media_data_st(
                      IN icf_call_modify_media_data_st *p_call_modify_media_data_st,
                      INOUT icf_vldt_err_st *p_vldt_err);
/******************************************************************************
*
* FUNCTION:       icf_vldt_icf_create_xconf_req_st
*      
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_create_xconf_req_st api.
*
*****************************************************************************/

icf_return_t icf_vldt_icf_create_xconf_req_st(
                            IN icf_uint8_t *p_payload,
                            INOUT icf_vldt_err_st *p_vldt_err);
/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_add_xconf_party_req_st
*      
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_add_xconf_party_req_st api.
*
*****************************************************************************/

icf_return_t icf_vldt_icf_add_xconf_party_req_st(
                            IN icf_uint8_t *p_payload,
                            INOUT icf_vldt_err_st *p_vldt_err);
/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_delete_xconf_party_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_delete_xconf_party_req_st api.
*
*****************************************************************************/

icf_return_t icf_vldt_icf_delete_xconf_party_req_st(
                            IN icf_uint8_t *p_payload,
                            INOUT icf_vldt_err_st *p_vldt_err);
/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_release_xconf_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_release_xconf_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_release_xconf_req_st(
                            IN icf_uint8_t *p_payload,
                            INOUT icf_vldt_err_st *p_vldt_err);


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_mode_t
*
* DESCRIPTION:     This function is invoked  to validate the
*                  media mode field .
*
*****************************************************************************/
icf_return_t icf_vldt_icf_media_mode_t(icf_media_mode_t media_mode,icf_vldt_err_st *p_vldt_err);

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_sip_duration_t
*
* DESCRIPTION:     This function is invoked  to validate the
*                  duration value of sip timers
*
*****************************************************************************/

icf_return_t icf_vldt_icf_sip_duration_t(
                      IN icf_duration_t duration,
                      INOUT icf_vldt_err_st *p_vldt_err);
/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_subs_head_duration_t
*
* DESCRIPTION:     This function is invoked  to validate the
*                  duration value of reg. head timer
*
*****************************************************************************/
icf_return_t icf_vldt_icf_subs_head_duration_t(
                      IN icf_duration_t duration,
                      INOUT icf_vldt_err_st *p_vldt_err);


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_local_user_alerted_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                   icf_local_user_alerted_req_st api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_local_user_alerted_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err);

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_connect_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                   icf_media_connect_req_st api.
*
*****************************************************************************/

icf_return_t icf_vldt_icf_media_connect_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err);

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_set_service_params_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_set_service_params_req_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_set_service_params_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err);



/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_config_address_st
*
* DESCRIPTION:     This function is invoked to validate 
*                   config address structure.
*
******************************************************************************/

icf_return_t icf_vldt_icf_config_address_st(
                  INOUT icf_config_address_st *p_config_addr,
                  INOUT icf_vldt_err_st *p_vldt_err);

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_set_call_params_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_set_call_params_req_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_set_call_params_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err);

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_msg_resp_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_msg_resp_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_msg_resp_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err) ;

/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_register_status_req_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_get_register_status_req_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_get_register_status_req_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err);


/******************************************************************************
*
* FUNCTION:        icf_vldt_icf_aka_auth_resp_st
*
* DESCRIPTION:     This function is invoked  to validate the
*                  icf_aka_auth_resp_st  api.
*
*****************************************************************************/
icf_return_t icf_vldt_icf_aka_auth_resp_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err);




/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_address_with_port_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  transport address structure for set_self_id .
*
******************************************************************************/

icf_return_t icf_vldt_icf_address_with_port_st(
                  IN icf_address_st *p_addr, 
                  INOUT icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_rem_call_transfer_initiated_cfm_st
*
* DESCRIPTION:     This function is invoked to validate 
*                  rem_call_transfer_initiated_cfm structure  .
*
******************************************************************************/
icf_return_t icf_vldt_icf_rem_call_transfer_initiated_cfm_st(
                           IN icf_uint8_t *p_payload,
                           INOUT icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_transport_type_et
*
* DESCRIPTION:     This Function validates the media transport type 
*                  in ICF_SET_CALL_PARAM_REQ API
* RETURN:          icf_return_t 
*
******************************************************************************/
icf_return_t icf_vldt_icf_media_transport_type_et(
  IN     icf_media_transport_mode_et   media_transport_type, 
  INOUT  icf_vldt_err_st *p_vldt_err);



/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_early_session_cfm_st
*
* DESCRIPTION:     This Function validates ICF_EARLY_SESSION_CFM API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_early_session_cfm_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err);



/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_tunnelled_create_media_session_resp_st
*
* DESCRIPTION:     This Function validates ICF_TUNNELLED_CREATE_MEDIA_SESSION_RESP API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_tunnelled_create_media_session_resp_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err);



/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_tunnelled_modify_media_session_resp_st
*
* DESCRIPTION:     This Function validates ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_tunnelled_modify_media_session_resp_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_tunnelled_media_capabilities_resp_st
*
* DESCRIPTION:     This Function validates ICF_TUNNELLED_MEDIA_CAPABILITIES_RESP API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_tunnelled_media_capabilities_resp_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err);


/****************************************************************************
* FUNCTION NAME: icf_vldt_icf_sdf_string_st	
* DESCRIPTION: This function is used to validate icf_sdf_string_st 
*              dStrLen, should be less than max. length of string allowed
*              dStrLen, character at str[str_len] is '\0'  
*              dStrLen, dStrLen !=0   
*              Str[ICF_MAX_STR_LEN], string operation!----> string length of
*              string should come 			
*              same as that of the value of dStrLen
* RETURNS: Returns either SUCCESS or FAILURE depending on the success or
*          failure of the validation
					
*****************************************************************************/

icf_return_t icf_vldt_icf_sdf_string_st(
                             IN icf_sdf_string_st *p_str, 
                             INOUT icf_vldt_err_st *p_vldt_err);

/***************************************************************************
 * FUNCTION NAME: icf_vldt_icf_sdf_address_st
 *
 * DESCRIPTION: This function is to validate icf_sdf_address_st
 * 	
 * RETURNS: Returns either SUCCESS or FAILURE depending on the success or 
 *          failure of the validation
 *
 ***************************************************************************/

icf_return_t icf_vldt_icf_sdf_address_st(
                          INOUT icf_sdf_address_st *p_addr, 
                          INOUT icf_vldt_err_st *p_vldt_err);

/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_tunnelled_sdp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_vldt_icf_tunnelled_sdp_st
*
**************************************************************************/

icf_return_t icf_vldt_icf_tunnelled_sdp_st(
                                      IN    icf_tunnelled_sdp_info_st *p_sdp ,
                                      INOUT icf_vldt_err_st *p_vldt_err);

#ifdef ICF_SESSION_TIMER
/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_session_timer_toggle_req_st
*
* DESCRIPTION:     This Function validates ICF_SESSION_TIMER_TOGGLE_REQ API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_session_timer_toggle_req_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err);
#endif

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_generic_msg_req_st
*
* DESCRIPTION:     This Function validates ICF_GENERIC_MSG_REQ API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_generic_msg_req_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_generic_msg_cfm_st
*
* DESCRIPTION:     This Function validates ICF_GENERIC_MSG_CFM API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_generic_msg_cfm_st(
                              IN icf_uint8_t *p_payload, 
                              INOUT icf_vldt_err_st *p_vldt_err);

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_app_confirmation_st
*
* DESCRIPTION:     This Function validates ICF_MESSAGE_CFM API 
* RETURN:          icf_return_t 
*
******************************************************************************/

icf_return_t icf_vldt_icf_app_confirmation_st(
                          IN icf_uint8_t *p_payload,
                          INOUT icf_vldt_err_st *p_vldt_err);

/* Hash functions prototypes end*/

/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strcmpi
*
*  DESCRIPTION    :   Utility function for performing case-insensitive
*         			  string comparison
*
*************************************************************************/
icf_uint8_t icf_vldt_port_strcmpi(
		IN  icf_uint8_t	        *p_target,
		IN  icf_uint8_t			*p_match);

/* Fix for CSR 1-7718121 : SPR-20074*/
/******************************************************************************
 * Function:     icf_vldt_sip_uri_address_string
 *
 * Description:  This function validates the input string for valid
 *               SIP/SIPS uri address.
 *****************************************************************************/
icf_return_t icf_vldt_sip_uri_address_string(INOUT  icf_uint8_t  *p_addr);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_VLDT_PROTOTYPES_H__ */

