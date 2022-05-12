/******************************************************************************
*                                    
* FILE NAME    : common_util.c                
*                                    
* DESCRIPTION  : Contains the util utility routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 03-Dec-2008    Anuradha Gupta         None          Initial
* 04-Dec-2008    Anuradha Gupta         LLD           Start of Code
* 04-Dec-2008    Rajiv Kumar            IPTK Rel 8.2  Vxwork 5.4 Porting
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#include "common_defs.h"
#include "common_types.h"
#include "common_prototypes.h"

/*Changes done for Vxwork 5.4 Porting*/
#if defined(UTIL_PORT_LINUX) || defined (UTIL_PORT_VXWORKS)
#include <stdarg.h>
#endif

#ifdef UTIL_PORT_VXWORKS
#include "stdlib.h"
#endif

/*******************************************************************************
 * FUNCTION: util_port_random 
 * 
 * Description: This is the porting function to generate a random number. 
 *
 * Return : An integer value.
 *******************************************************************************/
 util_uint32_t 
 util_port_random (util_void_t)
 {
    /* Generate a random number */ 

#if defined(UTIL_PORT_LINUX)
       return random();
#elif defined (UTIL_PORT_VXWORKS)
       return rand();
#else
       return 0;
#endif
 }

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
util_strlen (IN util_uint8_t    *p_s)
{
    const util_uint8_t    *p_sc;

    for(p_sc = p_s; *p_sc != '\0'; ++p_sc);

    return p_sc - p_s;
}


/*************************************************************************
*  FUNCTION NAME  :   util_strcmpi
*  DESCRIPTION    :   Utility function for performing case-insensitive
*  			string comparison
 *************************************************************************/
util_uint8_t util_strcmpi(
		IN  util_uint8_t	        *p_target,
		IN  util_uint8_t			*p_match)
{
	util_uint32_t							i = 0;
	util_uint8_t							temp1 = 0;
	util_uint8_t							temp2 = 0;

    /*Klocwork warning resolved.*/
    (util_void_t)temp1;
    (util_void_t)temp2;

	while(p_target[i])
	{ 
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((p_target[i] >= 97) && (p_target[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp1 = (util_uint8_t)(p_target[i] - 32);
		}
		else
		{
			temp1 = p_target[i];
		}
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((p_match[i] >= 97) && (p_match[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp2 = (util_uint8_t)(p_match[i] - 32);
		}
		else
		{
			temp2 = p_match[i];
		}

		if (temp1 != temp2)
			return((util_uint8_t)temp1 - temp2);
		i++;
	}

	if(p_match[i])
		return((util_uint8_t)0 - p_match[i]);

	return 0;
}

        
