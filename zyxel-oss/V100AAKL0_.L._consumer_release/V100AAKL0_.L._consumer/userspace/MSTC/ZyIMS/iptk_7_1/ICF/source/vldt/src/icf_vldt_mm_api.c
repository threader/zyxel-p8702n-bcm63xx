/*------------------------------------------------------------------------------
 *
 * File name        : icf_vldt_mm_api.c
 *
 * Purpose          : This file contains the funstions to validate the various MM API'
 *                    called by PA towards ICF.
 *
 * Revision History :
 *
 * Date         Name              Ref#            Comments
 * --------     ------------      ----            ---------------
 * 13-JUN-2007  Jagmohan Chauhan   No              Initial
                Rishi Saxena
 * Copyright (c) 2007, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_vldt_prototypes.h"
#include "icf_vldt_defs.h"

/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_get_codec_list_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_get_codec_list_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_get_codec_list_resp_st(
                                  IN    icf_uint8_t *p_payload ,
                                  INOUT    icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_list_st *p_temp_node;

    /* Initializing codec list resp local API structure */
    icf_get_codec_list_resp_st *p_icf_get_codec_list_resp = ICF_NULL;

    /* Assigning payload of codec list resp api to local api structure0 */
    p_icf_get_codec_list_resp = (icf_get_codec_list_resp_st *) p_payload;

    /* Create a temporary node */
     p_temp_node = ICF_NULL;
    /* Check: no check for bit_mask */

    /* Check: result should be validated against icf_vldt_icf_result_t */

    ret_val = icf_vldt_icf_result(p_icf_get_codec_list_resp->result,
                                                            p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
    } /* End of  if(ICF_FAILURE == ret_val) */


    /* Check: no check for error_cause */

    /* Check: p_reserved_stream_list should of type icf_list_st and
              each node hould be of type icf_stream_capab_st
    */

    /* Assign p_icf_get_codec_list_resp->p_reserved_stream_list to p_temp_node */
    p_temp_node = p_icf_get_codec_list_resp->p_reserved_stream_list;

    if((ICF_FAILURE != ret_val) && (p_icf_get_codec_list_resp-> result))
    {
        /* Traversing the p_stream_capab_list_st list */
        while(p_temp_node != ICF_NULL)
        {
            /*check each node is of type p_stream_capab_st */
            ret_val = icf_vldt_icf_stream_capab_st(p_temp_node->p_data,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_reserved_stream_list:");
                break;
            } /* End of if(ICF_FAILURE == ret_val) */

            p_temp_node = p_temp_node->p_next;
        } /* End of while */
    } /* End of if((ICF_FAILURE != ret_val) && (p_icf_get_codec_list_resp-> result)) */


    /* Check: rejected_stream_list should be validated against icf_stream_id_list_st */
    if(ICF_FAILURE != ret_val)
    {
         /* Check: rejected_stream_list is present or not */
         if(p_icf_get_codec_list_resp->bit_mask &
           ICF_CODEC_LIST_REJECTED_STREAM_LIST)
         {
              ret_val = icf_vldt_icf_stream_id_list_st(&p_icf_get_codec_list_resp->rejected_stream_list,
                                                                                      p_vldt_err);
              if(ICF_FAILURE == ret_val)
              {
                  /* Add error string to p_vldt_err structure */
                  icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"rejected_stream_list:");
              } /*  End of if(ICF_FAILURE == ret_val) */

         } /* End of if(p_icf_get_codec_list_resp->bit_mask &
           ICF_CODEC_LIST_REJECTED_STREAM_LIST) */

    } /* End of if(ICF_FAILURE != ret_val) */


    return(ret_val);
} /* End of icf_vldt_icf_get_codec_list_resp_st */



/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_codec_commit_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_codec_commit_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_codec_commit_resp_st(
                                 IN     icf_uint8_t *p_payload ,
                                 INOUT     icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS;
    icf_list_st *p_temp_node = ICF_NULL;

    /* Initializing codec commit resp local API structure */
    icf_codec_commit_resp_st *p_icf_codec_commit_resp = ICF_NULL;

    /* Assigning payload of codec commit resp api to local api structure0 */
    p_icf_codec_commit_resp = (icf_codec_commit_resp_st *) p_payload;
 
    /* Check: no check for bit_mask */

    /* Check: result should be validated against icf_vldt_icf_result_t */
    ret_val = icf_vldt_icf_result(p_icf_codec_commit_resp->result,
                                                            p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
    } /*  End of if(ICF_FAILURE == ret_val) */

   
    /* Check: no check for error_cause */

    /* Check: p_commited_stream_list should of type icf_list_st and
       each node hould be of type icf_stream_commit_st
    */

    /* Assign p_icf_codec_commit_resp->p_commited_stream_list to p_temp_node */

    p_temp_node = p_icf_codec_commit_resp->p_commited_stream_list;

    if(ICF_FAILURE != ret_val)
    {
        /* Traversing the p_commited_stream_list list */
        while(p_temp_node != ICF_NULL)
        { 
            /*check each node is of type p_stream_capab_st */
            ret_val = icf_vldt_icf_stream_commit_st(p_temp_node->p_data,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_commited_stream_list:");
                break;
            } /* End of if(ICF_FAILURE == ret_val) */

            p_temp_node=p_temp_node->p_next;
        } /* End of while loop */

    } /* End of if((ICF_FAILURE != ret_val) && (p_icf_codec_commit_resp->result)) */

    /* Check: rejected_stream_list should be validated against icf_stream_id_list_st */
    if((ICF_FAILURE != ret_val) && 
    (p_icf_codec_commit_resp->bit_mask & ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT))
    {
         ret_val = icf_vldt_icf_stream_id_list_st(&p_icf_codec_commit_resp->rejected_stream_list,
                                                                                      p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"rejected_stream_list:");
         } /*  End of if(ICF_FAILURE == ret_val) */

     } /* if((ICF_FAILURE != ret_val) && 
        (p_icf_codec_commit_resp->bit_mask & ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)) */


     return(ret_val);
} /* End of icf_vldt_icf_codec_commit_resp_st */


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_open_media_channel_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_open_media_channel_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_open_media_channel_resp_st(
                                            IN     icf_uint8_t *p_payload ,
                                            INOUT     icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_list_st *p_temp_node = ICF_NULL;
   
    /* Initializing open media channel resp local API structure */
    icf_open_media_channel_resp_st *p_icf_open_media_channel_resp = ICF_NULL;

    /* Assigning payload of open media channel resp api to local api structure */
    p_icf_open_media_channel_resp = (icf_open_media_channel_resp_st *) p_payload;

    
    /* Check: result should be validated against icf_vldt_icf_result_t */

    ret_val = icf_vldt_icf_result(p_icf_open_media_channel_resp->result,
                                                            p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
    } /* End of if(ICF_FAILURE == ret_val) */


    /* Check: no check is required for error_cause */

    /* Check: p_channel_addr_list should be of type icf_list_st and
              each node hould be of type icf_channel_addr_st
    */
    /* Assign p_icf_open_media_channel_resp->p_channel_addr_list to p_temp_node */

    p_temp_node = p_icf_open_media_channel_resp->p_channel_addr_list;

    if((ICF_FAILURE != ret_val) && (p_icf_open_media_channel_resp->result))
    {
        /* Traversing the p_channel_addr_list list */
        while(p_temp_node != ICF_NULL)
        {
            /*check each node is of type p_channel_addr_st */
            ret_val = icf_vldt_icf_channel_addr_st(p_temp_node->p_data,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_channel_addr_list:");
                break;
            } /* End of if(ICF_FAILURE == ret_val) */

            p_temp_node=p_temp_node->p_next;
        } /* End of while */
    } /* End of if((ICF_FAILURE != ret_val) && (p_icf_open_media_channel_resp->result)) */

    return(ret_val);
} /* End of  icf_vldt_icf_open_media_channel_resp_st */


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_create_media_session_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_create_media_session_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_create_media_session_resp_st(
                                      IN    icf_uint8_t *p_payload ,
                                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     icf_list_st *p_temp_node = ICF_NULL;

     /* Initializing create media session resp local API structure */
     icf_create_media_session_resp_st *p_icf_create_media_session_resp = ICF_NULL;

     /* Assigning payload of create media session resp api to local api structure0 */
     p_icf_create_media_session_resp = (icf_create_media_session_resp_st *) p_payload;

     /* Check: result should be validated against icf_vldt_icf_result_t */

     ret_val = icf_vldt_icf_result(p_icf_create_media_session_resp->result,
                                                            p_vldt_err);
     if(ICF_FAILURE == ret_val)
     {
         /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
     } /* End of if(ICF_FAILURE == ret_val) */


     /* Check: no check for error_cause */

     /* check: no check for bit_mask */

     /* Check: p_accepted_stream_list should of type icf_list_st and
        each node hould be of type icf_resp_stream_st
     */

     /* Assign p_icf_create_media_session_resp->p_accepted_stream_list to p_temp_node */

     p_temp_node = p_icf_create_media_session_resp->p_accepted_stream_list;

     if(ICF_FAILURE != ret_val)
     {
         /* Traversing the p_accepted_stream_list list */
         while(p_temp_node != ICF_NULL)
         {
             /*check each node is of type icf_resp_stream_st */
             ret_val = icf_vldt_icf_resp_stream_st(p_temp_node->p_data,p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_accepted_stream_list:");
                 break;
             } /* End of if(ICF_FAILURE == ret_val) */

             p_temp_node=p_temp_node->p_next;
         } /* End of while loop */

     } /* End of if((ICF_FAILURE != ret_val) && (p_icf_create_media_session_resp->result)) */

     
     /* Check: rejected_stream_list should be validated against icf_stream_id_list_st */
     if(ICF_FAILURE != ret_val)
     {
         if(p_icf_create_media_session_resp->bit_mask & ICF_CMS_REJECT_STREAM_PRESENT)
         {
             ret_val = icf_vldt_icf_stream_id_list_st(&p_icf_create_media_session_resp->rejected_stream_list, p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"rejected_stream_list:");

             } /*  End of if(ICF_FAILURE == ret_val) */
        }
    }

         return(ret_val);
} /* End of icf_vldt_icf_create_media_session_resp_st */


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_modify_media_session_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_modify_media_session_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_modify_media_session_resp_st(
                                    IN  icf_uint8_t *p_payload ,
                                    INOUT  icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_list_st *p_temp_node = ICF_NULL;

     /* Initializing modify media session resp local API structure */
    icf_modify_media_session_resp_st *p_icf_modify_media_session_resp = ICF_NULL;

    /* Assigning payload of modify media session resp to local api structure */
    p_icf_modify_media_session_resp = (icf_modify_media_session_resp_st *) p_payload;

    /* Check: result should be validated against icf_vldt_icf_result_t */

    ret_val = icf_vldt_icf_result(p_icf_modify_media_session_resp->result,
                                                            p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
    } /* if(ICF_FAILURE == ret_val) */


    /* Check: no check for bit_mask */

    /* Check: no check for error_cause */

    /* Check: p_accepted_stream_list should of type icf_list_st and
       each node hould be of type icf_resp_stream_st
    */
    
    /* Assign p_icf_modify_media_session_resp->p_accepted_stream_list to p_temp_node */
    p_temp_node = p_icf_modify_media_session_resp->p_accepted_stream_list;

    if((p_icf_modify_media_session_resp->result) && (ICF_FAILURE != ret_val))
    {
        /* Traversing the p_accepted_stream_list list */
        while(p_temp_node != ICF_NULL)
        { 
            /*check each node is of type p_stream_capab_st */
            ret_val = icf_vldt_icf_resp_stream_st(p_temp_node->p_data,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_accepted_stream_list:");
                break;
            } /* End of if(ICF_FAILURE == ret_val) */

            p_temp_node=p_temp_node->p_next;
        } /* End of while */

    } /* End of if((p_icf_modify_media_session_resp->result) && (ICF_FAILURE != ret_val)) */



    /* Check: rejected_stream_list should be validated against icf_stream_id_list_st
    */
    if(ICF_SUCCESS == ret_val)
    {
        if(p_icf_modify_media_session_resp->bit_mask & ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT)
        {
             ret_val= icf_vldt_icf_stream_id_list_st(
                  &p_icf_modify_media_session_resp->rejected_stream_list, p_vldt_err);

             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"rejected_stream_list:");
             } /* End of if(ICF_FAILURE == ret_val) */

         } /* End of if(p_icf_modify_media_session_resp->bit_mask & ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT) */
    }

     return(ret_val);
} /* End of icf_vldt_icf_modify_media_session_resp_st */
  


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_delete_media_session_ind_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_delete_media_session_ind_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_delete_media_session_ind_st(
                                     IN  icf_uint8_t *p_payload ,
                                     INOUT icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS;

    /* Initializing delete media session ind local API structure */
    icf_delete_media_session_ind_st *p_icf_delete_media_session_ind = ICF_NULL;

    /* Assigning payload of delete media session ind api to local api structure0 */
    p_icf_delete_media_session_ind = (icf_delete_media_session_ind_st *) p_payload;

    /* Check: stream_list should be validated against icf_stream_id_list_st
    */
    ret_val = icf_vldt_icf_stream_id_list_st(&p_icf_delete_media_session_ind->stream_list,
                                                            p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_list:");
    } /* End of if(ICF_FAILURE == ret_val) */


    /* Check: no check for bit_mask */

    /* check: no check for error_cause */

    return(ret_val);
} /* End of icf_vldt_icf_delete_media_session_ind_st *icf_vldt_icf_delete_media_session_ind_st*/


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_capabilities_resp_st
*
* DESCRIPTION:     This function is invoked to validate
*                  icf_media_capabilities_resp_st api.
*
**************************************************************************/
icf_return_t icf_vldt_icf_media_capabilities_resp_st(
                                     IN  icf_uint8_t *p_payload ,
                                     INOUT  icf_vldt_err_st *p_vldt_err)
{

    icf_return_t ret_val = ICF_SUCCESS;
    icf_list_st *p_temp_node;

    /* Initializing media capabilities resp local API structure */
    icf_media_capabilities_resp_st *p_icf_media_capabilities_resp = ICF_NULL;

    /* Assigning payload of media capabilities resp api to local api structure */
    p_icf_media_capabilities_resp = (icf_media_capabilities_resp_st *) p_payload;

    /* Check: result should be validated against icf_vldt_icf_result_t */

    ret_val = icf_vldt_icf_result(p_icf_media_capabilities_resp->result,
                                                            p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");
    }/* End of if(ICF_FAILURE == ret_val) */
 

    /* Check: no check for error_cause */

    /* Check: p_resp_stream_list should of type icf_list_st and
       each node hould be of type icf_resp_stream_st
    */

    /* Assign p_icf_media_capabilities_resp->p_resp_stream_list to p_temp_node */

    p_temp_node = p_icf_media_capabilities_resp->p_resp_stream_list;

    if((p_icf_media_capabilities_resp->result) && (ICF_FAILURE != ret_val))
    {
        /* Traversing the  list */
        while(p_temp_node != ICF_NULL)
        { 
            /*check each node is of type icf_resp_stream_st */
            ret_val = icf_vldt_icf_resp_stream_st(p_temp_node->p_data,p_vldt_err);
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_resp_stream_list:");
                break;
            } /* End of if(ICF_FAILURE == ret_val) */

            p_temp_node=p_temp_node->p_next;
        } /* End of while */
    } /* End of  if((p_icf_media_capabilities_resp->result) && (ICF_FAILURE != ret_val)) */
    return(ret_val);
} /* End of  icf_vldt_icf_media_capabilities_resp_st */

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
                                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;
     /* icf_string_st temp_str; */

     /* Initializing recieve udp data indication local API structure */
     icf_recv_udp_data_ind_st *p_icf_recv_udp_data_ind = ICF_NULL;

     /* Assigning payload of recieve udp data indication api to local api structure0 */
     p_icf_recv_udp_data_ind = (icf_recv_udp_data_ind_st *) p_payload;
 
     /* Check: dest_port should be validated against icf_vldt_port */

     ret_val = icf_vldt_port(p_icf_recv_udp_data_ind->dest_port, p_vldt_err);
     if(ICF_FAILURE == ret_val)
     {
         /* Add error string to p_vldt_err structure */
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"dest_port:");
     } /*  End of if(ICF_FAILURE == ret_val) */

     /* Check: source_addr should be validated against icf_transport_address_st      */
     if(ICF_FAILURE != ret_val)
     {

         ret_val = icf_vldt_icf_transport_address_st(&p_icf_recv_udp_data_ind->source_addr, p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"source_addr:");
         } /* End of if(ICF_FAILURE == ret_val) */

     } /*End of  if(ICF_FAILURE != ret_val) */

     return(ret_val);
} /* End of icf_vldt_icf_recv_udp_data_ind_st */

 
