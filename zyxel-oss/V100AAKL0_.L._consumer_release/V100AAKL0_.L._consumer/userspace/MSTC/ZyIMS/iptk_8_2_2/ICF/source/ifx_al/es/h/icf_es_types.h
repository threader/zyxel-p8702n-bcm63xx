/*------------------------------------------------------------------------------
 *
 * File name        : icf_es_types.h
 *
 * Purpose          : This file contains the prototypes of all functions 
 *                    used by ES 
 *                    
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 14-Feb-2005  Pankaj Negi     Non Service LLD     Initial
 *
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __ICF_ES_TYPES_H__
#define __ICF_ES_TYPES_H__


#include "icf_es_defs.h"
#include "icf_port.h"
#ifdef ICF_PORT_VXWORKS
#include "icf_port_types.h"
#endif
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

/* --------------------------------------------------------------------------
 * Constants to describe function arguments
 * --------------------------------------------------------------------------*/
#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN   const
#endif

/* --------------------------------------------------------------------------
 * Constant to set and check for NULL
 * --------------------------------------------------------------------------*/
#define ICF_NULL 0

/* --------------------------------------------------------------------------
 * Constants to check the return value of functions
 * --------------------------------------------------------------------------*/
/*#define ICF_FAILURE   0
#define ICF_SUCCESS   1
*/


/* --------------------------------------------------------------------------
 * Constants to set and check the boolean varibles
 * --------------------------------------------------------------------------*/
/*#define ICF_TRUE 1
#define ICF_FALSE 0
*/
    

#define ICF_MAX_EXT_MODULES  30
 
    /* Data structures and hash define constants
     * needed to make a RAM drive on the VxWorks Platform*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_ES_PROTOTYPES_H__ */    
