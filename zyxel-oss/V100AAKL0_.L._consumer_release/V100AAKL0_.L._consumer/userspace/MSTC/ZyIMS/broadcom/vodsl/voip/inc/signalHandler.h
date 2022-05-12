/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: signalHandler.h
*
****************************************************************************
*    Description:
*
*     Public APIs for setting up the signal exception handler
*     and obtaining signal info once the signal is catched.
*
****************************************************************************/

#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

/* ---- Constants and Types ---------------------------------------------- */

typedef void (*SIGNAL_HANDLER)( int sig, siginfo_t* siginfo, void* notused );

/* ---- Function Prototypes ---------------------------------------------- */

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
void signalRegisterHandler( SIGNAL_HANDLER handler );

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
void signalPrintInfo( int sig, siginfo_t* siginfo );

#ifdef __cplusplus
}
#endif

#endif /* SIGNAL_HANDLER_H */

