/******************************************************************************
*                                    
* FILE NAME    : aDigest_macro.h                
*                                    
* DESCRIPTION  : Contains the definitions for macros. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Alok Tiwari            LLD           aDigest 1.0   
*
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ADIGEST_MACRO_H__
#define __ADIGEST_MACRO_H__

#include "common_types.h"
#include "common_defs.h"
#include "aDigest_defs.h"


/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
/* This macro is used to convert the bits in the 128 bit digest (from most
 * significant to least significant bit,four bits at a time) to their
 * ASCII presentation as follows.
 * Each four bits is represented by its familiar hexadecimal
 * notation from the characters 0123456789abcdef. i.e. binary 0000 gets
 * represented by the character '0', 0001, by '1', and so on up to the
 * representation of 1111 as 'f'.
 */
  
#define ADIGEST_CONVERT_TO_HEXSTR(chksum,hex) \
{\
    util_uint8_t    hex_char[16] =   \
      {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'}; \
    util_uint16_t   index = 0;        \
    util_uint8_t    value = 0;    \
    (util_void_t)value;     \
    while (index < ADIGEST_CHECKSUM_LEN)        \
     {\
         value = (chksum[index] >> 4) & 0xf ; \
         hex[index*2] = hex_char[value];       \
          \
         value = chksum[index] & 0xf ;     \
         hex [index*2 + 1] = hex_char[value];  \
         index++ ; \
    }\
    hex [ADIGEST_HEX_CHECKSUM_LEN] = '\0';  \
} 


/* This Macro is used to print a given string.*/
#define ADIGEST_PRINT  UTIL_PRINT

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ADIGEST_MACRO_H__ */



