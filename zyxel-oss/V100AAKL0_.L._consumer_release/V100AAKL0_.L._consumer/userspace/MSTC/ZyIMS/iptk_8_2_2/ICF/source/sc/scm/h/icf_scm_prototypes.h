/*******************************************************************************
*
* FILE NAME    : icf_scm_prototypes.h
*
* DESCRIPTION  : Contains the prototypes of functions used in SCM service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   20-Jan-2005 Navneet Gupta     None           Initial
*   27-Feb-2006 Amit Sharma                  Merged code for ICF
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_SCM_PROTOTYPES_H__
#define __ICF_SCM_PROTOTYPES_H__
#include "icf_map_extif_feature_flags.h"
#ifdef ICF_SERVICE_ENABLE
#include "icf_scm_types.h"

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

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_init
 *
 * DESCRIPTION   : This is the initialization function of SCM module
 *                 which is called by DBM at the time of system startup
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t   icf_scm_init(
                INOUT   icf_glb_pdb_st        *p_glb_pdb,
                OUT     icf_void_t            **p_p_scm_data,
                OUT     icf_error_t           *p_ecode
                );

/*******************************************************************************
*
* FUNCTION NAME: icf_scm_process_mesg
*
* DESCRIPTION  : This is the Entry Point function of SCM service module
*
*
* RETURNS:
*
*******************************************************************************/
icf_return_t   icf_scm_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        );    
/*******************************************************************************
*
* FUNCTION NAME : icf_scm_route_message
*
* DESCRIPTION   : This function routes the message to the appropriate service 
*		  module.
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_scm_route_message(
		INOUT		icf_scm_pdb_st   *p_pdb,
		INOUT		icf_error_t      *p_ecode
				);

/*******************************************************************************
*
* FUNCTION NAME : icf_scm_validate_new_service 
*
* DESCRIPTION   : This function checks if new service can be activated.
*                 This is decided based on other active services on the 
*                 call(s) involved.
*
* RETURNS       : icf_return_t
*
*******************************************************************************/


icf_result_t icf_scm_validate_new_service
(
 INOUT 		icf_scm_pdb_st	*p_pdb,
 INOUT 		icf_boolean_t	*p_valid, 
 INOUT		icf_error_t		*p_ecode
);

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_process_return_status
 *
 * DESCRIPTION   : This function checks status when function call to service 
 *                 logic module returns. Based on the status, it does 
 *                 various actions like deallocating the service logic context, 
 *                 setting the request flag, sending internal events 
 *                 to other modules etc.
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t icf_scm_process_return_status
(
 INOUT 	icf_scm_pdb_st	*p_pdb,
 INOUT 	icf_error_t		*p_ecode
);

/*******************************************************************************
*
* FUNCTION NAME : icf_scm_send_service_failure_resp
*
* DESCRIPTION   : This function sends a failure response for the service 
* 		          request recieved.
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_scm_send_service_failure_resp(

		INOUT		icf_scm_pdb_st   *p_pdb,
		INOUT		icf_error_t      *p_ecode
);

/*******************************************************************************
*
* FUNCTION NAME : icf_scm_get_handler_module
*
* DESCRIPTION   : This function determines the handler Service Logic Module 
*                 responsible for the handling of the API received. If the 
*		          handler can't be determined a value of 
*		          ICF_INT_MODULE_INVALID is returned
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_scm_get_handler_module(
		IN     icf_scm_pdb_st   *p_pdb,
		INOUT  icf_service_id_t *handler_module,
		INOUT  icf_error_t      *p_ecode
	);

/*******************************************************************************
*
* FUNCTION NAME : icf_scm_check_subs_and_actv
*
* DESCRIPTION   : This function checks the subscription and activation 
*                 status for Service requests and returns failure if the 
*                 requested service is not subscribed or activated on the line.
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_scm_check_subs_and_actv(
		INOUT   icf_scm_pdb_st   *p_pdb,
                  OUT   icf_boolean_t    *p_valid,
		INOUT   icf_error_t      *p_ecode
	);
#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_trace
 * 
 * DESCRIPTION   : This is the trace function for SCM
 *
 * RETURNS       : icf_void
 *
 ******************************************************************************/
icf_void_t   icf_scm_trace(
        INOUT icf_scm_pdb_st *p_pdb,
        IN    icf_trace_id_t trace_id
        );
#endif
/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_error
 * 
 * DESCRIPTION   : This is the error function for SCM
 *
 * RETURNS       : icf_void
 *
 ******************************************************************************/
icf_void_t   icf_scm_error(
        INOUT icf_scm_pdb_st *p_pdb,
        IN    icf_error_t ecode);


/*******************************************************************************
*
* FUNCTION NAME : icf_scm_alloc_srvc_cntxt_blk 
*
* DESCRIPTION   :This function calls the dbm function to allocate the 
*               service ctx block and initializes the parameters of the
*               service ctx created. 
*
*******************************************************************************/
icf_return_t icf_scm_alloc_srvc_cntxt_blk(
        INOUT icf_scm_pdb_st     *p_pdb,
        OUT   icf_service_ctx_st **p_p_service_ctx,
        INOUT icf_error_t        *p_ecode);


/*******************************************************************************
*
* FUNCTION NAME : icf_scm_alloc_srvc_cntxt_lgc_blk 
*
* DESCRIPTION   :This function calls the dbm function to allocate the 
*               service logic ctx block and initializes the parameters of the
*               service logic ctx created. 
*
*******************************************************************************/
icf_return_t icf_scm_alloc_srvc_cntxt_lgc_blk(
        INOUT icf_scm_pdb_st     *p_pdb,
        OUT   icf_void_t          **p_p_service_ctx_logic_blk,
        INOUT icf_error_t        *p_ecode);    


/* dummy sl prototypes */
icf_return_t   icf_chr_process_mesg(
		INOUT   icf_internal_msg_st   *p_internal_msg
		);
icf_return_t   icf_twc_process_mesg(
		INOUT   icf_internal_msg_st   *p_internal_msg
		);
icf_return_t   icf_cta_process_mesg(
		INOUT   icf_internal_msg_st   *p_internal_msg
		);
icf_return_t   icf_ctu_process_mesg(
		INOUT   icf_internal_msg_st   *p_internal_msg
		);
icf_return_t   icf_rnw_process_mesg(
		INOUT   icf_internal_msg_st   *p_internal_msg
		);
icf_return_t   icf_cfw_process_mesg(
		INOUT   icf_internal_msg_st   *p_internal_msg
		);
icf_return_t   icf_xconf_process_mesg(
		INOUT   icf_internal_msg_st   *p_internal_msg
		);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* __ICF_CHR_PROTOTYPES_H__  */
