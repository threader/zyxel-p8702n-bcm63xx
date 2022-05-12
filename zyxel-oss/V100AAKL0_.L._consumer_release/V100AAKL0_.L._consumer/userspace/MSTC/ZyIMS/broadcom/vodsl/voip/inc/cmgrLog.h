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
*    Filename: cmgrLog.h
*    Creation Date: 19 July 2006 (v0.00)
*    VSS Info:
*        $Revision: 23 $
*        $Date: 7/19/06 4:54p $
*
****************************************************************************
*    Description:
*
*     This header file contains the needed macros and function prototypes
*     for logging on the terminal. It is intented for the logging of call 
*     manager module.
*
****************************************************************************/

#ifndef CMGR_LOG_H
#define CMGR_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vodslLog.h>

#define CMGR_LOG(fmt)        cmgrLog fmt ;
#define CMGR_LOGWARN(fmt)    cmgrLogWarn fmt ;
#define CMGR_LOGINFO(fmt)    cmgrLogInfo fmt ;
#define CMGR_LOGERROR(fmt)   cmgrLogErr fmt ;
#define CMGR_LOGDBG(fmt)     cmgrLogDebug fmt ;


/***********************************
** Error logging                   *
***********************************/

void cmgrLog( const char *format,...);
void cmgrLogWarn( char *pModName, const char *format,...);
void cmgrLogInfo( char *pModName, const char *format,...);
void cmgrLogErr( char *pModName, const char *format,...);
void cmgrLogDebug( char *pModName, const char *format,...);

#ifdef __cplusplus
    }
#endif

#endif /* CMGR_LOG_H */



