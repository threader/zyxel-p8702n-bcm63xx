/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Filename: signalHandler.c
*
****************************************************************************
*    Description:
*
*     Implements functions related to the signal exception handler.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <sys/types.h>
#include <unistd.h>

#include <signalHandler.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   signalRegisterHandler
**
** PURPOSE:    Registers signal handler for the application 
**
** PARAMETERS: handler     (in)  Signal handler passed by the application.
**
** RETURNS:
**
** NOTE:
** 
** *****************************************************************************
*/
void signalRegisterHandler( SIGNAL_HANDLER handler )
{
   struct sigaction sa;

   sa.sa_sigaction = handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_SIGINFO;

   sigaction( SIGHUP ,  &sa,  NULL );     /* Hangup (POSIX).  */
   sigaction( SIGINT ,  &sa,  NULL );     /* Interrupt (ANSI).  */
   sigaction( SIGQUIT,  &sa,  NULL );     /* Quit (POSIX).  */
   sigaction( SIGILL ,  &sa,  NULL );     /* Illegal instruction (ANSI).  */
   sigaction( SIGTRAP,  &sa,  NULL );     /* Trace trap (POSIX).  */
   sigaction( SIGABRT,  &sa,  NULL );     /* Abort (ANSI).  */
   sigaction( SIGEMT,   &sa,  NULL );
   sigaction( SIGFPE ,  &sa,  NULL );     /* Floating-point exception (ANSI).  */
   sigaction( SIGBUS,   &sa,  NULL );     /* BUS error (4.2 BSD).  */
   sigaction( SIGSEGV,  &sa,  NULL );     /* Segmentation violation (ANSI).  */
   sigaction( SIGSYS,   &sa,  NULL );
   sigaction( SIGPIPE,  &sa,  NULL );     /* Broken pipe (POSIX).  */
   sigaction( SIGALRM,  &sa,  NULL );     /* Alarm clock (POSIX).  */
   sigaction( SIGTERM,  &sa,  NULL );     /* Termination (ANSI).  */
   sigaction( SIGUSR1,  &sa,  NULL );     /* User-defined signal 1 (POSIX).  */
   sigaction( SIGUSR2,  &sa,  NULL );     /* User-defined signal 2 (POSIX).  */
   sigaction( SIGCHLD,  &sa,  NULL );     /* Child status has changed (POSIX).  */
   sigaction( SIGPWR,   &sa,  NULL );     /* Power failure restart (System V).  */
   sigaction( SIGWINCH, &sa,  NULL );     /* Window size change (4.3 BSD, Sun).  */
   sigaction( SIGURG,   &sa,  NULL );     /* Urgent condition on socket (4.2 BSD).  */
   sigaction( SIGIO,    &sa,  NULL );     /* I/O now possible (4.2 BSD).  */
   sigaction( SIGTSTP,  &sa,  NULL );     /* Keyboard stop (POSIX).  */
   sigaction( SIGCONT,  &sa,  NULL );     /* Continue (POSIX).  */
   sigaction( SIGTTIN,  &sa,  NULL );     /* Background read from tty (POSIX).  */
   sigaction( SIGTTOU,  &sa,  NULL );     /* Background write to tty (POSIX).  */
   sigaction( SIGVTALRM,&sa,  NULL );     /* Virtual alarm clock (4.2 BSD).  */
   sigaction( SIGPROF,  &sa,  NULL );     /* Profiling alarm clock (4.2 BSD).  */
   sigaction( SIGXCPU,  &sa,  NULL );     /* CPU limit exceeded (4.2 BSD).  */
   sigaction( SIGXFSZ,  &sa,  NULL );     /* File size limit exceeded (4.2 BSD).  */

   /* Ignore the Interrupt Signal (Ctrl -C) as it may interrupt the endpoint 
      packetAvailableSemaphore and cause an infinite while loop to execute if 
      that particular kernel thread begins running before the voice deinit
      can give up that semaphore. It is safer for now to just ignore this 
      signal */
   signal(SIGINT, SIG_IGN);
}


/*
*****************************************************************************
** FUNCTION:   signalPrintInfo
**
** PURPOSE:    Get info about the signal that occurred.
**
** PARAMETERS: sig         (in)  Signal number
**             siginfo     (in)  Siginfo structure that was passed to the handler
**
** RETURNS:
**
** NOTE:       This function should only be called from the signal handler.
** 
** *****************************************************************************
*/
void signalPrintInfo( int sig, siginfo_t* siginfo )
{
   char cmd[256];
   int pid;

   /* No need to print signal info when a voice stop is issued */
   if ( sig == SIGTERM )
   {
      printf("VODSL received a SIGTERM (%d). Voice Shutting down...\n", sig);
	  return;
   }

   memset (cmd, 0, sizeof(cmd));
   pid = getpid();

   printf("------------------------------------------------------------\n");
   printf("    *** Received signal %d on thread with PID %d\n", sig, pid );
   printf("    *** siginfo->si_signo   = %d\n", siginfo->si_signo );
   printf("    *** siginfo->si_errno   = %d\n", siginfo->si_errno );
   printf("    *** siginfo->si_code    = %d\n", siginfo->si_code );
   printf("    *** siginfo->si_pid     = %d\n", siginfo->si_pid );
   printf("    *** siginfo->si_addr    = 0x%08x\n", (unsigned int)siginfo->si_addr );
   printf("    *** siginfo->si_status  = 0x%08x\n", (unsigned int)siginfo->si_status );
   printf("------------------------------------------------------------\n");

   printf("/proc/%d/stat : \n", pid);
   snprintf(cmd, sizeof(cmd), "cat /proc/%d/stat", pid);
   system(cmd);
   printf("\n");

   printf("/proc/%d/status : \n", pid);
   snprintf(cmd, sizeof(cmd), "cat /proc/%d/status", getpid());
   system(cmd);
   printf("\n");

   printf("/proc/%d/maps : \n", pid);
   snprintf(cmd, sizeof(cmd), "cat /proc/%d/maps", getpid());
   system(cmd);
   printf("\n");
}
