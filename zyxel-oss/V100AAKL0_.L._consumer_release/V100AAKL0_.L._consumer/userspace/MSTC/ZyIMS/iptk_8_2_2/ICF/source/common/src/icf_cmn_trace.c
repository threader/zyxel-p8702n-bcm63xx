/*******************************************************************************
 *
 * FILENAME         : icf_cmn_trace.c
 *
 * DESCRIPTION      : This file defines the init of trace data.
 *
 * Revision History :
 *
 * DATE         NAME                REFERENCE       REASON
 * -----------  -------------       ------------    --------------
 * Dec-2004     Mayank Rastogi      ICF LLD      Initial version.
 * 15-Dec-2004  Rohit Aggarwal                      Added trace IDs for CC
 * 21-Dec-2004  Rohit Aggarwal                      Removed [CC] from trace 
 *                                                  strings so that they can be 
 *                                                  used by all modules; added 
 *                                                  traces for MMI requests
 * 22-Dec-2004  Rohit Aggarwal                      Added trace IDs
 * 06-Jan-2005  Aasnish Suchdev                     Updated for implementation
 * 27-Jan-2005  Navneet Gupta                       Added SCM Trace Strings
 * 30-Jan-2005  Jalaj Negi                          Added RGM Trace Strings
 * 08-Feb-2005  Amit Sharma                         Added CHR n TWC 
 *                                                  Trace Strings
 * 24-Feb-2005  Navneet Gupta                       Updated SCM Trace Strings
 * 17-Mar-2005  Jyoti Narula                        Added SM Trace Strings.
 * 04-Mar-2005  Amit Sharma                         Added RNW trace
 * 10-Mar-2006  Amit Sharma                         ICF merging with ICF2.0
 * 01-Nov-2007  Neha Choraria                       Added Interface traces
 * 17-Jan-2008  Shruti Thakral                      Enhancements for Rel 8.0
 * 24-Jan-2008  Kamini Gangwani    Rel 8.0          Added traces for SRTP Support.
 * 27-Jan-2008  Rajiv Kumar        Rel 8.0          Registration Procedure
 *                                                  Enhancements 
 * 15-Feb-2008  Tarun Gupta        Rel 8.0 LLD      Registration Procedures
 *                                                  Enhancements                
 * 28-Feb-2008  Abhishek Dhammawat Rel8.0           STATS for 
 *                                                  precondition
 * 11-Mar-2008  Vipul Varshney      SPR 18122       AKA Enhancement
 * 19-Mar-2008  Sumant Gupta       Rel 8.0          Merging for Publish & Message
 * 7-Apr-2008   Sumant Gupta       ICF 8.0          Rel7.1 Windows porting Merging
 * 16-Apr-2008  Kamini Gangwani    SPR 18438        Added trace for Secondary
 *                                                  Registrar Transport Address
 * 07-Jun-2008  Jagmohan Chauhan   IPTK 8.0 SRTP LLD      SRTP Enhancements
 * 04-Nov-2008  Tarun Gupta        SPR 19189        Merged Rel 7.1.2 SPR 18999
 * 21-Nov-2008  Rajiv Kumar        SPR 19003        Rel 7.1.2 SPR merging (SPR
 *                                                  19003)   
 * 01-Dec-2008  Abhishek Dhammawat SPR 19218        Merge CSR 1-6214356 
 * 01-Dec-2008 Rajiv Kumar         SPR 19215        CSR-1-6212448 Merged
 * 12-Dec-2008 Rajiv Kumar         SPR 19218        Merged CSR 1-6240518
 * 10-Feb-2009 Anurag Khare        ICF Rel 8.1      Traces initialization
 *                                                  for OPTIONS Enhancements
 * 10-Feb-2009 Tarun Gupta         Rel 8.1 LLD      SDP Tunnelling Enhancements
 * 02-Mar-2009 Alok Tiwari         Rel 8.1          Open source Stun enc dec 
 *                                                  replaced with arStunEncDec  
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349        Klocworks fix
 * 22-Mar-2009 Anuradha Gupta      Rel 8.1          UT defect fix
 * 27-Mar-2009 Anuradha Gupta      Rel 8.2          Register Enh code 
 * 31-Mar-2009 Anurag Khare        Rel 8.2          Changes done for PRACK Enh
 * 31-Mar-2009 Alok Tiwari         Rel 8.2          Modified Functions:
 *                                                 icf_cmn_trace_register_req
 *                                                 icf_cmn_trace_aka_auth_resp
 * 03-Apr-2009 Tarun Gupta         Rel 8.2          REFER/NOTIFY Enhancements
 * 06-Apr-2009 Kamal Ashraf        Rel 8.2         Klocwork Fix
 * 22-Apr-2009 Abhishek Dhammawat SPR 19473        modified
 *                                                 icf_cmn_trace_api_string
 * 2-May-2009 Abhishek Dhammawat SPR 19571          modified
 *                                                     icf_cmn_trace_route_info
 * 21-May-2009 Anuradha Gupta   SPR 19672          CSR 1-6580995 Merge 
 * 26-May-2009 Anuradha Gupta   SPR 19672          CSR 1-6667668 Merge 
 * 26-May-2009 Anuradha Gupta   SPR 19672          CSR 1-6727350 Merge
 * 28-May-2009 Tarun Gupta      Rel 8.2            Async NOTIFY Support
 * 28-May-2009 Ashutosh Mohan   SPR 19672          CSR-1-6976670 Merged
 * 31-May-2009 Preksha             Rel 8.2          Asynchronus Message enhancements
 * 02-Jun-2009 Kamal Ashraf     SPR 19672          CSR_1_7113139 Merged
 * 11-Jun-2009 Ashutosh Mohan   SPR 19737          Changes done for GCC 4.1 warnings removal
 * 14-Jun-2009 Abhishek Dhammawat SPR 19590        Modified
 *                                                 icf_cmn_trace_header_list
 * 18-Jun-2009 Anuradha Gupta   SPR 19752          Modified icf_cmn_trace_set_call_params 
 * 14-Jul-2009 Abhishek Dhammawat SPR 19785        Modified
 *                                                 icf_cmn_trace_pub_uri_list
 * 03-Aug-2009 Tarun Gupta      SPR 19886          Merged CSR 1-7193955
 * 03-Aug-2009 Tarun Gupta      SPR 19886          Merged CSR 1-7334184
 * 20-Aug-2009 Anuradha Gupta   SPR 19927          GCC Warning fix
 * 05-Nov-2009 Anuradha Gupta   SPR 20208          Modified icf_cmn_trace_connect_ind
 * 25-May-2010 Preksha Gupta    SPR 20412          Merged CSR 1-8389294
 * 27-May-2010 Preksha          SPR 20412          Merged CSR 1-8420460
 * 13-Jul-2010 Udit Gattani     SPR 20517          Added new parameter in 
 *    
 * 11-Oct-2010 Sachin Sharma    SPR-20697          CSR 1-9012191 Fixed.                                             icf_cmn_trace_alert_user
 *
 * Copyright (c) 2010 Aricent.
 ******************************************************************************/

#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_macro.h"
#include "icf_sm_int_types.h"
#include "icf_sm_prototypes.h"
#ifdef ICF_NAT_MEDIA
#include "arStunEncDec_api.h"
#endif
 
/*******************************************************************************
 *
 *  FUNCTION NAME   :     icf_cmn_trace_init
 *
 *  DESCRIPTION     :    This function initializes the trace data in global data
 *                       This function is called by MRM during initialization.
 *
 *  RETURNS         :    ICF_SUCCESS
 *
*******************************************************************************/
icf_return_t icf_cmn_trace_init(INOUT icf_glb_data_st *p_glb_data)
{
    if(ICF_NULL == p_glb_data)
    {
        return ICF_FAILURE;
    }
#ifdef ICF_TRACE_ENABLE
  p_glb_data->trace_data[ICF_TRACE_MODULE_ENTRY].p_trace_string = 
       (icf_uint8_t *)"Entered module:()";
  p_glb_data->trace_data[ICF_TRACE_MODULE_ENTRY].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_MODULE_ENTRY].trace_type = 
       ICF_TRACE_TYPE_GENERAL;

  p_glb_data->trace_data[ICF_TRACE_MODULE_EXIT].p_trace_string = 
       (icf_uint8_t *)"Exit from module:";
  p_glb_data->trace_data[ICF_TRACE_MODULE_EXIT].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_MODULE_EXIT].trace_type = 
       ICF_TRACE_TYPE_GENERAL;
  
  p_glb_data->trace_data[ICF_TRACE_FUNCTION_ENTRY].p_trace_string = 
       (icf_uint8_t *)"Entered function():";
  p_glb_data->trace_data[ICF_TRACE_FUNCTION_ENTRY].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_FUNCTION_ENTRY].trace_type = 
       ICF_TRACE_TYPE_GENERAL;

  p_glb_data->trace_data[ICF_TRACE_FUNCTION_EXIT].p_trace_string =
       (icf_uint8_t *)"Exit from function():";
  p_glb_data->trace_data[ICF_TRACE_FUNCTION_EXIT].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_FUNCTION_EXIT].trace_type =
       ICF_TRACE_TYPE_GENERAL;

  p_glb_data->trace_data[ICF_TRACE_SETUP_RECVD_FRM_PA].p_trace_string = 
       (icf_uint8_t *)"Setup received from PA";
  p_glb_data->trace_data[ICF_TRACE_SETUP_RECVD_FRM_PA].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_SETUP_RECVD_FRM_PA].trace_type = 
       ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_CONTROL;


  p_glb_data->trace_data[ICF_CALL_CTX_ALLOCATE].p_trace_string = 
       (icf_uint8_t *)"Call Context allocated Succesfully";
  p_glb_data->trace_data[ICF_CALL_CTX_ALLOCATE].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_CALL_CTX_ALLOCATE].trace_type = 
       ICF_TRACE_TYPE_GENERAL | ICF_TRACE_TYPE_CALL_CONTROL;

  p_glb_data->trace_data[ICF_DBM_BLOCK_ALLOCATED].p_trace_string = 
       (icf_uint8_t *)"Bloacl Allocation Succesfully Done";
  p_glb_data->trace_data[ICF_DBM_BLOCK_ALLOCATED].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_DBM_BLOCK_ALLOCATED].trace_type = 
       ICF_TRACE_TYPE_GENERAL | ICF_TRACE_TYPE_CALL_CONTROL;

  p_glb_data->trace_data[ICF_DBM_BLOCK_DEALLOCATED].p_trace_string = 
       (icf_uint8_t *)"Bloacl Deallocation Succesfully Done";
  p_glb_data->trace_data[ICF_DBM_BLOCK_DEALLOCATED].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_DBM_BLOCK_DEALLOCATED].trace_type = 
       ICF_TRACE_TYPE_GENERAL | ICF_TRACE_TYPE_CALL_CONTROL;

  p_glb_data->trace_data[ICF_CALL_CTX_FOUND].p_trace_string = 
       (icf_uint8_t *)"Call Context Block Found";
  p_glb_data->trace_data[ICF_CALL_CTX_FOUND].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_CALL_CTX_FOUND].trace_type = 
       ICF_TRACE_TYPE_GENERAL | ICF_TRACE_TYPE_CALL_CONTROL;

  p_glb_data->trace_data[ICF_CALL_CTX_FREED].p_trace_string = 
       (icf_uint8_t *)"Call Context Block Freed";
  p_glb_data->trace_data[ICF_CALL_CTX_FREED].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_CALL_CTX_FREED].trace_type = 
       ICF_TRACE_TYPE_GENERAL | ICF_TRACE_TYPE_CALL_CONTROL;

  /* more trace data will be added here */
  
    /* -------------------------------------------------------------------------
     * Trace ids used by Call Control for non-failure scenarios.
     * -----------------------------------------------------------------------*/

    p_glb_data->trace_data[ICF_TRACE_FSM_TRIGGERED].p_trace_string = 
        (icf_uint8_t *)"FSM triggered : ";
    p_glb_data->trace_data[ICF_TRACE_FSM_TRIGGERED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FSM_TRIGGERED].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_PA_CLR].p_trace_string = 
        (icf_uint8_t *)"PA interface cleared";
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_PA_CLR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_PA_CLR].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_MM_CLR].p_trace_string = 
        (icf_uint8_t *)"MM interface cleared";
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_MM_CLR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_MM_CLR].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_NW_CLR].p_trace_string = 
        (icf_uint8_t *)"NW interface cleared";
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_NW_CLR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_NW_CLR].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_SIP;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_ALL_CLR].p_trace_string = 
        (icf_uint8_t *)"PA, MM and NW interfaces cleared";
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_ALL_CLR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_STATUS_ALL_CLR].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA | ICF_TRACE_TYPE_SIP;
  
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_PA_INIT].p_trace_string = 
        (icf_uint8_t *)"Call termination triggered by PA";
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_PA_INIT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_PA_INIT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_MM_INIT].p_trace_string = 
        (icf_uint8_t *)"Call termination triggered by MM";
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_MM_INIT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_MM_INIT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_NW_INIT].p_trace_string = 
        (icf_uint8_t *)"Call termination triggered by Peer";
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_NW_INIT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_NW_INIT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_SIP;
  
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_TIMER_INIT].p_trace_string = 
        (icf_uint8_t *)"Call termination triggered by Timer expiry";
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_TIMER_INIT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_TIMER_INIT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;
  
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_INT_ERR].p_trace_string = 
        (icf_uint8_t *)"Call termination triggered by Internal error";
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_INT_ERR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REL_TYPE_INT_ERR].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_MUTE_SET].p_trace_string = 
        (icf_uint8_t *)"Media state set to Call Mute";
    p_glb_data->trace_data[ICF_TRACE_CALL_MUTE_SET].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_MUTE_SET].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_MUTE_RESET].p_trace_string = 
        (icf_uint8_t *)"Media state reset from Call Mute to Call Unmute";
    p_glb_data->trace_data[ICF_TRACE_CALL_MUTE_RESET].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_MUTE_RESET].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_DIR].p_trace_string = 
        (icf_uint8_t *)"Call direction : ";
    p_glb_data->trace_data[ICF_TRACE_CALL_DIR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_DIR].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_GENERAL;
  
    p_glb_data->trace_data[ICF_TRACE_MEDIA_SETUP_TYPE].p_trace_string = 
        (icf_uint8_t *)"Incoming call media setup type : ";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_SETUP_TYPE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_SETUP_TYPE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_GENERAL | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_MEDIA_COMMITTED].p_trace_string = 
        (icf_uint8_t *)"Media committed status : ";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_COMMITTED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_COMMITTED].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_MEDIA_MODIFY_SENT].p_trace_string = 
        (icf_uint8_t *)"Modify Media Sent: ";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_MODIFY_SENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_MODIFY_SENT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_MEDIA;

  
    p_glb_data->trace_data[ICF_TRACE_SDP_IN_PEER_MSG].p_trace_string = 
        (icf_uint8_t *)"SDP received in message from peer : ";
    p_glb_data->trace_data[ICF_TRACE_SDP_IN_PEER_MSG].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SDP_IN_PEER_MSG].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SIP;
  
    p_glb_data->trace_data[ICF_TRACE_PREVIOUS_STATE].p_trace_string = 
        (icf_uint8_t *)"Previous Call State (currently set): ";
    p_glb_data->trace_data[ICF_TRACE_PREVIOUS_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PREVIOUS_STATE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_SET_PREVIOUS_STATE].p_trace_string = 
        (icf_uint8_t *)"Previous Call State (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_PREVIOUS_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_PREVIOUS_STATE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_CURRENT_STATE].p_trace_string = 
        (icf_uint8_t *)"Present Call State (currently set) : ";
    p_glb_data->trace_data[ICF_TRACE_CURRENT_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CURRENT_STATE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_SET_CURRENT_STATE].p_trace_string = 
        (icf_uint8_t *)"Present Call State (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_CURRENT_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_CURRENT_STATE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_NEXT_STATE].p_trace_string = 
        (icf_uint8_t *)"Next Call State (currently set) : ";
    p_glb_data->trace_data[ICF_TRACE_NEXT_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NEXT_STATE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_SET_NEXT_STATE].p_trace_string = 
        (icf_uint8_t *)"Next Call State (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_NEXT_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_NEXT_STATE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_MEDIA_MODE].p_trace_string = 
        (icf_uint8_t *)"Media mode applied to MM (currently set) : ";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_MODE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_MODE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_SET_MEDIA_MODE].p_trace_string = 
        (icf_uint8_t *)"Media mode applied to MM (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_MEDIA_MODE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_MEDIA_MODE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_SET_MODE_TO_BE_APP].p_trace_string = 
        (icf_uint8_t *)"Media mode to be applied to MM (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_MODE_TO_BE_APP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_MODE_TO_BE_APP].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_SET_RELEASE_CAUSE].p_trace_string = 
        (icf_uint8_t *)"Call Release Cause : ";
    p_glb_data->trace_data[ICF_TRACE_SET_RELEASE_CAUSE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_RELEASE_CAUSE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_MEDIA_ONGOING].p_trace_string = 
        (icf_uint8_t *)"Media Ongoing status (currently set) : ";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_ONGOING].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_ONGOING].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_SET_MEDIA_ONGOING].p_trace_string = 
        (icf_uint8_t *)"Media Ongoing status (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_MEDIA_ONGOING].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_MEDIA_ONGOING].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_SIG_ONGOING].p_trace_string = 
        (icf_uint8_t *)"Signaling Ongoing status (currently set) : ";
    p_glb_data->trace_data[ICF_TRACE_SIG_ONGOING].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SIG_ONGOING].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_SET_SIG_ONGOING].p_trace_string = 
        (icf_uint8_t *)"Signaling Ongoing status (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_SIG_ONGOING].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_SIG_ONGOING].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_ONGOING_EVENT].p_trace_string = 
        (icf_uint8_t *)"Ongoing event (currently set) : ";
    p_glb_data->trace_data[ICF_TRACE_ONGOING_EVENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ONGOING_EVENT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_SET_ONGOING_EVENT].p_trace_string = 
        (icf_uint8_t *)"Ongoing event (changed to) : ";
    p_glb_data->trace_data[ICF_TRACE_SET_ONGOING_EVENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_ONGOING_EVENT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL;
  
    p_glb_data->trace_data[ICF_TRACE_INBAND_INFO].p_trace_string = 
        (icf_uint8_t *)"Inband information sent/received : ";
    p_glb_data->trace_data[ICF_TRACE_INBAND_INFO].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_INBAND_INFO].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SIP;

    p_glb_data->trace_data[ICF_TRACE_PRACK_REQD].p_trace_string = 
        (icf_uint8_t *)"Provisional response expected : ";
    p_glb_data->trace_data[ICF_TRACE_PRACK_REQD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PRACK_REQD].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SIP;

    p_glb_data->trace_data[ICF_TRACE_MEDIA_TO_BE_SENT].p_trace_string = 
        (icf_uint8_t *)"SDP to be sent in outgoing message : ";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_TO_BE_SENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_TO_BE_SENT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SIP;

    p_glb_data->trace_data[ICF_TRACE_EVENT_PA_CONNECT].p_trace_string = 
        (icf_uint8_t *)"User Answered from PA as pending event : ";
    p_glb_data->trace_data[ICF_TRACE_EVENT_PA_CONNECT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EVENT_PA_CONNECT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_TRACE_EVENT_PA_TERM_REQ].p_trace_string = 
        (icf_uint8_t *)"Call Terminate Request from PA as pending event : ";
    p_glb_data->trace_data[ICF_TRACE_EVENT_PA_TERM_REQ].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EVENT_PA_TERM_REQ].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_TRACE_EVENT_MM_ERR_IND].p_trace_string = 
        (icf_uint8_t *)"MM Error Indication as pending event : ";
    p_glb_data->trace_data[ICF_TRACE_EVENT_MM_ERR_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EVENT_MM_ERR_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_EVENT_OFFER_CHANGE].p_trace_string = 
        (icf_uint8_t *)"Offer Change Request from peer as pending event : ";
    p_glb_data->trace_data[ICF_TRACE_EVENT_OFFER_CHANGE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EVENT_OFFER_CHANGE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_SIP;
  
    p_glb_data->trace_data[ICF_TRACE_EVENT_INT_ERR].p_trace_string = 
        (icf_uint8_t *)"Internal Error Indication as pending event : ";
    p_glb_data->trace_data[ICF_TRACE_EVENT_INT_ERR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EVENT_INT_ERR].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;
  
    p_glb_data->trace_data[ICF_TRACE_EVENT_CALL_MUTE].p_trace_string = 
        (icf_uint8_t *)"Call Mute as pending event : ";
    p_glb_data->trace_data[ICF_TRACE_EVENT_CALL_MUTE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EVENT_CALL_MUTE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_EVENT_CALL_UNMUTE].p_trace_string = 
        (icf_uint8_t *)"Call Unmute as pending event : ";
    p_glb_data->trace_data[ICF_TRACE_EVENT_CALL_UNMUTE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EVENT_CALL_UNMUTE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_TIMER_EXPIRED].p_trace_string = 
        (icf_uint8_t *)"Timer expired : ";
    p_glb_data->trace_data[ICF_TRACE_TIMER_EXPIRED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TIMER_EXPIRED].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;
  
    p_glb_data->trace_data[ICF_TRACE_CALL_FAIL_NO_SDP].p_trace_string = 
        (icf_uint8_t *)"Call failure, peer SDP not received";
    p_glb_data->trace_data[ICF_TRACE_CALL_FAIL_NO_SDP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_FAIL_NO_SDP].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SIP;
  
    p_glb_data->trace_data[ICF_TRACE_START_TIMER].p_trace_string = 
        (icf_uint8_t *)"Timer started : ";
    p_glb_data->trace_data[ICF_TRACE_START_TIMER].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_START_TIMER].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;
  
    p_glb_data->trace_data[ICF_TRACE_STOP_TIMER].p_trace_string = 
        (icf_uint8_t *)"Timer stopped : ";
    p_glb_data->trace_data[ICF_TRACE_STOP_TIMER].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_STOP_TIMER].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_INTERFACE_FAILURE].p_trace_string = 
        (icf_uint8_t *)"Interface function returned failure : ";
    p_glb_data->trace_data[ICF_INTERFACE_FAILURE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_INTERFACE_FAILURE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_INCOMING_CALL_IND].p_trace_string = 
        (icf_uint8_t *)" Sending Incoming call indication to PA : ";
    p_glb_data->trace_data[ICF_PA_INCOMING_CALL_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_INCOMING_CALL_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_ALERT_USER_IND].p_trace_string = 
        (icf_uint8_t *)"Sending Alert inditcation to PA : ";
    p_glb_data->trace_data[ICF_PA_ALERT_USER_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_ALERT_USER_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_CONNECT_IND].p_trace_string = 
        (icf_uint8_t *)"Sending Connect to PA : ";
    p_glb_data->trace_data[ICF_PA_CONNECT_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_CONNECT_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_CALL_TERM_IND].p_trace_string = 
        (icf_uint8_t *)"Sending Call terminate Indication to PA : ";
    p_glb_data->trace_data[ICF_PA_CALL_TERM_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_CALL_TERM_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_TERMINATE_CALL_RESP].p_trace_string = 
        (icf_uint8_t *)"Sending Call terminate indication to PA : ";
    p_glb_data->trace_data[ICF_PA_TERMINATE_CALL_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_TERMINATE_CALL_RESP].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_CALL_REDIRECTION_IND].p_trace_string = 
        (icf_uint8_t *)"Sending Call redirection indication to PA : ";
    p_glb_data->trace_data[ICF_PA_CALL_REDIRECTION_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_CALL_REDIRECTION_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_CALL_HOLD_IND].p_trace_string = 
        (icf_uint8_t *)"Sending Call hold by Remote indication to PA ";
    p_glb_data->trace_data[ICF_PA_CALL_HOLD_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_CALL_HOLD_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_CALL_RESUME_IND].p_trace_string = 
        (icf_uint8_t *)"Sending Call resume by Remote indication to PA : ";
    p_glb_data->trace_data[ICF_PA_CALL_RESUME_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_CALL_RESUME_IND].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_PA_MEDIA_CONNECT].p_trace_string = 
        (icf_uint8_t *)"Sending media connect to PA ";
    p_glb_data->trace_data[ICF_PA_MEDIA_CONNECT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_PA_MEDIA_CONNECT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

    p_glb_data->trace_data[ICF_DIVERSION_INFO_VALIDATION_FAILED].
             p_trace_string = (icf_uint8_t *)"Validation for Diversion info header failed";
    p_glb_data->trace_data[ICF_DIVERSION_INFO_VALIDATION_FAILED].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_DIVERSION_INFO_VALIDATION_FAILED].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL;

    p_glb_data->trace_data[ICF_UNEXPECTED_EVENT].p_trace_string = 
        (icf_uint8_t *)"UNEXPECTED EVENT ";
    p_glb_data->trace_data[ICF_UNEXPECTED_EVENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_UNEXPECTED_EVENT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;


    p_glb_data->trace_data[ICF_TRANSPORT_ADDRESS].p_trace_string = 
        (icf_uint8_t *)"Address data is ";
    p_glb_data->trace_data[ICF_TRANSPORT_ADDRESS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRANSPORT_ADDRESS].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_SYSTEM;

  
    p_glb_data->trace_data[ICF_TRACE_SEND_MMI_REQ].p_trace_string = 
        (icf_uint8_t *)"Make and send request to MMI : ";
    p_glb_data->trace_data[ICF_TRACE_SEND_MMI_REQ].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SEND_MMI_REQ].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_DMS_REQ_PAYLOAD].p_trace_string = 
        (icf_uint8_t *)"DMS Request payload : ";
    p_glb_data->trace_data[ICF_TRACE_DMS_REQ_PAYLOAD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_DMS_REQ_PAYLOAD].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MMS_REQ_PAYLOAD].p_trace_string = 
        (icf_uint8_t *)"MMS Request payload : ";
    p_glb_data->trace_data[ICF_TRACE_MMS_REQ_PAYLOAD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_REQ_PAYLOAD].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;

	p_glb_data->trace_data[ICF_TRACE_CMS_REQ_SENT].p_trace_string = 
        (icf_uint8_t *)"CMS Req Payload : ";
    p_glb_data->trace_data[ICF_TRACE_CMS_REQ_SENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CMS_REQ_SENT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;

	p_glb_data->trace_data[ICF_TRACE_CMS_RESP_RECVD].p_trace_string = 
        (icf_uint8_t *)"CMS Response Received : ";
    p_glb_data->trace_data[ICF_TRACE_CMS_RESP_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CMS_RESP_RECVD].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;

	p_glb_data->trace_data[ICF_TRACE_ITH_LEVEL_VALIDATION_FAILED].p_trace_string = 
        (icf_uint8_t *)"ith level mismatch : ";
    p_glb_data->trace_data[ICF_TRACE_ITH_LEVEL_VALIDATION_FAILED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ITH_LEVEL_VALIDATION_FAILED].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;

	p_glb_data->trace_data[ICF_TRACE_MMS_RESP_RECVD].p_trace_string = 
        (icf_uint8_t *)"MMS Response Received : ";
    p_glb_data->trace_data[ICF_TRACE_MMS_RESP_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_RESP_RECVD].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;

	p_glb_data->trace_data[ICF_TRACE_APP_REJECTED_STREAMS].p_trace_string = 
        (icf_uint8_t *)"Streams Rejected by PA : ";
    p_glb_data->trace_data[ICF_TRACE_APP_REJECTED_STREAMS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_APP_REJECTED_STREAMS].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;

	p_glb_data->trace_data[ICF_TRACE_NO_ACTIVE_STREAMS_LEFT].p_trace_string = 
        (icf_uint8_t *)"No Active Stream Left : ";
    p_glb_data->trace_data[ICF_TRACE_NO_ACTIVE_STREAMS_LEFT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NO_ACTIVE_STREAMS_LEFT].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;

	p_glb_data->trace_data[ICF_TRACE_OFFER_CHANGE_FAILURE].p_trace_string = 
        (icf_uint8_t *)"Offer Change Failure : ";
    p_glb_data->trace_data[ICF_TRACE_OFFER_CHANGE_FAILURE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_OFFER_CHANGE_FAILURE].trace_type = 
        ICF_TRACE_TYPE_CALL_CONTROL | ICF_TRACE_TYPE_INTERFACE | 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_GET_GLB_DATA_FAILED].p_trace_string=
        (icf_uint8_t *)" Failure in getting Global Data.";
    p_glb_data->trace_data[ICF_TRACE_GET_GLB_DATA_FAILED].trace_level= 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_GET_GLB_DATA_FAILED].trace_type=
        ICF_TRACE_TYPE_GENERAL;
    
    p_glb_data->trace_data[ICF_TRACE_API_RECVD].p_trace_string=
        (icf_uint8_t *)"Received Api : ";
    p_glb_data->trace_data[ICF_TRACE_API_RECVD].trace_level=
        ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_API_RECVD].trace_type=
        ICF_TRACE_TYPE_GENERAL;
    
    p_glb_data->trace_data[ICF_TRACE_SVC_CONT_RECVD].p_trace_string=
        (icf_uint8_t *)"ICF_STATUS_SERVICE_CONTINUE status received. ";
    p_glb_data->trace_data[ICF_TRACE_SVC_CONT_RECVD].trace_level= 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SVC_CONT_RECVD].trace_type=
        ICF_TRACE_TYPE_GENERAL;
    
    p_glb_data->trace_data[ICF_TRACE_SVC_END_RECVD].p_trace_string= 
        (icf_uint8_t *)"Service Ended received.";
    p_glb_data->trace_data[ICF_TRACE_SVC_END_RECVD].trace_level=
        ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SVC_END_RECVD].trace_type=
        ICF_TRACE_TYPE_GENERAL;
    
    p_glb_data->trace_data[ICF_TRACE_SL_CTX_FREED].p_trace_string=(icf_uint8_t *)" Service Logic Context Freed. ";
    p_glb_data->trace_data[ICF_TRACE_SL_CTX_FREED].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SL_CTX_FREED].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_CH_SUCCESS_STATUS].p_trace_string=(icf_uint8_t *)" Service Ended Normal. Call Hold sucess response in return Path. ";
    p_glb_data->trace_data[ICF_TRACE_CH_SUCCESS_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_CH_SUCCESS_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_CR_SUCCESS_STATUS].p_trace_string=(icf_uint8_t *)" Service Ended Normal. Call Resume success response in return Path. ";
    p_glb_data->trace_data[ICF_TRACE_CR_SUCCESS_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_CR_SUCCESS_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SVC_ERR_IND_STATUS].p_trace_string=(icf_uint8_t *)" Service Error Indication recieved in status. ";
    p_glb_data->trace_data[ICF_TRACE_SVC_ERR_IND_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SVC_ERR_IND_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_CH_REQ_STATUS].p_trace_string=(icf_uint8_t *)" Call Hold Request recived in Status. ";
    p_glb_data->trace_data[ICF_TRACE_CH_REQ_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_CH_REQ_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_CR_REQ_STATUS].p_trace_string=(icf_uint8_t *)" Call Resume Request recived in Status. ";
    p_glb_data->trace_data[ICF_TRACE_CR_REQ_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_CR_REQ_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_START_PROC_MEDIA_STATUS].p_trace_string=(icf_uint8_t *)" Start Proc Media Request. ";
    p_glb_data->trace_data[ICF_TRACE_START_PROC_MEDIA_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_START_PROC_MEDIA_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_START_PROC_SIG_STATUS].p_trace_string=(icf_uint8_t *)" Start Proc Signalling Request. ";
    p_glb_data->trace_data[ICF_TRACE_START_PROC_SIG_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_START_PROC_SIG_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SIG_EVENTS_REQ_STATUS].p_trace_string=(icf_uint8_t *)" Got ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ in Status. ";
    p_glb_data->trace_data[ICF_TRACE_SIG_EVENTS_REQ_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SIG_EVENTS_REQ_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_EVENTS_REQ_STATUS].p_trace_string=(icf_uint8_t *)" Got ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ in Status. ";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_EVENTS_REQ_STATUS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_EVENTS_REQ_STATUS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_ALL_SVC_ENDED].p_trace_string=(icf_uint8_t *)" No service Active, Freeing Service Context.";
    p_glb_data->trace_data[ICF_TRACE_ALL_SVC_ENDED].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ALL_SVC_ENDED].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_INV_API_RECVD].p_trace_string=(icf_uint8_t *)" Invalid API received. ";
    p_glb_data->trace_data[ICF_TRACE_INV_API_RECVD].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_INV_API_RECVD].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SEND_FAILURE_TO_PA].p_trace_string=(icf_uint8_t *)" Sending Failure response to PA. ";
    p_glb_data->trace_data[ICF_TRACE_SEND_FAILURE_TO_PA].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SEND_FAILURE_TO_PA].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SEND_FAILURE_TO_SL].p_trace_string=(icf_uint8_t *)" Sending Failure Response to Service ";
    p_glb_data->trace_data[ICF_TRACE_SEND_FAILURE_TO_SL].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SEND_FAILURE_TO_SL].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_FAILURE_RESP_SEND_FAIL].p_trace_string=(icf_uint8_t *)" Failed to send Failure Response. ";
    p_glb_data->trace_data[ICF_TRACE_FAILURE_RESP_SEND_FAIL].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_FAILURE_RESP_SEND_FAIL].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_GET_HANDLER_SL].p_trace_string=(icf_uint8_t *)" icf_scm_get_handler_module returned Handler module ";
    p_glb_data->trace_data[ICF_TRACE_GET_HANDLER_SL].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_GET_HANDLER_SL].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_ONE_CALL].p_trace_string=(icf_uint8_t *)" New Service Request Recieved for Single Call. ";
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_ONE_CALL].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_ONE_CALL].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_TWO_CALLS].p_trace_string=(icf_uint8_t *)" New Service Request Recieved for Two Calls. ";
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_TWO_CALLS].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_TWO_CALLS].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_RNW_INV_STATE].p_trace_string=(icf_uint8_t *)" Refer from network recived in Invalid State. Changing API to ICF_SC_INTERNAL_RNW_REJ. ";
    p_glb_data->trace_data[ICF_TRACE_RNW_INV_STATE].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RNW_INV_STATE].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_VALID].p_trace_string=(icf_uint8_t *)" Service Request Validated against Service Interaction Rules. ";
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_VALID].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_VALID].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SVC_CTX_CREATE].p_trace_string=(icf_uint8_t *)" Service Context Created. ";
    p_glb_data->trace_data[ICF_TRACE_SVC_CTX_CREATE].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SVC_CTX_CREATE].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SL_CTX_CREATE].p_trace_string=(icf_uint8_t *)" Service Logic Context Created for ";
    p_glb_data->trace_data[ICF_TRACE_SL_CTX_CREATE].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SL_CTX_CREATE].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_INVALID].p_trace_string= (icf_uint8_t *)" Service Request Not valid according to Service Interaction Rules. ";
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_INVALID].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_SVC_REQ_INVALID].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_CALL_RELEASE_FORWARDED].p_trace_string=(icf_uint8_t *)"Call Release sent to ";
    p_glb_data->trace_data[ICF_TRACE_CALL_RELEASE_FORWARDED].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_RELEASE_FORWARDED].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_CALL_ID_EXTRACTED].p_trace_string=(icf_uint8_t *)"Call Id of Other Participant Call ";
    p_glb_data->trace_data[ICF_TRACE_CALL_ID_EXTRACTED].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_CALL_ID_EXTRACTED].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_GET_CALL_CTX].p_trace_string=(icf_uint8_t *)"Got Call Context for call id ";
    p_glb_data->trace_data[ICF_TRACE_GET_CALL_CTX].trace_level= ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_GET_CALL_CTX].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_MSG_ROUTING_FAILED].p_trace_string=(icf_uint8_t *)"Failure in routing Message";
    p_glb_data->trace_data[ICF_TRACE_MSG_ROUTING_FAILED].trace_level= ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_TRACE_MSG_ROUTING_FAILED].trace_type=ICF_TRACE_TYPE_GENERAL;
    p_glb_data->trace_data[ICF_TRACE_MSG_ROUTING_PROGRESS].p_trace_string=(icf_uint8_t *)"Forwarding API to module "; 
p_glb_data->trace_data[ICF_TRACE_MSG_ROUTING_PROGRESS].trace_level= ICF_TRACE_LEVEL_BRIEF;
p_glb_data->trace_data[ICF_TRACE_MSG_ROUTING_PROGRESS].trace_type=ICF_TRACE_TYPE_GENERAL;

/* -------------------------------------------------------------------------
 * Traces for RGM module
 * ------------------------------------------------------------------------*/
  p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_RECVD].p_trace_string = 
       (icf_uint8_t *)"[Init Complete received from CFG Module]";
  p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_RECVD].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_RECVD].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_SUCCESSFUL].p_trace_string
      = (icf_uint8_t *)"[Init Complete Successful]";
  p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_SUCCESSFUL].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_SUCCESSFUL].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_MODIFY_USER_ADDRESS_RECVD].p_trace_string
      =(icf_uint8_t *)"[Received request for User Address Modification from CFG]";
  p_glb_data->trace_data[ICF_TRACE_MODIFY_USER_ADDRESS_RECVD].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_MODIFY_USER_ADDRESS_RECVD].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_USER_BLK].p_trace_string
      = (icf_uint8_t *)"[Processing User Address Modification Block]";
  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_USER_BLK].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_USER_BLK].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_REGISTRATION_FSM_TRIGGERED].p_trace_string
      = (icf_uint8_t *)"[Triggering Registration State Machine]";
  p_glb_data->trace_data[ICF_TRACE_REGISTRATION_FSM_TRIGGERED].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_REGISTRATION_FSM_TRIGGERED].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
   p_glb_data->trace_data[ICF_TRACE_MODIFY_USER_ROLLBACK].p_trace_string
      = (icf_uint8_t *)"[Starting ROLLBACK Processing due to failure of Modify User Address]";
  p_glb_data->trace_data[ICF_TRACE_MODIFY_USER_ROLLBACK].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_MODIFY_USER_ROLLBACK].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
 
  p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ADDRESS_RECVD].p_trace_string
      = (icf_uint8_t *)"[Received request for Registrar Address Addition/Deletion from CFG]";
  p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ADDRESS_RECVD].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ADDRESS_RECVD].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_REG_BLK].p_trace_string
      = (icf_uint8_t *)"[Processing Registrar Address Addition/Deletion Block Node]";
  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_REG_BLK].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_REG_BLK].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
   p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ROLLBACK].p_trace_string
      = (icf_uint8_t *)"[Starting ROLLBACK Processing due to failure of ADD/DEL REGISTRAR \
      Address]";
  p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ROLLBACK].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ROLLBACK].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_MODIFY_DURATION_RECVD].p_trace_string
      = (icf_uint8_t *)"[Received request for Modification of Registration Duration from CFG]";
  p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ADDRESS_RECVD].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_MODIFY_REG_ADDRESS_RECVD].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
 
  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_DUR_BLK].p_trace_string
      = (icf_uint8_t *)"[Processing Registration Duration Modification Block Node]";
  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_DUR_BLK].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_PROCESS_MODIFY_DUR_BLK].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;  

  p_glb_data->trace_data[ICF_TRACE_MODIFY_DUR_ROLLBACK].p_trace_string
      = (icf_uint8_t *)"[Starting ROLLBACK Processing due to failure of Registration Duration\
      Modification]";
  p_glb_data->trace_data[ICF_TRACE_MODIFY_DUR_ROLLBACK].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_MODIFY_DUR_ROLLBACK].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
  p_glb_data->trace_data[ICF_TRACE_INVALID_STATE_EVENT].p_trace_string
      = (icf_uint8_t *)"[Event Unexpected in Current State]";
  p_glb_data->trace_data[ICF_TRACE_INVALID_STATE_EVENT].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_INVALID_STATE_EVENT].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
 
  p_glb_data->trace_data[ICF_TRACE_RETRY_TIMER_EXPIRY].p_trace_string
      = (icf_uint8_t *)"[Registration Retry Timer Expired]";
  p_glb_data->trace_data[ICF_TRACE_RETRY_TIMER_EXPIRY].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_RETRY_TIMER_EXPIRY].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
  p_glb_data->trace_data[ICF_TRACE_REG_TIMER_EXPIRY].p_trace_string
      = (icf_uint8_t *)"[Registration Duration Expired]";
  p_glb_data->trace_data[ICF_TRACE_REG_TIMER_EXPIRY].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_REG_TIMER_EXPIRY].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_REG_REATT_TIMER_EXPIRY].p_trace_string
      = (icf_uint8_t *)"[Registration Reattempt Timer Expired]";
  p_glb_data->trace_data[ICF_TRACE_REG_REATT_TIMER_EXPIRY].trace_level = 
       ICF_TRACE_LEVEL_DETAILED;
  p_glb_data->trace_data[ICF_TRACE_REG_REATT_TIMER_EXPIRY].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_SENDING_REG_REQUEST].p_trace_string
      = (icf_uint8_t *)"[Sending Registration Request]";
  p_glb_data->trace_data[ICF_TRACE_SENDING_REG_REQUEST].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_SENDING_REG_REQUEST].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_SENDING_DEREG_REQUEST].p_trace_string
      = (icf_uint8_t *)"[Sending Deregistration Request]";
  p_glb_data->trace_data[ICF_TRACE_SENDING_DEREG_REQUEST].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_SENDING_DEREG_REQUEST].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_DEREG_SUCCESS_RESP].p_trace_string
      = (icf_uint8_t *)"[Received Success Response for Deregistration Request]";
  p_glb_data->trace_data[ICF_TRACE_DEREG_SUCCESS_RESP].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_DEREG_SUCCESS_RESP].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
  p_glb_data->trace_data[ICF_TRACE_REG_SUCCESS_RESP].p_trace_string
      = (icf_uint8_t *)"[Received Success Response for Registration Request]";
  p_glb_data->trace_data[ICF_TRACE_REG_SUCCESS_RESP].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_REG_SUCCESS_RESP].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
  p_glb_data->trace_data[ICF_TRACE_DEREG_FAIL_RESP].p_trace_string
      = (icf_uint8_t *)"[Received Failure Response for De-Registration Request]";
  p_glb_data->trace_data[ICF_TRACE_DEREG_FAIL_RESP].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_DEREG_FAIL_RESP].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_REG_FAIL_RESP].p_trace_string
      = (icf_uint8_t *)"[Received Failure Response for Registration Request]";
  p_glb_data->trace_data[ICF_TRACE_REG_FAIL_RESP].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_REG_FAIL_RESP].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
  p_glb_data->trace_data[ICF_TRACE_DEREG_FAIL_NO_REATT_RESP].p_trace_string
      = (icf_uint8_t *)"[Received Failure Response for De-Registration Request With No Reattempt]";
  p_glb_data->trace_data[ICF_TRACE_DEREG_FAIL_NO_REATT_RESP].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_DEREG_FAIL_NO_REATT_RESP].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;  

  
  p_glb_data->trace_data[ICF_TRACE_REG_FAIL_NO_REATT_RESP].p_trace_string
      = (icf_uint8_t *)"[Received Failure Response for Registration Request With No Reattempt]";
  p_glb_data->trace_data[ICF_TRACE_REG_FAIL_NO_REATT_RESP].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_REG_FAIL_NO_REATT_RESP].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;


  p_glb_data->trace_data[ICF_TRACE_AUTHENTICATION_REQ].p_trace_string
      = (icf_uint8_t *)"[Received Authentication Required Response]";
  p_glb_data->trace_data[ICF_TRACE_AUTHENTICATION_REQ].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_AUTHENTICATION_REQ].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;

  p_glb_data->trace_data[ICF_TRACE_DURATION_SHORT].p_trace_string
      = (icf_uint8_t *)"[Received Duration Short Response]";
  p_glb_data->trace_data[ICF_TRACE_DURATION_SHORT].trace_level = 
       ICF_TRACE_LEVEL_BRIEF;
  p_glb_data->trace_data[ICF_TRACE_DURATION_SHORT].trace_type = 
       ICF_TRACE_TYPE_REGISTRATION;
  
  
    /* -------------------------------------------------------------------------
     * Trace ids used by Call Manager for non-failure scenarios.
     * -----------------------------------------------------------------------*/

    p_glb_data->trace_data[ICF_MSG_PROC_STATUS].p_trace_string = 
        (icf_uint8_t *)"Status of interface message processing : ";
    p_glb_data->trace_data[ICF_MSG_PROC_STATUS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_STATUS].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_MSG_PROC_RECV_INT_MSG].p_trace_string = 
        (icf_uint8_t *)"Internal message received : ";
    p_glb_data->trace_data[ICF_MSG_PROC_RECV_INT_MSG].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_RECV_INT_MSG].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_MSG_PROC_ACTION_RESULT].p_trace_string = 
        (icf_uint8_t *)"Internal message being dispatched : ";
    p_glb_data->trace_data[ICF_MSG_PROC_ACTION_RESULT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_ACTION_RESULT].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_MSG_PROC_FIELD_DATA].p_trace_string = 
        (icf_uint8_t *)"Current value of field data used for routing decision : ";
    p_glb_data->trace_data[ICF_MSG_PROC_FIELD_DATA].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_FIELD_DATA].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;

    p_glb_data->trace_data[ICF_MSG_PROC_FIELD_TYPE].p_trace_string = 
        (icf_uint8_t *)"Field type set : ";
    p_glb_data->trace_data[ICF_MSG_PROC_FIELD_TYPE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_FIELD_TYPE].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;
    
    p_glb_data->trace_data[ICF_MSG_PROC_PRESENCE_FIELD].p_trace_string = 
        (icf_uint8_t *)"Presence field : ";
    p_glb_data->trace_data[ICF_MSG_PROC_PRESENCE_FIELD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_PRESENCE_FIELD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_MSG_PROC_VALUE_ACTION].p_trace_string = 
        (icf_uint8_t *)"Tree node containing a particular value of a field : ";
    p_glb_data->trace_data[ICF_MSG_PROC_VALUE_ACTION].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_VALUE_ACTION].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;
  
    p_glb_data->trace_data[ICF_MSG_PROC_ACTION].p_trace_string = 
        (icf_uint8_t *)"Action for a particular value of a field : ";
    p_glb_data->trace_data[ICF_MSG_PROC_ACTION].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MSG_PROC_ACTION].trace_type = 
        ICF_TRACE_TYPE_INTERFACE;
  
    /* -------------------------------------------------------------------------
     * End Trace ids used by Call Manager for non-failure scenarios.
     * -----------------------------------------------------------------------*/

    /* -------------------------------------------------------------------------
     * Trace ids used by CHR
     * -----------------------------------------------------------------------*/

    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_RECVD].p_trace_string = 
        (icf_uint8_t *)"Call Hold request receives";
    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_RECVD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CALL_RESUME_RECVD].p_trace_string = 
        (icf_uint8_t *)"Call Resume request receives";
    p_glb_data->trace_data[ICF_TRACE_CALL_RESUME_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_RESUME_RECVD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_SUCC_RESP].p_trace_string = 
        (icf_uint8_t *)"Call Hold Success response sent";
    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_SUCC_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_SUCC_RESP].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_MMS_SEND].p_trace_string = 
        (icf_uint8_t *)"MMS request sent";
    p_glb_data->trace_data[ICF_TRACE_MMS_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_SEND].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_HOLD_REQ_TO_NW].p_trace_string = 
        (icf_uint8_t *)"Call hold req to network sent";
    p_glb_data->trace_data[ICF_TRACE_HOLD_REQ_TO_NW].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_HOLD_REQ_TO_NW].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_RSM_REQ_TO_NW].p_trace_string = 
        (icf_uint8_t *)"Call Resume request to netwok sent";
    p_glb_data->trace_data[ICF_TRACE_RSM_REQ_TO_NW].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RSM_REQ_TO_NW].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_RSM_RESP_FROM_NW].p_trace_string = 
        (icf_uint8_t *)"Call Resume response from netwok receive";
    p_glb_data->trace_data[ICF_TRACE_RSM_RESP_FROM_NW].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RSM_RESP_FROM_NW].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_HLD_FAIL_FROM_NW].p_trace_string = 
        (icf_uint8_t *)"Call hold failure from network";
    p_glb_data->trace_data[ICF_TRACE_HLD_FAIL_FROM_NW].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_HLD_FAIL_FROM_NW].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_REQ_CANCEL_SEND].p_trace_string = 
        (icf_uint8_t *)"Cnacel request to netwok sent";
    p_glb_data->trace_data[ICF_TRACE_REQ_CANCEL_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REQ_CANCEL_SEND].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CANCEL_RESP_RECV].p_trace_string = 
        (icf_uint8_t *)"Cancel Response from netwok recv";
    p_glb_data->trace_data[ICF_TRACE_CANCEL_RESP_RECV].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CANCEL_RESP_RECV].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_MED_ERR_DMS_IND].p_trace_string = 
        (icf_uint8_t *)"MMS failure unrecoverable recv";
    p_glb_data->trace_data[ICF_TRACE_MED_ERR_DMS_IND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MED_ERR_DMS_IND].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_MED_ERR_MMS_FAIL].p_trace_string = 
        (icf_uint8_t *)"MMS failure recoverable recv";
    p_glb_data->trace_data[ICF_TRACE_MED_ERR_MMS_FAIL].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MED_ERR_MMS_FAIL].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CHR_TIMER_EXPD].p_trace_string = 
        (icf_uint8_t *)"Call Hold Resume timer expired";
    p_glb_data->trace_data[ICF_TRACE_CHR_TIMER_EXPD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CHR_TIMER_EXPD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_RACE_TIMER_EXPD].p_trace_string = 
        (icf_uint8_t *)"Reinvite race timer expired";
    p_glb_data->trace_data[ICF_TRACE_RACE_TIMER_EXPD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RACE_TIMER_EXPD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_ST_PROC_SIG_RCVD].p_trace_string = 
        (icf_uint8_t *)"Start Proc signaling recvd";
    p_glb_data->trace_data[ICF_TRACE_ST_PROC_SIG_RCVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ST_PROC_SIG_RCVD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_ST_PROC_MED_RCVD].p_trace_string = 
        (icf_uint8_t *)"Start proc media recvd";
    p_glb_data->trace_data[ICF_TRACE_ST_PROC_MED_RCVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ST_PROC_MED_RCVD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CALL_RELEASED].p_trace_string = 
        (icf_uint8_t *)"Call Released recvd";
    p_glb_data->trace_data[ICF_TRACE_CALL_RELEASED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_RELEASED].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CALL_ALREADY_HOLD].p_trace_string = 
        (icf_uint8_t *)"Call Already on hold";
    p_glb_data->trace_data[ICF_TRACE_CALL_ALREADY_HOLD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_ALREADY_HOLD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SERVICE_TERM_NORMAL].p_trace_string = 
        (icf_uint8_t *)"CHR service is terminated normal";
    p_glb_data->trace_data[ICF_TRACE_SERVICE_TERM_NORMAL].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SERVICE_TERM_NORMAL].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SERVICE_TERM_ERROR].p_trace_string = 
        (icf_uint8_t *)"CHR service is terminated Error";
    p_glb_data->trace_data[ICF_TRACE_SERVICE_TERM_ERROR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SERVICE_TERM_ERROR].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CALL_REMOTE_HOLD].p_trace_string = 
        (icf_uint8_t *)"Call is remote hold";
    p_glb_data->trace_data[ICF_TRACE_CALL_REMOTE_HOLD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_REMOTE_HOLD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SIG_ONGOING].p_trace_string = 
        (icf_uint8_t *)"Signaling Ongoing in other module";
    p_glb_data->trace_data[ICF_TRACE_SIG_ONGOING].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SIG_ONGOING].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SET_ST_PROC_SIG].p_trace_string = 
        (icf_uint8_t *)"Request to set start proc signaling";
    p_glb_data->trace_data[ICF_TRACE_SET_ST_PROC_SIG].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_ST_PROC_SIG].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SSA_FAIL_RESP].p_trace_string = 
        (icf_uint8_t *)"SSA failure response recv";
    p_glb_data->trace_data[ICF_TRACE_SSA_FAIL_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SSA_FAIL_RESP].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SSA_SUCC_RESP].p_trace_string = 
        (icf_uint8_t *)"SSA success recv";
    p_glb_data->trace_data[ICF_TRACE_SSA_SUCC_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SSA_SUCC_RESP].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CHR_STATE].p_trace_string = 
        (icf_uint8_t *)"Current state of FSM";
    p_glb_data->trace_data[ICF_TRACE_CHR_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CHR_STATE].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CHR_EVENT].p_trace_string = 
        (icf_uint8_t *)"Current event recvd";
    p_glb_data->trace_data[ICF_TRACE_CHR_EVENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CHR_EVENT].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_ONGOING_ACT].p_trace_string = 
        (icf_uint8_t *)"CHR ongoing action";
    p_glb_data->trace_data[ICF_TRACE_ONGOING_ACT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ONGOING_ACT].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_PENDING_EVT].p_trace_string = 
        (icf_uint8_t *)"CHR pending event";
    p_glb_data->trace_data[ICF_TRACE_PENDING_EVT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PENDING_EVT].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_MMI_FAIL_RESP].p_trace_string = 
        (icf_uint8_t *)"MMI failure response";
    p_glb_data->trace_data[ICF_TRACE_MMI_FAIL_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMI_FAIL_RESP].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_MMI_SUCC_RESP].p_trace_string = 
        (icf_uint8_t *)"MMI success response";
    p_glb_data->trace_data[ICF_TRACE_MMI_SUCC_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMI_SUCC_RESP].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SET_ST_PROC_MED].p_trace_string = 
        (icf_uint8_t *)"Request to set Start proc media";
    p_glb_data->trace_data[ICF_TRACE_SET_ST_PROC_MED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SET_ST_PROC_MED].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_MED_ONGOING].p_trace_string = 
        (icf_uint8_t *)"Media is going on in other module";
    p_glb_data->trace_data[ICF_TRACE_MED_ONGOING].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MED_ONGOING].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CALL_ALRADY_ACTIVE].p_trace_string = 
        (icf_uint8_t *)"Call is already active";
    p_glb_data->trace_data[ICF_TRACE_CALL_ALRADY_ACTIVE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_ALRADY_ACTIVE].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_MMS_SUCC_RESP].p_trace_string = 
        (icf_uint8_t *)"MMS success response recevd";
    p_glb_data->trace_data[ICF_TRACE_MMS_SUCC_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_SUCC_RESP].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CURR_RET_STATUS].p_trace_string = 
        (icf_uint8_t *)"Current return status is : ";
    p_glb_data->trace_data[ICF_TRACE_CURR_RET_STATUS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CURR_RET_STATUS].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_NO_SDP_PRESENT].p_trace_string =
        (icf_uint8_t *)"No SDP is present in final response";
    p_glb_data->trace_data[ICF_TRACE_NO_SDP_PRESENT].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NO_SDP_PRESENT].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_NUMBER_OF_CODEC_DIFF].p_trace_string =
        (icf_uint8_t *)"Number of codec is different in final response";
    p_glb_data->trace_data[ICF_TRACE_NUMBER_OF_CODEC_DIFF].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NUMBER_OF_CODEC_DIFF].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CODEC_DIFF].p_trace_string =
        (icf_uint8_t *)"Codec received in final response is different";
    p_glb_data->trace_data[ICF_TRACE_CODEC_DIFF].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CODEC_DIFF].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_CODEC_SAME].p_trace_string =
        (icf_uint8_t *)"Codec received in final response is same as previous one";
    p_glb_data->trace_data[ICF_TRACE_CODEC_SAME].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CODEC_SAME].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_ADDR_DIFF].p_trace_string =
        (icf_uint8_t *)"Transport address in final response is different";
    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_ADDR_DIFF].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_ADDR_DIFF].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_PACKETIZATION_DIFF].p_trace_string =
        (icf_uint8_t *)"Packetization delay is different in final response";
    p_glb_data->trace_data[ICF_TRACE_PACKETIZATION_DIFF].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PACKETIZATION_DIFF].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    p_glb_data->trace_data[ICF_TRACE_SDP_SAME].p_trace_string =
        (icf_uint8_t *)"Received SDP is same as previous one";
    p_glb_data->trace_data[ICF_TRACE_SDP_SAME].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SDP_SAME].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CALL_HOLD_RESUME;

    /* -------------------------------------------------------------------------
     * Trace ids used by TWC
     * -----------------------------------------------------------------------*/

    p_glb_data->trace_data[ICF_TRACE_TWC_RECVD].p_trace_string =
        (icf_uint8_t *)"Conference request received";
    p_glb_data->trace_data[ICF_TRACE_TWC_RECVD].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TWC_RECVD].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TRACE_MERGE_MEDIA_RESP].p_trace_string =
        (icf_uint8_t *)"Merge media response received";
    p_glb_data->trace_data[ICF_TRACE_MERGE_MEDIA_RESP].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MERGE_MEDIA_RESP].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_MEDIA_NOT_COMMITTED].p_trace_string 
        = (icf_uint8_t *)"Media not committed in one call";
    p_glb_data->trace_data[ICF_TWC_TRACE_MEDIA_NOT_COMMITTED].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_MEDIA_NOT_COMMITTED].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_CALL_LOCAL_HOLD].p_trace_string =
        (icf_uint8_t *)"Call is local held";
    p_glb_data->trace_data[ICF_TWC_TRACE_CALL_LOCAL_HOLD].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_CALL_LOCAL_HOLD].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_END_OTHER_CALL].p_trace_string =
        (icf_uint8_t *)"End call other then one which is released";
    p_glb_data->trace_data[ICF_TWC_TRACE_END_OTHER_CALL].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_END_OTHER_CALL].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_TERMINATE_SERVICE].p_trace_string =
        (icf_uint8_t *)"Conference service terminated";
    p_glb_data->trace_data[ICF_TWC_TRACE_TERMINATE_SERVICE].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_TERMINATE_SERVICE].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_MEDIA_ONGOING].p_trace_string =
        (icf_uint8_t *)"Media is ongoing in one of two calls";
    p_glb_data->trace_data[ICF_TWC_TRACE_MEDIA_ONGOING].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_MEDIA_ONGOING].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_MERGE_MEDIA_SENT].p_trace_string =
        (icf_uint8_t *)"Merge media request is sent";
    p_glb_data->trace_data[ICF_TWC_TRACE_MERGE_MEDIA_SENT].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_MERGE_MEDIA_SENT].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_FAILURE_CONF_RESP_SENT].
        p_trace_string =
        (icf_uint8_t *)"Failure conference response is sent to ADM";
    p_glb_data->trace_data[ICF_TWC_TRACE_FAILURE_CONF_RESP_SENT].trace_level 
        = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_FAILURE_CONF_RESP_SENT].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_BOTH_CALL_ENDED].p_trace_string =
        (icf_uint8_t *)"Both calls in conference are ended";
    p_glb_data->trace_data[ICF_TWC_TRACE_BOTH_CALL_ENDED].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_BOTH_CALL_ENDED].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CHD].
        p_trace_string =
        (icf_uint8_t *)"CHR service is active for call hold";
    p_glb_data->trace_data[ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CHD].trace_level 
        = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CHD].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CRM].
        p_trace_string =
        (icf_uint8_t *)"CHR service is active for call resume";
    p_glb_data->trace_data[ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CRM].trace_level 
        = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CRM].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TRACE_START_TIMER].p_trace_string =
        (icf_uint8_t *)"Start timer";
    p_glb_data->trace_data[ICF_TRACE_START_TIMER].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_START_TIMER].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TRACE_STOP_TIMER].p_trace_string =
        (icf_uint8_t *)"Stop timer";
    p_glb_data->trace_data[ICF_TRACE_STOP_TIMER].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_STOP_TIMER].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;
    /* more trace data will be added here */

    p_glb_data->trace_data[ICF_TRACE_TWC_STATE].p_trace_string =
        (icf_uint8_t *)"Current state of FSM";
    p_glb_data->trace_data[ICF_TRACE_TWC_STATE].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TWC_STATE].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TRACE_TWC_EVENT].p_trace_string =
        (icf_uint8_t *)"Current event recvd";
    p_glb_data->trace_data[ICF_TRACE_TWC_EVENT].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TWC_EVENT].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TRACE_NEXT_STATE].p_trace_string =
        (icf_uint8_t *)"Next State is: ";
    p_glb_data->trace_data[ICF_TRACE_NEXT_STATE].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NEXT_STATE].trace_type =
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    p_glb_data->trace_data[ICF_TWC_TRACE_INVALID_SERVICE_SCENARIO].
         p_trace_string = (icf_uint8_t *)"Both calls are in setup phase, invalid action";
    p_glb_data->trace_data[ICF_TWC_TRACE_INVALID_SERVICE_SCENARIO].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TWC_TRACE_INVALID_SERVICE_SCENARIO].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_THREE_WAY_CALL;

    /* -------------------------------------------------------------------------
     * Trace ids used by RNW
     * -----------------------------------------------------------------------*/
    p_glb_data->trace_data[ICF_TRACE_RNW_RECVD].
         p_trace_string = (icf_uint8_t *)"REFER received";
    p_glb_data->trace_data[ICF_TRACE_RNW_RECVD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RNW_RECVD].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_TRACE_RNW_WITH_REPLACES].
         p_trace_string = (icf_uint8_t *)"REFER received with REPLACES header";
    p_glb_data->trace_data[ICF_TRACE_RNW_WITH_REPLACES].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RNW_WITH_REPLACES].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_TRACE_RNW_NEW_CALL_ID].
         p_trace_string = (icf_uint8_t *)"Call-ID of new call";
    p_glb_data->trace_data[ICF_TRACE_RNW_NEW_CALL_ID].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RNW_NEW_CALL_ID].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_RNW_TRACE_REFER_RESP_SENT].
         p_trace_string = (icf_uint8_t *)"REFER response sent to network";
    p_glb_data->trace_data[ICF_RNW_TRACE_REFER_RESP_SENT].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_RNW_TRACE_REFER_RESP_SENT].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_RNW_TRACE_NOTIFY_SENT].
         p_trace_string = (icf_uint8_t *)"NOTIFY is sent to referring party";
    p_glb_data->trace_data[ICF_RNW_TRACE_NOTIFY_SENT].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_RNW_TRACE_NOTIFY_SENT].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_RNW_TRACE_CC_INITIATED_CALL_REL].
         p_trace_string = (icf_uint8_t *)"Call release has been initiated";
    p_glb_data->trace_data[ICF_RNW_TRACE_CC_INITIATED_CALL_REL].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_RNW_TRACE_CC_INITIATED_CALL_REL].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_TRACE_MERGE_IND_SENT].
         p_trace_string = (icf_uint8_t *)"Both calls are merged and indication is sent to PA";
    p_glb_data->trace_data[ICF_TRACE_MERGE_IND_SENT].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MERGE_IND_SENT].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_TRACE_MERGE_MEDIA_SENT].
         p_trace_string = (icf_uint8_t *)"Both calls are merged and indication is sent to PA";
    p_glb_data->trace_data[ICF_TRACE_MERGE_MEDIA_SENT].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MERGE_MEDIA_SENT].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_TRACE_REFER_IND_SENT].
         p_trace_string = (icf_uint8_t *)"Both calls are merged and indication is sent to PA";
    p_glb_data->trace_data[ICF_TRACE_REFER_IND_SENT].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REFER_IND_SENT].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;

    p_glb_data->trace_data[ICF_RNW_TRACE_NEXT_STATE].
         p_trace_string = (icf_uint8_t *)"Next State is : ";
    p_glb_data->trace_data[ICF_RNW_TRACE_NEXT_STATE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_RNW_TRACE_NEXT_STATE].trace_type
        = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_REFER;


    /* -------------------------------------------------------------------------
     * Trace ids used by CFG for non-failure scenarios.
     * -----------------------------------------------------------------------*/

    p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_STATUS].p_trace_string = 
        (icf_uint8_t *)"Init Complete Status : ";
    p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_STATUS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_STATUS].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_RECD].p_trace_string = 
        (icf_uint8_t *)"Init Complete has already been received once : ";
    p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_RECD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_INIT_COMPLETE_RECD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

  
    p_glb_data->trace_data[ICF_TRACE_PARAM_VALIDITY].p_trace_string = 
        (icf_uint8_t *)"Validity of received configuration parameters : ";
    p_glb_data->trace_data[ICF_TRACE_PARAM_VALIDITY].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PARAM_VALIDITY].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_TRACE_INVALID_MSG_ID_RECD].p_trace_string = 
        (icf_uint8_t *)"Invalid message ID received : ";
    p_glb_data->trace_data[ICF_TRACE_INVALID_MSG_ID_RECD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_INVALID_MSG_ID_RECD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_TRACE_INVALID_API_ID_RECD].p_trace_string = 
        (icf_uint8_t *)"Invalid API ID received : ";
    p_glb_data->trace_data[ICF_TRACE_INVALID_API_ID_RECD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_INVALID_API_ID_RECD].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_TRACE_SEND_RESP_FAILURE].p_trace_string = 
        (icf_uint8_t *)"Response send failure : ";
    p_glb_data->trace_data[ICF_TRACE_SEND_RESP_FAILURE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SEND_RESP_FAILURE].trace_type = 
        ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
   
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_LINE_PRMS_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Received Scalable Line Params: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_LINE_PRMS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_LINE_PRMS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_CALL_PRMS_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Received Scalable Call Params: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_CALL_PRMS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_CALL_PRMS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

   
    p_glb_data->trace_data[ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"API to set Transport Params invoked: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_TRNSPT_PRMS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
  
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Received Proxy address list's params: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Received Secondary proxy address list's params: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Received secondary proxy address params: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_LINE_RECD].
         p_trace_string = (icf_uint8_t *)"Line data for proxy received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_LINE_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_LINE_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Proxy Address received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Secondary proxy Address received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Primary DNS Server Address received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Secondary DNS Server Address received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"DNS Server Address received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_DNS_SERVER_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
   
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Secondary DNS Server Address received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_DNS_SERVER_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Received Self Transport address:";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_PARAMS_RECD].
        p_trace_string =(icf_uint8_t *)"Recd Call Params:Presence flag ";
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_PARAMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_PARAMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_RECD].
         p_trace_string = (icf_uint8_t *)"Recd call default settings & QOS: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_TIMERS_RECD].
         p_trace_string = (icf_uint8_t *)"Call Timers Received: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_TIMERS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_TIMERS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REG_TIMERS_RECD].
         p_trace_string = (icf_uint8_t *)"Registration Timers have been received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_TIMERS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_TIMERS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_PRMS_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Self Id Parameters Received: presence flag";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_PRMS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_PRMS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_ADDR_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"Recd self_address's all_lines & num_of_elements";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_ADDR_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_ADDR_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_NAME_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"Recd Self_Name's params: all_lines & num_of_elements";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_NAME_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_NAME_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_DEF_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"Recd Self_def_setting's all_lines & num_of_elements";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_DEF_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ID_DEF_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Recd Self (User) Address";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_NAME_RECD].
         p_trace_string = (icf_uint8_t *)"Recd Self Name";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_NAME_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_NAME_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_DEF_RECD].
         p_trace_string = (icf_uint8_t *)"Received Self_id Default settings";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_DEF_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_DEF_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_REQ_RECD].
        p_trace_string =(icf_uint8_t *)"Registration parameters: all_lines & num_of_elements"; 
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"Registrar Address parameters received";
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Registration Address received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_REG_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"Secondary registration Address received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_REG_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_REG_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"Other Registration parameters received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_PRMS_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Received Service parameters:";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_PRMS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_PRMS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_TIMERS_RECD].
         p_trace_string = (icf_uint8_t *)"Received Service timers";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_TIMERS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_TIMERS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_FWD_TIMERS_RECD].
         p_trace_string = (icf_uint8_t *)"Received Call forward timers";
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_FWD_TIMERS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_FWD_TIMERS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"Service Flags'params received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_RECD].
         p_trace_string = (icf_uint8_t *)"Service Subscription & Activation flags received";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"CFU Address params received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"CFU Address received";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"CFB Address params received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"CFB Address received";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"CFD Address params received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_RECD].
         p_trace_string = (icf_uint8_t *)"CFD Address received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"VMS API params received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_ADDR_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"VMS Address received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_ADDR_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_ADDR_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
   
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_OTHER_PRMS_RECD].
         p_trace_string = (icf_uint8_t *)"Other VMS Params received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_OTHER_PRMS_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_OTHER_PRMS_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_RESP_SENT].
         p_trace_string = (icf_uint8_t *)"API Response Sent:";
    p_glb_data->trace_data[ICF_CFG_TRACE_RESP_SENT].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_RESP_SENT].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_LINE_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"Scalable Line Params have been set:";
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_LINE_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_LINE_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_CALL_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"Scalable Call Params have been set:";
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_CALL_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SCALABLE_CALL_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_LINE_SET].
         p_trace_string = (icf_uint8_t *)"Proxy address's parameters for a line set:";
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_LINE_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDR_LINE_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDRESS_SET].
         p_trace_string = (icf_uint8_t *)"Proxy Address set on this line";
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDRESS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_PROXY_ADDRESS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDRESS_SET].
         p_trace_string = (icf_uint8_t *)"Secondary Proxy Address set on this line";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDRESS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_PROXY_ADDRESS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_ADDR_SET].
         p_trace_string = (icf_uint8_t *)"Self Transport Address has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_ADDR_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_ADDR_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET].
         p_trace_string = (icf_uint8_t *)"Self Transport Mode has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_TRNSPT_MODE_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_SET].
         p_trace_string = (icf_uint8_t *)"Call's def setting and QOS set";
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_DEFAULT_SETTING_N_QOS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_TIMERS_SET].
         p_trace_string = (icf_uint8_t *)"Call timers have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_TIMERS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_TIMERS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REG_TIMERS_SET].
         p_trace_string = (icf_uint8_t *)"Registration Timers have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_TIMERS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_TIMERS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_MAX_NUM_OF_USERS_REACHED].
         p_trace_string = (icf_uint8_t *)"This line will have more users than max value if API is processed:" ;
    p_glb_data->trace_data[ICF_CFG_TRACE_MAX_NUM_OF_USERS_REACHED].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_MAX_NUM_OF_USERS_REACHED].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_SET].
         p_trace_string = (icf_uint8_t *)"Self Address has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_NAME_SET].
         p_trace_string = (icf_uint8_t *)"Self Name has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_NAME_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_NAME_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_DEFAULT_SETTINGS_SET].
         p_trace_string = (icf_uint8_t *)"Self Default Setings has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_DEFAULT_SETTINGS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_DEFAULT_SETTINGS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_LINE_BLK_DEALLOCATED].
         p_trace_string = (icf_uint8_t *)"Memory for this line has been freed";
    p_glb_data->trace_data[ICF_CFG_TRACE_LINE_BLK_DEALLOCATED].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_LINE_BLK_DEALLOCATED].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REQ_ALREADY_RECD].
         p_trace_string = (icf_uint8_t *)"This Request has already been received:";
    p_glb_data->trace_data[ICF_CFG_TRACE_REQ_ALREADY_RECD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REQ_ALREADY_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_SET].
         p_trace_string = (icf_uint8_t *)"Registrar Address has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_ADDR_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_REG_ADDR_SET].
         p_trace_string = (icf_uint8_t *)"Secondary registrar address has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_REG_ADDR_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SEC_REG_ADDR_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"Registration params have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_REG_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"Service Params have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_TIMERS_SET].
         p_trace_string = (icf_uint8_t *)"Service Timers have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_TIMERS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_TIMERS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_FWD_TIMERS_SET].
         p_trace_string = (icf_uint8_t *)"Service Params: Call forward timers have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_FWD_TIMERS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CALL_FWD_TIMERS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_SET].
         p_trace_string = (icf_uint8_t *)"Service Subscription & Activation flags set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_FLAGS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_SET].
         p_trace_string = (icf_uint8_t *)"CFU Address has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFU_ADDR_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_SET].
         p_trace_string = (icf_uint8_t *)"CFB Address has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFB_ADDR_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_SET].
         p_trace_string = (icf_uint8_t *)"CFD Address has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_CFD_ADDR_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_GRP_ADDR_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"VMS Group Address Params have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_GRP_ADDR_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_GRP_ADDR_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_GRP_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"VMS Group Duration & status of subsc have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_GRP_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_GRP_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_LINE_ADDR_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"VMS Line Address params have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_LINE_ADDR_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_LINE_ADDR_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;
    
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_LINE_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"VMS Line Duration has been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_LINE_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_VMS_LINE_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_SUBSCRIBE_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"Service Susbscribe params have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_SUBSCRIBE_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SRVC_SUBSCRIBE_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_PRMS_SET].
         p_trace_string = (icf_uint8_t *)"Self Address params have been set";
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_PRMS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_SELF_ADDR_PRMS_SET].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_CONFIG;

   /* -------------------------------------------------------------------------
     * End Trace ids used by CFG for non-failure scenarios.
     * -----------------------------------------------------------------------*/

    /* -------------------------------------------------------------------------
     * Trace ids used by SM handler.
     * -----------------------------------------------------------------------*/

    p_glb_data->trace_data[ICF_SM_TRACE_TRACE_LVL_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Set Trace Level requsest received.";
    p_glb_data->trace_data[ICF_SM_TRACE_TRACE_LVL_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_TRACE_LVL_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

    p_glb_data->trace_data[ICF_SM_TRACE_ERROR_LVL_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Set Error report Level requsest received.";
    p_glb_data->trace_data[ICF_SM_TRACE_ERROR_LVL_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_ERROR_LVL_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

    p_glb_data->trace_data[ICF_SM_TRACE_SET_STATS_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Set Stats request received.";
    p_glb_data->trace_data[ICF_SM_TRACE_SET_STATS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_SET_STATS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

    p_glb_data->trace_data[ICF_SM_TRACE_GET_STATS_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Get Stats Request Received.";
    p_glb_data->trace_data[ICF_SM_TRACE_GET_STATS_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_GET_STATS_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

    p_glb_data->trace_data[ICF_SM_TRACE_GET_STATS_RESP_SENT].
         p_trace_string = (icf_uint8_t *)"Get Stats Response Sent";
    p_glb_data->trace_data[ICF_SM_TRACE_GET_STATS_RESP_SENT].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_GET_STATS_RESP_SENT].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

    p_glb_data->trace_data[ICF_SM_TRACE_CHK_MODULE_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Check Module Status requets Received";
    p_glb_data->trace_data[ICF_SM_TRACE_CHK_MODULE_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_CHK_MODULE_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

    p_glb_data->trace_data[ICF_SM_TRACE_CHK_MODULE_RESP_SENT].
         p_trace_string = (icf_uint8_t *)"Check Module Status response Sent";
    p_glb_data->trace_data[ICF_SM_TRACE_CHK_MODULE_RESP_SENT].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_CHK_MODULE_RESP_SENT].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;


 p_glb_data->trace_data[ICF_SM_TRACE_CALL_LOG_REQ_RECD].
         p_trace_string = (icf_uint8_t *)"Get Call log request received.";
    p_glb_data->trace_data[ICF_SM_TRACE_CALL_LOG_REQ_RECD].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_CALL_LOG_REQ_RECD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

 p_glb_data->trace_data[ICF_SM_TRACE_CALL_LOG_RESP_SENT].
         p_trace_string = (icf_uint8_t *)"Call log response sent.";
    p_glb_data->trace_data[ICF_SM_TRACE_CALL_LOG_RESP_SENT].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SM_TRACE_CALL_LOG_RESP_SENT].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;


    p_glb_data->trace_data[ICF_SM_TRACE_VALIDATE_PARAM].
         p_trace_string = (icf_uint8_t *)"Parameter Validity Check performed.";
    p_glb_data->trace_data[ICF_SM_TRACE_VALIDATE_PARAM].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_VALIDATE_PARAM].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;

    p_glb_data->trace_data[ICF_SM_TRACE_NUM_CALLS_RECORD].
         p_trace_string = (icf_uint8_t *)"Num_calls_to_log stored.";
    p_glb_data->trace_data[ICF_SM_TRACE_NUM_CALLS_RECORD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_NUM_CALLS_RECORD].
        trace_type = ICF_TRACE_TYPE_INTERFACE | ICF_TRACE_TYPE_SM;


    p_glb_data->trace_data[ICF_SM_TRACE_DIALED_CALL_LOG_REQ].
         p_trace_string = (icf_uint8_t *)"Dialed Call logging request received.";
    p_glb_data->trace_data[ICF_SM_TRACE_DIALED_CALL_LOG_REQ].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_DIALED_CALL_LOG_REQ].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL
                     |ICF_TRACE_TYPE_SM
                     |ICF_TRACE_TYPE_GENERAL;

    p_glb_data->trace_data[ICF_SM_TRACE_DIALED_CALL_LOG_RESP].
         p_trace_string = (icf_uint8_t *)"Dialed Call logged.";
    p_glb_data->trace_data[ICF_SM_TRACE_DIALED_CALL_LOG_RESP].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_DIALED_CALL_LOG_RESP].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL
                     |ICF_TRACE_TYPE_SM
                     |ICF_TRACE_TYPE_GENERAL;

    p_glb_data->trace_data[ICF_SM_TRACE_MISSED_CALL_LOG_REQ].
         p_trace_string = (icf_uint8_t *)"Missed Call logging request received.";
    p_glb_data->trace_data[ICF_SM_TRACE_MISSED_CALL_LOG_REQ].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_MISSED_CALL_LOG_REQ].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL
                     |ICF_TRACE_TYPE_SM
                     |ICF_TRACE_TYPE_GENERAL;

    p_glb_data->trace_data[ICF_SM_TRACE_MISSED_CALL_LOG_RESP].
         p_trace_string = (icf_uint8_t *)"Missed Call logged.";
    p_glb_data->trace_data[ICF_SM_TRACE_MISSED_CALL_LOG_RESP].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_MISSED_CALL_LOG_RESP].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL
                     |ICF_TRACE_TYPE_SM
                     |ICF_TRACE_TYPE_GENERAL;

    p_glb_data->trace_data[ICF_SM_TRACE_RECD_CALL_LOG_REQ].
         p_trace_string = (icf_uint8_t *)"Received Call logging request received.";
    p_glb_data->trace_data[ICF_SM_TRACE_RECD_CALL_LOG_REQ].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_RECD_CALL_LOG_REQ].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL
                     |ICF_TRACE_TYPE_SM
                     |ICF_TRACE_TYPE_GENERAL;

    p_glb_data->trace_data[ICF_SM_TRACE_RECD_CALL_LOG_RESP].
         p_trace_string = (icf_uint8_t *)"Received Call logged.";
    p_glb_data->trace_data[ICF_SM_TRACE_RECD_CALL_LOG_RESP].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SM_TRACE_RECD_CALL_LOG_RESP].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL
                     |ICF_TRACE_TYPE_SM
                     |ICF_TRACE_TYPE_GENERAL;
    
    /********* Trace strings for CFW SLP ********************/
    
    p_glb_data->trace_data[ICF_CFW_TRACE_DIV_INFO_PRESENT].p_trace_string =
        (icf_uint8_t *)"Diversion info present in call ctx";
    p_glb_data->trace_data[ICF_CFW_TRACE_DIV_INFO_PRESENT].trace_level =
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_DIV_INFO_PRESENT].trace_type =
        ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_DIV_INFO_NOT_PRESENT].
         p_trace_string = (icf_uint8_t *)"Diversion info not present in call ctx";
    p_glb_data->trace_data[ICF_CFW_TRACE_DIV_INFO_NOT_PRESENT].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_DIV_INFO_NOT_PRESENT].
        trace_type = ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_MSG_ID].
         p_trace_string = (icf_uint8_t *)"Invalid msg id received";
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_MSG_ID].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_MSG_ID].
        trace_type = ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_CFW_TYPE].
         p_trace_string = (icf_uint8_t *)"Invalid Call forwarding type received";
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_CFW_TYPE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_CFW_TYPE].
        trace_type = ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_CFW_TYPE].
         p_trace_string = (icf_uint8_t *)"Call forwarding type received:";
    p_glb_data->trace_data[ICF_CFW_TRACE_CFW_TYPE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_CFW_TYPE].
        trace_type = ICF_TRACE_TYPE_CALL_FORWARDING;
       
    p_glb_data->trace_data[ICF_CFW_TRACE_VALIDITY].p_trace_string = 
        (icf_uint8_t *)"Validity of CFW Request:";
    p_glb_data->trace_data[ICF_CFW_TRACE_VALIDITY].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_VALIDITY].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_MSG_SEND].p_trace_string = 
        (icf_uint8_t *)"302 send successfully";
    p_glb_data->trace_data[ICF_CFW_TRACE_MSG_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_MSG_SEND].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_FAIL_TO_GET_CFG_DATA].
         p_trace_string = (icf_uint8_t *)"DBM failure to return CFG data";
    p_glb_data->trace_data[ICF_CFW_TRACE_FAIL_TO_GET_CFG_DATA].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_FAIL_TO_GET_CFG_DATA].
        trace_type = ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_LINE_ID].p_trace_string = 
        (icf_uint8_t *)"Fetching pointer for line:";
    p_glb_data->trace_data[ICF_CFW_TRACE_LINE_ID].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_LINE_ID].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_FAIL_TO_GET_LINE_DATA].
         p_trace_string = (icf_uint8_t *)"Failure to get line data pointer form DBM:";
    p_glb_data->trace_data[ICF_CFW_TRACE_FAIL_TO_GET_LINE_DATA].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_FAIL_TO_GET_LINE_DATA].
        trace_type = ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_CALLING_CFW_ADDR_MATCH].
         p_trace_string = (icf_uint8_t *)"Calling party address same as the address to which the call is to be forwarded:";
    p_glb_data->trace_data[ICF_CFW_TRACE_CALLING_CFW_ADDR_MATCH].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_CALLING_CFW_ADDR_MATCH].
        trace_type = ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_HOPS].p_trace_string = 
        (icf_uint8_t *)"Number of remaining hops is invalid";
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_HOPS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_INVALID_HOPS].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_CFW_DIV_ADDR_MATCH].
         p_trace_string = (icf_uint8_t *)"CFW Address already in the diversion data";
    p_glb_data->trace_data[ICF_CFW_TRACE_CFW_DIV_ADDR_MATCH].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_CFW_DIV_ADDR_MATCH].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
        
    p_glb_data->trace_data[ICF_CFW_TRACE_SELF_CFW_ADDR_MATCH].
         p_trace_string = (icf_uint8_t *)"CFW Address matches with self address:";
    p_glb_data->trace_data[ICF_CFW_TRACE_SELF_CFW_ADDR_MATCH].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_SELF_CFW_ADDR_MATCH].trace_type = 
       ICF_TRACE_TYPE_CALL_FORWARDING;
    
    p_glb_data->trace_data[ICF_CFW_TRACE_DATA_SEND].
         p_trace_string = (icf_uint8_t *)"Data send to SSA:";
    p_glb_data->trace_data[ICF_CFW_TRACE_DATA_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_DATA_SEND].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
    
    p_glb_data->trace_data[ICF_CFW_TRACE_STATUS].
         p_trace_string = (icf_uint8_t *)"Status:";
    p_glb_data->trace_data[ICF_CFW_TRACE_STATUS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_STATUS].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
    
    p_glb_data->trace_data[ICF_CFW_TRACE_REL_TYPE_N_CAUSE].
         p_trace_string = (icf_uint8_t *)"Release Type and cause:";
    p_glb_data->trace_data[ICF_CFW_TRACE_REL_TYPE_N_CAUSE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFW_TRACE_REL_TYPE_N_CAUSE].trace_type = 
        ICF_TRACE_TYPE_CALL_FORWARDING;
    
    /*********** End trace strings for CFW **********************/
    
    /************Traces for MMI**********************************/
    p_glb_data->trace_data[ICF_EVENT_RECVD].p_trace_string =
       (icf_uint8_t *)"Event Received is:  ";
    p_glb_data->trace_data[ICF_EVENT_RECVD].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_EVENT_RECVD].trace_type =
       ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_API_SEND].p_trace_string =
       (icf_uint8_t *)"API send:  ";
    p_glb_data->trace_data[ICF_TRACE_API_SEND].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_API_SEND].trace_type =
       ICF_TRACE_TYPE_MEDIA;
  
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_CODEC_CHANGE].p_trace_string =
       (icf_uint8_t *)"Modify Request for codec change";
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_CODEC_CHANGE].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_CODEC_CHANGE].trace_type =
       ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_MEDIA_COMMITED].p_trace_string =
       (icf_uint8_t *)"Modify Request for codec change";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_COMMITED].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_COMMITED].trace_type =
       ICF_TRACE_TYPE_GENERAL;
    
    p_glb_data->trace_data[ICF_TRACE_MEDIA_NOT_COMMITED].p_trace_string =
       (icf_uint8_t *)"Media is not commited";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_NOT_COMMITED].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_NOT_COMMITED].trace_type =
       ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_DMS_NOT_FORCEFUL].p_trace_string =
       (icf_uint8_t *)"DMS request received is not forceful";
    p_glb_data->trace_data[ICF_TRACE_DMS_NOT_FORCEFUL].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_DMS_NOT_FORCEFUL].trace_type =
       ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_FORCEFUL_DMS].p_trace_string =
       (icf_uint8_t *)"DMS request received is forceful";
    p_glb_data->trace_data[ICF_TRACE_FORCEFUL_DMS].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FORCEFUL_DMS].trace_type =
       ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_FAILURE_RESP].p_trace_string =
       (icf_uint8_t *)"Failure Response for the API ios received:    ";
    p_glb_data->trace_data[ICF_TRACE_FAILURE_RESP].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FAILURE_RESP].trace_type =
       ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_SUCCESS_RESP].p_trace_string =
       (icf_uint8_t *)"Success Response for the API is received:    ";
    p_glb_data->trace_data[ICF_TRACE_SUCCESS_RESP].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SUCCESS_RESP].trace_type =
       ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_CC_AWT_DMS].p_trace_string =
       (icf_uint8_t *)"CC is in AWT DMS state";
    p_glb_data->trace_data[ICF_TRACE_CC_AWT_DMS].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CC_AWT_DMS].trace_type =
       ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MEDIA_ESTABLISHED].p_trace_string =
       (icf_uint8_t *)"Media is established";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_ESTABLISHED].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_ESTABLISHED].trace_type =
       ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MEDIA_NOT_ESTABLISHED].p_trace_string =
       (icf_uint8_t *)"Media is not established";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_NOT_ESTABLISHED].trace_level =
       ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_NOT_ESTABLISHED].trace_type =
       ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MODIFY_FOR_CODEC_CHANGE_SET].
         p_trace_string = (icf_uint8_t *)"Modify for codec change is set";
    p_glb_data->trace_data[ICF_TRACE_MODIFY_FOR_CODEC_CHANGE_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MODIFY_FOR_CODEC_CHANGE_SET].
        trace_type = ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_RESTORATION_IN_PROGRESS_SET].
         p_trace_string = (icf_uint8_t *)"Restoration in progress is set";
    p_glb_data->trace_data[ICF_TRACE_RESTORATION_IN_PROGRESS_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_RESTORATION_IN_PROGRESS_SET].
        trace_type = ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_MODE_CHANGE].p_trace_string = 
        (icf_uint8_t *)"MMS is for mode change";
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_MODE_CHANGE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_MODE_CHANGE].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MMS_WITH_REMOTE_SDP].p_trace_string = 
        (icf_uint8_t *)"Remote SDP is present";
    p_glb_data->trace_data[ICF_TRACE_MMS_WITH_REMOTE_SDP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_WITH_REMOTE_SDP].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->
        trace_data[ICF_TRACE_MODIFY_FOR_REMOTE_MEDIA_APPLICATION_SET].
         p_trace_string = (icf_uint8_t *)"Bit Remote media app is set";
    p_glb_data->
        trace_data[ICF_TRACE_MODIFY_FOR_REMOTE_MEDIA_APPLICATION_SET].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->
        trace_data[ICF_TRACE_MODIFY_FOR_REMOTE_MEDIA_APPLICATION_SET].
        trace_type = ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_CALL_INCOMING_FS].p_trace_string = 
        (icf_uint8_t *)"Call is incoming FS";
    p_glb_data->trace_data[ICF_TRACE_CALL_INCOMING_FS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_INCOMING_FS].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_FAILURE_TO_SEND].p_trace_string = 
        (icf_uint8_t *)"Failure to send msg";
    p_glb_data->trace_data[ICF_TRACE_FAILURE_TO_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FAILURE_TO_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_FAILURE_BY_CM].p_trace_string = 
        (icf_uint8_t *)"Failure returned by CM";
    p_glb_data->trace_data[ICF_TRACE_FAILURE_BY_CM].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FAILURE_BY_CM].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_FAILURE_TO_PROCESS_MSG].
         p_trace_string = (icf_uint8_t *)"Failure to process msg";
    p_glb_data->trace_data[ICF_TRACE_FAILURE_TO_PROCESS_MSG].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FAILURE_TO_PROCESS_MSG].
        trace_type = ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_FAILURE_BY_DBM].p_trace_string = 
        (icf_uint8_t *)"Failure returned by DBM";
    p_glb_data->trace_data[ICF_TRACE_FAILURE_BY_DBM].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FAILURE_BY_DBM].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_UNEXPECTED_EVENT].p_trace_string = 
        (icf_uint8_t *)"Unexpected event is redeived";
    p_glb_data->trace_data[ICF_TRACE_UNEXPECTED_EVENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_UNEXPECTED_EVENT].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_MMI_TRACE_CURRENT_STATE].p_trace_string = 
        (icf_uint8_t *)"Current state is:  ";
    p_glb_data->trace_data[ICF_MMI_TRACE_CURRENT_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_MMI_TRACE_CURRENT_STATE].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_REMOTE_MEDIA_CHANGE].
         p_trace_string = (icf_uint8_t *)"MMS is for remote media change:";
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_REMOTE_MEDIA_CHANGE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_REMOTE_MEDIA_CHANGE].
        trace_type = ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_REMOTE_MEDIA_APPLICATION].
         p_trace_string = (icf_uint8_t *)"MMS for remote media application:  ";
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_REMOTE_MEDIA_APPLICATION].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_FOR_REMOTE_MEDIA_APPLICATION].
        trace_type = ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_NUM_OF_LOCAL_CODECS].
         p_trace_string = (icf_uint8_t *)"NUmber of local codecs: ";
    p_glb_data->trace_data[ICF_TRACE_NUM_OF_LOCAL_CODECS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NUM_OF_LOCAL_CODECS].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_NEGOTIATION_FAILURE].p_trace_string = 
        (icf_uint8_t *)"Codec negotiation failure";
    p_glb_data->trace_data[ICF_TRACE_NEGOTIATION_FAILURE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NEGOTIATION_FAILURE].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_HDR_SEND].p_trace_string = 
        (icf_uint8_t *)"Data in hdr:";
    p_glb_data->trace_data[ICF_TRACE_HDR_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_HDR_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_GCL_DATA_SEND].p_trace_string = 
        (icf_uint8_t *)"Data send in GCL request: ";
    p_glb_data->trace_data[ICF_TRACE_GCL_DATA_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_GCL_DATA_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_NUM_OF_CODECS].p_trace_string = 
        (icf_uint8_t *)"NUmber of codecs: ";
    p_glb_data->trace_data[ICF_TRACE_NUM_OF_CODECS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NUM_OF_CODECS].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_CODEC].p_trace_string = 
        (icf_uint8_t *)"Codec: ";
    p_glb_data->trace_data[ICF_TRACE_CODEC].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CODEC].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_MMS_DATA_SEND].p_trace_string = 
        (icf_uint8_t *)"Data send in MMS request:";
    p_glb_data->trace_data[ICF_TRACE_MMS_DATA_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_DATA_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_MMS_SDP_SEND].p_trace_string = 
        (icf_uint8_t *)"SDP send in GCL request: ";
    p_glb_data->trace_data[ICF_TRACE_MMS_SDP_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_SDP_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_MMS_ADDR_SEND].p_trace_string = 
        (icf_uint8_t *)"Address in MMS req: ";
    p_glb_data->trace_data[ICF_TRACE_MMS_ADDR_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MMS_ADDR_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_COMMIT_DATA_SEND].p_trace_string = 
        (icf_uint8_t *)"Data send in commit request: ";
    p_glb_data->trace_data[ICF_TRACE_COMMIT_DATA_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_COMMIT_DATA_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_CALL_OTG_OR_INC_NFS].p_trace_string = 
        (icf_uint8_t *)"Call outgoing or incoming NFS:";
    p_glb_data->trace_data[ICF_TRACE_CALL_OTG_OR_INC_NFS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_OTG_OR_INC_NFS].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_CMS_DATA_SEND].p_trace_string = 
        (icf_uint8_t *)"Data send in CMS request: ";
    p_glb_data->trace_data[ICF_TRACE_CMS_DATA_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CMS_DATA_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_LOCAL_AUDIO_CODEC_LIST].
         p_trace_string = (icf_uint8_t *)"Local audio codec list: ";
    p_glb_data->trace_data[ICF_TRACE_LOCAL_AUDIO_CODEC_LIST].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_LOCAL_AUDIO_CODEC_LIST].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_REMOTE_SDP].p_trace_string = 
        (icf_uint8_t *)"Remote SDP: ";
    p_glb_data->trace_data[ICF_TRACE_REMOTE_SDP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REMOTE_SDP].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_CMS_SDP_SEND].p_trace_string = 
        (icf_uint8_t *)"SDP send in CMS request:";
    p_glb_data->trace_data[ICF_TRACE_CMS_SDP_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CMS_SDP_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_CMS_ADDR_SEND].p_trace_string = 
        (icf_uint8_t *)"Address send in CMS request: ";
    p_glb_data->trace_data[ICF_TRACE_CMS_ADDR_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CMS_ADDR_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_GCL_DATA_RECVD].p_trace_string = 
        (icf_uint8_t *)"Data recieved in GCL Req: ";
    p_glb_data->trace_data[ICF_TRACE_GCL_DATA_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_GCL_DATA_RECVD].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_CMS_DATA_RECVD].p_trace_string = 
        (icf_uint8_t *)"Data received in CMS request:";
    p_glb_data->trace_data[ICF_TRACE_CMS_DATA_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CMS_DATA_RECVD].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_CMS_SDP_RECVD].p_trace_string = 
        (icf_uint8_t *)"SDP received in CMS request: ";
    p_glb_data->trace_data[ICF_TRACE_CMS_SDP_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CMS_SDP_RECVD].trace_type = 
        ICF_TRACE_TYPE_MEDIA;
    
    p_glb_data->trace_data[ICF_TRACE_ADDR].p_trace_string = 
        (icf_uint8_t *)"Address in the API is:";
    p_glb_data->trace_data[ICF_TRACE_ADDR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ADDR].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_MERGE_DATA_SEND].p_trace_string = 
        (icf_uint8_t *)"Data send in merge request: ";
    p_glb_data->trace_data[ICF_TRACE_MERGE_DATA_SEND].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MERGE_DATA_SEND].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_STREAM_ID].p_trace_string = 
        (icf_uint8_t *)"Stream information: \n Stream ID: ";
    p_glb_data->trace_data[ICF_TRACE_STREAM_ID].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_STREAM_ID].trace_type = 
        ICF_TRACE_TYPE_MEDIA;

    p_glb_data->trace_data[ICF_TRACE_STREAM_TYPE].p_trace_string = 
        (icf_uint8_t *)"Stream information: \n Stream TYPE";
    p_glb_data->trace_data[ICF_TRACE_STREAM_TYPE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_STREAM_TYPE].trace_type = 
        ICF_TRACE_TYPE_MEDIA;	
    
    /**********End trace strings for MMI**************************/
    p_glb_data->trace_data[ICF_CFG_TRACE_NETWORK_ACTIVATE_RECD].p_trace_string = 
        (icf_uint8_t *)"Network Activate Request Recvd: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_NETWORK_ACTIVATE_RECD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_NETWORK_ACTIVATE_RECD].trace_type = 
        ICF_TRACE_TYPE_CONFIG ;

    p_glb_data->trace_data[ICF_CFG_TRACE_NETWORK_DEACTIVATE_RECD].p_trace_string = 
        (icf_uint8_t *)"Network De-Activate Request Recvd: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_NETWORK_DEACTIVATE_RECD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_NETWORK_DEACTIVATE_RECD].trace_type = 
        ICF_TRACE_TYPE_CONFIG ;

	
    p_glb_data->trace_data[ICF_CFG_TRACE_APP_ADD_RECD].p_trace_string = 
        (icf_uint8_t *)"Application Add Request Recvd: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_APP_ADD_RECD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_APP_ADD_RECD].trace_type = 
        ICF_TRACE_TYPE_CONFIG ;

    p_glb_data->trace_data[ICF_CFG_TRACE_APP_REMOVE_RECD].p_trace_string = 
        (icf_uint8_t *)"Application Remove Request Recvd: ";
    p_glb_data->trace_data[ICF_CFG_TRACE_APP_REMOVE_RECD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_CFG_TRACE_APP_REMOVE_RECD].trace_type = 
        ICF_TRACE_TYPE_CONFIG ;
 
     /*********************************************************************
     * SSA TRACES
     *
     *********************************************************************/
    p_glb_data->trace_data[ICF_TRACE_ENTERED_SSA_PROCESS_MESG].
        p_trace_string = 
        (icf_uint8_t *)"Entered SSA process message";
    p_glb_data->trace_data[ICF_TRACE_ENTERED_SSA_PROCESS_MESG].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_ENTERED_SSA_PROCESS_MESG].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_APP_POINTER].p_trace_string = 
        (icf_uint8_t *)"Application pointer";
    p_glb_data->trace_data[ICF_TRACE_APP_POINTER].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_APP_POINTER].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SSA_BITMASK].p_trace_string = 
        (icf_uint8_t *)"SSA context bitmask value";
    p_glb_data->trace_data[ICF_TRACE_SSA_BITMASK].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SSA_BITMASK].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TCP_CONN_DB_INDEX].p_trace_string = 
        (icf_uint8_t *)"SSA TCP connection DB index";
    p_glb_data->trace_data[ICF_TRACE_TCP_CONN_DB_INDEX].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TCP_CONN_DB_INDEX].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TCP_EVENT].p_trace_string = 
        (icf_uint8_t *)"SSA TCP Event";
    p_glb_data->trace_data[ICF_TRACE_TCP_EVENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TCP_EVENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_STRAY_MSG_RECVD].p_trace_string = 
        (icf_uint8_t *)"Stray message received";
    p_glb_data->trace_data[ICF_TRACE_STRAY_MSG_RECVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_STRAY_MSG_RECVD].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_DEST_MODULE_ID].p_trace_string = 
        (icf_uint8_t *)"Internal message send to module";
    p_glb_data->trace_data[ICF_TRACE_DEST_MODULE_ID].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_DEST_MODULE_ID].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD].p_trace_string = 
        (icf_uint8_t *)"SIP method";
    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_CALL_LEG_MATCHED].p_trace_string = 
        (icf_uint8_t *)"Call leg matched";
    p_glb_data->trace_data[ICF_TRACE_CALL_LEG_MATCHED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_LEG_MATCHED].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_UPDATE_CALL_DET_STATUS].p_trace_string 
        = (icf_uint8_t *)"Updating call detail status";
    p_glb_data->trace_data[ICF_TRACE_UPDATE_CALL_DET_STATUS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_UPDATE_CALL_DET_STATUS].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SSA_PDB_BITMASK].p_trace_string = 
        (icf_uint8_t *)"SSA PDB bitmask value";
    p_glb_data->trace_data[ICF_TRACE_SSA_PDB_BITMASK].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SSA_PDB_BITMASK].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SSA_CTX_FAILURE_BITMASK].
        p_trace_string = 
        (icf_uint8_t *)"SSA Context failure bitmask value";
    p_glb_data->trace_data[ICF_TRACE_SSA_CTX_FAILURE_BITMASK].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SSA_CTX_FAILURE_BITMASK].trace_type = 
        ICF_TRACE_TYPE_SSA;

    
    p_glb_data->trace_data[ICF_TRACE_SENT_RESP_CODE].p_trace_string = 
        (icf_uint8_t *)"Response code sent to remote";
    p_glb_data->trace_data[ICF_TRACE_SENT_RESP_CODE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SENT_RESP_CODE].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_MODE].p_trace_string = 
        (icf_uint8_t *)"Transport mode";
    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_MODE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_MODE].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_SCHEME].p_trace_string = 
        (icf_uint8_t *)"Transport scheme";
    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_SCHEME].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TRANSPORT_SCHEME].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_NUMBER_OF_DIV_HDRS].p_trace_string = 
        (icf_uint8_t *)"Number of diversion headers";
    p_glb_data->trace_data[ICF_TRACE_NUMBER_OF_DIV_HDRS].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NUMBER_OF_DIV_HDRS].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_PRIV_HDR_PRESENT].p_trace_string = 
        (icf_uint8_t *)"Outgoing Privacy header present";
    p_glb_data->trace_data[ICF_TRACE_PRIV_HDR_PRESENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PRIV_HDR_PRESENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_REF_BY_HDR_PRESENT].p_trace_string = 
        (icf_uint8_t *)"Referred By header present";
    p_glb_data->trace_data[ICF_TRACE_REF_BY_HDR_PRESENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REF_BY_HDR_PRESENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_REPLACES_HDR_PRESENT].p_trace_string = 
        (icf_uint8_t *)"Replaces header present";
    p_glb_data->trace_data[ICF_TRACE_REPLACES_HDR_PRESENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REPLACES_HDR_PRESENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TCP_STATE].p_trace_string = 
        (icf_uint8_t *)"SSA TCP State";
    p_glb_data->trace_data[ICF_TRACE_TCP_STATE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TCP_STATE].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TRANS_MODE_REVERT_TO_UDP].
        p_trace_string = 
        (icf_uint8_t *)"Transport mode reverted to UDP";
    p_glb_data->trace_data[ICF_TRACE_TRANS_MODE_REVERT_TO_UDP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TRANS_MODE_REVERT_TO_UDP].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_CONNECT_SENT_WO_SDP].p_trace_string = 
        (icf_uint8_t *)"Connect to remote sent without SDP";
    p_glb_data->trace_data[ICF_TRACE_CONNECT_SENT_WO_SDP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CONNECT_SENT_WO_SDP].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_OTG_RPR_PRESENT].p_trace_string = 
        (icf_uint8_t *)"Outgoing RPR present";
    p_glb_data->trace_data[ICF_TRACE_OTG_RPR_PRESENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_OTG_RPR_PRESENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD_SENT].p_trace_string = 
        (icf_uint8_t *)"SIP method sent to remote";
    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD_SENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD_SENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD_RCVD].p_trace_string = 
        (icf_uint8_t *)"SIP method received from remote";
    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD_RCVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SIP_METHOD_RCVD].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_PRACK_SENT_W_SDP].p_trace_string = 
        (icf_uint8_t *)"PRACK sent without SDP";
    p_glb_data->trace_data[ICF_TRACE_PRACK_SENT_W_SDP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PRACK_SENT_W_SDP].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_MED_CHNG_RESP].p_trace_string = 
        (icf_uint8_t *)"Media change response";
    p_glb_data->trace_data[ICF_TRACE_MED_CHNG_RESP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MED_CHNG_RESP].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_TYPE].p_trace_string = 
        (icf_uint8_t *)"Call hold type";
    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_TYPE].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_CALL_HOLD_TYPE].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_NOTIFY_EVENT].p_trace_string = 
        (icf_uint8_t *)"Notify event";
    p_glb_data->trace_data[ICF_TRACE_NOTIFY_EVENT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NOTIFY_EVENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_REG_REQ].p_trace_string = 
        (icf_uint8_t *)"Register request";
    p_glb_data->trace_data[ICF_TRACE_REG_REQ].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REG_REQ].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TIMER_START].p_trace_string = 
        (icf_uint8_t *)"Timer started";
    p_glb_data->trace_data[ICF_TRACE_TIMER_START].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TIMER_START].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TIMER_STOP].p_trace_string = 
        (icf_uint8_t *)"Timer stoped";
    p_glb_data->trace_data[ICF_TRACE_TIMER_STOP].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TIMER_STOP].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TIMER_EXPIRY].p_trace_string = 
        (icf_uint8_t *)"Timer Expired";
    p_glb_data->trace_data[ICF_TRACE_TIMER_EXPIRY].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TIMER_EXPIRY].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SOCKET_CONNECTED].p_trace_string = 
        (icf_uint8_t *)"Socket connected";
    p_glb_data->trace_data[ICF_TRACE_SOCKET_CONNECTED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SOCKET_CONNECTED].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_FREE_APP_DATA_CALLED].p_trace_string = 
        (icf_uint8_t *)"Free application data called";
    p_glb_data->trace_data[ICF_TRACE_FREE_APP_DATA_CALLED].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_FREE_APP_DATA_CALLED].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_IND_TXN_TIMED_OUT].p_trace_string = 
        (icf_uint8_t *)"Transaction timed out";
    p_glb_data->trace_data[ICF_TRACE_IND_TXN_TIMED_OUT].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_IND_TXN_TIMED_OUT].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_TEL_URL_TO_IP_ADDR].p_trace_string = 
        (icf_uint8_t *)"Tel-URL to IP address";
    p_glb_data->trace_data[ICF_TRACE_TEL_URL_TO_IP_ADDR].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_TEL_URL_TO_IP_ADDR].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_REPLACE_NW_CALL_ID].p_trace_string = 
        (icf_uint8_t *)"Relaces NW Call-ID";
    p_glb_data->trace_data[ICF_TRACE_REPLACE_NW_CALL_ID].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_REPLACE_NW_CALL_ID].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_VALIDATION_OF_REQ_URI_FAILURE].
         p_trace_string = (icf_uint8_t *)"Validation for Request URI failure";
    p_glb_data->trace_data[ICF_TRACE_VALIDATION_OF_REQ_URI_FAILURE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_VALIDATION_OF_REQ_URI_FAILURE].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_MEDIA_CHANGE_RCVD_W_SDP].
         p_trace_string = (icf_uint8_t *)"Media change received with SDP";
    p_glb_data->trace_data[ICF_TRACE_MEDIA_CHANGE_RCVD_W_SDP].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MEDIA_CHANGE_RCVD_W_SDP].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_OPTIONS_RCVD_OUT_OF_DIALOG].
         p_trace_string = (icf_uint8_t *)"OPTIONS received out of dialog";
    p_glb_data->trace_data[ICF_TRACE_OPTIONS_RCVD_OUT_OF_DIALOG].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_OPTIONS_RCVD_OUT_OF_DIALOG].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_OPTIONS_USER_NOT_FOUND].
         p_trace_string = (icf_uint8_t *)"OPTIONS-User not found";
    p_glb_data->trace_data[ICF_TRACE_OPTIONS_USER_NOT_FOUND].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_OPTIONS_USER_NOT_FOUND].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_OPTIONS_USER_BUSY].
         p_trace_string = (icf_uint8_t *)"OPTIONS-User busy";
    p_glb_data->trace_data[ICF_TRACE_OPTIONS_USER_BUSY].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_OPTIONS_USER_BUSY].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SUBSCRIBE_RESP].
         p_trace_string = (icf_uint8_t *)"SUBSCRIBE response";
    p_glb_data->trace_data[ICF_TRACE_SUBSCRIBE_RESP].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SUBSCRIBE_RESP].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SUBSCRIBE_EVENT_PACKAGE].
         p_trace_string = (icf_uint8_t *)"SUBSCRIBE event package";
    p_glb_data->trace_data[ICF_TRACE_SUBSCRIBE_EVENT_PACKAGE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SUBSCRIBE_EVENT_PACKAGE].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_NOTIFY_RCVD].p_trace_string = 
        (icf_uint8_t *)"NOTIFY received";
    p_glb_data->trace_data[ICF_TRACE_NOTIFY_RCVD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NOTIFY_RCVD].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_INFO_RCVD_W_PAYLOAD].p_trace_string = 
        (icf_uint8_t *)"INFO received with payload";
    p_glb_data->trace_data[ICF_TRACE_INFO_RCVD_W_PAYLOAD].trace_level = 
        ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_INFO_RCVD_W_PAYLOAD].trace_type = 
        ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_SUPPORTED_100_REL_PRESENT].
         p_trace_string = (icf_uint8_t *)"100 rel present in supported";
    p_glb_data->trace_data[ICF_TRACE_SUPPORTED_100_REL_PRESENT].trace_level 
        = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_SUPPORTED_100_REL_PRESENT].trace_type = 
        ICF_TRACE_TYPE_SSA;

	p_glb_data->trace_data[ICF_TRACE_REQUIRE_PRECONDITION_PRESENT].
		p_trace_string = (icf_uint8_t *)"Precondition present in Require";
	p_glb_data->trace_data[ICF_TRACE_REQUIRE_PRECONDITION_PRESENT].trace_level
		= ICF_TRACE_LEVEL_DETAILED;
	p_glb_data->trace_data[ICF_TRACE_REQUIRE_PRECONDITION_PRESENT].trace_type
		= ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_NOTIFY_RCVD_N_CTXT_DELINKED].
         p_trace_string = (icf_uint8_t *)"Context delinked and NOTIFY received";
    p_glb_data->trace_data[ICF_TRACE_NOTIFY_RCVD_N_CTXT_DELINKED].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_NOTIFY_RCVD_N_CTXT_DELINKED].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_PHONE_CTXT_RECVD].
         p_trace_string = (icf_uint8_t *)"Phone context received";
    p_glb_data->trace_data[ICF_TRACE_PHONE_CTXT_RECVD].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_PHONE_CTXT_RECVD].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_MSG_TYPE_N_STATE_IN_OVLAP_TXN].
         p_trace_string = (icf_uint8_t *)"Message type and State in overlap trans";
    p_glb_data->trace_data[ICF_TRACE_MSG_TYPE_N_STATE_IN_OVLAP_TXN].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_MSG_TYPE_N_STATE_IN_OVLAP_TXN].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_TRACE_EXIT_PROCESS_MESG].
         p_trace_string = (icf_uint8_t *)"Exit process message";
    p_glb_data->trace_data[ICF_TRACE_EXIT_PROCESS_MESG].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_TRACE_EXIT_PROCESS_MESG].
        trace_type = ICF_TRACE_TYPE_SSA;

    p_glb_data->trace_data[ICF_SIP_STACK_BRIEF_TRACE].
         p_trace_string = (icf_uint8_t *)"[SDF]: ";
    p_glb_data->trace_data[ICF_SIP_STACK_BRIEF_TRACE].
        trace_level = ICF_TRACE_LEVEL_BRIEF;
    p_glb_data->trace_data[ICF_SIP_STACK_BRIEF_TRACE].
        trace_type = ICF_TRACE_TYPE_SIP_STACK;
    
    p_glb_data->trace_data[ICF_SIP_STACK_DETAILED_TRACE].
         p_trace_string = (icf_uint8_t *)"[SDF]: ";
    p_glb_data->trace_data[ICF_SIP_STACK_DETAILED_TRACE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SIP_STACK_DETAILED_TRACE].
        trace_type = ICF_TRACE_TYPE_SIP_STACK;
    
    p_glb_data->trace_data[ICF_SIP_STACK_DEBUG_TRACE].
         p_trace_string = (icf_uint8_t *)"[DEBUG]: ";
    p_glb_data->trace_data[ICF_SIP_STACK_DEBUG_TRACE].
        trace_level = ICF_TRACE_LEVEL_DETAILED;
    p_glb_data->trace_data[ICF_SIP_STACK_DEBUG_TRACE].
        trace_type = ICF_TRACE_TYPE_SIP_STACK_DEBUG;
#endif
    return ICF_SUCCESS;
}


/*******************************************************************************  
*   
*  FUNCTION NAME   :     icf_cmn_app_trace_init
*
*  DESCRIPTION     :    This function initializes the trace data in global data
*                       This function is called by MRM during initialization.
*
*  RETURNS         :    ICF_SUCCESS
*
*******************************************************************************/

icf_return_t icf_cmn_app_trace_init(
	INOUT icf_glb_data_st *p_glb_data)
{
  if(ICF_NULL == p_glb_data)
    {
          return ICF_FAILURE;
     }
#ifdef ICF_TRACE_ENABLE

    /*************************************************************************
     *  Trace IDs used by INTERFACE TRACES
     ************************************************************************/

    p_glb_data->app_trace_data[ICF_TRACE_SIP_PDU].trace_type =
        ICF_TRACE_TYPE_SIP_INTERFACE;
    p_glb_data->app_trace_data[ICF_TRACE_SIP_PDU].p_trace_string =
        (icf_uint8_t *)"SIP Message PDU";
    
    
    p_glb_data->app_trace_data[ICF_TRACE_APP_MESG].trace_type =
        ICF_TRACE_TYPE_APP_INTERFACE;
    p_glb_data->app_trace_data[ICF_TRACE_APP_MESG].p_trace_string =
        (icf_uint8_t *)"Application Message";

   
    p_glb_data->app_trace_data[ICF_TRACE_MEDIA_MESG].trace_type =
        ICF_TRACE_TYPE_MEDIA_INTERFACE;
    p_glb_data->app_trace_data[ICF_TRACE_MEDIA_MESG].p_trace_string =
        (icf_uint8_t *)"Media Message";

   
    /*************************************************************************
     *  Trace IDs used by CONFIGURATION TRACES
     ************************************************************************/

    p_glb_data->app_trace_data[ICF_TRACE_APP_ADD].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_APP_ADD].p_trace_string =
        (icf_uint8_t *)"Application Add Request Recvd";   

   
    p_glb_data->app_trace_data[ICF_TRACE_USER_INFO].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_USER_INFO].p_trace_string =
        (icf_uint8_t *)"User Configuration Info"; 

       
    p_glb_data->app_trace_data[ICF_TRACE_SELF_TRANSPORT_ADDR].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SELF_TRANSPORT_ADDR].p_trace_string =
        (icf_uint8_t *)"Self transport address configured";

       
    p_glb_data->app_trace_data[ICF_TRACE_PROXY_INFO].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_PROXY_INFO].p_trace_string =
        (icf_uint8_t *)"Proxy Transport Address configured";

    p_glb_data->app_trace_data[ICF_TRACE_SEC_PROXY_INFO].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SEC_PROXY_INFO].p_trace_string =
        (icf_uint8_t *)"Secondary Proxy Transport Address configured";
    

    p_glb_data->app_trace_data[ICF_TRACE_REGISTRAR_INFO].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REGISTRAR_INFO].p_trace_string =
        (icf_uint8_t *)"Registrar Transport Address configured";

    p_glb_data->app_trace_data[ICF_TRACE_SEC_REGISTRAR_INFO].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SEC_REGISTRAR_INFO].p_trace_string =
        (icf_uint8_t*)"Secondary Registrar Transport Address configured";

    p_glb_data->app_trace_data[ICF_TRACE_SYSTEM_RESTARTING].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SYSTEM_RESTARTING].p_trace_string =
        (icf_uint8_t *)"System Restart Req Recvd";


    p_glb_data->app_trace_data[ICF_TRACE_SYSTEM_REINITIALISING].trace_type =
        ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SYSTEM_REINITIALISING].p_trace_string =
        (icf_uint8_t *)"System ReInitialisation Req Recvd";

    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_PROXY_NOT_RESPONDING].
        trace_type = ICF_TRACE_TYPE_ERROR ;
    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_PROXY_NOT_RESPONDING].
         p_trace_string = (icf_uint8_t *)"Primary Proxy Server not responding";

    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_PROXY_NOT_RESPONDING].
        trace_type = ICF_TRACE_TYPE_ERROR ;
    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_PROXY_NOT_RESPONDING].
         p_trace_string = (icf_uint8_t *)"Secondary Proxy Server not responding";

    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_PROXY_SERVER_ACTIVE].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_PROXY_SERVER_ACTIVE].
         p_trace_string = (icf_uint8_t *)"Primary Proxy Server active";

    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_PROXY_SERVER_ACTIVE].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_PROXY_SERVER_ACTIVE].
         p_trace_string = (icf_uint8_t *)"Secondary Proxy Server active";

    p_glb_data->app_trace_data[ICF_TRACE_DHCP_SERVER_ALREADY_CONFIGURED].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_DHCP_SERVER_ALREADY_CONFIGURED].
         p_trace_string = (icf_uint8_t *)"DHCP Server already Configured";

    p_glb_data->app_trace_data[ICF_TRACE_DISCOVERED_PROXY_ADDRESS_FROM_DHCP].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_DISCOVERED_PROXY_ADDRESS_FROM_DHCP].
         p_trace_string = (icf_uint8_t *)"Discovered proxy addresses from DHCP Server";

    p_glb_data->app_trace_data[ICF_TRACE_PROXY_SERVER_ALREADY_CONFIGURED].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_PROXY_SERVER_ALREADY_CONFIGURED].
         p_trace_string = (icf_uint8_t *)"Proxy Server already Configured";


    p_glb_data->app_trace_data[ICF_TRACE_DHCP_FAILURE].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_DHCP_FAILURE].
         p_trace_string = (icf_uint8_t *)"DHCP Failure";

    p_glb_data->app_trace_data[ICF_TRACE_PCSCF_UNREACHABLE].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_PCSCF_UNREACHABLE].
         p_trace_string = (icf_uint8_t *)"Proxy Unreachable";

    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_DNS_SERVER_NOT_RESPONDING].
        trace_type = ICF_TRACE_TYPE_ERROR ;
    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_DNS_SERVER_NOT_RESPONDING].
         p_trace_string = (icf_uint8_t *)"Primary DNS Server Not Responding";

    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_DNS_SERVER_NOT_RESPONDING].
        trace_type = ICF_TRACE_TYPE_ERROR ;
    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_DNS_SERVER_NOT_RESPONDING].
         p_trace_string = (icf_uint8_t *)"Secondary DNS Server Not Responding";

    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_DNS_SERVER_ACTIVE].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_DNS_SERVER_ACTIVE].
         p_trace_string = (icf_uint8_t *)"Primary DNS Server active";

    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_DNS_SERVER_ACTIVE].
        trace_type = ICF_TRACE_TYPE_CONFIGURATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_DNS_SERVER_ACTIVE].
         p_trace_string = (icf_uint8_t *)"Secondary DNS Server active";


    /*************************************************************************
     *  Trace IDs used by REGISTRATION TRACES
     ************************************************************************/



    p_glb_data->app_trace_data[ICF_TRACE_REGISTARTION_REQ].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REGISTARTION_REQ].p_trace_string =
        (icf_uint8_t *)"Registration Req Recvd from App";


    p_glb_data->app_trace_data[ICF_TRACE_DEREG_REQ].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    /* Trace to specify the details of deregistration request either from
     * application or network initiated deregistration.*/
    p_glb_data->app_trace_data[ICF_TRACE_DEREG_REQ].p_trace_string =
        (icf_uint8_t *)"DeRegistration Req Recvd";


    p_glb_data->app_trace_data[ICF_TRACE_REG_RESP].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REG_RESP].p_trace_string =
        (icf_uint8_t *)"Registration Resp Sent to App";


    p_glb_data->app_trace_data[ICF_TRACE_REREG_REQ].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REREG_REQ].p_trace_string =
        (icf_uint8_t *)"ReRegistration sent to registrar";


    p_glb_data->app_trace_data[ICF_TRACE_REG_AUTH_401].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REG_AUTH_401].p_trace_string =
        (icf_uint8_t *)"Registration Challenged by 401";


    p_glb_data->app_trace_data[ICF_TRACE_REG_AUTH_407].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REG_AUTH_407].p_trace_string =
        (icf_uint8_t *)"Registration Challenged by 407";


    p_glb_data->app_trace_data[ICF_TRACE_REG_TIMER_EXPIRE].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REG_TIMER_EXPIRE].p_trace_string =
        (icf_uint8_t *)"Registration Timer Expires";

    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_REGISTRAR_NOT_RESPONDING].
        trace_type = ICF_TRACE_TYPE_ERROR ;
    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_REGISTRAR_NOT_RESPONDING].
         p_trace_string = (icf_uint8_t *)"Primary registrar not responding";

    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_REGISTRAR_NOT_RESPONDING].
        trace_type = ICF_TRACE_TYPE_ERROR ;
    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_REGISTRAR_NOT_RESPONDING].
         p_trace_string = (icf_uint8_t *)"Secondary registrar not responding";

    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_REGISTRAR_ACTIVE].
        trace_type = ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_PRIMARY_REGISTRAR_ACTIVE].
         p_trace_string = (icf_uint8_t *)"Primary Registrar active";

    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_REGISTRAR_ACTIVE].
        trace_type = ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_SECONDARY_REGISTRAR_ACTIVE].
         p_trace_string = (icf_uint8_t *)"Secondary Registrar active";

    /****************ICF 8.0***********************/
    /*Trace type for Reg-Event Package Subscription*/
    p_glb_data->app_trace_data[ICF_TRACE_REG_EVENT_SUBS_REQ].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REG_EVENT_SUBS_REQ].p_trace_string =
        (icf_uint8_t *)"Reg-Event Package Subscription Req";

     p_glb_data->app_trace_data[ICF_TRACE_REG_EVENT_SUBS_RESP].trace_type =
        ICF_TRACE_TYPE_REGISTRATION ;
    p_glb_data->app_trace_data[ICF_TRACE_REG_EVENT_SUBS_RESP].p_trace_string =
        (icf_uint8_t *)"Reg-Event Package Subscription Resp";

    p_glb_data->app_trace_data[ICF_TRACE_AKA_AUTH_FAIL].
        trace_type = ICF_TRACE_TYPE_ERROR; 
    p_glb_data->app_trace_data[ICF_TRACE_AKA_AUTH_FAIL].
         p_trace_string = (icf_uint8_t *)"AKA Authentication Response Failed.";

    p_glb_data->app_trace_data[ICF_TRACE_INVALID_AKA_AUTH_STATUS].
        trace_type = ICF_TRACE_TYPE_ERROR; 
    p_glb_data->app_trace_data[ICF_TRACE_INVALID_AKA_AUTH_STATUS].
         p_trace_string = (icf_uint8_t *)"Invalid AKA authentication status ";
   
    /* Trace for invalid assoc uri received in 200 ok respone to register
     * request*/
    p_glb_data->app_trace_data[ICF_TRACE_INVALID_ASSOC_URI].
        trace_type = ICF_TRACE_TYPE_ERROR; 
    p_glb_data->app_trace_data[ICF_TRACE_INVALID_ASSOC_URI].
         p_trace_string = (icf_uint8_t *)"Invalid Assoc Uri received ";

    /* Trace for SRTP Fallback invocation */
    p_glb_data->app_trace_data[ICF_TRACE_SRTP_FALLBACK_INVOCATION].
        trace_type = ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_SRTP_FALLBACK_INVOCATION].
         p_trace_string = (icf_uint8_t *)"SRTP Fallback invocation";

    
    p_glb_data->app_trace_data[ICF_TRACE_REG_AKA_AUTH_SUCCESS].
        trace_type = ICF_TRACE_TYPE_REGISTRATION; 
    p_glb_data->app_trace_data[ICF_TRACE_REG_AKA_AUTH_SUCCESS].
         p_trace_string = (icf_uint8_t *)"Registration AKA Authentication Response Success.";

    p_glb_data->app_trace_data[ICF_TRACE_REG_GUARD_TIMER_EXPIRY].
        trace_type = ICF_TRACE_TYPE_REGISTRATION; 
    p_glb_data->app_trace_data[ICF_TRACE_REG_GUARD_TIMER_EXPIRY].
         p_trace_string = (icf_uint8_t *)" Registration Guard Timer Expiry.";
    
    /*************************************************************************
     *  Trace IDs used by SUBSCRIBE TRACES
     ************************************************************************/



    p_glb_data->app_trace_data[ICF_TRACE_SUBSCRIPTION_REQ].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_SUBSCRIPTION_REQ].p_trace_string =
        (icf_uint8_t *)"Subscription Req Recvd from App";


    p_glb_data->app_trace_data[ICF_TRACE_SUBS_AUTH_401].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_SUBS_AUTH_401].p_trace_string =
        (icf_uint8_t *)"Subscription Challenged by 401";


    p_glb_data->app_trace_data[ICF_TRACE_SUBS_AUTH_407].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_SUBS_AUTH_407].p_trace_string =
        (icf_uint8_t *)"Subscription Challenged by 407";


    p_glb_data->app_trace_data[ICF_TRACE_UNSUBS_REQ].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_UNSUBS_REQ].p_trace_string =
        (icf_uint8_t *)"UnSubscription Req Recvd from App";


    p_glb_data->app_trace_data[ICF_TRACE_SUBS_RESP].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_SUBS_RESP].p_trace_string =
        (icf_uint8_t *)"Subscription Resp Sent to App";

    p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_FOR_SUBS].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_FOR_SUBS].p_trace_string =
        (icf_uint8_t *)"Notify for Subscription Recvd";

    p_glb_data->app_trace_data[ICF_TRACE_OTDLG_NOTIFY_IND].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_OTDLG_NOTIFY_IND].p_trace_string =
        (icf_uint8_t *)"Out of Dialog Notify Recvd";

    p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_CFM].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    /* CSR 1-6212448: Update trace description to indicate Notify cfm and nothing about
     * out-of-dialog */
    p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_CFM].p_trace_string =
        (icf_uint8_t *)"Notify Cfm Recvd from App";
        
    p_glb_data->app_trace_data[ICF_TRACE_RESUBS].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_RESUBS].p_trace_string =
        (icf_uint8_t *)"ReSubscription sent to nw.";

    p_glb_data->app_trace_data[ICF_TRACE_SUBS_TIMER_EXPIRE].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_SUBS_TIMER_EXPIRE].p_trace_string =
        (icf_uint8_t *)"Subscription Timer Expires";

    /* New trace for asynchronous NOTIFY support */
    p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_CFM_TIMER_EXPIRE].trace_type =
        ICF_TRACE_TYPE_SUBSCRIPTION ;
    p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_CFM_TIMER_EXPIRE].p_trace_string =
        (icf_uint8_t *)"Notify Cfm Timer Expires";


    /*************************************************************************
     *  Trace IDs used by OPTIONS TRACES
     ************************************************************************/



    p_glb_data->app_trace_data[ICF_TRACE_INC_OPTIONS].trace_type =
         ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_INC_OPTIONS].p_trace_string =
        (icf_uint8_t *)"Options Recvd from nw";


    p_glb_data->app_trace_data[ICF_TRACE_INC_OPTIONS_RESP].trace_type =
        ICF_TRACE_TYPE_CALL_CONTROL ;
    p_glb_data->app_trace_data[ICF_TRACE_INC_OPTIONS_RESP].p_trace_string =
        (icf_uint8_t *)"Options Resp from App";

    p_glb_data->app_trace_data[ICF_TRACE_OTG_OPTIONS].trace_type =
         ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_OTG_OPTIONS].p_trace_string =
        (icf_uint8_t *)"Options Recvd from App";

    p_glb_data->app_trace_data[ICF_TRACE_OTG_OPTIONS_RESP].trace_type =
         ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_OTG_OPTIONS_RESP].p_trace_string =
        (icf_uint8_t *)"Options Resp to App";

    p_glb_data->app_trace_data[ICF_TRACE_MEDIA_CAPABILITY].trace_type =
         ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_MEDIA_CAPABILITY].p_trace_string =
        (icf_uint8_t *)"Media Capability of Streams";

    p_glb_data->app_trace_data[ICF_TRACE_OPTIONS_TIMER_EXPIRE].trace_type =
         ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_OPTIONS_TIMER_EXPIRE].p_trace_string =
        (icf_uint8_t *)"Options Timer Expiry";



    /*************************************************************************
     *  Trace IDs used by INFO TRACE
     ************************************************************************/

    
    
    
     p_glb_data->app_trace_data[ICF_TRACE_INC_INFO].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_INC_INFO].p_trace_string =
            (icf_uint8_t *)"INFO Recvd from nw";
    
    
     p_glb_data->app_trace_data[ICF_TRACE_INC_INFO_RESP].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_INC_INFO_RESP].p_trace_string =
            (icf_uint8_t *)"INFO Resp from App";

    
     p_glb_data->app_trace_data[ICF_TRACE_OTG_INFO].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_OTG_INFO].p_trace_string =
            (icf_uint8_t *)"INFO Recvd from App";

    
     p_glb_data->app_trace_data[ICF_TRACE_OTG_INFO_RESP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_OTG_INFO_RESP].p_trace_string =
            (icf_uint8_t *)"INFO Resp to App";

    
     p_glb_data->app_trace_data[ICF_TRACE_INFO_TIMER_EXPIRE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_INFO_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"INFO Timer Expiry";



	/*************************************************************************
         *  Trace IDs used by 2 PARTY CALL TRACE
         ************************************************************************/



     p_glb_data->app_trace_data[ICF_TRACE_CALL_CONNECTED].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_CONNECTED].p_trace_string =
            (icf_uint8_t *)"Connected Call Info";


     p_glb_data->app_trace_data[ICF_TRACE_CREATE_CALL_REQ].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_CREATE_CALL_REQ].p_trace_string =
            (icf_uint8_t *)"Create Call Req from App";


     p_glb_data->app_trace_data[ICF_TRACE_INCOMING_CALL_IND].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_INCOMING_CALL_IND].p_trace_string =
            (icf_uint8_t *)"Incoming call from Remote";


     p_glb_data->app_trace_data[ICF_TRACE_INVITE_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_INVITE_SENT].p_trace_string =
            (icf_uint8_t *)"INVITE sent";


     p_glb_data->app_trace_data[ICF_TRACE_INVITE_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_INVITE_RECVD].p_trace_string =
            (icf_uint8_t *)"INVITE Recvd";

    
     p_glb_data->app_trace_data[ICF_TRACE_EARLY_INC_CALL_IND].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_EARLY_INC_CALL_IND].p_trace_string =
            (icf_uint8_t *)"Early Incoming Call Indication";


     p_glb_data->app_trace_data[ICF_TRACE_EARLY_INC_CALL_CFM].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_EARLY_INC_CALL_CFM].p_trace_string =
            (icf_uint8_t *)"Early Incoming Call Confirm";


     p_glb_data->app_trace_data[ICF_TRACE_LOCAL_SDP_OFFER].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_LOCAL_SDP_OFFER].p_trace_string =
            (icf_uint8_t *)"Local SDP Offer Info";


     p_glb_data->app_trace_data[ICF_TRACE_REMOTE_SDP_OFFER].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_REMOTE_SDP_OFFER].p_trace_string =
            (icf_uint8_t *)"Remote SDP Offer Info";


     p_glb_data->app_trace_data[ICF_TRACE_LOCAL_SDP_ANSWER].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_LOCAL_SDP_ANSWER].p_trace_string =
            (icf_uint8_t *)"Local SDP Answer Info";

  
     p_glb_data->app_trace_data[ICF_TRACE_REMOTE_SDP_ANSWER].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_REMOTE_SDP_ANSWER].p_trace_string =
            (icf_uint8_t *)"Remote SDP Answer Info";


     p_glb_data->app_trace_data[ICF_TRACE_CODEC_RESERVE].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_CODEC_RESERVE].p_trace_string =
            (icf_uint8_t *)"Codec Reserve Status";


     p_glb_data->app_trace_data[ICF_TRACE_RTP_ADDR_ALLOCATE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_RTP_ADDR_ALLOCATE].p_trace_string =
            (icf_uint8_t *)"RTP Address Allocation Status";


     p_glb_data->app_trace_data[ICF_TRACE_STREAM_CREATE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_STREAM_CREATE].p_trace_string =
            (icf_uint8_t *)"Stream Creation Status";


     p_glb_data->app_trace_data[ICF_TRACE_STREAM_MODIFY].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_STREAM_MODIFY].p_trace_string =
            (icf_uint8_t *)"Stream Modification Status";

    
     p_glb_data->app_trace_data[ICF_TRACE_CODEC_COMMIT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CODEC_COMMIT].p_trace_string =
            (icf_uint8_t *)"Codec Commit Status";


     p_glb_data->app_trace_data[ICF_TRACE_STREAM_DELETE].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_STREAM_DELETE].p_trace_string =
            (icf_uint8_t *)"Stream Deletion Status";


     p_glb_data->app_trace_data[ICF_TRACE_RTP_ADDR_DEALLOCATE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_RTP_ADDR_DEALLOCATE].p_trace_string =
            (icf_uint8_t *)"RTP Address DeAllocation Status";


     p_glb_data->app_trace_data[ICF_TRACE_CODEC_RELEASE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CODEC_RELEASE].p_trace_string =
            (icf_uint8_t *)"Codec Release Status";


     p_glb_data->app_trace_data[ICF_TRACE_ALERT_FROM_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_ALERT_FROM_APP].p_trace_string =
            (icf_uint8_t *)"Alert Req recvd from APP";
   

     p_glb_data->app_trace_data[ICF_TRACE_ALERT_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_ALERT_TO_APP].p_trace_string =
            (icf_uint8_t *)"Alert Req sent to APP";


     p_glb_data->app_trace_data[ICF_TRACE_MEDIA_CONNECT_FROM_APP].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_MEDIA_CONNECT_FROM_APP].p_trace_string =
            (icf_uint8_t *)"Media Connect Req recvd from APP";


     p_glb_data->app_trace_data[ICF_TRACE_MEDIA_CONNECT_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_MEDIA_CONNECT_TO_APP].p_trace_string =
            (icf_uint8_t *)"Media Connect Req sent to APP";


     p_glb_data->app_trace_data[ICF_TRACE_183_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_183_SENT].p_trace_string =
            (icf_uint8_t *)"183 Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_180_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_180_SENT].p_trace_string =
            (icf_uint8_t *)"180 Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_183_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_183_RECVD].p_trace_string =
            (icf_uint8_t *)"183 Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_180_RECVD].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_180_RECVD].p_trace_string =
            (icf_uint8_t *)"180 Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_100_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_100_SENT].p_trace_string =
            (icf_uint8_t *)"100 Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_100_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_100_RECVD].p_trace_string =
            (icf_uint8_t *)"100 Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_PROVISIONAL_RESP_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_PROVISIONAL_RESP_SENT].p_trace_string =
            (icf_uint8_t *)"Provisional Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_PROVISIONAL_RESP_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_PROVISIONAL_RESP_RECVD].p_trace_string =
            (icf_uint8_t *)"Provisional Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_200_SENT].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_200_SENT].p_trace_string =
            (icf_uint8_t *)"200 Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_200_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_200_RECVD].p_trace_string =
            (icf_uint8_t *)"200 Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_FAILURE_RESP_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_FAILURE_RESP_SENT].p_trace_string =
            (icf_uint8_t *)"Failure Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_FAILURE_RESP_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_FAILURE_RESP_RECVD].p_trace_string =
            (icf_uint8_t *)"Failure Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_202_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_202_SENT].p_trace_string =
            (icf_uint8_t *)"202 Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_202_RECVD].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_202_RECVD].p_trace_string =
            (icf_uint8_t *)"202 Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_PRACK_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_PRACK_RECVD].p_trace_string =
            (icf_uint8_t *)"PRACK Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_PRACK_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_PRACK_SENT].p_trace_string =
            (icf_uint8_t *)"PRACK Sent";


     p_glb_data->app_trace_data[ICF_TRACE_UPDATE_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_UPDATE_RECVD].p_trace_string =
            (icf_uint8_t *)"UPDATE Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_UPDATE_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_UPDATE_SENT].p_trace_string =
            (icf_uint8_t *)"UPDATE Sent";


     p_glb_data->app_trace_data[ICF_TRACE_REINVITE_RECVD].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_REINVITE_RECVD].p_trace_string =
            (icf_uint8_t *)"REINVITE Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_REINVITE_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_REINVITE_SENT].p_trace_string =
            (icf_uint8_t *)"REINVITE Sent";


     p_glb_data->app_trace_data[ICF_TRACE_OUT_OF_DIALOG_MESSAGE_RECVD].trace_type =
             ICF_TRACE_TYPE_MESSAGE_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_OUT_OF_DIALOG_MESSAGE_RECVD].p_trace_string =
            (icf_uint8_t *)"SIP Message Recvd out of dialog";


     p_glb_data->app_trace_data[ICF_TRACE_OUT_OF_DIALOG_MESSAGE_SENT].trace_type =
             ICF_TRACE_TYPE_MESSAGE_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_OUT_OF_DIALOG_MESSAGE_SENT].p_trace_string =
            (icf_uint8_t *)"SIP Message Sent out of dialog";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_TERMINATION_INITIATED].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_TERMINATION_INITIATED].p_trace_string =
            (icf_uint8_t *)"Call Termination Initiated";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_CANCELLATION_INITIATED].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_CANCELLATION_INITIATED].p_trace_string =
            (icf_uint8_t *)"Call Cancellation Initiated";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_IND].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_IND].p_trace_string =
            (icf_uint8_t *)"Call Modify Indication";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_REQ].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_REQ].p_trace_string =
            (icf_uint8_t *)"Call Modify Request";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_RESP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     /* CSR 1-6240518: String updated as per API document */     
     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_RESP].p_trace_string =
            (icf_uint8_t *)"Call Modify Response";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_CFM].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_MODIFY_CFM].p_trace_string =
            (icf_uint8_t *)"Call Modify Confirm";


     p_glb_data->app_trace_data[ICF_TRACE_BYE_SENT].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_BYE_SENT].p_trace_string =
            (icf_uint8_t *)"BYE Sent";


     p_glb_data->app_trace_data[ICF_TRACE_BYE_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_BYE_RECVD].p_trace_string =
            (icf_uint8_t *)"BYE Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_ACK_SENT].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_ACK_SENT].p_trace_string =
            (icf_uint8_t *)"ACK Sent";


     p_glb_data->app_trace_data[ICF_TRACE_ACK_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_ACK_RECVD].p_trace_string =
            (icf_uint8_t *)"ACK Recvd";

     p_glb_data->app_trace_data[ICF_TRACE_CANCEL_SENT].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_CANCEL_SENT].p_trace_string =
            (icf_uint8_t *)"CANCEL Sent";


     p_glb_data->app_trace_data[ICF_TRACE_CANCEL_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CANCEL_RECVD].p_trace_string =
            (icf_uint8_t *)"CANCEL Recvd";

     p_glb_data->app_trace_data[ICF_TRACE_SDP_RTP_AVP_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_SDP_RTP_AVP_RECVD].p_trace_string =
            (icf_uint8_t *)"SDP RTP/AVP Received";

     p_glb_data->app_trace_data[ICF_TRACE_SDP_RTP_SAVP_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_SDP_RTP_SAVP_RECVD].p_trace_string =
            (icf_uint8_t *)"SDP RTP/SAVP Received";

    
        /*************************************************************************
         *  Trace IDs used by FAX TRACES
         ************************************************************************/


     p_glb_data->app_trace_data[ICF_TRACE_START_FAX_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_START_FAX_TO_APP].p_trace_string =
            (icf_uint8_t *)"Start Fax Indication Sent to APP";


     p_glb_data->app_trace_data[ICF_TRACE_START_FAX_FROM_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_START_FAX_FROM_APP].p_trace_string =
            (icf_uint8_t *)"Start Fax Request Recvd from APP";


     p_glb_data->app_trace_data[ICF_TRACE_START_FAX_RESP_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_START_FAX_RESP_TO_APP].p_trace_string =
            (icf_uint8_t *)"Start Fax Response Sent to APP";


     p_glb_data->app_trace_data[ICF_TRACE_STOP_FAX_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_STOP_FAX_TO_APP].p_trace_string =
            (icf_uint8_t *)"Stop Fax Indication Sent to APP";


     p_glb_data->app_trace_data[ICF_TRACE_STOP_FAX_FROM_APP].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_STOP_FAX_FROM_APP].p_trace_string =
            (icf_uint8_t *)"Stop Fax Request Recvd from APP";


     p_glb_data->app_trace_data[ICF_TRACE_STOP_FAX_RESP_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_STOP_FAX_RESP_TO_APP].p_trace_string =
            (icf_uint8_t *)"Stop Fax Response Sent to APP";




        /*************************************************************************
         *  Trace IDs used by SERVICE TRACES
         ************************************************************************/


     p_glb_data->app_trace_data[ICF_TRACE_STREAM_MERGE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_STREAM_MERGE].p_trace_string =
            (icf_uint8_t *)"Merge Media Status";


     p_glb_data->app_trace_data[ICF_TRACE_CONFERENCE_REQ].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CONFERENCE_REQ].p_trace_string =
            (icf_uint8_t *)"Conference Req from APP";


     p_glb_data->app_trace_data[ICF_TRACE_PARTY_REPLACED_IND].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_PARTY_REPLACED_IND].p_trace_string =
            (icf_uint8_t *)"Party Replaced Indication to APP";


     p_glb_data->app_trace_data[ICF_TRACE_REFER_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_REFER_RECVD].p_trace_string =
            (icf_uint8_t *)"REFER Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_REFER_SENT].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_REFER_SENT].p_trace_string =
            (icf_uint8_t *)"REFER Sent";


     p_glb_data->app_trace_data[ICF_TRACE_CTA_REQ].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CTA_REQ].p_trace_string =
            (icf_uint8_t *)"Call Transfer Attended Req from APP";


     p_glb_data->app_trace_data[ICF_TRACE_CTU_REQ].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CTU_REQ].p_trace_string =
            (icf_uint8_t *)"Call Transfer UnAttended Req from APP";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_HOLD_REQ].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_HOLD_REQ].p_trace_string =
            (icf_uint8_t *)"Call Hold Req from APP";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_RESUME_REQ].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_RESUME_REQ].p_trace_string =
            (icf_uint8_t *)"Call Resume Req from APP";


     p_glb_data->app_trace_data[ICF_TRACE_CTA_RESP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CTA_RESP].p_trace_string =
            (icf_uint8_t *)"Call Transfer Attended Resp to APP";


     p_glb_data->app_trace_data[ICF_TRACE_CTU_RESP].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_CTU_RESP].p_trace_string =
            (icf_uint8_t *)"Call Transfer UnAttended Resp to APP";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_HOLD_RESP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_HOLD_RESP].p_trace_string =
            (icf_uint8_t *)"Call Hold Resp to APP";

     p_glb_data->app_trace_data[ICF_TRACE_CALL_RESUME_RESP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_RESUME_RESP].p_trace_string =
            (icf_uint8_t *)"Call Resume Resp to APP";


     p_glb_data->app_trace_data[ICF_TRACE_REMOTE_CALL_TRANSFER_CFM].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_REMOTE_CALL_TRANSFER_CFM].p_trace_string =
            (icf_uint8_t *)"Call Transfer Confirm from APP";


     p_glb_data->app_trace_data[ICF_TRACE_302_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_302_SENT].p_trace_string =
            (icf_uint8_t *)"302 Response Sent";


     p_glb_data->app_trace_data[ICF_TRACE_302_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_302_RECVD].p_trace_string =
            (icf_uint8_t *)"302 Response Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_CONFERENCE_RESP].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_CONFERENCE_RESP].p_trace_string =
            (icf_uint8_t *)"Conference Resp to APP";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_INITIATED_FOR_INC_REFER].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_INITIATED_FOR_INC_REFER].p_trace_string =
            (icf_uint8_t *)"Incoming REFER initiated Call";


     p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_FOR_REFER_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_FOR_REFER_RECVD].p_trace_string =
            (icf_uint8_t *)"Notify for Refer Recvd";


     p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_FOR_REFER_SENT].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_NOTIFY_FOR_REFER_SENT].p_trace_string =
            (icf_uint8_t *)"Notify for Refer Sent";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_WAITING_IND].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_WAITING_IND].p_trace_string =
            (icf_uint8_t *)"Call Waiting Indication to APP.";



        /************************************************************************
	 *Trace IDs used by ERROR TRACES
         ************************************************************************/


     p_glb_data->app_trace_data[ICF_TRACE_SIP_MSG_RETRANSMISSION_TIMEOUT].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_SIP_MSG_RETRANSMISSION_TIMEOUT].p_trace_string =
            (icf_uint8_t *)"SIP Message Retransmission Error";


     p_glb_data->app_trace_data[ICF_TRACE_DNS_QUERY_FAILURE].trace_type =
           ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_DNS_QUERY_FAILURE].p_trace_string =
            (icf_uint8_t *)"DNS Query Error";


     p_glb_data->app_trace_data[ICF_TRACE_STACK_ERROR].trace_type =
            ICF_TRACE_TYPE_ERROR ;
     p_glb_data->app_trace_data[ICF_TRACE_STACK_ERROR].p_trace_string =
            (icf_uint8_t *)"Stack Error";


     p_glb_data->app_trace_data[ICF_TRACE_SOCKET_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_SOCKET_ERROR].p_trace_string =
            (icf_uint8_t *)"Socket Operation Error";


     p_glb_data->app_trace_data[ICF_TRACE_UDP_SEND_ERROR].trace_type =
            ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_UDP_SEND_ERROR].p_trace_string =
            (icf_uint8_t *)"UDP Send error";


     p_glb_data->app_trace_data[ICF_TRACE_UDP_RECV_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_UDP_RECV_ERROR].p_trace_string =
            (icf_uint8_t *)"UDP Recv Error";


     p_glb_data->app_trace_data[ICF_TRACE_TCP_SEND_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_TCP_SEND_ERROR].p_trace_string =
            (icf_uint8_t *)"TCP Send error";


     p_glb_data->app_trace_data[ICF_TRACE_TCP_RECV_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_TCP_RECV_ERROR].p_trace_string =
            (icf_uint8_t *)"TCP Recv Error";


     p_glb_data->app_trace_data[ICF_TRACE_TLS_SEND_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_TLS_SEND_ERROR].p_trace_string =
            (icf_uint8_t *)"TLS Send error";


     p_glb_data->app_trace_data[ICF_TRACE_TLS_RECV_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_TLS_RECV_ERROR].p_trace_string =
            (icf_uint8_t *)"TLS Recv Error";


     p_glb_data->app_trace_data[ICF_TRACE_MEMGET_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_MEMGET_ERROR].p_trace_string =
            (icf_uint8_t *)"Memory Allocation Error";


     p_glb_data->app_trace_data[ICF_TRACE_MEMFREE_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_MEMFREE_ERROR].p_trace_string =
            (icf_uint8_t *)"Memory Free Error";


     p_glb_data->app_trace_data[ICF_TRACE_START_TIMER_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_START_TIMER_ERROR].p_trace_string =
            (icf_uint8_t *)"Start Timer Failure";


     p_glb_data->app_trace_data[ICF_TRACE_STOP_TIMER_ERROR].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_STOP_TIMER_ERROR].p_trace_string =
            (icf_uint8_t *)"Stop Timer Failure";


     p_glb_data->app_trace_data[ICF_TRACE_DMS_IND].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_DMS_IND].p_trace_string =
            (icf_uint8_t *)"Media Failure";

     p_glb_data->app_trace_data[ICF_TRACE_SRTP_NEG_FAILURE].trace_type =
             ICF_TRACE_TYPE_ERROR;
     p_glb_data->app_trace_data[ICF_TRACE_SRTP_NEG_FAILURE].p_trace_string =
            (icf_uint8_t *)"SRTP negotiation failed";





        /************************************************************************
         *Trace IDs used by SYSTEM TRACES
         ************************************************************************/


     p_glb_data->app_trace_data[ICF_TRACE_TCP_CONNECTION_OPEN].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TCP_CONNECTION_OPEN].p_trace_string =
            (icf_uint8_t *)"TCP Connection Opened";


     p_glb_data->app_trace_data[ICF_TRACE_TCP_CONNECTION_CLOSE].trace_type =
           ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TCP_CONNECTION_CLOSE].p_trace_string =
            (icf_uint8_t *)"TCP Connection Closed";


     p_glb_data->app_trace_data[ICF_TRACE_TLS_CONNECTION_OPEN].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TLS_CONNECTION_OPEN].p_trace_string =
            (icf_uint8_t *)"TLS Connection Opened";


     p_glb_data->app_trace_data[ICF_TRACE_TLS_CONNECTION_CLOSE].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TLS_CONNECTION_CLOSE].p_trace_string =
            (icf_uint8_t *)"TLS Connection Closed";


     p_glb_data->app_trace_data[ICF_TRACE_UDP_SERVER_OPEN].trace_type =
            ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_UDP_SERVER_OPEN].p_trace_string =
            (icf_uint8_t *)"UDP Server Opened";


     p_glb_data->app_trace_data[ICF_TRACE_UDP_SERVER_CLOSE].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_UDP_SERVER_CLOSE].p_trace_string =
            (icf_uint8_t *)"UDP Server Closed";


     p_glb_data->app_trace_data[ICF_TRACE_TCP_SERVER_OPEN].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TCP_SERVER_OPEN].p_trace_string =
            (icf_uint8_t *)"TCP Server Opened";


     p_glb_data->app_trace_data[ICF_TRACE_TCP_SERVER_CLOSE].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TCP_SERVER_CLOSE].p_trace_string =
            (icf_uint8_t *)"TCP Server Closed";


     p_glb_data->app_trace_data[ICF_TRACE_TLS_SERVER_OPEN].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TLS_SERVER_OPEN].p_trace_string =
            (icf_uint8_t *)"TLS Server Opened";


     p_glb_data->app_trace_data[ICF_TRACE_TLS_SERVER_CLOSE].trace_type =
             ICF_TRACE_TYPE_SYSTEM;
     p_glb_data->app_trace_data[ICF_TRACE_TLS_SERVER_CLOSE].p_trace_string =
            (icf_uint8_t *)"TLS Server Closed";

        

        /************************************************************************
         *Trace IDs used by TIMER TRACES
         ************************************************************************/


     p_glb_data->app_trace_data[ICF_TRACE_SETUP_TIMER_EXPIRE].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL ;
     p_glb_data->app_trace_data[ICF_TRACE_SETUP_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"Call Setup Timer Expires";


     p_glb_data->app_trace_data[ICF_TRACE_ALERTING_TIMER_EXPIRE].trace_type =
           ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_ALERTING_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"Call Alerting Timer Expires";


     p_glb_data->app_trace_data[ICF_TRACE_RELEASE_TIMER_EXPIRE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_RELEASE_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"Call Release Timer Expires";


     p_glb_data->app_trace_data[ICF_TRACE_MEDIA_TIMER_EXPIRE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_MEDIA_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"Media Timer Expire";


     p_glb_data->app_trace_data[ICF_TRACE_CALL_HOLD_RESUME_TIMER_EXPIRE].trace_type =
            ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CALL_HOLD_RESUME_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"Call Hold Resume Timer Expires";


     p_glb_data->app_trace_data[ICF_TRACE_CTU_TIMER_EXPIRE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CTU_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"Call Transger UnAttended Timer Expires";


     p_glb_data->app_trace_data[ICF_TRACE_CTA_TIMER_EXPIRE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CTA_TIMER_EXPIRE].p_trace_string =
           (icf_uint8_t *)"Call Transger Attended Timer Expires";


     p_glb_data->app_trace_data[ICF_TRACE_CONF_TIMER_EXPIRE].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_CONF_TIMER_EXPIRE].p_trace_string =
            (icf_uint8_t *)"Conference Timer Expires";

     /*************************************************************************
     *  Trace IDs used by PUBLISH/MESSAGE TRACES
     ************************************************************************/
    
    p_glb_data->app_trace_data[ICF_TRACE_OTG_PUBLISH].trace_type =
         ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_OTG_PUBLISH].p_trace_string =
        (icf_uint8_t *)"Publish Recvd From App";


    p_glb_data->app_trace_data[ICF_TRACE_OTG_PUBLISH_RESP].trace_type =
        ICF_TRACE_TYPE_CALL_CONTROL ;
    p_glb_data->app_trace_data[ICF_TRACE_OTG_PUBLISH_RESP].p_trace_string =
        (icf_uint8_t *)"Publish Resp to App";

    p_glb_data->app_trace_data[ICF_TRACE_OTG_MESSAGE].trace_type =
         ICF_TRACE_TYPE_MESSAGE_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_OTG_MESSAGE].p_trace_string =
        (icf_uint8_t *)"Message Recvd from App";

    p_glb_data->app_trace_data[ICF_TRACE_OTG_MESSAGE_RESP].trace_type =
         ICF_TRACE_TYPE_MESSAGE_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_OTG_MESSAGE_RESP].p_trace_string =
        (icf_uint8_t *)"Message Resp to App";

    p_glb_data->app_trace_data[ICF_TRACE_INC_MESSAGE].trace_type =
         ICF_TRACE_TYPE_MESSAGE_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_INC_MESSAGE].p_trace_string =
        (icf_uint8_t *)"Message Recvd from NW";

    /* Rel 8.1
     * TRACE initialization for OPTIONS 
     */
    p_glb_data->app_trace_data[ICF_TRACE_EARLY_SESSION_IND].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
     p_glb_data->app_trace_data[ICF_TRACE_EARLY_SESSION_IND].p_trace_string =
            (icf_uint8_t *)"Early Session Ind"; 

    p_glb_data->app_trace_data[ICF_TRACE_EARLY_SESSION_CFM].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_EARLY_SESSION_CFM].p_trace_string =
            (icf_uint8_t *)"Early Session Cfm";

    /* SDP Tunnelling : Traces for new MM APIs */
    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_CMS_REQ_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_CMS_REQ_SENT].p_trace_string =
            (icf_uint8_t *)"Tunnelled CMS Request";

    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_CMS_RESP_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_CMS_RESP_RECVD].p_trace_string =
            (icf_uint8_t *)"Tunnelled CMS Response";

    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_MMS_RESP_RECVD].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_MMS_RESP_RECVD].p_trace_string =
            (icf_uint8_t *)"Tunnelled MMS Response";

    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_MMS_REQ_SENT].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_TUNNELLED_MMS_REQ_SENT].p_trace_string =
            (icf_uint8_t *)"Tunnelled MMS Request";

    /*  Rel 8.2 changes Start */
    p_glb_data->app_trace_data[ICF_TRACE_PRACK_REQ_RECV_IND_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_PRACK_REQ_RECV_IND_TO_APP].p_trace_string =
             (icf_uint8_t *)"PRACK Request Received";

    p_glb_data->app_trace_data[ICF_TRACE_PRACK_RESP_SENT_IND_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_PRACK_RESP_SENT_IND_TO_APP].p_trace_string =
             (icf_uint8_t *)"PRACK Response Sent";

    p_glb_data->app_trace_data[ICF_TRACE_PRACK_REQ_SENT_IND_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_PRACK_REQ_SENT_IND_TO_APP].p_trace_string =
             (icf_uint8_t *)"PRACK Request Sent";

    p_glb_data->app_trace_data[ICF_TRACE_PRACK_RESP_RECV_IND_TO_APP].trace_type =
             ICF_TRACE_TYPE_CALL_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_PRACK_RESP_RECV_IND_TO_APP].p_trace_string =
             (icf_uint8_t *)"PRACK Response Received";
    /*  Rel 8.2 changes End */

    /*Asynchronus MESSAGE enhancements*/
    p_glb_data->app_trace_data[ICF_TRACE_MESSAGE_CFM_TIMER_EXPIRE].trace_type =
         ICF_TRACE_TYPE_MESSAGE_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_MESSAGE_CFM_TIMER_EXPIRE].p_trace_string =
        (icf_uint8_t *)"Message Cfm Timer Expires";

    p_glb_data->app_trace_data[ICF_TRACE_MESSAGE_CFM].trace_type =
        ICF_TRACE_TYPE_MESSAGE_CONTROL;
    p_glb_data->app_trace_data[ICF_TRACE_MESSAGE_CFM].p_trace_string =
        (icf_uint8_t *)"Message Cfm Recvd from App";


#endif
  return ICF_SUCCESS;

}

/*****************************************************************************
 ** INTERFACE TRACES START
 *****************************************************************************/
#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_cmn_trace_api_from_api_id 
 * Description :  identify the API and calls the appropriate trace handler
 *
 *----------------------------------------------------------------------------*/

icf_return_t icf_cmn_trace_api_from_api_id(	
	 icf_glb_pdb_st *p_glb_pdb,
	 icf_msg_st *p_msg,
         icf_error_t *p_ecode)
{
        icf_return_t ret_val = ICF_SUCCESS;
        icf_uint8_t *p_sprintf_count = ICF_NULL;
        icf_sm_glb_data_st  *p_sm_data =  ICF_NULL;

    /* In tunnel mode, we will not print interface traces for DMS request and response */
ICF_CHECK_TUNNEL_MODE_START(p_glb_pdb,ICF_TRUE) 
    if((ICF_DELETE_MEDIA_SESSION_REQ == p_msg->hdr.api_id) ||
       (ICF_DELETE_MEDIA_SESSION_RESP == p_msg->hdr.api_id))
    {
        return ICF_SUCCESS;
    }
ICF_CHECK_TUNNEL_MODE_END

    /* preliminary check - whether traces are enabled */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_SM),
                (icf_void_t *)&p_sm_data,
                p_ecode))
    {
        return ICF_FAILURE;
    }
    else if (ICF_FALSE == p_sm_data->trace_status)
    {
        return ICF_FAILURE;
    }
    /* Rel 8.2 Klocworks fix */
          ICF_MEMGET(
               p_glb_pdb,
               ICF_CMN_MAX_API_LENGTH,        
               ICF_MEM_COMMON,
               p_sprintf_count/* p_glb_pdb->p_interface_trace_info */,
               ICF_RET_ON_FAILURE,
               p_ecode,
               ret_val)
    /* Rel 8.2 Klocworks fix end */
	/* p_sprintf_count = */ p_glb_pdb->p_interface_trace_info = p_sprintf_count;
   ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_sprintf_count) 

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n{\n"));
	switch(p_msg->hdr.api_id)
	{
	case ICF_CFG_CC_SET_SCALE_PARAMS_REQ:
		p_sprintf_count=icf_cmn_trace_set_scale_params(p_msg,p_sprintf_count);
	        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ:
		p_sprintf_count=icf_cmn_trace_set_trans_params(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CFG_CC_SET_CALL_PARAMS_REQ:
		p_sprintf_count=icf_cmn_trace_set_call_params(p_msg,p_sprintf_count);
                 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ:
		p_sprintf_count=icf_cmn_trace_set_self_id_params(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ:
		p_sprintf_count=icf_cmn_trace_set_reg_params(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CFG_CC_SET_SERVICE_PARAMS_REQ:
		p_sprintf_count=icf_cmn_trace_set_service_params(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CFG_CC_INIT_COMPLETE_REQ:
		p_sprintf_count=icf_cmn_trace_no_payload_msg(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CC_CFG_SET_CONFIG_PARAM_RESP:
		p_sprintf_count=icf_cmn_trace_config_params_resp(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_HOLD_REQ:
		p_sprintf_count=icf_cmn_trace_chd_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_HOLD_RESP:
		p_sprintf_count=icf_cmn_trace_api_resp_msg(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_RESUME_REQ:
		p_sprintf_count=icf_cmn_trace_chd_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_RESUME_RESP:
		p_sprintf_count=icf_cmn_trace_api_resp_msg(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

        case ICF_EARLY_INC_CALL_IND:
		p_sprintf_count=icf_cmn_trace_early_inc_call_ind(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_EARLY_INC_CALL_CFM:
		p_sprintf_count=icf_cmn_trace_early_inc_call_cfm(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_CALL_REDIRECTION_IND:
		p_sprintf_count=icf_cmn_trace_call_redirection_ind(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CONFERENCE_REQ:
		p_sprintf_count=icf_cmn_trace_conference_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CONFERENCE_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case  ICF_CALL_TRANSFER_UNATTENDED_REQ:
		p_sprintf_count=icf_cmn_trace_ctu_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_TRANSFER_UNATTENDED_RESP:
		p_sprintf_count=icf_cmn_trace_ctu_resp(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_TRANSFER_ATTENDED_REQ:
		p_sprintf_count=icf_cmn_trace_cta_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_TRANSFER_ATTENDED_RESP:
		p_sprintf_count=icf_cmn_trace_cta_resp(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_HOLD_IND:
		p_sprintf_count=icf_cmn_trace_api_ind(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_RESUME_IND:
		p_sprintf_count=icf_cmn_trace_api_ind(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_REMOTE_CALL_TRANSFER_INITIATED_IND:
		p_sprintf_count=icf_cmn_trace_rem_call_transfer_init(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_MERGED_IND:
		p_sprintf_count=icf_cmn_trace_call_merged_ind(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case  ICF_GET_LINE_SETTINGS_REQ:
		p_sprintf_count=icf_cmn_trace_get_line_settings(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_GET_LINE_SETTINGS_RESP:
		p_sprintf_count=icf_cmn_trace_get_line_settings_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_GET_SYSTEM_SETTINGS_REQ:
		p_sprintf_count=icf_cmn_trace_get_sys_settings(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_GET_SYSTEM_SETTINGS_RESP:
		p_sprintf_count=icf_cmn_trace_get_sys_settings_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_APP_ADD_REQ:
		p_sprintf_count=icf_cmn_trace_app_add_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_APP_ADD_CFM:
		p_sprintf_count=icf_cmn_trace_app_add_cfm(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_APP_REMOVE_REQ:
		p_sprintf_count=icf_cmn_trace_app_remove_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_APP_REMOVE_CFM:
		p_sprintf_count=icf_cmn_trace_app_remove_cfm(p_msg,p_sprintf_count);
 		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

       	case ICF_REGISTER_REQ:
       		p_sprintf_count=icf_cmn_trace_register_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
    		break;

	case ICF_REGISTER_CFM:
       		p_sprintf_count=icf_cmn_trace_register_cfm(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

    case ICF_REGISTER_STATUS_IND:
       		p_sprintf_count=icf_cmn_trace_register_status_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_AKA_AUTH_RESP:
       		p_sprintf_count = icf_cmn_trace_aka_auth_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;
    
    case ICF_SUBSCRIBE_REQ:
       		p_sprintf_count=icf_cmn_trace_subscribe_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;
	case ICF_SUBSCRIPTION_STATUS_IND:
       		p_sprintf_count=icf_cmn_trace_subscribe_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

    	case ICF_REFER_REQ:
        	p_sprintf_count=icf_cmn_trace_refer_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

        case ICF_REFER_RESP:
        	p_sprintf_count=icf_cmn_trace_refer_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

       	case ICF_NOTIFY_IND:
        	p_sprintf_count=icf_cmn_trace_notify_ind(p_msg,p_sprintf_count);
   	 	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

	case ICF_PUBLISH_REQ:
        	p_sprintf_count=icf_cmn_trace_publish_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

    	case ICF_PUBLISH_RESP:
        	p_sprintf_count=icf_cmn_trace_publish_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break; 

        case ICF_MESSAGE_REQ:
        	p_sprintf_count=icf_cmn_trace_message_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

    	case ICF_MESSAGE_RESP:
        	p_sprintf_count=icf_cmn_trace_message_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

    	case ICF_MESSAGE_IND:
        	p_sprintf_count=icf_cmn_trace_message_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;
/*
	case ICF_CFG_CODEC_INFO_REQ:
		p_sprintf_count=icf_cmn_trace_codec_info_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
                break;

	case ICF_CFG_RINGER_TONES_REQ:
		p_sprintf_count=icf_cmn_trace_ringer_tones_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;

	case ICF_CFG_JBM_MGMT_REQ:
		p_sprintf_count=icf_cmn_trace_jbm_mgmt_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;

	case ICF_CFG_MAX_EARP_VOL_REQ:
		p_sprintf_count=icf_cmn_trace_max_earp_vol_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
		p_sprintf_count=icf_cmn_trace_max_speaker_vol_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;

	case ICF_CFG_SET_PORT_RANGE_REQ:
		p_sprintf_count=icf_cmn_trace_port_range_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;


	case ICF_CFG_SET_SELF_IP_REQ:
		p_sprintf_count=icf_cmn_trace_self_ip_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG,p_msg)
 		break;

	case ICF_CFG_SET_SELF_IP_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;

	case ICF_CFG_SET_DIGIT_TRANS_REQ:
		p_sprintf_count=icf_cmn_trace_digit_trans_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_SET_DIGIT_TRANS_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
		p_sprintf_count=icf_cmn_trace_remote_video_diaplay_req(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
		p_sprintf_count=icf_cmn_trace_preview_video_diaplay_req(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_MEDIA_REQ:
		p_sprintf_count=icf_cmn_trace_cfg_media_req(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_MEDIA_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_PLAY_MEDIA_REQ:
		p_sprintf_count=icf_cmn_trace_play_media_req(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_PLAY_MEDIA_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_MEDIA_PLAY_REQ:
		p_sprintf_count=icf_cmn_trace_stop_media_play_req(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_MEDIA_PLAY_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_MEDIA_PLAY_IND:
		p_sprintf_count=icf_cmn_trace_stop_media_play_ind(p_msg,p_sprintf_count);     
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SEND_DGTS_REQ:
		p_sprintf_count=icf_cmn_trace_send_dgts_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_START_RINGING_REQ:
		p_sprintf_count=icf_cmn_trace_start_ringing_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_GET_STATS_REQ:
		p_sprintf_count=icf_cmn_trace_get_stats_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_RINGING_REQ:
		p_sprintf_count=icf_cmn_trace_stop_ringing_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SET_EARP_VOLUME_REQ:
		p_sprintf_count=icf_cmn_trace_set_earp_volume_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SET_SPEAKER_VOLUME_REQ:
		p_sprintf_count=icf_cmn_trace_set_speaker_volume_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SEND_APP_DATA_REQ:
		p_sprintf_count=icf_cmn_trace_send_app_data_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break;
	case ICF_SEND_STATS_REQ:
		p_sprintf_count=icf_cmn_trace_send_stats_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)
		break;
    	case ICF_STOP_STATS_REQ:
		p_sprintf_count=icf_cmn_trace_stop_stats_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)
		break;
	case ICF_STOP_RINGING_IND:
		p_sprintf_count=icf_cmn_trace_stop_ringing_ind(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;
	case ICF_RCV_APP_DATA_IND:
		p_sprintf_count=icf_cmn_trace_rcv_app_data_ind(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)
		break;
	case ICF_CFG_CODEC_INFO_RESP:
		p_sprintf_count=icf_cmn_trace_codec_info_resp(p_msg,p_sprintf_count);
   	        p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_RINGER_TONES_RESP:
		p_sprintf_count=icf_cmn_trace_ringer_tones_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_JBM_MGMT_RESP:
		p_sprintf_count=icf_cmn_trace_jbm_mgmt_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_MAX_EARP_VOL_RESP:
		p_sprintf_count=icf_cmn_trace_max_earp_vol_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
		p_sprintf_count=icf_cmn_trace_max_speaker_vol_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_CFG_SET_PORT_RANGE_RESP:
		p_sprintf_count=icf_cmn_trace_set_port_range_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_MEDIA_SESSION_RESP:
		p_sprintf_count=icf_cmn_trace_media_session_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
    	case ICF_RTP_START_IND:
		p_sprintf_count=icf_cmn_trace_rtp_start_ind(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_GET_STATS_RESP:
		p_sprintf_count=icf_cmn_trace_get_stats_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;

	case ICF_START_RINGING_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_VIBRATION_REQ:
		p_sprintf_count=icf_cmn_trace_vibration_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_VIBRATION_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_VIBRATION_REQ:
		p_sprintf_count=icf_cmn_trace_stop_vibration_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_VIBRATION_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_ECHO_CANC_REQ:
		p_sprintf_count=icf_cmn_trace_echo_canc_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_ECHO_CANC_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_VAD_SS_CNG_REQ:
		p_sprintf_count=icf_cmn_trace_vad_ss_cng_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_VAD_SS_CNG_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_RINGING_RESP:
		p_sprintf_count=icf_cmn_trace_stop_ringing_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_MUTE_MEDIA_RESP:
		p_sprintf_count=icf_cmn_trace_mute_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SET_EARP_VOLUME_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SET_SPEAKER_VOLUME_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SEND_APP_DATA_RESP:
		p_sprintf_count=icf_cmn_trace_send_app_data_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break;
	case ICF_SEND_STATS_RESP:
		p_sprintf_count=icf_cmn_trace_send_stats_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break;
	case ICF_STOP_STATS_RESP:
		p_sprintf_count=icf_cmn_trace_stop_stats_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break;
*/

    	case ICF_INCOMING_CALL_IND:  
       		p_sprintf_count=icf_cmn_trace_incoming_call(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

    	case ICF_CREATE_CALL_REQ: 
       		p_sprintf_count=icf_cmn_trace_create_call(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

  	case ICF_REMOTE_USER_ALERTED_IND: 
       		p_sprintf_count=icf_cmn_trace_alert_user(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

    	case ICF_LOCAL_USER_ALERTED_REQ: 
       		p_sprintf_count=icf_cmn_trace_user_alerted(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;  

	case ICF_MEDIA_CONNECTED_IND:  
       		p_sprintf_count=icf_cmn_trace_media_connected(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;  

	case ICF_CONNECT_IND:
       	 	p_sprintf_count=icf_cmn_trace_connect_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;
/*
	case ICF_CONNECT_MEDIA_DEVICE_REQ: 
       		p_sprintf_count=icf_cmn_trace_connect_media_device_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break; 
	case ICF_CONNECT_MEDIA_DEVICE_RESP: 
       		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
   	 	p_sprintf_count += ICF_SPRINTF((p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break; 
*/

	case ICF_INCOMING_CALL_MODIFY_IND:
       		p_sprintf_count=icf_cmn_trace_incoming_call_modify_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_MODIFY_CFM:
        	p_sprintf_count=icf_cmn_trace_call_modify_cfm(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND:
       		p_sprintf_count=icf_cmn_trace_incoming_call_modify_cancel_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_MODIFY_REQ:
       		p_sprintf_count=icf_cmn_trace_call_modify_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CALL_MODIFY_RESP:
       		p_sprintf_count=icf_cmn_trace_call_modify_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CONNECT_REQ:
       		p_sprintf_count=icf_cmn_trace_connect_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_CONNECT_RESP:
       		p_sprintf_count=icf_cmn_trace_connect_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_REMOTE_USER_TERMINATE_CALL_RESP: 
       		p_sprintf_count=icf_cmn_trace_no_payload_msg(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_LOCAL_USER_TERMINATE_CALL_RESP: 
       		p_sprintf_count=icf_cmn_trace_no_payload_msg(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

    	case ICF_TERMINATE_CALL_IND: 
       		p_sprintf_count=icf_cmn_trace_term_call_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_TERMINATE_CALL_REQ: 
       		p_sprintf_count=icf_cmn_trace_term_call_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

    	case ICF_GET_CODEC_LIST_REQ:  
       		p_sprintf_count=icf_cmn_trace_get_codec_list_req(p_msg,p_sprintf_count);            
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_GET_CODEC_LIST_RESP: 
       		p_sprintf_count=icf_cmn_trace_get_codec_list_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

 	case ICF_CODEC_COMMIT_REQ:    
       		p_sprintf_count=icf_cmn_trace_codec_commit_req(p_msg,p_sprintf_count);            
    	        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

   	case ICF_CODEC_COMMIT_RESP:  
       		p_sprintf_count=icf_cmn_trace_codec_commit_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_OPEN_MEDIA_CHANNEL_REQ: 
       	     	p_sprintf_count=icf_cmn_trace_open_channel_req(p_msg,p_sprintf_count);
	     	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_OPEN_MEDIA_CHANNEL_RESP: 
       		p_sprintf_count=icf_cmn_trace_open_media_channel_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_CLOSE_MEDIA_CHANNEL_REQ: 
       		p_sprintf_count=icf_cmn_trace_close_media_channel_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_SEND_UDP_DATA_REQ: 
       		p_sprintf_count=icf_cmn_trace_send_udp_data_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_RECV_UDP_DATA_IND: 
       		p_sprintf_count=icf_cmn_trace_recv_udp_data_ind(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_CREATE_MEDIA_SESSION_REQ: 
       		p_sprintf_count=icf_cmn_trace_cms_req(p_msg,p_sprintf_count);  
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

    	case ICF_CREATE_MEDIA_SESSION_RESP: 
       		p_sprintf_count=icf_cmn_trace_cms_resp(p_msg,p_sprintf_count);      
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

   	case ICF_MODIFY_MEDIA_SESSION_REQ:  
       		p_sprintf_count=icf_cmn_trace_mms_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_MODIFY_MEDIA_SESSION_RESP: 
       		p_sprintf_count=icf_cmn_trace_mms_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

    	case ICF_DELETE_MEDIA_SESSION_REQ:  
       		p_sprintf_count=icf_cmn_trace_dms_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_DELETE_MEDIA_SESSION_RESP: 
               	p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

    	case ICF_MEDIA_CAPABILITIES_REQ:
        	p_sprintf_count=icf_cmn_trace_media_capabilities_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_MEDIA_CAPABILITIES_RESP:
       		p_sprintf_count=icf_cmn_trace_med_cap_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_DELETE_MEDIA_SESSION_IND:  
       		p_sprintf_count=icf_cmn_trace_dms_ind(p_msg,p_sprintf_count);   
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

    	case ICF_MERGE_MEDIA_SESSION_REQ:   
       		p_sprintf_count=icf_cmn_trace_merge_media_req_msg(p_msg,p_sprintf_count);   
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;

	case ICF_MERGE_MEDIA_SESSION_RESP:
               	p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);   
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,p_msg)
		break;    

	case ICF_START_FAX_REQ:
        	p_sprintf_count=icf_cmn_trace_start_fax_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_START_FAX_RESP:
        	p_sprintf_count=icf_cmn_trace_start_fax_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_START_FAX_IND:
        	p_sprintf_count=icf_cmn_trace_start_fax_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_STOP_FAX_RESP:
		p_sprintf_count=icf_cmn_trace_stop_fax_resp(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_STOP_FAX_IND:
		p_sprintf_count=icf_cmn_trace_stop_fax_ind(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

    /* MM API's */
/*
	case ICF_CFG_MAX_MIC_GAIN_REQ:
		p_sprintf_count=icf_cmn_trace_max_mic_gain_req(p_msg,p_sprintf_count);
	        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
	
		break;
	case ICF_SET_MIC_GAIN_REQ:
		p_sprintf_count=icf_cmn_trace_mic_gain_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;
	case ICF_CFG_MAX_MIC_GAIN_RESP:
		p_sprintf_count=icf_cmn_trace_max_mic_gain_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;
	case ICF_SET_MIC_GAIN_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)
		break;    
#ifdef ICF_STAT_ENABLED
	case ICF_GET_ICF_STATS_REQ:
		p_sprintf_count=icf_cmn_trace_get_icf_stats_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break;
    	case ICF_GET_ICF_STATS_RESP:
		p_sprintf_count=icf_cmn_trace_get_icf_stats_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

		break;
#endif
*/
	case ICF_INFO_REQ:     
       		p_sprintf_count=icf_cmn_trace_info_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
                break;

    	case ICF_INFO_RESP:    
       		p_sprintf_count=icf_cmn_trace_info_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break; 
/*
	case ICF_MEDIA_RECORD_REQ:
		p_sprintf_count=icf_cmn_trace_media_record_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_MEDIA_RECORD_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_MEDIA_RECORD_REQ:
		p_sprintf_count=icf_cmn_trace_stop_media_record_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_STOP_MEDIA_RECORD_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_PAUSE_MEDIA_REQ:
		p_sprintf_count=icf_cmn_trace_pause_media_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_PAUSE_MEDIA_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
		p_sprintf_count=icf_cmn_trace_set_media_playback_device_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
		p_sprintf_count=icf_cmn_trace_resp_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_MUTE_MEDIA_REQ:
		p_sprintf_count=icf_cmn_trace_mute_media_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
	case ICF_DGTS_IND:
		p_sprintf_count=icf_cmn_trace_dgts_ind(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break;
*/
	case ICF_CALL_FORWARDED_IND:
       		p_sprintf_count=icf_cmn_trace_call_fwd_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_REINITIALISE_REQ:
        	p_sprintf_count=icf_cmn_trace_no_payload_msg(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_INFO_IND:    
               	p_sprintf_count=icf_cmn_trace_info_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break; 

	case ICF_INFO_CFM:    
       		p_sprintf_count=icf_cmn_trace_info_cfm(p_msg,p_sprintf_count);     
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_NOTIFY_CFM:
        	p_sprintf_count=icf_cmn_trace_notify_cfm(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_OPTIONS_REQ:
       		p_sprintf_count=icf_cmn_trace_options_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_OPTIONS_RESP:
        	p_sprintf_count=icf_cmn_trace_options_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_OPTIONS_CFM:
       		p_sprintf_count=icf_cmn_trace_options_cfm(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_INC_OPTIONS_IND:
       		p_sprintf_count=icf_cmn_trace_options_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;

	case ICF_SET_LINE_PARAM_REQ:
       		p_sprintf_count=icf_cmn_trace_set_line_param_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;
#if 0
	case  ICF_API_ID_UNDEFINED:                            
       		p_sprintf_count=icf_cmn_trace_api_header(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG,ICF_NULL)

		break; 
#endif		

/******* Traces Add Start for SPR 17213 *******/

    case ICF_CREATE_XCONF_REQ:
            p_sprintf_count=icf_cmn_trace_create_ext_xconf_req(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_ADD_XCONF_PARTY_REQ:
            p_sprintf_count=icf_cmn_trace_add_xconf_party_req(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_DELETE_XCONF_PARTY_REQ:
            p_sprintf_count=icf_cmn_trace_delete_xconf_party_req(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
    
        break;  

    case ICF_RELEASE_XCONF_REQ:
            p_sprintf_count=icf_cmn_trace_release_xconf_req(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_RELEASE_XCONF_RESP:
            p_sprintf_count=icf_cmn_trace_release_xconf_resp(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_CREATE_XCONF_RESP:
            p_sprintf_count=icf_cmn_trace_create_ext_xconf_resp(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_ADD_XCONF_PARTY_RESP:
            p_sprintf_count=icf_cmn_trace_add_xconf_party_resp(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_DELETE_XCONF_PARTY_RESP:
            p_sprintf_count=icf_cmn_trace_delete_xconf_party_resp(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_RELEASE_XCONF_IND:
            p_sprintf_count=icf_cmn_trace_xconf_release_ind(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_UNEXPECTED_MSG_IND:
            p_sprintf_count=icf_cmn_trace_unexpected_msg_ind(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
            p_sprintf_count=icf_cmn_trace_remote_call_transfer_initiated_cfm(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_GET_REGISTER_STATUS_REQ:
            p_sprintf_count=icf_cmn_trace_get_register_status_req(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_GET_REGISTER_STATUS_RESP:
            p_sprintf_count=icf_cmn_trace_get_register_status_resp(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

        break;

    case ICF_MM_UNEXPECTED_MSG_IND:
            p_sprintf_count=icf_cmn_trace_mm_unexpected_msg_ind(p_msg,p_sprintf_count);
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)

        break;

    	case ICF_CALL_MODIFY_CANCEL_RESP:
		p_sprintf_count=icf_cmn_trace_call_modify_cancel_resp(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
	
		break;

	case ICF_PROGRESS_IND:
		p_sprintf_count=icf_cmn_trace_progress_ind(p_msg,p_sprintf_count);	
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)		
		break;

	case ICF_SET_TRACE_REQ:
		p_sprintf_count=icf_cmn_trace_set_trace_req(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)		
		break;
/*
	case ICF_REPORT_TRACE_IND:
		p_sprintf_count=icf_cmn_trace_report_trace_ind(p_msg,p_sprintf_count);
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)
	
		break;
*/
#ifdef ICF_ERROR_ENABLE
	case ICF_SET_ERROR_REPORT_LEVEL_REQ:
		p_sprintf_count=icf_cmn_trace_set_error_report_level_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		
		break;
/*
	case ICF_REPORT_ERROR_IND:
		p_sprintf_count=icf_cmn_trace_report_error_ind(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_MEDIA_MESG, p_msg)
		
		break;
*/
#endif
#ifdef ICF_STATS_ENABLE
	case ICF_STATS_REQ:
		p_sprintf_count=icf_cmn_trace_stats_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		
		break;
	case ICF_STATS_RESP:
		p_sprintf_count=icf_cmn_trace_stats_resp(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

		break;
#endif
	case ICF_MEDIA_CONNECT_REQ:
		p_sprintf_count=icf_cmn_trace_media_connect_req(p_msg,p_sprintf_count);
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)

		break;

#ifdef ICF_SESSION_TIMER
    case ICF_SESSION_TIMER_TOGGLE_REQ:
       		p_sprintf_count=icf_cmn_trace_session_timer_toggle_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break; 

   case ICF_SESSION_TIMER_TOGGLE_RESP:
       		p_sprintf_count=icf_cmn_trace_session_timer_toggle_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;  
#endif           

   case ICF_GENERIC_MSG_REQ:
       		p_sprintf_count=icf_cmn_trace_generic_msg_req(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break; 

   case ICF_GENERIC_MSG_RESP:
       		p_sprintf_count=icf_cmn_trace_generic_msg_resp(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break; 
		
   case ICF_GENERIC_MSG_IND:
       		p_sprintf_count=icf_cmn_trace_generic_msg_ind(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break; 

   case  ICF_GENERIC_MSG_CFM:
       		p_sprintf_count=icf_cmn_trace_generic_msg_cfm(p_msg,p_sprintf_count);
    		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
                *p_sprintf_count = '\0';
                ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
		break;  

    /*  Rel 8.2 changes start */
    case ICF_PRACK_REQ_RECV_IND:
        p_sprintf_count=
          icf_cmn_trace_prack_req_recv_ind(p_msg,p_sprintf_count);
        p_sprintf_count += 
               ICF_SPRINTF(((icf_int8_t  *)p_sprintf_count,"\n}\n"));
        *p_sprintf_count = '\0';
         ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
    break;

    case ICF_PRACK_RESP_SENT_IND:
        p_sprintf_count=
          icf_cmn_trace_prack_resp_sent_ind(p_msg,p_sprintf_count);
        p_sprintf_count += 
               ICF_SPRINTF(((icf_int8_t  *)p_sprintf_count,"\n}\n"));
        *p_sprintf_count = '\0';
         ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
     break;

    case ICF_PRACK_REQ_SENT_IND:
        p_sprintf_count=
          icf_cmn_trace_prack_req_sent_ind(p_msg,p_sprintf_count);
        p_sprintf_count += 
               ICF_SPRINTF(((icf_int8_t  *)p_sprintf_count,"\n}\n"));
        *p_sprintf_count = '\0';
         ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
     break;

     case ICF_PRACK_RESP_RECV_IND:
        p_sprintf_count=
          icf_cmn_trace_prack_resp_recv_ind(p_msg,p_sprintf_count);
        p_sprintf_count += 
               ICF_SPRINTF(((icf_int8_t  *)p_sprintf_count,"\n}\n"));
        *p_sprintf_count = '\0';
         ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
     break;
    /* Rel 8.2 changes End */
     case  ICF_MESSAGE_CFM:
         p_sprintf_count=icf_cmn_trace_message_cfm(p_msg,p_sprintf_count);
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n}\n"));
         *p_sprintf_count = '\0';
         ICF_CMN_APP_TRACE(p_glb_pdb,ICF_TRACE_APP_MESG, p_msg)
     break;  


/******* Traces Add End for SPR 17213 *******/	 
	}

	ICF_MEMFREE(
      			p_glb_pdb,
      			p_glb_pdb->p_interface_trace_info,
      			ICF_MEM_COMMON,
      			p_ecode,
      			ret_val)
	return ret_val;

}


/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_api_string
 *
 * DESCRIPTION:     gets the api name using the api_id
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_api_string(icf_api_id_t api_id,icf_uint8_t *p_sprintf_count)

{

    switch(api_id)

    {
    case ICF_CFG_CC_SET_SCALE_PARAMS_REQ         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_CC_SET_SCALE_PARAMS_REQ\n"));break;

    case ICF_CALL_TRANSFER_UNATTENDED_REQ  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\napi name = ICF_CALL_TRANSFER_UNATTENDED_REQ\n"));break;

    case  ICF_CALL_TRANSFER_ATTENDED_REQ         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_TRANSFER_ATTENDED_REQ\n"));break;

    case  ICF_REMOTE_CALL_TRANSFER_INITIATED_IND   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REMOTE_CALL_TRANSFER_INITIATED_IND \n"));break;

    case ICF_CALL_MERGED_IND                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_MERGED_IND\n "));break;

    case  ICF_CONFERENCE_REQ                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \n api name = ICF_CONFERENCE_REQ\n "));break;

    case ICF_CONFERENCE_RESP			: p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CONFERENCE_RESP\n "));break;

    case ICF_SET_SERVICE_PARAMS_REQ             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_SERVICE_PARAMS_REQ\n "));break;

    case ICF_DISPLAY_IND                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_DISPLAY_IND\n "));break;

    case ICF_CALL_REDIRECTION_IND               :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \n api name = ICF_CALL_REDIRECTION_IND\n "));break;

    case ICF_SET_MGMT_PLANE_AUTH_PARAM          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_MGMT_PLANE_AUTH_PARAM \n")); break;

    case ICF_CREATE_SECURE_CONN_REQ             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CREATE_SECURE_CONN_REQ\n ")); break;

    case ICF_CREATE_SECURE_CONN_RESP            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CREATE_SECURE_CONN_RESP\n ")); break;

    case ICF_CLOSE_SECURE_CONN_REQ              :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CLOSE_SECURE_CONN_REQ\n ")); break;

    case ICF_CLOSE_SECURE_CONN_RESP             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CLOSE_SECURE_CONN_RESP \n")); break;

    case ICF_SECURE_CONN_STATUS_IND             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SECURE_CONN_REQ \n")); break;

    case ICF_XCAP_CREATE_REPLACE_RES            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_XCAP_CREATE_REPLACE_RES \n")); break;

    case ICF_XCAP_DELETE_RES                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_XCAP_DELETE_RES\n")); break;

    case ICF_XCAP_FETCH_RES                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_XCAP_FETCH_RES\n ")); break;

    case ICF_SYNCML_SEND_REQ                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SYNCML_SEND_REQ\n")); break;

   case ICF_MGMT_PLANE_API_PROC_FAILURE        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MGMT_PLANE_API_PROC_FAILURE\n")); break;

    case ICF_MGMT_PLANE_NW_RESP                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MGMT_PLANE_NW_RESP\n"));break;

    case ICF_RES_UNAVAIL_IND                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RES_UNAVAIL_IND\n"));break;

    case ICF_SET_STATS_REQ                     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_STATS_REQ\n"));break;

    case ICF_CALL_TRANSFER_UNATTENDED_RESP     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_TRANSFER_UNATTENDED_RESP\n")); break;

    case ICF_CALL_TRANSFER_ATTENDED_RESP       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_TRANSFER_ATTENDED_RESP\n")); break;

    case ICF_CALL_FORWARDED_IND                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_FORWARDED_IND\n")); break;

    case ICF_LOGS_IND                          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_LOGS_IND\n"));break;

    case ICF_CFG_SET_VIDEO_ATTRIB_REQ          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_VIDEO_ATTRIB_REQ\n"));break;

    case ICF_MM_SET_NW_STATUS                  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MM_SET_NW_STATUS\n"));break;

    case ICF_SET_LINE_PARAM_REQ                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_LINE_PARAM_REQ\n")); break;

    case ICF_API_ID_INVALID                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_API_ID_INVALID\n"));break;

#ifdef ICF_NW_ACTIVATE

    case ICF_EVENT_INDICATION                  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_EVENT_INDICATION\n"));break;

#endif

    case ICF_APP_ADD_REQ                       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_APP_ADD_REQ\n"));break;

    case ICF_APP_ADD_CFM                       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_APP_ADD_CFM\n"));break;

    case ICF_APP_REMOVE_REQ                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_APP_REMOVE_REQ\n"));break;

    case ICF_APP_REMOVE_CFM                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_APP_REMOVE_CFM\n"));break;

#ifdef ICF_NW_ACTIVATE

    case ICF_NETWORK_ACTIVATE_REQ              :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NETWORK_ACTIVATE_REQ\n"));break;

    case ICF_NETWORK_ACTIVATE_CFM              :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NETWORK_ACTIVATE_CFM\n"));break;

    case ICF_NETWORK_DEACTIVATE_REQ            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NETWORK_DEACTIVATE_REQ\n"));break;

    case ICF_NETWORK_DEACTIVATE_CFM            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NETWORK_DEACTIVATE_CFM\n"));break;

#endif

    case ICF_SET_TRANSPORT_REQ                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_TRANSPORT_REQ\n"));break;

    case ICF_SET_CALL_PARAM_REQ                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_CALL_PARAM_REQ\n"));break;

    case ICF_SET_SELF_ID_REQ                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_SELF_ID_REQ"));break;

    case ICF_SET_REGISTRATION_REQ              :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_REGISTRATION_REQ"));break;

    case ICF_CONFIGURE_CFM                     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CONFIGURE_CFM"));break;

	case ICF_GET_SYSTEM_SETTINGS_REQ           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_SYSTEM_SETTINGS_REQ"));break;  

	case ICF_GET_LINE_SETTINGS_REQ             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_LINE_SETTINGS_REQ"));break;   

	case ICF_GET_SYSTEM_SETTINGS_RESP          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_SYSTEM_SETTINGS_RESP"));break; 

	case ICF_GET_LINE_SETTINGS_RESP            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_LINE_SETTINGS_RESP"));break; 

    case ICF_REGISTER_REQ                      :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REGISTER_REQ"));break;

    case ICF_REGISTER_CFM                      :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REGISTER_CFM"));break;

    case ICF_REGISTER_STATUS_IND               :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REGISTER_STATUS_IND"));break;

    case ICF_CREATE_CALL_REQ                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CREATE_CALL_REQ"));break;

    case ICF_LOCAL_USER_ALERTED_REQ            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_LOCAL_USER_ALERTED_REQ"));break;
    
    case ICF_MEDIA_CONNECT_REQ                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_CONNECT_REQ"));break;
 
    case ICF_TERMINATE_CALL_REQ                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_TERMINATE_CALL_REQ"));break;

    case ICF_LOCAL_USER_TERMINATE_CALL_RESP    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_LOCAL_USER_TERMINATE_CALL_RESP"));break;

    case ICF_CONNECT_REQ                       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CONNECT_REQ"));break;

    case ICF_SUBSCRIBE_REQ                     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SUBSCRIBE_REQ"));break;

    case ICF_PUBLISH_REQ                       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PUBLISH_REQ"));break;

    case ICF_MESSAGE_REQ                       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MESSAGE_REQ"));break;

    case ICF_REFER_REQ                         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REFER_REQ"));break;

    case ICF_CALL_HOLD_REQ                     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_HOLD_REQ"));break;

    case ICF_CALL_HOLD_RESP                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_HOLD_RESP"));break;

    case ICF_CALL_RESUME_REQ                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_RESUME_REQ"));break;

    case ICF_CALL_RESUME_RESP                  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_RESUME_RESP"));break;

    case ICF_INCOMING_CALL_IND                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INCOMING_CALL_IND"));break;

	case ICF_NOTIFY_CFM                         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NOTIFY_CFM"));break;

	case ICF_INFO_REQ                          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INFO_REQ"));break;

	case ICF_INFO_RESP                          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INFO_RESP"));break;

	case ICF_INFO_IND	                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INFO_IND"));break;
	case ICF_OPTIONS_REQ	                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_OPTIONS_REQ"));break;
    case ICF_OPTIONS_RESP	                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_OPTIONS_RESP"));break;
    case ICF_INC_OPTIONS_IND                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INC_OPTIONS_IND"));break;
    case ICF_OPTIONS_CFM                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_OPTIONS_CFM"));break;
    case ICF_INFO_CFM                           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INFO_CFM"));break;
    case ICF_RESTART_REQ                           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RESTART_REQ"));break;
    case ICF_RESTART_IND                           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RESTART_IND"));break;
    case ICF_RESTART_RESP                           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RESTART_RESP"));break;
    case ICF_REINITIALISE_REQ                           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REINITIALISE_REQ"));break;
    case ICF_REINIT_IND                           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REINIT_IND"));break;
    case ICF_REINIT_RESP                           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REINIT_RESP"));break;

ICF_HANDLE_REINVITE_START

	case ICF_CALL_MODIFY_REQ	                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_MODIFY_REQ"));break;

	case ICF_CALL_MODIFY_RESP	                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_MODIFY_RESP"));break;

	case ICF_INCOMING_CALL_MODIFY_IND           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INCOMING_CALL_MODIFY_IND"));break;

	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_INCOMING_CALL_MODIFY_CANCEL_IND"));break;

	case ICF_CALL_MODIFY_CFM		            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_MODIFY_CFM"));break;

ICF_HANDLE_REINVITE_END



    case ICF_REMOTE_USER_ALERTED_IND           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REMOTE_USER_ALERTED_IND"));break;

    case ICF_TERMINATE_CALL_IND                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_TERMINATE_CALL_IND"));break;

    case ICF_REMOTE_USER_TERMINATE_CALL_RESP   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REMOTE_USER_TERMINATE_CALL_RESP"));break;

    case ICF_MEDIA_CONNECTED_IND               :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_CONNECTED_IND"));break;

    case ICF_CONNECT_IND                       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CONNECT_IND"));break;

	case ICF_PARTY_REPLACED_IND            	:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PARTY_REPLACED_IND "));break;

    case ICF_SUBSCRIPTION_STATUS_IND           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SUBSCRIPTION_STATUS_IND"));break;

    case ICF_NOTIFY_IND                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NOTIFY_IND"));break;

    case ICF_PUBLISH_RESP                      :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PUBLISH_RESP"));break;

    case ICF_MESSAGE_RESP                      :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MESSAGE_RESP"));break;

    case ICF_MESSAGE_IND                       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MESSAGE_IND"));break;

    case ICF_REFER_RESP                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REFER_RESP"));break;

	
	case ICF_CONNECT_RESP                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CONNECT_RESP"));break;
    
	case ICF_START_FAX_REQ                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_START_FAX_REQ"));break;
	
	case ICF_START_FAX_RESP                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_START_FAX_RESP"));break;

	case ICF_START_FAX_IND                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_START_FAX_IND"));break;


	case ICF_CFG_CODEC_INFO_REQ                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_CODEC_INFO_REQ"));break;

    case ICF_CFG_CODEC_INFO_RESP               :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_CODEC_INFO_RESP"));break;

    case ICF_CFG_RINGER_TONES_REQ              :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_RINGER_TONES_REQ"));break;

    case ICF_CFG_RINGER_TONES_RESP             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_RINGER_TONES_RESP"));break;

    case ICF_CFG_JBM_MGMT_REQ                  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_JBM_MGMT_REQ"));break;

    case ICF_CFG_JBM_MGMT_RESP                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_JBM_MGMT_RESP"));break;

    case ICF_CFG_MAX_EARP_VOL_REQ              :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MAX_EARP_VOL_REQ"));break;

    case ICF_CFG_MAX_EARP_VOL_RESP             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MAX_EARP_VOL_RESP"));break;

    case ICF_CFG_MAX_SPEAKER_VOL_REQ           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MAX_SPEAKER_VOL_REQ"));break;

    case ICF_CFG_MAX_SPEAKER_VOL_RESP          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MAX_SPEAKER_VOL_RESP"));break;

    case ICF_CFG_SET_PORT_RANGE_REQ                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_PORT_RANGE_REQ"));break;

    case ICF_CFG_SET_PORT_RANGE_RESP               :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_PORT_RANGE_RESP"));break;

    case ICF_START_RINGING_REQ                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_START_RINGING_REQ"));break;

    case ICF_START_RINGING_RESP                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_START_RINGING_RESP"));break;

    case ICF_MEDIA_SESSION_REQ                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_SESSION_REQ"));break;

    case ICF_MEDIA_SESSION_RESP                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_SESSION_RESP"));break;

    case ICF_RTP_START_IND                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RTP_START_IND"));break;

    case ICF_GET_STATS_REQ                     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_STATS_REQ"));break;

    case ICF_GET_STATS_RESP                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_STATS_RESP"));break;

    case ICF_STOP_RINGING_REQ                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_RINGING_REQ"));break;

    case ICF_STOP_RINGING_RESP                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_RINGING_RESP"));break;

    case ICF_STOP_RINGING_IND                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_RINGING_IND"));break;

	
	case ICF_MUTE_MEDIA_REQ                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MUTE_MEDIA_REQ"));break;


    case ICF_MUTE_MEDIA_RESP                  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MUTE_MEDIA_RESP"));break;

	
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ"));break;

    case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP"));break;
    
	
	case ICF_PLAY_MEDIA_REQ					  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PLAY_MEDIA_REQ"));break;
  
    case ICF_PLAY_MEDIA_RESP			      :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PLAY_MEDIA_RESP"));break;


    case ICF_SET_EARP_VOLUME_REQ              :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_EARP_VOLUME_REQ"));break;

    case ICF_SET_EARP_VOLUME_RESP             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_EARP_VOLUME_RESP"));break;

    case ICF_SET_SPEAKER_VOLUME_REQ           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_SPEAKER_VOLUME_REQ"));break;

    case ICF_SET_SPEAKER_VOLUME_RESP          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_SPEAKER_VOLUME_RESP"));break;

    case ICF_SET_MIC_GAIN_REQ				   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_MIC_GAIN_REQ"));break;

   	case ICF_SET_MIC_GAIN_RESP			       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_MIC_GAIN_RESP"));break;

    case ICF_CFG_MAX_MIC_GAIN_REQ			   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MAX_MIC_GAIN_REQ"));break;

	case ICF_CFG_MAX_MIC_GAIN_RESP			   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MAX_MIC_GAIN_RESP"));break;

    case ICF_SEND_APP_DATA_REQ                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SEND_APP_DATA_REQ"));break;

    case ICF_SEND_APP_DATA_RESP                :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SEND_APP_DATA_RESP"));break;

    case ICF_RCV_APP_DATA_IND                  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RCV_APP_DATA_IND"));break;

    case ICF_SEND_STATS_REQ                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SEND_STATS_REQ"));break;

    case ICF_SEND_STATS_RESP                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SEND_STATS_RESP"));break;

    case ICF_STOP_STATS_REQ                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_STATS_REQ"));break;

    case ICF_STOP_STATS_RESP                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_STATS_RESP"));break;

	case ICF_MSG_FRM_PLTFM		       :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MSG_FRM_PLTFM"));break;	

    case ICF_NW_API_MSG_FROM_PEER            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NW_API_MSG_FROM_PEER"));break;

    case ICF_NW_API_OPEN_CONN_IND            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NW_API_OPEN_CONN_IND"));break;

    case ICF_NW_API_CONNECT_SUCCESS          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NW_API_CONNECT_SUCCESS"));break;

    case ICF_NW_API_CONNECT_FAILURE          :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NW_API_CONNECT_FAILURE"));break;

    case ICF_NW_API_CLOSE_CONN_IND            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_NW_API_CLOSE_CONN_IND"));break;

    case ICF_TIMER_MSG                        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_TIMER_MSG"));break;


#ifdef ICF_DEBUG_MEMPOOL

	case ICF_MEM_POOL_STATS_PRINT            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEM_POOL_STATS_PRINT"));break;

#endif        

    case ICF_VIBRATION_REQ					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_VIBRATION_REQ"));break;

    case ICF_VIBRATION_RESP					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_VIBRATION_RESP"));break;

    case ICF_ECHO_CANC_REQ					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_ECHO_CANC_REQ"));break;

    case ICF_ECHO_CANC_RESP					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_ECHO_CANC_RESP"));break;

    case ICF_VAD_SS_CNG_REQ					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_VAD_SS_CNG_REQ"));break;

    case ICF_VAD_SS_CNG_RESP				:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_VAD_SS_CNG_RESP"));break;
 
    case ICF_STOP_MEDIA_PLAY_REQ			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_MEDIA_PLAY_REQ"));break;

    case ICF_STOP_MEDIA_PLAY_RESP			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_MEDIA_PLAY_RESP"));break;

    case ICF_STOP_MEDIA_PLAY_IND			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_MEDIA_PLAY_IND"));break;

    case ICF_STOP_VIBRATION_REQ				:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_VIBRATION_REQ"));break;

    case ICF_STOP_VIBRATION_RESP			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_VIBRATION_RESP"));break;

    case ICF_CFG_MEDIA_REQ					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MEDIA_REQ"));break;

    case ICF_CFG_MEDIA_RESP					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_MEDIA_RESP"));break;

    case ICF_CFG_SET_DIGIT_TRANS_REQ		:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_DIGIT_TRANS_REQ"));break;

    case ICF_CFG_SET_DIGIT_TRANS_RESP		:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_DIGIT_TRANS_RESP"));break;

    case ICF_CFG_SET_SELF_IP_REQ			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_SELF_IP_REQ"));break;

    case ICF_CFG_SET_SELF_IP_RESP			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_SELF_IP_RESP"));break;


    case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ	:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ"));break;

    case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP	:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP"));break;
	
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ	:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ"));break;

    case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP	:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP"));break;

	
	case ICF_DGTS_IND				:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_DGTS_IND"));break;  

    case ICF_CONNECT_MEDIA_DEVICE_REQ		:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CONNECT_MEDIA_DEVICE_REQ"));break;

	case ICF_CONNECT_MEDIA_DEVICE_RESP		:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CONNECT_MEDIA_DEVICE_RESP"));break;

    case ICF_MEDIA_RECORD_REQ				:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_RECORD_REQ"));break;

	case ICF_MEDIA_RECORD_RESP				:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_RECORD_RESP"));break;

	case ICF_PAUSE_MEDIA_REQ				:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PAUSE_MEDIA_REQ"));break;

	case ICF_PAUSE_MEDIA_RESP				:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PAUSE_MEDIA_RESP"));break;


    case ICF_SEND_DGTS_REQ					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SEND_DGTS_REQ"));break;

    case ICF_SEND_DGTS_RESP					:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SEND_DGTS_RESP"));break;

    case ICF_STOP_MEDIA_RECORD_REQ			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_MEDIA_RECORD_REQ"));break;

	case ICF_STOP_MEDIA_RECORD_RESP			:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_MEDIA_RECORD_RESP"));break;


    case ICF_CREATE_MEDIA_SESSION_REQ    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CREATE_MEDIA_SESSION_REQ"));break;     

    case ICF_CREATE_MEDIA_SESSION_RESP   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CREATE_MEDIA_SESSION_RESP"));break;  

    case ICF_MODIFY_MEDIA_SESSION_REQ    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MODIFY_MEDIA_SESSION_REQ"));break;     

    case ICF_MODIFY_MEDIA_SESSION_RESP   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MODIFY_MEDIA_SESSION_RESP"));break;  

    case ICF_DELETE_MEDIA_SESSION_REQ    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_DELETE_MEDIA_SESSION_REQ"));break;   

    case ICF_DELETE_MEDIA_SESSION_RESP  :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_DELETE_MEDIA_SESSION_RESP"));break;  

    case ICF_DELETE_MEDIA_SESSION_IND    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_DELETE_MEDIA_SESSION_IND"));break;     

    case ICF_MERGE_MEDIA_SESSION_REQ    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MERGE_MEDIA_SESSION_REQ"));break;    

    case ICF_MERGE_MEDIA_SESSION_RESP    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MERGE_MEDIA_SESSION_RESP"));break;    

    case ICF_GET_CODEC_LIST_REQ         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_CODEC_LIST_REQ"));break;   

    case ICF_GET_CODEC_LIST_RESP        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_CODEC_LIST_RESP"));break;    

    case ICF_CODEC_COMMIT_REQ            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CODEC_COMMIT_REQ"));break;     

    case ICF_CODEC_COMMIT_RESP           :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CODEC_COMMIT_RESP"));break;  

    case ICF_REL_RES_REQ                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REL_RES_REQ"));break;    
	
	case ICF_MEDIA_CAPABILITIES_REQ      :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_CAPABILITIES_REQ"));break;     

    case ICF_MEDIA_CAPABILITIES_RESP     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MEDIA_CAPABILITIES_RESP"));break;  

	case ICF_OPEN_MEDIA_CHANNEL_REQ			 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_OPEN_CHANNEL_REQ"));break;     

    case ICF_OPEN_MEDIA_CHANNEL_RESP     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_OPEN_MEDIA_CHANNEL_RESP"));break;  

	case ICF_CLOSE_MEDIA_CHANNEL_REQ     :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CLOSE_MEDIA_CHANNEL_REQ"));break;     


    case ICF_SEND_UDP_DATA_REQ			 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SEND_UDP_DATA_REQ"));break;  

	case ICF_RECV_UDP_DATA_IND			 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RECV_UDP_DATA_IND"));break;

	case ICF_PORT_PLTFM_SA_CREATE_REQ         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_CREATE_REQ"));break;

    case ICF_PORT_PLTFM_SA_CREATE_RESP        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_CREATE_RESP"));break;

    case ICF_PORT_PLTFM_SA_MODIFY_REQ         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_MODIFY_REQ"));break;

    case ICF_PORT_PLTFM_SA_MODIFY_RESP        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_MODIFY_RESP"));break;

    case ICF_PORT_PLTFM_SA_DELETE_REQ         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_DELETE_REQ"));break;

    case ICF_PORT_PLTFM_SA_DELETE_RESP        :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_DELETE_RESP"));break;

    case ICF_PORT_PLTFM_SA_EXPIRY_IND         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"ICF_WRPR_SA_EXPIRY_IND"));break;

    case ICF_PORT_PLTFM_SA_FETCH_ENC_ALGO_REQ   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_FETCH_ENC_ALGO"));break;

    case ICF_PORT_PLTFM_SA_FETCH_AUTH_ALGO_RESP :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_WRPR_SA_FETCH_AUTH_ALGO"));break;

	case ICF_MM_INIT_COMPLETE :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MM_INIT_COMPLETE"));break;

	case ICF_STOP_FAX_REQ            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_FAX_REQ"));break;
	case ICF_STOP_FAX_RESP            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_FAX_RESP"));break;
	case ICF_STOP_FAX_IND            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STOP_FAX_IND"));break;
	case ICF_SET_TRACE_REQ            :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_TRACE_REQ"));break;
	case ICF_REPORT_TRACE_IND         :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REPORT_TRACE_IND"));break;
        case ICF_CALL_MODIFY_CANCEL_RESP      :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CALL_MODIFY_CANCEL_RESP"));break;
    case ICF_PROGRESS_IND                 :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PROGRESS_IND"));break;
    case ICF_SET_ERROR_REPORT_LEVEL_REQ   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SET_ERROR_REPORT_LEVEL_REQ"));break;
    case ICF_STATS_REQ                    :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STATS_REQ"));break;
    case ICF_REPORT_ERROR_IND             :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REPORT_ERROR_IND"));break;
    case ICF_STATS_RESP                   :p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_STATS_RESP"));break;

/******* Traces Add Start for SPR 17213 *******/

        case ICF_CREATE_XCONF_REQ:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CREATE_XCONF_REQ"));
             break;
        case ICF_ADD_XCONF_PARTY_REQ:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_ADD_XCONF_PARTY_REQ"));
             break;
        case ICF_DELETE_XCONF_PARTY_REQ:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_DELETE_XCONF_PARTY_REQ"));
             break;
        case ICF_RELEASE_XCONF_REQ:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RELEASE_XCONF_REQ"));
             break;
        case ICF_RELEASE_XCONF_RESP:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RELEASE_XCONF_RESP"));
             break;
        case ICF_CREATE_XCONF_RESP:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_CREATE_XCONF_RESP"));
             break;
        case ICF_ADD_XCONF_PARTY_RESP:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_ADD_XCONF_PARTY_RESP"));
             break;
        case ICF_DELETE_XCONF_PARTY_RESP:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_DELETE_XCONF_PARTY_RESP"));
             break;
        case ICF_RELEASE_XCONF_IND:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_RELEASE_XCONF_IND"));
             break;
        case ICF_EARLY_INC_CALL_IND:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_EARLY_INC_CALL_IND"));
             break;
        case ICF_EARLY_INC_CALL_CFM:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_EARLY_INC_CALL_CFM"));
             break;
        case ICF_UNEXPECTED_MSG_IND:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_UNEXPECTED_MSG_IND"));
             break;
        case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM"));
             break;
        case ICF_GET_REGISTER_STATUS_REQ:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_REGISTER_STATUS_REQ"));
             break;
        case ICF_GET_REGISTER_STATUS_RESP:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GET_REGISTER_STATUS_RESP"));
             break;
        case ICF_AKA_AUTH_RESP:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_AKA_AUTH_RESP"));
             break;
        case ICF_MM_UNEXPECTED_MSG_IND:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MM_UNEXPECTED_MSG_IND"));
             break;
 
        case ICF_SESSION_TIMER_TOGGLE_REQ:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SESSION_TIMER_TOGGLE_REQ"));
             break; 

         case ICF_SESSION_TIMER_TOGGLE_RESP:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_SESSION_TIMER_TOGGLE_RESP"));
             break;

	 case ICF_GENERIC_MSG_REQ:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GENERIC_MSG_REQ "));
	     break; 


	 case ICF_GENERIC_MSG_RESP:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GENERIC_MSG_RESP "));
	     break;	

	 case ICF_GENERIC_MSG_IND:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GENERIC_MSG_IND "));
	     break;	

	 case ICF_GENERIC_MSG_CFM:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_GENERIC_MSG_CFM "));
	     break;
     /*Start Added for SPR 19473 */
	 case ICF_PRACK_REQ_RECV_IND:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PRACK_REQ_RECV_IND "));
	     break;
	 case ICF_PRACK_RESP_SENT_IND:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PRACK_RESP_SENT_IND "));
	     break;
	 case ICF_PRACK_REQ_SENT_IND:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PRACK_REQ_SENT_IND "));
	     break;
	 case ICF_PRACK_RESP_RECV_IND:
	     p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_PRACK_RESP_RECV_IND "));
	     break;
     /*End SPR 19473 */
        case ICF_MESSAGE_CFM:
             p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = ICF_MESSAGE_CFM "));
             break;


/******* Traces Add End for SPR 17213 *******/


    default:p_sprintf_count+=ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n api name = UNKNOWN_API_ID"));break ;    

    }

    return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_msg()
 *
 * DESCRIPTION:     Try tracing icf_msg_st messages
 *
 * Functions called: 
 *                     
 *
 *****************************************************************************/

icf_return_t icf_cmn_trace_msg(
	INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_error_t *p_ecode)
{
    icf_msg_st *p_msg = ICF_NULL;
    /*Fix for SPR : 18999*/
    if (ICF_NULL ==  p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"[CMN] : p_glb_pdb is NULL.\n"))
        return ICF_FAILURE;
    }
    ICF_CHECK_IF_VAR_IS_NULL_THEN_RETURN(p_glb_pdb,p_glb_pdb->p_msg_lst) 
        
    if (!(p_glb_pdb->p_msg_lst->p_msg)) {
	return ICF_FAILURE;
    }
    p_msg = (icf_msg_st *) p_glb_pdb->p_msg_lst->p_msg;
    icf_cmn_trace_api_from_api_id(p_glb_pdb,p_msg,p_ecode);

    return ICF_SUCCESS;
}




/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_api_header
 *
 * DESCRIPTION:     Trace the api header inside the icf_msg_st
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_api_header(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

   if(ICF_NULL == p_msg)
   {
       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"p_msg is null"));
       return p_sprintf_count;

   }
   
    p_sprintf_count = icf_cmn_trace_api_string(p_msg->hdr.api_id,p_sprintf_count);
 
   /* p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nversion = %d, app_id = %d",p_msg->hdr.version, p_msg->hdr.app_id))

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            (icf_uint8_t *)"\nsource_id = %d dest_id = %d \ncall_id = %d api_length = %d",
                            p_msg->hdr.source_id,
			    p_msg->hdr.destination_id,
			    (icf_call_id_t)p_msg->hdr.call_id,
			    p_msg->hdr.api_length
                            ))*/
    return p_sprintf_count;
}




/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_no_payload_msg()
 *
 * DESCRIPTION:     Trace messages with no payload 
 *
 * Functions called: 
 *                     
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_no_payload_msg(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    
   /* p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n{\n}\n"));*/

#ifdef ICF_FOR_LOAD_TESTING

	if ( p_msg->hdr.api_id == ICF_REMOTE_USER_TERMINATE_CALL_RESP)
        {
		t_calls++;
        	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            (icf_uint8_t *)"\nTerminated Calls:%d",
                            t_calls
                            ))
        }

#endif

    return p_sprintf_count;
}




/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_resp_msg()
 *
 * DESCRIPTION:     trace response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_resp_msg(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_msg_resp_st* p_resp_msg = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_resp_msg = (icf_msg_resp_st*)p_msg->payload;

    /*Fix for SPR : 18999*/
    if (ICF_NULL == p_resp_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"[CMN] : p_resp_msg is NULL.\n"))
        return p_sprintf_count;
    }
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_cause = %d",p_resp_msg->result,p_resp_msg->error_cause));

    if (p_resp_msg->bit_mask & ICF_MSG_RESP_LINE_ID_PRESENT)
    {
       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nLine_id = %d",p_resp_msg->line_id));
    }            

    return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_api_resp_msg()
 *
 * DESCRIPTION:     trace response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_api_resp_msg(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_api_resp_st* p_resp_msg = NULL;
	icf_uint8_t		i = 0;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_resp_msg = (icf_api_resp_st*)p_msg->payload;

    /*Fix for SPR : 18999*/
    if (ICF_NULL == p_resp_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_resp_msg is NULL.\n"))
        return p_sprintf_count;
    }

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_cause = %d",p_resp_msg->result,p_resp_msg->error_cause));

    p_sprintf_count = icf_cmn_trace_error_details(p_resp_msg->error_cause,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d",p_resp_msg->bit_mask));

/* Header LIST */
    if (p_resp_msg->bit_mask & ICF_HDR_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_header_list(p_resp_msg->header_list.hdr, p_resp_msg->header_list.count,p_sprintf_count);
    }    

/* Body LIST */
    if (p_resp_msg->bit_mask & ICF_BODY_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_body_list(p_resp_msg->body_list.body, p_resp_msg->body_list.count,p_sprintf_count);
    }
	
	if(p_resp_msg->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream count %d:",p_resp_msg->stream_list.count));

		for(i=0;(i<p_resp_msg->stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",p_resp_msg->stream_list.stream_id[i]));
		}
	}
        if( ICF_RETRY_AFTER_DURATION_PRESENT & p_resp_msg->bit_mask)
	{
	     p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nREtry After Duration = %d",
		p_resp_msg->retry_after_duration));	    
	}

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_string_list(icf_string_list_st *p_str_list,icf_uint8_t *p_list_name, icf_uint8_t* p_sprintf_count)
{
    icf_string_list_node_st *p_cur_node = NULL;
    icf_uint8_t          i = 0;
    /*Fix For SPR 18999*/
    if (ICF_NULL == p_str_list)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_str_list is NULL.\n"))
        return p_sprintf_count;
    }
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\n\n%s.count = %d",
                            p_list_name,p_str_list->count
                            ))

    for(i = 0, p_cur_node = p_str_list->p_list;
    i < p_str_list->count && (p_cur_node);
    p_cur_node = p_cur_node->p_next,i++)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\n%s%d = %s",
                             p_list_name,i+1,p_cur_node->str.str
                            ))
    }
    return p_sprintf_count;
}




/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_config_params_resp()
 *
 * DESCRIPTION:     trace set_config_params_resp
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_config_params_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_cc_cfg_set_config_params_resp_st* p_resp_msg = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_resp_msg = (icf_cc_cfg_set_config_params_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_resp_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_resp_msg is NULL.\n"))
        return p_sprintf_count;
    }    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nLine ID = %d",p_resp_msg->line_id));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_cause = %d",p_resp_msg->result, p_resp_msg->error_cause));
   
    p_sprintf_count = icf_cmn_trace_error_details(p_resp_msg->error_cause,p_sprintf_count);

    return p_sprintf_count;
}/* Function icf_cmn_trace_config_params_resp() ends here */





icf_uint8_t* icf_cmn_trace_set_scale_params(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{

    
    icf_cfg_cc_set_scale_params_req_st* p_scale = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    
    
    p_scale = (icf_cfg_cc_set_scale_params_req_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_scale)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_scale is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\nbit_mask= %d, \nmax_lines = %d, max_audio_calls = %d,\nmax_video_calls = %d, \nmax_service_calls = %d, max_registration_calls = %d,\nmax_users_per_line = %d, \nnum_of_calls_to_log = %d, \nmax_num_of_apps = %d",
			    p_scale->bit_mask,
			    p_scale->max_lines,
			    p_scale->max_audio_calls,
			    p_scale->max_video_calls,
			    p_scale->max_service_calls,
			    p_scale->max_registration_calls,
			    p_scale->max_users_per_line,
			    p_scale->num_of_calls_to_log,
			    p_scale->max_num_of_apps
                            ));

    return p_sprintf_count;

}




icf_uint8_t* icf_cmn_trace_set_trans_params(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

   
    icf_uint8_t i = 0,addr_type=0;
    icf_uint32_t presence_flag = 0;

    icf_cfg_cc_set_transport_params_req_st* set_trans = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    set_trans = (icf_cfg_cc_set_transport_params_req_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == set_trans)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : set_trans is NULL.\n"))
        return p_sprintf_count;
    }        

    presence_flag = set_trans->bit_mask;

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\napi name : Set Trans Param Req\n"));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d",presence_flag));

    if (presence_flag & ICF_SET_TRP_REQ_PROXY_ADDR) 
    {            
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ntransport_addr[%d] = line_id = %d, action = %d, \taddr_type = %d,",
			     i, 
                	     set_trans->proxy_addr.line_id, 
            		     set_trans->proxy_addr.action,
              		     set_trans->proxy_addr.transport_addr.addr.addr_type
                            ));

        addr_type = set_trans->proxy_addr.transport_addr.addr.addr_type;
        
        switch (addr_type) {
        case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%s, ",
                            set_trans->proxy_addr.transport_addr.addr.addr.domain.str
                            ))
            break;
            
        case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d, \nport_num = %d, mode = %d ",
                            set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_1, 
               		    set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_2, 
                	    set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_3, 
                	    set_trans->proxy_addr.transport_addr.addr.addr.ipv4_addr.octet_4,
			    set_trans->proxy_addr.transport_addr.port_num,
            		    set_trans->proxy_addr.transport_mode
                            ));
            break;
            
        }
        
    }
    
    if (presence_flag & ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS) 
    {        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\nself_transport_address = (addr_type = %d,",
                            set_trans->self_transport_address.addr_type
                            ))

        addr_type = set_trans->self_transport_address.addr_type;
        
        switch (addr_type) {
        case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%s, ",
                            set_trans->self_transport_address.addr.domain.str
                            ))
            break;
            
        case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%d.%d.%d.%d ",
                            set_trans->self_transport_address.
                      	    addr.ipv4_addr.octet_1, 
                      	    set_trans->self_transport_address.
                      	    addr.ipv4_addr.octet_2, 
                      	    set_trans->self_transport_address.
                       	    addr.ipv4_addr.octet_3, 
                      	    set_trans->self_transport_address.
                      	    addr.ipv4_addr.octet_4
                            ))

            break;
        }
    }

    if (presence_flag & ICF_CFG_CC_SET_TRP_PRM_SLF_PORT) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\nself_port = %d",
                            set_trans->self_port
                            ))

    if (presence_flag & ICF_CFG_CC_SET_TRP_PRM_SLF_MODE) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            ", self_mode = %d",
                            set_trans->self_mode
                            ))

    if (ICF_SET_TRP_REQ_DHCP_SERVER_ADDR & presence_flag)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nDHCP Server Address =  addr_type = %d,",
                             set_trans->dhcp_server_addr.addr.addr_type
                            ));

        addr_type = set_trans->dhcp_server_addr.addr.addr_type;

        switch (addr_type) 
        {
            case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
                 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%s, ",
                            set_trans->dhcp_server_addr.addr.addr.domain.str
                            ))
            break;

        case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%d.%d.%d.%d ",
                            set_trans->dhcp_server_addr.addr.
                            addr.ipv4_addr.octet_1,
                            set_trans->dhcp_server_addr.addr.
                            addr.ipv4_addr.octet_2,
                            set_trans->dhcp_server_addr.addr.
                            addr.ipv4_addr.octet_3,
                            set_trans->dhcp_server_addr.addr.
                            addr.ipv4_addr.octet_4
                            ))

            break;
        }
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\nPort = %d, ",
                            set_trans->dhcp_server_addr.port_num
                            ))


    
    }
    if (ICF_SET_TRP_REQ_DHCP_CLIENT_PORT & presence_flag)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\n Client Port = %d, ",
                            set_trans->dhcp_client_port
                            ))

    }
    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_set_call_params(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    
    icf_uint32_t i = 0;

    icf_cfg_cc_set_call_params_req_st* p_set_call = NULL;
    icf_list_st *p_temp_node = ICF_NULL;
    
    p_set_call = (icf_cfg_cc_set_call_params_req_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_set_call)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_set_call is NULL.\n"))
        return p_sprintf_count;
    }    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "bit_mask = %u bit_mask_1 = %u \ndefault_settings = %u, qos_mark = %u, \nsetup_timer = %u, alerting_timer = %u,\nrelease_timer = %u, \nmodify_media_timer = %u, registration_resp_timer = %u,\nregistration_retry_timer = %u  ",
                            p_set_call->bit_mask,p_set_call->bit_mask_1,
                            p_set_call->default_settings, p_set_call->qos_mark,
                            p_set_call->setup_timer, p_set_call->alerting_timer,
	                        p_set_call->release_timer,p_set_call->modify_media_timer,
                            p_set_call->registration_resp_timer, 
                            p_set_call->registration_retry_timer
                            ))
   
    if (p_set_call->bit_mask & ICF_SET_CALL_PRM_INFO_TIMER)
    {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "\ninfo_timer = %u",
                            p_set_call->info_timer
                            ))
    }

    if (p_set_call->bit_mask & ICF_SET_CALL_PRM_OPTIONS_TIMER)
    {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\noptions_timer = %u",p_set_call->options_timer));
    }

    if(p_set_call->bit_mask & ICF_SET_CALL_PRM_PHONE_INFO)
    {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nphone_info = "));

	for(i=0;(i<p_set_call->phone_info.str_len) && (i < ICF_MAX_STR_LEN);i++)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
                    p_set_call->phone_info.str[i]));
	}
    }

#ifdef ICF_SESSION_TIMER
    if(p_set_call->bit_mask & ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL)
    {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession_timer = %u",
                    p_set_call->session_timer))

    }

    if(p_set_call->bit_mask & ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL)
    {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmin_se = %u",
                    p_set_call->min_se))
    }

    if(ICF_SET_CALL_PRM_SESSION_TIMER_ENABLE & p_set_call->bit_mask)
    {
    	p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession_timer_enable = %u",
                    p_set_call->session_timer_enable))
    }

    if (ICF_SET_CALL_PRM_SESSION_REFRESHER & p_set_call->bit_mask)
    {
    	p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession refresher = %u",
                    p_set_call->session_refresher))
    }

    if (ICF_SET_CALL_PRM_SESSION_TIMER_LOGIC & p_set_call->bit_mask)
    {
    	p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession_timer_logic = %u",
                    p_set_call->session_timer_logic))
    }
#endif

#ifdef ICF_NAT_RPORT_SUPPORT
    if(ICF_SET_BINDING_REFRESH_TIMER & p_set_call->bit_mask)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbinding_refresh_timer = %u",
                    p_set_call->binding_refresh_timer))
    }
#endif
   /* if(ICF_TRUE==p_set_call->multiple_answer_enable)
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmult_answer = true",))
      else
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmult_answer = false",))*/

     /* SPR 19752: The Printing traces for missing fields added */

    if( ICF_SET_CALL_PRM_MULTIPLE_ANSWER_ENABLE & p_set_call->bit_mask )
    {
	     p_sprintf_count += 
             ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmultiple_answer_enable = %u",
                p_set_call->multiple_answer_enable));
    }
 
    if( ICF_SET_CALL_PRM_DNS_MIN_TIMER & p_set_call->bit_mask )
    {
	     p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ndns_min_retrans = %u",
                p_set_call->dns_min_retrans));
    }
    if( ICF_SET_CALL_PRM_DNS_MAX_TIMER & p_set_call->bit_mask )
    {
	     p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ndns_max_retrans = %u",
                p_set_call->dns_max_retrans));
    }

    if (ICF_SET_CALL_PRM_DHCP_RESPONSE_TIMER & p_set_call->bit_mask)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nDHCP Response Timer = %u",
                        p_set_call->dhcp_response_timer));
    }

    if (ICF_SET_CALL_PRM_REG_EVENT_SUBS_REQD & p_set_call->bit_mask)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nReg Event Subs Reqd = %u",
                        p_set_call->reg_event_subs_reqd));
    }

    if (ICF_SET_CALL_PRM_MEDIA_TYPE_REQD & p_set_call->bit_mask)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nMedia Transport Type = %u",
                        p_set_call->media_transport_type));
    }

    if (ICF_SET_CALL_PRM_PRECONDITION_REQD & p_set_call->bit_mask)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPrecondition Reqd = %u",
                        p_set_call->precondition_required));
    }

    if (ICF_SET_CALL_PRM_SRTP_FALLBACK_FLAG & p_set_call->bit_mask)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSRTP Fallback required = %u",
                        p_set_call->srtp_fallback_reqd));
    }

    if(ICF_SET_CALL_PRM_SRTP_FALLBACK_RESP_CODES & p_set_call->bit_mask)
    {
        p_temp_node = p_set_call->p_srtp_fallback_resp_codes;
        while (ICF_NULL != p_temp_node)
        {
            p_sprintf_count += 
                ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSRTP Failure response code = %u",
                            *(icf_uint16_t*)p_temp_node->p_data))
            p_temp_node = p_temp_node->p_next;
        }
   
    }

    /* SPR 19752 : Instead of str ,access_network_info field was printed as string
     * the problem is corrected to print the string value
     */  
    /*Trace for access_network_info header*/
    if(ICF_SET_CALL_PRM_ACCESS_NW_INFO & p_set_call->bit_mask)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\naccess_network_info header = "));

        for(i=0;(i<p_set_call->access_network_info.str_len) && (i < ICF_MAX_STR_LEN);i++)
        {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
                        p_set_call->access_network_info.str[i]));
        }
    }

    if (p_set_call->bit_mask_1 & ICF_SET_CALL_PRM_APP_PORT_CONFIG)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\napp_port_config = %u",
                        p_set_call->app_port_config));
    }

    if (ICF_SET_PORT_IN_FROM_HDR & p_set_call->bit_mask_1)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnport_to_be_sent_in_from_hdr = %u",
                        p_set_call->port_to_be_sent_in_from_hdr));
    }

    /* SDP Tunnelling Enhancements */
    if (p_set_call->bit_mask_1 & ICF_SET_CALL_PRM_EARLY_INC_SESSION_IND_REQD)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nearly_incoming_session_ind_reqd = %u",
                        p_set_call->early_incoming_session_ind_reqd));
    }

    if (p_set_call->bit_mask_1 & 
			ICF_SET_CALL_PRM_UPDATE_FOR_OFFERANSWER_AFTER_CONNECT)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nupdate for offer answer afetr connect = %u",
                        p_set_call->update_for_offeranswer_after_connect));
    }

    if (p_set_call->bit_mask_1 & 
			ICF_SET_CALL_PRM_RETRY_CODEC_COMMIT_REQ)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nretry codec commit request = %u",
                        p_set_call->retry_codec_commit_req));
    }

    if (p_set_call->bit_mask_1 & 
			ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\napp_connect_reqd_for_replaces = %u",
                        p_set_call->app_connect_reqd_for_replaces));
    }

    /* Added for asynchronous NOTIFY support */
    if( ICF_SET_CALL_PRM_ASYNC_NOTIFY_SUPPORT_REQD & p_set_call->bit_mask_1 )
    {
  	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\nasync_notify_support_reqd = %u",
                p_set_call->async_notify_support_reqd));
    }
    
    /* CSR-1-6976670 Merged */
    if (p_set_call->bit_mask_1 & 
			ICF_SET_CALL_PRM_CHECK_REMOTE_CODEC_LIST)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ncheck_remote_codec_list = %u",
                        p_set_call->check_remote_codec_list));
    }

    /* CSR_1_7113139 Merged SPR 19672 Start */
    if (p_set_call->bit_mask_1 & 
			ICF_SET_CALL_PRM_INC_SDP_VERSION_WHEN_NO_CHANGE)
    {
        p_sprintf_count += 
            ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ninc_sdp_version_when_no_change = %u",
                        p_set_call->inc_sdp_version_when_no_change));
    }
    /* CSR_1_7113139 Merged SPR 19672 End */

    /* Added for asynchronous NOTIFY support */
    if(ICF_SET_CALL_PRM_ASYNC_MESSAGE_SUPPORT_REQD & p_set_call->bit_mask_1 )
    {
  	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\nasync_message_support_reqd = %u",
                p_set_call->async_message_support_reqd));
    }

    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_set_line_param_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_uint32_t i = 0,j = 0;
    icf_list_st *p_codec_info = NULL;
    icf_list_st *p_encoded_att = NULL;

    icf_set_line_param_req_st* p_set_line = NULL;
    
    p_set_line = (icf_set_line_param_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_set_line)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_set_line is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \nline id = %d, ",p_set_line->bitmask,p_set_line->line_id))

	if (p_set_line->bitmask & ICF_SET_LINE_PRM_SUBS_HS_TIMER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"subscribe head start timer = %d, ",p_set_line->subscribe_head_start_timer))
	}

	if (p_set_line->bitmask & ICF_SET_LINE_PRM_T1_TIMER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"SIP TImer T1 = %d, ",p_set_line->sip_timer_t1))
	}

	if (p_set_line->bitmask & ICF_SET_LINE_PRM_T2_TIMER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"SIP TImer T2 = %d, ",p_set_line->sip_timer_t2))
	}

	if (p_set_line->bitmask & ICF_SET_LINE_PRM_INV_TIMER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"SIP TImer B (invite) = %d, ",p_set_line->sip_timer_invite))
	}
	
	if (p_set_line->bitmask & ICF_SET_LINE_PRM_NON_INV_TIMER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSIP TImer F = %d ",p_set_line->sip_timer_noninvite))
        }

    if (p_set_line->bitmask & ICF_SET_LINE_PRM_T4_TIMER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"SIP TImer T4 = %d, ",p_set_line->sip_timer_t4))
	}

    if (p_set_line->bitmask & ICF_SET_LINE_SIP_DSCP)
    {
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSIP DSCP = %d",p_set_line->sip_dscp))
    }
    if (p_set_line->bitmask & ICF_SET_LINE_RTP_DSCP)
    {
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nRTP DSCP = %d",p_set_line->rtp_dscp))
    }

#ifdef ICF_SESSION_TIMER
	if (p_set_line->bitmask & ICF_SET_LINE_SESSION_TIMER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession timer = %d ",p_set_line->session_timer))
    	}
	
	if (p_set_line->bitmask & ICF_SET_LINE_MIN_SE)
	{
    	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmin se = %d ",p_set_line->min_se))
    	}
	if (p_set_line->bitmask & ICF_SET_LINE_SESSION_TIMER_REFRESHER)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession timer refresher = %d ",p_set_line->session_timer_refresher))
    	}
#endif

    if ( p_set_line->bitmask & ICF_SET_LINE_AUDIO_CODEC)
    {
        p_codec_info = p_set_line->p_codec_attributes_audio;
        i = 1;
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nAUDIO CODECS "));

		while(p_codec_info != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_codec_info->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_codec_info->p_data is NULL.\n"))
                return p_sprintf_count;
            }
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec %d \nCodec Num = %d \nCodec Name = ",i,((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));

			for(j=0;(j<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len) && (j<ICF_MAX_SHORT_STR_LEN);j++)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

		  p_sprintf_count = icf_cmn_trace_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
		
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes for codec"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," "));
                
                /*Fix For SPR : 18999*/
                if (ICF_NULL == p_encoded_att->p_data)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                    return p_sprintf_count;
                }
                for(i=0;(i<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (i< ICF_MAX_STR_LEN) ;i++)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n"));

				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}

     }
     if ( p_set_line->bitmask & ICF_SET_LINE_VIDEO_CODEC)
     {
        p_codec_info = p_set_line->p_codec_attributes_video;
	    i = 1;
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nVIDEO CODECS"));
		while(p_codec_info != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_codec_info->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_codec_info->p_data is NULL.\n"))
                return p_sprintf_count;
            }            
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec %d \nCodec Num = %d \nCodec Name = ",i,((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));
			for(j=0;(j<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len) && (j< ICF_MAX_SHORT_STR_LEN) ;j++)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

		        p_sprintf_count = icf_cmn_trace_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
		
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes for codec"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," "));
                /*Fix For SPR : 18999*/
                if (ICF_NULL == p_encoded_att->p_data)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                    return p_sprintf_count;
                }

				for(i=0;(i<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (i< ICF_MAX_STR_LEN);i++)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}

				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n"));
				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
        }	
      
     }
     /* SPR 19886 : Additional fix done while merging CSR 1-7334184 */
     if (ICF_SET_LINE_REMOVE_ROUTE_HDR & p_set_line->bitmask)
 	 {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    "\nremove_route_hdr = %d ",p_set_line->remove_route_hdr))
     }
     /*Fix for SPR: 19999 (While merging CSR 1-7547597)*/
     if (ICF_SET_LINE_SESSION_REFRESH_METHOD_STRICT_UPDATE & p_set_line->bitmask)
 	 {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    "\nsession_refresh_method_strict_update = %d ",p_set_line->session_refresh_method_strict_update))
     }
     /*Fix for CSR-9012191 SPR : 20697 */
     if (ICF_SET_LINE_STRIP_REQ_URI_PARAM & p_set_line->bitmask)
 	 {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    "\nstrip_req_uri_param = %d ",p_set_line->strip_req_uri_param))
     }

     return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_set_self_id_params(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_cfg_cc_set_self_id_params_req_st* p_set_self = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_set_self = (icf_cfg_cc_set_self_id_params_req_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_set_self)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_set_self is NULL.\n"))
        return p_sprintf_count;
    }       
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\npresence_flag = %d \nline_id = %d, action = %d, \nself_addr_type = %d, self_addr_val = %s, \nself_name = %s) \ndefault_settings = %d \nauthentication_key = %s \nauthentication_password = %s \nconfigured_realm = %s",p_set_self->bit_mask, p_set_self->line_id, p_set_self->action, p_set_self->self_addr.addr_type, p_set_self->self_addr.addr_val.str, p_set_self->self_name.str, p_set_self->default_settings, p_set_self->authentication_key.str, p_set_self->authentication_password.str, p_set_self->configured_realm.str));
    if( ICF_SET_SLF_ID_PRM_PRIORITY & p_set_self->bit_mask)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPriority = %d",p_set_self->priority));
    } 
 
    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_set_reg_params(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

   
    icf_uint8_t addr_type = 0;

    icf_cfg_cc_set_registration_params_req_st* p_set_reg = NULL;


    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_set_reg = (icf_cfg_cc_set_registration_params_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_set_reg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_set_reg is NULL.\n"))
        return p_sprintf_count;
    }   

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n \n line_id = %d, \n action = %d, \n registrar_addr = addr_type = %d, ",p_set_reg->line_id, p_set_reg->action, p_set_reg->registrar_addr.addr.addr_type));

    addr_type = p_set_reg->registrar_addr.addr.addr_type;
    
    switch (addr_type) {
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s, ",p_set_reg->registrar_addr.addr.addr.domain.str));
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d, ",p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_1,p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_2,p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_3,p_set_reg->registrar_addr.addr.addr.ipv4_addr.octet_4));
        break;
        
    }
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"port_num = %d),\npresence_flag = %d,reg_mode = %d) \nregistration_duration = %d, ",p_set_reg->registrar_addr.port_num, p_set_reg->bit_mask, p_set_reg->reg_mode, p_set_reg->registration_duration));
    
	if ( p_set_reg->bit_mask & ICF_SET_REG_HS_DURATION)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nregistraion head start duration = %d, ",p_set_reg->register_head_start_timer));
	}
    
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_set_service_params(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_cfg_cc_set_service_params_req_st* p_ser_prm_msg = NULL;

    icf_uint32_t presence_flag = 0,presence_flag2 = 0;
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

     
    p_ser_prm_msg = (icf_cfg_cc_set_service_params_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_ser_prm_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_ser_prm_msg is NULL.\n"))
        return p_sprintf_count;
    }     
    presence_flag = p_ser_prm_msg->bit_mask;
    
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsubscription_flag =(line_id = %d , status = %d )",p_ser_prm_msg->service_subscription_flag.line_id,p_ser_prm_msg->service_subscription_flag.status));
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nactivattion_flag = (line_id = %d , status = %d )",p_ser_prm_msg->service_activation_flag.line_id,p_ser_prm_msg->service_activation_flag.status));
    }
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ncall_hold_option =(hold_option= <%d>,line_id= <%d>",p_ser_prm_msg->call_hold_option.hold_option,p_ser_prm_msg->call_hold_option.line_id));
    }
    
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_MAX_NUM_OF_HOPS)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmax_num_of_calls_forwarding_hops = %d",p_ser_prm_msg->max_num_of_call_forwarding_hops));
    }
    
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_MAX_CALL_WAITING)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\nmax_num_of_calls_in_call_waiting = %d",  
                    p_ser_prm_msg->max_num_of_calls_in_call_waiting));
    }

    
    presence_flag2 = 0;
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CFU_ADDR)
    {
    
    
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
             "\naddr_for_cfu = ( line_id = %d ",
               p_ser_prm_msg->addr_for_cfu.line_id)); 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_val = %s",  
                    p_ser_prm_msg->addr_for_cfu.address.addr_val.str));
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_type = %d",    
                    p_ser_prm_msg->addr_for_cfu.address.addr_type));
    
    }    

   
    presence_flag2 = 0;
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CFB_ADDR)
    {
    
    
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
             "\naddr_for_cfb = ( line_id = %d ",
               p_ser_prm_msg->addr_for_cfb.line_id)); 
    
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_val = %s",  
                    p_ser_prm_msg->addr_for_cfb.address.addr_val.str));
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_type = %d",    
                    p_ser_prm_msg->addr_for_cfb.address.addr_type));
    
    }    

    
    
    presence_flag2 = 0;
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CFD_ADDR)
    {
    
    
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
             "\naddr_for_cfd = ( line_id = %d ",
               p_ser_prm_msg->addr_for_cfd.line_id)); 
    
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_val = %s",  
                    p_ser_prm_msg->addr_for_cfd.address.addr_val.str));
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_type = %d",    
                    p_ser_prm_msg->addr_for_cfd.address.addr_type));
    
    }    

    
    if ( presence_flag & ICF_CFG_CC_SET_SER_PRM_CHR_TIMER)
    {
    
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\ncall_hold_reume_timer = %d",
                  p_ser_prm_msg->call_hold_resume_timer));
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_REINVITE_RACE_TIMER)
    {
    
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
              "\nreinvite_race_timer = %d",
                p_ser_prm_msg->reinvite_race_timer));
    }
    
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_TWC_TIMER)
    {

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "twc_timer = %d",
                  p_ser_prm_msg->twc_timer));
             
     }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CTU_TIMER)    
    {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\nctu_timer = %d",
                        p_ser_prm_msg->ctu_timer));
          
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CTA_TIMER)  
    {
    
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\nrnw_timer = %d",
                      p_ser_prm_msg->rnw_timer));
    }

                          
    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CW_ALERT_TIMER)  
    {
    
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\ncw_alert_timer = %d",
                      p_ser_prm_msg->cw_alert_timer));
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CND_ALERT_TIMER)  
    {
    
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\ncnd_alert_timer = %d",
                      p_ser_prm_msg->cnd_alert_timer));
    }

    if (presence_flag & ICF_CFG_CC_SET_SER_PRM_CW_N_CND_ALERT_TIMER)  
    {
    
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\ncw_n_cnd_alert_timer = %d",
                      p_ser_prm_msg->cw_n_cnd_alert_timer));
     }

     if (presence_flag & ICF_CFG_CC_SET_SER_PRM_SUBS_RESP_TIMER)  
     {
    
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\nsubscribe_resp_timer = %d",
                     p_ser_prm_msg->subs_resp_timer));
      }

     if (presence_flag & ICF_CFG_CC_SET_SER_PRM_SUBS_RETRY_TIMER)  
     {
    
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\nsubscribe_retry_timer = %d",
                      p_ser_prm_msg->subs_retry_timer));
      }

     if(ICF_SET_SER_PRM_CONFERENCE_URI_PRESENT & presence_flag)
     {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
         	"\nConference Uri  = ( line_id = %d ",
	p_ser_prm_msg->conference_uri.line_id));
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_val = %s",
        p_ser_prm_msg->conference_uri.address.addr_val.str));
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                  "\naddr_type = %d",
        p_ser_prm_msg->conference_uri.address.addr_type));
     }
     if(ICF_SET_SER_PRM_SUBSCRIPTION_REQD_IN_EXT_CONF & presence_flag)
     {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\nXconf_subs_duration  = %d",
                      p_ser_prm_msg->xconf_subs_duration));
     }
       
     if(ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD & presence_flag)
     {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                      "\nconf_fail_call_clear_not_reqd  = %d",
                      p_ser_prm_msg->conf_fail_call_clear_not_reqd));
     }    
 
    return p_sprintf_count;
}







icf_uint8_t* icf_cmn_trace_incoming_call(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_cc_pa_incoming_call_ind_st* p_incoming_call = NULL;

    icf_list_st *p_list=ICF_NULL;    
    
    icf_uint8_t  header_list_count = 0;
    icf_uint8_t  body_list_count = 0,i=0;
#ifdef ICF_FOR_LOAD_TESTING
	icf_uint8_t command_string[ICF_MAX_BUFF_LEN];
#endif
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_incoming_call = (icf_cc_pa_incoming_call_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/    
    if (ICF_NULL == p_incoming_call) 
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_incoming_call is NULL.\n"))
        return p_sprintf_count;
    }
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_incoming_call->bit_mask));
	

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "line_id = %d, call_type = %d", 
              p_incoming_call->line_id, p_incoming_call->call_type));


    if (p_incoming_call->bit_mask & ICF_CALL_WAITING_IND_PRESENT)     
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\ncall_waiting_ind = %d",
            p_incoming_call->call_waiting_ind));

    if (p_incoming_call->bit_mask & ICF_CALLING_ADD_ASRTD_IND_PRESENT)     
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            " ,calling_party_addr_asserted = %d,",
            p_incoming_call->calling_party_addr_asserted));


    if (p_incoming_call->bit_mask & ICF_CALLING_PARTY_ADD_PRESENT)     
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "calling_party_addr = (addr_type = %d, %s)" ,
              p_incoming_call->calling_party_addr.addr_type,
              p_incoming_call->calling_party_addr.addr_val.str));

    if (p_incoming_call->bit_mask & ICF_CALLING_PARTY_NAME_PRESENT)     
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ncalling_party_name = %s",
              p_incoming_call->calling_party_name.str));
             
    if (p_incoming_call->bit_mask & ICF_TRANSF_PARTY_ADD_PRESENT) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\ntransfering_party_addr = (addr_type = %d, %s)",
            p_incoming_call->transferring_party_addr.addr_type,
            p_incoming_call->transferring_party_addr.addr_val.str));

    if (p_incoming_call->bit_mask & ICF_TRANSF_PARTY_NAME_PRESENT) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\ntransfering_party_name = %s", 
            p_incoming_call->transferring_party_name.str));        

    if (p_incoming_call->bit_mask & ICF_CALLED_PARTY_ADD_PRESENT)     
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\ncalled_party_addr = (addr_type = %d, %s)",
            p_incoming_call->called_party_addr.addr_type,
            p_incoming_call->called_party_addr.addr_val.str));
    
    
/* Header LIST  */
    if (p_incoming_call->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_incoming_call->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_incoming_call->header_list.hdr, header_list_count,p_sprintf_count);
    }

/* Body LIST  */
    if (p_incoming_call->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_incoming_call->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_incoming_call->body_list.body, body_list_count,p_sprintf_count);
    }

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Accepted Stream List:"));

	i = 1;
        
        p_list=p_incoming_call->p_accepted_stream_list;

	while(p_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_list->p_data,p_sprintf_count);
		
		p_list = p_list->p_next;
		
		i++;
	}


	if(p_incoming_call->bit_mask & ICF_REJECTED_MEDIA_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n rejected stream count %d:",p_incoming_call->count));	

		for(i=0;(i<p_incoming_call->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",p_incoming_call->deleted_stream_list[i].stream_type));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",p_incoming_call->deleted_stream_list[i].stream_id));
		}
	}

	if(ICF_INVALID_CALL_ID != p_incoming_call->call_id_to_be_joined )
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Call to be Joined =  %d:",
		p_incoming_call->call_id_to_be_joined));
	}

#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_USER_ALERTED_MSG);
    icf_command_handler(command_string);
    strcpy(command_string,ICF_DEFAULT_CONNECT_REQ_MSG);
    icf_command_handler(command_string);

#endif



    return p_sprintf_count;

}






icf_uint8_t* icf_cmn_trace_create_call(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_pa_cc_create_call_req_st* p_create_call = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	*p_pref_media_profile = ICF_NULL;

    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_create_call = (icf_pa_cc_create_call_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_create_call)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_create_call is NULL.\n"))
        return p_sprintf_count;
    }
     
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_create_call->bit_mask));
     
 
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "called_party_addr = (addr_type = %d, %s)"
              "\ncalled_party_name = %s",
              p_create_call->called_party_addr.addr_type,
              p_create_call->called_party_addr.addr_val.str,
              p_create_call->called_party_name.str));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ntoggle_presentation_indicator = %d, qos_mark = %d",
              p_create_call->toggle_presentation_indicator, 
              p_create_call->qos_mark));

/* Header LIST  */    if (p_create_call->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_create_call->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_create_call->header_list.hdr, header_list_count,p_sprintf_count);
    }


    if (p_create_call->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_create_call->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_create_call->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
        if (p_create_call->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_create_call->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_create_call->body_list.body, body_list_count,p_sprintf_count);
    }
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media Profile List:"));

	i = 1;

	p_pref_media_profile = p_create_call->p_pref_media_profile;

	while(p_pref_media_profile!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_pref_media_profile->p_data,p_sprintf_count);
		
		p_pref_media_profile = p_pref_media_profile->p_next;
		
		i++;
	}





	

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 

              "calling_party_addr = (addr_type = %d, %s)"

              "\ncalling_party_name = %s",

              p_create_call->calling_party_address.addr_type,

              p_create_call->calling_party_address.addr_val.str,

              p_create_call->calling_party_name.str));



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nprivacy_hdr_all_req = %d",
              p_create_call->privacy_hdr_all_req));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nres_reservation_reqd = %d",
              p_create_call->resource_reservation_reqd));

#ifdef ICF_SESSION_TIMER   
   if(ICF_CREATE_CALL_SESSION_TIMER_ENABLE_PRESENT & p_create_call->bit_mask)
   { 
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nsession timer enable = %d",
              p_create_call->session_timer_enable));
   }
#endif

    if (ICF_CREATE_CALL_SETTINGS_PRESENT & p_create_call->bit_mask)
    { 
        if(ICF_EMERGENCY_CALL & p_create_call->call_settings)
        {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
                        "\nEmergency Call = True"));
        }
    }

    /* redirect_support_reqd */

    if (ICF_CREATE_CALL_REDIRECT_SUPPORT_REQD & p_create_call->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nredirect_support_reqd = %d",
              p_create_call->redirect_support_reqd));
    }

    return p_sprintf_count;
}







icf_uint8_t* icf_cmn_trace_alert_user(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_cc_pa_alert_user_ind_st* p_alert_user = NULL;

    icf_list_st *p_list=ICF_NULL;
    
    icf_uint8_t  header_list_count = 0;  
    icf_uint8_t  body_list_count = 0,i=0;
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_alert_user = (icf_cc_pa_alert_user_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_alert_user)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_alert_user is NULL.\n"))
        return p_sprintf_count;
    }


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_alert_user->bit_mask));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ninband_info_status = %d", 
              p_alert_user->inband_info_status));

    
/* Header LIST  */
    if (p_alert_user->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_alert_user->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_alert_user->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
       
    /* Body LIST */
    if (p_alert_user->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_alert_user->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_alert_user->body_list.body, body_list_count,p_sprintf_count);
    }
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Accepted Stream List:"));

	i = 1;
	p_list=p_alert_user->p_accepted_stream_list;

	while(p_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_list->p_data,p_sprintf_count);
		
		p_list = p_list->p_next;
		
		i++;
	}


	if(p_alert_user->bit_mask & ICF_REM_USER_ALRTED_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Deleted stream count %d:",p_alert_user->count));	

		for(i=0;(i<p_alert_user->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",p_alert_user->deleted_media_streams[i].stream_type));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",p_alert_user->deleted_media_streams[i].stream_id));
		}
	}

      /* CSR 1-8780514 SPR 20517 */
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nis_sdp_received = %d", 
              p_alert_user->is_sdp_received));


    return p_sprintf_count;

}







icf_uint8_t* icf_cmn_trace_user_alerted(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_pa_cc_user_alerted_req_st* p_alert_ind = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;	
    icf_list_st *p_stream_list;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_alert_ind = (icf_pa_cc_user_alerted_req_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_alert_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_alert_ind is NULL.\n"))
        return p_sprintf_count;
    }    
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ninband_info_status = %d",
              p_alert_ind->inband_info_status));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nearly_media_reqd = %d",
              p_alert_ind->early_media_reqd));
    


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
        "\nprivacy_ind = %d", p_alert_ind->privacy_ind));

	if (p_alert_ind->bit_mask & ICF_USER_ALERTED_ALERT_TIMER_PRESENT) {      
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
        "\nAlert timer duration = %d", p_alert_ind->alert_timer_duration));
    }

/* Header LIST  */    if (p_alert_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_alert_ind->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_alert_ind->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    
    /* Tag LIST  */    if (p_alert_ind->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_alert_ind->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_alert_ind->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_alert_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_alert_ind->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_alert_ind->body_list.body, body_list_count,p_sprintf_count);
    }
    
	if(p_alert_ind->bit_mask & ICF_STREAM_LIST_PRESENT)
	{

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
	        "\nMedia Profile List:"));
		p_stream_list= p_alert_ind->p_stream_list;
		i = 1;
		while(p_stream_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
			p_sprintf_count = icf_cmn_trace_stream_capab(p_stream_list->p_data,p_sprintf_count);
			p_stream_list = p_stream_list->p_next;
			i++;
		}
	}

	if(p_alert_ind->bit_mask & ICF_USER_ALERTED_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Deleted stream count: %d",p_alert_ind->count));	

		for(i=0;(i<p_alert_ind->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type: %d",p_alert_ind->deleted_media_streams[i].stream_type));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id: %d",p_alert_ind->deleted_media_streams[i].stream_id));
		}
	}

	if( ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED & p_alert_ind->bit_mask)
        {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Early Mode = %d",p_alert_ind->early_mode));
	}
	if(ICF_USER_ALERTED_MEDIA_MODE_PRESENT  & p_alert_ind->bit_mask)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media Mode = %d",p_alert_ind->media_mode));
	}






    return p_sprintf_count;
}





icf_uint8_t* icf_cmn_trace_term_call_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_pa_cc_terminate_call_req_st* p_terminate_call_req = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_terminate_call_req = (icf_pa_cc_terminate_call_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_terminate_call_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_terminate_call_req is NULL.\n"))
        return p_sprintf_count;
    } 
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ncall_clear_reason = %d"
              "\npresence_flag = %d, line_id = %d",
              p_terminate_call_req->call_clear_reason,
              p_terminate_call_req->bit_mask,
              p_terminate_call_req->line_id));

   if(ICF_CALL_CLEAR_RESPONSE_CODE_PRESENT & p_terminate_call_req->bit_mask)
   {
       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
           "\nresponse_code = %d",p_terminate_call_req->response_code));
   }

   if (p_terminate_call_req->bit_mask & ICF_HDR_LIST_PRESENT) 
   {
        p_sprintf_count = icf_cmn_trace_header_list(p_terminate_call_req->header_list.hdr, 
                p_terminate_call_req->header_list.count,p_sprintf_count);
   }
       
   if (p_terminate_call_req->bit_mask & ICF_TAG_LIST_PRESENT) 
   {
        p_sprintf_count = icf_cmn_trace_tag_list(p_terminate_call_req->tag_list.tag, 
                p_terminate_call_req->tag_list.count,p_sprintf_count);
   }

   if (p_terminate_call_req->bit_mask & ICF_BODY_LIST_PRESENT) 
   {
        p_sprintf_count = icf_cmn_trace_body_list(p_terminate_call_req->body_list.body, 
                p_terminate_call_req->body_list.count,p_sprintf_count);
   }   
   
   return p_sprintf_count;
}






icf_uint8_t* icf_cmn_trace_term_call_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_cc_pa_terminate_call_ind_st* p_terminate_call = NULL;
#ifdef ICF_FOR_LOAD_TESTING
	icf_uint8_t command_string[ICF_MAX_BUFF_LEN];
#endif

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_terminate_call = (icf_cc_pa_terminate_call_ind_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_terminate_call)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_terminate_call is NULL.\n"))
        return p_sprintf_count;
    }    
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ncall_clear_reason = %d",
              p_terminate_call->call_clear_reason));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nadditional_info.str = %s", p_terminate_call->additional_info.str));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nresponse code = %d", p_terminate_call->response_code));

    if(ICF_RETRY_AFTER_DURATION_PRESENT & p_terminate_call->bit_mask)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
              "\nRetry After duration = %d", p_terminate_call->retry_after_duration));
    }

   if (p_terminate_call->bit_mask & ICF_HDR_LIST_PRESENT) 
   {
        p_sprintf_count = icf_cmn_trace_header_list(p_terminate_call->header_list.hdr, 
                p_terminate_call->header_list.count,p_sprintf_count);
   }
        
   if (p_terminate_call->bit_mask & ICF_BODY_LIST_PRESENT) 
   {
        p_sprintf_count = icf_cmn_trace_body_list(p_terminate_call->body_list.body, 
                p_terminate_call->body_list.count,p_sprintf_count);
   } 

#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_TERM_CALL_CFM_MSG);
    icf_command_handler(command_string);
    t_calls++;
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nTerminated_Calls:%d",t_calls));

#endif

    return p_sprintf_count;
}



icf_uint8_t* icf_cmn_trace_get_codec_list_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_get_codec_list_resp_st* p_gcl_resp_msg = NULL;

    icf_uint8_t i = 0;    
    icf_list_st *p_reserved_stream_list = ICF_NULL;
      
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
               
    p_gcl_resp_msg = (icf_get_codec_list_resp_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_gcl_resp_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_gcl_resp_msg is NULL.\n"))
        return p_sprintf_count;
    }    

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n result = %d", p_gcl_resp_msg->result));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bitmask = %d", p_gcl_resp_msg->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n error = %d", p_gcl_resp_msg->error_cause));
       
        p_sprintf_count = icf_cmn_trace_error_details(p_gcl_resp_msg->error_cause,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Reserved Stream List:"));

	i = 1;

	p_reserved_stream_list = p_gcl_resp_msg->p_reserved_stream_list;

	while(p_reserved_stream_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_reserved_stream_list->p_data,p_sprintf_count);
		
		p_reserved_stream_list = p_reserved_stream_list->p_next;
		
		i++;
	}

	if(p_gcl_resp_msg->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n count = %d", p_gcl_resp_msg->rejected_stream_list.count));

		for(i=0;(i<p_gcl_resp_msg->rejected_stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL) ;i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream_id = %d", p_gcl_resp_msg->rejected_stream_list.stream_id[i]));

		}
	}





    return p_sprintf_count;
}




icf_uint8_t* icf_cmn_trace_get_codec_list_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
  
    icf_get_codec_list_req_st* p_gcl_req_msg = NULL;
    
    icf_uint8_t i = 0;
        
    icf_list_st	*p_stream_capab_list = ICF_NULL;  

#ifdef ICF_FOR_LOAD_TESTING
	icf_uint8_t command_string[ICF_MAX_BUFF_LEN];
#endif


    p_gcl_req_msg = (icf_get_codec_list_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_gcl_req_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_gcl_req_msg is NULL.\n"))
        return p_sprintf_count;
    }    

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "line_id = %d", p_gcl_req_msg->line_id));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nMedia Profile List:"));

	i = 1;

	p_stream_capab_list = p_gcl_req_msg->p_stream_capab_list;

	while(p_stream_capab_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_stream_capab_list->p_data,p_sprintf_count);
		
		p_stream_capab_list = p_stream_capab_list->p_next;
		
		i++;
	}

	if (ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT ==(ICF_LOCAL_STREAM_CAPAB_INFO_PRESENT & p_gcl_req_msg->bit_mask))
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nLocal Media Profile List:\n"));
		
		i = 1;
		
		p_stream_capab_list = p_gcl_req_msg->p_local_stream_capab_list;
		
		while(p_stream_capab_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
			
			p_sprintf_count = icf_cmn_trace_stream_capab(p_stream_capab_list->p_data,p_sprintf_count);
			
			p_stream_capab_list = p_stream_capab_list->p_next;
			
			i++;
		}
	}


#ifdef ICF_FOR_LOAD_TESTING

	strcpy(command_string,ICF_DEFAULT_GCL_RSP_MSG);
    app_global_data.call_id = p_msg->hdr.call_id;
    icf_command_handler(command_string);

#endif

    return p_sprintf_count;


}

icf_uint8_t* icf_cmn_trace_codec_commit_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
      
    icf_codec_commit_req_st* p_gcc_req_msg = NULL;
	icf_list_st *p_list = ICF_NULL;
    
    	icf_uint32_t i=0;
#ifdef ICF_FOR_LOAD_TESTING
	icf_uint8_t command_string[ICF_MAX_BUFF_LEN];
#endif


    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
  
    p_gcc_req_msg = (icf_codec_commit_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_gcc_req_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_gcc_req_msg is NULL.\n"))
        return p_sprintf_count;
    }    

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nMedia Profile List:"));
	
	i = 1;
	 
	p_list = p_gcc_req_msg->p_stream_commit_list;

	while(p_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
	
		p_sprintf_count = icf_cmn_trace_stream_capab(p_list->p_data,p_sprintf_count);
		
		p_list = p_list->p_next;
		
		i++;
	}



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_CODEC_COMMIT_RSP_MSG);
    icf_command_handler(command_string);

#endif

    return p_sprintf_count;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_open_channel_req
 *
 * DESCRIPTION:     This function is used to trace open_channel_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_open_channel_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_uint8_t i = 0;

    icf_open_media_channel_req_st* p_open_channel_req = ICF_NULL;

	icf_list_st* p_channel_list = ICF_NULL;

    p_open_channel_req = (icf_open_media_channel_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_open_channel_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_open_channel_req is NULL.\n"))
        return p_sprintf_count;
    }    

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nChannel List:"));

	i = 1;

	p_channel_list = p_open_channel_req->p_channel_list;

	while(p_channel_list!=ICF_NULL)
	{
        /*Fix For SPR : 18999*/
        if (ICF_NULL == p_open_channel_req)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_channel_list->p_data is NULL.\n"))
            return p_sprintf_count;
        }          
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Type:%d",
					((icf_channel_st*)(p_channel_list->p_data))->stream_type));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id:%d",
					((icf_channel_st*)(p_channel_list->p_data))->stream_id));

		p_channel_list = p_channel_list->p_next;
		
		i++;
	}


	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_open_media_channel_resp
 *
 * DESCRIPTION:     This function is used to trace open_media_channel_resp
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_open_media_channel_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i = 0;

    icf_open_media_channel_resp_st* p_omc_resp = ICF_NULL;

	icf_list_st* p_channel_addr_list = ICF_NULL;

    p_omc_resp = (icf_open_media_channel_resp_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_omc_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_omc_resp is NULL.\n"))
        return p_sprintf_count;
    }    

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nResult:%d",
					p_omc_resp->result));

	
	if(!p_omc_resp->result)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nError Cause:%d",
					p_omc_resp->error_cause));
		p_sprintf_count = icf_cmn_trace_error_details(p_omc_resp->error_cause,p_sprintf_count);
	}
	if(p_omc_resp->result)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nChannel List:"));

		i = 1;

		p_channel_addr_list = p_omc_resp->p_channel_addr_list;

		while(p_channel_addr_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_channel_addr_list->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_channel_addr_list->p_data is NULL.\n"))
                return p_sprintf_count;
            }            

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Type:%d",
					((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_type));

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id:%d",
					((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_id));

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Stream Address "));
			p_sprintf_count = icf_cmn_trace_transport_addr((icf_transport_addr_st*)&(((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_address.addr),p_sprintf_count);
    
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\n stream_info.port = %d", ((icf_channel_addr_st*)(p_channel_addr_list->p_data))->stream_address.port_num));

			p_channel_addr_list = p_channel_addr_list->p_next;
		
			i++;
		}
	}


	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_close_media_channel_req
 *
 * DESCRIPTION:     This function is used to trace 
 *							close_media_channel_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_close_media_channel_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_uint8_t i = 0;

    icf_close_media_channel_req_st* p_close_channel_req = ICF_NULL;

	icf_list_st* p_channel_list = ICF_NULL;

    p_close_channel_req = (icf_close_media_channel_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_close_channel_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_close_channel_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

 	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nChannel List:"));

	i = 1;

	p_channel_list = p_close_channel_req->p_channel_list;

	while(p_channel_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

        /*Fix For SPR : 18999*/
        if (ICF_NULL == p_channel_list->p_data)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_channel_list->p_data is NULL.\n"))
            return p_sprintf_count;
        }            

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Type:%d",
					((icf_channel_st*)(p_channel_list->p_data))->stream_type));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id:%d",
					((icf_channel_st*)(p_channel_list->p_data))->stream_id));

		p_channel_list = p_channel_list->p_next;
		
		i++;
	}


	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_send_udp_data_req
 *
 * DESCRIPTION:     This function is used to trace 
 *							send_udp_data_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_send_udp_data_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_send_udp_data_req_st* p_send_udp_data_req = ICF_NULL;

#ifdef ICF_NAT_MEDIA
    arStun_msg_hdr_st         stun_msg_hdr_received;
    nat_octet_string_st       *p_buffer;
    nat_string_st             password;
    arStun_ecode_et           ecode;
#endif

    p_send_udp_data_req = (icf_send_udp_data_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_send_udp_data_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_send_udp_data_req is NULL.\n"))
        return p_sprintf_count;
    }       

#ifdef ICF_NAT_MEDIA
    memset(&stun_msg_hdr_received,0,sizeof(arStun_msg_hdr_st));
    p_buffer = malloc(sizeof(nat_octet_string_st));
        p_buffer->octet_len = p_send_udp_data_req->app_data_len;
    memcpy(p_buffer->str,p_send_udp_data_req->p_app_data,p_buffer->octet_len);

    if (UTIL_FAILURE == arStun_decode_message_header((arStun_msg_buffer_st *)p_buffer,
                                                  stun_msg_hdr_received,&ecode))
         return ICF_FAILURE;

    /* storing transaction id received in the app_global data.*/ 
    memcpy(&transaction_id,&(stun_msg_hdr_received.transaction_id),sizeof(nat_uint128_t));

    free(p_buffer);
#endif 

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSource Port: %d",
										p_send_udp_data_req->source_port));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nDestination Addr:"));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nAddr type: %d",
										p_send_udp_data_req->dest_addr.addr.addr_type));

	switch(p_send_udp_data_req->dest_addr.addr.addr_type)
	{
		case  ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
			  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s", 
              p_send_udp_data_req->dest_addr.addr.addr.domain.str));
			  break;
        
		case  ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
			  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d", 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_1, 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_2, 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_3, 
              p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr.octet_4));
			  break;

		case  ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
			  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s", 
              p_send_udp_data_req->dest_addr.addr.addr.ipv6_addr));
			  break;
	}

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPort num: %d",
										p_send_udp_data_req->dest_addr.port_num));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApp data len: %d",
										p_send_udp_data_req->app_data_len));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApp data: %s",
										p_send_udp_data_req->p_app_data));


	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_recv_udp_data_ind
 *
 * DESCRIPTION:     This function is used to trace 
 *							recv_udp_data_ind
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_recv_udp_data_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_recv_udp_data_ind_st* p_recv_udp_data_req = ICF_NULL;

    p_recv_udp_data_req = (icf_recv_udp_data_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_recv_udp_data_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_recv_udp_data_req is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nDest Port: %d",
										p_recv_udp_data_req->dest_port));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSource Addr:"));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nAddr type: %d",
										p_recv_udp_data_req->source_addr.addr.addr_type));

	switch(p_recv_udp_data_req->source_addr.addr.addr_type)
	{
		case  ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
			  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s", 
              p_recv_udp_data_req->source_addr.addr.addr.domain.str));
			  break;

		case  ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
			  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d", 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_1, 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_2, 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_3, 
              p_recv_udp_data_req->source_addr.addr.addr.ipv4_addr.octet_4));
			  break;

		case  ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
			  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s", 
              p_recv_udp_data_req->source_addr.addr.addr.ipv6_addr));
			  break;
	}

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPort num: %d",
										p_recv_udp_data_req->source_addr.port_num));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApp data len: %d",
										p_recv_udp_data_req->app_data_len));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApp data: %s",
										p_recv_udp_data_req->p_app_data));

	return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_cms_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        
        icf_uint8_t i = 0;
    icf_uint16_t bit_mask = 0;
#ifdef ICF_FOR_LOAD_TESTING
	icf_uint8_t command_string[ICF_MAX_BUFF_LEN];
#endif

    
    icf_create_media_session_req_st* p_cms_req = ICF_NULL;
	icf_list_st *p_list;

    p_cms_req = (icf_create_media_session_req_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_cms_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_cms_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "presence_flag = %d, line_id = %d",
        p_cms_req->bit_mask, p_cms_req->line_id));

	p_list = p_cms_req->p_stream_list_data;

    for (i = 0; p_list != ICF_NULL; i++) {
        
        /*Fix For SPR : 18999*/
        if (ICF_NULL == p_list->p_data)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_list->p_data is NULL.\n"))
            return p_sprintf_count;
        }   
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nstream_data[%d] = ", i));

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
			((icf_req_stream_st*)(p_list->p_data))->bit_mask));

		bit_mask = ((icf_req_stream_st*)(p_list->p_data))->bit_mask;

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n\tstream_data[%d].stream_type = %d"
            "\n\tstream_data[%d].stream_id = %d", 
            i, ((icf_req_stream_st*)(p_list->p_data))->stream_type,
            i, ((icf_req_stream_st*)(p_list->p_data))->stream_id));


      if (bit_mask & ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT) {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\n\tstream_data[%d].remote_sdp", i));
            p_sprintf_count = icf_cmn_trace_stream_sdp(&((icf_req_stream_st*)(p_list->p_data))->remote_sdp,\
									((icf_req_stream_st*)(p_list->p_data))->stream_type,p_sprintf_count);
        }
		
        if (bit_mask & ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT) {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\n\tstream_data[%d].local_sdp", i));
			p_sprintf_count = icf_cmn_trace_stream_sdp(&((icf_req_stream_st*)(p_list->p_data))->local_sdp,\
									((icf_req_stream_st*)(p_list->p_data))->stream_type,p_sprintf_count);
        }

        if (bit_mask & ICF_CREATE_MEDIA_STREAM_MODE_PRESENT) 
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			            "\n\tstream_data[%d].mode = ", i));
	
		switch(((icf_req_stream_st*)(p_list->p_data))->mode)
		{
			case 0:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_INVALID"));
				  break;
			case 1:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_INACTIVE"));
				  break;
			case 2:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_SEND_ONLY"));
				  break;
			case 3:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_RECV_ONLY"));
				  break;
			case 4:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_SEND_RECV"));
				  break;

		}
	}

		if (bit_mask & ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT)
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n\tstream_data[%d].qos_mark = %d", i, ((icf_req_stream_st*)(p_list->p_data))->qos_mark));

        if (bit_mask & ICF_CREATE_MEDIA_RTP_DETECTION_PRESENT)
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n\tstream_data[%d].rtp_detection = %d", i, ((icf_req_stream_st*)(p_list->p_data))->rtp_detection_reqd));

		if (bit_mask & ICF_CREATE_MEDIA_ECHO_CAN_PRESENT)
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n\tstream_data[%d].echo_cancellation = %d", i, ((icf_req_stream_st*)(p_list->p_data))->echo_cancellation));
		
		
		p_list = p_list->p_next;
		
	     
    }



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_CMS_RSP_MSG);
    icf_command_handler(command_string);

#endif
    return p_sprintf_count;

}



icf_uint8_t* icf_cmn_trace_dms_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
     
    icf_uint8_t	i = 0;

    icf_delete_media_session_ind_st* p_dms_ind = NULL;


    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
      
    p_dms_ind = (icf_delete_media_session_ind_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_dms_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_dms_ind is NULL.\n"))
        return p_sprintf_count;
    }       

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"count = %d ",p_dms_ind->stream_list.count));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"Stream IDs "));

	for(i=0;(i<p_dms_ind->stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d. %d",i,p_dms_ind->stream_list.stream_id[i]));
	}


	if (p_dms_ind->bit_mask & ICF_DEL_MEDIA_SESSION_ERR_CAUSE_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"error_cause = %d ",p_dms_ind->error_cause));
		p_sprintf_count = icf_cmn_trace_error_details(p_dms_ind->error_cause,p_sprintf_count);
    }
    
 
        
    return p_sprintf_count;
    
}






icf_uint8_t* icf_cmn_trace_merge_media_req_msg(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
     
    icf_uint8_t	 i=0;

    icf_merge_media_session_req_st* p_mms_req = NULL;


    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    
    p_mms_req = (icf_merge_media_session_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_mms_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_mms_req is NULL.\n"))
        return p_sprintf_count;
    }  

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"merge_call_id = %d ", p_mms_req->merge_call_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"count = %d \n", p_mms_req->count));

	for(i=0; (i<p_mms_req->count) && (i<ICF_MAX_STREAM_PER_CALL) ;i++)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"stream_count = %d ", i));
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," stream_id = %d ", p_mms_req->merge_stream_id[i].stream_id));
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," merge_stream_id = %d ", p_mms_req->merge_stream_id[i].merge_stream_id));
	}

    return p_sprintf_count;
    
}



icf_uint8_t* icf_cmn_trace_cms_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i = 0;
    icf_create_media_session_resp_st* p_cms_resp = ICF_NULL;

    icf_list_st *p_resp= ICF_NULL;
    p_cms_resp = (icf_create_media_session_resp_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_cms_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_cms_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_resp=p_cms_resp->p_accepted_stream_list;


    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);




    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bitmask = %d, result = %d, error_cause = %d",
        p_cms_resp->bit_mask, p_cms_resp->result, p_cms_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_cms_resp->error_cause,p_sprintf_count);

	while(p_resp!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_resp_stream((icf_resp_stream_st*)(p_resp->p_data),i,p_sprintf_count);
		
		p_resp=p_resp->p_next;		
		i++;
	}


	if(p_cms_resp->bit_mask & ICF_CMS_REJECT_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Rejected Stream List"));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n count = %d", p_cms_resp->rejected_stream_list.count));

		for(i=0;(i<p_cms_resp->rejected_stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream_id = %d", p_cms_resp->rejected_stream_list.stream_id[i]));

		}
	}



    return p_sprintf_count;

}



icf_uint8_t* icf_cmn_trace_mms_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{    
     
    icf_modify_media_session_req_st* p_mms_req = NULL;

       icf_uint8_t i = 0, bit_mask = 0;
	icf_list_st *p_stream_data_list = ICF_NULL;
#ifdef ICF_FOR_LOAD_TESTING
	icf_uint8_t command_string[ICF_MAX_BUFF_LEN];
#endif

    p_mms_req = (icf_modify_media_session_req_st*)p_msg->payload;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_mms_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_mms_req is NULL.\n"))
        return p_sprintf_count;
    }  

	p_stream_data_list = p_mms_req->p_stream_data_list;

	for (i = 0; p_stream_data_list != ICF_NULL; i++) {
        /*Fix for SPR : 18999*/ 
        if (ICF_NULL == p_stream_data_list->p_data)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stream_data_list->p_data is NULL.\n"))
            return p_sprintf_count;
        }    

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nstream_data[%d] = ", i));

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
			((icf_modify_stream_st*)(p_stream_data_list->p_data))->bit_mask));

		bit_mask = ((icf_modify_stream_st*)(p_stream_data_list->p_data))->bit_mask;

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n\tstream_data[%d].stream_type = %d"
            "\n\tstream_data[%d].stream_id = %d", 
            i, ((icf_modify_stream_st*)(p_stream_data_list->p_data))->stream_type,
            i, ((icf_modify_stream_st*)(p_stream_data_list->p_data))->stream_id));


		if ((bit_mask & ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT) || (bit_mask & ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT)) {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\n\tstream_data[%d].stream_sdp", i));
            p_sprintf_count = icf_cmn_trace_stream_sdp(&((icf_modify_stream_st*)(p_stream_data_list->p_data))->sdp_stream,\
									((icf_modify_stream_st*)(p_stream_data_list->p_data))->stream_type,p_sprintf_count);
        }
		
        if (bit_mask & ICF_MODIFY_MEDIA_MODE_PRESENT) 
	{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			            "\n\tstream_data[%d].media_mode = ", i));
	
		switch(((icf_modify_stream_st*)(p_stream_data_list->p_data))->mode)
		{
			case 0:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_INVALID"));
				  break;
			case 1:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_INACTIVE"));
				  break;
			case 2:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_SEND_ONLY"));
				  break;
			case 3:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_RECV_ONLY"));
				  break;
			case 4:  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
					            "ICF_MEDIA_MODE_SEND_RECV"));
				  break;

		}
	}
		if (bit_mask & ICF_MODIFY_MEDIA_ECHO_CAN_PRESENT)
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n\tstream_data[%d].echo_cancellation = %d", i, 
				((icf_modify_stream_st*)(p_stream_data_list->p_data))->echo_cancellation));
		
		
		p_stream_data_list = p_stream_data_list->p_next;
		
	    
    }



#ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_MMS_RSP_MSG);
    icf_command_handler(command_string);

#endif

    return p_sprintf_count;   
    
}

#if 0
icf_uint8_t* icf_cmn_trace_mm_unexpected_msg_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
        icf_mm_unexpected_msg_ind_st                  *p_mm_unexpected_msg = ICF_NULL;

        p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

        p_mm_unexpected_msg = (icf_mm_unexpected_msg_ind_st*)p_msg->payload;
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nError Cause = %d",
              p_mm_unexpected_msg->error_cause));

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApi_id = %d",
              p_mm_unexpected_msg->api_id));

        return p_sprintf_count;
}
#endif



icf_uint8_t* icf_cmn_trace_transport_addr(icf_transport_addr_st* p_trans, icf_uint8_t* p_sprintf_count)
{                                      


    icf_uint8_t addr_type = 0;

    if (!p_trans)
        return p_sprintf_count;


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
        "\naddr_type = %d, ", p_trans->addr_type));
    
    addr_type = p_trans->addr_type;
    
    switch (p_trans->addr_type) {
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s, ", 
            p_trans->addr.domain.str));
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d", 
            p_trans->addr.ipv4_addr.octet_1, 
            p_trans->addr.ipv4_addr.octet_2, 
            p_trans->addr.ipv4_addr.octet_3, 
            p_trans->addr.ipv4_addr.octet_4));
        break;
        
    }

    return p_sprintf_count;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_route_info
 *
 * DESCRIPTION:     This function is used to trace route info structure.
 *
 *****************************************************************************/
icf_uint8_t* icf_cmn_trace_route_info(
				icf_route_info_st	*p_rt_info,
				icf_uint8_t			*p_sprintf_count)
{
	/* Null check for input argument */
    if (!p_rt_info)
        return p_sprintf_count;

	/* -- UDP -- */
    if (ICF_IPC_UDP == p_rt_info->type)
    {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"  rt_info.type = ICF_IPC_UDP\n"));

        p_sprintf_count = icf_cmn_trace_transport_addr(
			((icf_transport_addr_st*)&(p_rt_info->ipc.udp.ip)),p_sprintf_count);
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nrt_info.udp.port = %d\n", p_rt_info->ipc.udp.port));
    }
    /* -- TCP -- */
    else if (ICF_IPC_TCP == p_rt_info->type)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"  rt_info.type = ICF_IPC_TCP\n"));

        if (ICF_TCP_SERVER == p_rt_info->ipc.tcp.type)
        {
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
				"\nrt_info.tcp.type = ICF_TCP_SERVER\n"));
        }
        else if (ICF_TCP_CLIENT == p_rt_info->ipc.tcp.type)
        {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
				"\nrt_info.tcp.type = ICF_TCP_CLIENT\n"));
        }
        p_sprintf_count = icf_cmn_trace_transport_addr(
			((icf_transport_addr_st*)&(p_rt_info->ipc.tcp.ip)),p_sprintf_count);
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nrt_info.tcp.port = %d\n", p_rt_info->ipc.tcp.port));
    }
    /* -- MSGQ -- */
    else if (ICF_IPC_MSGQ == p_rt_info->type)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"  rt_info.type = ICF_IPC_MSGQ\n"));

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nrt_info.msgq.mtype = %d\n", p_rt_info->ipc.msgq.mtype));
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nrt_info.msgq.mkey = %d\n", p_rt_info->ipc.msgq.mkey));

    }

    return p_sprintf_count;
} /* End function icf_cmn_trace_route_info() */


icf_uint8_t* icf_cmn_trace_header_list(icf_header_st* p_hdr, 
                                      icf_int32_t hdr_count,icf_uint8_t* p_sprintf_count)
{

    icf_uint8_t i = 0;

    if (!p_hdr || hdr_count < 0)
        return p_sprintf_count; 


/* Header LIST  */    
    if (hdr_count > 0)
   	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%c%d%c",
                            '\n',
			    hdr_count,
			    '\n'
                            )) 
    /*SPR 19590 revert the fix done for CSR 1-7160359: SPR 19509
      as now the trace buffer size is increased from 5K to 15K
      therefore we should remove the temporary fix done for not 
      sending the header lists in traces*/
    /*SPR 19785 To avoid crashes while communicating headers in trace
      we shall be communicating at max ten nodes because there can be
      other lists as well Assoc URI/contact in register_cfm to be 
      communicated in trace */
    for (i = 0; (i < hdr_count) && (p_hdr) && (i <
                ICF_CMN_MAX_TRACE_LIST_NODES); i++, p_hdr = p_hdr->next) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%c%s%c%s",
                            '\n',
			    p_hdr->hdr_name.str,
			    '\n',
			    p_hdr->hdr_value.str
                            ))

    return p_sprintf_count;
}



icf_uint8_t* icf_cmn_trace_tag_list(icf_tag_st* p_tag, 
                                   icf_int32_t tag_count,icf_uint8_t* p_sprintf_count)
{

    icf_uint8_t i = 0;

    if (!p_tag || tag_count < 0)
        return p_sprintf_count;

    if (tag_count > 0) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ntag_list.count = %d", 
            tag_count));

    /*SPR 19785 To avoid crashes while communicating tags in trace
      we shall be communicating at max ten nodes because there can be
      other lists as well Assoc URI/contact in register_cfm to be 
      communicated in trace */
    for (i = 0;(i < tag_count) && (p_tag) && (i < ICF_CMN_MAX_TRACE_LIST_NODES);
            i++, p_tag = p_tag->next) 
	{
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\ntag_list.tag.hdr_name.str = %s, \ntag_list.tag.tag_value.str = %s", 
            p_tag->hdr_name.str, p_tag->tag_value.str));
		if ( p_tag->tag_type == ICF_TAG_TYPE_SIP_URI_PARAM)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\nICF_TAG_TYPE_SIP_URI_PARAM"));
		}
		else if ( p_tag->tag_type == ICF_TAG_TYPE_SIP_URI_HDR_PARAM)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\nICF_TAG_TYPE_SIP_URI_HDR_PARAM"));
		}
		else if ( p_tag->tag_type == ICF_TAG_TYPE_HDR_PARAM)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\nICF_TAG_TYPE_HDR_PARAM"));
		}
		else
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\nICF_TAG_TYPE_INVALID"));
		}

	}



    return p_sprintf_count;
}





icf_uint8_t* icf_cmn_trace_body_list(icf_msg_body_st* p_body, 
                                    icf_int32_t body_count,icf_uint8_t* p_sprintf_count)
{

    icf_uint8_t i = 0;
    icf_uint8_t hdr_count = 0;

    if (!p_body || body_count < 0)
        return p_sprintf_count;

    if (body_count > 0) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%c%d",
                            '\n',
			     body_count
                            ))

    /*SPR 19785 To avoid crashes while communicating body in trace
      we shall be communicating at max ten nodes because there can be
      other lists as well Assoc URI/contact in register_cfm to be 
      communicated in trace */
    for (i = 0;(i < body_count) && (p_body) &&
            (i < ICF_CMN_MAX_TRACE_LIST_NODES); i++, p_body = p_body->next) 
    {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%c%s",
                            '\n',
			     p_body->content_type.str
                            ))

	    /* Display Body header list if present
	     */
	    hdr_count = p_body->msg_body_hdr_list.count;
		if(0 < p_body->msg_body_hdr_list.count)
	    {
		    p_sprintf_count = icf_cmn_trace_header_list(p_body->msg_body_hdr_list.hdr,
				   hdr_count,p_sprintf_count);
	    }
	    
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                            "%c%d%s",
                            '\n',
			    p_body->length,
			    p_body->value
                            ))

    }
   return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_header_name_list(icf_header_name_st* p_hdr, 
                                           icf_uint8_t hdr_count,icf_uint8_t* p_sprintf_count)
{

    icf_uint8_t i = 0;

    if (!p_hdr)
        return p_sprintf_count;


/* Header LIST  */    
    if (hdr_count > 0)
    { 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\nheader_name_list.count = %d", hdr_count));
    }

    for (i = 0; i < hdr_count && p_hdr; i++, p_hdr = p_hdr->next)
    { 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
            "\nheader_name_list.hdr.hdr_name.str = %s", p_hdr->hdr_name.str));
    }

    return p_sprintf_count;
}





icf_uint8_t* icf_cmn_trace_media_connected(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_media_connected_ind_st* p_med_con = NULL;

    icf_list_st *p_list=ICF_NULL;
    
	icf_uint8_t i=0;
    p_med_con = (icf_media_connected_ind_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_med_con)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_med_con is NULL.\n"))
        return p_sprintf_count;
    }     

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "bit_mask = %d", p_med_con->bit_mask));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Accepted Stream List:"));

	i = 1;
        p_list=p_med_con->p_accepted_stream_list;  
	while(p_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_list->p_data,p_sprintf_count);
		
		p_list = p_list->p_next;
		
		i++;
	}


	if(p_med_con->bit_mask & ICF_MEDIA_CONECT_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Deleted stream count %d:",p_med_con->count));	

		for(i=0;(i<p_med_con->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",p_med_con->deleted_media_streams[i].stream_type));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",p_med_con->deleted_media_streams[i].stream_id));
		}
	}




    return p_sprintf_count;

}








icf_uint8_t* icf_cmn_trace_connect_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_connect_req_st* p_con_req = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;   
    icf_list_st *p_stream_list;

    p_con_req = (icf_connect_req_st*)p_msg->payload;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_con_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_con_req is NULL.\n"))
        return p_sprintf_count;
    }     

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nbit_mask = %d", p_con_req->bit_mask));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
        "\nprivacy_ind = %d", p_con_req->privacy_ind));
    
    
/* Header LIST  */    if (p_con_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_con_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_con_req->header_list.hdr, header_list_count,p_sprintf_count);
    }
       if (p_con_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_con_req->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_con_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
        if (p_con_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_con_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_con_req->body_list.body, body_list_count,p_sprintf_count);
    }
    

	if(p_con_req->bit_mask & ICF_STREAM_LIST_PRESENT)
	{

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
	        "\nMedia Profile List:"));
		p_stream_list= p_con_req->p_stream_list;
		i = 1;
		while(p_stream_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
			p_sprintf_count = icf_cmn_trace_stream_capab(p_stream_list->p_data,p_sprintf_count);
			p_stream_list = p_stream_list->p_next;
			i++;
		}
	}

	if(p_con_req->bit_mask & ICF_CONNECT_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n rejected stream count: %d",
			p_con_req->count));	

		for(i=0;(i<p_con_req->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Stream %d", i+1));

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id: %d",
				p_con_req->deleted_media_streams[i].stream_id));

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type: %d",
				p_con_req->deleted_media_streams[i].stream_type));
		}
	}


  
    return p_sprintf_count;

}




icf_uint8_t* icf_cmn_trace_connect_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
       
    icf_connect_resp_st* p_con_resp = NULL;

    icf_uint8_t  i=1;
   
    icf_list_st *p_list=ICF_NULL;
    p_con_resp = (icf_connect_resp_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_con_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_con_resp is NULL.\n"))
        return p_sprintf_count;
    }    
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_list=p_con_resp->p_accepted_stream_list; 
	while(p_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_modify_media_change_req(p_list->p_data,p_sprintf_count);
		
		p_list = p_list->p_next;
		
		i++;
	}

	if(p_con_resp->bit_mask & ICF_CONNECT_RESP_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Deleted stream count: %d",p_con_resp->count));	

		for(i=0;(i<p_con_resp->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type: %d",p_con_resp->deleted_media_streams[i].stream_type));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id: %d",p_con_resp->deleted_media_streams[i].stream_id));
		}
	}

 
#ifdef ICF_FOR_LOAD_TESTING

	s_calls++;
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No.of Successfully calls:%d",s_calls));
    
#endif

    return p_sprintf_count;

}



icf_uint8_t* icf_cmn_trace_connect_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_connect_ind_st* p_con_ind = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0,i;
    
    icf_list_st *p_accepted_stream_list = ICF_NULL;

    p_con_ind = (icf_connect_ind_st*)p_msg->payload;
   
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_con_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_con_ind is NULL.\n"))
        return p_sprintf_count;
    }    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "bit_mask = %d", p_con_ind->bit_mask));
	if (p_con_ind->bit_mask & ICF_ASSERTED_ID_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
			"\nasserted_identity.addr_val.str = %s"
			"\nasserted_identity.addr_type = %d", 
			p_con_ind->asserted_identity.addr_val.str, 
			p_con_ind->asserted_identity.addr_type));
    

/* Header LIST  */    if (p_con_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_con_ind->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_con_ind->header_list.hdr, header_list_count,p_sprintf_count);
    }
  
    
    /* Body LIST  */
    if (p_con_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_con_ind->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_con_ind->body_list.body, body_list_count,p_sprintf_count);
    }

    i = 1;

       /* SPR 20208: Local reference taken to Accepted Stream List so that the
        * actual stream list is not overwritten.
        */
        p_accepted_stream_list = p_con_ind->p_accepted_stream_list;
	while(p_accepted_stream_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_modify_media_change_req(p_accepted_stream_list->p_data,p_sprintf_count);
		
		p_accepted_stream_list = p_accepted_stream_list->p_next;
		
		i++;
	}


	if(p_con_ind->bit_mask & ICF_CONNECT_IND_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Deleted stream count: %d",p_con_ind->count));	

		for(i=0;(i<p_con_ind->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type: %d",p_con_ind->deleted_media_streams[i].stream_type));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id: %d",p_con_ind->deleted_media_streams[i].stream_id));
		}
	}


#ifdef ICF_FOR_LOAD_TESTING

	s_calls++;
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNo.of Successfully Calls:%d",s_calls));
  
#endif

    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_get_line_settings_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
     
	icf_get_line_settings_resp_st* p_get_line_stg_resp = ICF_NULL;
	icf_uint32_t	presence_flag = ICF_NULL;
	icf_transport_addr_type_t  addr_type = ICF_NULL;

	p_get_line_stg_resp = (icf_get_line_settings_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_get_line_stg_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_get_line_stg_resp is NULL.\n"))
        return p_sprintf_count;
    }       
	presence_flag = p_get_line_stg_resp->bit_mask;

	p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, \
		"\n------ This is Print function for get_line_settings -------\n"));
		
	if(presence_flag & ICF_REQD_PROXY_ADDR)
	{
		addr_type = p_get_line_stg_resp->proxy_address.addr.addr_type;

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nProxy address = proxy_addr_type = %d addr = ",
					addr_type));
        
        switch (addr_type) 
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s, ", 
					p_get_line_stg_resp->proxy_address.addr.addr.domain.str));
				break;
            
			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d, ", 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_1, 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_2, 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_3, 
					p_get_line_stg_resp->proxy_address.addr.addr.ipv4_addr.octet_4));
				break;
          }

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
			"\n proxy_port = %d, ",
			  p_get_line_stg_resp->proxy_address.port_num));
	}

	if(presence_flag & ICF_REQD_REGISTRAR_ADDR)
	{

		addr_type = p_get_line_stg_resp->registrar_address.addr.addr_type;

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
				"\n registrar_addr = addr_type = %d, ",
				  addr_type));
				
		switch (addr_type) 
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s, ", 
					p_get_line_stg_resp->registrar_address.addr.addr.domain.str));
				break;
			
			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d, ", 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_1, 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_2, 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_3, 
					p_get_line_stg_resp->registrar_address.addr.addr.ipv4_addr.octet_4));
				break;
		}

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
			"\n registrar_port = %d, ",
			  p_get_line_stg_resp->registrar_address.port_num));
	}


	if(presence_flag & ICF_REQD_PROXY_TRANSPORT_MODE)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nproxy_transport_mode = %d" ,
			p_get_line_stg_resp->proxy_transport_mode));
	}

	if(presence_flag & ICF_REQD_SELF_ADDR_LIST)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nself_address_list = addr_type = %d self_address = %s" ,
			p_get_line_stg_resp->self_address_list.addr_type,
			p_get_line_stg_resp->self_address_list.addr_val.str));
	}
	
	if(presence_flag & ICF_REQD_SELF_NAME)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nself name = %s" ,
			p_get_line_stg_resp->self_name.str));
	}
	
	if(presence_flag & ICF_REQD_PRESENTATION_STATUS)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPresentation Status = %d" ,
			p_get_line_stg_resp->presentation_allowed));
	}
	
	if(presence_flag & ICF_REQD_AUTH_KEY)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nauth_key = %s" ,
			p_get_line_stg_resp->auth_key.str));
	}
	
	if(presence_flag & ICF_REQD_AUTH_PASSWD)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nauth_password = %s" ,
			p_get_line_stg_resp->auth_password.str));
	}
	if(presence_flag & ICF_REQD_REG_MODE)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nreg_mode = %d" ,
			p_get_line_stg_resp->reg_mode));
	}
	if(presence_flag & ICF_REQD_REG_DURATION)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nreg_duration = %d" ,
			p_get_line_stg_resp->reg_duration));
	}
        if(presence_flag & ICF_REQD_SIP_T1_TIMER)
        {
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsip_t1_timer = %d" ,
                        p_get_line_stg_resp->sip_T1_timer));
        }
        if(presence_flag & ICF_REQD_SIP_B_TIMER)
        {
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsip_b_timer = %d" ,
                        p_get_line_stg_resp->sip_B_timer));
        }
        if(presence_flag & ICF_REQD_SIP_F_TIMER)
        {
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsip_f_timer = %d" ,
                        p_get_line_stg_resp->sip_F_timer));
        } 

        if(presence_flag & ICF_REQD_SIP_T2_TIMER)
        {
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsip_t2_timer = %d" ,
                        p_get_line_stg_resp->sip_T2_timer));
        }

       if(presence_flag & ICF_REQD_SIP_T4_TIMER)
        {
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsip_t4_timer = %d" ,
                        p_get_line_stg_resp->sip_T4_timer));
        }
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_get_sys_settings_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	icf_get_system_settings_resp_st* p_get_sys_stg_resp = ICF_NULL;
	icf_uint32_t	presence_flag = ICF_NULL;
	icf_transport_addr_type_t  addr_type = ICF_NULL;
	icf_uint8_t i=0;
	p_get_sys_stg_resp = (icf_get_system_settings_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_get_sys_stg_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_get_sys_stg_resp is NULL.\n"))
        return p_sprintf_count;
    }        
	presence_flag = p_get_sys_stg_resp->bit_mask;

	p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, \
		"\n------ This is Print function for get_sys_settings -------\n"));

	if(presence_flag & ICF_REQD_PRACK_ENABLED_STATUS)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nprack_enabled = %d" ,
			p_get_sys_stg_resp->prack_enabled));
	}
	
    if(presence_flag & ICF_REQD_PRIVACY_SUPPORTED)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nprivacy_supported= %d" ,
			p_get_sys_stg_resp->privacy_supported));

	}
	if(presence_flag & ICF_REQD_MAX_LINES)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmax_lines = %d" ,
			p_get_sys_stg_resp->max_lines));
	}
	if(presence_flag & ICF_REQD_SETUP_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsetup_timer = %d" ,
			p_get_sys_stg_resp->setup_timer));
	}
	if(presence_flag & ICF_REQD_ALERTING_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nalerting_timer = %d" ,
			p_get_sys_stg_resp->alerting_timer));
	}
	if(presence_flag & ICF_REQD_RELEASE_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nrelease_timer = %d" ,
			p_get_sys_stg_resp->release_timer));
	}
	if(presence_flag & ICF_REQD_MODIFY_MEDIA_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmodify_media_timer = %d" ,
			p_get_sys_stg_resp->modify_media_timer));
	}
	if(presence_flag & ICF_REQD_REG_RESP_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nregistration_resp_timer = %d" ,
			p_get_sys_stg_resp->registration_resp_timer));
	}
	if(presence_flag & ICF_REQD_REG_RETRY_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nregistration_retry_timer = %d" ,
			p_get_sys_stg_resp->registration_retry_timer));
	}
	if(presence_flag & ICF_REQD_SUBS_RETRY_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsubs_retry_timer = %d" ,
			p_get_sys_stg_resp->subs_retry_timer));
	}

	if(presence_flag & ICF_REQD_SUBS_RESP_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsubs_resp_timer = %d" ,
			p_get_sys_stg_resp->subs_resp_timer));
	}

	if(presence_flag & ICF_REQD_SIG_COMP_STATUS)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsig_comp_enabled = %d" ,
			p_get_sys_stg_resp->sig_comp_enabled));
	}

	if(presence_flag & ICF_REQD_SECURITY_STATUS)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsecurity_enabled = %d" ,
			p_get_sys_stg_resp->security_enabled));
	}

	if(presence_flag & ICF_REQD_SELF_IP_ADDRESS)
	{

		addr_type = p_get_sys_stg_resp->self_ip_address.addr.addr_type;

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
				"\n self_ip_address = addr_type = %d, ",
				  addr_type));
				
		switch (addr_type) 
		{
			case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s, ", 
					p_get_sys_stg_resp->self_ip_address.addr.addr.domain.str));
				break;
			
			case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d, ", 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_1, 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_2, 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_3, 
					p_get_sys_stg_resp->self_ip_address.addr.addr.ipv4_addr.octet_4));
				break;
		}

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
				"\n self_port = %d, ",
				  p_get_sys_stg_resp->self_ip_address.port_num));
	}

	if(presence_flag & ICF_REQD_ASSO_URI_LIST)
	{
        p_sprintf_count = icf_cmn_trace_pub_uri_list(p_get_sys_stg_resp->p_uri_list,p_sprintf_count);
    }
		
	i = 0;
	if(presence_flag & ICF_REQD_APN_LIST)
	{
		while (i != p_get_sys_stg_resp->apn_list.count)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApn[%d] = %s" ,
				i,p_get_sys_stg_resp->apn_list.p_list_of_apn[i].str));

			i++;
		}
	}
	if(presence_flag & ICF_REQD_SELF_TRANSPORT_MODE)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nself_transport_mode = %d" ,
			p_get_sys_stg_resp->self_transport_mode));
	}
	if(presence_flag & ICF_REQD_SELF_PORT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nself_port = %d" ,
			p_get_sys_stg_resp->self_port));
	}
	if(presence_flag & ICF_REQD_CALL_THRU_PROXY_STATUS)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ncall_thru_proxy = %d" ,
			p_get_sys_stg_resp->call_thru_proxy));
	}
	if(presence_flag & ICF_REQD_SIP_COMPACT_HDR_STATUS)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsip_compact_header = %d" ,
			p_get_sys_stg_resp->sip_compact_header));
	}
	if(presence_flag & ICF_REQD_QOS_MARK)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nqos_mark = %d" ,
			p_get_sys_stg_resp->qos_mark));
	}
	if(presence_flag & ICF_REQD_SESSION_INTERVAL_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession_interval_timer = %d" ,
			p_get_sys_stg_resp->session_interval_timer));
	}

	if(presence_flag & ICF_REQD_MIN_SESSION_INTERVAL_TIMER)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmin_session_interval_timer = %d" ,
			p_get_sys_stg_resp->min_session_interval_timer));
	}
#ifdef ICF_NAT_RPORT_SUPPORT
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbinding_refresh_timer = %d" ,
		p_get_sys_stg_resp->binding_refresh_timer));
	if( ICF_TRUE == p_get_sys_stg_resp->binding_query_enable )
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbinding query enabled"));
	}	
	else if( ICF_FALSE == p_get_sys_stg_resp->binding_query_enable )
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbinding query not enabled"));
	}
#endif


	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_get_line_settings(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_get_line_settings_req_st*      p_get_line_stg_req = ICF_NULL;

    icf_route_info_st*          p_rt_info = ICF_NULL;

    p_get_line_stg_req = (icf_get_line_settings_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_get_line_stg_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_get_line_stg_req is NULL.\n"))
            return p_sprintf_count;
    }         
    p_rt_info = (icf_route_info_st*)p_get_line_stg_req->routing_info.ptr;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\nline-id = %d", p_get_line_stg_req->line_id));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_PROXY_ADDR) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_PROXY_ADDR present"));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_REGISTRAR_ADDR)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_REGISTRAR_ADDR present"));


    if (p_get_line_stg_req->bit_mask & ICF_REQD_PROXY_TRANSPORT_MODE) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_PROXY_TRANSPORT_MODE present "));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_SELF_ADDR_LIST) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SELF_ADDR_LIST present"));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_SELF_NAME) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SELF_NAME present"));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_PRESENTATION_STATUS) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_PRESENTATION_STATUS present"));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_AUTH_KEY) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_AUTH_KEY present "));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_AUTH_PASSWD) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_AUTH_PASSWD present "));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_REG_MODE) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_REG_MODE present "));

    if (p_get_line_stg_req->bit_mask & ICF_REQD_REG_DURATION) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_REG_DURATION present "));

    /* FIX FOR SPR 17944 */	
    if(p_get_line_stg_req->bit_mask & ICF_REQD_REG_ROUTE_ADDR)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_REG_ROUTE_ADDR present "));
    /* Fix for CSR 1-6214356 : SPR 18849 */
    /* Check the bitmask before accessing p_rt_info as it may be null.
     * Also check for IPC type in route info.
     */
    if(ICF_GET_SETTINGS_ROUTE_INFO_PRESENT & p_get_line_stg_req->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    "\np_rt_info.len = %d", p_get_line_stg_req->routing_info.len));
        p_sprintf_count = icf_cmn_trace_route_info(p_rt_info, p_sprintf_count);
    }

    if(ICF_REQD_SIP_T1_TIMER & p_get_line_stg_req->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SIP_T1_TIMER present"));
    }

    if(ICF_REQD_SIP_B_TIMER & p_get_line_stg_req->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SIP_B_TIMER present"));
    }

    if(ICF_REQD_SIP_F_TIMER & p_get_line_stg_req->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SIP_F_TIMER present"));
    }

    if(ICF_REQD_SIP_T2_TIMER & p_get_line_stg_req->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SIP_T2_TIMER present"));
    }

    if(ICF_REQD_SIP_T4_TIMER & p_get_line_stg_req->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SIP_T4_TIMER present"));
    }

    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_get_sys_settings(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        
    icf_get_system_settings_req_st*      p_get_sys_stg_req = ICF_NULL;

    icf_route_info_st*          p_rt_info = ICF_NULL;
    p_get_sys_stg_req = (icf_get_system_settings_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_get_sys_stg_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_get_sys_stg_req is NULL.\n"))
        return p_sprintf_count;
    }      
    p_rt_info = (icf_route_info_st*)p_get_sys_stg_req->routing_info.ptr;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


 

    if (p_get_sys_stg_req->bit_mask & ICF_REQD_PRACK_ENABLED_STATUS) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_PRACK_ENABLED_STATUS present"));


	if (p_get_sys_stg_req->bit_mask & ICF_REQD_PRIVACY_SUPPORTED) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_PRIVACY_SUPPORTED present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_MAX_LINES) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_MAX_LINES present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SETUP_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SETUP_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_ALERTING_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_ALERTING_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_RELEASE_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_RELEASE_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_MODIFY_MEDIA_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_MODIFY_MEDIA_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_REG_RESP_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_REG_RESP_TIMER present"));
	
	if (p_get_sys_stg_req->bit_mask & ICF_REQD_REG_RETRY_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_REG_RETRY_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SUBS_RETRY_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SUBS_RETRY_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SUBS_RESP_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_REQD_SUBS_RESP_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SIG_COMP_STATUS) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_SIG_COMP_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SECURITY_STATUS) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_SECURITY_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SELF_IP_ADDRESS) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_SELF_IP_ADDRESS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SUBS_RESP_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_ASSO_URI_LIST present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_APN_LIST) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_APN_LIST present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SELF_TRANSPORT_MODE) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_SELF_TRANSPORT_MODE present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SELF_PORT) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_SELF_PORT present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_CALL_THRU_PROXY_STATUS) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_CALL_THRU_PROXY_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SIP_COMPACT_HDR_STATUS) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_SIP_COMPACT_HDR_STATUS present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_QOS_MARK) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_QOS_MARK present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_SESSION_INTERVAL_TIMER) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_SESSION_INTERVAL_TIMER present"));

	if (p_get_sys_stg_req->bit_mask & ICF_REQD_MIN_SESSION_INTERVAL_TIMER ) 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ICF_REQD_MIN_SESSION_INTERVAL_TIMER  present"));
    /* Fix for CSR 1-6214356 : SPR 18849 */
	/* Check the bitmask before accessing p_rt_info as it may be null.
     * Also check for IPC type in route info.
     */
	if (ICF_GET_SETTINGS_ROUTE_INFO_PRESENT & p_get_sys_stg_req->bit_mask)
	{	
	   p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\np_rt_info.len = %d", p_get_sys_stg_req->routing_info.len));
       p_sprintf_count = icf_cmn_trace_route_info(p_rt_info, p_sprintf_count);
	}
     

    return p_sprintf_count;

}


/****************************************************************************
*
*				Function : icf_cmn_trace_audio_codec_param
*
*				Description : This function is used to trace 
*								audio codec parameters
*
*****************************************************************************/ 
icf_uint8_t*  icf_cmn_trace_audio_codec_param(icf_uint8_t *p_audio_param, icf_uint8_t* p_sprintf_count)	
{
	icf_audio_codec_param_st    *p_icf_audio_codec_param_st = (icf_audio_codec_param_st *)p_audio_param;

	icf_uint8_t			i = 0;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_icf_audio_codec_param_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_icf_audio_codec_param_st is NULL.\n"))
        return p_sprintf_count;
    }        
       
	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_CHANNEL)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nChannels = %d",p_icf_audio_codec_param_st->channels));
	
	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSampling Rate = %d",p_icf_audio_codec_param_st->sampling_rate));

	if(p_icf_audio_codec_param_st->bit_mask & ICF_AUDIO_CODEC_PARAM_AMR_PARAM)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nAmr Initial Mode = %d",
					p_icf_audio_codec_param_st->amr_params.amr_initial_mode));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec Mode = %d",
					p_icf_audio_codec_param_st->amr_params.codec_mode));
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCrc Enabled = %d",
					p_icf_audio_codec_param_st->amr_params.crc_enabled));
		if(ICF_AMR_MODE_LIST_PRESENT & p_icf_audio_codec_param_st->amr_params.bit_mask)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nAMR Mode Count = %d",
					p_icf_audio_codec_param_st->amr_params.amr_mode_count));
			for(i=0;(i<p_icf_audio_codec_param_st->amr_params.amr_mode_count) && (i<ICF_MAX_AMR_MODE);i++)
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d\n",
					p_icf_audio_codec_param_st->amr_params.amr_mode_list[i]));
		}
	}

	return p_sprintf_count;
}

/****************************************************************************
*
*				Function : icf_cmn_trace_video_codec_param
*
*				Description : This function is used to trace 
*								video codec parameters
*
*****************************************************************************/ 
icf_uint8_t*  icf_cmn_trace_video_codec_param(icf_uint8_t *p_video_param, icf_uint8_t* p_sprintf_count)	
{
	icf_video_codec_param_st    *p_icf_video_codec_param_st = (icf_video_codec_param_st *)p_video_param;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_icf_video_codec_param_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_icf_video_codec_param_st is NULL.\n"))
        return p_sprintf_count;
    }          
   
	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nClock rate = %d",p_icf_video_codec_param_st->clock_rate));

	if(p_icf_video_codec_param_st->bit_mask & ICF_VIDEO_CODEC_FRAMESIZE_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFrame Height = %d",p_icf_video_codec_param_st->frame_size.height));
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFrame Width = %d",p_icf_video_codec_param_st->frame_size.width));
	}
	
	return p_sprintf_count;
}

/****************************************************************************
*
*				Function : icf_cmn_trace_t38_attrib_param
*
*				Description : This function is used to trace
*								fax parameters
*
*****************************************************************************/ 
icf_uint8_t*  icf_cmn_trace_t38_attrib_param(icf_uint8_t *p_t38_param, icf_uint8_t* p_sprintf_count)	
{
	icf_t38_attrib_param_st    *p_icf_t38_codec_param_st = (icf_t38_attrib_param_st *)p_t38_param;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_icf_t38_codec_param_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_icf_t38_codec_param_st is NULL.\n"))
        return p_sprintf_count;
    }      
   
	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_VERSION)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFax Version = %d",p_icf_t38_codec_param_st->fax_version));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_BITRATE)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nBitrate = %d",p_icf_t38_codec_param_st->bitrate));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_RATE_MANAGEMENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nRate Management = %d",p_icf_t38_codec_param_st->rate_management));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_UDP_BUFFSIZE)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nUdp Buffersize = %d",p_icf_t38_codec_param_st->udp_buffersize));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_UDP_PACKETSIZE)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nUdp Packetsize = %d",p_icf_t38_codec_param_st->udp_packetsize));

	if(p_icf_t38_codec_param_st->bit_mask & ICF_FAX_T38_ERR_CORRECTION)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nErr Correction Method = %d",
				p_icf_t38_codec_param_st->err_correction_method));

	return p_sprintf_count;
}

/****************************************************************************
*
*				Function : icf_cmn_trace_audio_profile
*
*				Description : This function is used to trace 
*								audio profile.
*
*****************************************************************************/ 
icf_uint8_t* icf_cmn_trace_audio_profile(icf_uint8_t *p_audio_profile,icf_uint8_t* p_sprintf_count)
{
	icf_audio_profile_st    *p_audio_profile_st = (icf_audio_profile_st *)p_audio_profile;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_audio_profile_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_audio_profile_st is NULL.\n"))
        return p_sprintf_count;
    }          
   
	if(p_audio_profile_st->bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nBandwidth = %d",p_audio_profile_st->bandwidth));
	
	if(p_audio_profile_st->bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPref ptime = %d",p_audio_profile_st->pref_ptime));
	
	if(p_audio_profile_st->bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSilence Supp = %d",p_audio_profile_st->silence_suppression));
	
	return p_sprintf_count;
}

/****************************************************************************
*
*				Function : icf_cmn_trace_video_profile
*
*				Description : This function is used to trace 
*								video profile.
*
*****************************************************************************/ 
icf_uint8_t* icf_cmn_trace_video_profile(icf_uint8_t *p_video_profile,icf_uint8_t* p_sprintf_count)
{
	icf_video_profile_st    *p_video_profile_st = (icf_video_profile_st *)p_video_profile;
   
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_video_profile_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_video_profile_st is NULL.\n"))
        return p_sprintf_count;
    }   
    
	if(p_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nEncoding Quality = %d",p_video_profile_st->encoding_quality));
	
	if(p_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFrame Rate = %d",p_video_profile_st->frame_rate));
	
	if(p_video_profile_st->bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nBandwidth = %d",p_video_profile_st->bandwidth));
	
	return p_sprintf_count;
}
/****************************************************************************
*
*				Function : icf_cmn_trace_stream_capab
*
*				Description : This function is used to trace 
*								stream_capab_st structure.
*
*****************************************************************************/ 
icf_uint8_t* icf_cmn_trace_stream_capab(icf_stream_capab_st* p_stream_capab_st,icf_uint8_t* p_sprintf_count)
{
	icf_uint8_t		j=0,i = 1;
	icf_list_st		*p_codec_info = ICF_NULL;
	icf_list_st		*p_encoded_att = ICF_NULL;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stream_capab_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stream_capab_st is NULL.\n"))
        return p_sprintf_count;
    }       

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id = %d\n",p_stream_capab_st->stream_id));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Type = %d\n",p_stream_capab_st->stream_type));

	if(p_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
	{
		p_codec_info = p_stream_capab_st->p_codec_info_list;
		
		while(p_codec_info != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_codec_info->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_codec_info->p_data is NULL.\n"))
                return p_sprintf_count;
            }     
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec %d",i));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec Num = %d",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec Name = "));
			for(j=0;(j<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len) && (j< ICF_MAX_SHORT_STR_LEN);j++)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
						((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

			switch(p_stream_capab_st->stream_type)
			{
				case  ICF_STREAM_TYPE_AUDIO:
					  p_sprintf_count = icf_cmn_trace_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;

				case  ICF_STREAM_TYPE_VIDEO:
					  p_sprintf_count = icf_cmn_trace_video_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;

				case  ICF_STREAM_TYPE_FAX:
					   p_sprintf_count = icf_cmn_trace_t38_attrib_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;
			}
		
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes for codec\n"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
                /*Fix For SPR : 18999*/
                if (ICF_NULL == p_encoded_att->p_data)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                    return p_sprintf_count;
                }     
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," "));
				for(i=0;(i<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (i< ICF_MAX_STR_LEN);i++)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
							((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}
				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}
	}

	if(p_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nMedia Stream Parameters"));
		switch(p_stream_capab_st->stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
				  p_sprintf_count = icf_cmn_trace_audio_profile((icf_uint8_t*)&(p_stream_capab_st->media_stream_param),p_sprintf_count);			  
				  break;

			case  ICF_STREAM_TYPE_VIDEO:
				  p_sprintf_count = icf_cmn_trace_video_profile((icf_uint8_t*)&(p_stream_capab_st->media_stream_param),p_sprintf_count);	  
				  break;
		}
	}

	if(p_stream_capab_st->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
	{
		p_encoded_att = p_stream_capab_st->p_encoded_attributes;
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes\n"));

		while(p_encoded_att != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_encoded_att->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                return p_sprintf_count;
            }               
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," "));
			for(i=0;(i<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (i< ICF_MAX_STR_LEN) ;i++)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
						((icf_string_st*)p_encoded_att->p_data)->str[i]));
			}

			p_encoded_att = p_encoded_att->p_next;
		}
	}

	return p_sprintf_count;
}

/****************************************************************************
*
*				Function : icf_cmn_trace_modify_media_change_req
*
*				Description : This function is used to trace 
*								stream_capab_st structure.
*
*****************************************************************************/ 
icf_uint8_t* icf_cmn_trace_modify_media_change_req(icf_call_modify_media_data_st* p_modify_media_change_req,icf_uint8_t *p_sprintf_count)
{
	icf_uint8_t		i = 1;
	icf_list_st		*p_codec_info;
	icf_list_st		*p_encoded_att;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_modify_media_change_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_modify_media_change_req is NULL.\n"))
        return p_sprintf_count;
    }       

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id = %d",p_modify_media_change_req->stream_id));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Type = %d",p_modify_media_change_req->stream_type));

	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_CODEC_INFO_PRESENT) 
	{
		p_codec_info = p_modify_media_change_req->p_codec_info_list;
		
		while(p_codec_info != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_codec_info->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_codec_info->p_data is NULL.\n"))
                return p_sprintf_count;
            }   
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec %d",i));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec Num = %d",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec Name = "));

			for(i=0;(i<(((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name).str_len) && (i<ICF_MAX_SHORT_STR_LEN);i++)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
							((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[i]));
				}

			switch(p_modify_media_change_req->stream_type)
			{
				case  ICF_STREAM_TYPE_AUDIO:
					  p_sprintf_count = icf_cmn_trace_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;

				case  ICF_STREAM_TYPE_VIDEO:
					  p_sprintf_count = icf_cmn_trace_video_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;

				case  ICF_STREAM_TYPE_FAX:
					   p_sprintf_count = icf_cmn_trace_t38_attrib_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;
			}
		
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes for codec\n"));

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			while(p_encoded_att != ICF_NULL)
			{
                /*Fix For SPR : 18999*/
                if (ICF_NULL == p_encoded_att->p_data)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                    return p_sprintf_count;
                }   
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," "));
				for(i=0;(i<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (i< ICF_MAX_STR_LEN);i++)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
							((icf_string_st*)p_encoded_att->p_data)->str[i]));
				}

				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}
	}


	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nMedia Stream Parameters\n"));
		switch(p_modify_media_change_req->stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
				  p_sprintf_count = icf_cmn_trace_audio_profile((icf_uint8_t*)&(p_modify_media_change_req->media_stream_param),p_sprintf_count);			  
				  break;

			case  ICF_STREAM_TYPE_VIDEO:
				  p_sprintf_count = icf_cmn_trace_video_profile((icf_uint8_t*)&(p_modify_media_change_req->media_stream_param),p_sprintf_count);	  
				  break;
		}
	}

	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_ENC_ATTRIB_PRESENT)
	{
		p_encoded_att = p_modify_media_change_req->p_encoded_attributes;
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes\n"));

		while(p_encoded_att != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_encoded_att->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                return p_sprintf_count;
            }  
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," "));
			for(i=0;(i<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (i< ICF_MAX_STR_LEN);i++)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
						((icf_string_st*)p_encoded_att->p_data)->str[i]));
			}

		p_encoded_att = p_encoded_att->p_next;
		}
	}

	if(p_modify_media_change_req->bit_mask & ICF_CALL_MOD_MEDIA_MODE_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," media mode = %d",p_modify_media_change_req->media_mode));
	}

	return p_sprintf_count;
}
/****************************************************************************
*
*				Function : icf_cmn_trace_stream_sdp
*
*				Description : This function is used to trace 
*								stream_sdp_st structure.
*
*****************************************************************************/ 
icf_uint8_t* icf_cmn_trace_stream_sdp(icf_sdp_stream_st* p_sdp_stream_st,icf_media_type_t stream_type,icf_uint8_t* p_sprintf_count)
{
	icf_uint8_t		j=0,i = 1;
	icf_list_st		*p_codec_info;
	icf_list_st		*p_encoded_att;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_sdp_stream_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_glb_pdb is NULL.\n"))
        return p_sprintf_count;
    }         

	{

		p_codec_info = p_sdp_stream_st->p_codec_info_list;
		
		while(p_codec_info != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_codec_info->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_codec_info->p_data is NULL.\n"))
                return p_sprintf_count;
            }  
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec %d",i));
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCodec Num = %d",
				((icf_codec_attrib_st*)p_codec_info->p_data)->codec_num));

			for(j=0;(j<((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str_len) && (j< ICF_MAX_SHORT_STR_LEN) ;j++)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
						((icf_codec_attrib_st*)p_codec_info->p_data)->codec_name.str[j]));
			}

		
			
			switch(stream_type)
			{
				case  ICF_STREAM_TYPE_AUDIO:
					  p_sprintf_count = icf_cmn_trace_audio_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;

				case  ICF_STREAM_TYPE_VIDEO:
					  p_sprintf_count = icf_cmn_trace_video_codec_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;

				case  ICF_STREAM_TYPE_FAX:
					   p_sprintf_count = icf_cmn_trace_t38_attrib_param((icf_uint8_t*)&((icf_codec_attrib_st*)p_codec_info->p_data)->codec_params,p_sprintf_count);			  
					  break;
			}

			p_encoded_att = ((icf_codec_attrib_st*)p_codec_info->p_data)->p_encoded_attributes;
			
			if(p_encoded_att != ICF_NULL)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes for codec\n"));
			}
			while(p_encoded_att != ICF_NULL)
			{
                /*Fix For SPR : 18999*/
                if (ICF_NULL == p_encoded_att->p_data)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                    return p_sprintf_count;
                }  
				for(j=0;(j<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (j<ICF_MAX_STR_LEN);j++)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
							((icf_string_st*)p_encoded_att->p_data)->str[j]));
				}

				p_encoded_att = p_encoded_att->p_next;
			}

			p_codec_info = p_codec_info->p_next;
			i++;
		}
	}

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Stream Address "));
	p_sprintf_count = icf_cmn_trace_transport_addr(((icf_transport_addr_st*)&(p_sdp_stream_st->stream_address.addr)),p_sprintf_count);
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n stream_info.port = %d", p_sdp_stream_st->stream_address.port_num));

	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Transport Mode "));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n transport_mode = %d", p_sdp_stream_st->trans_mode));

	if(p_sdp_stream_st->bit_mask & ICF_SDP_STREAM_MEDIA_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nMedia Stream Parameters \n"));
		switch(stream_type)
		{
			case  ICF_STREAM_TYPE_AUDIO:
				  p_sprintf_count = icf_cmn_trace_audio_profile((icf_uint8_t*)&(p_sdp_stream_st->media_stream_param),p_sprintf_count);			  
				  break;

			case  ICF_STREAM_TYPE_VIDEO:
				  p_sprintf_count = icf_cmn_trace_video_profile((icf_uint8_t*)&(p_sdp_stream_st->media_stream_param),p_sprintf_count);	  
				  break;
		}
	}

	if(p_sdp_stream_st->bit_mask & ICF_SDP_STREAM_RTCP_INFO_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n RTCP Address "));
		p_sprintf_count = icf_cmn_trace_transport_addr(((icf_transport_addr_st*)&(p_sdp_stream_st->rtcp_address.addr)),p_sprintf_count);
    
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\n rtcp_info.port = %d", p_sdp_stream_st->rtcp_address.port_num));
	}


	if(p_sdp_stream_st->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
	{
		p_encoded_att = p_sdp_stream_st->p_encoded_attributes;
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded attributes\n"));

		while(p_encoded_att != ICF_NULL)
		{
            /*Fix For SPR : 18999*/
            if (ICF_NULL == p_encoded_att->p_data)
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_encoded_att->p_data is NULL.\n"))
                return p_sprintf_count;
            }  
			for(j=0;(j<((icf_string_st*)(p_encoded_att->p_data))->str_len) && (j<ICF_MAX_STR_LEN);j++)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
						((icf_string_st*)p_encoded_att->p_data)->str[j]));
			}

		p_encoded_att = p_encoded_att->p_next;
		}
	}

	return p_sprintf_count;
}

/****************************************************************************
*
*				Function : icf_cmn_trace_resp_stream
*
*				Description : This function is used to trace 
*								stream_capab_st structure.
*
*****************************************************************************/ 
icf_uint8_t* icf_cmn_trace_resp_stream(icf_resp_stream_st* p_resp_stream_st, icf_uint8_t i,icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_resp_stream_st)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_resp_stream_st is NULL.\n"))
        return p_sprintf_count;
    }       
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\tstream_data[%d].stream_type = %d"
        "\n\tstream_data[%d].stream_id = %d\n", i, p_resp_stream_st->stream_type,
        i, p_resp_stream_st->stream_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\t stream_data[%d].local_sdp",i));
    p_sprintf_count = icf_cmn_trace_stream_sdp(&(p_resp_stream_st->local_sdp),p_resp_stream_st->stream_type,p_sprintf_count);
	
   
	return p_sprintf_count;
}



icf_uint8_t* icf_cmn_trace_app_add_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t				header_name_list_count = 0;
    
    icf_app_add_req_st*      p_app_req = ICF_NULL;
    icf_route_info_st*       p_rt_info = ICF_NULL;
	icf_filter_st			 *p_filter_list=ICF_NULL;
	icf_uint32_t			 i;
    
	icf_list_st				 *p_media_profile_list = ICF_NULL;
    p_app_req = (icf_app_add_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_app_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_app_req is NULL.\n"))
        return p_sprintf_count;
    }        
    p_rt_info = (icf_route_info_st*)p_app_req->routing_info.ptr;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_rt_info)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : rt_info is NULL.\n"))
        return p_sprintf_count;
    }  
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nfilter.count = %d, filter.match_all = %d", 
        p_app_req->filter.count, p_app_req->filter.match_all));

	
	for(i=0,p_filter_list=p_app_req->filter.p_filter_list;i<p_app_req->filter.count;i++)
	{
        /*Fix For SPR : 18999*/
        if (ICF_NULL == p_filter_list)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_filter_list is NULL.\n"))
            return p_sprintf_count;
        }  
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFilter%d\n  filter_type = %d\n  match_type = %d\n  target_header = %s\n  expr_str = %s", 
		i+1,
        p_filter_list->filter_type,
		p_filter_list->match_type,
		p_filter_list->target_header.str,
		p_filter_list->expr_str.str));

        p_filter_list=p_filter_list->p_next;
	}

    p_sprintf_count = icf_cmn_trace_transport_addr(((icf_transport_addr_st*)&(p_rt_info->ipc.udp.ip)),p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\np_rt_info.port = %d", p_rt_info->ipc.udp.port));
    
	
    header_name_list_count = (icf_uint8_t)p_app_req->header_name_list.count;
    p_sprintf_count = icf_cmn_trace_header_name_list(p_app_req->header_name_list.hdr, header_name_list_count,p_sprintf_count);

    p_sprintf_count = icf_cmn_trace_string_list(&(p_app_req->supported_methods),(icf_uint8_t *)"supported_methods",p_sprintf_count);

    p_sprintf_count = icf_cmn_trace_string_list(&(p_app_req->supported_events),(icf_uint8_t *)"supported_events",p_sprintf_count);

    p_sprintf_count = icf_cmn_trace_string_list(&(p_app_req->supported_content_types),(icf_uint8_t *)"supported_content_types",p_sprintf_count);
    
    p_sprintf_count = icf_cmn_trace_string_list(&(p_app_req->supported_encodings),(icf_uint8_t *)"supported_encodings",p_sprintf_count);

    p_sprintf_count = icf_cmn_trace_string_list(&(p_app_req->supported_languages),(icf_uint8_t *)"supported_languages",p_sprintf_count);
    


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nMedia Profile List:"));
	p_media_profile_list= p_app_req->p_media_profile;
	i = 1;
	while(p_media_profile_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
		p_sprintf_count = icf_cmn_trace_stream_capab(p_media_profile_list->p_data,p_sprintf_count);
		p_media_profile_list = p_media_profile_list->p_next;
		i++;
	}



     p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nrtp_detection_reqd = %d", 
        p_app_req->rtp_detection_reqd));


    return p_sprintf_count;
}




icf_uint8_t* icf_cmn_trace_app_add_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_app_add_cfm_st*      p_app_cfm = ICF_NULL;
    
    p_app_cfm = (icf_app_add_cfm_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_app_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_app_cfm is NULL.\n"))
        return p_sprintf_count;
    }   
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    /* g_default_app_id = p_msg->hdr.app_id; */

   

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "bit_mask = %d, result = %d",
        p_app_cfm->bit_mask, p_app_cfm->result));
     if(ICF_FALSE == p_app_cfm->result)
     {
          p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        ", error_cause = %d",
         p_app_cfm->error_cause));

         p_sprintf_count = icf_cmn_trace_error_details(p_app_cfm->error_cause,p_sprintf_count);
     }

#ifdef ICF_NW_ACTIVATE

	if (p_app_cfm->bit_mask & ICF_APP_ADD_CFM_NET_INFO_PRESENT) {

		num_of_elements = p_app_cfm->num_nw;
		
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnum_nw = %d", num_of_elements);
		

		for (i = 0; i < num_of_elements; i++) {
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
				"\nnw_info[%d].bit_mask = %d", 
				i, p_app_cfm->nw_info[i].bit_mask);

            if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT) {
    			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnw_info[%d].ctxt_id = %d", 
                    i,p_app_cfm->nw_info[i].ctxt_id));
            }
			
            if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_SELF_IP_PRESENT) {
    			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnw_info[%d].self_ip", i));
				p_sprintf_count = icf_cmn_trace_transport_addr(&(p_app_cfm->nw_info[i].self_ip),p_sprintf_count);
            }
			
			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    (icf_uint8_t *)"\nnw_info[%d].access_type = %d", 
                    i, p_app_cfm->nw_info[i].access_type));

			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    (icf_uint8_t *)"\nnw_info[%d].access_info.str = %s", 
                    i, p_app_cfm->nw_info[i].access_info.str));
            

			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT) 
            {
                p_sprintf_count = icf_cmn_trace_pub_uri_list(p_app_cfm->nw_info[i].p_uri_list,p_sprintf_count);
            }
			
			if (p_app_cfm->nw_info[i].bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT) {
				asso_num = p_app_cfm->nw_info[i].out_proxy_addr.num_of_elements;
				
				
				for (j = 0; j < asso_num; j++) {
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
						"\nnw_info[%d].out_proxy_addr.addr", j);
					p_sprintf_count = icf_cmn_trace_transport_addr(&(p_app_cfm->nw_info[i].out_proxy_addr.addr[i]),p_sprintf_count);
										
				}
			}

			
		}
	}

#endif
    return p_sprintf_count;

}




icf_uint8_t* icf_cmn_trace_app_remove_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
       
    icf_app_remove_req_st*      p_app_rem = ICF_NULL;


    p_app_rem = (icf_app_remove_req_st*)p_msg->payload;
    
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_app_rem)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_app_rem is NULL.\n"))
        return p_sprintf_count;
    }     

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\noption = %d", p_app_rem->option));
  
    return p_sprintf_count;

}





icf_uint8_t* icf_cmn_trace_app_remove_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        
    icf_app_remove_cfm_st*      p_app_cfm = ICF_NULL;
 
    p_app_cfm = (icf_app_remove_cfm_st*)p_msg->payload;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_app_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_app_cfm is NULL.\n"))
        return p_sprintf_count;
    }  

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nresult = %d, error_cause = %d",
        p_app_cfm->result, p_app_cfm->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_app_cfm->error_cause,p_sprintf_count);


    return p_sprintf_count;

}



#ifdef ICF_NW_ACTIVATE

icf_uint8_t* icf_cmn_trace_network_activate_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_network_activate_req_st*      p_net_req = ICF_NULL;
    icf_route_info_st*          rt_info = ICF_NULL;
    p_net_req = (icf_network_activate_req_st*)p_msg->payload;
    rt_info = (icf_route_info_st*)p_net_req->routing_info.p_sprintf_count;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\nsource_id = %d, dest_id = %d"
        (icf_uint8_t *)"\nversion = %d, call_id = %d"
        (icf_uint8_t *)"\naccess_type = %d, access_info.str = %s"
        (icf_uint8_t *)"\nrt_info len = %d, rt_info type = %d"
        (icf_uint8_t *)"\nrt_info addr_type = %d, rt_info octet1 = %d"
        (icf_uint8_t *)"\nrt_info octet2 = %d, rt_info octet3 = %d"
        (icf_uint8_t *)"\nrt_info octet4 = %d, rt_info port = %d",
         p_msg->hdr.source_id, p_msg->hdr.destination_id, 
        p_msg->hdr.version, p_msg->hdr.call_id, 
        p_net_req->access_type, p_net_req->access_info.str, 
		p_net_req->routing_info.len, rt_info->type,
        rt_info->ipc.udp.ip.addr_type, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_1,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_2, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_3,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_4, rt_info->ipc.udp.port);


    return p_sprintf_count;

}




icf_uint8_t* icf_cmn_trace_event_indication(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_event_indication_st*      p_event_ind = ICF_NULL;
    
    p_event_ind = (icf_event_indication_st*)p_msg->payload;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);




    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\nevent_code = %d",
        p_event_ind->event_code));


    return p_sprintf_count;

}




icf_uint8_t* icf_cmn_trace_network_activate_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        
    icf_network_activate_cfm_st*      p_net_cfm = ICF_NULL;
    icf_uint8_t  i = 0, num_of_elements = 0, asso_num = 0, j = 0;

    p_net_cfm = (icf_network_activate_cfm_st*)p_msg->payload;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\nbit_mask = %d, result = %d, error_cause = %d",
        p_net_cfm->bit_mask, p_net_cfm->result, p_net_cfm->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_net_cfm->error_cause,p_sprintf_count);




	if (p_net_cfm->bit_mask & ICF_NET_ACT_CFM_NW_INFO_PRESENT) {

		num_of_elements = p_net_cfm->num_nw;
		
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnum_nw = %d", num_of_elements);
		

		for (i = 0; i < num_of_elements; i++) {
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
				"\nnw_info[%d].bit_mask = %d", 
				i, p_net_cfm->nw_info[i].bit_mask);

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT) {
    			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnw_info[%d].ctxt_id = %d", 
                    i,p_net_cfm->nw_info[i].ctxt_id));
            }
			
            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_SELF_IP_PRESENT) {
    			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnw_info[%d].self_ip", i));
				p_sprintf_count = icf_cmn_trace_transport_addr(&(p_net_cfm->nw_info[i].self_ip),p_sprintf_count);
            }
			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    (icf_uint8_t *)"\nnw_info[%d].access_type = %d", 
                    i, p_net_cfm->nw_info[i].access_type));

			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    (icf_uint8_t *)"\nnw_info[%d].access_info.str = %s", 
                    i, p_net_cfm->nw_info[i].access_info.str));
            

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT) 
            {
                p_sprintf_count = icf_cmn_trace_pub_uri_list(p_net_cfm->nw_info[i].p_uri_list,p_sprintf_count);
            }

			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT) {
				asso_num = p_net_cfm->nw_info[i].out_proxy_addr.num_of_elements;
				
				
				for (j = 0; j < asso_num; j++) {
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
						"\nnw_info[%d].out_proxy_addr.addr", j);
					p_sprintf_count = icf_cmn_trace_transport_addr(&(p_net_cfm->nw_info[i].out_proxy_addr.addr[i]),p_sprintf_count);
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n"));
					
				}
			}

			
		}
	}


 
    return p_sprintf_count;

}





icf_uint8_t* icf_cmn_trace_network_deactivate_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_network_deactivate_req_st*      p_net_req = ICF_NULL;
    icf_route_info_st*          rt_info = ICF_NULL;
    p_net_req = (icf_network_deactivate_req_st*)p_msg->payload;
    rt_info = (icf_route_info_st*)p_net_req->routing_info.p_sprintf_count;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\nsource_id = %d, dest_id = %d"
        (icf_uint8_t *)"\nversion = %d, call_id = %d"
        (icf_uint8_t *)"\nctxt id = %d"
        (icf_uint8_t *)"\nrt_info len = %d, rt_info type = %d"
        (icf_uint8_t *)"\nrt_info addr_type = %d, rt_info octet1 = %d"
        (icf_uint8_t *)"\nrt_info octet2 = %d, rt_info octet3 = %d"
        (icf_uint8_t *)"\nrt_info octet4 = %d, rt_info port = %d",     
        p_msg->hdr.source_id, p_msg->hdr.destination_id, 
        p_msg->hdr.version, p_msg->hdr.call_id, 
        p_net_req->ctxt_id, 
		p_net_req->routing_info.len, rt_info->type,
        rt_info->ipc.udp.ip.addr_type, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_1,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_2, rt_info->ipc.udp.ip.addr.ipv4_addr.octet_3,
        rt_info->ipc.udp.ip.addr.ipv4_addr.octet_4, rt_info->ipc.udp.port);



    return p_sprintf_count;

}




icf_uint8_t* icf_cmn_trace_network_deactivate_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        
    icf_network_deactivate_cfm_st*      p_net_cfm = ICF_NULL;
    icf_uint8_t  i = 0, num_of_elements = 0, asso_num = 0, j = 0;

    p_net_cfm = (icf_network_deactivate_cfm_st*)p_msg->payload;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);




    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\nbit_mask = %d, result = %d, error_cause = %d",
        p_net_cfm->bit_mask, p_net_cfm->result, p_net_cfm->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_net_cfm->error_cause,p_sprintf_count);




	if (p_net_cfm->bit_mask & ICF_NET_DEACT_CFM_NW_INFO_PRESENT) {

		num_of_elements = p_net_cfm->num_nw;
		
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnum_nw = %d", num_of_elements);
		

		for (i = 0; i < num_of_elements; i++) {
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
				"\nnw_info[%d].bit_mask = %d", 
				i, p_net_cfm->nw_info[i].bit_mask);

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_CTXT_ID_PRESENT) {
    			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnw_info[%d].ctxt_id = %d", 
                    i,p_net_cfm->nw_info[i].ctxt_id));
            }
			
            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_SELF_IP_PRESENT) {
    			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnw_info[%d].self_ip", i));
				p_sprintf_count = icf_cmn_trace_transport_addr(&(p_net_cfm->nw_info[i].self_ip),p_sprintf_count);
            }
			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_TYPE_PRESENT)
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    (icf_uint8_t *)"\nnw_info[%d].access_type = %d", 
                    i, p_net_cfm->nw_info[i].access_type));

			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ACCESS_INFO_PRESENT)
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    (icf_uint8_t *)"\nnw_info[%d].access_info.str = %s", 
                    i, p_net_cfm->nw_info[i].access_info.str));
            

            if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_ASSO_URI_PRESENT) 
            {
                p_sprintf_count = icf_cmn_trace_pub_uri_list(p_net_cfm->nw_info[i].p_uri_list,p_sprintf_count);
            }

			
			if (p_net_cfm->nw_info[i].bit_mask & ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT) {
				asso_num = p_net_cfm->nw_info[i].out_proxy_addr.num_of_elements;
				
				
				for (j = 0; j < asso_num; j++) {
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
						"\nnw_info[%d].out_proxy_addr.addr", j);
					p_sprintf_count = icf_cmn_trace_transport_addr(&(p_net_cfm->nw_info[i].out_proxy_addr.addr[i]),p_sprintf_count);
										
				}
			}

			
		}
	}


    return p_sprintf_count;

}


#endif


icf_uint8_t* icf_cmn_trace_register_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_register_req_st* p_reg_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0;
    p_reg_req = (icf_register_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reg_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reg_req is NULL.\n"))
        return p_sprintf_count;
    }  

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



/* Header LIST  */    if (p_reg_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reg_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_reg_req->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Tag LIST  */    if (p_reg_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_reg_req->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_reg_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    /* Body LIST  */
    if (p_reg_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reg_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_reg_req->body_list.body, body_list_count,p_sprintf_count);
    }

    /*Contact list required*/
    if (p_reg_req->bit_mask & ICF_REG_REQ_CONTACT_LIST_REQD)
    {
       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
          "\ncontact_list_required = %u", 
          p_reg_req->contact_list_required));

    }
     
    /*Reg Event package subscription required*/
    if (p_reg_req->bit_mask & ICF_REG_REQ_REG_EVENT_PCKG_SUBS_FLAG_PRESENT)
    {
       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
          "\nreg_event_pckg_subs_required = %u", 
          p_reg_req->reg_event_pckg_subs_required));

    }

    /*  registration_refresh_required */
    if (p_reg_req->bit_mask & ICF_REG_REQ_REFRESH_REG_FLAG_PRESENT)
    {
       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nregistration_refresh_required = %d", 
        p_reg_req->registration_refresh_required));
    }

    /* security_client */
    if(p_reg_req->bit_mask & ICF_REG_REQ_SECURITY_CLIENT_PRESENT)
    {
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
               "\nsecurity_client.str = %s",
                 p_reg_req->security_client.str));
    }

    /* SPR 19886 : Additional fix done while merging CSR 1-7193955 */
    if(p_reg_req->bit_mask & ICF_REG_REQ_REFRESH_MULTIPLIER_PRESENT)
    {
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
               "\nregistration_refresh_multiplier = %d",
                 p_reg_req->registration_refresh_multiplier));
    }
 
    if (ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT & p_reg_req->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    "\nDeregister Specific Contact = %d", 
                    p_reg_req->dereg_specific_contact));

    }
 
    return p_sprintf_count;

}






icf_uint8_t* icf_cmn_trace_register_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_register_cfm_st* p_reg_cfm = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0;
    
    p_reg_cfm = (icf_register_cfm_st*)p_msg->payload;
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reg_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reg_cfm is NULL.\n"))
        return p_sprintf_count;
    }  

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "bit_mask = %d, line_id = %d, request_type = %d"
        "\nresult = %d, error_cause = %d",
        p_reg_cfm->bit_mask, p_reg_cfm->line_id, p_reg_cfm->request_type, 
        p_reg_cfm->result, p_reg_cfm->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_reg_cfm->error_cause,p_sprintf_count);

    p_sprintf_count = icf_cmn_trace_pub_uri_list(p_reg_cfm->p_uri_list,p_sprintf_count);


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nto.addr_type = %d, to.addr_val.str = %s", 
        p_reg_cfm->to.addr_type, p_reg_cfm->to.addr_val.str));


/* Header LIST  */    if (p_reg_cfm->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reg_cfm->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_reg_cfm->header_list.hdr, header_list_count,p_sprintf_count);
    }
  
    /* Body LIST  */
    if (p_reg_cfm->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count =(icf_uint8_t) p_reg_cfm->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_reg_cfm->body_list.body, body_list_count,p_sprintf_count);
    }
  
    /* Response code */

    if (p_reg_cfm->bit_mask & ICF_REG_CFM_RESP_CODE_PRESENT) 
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\nresponse_code = %d", 
            p_reg_cfm->response_code));
    }
     

  
    return p_sprintf_count;

}



icf_uint8_t* icf_cmn_trace_register_status_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
       

    icf_register_status_ind_st* p_reg_stat = ICF_NULL;

    icf_uint8_t  header_list_count = 0;      

    p_reg_stat = (icf_register_status_ind_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reg_stat)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reg_stat is NULL.\n"))
        return p_sprintf_count;
    }  

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d, line_id = %d, nrequest_type = %d"
        "\nresult = %d, error_cause = %d",
        p_reg_stat->bit_mask, p_reg_stat->line_id, p_reg_stat->request_type, 
        p_reg_stat->result, p_reg_stat->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_reg_stat->error_cause,p_sprintf_count);

        p_sprintf_count = icf_cmn_trace_pub_uri_list(p_reg_stat->p_uri_list,p_sprintf_count);


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nto.addr_type = %d, to.addr_val.str = %s", 
        p_reg_stat->to.addr_type, p_reg_stat->to.addr_val.str));


/* Header LIST  */    if (p_reg_stat->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reg_stat->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_reg_stat->header_list.hdr, header_list_count,p_sprintf_count);
    }
    /* Response Code */


    if (p_reg_stat->bit_mask & ICF_REG_IND_RESP_CODE_PRESENT) 
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\nresponse_code = %d", 
            p_reg_stat->response_code));
    }

    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_aka_auth_resp(
	INOUT icf_msg_st *p_msg,
    OUT   icf_uint8_t *p_sprintf_count)
{
    icf_aka_auth_resp_st *p_aka_auth_resp = ICF_NULL;
    p_aka_auth_resp = (icf_aka_auth_resp_st*)p_msg->payload;

    /* Fix For SPR 19189 */
    if (ICF_NULL == p_aka_auth_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_aka_auth_resp is NULL.\n"))
        return p_sprintf_count;
    }  

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_aka_auth_resp->bit_mask));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
        " Sip Method Name = %d",p_aka_auth_resp->sip_method_name));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
        "\nLine id = %d",p_aka_auth_resp->line_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
        " Result = %d",p_aka_auth_resp->result));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
            "\nUser Address = (addr_type = %d, %s)",p_aka_auth_resp->
            user_address.addr_type,p_aka_auth_resp->user_address.addr_val.str));

    if (p_aka_auth_resp->bit_mask & ICF_AKA_AUTH_RESP_ERROR_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count, 
			"\nError Cause = %d",p_aka_auth_resp->error_cause));
	}

    if (p_aka_auth_resp->bit_mask & ICF_AKA_AUTH_RESPONSE_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count, 
		"\nAuth Response = %s",p_aka_auth_resp->auth_response.str));
	}

    if (p_aka_auth_resp->bit_mask & ICF_AKA_AUTH_AUTS_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count, 
			"\nAuts token = %s",p_aka_auth_resp->auts_token.str));
	}

    if (p_aka_auth_resp->bit_mask & ICF_AKA_AUTH_IK_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count, 
			"\nIk = %s",p_aka_auth_resp->Ik.str));
	}
    if (p_aka_auth_resp->bit_mask & ICF_AKA_AUTH_CK_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count, 
			"\nCk = %s",p_aka_auth_resp->Ck.str));
	}
    if (p_aka_auth_resp->bit_mask & ICF_AKA_AUTH_SEQ_NUM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count, 
			"\nsequence_num = %d",p_aka_auth_resp->sequence_num));
	}

    /* security_association_ind */
    if (p_aka_auth_resp->bit_mask &
                    ICF_AKA_AUTH_SECURITY_ASSOCIATION_REQD_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
			       (icf_int8_t *)"\nsecurity_association_reqd = %d",
                    p_aka_auth_resp->security_association_reqd));
	}

    return p_sprintf_count;
} 

    
icf_uint8_t* icf_cmn_trace_subscribe_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_subscribe_req_st* p_sub_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
       
    p_sub_req = (icf_subscribe_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_sub_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_sub_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_sub_req->bit_mask));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\ndestination_info = (addr_type = %d, %s)",
		p_sub_req->destination_info.addr_type,
		p_sub_req->destination_info.addr_val.str));



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nevent_pkg_name.str = %s, duration = %d"
        "\npref_identity.str = %s", 
        p_sub_req->event_pkg_name.str, p_sub_req->duration, 
        p_sub_req->pref_identity.addr_val.str));
	
	


	/* Header LIST  */    if (p_sub_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count =(icf_uint8_t) p_sub_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_sub_req->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Tag LIST  */    if (p_sub_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_sub_req->tag_list.count;
		p_sprintf_count = icf_cmn_trace_tag_list(p_sub_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_sub_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_sub_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_sub_req->body_list.body, body_list_count,p_sprintf_count);
    }
    
    if(p_sub_req->bit_mask & ICF_SUB_REQ_SUBS_HEAD_START_DURATION_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                    "\nsubs_head_start_duration = %d", p_sub_req->subs_head_start_duration));
    }

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_nw_buff_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
	icf_trace_nw_buff_ind_st* p_nw_buff_ind = ICF_NULL;
	icf_transport_addr_type_t addr_type = ICF_NULL;

	p_nw_buff_ind = (icf_trace_nw_buff_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_nw_buff_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_nw_buff_ind is NULL.\n"))
        return p_sprintf_count;
    }  
	p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

 
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
        "\n NW Address : addr_type = %d, ",
        p_nw_buff_ind->nw_address.addr.addr_type));
    
    
    addr_type = p_nw_buff_ind->nw_address.addr.addr_type;
    
    
    switch (addr_type) {
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n%s, ", 
            p_nw_buff_ind->nw_address.addr.addr.domain.str));
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n%d.%d.%d.%d, ", 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_1, 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_2, 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_3, 
            p_nw_buff_ind->nw_address.addr.addr.ipv4_addr.octet_4));
        break;
        
    }
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nport_num = %d",
        p_nw_buff_ind->nw_address.port_num));

	p_nw_buff_ind->nw_buffer.p_buff[p_nw_buff_ind->nw_buffer.len] = 0;

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n%s",
			p_nw_buff_ind->nw_buffer.p_buff));

	return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_subscribe_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_subscription_status_ind_st* p_sub_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_sub_resp = (icf_subscription_status_ind_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_sub_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_sub_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_sub_resp->bit_mask));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\nICF_SUBS_ACCEPTED\t\t\t\t0"));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_SUBS_SUBSCRIBED\t\t\t\t1"));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_SUBS_FAIL_RETRY\t\t\t\t2"));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_SUBS_FAIL_NO_RETRY\t\t\t\t3"));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nICF_SUBS_UNSUBSCRIBED\t\t\t\t4"));
	
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nsubscription_status = %d",
        p_sub_resp->status));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nduration = %d ",
        p_sub_resp->duration));

	if (p_sub_resp->bit_mask & ICF_SUBS_ST_IND_RESP_CODE_PRESENT) 
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nresponse_code = %d ",
			p_sub_resp->response_code));
	}

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nerror_code = %d ",
        p_sub_resp->error_code));
	



	/* Header LIST  */    if (p_sub_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_sub_resp->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_sub_resp->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Event package name for which subscribe request was sent */
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\nevent_pkg_name = %s\n", p_sub_resp->event_pkg_name.str));


   
    return p_sprintf_count;

}





icf_uint8_t* icf_cmn_trace_publish_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_publish_req_st* p_pub_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
       
    p_pub_req = (icf_publish_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_pub_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_pub_req is NULL.\n"))
        return p_sprintf_count;
    }  
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);




    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_pub_req->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\ndestination_info = (addr_type = %d, %s)",
		p_pub_req->destination_info.addr_type,
		p_pub_req->destination_info.addr_val.str));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
        "\nline_id = %d", p_pub_req->line_id));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
        "\nevent_pkg_name.str = %s, duration = %d", 
    p_pub_req->event_pkg_name.str, p_pub_req->duration));

    if (p_pub_req->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
                    "\nuser_address.str = %s",p_pub_req->user_address.addr_val.str));
    }
	
    if (p_pub_req->bit_mask & ICF_PREF_IDENTITY_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,
                    "\npref_identity.str = %s",p_pub_req->pref_identity.addr_val.str));
    }

	/* Header LIST  */    if (p_pub_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_pub_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_pub_req->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Tag LIST  */    if (p_pub_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_pub_req->tag_list.count;
		p_sprintf_count = icf_cmn_trace_tag_list(p_pub_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_pub_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count =(icf_uint8_t) p_pub_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_pub_req->body_list.body, body_list_count,p_sprintf_count);
    }
    


    return p_sprintf_count;


}



icf_uint8_t* icf_cmn_trace_publish_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_publish_resp_st* p_pub_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_pub_resp = (icf_publish_resp_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_pub_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_pub_resp is NULL.\n"))
        return p_sprintf_count;
    }  
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);



	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_pub_resp->bit_mask));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nresult = %d, response_code = %d",
        p_pub_resp->result, p_pub_resp->response_code));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nerror_code = %d, duration = %d entity_tag = %s",
        p_pub_resp->error_code, p_pub_resp->duration, p_pub_resp->entity_tag.str));
	



	/* Header LIST  */    if (p_pub_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_pub_resp->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_pub_resp->header_list.hdr, header_list_count,p_sprintf_count);
    }
    if(ICF_RETRY_AFTER_DURATION_PRESENT & p_pub_resp->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
              "\nRetry After duration = %d", p_pub_resp->retry_after_duration));
    }
    

    return p_sprintf_count;

}



icf_uint8_t* icf_cmn_trace_message_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_message_req_st* p_msg_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
   
    
    p_msg_req = (icf_message_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_msg_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_msg_req is NULL.\n"))
        return p_sprintf_count;
    }  
    
   p_sprintf_count =  icf_cmn_trace_api_header(p_msg,p_sprintf_count);



    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_msg_req->bit_mask));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\ndestination_info = (addr_type = %d, %s)",
		p_msg_req->destination_info.addr_type,
		p_msg_req->destination_info.addr_val.str));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\npref_identity.str = %s", 
        p_msg_req->pref_identity.addr_val.str));

    if(p_msg_req->bit_mask & ICF_USER_ADDRESS_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *) (icf_int8_t *)p_sprintf_count,
        "\nuser_address.str = %s", 
        p_msg_req->user_address.addr_val.str));
    }
	
	/* Header LIST  */    if (p_msg_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_msg_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_msg_req->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Tag LIST  */    if (p_msg_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_msg_req->tag_list.count;
		p_sprintf_count = icf_cmn_trace_tag_list(p_msg_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_msg_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_msg_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_msg_req->body_list.body, body_list_count,p_sprintf_count);
    }
    


    return p_sprintf_count;


}



icf_uint8_t* icf_cmn_trace_message_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_message_resp_st* p_msg_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_msg_resp = (icf_message_resp_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_msg_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_msg_resp is NULL.\n"))
        return p_sprintf_count;
    }  
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_msg_resp->bit_mask));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nresult = %d, response_code = %d",
        p_msg_resp->result, p_msg_resp->response_code));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nerror_code = %d",
        p_msg_resp->error_code));
	



	/* Header LIST  */    if (p_msg_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_msg_resp->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_msg_resp->header_list.hdr, header_list_count,p_sprintf_count);
    }
    if(ICF_RETRY_AFTER_DURATION_PRESENT & p_msg_resp->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
              "\nRetry After duration = %d", p_msg_resp->retry_after_duration));
    }
    

    return p_sprintf_count;

}




icf_uint8_t* icf_cmn_trace_message_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_message_ind_st* p_msg_ind = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0;

    
    p_msg_ind = (icf_message_ind_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_msg_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_msg_ind is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


 
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_msg_ind->bit_mask));


	/* Header LIST  */    if (p_msg_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_msg_ind->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_msg_ind->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Body LIST  */
    if (p_msg_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_msg_ind->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_msg_ind->body_list.body, body_list_count,p_sprintf_count);
    }

    return p_sprintf_count;


}

icf_uint8_t* icf_cmn_trace_codec_info_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_cfg_codec_info_req_st* p_ci_req = NULL;
    
    icf_uint32_t k = 0,i = 0,j = 0;
	icf_list_st		*p_enc_parm = NULL;

    p_ci_req = (icf_cfg_codec_info_req_st*)p_msg->payload;

    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_ci_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_ci_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Count = %d", p_ci_req->count));

	for(i=0;(i<p_ci_req->count) && (i< ICF_MAX_CODEC_SUPPORTD) ;i++)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Codec Type = %d", p_ci_req->codec_supp[i].codec_type));
	
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Codec Name = "));

		for(k=0;(k<p_ci_req->codec_supp[i].codec_name.str_len) && (k< ICF_MAX_SHORT_STR_LEN) ;k++)

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c", p_ci_req->codec_supp[i].codec_name.str[k]));

		if(p_ci_req->codec_supp[i].bitmask & ICF_ENCODED_PARAM_PRESENT)
		{
			p_enc_parm = p_ci_req->codec_supp[i].p_encoded_params;

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of encoded params\n"));

			while(p_enc_parm != ICF_NULL)
			{
                /*Fix For SPR : 18999*/
                if (ICF_NULL == p_enc_parm->p_data)
                {
                    ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_enc_parm->p_data is NULL.\n"))
                    return p_sprintf_count;
                }   
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," "));

				for(j=0;(j<((icf_string_st*)(p_enc_parm->p_data))->str_len) && (j < ICF_MAX_STR_LEN);j++)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
							((icf_string_st*)p_enc_parm->p_data)->str[j]));
				}

				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n"));
				p_enc_parm = p_enc_parm->p_next;
			}
		}

		if(p_ci_req->codec_supp[i].bitmask & ICF_CODEC_PARAM_PRESENT)
		{
			if(ICF_CODEC_TYPE_AUDIO == p_ci_req->codec_supp[i].codec_type)
			{
				if(ICF_AUDIO_CODEC_CHANNEL & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Channels = %d",p_ci_req->codec_supp[i].codec_param.audio_param.channels));

				if(ICF_AUDIO_CODEC_PTIME & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of PTime supported = %d",p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supported_ptime));

					for(j=0;(j<p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supported_ptime) && (j < ICF_MAX_SUP_PTIME) ;j++)
					{
						p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n PTime[%d] = %d", j ,
											p_ci_req->codec_supp[i].codec_param.audio_param.ptime[j]));
					}
				}

				if(ICF_AUDIO_CODEC_BANDWIDTH & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Bandwidth = %d",p_ci_req->codec_supp[i].codec_param.audio_param.bandwidth));


				if(ICF_AUDIO_CODEC_SAMPLING_RATE & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Sampling Rate supported = %d",p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supp_sampling_rate));

					for(j=0;(j<p_ci_req->codec_supp[i].codec_param.audio_param.num_of_supp_sampling_rate) && (j < ICF_MAX_SUP_SAMPLING_RATE) ;j++)
					{
						p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Sampling Rate[%d] = %d", j ,
											p_ci_req->codec_supp[i].codec_param.audio_param.sampling_rate[j]));
					}
				}

				if(ICF_AUDIO_CODEC_SILENCE_SUPPRESSION & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Silence Suppression = %d",p_ci_req->codec_supp[i].codec_param.audio_param.silence_suppression));


				if(ICF_AUDIO_CODEC_AMR_PARAM & p_ci_req->codec_supp[i].codec_param.audio_param.bit_mask)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n AMR Initial Mode = %d",p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_initial_mode));

					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Codec Mode = %d",p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.codec_mode));

					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n CRC Enabled = %d",p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.crc_enabled));

					if(ICF_AMR_MODE_LIST_PRESENT & p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.bit_mask)
					{
						p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nAMR Mode Count = %d",
								p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_mode_count));
						for(j=0;(j<p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_mode_count) && (j< ICF_MAX_AMR_MODE);j++)
							p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d\n",
								p_ci_req->codec_supp[i].codec_param.audio_param.amr_params.amr_mode_list[j]));
					}
								
				}

			}

			else if(ICF_CODEC_TYPE_VIDEO == p_ci_req->codec_supp[i].codec_type)
			{
				if(ICF_VIDEO_CFG_CODEC_FRAME_RATE_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Frame Rate = %d",p_ci_req->codec_supp[i].codec_param.video_param.frame_rate));

				if(ICF_VIDEO_CFG_CODEC_ENC_QUALITY_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Encoding Quality = %d",p_ci_req->codec_supp[i].codec_param.video_param.encoding_quality));

				if(ICF_VIDEO_CFG_CODEC_BANDWIDTH_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Bandwidth = %d",p_ci_req->codec_supp[i].codec_param.video_param.bandwidth));

				if(ICF_VIDEO_CFG_CODEC_CLOCK_RATE_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Clock Rate supported = %d",p_ci_req->codec_supp[i].codec_param.video_param.num_of_supp_clock_rate));

					for(j=0;(j<p_ci_req->codec_supp[i].codec_param.video_param.num_of_supp_clock_rate) && (j < ICF_MAX_SUP_CLOCK_RATE) ;j++)
					{
						p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Clock Rate[%d] = %d", j ,
											p_ci_req->codec_supp[i].codec_param.video_param.clock_rate[j]));
					}
				}

				if(ICF_VIDEO_CFG_CODEC_ENC_QUALITY_PRESENT & p_ci_req->codec_supp[i].codec_param.video_param.bit_mask)
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Frame Size - Height = %d",p_ci_req->codec_supp[i].codec_param.video_param.frame_size.height));

					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Frame Size - Width = %d",p_ci_req->codec_supp[i].codec_param.video_param.frame_size.width));
				}

			}

			else if(ICF_CODEC_TYPE_IMAGE == p_ci_req->codec_supp[i].codec_type)
			{
				if(ICF_FAX_T38_VERSION & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Fax Version = %d",p_ci_req->codec_supp[i].codec_param.t38_param.fax_version));

				if(ICF_FAX_T38_BITRATE & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Bit Rate = %d",p_ci_req->codec_supp[i].codec_param.t38_param.bitrate));

				if(ICF_FAX_T38_RATE_MANAGEMENT & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Rate Management = %d",p_ci_req->codec_supp[i].codec_param.t38_param.rate_management));

				if(ICF_FAX_T38_UDP_BUFFSIZE & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n UDP Buffer Size = %d",p_ci_req->codec_supp[i].codec_param.t38_param.udp_buffersize));

				if(ICF_FAX_T38_UDP_PACKETSIZE & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n UDP PACKETSIZE = %d",p_ci_req->codec_supp[i].codec_param.t38_param.udp_packetsize));

				if(ICF_FAX_T38_ERR_CORRECTION & p_ci_req->codec_supp[i].codec_param.t38_param.bit_mask)
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n ERR CORRECTION = %d",p_ci_req->codec_supp[i].codec_param.t38_param.err_correction_method));
				
			}
		}
	}

    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_ringer_tones_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{    
    
    icf_cfg_ringer_tones_req_st* p_req = (icf_cfg_ringer_tones_req_st*)
		p_msg->payload;
    icf_uint8_t i = 0;
    icf_uint16_t num = 0;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }       
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nnum_of_ringer = %d", p_req->num_of_ringer));

    num=p_req->num_of_ringer;

    if(num > ICF_MAX_RINGER_LIST)
		num = ICF_MAX_RINGER_LIST;

    for (i=0; i<num; i++) {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\n\tringer[%d].ringer_id = %d"
			"\n\tringer[%d].play_file.str = %s"
			"\n\tringer[%d].play_file.str_len = %d"
			"\n",
			i,p_req->ringer[i].ringer_id,
			i,p_req->ringer[i].play_file.str,
			i,p_req->ringer[i].play_file.str_len			
			));
    }

   
    
    return p_sprintf_count;

}
icf_uint8_t* icf_cmn_trace_jbm_mgmt_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{    
    
    icf_cfg_jbm_mgmt_req_st* p_req = (icf_cfg_jbm_mgmt_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }       
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
		    "\njbm_type = %d"
		    "\njbm_scaling_factor = %d"
		    "\njbm_initial_size = %d"
		    "\njbm_max_size = %d"
		    "\njbm_min_size = %d",
		    p_req->jbm_type,
		    p_req->jbm_scaling_factor,
		    p_req->jbm_initial_size,
		    p_req->jbm_max_size,
		    p_req->jbm_min_size
	     ));
    
    return p_sprintf_count;

}
icf_uint8_t* icf_cmn_trace_max_earp_vol_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{   
    
    icf_cfg_max_earp_vol_req_st* p_req = (icf_cfg_max_earp_vol_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }       
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nmax_earp_volume = %d", p_req->max_earp_volume));

/*    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask);*/

    
    return p_sprintf_count;

}
icf_uint8_t* icf_cmn_trace_max_speaker_vol_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{    
   
    icf_cfg_max_speaker_vol_req_st* p_req = (icf_cfg_max_speaker_vol_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nmax_speaker_volume = %d", p_req->max_speaker_volume));

   /*p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask);*/

    
    return p_sprintf_count;

}
icf_uint8_t* icf_cmn_trace_port_range_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_cfg_set_port_range_req_st* p_req = (icf_cfg_set_port_range_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask));



	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n start_rtp_port = %d", p_req->start_rtp_port));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n end_rtp_port = %d", p_req->end_rtp_port));

	if(ICF_TRUE == p_req->is_app_rtcp_port_mux)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Multiplexing Enabled"));
	}
	else
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Multiplexing Not Enabled"));
	}

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n start_app_port = %d", p_req->start_app_port));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n end_app_port = %d", p_req->end_app_port));
	



    
    return p_sprintf_count;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_self_ip_req
 *
 * DESCRIPTION:     This function is used to trace cfg_set_self_ip
 *
 *****************************************************************************/
icf_uint8_t* icf_cmn_trace_self_ip_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_cfg_set_self_ip_req_st* p_self_ip_req = (icf_cfg_set_self_ip_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_self_ip_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_self_ip_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSelf IP Address\n"));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nValue = %s", p_self_ip_req->self_ip_addr.addr_val.str));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nAddr Type = %d", p_self_ip_req->self_ip_addr.addr_type));

        return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_digit_trans_req
 *
 * DESCRIPTION:     This function is used to trace cfg_set_digit_trans
 *
 *****************************************************************************/
icf_uint8_t* icf_cmn_trace_digit_trans_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_cfg_set_digit_trans_req_st* p_digit_trans_req = (icf_cfg_set_digit_trans_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_digit_trans_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_digit_trans_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    	
    if(p_digit_trans_req->dgt_trns_method == 1)
    {
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nDgt Trans Method = %s", "RFC_2833"));

    }
    if(p_digit_trans_req->dgt_trns_method == 2)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nDgt Trans Method = %s", "INBAND_TONE"));
    }

   
    return p_sprintf_count;

}



icf_uint8_t* icf_cmn_trace_start_ringing_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   

    icf_start_ringing_req_st* p_req = (icf_start_ringing_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", p_req->bit_mask));

    if (p_req->bit_mask & ICF_RINGING_LINE_ID_PRESENT)
           p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nLine_id = %d", p_req->line_id));
		
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nringer_id = %d \nduration = %d",p_req->ringer_id,p_req->duration));

    if(p_req->bit_mask & ICF_RINGING_TIMER_VAL)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ntimer_value = %d", p_req->timer_value));

        return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_get_stats_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_get_stats_req_st* p_req = (icf_get_stats_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nstream_id = %d"
			"\nstats_type = %d"
			"\ninterval_type = %d", 
			p_req->stream_id,
			p_req->stats_type,
			p_req->interval_type
	     ));

   /* p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask);*/

    
    return p_sprintf_count;

}
icf_uint8_t* icf_cmn_trace_stop_ringing_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{    
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_set_earp_volume_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_set_earp_volume_req_st* p_req = (icf_set_earp_volume_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_req->bit_mask));

    if (p_req->bit_mask & ICF_LINE_ID_PRESENT) 
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_req->line_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nlevel = %d", p_req->level));

/*    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask));*/

    
    return p_sprintf_count;

}
icf_uint8_t* icf_cmn_trace_set_speaker_volume_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_set_speaker_volume_req_st* p_req = (icf_set_speaker_volume_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_req->bit_mask));

    if (p_req->bit_mask & ICF_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_req->line_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nlevel = %d", p_req->level));

/*    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask);*/

   
    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_send_app_data_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_send_app_data_req_st* p_req = (icf_send_app_data_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nstream_id = %d"
			"\n\tapplication_data.str = %s"
			"\n\tapplication_data.str_len = %d"
			"\n\tsequence_num = %d"
			"\n\tsub-type = %d",
			p_req->stream_id,
			p_req->application_data.str,
			p_req->application_data.str_len,
			p_req->sequence_num,
			p_req->application_data.sub_type
	     ));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask));

   
    return p_sprintf_count;

}



icf_uint8_t* icf_cmn_trace_send_stats_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{    
    
    icf_send_stats_req_st* p_req = (icf_send_stats_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nstream_id = %d\nreport_type = %d", p_req->stream_id, p_req->report_type));
    
/*    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", p_req->bit_mask);*/

   
    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_stop_stats_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_stop_stats_req_st* p_req = (icf_stop_stats_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

  
    
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask));

	if(p_req->bit_mask & ICF_STOP_STATS_REQ_STREAM_ID)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nstream_id = %d",p_req->stream_id));
	}

    
    return p_sprintf_count;

}
icf_uint8_t* icf_cmn_trace_stop_ringing_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_stop_ringing_ind_st* p_req = (icf_stop_ringing_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nringer_id = %d",
			p_req->ringer_id
	     ));
   
    return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_rcv_app_data_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_rcv_app_data_ind_st* p_req = (icf_rcv_app_data_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nstream_id = %d"
			"\n\tapplication_data.str = %s"
			"\n\tapplication_data.str_len = %d"
			"\nsequence_num = %d",
			p_req->stream_id,
			p_req->application_data.str,
			p_req->application_data.str_len,
			p_req->sequence_num
	     ));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask));

    
    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_codec_info_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_ringer_tones_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_jbm_mgmt_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_max_earp_vol_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_max_speaker_vol_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_set_port_range_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_ringing_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_media_session_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_stop_ringing_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_stop_vibration_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_send_dgts_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_play_media_spkr_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_set_earp_volume_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_set_speaker_volume_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_send_app_data_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_send_stats_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_stop_stats_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_get_stats_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_get_stats_resp_st* p_get_stats_resp = ICF_NULL;
    
    p_get_stats_resp = (icf_get_stats_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_get_stats_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_get_stats_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nStream ID = %d", p_get_stats_resp->stream_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\ntime_stamp = %d", p_get_stats_resp->rtcp_report.time_stamp));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nnum_packet_send = %d", p_get_stats_resp->rtcp_report.num_packet_send));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nnum_octet_send = %d", p_get_stats_resp->rtcp_report.num_octet_send));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nnum_packet_rcvd = %d", p_get_stats_resp->rtcp_report.num_packet_rcvd));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nnum_octet_rcvd = %d", p_get_stats_resp->rtcp_report.num_octet_rcvd));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\ncum_packet_loss = %d", p_get_stats_resp->rtcp_report.cum_packet_loss));

	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_mm_msg_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_mm_msg_resp_st* p_mm_msg_resp = ICF_NULL;
    
    p_mm_msg_resp = (icf_mm_msg_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_mm_msg_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_mm_msg_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nResult = %d", p_mm_msg_resp->result));
/*
	if(p_mm_msg_resp->bit_mask & ICF_MM_MSG_RESP_ERROR)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
			"\nError Cause = %d", p_mm_msg_resp->error_cause));
	}
*/
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
			"\nError Cause = %d", p_mm_msg_resp->error_cause));
	p_sprintf_count = icf_cmn_trace_error_details(p_mm_msg_resp->error_cause,p_sprintf_count);
		return p_sprintf_count;
}





icf_uint8_t* icf_cmn_trace_refer_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_refer_req_st* p_refer_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, 
					tag_list_count = 0;
        
    p_refer_req = (icf_refer_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_refer_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_refer_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


    

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_refer_req->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\ndestination_info = (addr_type = %d, %s)",
		p_refer_req->destination_info.addr_type,
		p_refer_req->destination_info.addr_val.str));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nrefer_to.str = %s", p_refer_req->refer_to.addr_val.str));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nrefer_to.addr_type = %d", p_refer_req->refer_to.addr_type));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\npref_identity.str = %s", p_refer_req->pref_identity.addr_val.str));
	
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nsubscription_required = %d", p_refer_req->subscription_required));
	


	/* Header LIST  */    if (p_refer_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_refer_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_refer_req->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Tag LIST  */    if (p_refer_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_refer_req->tag_list.count;
		p_sprintf_count = icf_cmn_trace_tag_list(p_refer_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_refer_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_refer_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_refer_req->body_list.body, body_list_count,p_sprintf_count);
    }
    

    
    return p_sprintf_count;


}



icf_uint8_t* icf_cmn_trace_refer_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_refer_resp_st* p_refer_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

    
    p_refer_resp = (icf_refer_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_refer_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_refer_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_refer_resp->bit_mask));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nresult = %d, response_code = %d",
        p_refer_resp->result, p_refer_resp->response_code));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nerror_code = %d",
        p_refer_resp->error_code));
	

	/* Header LIST  */    if (p_refer_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_refer_resp->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_refer_resp->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    if(ICF_RETRY_AFTER_DURATION_PRESENT & p_refer_resp->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
              "\nRetry After duration = %d", p_refer_resp->retry_after_duration));
    }

    
    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_sec_pdp_activate_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_gprs_sec_pdp_activate_req_st *p_msg_payload = ICF_NULL;
    
    p_msg_payload = (icf_gprs_sec_pdp_activate_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_msg_payload)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_msg_payload is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPDP Context ID = %d", p_msg_payload->context_id));
 
		
	if(p_msg_payload->auth_token.str_len)
        {
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nP-Media-Authorization Token  = %s", p_msg_payload->auth_token.str));
         }
 	else
        { 
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nP-Media-Authorization Token is not present..No SBLP implemented\n"));
        }


	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_sec_pdp_activate_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
	icf_port_pltfm_pdp_def_resp_st	*p_port_resp=(icf_port_pltfm_pdp_def_resp_st*)(p_msg->payload);
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_port_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_port_resp is NULL.\n"))
        return p_sprintf_count;
    }      
	
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nPDP Context ID = %d", p_port_resp->context_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nStatus = %d", p_port_resp->status));

	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_pdp_deactivate_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_gprs_pdp_deactivate_req_st *p_msg_payload = ICF_NULL;
    
    p_msg_payload = (icf_gprs_pdp_deactivate_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_msg_payload)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_msg_payload is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nPDP Context ID = %d", p_msg_payload->context_id));


	return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_pdp_modify_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    

    icf_port_pltfm_pdp_mod_ind_st	*p_port_resp=(icf_port_pltfm_pdp_mod_ind_st*)(p_msg->payload);
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_port_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_port_resp is NULL.\n"))
        return p_sprintf_count;
    }         
	
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nPDP Context ID = %d", p_port_resp->context_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nStatus = %d", p_port_resp->status));

	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_pdp_deactivate_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

	icf_port_pltfm_pdp_deact_ind_st	*p_port_resp=(icf_port_pltfm_pdp_deact_ind_st*)(p_msg->payload);
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_port_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_port_resp is NULL.\n"))
        return p_sprintf_count;
    }       
	
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nPDP Context ID = %d", p_port_resp->context_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nStatus = %d", p_port_resp->status));

	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_platform_event_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_port_pltfm_event_ind_st	*p_port_resp=(icf_port_pltfm_event_ind_st*)(p_msg->payload);
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_port_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_port_resp is NULL.\n"))
        return p_sprintf_count;
    }       
	
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\nPDP Event = %d", p_port_resp->event));

	
	return p_sprintf_count;
}


/* -------------------------------------------------------------------------- */
#ifdef ICF_IPSEC_ENABLED

icf_void_t  icf_ipsec_set_spi(icf_uint32_t spi)
{
    if (ICF_NULL == app_global_data.spi_1)
    {
        app_global_data.spi_1 = spi;
        app_global_data.spi_expiry = spi;
    }
    else if (ICF_NULL == app_global_data.spi_2)
    {
        app_global_data.spi_2 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_3)
    {
        app_global_data.spi_3 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_4)
    {
        app_global_data.spi_4 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_5)
    {
        app_global_data.spi_5 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_6)
    {
        app_global_data.spi_6 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_7)
    {
        app_global_data.spi_7 = spi;
    }
    else if (ICF_NULL == app_global_data.spi_8)
    {
        app_global_data.spi_8 = spi;
    }
}

icf_uint8_t* icf_cmn_trace_sa_create_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{    
	return p_sprintf_count;
}

#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
icf_uint8_t* icf_cmn_trace_sa_modify_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	return p_sprintf_count;
}
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */

icf_uint8_t* icf_cmn_trace_sa_delete_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_sa_cmd_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_sa_expiry_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	return p_sprintf_count;
}

#endif /* ICF_IPSEC_ENABLED */


/*ICF_HANDLE_REINVITE_START*/

icf_uint8_t* icf_cmn_trace_incoming_call_modify_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_incoming_call_modify_ind_st* p_reinvite_ind = NULL;
    
    icf_uint8_t  header_list_count = 0,body_list_count = 0,i=0;

    icf_list_st *p_list=ICF_NULL;
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_reinvite_ind = (icf_incoming_call_modify_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reinvite_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reinvite_ind is NULL.\n"))
        return p_sprintf_count;
    }       
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d\n", 
              p_reinvite_ind->bit_mask));

/* Header LIST  */    if (p_reinvite_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_ind->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_reinvite_ind->header_list.hdr, header_list_count,p_sprintf_count);
    }

/* Body LIST  */
    if (p_reinvite_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_ind->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_reinvite_ind->body_list.body, body_list_count,p_sprintf_count);
    }


     p_list=p_reinvite_ind->p_active_stream_list;
  
	if(p_reinvite_ind->bit_mask & ICF_INC_CALL_ACCEPT_STREAM_LIST_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\n Accepted Stream List:\n"));

		i = 1;

		while(p_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

			p_sprintf_count = icf_cmn_trace_modify_media_change_req(p_list->p_data,p_sprintf_count);
			
			p_list = p_list->p_next;
			
			i++;
		}
	}

	if(p_reinvite_ind->bit_mask & ICF_INC_CALL_MOD_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Rejected stream count %d:",p_reinvite_ind->count));	

		for(i=0;(i<p_reinvite_ind->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",p_reinvite_ind->deleted_stream_list[i].stream_id));

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",p_reinvite_ind->deleted_stream_list[i].stream_type));
		}
	}


   
    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_call_modify_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_uint8_t i;
    icf_call_modify_cfm_st* p_reinvite_cfm = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_reinvite_cfm = (icf_call_modify_cfm_st *)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reinvite_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reinvite_cfm is NULL.\n"))
        return p_sprintf_count;
    }       
     
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_reinvite_cfm->bit_mask));
     

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmedia_change_result = %d", 
              p_reinvite_cfm->result));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nreject_reason = %d", 
              p_reinvite_cfm->cause));

/* Header LIST  */    if (p_reinvite_cfm->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_cfm->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_reinvite_cfm->header_list.hdr, header_list_count,p_sprintf_count);
    }


/* Tag LIST  */    if (p_reinvite_cfm->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_reinvite_cfm->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_reinvite_cfm->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_reinvite_cfm->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_cfm->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_reinvite_cfm->body_list.body, body_list_count,p_sprintf_count);
    }
    if( ICF_CALL_MODIFY_CFM_REJECTED_STREAM_PRESENT & p_reinvite_cfm->bit_mask)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Rejected stream count %d:",p_reinvite_cfm->count));

                for(i=0;(i<p_reinvite_cfm->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
                {
                        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",
			p_reinvite_cfm->deleted_stream_list[i].stream_id));

                        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",
			p_reinvite_cfm->deleted_stream_list[i].stream_type));
                }

    }
    if( ICF_CALL_MODIFY_RESPONSE_SIGNALING_MODE_PRESENT & p_reinvite_cfm->bit_mask)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSignaling Mode = %d",
              p_reinvite_cfm->signaling_mode));
    }
    
   
    return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_incoming_call_modify_cancel_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_call_modify_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_call_modify_resp_st* p_reinvite_resp = NULL;

    icf_uint8_t  header_list_count = 0,body_list_count = 0,i=0;
 
    icf_list_st *p_list=ICF_NULL;                    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_reinvite_resp = (icf_call_modify_resp_st *)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reinvite_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reinvite_resp is NULL.\n"))
        return p_sprintf_count;
    }      
     
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_reinvite_resp->bit_mask));
     

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmedia_change_result = %d", 
              p_reinvite_resp->result));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nreject_reason = %d", 
              p_reinvite_resp->error_cause));
        p_sprintf_count = icf_cmn_trace_error_details(p_reinvite_resp->error_cause,p_sprintf_count);

	if (p_reinvite_resp->bit_mask & ICF_RESPONSE_CODE_PRESENT) {
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresponse_code = %d", 
              p_reinvite_resp->response_code));
	}

/* Header LIST  */    if (p_reinvite_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_resp->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_reinvite_resp->header_list.hdr, header_list_count,p_sprintf_count);
    }

    
    /* Body LIST  */
    if (p_reinvite_resp->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_resp->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_reinvite_resp->body_list.body, body_list_count,p_sprintf_count);
    }
    

        p_list=p_reinvite_resp->p_active_stream_list;
	if(p_reinvite_resp->bit_mask & ICF_CALL_MOD_ACCEPT_STREAM_LIST_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Accepted Stream List:\n"));

		i = 1;

		while(p_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

			p_sprintf_count = icf_cmn_trace_modify_media_change_req(p_list->p_data,p_sprintf_count);
			
			p_list = p_list->p_next;
			
			i++;
		}
	}

	if(p_reinvite_resp->bit_mask & ICF_CALL_MOD_RESP_REJECT_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Rejected stream count %d:",p_reinvite_resp->count));	

		for(i=0;(i<p_reinvite_resp->count) && ( i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",p_reinvite_resp->deleted_stream_list[i].stream_id));

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",p_reinvite_resp->deleted_stream_list[i].stream_type));
		}
	}

  	if( ICF_RETRY_AFTER_DURATION_PRESENT & p_reinvite_resp->bit_mask)
       {
	   p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nREtry After Duration = %d",
              p_reinvite_resp->retry_after_duration)); 
       }



    
    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_call_modify_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_call_modify_req_st* p_reinvite_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	 *p_media_modify_list;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_reinvite_req = (icf_call_modify_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reinvite_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reinvite_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_reinvite_req->bit_mask));

/* Header LIST  */    if (p_reinvite_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_reinvite_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_reinvite_req->header_list.hdr, header_list_count,p_sprintf_count);
    }

	/* Tag LIST  */    if (p_reinvite_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_reinvite_req->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_reinvite_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }

/* Body LIST  */
    if (p_reinvite_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_reinvite_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_reinvite_req->body_list.body, body_list_count,p_sprintf_count);
    }

	if(p_reinvite_req->bit_mask & ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Accepted Stream List:\n"));

		i = 1;

		p_media_modify_list = p_reinvite_req->p_media_modify_list;

		while(p_media_modify_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

			p_sprintf_count = icf_cmn_trace_modify_media_change_req(p_media_modify_list->p_data,p_sprintf_count);
			
			p_media_modify_list = p_media_modify_list->p_next;
			
			i++;
		}
	}

	if(p_reinvite_req->bit_mask & ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Rejected stream count %d:",p_reinvite_req->count));	

		for(i=0;(i<p_reinvite_req->count) && (i<ICF_MAX_STREAM_PER_CALL) ;i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",p_reinvite_req->deleted_media_streams[i].stream_id));

			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type %d:",p_reinvite_req->deleted_media_streams[i].stream_type));
		}
	}




   
    return p_sprintf_count;

}


/* -------------------------------------------------------------------------- */


/*************** Management Plane Handler (MPH) APIs Starts here *************/




icf_uint8_t* icf_cmn_trace_create_secure_conn_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
	icf_create_secure_conn_resp_st      *p_sec_conn_resp = ICF_NULL;
    p_sec_conn_resp = (icf_create_secure_conn_resp_st *)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_sec_conn_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_sec_conn_resp is NULL.\n"))
        return p_sprintf_count;
    }         

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    /* g_default_app_id = p_msg->hdr.app_id; */

    

	/*	icf_uint16_t	exchange_id;
		icf_result_t	conn_req_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "exchange_id = %d, conn_req_status=%d, conn_handle = %d, error cause = %d",  
		p_sec_conn_resp->exchange_id, p_sec_conn_resp->conn_req_status ,p_sec_conn_resp->conn_handle,
        p_sec_conn_resp->error_cause));
	p_sprintf_count = icf_cmn_trace_error_details(p_sec_conn_resp->error_cause,p_sprintf_count);


	
	return p_sprintf_count;
}



icf_uint8_t* icf_cmn_trace_set_close_secure_conn_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	icf_close_secure_conn_resp_st      *p_close_sec_conn_resp = ICF_NULL;
    p_close_sec_conn_resp = (icf_close_secure_conn_resp_st *)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_close_sec_conn_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_close_sec_conn_resp is NULL.\n"))
        return p_sprintf_count;
    }         

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    /* g_default_app_id = p_msg->hdr.app_id; */

    

	/*	icf_uint16_t	exchange_id;
		icf_result_t	close_conn_status;
		icf_uint32_t	conn_handle;
		icf_uint32_t	error_cause; */

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "conn_status = %d, close_conn_status=%d, conn_handle = %d", 
		p_close_sec_conn_resp->exchange_id, p_close_sec_conn_resp->close_conn_status ,p_close_sec_conn_resp->conn_handle));


	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_secure_conn_status_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

	icf_secure_conn_status_ind_st      *p_conn_status_ind = ICF_NULL;
    p_conn_status_ind = (icf_secure_conn_status_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_conn_status_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_conn_status_ind is NULL.\n"))
        return p_sprintf_count;
    }         

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    /* g_default_app_id = p_msg->hdr.app_id; */

    
	/*	icf_uint8_t		conn_status;
		icf_uint32_t	conn_handle; */

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "conn_status = %d, conn_handle = %d", 
		p_conn_status_ind->conn_status, p_conn_status_ind->conn_handle));

	

	
	return p_sprintf_count;
}





icf_uint8_t* icf_cmn_trace_mgmt_plane_api_proc_failure(
	INOUT icf_msg_st *p_msg,
	INOUT icf_uint8_t* p_sprintf_count)

{
   
    icf_api_proc_failure_st				*p_payload = ICF_NULL; 

    p_payload = (icf_api_proc_failure_st *)p_msg->payload;

	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_set_mgmt_plane_nw_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	icf_mgmt_plane_nw_resp_st      *p_nw_resp = ICF_NULL;
    p_nw_resp = (icf_mgmt_plane_nw_resp_st *)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_nw_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_nw_resp is NULL.\n"))
        return p_sprintf_count;
    }          

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    /* g_default_app_id = p_msg->hdr.app_id; */

   
	/* 
	icf_uint8_t				bitmask;
	icf_api_id_t			req_api;
	icf_uint16_t			exchange_id;
	icf_uint32_t			conn_handle;
	icf_stretchable_hdr_st  entity_tag;
	icf_string_st			content_type;
	icf_header_list_st		header_list;
	icf_uint32_t			response_code;
	icf_uint32_t			payload_length;
	icf_uint8_t				*payload; */

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nexchange_id = %d, conn_handle = %d, payload_length = %d", 
		p_nw_resp->exchange_id, p_nw_resp->conn_handle, p_nw_resp->payload_length));

	if (p_nw_resp->payload_length > 0)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\npayload = %s", p_nw_resp->payload));
	}

	return p_sprintf_count;
}

/*************** Management Plane Handler (MPH) APIs Ends here ***************/

icf_uint8_t* icf_cmn_trace_call_merged_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_cc_pa_call_merged_ind_st* p_call_merge = NULL;
       
    p_call_merge = (icf_cc_pa_call_merged_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_call_merge)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_call_merge is NULL.\n"))
        return p_sprintf_count;
    }       
       
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
       
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmerged_call_id = %d", 
              p_call_merge->merge_call_id));

    return p_sprintf_count;
     
}


icf_uint8_t* icf_cmn_trace_rem_call_transfer_init(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_uint8_t presence_flag = 0;    
    icf_cc_pa_remote_call_transfer_initiated_ind_st* 
    p_remote_call_transfer = NULL;
     
    p_remote_call_transfer = 
    (icf_cc_pa_remote_call_transfer_initiated_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_remote_call_transfer)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_remote_call_transfer is NULL.\n"))
        return p_sprintf_count;
    }     
    
    presence_flag = (icf_uint8_t)p_remote_call_transfer->bit_mask;
   
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nnew_call_id  = %d",
              p_remote_call_transfer->new_call_id));
   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nremote_call_transfer = (addr_type = %d, %s)",
              p_remote_call_transfer->referred_party_addr.addr_type ,
              p_remote_call_transfer->referred_party_addr.addr_val.str));

    if(presence_flag & ICF_CC_PA_REFER_INITIATED_REFERRED_PARTY_NAME)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                 "\nreferred_party_name = %s",
          p_remote_call_transfer->referred_party_name.str));
    }
        
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
             "\npresence_flag = %d", 
              p_remote_call_transfer->bit_mask));
    
    return p_sprintf_count;
}
     
icf_uint8_t* icf_cmn_trace_rem_call_transfer_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_uint8_t presence_flag = 0;    
    icf_cc_pa_remote_call_transfer_initiated_ind_st* 
    p_remote_call_transfer = NULL;

    p_remote_call_transfer = 
    (icf_cc_pa_remote_call_transfer_initiated_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_remote_call_transfer)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_remote_call_transfer is NULL.\n"))
        return p_sprintf_count;
    }      

    presence_flag = (icf_uint8_t)p_remote_call_transfer->bit_mask;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nnew_call_id  = %d",
              p_remote_call_transfer->new_call_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
              "\nremote_call_transfer = (addr_type = %d, %s)",
              p_remote_call_transfer->referred_party_addr.addr_type ,
              p_remote_call_transfer->referred_party_addr.addr_val.str));

    if(presence_flag & ICF_CC_PA_REFER_INITIATED_REFERRED_PARTY_NAME)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                 "\nreferred_party_name = %s",
          p_remote_call_transfer->referred_party_name.str));
    }

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
             "\npresence_flag = %d",
              p_remote_call_transfer->bit_mask));

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_cta_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_pa_cc_call_transfer_attended_req_st* 
    p_call_transfer_attended_req = NULL;
     
    p_call_transfer_attended_req = 
        (icf_pa_cc_call_transfer_attended_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_call_transfer_attended_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_call_transfer_attended_req is NULL.\n"))
        return p_sprintf_count;
    }        
     
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
            
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ntransferred_party_id = %d",
              p_call_transfer_attended_req->transferred_party_id));
 
    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_ctu_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
/*

api_id=,source_module_id=,dest_module_id=,call_id=
{
transferred_party_address=(addr_type=,%s),presence_flag=, 
toggle_presentation_ind=
}

*/

        icf_uint32_t presence_flag = 0;
     
    icf_pa_cc_call_transfer_unattended_req_st* 
    p_call_transfer_unattended = NULL;
     
    p_call_transfer_unattended = 
         (icf_pa_cc_call_transfer_unattended_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_call_transfer_unattended)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_call_transfer_unattended is NULL.\n"))
        return p_sprintf_count;
    }            
    
    presence_flag = p_call_transfer_unattended->bit_mask;
         
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
        
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"presence_flag = %d",
             p_call_transfer_unattended->bit_mask));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ntransferred_party_address = (addr_type = %d, %s),",
              p_call_transfer_unattended->transferred_party_addr.addr_type,
              p_call_transfer_unattended->transferred_party_addr.addr_val.str));
    

    if (presence_flag & ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\ntoggle_presentation_indicator = %d ",
              p_call_transfer_unattended->toggle_presentation_indicator));
    }

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_conference_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
/*
api_id=,source_module_id=,dest_module_id=,call_id=
{
merge_call_id=
}
*/
    icf_pa_cc_conference_req_st* p_conference_req = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
        
    p_conference_req = (icf_pa_cc_conference_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_conference_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_conference_req is NULL.\n"))
        return p_sprintf_count;
    }      
      
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmerge_call_id = %d", 
              p_conference_req->merge_call_id));
     
    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_call_redirection_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
/*
api_id=,source_module_id=,dest_module_id=,call_id=
{
presence_flag=,redirected_party_address=(addr_type=,%s),
redirected_party_name=%s
}
*/
       icf_cc_pa_call_redirection_ind_st* p_call_redir = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    
    p_call_redir = (icf_cc_pa_call_redirection_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_call_redir)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_call_redir is NULL.\n"))
        return p_sprintf_count;
    }      
    
    if (ICF_CALL_RDR_IND_PTY_ADDR & p_call_redir->bit_mask)
        p_sprintf_count = icf_cmn_trace_icf_address_st(&(p_call_redir->redirected_party_addr),p_sprintf_count);

    if (ICF_CALL_RDR_IND_PTY_NAME & p_call_redir->bit_mask)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nself_name = %s)",
        p_call_redir->redirected_party_name.str));

    if (ICF_HDR_LIST_PRESENT & p_call_redir->bit_mask)
                p_sprintf_count = icf_cmn_trace_header_list(p_call_redir->header_list.hdr, p_call_redir->header_list.count,p_sprintf_count);

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_early_inc_call_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   icf_early_incoming_call_ind_st* p_early_call_ind = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_early_call_ind = (icf_early_incoming_call_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_early_call_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_early_call_ind is NULL.\n"))
        return p_sprintf_count;
    }          

    if (ICF_EARLY_CALL_CLG_PTY_ADDR & p_early_call_ind->bit_mask)
        p_sprintf_count = icf_cmn_trace_icf_address_st(&(p_early_call_ind->calling_party_addr),p_sprintf_count);

    if (ICF_EARLY_CALL_CLG_PTY_NAME & p_early_call_ind->bit_mask)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
				"\nremote_party_name = %s)",
        p_early_call_ind->calling_party_name.str));

    if (ICF_EARLY_CALL_CALLED_PTY_ADDR & p_early_call_ind->bit_mask)
        p_sprintf_count = icf_cmn_trace_icf_address_st(&(p_early_call_ind->called_party_addr),p_sprintf_count);	

    if (ICF_HDR_LIST_PRESENT & p_early_call_ind->bit_mask)
                p_sprintf_count = icf_cmn_trace_header_list(p_early_call_ind->header_list.hdr, p_early_call_ind->header_list.count,p_sprintf_count);

    return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_icf_address_st(icf_address_st *p_uri, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_uri)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_uri is NULL.\n"))
        return p_sprintf_count;
    }    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
        "\naddr_type = %d, addr_val = %s", 
        p_uri->addr_type,p_uri->addr_val.str));
    return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_pub_uri_list(icf_pub_uri_list_node_st *p_list, icf_uint8_t* p_sprintf_count)
{
    icf_uint8_t i,j;
    icf_address_list_node_st *p_assoc_uri_list = ICF_NULL;
    
    for(i=1; ICF_NULL != p_list;p_list=p_list->p_next,i++)
    {
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
					"\nPublic Uri %d",i));
        p_sprintf_count = icf_cmn_trace_icf_address_st(&(p_list->pub_uri),p_sprintf_count);
        /*SPR 19785 To avoid crashes while communicating Assoc URI in trace
          we shall be communicating at max ten nodes because there can be
          other lists as well header/contact to be communicated in trace */
        for(j=1,p_assoc_uri_list = p_list->p_assoc_uri_list; 
        (ICF_NULL != p_assoc_uri_list) && (j < ICF_CMN_MAX_TRACE_LIST_NODES);
        p_assoc_uri_list = p_assoc_uri_list->p_next,j++)
        {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
                "\n\tAssociated Uri %d", j));
            p_sprintf_count = icf_cmn_trace_icf_address_st(&(p_assoc_uri_list->user_addr),p_sprintf_count);
        }
   }

return p_sprintf_count;
}


/* MM API's */

icf_uint8_t* icf_cmn_trace_max_mic_gain_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_cfg_max_mic_gain_req_st* p_req = (icf_cfg_max_mic_gain_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }     
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nmax_mic_gain = %d", p_req->max_mic_gain));

/*    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask));*/

   
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_max_mic_gain_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_mic_gain_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_set_mic_gain_req_st* p_req = (icf_set_mic_gain_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }        
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\ngain_level = %d", p_req->gain_level));

/*    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nbit_mask = %d", p_req->bit_mask));*/

   
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_mic_gain_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    	p_sprintf_count = icf_cmn_trace_mm_msg_resp(p_msg,p_sprintf_count);
	return p_sprintf_count;
}
#ifdef ICF_STAT_ENABLED
icf_uint8_t* icf_cmn_trace_get_icf_stats_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_get_icf_stats_req_st* p_req = (icf_get_icf_stats_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }        
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nstats_id = %d", p_req->stats_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nreset = %d", p_req->reset));

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_get_icf_stats_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_uint8_t        i=0;
    icf_stats_resp_st* p_req = (icf_stats_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nresult = %d", p_req->result));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nstats_id = %d", p_req->stats_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nerror_cause = %d", p_req->error_cause));
   p_sprintf_count =  icf_cmn_trace_error_details(p_req->error_cause,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nnum_of_stats_elements = %d", p_req->num_of_stats_elements));

    for (i=0; i < p_req->num_of_stats_elements; i++)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
		    "\nstats_data[%d] = %d", i, p_req->stats_data[i]));

   
    return p_sprintf_count;
}
#endif

icf_uint8_t* icf_cmn_trace_info_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_info_req_st* p_info_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_info_req = (icf_info_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_info_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_info_req is NULL.\n"))
        return p_sprintf_count;
    }        
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_info_req->bit_mask));

/* Header LIST  */    if (p_info_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_info_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_info_req->header_list.hdr, header_list_count,p_sprintf_count);
    }

    if (p_info_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_info_req->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_info_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }

/* Body LIST  */
    if (p_info_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_info_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_info_req->body_list.body, body_list_count,p_sprintf_count);
    }

    
    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_chd_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_call_hold_req_st* p_chd_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_chd_req = (icf_call_hold_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_chd_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_chd_req is NULL.\n"))
        return p_sprintf_count;
    }        
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_chd_req->bit_mask));

/* Header LIST  */    if (p_chd_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_chd_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_chd_req->header_list.hdr, header_list_count,p_sprintf_count);
    }

    if (p_chd_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_chd_req->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_chd_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }

/* Body LIST  */
    if (p_chd_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_chd_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_chd_req->body_list.body, body_list_count,p_sprintf_count);
    }


	if(p_chd_req->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream_list count = %d",p_chd_req->stream_list.count));
	
		for(i=0;(i<p_chd_req->stream_list.count) && (i< ICF_MAX_STREAM_PER_CALL) ;i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream_list id = %d", 
				  p_chd_req->stream_list.stream_id[i]));
		}
	}



   
    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_crm_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_call_resume_req_st* p_crm_req = NULL;
    
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_crm_req = (icf_call_resume_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_crm_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_crm_req is NULL.\n"))
        return p_sprintf_count;
    }        
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d",p_crm_req->bit_mask)); 
             

/* Header LIST  */    if (p_crm_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_crm_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_crm_req->header_list.hdr, header_list_count,p_sprintf_count);
    }

    if (p_crm_req->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_crm_req->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_crm_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }

/* Body LIST  */
    if (p_crm_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_crm_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_crm_req->body_list.body, body_list_count,p_sprintf_count);
    }



	if(p_crm_req->bit_mask & ICF_STREAM_LIST_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream_list count = %d",p_crm_req->stream_list.count));
	
		for(i=0;(i<p_crm_req->stream_list.count) && (i < ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream_list id = %d", 
				  p_crm_req->stream_list.stream_id[i]));
		}
	}



    
    return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_ctu_resp()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_ctu_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_ctu_resp_st* p_ctu_resp = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_ctu_resp = (icf_ctu_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_ctu_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_ctu_resp is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nresult = %d, error_cause = %d",
              p_ctu_resp->result, p_ctu_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_ctu_resp->error_cause,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n{\nbit_mask = %d", 
              p_ctu_resp->bit_mask));

/* Header LIST  */    if (p_ctu_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_header_list(p_ctu_resp->header_list.hdr, p_ctu_resp->header_list.count,p_sprintf_count);
    }    

    if( ICF_RETRY_AFTER_DURATION_PRESENT & p_ctu_resp->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nREtry After Duartion = %d",
	p_ctu_resp->retry_after_duration)); 
    }
    
    return p_sprintf_count;
}


/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_cta_resp()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_cta_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_cta_resp_st* p_cta_resp = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_cta_resp = (icf_cta_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_cta_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_cta_resp is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nresult = %d, error_cause = %d",
              p_cta_resp->result, p_cta_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_cta_resp->error_cause,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n{\nbit_mask = %d", 
              p_cta_resp->bit_mask));

/* Header LIST  */    if (p_cta_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_header_list(p_cta_resp->header_list.hdr, p_cta_resp->header_list.count,p_sprintf_count);
    }    

    if( ICF_RETRY_AFTER_DURATION_PRESENT & p_cta_resp->bit_mask)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nRetry After Duartion = %d",
	p_cta_resp->retry_after_duration)); 
    }
    
    return p_sprintf_count;
}


/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_api_ind()
 *
 * DESCRIPTION:     Print response messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_api_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_api_ind_st* p_resp_msg = NULL;

	icf_uint8_t		i = 0;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_resp_msg = (icf_api_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_resp_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_resp_msg is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_resp_msg->bit_mask));

/* Header LIST  */    if (p_resp_msg->bit_mask & ICF_HDR_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_header_list(p_resp_msg->header_list.hdr, p_resp_msg->header_list.count,p_sprintf_count);
    }    

/* Body LIST  */
    if (p_resp_msg->bit_mask & ICF_BODY_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_body_list(p_resp_msg->body_list.body, p_resp_msg->body_list.count,p_sprintf_count);
    }


	if(p_resp_msg->bit_mask & ICF_STREAM_LIST_PRESENT)/* for deleted stream lists */
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Deleted stream count %d:",p_resp_msg->stream_list.count));	

		for(i=0;(i<p_resp_msg->stream_list.count) && (i < ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id %d:",p_resp_msg->stream_list.stream_id[i]));
		}
	}



    
    return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_rtp_start_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_rtp_start_ind_st* p_rtp_start_ind = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_rtp_start_ind = (icf_rtp_start_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_rtp_start_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_rtp_start_ind is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nstream_id = %d, ssrc = %L",
              p_rtp_start_ind->stream_id, p_rtp_start_ind->ssrc));

    return p_sprintf_count;
}

#if 0
icf_uint8_t* icf_cmn_trace_unexpected_msg_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        icf_unexpected_msg_ind_st                  *p_unexpected_msg = ICF_NULL;

        p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

        p_unexpected_msg = (icf_unexpected_msg_ind_st*)p_msg->payload;
        
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nError Cause = %d",
              p_unexpected_msg->error_cause));

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApi_id = %d",
              p_unexpected_msg->api_id));

        return p_sprintf_count;
}
#endif



#ifdef ICF_FAX_SUPPORT

icf_uint8_t* icf_cmn_trace_start_fax_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_start_fax_req_st* p_start_fax_req = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_start_fax_req = (icf_start_fax_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_start_fax_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_start_fax_req is NULL.\n"))
        return p_sprintf_count;
    }       
   
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFax Type = %d", 
              p_start_fax_req->fax_type));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nPref Fax Codec = %d", 
              p_start_fax_req->fax_pt_codec));

	if( ICF_START_FAX_REQ_METHOD_PRESENT & p_start_fax_req->bitmask)
	{
	   p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFax_method = %d",
              p_start_fax_req->fax_method));
	}		
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_start_fax_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_start_fax_resp_st* p_start_fax_resp = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_start_fax_resp = (icf_start_fax_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_start_fax_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_start_fax_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nResult = %d", 
              p_start_fax_resp->result));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nError Cause = %d", 
              p_start_fax_resp->error_cause));
        p_sprintf_count = icf_cmn_trace_error_details(p_start_fax_resp->error_cause,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFax Type = %d", 
              p_start_fax_resp->fax_type));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit mask = %d",
              p_start_fax_resp->bit_mask));
	if( ICF_RETRY_AFTER_DURATION_PRESENT & p_start_fax_resp->bit_mask)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nRetry After Duration = %d",
              p_start_fax_resp->retry_after_duration));
	}


	return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_start_fax_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_start_fax_ind_st* p_start_fax_ind = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_start_fax_ind = (icf_start_fax_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_start_fax_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_start_fax_ind is NULL.\n"))
        return p_sprintf_count;
    }      
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFax Type = %d", 
              p_start_fax_ind->fax_type));

       return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_stop_fax_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_stop_fax_resp_st* p_stop_fax_resp = NULL;
   
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_stop_fax_resp = (icf_stop_fax_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stop_fax_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stop_fax_resp is NULL.\n"))
        return p_sprintf_count;
    }  
   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nResult = %d",
           p_stop_fax_resp->result));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nError Cause = %d",
              p_stop_fax_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_stop_fax_resp->error_cause,p_sprintf_count);

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_stop_fax_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_stop_fax_ind_st* p_stop_fax_ind = NULL;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_stop_fax_ind = (icf_stop_fax_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stop_fax_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stop_fax_ind is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFax Type = %d",
              p_stop_fax_ind->fax_type));

    return p_sprintf_count;

}

#endif



icf_uint8_t* icf_cmn_trace_remote_video_diaplay_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_cfg_set_remote_video_display_st* p_remote_video_display_req = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_remote_video_display_req = (icf_cfg_set_remote_video_display_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_remote_video_display_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_remote_video_display_req is NULL.\n"))
        return p_sprintf_count;
    }      
    
	/*p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nWindow Handle = %d", 
              p_remote_video_display_req->window_handle)));*/

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nTOP = %d", 
              p_remote_video_display_req->top));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nLEFT = %d", 
              p_remote_video_display_req->left));
	

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nWIDTH = %d", 
              p_remote_video_display_req->width));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nHEIGHT = %d", 
              p_remote_video_display_req->height));

    
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_preview_video_diaplay_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_cfg_set_preview_video_display_st* p_preview_video_display_req = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_preview_video_display_req = (icf_cfg_set_preview_video_display_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_preview_video_display_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_preview_video_display_req is NULL.\n"))
        return p_sprintf_count;
    }      

	/*p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nWindow Handle = %d", 
              p_preview_video_display_req->window_handle));*/

    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nTOP = %d", 
              p_preview_video_display_req->top));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nLEFT = %d", 
              p_preview_video_display_req->left));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nWIDTH = %d", 
              p_preview_video_display_req->width));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nHEIGHT = %d", 
              p_preview_video_display_req->height));

   
    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_connect_media_device_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_connect_media_device_req_st* p_con_media_dev_req = NULL;

    p_con_media_dev_req = (icf_connect_media_device_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_con_media_dev_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_con_media_dev_req is NULL.\n"))
        return p_sprintf_count;
    }     

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_con_media_dev_req->bit_mask));

    if (p_con_media_dev_req->bit_mask & ICF_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_con_media_dev_req->line_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media Device = %d:",p_con_media_dev_req->media_device));
	
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Connect Status = %d:",p_con_media_dev_req->connect_status));

   
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_mms_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i = 1;


    icf_modify_media_session_resp_st* p_mms_resp = ICF_NULL;


    icf_list_st *p_list=ICF_NULL;    
 
    p_mms_resp = (icf_modify_media_session_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_mms_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_mms_resp is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbitmask = %d, result = %d, error_cause = %d",
        p_mms_resp->bit_mask, p_mms_resp->result, p_mms_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_mms_resp->error_cause,p_sprintf_count);

    p_list=p_mms_resp->p_accepted_stream_list;
	while(p_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_resp_stream((icf_resp_stream_st*)(p_list->p_data),i,p_sprintf_count);
		
		p_list = p_list->p_next;
		
		i++;
	}

	if(p_mms_resp->bit_mask & ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nDeleted media count = %d", p_mms_resp->rejected_stream_list.count));

		for(i=0;(i<p_mms_resp->rejected_stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL) ;i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nStream ID = %d", p_mms_resp->rejected_stream_list.stream_id[i]));
		}
	}

    return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_codec_commit_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_uint8_t i = 1, j=0,k=0;

    icf_list_st *p_list= ICF_NULL;
    icf_codec_commit_resp_st* p_ccr_resp = ICF_NULL;

    p_ccr_resp = (icf_codec_commit_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_ccr_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_ccr_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbitmask = %d, result = %d, error_cause = %d",
        p_ccr_resp->bit_mask, p_ccr_resp->result, p_ccr_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_ccr_resp->error_cause,p_sprintf_count);

    p_list=p_ccr_resp->p_commited_stream_list;

	while(p_list!=ICF_NULL)
	{
        /*Fix For SPR : 18999*/
        if (ICF_NULL == p_list->p_data)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_list->p_data is NULL.\n"))
            return p_sprintf_count;
        } 
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nStream ID = %d",
				((icf_stream_commit_st*)(p_list->p_data))->stream_id));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nCodec Count = %d",
				((icf_stream_commit_st*)(p_list->p_data))->codec_count));

		for(j=0;(j<((icf_stream_commit_st*)(p_list->p_data))->codec_count) && (j<ICF_MAX_CODECS);j++)
		{
			if(((icf_stream_commit_st*)(p_list->p_data))->codec[j].bit_mask &
				ICF_CODEC_ID_CODEC_ID_PRESENT)
			{
				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nCodec ID = %d", 
					((icf_stream_commit_st*)(p_list->p_data))->codec[j].codec_id));
			}

			if(((icf_stream_commit_st*)(p_list->p_data))->codec[j].bit_mask &
				ICF_CODEC_ID_CODEC_NAME_PRESENT)
			{

				k = 0;

				p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nCodec Name = "));

				while((k<((icf_stream_commit_st*)(p_list->p_data))->codec[j].codec_name.str_len)&& (k<ICF_MAX_SHORT_STR_LEN))
				{
					p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",
						((icf_stream_commit_st*)(p_list->p_data))->codec[j].codec_name.str[k]));

					k++;
				}
			}
			
		}
		
		p_list = p_list->p_next;
		
		i++;
	}

	if(p_ccr_resp->bit_mask & ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \n Rejected Stream List Count = %d", p_ccr_resp->rejected_stream_list.count));

		for(i=0;(i<p_ccr_resp->rejected_stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nStream ID = %d", p_ccr_resp->rejected_stream_list.stream_id[i]));
		}
	}

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_mute_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	icf_mute_media_resp_st* p_media_mute_resp = NULL;

    p_media_mute_resp = (icf_mute_media_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_media_mute_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_media_mute_resp is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Result = %d:",p_media_mute_resp->result));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error Cause = %d:",p_media_mute_resp->error_cause));
	p_sprintf_count = icf_cmn_trace_error_details(p_media_mute_resp->error_cause,p_sprintf_count);

    if (p_media_mute_resp->bit_mask & ICF_MUTE_MEDIA_RESP_LINE_ID_PRESENT)
    {
       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line_id = %d:",p_media_mute_resp->line_id)); 
    }
    
    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_cfg_media_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
	icf_uint8_t		i = 0;

	icf_cfg_media_req_st* p_cfg_media_req = NULL;

    p_cfg_media_req = (icf_cfg_media_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_cfg_media_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_cfg_media_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

  
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Count = %d:",p_cfg_media_req->count));

	for(i=0;(i<p_cfg_media_req->count) && (i < ICF_MAX_MEDIA);i++)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media ID = %d:",p_cfg_media_req->media[i].media_id));

		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Play File = %s:",p_cfg_media_req->media[i].play_file.str));
	}

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_play_media_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
	icf_uint8_t		i = 0;
	
	icf_play_media_req_st* p_play_media_req = NULL;

    p_play_media_req = (icf_play_media_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_play_media_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_play_media_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    if (p_play_media_req->is_present & ICF_PLAY_MEDIA_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_play_media_req->line_id));

	if(p_play_media_req->is_present & ICF_MEDIA_ID_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media ID = %d:",p_play_media_req->media_id));

	if(p_play_media_req->is_present & ICF_FILE_NAME_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n File Name = ",p_play_media_req->file_name.str[i]));
		
		for(i=0;i<p_play_media_req->file_name.str_len;i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",p_play_media_req->file_name.str[i]));
		}
	}

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Play Mode = %d:",p_play_media_req->play_mode));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Duration = %d:",p_play_media_req->duration));

	if(p_play_media_req->is_present & ICF_PLAY_TONE_TIMER_VAL)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Count = %d:",p_play_media_req->timer_value));

   	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_stop_media_play_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
	icf_uint8_t		i = 0;
	
	icf_stop_media_play_req_st* p_stop_media_play_req = NULL;

    p_stop_media_play_req = (icf_stop_media_play_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stop_media_play_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stop_media_play_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

  
    if (p_stop_media_play_req->is_present & ICF_STOP_MEDIA_PLAY_REQ_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_stop_media_play_req->line_id));

	if(p_stop_media_play_req->is_present & ICF_MEDIA_ID_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media ID = %d:",p_stop_media_play_req->media_id));

	if(p_stop_media_play_req->is_present & ICF_FILE_NAME_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n File Name = ",p_stop_media_play_req->file_name.str[i]));
		
		for(i=0;i<p_stop_media_play_req->file_name.str_len;i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",p_stop_media_play_req->file_name.str[i]));
		}
	}

	
	return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_stop_media_play_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
	icf_uint8_t		i = 0;
	
	icf_stop_media_play_ind_st* p_stop_media_play_ind = NULL;

    p_stop_media_play_ind = (icf_stop_media_play_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stop_media_play_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stop_media_play_ind is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    if (p_stop_media_play_ind->is_present & ICF_STOP_MEDIA_PLAY_IND_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_stop_media_play_ind->line_id));

	if(p_stop_media_play_ind->is_present & ICF_MEDIA_ID_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media ID = %d:",p_stop_media_play_ind->media_id));

	if(p_stop_media_play_ind->is_present & ICF_FILE_NAME_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n File Name = ",p_stop_media_play_ind->file_name.str[i]));
		
		for(i=0;i<p_stop_media_play_ind->file_name.str_len;i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",p_stop_media_play_ind->file_name.str[i]));
		}
	}

	
	return p_sprintf_count;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_media_record_req
 *
 * DESCRIPTION:     This function is used to print media_record_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_media_record_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i = 0;

    icf_media_record_req_st* p_media_record_req = ICF_NULL;

    p_media_record_req = (icf_media_record_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_media_record_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_media_record_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

  
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_media_record_req->bit_mask));

    if (p_media_record_req->bit_mask & ICF_MEDIA_RECORD_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_media_record_req->line_id));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nMedia Source:%d",
									p_media_record_req->media_source));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFile Type:%d",
									p_media_record_req->file_type));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nFile name: "));

	for(i=0;i<p_media_record_req->file_name.str_len;i++)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%c",p_media_record_req->file_name.str[i]));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nBit mask:%d",
									p_media_record_req->bit_mask));

	if(p_media_record_req->bit_mask & ICF_STREAM_ID_PRESENT){
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream id:%d",
									p_media_record_req->stream_id));
	}


	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_stop_media_record_req
 *
 * DESCRIPTION:     This function is used to print stop_media_record_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_stop_media_record_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_stop_media_record_req_st* p_stop_media_record_req = ICF_NULL;

    p_stop_media_record_req = (icf_stop_media_record_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stop_media_record_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stop_media_record_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_stop_media_record_req->bit_mask));

    if (p_stop_media_record_req->bit_mask & ICF_STOP_MEDIA_RECORD_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_stop_media_record_req->line_id));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nMedia Source:%d",
									p_stop_media_record_req->media_source));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nBit mask:%d",
									p_stop_media_record_req->bit_mask));

	if(p_stop_media_record_req->bit_mask & ICF_STREAM_ID_PRESENT){
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream id:%d",
									p_stop_media_record_req->stream_id));
	}


	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_pause_media_req
 *
 * DESCRIPTION:     This function is used to print pause_media_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_pause_media_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i = 0;

    icf_pause_media_req_st* p_pause_media_req = ICF_NULL;

    p_pause_media_req = (icf_pause_media_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_pause_media_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_pause_media_req is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

  
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_pause_media_req->bit_mask));

    if (p_pause_media_req->bit_mask & ICF_LINE_ID_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_pause_media_req->line_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of Stream ids"));

	for(i=0;(i<p_pause_media_req->stream_id.count) && (i < ICF_MAX_STREAM_PER_CALL);i++)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id %d: %d",
									i+1,p_pause_media_req->stream_id.stream_id[i]));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nActivate Pause:%d",
									p_pause_media_req->activate_pause));

	
	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_set_media_playback_device_req
 *
 * DESCRIPTION:     This function is used to print set_media_playback_device_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_set_media_playback_device_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
     
    icf_set_media_playback_device_req_st* p_set_media_req = ICF_NULL;

    p_set_media_req = (icf_set_media_playback_device_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_set_media_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_set_media_req is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

  
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_set_media_req->bit_mask)); 

    if (p_set_media_req->bit_mask & ICF_LINE_ID_PRESENT) 
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_set_media_req->line_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nMedia playout device:%d",
									p_set_media_req->media_play_out_device));


	return p_sprintf_count;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_mute_media_req
 *
 * DESCRIPTION:     This function is used to print mute_media_req
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_mute_media_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i = 0;

    icf_mute_media_req_st* p_mute_media_req = ICF_NULL;

    p_mute_media_req = (icf_mute_media_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_mute_media_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_mute_media_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_mute_media_req->bit_mask));

    if (p_mute_media_req->bit_mask & ICF_LINE_ID_PRESENT)
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_mute_media_req->line_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nList of Stream ids"));

	for(i=0;(i<p_mute_media_req->stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id %d: %d",
									i+1,p_mute_media_req->stream_list.stream_id[i]));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nActivate Mute:%d",
									p_mute_media_req->activate_mute));
	return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_dgts_ind
 *
 * DESCRIPTION:     This function is used to print dgts_ind
 *
 *****************************************************************************/

icf_uint8_t* icf_cmn_trace_dgts_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
   
    icf_uint8_t i = 0;

    icf_dgts_ind_st* p_icf_dgts = ICF_NULL;

    p_icf_dgts = (icf_dgts_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_icf_dgts)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_icf_dgts is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d:",p_icf_dgts->bit_mask));

    if (p_icf_dgts->bit_mask & ICF_LINE_ID_PRESENT)
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Line id = %d:",p_icf_dgts->line_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream Id: %d",
									p_icf_dgts->stream_id));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nDigits:"));

	for(i=0;(i<p_icf_dgts->digits.number_of_dgts) && (i<ICF_MAX_DGT_LEN);i++)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nDigit %d: %d",
									i+1, p_icf_dgts->digits.dgts[i]));
	return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_vibration_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        
    icf_vibration_req_st* p_vibration_req = ICF_NULL;

    p_vibration_req = (icf_vibration_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_vibration_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_vibration_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nVibration Id: %d",p_vibration_req->vibration_id));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nDuration: %d",p_vibration_req->duration));

	if(p_vibration_req->bit_mask & ICF_VIBRATION_TIMER_VAL)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nTimer Value: %d",p_vibration_req->timer_value));


	return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_echo_canc_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
        
    icf_echo_canc_req_st* p_echo_canc_req = ICF_NULL;

    p_echo_canc_req = (icf_echo_canc_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_echo_canc_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_echo_canc_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nEcho Cancel: %d",p_echo_canc_req->echo_cancel));

	if(p_echo_canc_req->bit_mask & ICF_ECHO_CANC_STREAM_ID)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream ID: %d",p_echo_canc_req->stream_id));


	return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_vad_ss_cng_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
       
    icf_vad_ss_cng_req_st* p_vad_ss_cng_req = ICF_NULL;

    p_vad_ss_cng_req = (icf_vad_ss_cng_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_vad_ss_cng_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_vad_ss_cng_req is NULL.\n"))
        return p_sprintf_count;
    }     

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    
	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_STREAM_ID)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nStream ID: %d",p_vad_ss_cng_req->stream_id));

	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_SUPP)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nVad Supp: %d",p_vad_ss_cng_req->vad_supp));


	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_SILENCE_SUPP)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSilence Supp: %d",p_vad_ss_cng_req->silence_supp));


	if(p_vad_ss_cng_req->bit_mask & ICF_VAD_COMFORT_NOISE)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nComfort Noise: %d",p_vad_ss_cng_req->comfort_noise));

	
	return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_stop_vibration_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    return p_sprintf_count;
}
/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_send_dgts_req
 *
 * DESCRIPTION:     This function is used to print send_dgts_req
 *
 *****************************************************************************/
icf_uint8_t* icf_cmn_trace_send_dgts_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_send_dgts_st* p_send_dgts = (icf_send_dgts_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_send_dgts)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_send_dgts is NULL.\n"))
        return p_sprintf_count;
    }       
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    	
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nStream Id = %d", p_send_dgts->stream_id));

	if(p_send_dgts->bit_mask & ICF_SEND_DGT_TRANSPORT_METHOD_PRESENT)
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
				"\nTransport Method = %d", p_send_dgts->transport_method));
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nDigits = %d", p_send_dgts->digits));

    
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_med_cap_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i = 1;

    
    icf_media_capabilities_resp_st* p_med_cap_resp = ICF_NULL;
    icf_list_st *p_stream_list = ICF_NULL;

    p_med_cap_resp = (icf_media_capabilities_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_med_cap_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_med_cap_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_cause = %d",
        p_med_cap_resp->result, p_med_cap_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_med_cap_resp->error_cause,p_sprintf_count);

   p_stream_list = p_med_cap_resp->p_resp_stream_list;

	while(p_stream_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_resp_stream((icf_resp_stream_st*)(p_stream_list->p_data),i,p_sprintf_count);
		
		p_stream_list = p_stream_list->p_next;
		
		i++;
	}

	return p_sprintf_count;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_call_fwd_ind
 *
 * DESCRIPTION:     This function is used to print call_forwarded_ind
 *
 *****************************************************************************/
icf_uint8_t* icf_cmn_trace_call_fwd_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_call_forwarded_ind_st* p_call_fwd = (icf_call_forwarded_ind_st*)p_msg->payload;

    icf_uint32_t i = 0;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_call_fwd)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_call_fwd is NULL.\n"))
        return p_sprintf_count;
    }       
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

   	
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
			"\nRemote party address ="));

	p_sprintf_count = icf_cmn_trace_icf_address_st(&p_call_fwd->remote_party_addr,p_sprintf_count);

	if(p_call_fwd->bit_mask & ICF_REMOTE_PARTY_NAME)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,\
				"\nRemote party name = "));

		for(i=0;(i<p_call_fwd->remote_party_name.str_len) && (i<ICF_MAX_STR_LEN);i++)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              			"%c",p_call_fwd->remote_party_name.str[i]));
		}
	}

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nCall forwarded type = %d", p_call_fwd->call_forwarded_type));

				
  
    return p_sprintf_count;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_call_dms_req
 *
 * DESCRIPTION:     This function is used to print delete_media_session_req *
 *****************************************************************************/
icf_uint8_t* icf_cmn_trace_dms_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_uint8_t i;

	icf_delete_media_session_req_st* p_dms_req = (icf_delete_media_session_req_st*)p_msg->payload;
    
#ifdef ICF_FOR_LOAD_TESTING
	icf_uint8_t command_string[ICF_MAX_BUFF_LEN];
#endif
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_dms_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_dms_req is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

  	
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nDeleted media count = %d", p_dms_req->stream_list.count));

	for(i=0;(i<p_dms_req->stream_list.count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count," \nStream ID = %d", p_dms_req->stream_list.stream_id[i]));
	}

   #ifdef ICF_FOR_LOAD_TESTING

	app_global_data.call_id = p_msg->hdr.call_id;
    strcpy(command_string,ICF_DEFAULT_DMS_RSP_MSG);
    icf_command_handler(command_string);

#endif
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_notify_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_notify_ind_st* p_notfy_ind = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0;
    
    p_notfy_ind = (icf_notify_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_notfy_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_notfy_ind is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_notfy_ind->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nline_id = %d", p_notfy_ind->line_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nevent_pkg_name = %s", p_notfy_ind->event_pkg_name.str));
	
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nsubs_state = %d", p_notfy_ind->subs_state));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nreason_code = %d", p_notfy_ind->reason_code));

	if (p_notfy_ind->bit_mask & ICF_SUBS_DURATION_PRESENT) {
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nduration = %d", p_notfy_ind->duration));
	}

	if (p_notfy_ind->bit_mask & ICF_NOTIFY_IND_SENDER_PRESENT) {
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nsender = (addr_type = %d, %s),",
              p_notfy_ind->sender.addr_type,
              p_notfy_ind->sender.addr_val.str));
	}

	if (p_notfy_ind->bit_mask & ICF_NOTIFY_IND_USER_ADDR_PRESENT) {
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
              "\nuser_address = (addr_type = %d, %s),",
              p_notfy_ind->user_address.addr_type,
              p_notfy_ind->user_address.addr_val.str));
	}

	if (p_notfy_ind->bit_mask & ICF_NOTIFY_IND_OUT_OF_DIALOG){
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nOut of Dialog Message = TRUE"));
	}

	/* Header LIST  */    if (p_notfy_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_notfy_ind->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_notfy_ind->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Body LIST  */
    if (p_notfy_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_notfy_ind->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_notfy_ind->body_list.body, body_list_count,p_sprintf_count);
    }
    
    return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_notify_cfm()
 *
 * DESCRIPTION:     Print notify cfm messages 
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_notify_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_notify_cfm_st* p_notfy_cfm = ICF_NULL;
    icf_uint8_t  header_list_count = 0, body_list_count = 0,
                 tag_list_count = 0;
   
    p_notfy_cfm = (icf_notify_cfm_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_notfy_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_notfy_cfm is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_notfy_cfm->bit_mask));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nresult = %d", p_notfy_cfm->result));


	if (p_notfy_cfm->bit_mask & ICF_NOTIFY_ERROR_CAUSE_PRESENT) {
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nerror_code = %d", p_notfy_cfm->error_code));
	}

	if (p_notfy_cfm->bit_mask & ICF_NOTIFY_RESP_CODE_PRESENT) {
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
			"\nresponse_code = %d", p_notfy_cfm->response_code));
	}

  	/* Header List */
    if (p_notfy_cfm->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        header_list_count = (icf_uint32_t)p_notfy_cfm->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_notfy_cfm->header_list.hdr,
            header_list_count,p_sprintf_count);
    }

    /* Tag List */
    if (p_notfy_cfm->bit_mask & ICF_TAG_LIST_PRESENT) 
    {
        tag_list_count = (icf_uint32_t)p_notfy_cfm->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_notfy_cfm->tag_list.tag,
            tag_list_count,p_sprintf_count);
    }
    
    /* Body List */
    if (p_notfy_cfm->bit_mask & ICF_BODY_LIST_PRESENT) 
    {
        body_list_count = (icf_uint32_t)p_notfy_cfm->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_notfy_cfm->body_list.body,
            body_list_count,p_sprintf_count);
    }

    return p_sprintf_count;
}


icf_uint8_t* icf_cmn_trace_info_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_info_ind_st* p_info_ind = NULL;
    
    icf_uint8_t  header_list_count = 0, body_list_count = 0;


    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_info_ind = (icf_info_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_info_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_info_ind is NULL.\n"))
        return p_sprintf_count;
    }      
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_info_ind->bit_mask));

/* Header LIST  */    if (p_info_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_info_ind->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_info_ind->header_list.hdr, header_list_count,p_sprintf_count);
    }

/* Body LIST  */
    if (p_info_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_info_ind->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_info_ind->body_list.body, body_list_count,p_sprintf_count);
    }

    
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_info_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_info_resp_st* p_info_resp = NULL;
    
    icf_uint8_t  header_list_count = 0, body_list_count = 0;
                   

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_info_resp = (icf_info_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_info_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_info_resp is NULL.\n"))
        return p_sprintf_count;
    }      
   
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_info_resp->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d", 
              p_info_resp->result));

	if(ICF_FAILURE == p_info_resp->result)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nerror_cause = %d\n", 
				  p_info_resp->error_cause));
		p_sprintf_count = icf_cmn_trace_error_details(p_info_resp->error_cause,p_sprintf_count);
	
		/*
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"response_code = %d\n", 
				  p_info_resp->response_code));
		*/
	}
	else
	{

	/* Header LIST  */		if (p_info_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
			header_list_count = (icf_uint8_t)p_info_resp->header_list.count;
			p_sprintf_count = icf_cmn_trace_header_list(p_info_resp->header_list.hdr, header_list_count,p_sprintf_count);
		}

	/* Body LIST  */
		if (p_info_resp->bit_mask & ICF_BODY_LIST_PRESENT) {
			body_list_count =(icf_uint8_t) p_info_resp->body_list.count;
			p_sprintf_count = icf_cmn_trace_body_list(p_info_resp->body_list.body, body_list_count,p_sprintf_count);
		}
	}

    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_nw_status(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_nw_status_st* p_nw_status = NULL;
    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_nw_status = (icf_nw_status_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_nw_status)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_nw_status is NULL.\n"))
        return p_sprintf_count;
    }      
    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nnw_id = %d\n", 
				  p_nw_status->nw_id));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"nw_status = %d", 
				  p_nw_status->nw_status));

   
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_media_capabilities_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
	icf_media_capabilities_req_st *p_media_capab = ICF_NULL;

	icf_list_st	*p_req_stream_list = ICF_NULL;

	icf_uint8_t i =0;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_media_capab = (icf_media_capabilities_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_media_capab)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_media_capab is NULL.\n"))
        return p_sprintf_count;
    }     
	
	if ( ICF_MED_CAP_STREAM_PRSNT ==
		 (p_media_capab->bit_mask & ICF_MED_CAP_STREAM_PRSNT))
	{
		p_req_stream_list = p_media_capab->p_req_stream_list;
		
		while(p_req_stream_list!=ICF_NULL)
		{
			p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
			
			p_sprintf_count = icf_cmn_trace_stream_capab(p_req_stream_list->p_data,p_sprintf_count);
			
			p_req_stream_list = p_req_stream_list->p_next;
			
			i++;
		}
		
	}

	return p_sprintf_count;
}
icf_uint8_t* icf_cmn_trace_options_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_options_cfm_st* p_options_cfm = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	*p_app_sdp_list;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_options_cfm = (icf_options_cfm_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_options_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_options_cfm is NULL.\n"))
        return p_sprintf_count;
    }         
     
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_options_cfm->bit_mask));
     
/* Header LIST  */    if (p_options_cfm->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_cfm->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_options_cfm->header_list.hdr, header_list_count,p_sprintf_count);
    }

/* Tag LIST  */    if (p_options_cfm->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_options_cfm->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_options_cfm->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    /* Body LIST  */
    if (p_options_cfm->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_options_cfm->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_options_cfm->body_list.body, body_list_count,p_sprintf_count);
    }
    

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media Profile List:"));

	i = 1;

	p_app_sdp_list = p_options_cfm->p_app_sdp_list;

	while(p_app_sdp_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_app_sdp_list->p_data,p_sprintf_count);
		
		p_app_sdp_list = p_app_sdp_list->p_next;
		
		i++;
	}

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d", 
              p_options_cfm->result));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ndelete count = %d", 
              p_options_cfm->count));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d", 
              p_options_cfm->result));
	for (i = 0; (i < p_options_cfm->count) && (i<ICF_MAX_STREAM_PER_CALL) ; i++)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ndelete count = %d", 
			p_options_cfm->deleted_media_streams[i].stream_id));
		
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\ndelete count = %d", 
			p_options_cfm->deleted_media_streams[i].stream_type));
		
	}


    return p_sprintf_count;
}

icf_uint8_t* icf_cmn_trace_options_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_inc_options_ind_st* p_options_ind = NULL;

    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
	icf_list_st	*p_app_sdp_list;

    
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_options_ind = (icf_inc_options_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_options_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_options_ind is NULL.\n"))
        return p_sprintf_count;
    }       
     
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", 
              p_options_ind->bit_mask));
     
/* Header LIST  */    if (p_options_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_ind->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_options_ind->header_list.hdr, header_list_count,p_sprintf_count);
    }


/* Tag LIST  */    if (p_options_ind->bit_mask & ICF_TAG_LIST_PRESENT) {
        tag_list_count = (icf_uint8_t)p_options_ind->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_options_ind->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_options_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count =(icf_uint8_t) p_options_ind->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_options_ind->body_list.body, body_list_count,p_sprintf_count);
    }

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media Profile List:"));

	i = 1;

	p_app_sdp_list = p_options_ind->p_app_sdp_list;

	while(p_app_sdp_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

		p_sprintf_count = icf_cmn_trace_stream_capab(p_app_sdp_list->p_data,p_sprintf_count);
		
		p_app_sdp_list = p_app_sdp_list->p_next;
		
		i++;
	}

    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_options_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_options_req_st* p_options_req = ICF_NULL;

    icf_uint8_t  header_list_count = 0, body_list_count = 0, tag_list_count = 0;
        
    /* Fix for SPR 20412: CSR 1-8223807 merged*/
    icf_uint8_t  remove_hdr_list_count = 0;

    p_options_req = (icf_options_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_options_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_options_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nbit_mask = %d", p_options_req->bit_mask));


	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
		"\ndestination_info = (addr_type = %d, %s)",
		p_options_req->destination_info.addr_type,
		p_options_req->destination_info.addr_val.str));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\npref_identity.str = %s", 
        
        p_options_req->pref_identity.addr_val.str));
	
	/* Header LIST  */    if (p_options_req->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_options_req->header_list.hdr, header_list_count,p_sprintf_count);
    }
    
    /* Tag LIST  */    if (p_options_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		tag_list_count = (icf_uint8_t)p_options_req->tag_list.count;
		p_sprintf_count = icf_cmn_trace_tag_list(p_options_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }
    
    
    /* Body LIST  */
    if (p_options_req->bit_mask & ICF_BODY_LIST_PRESENT) {
        body_list_count = (icf_uint8_t)p_options_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_options_req->body_list.body, body_list_count,p_sprintf_count);
    }
    
    /* Fix for SPR 20412: CSR 1-8223807 merged*/ 
    /* headers need to be remove from sip message*/
    if (p_options_req->bit_mask & ICF_OPTIONS_REQ_REMOVE_HDR_LIST_PRESENT) {
         remove_hdr_list_count = (icf_uint8_t)p_options_req->remove_hdr_list.count;
         p_sprintf_count = icf_cmn_trace_header_name_list(p_options_req->remove_hdr_list.hdr, remove_hdr_list_count,p_sprintf_count);
    } 
    return p_sprintf_count;

}

icf_uint8_t* icf_cmn_trace_options_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_options_resp_st* p_options_resp = ICF_NULL;

    icf_uint8_t  header_list_count = 0;

	icf_uint8_t	 i = 1;
    icf_list_st *p_list = ICF_NULL;
    
    p_options_resp = (icf_options_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_options_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_options_resp is NULL.\n"))
        return p_sprintf_count;
    }      

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


    
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d", p_options_resp->bit_mask));


       
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nresult = %d, response_code = %d",p_options_resp->result, p_options_resp->response_code_for_options));

        

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nerror_code = %d",p_options_resp->error_code_for_options));
        
       


	/* Header LIST  */    if (p_options_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
        header_list_count = (icf_uint8_t)p_options_resp->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_options_resp->header_list.hdr, header_list_count,p_sprintf_count);
    }


    /* Body LIST  */
	if (p_options_resp->bit_mask & ICF_BODY_LIST_PRESENT) {

		p_sprintf_count = (icf_uint8_t *)icf_cmn_trace_body_list(p_options_resp->body_list.body,
			p_options_resp->body_list.count,p_sprintf_count);
	}

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Remote Stream List:"));

    /* Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 19003)*/
    /* SPR 19003: Take the local reference for list pointer.
     * Traversing original list, result into loss of data
     */
    p_list = p_options_resp->p_remote_stream_list;

	while(p_list!=ICF_NULL)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
        
		p_sprintf_count = icf_cmn_trace_stream_capab(p_list->p_data,p_sprintf_count);

		p_list = p_list->p_next;
		
		i++;
	}

    return p_sprintf_count;

}


icf_uint8_t* icf_cmn_trace_info_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    
    icf_info_cfm_st* p_info_cfm = ICF_NULL;

    p_info_cfm = (icf_info_cfm_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_info_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_info_cfm is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

       
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Result = %d\n",p_info_cfm->result));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n bit_mask = %d",p_info_cfm->bit_mask));

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_RESPONSE_CODE_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Response Code = %d\n",p_info_cfm->response_code));

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_ERROR_CAUSE_PRESENT)
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error Cause = %d",p_info_cfm->error_cause));
	p_sprintf_count = icf_cmn_trace_error_details(p_info_cfm->error_cause,p_sprintf_count);

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_HEADER_LIST_PRESENT)
       p_sprintf_count = icf_cmn_trace_header_list(p_info_cfm->header_list.hdr, p_info_cfm->header_list.count,p_sprintf_count);

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_TAG_LIST_PRESENT)
       p_sprintf_count = icf_cmn_trace_tag_list(p_info_cfm->tag_list.tag, p_info_cfm->tag_list.count,p_sprintf_count);

    if (p_info_cfm->bit_mask & ICF_INFO_CFM_BODY_LIST_PRESENT)
       p_sprintf_count = icf_cmn_trace_body_list(p_info_cfm->body_list.body,p_info_cfm->body_list.count,p_sprintf_count);
			
           return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_early_inc_call_cfm
 *
 * DESCRIPTION:     Print Early Call cfm messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_early_inc_call_cfm(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{

    icf_early_incoming_call_cfm_st* p_early_inc_call_cfm = ICF_NULL;

    p_early_inc_call_cfm = (icf_early_incoming_call_cfm_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_early_inc_call_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_early_inc_call_cfm is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nresult = %d", p_early_inc_call_cfm->result));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nline_id = %d", p_early_inc_call_cfm->line_id));

#ifdef ICF_SESSION_TIMER    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nsession_timer_enable = %d", p_early_inc_call_cfm->session_timer_enable));
#endif

    return p_sprintf_count;
}

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_error_details
 *
 * DESCRIPTION:     Prints the error detail which has occured
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/


icf_uint8_t* icf_cmn_trace_error_details(int error_no, icf_uint8_t* p_sprintf_count)
{

	switch(error_no)
	{
		case 0:
			break;

		case ICF_ERROR_MEM_ALLOC_FAILURE:
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_MEM_ALLOC_FAILURE"));  
			break;
			
		case ICF_ERROR_MEM_FREE_FAILURE:
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_MEM_FREE_FAILURE"));  
			break;
			
		case ICF_ERROR_TMR_START_FAILURE:
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_TMR_START_FAILURE"));  
			break;
			
		case ICF_ERROR_TMR_STOP_FAILURE: 
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_TMR_STOP_FAILURE"));  
			break;
			
		case ICF_ERROR_INVALID_API_ID:
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_INVALID_API_ID ")); 
			break;
			
		case ICF_ERROR_CALL_FAIL_NO_SDP:
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_CALL_FAIL_NO_SDP ")); 
			break;
			
		case ICF_MSG_RECEIVE_ERROR: 
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MSG_RECEIVE_ERROR "));      
			break;
			
		case ICF_INVALID_SRC_MODULE_ID: 
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_INVALID_SRC_MODULE_ID"));        
			break;
			
		case ICF_ERROR_MEM_INIT: 
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_MEM_INIT "));        
			break;
			
		case ICF_ERROR_MEM_DEINIT:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_MEM_DEINIT "));      
			 break;
			 
		case ICF_ERROR_TMR_INIT:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_TMR_INIT "));      
			 break;			
			 
		case  ICF_ERROR_UDP_SEND:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_UDP_SEND "));      
			 break;
			 
		case  ICF_ERROR_TCP_SEND: 
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_TCP_SEND"));       
			 break;
			 
		case ICF_ERROR_SOCKET_CREATE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_SOCKET_CREATE"));       
			 break;
			 
		case ICF_ERROR_SOCKET_BIND:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_SOCKET_BIND"));     
			 break;
			 
		case ICF_ERROR_SET_SOCKET:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_SET_SOCKET"));     
			 break;
			 
		case ICF_ERROR_UDP_RECV:
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_UDP_RECV "));    
    			 break;
    			 
		case ICF_CALL_CTX_BLK_EXHAUSTED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CALL_CTX_BLK_EXHAUSTED "));
		 break;
		 
		case ICF_RGN_CTX_BLK_EXHAUSTED :
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_RGN_CTX_BLK_EXHAUSTED "));
			 break;
			 
		case ICF_NO_MODULE_DATA_AVAILABLE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_NO_MODULE_DATA_AVAILABLE "));
			 break;
			 
		case ICF_NO_CALL_CTX_EXIST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_NO_CALL_CTX_EXIST "));
			 break;
			 
		case ICF_NO_RGN_CTX_EXIST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_NO_RGN_CTX_EXIST"));  
			 break;
			 
		case ICF_USER_NOT_ADDED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_USER_NOT_ADDED "));
			 break;
			 
		case ICF_DUPLICATE_USER:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_DUPLICATE_USER"));   
			 break;
			 
		case ICF_LA_INIT_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_LA_INIT_FAILURE "));    
			 break;
			 
		case ICF_MSG_PROC_FIELD_TYPE_MISMATCH:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MSG_PROC_FIELD_TYPE_MISMATCH "));      
			 break;
			 
		case ICF_MSG_PROC_OPR_MISMATCH:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MSG_PROC_OPR_MISMATCH "));   
			 break;
			 
		case ICF_ERROR_INVALID_TRNSPT_ADDR:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_INVALID_TRNSPT_ADDR"));    
			 break;
			 
		case ICF_POOL_EXHAUSTED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_POOL_EXHAUSTED "));  
			 break;
			 
		case ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE "));    
			 break;
			 
		case ICF_CAUSE_LINE_NOT_CONFIGURED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_LINE_NOT_CONFIGURED "));      
			 break
			 ;
		case ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED"));   
			 break;
		case ICF_CAUSE_INVALID_TIMER_VALUE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_TIMER_VALUE "));    
			 break;
			 
		case ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG"));  
			break;
			
		case ICF_CAUSE_MAX_NUM_APPLICATION_EXCEEDED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_NUM_APPLICATION_EXCEEDED "));   
			break;
			
		case ICF_CAUSE_ICF_NOT_INITIALISED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ICF_NOT_INITIALISED"));    
			break;
			
		case ICF_CAUSE_INVALID_ACCESS_TYPE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_ACCESS_TYPE"));    
			break;
						
		case ICF_CAUSE_INVALID_ACCESS_INFO:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_ACCESS_INFO ")); 
			break;
			
		case ICF_CAUSE_INVALID_VERSION_ID:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_VERSION_ID  "));      
			break;
			
		case ICF_CAUSE_INVALID_CALL_ID:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_CALL_ID  "));                   
			break;
			
		case ICF_CAUSE_INVALID_SOURCE_ID:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_SOURCE_ID  "));                       
			break;
			
		case ICF_CAUSE_INVALID_DESTINATION_ID:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_DESTINATION_ID "));                
			break;
			
		case ICF_CAUSE_DUPLICATE_REQUEST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_DUPLICATE_REQUEST "));    
			break;
		case ICF_CAUSE_MAX_APP_EXCEEDED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_APP_EXCEEDED"));                   
			break;
		case ICF_CAUSE_MAX_NETWORK_EXCEEDED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_NETWORK_EXCEEDED  "));      
			break;
			
		case ICF_CAUSE_ONGOING_REGISTRATION:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ONGOING_REGISTRATION "));            
			break;
			
		case  ICF_CAUSE_NO_PUBLIC_URI:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NO_PUBLIC_URI"));     
			break;
			
		case  ICF_CAUSE_NO_PRIVATE_URI:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NO_PRIVATE_URI "));                  
			break;
			
		case ICF_CAUSE_NO_AUTH_PARAMS:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NO_AUTH_PARAMS"));     
			break;
			
		case ICF_CAUSE_NO_HOME_NETWORK:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NO_HOME_NETWORK"));                 
			break;
			
		case ICF_CAUSE_TRANSPORT_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_TRANSPORT_FAILURE"));     
			break;
			
		case ICF_CAUSE_SECUITY_ASSOCIATION_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SECUITY_ASSOCIATION_FAILURE"));             
			break;
			
		case  ICF_CAUSE_AUTH_FAILED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_AUTH_FAILED"));   
			break;
			
		case ICF_CAUSE_REGISTERATION_RETRY_OVER:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REGISTERATION_RETRY_OVER "));                 
			break;
			
		case ICF_CAUSE_NETWORK_DEREGISTERED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NETWORK_DEREGISTERED "));   
			break;
			
		case ICF_CAUSE_PUBLIC_URI_BARRED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_PUBLIC_URI_BARRED "));   
			break;
			
		case ICF_CAUSE_HOME_NETWORK_NOT_AUTH:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_HOME_NETWORK_NOT_AUTH"));       
			break;
			
		case ICF_CAUSE_INVALID_CONTEXT:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_CONTEXT "));      
			break;
			
		case ICF_CAUSE_CONTEXT_BUSY:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CONTEXT_BUSY"));
			break;
			
		case ICF_CAUSE_INVALID_APP_ID:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_APP_ID  "));
		break;
		
		case ICF_CAUSE_INVALID_OPTION:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_OPTION "));
			break;
			
		case ICF_CAUSE_NETWORK_ERROR:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NETWORK_ERROR "));
			break;
			
		case ICF_CAUSE_TIMER_EXPIRY:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_TIMER_EXPIRY "));
			break;
			
		case ICF_CAUSE_CALL_ALREADY_ACTIVE :
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CALL_ALREADY_ACTIVE "));
			break;
			
		case ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE "));
		   	break;
		   	
		case ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE "));
		 	break;
		 	
		case ICF_CAUSE_VIDEO_CODEC_NOT_AVAILABLE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_VIDEO_CODEC_NOT_AVAILABLE "));
			break;
			
		case ICF_CAUSE_UNABLE_TO_RELEASE_RESOURCES:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_UNABLE_TO_RELEASE_RESOURCES "));
			break;
			
		case ICF_CAUSE_MODE_NOT_COMPATIBLE_TO_SDP:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MODE_NOT_COMPATIBLE_TO_SDP "));
			break;
			
		case ICF_CAUSE_UNABLE_TO_CREATE_MEDIA:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_UNABLE_TO_CREATE_MEDIA "));
			break;
			
		case ICF_CAUSE_INSUFFICIENT_PARAMETERS:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INSUFFICIENT_PARAMETERS "));
			break;
			
		case ICF_CAUSE_INSUFFICIENT_RESOURCES:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INSUFFICIENT_RESOURCES  "));
			 break;
			 
		case ICF_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION :
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION "));
			 break;
			 
		case ICF_CAUSE_SELF_IP_ADDR_BIND_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SELF_IP_ADDR_BIND_FAILURE "));
			 break;
			 
		case ICF_CAUSE_INVALID_STATS_ID:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_STATS_ID "));
 			 break;
 			 
		case ICF_CAUSE_IP_PORT_OUT_OF_RANGE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_IP_PORT_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_MEDIA_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MEDIA_FAILURE "));
			 break;
			 
		case ICF_CAUSE_MAX_LINES_OUT_OF_RANGE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_LINES_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_INVALID_LINE_ID :
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_LINE_ID "));
			 break;
			 
		case ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED "));
			 break;
			 
		case  ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED "));
			 break;
			 
		case ICF_CAUSE_INVALID_ADDRESS_RECEIVED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_ADDRESS_RECEIVED "));
			 break;
			 
		case ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE "));
			 break;
			 
		case ICF_ERROR_CAUSE_URI_NOT_DEREGISTERED :
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_CAUSE_URI_NOT_DEREGISTERED "));
			 break;
			 
		case ICF_CAUSE_ACTIVATE_DENIED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ACTIVATE_DENIED "));
			 break;
			 
		case ICF_CAUSE_ACTIVATION_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ACTIVATION_FAILURE "));
			 break;
			 
		case ICF_CAUSE_DEACTIVATION_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_DEACTIVATION_FAILURE"));
			 break;
			 
		case ICF_CAUSE_ILLEGAL_REQUEST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ILLEGAL_REQUEST "));
			 break;
			 
		case ICF_CAUSE_INVALID_CALL_TYPE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_CALL_TYPE "));
			 break;
			 
		case ICF_CAUSE_INVALID_ROUTING_INFO:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_ROUTING_INFO "));
			 break;
			 
		case ICF_CAUSE_FILTER_PARSE_FAILED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_FILTER_PARSE_FAILED "));
			 break;
			 
		case ICF_CAUSE_INTERNAL_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INTERNAL_FAILURE  "));
			 break;
			 
		case ICF_CAUSE_NW_CURRENTLY_NOT_AVAILABLE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NW_CURRENTLY_NOT_AVAILABLE  "));
			 break;
			 
		case ICF_CAUSE_INVALID_REG_REQUEST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_REG_REQUEST "));
			 break;	
			 	
		case ICF_CAUSE_SCHEME_NOT_SUPPORTED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SCHEME_NOT_SUPPORTED "));
			 break;
		case ICF_CAUSE_ALGO_NOT_SUPPORTED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ALGO_NOT_SUPPORTED "));
			 break;
		case ICF_CAUSE_PROPER_NONCE_NOT_RECD:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_PROPER_NONCE_NOT_RECD "));
			 break;
		case ICF_CAUSE_SQN_OUT_OF_RANGE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SQN_OUT_OF_RANGE "));
			 break;
			 
		case ICF_CAUSE_LINE_ID_MISMATCH:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_LINE_ID_MISMATCH "));
			 break;
			 
		case ICF_CAUSE_NW_DEACTIVATED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NW_DEACTIVATED "));      
			 break;
			 
		case ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE"));       
			 break;
			 
		case ICF_CAUSE_NW_PROBATION:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NW_PROBATION "));
			 break;
			 
		case ICF_CAUSE_NW_EXPIRATION:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NW_EXPIRATION"));      
			 break;
			 
		case ICF_CAUSE_NW_REJECTED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NW_REJECTED"));   
			 break;
			 
		case ICF_CAUSE_NW_UNREGISTERED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NW_UNREGISTERED "));
			 break;
			 
		case ICF_CAUSE_EMPTY_SUPP_HDR_LIST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_EMPTY_SUPP_HDR_LIST "));
			 break;
			 
		case ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ "));
			 break;
			 
/*		case ICF_CAUSE_SA_CREATION_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SA_CREATION_FAILURE "));
			 break;
			 
		case ICF_CAUSE_MULTIPLE_SA_EXIST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MULTIPLE_SA_EXIST "));
			 break;
			 
		case ICF_CAUSE_IPSEC_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_IPSEC_FAILURE "));
			 break;
*/			 
		case ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED "));
			 break;
			 
		case ICF_CAUSE_SERVICE_NOT_SUBSCRIBED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SERVICE_NOT_SUBSCRIBED "));
			 break;
			 
		case ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SCALE_PARAMS_NOT_CONFIGURED "));
			 break;
			 
		case ICF_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED "));
			 break;
			 
		case ICF_CAUSE_CALL_MODIFY_REQ_PENDING:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CALL_MODIFY_REQ_PENDING" ));
			 break;
			 
		case ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST "));
			 break;
			 
		case ICF_CAUSE_REMOTE_RELEASED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REMOTE_RELEASED "));
			 break;
			 
		case ICF_CAUSE_IMPLICITLY_REGISTERED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_IMPLICITLY_REGISTERED "));
			 break;
			 
		case ICF_CAUSE_MANDATORY_HDR_NOT_RCVD:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MANDATORY_HDR_NOT_RCVD "));
			 break;
			 
		case ICF_CAUSE_STREAM_ADDITION_NOT_ALLOWED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_STREAM_ADDITION_NOT_ALLOWED "));
		 	 break;
		 
		case ICF_CAUSE_FAX_CALL_NOT_ALLOWED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_FAX_CALL_NOT_ALLOWED "));
			 break;
			 
		case ICF_CAUSE_INVALID_FAX_REQUEST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_FAX_REQUEST "));
			 break;
			 
		case ICF_CAUSE_MIME_BODY_DECODE_FAILURE:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MIME_BODY_DECODE_FAILURE "));
  			 break;
  			 
		case ICF_CAUSE_CALL_SETUP_IN_PROGRESS:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CALL_SETUP_IN_PROGRESS "));
			 break;
			 
		case ICF_CAUSE_FAX_PROFILE_NOT_CONFIGURED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_FAX_PROFILE_NOT_CONFIGURED "));
 			 break;
 			 
		case ICF_CAUSE_FAX_REQ_PENDING:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_FAX_REQ_PENDING "));
			 break;
			 
		case ICF_CAUSE_FAX_CALL_ONGOING:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_FAX_CALL_ONGOING "));
			 break;
			 
		case ICF_CAUSE_SERVICE_ONGOING:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SERVICE_ONGOING "));
			 break;
			 
		case ICF_CAUSE_REALM_MISMATCH:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REALM_MISMATCH "));
			 break;
			 
		case ICF_CAUSE_BASIC_AUTH_SCHEME_NOT_SUPPORTED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_BASIC_AUTH_SCHEME_NOT_SUPPORTED "));
			 break;
			 
		case ICF_CAUSE_LICENSE_EXPIRED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_LICENSE_EXPIRED "));
			 break;
			 
		case ICF_CAUSE_USER_NOT_LICENSED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_USER_NOT_LICENSED "));
			 break;
			 
		case ICF_CAUSE_INFO_CFM_PENDING:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INFO_CFM_PENDING "));
			 break;
			 
		case ICF_CAUSE_MAX_SERVER_LIMIT_REACHED:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_SERVER_LIMIT_REACHED "));
			 break;
			 
		case ICF_CAUSE_SELF_ADDR_MODIFY_NOT_SUPP:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SELF_ADDR_MODIFY_NOT_SUPP"));
			 break;
			 
		case ICF_CAUSE_UNSUPPORTED_METHOD_IN_LIST:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_UNSUPPORTED_METHOD_IN_LIST"));
			 break;
			 
		case ICF_ERROR_END_CFG:
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_END_CFG"));
			 break;	

		case ICF_CAUSE_CLOSE_CONN_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CLOSE_CONN_FAILURE"));
			break;
			
		case ICF_CAUSE_NO_RESP_FROM_SERVER:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NO_RESP_FROM_SERVER"));
			break;

		case ICF_CAUSE_MAX_APPLICATION_EXCEEDED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_APPLICATION_EXCEEDED"));
			break;
			
		case ICF_CAUSE_MAX_CONNECTION_EXCEEDED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAX_CONNECTION_EXCEEDED "));
			break;
			
		case ICF_CAUSE_REQ_PENDING_ON_CURRENT_AUTH_DATA:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REQ_PENDING_ON_CURRENT_AUTH_DATA"));
			break;
			
		case ICF_CAUSE_SERVER_INFO_MISSING:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SERVER_INFO_MISSING"));
			break;
			
		case ICF_CAUSE_SERVER_INFO_REMOVED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SERVER_INFO_REMOVED"));
			break;
			
		case ICF_CAUSE_CONNECTION_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CONNECTION_FAILURE"));
			break;
			
		case ICF_CAUSE_CONNECTION_ALREADY_EXISTS:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CONNECTION_ALREADY_EXISTS"));
			break;
			
		case ICF_CAUSE_FROM_PORT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_FROM_PORT"));
			break;
			
		case ICF_CAUSE_INVALID_CONN_CLOSURE_TYPE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_CONN_CLOSURE_TYPE"));
			break;
			
		case ICF_CAUSE_PORT_SEND_HTTP_REQ:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_PORT_SEND_HTTP_REQ"));
			break;
			
		case ICF_CAUSE_PORT_MAKE_HTTP_REQ:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_PORT_MAKE_HTTP_REQ"));
			break;
			
		case ICF_CAUSE_SYNCML_HEADERS_NOT_PRESENT:p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SYNCML_HEADERS_NOT_PRESENT"));
			break;
			
		case ICF_CAUSE_INVALID_CONNECTION_ID:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_CONNECTION_ID "));
			break;
			
		case ICF_CAUSE_MANDATORY_HEADERS_NOT_PRESENT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MANDATORY_HEADERS_NOT_PRESENT"));
			break;
			
		case ICF_CAUSE_CONNECTION_REATTEMPT_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CONNECTION_REATTEMPT_FAILURE"));
			break;
			
		case ICF_CAUSE_WWW_AUTHENTICATE_NOT_PRESENT_IN_401:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_WWW_AUTHENTICATE_NOT_PRESENT_IN_401"));
			break;
			
		case ICF_CAUSE_INVALID_USERNAME_PASSWD:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_USERNAME_PASSWD"));
			break;
			
		case ICF_CAUSE_AUTH_DATA_NOT_PRESENT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_AUTH_DATA_NOT_PRESENT"));
			break;
			
		case ICF_CAUSE_UNKNOWN_AUTH_SCHEME_IN_401:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_UNKNOWN_AUTH_SCHEME_IN_401"));
			break;
			
		case ICF_CAUSE_REALM_NOT_PRESENT_IN_401:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REALM_NOT_PRESENT_IN_401"));
			break;
			
		case ICF_CAUSE_NONCE_NOT_PRESENT_IN_401:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NONCE_NOT_PRESENT_IN_401"));
			break;
			
		case ICF_CAUSE_APPLICATION_REMOVED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_APPLICATION_REMOVED "));
			break;
			
		case ICF_CAUSE_NETWORK_DEACTIVATED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NETWORK_DEACTIVATED"));
			break;
			
		case ICF_CAUSE_APPLICATION_ADDED_AGAIN:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_APPLICATION_ADDED_AGAIN"));
			break;
			
		case ICF_CAUSE_CLOSE_CONN_REQ:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CLOSE_CONN_REQ"));
			break;
			
		case ICF_CAUSE_MAKE_AUTHORISATION_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MAKE_AUTHORISATION_FAILED"));
			break;
			
		case ICF_CAUSE_INVALID_CONNECTION_USAGE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_CONNECTION_USAGE"));
			break;
			
		case ICF_CAUSE_NW_RESP_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NW_RESP_FAILED"));
			break;
			
		case ICF_CAUSE_RING_TONE_NOT_SUPPORTED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_RING_TONE_NOT_SUPPORTED"));
			break;
			
		case ICF_CAUSE_INTERNAL_ERROR:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INTERNAL_ERROR"));
			break;
			
		case ICF_CAUSE_CONNECTION_FOR_CALLID_NOT_FOUND:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CONNECTION_FOR_CALLID_NOT_FOUND"));
			break;
			
		case ICF_CAUSE_INVALID_VALUE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_VALUE"));
			break;
			
		case ICF_CAUSE_NOT_SUPPORTED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_NOT_SUPPORTED"));
			break;
			
		case ICF_CAUSE_FILE_PATH_NOT_FOUND:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_FILE_PATH_NOT_FOUND"));
			break;
			
		case ICF_CAUSE_TONE_NOT_SUPPORTED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_TONE_NOT_SUPPORTED"));
			break;
			
		case ICF_CAUSE_INVALID_ACTION:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_ACTION"));
			break;
			
		case ICF_CAUSE_INVALID_PLAY_OUT_VAL:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_PLAY_OUT_VAL"));
			break;
			
		case ICF_CAUSE_INVALID_FILE_TYPE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_FILE_TYPE"));
			break;
			
		case ICF_CAUSE_INVALID_DIGIT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_DIGIT"));
			break;
			
		case ICF_CAUSE_OUT_OF_RANGE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_OUT_OF_RANGE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_TYPE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_JB_TYPE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_SCALING_FACT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_JB_SCALING_FACT"));
			break;
			
		case ICF_CAUSE_INVALID_JB_INIT_SIZE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_JB_INIT_SIZE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_MAX_SIZE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_JB_MAX_SIZE"));
			break;
			
		case ICF_CAUSE_INVALID_JB_MIN_SIZE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_JB_MIN_SIZE"));
			break;
			
		case ICF_ERROR_UATK_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_UATK_FAILURE"));
			break;
			
		case ICF_CONNECT_RESP_PENDING:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CONNECT_RESP_PENDING"));
			break;
			
		case ICF_ERROR_NW_INTERFACE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_NW_INTERFACE"));
			break;
			
		case ICF_ERROR_RGM_INIT_CMPLT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_RGM_INIT_CMPLT"));
			break;
			
		case ICF_ERROR_DBM_PORT_DATA:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_DBM_PORT_DATA"));
			break;
			
		case ICF_ERROR_ES_INIT_CMPLT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_ES_INIT_CMPLT"));
			break;
			
		case ICF_ERROR_INIT_CMPLT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_INIT_CMPLT"));
			break;
			
		case ICF_MSG_PROC_INIT_NOT_COMPLETE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MSG_PROC_INIT_NOT_COMPLETE"));
			break;
			
		case ICF_INVALID_API_ID_RECD:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_INVALID_API_ID_RECD"));
			break;
			
		case ICF_INVALID_MSG_ID_RECD:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_INVALID_MSG_ID_RECD"));
			break;
			
		case ICF_MODIFY_USER_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MODIFY_USER_FAILED"));
			break;
			
		case ICF_PROCESS_MODIFY_USER_BLK_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_PROCESS_MODIFY_USER_BLK_FAILED"));
			break;
			
		case ICF_MODIFY_REGISTRAR_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MODIFY_REGISTRAR_FAILED"));
			break;
			
		case ICF_PROCESS_MODIFY_REG_BLK_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_PROCESS_MODIFY_REG_BLK_FAILED"));
			break;
			
		case ICF_MODIFY_DURATION_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MODIFY_DURATION_FAILED"));
			break;
			
		case ICF_PROCESS_MODIFY_DUR_BLK_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_PROCESS_MODIFY_DUR_BLK_FAILED"));
			break;
			
		case ICF_AUTHENTICATION_INFO_ABSENT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_AUTHENTICATION_INFO_ABSENT"));
			break;
			
		case ICF_INVALID_NETWORK_RESPONSE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_INVALID_NETWORK_RESPONSE"));
			break;
			
		case ICF_UNABLE_TO_SEND_REG_REQ:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_UNABLE_TO_SEND_REG_REQ"));
			break;
			
		case ICF_UNABLE_TO_SEND_DEREG_REQ:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_UNABLE_TO_SEND_DEREG_REQ"));
			break;
			
		case ICF_INIT_COMPLETE_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_INIT_COMPLETE_FAILED"));
			break;
			
		case ICF_NO_SRVC_CTX_EXIST:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_NO_SRVC_CTX_EXIST"));
			break;
			
		case ICF_SRVC_CTX_BLK_EXHAUSTED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_SRVC_CTX_BLK_EXHAUSTED"));
			break;
			
		case ICF_SRVC_CTX_LGC_BLK_EXHAUSTED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_SRVC_CTX_LGC_BLK_EXHAUSTED"));
			break;
			
		case ICF_ERROR_SERVICE_NOT_SUBSCRIBED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_SERVICE_NOT_SUBSCRIBED"));
			break;
			
		case ICF_ERROR_SERVICE_NOT_ACTIVE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_SERVICE_NOT_ACTIVE"));
			break;
			
		case ICF_ERROR_INVALID_SERVICE_INTERACTION:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_INVALID_SERVICE_INTERACTION"));
			break;
			
		case ICF_ERROR_SL_NOT_RUNNING:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_SL_NOT_RUNNING"));
			break;
			
		case ICF_ERROR_INV_PAYLOAD_RECVD:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_INV_PAYLOAD_RECVD"));
			break;
			
		case ICF_ERROR_INVALID_PARAM:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_INVALID_PARAM"));
			break;
			
		case ICF_ERROR_FIFO_CREATION:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_FIFO_CREATION"));
			break;
			
		case ICF_ERROR_FIFO_SEND:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_FIFO_SEND"));
			break;
			
		case ICF_ERROR_FIFO_RECV:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_FIFO_RECV"));
			break;
			
		case ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND"));
			break;
			
		case ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND"));
			break;
			
		case ICF_MM_AL_NO_VALID_CODEC_RETURNED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MM_AL_NO_VALID_CODEC_RETURNED"));
			break;
			
		case ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI"));
			break;
			
		case ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM"));
			break;
			
		case ICF_TERMINAL_FAILURE_CALL_CLEAR:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_TERMINAL_FAILURE_CALL_CLEAR"));
			break;
			
		case ICF_ERROR_NO_API_HANDLER:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_NO_API_HANDLER"));
			break;
			
		case ICF_ERROR_CALL_WAITING_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_CALL_WAITING_FAILURE"));
			break;
			
		case ICF_ERROR_LIC_INTEGRTY:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_LIC_INTEGRTY"));
			break;
			
		case ICF_RESPONSE_SEND_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_RESPONSE_SEND_FAILURE"));
			break;
			
		case ICF_ERROR_CALL_LOGGING_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_CALL_LOGGING_FAILURE"));
			break;
			
		case ICF_CAUSE_MM_AL_MEMORY_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_MM_AL_MEMORY_FAILURE"));
			break;
			
		case ICF_CAUSE_INVALID_SERVICE_ACTION:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_INVALID_SERVICE_ACTION"));
			break;
			
		case ICF_CAUSE_PARTY_ALREADY_ON_LOCAL_HOLD:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_PARTY_ALREADY_ON_LOCAL_HOLD "));
			break;
			
		case ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ"));
			break;
			
		case ICF_CAUSE_CALL_MERGE_FAIL:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_CALL_MERGE_FAIL"));
			break;
			
		case ICF_CAUSE_SERVICE_NOT_ACTIVATED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SERVICE_NOT_ACTIVATED "));
			break;
			
		case ICF_CAUSE_PROHIBITED_BY_SERVICE_INTERACTION:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_PROHIBITED_BY_SERVICE_INTERACTION"));
			break;
			
		case ICF_CAUSE_GPRS_SUSPENDED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_GPRS_SUSPENDED"));
			break;
			
		case ICF_RES_THRESHOLD_REACHED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_RES_THRESHOLD_REACHED "));
			break;
			
		case ICF_ERROR_NO_PREF_ID_EXISTS:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_NO_PREF_ID_EXISTS"));
			break;
			
		case ICF_ERROR_GENERIC:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_GENERIC"));
			break;
			
		case ICF_ERROR_NO_MMI_CONTEXT_FOUND:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_NO_MMI_CONTEXT_FOUND"));
			break;
			
		/*case ICF_ERROR_INCOMPLETE_MSG_RECEIVED:
	                {
	                 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_INCOMPLETE_MSG_RECEIVED "));
	                 break;
	
	               }*/
		case ICF_ERROR_REFER_PENDING:
	                {   
	                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_REFER_PENDING"));
	                break;
	                }
		case ICF_ERROR_MISMATCH_IN_ADDR_PLAN:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_MISMATCH_IN_ADDR_PLAN"));
			break;
			
		case ICF_CAUSE_DNS_QUERY_FAILURE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_DNS_QUERY_FAILURE"));
			break;
			
		case ICF_CAUSE_REQUEST_ALREADY_PENDING:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REQUEST_ALREADY_PENDING"));
			break;
			
		case ICF_CAUSE_REMOTE_UNREACHABLE:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REMOTE_UNREACHABLE"));
			break;
			
		case ICF_CAUSE_IN_DIALOG_OTG_OPTIONS_RECEIVED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_IN_DIALOG_OTG_OPTIONS_RECEIVED"));
			break;
			
		case ICF_CAUSE_REFER_TO_ABSENT:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_REFER_TO_ABSENT"));
			break;
			
		case ICF_CAUSE_OPEN_NW_SERVER_FAILED:
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_OPEN_NW_SERVER_FAILED"));
			break;

                case ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD:
               p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_ERROR_DIRECT_ROUTED_CALL_NOT_ALWD"));
		        break;

               case ICF_CAUSE_SRTP_NEG_FAILURE:
               p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Error String = ICF_CAUSE_SRTP_NEG_FAILURE"));
		        break;

	}

return p_sprintf_count;
}

/********* Traces Add Start as per SPR 17213 **********/

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_create_ext_xconf_req
 *
 * DESCRIPTION:     Print create xconf request messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_create_ext_xconf_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_create_ext_xconf_req_st *p_create_xconf = ICF_NULL;
      icf_list_st *p_pref_media_profile = ICF_NULL;
      icf_uint8_t i = 0;

      p_create_xconf = (icf_create_ext_xconf_req_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_create_xconf)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_create_xconf is NULL.\n"))
          return p_sprintf_count;
      }         
      
      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \nline id = %d, "
                              ,p_create_xconf->bitmask,p_create_xconf->line_id))
     
      if(p_create_xconf->bitmask & ICF_CREATE_XCONF_CONF_URI_PRESENT)
      { 
           p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"conference_uri_addr_type = %d,"
                              "\nconference_uri_addr_val = %s, "
                              ,p_create_xconf->conference_uri.addr_type
                              ,p_create_xconf->conference_uri.addr_val.str))
      }
       
      if(p_create_xconf->bitmask & ICF_CREATE_XCONF_EXIST_CALL_ID_PRESENT)
      {   
           p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"existing_call_id = %d, "
                              ,p_create_xconf->existing_call_id))
      }
 
      /* Header LIST */
      if (p_create_xconf->bitmask & ICF_HDR_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_header_list(p_create_xconf->header_list.hdr,
                             p_create_xconf->header_list.count,p_sprintf_count);
      }

      if (p_create_xconf->bitmask & ICF_TAG_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_tag_list(p_create_xconf->tag_list.tag, 
                             p_create_xconf->tag_list.count,p_sprintf_count);
      }

      if (p_create_xconf->bitmask & ICF_CREATE_XCONF_PREF_MEDIA_PROFILE_PRESENT)
      {
            i = 1;

            p_pref_media_profile = p_create_xconf->p_pref_media_profile;

            while(p_pref_media_profile!=ICF_NULL)
            {
                  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));

                  p_sprintf_count = icf_cmn_trace_stream_capab(p_pref_media_profile->p_data,p_sprintf_count);

                  p_pref_media_profile = p_pref_media_profile->p_next;

                  i++;
             }
       } 
      
       if(p_create_xconf->bitmask & ICF_CREATE_XCONF_CALLING_PARTY_ADDR_PRESENT)
       {
             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,

                                "calling_party_addr = (addr_type = %d, %s)",

                                p_create_xconf->calling_party_address.addr_type,
  
                                p_create_xconf->calling_party_address.addr_val.str))
       }

       if(p_create_xconf->bitmask & ICF_CREATE_XCONF_CALLING_PARTY_NAME_PRESENT)
       {
             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,

                                "\ncalling_party_name = %s",

                                p_create_xconf->calling_party_name.str))
       }

       if(p_create_xconf->bitmask & ICF_CREATE_XCONF_TOGGLE_PRES_IND_PRESENT)
       {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                               "\ntoggle_presentation_indicator = %d, ",
                               p_create_xconf->toggle_presentation_indicator))
       }

       return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_add_xconf_party_req
 *
 * DESCRIPTION:     Print add xconf party request messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_add_xconf_party_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_add_xconf_party_req_st *p_add_xconf_party = ICF_NULL;

      p_add_xconf_party = (icf_add_xconf_party_req_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_add_xconf_party)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_add_xconf_party is NULL.\n"))
          return p_sprintf_count;
      }       

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, "
                              ,p_add_xconf_party->bitmask))

      /* Header LIST */
      if (p_add_xconf_party->bitmask & ICF_HDR_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_header_list(p_add_xconf_party->header_list.hdr,
                             p_add_xconf_party->header_list.count,p_sprintf_count);
      }

      if (p_add_xconf_party->bitmask & ICF_TAG_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_tag_list(p_add_xconf_party->tag_list.tag,
                             p_add_xconf_party->tag_list.count,p_sprintf_count);
      }

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"add_party_id = %d, "
                         ,p_add_xconf_party->add_party_id))

      return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_delete_xconf_party_req
 *
 * DESCRIPTION:     Print delete xconf party request messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_delete_xconf_party_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_delete_xconf_party_req_st *p_delete_xconf_party = ICF_NULL;

      p_delete_xconf_party = (icf_delete_xconf_party_req_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_delete_xconf_party)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_delete_xconf_party is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, "
                              ,p_delete_xconf_party->bitmask))

      /* Header LIST */
      if (p_delete_xconf_party->bitmask & ICF_HDR_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_header_list(p_delete_xconf_party->header_list.hdr,
                             p_delete_xconf_party->header_list.count,p_sprintf_count);
      }

      if (p_delete_xconf_party->bitmask & ICF_TAG_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_tag_list(p_delete_xconf_party->tag_list.tag,
                             p_delete_xconf_party->tag_list.count,p_sprintf_count);
      }

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"del_party_uri_addr_type = %d,"
                         "\ndel_party_uri_addr_val = %s, "
                         ,p_delete_xconf_party->del_party_uri.addr_type
                         ,p_delete_xconf_party->del_party_uri.addr_val.str))

      return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_release_xconf_req
 *
 * DESCRIPTION:     Print release xconf request messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_release_xconf_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_release_xconf_req_st *p_release_xconf = ICF_NULL;

      p_release_xconf = (icf_release_xconf_req_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_release_xconf)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_release_xconf is NULL.\n"))
          return p_sprintf_count;
      }           

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, "
                              ,p_release_xconf->bitmask))

      /* Header LIST */
      if (p_release_xconf->bitmask & ICF_HDR_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_header_list(p_release_xconf->header_list.hdr,
                             p_release_xconf->header_list.count,p_sprintf_count);
      }

      if (p_release_xconf->bitmask & ICF_TAG_LIST_PRESENT)
      {
           p_sprintf_count = icf_cmn_trace_tag_list(p_release_xconf->tag_list.tag,
                             p_release_xconf->tag_list.count,p_sprintf_count);
      }

      return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_release_xconf_resp
 *
 * DESCRIPTION:     Print release xconf response messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_release_xconf_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_release_xconf_resp_st *p_release_xconf = ICF_NULL;

      p_release_xconf = (icf_release_xconf_resp_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_release_xconf)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_release_xconf is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_code = %d"
                         ,p_release_xconf->result,p_release_xconf->error_code))

      return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_create_ext_xconf_resp
 *
 * DESCRIPTION:     Print create ext xconf response messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_create_ext_xconf_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_create_ext_xconf_resp_st *p_create_xconf = ICF_NULL;

      p_create_xconf = (icf_create_ext_xconf_resp_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_create_xconf)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_create_xconf is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, "
                              ,p_create_xconf->bitmask))

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_cause = %d"
                         ,p_create_xconf->result,p_create_xconf->error_cause))

      return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_add_xconf_party_resp
 *
 * DESCRIPTION:     Print add xconf party response messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_add_xconf_party_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_add_xconf_party_resp_st *p_add_xconf_party = ICF_NULL;

      p_add_xconf_party = (icf_add_xconf_party_resp_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_add_xconf_party)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_add_xconf_party is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_cause = %d"
                         ,p_add_xconf_party->result,p_add_xconf_party->error_cause))
 
      return p_sprintf_count;

}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_delete_xconf_party_resp
 *
 * DESCRIPTION:     Print delete xconf party response messages
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_delete_xconf_party_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_delete_xconf_party_resp_st *p_delete_xconf_party = ICF_NULL;

      p_delete_xconf_party = (icf_delete_xconf_party_resp_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_delete_xconf_party)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_delete_xconf_party is NULL.\n"))
          return p_sprintf_count;
      }          

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_code = %d"
                         ,p_delete_xconf_party->result,p_delete_xconf_party->error_code))

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"deleted_party_uri_addr_type = %d,"
                         "\ndeleted_party_uri_addr_val = %s, "
                         ,p_delete_xconf_party->deleted_party_uri.addr_type
                         ,p_delete_xconf_party->deleted_party_uri.addr_val.str))

      return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_xconf_release_ind
 *
 * DESCRIPTION:     Print xconf release indication
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_xconf_release_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_xconf_release_ind_st *p_xconf_release_ind = ICF_NULL;

      p_xconf_release_ind = (icf_xconf_release_ind_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_xconf_release_ind)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_xconf_release_ind is NULL.\n"))
          return p_sprintf_count;
      }          

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                         "\ncall_clear_reason = %d",
                         p_xconf_release_ind->call_clear_reason));

      return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_early_incoming_call_ind
 *
 * DESCRIPTION:     Print early incoming call indication
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_early_incoming_call_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_early_incoming_call_ind_st *p_early_inc_call_ind = ICF_NULL;

      p_early_inc_call_ind = (icf_early_incoming_call_ind_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_early_inc_call_ind)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_early_inc_call_ind is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, "
                              ,p_early_inc_call_ind->bit_mask))

      if(p_early_inc_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_ADDR)
       {
             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,

                                "calling_party_addr = (addr_type = %d, %s)",

                                p_early_inc_call_ind->calling_party_addr.addr_type,

                                p_early_inc_call_ind->calling_party_addr.addr_val.str))
       }

       if(p_early_inc_call_ind->bit_mask & ICF_EARLY_CALL_CLG_PTY_NAME)
       {
             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,

                                "\ncalling_party_name = %s",

                                p_early_inc_call_ind->calling_party_name.str))
       }
       
       if(p_early_inc_call_ind->bit_mask & ICF_EARLY_CALL_CALLED_PTY_ADDR)
       {
             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,

                                "calling_party_addr = (addr_type = %d, %s)",

                                p_early_inc_call_ind->called_party_addr.addr_type,

                                p_early_inc_call_ind->called_party_addr.addr_val.str))
       }

       /* Header LIST */
       if (p_early_inc_call_ind->bit_mask & ICF_HDR_LIST_PRESENT)
       {
             p_sprintf_count = icf_cmn_trace_header_list(p_early_inc_call_ind->header_list.hdr,
                             p_early_inc_call_ind->header_list.count,p_sprintf_count);
       }
     
       return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_early_incoming_call_cfm
 *
 * DESCRIPTION:     Print early incoming call confirmation
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_early_incoming_call_cfm(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_early_incoming_call_cfm_st *p_early_inc_call_cfm = ICF_NULL;

      p_early_inc_call_cfm = (icf_early_incoming_call_cfm_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_early_inc_call_cfm)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_early_inc_call_cfm is NULL.\n"))
          return p_sprintf_count;
      }          

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, ",p_early_inc_call_cfm->result))

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nLine_id = %d",p_early_inc_call_cfm->line_id))

      return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_unexpected_msg_ind
 *
 * DESCRIPTION:     Print unexpected message indication
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_unexpected_msg_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_unexpected_msg_ind_st *p_unexpected_msg = ICF_NULL;

      p_unexpected_msg = (icf_unexpected_msg_ind_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_unexpected_msg)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_unexpected_msg is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApi_id = %d",
              p_unexpected_msg->api_id))
     
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nerror_cause = %d"
                         ,p_unexpected_msg->error_cause))

      return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_remote_call_transfer_initiated_cfm
 *
 * DESCRIPTION:     Print remote call transfer initiated confirmation
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_remote_call_transfer_initiated_cfm(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_remote_call_transfer_initiated_cfm_st *p_rem_call_transfer = ICF_NULL;

      p_rem_call_transfer = (icf_remote_call_transfer_initiated_cfm_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_rem_call_transfer)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_rem_call_transfer is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, "
                         ,p_rem_call_transfer->bit_mask))
  
      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, "
                         ,p_rem_call_transfer->result)) 

      if(p_rem_call_transfer->bit_mask & ICF_CALL_TRANS_INIT_ERROR_CAUSE_PRESENT)
      {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nerror_code = %d"
                               ,p_rem_call_transfer->error_code))
      }

      if(p_rem_call_transfer->bit_mask & ICF_CALL_TRANS_INIT_CFM_CODE_PRESENT)
      {
            p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"response_code = %d, "
                               ,p_rem_call_transfer->response_code))
      }

      return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_get_register_status_req
 *
 * DESCRIPTION:     Print register status request
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_get_register_status_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_get_register_status_req_st *p_reg_status = ICF_NULL;

      p_reg_status = (icf_get_register_status_req_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_reg_status)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reg_status is NULL.\n"))
          return p_sprintf_count;
      }         

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nLine_id = %d", p_reg_status->line_id))

      return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_get_register_status_resp
 *
 * DESCRIPTION:     Print register status response
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_get_register_status_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_get_register_status_resp_st *p_reg_status = ICF_NULL;
      icf_list_st                     *p_user_reg = ICF_NULL;
      icf_uint8_t i = 0;

      p_reg_status = (icf_get_register_status_resp_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_reg_status)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reg_status is NULL.\n"))
          return p_sprintf_count;
      }         

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      i = 1;

      p_user_reg = p_reg_status->p_user_reg_status;

      while(p_user_reg != ICF_NULL)
      {
             /*Fix For SPR : 18999*/
             if (ICF_NULL == p_user_reg->p_data)
             {
                 ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_glb_pdb is NULL.\n"))
                 return p_sprintf_count;
             }   
             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nUser %d:",i));
             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,

                                "\nuser_addr_type = %d,user_addr_val = %s",

                                ((icf_user_reg_status_st *)(p_user_reg)->p_data)->user.addr_type,

                                ((icf_user_reg_status_st *)(p_user_reg)->p_data)->user.addr_val.str))

             p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,

                                "\nreg_status = %d",

                                ((icf_user_reg_status_st *)(p_user_reg)->p_data)->reg_status))

             p_user_reg = p_user_reg->p_next;

             i++;
       }

       p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nRegistrar address = registrar_addr_type = %d addr = ",
                          p_reg_status->registrar_address.addr.addr_type));

        switch (p_reg_status->registrar_address.addr.addr_type)
        {
            case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%s, ",
                    p_reg_status->registrar_address.addr.addr.domain.str));
                break;

            case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"%d.%d.%d.%d, ",
                    p_reg_status->registrar_address.addr.addr.ipv4_addr.octet_1,
                    p_reg_status->registrar_address.addr.addr.ipv4_addr.octet_2,
                    p_reg_status->registrar_address.addr.addr.ipv4_addr.octet_3,
                    p_reg_status->registrar_address.addr.addr.ipv4_addr.octet_4));
                break;
          }

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n registrar_port = %d, ",
              p_reg_status->registrar_address.port_num));

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n nexpires_duration = %d, ",
              p_reg_status->expires_duration));

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, "
                         ,p_reg_status->result))

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nerror_cause = %d"
                         ,p_reg_status->error_cause))

        return p_sprintf_count;
}

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_mm_unexpected_msg_ind
 *
 * DESCRIPTION:     Print mm unexpected message indication
 *
 * Functions called:
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_mm_unexpected_msg_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
      icf_mm_unexpected_msg_ind_st *p_unexpected_msg = ICF_NULL;

      p_unexpected_msg = (icf_mm_unexpected_msg_ind_st*)p_msg->payload;
      /*Fix For SPR : 18999*/
      if (ICF_NULL == p_unexpected_msg)
      {
          ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_unexpected_msg is NULL.\n"))
          return p_sprintf_count;
      }        

      p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nApi_id = %d",
              p_unexpected_msg->api_id))

      p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nerror_cause = %d"
                         ,p_unexpected_msg->error_cause))
      
      return p_sprintf_count;
}



/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_call_modify_cancel_resp
 *
 * DESCRIPTION:     Print call modify cancel resp traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_call_modify_cancel_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_call_modify_cancel_resp_st* p_call_modify_cancel_resp = ICF_NULL;


    p_call_modify_cancel_resp = (icf_call_modify_cancel_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_call_modify_cancel_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_call_modify_cancel_resp is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nresult = %d, error_cause = %d",
			p_call_modify_cancel_resp->result,p_call_modify_cancel_resp->error_cause));
    p_sprintf_count = icf_cmn_trace_error_details(p_call_modify_cancel_resp->error_cause,p_sprintf_count);
    return p_sprintf_count;
}


/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_progress_ind
 *
 * DESCRIPTION:     Print progress Indication traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_progress_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_progress_ind_st* p_progress_ind = ICF_NULL;
    
     p_progress_ind = (icf_progress_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_progress_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_progress_ind is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
               "\nresponse code = %d", p_progress_ind->response_code));
    return p_sprintf_count;
}



/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_set_trace_req
 *
 * DESCRIPTION:     Print set_trace_req traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_set_trace_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_set_trace_req_st* p_set_trace_req = ICF_NULL;
    
     p_set_trace_req = (icf_set_trace_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_set_trace_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_set_trace_req is NULL.\n"))
        return p_sprintf_count;
    }         

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %d",p_set_trace_req->bitmask));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\nset_trace_req_status = %d",
        p_set_trace_req->status));

    if(ICF_TRACE_TYPE_PRESENT & p_set_trace_req->bitmask)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Trace type = %d",
        p_set_trace_req->trace_type));	
    }   
    if(ICF_TRACE_CRITERIA_PRESENT & p_set_trace_req->bitmask)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Trace Criteria = %d",
        p_set_trace_req->trace_criteria));
    }
    if(ICF_TRACE_RULE_PRESENT & p_set_trace_req->bitmask)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Trace rule = %d",
        p_set_trace_req->trace_rule));
    }
    return p_sprintf_count;
}



/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_report_trace_ind
 *
 * DESCRIPTION:     Print report_trace_ind traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_report_trace_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_report_trace_ind_st* p_report_trace_ind = ICF_NULL;

    p_report_trace_ind = (icf_report_trace_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_report_trace_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_report_trace_ind is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Trace ID = %d",
        p_report_trace_ind->trace_id));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Trace type = %d",
        p_report_trace_ind->trace_type));
   
	p_sprintf_count = icf_cmn_trace_trace_data(&p_report_trace_ind->trace_data,p_sprintf_count); 

	return p_sprintf_count;
}    



#ifdef ICF_ERROR_ENABLE
/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_set_error_report_level_req
 *
 * DESCRIPTION:     Print icf_set_error_report_level_req traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_set_error_report_level_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_set_error_report_level_req_st* p_report_level_req = ICF_NULL;

    p_report_level_req  = (icf_set_error_report_level_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_report_level_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_report_level_req is NULL.\n"))
        return p_sprintf_count;
    }         

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Status = %d",
        p_report_level_req->status));
#if 0
    switch(p_report_level_req->error_level)
    {
	case 0:
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Level = ICF_ERROR_LEVEL_UNDEFINED"));
	break;
	case 1:
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Level = ICF_ERROR_LEVEL_CRITICAL"));
	break;
	case 2:
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Level = ICF_ERROR_LEVEL_MAJOR"));
	break;
	case 3:
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Level = ICF_ERROR_LEVEL_INFO"));
    	break;
 
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_UNDEFINED)
    {
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_UNDEFINED"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_SYSTEM)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_SYSTEM"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_INTERFACE)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_INTERFACE"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_SIP)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_SIP"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_CALL_CONTROL)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_CALL_CONTROL"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_MEDIA)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_MEDIA"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_CONFIG)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_CONFIG"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_REGISTRATION)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_REGISTRATION"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_SM)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_SM"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_CALL_HOLD_RESUME)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_CALL_HOLD_RESUME"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_THREE_WAY_CALL)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_THREE_WAY_CALL"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_CALL_TRANSFER_UNATTENDED)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_CALL_TRANSFER_UNATTENDED"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_CALL_TRANSFER_ATTENDED)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_CALL_TRANSFER_ATTENDED"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_REFER)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_REFER"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_CALL_FORWARDING)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_CALL_FORWARDING"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_CALL_WAITING)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_CALL_WAITING"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_MWI)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_MWI"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_SUBM)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_SUBM"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_GENERAL)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_GENERAL"));
    }
    if(p_report_level_req->error_type & ICF_ERROR_TYPE_STACK_ERROR)
    {
         p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Error Type = ICF_ERROR_TYPE_STACK_ERROR"));
    }
#endif
    return p_sprintf_count;
}



/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_report_error_ind
 *
 * DESCRIPTION:     Print report_error_ind traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_report_error_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_report_error_ind_st* p_report_error_ind = ICF_NULL;

    p_report_error_ind = (icf_report_error_ind_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_report_error_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_glb_pdb is NULL.\n"))
        return p_sprintf_count;
    }          

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        "\n Error ID = %d",
        p_report_error_ind->error_id));
    p_sprintf_count = icf_cmn_trace_error_data(&p_report_error_ind->error_data,p_sprintf_count); 
    return p_sprintf_count;

}
#endif



#ifdef ICF_STATS_ENABLE
/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_stats_req	
 *
 * DESCRIPTION:     Print stats_req traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_stats_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_stats_req_st* p_stats_req = ICF_NULL;

     p_stats_req = (icf_stats_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stats_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stats_req is NULL.\n"))
        return p_sprintf_count;
    }  
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    if(ICF_TRUE == p_stats_req->reset_stats)
    {
    	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Reset Stats = TRUE"));
    }
    else if(ICF_FALSE == p_stats_req->reset_stats)
    {
	 p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Reset Stats = FALSE"));
    }

}


/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_stats_resp     
 *
 * DESCRIPTION:     Print stats_resp traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_stats_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_stats_resp_st* p_stats_resp = ICF_NULL;

    p_stats_resp = (icf_stats_resp_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stats_resp)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stats_resp is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    
    p_sprintf_count = icf_cmn_trace_stats_data(p_stats_resp->stats_data,p_sprintf_count);

        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
        (icf_uint8_t *)"\n Last_reset Time  = %d seconds and %d milisecs",p_stats_resp->last_reset_time.time_in_secs,
	p_stats_resp->last_reset_time.time_in_millisecs));
/*   NOT COMPLETED YET */

}

#endif

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_media_connect_req
 *
 * DESCRIPTION:     Print media_connect_req traces
 *
 * Functions called:
 *
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_media_connect_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_uint8_t  header_list_count = 0, tag_list_count = 0,
                    body_list_count = 0,i=0;
    icf_list_st *p_stream_list;

    icf_media_connect_req_st* p_media_connect_req = ICF_NULL;

    p_media_connect_req = (icf_media_connect_req_st*)p_msg->payload;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_media_connect_req)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_media_connect_req is NULL.\n"))
        return p_sprintf_count;
    }       

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count, 
        "\nBit Mask = %d", p_media_connect_req->bit_mask));
    if( ICF_PRIVACY_IND_PRESENT & p_media_connect_req->bit_mask)
    {
	    if(ICF_TRUE == p_media_connect_req->privacy_ind)
	    {
	        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n Privacy Ind = TRUE"));
	    }
	    else if(ICF_FALSE == p_media_connect_req->privacy_ind)
	    {
	        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
            "\n Privacy Ind = FALSE"));
	    }
    }
    
    /* Header LIST */
    if (p_media_connect_req->bit_mask & ICF_HDR_LIST_PRESENT) 
    {
        header_list_count = (icf_uint8_t)p_media_connect_req->header_list.count;
        p_sprintf_count = icf_cmn_trace_header_list(p_media_connect_req->header_list.hdr, header_list_count,p_sprintf_count);
    }


    /* Tag LIST  */    
    if (p_media_connect_req->bit_mask & ICF_TAG_LIST_PRESENT) 
    {
        tag_list_count = (icf_uint8_t)p_media_connect_req->tag_list.count;
        p_sprintf_count = icf_cmn_trace_tag_list(p_media_connect_req->tag_list.tag, tag_list_count,p_sprintf_count);
    }


    /* Body LIST  */
    if (p_media_connect_req->bit_mask & ICF_BODY_LIST_PRESENT) 
    {
        body_list_count = (icf_uint8_t)p_media_connect_req->body_list.count;
        p_sprintf_count = icf_cmn_trace_body_list(p_media_connect_req->body_list.body, body_list_count,p_sprintf_count);
    }

    if(p_media_connect_req->bit_mask & ICF_STREAM_LIST_PRESENT)
        {

                p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,
                "\nMedia Profile List:"));
                p_stream_list= p_media_connect_req->p_stream_list;
                i = 1;
                while(p_stream_list!=ICF_NULL)
                {
                        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nNode %d:",i));
                        p_sprintf_count = icf_cmn_trace_stream_capab(p_stream_list->p_data,p_sprintf_count);
                        p_stream_list = p_stream_list->p_next;
                        i++;
                }
        }

    if(p_media_connect_req->bit_mask & ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Deleted stream count: %d",p_media_connect_req->count));

        for(i=0;(i<p_media_connect_req->count) && (i<ICF_MAX_STREAM_PER_CALL);i++)
        {
                        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream type: %d",
			p_media_connect_req->deleted_media_streams[i].stream_type));
                        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n stream id: %d",
			p_media_connect_req->deleted_media_streams[i].stream_id));
        }
    }
    if(ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT & p_media_connect_req->bit_mask)
	{
	     p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Media Mode: %d",p_media_connect_req->media_mode));
	}
    if(ICF_TRUE == p_media_connect_req->send_without_sdp)
	{
	    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Send Without SDP : TRUE"));
	}
	else if(ICF_TRUE == p_media_connect_req->send_without_sdp)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Send Without SDP : FALSE"));
	}

        return p_sprintf_count; 
}



/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_trace_data
 *
 * DESCRIPTION:     Prints the trace data
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/


icf_uint8_t* icf_cmn_trace_trace_data(icf_trace_data_st* p_trace_data, icf_uint8_t* p_sprintf_count)
{
    icf_uint8_t i=0;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_trace_data)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_glb_pdb is NULL.\n"))
        return p_sprintf_count;
    }       
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n TIMESTAMP : %d Seconds & %d MiliSeconds",p_trace_data->trace_timestamp.time_in_secs,
   	p_trace_data->trace_timestamp.time_in_millisecs));
     p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Integer values : %d",p_trace_data->num_of_integer_values));
    
    for(i=0; (i<p_trace_data->num_of_integer_values) && (i<ICF_MAX_INT_TRACE_DATA); i++)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n %d integer value  : %d",i,p_trace_data->int_val[i]));
    }
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of String values : %d",p_trace_data->number_of_string_values));
    for(i=0; i<p_trace_data->number_of_string_values; i++)
    {
	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n %d String value  : %s",i,p_trace_data->string_val));
    }
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Trace String  : %s",p_trace_data->trc_string));
    return p_sprintf_count;
}

#ifdef ICF_SESSION_TIMER
/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_session_timer_toggle_req
 *
 * DESCRIPTION:     Prints the session timer toggle req API
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_session_timer_toggle_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_session_timer_toggle_req_st* p_session_toggle = NULL;
    
    p_session_toggle = (icf_session_timer_toggle_req_st *)p_msg->payload;
    if (ICF_NULL == p_session_toggle)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_session_toggle is NULL.\n"))
            return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \n",p_session_toggle->bit_mask))

    if (p_session_toggle->bit_mask & ICF_SESS_TOGG_REQ_TIMER_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession timer = %d ",
                    p_session_toggle->session_timer))
    }

    if (p_session_toggle->bit_mask & ICF_SESS_TOGG_REQ_MINSE_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nmin se = %d ",
                    p_session_toggle->min_se))
    }

    if (p_session_toggle->bit_mask & ICF_SESS_TOGG_REQ_REFRESHER_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nsession refresher = %d ",
                    p_session_toggle->session_refresher))
    }
    return p_sprintf_count;

}  

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_session_timer_toggle_resp
 *
 * DESCRIPTION:     Prints the session timer toggle resp API
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_session_timer_toggle_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_session_timer_toggle_resp_st* p_session_toggle = NULL;
    
    p_session_toggle = (icf_session_timer_toggle_resp_st *)p_msg->payload;
    if (ICF_NULL == p_session_toggle)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_session_toggle is NULL.\n"))
            return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, result = %d \n",
                p_session_toggle->bit_mask, p_session_toggle->result))

    if (p_session_toggle->bit_mask & ICF_SESSION_TOGG_RESP_ERROR_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nerror cause = %d ",
                    p_session_toggle->error_cause))
    }

    return p_sprintf_count;

}
#endif
/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_req
 *
 * DESCRIPTION:     Prints the generic_msg_req API
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_req(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
	icf_generic_msg_req_st* p_generic_msg_req = NULL;

	p_generic_msg_req = (icf_generic_msg_req_st *)p_msg->payload;
	if (ICF_NULL == p_generic_msg_req)
	{
		ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_generic_msg_req is NULL.\n"))
			return p_sprintf_count;
	}        

	p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \n",p_generic_msg_req->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"method_name = %s, \n",p_generic_msg_req->method_name.str));


	/* Header LIST */
	if (p_generic_msg_req->bit_mask & ICF_HDR_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_header_list(p_generic_msg_req->header_list.hdr, p_generic_msg_req->header_list.count,p_sprintf_count);
	}    
	/* Tag LIST*/
	if (p_generic_msg_req->bit_mask & ICF_TAG_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_tag_list(p_generic_msg_req->tag_list.tag, p_generic_msg_req->tag_list.count,p_sprintf_count);
	}
	/* Body LIST */
	if (p_generic_msg_req->bit_mask & ICF_BODY_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_body_list(p_generic_msg_req->body_list.body, p_generic_msg_req->body_list.count,p_sprintf_count);
	}
	return p_sprintf_count;
}

  
/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_resp
 *
 * DESCRIPTION:     Prints the generic_msg_resp API
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{

	icf_generic_msg_resp_st* p_generic_msg_resp = NULL;

	p_generic_msg_resp = (icf_generic_msg_resp_st *)p_msg->payload;
	if (ICF_NULL == p_generic_msg_resp)
	{
		ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_generic_msg_resp is NULL.\n"))
			return p_sprintf_count;
	}        

	p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \n",p_generic_msg_resp->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"method_name = %s, \n",p_generic_msg_resp->method_name.str));

    if(p_generic_msg_resp->bit_mask & ICF_GENERIC_MSG_RESP_RESPONSE_CODE_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"response_code = %d, \n",p_generic_msg_resp->response_code));
    }

	if(p_generic_msg_resp->bit_mask & ICF_GENERIC_MSG_RESP_ERROR_CAUSE_PRESENT)
	{
		p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"error_cause = %d, \n",p_generic_msg_resp->error_cause))
	}

	/* Header LIST */
	if (p_generic_msg_resp->bit_mask & ICF_HDR_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_header_list(p_generic_msg_resp->header_list.hdr, p_generic_msg_resp->header_list.count,p_sprintf_count);
	}    
	/* Body LIST */
	if (p_generic_msg_resp->bit_mask & ICF_BODY_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_body_list(p_generic_msg_resp->body_list.body, p_generic_msg_resp->body_list.count,p_sprintf_count);
	}
	return p_sprintf_count;

}




/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_ind
 *
 * DESCRIPTION:     Prints the generic_msg_ind API
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_generic_msg_ind_st* p_generic_msg_ind = NULL;
    
    p_generic_msg_ind = (icf_generic_msg_ind_st *)p_msg->payload;
    if (ICF_NULL == p_generic_msg_ind)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_generic_msg_ind is NULL.\n"))
            return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \n",p_generic_msg_ind->bit_mask));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"method_name = %s, \n",p_generic_msg_ind->method_name.str));

	 
/* Header LIST */
    if (p_generic_msg_ind->bit_mask & ICF_HDR_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_header_list(p_generic_msg_ind->header_list.hdr, p_generic_msg_ind->header_list.count,p_sprintf_count);
    }    
/* Body LIST */
    if (p_generic_msg_ind->bit_mask & ICF_BODY_LIST_PRESENT) {
        p_sprintf_count = icf_cmn_trace_body_list(p_generic_msg_ind->body_list.body, p_generic_msg_ind->body_list.count,p_sprintf_count);
    }
	  

   return p_sprintf_count;
}

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_cfm
 *
 * DESCRIPTION:     Prints the generic_msg_cfm API
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
	icf_generic_msg_cfm_st* p_generic_msg_cfm = NULL;

	p_generic_msg_cfm = (icf_generic_msg_cfm_st *)p_msg->payload;
	if (ICF_NULL == p_generic_msg_cfm)
	{
		ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_generic_msg_cfm is NULL.\n"))
			return p_sprintf_count;
	}        

	p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \n",p_generic_msg_cfm->bit_mask));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"method_name = %s, \n",p_generic_msg_cfm->method_name.str));

	p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"response_code = %d, \n",p_generic_msg_cfm->response_code));
	/* Header LIST*/
	if (p_generic_msg_cfm->bit_mask & ICF_HDR_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_header_list(p_generic_msg_cfm->header_list.hdr, p_generic_msg_cfm->header_list.count,p_sprintf_count);
	}    
	/* Tag LIST */

	if (p_generic_msg_cfm->bit_mask & ICF_TAG_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_tag_list(p_generic_msg_cfm->tag_list.tag, p_generic_msg_cfm->tag_list.count,p_sprintf_count);
	}
	/* Body LIST */
	if (p_generic_msg_cfm->bit_mask & ICF_BODY_LIST_PRESENT) {
		p_sprintf_count = icf_cmn_trace_body_list(p_generic_msg_cfm->body_list.body, p_generic_msg_cfm->body_list.count,p_sprintf_count);
	}
	return p_sprintf_count;
}

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_req_recv_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_REQ_RECV_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_req_recv_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
    icf_prack_req_recv_ind_st *p_prack_req_recv_ind = NULL;

    icf_uint8_t header_list_count = (icf_uint8_t) 0,
         body_list_count = (icf_uint8_t) 0;
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_prack_req_recv_ind = (icf_prack_req_recv_ind_st*)p_msg->payload;
    p_sprintf_count += 
         ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %u",
              p_prack_req_recv_ind ->bit_mask));
    p_sprintf_count +=
        ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSettings  = %u",
              p_prack_req_recv_ind ->settings));
    if (p_prack_req_recv_ind ->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        header_list_count = 
           (icf_uint8_t) p_prack_req_recv_ind ->header_list.count;
        p_sprintf_count = 
              icf_cmn_trace_header_list(
                 p_prack_req_recv_ind ->header_list.hdr,
                 header_list_count,p_sprintf_count);
    }
    /* Body LIST */
    if (p_prack_req_recv_ind ->bit_mask & ICF_BODY_LIST_PRESENT) 
    {
        body_list_count = 
            (icf_uint8_t) p_prack_req_recv_ind ->body_list.count;
        p_sprintf_count = 
           icf_cmn_trace_body_list(
                    p_prack_req_recv_ind ->body_list.body, 
                    body_list_count,p_sprintf_count);
    }
    return p_sprintf_count;
}

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_resp_sent_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_RESP_SENT_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_resp_sent_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
    icf_prack_resp_sent_ind_st *p_prack_resp_sent_ind = ICF_NULL;

    icf_uint8_t header_list_count = (icf_uint8_t) 0;
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_prack_resp_sent_ind = (icf_prack_resp_sent_ind_st*)p_msg->payload;
    p_sprintf_count += 
         ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %u",
              p_prack_resp_sent_ind->bit_mask));
    p_sprintf_count +=
        ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSettings  = %u",
              p_prack_resp_sent_ind ->settings));
    p_sprintf_count +=
        ICF_SPRINTF (((icf_int8_t *)p_sprintf_count,"\nResponse code  = %u",
              p_prack_resp_sent_ind ->response_code));         

    if (p_prack_resp_sent_ind ->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        header_list_count = 
           (icf_uint8_t) p_prack_resp_sent_ind ->header_list.count;
        p_sprintf_count = 
              icf_cmn_trace_header_list(
                 p_prack_resp_sent_ind ->header_list.hdr,
                 header_list_count,p_sprintf_count);
    }
    return p_sprintf_count;
}


/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_req_sent_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_REQ_SENT_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_req_sent_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
    icf_prack_req_sent_ind_st *p_prack_req_sent_ind = ICF_NULL;

    icf_uint8_t header_list_count = (icf_uint8_t) 0;

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_prack_req_sent_ind = (icf_prack_req_sent_ind_st*)p_msg->payload;
    p_sprintf_count += 
         ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %u",
              p_prack_req_sent_ind->bit_mask));

    if (p_prack_req_sent_ind ->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        header_list_count = 
           (icf_uint8_t) p_prack_req_sent_ind ->header_list.count;
        p_sprintf_count = 
              icf_cmn_trace_header_list(
                 p_prack_req_sent_ind ->header_list.hdr,
                 header_list_count,p_sprintf_count);
    }
    return p_sprintf_count;    
}

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_resp_recv_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_RESP_RECV_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_resp_recv_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)
{
    icf_prack_resp_recv_ind_st *p_prack_resp_recv_ind = ICF_NULL;

    icf_uint8_t header_list_count = (icf_uint8_t) 0,
         body_list_count = (icf_uint8_t) 0;
    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);
    p_prack_resp_recv_ind = (icf_prack_resp_recv_ind_st*)p_msg->payload;
    p_sprintf_count += 
         ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nbit_mask = %u",
              p_prack_resp_recv_ind->bit_mask));
    p_sprintf_count +=
        ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\nSettings  = %u",
              p_prack_resp_recv_ind ->settings));
    p_sprintf_count +=
        ICF_SPRINTF (((icf_int8_t *)p_sprintf_count,"\nResponse code  = %u",
              p_prack_resp_recv_ind ->response_code));         

    if (p_prack_resp_recv_ind ->bit_mask & ICF_HDR_LIST_PRESENT)
    {
        header_list_count = 
           (icf_uint8_t) p_prack_resp_recv_ind ->header_list.count;
        p_sprintf_count = 
              icf_cmn_trace_header_list(
                 p_prack_resp_recv_ind ->header_list.hdr,
                 header_list_count,p_sprintf_count);
    }
    /* Body LIST */
    if (p_prack_resp_recv_ind ->bit_mask & ICF_BODY_LIST_PRESENT) 
    {
        body_list_count = 
            (icf_uint8_t) p_prack_resp_recv_ind ->body_list.count;
        p_sprintf_count = 
           icf_cmn_trace_body_list(
                    p_prack_resp_recv_ind ->body_list.body, 
                    body_list_count,p_sprintf_count);
    }
    return p_sprintf_count;
}

#ifdef ICF_ERROR_ENABLE
/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_error_data
 *
 * DESCRIPTION:     Prints the error data
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/


icf_uint8_t* icf_cmn_trace_error_data(icf_error_data_st* p_error_data, icf_uint8_t* p_sprintf_count)
{
    icf_uint8_t i=0;
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_error_data)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_error_data is NULL.\n"))
        return p_sprintf_count;
    }        

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Integer values : %d",p_error_data->num_of_integer_values));

    for(i=0; (i<p_error_data->num_of_integer_values) && (i<ICF_MAX_INT_TRACE_DATA); i++)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n %d integer value  : %d",i,p_error_data->int_val[i]));
    }
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of String values : %d",p_error_data->number_of_string_values));
    for(i=0; i<p_error_data->number_of_string_values; i++)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n %d String value  : %s",i,p_error_data->string_val));
    }
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Trace String  : %s",p_error_data->err_string));
    return p_sprintf_count;
}

#endif


#ifdef ICF_STATS_ENABLE
/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_stats_data
 *
 * DESCRIPTION:     Prints the stats data
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_stats_data(icf_stats_st* p_stats, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_stats)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_stats is NULL.\n"))
        return p_sprintf_count;
    }  
    p_sprintf_count = icf_cmn_trace_tcp_connections_stats(&p_stats->stat_tcp_connections,p_sprintf_count);
    p_sprintf_count = icf_cmn_trace_udp_connections_stats(&p_stats->stat_udp_connections,p_sprintf_count);
    p_sprintf_count = icf_cmn_trace_tls_connections_stats(&p_stats->stat_tls_connections,p_sprintf_count);
    p_sprintf_count = icf_cmn_trace_register_stats(&p_stats->stat_tls_connections,p_sprintf_count);
    p_sprintf_count = icf_cmn_trace_request_msg_stats(&p_stats->stat_tls_connections,p_sprintf_count);
    p_sprintf_count = icf_cmn_trace_resp_msg_stats(&p_stats->stat_tls_connections,p_sprintf_count);
    p_sprintf_count = icf_cmn_trace_misc_stats(&p_stats->stat_tls_connections,p_sprintf_count);
  
    return p_sprintf_count;

}


/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_tcp_connections_stats
 *
 * DESCRIPTION:     Prints the tcp connections stats
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_tcp_connections_stats(icf_tcp_connections_stats_st* p_tcp_con, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_tcp_con)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_tcp_con is NULL.\n"))
        return p_sprintf_count;
    }      
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n TCP CONNECTION Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Active TCP Connections : %d",p_tcp_con->stats_active_conn));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Total TCP connections active or closed after last reboot : %d",
    	p_tcp_con->total_tcp_conn_since_last_reboot));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Total number of failures : %d",p_tcp_con->stats_send_failures));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Connection Failures : %d",p_tcp_con->stats_conn_failures));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Number of TCP connections created by local and closed by remote : %d",
	p_tcp_con->stats_remote_closure_for_local_conn));
    
    return p_sprintf_count;
}


/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_udp_connections_stats
 *
 * DESCRIPTION:     Prints the udp connections stats
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_udp_connections_stats(icf_udp_connections_stats_st* p_udp_con, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_udp_con)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_udp_con is NULL.\n"))
        return p_sprintf_count;
    }      
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n UDP CONNECTION Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Total number of failures : %d",p_udp_con->stats_send_failures));
    
    return p_sprintf_count;
}



/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_tls_connections_stats
 *
 * DESCRIPTION:     Prints the tls connections stats
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_tls_connections_stats(icf_tls_connections_stats_st* p_tls_con, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_tls_con)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_tls_con is NULL.\n"))
        return p_sprintf_count;
    }       
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n TLS CONNECTION Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Active TLS Connections : %d",p_tls_con->stats_active_conn));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Total TLS connections active or closed after last reboot : %d",
        p_tls_con->total_tls_conn_since_last_reboot));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Total number of failures : %d",p_tls_con->stats_send_failures));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Connection Failures : %d",p_tls_con->stats_conn_failures));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Number of TLS connections created by local and closed by remote : %d",
        p_tls_con->stats_remote_closure_for_local_conn));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n STATS Handshake Error: %d",p_tls_con->stats_handshake_errors));
    return p_sprintf_count;
}



/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_register_stats
 *
 * DESCRIPTION:     Prints the  register stats
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_register_stats(icf_register_stats_st* p_reg_stat, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_reg_stat)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_reg_stat is NULL.\n"))
        return p_sprintf_count;
    }          
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n REGISTRATION Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Active Registration : %d",p_reg_stat->stats_active_reg));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Failed Registrations: %d",
        p_reg_stat->stats_failed_reg));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of DeRegistrations Done : %d",p_reg_stat->stats_dereg_done));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Users Registered : %d",p_reg_stat->stats_user_registered));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Registrar Addresses: %d",
        p_reg_stat->stats_registrar_addr));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Regter Retransmissions: %d",p_reg_stat->stats_rereg_done));
    /**************ICF 8.0*******************/
    /*Printing register stat for ,number of reg event subscription done
      corresponding to successful registration and for number of network 
      initiated deregistration done */
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Subscription for \
                Reg-Event package: %d",p_reg_stat->stats_reg_event_package_subs));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Network \
                Initiated Deregistration : %d",p_reg_stat->stats_nw_dereg_done));

    return p_sprintf_count;
}


/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_request_msg_stats
 *
 * DESCRIPTION:     Prints the  req mesg stats
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_request_msg_stats(icf_request_message_stats_st* p_req_stat, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_req_stat)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_req_stat is NULL.\n"))
        return p_sprintf_count;
    }          
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Request Message Stats \n OUTBOUND Request Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of INVITE Sent: %d",p_req_stat->stat_outbound_req.stats_INVITE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of PRACK Sent: %d",
        p_req_stat->stat_outbound_req.stats_PRACK));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of UPDATE Sent : %d",p_req_stat->stat_outbound_req.stats_UPDATE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of PUBLISH Sent : %d",p_req_stat->stat_outbound_req.stats_PUBLISH));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of MESSAGE Sent : %d",p_req_stat->stat_outbound_req.stats_MESSAGE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of REINVITE Sent: %d",p_req_stat->stat_outbound_req.stats_ReINVITE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of INFO Sent: %d",
        p_req_stat->stat_outbound_req.stats_INFO));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Out Of Dialog OPTIONS Sent: %d",p_req_stat->stat_outbound_req.stats_outdlg_OPTION));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of IN Dialog OPTIONS Sent: %d",p_req_stat->stat_outbound_req.stats_indlg_OPTION));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Out Of Cancel Sent: %d",p_req_stat->stat_outbound_req.stats_CANCEL));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Out Of ACK Sent: %d",p_req_stat->stat_outbound_req.stats_ACK));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of BYE Sent: %d",p_req_stat->stat_outbound_req.stats_BYE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of REGISTER Sent: %d",p_req_stat->stat_outbound_req.stats_REGISTER));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of SUBSCRIBE Sent: %d",p_req_stat->stat_outbound_req.stats_SUBSCRIBE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of REFER Sent: %d",p_req_stat->stat_outbound_req.stats_REFER));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of NOTIFY Sent: %d",p_req_stat->stat_outbound_req.stats_NOTIFY;));
  p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of QOS INVITE Sent \
                %d",p_req_stat->stat_outbound_req.stats_qos_INVITE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n INBOUND Request Stats ")); 
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of INVITE Received: %d",p_req_stat->stat_inbound_req.stats_INVITE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of PRACK Received: %d",
    p_req_stat->stat_inbound_req.stats_PRACK));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of UPDATE Received : %d",p_req_stat->stat_inbound_req.stats_UPDATE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of MESSAGE Received : %d",p_req_stat->stat_inbound_req.stats_MESSAGE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of REINVITE Received: %d",p_req_stat->stat_inbound_req.stats_ReINVITE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of INFO Received: %d",
        p_req_stat->stat_inbound_req.stats_INFO));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Out Of Dialog OPTIONS Received: %d",
   	p_req_stat->stat_inbound_req.stats_outdlg_OPTION));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of IN Dialog OPTIONS Received: %d",
	p_req_stat->stat_inbound_req.stats_indlg_OPTION));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Cancel Received: %d",p_req_stat->stat_inbound_req.stats_CANCEL));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of ACK Received: %d",p_req_stat->stat_inbound_req.stats_ACK));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of BYE Received: %d",p_req_stat->stat_inbound_req.stats_BYE));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of REFER Received: %d",p_req_stat->stat_inbound_req.stats_REFER));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of NOTIFY Received: %d",p_req_stat->stat_inbound_req.stats_NOTIFY;));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of QOS INVITE \
                Received: %d",p_req_stat->stat_inbound_req.stats_qos_INVITE));


    return p_sprintf_count;

}



/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_resp_msg_stats
 *
 * DESCRIPTION:     Prints the  resp mesg stats
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_resp_msg_stats(icf_respone_message_stats_st* p_resp_stat, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_resp_stat)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_resp_stat is NULL.\n"))
        return p_sprintf_count;
    }      
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n Response Message Stats \n OUTBOUND INVITE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 100 response of outbound INVITE: %d",
	p_resp_stat->stat_outbound_invite_resp.stats_100));
   p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 2xx response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_2xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 180 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_180));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 183 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_183));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 3xx response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_3xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 486 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_486));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 404 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_404));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 415 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_415));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 100 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_100));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 407 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_407));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 401 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_401));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 491 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_491));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 503 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_503));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 488 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_488));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 480 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_480));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 403 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_403));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 487 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_487));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_400));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 420 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_420));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 423 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_423));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 484 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_484));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 485 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_485));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 501 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_501));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 502 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_502));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 504 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_504));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 505 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_505));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 580 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_580));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 600 response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_600));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of remaining 4xx response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_rest_4xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of remaining 5xx response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_rest_5xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of remaining 6xx response of outbound INVITE: %d",
        p_resp_stat->stat_outbound_invite_resp.stats_rest_6xx));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND CANCEL Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound CANCEL: %d",
        p_resp_stat->stat_outbound_cancel_resp.stats_200));
     p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound CANCEL: %d",
        p_resp_stat->stat_outbound_cancel_resp.stats_other_resp));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND PRACK Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound PRACK: %d",
        p_resp_stat->stat_outbound_prack_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound PRACK: %d",
        p_resp_stat->stat_outbound_prack_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND REFER Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 202 response of outbound REFER: %d",
        p_resp_stat->stat_outbound_refer_resp.stats_202));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of outbound REFER: %d",
        p_resp_stat->stat_outbound_refer_resp.stats_400));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of outbound REFER: %d",
        p_resp_stat->stat_outbound_refer_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other response of outbound REFER: %d",
        p_resp_stat->stat_outbound_refer_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND SUBSCRIBE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound SUBSCRIBE: %d",
        p_resp_stat->stat_outbound_subscribe_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 401 response of outbound SUBSCRIBE: %d",
        p_resp_stat->stat_outbound_subscribe_resp.stats_401));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 407 response of outbound SUBSCRIBE: %d",
        p_resp_stat->stat_outbound_subscribe_resp.stats_407));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 489 response of outbound SUBSCRIBE: %d",
        p_resp_stat->stat_outbound_subscribe_resp.stats_489));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other response of outbound SUBSCRIBE: %d",
        p_resp_stat->stat_outbound_subscribe_resp.stats_other_resp));
 
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND INFO Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound INFO: %d",
        p_resp_stat->stat_outbound_info_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound INFO: %d",
        p_resp_stat->stat_outbound_info_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND BYE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound BYE: %d",
        p_resp_stat->stat_outbound_bye_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound BYE: %d",
        p_resp_stat->stat_outbound_bye_resp.stats_other_resp));
    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND NOTIFY Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound NOTIFY: %d",
        p_resp_stat->stat_outbound_notify_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound NOTIFY: %d",
        p_resp_stat->stat_outbound_notify_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND UPDATE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound UPDATE: %d",
        p_resp_stat->stat_outbound_update_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound UPDATE: %d",
        p_resp_stat->stat_outbound_update_resp.stats_other_resp));
   
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND PUBLISH Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound PUBLISH: %d",
        p_resp_stat->stat_outbound_publish_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound PUBLISH: %d",
        p_resp_stat->stat_outbound_publish_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND MESSAGE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound MESSAGE: %d",
        p_resp_stat->stat_outbound_message_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound MESSAGE: %d",
        p_resp_stat->stat_outbound_message_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND REGISTER Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound REGISTER: %d",
        p_resp_stat->stat_outbound_register_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 401 response of outbound REGISTER: %d",
        p_resp_stat->stat_outbound_register_resp.stats_401));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 407 response of outbound REGISTER: %d",
        p_resp_stat->stat_outbound_register_resp.stats_407));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 489 response of outbound REGISTER: %d",
        p_resp_stat->stat_outbound_register_resp.stats_489));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other response of outbound REGISTER: %d",
        p_resp_stat->stat_outbound_register_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n OUTBOUND OPTIONS Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of outbound OPTIONS: %d",
        p_resp_stat->stat_outbound_options_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of other(except 200) response of outbound OPTIONS: %d",
        p_resp_stat->stat_outbound_options_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND INVITE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 100 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_100));
   p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 2xx response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_2xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 180 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_180));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 183 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_183));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 3xx response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_3xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 486 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_486));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 404 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_404));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 415 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_415));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 100 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_100));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 407 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_407));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 401 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_401));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 491 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_491));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 503 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_503));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 488 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_488));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 480 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_480));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 403 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_403));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 487 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_487));
/*    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_400));*/
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 420 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_420));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 423 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_423));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 484 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_484));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 485 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_485));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 501 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_501));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 502 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_502));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 504 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_504));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 505 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_505));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 580 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_580));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 600 response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_600));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of remaining 4xx response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_rest_4xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of remaining 5xx response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_rest_5xx));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of remaining 6xx response of inbound INVITE: %d",
        p_resp_stat->stat_inbound_invite_resp.stats_rest_6xx));

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND CANCEL Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound CANCEL: %d",
        p_resp_stat->stat_inbound_cancel_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound CANCEL: %d",
        p_resp_stat->stat_inbound_cancel_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound CANCEL: %d",
        p_resp_stat->stat_inbound_cancel_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound CANCEL: %d",
        p_resp_stat->stat_inbound_cancel_resp.stats_400));
    

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND PRACK Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound PRACK: %d",
        p_resp_stat->stat_inbound_prack_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound PRACK: %d",
        p_resp_stat->stat_inbound_prack_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound PRACK: %d",
        p_resp_stat->stat_inbound_prack_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound PRACK: %d",
        p_resp_stat->stat_inbound_prack_resp.stats_400));
 
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND REFER Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 202 response of inbound REFER: %d",
        p_resp_stat->stat_inbound_refer_resp.stats_202));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound REFER: %d",
        p_resp_stat->stat_inbound_refer_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound REFER: %d",
        p_resp_stat->stat_inbound_refer_resp.stats_400));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 403 response of inbound REFER: %d",
        p_resp_stat->stat_inbound_refer_resp.stats_403));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of failure response of inbound REFER: %d",
        p_resp_stat->stat_inbound_refer_resp.stats_failure_resp));

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND NOTIFY Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound NOTIFY: %d",
        p_resp_stat->stat_inbound_notify_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound NOTIFY: %d",
        p_resp_stat->stat_inbound_notify_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound NOTIFY: %d",
        p_resp_stat->stat_inbound_notify_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound NOTIFY: %d",
        p_resp_stat->stat_inbound_notify_resp.stats_400));

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND INFO Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound INFO: %d",
        p_resp_stat->stat_inbound_info_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound INFO: %d",
        p_resp_stat->stat_inbound_info_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound INFO: %d",
        p_resp_stat->stat_inbound_info_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound INFO: %d",
        p_resp_stat->stat_inbound_info_resp.stats_400));

    
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND BYE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound BYE: %d",
        p_resp_stat->stat_inbound_bye_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound BYE: %d",
        p_resp_stat->stat_inbound_bye_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound BYE: %d",
        p_resp_stat->stat_inbound_bye_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound BYE: %d",
        p_resp_stat->stat_inbound_bye_resp.stats_400));


    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND UPDATE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound UPDATE: %d",
        p_resp_stat->stat_inbound_update_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound UPDATE: %d",
        p_resp_stat->stat_inbound_update_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound UPDATE: %d",
        p_resp_stat->stat_inbound_update_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound UPDATE: %d",
        p_resp_stat->stat_inbound_update_resp.stats_400));
	
   

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n INBOUND OPTIONS Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound OPTIONS: %d",
        p_resp_stat->stat_inbound_options_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound OPTIONS: %d",
        p_resp_stat->stat_inbound_options_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of Other response of inbound OPTIONS: %d",
        p_resp_stat->stat_inbound_options_resp.stats_other_resp));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n\n INBOUND MESSAGE Response Stats "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of 200 response of inbound MESSAGE: %d",
        p_resp_stat->stat_inbound_message_resp.stats_200));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of 500 response of inbound MESSAGE: %d",
        p_resp_stat->stat_inbound_message_resp.stats_500));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of 481 response of inbound MESSAGE: %d",
        p_resp_stat->stat_inbound_message_resp.stats_481));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)(icf_int8_t *)p_sprintf_count,"\n No of 400 response of inbound MESSAGE: %d",
        p_resp_stat->stat_inbound_message_resp.stats_400));


    return p_sprintf_count;

}



/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_misc_stats
 *
 * DESCRIPTION:     Prints the  misc  stats
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/

icf_uint8_t* icf_cmn_trace_misc_stats(icf_misc_stats_st* p_misc_stat, icf_uint8_t* p_sprintf_count)
{
    /*Fix For SPR : 18999*/
    if (ICF_NULL == p_misc_stat)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_misc_stat is NULL.\n"))
        return p_sprintf_count;
    }      
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n\n MISC Traces "));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of AAA DNS Query: %d",
        p_misc_stat->stats_AAA_DNS_QUERY));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of SRV DNS Query: %d",
        p_misc_stat->stats_SRV_DNS_QUERY));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of AAA DNS Query Success Response: %d",
        p_misc_stat->stats_AAA_DNS_QUERY_SUCCESS_RESP));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of SRV DNS Query Success Response: %d",
        p_misc_stat->stats_SRV_DNS_QUERY_SUCCESS_RESP));
    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of AAA DNS Query Failure Response: %d",
        p_misc_stat->stats_AAA_DNS_QUERY_FAILURE_RESP));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"\n No of times SRTP fallback invoked: %d",
        p_misc_stat->stats_srtp_fallback_done));


    return p_sprintf_count;
}

 

#endif

/********* Traces Add End as per SPR 17213 **********/

#endif  /* end of ICF_TRACE_ENABLE */

#ifdef ICF_TRACE_ENABLE

/**********************************************************************
 * FUNCTION:    icf_cmn_check_app_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              to check whether the trace criteria and the type requested
 *              is enabled and is to be reported or not.
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received trace_id
 *              can be reported.
 *              ICF_FAILURE to indicate:
 *                  -Received trace_id is INAVLID
 *                  -DBM failure to obtain SM Global data.
 *                  -Received trace_id cannot be reported.
 *
 *
 **********************************************************************/
 icf_return_t icf_cmn_check_app_trace(
         INOUT    icf_glb_pdb_st *p_glb_pdb,
         IN       icf_trace_id_t trace_id,
         IN  icf_trace_criteria_data_st  *p_criteria_data)
 {

#ifdef ICF_TRACE_ENABLE
      icf_return_t ret_val = ICF_SUCCESS;

     /* Variables used to store the 2 categories of traces
      * to compare: recd_ and global_. */
     icf_trace_type_t recd_trace_type =  ICF_NULL;
     icf_trace_type_t glb_trace_type =  ICF_NULL;

     icf_boolean_t glb_trace_status =  ICF_FALSE;
     icf_sm_glb_data_st *p_sm_data =  ICF_NULL;
     /*Fix For SPR : 18999*/
     if (ICF_NULL == p_glb_pdb)
     {
         ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_glb_pdb is NULL.\n"))
         return ICF_FAILURE;
     }      
     /*Checking for Received trace_id's range.Processing begins*/
     if (ICF_MAX_APP_TRACE_IDS >= trace_id)
     {
         /* Received trace_id is within range.Processing begins*/
          ret_val = icf_sm_get_module_glb_data(p_glb_pdb,
                  (icf_module_id_t)(ICF_INT_MODULE_SM),
                  (icf_void_t *)&p_sm_data);
          if (ICF_FAILURE !=  ret_val)
          {
            /* Successul retrieval of SM data: processing begins. */
            glb_trace_status = p_sm_data->trace_status;
            if (ICF_FALSE == glb_trace_status)
            {
                ret_val = ICF_FAILURE;
            }
           else
            {
              /*Fix For SPR : 18999*/  
              ICF_CHECK_IF_VAR_IS_NULL_THEN_RETURN(p_glb_pdb,p_glb_pdb->
                                                   p_glb_data)  
              /* Processing of trace_status=TRUE begins. */
               if (trace_id < ICF_MAX_APP_TRACE_IDS) recd_trace_type = p_glb_pdb->p_glb_data->
                   app_trace_data[trace_id].trace_type;
               glb_trace_type = p_sm_data->trace_type;
               if(recd_trace_type & glb_trace_type)
               {
                 if(ICF_TRACE_CALL_CONNECTED != trace_id)
                 {
                   ret_val = icf_port_trace_match_criteria_value(
                               recd_trace_type,
                               p_sm_data->trace_criteria,
                               p_sm_data->trace_rule,
                               p_criteria_data);
                 }
                else
                 {
                    ret_val = ICF_SUCCESS;
                 }
               }
              else
               {
                    ret_val = ICF_FAILURE;
               }
            }/* Processing of trace_status=TRUE ends. */
          }/* DBM successul with SM data: processing ends. */
     }/* received trace_id was within valid range: processing ends. */
     else
     {
         ret_val = ICF_FAILURE;
     }/* Received trace_id found to be INVALID.*/
      return ret_val;
#else
    icf_return_t ret_val  =  ICF_SUCCESS;
    p_glb_pdb = p_glb_pdb;
    trace_id = trace_id;
     return ret_val;
#endif     

}/* Function icf_cmn_check_app_trace() ends here. */


/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_message_cfm
 *
 * DESCRIPTION:     Prints the message_cfm API
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_message_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count)

{
    icf_message_cfm_st* p_message_cfm = ICF_NULL;

    p_message_cfm = (icf_message_cfm_st *)p_msg->payload;
    if (ICF_NULL == p_message_cfm)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[CMN] : p_message_cfm is NULL.\n"))
            return p_sprintf_count;
    }        

    p_sprintf_count = icf_cmn_trace_api_header(p_msg,p_sprintf_count);

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"bit_mask = %d, \n",p_message_cfm->bit_mask));

    p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"result = %d, \n",p_message_cfm->result));

    if (p_message_cfm->bit_mask & ICF_APP_CONFIRMATION_RESP_CODE_PRESENT)
    {
        p_sprintf_count += ICF_SPRINTF(((icf_int8_t *)p_sprintf_count,"response_code = %d, \n",p_message_cfm->response_code));
    }

    /* Header LIST*/
    if (p_message_cfm->bit_mask & ICF_HDR_LIST_PRESENT) 
    {
        p_sprintf_count = icf_cmn_trace_header_list(p_message_cfm->header_list.hdr, p_message_cfm->header_list.count,p_sprintf_count);
    }    

    /* Tag LIST */
    if (p_message_cfm->bit_mask & ICF_TAG_LIST_PRESENT)
    {
        p_sprintf_count = icf_cmn_trace_tag_list(p_message_cfm->tag_list.tag, p_message_cfm->tag_list.count,p_sprintf_count);
    }

    return p_sprintf_count;

}

#endif
