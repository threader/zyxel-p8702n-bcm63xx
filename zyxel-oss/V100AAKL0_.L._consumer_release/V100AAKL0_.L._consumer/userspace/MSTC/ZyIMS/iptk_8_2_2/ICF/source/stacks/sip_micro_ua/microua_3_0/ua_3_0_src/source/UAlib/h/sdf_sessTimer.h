

/***********************************************************************
 ** FUNCTION:
 **			This file has all the accessor API's to retrieve/clone 
 **			various structures in the UA toolkit
 *********************************************************************
 **
 ** FILENAME		: sdf_accessor.h
 **
 ** DESCRIPTION		: This file has all the accessor API's to retrieve/clone 
 **					  various structures in the UA toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 06/12/00	Subhash						Creation
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __SDF_SESSTIMER_H_
#define __SDF_SESSTIMER_H_
#include "sdf_common.h"
#include "sdf_struct.h"
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
#ifdef SDF_SESSION_TIMER

/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetRefresher
**
** DESCRIPTION: This function returns the Refresher member of the 
**	SessionTimerInfo Structure of the Call Object
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaGetRefresher	_ARGS_((Sdf_st_callObject *pCallObj,\
 	Sdf_ty_refresher *pRefresher, Sdf_st_error  *pErr ));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetRefresher
**
** DESCRIPTION: This function Sets the Refresher  member of the 
**	SessionTimerInfo Structure of the Call Object what passed in the argument
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaSetRefresher _ARGS_((Sdf_st_callObject *pCallObj,\
  	Sdf_ty_refresher dRefresher, Sdf_st_error  *pErr ));
  
  
/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetSessionInterval
**
** DESCRIPTION: This function returns the Session Interval value from the 
**	SessionTimerInfo Structure of the Call Object
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaGetSessionInterval _ARGS_((Sdf_st_callObject \
  	*pCallObj, Sdf_ty_u32bit *pSessionInterval, Sdf_st_error  *pErr ));


/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetSessionInterval
**
** DESCRIPTION: This function Sets the Session Interval  member of the 
**	SessionTimerInfo Structure of the Call Object what passed in the argument
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaSetSessionInterval _ARGS_(( Sdf_st_callObject  \
  	*pCallObj, Sdf_ty_s32bit dSessionInterval, Sdf_st_error  *pErr ));


/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetMinSe
**
** DESCRIPTION: This function returns the MinSe value from the 
**	SessionTimerInfo Structure of the Call Object
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaGetMinSe _ARGS_(( Sdf_st_callObject \
  	*pCallObj,Sdf_ty_u32bit *pMinSe, Sdf_st_error  *pErr ));


/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetMinSe
**
** DESCRIPTION: This function Sets the MinSe member of the 
**	SessionTimerInfo Structure of the Call Object what passed in the argument
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaSetMinSe _ARGS_(( Sdf_st_callObject \
  	*pCallObj,Sdf_ty_s32bit dMinSe, Sdf_st_error  *pErr ));


/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetSessionTimerInfoFromCallObject
**
** DESCRIPTION: This function returns the SessionTimerInfo structure from
**				 the Call Object
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaGetSessionTimerInfoFromCallObject _ARGS_((\
  	Sdf_st_callObject *pCallObj, Sdf_st_sessionTimerInfo \
	**pSessionExpire, Sdf_st_error  *pErr ));

  
/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetSessionTimerInfoIntoCallObject
**
** DESCRIPTION: This function sets the SessionTimerInfo structure into
**				 the Call Object
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaSetSessionTimerInfoIntoCallObject _ARGS_((\
  	Sdf_st_callObject *pCallObj, Sdf_st_sessionTimerInfo \
	*pSessionExpire, Sdf_st_error  *pErr ));
  

/*****************************************************************************
** FUNCTION: sdf_ivk_uaInitSessionTimerInfo
**
** DESCRIPTION: This function initialise the members of the SessionTimerInfo
**				 Structure of the Call Object
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaInitSessionTimerInfo _ARGS_((\
  	Sdf_st_sessionTimerInfo **pSessionExpire, Sdf_st_error  *pErr ));



/*****************************************************************************
** FUNCTION: sdf_ivk_uaFreeSessionTimerInfo
**
** DESCRIPTION: This function Free the SessionTimerInfo
**				 Structure from the Call Object. Decrements the refcount
**				and if it is zero then free the structure
**
******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaFreeSessionTimerInfo _ARGS_((\
  	Sdf_st_sessionTimerInfo *pSessionExpire));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionTimerInfo
 **
 ** DESCRIPTION: This function copies the SessionTimerInfo structure from 
 **				 src to dest
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaCloneSessionTimerInfo _ARGS_((\
  	Sdf_st_sessionTimerInfo *pDestSessionExpire,\
	Sdf_st_sessionTimerInfo *pSrcSessionExpire, Sdf_st_error  *pErr));



/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaSessionTimerExpire
 **
 ** DESCRIPTION: This is a Session Timer Expiry function. 
 **				 This function will send refresh re-INVITE or BYE on Expiry 
 **				 of the Session Timer Interval
 **
 ******************************************************************************/
  Sdf_ty_retVal  sdf_fn_uaSessionTimerExpire _ARGS_((\
  	Sdf_ty_timerType dTimerType,Sdf_ty_pvoid  pContextInfo,Sdf_st_error *pErr));



/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaFormRefresh
 **
 ** DESCRIPTION:	 This function will send forms a refresh re-INVITE
 **					 It will insert the same sdp as previously send
 **					 or BYE on Expiry 
 **
 ******************************************************************************/
  Sdf_ty_retVal  sdf_ivk_uaFormRefresh _ARGS_((\
  	Sdf_st_callObject  *pCallObj, Sdf_st_error  *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaNegotiateRefresher
 **
 ** DESCRIPTION: This function will negotiate the refresher member
 **				 of the Session Expires header of the incoming message
 **				 and set the refresher member of the SessionTimerInfo
 **				 structure
 **
 ******************************************************************************/
  
  Sdf_ty_retVal  sdf_fn_uaNegotiateRefresher _ARGS_((\
  	Sdf_st_callObject *pCallObj,Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIsSupportedSessionTimer
 **
 ** DESCRIPTION: This function checks whether incoming message has
 **				 Supported header with value Timer. If present then
 **				 *pIsSupported argumet is assign to true 
 **				 otherwise false.
 **				 
 **				 it checks the UAS transaction SipMessage.
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_fn_uaIsSupportedSessionTimer _ARGS_((\
  	Sdf_st_callObject *pCallObj, SipMessage *pMessage,\
	Sdf_ty_bool *pIsSupported,	Sdf_st_error  *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaAddSessionTimerSpecificHeader
 **
 ** DESCRIPTION: This function will add the Session Timer Specific
 **				 header depending on the value presents in the
 **				 SessionTimerInfo structure into the outgoing
 **				 message 
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_fn_uaAddSessionTimerSpecificHeader _ARGS_((\
  	Sdf_st_callObject *pCallObj,SipMessage *pMessage,\
	Sdf_ty_messageType 	dMsgType, Sdf_st_error  *pErr));


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaIsActiveSessionTimer
 **
 ** DESCRIPTION: This function checks whether timer is started or not
 **				 for the Session Timer
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_ivk_uaIsActiveSessionTimer  _ARGS_((\
  	Sdf_st_callObject *pCallObj,Sdf_ty_bool *pIsActive,\
	Sdf_st_error  *pErr));


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaStartRefresh
 **
 ** DESCRIPTION: This function start the timer by calling 
 **				  sdf_cbk_uaStartTimer 
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaStartRefresh  _ARGS_((\
	Sdf_st_callObject *pCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaStopSessionTimer
 **
 ** DESCRIPTION: This function stop the timer by calling 
 **				  sdf_cbk_uaStopTimer 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaStopSessionTimer _ARGS_((\
	Sdf_ty_pvoid	*pTimerHandle, Sdf_st_error	*pErr));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaStartSessionTimer
 **
 ** DESCRIPTION: This function start the timer by calling 
 **				  sdf_cbk_uaStartTimer 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaStartSessionTimer _ARGS_((\
	Sdf_st_callObject *pCallObj, Sdf_ty_pvoid *ppTimerHandle,\
	Sdf_ty_s32bit duration, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr));

	
/*****************************************************************************
** FUNCTION: sdf_fn_uaFreeSessionTimerContextInfo
**
** DESCRIPTION: This function Free the SessionTimerContextInfo
**				  Decrements the refcount
**				and if it is zero then free the structure
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFreeSessionTimerContextInfo _ARGS_((\
	Sdf_st_sessionTimerContextInfo		*pSessionTimerConext));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetRetransCount
 **
 ** DESCRIPTION: This function Returns the retransmission count required.
 **				 Based on the duration of the total timer expiry, this
 **				 function calculates the max retransmission count for invites.
 **					 Provide the duration in milli secs.
 **	PARAMATERS	:	
 **		InitDuration:	Initial timer duration. After which first
 **						 retransmission happens
 **		TotalDuration:	Final Duration after which stop retransmission.
 **	
 **		Return value :  This function returns the number of retransmissions
 **		 
 **
 ****************************************************************************/

Sdf_ty_u32bit sdf_fn_uaGetRetransCount _ARGS_(( Sdf_ty_u32bit InitDuration, \
				Sdf_ty_u32bit TotalDuration, Sdf_st_error *pErr ));

#endif  
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

#endif	
