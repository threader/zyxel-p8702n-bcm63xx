/*------------------------------------------------------------------------------
 *
 * File name        : icf_common_interface_defs.h
 *
 * Purpose          : This file contains all hash define values used by all 
 *                    internal modules of ICF on the interface messages
 *                    between the modules. 
 *
 * Revision History :
 *
 * Date              Name         Ref#            Comments
 * --------      ------------     ----            ---------------
 * 21-Dec-2004    R. Ezhirpavai   ICF LLD      Initial Version. 
 * 30-Dec-2004    A. Suchdev                      Removal of define for
 *                                                MAX CALL ID DIGITS
 * 31-Jan-2005    Jalaj Negi                      Added #defs for RGM - SSA
 *                                                Interface
 * 10-Jun-2005    Jalaj Negi      IMS CLIENT      Added defines for IMS CLIENT
 * 7-Dec-2005	  Jyoti Narula	  ICF 4.0		  New ecode between SSA and RGM   
 * 19-Jan-2007    Umang Singh              Removal of unsupported call hold options
 * 
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_COMMON_INTERFACE_DEFS_H__
#define __ICF_COMMON_INTERFACE_DEFS_H__

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

/* ----------------------------------------------------------------------
 * Hash Defines : icf_ssa_reason_t 
 * Description  : This contains the reason codes on the interface messages.
 * -----------------------------------------------------------------------*/

#define ICF_SA_REJECT                    1
#define ICF_SA_SERVER_INTERNAL_ERROR     2
#define ICF_REQUEST_PENDING              3
#define ICF_SA_PRECONDITION_FAILURE     4
#define ICF_SA_NETWORK_ERROR			5

/* ----------------------------------------------------------------------
 * Hash Defines : icf_mmi_reason_t 
 * Description  : This contains the reason codes on the interface messages.
 * -----------------------------------------------------------------------*/

/*Delete Media Session received from MM*/
#define  ICF_ECODE_DMS_IND_RECVD                   0X01
#define  ICF_ECODE_MMI_INITIATED_CALL_CLEAR		   0x02
    
/*These are the status values returned by DBM for add user and delete user 
 * registrations*/
#define ICF_RGM_DBM_NO_FREE_BLOCK       0
#define ICF_RGM_DBM_PAIR_PRESENT_ALONE  1
#define ICF_RGM_DBM_NEW_BLOCK           2
#define ICF_RGM_DBM_DURATION_CHANGE     3
#define ICF_RGM_DBM_DELETE_BLOCK        4
#define ICF_RGM_DBM_USER_NOT_FOUND      5
#define ICF_RGM_DBM_NO_ACTION           6   

/* Interface with RGM with SSA */
/* Response type(to be returned by SSA) */
#define ICF_RGM_SSA_NW_SUCCESS                 1    
#define ICF_RGM_SSA_NW_FAILURE                 0 

/* This is the ecode when failure response 
 * (302,400,500,502,503,491,stack timer expiry) is received in response to
 * registration request
 */
#define ICF_RGM_SSA_FAIL_REATTEMPT         1

/* This is the ecode when failure response
 * (403,404,481) in which registration should not be retried is received in
 * response to registration request.
 */
#define ICF_RGM_SSA_FAIL_NO_REATTEMPT     2

/* This is the ecode when authentication required(401/407) is received in
 * response to registration request
 */
#define ICF_RGM_SSA_AUTHENTICATION_REQ       3

/* This is the ecode when duration too brief(423) is received in response to
 * registration request 
 */  
#define ICF_RGM_SSA_DURATION_SHORT           4

ICF_4_0_CRM_START
/* This is used to indicate the timeout of transaction */
#define ICF_RGM_SSA_FAIL_TIMEOUT           5
ICF_4_0_CRM_END

#define ICF_REGM_SSA_BASIC_AUTH_SCHEME_NOT_SUPPORTED		6

#define ICF_REGM_SSA_AUTH_FAILURE			7

#define ICF_REGM_SSA_DNS_FAILURE               8


/*------------------------------------------------------------------------------
 *
 * Line status values returned by DBM's get_line_ctx_blk function
 *
 *----------------------------------------------------------------------------*/

#define ICF_STATUS_LINE_NEW                  1

#define ICF_STATUS_LINE_OLD                  2

/*------------------------------------------------------------------------------
 *
 * Error codes returned by DBM to CFG
 *
 *----------------------------------------------------------------------------*/

/* line id is more than the line id configured in the max_lines field of the
 * config_data_st structure */
#define ICF_STATUS_INVALID_LINE_ID            1

/* line is not configured till now */
#define ICF_STATUS_LINE_NOT_CONFIGURED        2

 

    /*
     * option using which the hold indication is sent to the remote party -
     * default value is ICF_CFG_CC_MUTE_OPTION_INVITE_MODE_INACTIVE (0x02)
     */
#define ICF_CFG_CC_MUTE_OPTION_INVITE_C_ZERO         ICF_CALL_HOLD_OPT_INV_WTH_C_0
#define ICF_CFG_CC_MUTE_OPTION_INVITE_MODE_INACTIVE  ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY
#define ICF_CFG_CC_MUTE_OPTION_UPDATE_C_ZERO         ICF_CALL_HOLD_OPT_UPDT_WTH_C_0
#define ICF_CFG_CC_MUTE_OPTION_UPDATE_MODE_INACTIVE  ICF_CALL_HOLD_OPT_UPDT_WTH_MODE_SEND_ONLY
#define ICF_CFG_CC_HOLD_OPTION_INV_WTH_MODE_SEND_ONLY_N_C_0  ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY_N_C_0 

/* UPDATE with mode=send only and c=0 */
#define ICF_CFG_CC_HOLD_OPTION_UPD_WTH_MODE_SEND_ONLY_N_C_0  ICF_CALL_HOLD_OPT_UPDT_WTH_MODE_SEND_ONLY_N_C_0
   


       /* bitmask that give value of default settings on line data basis */
#define ICF_CFG_CC_USER_PRESENTATION_IND       ICF_SETTINGS_CONFIG_PRESENTATION_IND   /* default is TRUE */

     
       /* bitmask that give value of default settings on config data basis */
#define ICF_CFG_CC_PRIVACY_HEADER_PRESENT ICF_CFG_CC_CONFIG_PRIVACY_HEADER_PRESENT /* default is TRUE */
#define ICF_CFG_CC_PRACK_REQD             ICF_CFG_CC_CONFIG_PRACK_REQD 
    /* default is FALSE */
#define ICF_CFG_CC_CALL_THRU_SIP_PROXY    ICF_CFG_CC_CONFIG_CALL_THRU_SIP_PROXY 

    
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_COMMON_INTERFACE_DEFS_H__ */

    
