/******************************************************************************
*                                    
* FILE NAME    : common_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the common data types. 
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

#ifndef __COMMON_TYPES_H__
#define __COMMON_TYPES_H__

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    
/*
---------------------------- BASIC TYPES -------------------------------------
*/

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_void_t
 * 
 * Description: Represent the type void.
 *
 *---------------------------------------------------------------------------*/
typedef void util_void_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_uint8_t
 *
 * Description: This data type represents a 1 byte unsigned integer.
 *
 *---------------------------------------------------------------------------*/
typedef unsigned char util_uint8_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_boolean_t
 * 
 * Description: Represent boolean vales
 *
 *---------------------------------------------------------------------------*/
typedef util_uint8_t util_boolean_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_return_t
 * 
 * Description: Represent the  execution status values return by a function.
 *              This type is used in function interface and not in message based
 *              interface.
 *
 *---------------------------------------------------------------------------*/
typedef util_boolean_t util_return_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_uint16_t
 * 
 * Description: This data type represents a 2 byte unsigned integer. 
 * 
 *---------------------------------------------------------------------------*/

typedef unsigned short util_uint16_t;



/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_uint16_t
 * 
 * Description: This data type represents a 2 byte unsigned error code. 
 * 
 *---------------------------------------------------------------------------*/
typedef util_uint16_t           util_error_t;

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_uint32_t
 *
 * Description: This data type represents a 4 byte unsigned long integer.
 *  
 *---------------------------------------------------------------------------*/
typedef unsigned int util_uint32_t;



/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_int8_t
 * 
 * Description: This data type represents a 1 byte signed integer
 *
 *---------------------------------------------------------------------------*/
typedef char util_int8_t;



/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_int16_t
 *
 * Description: This data type represents a 2 byte-signed integer.
 * 
 *---------------------------------------------------------------------------*/
typedef signed short util_int16_t; 

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: util_int32_t
 *
 * Description: This data type represents a 4 byte signed long integer
 *
 *---------------------------------------------------------------------------*/
typedef signed int util_int32_t;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: util_string_st
 * 
 * Description: This data type represents a structure, which consists of a 
 *              character pointer of specified length
 *
 *---------------------------------------------------------------------------*/
typedef struct        
{
    /* character pointer to point a string */ 
    util_uint8_t *str;

   /* specifies the length of the string including '\0' character */ 
    util_uint16_t str_len;
} util_string_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMON_TYPES_H__ */



