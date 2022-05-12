/******************************************************************************
 *
 *
 * FILE NAME:   icf_sic_types.h
 *
 * DESCRIPTION: This file contains the structures used in SIC.
 *
 * REVISION HISTORY:
 *          Name            Date            Reference           Comments
 *      Syed Ali Rizvi  11-Aug-2005       LLD                  Initial
 *
 * Copyright (c) 2006, Aricent.
 ******************************************************************************/

#ifndef __ICF_SIC_TYPES_H__
#define __ICF_SIC_TYPES_H__

#include "icf_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_common_defs.h"
#include "icf_sic_defs.h"
#include "icf_macro.h"
/*#include "icf_port.h"*/


#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_pdb_st
 *
 * Description:     This is the loacl pdb of SIC 
 * 
 * ---------------------------------------------------------------------------*/
typedef struct
{
    /* The function to validate the event received uses this field to return 
     * the validity status of the event
     */
    icf_boolean_t        valid_event;

	/* New Code */
	icf_boolean_t		best_effort_unsub;
	
    /* This is the notify resp to be send to SSA. Success or failure */       
     icf_result_t	    notify_resp;
    
    /* current event received */
    icf_uint8_t          event;

    icf_uint32_t          error_cause;
    
    /* This is the subscription status returned in the resposne to MWI or REG.
     * This is filled in by the macro ICF_SIC_SET_CURR_STATUS and is used
     * by the function sending the response to MWI or REG.
     */
    icf_uint8_t          curr_subs_status;
    
    /* Event package received */ 
    icf_string_st	    event_package;

    /* For traces */
    icf_line_id_t	    line_id;
    
    /* This is the duration of which the timer (retry timer or duration timer)
     * are to be started. This is filled in before calling the function to
     * start the timer
     */
    icf_duration_t       timer_duration;

    /* Pointer to the msg received */
    icf_void_t	    *p_msg; 
    
    /* pointer to the error code */
    icf_error_t          *p_ecode;

	icf_uint32_t          resp_code;
    
    /* pointer to the internal msg */
    icf_internal_msg_st  *p_internal_msg;
        
    /* pointer to the lobal pdb */
    icf_glb_pdb_st       *p_glb_pdb;

    /* Pointer to the config data */
    icf_config_data_st   *p_config_data;
    
    /* pointer to the SIC ctx */
    icf_sic_ctx_st      *p_sic_ctx;

    /* Pointer to the destination address */
    icf_address_st       dest_addr;

	icf_boolean_t        send_htb_list;

    /* Pointers to the line data */
    icf_line_data_st     *p_line_data[ICF_MAX_NUM_OF_LINES];

	 /* Used whenever a list is to be passed to a utility function */
    icf_list_st          *p_list_1;

    /* This will be used for containing pointer of type
    * icf_stream_sdp_info_st
    */
    icf_stream_sdp_info_st	*p_stream_sdp_info;

    /* The following is a void pointer to be used whenever any pointers
     * need to be passed to a utility function
     */
     icf_void_t			*p_generic_ptr_1;
     /* this boolean is true if option is send within the dialog */
     icf_boolean_t      is_option_in_dlg;

    icf_uint8_t      subs_state;
} icf_sic_pdb_st;


typedef icf_return_t  (*icf_sic_fsm_func_ptr_t) (
      INOUT icf_sic_pdb_st      *p_sic_pdb );

/*-----------------------------------------------------------------------------
 *
 * Name : icf_sic_data_st
 *
 * Description  : This structure defines the global data block of SIC.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_sic_fsm_func_ptr_t
        sic_fsm_table[ICF_SIC_MAX_STATES][ICF_SIC_MAX_EVENTS];
   /* ICF_6_1_OPTIONS_SUPPORT_START */
    icf_sic_fsm_func_ptr_t
	sic_options_fsm_table[ICF_SIC_MAX_OPTIONS_STATES][ICF_SIC_MAX_OPTIONS_EVENTS];
    /* ICF_6_1-OPTIONS_SUPPORT_END */

} icf_sic_data_st;

/* ICF_6_1_OPTIONS_SUPPORT start */
/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_sic_mm_stream_capab_st
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

typedef struct
{
#define ICF_SIC_STREAM_CAPAB_CODEC_INFO_PRESENT             0x01
#define ICF_SIC_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT     0x02
#define ICF_SIC_STREAM_CAPAB_ENC_ATTRIB_PRESENT             0x04
#define ICF_SIC_STREAM_RTCP_INFO_PRESENT		    0x08


    icf_uint8_t			bit_mask;

	icf_stream_id_t		stream_id;
	icf_media_type_t 	stream_type;

    /* IP address and port number of the RTP session. Incase of the application
	 * stream, this will store the TBCP IP/Port */
	/* When this structure is used to store the local SDP stream information
	 * this will contain the public IP/port as retrieved from STUN (by MMI).
	 * this information will be used by SSA to populate the outgoing SDP on the
	 * n/w. Incase ICF is not behind the NAT, this address will be the same as
	 * the local IP/port */
    icf_transport_address_st   stream_address;    
    
    /* need to define the media stream protocol can be UDP/TCP/RTP/AVP */
	icf_media_transport_mode_et		trans_mode;

	/* peer RTCP address. This information is optional and will be updated
	 * when the peer's RTCP port/IP is received 
	 */
    icf_transport_address_st    rtcp_address;    


	/* Linked list of node of type  icf_codec_attrib_st.
	 * Each node contains codec specific attributes.*/
    icf_list_st			*p_codec_info_list;
    union icf_media_stream_param_ut   media_stream_param;
    icf_list_st         *p_encoded_attributes;
} icf_sic_mm_stream_capab_st;
/* ICF_6_1_OPTIONS_SUPPORT end */


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SIC_TYPES_H__ */

