/*------------------------------------------------------------------------------
 *
 * File name        : icf_common_interface_types.h
 *
 * Purpose          : This file contains all the common structures
 *                    that are required for message exchange between
 *                    different internal modules
 *
 * Revision History :
 *
 *   Date           Name           Ref#           Comments
 * --------      ------------      ----          ---------------
 * 21-Dec-2004   R. Ezhirpavai    ICF LLD     Initial Version.
 * 31-Jan-2005   Jalaj Negi                      Added RGM - SSA Interface
 *                                               Structures
 * 27-Apr-2005   Umang Singh              	 Modified the SSA RNW interface
 *	                                         for incoming REFER
 * 02-Mar-2005   Jalaj Negi       SPR 7477       Added #define for DELETE
 *                                               Indication in registration
 *                                               context.
 * 08-Jun-2005   Jalaj Negi       IMS CLIENT     Added structures for IMS
 *                                               CLIENT
 * 10-Aug-2005   Ashutos Rath     SIGCOMP LLD    Added Interface related to
 *                                               RGM-SIGSA
 * 17-Aug-2005	Jyoti Narula	  ICF LLD		 Added new members for rgm_context_st
 * 10-SEP-2005  Ram Narayan                      Changed the bitmask from  8 bit 
 *                                               to 32 bit in icf_rgm_app_ctxt_st
 * 07-Dec-2005  Rohit Aggarwal    ICF 4.0 CDD    Code changes for IPSec support
 *
 * 28-Dec-2005	Jyoti Narula	SPR 9068: fix	Changed the type for reginfo version: 
 *												Negative 1 is its invalid value.
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 01-Mar-2006     Amit Sharma                  ICF merging with ICF2.0
 * 08-MAR-2006	Aman Aggarwal					Merging ICF2.0 with ICF5.0
 * 03-Jan-2007  Amit Mahajan                    INFO confirmation changes. 
 * 09-Jul-2007  Amit Sharma     ICF 7.0         Added changes for 7.0 enhancements
 * 10-Aug-2007 Manoneet Khosla	SPR # 15721 
 * 7-Sep-2007   Deepti Goyal    SPR 16019       Registration-DeRegistration
 *                                              handling changes.
 * 21-Jan-2008  Rajiv Kumar     ICF 8.0         Registration Procedure
 *                                              Enhancements
 * 24-Jan-2008  Tarun Gupta    IPTK Rel 8.0 LLD Added new structure definitions
 *                                              for AKA challenge related handling
 * 26-Jan-2008  Amit Mahajan    ICF Rel 8.0     Changes for reg event package support
 * 10-Mar-2008  Divij Agarwal   SPR 18122       Support for async AKA negotiation
 * 19-Mar-2008  Sumant Gupta    ICF 8.0         Merging of Publish & Message
 * 01-Apr-2008  Divij Agarwal   SPR 18345       De-reg with 401 challenge support
 *                                              in NAT
 * 03-May-2008  Abhishek Dhammawat SPR18430     Added detach_sic_ctx flag to 
 *                                              sic_ctx structure
 * 01-Dec-2008  Rajiv Kumar        SPR 19215    CSR-1-6212448 Merged
 * 06-Feb-2009  Tarun Gupta    Rel 8.1 LLD      SDP Tunnelling Enhancements
 * 06-Feb-2009  Anurag Khare     Rel 8.1.0      enhancements in sic_ctx,
 *                                              icf_nw_inc_opt_ind,
 *                                              & new structure 
 *                                              icf_sic_early_session_cfm 
 *                                              introduced
 * 16-Feb-2009 Anurag Khare      Rel 8.1        Forking related call
 *                                              modify enhancements
 * 18-Mar-2009 Alok Tiwari       Rel 8.1        New fields added in structure
 *                                              icf_rgm_context_st.
 * 27-Mar-2009 Anuradha Gupta    Rel 8.2        New fields added in structure
 *                                              icf_rgm_context_st.
 * 30-Mar-2009 Anurag Khare      Rel 8.2        changes doen for Prack Enh
 * 30-Mar-2009 Tarun Gupta       Rel 8.2        Refer/Notify Enhancements
 * 31-Mar-2009 Alok Tiwari       Rel 8.2        New members added in structure
 *                                              icf_rgm_context_st for Security
 *                                              Association.
 * 28-May-2009 Tarun Gupta       Rel 8.2        Async NOTIFY Support
 * 31-May-2009 Preksha           Rel 8.2        Asynchronus Message enh
 * 18-Jun-2009 Rajiv Kumar       Rel 8.2        Fix For SPR 19494
 * 03-Aug-2009 Tarun Gupta       SPR 19886      Merged CSR 1-7193955
 * 08-Sep-2009 Anuradha Gupta    SPR 19999      Merged CSR 1-7533733
 * 12-Sep-2009 Rajiv Kumar       SPR 19999      Merged CSR 1-7467002 
 * 16-Sep-2009 Rajiv Kumar       SPR 19999      Merged CSR 1-7615966
 * 16-Oct-2009 Abhishek Dhammawat SPR 20026     Added indicator_1 val in regm
 *                                              ctx for associated URI recv in
 *                                              NOTIFY
 * 25-May-2010 Preksha           SPR 20412      Merged CSR 1-8389294
 * 29-May-2010 Alok Tiwari       SPR 20412      Merged CSR 1-8223807
 *
 * Copyright (c) 2010, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_COMMON_INTERFACE_TYPES_H__
#define __ICF_COMMON_INTERFACE_TYPES_H__

#ifdef IMS_CLIENT
/*#include "stack_headers.h"*/
/*#include "uatk_headers.h"*/
#endif

#include "icf_common_types.h"
#include "icf_common_interface_defs.h"

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

/* ----------------------------------------------------------------------------
 * Type Name   : icf_mmi_reason_t
 * Decription  : Typedef to get the reason for failure 
 * --------------------------------------------------------------------------*/
typedef icf_uint8_t  icf_mmi_reason_t;

/*typedef icf_uint32_t	icf_time_t;*/

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_cfg_open_nw_server_st
 * Description :  This API is sent by CFG to SSA to open the network servers
 *              SSA requests ES to open UDP/TCP/Both sockets 
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_transport_mode_t mode;
    icf_line_id_t        line_id;
}icf_cfg_open_nw_server_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_cfg_close_nw_server_st
 * Description :  This API is sent by CFG to SSA to close the network servers
 *              SSA requests ES to open UDP/TCP/Both sockets
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_line_id_t        line_id;
}icf_cfg_close_nw_server_st;


/*-----------------------------------------------------------------------------
 *
 *  Structure Name : icf_cfg_pcscf_return_val_st
 *
 *  Description :    store return value of api for proxy addr
 *
 *---------------------------------------------------------------------------*/

typedef struct
{
	icf_init_param_st		*p_init_param;
	icf_boolean_t			pcscf_return;

}icf_cfg_pcscf_return_val_st;

/* ICF REl 6.0: multiple m lines */
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_mms_request_st
 * Description :  Internal message structure to be populated when a Modify 
 *                Media Session Request request is sent to MMI
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* ICF REL 6.0: multiple m line
	 * THe structure is being modified for ICF release 6.0 */
	/* this structure will contain a list of stream_id's to be deleted or
	 * modified or added (mid-call- in this release only T.38 addition is 
	 * allowed). For streams that need to be modified, CC also marks the
	 * actions to be taken by MMI, so that MMI can work accordingly. */
	icf_uint8_t						bit_mask;
#define ICF_MMI_MMS_ACTION_DELETE		0x1
#define ICF_MMI_MMS_ACTION_MODIFY		0x2
#define ICF_MMI_MMS_ACTION_ADD			0x4
	icf_stream_id_list_st			*p_delete_media;
	icf_uint8_t						modify_stream_count;
	icf_mms_modify_media_info_st	modify_media[ICF_MAX_STREAM_PER_CALL];
	icf_stream_id_list_st			*p_add_media;
}icf_mmi_mms_request_st;

/* ICF REl 6.0: multiple m lines */
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_mms_response_st
 * Description :  Internal message structure to be populated by MMI, when 
 * 				  responding to the MMS response. MMI buffers this response in 
 * 				  its context and sends the response once all the MMS actions
 * 				  are complete. 
 *---------------------------------------------------------------------------*/
typedef struct
{
	icf_result_t			result;
	icf_error_t				error_cause;
	icf_uint8_t				bit_mask;
#define ICF_MMI_MMS_ACTION_MODIFY_RESP	0x1
#define ICF_MMI_MMS_ACTION_ADD_RESP		0x2
    /* The following bit will be set by MMI whenever it receives
       a local SDP from the application through
       ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP API
    */
#define ICF_MMI_MMS_ACTION_LOCAL_SDP_RCVD 0x04
	
	/* determines the response for the modified streams */
	icf_uint8_t				mod_stream_count;
	icf_stream_status_st	modified_stream_resp[ICF_MAX_STREAM_PER_CALL];
	
	/* determines the status of the added streams. In the current release
	 * onT.38 stream addition is allowed mid-call */
	icf_uint8_t				add_stream_count;
	icf_stream_status_st	added_stream_resp[ICF_MAX_STREAM_PER_CALL];
} icf_mmi_mms_resp_st;


/* ICF REl 6.0: multiple m lines */
/* this new strucutre has been defined for the CMS request from CC to MMI */
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_cms_request_st
 * Description :  Internal message structure to respond to the CMS request 
 * 				  from MMI to CC. MMI buffers this response in its ctxt and 
 * 				  sends a response to CC, after GCL, open_channel, get_public_ip
 * 				  and CMS.
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
	icf_result_t			result;
	/* the error cause to be filled if the result if failure */
	icf_error_t				error_cause;
	
	/* for each stream MMI will indicate if the stream is successful or not */
	icf_uint8_t				count;
	icf_stream_status_st	stream_status[ICF_MAX_STREAM_PER_CALL];
}icf_mmi_cms_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_dms_request_st
 * Description :  Internal message structure to be populated when a Delete 
 *                Media Session Request request is sent to MMI
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /*
     * Flag to identify whether this DMS is a forceful one or not
     */
    icf_boolean_t			forceful_delete;

}icf_mmi_dms_request_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mmi_merge_request_st
 * Description :  Internal message structure to be populated when a
 *                merge request is sent to MMI
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

   /* First call id would be the one store in glb pdb. So
    * only second call id is given. */

   /*
    *  Identifier of second call which needs to be merged 
    */ 
   icf_call_id_t 			merge_call_id_2;

   /* ICF rel 6.0: multiple m lines */
   /* these will be only audio streams, so no need for the stream_type here */
   icf_uint8_t				count;
   icf_merge_stream_id_st 	merge_stream[ICF_MAX_STREAM_PER_CALL];

}icf_mmi_merge_request_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_mm_err_ind_st
 * Description :  Internal message structure to be populated when a
 *                media error indication to be sent from MMI
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   /*Contains the reason of media failure*/
   icf_mmi_reason_t     reason;
} icf_mm_err_ind_st;

/* ####################### SSA Interface typedefs ######################## */

/* ----------------------------------------------------------------------------
 * Type Name   : icf_ssa_reason_t
 * Decription  : Typedef to get the reason for failure 
 * --------------------------------------------------------------------------*/
typedef icf_error_t  icf_ssa_reason_t;



/*------------------------------------------------------------------------------
*
* Name : icf_nw_inc_reject_st
*
* Description  : This structure defines the intternal message data when 
*                 NW reject the update request
*
*-----------------------------------------------------------------------------*/
typedef struct
{
  icf_ssa_reason_t reason_code;
} icf_nw_inc_reject_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_notify_st 
*
* Description  : This structure defines the internal message data when
*               SLP intends to send a NOTIFY to peer.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    /*if FALSE, SSA will send 503 in fragmented SIP msg body in the 
     * Notify, else will send a 200 OK*/
    icf_boolean_t        status;
} icf_nw_otg_notify_st ;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_media_change_req_st
*
* Description  : This structure defines the internal message data when
*                 SC sends reinvite request to SSA to send INVITE req on NW.
*                 Mode is picked from CallCtxt in case request is for resume
*
*-----------------------------------------------------------------------------*/
typedef struct
{
#define ICF_MESG_TYPE_PRESENT   1
#define ICF_TO_TAG_PRESENT      0x02
    icf_uint8_t     bitmask;
    /*
     * If the following flag is true, SSA will pick from config data
     * to decide what type of SDP is to be sent in UPDATE/Re-Invite is
     * to be sent to peer.  If false, then SSA will pick the SDP data
     * from call context
     */
    icf_boolean_t  	call_hold;

	/* this field indicates that CC wants to send either Re-Invite or Update
	 * to the n/w. Currently we need to send Re-Invite for T.38, in future
	 * we may need to specify the mid-call req type based on some service, hence
	 * this enum. This value will take precedence over the configuration
	 * by the application */
#define ICF_NW_OTG_SEND_UPDATE		1
#define ICF_NW_OTG_SEND_RE_INVITE	2
    icf_uint8_t          mesg_type;	 

/* THis field shall be used by SSA 
 * to identify that SSA ctxt need to be recognized
 * for the to tag or not.
 */
    icf_uint8_t         *p_to_tag;
} icf_nw_otg_media_change_req_st;

#ifdef ICF_SESSION_TIMER
/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_session_refresh_req_st
*
* Description  : This structure defines the internal message data when
*                UATK callback sends reinvite request to SSA to send session
*				 refresh request without changing the media parameters            
*
*-----------------------------------------------------------------------------*/
typedef struct
{
	/*This will specify the session refresh method name (INVITE/UPDATE)*/
	  icf_uint8_t refresher_req_type;
} icf_nw_otg_session_refresh_req_st;

/*------------------------------------------------------------------------------
*
* Name : icf_cfg_ssa_set_min_se
*
* Description  : This structure defines the Internal Message data when
*				 CFG resets the min_se.          
*
*-----------------------------------------------------------------------------*/
typedef struct
{
	/*This will specify the session refresh method name (INVITE/UPDATE)*/
	  icf_uint32_t min_se;
} icf_cfg_ssa_set_min_se;

#endif

/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_reject_st
*
* Description  : This structure defines the internal message data when 
*                 CC  rejects the update request received from NW
*
*-----------------------------------------------------------------------------*/
typedef struct
{
  icf_ssa_reason_t reason_code;
  icf_uint16_t    response_code;
} icf_nw_otg_reject_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_call_setup_st
*
* Description  : This structure defines the internal message data when 
*                 CC sends a setup message to SSA to send INVITE on NW. 
*                 When no additional info needs to be sent, then donot send
*                 the payload.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
    /* This bitmask represents that the replace call id has to be sent
     * in the INVITE with the given nw call id.SSA to pick the value
     * from CallCtxt. CC to free it after use
     */
#define ICF_NW_OTG_SETUP_REPLACE_CALL_ID  0x01
    /* This bitmask represents that the diversion info needs to be sent
     * in the outgoing INVITE message.SSA to pick the value from CallCtxt
     * CC to free it
     */
#define ICF_NW_OTG_SETUP_DIV_INFO        0x02
    /* This bit mask represents that the referred by field needs to be
     * sent with the given address. SSA will pick this field from call ctx
     * p_referred_party_addr field 
     */
#define ICF_NW_OTG_SETUP_REF_BY          0x04
} icf_nw_otg_call_setup_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_call_alert_st
 * Decription  : Typedef to define the network alert message without media.
 *               When RPR is not to be sent (or when no info needs to be
 *               sent, then donot send the payload. 
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_OTG_ALERT_RPR_REQD  0x01
    /* By default it implies that the ALERT need not be sent with RPR. */
} icf_nw_otg_call_alert_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_alertmedia_st
 * Decription  : Typedef to define the network alert message with media.
 *               When RPR is not to be sent (or when no info needs to be
 *               sent, then donot send the payload. 
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_OTG_ALERT_WITH_MEDIA_RPR_REQD  0x01
    /* By default it implies that the ALERT need not be sent with RPR. */
} icf_nw_otg_alertmedia_st;

/*----------------------------------------------------------------------------
 ** Type Name   : icf_nw_otg_media_param_st
 ** Decription  : Typedef to define the 183 message with media.
 **               When RPR is not to be sent (or when no info needs to be
 **               sent, then donot send the payload. 
 **--------------------------------------------------------------------------*/
typedef struct
{
	    icf_uint8_t           bitmask;
/* By default it implies that the 183(media param) need not be sent with RPR.*/
#define ICF_NW_OTG_MEDIA_PARAM_RPR_REQD  0x01
/* Flag that determines whether outgoing 183 should contain SDP or not */
#define ICF_NW_OTG_MEDIA_PARAM_WITHOUT_SDP	0x02
               
}icf_nw_otg_media_param_st; 

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_prack_ack_st
 * Decription  : Typedef for outgoing prack ack. This payload is to be sent 
 *               only when any of the bitmask/info needs to be sent. Else 
 *               payload need not be sent.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_boolean_t         status; /* ICF_TRUE implies successful
                                      * response and ICF_FALSE implies
                                      * failure response. */
    icf_ssa_reason_t      reason_type;

    icf_uint8_t           bitmask;
#define ICF_NW_OTG_PRACK_ACK_SDP      0x01
#define ICF_NW_OTG_PRACK_RESP_REASON  0x02
    /* IF this bitmask is set, SSA will pick SDP from p_local_sdp filed of 
     * call context. This will be later freed by CC 
     * */
} icf_nw_otg_prack_ack_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_connect_st
 * Decription  : Typedef for interface of outgoing connect. If this additional
 *               info is not to be sent, then donot send the payload.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_OTG_CONNECT_WITHOUT_SDP   0x01
    /* By default it implies that Connect is to be sent with SDP. SDP needs
     * to be picked from local  sdp field of call context. */
} icf_nw_otg_connect_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_connect_ack_st
 * Decription  : Typedef for interface of outgoing connect ack. 
 *               If this additional
 *               info is not to be sent, then donot send the payload.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_OTG_CONNECT_ACK_WITH_SDP   0x01
    /* By default it implies that Connect is to be sent without SDP. SDP needs
     * to be picked from local sdp field of call context. */
} icf_nw_otg_connect_ack_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_refer_st
*
* Description  : This structure defines the internal message data when 
*                 SC sends a refer message to SSA to send REFER on NW. 
*                 When no additional info needs to be sent, then donot send
*                 the payload.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
      icf_uint8_t           bitmask;
      /* This bitmask represents that the replace call id has to be sent
       * in the INVITE with the nw call id for the given internal call id. */
#define ICF_NW_OTG_REFER_RELACE_CALL_ID  0x01
      /*This bitmask indicates that the SSA has to add a referred by header
       * in outgoing REFER.  SSA will use the information from from header data
       * for making referred by Header. SLP will set this flag if it decides
       *that presentation is not restricted*/
#define ICF_NW_OTG_REFER_REF_BY          0x02
/* Following bit will be set by CTU when method=BYE is to be send in 
 * refer-to header in REFER request*/

      
      icf_call_id_t         replace_call_id;
      /* This memory will be allocated and freed by CTA/CTU SLP */
      icf_address_st        *p_referred_to;
} icf_nw_otg_refer_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_refer_resp_st
*
* Description  : This structure defines the internal message data when 
*                 SC sends a ack message to SSA to send refer resp on NW. 
*                 When no additional info needs to be sent, then donot send
*                 the payload.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
      icf_uint8_t           bitmask;
#define ICF_NW_OTG_REFER_RESP_REASON   0x01
      icf_ssa_reason_t      reason;
      /* By default it implies successful response. */
      /*If response code is present then it has higher priority*/
      icf_uint32_t          resp_code;

} icf_nw_otg_refer_resp_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_otg_call_fwd_st
*
* Description  : This structure defines the internal message data when 
*                 CFW sends meassage to SSA to send 302 on the network.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    /* This is the address to which the call is to be forwarded 
	This is the address of local instance in the SLP PDB*/
    icf_address_st   *p_addr_for_cfw;
    
    /* This bit is used to specify that diversion info is present or not in 
     * the call ctx 
     */
#define ICF_NW_OTG_DIV_INFO 0x01
    icf_uint8_t      bitmask;
} icf_nw_otg_call_fwd_st;    

/*------------------------------------------------------------------------------
*
* Name : icf_nw_inc_call_setup_st
*
* Description  : This structure defines the internal message data when 
*                 SSA sends a setup message to CC on receiving INVITE from NW. 
*                 When no additional info needs to be sent, then donot send
*                 the payload.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
      icf_uint8_t           bitmask;
      /* This bitmask represents that the replace call id has to be sent
       * in the INVITE with the given nw call id. */
#define ICF_NW_INC_SETUP_RELACE_CALL_ID  0x01
      /* This bit mask represents that the referred by field needs to be
       * sent with the given address. SSA will fill this in Call context
       * in p_referred_party_addr field.CC to free this */
#define ICF_NW_INC_SETUP_REF_BY          0x02
/* Changes for XCONF */ 
	/* This bitmask represents that the incoming call is conference call or not.
	It means that incoming invite contains isfocus parameter or not */  
#define ICF_NW_INC_SETUP_ISFOCUS_FOUND   0x04



      /* This is used by CM to create a new Call context and update the fields
       * into new call context. This field shall be populated into the call
       * context and would be used by CM on getting response from MMI.*/
      icf_call_id_t      replace_call_id;

} icf_nw_inc_call_setup_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_prack_st
 * Decription  : Typedef for incoming prack. This payload is to be sent 
 *               only when any of the bitmask/info needs to be sent. Else 
 *               payload need not be sent. This will be allocated by SSA
 *               and freed by SSA 
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_INC_PRACK_SDP					0x01
#define ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM	0x02
#define ICF_NW_INC_MEDIA_ADD_STREAM				0x04

	/*p_sdp_list contains the list of streams received in SDP from remote
	 *which contains node of type icf_inc_media_sdp_info_st*/
	icf_list_st           *p_sdp_list;

     /* new added streams */
	icf_list_st           *p_added_sdp_list;
	
	/* this lists the additional streams that have been receieved in 
    * the incoming 200ok from the network, that is not already   
    * a part of the established session. 
    */
	icf_uint8_t				count;
	icf_stream_list_st rejected_streams[ICF_MAX_STREAM_PER_CALL];
} icf_nw_inc_prack_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_prack_resp_st
 * Decription  : Typedef for response for PRACK. This is to be sent only when
 *               a response for a PRACK has been received. This will be 
 *               allocated by SSA and freed by SSA. 
 * --------------------------------------------------------------------------*/
typedef struct
{
    /* ICF_TRUE indicates successful response for PRACK */
    icf_boolean_t        status;

    /* This field is filled if status is FALSE */
    icf_ssa_reason_t    error_cause;
    
    /* This boolean indicates that SDP is present or not */
    icf_boolean_t             sdp_present;

    /*  THis field shall contain the response code 
     *  received for outgooing PRACK 
     */
    icf_uint16_t              resp_code;
}icf_nw_inc_prack_resp_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_media_change_req_st
 * Decription  : Typedef for incoming mdia change req
 *               This payload is to be sent 
 *               only when bitmask is set else 
 *               payload need not be sent.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_INC_MEDIA_CHG_SDP_PRESENT     	0x01
#define ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM	0x02
#define ICF_NW_INC_MEDIA_ADD_STREAM		0x04
#define ICF_INC_MEDIA_CHANGE_METHOD_REINVITE    0x08
#define ICF_NW_INC_MEDIA_NW_REJECT_STREAM       0x10
/*  Following bitmask will be set by SSA when conference-ID and 
 *  isfocus is received first time in contact header*/
/*#define ICF_INC_MEDIA_CHANGE_ISFOCUS_FOUND      0x20*/


	/*
     *  This memory will be allocated and freed by SSA. 
     */    
	/*p_sdp_list contains the list of streams received in SDP from remote
	 *which contains node of type icf_inc_media_sdp_info_st*/
	icf_list_st    			*p_sdp_list;

	/* list of new added sdp nodes */
    icf_list_st    			*p_added_sdp_list;

    /* list of newly added sdp nodes which are recieved with port 0*/
    icf_list_st            *p_nw_reject_stream_list;

	/* this lists the additional streams that have been receieved in the 
	 * incoming Re-Invite/Update from the network, that is not already
	 * a part of the established session. */
	icf_uint8_t				count;
	icf_stream_list_st		rejected_streams[ICF_MAX_STREAM_PER_CALL];

} icf_nw_inc_media_change_req_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_media_change_resp_st
 * Decription  : Typedef for incoming mdia change resp. 
 *               This payload is to be sent 
 *               only when any of the bitmask/info needs to be sent. Else 
 *               payload need not be sent.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_boolean_t         status; /* ICF_TRUE implies successful
                                      * response and ICF_FALSE implies
                                      * failure response. */
    icf_uint8_t           bitmask;
#define ICF_NW_INC_MC_RESP_REASON     0x01
#define ICF_NW_INC_MC_SDP_PRESENT     0x02
#define ICF_CALL_MOD_REINV_CANCELLED  0x04
#define ICF_CALL_MOD_REINV_2_CANCELLED  0x08
    icf_ssa_reason_t      reason_type;

    /* SDP would be present only in case of success */
    /* This memory will be allocated and freed by SSA. CHR will send
     * service err ind if SDP contains a changed codec or ip address
     * & port. */    
#ifdef ICF_REL_OLD
    /* By defualt it implies no SDP is to be sent. */
    icf_sdp_st            *p_sdp_info;
#endif
	/*p_sdp_list contains the list of streams received in SDP from remote
	 *which contains node of type icf_inc_media_sdp_info_st*/
	icf_list_st           *p_sdp_list;
} icf_nw_inc_media_change_resp_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_media_change_ack_st
 * Decription  : Typedef for incoming mdia change ack
 *                .ICF_NW_INC_MEDIA_CHANGE_ACK
 *
 *               This payload is to be sent 
 *               only whenAck for reinvite is sent.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_INC_MC_ACK_SDP_PRESENT       0x01
#define ICF_NW_INC_MC_ACK_REJECTED_STREAM	0x02

    /* SDP would be present only in case of success */
    /* This memory will be allocated by SSA and freed by CC */ 
	/*p_sdp_list contains the list of streams received in SDP from remote
	 *which contains node of type icf_inc_media_sdp_info_st*/
	icf_list_st           *p_sdp_list;

    /* this lists the additional streams that have been receieved in the 
	 * incoming Re-Invite/Update from the network, that is not already
	 * a part of the established session. */
	icf_uint8_t				count;
	icf_stream_list_st		rejected_streams[ICF_MAX_STREAM_PER_CALL];
} icf_nw_inc_media_change_ack_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_media_change_resp_st
 * Decription  : Typedef for outgoing mdia change resp. 
 *               In case of outgiong media change response, the sdp would
 *               never be filled in message. Rather SSA would pick from 
 *               p_local_sdp. 
 *
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_boolean_t         status; /* ICF_TRUE implies successful
                                      * response and ICF_FALSE implies
                                      * failure response. */
    icf_uint8_t           bitmask;
#define ICF_NW_OTG_MC_RESP_REASON  0x01
#define ICF_NW_OTG_SDP_PRESENT     0x02
#define ICF_NW_OTG_MC_RESP_NW_RESPONSE  0x04

    icf_ssa_reason_t      reason_type;
    icf_uint16_t          nw_response_code;

    /* SDP would be present only in case of success,for normal scenarios.
	 * The only exception to this is in case of nw_res_reservation for QoS, 
	 * where SDP would be present even in failure response 
     * SDP would be picked by SSA only in case of bitmask present. It
     * shall pick up from p_local_sdp.*/

    /* SDP would be picked by SSA only in case of bitmask present. It
     * shall pick up from p_local_sdp.*/
} icf_nw_otg_media_change_resp_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_media_param_st
 * Decription  : Typedef for ICF_NW_INC_MEDIA_PARAM 183 from remote.
 *               (SSA will write SDP in p_remote_recv_sdp) 
 *
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_INC_MEDIA_PARAM_RPR_REQD  0x01

    /* By default it implies that RPR needs to be sent for incoming
     *  MEDIA PARAM msg */

   /*p_sdp_list contains the list of streams received in SDP from     
    *remote which contains node of type icf_inc_media_sdp_info_st*/

  icf_list_st    			*p_sdp_list;
   
   /* This number uniquely identifies a SDP received. This is required in
    * case of forking. */
   icf_uint8_t        forked_resp_id;

} icf_nw_inc_media_param_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_call_alert_st
 * Decription  : Typedef for ICF_NW_INC_CALL_ALERT 180 from remote.
 *
 * --------------------------------------------------------------------------*/

typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_INC_ALERT_RPR_REQD  0x01
    /* By default it implies that RPR needs to be sent for incoming ALERT */
} icf_nw_inc_call_alert_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_alert_media_st
 * Decription  : Typedef for ICF_NW_INC_ALERT_WITH_MEDIA 180 with sdp
 *               (SSA will write SDP in p_remote_recv_sdp)
 *
 * --------------------------------------------------------------------------*/

typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_INC_ALERT_MEDIA_RPR_REQD  0x01

    /* By default it implies that RPR needs to be sent for incoming ALERT
     * with Media */

   /*p_sdp_list contains the list of streams received in SDP from     
    *remote which contains node of type icf_inc_media_sdp_info_st*/

	icf_list_st    			*p_sdp_list;

   /* This number uniquely identifies a SDP received. This is required in
    * case of forking. */
    icf_uint8_t         forked_resp_id;

} icf_nw_inc_alert_media_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_connect_ack_st
 * Decription  : Typedef for ICF_NW_INC_CONNECT_ACK
 *               ((SSA will send SDP in message and CC will copy it)
 *               in p_remote_recv_sdp. SSA will free this memory
 *
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t           bitmask;
#define ICF_NW_INC_CONNECT_ACK_SDP   0x01
#define ICF_NW_INC_CONNECT_ACK_ERROR 0x02
    /* By defualt it implies  SDP is present. */

	/*p_sdp_list contains the list of streams received in SDP from remote
	 *which contains node of type icf_inc_media_sdp_info_st*/
	icf_list_st           *p_sdp_list;
} icf_nw_inc_connect_ack_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_connect_st
 * Decription  : Typedef for interface of incoming connect. If this additional
 *               info is not to be sent, then donot send the payload.
 * --------------------------------------------------------------------------*/
typedef struct
{
     icf_uint8_t           bitmask;
#define ICF_NW_INC_CONNECT_WITH_SDP				0x01
     /*SPR 13995 Aman 04-01-07*/
/*This is set when there is a failure in processing in 200ok*/
#define ICF_NW_INC_CONNECT_GENERIC_ERROR 			0x02
/* Following bitmask will be set by SSA when conference-ID and 
 * isfocus is received first time in contact header */
/*#define ICF_NW_INC_CONNECT_ISFOCUS_FOUND                        0x04*/


    /* By default it implies that Connect is to be received without SDP. */
    /* THis memory will be allocated by SSA, pointer copied into CC ctx 
     * and freed by CC */

	 /*p_sdp_list contains the list of streams received in SDP from remote
	 *which contains node of type icf_inc_media_sdp_info_st*/
	icf_list_st        *p_sdp_list;

   /* This number uniquely identifies a SDP received. This is required in
    * case of forking. */
    icf_uint8_t        forked_resp_id;

} icf_nw_inc_connect_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_inc_refer_st
*
* Description  : This structure defines the internal message data when 
*                 SSA indicates that refer has been received from NW. 
*                 When no additional info needs to be sent, then donot send
*                 the payload.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
      icf_uint8_t           bitmask;
      /* This bitmask represents that the replace call id has been received
       * and this needs to be sent
       * in the INVITE with the given nw call id. */
#define ICF_NW_INC_REFER_REPLACE_CALL_ID  0x01
      /* This bit mask represents that the referred to field has been
       * received with the given address. */
#define ICF_NW_INC_REFER_REF_TO          0x02
      /* This bit mask represents that the referred to field has been
       * received with the given name. */
#define ICF_NW_INC_REFER_REF_TO_NAME      0x04
          
      /* SSA will allocate this memory and RNW SLP Will copy this in new call
       * context and will be freed by CC after sending INVITE ind to SSA 
       */ 
      icf_replaces_hdr_st   *p_replaces_hdr;
      /* SSA will allocate this memory and RNW SLP Will copy this in new call
       * context and will be freed by CC after sending INVITE ind to SSA 
       */
      icf_address_st        *p_referred_to;
      /* SSA will allocate this memory and RNW SLP Will copy this in new call
       * context and will be freed by CC after sending INVITE ind to SSA 
       */
      icf_name_st            *p_referred_to_name;

} icf_nw_inc_refer_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_inc_refer_resp_st
*
* Description  : This structure defines the internal message data when
*                 SSA receives 202 from NW.
*
*-----------------------------------------------------------------------------*/
/*typedef struct
{
    icf_uint8_t   status;
    icf_uint8_t   error_cause;
} icf_nw_inc_refer_resp_st;*/

/*------------------------------------------------------------------------------
*
* Name : icf_nw_inc_call_redirect_st
*
* Description  : This structure defines the internal message data when 
*                 SSA receives 302 from NW. 
*                 When no additional info needs to be sent, then donot send
*                 the payload.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
      icf_uint8_t           bitmask;
      /* This bitmask represents that the diversion info that is received and
       * the one which needs to be sent in INVITE. The memory for diversion
       * info is allocated by SSA and pointer copied into call context on
       * getting 302 and will be freed by CC after sending diverison info in
       * the outgoing INVITE message. */
#define ICF_NW_INC_CALL_REDIRECT_DIV_INFO        0x01
} icf_nw_inc_call_redirect_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_inc_redirect_ind_st
*
* Description  : This structure defines the internal message data when 
*                 SSA receives 181 from NW. 
*                 When no additional info needs to be sent, then donot send
*                 the payload.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
      icf_uint8_t           bitmask;
      /* This bitmask represents that the diversion info that is received*/
#define ICF_NW_INC_REDIRECT_IND_DIV_INFO        0x01
      /* This bitmask represents that the diversion info that is received and
       * the one which needs to be sent to PA. The memory for diversion
       * info is allocated by SSA and pointer copied into call context on
       * getting 181 and will be freed by CC after sending diverison info in
       * the indication to PA */

} icf_nw_inc_redirect_ind_st;

#define ICF_CRM_START
#define ICF_CRM_END


ICF_CRM_START

ICF_4_0_CRM_START
/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cmn_auth_data_st
 * Description      : This datatype is used to store authentication related
 *                    information for a particular rgm_context/ssa_ctxt.
 *					  This information will be stored for both sent and received
 *					  auth_data.
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
	/*
     * bitmask that identifies what all auth parameters 
     * are present
     */
#define ICF_AUTH_SCHEME_PRESENT						0x0001
#define ICF_AUTH_ALGO_PRESENT						0x0002
#define ICF_AUTH_REALM_PRESENT						0x0004
#define ICF_AUTH_NONCE_PRESENT						0x0008
#define ICF_AUTH_NC_PRESENT							0x0010
#define ICF_AUTH_CNONCE_PRESENT 				        0x0020
#define ICF_AUTH_RES_PRESENT							0x0040
#define ICF_AUTH_OPAQUE_PRESENT                      0x0080
#define ICF_AUTH_AUTS_PRESENT                        0x0100
#define ICF_AUTH_URI_PRESENT							0x0200
#define ICF_AUTH_QOP_PRESENT							0x0400
    icf_uint16_t                  auth_data_present;


    /* Contains the scheme */
    icf_string_st                 scheme;

	/* Contains the algo */
    icf_string_st                 algo;

	/* Contains the realm */
    icf_string_st                 realm;

    /* Contains the nonce */
    icf_uint8_t	                 *p_nonce;
	/* Contains the nonce_len */
	icf_uint32_t					 nonce_len;

    /* Contains the nc */
    icf_uint32_t                  nonce_count;
    

    /* Contains the response */
    icf_string_st                 res;

    /* Contains the opaque */
    icf_string_st                 opaque;

    /* Contains the AUTS parameter */
    icf_string_st                 auts;

    /* Contains the URI parameter */
    icf_string_st				uri;
	
    /* Contains the QoP parameter */
    icf_string_st				qop;

	/* Contains the cnonce to be used */
	icf_string_st		cnonce;

    /* The following two AKA specific fields have been moved from rgm
     * context st and defined here.This structure is visible in 
     * the port procedure function, the reason being a pointer to this
     * structure is passed to that function..These are computed by the aka
     * procedure function and stored in rgm_context->auth_data_sent or
     * ssa_ctxt->auth_data_sent.Earlier these were being directly copied
     * to rgm_ctx.
     */

     /* encryption key and length: this is imsCk(Cipher key) generated
      * in case of AKA challenge algo.*/
     icf_short_string_st                enc_key;

     /* integrity password and length: this is imsIk(Integrity key)generated
      * in case of AKA challenge algo.*/
     icf_short_string_st                auth_passwd;

     /* This field is used only in case of AKA challenge.It stores the
      * sequence number which is derived from the nonce value received 
      * in the challenge(by the porting function for AKA).The sqn so computed
      * is stored and subsequently passed to the same porting function
      * which uses it for determining synchronization failures(if any).
      * A synchornization failure occurs if the difference in the last
      * and the new seq number is more than 32.
      */
     icf_uint32_t sequence_num;

}icf_cmn_auth_data_st;
/*------------------------------------------------------------------------------
*
* Type Name        : icf_rgm_optional_param_st
* Description      : This datatype is used to store the optional parameters 
*					  received with the contact subelement 
*					  of the NOTIFY message body.
*
*----------------------------------------------------------------------------*/
typedef struct
{
	icf_uint8_t		*p_name;
	icf_uint8_t		*p_value;
}icf_rgm_optional_param_st;


/*------------------------------------------------------------------------------
*
* Type Name        : icf_rgm_contact_blk_st
* Description      : This datatype is used to store the contact 
*					  subelement received in the registration element
*					  of the NOTIFY message body.
*
*----------------------------------------------------------------------------*/
typedef struct icf_rgm_contact_blk_st
{
    icf_uint8_t                 *p_id;
    icf_uint8_t                 state;
	
#define ICF_RGM_NFY_CON_STATE_INVALID        0
#define ICF_RGM_NFY_CON_STATE_ACTIVE			1
#define ICF_RGM_NFY_CON_STATE_TERMINATED		2
	
	icf_uint8_t                 event;/* possible values: registered,
                                         created, refreshed, shortened, 
                                         expired, deactivated,probation,
										unregistered, rejected */
#define ICF_RGM_NFY_CON_EVT_INVALID				0
#define ICF_RGM_NFY_CON_EVT_REGISTERED			1
#define ICF_RGM_NFY_CON_EVT_CREATED				2
#define ICF_RGM_NFY_CON_EVT_REFRESHED			3
#define ICF_RGM_NFY_CON_EVT_SHORTENED			4
#define ICF_RGM_NFY_CON_EVT_EXPIRED				5
#define ICF_RGM_NFY_CON_EVT_DEACTIVATED			6
#define ICF_RGM_NFY_CON_EVT_PROBATION			7
#define ICF_RGM_NFY_CON_EVT_UNREGISTERED			8
#define ICF_RGM_NFY_CON_EVT_REJECTED				9
	
	
	icf_address_st              *p_contact_uri; /* to contain the uri */										
	icf_uint16_t                bitmask; /* because many of below members 
												*  here are optional */
												
#define ICF_RGM_NFY_CON_INVALID_OPTION			0x0000
#define ICF_RGM_NFY_CON_DISPLAY_NAME_PRESENT		0x0001
#define ICF_RGM_NFY_CON_OPT_PARAM_PRESENT		0x0002
#define ICF_RGM_NFY_CON_EXPIRES_PRESENT			0x0004
#define ICF_RGM_NFY_CON_RETRY_AFTER_PRESENT		0x0008
#define ICF_RGM_NFY_CON_DURATION_PRESENT			0x0010
#define ICF_RGM_NFY_CON_PRIORITY_PRESENT			0x0020
#define ICF_RGM_NFY_CON_CALL_ID_PRESENT			0x0040
#define ICF_RGM_NFY_CON_CSEQ_PRESENT				0x0080
												
	icf_uint32_t               expires; /* When event = shortened */
	icf_uint32_t               retry_after; /* When event = probation */
	icf_uint32_t               duration_registered;
	icf_uint32_t               cseq;	
	icf_uint8_t                *p_display_name_element;/* optional string to 
													contain the display name */
	icf_uint8_t                *p_relative_priority; /*"q" */
	icf_uint8_t                *p_call_id;
												
	/* this will be a structure as 	
	  * to contain the name and the value of this 
	  * optional parameter */ 
	 /* For ICF: this corresponds to the tag given by 
	* an application  which is actually a value of an unknown/user-specific parameter */
	icf_rgm_optional_param_st	*p_opt_param; 
												
	struct icf_rgm_contact_blk_st	*p_next; /* pointer to next Element */
												
}icf_rgm_contact_blk_st;


/*------------------------------------------------------------------------------
*
* Type Name        : icf_rgm_contact_list_st
* Description      : This datatype is used to store the list of contact 
*					  subelements received in the registration element
*					  of the NOTIFY message body.
*
*----------------------------------------------------------------------------*/
typedef struct
{
	/* Number of contacts */
	icf_uint32_t		count;
	
	/*pointer to the first contact_bock*/
	icf_rgm_contact_blk_st		*p_contact_blk;
	
}icf_rgm_contact_list_st;


/*------------------------------------------------------------------------------
*
* Type Name        : icf_rgm_regn_block_st
* Description      : This datatype is used to store the registration 
*					  subelement received in the reginfo element
*					  of the NOTIFY message body.
*
*----------------------------------------------------------------------------*/
typedef struct icf_rgm_regn_block_st	
{
    /* AOR attribute of registration element */
	icf_address_st                 *p_aor;
	
	/* Unique ID of each registration element */
	icf_uint8_t                 *p_id;
	
	/* States allowed as attribute value of registration element */
	icf_uint8_t					state;
	
	/* possible values: init(in the first notify, when there are no active registrations), 
				active,  terminated */
#define ICF_RGM_NFY_REG_STATE_INVALID		0
#define ICF_RGM_NFY_REG_STATE_INIT			1
#define ICF_RGM_NFY_REG_STATE_ACTIVE			2
#define ICF_RGM_NFY_REG_STATE_TERMINATED		3
	
	/* List of Contact subelements obtained in registration element */
	icf_rgm_contact_list_st	contact_list;
	
	
	/*pointer to next registration_block*/
	struct icf_rgm_regn_block_st		*p_next;
	
}icf_rgm_regn_block_st;


/*------------------------------------------------------------------------------
*
* Type Name        : icf_rgm_regn_list_st
* Description      : This datatype is used to store the list of 
*					  registrations subelements received in the reginfo element
*					  of the NOTIFY message body.
*
*----------------------------------------------------------------------------*/
typedef struct
{
	/* The number of registration elements received in reginfo */
	icf_uint32_t						count;
	
	/* pointer to the first registration block */
	icf_rgm_regn_block_st		 	*p_regn_block;
	
}icf_rgm_regn_list_st;


/*------------------------------------------------------------------------------
*
* Type Name        : icf_rgm_reginfo_st
* Description      : This datatype is used to store the reg_info element parsed from the
*					  NOTIFY message body.
*
*----------------------------------------------------------------------------*/
typedef struct
{
	
/* SPR 9068: fix starts */
	/* A number value to indicate the order of NOTIFY message:
	 * Invalid number is -1: default value */
#define ICF_INVALID_REGINFO_VERSION			-1
	icf_int32_t	version;
/* SPR 9068: fix ends */
	
	/* The state of reginfo allowed: */
#define ICF_RGM_NFY_REGINFO_STATE_INVALID 		0
#define ICF_RGM_NFY_REGINFO_STATE_FULL	 		1
#define ICF_RGM_NFY_REGINFO_STATE_PARTIAL		2
	icf_uint8_t	reginfo_state;
	
	/*The list ot registrations received in this reginfo root element */
	icf_rgm_regn_list_st registration_list;
}icf_rgm_reginfo_st;



/*------------------------------------------------------------------------------
*
* Type Name        : icf_rgm_glb_parse_reginfo_st
* Description      : This datatype is used while parsing the reg_info element of the
*					  NOTIFY message body.
*
*----------------------------------------------------------------------------*/
typedef struct
{
	icf_glb_pdb_st          *p_glb_pdb; 
    icf_uint16_t             bit_mask;
#define ICF_RGM_NFY_XML_PARSE_REGINFO_START				0x0001
#define ICF_RGM_NFY_XML_PARSE_REGNBLOCK_START			0x0002
#define ICF_RGM_NFY_XML_PARSE_CONTACTBLOCK_START			0x0004
#define ICF_RGM_NFY_XML_PARSE_URIBLOCK_START				0x0008

	icf_rgm_regn_block_st    *p_temp_regn_block;
	icf_rgm_contact_blk_st   *p_temp_contact_block;

	icf_rgm_reginfo_st		*p_actual_rgm_reg_info;
}icf_rgm_glb_parse_reginfo_st;


ICF_4_0_CRM_END


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_app_ctxt_st
 * Description      : This data type is used to store the application 
 *					  specific information in the registration context.
 *----------------------------------------------------------------------------*/


typedef struct {
	
	icf_boolean_t   			is_valid; /* valid entry or not */
	icf_app_id_t   			app_id; 
	
	/* used while sending confirm/indication only */
	icf_line_id_t   			line_id; 
	
	icf_uint8_t      		app_indicator ; 

#define ICF_RGM_APP_INDICATE       		0x01
#define ICF_RGM_APP_DELETE       	  	0x02
#define ICF_RGM_APP_CFM_SENT			0x04
#define ICF_RGM_APP_DEREG_ON_PROBATION   	0x08

 
	icf_error_t      			app_indicated_err_cause; 
 	icf_duration_t	    		expiry ;

	/* To indicate the presence of the lists: the hash
     * define values will be same as used for payload */

    /* RN changed from 8 bit to 32 bit*/
    icf_uint32_t                  bitmask;
#define ICF_TAGS_REFRESHED		0x01
	
	icf_header_list_st   		header_list;
	icf_header_list_st   		inc_header_list;
	icf_tag_list_st   			tag_list;
 	icf_msg_body_list_st  		body_list ;
    /* This contains the 'to' recieved from 
       the application in the reg request*/
    icf_address_st              to;
} icf_rgm_app_ctxt_st ; 


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_reg_event_subs_ctx_st
 * Description      : This is the structure is used for maintaining the context for 
 *						"reg-event" subscription for ICF.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{

	icf_subs_ntfy_evnt_et	notify_ind;

	/* To store the last received version of reg_info */
	icf_int32_t				reg_info_version;

	icf_uint16_t			indicator;

	icf_duration_t			duration;

        /* This will be stored while allocating memory for this
         * structure in regm module.*/
        struct icf_rgm_context_st       *p_rgm_context;

}icf_reg_event_subs_ctx_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_context_st
 * Description      : This datatype is used to store registration information
 *                    for a particular user and registrar combination.
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_rgm_context_st 
{

	/* Address of the user */
	icf_address_st rgm_user_address;


	/* Registration Duration */
    icf_duration_t rgm_duration;

    /* State of the user */    
	icf_uint8_t rgm_state;

    /* Number of lines present in the current block */
    icf_line_id_t num_of_lines;
    
	/* Array of line id's */
    icf_line_id_t rgm_list_of_lines[ICF_MAX_NUM_OF_LINES];
    
    /* Required by SSA */
	icf_uint8_t reg_id;			/* key to map the response */
    
	/* List of IP Address corresponding to a Registrar */
    icf_transport_addr_st list_of_registrar_ip[ICF_MAX_IP_PER_REGISTRAR];

    /* Bit flag to indicate authentication(0x1). 1st bit is set to indicate
     * request is to be send with Authentication Information. 2nd bit is
     * set to indicate that PA is indicated about Registration Failure.
     * 3rd bit is set to indicate that current registration block has been
     * marked for Deletion.
     */
#define ICF_RGM_AUTH_SET           0x0001
#define ICF_RGM_PA_INDICATED       0x0002
#define ICF_RGM_DELETE_INDICATION  0x0004

	  /* This value will be set by SSA when the duration
	   * in rgm_context is to be filled with min-expires value */
#define ICF_RGM_SSA_DURATION_FILLED	0x0008

	   /* This value used to indicate that the warning header
	    * has been filled in the nw_resp structure on receipt of
		* 403 : SPR 8064 */
#define ICF_RGM_SSA_WARNING_FILLED	0x0010
		/* On obtaining 200 OK for Register message, SSA checks 
		 * if Contact header is obtained. If yes then it sets this
		 * flag. REGM checks for this flag, and if set trigger SIC
		 * module to send the SUBSCRIBE for reg event. 
		 */
#define ICF_RGM_SSA_CONTACT_RECD		0x0020

		 /* For receipt of QoP in 401 */
#define ICF_RGM_SSA_QOP_RECD			0x0040

ICF_CRM_END

ICF_4_0_CRM_START
		/* In case 200 is received even once, the rgm_context
		 * becomes  ONCE _REGISTERED: example usage: CC will allow
		 * the call to proceed if in the ongoing registration this 
		 * value is set, for URI is considered registered during
		 * RE_RGISTRATION.*/

#define ICF_RGM_ONCE_REGISTERED			0x0080

		 /* NW_STATUS_DEACTIVATED value to be used for Registration Recovery */
#define ICF_RGM_NW_STATUS_DEACTIVATED	0x01000
		 /* To store the status that NOTIFY has been received atleast once. */
#define ICF_RGM_NOTIFY_ONCE_RECD				0x02000
        /* To be used after all contacts get processed for NOTIFY */

#define ICF_RGM_NFY_ON_PROBATION             0x04000
#define ICF_RGM_NFY_ON_DEACTIVATION          0x08000

        /* This value is set if the probation timer expires during the 
         * REGISTERING or AUTORIZING state : To be used on receipt of 200 */
#define ICF_RGM_REFRESH_REGISTER_ON_PRBN     0x10000
#define ICF_RGM_BUFFER_FOR_REGISTRATION   0x20000
#define ICF_RGM_IMPLICITLY_REGISTERED     0x40000
#define ICF_REGISTER_FOR_ALL_USER		  0x80000
         /* To have the indication for the UNREGISTERED state 
          * due to start of reattempt_timer in auto_reg ON 
          * and network failure sent */
#define ICF_RGM_UNREG_ON_REATTEMPT        0x100000

/* If set, this bit indicates that a request had been challenged and is 
 * now being sent again with Authorization parameters. So cnonce needs to 
 * be generated afresh for each challenge */
#define ICF_REQ_AGAIN_AFTER_CHALLENGE     0x200000
/* SPR 16019: 
 * If set, this bit indicates that Ongoing Registration.De-registration 
 * has to be clearedbefore sending the new register/De-register request.
 * Used to clear the ongoing register transaction without deleting the 
 * registration context/call object.
 */
#define ICF_RGM_CLEAR_ONGOING_REGISTRATION   0x400000
#define ICF_RGM_INTERNAL_GENERATED_REG       0x800000

/****************ICF 8.0***********************/
/*Bit field use in the parsing of notify received from remote*/

/*If set this bit indicate that the current AOR under notify parsing is
  a Public AOR*/

#define ICF_RGM_PUBLIC_AOR_RECEIVED          0x1000000
/*If set this bit indicate that the regm context need to be cleared as our
 *contact under current AOR ie Public Uri in notify parsing is terminated or
 *event is either one of them ie expired,deactivated,unregistered or
 *rejected*/
  
#define ICF_RGM_CTXT_NEEDS_TO_BE_CLEARED     0x2000000

/*If set this bit indicate that the contact under  current AOR in notify 
 *parsing matches with self contact*/
#define ICF_RGM_SELF_CONTACT_RECEIVED        0x4000000
/* If this bit is set then it means that deregister request is sent by the app. */ 
#define ICF_RGM_APP_INITIATED_DEREG          0x8000000       

/* If this bit is set then it means that deregister request is sent automatically
 * by the IPTK. This is currently used in case of de-reg initiated by IPTK in case 
 * of NAT binding change */ 
#define ICF_RGM_INTERNAL_GENERATED_DEREG         0x10000000    
/* CSR 1-6212448: Define a new bit to indicate whether Register response trace 
 * to be sent or not */
#define ICF_RGM_NO_RESP_TRACE		        	 0x20000000 

/* This is used to allow only one time refresh by the application to the register request */

#define ICF_REG_REFRESH_CONFIG_ALREADY_ISSUED                   0x80000000

   /* This bitmask indicates whether the reg event package subscription 
    * configuration is already issued or not so that the subsequent values
    * can be ignored.
    */
#define ICF_REG_EVENT_PCKG_SUBS_CONFIG_ISSUED    0x40000000

    icf_uint32_t indicator;

/* If this bit is set, it indicates that the security-association
 * has been done.
 */
#define ICF_RGM_SECURITY_ASSOCIATION_DONE        0x01
/*SPR 20026 This bitmask shall be used to identify aor received 
  in xml of NOTIFY for reg event pkg subscription is the Associated URI*/
#define ICF_RGM_ASSOCIATED_URI_RECEIVED          0x02
 
    icf_uint32_t indicator_1;

    /* This is the identifier of the timer which is running on this block */
    icf_timer_id_t timer_id;

    /* This is the pointer to the timer which is running on this block */
    icf_timer_data_st *p_timer_buff;

    /* This is the pointer to SSA Context */
    icf_void_t *p_ssa_ctxt;

	/* SPR 8064: Retrieving Warning header's value from 403 */
	 icf_string_st	warning_hdr_value;

    /*********************ICF 8.0********************/
    /* This field is used to store the Service-Route Header
     * received in the sucess response of registration*/
    icf_void_t *p_srv_rte;

#ifdef IMS_CLIENT

ICF_CRM_START

    
	
	/* This is 0 when 200 OK is obtained or when initial register is to be sent ; 
	 * This is a guard count to avoid infinite looping (ICF_MAX_REGM_RETRY = 5 )
	 * Example: if 5 times invalid challenge is obtained, 
	 then user shall be informed  */
	icf_uint8_t    	           retry_count ; 

	
ICF_CRM_END

ICF_4_0_CRM_START

    icf_uint8_t             buffer_id;

ICF_4_0_CRM_END

#endif

	/* To store the id of the registration block received in
	 * the first NOTIFY */
	icf_string_st			notify_reg_id;

	 /* To store the RETRY count for processing invalid 401*/
	icf_uint8_t				auth_retry_count;

	 /*ICF4_0: This is supported in non IMS scenario:
		And this will be picked from the icf.cfg file
	  To maintain the client_nonce and nonce_count */

	  icf_string_st		client_nonce;/* this is cnonce as in Specs */

	  /* to store the auth_data to be sent on the network */
	  icf_cmn_auth_data_st		auth_data_sent;

	  /* To store the auth_data received in 401 */
	  icf_cmn_auth_data_st		auth_data_recd;

	/* This is the list of application data to be maintained by each 
		RGM context */
	icf_rgm_app_ctxt_st  	*p_app_data_list ;  
   
 /* auto_register flag */
    icf_boolean_t            auto_reg_flag;

    /* Bitmask to store the event that made auto-reg false  */
    icf_uint8_t        event_auto_reg;
#define ICF_AUTO_REG_NOT_ON              0
#define ICF_AUTO_REG_EXPIRED             1
#define ICF_AUTO_REG_REJECTED            2
#define ICF_AUTO_REG_DEACTIVATED         3
#define ICF_AUTO_REG_PROBATION           4
#define ICF_AUTO_REG_UNREGISTERED        5


    /* Timer value for probation event */
    icf_uint32_t         nfy_retry_timer_value;

    /* This is the identifier of the timer which is 
       running for probation event of NOTIFY */
    icf_timer_id_t   prbn_timer_id;

    /* This is the pointer to the timer which is running 
       for probation event of NOTIFY */
    icf_timer_data_st *p_prbn_timer_buff;

    icf_address_st    *p_registered_pub_uri;

    /* This stores the contact list sent out in register request */
	icf_address_list_node_st *p_contact_uri_list;

#ifdef ICF_TRACE_ENABLE
    icf_trace_criteria_data_st *p_trace_criteria_data;
#endif

#ifdef ICF_NAT_RPORT_SUPPORT

   /* this field is added to maintain RPORT Status on per registration
      block context */
#define ICF_INITIAL_RPORT_INITIAL   0x0000
#define ICF_INITIAL_RPORT_ONGOING   0x0001
#define ICF_INITIAL_RPORT_PENDING   0x0010
#define ICF_INITIAL_RPORT_COMPLETED 0x0011
#define ICF_BINDING_REFRESH         0x0100
   icf_uint16_t                   rport_status;
   icf_boolean_t                  ong_dereg_counter;
#endif
   /* SPR 16019:Store the line id and app id in the rgm context, This will be 
    * used to send the response once the message is processed and sent to SSA.
    */
   icf_line_id_t       line_id;
   icf_app_id_t        app_id;
   
   /*****************ICF 8.0**************************/
   /*It is used to store the sic context  which will be allocated before
    * sending SUBSCRIBE for reg event package*/
   struct icf_sic_ctx_st     *p_sic_ctxt;

   /*This structure hold subscription related information for the ?œreg??event*/
   icf_reg_event_subs_ctx_st	*p_reg_event_subs_ctx;

   /* This field has been added to store the challenge algo
    * received(Aka or MD5) */
   icf_challenge_algo_t challenge_algo;

   /* This parameter specifies whether the application wants the
    * contact header(s) other than local contact as obtained in the 200 OK
    * response for REGISTER request.
    */
   icf_uint8_t            contact_list_required;

   /* It contains the list of contact header(s) other than local contact
    * as obtained in the 200 OK response for REGISTER request.
    */
   icf_header_list_st     contact_list;

   /* This parameter is used to store the Security-client
    * parameters provided by application in REGISTER request.
    */
   icf_large_string_st*   p_security_client;
   
   /* This parameter is used to store the Security-Verify
    * parameters which will be a mirror copy of security-server header 
    * value received in 401 of REGISTER request.
    */
   icf_large_string_st*   p_security_verify;

   /* This parameter is used to store the security-server header
    * value received in 401 of REGISTER request when security association
    * already exists.
    */
   icf_large_string_st*   p_security_server;

   /* This parameter specifies whether the reg event package subscription 
    * configuration is already issued or not so that the subsequent values
    * can be ignored.
    */
   icf_boolean_t    reg_event_pckg_subs_required;

    /* This parameter specifies whether the IPTK should refresh the registration on its own.*/

    icf_boolean_t    reg_refresh_required;
    /*  Fix for CSR 1-7193955: SPR 19668*/
    /*  IPTK shall use this value to start the Regsistartion
     *  refresh multiplier.
     *  1:if its value is 0 then IPTK shall use the below mentioned 
     *    arithmatic to start the refresh reg timer:
     *    If register_head_start_timer is configured.
     *        If register_head_start_timer is less than (0.5) * Expiry duration, 
     *            duration = Expiry duration - register_head_start 
     *        Else
     *            duration = (0.5) * Expiry duration.
     *     Else
     *        duration = (0.5) * Expiry duration.
     *  2:If its value is greater than 0 and less than 100
     *    then timer will be started on below mentioned 
     *    arithmatic:
     *    (refresh_registration_multiplier /100) * (Expiry duration) duration.
     */
    icf_uint8_t registration_refresh_multiplier;

    /* SPR 19999 CSR 1-7533733 SPR 19870 *
     * This boolean checks if the application requires 
     * status ind on refresh register
     */
     icf_boolean_t	refresh_ind_reqd;
     /* A boolean value used by the application to deregister a specific Contact
     * in the API */
    icf_boolean_t dereg_specific_contact;


} icf_rgm_context_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_nw_resp_t
 * Description      : This is the datatype of the response structure returned  
 *                    by SSA to RGM.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* This field indicates type of response (SUCCESS/ FAILURE) */
    icf_boolean_t status;   

    /* This field indicates the type of failure and is significant in case
     * status field contains FAULURE */
    icf_error_t ecode;
    
    /* This is the pointer to the registration block for which response is
     * received */
	icf_rgm_context_st *p_rgm_context;

    /* This is filled from the Expires header received in 200 OK from Registrar
     */
    icf_duration_t   expires_duration;


	/* This member is to contain the nonce/challenge
	 * received in www-authenticate header of 401 response */
	 
	 /* For 4_0 These members are not needed, once the SSA code is changed, 
	  * these will be removed. */
	 icf_uint8_t		*p_www_auth_challenge;
	 icf_uint32_t	challenge_len;
 
	/* response code will be sent by SSA to RGM for cases of FAILURE
	 * so that a proper error cause is sent to the application by ICF. */
	 icf_uint32_t		response_code;


}icf_rgm_nw_resp_st;

ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_netact_app_ctxt_st
 * Description      : This datatype is used to store applications associated 
 *                    with a particular Network(APN).
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    /* ICF_FALSE implies this entry is invalid */
	icf_boolean_t is_valid;
      
	/* Application ID */
	icf_app_id_t app_id; 
      
	/* Bitwise Flag to indicate whether response has been 
	 * sent to the user - for future use */
#define ICF_NETACT_APP_INDICATED		0x01 
	icf_uint8_t indicator; 
}icf_netact_app_ctxt_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_netact_ctxt_st
 * Description      : This datatype is used to store network activation
 *                    information for a particular APN.
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    /* ICF_FALSE implies this network entry is invalid */
	icf_boolean_t is_valid; 
	
ICF_NW_RECOVERY_START
    /* ICF_TRUE implies this network was auto_activated */
	icf_boolean_t auto_activated; 
ICF_NW_RECOVERY_END

	/* Access Point Name specifying service and operator information */
    icf_string_st apn; 
    
	/* Access Network type */                                                                
	icf_uint8_t access_type; 
    
	/* Activation State of network */
	icf_uint8_t activate_state ; 
	/* Initial State and Deactivated state */
#define ICF_NETACT_00_IDLE		        0
	/* Activation Request in Progress */
#define ICF_NETACT_01_ACT_RESP_AWAITED	1
	/* Deactivated state */
#define ICF_NETACT_02_ACTIVATED          2
	/* Deactivation Request in Progress */
#define ICF_NETACT_03_DEACT_RESP_AWAITED 3
ICF_NW_RECOVERY_START
	/* Deactivation Request in Progress */
#define ICF_NETACT_04_NW_INDICATED_DEACT 4
ICF_NW_RECOVERY_END
    /* Network is Suspended */
#define ICF_NETACT_05_NW_SUSPENDED       5


	/* Pointer to list of applications associated with the network */
	icf_netact_app_ctxt_st *p_app_ctxt;

	/* Field presence indicator */
	icf_uint8_t bit_mask ; 

    /* #defines commented as already defined */
    /* This bit mask represents that ctxt_id field is set to a valid value */
/* #define ICF_NET_INFO_CTXT_ID_PRESENT        0x01 */
	/* This bit mask represents that self_ip field is set to a valid value */
/* #define ICF_NET_INFO_SELF_IP_PRESENT        0x02 */
    /* This bit mask represents that outgoing_proxy_addr field is set to a valid value */
/* #define ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT 0x04 */


	/* Logical PDP context */
	icf_uint32_t ctxt_id ; 

	/* Self IP Address */
	icf_transport_addr_st self_ip;

	/* List of Outbound Proxy Addresses */	
	icf_transport_list_st	outbound_proxy_addr; 
#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
	/* List of DNS Server Addresses */	
	icf_transport_list_st	dns_addr; 
#endif
}icf_netact_ctxt_st;
#endif/*#ifdef ICF_NW_ACTIVATE*/

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_app_profile_st
 * Description      : This datatype is used to store application profile information
 *
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    icf_string_list_st supported_methods;
    icf_string_list_st supported_events;
    icf_string_list_st supported_content_types;
    icf_string_list_st supported_encodings;
    icf_string_list_st supported_languages;
    /* This flag specifies whether the Application supports
       basic SIP methods (INVITE,BYE,ACK,OPTIONS,CANCEL) */
    icf_boolean_t      basic_method_support;
/*ICF_HANDLE_REINVITE_START*/
	/* This flag specifies of the application requires the support
	   for Call Modify or Call Hold / Resume */
	icf_boolean_t		call_modify_support_reqd;	
   
    /*Application can set this boolean to enable ICF to send merge_media_req
     *before receiving remote media paramaters, if this flag is not set ICF
     *will send merge_media_req when it has received remote SDP*/
    icf_boolean_t           merge_req_wo_rem_sdp;

/*ICF_HANDLE_REINVITE_END*/
}icf_app_profile_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_app_conf_st
 * Description      : This datatype is used to store application information                  
 *
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
	/* ICF_FALSE implies this application entry is invalid */
  	icf_boolean_t 			is_valid;

	/* Application ID */
    icf_app_id_t 			app_id; 

	/* Application Name */
  	icf_string_st 			app_name; 

    /* Routing Info required in absence of Application ID */
	icf_void_st 			routing_info; 

	/* ICF REL 6.0: multiple m lines
	 * In the curent ICF release, call type will be a bitmap rather than being 
	 * an enum. This is to allow for the multiple call combinations possible 
	 * in this release
	 */
	icf_uint8_t				call_type;

	/* Filter List specified by the application */
  	icf_int_filter_list_st 	*p_filter_list; 


    /*  List of additional headers required by the application */ 
    icf_header_name_list_st hdr_list; 

    icf_module_id_t			app_source_id;

    /* Application specific profile data */
    icf_app_profile_st 		app_profile;

	/* ICF REL 6.0: multiple m lines */
	/* this is list of node type icf_stream_capab_st. Based on the media
	 * streams available in the media_profile, ICF will set the bits in the 
	 * call_type field of the structure. For an outgoing/incoming call, CC 
	 * will need to validate that the streams in the call are a subset of the
	 * streams in the media_profile*/
	icf_internal_list_st	*p_app_media_profile;

}icf_app_conf_st ;
ICF_CRM_END

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_service_trigger_st
 * Description      : This is the datatype of message from CM to SCM on return 
 *                    path of CC message for Call forwarding in status
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* 
     * This can take values ICF_STATUS_CALL_FWD_UNCOND or 
     * ICF_STATUS_CALL_FWD_NO_ANS or ICF_STATUS_CALL_FWD_BUSY_LINE
     */
    icf_uint32_t         status;

} icf_service_trigger_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_service_call_data_st
 * Description      : This is the datatye of message from CM to CC,
 *                    from SLP to SCM. This datastructure is used to 
 *                    indicate other call context in service logic
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_call_ctx_st         *p_other_call_ctx;

} icf_service_call_data_st;


/************************************************************************
 *
 * STRUCTURE NAME: icf_dbm_scalable_data_init_info_st
 *
 * DESCRIPTION:  This structure would contain the information which is received
 *               from configuration and needs to be passed on to DBM for
 *               initializing global data structure.
 *
 ************************************************************************/
typedef struct
{

    /*Maximum number of configured Lines*/
    icf_uint16_t max_num_of_lines;

    /*Maximum number of calls supported at a time*/
    icf_uint16_t max_num_of_calls;

    /*Maximum number of registrations allowed*/
    icf_uint16_t max_num_of_registrations;

    /*Optional bit flag to indicate whether option parameters exist*/
    icf_uint16_t optional_bit_flag;
    
    /*Maximum number of subm context to be created*/
    icf_uint16_t max_num_of_subm_ctx;
    
    /*Maximum number of service calls allowed*/
    icf_uint16_t max_num_of_service_calls;

    /*Maximum number of timers allowed*/
    icf_uint16_t max_num_of_timers;
}icf_dbm_scale_data_init_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_ctx_st
 *
 * Description:     This is the context of SIC Module 
 * 
 * ---------------------------------------------------------------------------*/
typedef struct icf_sic_ctx_st
{
	/*Used to store the SIP method for which this
	 * ctxt was created*/
#define	ICF_SIC_METHOD_TYPE_OPTIONS		0x01
#define	ICF_SIC_METHOD_TYPE_MESSAGE		0x02
#define	ICF_SIC_METHOD_TYPE_SUBSCRIBE	0x04
#define	ICF_SIC_METHOD_TYPE_PUBLISH		0x08
#define	ICF_SIC_REQUEST_WITHIN_DIALOG	0x10
/* This bitmask will be set when Poll subscribe request
 * was sent
 */
#define ICF_SIC_POLL_SUBSCRIBE_REQ		0x20
	
#define ICF_SIC_METHOD_TYPE_OOD_NOTIFY		0x40
#define ICF_SIC_EXPIRES_HDR_NOT_TO_BE_SEND	0x80
#define ICF_SIC_RESUBSCRIPTION_NOT_REQUIRED	0x100

/* This bit will be used to stop subscription if server sends 401 again 
 * in response to the re-sent SUBSCRIBE with credentials, ie. if 401 is 
 * received twice for the same SUBSCRIBE instead of a success/failure response
 */
/* This bit should be set on getting 401 and reset on getting non-401/407 
 * final response from the server for the same subscribe request
 */
#define ICF_SIC_SUBS_ALREADY_CHALLENGED_ONCE   0x200

/*Following bit indicate that SUBSCRIBE request is initiated by XCONF module
 * and this is not externally generate by application*/
#define ICF_SIC_INTERNAL_SUBSCRIBE_REQUEST     0x400

/*This bit will be set when we have already sent SUBSCRIBE on the network
 * after the decode failure of 200 OK of iinitial SUBSCRIBE because of 
 * mandatory contact headre was missing in it.*/

#define ICF_SIC_SUBSCRIBE_ALREADY_RETRIED      0x800

/*This bit is used for specifying that the notify is a special one which is 
  given to APP only because it indicated in the set_self_id by setting
  ICF_SUPPORT_OUT_OF_DIALOG_NOTIFY_IGNORE_DIALOG_INFO
 */
#define	ICF_SIC_METHOD_TYPE_STRAY_NOTIFY         0x1000

/*This bit is used to indicate that the subscribe request for reg event package
 * is triggered internally by IPTK .This bit is set in regm module ,where the
 * sic context for reg event package has been allocated after the success
 * respose for register request has been received(in function icf_rgm_send_subs
 * _on_succ ).*/    
#define ICF_SIC_AUTO_SUBSCRIPTION_FOR_REG_EVENT   0x2000 

/*  SPR 18064 Merge */
#define ICF_SIC_AUTH_PARAM_NOT_RCVD               0x4000

/* 
   The following bit is set when ICF_NOTIFY_IND has been successfully given to the
   application. This field is applicable only when application has enabled support
   of asynchronous NOTIFY.
 */
#define ICF_SIC_NOTIFY_IND_SENT_TO_APP            0x8000

/* 
   The following bit is set when an incoming NOTIFY has been successfully responded
   to. This field is applicable only when application has enabled support of
   asynchronous NOTIFY.
 */
#define ICF_SIC_NOTIFY_RESP_SENT_TO_PEER          0x10000

/* 
   The following bit is set in various Subscribe FSM functions that are hit on
   receipt of NOTIFY from peer. This bit is set to indicate that the SIC
   context has to be deallocated in new Subscribe FSM state 05 and not in the
   FSM function where this bit is set.This field is applicable only when
   application has enabled support of asynchronous NOTIFY.
 */ 
#define ICF_SIC_CONTEXT_TO_BE_DEALLOCATED         0x20000

/* 
   The following bit is set when a NOTIFY for SUBSCRIBE is being processed and
   is reset when that NOTIFY has been successfuuly responded to.This field is
   applicable only when application has enabled support of asynchronous NOTIFY.
*/
#define ICF_SIC_NOTIFY_PROCESSING_ONGOING         0x40000
 /* 
   The following bit is set when a MESSAGE request  is being processed and
   is reset when that MESSAGE request  has been successfuuly responded to.This field is
   applicable only when application has enabled support of asynchronous MESSAGE.
*/   
#define ICF_SIC_MESSAGE_REQ_ONGOING  0x80000/*bitmask*/

/* This bit is indicates whether the subs_hs_duration has been
 * configured or not.
 */ 
#define ICF_SIC_SUBS_HEAD_START_DURATION_RECVD   0x100000
/* Fix for SPR: 19999 (Merged CSR 1-7615966 : SPR 20030)*/
/* This bitmask is set if the bitmask ICF_SUB_REQ_RESUBSCRIPTION_NOT_REQUIRED
 * is set in incoming API ICF_SUBSCRIBE_REQ for initial subscription.
 */
#define ICF_SIC_SUBS_RETRY_NOT_REQUIRED          0x200000
/* This bitmask is set if the bitmask
 * ICF_SUB_REQ_RESUBSCRIPTION_NOT_REQUIRED_FOR_3XX
 * is set in incoming API ICF_SUBSCRIBE_REQ for initial subscription.
 */
#define ICF_SIC_SUBS_RETRY_NOT_REQUIRED_FOR_3XX  0x400000
    icf_uint32_t		bitmask;
	/* call identifier */
    icf_call_id_t            call_id;

    icf_call_ctx_st			*p_call_ctx;


    /* line identifier on which call is running */
    icf_line_id_t            line_id;

    /* Buffer for the event to be processed */
    icf_boolean_t    event_buffer;

    /* State of SIC */
    icf_uint8_t      sic_state;
    
    /* timer type of the timer */
    icf_timer_type_t timer_type;
    
    /* Duration of subscription */
    icf_duration_t   duration;
   
    /* Original duration of subscription received from the app in subs_req */
    icf_duration_t   app_requested_duration;	
   
    /* Name of the event package */
    icf_string_st      event_package;
    
    /* Timer id of the timer running */
    icf_timer_id_t   timer_id;
   
    /* Pointer to the app ctx received in Requests.
     * This depends on the event package 
     */
    icf_void_t       *p_app_ctx; 
    
    /* pointer to the ssa ctx */
    icf_void_t       *p_ssa_ctx;
    
    /* Timer data pointer */
    icf_timer_data_st    *p_timer_data;
    
    /* This is the user address from the line data. 
     * This can be SIP/ TEL/ E.164
     * This will be send in the "to" and "from" field in the Subscribe Request
     */
    icf_address_st   *p_user_address;
    
    /* It can be registrar address in case of reg and the SUBSCRIPTION server address
     * in case of other event pkgs. In  case of MESSAGE/PUBLISH/OPTIONS this is 
	 * the destination address
     */
	icf_address_st    dest_addr;

#ifdef ICF_TRACE_ENABLE
         icf_address_st    source_addr;
#endif

	/* preferred identity of the local user */
    icf_address_st    *p_preferred_identity;

    icf_app_id_t      app_id;

	icf_address_st	 refer_to;

	icf_boolean_t		 subs_reqd;

	icf_string_st	 entity_tag;
	
	/* List of additional headers that the Calling Module wants to send in the
     * request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the the Calling Module wants to send in the 
     * request.
     */
    icf_tag_list_st	   tag_list;

	icf_msg_body_list_st body_list;

   	icf_header_list_st	persistent_header_list;

    /* this is the error code send to app in case of an invalid request */
    icf_uint32_t         error_code;
	/* p_sdp_list contains the list of streams received in SDP from remote
	 * which contains node of type icf_inc_media_sdp_info_st */
	icf_list_st			*p_sdp_list;

	/* ICF_6_1_OPTIONS_SUPPORT start */

        /* OPTIONS timer id */
        icf_timer_id_t       options_timer_id;
        /* OPTIONS timer data */
        icf_timer_data_st    *p_options_timer;

	/* Media Capabilities of Media Manager */
	icf_list_st	*p_mm_media_capab;
	
	/* Pending events */
#define ICF_SIC_PENDING_CLEAR_OPTIONS_CTX 0x1
	icf_uint8_t	pending_event;


	/* ICF_6_1_OPTIONS_SUPPORT end */
#ifdef ICF_TRACE_ENABLE
        icf_trace_criteria_data_st *p_trace_criteria_data;
#endif

    /*This field will be populated by SSA if retry-after header is received 
     *from remote in any failure response and module which is sending failure 
     *response to application will mark it NULL after populating this duration 
     *in corresponding API payload*/
    icf_duration_t   retry_after_duration;
    icf_uint8_t      method;
    /*This is set to True when only marking of SIC ctx 
      to NULL is required within SSA ctx */
    icf_boolean_t    detach_sic_ctx;
    
    /* Rel 8.1.0 changes
     * This boolean contains the value of the tunnel mode 
     * It can be filled in 2 ways:
     * 1: when out of dialog options_req received with tunnel mode
     * 2: when early_session_cfm received from application 
     */
    icf_boolean_t    tunnel_mode;

    /*  This will be set when Options received and SDP is required in 
         200 OK */
    icf_boolean_t    is_sdp_reqd;
    /* ASYNCHRONUS_MESSAGE enhancements  */
    /* MESSAGE timer id */
    icf_timer_id_t       message_cfm_timer_id;
    /* MESSAGE timer data */
    icf_timer_data_st    *p_message_cfm_timer_data;



    /* 
       This field stores the previous SIC Subscribe FSM state. This field
       is used to store the current state whenever a NOTIFY is received
       and flow moves to the new state ICF_SIC_05_AWAIT_NOTIFY_CFM. This
       field is applicable only when application has enabled support
       of asynchronous NOTIFY. In the new state handler functions,this 
       stored state is used to restore the call state.
    */
    icf_uint8_t prev_state;

    /* This field stores the pending event. This is used in the new state 05
       of Subscribe FSM to handle pending events.
    */
    icf_uint8_t subs_fsm_pending_event;

    /*
       This field stores the previous SIC Subscribe FSM event. This field
       is used to store the current event whenever a NOTIFY is received
       (with events ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM or
       ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER) 
       and flow moves to the new state ICF_SIC_05_AWAIT_NOTIFY_CFM. This
       field is applicable only when application has enabled support
       of asynchronous NOTIFY. In the new state handler functions,this 
       stored event is used to do the handling that the calling handler 
       function performed.
       For eg: If in state 02, event ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM 
       is received, then the handler function icf_sic_02_notify_term_retry_imm
       delinks SSA from SIC and sends a new SUBSCRIBE request. Now in this function
       we will store the event ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM in this 
       field and use it in state 05 to perform the above mentioned handling.
    */  
    icf_uint8_t prev_event;

    /* NOTIFY_CFM timer id */
    icf_timer_id_t       notify_cfm_timer_id;

    /* NOTIFY_CFM timer data */
    icf_timer_data_st    *p_notify_cfm_timer_data;

    /* The following field will be used when application has enabled support
       of asynchronous NOTIFY. Whenever the control is in ICF_SIC_05_AWAIT_NOTIFY_CFM
       state and a request is received from the application or from the peer,
       the same is marked as a pending event.This field will store the request
       received from the application or from the peer. Currently the same is
       stored in p_sic_pdb->p_msg whenever SIC is trigerred. We will make a
       copy of the internal message received in this new field and will use
       the same while processing any pending events in the state
       ICF_SIC_05_AWAIT_NOTIFY_CFM. 
    */   
    icf_void_t *p_pending_req;

    /* 
      If the application has enabled support of asynchronous NOTIFY,
      we will take a backup of the HTB lists received from the application.
      These will be used in state ICF_SIC_05_AWAIT_NOTIFY_CFM.Since
      the application can provide HTB lists in ICF_NOTIFY_CFM,
      the HTB lists received in a Subscribe request can be overwritten.
      We will take a backup of the same and will use it to restore the HTB list
      in the SIC context (after processing the NOTIFY confirmation).
    */
    icf_header_list_st   header_list_backup;
    icf_tag_list_st	     tag_list_backup;
	icf_msg_body_list_st body_list_backup;
    
    /* subscription head start duration per subscription.
     * This field is updated whenever API ICF_SUBSCRIBE_REQ is received
     * with subs_heade_start_duration*/
    icf_duration_t   subs_hs_duration; 
    
    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    /* This field is a List of headers that the application wants 
     * to not send in the out of dialog OPTIONS request.all headers 
     * are saved as a string buffer and seperated by "<SP><comma>".
     */
    icf_uint8_t      *p_remove_hdrs;
    
}icf_sic_ctx_st;


/*------------------------------------------------------------------------------
*
* Name : icf_sic_options_req_st
*
* Description  : This structure defines the internal message data when
*                Other Module Want to send OPTIONS
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    /* Use common hash defines if header/msg-body/tag list present */
    icf_uint32_t            bit_mask;
   
    /* This bit indicates presence of optional field tunnel_mode */ 
    #define   ICF_SIC_OPTIONS_TUNNEL_MODE_OPERATION_PRESENT   0x01
    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    /* This bitmask is set when Application mentions the list of headers
     * which need not to be send on network in OPTIONS request.
     */ 
    #define   ICF_OPTIONS_REQ_REMOVE_HDR_LIST_PRESENT         0x02
    
    icf_call_id_t           call_id;

    icf_app_id_t            app_id;
	
    /* Points to the Key passed by calling module which will be returned
     * in response. */
     icf_void_t              *p_key;

    /* Indicates the SIP URL of the remote user to which MESSAGE request has
     *  to be sent. */
    icf_address_st          destination_info;

    /* line id for sending OPTIONS */
    icf_line_id_t           line_id;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st          pref_identity;

    /* List of additional headers that the Calling Module wants to send in the
     * OPTIONS request
     */
    icf_header_list_st      header_list;

    /* List of additional tags that the the Calling Module wants to send in the 
     * OPTIONS request.
     */
    icf_tag_list_st         tag_list;

    /* message body list */
    icf_msg_body_list_st    body_list;
   
    /* Rel 8.1.0 changes
     * This boolean contains the value of the tunnel mode 
     * as it was received in ICF_OPTIONS_REQ from application
     */ 
    icf_boolean_t           tunnel_mode; 
    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    /* This field is a List of additional headers that the application wants 
     * to not send in the out of dialog OPTIONS request.
     */
    icf_header_name_list_st remove_hdr_list;

} icf_sic_options_req_st;


/* ICF_6_1_OPTIONS_SUPPORT start */

/*-----------------------------------------------------------------------------
*
* Name : icf_sic_clear_options_ctx_st
*
* Description  : This structure defines the internal message data when
*                ssa sends ICF_SIC_CLEAR_OPTIONS_CTX
*
*----------------------------------------------------------------------------*/
typedef struct
{
	icf_sic_ctx_st     *p_sic_ctx;
} icf_sic_clear_options_ctx_st;

/*-----------------------------------------------------------------------------
*
* Name : icf_sic_options_cfm_st
*
* Description  : This structure defines the internal message data when
*                Application sends icf_options_cfm_st
*
*----------------------------------------------------------------------------*/
typedef struct
{
    
	icf_call_id_t	call_id;

	icf_app_id_t	app_id; 
	
    icf_line_id_t	line_id;
    #define ICF_SIC_OPTIONS_FAILURE_RESP_PRESENT      0x01

    icf_uint8_t        bitmask;

	/* the application can decide to accept or reject a call following the
	 * OPTIONS request that has been received. If the application returns
	 * failure in the CFM, ICF will respond to the received OPTIONS with a 
	 * 486 */
	icf_result_t			result;

	/* List of additional headers that the application wants to send in the
     * OPTIONS request
     */
    icf_header_list_st 		header_list;

    /* List of additional tags that the application wants to send in the 
     * OPTIONS request.
     */
    icf_tag_list_st 		tag_list;

    /* List of message bodies that the application wants to send in the
     * OPTIONS request.
     */
    icf_msg_body_list_st 	body_list;

	/* This is a list of the node type icf_stream_capab_st */ 
	/* This structure contains capabilities modified by the application
	 * If Incoming OPTIONS contains Accept Header with value as application/sdp, 
	 * then this is set and contains SDP that is to be sent in 200ok.
	 * Application can modify the attributes of the streams and can add only 
	 * encoded attributes of the stream. However, it can add only application 
	 * based streams. 
     * The application can add a media stream to this list only if the media stream
     * is already configured by the application in the media profile in app_conf
     * during ICF_APP_ADD 
	 */
     icf_list_st			*p_app_sdp_list;

	/* the application can reject a complete media stream it it desires */
	icf_uint8_t				count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
    icf_uint32_t                    response_code;


} icf_sic_options_cfm_st;


/* ICF_6_1-OPTIONS_SUPPORT end */
/*------------------------------------------------------------------------------
*
* Name : icf_sic_publish_req_st
*
* Description  : This structure defines the internal message data when
*                Other Module Want to send PUBLISH
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    /* code change for CSR_1-4816176/SPR 16075 - not tested */  
    icf_uint32_t            bit_mask;
    
	icf_call_id_t           call_id;

	icf_app_id_t			app_id; 

	icf_string_st			event_package;

	icf_uint32_t			duration;
	
	/* Points to the Key passed by calling module which will be returned
     * in response. */
    icf_void_t				*p_key;

    /* Indicates the SIP URL of the remote user to which PUBLISH request has
     *  to be sent. */
    icf_address_st destination_info;

    /* line id for sending PUBLISH */
    icf_line_id_t			line_id;
    
   /* User id */
    icf_address_st			user_address;
   
    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st			pref_identity;

    /* List of additional headers that the Calling Module wants to send in the
     * PUBLISH request
     */
    icf_header_list_st		header_list;

    /* List of additional tags that the the Calling Module wants to send in the 
     * PUBLISH request.
     */
    icf_tag_list_st			tag_list;

	icf_msg_body_list_st		body_list;


} icf_sic_publish_req_st;

/*------------------------------------------------------------------------------
*
* Name : icf_sic_message_req_st
*
* Description  : This structure defines the internal message data when
*                Other Module Want to send MESSAGE
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    /*code change for CSR_1-4816176/SPR 16075 - not tested */ 

    icf_uint32_t bit_mask;
    /* Use common hash defines if header/msg-body/tag list present */

	icf_call_id_t            call_id;

	icf_app_id_t app_id; 
	
	/* Points to the Key passed by calling module which will be returned
     * in response. */
    icf_void_t  *p_key;

    /* Indicates the SIP URL of the remote user to which MESSAGE request has
     *  to be sent. */
    icf_address_st destination_info;

    /* line id for sending MESSAGE */
    icf_line_id_t line_id;
  
   /* user id */
    icf_address_st user_address;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st pref_identity;

    /* List of additional headers that the Calling Module wants to send in the
     * MESSAGE request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the the Calling Module wants to send in the 
     * MESSAGE request.
     */
    icf_tag_list_st tag_list;

	icf_msg_body_list_st body_list;


} icf_sic_message_req_st;

/*------------------------------------------------------------------------------
*
* Name : icf_sic_message_req_st
*
* Description  : This structure defines the internal message data when
*                Other Module Want to send MESSAGE
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    
	icf_call_id_t            call_id;

	icf_app_id_t app_id; 

	/* Referred To */
  	icf_address_st refer_to;

	icf_boolean_t subs_reqd;
	
	/* Points to the Key passed by calling module which will be returned
     * in response. */
    icf_void_t  *p_key;

    /* Indicates the SIP URL of the remote user to which REFER request has
     *  to be sent. */
    icf_address_st destination_info;

    /* line id for sending REFER */
    icf_line_id_t line_id;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st pref_identity;

    /* List of additional headers that the Calling Module wants to send in the
     * REFER request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the the Calling Module wants to send in the 
     * REFER request.
     */
    icf_tag_list_st tag_list;

	icf_msg_body_list_st body_list;


} icf_sic_refer_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_clear_ctx_st
 *
 * Description:     This is send from MWI to SIC to send Subscribe Request.
 *
 * ---------------------------------------------------------------------------*/
typedef struct 
{
    /* ICF_EVENT_PKG_MESSAGE_SUMMARY or ICF_EVENT_PKG_REG */
    icf_string_st      event_package;

	icf_call_id_t            call_id;

	icf_app_id_t app_id; 

		/* Points to the Key passed by calling module which will be returned
     * in response. */
    icf_void_t  *p_key;

} icf_sic_clear_ctx_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_subs_req_st
 *
 * Description:     This is send from MWI to SIC to send Subscribe Request.
 *
 * ---------------------------------------------------------------------------*/

typedef struct 
{
    icf_uint32_t     bitmask;
    /* ICF_EVENT_PKG_MESSAGE_SUMMARY or ICF_EVENT_PKG_REG */
    icf_string_st      event_package;

	icf_uint32_t      duration;

	icf_call_id_t            call_id;

	icf_app_id_t app_id; 

		/* Points to the Key passed by calling module which will be returned
     * in response. */
    icf_void_t  *p_key;

    /* Indicates the SIP URL of the remote user to which SUBSCRIBE request has
     *  to be sent. */
    icf_address_st destination_info;

    /* line id for sending SUBSCRIBE */
    icf_line_id_t line_id;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st pref_identity;

    /* List of additional headers that the Calling Module wants to send in the
     * SUBSCRIBE request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the the Calling Module wants to send in the 
     * SUBSCRIBE request.
     */
    icf_tag_list_st tag_list;

	icf_msg_body_list_st body_list;
   
    /* subscription head start duration for current subscription.*/
    icf_duration_t   subs_head_start_duration;    
    
} icf_sic_subs_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_unsubs_req_st
 *
 * Description:     This is send from MWI to SIC to send Unsubscribe Request.
 *
 * ---------------------------------------------------------------------------*/
typedef struct
{
	icf_call_id_t            call_id;

     /* Flag to indicate whether the request is for forceful delete */
    icf_boolean_t    forceful_delete;
    
} icf_sic_unsubs_req_st;


/*-----------------------------------------------------------------------------
 *
 * Name : icf_delink_app_ctx_st
 *
 * Description  : This is send from SIC to SSA to delink the applictaion ctx.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* This flag will be true in case of forceful else it will be false */
    icf_boolean_t forceful_delete;
    
    /* Indicates the module of which the pointer is present */
    icf_int_module_id_t  module_id;
    /* Pointer to the SIC Context
     */
    icf_void_t   *p_sic_ctx;
} icf_delink_app_ctx_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_subs_state_st
 *
 * Description:     This is send from SIC to REG/APP to tell the subscription 
 *                  status.
 *
 * ---------------------------------------------------------------------------*/

typedef struct
{
	icf_call_id_t            call_id;

	icf_uint32_t		resp_code;
    /* There will be 4 states as defined in the display ind API in the 
     * external interface.
     */
    icf_uint8_t  subs_state;

    /* Event package */ 
    icf_string_st      event_package;

	icf_duration_t   duration;
   
    /* Line id */ 
    icf_line_id_t    line_id; 

	icf_app_id_t   		app_id;
    
     /* This pointer will be the pointer to app specific data
     */
    icf_void_t  *p_subs_key;

	icf_header_list_st	header_list;

    
} icf_sic_subs_state_st;


/*-----------------------------------------------------------------------------
*
* Name : icf_sic_options_status_st
*
* Description  : This structure defines the internal message data when
*                SIC received OPTIONS response 
*
*-----------------------------------------------------------------------------*/
typedef struct
{
#define ICF_SIC_OPTIONS_SUCCESS      1
#define ICF_SIC_OPTIONS_FAILED       2
#define ICF_SIC_OPTIONS_TIMEDOUT     3
    
    /* Response Specifies whether options status has been succeeded or
     * failed or timed out */
    icf_uint8_t    response;

	icf_call_id_t        call_id;

    /* SIP Specific reponse code as received from network */
	icf_uint32_t			resp_code;

	/* Line id */ 
    icf_line_id_t		line_id; 

	icf_app_id_t   		app_id;

    /* Points to the Key which was passed by calling module and returned
     * in response. */
    icf_void_t			*p_key;
    
    icf_header_list_st	header_list;
    
    icf_tag_list_st		tag_list;
    
    icf_msg_body_list_st	body_list;

} icf_sic_options_status_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_publish_status_st
 *
 * Description:     This is send from SIC to APP to tell the publication 
 *                  status.
 *
 * ---------------------------------------------------------------------------*/

typedef struct
{
	icf_call_id_t        call_id;
    
    /* Line id */ 
    icf_line_id_t		line_id; 
	
	icf_app_id_t   		app_id;
    
    icf_uint32_t			resp_code;

    /* Event package */ 
    icf_uint8_t			event_package;

	icf_duration_t		duration;
   
      
     /* This pointer will be the pointer to app specific data
     */
    icf_void_t			*p_key;

	icf_header_list_st	header_list;

    
} icf_sic_publish_status_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_message_status_st
 *
 * Description:     This is send from SIC to APP to tell the message 
 *                  status.
 *
 * ---------------------------------------------------------------------------*/

typedef struct
{
	icf_call_id_t        call_id;
    
    /* Line id */ 
    icf_line_id_t		line_id; 
	
	icf_app_id_t   		app_id;

	icf_uint32_t			resp_code;

    
     /* This pointer will be the pointer to app specific data
     */
    icf_void_t			*p_key;

	icf_header_list_st	header_list;

    
} icf_sic_message_status_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_refer_status_st
 *
 * Description:     This is send from SIC to APP to tell the refer 
 *                  status.
 *
 * ---------------------------------------------------------------------------*/

typedef struct
{
	icf_call_id_t        call_id;

	/* Line id */ 
    icf_line_id_t		line_id; 

	icf_app_id_t   		app_id;
    
    icf_uint32_t			resp_code;

    
    
     /* This pointer will be the pointer to app specific data
     */
    icf_void_t			*p_key;

    icf_header_list_st	header_list;
    
} icf_sic_refer_status_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_notify_msg_st 
 *
 * Description:     This is send from SIC to MWI to tell the NOTIFY message 
 *                  received.
 *
 * ---------------------------------------------------------------------------*/

typedef struct 
{
	icf_call_id_t        call_id;

    /* Line id */ 
    icf_line_id_t		line_id;

	icf_app_id_t   		app_id;

    /* This pointer will be the pointer to the APP context
     */
    icf_void_t			*p_subs_key;

	icf_uint32_t			duration ;

	/* Package name for which the notification is received. */

    icf_string_st				event_pkg;

	icf_uint8_t      subs_state;

	icf_address_st   *p_user_address;

	icf_address_st   *p_sender_address;

    /* Reason code is present in case of terminated state */
    icf_uint8_t      reason_code;

	icf_boolean_t	 out_of_dlg_mesg;
   
    icf_header_list_st	header_list;
    
    icf_tag_list_st		tag_list;

	icf_msg_body_list_st body_list;

    
} icf_sic_notify_msg_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_sic_notify_msg_st 
 *
 * Description:     This is send from SIC to MWI to tell the NOTIFY message 
 *                  received.
 *
 * ---------------------------------------------------------------------------*/

typedef struct 
{
	icf_call_id_t			call_id;
    /* Line id */ 
    icf_line_id_t			line_id;

	icf_app_id_t   			app_id;

    /* This pointer will be the pointer to the APP context
     */
    icf_void_t				*p_key;

   
    icf_header_list_st		header_list;
    
    icf_tag_list_st			tag_list;

	icf_msg_body_list_st		body_list;

    
} icf_sic_msg_ind_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_subs_req_st
 *
 * Description  : This is send from SIC to SSA to send Subscribe Request
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_otg_subs_req_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_msg_req_st
 *
 * Description  : This is send from SIC to SSA to send MESSAGE Request
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_otg_msg_req_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_publish_req_st
 *
 * Description  : This is send from SIC to SSA to send PUBLISH Request
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_otg_publish_req_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_opt_req_st
 *
 * Description  : This is send from SIC to SSA to send OPTIONS Request
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_sic_ctx_st  *p_sic_ctx;
    icf_boolean_t   is_option_in_dlg;
} icf_nw_otg_opt_req_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_refer_req_st
 *
 * Description  : This is send from SIC to SSA to send REFER Request
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
          icf_uint8_t           bitmask;
      /* This bitmask represents that the replace call id has to be sent
       * in the INVITE with the nw call id for the given internal call id. */
#define ICF_NW_OTG_REFER_RELACE_CALL_ID  0x01
      /*This bitmask indicates that the SSA has to add a referred by header
       * in outgoing REFER.  SSA will use the information from from header data
       * for making referred by Header. SLP will set this flag if it decides
       *that presentation is not restricted*/
#define ICF_NW_OTG_REFER_REF_BY          0x02
     
#define ICF_REFER_TO_ADDNL_URI_PARAMTR_PRES 0x04
#define ICF_NW_OTG_REFER_METHOD_BYE      0x08

 
      icf_call_id_t         replace_call_id;
      /* This memory will be allocated and freed by CTA/CTU SLP */
      icf_address_st        *p_referred_to;

      icf_sic_ctx_st  *p_sic_ctx;

      icf_large_string_st *p_uri_parameter;

} icf_nw_otg_refer_req_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_options_req_st
 *
 * Description  : This is send from SIC to SSA to send Options Request
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_sic_ctx_st  *p_sic_ctx;
    icf_boolean_t    is_option_in_dlg;
} icf_nw_otg_options_req_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_options_resp_st
 *
 * Description  : This is sent from SIC to SSA to send OPTIONS Response
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* ICF_SUCCESS or ICF_FAILURE */
    icf_result_t     result;

    /* Pointer to the SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;

} icf_nw_otg_options_resp_st;
/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_nw_inc_opt_ind_st   
 *
 * Description:     This is send from SSA to SIC when OPTIONS is received.
 *
 * ---------------------------------------------------------------------------*/
typedef struct
{
    /*  Rel 8.1.0 changes */
    icf_uint8_t bitmask;
    /* this bit is used by SIC when sender address dest_info present */
    #define ICF_NW_INC_OPTIONS_SENDER_PRESENT        0x01

    /* this bit is used by SIC when called party address present */  
    #define ICF_NW_INC_OPTIONS_USER_ADDR_PRESENT     0x02
 
    /* This flag is TRUE, when Accept header contains application/sdp
     * otherwise FALSE.
     */
    icf_boolean_t    is_sdp_reqd;

    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_inc_opt_ind_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_nw_inc_opt_cfm_st   
 *
 * Description:     This is send from SIC to SSA when response for incoming OPTIONS
 *				    is to be sent on the network.	
 *
 * ---------------------------------------------------------------------------*/
typedef struct
{
	/* Use common hash defines if header/msg-body/tag list present */
	icf_uint32_t 			bit_mask;
#define ICF_OPTIONS_CFM_MEDIA_PRESENT		0x01
#define ICF_OPTIONS_CFM_ERROR_PRESENT		0x02
	
	icf_uint32_t	error_code;

	/* List of additional headers that the application wants to send in the
     * OPTIONS request
     */
    icf_header_list_st 		header_list;

    /* List of additional tags that the application wants to send in the 
     * OPTIONS request.
     */
    icf_tag_list_st 		tag_list;

    /* List of message bodies that the application wants to send in the
     * OPTIONS request.
     */
    icf_msg_body_list_st 	body_list;

	/* This is a list of the node type icf_stream_capab_st */ 
	/* This list contains the final capabilities to be sent on the network in 
	 * response to the OPTIONS request.
	 * This list contains the streams modified by the application and in case, application
	 * doesnt specify any stream, then that stream is taken from MM capability response.
	 * Moreover,if application deletes any stream then it shouldnt be sent on the network.
	 * 
	 * All the streams are sent with PORT=0.
	 */
	icf_list_st			*p_sdp_list;

	icf_sic_ctx_st	*p_sic_ctx;
	
	
} icf_nw_inc_opt_cfm_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_notify_resp_st
 *
 * Description  : This is send from SIC to SSA to send NOTIFY Response
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* ICF_SUCCESS or ICF_FAILURE */
    icf_result_t     result;

	icf_uint32_t	response_code;

    /* Pointer to the SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;

} icf_nw_otg_notify_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_otg_message_resp_st
 *
 * Description  : This is send from SIC to SSA to send MESSAGE Response
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t     bit_mask;
    /* ICF_SUCCESS or ICF_FAILURE */
    icf_result_t     result;

    /* Pointer to the SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;
    /* New Code for ASYNCHRONUS_MESSAGE enhancements*/
    icf_uint16_t    response_code ;

    icf_header_list_st header_list;

    icf_tag_list_st  tag_list;
} icf_nw_otg_message_resp_st;


/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_inc_subs_resp_st
 *
 * Description  : This is send from SSA to SIC when response for subscribe
 *                  is received or no response is received after
 *                  re-transmission.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
#define ICF_NW_INC_SUBS_RESP_SUCCESS 1
#define ICF_NW_INC_SUBS_RESP_NO_RETRY 2
#define ICF_NW_INC_SUBS_RESP_WITH_RETRY 3

    icf_uint8_t    response;

    #define ICF_NW_INC_SUBS_DURATION_PRESENT 0x01
    icf_uint8_t      bitmask;
    
    /* In case of success response - This will be the actual duration of 
     * subscription.
     * In case of failure response - This will be the retry after duration.
     */
    icf_duration_t   duration;

	icf_uint32_t    resp_code;

    /* Pointer to SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_inc_subs_resp_st;


/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_inc_notify_ind_st
 *
 * Description  : This is send from SSA to SIC to give the NOTIFY message
 *                received.
 *----------------------------------------------------------------------------*/
typedef struct
{
#define ICF_SUBS_STATE_ACTIVE 1
#define ICF_SUBS_STATE_TERMINATED 0

    /* This is the subscription state received in notify */
    icf_uint8_t      subs_state;

#define ICF_NW_INC_NOTIFY_RETRY_IMM 1
#define ICF_NW_INC_NOTIFY_RETRY_AFTER 2
#define ICF_NW_INC_NOTIFY_NO_RETRY 3

    /* Reason code is present in case of terminated state */
    icf_uint8_t      reason_code;

    /* This duration can be the remaining subscription duration
     * in case of active state and retry after duration in case of
     * terminated state
     */
    icf_duration_t   duration;

    /* bitmamsk for duration ICF_NW_INC_SUBS_DURATION_PRESENT */
#define ICF_NW_INC_NOTIFY_OUT_OF_DIALOG		0x20
#define ICF_NW_INC_NOTIFY_SENDER_PRESENT	0x40
#define ICF_NW_INC_NOTIFY_USER_ADDR_PRESENT	0x80
    icf_uint8_t      bitmask;

    /* Pointer to the SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;

    icf_uint8_t   status;

    /* The message body on the basis of event package
    icf_nw_inc_notify_ut message_body;*/
} icf_nw_inc_notify_ind_st;

/*------------------------------------------------------------------------------
*
* Name : icf_nw_inc_info_resp_st
*
* Description  : This structure defines the internal message data when
*                 SSA receives info esponse from Network.
*
*-----------------------------------------------------------------------------*/
typedef struct
{
    icf_boolean_t      status;
    icf_ssa_reason_t    error_cause;
    icf_uint32_t		response_code;
} icf_nw_inc_info_resp_st;


/*-----------------------------------------------------------------------------
*
* NAME : icf_nw_otg_info_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_NW_OTG_INFO_REQ.
-----------------------------------------------------------------------------*/

typedef struct
{
      /*This bitwise field contains presence information*/

#define ICF_OTG_INFO_PAYLOAD_PRESENT    0x01
/*  CSR 1-5576525 Merge */
#define ICF_OTG_INFO_USERNAME_PRESENT   0x02

      icf_uint8_t  presence_flag ;

    /* Contents of message body to be sent in INFO request  */
    icf_string_st        info_payload;

    /* Type of the contents of message body to be sent in INFO request  */
    icf_string_st  content_type;
    /* CSR 1-5576525 Merge */
    /* CSR 1-5576525: Store the username received in info request from 
     * application.
     */
    icf_string_st   username;


} icf_nw_otg_info_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_nw_inc_info_cfm_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_NW_INC_INFO_CFM
-----------------------------------------------------------------------------*/
typedef struct
{
    
    /* Response code to be sent for the INFO request. */
    icf_uint32_t 		response_code ;

    /* bit_mask field will tell whether header,tag and body list are there or not.*/  
    icf_uint8_t                 bit_mask; 
#define ICF_NW_INC_INFO_CFM_HEADER_LIST_PRESENT          0x04
#define ICF_NW_INC_INFO_CFM_TAG_LIST_PRESENT             0x08
#define ICF_NW_INC_INFO_CFM_BODY_LIST_PRESENT            0x10

    icf_header_list_st		header_list;
    icf_tag_list_st		tag_list;
    icf_msg_body_list_st	body_list;

} icf_nw_inc_info_cfm_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_nw_inc_msg_ind_st   
 *
 * Description:     This is send from SSA to SIC and SIC to MWI when MESSAGE 
 *                  is received.
 *
 * ---------------------------------------------------------------------------*/
typedef struct
{
	icf_sic_ctx_st	*p_sic_ctx;
} icf_nw_inc_msg_ind_st;


/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_inc_opt_resp_st
 *
 * Description  : This is send from SSA to SIC when response for OPTIONS
 *                  is received or no response is received after
 *                  re-transmission.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t    response;

    /* Pointer to SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_inc_opt_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_inc_msg_resp_st
 *
 * Description  : This is send from SSA to SIC when response for MESSAGE
 *                  is received or no response is received after
 *                  re-transmission.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t    response;

    /* Pointer to SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_inc_msg_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_inc_publish_resp_st
 *
 * Description  : This is send from SSA to SIC when response for PUBLISH
 *                  is received or no response is received after
 *                  re-transmission.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t    response;

	icf_duration_t   duration;

    /* Pointer to SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_inc_publish_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_inc_refer_resp_st
 *
 * Description  : This is send from SSA to SIC when response for REFER
 *                  is received or no response is received after
 *                  re-transmission.
 *                 If used for CTA/CTU service, status and error_cause will
 *                 be used
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t   status;

    icf_ssa_reason_t   error_cause;

    icf_uint32_t    response;

    /* Pointer to SIC ctx */
    icf_sic_ctx_st  *p_sic_ctx;
} icf_nw_inc_refer_resp_st;


#ifdef ICF_SIGCOMP_SUPPORT
/*------------------------------------------------------------------------------
*
* Name : icf_secure_conn_up_ind_st
*
* Description  : This structure defines the internal message data to indicate
*                SIGSA that the secure connection is up. when
*                RGM receives First Secured response from Network It invokes
*                ICF_SECURE_CONN_UP_IND interface of sigsa
*
*-----------------------------------------------------------------------------*/
typedef struct
{
   icf_rgm_context_st   *p_rgm_ctxt;
} icf_secure_conn_up_ind_st;

/*------------------------------------------------------------------------------
*
* Name : icf_create_comp_req_st
*
* Description  : This structure defines the internal message data to request
*                SIGSA for creating a compartment.ICF_CREATE_COMP_REQ 
*                interface is from RGM to SIGSA.
*-----------------------------------------------------------------------------*/
typedef struct
{
   icf_rgm_context_st   *p_rgm_ctxt;
} icf_create_comp_req_st;


/*------------------------------------------------------------------------------
*
* Name : icf_create_comp_resp_st
*
* Description  : This structure defines the internal message data as a response
*                to Create Compartment request. This interface is from SIGSA to
*                RGM.If SIGSA indicates to RGM to Wait in the Create Compartment
*                request, Then only SIGSA invokes this Interface 
*                ICF_CREATE_COMP_RESP
*-----------------------------------------------------------------------------*/
typedef struct
{
    /* Values used to indicate the create compartment response */
#define ICF_SIGSA_CREATE_COMP_FAILED      0
#define ICF_SIGSA_CREATE_COMP_SUCCEED     1
    
    icf_uint8_t          response;
    /* RGM context as indicated in the icf_create_comp_req */    
    icf_rgm_context_st   *p_rgm_ctxt;
} icf_create_comp_resp_st;

/*------------------------------------------------------------------------------
*
* Name : icf_delete_comp_ind_st
*
* Description  : This structure defines the internal message data to indicate
*                SIGSA that delete the compartment associated with the
*                Registred Context. when
*                RGM response from Network for the deregister Request
*                It invokes
*                ICF_DELETE_COMP_IND interface of sigsa
*
*-----------------------------------------------------------------------------*/
typedef struct
{
   icf_rgm_context_st   *p_rgm_ctxt;
} icf_delete_comp_ind_st;

#endif


#ifdef ICF_IPSEC_ENABLED
/*------------------------------------------------------------------------------
 * Structures needed for IPSec Support at REGM-NM interface
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_rgm_nm_sa_create_req_st
 * Description :  Structure for SA create request from RGM to NM
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* UE's IP address and protected port */
    icf_transport_address_st     ue_ip_port;

    /* P-CSCF's IP address and protected port */
    icf_transport_address_st     proxy_ip_port;

    /* UE generated spi value identifying the SA */
    icf_uint32_t                 spi;

    /* lifetime for the new SA */
    icf_duration_t               lifetime;

    /* direction - inbound or outbound */
#define ICF_SA_DIR_IN            0x01
#define ICF_SA_DIR_OUT           0x02
    icf_uint8_t                  direction;

    /* encryption algorithm to be used */
    icf_uint8_t                  enc_algo[ICF_MAX_STR_LEN];

    /* encryption key and length */
    icf_string_st                enc_key;

    /* integrity algorithm to be used */
    icf_uint8_t                  auth_algo[ICF_MAX_STR_LEN];

    /* integrity password and length */
    icf_string_st                auth_passwd;

} icf_rgm_nm_sa_create_req_st;


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_rgm_nm_sa_modify_req_st
 * Description :  Structure for SA modify request from RGM to NM
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA that has to be modified */
    icf_uint32_t                 spi;

    /* new duration for the SA */
    icf_duration_t               lifetime;
    
} icf_rgm_nm_sa_modify_req_st;
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_rgm_nm_sa_delete_req_st
 * Description :  Structure for SA delete request from RGM to NM
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA that has to be deleted */
    icf_uint32_t                 spi;
    
} icf_rgm_nm_sa_delete_req_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_nm_rgm_sa_cmd_resp_st
 * Description :  Structure for SA create/modify/delete requests from NM to RGM
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA for which the response is received */
    icf_uint32_t                 spi;
    
    /* response for the SA specific operation - can be Success or Failure */
    icf_return_t	                response;

} icf_nm_rgm_sa_cmd_resp_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_nm_rgm_sa_expiry_ind_st
 * Description :  Structure for SA expiry indication from NM to RGM
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA that has expired */
    icf_uint32_t                 spi;

} icf_nm_rgm_sa_expiry_ind_st;


/* ------ End: Structures for IP Security Support at REGM-NM interface ----- */
#endif /* ICF_IPSEC_ENABLED */

/*-----------------------------------------------------------------------------
 *
 * NAME : icf_call_hold_resume_int_payload_st
 *
 * DESCRIPTION  : This data type is used to define a structure,
 *                which specifies the payload send in the interface structure 
 *                from SCM to CHR to specify whether the request is to be 
 *                buffered or not.
 ----------------------------------------------------------------------------*/
typedef struct
{
    /* This field is set when the request is to be buffered in the case when 
     * TWC or RNW is ongoing.
     */
    icf_boolean_t    buffer_event;
} icf_call_hold_resume_int_payload_st;

/*-----------------------------------------------------------------------------
 *
 * NAME : icf_get_media_cap_req_st
 *
 * DESCRIPTION  : This data type is used to define a structure,
 *                which specifies payload data of ICF_GET_MED_CAP_REQ send by
 *                SIC to MMI.
 -----------------------------------------------------------------------------*/
typedef struct
{
    /* Would represent the context identifier for meidia capabilities request */
    icf_call_id_t    context_identifier;

	/* This app-id identifies that media-capabilities are requires corresponding to 
	 * this application
	 */
	icf_app_id_t	app_id;

	/* This is a list of the node type icf_stream_capab_st
	 * This contains sdp from the application profile,if present   	
	 */
	icf_list_st           *p_app_sdp_list;
    icf_line_id_t          line_id;

}icf_get_media_cap_req_st;

/*-----------------------------------------------------------------------------
 *
 * NAME : icf_get_media_cap_req_st
 *
 * DESCRIPTION  : This data type is used to define a structure,
 *                which specifies payload data of ICF_GET_MED_CAP_RESP send 
 *                by MMI to SSA.
 -----------------------------------------------------------------------------*/
typedef struct
{
    /* Would represent the context identifier for meidia capabilities request */
    icf_call_id_t    context_identifier;

    /* Would specify the result of API in  terms of Success/Failure */
    icf_result_t     result;
#ifdef ICF_REL_OLD
    /* If the API was successfull then this would contain the Audio SDP else 
     * would be null */
    icf_sdp_st       *p_audio_sdp;

    /* If the API was successfull then this would contain the Video SDP else 
     * would be null */
    icf_sdp_st       *p_video_sdp;
#endif

	/*p_sdp_list contains the list of streams received in SDP from remote
	 *which contains node of type icf_inc_media_sdp_info_st*/
	icf_list_st           *p_sdp_list;

    /* For failure error cause would be returned in this*/
    icf_ssa_reason_t      reason;
}icf_get_media_cap_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_cfg_mph_app_remove_st
 * Description :  Structure for triggering MPH on app remvoal through CFG
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /*Application ID for the application to be removed*/
    icf_app_id_t	app_id;

#define ICF_NETWORK_DEACTIVATE	    0x01
#define ICF_APP_ADD_AGAIN			0x02

	icf_uint8_t  option;

} icf_cfg_mph_app_remove_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_www_authenticate_param_st
 *
 * Description: This structure is used to specify tag values in WwwAuthenticate
 *				header
 *                
 *---------------------------------------------------------------------------*/
typedef struct
{

#define ICF_NONCE_PRESENT            0x01 
#define ICF_OPAQUE_PRESENT           0x02 
#define ICF_STALE_PRESENT            0x04 
#define ICF_ALGO_PRESENT             0x08 
#define ICF_QOP_PRESENT              0x10 
    icf_uint8_t bitmask;
    
    /* Specifies authentication scheme - Digest/Basic */
    icf_uint8_t	   *p_auth_scheme;
    
    /* Specifies user realm to identify username/password */
    icf_uint8_t	   *p_realm;
    
    /* Specifies the nonce/challenge
	 * received in www-authenticate header of 401 response */
	 icf_uint8_t	   *p_challenge;

    /* Specifies string to be returned unchanged in Authorization resp */
     icf_uint8_t	   *p_opaque;

    /* Specifies if stale bit is set to true/false */
    icf_boolean_t stale;

    /* Specifies digest algo */
    icf_uint8_t	   *p_algorithm;

    /* Specifies qop option */
    icf_uint8_t	   *p_qop;

}icf_www_authenticate_param_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_authorisation_param_st
 *
 * Description: This structure is used to specify tag values in Authorisation
 *				header
 *                
 *---------------------------------------------------------------------------*/
typedef struct
{

    /* Specifies username */
    icf_uint8_t    *p_username;
    
    /* Specifies user passwd */
    icf_uint8_t    *p_passwd;
    
    /* Specifies the client calculated nonce/challenge */
	 icf_uint8_t	  *p_cnonce;

    /* Specifies nonce_count */
 	 icf_uint8_t	  *p_nonce_count;

    /* Specifies request method */
    icf_uint8_t	  *p_method;

    /* Specifies digest algo */
    icf_uint8_t	  *p_uri;

    /* Specifies response */
    icf_uint8_t    *p_response;

}icf_authorisation_param_st;


/*NOTIFY_OUT_OF_DIALOG_START*/
/*-----------------------------------------------------------------------------
 *
 * Structure Name:	icf_sic_notify_cfm_st
 *
 * Description: This API is used by the application to inform the framework.
 *                about the result of an the notify request received  by 
 *                the application.
 *---------------------------------------------------------------------------*/

typedef struct
{
	icf_call_id_t call_id;

    /* Indicates the result of the notify (success/failure). 
		success : 1 and failure : 0 */
	icf_return_t			result;

   	icf_uint32_t			error_code ;
	icf_uint32_t			response_code ;
    
    /* List of additional headers that the application wants to
       send in the NOTIFY response.
     */
    icf_header_list_st  header_list;

    /* List of additional tags that the application wants to
       send in the NOTIFY response.
     */
    icf_tag_list_st  tag_list;

    /* List of additional bodies that the application wants to
       send in the NOTIFY response.
     */
    icf_msg_body_list_st  body_list;
} icf_sic_notify_cfm_st;
/*NOTIFY_OUT_OFDIALOG_EN*/

/*HANDLE_INFO_IND_START*/
/*-----------------------------------------------------------------------------
 *
 * Name : icf_nw_inc_info_ind_st
 *
 * Description  : This is send from SSA to CC to give the INFO message
 *                received.
 *----------------------------------------------------------------------------*/
typedef struct
{
	icf_uint8_t           bitmask;
#define ICF_INC_INFO_MESG_BDY_PRESENT	0x100
} icf_nw_inc_info_ind_st;

/*HANDLE_INFO_IND_END*/
#ifdef ICF_SESSION_TIMER
typedef struct
{
  icf_duration_t               session_timer;
  icf_duration_t               min_se;
  icf_refresher_type_t         session_timer_refresher;
}icf_ssa_session_timer_set_st;  

/*-----------------------------------------------------------------------------
 *
 * Name : icf_session_timer_from_toggle_req_st
 *
 * Description  : This is send from CC to SSA to convey the session timer
 *                related values as provided in ICF_SESSION_TIMER_TOGGLE_REQ
 *                API. 
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t bitmask;
#define ICF_SESS_TIMER_PRESENT_IN_TOGG_REQ            0x01
#define ICF_MINSE_PRESENT_IN_TOGG_REQ                 0x02
#define ICF_REFRESHER_PRESENT_IN_TOGG_REQ             0x04

    icf_duration_t               session_timer;
    icf_duration_t               min_se;
    icf_refresher_type_t         session_timer_refresher;
}icf_session_timer_from_toggle_req_st;  
#endif


/*-----------------------------------------------------------------------------
 *
 * Name : icf_internal_refer_req_st
 *
 * Description  : This is send from SCM to CTU/CTA services in return path of 
 *                XCONF Service
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t bitmask;
#define ICF_REFER_METHOD_BYE                              0x01
#define ICF_REFER_URI_PRESENT                              0x02
#define ICF_REFER_REPLACE_CALL_ID_PRESENT   0x04
/* URI of the party which will be in referto header, 
 * SCM will initiate this request to CTA to invoke REFER request towards conference 
 * server with refer_to address as refer_uri*/
   icf_address_st  refer_uri; 

/*Call ID to be send in replaces, as this request will go to CTA 
 *and CTA will initiate REFER with replaces towards conference server*/
   icf_call_id_t      replaces_call_id;
}icf_internal_refer_req_st;


/*-----------------------------------------------------------------------------
 *
 * Name : icf_internal_refer_resp_st
 *
 * Description  : This is send from SCM to XCONF service in return path of 
 *                CTU/CTA Service
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
/* Denotes the Failure status only. */
   icf_result_t result;

/* Denotes the error cause */
   icf_error_t   ecode;
}icf_internal_refer_resp_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_call_progress_st
 * Decription  : Typedef for ICF_NW_INC_CALL_PROGRESS. This indicates that 
 *		 181/182/183 without sdp have been received from remote.
 *
 * --------------------------------------------------------------------------*/

typedef struct
{
    /* status code received in the progress message from peer */
    icf_uint16_t                    response_code;

} icf_nw_inc_call_progress_st;


/* ----------------------------------------------------------------------------
 * Type Name   : icf_sic_early_session_cfm_st
 * Decription  : This structure is used to communicate the data
 *               that MRM has received from application in ICF_EARLY_SESSION_CFM
 *               API, between MRM and SIC(Rel 8.1.0)
 *
 * --------------------------------------------------------------------------*/

typedef struct
{
    icf_result_t              result;

    /*  This is the line id that will be received in ICF_EARLY_SESSION_CFM */
    icf_line_id_t             line_id;

    icf_call_id_t             call_id;

    icf_uint32_t bit_mask;	
    #define ICF_SIC_EARLY_SESSION_TUNNEL_MODE_OPERATION_PRESENT  0x00000001
    #define ICF_SIC_EARLY_SESSION_RESPONSE_CODE_PRESENT             0x00000002
   /* This is the tunnel mode that will be received in 
      ICF_EARLY_SESSION_CFM */			
    icf_boolean_t tunnel_mode;

  /* This is the response code  that will be received in 
      ICF_EARLY_SESSION_CFM in case of failure*/
    icf_uint16_t  response_code;

}icf_sic_early_session_cfm_st;


/*  This feature is now out if scope in 8.0 */
#if 0
/* ----------------------------------------------------------------------------
 * Type Name   : icf_rediscover_proxy_server_st
 * Decription  : This message will be send from SSA to CFG when SSA finds that 
 *               SIP proxy server is not responding. CFG will clear all active
 *               dialogs for that proxy on all lines.
 *
 * --------------------------------------------------------------------------*/

typedef struct
{
/*  Following parameter specifies the proxy address not responding  */

    icf_string_st      proxy_addr;
}icf_rediscover_proxy_server_st;
#endif

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_generic_msg_req_st
 * Decription  : Typedef for ICF_NW_OTG_GENERIC_MSG_REQ.This indicates that 
 *               the application has sent a generic message request to send
 *               a REFER/NOTIFY request to the peer.
 * --------------------------------------------------------------------------*/
typedef struct 
{
    /* This indicates the generic request (REFER/NOTIFY) that the application wants
       to send to the peer.
    */   
    icf_string_st method_name;
}icf_nw_otg_generic_msg_req_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_generic_msg_ind_st
 * Decription  : Typedef for ICF_NW_INC_GENERIC_MSG_IND.This indicates that 
 *               the peer has sent a generic message request (REFER/NOTIFY request).
 * --------------------------------------------------------------------------*/
typedef struct 
{
    /* This indicates the generic request (REFER/NOTIFY) received from the peer. */
    icf_string_st method_name;
}icf_nw_inc_generic_msg_ind_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_inc_generic_msg_resp_st
 * Decription  : Typedef for ICF_NW_INC_GENERIC_MSG_RESP.This indicates that 
 *               the peer has sent a response to a generic message request
 *               (REFER/NOTIFY request) sent by the application.
 * --------------------------------------------------------------------------*/

typedef struct 

{
    /* This indicates the generic request (REFER/NOTIFY) for which a response
       has been received from the peer.
    */   
    icf_string_st method_name;
    /* This indicates the response received for the generic message request sent
       by the application.
    */   
    icf_uint16_t response_code;
}icf_nw_inc_generic_msg_resp_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_otg_generic_msg_cfm_st
 * Decription  : Typedef for ICF_NW_OTG_GENERIC_MSG_CFM.This indicates that 
 *               the application has sent a response to a generic message request
 *               (REFER/NOTIFY request) received from the peer.
 * --------------------------------------------------------------------------*/

typedef struct 
{
    /* This indicates the generic request (REFER/NOTIFY) for which a response
       has been received from the application.
    */   
    icf_string_st method_name;
    /* This indicates the response received for the generic message request sent
       by the peer.
    */   
    icf_uint16_t response_code;
}icf_nw_otg_generic_msg_cfm_st;

/* New Code for ASYNCHRONUS_MESSAGE enhancements*/
/*-----------------------------------------------------------------------------
 *
 * Structure Name:	icf_sic_message_cfm_st
 *
 * Description: This API is used by the application to inform the framework.
 *                about the result of an incoming MESSAGE request received  by 
 *                the application.
 *---------------------------------------------------------------------------*/

typedef struct
{
	icf_call_id_t call_id;

    /* Indicates the result of the MESSAGE (success/failure). 
		success : 1 and failure : 0 */
	icf_return_t			result;
    icf_uint32_t               bitmask;
    /* This bit is set whenever response_code field is filled */
    #define ICF_SIC_APP_CONFIRMATION_RESP_CODE_PRESENT 0x00000001

	icf_uint32_t			response_code ;

    /* List of additional headers that the application wants to send in the
     * SIP response message. */ 
    icf_header_list_st         header_list;

    /* List of additional tags that the application wants to send in the 
     * SIP response message.*/
    icf_tag_list_st            tag_list;

} icf_sic_message_cfm_st;



#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif /* __ICF_COMMON_INTERFACE_TYPES_H__  */
