/******************************************************************************
** FUNCTION:
**			This file has the types and constants required for the
**			Digest authentication scheme.
**
*******************************************************************************
**
** FILENAME			: global.h
**
** DESCRIPTION		: This file has the types and constants required for the
**						Digest authentication scheme.
**  	
** DATE			NAME		REFERENCE		REASON
** ----			----		---------		------
** 13/01/01 	Subhash						Creation
**
*******************************************************************************
**				COPYRIGHT , Aricent, 2006
*******************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/* GLOBAL.H - RSAREF types and constants
 */

/* PROTOTYPES should be set to one if and only if the compiler supports
  function argument prototyping.
  The following makes PROTOTYPES default to 0 if it has not already
  been defined with C compiler flags.
 */

/* SDF_POINTER defines a generic pointer type */
typedef unsigned char *SDF_POINTER;

/* SDF_UINT2 defines a two byte word */
typedef unsigned short int SDF_UINT2;

/* SDF_UINT4 defines a four byte word */
typedef unsigned long int SDF_UINT4;

/* SDF_PROTO is defined depending on how PROTOTYPES is defined above.
If using PROTOTYPES, then SDF_PROTO returns the list, otherwise it
  returns an empty list.
 */
#ifdef ANSI_PROTO
#define SDF_PROTO(list) list
#else
#define SDF_PROTO(list) ()
#endif

#endif
