/******************************************************************************
 * 
 * 
 * FILE NAME:   icf_mmi_defs.h 
 * 
 * DESCRIPTION: This file contains the hash defines  used to implement the 
 *              functionality of MMI.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 *          Shefali Aggarwal     26-nov-2004        Initial
 *          Kirti Dhingra        29-nov-2005		Added States for QOS Support
 *
 *          Deepti Goyal         30-Nov-2006        NAT Feature Support
 *          Tarun Gupta          06-Feb-2009        Rel 8.1 SDP Tunnelling Enhancements
 * Copyright 2009, Aricent.                                  *
 * ****************************************************************************/


/*------------------------------------------------------------------------------
 * 
 * Hash defines for the states of MMI.
 * 
 * --------------------------------------------------------------------------*/

#define ICF_MMI_00_IDLE 0
#define ICF_MMI_01_AWT_GCL_RESP 1
#define ICF_MMI_02_AWT_CMS_RESP 2 
#define ICF_MMI_03_AWT_COMMIT_RESP 3
#define ICF_MMI_04_AWT_MMS_RESP 4
#define ICF_MMI_05_AWT_DMS_RESP 5
#define ICF_MMI_06_AWT_OPEN_CHANNEL_RESP 6
#define ICF_MMI_07_AWT_MERGE_RESP 7
#define ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP 8
#define ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP 9
/* The following two new states are introduced in IPTK's tunnel mode of
   operation. By that we mean that the application does not possess local media
   processing capabilities. The 4 new APIs introduced are
   1. ICF_TUNNELLED_CREATE_MEDIA_SESSION_REQ
   2. ICF_TUNNELLED_CREATE_MEDIA_SESSION_RESP
   3. ICF_TUNNELLED_MODIFY_MEDIA_SESSION_REQ
   4. ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP.
   These APIs are used to exchange icf_tunnelled_sdp_info_st with the application.
   The icf_tunnelled_sdp_info_st contains local and remote media sesion paramters (SDP).
*/   
#define ICF_MMI_11_AWT_TUNNELLED_CMS_RESP 10
#define ICF_MMI_12_AWT_TUNNELLED_MMS_RESP 11
#define ICF_MMI_MAX_STATES 12

/*------------------------------------------------------------------------------
 * 
 * Hash defines for the events of MMI.
 * 
 * --------------------------------------------------------------------------*/
#define ICF_MMI_CMS_REQ				0
#define ICF_MMI_MMS_REQ				1
#define ICF_MMI_DMS_REQ				2
#define ICF_MMI_MERGE_REQ			3
#define ICF_MMI_GCL_RESP			4
#define ICF_MMI_COMMIT_RESP			5
#define ICF_MMI_OPEN_CHANNEL_RESP	6
#define ICF_MMI_CMS_RESP			7
#define ICF_MMI_MMS_RESP			8
#define ICF_MMI_DMS_RESP			9
#define ICF_MMI_DMS_IND				10
#define ICF_MMI_MERGE_RESP			11
#define ICF_MMI_CALL_CLEAR			12
#define ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP 13

#define ICF_MMI_NM_SEC_ACT_RESP		14
#define ICF_MMI_NM_SEC_DEACT_RESP	15
#define ICF_MMI_NM_SEC_DEACT_IND	16
#define ICF_MMI_NM_SEC_MODIFY_IND	17
#define ICF_MMI_NM_QOS_RELEASE_IND	18
#define ICF_MMI_NM_QOS_MODIFY_IND	19
#define ICF_MMI_NM_QOS_RESERVE_RESP	20
/* The following 5 new events are introduced in IPTK's tunnel mode of
   operation. By that we mean that the application does not possess local media
   processing capabilities.
*/   
#define ICF_MMI_TUNNELLED_CMS_REQ   21
#define ICF_MMI_TUNNELLED_MMS_REQ   22
#define ICF_MMI_TUNNELLED_CMS_RESP   23
#define ICF_MMI_TUNNELLED_MMS_RESP   24
#define ICF_MMI_TUNNELLED_DMS_REQ    25
#define ICF_MMI_MAX_EVENTS			 26

/* Traces */
/*#define ICF_EVENT_RECVD 101
#define ICF_TRACE_API_SEND 102
#define ICF_TRACE_MMS_FOR_CODEC_CHANGE 103
#define ICF_TRACE_MEDIA_COMMITED 104
#define ICF_TRACE_MEDIA_NOT_COMMITED 105
#define ICF_TRACE_DMS_NOT_FORCEFUL 106
#define ICF_TRACE_FORCEFUL_DMS 107
#define ICF_TRACE_FAILURE_RESP 108
#define ICF_TRACE_SUCCESS_RESP 109
#define ICF_TRACE_CC_AWT_DMS 110
#define ICF_TRACE_MEDIA_ESTABLISHED 111
#define ICF_TRACE_MEDIA_NOT_ESTABLISHED 112
#define ICF_TRACE_MODIFY_FOR_CODEC_CHANGE_SET 113
#define ICF_TRACE_RESTORATION_IN_PROGRESS_SET 114
#define ICF_TRACE_MMS_FOR_MODE_CHANGE 115
#define ICF_TRACE_MMS_WITH_REMOTE_SDP 116
#define ICF_TRACE_MODIFY_FOR_REMOTE_MEDIA_APPLICATION_SET 117
#define ICF_TRACE_CALL_INCOMING_FS 118
#define ICF_TRACE_FAILURE_TO_SEND 119
#define ICF_TRACE_FAILURE_BY_CM 120
#define ICF_TRACE_FAILURE_TO_PROCESS_MSG 121
#define ICF_TRACE_FAILURE_BY_DBM 122
#define ICF_TRACE_UNEXPECTED_EVENT 123
#define ICF_MMI_TRACE_CURRENT_STATE 124*/

/* Temperory for compilation */

#define ICF_CC_CREATE_MEDIA_SESSION_LOCAL_AUDIO_CODEC_LIST 4
#define ICF_ECODE_MERGE_RESP_FAILURE 4


/* constants for MM initialisation */
#define ICF_DEFAULT_AMR_MODE			7
#define	ICF_DEFAULT_CODEC_MODE			1
#define ICF_DEFAULT_CRC_ENABLED			1
#define ICF_DEFAULT_APP_RTCP_PORT_MUX	1
#define ICF_DEFAULT_PKT_PERIOD			200
#define ICF_INIT_MAX_NO_OF_RINGER_TONES	4
#define ICF_DEFAULT_RTP_START_PORT		1024
#define ICF_DEFAULT_RTP_END_PORT		1034
#define ICF_DEFAULT_APP_DATA_START_PORT		3024
#define ICF_DEFAULT_APP_DATA_END_PORT		3034

#ifdef ICF_NAT_MEDIA_TRAVERSAL
#define ICF_MEDIA_REQ_TYPE_STREAM  0
#define ICF_MEDIA_REQ_TYPE_RTCP 1
#define ICF_NAT_MEDIA_GET_PUBLIC_IP_PORT_REQ    200
#endif


