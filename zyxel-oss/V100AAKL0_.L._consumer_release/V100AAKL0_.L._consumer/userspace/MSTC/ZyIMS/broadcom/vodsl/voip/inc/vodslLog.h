/***************************************************************************
*    Copyright 2006  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: vodslLog.h
*    Creation Date: 19 July 2006 (v0.00)
*    VSS Info:
*        $Revision: 23 $
*        $Date: 7/19/06 4:54p $
*
****************************************************************************
*    Description:
*
*     This header file contains the needed macros and function prototypes
*     for logging on the terminal. It is intented for the generic use for 
*     tasks in the user space or in the VODSL process.  
*     For module specific logging, refer to the module specific header file
*     and implementation if there is one.
*
****************************************************************************/

#ifndef VODSL_LOG_H
#define VODSL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#ifndef LOG
#define LOG(fmt)	vodslLog fmt ;
#endif
#ifndef LOGINFO
#define LOGINFO(fmt)	vodslLogInfo fmt ;
#endif
#ifndef LOGERROR
#define LOGERROR(fmt)	vodslLogErr fmt ;
#endif

#define VODSL_LOG(fmt)        vodslLog fmt ; 
#define VODSL_LOGINFO(fmt)    vodslLogInfo fmt ;
#define VODSL_LOGERROR(fmt)   vodslLogErr fmt ;

/* The log levels are inline with linux kernel system log levels. We will
 * use less log level in user space logging. Each module can decide how to 
 * map its log levels to this one so that it can differenciate with other
 * modules. When a desired log level is configured, a logging with higher
 * priorities (smaller number) will be displayed.*/
typedef enum
{
   VODSL_LOG_OFF     = 0,
   VODSL_LOG_EMERG   = 1,   
   VODSL_LOG_CRIT    = 2,
   VODSL_LOG_ALERT   = 3,
   VODSL_LOG_ERROR   = 4,
   VODSL_LOG_WARNING = 5,
   VODSL_LOG_NOTICE  = 6,
   VODSL_LOG_INFO    = 7,
   VODSL_LOG_DEBUG   = 8
}VODSL_LOGLEVEL;

/* Expand this module list for any module that needs
 * individual log implementation
 */
typedef enum
{
   VODSL_MODULE_GENERAL = 0,	
   VODSL_MODULE_CMGR = 1,
   /* Add new module before this line, and increment
    * VODSL_MODULE_MAX which will be used for defining
    * module array size.  Do not create gaps in the numbering.
    */
   VODSL_MODULE_MAX = 2
}VODSL_MODULE;

/***********************************
** Error logging                   *
***********************************/

void vodslLog( const char *format,...);
void vodslLogInfo( const char *format,...);
void vodslLogErr( const char *format,...);

void vodslPrint( const char *format, va_list va);

VODSL_LOGLEVEL vodslGetLogLevel(VODSL_MODULE module);

void vodslSetLogLevel(VODSL_LOGLEVEL level);

#ifdef __cplusplus
    }
#endif

#endif /* VODSL_LOG_H */

