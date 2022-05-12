/*******************************************************************************
*                                        
* FILE NAME    :icf_pack_mm_api.c
*                                        
* DESCRIPTION  :The file contains the routines for the buffer packing code.
*               Fraction of this code is automatically generated.
*                                        
* Revision History :                
*                
*   DATE             NAME           REFERENCE              REASON
* -----------    ---------------  -------------      ------------------
* 05-JUL-2005     Raveen Sharma	   ICF REL 6.0		       Initial
* 04-Jan-2007     Amit Mahajan                           MultiLine changes.
* 11-Feb-2009     Ashutosh Mohan   IPTK Rel8.1       Added code for following functions:
*                                                icf_pack_icf_tunnelled_create_media_session_req_st
*                                                icf_pack_icf_tunnelled_create_media_session_resp_st
*                                                icf_pack_icf_tunnelled_modify_media_session_req_st
*                                                icf_pack_icf_tunnelled_media_capabilities_resp_st
*                                                icf_pack_icf_tunnelled_modify_media_session_resp_st
* 04-Mar-2009 Saurabh Gupta	      SPR 19349      Klocworks fix
*										
* Copyright 2006, Aricent.                       
*******************************************************************************/



#include "icf_clib_mm.h"
#include "icf_utils.h"
#include "icf_mm_api.h"
#include "icf_api.h"

/***************************************************

			RESOURCE MANAGEMENT APIs

****************************************************/
icf_uint32_t
icf_pack_icf_encoded_attributes_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_encoded_attributes_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_encoded_attributes_st*/
    ICF_CLIB_TRACE(("Packing list of icf_encoded_attributes_st\n"));
    
	/*Pack count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_encoded_attributes_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_string_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_amr_params_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;

	icf_amr_params_st *p_icf_amr_params_st = (icf_amr_params_st *)p_src;

	/* This function packs icf_audio_profile_st */
    ICF_CLIB_TRACE(("Packing icf_audio_profile_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_amr_params_st->bit_mask), &offset);

	*p_length += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_amr_params_st->amr_initial_mode), &offset);

	*p_length += offset;

	if(p_icf_amr_params_st->bit_mask & ICF_AMR_MODE_LIST_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_amr_params_st->amr_mode_count), &offset);

		*p_length += offset;

		while((count<p_icf_amr_params_st->amr_mode_count) && (count < ICF_MAX_AMR_MODE))
		{
			icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_amr_params_st->amr_mode_list[count]), &offset);

			*p_length += offset;

			count++;
		}
	}

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_amr_params_st->codec_mode), &offset);

	*p_length += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_amr_params_st->crc_enabled), &offset);

	*p_length += offset;

	return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_audio_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_audio_codec_param_st    *p_icf_audio_codec_param_st = ICF_NULL;
    
    p_icf_audio_codec_param_st    = (icf_audio_codec_param_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_audio_codec_param_st */
    ICF_CLIB_TRACE(("Packing icf_audio_codec_param_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_codec_param_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_CHANNEL)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_codec_param_st->channels), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_codec_param_st->sampling_rate), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_AMR_PARAM)
	{
		icf_pack_icf_amr_params_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_codec_param_st->amr_params), &offset);
    
		*p_length    += offset;
	}

        if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_VBD_VAL)
        {
           icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_codec_param_st->vbd_val), &offset);

        *p_length    += offset;
        }

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_video_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_video_codec_param_st    *p_icf_video_codec_param_st = ICF_NULL;
    
    p_icf_video_codec_param_st    = (icf_video_codec_param_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_video_codec_param_st */
    ICF_CLIB_TRACE(("Packing icf_video_codec_param_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_codec_param_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_codec_param_st->clock_rate), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_FRAMESIZE_PRESENT)
	{
		icf_pack_icf_size_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_codec_param_st->frame_size), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_size_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_size_st    *p_icf_size_st = ICF_NULL;
    
    p_icf_size_st    = (icf_size_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_size_st */
    ICF_CLIB_TRACE(("Packing icf_size_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_size_st->height), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_size_st->width), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_config_video_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
   
    icf_config_video_codec_param_st    *p_icf_config_video_codec_param_st = ICF_NULL;
    
    p_icf_config_video_codec_param_st    = (icf_config_video_codec_param_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_video_codec_param_st */
    ICF_CLIB_TRACE(("Packing icf_config_video_codec_param_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_video_codec_param_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_FRAME_RATE_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_video_codec_param_st->frame_rate), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_ENC_QUALITY_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_video_codec_param_st->encoding_quality), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_BANDWIDTH_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_video_codec_param_st->bandwidth), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_CLOCK_RATE_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_video_codec_param_st->num_of_supp_clock_rate), &offset);
    
		*p_length    += offset;
		
		while((count<p_icf_config_video_codec_param_st->num_of_supp_clock_rate) && (count < ICF_MAX_SUP_CLOCK_RATE))
		{
			icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_video_codec_param_st->clock_rate[count]), &offset);
    
			*p_length    += offset;

			count++;
		}
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_FRAMESIZE_PRESENT)
	{
		icf_pack_icf_size_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_video_codec_param_st->frame_size), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_config_t38_attrib_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_config_t38_attrib_param_st    *p_icf_config_t38_attrib_param_st = ICF_NULL;
    
    p_icf_config_t38_attrib_param_st    = (icf_config_t38_attrib_param_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_t38_attrib_param_st */
    ICF_CLIB_TRACE(("Packing icf_config_t38_attrib_param_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_VERSION)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->fax_version), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_BITRATE)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->bitrate), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_RATE_MANAGEMENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->rate_management), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_UDP_BUFFSIZE)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->udp_buffersize), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_UDP_PACKETSIZE)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->udp_packetsize), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_ERR_CORRECTION)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->err_correction_method), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_codec_attrib_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length,
	icf_uint8_t	   stream_type
)
{
    icf_uint32_t        offset = 0;

	icf_codec_attrib_st    *p_icf_codec_attrib_st = ICF_NULL;
    
    p_icf_codec_attrib_st    = (icf_codec_attrib_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_codec_attrib_st */
    ICF_CLIB_TRACE(("Packing icf_codec_attrib_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_attrib_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_codec_attrib_st->bit_mask & ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_attrib_st->codec_num), &offset);
    
		*p_length    += offset;
	}

	icf_pack_icf_short_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_attrib_st->codec_name), &offset);
    
    *p_length    += offset;

	if(p_icf_codec_attrib_st->bit_mask & ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT)
	{
		switch(stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:

				  icf_pack_icf_audio_codec_param_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_attrib_st->codec_params), &offset);
    
				  *p_length    += offset;

				  break;
		
			case  ICF_STREAM_TYPE_VIDEO:

				  icf_pack_icf_video_codec_param_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_attrib_st->codec_params), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_STREAM_TYPE_FAX:

				  icf_pack_icf_config_t38_attrib_param_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_attrib_st->codec_params), &offset);
    
				  *p_length    += offset;

				  break;

		}
	}

	if(p_icf_codec_attrib_st->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)
	{
		icf_pack_icf_encoded_attributes_list_st(p_dest + (*p_length), (p_icf_codec_attrib_st->p_encoded_attributes), &offset);
	    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_codec_attrib_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_codec_attrib_list_st,
    icf_uint32_t   *p_length,
	icf_uint8_t	   stream_type
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_codec_attrib_st*/
    ICF_CLIB_TRACE(("Packing list of icf_codec_attrib_st\n"));
    
	/*Pack count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_codec_attrib_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_codec_attrib_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset, stream_type);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_codec_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint32_t        count = 0;

	icf_codec_info_st *p_icf_codec_info_st = (icf_codec_info_st *)p_src;

	/* This function packs icf_codec_info_st */
    ICF_CLIB_TRACE(("Packing icf_codec_info_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_info_st->codec_count), &offset);

	*p_length += offset;


	while((count<p_icf_codec_info_st->codec_count) && (count < ICF_MAX_CODECS))
	{
		icf_pack_icf_short_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_info_st->codec_name[count]), &offset);
		
		*p_length += offset;

		count++;

	}
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_audio_profile_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_audio_profile_st *p_icf_audio_profile_st = (icf_audio_profile_st *)p_src;

	/* This function packs icf_audio_profile_st */
    ICF_CLIB_TRACE(("Packing icf_audio_profile_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_profile_st->bit_mask), &offset);

	*p_length += offset;

	if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_profile_st->bandwidth), &offset);

		*p_length += offset;
	}

	if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_profile_st->pref_ptime), &offset);

		*p_length += offset;
	}

	if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_profile_st->silence_suppression), &offset);

		*p_length += offset;
	}
      	if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_ECHO_CANC_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_audio_profile_st->echo_canc), &offset);

		*p_length += offset;
	}
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_video_profile_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_video_profile_st *p_icf_video_profile_st = (icf_video_profile_st *)p_src;

	/* This function packs icf_video_profile_st */
    ICF_CLIB_TRACE(("Packing icf_video_profile_st\n"));

	/* Initialise length to 0 */
    *p_length    = 0;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_profile_st->bit_mask), &offset);

	*p_length += offset;

	if(p_icf_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_profile_st->encoding_quality), &offset);

		*p_length += offset;
	}

	if(p_icf_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_profile_st->frame_rate), &offset);

		*p_length += offset;
	}

	if(p_icf_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_profile_st->bandwidth), &offset);

		*p_length += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_t_38_media_profile_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

	icf_uint32_t        offset = 0;
	   
	icf_t_38_media_profile_st  *p_icf_t_38_media_profile_st = (icf_t_38_media_profile_st *)p_src;

	/* This function packs icf_t_38_media_profile_st */
    ICF_CLIB_TRACE(("Packing icf_t_38_media_profile_st\n"));


	/* Initialise length to 0 */
    *p_length    = 0;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_t_38_media_profile_st->max_bit_rate), &offset);

	*p_length += offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_stream_capab_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

	icf_stream_capab_st    *p_icf_stream_capab_st = ICF_NULL;
    
    p_icf_stream_capab_st    = (icf_stream_capab_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stream_capab_st */
    ICF_CLIB_TRACE(("Packing icf_stream_capab_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_capab_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_capab_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_capab_st->stream_type), &offset);
    
    *p_length    += offset;

	if(p_icf_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
	{
		icf_pack_icf_codec_attrib_list_st(p_dest + (*p_length), (p_icf_stream_capab_st->p_codec_info_list), &offset, p_icf_stream_capab_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT)
	{
		switch(p_icf_stream_capab_st->stream_type)
		{
	
			case  ICF_STREAM_TYPE_AUDIO:

				  icf_pack_icf_audio_profile_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_capab_st->media_stream_param), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_STREAM_TYPE_VIDEO:

				  icf_pack_icf_video_profile_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_capab_st->media_stream_param), &offset);
    
				  *p_length    += offset;

				  break;

		}
	}

	if(p_icf_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
	{
		icf_pack_icf_encoded_attributes_list_st(p_dest + (*p_length), (p_icf_stream_capab_st->p_encoded_attributes), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}


icf_uint32_t
icf_pack_icf_stream_capab_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_stream_capab_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;
  
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_stream_capab_st*/
    ICF_CLIB_TRACE(("Packing list of icf_stream_capab_st\n"));
    
	/*Pack count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_stream_capab_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_stream_capab_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}
	
	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_get_codec_list_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_get_codec_list_req_st    *p_icf_get_codec_list_req_st = ICF_NULL;
    
    p_icf_get_codec_list_req_st    = (icf_get_codec_list_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_codec_list_req_st */
    ICF_CLIB_TRACE(("Packing icf_get_codec_list_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_codec_list_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_codec_list_req_st->line_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_get_codec_list_req_st->p_stream_capab_list), &offset);
    
    *p_length    += offset;

	if(ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT & p_icf_get_codec_list_req_st->bit_mask)
	{
		icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_get_codec_list_req_st->p_local_stream_capab_list), &offset);

		*p_length    += offset;
	}

    return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_config_audio_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_config_audio_codec_param_st    *p_icf_config_audio_codec_param_st = ICF_NULL;
    
    p_icf_config_audio_codec_param_st    = (icf_config_audio_codec_param_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_audio_codec_param_st */
    ICF_CLIB_TRACE(("Packing icf_config_audio_codec_param_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_CHANNEL)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->channels), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PTIME)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->num_of_supported_ptime), &offset);
    
		*p_length    += offset;

		while((count<p_icf_config_audio_codec_param_st->num_of_supported_ptime) && (count < ICF_MAX_SUP_PTIME))
		{
			icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->ptime[count]), &offset);
    
			*p_length    += offset;

			count++;
		}
#if 1 /*ZyXEL porting*/
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->pref_ptime), &offset);
    
		*p_length    += offset;	
#endif		
	}

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_BANDWIDTH)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->bandwidth), &offset);
    
		*p_length    += offset;
	}

	count = 0;

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_SAMPLING_RATE)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->num_of_supp_sampling_rate), &offset);
    
		*p_length    += offset;

		while((count<p_icf_config_audio_codec_param_st->num_of_supp_sampling_rate) && (count < ICF_MAX_SUP_SAMPLING_RATE)) 
		{
		
			icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->sampling_rate[count]), &offset);
    
			*p_length    += offset;

			count++;
		}
	}

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_SILENCE_SUPPRESSION)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->silence_suppression), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_AMR_PARAM)
	{
		icf_pack_icf_amr_params_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_audio_codec_param_st->amr_params), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_get_codec_list_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_get_codec_list_resp_st    *p_icf_get_codec_list_resp_st = ICF_NULL;
    
    p_icf_get_codec_list_resp_st    = (icf_get_codec_list_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_codec_list_resp_st */
    ICF_CLIB_TRACE(("Packing icf_get_codec_list_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_codec_list_resp_st->result), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_codec_list_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(!p_icf_get_codec_list_resp_st->result)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_codec_list_resp_st->error_cause), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_get_codec_list_resp_st->result)
	{
		icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_get_codec_list_resp_st->p_reserved_stream_list), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_get_codec_list_resp_st->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_codec_list_resp_st->rejected_stream_list), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_codec_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_codec_id_st    *p_icf_codec_id_st = ICF_NULL;
    
    p_icf_codec_id_st    = (icf_codec_id_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_codec_id_st */
    ICF_CLIB_TRACE(("Packing icf_codec_id_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_id_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_codec_id_st->bit_mask & ICF_CODEC_ID_CODEC_NAME_PRESENT)
	{
		icf_pack_icf_short_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_id_st->codec_name), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_codec_id_st->bit_mask & ICF_CODEC_ID_CODEC_ID_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_id_st->codec_id), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_stream_commit_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
   
    icf_stream_commit_st    *p_icf_stream_commit_st = ICF_NULL;
    
    p_icf_stream_commit_st    = (icf_stream_commit_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stream_commit_st */
    ICF_CLIB_TRACE(("Packing icf_stream_commit_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_commit_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_commit_st->codec_count), &offset);
    
    *p_length    += offset;

	while((count<p_icf_stream_commit_st->codec_count) && (count < ICF_MAX_CODECS))
	{
		icf_pack_icf_codec_id_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stream_commit_st->codec[count]), &offset);
    
		*p_length    += offset;

		count++;

	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_stream_commit_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_stream_commit_req_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;
   
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_stream_commit_req_st*/
    ICF_CLIB_TRACE(("Packing list of icf_stream_commit_req_st\n"));
    
	/*Pack the count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_stream_commit_req_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_stream_commit_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}


icf_uint32_t	
icf_pack_icf_codec_commit_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_codec_commit_req_st    *p_icf_codec_commit_req_st = ICF_NULL;
    
    p_icf_codec_commit_req_st    = (icf_codec_commit_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_codec_commit_req_st */
    ICF_CLIB_TRACE(("Packing icf_codec_commit_req_st\n"));
    
    icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_codec_commit_req_st->p_stream_commit_list), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}


icf_uint32_t
icf_pack_icf_codec_commit_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_codec_commit_resp_st    *p_icf_codec_commit_resp_st = ICF_NULL;
    
    p_icf_codec_commit_resp_st    = (icf_codec_commit_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_codec_commit_resp_st */
    ICF_CLIB_TRACE(("Packing icf_codec_commit_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_commit_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_commit_resp_st->result), &offset);
    
    *p_length    += offset;

	if(!p_icf_codec_commit_resp_st->result)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_commit_resp_st->error_cause), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_codec_commit_resp_st->result)
	{
		icf_pack_icf_stream_commit_list_st(p_dest + (*p_length), (p_icf_codec_commit_resp_st->p_commited_stream_list), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_codec_commit_resp_st->bit_mask & ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_codec_commit_resp_st->rejected_stream_list), &offset);
    
		*p_length    += offset;
	}
	
	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_rel_res_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_rel_res_req_st    *p_icf_rel_res_req_st = ICF_NULL;
    
    p_icf_rel_res_req_st    = (icf_rel_res_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_rel_res_req_st */
    ICF_CLIB_TRACE(("Packing icf_rel_res_req_st\n"));
    
    icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rel_res_req_st->stream_list), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_channel_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_channel_st    *p_icf_channel_st = ICF_NULL;
    
    p_icf_channel_st    = (icf_channel_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_channel_st */
    ICF_CLIB_TRACE(("Packing icf_channel_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_channel_st->stream_type), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_channel_st->stream_id), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_channel_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_channel_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;
   
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_channel_st*/
    ICF_CLIB_TRACE(("Packing list of icf_channel_st\n"));
    
	/*Pack the count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_channel_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_channel_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_open_media_channel_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_open_media_channel_req_st    *p_icf_open_media_channel_req_st = ICF_NULL;
    
    p_icf_open_media_channel_req_st    = (icf_open_media_channel_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_open_media_channel_req_st */
    ICF_CLIB_TRACE(("Packing icf_open_media_channel_req_st\n"));
    
    icf_pack_icf_channel_list_st(p_dest + (*p_length), (p_icf_open_media_channel_req_st->p_channel_list), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}


icf_uint32_t
icf_pack_icf_channel_addr_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_channel_addr_st    *p_icf_channel_addr_st = ICF_NULL;
    
    p_icf_channel_addr_st    = (icf_channel_addr_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_channel_addr_st */
    ICF_CLIB_TRACE(("Packing icf_channel_addr_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_channel_addr_st->stream_type), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_channel_addr_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_channel_addr_st->stream_address), &offset);

	*p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_channel_addr_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_channel_addr_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;
   
    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_channel_addr_st*/
    ICF_CLIB_TRACE(("Packing list of icf_channel_addr_st\n"));
    
	/*Pack the count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_channel_addr_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_channel_addr_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_open_media_channel_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_open_media_channel_resp_st    *p_icf_open_media_channel_resp_st = ICF_NULL;
    
    p_icf_open_media_channel_resp_st    = (icf_open_media_channel_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_open_media_channel_resp_st */
    ICF_CLIB_TRACE(("Packing icf_open_media_channel_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_open_media_channel_resp_st->result), &offset);
    
    *p_length    += offset;

	if(!p_icf_open_media_channel_resp_st->result)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_open_media_channel_resp_st->error_cause), &offset);
    
	    *p_length    += offset;
	}

	if(p_icf_open_media_channel_resp_st->result)
	{
		icf_pack_icf_channel_addr_list_st(p_dest + (*p_length), (p_icf_open_media_channel_resp_st->p_channel_addr_list), &offset);
    
		*p_length    += offset;
	}


	return ICF_SUCCESS;

}

/***************************************************

			     MEDIA CONTROL APIs

****************************************************/

icf_uint32_t
icf_pack_icf_sdp_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length,
	icf_uint8_t   stream_type
)
{
	icf_uint32_t       offset = 0;
   
    icf_sdp_stream_st    *p_icf_sdp_stream_st = ICF_NULL;
    
    p_icf_sdp_stream_st    = (icf_sdp_stream_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_sdp_stream_st */
    ICF_CLIB_TRACE(("Packing icf_sdp_stream_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdp_stream_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_icf_codec_attrib_list_st(p_dest + (*p_length), (p_icf_sdp_stream_st->p_codec_info_list), &offset, stream_type);
    
    *p_length    += offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdp_stream_st->stream_address), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdp_stream_st->trans_mode), &offset);
    
    *p_length    += offset;

	if(p_icf_sdp_stream_st->bit_mask & ICF_SDP_STREAM_MEDIA_STREAM_PRESENT)
	{
		switch(stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
			
		     	  icf_pack_icf_audio_profile_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdp_stream_st->media_stream_param), &offset);
    
				  *p_length    += offset;

				  break;
			  

			case  ICF_STREAM_TYPE_VIDEO:

				  icf_pack_icf_video_profile_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdp_stream_st->media_stream_param), &offset);
    
				  *p_length    += offset;
	
				  break;

		}
	}

	if(p_icf_sdp_stream_st->bit_mask & ICF_SDP_STREAM_RTCP_INFO_PRESENT)
	{
		icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_sdp_stream_st->rtcp_address), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_sdp_stream_st->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
	{
		icf_pack_icf_encoded_attributes_list_st(p_dest + (*p_length), (p_icf_sdp_stream_st->p_encoded_attributes), &offset);
    
		*p_length    += offset;
	}


	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_req_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	 icf_uint32_t        offset = 0;
   
    icf_req_stream_st    *p_icf_req_stream_st = ICF_NULL;
    
    p_icf_req_stream_st    = (icf_req_stream_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_req_stream_st */
    ICF_CLIB_TRACE(("Packing icf_req_stream_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->stream_type), &offset);
    
    *p_length    += offset;

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT)
	{
		icf_pack_icf_sdp_stream_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->remote_sdp), &offset, p_icf_req_stream_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT)
	{
		icf_pack_icf_sdp_stream_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->local_sdp), &offset, p_icf_req_stream_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_STREAM_MODE_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->mode), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->qos_mark), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_RTP_DETECTION_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->rtp_detection_reqd), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_ECHO_CAN_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_req_stream_st->echo_cancellation), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_req_stream_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_req_stream_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_req_stream_st*/
    ICF_CLIB_TRACE(("Packing list of icf_req_stream_st\n"));
    
	/*Pack the count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_req_stream_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {
	
		icf_pack_icf_req_stream_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_create_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_create_media_session_req_st    *p_icf_create_media_session_req_st = ICF_NULL;
    
    p_icf_create_media_session_req_st    = (icf_create_media_session_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_create_media_session_req_st */
    ICF_CLIB_TRACE(("Packing icf_create_media_session_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_media_session_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_create_media_session_req_st->bit_mask & ICF_LINE_ID_PRESENT)
	{
		icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_media_session_req_st->line_id), &offset);
    
		*p_length    += offset;
	}

	icf_pack_icf_req_stream_list_st(p_dest + (*p_length), (p_icf_create_media_session_req_st->p_stream_list_data), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_resp_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_resp_stream_st    *p_icf_resp_stream_st = ICF_NULL;
    
    p_icf_resp_stream_st    = (icf_resp_stream_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_resp_stream_st */
    ICF_CLIB_TRACE(("Packing icf_resp_stream_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_resp_stream_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_resp_stream_st->stream_type), &offset);
    
    *p_length    += offset;

	icf_pack_icf_sdp_stream_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_resp_stream_st->local_sdp), &offset, p_icf_resp_stream_st->stream_type);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_resp_stream_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_resp_stream_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_list_st       *p_list_ptr = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_resp_stream_st*/
    ICF_CLIB_TRACE(("Packing list of icf_resp_stream_st\n"));
    
	/*Pack the count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_resp_stream_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_resp_stream_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_create_media_session_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_create_media_session_resp_st    *p_icf_create_media_session_resp_st = ICF_NULL;
    
    p_icf_create_media_session_resp_st    = (icf_create_media_session_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_create_media_session_resp_st */
    ICF_CLIB_TRACE(("Packing icf_create_media_session_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_media_session_resp_st->result), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_media_session_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(!p_icf_create_media_session_resp_st->result)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_media_session_resp_st->error_cause), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_create_media_session_resp_st->result)
	{
		icf_pack_icf_resp_stream_list_st(p_dest + (*p_length), (p_icf_create_media_session_resp_st->p_accepted_stream_list), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_create_media_session_resp_st->bit_mask & ICF_CMS_REJECT_STREAM_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_create_media_session_resp_st->rejected_stream_list), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_modify_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_modify_stream_st    *p_icf_modify_stream_st = ICF_NULL;
    
    p_icf_modify_stream_st    = (icf_modify_stream_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_modify_stream_st */
    ICF_CLIB_TRACE(("Packing icf_modify_stream_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_stream_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_stream_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_stream_st->stream_type), &offset);
    
    *p_length    += offset;

	if((p_icf_modify_stream_st->bit_mask & ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT) || (p_icf_modify_stream_st->bit_mask & ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT))
	{
		icf_pack_icf_sdp_stream_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_stream_st->sdp_stream), &offset, p_icf_modify_stream_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_modify_stream_st->bit_mask & ICF_MODIFY_MEDIA_MODE_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_stream_st->mode), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_modify_stream_st->bit_mask & ICF_MODIFY_MEDIA_ECHO_CAN_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_stream_st->echo_cancellation), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_modify_stream_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_modify_stream_list_st,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
    icf_list_st       *p_list_ptr = ICF_NULL;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs list of icf_modify_stream_st*/
    ICF_CLIB_TRACE(("Packing list of icf_modify_stream_st\n"));
    
	/*Pack the count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

    for(p_list_ptr = p_icf_modify_stream_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {

		icf_pack_icf_modify_stream_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;

	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_modify_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_modify_media_session_req_st    *p_icf_modify_media_session_req_st = ICF_NULL;
    
    p_icf_modify_media_session_req_st    = (icf_modify_media_session_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_modify_media_session_req_st */
    ICF_CLIB_TRACE(("Packing icf_modify_media_session_req_st\n"));
    
    icf_pack_icf_modify_stream_list_st(p_dest + (*p_length), (p_icf_modify_media_session_req_st->p_stream_data_list), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_modify_media_session_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_modify_media_session_resp_st    *p_icf_modify_media_session_resp_st = ICF_NULL;
    
    p_icf_modify_media_session_resp_st    = (icf_modify_media_session_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_modify_media_session_resp_st */
    ICF_CLIB_TRACE(("Packing icf_modify_media_session_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_media_session_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_media_session_resp_st->result), &offset);
    
    *p_length    += offset;

	if(!p_icf_modify_media_session_resp_st->result)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_media_session_resp_st->error_cause), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_modify_media_session_resp_st->result)
	{
		icf_pack_icf_resp_stream_list_st(p_dest + (*p_length), (p_icf_modify_media_session_resp_st->p_accepted_stream_list), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_modify_media_session_resp_st->bit_mask & ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT)
	{
		icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_modify_media_session_resp_st->rejected_stream_list), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_delete_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_delete_media_session_req_st    *p_icf_delete_media_session_req_st = ICF_NULL;
    
    p_icf_delete_media_session_req_st    = (icf_delete_media_session_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_delete_media_session_req_st */
    ICF_CLIB_TRACE(("Packing icf_delete_media_session_req_st\n"));
    
    icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_media_session_req_st->stream_list), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_msg_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_msg_resp_st    *p_icf_msg_resp_st = ICF_NULL;
    
    p_icf_msg_resp_st    = (icf_msg_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_msg_resp_st */
    ICF_CLIB_TRACE(("Packing icf_msg_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_resp_st->bit_mask), &offset);
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_resp_st->result), &offset);
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_resp_st->error_cause), &offset);
    *p_length    += offset;

    if (p_icf_msg_resp_st->bit_mask & ICF_MSG_RESP_LINE_ID_PRESENT)
    {
       icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_msg_resp_st->line_id), &offset); 
       *p_length    += offset;
    }  

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_close_media_channel_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_close_media_channel_req_st    *p_icf_close_media_channel_req_st = ICF_NULL;
    
    p_icf_close_media_channel_req_st    = (icf_close_media_channel_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_close_media_channel_req_st */
    ICF_CLIB_TRACE(("Packing icf_close_media_channel_req_st\n"));
    
    icf_pack_icf_channel_list_st(p_dest + (*p_length), (p_icf_close_media_channel_req_st->p_channel_list), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_delete_media_session_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_delete_media_session_ind_st    *p_icf_delete_media_session_ind_st = ICF_NULL;
    
    p_icf_delete_media_session_ind_st    = (icf_delete_media_session_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_delete_media_session_ind_st */
    ICF_CLIB_TRACE(("Packing icf_delete_media_session_ind_st\n"));
    
    icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_media_session_ind_st->stream_list), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_media_session_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_delete_media_session_ind_st->bit_mask & ICF_DEL_MEDIA_SESSION_ERR_CAUSE_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_delete_media_session_ind_st->error_cause), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_merge_stream_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_merge_stream_id_st    *p_icf_merge_stream_id_st = ICF_NULL;
    
    p_icf_merge_stream_id_st    = (icf_merge_stream_id_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_merge_stream_id_st */
    ICF_CLIB_TRACE(("Packing icf_merge_stream_id_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_merge_stream_id_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_merge_stream_id_st->merge_stream_id), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_pack_icf_merge_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_merge_media_session_req_st    *p_icf_merge_media_session_req_st = ICF_NULL;
    
    p_icf_merge_media_session_req_st    = (icf_merge_media_session_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_merge_media_session_req_st */
    ICF_CLIB_TRACE(("Packing icf_merge_media_session_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_merge_media_session_req_st->merge_call_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_merge_media_session_req_st->count), &offset);
    
    *p_length    += offset;

	while((count<p_icf_merge_media_session_req_st->count) && (count < ICF_MAX_STREAM_PER_CALL))
	{
		icf_pack_icf_merge_stream_id_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_merge_media_session_req_st->merge_stream_id[count]), &offset);
    
		*p_length    += offset;

		count++;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_media_capabilities_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_media_capabilities_req_st    *p_icf_media_capabilities_req_st = ICF_NULL;
    
    p_icf_media_capabilities_req_st    = (icf_media_capabilities_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_media_capabilities_req_st */
    ICF_CLIB_TRACE(("Packing icf_media_capabilities_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_capabilities_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(ICF_MED_CAP_STREAM_PRSNT & p_icf_media_capabilities_req_st->bit_mask)
	{
		icf_pack_icf_stream_capab_list_st(p_dest + (*p_length), (p_icf_media_capabilities_req_st->p_req_stream_list), &offset);
    
		*p_length    += offset;
	}
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_capabilities_req_st->line_id), &offset);
        *p_length    += offset;
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_media_capabilities_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_media_capabilities_resp_st    *p_icf_media_capabilities_resp_st = ICF_NULL;
    
    p_icf_media_capabilities_resp_st    = (icf_media_capabilities_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_media_capabilities_resp_st */
    ICF_CLIB_TRACE(("Packing icf_media_capabilities_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_capabilities_resp_st->result), &offset);
    
    *p_length    += offset;

	if(!p_icf_media_capabilities_resp_st->result)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_capabilities_resp_st->error_cause), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_media_capabilities_resp_st->result)
	{
		icf_pack_icf_resp_stream_list_st(p_dest + (*p_length), (p_icf_media_capabilities_resp_st->p_resp_stream_list), &offset);
    
		*p_length    += offset;
	}
    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_capabilities_resp_st->line_id), &offset);

    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_send_udp_data_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;
   
    icf_send_udp_data_req_st    *p_icf_send_udp_data_req_st = ICF_NULL;
    
    p_icf_send_udp_data_req_st    = (icf_send_udp_data_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_send_udp_data_req_st */
    ICF_CLIB_TRACE(("Packing icf_send_udp_data_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_udp_data_req_st->source_port), &offset);
    
    *p_length    += offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_udp_data_req_st->dest_addr), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_udp_data_req_st->app_data_len), &offset);
    
    *p_length    += offset;

    count = 0;

    while(count < p_icf_send_udp_data_req_st->app_data_len)
    {
		icf_pack_U8(p_dest + (*p_length), 
			(icf_uint8_t*)&(p_icf_send_udp_data_req_st->p_app_data[count]), 
			&offset);
        
		*p_length    += offset;
        count++;
    }
	
	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_recv_udp_data_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;
   
    icf_recv_udp_data_ind_st    *p_icf_recv_udp_data_ind_st = ICF_NULL;
    
    p_icf_recv_udp_data_ind_st    = (icf_recv_udp_data_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_recv_udp_data_ind_st */
    ICF_CLIB_TRACE(("Packing icf_recv_udp_data_ind_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->dest_port), &offset);
    
    *p_length    += offset;

	icf_pack_icf_transport_address_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->source_addr), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->app_data_len), &offset);
    
    *p_length    += offset;

	count = 0;

    while(count < p_icf_recv_udp_data_ind_st->app_data_len)
    {
		icf_pack_U8(p_dest + (*p_length), 
			(icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->p_app_data[count]), 
			&offset);
		*p_length    += offset;
        count++;
    }
	
	return ICF_SUCCESS;
}

#ifdef ICF_MM_CFG_FUTURE_RELEASE

/***************************************************

			   COMMOM APPLICATION APIs

****************************************************/

icf_uint32_t
icf_pack_icf_start_ringing_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_start_ringing_req_st    *p_icf_start_ringing_req_st = ICF_NULL;
    
    p_icf_start_ringing_req_st    = (icf_start_ringing_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_start_ringing_req_st */
    ICF_CLIB_TRACE(("Packing icf_start_ringing_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_ringing_req_st->ringer_id), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_ringing_req_st->duration), &offset);
    
    *p_length    += offset;

    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_ringing_req_st->bit_mask), &offset);
    
    *p_length    += offset;	

     if(p_icf_start_ringing_req_st->bit_mask & ICF_RINGING_TIMER_VAL)
     {
	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_start_ringing_req_st->timer_value), &offset);
    
	*p_length    += offset;	
     }

     icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_start_ringing_req_st->line_id), &offset); 
     *p_length    += offset;  

     

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_vibration_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_vibration_req_st    *p_icf_vibration_req_st = ICF_NULL;
    
    p_icf_vibration_req_st    = (icf_vibration_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_vibration_req_st */
    ICF_CLIB_TRACE(("Packing icf_vibration_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vibration_req_st->vibration_id), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vibration_req_st->duration), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vibration_req_st->bit_mask), &offset);
    
    *p_length    += offset;	

	if(p_icf_vibration_req_st->bit_mask & ICF_VIBRATION_TIMER_VAL)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vibration_req_st->timer_value), &offset);
    
		*p_length    += offset;	
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_connect_media_device_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_connect_media_device_req_st    *p_icf_connect_media_device_req_st = ICF_NULL;
    
    p_icf_connect_media_device_req_st    = (icf_connect_media_device_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_connect_media_device_req_st */
    ICF_CLIB_TRACE(("Packing icf_connect_media_device_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_media_device_req_st->media_device), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_connect_media_device_req_st->connect_status), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_connect_media_device_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_echo_canc_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_echo_canc_req_st    *p_icf_echo_canc_req_st = ICF_NULL;
    
    p_icf_echo_canc_req_st    = (icf_echo_canc_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_echo_canc_req_st */
    ICF_CLIB_TRACE(("Packing icf_echo_canc_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_echo_canc_req_st->echo_cancel), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_echo_canc_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_echo_canc_req_st->bit_mask & ICF_ECHO_CANC_STREAM_ID)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_echo_canc_req_st->stream_id), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_vad_ss_cng_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_vad_ss_cng_req_st    *p_icf_vad_ss_cng_req_st = ICF_NULL;
    
    p_icf_vad_ss_cng_req_st    = (icf_vad_ss_cng_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_vad_ss_cng_req_st */
    ICF_CLIB_TRACE(("Packing icf_vad_ss_cng_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_STREAM_ID)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->stream_id), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_SUPP)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->vad_supp), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_SILENCE_SUPP)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->silence_supp), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_COMFORT_NOISE)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->comfort_noise), &offset);
    
		*p_length    += offset;
	}


	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_get_stats_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_get_stats_req_st    *p_icf_get_stats_req_st = ICF_NULL;
    
    p_icf_get_stats_req_st    = (icf_get_stats_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_stats_req_st */
    ICF_CLIB_TRACE(("Packing icf_get_stats_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_stats_req_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_stats_req_st->stats_type), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_stats_req_st->interval_type), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_rtcp_report_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_rtcp_report_st    *p_icf_rtcp_report_st = ICF_NULL;
    
    p_icf_rtcp_report_st    = (icf_rtcp_report_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_rtcp_report_st */
    ICF_CLIB_TRACE(("Packing icf_rtcp_report_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rtcp_report_st->time_stamp), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rtcp_report_st->num_packet_send), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rtcp_report_st->num_octet_send), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rtcp_report_st->num_packet_rcvd), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rtcp_report_st->num_octet_rcvd), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rtcp_report_st->cum_packet_loss), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_get_stats_resp_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_get_stats_resp_st    *p_icf_get_stats_resp_st = ICF_NULL;
    
    p_icf_get_stats_resp_st    = (icf_get_stats_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_get_stats_resp_st */
    ICF_CLIB_TRACE(("Packing icf_get_stats_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_stats_resp_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_rtcp_report_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_get_stats_resp_st->rtcp_report), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_file_name_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
)
{

    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;
  
    icf_file_name_st    *p_icf_file_name_st = ICF_NULL;
    
    p_icf_file_name_st    = (icf_file_name_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_file_name_st */
    ICF_CLIB_TRACE(("Packing icf_file_name_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_file_name_st->str_len), &offset);
    
    *p_length    += offset;

    if (0 >= p_icf_file_name_st->str_len || p_icf_file_name_st->str_len > ICF_MAX_LONG_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_file_name_st - Invalid String Length (%d) Bytes\n", p_icf_file_name_st->str_len));
        return ICF_FAILURE;
    } else 
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_pack_icf_file_name_st - Packing String of Length (%d) Bytes\n", p_icf_file_name_st->str_len));


    /* This piece of code packs a variable length octet string */
    
    count = 0;
    /* Pack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_file_name_st->str_len + 1)
    {
        icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_file_name_st->str[count]), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_play_media_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_play_media_req_st    *p_icf_play_media_req_st = ICF_NULL;
    
    p_icf_play_media_req_st    = (icf_play_media_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_play_media_req_st */
    ICF_CLIB_TRACE(("Packing icf_play_media_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_play_media_req_st->is_present), &offset);
    
    *p_length    += offset;

	if(p_icf_play_media_req_st->is_present & ICF_MEDIA_ID_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_play_media_req_st->media_id), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_play_media_req_st->is_present & ICF_FILE_NAME_PRESENT)
	{
		icf_pack_icf_file_name_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_play_media_req_st->file_name), &offset);
    
		*p_length    += offset;
	}

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_play_media_req_st->play_mode), &offset);
    
	*p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_play_media_req_st->duration), &offset);
    
	*p_length    += offset;

	if(p_icf_play_media_req_st->is_present & ICF_PLAY_TONE_TIMER_VAL)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_play_media_req_st->timer_value), &offset);
    
		*p_length    += offset;
	}

        icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_play_media_req_st->line_id), &offset); 
        *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_stop_media_play_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_stop_media_play_req_st    *p_icf_stop_media_play_req_st = ICF_NULL;
    
    p_icf_stop_media_play_req_st    = (icf_stop_media_play_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stop_media_play_req_st */
    ICF_CLIB_TRACE(("Packing icf_stop_media_play_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_play_req_st->is_present), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_media_play_req_st->is_present & ICF_MEDIA_ID_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_play_req_st->media_id), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_stop_media_play_req_st->is_present & ICF_FILE_NAME_PRESENT)
	{
		icf_pack_icf_file_name_st (p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_play_req_st->file_name), &offset);
    
		*p_length    += offset;
	}

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_stop_media_play_req_st->line_id), &offset); 
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_stop_media_play_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_stop_media_play_ind_st    *p_icf_stop_media_play_ind_st = ICF_NULL;
    
    p_icf_stop_media_play_ind_st    = (icf_stop_media_play_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stop_play_file_ind_st */
    ICF_CLIB_TRACE(("Packing icf_stop_play_file_ind_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_play_ind_st->is_present), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_media_play_ind_st->is_present & ICF_MEDIA_ID_PRESENT)
	{
		icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_play_ind_st->media_id), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_stop_media_play_ind_st->is_present & ICF_FILE_NAME_PRESENT)
	{
		icf_pack_icf_file_name_st (p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_play_ind_st->file_name), &offset);
    
		*p_length    += offset;
	}

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_stop_media_play_ind_st->line_id), &offset); 
    *p_length    += offset;


	return ICF_SUCCESS;
}

/***************************************************

			VIDEO/VOICE APPLICATION APIs

****************************************************/

icf_uint32_t
icf_pack_icf_dgts_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_dgts_st    *p_icf_dgts_st = ICF_NULL;
    
    p_icf_dgts_st    = (icf_dgts_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs p_icf_dgts_st */
    ICF_CLIB_TRACE(("Packing p_icf_dgts_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_dgts_st->number_of_dgts), &offset);
    
    *p_length    += offset;

	while(count<p_icf_dgts_st->number_of_dgts)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_dgts_st->dgts[count]), &offset);
    
		*p_length    += offset;

		count++;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_dgts_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_dgts_ind_st    *p_icf_dgts_ind_st = ICF_NULL;
    
    p_icf_dgts_ind_st    = (icf_dgts_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_dgts_ind_st */
    ICF_CLIB_TRACE(("Packing icf_dgts_ind_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_dgts_ind_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_dgts_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_dgts_ind_st->digits), &offset);
    
	*p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_dgts_ind_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_mute_media_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_mute_media_req_st    *p_icf_mute_media_req_st = ICF_NULL;
    
    p_icf_mute_media_req_st    = (icf_mute_media_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_mute_media_req_st */
    ICF_CLIB_TRACE(("Packing icf_mute_media_req_st\n"));
    
    icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mute_media_req_st->stream_list), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mute_media_req_st->activate_mute), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_mute_media_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_mute_media_resp_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
    icf_uint8_t         count = 0;
   
    icf_mute_media_resp_st    *p_icf_mute_media_resp_st = ICF_NULL;
    
    p_icf_mute_media_resp_st    = (icf_mute_media_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_mute_media_resp_st */
    ICF_CLIB_TRACE(("Packing icf_mute_media_resp_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mute_media_resp_st->bit_mask), &offset);
    *p_length    += offset;      

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mute_media_resp_st->result), &offset);
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mute_media_resp_st->error_cause), &offset);
    *p_length    += offset;

    if (p_icf_mute_media_resp_st->bit_mask & ICF_MUTE_MEDIA_RESP_LINE_ID_PRESENT)
    {
       icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_mute_media_resp_st->line_id), &offset); 
       *p_length    += offset;        
    }
	
    return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_set_media_playback_device_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_set_media_playback_device_req_st    *p_icf_set_media_playback_device_req_st = ICF_NULL;
    
    p_icf_set_media_playback_device_req_st    = (icf_set_media_playback_device_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_media_playback_device_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_media_playback_device_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_media_playback_device_req_st->media_play_out_device), &offset);
    
	*p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_set_media_playback_device_req_st->line_id), &offset); 
        *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_media_record_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_media_record_req_st    *p_icf_media_record_req_st = ICF_NULL;
    
    p_icf_media_record_req_st    = (icf_media_record_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_media_record_req_st */
    ICF_CLIB_TRACE(("Packing icf_media_record_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_record_req_st->media_source), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_record_req_st->file_type), &offset);
    
	*p_length    += offset;

	icf_pack_icf_file_name_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_record_req_st->file_name), &offset);
    
	*p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_record_req_st->bit_mask), &offset);
    
	*p_length    += offset;

	if(p_icf_media_record_req_st->bit_mask & ICF_STREAM_ID_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_record_req_st->stream_id), &offset);
    
		*p_length    += offset;
	}

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_media_record_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_send_dgts_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_send_dgts_st    *p_icf_send_dgts_st = ICF_NULL;
    
    p_icf_send_dgts_st    = (icf_send_dgts_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_send_dgts_st */
    ICF_CLIB_TRACE(("Packing icf_send_dgts_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_dgts_st->stream_id ), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_dgts_st->transport_method), &offset);
    
	*p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_dgts_st->digits), &offset);
    
	*p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_set_earp_volume_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_set_earp_volume_req_st    *p_icf_set_earp_volume_req_st = ICF_NULL;
    
    p_icf_set_earp_volume_req_st    = (icf_set_earp_volume_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_earp_volume_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_earp_volume_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_earp_volume_req_st->level ), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_set_earp_volume_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_set_speaker_volume_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_set_speaker_volume_req_st    *p_icf_set_speaker_volume_req_st = ICF_NULL;
    
    p_icf_set_speaker_volume_req_st    = (icf_set_speaker_volume_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_speaker_volume_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_speaker_volume_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_speaker_volume_req_st->level ), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_set_speaker_volume_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_stop_media_record_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_stop_media_record_req_st    *p_icf_stop_media_record_req_st = ICF_NULL;
    
    p_icf_stop_media_record_req_st    = (icf_stop_media_record_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stop_media_record_req_st */
    ICF_CLIB_TRACE(("Packing icf_stop_media_record_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_record_req_st->media_source), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_record_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_media_record_req_st->bit_mask & ICF_STREAM_ID_PRESENT)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_media_record_req_st->stream_id), &offset);
    
		*p_length    += offset;
	}

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_stop_media_record_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_set_mic_gain_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_set_mic_gain_req_st    *p_icf_set_mic_gain_req_st = ICF_NULL;
    
    p_icf_set_mic_gain_req_st    = (icf_set_mic_gain_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_set_mic_gain_req_st */
    ICF_CLIB_TRACE(("Packing icf_set_mic_gain_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_set_mic_gain_req_st->gain_level), &offset);
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_pause_media_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_pause_media_req_st    *p_icf_pause_media_req_st = ICF_NULL;
    
    p_icf_pause_media_req_st    = (icf_pause_media_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_pause_media_req_st */
    ICF_CLIB_TRACE(("Packing icf_pause_media_req_st\n"));
    
    icf_pack_icf_stream_id_list_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_pause_media_req_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_pause_media_req_st->activate_pause), &offset);
    
    *p_length    += offset;

    icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_pause_media_req_st->line_id), &offset); 
    *p_length    += offset; 

	return ICF_SUCCESS;
}

/***************************************************

				PUSH TO TALK APIs

****************************************************/
icf_uint32_t
icf_pack_icf_stop_stats_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
   
    icf_stop_stats_req_st    *p_icf_stop_stats_req_st = ICF_NULL;
    
    p_icf_stop_stats_req_st    = (icf_stop_stats_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_stop_stats_req_st */
    ICF_CLIB_TRACE(("Packing icf_stop_stats_req_st\n"));
    
    icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_stats_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_stats_req_st->bit_mask & ICF_STOP_STATS_REQ_STREAM_ID)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_stop_stats_req_st->stream_id), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_app_data_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
	icf_uint8_t         app_count = 0;
   
    icf_app_data_st    *p_icf_app_data_st = ICF_NULL;
    
    p_icf_app_data_st    = (icf_app_data_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_app_data_st */
    ICF_CLIB_TRACE(("Packing icf_app_data_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_data_st->str_len), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_data_st->sub_type), &offset);
    
    *p_length    += offset;

	while(app_count<4)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_data_st->app_name[app_count]), &offset);
    
		*p_length    += offset;

		app_count++;
	}

	while(count<p_icf_app_data_st->str_len)
	{
		icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_app_data_st->str[count]), &offset);
    
		*p_length    += offset;

		count++;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_rcv_app_data_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_rcv_app_data_ind_st    *p_icf_rcv_app_data_ind_st = ICF_NULL;
    
    p_icf_rcv_app_data_ind_st    = (icf_rcv_app_data_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_rcv_app_data_ind_st */
    ICF_CLIB_TRACE(("Packing icf_rcv_app_data_ind_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_app_data_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->application_data), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->ssrc_num), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_rcv_app_data_ind_st->bit_mask & ICF_MM_PTT_RCV_APP_DATA_IND_SEQUENCE_NUMBER)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->sequence_num), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_send_app_data_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_send_app_data_req_st    *p_icf_send_app_data_req_st = ICF_NULL;
    
    p_icf_send_app_data_req_st    = (icf_send_app_data_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_send_app_data_req_st */
    ICF_CLIB_TRACE(("Packing icf_send_app_data_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_app_data_req_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_app_data_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_app_data_req_st->application_data), &offset);
    
    *p_length    += offset;

	icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_app_data_req_st->bit_mask), &offset);
    
    *p_length    += offset;

	if(p_icf_send_app_data_req_st->bit_mask & ICF_SEND_APP_DATA_REQ_SEQUENCE_NUMBER)
	{
		icf_pack_U32(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_app_data_req_st->sequence_num), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_send_stats_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_send_stats_req_st    *p_icf_send_stats_req_st = ICF_NULL;
    
    p_icf_send_stats_req_st    = (icf_send_stats_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_send_stats_req_st */
    ICF_CLIB_TRACE(("Packing icf_send_stats_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_stats_req_st->stream_id), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_send_stats_req_st->report_type), &offset);
    
    *p_length    += offset;
	
	return ICF_SUCCESS;
}

#endif

/***************************************************

			     CONFIGURATION APIs

****************************************************/

icf_uint32_t
icf_pack_icf_encoded_params_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_encoded_params_list_st,
    icf_uint32_t    *p_length
)
{

	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
	   
	icf_list_st       *p_list_ptr = ICF_NULL;
   
	/* This function packs icf_encoded_params_list_st */
    ICF_CLIB_TRACE(("Packing icf_encoded_params_list_st\n"));


	/* Initialise length to 0 */
    *p_length    = 0;

	/*Pack count = 0 initially*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&count, &offset);
    
    *p_length    += offset;

	for(p_list_ptr = p_icf_encoded_params_list_st;ICF_NULL != p_list_ptr;p_list_ptr = p_list_ptr->p_next)
    {
		
		icf_pack_icf_string_st(p_dest + (*p_length), (p_list_ptr->p_data), &offset);

		*p_length += offset;

		count++;
	}

	if(count)
	{
		/* Updating the count */
		icf_pack_U8(p_dest, (icf_uint8_t*)&count, &offset);
	}

	return ICF_SUCCESS;
}



icf_uint32_t
icf_pack_icf_config_codec_attrib_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_config_codec_attrib_st    *p_icf_config_codec_attrib_st = ICF_NULL;
    
    p_icf_config_codec_attrib_st    = (icf_config_codec_attrib_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_config_codec_attrib_st */
    ICF_CLIB_TRACE(("Packing icf_config_codec_attrib_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->bitmask), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_type), &offset);
    
    *p_length    += offset;

	icf_pack_icf_short_string_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_name), &offset);
    
	*p_length    += offset;

#if 1 /*ZyXEL porting*/
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_num), &offset);
    
    *p_length    += offset;
	
	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->enable), &offset);
    
    *p_length    += offset;
#endif	

	icf_pack_icf_encoded_params_list_st(p_dest + (*p_length), (p_icf_config_codec_attrib_st->p_encoded_params), &offset);
    
	*p_length    += offset;

	if(p_icf_config_codec_attrib_st->bitmask & ICF_CODEC_PARAM_PRESENT)
	{
		switch(p_icf_config_codec_attrib_st->codec_type)
		{
			case  ICF_CODEC_TYPE_AUDIO:

				  icf_pack_icf_config_audio_codec_param_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_param), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_CODEC_TYPE_VIDEO:

				  icf_pack_icf_config_video_codec_param_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_param), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_CODEC_TYPE_IMAGE:

				  icf_pack_icf_config_t38_attrib_param_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_param), &offset);
    
				  *p_length    += offset;

				  break;
		}

	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_cfg_codec_info_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_codec_info_req_st    *p_icf_cfg_codec_info_req_st = ICF_NULL;
    
    p_icf_cfg_codec_info_req_st    = (icf_cfg_codec_info_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_codec_info_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_codec_info_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_codec_info_req_st->count), &offset);
    
    *p_length    += offset;

	while((count<p_icf_cfg_codec_info_req_st->count) && (count < ICF_MAX_CODEC_SUPPORTD))
	{
		icf_pack_icf_config_codec_attrib_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_codec_info_req_st->codec_supp[count]), &offset);
    
		*p_length    += offset;

		count++;
	}
#if 1 /*ZyXEL porting*/
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_codec_info_req_st->line_id), &offset);
    
    *p_length    += offset;
#endif	
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_mm_unexpected_msg_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_mm_unexpected_msg_ind_st    *p_icf_mm_unexpected_msg_ind_st = ICF_NULL;

    p_icf_mm_unexpected_msg_ind_st    = (icf_mm_unexpected_msg_ind_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_mm_unexpected_msg_ind_st */
    ICF_CLIB_TRACE(("Packing icf_mm_unexpected_msg_ind_st\n"));

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mm_unexpected_msg_ind_st->error_cause), &offset);
    *p_length    += offset;

    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_mm_unexpected_msg_ind_st->api_id), &offset);
    *p_length    += offset;

    return ICF_SUCCESS;
}


#ifdef ICF_MM_CFG_FUTURE_RELEASE

icf_uint32_t
icf_pack_icf_ringer_id_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_ringer_id_st    *p_icf_ringer_id_st = ICF_NULL;
    
    p_icf_ringer_id_st    = (icf_ringer_id_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_ringer_id_st */
    ICF_CLIB_TRACE(("Packing icf_ringer_id_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ringer_id_st->ringer_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_file_name_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_ringer_id_st->play_file), &offset);
    
	*p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_cfg_ringer_tones_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_ringer_tones_req_st    *p_icf_cfg_ringer_tones_req_st = ICF_NULL;
    
    p_icf_cfg_ringer_tones_req_st    = (icf_cfg_ringer_tones_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_ringer_tones_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_ringer_tones_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_ringer_tones_req_st->num_of_ringer), &offset);
    
    *p_length    += offset;

	while(count<p_icf_cfg_ringer_tones_req_st->num_of_ringer)
	{
		icf_pack_icf_ringer_id_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_ringer_tones_req_st->ringer[count]), &offset);
    
		*p_length    += offset;

		count++;
	}
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_media_id_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_media_id_st    *p_icf_media_id_st = ICF_NULL;
    
    p_icf_media_id_st    = (icf_media_id_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_media_id_st */
    ICF_CLIB_TRACE(("Packing icf_media_id_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_id_st->media_id), &offset);
    
    *p_length    += offset;

	icf_pack_icf_file_name_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_media_id_st->play_file), &offset);
    
	*p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_cfg_media_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_media_req_st    *p_icf_cfg_media_req_st = ICF_NULL;
    
    p_icf_cfg_media_req_st    = (icf_cfg_media_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_media_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_media_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_media_req_st->count), &offset);
    
    *p_length    += offset;

	while(count<p_icf_cfg_media_req_st->count)
	{
		icf_pack_icf_media_id_st(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_media_req_st->media[count]), &offset);
    
		*p_length    += offset;

		count++;
	}
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_cfg_set_digit_trans_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_set_digit_trans_req_st    *p_icf_cfg_set_digit_trans_req_st = ICF_NULL;
    
    p_icf_cfg_set_digit_trans_req_st    = (icf_cfg_set_digit_trans_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_set_digit_trans_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_set_digit_trans_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_set_digit_trans_req_st->dgt_trns_method), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_cfg_jbm_mgmt_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_jbm_mgmt_req_st    *p_icf_cfg_jbm_mgmt_req_st = ICF_NULL;
    
    p_icf_cfg_jbm_mgmt_req_st    = (icf_cfg_jbm_mgmt_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_jbm_mgmt_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_jbm_mgmt_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_type), &offset);
    
    *p_length    += offset;

	icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_scaling_factor), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_initial_size), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_max_size), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_min_size), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_cfg_max_earp_vol_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_max_earp_vol_req_st    *p_icf_cfg_max_earp_vol_req_st = ICF_NULL;
    
    p_icf_cfg_max_earp_vol_req_st    = (icf_cfg_max_earp_vol_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_max_earp_vol_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_max_earp_vol_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_max_earp_vol_req_st->max_earp_volume), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_cfg_max_speaker_vol_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_max_speaker_vol_req_st    *p_icf_cfg_max_speaker_vol_req_st = ICF_NULL;
    
    p_icf_cfg_max_speaker_vol_req_st    = (icf_cfg_max_speaker_vol_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_max_speaker_vol_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_max_speaker_vol_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_max_speaker_vol_req_st->max_speaker_volume), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_pack_icf_cfg_max_mic_gain_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_cfg_max_mic_gain_req_st    *p_icf_cfg_max_mic_gain_req_st = ICF_NULL;
    
    p_icf_cfg_max_mic_gain_req_st    = (icf_cfg_max_mic_gain_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_cfg_max_mic_gain_req_st */
    ICF_CLIB_TRACE(("Packing icf_cfg_max_mic_gain_req_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_cfg_max_mic_gain_req_st->max_mic_gain), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_pack_icf_video_display_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
   
    icf_video_display_st    *p_icf_video_display_st = ICF_NULL;
    
    p_icf_video_display_st    = (icf_video_display_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_video_display_st */
    ICF_CLIB_TRACE(("Packing icf_video_display_st\n"));
    
    icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_display_st->top), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_display_st->left), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_display_st->width), &offset);
    
    *p_length    += offset;

	icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_video_display_st->height), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}


#endif

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_tunnelled_create_media_session_req_st
*
* DESCRIPTION: This function is used to pack icf_tunnelled_create_media_session_req_st
*              
*
*****************************************************************************/

icf_uint32_t
icf_pack_icf_tunnelled_create_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_tunnelled_create_media_session_req_st  *p_icf_tunnelled_create_media_session_req_st = ICF_NULL;
    
    p_icf_tunnelled_create_media_session_req_st  = (icf_tunnelled_create_media_session_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tunnelled_create_media_session_req_st */
    ICF_CLIB_TRACE(("Packing icf_tunnelled_create_media_session_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tunnelled_create_media_session_req_st->\
        bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_tunnelled_create_media_session_req_st->bit_mask & 
        ICF_TUNNELLED_CMS_REQ_LINE_ID_PRESENT)
    {
        icf_pack_icf_line_id_t(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_create_media_session_req_st->line_id), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_tunnelled_create_media_session_req_st->bit_mask & 
        ICF_TUNNELLED_CMS_RESP_SDP_INFO_PRESENT)
    {
 
        icf_pack_icf_tunnelled_sdp_info_st(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_create_media_session_req_st->tunnelled_sdp_info), &offset);

        *p_length    += offset;
    }
return ICF_SUCCESS;
}

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_tunnelled_create_media_session_resp_st
*
* DESCRIPTION: This function is used to pack icf_tunnelled_create_media_session_resp_st
*              
*
*****************************************************************************/

icf_uint32_t
icf_pack_icf_tunnelled_create_media_session_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_tunnelled_create_media_session_resp_st *p_icf_tunnelled_create_media_session_resp_st = ICF_NULL;
    
    p_icf_tunnelled_create_media_session_resp_st = (icf_tunnelled_create_media_session_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tunnelled_create_media_session_resp_st */
    ICF_CLIB_TRACE(("Packing icf_tunnelled_create_media_session_resp_st\n"));

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
        (p_icf_tunnelled_create_media_session_resp_st->result), &offset);
    
    *p_length    += offset;
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
        (p_icf_tunnelled_create_media_session_resp_st->bit_mask), &offset);
    
    *p_length    += offset;
 
    if(p_icf_tunnelled_create_media_session_resp_st->bit_mask & 
         ICF_TUNNELLED_CMS_RESP_ERROR_CAUSE_PRESENT)
    {
        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_create_media_session_resp_st->error_cause), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_tunnelled_create_media_session_resp_st->bit_mask & 
        ICF_TUNNELLED_CMS_RESP_SDP_INFO_PRESENT)
    {
        
        icf_pack_icf_tunnelled_sdp_info_st(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_create_media_session_resp_st->tunnelled_sdp_info), &offset);

        *p_length    += offset;
    }
return ICF_SUCCESS;
}





/*****************************************************************************
*
* FUNCTION: icf_pack_icf_tunnelled_modify_media_session_req_st
*
* DESCRIPTION: This function is used to pack icf_tunnelled_modify_media_session_req_st
*              
*
*****************************************************************************/



icf_uint32_t
icf_pack_icf_tunnelled_modify_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_tunnelled_modify_media_session_req_st *p_icf_tunnelled_modify_media_session_req_st = ICF_NULL;
    
    p_icf_tunnelled_modify_media_session_req_st = (icf_tunnelled_modify_media_session_req_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tunnelled_modify_media_session_req_st */
    ICF_CLIB_TRACE(("Packing icf_tunnelled_modify_media_session_req_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
        (p_icf_tunnelled_modify_media_session_req_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_modify_media_session_req_st->local_sdp_reqd), &offset);
    
    *p_length    += offset;

    if(p_icf_tunnelled_modify_media_session_req_st->bit_mask & 
        ICF_TUNNELLED_MMS_REQ_SDP_INFO_PRESENT)
    {
        icf_pack_icf_tunnelled_sdp_info_st(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_modify_media_session_req_st->tunnelled_sdp_info), &offset);

        *p_length    += offset;
    }
return ICF_SUCCESS;
}        




/*****************************************************************************
*
* FUNCTION: icf_pack_icf_tunnelled_media_capabilities_resp_st
*
* DESCRIPTION: This function is used to pack icf_tunnelled_media_capabilities_resp_st
*              
*
*****************************************************************************/



icf_uint32_t
icf_pack_icf_tunnelled_media_capabilities_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_tunnelled_media_capabilities_resp_st *p_icf_tunnelled_med_cap_resp_st = ICF_NULL;
    
    p_icf_tunnelled_med_cap_resp_st = (icf_tunnelled_media_capabilities_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tunnelled_med_cap_resp_st */
    ICF_CLIB_TRACE(("Packing icf_tunnelled_med_cap_resp_st\n"));
    
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
        (p_icf_tunnelled_med_cap_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&(p_icf_tunnelled_med_cap_resp_st->result), &offset);
    
    *p_length    += offset;

    if(p_icf_tunnelled_med_cap_resp_st->bit_mask & ICF_TUNNELLED_MED_CAP_RESP_ERROR_CAUSE_PRESENT)
    {

        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_med_cap_resp_st->error_cause), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_tunnelled_med_cap_resp_st->bit_mask & ICF_TUNNELLED_MED_CAP_RESP_SDP_INFO_PRESENT)
    {
     
        icf_pack_icf_tunnelled_sdp_info_st(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_med_cap_resp_st->tunnelled_sdp_info), &offset);

        *p_length    += offset;
    }
return ICF_SUCCESS;
}

/*****************************************************************************
*
* FUNCTION: icf_pack_icf_tunnelled_modify_media_session_resp_st
*
* DESCRIPTION: This function is used to pack icf_tunnelled_modify_media_session_resp_st
*              
*
*****************************************************************************/



icf_uint32_t
icf_pack_icf_tunnelled_modify_media_session_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
   
    icf_tunnelled_modify_media_session_resp_st *p_icf_tunnelled_modify_media_session_resp_st = ICF_NULL;
    
    p_icf_tunnelled_modify_media_session_resp_st = (icf_tunnelled_modify_media_session_resp_st *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_tunnelled_modify_media_session_resp_st */
    ICF_CLIB_TRACE(("Packing icf_tunnelled_modify_media_session_resp_st\n"));
 
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
        (p_icf_tunnelled_modify_media_session_resp_st->result), &offset);
    
    *p_length    += offset;
   
    icf_pack_U8(p_dest + (*p_length), (icf_uint8_t*)&\
        (p_icf_tunnelled_modify_media_session_resp_st->bit_mask), &offset);
    
    *p_length    += offset;

    if(p_icf_tunnelled_modify_media_session_resp_st->bit_mask & 
        ICF_TUNNELLED_MMS_RESP_ERROR_CAUSE_PRESENT)
    {

        icf_pack_U16(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_modify_media_session_resp_st->error_cause), &offset);
    
        *p_length    += offset;
    }

    if(p_icf_tunnelled_modify_media_session_resp_st->bit_mask & 
        ICF_TUNNELLED_MMS_RESP_SDP_INFO_PRESENT)
    {

        icf_pack_icf_tunnelled_sdp_info_st(p_dest + (*p_length), (icf_uint8_t*)&\
            (p_icf_tunnelled_modify_media_session_resp_st->tunnelled_sdp_info), &offset);

        *p_length    += offset;
    }
return ICF_SUCCESS;
}





