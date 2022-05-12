/*------------------------------------------------------------------------------
 *      
 * File name        : icf_regm_types.h
 *
 * Purpose          : This file contains the datatypes used by RGM.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 20-Dec-2004  Jalaj Negi      Non Service LLD     Initial
 * 07-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD     Coding for IPSec support
 * 06-Jan-2006	Jyoti Narula	SPR 8063/ 9043		new members in rgm_nm_info to fetch Ik/Ck
 * 15-Jan-2008  Rajiv Kumar     ICF 8.0             Registration Procedure
 *                                                  Enhancements
 * 22-Jul-2009  Anuradha Gupta  SPR 18940           Modified icf_rgm_glb_data_st
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_REGM_TYPES_H__
#define __ICF_REGM_TYPES_H__

#include "icf_map_extif_feature_flags.h"
#include "icf_cfg_internal_types.h"
#include "icf_common_types.h"
#include "icf_regm_defs.h"
#if defined ICF_NW_ACTIVATE || defined IMS_CLIENT
#include "icf_nm_types.h"
#endif



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
 * Type Name        : icf_rgm_fsm_func_ptr_t
 * Description      : This is the function pointer data type used by RGM 
 *                    state machine.
 *
 *----------------------------------------------------------------------------*/
typedef icf_return_t (*icf_rgm_fsm_func_ptr_t)
    (icf_crm_pdb_st *p_crm_pdb);

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_glb_data_st
 * Description      : This is the global data structure for registration. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_rgm_fsm_func_ptr_t icf_rgm_state_machine[ICF_RGM_MAX_STATES]
        [ICF_RGM_MAX_EVENTS];

    /* SPR 18940: Instead of maintaing the ongoing reg counter at global
     * level, it should be maintained per line so that new registration
     * request on different line is not rejected.
     */ 
    icf_uint16_t   reg_count_for_line[ICF_MAX_NUM_OF_LINES];
    
#ifdef IMS_CLIENT
    icf_uint8_t reg_ongoing;
    icf_uint8_t     last_buffer_id;
#ifdef ICF_IPSEC_ENABLED
    icf_rgm_context_st *p_sa_resp_awaiting;
 /* Parameters for IP Security */

    /* context of the latest SA */
    icf_ipsec_sa_context_st          *p_new_sa_context;

    /* context of the previously created SA in case old SA exists even after a 
     * new one has been created */
    icf_ipsec_sa_context_st          *p_old_sa_context;

    /* port and spi values to be sent in Security-Client header by SSA */
    icf_ipsec_security_params_st     *p_security_client_params;

    /* this identifies that NM request is sent for which of the existing SAs */
    /* invalid identifier */
#define ICF_RGM_IPSEC_SA_NONE	    0x00
    /* action performed on old SA */
#define ICF_RGM_IPSEC_SA_OLD		    0x01
    /* action performed on new SA */
#define ICF_RGM_IPSEC_SA_NEW		    0x02
    /* action performed on all existing SAs */
#define ICF_RGM_IPSEC_SA_ALL		    0x04

    icf_uint8_t		                sa_identifier;

    /* this will store the system time in seconds */
    icf_time_t		                prev_timestamp;


    /* bitmask to find whether response has been received from NM for all 
     * unidirectional SAs. It will have a bit for each of the 4 possible spi 
     * values generated at UE for old and new SAs. A bit will be set in this 
     * bitmask while constructing NM request and reset on receiving response 
     * from NM for a particular spi value. The main purpose of this bitmask is
     * to check the number of responses awaited from NM */

    /* first bit from right identifies UE spi_s for new SA */
#define ICF_RGM_IPSEC_NEW_SA_UE_SPI_S        0x01
    /* second bit from right identifies UE spi_c for new SA */
#define ICF_RGM_IPSEC_NEW_SA_UE_SPI_C	    0x02
    /* third bit from right identifies P-CSCF spi_s for new SA */
#define ICF_RGM_IPSEC_NEW_SA_PROXY_SPI_S	    0x04
    /* fourth bit from right identifies P-CSCF spi_c for new SA */
#define ICF_RGM_IPSEC_NEW_SA_PROXY_SPI_C	    0x08
 
    /* fifth bit from right identifies UE spi_s for old SA */
#define ICF_RGM_IPSEC_OLD_SA_UE_SPI_S		0x10
    /* sixth bit from right identifies UE spi_c for old SA */
#define ICF_RGM_IPSEC_OLD_SA_UE_SPI_C		0x20
    /* seventh bit from right identifies P-CSCF spi_s for old SA */
#define ICF_RGM_IPSEC_OLD_SA_PROXY_SPI_S	    0x40
    /* eighth bit from right identifies P-CSCF spi_c for old SA */
#define ICF_RGM_IPSEC_OLD_SA_PROXY_SPI_C	    0x80

    /* identifier for all the bits in new SA */
#define ICF_RGM_IPSEC_NEW_SA_SPI_ALL         0x0F
    /* identifier for all the bits in old SA */
#define ICF_RGM_IPSEC_OLD_SA_SPI_ALL         0xF0
    /* identifier for all the bits in both new and old SA */
#define ICF_RGM_IPSEC_BOTH_SA_SPI_ALL        0xFF

    icf_uint8_t		req_bitmask;

    /* locally supported security algorithms */
    icf_ipsec_self_algo_st    *p_ue_algo_list;

	/* Pointer to sigsa context */
    icf_void_t          *p_sigsa_ctxt;

#endif /* ICF_IPSEC_ENABLED */

#endif /* IMS_CLIENT */
}icf_rgm_glb_data_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_modify_user_st
 * Description      : This is the structure which makes single entry of the 
 *                    local context prepared by RGM module for processing
 *                    Set Self Identifier API. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t index_in_api;
    icf_uint8_t status;
    icf_rgm_context_st *p_rgm_context;
}icf_rgm_modify_user_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_modify_user_not_all_lines_list_st
 * Description      : This is the structure for local context prepared by RGM
 *                    module for processing Set Self Identifier API in case
 *                    all lines field is set to ICF_FALSE. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t num_of_elements;
    icf_rgm_modify_user_st local_context[ICF_MAX_NUM_OF_CONFIG_ELEMENTS];
}icf_rgm_modify_user_not_all_lines_list_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_modify_user_all_lines_list_st
 * Description      : This is the structure for local context prepared by RGM
 *                    module for processing Set Self Identifier API in case
 *                    all lines field is set to ICF_TRUE. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_line_id_t num_of_elements;
    icf_rgm_modify_user_st local_context[ICF_MAX_NUM_OF_LINES];
}icf_rgm_modify_user_all_lines_list_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_set_reg_params_node_st
 * Description      : This is the structure of single node prepared by 
 *                    RGM module for processing Set Registration Parameters
 *                    API. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_address_st *p_user_address;
    icf_uint8_t status;
    icf_rgm_context_st *p_rgm_context;
}icf_rgm_set_reg_params_node_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_set_reg_params_block_st
 * Description      : This is the structure which makes single entry of the 
 *                    local context prepared by RGM module for processing
 *                    Set Registration Parameters API. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t index_in_api;
    icf_uint8_t num_of_nodes;
    icf_rgm_set_reg_params_node_st *p_node;
}icf_rgm_set_reg_params_block_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_set_reg_params_not_all_lines_list_st
 * Description      : This is the structure for local context prepared by RGM
 *                    module for processing Set Registration Parameters API
 *                    in case all lines field is set to ICF_FALSE. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t num_of_elements;
    icf_rgm_set_reg_params_block_st 
        local_context[ICF_MAX_NUM_OF_CONFIG_ELEMENTS];
}icf_rgm_set_reg_params_not_all_lines_list_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_set_reg_params_all_lines_list_st
 * Description      : This is the structure for local context prepared by RGM
 *                    module for processing Set Registration Parameters API
 *                    in case all lines field is set to ICF_FALSE. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_line_id_t num_of_elements;
    icf_rgm_set_reg_params_block_st local_context[ICF_MAX_NUM_OF_LINES];
}icf_rgm_set_reg_params_all_lines_list_st;


ICF_CRM_START
/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_register_resp_st
 * Description      : This is the structure used between functions of
 *					  REGM to indicate the response to be sent over.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_rgm_context_st	*p_rgm_context;
	icf_crm_pdb_st		*p_crm_pdb;
	
	icf_app_id_t			app_id;/* Used when the options_flag is set with
									* SINGLE_APP value */


	icf_register_req_st		*p_request_payload;

	icf_uint16_t			options_flag;
#define	ICF_RGM_RESPONSE_CFM			0x01 /* Prepare CFM response */
#define	ICF_RGM_RESPONSE_IND			0x02 /* Prepare IND response*/
#define ICF_RGM_REQUEST_COPY			0x04 /* Copy To/ From values from request */
#define ICF_RGM_CTXT_COPY			0x08 /* Copy To/ From values from Rgm_ctxt */
#define ICF_RGM_FILL_URIS			0x10 /* Fill the P-associated Uris list */
#define ICF_RGM_SINGLE_APP			0x20 /* Send response only to single
												application given by app_id, If 
												this value not present then send
												to all applications in rgm_ctxt */


	/* SPR 8064: To indicate the presence of warning_hdr_value
	 * and to send it to the application if the app has asked for it */
#define	ICF_RGM_FILL_WARNING		           0x40

	/* To clone the app specific header list to put in CFM on receipt of 200 */
#define ICF_RGM_CLONE_HEADERS		           0x80

/*This options flag specify whether to send the value of retry after in
 * register status indication to application .The value received in the notify
 * message specifying that contact of some registered public user is
 * terminated with event as probation and with retry-after attribute .*/  
#define ICF_RGM_RETRY_AFTER_DURATION_PRESENT   0x100


	icf_return_t			status;/* whether SUCCESS or FAILURE */
	icf_error_t			error_cause;/* Error cause incase of FAILURE*/
	icf_uint8_t			request_type;/* Current state of ongoing Registration
										  * and also used in Deregister response */
        icf_uint16_t                    response_code;

	icf_line_id_t		line_id;

	
}icf_rgm_register_resp_st;


#ifdef IMS_CLIENT
/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_rgm_nm_info_st
 * Description      : This is the structure used between functions of
 *					  REGM to fetch the data from NM
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_rgm_context_st	*p_rgm_context;
	icf_crm_pdb_st		*p_crm_pdb;
/*#if 0  NM not available */ 
	icf_nm_access_info_st	*p_access_info;
/* #endif NM not available */

	icf_string_st			rand;
	icf_string_st			autn;	
	icf_string_st			res;
	icf_string_st			auts;
	icf_string_st			imsCk;
	icf_string_st			imsIk;


	icf_uint16_t				options_flag;
#define ICF_RGM_NM_ACS_INFO				0x01
#define ICF_RGM_NM_AKA_INFO				0x02

	icf_return_t					status;/* The return status: whether data 
											* was correctly fetched or not */

	icf_error_t					ecode;/* It will contain the ecode */


}icf_rgm_nm_info_st;





ICF_CRM_END
#endif/* IMS_CLIENT */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_REGM_TYPES_H__ */
