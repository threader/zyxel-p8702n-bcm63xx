
/*****************************************************************************
 ** FUNCTION:
 **			This file contains the prototypes for all callbacks issued
 **			by the UA Toolkit
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf_callbacks.h
 **
 ** DESCRIPTION		: This file contains the prototypes for all callbacks 
 **						issued by the UA Toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 27/10/00	Anitha						Creation
 **	20/01/01	Prashant					Updated
 **	07/08/01	Subhash						Replaced authentication callbacks
 **											with sdf_cbk_uaAuthenticateUser
 **											& sdf_cbk_uaChallengeUser
 **
 ****************************************************************************
 **					COPYRIGHT , Aricent, 2006
 ****************************************************************************/

#ifndef __SDF_CALLBACKS_H_
#define __SDF_CALLBACKS_H_
#ifdef SDF_APPCONTROLLEDMEMPOOL
#include "sdf_mempool.h"
#endif

#include "sdf_portlayer.h"

#ifdef SDF_TEL
#include "microsip_telstruct.h"
#endif
#ifdef SDF_IM
#include "microsip_imurlstruct.h"
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
/****************************************************************************
 ** FUNCTION:  sdf_cbk_uaNewCallReceived
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of a
 **				fresh INVITE message indicating receipt of a new call
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP INVITE message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaNewCallReceived _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/****************************************************************************
 ** FUNCTION:  sdf_cbk_uaCancelReceived
 **
 ** DESCRIPTION: This callback is issued by the toolkit when 
 **				a CANCEL is received or a BYE is received when the INVITE
 **				has not yet been responded to.
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Temporary Call Object on which CANCEL was received
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCancelReceived _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/****************************************************************************
 ** FUNCTION:  sdf_cbk_uaCancelCompleted
 **
 ** DESCRIPTION: This callback is issued by the toolkit when 
 **				a final response to a CANCEL is received or a 
 **				final response to a BYE is received when the INVITE 
 **				has not yet been responded to.
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Temporary Call Object on which final resp was recd
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCancelCompleted _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/****************************************************************************
 ** FUNCTION:  sdf_cbk_uaInProgress
 **
 ** DESCRIPTION: This callback is issued onto the application on the receipt 
 **				of an 1xx message indicating that the call is in progress
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP 1xx message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaInProgress  _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/****************************************************************************
 ** FUNCTION:  sdf_cbk_uaCallAccepted
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				200 OK message to the INVITE message
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP 200 OK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCallAccepted  _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr )) ;


/****************************************************************************
 ** FUNCTION:  sdf_cbk_uaCallAcknowledged
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				ACK message 
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP ACK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCallAcknowledged  _ARGS_((Sdf_st_callObject \
	 **ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr )); 


/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaCallTerminateRequest
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				BYE message indicating termination of an existing call
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP BYE message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCallTerminateRequest  _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr )) ;

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaCallTerminated
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				200 OK message to the BYE sent
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP 200 OK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCallTerminated  _ARGS_((Sdf_st_callObject \
	 **ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaByeRejected
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				Non 200 Response message to the BYE sent
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP 200 OK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaByeRejected  _ARGS_((Sdf_st_callObject \
	 **ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaCallFailed
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				final failure message to an INVITE indicating failure of 
 **				a call request
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCallFailed  _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));		

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaFailedCallAcknowledged
 **
 ** DESCRIPTION: Callback issued to the application on the receipt of an
 **				ACK message to the final failure message sent
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP ACK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaFailedCallAcknowledged  _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaCallRedirected
 **
 ** DESCRIPTION: Callback issued to the application on the receipt of an
 **				3xx message indicating redirection of a call
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP 3xx message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCallRedirected  _ARGS_((Sdf_st_callObject \
		 **ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaTransactionReceived
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				REGISTER/OPTIONS/INFO/SUBSCRIBE/NOTIFY message 
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaTransactionReceived  _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaTransactionCompleted
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				200 OK message to one of REGISTER/OPTIONS/INFO/SUBSCRIBE
 **				NOTIFY
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP 200 OK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaTransactionCompleted  _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaReInviteReceived
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				INVITE message in the call established state, indicating 
 **				receipt of a reinvite
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP (re)INVITE message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaReInviteReceived _ARGS_((Sdf_st_callObject  \
	**ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaReInviteAccepted
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **	 			200 OK message to the re-invite message sent
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP 200 OK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaReInviteAccepted  _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr )) ;

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaReInviteAcknowledged
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				ACK message to a final repsonse to a reinvite
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP ACK message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaReInviteAcknowledged _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr )); 

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaReInviteFailed
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of an
 **				final failure response message indicating failure of the 
 **				reinvite request
 **
 ** PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaReInviteFailed  _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));	

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaRPRReceived
 **
 ** DESCRIPTION: Callback issued to the application on the receipt of a
 **	reliable provisional response
 **
 ** PARAMETERS:
 **		ppCallObj(IN)			: Call Object having the SIP message
 **		pOverlapTransInfo(IN)	: Reference to the Sdf_st_overlapTransInfo
 **									structure updated with this RPR
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)				: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaRPRReceived  _ARGS_((Sdf_st_callObject \
	**ppCallObj, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));	
/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaOverlapTransactionReceived
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of a
 **				PRACK/COMET
 **
 ** PARAMETERS:
 **		ppCallObj(IN)			: Call Object having the SIP message
 **		pOverlapTransInfo(IN)	: Reference to the Sdf_st_overlapTransInfo
 **									structure updated with this PRACK/COMET
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)				: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaOverlapTransactionReceived  _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_overlapTransInfo \
	*pOverlapTransInfo, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaOverlapTransactionCompleted
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of a
 **	final response to a PRACK/COMET
 **
 ** PARAMETERS:
 **		ppCallObj(IN)			: Call Object having the SIP message
 **		pOverlapTransInfo(IN)	: Reference to the Sdf_st_overlapTransInfo
 **									structure updated with this final response
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)				: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaOverlapTransactionCompleted  _ARGS_\
	((Sdf_st_callObject **ppCallObj, Sdf_st_overlapTransInfo \
	*pOverlapTransInfo, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr));	

/***********************************************************************
 ** FUNCTION:  sdf_cbk_uaForkedRespReceived
 **
 ** DESCRIPTION: Callback issued onto the application on the receipt of a
 **	Forked response to Invite.
 **
 ** PARAMETERS:
 **		pForkCallObj(IN) 	: Call Object Over which the Forked Resp Received
 **		pOrgCallObj(IN) 	: Call Object related to Main dialog.
 **		pEventContext(IN)	: The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)			: Error structure in case the function fails
 **
 ************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaForkedRespReceived  \
	_ARGS_((Sdf_st_callObject *pForkCallObj, Sdf_st_callObject *pOrgCallObj, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));

#ifdef SDF_AUTHENTICATE
/*****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaAuthenticateUser
 **
 ** DESCRIPTION: This callback is issued by the toolkit when 
 **				 it needs to verify the credentials of a user. 
 **				 It is issued by the sdf_ivk_uaAuthorize API 
 **				 when the request contains an Authorization 
 **				 header, and the toolkit needs to verify if 
 **				 the credentials in the header are valid. The
 **				 application should return the clear-text password
 **				 (in case of Basic) or H(A1) (in case of Digest)
 **				 from its database.
 **			
 ** PARAMETERS:
 **		ppCallObj(IN)			: Call Object pertaining to the call
 **		pAddrSpec(IN)			: SipAddrSpec of the endpoint from where 
 **								  the request originated ("From" address)
 **		pAuthParams (IN/OUT) 	: Partially filled structure. By examining
 **									this structure and the pAddrSpec, the
 **									application should fill the pSecret
 **									member with the clear-text password
 **									(in case of Basic) or H(A1) (in case
 **									of Digest)
 **		ppContext(IN/OUT)		: Used to pass any additional data back
 **		pErr(OUT)				: Error structure in case the function fails
 **
 *****************************************************************************/
extern Sdf_ty_authResult sdf_cbk_uaAuthenticateUser _ARGS_\
	((Sdf_st_callObject **ppCallObj, SipAddrSpec *pAddrSpec, \
	Sdf_st_uasAuthInfo *pUasAuthInfo, Sdf_ty_pvoid *ppContext, \
	Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaChallengeUser
 **
 ** DESCRIPTION: This callback is issued by the toolkit under
 **				 two scenarios:
 **				 a) If a request arrives without Authorization header.
 **			 		The application should look up it's database, and
 **					inform the toolkit if the user needs to be challenged
 **					and if so, with what parameters.
 **				 b) If a request arrives with a Authorization header
 **					but the credentials do not match with that
 **					present in the application. In this case, the
 **					application is issued the callback to re-challenge
 **					the user with new/same parameters
 **
 **				 The application should indicate whether the user is to be
 **				 challenged, and if so, return the realm (in case of Basic)
 **				 OR the {realm, opaque, qop, algorithm, domain, stale, 
 **				 nonce} (in case of Digest)
 **
 ** PARAMETERS:
 **		ppCallObj(IN)			: Call Object pertaining to the call
 **		pAddrSpec(IN)			: SipAddrSpec of the endpoint from where 
 **									the request originated ("From" address)
 **		pAuthParams (IN/OUT)	: Structure containing challenge parameters
 **									that are to be filled in by the application
 **		ppContext(IN/OUT)		: Used to pass any additional data back
 **		pErr(OUT)				: Error structure in case the function fails
 **
 ******************************************************************************/
extern Sdf_ty_authResult sdf_cbk_uaChallengeUser _ARGS_\
	((Sdf_st_callObject **ppCallObj,\
	SipAddrSpec *pAddrSpec,Sdf_st_authenticationParams *pAuthParams,\
	Sdf_ty_pvoid *ppContext,Sdf_st_error *pErr));
#endif /* SDF_AUTHENTICATE */

/****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaUnknownMethodReceived
 **
 ** DESCRIPTION: This callback is issued by the toolkit when 
 **				 it gets a request with a method that is defined in the 
 **  			 user profile and that is normally not understood by the 
 **  			 toolkit.
 **
 **  PARAMETERS:
 **		ppCallObj(IN)	: Call Object having the SIP message
 **		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaUnknownMethodReceived _ARGS_((Sdf_st_callObject \
    **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaFreeApplicationData
 **
 ** DESCRIPTION: This callback is issued by the toolkit when 
 **				 sdf_ivk_uaFreeAppData is invoked and the reference count of
 **				 the Sdf_st_appData structure is 0. It indicates that the
 **				 toolkit no longer needs access to the structure. The 
 **				 application is passed the pData member of the Sdf_st_appData
 **				 structure and may free the member at this stage if it 
 **				 wants to.
 **
 **  PARAMETERS:
 **		ppData(IN)	:	Pointer to the void* member of the Sdf_st_appData 
 **						structure
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaFreeApplicationData _ARGS_\
	((Sdf_ty_pvoid *ppData));


/*****************************************************************************
 ** FUNCTION:  sdf_cbk_uaCloneAppData
 **
 ** DESCRIPTION: This is issued by the toolkit to the application when 
 **				the CallObject is to be cloned. The application should 
 **				clone the pData member of the AppData structure
 **				to copy the source to the destination.
 **
 ** PARAMETERS:
 **		pDestInfo(OUT): The structure to which the elements are copied
 **		pSrcInfo(IN) : The structure from which the elements are copied
 **		pError(OUT)	: Error structure in case the function fails
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCloneAppData _ARGS_((Sdf_st_appData \
	 *pDestInfo, Sdf_st_appData *pSrcInfo, Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaFreeEventContextData
 **
 ** DESCRIPTION: This callback is issued by the toolkit when 
 **				 sdf_ivk_uaFreeEventContext is invoked and the reference
 **				 count of the Sdf_st_eventContext structure is 0. It 
 **				 indicates that the toolkit no longer needs access to the 
 **				 structure. The application is passed the pData member of 
 **				 the Sdf_st_eventContext structure and may free the member 
 **				 at this stage if it wants to.
 **
 **  PARAMETERS:
 **		ppData(IN)	: Pointer to the void* member of the Sdf_st_eventContext 
 **						structure
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaFreeEventContextData _ARGS_\
	((Sdf_ty_pvoid *ppData));

extern Sdf_ty_retVal sdf_cbk_uaCloneEventContextpData
	_ARGS_((Sdf_st_eventContext *pDestInfo,
	Sdf_st_eventContext *pSrcInfo, Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaFreeEventContextData
 **
 ** DESCRIPTION: This callback is issued to the application on expiration
 **				 of session timer. In this callback application can perform
 **				 his own sets of operation to refresh/terminate the session 
 **				 and gives an indication to the UATK through pRefresh enum. 
 **				 If return value of pRefresh enum is Sdf_en_applicaton then
 **				 UATK will not perform refresh on behalf of application. 
 **				 If the value is Sdf_en_uatoolkit then UATK will 
 **				 UATK will perform refresh/terminate on behalf of
 **				 application 
 **
 ** PARAMETERS:
 **		pCallObj(IN)	: Call Object that has Session Timer times out
 **		pRefresh(OUT)	: The application is expected to set this enum 
 **						  for performing the refresh/terminated by the UATK 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/

#ifdef SDF_SESSION_TIMER
extern Sdf_ty_retVal sdf_cbk_uaHandleSTTimeout _ARGS_((\
	Sdf_st_callObject *pCallObj,Sdf_ty_handleEvent *pRefresh,\
	Sdf_ty_sessionRefreshRequest *pRefreshReq, Sdf_st_error  *pErr ));
#endif



typedef Sdf_ty_retVal (*Sdf_ty_TimertimeOutFunc)(Sdf_ty_timerType dTimerType,\
	Sdf_ty_pvoid pContextInfo, Sdf_st_error *pErr);

/*
 * Timer Related callbacks 
 */

/***************************************************************************
 ** FUNCTION: 		sdf_cbk_uaStartTimer
 **
 ** DESCRIPTION: 	This function will be used by all the modules to start 
 **					the timer. pContextInfo will be returned to the app when 
 **					the timer expires
 **
 ***************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaStartTimer _ARGS_(( 
	Sdf_ty_u32bit			dDuration, 
	Sdf_ty_timerType		dTimerType, 
	Sdf_ty_pvoid			pContextInfo, 
	Sdf_ty_pvoid			pAppData,
	Sdf_ty_TimertimeOutFunc timeoutFunc,
	Sdf_ty_pvoid			*ppTimerHandle, 
	Sdf_st_error			*pErr)); 


/***************************************************************************
 ** FUNCTION: 		sdf_cbk_uaFreeTimerHandle
 **
 ** DESCRIPTION: 	
 **
 ***************************************************************************/
void sdf_cbk_uaFreeTimerHandle _ARGS_((Sdf_ty_pvoid pTimerHandle));



/***************************************************************************
 ** FUNCTION: 		sdf_cbk_uaStopTimer
 **
 ** DESCRIPTION:	Stop the specified timer. pTimerHandle must be the handle
 **					that was returned by sdf_cbk_uaStartTimer. pContextInfo arg
 **					that was passed to sdf_cbk_uaStartTimer will be returned in
 **					ppContextInfo.
 **
 ***************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaStopTimer _ARGS_(( 
	Sdf_ty_pvoid	pTimerHandle, 
	Sdf_ty_pvoid	*ppContextInfo,
	Sdf_st_error	*pErr)); 


#ifdef SDF_APP_CONTROLLED_REMOTE_RETX

/****************************************************************************
 ** Function:  sdf_cbk_uaInitRemoteRetransTables
 **
 ** Description: This callback is issued while initializing the UATK if 
 **				 UATK has been compiled with SDF_APP_CONTROLLED_REMOTE_RETX 
 **				 flag.
 **				
 **				 This flag must be used if the application use its on remote
 **				 retransmission logic. Inside this callback, application is 
 **				 expected to initialize its remote retransmission manager.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaInitRemoteRetransTables _ARGS_\
	((Sdf_st_error *pErr));


/****************************************************************************
 ** Function:  sdf_cbk_uaGetRemoteRetransHashTable
 **
 ** Description: This callback is issued when UATK has to add a remote 
 **				retransmission entry (if UATK has been compiled with 
 **				SDF_APP_CONTROLLED_REMOTE_RETX flag.
 **				
 **				This flag must be used if the application use its on remote
 **				retransmission logic. Inside this callback, application is 
 **				expected to retrieve appropriate remote retransmission entry
 **				has table and return in pRemoteRetransHash parameter. UATK
 **				will then insert the remote retransmission entry into this
 **				hash table. If the application want's to completely override
 **				UATK's remote retransmission functionality, it has to return
 **				Sdf_co_null in pRemoteRetransHash.
 **
 ****************************************************************************/

Sdf_ty_retVal sdf_cbk_uaGetRemoteRetransHashTable _ARGS_\
	((Sdf_st_hash **ppRemoteRetransHash, Sdf_st_eventContext *pContext, \
	Sdf_st_error *pErr));

Sdf_ty_retVal sdf_cbk_uaFreeRemoteRetransTables _ARGS_\
	((Sdf_st_error *pErr));



#endif

#ifdef SDF_TEL
/****************************************************************************
 ** FUNCTION: sdf_cbk_uaGetIpFromTelUrl
 **
 ** DESCRIPTION: This Callback resolves a TelUrl into a valid IP and port.It
 **				 fills up the pDestInfo structure passed to it.
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaGetIpFromTelUrl _ARGS_((\
	Sdf_st_callObject *pCallobj, TelUrl *pTelUrl, \
	Sdf_st_transportInfo *pDestInfo, Sdf_st_error *pErr));
#endif

#ifdef SDF_IM
/****************************************************************************
 ** FUNCTION: sdf_cbk_uaResolveImUrl
 **
 ** DESCRIPTION: This Callback resolves a IM url into the Other schemes.
 **				  Application need to allocate memory to ppReslUrl and put
 **				 the resolved url as string.
 **	Note:	Ex: sip:hssua@sarovar:11111, or 
 **				tel:+12121				 or
 **				sips:hssua@sarovar:22222
 **
 ****************************************************************************/

extern Sdf_ty_retVal sdf_cbk_uaResolveImUrl
( Sdf_ty_s8bit	*pImUrl, Sdf_ty_s8bit **ppReslUrl, Sdf_st_error *pError);

/****************************************************************************
 ** FUNCTION: sdf_cbk_uaGetIpFromImUrl
 **
 ** DESCRIPTION: This Callback resolves a IM URL into IP and port .
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetIpFromImUrl _ARGS_((Sdf_st_callObject *pCallobj,
 ImUrl *pImUrl, Sdf_st_transportInfo *pDestInfo, Sdf_st_error *pErr));

#endif
#ifdef SDF_PRES
/****************************************************************************
 ** FUNCTION: sdf_cbk_uaResolvePresUrl
 **
 ** DESCRIPTION: This Callback resolves a Pres url into the Other schemes.
 **				  Application need to allocate memory to ppReslUrl and put
 **				 the resolved url as string.
 **	Note:	Ex: sip:hssua@sarovar:11111, or 
 **				tel:+12121				 or
 **				sips:hssua@sarovar:22222
 **
 ****************************************************************************/

extern Sdf_ty_retVal sdf_cbk_uaResolvePresUrl
( Sdf_ty_s8bit	*pPresUrl, Sdf_ty_s8bit **ppReslUrl, Sdf_st_error *pError);

/****************************************************************************
 ** FUNCTION: sdf_cbk_uaGetIpFromPresUrl
 **
 ** DESCRIPTION: This Callback resolves a PRES URL into IP and port .
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetIpFromPresUrl _ARGS_((Sdf_st_callObject *pCallobj,
 PresUrl *pPresUrl, Sdf_st_transportInfo *pDestInfo, Sdf_st_error *pErr));

#endif
/***********************************************************************
** FUNCTION:  sdf_cbk_uaUnexpectedRequestReceived
**
** DESCRIPTION: Callback issued onto the application from sdf_ivk_uaHandleCall
**				if message contains a TO tag. The application is expected to
**				send 481 from inside this callback or reconstruct the 
**				call-object and proceed with the call (i.e. crash recovery)
**
** PARAMETERS:
**		ppCallObj(IN)	: Call Object having the SIP message
**		pEventContext(IN): The UA Toolkit Sdf_st_eventContext structure 
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaUnexpectedRequestReceived  _ARGS_(( \
	Sdf_st_callObject  **ppCallObj, Sdf_st_eventContext *pEventContext, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr));


#ifdef SDF_TXN

/****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaSendMsgOnNetwork
 **
 ** DESCRIPTION: 
 **
 ** PARAMETERS:
 **
 ****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaSendMsgOnNetwork 
	_ARGS_((Sdf_st_callObject *pCallObj, Sdf_ty_s8bit *pBuffer, \
	Sdf_ty_u32bit dBuflen, SipTranspAddr *pTranspaddr, \
	Sdf_ty_protocol dTransptype, Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION:		sdf_cbk_uaIndicateTxnTimeOut
 **
 ** DESCRIPTION:	This callback is issued by the toolkit when 
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaIndicateTxnTimeOut _ARGS_( \
	(Sdf_st_callObject *pCallObj, Sdf_st_overlapTransInfo *pOverlapTxn, \
	 Sdf_ty_txnTimerType dTimerType, Sdf_ty_pvoid pContext, \
	 Sdf_st_error *pError));

#endif /* End -ifndef SDF_TXN */
#ifdef SDF_APPCONTROLLEDMEMPOOL
Sdf_ty_retVal	sdf_cbk_uaInitPreAllocatedBuffers( Sdf_st_error *pErr);

Sdf_ty_retVal	sdf_cbk_uaDestroyPreAllocatedBuffers(Sdf_st_error *pErr);

Sdf_ty_retVal	sdf_cbk_uaFreePreallocBuffers(Sdf_ty_pvoid *ppData, Sdf_ty_u32bit dMemPoolIdx);

Sdf_ty_retVal	sdf_cbk_uaGetDirectBufMemPool(Sdf_st_mempool **ppPreAllocBuf, Sdf_st_eventContext *pEventContext, Sdf_st_error *pError);

#endif
#ifdef SDF_CONGEST
extern Sdf_ty_retVal sdf_cbk_uaFormResourcePriority(Sdf_st_callObject *pCallObj,
        SipMessage *pSipMsg,Sdf_st_error *pErr);
#endif
#ifdef SDF_TLS

/**********************************************************
** FUNCTION:  sdf_cbk_uaGetPrivateKeyPassword
**
** DESCRIPTION: This function will set the password in the output parameter 
**              buf and return the length of the password. While constructing 
**              the SSL_CTX, UATK would have set the password as the callback 
**              userdata using the API SSL_CTX_set_default_passwd_cb_userdata. 
**              The same userdata will be provided as an input argument to 
**              this callback function.
**
**********************************************************/
Sdf_ty_s32bit sdf_cbk_uaGetPrivateKeyPassword _ARGS_((Sdf_ty_s8bit *buf, \
	 Sdf_ty_s32bit num, Sdf_ty_s32bit rwflag, Sdf_ty_pvoid  pUserData));

/**********************************************************
** FUNCTION:  sdf_cbk_uaVerifyCeritificate
**
** DESCRIPTION: The SSL layer would have done its part of verifications and 
**              set the result in preverifyOk. Extra verifications can be 
**              done by the application in this function.
**
**********************************************************/
Sdf_ty_s32bit sdf_cbk_uaVerifyCeritificate _ARGS_((\
	 Sdf_ty_s32bit  preverifyOk, X509_STORE_CTX *ctx));

/***********************************************************************
** FUNCTION:  sdf_cbk_uaTLSConnectionEstdNtfn
**
** DESCRIPTION: This is a notification callback thrown on the application 
**              to notify that a new TLS connection has been established. 
**
** PARAMETERS:
** 		dSockfd(IN)	: The Socket Descriptor. 
** 		pSockInfo(IN)	: The socket related information corresponding to
**                        dSockfd.
**	 	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTLSConnectionEstdNtfn _ARGS_((\
	Sdf_ty_socketId	dSockfd, Sdf_st_sockInfo	*pSockInfo, \
	Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_cbk_uaTLSConnectionClosedNtfn
**
** DESCRIPTION: This is a notification callback thrown on the application 
**              to notify that an existing TLS connection has been closed. 
**
** PARAMETERS:
** 		dSockfd(IN)	: The Socket Descriptor that was closed. 
**	 	pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTLSConnectionClosedNtfn _ARGS_((\
	Sdf_ty_socketId	dSockfd, Sdf_st_error *pErr));

#endif /* SDF_TLS */



/******************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaGetHostIp
 **
 ** DESCRIPTION: This function should resolves the provided hostname into an
 **				 IP address and returns the first IP address resolved 
 **				 as a string.
 **
 ******************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaGetHostIp
	_ARGS_((Sdf_ty_s8bit* pHost, Sdf_ty_s8bit** ppDest, 
		Sdf_st_error *pError));

#ifdef SDF_NETWORKCALLBACKS

/******************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaGetHostName
 **
 ** DESCRIPTION: This function should returns the machines local host name. 
 **
 ******************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaGetHostName
	_ARGS_((Sdf_ty_s8bit **ppHost, Sdf_st_error *pError));


/*****************************************************************************
** FUNCTION: sdf_cbk_uaIsConnectedTcpSocket
**
** DESCRIPTION: The function should return whether the socket is connected to
**				the other end or not.
**
******************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaIsConnectedTcpSocket
	_ARGS_((Sdf_ty_s32bit dSockfd, Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaCloseSocket
 **
 ** DESCRIPTION: The function should close a network socket.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaCloseSocket
	_ARGS_((Sdf_st_socket *pSocket));	
	
	
/*****************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaSocketCreate
 **
 ** DESCRIPTION: The function should create a new network socket.
 **
 *****************************************************************************/
extern Sdf_ty_socketId sdf_cbk_uaSocketCreate
	_ARGS_((Sdf_ty_s32bit dDomain, Sdf_ty_s32bit dType, \
		Sdf_ty_s32bit dProtocol));

	
/*****************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaConnectSocket
 **
 ** DESCRIPTION: The function should open a new network connection.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaConnectSocket
	_ARGS_((Sdf_st_socket *pSocket, Sdf_ty_pvoid  pName, \
		Sdf_ty_s32bit dNameLength, Sdf_ty_pvoid  pExtraArgs));	


/*****************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaBindSocket
 **
 ** DESCRIPTION: The function should bind a socket to a specified port.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaBindSocket
	_ARGS_((Sdf_ty_socketId dSocket, Sdf_ty_pvoid  pName, \
		Sdf_ty_s32bit dNameLength));	


/*****************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaListenSocket
 **
 ** DESCRIPTION: The function should set the socket in listen mode for TCP 
 **				 connections.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaListenSocket
	_ARGS_((Sdf_ty_socketId dSocket, \
		Sdf_ty_s32bit dMaxPendingConnections));


/*****************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaSetSockOpt
 **
 ** DESCRIPTION: The function should set socket options.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_cbk_uaSetSockOpt
	_ARGS_((Sdf_ty_socketId dSocket, Sdf_ty_s32bit  dLevel, \
	Sdf_ty_s32bit  dOptName, Sdf_ty_pvoid   pOptVal, \
	Sdf_ty_s32bit dOptLen));

	
#endif /* endif for SDF_NETWORKCALLBACKS */
	
#ifdef SDF_LOOKUP
/***********************************************************************
** FUNCTION: sdf_cbk_uaSendCallToPeer 
**
** DESCRIPTION: This callback is thrown after UATK forms the message.
** 				 The application has to perform lookup and send the message
**				 on the Network.
**
** PARAMETERS:
**	pObject(IN)		 	: Call Object which contains the SIP message to be sent
**	pSipMsg(IN)		 	: The SipMessage to be sent out.
**
**	dOptions(IN)	 	: The options to be used for forming the text
**							message. Multiple options can be ORed. The 
**							supported. Options are:
**							SIP_OPT_SINGLE: Each header is formed in a 
**								separate line. The message will not contain 
**								comma separated headers. Cannot be used with 
**								SIP_OPT_COMMASEPARATED.
**							SIP_OPT_COMMASEPARATED: Multiple instances of the 
**								same kind of header will appear on the same 
**								header line separated by commas. Cannot be 
**								used with SIP_OPT_SINGLE.
**							SIP_OPT_SHORTFORM: Single-letter short forms will 
**								be used for headers	that have short names.
**								Cannot be used with SIP_OPT_SULLFORM.
**							SIP_OPT_FULLFORM: All headernames will appear in 
**								full. Cannot be used with SIP_OPT_SHORTFORM.
**							SIP_OPT_CLEN: A Content-Length header with the 
**								correct length is inserted in the message 
**								being formed. If the message structure has a 
**								Content-Length header, the function corrects 
**								the length field depending on size of the 
**								message body.
**							SIP_OPT_REORDERHOP: All hop to hop headers will be 
**								placed above the end to end headers.
**							SIP_OPT_AUTHCANONICAL: All headers after the 
**								Authorization header will be formed in the 
**								canonical form. This will override options 
**								SIP_OPT_SHORTFORM and SIP_OPT_COMMASEPARATED 
**								from the Authorization header onwards.
**
**		transptype(IN)	: The transport to be used for transmitting the 
**							SIP text message. Possible values are
**							SDF_NORETRANS: This option disables the stack 
**								retransmission mechanism for the message.
**		pContext(IN)	: The application may pass any information in this
**							structure through its pData (void pointer) 
**							element. This structure is passed to the 
**							application in the Time-out indicate function. 
**							If NULL is passed in this parameter, the Toolkit 
**							creates an event context structure and attaches 
**							the call Object's pAppData pointer as the 
**							context's data.
**		pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_cbk_uaSendCallToPeer  _ARGS_((Sdf_st_callObject  \
		*pObject,Sdf_st_overlapTransInfo *pOverlapTransInfo,\
		SipMessage *pSipMsg, SipOptions dOptions, \
		Sdf_ty_s8bit transptype, Sdf_st_eventContext *pContext, \
		Sdf_st_error *pErr));
 #ifdef SDF_ASYNC_DNS
 /*********************************************************************
 ** FUNCTION:  sdf_cbk_QueryRespProcs()
 **
 ** DESCRIPTION: This function sends out the SIP message and performs  
 **              appln post query resp procs
 *********************************************************************/
Sdf_ty_retVal sdf_cbk_uaQueryRespProcs(
	Sdf_st_callObject      *pCallObj,
	Sdf_st_eventContext *pEventContext, 
	Sdf_st_overlapTransInfo *pOverlapTransaction,
	en_SipMessageType    dType,
	Sdf_st_error		   *pErr);
#endif /*SDF_ASYNC_DNS*/
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
#endif
