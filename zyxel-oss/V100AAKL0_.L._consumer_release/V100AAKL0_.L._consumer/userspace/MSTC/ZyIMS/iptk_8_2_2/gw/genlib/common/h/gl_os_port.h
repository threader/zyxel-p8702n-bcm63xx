/*******************************************************************************
*
* FUNCTION : Generic Libraries OS porting abstraction.
*
********************************************************************************
*
* FILENAME : gl_os_port.h
*
* DESCRIPTION : Contains GL OS porting abstraction data types and
*               porting abstraction function prototypes.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* 19/1/2001  Sachin Mittal                   Initial  
* 07/03/2001 Menuka Jain                     Changes to add gl_mutex_init.
*
* 18/08/2001 Prashant Jain   SPR 2276        Compiling with CC on Solaris.
*
* 04/09/2001 Prashant Jain   SPR-2560        Adding OSE Operating System Support.
* 04/10/2001 Ravi Jindal     SPR-2751        Remove warnings with gcc,cc,CC on 
*                                            Solaris.
* 18/10/2001 Ravi Jindal     SPR-2788        Packaging of Genlib-Code.
* 19/10/2001 Ravi Jindal     SPR-2802        Adding Pre-ANSI-C features.
* 29/10/2001 Ravi Jindal     SPR-2798        Adding the semaphore operation
*                                            releated functions. 
* 17/11/2001 Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                            cosmmetics of file.
*
* 04/12/2001 Ravi Jindal    SPR-3339         Changed the interface of
*            Prashant Jain                   gl_try_wait_on_semaphore
*                                            to tackle some race conditions
*                                            in case this call fails.
*
* 06/12/2001 Prashant Jain  SPR-3363         Removed "pthread.h" to remove
*                                            compile-time error with "cc". 
*
* 26/01/2002 Prashant Jain  SPR-3391         Porting over OSE for Soft-kernel.
*
* 16/09/2002 Ravi Jindal    SPR-4853         To change winsock.h to winsock2.h
*                                            for WINNT.
* 12/07/2004 Navneet Gupta  SPR 6946         QNX port
*
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

/* 
 * Check to see if this file has been included already.
 */

#ifndef _GL_OS_PORT_H_
#define _GL_OS_PORT_H_

#include "gl_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef GL_VXWORKS

#include "vxWorks.h"
#include "stdarg.h"
#include "arpa/inet.h"
#include "msgQLib.h"
#include "tickLib.h"
#include "time.h"
#include "sys/times.h"
#include "string.h"
#include "signal.h"
#include "ramDrv.h"
#include "dosFsLib.h"
#include "ftpLib.h"
#include "ioLib.h"

#ifdef GL_IPV6
#include "netinet6/in6.h"
#include "netinet6/ip6.h"
#endif

#include "stdlib.h"
#include "stdio.h"
#include "memLib.h"
#include "timers.h"
#include "sockLib.h"
#include "inetLib.h"
#include "in.h"
#include "ctype.h"
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


#ifdef GL_MT_ENABLED

#include "semLib.h"

#define GL_MUTEX_ID         SEM_ID
#define GL_SEMAPHORE_ID     sem_t
#define gl_thread_id_t      int
#define gl_thread_handle_t  int
#define gl_thread_return_t  int

typedef int (* gl_fn_ptr_t) (void);

#endif  /* End of GL_MT_ENABLED */
#endif

/************* End for GL_VXWORKS ******/


#ifdef GL_WINNT

#include <winsock2.h>
#include <time.h>  
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "sys/timeb.h"

#ifdef GL_MT_ENABLED

#include "windows.h"
#include "winbase.h"
#include "sys/types.h"

#ifdef GL_SHARED_SHM

#include <windowsx.h>
#include <tchar.h>

#endif

#define GL_MUTEX_ID         CRITICAL_SECTION
#define GL_MUTEX_ID         CRITICAL_SECTION
#define GL_SEMAPHORE_ID     HANDLE

#define gl_thread_id_t      DWORD
#define gl_thread_handle_t  HANDLE
#define gl_thread_return_t  DWORD

typedef    DWORD(* gl_fn_ptr_t)(void*);

#endif  /* End of GL_MT_ENABLED */
#endif

/************* End for GL_WINNT ******/


#ifdef GL_SOLARIS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/filio.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>

#ifdef GL_MT_ENABLED

#include <thread.h>
#include <synch.h>

#define GL_MUTEX_ID         mutex_t
#define GL_SEMAPHORE_ID     sem_t
#define gl_thread_id_t      thread_t
#define gl_thread_handle_t  gw_U32bit
#define gl_thread_return_t  void*

typedef    void* (* gl_fn_ptr_t) (void*);

#endif /* End of GL_MT_ENABLED */

/*
 * The prototype of this system call is not found 
 * in any of the system-defined header file,hence 
 * has been declared explicitly here to remove
 * warnings and error in case "CC" compiler is used.
 *
 * The declearation for gethostname is given in 
 *  /usr/include/unistd.h 
 *  but this decleration is available only on varuna 
 *  and on some other servers. So it is explicitly decleared
 *  here to use in general on other servers as (vajra)
 */
   extern int gethostname(char *name, int namelen);

#endif
   
/************* End for GL_SOLARIS ******/

   
#ifdef GL_LINUX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <semaphore.h>
   
#ifdef GL_MT_ENABLED

#include <pthread.h>

#define GL_MUTEX_ID         pthread_mutex_t
#define GL_SEMAPHORE_ID     sem_t
#define gl_thread_id_t      pthread_t
#define gl_thread_handle_t  gw_U32bit
#define gl_thread_return_t  void*

typedef    void* (* gl_fn_ptr_t) (void*);

#endif /* End of GL_MT_ENABLED */
#endif

/************* End for GL_LINUX ******/   
 
#ifdef GL_QNX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h>
#include <semaphore.h>
   
#ifdef GL_MT_ENABLED

#include <pthread.h>

#define GL_MUTEX_ID         pthread_mutex_t
#define GL_SEMAPHORE_ID     sem_t
#define gl_thread_id_t      pthread_t
#define gl_thread_handle_t  gw_U32bit
#define gl_thread_return_t  void*

typedef    void* (* gl_fn_ptr_t) (void*);

#endif /* End of GL_MT_ENABLED */
#endif

/************* End for GL_QNX ******/   

#ifdef GL_OSE

#include <ctype.h>
#include <string.h>
#include <ose.h>
#include <ose_err.h>
#include <inet.h>
#include <inet.sig>
#include <netdb.h>
#include <rtc.h>
#include <time.h>
#include <ose_mmu.h>
#include <ose_rem.h> 
#include <osetypes.h>
#include <stdlib.h>
#include <malloc.h>

#ifdef GL_MT_ENABLED

#include <pthread.h>

#define GL_MUTEX_ID         SEMAPHORE
#define GL_SEMAPHORE_ID     SEMAPHORE

#define gl_thread_id_t      pthread_t
#define gl_thread_handle_t  gw_U32bit
#define gl_thread_return_t  void*

typedef    void*   (* gl_fn_ptr_t) (void*);

#endif /* End of GL_MT_ENABLED */
/*
 * The prototype of this system call is not found 
 * in any of the system-defined header file,hence 
 * has been declared explicitly here to remove
 * warnings.
 */
extern int gethostname(char *name, int namelen);

#endif /* GL_OSE */



/*
 * These decleration are kept in gl_os_port.h as they are required 
 * by both os and db managers.
 */
#ifdef GL_MT_ENABLED

/*#define GL_MUTEX_CREATE(mutex_ptr,p_ecode, x)    gl_create_mutex((mutex_ptr), p_ecode)*/
#define GL_MUTEX_LOCK(mutex_ptr, p_ecode, x)    gl_lock_mutex((mutex_ptr), GL_NULL, p_ecode)
#define GL_MUTEX_UNLOCK(mutex_ptr, p_ecode, x)  gl_unlock_mutex(GL_NULL, (mutex_ptr), p_ecode)
#define GL_MUTEX_DELETE(mutex_ptr, p_ecode, x)  gl_delete_mutex((mutex_ptr) , p_ecode)

#else

/*#define GL_MUTEX_CREATE(mutex_ptr,p_ecode, x)*/
#define GL_MUTEX_LOCK(mutex_ptr, p_ecode, x)    
#define GL_MUTEX_UNLOCK(mutex_ptr, p_ecode, x)  
#define GL_MUTEX_DELETE(mutex_ptr, p_ecode, x)

#endif

#ifdef GL_TOTAL_MT_SAFE

/*#define GL_MT_CREATE(mutex_ptr,p_ecode, x)    gl_create_mutex((mutex_ptr), p_ecode)*/
#define GL_MT_LOCK(mutex_ptr, p_ecode, x)    gl_lock_mutex((mutex_ptr), GL_NULL, p_ecode)
#define GL_MT_UNLOCK(mutex_ptr, p_ecode, x)  gl_unlock_mutex(GL_NULL, (mutex_ptr), p_ecode)
#define GL_MT_DELETE(mutex_ptr, p_ecode, x)  gl_delete_mutex((mutex_ptr) , p_ecode)

#else

/*#define GL_MT_CREATE(mutex_ptr,p_ecode, x)*/
#define GL_MT_LOCK(mutex_ptr, p_ecode, x)    
#define GL_MT_UNLOCK(mutex_ptr, p_ecode, x)  
#define GL_MT_DELETE(mutex_ptr, p_ecode, x)

#endif

#ifdef __cplusplus
}
#endif

#endif  /* 
         * End of _GL_OS_PORT_H_ 
         */

