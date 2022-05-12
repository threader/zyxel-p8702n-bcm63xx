#ifdef SDF_AUTHENTICATE
/******************************************************************************
** FUNCTION:
**			This file has the implementation of the Basic authentication
**			scheme.
**
*******************************************************************************
**
** FILENAME			: basic.h
**
** DESCRIPTION		: This file has the implementation of the Basic
**						authentication scheme
**  	
** DATE			NAME		REFERENCE		REASON
** ----			----		---------		------
** 13/01/01 	Subhash						Creation
**
*******************************************************************************
**				COPYRIGHT , Aricent, 2006
*******************************************************************************/
#ifndef __SDFBASIC_H__
#define __SDFBASIC_H__

#include "sdf_portlayer.h"

#ifdef __cplusplus
extern "C" {
#endif 

extern void encodeBase64 _ARGS_((char *input, char *output));

extern void decodeBase64 _ARGS_((char* input,char* result));
#ifdef __cplusplus
}
#endif 

#endif
#endif /* SDF_AUTHENTICATE */
