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
* 28/10/2002    Ravi Jindal      SPR-5005        Proper handling of check 
*                                                pointing node removel in
*                                                get_key_list function.
*
* 10/09/2003    Shobhit Bansal   SPR-6364        Changed the varibale type
*                                                to gw_U32bit
*
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_htm.h"


/*****************************************************************************
* Function name    : gl_htm_add_key_to_hash_table
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gl_hash_key_ut     *p_key,
*                    gl_pvoid_t         p_element,
*                    gl_pvoid_t         *p_p_key_node_handle,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Adds an element at the key in the hash table and 
*                    increments the element count.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_add_key_to_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key,
      gl_pvoid_t                p_element,
      gl_pvoid_t                *p_p_key_node_handle,
      gl_error_t*               p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_element, 
      p_p_key_node_handle, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key;
      gl_pvoid_t                p_element; 
      gl_pvoid_t                *p_p_key_node_handle;
      gl_error_t*               p_ecode;
#endif
    
/****************************************************************************/
{

    gw_U32bit                   bucket_index;
    gw_U32bit                   key_hash_val;
    gl_htm_bucket_array_t       *p_bucket;
    gl_htm_key_node_t           *p_key_node;
    gl_htm_key_node_t           *p_temp_node;
#ifdef GL_STATS_ENABLED
    gw_U32bit                   i;
    gw_U32bit                   total_key = 0;
#endif /* Endif for GL_STATS_ENABLED */

    GL_INC_HTM_G_ADD_KEY_RQ()
        
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
    if(GL_NULL == p_key)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_KEY_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    key_hash_val = g_a_p_hashtable[hash_table_id]->p_hash_fn(p_key, 
        g_a_p_hashtable[hash_table_id]->p_hash_arg);

    bucket_index = (key_hash_val % g_a_p_hashtable[hash_table_id]->bucket_qty);
    
    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + bucket_index;
    
    GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
    if(GL_NULL == p_bucket->p_first)
    {
        if(GL_SUCCESS == htm_allocate_key_node(hash_table_id, &p_key_node, 
            p_key, p_ecode))
        {
            p_key_node->p_elem = p_element;
            p_key_node->key_hash_val = key_hash_val;
            
            p_bucket->key_qty = 1;
            p_bucket->p_first = p_bucket->p_last = p_key_node;

            /*
             * Return the address of key node as handle to key node
             * So that key can be directly accessed based on returned
             * handle to key node.
             */ 
            *p_p_key_node_handle = (gl_pvoid_t)p_key_node;
    
#ifdef GL_STATS_ENABLED
            for(i=0; i<g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
            {
                total_key += (g_a_p_hashtable[hash_table_id]->a_bucket_array 
                    + i)->key_qty;
            }

            GL_HTM_C_PEAK_KEY_INSRT(hash_table_id, total_key)
#endif /* Endif for GL_STATS_ENABLED */
            
            GL_HTM_C_MAX_KEY_AY_BKT(hash_table_id, p_bucket->key_qty)
            GL_HTM_C_MIN_KEY_AY_BKT(hash_table_id, p_bucket->key_qty)

            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
            return GL_SUCCESS;
        }
        else
        {
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

            GL_INC_HTM_C_ADD_KEY_FAIL(hash_table_id)
    
            return GL_FAILURE;
        }
    }
    else
    {
        for(p_temp_node = p_bucket->p_first; 
            GL_NULL != p_temp_node;
            p_temp_node = p_temp_node->p_next)
        {
            if(GL_SUCCESS == g_a_p_hashtable[hash_table_id]->p_key_cmp_fn(
                p_key, &(p_temp_node->key), key_hash_val, 
                p_temp_node->key_hash_val))
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
                
                GL_INC_HTM_G_DUP_KEY_INSRT()

                *p_ecode = GL_HTM_ERR_DUPLICATE_KEY;
                return GL_FAILURE;                
            }
        }
        
        if(GL_SUCCESS == htm_allocate_key_node(hash_table_id, &p_key_node, 
            p_key, p_ecode))
        {
            p_key_node->p_elem = p_element;
            p_key_node->key_hash_val = key_hash_val;
            
            p_key_node->p_prev = p_bucket->p_last;
            p_bucket->p_last->p_next = p_key_node;
            p_bucket->p_last = p_key_node;
    
            p_bucket->key_qty += 1;
            
            /*
             * Return the address of key node as handle to key node
             * So that key can be directly accessed based on returned
             * handle to key node.
             */ 
            *p_p_key_node_handle = (gl_pvoid_t)p_key_node;
    
#ifdef GL_STATS_ENABLED
            for(i=0; i<g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
            {
                total_key += (g_a_p_hashtable[hash_table_id]->a_bucket_array 
                    + i)->key_qty;
            }

            GL_HTM_C_PEAK_KEY_INSRT(hash_table_id, total_key)
#endif /* Endif for GL_STATS_ENABLED */

            GL_HTM_C_MAX_KEY_AY_BKT(hash_table_id, p_bucket->key_qty)
            GL_HTM_C_MIN_KEY_AY_BKT(hash_table_id, p_bucket->key_qty)

            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
            return GL_SUCCESS;
        }
        else
        {
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
            
            GL_INC_HTM_C_ADD_KEY_FAIL(hash_table_id)
    
            return GL_FAILURE;
        }
    }
}




/*****************************************************************************
* Function name    : gl_htm_get_key_ref_from_hash_table
* Arguments        : gw_U32bit          user_id, 
*                    gl_hash_table_t    hash_table_id, 
*                    gl_hash_key_ut     *p_key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the element at the key in the hash table.
* Functions called :
******************************************************************************/
gl_return_t 
gl_htm_get_key_ref_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key, 
      gl_pvoid_t                *p_p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_p_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key; 
      gl_pvoid_t                *p_p_element; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gw_U32bit                   key_hash_val;
    gl_htm_bucket_array_t       *p_bucket;
    gl_htm_key_node_t           *p_temp_node;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    key_hash_val = g_a_p_hashtable[hash_table_id]->p_hash_fn(p_key,
        g_a_p_hashtable[hash_table_id]->p_hash_arg);

    bucket_index = (key_hash_val % g_a_p_hashtable[hash_table_id]->bucket_qty);

    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + bucket_index;

    GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
    if(GL_NULL == p_bucket->key_qty)
    {
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        GL_INC_HTM_G_KEY_NOT_FOUND()
        
        *p_ecode = GL_HTM_ERR_NO_ELEMENT;
        return GL_FAILURE;
    }
    
    for(p_temp_node = p_bucket->p_first; GL_NULL != p_temp_node;
        p_temp_node = p_temp_node->p_next)
    {
        if(GL_SUCCESS == g_a_p_hashtable[hash_table_id]->p_key_cmp_fn( p_key, 
            &(p_temp_node->key), key_hash_val, p_temp_node->key_hash_val))
        {
            *p_p_element = p_temp_node->p_elem;
            
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
            p_temp_node->ref_count += 1;
#endif
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

            return GL_SUCCESS;
        }
    }
    
    GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
    GL_INC_HTM_G_KEY_NOT_FOUND()
        
    *p_ecode = GL_HTM_ERR_KEY_NOT_FOUND;
    return GL_FAILURE;
}





/*****************************************************************************
* Function name    : gl_htm_get_key_ref_by_key_node_handle
* Arguments        : gw_U32bit          user_id, 
*                    gl_hash_table_t    hash_table_id,
*                    gl_pvoid_t         p_key_node_handle,
*                    gl_hash_key_ut     *p_key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the element at the key in the hash table.
* Functions called :
******************************************************************************/
gl_return_t 
gl_htm_get_key_ref_by_key_node_handle

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_pvoid_t                p_key_node_handle,
      gl_hash_key_ut            *p_key, 
      gl_pvoid_t                *p_p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key_node_handle, p_key, p_p_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_pvoid_t                p_key_node_handle;
      gl_hash_key_ut            *p_key; 
      gl_pvoid_t                *p_p_element; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gl_htm_key_node_t           *p_temp_node;

    /*
     * The input paramter p_key_node_handle must be a valid
     * handle to key node. 
     * Hash Table Manager can not provide any check to this value.
     */
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(GL_NULL == p_key_node_handle)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_NULL_KEY_NODE_HANDLE_PTR;
        return GL_FAILURE;
    }
    p_temp_node = (gl_htm_key_node_t *)p_key_node_handle;
    
    *p_p_element = p_temp_node->p_elem;

#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
    p_temp_node->ref_count += 1;
#endif
    
    if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable[hash_table_id]->
            key_type)
    {
        p_key->numeric_id = p_temp_node->key.numeric_id;
    }
    else
    {
        p_key->p_user_def_id = p_temp_node->key.p_user_def_id;
    }
    
    return GL_SUCCESS;
}



/*
 * As this API is usefull only if Key Reference count is used.
 */ 
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT

/*****************************************************************************
* Function name    : gl_htm_remove_key_ref_from_hash_table
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gl_hash_key_ut     *p_key, 
*                    gl_pvoid_t         *p_p_key_buf, 
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Remove the key Reference from the hash table by
*                    decrementing Key Reference count by 1.
*                    If Key Reference Count is equal to 0. Remove the key from
*                    Hash-table and returned respectiove the stored pointer,
*                    p_p_element.
*                    If Key Reference Count is more than 0, then return value 
*                    of p_p_element is GL_NULL.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_remove_key_ref_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key, 
      gl_pvoid_t                *p_p_key_buf,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_p_key_buf, p_p_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key; 
      gl_pvoid_t                *p_p_key_buf;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gw_U32bit                   key_hash_val;
    gl_htm_bucket_array_t       *p_bucket;
    gl_htm_key_node_t           *p_temp_node;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    key_hash_val = g_a_p_hashtable[hash_table_id]->p_hash_fn(p_key, 
        g_a_p_hashtable[hash_table_id]->p_hash_arg);

    bucket_index = (key_hash_val % g_a_p_hashtable[hash_table_id]->bucket_qty);

    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + bucket_index;

    GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    if(GL_NULL == p_bucket->key_qty)
    {
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        GL_INC_HTM_G_KEY_NOT_FOUND()
        
        *p_ecode = GL_HTM_ERR_NO_ELEMENT;
        return GL_FAILURE;
    }

    for(p_temp_node = p_bucket->p_first; GL_NULL != p_temp_node;
        p_temp_node = p_temp_node->p_next)
    {
        if(GL_SUCCESS == g_a_p_hashtable[hash_table_id]->p_key_cmp_fn( p_key,
            &(p_temp_node->key), key_hash_val, p_temp_node->key_hash_val))
        {
            if(GL_NULL != p_temp_node->ref_count)
            {
                p_temp_node->ref_count -= 1;
            }
            
            if(GL_NULL != p_temp_node->ref_count)
            {
                *p_p_element = GL_NULL;
                
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, 
                    GL_FAILURE);

                return GL_SUCCESS;
            }
            *p_p_element = p_temp_node->p_elem;
            
            if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                [hash_table_id]->key_type)
            {
                *p_p_key_buf =  GL_NULL;
            }
            else
            {
                *p_p_key_buf = p_temp_node->key.p_user_def_id;
            }
    
            if(GL_FAILURE == htm_remove_key_node(hash_table_id, p_bucket,
                p_temp_node, p_ecode))
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                        GL_FAILURE);

                return GL_FAILURE;
            }

            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
            
            GL_HTM_C_MIN_KEY_AY_BKT(hash_table_id, p_bucket->key_qty)

            return GL_SUCCESS;
        }
    }

    GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    GL_INC_HTM_G_KEY_NOT_FOUND()
        
    *p_ecode = GL_HTM_ERR_KEY_NOT_FOUND;
    return GL_FAILURE;
}
    
#endif /* Endif for the flag GL_HTM_ENABLE_KEY_REF_COUNT */  




/*****************************************************************************
* Function name    : gl_htm_replace_key_elem_in_hash_table
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gl_hash_key_ut     *p_key, 
*                    gl_pvoid_t         p_new_element,
*                    gl_pvoid_t         *p_p_old_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Replaces the element at the key in the hash table.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_replace_key_elem_in_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key, 
      gl_pvoid_t                p_new_element,
      gl_pvoid_t                *p_p_old_element, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_new_element, p_p_old_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key; 
      gl_pvoid_t                p_new_element;
      gl_pvoid_t                *p_p_old_element; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gw_U32bit                   key_hash_val;
    gl_htm_bucket_array_t       *p_bucket;
    gl_htm_key_node_t           *p_temp_node;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    key_hash_val = g_a_p_hashtable[hash_table_id]->p_hash_fn(p_key, 
        g_a_p_hashtable[hash_table_id]->p_hash_arg);

    bucket_index = (key_hash_val % g_a_p_hashtable[hash_table_id]->bucket_qty);

    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + bucket_index;

    GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    if(GL_NULL == p_bucket->key_qty)
    {
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        GL_INC_HTM_G_KEY_NOT_FOUND()
        
        *p_ecode = GL_HTM_ERR_NO_ELEMENT;
        return GL_FAILURE;
    }

    for(p_temp_node = p_bucket->p_first; GL_NULL != p_temp_node;
        p_temp_node = p_temp_node->p_next)
    {
        if(GL_SUCCESS == g_a_p_hashtable[hash_table_id]->p_key_cmp_fn( p_key,
            &(p_temp_node->key), key_hash_val, p_temp_node->key_hash_val))
        {
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
            if(GL_NULL != p_temp_node->ref_count)
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, 
                        GL_FAILURE);

                *p_ecode = GL_HTM_ERR_KEY_REF_NOT_ZERO;
                return GL_FAILURE;
            }
#endif
            *p_p_old_element = p_temp_node->p_elem;
            p_temp_node->p_elem = p_new_element;

            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

            return GL_SUCCESS;
        }
    }

    GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    GL_INC_HTM_G_KEY_NOT_FOUND()
        
    *p_ecode = GL_HTM_ERR_KEY_NOT_FOUND;
    return GL_FAILURE;
}
    


/*****************************************************************************
* Function name    : gl_htm_remove_key_from_hash_table
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id, 
*                    gl_hash_key_ut     *p_key, 
*                    gl_pvoid_t         *p_p_key_buf, 
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes the element at the key in the hash table.
*                    This API does not consider the ref count of key. 
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_remove_key_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key, 
      gl_pvoid_t                *p_p_key_buf, 
      gl_pvoid_t                *p_p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_p_key_buf, p_p_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key; 
      gl_pvoid_t                *p_p_key_buf; 
      gl_pvoid_t                *p_p_element; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gw_U32bit                   key_hash_val;
    gl_htm_bucket_array_t       *p_bucket;
    gl_htm_key_node_t           *p_temp_node;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    key_hash_val = g_a_p_hashtable[hash_table_id]->p_hash_fn(p_key,
        g_a_p_hashtable[hash_table_id]->p_hash_arg);

    bucket_index = (key_hash_val % g_a_p_hashtable[hash_table_id]->bucket_qty);

    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + bucket_index;

    GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    if(GL_NULL == p_bucket->key_qty)
    {
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        GL_INC_HTM_G_KEY_NOT_FOUND()
        
        *p_ecode = GL_HTM_ERR_NO_ELEMENT;
        return GL_FAILURE;
    }

    for(p_temp_node = p_bucket->p_first; GL_NULL != p_temp_node;
        p_temp_node = p_temp_node->p_next)
    {
        if(GL_SUCCESS == g_a_p_hashtable[hash_table_id]->p_key_cmp_fn( p_key,
            &(p_temp_node->key), key_hash_val, p_temp_node->key_hash_val))
        {
            *p_p_element = p_temp_node->p_elem;
            
            if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                [hash_table_id]->key_type)
            {
                *p_p_key_buf =  GL_NULL;
            }
            else
            {
                *p_p_key_buf = p_temp_node->key.p_user_def_id;
            }
    
            if(GL_FAILURE == htm_remove_key_node(hash_table_id, p_bucket,
                p_temp_node, p_ecode))
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                        GL_FAILURE);

                return GL_FAILURE;
            }
            
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

            GL_HTM_C_MIN_KEY_AY_BKT(hash_table_id, p_bucket->key_qty)

            return GL_SUCCESS;
        }
    }
    
    GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    GL_INC_HTM_G_KEY_NOT_FOUND()
        
    *p_ecode = GL_HTM_ERR_KEY_NOT_FOUND;
    return GL_FAILURE;
}




/******************************************************************************
* Function name    : gl_htm_get_key_count_of_hash_table
* Arguments        : gl_hash_table_t    hash_table_id, 
*                    gw_U32bit		    *p_number_of_keys,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the count of the keys currently stored 
*                    in the hash table. 
* Functions called :
******************************************************************************/
gl_return_t
gl_htm_get_key_count_of_hash_table

#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id,
      gw_U32bit		            *p_number_of_keys,
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_number_of_keys, p_ecode )
      gl_hash_table_t           hash_table_id;
      gw_U32bit		            *p_number_of_keys;
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gl_htm_bucket_array_t       *p_bucket;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_HASH_TABLE_ID_DISABLE flag not defined.
     */
    p_ecode = p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    *p_number_of_keys = GL_NULL;
    
    for(bucket_index = 0; bucket_index < g_a_p_hashtable[hash_table_id]
        ->bucket_qty; bucket_index++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array +
            bucket_index;

        GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        *p_number_of_keys += p_bucket->key_qty;

        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    }
    
    return GL_SUCCESS;
}



/******************************************************************************
* Function name    : gl_htm_is_hash_table_empty
* Arguments        : gl_hash_table_t    hash_table_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_boolean
* Functionality    : Returns true only if the hash table contains zero 
*                    elements.
* Functions called :
******************************************************************************/
gl_boolean_t
gl_htm_is_hash_table_empty

#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id, 
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_ecode )
      gl_hash_table_t           hash_table_id; 
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gl_htm_bucket_array_t       *p_bucket;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FALSE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    for(bucket_index = 0; bucket_index < g_a_p_hashtable[hash_table_id]
        ->bucket_qty; bucket_index++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + 
            bucket_index;

        GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        if(GL_NULL != p_bucket->key_qty)
        {
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

            *p_ecode = GL_HTM_ERR_TABLE_NOT_EMPTY;
            return GL_FALSE;
        }

        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    }
    return GL_TRUE;
}



/******************************************************************************
* Function name    : gl_htm_get_first_key_ref_from_hash_table
* Arguments        : gw_U32bit          user_id, 
*                    gl_hash_table_t    hash_table_id, 
*                    gl_hash_key_ut     *p_key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the key and corresponding element from first 
*                    non-empty bucket of the hash-table.
* Functions called :
******************************************************************************/
gl_return_t
gl_htm_get_first_key_ref_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key, 
      gl_pvoid_t                *p_p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_p_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key; 
      gl_pvoid_t                *p_p_element; 
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gl_htm_bucket_array_t       *p_bucket;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */


    for(bucket_index = 0; bucket_index < g_a_p_hashtable[hash_table_id]->
            bucket_qty; bucket_index++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + 
            bucket_index;

        GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        if(GL_NULL != p_bucket->key_qty)
        {
            if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                    [hash_table_id]->key_type)
            {
                p_key->numeric_id = p_bucket->p_first->key.numeric_id;
            }
            else
            {
                p_key->p_user_def_id = p_bucket->p_first->key.p_user_def_id;
            }
    
            *p_p_element = p_bucket->p_first->p_elem;
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
            (p_bucket->p_first)->ref_count += 1;
#endif            
            
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
            
            return GL_SUCCESS;
        }

        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    }
    
    *p_ecode = GL_HTM_ERR_HASH_TABLE_EMPTY;
    return GL_FAILURE;
}




/******************************************************************************
* Function name    : gl_htm_get_next_key_ref_from_hash_table
* Arguments        : gw_U32bit          user_id, 
*                    gl_hash_table_t    hash_table_id, 
*                    gl_hash_key_ut     *p_key,
*                    gl_hash_key_ut     *p_next_key, 
*                    gl_pvoid_t         *p_p_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the key and corresponding element next to the given 
*                    key in the hash table (The next key may be in the 
*                    same bucket or in bucket after the p_key containing
*                    bucket).
* Functions called :
******************************************************************************/
gl_return_t 
gl_htm_get_next_key_ref_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key, 
      gl_hash_key_ut            *p_next_key,
      gl_pvoid_t                *p_p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_next_key, p_p_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key; 
      gl_hash_key_ut            *p_next_key;
      gl_pvoid_t                *p_p_element; 
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gw_U32bit                   key_hash_val;
    gl_htm_bucket_array_t       *p_bucket;
    gw_U32bit                   next_bucket_index;
    gl_htm_bucket_array_t       *p_next_bucket;
    gl_htm_key_node_t           *p_temp_node;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_KEY_NOT_FOUND()
        
        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE;
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_KEY_NOT_FOUND()
        
        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    key_hash_val = g_a_p_hashtable[hash_table_id]->p_hash_fn(p_key,
        g_a_p_hashtable[hash_table_id]->p_hash_arg);

    bucket_index = (key_hash_val % g_a_p_hashtable[hash_table_id]->bucket_qty);

    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + bucket_index;
    
    GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    if(GL_NULL == p_bucket->key_qty)
    {
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        GL_INC_HTM_G_KEY_NOT_FOUND()

        *p_ecode = GL_HTM_ERR_NO_ELEMENT;
        return GL_FAILURE;
    }

    for(p_temp_node = p_bucket->p_first; GL_NULL != p_temp_node;
        p_temp_node = p_temp_node->p_next)
    {
        if(GL_SUCCESS == g_a_p_hashtable[hash_table_id]->p_key_cmp_fn( p_key,
            &(p_temp_node->key), key_hash_val, p_temp_node->key_hash_val))
        {
            if(GL_NULL != p_temp_node->p_next)
            {
                if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                    [hash_table_id]->key_type)
                {
                    p_next_key->numeric_id = (p_temp_node->p_next)->key.
                        numeric_id;
                }
                else
                {
                    p_next_key->p_user_def_id = (p_temp_node->p_next)->key.
                        p_user_def_id;
                }
                *p_p_element = (p_temp_node->p_next)->p_elem;
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
                (p_temp_node->p_next)->ref_count += 1;
#endif 
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, 
                        GL_FAILURE);

                return GL_SUCCESS;
            }
            else
            {
                for(next_bucket_index = bucket_index+1; 
                    next_bucket_index < g_a_p_hashtable[hash_table_id]
                    ->bucket_qty; next_bucket_index++)
                {
                    p_next_bucket = g_a_p_hashtable[hash_table_id]
                        ->a_bucket_array + next_bucket_index;

                    GL_MT_LOCK(&(p_next_bucket->bucket_mutex), p_ecode, 
                            GL_FAILURE);

                    if(GL_NULL != p_next_bucket->p_first)
                    {
                        if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                            [hash_table_id]->key_type)
                        {
                            p_next_key->numeric_id = (p_next_bucket->p_first)->
                                key.numeric_id;
                        }
                        else
                        {
                            p_next_key->p_user_def_id = (p_next_bucket->
                                p_first)->key.p_user_def_id;
                        }

                        *p_p_element = p_next_bucket->p_first->p_elem;
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
                        (p_next_bucket->p_first)->ref_count += 1;
#endif           

                        GL_MT_UNLOCK(&(p_next_bucket->bucket_mutex),p_ecode,
                                GL_FAILURE);
                        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                                GL_FAILURE);
                        
                        return GL_SUCCESS;
                    }

                    GL_MT_UNLOCK(&(p_next_bucket->bucket_mutex), p_ecode, 
                            GL_FAILURE);
                }
                
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
                
                *p_ecode = GL_HTM_ERR_NO_NEXT_KEY;
                return GL_FAILURE;
                
            }
        }
    }

    GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    GL_INC_HTM_G_KEY_NOT_FOUND()

    *p_ecode = GL_HTM_ERR_KEY_NOT_FOUND;
    return GL_FAILURE;
}



/******************************************************************************
* Function name    : gl_htm_get_prev_key_ref_from_hash_table
* Arguments        : gw_U32bit          user_id, 
*                    gl_hash_table_t    hash_table_id,
*                    gl_hash_key_ut     *p_key,
*                    gl_hash_key_ut     *p_prev_key,
*                    gl_pvoid_t         *p_p_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the key and corresponding element, which is previous
*                    to the given key in the hash table (The previous key may
*                    in the same bucket or in bucket before the p_key 
*                    containing bucket).
* Functions called :
******************************************************************************/
gl_return_t
gl_htm_get_prev_key_ref_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_hash_key_ut            *p_key,
      gl_hash_key_ut            *p_prev_key, 
      gl_pvoid_t                *p_p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_prev_key, p_p_element, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_hash_key_ut            *p_key;
      gl_hash_key_ut            *p_prev_key; 
      gl_pvoid_t                *p_p_element; 
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gw_U32bit                   key_hash_val;
    gl_htm_bucket_array_t       *p_bucket;
    gw_S32bit                   prev_bucket_index;
    gl_htm_bucket_array_t       *p_prev_bucket;
    gl_htm_key_node_t           *p_temp_node;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE;
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */


    key_hash_val = g_a_p_hashtable[hash_table_id]->p_hash_fn(p_key,
        g_a_p_hashtable[hash_table_id]->p_hash_arg);

    bucket_index = (key_hash_val % g_a_p_hashtable[hash_table_id]->bucket_qty);

    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + bucket_index;
    
    GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    if(GL_NULL == p_bucket->key_qty)
    {
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        GL_INC_HTM_G_KEY_NOT_FOUND()

        *p_ecode = GL_HTM_ERR_NO_ELEMENT;
        return GL_FAILURE;
    }

    for(p_temp_node = p_bucket->p_first; GL_NULL != p_temp_node;
        p_temp_node = p_temp_node->p_next)
    {
        if(GL_SUCCESS == g_a_p_hashtable[hash_table_id]->p_key_cmp_fn( p_key,
            &(p_temp_node->key), key_hash_val, p_temp_node->key_hash_val))
        {
            if(GL_NULL != p_temp_node->p_prev)
            {
                if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                    [hash_table_id]->key_type)
                {
                    p_prev_key->numeric_id = (p_temp_node->p_prev)->key.
                        numeric_id;
                }
                else
                {
                    p_prev_key->p_user_def_id = (p_temp_node->p_prev)->key.
                        p_user_def_id;
                }
                
                *p_p_element = (p_temp_node->p_prev)->p_elem;
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
                (p_temp_node->p_prev)->ref_count += 1;
#endif 
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                        GL_FAILURE);

                return GL_SUCCESS;
            }
            else
            {
                for(prev_bucket_index = (gw_S32bit)(bucket_index-1); 
                    prev_bucket_index >= 0;
                    prev_bucket_index--)
                {
                    p_prev_bucket = g_a_p_hashtable[hash_table_id]->
                        a_bucket_array + prev_bucket_index;

                    GL_MT_LOCK(&(p_prev_bucket->bucket_mutex), p_ecode,
                            GL_FAILURE);

                    if(GL_NULL != p_prev_bucket->p_last)
                    {
                        if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                            [hash_table_id]->key_type)
                        {
                            p_prev_key->numeric_id = (p_prev_bucket->p_last)->
                                key.numeric_id;
                        }
                        else
                        {
                            p_prev_key->p_user_def_id = (p_prev_bucket->
                                p_last)->key.p_user_def_id;
                        }

                        *p_p_element = p_prev_bucket->p_last->p_elem;
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
                        (p_prev_bucket->p_last)->ref_count += 1;
#endif           

                        GL_MT_UNLOCK(&(p_prev_bucket->bucket_mutex), 
                                p_ecode, GL_FAILURE);
                        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                                GL_FAILURE);
                        
                        return GL_SUCCESS;
                    }

                    GL_MT_UNLOCK(&(p_prev_bucket->bucket_mutex), p_ecode,
                            GL_FAILURE);
                }
                
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, 
                        GL_FAILURE);
                
                *p_ecode = GL_HTM_ERR_NO_NEXT_KEY;
                return GL_FAILURE;
                
            }
        }
    }

    GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

    GL_INC_HTM_G_KEY_NOT_FOUND()

    *p_ecode = GL_HTM_ERR_KEY_NOT_FOUND;
    return GL_FAILURE;
}




/******************************************************************************
* Function name    : gl_htm_get_key_list_from_hash_table
* Arguments        : gw_U32bit          user_id, 
*                    gl_hash_table_t    hash_table_id,
*                    gl_pvoid_t         p_hash_walk_handle,
*                    gw_U8bit           *p_num_of_nodes,
*                    gl_htm_key_select_fn_t  p_key_select_fn,
*                    gl_pvoid_t         p_key_select_fn_argu,
*                    gl_hash_key_ut     a_hash_key[],
*                    gl_pvoid_t         a_p_element[],
*                    gw_boolean         *p_is_more_key_to_traverse,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Get a list of key and corresponding element from first 
*                    non-empty bucket of the hash-table for the first time 
*                    and for further calls return the key, pointer to element
*                    and handle to key-node.
* Functions called :
******************************************************************************/
gl_return_t
gl_htm_get_key_list_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_pvoid_t                *p_p_hash_walk_handle,
      gw_U8bit                  *p_num_of_nodes,
      gl_htm_key_select_fn_t    p_key_select_fn,
      gl_pvoid_t                p_key_select_fn_argu,
      gl_hash_key_ut            a_hash_key[], 
      gl_pvoid_t                a_p_element[],
      gw_boolean                *p_is_more_key_to_traverse,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_p_hash_walk_handle, p_num_of_nodes, 
      p_key_select_fn, p_key_select_fn_argu, a_hash_key, a_p_element, 
      p_is_more_key_to_traverse, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_pvoid_t                *p_p_hash_walk_handle;
      gw_U8bit                  *p_num_of_nodes;
      gl_htm_key_select_fn_t    p_key_select_fn;
      gl_pvoid_t                p_key_select_fn_argu;
      gl_hash_key_ut            a_hash_key[];
      gl_pvoid_t                a_p_element[]; 
      gw_boolean                *p_is_more_key_to_traverse;
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   i;
    gw_U32bit                   bucket_index;
    gw_U32bit                   next_bucket_index;
    gl_htm_bucket_array_t       *p_bucket = GL_NULL;
    gl_htm_key_node_t           *p_temp_node = GL_NULL;
    gw_boolean                  is_hash_empty = GL_TRUE;
    gw_boolean                  is_any_next_key_found = GL_FALSE;
    gw_boolean                  is_first_node = GL_FALSE;
    gl_htm_key_action_et        is_key_to_include;
    gl_htm_hash_walk_handle_t   *p_handle;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(GL_NULL == *p_p_hash_walk_handle)
    {
        /*
         * As handle to hash walk is NULL, So start a new hash-walk
         * from the first stored key in the hash-table.
         */
        if(GL_FAILURE == (*p_p_hash_walk_handle = (gl_htm_hash_walk_handle_t *)
            gl_htm_get_memory_buffer( g_a_p_hashtable[hash_table_id]->
            owner_user_id, g_a_p_hashtable[hash_table_id]->user_mem_info, 
            sizeof(gl_htm_hash_walk_handle_t), p_ecode)))
        {
            *p_ecode = GL_HTM_ERR_GET_MEMORY_FAILS;
            return GL_FAILURE;
        }

        p_handle = (gl_htm_hash_walk_handle_t *)*p_p_hash_walk_handle;
        /*
         * Get the index of first non-empty bucket of hash-table.
         */ 
        for(bucket_index = 0; bucket_index < g_a_p_hashtable[hash_table_id]->
            bucket_qty; bucket_index++)
        {
            p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + 
                bucket_index;
            
            GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
            
            if(GL_NULL != p_bucket->p_first)
            {
                p_temp_node = p_bucket->p_first;
                
                p_handle->bucket_number = bucket_index;
                p_handle->key_node_number = 1;
                
                is_hash_empty = GL_FALSE;
                is_first_node = GL_TRUE;
                
                break;
            }
            
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
        }
        
    }
    else
    {
        /*
         * As handle to hash-walk is Non-Null, So start the hash-walk
         * from the key returned as last key in the previous call to API
         * with same handle to hash-walk.
         */
        p_handle = (gl_htm_hash_walk_handle_t *)*p_p_hash_walk_handle;
        
        if(g_a_p_hashtable[hash_table_id]->bucket_qty < p_handle->
            bucket_number)
        {
            *p_ecode = GL_HTM_ERR_INV_HANDLE_TO_HASH_WALK;
            return GL_FAILURE;
        }
        
        is_hash_empty = GL_FALSE;
        
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + p_handle->
            bucket_number;
            
        GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
        
        if(p_bucket->key_qty > p_handle->key_node_number)
        {
            p_temp_node = p_bucket->p_first;
            
            for(i=0; (GL_NULL != p_temp_node) && 
                     (i < (p_handle->key_node_number - 1)); i++)
            {
                p_temp_node = p_temp_node->p_next;   
            }
        }
        else 
        {
            if(GL_NULL != p_bucket->p_last)
            {
                p_temp_node = p_bucket->p_last;
            }
            else
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
                
                is_any_next_key_found = GL_FALSE;
            
                /*
                 * Find the next non-empty bucket.
                 */ 
                for(next_bucket_index = p_handle->bucket_number + 1; 
                    next_bucket_index < g_a_p_hashtable[hash_table_id]->bucket_qty;
                    next_bucket_index++)
                {
                    p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array
                        + next_bucket_index;
                
                    GL_MT_LOCK(&(p_bucket->bucket_mutex),p_ecode,GL_FAILURE);

                    if(GL_NULL != p_bucket->p_first)
                    {
                        p_temp_node = p_bucket->p_first;
                
                        p_handle->bucket_number = next_bucket_index;
                        p_handle->key_node_number = 1;
                
                        is_first_node = GL_TRUE;
                        is_any_next_key_found = GL_TRUE;
                
                        break;
                    }
                
                    GL_MT_UNLOCK(&(p_bucket->bucket_mutex),p_ecode,GL_FAILURE);
                }
                if(GL_FALSE == is_any_next_key_found)
                {   
                    *p_num_of_nodes = 0;
                    *p_is_more_key_to_traverse = GL_FALSE;
                    /*
                     * Free the memory allocated for hash-walk handle.
                     */ 
                    gl_htm_terminate_get_key_list_search(
                        hash_table_id, p_p_hash_walk_handle, p_ecode );

                    return GL_SUCCESS;
                }
            }
        }
    }
    
    if(GL_TRUE == is_hash_empty)
    {
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
        *p_is_more_key_to_traverse = GL_FALSE;
        *p_ecode = GL_HTM_ERR_HASH_TABLE_EMPTY;
        return GL_FAILURE;
    }
    
    for(i=0; i < *p_num_of_nodes; )
    {
        if((GL_NULL != p_temp_node->p_next) || (GL_TRUE == is_first_node))
        {
            /*
             * If the first node of the bucket, is that starting od hash-walk,
             * then get the key respective to p_temp_node.
             */
            if(GL_TRUE == is_first_node)
            {
                is_first_node = GL_FALSE;
            }
            else
            {
                p_temp_node = p_temp_node->p_next;
                (p_handle->key_node_number)++;
            }
            
            is_key_to_include = GL_HTM_INCLUDE_KEY_IN_RET_LIST;
            /*
             * User defined, pointer to key filter function is not NULL
             * then call the function, whether key and element pointer 
             * is included in the list depends on the return value of 
             * flag.
             * If return value of flag = GL_TRUE, include key.
             * If return value of flag = GL_FALSE, do not include key.
             */ 
            
            if(GL_NULL != p_key_select_fn)
            { 
                if(GL_FAILURE == p_key_select_fn(p_key_select_fn_argu, 
                    &(p_temp_node->key), p_temp_node->p_elem, 
                    &is_key_to_include, p_ecode))
                {
                    *p_num_of_nodes = (gw_U8bit)i;
                    GL_MT_UNLOCK(&(p_bucket->bucket_mutex),p_ecode,
                        GL_FAILURE);
                    
                    return GL_FAILURE;
                }
            }
            
            if(GL_HTM_INCLUDE_KEY_IN_RET_LIST == is_key_to_include)
            {
                /*
                 * key is to be included in the returned list.
                 */ 
                if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                    [hash_table_id]->key_type)
                {
                    /*
                     * If key-type is Numeric, then copy the value of key.
                     */ 
                    a_hash_key[i].numeric_id = p_temp_node->key.numeric_id;
                }
                else
                {
                    /*
                     * Copy the pointer to user defined id in the key value.
                     */ 
                    a_hash_key[i].p_user_def_id = p_temp_node->key.
                        p_user_def_id;
                }
                a_p_element[i] = p_temp_node->p_elem;
                
                i++;
            }/* end for if GL_TRUE == is_key_to_include flag */ 
        }
        else
        {
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

            is_any_next_key_found = GL_FALSE;
            
            /*
             * Find the next non-empty bucket.
             */ 
            for(next_bucket_index = p_handle->bucket_number + 1; 
                next_bucket_index < g_a_p_hashtable[hash_table_id]->bucket_qty;
                next_bucket_index++)
            {
                p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array
                    + next_bucket_index;
                
                p_handle->bucket_number = next_bucket_index;

                GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, 
                    GL_FAILURE);

                if(GL_NULL != p_bucket->p_first)
                {
                    is_key_to_include = GL_HTM_INCLUDE_KEY_IN_RET_LIST;
                    
                    if(GL_NULL != p_key_select_fn)
                    {
                        if(GL_FAILURE == p_key_select_fn(p_key_select_fn_argu,
                            &(p_bucket->p_first->key), p_bucket->p_first->
                            p_elem, &is_key_to_include, p_ecode))
                        {
                            *p_num_of_nodes = (gw_U8bit)i;
                            GL_MT_UNLOCK(&(p_bucket->bucket_mutex),p_ecode,
                                GL_FAILURE);
                            return GL_FAILURE;
                        }
                    }
                    if(GL_HTM_INCLUDE_KEY_IN_RET_LIST == is_key_to_include)
                    {
                        if((gw_U8bit)GL_HTM_NUMERIC_KEY_TYPE == g_a_p_hashtable
                            [hash_table_id]->key_type)
                        {
                            a_hash_key[i].numeric_id = (p_bucket->p_first)->
                                key.numeric_id;
                        }
                        else
                        {
                            a_hash_key[i].p_user_def_id = (p_bucket->p_first)->
                                key.p_user_def_id;
                        }
                        a_p_element[i] = p_bucket->p_first->p_elem;
                
                        i++;
                    }
                    p_handle->key_node_number = 1;
                    p_temp_node = p_bucket->p_first;
                    is_any_next_key_found = GL_TRUE;
                    break;
                }
                
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex),p_ecode,
                    GL_FAILURE);
            }

            if(GL_FALSE == is_any_next_key_found)
            {
                *p_num_of_nodes = (gw_U8bit)i;
                *p_is_more_key_to_traverse = GL_FALSE;
                /*
                 * Free the memory allocated for hash-walk handle.
                 */ 
                gl_htm_terminate_get_key_list_search(
                    hash_table_id, p_p_hash_walk_handle, p_ecode );

                return GL_SUCCESS;
            }
        }
    }

    GL_MT_UNLOCK(&(p_bucket->bucket_mutex),p_ecode, GL_FAILURE);
    /*
     * set the value of p_num_of_nodes.
     */
    *p_num_of_nodes = (gw_U8bit)i;
    *p_is_more_key_to_traverse = GL_TRUE;
    return GL_SUCCESS;
}




/******************************************************************************
* Function name    : gl_htm_terminate_get_key_list_search
* Arguments        : gl_hash_table_t    hash_table_id,
*                    gl_pvoid_t         *p_p_hash_walk_handle, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To free the resources of the hash-table, allocted for 
 *                   the hash-walk. 
* Functions called :
******************************************************************************/
gl_return_t 
gl_htm_terminate_get_key_list_search

#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id,
      gl_pvoid_t                *p_p_hash_walk_handle, 
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_p_hash_walk_handle, p_ecode )
      gl_hash_table_t           hash_table_id;
      gl_pvoid_t                *p_p_hash_walk_handle; 
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    /*
     * To free the resouces (buffer) allocated for the hash-walk handle.
     */
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(GL_NULL != *p_p_hash_walk_handle)
    {
        if(GL_FAILURE == gl_htm_release_memory_buffer(
            g_a_p_hashtable[hash_table_id]->owner_user_id,
            g_a_p_hashtable[hash_table_id]->user_mem_info,
            *p_p_hash_walk_handle, p_ecode) )
        {
            *p_ecode = GL_HTM_ERR_RELEASE_MEMORY_FAILS;
            return GL_FAILURE;
        }       
    }
    *p_p_hash_walk_handle = GL_NULL;
    return GL_SUCCESS;
}




/******************************************************************************
* Function name    : gl_htm_hash_walk_in_one_shot
* Arguments        : gw_U32bit          user_id, 
*                    gl_hash_table_t    hash_table_id,
*                    gl_htm_key_callback_fn_t  p_key_callback_fn,
*                    gl_pvoid_t         p_key_callback_fn_argu,
*                    gw_U32bit          *p_num_of_nodes_deleted,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Traverse all the keys in one shot and invoke vcallback
*                    function for each key and delete the key node from 
*                    hash-table (for alll the key for which the return value 
*                    of flag = GL_HTM_DELETE_KEY_NODE and it is assumed that
*                    that memory of key and data element is released in 
*                    callback function and Hash Table Manager only deletes 
*                    the key node from hash-table).
* Functions called :
******************************************************************************/
gl_return_t
gl_htm_hash_walk_in_one_shot

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_htm_key_callback_fn_t  p_key_callback_fn,
      gl_pvoid_t                p_key_callback_fn_argu,
      gw_U32bit                 *p_num_of_nodes_deleted,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id,  p_key_callback_fn, p_key_callback_fn_argu,
      p_num_of_nodes_deleted, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_htm_key_callback_fn_t  p_key_callback_fn;
      gl_pvoid_t                p_key_callback_fn_argu;
      gw_U32bit                 *p_num_of_nodes_deleted;
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gl_htm_bucket_array_t       *p_bucket;
    gl_htm_key_node_t           *p_temp_node = GL_NULL;
    gl_htm_key_node_t           *p_next_node = GL_NULL;
    gw_boolean                  is_hash_empty = GL_TRUE;
    gl_htm_key_action_et        is_key_to_delete;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(hash_table_id >= GL_HTM_MAX_HASH_TABLE
        || GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_HASH_TABLE; 
        return GL_FAILURE;
    }
    if( g_a_p_hashtable[hash_table_id]->owner_user_id != user_id)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
    if(GL_NULL == p_key_callback_fn)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_NULL_CALLBACK_FUNC_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    *p_num_of_nodes_deleted = 0;
    
    for(bucket_index = 0; bucket_index < g_a_p_hashtable[hash_table_id]->
        bucket_qty; bucket_index++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array
            + bucket_index;
        
        GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        for(p_temp_node = p_bucket->p_first; p_temp_node != GL_NULL;
            p_temp_node = p_next_node)
        {
            is_hash_empty = GL_FALSE;
            
            p_next_node = p_temp_node->p_next;

            is_key_to_delete = GL_HTM_NO_ACTION;
            
            if(GL_FAILURE == p_key_callback_fn(p_key_callback_fn_argu,
                &(p_temp_node->key), p_temp_node->p_elem, &is_key_to_delete,
                p_ecode))
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex),p_ecode, GL_FAILURE);
                return GL_FAILURE;
            }
            /*
             * Check the return value of flag, and delete the key node if
             * flag = GL_HTM_DELETE_KEY_NODE .
             */  
            if(GL_HTM_DELETE_KEY_NODE == is_key_to_delete)
            {
                if(GL_FAILURE == htm_remove_key_node(hash_table_id, p_bucket,
                    p_temp_node, p_ecode))
                {
                    GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                        GL_FAILURE);

                    return GL_FAILURE;
                }

                *p_num_of_nodes_deleted += 1;
            }
        }

        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    }

    if(GL_TRUE == is_hash_empty)
    {
        *p_ecode = GL_HTM_ERR_HASH_TABLE_EMPTY;
        return GL_FAILURE;
    }
    
    return GL_SUCCESS;

}            

