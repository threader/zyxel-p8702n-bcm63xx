/***********************************************************************
*
*  env.h - Standard environment definitions
*
*  PURPOSE:
*     This file only includes cxcEnv.h, which contains the system wide definitions.
*
*  NOTE:
*     Copyright (c) 2000 Broadcom Canada Ltd.
*     All Rights Reserved.
*
***********************************************************************/
#ifndef ENV_H
#define ENV_H

#ifdef __KERNEL__

#include <cxcEnv.h>

#else

#include <stdint.h>

#endif 

#if !defined(SINT32)
#define SINT32 int
#endif
#if !defined(SINT16)
#define SINT16 short
#endif
#if !defined(SINT8)
#define SINT8 char
#endif
#if !defined(UINT32)
#define UINT32 unsigned int
#endif
#if !defined(UINT16)
#define UINT16 unsigned short
#endif
#if !defined(UINT8)
#define UINT8 unsigned char
#endif
#if !defined(BOOL)
#define BOOL int
#endif

#define NUMBER_TYPES_ALREADY_DEFINED

#ifndef FALSE
#define FALSE           0
#endif

#ifndef TRUE
#define TRUE            1
#endif

/***********************************
** Minimum and maximum macros      *
***********************************/
#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

typedef struct
{
   signed long   val;
   void  *ref;
} BCMSUBCMD;

typedef struct
{
   signed long   command;
   signed long   op1;
   signed long   op2;
   signed long   op3;
   BCMSUBCMD   op4;
} BCMCMD;

#endif /* ENV_H */
