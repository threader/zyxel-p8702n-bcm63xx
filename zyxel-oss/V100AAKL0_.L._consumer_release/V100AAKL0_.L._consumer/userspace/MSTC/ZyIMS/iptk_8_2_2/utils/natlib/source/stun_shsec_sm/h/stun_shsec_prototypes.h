/******************************************************************************
*                                    
* FILE NAME    : nat_shsec_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT Lifetime
*				 Discovery Prototypes APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
*  14-July-2006  Anuradha Gupta                LLD           Initial version
* Copyright 2006, Aricent                       
*                                    
*******************************************************************************/



#ifndef __NAT_SHSEC_PROTOTYPES_H_

#define __NAT_SHSEC_PROTOTYPES_H_

#include "nat_types.h"
#include "stun_shsec_types.h"

#ifdef __cplusplus

extern C
{
#endif

/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_init
 *
 * DESCRIPTION:     This is the initialization function of STUN Shared Secret State
 *                        Machine.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure. 
 *
 ******************************************************************************/
nat_return_t stun_shsec_init(
                INOUT   nat_internal_msg_st        *p_internal_msg);
                
/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_deinit
 *
 * DESCRIPTION:     This is the deinitialization function of STUN Shared Secret 
 *                  State  Machine.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure. 
 *
 ******************************************************************************/
nat_void_t stun_shsec_deinit(
                INOUT   nat_internal_msg_st        *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_api_to_event
 *
 * DESCRIPTION:     This function maps the messages recieved to events.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/

nat_return_t stun_shsec_api_to_event(
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb);

/*******************************************************************************
 *
 * FUNCTION:        nat_stun_shsec_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of Shared Secret state Machine
 *                        The function checks the API Id in the message received and invokes
 *                  appropriate handler based on current state and the event
 *                    (mapped from API Id).
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/

nat_return_t nat_stun_shsec_process_mesg(
        INOUT nat_internal_msg_st        *p_internal_msg);


/******************************************************************************
 *
 * FUNCTION:        stun_shsec_unexpected_event
 *
 * DESCRIPTION:     This function is invoked when any unexpected event is
 *                  received.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 *****************************************************************************/
nat_return_t stun_shsec_unexpected_event(
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_shsec_00_shared_secret_req
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared Secret State Machine is in
 *                  in IDLE state and it receives Shared Secret Request
 *                  Current State: NAT_STUN_SHSEC_00_IDLE
 *                  Current Event: NAT_STUN_SHSEC_REQ_EVENT
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 *****************************************************************************/

nat_return_t stun_shsec_00_shared_secret_req(
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_shsec_00_refresh_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared Secret State Machine is in
 *                  in IDLE state and it refresh timer expiry event.
 *                  Current State: NAT_STUN_SHSEC_00_IDLE
 *                  Current Event: NAT_STUN_SHSEC_TIMER_EXPIRY_IND_EVENT
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 *****************************************************************************/

nat_return_t stun_shsec_00_refresh_timer_expiry(
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_shsec_01_shared_secret_response
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared Secret
 *                                      State Machine is in NAT_STUN_SHSEC_01_AWT_RESP state and
 *                                      it receives a Shared Secret Response or Shared
 *                                      Secret Error response
 *                      Current State: NAT_STUN_SHSEC_01_AWT_RESP
 *                Current Event: NAT_STUN_SHSEC_RESP_EVENT
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_shsec_01_shared_secret_response(
        INOUT stun_shsec_pdb_st              *p_stun_shsec_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_shsec_01_guard_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared State Machine is in
 *                  in NAT_STUN_SHSEC_01_AWT_RESP state and it receives a Guard Timer
 *                        expiry event.
 *                  Current State: NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP
 *                Current Event: NAT_STUN_SHSEC_TIMER_EXPIRY_IND_EVENT
 *
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_shsec_01_guard_timer_expiry(
        INOUT stun_shsec_pdb_st                 *p_stun_shsec_pdb);

/******************************************************************************
 *
 * FUNCTION:        stun_shsec_02_shared_secret_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared Secret
 *                        State Machine is in NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP state
 *                  and t receives a Shared Secret Response or Shared Secret Error response
 *                      Current State: NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP
 *                Current Event: NAT_STUN_SHSEC_TIMER_START_RESP
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_shsec_02_shared_secret_response(
        INOUT stun_shsec_pdb_st                    *p_stun_shsec_pdb);


/******************************************************************************
 *
 * FUNCTION:        stun_shsec_02_timer_start_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared State Machine is in
 *                  in AWT_GUARD_TIMER_START_RESP state and
 *                                      it receives a Timer Start Response
 *                  Current State: NAT_STUN_SHSEC_01_AWT_GUARD_TIMER_START_RESP
 *                  Current Event: NAT_STUN_SHSEC_TIMER_START_RESP
 *
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in
 *                  case of any failure.
 *
 ******************************************************************************/

nat_return_t stun_shsec_02_timer_start_resp(
        INOUT stun_shsec_pdb_st                    *p_stun_shsec_pdb);

/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_stop_current_timers
 *
 * DESCRIPTION:     This function is used to stop all running timers.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_shsec_stop_current_timers(
		INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb);
		
/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_stop_timer
 *
 * DESCRIPTION:     This function is used to stop the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_shsec_stop_timer(
		INOUT stun_shsec_pdb_st        *p_stun_shsec_pdb,
        IN    nat_timer_type_t         timer_type);
        
/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_start_timer
 *
 * DESCRIPTION:     This function is used to start the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_shsec_start_timer(
		INOUT stun_shsec_pdb_st        *p_stun_shsec_pdb,
		IN    nat_uint32_t             context_id,
        IN    nat_timer_type_t         timer_type,
        IN    nat_duration_t           timer_dur);
        




#ifdef __cpluplus

}
#endif

#endif
