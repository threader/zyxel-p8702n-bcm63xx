 
/***********************************************************************
 ** FUNCTION: Debug related functions
 **             
 *********************************************************************
 **
 ** FILENAME:	sdf_debug.h
 **
 ** DESCRIPTION:
 ** 	This file contains prototypes for all debug related functions.
 **
 **
 ** DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 ** 23/02/01   Rajasri				Initial Creation    	
 **
 **
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

 
#ifndef __SDF_DEBUG_H_
#define __SDF_DEBUG_H_

#include "sdf_struct.h"
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 ** FUNCTION:  	 sdf_fn_debug
 **
 ** DESCRIPTION: For tracing the path of execution. This prints debug
 ** 			 comments if SDF_DEBUG flag is used as a compile time 
 **				 option.
 **
 ** PARAMETERS:
 **	 str(IN): Debug text 
 **
 ************************************************************************/
  void sdf_fn_debug _ARGS_((Sdf_ty_s8bit *str));


/*************************************************************************
 ** FUNCTION:  	 sdf_fn_dumpError
 **
 ** DESCRIPTION: For tracing the path of execution. This prints debug 
 ** 			 comments if SDF_ERROR flag is used as a compile time 
 **				 option.
 **
 ** PARAMETERS:
 **		str(IN): Debug text 
 **
 ************************************************************************/
  void sdf_fn_dumpError _ARGS_((Sdf_ty_s8bit *str));


/*************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaInitCallFlow 
 **
 ** DESCRIPTION: For tracing the SipMessage flow. This initializes
 **			 	 the ctascii utility to show the SipMessage flow 
 **				 from/to the UA.
 **
 ** PARAMETERS:
 **	 pIp	(IN)  : The IP address on which ctascii is running
 **	 dPort	(IN)  : The port on which ctascii is running
 **	 pUaOne	(IN)  : String representing the first UA
 **	 pUaTwo	(IN)  : String representing the second UA
 **	 pErr	(OUT) : Error structure in case the function fails
 **
 ***********************************************************************/
  Sdf_ty_retVal sdf_ivk_uaInitCallFlow _ARGS_((Sdf_ty_s8bit *pIp,\
		Sdf_ty_u32bit dPort,Sdf_ty_s8bit *pUaOne,Sdf_ty_s8bit *pUaTwo, \
		Sdf_st_error *pErr));


/************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateCallFlow
 **
 ** DESCRIPTION: For tracing the SipMessage flow. This uses the ctascii 
 **				 utility to show the SipMessage flow from/to the UA
 **
 ** PARAMETERS:
 **  pSipMsg (IN)		: The SipMessage to be sent or just received.
 **	 dMsgDirection (IN)	: Boolean indicating whether the message is being
 **							sent or was just received by the UA toolkit.
 **							Sdf_co_true -> Message received from network.
 **							Sdf_co_false -> Message being sent out.
 **
 ***********************************************************************/
  void sdf_fn_uaUpdateCallFlow _ARGS_((SipMessage *pSipMsg, \
 		Sdf_ty_bool dMsgDirection));


/***********************************************************************
 ** FUNCTION: 	 sdf_ivk_uaDisplayCallFlow
 **
 ** DESCRIPTION: For tracing the SipMessage flow. This interprets the
 **				 input buffer to a graphic display using the ctascii 
 **				 utility.
 **
 ** PARAMETERS:
 **	 buf		(IN)  : String representing the display required
 **
 ***********************************************************************/
  void sdf_ivk_uaDisplayCallFlow _ARGS_((	Sdf_ty_s8bit *buf));


/***********************************************************************
 ** FUNCTION: 	 sdf_fn_setError
 **
 ** DESCRIPTION: This functions is used to set the error code and 
 **				 dump the error message on stderr.
 **
 ** PARAMETERS:
 **	 dErrorLevel(IN)  : The error level for the error passed 
 **	 dErrorCode	(IN)  : The error code to be set
 **	 pStr		(IN)  :	The string to be displayed for this error
 **	 pErr		(OUT) : Error structure in case the function fails
 **
 ***********************************************************************/
 void sdf_fn_setError _ARGS_((Sdf_ty_uaErrorLevel dErrorLevel,\
	Sdf_ty_errorCode dErrorCode, const Sdf_ty_s8bit *pStr, Sdf_st_error *pErr));


/***********************************************************************
 ** FUNCTION: 	 sdf_ivk_setErrorLevel
 **
 ** DESCRIPTION: This functions is used to set the error level
 **				 for the UA Toolkit
 **
 ** PARAMETERS:
 **	 dErrorLevel	(IN)  : The error level  to be set.
 **	 pErr		(OUT) : Error structure in case the function fails.
 **
 ***********************************************************************/	
  Sdf_ty_retVal sdf_ivk_setErrorLevel _ARGS_((Sdf_ty_uaErrorLevel \
		dErrLevel, Sdf_st_error *pErr));

	
/************************************************************************
 ** FUNCTION: sdf_ivk_getErrorLevel
 **
 ** DESCRIPTION:  This functions is used to get the error level.
 **
 ** PARAMETERS:
 **	 pErrLevel	(IN)  : The error level is returned in this variable.
 **	 pErr		(OUT) : Error structure in case the function fails.
 **
 ***********************************************************************/
  Sdf_ty_retVal sdf_ivk_getErrorLevel _ARGS_((Sdf_ty_uaErrorLevel \
	*pErrLevel, Sdf_st_error *pErr));


 /***************************************************************************
 ** FUNCTION: 		sdf_fn_initErrorStrings
 **
 ** DESCRIPTION:  	This functions is used to set the Error Strings
 **					corresponding to the error Codes in the UA Toolkit.
 **
 ***************************************************************************/
void sdf_fn_initErrorStrings _ARGS_((void));


#ifdef __cplusplus
}
#endif 
#endif
