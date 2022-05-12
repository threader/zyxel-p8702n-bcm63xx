/*------------------------------------------------------------------------------
 *
 * File name        : icf_vldt_mm_utils.c
 *
 * Purpose          : This file contains the funstions to validate the various API'
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
* FUNCTION:        icf_vldt_icf_channel_addr_st
*
* DESCRIPTION:     This function is invoked to validate icf_channel_addr_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_channel_addr_st(
                                          IN icf_channel_addr_st *p_channel_addr,
                                          INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Check: stream_type should be checked for its minimum and
           maximum values and should be checked against
           icf_vldt_stream_type validations
    */

    ret_val = icf_vldt_icf_stream_type_t(p_channel_addr->stream_type, p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_type:");
    } /* End of if ICF_FAILURE == ret_val */


     /* Check: stream_address should be validated against
        icf_transport_address_st 
     */

     if(ICF_FAILURE != ret_val)
     {   
        ret_val = icf_vldt_icf_transport_address_st(&p_channel_addr->stream_address, 
                                             p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_address:");
        } /* End of if ICF_FAILURE == ret_val */

     } /* End of if ICF_FAILURE != ret_val */


     return(ret_val);
} /* End of icf_vldt_icf_channel_addr_st */


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
                                         INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_list_st *p_temp_node = ICF_NULL;

    /* no check is required for bit_mask */
   
    /* Check: stream_address should be validated against
        icf_transport_addr_st
    */

     if(ICF_FAILURE != ret_val)
     {
        ret_val = icf_vldt_icf_transport_address_st(
                           &p_sdp_stream->stream_address,
                           p_vldt_err);
        if(ICF_FAILURE == ret_val)
        {
            /* Add error string to p_vldt_err structure */
            icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_address");
        } /* End of if ICF_FAILURE == ret_val */

     } /* if ICF_FAILURE != ret_val */ 
     
     if(ICF_SUCCESS == ret_val) 
     {
 
        if(ICF_SDP_STREAM_RTCP_INFO_PRESENT ==
           (p_sdp_stream->bit_mask & ICF_SDP_STREAM_RTCP_INFO_PRESENT))
        {
            ret_val = icf_vldt_icf_transport_address_st(
                             &p_sdp_stream->rtcp_address,
                             p_vldt_err);
     
            if(ICF_FAILURE == ret_val)
            {
                /* Add error string to p_vldt_err structure */
                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"rtcp_address:");
            } /* End of  if ICF_FAILURE == ret_val */

        }
 
    } /* if((ICF_FAILURE != ret_val) && 
          (p_sdp_stream->bit_mask & ICF_SDP_STREAM_RTCP_INFO_PRESENT)) 
       */
   
     if((ICF_FAILURE != ret_val) && 
        (p_sdp_stream->bit_mask & ICF_SDP_STREAM_MEDIA_STREAM_PRESENT))
     {
         /* Check: p_codec_info_list is of type icf_list_st and
             each node of this is of type icf_codec_attrib_st
         */
         p_temp_node = p_sdp_stream->p_codec_info_list;
         while(p_temp_node != ICF_NULL)
         {
               /* Check: p_data is of type icf_codec_attrib_st */
               ret_val = icf_vldt_icf_codec_attrib_st(
                         p_temp_node->p_data,
                         stream_type, 
                         p_vldt_err);

               if(ICF_FAILURE == ret_val)
               {
                   /* Add error string to p_vldt_err structure */
                   icf_vldt_port_strcat(p_vldt_err->err_str,
                                    (icf_uint8_t *)"p_codec_info_list:" );
                   break;
               }
               p_temp_node = p_temp_node ->p_next;
           }
     }
     
     if((ICF_FAILURE != ret_val) && 
        (p_sdp_stream->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT))
     {
         p_temp_node = p_sdp_stream->p_encoded_attributes;
         while(p_temp_node != ICF_NULL)
         {
             ret_val = icf_vldt_icf_string_st(
                       p_temp_node->p_data, 
                       p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"p_encoded_attributes:" );
                 break;
             }
             p_temp_node = p_temp_node->p_next;
          }
     }
     if((ICF_FAILURE != ret_val) &&
        (p_sdp_stream->bit_mask & ICF_SDP_STREAM_MEDIA_STREAM_PRESENT))
     {
         if(ICF_STREAM_TYPE_AUDIO == stream_type)
         {

             if(p_sdp_stream->bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)
             {
                 ret_val = icf_vldt_icf_audio_profile_st(
                                &p_sdp_stream->media_stream_param.audio_profile,
                                p_vldt_err);
                 if(ICF_FAILURE == ret_val)
                 {
                     /* Add error string to p_vldt_err structure */
                     icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"audio_profile:" );

                 }
            }
         }
         if(ICF_STREAM_TYPE_VIDEO == stream_type)
         {
             ret_val = icf_vldt_icf_video_profile_st(
                                &p_sdp_stream->media_stream_param.video_profile,
                                p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"video_profile:" );
             }

         }
         if(ICF_FAILURE != ret_val)
         {
             ret_val= icf_vldt_icf_transport_mode_et(
                               (icf_uint8_t) p_sdp_stream->trans_mode,p_vldt_err);
             if(ICF_FAILURE == ret_val)
             {
                 /* Add error string to p_vldt_err structure */
                 icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"video_profile:" );
             }
         }
     }
     return(ret_val);
} /* End of icf_vldt_icf_sdp_stream_st */


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
                                         INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    /* Check: stream_type should be checked for its minimum and
       maximum values and should be checked against        
       icf_vldt_stream_type validations
    */

    ret_val = icf_vldt_icf_stream_type_t(p_resp_stream->stream_type, p_vldt_err);
    if(ICF_FAILURE == ret_val)
    {
        /* Add error string to p_vldt_err structure */
        icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"stream_type:");
    }     /* if(ICF_FAILURE == ret_val) */

 
     /* Check: local_sdp should be validated against icf_sdp_stream_st
     */

     if(ICF_SUCCESS == ret_val)
     {
         ret_val = icf_vldt_icf_sdp_stream_st(&p_resp_stream->local_sdp,
                                                 p_resp_stream->stream_type,
                                                            p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
             /* Add error string to p_vldt_err structure */
             icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"local_sdp:");
         } /* End of if(ICF_FAILURE == ret_val) */

     } /* End of if(ICF_FAILURE != ret_val) */

     return(ret_val);
} /* End of icf_vldt_icf_resp_stream_st */

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
                                           INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_uint8_t counter=0;

     /* Check: codec_count should not be more than ICF_MAX_CODECS */
     if(ICF_MAX_CODECS < p_stream_commit->codec_count)
     {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;
 
         /*Assign Error Code to eco    de in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_INVALID_CODEC_COUNT;
         icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"codec_count:");
             
     } /* End of if(ICF_MAX_CODECS < p_stream_commit->codec_count) */
 
 
     if(ICF_FAILURE != ret_val)
     {
         /* Check: every element of array codec_name[ICF_MAX_CODECS]
            should be of type icf_codec_id_st 
         */
       for(;counter<p_stream_commit->codec_count;counter++)
       {
         ret_val = icf_vldt_icf_codec_id_st(&p_stream_commit->codec[counter],
                                         		  p_vldt_err);
         if(ICF_FAILURE == ret_val)
         {
              /* Add error string to p_vldt_err structure */
              icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"codec:");
              break;
         } /* End of  if(ICF_FAILURE == ret_val) */

       } /* End of for loop */
     }
    
    return(ret_val);
} /* End of icf_vldt_icf_stream_commit_st */ 

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
                             INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    if((ICF_TRUE != result) && (ICF_FALSE != result))
    {
        /*Assign failure to ret_val*/
        ret_val = ICF_FAILURE;

        /*Assign Error Code to ecode in p_vldt_err */
        p_vldt_err->ecode = ICF_CAUSE_INVALID_VALUE_FOR_BOOLEAN;

    } /* End of if((ICF_TRUE != result) && (ICF_FALSE != result)) */
  

    return(ret_val);
} /* End of icf_vldt_boolean */ 




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
                                INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Check: result can have two values ICF_TRUE or ICF_FALSE */

    ret_val= icf_vldt_boolean(result, p_vldt_err);

    if(ICF_FAILURE== ret_val)
    {
        /* Add error string to p_vldt_err structure */

       icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"result:");

    } /* End of  if(ICF_FAILURE== ret_val) */


    return(ret_val);
} /* End of icf_vldt_icf_result */


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
                                      INOUT icf_vldt_err_st *p_vldt_err)
{

   icf_return_t ret_val = ICF_SUCCESS;

   /* no check for codec_id */

   /* Check: codec_name should be validated against icf_short_string_st
             if it present.
   */
  
       if(p_codec_id->bit_mask & ICF_CODEC_ID_CODEC_NAME_PRESENT)
       {
           ret_val = icf_vldt_icf_short_string_st(&p_codec_id->codec_name,
                  p_vldt_err);
           if(ICF_FAILURE == ret_val )
           {
                /* Add error string to p_vldt_err structure */

                icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"codec_name:");
           } /* End of if(ICF_FAILURE == ret_val ) */

       } /* End of if(p_codec_id->bit_mask & ICF_CODEC_ID_CODEC_NAME_PRESENT) */
   
    if(ICF_SUCCESS == ret_val)
    {
        if(p_codec_id->bit_mask & ICF_CODEC_ID_CODEC_ID_PRESENT)
        {
           if(p_codec_id->codec_id > 127)
           {
               ret_val = ICF_FAILURE;
               p_vldt_err->ecode = ICF_CAUSE_INVALID_CODEC_NUM;

               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"codec_id:");
           }
        }
    }
 
   return(ret_val);

}/* End of icf_vldt_icf_codec_id_st */

/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_stream_id_list_st
*
* DESCRIPTION:     This function is invoked to validate icf_stream_id_list_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_stream_id_list_st(
                                           IN icf_stream_id_list_st *p_stream_id,
                                           INOUT icf_vldt_err_st *p_vldt_err)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Check: Count should be less than ICF_MAX_STREAM_PER_CALL */
    if(p_stream_id->count > ICF_MAX_STREAM_PER_CALL )
    {
        /*Assign failure to ret_val*/
        ret_val = ICF_FAILURE;

        /*Assign Error Code to ecode in p_vldt_err */
        p_vldt_err->ecode = ICF_CAUSE_COUNT_OUT_OF_RANGE;
    } /* if(p_stream_id->count > ICF_MAX_STREAM_PER_CALL ) */
    return(ret_val);

} /* End of icf_vldt_icf_stream_id_list_st */


/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_transport_address_st
*
* DESCRIPTION:     This function is invoked to validate icf_transport_address_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_transport_address_st(
                                             IN icf_transport_address_st *p_transport_address,
                                             INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;

     /* Check: addr should be validated against icf_transport_addr_st */

     ret_val = icf_vldt_icf_transport_addr_st(&p_transport_address->addr,
                                              p_vldt_err,ICF_FALSE);
     if(ICF_FAILURE == ret_val)
     {	
          /* Add error string to p_vldt_err structure */		
          icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr:");
     } /*  End of if(ICF_FAILURE == ret_val) */

     else
     {
          /* Check: port should be a validated against icf_vldt_port */
          ret_val = icf_vldt_port(p_transport_address->port_num,p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"port_num:");
          } /* End of   if(ICF_FAILURE == ret_val) */

     } /* End of else */
     return(ret_val);
} /* End of icf_vldt_icf_transport_address_st */

/**************************************************************************
*
* FUNCTION:        icf_vldt_icf_transport_addr_reg_st
*
* DESCRIPTION:     This function is invoked to validate icf_transport_address_st.
*
*
**************************************************************************/
icf_return_t icf_vldt_icf_transport_addr_reg_st(
                                             IN icf_transport_address_st *p_transport_address,
                                             INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;

     /* Check: addr should be validated against icf_transport_addr_st */

     ret_val = icf_vldt_icf_transport_addr_st(&p_transport_address->addr,
                                              p_vldt_err,ICF_FALSE);
     if(ICF_FAILURE == ret_val)
     {
          /* Add error string to p_vldt_err structure */
          icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"addr:");
     } /*  End of if(ICF_FAILURE == ret_val) */

     else
     {
          /* Check: port should be a validated against icf_vldt_port */
          ret_val = icf_vldt_port_reg(p_transport_address->port_num,p_vldt_err);
          if(ICF_FAILURE == ret_val)
          {
               /* Add error string to p_vldt_err structure */
               icf_vldt_port_strcat(p_vldt_err->err_str,(icf_uint8_t *)"port_num:");
          } /* End of   if(ICF_FAILURE == ret_val) */

     } /* End of else */
     return(ret_val);
} /* End of icf_vldt_icf_transport_address_st */

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
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;

     /* Check: transport_mode can have following values ICF_TRANSPORT_TYPE_UDP,
        ICF_TRANSPORT_TYPE_TCP, ICF_TRANSPORT_MODE_BOTH, ICF_TRANSPORT_TYPE_TLS
     */

     if((ICF_MAX_TRANSPORT_TYPE < tmode))
     {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_INVALID_TRANSPORT_MODE;
     }
     return(ret_val);
} 

/*******************************************************************************
*
* FUNCTION:        icf_vldt_icf_media_mode_t
*
* DESCRIPTION:     This function is invoked to validate icf_media_mode_t.
*
*
******************************************************************************/
icf_return_t icf_vldt_icf_media_mode_t(
                      IN icf_mode_t mode,
                      INOUT icf_vldt_err_st *p_vldt_err)
{
     icf_return_t ret_val = ICF_SUCCESS;

     /* Check: media mode can have following values 
        ICF_MEDIA_MODE_INACTIVE,ICF_MEDIA_MODE_SEND_ONLY,ICF_MEDIA_MODE_RECV_ONLY,
        ICF_MEDIA_MODE_SEND_RECV
     */

     if((ICF_MEDIA_MODE_INVALID == mode) || (ICF_MEDIA_MODE_SEND_RECV < mode))
     {
         /*Assign failure to ret_val*/
         ret_val = ICF_FAILURE;

         /*Assign Error Code to ecode in p_vldt_err */
         p_vldt_err->ecode = ICF_CAUSE_INVALID_MEDIA_MODE;
     }
     return(ret_val);
}

