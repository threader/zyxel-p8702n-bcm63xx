/******************************************************************************
*                                    
* FILE NAME    : common_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the common prototypes. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Anuradha Gupta         LLD           Start of Code
*           
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __COMMON_PROTOTYPES_H__
#define __COMMON_PROTOTYPES_H__

#include "common_defs.h"
#include "common_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
/*************************************************************************
*
*  FUNCTION NAME  :   util_strlen
*
*  DESCRIPTION    :   Utility function for calculating string length.
*
*  RETURNS        :   A U32 holding value of the length of the string  
*
 *************************************************************************/
util_uint32_t
util_strlen (IN util_uint8_t    *p_s);


/*************************************************************************
*  FUNCTION NAME  :   util_strcmpi
*  DESCRIPTION    :   Utility function for performing case-insensitive
*  			string comparison
 *************************************************************************/
util_uint8_t util_strcmpi(
		IN  util_uint8_t	        *p_target,
		IN  util_uint8_t			*p_match);


/*******************************************************************************
 * FUNCTION: util_port_random 
 * 
 * Description: This is the porting function to generate a random number. 
 *              
 *
 * Return : An integer value.
 *******************************************************************************/
 util_uint32_t   util_port_random (util_void_t) ;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMON_PROTOTYPES_H__ */



