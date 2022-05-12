/*****************************************************************************
* FUNCTION    : Source file for Generic library - Common String Utilities
*               implementation
******************************************************************************
*
* FILENAME    : gl_ulm_str_manipultn_api.c
*
* DESCRIPTION : This file implements string manipulation functionality of 
*               generic libraries. 
*
* DATE            NAME         REFERENCE        REASON
*----------   -------------    ---------    -------------------
* April'01      Rajesh Jain                 New file
*                                           Generic Libraries
* July' 01      Shobhit Bansal              Functions for case 
*                                           insensitive string compare
*                                           added
* 18/10/2001    Ravi Jindal     SPR-2788    Packaging of Genlib-Code.
* 19/10/2001    Ravi Jindal     SPR-2802    Adding Pre-ANSI-C features.
* 17/11/2001    Ravi Jindal     SPR-3107    Incoprate the defects and change
*                                           cosmmetics of file.
*
* 03/04/2002    Ravi Jindal     SPR-3959    Enhancements for Rel-1.0.
*
* 08/10/2002    Ravi Jindal     SPR-4910    Fix in strcmpi, strncmpi.    
*
* 16/10/2002    Prashant Jain   SPR-4948    Fix in strcmpi, strncmpi.    
*
* 31/05/2003    Prashant Jain   SPR-5810    Fix in the function "gl_stm_itoa"
* 
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/


#include "gl_ulm_interfaces.h"


/*****************************************************************************
* Function Name    : gl_stm_strcpy
* Arguments        : gw_U8bit           *p_dest_buffer,
*                    gw_U8bit           *p_source_buffer,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Copy one string to another.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_stm_strcpy

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_dest_buffer,
      gw_U8bit                  *p_source_buffer, 
      gl_error_t                *p_ecode
    )
#else
    ( p_dest_buffer, p_source_buffer, p_ecode )
      gw_U8bit                  *p_dest_buffer;
      gw_U8bit                  *p_source_buffer;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   index1 = 0;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_source_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_SRC_STRING_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL == p_dest_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_DEST_STRING_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    while('\0' != p_source_buffer[index1])
    {
        p_dest_buffer[index1] = p_source_buffer[index1];
        index1++;
    }
    p_dest_buffer[index1] = '\0';
    
    return GL_SUCCESS;
}




/*****************************************************************************
* Function Name    : gl_stm_strncpy
* Arguments        : gw_U8bit           *p_dest_buffer,
*                    gw_U8bit           *p_source_buffer,
*                    gw_U32bit          num_of_characters,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Copy one string to another.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_stm_strncpy

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_dest_buffer,
      gw_U8bit                  *p_source_buffer,
      gw_U32bit                 num_of_characters,
      gl_error_t                *p_ecode
    )
#else
    ( p_dest_buffer, p_source_buffer, num_of_characters, p_ecode )
      gw_U8bit                  *p_dest_buffer;
      gw_U8bit                  *p_source_buffer;
      gw_U32bit                 num_of_characters;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   index1 = 0;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_source_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_SRC_STRING_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL == p_dest_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_DEST_STRING_PTR;
        return GL_FAILURE;
    }
    if(GL_STM_MAX_CHARS_IN_STRING < num_of_characters)
    {
        *p_ecode = GL_STM_ERR_INV_NUM_OF_CHARS;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    while(('\0' != p_source_buffer[index1]) && (index1 < num_of_characters ))
    {
        p_dest_buffer[index1] = p_source_buffer[index1];
        index1++;
    }
    p_dest_buffer[index1] = '\0';
    
    return GL_SUCCESS;
}


/*****************************************************************************
* Function Name    : gl_stm_strcat
* Arguments        : gw_U8bit           *p_dest_buffer,
*                    gw_U8bit           *p_source_buffer,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Append one string to another.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_stm_strcat

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_dest_buffer,
      gw_U8bit                  *p_source_buffer, 
      gl_error_t                *p_ecode
    )
#else
    ( p_dest_buffer, p_source_buffer, p_ecode )
      gw_U8bit                  *p_dest_buffer;
      gw_U8bit                  *p_source_buffer;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   sindex1=0,dindex1=0;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_source_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_SRC_STRING_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL == p_dest_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_DEST_STRING_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    while('\0' != p_dest_buffer[dindex1])
    {
        dindex1++;
    }
    
    while('\0' != p_source_buffer[sindex1])
    {
        p_dest_buffer[dindex1] = p_source_buffer[sindex1];
        sindex1++;
        dindex1++;
    }
    p_dest_buffer[dindex1] = '\0';
    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_stm_strncat
* Arguments        : gw_U8bit           *p_dest_buffer,
*                    gw_U8bit           *p_source_buffer,
*                    gw_U32bit          num_of_characters,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Append one string to another.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_stm_strncat

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_dest_buffer,
      gw_U8bit                  *p_source_buffer, 
      gw_U32bit                 num_of_characters,
      gl_error_t                *p_ecode
    )
#else
    ( p_dest_buffer, p_source_buffer, num_of_characters, p_ecode )
      gw_U8bit                  *p_dest_buffer;
      gw_U8bit                  *p_source_buffer;
      gw_U32bit                 num_of_characters;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   sindex1=0,dindex1=0;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_source_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_SRC_STRING_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL == p_dest_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_DEST_STRING_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    while('\0' != p_dest_buffer[dindex1])
    {
        dindex1++;
    }
    
    if(GL_STM_MAX_CHARS_IN_STRING < (num_of_characters + dindex1))
    {
        *p_ecode = GL_STM_ERR_INV_NUM_OF_CHARS;
        return GL_FAILURE;
    }
    
    while(('\0' != p_source_buffer[sindex1]) && (sindex1 < num_of_characters))
    {
        p_dest_buffer[dindex1] = p_source_buffer[sindex1];
        sindex1++;
        dindex1++;
    }
    p_dest_buffer[dindex1] = '\0';
    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_stm_strlen
* Arguments        : gw_U8bit           *p_buffer,
*                    gl_error_t         *p_ecode
* Return value     : gw_S32bit
* Functionality    : Find length of string.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_stm_strlen

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_buffer,
      gl_error_t                *p_ecode
    )
#else
    ( p_buffer, p_ecode )
      gw_U8bit                  *p_buffer;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   index1=0;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_buffer)
    {
        *p_ecode = GL_STM_ERR_NULL_SRC_STRING_PTR;
        return GW_ERROR;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    while('\0'!= p_buffer[index1])
    {
        ++index1;
    }

    return index1;
}

/*****************************************************************************
* Function Name    : gl_stm_strcmp
* Arguments        : gw_U8bit           *p_string1,
*                    gw_U8bit           *p_string2,
*                    gl_error_t         *p_ecode
* Return value     : gw_S8bit
* Functionality    : Compare two strings.
* Functions called : 
*****************************************************************************/
gw_S8bit 
gl_stm_strcmp

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string1, 
      gw_U8bit                  *p_string2,
      gl_error_t                *p_ecode
    )
#else
    ( p_string1, p_string2, p_ecode )
      gw_U8bit                  *p_string1;
      gw_U8bit                  *p_string2;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   counter1=0;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_NULL == p_string1) || (GL_NULL == p_string2))
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GW_ERROR;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    while((p_string1[counter1] == p_string2[counter1]) &&
          (p_string1[counter1]!='\0') && (p_string2[counter1]!='\0'))
    {
        counter1++;
    }
    
    if(p_string1[counter1] == p_string2[counter1])
    {
        return 0;
    }
    else if(p_string1[counter1] > p_string2[counter1])
    {
        return 1;
    }
    else
    {
        return 2;
    }
}


/*****************************************************************************
* Function Name    : gl_stm_strncmp
* Arguments        : gw_U8bit           *p_string1,
*                    gw_U8bit           *p_string2,
*                    gw_U32bit          compare_len,
*                    gl_error_t         *p_ecode
* Return value     : gw_S8bit
* Functionality    : Compare characters of two strings.
* Functions called :
*****************************************************************************/
gw_S8bit 
gl_stm_strncmp

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string1,
      gw_U8bit                  *p_string2,
      gw_U32bit                 compare_len,
      gl_error_t                *p_ecode
    )
#else
    ( p_string1, p_string2, compare_len, p_ecode )
      gw_U8bit                  *p_string1;
      gw_U8bit                  *p_string2;
      gw_U32bit                 compare_len;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   counter1=0;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_NULL == p_string1) || (GL_NULL == p_string2))
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GW_ERROR;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    while((p_string1[counter1] == p_string2[counter1])
        && (p_string1[counter1]!= '\0' && p_string2[counter1]!='\0')
        && (counter1 < (compare_len-1)))
    {
        counter1++;
    }

    if(p_string1[counter1] == p_string2[counter1])
    {
        return 0;
    }
    else if(p_string1[counter1] > p_string2[counter1])
    {
        return 1;
    }
    else
    {
        return 2;
    }
}


/*****************************************************************************
* Function Name    : gl_stm_atoi
* Arguments        : gw_U8bit           *p_str,
*                    gw_U32bit          *p_num,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Convert string into int.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_stm_atoi

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_str,
      gw_U32bit                 *p_num,
      gl_error_t                *p_ecode
    )
#else
    ( p_str, p_num, p_ecode )
      gw_U8bit                  *p_str;
      gw_U32bit                 *p_num;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   str_len = 0;
    gw_U32bit                   multiple = 1;
    gw_U32bit                   temp_num = 0;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_str)
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL == p_num)
    {
        *p_ecode = GL_STM_ERR_NULL_NUMBER_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    str_len = gl_stm_strlen(p_str, p_ecode);
    
    if(GW_ERROR == str_len)
    {
        return GL_FAILURE;
    }
    *p_num = 0;
    /*
     * Go to last char. 
     */
    p_str += (str_len-1);

    /*
     * Convert each char. 
     */
    while(str_len--)
    {
        /*
         * If char is other than 0-9 return error. 
         */
        if((*p_str <'0') || (*p_str >'9'))
        {
            return GL_FAILURE;
        }

        temp_num += (*(p_str--) - GL_ZERO_ASCII_VAL)*multiple;
        multiple = multiple*10;
    }

    *p_num = temp_num;
    return GL_SUCCESS;
}


/*****************************************************************************
* Function Name    : gl_stm_itoa
* Arguments        : gw_U32bit          num,
*                    gw_U8bit           str[],
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Convert int to string.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_stm_itoa

#ifdef _ANSI_C_
    ( gw_U32bit                 num,
      gw_U8bit                  str[],
      gl_error_t                *p_ecode
    )
#else
    ( num, str, p_ecode )
      gw_U32bit                 num;
      gw_U8bit                  str[];
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U8bit                    count = 0;
    gw_U8bit                    location = 0;
    gw_U32bit                   temp_num;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == str)
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    temp_num = num;
    /*
     * Find the length req. for string. 
     */
    while(temp_num != 0)
    {
        temp_num = temp_num/10;
        count++;
    }
    if(count == 0)
    {
        str[count] = GL_ZERO_ASCII_VAL;
        str[count+1] = '\0';
        return GL_SUCCESS;
    }

    str[ count ] = '\0';

    location = count;

    while( location-- )
    {
        count--;
        /*
         * Add ascii value of '0' to last digit. 
         * Then store that number of array. 
         */ 
        str[count] = (gw_U8bit) (num%10 + GL_ZERO_ASCII_VAL);
            
        num = num/10;
    }
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_stm_strcmpi
* Arguments        : gw_U8bit           *p_string1,
*                    gw_U8bit           *p_string2,
*                    gl_error_t         *p_ecode
* Return value     : gw_U8bit
* Functionality    : Compare strings. Case insensitive.
* Functions called : 
*****************************************************************************/
gw_S8bit 
gl_stm_strcmpi

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string1,
      gw_U8bit                  *p_string2,
      gl_error_t                *p_ecode
    )
#else
    ( p_string1, p_string2, p_ecode )
      gw_U8bit                  *p_string1;
      gw_U8bit                  *p_string2;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_NULL == p_string1) || (GL_NULL == p_string2))
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GW_ERROR;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    while((*p_string1 != '\0') && (*p_string2 != '\0'))
    {
        /*
         * If char is same.
         * Or Char is between a-z or A-Z. Then if they are differ by 32.
         */
        if( (*p_string1 == *p_string2) || 
            ((*p_string1 >='a' && *p_string1 <= 'z') && 
             (*p_string2 >='A' && *p_string2 <= 'Z') &&
             (*p_string1 - 32 == *p_string2)) ||
            ((*p_string1 >='A' && *p_string1 <= 'Z') && 
             (*p_string2 >='a' && *p_string2 <= 'z') &&
             (*p_string1 + 32 == *p_string2)) )
        {
            p_string1++;
            p_string2++;
        }
        else
        {
            /*
             * String are not equal.
             */
            break;
        }
    }

    if((*p_string1 == '\0') && (*p_string2 == '\0'))
    {
        return 0;
    }
    else if(*p_string1 > *p_string2)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}



/*****************************************************************************
* Function name    : gl_stm_strncmpi
* Arguments        : gw_U8bit           *p_string1,
*                    gw_U8bit           *p_string2,
*                    gw_U32bit          compare_len,
*                    gl_error_t         *p_ecode
* Return value     : gw_U8bit
* Functionality    : Compare strings for some length. Case insensitive.
* Functions called : 
*****************************************************************************/
gw_S8bit 
gl_stm_strncmpi

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string1,
      gw_U8bit                  *p_string2, 
      gw_U32bit                 compare_len,
      gl_error_t                *p_ecode
    )
#else
    ( p_string1, p_string2, compare_len, p_ecode )
      gw_U8bit                  *p_string1;
      gw_U8bit                  *p_string2;
      gw_U32bit                 compare_len;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_NULL == p_string1) || (GL_NULL == p_string2))
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GW_ERROR;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(0 == compare_len)
    {
        return 0;
    }

    while(compare_len > 0) 
    {
        /*
         * If char is same.
         * Or Char is between a-z or A-Z. Then if they are differ by 32.
         */
        if((*p_string1 != '\0') && 
           (*p_string2 != '\0') &&
           ( (*p_string1 == *p_string2) || 
             ( (*p_string1 >= 'a' && *p_string1 <= 'z') && 
               (*p_string2 >= 'A' && *p_string2 <= 'Z') &&
               (*p_string1 - 32 == *p_string2)
             )||
            (  (*p_string1 >= 'A' && *p_string1 <= 'Z') && 
               (*p_string2 >= 'a' && *p_string2 <= 'z') &&
               (*p_string1 + 32 == *p_string2)
            )))
        {
            p_string1++;
            p_string2++;
            compare_len--;
        }
        else
        {
            break;
        }
    }
    
    if((0 == compare_len) || ((*p_string1 == '\0') && (*p_string2 == '\0')))
    {
        return 0;
    }
    else if(*p_string1 > *p_string2)
    {
        return 1;
    }
    else
    {
        return 2;
    }

}



/*****************************************************************************
* Function name    : gl_stm_strchr
* Arguments        : gw_U8bit           *p_string,
*                    gw_U8bit           character,
*                    gl_error_t         *p_ecode
* Return value     : gw_U8bit *
* Functionality    : Returns pointer to first occurrrnce of a character in a 
*                    given string.
* Functions called : 
*****************************************************************************/
gw_U8bit * 
gl_stm_strchr

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string,
      gw_U8bit                  character,
      gl_error_t                *p_ecode
    )
#else
    ( p_string, character, p_ecode )
      gw_U8bit                  *p_string;
      gw_U8bit                  character;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_string)
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GL_NULL;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    for( ; *p_string != '\0'; p_string++)
    {
        /*
         * If char is same.
         */
        if(*p_string == character)  
        {
            return p_string;
        }
    }
    return GL_NULL;
}




/*****************************************************************************
* Function name    : gl_stm_strrchr
* Arguments        : gw_U8bit           *p_string,
*                    gw_U8bit           character,
*                    gl_error_t         *p_ecode
* Return value     : gw_U8bit *
* Functionality    : Returns pointer to last occurrrnce of a character in a 
*                    given string.
* Functions called : 
*****************************************************************************/
gw_U8bit * 
gl_stm_strrchr

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string,
      gw_U8bit                  character,
      gl_error_t                *p_ecode
    )
#else
    ( p_string, character, p_ecode )
      gw_U8bit                  *p_string;
      gw_U8bit                  character;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   str_len;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_string)
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GL_NULL;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    str_len = gl_stm_strlen(p_string, p_ecode);

    if(GW_ERROR == str_len)
    {
        return GL_NULL;
    }
    
    for( str_len -= 1; str_len >= 0; str_len--)
    {
        /*
         * If char is same.
         */
        if(p_string[str_len] == character)  
        {
            return (p_string + str_len);
        }
    }
    return GL_NULL;
}




/*****************************************************************************
* Function name    : gl_stm_strpbrk
* Arguments        : gw_U8bit           *p_string,
*                    gw_U8bit           *p_search_string,
*                    gl_error_t         *p_ecode
* Return value     : gw_U8bit *
* Functionality    : Returns pointer to first occurrence of any character 
*                    of string pointed by p_search_string in the given 
*                    string pointed by p_string. 
* Functions called : 
*****************************************************************************/
gw_U8bit * 
gl_stm_strpbrk

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string,
      gw_U8bit                  *p_search_string,
      gl_error_t                *p_ecode
    )
#else
    ( p_string, p_search_string, p_ecode )
      gw_U8bit                  *p_string;
      gw_U8bit                  *p_search_string;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U8bit                    *p_temp_str;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_string)
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GL_NULL;
    }
    if(GL_NULL == p_search_string)
    {
        *p_ecode = GL_STM_ERR_NULL_SEARCH_STRING_PTR;
        return GL_NULL;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    for( ; *p_string != '\0'; p_string++)
    {
        for(p_temp_str = p_search_string ; *p_temp_str != '\0'; p_temp_str++)
        {    
            /*
             * If char is same.
             */
            if(*p_string == *p_temp_str)  
            {
                return p_string;
            }
        }
    }
    return GL_NULL;
    
}



/*****************************************************************************
* Function name    : gl_stm_strstr
* Arguments        : gw_U8bit           *p_string,
*                    gw_U8bit           *p_search_string,
*                    gl_error_t         *p_ecode
* Return value     : gw_U8bit *
* Functionality    : Returns pointer to first occurrence of complete 
*                    string pointed by p_search_string in the given 
*                    string pointed by p_string. 
* Functions called : 
*****************************************************************************/
gw_U8bit * 
gl_stm_strstr

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_string,
      gw_U8bit                  *p_search_string,
      gl_error_t                *p_ecode
    )
#else
    ( p_string, p_search_string, p_ecode )
      gw_U8bit                  *p_string;
      gw_U8bit                  *p_search_string;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U8bit                    *p_temp_str;
    gw_U8bit                    *p_temp_src_str;
    gw_U8bit                    is_found_string = GL_TRUE;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_string)
    {
        *p_ecode = GL_STM_ERR_NULL_STRING_PTR;
        return GL_NULL;
    }
    if(GL_NULL == p_search_string)
    {
        *p_ecode = GL_STM_ERR_NULL_SEARCH_STRING_PTR;
        return GL_NULL;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    for( ; *p_string != '\0'; p_string++)
    {
        for(p_temp_str = p_search_string, p_temp_src_str = p_string ;
            *p_temp_str != '\0'; 
            p_temp_str++, p_temp_src_str++)
        {    
            if(*p_temp_src_str == '\0')
            {
                /*
                 * Number of characters in Source string is less than 
                 * Search string.
                 */
                return GL_NULL;
            }
            if(*p_temp_src_str != *p_temp_str)
            {
                /*
                 * If char is not same.
                 */
                is_found_string = GL_FALSE;
                break;
            }
        }
        if(GL_TRUE == is_found_string)
        {
            return p_string;
        }
    }
    return GL_NULL;
    
}

/****************************************************************************/
