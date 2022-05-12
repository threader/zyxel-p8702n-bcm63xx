

#ifndef __SDF_BASICTYPES_H_
#define __SDF_BASICTYPES_H_

#include "microsip_common.h"

#ifdef ICF_PORT_SYMBIAN 
#include<sys/types.h>
#endif

#ifdef ICF_PORT_WINDOWS
#include <windows.h>
#include <winsock2.h>
#endif

typedef enum
{
	/* ------------------------------------------------------------------------
	 DESCRIPTION:	Enum defining the Boolean values.
	 
	 Sdf_co_true	- TRUE boolean.
	 Sdf_co_false - FALSE boolean.
	-----------------------------------------------------------------------*/
	Sdf_co_false  = (SipFail),
	Sdf_co_true	= (SipSuccess)
} Sdf_ty_bool;

typedef enum
{
	/* ------------------------------------------------------------------
	 DESCRIPTION:	Enum defining the return values.
	 
	 Sdf_co_success	-	Return value indicating success.
	 Sdf_co_fail		-	Return value indicating failure.
	 -----------------------------------------------------------------*/

	Sdf_co_fail = (SipFail),
	Sdf_co_success = (SipSuccess)
} Sdf_ty_retVal;


typedef 	SIP_U8bit					Sdf_ty_u8bit;
typedef 	SIP_U16bit					Sdf_ty_u16bit;
typedef	 	SIP_U32bit					Sdf_ty_u32bit;

typedef 	SIP_S8bit					Sdf_ty_s8bit;
typedef		SIP_S16bit					Sdf_ty_s16bit;
typedef		SIP_S32bit					Sdf_ty_s32bit;

typedef     void                        Sdf_ty_void; 
typedef		SIP_Pvoid					Sdf_ty_pvoid;
typedef		FILE                        Sdf_ty_file;
typedef		fd_set						Sdf_ty_fdSet;

/* Basic data types that are ported differently for different
 * operating systems, e.g. socket IDs, thread IDs, Mutex objects
 * etc
 */

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#ifdef SDF_LINT
typedef     Sdf_ty_s32bit		Sdf_ty_socketId;
#define     Sdf_ty_fnreturn             DWORD WINAPI    
#else
typedef     	SOCKET			Sdf_ty_socketId;
#define     Sdf_ty_fnreturn             DWORD WINAPI    
#endif
#else
typedef     Sdf_ty_s32bit		Sdf_ty_socketId;
typedef     Sdf_ty_pvoid                Sdf_ty_fnreturn;             
#endif

typedef		struct timeval			Sdf_st_timeVal;

#if defined(SDF_USE_INTERNALTHREADS) || defined(SDF_THREAD_SAFE)
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
typedef		HANDLE						Sdf_ty_threadT;         
typedef		DWORD						Sdf_ty_threadId;        
#else                                                           
typedef		thread_id_t					Sdf_ty_threadT;         
typedef		thread_id_t					Sdf_ty_threadId;        
#endif

#if defined (SDF_LINUX) || defined (SDF_SOLARIS) || defined (SDF_OSE)
typedef		pthread_attr_t				Sdf_ty_threadAttrT;
typedef         struct in_addr                    Sdf_st_inAddr;
#endif

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)

typedef struct 
{
	SECURITY_ATTRIBUTES			*pSecurityAttributes;
	DWORD					dStackSize;
	DWORD					dCreationFlags;
	DWORD					dThreadId;
} Sdf_ty_threadAttrT;
#endif
#endif

#ifdef SDF_THREAD_SAFE
typedef struct
{
	/* 
	 * Mutual exclusion variable 
	 */
	synch_id_t		dMutex;
	/* 
	 * Thread association indicating the thread 
	 * that has currently locked the mutex
	 */
	Sdf_ty_threadId		dThreadId;

	/* Boolean indicating whether the above 
	 * thread association is to be used in 
	 * this mutex 
	 */
	Sdf_ty_bool 		dUseThreadAssociation;

} Sdf_ty_syncIdT;
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)

typedef struct
{
	/* 
	 * Mutual exclusion variable 
	 */
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
	HANDLE				dMutex;
#else
	synch_id_t			dMutex;
#endif

	/* 
	 * Thread association indicating the thread 
	 * that has currently locked the mutex
	 */
	Sdf_ty_threadId			dThreadId;
	/* Boolean indicating whether the above 
	 * thread association is to be used in 
	 * this mutex 
	 */
	Sdf_ty_bool 		dUseThreadAssociation;
    /*
	 * dLocalCount is use to maintain
	 * No of Locks in a recursion.This
	 * is under SDF_WINDOWS Flag becausee
	 * Recursive locking is possible in windows only
	 */
#if defined (SDF_WINDOWS) || defined(SDF_WINCE)
	Sdf_ty_u8bit	dLockCount;
#endif

} Sdf_ty_mutexIdT;
#else
typedef Sdf_ty_syncIdT		Sdf_ty_mutexIdT;
#endif
#endif

	#define Sdf_co_null 					NULL
#ifdef SDF_THREAD_SAFE
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
typedef struct 
{
	LPSECURITY_ATTRIBUTES  pSecurity;
	/* Security Attribute must be null */
	DWORD dStacksize;
	DWORD dwCreationFlags;
	DWORD dThreadId;
}Sdf_ty_threadParameter;
#endif
#endif
#define Sdf_co_InvalidSocketFd	-1


#endif
