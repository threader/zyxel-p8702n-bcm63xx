/***********************************************************************
 ** FUNCTION:
 **			This file has the freeing functions for all SDP structures
 **
 *********************************************************************
 **
 ** FILENAME		: sdf_sdpfree.h
 **
 ** DESCRIPTION		: This file has the freeing functions for all SDP structures
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 29/11/02	Pankaj Nath						Creation
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __SDF_SDP_FREE_H_
#define __SDF_SDP_FREE_H_

#include "sdf_struct.h"
#include "sdf_sdpstruct.h"

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeCodec
**
** DESCRIPTION: This function frees the Sdf_st_codec  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeCodec _ARGS_((Sdf_st_codec *pCodec));

#ifdef SDF_MEDIAGROUP
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeMediaStreamGroup
**
** DESCRIPTION: This function frees the Sdf_st_mediaStreamGroup  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeMediaStreamGroup _ARGS_((\
	Sdf_st_mediaStreamGroup *pMediaStreamGroup));
#endif

#ifdef SDF_QOS
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeQosInfo
**
** DESCRIPTION: This function frees the Sdf_st_qosInfo  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeQosInfo _ARGS_((Sdf_st_qosInfo *pQosInfo));


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSegmentedQosDetails
**
** DESCRIPTION: This function frees the Sdf_st_segmentedQosDetails  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSegmentedQosDetails _ARGS_((\
	Sdf_st_segmentedQosDetails *pSegmentedQosDetails));


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeE2eQosDetails
**
** DESCRIPTION: This function frees the Sdf_st_e2eQosDetails  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeE2eQosDetails _ARGS_((\
	Sdf_st_e2eQosDetails *pE2eQosDetails));


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeQosAttrib
**
** DESCRIPTION: This function frees the Sdf_st_qosAttrib  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeQosAttrib _ARGS_((\
	Sdf_st_qosAttrib *pQosAttrib));
#endif

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSessionAttrib
**
** DESCRIPTION: This function frees the Sdf_st_sessionAttrib  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSessionAttrib _ARGS_((\
	Sdf_st_sessionAttrib *pSessionAttrib));


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeMediaStream
**
** DESCRIPTION: This function frees the Sdf_st_mediaStream  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeMediaStream _ARGS_((\
	Sdf_st_mediaStream *pMediaStream));


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSessionParam
**
** DESCRIPTION: This function frees the Sdf_st_sessionParam  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSessionParam _ARGS_((\
	Sdf_st_sessionParam *pSessionParam));

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeMediaHandling
**
** DESCRIPTION: This function frees the Sdf_st_mediaHandling  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeMediaHandling _ARGS_((\
	Sdf_st_mediaHandling *pMediaHandling));

#ifdef SDF_SESSIONCHANGE
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSessionChange
**
** DESCRIPTION: This function frees the Sdf_st_sessionChange  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSessionChange _ARGS_((\
	Sdf_st_sessionChange *pSessionChange));
#endif

void __sdf_fn_uaFreeCodec _ARGS_((Sdf_ty_pvoid pCodec));

/***********************************************************************
** Function: __sdf_fn_uaFreeMediaStream
** Description: Frees the Sdf_st_mediaStream structure
**
************************************************************************/
void __sdf_fn_uaFreeMediaStream 
_ARGS_((Sdf_ty_pvoid pMediaStream));

#ifdef SDF_MEDIAGROUP
/***********************************************************************
** Function: __sdf_fn_uaFreeMediaStreamGroup
** Description: Frees the Sdf_st_mediaStreamGroup structure
**
************************************************************************/
void __sdf_fn_uaFreeMediaStreamGroup
_ARGS_((Sdf_ty_pvoid pMediaStreamGroup));
#endif


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

#endif
