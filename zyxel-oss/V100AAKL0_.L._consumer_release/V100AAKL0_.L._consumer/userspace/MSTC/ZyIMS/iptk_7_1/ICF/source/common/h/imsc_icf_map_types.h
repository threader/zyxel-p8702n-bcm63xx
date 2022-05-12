/*------------------------------------------------------------------------------
 *
 * File name        : imsc_icf_map_types.h
 *
 * Purpose          : This file contains the mapping of ICF and IMSC types.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 11-Aug-2005  Kirti/Rohit						Initial creation
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/



#ifndef __IMSC_ICF_MAP_TYPES_H__
#define __IMSC_ICF_MAP_TYPES_H__


#include "imsc_extif_cc_mm_api.h"
#include "imsc_extif_cc_rm_api.h"



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



/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_sdp_st
 *
 * Description: This data type is used to define a structure, which contains the
 *              media session parameters (SDP).
 *
 *---------------------------------------------------------------------------*/
#define ICF_SDP_TX_DTMF_PT IMSC_SDP_TX_DTMF_PT

typedef imsc_sdp_st icf_sdp_st;


 /*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_mode_t
 *
 * Description: This is used to define a data type whose value specifies 
 * 	          the direction of an RTP session
 * 
 *---------------------------------------------------------------------------*/

/* typedef imsc_mode_t icf_mode_t; */



/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_qos_mark_t
 * Description:  This is used to define a datatype, whose value indicates 
 *               the type of service indicator in the RTP payload for the call. 
 * 
 * Values :      If it is set to IMSC_QOS_MARK_NONE, it indicates that the QoS 
 *               is not to be set in the RTP payload. 
 *               
 *               If it is set to IMSC_QOS_MARK_TOS_AUDIO, it indicates that it 
 *               is an audio call in the Type of Service (ToS) parameter of the
 *               RTP payload. 
 *
 *----------------------------------------------------------------------------*/

#define ICF_QOS_MARK_NONE			IMSC_QOS_MARK_NONE
#define ICF_QOS_MARK_TOS_AUDIO		IMSC_QOS_MARK_AUDIO

/* typedef imsc_qos_mark_t				icf_qos_mark_t; */

#ifdef ICF_QOS_SUPPORT
/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_qos_st
 *
 * Description: This data type is used to define a structure, which contains the
 *              Qos parameters for a particular direction.
 *
 *---------------------------------------------------------------------------*/

#define ICF_RESOURCE_NOT_RESERVED			IMSC_RESOURCE_NOT_RESERVED
#define ICF_RESOURCE_RESERVED				IMSC_RESOURCE_RESERVED

    /* Following are the values taken by desired status */
#define ICF_RESOURCE_RESERVATION_MANDATORY	IMSC_RESOURCE_RESERVATION_MANDATORY
#define ICF_RESOURCE_RESERVATION_OPTIONAL	IMSC_RESOURCE_RESERVATION_OPTIONAL
#define ICF_RESOURCE_RESERVATION_NONE		IMSC_RESOURCE_RESERVATION_NONE
#define ICF_RESOURCE_RESERVATION_FAILURE		IMSC_RESOURCE_RESERVATION_FAILURE
#define ICF_RESOURCE_RESERVATION_UNKNOWN		IMSC_RESOURCE_RESERVATION_UNKNOWN

    /* Following are the values taken by confirmed status */
#define ICF_RESOURCE_RESERVATION_CONF_NOT_REQ	IMSC_RESOURCE_RESERVATION_CONF_NOT_REQ
#define ICF_RESOURCE_RESERVATION_CONF_REQ		IMSC_RESOURCE_RESERVATION_CONF_REQ

typedef imsc_qos_st icf_qos_st;
#endif


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_codec_st
 *
 * Description: It is used to define the properties of the codecs.
 *
 *---------------------------------------------------------------------------*/

 typedef imsc_codec_st			icf_codec_st;

 /*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_codec_list_st
 *
 * Description: This data type is used to define a structure, which contains the
 *              list of codecs
 *
 *---------------------------------------------------------------------------*/

  typedef imsc_codec_list_st	icf_codec_list_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_req_stream_st
 *
 * Description: Denotes Media Stream and its associated parameters.
 *
 *---------------------------------------------------------------------------*/

#define ICF_LOCAL_CODEC_PRESENT		  IMSC_CREATE_MEDIA_LOCAL_CODEC_PRESENT
#define ICF_REMOTE_SDP_PRESENT		  IMSC_CREATE_MEDIA_REMOTE_SDP_PRESENT                
#define ICF_REMOTE_TBCP_INFO_PRESENT	  IMSC_CREATE_MEDIA_REMOTE_TBCP_INFO_PRESENT          
#define ICF_MEDIA_MODE_PRESENT	      IMSC_CREATE_MEDIA_MODE_PRESENT                       
#define ICF_QOS_MARK_PRESENT			  IMSC_CREATE_MEDIA_QOS_MARK_PRESENT               

  typedef imsc_req_stream_st icf_req_stream_st;

  
/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_resp_stream_st
 *
 * Description: Contains the media stream parameters returned in response for 
                each stream specified in request.
 *
 *---------------------------------------------------------------------------*/

#define ICF_LOCAL_SDP_PRESENT                  IMSC_LOCAL_SDP_PRESENT
#define ICF_TBCP_PORT_PRESENT                  IMSC_TBCP_PORT_PRESENT

  typedef imsc_resp_stream_st icf_resp_stream_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: imsc_modify_stream_st
 *
 * Description: Denotes Media Stream and its associated parameters.
 *
 *---------------------------------------------------------------------------*/

   typedef imsc_modify_stream_st icf_modify_stream_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_mm_create_media_session_req_st
 *
 * Description: Used to pass parameters for ICF_CC_MM_CREATE_MEDIA_SESSION_REQ
 *              API      
 *
 *---------------------------------------------------------------------------*/

#define ICF_LINE_ID_PRESENT IMSC_LINE_ID_PRESENT

typedef imsc_cc_mm_create_media_session_req_st icf_cc_mm_create_media_session_req_st;



/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_mm_cc_create_media_session_resp_st
 *
 * Description: Used to pass parameters for ICF_MM_CC_CREATE_MEDIA_SESSION_RESP API
 *
 *---------------------------------------------------------------------------*/

typedef imsc_mm_cc_create_media_session_resp_st icf_mm_cc_create_media_session_resp_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_mm_modify_media_session_req_st
 *
 * Description: Used to pass parameters for 
 *              ICF_CC_MM_MODIFY_MEDIA_SESSION_REQ
 *
 *---------------------------------------------------------------------------*/

typedef imsc_cc_mm_modify_media_session_req_st icf_cc_mm_modify_media_session_req_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_mm_merge_media_session_req_st
 *
 * Description: Used to pass parameters for ICF_CC_MM_MERGE_MEDIA_SESSION_REQ
 *
 *---------------------------------------------------------------------------*/

typedef imsc_cc_mm_merge_media_session_req_st icf_cc_mm_merge_media_session_req_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_mm_cc_delete_media_session_ind_st 
 *
 * Description: Used to pass parameters for
 *              ICF_MM_CC_DELETE_MEDIA_SESSION_IND 
 *
 *---------------------------------------------------------------------------*/

typedef imsc_mm_cc_delete_media_session_ind_st icf_mm_cc_delete_media_session_ind_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_rm_get_codec_list_req_st
 *
 * Description: Used to pass parameters for ICF_CC_RM_GET_CODEC_LIST_REQ API
 *
 *---------------------------------------------------------------------------*/

#define ICF_CC_RM_GET_CODEC_REMOTE_AUDIO_CODEC_LIST IMSC_CC_RM_GET_CODEC_REMOTE_AUDIO_CODEC_LIST          
#define ICF_CC_RM_GET_CODEC_REMOTE_VIDEO_CODEC_LIST IMSC_CC_RM_GET_CODEC_REMOTE_VIDEO_CODEC_LIST          

typedef imsc_cc_rm_get_codec_list_req_st icf_cc_rm_get_codec_list_req_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_rm_cc_codec_list_resp_st
 *
 * Description: Used to pass parameters for ICF_RM_CC_GET_CODEC_LIST_RESP API
 *
 *---------------------------------------------------------------------------*/

#define ICF_RM_CC_CODEC_LIST_RESP_AUDIO_CODEC_LIST IMSC_RM_CC_CODEC_LIST_RESP_AUDIO_CODEC_LIST 
#define ICF_RM_CC_CODEC_LIST_RESP_VIDEO_CODEC_LIST IMSC_RM_CC_CODEC_LIST_RESP_VIDEO_CODEC_LIST 

typedef imsc_rm_cc_get_codec_list_resp_st icf_rm_cc_get_codec_list_resp_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_rm_codec_commit_req_st
 *
 * Description: Used to pass parameters for ICF_CC_RM_CODEC_COMMIT_REQ
 *
 *---------------------------------------------------------------------------*/

#define ICF_CC_RM_CODEC_COMMIT_VIDEO_CODEC IMSC_CC_RM_CODEC_COMMIT_VIDEO_CODEC

typedef imsc_cc_rm_codec_commit_req_st icf_cc_rm_codec_commit_req_st;

typedef icf_long_string_st icf_long_string_st;

#define ICF_MM_CC_MEDIA_CAPABILITIES_RESP_VIDEO_SDP IMSC_MM_CC_MEDIA_CAPABILITIES_RESP_VIDEO_SDP
typedef imsc_mm_cc_media_capabilities_resp_st icf_mm_cc_media_capabilities_resp_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__IMSC_ICF_MAP_TYPES_H__*/
