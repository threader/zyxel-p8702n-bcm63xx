/*******************************************************************************
*                                        
* FILE NAME    : icf_unpack_mm_api.c
*                                        
* DESCRIPTION  :The file contains the routines for the buffer Unpacking code.
*               Fraction of this code is automatically generated.
*                                        
* Revision History :                
*                
*   DATE             NAME               REFERENCE              REASON
* -----------    ---------------      -------------      ------------------
* 05-JUL-2005     Raveen Sharma	       ICF REL 6.0		       Initial
* 04-Jan-2007     Amit Mahajan                               Multiline changes.
*
* Copyright 2006, Aricent.                       
*******************************************************************************/




#include "icf_clib.h" 
#include "icf_utils.h"
#include "icf_api.h"


#include "icf_unpack_mm_api.h"


/***************************************************

			RESOURCE MANAGEMENT APIs

****************************************************/
icf_uint32_t
icf_unpack_icf_encoded_attributes_list_st
(
	icf_list_st    **p_p_icf_encoded_attributes_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_string_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_string_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_encoded_attributes_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_string_st*/
		(*p_p_icf_encoded_attributes_list_st)->p_data = (icf_string_st*)ICF_CLIB_MEMGET(sizeof(icf_string_st));
		
		icf_unpack_icf_string_st((icf_uint8_t*)((*p_p_icf_encoded_attributes_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_encoded_attributes_list_st = &((*p_p_icf_encoded_attributes_list_st)->p_next);

	}

	(*p_p_icf_encoded_attributes_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_codec_attrib_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length,
	icf_uint8_t	stream_type
)
{
    icf_uint32_t        offset = 0;
 
    icf_codec_attrib_st    *p_icf_codec_attrib_st = ICF_NULL;
    
    p_icf_codec_attrib_st    = (icf_codec_attrib_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_codec_attrib_st */
    ICF_CLIB_TRACE(("Unpacking icf_codec_attrib_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_codec_attrib_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_codec_attrib_st->bit_mask & ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_codec_attrib_st->codec_num), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	icf_unpack_icf_short_string_st((icf_uint8_t*)&(p_icf_codec_attrib_st->codec_name), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_codec_attrib_st->bit_mask & ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT)
	{
		switch(stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:

				  icf_unpack_icf_audio_codec_param_st((icf_uint8_t*)&(p_icf_codec_attrib_st->codec_params), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;
		
			case  ICF_STREAM_TYPE_VIDEO:

				  icf_unpack_icf_video_codec_param_st((icf_uint8_t*)&(p_icf_codec_attrib_st->codec_params), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_STREAM_TYPE_FAX:

				  icf_unpack_icf_config_t38_attrib_param_st((icf_uint8_t*)&(p_icf_codec_attrib_st->codec_params), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;

		}
	}

	if(p_icf_codec_attrib_st->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)
	{
		icf_unpack_icf_encoded_attributes_list_st(&(p_icf_codec_attrib_st->p_encoded_attributes), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_codec_attrib_list_st
(
	icf_list_st    **p_p_icf_codec_attrib_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length,
	icf_uint8_t   stream_type
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_codec_attrib_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_codec_attrib_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_codec_attrib_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_codec_attrib_st*/
		(*p_p_icf_codec_attrib_list_st)->p_data = (icf_codec_attrib_st*)ICF_CLIB_MEMGET(sizeof(icf_codec_attrib_st));
		
		icf_unpack_icf_codec_attrib_st((icf_uint8_t*)((*p_p_icf_codec_attrib_list_st)->p_data), p_src + (*p_length), &offset, stream_type);

		*p_length    += offset;

		p_p_icf_codec_attrib_list_st = &((*p_p_icf_codec_attrib_list_st)->p_next);

	}

	 (*p_p_icf_codec_attrib_list_st) = ICF_NULL; 

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_codec_info_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint32_t        count = 0;

	icf_codec_info_st *p_icf_codec_info_st = ICF_NULL;

	p_icf_codec_info_st = (icf_codec_info_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_codec_info_st */
    ICF_CLIB_TRACE(("Unpacking icf_codec_info_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_codec_info_st->codec_count), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	while(count<p_icf_codec_info_st->codec_count)
	{
		icf_unpack_icf_short_string_st((icf_uint8_t*)&(p_icf_codec_info_st->codec_name[count]), p_src + (*p_length), &offset);
		
		*p_length += offset;

		count++;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_audio_profile_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

	icf_audio_profile_st *p_icf_audio_profile_st = ICF_NULL;

	p_icf_audio_profile_st = (icf_audio_profile_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_audio_profile_st */
    ICF_CLIB_TRACE(("Unpacking icf_audio_profile_st\n"));

	icf_unpack_U8((icf_uint8_t*)&(p_icf_audio_profile_st->bit_mask), p_src + (*p_length), &offset);
    
	*p_length    += offset;

    if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_audio_profile_st->bandwidth), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_audio_profile_st->pref_ptime), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_audio_profile_st->silence_suppression), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
        if(p_icf_audio_profile_st->bit_mask & ICF_AUDIO_PROF_ECHO_CANC_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_audio_profile_st->echo_canc), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
        
	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_video_profile_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_video_profile_st *p_icf_video_profile_st = ICF_NULL;

	p_icf_video_profile_st = (icf_video_profile_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_video_profile_st */
    ICF_CLIB_TRACE(("Unpacking icf_video_profile_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_video_profile_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_video_profile_st->encoding_quality), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_video_profile_st->frame_rate), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_video_profile_st->bandwidth), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_t_38_media_profile_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

	icf_t_38_media_profile_st *p_icf_t_38_media_profile_st = ICF_NULL;

	p_icf_t_38_media_profile_st = (icf_t_38_media_profile_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_t_38_media_profile_st */
    ICF_CLIB_TRACE(("Unpacking icf_t_38_media_profile_st\n"));

	icf_unpack_U32((icf_uint8_t*)&(p_icf_t_38_media_profile_st->max_bit_rate), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stream_capab_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
 
    icf_stream_capab_st    *p_icf_stream_capab_st = ICF_NULL;
    
    p_icf_stream_capab_st    = (icf_stream_capab_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stream_capab_st */
    ICF_CLIB_TRACE(("Unpacking icf_stream_capab_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_capab_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;


	icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_capab_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;


	icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_capab_st->stream_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;


	if(p_icf_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
	{
		icf_unpack_icf_codec_attrib_list_st(&(p_icf_stream_capab_st->p_codec_info_list), p_src + (*p_length), &offset, p_icf_stream_capab_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT)
	{
		switch(p_icf_stream_capab_st->stream_type)
		{
	
			case  ICF_STREAM_TYPE_AUDIO:

				  icf_unpack_icf_audio_profile_st((icf_uint8_t*)&(p_icf_stream_capab_st->media_stream_param), p_src + (*p_length), &offset);
    
			      *p_length    += offset;

				  break;

			case  ICF_STREAM_TYPE_VIDEO:

				  icf_unpack_icf_video_profile_st((icf_uint8_t*)&(p_icf_stream_capab_st->media_stream_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;

		}
	}

	if(p_icf_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
	{
		icf_unpack_icf_encoded_attributes_list_st(&(p_icf_stream_capab_st->p_encoded_attributes), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_stream_capab_list_st
(
	icf_list_st    **p_p_icf_stream_capab_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_stream_capab_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_stream_capab_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_stream_capab_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_stream_capab_st*/
		(*p_p_icf_stream_capab_list_st)->p_data = (icf_stream_capab_st*)ICF_CLIB_MEMGET(sizeof(icf_stream_capab_st));
		
		icf_unpack_icf_stream_capab_st((icf_uint8_t*)((*p_p_icf_stream_capab_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_stream_capab_list_st = &((*p_p_icf_stream_capab_list_st)->p_next);

	}

	(*p_p_icf_stream_capab_list_st) = ICF_NULL;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_get_codec_list_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
  
    icf_get_codec_list_req_st    *p_icf_get_codec_list_req_st = ICF_NULL;
    
    p_icf_get_codec_list_req_st    = (icf_get_codec_list_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_get_codec_list_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_get_codec_list_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_codec_list_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_get_codec_list_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_stream_capab_list_st(&(p_icf_get_codec_list_req_st->p_stream_capab_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT & p_icf_get_codec_list_req_st->bit_mask)
	{
		icf_unpack_icf_stream_capab_list_st(&(p_icf_get_codec_list_req_st->p_local_stream_capab_list), p_src + (*p_length), &offset);

		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_amr_params_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

	icf_amr_params_st *p_icf_amr_params_st = ICF_NULL;

	p_icf_amr_params_st = (icf_amr_params_st*)p_dest;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_amr_params_st */
    ICF_CLIB_TRACE(("Unpacking icf_amr_params_st\n"));

	icf_unpack_U8((icf_uint8_t*)&(p_icf_amr_params_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_amr_params_st->amr_initial_mode), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_amr_params_st->bit_mask & ICF_AMR_MODE_LIST_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_amr_params_st->amr_mode_count), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		while(count<p_icf_amr_params_st->amr_mode_count)
		{
			icf_unpack_U8((icf_uint8_t*)&(p_icf_amr_params_st->amr_mode_list[count]), p_src + (*p_length), &offset);
    
			*p_length    += offset;

			count++;
		}
	}
	
	icf_unpack_U8((icf_uint8_t*)&(p_icf_amr_params_st->codec_mode), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_amr_params_st->crc_enabled), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_audio_codec_param_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
  
    icf_audio_codec_param_st    *p_icf_audio_codec_param_st = ICF_NULL;
    
    p_icf_audio_codec_param_st    = (icf_audio_codec_param_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_audio_codec_param_st */
    ICF_CLIB_TRACE(("Unpacking icf_audio_codec_param_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_audio_codec_param_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_CHANNEL)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_audio_codec_param_st->channels), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_audio_codec_param_st->sampling_rate), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_AMR_PARAM)
	{
		icf_unpack_icf_amr_params_st((icf_uint8_t*)&(p_icf_audio_codec_param_st->amr_params), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
        if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_VBD_VAL)
        {
                icf_unpack_U8((icf_uint8_t*)&(p_icf_audio_codec_param_st->vbd_val), p_src + (*p_length), &offset);
    
                *p_length    += offset;
        }
 
	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_video_codec_param_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
  
    icf_video_codec_param_st    *p_icf_video_codec_param_st = ICF_NULL;
    
    p_icf_video_codec_param_st    = (icf_video_codec_param_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_video_codec_param_st */
    ICF_CLIB_TRACE(("Unpacking icf_video_codec_param_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_video_codec_param_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_video_codec_param_st->clock_rate), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_FRAMESIZE_PRESENT)
	{
		icf_unpack_icf_size_st((icf_uint8_t*)&(p_icf_video_codec_param_st->frame_size), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_config_audio_codec_param_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
  
    icf_config_audio_codec_param_st    *p_icf_config_audio_codec_param_st = ICF_NULL;
    
    p_icf_config_audio_codec_param_st    = (icf_config_audio_codec_param_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_audio_codec_param_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_audio_codec_param_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_CHANNEL)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->channels), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PTIME)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->num_of_supported_ptime), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		while(count<p_icf_config_audio_codec_param_st->num_of_supported_ptime)
		{
			icf_unpack_U8((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->ptime[count]), p_src + (*p_length), &offset);
    
			*p_length    += offset;

			count++;
		}
#if 1 /* ZyXEL_PORTING*/
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->pref_ptime), p_src + (*p_length), &offset);
    
		*p_length    += offset;
#endif
	}
	
	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_BANDWIDTH)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->bandwidth), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	count = 0;

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_SAMPLING_RATE)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->num_of_supp_sampling_rate), p_src + (*p_length), &offset);
    
		*p_length    += offset;
		
		while(count<p_icf_config_audio_codec_param_st->num_of_supp_sampling_rate)
		{
			icf_unpack_U16((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->sampling_rate[count]), p_src + (*p_length), &offset);
    
			*p_length    += offset;

			count++;
		}
	}

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_SILENCE_SUPPRESSION)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->silence_suppression), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_AMR_PARAM)
	{
		icf_unpack_icf_amr_params_st((icf_uint8_t*)&(p_icf_config_audio_codec_param_st->amr_params), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}


icf_uint32_t
icf_unpack_icf_size_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
  
    icf_size_st    *p_icf_size_st = ICF_NULL;
    
    p_icf_size_st    = (icf_size_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_size_st */
    ICF_CLIB_TRACE(("Unpacking icf_size_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_size_st->height), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_size_st->width), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_config_video_codec_param_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;
  
    icf_config_video_codec_param_st    *p_icf_config_video_codec_param_st = ICF_NULL;
    
    p_icf_config_video_codec_param_st    = (icf_config_video_codec_param_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_video_codec_param_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_video_codec_param_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_video_codec_param_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_FRAME_RATE_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_video_codec_param_st->frame_rate), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_ENC_QUALITY_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_video_codec_param_st->encoding_quality), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_BANDWIDTH_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_config_video_codec_param_st->bandwidth), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_CLOCK_RATE_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_video_codec_param_st->num_of_supp_clock_rate), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		while(count<p_icf_config_video_codec_param_st->num_of_supp_clock_rate)
		{
			icf_unpack_U32((icf_uint8_t*)&(p_icf_config_video_codec_param_st->clock_rate[count]), p_src + (*p_length), &offset);
    
			*p_length    += offset;

			count++;
		}
	}

	if(p_icf_config_video_codec_param_st->bit_mask & ICF_VIDEO_CFG_CODEC_FRAMESIZE_PRESENT)
	{
		icf_unpack_icf_size_st((icf_uint8_t*)&(p_icf_config_video_codec_param_st->frame_size), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_config_t38_attrib_param_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
  
    icf_config_t38_attrib_param_st    *p_icf_config_t38_attrib_param_st = ICF_NULL;
    
    p_icf_config_t38_attrib_param_st    = (icf_config_t38_attrib_param_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_t38_attrib_param_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_t38_attrib_param_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_VERSION)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->fax_version), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_BITRATE)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->bitrate), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_RATE_MANAGEMENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->rate_management), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_UDP_BUFFSIZE)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->udp_buffersize), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_UDP_PACKETSIZE)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->udp_packetsize), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_config_t38_attrib_param_st->bit_mask & ICF_FAX_T38_ERR_CORRECTION)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_config_t38_attrib_param_st->err_correction_method), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}


icf_uint32_t
icf_unpack_icf_get_codec_list_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_get_codec_list_resp_st    *p_icf_get_codec_list_resp_st = ICF_NULL;
    
    p_icf_get_codec_list_resp_st    = (icf_get_codec_list_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_get_codec_list_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_get_codec_list_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_codec_list_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_get_codec_list_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(!p_icf_get_codec_list_resp_st->result)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_get_codec_list_resp_st->error_cause), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_get_codec_list_resp_st->result)
	{
		icf_unpack_icf_stream_capab_list_st(&(p_icf_get_codec_list_resp_st->p_reserved_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_get_codec_list_resp_st->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_get_codec_list_resp_st->rejected_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_codec_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_codec_id_st    *p_icf_codec_id_st = ICF_NULL;
    
    p_icf_codec_id_st    = (icf_codec_id_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_codec_id_st */
    ICF_CLIB_TRACE(("Unpacking icf_codec_id_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_codec_id_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_codec_id_st->bit_mask & ICF_CODEC_ID_CODEC_NAME_PRESENT)
	{
		icf_unpack_icf_short_string_st((icf_uint8_t*)&(p_icf_codec_id_st->codec_name), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_codec_id_st->bit_mask & ICF_CODEC_ID_CODEC_ID_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_codec_id_st->codec_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stream_commit_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
	  
    icf_stream_commit_st    *p_icf_stream_commit_st = ICF_NULL;
    
    p_icf_stream_commit_st    = (icf_stream_commit_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stream_commit_st */
    ICF_CLIB_TRACE(("Unpacking icf_stream_commit_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_commit_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_stream_commit_st->codec_count), p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count<p_icf_stream_commit_st->codec_count)
	{
		icf_unpack_icf_codec_id_st((icf_uint8_t*)&(p_icf_stream_commit_st->codec[count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		count++;

	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stream_commit_list_st
(
	icf_list_st    **p_p_icf_stream_commit_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_stream_commit_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_stream_commit_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_stream_commit_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		
		/* get memory for icf_stream_commit_st*/
		(*p_p_icf_stream_commit_list_st)->p_data = (icf_stream_commit_st*)ICF_CLIB_MEMGET(sizeof(icf_stream_commit_st));
		
		icf_unpack_icf_stream_commit_st((icf_uint8_t*)((*p_p_icf_stream_commit_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_stream_commit_list_st = &((*p_p_icf_stream_commit_list_st)->p_next);

	}

	(*p_p_icf_stream_commit_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_codec_commit_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_codec_commit_req_st    *p_icf_codec_commit_req_st = ICF_NULL;
    
    p_icf_codec_commit_req_st    = (icf_codec_commit_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_codec_commit_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_codec_commit_req_st\n"));

    icf_unpack_icf_stream_capab_list_st(&(p_icf_codec_commit_req_st->p_stream_commit_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_codec_commit_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_codec_commit_resp_st    *p_icf_codec_commit_resp_st = ICF_NULL;
    
    p_icf_codec_commit_resp_st    = (icf_codec_commit_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_codec_commit_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_codec_commit_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_codec_commit_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_codec_commit_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;
	
	if(!p_icf_codec_commit_resp_st->result)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_codec_commit_resp_st->error_cause), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_codec_commit_resp_st->result)
	{
		icf_unpack_icf_stream_commit_list_st(&(p_icf_codec_commit_resp_st->p_commited_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_codec_commit_resp_st->bit_mask & ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_codec_commit_resp_st->rejected_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_rel_res_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_rel_res_req_st    *p_icf_rel_res_req_st = ICF_NULL;
    
    p_icf_rel_res_req_st    = (icf_rel_res_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_rel_res_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_rel_res_req_st\n"));

    icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_rel_res_req_st->stream_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_channel_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	  
    icf_channel_st    *p_icf_channel_st = ICF_NULL;
    
    p_icf_channel_st    = (icf_channel_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_channel_st */
    ICF_CLIB_TRACE(("Unpacking icf_channel_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_channel_st->stream_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_channel_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_channel_list_st
(
	icf_list_st    **p_p_icf_channel_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_channel_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_channel_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_channel_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		
		/* get memory for icf_channel_st*/
		(*p_p_icf_channel_list_st)->p_data = (icf_channel_st*)ICF_CLIB_MEMGET(sizeof(icf_channel_st));
		
		icf_unpack_icf_channel_st((icf_uint8_t*)((*p_p_icf_channel_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_channel_list_st = &((*p_p_icf_channel_list_st)->p_next);

	}

	(*p_p_icf_channel_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_open_media_channel_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	  
    icf_open_media_channel_req_st    *p_icf_open_media_channel_req_st = ICF_NULL;
    
    p_icf_open_media_channel_req_st    = (icf_open_media_channel_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_open_media_channel_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_open_media_channel_req_st\n"));

    icf_unpack_icf_channel_list_st(&(p_icf_open_media_channel_req_st->p_channel_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}


icf_uint32_t
icf_unpack_icf_channel_addr_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	  
    icf_channel_addr_st    *p_icf_channel_addr_st = ICF_NULL;
    
    p_icf_channel_addr_st    = (icf_channel_addr_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_channel_addr_st */
    ICF_CLIB_TRACE(("Unpacking icf_channel_addr_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_channel_addr_st->stream_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_channel_addr_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_channel_addr_st->stream_address), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;

}

icf_uint32_t
icf_unpack_icf_channel_addr_list_st
(
	icf_list_st    **p_p_icf_channel_addr_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_channel_addr_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_channel_addr_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_channel_addr_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		
		/* get memory for icf_channel_addr_st*/
		(*p_p_icf_channel_addr_list_st)->p_data = (icf_channel_addr_st*)ICF_CLIB_MEMGET(sizeof(icf_channel_addr_st));
		
		icf_unpack_icf_channel_addr_st((icf_uint8_t*)((*p_p_icf_channel_addr_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_channel_addr_list_st = &((*p_p_icf_channel_addr_list_st)->p_next);

	}

	(*p_p_icf_channel_addr_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_open_media_channel_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	  
    icf_open_media_channel_resp_st    *p_icf_open_media_channel_resp_st = ICF_NULL;
    
    p_icf_open_media_channel_resp_st    = (icf_open_media_channel_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_open_media_channel_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_open_media_channel_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_open_media_channel_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(!p_icf_open_media_channel_resp_st->result)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_open_media_channel_resp_st->error_cause), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_open_media_channel_resp_st->result)
	{
		icf_unpack_icf_channel_addr_list_st(&(p_icf_open_media_channel_resp_st->p_channel_addr_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;

}

/***************************************************

			     MEDIA CONTROL APIs

****************************************************/
icf_uint32_t
icf_unpack_icf_sdp_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length,
	icf_uint8_t   stream_type
)
{
	icf_uint32_t        offset = 0;
	  
    icf_sdp_stream_st    *p_icf_sdp_stream_st = ICF_NULL;
    
    p_icf_sdp_stream_st    = (icf_sdp_stream_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_sdp_stream_st */
    ICF_CLIB_TRACE(("Unpacking icf_sdp_stream_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_sdp_stream_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_codec_attrib_list_st(&(p_icf_sdp_stream_st->p_codec_info_list), p_src + (*p_length), &offset, stream_type);
    
    *p_length    += offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_sdp_stream_st->stream_address), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_sdp_stream_st->trans_mode), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_sdp_stream_st->bit_mask & ICF_SDP_STREAM_MEDIA_STREAM_PRESENT)
	{
		switch(stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
			
		     	  icf_unpack_icf_audio_profile_st((icf_uint8_t*)&(p_icf_sdp_stream_st->media_stream_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;
			  

			case  ICF_STREAM_TYPE_VIDEO:

				  icf_unpack_icf_video_profile_st((icf_uint8_t*)&(p_icf_sdp_stream_st->media_stream_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;
	
				  break;

		}
	}

	if(p_icf_sdp_stream_st->bit_mask & ICF_SDP_STREAM_RTCP_INFO_PRESENT)
	{
		icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_sdp_stream_st->rtcp_address), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_sdp_stream_st->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
	{
		icf_unpack_icf_encoded_attributes_list_st(&(p_icf_sdp_stream_st->p_encoded_attributes), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_req_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_req_stream_st    *p_icf_req_stream_st = ICF_NULL;
    
    p_icf_req_stream_st    = (icf_req_stream_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_req_stream_st */
    ICF_CLIB_TRACE(("Unpacking icf_req_stream_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_req_stream_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_req_stream_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_req_stream_st->stream_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT)
	{
		icf_unpack_icf_sdp_stream_st((icf_uint8_t*)&(p_icf_req_stream_st->remote_sdp), p_src + (*p_length), &offset, p_icf_req_stream_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT)
	{
		icf_unpack_icf_sdp_stream_st((icf_uint8_t*)&(p_icf_req_stream_st->local_sdp), p_src + (*p_length), &offset, p_icf_req_stream_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_STREAM_MODE_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_req_stream_st->mode), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_req_stream_st->qos_mark), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_RTP_DETECTION_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_req_stream_st->rtp_detection_reqd), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_req_stream_st->bit_mask & ICF_CREATE_MEDIA_ECHO_CAN_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_req_stream_st->echo_cancellation), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_req_stream_list_st
(
	icf_list_st    **p_p_icf_req_stream_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_req_stream_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_req_stream_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_req_stream_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_req_stream_st*/
		(*p_p_icf_req_stream_list_st)->p_data = (icf_req_stream_st*)ICF_CLIB_MEMGET(sizeof(icf_req_stream_st));
		
		icf_unpack_icf_req_stream_st((icf_uint8_t*)((*p_p_icf_req_stream_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_req_stream_list_st = &((*p_p_icf_req_stream_list_st)->p_next);

	}

	(*p_p_icf_req_stream_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_create_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_create_media_session_req_st    *p_icf_create_media_session_req_st = ICF_NULL;
    
    p_icf_create_media_session_req_st    = (icf_create_media_session_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_create_media_session_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_create_media_session_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_create_media_session_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_create_media_session_req_st->bit_mask & ICF_LINE_ID_PRESENT)
	{
		icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_create_media_session_req_st->line_id), p_src + (*p_length), &offset);
    
	    *p_length    += offset;
	}

	icf_unpack_icf_req_stream_list_st(&(p_icf_create_media_session_req_st->p_stream_list_data), p_src + (*p_length), &offset);
    
	*p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_resp_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_resp_stream_st    *p_icf_resp_stream_st = ICF_NULL;
    
    p_icf_resp_stream_st    = (icf_resp_stream_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_resp_stream_st */
    ICF_CLIB_TRACE(("Unpacking icf_resp_stream_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_resp_stream_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_resp_stream_st->stream_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_sdp_stream_st((icf_uint8_t*)&(p_icf_resp_stream_st->local_sdp), p_src + (*p_length), &offset, p_icf_resp_stream_st->stream_type);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_resp_stream_list_st
(
	icf_list_st    **p_p_icf_resp_stream_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_resp_stream_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_resp_stream_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_resp_stream_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_resp_stream_st*/
		(*p_p_icf_resp_stream_list_st)->p_data = (icf_resp_stream_st*)ICF_CLIB_MEMGET(sizeof(icf_resp_stream_st));
		
		icf_unpack_icf_resp_stream_st((icf_uint8_t*)((*p_p_icf_resp_stream_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_resp_stream_list_st = &((*p_p_icf_resp_stream_list_st)->p_next);

	}

   	(*p_p_icf_resp_stream_list_st) = ICF_NULL; 

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_create_media_session_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_create_media_session_resp_st    *p_icf_create_media_session_resp_st = ICF_NULL;
    
    p_icf_create_media_session_resp_st    = (icf_create_media_session_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_create_media_session_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_create_media_session_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_create_media_session_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_create_media_session_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(!p_icf_create_media_session_resp_st->result)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_create_media_session_resp_st->error_cause), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_create_media_session_resp_st->result)
	{
		icf_unpack_icf_resp_stream_list_st(&(p_icf_create_media_session_resp_st->p_accepted_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_create_media_session_resp_st->bit_mask & ICF_CMS_REJECT_STREAM_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_create_media_session_resp_st->rejected_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_modify_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_modify_stream_st    *p_icf_modify_stream_st = ICF_NULL;
    
    p_icf_modify_stream_st    = (icf_modify_stream_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_modify_stream_st */
    ICF_CLIB_TRACE(("Unpacking icf_modify_stream_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_modify_stream_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_modify_stream_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_modify_stream_st->stream_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if((p_icf_modify_stream_st->bit_mask & ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT) || (p_icf_modify_stream_st->bit_mask & ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT))
	{
		icf_unpack_icf_sdp_stream_st((icf_uint8_t*)&(p_icf_modify_stream_st->sdp_stream), p_src + (*p_length), &offset, p_icf_modify_stream_st->stream_type);
    
		*p_length    += offset;
	}

	if(p_icf_modify_stream_st->bit_mask & ICF_MODIFY_MEDIA_MODE_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_modify_stream_st->mode), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_modify_stream_st->bit_mask & ICF_MODIFY_MEDIA_ECHO_CAN_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_modify_stream_st->echo_cancellation), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_modify_stream_list_st
(
	icf_list_st    **p_p_icf_modify_stream_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking list of icf_modify_stream_st */
    ICF_CLIB_TRACE(("Unpacking list of icf_modify_stream_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_modify_stream_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_modify_stream_st*/
		(*p_p_icf_modify_stream_list_st)->p_data = (icf_modify_stream_st*)ICF_CLIB_MEMGET(sizeof(icf_modify_stream_st));
		
		icf_unpack_icf_modify_stream_st((icf_uint8_t*)((*p_p_icf_modify_stream_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_modify_stream_list_st = &((*p_p_icf_modify_stream_list_st)->p_next);

	}

	(*p_p_icf_modify_stream_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_modify_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_modify_media_session_req_st    *p_icf_modify_media_session_req_st = ICF_NULL;
    
    p_icf_modify_media_session_req_st    = (icf_modify_media_session_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_modify_media_session_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_modify_media_session_req_st\n"));

    icf_unpack_icf_modify_stream_list_st(&(p_icf_modify_media_session_req_st->p_stream_data_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_modify_media_session_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_modify_media_session_resp_st    *p_icf_modify_media_session_resp_st = ICF_NULL;
    
    p_icf_modify_media_session_resp_st    = (icf_modify_media_session_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_modify_media_session_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_modify_media_session_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_modify_media_session_resp_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_modify_media_session_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(!p_icf_modify_media_session_resp_st->result)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_modify_media_session_resp_st->error_cause), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_modify_media_session_resp_st->result)
	{
		icf_unpack_icf_resp_stream_list_st(&(p_icf_modify_media_session_resp_st->p_accepted_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_modify_media_session_resp_st->bit_mask & ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT)
	{
		icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_modify_media_session_resp_st->rejected_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_delete_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_delete_media_session_req_st    *p_icf_delete_media_session_req_st = ICF_NULL;
    
    p_icf_delete_media_session_req_st    = (icf_delete_media_session_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_delete_media_session_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_delete_media_session_req_st\n"));

    icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_delete_media_session_req_st->stream_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_msg_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_msg_resp_st    *p_icf_msg_resp_st = ICF_NULL;
    
    p_icf_msg_resp_st    = (icf_msg_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_msg_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_msg_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_msg_resp_st->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_msg_resp_st->result), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_msg_resp_st->error_cause), p_src + (*p_length), &offset);
    *p_length    += offset;

    if (p_icf_msg_resp_st->bit_mask & ICF_MSG_RESP_LINE_ID_PRESENT)
    { 
       icf_unpack_icf_line_id_t((icf_uint8_t*)&(p_icf_msg_resp_st->line_id), p_src + (*p_length) ,&offset); 
       *p_length    += offset;
    } 

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_close_media_channel_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_close_media_channel_req_st    *p_icf_close_media_channel_req_st = ICF_NULL;
    
    p_icf_close_media_channel_req_st    = (icf_close_media_channel_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_close_media_channel_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_close_media_channel_req_st\n"));

    icf_unpack_icf_channel_list_st(&(p_icf_close_media_channel_req_st->p_channel_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_delete_media_session_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_delete_media_session_ind_st    *p_icf_delete_media_session_ind_st = ICF_NULL;
    
    p_icf_delete_media_session_ind_st    = (icf_delete_media_session_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_delete_media_session_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_delete_media_session_ind_st\n"));

    icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_delete_media_session_ind_st->stream_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_delete_media_session_ind_st->bit_mask), p_src + (*p_length), &offset);
    
	*p_length    += offset;

	if(p_icf_delete_media_session_ind_st->bit_mask & ICF_DEL_MEDIA_SESSION_ERR_CAUSE_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_delete_media_session_ind_st->error_cause), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_merge_stream_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_merge_stream_id_st    *p_icf_merge_stream_id_st = ICF_NULL;
    
    p_icf_merge_stream_id_st    = (icf_merge_stream_id_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_merge_stream_id_st */
    ICF_CLIB_TRACE(("Unpacking icf_merge_stream_id_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_merge_stream_id_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_merge_stream_id_st->merge_stream_id), p_src + (*p_length), &offset);
    
	*p_length    += offset;
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_merge_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t			count = 0;
	  
    icf_merge_media_session_req_st    *p_icf_merge_media_session_req_st = ICF_NULL;
    
    p_icf_merge_media_session_req_st    = (icf_merge_media_session_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_merge_media_session_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_merge_media_session_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_merge_media_session_req_st->merge_call_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_merge_media_session_req_st->count), p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count<p_icf_merge_media_session_req_st->count)
	{
		icf_unpack_icf_merge_stream_id_st((icf_uint8_t*)&(p_icf_merge_media_session_req_st->merge_stream_id[count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		count++;
	}
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_media_capabilities_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_media_capabilities_resp_st    *p_icf_media_capabilities_resp_st = ICF_NULL;
    
    p_icf_media_capabilities_resp_st    = (icf_media_capabilities_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_media_capabilities_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_media_capabilities_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_media_capabilities_resp_st->result), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(!p_icf_media_capabilities_resp_st->result)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_media_capabilities_resp_st->error_cause), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_media_capabilities_resp_st->result)
	{
		icf_unpack_icf_resp_stream_list_st(&(p_icf_media_capabilities_resp_st->p_resp_stream_list), p_src + (*p_length), &offset);
	    
		*p_length    += offset;
	}
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_send_udp_data_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint32_t        count = 0;
	  
    icf_send_udp_data_req_st    *p_icf_send_udp_data_req_st = ICF_NULL;
    
    p_icf_send_udp_data_req_st    = (icf_send_udp_data_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_send_udp_data_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_send_udp_data_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_send_udp_data_req_st->source_port), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_send_udp_data_req_st->dest_addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_send_udp_data_req_st->app_data_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	p_icf_send_udp_data_req_st->p_app_data = ICF_NULL;
    /* This piece of code unpacks variable length octet string */

    if (0 < p_icf_send_udp_data_req_st->app_data_len  && 
		p_icf_send_udp_data_req_st->app_data_len < 
					ICF_CLIB_MAX_API_LENGTH) 
	{
        ICF_CLIB_TRACE(
		  ("\n[ICF_CLIB]: Getting memory for p_icf_send_udp_data_req_st->p_app_data\n"));
        p_icf_send_udp_data_req_st->p_app_data = 
				ICF_CLIB_MEMGET(p_icf_send_udp_data_req_st->app_data_len);

        if (!p_icf_send_udp_data_req_st->p_app_data) 
		{
            ICF_CLIB_TRACE(
			 ("\n[ICF_CLIB]: Failed In Allocating Memory "
			  "for p_icf_send_udp_data_req_st->app_data_len\n"));
            return ICF_FAILURE;
        }
    } 
	else 
	{
        ICF_CLIB_TRACE(
			("\n[ICF_CLIB]: icf_unpack_icf_send_udp_data_req_st - "
			 "Invalid Structure Length!! Quitting! (%d) Bytes\n"));
        return ICF_FAILURE;
    }
	count = 0;
    while(count < p_icf_send_udp_data_req_st->app_data_len && 
					p_icf_send_udp_data_req_st->p_app_data)
    {
        icf_unpack_U8(
			(icf_uint8_t*)&(p_icf_send_udp_data_req_st->p_app_data[count]), 
			p_src + (*p_length), 
			&offset);
        
        *p_length    += offset;
        
        count++;
    }
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_recv_udp_data_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint32_t        count = 0;
	  
    icf_recv_udp_data_ind_st    *p_icf_recv_udp_data_ind_st = ICF_NULL;
    
    p_icf_recv_udp_data_ind_st    = (icf_recv_udp_data_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_recv_udp_data_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_recv_udp_data_ind_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->dest_port), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_transport_address_st((icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->source_addr), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->app_data_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    p_icf_recv_udp_data_ind_st->p_app_data = ICF_NULL;
    /* This piece of code unpacks variable length octet string */

    if (0 < p_icf_recv_udp_data_ind_st->app_data_len  && 
		p_icf_recv_udp_data_ind_st->app_data_len < ICF_CLIB_MAX_API_LENGTH) 
	{
        ICF_CLIB_TRACE(
		("\n[ICF_CLIB]: Getting memory for p_icf_recv_udp_data_ind_st->p_app_data\n"));
        p_icf_recv_udp_data_ind_st->p_app_data = 
				ICF_CLIB_MEMGET(p_icf_recv_udp_data_ind_st->app_data_len);

        if (!p_icf_recv_udp_data_ind_st->p_app_data) 
		{
            ICF_CLIB_TRACE(
			("\n[ICF_CLIB]: Failed In Allocating Memory for "
			 	"p_icf_recv_udp_data_ind_st->app_data_len\n"));
            return ICF_FAILURE;
        }
    } 
	else 
	{
        ICF_CLIB_TRACE(
		("\n[ICF_CLIB]: icf_unpack_icf_recv_udp_data_ind_st - "
		 	"Invalid Structure Length!! Quitting! (%d) Bytes\n"));
        return ICF_FAILURE;
    }
	count = 0;
    while(count < p_icf_recv_udp_data_ind_st->app_data_len && 
					p_icf_recv_udp_data_ind_st->p_app_data)
    {
        icf_unpack_U8(
			(icf_uint8_t*)&(p_icf_recv_udp_data_ind_st->p_app_data[count]), 
			p_src + (*p_length), 
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
icf_unpack_icf_start_ringing_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_start_ringing_req_st    *p_icf_start_ringing_req_st = ICF_NULL;
    
    p_icf_start_ringing_req_st    = (icf_start_ringing_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_start_ringing_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_start_ringing_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_start_ringing_req_st->ringer_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_start_ringing_req_st->duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_start_ringing_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_start_ringing_req_st->bit_mask & ICF_RINGING_TIMER_VAL)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_start_ringing_req_st->timer_value), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

    icf_unpack_icf_line_id_t((icf_line_id_t*)&(p_icf_start_ringing_req_st->line_id), p_src + (*p_length), &offset);
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_vibration_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_vibration_req_st    *p_icf_vibration_req_st = ICF_NULL;
    
    p_icf_vibration_req_st    = (icf_vibration_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_vibration_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_vibration_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_vibration_req_st->vibration_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_vibration_req_st->duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_vibration_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_vibration_req_st->bit_mask & ICF_VIBRATION_TIMER_VAL)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_vibration_req_st->timer_value), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_connect_media_device_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_connect_media_device_req_st    *p_icf_connect_media_device_req_st = ICF_NULL;
    
    p_icf_connect_media_device_req_st    = (icf_connect_media_device_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_connect_media_device_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_connect_media_device_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_connect_media_device_req_st->media_device), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_connect_media_device_req_st->connect_status), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_connect_media_device_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_echo_canc_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_echo_canc_req_st    *p_icf_echo_canc_req_st = ICF_NULL;
    
    p_icf_echo_canc_req_st    = (icf_echo_canc_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_echo_canc_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_echo_canc_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_echo_canc_req_st->echo_cancel), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_echo_canc_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_echo_canc_req_st->bit_mask & ICF_ECHO_CANC_STREAM_ID)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_echo_canc_req_st->stream_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}


icf_uint32_t
icf_unpack_icf_vad_ss_cng_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_vad_ss_cng_req_st    *p_icf_vad_ss_cng_req_st = ICF_NULL;
    
    p_icf_vad_ss_cng_req_st    = (icf_vad_ss_cng_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_vad_ss_cng_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_vad_ss_cng_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_STREAM_ID)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->stream_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_SUPP)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->vad_supp), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_SILENCE_SUPP)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->silence_supp), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_vad_ss_cng_req_st->bit_mask & ICF_VAD_COMFORT_NOISE)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_vad_ss_cng_req_st->comfort_noise), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_get_stats_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_get_stats_req_st    *p_icf_get_stats_req_st = ICF_NULL;
    
    p_icf_get_stats_req_st    = (icf_get_stats_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_get_stats_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_get_stats_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_stats_req_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_get_stats_req_st->stats_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_get_stats_req_st->interval_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_rtcp_report_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_rtcp_report_st    *p_icf_rtcp_report_st = ICF_NULL;
    
    p_icf_rtcp_report_st    = (icf_rtcp_report_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_rtcp_report_st */
    ICF_CLIB_TRACE(("Unpacking icf_rtcp_report_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_rtcp_report_st->time_stamp), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_rtcp_report_st->num_packet_send), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_rtcp_report_st->num_octet_send), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_rtcp_report_st->num_packet_rcvd), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_rtcp_report_st->num_octet_rcvd), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_rtcp_report_st->cum_packet_loss), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_get_stats_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_get_stats_resp_st    *p_icf_get_stats_resp_st = ICF_NULL;
    
    p_icf_get_stats_resp_st    = (icf_get_stats_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_get_stats_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_get_stats_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_get_stats_resp_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_rtcp_report_st((icf_uint8_t*)&(p_icf_get_stats_resp_st->rtcp_report), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_file_name_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
)
{
    
    icf_uint32_t        offset = 0;
    icf_uint32_t        count = 0;
    icf_uint32_t        byte_count = 0;

    
    icf_file_name_st    *p_icf_file_name_st = ICF_NULL;
    
    p_icf_file_name_st    = (icf_file_name_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_file_name_st */
    ICF_CLIB_TRACE(("Unpacking icf_file_name_st\n"));
    icf_unpack_U16((icf_uint8_t*)&(p_icf_file_name_st->str_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    if (0 >= p_icf_file_name_st->str_len || p_icf_file_name_st->str_len > ICF_CLIB_MAX_STR_LEN) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_file_name_st - Invalid String Length (%d) Bytes\n", p_icf_file_name_st->str_len));
        return ICF_FAILURE;
    } else 
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_unpack_icf_file_name_st - Unpacking String of Length (%d) Bytes\n", p_icf_file_name_st->str_len));



    /* This piece of code unpacks variable length octet string */

    count = 0;
    /* Unpack the NULL Character also*/
    while((icf_uint16_t)count < p_icf_file_name_st->str_len + 1)
    {
        icf_unpack_U8((icf_uint8_t*)&(p_icf_file_name_st->str[count]), p_src + (*p_length), &offset);
        
        *p_length    += offset;
        
        count++;
    }
    
    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_play_media_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_play_media_req_st    *p_icf_play_media_req_st = ICF_NULL;
    
    p_icf_play_media_req_st    = (icf_play_media_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_play_media_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_play_media_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_play_media_req_st->is_present), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_play_media_req_st->is_present & ICF_MEDIA_ID_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_play_media_req_st->media_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_play_media_req_st->is_present & ICF_FILE_NAME_PRESENT)
	{
		icf_unpack_icf_file_name_st((icf_uint8_t*)&(p_icf_play_media_req_st->file_name), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	icf_unpack_U8((icf_uint8_t*)&(p_icf_play_media_req_st->play_mode), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_play_media_req_st->duration), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_play_media_req_st->is_present & ICF_PLAY_TONE_TIMER_VAL)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_play_media_req_st->timer_value), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

       icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_play_media_req_st->line_id), &offset); 
       *p_length    += offset;         

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stop_media_play_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_stop_media_play_req_st    *p_icf_stop_media_play_req_st = ICF_NULL;
    
    p_icf_stop_media_play_req_st    = (icf_stop_media_play_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stop_media_play_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_stop_media_play_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_stop_media_play_req_st->is_present), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_media_play_req_st->is_present & ICF_MEDIA_ID_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_stop_media_play_req_st->media_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_stop_media_play_req_st->is_present & ICF_FILE_NAME_PRESENT)
	{
		icf_unpack_icf_file_name_st((icf_uint8_t*)&(p_icf_stop_media_play_req_st->file_name), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

     icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_stop_media_play_req_st->line_id), &offset); 
     *p_length    += offset;

     return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stop_media_play_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_stop_media_play_ind_st    *p_icf_stop_media_play_ind_st = ICF_NULL;
    
    p_icf_stop_media_play_ind_st    = (icf_stop_media_play_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stop_play_file_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_stop_play_file_ind_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_stop_media_play_ind_st->is_present), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_media_play_ind_st->is_present & ICF_MEDIA_ID_PRESENT)
	{
		icf_unpack_U16((icf_uint8_t*)&(p_icf_stop_media_play_ind_st->media_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	if(p_icf_stop_media_play_ind_st->is_present & ICF_FILE_NAME_PRESENT)
	{
		icf_unpack_icf_file_name_st((icf_uint8_t*)&(p_icf_stop_media_play_ind_st->file_name), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_stop_media_play_ind_st->line_id), &offset); 
    *p_length    += offset;
	return ICF_SUCCESS;
}

/***************************************************

			VIDEO/VOICE APPLICATION APIs

****************************************************/

icf_uint32_t
icf_unpack_icf_dgts_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;
	  
    icf_dgts_st    *p_icf_dgts_st = ICF_NULL;
    
    p_icf_dgts_st    = (icf_dgts_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_dgts_st */
    ICF_CLIB_TRACE(("Unpacking icf_dgts_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_dgts_st->number_of_dgts), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	while(count<p_icf_dgts_st->number_of_dgts)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_dgts_st->dgts[count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		count++;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_dgts_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_dgts_ind_st    *p_icf_dgts_ind_st = ICF_NULL;
    
    p_icf_dgts_ind_st    = (icf_dgts_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_dgts_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_dgts_ind_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_dgts_ind_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_dgts_st((icf_uint8_t*)&(p_icf_dgts_ind_st->digits), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_dgts_ind_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_mute_media_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_mute_media_req_st    *p_icf_mute_media_req_st = ICF_NULL;
    
    p_icf_mute_media_req_st    = (icf_mute_media_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_mute_media_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_mute_media_req_st\n"));

    icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_mute_media_req_st->stream_list), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_mute_media_req_st->activate_mute), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_mute_media_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_mute_media_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
    icf_uint32_t        offset = 0;

    icf_mute_media_resp_st    *p_icf_mute_media_resp_st = ICF_NULL;
    
    p_icf_mute_media_resp_st    = (icf_mute_media_resp_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_mute_media_resp_st */
    ICF_CLIB_TRACE(("Unpacking icf_mute_media_resp_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_mute_media_resp_st->bit_mask), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U8((icf_uint8_t*)&(p_icf_mute_media_resp_st->result), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_mute_media_resp_st->error_cause), p_src + (*p_length), &offset);
    *p_length    += offset;

    if (p_icf_mute_media_resp_st->bit_mask & ICF_MUTE_MEDIA_RESP_LINE_ID_PRESENT)
    {
       icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_mute_media_resp_st->line_id), &offset); 
       *p_length    += offset;        
    }

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_set_media_playback_device_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_set_media_playback_device_req_st    *p_icf_set_media_playback_device_req_st = ICF_NULL;
    
    p_icf_set_media_playback_device_req_st    = (icf_set_media_playback_device_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_media_playback_device_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_media_playback_device_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_set_media_playback_device_req_st->media_play_out_device), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_set_media_playback_device_req_st->line_id), &offset); 
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_media_record_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_media_record_req_st    *p_icf_media_record_req_st = ICF_NULL;
    
    p_icf_media_record_req_st    = (icf_media_record_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_media_record_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_media_record_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_media_record_req_st->media_source), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_media_record_req_st->file_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_file_name_st((icf_uint8_t*)&(p_icf_media_record_req_st->file_name), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_media_record_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_media_record_req_st->bit_mask & ICF_STREAM_ID_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_media_record_req_st->stream_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_media_record_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_send_dgts_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_send_dgts_st    *p_icf_send_dgts_st = ICF_NULL;
    
    p_icf_send_dgts_st    = (icf_send_dgts_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_send_dgts_st */
    ICF_CLIB_TRACE(("Unpacking icf_send_dgts_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_send_dgts_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_send_dgts_st->transport_method), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_send_dgts_st->digits), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_set_earp_volume_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_set_earp_volume_req_st    *p_icf_set_earp_volume_req_st = ICF_NULL;
    
    p_icf_set_earp_volume_req_st    = (icf_set_earp_volume_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_earp_volume_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_earp_volume_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_set_earp_volume_req_st->level), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_set_earp_volume_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_set_speaker_volume_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_set_speaker_volume_req_st    *p_icf_set_speaker_volume_req_st = ICF_NULL;
    
    p_icf_set_speaker_volume_req_st    = (icf_set_speaker_volume_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_speaker_volume_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_speaker_volume_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_set_speaker_volume_req_st->level), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_set_speaker_volume_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_stop_media_record_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_stop_media_record_req_st    *p_icf_stop_media_record_req_st = ICF_NULL;
    
    p_icf_stop_media_record_req_st    = (icf_stop_media_record_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stop_media_record_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_stop_media_record_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_stop_media_record_req_st->media_source), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_stop_media_record_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_media_record_req_st->bit_mask & ICF_STREAM_ID_PRESENT)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_stop_media_record_req_st->stream_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_stop_media_record_req_st->line_id), &offset); 
    *p_length    += offset;
  
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_set_mic_gain_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_set_mic_gain_req_st    *p_icf_set_mic_gain_req_st = ICF_NULL;
    
    p_icf_set_mic_gain_req_st    = (icf_set_mic_gain_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_set_mic_gain_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_set_mic_gain_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_set_mic_gain_req_st->gain_level), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_pause_media_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_pause_media_req_st    *p_icf_pause_media_req_st = ICF_NULL;
    
    p_icf_pause_media_req_st    = (icf_pause_media_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_pause_media_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_pause_media_req_st\n"));

    icf_unpack_icf_stream_id_list_st((icf_uint8_t*)&(p_icf_pause_media_req_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_pause_media_req_st->activate_pause), p_src + (*p_length), &offset);
    
    *p_length    += offset;

    icf_unpack_icf_line_id_t(p_dest + (*p_length), (icf_line_id_t*)&(p_icf_pause_media_req_st->line_id), &offset); 
    *p_length    += offset;

	return ICF_SUCCESS;
}

/***************************************************

				PUSH TO TALK APIs

****************************************************/

icf_uint32_t
icf_unpack_icf_stop_stats_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_stop_stats_req_st    *p_icf_stop_stats_req_st = ICF_NULL;
    
    p_icf_stop_stats_req_st    = (icf_stop_stats_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_stop_stats_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_stop_stats_req_st\n"));

    icf_unpack_U32((icf_uint8_t*)&(p_icf_stop_stats_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_stop_stats_req_st->bit_mask & ICF_STOP_STATS_REQ_STREAM_ID)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_stop_stats_req_st->stream_id), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_app_data_st
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
    
    p_icf_app_data_st    = (icf_app_data_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_app_data_st */
    ICF_CLIB_TRACE(("Unpacking icf_app_data_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_app_data_st->str_len), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_app_data_st->sub_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	
	while(app_count<4)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_app_data_st->app_name[app_count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		app_count++;
	}

	while(count<p_icf_app_data_st->str_len)
	{
		icf_unpack_U8((icf_uint8_t*)&(p_icf_app_data_st->str[count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		count++;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_rcv_app_data_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_rcv_app_data_ind_st    *p_icf_rcv_app_data_ind_st = ICF_NULL;
    
    p_icf_rcv_app_data_ind_st    = (icf_rcv_app_data_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_rcv_app_data_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_rcv_app_data_ind_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_app_data_st((icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->application_data), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->ssrc_num), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_rcv_app_data_ind_st->bit_mask & ICF_MM_PTT_RCV_APP_DATA_IND_SEQUENCE_NUMBER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_rcv_app_data_ind_st->sequence_num), p_src + (*p_length), &offset);
    
		*p_length    += offset;

	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_send_app_data_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_send_app_data_req_st    *p_icf_send_app_data_req_st = ICF_NULL;
    
    p_icf_send_app_data_req_st    = (icf_send_app_data_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_send_app_data_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_send_app_data_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_send_app_data_req_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_app_data_st((icf_uint8_t*)&(p_icf_send_app_data_req_st->application_data), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U32((icf_uint8_t*)&(p_icf_send_app_data_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(p_icf_send_app_data_req_st->bit_mask & ICF_SEND_APP_DATA_REQ_SEQUENCE_NUMBER)
	{
		icf_unpack_U32((icf_uint8_t*)&(p_icf_send_app_data_req_st->sequence_num), p_src + (*p_length), &offset);
    
		*p_length    += offset;

	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_send_stats_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_send_stats_req_st    *p_icf_send_stats_req_st = ICF_NULL;
    
    p_icf_send_stats_req_st    = (icf_send_stats_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_send_stats_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_send_stats_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_send_stats_req_st->stream_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_send_stats_req_st->report_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

#endif
/***************************************************

			     CONFIGURATION APIs

****************************************************/


icf_uint32_t
icf_unpack_icf_encoded_params_list_st
(
	icf_list_st    **p_p_icf_encoded_params_list_st,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

	/* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_encoded_params_list_st */
    ICF_CLIB_TRACE(("Unpacking icf_encoded_params_list_st\n"));

	/*Unpack count*/
	icf_unpack_U8((icf_uint8_t*)&count, p_src + (*p_length), &offset);
    
	*p_length    += offset;

	while(count--)
    {
				
		*p_p_icf_encoded_params_list_st = (icf_list_st *)ICF_CLIB_MEMGET(sizeof(icf_list_st));

		/* get memory for icf_encoded_params_list_st*/
		(*p_p_icf_encoded_params_list_st)->p_data = (icf_string_st*)ICF_CLIB_MEMGET(sizeof(icf_string_st));
		
		icf_unpack_icf_string_st((icf_uint8_t*)((*p_p_icf_encoded_params_list_st)->p_data), p_src + (*p_length), &offset);

		*p_length    += offset;

		p_p_icf_encoded_params_list_st = &((*p_p_icf_encoded_params_list_st)->p_next);

	}

	(*p_p_icf_encoded_params_list_st) = ICF_NULL;

	return ICF_SUCCESS;
}



icf_uint32_t
icf_unpack_icf_config_codec_attrib_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_config_codec_attrib_st    *p_icf_config_codec_attrib_st = ICF_NULL;
    
    p_icf_config_codec_attrib_st    = (icf_config_codec_attrib_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_config_codec_attrib_st */
    ICF_CLIB_TRACE(("Unpacking icf_config_codec_attrib_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_config_codec_attrib_st->bitmask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_short_string_st((icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_name), p_src + (*p_length), &offset);
    
	*p_length    += offset;
#if 1 /* ZyXEL_PORTING*/
	icf_unpack_U8((icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_num), p_src + (*p_length), &offset);
    
    *p_length    += offset;
	
	icf_unpack_U8((icf_uint8_t*)&(p_icf_config_codec_attrib_st->enable), p_src + (*p_length), &offset);
    
    *p_length    += offset;
#endif

	icf_unpack_icf_encoded_params_list_st(&(p_icf_config_codec_attrib_st->p_encoded_params), p_src + (*p_length), &offset);
    
	*p_length    += offset;

	if(p_icf_config_codec_attrib_st->bitmask & ICF_CODEC_PARAM_PRESENT)
	{
		switch(p_icf_config_codec_attrib_st->codec_type)
		{
			case  ICF_CODEC_TYPE_AUDIO:
				  
				  icf_unpack_icf_config_audio_codec_param_st((icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_CODEC_TYPE_VIDEO:
				  
				  icf_unpack_icf_config_video_codec_param_st((icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;

			case  ICF_CODEC_TYPE_IMAGE:
				  
				  icf_unpack_icf_config_t38_attrib_param_st((icf_uint8_t*)&(p_icf_config_codec_attrib_st->codec_param), p_src + (*p_length), &offset);
    
				  *p_length    += offset;

				  break;
		}
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_codec_info_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t        count = 0;

    icf_cfg_codec_info_req_st    *p_icf_cfg_codec_info_req_st = ICF_NULL;
    
    p_icf_cfg_codec_info_req_st    = (icf_cfg_codec_info_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_codec_info_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_codec_info_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_cfg_codec_info_req_st->count), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	while(count<p_icf_cfg_codec_info_req_st->count)
	{
		icf_unpack_icf_config_codec_attrib_st((icf_uint8_t*)&(p_icf_cfg_codec_info_req_st->codec_supp[count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		count++;
	}
#if 1 /* ZyXEL_PORTING*/
    icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_codec_info_req_st->line_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;
#endif
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_mm_unexpected_msg_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
        icf_uint32_t        offset = 0;

    icf_mm_unexpected_msg_ind_st    *p_icf_mm_unexpected_msg_ind_st = ICF_NULL;
   
    p_icf_mm_unexpected_msg_ind_st    = (icf_mm_unexpected_msg_ind_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_mm_unexpected_msg_ind_st */
    ICF_CLIB_TRACE(("Unpacking icf_mm_unexpected_msg_ind_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_mm_unexpected_msg_ind_st->error_cause), p_src + (*p_length), &offset);
    *p_length    += offset;

    icf_unpack_U16((icf_uint8_t*)&(p_icf_mm_unexpected_msg_ind_st->api_id), p_src + (*p_length), &offset);
    *p_length    += offset;


    return ICF_SUCCESS;
}


#ifdef ICF_MM_CFG_FUTURE_RELEASE

icf_uint32_t
icf_unpack_icf_ringer_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_ringer_id_st    *p_icf_ringer_id_st = ICF_NULL;
    
    p_icf_ringer_id_st    = (icf_ringer_id_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_ringer_id_st */
    ICF_CLIB_TRACE(("Unpacking icf_ringer_id_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_ringer_id_st->ringer_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_file_name_st((icf_uint8_t*)&(p_icf_ringer_id_st->play_file), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_ringer_tones_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_cfg_ringer_tones_req_st    *p_icf_cfg_ringer_tones_req_st = ICF_NULL;
    
    p_icf_cfg_ringer_tones_req_st    = (icf_cfg_ringer_tones_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_ringer_tones_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_ringer_tones_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_ringer_tones_req_st->num_of_ringer), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	while(count<p_icf_cfg_ringer_tones_req_st->num_of_ringer)
	{
		icf_unpack_icf_ringer_id_st((icf_uint8_t*)&(p_icf_cfg_ringer_tones_req_st->ringer[count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		count++;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_media_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;

    icf_media_id_st    *p_icf_media_id_st = ICF_NULL;
    
    p_icf_media_id_st    = (icf_media_id_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_media_id_st */
    ICF_CLIB_TRACE(("Unpacking icf_media_id_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_media_id_st->media_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_icf_file_name_st((icf_uint8_t*)&(p_icf_media_id_st->play_file), p_src + (*p_length), &offset);
    
	*p_length    += offset;
	
	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_media_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_cfg_media_req_st    *p_icf_cfg_media_req_st = ICF_NULL;
    
    p_icf_cfg_media_req_st    = (icf_cfg_media_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_media_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_media_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_media_req_st->count), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	while(count<p_icf_cfg_media_req_st->count)
	{
		icf_unpack_icf_media_id_st((icf_uint8_t*)&(p_icf_cfg_media_req_st->media[count]), p_src + (*p_length), &offset);
    
		*p_length    += offset;

		count++;
	}

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_set_digit_trans_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_cfg_set_digit_trans_req_st    *p_icf_cfg_set_digit_trans_req_st = ICF_NULL;
    
    p_icf_cfg_set_digit_trans_req_st    = (icf_cfg_set_digit_trans_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_set_digit_trans_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_set_digit_trans_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_cfg_set_digit_trans_req_st->dgt_trns_method), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_jbm_mgmt_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_cfg_jbm_mgmt_req_st    *p_icf_cfg_jbm_mgmt_req_st = ICF_NULL;
    
    p_icf_cfg_jbm_mgmt_req_st    = (icf_cfg_jbm_mgmt_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_jbm_mgmt_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_jbm_mgmt_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_type), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U8((icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_scaling_factor), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_initial_size), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_max_size), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_jbm_mgmt_req_st->jbm_min_size), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_max_earp_vol_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_cfg_max_earp_vol_req_st    *p_icf_cfg_max_earp_vol_req_st = ICF_NULL;
    
    p_icf_cfg_max_earp_vol_req_st    = (icf_cfg_max_earp_vol_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_max_earp_vol_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_max_earp_vol_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_max_earp_vol_req_st->max_earp_volume), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_max_speaker_vol_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_cfg_max_speaker_vol_req_st    *p_icf_cfg_max_speaker_vol_req_st = ICF_NULL;
    
    p_icf_cfg_max_speaker_vol_req_st    = (icf_cfg_max_speaker_vol_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_max_speaker_vol_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_max_speaker_vol_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_max_speaker_vol_req_st->max_speaker_volume), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_cfg_max_mic_gain_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_cfg_max_mic_gain_req_st    *p_icf_cfg_max_mic_gain_req_st = ICF_NULL;
    
    p_icf_cfg_max_mic_gain_req_st    = (icf_cfg_max_mic_gain_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_cfg_max_mic_gain_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_cfg_max_mic_gain_req_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_cfg_max_mic_gain_req_st->max_mic_gain), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}

icf_uint32_t
icf_unpack_icf_video_display_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	icf_uint8_t         count = 0;

    icf_video_display_st    *p_icf_video_display_st = ICF_NULL;
    
    p_icf_video_display_st    = (icf_video_display_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_video_display_st */
    ICF_CLIB_TRACE(("Unpacking icf_video_display_st\n"));

    icf_unpack_U16((icf_uint8_t*)&(p_icf_video_display_st->top), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_video_display_st->left), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_video_display_st->width), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	icf_unpack_U16((icf_uint8_t*)&(p_icf_video_display_st->height), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	return ICF_SUCCESS;
}
#endif

icf_uint32_t
icf_unpack_icf_media_capabilities_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
)
{
	icf_uint32_t        offset = 0;
	  
    icf_media_capabilities_req_st    *p_icf_media_capabilities_req_st = ICF_NULL;
    
    p_icf_media_capabilities_req_st    = (icf_media_capabilities_req_st*)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_media_capabilities_req_st */
    ICF_CLIB_TRACE(("Unpacking icf_media_capabilities_req_st\n"));

    icf_unpack_U8((icf_uint8_t*)&(p_icf_media_capabilities_req_st->bit_mask), p_src + (*p_length), &offset);
    
    *p_length    += offset;

	if(ICF_MED_CAP_STREAM_PRSNT & p_icf_media_capabilities_req_st->bit_mask)
	{
		icf_unpack_icf_stream_capab_list_st(&(p_icf_media_capabilities_req_st->p_req_stream_list), p_src + (*p_length), &offset);
    
		*p_length    += offset;
	}

	return ICF_SUCCESS;
}
