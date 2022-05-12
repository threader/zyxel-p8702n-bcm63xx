#ifdef SDF_AUTHENTICATE
/******************************************************************************
** FUNCTION:
**			This file has the implementation of the MD5 algorithm
**
*******************************************************************************
**
** FILENAME			: md5.h
**
** DESCRIPTION		: This file has the implementation of the MD5 algorithm
**  	
** DATE			NAME		REFERENCE		REASON
** ----			----		---------		------
** 13/01/01 	Subhash						Creation
**
*******************************************************************************
**				COPYRIGHT , Aricent, 2006
*******************************************************************************/

/* MD5.H - header file for MD5C.C
 */

/* COPYRIGHT , Aricent, 2006
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */
#ifndef __SDF_MD5_H__
#define __SDF_MD5_H__

#include "global.h"

/* MD5 context. */
	/* MD5 context. */
typedef struct {
  SDF_UINT4 state[4];                                   /* state (ABCD) */
  SDF_UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} SDF_MD5_CTX;

#ifdef __cplusplus
extern "C" {
#endif


void sdf_MD5Init SDF_PROTO ((SDF_MD5_CTX *));

void sdf_MD5Update SDF_PROTO ((SDF_MD5_CTX *, unsigned char *, unsigned int));

void sdf_MD5Final SDF_PROTO ((unsigned char [16], SDF_MD5_CTX *));

#ifdef __cplusplus
}
#endif 

#endif
#endif /* SDF_AUTHENTICATE */

