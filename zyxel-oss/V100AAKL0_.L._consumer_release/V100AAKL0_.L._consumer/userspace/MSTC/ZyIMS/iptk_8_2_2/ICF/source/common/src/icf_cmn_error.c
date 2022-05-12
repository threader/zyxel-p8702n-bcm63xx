/*************************************************************************
 *
 *     FILENAME     :    icf_cmn_error.c
 *
 *     DESCRIPTION  :  This file defines the init of error data.
 *
 *     Revision History    :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         Dec 2004    Mayank Rastogi                      Initial version.
 *         22-Dec-2004 Rohit Aggarwal                      Added error code
 *         06-Jan-2005 Aashish Suchdev                     Compiation changes
 *         30-Jan-2005 Jalaj Negi                          Added Error Strings
 *                                                         for RGM Module 
 *         24-Feb-2005 Navneet Gupta                       Added SCM Error Strings
 *         24-Jan-2008 Kamini Gangwani    Rel 8.0          Added error codes for SRTP Support 
 *                                                         and no direct routed call in IMS mode.
 *         3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 *         22-May-2009  Kamal Ashraf       SPR 19672       CSR_1_6763883 Merged
 *   
 *     Copyright 2009, Aricent.
 *
 *************************************************************************/

#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_macro.h"
#include "icf_ecode.h"


#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 *  FUNCTION NAME   :     icf_cmn_error_init
 *
 *  DESCRIPTION     :    This function initializes the error data in global data
 *                       This function is called by MRM during initialization.
 *
 *  RETURNS         :    ICF_SUCCESS
 *
*******************************************************************************/

icf_return_t icf_cmn_error_init(INOUT icf_glb_data_st *p_glb_data)
{

/* Error data for ecodes corresponding to error_id. 
   Note that index to this array is ecode id and not error id.
 */

  p_glb_data->error_data[ICF_ERROR_MEM_ALLOC_FAILURE].p_error_string = 
       (icf_uint8_t *)"Memory allocation failure";
  p_glb_data->error_data[ICF_ERROR_MEM_ALLOC_FAILURE].error_level = 
       ICF_ERROR_LEVEL_CRITICAL;
  p_glb_data->error_data[ICF_ERROR_MEM_ALLOC_FAILURE].error_type = 
       ICF_ERROR_TYPE_SYSTEM;

  p_glb_data->error_data[ICF_ERROR_MEM_FREE_FAILURE].p_error_string = 
       (icf_uint8_t *) "Memory Free failure";
  p_glb_data->error_data[ICF_ERROR_MEM_FREE_FAILURE].error_level = 
       ICF_ERROR_LEVEL_CRITICAL;
  p_glb_data->error_data[ICF_ERROR_MEM_FREE_FAILURE].error_type = 
       ICF_ERROR_TYPE_SYSTEM;
  /* When a new error id and ecode id is added in files icf_error_id.h and
   * icf_ecode.h then add the corresponding data here.  
   */

  p_glb_data->error_data[ICF_ERROR_CALL_FAIL_NO_SDP].p_error_string = 
       (icf_uint8_t *)"Call failed, peer SDP not received";
  p_glb_data->error_data[ICF_ERROR_CALL_FAIL_NO_SDP].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_ERROR_CALL_FAIL_NO_SDP].error_type = 
       ICF_ERROR_TYPE_CALL_CONTROL | ICF_ERROR_TYPE_SIP;

  p_glb_data->error_data[ICF_MSG_RECEIVE_ERROR].p_error_string = 
      (icf_uint8_t *) "IPC Receive Failure";
  p_glb_data->error_data[ICF_MSG_RECEIVE_ERROR].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_MSG_RECEIVE_ERROR].error_type = 
       ICF_ERROR_TYPE_SYSTEM | ICF_ERROR_TYPE_INTERFACE ;

   p_glb_data->error_data[ICF_INVALID_SRC_MODULE_ID].p_error_string = 
      (icf_uint8_t *) "Invalid source module";
  p_glb_data->error_data[ICF_INVALID_SRC_MODULE_ID].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_INVALID_SRC_MODULE_ID].error_type = 
       ICF_ERROR_TYPE_INTERFACE ;


    
    /* -------------------------------------------------------------------------
     * Error ids used by Call Manager for non-failure scenarios.
     * -----------------------------------------------------------------------*/
    
    p_glb_data->error_data[ICF_MSG_PROC_FIELD_TYPE_MISMATCH].p_error_string =
     (icf_uint8_t *)"Invalid field type used in node matching : ";
    p_glb_data->error_data[ICF_MSG_PROC_FIELD_TYPE_MISMATCH].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_MSG_PROC_FIELD_TYPE_MISMATCH].error_type = 
        ICF_ERROR_TYPE_INTERFACE ;

    p_glb_data->error_data[ICF_MSG_PROC_OPR_MISMATCH].p_error_string = 
     (icf_uint8_t *)"Invalid operator used for comparison : ";
    p_glb_data->error_data[ICF_MSG_PROC_OPR_MISMATCH].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_MSG_PROC_OPR_MISMATCH].error_type = 
        ICF_ERROR_TYPE_INTERFACE ;

    p_glb_data->error_data[ICF_MSG_PROC_INIT_NOT_COMPLETE].p_error_string = 
     (icf_uint8_t *)"Init Complete yet not occurred, message processing aborted";
    p_glb_data->error_data[ICF_MSG_PROC_INIT_NOT_COMPLETE].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_MSG_PROC_INIT_NOT_COMPLETE].error_type = 
        ICF_ERROR_TYPE_INTERFACE ;

 
/* -------------------------------------------------------------------------
 * Errors for REGM module
 * ------------------------------------------------------------------------*/
  p_glb_data->error_data[ICF_INIT_COMPLETE_FAILED].p_error_string = 
    (icf_uint8_t *)"[Regstration Processing at Init Complete FAILED !]";
  p_glb_data->error_data[ICF_INIT_COMPLETE_FAILED].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_INIT_COMPLETE_FAILED].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;

  p_glb_data->error_data[ICF_MODIFY_USER_FAILED].p_error_string = 
    (icf_uint8_t *)"[Modification of User Address API FAILED !]";
  p_glb_data->error_data[ICF_MODIFY_USER_FAILED].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_MODIFY_USER_FAILED].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;

  p_glb_data->error_data[ICF_PROCESS_MODIFY_USER_BLK_FAILED].p_error_string
      = (icf_uint8_t *)"[Processing of Modify User Address Block FAILED !]";
  p_glb_data->error_data[ICF_PROCESS_MODIFY_USER_BLK_FAILED].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_PROCESS_MODIFY_USER_BLK_FAILED].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;
  
  p_glb_data->error_data[ICF_MODIFY_REGISTRAR_FAILED].p_error_string = 
    (icf_uint8_t *)"[Addition/Deletion of Registrar Address  API FAILED !]";
  p_glb_data->error_data[ICF_MODIFY_REGISTRAR_FAILED].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_MODIFY_REGISTRAR_FAILED].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;
  
  p_glb_data->error_data[ICF_PROCESS_MODIFY_REG_BLK_FAILED].p_error_string
      = (icf_uint8_t *)"[Processing of ADD/DELETE Registrar Address Block FAILED !]";
  p_glb_data->error_data[ICF_PROCESS_MODIFY_REG_BLK_FAILED].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_PROCESS_MODIFY_REG_BLK_FAILED].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;
 
  p_glb_data->error_data[ICF_MODIFY_DURATION_FAILED].p_error_string = 
    (icf_uint8_t *)"[Modification of Registration Duration API FAILED !]";
  p_glb_data->error_data[ICF_MODIFY_DURATION_FAILED].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_MODIFY_DURATION_FAILED].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;
 
  p_glb_data->error_data[ICF_PROCESS_MODIFY_DUR_BLK_FAILED].p_error_string
      = (icf_uint8_t *)"[Processing of Registrar Duration Modification Block FAILED !]";
  p_glb_data->error_data[ICF_PROCESS_MODIFY_DUR_BLK_FAILED].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_PROCESS_MODIFY_DUR_BLK_FAILED].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;

  p_glb_data->error_data[ICF_AUTHENTICATION_INFO_ABSENT].p_error_string
      = (icf_uint8_t *)"[Authentication Information is NOT Available !]";
  p_glb_data->error_data[ICF_AUTHENTICATION_INFO_ABSENT].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_AUTHENTICATION_INFO_ABSENT].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;

  p_glb_data->error_data[ICF_INVALID_NETWORK_RESPONSE].p_error_string
      = (icf_uint8_t *)"[Invalid Network Response Received from SSA !]";
  p_glb_data->error_data[ICF_INVALID_NETWORK_RESPONSE].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_INVALID_NETWORK_RESPONSE].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;
  
  p_glb_data->error_data[ICF_UNABLE_TO_SEND_REG_REQ].p_error_string
      = (icf_uint8_t *)"[Unable To Send Registration Request: SSA Returned FAILURE!]";
  p_glb_data->error_data[ICF_UNABLE_TO_SEND_REG_REQ].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_UNABLE_TO_SEND_REG_REQ].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ;  
 
  p_glb_data->error_data[ICF_UNABLE_TO_SEND_DEREG_REQ].p_error_string
      = (icf_uint8_t *)"[Unable To Send Deregistration Request: SSA Returned FAILURE!]";
  p_glb_data->error_data[ICF_UNABLE_TO_SEND_DEREG_REQ].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_UNABLE_TO_SEND_DEREG_REQ].error_type = 
       ICF_ERROR_TYPE_REGISTRATION ; 


    /* -------------------------------------------------------------------------
     * Error ids used by CFG for non-failure scenarios.
     * -----------------------------------------------------------------------*/
    
    p_glb_data->error_data[ICF_RESPONSE_SEND_FAILURE].p_error_string =
     (icf_uint8_t *)"Failed to send response message to external module";
    p_glb_data->error_data[ICF_RESPONSE_SEND_FAILURE].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_RESPONSE_SEND_FAILURE].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_CONFIG;

    p_glb_data->error_data[ICF_INVALID_MSG_ID_RECD].p_error_string =
     (icf_uint8_t *)"Invalid internal message ID received : ";
    p_glb_data->error_data[ICF_INVALID_MSG_ID_RECD].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_INVALID_MSG_ID_RECD].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_CONFIG;

    p_glb_data->error_data[ICF_INVALID_API_ID_RECD].p_error_string =
     (icf_uint8_t *)"Invalid external API ID received : ";
    p_glb_data->error_data[ICF_INVALID_API_ID_RECD].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_INVALID_API_ID_RECD].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_CONFIG;
    
    p_glb_data->error_data[ICF_ERROR_NW_INTERFACE].p_error_string =
     (icf_uint8_t *)"Network Interface failed to Initialize : ";
    p_glb_data->error_data[ICF_ERROR_NW_INTERFACE].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_NW_INTERFACE].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_CONFIG;
    
    p_glb_data->error_data[ICF_ERROR_RGM_INIT_CMPLT].p_error_string =
     (icf_uint8_t *)"Failure in RGM Init Complete : ";
    p_glb_data->error_data[ICF_ERROR_RGM_INIT_CMPLT].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_RGM_INIT_CMPLT].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_CONFIG;
    
    p_glb_data->error_data[ICF_ERROR_DBM_PORT_DATA].p_error_string =
     (icf_uint8_t *)"Failure in getting DBM port data : ";
    p_glb_data->error_data[ICF_ERROR_DBM_PORT_DATA].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_DBM_PORT_DATA].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_CONFIG;
    
    p_glb_data->error_data[ICF_ERROR_ES_INIT_CMPLT].p_error_string =
     (icf_uint8_t *)"Failure in ES Init Complete : ";
    p_glb_data->error_data[ICF_ERROR_ES_INIT_CMPLT].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_ES_INIT_CMPLT].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_CONFIG;

/*-----------------------------------------------------------------------
 * Error Codes used in DBM
 * --------------------------------------------------------------------*/
    p_glb_data->error_data[ICF_LA_INIT_FAILURE].p_error_string = 
    (icf_uint8_t *)"Data Structure initialization failure";
  p_glb_data->error_data[ICF_LA_INIT_FAILURE].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_LA_INIT_FAILURE].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

   p_glb_data->error_data[ICF_NO_CALL_CTX_EXIST].p_error_string = 
    (icf_uint8_t *)"No call context block could be found";
  p_glb_data->error_data[ICF_NO_CALL_CTX_EXIST].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_NO_CALL_CTX_EXIST].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

  p_glb_data->error_data[ICF_NO_RGN_CTX_EXIST].p_error_string = 
    (icf_uint8_t *)"No registration context block could be found";
  p_glb_data->error_data[ICF_NO_RGN_CTX_EXIST].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_NO_RGN_CTX_EXIST].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

   p_glb_data->error_data[ICF_NO_SRVC_CTX_EXIST].p_error_string = 
    (icf_uint8_t *)"No service context block could be found";
  p_glb_data->error_data[ICF_NO_SRVC_CTX_EXIST].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_NO_SRVC_CTX_EXIST].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

  p_glb_data->error_data[ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD].p_error_string =
   (icf_uint8_t *)" No directed routed call in IMS mode allowed!";
   p_glb_data->error_data[ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD].error_level = 
       ICF_ERROR_LEVEL_INFO;
   p_glb_data->error_data[ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD].error_type = 
       ICF_ERROR_TYPE_GENERAL;

   p_glb_data->error_data[ICF_CAUSE_SRTP_NEG_FAILURE].p_error_string =
   (icf_uint8_t *)"SRTP negotiation failed";
   p_glb_data->error_data[ICF_CAUSE_SRTP_NEG_FAILURE].error_level = 
       ICF_ERROR_LEVEL_INFO;
   p_glb_data->error_data[ICF_CAUSE_SRTP_NEG_FAILURE].error_type = 
       ICF_ERROR_TYPE_GENERAL;


    p_glb_data->error_data[ICF_CALL_CTX_BLK_EXHAUSTED].p_error_string = 
    (icf_uint8_t *)"No free call context block available";
  p_glb_data->error_data[ICF_CALL_CTX_BLK_EXHAUSTED].error_level = 
       ICF_ERROR_LEVEL_MAJOR;
  p_glb_data->error_data[ICF_CALL_CTX_BLK_EXHAUSTED].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

   p_glb_data->error_data[ICF_SRVC_CTX_BLK_EXHAUSTED].p_error_string = 
    (icf_uint8_t *)"No free service context block available";
  p_glb_data->error_data[ICF_SRVC_CTX_BLK_EXHAUSTED].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_SRVC_CTX_BLK_EXHAUSTED].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

   p_glb_data->error_data[ICF_SRVC_CTX_LGC_BLK_EXHAUSTED].p_error_string = 
    (icf_uint8_t *)"No free service context logic block available";
  p_glb_data->error_data[ICF_SRVC_CTX_LGC_BLK_EXHAUSTED].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_SRVC_CTX_LGC_BLK_EXHAUSTED].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

  p_glb_data->error_data[ICF_RGN_CTX_BLK_EXHAUSTED].p_error_string = 
    (icf_uint8_t *)"No free registration context block available";
  p_glb_data->error_data[ICF_RGN_CTX_BLK_EXHAUSTED].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_RGN_CTX_BLK_EXHAUSTED].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

  p_glb_data->error_data[ICF_NO_MODULE_DATA_AVAILABLE].p_error_string = 
    (icf_uint8_t *)"No data block available for the module";
  p_glb_data->error_data[ICF_NO_MODULE_DATA_AVAILABLE].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_NO_MODULE_DATA_AVAILABLE].error_type = 
       ICF_ERROR_TYPE_SYSTEM ;

   p_glb_data->error_data[ICF_ECODE_NO_MMI_CONTEXT_FOUND].p_error_string = 
    (icf_uint8_t *)"No MMI context block found";
  p_glb_data->error_data[ICF_ECODE_NO_MMI_CONTEXT_FOUND].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_ECODE_NO_MMI_CONTEXT_FOUND].error_type = 
       ICF_ERROR_TYPE_GENERAL ;

  
  p_glb_data->error_data[ICF_ERROR_SERVICE_NOT_SUBSCRIBED].p_error_string
      = (icf_uint8_t *)"Service Request recieved for an UnSubscribed Service!";
  p_glb_data->error_data[ICF_ERROR_SERVICE_NOT_SUBSCRIBED].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_ERROR_SERVICE_NOT_SUBSCRIBED].error_type = 
       ICF_ERROR_TYPE_GENERAL; 

  p_glb_data->error_data[ICF_ERROR_SERVICE_NOT_ACTIVE].p_error_string
      = (icf_uint8_t *)"Service Request recieved for an Inactive Service!";
  p_glb_data->error_data[ICF_ERROR_SERVICE_NOT_ACTIVE].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_ERROR_SERVICE_NOT_ACTIVE].error_type = 
       ICF_ERROR_TYPE_GENERAL; 

  p_glb_data->error_data[ICF_ERROR_INVALID_SERVICE_INTERACTION].p_error_string
      = (icf_uint8_t *)"Service Request failed in Service Interaction Rules!";
  p_glb_data->error_data[ICF_ERROR_INVALID_SERVICE_INTERACTION].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_ERROR_INVALID_SERVICE_INTERACTION].error_type = 
       ICF_ERROR_TYPE_GENERAL; 

  p_glb_data->error_data[ICF_ERROR_SL_NOT_RUNNING].p_error_string
      = (icf_uint8_t *)"The handler Service for this API is not running.";
  p_glb_data->error_data[ICF_ERROR_SL_NOT_RUNNING].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_ERROR_SL_NOT_RUNNING].error_type = 
       ICF_ERROR_TYPE_GENERAL; 
  p_glb_data->error_data[ICF_ERROR_INV_PAYLOAD_RECVD].p_error_string
      = (icf_uint8_t *)"Invalid Payload Recieved in API or Return Path.";
  p_glb_data->error_data[ICF_ERROR_INV_PAYLOAD_RECVD].error_level = 
       ICF_ERROR_LEVEL_INFO;
  p_glb_data->error_data[ICF_ERROR_INV_PAYLOAD_RECVD].error_type = 
       ICF_ERROR_TYPE_GENERAL;

  /* -------------------------------------------------------------------------
     * Error ids used by SM Handler.
     * -----------------------------------------------------------------------*/
 p_glb_data->error_data[ICF_ERROR_INVALID_API_ID].p_error_string =
     (icf_uint8_t *)"Invalid external API ID received : ";
    p_glb_data->error_data[ICF_ERROR_INVALID_API_ID].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_ERROR_INVALID_API_ID].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_SM;

 p_glb_data->error_data[ICF_ERROR_INVALID_PARAM].p_error_string =
     (icf_uint8_t *)"Invalid Parameter received with the API : ";
    p_glb_data->error_data[ICF_ERROR_INVALID_PARAM].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_ERROR_INVALID_PARAM].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_SM;

    p_glb_data->error_data[ICF_ERROR_CALL_LOGGING_FAILURE].p_error_string =
     (icf_uint8_t *)"Call Logging will be disabled. ";
    p_glb_data->error_data[ICF_ERROR_CALL_LOGGING_FAILURE].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_ERROR_CALL_LOGGING_FAILURE].error_type = 
        ICF_ERROR_TYPE_INTERFACE | ICF_ERROR_TYPE_SM;
 



    /* License Related Errors */
    
/*    p_glb_data->error_data[ICF_ERROR_SYSCALL].p_error_string =
     (icf_uint8_t *)"stat()/time() System Call Failed. ";
    p_glb_data->error_data[ICF_ERROR_SYSCALL].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_SYSCALL].error_type = 
        ICF_ERROR_TYPE_SYSTEM | ICF_ERROR_TYPE_SM;*/
 
  /*  p_glb_data->error_data[ICF_ERROR_LIC_EXPIRED].p_error_string =
     (icf_uint8_t *)"ICF License Expired. ";
    p_glb_data->error_data[ICF_ERROR_LIC_EXPIRED].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_LIC_EXPIRED].error_type = 
        ICF_ERROR_TYPE_SYSTEM | ICF_ERROR_TYPE_SM;*/
/* 
    p_glb_data->error_data[ICF_ERROR_LIC_INTEGRITY].p_error_string =
     (icf_uint8_t *)"ICF License Integrity Violation Detected. ";
    p_glb_data->error_data[ICF_ERROR_LIC_INTEGRITY].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_LIC_INTEGRITY].error_type = 
        ICF_ERROR_TYPE_SYSTEM | ICF_ERROR_TYPE_SM;
        */
 

    p_glb_data->error_data[ICF_ERROR_TMR_START_FAILURE].p_error_string =
     (icf_uint8_t *)"Failed to start timer";
    p_glb_data->error_data[ICF_ERROR_TMR_START_FAILURE].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_TMR_START_FAILURE].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_TMR_STOP_FAILURE].p_error_string =
     (icf_uint8_t *)"Failed to stop timer";
    p_glb_data->error_data[ICF_ERROR_TMR_STOP_FAILURE].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_ERROR_TMR_STOP_FAILURE].error_type = 
        ICF_ERROR_TYPE_SYSTEM;
    
    p_glb_data->error_data[ICF_ERROR_MEM_INIT].p_error_string =
     (icf_uint8_t *)"Memory Initialization failure";
    p_glb_data->error_data[ICF_ERROR_MEM_INIT].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_MEM_INIT].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_MEM_DEINIT].p_error_string =
     (icf_uint8_t *)"Memory Deinitialization failure";
    p_glb_data->error_data[ICF_ERROR_MEM_DEINIT].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_ERROR_MEM_DEINIT].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_TMR_INIT].p_error_string =
     (icf_uint8_t *)"Timer Initialization failure";
    p_glb_data->error_data[ICF_ERROR_TMR_INIT].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_TMR_INIT].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_FIFO_CREATION].p_error_string =
     (icf_uint8_t *)"Error in FIFO creation";
    p_glb_data->error_data[ICF_ERROR_FIFO_CREATION].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_FIFO_CREATION].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_UDP_SEND].p_error_string =
     (icf_uint8_t *)"Sending data via UDP failed";
    p_glb_data->error_data[ICF_ERROR_UDP_SEND].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_UDP_SEND].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_TCP_SEND].p_error_string =
     (icf_uint8_t *)"Sending data via TCP failed";
    p_glb_data->error_data[ICF_ERROR_TCP_SEND].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_TCP_SEND].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_FIFO_SEND].p_error_string =
     (icf_uint8_t *)"Sending data via FIFO failed";
    p_glb_data->error_data[ICF_ERROR_FIFO_SEND].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_FIFO_SEND].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_SOCKET_CREATE].p_error_string =
     (icf_uint8_t *)"Socket creation failed";
    p_glb_data->error_data[ICF_ERROR_SOCKET_CREATE].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_SOCKET_CREATE].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_SOCKET_BIND].p_error_string =
     (icf_uint8_t *)"Socked bind failure";
    p_glb_data->error_data[ICF_ERROR_SOCKET_BIND].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_ERROR_SOCKET_BIND].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_SET_SOCKET].p_error_string =
     (icf_uint8_t *)"Error in setting up socket";
    p_glb_data->error_data[ICF_ERROR_SET_SOCKET].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_ERROR_SET_SOCKET].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_UDP_RECV].p_error_string =
     (icf_uint8_t *)"Receiving data via UDP failed";
    p_glb_data->error_data[ICF_ERROR_UDP_RECV].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_ERROR_UDP_RECV].error_type = 
        ICF_ERROR_TYPE_SYSTEM;
    
    p_glb_data->error_data[ICF_ERROR_FIFO_RECV].p_error_string =
     (icf_uint8_t *)"Receiving data via FIFO failed";
    p_glb_data->error_data[ICF_ERROR_FIFO_RECV].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_ERROR_FIFO_RECV].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND].p_error_string 
        = (icf_uint8_t *)"No MM_AL context found";
    p_glb_data->error_data[ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND].
        p_error_string = (icf_uint8_t *)"No free MM_AL context could be found";
    p_glb_data->error_data[ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND].error_level
       = ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND].error_type 
        = ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_CAUSE_MM_AL_MEMORY_FAILURE].p_error_string =
     (icf_uint8_t *)"Memory failure in MM_AL";
    p_glb_data->error_data[ICF_CAUSE_MM_AL_MEMORY_FAILURE].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_CAUSE_MM_AL_MEMORY_FAILURE].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_MM_AL_NO_VALID_CODEC_RETURNED].p_error_string 
        = (icf_uint8_t *)"No valid codecs returned by MM_AL";
    p_glb_data->error_data[ICF_MM_AL_NO_VALID_CODEC_RETURNED].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_MM_AL_NO_VALID_CODEC_RETURNED].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI].p_error_string
        = (icf_uint8_t *)"Unknown request received from MMI";
    p_glb_data->error_data[ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM].p_error_string 
        = (icf_uint8_t *)"Unknown message received from RM";
    p_glb_data->error_data[ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_USER_NOT_ADDED].p_error_string =
     (icf_uint8_t *)"User not present on line";
    p_glb_data->error_data[ICF_USER_NOT_ADDED].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_USER_NOT_ADDED].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_DUPLICATE_USER].p_error_string =
     (icf_uint8_t *)"Duplicate registration request";
    p_glb_data->error_data[ICF_DUPLICATE_USER].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_DUPLICATE_USER].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_TERMINAL_FAILURE_CALL_CLEAR].p_error_string =
     (icf_uint8_t *)"Terminal failure in SSA";
    p_glb_data->error_data[ICF_TERMINAL_FAILURE_CALL_CLEAR].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_TERMINAL_FAILURE_CALL_CLEAR].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_ERROR_NO_API_HANDLER].p_error_string =
     (icf_uint8_t *)"No Service Logic Handler for the recvd API";
    p_glb_data->error_data[ICF_ERROR_NO_API_HANDLER].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_ERROR_NO_API_HANDLER].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_ERROR_CALL_WAITING_FAILURE].p_error_string =
     (icf_uint8_t *)"Call waiting request could not be validated";
    p_glb_data->error_data[ICF_ERROR_CALL_WAITING_FAILURE].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_ERROR_CALL_WAITING_FAILURE].error_type = 
        ICF_ERROR_TYPE_INTERFACE;
    
    p_glb_data->error_data[ICF_ERROR_LIC_INTEGRTY].p_error_string =
     (icf_uint8_t *)"License intigrity failure";
    p_glb_data->error_data[ICF_ERROR_LIC_INTEGRTY].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_LIC_INTEGRTY].error_type = 
        ICF_ERROR_TYPE_SM|ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_INIT_CMPLT].p_error_string =
     (icf_uint8_t *)"All conditions to perform init complete are not met";
    p_glb_data->error_data[ICF_ERROR_INIT_CMPLT].error_level = 
        ICF_ERROR_LEVEL_MAJOR;
    p_glb_data->error_data[ICF_ERROR_INIT_CMPLT].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_UATK_FAILURE].p_error_string =
     (icf_uint8_t *)"UATK failure";
    p_glb_data->error_data[ICF_ERROR_UATK_FAILURE].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_ERROR_UATK_FAILURE].error_type = 
        ICF_ERROR_TYPE_INTERFACE;

    p_glb_data->error_data[ICF_CONNECT_RESP_PENDING].p_error_string =
     (icf_uint8_t *)"Connect Response Pending On Network";
    p_glb_data->error_data[ICF_CONNECT_RESP_PENDING].error_level = 
        ICF_ERROR_LEVEL_INFO;
    p_glb_data->error_data[ICF_CONNECT_RESP_PENDING].error_type = 
        ICF_ERROR_TYPE_INTERFACE;

   p_glb_data->error_data[ICF_ERROR_DNS_QUERY_FAILURE].p_error_string = 
    (icf_uint8_t *)"Cannot resolve Domain Name";
    p_glb_data->error_data[ICF_ERROR_DNS_QUERY_FAILURE].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_DNS_QUERY_FAILURE].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

     p_glb_data->error_data[ICF_ERROR_AUTHENTICATION_FAILURE].p_error_string = 
    (icf_uint8_t *)"Cannot authenticate";
    p_glb_data->error_data[ICF_ERROR_AUTHENTICATION_FAILURE].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_AUTHENTICATION_FAILURE].error_type = 
        ICF_ERROR_TYPE_SYSTEM;
    
    p_glb_data->error_data[ICF_ERROR_RETRANSMISSION_TIMEOUT].p_error_string = 
    (icf_uint8_t *)"Retransmissions Timed Out";
    p_glb_data->error_data[ICF_ERROR_RETRANSMISSION_TIMEOUT].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_RETRANSMISSION_TIMEOUT].error_type = 
        ICF_ERROR_TYPE_SYSTEM;

#ifndef ICF_SUPPRESS_INTERMEDIATE_ERROR_IND

    p_glb_data->error_data[ICF_ERROR_INTERMIDIATE_RETRANSMISSION_TIMEOUT].p_error_string =
    (icf_uint8_t *)"Intermidiate Retransmission Timed Out";
    p_glb_data->error_data[ICF_ERROR_INTERMIDIATE_RETRANSMISSION_TIMEOUT].error_level =
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_INTERMIDIATE_RETRANSMISSION_TIMEOUT].error_type =
        ICF_ERROR_TYPE_SYSTEM;
#endif

    /* CSR_1_6763883 Merged SPR 19672 Start */
    p_glb_data->error_data[ICF_ERROR_INCOMPLETE_MSG_FROM_NW].p_error_string = 
       (icf_uint8_t *)"Incomplete message from network";
    p_glb_data->error_data[ICF_ERROR_INCOMPLETE_MSG_FROM_NW].error_level = 
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_INCOMPLETE_MSG_FROM_NW].error_type = 
        ICF_ERROR_TYPE_SYSTEM;
   /* CSR_1_6763883 Merged SPR 19672 End */
    p_glb_data->error_data[ICF_ERROR_OPEN_SOCKET].p_error_string =
    (icf_uint8_t *)"Cannot open TCP socket";
    p_glb_data->error_data[ICF_ERROR_OPEN_SOCKET].error_level =
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_OPEN_SOCKET].error_type =
        ICF_ERROR_TYPE_SYSTEM;

    p_glb_data->error_data[ICF_ERROR_FAILURE_RESPONSE].p_error_string =
    (icf_uint8_t *)"Failure response received from Server";
    p_glb_data->error_data[ICF_ERROR_FAILURE_RESPONSE].error_level =
        ICF_ERROR_LEVEL_CRITICAL;
    p_glb_data->error_data[ICF_ERROR_FAILURE_RESPONSE].error_type =
        ICF_ERROR_TYPE_SYSTEM;

  return ICF_SUCCESS;
}
#endif /* #ifdef ICF_ERROR_ENABLE */
