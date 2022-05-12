/*------------------------------------------------------------------------------
 *
 * File name        : imsc_icf_map_defs.h
 *
 * Purpose          : This file contains the mapping of ICF and ICF defs.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 03-Aug-2005  Rohit Aggarwal                  Initial creation
 * 01-Mar-2006  Amit Sharma                     ICF merging with ICF2.0
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/



#ifndef __IMSC_ICF_MAP_DEFS_H__
#define __IMSC_ICF_MAP_DEFS_H__


#include "imsc_defs.h"
#include "imsc_extif_mm_api_id.h"


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
#if 0
/* Maximum number of media streams per call used in CMS and MMS req and resp. */	
#define ICF_MAX_MEDIA_STREAMS_PER_CALL	IMSC_MAX_MEDIA_STREAMS_PER_CALL

/* Maximum length of String to be encoded used in codec_st. */	
#define ICF_MAX_ENCODING_STR_LEN			IMSC_MAX_ENCODING_STR_LEN

/* Maximum number of codecs in a single Codec List used in codec_list_st */	
#define ICF_MAX_CODEC_IN_LIST			IMSC_MAX_CODEC_IN_LIST

/* Identifies the Stream type identifier as AUDIO */
#define ICF_STREAM_TYPE_AUDIO			IMSC_STREAM_TYPE_AUDIO

/* Identifies the Stream type identifier as VIDEO */
#define ICF_STREAM_TYPE_VIDEO			IMSC_STREAM_TYPE_VIDEO

/* Identifies the Stream type identifier as TBCP */
#define ICF_STREAM_TYPE_TBCP				IMSC_STREAM_TYPE_TBCP


/*---------------------------------------------------------------------------*/
/* Following are the APIs from SIP Packet Phone Signaling Toolkit to
 * Resource Manager and back. */

#define ICF_RM_API_START                      IMSC_GET_CODEC_LIST_REQ

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards RM to 
 * get the list of codecs available for the new call and reserve them. 
 * It can be used without a list of remote codecs in which case the
 * RM needs to reserve all list of available codecs or it can be sent
 * with remote codecs in which case RM needs to reserve one - the best
 * possible - codec. */

#define ICF_CC_RM_GET_CODEC_LIST_REQ          IMSC_GET_CODEC_LIST_REQ

/* This API is sent in response to ICF_CC_RM_GET_CODEC_LIST_REQ with
 * list of local codecs allocated for the current media session. */

#define ICF_RM_CC_GET_CODEC_LIST_RESP         IMSC_GET_CODEC_LIST_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards 
 * Resource manager to commit one of the codec among the list of codecs
 * that were earlier reserved by RM. */

#define ICF_CC_RM_CODEC_COMMIT_REQ            IMSC_CODEC_COMMIT_REQ

/* This API is sent in response to ICF_CC_RM_CODEC_COMMIT_REQ to 
 * inform that the codec has been committed and all other reserved codecs
 * have been unreserved. */

#define ICF_RM_CC_CODEC_COMMIT_RESP           IMSC_CODEC_COMMIT_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Resource manager to release all reserved codecs for this media
 * session. */

#define ICF_CC_RM_REL_RES_REQ                 IMSC_REL_RES_REQ

/* This API is sent in response to ICF_CC_RM_REL_RES_REQ to inform that the
 * codecs reserved for this media session have been deallocated. */

#define ICF_RM_CC_REL_RES_RESP                IMSC_REL_RES_RESP

#define ICF_RM_API_END                        ICF_RM_CC_REL_RES_RESP

/* The following list of APIs list the APIs towards Media Manager and back
 * from Media Manager. */

#define ICF_MM_API_START                      IMSC_CC_MM_API_START

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to create media session for a new call. The API may 
 * contain the local parameters for the media session like codec and 
 * the media direction. The API may also contain the remote media 
 * parameters like remote codec, remote RTP port and IP address etc.*/

#define ICF_CC_MM_CREATE_MEDIA_SESSION_REQ    IMSC_CREATE_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_CREATE_MEDIA_SESSION_REQ
 * to inform that the media session has been created. It also informs
 * the list of local media parameters for this media session in this API. */

#define ICF_MM_CC_CREATE_MEDIA_SESSION_RESP   IMSC_CREATE_MEDIA_SESSION_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to modify the media session parameters for a existing call. 
 * */

#define ICF_CC_MM_MODIFY_MEDIA_SESSION_REQ    IMSC_MODIFY_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_MODIFY_MEDIA_SESSION_REQ
 * to inform that the media session has been modified.*/

#define ICF_MM_CC_MODIFY_MEDIA_SESSION_RESP   IMSC_MODIFY_MEDIA_SESSION_RESP

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to delete an existing media session. */

#define ICF_CC_MM_DELETE_MEDIA_SESSION_REQ    IMSC_DELETE_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_DELETE_MEDIA_SESSION_REQ
 * to inform that the media session has been deleted. */

#define ICF_MM_CC_DELETE_MEDIA_SESSION_RESP   IMSC_DELETE_MEDIA_SESSION_RESP

/* This API is sent by Media Manager to SIP Packet Phone Signaling Toolkit 
 * to indicate any media failure and that the media session for an 
 * existing call has been deleted. This API has no response. */

#define ICF_MM_CC_DELETE_MEDIA_SESSION_IND    IMSC_DELETE_MEDIA_SESSION_IND

/* This API is invoked by SIP Packet Phone Signaling Toolkit towards
 * Media Manager to merge two pre existing media sessions. The two media 
 * sessions are identified by their respective call id. */

#define ICF_CC_MM_MERGE_MEDIA_SESSION_REQ     IMSC_MERGE_MEDIA_SESSION_REQ

/* This API is sent in response to ICF_CC_MM_MERGE_MEDIA_SESSION_REQ
 * to indicate that the merging of the media sessions was successful. */

#define ICF_MM_CC_MERGE_MEDIA_SESSION_RESP    IMSC_MERGE_MEDIA_SESSION_RESP

#define ICF_CC_MM_MEDIA_CAPABILITIES_REQ      IMSC_CC_MM_MEDIA_CAPABILITIES_REQ

#define ICF_MM_CC_MEDIA_CAPABILITIES_RESP     IMSC_MM_CC_MEDIA_CAPABILITIES_RESP

#define ICF_MM_API_END                        IMSC_CC_MM_API_END


/* Module Mapping for Media Manager */
#define ICF_MODULE_MM              IMSC_MODULE_MM
#define ICF_MODULE_RM              IMSC_MODULE_MM
/* Module Mapping for PA, This is a temporary fix for MMI
   which uses the src and dest module-id for routing */
#define ICF_MODULE_PA              IMSC_MODULE_APP


/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__IMSC_ICF_MAP_DEFS_H__*/
