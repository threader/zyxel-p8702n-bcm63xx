/*------------------------------------------------------------------------------
 *
 * File name        : icf_sc_common_types.h
 *
 * Purpose          : This file contains the types definitions of SCM module.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 15-Dec-2004  Amit Sharma                           Initial
 * 28-Jan-2005  Navneet Gupta			      Changed Active Services 
 *	                                              Structure
 * 18-Feb-2005  Navneet Gupta			      Added Service Logic Cntxts 
 *                                                    and Service Logic Cntxt Union
 *
 * 21-Feb-2005  Amit Sharma                           To add a ongoing action bitmask value
 *                                                    in chr_ctx
 * 24-Feb-2005  Amit Sharma                           Added cta_ctx
 * 15-Mar-2005  Amit Sharma                           Added rnw_ctx
 * 02-Jun-2005  Amit Sharma     SPR:7535              Added resp_frm_twc in 
 *                                                    expected_event
 * 09-Jul-2007  Amit Sharma     ICF 7.0               Added changes for 7.0 enhancements
 * 24-Dec-2007  Aman Aggarwal   SPR 17865     Added a pointer to call context that 
 *                                            initiated conference request
 * 28-May-2009  Kamal Ashraf    SPR 19672             CSR_1_7129410 Merged
 *     
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_SC_COMMON_TYPES_H__
#define __ICF_SC_COMMON_TYPES_H__

#include "icf_common_types.h"
/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */


/*------------------------------------------------------------------------------
 *
 * Name : icf_service_id_t
 *
 * Description  : This contains the id of the services
 *
 *----------------------------------------------------------------------------*/
typedef icf_uint8_t  icf_service_id_t;

/*------------------------------------------------------------------------------
 *
 * Name : icf_service_list_st
 *
 * Description  : This structure contains the list of active services
 *                and pointers of service logic contexts
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t        active_service_status;
    icf_void_t         *p_sl_ctx[ICF_SCM_MAX_SL];


} icf_service_list_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_expected_events_st
 *
 * Description  : This structure contains parameters to route the message
 *                to service logics
 *
 *----------------------------------------------------------------------------*/
typedef struct
{

    icf_service_id_t     start_proc_media;

    icf_service_id_t     start_proc_signaling;

    icf_service_id_t     resp_frm_chr;

    icf_service_id_t     resp_frm_twc;

    icf_service_id_t     resp_frm_rnw;
    
    icf_service_id_t     resp_frm_cta;
    
    icf_service_id_t     resp_frm_ctu;

    icf_service_id_t     service_ind_handler;

} icf_expected_events_st; /* typedef struct */



/*------------------------------------------------------------------------------
 *
 * Name : icf_service_ctx_st
 *
 * Description  : This structure defines the service context
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_service_ctx_st
{
    /* list of services logic context which are active */
    icf_service_list_st      active_service_list;

    /* this structure stores the expected events and the service ids 
     * which can want to handle them */
    icf_expected_events_st   expected_events;

    /* pointer to self call context */
    icf_call_ctx_st          *p_call_ctx;


} icf_service_ctx_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_chr_ctx_st
 *
 * Description  : This structure defines the service context of CHR service
 *
 *----------------------------------------------------------------------------*/

typedef struct icf_chr_ctx_st
{

#define ICF_CHR_PE_INVALID                0x00
#define ICF_CHR_PE_CALL_HOLD              0x01
#define ICF_CHR_PE_CALL_RESUME            0x02
#define ICF_CHR_PE_CALL_HOLD_NO_NW_OP     0x04
#define ICF_CHR_PE_CALL_RESUME_NO_NW_OP   0x08
#define ICF_CHR_PE_REINVITE_RECVD         0x10

    /*Bit-Mask contains the list of all pending events*/
    icf_uint8_t      pending_event;

    /*Contains the information of state which should be reached next*/
    icf_uint8_t      next_state;

#define ICF_CHR_OA_INVALID    0x00
#define ICF_CHR_OA_CALL_HOLD  0x01
#define ICF_CHR_OA_CALL_RESUME 0x02
#define ICF_CHR_OA_NW_OG_SETUP_FAIL 0x04
#define ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP 0x08
#define ICF_CHR_OA_MMS_FOR_RESUME_REMOTE_RESP 0x10
    /*Bit-Mask contains the action Call Hold or Resume or NW Failure*/
    icf_uint8_t      ongoing_action;

#define ICF_CHR_HOLD_RESUME_TIMER 0X01
#define ICF_CHR_RACE_TIMER 0X02

    icf_uint32_t   chr_timer_id;

    icf_uint32_t   race_timer_id;

    icf_uint8_t      state;

    icf_uint8_t      event;

    icf_boolean_t   ret_error;

    icf_boolean_t    internal_resume_resp;

    icf_boolean_t    internal_hold_resp;

    icf_timer_data_st  *p_chr_timer_data;

    icf_timer_data_st  *p_race_timer_data;
    icf_void_t       *p_pending_req;

} icf_chr_ctx_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_twc_ctx_st
 *
 * Description  : This structure defines the service context of TWC service
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_twc_ctx_st
{
    icf_uint8_t      next_state;

    icf_uint8_t      state;

    icf_uint8_t      event;

    /*Call context of the already existing call on
     * which TWC is invoked*/
    icf_call_ctx_st  *p_call_ctx_b;

    /* Call context of the second call with party C*/
    icf_call_ctx_st  *p_call_ctx_c;

    /*Call context of initial conference req*/
    icf_call_ctx_st  *p_call_ctx_req;

    icf_timer_data_st  *p_twc_timer_data;

    icf_timer_id_t     twc_timer_id;

    icf_boolean_t      chr_for_call_b;

    icf_boolean_t      chr_for_call_c;

} icf_twc_ctx_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_ctu_ctx_st
 *
 * Description  : This structure defines the service context of CTU service
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_ctu_ctx_st
{
    /*Stores the call context on which CTU is initiated*/
    icf_call_ctx_st      *p_call_ctx;

    /*Stores the transferred party address from
     *CTU request received from PA*/
    icf_address_st       transferred_addr;

    /*Store the information to toggle PI*/
    icf_boolean_t        toggle_pi;

    /*Stores CTU timer-Id*/
    icf_timer_id_t       ctu_timer_id;

    /*Stores CTU timer data structure*/
    icf_timer_data_st    *p_ctu_timer_data;

    /*Indicate that PA has initiated call release or not*/
    icf_boolean_t        call_rel_initiated;

    /*It is used to store the current state of FSM*/
    icf_uint8_t          state;

    /*It is used to store the incoming event*/
    icf_uint8_t          event;
  
    icf_large_string_st *p_uri_parameter;

    /*This parameter will be TRUE is CTU service is invoked from XCONF as
     *internal request, by default it will be set as FALSE*/
    icf_boolean_t       internal_refer_req;

} icf_ctu_ctx_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_cta_ctx_st
 *
 * Description  : This structure defines the service context of CTA service
 *
 *----------------------------------------------------------------------------*/

typedef struct icf_cta_ctx_st
{
        /*Stores the call to be transferred*/
        icf_call_ctx_st      *p_call_ctx_b;

        /*Stores the call context to which REFER is send*/
        icf_call_ctx_st      *p_call_ctx_c;

        /*Stores the transferred party call id from
         *CTA request received from PA*/
        icf_call_id_t        transferred_id;

        /*Stores CTA timer-Id*/
        icf_timer_id_t       cta_timer_id;

        /*Stores CTA timer data structare*/
        icf_timer_data_st    *p_cta_timer_data;

        /*Indicate that PA has initiated call release or not*/
        icf_boolean_t        call_rel_initiated;

        /*Call clear reason received from PA*/
        icf_boolean_t        call_clear_reason;

        /*Marked true when call clear is initiated by party B*/
        icf_module_id_t      call_clear_initiator_b;

        /*Marked true when call clear is initiated by party C*/
        icf_module_id_t      call_clear_initiator_c;

        /*It is used to store the current state of FSM*/
        icf_uint8_t          state;

        /*It is used to store the incoming event*/
        icf_uint8_t          event;

        
        icf_boolean_t      chr_for_call_b;

        icf_boolean_t      chr_for_call_c;

        /*This parameter will be TRUE is CTA service is invoked from XCONF as
         *internal request, by default it will be set as FALSE*/
        icf_boolean_t       internal_refer_req;

}icf_cta_ctx_st;


/*------------------------------------------------------------------------------
*
* Name : icf_rnw_ctx_st
*
* Description  : This structure defines the Service Logic Context for RNW
*                service
*
*-----------------------------------------------------------------------------*/

    typedef struct
    {
        /*Current FSM state*/
        icf_uint8_t            state;

        /*Received event*/
        icf_uint8_t            event;

        /*Pointer to the Call Context of call with Party B*/
        icf_call_ctx_st        *p_call_ctx_b;

        /*Pointer to the Call Context of call with Party C*/
        icf_call_ctx_st        *p_call_ctx_c;

        icf_uint8_t            pending_event;

        /*Next state of RNW FSM*/
        icf_uint8_t            next_state;

        /*Timer id returned when rnw timer started*/
        icf_timer_id_t         rnw_timer_id;

        /*Timer Data pointer for RNW timer*/
        icf_timer_data_st      *p_rnw_timer_data;

       /*cta_handling-adutta*/
    /* Pointer to save the new call ctx allocated temperorily till the 
     * application response is received
     */
      icf_call_ctx_st            *p_new_call_ctx;

    /* CSR_1_7129410 Merged SPR 19672 Start */
    /*This bitmask is set TRUE when RNW sends NOTIFY on the network*/		
    icf_boolean_t	   notify_sent;
    icf_boolean_t	   merge_media_sent;
    /* CSR_1_7129410 Merged SPR 19672 End */

    } icf_rnw_ctx_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_cfw_ctx_st
 *
 * Description  : This structure defines the service context of CFW service
 *
 *----------------------------------------------------------------------------*/

typedef struct icf_cfw_ctx_st
{
    icf_uint8_t temp;
}icf_cfw_ctx_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_xconf_ctx_st
 *
 * Description  : This structure defines the service context of XCONF service
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
/*Stores the call context on which XCONF is initiated*/
    icf_call_ctx_st* p_call_ctx;

/*It is used to store the current state of XCONF FSM*/
    icf_uint8_t state;

/*It is used to store the incoming event*/
    icf_uint8_t event;

/*This stores current congoing action*/
    icf_uint8_t ongoing_action;

/* callid for the call between conference server and host */
    icf_call_id_t   conference_id;

/* It is used to tell that this conference req is incoming conference req 
 * or outgoing Conference request */
    icf_boolean_t   incoming_conf_req;

/* It is used to tell that subscribe is required or not */
    icf_boolean_t subs_done;

 /* This boolean flag will indicate that delete party uri should be fetched 
    from xconf ctx or recv mesg for sending in delete party resp*/
    icf_boolean_t del_uri_from_xconf_ctx;

 /* This will be set when sending the REFER request to CTU using the recv
    payload of delete party request. This is required for storing the delete
    party uri for delete party resp formation as recv payload will not be
    having the same in success scenario. */
    icf_address_st delete_party_uri;
    
    /* Flag indicating that add_xconf_party_resp is sent.*/
    icf_boolean_t add_xconf_party_resp_sent;

}icf_xconf_ctx_st;

/*------------------------------------------------------------------------------
 *
 * Name : icf_sl_ctx_ut
 *
 * Description  : This structure defines the service context of CFW service
 *
 *----------------------------------------------------------------------------*/

typedef union
{
    icf_chr_ctx_st chr_ctx;
    icf_twc_ctx_st twc_ctx;
    icf_ctu_ctx_st ctu_ctx;
    icf_cta_ctx_st cta_ctx;
    icf_rnw_ctx_st rnw_ctx;
    icf_cfw_ctx_st cfw_ctx;
    icf_xconf_ctx_st xconf_ctx;
}icf_srvc_ctx_lgc_ut;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
