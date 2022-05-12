/*------------------------------------------------------------------------------
 *
 * File name        : icf_ssa_types.h
 *
 * Purpose          : This file contains all the structures defined for internal
 *                    usage in SSA.
 *
 * Revision History :
 *
 *   Date        Name           Ref#         Comments
 * --------      ------------   ----          ---------------
 * 17-Jan-2005   Aashish Suchdev  ICF LLD  Initial Version
 * 27-Jan-2005   Aashish Suchdev  ICF LLD  Added ssa ctxt/pdb
 * 29-Jan-2005   Aashish Suchdev              Added interface struct union
 * 01-Feb-2005   Umang Singh                  Added fields in the pdb
 * 23-Mar-2005   Umang Singh                  Added bitmask for failure handling
 * 24-May-2005   Umang Singh      SPR 7551    
 * 24-May-2005   Umang Singh      SPR 7665    
 * 07-JUL-2005   Aashish Suchdev              TCP checkin       
 * 13-DEC-2005   Aman Arora		  ICF CDD 4.0 Coding for IPSec support
 * 22-Feb-2005	 Amit Sharma                  Added code for auth support
 * 01-Mar-2006   Amit Sharma                  ICF merging with ICF2.0
 * 23-Mar-2006   Amit Sharma                  Code merged with ICF 4.2
 * 06-Sep-2006   Umang Singh      SPR 1285    Parsing of Require hdr for 100rel
 *
 * 6-Sep-2006    Deepti Goyal     SPR 12699    Bitmask added for SDP
 *                                             send in update/re-invite.
 * 27-Nov-2006   Deepti Goyal     ICF Rel 6.2   NAT Feature Support
 *
 * 12-Dec-2006   Deepti Goyal     SPR 14036     destination ip in ssa context
 *                                              type modified.
 * 05-JAN-2007	Vidyut Gupta	Added bit mask and the field for FQDN
	
 * 05-JAN-2007  Krishna		Added new structure for DNS buffering
 * 05-Jul-2007  Sudhesh     Persisten TCP connection
 * 09-Jul-2007  Neha Choraria     ICF_7_0       Changes related to forking
 *                                              and multiple answer
 * 12-Jul-2007     Puneet Sharma
 *                 & Amit Mahajan SPR-15259      IPTK should send 488 for incorrect media format.
 * 22-Aug-2007  Deepti Goyal    ICF 6.5         DNS/TCP Nameserver fix.
 * 04-Dec-2009  Pavneet Bajaj     SPR 20231             Fix for CSR 8061646
 *                 
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_SSA_TYPES_H__
#define __ICF_SSA_TYPES_H__

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

#include "icf_common_types.h"
#include "icf_ssa_common_types.h"

#include "icf_common_interface_types.h"
#include "sdf_struct.h"
#include "sdf_callbacks.h"
#include "icf_ssa_defs.h"
#include "icf_regm_types.h"



#ifdef IMS_CLIENT
ICF_CRM_START
#include "microsip_struct.h"

ICF_CRM_END
#endif
/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_intrnl_intf_ut
 * Description :  This is the union of all possible interface structures
 * which SSA shares with other ICF internal modules ( structs defined
 * in icf_common_interface_types.h) 
 * ------------------------------------------------------------------------*/
   typedef union{
    icf_nw_inc_reject_st             nw_inc_reject;
    icf_nw_otg_media_change_req_st   nw_otg_media_chng_req;
    icf_nw_otg_reject_st             nw_otg_reject;
    icf_nw_otg_call_setup_st         nw_otg_call_setup;
    icf_nw_otg_call_alert_st         nw_otg_call_alert;
    icf_nw_otg_alertmedia_st         nw_otg_alertmedia;
    icf_nw_otg_prack_ack_st          nw_otg_prack_ack;
    icf_nw_otg_connect_st            nw_otg_connect;
    icf_nw_otg_connect_ack_st        nw_otg_connect_ack;
    icf_nw_otg_refer_st              nw_otg_refer;
    icf_nw_otg_refer_resp_st         nw_otg_refer_resp;
    icf_nw_otg_media_change_resp_st  nw_otg_media_chng_resp;

    icf_nw_inc_call_setup_st         nw_inc_call_setup;
    icf_nw_inc_prack_st              nw_inc_prack;
    icf_nw_inc_media_change_req_st   nw_inc_media_chng_req;
    icf_nw_inc_media_change_resp_st  nw_inc_media_chng_resp;
    icf_nw_inc_media_param_st        nw_inc_media_param;
    icf_nw_inc_call_alert_st         nw_inc_call_alert;
    icf_nw_inc_alert_media_st        nw_inc_alert_media;
    icf_nw_inc_connect_ack_st        nw_inc_connect_ack;
    icf_nw_inc_connect_st            nw_inc_connect;
    icf_nw_inc_refer_st              nw_inc_refer;
    icf_nw_inc_call_redirect_st      nw_inc_call_redirect;
    icf_nw_inc_redirect_ind_st       nw_inc_redirect_ind;

	icf_rgm_nw_resp_st				rgm_nw_resp;

	icf_nw_otg_subs_req_st			nw_subs_resp;	
	icf_nw_otg_msg_req_st			nw_otg_msg;
	icf_nw_otg_message_resp_st		nw_otg_msg_resp;
	icf_nw_inc_msg_resp_st			nw_inc_msg_resp;
	icf_nw_inc_msg_ind_st		    nw_inc_msg_ind;
	icf_nw_otg_publish_req_st		nw_otg_publish;
	icf_nw_inc_publish_resp_st		nw_inc_pub_resp;
    icf_get_media_cap_req_st         get_media_cap_req;
#ifdef ICF_SESSION_TIMER
	/*Special case for Session Timer : for sending session refresh
		requests when a session timer expires*/
	icf_nw_otg_session_refresh_req_st session_refresh_req;
#endif

    icf_nw_inc_call_progress_st		nw_inc_call_progress;

    }icf_ssa_intrnl_intf_ut;

/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_timer_list_st
 * Description :  SSA timer list structure, maintains the list of all
 *                running timers
 * ------------------------------------------------------------------------*/
typedef struct icf_ssa_timer_list_st
{
    /*Timer-ID returned by GENLIB when timer is started*/
    icf_timer_id_t     timer_id;

    /*Timer data passed to GENLIB when timer is started*/
    icf_timer_data_st  *p_timer_data;

    /*Pointer to next timer info*/
    struct icf_ssa_timer_list_st *p_next;
} icf_ssa_timer_list_st;

#ifdef ICF_IPSEC_ENABLED


#define ICF_SSA_UDP_SA_CONNECTION		0
#define ICF_SSA_TCP_SA_CONNECTION		1
#define ICF_MAX_SA_CONNECTIONS           ICF_SSA_TCP_SA_CONNECTION +1

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_sa_conn_db_node_st
 * Description :  Contains the protected server port and the server fd
 *
 *----------------------------------------------------------------------------*/
typedef struct {
	icf_uint16_t		        prot_port_num;
	icf_uint16_t		        fd;
} icf_sa_conn_db_node_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_ipsec_ssa_sa_context_st
 * Description :  Contains pointer to RGM and SA contexts for IPSec
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_ipsec_sa_context_st      *p_sa_context;
    icf_rgm_context_st           *p_rgm_context;

} icf_ipsec_ssa_sa_context_st;
#endif


/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_rejected_streams_st
 *
 * Description :  This structure will be used when there is a rejected stream in 
 *				  incoming INVITE. In that case, we will save the protocol corresponding
 *				  to that stream, so that when we will form an answer for that, then 
 *				  we will fetch the transport protocol for that stream from this list.	
 * ------------------------------------------------------------------------*/

typedef struct 
{
	icf_stream_id_t	stream_id;
	icf_string_st	protocol_name;
}icf_ssa_rejected_streams_st;


/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_inc_media_stream_info_st
 *
 * Description :  This structure keeps the info regarding the incoming SDP.
 *				  This structure is then populated in the internal messages of 
 *				  each incoming message having SDP. The purpose is to reduce the number of
 *				  arguments passed in the functions.
 * ------------------------------------------------------------------------*/

typedef struct 
{
   icf_list_st		  *p_sdp_info_list;
   icf_list_st		  *p_added_sdp_list;
   icf_list_st            *p_nw_reject_stream_list;
   /*The following variable contains the stream-ids of the streams which are marked 
    * rejected by SSA.
    */
   icf_uint8_t		  count;
   icf_stream_list_st rejected_streams[ICF_MAX_STREAM_PER_CALL];
}icf_ssa_inc_media_stream_info_st;

/* --------------------------------------------------------------------
 * Type Name	: icf_dns_buffer_tg
 * Description	: This structure is used to store the node of DNS Buffer. 
 * The DNS record will be kept as linked list.
 * ------------------------------------------------------------------------*/
typedef struct icf_dns_buffer_tg{

	/* Cotains Fully Qualified Domain Name */
	icf_string_st		  fqdn;

	/* Contains array of IP addresses string */
	icf_uint8_t	          ip_address[ICF_SSA_DNS_MAX_IP_ADDR_SIZE];

	/* Contains port information */
	icf_uint16_t         port;

	/* Contains the timer value for particular node of DNS Buffer */ 
	icf_uint16_t		timer_count;

}icf_dns_buffer_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_ctxt_st
 * Description :  SSA context structure.  Will hold pointers for Call
 *                Objects, Call Context and other SSA specific data
 *                persistent for duration of a call context.
 * ------------------------------------------------------------------------*/
struct  icf_ssa_tcpm_conn_data_st;
    typedef struct icf_ssa_ctxt_st{

#ifdef ICF_LOAD_DEBUG
    icf_call_id_t    call_id;
#endif        
        /*Following is used to store conditions/indications which 
         * are persistent across multiple invocations of SSA for a call
         */
        /* CAUTION */
	/* Please do not add any other parameter above
	 * Sdf_st_auth_param	auth_params;
	 * As it is used by UA code by type-casting the 
	 * whole context !!!!
	 */
#ifdef IMS_CLIENT
	/*Following is the structure containing user specified Authorization
	 *Parameters. IT wil be accessed by UA code to fill it before sending 
	 *Network. */
		
	Sdf_st_auth_param		auth_params;


	/* ICF: It has been understood that the UA-TK expects a list of 
	 * Sdf_st_uacAuthInfo members corresponding to each member of the
	 * list made of Sdf_st_authenticationParams structure that it derives from
	 * the response received. Hence REGM will use the auth_params_list to
	 * make the uac_auth_info list that will be used by SSA to form request with
	 * Authorization */
ICF_CRM_START
	
	    Sdf_ty_slist            *p_uac_auth_info_list;
        Sdf_ty_u32bit           uac_auth_info_list_size;

ICF_CRM_END
		 
#endif
        /*this is set by SSA in the callback for re-invite and
         * reset when SSA successfully sends resposne for it*/
#define ICF_SSA_CTXT_REINVITE_RECVD          0x01
        /*this is set by SSA in the callback for transaction recvd
         * reset when SSA successfully sends resposne for it*/
#define ICF_SSA_CTXT_UPDATE_RECVD            0x02
        /*This is set by decode result handler routines in SSA to indicate
         * to the calling routine whether the SSA ctxt which was allocated
         *  by the calling routine and passed to this called routine be 
         *  released when the calling routine again gets back control
         *  If this is set, then calling routine should not release the
         *  SSA ctxt memory*/
#define ICF_SSA_SSA_CTXT_STORED               0x04
#define ICF_SSA_REINVITE_CANCELLED            0x08
        /* this bit indicates the reliable provisional response code that was 
         * sent by toolkit to peer. It is set if 180 was sent and unset if 183 
         * was sent */
#define ICF_SSA_OTG_RPR_ALERT                 0x10
        /*this bit indicates that this call has to proceed on TCP connection
         * this will be reset when the connection is closed for some reason*/
#define ICF_SSA_TCP_TRANSPORT                 0x20
#define ICF_SSA_INVITE_PENDING                0x40
#define ICF_SSA_REG_PENDING                   0x80
/*This bit indicates that the previous REG txn was on UDP.
 * This is used in sending auth req when a 401/407 is rx
 * in response to first REG request*/

#define ICF_SSA_RGM_PREV_TRANS_TYPE_UDP       0x100
        /*SPR 7743 Starts*/
        /*Following is used to indicate that no message
         * is to be sent for this dialog to any App*/
#define ICF_SSA_APP_CTXT_TERMINATED           0x200
        /*SPR 7743 Ends*/

/*This bit is used in await conn state of TCP SM, and set when
 *forceful call clear received*/
#define ICF_SSA_CLEAN_TCP_CONN_FORCEFUL_CLR   0x0400

#define	ICF_SSA_OPTIONS_ONGOING			     0x0800

/* This bit indiactes that OPTIONS is recvd out of dialog */
#define ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG	 0x1000

/*Field indicating that application context is marked NULL 
 * i.e. delinked from ssa_ctxt*/
#define ICF_SSA_APP_CTX_DELINKED              0x2000

/* added for TCP support in ICF release 4.0 */
#define ICF_SSA_SUBS_PENDING                  0x4000
#define ICF_SSA_PUBLISH_PENDING               0x8000
#define ICF_SSA_MESSAGE_PENDING               0x10000
#define ICF_SSA_REFER_PENDING                 0x20000
#define ICF_SSA_OPTION_PENDING                0x40000
#define ICF_SSA_UPDATE_WITH_SDP               0x80000  

#ifdef ICF_SIGCOMP_SUPPORT
        /* Following is used to indicate that the Message
         * should compress. SSA will set this bit when it 
         * founds the request message contains comp=sigcomp
         * param in top most route header (request URI if Routeheader not
         * present) or the response message contains comp=sigcomp
         * param in the topmost via header 
         * It reset the flag when the message is sent succesfully
         */

#define ICF_SSA_SIP_MSG_SHOULD_BE_COMPRESSED   0x100000
#endif
/* This bitmask will be set when an unexpected message was received */
#define	IPPSKT_SSA_UNEXPECTED_SIP_MSG_RECV	   0x200000

#define ICF_SSA_TLS_TRANSPORT                  0x400000
/*The following bitmask is used to identify that an out-of-uatk connect
is required. This will only be set in case of dialog creating requests*/
#define ICF_SSA_EXPLICIT_CONNECT_REQ           0x800000

/* this bit is set to disallow further responses */
#define ICF_FINAL_RESP_RECEIVED                0x1000000 

#ifdef ICF_DNS_LOOKUP_ENABLED
/* for the record of DNS ip for contacts */
#define ICF_SSA_CONTACT_IP_PRESENT            0x2000000 
/* this is to remove the DNS context if no response is received for the same */
#define ICF_DNS_RECORD_FETCHED                0x4000000
/* this is to store the FQDN once the response is receved */
#define ICF_SSA_DNS_FQDN_PRESENT              0x8000000  
/* this is to add the DNS retries in TCP for storing the 
 * Transaction information  here XSACTION = Transaction*/
#define ICF_SSA_DNS_XSACTION_PRESENT          0x10000000   
#endif

#define ICF_SSA_CTXT_REINVITE_RECVD_WO_SDP    0x20000000

#ifdef ICF_FAX_SUPPORT
#define ICF_SSA_CTXT_T38_RCVD_IN_NEW_CALL     0x40000000
#endif

/*This will check whether the authorization for a particular request is in progress or not*/
#define ICF_SSA_AUTH_IN_PROGRESS    0x80000000

/*Bitmask changed from 8 bit to 16 bit in TCP merge*/

       icf_uint32_t          bitmask;
/******** 7.0. New bitmask field *******/

/*This is stop the retranmission of DNS queries*/
#define ICF_SSA_DNS_RESP_RECEIVED            0x01       
#define ICF_BYE_ALREADY_SEND                 0x02


/* This bitmask will be set while out of dialog message handling, till now
 * it is only being checked in case of Notify received out of dialog.*/
#define ICF_MSG_RECVD_OUT_OF_DIALOG        0x04

/*This bit will be set when decode fail of 200OK of INVITE occurs because 
 * mandatory Contact header was not received in 200 OK. */
#define ICF_MANDATORY_CONTACT_HDR_MISSING    0x08
#define ICF_REQ_URI_TOO_LONG                 0x10

/* Fix for CSR 8061646 SPR 20231 : New bitmasks to identify the API for sending UPDATE.
 * These bitmasks will be set when UPdate due to call modify/chr has to 
 * be sent on network and used when sending Update again for 422 received.
 */
#define ICF_SSA_MEDIA_MODIFY_FOR_CALL_MODIFY		      	  0x20
#define ICF_SSA_MEDIA_MODIFY_FOR_CALL_HOLD		      	      0x40
#define ICF_SSA_MEDIA_MODIFY_FOR_CALL_RESUME		      	  0x80


        icf_uint32_t          bitmask_1;

/* Bitmask for rport parameters Indications.This bitmask is used to check 
 * whether rport is received in request or not.
 */
#ifdef ICF_NAT_RPORT_SUPPORT

#define ICF_RPORT_RECEIVED      0x01
icf_uint8_t           rport_bitmask;

/* This field will contain the Network IP Address from which the SIP
 * message is received.
 */
icf_uint8_t      destination_ip[ICF_MAX_IPV4_ADDRESS_LENGTH];
icf_uint16_t   destination_port;
#endif
        /*
         *
         * Following is used to hold the index at which the call object 
         * to nw call ID map is being maintained for this call object.
         * We will need this index when we will need to reset the entry
         * in the map at the time of CallObj destruction
         */
        icf_uint16_t          callobj_map_index;
	
		icf_uint8_t			 *p_hash_key;	
        icf_uint16_t          options_map_index;

        /*
         * Pointer to call context.  If, in the event of SSA processing
         * start, the call context pointer here is NULL, we will clear
         * the resources allocated and return
         */
        icf_call_ctx_st     *p_call_ctxt;

        /* Pointer to rgm context */
        icf_rgm_context_st  *p_rgm_ctxt;

#ifdef ICF_SIGCOMP_SUPPORT
        /* Pointer to sigsa context */
        icf_void_t          *p_sigsa_ctxt;
#endif
        
		
ICF_SIC_START
		/* Pointer to SIC context */
        icf_sic_ctx_st     *p_sic_ctxt;
ICF_SIC_END
		
		/*
         * Save the authentication parameters as received in the Registration 
         * response. This information helps the user authorize himself to the 
         * specific domain/realm for the correct scheme
         */
        Sdf_ty_slist            *p_auth_params_list;
        Sdf_ty_u32bit           auth_params_list_size;
        
        /*Following is being used to store the GLB PDB in SSA ctxt.
         * We will need this in StartTimer callback to get the GLB PDB*/
        /*icf_glb_pdb_st       *p_glb_pdb;*/

        struct icf_ssa_pdb_st	*p_ssa_pdb;

        /*
         * Pointer to Call Object. The call Object will have the pointer
         * to SSA context in the Application Data field.  There can 
         * be multiple call objects in some services scenario.  That
         * is one of the reason for having SSA context sepeartely.
         */
        Sdf_st_callObject      *p_call_obj;
#ifdef ICF_SESSION_TIMER
		icf_boolean_t session_timer_expired_refresh;
		icf_uint32_t		refresher_method_type;
#endif

        /*
         * The following bitmask stores the response code which needs
         * which will be sent from entry point .
         * SSA will not be sending a failure response from the callback
         * Instead,if the callback fails and this bitmask is set
         * it will send failure response to peer
         */
        icf_uint32_t         failure_bitmask;
#define ICF_SSA_FAILURE_RESP_CODE_500    0x0001
#define ICF_SSA_FAILURE_RESP_CODE_503    0x0002
#define ICF_SSA_FAILURE_RESP_CODE_481    0x0004
#define ICF_SSA_FAILURE_RESP_CODE_400    0x0008
#define ICF_SSA_FAILURE_RESP_CODE_416    0x0010
#define ICF_SSA_FAILURE_RESP_CODE_505    0x0020
#define ICF_SSA_FAILURE_RESP_CODE_415    0x0040
#define ICF_SSA_FAILURE_RESP_CODE_482    0x0080
#define ICF_SSA_FAILURE_RESP_CODE_404    0x0100
#define ICF_SSA_FAILURE_RESP_CODE_405    0x0200
#define ICF_SSA_DECODE_FAILURE           0x0400
#define ICF_SSA_FAILURE_RESP_CODE_403    0x0800
#define ICF_SSA_FAILURE_RESP_CODE_488    0x1000
#define ICF_SSA_FAILURE_RESP_CODE_580    0x2000
#define ICF_SSA_FAILURE_RESP_CODE_487    0x4000
#define ICF_SSA_FAILURE_RESP_CODE_603    0x8000
#define ICF_SSA_FAILURE_RESP_CODE_414    0x10000
#define ICF_SSA_FAILURE_RESP_CODE_501    0x20000

#ifdef IMS_CLIENT
        icf_timer_id_t       timer_id;
        icf_timer_data_st   *p_timer_data;

#endif


		        /* Pointer to SSA global data */
        struct icf_ssa_glb_data_st  *p_ssa_glb_data;
        /* Pointer to global pdb */
        icf_glb_pdb_st       *p_glb_pdb;
#if 0
        /*Invalid value is ICF_SSA_MAX_TCP_CONN*/
        icf_uint32_t         ssa_conn_db_index;
#endif
        icf_transport_mode_t  transport;
        
        /* Following is the pointer to pending dialog starter
        * txn. Currently it can be either for REGISTER or for
        * first INVITE.  What it is in an instance of SSA ctxt
        * is indicated by the flags ICF_SSA_INVITE_PENDING
        * and ICF_SSA_REG_PENDING */
        icf_void_t*          p_pending_txn;
        
        icf_boolean_t          revert_mode;

#ifdef ICF_IPSEC_ENABLED
        /* pointer to SA context and related RGM context */
        icf_ipsec_ssa_sa_context_st      ssa_sa_context;
#endif /* ICF_IPSEC_ENABLED */
    

        icf_boolean_t		trans_mode_reverted;
        
        /* SSA need to set line id in call context as this info is required
        * at time of handling 401 for BYE. This will be updated only
        * on reciving BYE 
        */
        icf_line_id_t       line_id;

        /*SSA timer list of all running timers*/
        icf_ssa_timer_list_st  *p_timer_list;
        /* 
         * The following field is used for request - response correlation
         * between SSA and MMI when OPTIONS is received from metwork and
         * SSA queries MMI for media capabilities
         */
        icf_uint16_t         context_identifier;
        
        /* OPTIONS timer id */
        icf_timer_id_t       options_timer_id;
        /* OPTIONS timer data */
        icf_timer_data_st    *p_options_timer;

		/* This array list contains the protocol for the rejected streams only 
		 * in case of INVITE.So, SSA will save transport protocol for that rejected stream
		 * So, while forming sdp for rejected streams for this offer,
		 * SSA will fetch the protocol for the corresponding stream-id.
		 */
		icf_ssa_rejected_streams_st	rej_stream[ICF_MAX_STREAM_PER_CALL];

		 /* For incrementing version no. in o-line in case of reinvite */
        icf_uint32_t        sdp_o_version;
        /* To store the call type in the ssa context and use this call type to 
         * validate the incoming streams.
         */
        icf_call_type_t		call_type;

		/* For calculating nc (nonce-count) value if qop parameter is
          present in the challenge */
        icf_uint32_t        nc;
		
        /* This variable would keep a count of SRV  records returned 
		 * in the SRV query */
         icf_uint8_t 		dns_srv_count;

		/* Number of timers that are currently running */
		icf_uint8_t			timer_count;
#ifdef ICF_DNS_LOOKUP_ENABLED
        /* for storing the DNS info of contact FQDNs */
        icf_dns_buffer_st   *p_dns_ip_addr;
        /* for temporarily storing the fqdn later on when the 
         * response is received it will moved to DNS buffer
         * it also used to remove the DNS entry if all retries expires */
        icf_string_st       fqdn;
        /* for the case of TCP to store trasaction and and overlap
         * transaction pointers */
        icf_void_t          *p_transaction;
        icf_void_t          *p_overlap_transaction;
        /* CSR1-5285622 */
         /* timer id corresponds to dns resp timer id.*/
        icf_timer_id_t          dns_resp_timer_id;

        /* will be holding pointer to timer data corresponding
         * to dns resp timer.*/  
        icf_timer_data_st       *p_dns_resp_timer_data;
        
         /*This Ip and port will keep the info on which the request has been sent
          * so that when the response comes,then that info is used to populate the 
          * dns buffering
          */
 
        icf_uint8_t          ip_fr_req[ICF_MAX_IPV4_ADDRESS_LENGTH];
        icf_uint16_t         port_fr_req; 
         /*This contains ares fd on which DNS query has been made.
         *This is required in case of DNS timer expiry when IPTK needs 
         *to remove the fd from the read_fd_set.
         */
        icf_uint32_t          ares_fd;
        icf_uint32_t          dns_retrans_timer;      
        icf_uint16_t          ares_qid;  
#endif

        /*If this flag is true then no retransmissions will be happened in 
         *case of forceful call clear*/
        /*This flag is checked in IndicateTxnTimedOut*/
        icf_boolean_t       no_retrans_req;

        /* Rel 7.0 enhancement */

       icf_uint8_t      *p_pending_pdu[ICF_MAX_PENDING_PDU_PER_CONN];
       icf_uint16_t    no_of_pending_pdu;
/* Is an array of pointers for those pdus which have not been sent due to connection unavailability */
     struct icf_ssa_tcpm_conn_data_st    *p_conn_table_req_entry;   /* Pointer to connection data */
     struct icf_ssa_tcpm_conn_data_st    *p_conn_table_resp_entry;   /* Pointer to connection data */

        /********** Changes for Forking & Multiple Answer Start (Rel7.0) *********/

        /* counter for forked response */
        icf_uint8_t        forked_resp_counter;

        /*********** Changes for Forking & Multiple Answer End (Rel7.0) **********/
#ifdef ICF_RUNTIME_NAMESERVER_CONFIG
		/* this flag will be set by SSA whenever it initiates DNS query, and 
		 * will be reset when it receives the DNS response.
         */
		icf_boolean_t		refresh_nameserver;
#endif
         /* To store the nonce received in 401/407 in resposne to 
         * a REINVITE/REGISTER/SUBSCRIBE/BYE.This is required to compare the new
         * nonce value received from the old one.
         */
          
	icf_uint8_t	                 *p_old_nonce;

} icf_ssa_ctxt_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_ctxt_list_st
 * Description :  This structre is actually added in hash table for ssa
 * context. It shall be a linked list
 * ------------------------------------------------------------------------*/
typedef struct icf_ssa_ctxt_list_st
{
    icf_ssa_ctxt_st *p_ssa_ctxt;
    struct icf_ssa_ctxt_list_st  *p_next;
}icf_ssa_ctxt_list_st;   
/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_callid_callobj_map_st
 * Description :  Used to map the call object pointer for a call id
 * This mapping is used to identify a call object at the time of reception
 * of a n/w msg
 * ------------------------------------------------------------------------*/
    typedef struct{        
        icf_ssa_ctxt_st  *p_ssa_ctxt;
    }icf_ssa_callid_callobj_map_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_context_id_ssa_ctx_map
 * Description :  This mapping between SSA ctxt and a context 
 *                identifier is used for request - reponse correlation
 *                between SSA and MMI when SSA queries MMI for
 *                media capabilities corresponding to incoming OPTIONS
 * ------------------------------------------------------------------------*/
    typedef struct{
        icf_uint16_t     context_identifier;
        icf_ssa_ctxt_st  *p_ssa_ctxt;
    }icf_ssa_context_id_ssa_ctx_map;


/*------------------------------------------------------------------------------
 * Type Name        : icf_ssa_tcpm_conn_data_st
 *----------------------------------------------------------------------------*/
typedef struct icf_ssa_tcpm_conn_data_st{
    icf_int16_t         conn_fd;
    icf_uint16_t         peer_port_num;
    icf_uint8_t          tcpm_sm_state;
    icf_uint8_t          timer_count; /* 7.0  For each purge timer expiry, this count shall
                                         be incremented. It denotes that for how long
                                         the connection is in idle state */
#ifdef ICF_SECURE_SUPPORT	       
    icf_long_string_st  *p_peer_name; /* 7.0 */
#endif
    icf_uint16_t   ref_count; /* 7.0 This shall denote currently how many calls
                                 are there for this connection */
    icf_boolean_t     is_persistent; /* 7.0 For TCP this value should be 1
                                        always but for TLS, it will only be 1 if
                                        alias has been received or IPTK has make this
                                        connection  */
#define     ICF_TCP_DATA_SERVER_PORT  0x01
    icf_uint8_t     bitmap;

#ifdef ICF_IPSEC_ENABLED
    /* each IPSEC connection on this FD will increment this ref count
     * and the count will be decremented when the connection is closed. But
     * finally the connection will be removed only when this count is 0
     */
    icf_uint8_t          act_connec_count;
#endif
#define ICF_TRANSPORT_TCP             0x01
#define ICF_TRANSPORT_TLS             0x02
#define ICF_CONN_TYPE_REQUEST  0x10
#define ICF_CONN_TYPE_RESPONSE 0x20
#define ICF_CONN_TYPE_BOTH 0x40

    icf_uint8_t            prot_conn_bitmask;
/* This bitmask shall contain the value of protocol as well as connection type
 * set to its MSB*/

#ifdef ICF_IPV6_ENABLED
    icf_uint8_t          peer_ip[ICF_MAX_IPV6_ADDRESS_LENGTH];
#else
    icf_uint8_t          peer_ip[ICF_MAX_IPV4_ADDRESS_LENGTH];
#endif
#if 0
    /*Array used to store the pointers to pdus which are waiting to be sent
     * on to network.  The end of array is identified by the presence
     * of first NULL pointer*/
    icf_uint8_t      *p_pending_pdu[ICF_MAX_PENDING_PDU_PER_CONN];
#endif

}icf_ssa_tcpm_conn_data_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_glb_data_st
 * Description :  SSA global data.  It is allocated and init by SSA at the 
 * time of icf_ssa_init.  Right now we are storing the linear array for
 * storing network call-id mapping to call object pointer
 * ------------------------------------------------------------------------*/
    typedef struct icf_ssa_glb_data_st{
        Sdf_st_initData             *p_default_profile;
        icf_uint32_t             purge_timer_id;
        icf_timer_data_st        *p_purge_timer;
/* From ICF.6.4 this array is replaced with hash table*/
/*
        icf_ssa_callid_callobj_map_st    
            call_id_obj_map[ICF_SSA_MAX_CALL_OBJ];*/

        icf_ssa_context_id_ssa_ctx_map
            options_map[ICF_SSA_MAX_OPTIONS_CALL_OBJ];

        /* function pointer table for SSA fsm */
        icf_void_t*
            ssa_tcpm_fsm_table[ICF_SSA_TCPM_MAX_STATES][ICF_SSA_TCPM_MAX_EVENTS];
        icf_ssa_tcpm_conn_data_st    *p_ssa_tcpm_conn_db;

#ifdef ICF_IPSEC_ENABLED      
        /* number of successive transactions that have timed-out in ICF.
		 * When this limit is reached we shall clear al ongoing/pending
		 * transactions/calls and removed registration */
        icf_uint8_t    num_successive_trans_fail;
		/* we will have info for 2 SA's old and new */
		icf_sa_conn_db_node_st		sa_conn_db_node
										[ICF_MAX_SA_CONNECTIONS];
#endif /* ICF_IPSEC_ENABLED */

         /* User Agent string */
        icf_string_st  user_agent_str;

        /* Common bitmask */
        icf_uint8_t ssa_glb_bitmask;
        
        /* 
         * This bit will be set during SSA init  .
         * On the first invocation of ICF_CFG_OPEN_NW_SERVERS,
         * this bit will be reset.Also on ssa de-init this bit will be re-set
         */
#define ICF_SSA_FIRST_INVC_OPEN_NW_SERVERS   0x01

    }icf_ssa_glb_data_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_scope_mem_list_st
 * Description :
 * ------------------------------------------------------------------------*/
    typedef struct{
        icf_uint8_t  ssa_mem_type;
        icf_void_t   *p_mem_chunk;
    } icf_ssa_scope_mem_list_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_pdb_st
 * Description :  SSA process data block.  This is used to pass as pData
 * in EventContext pointer to SDF.  This is relevant for one invocation
 * of the entry point
 * ------------------------------------------------------------------------*/
    typedef struct icf_ssa_pdb_st{
        /*
         * This is the pointer to the global data which is allocated
         * by SSA at module init time and held in p_module_data array
         * in glb data
         */
        icf_ssa_glb_data_st      *p_ssa_glb_data;

        /*
         * We need glb pdb here as this will be used at many places
         * like for traces/stats/alarms etc in functions
         */
        icf_glb_pdb_st           *p_glb_pdb;

        icf_line_data_st         *p_line_data;
        icf_config_data_st       *p_glb_cfg_data;

        /*
         * Pointer to internal message structure.
         */
        icf_internal_msg_st      *p_internal_msg;
        /* Pointer to ecode */
        icf_error_t              *p_ecode;
#ifdef ICF_TRACE_ENABLE
    	icf_uint8_t              result;
#endif

        /*
         * Destination module ID: this is set in the callback, based on this
         * the SSA main flow identifies which module's msg handler is to be
         * invoked
         */
        icf_int_module_id_t      dest_module_id;

        /*
         * Array of void pointers to store the pointers to those memory
         * chunks( allocated in call back processing for internal message 
         * structre population) which are to be released
         * by SSA on return path - after invocation of the entry point of CM/
         * CFG.
         */
        icf_ssa_scope_mem_list_st
            allocated_memory[ICF_SSA_MAX_MMRY_CHUNK_LST];
        icf_uint8_t      alloc_index;

        /*Invalid value is ICF_SSA_MAX_TCP_CONN*/
        /*We keep two place holders for the connection FD.  This is because
         * in the SM we will always use the FD from the SSA PDB.
         * The source of this FD can be either an incoming network connection
         * ( in which case we do not have a SSA CTXT-as no msg has been recvd)
         * or an outgoing connection ( in which case we will have a SSA ctxt
         * and a CallObject both).We will take care to update the FD for inc
         * when a msg is received on it*/
#if 0
        icf_uint32_t         ssa_conn_db_index;
#endif
        /*Used to invoke the SM to send msg over a tcp conn
         * or buffer the pdu in list.Filled in SendMsgOnNetwork CBK*/
        icf_uint8_t          *p_stream_pdu;
	    icf_uint32_t         pdu_len;
#ifdef ICF_SIGCOMP_SUPPORT
        Sdf_ty_matchMessageType     msg_type;
        Sdf_st_error                sdf_error;
#endif
        /*
         * Common bitmask which is used to indicate various flags
         * For. eg (1)whether media has to be sent or not 
         *         (2)whether this is a reliable response
         * The following #defines will indicate ther corresponding bits
         */
#define ICF_SSA_RPR_PRESENT              0x0001
#define ICF_SSA_MEDIA_INFO_PRESENT       0x0002
#define ICF_SSA_USER_ADDR_FRM_CC         0x0004
#define ICF_SSA_USER_ADDR_FRM_RGM        0x0008
        /* this bit is set for REQ req and unset for De-REG req */
#define ICF_SSA_RGM_REQ_TYPE             0x0010
        /* this bit is set if this is the first/initial REG request */
#define ICF_SSA_RGM_INITIAL_REQ          0x0020

        /* this bit is set by SIP messsage processor in case an internal
         * message is to be sent to CFG/CM due to reception of the SIP
         * message*/
#define ICF_SSA_INTER_MODULAR_MSG        0x0040
        /*SPR 7743 Starts*/
        /*
         * This bit indicates that the SSA ctxt awaits deletion
         * and the call context has been cleaned up.
         */
#define ICF_SSA_AWAITING_SSA_CTXT_DEL    0x0080
        /*SPR 7743 Ends*/
#ifdef ICF_SIGCOMP_SUPPORT

        /* This bit Indicates that the Sip Message is decoded by
         * SIPUATK. This bit will be set once the Uncompressed
         * sip Message has been decoded.
         */
#define ICF_SSA_SIP_MSG_DECODED_BY_UATK        0x0100
#endif

ICF_SIC_START
/*
 * This bit indicates that fetch user address from SIC context
 */
#define ICF_SSA_USER_ADDR_FRM_SIC       0x0400

/*
 * This bit indicates that it is first subscribe req
 */
#define ICF_SSA_SIC_INITIAL_REQ         0x0800

#define ICF_SSA_MSG_RECVD_OUT_OF_DIALOG       0x1000
ICF_SIC_END

/*This indicates to the timer entry point that a session
refresh request is to be sent out a result of session timer
expiry*/
#define ICF_SSA_PENDING_SESSION_REFRESH       0x2000
#define ICF_SSA_SECURE_CONN		      0x4000
/* This is being set in cases where we want to remove our mark from callobject
when UATK returns processing to the entry point code ( after UpdateCallDetails)
This is required as in case of TCP (for some response processing) UATK does not
start the timer, and as of current implementation we release callobject when we get the 
final response. If UATK then tries to access callobj, it crashes, 
To avoid such cases we will not release callobject there and rather will set 
a bitmassk in PDB*/
#define ICF_SSA_REL_CALL_OBJ_ON_RET               0x8000

/* This bit is set when the Content-Disposition header is received with display type = session
 * and the handling is required
 */
#define ICF_SSA_SESSION_HANDLING_REQUIRED         0x00010000
/* Bit mask is added for fixing the spr 14989 */  
#define ICF_SSA_CALL_OBJ_ALREADY_FREED            0X00020000
#define ICF_SSA_REQUIRE_VALUE_IS_NOT_SUPPORTED    0x00040000
/* This bit is added to indicate whether trace is required to be sent
   to the application.It is used when 200 OK is received for a sent
   BYE.In such a scenario,the icf_ssa_cpature_trace function is being
   invoked from the call back sdf_cbk_uaCallTerminated.In all other 
   scenarios,the trace function is being invoked from 
   icf_ssa_handle_successful_decode function as earlier.
 */
#define ICF_SSA_TRACE_REQUIRED                    0X00080000
/* This bit indicates that this request does not need refetching the name 
 * server
 */
#define ICF_SSA_NO_NAMESERVER_REFRESH             0X00100000
#define ICF_SSA_MSG_TRACE_ALREADY_SENT            0X00200000

#define ICF_SSA_RESPONSE_SENT_TO_NETWORK          0x00400000
#define ICF_SSA_MEDIA_MODE_RECEIVED               0X00800000
#define ICF_SSA_INC_STOP_FAX_REINV                0x01000000
/* This bit determines whether we need to copy the incoming 
 * SDP or not on the basis of the content-disposition header 
 * value.This bit is set in the function icf_ssa_validate_content_disp
 * This change is done for ETSI conformance cases
 * SIP_CC_TE_CE_V_008 and SIP_CC_TE_CE_V_009
 */
#define ICF_SSA_SDP_TO_BE_IGNORED                  0x02000000

        icf_uint32_t common_bitmask;
     
        /* SSA Context has been stored for common processing */
        icf_ssa_ctxt_st   *p_ssa_ctxt;



#ifdef ICF_IPSEC_ENABLED

        /* temporary pointer to REGM context derived on receiving any message 
         * from network; this will be used subsequently for accessing the SA 
         * context anywhere within SSA */
        icf_rgm_context_st           *p_rgm_context;

        /* temporary pointer to SA context */
        icf_ipsec_sa_context_st      *p_sa_context;
#endif /* ICF_IPSEC_ENABLED */



		icf_boolean_t     send_trans_param;
		/* Required for TCP handling for decompressed SIP message */
		icf_uint16_t      processed_bytes;

        /* The method received/being worked on */
        icf_uint8_t          *p_method;

        /* For storing application conf data */
        icf_app_conf_st *p_app_conf;

        icf_void_t       *p_glb_rgm_data;
		
		/* This varaible keeps track of the duplicate streams.Once the stream is
		 * received, the corresponding bit is set for that stream.So, if again that 
		 * stream is received, SSA will check whether that stream is received before or not.
		 */
		icf_call_type_t		line_recvd;

		icf_uint16_t	dest_port;
		icf_int8_t	dest_ip[ICF_MAX_SIZE_MEDIUM_STR];
        icf_uint8_t  protocol;
#ifdef ICF_DNS_LOOKUP_ENABLED
		/*this is used to convey the value of event context to
		routine attmept_out_of_uatk_connect*/
		Sdf_st_eventContext *p_event_ctxt;
#endif
        /*If is_otg_msg is TRUE then its outgoing message else
         *its an incoming message*/
        icf_boolean_t        is_otg_msg;

        /* Warning Header response code, this will be set while 488 response code is to be sent on nw. */
        icf_uint32_t     warning_header_code;

        /* warning text, this will also be set while setting 488 reponse is to be sent on nw.*/
        icf_string_st    warning_text;
        

	/* this index will be used in case of forking for 
	 * hash key */
	
        /*variable to store the number of retransmissions count*/
        icf_uint8_t     retrans_count;
       /*variable to store the response code*/
        icf_uint16_t    resp_code;
	icf_uint32_t	hash_index;
 
     /* Added for traces functionality */
     icf_uint16_t    dRespCode;
  
#ifdef ICF_TRACE_ENABLE
     icf_uint16_t       dialog_status;
     icf_boolean_t 	sdp_present;
     icf_boolean_t      replace_header_present;
     icf_address_st     local_addr;
     icf_address_st     remote_addr;
     icf_uint8_t	*p_buf;
     icf_uint32_t 	len;
     SIP_S8bit          *p_subs_state;
     icf_uint8_t        dns_query_type;
     icf_uint32_t       notify_duration;
     icf_uint8_t     reason_code[15];
#endif
     /* 7.0 enhancement */
     icf_ssa_tcpm_conn_data_st    *p_conn_table_entry; /* To store connection data
                                                          ssa_conn_db_index field shall be dropped */
     icf_uint8_t  conn_type; /* To store the type of connection. Shall be used while passing
                              it to find connection from connection table */
     icf_address_st  conference_uri;
     en_SipMessageType   dMsgType;

     icf_boolean_t   bye_rejected;
     /* SPR 17555: Call Direction added in ssa pdb
      */
     icf_uint8_t        call_direction;
     icf_uint8_t      trans_param[5];

     icf_boolean_t       authorised_req;
     icf_boolean_t    notify_tag_mismatch; 
 
     /*This param is set to TRUE when SIP URI param not required
      * to copy in address_st*/
     icf_boolean_t    addr_param_not_reqd; 
}icf_ssa_pdb_st;

/*------------------------------------------------------------------------------
 * Type Name        : icf_cc_fsm_func_ptr_t
 * Description      : CC's function pointer table type
 *----------------------------------------------------------------------------*/
typedef icf_return_t (*icf_ssa_tcpm_fsm_func_ptr_t)(
       icf_ssa_pdb_st* p_ssa_pdb) ;


#ifdef IMS_CLIENT
/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_subs_event_et
 * Description :  This is is used to define supported event by toolkit for 
 * 	          Subscription.
 * ------------------------------------------------------------------------*/
    typedef enum
    {
	ICF_SUBS_EVENT_REG = 0,
	ICF_SUBS_EVENT_INVALID
    }icf_ssa_subs_event_et;
#endif
#if 0
/* --------------------------------------------------------------------
 * Type Name   :  icf_ssa_timer_data_st
 * Description :  This structure is used to store the callback related info
 * for the UATK. This will be passed to genlib on timer start.  On timer
 * expiry, genlib will invoke a function ( a timer expiry warapper in SSA)
 * which will then invoke UATK's callback function with the timer type and
 * pcontext info
 * ------------------------------------------------------------------------*/
    typedef struct{
        /*Following is the pointer to the SSA context, passed as AppData in
         * the StartTimer callback.  This is usedin case of a timer expiry
         * indication from MRM. SSA received the address of TK timer data 
         * from which it gets the SSA timer data. Then p_ssa_ctxt is used
         * to hold the address of the internal message passed from MRM.
         * When the Callback for IndicateTxnTimeout is hit, the pContext
         * there is passed the AppData( SSA CTXT). There the internal msg
         * is retreived from it and used to populate the outgoing message 
         * to CM
         */
        icf_ssa_ctxt_st         *p_ssa_ctxt;

        /*ID of the timer returned by porting routine for start timer
         * This is stored so that in case of a StopTimer invocation from
         * UATK, we can invoke the porting routine's stop timer interface
         * with correct identifier*/
        icf_timer_id_t           timer_id;
        /*Pointer of the TK timer data block allocated at the time of 
         * StartTimer. This is required at time of StopTimer, so that 
         * we can release this instance.The interface of StopTimer passes
         * TimerHandle which is the SSA TIMER DATA block
         */
        icf_timer_data_st         *p_timer_data;

        Sdf_ty_timerType            timer_type;
        Sdf_ty_TimertimeOutFunc     p_uatk_timer_cbk;
        Sdf_ty_pvoid                p_uatk_ctxt_info;
    }icf_ssa_timer_data_st;

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SSA_TYPES_H__ */
