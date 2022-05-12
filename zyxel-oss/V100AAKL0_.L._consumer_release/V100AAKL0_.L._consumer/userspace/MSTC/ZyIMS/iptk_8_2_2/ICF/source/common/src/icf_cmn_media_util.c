/*******************************************************************************
 *
 * FILENAME         : icf_cmn_sdp_util.c
 *
 * DESCRIPTION      :
 *
 * Revision History :
 *
 * DATE         NAME                REFERENCE       REASON
 * -----------  -------------    --------------    --------------
 * 28-July-2006 Aman Arora        ICF Rel 6.0     The file has common utility 
 * 						functions for managing SDP
 * 						structures within ICF
 *
 * 4-Dec-2006   Deepti Goyal       ICF Rel 6.2    NAT Feature Support changes
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 10-Apr-2008  Alok Tiwari       SPR-18364      Klocwork warning removal
 * 15-Apr-2008  Alok Tiwari       SPR-18369      Klocwork warning removal
 * 18-Apr-2008  Alok Tiwari       SPR-18456      Klocwork warning removal
 * 29-Apr-2008  Shruti Thakral    SPR 18455      Klocwork warning removal
 * 03-Jul-2008  Anurag Khare      SPR 18684      Comilation issue on gcc 4.2.3
 * 04-Nov-2008  Rajiv Kumar       SPR 19188      Rel 7.1.2 SPR merging (SPR
 *                                               19001)
 * 05-Dec-2008  Tarun Gupta       SPR 19218      Merged CSR 1-6367281
 * 16-Dec-2008  Anurag Khare      SPR 19218      CSR 1-6449001 Merge
 * 10-Feb-2009  Anurag Khare      ICF Rel 8.1    Changes done for Tunnel mode 
 * 12-Feb-2009  Ashutosh Mohan    Rel8.1         Changes done for Tunnel mode 
 * 18-Feb-2009  Anurag Khare      Rel 8.1        KlocWorks Warning removal
 * 20-May-2009  Ashutosh Mohan    CSR-1-6794762  Modified icf_cmn_clone_sdp_stream_params
 * 28-May-2009  Ashutosh Mohan    SPR 19672      CSR-1-6976670 Merged
 * 11-Jun-2009 Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal
 * 06-Oct-2009 Abhishek Dhammawat SPR 20063     Merged CSR 1-7732501
 * Copyright (c) 2009, Aricent.
 ******************************************************************************/
/* included files */
#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_port_macros.h"
#include "icf_port_defs.h"
#include "icf_port.h"
#include "icf_macro.h"
#include "icf_sm_prototypes.h"
#include "icf_dbm_types.h"
#include "icf_dbm_prototypes.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif


/* function definitions */
/******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_sdp_in_list_for_stream_id
 * 
 * DESCRIPTION:     This is a utility function that will be used to fetch the
 * 					SDP node from within the CC's icf_sdp_stream_info_st 
 * 					list for the given stream_id. The type of node returned 
 * 					in the function depends on the SDP_TYPE passed as an 
 * 					argument to this function
 *
 *****************************************************************************/
icf_return_t	icf_cmn_fetch_sdp_in_list_for_stream_id
(
	INOUT icf_glb_pdb_st				*p_glb_pdb,
	IN	  icf_stream_id_t				stream_id,
	IN	  icf_uint8_t					sdp_type,
	INOUT icf_internal_sdp_stream_st	**p_p_dest_sdp
)
{
	/* local variables */
	icf_internal_list_st		*p_tmp_node = ICF_NULL;
	icf_stream_sdp_info_st		*p_stream_sdp = ICF_NULL;
	icf_return_t				ret_val = ICF_FAILURE;
	icf_internal_sdp_stream_st	*p_tmp_sdp = ICF_NULL;
	icf_uint32_t				loc_stream = stream_id;
	
	p_tmp_node = icf_cmn_get_node_in_internal_list(
					p_glb_pdb->p_call_ctx->p_stream_sdp_info,
					&loc_stream);

	if(ICF_NULL != p_tmp_node)
	{
		p_stream_sdp = p_tmp_node->p_payload;

		/* if node retrieved successfully */
		switch (sdp_type)
		{
			case ICF_SDP_LOCAL:
				p_tmp_sdp = p_stream_sdp->p_local_sdp;
				break;
				
			case ICF_SDP_REMOTE:
				p_tmp_sdp = p_stream_sdp->p_remote_received_sdp;
				break;
				
			case ICF_SDP_APPLIED:
				p_tmp_sdp = p_stream_sdp->p_applied_sdp;
				break;
				
			case ICF_SDP_LAST_COMMITED:
				p_tmp_sdp = p_stream_sdp->p_last_commited_sdp;
				break;
				
			default:
				p_tmp_sdp = ICF_NULL;
				break;
		} /* switch ends */
	}
	else
	{
		*p_p_dest_sdp = ICF_NULL;
	}

	if(ICF_NULL != p_tmp_sdp)
	{
		*p_p_dest_sdp = p_tmp_sdp;

		ret_val = ICF_SUCCESS;
	}
	else
	{
		*p_p_dest_sdp = ICF_NULL;
	}
	
	return ret_val;
}/* function ends */


/******************************************************************************
 *
 * FUNCTION:        icf_cmn_update_codecs_for_stream_id_in_call_ctxt
 * 
 * DESCRIPTION:     This is a utility function that will copy the contents of
 * 					source sdp (for a given stream_id) into the designated
 * 					SDP node in the CC call context. 
 * 					This function will only copy the contents of 
 * 					icf_codec_attrib_st into the destination
 *
 *****************************************************************************/
icf_return_t	icf_cmn_update_codecs_for_stream_id_in_call_ctxt
(
	INOUT icf_glb_pdb_st		*p_glb_pdb,
	IN	  icf_stream_id_t		stream_id,
	IN	  icf_uint8_t			sdp_type,
	INOUT icf_sdp_stream_st		*p_src_sdp
)
{
	/* local variables */
	icf_sdp_stream_st			*p_sdp = ICF_NULL;
	icf_internal_sdp_stream_st	*p_tmp_sdp_node = ICF_NULL;
							
	icf_error_t			ecode = ICF_ERROR_UNDEFINED;
	icf_return_t		ret_val = ICF_FAILURE;
	

	/* get the node for the stream_id from the CC- sdp_stream_info */
	if (ICF_SUCCESS == icf_cmn_fetch_sdp_in_list_for_stream_id(
							p_glb_pdb,
							stream_id,
							sdp_type,
							&p_tmp_sdp_node))
	{
		if(p_tmp_sdp_node)
		{
			p_sdp	= &p_tmp_sdp_node->sdp_stream;
		}
	
		if(ICF_NULL != p_sdp)
		{
			/* copy the icf_codec_attrib_st into the extracted structure from 
			 * the source */
			if (ICF_NULL != p_sdp->p_codec_info_list)	
			{
				icf_cmn_delete_codec_attrib_list(p_glb_pdb,
							&p_sdp->p_codec_info_list,
							&ecode);
			}
			/* copy the src codec_attrib_st into p_tmp_node */
			icf_cmn_clone_codec_attrib_list(p_glb_pdb,
							&(p_sdp->p_codec_info_list),
							p_src_sdp->p_codec_info_list);
            /*Klocwork Warning Removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_sdp->p_codec_info_list)
			
			ret_val = ICF_SUCCESS;			
		} /* while loop ends */
	}/* if case ends */
	
	return ret_val;
}/* function ends */

/*****************************************************************************
 ** FUNCTION:     icf_cmn_delete_codec_attrib_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_string_st
 **
 *****************************************************************************/
icf_return_t icf_cmn_delete_codec_attrib_list
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_p_codec_attrib,
	INOUT	icf_error_t    	*p_ecode
)
{
	/* local variables */	
	icf_list_st		*p_codec_attrib= *p_p_codec_attrib;
	icf_list_st		*p_temp_codec_attrib = ICF_NULL;
	icf_return_t 	ret_val=ICF_SUCCESS;
	icf_codec_attrib_st	*p_codec_attrib_data =ICF_NULL;

	/* Following while loop is deleting first the payload part of the node and
	 * then deleting the node itself.*/
	while(ICF_NULL != p_codec_attrib)
	{
		p_codec_attrib_data=(icf_codec_attrib_st *)
				ICF_CMN_GET_DATA_NODE_FOR_LIST(p_codec_attrib); 

		if (p_codec_attrib_data != ICF_NULL)
		{
			/*This function is releasing the memory assigned to 
			p_encoded_attributes list inside the node of codec attrib list.*/
			if (ICF_CODEC_ENC_ATTRIB_PRESENT & 
							p_codec_attrib_data->bit_mask)
			{
				if (ICF_NULL != p_codec_attrib_data->p_encoded_attributes)
				{
					ret_val = icf_cmn_delete_list(p_glb_pdb,
						(&(p_codec_attrib_data->p_encoded_attributes)),
						ICF_MEM_COMMON);
				}
				
			}

            /*Releasing the memory assigned to data part.*/
			ICF_MEMFREE	(
					p_glb_pdb, 
                	(p_codec_attrib_data), 
                	ICF_MEM_COMMON, 
					p_ecode, 
					ret_val);

		}/*End of if (p_codec_attrib_data != NULL) */

		p_temp_codec_attrib = p_codec_attrib;
		p_codec_attrib = p_codec_attrib->p_next;
		
		/*Releasing the memory assigned to list node itself.*/
		ICF_MEMFREE (
					p_glb_pdb, 
                	(p_temp_codec_attrib), 
                	ICF_MEM_COMMON, 
					p_ecode, 
					ret_val
					);

	}/*End of while(p_codec_attrib != NULL) */
	*p_p_codec_attrib=ICF_NULL;
	return ret_val;
}/* function ends */

/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_codec_attrib_list
 **
 ** DESCRIPTION:  This function clones the icf_codec_attrib_st from the
 **				  source to the destination sdp_stream
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_codec_attrib_list
(
 	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_list_st	**p_p_dest_codec_attrib,
	INOUT	icf_list_st			*p_src_codec_attrib
)
{
	/* local variables */
	icf_list_st		*p_curr_node = ICF_NULL;
	icf_codec_attrib_st	*p_codec_payload = ICF_NULL,
                        *p_curr_codec_node = ICF_NULL;
	icf_return_t 	ret_val=ICF_SUCCESS;
	icf_error_t  	ecode = ICF_ERROR_UNDEFINED; 


	if (ICF_NULL != *p_p_dest_codec_attrib)
	{
		ret_val = icf_cmn_delete_codec_attrib_list(p_glb_pdb,
						p_p_dest_codec_attrib,
						&ecode);
		if (ICF_FAILURE == ret_val)
		{
			return ret_val;
		}
	}

	p_curr_node = p_src_codec_attrib;
	while (p_curr_node)
	{
		p_curr_codec_node = (icf_codec_attrib_st*)
				ICF_CMN_GET_DATA_NODE_FOR_LIST(p_curr_node);
		
		/* allocate memory for the code_attrib_st- payload */
		ICF_MEMGET (p_glb_pdb,
					ICF_PORT_SIZEOF(icf_codec_attrib_st),
					ICF_MEM_COMMON,
					p_codec_payload,
					ICF_DONOT_RET_ON_FAILURE,
					&ecode,
					ret_val);
                /*klocwork warning removal*/
                ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_codec_payload);

		if (ICF_SUCCESS == ret_val)
		{
 
			p_codec_payload->bit_mask = p_curr_codec_node->bit_mask;
			p_codec_payload->codec_num = p_curr_codec_node->codec_num;
			
                        icf_port_strcpy(p_codec_payload->codec_name.str,
                        p_curr_codec_node->codec_name.str);
            
			p_codec_payload->codec_name.str_len = 
					p_curr_codec_node->codec_name.str_len;
		
			p_codec_payload->codec_params =
				p_curr_codec_node->codec_params;
		
			if (ICF_CODEC_ENC_ATTRIB_PRESENT & p_curr_codec_node->bit_mask)
			{
				/* clone the codec attributes */
				/* in the current release this is not going to be used, so this
			 	 * block is empty- no concept of encoded attribute lines within
			     * the codec */
			}
			p_codec_payload->bit_mask = p_curr_codec_node->bit_mask;

			/* payload is prepared, not add this to icf_list_st */
			if(ICF_FAILURE == icf_cmn_add_node_to_list(p_glb_pdb,
						p_p_dest_codec_attrib,
						p_codec_payload))
			{
                                icf_cmn_delete_codec_attrib_list(p_glb_pdb,
						p_p_dest_codec_attrib,
						&ecode);
                                /*klocwork warning removal*/
                                ICF_MEMFREE(p_glb_pdb,
                                            p_codec_payload,
                                            ICF_MEM_COMMON, 
                                            &ecode,
                                            ret_val)

				ret_val = ICF_FAILURE;
				break;
			}

			p_curr_node = p_curr_node->p_next;
		}
                else
                {
                        icf_cmn_delete_codec_attrib_list(p_glb_pdb,
						p_p_dest_codec_attrib,
						&ecode);
                        ret_val = ICF_FAILURE;
                        break;                      
                }
	}/* while loop ends */
	return ret_val;					
}/* function ends */

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_codec_attrib_list_count
*
*  DESCRIPTION    :  This function is used to get the count/size of the
*                    codec attrib list, excluding the telephone-event,fec and red,
*                    and comfort noise.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t      icf_cmn_get_codec_attrib_list_count
(
        INOUT   icf_list_st             *p_list,
        INOUT   icf_uint8_t             *p_count
)
{
        /* local variable */
        icf_list_st     *p_tmp_list = ICF_NULL;
        icf_uint8_t     *p_codec_name = ICF_NULL;

        *p_count = 0;
        p_tmp_list = p_list;

        while (ICF_NULL != p_tmp_list)
        {
            p_codec_name = (((icf_codec_attrib_st *)p_tmp_list->p_data)->\
                           codec_name.str);
            /* SPR 19187 : Special handling for fec/red */
            /* CSR 1-7732501: SPR 20073: Special handling for CN */
    		if ((ICF_NULL != icf_port_strcmp((icf_uint8_t *)"telephone-event", p_codec_name)) &&
	    		(ICF_NULL != icf_port_strcmp((icf_uint8_t *)"parityfec", p_codec_name)) &&
		    	(ICF_NULL != icf_port_strcmp((icf_uint8_t *)"red", p_codec_name)) &&
                (ICF_NULL != icf_port_strcmp((icf_uint8_t *)"CN", p_codec_name)))
            {
                *p_count += 1;
            }
            p_tmp_list = p_tmp_list->p_next;
        }
        return ;
}
		
/*****************************************************************************
 ** FUNCTION:     icf_cmn_update_media_params_for_stream_id_in_call_ctxt
 **
 ** DESCRIPTION:  This function clones the parameters of icf_sdp_stream_st
 **				  minus the codec_attribute_st. the codec parameters are
 **	              seperately in another function. The function also fetches
 **				  the node corresponding to the given stream_id from the
 **				  icf_sdp_stream_info_st in the call_ctxt
 **
 *****************************************************************************/
icf_return_t	icf_cmn_update_media_params_for_stream_id_in_call_ctxt
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_uint8_t			sdp_type,
	INOUT	icf_sdp_stream_st	*p_src_sdp
)
{
	/* local variables */
	icf_sdp_stream_st			*p_sdp = ICF_NULL;
	icf_internal_sdp_stream_st	*p_internal_sdp = ICF_NULL;
	icf_return_t				ret_val = ICF_FAILURE;
	
	/* get the node for the stream_id from the CC- sdp_stream_info */
	if (ICF_SUCCESS == icf_cmn_fetch_sdp_in_list_for_stream_id(
							p_glb_pdb,
							stream_id,
							sdp_type,
							&p_internal_sdp))
	{
		if(p_internal_sdp)
		{
			p_sdp = &p_internal_sdp->sdp_stream;			
		}
	
		if(p_sdp)
		{
			/* copy parameters in the SDP stream ,minus the codec params */
			icf_cmn_clone_sdp_stream_params(
							p_glb_pdb,sdp_type,
							p_sdp,
							p_src_sdp);
           
           /*Klocwork Warning Removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_sdp->p_encoded_attributes)
			
			ret_val = ICF_SUCCESS;			
		} /* while loop ends */
	}/* if case ends */
	
	return ret_val;

}/* function ends */


/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_sdp_stream_params
 **
 ** DESCRIPTION:  This function clones the parameters of icf_sdp_stream_st
 **				  minus the codec_attribute_st. the codec parameters are
 **	              seperately in another function.
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_sdp_stream_params
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN	icf_uint8_t		sdp_type,
	INOUT	icf_sdp_stream_st	*p_dest_sdp_stream,
	INOUT	icf_sdp_stream_st	*p_src_sdp_stream
)
{
	/* local variables */
	icf_error_t		ecode = ICF_ERROR_UNDEFINED;
    /* TO remove warning */
    if(ICF_SDP_LOCAL != sdp_type)
    {
    }

#ifdef ICF_NAT_MEDIA_TRAVERSAL
    if(ICF_SDP_LOCAL != sdp_type)
    {
        p_dest_sdp_stream->stream_address = p_src_sdp_stream->\
                                            stream_address;
    }
#else
    p_dest_sdp_stream->stream_address = p_src_sdp_stream->stream_address;
#endif
    p_dest_sdp_stream->trans_mode = p_src_sdp_stream->trans_mode;
    if (ICF_SDP_STREAM_MEDIA_STREAM_PRESENT & p_src_sdp_stream->bit_mask)
	{
		p_dest_sdp_stream->media_stream_param = 
			p_src_sdp_stream->media_stream_param;
		p_dest_sdp_stream->bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
	}
#ifdef ICF_NAT_MEDIA_TRAVERSAL
    if(ICF_SDP_LOCAL != sdp_type)
    {
        if (ICF_SDP_STREAM_RTCP_INFO_PRESENT & p_src_sdp_stream->bit_mask)
        {
            p_dest_sdp_stream->rtcp_address = p_src_sdp_stream->\
                                              rtcp_address;
            p_dest_sdp_stream->bit_mask |= \
                                        ICF_SDP_STREAM_RTCP_INFO_PRESENT;
        }
    }
#else
    if (ICF_SDP_STREAM_RTCP_INFO_PRESENT & p_src_sdp_stream->bit_mask)
    {
        p_dest_sdp_stream->rtcp_address = p_src_sdp_stream->\
                                          rtcp_address;
        p_dest_sdp_stream->bit_mask |= \
                                       ICF_SDP_STREAM_RTCP_INFO_PRESENT;
    }

#endif
	if (ICF_SDP_STREAM_ENCPARAMS_PRESENT & p_src_sdp_stream->bit_mask)
	{
		icf_cmn_copy_string_list(p_glb_pdb,
						&(p_dest_sdp_stream->p_encoded_attributes),
						p_src_sdp_stream->p_encoded_attributes,
						&ecode);

		p_dest_sdp_stream->bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
	}
        /*CSR 1-6794762: In case th enc params is absent in RESP SDP,
          remove the same from the local SDP*/
        else if (ICF_SDP_STREAM_ENCPARAMS_PRESENT & p_dest_sdp_stream->bit_mask)
        {
            icf_cmn_delete_list(p_glb_pdb,
                &p_dest_sdp_stream->p_encoded_attributes,
                ICF_MEM_COMMON);
            p_dest_sdp_stream->bit_mask &= ~ICF_SDP_STREAM_ENCPARAMS_PRESENT;
        }

	return ICF_SUCCESS;
}/* function ends */


/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_media_sdp_info_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_inc_media_sdp_info_st
 **
 *****************************************************************************/
icf_return_t icf_cmn_free_media_sdp_info_list(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_list_st	                **p_p_media_sdp_info,
    OUT     icf_error_t                 *p_ecode)
{
    /* local variables */	
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_list_st                         *p_media_sdp_list = ICF_NULL;
    icf_inc_media_sdp_info_st           *p_media_sdp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)
 
    /* Free the acquired memory for the SDP in the nodes of the list */

    p_media_sdp_list = *(p_p_media_sdp_info);
    if(ICF_NULL != p_media_sdp_list)
    {    
        p_media_sdp = (icf_inc_media_sdp_info_st *)(p_media_sdp_list->p_data);
    }    

    while(ICF_NULL != p_media_sdp)
    {
        if (ICF_NULL != p_media_sdp->p_int_sdp_stream)
        {

            if(ICF_NULL !=
                p_media_sdp->p_int_sdp_stream->sdp_stream.p_codec_info_list)
            {
                icf_cmn_delete_codec_attrib_list(
                    p_glb_pdb,
                    &(p_media_sdp->p_int_sdp_stream->sdp_stream.p_codec_info_list),
                    p_ecode);
            }
            if(ICF_NULL != 
                p_media_sdp->p_int_sdp_stream->sdp_stream.p_encoded_attributes)
            {
                icf_cmn_delete_list(
                    p_glb_pdb,
                    &(p_media_sdp->p_int_sdp_stream->sdp_stream.
                    p_encoded_attributes),
                    ICF_MEM_COMMON);
            }
            if(ICF_NULL !=
                p_media_sdp->p_int_sdp_stream->p_tunnelled_sdp)
            {
                ICF_MEMFREE(p_glb_pdb,
                            p_media_sdp->p_int_sdp_stream->p_tunnelled_sdp,
                            ICF_MEM_COMMON,
                            p_ecode,
                            ret_val)
            }

 

            icf_dbm_ret_mem_to_pool(
                p_glb_pdb, 
                ICF_MEM_SDP,
                p_media_sdp->p_int_sdp_stream,
                p_ecode);
        }
        p_media_sdp = 
           icf_cmn_get_next_node_in_list(p_media_sdp_list, p_media_sdp);
    } /* for (p_media_sdp_list) */

    /* Free the acquired memory for the list node */
    icf_cmn_delete_list(
        p_glb_pdb,
        p_p_media_sdp_info,
        ICF_MEM_COMMON);

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* End function icf_cmn_free_media_sdp_info_list() */

/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_media_sdp_info_list
 **
 ** DESCRIPTION:  The function clones  p_p_src_media_sdp_info to 
 **               p_p_target_media_sdp_info
 **
 *****************************************************************************/
icf_return_t icf_cmn_clone_media_sdp_info_list(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_list_st	                **p_p_target_media_sdp_info,
    INOUT   icf_list_st	                *p_src_media_sdp_info,
    OUT     icf_error_t                 *p_ecode)
{
    /* local variables */	
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_inc_media_sdp_info_st           *p_src_media_sdp = ICF_NULL,*p_target_media_sdp=ICF_NULL;
   
    icf_list_st                         *p_tmp_media_sdp_info_list = p_src_media_sdp_info;

    ICF_FUNCTION_ENTER(p_glb_pdb)
 
    while(ICF_NULL != p_tmp_media_sdp_info_list && ICF_NULL != p_tmp_media_sdp_info_list->p_data)
    {
        p_src_media_sdp = (icf_inc_media_sdp_info_st *)(p_tmp_media_sdp_info_list->p_data);

	ICF_MEMGET( p_glb_pdb,
			sizeof(icf_inc_media_sdp_info_st),
			ICF_MEM_COMMON,
			(p_target_media_sdp),
			ICF_DONOT_RET_ON_FAILURE,
			p_ecode,
			ret_val);
        /*klocwork warning removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb, p_target_media_sdp);

        if(ICF_FAILURE != ret_val)
        {
	    icf_port_memcpy( p_target_media_sdp,
			    p_src_media_sdp,
			    (icf_uint32_t)sizeof(icf_inc_media_sdp_info_st));


	    p_target_media_sdp->p_int_sdp_stream = ICF_NULL;

	    if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
			     p_glb_pdb,
			     (icf_pool_id_t const)ICF_MEM_SDP,
			     (icf_void_t **) &(p_target_media_sdp->p_int_sdp_stream),
			     p_ecode))
            {
                /*Klocwork Warning removal*/
                ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,
                             p_target_media_sdp->p_int_sdp_stream)
                ret_val = ICF_FAILURE;
            }
            else
            {

                icf_port_memcpy( p_target_media_sdp->p_int_sdp_stream,
                                 p_src_media_sdp->p_int_sdp_stream,
                                 (icf_uint32_t)sizeof(icf_internal_sdp_stream_st));

                p_target_media_sdp->p_int_sdp_stream->sdp_stream.p_codec_info_list = ICF_NULL;
ICF_CHECK_TUNNEL_MODE_START(p_glb_pdb,ICF_TRUE)
               p_target_media_sdp->p_int_sdp_stream->p_tunnelled_sdp = ICF_NULL;
               if(ICF_NULL != p_src_media_sdp->p_int_sdp_stream->p_tunnelled_sdp)
               {
                   ICF_MEMGET(
                       p_glb_pdb,
                       ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st),
                       ICF_MEM_COMMON,
                       p_target_media_sdp->p_int_sdp_stream->p_tunnelled_sdp,
                       ICF_DONOT_RET_ON_FAILURE,
                       p_ecode,
                       ret_val)
                   if (p_target_media_sdp->p_int_sdp_stream->p_tunnelled_sdp) 
				   {
				   	icf_port_memcpy( p_target_media_sdp->p_int_sdp_stream->p_tunnelled_sdp,
                                     p_src_media_sdp->p_int_sdp_stream->p_tunnelled_sdp,
                                     (icf_uint32_t)sizeof(icf_tunnelled_sdp_info_st));
				   }
               }
 
ICF_CHECK_TUNNEL_MODE_END
/* Rel 8.1
 * codec attribute list should not be populated if 
 * Tunnel mode is ON
 */
ICF_CHECK_TUNNEL_MODE_START(p_glb_pdb,ICF_FALSE)
               if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(p_glb_pdb,
                  &(p_target_media_sdp->p_int_sdp_stream->sdp_stream.p_codec_info_list),
                   p_src_media_sdp->p_int_sdp_stream->sdp_stream.p_codec_info_list))
               {
                   ret_val = ICF_FAILURE;
               }
ICF_CHECK_TUNNEL_MODE_END
               if(ICF_SUCCESS == ret_val)
               {
                   if(ICF_FAILURE ==  icf_cmn_add_node_to_list(p_glb_pdb,
                                      p_p_target_media_sdp_info,
                                      p_target_media_sdp))
                   {
                       ret_val = ICF_FAILURE;
                   }
               }
               /*Klocwork Warning Removal*/
               ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,
                  p_target_media_sdp->p_int_sdp_stream->
                  sdp_stream.p_codec_info_list)
           }/* End else */

        }/*END if(ICF_FAILURE != ret_val)*/

        if(ICF_FAILURE == ret_val)
        {

            icf_cmn_free_media_sdp_info_list(p_glb_pdb,
                                             p_p_target_media_sdp_info,
                                             p_ecode);
            break;
        }
        p_tmp_media_sdp_info_list = p_tmp_media_sdp_info_list->p_next;
    
    }/* END while(ICF_NULL != p_tmp_media_sdp_info_list->p_data) */
    /*Klocwork Warning Removal*/
    if(ICF_NULL != p_target_media_sdp)
    {
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,
                         p_target_media_sdp->p_int_sdp_stream)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
} /* End function icf_cmn_clone_media_sdp_info_list() */


/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_stream_sdp_info_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_stream_sdp_info_st
 **
 *****************************************************************************/
icf_return_t icf_cmn_free_stream_sdp_info_list(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_internal_list_st	    **p_p_stream_sdp_info,
    OUT     icf_error_t                 *p_ecode)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_internal_list_st                *p_stream_sdp_list = ICF_NULL;
    icf_stream_sdp_info_st				*p_stream_sdp = ICF_NULL;

 
	if (ICF_NULL != *p_p_stream_sdp_info)
	{
		/* Free the acquired memory for the SDP in the nodes of the list */
		p_stream_sdp_list = *(p_p_stream_sdp_info);
		p_stream_sdp = (icf_stream_sdp_info_st *)(p_stream_sdp_list->p_payload);

		while(ICF_NULL != p_stream_sdp)
		{
			if (ICF_NULL != p_stream_sdp->p_applied_sdp)
			{
				icf_cmn_delete_internal_sdp_stream(p_glb_pdb,
								&(p_stream_sdp->p_applied_sdp),
								p_ecode);

				icf_dbm_ret_mem_to_pool(
					p_glb_pdb, 
					ICF_MEM_SDP,
					p_stream_sdp->p_applied_sdp,
					p_ecode);
			}
			if (ICF_NULL != p_stream_sdp->p_local_sdp)
			{
				icf_cmn_delete_internal_sdp_stream(p_glb_pdb,
								&(p_stream_sdp->p_local_sdp),
								p_ecode);

				icf_dbm_ret_mem_to_pool(
					p_glb_pdb, 
					ICF_MEM_SDP,
					p_stream_sdp->p_local_sdp,
					p_ecode);
			}
			if (ICF_NULL != p_stream_sdp->p_last_commited_sdp)
			{
				icf_cmn_delete_internal_sdp_stream(p_glb_pdb,
								&(p_stream_sdp->p_last_commited_sdp),
								p_ecode);

				icf_dbm_ret_mem_to_pool(
					p_glb_pdb, 
					ICF_MEM_SDP,
					p_stream_sdp->p_last_commited_sdp,
					p_ecode);
			}
			if (ICF_NULL != p_stream_sdp->p_remote_received_sdp)
			{
				icf_cmn_delete_internal_sdp_stream(p_glb_pdb,
								&(p_stream_sdp->p_remote_received_sdp),
								p_ecode);

				icf_dbm_ret_mem_to_pool(
					p_glb_pdb, 
					ICF_MEM_SDP,
					p_stream_sdp->p_remote_received_sdp,
					p_ecode);
			}
			if (ICF_NULL != p_stream_sdp->p_offered_sdp)
            {
			    icf_cmn_delete_internal_sdp_stream(p_glb_pdb,
					&(p_stream_sdp->p_offered_sdp),
					p_ecode);

			    icf_dbm_ret_mem_to_pool(
					p_glb_pdb, 
					ICF_MEM_SDP,
					p_stream_sdp->p_offered_sdp,
					p_ecode);
			}
                        /*  Fix Merge for CSR 1-6449001 */
                        /*  Free the memory of the field p_remote_recvd_codec_list */
                        if (ICF_NULL != p_stream_sdp->p_remote_recvd_codec_list)
                        {
                            icf_cmn_delete_codec_attrib_list(p_glb_pdb,
                                        &p_stream_sdp->p_remote_recvd_codec_list,
                                        p_ecode);
                        }

                        /* CSR-1-6976670:If bckup_codec_from_remote & 
                           lastcommitted_codeclist_from_remote is NULL 
                           then delete codec_attrib_list */

                        if (ICF_NULL != p_stream_sdp->p_bckup_codec_from_remote)
                        {
       	                    icf_cmn_delete_codec_attrib_list(p_glb_pdb, 
                                &p_stream_sdp->p_bckup_codec_from_remote,
                                p_ecode);
                        }
                        if (ICF_NULL != p_stream_sdp->p_lastcommitted_codeclist_from_remote)
                        {
       	                    icf_cmn_delete_codec_attrib_list(p_glb_pdb, 
                                &p_stream_sdp->p_lastcommitted_codeclist_from_remote,
                                p_ecode);
                        }
			p_stream_sdp = 
					(icf_stream_sdp_info_st *)
					(icf_cmn_get_next_node_in_internal_list
					(p_stream_sdp_list,p_stream_sdp));

		} /* for (p_stream_sdp_list) */

		/* Free the acquired memory for the list node */
		icf_cmn_delete_internal_list(
			p_glb_pdb,
			p_p_stream_sdp_info,
			ICF_MEM_COMMON);
	}/* End of if (ICF_NULL != *p_p_stream_sdp_info)*/

    return ret_val;

} /* End function icf_cmn_free_stream_sdp_info_list() */



/*******************************************************************************
 * FUNCTION:        icf_cmn_free_stream_capab_list
 *
 * DESCRIPTION:     Function to free list of stream_capab_st.
 *
 ******************************************************************************/

icf_return_t icf_cmn_free_stream_capab_list
(
	INOUT icf_glb_pdb_st         	*p_glb_pdb,
	INOUT icf_internal_list_st 		*p_stream_list
)
{
	icf_internal_list_st	*p_curr_node = ICF_NULL;
	icf_internal_list_st	*p_next_node = ICF_NULL;
	icf_error_t 			ecode;
    icf_return_t 			ret_val = ICF_SUCCESS;

	if (ICF_NULL != p_stream_list)
    {
		p_curr_node = p_stream_list;
		
		while(ICF_NULL != p_curr_node)
		{
			p_next_node = (icf_internal_list_st *)p_curr_node->p_next;

			if(((icf_stream_capab_st*)p_curr_node->p_payload)->bit_mask &\
				ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
			{
				icf_cmn_delete_codec_attrib_list
					(p_glb_pdb,
					&((icf_stream_capab_st*)p_curr_node->p_payload)->\
						p_codec_info_list,
					&ecode);
			
			}	

			if(((icf_stream_capab_st*)p_curr_node->p_payload)->bit_mask &\
				ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
			{
				icf_cmn_delete_list
					(p_glb_pdb, 
					&((icf_stream_capab_st*)p_curr_node->p_payload)->\
						p_encoded_attributes,
					ICF_MEM_COMMON);
			}

			p_curr_node = p_next_node;

		}

		icf_cmn_delete_internal_list(p_glb_pdb, 
									&(p_stream_list),
									ICF_MEM_COMMON);
	}

	return ret_val;

}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_copy_string_list
 *
 * DESCRIPTION:     This function makes a copy of list of type icf_string_st.
 *
 ******************************************************************************/
icf_return_t icf_cmn_copy_string_list
(
	icf_glb_pdb_st	*p_glb_pdb,
	icf_list_st		**p_p_string_list_new,
	icf_list_st		*p_string_list_old,
	icf_error_t     *p_ecode
)
{
	icf_return_t	ret_val=ICF_SUCCESS;

	icf_string_st	*p_string_data = ICF_NULL;

	if (ICF_NULL != *p_p_string_list_new)
	{
		ret_val = icf_cmn_delete_list(p_glb_pdb,
						p_p_string_list_new,
						ICF_MEM_COMMON);
		if (ICF_FAILURE == ret_val)
		{
			return ret_val;
		}
	}

	while (p_string_list_old != NULL)
	{
		p_string_data = ICF_NULL;
		if (p_string_list_old->p_data != NULL)
		{
			
			ICF_MEMGET(
			           p_glb_pdb,
				   ICF_PORT_SIZEOF(icf_string_st),
				   ICF_MEM_COMMON,
				   p_string_data,
				   ICF_DONOT_RET_ON_FAILURE,
				   p_ecode,
				   ret_val
				  )
                        /*klocwork warning removal*/
                        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_string_data);

                        if (ICF_SUCCESS == ret_val && ICF_NULL != p_string_data)
                        { 
			   /*Copying memory of type icf_string_st.*/
			   icf_port_memcpy(
						p_string_data,
						p_string_list_old->p_data,
				        	(icf_uint32_t)sizeof(icf_string_st)
					  );
                        }
                        else
                        {
                           icf_cmn_delete_list(p_glb_pdb,
			                       p_p_string_list_new,
					       ICF_MEM_COMMON);
                           ret_val = ICF_FAILURE;
                           break;                                    
                        }
		}
		ret_val = icf_cmn_add_node_to_list(p_glb_pdb,
							p_p_string_list_new,
							p_string_data);
                if (ICF_FAILURE == ret_val)
                {
                    ICF_MEMFREE(p_glb_pdb,
               		        p_string_data, 
               		        ICF_MEM_COMMON, 
				p_ecode, 
				ret_val)

                    icf_cmn_delete_list(p_glb_pdb,
		                        p_p_string_list_new,
		                        ICF_MEM_COMMON);                 
                    ret_val = ICF_FAILURE;
                    break;   
                }

		p_string_list_old=p_string_list_old->p_next;
	}/* End of while (p_string_list_old != NULL)*/
	return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_cmn_delete_internal_sdp_stream
 **
 ** DESCRIPTION:  This function frees a list of type icf_string_st

  THIS FUNCTION:	will be freeing the memory assigned to each pointer 
  * 				inside the icf_internal_stream_sdp and also within those pointers if 
  * 				any pointers are present those will also be freed and so on.
  *
  *					In icf_internal_ sdp_stream, we don't have any pointer to be freed. 
  *	
  *					Within icf_internal_stream_sdp there is variable of type 
  * 				icf_stream_sdp, we need to go in it and free the memory assigned 
  * 				to pointer p_codec_info_list by calling some function written for it.
  *
  *					And also within icf_sdp_stream, there is pointer 
  * 				p_encoded_parameters which needs to be freed by calling some 
  * 				function written for it.
 **
 *****************************************************************************/

icf_return_t icf_cmn_delete_internal_sdp_stream
(
	icf_glb_pdb_st *p_glb_pdb,
	icf_internal_sdp_stream_st **p_p_internal_sdp_stream,
	icf_error_t                 *p_ecode
)
{
	icf_return_t	ret_val=ICF_SUCCESS;
	icf_sdp_stream_st	*p_sdp_stream= ICF_NULL;

	if(ICF_NULL == *p_p_internal_sdp_stream)
	{
		return ICF_SUCCESS;
	}
						
	p_sdp_stream = &((*p_p_internal_sdp_stream)->sdp_stream);
        if(ICF_NULL != p_sdp_stream)
        {
	/*Deleting codec info list. */
	if (p_sdp_stream->p_codec_info_list != ICF_NULL)
	{
		ret_val=icf_cmn_delete_codec_attrib_list(
			p_glb_pdb,&(p_sdp_stream->p_codec_info_list),p_ecode);
	}

	/*Deleting encoded attribute list from icf_sdp_stream. */
	if (p_sdp_stream->p_encoded_attributes != ICF_NULL)
	{
		ret_val=icf_cmn_delete_list(
									p_glb_pdb,
									&(p_sdp_stream->p_encoded_attributes),
									ICF_MEM_COMMON
								   );
	}
        }

            if(ICF_NULL != (*p_p_internal_sdp_stream)->p_tunnelled_sdp)
            {
                ICF_MEMFREE(p_glb_pdb,
                            (*p_p_internal_sdp_stream)->p_tunnelled_sdp,
                            ICF_MEM_COMMON,
                            p_ecode,
                            ret_val)
            }




	/* Releasing the memory assigned to pointer of 
	 * type icf_internal_sdp_stream.*/
	ICF_MEMFREE
				(
					p_glb_pdb, 
               		(*p_p_internal_sdp_stream), 
               		ICF_MEM_SDP, 
					p_ecode, 
					ret_val
				);
	*p_p_internal_sdp_stream=ICF_NULL;

	return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_node_from_stream_capab_list
 *
 * DESCRIPTION:     This function will return pointer node of list of type
 *					icf_stream_capab_st, if corresponding matching stream type
 *					is found in the list.
******************************************************************************/
icf_list_st* icf_cmn_get_node_from_stream_capab_list
(
	icf_list_st			*p_list_node,
	icf_media_type_t	stream_type
)
{
	icf_stream_capab_st	*p_node_data = ICF_NULL;

	while (ICF_NULL != p_list_node)
	{
		p_node_data = (icf_stream_capab_st *)
								(p_list_node->p_data);

		if (p_node_data->stream_type == stream_type)
		{
			break;
		}
		p_list_node = p_list_node->p_next;
	}

	return p_list_node;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_node_from_stream_data_list
 *
 * DESCRIPTION:     This function will return pointer node of list of type
 *                                      icf_stream_data_st, if corresponding matching stream type
 *                                      is found in the list.
******************************************************************************/
icf_stream_data_st* icf_cmn_get_node_from_stream_data_list
(
        icf_list_st                     *p_list_node,
        icf_media_type_t        stream_type
)
{
        icf_stream_data_st      *p_node_data = ICF_NULL;

        while (ICF_NULL != p_list_node)
        {
                p_node_data = (icf_stream_data_st *)
                                        (p_list_node->p_data);

                if (p_node_data->stream_type == stream_type)
                {
                        break;
                }
                else
                {
                     p_node_data = ICF_NULL;
                }
                p_list_node = p_list_node->p_next;
        }

        return p_node_data;
}


/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_media_sdp_info_internal_list
 **
 ** DESCRIPTION:  This function frees remote_recvd sdp in mmi ctxt
 **
 *****************************************************************************/
icf_return_t icf_cmn_free_media_sdp_info_internal_list
(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_internal_list_st        **p_p_media_sdp_info,
    OUT     icf_error_t                 *p_ecode
)
{
	/* local variables */	
    icf_return_t						ret_val = ICF_SUCCESS;
    icf_internal_list_st				*p_media_sdp_list = ICF_NULL;
    icf_inc_media_sdp_info_st           *p_media_sdp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)
 
    /* Free the acquired memory for the SDP in the nodes of the list */

	p_media_sdp_list = *(p_p_media_sdp_info);

	while(ICF_NULL != p_media_sdp_list)
	{
		p_media_sdp = 
			(icf_inc_media_sdp_info_st *)(p_media_sdp_list->p_payload);

        if (ICF_NULL != p_media_sdp->p_int_sdp_stream)
        {
            if(ICF_NULL != p_media_sdp->p_int_sdp_stream->sdp_stream.
				p_codec_info_list)
            {
                icf_cmn_delete_list(
                    p_glb_pdb,
                    &(p_media_sdp->p_int_sdp_stream->sdp_stream.
							    p_codec_info_list),
                    ICF_MEM_COMMON);
            }

            if(ICF_NULL != p_media_sdp->p_int_sdp_stream->sdp_stream.
				p_encoded_attributes)
            {
                icf_cmn_delete_list(
                    p_glb_pdb,
                    &(p_media_sdp->p_int_sdp_stream->sdp_stream.
							    p_encoded_attributes),
                    ICF_MEM_COMMON);
            }

            if(ICF_NULL != p_media_sdp->p_int_sdp_stream->p_tunnelled_sdp)
            {
                ICF_MEMFREE(p_glb_pdb,
                            p_media_sdp->p_int_sdp_stream->p_tunnelled_sdp,
                            ICF_MEM_COMMON,
                            p_ecode,
                            ret_val)            
            }




            icf_dbm_ret_mem_to_pool(
                p_glb_pdb, 
                ICF_MEM_SDP,
                p_media_sdp->p_int_sdp_stream,
                p_ecode);
        }

		p_media_sdp_list = p_media_sdp_list->p_next;
    } /* for (p_media_sdp_list) */

    /* Free the acquired memory for the list node */
    icf_cmn_delete_internal_list(
        p_glb_pdb,
        p_p_media_sdp_info,
        ICF_MEM_COMMON);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}
/* ICF_6_1_OPTIONS_SUPPORT start */

/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_stream_capab
 **
 ** DESCRIPTION:  This function clones the parameters of icf_stream_capab_st
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_stream_capab
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_stream_capab_st	**p_dest_stream_capab,
	INOUT	icf_stream_capab_st	*p_src_stream_capab
)
{
	icf_stream_capab_st	*p_temp_dest_stream_capab = ICF_NULL;
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_error_t	ecode;

	ICF_FUNCTION_ENTER(p_glb_pdb)
	ICF_MEMGET(
		p_glb_pdb,
		sizeof(icf_stream_capab_st),
		ICF_MEM_COMMON,
		*p_dest_stream_capab,
		ICF_RET_ON_FAILURE,
		&ecode,
		ret_val
		);
	
	p_temp_dest_stream_capab = *p_dest_stream_capab;

	p_temp_dest_stream_capab->bit_mask = 
		p_src_stream_capab->bit_mask;
	
	p_temp_dest_stream_capab->stream_id = 
		p_src_stream_capab->stream_id;
	
	p_temp_dest_stream_capab->stream_type = 
		p_src_stream_capab->stream_type;
	
	p_temp_dest_stream_capab->media_stream_param = 
		p_src_stream_capab->media_stream_param;
	
	if ( ICF_NULL != (p_temp_dest_stream_capab->bit_mask & 
		ICF_STREAM_CAPAB_CODEC_INFO_PRESENT))
	{
		icf_cmn_clone_codec_attrib_list(p_glb_pdb,
			&(p_temp_dest_stream_capab->p_codec_info_list),
			p_src_stream_capab->p_codec_info_list);
		
	}
	
	if ( ICF_NULL != (p_temp_dest_stream_capab->bit_mask & 
		ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT))
	{
		icf_cmn_copy_string_list(p_glb_pdb,
			&(p_temp_dest_stream_capab->p_encoded_attributes),
			p_src_stream_capab->p_encoded_attributes,
			&ecode);
	}

	ICF_FUNCTION_EXIT(p_glb_pdb)
	return ICF_SUCCESS;
}



/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_stream_capab_list
 **
 ** DESCRIPTION:  This function clones the parameters of icf_stream_capab_st
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_stream_capab_list
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_list_st			**p_dest_stream_capab_list,
	INOUT	icf_list_st			*p_src_stream_capab_list
)
{

    icf_list_st			*p_list_ptr = ICF_NULL;
	icf_stream_capab_st	*p_temp_dest_stream_capab = ICF_NULL;
	icf_stream_capab_st	*p_temp_src_stream_capab = ICF_NULL;
	icf_error_t			ecode = ICF_ERROR_UNDEFINED;
	icf_list_st			*p_temp_dest_list = ICF_NULL;
	icf_return_t	ret_val = ICF_SUCCESS;


	if ( ICF_NULL != *p_dest_stream_capab_list)
	{
		return ICF_FAILURE;
	}


    for(p_list_ptr = p_src_stream_capab_list;ICF_NULL != p_list_ptr;
		p_list_ptr = p_list_ptr->p_next)
    {
		
		p_temp_src_stream_capab = (icf_stream_capab_st *)
			(p_list_ptr->p_data);
		
		icf_cmn_clone_stream_capab(p_glb_pdb,
			&p_temp_dest_stream_capab,
			p_temp_src_stream_capab);

		if ( ICF_NULL == *p_dest_stream_capab_list)
		{
			ICF_MEMGET(
				p_glb_pdb,
				ICF_PORT_SIZEOF(icf_list_st),
				ICF_MEM_COMMON,
				*p_dest_stream_capab_list,
				ICF_RET_ON_FAILURE,
				&ecode,
				ret_val
				);
			p_temp_dest_list = *p_dest_stream_capab_list;
		}
		else
		{
			ICF_MEMGET(
				p_glb_pdb,
				ICF_PORT_SIZEOF(icf_list_st),
				ICF_MEM_COMMON,
				p_temp_dest_list->p_next,
				ICF_RET_ON_FAILURE,
				&ecode,
				ret_val
				);
			p_temp_dest_list = p_temp_dest_list->p_next;
		}
		p_temp_dest_list->p_data = (icf_void_t *)p_temp_dest_stream_capab;
		p_temp_dest_list->p_next = ICF_NULL;
	}
	return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_icf_stream_capab_list
 **
 ** DESCRIPTION:  This function frees the parameters of the list of 
 **               items of type icf_stream_capab_st
 **
 *****************************************************************************/
icf_return_t	icf_cmn_free_icf_stream_capab_list
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_list_st			**p_stream_capab_list
)
{

    icf_list_st			*p_list_ptr = ICF_NULL;
	icf_stream_capab_st	*p_temp_src_stream_capab = ICF_NULL;



    for(p_list_ptr = *p_stream_capab_list;ICF_NULL != p_list_ptr;
		p_list_ptr = p_list_ptr->p_next)
    {
		
		p_temp_src_stream_capab = (icf_stream_capab_st *)
			(p_list_ptr->p_data);
		
		if ( ICF_NULL != (p_temp_src_stream_capab->bit_mask & 
			ICF_STREAM_CAPAB_CODEC_INFO_PRESENT))
		{
                icf_cmn_delete_list(
                    p_glb_pdb,
                    &(p_temp_src_stream_capab->p_codec_info_list),
                    ICF_MEM_COMMON);


		}

		if ( ICF_NULL != (p_temp_src_stream_capab->bit_mask & 
			ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT))
		{
                icf_cmn_delete_list(
                    p_glb_pdb,
                    &(p_temp_src_stream_capab->p_encoded_attributes),
                    ICF_MEM_COMMON);
		}
	}
	icf_cmn_delete_list(
		p_glb_pdb,
		p_stream_capab_list,
		ICF_MEM_COMMON);
	*p_stream_capab_list = ICF_NULL;

	return ICF_SUCCESS;
		
}

/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_icf_stream_capab_node
 **
 ** DESCRIPTION:  This function frees the parameters of icf_stream_capab_st
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_cmn_free_icf_stream_capab_node
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_stream_capab_st	**p_p_stream_capab_node
)
{
    icf_return_t        dummy_ret_val = ICF_FAILURE;
    icf_error_t         dummy_ecode;
	icf_stream_capab_st	*p_temp_src_stream_capab = ICF_NULL;

    p_temp_src_stream_capab = *p_p_stream_capab_node;

    if ( ICF_NULL != (p_temp_src_stream_capab->bit_mask & 
                ICF_STREAM_CAPAB_CODEC_INFO_PRESENT))
    {
        icf_cmn_delete_list(
                p_glb_pdb,
                &(p_temp_src_stream_capab->p_codec_info_list),
                ICF_MEM_COMMON);


    }

    if ( ICF_NULL != (p_temp_src_stream_capab->bit_mask & 
                ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT))
    {
        icf_cmn_delete_list(
                p_glb_pdb,
                &(p_temp_src_stream_capab->p_encoded_attributes),
                ICF_MEM_COMMON);
    }
    ICF_MEMFREE(p_glb_pdb, *p_p_stream_capab_node, ICF_MEM_COMMON, 
								&dummy_ecode, dummy_ret_val)
    *p_p_stream_capab_node = ICF_NULL;

    return ICF_SUCCESS;

}

#ifdef ICF_NAT_MEDIA_TRAVERSAL
/******************************************************************************
 *
 * FUNCTION:    icf_cmn_fetch_stream_id_from_nat_context_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_stream_id_t	icf_cmn_fetch_stream_id_from_nat_context_id
(
	icf_uint32_t	context_id
)
{
    icf_uint8_t     stream_id = ICF_NULL;  
    icf_uint16_t     temp_stream_id = ICF_NULL;

    temp_stream_id = context_id & 0x00ffff;
    stream_id = temp_stream_id >> 8;
    return stream_id;
}


/******************************************************************************
 *
 * FUNCTION:    icf_cmn_fetch_call_id_from_nat_context_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_call_id_t	icf_cmn_fetch_call_id_from_nat_context_id
(
	icf_uint32_t	context_id
)
{
    icf_call_id_t     call_id = ICF_NULL;    
    call_id = context_id >> 16;
    return call_id;
}

/******************************************************************************
 *
 * FUNCTION:    icf_cmn_fetch_req_type_from_nat_context_id
 *
 * DESCRIPTION: Function to send the nat request type : RTP/UDP or RTCP from
 *              context id.
 *****************************************************************************/

icf_uint8_t	icf_cmn_fetch_req_type_from_nat_context_id
(
	icf_uint32_t	context_id
)
{
    icf_uint8_t     req_type = ICF_NULL;
    icf_uint16_t    temp_req_type = ICF_NULL;
    temp_req_type = context_id & 0xffff;
    req_type = temp_req_type & 0xff;
    return req_type;
}

#endif


/* ICF_6_1_OPTIONS_SUPPORT end */


/*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19001)*/
/*******************************************************************************
 * FUNCTION:        icf_cmn_free_stream_capab_list_stored_as_icf_list
 *
 * DESCRIPTION:     Function to free list of stream_capab_st.
 *                  This functionn is similar to icf_cmn_free_stream_capab_list
 *                  with only one difference.
 *                  1.icf_cmn_free_stream_capab_list function frees the 
 *                  stream_capab_st stored as icf_internal_list_st 
 *                  2.icf_cmn_free_stream_capab_list_stored_as_icf_list frees
 *                  the stream_capab_st stored as icf_list_st
 *
 ******************************************************************************/

icf_return_t icf_cmn_free_stream_capab_list_stored_as_icf_list
(
	INOUT icf_glb_pdb_st       *p_glb_pdb,
	INOUT icf_list_st 		   *p_src_list
)
{
	icf_list_st			  *p_next_node = ICF_NULL;
	icf_stream_capab_st	  *p_dest_data = ICF_NULL;    
    icf_return_t           ret_val     = ICF_SUCCESS; 
    icf_error_t			   ecode       = ICF_ERROR_UNDEFINED; 

    ICF_FUNCTION_ENTER(p_glb_pdb)

    for( ; p_src_list != ICF_NULL ; p_src_list = p_next_node)
    {
        p_dest_data = (icf_stream_capab_st*)(p_src_list->p_data);
        p_next_node = p_src_list->p_next;
                    
        if (ICF_NULL != p_dest_data)
        { 
            /*Free the codec list if specified by application*/
            if (p_dest_data->bit_mask & 
                      ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
            {
                icf_cmn_delete_codec_attrib_list(
                        p_glb_pdb,
                        &(p_dest_data->p_codec_info_list),
                        &ecode);
            }
            /*Free the list of encoded attributes if specified by
              application*/
            if ( p_dest_data->bit_mask & 
                        ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
            {
                icf_cmn_delete_list(
                               p_glb_pdb,
                               &(p_dest_data->p_encoded_attributes),
                               ICF_MEM_COMMON);
            }
                        
            icf_cmn_memfree(p_glb_pdb,p_dest_data,ICF_MEM_COMMON,
                            &ecode);
        }
                    
        icf_cmn_memfree(p_glb_pdb,p_src_list,ICF_MEM_COMMON,
                        &ecode);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return  ret_val;        
}

