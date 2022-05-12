/********************************************************************
 * Copyright (C) 2008 Aricent Inc . All Rights Reserved
 *
 * $Id: cryardef.h,v 1.1 2009/02/27 05:53:09 gur16603 Exp $
 *
 * Description: This file contains the common defines for Aricent 
 * Crypto.
 *********************************************************************/
#ifndef _CRY_DEFS_H
#define _CRY_DEFS_H

/* Undefine names that are defined in standard system headers. */
#undef PRIVATE
#undef PUBLIC
#undef IN
#undef INOUT
#undef OUT

/* Define the following if debugging is required */

#ifndef NULL
#define NULL    (0)
#endif

#if !defined(PRIVATE)
#define PRIVATE static
#endif

#if !defined(PUBLIC)
#define PUBLIC  extern
#endif

#if !defined(IN)
#define IN  const
#endif

#if !defined(OUT)
#define OUT  
#endif

#if !defined(INOUT)
#define INOUT 
#endif

#ifdef CRYPTO_DEBUG_LOG
#define CRYPTO_DEBUG_PRINT    printf
#else
#define CRYPTO_DEBUG_PRINT
#endif

#ifdef   __STDC__
#define  PROTO(x)     x
#else
#define  PROTO(x)     ()
#endif

#define CRY_ARRAY_INDEX_ZERO     0
#define CRY_ARRAY_INDEX_ONE      1
#define CRY_ARRAY_INDEX_TWO      2
#define CRY_ARRAY_INDEX_THREE    3
#define CRY_ARRAY_INDEX_FOUR     4
#define CRY_ARRAY_INDEX_FIVE     5
#define CRY_ARRAY_INDEX_SIX      6
#define CRY_ARRAY_INDEX_SEVEN    7
#define CRY_ARRAY_INDEX_EIGHT    8
#define CRY_ARRAY_INDEX_NINE     9
#define CRY_ARRAY_INDEX_TEN      10
#define CRY_ARRAY_INDEX_ELEVEN   11
#define CRY_ARRAY_INDEX_TWELVE   12
#define CRY_ARRAY_INDEX_THIRTEEN 13
#define CRY_ARRAY_INDEX_FOURTEEN 14
#define CRY_ARRAY_INDEX_FIFTEEN  15


#define CRY_MEMCPY      memcpy
#define CRY_INIT_VAL    0
#endif
