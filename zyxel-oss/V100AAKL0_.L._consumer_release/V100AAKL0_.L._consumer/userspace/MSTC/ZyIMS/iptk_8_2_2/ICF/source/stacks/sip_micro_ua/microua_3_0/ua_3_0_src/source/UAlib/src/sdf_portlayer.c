
/*****************************************************************************
 ** FUNCTION:	Porting Layer code - has all memory related and 
 **				other common utils
 **
 *****************************************************************************
 **
 ** FILENAME	: 	sdf_portlayer.c
 **
 ** DESCRIPTION	:	This file contains code to free all structures
 **
 ** DATE        NAME              REFERENCE           REASON
 ** ----        ----              ---------          --------
 ** 28/02/01   Rajasri			Initial Creation    Move all porting 
 **													functions to one file
 **
 ** 31/120/01  Srikanth.S							Ported for win-32 OS
 ** 22/04/03   Anil Kumar            ---            Changes for TLS feature.
 ** 14/11/08   Anuradha Gupta      SPR 19188       Rel 7.1.2 SPR Merging (SPR 19010)
 ** 29/01/09   Alok Tiwari      ICF Rel-8.1       Changes done for 
 **                               flag ICF_UATK_PORT.This flag filters
 **                               the ICF related code/changes from this file.
 ** 07-Feb-09  Alok Tiwari      ICF Rel-8.1       Code review comments disposed.
 ** 26-May-09 Ashutosh Mohan  SPR 19672         Modified
 **                                             sdf_fn_uaGetDeltaSecondsForRetryAfter
 ** 12-Jun-09  Anuradha Gupta    SPR 19737           Klocworks warning removal
 ** 11-Jun-2009 Ashutosh Mohan   SPR 19737     Changes done for GCC 4.1 warnings removal
 ** 22-Jun-2009 Abhishek Dhammawat SPR 19769   Modified
 **                                            sdf_fn_uaDisplayFnTrace
 ** 13-Jul-2009 Rajiv Kumar        IPTK Rel8.2 Openssl Removal
 ** 16-Jul-2009 Rajiv Kumar      IPTK Rel 8.2  Vxwork 5.4 Porting
 ** 20-Aug-2009 Anuradha Gupta   SPR 19935     Snprintf implementation modified
 ** 26-Jul-2009 Rajiv Kumar      SPR 19991     Fix For SPR: 19991
 ** 29-Apr-2010 Rohit Aggarwal  CSR 1-8534364    Make snprintf OS independent
 **
 *****************************************************************************
 **     COPYRIGHT , Aricent, 2010
 *****************************************************************************/


#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

#if !defined(SDF_VXWORKS) && !defined(SDF_WINDOWS) && !defined(ICF_PORT_WINCE)
#ifndef SDF_LINT 
#include <sys/time.h>
#endif
#endif

#if defined(SDF_WINDOWS) || defined(ICF_PORT_WINDOWS)
#ifndef ICF_PORT_WINCE
#include <sys/timeb.h>
#include <process.h>
#endif
#include <windows.h>
#endif
	

#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || defined(SDF_OSE)
#include <pthread.h>
#endif

#ifdef ICF_UATK_PORT
#if defined (ICF_PORT_SYMBIAN) 
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#endif

#if !defined(ICF_PORT_WINCE)
#include <signal.h>
#endif
#endif /*******ICF_UATK_PORT********/


#if defined (ICF_PORT_WINDOWS) && !defined(ICF_PORT_WINCE)
#include <sys/timeb.h>
#include  <IO.h>
#include <signal.h>
#include <errno.h>
#endif
#if defined (ICF_PORT_WINDOWS)
#include <winsock2.h>
#endif

#if !defined(SDF_VXWORKS)
#if defined (ICF_PORT_WINDOWS) && !defined(ICF_PORT_WINCE)
#include <errno.h>
#endif
#endif


#include "sdf_portlayer.h"
#ifndef SDF_LINT
#include "sdf_common.h"
#endif

#ifdef SDF_TLS
#include "openssl_wrapper.h"
#endif

#include "sdf_debug.h"
#include "sdf_reqresp.h"
#include "sdf_callbacks.h"
#include "sdf_internal.h"

/*Fix for openssl start*/
/* Various parameters which are used in open ssl calls are replaced with UATK
 * defined parameters.*/
#define OP_SINGLE_DH_USE				0x00100000L 
#define OP_ALL				        	0x00000FFFL
#define SDF_SSL_FILETYPE_PEM            1
#define SDF_SSL_FILETYPE_ASN1           2
#define SDF_SSL_SENT_SHUTDOWN           1
#define SDF_SSL_RECEIVED_SHUTDOWN       2
/*Fix for openssl end*/

#ifdef SDF_LOOKUP
#include "sdf_lookupport.h"
#ifdef SDF_THREAD_SAFE
#if defined(SDF_WINDOWS) || defined(SDF_WINCE)
extern Sdf_st_ThreadKey dThreadKey;
#else
/*extern pthread_key_t dThreadKey;*/
static pthread_key_t dThreadKey;
#endif
#endif
#endif

#ifdef ICF_UATK_PORT
#include "icf_port_defs.h"
#include "icf_port_prototypes.h"

#include "icf_common_types.h"
extern icf_glb_pdb_st	*p_persistent_glb_pdb;
#endif /***************ICF_UATK_PORT*********************/

#ifdef SDF_SESSION_TIMER
Sdf_ty_s32bit 	glbMinSE=10;
Sdf_ty_bool 	glbProfileSupportedTimer=Sdf_co_true;
#endif
Sdf_ty_u32bit	glbPid = 0;

#ifndef SDF_NETWORKCALLBACKS
extern Sdf_st_fdSetInfo	dGlbFdSet;
#endif

#if defined(SDF_WINDOWS) || defined(SDF_VXWORKS)
static const Sdf_ty_s8bit *
sdf_fn_uaInetNtoP4(
        const u_char *src,
        Sdf_ty_s8bit *dst,
        size_t size);
#endif

#ifdef ICF_PORT_SYMBIAN
/*#define FD_ZERO(x) memset(x, 0, sizeof(Sdf_ty_fdSet) ) */
void bzero(char* ptr, size_t  len);
#endif
#ifdef SDF_DEP_SYNC_HANDLING
Sdf_ty_retVal sdf_ivk_uaDepMgrFreeDialogUpdate _ARGS_((\
   Sdf_st_DialogUpdate *pDialogUpdate));
Sdf_ty_retVal sdf_ivk_uaDepFormAndSendNotify (
	Sdf_st_DialogUpdate *pUpdateElement,					
	Sdf_st_error        *pErr);
#endif
#ifdef SDF_DEP
/* For DEP, the slStateChangeQueue is the queue on which event changes for
 * any dialogs with valid subscriptions will be posted.This queue is
 * implemented as a list here.dStateChangeMutex is the mutex corrosponding 
 * to this queue.*/
Sdf_ty_mutexIdT dStateChangeMutex;
Sdf_ty_slist slStateChangeQueue;

/******************************************************************************
 ** FUNCTION: __sdf_fn_uaDepFreeDialogUpdate
 **
 ** DESCRIPTION: this function is used by list to free the elment.  
 **             element is Sdf_st_DialogUpdate structure
 **
 ** PARAMETERS:
 **   pDialogUpdate(IN): Void pointer
******************************************************************************/
Sdf_ty_void __sdf_fn_uaDepFreeDialogUpdate(void *pDialogUpdate);

#endif
/* ============================================================================
 * 
 * 				MEMORY ALLOCATION/DE-ALLOCATION APIs
 *
 * ========================================================================== */

/******************************************************************************
 ** FUNCTION:	 sdf_memget
 **
 ** DESCRIPTION: Same as sip_memget of the core stack - 
 **				 defined for convenience
 **
 ******************************************************************************/
Sdf_ty_pvoid sdf_memget
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit module_id, Sdf_ty_u32bit noctets, Sdf_st_error *pErr)
#else
	(module_id, noctets, pErr)
	Sdf_ty_u32bit module_id;
	Sdf_ty_u32bit noctets;
	Sdf_st_error *pErr;
#endif
{

#ifdef SDF_LINT
		if(pErr == Sdf_co_null)
 	     return ( sip_memget(module_id, noctets,Sdf_co_null));
		else
#endif
					 return ( sip_memget(module_id, noctets, (SipError*)&(pErr->errCode)));
}

/******************************************************************************
 ** FUNCTION:	 sdf_memfree
 **
 ** DESCRIPTION: Same as sip_memfree of the core stack - 
 **				 defined for convenience
 **
 ******************************************************************************/
Sdf_ty_bool sdf_memfree
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit module_id, Sdf_ty_pvoid* p_buf , Sdf_st_error *pErr)
#else
	(module_id, p_buf , pErr)
	Sdf_ty_u32bit module_id;
	Sdf_ty_pvoid* p_buf;
	Sdf_st_error *pErr;
#endif
{
    if ((p_buf == Sdf_co_null) ||
         (*p_buf == Sdf_co_null))
		return Sdf_co_true;
		
#ifdef SDF_LINT
		if(pErr == Sdf_co_null)
				return (Sdf_ty_bool)(sip_memfree(module_id,p_buf, \
															Sdf_co_null));

		else
#endif
	return (Sdf_ty_bool)(sip_memfree(module_id,p_buf, \
				(SipError*)&(pErr->errCode)));
}


/* ===========================================================================
 * 
 * 						 APIs USING TIME LIBRARY ROUTINES
 *
 * ========================================================================= */

/******************************************************************************
 ** FUNCTION: sdf_fn_uaGetCurrentTimeInMicroSecs
 **
 ** DESCRIPTION: This function returns the current time in Micro seconds 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetCurrentTimeInMicroSecs
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit* pMicroSeconds)
#else
	(pMicroSeconds)
	Sdf_ty_u32bit* pMicroSeconds;
#endif
{
#ifdef ICF_UATK_PORT
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || defined(ICF_PORT_SYMBIAN) 
	struct timeval tv;
#endif
	
#ifdef SDF_VXWORKS	
struct timespec             ts;
/*	Sdf_ty_s8bit *pTime = Sdf_co_null;
	char buffer[26];
	size_t buffer_len = 26;*/
#endif

#if defined(ICF_PORT_WINCE)
	SYSTEMTIME dSysTime,dSysTime70;
	FILETIME dFlTime,dFlTime70;
	__int64 dTime1,dTime2;

	dSysTime70.wYear = 1970;
	dSysTime70.wMonth = 1;
	dSysTime70.wDayOfWeek=4;
	dSysTime70.wDay=1;
	dSysTime70.wHour=0;
	dSysTime70.wMinute=0;
	dSysTime70.wSecond=0;
	dSysTime70.wMilliseconds=0;

	GetSystemTime(&dSysTime);
	SystemTimeToFileTime(&dSysTime, &dFlTime);
	SystemTimeToFileTime(&dSysTime70, &dFlTime70);

	dTime1 = dFlTime.dwHighDateTime;
	dTime1 <<= 32;
	dTime1 |= dFlTime.dwLowDateTime;

	dTime2 = dFlTime70.dwHighDateTime;
	dTime2 <<= 32;
	dTime2 |= dFlTime70.dwLowDateTime;

	*pMicroSeconds = ((unsigned int)(dTime1 - dTime2) / 10);

#else /* else for ICF_PORT_WINCE */

#ifdef ICF_PORT_WINDOWS
	struct _timeb	timebuffer;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "

			"sdf_fn_uaGetCurrentTimeInMicroSecs");

	_ftime( &timebuffer );

	*pMicroSeconds = timebuffer.millitm * 1000 + timebuffer.time * 1000000;

	return Sdf_co_success;

#else

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
			"sdf_fn_uaGetCurrentTimeInMicroSecs");
#ifdef SDF_VXWORKS
		
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	
	
#else
	if (gettimeofday(&tv, Sdf_co_null) == -1)
#endif
	{
		*pMicroSeconds = 0;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaGetCurrentTimeInMicroSecs");
		return  Sdf_co_fail;
	}
	else
	{
#ifdef SDF_VXWORKS
	/* nano sec * 1000 + sec * 1000000 == micro sec  */
	*pMicroSeconds = ts.tv_nsec * 1000 + ts.tv_sec * 1000000;

#else
		*pMicroSeconds = tv.tv_usec + tv.tv_sec * 1000000;
#endif
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_fn_uaGetCurrentTimeInMicroSecs");
		return Sdf_co_success;
	}
#endif /* end of flag port to windows */
#endif /* flag port on Win_Ce */
	return Sdf_co_success;

#else /*ifndef ICF_UATK_PORT */

#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
	struct timeval tv;
#endif	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_fn_uaGetCurrentTimeInMicroSecs");

#if !defined (SDF_WINDOWS) && !defined (SDF_WINCE)
	if (gettimeofday(&tv, Sdf_co_null) == -1)
	{
		*pMicroSeconds = 0;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaGetCurrentTimeInMicroSecs");
		return  Sdf_co_fail;
	}
	else
	{
		*pMicroSeconds = tv.tv_usec + tv.tv_sec * 1000000;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaGetCurrentTimeInMicroSecs");
		return Sdf_co_success;
	}
#else
	/* SDF_WINCE */
	dSysTime70.wYear = 1970;
	dSysTime70.wMonth = 1;
	dSysTime70.wDayOfWeek=4;
	dSysTime70.wDay=1;
	dSysTime70.wHour=0;
	dSysTime70.wMinute=0;
	dSysTime70.wSecond=0;
	dSysTime70.wMilliseconds=0;

	GetSystemTime(&dSysTime);
	SystemTimeToFileTime(&dSysTime, &dFlTime);
	SystemTimeToFileTime(&dSysTime70, &dFlTime70);
		
	dTime1 = dFlTime.dwHighDateTime;
	dTime1 <<= 32;
	dTime1 |= dFlTime.dwLowDateTime;

	dTime2 = dFlTime70.dwHighDateTime;
	dTime2 <<= 32;
	dTime2 |= dFlTime70.dwLowDateTime;

	*pMicroSeconds = ((unsigned int)(dTime1 - dTime2) / 10);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaGetCurrentTimeInMicroSecs");
	return Sdf_co_success;

#endif /* !defined (SDF_WINDOWS) && !defined (SDF_WINCE) */

#endif /***ICF_UATK_PORT***/
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetCurrentTimeInSecs
 **
 ** DESCRIPTION: This function returns the current time in seconds 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetCurrentTimeInSecs
#ifdef ANSI_PROTO
(Sdf_ty_u32bit* pSeconds)
#else
(pSeconds)
	Sdf_ty_u32bit* pSeconds = Sdf_co_null;
#endif
{
#ifdef ICF_UATK_PORT
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || defined(ICF_PORT_SYMBIAN) 
	struct timeval tv;
#endif

#ifdef SDF_VXWORKS	
struct timespec             ts;
/*	Sdf_ty_s8bit *pTime = Sdf_co_null;
	char buffer[26];
	size_t buffer_len = 26;*/
#endif
	
#if defined (ICF_PORT_WINCE)
	SYSTEMTIME dSysTime,dSysTime70;
	FILETIME dFlTime,dFlTime70;
	__int64 dTime1,dTime2;
#endif

#ifdef SDF_ERROR		
#ifdef SDF_LINT
	Sdf_st_error dErr={Sdf_en_LastUatkError};
#else
	Sdf_st_error dErr;
#endif
#endif	
#if defined (ICF_PORT_WINCE)	
	dSysTime70.wYear = 1970;
	dSysTime70.wMonth = 1;
	dSysTime70.wDayOfWeek=4;
	dSysTime70.wDay=1;
	dSysTime70.wHour=0;
	dSysTime70.wMinute=0;
	dSysTime70.wSecond=0;
	dSysTime70.wMilliseconds=0;

	GetSystemTime(&dSysTime);
	SystemTimeToFileTime(&dSysTime, &dFlTime);
	SystemTimeToFileTime(&dSysTime70, &dFlTime70);
		
	dTime1 = dFlTime.dwHighDateTime;
	dTime1 <<= 32;
	dTime1 |= dFlTime.dwLowDateTime;

	dTime2 = dFlTime70.dwHighDateTime;
	dTime2 <<= 32;
	dTime2 |= dFlTime70.dwLowDateTime;
	*pSeconds = ((unsigned int)(dTime1 - dTime2) / 10000000);
	return Sdf_co_success;

#else /* else for WIN_CE FLAG */

#ifdef ICF_PORT_WINDOWS
struct _timeb	timebuffer;
#ifdef SDF_ERROR
	(void)dErr;
#endif
	_ftime( &timebuffer );
	*pSeconds = timebuffer.time ;
	return Sdf_co_success;
#else	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaGetCurrentTimeInSecs");
#ifdef SDF_VXWORKS
		
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	
	
#else

	if (gettimeofday(&tv, Sdf_co_null) == -1)
#endif
	{
		*pSeconds = 0;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetCurrentTimeInSecs( ): "
			"Failed to get Current time", &dErr);
#endif		
		return  Sdf_co_fail;
	}
	else
	{
#ifdef SDF_VXWORKS
	*pSeconds =  ts.tv_sec;

#else
		*pSeconds =  tv.tv_sec;
#endif
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaGetCurrentTimeInSecs");
		return Sdf_co_success;
	}

#endif  /* end of WINWOWS FLAG */
#endif /* end of WIN_CE FLAG */

#else /*ifndef ICF_UATK_PORT*/

#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
	struct timeval tv;
#endif	


#ifdef SDF_ERROR		
	Sdf_st_error dErr;
#endif	

#if defined (SDF_WINCE)
	SYSTEMTIME dSysTime,dSysTime70;
	FILETIME dFlTime,dFlTime70;
	__int64 dTime1,dTime2;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaGetCurrentTimeInSecs");

#if !defined (SDF_WINDOWS) && !defined (SDF_WINCE)
	if (gettimeofday(&tv, Sdf_co_null) == -1)
	{
		*pSeconds = 0;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetCurrentTimeInSecs( ): "
			"Failed to get Current time", &dErr);
#endif		
		return  Sdf_co_fail;
	}
	else
	{
		*pSeconds =  tv.tv_sec;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaGetCurrentTimeInSecs");
		return Sdf_co_success;
	}
#else
	//SDF_WINCE
	dSysTime70.wYear = 1970;
	dSysTime70.wMonth = 1;
	dSysTime70.wDayOfWeek=4;
	dSysTime70.wDay=1;
	dSysTime70.wHour=0;
	dSysTime70.wMinute=0;
	dSysTime70.wSecond=0;
	dSysTime70.wMilliseconds=0;

	GetSystemTime(&dSysTime);
	SystemTimeToFileTime(&dSysTime, &dFlTime);
	SystemTimeToFileTime(&dSysTime70, &dFlTime70);
		
	dTime1 = dFlTime.dwHighDateTime;
	dTime1 <<= 32;
	dTime1 |= dFlTime.dwLowDateTime;

	dTime2 = dFlTime70.dwHighDateTime;
	dTime2 <<= 32;
	dTime2 |= dFlTime70.dwLowDateTime;

	*pSeconds = ((unsigned int)(dTime1 - dTime2) / 10000000);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_fn_uaGetCurrentTimeInSecs");
	return Sdf_co_success;

#endif /* !defined (SDF_WINDOWS) && !defined (SDF_WINCE) */

#endif /***ICF_UATK_PORT***/
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetTimeString
 **
 ** DESCRIPTION: This function returns the current time as a string
 **
 ******************************************************************************/
void sdf_fn_uaGetTimeString
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit **ppString)
#else
	(ppString)
	Sdf_ty_s8bit **ppString;
#endif
{
#ifdef ICF_UATK_PORT
#if defined (ICF_PORT_WINCE)
	SIP_U16bit *pDateStr,*pTimeStr,*pTempStr;
	SYSTEMTIME dSysTime;
	SIP_S32bit dLen;

	pDateStr = NULL;
	pTimeStr = NULL;
	pTempStr = NULL;


	GetLocalTime(&dSysTime);
			
	dLen = GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &dSysTime,_T("ddd MMM dd yyyy"),pDateStr,0 );
	pDateStr = (TCHAR *)malloc(dLen * sizeof(TCHAR));
	memset(pDateStr, 0, dLen * sizeof(TCHAR));
	GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &dSysTime,_T("ddd MMM dd yyyy"),pDateStr,dLen );
		
	dLen = 0;
	dLen = GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, &dSysTime, NULL, pTimeStr, 0 );
	pTimeStr = (TCHAR *)malloc(dLen * sizeof(TCHAR));
	memset(pTimeStr, 0, dLen * sizeof(TCHAR));
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, &dSysTime, NULL, pTimeStr, dLen );


	dLen = 0;
	dLen = wcslen(pDateStr) + wcslen(pTimeStr) + wcslen(_T(" ")) + wcslen(_T("\0")); 
	pTempStr = (TCHAR *)malloc(dLen * sizeof(TCHAR));
	memset(pTempStr, 0, dLen * sizeof(TCHAR));
	wcscpy(pTempStr,pDateStr);
	wcscat(pTempStr,_T(" "));
	wcscat(pTempStr,pTimeStr);
	
	*ppString = sip_wideCharToMultiByte(pTempStr);

	free(pDateStr);
	free(pTimeStr);
	free(pTempStr);
#else
#if defined(ICF_PORT_WINDOWS)

	/* this the code for windows */
	Sdf_ty_s8bit *pTime=Sdf_co_null;
	struct _timeb timebuffer;
	_ftime( &timebuffer );
    pTime = ctime( & ( timebuffer.time ) );
	Sdf_mc_strdup(*ppString, pTime);
/* the code ends here */
#endif
#endif
#if defined(ICF_PORT_SYMBIAN)
	struct timeval tv;
	Sdf_ty_s8bit buffer[SDF_MAX_CTIME_R_BUF_SIZE];
	Sdf_ty_s8bit *pTime = Sdf_co_null;

	gettimeofday(&tv,Sdf_co_null);
/*	#if (_POSIX_C_SOURCE - 0 >= 199506L) */
	pTime = ctime_r((const int*)&tv.tv_sec,buffer);

	Sdf_mc_strdup(*ppString, pTime);
#endif
#if defined(ICF_PORT_LINUX)
	struct timeval tv;
	Sdf_ty_s8bit *pTime = Sdf_co_null;
	gettimeofday(&tv,Sdf_co_null);
	pTime = ctime(&tv.tv_sec);
	Sdf_mc_strdup(*ppString, pTime);
#endif

#if defined(ICF_PORT_VXWORKS)
	struct timespec             ts;
	Sdf_ty_s8bit *pTime = Sdf_co_null;
	char buffer[26];
	size_t buffer_len = 26;
	
    	clock_gettime(CLOCK_REALTIME, &ts);
	pTime = ctime_r(&ts.tv_sec,buffer,&buffer_len);
	Sdf_mc_strdup(*ppString, pTime);
#endif

#else /*ifndef ICF_UATK_PORT */

	struct timeval tv;
	Sdf_ty_s8bit *pTime = Sdf_co_null;
	(void)gettimeofday(&tv,Sdf_co_null);
	pTime = ctime(&tv.tv_sec);
	Sdf_mc_strdup(*ppString, pTime);

#endif /***ICF_UATK_PORT***/

}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGenerateFixedTag
 **
 ** DESCRIPTION: This function generates a Fixed Tag which will be used 
 ** 			 in From & To Tag fields
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGenerateFixedTag
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit** ppGeneratedTag)
#else
	(ppGeneratedTag)
	Sdf_ty_s8bit** ppGeneratedTag;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
			"SDF_DEBUG - Entering sdf_fn_uaGenerateFixedTag");

	*ppGeneratedTag = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId, \
		Sdf_mc_strlen(Sdf_co_fixedTag) + 1, (Sdf_st_error *)Sdf_co_null);
	
    if(Sdf_co_null == *ppGeneratedTag)
	{
		return Sdf_co_fail;
	}

	Sdf_mc_strcpy(*ppGeneratedTag, Sdf_co_fixedTag);	

	sdf_fn_debug((Sdf_ty_s8bit *) \
			"SDF_DEBUG - Exiting sdf_fn_uaGenerateFixedTag");
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGenerateGlobalTag
 **
 ** DESCRIPTION: This function generates a random Tag which will be used 
 ** 			 in From & To Tag fields
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGenerateGlobalTag
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pConstTag, Sdf_ty_s8bit** ppGeneratedTag)
#else
	(pConstTag, ppGeneratedTag)
	Sdf_ty_s8bit*  pConstTag;
	Sdf_ty_s8bit** ppGeneratedTag;
#endif
{

	Sdf_ty_s8bit dTemp[Sdf_co_characterBufferSize];
	Sdf_ty_u32bit dMicroSecs = 0;
	Sdf_ty_u32bit dRand;
#ifdef SDF_ENCRYPT_API_TEST_IN_GLB_TAG
    Sdf_st_error dErr;
#endif

	*ppGeneratedTag=Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaGenerateGlobalTag");

	/*w534*/ (void)sdf_fn_uaGetCurrentTimeInMicroSecs(&dMicroSecs);
	/*w534*/ (void)sdf_fn_uaGenerateRand(&dRand);
	/*w534*/ (void)sdf_fn_uaSprintf(dTemp, "%s_%lu-%d", pConstTag, dMicroSecs, dRand);

	*ppGeneratedTag = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId, \
		Sdf_mc_strlen(dTemp) + 1, (Sdf_st_error *)Sdf_co_null);
	if(Sdf_co_null == *ppGeneratedTag)
	{
		return Sdf_co_fail;
	}

	Sdf_mc_strcpy(*ppGeneratedTag, dTemp);	

#ifdef SDF_ENCRYPT_API_TEST_IN_GLB_TAG
	/* To test the Encryption API enable the above flag 
	 * and delete above two lines */
	/*w534*/ (void)sdf_ivk_uaEncryptString(dTemp, ppGeneratedTag, &dErr);
#endif
		

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaGenerateGlobalTag");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGenerateViaBranch
 **
 ** DESCRIPTION: This function generates a random Branch based on 
 **				 Transport Address which will be used in Via branch
 **				 fields
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGenerateViaBranch
#ifdef ANSI_PROTO
	( Sdf_st_transportInfo *pTransportAddr, Sdf_ty_s8bit** ppGeneratedTag)
#else
	( pTransportAddr, ppGeneratedTag)
	Sdf_st_transportInfo *pTransportAddr;
	Sdf_ty_s8bit** 		 ppGeneratedTag;
#endif
{
 
	Sdf_ty_s8bit dTemp[Sdf_co_characterBufferSize];
	Sdf_ty_u32bit dMicroSecs = 0;
 
	Sdf_ty_u32bit dRand;
#ifdef SDF_ENCRYPT_API_TEST_IN_VIA
    Sdf_st_error dErr;
#endif
 
	*ppGeneratedTag=Sdf_co_null;
	
	(void)pTransportAddr;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaGenerateViaBranch");
	/*w534*/ (void)sdf_fn_uaGetCurrentTimeInMicroSecs(&dMicroSecs);
	
	/*w534*/ (void)sdf_fn_uaGenerateRand(&dRand);
	/*w534*/ (void)sdf_fn_uaSprintf(dTemp, "%lu-%d",dMicroSecs, dRand);

	*ppGeneratedTag = (Sdf_ty_s8bit *)sdf_memget(Sdf_mc_callHandlerMemId, \
		Sdf_mc_strlen(dTemp) + 1, (Sdf_st_error *)Sdf_co_null);
    if(*ppGeneratedTag == Sdf_co_null)
      return Sdf_co_fail;
	Sdf_mc_strcpy(*ppGeneratedTag, dTemp);	

#ifdef SDF_ENCRYPT_API_TEST_IN_VIA
	/* To test the Encryption API enable the above flag 
	 * and delete above two lines */
	/*w534*/ (void)sdf_ivk_uaEncryptString(dTemp, ppGeneratedTag, &dErr);
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaGenerateViaBranch");
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetDeltaSecondsForRetryAfter
 **
 ** DESCRIPTION: This function generates a random value in delta seconds 
 ** 			 to be used in RetryAfter header
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetDeltaSecondsForRetryAfter
#ifdef ANSI_PROTO
	(Sdf_ty_u16bit dRespCode, Sdf_ty_u32bit *pDeltaSecs)
#else
	(pRespCode, pDeltaSecs)
	Sdf_ty_u16bit dRespCode;
	Sdf_ty_u32bit *pDeltaSecs;
#endif
{
#ifdef ICF_UATK_PORT
#if defined ICF_PORT_WINCE
	SYSTEMTIME dSysTime;
	FILETIME dFlTime;
#endif

int j=0;
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || defined(ICF_PORT_SYMBIAN) 
	struct timeval tv;
#endif	
#ifdef SDF_VXWORKS	
struct timespec             ts;
/*	Sdf_ty_s8bit *pTime = Sdf_co_null;
	char buffer[26];
	size_t buffer_len = 26;*/
#endif



#ifdef SDF_ERROR		
#ifdef SDF_LINT
	Sdf_st_error dErr={Sdf_en_LastUatkError};
#else
	Sdf_st_error dErr;
#endif
#endif	

    /* CSR-1-7037536: If response code is 486 then fill Retry-After 
       with ICF_RETRY_AFTER_DURATION */

    if (486 == dRespCode)
    {
        *pDeltaSecs = ICF_RETRY_AFTER_DURATION; 
    }
    else
    { 

#if defined (ICF_PORT_WINCE)
	GetLocalTime(&dSysTime);
	SystemTimeToFileTime(&dSysTime,&dFlTime);
	j = dFlTime.dwHighDateTime * 10000000;
	(void)srand(j);
#else

#ifdef ICF_PORT_WINDOWS
/* this is the code for windows */
struct _timeb	timebuffer;
	(void)dRespCode;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaGetDeltaSecondsForRetryAfter");

#ifdef SDF_ERROR
(void)dErr;
#endif
_ftime( &timebuffer );
j=timebuffer.time;
srand(j);

	/* SPR 19010 : The timer value should be a random duration between 1 and 
     * Sdf_mc_maxRetryDuration. The previous handling was generating it as 1
     * always which is not correct, Merged the handling of VxWorks and non Vxworks
     */	
	
	/* Mixed-congruential random number generator : X = (aX   + b) % max
	**                                               n     n-1
	** where 
	**	a % k == 1 for each prime factor 'k' of max and
	**  a % 4 == 1 if 4 is a factor of max.
	** 	b is relatively prime to max
	** 
	** This random number generator generates random numbers in the 
	** range 0 to (max - 1) and its period covers the entire range.
	*/
	j = j % (int)Sdf_mc_maxRetryDuration;
	*pDeltaSecs = 1 + ((int)((Sdf_mc_maxRetryDuration+1)*j + 17) % (int)Sdf_mc_maxRetryDuration);

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "

	"Exiting sdf_fn_uaGetDeltaSecondsForRetryAfter");
	return Sdf_co_success;
/** the code for windows ends here */
#else	
	(void)dRespCode;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaGetDeltaSecondsForRetryAfter");
#ifdef SDF_VXWORKS
		
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	
	
#else
	if (gettimeofday(&tv, Sdf_co_null) == -1)
#endif
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetDeltaSecondsForRetryAfter( ): "
			"Failed to get Current time", &dErr);
#endif		
		*pDeltaSecs = 0;
		return Sdf_co_fail;
	}
#ifdef SDF_VXWORKS
	j = ts.tv_sec;
#else
	j=tv.tv_sec;
#endif
	srand(j);

	/* SPR 19010 : The timer value should be a random duration between 1 and 
     * Sdf_mc_maxRetryDuration. The previous handling was generating it as 1
     * always which is not correct, Merged the handling of VxWorks and non Vxworks
     */	
	
	/* Mixed-congruential random number generator : X = (aX   + b) % max
	**                                               n     n-1
	** where 
	**	a % k == 1 for each prime factor 'k' of max and
	**  a % 4 == 1 if 4 is a factor of max.
	** 	b is relatively prime to max
	** 
	** This random number generator generates random numbers in the 
	** range 0 to (max - 1) and its period covers the entire range.
	*/
	j = j % (int)Sdf_mc_maxRetryDuration;
	*pDeltaSecs = 1 + ((int)((Sdf_mc_maxRetryDuration+1)*j + 17) % (int)Sdf_mc_maxRetryDuration);
	
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaGetDeltaSecondsForRetryAfter");

	return Sdf_co_success;

#endif /* end of #ifdef for windows */
#endif /* end of WIN-CE flag */
    }

	return Sdf_co_success;

#else /*ifndef ICF_UATK_PORT ***/ 

	int j=0;
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) 
	struct timeval tv;
#endif	


	
	
#ifdef SDF_ERROR		
	Sdf_st_error dErr;
#endif	
	(void)dRespCode;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaGetDeltaSecondsForRetryAfter");

#if !defined (SDF_WINDOWS) && !defined (SDF_WINCE)
	if (gettimeofday(&tv, Sdf_co_null) == -1)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetDeltaSecondsForRetryAfter( ): "
			"Failed to get Current time", &dErr);
#endif		
		*pDeltaSecs = 0;
		return Sdf_co_fail;
	}
	j=tv.tv_sec;
#else
	/* SDF_WINCE */
	GetLocalTime(&dSysTime);
	SystemTimeToFileTime(&dSysTime,&dFlTime);
	j = dFlTime.dwHighDateTime * 10000000;
#endif /* !defined (SDF_WINDOWS) && !defined (SDF_WINCE) */

	(void)srand(j);

	*pDeltaSecs = 1+(int) (Sdf_mc_maxRetryDuration*rand()/((RAND_MAX-1)+1.0));

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_fn_uaGetDeltaSecondsForRetryAfter");

	return Sdf_co_success;

#endif /*** ICF_UATK_PORT ***/
    
}


#ifdef SDF_THREAD_SAFE

/* =========================================================================
 * 
 * 						THREAD LIBRARY ROUTINES
 *
 * ========================================================================= */


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCreateThread
 **
 ** DESCRIPTION: This function creates a thread and passes the id back to 
 **				 the invocation.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCreateThread 
#ifdef ANSI_PROTO
	(Sdf_ty_threadT *pThread,
	Sdf_ty_pvoid pThreadAttr,
	Sdf_ty_fptrStartFunc fptrStart,
	Sdf_ty_pvoid pArg)
#else
	(pThread,pThreadAttr,fptrStart,pArg)
	Sdf_ty_threadT *pThread;
	Sdf_ty_pvoid pThreadAttr;
	Sdf_ty_fptrStartFunc fptrStart;
	Sdf_ty_pvoid pArg;
#endif
{
#ifdef SDF_ERROR	
	Sdf_st_error dErr;
#endif		

#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
	if ((pthread_create(pThread, (pthread_attr_t *)pThreadAttr, \
		fptrStart, pArg)) != 0)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCreateThread( ): "
			"Thread creation failed", &dErr);
#endif		
		return Sdf_co_fail;
	}
#endif
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	Sdf_ty_threadParameter *pThreadParameter=\
								(Sdf_ty_threadParameter *)pThreadAttr;
	*pThread=CreateThread(pThreadParameter->pSecurity,\
				pThreadParameter->dStacksize,fptrStart,pArg,\
				pThreadParameter->dwCreationFlags,\
				&(pThreadParameter->dThreadId));	
	if(*pThread==Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCreateThread( ): "
			"Thread creation failed", &dErr);
#endif		
		return Sdf_co_fail;
	}

#endif
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaTerminateThread
 **
 ** DESCRIPTION: This function terminates the thread
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaTerminateThread
#ifdef ANSI_PROTO
	(Sdf_ty_threadT dThread)
#else
	(dThread)
	Sdf_ty_threadT dThread;
#endif
{
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
DWORD	winError=0;
#endif

#ifdef SDF_ERROR	
#ifdef SDF_LINT
	Sdf_st_error dErr = {Sdf_en_LastUatkError,Sdf_en_LastUatkError,\
						0,{'\0',}};
#else
  Sdf_st_error dErr;
/*    Sdf_en_LastUatkError*/
#endif    
	(void) dErr;
#endif


#if defined(SDF_SOLARIS) || defined(SDF_LINUX) 
	if (pthread_kill(dThread, SIGTERM) != 0)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaTerminateThread( ): "
			"Thread termination failed", &dErr);
#endif		
		return Sdf_co_fail;
	}
#endif

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	if(TerminateThread(dThread,winError) == 0)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaTerminateThread( ): "
			"Thread termination failed", &dErr);
#endif		
		return Sdf_co_fail;
	}
#endif
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCancelThread
 **
 ** DESCRIPTION: This function cancels the thread
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCancelThread
#ifdef ANSI_PROTO
	(Sdf_ty_threadT dThread)
#else
	(dThread)
	Sdf_ty_threadT dThread;
#endif
{
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
DWORD winError=0;
#endif
#ifdef SDF_ERROR		
	Sdf_st_error dErr;
#endif	
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) 
	if ( pthread_cancel(dThread) != 0)
#else
	if(TerminateThread(dThread,winError) == 0)
#endif	
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaCancelThread( ): "
			"Thread cancellation failed", &dErr);
#endif		
		return Sdf_co_fail;
	}
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaJoinThread
 **
 ** DESCRIPTION: This function suspends execution of the calling thread
 **				 until the target thread completes
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaJoinThread
#ifdef ANSI_PROTO
	(Sdf_ty_threadT dThread, Sdf_ty_pvoid *ppVal)
#else
	(dThread, ppVal)
	Sdf_ty_threadT dThread;
	Sdf_ty_pvoid *ppVal;
#endif
{
#ifdef SDF_LINT
    Sdf_st_error dErr = {Sdf_en_LastUatkError,Sdf_en_LastUatkError,\
						0,{'\0',}};
#else
    Sdf_st_error dErr;
#endif    
	(void) dErr;
#if defined(SDF_SOLARIS) || defined(SDF_LINUX)

	if (pthread_join(dThread, ppVal) != 0)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaJoinThread( ): "
			"Thread join failed", &dErr);
#endif		
		return Sdf_co_fail;
	}
#endif
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	if(WaitForSingleObject(dThread,INFINITE)==WAIT_FAILED)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaJoinThread( ): "
			"Thread join failed", &dErr);
#endif		
		return Sdf_co_fail;
	}
#endif
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetThreadCancelState
 **
 ** DESCRIPTION: This function can be used to enable/disable cancellation
 **				 of a thread
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaSetThreadCancelState
#ifdef ANSI_PROTO
	(Sdf_ty_s32bit state, Sdf_ty_s32bit *oldState)
#else
	(state, oldState)
	Sdf_ty_s32bit state;
	Sdf_ty_s32bit *oldState;
#endif
{
#if !defined(SDF_VXWORKS) && !defined(SDF_WINDOWS) && !defined(SDF_WINCE)
	if (pthread_setcancelstate(state, oldState) == 0)
		return Sdf_co_success;
	else
		return Sdf_co_fail;
#else
	return Sdf_co_success;
#endif	
}
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaTestCancel
 **
 ** DESCRIPTION: This function checks if thread was Cancelled
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaTestCancel
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid	pVoid)
#else
	(pVoid)
	Sdf_ty_pvoid	pVoid;
#endif
{
	(void)pVoid;
#if defined(SDF_SOLARIS) || defined (SDF_LINUX)
	pthread_testcancel();
#endif
	return Sdf_co_success;
}

#ifdef SDF_LOOKUP
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetSpecificThreadKey
 **
 ** DESCRIPTION: This function get Specific key for the thread.
 **
 ******************************************************************************/
Sdf_ty_pvoid sdf_fn_uaGetSpecificThreadKey ( \
   Sdf_st_ThreadKey dLocThreadKey)
{
#if defined(SDF_WINDOWS) && defined (SDF_IPV6) || defined(SDF_WINCE) 	
   return TlsGetValue(dLocThreadKey);
#endif
#if defined(SDF_SOLARIS) || defined (SDF_LINUX) || defined (SDF_VXWORKS) || defined(SDF_WINDOWS)
	return pthread_getspecific(dLocThreadKey);
#endif
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetSpecificThreadKey
 **
 ** DESCRIPTION: This function set Specific key for the thread.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaSetSpecificThreadKey ( \
   Sdf_st_ThreadKey dLocThreadKey, 
   Sdf_ty_pvoid     *ppVoid)
{

#if defined(SDF_WINDOWS) && defined (SDF_IPV6)
	if(TlsSetValue(dLocThreadKey, *ppVoid) == 0)
		return Sdf_co_fail;
#endif
#if defined(SDF_WINDOWS) || defined(SDF_WINCE)	
	if (pthread_setspecific(dLocThreadKey,*ppVoid) !=0)
		return Sdf_co_success;
	else
		return Sdf_co_fail;
#endif
#if defined(SDF_SOLARIS) || defined (SDF_LINUX) || defined (SDF_VXWORKS)
	if (pthread_setspecific(dLocThreadKey,*ppVoid) !=0)
    		return Sdf_co_fail;
#endif

  return Sdf_co_success;

}
/*****************************************************************
** FUNCTION:sdf_fn_CreateThreadKey
**
**
** DESCRIPTION: It Creates the Thread Key for Gloable to all thread
**     
*******************************************************************/

Sdf_ty_retVal sdf_fn_CreateThreadKey
#ifdef ANSI_PROTO
( Sdf_ty_pvoid pDestFunc)
#else
(pDestFunc )
Sdf_ty_pvoid pDestFunc;
#endif
{
#if defined(SDF_WINDOWS) || defined (SDF_WINCE)

		 pthread_key_create(&dThreadKey, (Sdf_ty_void (*)(Sdf_ty_pvoid))pDestFunc);

		 if(dThreadKey == TLS_OUT_OF_INDEXES)
			return Sdf_co_fail;
		 else
			return Sdf_co_success;
#else
		 Sdf_ty_s32bit dRetVal;
	         dRetVal = pthread_key_create(&dThreadKey, (Sdf_ty_void (*)(Sdf_ty_pvoid))pDestFunc);
		 return (dRetVal == 0 ? Sdf_co_success : Sdf_co_fail);
#endif
#if defined(SDF_WINDOWS) && defined (SDF_IPV6)
		 dThreadKey = TlsAlloc();	
		 return (dThreadKey == TLS_OUT_OF_INDEXES ? Sdf_co_fail : Sdf_co_success);
#endif
}

#endif

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetCallingThreadId
 **
 ** DESCRIPTION: This function returns the threadId of the calling
 **				 thread.
 **
 *****************************************************************************/
Sdf_ty_threadId sdf_fn_uaGetCallingThreadId
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pData)
#else
	(pData)
	Sdf_ty_pvoid pData;
#endif
{
	/* to remove compiler warning */
	(void)pData;

#if defined(SDF_VXWORKS)
	return taskIdSelf();
#elif defined(SDF_WINDOWS) || defined (SDF_WINCE)
	return GetCurrentThreadId();
#elif defined(SDF_LINUX)
	return pthread_self();
#endif

}
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInitMutex
 **
 ** DESCRIPTION: This function initializes the mutex
 **
 *****************************************************************************/
void sdf_fn_uaInitMutex
#ifdef ANSI_PROTO
	(Sdf_ty_mutexIdT *pMutex, 
	Sdf_ty_bool dUseThreadAssociation)
#else
	(pMutex, dUseThreadAssociation)
	Sdf_ty_mutexIdT *pMutex;
	Sdf_ty_bool dUseThreadAssociation;
#endif
{
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	fast_init_mutex(&(pMutex->dMutex));
#else
	/*for Linux*/
	fast_init_synch(&(pMutex->dMutex));
#endif
	pMutex->dUseThreadAssociation = dUseThreadAssociation;
	pMutex->dThreadId = 0;
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	pMutex->dLockCount = 0;
#endif
	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG: Initialised Mutex Lock");*/
}


/*****************************************************************************
 ** FUNCTION: sdf_fn_uaDestroyMutex
 **
 ** DESCRIPTION: This function destroys the mutex
 **
 *****************************************************************************/
void sdf_fn_uaDestroyMutex
#ifdef ANSI_PROTO
	(Sdf_ty_mutexIdT *pMutex)
#else
	(pMutex)
	Sdf_ty_mutexIdT *pMutex;
#endif
{
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	fast_free_mutex(&(pMutex->dMutex)); 
#else
	/*for Linux*/
	fast_free_synch(&(pMutex->dMutex)); 
#endif
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG: Destroyed Mutex Lock");
}


/*****************************************************************************
 ** FUNCTION: 	 	sdf_fn_uaTryLock
 **
 ** DESCRIPTION: 	This function is identical to the sdf_fn_uaLockMutex 
 **					except that if the mutex object is already locked by 
 **					any thread, this call returns immediately.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaTryLock
#ifdef ANSI_PROTO
	(Sdf_ty_mutexIdT *pMutex,
	Sdf_ty_threadId dThreadId)
#else
	(pMutex, dThreadId)
	Sdf_ty_mutexIdT *pMutex;
	Sdf_ty_threadId dThreadId;
#endif
{
	if (pMutex != Sdf_co_null)
	{

			
#ifndef SDF_LINT
	  Sdf_ty_s32bit	ret_val=0;
#endif


		/*
		 * If this is a mutex that uses the Thread Association concept,
		 * and the same thread that has locked it is trying to lock it
		 * again, we return without attempting to fast_lock_synch (so as
		 * to avoid incrementing Lock count).
		 */
		if (pMutex->dUseThreadAssociation == Sdf_co_true)
		{
			if (pMutex->dThreadId == dThreadId)
				return Sdf_co_success;
		}
#if defined(SDF_VXWORKS)
		if(semTake (pMutex->dMutex,NO_WAIT) == ERROR)
		{
			return Sdf_co_fail;
		}
		else
		{
			pMutex->dThreadId = dThreadId;
			/* sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG: Got Mutex Lock "
				"using sdf_fn_uaTryLock()"); */
			return Sdf_co_success;
		}

#elif defined(SDF_WINDOWS) || defined(SDF_WINCE)
		if(WaitForSingleObject(pMutex->dMutex,0)==WAIT_TIMEOUT)
			return Sdf_co_fail;
		else
		{
			pMutex->dThreadId = dThreadId;
			pMutex->dLockCount++;
			/* sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG: Got Mutex Lock "
					"using sdf_fn_uaTryLock()"); */
			return Sdf_co_success;
		}
#else
		if((ret_val = pthread_mutex_trylock(&(pMutex->dMutex)))==0) 
		{
			pMutex->dThreadId = dThreadId;

			/* sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG: Got Mutex Lock "
				"using sdf_fn_uaTryLock()"); */

			return Sdf_co_success;
		}
		else
		{
			if (ret_val == EDEADLK)
			{
				/* Current thread already owns the mutex */
				return Sdf_co_success;
			}
			else
			{
				return Sdf_co_fail;
			}
		}
#endif		
	}
	
	else
		return Sdf_co_fail;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaLockMutex
 **
 ** DESCRIPTION: This function locks a mutex. 
 **
 ** NOTE:		 The check for mutex not NULL is a must here.
 **
 ******************************************************************************/
void sdf_fn_uaLockMutex 
#ifdef ANSI_PROTO
	(Sdf_ty_threadId dThreadId,
	Sdf_ty_mutexIdT *pMutex,
	Sdf_ty_u32bit flags)
#else
	(dThreadId,pMutex,flags)
	Sdf_ty_threadId dThreadId;
	Sdf_ty_mutexIdT *pMutex;
	Sdf_ty_u32bit flags;
#endif
{
	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Grabbing mutex lock");*/

	if (pMutex != Sdf_co_null)
	{
		/* 
		 * Recursive locking - if the same thread attempts to lock again,
		 * lock count will be incremented
		 */
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)

		fast_lock_mutex(0, &(pMutex->dMutex), flags);
#else

#ifdef ICF_UATK_PORT
		/*for Linux*/
		fast_lock_synch(0, &(pMutex->dMutex), flags);
#else /* ifndef ICF_UATK_PORT*/
		fast_lock_synch(dThreadId,&(pMutex->dMutex),flags);
#endif /*ICF_UATK_PORT*/

#endif
		pMutex->dThreadId = dThreadId;
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
		pMutex->dLockCount++;
#endif

	}

	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Successfully grabbed mutex
   * lock");*/
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUnlockMutex
 **
 ** DESCRIPTION: This function unlocks a mutex
 **
 ** NOTE:		 The check for mutex not NULL is a must here.
 **
 ******************************************************************************/
void sdf_fn_uaUnlockMutex 
#ifdef ANSI_PROTO
 	(Sdf_ty_threadId dThreadId,
	Sdf_ty_mutexIdT *pMutex)
#else
	(dThreadId,pMutex)
	Sdf_ty_threadId dThreadId;
	Sdf_ty_mutexIdT *pMutex;
#endif
{
	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - unlocking mutex");*/

	if (pMutex != Sdf_co_null)
	{
		if ( ((pMutex->dUseThreadAssociation == Sdf_co_true) \
			&& (dThreadId == pMutex->dThreadId)) || \
			(pMutex->dUseThreadAssociation == Sdf_co_false) )
		{
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
			fast_unlock_mutex(0, &(pMutex->dMutex));
#else
			fast_unlock_synch(dThreadId,&(pMutex->dMutex));
#endif
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
			pMutex->dLockCount--;
			if (0 == pMutex->dLockCount)
#endif
				pMutex->dThreadId = 0;

		}
	}

	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - unlocked mutex");*/
}
/*
 * Functions that use the critical section object.
 */

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInitSyncObject
 **
 ** DESCRIPTION: This function initializes the Critical Section
 **
 *****************************************************************************/
void sdf_fn_uaInitSyncObject
#ifdef ANSI_PROTO
	(Sdf_ty_syncIdT *pMutex, 
	Sdf_ty_bool dUseThreadAssociation)
#else
	(pMutex, dUseThreadAssociation)
	Sdf_ty_syncIdT *pMutex;
	Sdf_ty_bool dUseThreadAssociation;
#endif
{
	fast_init_synch(&(pMutex->dMutex));
	pMutex->dUseThreadAssociation = dUseThreadAssociation;
	pMutex->dThreadId = 0;
	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG: Initialised Critical
   * Section");*/

}


/*****************************************************************************
 ** FUNCTION: sdf_fn_uaDestroySyncObject
 **
 ** DESCRIPTION: This function destroys the Critical Section
 **
 *****************************************************************************/
void sdf_fn_uaDestroySyncObject
#ifdef ANSI_PROTO
	(Sdf_ty_syncIdT *pMutex)
#else
	(pMutex)
	Sdf_ty_syncIdT *pMutex;
#endif
{
	fast_free_synch(&(pMutex->dMutex));
	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG: Destroyed Critical Section
   * ");*/
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaLockSyncObject
 **
 ** DESCRIPTION: This function Enters the Critical Section. 
 **
 ** NOTE:		 The check for mutex not NULL is a must here.
 **
 ******************************************************************************/
void sdf_fn_uaLockSyncObject 
#ifdef ANSI_PROTO
	(Sdf_ty_threadId dThreadId,
	Sdf_ty_syncIdT *pMutex,
	Sdf_ty_u32bit flags)
#else
	(dThreadId,pMutex,flags)
	Sdf_ty_threadId dThreadId;
	Sdf_ty_syncIdT *pMutex;
	Sdf_ty_u32bit flags;
#endif
{
	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - =Entering Critical Section
   * ");*/

	if (pMutex != Sdf_co_null)
	{
		fast_lock_synch(0, &(pMutex->dMutex), flags);
		pMutex->dThreadId = dThreadId;
	}

	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Successfully Entered Critical
   * Section");*/
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUnlockSyncObject
 **
 ** DESCRIPTION: This function Leaves the Critical Section
 **
 ** NOTE:		 The check for mutex not NULL is a must here.
 **
 ******************************************************************************/
void sdf_fn_uaUnlockSyncObject 
#ifdef ANSI_PROTO
 	(Sdf_ty_threadId dThreadId,
	Sdf_ty_syncIdT *pMutex)
#else
	(dThreadId,pMutex)
	Sdf_ty_threadId dThreadId;
	Sdf_ty_syncIdT *pMutex;
#endif
{
	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Leaving Critical Section
   * ");*/

	if (pMutex != Sdf_co_null)
	{
		if ( ((pMutex->dUseThreadAssociation == Sdf_co_true) \
			&& (dThreadId == pMutex->dThreadId)) || \
			(pMutex->dUseThreadAssociation == Sdf_co_false) )
		{
			pMutex->dThreadId = 0;
			fast_unlock_synch(0, &(pMutex->dMutex));
			
		}
	}

	/*sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Successfully Left Critical
   * Section");*/
}

#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaThreadAttrInit
 **
 ** DESCRIPTION: This function initializes a thread attribute object
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaThreadAttrInit
#ifdef ANSI_PROTO
(Sdf_ty_threadAttrT *pAttr)
#else
(pAttr)
Sdf_ty_threadAttrT *pAttr;
#endif
{
	if (pthread_attr_init(pAttr) == 0)
	{
		return Sdf_co_success;
	}
	else
		return Sdf_co_fail;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaThreadAttrDestroy
 **
 ** DESCRIPTION: This function destroys a thread attribute object
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaThreadAttrDestroy
#ifdef ANSI_PROTO
	(Sdf_ty_threadAttrT *pAttr)
#else
	(pAttr)
	Sdf_ty_threadAttrT *pAttr;
#endif
{
	if (pthread_attr_destroy(pAttr) == 0)
		return Sdf_co_success;
	else
		return Sdf_co_fail;
}
#endif

#endif



/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSleep
 **
 ** DESCRIPTION: This function pauses the calling thread for the 
 **				 duration specified
 **
 ******************************************************************************/
void sdf_fn_uaSleep 
#ifdef ANSI_PROTO
(Sdf_ty_u32bit dSecs,
Sdf_ty_u32bit dMicroSecs)
#else
(dSecs , dMicroSecs)
Sdf_ty_u32bit dSecs;
Sdf_ty_u32bit dMicroSecs;
#endif
{
#ifdef ICF_UATK_PORT
	struct timeval tv;

	tv.tv_sec = dSecs;
	tv.tv_usec =dMicroSecs;
#if defined(SDF_SOLARIS) || defined (SDF_LINUX)	


	pthread_testcancel();

#endif
#ifndef ICF_PORT_SYMBIAN
	select(1, Sdf_co_null, Sdf_co_null, Sdf_co_null, &tv);
#else
	/*pankaj :- Add sleep function.*/
	/*unsigned sleep(unsigned int _seconds);*/
#endif
#if defined(SDF_SOLARIS) || defined (SDF_LINUX)

	pthread_testcancel();

#endif

#else  /*ifndef ICF_UATK_PORT*/

	struct timeval tv;

	tv.tv_sec = dSecs;
	tv.tv_usec =dMicroSecs;
	pthread_testcancel();
	(void)select(1, Sdf_co_null, Sdf_co_null, Sdf_co_null, &tv);
	pthread_testcancel();



#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	Sdf_ty_u32bit delayInMilliSecs;
	delayInMilliSecs = (dSecs*1000) + (dMicroSecs/1000);
	Sleep(delayInMilliSecs); 
#endif

#endif /***ICF_UATK_PORT***/

}

/* End of USE Internal threads ifdef around this API */



/* =========================================================================
 * 
 * 						STRING LIBRARY ROUTINES
 *
 * ========================================================================= */

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSprintf
 **
 ** DESCRIPTION: This function implements sprintf
 **
 *****************************************************************************/
Sdf_ty_s32bit sdf_fn_uaSprintf (Sdf_ty_s8bit *s ,\
	const Sdf_ty_s8bit * fmt, ...)
{
	va_list  ap;
	va_start(ap,fmt);
	(void)vsprintf(s,fmt,ap);
	va_end(ap);
	return 1; /* Should realy be returning the number of bytes formatted */
}

/* Fix for CSR 1-8534364: SPR 20388 */
#ifndef ICF_UATK_PORT
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSnprintf
 **
 ** DESCRIPTION: This function implements snprintf
 **
 *****************************************************************************/
Sdf_ty_s32bit sdf_fn_uaSnprintf (Sdf_ty_s8bit *s ,Sdf_ty_u32bit dSize,\
	const Sdf_ty_s8bit * fmt, ...)
{
	va_list  ap;
	va_start(ap,fmt);
#if defined(SDF_VXWORKS) || defined(SDF_WINDOWS) || defined(SDF_WINCE)
	(void)vsprintf(s,fmt,ap);
#else	
	(void)vsnprintf(s,dSize,fmt,ap);
#endif	
	va_end(ap);
	return 1; /* Should realy be returning the number of bytes formatted */

}
#endif /*** ICF_UATK_PORT ***/


/* =========================================================================
 * 
 * 						NETWORK LIBRARY ROUTINES
 *
 * ========================================================================= */

#if defined(SDF_WINDOWS) || defined(SDF_VXWORKS) || defined(SDF_WINCE)


/*****************************************************************************
 ** FUNCTION:  sdf_fn_uaInetNtoP
 **
 ** DESCRIPTION: This function implements Conversion of network address to 
 **              presentable format.
 **
 ** NOTE: We need to implement this function as windows libraries don't have inet_ntop()
 *****************************************************************************/

const Sdf_ty_s8bit * sdf_fn_uaInetNtoP(af, src, dst, size)
        Sdf_ty_s32bit af;
        const Sdf_ty_void *src;
        Sdf_ty_s8bit *dst;
        size_t size;
{
        switch (af) {
        case AF_INET:
                return (sdf_fn_uaInetNtoP4(src, dst, size));
#if defined (SDF_WINDOWS) && defined(SDF_IPV6)                
        case AF_INET6:
                return (sdf_fn_uaInetNtoP6(src, dst, size));
#endif
        default:
#ifdef ICF_UATK_PORT
#if defined (SDF_WINDOWS) || defined(ICF_PORT_WINDOWS) 
#ifndef ICF_PORT_WINCE
                errno = WSAEAFNOSUPPORT;
#endif
#endif
#else /*ifndef ICF_UATK_PORT*/
                errno = WSAEAFNOSUPPORT;
#endif /* ICF_UATK_PORT */
        }
        /* NOTREACHED */
        return (NULL);
}



/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInetNtoP4
 **
 ** DESCRIPTION: This function implements conversion of IPv4 binary address into 
 **				 presentation (printable) format
 **
 *****************************************************************************/

static const Sdf_ty_s8bit *
sdf_fn_uaInetNtoP4(src, dst, size)
        const u_char *src;
        Sdf_ty_s8bit *dst;
        size_t size;
{
        static const Sdf_ty_s8bit fmt[] = "%u.%u.%u.%u";
        Sdf_ty_s8bit tmp[sizeof "255.255.255.255"];

        sprintf(tmp, fmt, src[0], src[1], src[2], src[3]);
        if ((size_t)Sdf_mc_strlen(tmp) > size) {
#ifndef ICF_PORT_WINCE
                errno = ENOSPC;
#endif
                return (NULL);
        }
        Sdf_mc_strcpy(dst, tmp);
        return (dst);
}


#if defined(SDF_IPV6)
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInetNtoP6
 **
 ** DESCRIPTION: This function implements conversion of IPv6 binary address into 
 **				 presentation (printable) format
 **
 *****************************************************************************/

/* 
 *      convert IPv6 binary address into presentation (printable) format
 */
static const Sdf_ty_s8bit *
sdf_fn_uaInetNtoP6(src, dst, size)
        const u_char *src;
        Sdf_ty_s8bit *dst;
        size_t size;
{
        /*
         * Note that int32_t and int16_t need only be "at least" large enough
         * to contain a value of the specified size.  
         */
        Sdf_ty_s8bit tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"], *tp;
        struct { Sdf_ty_s32bit base, len; } best, cur;
        uint32_t words[IN6ADDRSZ / INT16SZ];
        Sdf_ty_s32bit i;

        /*
         * Preprocess:
         *      Copy the input (bytewise) array into a wordwise array.
         *      Find the longest run of 0x00's in src[] for :: shorthanding.
         */
        memset(words, 0, sizeof words);
        for (i = 0; i < IN6ADDRSZ; i++)
		{
			if (i==0)
				words[0] |= (src[i] << ((1 - (i % 2)) << 3));
			else
			words[i / 2] |= (src[i] << ((1 - (i % 2)) << 3));
		}
        best.base = -1;
        cur.base = -1;
        for (i = 0; i < (IN6ADDRSZ / INT16SZ); i++) {
                if (words[i] == 0) {
                        if (cur.base == -1)
                                cur.base = i, cur.len = 1;
                        else
                                cur.len++;
                } else {
                        if (cur.base != -1) {
                                if (best.base == -1 || cur.len > best.len)
                                        best = cur;
                                cur.base = -1;
                        }
                }
        }
        if (cur.base != -1) {
                if (best.base == -1 || cur.len > best.len)
                        best = cur;
        }
        if (best.base != -1 && best.len < 2)
                best.base = -1;

        /*
         * Format the result.
         */
        tp = tmp;
        for (i = 0; i < (IN6ADDRSZ / INT16SZ); i++) {
                /* Are we inside the best run of 0x00's? */
                if (best.base != -1 && i >= best.base &&
                    i < (best.base + best.len)) {
                        if (i == best.base)
                                *tp++ = ':';
                        continue;
                }
                /* Are we following an initial run of 0x00s or any real hex? */
                if (i != 0)
                        *tp++ = ':';
                /* Is this address an encapsulated IPv4? */
                if (i == 6 && best.base == 0 &&
                    (best.len == 6 || (best.len == 5 && words[5] == 0xffff))) {
                        if (!sdf_fn_uaInetNtoP4(src+12, tp, sizeof tmp - (tp - tmp)))
                                return (NULL);
                        tp += Sdf_mc_strlen(tp);
                        break;
                }
                (void)sdf_fn_uaSprintf(tp, "%x", words[i]);
                tp += strlen(tp);
        }
        /* Was it a trailing run of 0x00's? */
        if (best.base != -1 && (best.base + best.len) == (IN6ADDRSZ / INT16SZ))
                *tp++ = ':';
        *tp++ = '\0';

        /*
         * Check for overflow, copy, and we're done.
         */
        if ((size_t) (tp - tmp) > size) {
#ifndef ICF_PORT_WINCE
                errno = ENOSPC;
#endif
                return (NULL);
        }
        Sdf_mc_strcpy(dst, tmp);
        return (dst);
}
#endif /* #if defined (SDF_IPV6) */
#endif /* defined (SDF_WINDOWS) || defined(SDF_VXWORKS) */

#if !defined(SDF_SUPER) && !defined(ICF_UATK_PORT)
/* Fix for SPR:19991*/
/* Function defination for sdf_fn_uaGetHostIp is replaced by
 * sdf_cbk_uaGetHostIp as in file sdf_portlayer.h we have defined function
 * sdf_fn_uaGetHostIp as sdf_cbk_uaGetHostIp for both EGT and SIP Leg.For EGT
 * leg defination of this function is present in SSA module and for SIP leg it
 * is defined here as in hssUA code this call back is not
 * defined.Customer can either use the same defination or can defined its own
 * implementation.*/
/******************************************************************************
 ** FUNCTION: 	 sdf_cbk_uaGetHostIp
 **
 ** DESCRIPTION: This funtion resolves the provided hostname into an
 **				 IP address and returns the first IP address resolved 
 **				 as a string.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetHostIp
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit* pHost,
	Sdf_ty_s8bit** ppDest,
	Sdf_st_error *pError)
#else
	(pHost,ppDest,pError)
	Sdf_ty_s8bit* pHost;
	Sdf_ty_s8bit** ppDest;
	Sdf_st_error *pError;
#endif
{	
#if defined(SDF_SOLARIS) || defined(SDF_LINUX)||(defined(SDF_WINDOWS) && defined(SDF_IPV6))\
    ||  defined (SDF_WINCE)
	Sdf_ty_s8bit    tmpAddr[INET_ADDRSTRLEN];
	Sdf_ty_s8bit    tmpAddr6[INET6_ADDRSTRLEN];
	struct addrinfo Hints;
	struct addrinfo *pResult;
	Sdf_ty_s32bit 	dErrNo=0;
#endif


#if defined (SDF_WINDOWS) && !defined(SDF_IPV6)
	struct hostent *pHostIp = Sdf_co_null;
#endif

	*ppDest=Sdf_co_null;

#ifdef SDF_DEBUG
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_cbk_uaGetHostIp");
#endif	

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_uaDisplayFnError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_cbk_uaGetHostIp( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
	if (pHost == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaGetHostIp( ): "
			"pHost parameter passed to function is NULL",pError);
#endif
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif


#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || (defined(SDF_WINDOWS) && defined(SDF_IPV6)) \
    ||  defined (SDF_WINCE)
	memset(&Hints, 0, sizeof(Hints));
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) && !defined(SDF_WINDOWS)	

    Hints.ai_flags = AI_ADDRCONFIG;
     
#else
	Hints.ai_flags = AI_PASSIVE;
#endif	
#ifdef SDF_IPV6
	Hints.ai_family = AF_UNSPEC;
#else
	Hints.ai_family = AF_INET;
#endif

	dErrNo=getaddrinfo(pHost, Sdf_co_null, &Hints, &pResult);

	if((dErrNo != 0) || (pResult == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaGetHostIp(): "
			"Failed to resolve hostname to IP Address",pError);
#endif
		pError->errCode = Sdf_en_systemError;
		return Sdf_co_fail;
	}
	else
	{
		if (pResult->ai_family == AF_INET)
		{
			struct in_addr *inaddrptr;

			inaddrptr = (struct in_addr *)(&(((struct sockaddr_in*)(pResult->ai_addr))->sin_addr));
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
			if(Sdf_co_null == sdf_fn_uaInetNtoP(AF_INET,(const u_char *)(inaddrptr),
									(Sdf_ty_s8bit *) tmpAddr, INET_ADDRSTRLEN))
#else

			if(Sdf_co_null == inet_ntop(AF_INET, (Sdf_ty_pvoid)(inaddrptr),
									tmpAddr, INET_ADDRSTRLEN))
#endif
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
					(Sdf_ty_s8bit *)"sdf_cbk_uaGetHostIp(): "
					"Failed to convert numeric address to string format.",pError);
#endif
				/*freehostent is not defined in Linux: We must find the alternative*/
				freeaddrinfo(pResult);
				pError->errCode = Sdf_en_systemError;
				return Sdf_co_fail;
			}
			else
			{
				freeaddrinfo(pResult);
				Sdf_mc_strdup((*ppDest), (Sdf_ty_s8bit *)tmpAddr);
			}
		}
		else if (pResult->ai_family == AF_INET6)
		{
			struct in6_addr *inaddrptr;

			inaddrptr = (struct in6_addr *)(&(((struct sockaddr_in6*)(pResult->ai_addr))->sin6_addr));
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
			if(Sdf_co_null == sdf_fn_uaInetNtoP(AF_INET6,(const u_char *)(inaddrptr),
									(Sdf_ty_s8bit *) tmpAddr6, INET6_ADDRSTRLEN))
#else
			if(Sdf_co_null == inet_ntop(AF_INET6, (Sdf_ty_pvoid)(inaddrptr),
									tmpAddr6, INET6_ADDRSTRLEN))
#endif
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
					(Sdf_ty_s8bit *)"sdf_cbk_uaGetHostIp(): "
					"Failed to convert numeric address to string format.",pError);
#endif
				/*freehostent is not defined in Linux: We must find the alternative*/
				freeaddrinfo(pResult);
				pError->errCode = Sdf_en_systemError;
				return Sdf_co_fail;
			}
			else
			{
				freeaddrinfo(pResult);
				Sdf_mc_strdup((*ppDest), (Sdf_ty_s8bit *)tmpAddr6);
			}
		}

		if(Sdf_co_null == *ppDest)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
					(Sdf_ty_s8bit *)"sdf_cbk_uaGetHostIp( ):"
					"Failed to allocate memory ppDest.",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}
	}
	
#endif	/* SDF_LINUX or SDF_SOLARIS or SDF_WINDOWS && SDF_IPV6 */

#if defined (SDF_WINDOWS) && !defined(SDF_IPV6)
		/* For Handling the IPv4 address Handling */

		pHostIp=gethostbyname(pHost);

		if(pHostIp != Sdf_co_null)
		{
	
			/* inet_addr() function checks if the given string is a valid IP Address
				if this returns INADDR_NONE(Not a IP Address),we go in for name 
				resoultion, else we return the IP Address directly
			*/
			struct in_addr *inaddrptr;
	    inaddrptr = (struct in_addr *)*pHostIp->h_addr_list;

	    /* Set ppDest to the IP address resolved. */
	    Sdf_mc_strdup((*ppDest), (Sdf_ty_s8bit *) inet_ntoa(*inaddrptr));
    }
    else
	  {

#ifdef SDF_ERROR
	    sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaGetHostIp(): "
				"Failed to resolve hostname to IP Address",pError);
#endif
	    pError->errCode = Sdf_en_systemError;
	    return Sdf_co_fail;
    } 
#endif

#ifdef SDF_DEBUG
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_cbk_uaGetHostIp");
#endif	
	pError->errCode=Sdf_en_noUaError;
	return Sdf_co_success;
}
#endif /*SDF_SUPER*/

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetHostName
 **
 ** DESCRIPTION: This function gets the machines local host name by calling 
 **				 gethostname().
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetHostName
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit **ppHost,
	Sdf_st_error *pError)
#else
	(pHost,pError)
	Sdf_ty_s8bit **ppHost;
	Sdf_st_error *pError;
#endif
{
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaGetHostName(ppHost, pError);
#else
	Sdf_ty_s8bit	dTempHost[Sdf_co_smallBufferSize];

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_fn_uaGetHostName");

	if(gethostname(dTempHost,100)!=0)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaGetHostName(): "
				"Failed to get hostname",pError);
#endif
		pError->errCode = Sdf_en_systemError;
		return Sdf_co_fail;
	}
	
	Sdf_mc_strdup((*ppHost),dTempHost);
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_fn_uaGetHostName");
	return Sdf_co_success;
#endif /* endif for SDF_NETWORKCALLBACKS */	
}

/*****************************************************************************
** FUNCTION: sdf_sdf_fn_uaIsConnectedTcpSocket
**
** DESCRIPTION: The function is called internally by the toolkit to check 
**				Whether the socket is connected to the other end
**
******************************************************************************/

Sdf_ty_retVal sdf_fn_uaIsConnectedTcpSocket
#ifdef ANSI_PROTO
	(Sdf_ty_s32bit dSockfd,
	Sdf_st_error *pError)
#else 	
	(dSockfd, pError)
	Sdf_ty_s32bit  dSockfd;
	Sdf_st_error *pError;
#endif
{
#ifdef ICF_PORT_SYMBIAN
	return Sdf_co_success;
#else
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaIsConnectedTcpSocket(dSockfd, pError);
#else
    struct sockaddr cli_addr;

#if defined (SIP_LINUX) || defined(SIP_SOLARIS)
        socklen_t clilen;
#else
	Sdf_ty_s32bit clilen=0;
#endif
	clilen = sizeof(cli_addr);
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
		"Entering sdf_fn_uaIsConnectedTcpSocket\n");

	if (dSockfd == Sdf_co_InvalidSocketFd)
	{
		pError->errCode = Sdf_en_noUaError;
		return Sdf_co_fail;
	}
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_SYMBIAN)

	if(getpeername(dSockfd,(struct sockaddr *)&cli_addr,\

		&clilen)== -1)

#else

	if(getpeername(dSockfd,(struct sockaddr *)&cli_addr,\
		(socklen_t *)&clilen)== -1)

#endif
	{
		pError->errCode = Sdf_en_noUaError;
		return Sdf_co_fail;
	}	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaIsConnectedTcpSocket\n");

	return Sdf_co_success;
#endif /* endif for SDF_NETWORKCALLBACKS */	
#endif /*ICF_PORT_SYMBIAN*/
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCloseSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to close
 **				 a network socket descriptor.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloseSocket
#ifdef ANSI_PROTO
	(Sdf_st_socket *pSocket)	
#else 	
	(pSocket)
	Sdf_st_socket *pSocket;	
#endif
{
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaCloseSocket(pSocket);
#else	

	if((pSocket->dProtocol == Sdf_en_protoNone) || \
		(pSocket->dSockfd == Sdf_co_InvalidSocketFd))
		return Sdf_co_success;		

#ifdef SDF_TLS

	if (pSocket->dProtocol == Sdf_en_protoTls)
	{
		Sdf_st_error   error;
	/* Notify the application about the closure of the TLS connection. */
#ifdef SDF_LINT
(void)sdf_cbk_uaTLSConnectionClosedNtfn(pSocket->dSockfd, &error);
#else
sdf_cbk_uaTLSConnectionClosedNtfn(pSocket->dSockfd, &error);
#endif
	}
#endif /* SDF_TLS */
	(void)sdf_fn_uaClearSocketFd(&dGlbFdSet, pSocket->dSockfd);
#if defined (SDF_WINDOWS) || defined (SDF_WINCE) || defined (ICF_PORT_WINDOWS)
#ifdef SDF_LINT
	(void)closesocket(pSocket->dSockfd);
#else
	closesocket(pSocket->dSockfd);
#endif

#else
	close(pSocket->dSockfd);
#endif
	
	pSocket->dSockfd = Sdf_co_InvalidSocketFd;
	pSocket->dProtocol = Sdf_en_protoNone;
	return Sdf_co_success;
#endif /* endif for SDF_NETWORKCALLBACKS */	
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSocketCreate
 **
 ** DESCRIPTION: The function is called internally by the toolkit to create
 **				 a new network socket descriptor.
 **
 *****************************************************************************/
Sdf_ty_socketId sdf_fn_uaSocketCreate
#ifdef ANSI_PROTO
	(Sdf_ty_s32bit dDomain,
	Sdf_ty_s32bit dType,
	Sdf_ty_s32bit dProtocol)
#else
	(dDomain,dType,dProtocol)
	Sdf_ty_s32bit dDomain;
	Sdf_ty_s32bit dType;
	Sdf_ty_s32bit dProtocol;
#endif
{
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaSocketCreate(dDomain, dType, dProtocol);
#else	
	Sdf_ty_socketId dSocket;
#ifdef SDF_ERROR	
	Sdf_st_error dError;
#endif	
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	if ((dSocket = socket(dDomain,dType,dProtocol))  == INVALID_SOCKET)
#else
	if ((dSocket = socket(dDomain,dType,dProtocol))  < 0)
#endif
	{
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#ifdef SDF_LINT
		(void)WSAGetLastError();
		(void)WSACleanup();
#else
		WSAGetLastError();
		WSACleanup();
#endif
#endif

	if ((dSocket = socket(dDomain,dType,dProtocol))  == INVALID_SOCKET)
	{
 
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaSocketCreate( ): "
			"Could not open UDP socket for initiating call",&dError) ;
#endif
	
#ifdef SDF_LINT
 		 (void)closesocket(dSocket);
#else
  		closesocket(dSocket);
#endif

		return Sdf_co_fail;
	}

	/*
	 * SPR 2221 Fix.
	 * No need to set the socket fd here again as it need to be 
	 * set in the sdf_ivk_uaInitSocket function and not here.
	 */
	/*sdf_fn_uaSetSocketFd(&dGlbFdSet, dSocket);*/
	
	return dSocket;
#endif /* endif for SDF_NETWORKCALLBACKS */	
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaConnectSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to open
 **				 a new network connection.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaConnectSocket
#ifdef ANSI_PROTO
	(Sdf_st_socket *pSocket,
	Sdf_ty_pvoid  pName,
	Sdf_ty_s32bit dNameLength,
	Sdf_ty_pvoid  pExtraArgs)
#else
	(pSocket, pName, dNameLength, pExtraArgs)
	Sdf_st_socket *pSocket;
	Sdf_ty_pvoid  pName;
	Sdf_ty_s32bit dNameLength;
	Sdf_ty_pvoid  pExtraArgs;
#endif
{
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaConnectSocket(pSocket, pName, dNameLength, pExtraArgs);
#else	
#if defined(SDF_ERROR) || defined(SDF_TLS)	
	Sdf_st_error dErr;
#endif

	/* To avoid warnings in case code is compiled without SDF_TLS. */
	pExtraArgs = pExtraArgs;
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	if (connect(pSocket->dSockfd, (struct sockaddr *)pName,
		dNameLength) == SOCKET_ERROR)	
#else
	/*for Linux*/
	if (connect(pSocket->dSockfd, (struct sockaddr *)pName,
		dNameLength) < 0)	
#endif
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_tcpConnectError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaConnectSocket( ): "
			"TCP connect error", &dErr);
#endif			
		return Sdf_co_fail;
	}

#ifdef SDF_TLS
#if defined(SDF_LINUX)    
	if (pSocket->dProtocol == Sdf_en_protoTls)
	{
		/*
		 * TCP connect is done. Now we need to do SSL connect.
		 */
        Sdf_ty_pvoid pSsl = Sdf_co_null;
        SSL_CTX* pSslCtx = Sdf_co_null;        
        Sdf_st_sockInfo   *pSockInfo = Sdf_co_null;

		if (pExtraArgs == Sdf_co_null)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_tlsConnectError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaConnectSocket( ): "
				"TLS connect error", &dErr);
#endif			
			return Sdf_co_fail;
		}

		pSslCtx = ((Sdf_st_sslData *)pExtraArgs)->pData;
		
		if ((pSsl = SSL_new(pSslCtx)) == Sdf_co_null)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_tlsConnectError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaConnectSocket( ): "
				"TLS connect error", &dErr);
#endif			
			return Sdf_co_fail;
		}

#ifdef SDF_LINT
        (void)SSL_set_fd((SSL *)pSsl, pSocket->dSockfd);
#else
        SSL_set_fd((SSL *)pSsl, pSocket->dSockfd);
#endif
	    SSL_set_connect_state((SSL *)pSsl);
	
		if(SSL_connect((SSL *)pSsl) <= 0)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_tlsConnectError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaConnectSocket( ): "
				"TLS connect error", &dErr);
#endif			
            SSL_free(pSsl);
            return Sdf_co_fail;
		}

		/*
		 * TLS connection established. Now notify this to the application
		 * after creating the SockInfo.
		 */
		pSockInfo = (Sdf_st_sockInfo *)sdf_memget(Sdf_mc_callHandlerMemId, \
						sizeof(Sdf_st_sockInfo), &dErr);

		pSockInfo->pSslData = (Sdf_ty_pvoid)pSsl;
		pSockInfo->pSslCtxData = Sdf_co_null;

		if (sdf_cbk_uaTLSConnectionEstdNtfn(pSocket->dSockfd, \
				pSockInfo, &dErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketInfoMapError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaConnectSocket( ): "
				"TLS connect error", &dErr);
#endif
			SSL_free(pSsl);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pSockInfo, \
					&dErr);
			return Sdf_co_fail;
		}
	}
#else
/* TLS transport is not supported on platform other than Linux while using 
 * UATK seperatly from EGT.*/
    return Sdf_co_fail;
#endif
#endif /* SDF_TLS */
	/*
	 * SPR 2221 Fix.
	 * We should set the socket fd here as it not set in the 
	 * sdf_fn_uaSocketCreate function.
	 */
(void)sdf_fn_uaSetSocketFd(&dGlbFdSet, pSocket->dSockfd);
	
	return Sdf_co_success;
#endif /* endif for SDF_NETWORKCALLBACKS */	
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaBindSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to bind
 **				 a socket to a specified port.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaBindSocket
#ifdef ANSI_PROTO
	(Sdf_ty_socketId dSocket,
	Sdf_ty_pvoid  pName,
	Sdf_ty_s32bit dNameLength)
#else
	(dSocket,pName,dNameLength)
	Sdf_ty_socketId dSocket;
	Sdf_ty_pvoid  pName;
	Sdf_ty_s32bit dNameLength;
#endif
{
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaBindSocket(dSocket, pName, dNameLength);
#else	
#ifdef SDF_ERROR
	Sdf_st_error dErr;
#endif

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	if ((bind(dSocket,(struct sockaddr *)pName, dNameLength)) == SOCKET_ERROR)





#else
	/*for linux*/
	if ((bind(dSocket,(struct sockaddr *)pName, dNameLength)) < 0)
#endif
	{
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#ifdef SDF_LINT
		(void)WSAGetLastError();
		(void)WSACleanup();
#else
		WSAGetLastError();
		WSACleanup();
#endif
#endif

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_fn_uaBindSocket(): "
			"Could not bind to address", &dErr);
#endif
		return Sdf_co_fail;
	}
	return Sdf_co_success;
#endif /* endif for SDF_NETWORKCALLBACKS */	
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaListenSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to listen
 **				 on a socket for TCP connections.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaListenSocket
#ifdef ANSI_PROTO
	(Sdf_ty_socketId dSocket,
	Sdf_ty_s32bit dMaxPendingConnections)
#else
	(dSocket, dMaxPendingConnections)
	Sdf_ty_socketId dSocket;
	Sdf_ty_s32bit dMaxPendingConnections;
#endif
{
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaListenSocket(dSocket, dMaxPendingConnections);
#else	
	if (listen(dSocket, dMaxPendingConnections) < 0)
		return Sdf_co_fail;
	else
		return Sdf_co_success;
#endif /* endif for SDF_NETWORKCALLBACKS */	
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetSockOpt
 **
 ** DESCRIPTION: The function is called internally by the toolkit to set
 **				 a socket option.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaSetSockOpt
#ifdef ANSI_PROTO
	(Sdf_ty_socketId dSocket,
	Sdf_ty_s32bit  dLevel,
	Sdf_ty_s32bit  dOptName,
	Sdf_ty_pvoid   pOptVal,
	Sdf_ty_s32bit dOptLen)
#else
	(dSocket,dLevel,dOptName,dOptVal,dOptLen)
	Sdf_ty_socketId dSocket;
	Sdf_ty_s32bit  dLevel;
	Sdf_ty_s32bit  dOptName;
	Sdf_ty_pvoid   pOptVal;
	Sdf_ty_s32bit dOptLen;
#endif
{
#ifdef SDF_NETWORKCALLBACKS
	return sdf_cbk_uaSetSockOpt(dSocket, dLevel, dOptName, pOptVal, dOptLen);
#else	
#ifdef SDF_ERROR
	Sdf_st_error dErr;
#endif

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	if (setsockopt(dSocket,dLevel, \
		dOptName,(Sdf_ty_s8bit *)pOptVal,dOptLen) ==SOCKET_ERROR)
#else
	if (setsockopt(dSocket,dLevel, \
		/*Linux*/
		dOptName,(Sdf_ty_s8bit *)pOptVal,dOptLen) < 0)
#endif
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_fn_uaSetSockOpt"
			"(): Failed to join multicast group", &dErr);
#endif
		return Sdf_co_fail;
	}	
		return Sdf_co_success;
#endif /* endif for SDF_NETWORKCALLBACKS */	
}


#ifdef SDF_THREAD_SAFE
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaThreadSafePrintf
 **
 ** DESCRIPTION: 
 **
 *****************************************************************************/
Sdf_ty_s32bit sdf_fn_uaThreadSafePrintf(const Sdf_ty_s8bit * fmt, ...)
{
	va_list ap;
	Sdf_ty_threadId dummyThreadId=0;

	sdf_fn_uaLockMutex(dummyThreadId,  &dGlbPrintMutex, 0);
	va_start(ap,fmt);
	(void)vprintf(fmt,ap);
	va_end(ap);
	sdf_fn_uaUnlockMutex(dummyThreadId,  &dGlbPrintMutex);
	return(1);
}
#endif

/*****************************************************************************
 ** FUNCTION: sdf_fn_uaGenerateRandom
 **
 ** DESCRIPTION: This function generates a random value 
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGenerateRand
#ifdef ANSI_PROTO
(Sdf_ty_u32bit *pRand)
#else
(pRand)
Sdf_ty_u32bit *pRand;
#endif
{
#if defined(SDF_WINDOWS) || defined(SDF_VXWORKS) || defined(SDF_WINCE)
		static int dDelta=0;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_fn_uaGenerateRand");

#if defined(SDF_SOLARIS) || defined(SDF_LINUX) 
	if( glbPid == 0)
	{
		glbPid = getpid();
	}
	*pRand = glbPid;
#endif		

#ifdef ICF_UATK_PORT
#if !defined(ICF_PORT_WINCE) && defined(SDF_WINDOWS)
	*pRand = _getpid()+dDelta;
	dDelta++;
#endif
#if defined(SDF_VXWORKS)
    *pRand = dDelta;    
	dDelta++;
#endif
#endif /*ICF_UATK_PORT*/

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaGenerateRand");

	return Sdf_co_success;
}

#ifdef SDF_SESSION_TIMER
 /***************************************************************************
 ** FUNCTION: 		sdf_fn_uaModifySessionInterval
 **
 ** DESCRIPTION: 	This function Modifies the Session Interval value 
 **
 ***************************************************************************/
Sdf_ty_retVal sdf_fn_uaModifySessionInterval
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit *pDuration,
	Sdf_ty_refresher dRefesher,
	Sdf_st_error *pErr)
#else
	(pDuration,dRefesher,pErr)
	Sdf_ty_u32bit *pDuration;
	Sdf_ty_refresher dRefesher;
	Sdf_st_error *pErr;
#endif	
{
#ifdef ICF_UATK_PORT
        icf_session_timer_data_st          *p_session_timer_data = ICF_NULL;

 	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering"
		"sdf_fn_uaModifySessionInterval");

        if((ICF_NULL == p_persistent_glb_pdb) || 
           (ICF_NULL == p_persistent_glb_pdb->p_call_ctx))
        {

            ICF_PRINT(((icf_uint8_t*)"\n[SDF_ERROR]: Session Timer, p_persistent_glb_pdb or p_persistent_glb_pdb->p_call_ctx is NULL.\n")) 

           return Sdf_co_fail;
        }

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaModifySessionInterval():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDuration == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaModifySessionInterval():  "
			"pDuration param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(dRefesher == Sdf_en_refresherNone)
	{
             
               if ((ICF_NULL != p_persistent_glb_pdb) && \
                   (ICF_NULL != p_persistent_glb_pdb->p_call_ctx) && \
                   (ICF_TRUE == p_persistent_glb_pdb->p_call_ctx->session_timer_enable) && \
                   (ICF_NULL != p_persistent_glb_pdb->p_call_ctx->p_session_timer_data) && \
                   (ICF_FALSE == p_persistent_glb_pdb->p_call_ctx->p_session_timer_data->session_refresher_reqd))
               {
                    dRefesher = Sdf_en_refresherLocal;
               }
               else
               {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaModifySessionInterval():  "
			"dRefesher param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidRefesher;
	        }	
		return Sdf_co_success;
	}	
#endif /* End param validations ifdef */

	if(dRefesher == Sdf_en_refresherLocal)
		*pDuration= (Sdf_ty_u32bit)(*pDuration/(Sdf_ty_u32bit)2);
	else if (dRefesher == Sdf_en_refresherRemote)
	{    
             if ((ICF_TRUE == p_persistent_glb_pdb->p_call_ctx->session_timer_enable) &&  
                 (ICF_NULL != p_persistent_glb_pdb->p_call_ctx->p_session_timer_data))
             { 
                p_session_timer_data = p_persistent_glb_pdb->p_call_ctx->p_session_timer_data;
                if (ICF_FALSE == p_session_timer_data->session_timer_logic)
                {
                  /* Session Timer will be started as per existing logic.*/
	  	  
		if ((Sdf_ty_u32bit)(*pDuration/3) >(Sdf_ty_u32bit)10) 
			*pDuration= *pDuration	- (Sdf_ty_u32bit)10;
		else
			*pDuration= *pDuration-(Sdf_ty_u32bit)(*pDuration/(Sdf_ty_u32bit)3);
              }
              else
                {
                  /* Session Timer will be started as per logic:
                   * Session Timer will be started for the duration, Either 2/3 of the negotiated Session
                   * expires or Session-Expires - 32, whichever is minimum.
                   */   
                  if ((*pDuration - (Sdf_ty_u32bit)(*pDuration/(Sdf_ty_u32bit)3)) > \
                      (*pDuration - (Sdf_ty_u32bit)32))
                  {
                      *pDuration = *pDuration - (Sdf_ty_u32bit)32;
                  }
                  else
                  {
                      *pDuration= *pDuration-(Sdf_ty_u32bit)(*pDuration/(Sdf_ty_u32bit)3); 
                  } 
                } 
             }
             else
             {
                ICF_PRINT(((icf_uint8_t*)"\n[SDF_ERROR]:Either Session timer is Disabled or Session Timer Data is not available.\n"))
                return Sdf_co_success;
             } 
	}
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_fn_uaModifySessionInterval");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;

#else /*ifndef ICF_UATK_PORT*/

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering"
		"sdf_fn_uaModifySessionInterval");


	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaModifySessionInterval():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDuration == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaModifySessionInterval():  "
			"pDuration param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(dRefesher == Sdf_en_refresherNone)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaModifySessionInterval():  "
			"dRefesher param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	if(dRefesher == Sdf_en_refresherLocal)
		*pDuration= (Sdf_ty_u32bit)(*pDuration/(Sdf_ty_u32bit)2);
	else if (dRefesher == Sdf_en_refresherRemote)
	{
		if ((Sdf_ty_u32bit)(*pDuration/3) >(Sdf_ty_u32bit)10) 
			*pDuration= *pDuration	- (Sdf_ty_u32bit)10;
		else
			*pDuration= *pDuration-(Sdf_ty_u32bit)(*pDuration/(Sdf_ty_u32bit)3);
	}
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_fn_uaModifySessionInterval");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;

#endif /*** ICF_UATK_PORT ***/
}

 /***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetReinviteTimeoutTime
 **
 ** DESCRIPTION: 	This function gives the re-INVITE timeout time for 
 ** 				Sending BYE after re-INVITE timeout
 **
 ***************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetReinviteTimeoutTime
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit *pDuration,
	Sdf_st_error *pErr)
#else
	(pDuration,pErr)
	Sdf_ty_u32bit *pDuration;
	Sdf_st_error *pErr;
#endif	
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering"
		"sdf_fn_uaGetReinviteTimeoutTime");


	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaModifySessionInterval():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDuration == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaModifySessionInterval():  "
			"pDuration param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/* while re-INVITE times out
	 * BYE need to be send
	 * For that we need to calculate re-INVITE timeout time
	 * The re-INVITE timeout time should minimum of 1/3 of Session-Interval
	 * and 10 secconds
	 */

	if ((Sdf_ty_u32bit)(*pDuration/3) >(Sdf_ty_u32bit)10) 
			*pDuration= *pDuration	- (Sdf_ty_u32bit)10;
	else
			*pDuration= *pDuration-(Sdf_ty_u32bit)(*pDuration/(Sdf_ty_u32bit)3);
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_fn_uaGetReinviteTimeoutTime");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
#endif

#ifdef SDF_LINT
/*****************************************************************
        To avoid the Lint Warnings
*****************************************************************/
Sdf_ty_s32bit sdf_fn_uaLintStrcasecmp
#ifdef ANSI_PROTO
        (const Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2)
#else
        (s1,s2)
        const Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
#endif
{
    if ((s1!= Sdf_co_null)&&(s2!= Sdf_co_null)) 
    {
#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
        return(strcasecmp(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2)));
#endif
#ifdef ICF_UATK_PORT
        return(_strcmpi(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2)));
#endif
    }
    return -1;
}


Sdf_ty_s32bit sdf_fn_uaLintStrcmp
#ifdef ANSI_PROTO
        (const Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2)
#else
        (s1,s2)
        const Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
#endif
{
    if ((s1!= Sdf_co_null) && (s2!= Sdf_co_null)) 
    {
        return (strcmp(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2)));
    }
    return -1;
}

Sdf_ty_s32bit sdf_fn_uaLintStrncmp
#ifdef ANSI_PROTO
        (const Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2,Sdf_ty_u32bit n )
#else
        (s1,s2.n)
        const Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
        Sdf_ty_u32bit n;
#endif
{
    if ((s1!= Sdf_co_null) && (s2 != Sdf_co_null)) 
    {
        return(strncmp(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2),n));
    }
    return -1;
}

void sdf_fn_uaLintStrcat
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2)
#else
        (s1,s2)
        Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
#endif
{
    if ((s2!= Sdf_co_null)) 
        strcat(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2));
    return;
}
/*Codenomicon fix start: CSR_1-6714693*/
void sdf_fn_uaLintStrncat
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2,Sdf_ty_u32bit n)
#else
        (s1,s2,n)
        Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
	Sdf_ty_u32bit n;
#endif
{
    if ((s2!= Sdf_co_null))
    {
	strncat(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2),n);
        return;
    }
}
/*Codenomicon fix end: CSR_1-6714693*/

void sdf_fn_uaLintStrcpy
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2)
#else
        (s1,s2)
        Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
#endif
{
    if ((s2!= Sdf_co_null)) 
        strcpy(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2));
    return;
}

void sdf_fn_uaLintStrncpy
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2,Sdf_ty_u32bit n )
#else
        (s1,s2.n)
        Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
        Sdf_ty_u32bit n;
#endif
{
    if ((s2 != Sdf_co_null )) 
        strncpy(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2),n);
    return;
}


#if defined(SDF_SOLARIS) || defined(SDF_VXWORKS)
Sdf_ty_s8bit* sdf_fn_uaLintStrtok
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2)
#else
        (s1,s2)
        Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
#endif
{
    if ((s2 != Sdf_co_null)) 
    {
        return(strtok(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2)));
    }
    return Sdf_co_null;
}
#endif

Sdf_ty_u32bit sdf_fn_uaLintStrlen
#ifdef ANSI_PROTO
        (const Sdf_ty_s8bit *s1)
#else
        (s1)
        const Sdf_ty_s8bit * s1;
#endif
{
    if ((s1 != Sdf_co_null )) 
    {
        return(strlen(((Sdf_ty_s8bit *)s1)));
    }
    return 0;
}

Sdf_ty_s8bit* sdf_fn_uaLintStrstr
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2)
#else
        (s1,s2)
        Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
#endif
{
    if ((s1 != Sdf_co_null) && (s2 != Sdf_co_null)) 
    {
        return(strstr(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2)));
    }
    return Sdf_co_null;
}

#if defined(SDF_SOLARIS) || defined(SDF_VXWORKS) || defined(SDF_OSE)
Sdf_ty_s8bit* sdf_fn_uaLintStrtokr
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2,Sdf_ty_s8bit **s3)
#else
        (s1,s2,s3)
        Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
        Sdf_ty_s8bit **s3;
#endif
{
    if (s1 != Sdf_co_null ) 
    {
        return((Sdf_ty_s8bit*)strtok_r(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2),((Sdf_ty_s8bit **)s3)));
    }
    return Sdf_co_null;
}
#endif

Sdf_ty_s8bit* sdf_fn_uaLintStrdup
#ifdef ANSI_PROTO
        (const Sdf_ty_s8bit *s1, Sdf_ty_s32bit id)
#else
        (s1,id)
        const Sdf_ty_s8bit * s1;
        Sdf_ty_s32bit id;
#endif
{
    if (s1 != Sdf_co_null ) 
    {
        return( (sip_strdup(((Sdf_ty_s8bit *)s1),id)));
    }
    return Sdf_co_null;
}

Sdf_ty_s32bit sdf_fn_uaLintAtoI
#ifdef ANSI_PROTO
        (const Sdf_ty_s8bit *s1)
#else
        (s1)
        const Sdf_ty_s8bit * s1;
#endif
{
    if ((s1 != Sdf_co_null )) 
    {
        return(atoi(((Sdf_ty_s8bit *)s1)));
    }
    return 0;
}

Sdf_ty_s8bit* sdf_fn_uaLintStrpbrk
#ifdef ANSI_PROTO
        (const Sdf_ty_s8bit *s1, const Sdf_ty_s8bit *s2)
#else
        (s1,s2)
        const Sdf_ty_s8bit * s1;
        const Sdf_ty_s8bit * s2;
#endif
{
    if (s1 != Sdf_co_null && s2 != Sdf_co_null) 
    {
        return(strpbrk(((Sdf_ty_s8bit *)s1),((Sdf_ty_s8bit *)s2)));
    }
    return Sdf_co_null;
}
#endif

/*****************************Tel Url Resolution Function********************/




/*****************************************************************************
 *
 * 						Windows porting functions 				
 * 						
 *****************************************************************************/

/*****************************************************************************
 ** FUNCTION: 	 Sdf_mc_strtokr
 **
 ** DESCRIPTION: The function is called internally by the toolkit to split
 **				 a string into substrings.
 **
 *****************************************************************************/
#ifdef ICF_UATK_PORT
#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_SYMBIAN)|| defined(ICF_PORT_WINCE)
Sdf_ty_s8bit* Sdf_mc_strtokr
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *x,
	Sdf_ty_s8bit *y,
	Sdf_ty_s8bit **z)
#else
	(x,y,z)
	Sdf_ty_s8bit *x;
	Sdf_ty_s8bit *y;
	Sdf_ty_s8bit **z;
#endif
{
	Sdf_ty_s8bit *temp;
	if(x==Sdf_co_null)
	{
		*z=Sdf_co_null;
		return Sdf_co_null;
	}
	temp=strtok(x,y);
	*z=strtok(Sdf_co_null,"\0");

	if(Sdf_mc_strcmp(*z, "") == 0)
		*z = Sdf_co_null;

	return temp;
}

#endif
#else /*ifndef ICF_UATK_PORT ***/

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
Sdf_ty_s8bit* Sdf_mc_strtokr
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *x,
	Sdf_ty_s8bit *y,
	Sdf_ty_s8bit **z)
#else
	(x,y,z)
	Sdf_ty_s8bit *x;
	Sdf_ty_s8bit *y;
	Sdf_ty_s8bit **z;
#endif
{
	Sdf_ty_s8bit *temp;
	if(x==Sdf_co_null)
	{
		*z=Sdf_co_null;
		return Sdf_co_null;
	}
	temp=strtok(x,y);
	*z=strtok(Sdf_co_null,"\0");

	if(Sdf_mc_strcmp(*z, "") == 0)
		*z = Sdf_co_null;

	return temp;
}
#endif


#if defined(SDF_LINUX)

Sdf_ty_s8bit* Sdf_mc_strtokr(Sdf_ty_s8bit *x, Sdf_ty_s8bit* y, Sdf_ty_s8bit** z)
{
	Sdf_ty_s8bit *temp;
	if(x==Sdf_co_null)
	{
		*z=Sdf_co_null;
		return Sdf_co_null;
	}
	temp=strtok_r(x,y,z);
	if(Sdf_mc_strcmp(*z, "") == 0)
		*z = Sdf_co_null;

	return temp;
}
#endif

#endif /***ICF_UATK_PORT***/ 

#if defined(SDF_LINUX) || defined(SDF_WINDOWS) || defined(ICF_PORT_SYMBIAN) || defined(ICF_PORT_WINCE)
/*****************************************************************************
 ** FUNCTION: 	 Sdf_mc_strtok
 **
 ** DESCRIPTION: The function is called internally by the toolkit to split
 **				 a string into substrings.
 **
 *****************************************************************************/
Sdf_ty_s8bit *Sdf_mc_strtok(Sdf_ty_s8bit *x, Sdf_ty_s8bit *y)
{
	Sdf_ty_s8bit *retVal=Sdf_co_null;
	
	if (y != Sdf_co_null)
	{
		retVal = strtok(x,y);
		if (x != Sdf_co_null)
			if (Sdf_mc_strcmp(x, "\n") == 0)
				Sdf_mc_strcpy(x, "");
	}
	return retVal;
}
#endif



Sdf_ty_retVal sdf_fn_uaGetNumberOfHopsForMaxForward
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_ty_u32bit 	*dHops,
	Sdf_st_error *pError)
#else
	(pCallObj,dHops,pError)
	Sdf_st_callObject *pCallObj;
	Sdf_ty_u32bit 	*dHops;
	Sdf_st_error *pError;
#endif
{
	(void)pCallObj;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_fn_uaGetNumberOfHopsForMaxForward");

	/* Add logic to generate number of hops */

	*dHops = 70;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_fn_uaGetNumberOfHopsForMaxForward");
	pError->errCode=Sdf_en_noUaError;
	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_fn_uaEndHostEnt
**
** DESCRIPTION: This function frees the system resources allocated 
**				in gethostbyname_r
**
**
**********************************************************/
void sdf_fn_uaEndHostEnt(void)
{
#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
	/*w534*/ (void)endhostent();
#endif
}

/*********************************************************
** FUNCTION: sdf_fn_uaDisplayFnTrace
**
** DESCRIPTION: This function displays the trace string 
**				appropriately for different platforms
**
**
**********************************************************/
void Sdf_fn_uaDisplayFnTrace( const Sdf_ty_s8bit *pStr,     \
                             Sdf_ty_u32bit dLen,     \
                             Sdf_st_error *pErr)
{
    Sdf_ty_s8bit *pTimeStr;


#ifdef SDF_THREAD_SAFE
    (void)sdf_fn_uaTestCancel(Sdf_co_null);                                
#endif	
	write(2,(Sdf_ty_s8bit *)"SDF_TRACE: Generated At ",24);          
	sdf_fn_uaGetTimeString(&pTimeStr);                               
	write(2,(Sdf_ty_s8bit *)pTimeStr,Sdf_mc_strlen(pTimeStr));       
	(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid*)&pTimeStr,pErr); 
	write(2, "::::<>::::", 10);                                      
	if(dLen == 0)                                                    
		write(2, (Sdf_ty_s8bit *)pStr, Sdf_mc_strlen(pStr));                         
	else                                                             
		write(2, (Sdf_ty_s8bit *)pStr, dLen);                                        
	write(2, "\n\n", 2);                                             
#ifdef SDF_THREAD_SAFE
	(void)sdf_fn_uaTestCancel(Sdf_co_null);
#endif
}


/*********************************************************
** FUNCTION: sdf_fn_uaDisplayFnDebug
**
** DESCRIPTION: This function displays the debug string 
**				appropriately for different platforms
**
**
**********************************************************/
void sdf_fn_uaDisplayFnDebug(Sdf_ty_s8bit *str)
{
#if defined(SDF_SOLARIS) || defined(SDF_WINDOWS) || defined(SDF_LINUX) || defined(SDF_WINCE)
#ifdef SDF_DEBUG

 	Sdf_mc_fprintf (stderr, "%s\n", str);
#else
   (void)str;
#endif
#endif

#if defined(SDF_VXWORKS)
#ifdef SDF_DEBUG
 	vxprintf("%s\n", str);
#else
   (void)str;
#endif
#endif

}

/*********************************************************
** FUNCTION: sdf_fn_uaDisplayFnError
**
** DESCRIPTION: This function displays the error string 
**				appropriately for different platforms
**
**
**********************************************************/
void sdf_fn_uaDisplayFnError(Sdf_ty_s8bit *str)
{
#ifdef ICF_UATK_PORT 
    ICF_PRINT(((icf_uint8_t*)"[SDF_ERROR]: %s \n",str));
	(void)str;
    
    if(ICF_NULL == str)
    {
       ICF_PRINT(((icf_uint8_t*)"str is null"));
    }
#endif /*ICF_UATK_PORT*/

#if defined(SDF_SOLARIS) || defined(SDF_WINDOWS) || defined(SDF_LINUX)
 	Sdf_mc_fprintf (stderr, "%s\n", str);
#endif

#if defined(SDF_VXWORKS) || defined (SDF_WINCE)
 	(void)Sdf_mc_printf("%s\n", str);
#endif

}



#ifdef SDF_THREAD_SAFE
/*********************************************************
** FUNCTION: sdf_fn_uaCreateRemoteRetransThread
**
** DESCRIPTION: This function creates the remote retrans thread 
**				appropriately for different platforms
**
**
**********************************************************/
Sdf_ty_retVal
sdf_fn_uaCreateRemoteRetransThread
(Sdf_ty_threadT *pRetransThread, 
Sdf_ty_fptrStartFunc pFuncPtr, 
Sdf_st_error        *pErr)
{
#if defined(SDF_OSE)
Sdf_ty_threadAttrT dAttr;
#endif 
#if defined(SDF_VXWORKS) || defined(SDF_WINDOWS) || defined (SDF_WINCE)
Sdf_ty_threadParameter dAttr;
#endif
Sdf_ty_s32bit i=0;

#if defined(SDF_SOLARIS) || defined(SDF_LINUX)		                         
	if (sdf_fn_uaCreateThread(pRetransThread, Sdf_co_null,         
		pFuncPtr, &i) == Sdf_co_fail)                  
#endif			
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	dAttr.pSecurity=Sdf_co_null;                                             
	dAttr.dStacksize=0;                                                      
	dAttr.dwCreationFlags=0;                                                 
	if (sdf_fn_uaCreateThread(pRetransThread,(Sdf_ty_pvoid)&dAttr, 
		pFuncPtr,&i) == Sdf_co_fail)  
#endif        
	{                                                                        
#ifdef SDF_ERROR                                                             
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_threadCreationError,       
			(Sdf_ty_s8bit *)"sdf_fn_uaCreateRemoteRetransThread( ):"         
			"Failed to create Remote Retransmission Manager Thread",pErr);   
#endif                                                                       
		pErr->errCode=Sdf_en_threadCreationError;                            
		return Sdf_co_fail;                                                  
	}                                                                        

    return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_fn_uaDeleteRemoteRetransThread
**
** DESCRIPTION: This function cancels the remote retrans thread 
**				appropriately for different platforms
**
**
**********************************************************/
Sdf_ty_retVal sdf_fn_uaDeleteRemoteRetransThread \
                 (Sdf_ty_threadT *pRetransThread,\
                  Sdf_ty_pvoid   ptr)
{
	if (sdf_fn_uaCancelThread(*pRetransThread) == Sdf_co_fail)        
	{                                                                         
		return Sdf_co_fail;                                                   
	}                                                                         
	if (sdf_fn_uaJoinThread(*pRetransThread, &ptr) == Sdf_co_fail)    
	{                                                                         
		return Sdf_co_fail;                                                   
	}                                                                         
    return Sdf_co_success;
}
#endif
#ifdef SDF_SESSION_TIMER
/*****************************************************************************
 *
 * 						Session Timer Porting Functions
 * 						
 *****************************************************************************/

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetGlbMinSe
 **
 ** DESCRIPTION: The function returns the configured MinSE of the UA
 **				 The function sdf_fn_uaSetGlbMinSe is used to set the value
 **				 of this parameter
 **
 *****************************************************************************/
Sdf_ty_s32bit sdf_fn_uaGetGlbMinSe(void)
{
	return glbMinSE;
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetGlbIsSupportedTimer
 **
 ** DESCRIPTION: The function returns whether or not Session Timer feature is
 **				 supported by this UA.
 **				 The function sdf_fn_uaSetGlbIsSupportedTimer is used to set 
 **				 the value of this parameter
 **
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaGetGlbIsSupportedTimer(void)
{
#ifdef ICF_UATK_PORT
	if ((ICF_NULL == p_persistent_glb_pdb) ||
            (ICF_NULL == p_persistent_glb_pdb->p_call_ctx))
        {
            /* IPTK is unable to fetch Call Ctxt then return the default
             * value in glb data glbProfileSupportedTimer.
             */
            ICF_PRINT(((icf_uint8_t*)"\n[sdf_portlayer]: Session Timer, Either persisten glb data is NULL or Call Ctxt is NULL.\n"));
            return glbProfileSupportedTimer;
        }
        if (ICF_TRUE == p_persistent_glb_pdb->p_call_ctx->session_timer_enable) 
        {
            return glbProfileSupportedTimer;
        }
        else
        { 
	   return Sdf_co_false;
        }
#else /*ifndef ICF_UATK_PORT */

     return glbProfileSupportedTimer;

#endif /* ICF_UATK_PORT */
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetGlbMinSe
 **
 ** DESCRIPTION: The function configures MinSE for this UA
 **				 The function sdf_fn_uaGetGlbMinSe is used to get the value
 **				 of this parameter
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaSetGlbMinSe(Sdf_ty_s32bit dMinSe, Sdf_st_error *pErr)
{
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
		return Sdf_co_fail;
#endif
	if (dMinSe < 0)
	{
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	glbMinSE = dMinSe;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetGlbIsSupportedTimer
 **
 ** DESCRIPTION: The function sets whether or not Session Timer feature is
 **				 supported by this UA.
 **				 The function sdf_fn_uaGetGlbIsSupportedTimer is used to get 
 **				 the value of this parameter
 **
 *****************************************************************************/
void sdf_fn_uaSetGlbIsSupportedTimer(Sdf_ty_bool dIsSuppTimer)
{
	glbProfileSupportedTimer = dIsSuppTimer;
}
#endif


#ifdef SDF_MEMCAL
void sip_memStatInit(void)
{
	glbSipMemStat.cum_memget = 0;
	glbSipMemStat.cum_memfree = 0;
	glbSipMemStat.max_mem_usage = 0;

	fast_init_mutex(&glbSipMemStat.lock);
}

SIP_Pvoid fast_memget(SIP_U32bit module_id, SIP_U32bit noctets, SipError *err)
{
	SIP_Pvoid pvoid;
	SIP_U32bit dummy;
	SipMemInfo *p_mem_info;
	dummy = module_id;

	pvoid = (Sdf_ty_pvoid)malloc(noctets+sizeof(SipMemInfo));

	if(pvoid == Sdf_co_null)
	{
		if (err!=Sdf_co_null)
			*err = E_NO_MEM;
		return Sdf_co_null;
	}

	if (err!=Sdf_co_null) 
		*err = E_NO_ERROR;

	/* Set memInfo for the alloced buffer */
	p_mem_info = pvoid;
	p_mem_info->buf_size = noctets;
	p_mem_info->module_id = module_id;

	/* Update Mem Stats */
	fast_lock_mutex(0, &glbSipMemStat.lock, 0);

	glbSipMemStat.cum_memget += noctets;

	if ((glbSipMemStat.cum_memget - glbSipMemStat.cum_memfree) > 
		glbSipMemStat.max_mem_usage);
	{
		glbSipMemStat.max_mem_usage = \
			glbSipMemStat.cum_memget - glbSipMemStat.cum_memfree;
	}
	fast_unlock_mutex(0, &glbSipMemStat.lock);

	return (SIP_Pvoid)((SIP_U8bit *)pvoid + sizeof(SipMemInfo));

}

/*****************************************************************
** FUNCTION:fast_memfree
**
**
** DESCRIPTION:
*******************************************************************/

SipBool fast_memfree ( SIP_U32bit module_id, SIP_Pvoid p_buf , SipError *err)
{
	SIP_U32bit dummy;
	SipMemInfo *p_mem_info;

	dummy = module_id;
	if (p_buf == Sdf_co_null)
		return SipSuccess;
	/* Update Mem Stats */
	fast_lock_mutex(0, &glbSipMemStat.lock, 0);
	p_mem_info = (SipMemInfo*)((SIP_U8bit *)p_buf - sizeof(SipMemInfo));
	glbSipMemStat.cum_memfree += p_mem_info->buf_size;
	fast_unlock_mutex(0, &glbSipMemStat.lock);

	free((SIP_U8bit *)p_buf - sizeof(SipMemInfo));

	if (err !=  Sdf_co_null) 
		*err = E_NO_ERROR;
     return SipSuccess;
}
#endif


#ifdef SDF_TLS

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInitSslData
 **
 ** DESCRIPTION: The function will do the SSL layer initializations.
 **              right now this function will call OpenSSL APIs since UATK
 **              will be using OpenSSL libraries for TLS support.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitSslData(Sdf_ty_s8bit *pEGDRandFile)
{
	/*
	 * This API should be called to initialize the OpenSSL library.
	 */
    /*w534*/ (void)wrapper_SSLeay_add_ssl_algorithms();

	/*
	 * Register the libcrypto and libssl error strings. This function should 
	 * be called before generating textual error messages.
	 */
	wrapper_SSL_load_error_strings();

	/*
	 * Query the entropy gathering daemon (EGD) on path specified.
	 * It queries 255 bytes and uses it to seed the OpenSSL built-in
	 * Pseudorandom Number Generator (PRNG).
	 */
	(void)wrapper_RAND_egd(pEGDRandFile);
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFreeSslData
 **
 ** DESCRIPTION: This function will free the SSL data. Currently, this pData
 **              will be nothing but SSL_CTX.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFreeSslData(Sdf_ty_pvoid *pData)
{
	if (pData == Sdf_co_null)
		return Sdf_co_fail;

	wrapper_SSL_CTX_free(*pData);
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFileOpen
 **
 ** DESCRIPTION: This function opens the file whose pathname is the string  
 **              pointed to by pFilename, and associates a stream with it.
 **
 ******************************************************************************/
Sdf_ty_file* sdf_fn_uaFileOpen
#ifdef ANSI_PROTO
        (Sdf_ty_s8bit   *pFilename,
         Sdf_ty_s8bit   *pMode)
#else
		(pFilename, pMode)
		 Sdf_ty_s8bit   *pFilename;
         Sdf_ty_s8bit   *pMode;
#endif
{
    return fopen( pFilename, pMode );
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFileClose
 **
 ** DESCRIPTION: This function closes the file whose pathname is the string  
 **              pointed to by pFilename.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFileClose
#ifdef ANSI_PROTO
        (Sdf_ty_file   *pFilename)
#else
		(pFilename)
		 Sdf_ty_s8bit   *pFilename;
#endif
{
	if( 0 == fclose( pFilename ) )
        return Sdf_co_success;
    else
        return Sdf_co_fail;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormSslData
 **
 ** DESCRIPTION: This function will form the SSL data that will be stored 
 **              as pData in the pSslData of InitData. In the current 
 **              implementation SSL_CTX structure will be formed and stored 
 **              as pData in the pSslData of InitData.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormSslData
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid    *pSslParams,
	 Sdf_st_sslData  *pSslData,
	 Sdf_st_error    *pErr)
#else
	( pSslParams, pSslData, pErr )
	 Sdf_ty_pvoid    *pSslParams;
	 Sdf_st_sslData  *pSslData;
	 Sdf_st_error    *pErr;
#endif
{
	Sdf_ty_pvoid    *pSslCtx = Sdf_co_null;
	Sdf_ty_s32bit   dCertFormat;
	Sdf_ty_s8bit    pCipherList[Sdf_co_maxCipherListLength];
	Sdf_ty_file     *pDHParamfp = Sdf_co_null;
	Sdf_ty_pvoid    *dh = Sdf_co_null;
	Sdf_st_sslParams  *pTempSslParams = Sdf_co_null;

	pTempSslParams = (Sdf_st_sslParams *)pSslParams;

	pSslCtx = wrapper_SSL_CTX_new();

	if(pSslCtx == Sdf_co_null)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
			"Failed to create context structure for SSL",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
    }

	/* 
	 * Loding certificate Info
     */
    if (pTempSslParams->dCertFormat == Sdf_en_fileFormatPEM)
		dCertFormat = SDF_SSL_FILETYPE_PEM;
	else
		dCertFormat = SDF_SSL_FILETYPE_ASN1;

    if (wrapper_SSL_CTX_use_certificate_file(pSslCtx, pTempSslParams->pCertFile, \
				dCertFormat ) != 1)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidTlsCertificateInfoError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
			"Failed to set the Certificate file.",pErr);
#endif
		pErr->errCode=Sdf_en_invalidTlsCertificateInfoError;
		wrapper_SSL_CTX_free(pSslCtx);
		return Sdf_co_fail;
    }

	wrapper_SSL_CTX_set_default_passwd_cb(pSslCtx, sdf_cbk_uaGetPrivateKeyPassword);

	/* 
	 * Set the password in the userdata. The same userdata will be returned
	 * when the callback is issued.
	 */
	wrapper_SSL_CTX_set_default_passwd_cb_userdata(pSslCtx, \
			(Sdf_ty_pvoid)(pTempSslParams->pPrivateKeyPasswd));

	if (wrapper_SSL_CTX_use_PrivateKey_file(pSslCtx, pTempSslParams->pPrivateKeyFile, \
				dCertFormat) != 1)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidTlsCertificateInfoError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
			"Failed to set the PrivateKey file.",pErr);
#endif
		pErr->errCode=Sdf_en_invalidTlsCertificateInfoError;
		wrapper_SSL_CTX_free(pSslCtx);
		return Sdf_co_fail;
    }

	/*
	 * Check whether the private key is correct or not.
	 */
	if (wrapper_SSL_CTX_check_private_key(pSslCtx) != 1) 
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidTlsCertificateInfoError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
			"Private key does not match the certificate public key.",pErr);
#endif
		pErr->errCode=Sdf_en_invalidTlsCertificateInfoError;
		wrapper_SSL_CTX_free(pSslCtx);
		return Sdf_co_fail;
    }

	/* setting CAPath */
	/* CSR_1-5036521*/
    if (wrapper_SSL_CTX_load_verify_locations(pSslCtx, pTempSslParams->pCACertFile, \
				pTempSslParams->pCACertDirPath) != 1)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidTlsCertificateInfoError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
			"Error in loading CACertDirPath.",pErr);
#endif
		pErr->errCode=Sdf_en_invalidTlsCertificateInfoError;
		wrapper_SSL_CTX_free(pSslCtx);
		return Sdf_co_fail;
    }
    /* CSR_1-5036521*/

	if (wrapper_SSL_CTX_set_default_verify_paths(pSslCtx) != 1)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidTlsCertificateInfoError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
			"Error loading default CA file and/or directory.",pErr);
#endif
		pErr->errCode=Sdf_en_invalidTlsCertificateInfoError;
		wrapper_SSL_CTX_free(pSslCtx);
		return Sdf_co_fail;
    }

	/* Set the Certificate verification mode for this SSL_CTX. */
	wrapper_SSL_CTX_set_verify(pSslCtx, pTempSslParams->dCertVerifyMode, \
			sdf_cbk_uaVerifyCeritificate);
		
	/* Loading the ciphersuite list */
	pCipherList[0] = '\0';

	if (pTempSslParams->pCipherSuites != Sdf_co_null)
	{
		Sdf_ty_s8bit    pSslCipherStr[Sdf_co_maxCipher_suite_NameLength] = "\0";
		Sdf_ty_s8bit    *pTlsCipherStr = Sdf_co_null;
		Sdf_ty_s8bit    *pTempCipherList;

		pTempCipherList = Sdf_mc_strdupConfig(pTempSslParams->pCipherSuites);

		pTlsCipherStr = Sdf_mc_strtok(pTempCipherList, ",");

		while(pTlsCipherStr != Sdf_co_null)
		{
			if (sdf_fn_uaMapCipherName(pTlsCipherStr, pSslCipherStr,\
					Sdf_co_maxCipher_suite_NameLength) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_invalidTlsCertificateInfoError, \
					(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
					"Error mapping TLS cipher name to SSL cipher name.",pErr);
#endif
				pErr->errCode=Sdf_en_invalidTlsCertificateInfoError;
				(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
				              (Sdf_ty_pvoid*)&(pTempCipherList), pErr);
		        wrapper_SSL_CTX_free(pSslCtx);
				return Sdf_co_fail;
			}

			Sdf_mc_strcat(pCipherList, pSslCipherStr);
			Sdf_mc_strcat(pCipherList, ":");

		    pTlsCipherStr = Sdf_mc_strtok(Sdf_co_null, ",");
		}

		if (pCipherList[0] == '\0')
		{
			/* 
			 * The string configured was a NULL string. Configure the 
			 * default cipher list. 
			 */
		    Sdf_mc_strcpy(pCipherList, Sdf_mc_defaultCipherList);
		}

		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&(pTempCipherList), pErr);
	}
	else
	{
		/* Configure the defult cipher list. */
		Sdf_mc_strcpy(pCipherList, Sdf_mc_defaultCipherList);
	}

	if(wrapper_SSL_CTX_set_cipher_list(pSslCtx, pCipherList) != 1)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidTlsCiphersInfoError, (Sdf_ty_s8bit *)\
			"sdf_fn_uaFormSslData( ): Failed to load cipher-suites.",pErr);
#endif
		pErr->errCode=Sdf_en_invalidTlsCiphersInfoError;
		wrapper_SSL_CTX_free(pSslCtx);
		return Sdf_co_fail;
	}

	/* Loading DH param file Info */
	if (pTempSslParams->pDHParamFile != Sdf_co_null)
	{
		pDHParamfp = sdf_fn_uaFileOpen(pTempSslParams->pDHParamFile,\
			(Sdf_ty_s8bit *)"r");

		if(pDHParamfp == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_DHParamFileError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
				"Failed to read DH Param file.",pErr);
#endif
			pErr->errCode=Sdf_en_DHParamFileError;
		    wrapper_SSL_CTX_free(pSslCtx);
			return Sdf_co_fail;
		}

		if (pTempSslParams->dDHParamFileFormat == Sdf_en_fileFormatPEM)
		{
			dh = wrapper_PEM_read_DHparams(pDHParamfp, Sdf_co_null, Sdf_co_null, \
					Sdf_co_null);
		}
		else
		{
			/* FORMAT_ASN1 case */
			dh = wrapper_d2i_DHparams_fp(pDHParamfp, Sdf_co_null);
		}

		/*w534*/ (void)sdf_fn_uaFileClose(pDHParamfp);

		if(Sdf_co_null != dh)
		{
			/*w534*/ (void)wrapper_SSL_CTX_set_tmp_dh(pSslCtx, dh);
			/*w534*/ (void)wrapper_SSL_CTX_set_options(pSslCtx,OP_SINGLE_DH_USE);
			wrapper_DH_free(dh);
		}
		else
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_DHParamFileError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaFormSslData( ):"
				"Failed to Load DH Param file.",pErr);
#endif
			pErr->errCode=Sdf_en_DHParamFileError;
			wrapper_SSL_CTX_free(pSslCtx);
			return Sdf_co_fail;
		}
	}

	/* To use all the options available with the SSL library. */
	/*w534*/ (void)wrapper_SSL_CTX_set_options(pSslCtx,OP_ALL);

	/* 
	 * This is required for "close notify" alert messages (which are part
	 * of SSL Handshake messages.
	 */
    wrapper_SSL_CTX_set_quiet_shutdown(pSslCtx,\
            SDF_SSL_SENT_SHUTDOWN | SDF_SSL_RECEIVED_SHUTDOWN);

	/* SSL_CTX formed. Now set it as the pData of pSslData. */
	pSslData->pData = (Sdf_ty_pvoid)pSslCtx;

	return Sdf_co_success;
}

#endif /* SDF_TLS */

/* Socket fdset related functions */

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitFdSetInfo
 **
 ** DESCRIPTION: This function is used to initialise the 
 **				 Socket Information structure which can be used to store
 **				 socket fd's and max fd in use. This structure is mutex
 **				 protected.
 **
 ******************************************************************************/

Sdf_ty_retVal	sdf_fn_uaInitFdSetInfo
(Sdf_st_fdSetInfo *pFdSetInfo)
{
	FD_ZERO(&(pFdSetInfo->dSockFdSet));
	pFdSetInfo->dMaxFdVal = Sdf_co_InvalidSocketFd;
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitMutex(&(pFdSetInfo->dFdsetMutex), Sdf_co_false);
#endif
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaSetSocketFd
 **
 ** DESCRIPTION: This function is used to Set the new fd into the fdset 
 **				 maintained by UATK and also update the max value of fd used.
 **				 used by application to form an fd_set for network handling.
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaSetSocketFd
(Sdf_st_fdSetInfo *pFdSetInfo, Sdf_ty_socketId dSockfd)
{
	if(dSockfd == Sdf_co_InvalidSocketFd)
			return Sdf_co_fail;

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &(pFdSetInfo->dFdsetMutex), 0);
#endif
		FD_SET((Sdf_ty_u32bit)dSockfd, &(pFdSetInfo->dSockFdSet));
	/*to avoid compiler warning dsockfd is typecasted*/
	if( pFdSetInfo->dMaxFdVal < dSockfd )
		pFdSetInfo->dMaxFdVal = dSockfd;
	
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pFdSetInfo->dFdsetMutex));
#endif
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaGetFdSetInfo
 **
 ** DESCRIPTION: Application can use this function to copy the information
 **				 about the fd_set and max fd value maintained at UATK.
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaGetFdSetInfo
(Sdf_st_fdSetInfo *pDstFdSetInfo, Sdf_st_fdSetInfo *pSrcFdSetInfo)
{
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &(pDstFdSetInfo->dFdsetMutex), 0);
#endif
	pDstFdSetInfo->dSockFdSet = pSrcFdSetInfo->dSockFdSet;
	pDstFdSetInfo->dMaxFdVal = pSrcFdSetInfo->dMaxFdVal;
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pDstFdSetInfo->dFdsetMutex));
#endif
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaClearSocketFd
 **
 ** DESCRIPTION: This function removes the specified socket fd from the
 **				 Maintained socket fd set.
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaClearSocketFd
(Sdf_st_fdSetInfo *pFdSetInfo, Sdf_ty_socketId dSockfd)
{
	if(dSockfd == Sdf_co_InvalidSocketFd)
			return Sdf_co_fail;
			
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &(pFdSetInfo->dFdsetMutex), 0);
#endif
	FD_CLR((Sdf_ty_u32bit)dSockfd, &(pFdSetInfo->dSockFdSet));
	/*to avoid compiler warning dsockfd is typecasted*/
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pFdSetInfo->dFdsetMutex));
#endif
	return Sdf_co_success;
}
/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaClearFdSetInfo
 **
 ** DESCRIPTION: This function is used to Clear the socket info resources.
 **
 ******************************************************************************/

Sdf_ty_retVal	sdf_fn_uaClearFdSetInfo
(Sdf_st_fdSetInfo *pFdSetInfo)
{
	FD_ZERO(&(pFdSetInfo->dSockFdSet));
	pFdSetInfo->dMaxFdVal = Sdf_co_InvalidSocketFd;
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaDestroyMutex(&(pFdSetInfo->dFdsetMutex));
#endif
	return Sdf_co_success;
}

#ifdef ICF_UATK_PORT
/* this function is added by vidyut for strtok_r*/
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_SYMBIAN)

char * win_self_implemented_strtok_r(char *s, const char *delim, char **last)
 {
   char *spanp;
   int c, sc;
   char *tok;
 
   if (s == NULL && (s = *last) == NULL)
   {
     return NULL;
   }
 
   /*
    * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
    */
 cont:
   c = *s++;
   for (spanp = (char *)delim; (sc = *spanp++) != 0; )
   {
     if (c == sc)
     {
       goto cont;
     }
   }
 
   if (c == 0)  /* no non-delimiter characters */
   {
     *last = NULL;
     return NULL;
   }
   tok = s - 1;
 
   /*
    * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
    * Note that delim must have one NUL; we stop if we see that, too.
    */
   for (;;)
   {
     c = *s++;
     spanp = (char *)delim;
     do
     {
       if ((sc = *spanp++) == c)
       {
         if (c == 0)
         {
           s = NULL;
         }
         else
         {
           char *w = s - 1;
           *w = '\0';
         }
         *last = s;
         return tok;
       }
     }
     while (sc != 0);
   }
   /* NOTREACHED */
 }
#endif


/*This should have been found in estlib.lib. as per SDK */
/* But we are implementing it as we could not find it*/
#ifdef ICF_PORT_SYMBIAN
void bzero(char* ptr, size_t  len)
{
	memset(ptr, 0, len );
}
#endif

#ifdef SDF_VXWORKS
/* Function definition added for VxWorks */
Sdf_ty_s32bit sdf_fn_strcasecmp_vx(
       Sdf_ty_u8bit *p_target, 
       Sdf_ty_u8bit *p_match)
{
    Sdf_ty_u32bit 							i = 0;
    Sdf_ty_u8bit							temp1 = 0;
    Sdf_ty_u8bit							temp2 = 0;

	while(p_target[i])
	{ 
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((p_target[i] >= 97) && (p_target[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp1 = p_target[i] - 32;
		}
		else
		{
			temp1 = p_target[i];
		}
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((p_match[i] >= 97) && (p_match[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp2 = p_match[i] - 32;
		}
		else
		{
			temp2 = p_match[i];
		}

		if (temp1 != temp2)
			return(temp1 - temp2);
		i++;
	}

	if(p_match[i])
		return(0 - p_match[i]);

	return 0;

}/* Function sdf_fn_strcasecmp_vx() ends here */
#endif
#endif /*** ICF_UATK_PORT ***/


#ifdef SDF_DEP
/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepMgrPostStateChange
 **
 ** DESCRIPTION: This is an event manager function used in case of DEP.This function
 **							 is used to post any changes on to the event queue. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDepMgrPostStateChange(Sdf_st_DialogUpdate *updateElement,Sdf_st_error *pErr)
{
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null )
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"Invalid pErr passed to sdf_fn_uaDepMgrPostStateChange ");
#endif
		return Sdf_co_fail;
	} 
   	if(updateElement == Sdf_co_null)
	{
#ifdef SDF_ERROR
  	  sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
		(Sdf_ty_s8bit *)"sdf_fn_uaDepMgrPostStateChange():"
		"Invalid updateElement pointer passed to the function",pErr);
#endif
	  pErr->errCode=Sdf_en_invalidParamError;
	  return Sdf_co_fail;  	
	}
 
#endif

#ifdef SDF_DEP_SYNC_HANDLING
	/* If the Application wants the event to be handled in a synchronous
	 * way, ie the decode thread or the thread which initiates a dialog,
	 * call the ivk which handles the trigger here.
	 */
	if(sdf_ivk_uaDepFormAndSendNotify(updateElement,pErr) == Sdf_co_fail)
	{
		printf("\n*******************************8\n");  
		printf("\n****FAILURE IN sdf_ivk_uaDepFormAndSendNotify\n");  
		printf("\n*******************************8\n");  
	}
   sdf_ivk_uaDepMgrFreeDialogUpdate(updateElement);
#else
	/* Lock Mutex before accessing message queue */
	sdf_fn_uaLockMutex(0, &dStateChangeMutex, 0);

	/* add the event to the Queue */
	if (sdf_listAppend(&slStateChangeQueue, \
				(Sdf_ty_pvoid)updateElement, pErr) == Sdf_co_fail)
	{
		/* UnLock Mutex */
		sdf_fn_uaUnlockMutex(0, &dStateChangeMutex);
		return Sdf_co_fail;
	}

	/* UnLock Mutex */
	sdf_fn_uaUnlockMutex(0, &dStateChangeMutex);
#endif	
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepFetchUpdateElement
 **
 ** DESCRIPTION: This is an event manager function used in case of DEP.This function
 **							 is used to fetch an element from the event queue. 
 **
 ******************************************************************************/
Sdf_st_DialogUpdate* sdf_fn_uaDepFetchUpdateElement()
{

	Sdf_st_DialogUpdate *pUpdateElement = Sdf_co_null;
	Sdf_ty_u32bit	dSize=0;
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering " 
			"sdf_fn_uaDepFetchUpdateElement");	

	while(1)
	{ 
		/* Lock Mutex before accessing message queue */
		sdf_fn_uaLockMutex(0, &dStateChangeMutex, 0);
		if(sdf_listSizeOf(&slStateChangeQueue,&dSize,&dError)==Sdf_co_fail)
		{
			sdf_fn_uaUnlockMutex(0, &dStateChangeMutex);
			continue;
		}

		if(dSize == 0)
		{
			/* 
			 * Queue has no elements currently. Wait for a while and
			 * continue in loop 
			 */	
			sdf_fn_uaUnlockMutex(0, &dStateChangeMutex);
			sdf_fn_uaSleep(0,1000);
			(void)sdf_fn_uaTestCancel(Sdf_co_null);
			continue;
		}
		else
		{
			/* 
			 * Pick up the first element in the queue 
			 */
			if(sdf_listGetAt(&slStateChangeQueue, 0, (Sdf_ty_pvoid *)&pUpdateElement,\
						&dError) == Sdf_co_fail)
			{
				sdf_fn_uaUnlockMutex(0, &dStateChangeMutex);
				continue;
			}

			/* Delete the element from the queue */
			if (sdf_listDeleteAt(&slStateChangeQueue, 0, &dError) == Sdf_co_fail)
			{
				sdf_fn_uaUnlockMutex(0, &dStateChangeMutex);
				continue;
			}

			/* Element has been extracted from queue. Mutex can be unlocked */
			sdf_fn_uaUnlockMutex(0, &dStateChangeMutex);

			/* Return the element to calling function */
			break;
		}
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting" 
			"sdf_fn_uaDepFetchUpdateElement");	
	return pUpdateElement;
}

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepInitEventQueue
 **
 ** DESCRIPTION: This is an event manager function used to initialize the state
 **              change event queue. 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDepInitEventQueue(Sdf_st_error *pErr)
{
	Sdf_ty_bool dThreadSafe;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering" 
			"sdf_fn_uaDepInitEventQueue");	
#ifdef SDF_THREAD_SAFE	
	dThreadSafe = Sdf_co_true;
	sdf_fn_uaInitMutex(&(dStateChangeMutex),Sdf_co_false);
#else
	dThreadSafe = Sdf_co_false;
#endif

	/* Init the event queue list */
	if (sdf_listInit(&slStateChangeQueue, \
				Sdf_co_null, dThreadSafe, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaDepInitEventQueue( )"
				"Failed to initalize event queue", pErr);
#endif
		return Sdf_co_fail;
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting" 
			"sdf_fn_uaDepInitEventQueue");	
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepFreeEventQueue
 **
 ** DESCRIPTION: This is an event manager function used to free the state
 **              change event queue. 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDepFreeEventQueue(Sdf_st_error *pErr)
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering" 
			"sdf_fn_uaDepFreeEventQueue");	
#ifdef SDF_THREAD_SAFE	
	sdf_fn_uaDestroyMutex(&dStateChangeMutex);
#endif
	/* First free the Any Dialog List */
	if(sdf_listDeleteAll(&slStateChangeQueue,pErr) ==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaDepFreeEventQueue(): "
				"Failed to delete AnyDialog list.", pErr);
#endif
	}
	(Sdf_ty_void)sdf_listFree(&slStateChangeQueue,pErr);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting" 
			"sdf_fn_uaDepFreeEventQueue");	
	return Sdf_co_success;
}

#endif
#ifdef ICF_UATK_PORT
/* IPTK CML Start */
/******************************************************************************
** FUNCTION: sdf_ivk_uaIPTKCheckMessageLength
**
** DESCRIPTION: This API checks if the length of any of the header 
**				(present in SIP message) is more than the specified length.
**
** PARAMETERS:
**		pString(IN)	: Incoming SIP Message (plain text)
**		dLen(IN)	: Boundary length. All the headers in 'pString' is checked
**					  to see if any of the headers exceeds this length.
        pOption(IN)	: Specifies, which are the headers to be tested
**		pResult(OUT): Specifies whether or not this message contains a header of
**					  length more than 'dLen'
**		pError(OUT)	: Error structure in case the function fails
**
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaIPTKCheckMessageLength(Sdf_ty_s8bit *pString, 
		Sdf_ty_u32bit dLen,Sdf_ty_u32bit dLen2, Sdf_ty_u32bit *pOption,
        Sdf_ty_IPTKmessageLengthValidity *pResult, Sdf_st_error *pError)
{
    Sdf_ty_s8bit *pTempStr = Sdf_co_null;
	Sdf_ty_s8bit *pTempStr1 = Sdf_co_null;
	Sdf_ty_s8bit *pTempStr2 = Sdf_co_null;
	Sdf_ty_s8bit *phdrName = Sdf_co_null;
	Sdf_ty_s8bit *phdrValue = Sdf_co_null;
    Sdf_ty_bool dReqRespLine = 1;
	/* Codenomicon fixes: CSR 1-7132323: SPR 19484 */
    Sdf_ty_bool dMsgBodyReached = 0;
	Sdf_ty_u32bit	tmpVar = 0;
	/* Fix for IPTK CSR 1-7160337: SPR 19508 */
    Sdf_ty_bool dColonReceived = 1;
    Sdf_ty_u32bit dOption = *pOption;	

    (void)dOption;
    sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaIPTKCheckMessageLength");

#ifdef SDF_PARAMVALIDATION
if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
				"sdf_ivk_uaCheckMessageLength(): pError param passed is Invalid");
#endif
		return Sdf_co_fail;
	}
    if (pString == Sdf_co_null)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCheckMessageLength(): "
				"pString/pResult param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
#endif

	if((dLen == 0) || (dLen2 == 0) || (dOption != 0))
            return Sdf_co_fail;
    *pResult = Sdf_en_validlength;
	Sdf_mc_strdup(pTempStr, pString);
#ifdef SDF_LINT
    	if(pTempStr == Sdf_co_null)
        	return Sdf_co_fail;
#endif    
	pTempStr2 = pTempStr;

	/* Codenomicon fixes: CSR 1-7132323: SPR 19484 */
	while(*pTempStr != '\0')
	{
		phdrName = Sdf_co_null;
		phdrValue = Sdf_co_null;
		dColonReceived = 1;

		pTempStr1 = Sdf_mc_strchr(pTempStr, '\r');
		/* Break if source string is exhausted */
		if (pTempStr1 == Sdf_co_null)
		{
			pTempStr1 = Sdf_mc_strchr(pTempStr, '\n');
			if(pTempStr1 == Sdf_co_null)
			{
				pTempStr1 = Sdf_mc_strchr(pTempStr, '\0');
			}
		}
        if(pTempStr1 == Sdf_co_null || pTempStr == Sdf_co_null)
            		return Sdf_co_fail;
        else
        {          
			/* -------------------------------------------------- */
			/* Check if we have reached the line that separates
			 * msg body from headers above it.
			 */       
			/* -------------------------------------------------- */
			if (pTempStr1 == pTempStr)
			{
				dMsgBodyReached = 1;

				/* Proceed to check the next line */
            	if(*pTempStr1 == '\0')
                	pTempStr = pTempStr1;
            	else
                	pTempStr = pTempStr1+1;

				if (*pTempStr == '\n')
					pTempStr = pTempStr + 1;

				continue;

			}

			/* Codenomicon updated fixes: CSR 1-7132323: SPR 19484 */
			/* -------------------------------------------------- */
            /* Updated fix: Remove all checks for message body.
             * SSA will take care to copy only 127 bytes from
             * SDP parameters.
             */
			/* -------------------------------------------------- */
			if (1 == dMsgBodyReached)
			{
				break;
			}
 
			/* -------------------------------------------------- */
			/* Total length of hdr name + value should not exceed
			 * (dLen + dLen2 - 1) bytes.
		 	 */
			/* -------------------------------------------------- */
			if ((dLen + dLen2 - 1) < ((Sdf_ty_u32bit)(pTempStr1 - pTempStr)))
			{
				/* If this was the 1st line, set the flag for Req-line also.
	 			 * This may be used to send 414 response.
	             */
				if (dReqRespLine == 1)
				{
        			*pResult = *pResult | Sdf_en_reqresplinelong;
				}
				else
				{
					/* do not get misled by the result code */
					/* If both these are set, it means that the 
				 	 * complete header is long. It does not 
				 	 * necessarily mean that any of these 2 is 
				 	 * long.
				 	 */
					*pResult = *pResult | Sdf_en_headernamelong;
					*pResult = *pResult | Sdf_en_headervaluelong;
				}

				break;
			}
			/* Subsequent checks should not be performed for req-line */
			else if (0 == dReqRespLine)
			{
				/* ------------------------------------------------ */
				/* Extract hdr name and value from the msg line */
				/* ------------------------------------------------ */
				/* If ':' is found, this function returns pointer to
				 * ':' else it returns null.
				 */
				phdrValue = sip_tokenize(pTempStr, pTempStr1, ':');

				/* For empty headers containing name followed by colon
				 * but no value, it may happen that phdrValue retrieved
				 * above contains the sequence ":\r\n" or ":\n" or ":\0".
				 * This is as good as an empty value.
				 */
				if (Sdf_co_null == phdrValue)
				{
					dColonReceived = 0;
				}
				else if ((Sdf_co_null != phdrValue) &&
						((*(phdrValue + 1) == '\r') || 
						 (*(phdrValue + 1) == '\n') ||
						 (*(phdrValue + 1) == '\0')))
				{
					phdrValue = Sdf_co_null;
				}

                if (Sdf_co_null != phdrValue)
                {
				    phdrValue = phdrValue + 1;
				    tmpVar = (Sdf_ty_u32bit)(phdrValue - pTempStr);
				    phdrName = phdrValue - tmpVar;
					phdrName[tmpVar-1] = '\0';
				}
                else
				{
					tmpVar = (Sdf_ty_u32bit)(pTempStr1 - pTempStr);
					phdrName = pTempStr;
					phdrName[tmpVar-1] = '\0';
				}

				/* ------------------------------------------------ */
				/* Length of hdr name cannot be null and should not
				 * exceed dLen bytes.
				 */
				/* ------------------------------------------------ */
				if (Sdf_co_null == phdrName)
				{
					/* do not get misled by the result code */
					*pResult = *pResult | Sdf_en_headernamelong;
					break;
				}
				else if (dLen < Sdf_mc_strlen(phdrName))
				{
					*pResult = *pResult | Sdf_en_headernamelong;
					break;
				}

				/* ------------------------------------------------ */
				/* Length of hdr value is checked as follows:
				 *
               			 * Updated fix for CSR 1-7132323: SPR 19484:
				 * Empty header value is permitted here.
				 *
                 		 * If hdr value is not empty, checks are as follows:
                 		 *
				 * (a) For Event, SIP-ETag, Content-Type and Warning hdrs,
				 * value should be less than dLen bytes.
				 *
				 * (b) For Call-ID, value should be less than 
				 * internal constant ICF_MAX_NW_CALL_ID_DGTS.
				 *
				 * For all other hdrs, it should be less than dLen2.
				 *
				 * Codenomicon updated fixes: CSR 1-7132323: SPR 19484:
				 * Header name comparison should be case-insensitive and
                 		 * should allow spaces.
				 *
				 */
				/* ------------------------------------------------ */
				if (Sdf_co_null == phdrValue)
				{
                    			/* Updated fix for CSR 1-7132323: SPR 19484:
				 	 * Empty header value is permitted here.*/
                    			/* do nothing */
				}
               			 /* Fix for SPR 19895 : Pass a char* to icf_port_strstri2 and not a &(char*). */
				else if ((Sdf_co_null != icf_port_strstri2((icf_uint8_t*)(phdrName), (icf_uint8_t*)&("Event"))) ||
						 (0 == Sdf_mc_strcmp(phdrName, "o")) ||
						 (Sdf_co_null != icf_port_strstri2((icf_uint8_t*)(phdrName),(icf_uint8_t*)&("SIP-ETag"))) ||
						 (Sdf_co_null != icf_port_strstri2((icf_uint8_t*)(phdrName), (icf_uint8_t*)&("Content-Type"))) ||
						 (0 == Sdf_mc_strcmp(phdrName, "c")) ||
						 (Sdf_co_null != icf_port_strstri2((icf_uint8_t*)(phdrName), (icf_uint8_t*)&("Warning"))))
				{
					#if 1 /*ZyXEL , Jason , temporal modification*/
					if (dLen2 < ((Sdf_ty_u32bit)(pTempStr1-phdrValue)))					
					#else
					if (dLen < ((Sdf_ty_u32bit)(pTempStr1-phdrValue)))
					#endif	
					{
						*pResult = *pResult | Sdf_en_headervaluelong;
						break;
					}
				}
                /* Fix for SPR 19895 : Pass a char* to icf_port_strstri2 and not a &(char*). */
				else if ((Sdf_co_null != icf_port_strstri2((icf_uint8_t*)(phdrName),
                               		 (icf_uint8_t*)&("Call-ID"))) ||
                         		(0 == Sdf_mc_strcmp(phdrName, "i"))) 
				{
					if (ICF_MAX_NW_CALL_ID_DGTS < ((Sdf_ty_u32bit)(pTempStr1-phdrValue)))
					{
						*pResult = *pResult | Sdf_en_headervaluelong;	
						break;
					}
				}
				else
				{
					if (dLen2 < ((Sdf_ty_u32bit)(pTempStr1-phdrValue)))
					{
						*pResult = *pResult | Sdf_en_headervaluelong;
						 printf("\r\n Jason , test4444444444444444444444444"); 		
						break;
					}
				}

			} /* End else */

			dReqRespLine = 0;

			/* Proceed to check the next line */
            if(*pTempStr1 == '\0')
                pTempStr = pTempStr1;
            else
                pTempStr = pTempStr1+1;

			if ((*pTempStr == '\n') && (*(pTempStr - 1) == '\n'))
			{
				dMsgBodyReached = 1;
				pTempStr = pTempStr + 1;
			}
			else if (*pTempStr == '\n')
			{
				pTempStr = pTempStr + 1;
			}
		}
	} /* End while */

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pTempStr2),
                      pError);
    return Sdf_co_success;
}
#endif


