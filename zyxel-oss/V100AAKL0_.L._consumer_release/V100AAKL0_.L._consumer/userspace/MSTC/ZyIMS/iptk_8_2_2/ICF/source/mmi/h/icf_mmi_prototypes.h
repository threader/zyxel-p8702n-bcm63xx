/******************************************************************************
 * 
 * 
 * FILE NAME:   icf_mmi_prototypes.h 
 * 
 * DESCRIPTION: This file contains the prototypes of the functions used 
 *              to implement the functionality of MMI.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Shefali Aggarwal     26-nov-2004        Initial
 *          Kirti Dhingra        29-nov-2005        Added function Prototypes
 *                                                  for QOS Support 
 *          Amit Sharma          09-Mar-2006        ICF merging with ICF2.0
 * 25-Aug-2006      Amit Sharma    SPR 12554
 *                  Umang Singh
 * 
 * 27-Nov-2006  Deepti Goyal    ICF Rel 6.2         NAT Feature support
 * 31-Jan-2008  Abhishek Dhammawat Rel8.0           renamed the function
 *                                              icf_mmi_send_qos_release_for_non_ims_req. 
 * 25-Mar-2008  Tarun Gupta     IPTK Rel 8.0        Merged User Param and 
 *                                                  Dynamic Payload Enhancements 
 *
 * 29-Mar-2008  Alok Tiwari      SPR 18235          Klocwork warning removal
 * 29-Apr-2008  Shruti Thakral   SPR 18455          Klocwork warning removal
 * 29-Mar-2008  Alok Tiwari      SPR 18456          Klocwork warning removal
 * 30-Apr-2008  Shruti Thakral   SPR 18455          Klocwork warning removal
 * 05-Dec-2008  Tarun Gupta      SPR 19218          Merged CSR 1-6367281
 * 09-Feb-2009  Tarun Gupta      Rel 8.1 LLD        SDP Tunnelling Enhancements
 * 27-Feb-2009  Anurag Khare     Rel 8.1            changes for OPTIONS Enhancements
 * 06-Oct-2009 Abhishek Dhammawat SPR 20063       Merged CSR 1-7732501
 * Copyright 2009, Aricent.                                                   *
 ******************************************************************************/

#ifndef __ICF_MMI_PROTOTYPES_H__
#define __ICF_MMI_PROTOTYPES_H__

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */
    
#include "icf_common_types.h"
#include "icf_common_defs.h"
#include "icf_common_interface_types.h"
#include "icf_common_interface_defs.h"
#include "icf_common_prototypes.h"
    
    
icf_return_t icf_mmi_process_mesg(
        INOUT icf_internal_msg_st        *p_internal_msg);

icf_return_t icf_mmi_init(
        INOUT   icf_glb_pdb_st           *p_glb_pdb,
        OUT icf_void_t                   **p_p_glb_data,
        OUT icf_error_t                  *p_ecode);

icf_return_t icf_mmi_api_to_event(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_mrm_event(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_unexpected_event(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_00_cms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_00_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_00_dms_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_00_mms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_00_merge_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);


#ifdef ICF_QOS_SUPPORT
icf_return_t icf_mmi_xx_nm_deact_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_xx_nm_modify_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);
#endif

icf_return_t icf_mmi_01_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_01_gcl_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_02_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_02_cms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_03_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_03_commit_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_03_dms_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_04_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_04_mms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_04_dms_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

#ifdef ICF_QOS_SUPPORT
icf_return_t icf_mmi_04_nm_deact_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_04_nm_modify_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);
#endif

icf_return_t icf_mmi_05_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_05_dms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_05_dms_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_06_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_06_rel_res_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_07_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_07_dms_ind(
       INOUT icf_mmi_pdb_st              *p_mmi_pdb);

icf_return_t icf_mmi_07_merge_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

#ifdef ICF_QOS_SUPPORT
icf_return_t icf_mmi_08_dms_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_08_nm_qos_reserve_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);
#endif

icf_return_t icf_mmi_send_gcl_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_send_mms_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_send_commit_req(
    INOUT icf_mmi_pdb_st                 *p_mmi_pdb);

icf_return_t icf_mmi_send_rel_res_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_send_cms_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_negotiate_codec(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_send_dms_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_send_merge_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_gcl_failure_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_gcl_success_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_commit_success_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_commit_failure_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_cms_success_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_cms_failure_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);
#ifdef ICF_NAT_MEDIA_TRAVERSAL
icf_return_t icf_mmi_10_nat_public_ip_port_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_10_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

#endif


#ifdef ICF_QOS_SUPPORT

icf_return_t icf_mmi_qos_reserve_success_resp(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

icf_return_t icf_mmi_qos_reserve_failure_resp(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);


icf_return_t icf_mmi_send_qos_release_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

#ifdef ICF_NW_ACTIVATE
#ifdef IMS_CLIENT
/******************************************************************************
 *
 * FUNCTION:    icf_mmi_qos_map_sdp_params_to_sec_pdps
 *
 * DESCRIPTION: This function is invoked when the MMI wants 
 *              to convert sdp params to UMTS QOS params. One secondary PDP is 
 *				activated per m-line.
 * PARAMETERS : 
 *				p_media_auth_token - Pointer Authorization Token received in 
 *				P-Media-Authorization-Token
 *              p_icf_local_sdp  - Pointer to Local Sdp structure 
 *              p_icf_remote_sdp  - Pointer to Remote Sdp structure 
 *			    p_icf_remote_sdp -  Pointer to Number of PDP Context 
 *				variable passed from calling module
 *			    icf_nm_pdp_context_st - Pointer to Block of PDP Contexts. 
 *				Allocation done within this function.Must be NULL initially
 *
 *  RETURNS:    ICF_SUCCESS in case of success and ICF_FAILURE in 
 *				case of any failure.In case of Success this function returns
 *				the number of PDP Contexts and the pointer to the starting
 *				block of pdp context array.The memory Allocation is done inside
 *				this function and the calling entity must free this memory once
 *				it is through with the pdp context structure.
 *
 *****************************************************************************/

icf_return_t  icf_mmi_qos_map_sdp_params_to_sec_pdps(
        INOUT   icf_mmi_pdb_st			*p_mmi_pdb,
        INOUT 	icf_nm_pdp_context_st	*p_pdp_ctxts);
#endif
#endif
#endif

icf_return_t icf_mmi_copy_addr(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb,
        OUT   icf_transport_addr_st      *p_source_addr,
        OUT   icf_transport_addr_st      *p_dest_addr);

icf_return_t icf_mmi_call_clear(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

#ifdef ICF_TRACE_ENABLE
icf_void_t icf_mmi_trace(
       IN    icf_mmi_pdb_st              *p_mmi_pdb,
       IN    icf_trace_id_t              trace_id);

icf_return_t icf_mmi_app_trace(
       IN    icf_mmi_pdb_st              *p_mmi_pdb,
       IN    icf_trace_id_t              trace_id);

#endif

#ifdef ICF_ERROR_ENABLE
icf_void_t icf_mmi_ecode(
       IN    icf_mmi_pdb_st              *p_mmi_pdb,
       IN    icf_error_id_t              ecode);
#endif



icf_return_t icf_mmi_init_mmi_ctx(
       INOUT icf_mmi_pdb_st *p_mmi_pdb);


ICF_MM_ROUTING_START
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_proc_icf_imsc_msg
 * 
 * DESCRIPTION:     This function is invoked when an application wants to send
 *					a request to Media Manager and ICF routes it by doing 
 *					api_id conversion.
 *
 *****************************************************************************/
icf_return_t	icf_mmi_proc_icf_imsc_msg(
		INOUT	icf_mmi_pdb_st		*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_proc_imsc_icf_msg
 * 
 * DESCRIPTION:     This function is invoked when Media Manager sends response
 *					or indication to an application and ICF routes it by doing 
 *					api_id and error_cause conversion.
 *
 *****************************************************************************/
icf_return_t	icf_mmi_proc_imsc_icf_msg(
		INOUT	icf_mmi_pdb_st		*p_mmi_pdb);

ICF_MM_ROUTING_END

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_med_cap_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send media capabilities request to MM. 
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_mmi_send_med_cap_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/****************************************************************************** *
 *
 * FUNCTION:        icf_mmi_send_sic_med_cap_resp
 *
 * DESCRIPTION:     This function is invoked whenever media capability response
 *                  is to be sent to SIC. 
 *
 ******************************************************************************/
icf_return_t icf_mmi_send_sic_med_cap_resp(
        INOUT icf_mmi_pdb_st *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_xx_med_cap_request
 *
 * DESCRIPTION:     This function is invoked outsice the fsm when Media 
 *                  Capabilities Request is received from SSA
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t 
icf_mmi_xx_med_cap_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_xx_med_cap_resp
 *
 * DESCRIPTION:     This function is invoked outsice the fsm when Media 
 *                  Capabilities Response is received from MM
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t 
icf_mmi_xx_med_cap_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_validate_media_session_req
 * 
 * DESCRIPTION:     This function verifies the mmi state machine for media
 *					session request
 *
 *****************************************************************************/
icf_return_t	icf_mmi_validate_media_session_req(
		INOUT	icf_mmi_pdb_st		*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_04_pending_req
 *
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting MMS resp from MM and it receives DMS 
 *                  request from CC or media_session_req from app.
 *.
 *                  Current State: IPPSTK_MMI_04_AWT_MMS_RESP
 *                  Current Event: IPPSTK_MMI_DMS_REQ
 *                  
 * RETURNS:         IPPSTK_SUCCESS in case of success and IPPSTK_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_04_pending_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

#ifdef FSS_EMUZED_MEDIA_MANAGER
/******************************************************************************
 *
 * FUNCTION:        icf_send_port_cfg_req_to_mm
 * 
 * DESCRIPTION:     This function is invoked on receiving codec_cfg response from 
 *					Media Manager during ICF init phase. On receiving the response
 *					ICF shall invoke the API to cfg port range with MM
 *
 *****************************************************************************/
icf_return_t	icf_send_port_cfg_req_to_mm(
		INOUT	icf_mmi_pdb_st	*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_send_ringer_tones_req_to_mm
 * 
 * DESCRIPTION:     This function is invoked on receiving 
 *					ICF_CFG_SET_PORT_RANGE_RESP response from 
 *					Media Manager during ICF init phase. On receiving the response
 *					ICF shall invoke the API to ringer tones with MM
 *
 *****************************************************************************/
icf_return_t	icf_send_ringer_tones_req_to_mm(
		INOUT	icf_mmi_pdb_st	*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_handle_init_comp_ind
 * 
 * DESCRIPTION:     This function is invoked when MM init complete is received 
 *					from MM. ICF shall invk the api to configure the codec attrib
 *					at MMI.
 *
 *****************************************************************************/
icf_return_t	icf_mmi_handle_init_comp_ind(
		INOUT	icf_mmi_pdb_st	*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_set_init_complete
 * 
 * DESCRIPTION:     This function is invoked when ringer tones resp is received 
 *					from MM. ICF shall set MM init complete.
 *
 *****************************************************************************/
icf_return_t	icf_mmi_set_init_complete(
		INOUT	icf_mmi_pdb_st	*p_mmi_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_init_mm_cfg_params
 * 
 * DESCRIPTION:     This function is invoked at MMI init. Here we read from 
 *					mm.cfg and save the init params in the mm_init_params str.
 *					in the mmi_data_st 
 *
 *****************************************************************************/
icf_return_t icf_mmi_init_mm_cfg_params(
		INOUT	icf_mmi_data_st	*p_mmi_data,
		INOUT   icf_glb_pdb_st   *p_glb_pdb);
/****************************************************************************
** Function: icf_mmi_read_conf_file
**
****************************************************************************
** DESCRIPTION: This function reads the configuration file and stores
**              in a structure p_file.
**              Comments in the configuration files are not stored.
** 
** 
**
***************************************************************************/
icf_return_t icf_mmi_read_conf_file(
		FILE* fp,
		icf_conf_file_st* p_file);

/****************************************************************************
** Function: icf_mmi_str_trim 
**
****************************************************************************
** DESCRIPTION: This function removes extra TAB/spaces from start and 
**              end of the string. Replaces all tabs by space in the string.
** 
** 
**
***************************************************************************/
icf_void_t icf_mmi_str_trim(
				INOUT icf_uint8_t *str);
#endif

/****************************************************************************
 * Function: icf_mmi_negotiate_codecs_per_stream
 * DESCRIPTION: This function returns success in case any codec matches in
 *				remote SDP and local SDP for a particular stream
 *
 ***************************************************************************/

icf_return_t	icf_mmi_negotiate_codecs_per_stream(
			INOUT	icf_mmi_pdb_st			*p_mmi_pdb,
			IN		icf_stream_id_t			stream_id);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_compare_codecs_value
 *
 * DESCRIPTION: This function returns success in case two codecs match
 *****************************************************************************/

icf_return_t	icf_mmi_compare_codecs_value
(
	icf_mmi_pdb_st			*p_mmi_pdb,
	icf_codec_attrib_st		*p_codec1,
	icf_codec_attrib_st		*p_codec2
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_process_mms_delete_action
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_process_mms_delete_action
(
	icf_mmi_pdb_st		*p_mmi_pdb
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_process_mms_modify_action
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_process_mms_modify_action
(
	icf_mmi_pdb_st *p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_failure_stream_mms_mod_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_mark_failure_stream_mms_mod_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb,
	icf_stream_id_t	stream_id
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_failure_stream_mms_add_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_mark_failure_stream_mms_add_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb, 
	icf_stream_id_t	stream_id
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_failure_stream_cms_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_mark_failure_stream_cms_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb, 
	icf_stream_id_t	stream_id
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_all_stream_mms_add_resp
 *
 * DESCRIPTION: This function marks the status of all streams as failure
 *				in ADD action of mms_resp in MMI ctxt
 *****************************************************************************/

icf_return_t	icf_mmi_mark_all_stream_mms_add_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_mark_all_stream_cms_resp
 *
 * DESCRIPTION: This function marks the status of all streams as failure
 *				in cms_resp in MMI ctxt
 *****************************************************************************/

icf_return_t	icf_mmi_mark_all_stream_cms_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_search_failure_stream
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_search_failure_stream
(
	icf_mmi_pdb_st			*p_mmi_pdb, 
	icf_stream_id_list_st	*p_src,
	icf_stream_id_t			stream_id
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_sdp_for_stream_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_get_sdp_for_stream_id
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type,
	icf_sdp_stream_st	*p_sdp
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_gcl_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_gcl_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_sdp_stream_st	*p_sdp,
	icf_media_type_t	stream_type,
	icf_stream_capab_st	*p_node
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_cms_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_cms_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_media_type_t	stream_type,
	icf_req_stream_st	*p_node
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_check_sdp_for_commited_codecs
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_check_sdp_for_commited_codecs
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_list_st			**p_p_list
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_is_codec_commited
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_is_codec_commited
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_codec_attrib_st	*p_codec
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_mms_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_mms_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_sdp_stream_st	*p_sdp,
	icf_media_type_t	stream_type,
	icf_modify_stream_st*p_node
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_commit_payload
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_make_commit_payload
(
	icf_mmi_pdb_st		*p_mmi_pdb,
	icf_sdp_stream_st	*p_sdp,
	icf_media_type_t	stream_type,
	icf_stream_capab_st *p_node
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clone_sdp_stream_lists
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clone_sdp_stream_lists
(
	INOUT icf_mmi_pdb_st		*p_mmi_pdb,
	IN	icf_sdp_stream_st	*p_src_sdp,
	OUT	icf_sdp_stream_st	*p_dest_sdp
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_delete_sdp_stream_cloned_lists
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_delete_sdp_stream_cloned_lists
(
	INOUT	icf_mmi_pdb_st		*p_mmi_pdb,
	INOUT	icf_sdp_stream_st	*p_sdp
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clone_codec_info_list
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clone_codec_info_list
(
	INOUT icf_mmi_pdb_st		*p_mmi_pdb,
	IN	  icf_list_st			*p_src_list,
	OUT	  icf_list_st			**p_p_dest_list
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clone_enc_attrib_list
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clone_enc_attrib_list
(
	IN	icf_mmi_pdb_st		*p_mmi_pdb,
	IN	icf_list_st			*p_src_list,
	OUT	icf_list_st			**p_p_dest_list
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_mode_for_stream_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_media_mode_t	icf_mmi_get_mode_for_stream_id
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_echo_cancellation
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_boolean_t	icf_mmi_get_echo_cancellation
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_sdp_stream_st	*p_sdp
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_get_stream_type_for_stream_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_media_type_t	icf_mmi_get_stream_type_for_stream_id
(
	icf_mmi_pdb_st			*p_mmi_pdb,
	icf_stream_id_t			stream_id
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_internal_sdp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_trace_internal_sdp(
							  icf_mmi_pdb_st	*p_mmi_pdb,
							  icf_api_id_t		api_id);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_internal_codecs
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_trace_internal_codecs(
							  icf_mmi_pdb_st			*p_mmi_pdb,
							  icf_list_st				*p_codec);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_api_sdp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_trace_api_sdp(
							  icf_mmi_pdb_st	*p_mmi_pdb,
							  icf_api_id_t		api_id);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_trace_api_codecs
 *
 * DESCRIPTION: 
 *****************************************************************************/

 icf_return_t	icf_mmi_trace_api_codecs(
							  icf_mmi_pdb_st			*p_mmi_pdb,
							  icf_list_st				*p_codecs);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_handle_mms_failure_resp
 *
 * DESCRIPTION:     This function is invoked to handle the failure MMS response
 * 					within MMI from MM
 *                  Current State: 	ICF_MMI_04_AWT_MMS_RESP
 *                  EVENT:			ICF_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_result_t icf_mmi_handle_mms_failure_resp
(
 	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_handle_mms_success_resp
 *
 * DESCRIPTION:     This function is invoked to handle the sucessful MMS 
 * 					response within MMI from MM
 *                  Current State: 	ICF_MMI_04_AWT_MMS_RESP
 *                  EVENT:			ICF_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_result_t icf_mmi_handle_mms_success_resp
(
 	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_from_gcl_resp_for_stream_id
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_sdp_from_gcl_resp_for_stream_id
 (
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_media_type_t	stream_type,
	icf_uint8_t			sdp_type,
	icf_list_st			*p_list
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_dms_success_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/
icf_return_t	icf_mmi_dms_success_resp(icf_mmi_pdb_st		*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_dms_failure_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/
icf_return_t	icf_mmi_dms_failure_resp(icf_mmi_pdb_st		*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_open_channel_success_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/
icf_return_t	icf_mmi_open_channel_success_resp(icf_mmi_pdb_st	*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_open_channel_failure_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/
icf_return_t	icf_mmi_open_channel_failure_resp(icf_mmi_pdb_st	*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_open_channel_req
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  send the OPEN_CHANNEL request to the MM.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_open_channel_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_from_cms_resp_for_stream_id
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_sdp_from_cms_resp_for_stream_id
 (
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type,
	icf_list_st			*p_list
);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_for_tbcp_stream
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_sdp_for_tbcp_stream
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type
);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_ctx_from_commit_resp
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_ctx_from_commit_resp
 (
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_list_st			*p_list
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_update_sdp_from_open_channel_resp
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_update_sdp_from_open_channel_resp
(
	icf_mmi_pdb_st	*p_mmi_pdb,
	icf_list_st		*p_src
);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_any_state_dms_ind
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives DMS indication
 *                  from MM
 *                  Current State: Any State
 *                  Current Event: ICF_MMI_DMS_IND
 *                  
 * RETURNS:        ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                   
 *
 ******************************************************************************/

icf_return_t icf_mmi_any_state_dms_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_06_open_channel_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting Open Channel resp from MM and it receives it.
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_06_open_channel_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_copy_remote_recvd_sdp_in_mmi_ctxt
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_copy_remote_recvd_sdp_in_mmi_ctxt
(
	icf_mmi_pdb_st			*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_close_channel_ind
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_close_channel_ind(icf_mmi_pdb_st		*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_err_ind_call_clear
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_err_ind_call_clear
(
	icf_mmi_pdb_st			*p_mmi_pdb
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_clear_buffered_req_resp_in_mmi_ctx
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_clear_buffered_req_resp_in_mmi_ctx
(
	icf_mmi_pdb_st		*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_check_sdp_in_call_ctx
 *
 * DESCRIPTION: 
 *
 *****************************************************************************/

icf_return_t icf_mmi_check_sdp_in_call_ctx
(
	IN		icf_mmi_pdb_st		*p_mmi_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_uint8_t			sdp_type
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_check_api_total_streams
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_check_api_total_streams
(
	IN icf_mmi_pdb_st	*p_mmi_pdb,
	IN icf_api_id_t		api_id
);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_handle_mm_init_comp_ind
 * 
 * DESCRIPTION:     This function is invoked when MM init complete is received 
 *					from MM. ICF shall invk the api to configure the codec attrib
 *					at MMI.
 *
 *****************************************************************************/

icf_return_t	icf_mmi_handle_mm_init_comp_ind(
	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_err_ind_dms_ind
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_err_ind_dms_ind
(
	icf_mmi_pdb_st			*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_delete_media_response
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_delete_media_response
(
	icf_mmi_pdb_st			*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_modify_media_response
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_modify_media_response
(
	icf_mmi_pdb_st			*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_create_media_response
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_create_media_response
(
	icf_mmi_pdb_st			*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_awaited_dms_req
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_awaited_dms_req
(
	icf_mmi_pdb_st			*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_forceful_dms_req
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_forceful_dms_req
(
	icf_mmi_pdb_st			*p_mmi_pdb
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_free_mm_req
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_free_mm_req
(
	IN    icf_mmi_pdb_st	*p_mmi_pdb,
	IN    icf_msg_st		*p_msg_data
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_negotiate_amr_params
 *
 * DESCRIPTION: 
 *
 *****************************************************************************/

icf_return_t icf_mmi_negotiate_amr_params
(
	INOUT      icf_mmi_pdb_st    *p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_fetch_common_mode_set_for_amr_params
 *
 * DESCRIPTION: This function will find the common mode-set value from the
 *              received mode-set and the app-conf mode set for amr params.
 *
 *****************************************************************************/

icf_return_t icf_mmi_fetch_common_mode_set_for_amr_params
(
	INOUT   icf_mmi_pdb_st		*p_mmi_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_list_st			*p_codec_list
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_compare_amr_params_per_codec
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_compare_amr_params_per_codec
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_codec_attrib_st	*p_codec1,
	icf_codec_attrib_st	*p_codec2
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_validate_transport_addr
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_validate_transport_addr
(
	icf_mmi_pdb_st				*p_mmi_pdb, 
	icf_transport_address_st	*p_addr
);

#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_qos_reserve_success_resp
 *
 * DESCRIPTION:     This function is invoked when ever DMS request is to be
 *                  send to the MM.                   
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_qos_reserve_success_resp(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_qos_reserve_failure_resp
 *
 * DESCRIPTION:     This function is invoked when ever DMS request is to be
 *                  send to the MM.                   
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_qos_reserve_failure_resp(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_qos_reserve_req
 *
 * DESCRIPTION:                        
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_qos_reserve_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_qos_release_req
 *
 * DESCRIPTION:                        
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_qos_release_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_qos_release_res_req
 *
 * DESCRIPTION:                        
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_qos_release_res_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_qos_reserve_res_req
 *
 * DESCRIPTION:                       
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_qos_reserve_res_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_sec_pdp_activ_req
 *
 * DESCRIPTION:                       
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_sec_pdp_activ_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_sec_pdp_deactiv_req
 *
 * DESCRIPTION:                        
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_sec_pdp_deactiv_req(
		INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_convert_sdp_m_lines_to_flow_ids
 * 
 * DESCRIPTION:     This function converts SDP M lines to flow ids
 *					It will extract the media parameters from each
 *					node of the media stream list and assign flow
 *					parameters depending on whether media stream is
 *					Sendrecv / Send only / Recv only.
 *					The rule for generating an ip_flow id is :
 *					i) If 2 downlink streams are present, assign 1
 *					to the flow id with lower port number. The 
 *					other is assigned ip_flow_id as 2 .
 *					ii) If two UPLINKS are present, the above rule
 *					hold valid.
 *					iii) If one uplink and one downlink are present,
 *					the downlink stream is given preference and its 
 *					ip_flow_id is assigned 1.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_convert_sdp_m_lines_to_flow_ids(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        IN    icf_internal_list_st   *p_sdp,
        INOUT icf_call_flow_map_st	 *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_traffic_class_per_flow_id
 * 
 * DESCRIPTION:     This function calculates the traffic class for flow ids
 *					already generated. The traffic class depends upon the no
 *					of UPLINKS and DOWNLINKS streams.
 * 					If flow directions (for all media streams) is strictly 
 *					uplink or downlink, it is a STREAMING_TRAFFIC_CLASS.
 * 					If it is both uplink and downlink, then it is a 
 *					CONVERSATIONAL_TRAFFIC_CLASS.
 *
 *                  Note: Interactive class and Background are mainly meant to
 *					be used by traditional Internet applications like WWW, Email,
 *					Telnet, FTP. Telephony applications involving Audio and Video
 *					are delay sensitive and belong to Streaming and Conversational
 *					classes. We will not have Interactive and Background traffic 
 *					class because ICF supports only Real time streams like AUDIO 
 *					and VIDEO.
 *					
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

 icf_return_t icf_mmi_calculate_max_authorised_traffic_class_per_flow_id(
        INOUT icf_mmi_pdb_st			*p_mmi_pdb,
		INOUT icf_call_flow_map_st		*p_call_flow_map);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_traffic_class_per_pdp
 * 
 * DESCRIPTION:     This function calculates the maximum traffic class for
 *					the entire PDP. Max Traffic for PDP is the max traffic
 *					class among all the flow ids.
 *
 *					Priority of different traffic classes is as follows :
 *
 *					Conversational > Streaming > Interactive with 
 *					priority 1 > Interactive with priority 2 > Interactive
 *					with priority 3 > Background
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_calculate_max_authorised_traffic_class_per_pdp(
        IN	  icf_stream_id_t		 num_of_m_lines,
		INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts);
 
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_bit_rate_per_flow_id
 * 
 * DESCRIPTION:     This function calculates the bandwidth per flow id. For
 *					uplink streams, the downlink bandwidth will be 0 while 
 *					for downlink streams, the uplink bandwidth will be 0.
 *					For streams with media mode as sendrecv, the bandwidth
 *					in up/down directions will be equal.
 *					For RTCP flow, the uplink and downlink bandwidth will 
 *					be 5% of the bandwidth available for RTP flows.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_calculate_max_authorised_bit_rate_per_flow_id(
        INOUT icf_mmi_pdb_st			*p_mmi_pdb,
		INOUT icf_stream_flow_map_st	*p_stream_flow_map,
		INOUT icf_call_flow_map_st		*p_call_flow_map);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_calculate_max_authorised_bit_rate_per_pdp
 * 
 * DESCRIPTION:     This function calculates the maximum bit rate per PDP.
 *					Maximum bit rate for uplink / downlink is to be used 
 *					in case the Traffic Class is Interactive or Background.
 *					Guaranteed bit rate for uplink, Guaranteed bit rate for
 * 					downlink is to be used in case the Traffic Class is 
 *					Conversational or Streaming.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_calculate_max_authorised_bit_rate_per_pdp(
		IN	  icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_encode_bit_rate
 * 
 * DESCRIPTION:     This function is used to calculate the encoded bit rate
 *					from the actual bit rate.
 *					
 *					The rule for calculating the bit rate is :
 *					
 *					1) The maximum bit rate is binary coded in 8 bits, using a 
 *					granularity of 1 kbps giving a range of values from 1 kbps
 *					to 63 kbps in 1 kbps increments.
 *					
 *					2) The maximum bit rate is 64 kbps + ((the binary coded value 
 *					in 8 bits -01000000) * 8 kbps) giving a range of values from
 *					64 kbps to 568 kbps in 8 kbps increments.
 *
 *					3) The maximum bit rate is 576 kbps + ((the binary coded value
 *					in 8 bits -10000000) * 64 kbps) giving a range of values from
 *					576 kbps to 8640 kbps in 64 kbps increments.
 *
 *					The above rule can be referenced in document no. 3GPP TS 24.008
 *					under section 10.5.6.5 and table no 10.5.156.
 *                    
 * RETURNS:         Encoded bit rate as calculated from the rule.
 *
 ******************************************************************************/

icf_void_t icf_mmi_encode_bit_rate(icf_uint16_t bit_rate, icf_uint16_t *encoded_val);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_qos_per_pdp
 * 
 * DESCRIPTION:     This function popoulates the QoS information element
 *					for each media stream. It uses pre-defined values for
 *					error ratio and transfer delay for both AUDIO and VIDEO
 *					streams as stated in document no. 3GPP TS 26.236.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_qos_per_pdp(
		IN	  icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_tft_per_pdp
 * 
 * DESCRIPTION:     This function populates the TFT Information element
 *					per PDP for each media stream.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_tft_per_pdp(
		IN    icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        IN 	  icf_nm_auth_token_st   *p_media_auth_token,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts);


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_tft_pkt_filter_list_per_pdp
 * 
 * DESCRIPTION:     This function populates the packet flter list per PDP.
 *					The packet filter list contains a variable number of 
 *					packet filters. Each packet filter is of variable length
 *					and consists of 
 *					-	a packet filter identifier (1 octet);
 *					-	a packet filter evaluation precedence (1 octet);
 *					- 	the length of the packet filter contents (1 octet); and
 *					-	the packet filter contents itself (v octets).
 *					The packet filter identifier field is used to identify each
 *					packet filter in a TFT. The packet filter evaluation 
 *					precedence field is used to specify the precedence for the 
 *					packet filter among all packet filters in all TFTs associated
 *					with this PDP address. Higher the value of the packet filter 
 *					evaluation precedence field, lower is the precedence of that 
 *					packet filter.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_tft_pkt_filter_list_per_pdp(
		IN    icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_populate_tft_parameter_list_per_pdp
 * 
 * DESCRIPTION:     This function is used to fill the parameter list for
					each PDP. Each parameter included in the parameters 
					list is of variable length and consists of:
					-	a parameter identifier (1 octet);
					-	the length of the parameter contents (1 octet); and
					-	the parameter contents itself (v octets).
					The parameter identifier field is used to identify each 
					parameter included in the parameters list and it contains
					the hexadecimal coding of the parameter identifier. 
					
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_populate_tft_parameter_list_per_pdp(
		IN    icf_stream_id_t		 num_of_m_lines,
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        IN 	  icf_nm_auth_token_st   *p_media_auth_token,
		INOUT icf_call_flow_map_st   *p_call_flow_map,
		INOUT icf_stream_flow_map_st *p_stream_flow_map,
		INOUT icf_nm_pdp_context_st  *p_pdp_ctxts);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_qos_map_sdp_params_to_sec_pdps
 *
 * DESCRIPTION: This function is invoked when the MMI wants 
 *              to convert sdp params to UMTS QOS params. One secondary PDP is 
 *				activated per m-line.
 * PARAMETERS : 
 *				p_media_auth_token - Pointer Authorization Token received in 
 *				P-Media-Authorization-Token
 *              p_icf_local_sdp  - Pointer to Local Sdp structure 
 *              p_icf_remote_sdp  - Pointer to Remote Sdp structure 
 *			    p_icf_remote_sdp -  Pointer to Number of PDP Context 
 *				variable passed from calling module
 *			    icf_nm_pdp_context_st - Pointer to Block of PDP Contexts. 
 *				Allocation done within this function.Must be NULL initially
 *
 *  RETURNS:    ICF_SUCCESS in case of success and ICF_FAILURE in 
 *				case of any failure.In case of Success this function returns
 *				the number of PDP Contexts and the pointer to the starting
 *				block of pdp context array.The memory Allocation is done inside
 *				this function and the calling entity must free this memory once
 *				it is through with the pdp context structure.
 *
 *****************************************************************************/

icf_return_t  icf_mmi_qos_map_sdp_params_to_sec_pdps(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb,
        INOUT icf_nm_pdp_context_st  *p_pdp_ctxts);
#endif
#endif
#endif


#ifdef ICF_NAT_MEDIA_TRAVERSAL

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_get_public_ip_port_req
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_get_public_ip_port_req
(
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
    icf_uint8_t         req_type
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_rel_nat_resources
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_rel_nat_resources
(
	icf_mmi_pdb_st		*p_mmi_pdb
);


/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_rel_public_ip_port
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_return_t	icf_mmi_send_rel_public_ip_port
(
	icf_mmi_pdb_st	*p_mmi_pdb,
	icf_stream_id_t	stream_id
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_make_nat_context_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_uint32_t	icf_mmi_make_nat_context_id
( 
	icf_call_id_t		call_id,
	icf_stream_id_t		stream_id,
     icf_media_type_t    stream_type
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_convert_to_nat_addr
 *
 * DESCRIPTION: Function to convert the ICF transport address to 
 *              NAT Address
 **************************************************************************/

icf_return_t icf_mmi_convert_to_nat_addr
(
 icf_transport_address_st       *p_local_addr,
 nat_ip_port_address_st         *p_nat_addr
);


/******************************************************************************
 *
 * FUNCTION:     icf_mmi_convert_nat_addr_to_trans_addr
 * 
 * DESCRIPTION: Function to convert the NAT address to 
 *              ICF Transport Address
 **************************************************************************/

icf_return_t icf_mmi_convert_nat_addr_to_trans_addr
(
 nat_ip_port_address_st         *p_nat_addr,
icf_transport_address_st        *p_public_addr
);

#endif /* #ifdef ICF_NAT_MEDIA_TRAVERSAL */



/************************************************************************
 * NAME:          icf_mmi_send 
 * DESCRIPTION:   Function is used to send message to PA/MM
 * 
 ***********************************************************************/
icf_return_t icf_mmi_send(
        IN   icf_mmi_pdb_st        *p_mmi_pdb,
        INOUT   icf_msg_st *p_msg_data,
        OUT icf_error_t *p_ecode);

#ifdef ICF_MM_REQUEST_BUFFERING
/************************************************************************
 * NAME:          icf_mmi_buffer_msg
 * DESCRIPTION:   Function is used to buffer outgoing MM message
 * 
 ***********************************************************************/
icf_return_t icf_mmi_buffer_msg(IN   icf_mmi_pdb_st  *p_mmi_pdb,
                                IN icf_msg_st *p_msg_data,
                                OUT icf_error_t *p_ecode);

/************************************************************************
 * NAME:          icf_mmi_pop_buffered_msg
 * DESCRIPTION:   Function is used remove a message from the buffered messages  
 * 
 ***********************************************************************/
icf_void_t icf_mmi_pop_buffered_msg(IN  icf_glb_pdb_st   *p_glb_pdb,
                                    INOUT   icf_mmi_data_st  *p_mmi_data,
                                OUT icf_msg_st **p_p_msg_data,
                                OUT icf_error_t *p_ecode);

/************************************************************************
 * NAME:          icf_mmi_check_mm_req_msg
 * DESCRIPTION:   Function checks is the given message is a request message
 *                for MM.
 * 
 ***********************************************************************/
icf_boolean_t icf_mmi_check_mm_req_msg(icf_msg_st *p_msg_data);

/************************************************************************
 * NAME:          icf_mmi_check_mm_req_msg
 * DESCRIPTION:   Function checks is the given message is a response message
 *                from MM.
 * 
 ***********************************************************************/
icf_boolean_t icf_mmi_check_mm_resp_msg(icf_msg_st *p_msg_data);
#endif

/************************************************************************
 * NAME:          icf_mmi_populate_codec_in_applied_sdp
 * DESCRIPTION:   This function copies codecs from codec_for_commit
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_populate_codec_in_applied_sdp(
            icf_mmi_pdb_st      *p_mmi_pdb,
            icf_stream_id_t     stream_id,
            icf_list_st         **p_p_codec_info_list);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_from_mms_resp_for_stream_id
 *
 * DESCRIPTION:                      
 *
 *****************************************************************************/
icf_return_t icf_mmi_update_sdp_from_mms_resp_for_stream_id
(
	icf_mmi_pdb_st		*p_mmi_pdb,
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type,
	icf_uint32_t		bitmask,
	icf_resp_stream_st	*p_stream_node
);

/*****************************************************************************
 ** FUNCTION:     icf_mmi_update_media_params_for_stream_id_in_call_ctxt
 **
 ** DESCRIPTION:  This function clones the parameters of icf_sdp_stream_st
 **				  minus the codec_attribute_st. the codec parameters are
 **	              seperately in another function. The function also fetches
 **				  the node corresponding to the given stream_id from the
 **				  icf_sdp_stream_info_st in the call_ctxt
 **
 *****************************************************************************/
icf_return_t	icf_mmi_update_media_params_for_stream_id_in_call_ctxt
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_uint8_t			sdp_type,
	INOUT	icf_sdp_stream_st	*p_src_sdp
);

/*****************************************************************************
 ** FUNCTION:     icf_mmi_clone_sdp_stream_params
 **
 ** DESCRIPTION:  
 **
 *****************************************************************************/
icf_return_t	icf_mmi_clone_sdp_stream_params
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_sdp_stream_st	*p_dest_sdp_stream,
	INOUT	icf_sdp_stream_st	*p_src_sdp_stream
);

/************************************************************************
 * NAME:          icf_mmi_validate_commited_no_of_codecs
 * DESCRIPTION:   This function validates that the no of codecs present in
 *				  the list sent in is same as that of commited codecs stored 
 *				  in mmi_ctx
 * 
 ***********************************************************************/
icf_boolean_t  icf_mmi_validate_commited_no_of_codecs
(
	icf_mmi_pdb_st      *p_mmi_pdb,
	icf_stream_id_t     stream_id,
    icf_list_st         *p_codec_info_list
);

/************************************************************************
 * NAME:          icf_mmi_unmark_send_mms_in_mms_resp
 * DESCRIPTION:   
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_unmark_send_mms_in_mms_resp
(
	icf_mmi_pdb_st      *p_mmi_pdb,
	icf_stream_id_t     stream_id
);


/******************************************************************************
 *
 * FUNCTION:        icf_send_self_ip_req_to_mm
 * 
 *
 *****************************************************************************/
icf_return_t	icf_send_self_ip_req_to_mm(
	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to
 *                  send the UNEXPECTED EVENT IND to the MM.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

icf_return_t icf_mmi_send_unexpected_msg_ind(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb,
        IN    icf_error_t                *p_ecode);


#ifdef ICF_QOS_SUPPORT
/************************************************************************
 * NAME:          icf_mmi_check_if_qos_stream
 * DESCRIPTION:
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_check_if_qos_stream
(
            icf_mmi_pdb_st      *p_mmi_pdb,
            icf_stream_id_t     stream_id
);

/************************************************************************
 * NAME:          icf_mmi_get_bandwidth_for_flow_id
 * DESCRIPTION:
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_get_bandwidth_for_flow_id
(
            icf_mmi_pdb_st			*p_mmi_pdb,
            icf_stream_flow_map_st	*p_stream_flow_map,
			icf_uint8_t				flow_idx,
			icf_uint16_t			*p_bandwidth
);


/************************************************************************
 * NAME:          icf_mmi_qos_check_av_stream
 * DESCRIPTION:
 * 
 ***********************************************************************/
icf_return_t  icf_mmi_qos_check_av_stream
(
            icf_mmi_pdb_st			*p_mmi_pdb
);
#endif

#ifdef ICF_NAT_MEDIA_TRAVERSAL
/******************************************************************************
 *
 * FUNCTION:    icf_make_nat_context_id
 *
 * DESCRIPTION: This function will make the context id from strream id ,\
 *              stream type and call id.
 *              First 2 bytes will represent call_id,
 *              Third Byte will represent the stream id.
 *              Last byte represent the stream_type.
 *
 *****************************************************************************/

icf_uint32_t	icf_make_nat_context_id
( 
	icf_call_id_t		call_id,
	icf_stream_id_t		stream_id,
    icf_media_type_t    stream_type
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_send_recv_udp_packet_ind
 *
 * DESCRIPTION: This function will send the recv udp packet indication
 *              message to NAT Client Library.
 *
 * RETURN_TYPES:ICF_SUCCESS/ICF_FAILURE
 *
 *****************************************************************************/

icf_return_t	icf_mmi_send_recv_udp_packet_ind
(
	icf_mmi_pdb_st		*p_mmi_pdb
);

#endif

/******************************************************************************
 * FUNCTION:       icf_mmi_handle_local_dtmf_capability
 *
 * DESCRIPTION:    This function can be triggered while making GCL/COMMIT/MMS
 *                 payload. This function will be triggered only when the payload
 *                 being made uses the remote SDP. In such a scenario the request
 *                 payload being sent should contain the local DTMF information.
 *                 If remote DTMF information is already present in SDP then we 
 *                 replace it by local DTMF info. If remote DTMF info is not 
 *                 present then we add the local information onto the payload.
 *
 *****************************************************************************/
icf_return_t    icf_mmi_handle_local_dtmf_capability
(
    INOUT   icf_mmi_pdb_st      *p_mmi_pdb,
    INOUT   icf_sdp_stream_st   *p_rem_sdp
);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_init_sdp
 *
 * DESCRIPTION:     This function is used to initialize the variable of type 
 *                  icf_sdp_stream_st.
 *
 ******************************************************************************/
 icf_void_t icf_mmi_init_sdp(INOUT icf_sdp_stream_st *p_sdp);

/******************************************************************************
 * FUNCTION:       icf_mmi_check_dtmf_codec
 *
 * DESCRIPTION:    This function is triggered when validating commited number
 *                 of codecs upon recving MMS response. This will return success
 *                 if either commited codecs or codecs in MMS response contains
 *                 DTMF codec.
 *****************************************************************************/
icf_void_t    icf_mmi_check_dtmf_codec
(
    IN      icf_mmi_pdb_st          *p_mmi_pdb,
    IN      icf_list_st             *p_codec_list,
    IN      icf_codec_for_commit_st *p_codec_for_commit,
    INOUT   icf_uint8_t             *p_codec_count
);

/*****************************************************************************
 ** FUNCTION:     icf_mmi_free_icf_req_stream_node
 **
 ** DESCRIPTION:  This function frees the parameters of icf_req_stream_st
 **				 
 **
 *****************************************************************************/
icf_return_t    icf_mmi_free_icf_req_stream_node
(
	INOUT	icf_mmi_pdb_st          *p_mmi_pdb,
	INOUT	icf_req_stream_st	**p_p_req_stream_node,
    INOUT	icf_error_t    	    *p_ecode
);

/*****************************************************************************
 ** FUNCTION:     icf_mmi_free_icf_codec_attrib_node
 **
 ** DESCRIPTION:  This function frees the parameters of icf_codec_attrib_st
 **
 *****************************************************************************/
icf_return_t icf_mmi_free_icf_codec_attrib_node
(
	INOUT	icf_glb_pdb_st	    *p_glb_pdb,
	INOUT	icf_codec_attrib_st	**p_p_codec_attrib,
    INOUT	icf_error_t    	    *p_ecode
);

/******************************************************************************
 * FUNCTION:       icf_mmi_check_special_codec
 *
 * DESCRIPTION:    This function is triggered when validating commited number
 *                 of codecs upon recving MMS response. This will return success
 *                 if either commited codecs or codecs in MMS response contains
 *                 DTMF/FEC/RED/CN payload types.
 *****************************************************************************/
icf_void_t    icf_mmi_check_special_codec
(
    IN      icf_mmi_pdb_st          *p_mmi_pdb,
    IN      icf_list_st             *p_codec_list,
    IN      icf_codec_for_commit_st *p_codec_for_commit,
    INOUT   icf_uint8_t             *p_codec_count
);

/* New function prototypes for tunnel mode specific functions */

/******************************************************************************
 * FUNCTION:        icf_mmi_xx_tunnelled_dms_request
 * DESCRIPTION:     This function is invoked when the MMI is in any tunnel
 *                  mode specific state and it receives a tunnelled DMS request.
 *                  Current State: ICF_MMI_00_IDLE/
 *                                 ICF_MMI_11_AWT_TUNNELLED_CMS_RESP/
 *								   ICF_MMI_12_AWT_TUNNELLED_MMS_RESP
 *                  Current Event: ICF_MMI_TUNNELLED_DMS_REQ
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *****************************************************************************/
icf_return_t icf_mmi_xx_tunnelled_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_tunnelled_cms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives a tunnelled CMS request from CC.
 *                  Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_TUNNELLED_CMS_REQ
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_00_tunnelled_cms_request(
    icf_mmi_pdb_st			*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_11_tunnelled_cms_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting tunnelled CMS response from MM and it receives the same.
 *                  Current State: ICF_MMI_11_AWT_TUNNELLED_CMS_RESP
 *                  Current Event: ICF_MMI_TUNNELLED_CMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_11_tunnelled_cms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_tunnelled_cms_resp
 * 
 * DESCRIPTION:     This function sends the tunnelled CMS response received
*                   from the application/MM to CC.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_send_tunnelled_cms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_00_tunnelled_mms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is in
 *                  in IDLE state and it receives a tunnelled MMS request from CC.
 *                  Current State: ICF_MMI_00_IDLE
 *                  Current Event: ICF_MMI_TUNNELLED_MMS_REQ
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_00_tunnelled_mms_request(
    icf_mmi_pdb_st			*p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_12_tunnelled_mms_resp
 * 
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting tunnelled MMS response from MM and it receives the same.
 *                  Current State: ICF_MMI_12_AWT_TUNNELLED_MMS_RESP
 *                  Current Event: ICF_MMI_TUNNELLED_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_12_tunnelled_mms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_send_tunnelled_mms_resp
 * 
 * DESCRIPTION:     This function sends the tunnelled MMS response received
*                   from the application/MM to CC.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_send_tunnelled_mms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb);



/****************************************************************************** *
 *
 * FUNCTION:        icf_mmi_send_sic_tunnelled_med_cap_resp
 *
 * DESCRIPTION:     This function is invoked whenever tunnelled media capability 
 *                  response is to be sent to SIC.
 *                  THis function copies tunnelled_sdp in 
 *                  internal message and send it to SIC when Tunnel mode is ON
 *
 ******************************************************************************/
icf_return_t icf_mmi_send_sic_tunnelled_med_cap_resp(
        INOUT icf_mmi_pdb_st *p_mmi_pdb);


#ifdef __cplusplus
}


#endif /* __cplusplus */
#endif /* __ICF_MMI_PROTOTYPES_H__ */



