/******************************************************************************
 * 
 * 
 * FILE NAME:   icf_mmi_types.h 
 * 
 * DESCRIPTION: This file contains the data types used to implement the 
 *              functionality of MMI.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Shefali Aggarwal     26-nov-2004        Initial
 * 
 *          Deepti Goyal         4-Dec-2006         NAT Feature Support
 *
 * Copyright 2006, Aricent.         
 ****************************************************************************/
#ifndef __ICF_MMI_TYPES_H__
#define __ICF_MMI_TYPES_H__

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */


#include "icf_common_types.h"
#include "icf_mmi_defs.h"
#include "icf_mm_types.h"

#ifdef ICF_NAT_MEDIA_TRAVERSAL
#include "nat_types.h"
#include "nat_api.h"
#endif

/*-----------------------------------------------------------------------------
 * 
 * Structure Name:  icf_mmi_pdb_st
 * 
 * Description:     This is the loacl pdb of MMI.
 * 
 * ---------------------------------------------------------------------------*/

typedef struct
{
    /* pointer to internal msg */
    icf_internal_msg_st          *p_internal_msg;
    
    /* Pointer to call context */
    icf_call_ctx_st              *p_call_ctx;
    
    /* pointer to MMI context */
    icf_mmi_context_st           *p_mmi_ctx; 
    
    /* pointer to global pdb */
    icf_glb_pdb_st               *p_glb_pdb;
    
    /*Pointer to ecode */ 
    icf_error_t                  *p_ecode;
    
    /* Pointer to msg being send or received. This is for traces only */
    icf_void_t                   *p_mmi_msg_payload;

    icf_api_header_st            hdr;
    
	icf_stream_id_t				stream_id;

	icf_media_type_t			stream_type;

    /* For trace */
    icf_codec_attrib_st         codec;

    /* For trace */
    icf_uint8_t                  num_of_codecs;

	icf_transport_address_st	stream_addr;

	icf_media_mode_t			mode;

/* This bitmask is set when we need to Send CLOSE_CHANNEL for all streams */
#define ICF_MMI_PDB_ALL_STREAMS			0x01
/* This bitmask is set when we need to Send REL_RES_IND for all streams marked success in cms_resp except TBCP
 * and Send CLOSE_CHANNEL for success streams in cms_resp in MMI ctxt */
#define ICF_MMI_PDB_CMS_FAILURE			0x02
	icf_uint8_t					bitmask;

#ifdef ICF_TRACE_ENABLE
        icf_codec_attrib_st             *p_codec_info;
    icf_result_t            result;
        icf_list_st                     *p_codec_list;
    icf_transport_address_st    remote_stream_addr;
#endif

}icf_mmi_pdb_st;


/*-----------------------------------------------------------------------------
 *
 * Name : icf_mm_init_params_st
 *
 * Description  : This structure stores the init params for MM in MMI.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	icf_uint8_t		amr_mode;

	/* 1 Octet Aligned 
	   2 Bandwidth Efficient
	 */
	icf_uint8_t		codec_mode;

	/*  1- CRC enabled
	    0- Not enabled
	 */
	icf_boolean_t	crc_enabled;

	/*  1- APP_RTCP_PORT_MUX
	    0- APP_RTCP_PORT_NOT_MUX
	 */
	icf_boolean_t	app_rtcp_port_mux;
	/* packetization period would be read from mm.cfg, as it is specific to 
	 * MM. This will be saved to cfg params during scalable param init
	 */
	icf_uint8_t		pkt_period;

	icf_uint16_t		rtp_start_port;

	icf_uint16_t		rtp_end_port;

	icf_uint16_t		app_data_start_port;

	icf_uint16_t		app_data_end_port;

}icf_mm_init_params_st;



typedef icf_return_t  (*icf_mmi_fsm_func_ptr_t) (
      INOUT icf_mmi_pdb_st      *p_mmi_pdb );


/*-----------------------------------------------------------------------------
 *
 * Name : icf_mm_msg_node_st
 *
 * Description  : This structure defines a node of the buffered msgs list
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_mm_msg_node_st
{
	icf_msg_st                  *p_msg;
    struct icf_mm_msg_node_st   *p_next;
} icf_mm_msg_node_st;
      

/*-----------------------------------------------------------------------------
 *
 * Name : icf_mmi_data_st
 *
 * Description  : This structure defines the global data block of MMI.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
	icf_mm_init_params_st		mm_init_params;

	icf_boolean_t				init_complete;
		
    icf_boolean_t               mm_resp_awaited;

    icf_mm_msg_node_st          *p_mm_msg_buf;

    icf_mmi_fsm_func_ptr_t 
        mmi_fsm_table[ICF_MMI_MAX_STATES][ICF_MMI_MAX_EVENTS];
} icf_mmi_data_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_stream_status_st
 * Description :   
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
	icf_stream_id_t		stream_id;

	icf_media_type_t 	stream_type;

	icf_uint16_t			bitmask;

	icf_boolean_t		status_success;

	icf_boolean_t		send_mms;

}icf_mmi_stream_status_st;
/* ICF REl 6.0: multiple m lines */
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_internal_mms_resp_st
 * Description :  Internal message structure to be populated by MMI, when 
 * 				  responding to the MMS response. MMI buffers this response in 
 * 				  its context and sends the response once all the MMS actions
 * 				  are complete. 
 *---------------------------------------------------------------------------*/
typedef struct
{
	/* determines the response for the modified streams */
	icf_result_t				result;
	icf_error_t				error_cause;
	icf_uint8_t				bit_mask;
#define ICF_MMI_MMS_ACTION_MODIFY_RESP	0x1
#define ICF_MMI_MMS_ACTION_ADD_RESP		0x2
	icf_stream_id_t				mod_stream_count;
	icf_mmi_stream_status_st	modified_stream_resp[ICF_MAX_STREAM_PER_CALL];
	
	/* determines the status of the added streams. In the current release
	 * onT.38 stream addition is allowed mid-call */
	icf_stream_id_t				add_stream_count;
	icf_mmi_stream_status_st	added_stream_resp[ICF_MAX_STREAM_PER_CALL];
} icf_mmi_internal_mms_resp_st;


/* ICF REl 6.0: multiple m lines */
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_internal_cms_resp_st
 * Description :  Internal message structure to respond to the CMS request 
 * 				  from MMI to CC. MMI buffers this response in its ctxt and 
 * 				  sends a response to CC, after GCL, open_channel, get_public_ip
 * 				  and CMS.
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
	icf_result_t				result;
	/* the error cause to be filled if the result if failure */
	icf_error_t					error_cause;
	
	/* for each stream MMI will indicate if the stream is successful or not */
	icf_stream_id_t				count;
	icf_mmi_stream_status_st	stream_status[ICF_MAX_STREAM_PER_CALL];
}icf_mmi_internal_cms_resp_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_MMI_TYPES_H__ */



