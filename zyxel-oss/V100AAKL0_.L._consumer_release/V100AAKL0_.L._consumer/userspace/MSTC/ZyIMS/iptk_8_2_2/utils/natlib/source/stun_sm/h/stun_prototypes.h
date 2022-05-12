/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_prototypes.h
 * 
 * DESCRIPTION: This file contains the prototypes of the functions used 
 *              to implement the functionality of STUN.
 *              
 * REVISION HISTORY:
 *  DATE            NAME            REFERENCE       REASON        
 * ------         ------------      ---------     ----------
 * 24-Nov-2006	Anuradha Gupta     LLD			 Initial Version
 * 14-May-2010	Preksha Gupta	   SPR 20375	 Merged CSR 1-7845448
 *          
 * Copyright 2006, Aricent                                 *
 ******************************************************************************/

#ifndef __STUN_PROTOTYPES_H__
#define __STUN_PROTOTYPES_H__

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */
    
#include "stun_types.h"
 
/*******************************************************************************
 *
 * FUNCTION:        nat_stun_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of STUN state machine.  
 *			  The function checks the API Id in the message 
 *                  received and invokes appropriate handler based on current 
 *                  stun sm state and the event (mapped from API Id).
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/   
nat_return_t nat_stun_process_mesg(
        INOUT nat_internal_msg_st        *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        stun_init
 *
 * DESCRIPTION:     This is the initialization function of STUN State Machine
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_init(
	  INOUT   nat_internal_msg_st        *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        stun_api_to_event
 *
 * DESCRIPTION:     This function maps the messages recieved to events.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_api_to_event(
        INOUT stun_pdb_st             *p_stun_pdb);

/*******************************************************************************
 *
 * FUNCTION:        stun_deinit
 *
 * DESCRIPTION:     This is the deinitialization function of STUN State Machine
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_void_t stun_deinit(INOUT nat_internal_msg_st        *p_internal_msg);

/******************************************************************************
 *
 * FUNCTION:        stun_unexpected_event
 *
 * DESCRIPTION:     This function is invoked when any unexpected event is
 *                  received.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 *****************************************************************************/
nat_return_t stun_unexpected_event(
        INOUT stun_pdb_st             *p_stun_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_00_binding_request
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in IDLE state and it receives Binding Request
 *                  Current State: NAT_STUN_00_IDLE
 *                  Current Event: NAT_STUN_BINDING_REQ
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 *****************************************************************************/
nat_return_t stun_00_binding_request(
        INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_00_refresh_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in IDLE state and it receives a Timer Expiry Indication
 *                  Current State: NAT_STUN_00_IDLE
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t stun_00_refresh_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_00_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in IDLE state and it receives a Release Request
 *                  Current State: NAT_STUN_00_IDLE
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t stun_00_release_req(
     INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_01_retransmit_timer_start
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in AWT_RETRANSMIT_TIMER_START_RESP state and
 *                                      it receives a Timer Start Response
 *                  Current State: NAT_STUN_01_AWT_RETRANSMIT_TIMER_START_RESP
 *                  Current Event: NAT_STUN_TIMER_START_RESP
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t stun_01_retransmit_timer_start(
        INOUT stun_pdb_st             *p_stun_pdb);



/******************************************************************************
 *
 * FUNCTION:        stun_01_retransmit_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in AWT_BIND_RESP state and
 *                                      it receives a Timer Expiry Indication
 *                  Current State: NAT_STUN_01_AWT_BIND_RESP
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/
nat_return_t stun_01_retransmit_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_01_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in AWT_BIND_RESP state and it receives a Binding Reqponse
 *                  Current State: NAT_STUN_01_AWT_BIND_RESP
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of any failure.
 *
 ******************************************************************************/
nat_return_t stun_01_binding_resp(
 	INOUT stun_pdb_st             *p_stun_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_01_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in Awaiting Binding response state and it receives a Release 
 *                  Request.
 *                  Current State: NAT_STUN_01_AWT_BIND_RESP
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/
nat_return_t stun_01_release_req(
     INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_02_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_02_AWT_EXTRA_BIND_RESP state and
 *                        it receives a Binding Response
 *                  Current State: NAT_STUN_02_AWT_EXTRA_BIND_RESP
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_02_binding_resp(
        INOUT stun_pdb_st             *p_stun_pdb);



/******************************************************************************
 *
 * FUNCTION:        stun_02_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in AWT_EXTRA_BIND_RESP state and
 *                                      it receives a Binding Response
 *                  Current State: NAT_STUN_02_AWT_EXTRA_BIND_RESP
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_02_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_02_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in Awaiting Binding response state and it receives a Release 
 *                  Request.
 *                  Current State: NAT_STUN_02_AWT_EXTRA_BIND_RESP
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_02_release_req(
     INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_03_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_03_AWT_START_TIMER_RESP state and
 *                                      it receives a Binding Response
 *                  Current State: NAT_STUN_03_AWT_START_TIMER_RESP
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_03_binding_resp(
        INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_03_start_timer_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_03_AWT_START_TIMER_RESP state and it receives 
 *                  the start timer Response.
 *                  Current State: NAT_STUN_03_AWT_START_TIMER_RESP
 *                  Current Event: NAT_STUN_TIMER_START_RESP_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_03_start_timer_resp(
        INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_03_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_03_AWT_START_TIMER_RESP state and it receives 
 *                  a Release Request.
 *                  Current State: NAT_STUN_03_AWT_START_TIMER_RESP
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_03_release_req(
     INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_04_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK state and
 *                                      it receives a Timer Expiry Indication
 *                  Current State: NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_04_binding_resp(
        INOUT stun_pdb_st             *p_stun_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_04_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK state and
 *                                      it receives a Binding Response
 *                  Current State: NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_04_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_04_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in Awaiting Binding response after ACK state and it receives 
 *                  a Release Request.
 *                  Current State: NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_04_release_req(
     INOUT stun_pdb_st             *p_stun_pdb);


/*******************************************************************************
 *
 * FUNCTION:        stun_get_retransmission_timer_value
 *
 * DESCRIPTION:     This function is used to calculate retransmit timer value.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_get_retransmission_timer_value(
		IN  nat_uint8_t retransmit_counter, 
        OUT nat_duration_t *p_retransmit_timer_value);
		
/*******************************************************************************
 *
 * FUNCTION:        stun_stop_current_timers
 *
 * DESCRIPTION:     This function is used to stop all the running timers
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_stop_current_timers(
		INOUT stun_pdb_st             *p_stun_pdb);

/*******************************************************************************
 *
 * FUNCTION:        stun_stop_timer
 *
 * DESCRIPTION:     This function is used to stop the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_stop_timer(
		INOUT stun_pdb_st             *p_stun_pdb,
        IN    nat_timer_type_t         timer_type);
   
/*******************************************************************************
 *
 * FUNCTION:        stun_start_timer
 *
 * DESCRIPTION:     This function is used to start the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_start_timer(
		INOUT stun_pdb_st             *p_stun_pdb,
		IN    nat_uint8_t             context_id,
        IN    nat_timer_type_t         timer_type,
        IN    nat_duration_t           timer_dur);
             	
/*******************************************************************************
 *
 * FUNCTION:        stun_create_and_send_binding_request
 *
 * DESCRIPTION:     This function is used to create / encode the binding request
 *                  message and sending it to network.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_create_and_send_binding_request(
		INOUT stun_pdb_st             *p_stun_pdb);
        
   
/*******************************************************************************
 *
 * FUNCTION:        stun_retransmit_binding_request
 *
 * DESCRIPTION:     This function is used to retranmit the binding request.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_retransmit_binding_request(
		INOUT stun_pdb_st             *p_stun_pdb,
        IN    nat_uint8_t             context_id);
     
/*******************************************************************************
 *
 * FUNCTION:        stun_map_response_code_to_error_cause
 *
 * DESCRIPTION:     This function is to map the network response code to 
 *                  application specific error codes.
 *                  
 * RETURNS:         void                 
 *
 ******************************************************************************/
nat_void_t stun_map_response_code_to_error_cause(
		IN   nat_error_t response_code,
        OUT  nat_error_t *p_error_cause);

/*******************************************************************************
 *
 * FUNCTION:        stun_pending_timer_info
 *
 * DESCRIPTION:     This function is to map the network response code to 
 *                  application specific error codes.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS : some timer response is pending
 *                  NAT_RETURN_FAILURE : no timer response is pending                 
 *
 ******************************************************************************/
nat_return_t stun_pending_timer_info(
        INOUT stun_pdb_st             *p_stun_pdb);
        
/*******************************************************************************
 *
 * FUNCTION:        stun_process_initial_binding_success_response
 *
 * DESCRIPTION:     This function is used to process the initial binding 
 *                  success response.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_process_initial_binding_success_response(
		INOUT stun_pdb_st             *p_stun_pdb);
     
/*******************************************************************************
 *
 * FUNCTION:        stun_process_subs_binding_success_response
 *
 * DESCRIPTION:     This function is used to process the subsequent binding 
 *                  success response.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_process_subs_binding_success_response(
		INOUT stun_pdb_st             *p_stun_pdb);
         
/*******************************************************************************
 *
 * FUNCTION:        stun_process_binding_error_response
 *
 * DESCRIPTION:     This function is used to process the binding 
 *                  error response.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_process_binding_error_response(
		INOUT stun_pdb_st             *p_stun_pdb);

/*******************************************************************************
 *
 * FUNCTION:        stun_create_and_send_binding_refresh_request
 *
 * DESCRIPTION:     This function is used to create / encode the binding request
 *                  message (refresh) and sending it to network.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_create_and_send_binding_refresh_request(
		INOUT stun_pdb_st             *p_stun_pdb);
                          	
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STUN_PROTOTYPES_H__ */



