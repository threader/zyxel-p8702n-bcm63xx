/*------------------------------------------------------------------------------
 *
 * File name        : icf_cfg_internal_types.h
 *
 * Purpose          : This file contains the data structures used by CFG.
 *
 * Revision History :
 *
 * Date         Name            	Ref#        Comments
 * --------     ------------    	----        ---------------
 * 21-Jan-2005  Shefali Aggarwal	ICF LLD	Initial
 * 05-Dec-2005	Jyoti Narula		ICF 4.0		Changes under ICF_4_0_CRM_START/ END
 * 07-Dec-2005  Rohit Aggarwal      ICF 4.0 CDD Coding for IPSec support
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 7-Sep-2007   Deepti Goyal        SPR 16019   Registrar mode change variable.
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CFG_INTERNAL_TYPES_H__
#define __ICF_CFG_INTERNAL_TYPES_H__

#include "icf_common_types.h"
#include "icf_common_interface_types.h"

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


/* Global variable to store the activation flag value recd in payload
 * temporarily
 */ 
icf_uint16_t                         act_status[ICF_MAX_NUM_OF_LINES];


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_rgm_pdb_st
 * Description :  Structure for RGM module's pdb. This is filled in by the CFG 
 *                and used by REGM to process the API's received.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_boolean_t			all_lines;
    icf_uint8_t 				num_of_elements;
    icf_uint8_t 				api_indexes[ICF_MAX_NUM_OF_CONFIG_ELEMENTS];

}icf_rgm_pdb_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_crm_pdb_st
 * Description :  Structure for CFG and RGM module's pdb
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* pointer to the global pdb received by entry point */
    icf_glb_pdb_st           *p_glb_pdb;
    
    /* pointer to the internal message received by entry point */
    icf_internal_msg_st      *p_internal_msg;

    /* 
     * flag that determines whether a response needs to be sent for an API - 
     * init value is ICF_TRUE
     */
    icf_boolean_t            resp_to_be_sent;

    /* result of request processing - to be sent in response message */
    icf_result_t             result;
    
    /* error cause in request processing - to be sent in response message */
    icf_error_t              error_cause;
    
	/* pointer to the config data structure */
    icf_config_data_st       *p_config_data;
    
	/* pointer to the line data structure */
    icf_line_data_st         *p_line_data[ICF_MAX_NUM_OF_LINES];
    
    /* 
	 * This array will contain the boolean flag set for the lines 
     * whose pointers are passes in the above array. Rest values are false 
     * This is used in case all_lines field is true.
     */
    icf_boolean_t            lines_present[ICF_MAX_NUM_OF_LINES];
    
    /* 
	 * This boolean flag is set by the functions after validating the 
     * API data. It is true if the data is valid and false if invalid.
     */
    icf_boolean_t		    is_valid_param;
    
    /* 
	 * This is used to indicate the valid indexes in case all_lines field
     * is false in the API. The dupliacte entries are removed from
     * the API and entries to be actually processed are kept in this array
     */
    icf_boolean_t            api_indexes[ICF_MAX_NUM_OF_CONFIG_ELEMENTS];

    /* This pointer is used to indicate the lines that are configured in order
     * to add service params on those lines. This is used only for the case 
     * when all_lines is false.
     */
    icf_boolean_t            requested_lines[ICF_MAX_NUM_OF_LINES];
        
    /* This pointer is used to copy the payload of the received msg in the
     * pdb
     */
    icf_void_t               *p_recv_payload;
    
	/* pdb for Registration module */
    icf_rgm_pdb_st           rgm_pdb;

    /* This Pointer will be initialized to NULL by CFG before invoking
     * any entry point function of REGM Module
     */
    icf_rgm_context_st       *p_rgm_context;

ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
    /* This Pointer will be initialized to NULL by CFG before invoking
     * any entry point function of Network Access Module
     */
    icf_netact_ctxt_st       *p_netact_context;
	
	/* This flag will be set when network deactivation has happened
     * and all users are being forcefully deregistered */
	icf_boolean_t			network_deactivated;
#endif
ICF_CRM_END

    /* This boolean flag is used to indicate whether there are any ADD/Delete 
     * requests in a registration API received after init complete. */ 

    icf_boolean_t            add_del_recd;

    /* This is used for traces only */
    icf_line_id_t            line_id;

    /* This is used for traces only */
    icf_int32_t              user_id;
    icf_int32_t              expiry;
    
    /* pointer to ecode */
    icf_error_t              *p_ecode;

	ICF_4_0_CRM_START
	
	/* Duration stored in case of NOTIFY with shortened */
	icf_duration_t			shortened_expiry;

ICF_4_0_CRM_END
     icf_boolean_t       request_recvd_from_app;
     icf_boolean_t       auto_activated;
	
	 icf_app_id_t		app_id;

     /* To store the value of retry_after received
      * with probation event, the most recent received 
      * NOTIFY message. */
     icf_uint32_t        new_retry_value;

     /* The app_id extracted from the contact
      * currently being processed from the NOTIFY message */
     icf_app_id_t        recd_con_app_id;

     /* To store the event received in the contact element 
      * in the NOTIFY message */
     icf_uint8_t         recd_con_event;


     
    /* Global data init complete flag - indicates that Init complete has been 
     * recd one.  This will point to the value of the flag in p_glb_data. It is 
     * being defined here, to reduce the indirections as it is used a number of 
     * times in the code
     */
    icf_boolean_t          global_data_init_complete;

    icf_void_t             *p_rgm_glb_data;
    icf_transport_mode_t    effective_self_mode;
    icf_string_st           app_name;
    icf_boolean_t			forceful_clear;
    /* SPR 16019: Registrar mode change: Stores the mode received from 
     * application in set_reg_params (modify action)
     */
    icf_transport_mode_t    effective_reg_mode;
} icf_crm_pdb_st;

ICF_CRM_START
/*-----------------------------------------------------------------------------
 * 
 * Structure Name:icf_app_id_list_st   
 * Description: This data type is used to define a structure, which contains a
 *              list of application ids.
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{

	icf_uint8_t		     num_of_elements;
	icf_app_id_t          app_id[ICF_MAX_NUM_APPLICATION];
	
} icf_app_id_list_st;
ICF_CRM_END    

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cfg_net_act_retry_timer_st
 * Description      : This datatype is used to store network activation
 *                    information for retrying network activation when RETRY 
 *					  timer expires.
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    /* Application ID of application that triggered network_activation */
	/* Dummy app_id implies success/failure response need not be sent to any 
		application */
	icf_app_id_t app_id; 
	
	/* Access Point Name specifying service and operator information */
    icf_string_st apn; 
    
	/* Access Network type */                                                                
	icf_uint8_t access_type; 
}icf_cfg_net_act_retry_timer_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_CFG_INTERNAL_TYPES_H__ */  
