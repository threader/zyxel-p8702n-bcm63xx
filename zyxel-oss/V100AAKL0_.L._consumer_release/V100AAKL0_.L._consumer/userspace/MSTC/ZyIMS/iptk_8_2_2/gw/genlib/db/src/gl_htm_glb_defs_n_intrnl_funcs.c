/*****************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - Hash table 
*                     implementation.  
******************************************************************************
*
*    FILENAME    :    gl_htm_glb_defs_n_intrnl_funcs.c
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
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_htm.h"

/*
 * Array of hash table structures where
 * the hash tables are added at runtime
 */
gl_htm_table_t                  *g_a_p_hashtable[GL_HTM_MAX_HASH_TABLE];


gl_boolean                      htm_mgr_init_flag = GL_FALSE;



/*****************************************************************************
* Function name    : gl_htm_def_numeric_key_hash_fn
* Arguments        : gl_hash_key_ut     *p_key,
*                    gl_pvoid_t         p_hash_arg
* Return Value     : gw_U32bit
* Functionality    : Default Hashing Function for Numeric keys.
* Functions called :
*****************************************************************************/
gw_U32bit
gl_htm_def_numeric_key_hash_fn

#ifdef _ANSI_C_
    ( gl_hash_key_ut            *p_key,
      gl_pvoid_t                p_hash_arg
    )
#else
    ( p_key, p_hash_arg )
      gl_hash_key_ut            *p_key;
      gl_pvoid_t                p_hash_arg;
#endif

/******************************************************************************/
{
    /*
     * To remove warning.
     */
    p_hash_arg = p_hash_arg; 
    /*
     * As this function expects numeric keys,
     * So numeric_id part of key is used in this function.
     * As keys must be of type Integer for this function.
     */ 
    
    /*
     * Numeric key hahing function.
     * At present none of the hashing ALgo is used for numeric key,
     * Modulo is taken in the actual function after return of hashing
     * value from hash-function for the key.
     * So simply return back the numeric key.
     */
    
    return (p_key->numeric_id);
}




/*****************************************************************************
* Function name    : gl_htm_def_numeric_key_cmp_fn
* Arguments        : gl_hash_key_ut     *p_key_to_be_compared,
*                    gl_hash_key_ut     *p_node_key,
*                    gw_U32bit          key_hash_val,
*                    gw_U32bit          node_key_hash_val
* Return Value     : gl_return_t
* Functionality    : Default Key comparision Function for Numeric keys.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_def_numeric_key_cmp_fn

#ifdef _ANSI_C_
    ( gl_hash_key_ut            *p_key_to_be_compared,
      gl_hash_key_ut            *p_node_key,
      gw_U32bit                 key_hash_val,
      gw_U32bit                 node_key_hash_val
    )
#else
    ( p_key_to_be_compared, p_node_key, key_hash_val, node_key_hash_val )
      gl_hash_key_ut            *p_key_to_be_compared;
      gl_hash_key_ut            *p_node_key;
      gw_U32bit                 key_hash_val;
      gw_U32bit                 node_key_hash_val;
#endif

/******************************************************************************/
{
    /*
     * To remove Warnings.
     */
    key_hash_val = key_hash_val;
    node_key_hash_val = node_key_hash_val;

    /*
     * As the keys must be of type Integer for this function.
     */
    if(p_key_to_be_compared->numeric_id == p_node_key->numeric_id)
    {
        return GL_SUCCESS;
    }

    return GL_FAILURE;
}



/*****************************************************************************
* Function name    : gl_htm_def_string_key_hash_fn
* Arguments        : gl_hash_key_ut     *p_key,
*                    gl_pvoid_t         p_hash_arg
* Return Value     : gw_U32bit
* Functionality    : Default Hashing Function for Null terminated String keys.
* Functions called :
*****************************************************************************/
gw_U32bit
gl_htm_def_string_key_hash_fn

#ifdef _ANSI_C_
    ( gl_hash_key_ut            *p_key,
      gl_pvoid_t                p_hash_arg
    )
#else
    ( p_key, p_hash_arg )
      gl_hash_key_ut            *p_key;
      gl_pvoid_t                p_hash_arg;
#endif

/******************************************************************************/
{
    /*
     * As the keys must be of type String for this function.
     */ 
    gw_U8bit       *p_string_key = (gw_U8bit *)(p_key->p_user_def_id);
    gw_U8bit       *p_temp;
    gw_U32bit      hash_value = 0;
    gw_U32bit      temp_value;

    p_hash_arg = p_hash_arg; 
    
    /*
     * String key Hashing Function.
     */ 
    for(p_temp = p_string_key; *p_temp != '\0'; p_temp += 1) 
    {
        hash_value = ( hash_value << 4 ) + *p_temp;
       
        temp_value = hash_value & 0xf0000000;
        
        if(temp_value)
        {
            hash_value = hash_value ^ (temp_value >> 24);
            hash_value = hash_value ^ temp_value;
        }
    }

    return hash_value;
}




/*****************************************************************************
* Function name    : gl_htm_def_string_key_cmp_fn
* Arguments        : gl_hash_key_ut     *p_key_to_be_compared,
*                    gl_hash_key_ut     *p_node_key,
*                    gw_U32bit          key_hash_val,
*                    gw_U32bit          node_key_hash_val
* Return Value     : gl_return_t
* Functionality    : Default Key comparision Function for Null terminated keys.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_def_string_key_cmp_fn

#ifdef _ANSI_C_
    ( gl_hash_key_ut            *p_key_to_be_compared,
      gl_hash_key_ut            *p_node_key,
      gw_U32bit                 key_hash_val,
      gw_U32bit                 node_key_hash_val
    )
#else
    ( p_key_to_be_compared, p_node_key, key_hash_val, node_key_hash_val )
      gl_hash_key_ut            *p_key_to_be_compared;
      gl_hash_key_ut            *p_node_key;
      gw_U32bit                 key_hash_val;
      gw_U32bit                 node_key_hash_val;
#endif

/******************************************************************************/
{
    /*
     * As the keys must be of type null terminated String for this function.
     */ 
    gw_U8bit    *p_key1 = (gw_U8bit *)(p_key_to_be_compared->p_user_def_id);
    gw_U8bit    *p_key2 = (gw_U8bit *)(p_node_key->p_user_def_id);

    
    if(key_hash_val != node_key_hash_val)
    {
        return GL_FAILURE;
    }
    
    while((*p_key1 != '\0') && (*p_key2 != '\0'))
    {
        if(*p_key1 != *p_key2)
        {
           break; 
        }
        p_key1 += 1;
        p_key2 += 1;
    }
    
    if(*p_key1 == *p_key2)
    {
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}

    
/*****************************************************************************
* Function name    : htm_allocate_key_node
* Arguments        : gl_hash_table_t    hash_table_id
*                    gl_htm_key_node_t  **p_p_key_node,  
*                    gl_hash_key_ut     *p_key,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To allocate memory for the key node and set default 
*                    values tothe pointer to next key node and previous key 
*                    node.
* Functions called :
*****************************************************************************/
gl_return_t
htm_allocate_key_node

#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id,
      gl_htm_key_node_t         **p_p_key_node,
      gl_hash_key_ut            *p_key,
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_p_key_node, p_key, p_ecode )
      gl_hash_table_t           hash_table_id;
      gl_htm_key_node_t         **p_p_key_node;
      gl_hash_key_ut            *p_key;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    /*
     * In present Implementation For Memory Requirements
     * Use the porting functions.
     */
    
    *p_p_key_node = (gl_htm_key_node_t *)gl_htm_get_memory_buffer( 
            g_a_p_hashtable[hash_table_id]->owner_user_id,
            g_a_p_hashtable[hash_table_id]->user_mem_info,
            sizeof(gl_htm_key_node_t),
            p_ecode);
    
    if(GL_NULL == *p_p_key_node)
    {
        *p_ecode = GL_HTM_ERR_GET_MEMORY_FAILS;
        return GL_FAILURE;
    }
    
    if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE != g_a_p_hashtable[hash_table_id]->
        key_type)
    {
        /*
         * As key type is other than Numeric, then to only pointer to key 
         * is stored in the hash-table.
         * Memory for the key is to be allocated by user.
         */ 
        (*p_p_key_node)->key.p_user_def_id = p_key->p_user_def_id;
    }
    else
    {
        /*
         * As key type is numeric, So store the valus of key in the interger
         * part of union to key. 
         */ 
        (*p_p_key_node)->key.numeric_id = p_key->numeric_id;
    }
    
    (*p_p_key_node)->p_next = GL_NULL;
    (*p_p_key_node)->p_prev = GL_NULL;
    (*p_p_key_node)->p_elem = GL_NULL;
    
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
    (*p_p_key_node)->ref_count = 0;
#endif
    return GL_SUCCESS;
    
}



/*****************************************************************************
* Function name    : htm_remove_key_node
* Arguments        : gl_hash_table_t        hash_table_id,
*                    gl_htm_bucket_array_t  *p_bucket,
*                    gl_htm_key_node_t      *p_temp_node,
*                    gl_error_t             *p_ecode
* Return Value     : gl_return_t
* Functionality    : To remove Key node from the hash table and release the
*                    memory allocated for key node.
* Functions called :
*****************************************************************************/
gl_return_t
htm_remove_key_node

#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id,
      gl_htm_bucket_array_t     *p_bucket,
      gl_htm_key_node_t         *p_temp_node,   
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_bucket, p_temp_node, p_ecode )
      gl_hash_table_t           hash_table_id;
      gl_htm_bucket_array_t     *p_bucket;
      gl_htm_key_node_t         *p_temp_node;  
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    if(1 == p_bucket->key_qty)
    {
        p_bucket->p_first = p_bucket->p_last =  GL_NULL;
    }
    else if(p_bucket->p_first == p_temp_node)
    {
        p_bucket->p_first->p_next->p_prev = GL_NULL;
        p_bucket->p_first = p_bucket->p_first->p_next;
    }
    else if(p_bucket->p_last == p_temp_node)
    {
        p_bucket->p_last->p_prev->p_next = GL_NULL;
        p_bucket->p_last = p_bucket->p_last->p_prev;
    }
    else
    {
        p_temp_node->p_prev->p_next = p_temp_node->p_next;
        p_temp_node->p_next->p_prev = p_temp_node->p_prev;
    }
    p_bucket->key_qty -= 1;
        
    /*
     * In present Implementation For Memory Requirements
     * Use the porting functions.
     * Free the memory allocate for the hash-table node.
     */ 
    
    if(GL_FAILURE == gl_htm_release_memory_buffer( 
            g_a_p_hashtable[hash_table_id]->owner_user_id,
            g_a_p_hashtable[hash_table_id]->user_mem_info,
            p_temp_node,
            p_ecode) )
    {
        *p_ecode = GL_HTM_ERR_RELEASE_MEMORY_FAILS;
        return GL_FAILURE;
    }
    
    return GL_SUCCESS;
}
