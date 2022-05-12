
/******************************************************************************
 ** FUNCTION:
 **			This file has the initialization functions for all SDP structures
 **
 ******************************************************************************
 **
 ** FILENAME		: sdf_sdpinit.h
 **
 ** DESCRIPTION		: This file has the initialization functions for 
 **						all SDP structures
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 29/11/02	Pankaj Nath						Creation
 **
 ******************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_SDP_INIT_H_
#define __SDF_SDP_INIT_H_

#include "sdf_struct.h"
#include "sdf_sdpstruct.h"
#include "sdf_free.h"
#include "sdf_sdpfree.h"


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitCodec
**
** DESCRIPTION: This function inits the Sdf_st_codec Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitCodec _ARGS_((\
	Sdf_st_codec **ppCodec,Sdf_st_error *pErr));


#ifdef SDF_MEDIAGROUP
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaStreamGroup
**
** DESCRIPTION: This function inits the Media Stream Group Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaStreamGroup _ARGS_((\
	Sdf_st_mediaStreamGroup **ppMediaStreamGroup,Sdf_st_error *pErr));
#endif

#ifdef SDF_QOS
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitQosInfo
**
** DESCRIPTION: This function inits the Qos Info Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitQosInfo _ARGS_((\
	Sdf_st_qosInfo **ppQosInfo,Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSegmentedQosDetails
**
** DESCRIPTION: This function inits the Qos Info Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSegmentedQosDetails _ARGS_((\
	Sdf_st_segmentedQosDetails **ppSegmentedQosDetails,Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitE2eQosDetails
**
** DESCRIPTION: This function inits the Sdf_st_e2eQosDetails Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitE2eQosDetails _ARGS_((\
	Sdf_st_e2eQosDetails **ppE2eQosDetails,Sdf_st_error *pErr));
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitQosAttrib
**
** DESCRIPTION: This function inits the Sdf_st_qosAttrib Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitQosAttrib  _ARGS_((\
	Sdf_st_qosAttrib **ppQosAttrib,Sdf_ty_qosType dQosType,Sdf_st_error *pErr));
#endif

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSessionAttrib
**
** DESCRIPTION: This function inits the Sdf_st_sessionAttrib Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSessionAttrib _ARGS_((\
	Sdf_st_sessionAttrib **ppSessionAttrib,Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaStream
**
** DESCRIPTION: This function inits the Sdf_st_mediaStream Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaStream _ARGS_((\
	Sdf_st_mediaStream **ppMediaStream,Sdf_st_error *pErr));



/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSessionParam
**
** DESCRIPTION: This function inits the Sdf_st_sessionParam Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSessionParam _ARGS_((\
	Sdf_st_sessionParam **ppSessionParam,Sdf_st_error *pErr));

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaHandling
**
** DESCRIPTION: This function inits the Sdf_st_mediaHandling Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaHandling _ARGS_((\
	Sdf_st_mediaHandling **ppMediaHandling,Sdf_st_error *pErr));

#ifdef SDF_SESSIONCHANGE
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaChange
**
** DESCRIPTION: This function inits the Sdf_st_mediaChange Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaChange _ARGS_((\
	Sdf_st_mediaChange *pMediaChange,Sdf_st_error *pErr));


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSessionChange
**
** DESCRIPTION: This function inits the Sdf_st_sessionChange Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSessionChange _ARGS_((\
	Sdf_st_sessionChange **ppSessionChange,Sdf_st_error *pErr));
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

#endif
