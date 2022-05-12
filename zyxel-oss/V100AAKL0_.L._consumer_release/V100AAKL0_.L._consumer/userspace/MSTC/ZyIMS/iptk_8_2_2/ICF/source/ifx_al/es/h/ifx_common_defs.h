/*****************************************************************************
 **   FILE NAME    : ifx_common_defs.h
 **   PROJECT      : All Projects
 **   MODULES      : All Modules
 **   SRC VERSION  : V1.0
 **
 **   DATE         : 14-01-2004
 **   AUTHOR       : IFIN COM Engineers
 **   DESCRIPTION  :
 **   FUNCTIONS    :
 **   COMPILER     :
 **   REFERENCE    : Coding guide lines for VSS, DIS of CCS.
 **   COPYRIGHT    : Infineon Technologies AG 2003 - 2004
 **
 **  Version Control Section
 **   $Author: achhibber $
 **   $Date: 2006/04/17 04:02:28 $
 **   $Revisions$
 **   $Log: ifx_common_defs.h,v $
 **   Revision 1.3  2006/04/17 04:02:28  achhibber
 **   no message
 **       Revision history
 ****************************************************************************/
#ifndef		__IFX_COMMON_DEFS_H__
#define		__IFX_COMMON_DEFS_H__


#define	PUBLIC
#define	EXTERN			extern
#define	STATIC			static

#define	PRINT			printf

#define	IN
#define	OUT
#define	IN_OUT

typedef char               char8;
typedef unsigned char      uchar8;
typedef char               int8;
typedef unsigned char      uint8;
typedef short int	         int16;
typedef unsigned short int	uint16;
typedef int                int32;
typedef unsigned int	      uint32;
typedef long               int64;
typedef unsigned long int	uint64;
typedef float              float32;
typedef double             float64;
typedef float              double32;
typedef double             double64;
typedef long               long32;
typedef unsigned long      ulong32;
typedef char               bool;
typedef char               boolean;

#endif	/* __IFX_COMMON_DEFS_H__ */
