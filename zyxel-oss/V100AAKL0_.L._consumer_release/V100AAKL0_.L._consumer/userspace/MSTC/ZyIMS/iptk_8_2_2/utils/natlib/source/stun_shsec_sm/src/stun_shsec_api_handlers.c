/******************************************************************************
 * 
 * FILE NAME:   stun_shsec_api_handlers.c
 * 
 * DESCRIPTION: This file contains the api handlers of STUN Shared Secret 
 *				State machine  
 *              
 * REVISION HISTORY:    
 *              Date            Name            Reference       Comments
 *          22-Jun-2006    Abhishek Chhibber    LLD         Initial Version
 *
 ******************************************************************************/

#include "nat_common_internal_api_id.h"
#include "stun_shsec_defs.h"
#include "stun_shsec_types.h"
#include "stun_shsec_macro.h"
#include "stun_shsec_prototypes.h"


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
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb)
{
    nat_return_t                        ret_val = NAT_RETURN_SUCCESS;
    p_stun_shsec_pdb = p_stun_shsec_pdb;
    
	/* Currently not implemented 
		NAT_STUN_TRACE(p_stun_shsec_pdb,NAT_TRACE_UNEXPECTED_EVENT)
	*/

    return ret_val;
}/* End function */


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
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;
    p_stun_shsec_pdb = p_stun_shsec_pdb;

	/* 	Here STUN state machine should : 

	   1. Open TLS connection using port layer api call 
		  (TLS Negotiation and certificate handling shall be done here )
		a) If Failure, then set error-cause as "NAT_ERR_OPEN_SECURE_CONN_FAILED".
		b) else execute 2.
	   2. Generate a new transaction ID 
	   3. Encode the Shared Secret Request.
	   4. Start a timer with a configured value in global shared secret context.
		   	a) If asynchronous timer handling then go to state 2
			b) Else Store the timer id in local context and go to state 1
	   5. Change the is_shared_secret_status to NAT_STUN_SHARED_SECRET_ONGOING.

	*/
		
    
   return ret_val;
}

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
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb)
{
	nat_return_t                         ret_val = NAT_RETURN_SUCCESS;

	/*
		Here the state machine should 
		1. Reset the timer info in the global context.
		2. Send the shared secret request again.
	*/
	ret_val = stun_shsec_00_shared_secret_req(p_stun_shsec_pdb);

	return ret_val;
}
 
/******************************************************************************
 *
 * FUNCTION:        stun_shsec_01_shared_secret_response
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared Secret 
 *					State Machine is in NAT_STUN_SHSEC_01_AWT_RESP state and 
 *					it receives a Shared Secret Response or Shared 
 *					Secret Error response
 *			Current State: NAT_STUN_SHSEC_01_AWT_RESP 
 *                Current Event: NAT_STUN_SHSEC_RESP_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_shsec_01_shared_secret_response(
        INOUT stun_shsec_pdb_st				*p_stun_shsec_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;
    p_stun_shsec_pdb = p_stun_shsec_pdb;

	/* Here the STUN Shared Secret State Machine has received a Shared Secret response

		1. Check if the trasaction id in the response matches the one stored in 
		   local context. If yes then execute 3-5 else ignore this response
		2. Close the TLS Connection using port layer 
		If it is a Shared Secret Error Response 
		
		3. Send a failure to the global nat module mapping 4xx,5xx,6xx same as for 
		   Binding responses 
			
		4. Change the is_shared_secret_status to NAT_STUN_SHARED_SECRET_NOT_STARTED.
		5. Clear the context of the STUN Shared Secret state machine instance.

		If it is a Shared Secret Success Response : 

		6. Store the USERNAME and PASSWORD in the Stun Shared Secret Context
		7. Change the is_shared_secret_status to NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE
		8. Do not clear the STUN Shared Secret context
		9. Start a Refresh Timer of 5 minutes : 
			a) If asynchronous timer handling then go to state 2
			b) Else Store the timer id in local context and go to state 0 and 
	*/

	return ret_val;
}/* End function */




/******************************************************************************
 *
 * FUNCTION:        stun_shsec_01_guard_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared State Machine is in
 *                  in NAT_STUN_SHSEC_01_AWT_RESP state and it receives a Guard Timer
 *	 		  expiry event.
 *                  Current State: NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP
 *                Current Event: NAT_STUN_SHSEC_TIMER_EXPIRY_IND_EVENT
 *
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_shsec_01_guard_timer_expiry(
        INOUT stun_shsec_pdb_st				*p_stun_shsec_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;

     p_stun_shsec_pdb = p_stun_shsec_pdb;

	/* Here the STUN Shared Secret State Machine has received a Timer Expiry 
		Indication  and should : 

		1. Send a failure to the global nat module 
		2. Close the TLS Connection using port layer 
		4. Change the is_shared_secret_status to SHARED_SECRET_PROC_NOT_STARTED
		5. Clear the context of the STUN Shared Secret state machine instance
	*/

	return ret_val;
}/* End function */


 
/******************************************************************************
 *
 * FUNCTION:        stun_shsec_02_shared_secret_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared Secret 
 *			  State Machine is in NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP state
 *                  and t receives a Shared Secret Response or Shared Secret Error response
 *			Current State: NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP
 *                Current Event: NAT_STUN_SHSEC_TIMER_START_RESP
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_shsec_02_shared_secret_response(
        INOUT stun_shsec_pdb_st				*p_stun_shsec_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;
    p_stun_shsec_pdb = p_stun_shsec_pdb;

	/* Here the STUN Shared Secret State Machine has received a Shared Secret response

		1. Check if the trasaction id in the response matches the one stored in 
		   local context. If yes then execute 2-5 else ignore this response

		2. Close the TLS Connection using port layer.
		If it is a Shared Secret Error Response 
		
		3. Send a failure to the global nat module mapping 4xx,5xx,6xx same as for 
		   Binding responses 
			
		4. Change the is_shared_secret_status to NAT_STUN_SHARED_SECRET_NOT_STARTED.
		5. Clear the context of the STUN Shared Secret state machine instance.

		If it is a Shared Secret Success Response : 

		6. Store the USERNAME and PASSWORD in the Stun Shared Secret Context
		7. Change the is_shared_secret_status to NAT_STUN_SHARED_SECRET_COMPLETE
		8. Do not clear the STUN Shared Secret context
		9. Start a Refersh Timer of 5 minutes : 
			a) If asynchronous timer handling then remain in same state
			b) Else Store the timer id in local context and go to state 0.
	*/

	return ret_val;
}/* End function */




/******************************************************************************
 *
 * FUNCTION:        stun_shsec_02_timer_start_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN Shared State Machine is in
 *                  in AWT_GUARD_TIMER_START_RESP state and 
 *					it receives a Timer Start Response
 *                  Current State: NAT_STUN_SHSEC_01_AWT_GUARD_TIMER_START_RESP
 *                  Current Event: NAT_STUN_SHSEC_TIMER_START_RESP
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_shsec_02_timer_start_resp(
        INOUT stun_shsec_pdb_st				*p_stun_shsec_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;
    p_stun_shsec_pdb = p_stun_shsec_pdb;

	/* Here the STUN Shared Secret State Machine has received a Timer Start Response 
	   In this case the STUN State Machine  should
	1. Store the returned in timer id based on timer type in local context.
	2. If timer type is "GUARD TIMER" then 
		a) If "REFRESH TIMER" is not in pending state then moveto state 1
		b) Else remain in same state.

     3.  If timer type is "REFRESH TIMER" then 
		a) If "GUARD TIMER" is not in pending state then move to state 0
		b) Else if "GUARD TIMER" is in Valid state then stop the timer.
		c) Else remain in same state.
		


*/

	return ret_val;
}/* End function */


