
/*****************************************************************************
 ** FUNCTION:
 **          This file has all APIs for abstraction of system dependant 
 **			 functions. 
 **
 *****************************************************************************
 **
 ** FILENAME	: 	sdf_portlayer.h
 **
 ** DESCRIPTION	:	This file has all APIs for abstraction of system 
 **					dependant functions. It might need to be modified 
 **					for porting to a different plaform.
 **
 **
 ** DATE        NAME              REFERENCE               REASON
 ** ----        ----              ---------              --------
 ** 23/02/01   Rajasri			Initial Creation    Move all porting 
 **													functions to one file.	
 ** 22/04/03   Anil Kumar           ---             Added macros for some
 **                                                 socket library functions.
 ** 29-Jan-2009 Alok Tiwari    ICF Rel-8.1       Changes done for 
 **                               flag ICF_UATK_PORT.This flag filters
 **                               the ICF related code/changes from this file.
 ** 27-Feb-2009 Alok Tiwari    ICF Rel-8.1      New hash define
 **                                             SDF_ADIGEST_HEX_CHECKSUM_LEN
 **                                             added used to integrate aDigest.
 ** 13-Jul-2009 Rajiv Kumar    IPTK Rel8.2      Changes for Openssl Removal.
 ** 20-Aug-2009 Anuradha Gupta SPR 19935        Vxworks- snprintf changes
 ** 26-Sep-2009 Rajiv Kumar    SPR 19991        Fix for SPR: 19991
 ** 29-Apr-2010 Rohit Aggarwal  CSR 1-8534364    Make snprintf OS independent
 **
 ****************************************************************************
 **     		COPYRIGHT , Aricent, 2010
 ****************************************************************************/


#ifndef __SDF_PORTLAYER_H_
#define __SDF_PORTLAYER_H_
/* SPR-4048 : extern C statement is added after the header file
 * declaration.(ssf team gatting compilation problem with g++)*/
/*#ifdef __cplusplus
extern "C" {
#endif */

#include <ctype.h>
#ifdef ICF_UATK_PORT
#include "icf_port_prototypes.h"
#include "icf_map_extif_feature_flags.h"
#else /****************ICF_UATK_PORT****************/
#include <string.h>
#endif /****************ICF_UATK_PORT****************/

#include "microsip_portlayer.h"
#include "sdf_basictypes.h"

#ifndef ICF_PORT_SYMBIAN
#ifdef SDF_TLS
/* VxWorks Ported code */
/* Old Code */
#include "openssl/x509.h"
#include "openssl/dh.h"
#include "openssl/pem.h"
#include "openssl/ssl.h"
#include "openssl/rand.h"
#endif	/* End of TLS */
#endif /*End of Port symbian*/

#ifdef ICF_UATK_PORT
#ifdef SDF_VXWORKS

#include "vxWorks.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "memLib.h"
#include "timers.h"
#include "inetLib.h"
#include "in.h"
#include "arpa/inet.h"
#include "msgQLib.h"
#include "tickLib.h"
#include "time.h"
#include "sys/times.h"
#include "sockLib.h"
#include "ctype.h"
#include "string.h"
#include "ioLib.h"
#include "ioctl.h"
#include "assert.h"
#include "hostLib.h"
#include "taskLib.h"
#include "kernelLib.h"
#include "errnoLib.h"
#include "resolvLib.h"
#include "netdb.h"
#include "types.h"
#include "mbuf.h"
#include "socket.h"
#include "socketvar.h"
#include "semaphore.h"
#include "signal.h"
#include "ramDrv.h"
#include "dosFsLib.h"
#include "ftpLib.h"
#include "ioLib.h"
#ifdef SDF_IPV6
#include "netinet6/in6.h"
#include "netinet6/ip6.h"
#endif

#endif
#endif /****************ICF_UATK_PORT****************/

#ifdef SDF_AUTHENTICATE
/* This value is same as ADIGEST_HEX_CHECKSUM_LEN defined
 * in aDigest (an Aricent open source software). It's value
 * will be equal to 32 as defined in RFC-2617.
 */ 

#define SDF_ADIGEST_HEX_CHECKSUM_LEN  32
#endif

#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#endif

#ifdef ICF_UATK_PORT
#if defined(ICF_PORT_SYMBIAN)
#include <string.h>
#endif
#if defined(SDF_WINDOWS) || defined(ICF_PORT_WINDOWS)
#ifndef ICF_PORT_WINCE
#include <process.h>
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#else  /****************ICF_UATK_PORT****************/
#if defined (SDF_WINDOWS) && defined (SDF_IPV6)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#endif /****************ICF_UATK_PORT****************/



#if defined(SDF_SOLARIS) ||  defined(SDF_LINUX)
#include <strings.h>
#endif

/*#include <io.h>*/


#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif 

#include "sdf_common.h"
#include "sdf_struct.h"
/* SPR-4048 : extern C statement is added after the header file
 * declaration.(ssf team gatting compilation problem with g++)*/
#ifdef __cplusplus
extern "C" {
#endif 

#ifdef SDF_DEBUG
#define sdf_fn_debug(str) 	sdf_fn_uaDisplayFnDebug(str)
#else
#define sdf_fn_debug(str)
#endif

#ifdef SDF_ERROR
#define sdf_fn_dumpError(str) 	sdf_fn_uaDisplayFnError(str)
#else
#define sdf_fn_dumpError(str)
#endif

#define Sdf_mc_displayFnTrace(pStr,dLen,pErr) \
            Sdf_fn_uaDisplayFnTrace(pStr,dLen,pErr)

#define Sdf_mc_deleteRemoteRetransThread(pRetransThread,ptr) \
            Sdf_fn_uaDeleteRemoteRetransThread(pRetransThread,ptr)

#define Sdf_mc_createRemoteRetransThread(pRetransThread,pFuncPtr,pErr) \
        sdf_fn_uaCreateRemoteRetransThread(pRetransThread,pFuncPtr,pErr)


#ifdef SDF_CALLFLOW 

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#define Sdf_mc_displayCallFlow(dCallFlowSockfd,buf) \
        send(dCallFlowSockfd, buf, Sdf_mc_strlen(buf),0)
#else
#define Sdf_mc_displayCallFlow(dCallFlowSockfd,buf)
#endif

#define Sdf_mc_uaTestCancel sdf_fn_uaTestCancel(Sdf_co_null);

#endif                                                  

#define Sdf_mc_uaGetHostIp(pHost,pIp,dAddrAny,pErr) \
               (void)sdf_fn_uaGetHostName(&pHost, pErr);\
			   (void)sdf_fn_uaGetHostIp(pHost,&pIp, pErr);\
			   dAddrAny=Sdf_co_true/*534*/

#define sdf_fn_uaGetHostIp(pHost,pIp, pErr) sdf_cbk_uaGetHostIp(pHost,pIp,pErr)

#define Sdf_mc_setSysClkRate 

#if defined(SDF_SOLARIS) || defined(SDF_LINUX)
#define Sdf_mc_uaCancelThread			PTHREAD_CANCEL_ENABLE
#endif
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#define Sdf_mc_uaCancelThread 			0
#endif

/*
 * Timer ticks per second on the system clock;
 * needs to be atleast 100 for the timer logic to work properly
 */
/*
 * The max retry duration. A random value within this range is chosen
 * every time a Retry-After is to be inserted in a SIP message by the 
 * UA toolkit
 */
#define Sdf_mc_maxRetryDuration 10

/* For the INET address len */
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || defined(SDF_VXWORKS)

#define Sdf_mc_INET_ADDRSTRLEN		INET_ADDRSTRLEN
#define Sdf_mc_INET6_ADDRSTRLEN		INET6_ADDRSTRLEN
#else
				 
/*  INET_ADDRSTRLEN for IPV4 has been hard-coded here 
	to avoid compilation errors/warnings */
#if defined(SDF_WINDOWS) || defined(SDF_WINCE)
#define Sdf_mc_INET_ADDRSTRLEN		22
#define Sdf_mc_INET6_ADDRSTRLEN		INET6_ADDRSTRLEN
#endif
#endif


/* The fixed part of from/to tags. The tags generated by this app will look like 
 * hssua_XXXXX where XXXXX is some randomly generated string */
/* ZyXEL porting */
#if 1 
#define Sdf_co_fixedTag			"ZyXELUA"
#else
#define Sdf_co_fixedTag			"AricentUA"
#endif
/* ZyXEL porting end*/
#define SDF_MAX_CTIME_R_BUF_SIZE 26
/* ZyXEL porting */
#if 1 
#define Sdf_co_boundary			"ZyXELUA"
#else
#define Sdf_co_boundary			"AricentUA"
#endif
/* ZyXEL porting end*/

#ifdef SDF_LINT
extern Sdf_ty_s32bit sdf_fn_uaLintStrcasecmp _ARGS_ ((const Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2));
extern Sdf_ty_s32bit sdf_fn_uaLintStrcmp _ARGS_ ((const Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2));
extern Sdf_ty_s32bit sdf_fn_uaLintStrncmp _ARGS_ ((const Sdf_ty_s8bit *s1,const \
            Sdf_ty_s8bit *s2,Sdf_ty_u32bit n));
extern void sdf_fn_uaLintStrcat _ARGS_ ((Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2));
/*Codenomicon fix start: CSR_1-6714693*/
extern void sdf_fn_uaLintStrncat _ARGS_ ((Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2,Sdf_ty_u32bit n));
/*Codenomicon fix end: CSR_1-6714693*/
extern void sdf_fn_uaLintStrcpy _ARGS_ ((Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2));
extern void sdf_fn_uaLintStrncpy _ARGS_ ((Sdf_ty_s8bit *s1,const \
            Sdf_ty_s8bit *s2,Sdf_ty_u32bit n));
#if defined(SDF_SOLARIS) || defined(SDF_VXWORKS)
extern Sdf_ty_s8bit* sdf_fn_uaLintStrtok _ARGS_ ((Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2));
#endif
extern Sdf_ty_u32bit sdf_fn_uaLintStrlen _ARGS_ ((const Sdf_ty_s8bit *s1));
extern Sdf_ty_s8bit* sdf_fn_uaLintStrstr _ARGS_ ((Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2));
#if defined(SDF_SOLARIS) || defined(SDF_VXWORKS) || defined(SDF_OSE) ||defined( SDF_LINUX) || \
			defined(SDF_WINDOWS)
extern Sdf_ty_s8bit* sdf_fn_uaLintStrtokr _ARGS_ ((Sdf_ty_s8bit *s1,const \
            Sdf_ty_s8bit *s2,Sdf_ty_s8bit **s3 ));
extern Sdf_ty_s8bit* sdf_fn_uaLintStrdup _ARGS_ ((const Sdf_ty_s8bit \
            *s1,Sdf_ty_s32bit id ));
extern Sdf_ty_s32bit sdf_fn_uaLintAtoI _ARGS_ ((const Sdf_ty_s8bit *s1));
extern Sdf_ty_s8bit* sdf_fn_uaLintStrpbrk _ARGS_ ((const Sdf_ty_s8bit *s1,const Sdf_ty_s8bit *s2));
#endif /* End of SDF_SOLARIS || SDF_VXWORKS || SDF_OSE */

#endif /* End of SDF_LINT */
#ifdef SDF_VXWORKS
extern Sdf_ty_s32bit sdf_fn_strcasecmp_vx(
       Sdf_ty_u8bit *p_target, 
       Sdf_ty_u8bit *p_match);

#endif

#ifdef SDF_LINT
#define Sdf_mc_strdup(a,b) \
    ( (a) = (Sdf_ty_s8bit *)sdf_fn_uaLintStrdup(b,0) ) 

#define Sdf_mc_strcat(a,b) sdf_fn_uaLintStrcat(a,b)
#define Sdf_mc_strcpy(a,b) sdf_fn_uaLintStrcpy((Sdf_ty_s8bit*)a,(Sdf_ty_s8bit*)b)
#define Sdf_mc_strncpy(a,b,n) sdf_fn_uaLintStrncpy(a,b,n)
/*Codenomicon fix start: CSR_1-6714693*/
#define Sdf_mc_strncat(a,b,n) sdf_fn_uaLintStrncat(a,b,n)
/*Codenomicon fix end: CSR_1-6714693*/
#else
/*Codenomicon fix start: CSR_1-6714693*/
#define Sdf_mc_strncat(a,b,n) \
do \
{ \
	if ((b!=SIP_NULL)) strncat (((Sdf_ty_s8bit *)a),((Sdf_ty_s8bit *)b),n);\
}\
while(0)    
/*Codenomicon fix end: CSR_1-6714693*/
#define Sdf_mc_strdup(a, b) \
	( (a) = (Sdf_ty_s8bit *) (((b)!=NULL)?sip_strdup(b,0):NULL) ) 
    
#define Sdf_mc_strcat(a,b) \
do \
{ \
	if ((b!=SIP_NULL)) strcat (((Sdf_ty_s8bit *)a),((Sdf_ty_s8bit *)b));\
}\
while(0)
    
#define Sdf_mc_strcpy(a,b) \
do \
{ \
	if ((b!=SIP_NULL)) strcpy (((Sdf_ty_s8bit *)a),((Sdf_ty_s8bit *)b));\
}\
while(0)

    
#define Sdf_mc_strncpy(a,b,n) \
do \
{ \
	if ((b!=SIP_NULL)) strncpy (((Sdf_ty_s8bit *)a),((Sdf_ty_s8bit *)b),n);\
}\
while(0)
#endif /* End of Else SDF_LINT */

#define sdf_mc_init(x) \
do \
{ \
	x=Sdf_co_null;\
} \
while(0)

#ifdef SDF_LINT
#if defined(SDF_SOLARIS) || defined(SDF_VXWORKS)
#define Sdf_mc_strtok(x,y) 	sdf_fn_uaLintStrtok(x,y)
#endif
#define Sdf_mc_strlen(x) 		sdf_fn_uaLintStrlen((Sdf_ty_s8bit*)x) 
#define Sdf_mc_strstr(x,y)		sdf_fn_uaLintStrstr(x,y)
#else    
#if defined(SDF_SOLARIS) || defined(SDF_VXWORKS)
#define Sdf_mc_strtok(x,y) 	(((y)!=NULL)?strtok(x,y):NULL)
#endif
#define Sdf_mc_strlen(x) 		(((x)!=NULL)?strlen(x):0) 
#define Sdf_mc_strstr(x,y)		((((y)!=NULL)&&((x)!=NULL))?strstr(x,y):NULL)
#endif /* End of Else SDF_LINT */

#define Sdf_mc_strchr(x,y)		(((x)!=NULL)?strchr(x,y):NULL)
#define Sdf_mc_sscanf		sscanf
#define Sdf_mc_fprintf		fprintf

#ifdef SDF_LINT
#define Sdf_mc_atoi(x) 		sdf_fn_uaLintAtoI(x)    
#else    
#define Sdf_mc_atoi			atoi
#endif /* End of Else SDF_LINT */

#define Sdf_mc_strtoul(x)	 strtoul(x,(Sdf_ty_s8bit**)NULL,10) 

#define Sdf_mc_htons		htons
#define Sdf_mc_htonl		htonl
#define Sdf_mc_ntohs		ntohs
#define Sdf_mc_ntohl		ntohl



#ifdef SDF_LINT

#define Sdf_mc_strcasecmp(x,y)	sdf_fn_uaLintStrcasecmp(x,y)
#else /* Else of SDF_LINT */
#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || defined(ICF_PORT_SYMBIAN)
#define Sdf_mc_strcasecmp(x,y)	((((x)!=NULL)&&(y!=NULL))?strcasecmp(x,y):-1)
#endif
#if defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_WINCE)
#define Sdf_mc_strcasecmp(x,y)	((((x)!=NULL)&&(y!=NULL))?_strcmpi(x,y):-1)
#define Sdf_mc_strtokr(x,y,z)	(((x)!=NULL)?(char *)win_self_implemented_strtok_r(x,y,z):NULL)
#endif

#endif /* End of Else SDF_LINT */
#if defined(SDF_VXWORKS)
#define Sdf_mc_strcasecmp(x,y) sdf_fn_strcasecmp_vx(x,y)
#endif

#ifdef ICF_PORT_WINCE
#define Sdf_mc_strcasecmp(x,y)	((((x)!=NULL)&&(y!=NULL))?_stricmp(x,y):-1)
#define Sdf_mc_strtokr(x,y,z)	(((x)!=NULL)?(char *)win_self_implemented_strtok_r(x,y,z):NULL)
#endif

#ifdef SDF_LINT
#define Sdf_mc_strcmp(x,y)		sdf_fn_uaLintStrcmp(x,y)
#define Sdf_mc_strncmp(x,y,n)	sdf_fn_uaLintStrncmp(x,y,n)
#define Sdf_mc_strpbrk(x,y)      sdf_fn_uaLintStrpbrk(x,y)    
#else
#define Sdf_mc_strcmp(x,y)		((((x)!=NULL)&&(y!=NULL))?strcmp(x,y):-1)
#define Sdf_mc_strncmp(x,y,n)	((((x)!=NULL)&&(y!=NULL))?strncmp(x,y,n):-1)
#define Sdf_mc_strpbrk(x,y) ((((x)!=NULL)&&(y!=NULL))?strpbrk(x,y):NULL)
#endif /* End of Else SDF_LINT */

#if defined(SDF_SOLARIS) || defined(SDF_VXWORKS) || defined(SDF_OSE)

#ifdef SDF_LINT
#define Sdf_mc_strtokr(x,y,z)	sdf_fn_uaLintStrtokr(x,y,z)
#else    
#define Sdf_mc_strtokr(x,y,z)	(((x)!=NULL)?(Sdf_ty_s8bit *)strtok_r(x,y,z):NULL)
#endif /* End of Else SDF_LINT */

#endif /* End of SDF_SOLARIS || SDF_VXWORKS || SDF_OSE */


/*	These macros define STRDUP for all components.
	If you want them to be allocated
 	to different pools, redefine these macros */

#ifdef SDF_LINT
#define Sdf_mc_strdupCT(x)					sdf_fn_uaLintStrdup(x,0) 
#define Sdf_mc_strdupSdp(x)					sdf_fn_uaLintStrdup(x,0)
#define Sdf_mc_strdupConfig(x) 				sdf_fn_uaLintStrdup(x,0) 
#define Sdf_mc_strdupAuth(x) 				sdf_fn_uaLintStrdup(x,0) 
#define Sdf_mc_strdupAccessor(x) 			sdf_fn_uaLintStrdup(x,0) 
#define Sdf_mc_strdupRemoteRetransMngr(x) 	sdf_fn_uaLintStrdup(x,0) 
#define Sdf_mc_strdupCallHandler(x) 		sdf_fn_uaLintStrdup(x,0) 
#else    
#define Sdf_mc_strdupCT(x)					(((x)!=NULL)?sip_strdup(x,0):NULL) 
#define Sdf_mc_strdupSdp(x)					(((x)!=NULL)?sip_strdup(x,0):NULL) 
#define Sdf_mc_strdupConfig(x) 				(((x)!=NULL)?sip_strdup(x,0):NULL) 
#define Sdf_mc_strdupAuth(x) 				(((x)!=NULL)?sip_strdup(x,0):NULL) 
#define Sdf_mc_strdupAccessor(x) 			(((x)!=NULL)?sip_strdup(x,0):NULL) 
#define Sdf_mc_strdupRemoteRetransMngr(x) 	(((x)!=NULL)?sip_strdup(x,0):NULL) 
#define Sdf_mc_strdupCallHandler(x) 		(((x)!=NULL)?sip_strdup(x,0):NULL) 
#endif /* End of Else SDF_LINT */

    
#define Sdf_mc_toupper(x)					(((x)!=0)?toupper(x):0)

/* These are memory IDS that will be used for fast_memget */
#define Sdf_mc_configMemId				30
#define	Sdf_mc_callHandlerMemId			31
#define Sdf_mc_authMemId				32
#define Sdf_mc_remoteRetransMngrMemId	33
#define Sdf_mc_accessorMemId			34
#define Sdf_mc_sdpMemId					35

#ifdef SDF_HA
#define Sdf_mc_haMemId					36
#endif

#ifdef SDF_SERVICE
#define	Sdf_mc_serviceMemId				37
#endif

/* INIT and FREE IDs are used by sdf_init.c & sdf_free.c
  since there is no way of guessing in which context
  these free/inits were called, they have a different
  memory id */

#define	Sdf_mc_initMemId				40
#define Sdf_mc_freeMemId				41

/*Application mem Id*/
#define Sdf_mc_appMemId					42

#ifdef SDF_SUBSCRIPTION_LAYER
#define Sdf_mc_subsLayerMemId           43
#endif    
#ifdef SDF_DEP
#define Sdf_mc_depMemId					44
#endif

/* All the requests sent out with hssUA will have this 
 * number in its MAX-FORWARDS header */

/* Wrappers for memory related calls */

#if defined(SDF_SOLARIS) || defined(SDF_LINUX) || defined(SDF_VXWORKS)
#define Sdf_mc_bzero(s,n) (bzero(s,n))
#endif

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#define Sdf_mc_bzero(s,n) (memset(s,0,n))
#endif

#define Sdf_mc_memcpy(s1,s2,n) ((((s1)!=NULL)&&((s2)!=NULL))?memcpy(s1,s2,n):NULL)

#define Sdf_mc_realloc(s1,n) fast_memrealloc(0,s1,n,NULL)


#ifdef SDF_THREAD_SAFE
extern Sdf_ty_mutexIdT dGlbPrintMutex;
extern Sdf_ty_s32bit sdf_fn_uaThreadSafePrintf _ARGS_((\
	const Sdf_ty_s8bit * fmt, ...));
#define Sdf_mc_printf sdf_fn_uaThreadSafePrintf
#else
#define Sdf_mc_printf printf
#endif


#if defined(SDF_WINDOWS) || defined(ICF_PORT_WINDOWS)
typedef 	DWORD WINAPI	Sdf_ty_fptrStartFunc(Sdf_ty_pvoid);
#endif
#define IN6ADDRSZ       16
#define INT16SZ          2

#if defined (SDF_WINDOWS) 
static const Sdf_ty_s8bit *sdf_fn_uaInetNtoP4(const u_char *src, Sdf_ty_s8bit *dst, size_t size);
#ifdef SDF_IPV6
static const Sdf_ty_s8bit *sdf_fn_uaInetNtoP6(const u_char *src, Sdf_ty_s8bit *dst, size_t size);
#endif
#endif
#if defined (SDF_WINDOWS) || defined(SDF_VXWORKS) 
const Sdf_ty_s8bit *sdf_fn_uaInetNtoP(Sdf_ty_s32bit af, const Sdf_ty_void *src, Sdf_ty_s8bit *dst, size_t size);
#endif

/*
 * the definitions below are valid for 32-bit architectures and will have to
 * be adjusted for 16- or 64-bit architectures
 */
#if defined(SDF_WINDOWS) || defined(ICF_PORT_WINDOWS)
typedef unsigned __int8		uint8_t;
typedef unsigned __int16	uint16_t;
typedef unsigned __int32	uint32_t;
typedef unsigned __int64    uint64_t;
typedef __int8		int8_t;
typedef __int16		int16_t;
typedef __int32		int32_t;
typedef __int64		int64_t;
typedef unsigned long	in_addr_t;
#endif
#if defined(SDF_LINUX) || defined(SDF_SOLARIS)
typedef 	Sdf_ty_pvoid 	(*Sdf_ty_fptrStartFunc)(Sdf_ty_pvoid);
#endif



/* 
 * Functions which might need to be ported.
 */

/* ==========================================================================
 * 
 * 				MEMORY ALLOCATION/DE-ALLOCATION APIs
 *
 * ========================================================================== */
/***************************************************************************
 ** FUNCTION:		sdf_memget
 **
 ** DESCRIPTION: 	Same as sip_memget of the core stack - 
 **				 	defined for convenience
 **
 ** PARAMETERS
 **   module_id	(IN) : Module requesting the memory allocation.
 **   noctets	(IN) : Number of octets of memory requested.
 **   pErr	   (OUT) : Error variable filled in case of failure.
 **
 ***************************************************************************/
extern Sdf_ty_pvoid sdf_memget _ARGS_((Sdf_ty_u32bit module_id, \
	Sdf_ty_u32bit noctets, Sdf_st_error *pErr));


/****************************************************************************
 ** FUNCTION:		sdf_memfree
 **
 ** DESCRIPTION: 	Same as sip_memfree of the core stack - 
 **				 	defined for convenience
 ** 
 ** PARAMETERS
 **   module_id (IN) : Module requesting the memory allocation.
 **   p_buf		(IN) : Pointer to memory to be freed.
 **   pErr	   (OUT) : Error variable filled in case of failure.
 **   
 ****************************************************************************/
 extern Sdf_ty_bool  sdf_memfree _ARGS_((Sdf_ty_u32bit module_id, \
	Sdf_ty_pvoid* p_buf , Sdf_st_error *pErr));



/* =========================================================================
 * 
 * 						 APIs USING TIME LIBRARY ROUTINES
 *
 * ========================================================================= */

/****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetCurrentTimeInMicroSecs
 **
 ** DESCRIPTION: 	This function returns the current time in Micro 
 **				 	seconds 
 **
 ** PARAMETERS
 **   pMicroSeconds	(OUT) : Time elapsed till present (in microseconds  
 **							since 1970) is returned in this variable.
 ** 
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaGetCurrentTimeInMicroSecs \
 	_ARGS_((Sdf_ty_u32bit* pMicroSeconds));


/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetCurrentTimeInSecs
 **
 ** DESCRIPTION: 	This function returns the current time in seconds 
 **
 ** PARAMETERS
 **   pSeconds	(OUT) : Time elapsed till present (in seconds  
 **						since 1970) is returned in this variable.
 **
 *****************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaGetCurrentTimeInSecs \
 	_ARGS_((Sdf_ty_u32bit* pSeconds));


/***************************************************************************
 ** FUNCTION: sdf_fn_uaGetTimeString
 **
 ** DESCRIPTION: This function returns the current time as a string
 **
 ***************************************************************************/
 extern void sdf_fn_uaGetTimeString _ARGS_((Sdf_ty_s8bit **ppString));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGenerateGlobalTag
 **
 ** DESCRIPTION: 	This function generates a random Tag which will be used 
 ** 			 	in From & To Tag fields. Memory allocation for the tag
 **				 	happens within the function.
 **
 ** PARAMETERS
 **   pConstTag			(IN) 	: Constant part of tag from User profile. 
 **   ppGeneratedTag 	(OUT) 	: tag is returned in this variable.
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaGenerateGlobalTag \
 	_ARGS_((Sdf_ty_s8bit *pConstTag, Sdf_ty_s8bit** ppGeneratedTag));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGenerateFixedTag
 **
 ** DESCRIPTION: 	This function generates a Fixed part of Tag which will 
 **					be used  in From & To Tag fields. Memory allocation for 
 **					the tag happens within the function.
 **
 ** PARAMETERS
 **   ppGeneratedTag 	(OUT) 	: tag is returned in this variable.
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaGenerateFixedTag \
 	_ARGS_((Sdf_ty_s8bit** ppGeneratedTag));

/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGenerateViaBranch
 **
 ** DESCRIPTION: 	This function generates a random Tag which will be used 
 ** 			 	in From & To Tag fields. Memory allocation for the tag
 **				 	happens within the function.
 **
 ** PARAMETERS
 **   pConstTag			(IN) 	: Constant part of tag from User profile. 
 **   ppGeneratedTag 	(OUT) 	: tag is returned in this variable.
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaGenerateViaBranch _ARGS_(( \
	Sdf_st_transportInfo *pTransportAddr, Sdf_ty_s8bit** ppGeneratedBranch));

/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetDeltaSecondsForRetryAfter
 **
 ** DESCRIPTION: 	This function generates a random value in delta seconds 
 ** 			 	to be used in RetryAfter header
 **
 ** PARAMETERS
 **	  pRespCode		(IN)  :	Response code based on which different algos 
 **							can be used to calculate delta secs.
 **
 **   pDeltaSecs	(OUT) : the seconds to be inserted in the 
 **							Retry-After header is returned in this 
 **							variable.
 **
 **************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaGetDeltaSecondsForRetryAfter \
 	_ARGS_((Sdf_ty_u16bit pRespCode, Sdf_ty_u32bit *pDeltaSecs));



/* =========================================================================
 * 
 * 						THREAD LIBRARY ROUTINES
 *
 * ========================================================================= */

/**************************************************************************
 ** FUNCTION: 		sdf_fn_uaSleep
 **
 ** DESCRIPTION: 	This function pauses the calling thread for the 
 **					duration specified
 **
 ** PARAMETERS
 **	  dSecs			(IN) : 	seconds for which the invoking thread 
 **							should sleep.
 **	  dMicroSecs	(IN) :	microseconds for which the invoking 
 **							thread should sleep.
 **
 *************************************************************************/
 extern void sdf_fn_uaSleep _ARGS_((Sdf_ty_u32bit dSecs , \
 		Sdf_ty_u32bit dMicroSecs));
 
/*********************************************************
** FUNCTION: sdf_fn_uaEndHostEnt
**
** DESCRIPTION: This function frees the system resources allocated 
**				by gethostbyname_r
**
** PARAMETERS:
**			none
**
**********************************************************/
 void sdf_fn_uaEndHostEnt _ARGS_((void));

/*********************************************************
** FUNCTION: sdf_fn_uaDisplayFnTrace
**
** DESCRIPTION: This function displays the trace string 
**				appropriately for different platforms
**
**
**********************************************************/

 void Sdf_fn_uaDisplayFnTrace _ARGS_(( const Sdf_ty_s8bit *pStr, \
                                            Sdf_ty_u32bit dLen, \
                                            Sdf_st_error *pErr));

/*********************************************************
** FUNCTION: sdf_fn_uaDisplayFnDebug
**
** DESCRIPTION: This function displays the debug string 
**				appropriately for different platforms
**
**
**********************************************************/

 void sdf_fn_uaDisplayFnDebug _ARGS_((Sdf_ty_s8bit *str));

/*********************************************************
** FUNCTION: sdf_fn_uaDisplayFnError
**
** DESCRIPTION: This function displays the error string 
**				appropriately for different platforms
**
**
**********************************************************/

 void sdf_fn_uaDisplayFnError _ARGS_((Sdf_ty_s8bit *str));

#ifdef SDF_THREAD_SAFE
/*********************************************************
** FUNCTION: sdf_fn_uaCreateRemoteRetransThread
**
** DESCRIPTION: This function creates the remote retrans thread 
**				appropriately for different platforms
**
**
**********************************************************/

 Sdf_ty_retVal sdf_fn_uaCreateRemoteRetransThread _ARGS_(\
 (Sdf_ty_threadT *pRetransThread, Sdf_ty_fptrStartFunc pFuncPtr, \
 Sdf_st_error   *pErr));

/*********************************************************
** FUNCTION: sdf_fn_uaDeleteRemoteRetransThread
**
** DESCRIPTION: This function cancels the remote retrans thread 
**				appropriately for different platforms
**
**
**********************************************************/

 Sdf_ty_retVal sdf_fn_uaDeleteRemoteRetransThread _ARGS_(\
 (Sdf_ty_threadT *pRetransThread, Sdf_ty_pvoid   ptr));
/*****************************************************************
** FUNCTION:sdf_fn_CreateThreadKey
**
**
** DESCRIPTION: It Creates the Thread Key for Gloable to all thread
**     
*******************************************************************/
Sdf_ty_retVal sdf_fn_CreateThreadKey (Sdf_ty_pvoid pDestFunc);
#if defined(SDF_WINDOWS) || defined(SDF_WINCE)
#define pthread_key_create(a,b) {*a= TlsAlloc();}; 
#endif 
#endif

/*************************************************************************
 ** FUNCTION: 		sdf_fn_uaCreateThread
 **
 ** DESCRIPTION: 	This function creates a thread and passes the
 **					id back to the invocation.
 **
 ** PARAMETERS
 **   pThread		(IN/OUT) : 	Thread id for the new thread created is 
 **								returned in this variable.
 **   pThreadAttr	(IN)	 :	Attributes for creation of the new thread.
 **	  fptrStart		(IN)	 :	Function from where the new thread should
 **								start execution
 **	  pArg			(IN)	 :	Argument to be passed to the thread.
 **
 *************************************************************************/
#if defined (SDF_THREAD_SAFE) || defined (SDF_USE_INTERNALTHREADS) 
 extern Sdf_ty_retVal sdf_fn_uaCreateThread _ARGS_((Sdf_ty_threadT *pThread, \
		Sdf_ty_pvoid pThreadAttr, Sdf_ty_fptrStartFunc fptrStart, \
		Sdf_ty_pvoid pArg));


/*************************************************************************
 ** FUNCTION: 		sdf_fn_uaTerminateThread
 **
 ** DESCRIPTION: 	This function terminates a thread 
 **
 ** PARAMETERS
 **   dThreadId	(IN) :	Thread id of the thread to be killed.
 **
 *************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaTerminateThread _ARGS_((Sdf_ty_threadT dThread));


/*************************************************************************
 ** FUNCTION: sdf_fn_uaCancelThread
 **
 ** DESCRIPTION: This function cancels the thread
 **
 ** PARAMETERS
 **	  dThread 	(IN) :	Thread id of the thread to be cancelled.
 ** 
 *************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaCancelThread _ARGS_((Sdf_ty_threadT dThread));


/***************************************************************************
 ** FUNCTION: sdf_fn_uaJoinThread
 **
 ** DESCRIPTION: This function suspends execution of the calling thread
 **					until the target thread completes
 **
 ** PARAMETERS
 **   dThread	(IN) : 	Thread if of the thread which has to be joined 
 **						with the current thread.
 **	  ppVal		(IN) : 	The value passed by the terminating thread is 
 **						placed here.
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaJoinThread _ARGS_((Sdf_ty_threadT dThread, \
		Sdf_ty_pvoid *ppVal));


/***************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetThreadCancelState
 **
 ** DESCRIPTION: This function can be used to enable/disable cancellation
 **				 of a thread
 **
 **	state[IN] 	: The calling thread's cancellation state to be set.
 ** oldstate[IN]: Previous cancellation state of the thread.
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaSetThreadCancelState \
 	_ARGS_((Sdf_ty_s32bit state, Sdf_ty_s32bit *oldState));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaTestCancel
 **
 ** DESCRIPTION: This function checks if thread was Cancelled
 **	
 ** pVoid		:Future use for other ports.
 **
 ******************************************************************************/ 
extern Sdf_ty_retVal sdf_fn_uaTestCancel _ARGS_((Sdf_ty_pvoid pVoid));

#if !defined(SDF_VXWORKS) && !defined(SDF_WINDOWS) && !defined(SDF_WINCE)

/***************************************************************************
 ** FUNCTION: sdf_fn_uaThreadAttrInit
 **
 ** DESCRIPTION: This function initializes a thread attribute object
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaThreadAttrInit _ARGS_((Sdf_ty_threadAttrT *pAttr));


/***************************************************************************
 ** FUNCTION: sdf_fn_uaThreadAttrDestroy
 **
 ** DESCRIPTION: This function destroys a thread attribute object
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaThreadAttrDestroy \
 	_ARGS_((Sdf_ty_threadAttrT *pAttr));

#endif /* SDF_VXWORKS || SDF_WINDOWS */
#endif /* End of SDF_THREAD_THREAD || SDF_USE_INTERNALTHREADS */

#ifdef SDF_THREAD_SAFE

/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetCallingThreadId
 **
 ** DESCRIPTION: 	This function returns the threadId of the calling
 **					thread.
 **
 ***************************************************************************/
extern Sdf_ty_threadId sdf_fn_uaGetCallingThreadId _ARGS_((Sdf_ty_pvoid pData));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaInitMutex
 **
 ** DESCRIPTION: 	This function initializes the mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaInitMutex _ARGS_((Sdf_ty_mutexIdT *pMutex, \
	Sdf_ty_bool dUseThreadAssociation));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaDestroyMutex
 **
 ** DESCRIPTION: 	This function destroys the mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaDestroyMutex _ARGS_((Sdf_ty_mutexIdT *pMutex));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaLockMutex
 **
 ** DESCRIPTION: 	This function locks a mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaLockMutex _ARGS_((Sdf_ty_threadId dThreadId, \
	Sdf_ty_mutexIdT *pMutexId, Sdf_ty_u32bit flags));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaTryLock
 **
 ** DESCRIPTION: 	This function is identical to the sdf_fn_uaLockMutex 
 **					except that if the mutex object is already locked by 
 **					any thread, this call returns immediately.
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaTryLock _ARGS_((Sdf_ty_mutexIdT *pMutexId, \
	Sdf_ty_threadId dThreadId));


/***************************************************************************
 ** FUNCTION: sdf_fn_uaUnlockMutex
 **
 ** DESCRIPTION: This function unlocks a mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaUnlockMutex _ARGS_((Sdf_ty_threadId dThreadId, \
	Sdf_ty_mutexIdT *pMutexId));
/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaInitSyncObject
 **
 ** DESCRIPTION: 	This function initializes the mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaInitSyncObject _ARGS_((Sdf_ty_syncIdT *pMutex, \
	Sdf_ty_bool dUseThreadAssociation));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaDestroySyncObject
 **
 ** DESCRIPTION: 	This function destroys the mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaDestroySyncObject _ARGS_((Sdf_ty_syncIdT *pMutex));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaLockSyncObject
 **
 ** DESCRIPTION: 	This function locks a mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaLockSyncObject _ARGS_((Sdf_ty_threadId dThreadId, \
	Sdf_ty_syncIdT *pMutexId, Sdf_ty_u32bit flags));


/***************************************************************************
 ** FUNCTION: sdf_fn_uaUnlockSyncObject
 **
 ** DESCRIPTION: This function unlocks a mutex
 **
 ***************************************************************************/
 extern void sdf_fn_uaUnlockSyncObject _ARGS_((Sdf_ty_threadId dThreadId, \
	Sdf_ty_syncIdT *pMutexId));

#if defined(SDF_LINUX) || defined(SDF_SOLARIS) || defined(SDF_WINDOWS) ||defined(SDF_WINCE)
#ifdef SDF_LOOKUP
/***************************************************************************
 ** FUNCTION: sdf_fn_uaGetSpecificThreadKey
 **
 ** DESCRIPTION: This function get ThreadSpecfic key.
 **
 ***************************************************************************/

 extern Sdf_ty_pvoid sdf_fn_uaGetSpecificThreadKey(Sdf_st_ThreadKey dLocThreadKey);


 /***************************************************************************
 ** FUNCTION: sdf_fn_uaSetSpecificThreadKey
 **
 ** DESCRIPTION: This function set ThreadSpecfic key.
 **
 ***************************************************************************/

 extern Sdf_ty_retVal sdf_fn_uaSetSpecificThreadKey _ARGS_((\
				Sdf_st_ThreadKey dLocThreadKey, Sdf_ty_pvoid *ppVoid ));
#if defined(SDF_WINDOWS) || defined(SDF_WINCE)
#define pthread_getspecific(ts_key) TlsGetValue(ts_key)
#define pthread_setspecific(ts_key, value) TlsSetValue(ts_key, (void *)value)
#endif 


#endif 
#endif
#endif



/* =========================================================================
 * 
 * 						STRING LIBRARY ROUTINES
 *
 * ========================================================================= */

/***************************************************************************
 ** FUNCTION: sdf_fn_uaSprintf
 **
 ** DESCRIPTION: This function implements sprintf
 **
 ***************************************************************************/
 extern Sdf_ty_s32bit sdf_fn_uaSprintf _ARGS_((Sdf_ty_s8bit *s , \
	const Sdf_ty_s8bit * fmt, ...));

/* Fix for CSR 1-8534364: SPR 20388 */
#ifndef ICF_UATK_PORT
/***************************************************************************
 ** FUNCTION: sdf_fn_uaSnprintf
 **
 ** DESCRIPTION: This function implements sprintf
 **
 ***************************************************************************/
 extern Sdf_ty_s32bit sdf_fn_uaSnprintf _ARGS_((Sdf_ty_s8bit *s , \
	Sdf_ty_u32bit dSize,const Sdf_ty_s8bit * fmt, ...));
#else
#define sdf_fn_uaSnprintf icf_port_snprintf
#endif

/* =========================================================================
 * 
 * 						NETWORK LIBRARY ROUTINES
 *
 * ========================================================================= */

 /******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetHostName
 **
 ** DESCRIPTION: This function gets the machines local host name by calling 
 **				 gethostname().
 **	PARAMETERS:
 **		ppHost(OUT)	:Local Machines Host name
 **		pError(OUT) :Error structure in case the function fails
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetHostName _ARGS_((Sdf_ty_s8bit **ppHost,\
					Sdf_st_error *pError));

#if defined(ICF_UATK_PORT) && (!defined(SDF_LINUX) || !defined(ICF_PORT_LINUX))
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetHostIp
 **
 ** DESCRIPTION: This funtion resolves the provided hostname into an
 **				 IP address and returns the first IP address resolved 
 **				 as a string.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetHostIp _ARGS_((Sdf_ty_s8bit* pHost,\
					Sdf_ty_s8bit** ppDest,
					Sdf_st_error *pError));
#elif !defined(ICF_UATK_PORT)
Sdf_ty_retVal sdf_fn_uaGetHostIp _ARGS_((Sdf_ty_s8bit* pHost,\
					Sdf_ty_s8bit** ppDest,
					Sdf_st_error *pError));
#endif
/***********************************************************************
 ** FUNCTION:  sdf_fn_uaIsConnectedTcpSocket
 **
 ** DESCRIPTION: This function checks whether the socket is connected to the 
 **				other side or not
 **
 ** PARAMETERS:
 **		dSockfd(IN) 	: The Sockfd which needs to be checked
 **		pError(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/

extern Sdf_ty_retVal sdf_fn_uaIsConnectedTcpSocket _ARGS_((Sdf_ty_s32bit \
	dSockfd, Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaListenSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to listen
 **				 on a socket for TCP connections.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaListenSocket \
	_ARGS_((Sdf_ty_socketId dSocket, Sdf_ty_s32bit dMaxPendingConnections));


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCloseSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to close
 **				 a network socket descriptor.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaCloseSocket _ARGS_((Sdf_st_socket *pSocket));


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSocketCreate
 **
 ** DESCRIPTION: The function is called internally by the toolkit to create
 **				 a new network socket descriptor.
 **
 *****************************************************************************/
extern Sdf_ty_socketId sdf_fn_uaSocketCreate _ARGS_((Sdf_ty_s32bit dDomain, \
		Sdf_ty_s32bit dType, Sdf_ty_s32bit dProtocol));


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaConnectSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to open
 **				 a new network connection.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaConnectSocket _ARGS_((Sdf_st_socket *pSocket, \
		Sdf_ty_pvoid  pName, Sdf_ty_s32bit dNameLength, \
		Sdf_ty_pvoid  pExtraArgs));


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaBindSocket
 **
 ** DESCRIPTION: The function is called internally by the toolkit to bind
 **				 a socket to a specified port.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaBindSocket _ARGS_((Sdf_ty_socketId dSocket,
		Sdf_ty_pvoid  pName, Sdf_ty_s32bit dNameLength));


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetSockOpt
 **
 ** DESCRIPTION: The function is called internally by the toolkit to set
 **				 a socket option.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaSetSockOpt _ARGS_((Sdf_ty_socketId dSocket,
	Sdf_ty_s32bit  dLevel,	Sdf_ty_s32bit  dOptName, \
	Sdf_ty_pvoid   pOptVal,	Sdf_ty_s32bit dOptLen));


/***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGenerateRand
 **
 ** DESCRIPTION: 	This function generates a random value 
 **
 ***************************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaGenerateRand _ARGS_((Sdf_ty_u32bit *pRand));

#ifdef SDF_SESSION_TIMER
 /***************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetReinviteTimeoutTime
 **
 ** DESCRIPTION: 	This function gives the re-INVITE timeout time for 
 ** 				Sending BYE after re-INVITE timeout
 **
 ***************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaGetReinviteTimeoutTime _ARGS_((\
	Sdf_ty_u32bit *pDuartion,Sdf_st_error *pErr));


 /***************************************************************************
 ** FUNCTION: 		sdf_fn_uaModifySessionInterval
 **
 ** DESCRIPTION: 	This function Modifies the Session Interval value 
 **
 ***************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaModifySessionInterval _ARGS_((\
	Sdf_ty_u32bit *pDuartion,Sdf_ty_refresher dRefesher,Sdf_st_error *pErr));

#endif /* End of SDF_SESSION_TIMER */




extern Sdf_ty_retVal sdf_fn_uaGetNumberOfHopsForMaxForward _ARGS_((\
	Sdf_st_callObject *pCallObj,Sdf_ty_u32bit 	*dHops, Sdf_st_error *pError));


#ifdef SDF_SESSION_TIMER
Sdf_ty_s32bit sdf_fn_uaGetGlbMinSe(void);

Sdf_ty_bool sdf_fn_uaGetGlbIsSupportedTimer(void);

Sdf_ty_retVal sdf_fn_uaSetGlbMinSe(Sdf_ty_s32bit dMinSe, Sdf_st_error *pErr);

void sdf_fn_uaSetGlbIsSupportedTimer(Sdf_ty_bool dIsSuppTimer);
#endif

#ifdef SDF_MEMCAL
/* Use below portion of code to enable the memory calculations in UATK */
typedef struct SipMemInfo 
{
	SIP_U32bit  buf_size;
	SIP_U32bit	module_id;
} SipMemInfo;
	
typedef struct SipMemStat
{
	mutex_id_t	lock;
	SIP_U32bit	cum_memget;
	SIP_U32bit	cum_memfree;
	SIP_U32bit	max_mem_usage;
} SipMemStat;

extern SipMemStat glbSipMemStat;
void sip_memStatInit _ARGS_((void));
SipMemStat glbSipMemStat;
#endif /* End of SDF_MEMCAL*/
/*
#ifdef ICF_PORT_WINDOWS*/

/* Change for vxWork Compilation */
#if defined(SDF_WINDOWS) || defined(SDF_WINCE)
#define Sdf_mc_strncasecmp(x,y,n) ((((x)!=NULL)&&(y!=NULL))?_strnicmp(x,y,n):-1)
#elif defined(SDF_VXWORKS)
#define Sdf_mc_strncasecmp(x,y,n) ((((x)!=NULL)&&(y!=NULL))?sip_strncasecmp(x,y,n):-1)
#else
#define Sdf_mc_strncasecmp(x,y,n) ((((x)!=NULL)&&(y!=NULL))?strncasecmp(x,y,n):-1)
#endif

#if defined(SDF_WINDOWS) || defined(SDF_LINUX) || defined(SDF_WINCE)
/*Function for Sdf_mc_strtokr*/
extern Sdf_ty_s8bit* Sdf_mc_strtokr _ARGS_((Sdf_ty_s8bit *x,Sdf_ty_s8bit *y,Sdf_ty_s8bit **z));
extern Sdf_ty_s8bit *Sdf_mc_strtok(Sdf_ty_s8bit *x, Sdf_ty_s8bit *y);
#endif

#ifdef SDF_TLS
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaInitSslData
 **
 ** DESCRIPTION: The function will do the SSL layer initializations.
 **              right now this function will call Wrapper_openSSL APIs which
 **              internally used OpenSSL libraries for TLS support.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaInitSslData _ARGS_((\
			Sdf_ty_s8bit *pEGDRandFile));

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFreeSslData
 **
 ** DESCRIPTION: This function will free the SSL data. Currently, this pData
 **              will be nothing but SSL_CTX.
 **
 *****************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaFreeSslData _ARGS_((Sdf_ty_pvoid *pData));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFileOpen
 **
 ** DESCRIPTION: This function opens the file whose pathname is the string  
 **              pointed to by pFilename, and associates a stream with it.
 **
 ******************************************************************************/
extern Sdf_ty_file* sdf_fn_uaFileOpen _ARGS_((\
        Sdf_ty_s8bit   *pFilename, Sdf_ty_s8bit   *pMode));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFileClose
 **
 ** DESCRIPTION: This function closes the file whose pathname is the string  
 **              pointed to by pFilename.
 **
 ******************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaFileClose _ARGS_((Sdf_ty_file *pFilename));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormSslData
 **
 ** DESCRIPTION: This function will form the SSL data that will be stored 
 **              as pData in the pSslData of InitData. In the current 
 **              implementation SSL_CTX structure will be formed and stored 
 **              as pData in the pSslData of InitData.
 **
******************************************************************************/
extern Sdf_ty_retVal sdf_fn_uaFormSslData _ARGS_((\
	 Sdf_ty_pvoid *pSslParams, Sdf_st_sslData *pSslData, Sdf_st_error *pErr));


#endif /* SDF_TLS */

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitFdSetInfo
 **
 ** DESCRIPTION: This function is used to initialise the 
 **				 Socket Information structure which can be used to store
 **				 socket fd's and max fd in use. This structure is mutex
 **				 protected.
 **
 ******************************************************************************/

Sdf_ty_retVal	sdf_fn_uaInitFdSetInfo (Sdf_st_fdSetInfo *pSockInfo);

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaSetSocketFd
 **
 ** DESCRIPTION: This function is used to Set the new fd into the fdset 
 **				 maintained by UATK and also update the max value of fd used.
 **				 used by application to form an fd_set for network handling.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaSetSocketFd (Sdf_st_fdSetInfo *pSockInfo, \
	Sdf_ty_socketId dSockfd);

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaGetFdSetInfo
 **
 ** DESCRIPTION: Application can use this function to copy the information
 **				 about the fd_set and max fd value maintained at UATK.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetFdSetInfo (Sdf_st_fdSetInfo *pDstSockInfo, \
	Sdf_st_fdSetInfo *pSrcSocketInfo);

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaClearSocketFd
 **
 ** DESCRIPTION: This function removes the specified socket fd from the
 **				 Maintained socket fd set.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaClearSocketFd (Sdf_st_fdSetInfo *pSockInfo, \
	Sdf_ty_socketId dSockfd);

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaClearFdSetInfo
 **
 ** DESCRIPTION: This function is used to Clear the socket info resources.
 **
 ******************************************************************************/

Sdf_ty_retVal	sdf_fn_uaClearFdSetInfo _ARGS_((Sdf_st_fdSetInfo *pSockInfo));


/* this declaration is put by vidyut */
#if defined (ICF_PORT_WINDOWS) || defined (ICF_PORT_SYMBIAN)
char * win_self_implemented_strtok_r(char *s, const char *delim, char **last);
#endif

#ifdef ICF_UATK_PORT
/* IPTK CML Start */
/* Codenomicon fixes: CSR 1-7132323: SPR 19484 */
/******************************************************************************
 * ** FUNCTION: sdf_ivk_uaIPTKCheckMessageLength
 * **
 * ** DESCRIPTION: This API checks if the length of any of the header 
 * **               (present in SIP message) is more than the specified
 * length.
 * **
 * *******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaIPTKCheckMessageLength(Sdf_ty_s8bit *pStr, Sdf_ty_u32bit
                len, Sdf_ty_u32bit len2, Sdf_ty_u32bit *pOption,
                        Sdf_ty_IPTKmessageLengthValidity *pResult, Sdf_st_error *pError);
/* IPTK CML End */
#endif 
#ifdef SDF_DEP
/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepMgrPostStateChange
 **
 ** DESCRIPTION: This is an event manager function used in case of DEP.This function
 **							 is used to post any changes on to the event queue. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDepMgrPostStateChange(Sdf_st_DialogUpdate *updateElement,Sdf_st_error *pErr);

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepFetchUpdateElement
 **
 ** DESCRIPTION: This is an event manager function used in case of DEP.This function
 **							 is used to fetch an element from the event queue. 
 **
 ******************************************************************************/
Sdf_st_DialogUpdate* sdf_fn_uaDepFetchUpdateElement(void);

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepInitEventQueue
 **
 ** DESCRIPTION: This is an event manager function used to initialize the state
 **              change event queue. 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDepInitEventQueue(Sdf_st_error *pErr);

/******************************************************************************
 ** FUNCTION:	 sdf_fn_uaDepFreeEventQueue
 **
 ** DESCRIPTION: This is an event manager function used to free the state
 **              change event queue. 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDepFreeEventQueue(Sdf_st_error *pErr);

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
#endif  /* __SDF_PORTLAYER_H_ */

