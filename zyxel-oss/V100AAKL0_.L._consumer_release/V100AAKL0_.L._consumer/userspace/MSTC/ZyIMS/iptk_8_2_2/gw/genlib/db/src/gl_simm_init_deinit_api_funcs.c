/*****************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - String Identifier 
*                     Mapping Manager implementation.  
******************************************************************************
*
*    FILENAME    :    gl_simm_init_deinit_api_funcs.c
*
*    DESCRIPTION :    This file contains the constants and function 
*                     declarations of string identifier mapping sub-module of
*                     the generic library implemntation. The functions include
*                     those for creation of database and operations such as 
*                     addition/searching and removal of maps etc.
*                     (source file)
*
* DATE             NAME          REFERENCE        REASON
*-------------  ---------------  ---------------  ----------------------
*
* 27/05/2002    Ravi Jindal     SPR-3960        Intial Draft, Enhancements 
*                                               for Rel-1.0.       
*
* COPYRIGHT , Aricent, 2006
* 
*****************************************************************************/

#include "gl_sim.h"

#ifdef GL_MT_ENABLED
    GL_MUTEX_ID sim_mgr_sem;
#endif

/*****************************************************************************
* Function name    : gl_sim_init
* Arguments        : void
* Return Value     : gl_return_t
* Functionality    : Initializes the generic libraries string Identifier 
*                    Mapping Manager.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_init( void )
    
/****************************************************************************/
{
    gw_U32bit loop_cntr;
    gl_error_t ecode;

    if(sim_mgr_init_flag == GL_FALSE)
    {
        gl_htm_init();
        
#ifdef GL_MT_ENABLED
        if(GL_FAILURE == gl_create_mutex (&sim_mgr_sem, &ecode))
        {
            return GL_FAILURE;
        }
#else
        ecode = 0;
#endif
        for(loop_cntr=0; loop_cntr < GL_SIM_MAX_DB; loop_cntr++)
        {
            g_a_p_sim_db[loop_cntr] = GL_NULL;
        }
        sim_mgr_init_flag = GL_TRUE;
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}




/*****************************************************************************
* Function name    : gl_sim_create_db
* Arguments        : gw_U32bit                  user_id, 
*                    gl_sim_db_id_t             *p_sim_db_id,
*                    gw_U32bit                  max_num_of_map,
*                    gw_U32bit                  max_num_of_bucket,
*                    gl_sim_free_mem_fn_t       p_free_mem_fn,
                     gw_boolean                 str_id_case_insensitive,
*                    gl_error_t                 *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the hash table structure and initializes the.
*                    hash table structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_create_db

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_sim_db_id_t            *p_sim_db_id,
      gw_U32bit                 max_num_of_map,
      gw_U32bit                 max_num_of_bucket,
      gl_sim_free_mem_fn_t      p_free_mem_fn,
      gw_boolean                str_id_case_insensitive,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_sim_db_id, max_num_of_map, max_num_of_bucket, 
      p_free_mem_fn, str_id_case_insensitive, p_ecode )
      gw_U32bit                 user_id;
      gl_sim_db_id_t            *p_sim_db_id;
      gw_U32bit                 max_num_of_map;
      gw_U32bit                 max_num_of_bucket;
      gl_sim_free_mem_fn_t      p_free_mem_fn;
      gw_boolean                str_id_case_insensitive;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   i;
    gw_U16bit                   tmp_index;
    gl_hash_table_t             sim_db_id = 0;
    gl_sim_ln_array_node_t      *p_ln_node;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(0 == max_num_of_bucket)
    {
        *p_ecode = GL_SIM_ERR_INV_BUCKET_NUM;
        return GL_FAILURE;
    }
    if(0 == max_num_of_map)
    {
        *p_ecode = GL_SIM_ERR_INV_MAP_QTY;
        return GL_FAILURE;
    }
    if(GL_NULL == p_free_mem_fn)
    {
        *p_ecode = GL_SIM_ERR_INV_FREE_MEM_FUNC_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
    
    for(tmp_index=0; tmp_index < GL_SIM_MAX_DB; tmp_index++)
    {
        if( GL_NULL == g_a_p_sim_db[tmp_index])
        {
            sim_db_id = tmp_index;
            break;
        }
    }
    
    if(tmp_index == GL_SIM_MAX_DB)
    {
    
        GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
    
        *p_ecode = GL_SIM_ERR_NO_RESOURCES;
        return GL_FAILURE;
    }
    
    g_a_p_sim_db[sim_db_id] = (gl_sim_db_t *) SYS_MALLOC( sizeof(gl_sim_db_t) +
        (max_num_of_map * sizeof(gl_sim_ln_array_node_t)));
    
    if(GL_NULL == g_a_p_sim_db[sim_db_id])
    {
        GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_SIM_ERR_NO_RESOURCES;
        return GL_FAILURE;
    }
    
    GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
    
    g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes =
        (gl_sim_ln_array_node_t *) ((gw_S8bit *)g_a_p_sim_db[sim_db_id] + 
        sizeof(gl_sim_db_t));
    
#ifdef GL_TOTAL_MT_SAFE
    if(GL_FAILURE == gl_create_mutex(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), 
        p_ecode))
    {
        g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes = GL_NULL;
        SYS_FREE(g_a_p_sim_db[sim_db_id]);
        g_a_p_sim_db[sim_db_id] = GL_NULL;
        
        *p_ecode = GL_SIM_ERR_NO_RESOURCES;
        return GL_FAILURE;
    }
#endif
    
    if(GL_FAILURE == gl_htm_create_hash_table (user_id, 
        &(g_a_p_sim_db[sim_db_id]->hash_table_id), max_num_of_map, 
        max_num_of_bucket, GL_HTM_USER_DEFINED_KEY_TYPE, 
        GL_HTM_HASH_FUNC_FOR_STRING_KEYS, GL_NULL, (gl_htm_free_key_elem_fn_t)
        sim_memory_free_func, GL_NULL, GL_NULL, p_ecode))    
    {
        g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes = GL_NULL;
        
        GL_MT_DELETE(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        SYS_FREE(g_a_p_sim_db[sim_db_id] );
        g_a_p_sim_db[sim_db_id] = GL_NULL;

        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
        
    }
    
    g_a_p_sim_db[sim_db_id]-> a_ln_array.first_free_index = 0;
    g_a_p_sim_db[sim_db_id]-> a_ln_array.last_free_index = max_num_of_map - 1;

    /*
     * For first node set values ouside the loop as
     * the previous index of first node should be an invalid number.
     */
    p_ln_node = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes;
    p_ln_node->next_free_index = 1;
    p_ln_node->prev_free_index = max_num_of_map;
    p_ln_node->p_hash_node_handle = GL_NULL;
    p_ln_node->p_str_id_info = GL_NULL;

    for(i=1; i < max_num_of_map; i++)
    {
        p_ln_node = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes + i;
        p_ln_node->next_free_index = i+1;
        p_ln_node->prev_free_index = i-1;
        p_ln_node->p_hash_node_handle = GL_NULL;
        p_ln_node->p_str_id_info = GL_NULL;
    }
    *p_sim_db_id = sim_db_id;
    g_a_p_sim_db[sim_db_id]->user_id = user_id;
    g_a_p_sim_db[sim_db_id]->max_num_of_bucket = max_num_of_bucket;
    g_a_p_sim_db[sim_db_id]->max_num_of_map = max_num_of_map;
    g_a_p_sim_db[sim_db_id]->p_free_mem_fn = p_free_mem_fn;
    g_a_p_sim_db[sim_db_id]->curr_num_of_map = GL_NULL;
    g_a_p_sim_db[sim_db_id]->str_id_case_insensitive = str_id_case_insensitive;

    return GL_SUCCESS;
}




/*****************************************************************************
* Function name    : gl_sim_delete_db
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the database and resets the 
*                    database structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_delete_db

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gl_error_t*               p_ecode
    )
#else
    ( user_id, sim_db_id, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t            sim_db_id;
      gl_error_t*               p_ecode;
#endif
    
/****************************************************************************/
{
    gl_pvoid_t                  p_hash_arg;
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( sim_db_id >= GL_SIM_MAX_DB
        || GL_NULL == g_a_p_sim_db[sim_db_id])
    {
        *p_ecode = GL_SIM_ERR_INV_DB_ID; 
        return GL_FAILURE;
    }
    if( g_a_p_sim_db[sim_db_id]->user_id != user_id)
    {
        *p_ecode = GL_SIM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MUTEX_LOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
    
    GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    if(GL_NULL != g_a_p_sim_db[sim_db_id]->curr_num_of_map)
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
        *p_ecode = GL_SIM_ERR_DB_NOT_EMPTY;
        return GL_FAILURE;
    }
    /*
     * Delete the hash-table.
     */ 
    if(GL_FAILURE == gl_htm_delete_hash_table(g_a_p_sim_db[sim_db_id]->user_id, 
        g_a_p_sim_db[sim_db_id]->hash_table_id, &p_hash_arg, p_ecode))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
        
    }
    
    GL_MT_DELETE(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
        GL_FAILURE);
    /*
     * Free the memeory allocated for the Linear Array.
     */ 
    g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes          = GL_NULL;
    g_a_p_sim_db[sim_db_id]->a_ln_array.first_free_index    = GL_NULL;
    g_a_p_sim_db[sim_db_id]->a_ln_array.last_free_index     = GL_NULL;
    g_a_p_sim_db[sim_db_id]->max_num_of_map                 = GL_NULL;
    g_a_p_sim_db[sim_db_id]->max_num_of_bucket              = GL_NULL;
    g_a_p_sim_db[sim_db_id]->p_free_mem_fn                  = GL_NULL;
    g_a_p_sim_db[sim_db_id]->curr_num_of_map                = GL_NULL;
    
    SYS_FREE(g_a_p_sim_db[sim_db_id]);
    g_a_p_sim_db[sim_db_id] = GL_NULL; 

    GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 





/*****************************************************************************
* Function name    : gl_sim_remove_all_maps
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Remove all the stored maps from database and this 
*                    function internally calls the function to free the memory
*                    allocated for the string-id and information structure.
*                    Pointer to function is stored in "gl_sim_create_db" API.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_remove_all_maps

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t            sim_db_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   i;  
    gw_U32bit                   numeric_id;
    gl_hash_key_ut              hash_key;
    gl_sim_ln_array_node_t      *p_ln_node;
    gl_pvoid_t                  p_hash_node_handle;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( sim_db_id >= GL_SIM_MAX_DB
        || GL_NULL == g_a_p_sim_db[sim_db_id])
    {
        *p_ecode = GL_SIM_ERR_INV_DB_ID; 
        return GL_FAILURE;
    }
    if( g_a_p_sim_db[sim_db_id]->user_id != user_id)
    {
        *p_ecode = GL_SIM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    
    GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    if(GL_NULL == g_a_p_sim_db[sim_db_id]->curr_num_of_map)
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_DB_EMPTY;
        return GL_FAILURE;
    }
    
    for(numeric_id = 0; numeric_id < g_a_p_sim_db[sim_db_id]->max_num_of_map;
        numeric_id++)
    {
        p_hash_node_handle = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes
            [numeric_id].p_hash_node_handle;
        /*
         * If p_hash_node_handle != GL_NULL, then it is a valid stored key 
         * handle.
         */ 
        if(GL_NULL != p_hash_node_handle)
        {
            /*
             * Search the map from hash-table, by the key node handle.
             */ 
            if(GL_FAILURE == gl_htm_get_key_ref_by_key_node_handle(g_a_p_sim_db
                [sim_db_id]->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id,
                p_hash_node_handle, &hash_key, (gl_pvoid_t *)(&p_ln_node), 
                p_ecode))
            {
                GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex),
                    p_ecode, GL_FAILURE);
        
                *p_ecode = GL_SIM_ERR_INTERNAL;
                return GL_FAILURE;
        
            }

            /*
             * As the pointer to key is returned, to free the memory 
             * allocated by user for string-id and information structure
             * Call the user defined function. Pointer to function is 
             * stored at "gl_sim_create_db" API.
             */
            if(GL_FAILURE == g_a_p_sim_db[sim_db_id]->p_free_mem_fn(
                g_a_p_sim_db[sim_db_id]->user_id, (gw_U8bit *)(hash_key.
                p_user_def_id), g_a_p_sim_db[sim_db_id]->a_ln_array.
                a_ln_nodes[numeric_id].p_str_id_info, p_ecode))
            {
                GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex),
                    p_ecode, GL_FAILURE);
                
                return GL_FAILURE;
            }
        }
    }
              
            
    /*
     * Remove all the key-map from hash-table.
     */ 
    if(GL_FAILURE == gl_htm_remove_all_keys_from_hash_table(g_a_p_sim_db
        [sim_db_id]->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id,
        p_ecode)) 
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), 
            p_ecode, GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
        
    }
    /*
     * set the all the indexs of linear array free.
     */
    g_a_p_sim_db[sim_db_id]-> a_ln_array.first_free_index = 0;
    g_a_p_sim_db[sim_db_id]-> a_ln_array.last_free_index = 
        g_a_p_sim_db[sim_db_id]->max_num_of_map - 1;
    /*
     * For first node set values ouside the loop as
     * the previous index of first node should be an invalid number.
     */
    p_ln_node = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes;
    p_ln_node->next_free_index = 1;
    p_ln_node->prev_free_index =g_a_p_sim_db[sim_db_id]->max_num_of_map;
    p_ln_node->p_hash_node_handle = GL_NULL;
    p_ln_node->p_str_id_info = GL_NULL;

    for(i=1; i < g_a_p_sim_db[sim_db_id]->max_num_of_map; i++)
    {
        p_ln_node = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes + i;
        p_ln_node->next_free_index = i+1;
        p_ln_node->prev_free_index = i-1;
        p_ln_node->p_hash_node_handle = GL_NULL;
        p_ln_node->p_str_id_info = GL_NULL;
    }
    
    g_a_p_sim_db[sim_db_id]->curr_num_of_map = GL_NULL;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode,
       GL_FAILURE);
    
    return GL_SUCCESS;
} 





/*******************************************************************************
* Function name    : gl_sim_destroy_all_dbs
* Arguments        : gw_U8bit           is_forceful_cleanup
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes all the databases and free all the corresponding
*                    occupied resources.
* Functions called :
*******************************************************************************/
gl_return_t
gl_sim_destroy_all_dbs

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
    gw_U16bit                   sim_db_id;
    gw_boolean                  is_not_empty_flag = GL_FALSE; 
    gl_pvoid_t                  p_hash_arg;
   
    GL_MUTEX_LOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);

    for(sim_db_id=0; sim_db_id < GL_SIM_MAX_DB;
            sim_db_id++)
    {
        if(GL_NULL == g_a_p_sim_db[sim_db_id])
        {
            continue;
        }  
        
        if((is_forceful_cleanup == GL_FALSE ) &&
           (GL_NULL != g_a_p_sim_db[sim_db_id]->curr_num_of_map))
        {
            is_not_empty_flag = GL_TRUE;
            continue;
        }
        if(GL_NULL != g_a_p_sim_db[sim_db_id]->curr_num_of_map)
        {
            if(GL_FAILURE == gl_sim_remove_all_maps(
                g_a_p_sim_db[sim_db_id]->user_id, sim_db_id, p_ecode))
            {
                GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
                return GL_FAILURE;
            }
        }

        GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);

        /*
         * Delete the hash-table.
         */ 
        if(GL_FAILURE == gl_htm_delete_hash_table(g_a_p_sim_db[sim_db_id]->
            user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, &p_hash_arg,
            p_ecode))
        {
            GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
                GL_FAILURE);
            GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);
            *p_ecode = GL_SIM_ERR_INTERNAL;
            return GL_FAILURE;
        
        }
    
        GL_MT_DELETE(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes       = GL_NULL;
        g_a_p_sim_db[sim_db_id]->a_ln_array.first_free_index = GL_NULL;
        g_a_p_sim_db[sim_db_id]->a_ln_array.last_free_index  = GL_NULL;
        g_a_p_sim_db[sim_db_id]->max_num_of_map              = GL_NULL;
        g_a_p_sim_db[sim_db_id]->max_num_of_bucket           = GL_NULL;
        g_a_p_sim_db[sim_db_id]->p_free_mem_fn               = GL_NULL;
        g_a_p_sim_db[sim_db_id]->curr_num_of_map             = GL_NULL;
        /*
         * Free the memeory allocated for the Database.
         */ 
        SYS_FREE(g_a_p_sim_db[sim_db_id]);
        g_a_p_sim_db[sim_db_id] = GL_NULL;
    
    }
        
    if(is_not_empty_flag == GL_TRUE)
    {
        /* 
         * Some databases are not empty and no forcefull destroy.
         */
        
        GL_MUTEX_UNLOCK(&sim_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_SIM_ERR_DB_NOT_EMPTY;
        return GL_FAILURE;
    }
    else
    {
        GL_MUTEX_DELETE(&sim_mgr_sem, p_ecode, GL_FAILURE);

        sim_mgr_init_flag = GL_FALSE;
        return GL_SUCCESS;
     }
}

