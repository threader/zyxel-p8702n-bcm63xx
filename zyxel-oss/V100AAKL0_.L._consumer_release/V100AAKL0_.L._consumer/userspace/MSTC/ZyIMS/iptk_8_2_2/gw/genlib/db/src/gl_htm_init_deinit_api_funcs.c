/***************************************************************************** 
*
*    FUNCTION    :    Source file for Generic Library - Hash table 
*                     implementation.  
******************************************************************************
*
*    FILENAME    :    gl_htm_init_deinit_api_funcs.c
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
* 28/03/2008    Shruti Thakral   SPR 18315       Klocwork warning removal
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_htm.h"


#ifdef GL_MT_ENABLED
    GL_MUTEX_ID htm_mgr_sem;
#endif


/*****************************************************************************
* Function name    : gl_htm_init
* Arguments        : void
* Return Value     : gl_return_t
* Functionality    : Initializes the generic libraries hash table manager.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_init( void )
    
/****************************************************************************/
{
    gw_U32bit loop_cntr;
    gl_error_t ecode;

    if(htm_mgr_init_flag == GL_FALSE)
    {
#ifdef GL_MT_ENABLED
        if(GL_FAILURE == gl_create_mutex(&htm_mgr_sem, &ecode))
        {
            GL_INC_HTM_G_MUTX_FAIL()
                
            return GL_FAILURE;
        }
#ifdef GL_STATS_ENABLED
        g_htm_stat_init = GL_TRUE;
        
        if(GL_FAILURE == gl_create_mutex(&g_htm_stat_sem, &ecode))
        {
            GL_INC_HTM_G_MUTX_FAIL()
                
            return GL_FAILURE;
        }
#endif /* Endif for GL_STATS_ENABLED */

#else
        ecode = 0;
#endif

        for(loop_cntr=0; loop_cntr < GL_HTM_MAX_HASH_TABLE; loop_cntr++)
        {
            g_a_p_hashtable[loop_cntr]  = GL_NULL;
        }
        
        htm_mgr_init_flag = GL_TRUE;
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}




/*****************************************************************************
* Function name    : gl_htm_create_hash_table
* Arguments        : gw_U32bit                  user_id, 
*                    gl_hash_table_t            *p_hash_table_id,
*                    gw_U32bit                  max_key_qty,
*                    gw_U32bit                  max_num_buckets,
*                    gw_U8bit                   key_type,
*                    gw_U8bit                   hash_func_type,
*                    gl_htm_hash_fn_t           p_hash_fn,
*                    gl_htm_free_key_elem_fn_t  p_elem_free_fn,
*                    gl_pvoid_t                 p_hash_arg,
*                    gl_htm_key_cmp_fn_t        p_key_cmp_fn,
*                    gl_error_t                 *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the hash table structure and initializes the.
*                    hash table structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_create_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           *p_hash_table_id,
      gw_U32bit                 max_key_qty,
      gw_U32bit                 max_num_buckets,
      gw_U8bit                  key_type,
      gw_U8bit                  hash_func_type,
      gl_htm_hash_fn_t          p_hash_fn,
      gl_htm_free_key_elem_fn_t p_elem_free_fn,
      gl_pvoid_t                p_hash_arg,
      gl_htm_key_cmp_fn_t       p_key_cmp_fn,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_hash_table_id, max_key_qty, max_num_buckets, key_type, 
      hash_func_type, p_hash_fn, p_elem_free_fn, p_hash_arg, p_key_cmp_fn,
      p_ecode )
    
      gw_U32bit                 user_id; 
      gl_hash_table_t*          p_hash_table_id;
      gw_U32bit                 max_key_qty;
      gw_U32bit                 max_num_buckets;
      gw_U8bit                  key_type;
      gw_U8bit                  hash_func_type;
      gl_htm_hash_fn_t          p_hash_fn;
      gl_htm_free_key_elem_fn_t p_elem_free_fn;
      gl_pvoid_t                p_hash_arg;
      gl_htm_key_cmp_fn_t       p_key_cmp_fn;
      gl_error_t*               p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   i;
    gw_U16bit                   tmp_index;
    gl_hash_table_t             hash_table_id = 0;
    gl_htm_bucket_array_t       *p_bucket;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(0 == max_num_buckets)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_BUCKET_NUM;
        return GL_FAILURE;
    }
    if(0 == max_key_qty)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_KEY_QTY;
        return GL_FAILURE;
    }
    if(GL_NULL == p_elem_free_fn)
    {
        GL_INC_HTM_G_PARM_VLD_FAIL()

        *p_ecode = GL_HTM_ERR_INV_ELEM_FREE_FUNC_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if((gw_U8bit)GL_HTM_HASH_FUNC_FOR_NUMERIC_KEYS == hash_func_type) 
    {
        p_hash_fn = (gl_htm_hash_fn_t)gl_htm_def_numeric_key_hash_fn;
        p_key_cmp_fn = (gl_htm_key_cmp_fn_t)gl_htm_def_numeric_key_cmp_fn;
    }
    else if((gw_U8bit)GL_HTM_HASH_FUNC_FOR_STRING_KEYS == hash_func_type)
    {
        p_hash_fn = (gl_htm_hash_fn_t)gl_htm_def_string_key_hash_fn;
        p_key_cmp_fn = (gl_htm_key_cmp_fn_t)gl_htm_def_string_key_cmp_fn;
    }
#ifdef GL_STATIC_VALIDATION_ENABLED
    else
    {
        if(GL_NULL == p_hash_fn)
        {
            GL_INC_HTM_G_PARM_VLD_FAIL()

            *p_ecode = GL_HTM_ERR_INV_HASH_FUNC_PTR;
            return GL_FAILURE;    
        }
        if(GL_NULL == p_key_cmp_fn)
        {
            GL_INC_HTM_G_PARM_VLD_FAIL()

            *p_ecode = GL_HTM_ERR_INV_KEY_COMP_FUNC_PTR;
            return GL_FAILURE;    
        }
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    
    GL_MUTEX_LOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
    
    for(tmp_index=0; tmp_index < GL_HTM_MAX_HASH_TABLE; tmp_index++)
    {
        if(GL_NULL == g_a_p_hashtable[tmp_index])
        {
            hash_table_id = tmp_index;
            break;
        }
    }
    /*
     * If(hash_table_id == GL_HTM_MAX_HASH_TABLE) Laha, 24/02/00 -
     * in case of no entry found, hash_table_id is never initialized
     * and hence check makes no sense. 
     */
    if(tmp_index == GL_HTM_MAX_HASH_TABLE)
    {
    
        GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
    
        *p_ecode = GL_HTM_ERR_NO_RESOURCES;
        return GL_FAILURE;
    }
    
    g_a_p_hashtable[hash_table_id] = (gl_htm_table_t *) SYS_MALLOC(
        sizeof(gl_htm_table_t) + (max_num_buckets *
        sizeof(gl_htm_bucket_array_t)));
    
    if(GL_NULL == g_a_p_hashtable[hash_table_id])
    {
        GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
        
        GL_INC_HTM_G_MALOC_FAIL()
 
        *p_ecode = GL_HTM_ERR_NO_RESOURCES;
        return GL_FAILURE;
    }
    
    g_a_p_hashtable[hash_table_id]->a_bucket_array =
        (gl_htm_bucket_array_t *) ((gw_S8bit *)g_a_p_hashtable[hash_table_id] 
        + sizeof(gl_htm_table_t));
    
    for(i=0; i < max_num_buckets; i++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + i;
        p_bucket->p_first = GL_NULL;
        p_bucket->p_last = GL_NULL;
        p_bucket->key_qty = 0;

#ifdef GL_TOTAL_MT_SAFE
        if(GL_FAILURE == gl_create_mutex (&(p_bucket->bucket_mutex), p_ecode))
        {
            GL_INC_HTM_G_MUTX_FAIL()
                
            SYS_FREE(g_a_p_hashtable[hash_table_id]);
            g_a_p_hashtable[hash_table_id] = GL_NULL;
            
            GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
        
            *p_ecode = GL_HTM_ERR_NO_RESOURCES;
            return GL_FAILURE;
        }
#endif
    }

#ifdef GL_STATS_ENABLED
    if(GL_FAILURE == gl_htm_stat_allocte_memory(hash_table_id, p_ecode))
    {
        GL_INC_HTM_G_MALOC_FAIL()

        SYS_FREE(g_a_p_hashtable[hash_table_id]);
        g_a_p_hashtable[hash_table_id] = GL_NULL;

        GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
#endif    
    /*
     * Memory required to store contents of a key node, to be stored
     * in a bucket.
     */ 
    if(GL_FAILURE == gl_htm_reserve_memory(user_id, 
        &(g_a_p_hashtable[hash_table_id]->user_mem_info), max_key_qty, 
        sizeof(gl_htm_key_node_t), p_ecode))
    {
        SYS_FREE(g_a_p_hashtable[hash_table_id]);
        g_a_p_hashtable[hash_table_id] = GL_NULL;
        
        GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
    
    GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
    
    *p_hash_table_id = hash_table_id;
    
    g_a_p_hashtable[hash_table_id]->owner_user_id  = user_id;
    g_a_p_hashtable[hash_table_id]->bucket_qty     = max_num_buckets;
    g_a_p_hashtable[hash_table_id]->key_type       = key_type;
    g_a_p_hashtable[hash_table_id]->max_key_qty    = max_key_qty;
    g_a_p_hashtable[hash_table_id]->p_hash_fn      = p_hash_fn;
    g_a_p_hashtable[hash_table_id]->p_elem_free_fn = p_elem_free_fn;
    g_a_p_hashtable[hash_table_id]->p_key_cmp_fn   = p_key_cmp_fn;
    g_a_p_hashtable[hash_table_id]->p_hash_arg     = p_hash_arg;
    g_a_p_hashtable[hash_table_id]->max_buket_size_thrsh = GL_NULL;
    
    GL_INC_HTM_C_TOTAL_KEY_QTY(hash_table_id, max_key_qty)
        
    return GL_SUCCESS;
}




/*****************************************************************************
* Function name    : gl_htm_delete_hash_table
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gl_pvoid_t         *p_p_hash_arg,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the hash table and resets the 
*                    hash table structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_delete_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id,
      gl_pvoid_t                *p_p_hash_arg,
      gl_error_t*               p_ecode
    )
#else
    ( user_id, hash_table_id, p_p_hash_arg, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id;
      gl_pvoid_t                *p_p_hash_arg;
      gl_error_t*               p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   i;
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

    
    for(i=0; i<g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + i;
    
        GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
        if(GL_NULL != p_bucket->key_qty)
        {
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
            GL_INC_HTM_G_DEL_RQ_TABLE_BSY()
    
            *p_ecode= GL_HTM_ERR_TABLE_NOT_EMPTY;
            return GL_FAILURE;
        }
    
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    }
    
    for(i=0; i<g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + i;
    
        GL_MT_DELETE(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
    }

    
    GL_MUTEX_LOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
    
    *p_p_hash_arg = g_a_p_hashtable[hash_table_id]->p_hash_arg;
    /*
     * Call Portable function to free the memory, reserved for key nodes.
     */ 
    gl_htm_free_memory(g_a_p_hashtable[hash_table_id]->owner_user_id, 
        g_a_p_hashtable[hash_table_id]->user_mem_info, g_a_p_hashtable
        [hash_table_id]->max_key_qty, sizeof(gl_htm_key_node_t), p_ecode);

#ifdef GL_STATS_ENABLED
    gl_htm_stat_free_memory(hash_table_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */
    
    g_a_p_hashtable[hash_table_id]->owner_user_id  = GL_INVALID_TASK_ID;
    g_a_p_hashtable[hash_table_id]->bucket_qty     = GL_NULL;
    g_a_p_hashtable[hash_table_id]->max_key_qty    = GL_NULL;
    g_a_p_hashtable[hash_table_id]->p_hash_fn      = GL_NULL;
    g_a_p_hashtable[hash_table_id]->p_elem_free_fn = GL_NULL;
    g_a_p_hashtable[hash_table_id]->p_key_cmp_fn   = GL_NULL;
    g_a_p_hashtable[hash_table_id]->p_hash_arg     = GL_NULL;
    g_a_p_hashtable[hash_table_id]->key_type    = GL_HTM_USER_DEFINED_KEY_TYPE;
    
    SYS_FREE(g_a_p_hashtable[hash_table_id]);
    
    g_a_p_hashtable[hash_table_id]                  = GL_NULL;
    
    GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 



/*****************************************************************************
* Function name    : gl_htm_remove_all_keys_from_hash_table
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id, 
                     gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Flush all the elements in the hash table and reset the 
*                    hash table structure elements.This function internally
*                    call user defined element free function.
* Functions called :
******************************************************************************/
gl_return_t 
gl_htm_remove_all_keys_from_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id, 
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id; 
      gl_error_t                *p_ecode;
#endif
    
/*****************************************************************************/
{
    gw_U32bit                   bucket_index;
    gl_htm_bucket_array_t       *p_bucket = GL_NULL;
    gl_htm_key_node_t           *p_temp_node = GL_NULL;
    gl_htm_key_node_t           *p_dummy_node = GL_NULL;
    gw_U8bit                    is_hash_table_empty = GL_TRUE;
    
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

        if(GL_NULL == p_bucket->key_qty)
        {
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

            continue;
        }
        
        is_hash_table_empty = GL_FALSE;
        
        for(p_temp_node = p_bucket->p_first; GL_NULL != p_temp_node;)
        {
            /*
             * To free the memory allocated by the User for
             * p_element corrosponding to p_key.
             * For the same HTM call User defined element memory free 
             * function, defined in Create hash table API.
             */ 
            if(GL_FAILURE == g_a_p_hashtable[hash_table_id]->p_elem_free_fn
                (g_a_p_hashtable[hash_table_id]->owner_user_id, &(p_temp_node
                ->key), g_a_p_hashtable[hash_table_id]->key_type, p_temp_node
                ->p_elem, g_a_p_hashtable[hash_table_id]->p_hash_arg, p_ecode))
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                        GL_FAILURE);
                
                return GL_FAILURE;
            }
            /* Current node is to be removed now.
             * Thus, it is assigned to a dummy node.
             * Current node now moves to next 
             * and dummy node is removed */

            p_dummy_node = p_temp_node;
            p_temp_node = p_temp_node->p_next;

            if(GL_FAILURE == htm_remove_key_node(hash_table_id, p_bucket,
                p_dummy_node, p_ecode))
            {
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode,
                        GL_FAILURE);

                return GL_FAILURE;
            }
        }

        p_bucket->key_qty = 0;
        
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    }
    if(GL_TRUE == is_hash_table_empty)
    {
        *p_ecode = GL_HTM_ERR_HASH_TABLE_EMPTY;
        return GL_FAILURE;
    }
    
    return GL_SUCCESS;
}






/*****************************************************************************
* Function name    : gl_htm_resize_hash_table
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          max_buket_size_thrsh,
*                    gw_U32bit          *p_new_num_buckets,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Increase the number of buckets of a hash-table and
*                    rehash all the keys stored in old hash-table to 
*                    new hash-table.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_resize_hash_table

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 max_buket_size_thrsh,
      gw_U32bit                 *p_new_num_buckets,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, max_buket_size_thrsh, p_new_num_buckets,
      p_ecode )
      gw_U32bit                 user_id; 
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 max_buket_size_thrsh;
      gw_U32bit                 *p_new_num_buckets;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   i;
    gw_U8bit                    is_rehashing_required = GL_FALSE;
    gw_U32bit                   new_bucket_index;
    gw_U32bit                   key_hash_val;
    gw_U32bit                   new_bucket_qty;
    gl_htm_bucket_array_t       *p_bucket;
    gl_htm_bucket_array_t       *p_new_bucket;
    gl_htm_bucket_array_t       *a_new_bucket_array;
    gl_htm_key_node_t           *p_key_node;
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

    
    for(i=0; i<g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
    {
        p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + i;
    
        GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
        if(p_bucket->key_qty > max_buket_size_thrsh)
        {
            is_rehashing_required = GL_TRUE;
    
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
            break;
        }
        
        GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    }

    new_bucket_qty = g_a_p_hashtable[hash_table_id]->bucket_qty + 30;

    if(GL_TRUE == is_rehashing_required)
    {
        a_new_bucket_array = (gl_htm_bucket_array_t *) 
           calloc(new_bucket_qty, sizeof(gl_htm_bucket_array_t));
       
        if(GL_NULL == a_new_bucket_array)
        {
            *p_ecode = GL_HTM_ERR_NO_RESOURCES;
            return GL_FAILURE;
        }
    
        for(i=0; i < new_bucket_qty; i++)
        {
            p_new_bucket = a_new_bucket_array + i;
            p_new_bucket->p_first = GL_NULL;
            p_new_bucket->p_last = GL_NULL;
            p_new_bucket->key_qty = 0;
#ifdef GL_TOTAL_MT_SAFE
            if(GL_FAILURE == gl_create_mutex(&(p_new_bucket->bucket_mutex),
                p_ecode))
            {
                SYS_FREE(a_new_bucket_array);
                *p_ecode = GL_HTM_ERR_NO_RESOURCES;
                return GL_FAILURE;
            }
#endif
        }
        /*
         * No Addtional Memory is required to store contents of a key node,
         * to be stored in a bucket.
         * Only the postion of key node is changed from previous bucket 
         * to new bucket(after resize of hash table).
         */ 
        for(i=0; i < g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
        {
            p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + i;
    
            GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
        
            for(p_key_node = p_bucket->p_first; 
                GL_NULL != p_key_node;
                p_key_node = p_temp_node)
            {
                /*
                 * Store the value of p_next of p_key_node as when postion of
                 * key-node is changed, its p_next and p_prev get changed.
                 */ 
                p_temp_node = p_key_node->p_next;
                
                key_hash_val = p_key_node->key_hash_val;
    
                new_bucket_index = (key_hash_val % new_bucket_qty);
    
                p_new_bucket = a_new_bucket_array + new_bucket_index;
    
                if(GL_NULL == p_new_bucket->p_first)
                {
                    p_key_node->p_next = p_key_node->p_prev = GL_NULL;
                    p_new_bucket->p_first = p_new_bucket->p_last = p_key_node;
                }
                else
                {
                    p_key_node->p_next = GL_NULL;
                    p_key_node->p_prev = p_new_bucket->p_last;
                    p_new_bucket->p_last->p_next = p_key_node;
                    p_new_bucket->p_last = p_key_node;
                }
            }
    
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
    
        }
        
        SYS_FREE(g_a_p_hashtable[hash_table_id]->a_bucket_array);
        g_a_p_hashtable[hash_table_id]->a_bucket_array  = GL_NULL;

        g_a_p_hashtable[hash_table_id]->a_bucket_array = a_new_bucket_array;
        *p_new_num_buckets = g_a_p_hashtable[hash_table_id]->bucket_qty = 
        new_bucket_qty;

        return GL_SUCCESS;
    }
    else
    {
        *p_ecode = GL_HTM_ERR_RESIZE_NOT_REQUIRED;
        return GL_FAILURE;
    }
}

        



/******************************************************************************
* Function name    : gl_htm_destroy_all_hash_tables
* Arguments        : gw_U8bit           is_forceful_cleanup
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes all the hash-tables
*                    and free all the corresponding occupied resources.
* Functions called :
******************************************************************************/
gl_return_t
gl_htm_destroy_all_hash_tables

#ifdef _ANSI_C_
    ( gw_U8bit                  is_forceful_cleanup,
      gl_error_t                *p_ecode
    )
#else
    ( is_forceful_cleanup, p_ecode )
      gw_U8bit                  is_forceful_cleanup;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U16bit                   hash_table_id = GL_NULL;
    gw_boolean                  is_not_empty_flag = GL_FALSE; 
    gw_U32bit                   i;
    gl_htm_bucket_array_t       *p_bucket;
    gw_boolean                  is_hash_empty = GL_TRUE;
   
    GL_MUTEX_LOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);

    for(hash_table_id=0; hash_table_id < GL_HTM_MAX_HASH_TABLE;
            hash_table_id++)
    {
        if( GL_NULL == g_a_p_hashtable[hash_table_id])
        {
            continue;
        }                 
        for(i=0; i<g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
        {
            p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + i;
   
            GL_MT_LOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
   
            if(GL_NULL != p_bucket->key_qty)
            {
                is_hash_empty = GL_FALSE;
                
                GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, 
                        GL_FAILURE);
   
                break;
            }
            
            GL_MT_UNLOCK(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);
        }
        if((is_forceful_cleanup == GL_FALSE) &&
           (is_hash_empty == GL_FALSE))
        {
            is_not_empty_flag = GL_TRUE;
            continue;
        }
        if(GL_FALSE == is_hash_empty)
        {
            if(GL_FAILURE == gl_htm_remove_all_keys_from_hash_table(
                g_a_p_hashtable[hash_table_id]->owner_user_id, hash_table_id,
                p_ecode))
            {
                GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
                return GL_FAILURE;
            }
        }
        for(i=0; i<g_a_p_hashtable[hash_table_id]->bucket_qty; i++)
        {
            p_bucket = g_a_p_hashtable[hash_table_id]->a_bucket_array + i;

            GL_MT_DELETE(&(p_bucket->bucket_mutex), p_ecode, GL_FAILURE);

        }
        g_a_p_hashtable[hash_table_id]->a_bucket_array  = GL_NULL;

        /*
         * Call Portable function to free the memory, reserved for key nodes.
         */ 
        gl_htm_free_memory(g_a_p_hashtable[hash_table_id]->owner_user_id, 
            g_a_p_hashtable[hash_table_id]->user_mem_info, g_a_p_hashtable
            [hash_table_id]->max_key_qty, sizeof(gl_htm_key_node_t), p_ecode);
    
#ifdef GL_STATS_ENABLED
        gl_htm_stat_free_memory(hash_table_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        SYS_FREE(g_a_p_hashtable[hash_table_id]);
        g_a_p_hashtable[hash_table_id] = GL_NULL;
    }
        
    if( is_not_empty_flag == GL_TRUE)
    {
        /* 
         * Some list are not empty and no forcefull destroy.
         */
        
        GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_HTM_ERR_TABLE_NOT_EMPTY;
        return GL_FAILURE;
    }
    else
    {
        GL_MUTEX_UNLOCK(&htm_mgr_sem, p_ecode, GL_FAILURE);
        GL_MUTEX_DELETE(&htm_mgr_sem, p_ecode, GL_FAILURE);
#ifdef GL_STATS_ENABLED
#ifdef GL_MT_ENABLED
        if(GL_TRUE == g_htm_stat_init)
        {
            gl_delete_mutex(&g_htm_stat_sem, p_ecode);
        }
        g_htm_stat_init = GL_FALSE;
#endif
#endif /* Endif for GL_STATS_ENABLED */
        htm_mgr_init_flag = GL_FALSE;
        return GL_SUCCESS;

     }
}

