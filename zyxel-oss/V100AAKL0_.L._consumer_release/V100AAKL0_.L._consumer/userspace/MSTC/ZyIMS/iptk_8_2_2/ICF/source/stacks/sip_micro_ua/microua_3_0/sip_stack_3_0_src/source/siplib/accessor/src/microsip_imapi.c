/*******************************************************************************
** FUNCTION:
**			IM URL API Accessor functionality
********************************************************************************
**
** FILENAME:
** microsip_imapi.c
**
** DESCRIPTION:
**		This file contains functions for getting and setting fields of IMURL
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
**
**
**	COPYRIGHT , Aricent, 2006
*******************************************************************************/

#include "microsip_imapi.h"
#include "microsip_imurlfree.h"
#include "microsip_imurlinit.h"

/***********************************************************************
** Function: sip_getDispNameFromImUrl
** Description: gets the Display Name field from the ImUrl structure
** Parameters:
**		pUrl (IN)		- ImUrl
**		ppDispName (OUT)- retrieved Display Name
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getDispNameFromImUrl
	(ImUrl *pUrl,
	SIP_S8bit **ppDispName,
	SipError *pErr)
{
	SIPDEBUGFN("Entering sip_getDispNameFromImUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (( pUrl == SIP_NULL) || (ppDispName == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (pUrl->pDispName == SIP_NULL)
	{
		*pErr=E_NO_EXIST;
		return SipFail;
	}

	*ppDispName = pUrl->pDispName;

	SIPDEBUGFN("Exiting sip_getDispNameFromImUrl\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setDispNameInImUrl
** Description: sets the Display Name in the ImUrl structure
** Parameters:
**		pUrl (IN/OUT)	- ImUrl
**		pDispName (OUT)	- Display Name to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setDispNameInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pUrl,
	SIP_S8bit *pDispName,
	SipError *pErr)
#else
	(pUrl,pDispName,pErr)
	ImUrl *pUrl;
	SIP_S8bit *pDispName;
	SipError *pErr;
#endif
{
	SIP_S8bit *pTemp_disp_name=SIP_NULL;
	SIP_S8bit *pName=SIP_NULL;

	SIPDEBUGFN("Entering sip_setDispNameInImUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if( pDispName == SIP_NULL)
		pTemp_disp_name = SIP_NULL;
	else
	{
		pTemp_disp_name = pDispName;
	}

	pName = pUrl->pDispName;
	if (pName != SIP_NULL)
	{
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pName), pErr) == \
			SipFail)
		{
			return SipFail;
		}
	}

	pUrl->pDispName = pTemp_disp_name;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting sip_setDispNameInImUrl\n");
	return SipSuccess;
}


/***********************************************************************
** Function: sip_getUserFromImUrl
** Description: gets the User Name field from the ImUrl structure
** Parameters:
**		pUrl (IN)		- ImUrl
**		ppUser (OUT)	- retrieved User
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getUserFromImUrl
#ifdef ANSI_PROTO
	(ImUrl *pUrl,
	SIP_S8bit **ppUser,
	SipError *pErr)
#else
	(pUrl,ppUser,pErr)
	ImUrl *pUrl;
	SIP_S8bit **ppUser;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_getUserFromImUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (( pUrl == SIP_NULL) || (ppUser  == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (pUrl->pUser == SIP_NULL)
	{
		*pErr=E_NO_EXIST;
		return SipFail;
	}

	*ppUser = pUrl->pUser;

	SIPDEBUGFN("Exiting sip_getUserFromImUrl\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setUserInImUrl
** Description: sets the User in the ImUrl structure
** Parameters:
**		pUrl (IN/OUT)	- ImUrl
**		pUser (OUT)		- User to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setUserInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pUrl,
	SIP_S8bit *pUser,
	SipError *pErr)
#else
	(pUrl,pUser,pErr)
	ImUrl *pUrl;
	SIP_S8bit *pUser;
	SipError *pErr;
#endif
{
	SIP_S8bit *pTemp_user_name=SIP_NULL;
	SIP_S8bit *pUsertemp=SIP_NULL;

	SIPDEBUGFN("Entering sip_setUserInImUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if( pUser == SIP_NULL)
		pTemp_user_name = SIP_NULL;
	else
	{
		pTemp_user_name = pUser;
	}

	pUsertemp = pUrl->pUser;
	if (pUsertemp != SIP_NULL)
	{
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pUsertemp), pErr) == \
			SipFail)
		{
			return SipFail;
		}
	}

	pUrl->pUser = pTemp_user_name;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting sip_setUserInImUrl\n");
	return SipSuccess;
}


/***********************************************************************
** Function: sip_getHostFromImUrl
** Description: gets the Host Name field from the ImUrl structure
** Parameters:
**		pUrl (IN)		- ImUrl
**		ppHost (OUT)	- retrieved Host
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getHostFromImUrl
#ifdef ANSI_PROTO
	(ImUrl *pUrl,
	SIP_S8bit **ppHost,
	SipError *pErr)
#else
	(pUrl,ppHost,pErr)
	ImUrl *pUrl;
	SIP_S8bit **ppHost;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_getHostFromImUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (( pUrl == SIP_NULL) || (ppHost  == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (pUrl->pHost == SIP_NULL)
	{
		*pErr=E_NO_EXIST;
		return SipFail;
	}

	*ppHost = pUrl->pHost;

	SIPDEBUGFN("Exiting sip_getHostFromImUrl\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setHostInImUrl
** Description: sets the Host in the ImUrl structure
** Parameters:
**		pUrl (IN/OUT)	- ImUrl
**		pHost (OUT)		- Host to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setHostInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pUrl,
	SIP_S8bit *pHost,
	SipError *pErr)
#else
	(pUrl,pHost,pErr)
	ImUrl *pUrl;
	SIP_S8bit *pHost;
	SipError *pErr;
#endif
{
	SIP_S8bit *pTemp_host_name=SIP_NULL;
	SIP_S8bit *pHosttemp=SIP_NULL;

	SIPDEBUGFN("Entering sip_setHostInImUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if( pHost == SIP_NULL)
		pTemp_host_name = SIP_NULL;
	else
	{
		pTemp_host_name = pHost;
	}

	pHosttemp = pUrl->pHost;
	if (pHosttemp != SIP_NULL)
	{
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pHosttemp), pErr) == \
			SipFail)
		{
			return SipFail;
		}
	}

	pUrl->pHost = pTemp_host_name;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting sip_setHostInImUrl\n");
	return SipSuccess;
}


/***********************************************************************
** Function: sip_getParamCountFromImUrl
** Description: gets the number of parameters in ImUrl
** Parameters:
**		pImUrl (IN)		- ImUrl
**		pCount (OUT)	- number of parameters
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getParamCountFromImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl,SIP_U32bit *pCount, SipError *pErr)
#else
 	(pImUrl,pCount,pErr)
	ImUrl *pImUrl;
 	SIP_U32bit *pCount;
 	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_getParamCountFromImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pImUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pCount == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf( &(pImUrl)->slParams, pCount , pErr) == SipFail )
	{
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getParamCountFromImUrl");
	return SipSuccess;
}

/*****************************************************************************
** Function: sip_getParamAtIndexFromImUrl
** Description: gets the Param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN)		- ImUrl
**	ppParam(OUT)	- retreived Parameter
**	dIndex (IN)		- index at which param is to be retieved
**	pErr (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/

SipBool sip_getParamAtIndexFromImUrl
	(ImUrl *pImUrl,SipParam **ppParam, SIP_U32bit dIndex,\
		SipError *pErr)
{
	SIP_Pvoid element_from_list;
	SipParam *pTemp_param=SIP_NULL;
	SIPDEBUGFN("Entering function sip_getParamAtIndexFromImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pImUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (ppParam == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif

	if (sip_listGetAt( &(pImUrl->slParams), dIndex,  \
		&element_from_list, pErr) == SipFail)
		return SipFail;

	if (element_from_list == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	pTemp_param = (SipParam *)element_from_list;

	HSS_LOCKEDINCREF(pTemp_param->dRefCount);
	*ppParam = pTemp_param;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getParamAtIndexFromImUrl");
	return SipSuccess;
}

/*****************************************************************************
** Function: sip_setParamAtIndexInImUrl
** Description: sets the Param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN/OUT)		- ImUrl
**	pParam(IN)			- Param to be set
**	dIndex (IN)			- index at which param is set in ImUrl
**	pErr (OUT)			- Possible Error value (see API ref doc)
******************************************************************************/

SipBool sip_setParamAtIndexInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl,SipParam *pParam, SIP_U32bit dIndex, \
		SipError *pErr)
#else
	(pImUrl,pParam, dIndex, pErr)
	ImUrl *pImUrl;
	SipParam *pParam;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	SipParam *pTemp_param=SIP_NULL;
	SIPDEBUGFN("Entering function sip_setParamAtIndexInImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pImUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pParam == SIP_NULL )
		pTemp_param = SIP_NULL;
	else
	{
		HSS_LOCKEDINCREF(pParam->dRefCount);
		pTemp_param = pParam;
	}

	if( sip_listSetAt( &(pImUrl->slParams),  \
		dIndex, (SIP_Pvoid)(pTemp_param), pErr) == SipFail)
	{
		if (pTemp_param != SIP_NULL)
		    HSS_LOCKEDDECREF(pParam->dRefCount);
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setParamAtIndexInImUrl");
	return SipSuccess;
}
/******************************************************************************
** Function: sip_insertParamAtIndexInImUrl
** Description: inserts the Param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN/OUT)	- ImUrl
**	pParam(IN)		- Param to be inserted
**	dIndex (IN)		- index at which param is inserted in ImUrl
**	pErr (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/

SipBool sip_insertParamAtIndexInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl, SipParam *pParam, SIP_U32bit dIndex, \
		SipError *pErr)
#else
	(pImUrl, pParam, dIndex, pErr)
	ImUrl *pImUrl;
	SipParam *pParam;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	SipParam *pTemp_param=SIP_NULL;
	SIPDEBUGFN("Entering function sip_insertParamAtIndexInImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pImUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	if ( pParam == SIP_NULL )
		pTemp_param = SIP_NULL;
	else
	{
		HSS_LOCKEDINCREF(pParam->dRefCount);
		pTemp_param = pParam;
	}

	if( sip_listInsertAt( &(pImUrl->slParams),  \
		dIndex, (SIP_Pvoid)(pTemp_param), pErr) == SipFail)
	{
		if (pTemp_param != SIP_NULL)
            HSS_LOCKEDDECREF(pParam->dRefCount);   
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_insertParamAtIndexInImUrl");
	return SipSuccess;
}

/***********************************************************************
** Function: sip_deleteParamAtIndexInImUrl
** Description: deletes the param at the specified index in ImUrl
** Parameters:
**	pImUrl (IN)		- ImUrl
**	dIndex (IN)		- index at which param is deleted in ImUrl
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_deleteParamAtIndexInImUrl
#ifdef ANSI_PROTO
(ImUrl *pImUrl, SIP_U32bit dIndex, SipError *pErr)
#else
(pImUrl, dIndex, pErr)
ImUrl *pImUrl;
SIP_U32bit dIndex;
SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_deleteParamAtIndexInImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pImUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	if( sip_listDeleteAt( &(pImUrl->slParams), dIndex, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_deleteParamAtIndexInImUrl");
	return SipSuccess;
}


/***************************************************************
** Function: sip_getRouteCountFromImUrl
** Description: gets the Route count from the ImUrl Structure
** Parameters:
**	pImUrl (IN)		- ImUrl
**	pCount (OUT)	- number of ImUrl Routes
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_getRouteCountFromImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl,
	SIP_U32bit *pCount,
	SipError *pErr)
#else
	(pImUrl, pCount, pErr)
	ImUrl *pImUrl;
	SIP_U32bit *pCount;
	SipError *pErr;
#endif
{
	SIPDEBUGFN ( "Entering getRouteCountFromImUrl");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
		return SipFail;

	if ( (pImUrl == SIP_NULL) || ( pCount == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf(&(pImUrl->slRoute), pCount , pErr) == SipFail )
	{
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting getRouteCountFromImUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_getRouteAtIndexFromImUrl
** Description: gets the Route field at the index from ImUrl
** Parameters:
**	pImUrl (IN)		- ImUrl
**	ppRoute(OUT)	- retreived Route
**	dIndex (IN)		- index at which Route is to be retrieved
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_getRouteAtIndexFromImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl,
	 SIP_S8bit **ppRoute,
	 SIP_U32bit dIndex,
	 SipError 	*pErr )
#else
	(pImUrl,ppRoute,dIndex,pErr)
	 ImUrl *pImUrl;
	 SIP_S8bit **ppRoute;
     SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{
	SIP_Pvoid pElementFromList;

	SIPDEBUGFN ( "Entering sip_getRouteAtIndexFromImUrl ");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if ( (pImUrl == SIP_NULL) || (ppRoute == SIP_NULL) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listGetAt(&(pImUrl->slRoute), dIndex, &pElementFromList, \
		pErr) == SipFail)
		return SipFail;

	if ( pElementFromList == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	*ppRoute = (SIP_S8bit*)pElementFromList;
	SIPDEBUGFN ( "Exitting sip_getRouteAtIndexFromImUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setRouteAtIndexInImUrl
** Description: sets the Route field at the index in ImUrl
** Parameters:
**	pImUrl (IN/OUT)		- ImUrl
**	pRoute(IN)			- Route to be set
**	dIndex (IN)			- index at which Route is to be set
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool  sip_setRouteAtIndexInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl,
	 SIP_S8bit *pRoute,
	 SIP_U32bit dIndex,
	 SipError 	*pErr )
#else
	(pImUrl,pRoute,dIndex,pErr)
	 ImUrl *pImUrl;
	 SIP_S8bit *pRoute;
     SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{

	SIP_S8bit * pElementFromList=SIP_NULL;

	SIPDEBUGFN ( "Entering sip_setRouteAtIndexInImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pImUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pRoute  == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
		pElementFromList = pRoute;
	}

	if( sip_listSetAt(&(pImUrl->slRoute), dIndex, pElementFromList, \
		pErr) == SipFail)
	{
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting sip_setRouteAtIndexInImUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_insertRouteAtIndexInImUrl
** Description: inserts the Route field at the index in ImUrl
** Parameters:
**	pImUrl (IN/OUT)		- ImUrl
**	pRoute(IN)			- Route to be inserted
**	dIndex (IN)			- index at which Route is to be inserted
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_insertRouteAtIndexInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl,
	 SIP_S8bit *pRoute,
	 SIP_U32bit	dIndex,
	 SipError 	*pErr )
#else
	(pImUrl,pRoute,dIndex,pErr)
	 ImUrl *pImUrl;
	 SIP_S8bit *pRoute;
     SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{

	SIP_S8bit * pElementFromList=SIP_NULL;

	SIPDEBUGFN ( "Entering sip_insertRouteAtIndexInImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pImUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* copying the Value Headers structure/char*  */
	if (pRoute  == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
		pElementFromList = pRoute	;
	}

	if( sip_listInsertAt(&(pImUrl->slRoute), dIndex, pElementFromList,\
		pErr) == SipFail)
	{
		if ( pElementFromList != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pElementFromList)), \
				pErr);
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting sip_insertRouteAtIndexInImUrl");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_deleteRouteAtIndexInImUrl
** Description: deletes the Route field at the dIndex in ImUrl
** Parameters:
**	pImUrl (IN/OUT)		- ImUrl
**	pRoute(IN)			- Route to be deleted
**	dIndex (IN)			- index at which Route is to be deleted
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_deleteRouteAtIndexInImUrl
#ifdef ANSI_PROTO
	(ImUrl *pImUrl,
	  SIP_U32bit dIndex,
	  SipError 	*pErr )
#else
	(pImUrl,dIndex,pErr)
  	ImUrl *pImUrl;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{

	SIPDEBUGFN ( "Entering sip_deleteRouteAtIndexInImUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pImUrl  == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt(&(pImUrl->slRoute), dIndex, pErr) == SipFail)
		return SipFail;

	SIPDEBUGFN ( "Exitting sip_deleteRouteAtIndexInImUrl");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


