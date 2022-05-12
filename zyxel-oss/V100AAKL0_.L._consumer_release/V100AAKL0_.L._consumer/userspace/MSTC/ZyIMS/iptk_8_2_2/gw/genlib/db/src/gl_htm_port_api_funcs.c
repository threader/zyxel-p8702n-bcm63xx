
/*****************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - Hash table 
*                     implementation.  
******************************************************************************
*
*    FILENAME    :    gl_htm_port_api_funcs.c
*
*    DESCRIPTION :    This file contains the constants and function 
*                     declarations of hash table sub-module of the generic 
*                     library implemntation. The functions include those for 
*                     creation of hash table and operations such as addition,
*                     subtraction of keys/elements etc. (source file)
*
* DATE             NAME          REFERENCE        REASON
*-------------  ---------------  ---------------  ----------------------
*
* Oct'99        Kapil Nayar      HSS:40600026    Generic Libraries
*                                HSS:40600036
*                                 (Gateways)
* 04/10/2001    Ravi Jindal      SPR-2751        Remove warning with 
*                                                gcc,cc,CC on Solaris
* 18/10/2001    Ravi Jindal      SPR-2788        Packaging of Genlib-Code.
* 19/10/2001    Ravi Jindal      SPR-2802        Adding Pre-ANSI-C features.
* 30/10/2001    Ravi Jindal      SPR-2744        Adding de-init functions.
* 17/11/2001    Ravi Jindal      SPR-3107        Incoprate the defects and 
*                                                change cosmmetics of file.
*
* 05/04/2002    Ravi Jindal      SPR-3414        Enhancements for Rel-1.0.
*
* 10/09/2002    Ravi Jindal      SPR-4841        To change the reserve memory 
*                                                function prototype for 
*                                                user_mem_info.
*
*COPYRIGHT , Aricent, 2006
*****************************************************************************/


 #include "gl_htm.h"
/*
 * As in default implementation GENLIB provided Memory Manager is used,
 * So Header file is included (if other memory mgt. is used no need to include 
 * the file.
 */
#ifdef GENLIB_MEMORY
#include "gl_mm_interfaces.h"
#endif

/*
 * User_mem_info is used as pool_id of the common pool.
 */

/*****************************************************************************
* Function name    : gl_htm_reserve_memory
* Arguments        : gw_U32bit          user_id,
*                    gw_U32bit          *p_user_mem_info,
*                    gw_U32bit          buf_qty,
*                    gw_U32bit          buf_size,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To reserve memory for Hash Table Nodes. As their can be 
*                    any number of keys in a bucket.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_reserve_memory

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gw_U32bit                 *p_user_mem_info,
      gw_U32bit                 buf_qty,
      gw_U32bit                 buf_size,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_user_mem_info, buf_qty, buf_size, p_ecode )
      gw_U32bit                 user_id;
      gw_U32bit                 *p_user_mem_info;
      gw_U32bit                 buf_qty;
      gw_U32bit                 buf_size;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    _STATIC_ gw_boolean is_init = GL_FALSE;

    /* At present No Seprate Pool is creatred and it is assumed that
     * user have already created the pool for the memory requirements
     * of Hash-tables.
     * So if required then the implemenmtation Commented can be used for 
     * creation of Common Pools for Hash-table Memory requirments.
     */
    /*
     * In present Implementation For Memory Requirements 
     * GENLIB Supported Memory Manager is used.
     * A common memory pool is created.
     */
     gl_pool_id_t                pool_id;
     gl_return_t                 ret_val; 
#ifndef GENLIB_MEMORY
	if ( GW_NULL == p_ecode)
	{
	}
	if ( 0 == buf_size)
	{
	}
	if ( 0 == buf_qty)
	{
	}
	pool_id = 0;
	ret_val = GL_SUCCESS;


#endif         
     if(GL_FALSE == is_init)
     {
        /*
         * As Memory Manager is used for memory requirements.
         * If memory init is done previous to this remove the init call.
         */
#ifdef GENLIB_MEMORY
        gl_mm_init();
#endif
        is_init = GL_TRUE;

     }
     
    /*
     * As in Common Pool creation user_id is not used,
     * So to remove warning.
     */ 
    (void)user_id;
#ifdef GENLIB_MEMORY    
     ret_val = gl_mm_common_pool_create (
                                 &pool_id,
                                 buf_qty,
                                 buf_size,
                                 p_ecode);
#endif
      
    /*
     * Store the returned value of Pool-id in the feild user_mem_info.
     * So that same pool can be referenced later.
     */
    *p_user_mem_info = (gw_U32bit)pool_id;

    return (ret_val);
}

    
/*****************************************************************************
* Function name    : gl_htm_get_memory_buffer
* Arguments        : gw_U32bit          user_id,
*                    gl_U32bit          user_mem_info,
*                    gw_U32bit          buf_size,
*                    gl_error_t         *p_ecode
* Return Value     : gl_pvoid_t
* Functionality    : To get the memory buffer for memory requirements of key
*                    node. As addition of a key in Hash-table dynamic memory
*                    is required for the node specific to key to be added.
*                    The key node comntain pointer to key, pointer to element,
*                    pointer to next node, pointer to previous node.
* Functions called :
*****************************************************************************/
gl_pvoid_t
gl_htm_get_memory_buffer

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gw_U32bit                 user_mem_info,
      gw_U32bit                 buf_size,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, user_mem_info, buf_size, p_ecode )
      gw_U32bit                 user_id;
      gw_U32bit                 user_mem_info;
      gw_U32bit                 buf_size;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_pvoid_t                  p_ret_val;
    /*
     * In present Implementation For Memory Requirements 
     * GENLIB Supported Memory Manager is used.
     * A common memory pool buffer is allocated from a already created
     * common memory pool.
     */
    
    /*
     * As in Get buffer from Common Memory Pool, pool_id is not used
     * So to remove warning.
     */ 
    (void)user_mem_info;
#ifdef GENLIB_MEMORY     
    p_ret_val = gl_mm_get_buffer (
                                 user_id,
                                 buf_size,
                                 p_ecode);
#else
	if ( GW_NULL == p_ecode)
	{
	}
	if ( 0 == user_id)
	{
	}
	p_ret_val = malloc(buf_size);

#endif    
    return(p_ret_val);
    
}

    

/*****************************************************************************
* Function name    : gl_htm_release_memory_buffer
* Arguments        : gw_U32bit          user_id,
*                    gl_U32bit          user_mem_info,
*                    gl-pvoid_t         p_buffer,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To release the memory buffer, allocated for memory 
*                    requirements of key node. 
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_release_memory_buffer

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gw_U32bit                 user_mem_info,
      gl_pvoid_t                p_buffer,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, user_mem_info, p_buffer, p_ecode )
      gw_U32bit                 user_id;
      gw_U32bit                 user_mem_info;
      gl_pvoid_t                p_buffer;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_return_t                 ret_val;
    /*
     * In present Implementation For Memory Requirements 
     * GENLIB Supported Memory Manager is used.
     * As common memory pool buffer is allocated from a common memory pool.
     * So release the buffer.
     */
    
    /*
     * As in Release buffer from Common Memory Pool, pool_id is not used
     * So to remove warning.
     */ 
    (void)user_mem_info;
#ifdef GENLIB_MEMORY     
    ret_val = gl_mm_release_buffer (
                                 user_id,
                                 p_buffer,
                                 p_ecode);
#else
	if ( GW_NULL == p_ecode)
	{
	}
	if ( 0 == user_id)
	{
	}
	free(p_buffer);
	ret_val = GL_SUCCESS;	
#endif
    
    return (ret_val);
    
}



/*****************************************************************************
* Function name    : gl_htm_free_memory
* Arguments        : gw_U32bit          user_id,
*                    gl_U32bit          user_mem_info,
*                    gw_U32bit          buf_qty,
*                    gw_U32bit          buf_size,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To free the memory reserved for Hash Table Nodes.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_free_memory

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gw_U32bit                 user_mem_info,
      gw_U32bit                 buf_qty,
      gw_U32bit                 buf_size,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, user_mem_info, buf_qty, buf_size, p_ecode )
      gw_U32bit                 user_id;
      gw_U32bit                 user_mem_info;
      gw_U32bit                 buf_qty;
      gw_U32bit                 buf_size;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    /*
     * In present Implementation For Memory Requirements 
     * GENLIB Supported Memory Manager is used.
     * A common memory pool is to be deleted at deletion of Hash-Table.
     */ 
     
    /*
     * As in current implementation buffer of a Common Pool can be shared 
     * across different modules.
     * So we try to delete the pool created for the hash-table nodes.
     * If the buffers are allocated for some other modules then the delete 
     * pool will fail but we should return Success 
     */
    
    /* As current Common poool delete API would not satisfy the delete of a 
     * specific Common Pool and their can be multiple pools of given size.
     * So donot delete the pool and 
     * to remove warning.
     */ 
    (void)user_id;
    (void)user_mem_info;
    (void)buf_qty;
    (void)buf_size;
    (void)*p_ecode;
    
    return GL_SUCCESS;
    
}

    
