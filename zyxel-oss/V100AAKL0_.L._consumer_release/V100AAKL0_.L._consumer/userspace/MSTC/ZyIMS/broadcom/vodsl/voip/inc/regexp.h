/***************************************************************************
*    Copyright 2000  Broadcom Corporation
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
*    Filename: regexp.h
*    Creation Date: 27 June 1999 (v0.00)
*    VSS Info:
*        $Revision: 1 $
*        $Date: 10/03/01 6:23p $
*
****************************************************************************
*    Description:
*
*      This file contains the regular expression definitions
*
****************************************************************************/

/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */

#ifndef _REGEXP
#define _REGEXP 1
#if defined(__cplusplus)
extern "C"
{
#endif
extern void *regcomp(void *regbuf, char *exp);
extern int regexec(void *rprog, char *string);

#if defined(__cplusplus)
}
#endif 

#endif /* REGEXP */
