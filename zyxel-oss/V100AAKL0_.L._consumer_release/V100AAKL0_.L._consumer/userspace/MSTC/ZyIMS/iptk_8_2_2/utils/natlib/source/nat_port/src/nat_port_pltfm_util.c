/******************************************************************************
*                                    
* FILE NAME    : nat_port_pltfm_util.c                
*                                    
* DESCRIPTION  : Contains the platform specific declarations.
*                
*                                    
* Revision History :                            
*
*  DATE            NAME                      REFERENCE       REASON        
* -----------    ------------                ---------     ---------------
* 20-Nov-2006    Anuradha Gupta                HLD         Initial Version
*
* Copyright 2006, Aricent                       
*                                    
*******************************************************************************/
#include "nat_types.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_error_id.h"
#include "nat_port_types.h"


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_static_memget
*
*  DESCRIPTION    :   This function is called to allocate static buffers
*                     in the system.
*
*
*  RETURNS        :   Pointer to the allocated buffer if there is available
*                     memory in the system, else NAT_NULL with ecode as
*                     NAT_ERR_MEM_ALLOC_FAILURE.
*
 *************************************************************************/

nat_void_t*
nat_port_static_memget(
        IN  nat_uint32_t          size,
        OUT nat_error_t    *p_ecode )
{
    nat_void_t*       p_buf = NAT_NULL;

    p_buf = (nat_void_t*)malloc(size);


    if(p_buf == (nat_void_t*) NAT_NULL)
    {
         *p_ecode = NAT_ERR_MEM_ALLOC_FAILURE;
    }
    else
    {
        nat_port_memset(p_buf,0,size);
    }


    return p_buf;
} /* End of nat_port_static_memget */


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_memset
*
*  DESCRIPTION    :   Utility function for setting the buffer to zero
*
*  RETURNS        :  Void
*
 *************************************************************************/
nat_void_t nat_port_memset(
        IN  nat_void_t       *p_buf,
        IN  nat_uint8_t      ch,
        IN  nat_uint32_t     n)
{
    nat_uint32_t     count = 0;
    nat_uint8_t      *ptr = (nat_uint8_t*)p_buf;

    for(; count < n; count++ )
    {
        ptr[count] = ch;
    }
}

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_memcpy
*
*  DESCRIPTION    :   Utility function for copying memory block.
*
*  RETURNS        :   Address of the destination memory location 
*
 *************************************************************************/
nat_void_t
nat_port_memcpy (
        OUT nat_void_t   *p_dest,
        IN  nat_void_t   *p_src,
        IN  nat_uint32_t count)
{
    nat_uint32_t num_of_bytes = count;
    nat_uint8_t *p_tmp = (nat_uint8_t *)p_dest, 
    *p_s = (nat_uint8_t *)p_src;

    while (num_of_bytes--)
        *p_tmp++ = *p_s++;
}

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_static_memfree
*
*  DESCRIPTION    :   This function is called to free static buffers
*                     in the system.
*
*
*  RETURNS        :   NAT_RETURN_SUCCESS/NAT_RETURN_FAILURE
*
 *************************************************************************/

nat_return_t
nat_port_static_memfree(
        IN  nat_void_t     *p_buf,
        OUT nat_error_t    *p_ecode )
{
    if ( NAT_NULL != p_buf)
    {
        free ((nat_void_t *)p_buf);
        p_buf = NAT_NULL;
        return NAT_RETURN_SUCCESS;
    }
    else
    {
         *p_ecode = NAT_ERR_MEM_FREE_FAILURE;
        return NAT_RETURN_FAILURE;
    }

} /* End of nat_static_memfree */

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strcmp
*
*  DESCRIPTION    :   Utility function for string comparison.
*
*  RETURNS        :  0 if strings match, non zero if they don't
*
 *************************************************************************/
nat_int32_t nat_port_strcmp(
        IN nat_uint8_t   *p_str1,
        IN  nat_uint8_t   *p_str2)
{
    for(; *p_str1 == *p_str2; p_str1++, p_str2++ )
        if( *p_str1 == '\0' )
            return 0;
    return (*p_str1-*p_str2);
}


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strcpy
*
*  DESCRIPTION    :   Utility function for string copy.
*
*  RETURNS        :  Void
*
 *************************************************************************/
nat_void_t nat_port_strcpy(
        INOUT nat_uint8_t   *p_dest,
        IN  nat_uint8_t   *p_src)
{
    while ((*p_dest++ = *p_src++) != '\0');
}

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strlen
*
*  DESCRIPTION    :   Utility function for calculating string length.
*
*  RETURNS        :   A U32 holding value of the length of the string  
*
 *************************************************************************/
nat_uint32_t
nat_port_strlen (IN nat_uint8_t    *p_s)
{
    const nat_uint8_t    *p_sc;

    for(p_sc = p_s; *p_sc != '\0'; ++p_sc);

    return p_sc - p_s;
}


/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strmatch
*
*  DESCRIPTION    :   Utility function for string comparison.
*                     It checks if str1 comes at the start of str2
*                     This is slightly different than strcmp. 
*                     It lets str2 to be longer than str1.
*
*  RETURNS        :   0 if strings match, non zero if they don't
*
 *************************************************************************/
nat_int32_t nat_port_strmatch(
        IN nat_uint8_t   *p_str1,
        IN nat_uint8_t   *p_str2)
{
    for(; *p_str1 == *p_str2; p_str1++, p_str2++ )
        if( *p_str1 == '\0' )
            return 0;
	
	if( *p_str1 == '\0' ) 
		return 0;
	else
        return (*p_str1-*p_str2);
}




/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_realloc

*
*  DESCRIPTION    :   reallocates memory
*
*  RETURNS        :   
*
*************************************************************************/

nat_void_t* 
nat_port_realloc(
        INOUT  	nat_void_t           	*p_port_info,
        IN  	nat_uint32_t		size
        )
{

	return realloc(p_port_info,size);

}




/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strcat
*
*  DESCRIPTION    :   Utility function for string cat.
*
*  RETURNS        :  Void
*
 *************************************************************************/
nat_void_t nat_port_strcat(
        INOUT nat_uint8_t   *p_dest,
        IN  nat_uint8_t   *p_src)
{
    while(*p_dest)
        p_dest++;

    while((*p_dest++ = *p_src++) != '\0');
}

/*************************************************************************
*
*  FUNCTION NAME  :   nat_port_strdup
*
*                     Duplicates the the string 
*Uses:
*
*Exceptions:
*
*******************************************************************************/

nat_uint8_t*  nat_port_strdup (
        IN nat_uint8_t* p_string
        )
{
        nat_uint8_t* p_memory=NAT_NULL ;
		nat_return_t  ret_val = NAT_RETURN_SUCCESS;
		nat_error_t   ecode;
        if (!p_string)
                return(NAT_NULL);

		p_memory = nat_port_static_memget(nat_port_strlen(p_string)+1, &ecode);
        if (NAT_NULL != p_memory)
		{
          nat_port_strcpy(p_memory,p_string);
		  return p_memory;
		}
        ret_val = ret_val;
        return(NAT_NULL);
}
