/*------------------------------------------------------------------------------
 *
 * File name        : icf_vldt_port_utils.c
 *
 * Purpose          : Contains the definitions for the functions used in 
 *                    PA Validation. 
 *
 * Revision History :
 *
 * Date         Name              Ref#            Comments
 * --------     ------------      ----            ---------------
 * 13-JUN-2007  Jagmohan Chauhan   No              Initial
                Rishi Saxena
 * Copyright (c) 2007, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_vldt_prototypes.h"

/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strcpy
*
*  DESCRIPTION    :   Utility function for string copy.
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_vldt_port_strcpy(
        INOUT icf_uint8_t   *p_dest,
        IN  icf_uint8_t   *p_src)
{
    while ((*p_dest++ = *p_src++) != '\0');
}



/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strcat
*
*  DESCRIPTION    :   Utility function for string cat.
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_vldt_port_strcat(
         icf_uint8_t   *p_dest,
          icf_uint8_t   *p_src)
{
    while(*p_dest)
        p_dest++;

    while((*p_dest++ = *p_src++) != '\0');
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strlen
*
*  DESCRIPTION    :   Utility function for calculating string length.
*
*  RETURNS        :   A U32 holding value of the length of the string
*
 *************************************************************************/
icf_uint32_t
icf_vldt_port_strlen ( icf_uint8_t    *p_s)
{
    const icf_uint8_t    *p_sc;

    for(p_sc = p_s; *p_sc != '\0'; ++p_sc);

    return p_sc - p_s;
}


/*************************************************************************
*
*  FUNCTION NAME  :   icf_vldt_port_strchr
*
*  DESCRIPTION    : Returns pointer to first occurrrnce of a character in a
*                    given string.
*
*  RETURNS        :   Returns pointer to first occurrrnce of a character in a
*                    given string.
*
*************************************************************************/
icf_uint8_t *
icf_vldt_port_strchr (
         icf_uint8_t                 *p_string,
         icf_uint8_t                  character)
{

    for( ; *p_string != '\0'; p_string++)
    {
        /*
         * * If char is same.
         */
        if(*p_string == character)
        {
            return (icf_uint8_t *)p_string;
        }
    }
    return ICF_NULL;

}
