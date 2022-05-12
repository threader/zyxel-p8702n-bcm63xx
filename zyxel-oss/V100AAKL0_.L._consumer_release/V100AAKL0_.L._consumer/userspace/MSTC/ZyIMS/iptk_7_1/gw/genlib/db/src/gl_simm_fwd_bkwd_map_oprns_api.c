/*****************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - String Identifier 
*                     Mapping Manager implementation.  
******************************************************************************
*
*    FILENAME    :    gl_simm_fwd_bkwd_map_oprns_api.c
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
* 29/08/2002    Ravi Jindal     SPR-4818        Remove warnings on Insure.
*
* 09/08/2002    Ravi Jindal     SPR-4840        To add the range based
*                                               searching.
*
* 17/10/2002    Ravi Jindal     SPR-4960        To add the bug fix in add map
*                                               APIs, for failure cases. The 
*                                               actions needs to be revert back.
*
* COPYRIGHT , Aricent, 2006
* 
*****************************************************************************/

#include "gl_sim.h"


/*****************************************************************************
* Function name    : gl_sim_add_string_id_map
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gw_U8bit           *p_string_id,
*                    gl_pvoid_t         p_str_id_info,
*                    gw_U32bit          *p_numeric_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Add the string-id and corresponding pointer to user 
*                    information structure to database, also generate and 
*                    return back the numeric-id corresponding to map.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_add_string_id_map

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gw_U8bit                  *p_string_id,
      gl_pvoid_t                p_str_id_info,
      gw_U32bit                 *p_numeric_id,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, p_string_id, p_str_id_info, p_numeric_id, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t               sim_db_id;
      gw_U8bit                  *p_string_id;
      gl_pvoid_t                p_str_id_info;
      gw_U32bit                 *p_numeric_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   numeric_id;
    gl_hash_key_ut              hash_key;
    gw_U8bit                    *p_temp_str_id;
    
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
    if(GL_NULL == p_string_id)
    {
        *p_ecode = GL_SIM_ERR_NULL_STRING_ID_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    if(g_a_p_sim_db[sim_db_id]->max_num_of_map == 
        g_a_p_sim_db[sim_db_id]->curr_num_of_map)
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_DB_FULL;
        return GL_FAILURE;
    }
    /*
     * If string-id are case-insensitive change the string to lower case.
     */
    if(GL_TRUE == g_a_p_sim_db[sim_db_id]->str_id_case_insensitive)
    {
        p_temp_str_id = p_string_id;
        while('\0' != *p_temp_str_id)
        {
            *p_temp_str_id = (gw_U8bit)tolower((gw_S32bit)*p_temp_str_id);
            p_temp_str_id++;
        }
    }
    /*
     * Get the next free index of the linear array,
     * the free index is used as numeric-id for the map.
     */
    numeric_id = sim_get_free_index(&(g_a_p_sim_db[sim_db_id]->a_ln_array),
        g_a_p_sim_db[sim_db_id]->max_num_of_map);
    
    g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes[numeric_id].p_str_id_info =
        p_str_id_info;
    
    hash_key.p_user_def_id = (gl_pvoid_t)p_string_id;
    /*
     * Add the map to hash-table, by the given string-id pointer.
     */ 
    if(GL_FAILURE == gl_htm_add_key_to_hash_table(g_a_p_sim_db[sim_db_id]->
        user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, &hash_key, 
        &(g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes[numeric_id]),
        &(g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes[numeric_id].
        p_hash_node_handle),p_ecode))
    {
        /*
         * set the numeric-id to free numeric-id list.
         */
        sim_set_index_free(&(g_a_p_sim_db[sim_db_id]->a_ln_array), numeric_id,
            g_a_p_sim_db[sim_db_id]->max_num_of_map);
    
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        if(GL_HTM_ERR_GET_MEMORY_FAILS == *p_ecode)
        {
            /*
             * If HAsh table call fails due to lack of memory,
             * then error code should be No-Resources.
             */ 
            *p_ecode = GL_SIM_ERR_NO_RESOURCES;
        }
        else if(GL_HTM_ERR_DUPLICATE_KEY == *p_ecode)
        {
            *p_ecode = GL_SIM_ERR_DUPLICATE_STRING_ID;
        }
        else
        {
            /*
             * Error is due to some internal problem.
             */ 
            *p_ecode = GL_SIM_ERR_INTERNAL;
        }
        return GL_FAILURE;
        
    }
    
    g_a_p_sim_db[sim_db_id]->curr_num_of_map += 1;
    /*
     * Add 1 to numeric-id as numeric-id to be returned to user should be in
     * range og 1 - max_num_of_map, 
     * but the numeric-id used for internal processing is in range
     * 0 - (max_num_of_map - 1).
     */ 
    *p_numeric_id = numeric_id + 1;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 





/*****************************************************************************
* Function name    : gl_sim_add_numeric_id_map
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gw_U32bit          numeric_id,
*                    gw_U8bit           *p_string_id,
*                    gl_pvoid_t         p_str_id_info,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Add the numeric-id map corresponding string-id and
*                    pointer to user information structure to database.
*                    This function only to be used for already generated
*                    numeric-id by an API "gl_sim_add_string_id_map".
*                    This function maintains the same mapping of string-id
*                    and numeric-id (may be required to copy database in 
*                    standby mode). So no new mapping is maintained only
*                    previously stored map is restored in a new database.  
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_add_numeric_id_map

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gw_U32bit                 numeric_id,
      gw_U8bit                  *p_string_id,
      gl_pvoid_t                p_str_id_info,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, numeric_id, p_string_id, p_str_id_info, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t            sim_db_id;
      gw_U32bit                 numeric_id;
      gw_U8bit                  *p_string_id;
      gl_pvoid_t                p_str_id_info;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gl_hash_key_ut              hash_key;
    gw_U8bit                    *p_temp_str_id;
    
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
    if(GL_NULL == p_string_id)
    {
        *p_ecode = GL_SIM_ERR_NULL_STRING_ID_PTR;
        return GL_FAILURE;
    }
    if((0 == numeric_id) || (g_a_p_sim_db[sim_db_id]->max_num_of_map <
        numeric_id))
    {
        *p_ecode = GL_SIM_ERR_INV_NUMERIC_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    user_id = user_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    if(g_a_p_sim_db[sim_db_id]->max_num_of_map == 
        g_a_p_sim_db[sim_db_id]->curr_num_of_map)
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_DB_FULL;
        return GL_FAILURE;
    }
    /*
     * If string-id are case-insensitive change the string to lower case.
     */
    if(GL_TRUE == g_a_p_sim_db[sim_db_id]->str_id_case_insensitive)
    {
        p_temp_str_id = p_string_id;
        while('\0' != *p_temp_str_id)
        {
            *p_temp_str_id = (gw_U8bit)tolower((gw_S32bit)*p_temp_str_id);
            p_temp_str_id++;
        }
    }
    
    /*
     * Set the given numeric_id as busy for the (numeric_id - 1) index 
     * of the linear array.
     * As numeric-id (index) used for internal processing is in range
     * 0 - (max_num_of_map - 1).
     */ 
    if(GL_FAILURE == sim_set_index_allocated(&(g_a_p_sim_db[sim_db_id]->
        a_ln_array), numeric_id - 1, g_a_p_sim_db[sim_db_id]->max_num_of_map))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        *p_ecode = GL_SIM_ERR_NUMERIC_ID_ALREADY_ALLOCATED;
        return GL_FAILURE;
    }
    
    g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes[numeric_id-1].
        p_str_id_info = p_str_id_info;
    
    hash_key.p_user_def_id = (gl_pvoid_t)p_string_id;
    /*
     * Add the map to hash-table, by the given string-id pointer.
     */ 
    if(GL_FAILURE == gl_htm_add_key_to_hash_table(g_a_p_sim_db[sim_db_id]->
        user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, &hash_key, 
        &(g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes[numeric_id - 1]), 
        &(g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes[numeric_id - 1].
        p_hash_node_handle),p_ecode))
    {
        /*
         * set the numeric-id to free numeric-id list.
         */
        sim_set_index_free(&(g_a_p_sim_db[sim_db_id]->a_ln_array), numeric_id-1,
            g_a_p_sim_db[sim_db_id]->max_num_of_map);
    
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        if(GL_HTM_ERR_GET_MEMORY_FAILS == *p_ecode)
        {
            /*
             * If HAsh table call fails due to lack of memory,
             * then error code should be No-Resources.
             */ 
            *p_ecode = GL_SIM_ERR_NO_RESOURCES;
        }
        else if(GL_HTM_ERR_DUPLICATE_KEY == *p_ecode)
        {
            *p_ecode = GL_SIM_ERR_DUPLICATE_STRING_ID;
        }
        else
        {
            /*
             * Error is due to some internal problem.
             */ 
            *p_ecode = GL_SIM_ERR_INTERNAL;
        }
        return GL_FAILURE;
    }
    
    g_a_p_sim_db[sim_db_id]->curr_num_of_map += 1;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode,
        GL_FAILURE);
    
    return GL_SUCCESS;
} 




/*****************************************************************************
* Function name    : gl_sim_get_numeric_id
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gw_U8bit           *p_string_id,
*                    gw_U32bit          *p_numeric_id,
*                    gl_pvoid_t         *p_p_str_id_info,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Search the database for a given string-id and returns 
*                    corresponding numeric-id and pointer to user information
*                    structure.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_get_numeric_id

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gw_U8bit                  *p_string_id,
      gw_U32bit                 *p_numeric_id,
      gl_pvoid_t                *p_p_str_id_info,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, p_string_id, p_numeric_id, p_p_str_id_info, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t               sim_db_id;
      gw_U8bit                  *p_string_id;
      gw_U32bit                 *p_numeric_id;
      gl_pvoid_t                *p_p_str_id_info;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   numeric_id;
    gl_hash_key_ut              hash_key;
    gl_sim_ln_array_node_t      *p_ln_node;
    gw_U8bit                    *p_temp_str_id;

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
    if(GL_NULL == p_string_id)
    {
        *p_ecode = GL_SIM_ERR_NULL_STRING_ID_PTR;
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
    
    /*
     * If string-id are case-insensitive change the string to lower case.
     */
    if(GL_TRUE == g_a_p_sim_db[sim_db_id]->str_id_case_insensitive)
    {
        p_temp_str_id = p_string_id;
        while('\0' != *p_temp_str_id)
        {
            *p_temp_str_id = (gw_U8bit)tolower((gw_S32bit)*p_temp_str_id);
            p_temp_str_id++;
        }
    }
    hash_key.p_user_def_id = (gl_pvoid_t)p_string_id;
    /*
     * Search the map from hash-table, by the given string-id pointer.
     */ 
    if(GL_FAILURE == gl_htm_get_key_ref_from_hash_table(g_a_p_sim_db[sim_db_id]
        ->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, &hash_key, 
        (gl_pvoid_t *)(&p_ln_node), p_ecode))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        if(GL_HTM_ERR_KEY_NOT_FOUND == *p_ecode)
        {
            /*
             * If string-id is not fiound in the hash-table.
             */ 
            *p_ecode = GL_SIM_ERR_STRING_ID_NOT_FOUND;
        }
        else
        {
            *p_ecode = GL_SIM_ERR_INTERNAL;
        }
        return GL_FAILURE;
        
    }
    /*
     * Get the index of linear for tyhe given node pointer,
     * the index is the numeric-id for the map.
     */
    numeric_id = (p_ln_node - g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes);
    
    /*
     * Add 1 to numeric-id as numeric-id to be returned to user should be in
     * range og 1 - max_num_of_map, 
     * but the numeric-id used for internal processing is in range
     * 0 - (max_num_of_map - 1).
     */ 
    *p_numeric_id = numeric_id + 1;
    *p_p_str_id_info = p_ln_node->p_str_id_info;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 






/*****************************************************************************
* Function name    : gl_sim_get_string_id
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gw_U32bit          numeric_id,
*                    gw_U8bit           *p_p_string_id,
*                    gl_pvoid_t         *p_p_str_id_info,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Search the database for a given numeric-id and returns 
*                    corresponding string-id and pointer to user information
*                    structure.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_get_string_id

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gw_U32bit                 numeric_id,
      gw_U8bit                  **p_p_string_id,
      gl_pvoid_t                *p_p_str_id_info,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, numeric_id, p_p_string_id, p_p_str_id_info, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t            sim_db_id;
      gw_U32bit                 numeric_id;
      gw_U8bit                  **p_p_string_id;
      gl_pvoid_t                *p_p_str_id_info;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
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
    if((GL_NULL == numeric_id) || (g_a_p_sim_db[sim_db_id]->max_num_of_map < 
        numeric_id))
    {
        *p_ecode = GL_SIM_ERR_INV_NUMERIC_ID;
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
    
    p_hash_node_handle = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes
        [numeric_id-1].p_hash_node_handle;
    /*
     * Search the map from hash-table, by the key node handle.
     */ 
    if(GL_FAILURE == gl_htm_get_key_ref_by_key_node_handle(g_a_p_sim_db
        [sim_db_id]->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, 
        p_hash_node_handle, &hash_key, (gl_pvoid_t *)(&p_ln_node), p_ecode))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
        
    }
    /*
     * The string-id is stored in the returned hash-key.
     */
    *p_p_string_id = (gw_U8bit *)(hash_key.p_user_def_id);
    *p_p_str_id_info = p_ln_node->p_str_id_info;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 





/*****************************************************************************
* Function name    : gl_sim_remove_map_by_string_id
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gw_U8bit           *p_string_id,
*                    gw_U8bit           **p_p_str_id_buf,
*                    gw_U32bit          *p_numeric_id,
*                    gl_pvoid_t         *p_p_str_id_info,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Search and remove the map from the database for a given
*                    string-id and returns corresponding numeric-id and pointer
*                    to user information structure.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_remove_map_by_string_id

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gw_U8bit                  *p_string_id,
      gw_U8bit                  **p_p_str_id_buf,
      gw_U32bit                 *p_numeric_id,
      gl_pvoid_t                *p_p_str_id_info,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, p_string_id, p_p_str_id_buf, p_numeric_id,
      p_p_str_id_info, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t            sim_db_id;
      gw_U8bit                  *p_string_id;
      gw_U8bit                  **p_p_str_id_buf;
      gw_U32bit                 *p_numeric_id;
      gl_pvoid_t                *p_p_str_id_info;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   numeric_id;
    gl_hash_key_ut              hash_key;
    gl_sim_ln_array_node_t      *p_ln_node;
    gw_U8bit                    *p_temp_str_id;

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
    if(GL_NULL == p_string_id)
    {
        *p_ecode = GL_SIM_ERR_NULL_STRING_ID_PTR;
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
    /*
     * If string-id are case-insensitive change the string to lower case.
     */
    if(GL_TRUE == g_a_p_sim_db[sim_db_id]->str_id_case_insensitive)
    {
        p_temp_str_id = p_string_id;
        while('\0' != *p_temp_str_id)
        {
            *p_temp_str_id = (gw_U8bit)tolower((gw_S32bit)*p_temp_str_id);
            p_temp_str_id++;
        }
    }
    
    hash_key.p_user_def_id = (gl_pvoid_t)p_string_id;
    /*
     * Remove the key-map from hash-table, by the given string-id pointer.
     */ 
    if(GL_FAILURE == gl_htm_remove_key_from_hash_table(g_a_p_sim_db[sim_db_id]
        ->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, &hash_key, 
        (gl_pvoid_t *)p_p_str_id_buf, (gl_pvoid_t *)(&p_ln_node), p_ecode))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        if(GL_HTM_ERR_KEY_NOT_FOUND == *p_ecode)
        {
            /*
             * If string-id is not fiound in the hash-table.
             */ 
            *p_ecode = GL_SIM_ERR_STRING_ID_NOT_FOUND;
        }
        else
        {
            *p_ecode = GL_SIM_ERR_INTERNAL;
        }
        
        return GL_FAILURE;
        
    }
    /*
     * Get the index of linear for tyhe given node pointer,
     * the index is the numeric-id for the map.
     */
    numeric_id = (p_ln_node - g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes);
    
    /*
     * Add 1 to numeric-id as numeric-id to be returned to user should be in
     * range og 1 - max_num_of_map, 
     * but the numeric-id used for internal processing is in range
     * 0 - (max_num_of_map - 1).
     */ 
    *p_numeric_id = numeric_id + 1;
    *p_p_str_id_info = p_ln_node->p_str_id_info;
    /*
     * set the numeric-id to free numeric-id list.
     */
    sim_set_index_free(&(g_a_p_sim_db[sim_db_id]->a_ln_array), numeric_id,
        g_a_p_sim_db[sim_db_id]->max_num_of_map);
    
    g_a_p_sim_db[sim_db_id]->curr_num_of_map -= 1;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 






/*****************************************************************************
* Function name    : gl_sim_remove_map_by_numeric_id
* Arguments        : gw_U32bit          user_id,
*                    gl_sim_db_id_t     sim_db_id,
*                    gw_U32bit          numeric_id,
*                    gw_U8bit           *p_p_string_id,
*                    gl_pvoid_t         *p_p_str_id_info,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Search the remove the map from database for a given 
*                    numeric-id and returns corresponding string-id and 
*                    pointer to user information structure.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_remove_map_by_numeric_id

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gw_U32bit                 numeric_id,
      gw_U8bit                  **p_p_string_id,
      gl_pvoid_t                *p_p_str_id_info,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, numeric_id, p_p_string_id, p_p_str_id_info, p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t            sim_db_id;
      gw_U32bit                 numeric_id;
      gw_U8bit                  **p_p_string_id;
      gl_pvoid_t                *p_p_str_id_info;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gl_hash_key_ut              hash_key;
    gl_sim_ln_array_node_t      *p_ln_node;
    gl_sim_ln_array_node_t      *p_temp_ln_node;
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
    if((GL_NULL == numeric_id) || (g_a_p_sim_db[sim_db_id]->max_num_of_map < 
        numeric_id))
    {
        *p_ecode = GL_SIM_ERR_INV_NUMERIC_ID;
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
    
    p_hash_node_handle = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes
        [numeric_id-1].p_hash_node_handle;
    /*
     * Search the map from hash-table, by the key node handle.
     */ 
    if(GL_FAILURE == gl_htm_get_key_ref_by_key_node_handle(g_a_p_sim_db
        [sim_db_id]->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, 
        p_hash_node_handle, &hash_key, (gl_pvoid_t *)(&p_ln_node), p_ecode))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
        
    }
    /*
     * The string-id is stored in the returned hash-key.
     * Remove the key-map from hash-table, by the returned hash_key.
     */ 
    if(GL_FAILURE == gl_htm_remove_key_from_hash_table(g_a_p_sim_db
        [sim_db_id]->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id,
        &hash_key,(gl_pvoid_t *)p_p_string_id, (gl_pvoid_t *)(&p_temp_ln_node),
        p_ecode))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
        
    }
    
    *p_p_string_id = (gw_U8bit *)(hash_key.p_user_def_id);
    *p_p_str_id_info = p_ln_node->p_str_id_info;
    /*
     * set the numeric-id to free numeric-id list.
     */
    sim_set_index_free(&(g_a_p_sim_db[sim_db_id]->a_ln_array), numeric_id - 1,
       g_a_p_sim_db[sim_db_id]->max_num_of_map);
    
    g_a_p_sim_db[sim_db_id]->curr_num_of_map -= 1;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 



/*******************************************************************************
* Function name    : gl_sim_get_string_id_map_list
* Arguments        : gw_U32bit          user_id, 
*                    gl_sim_db_id_t     sim_db_id,
*                    gl_pvoid_t         *p_p_handle,
*                    gw_U8bit           *p_string_to_match,
*                    gw_U8bit           *p_num_of_match,
*                    gl_sim_str_id_filter_fn_t   p_str_id_filter_fn,
*                    gl_pvoid_t         p_str_id_filter_fn_argu,
*                    gw_U8bit           *a_p_string_id[],
*                    gl_pvoid_t         a_p_str_id_info[],
*                    gw_U32bit          a_numeric_id[], 
*                    gw_boolean         *p_is_more_ids_to_match,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : This API is used to find out the String Identifiers, 
*                    which have string to match as a common prefix and suffix and 
*                    associated pointer to information structures in a 
*                    set of length num_of_match.
*                    The format of string_to_match = prefix + * + suffix
*                    suffix or prefix or both can absent,
*                    To find all the maps string = *
* Functions called :
*******************************************************************************/
gl_return_t
gl_sim_get_string_id_map_list

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id, 
      gl_sim_db_id_t            sim_db_id,
      gl_pvoid_t                *p_p_handle,
      gw_U8bit                  *p_string_to_match, 
      gw_U8bit                  *p_num_of_match, 
      gl_sim_str_id_filter_fn_t p_str_id_filter_fn, 
      gl_pvoid_t                p_str_id_filter_fn_argu, 
      gw_U8bit                  *a_p_string_id[], 
      gl_pvoid_t                a_p_str_id_info[], 
      gw_U32bit                 a_numeric_id[], 
      gw_boolean                *p_is_more_ids_to_match,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, sim_db_id, p_p_handle, p_string_to_match, p_num_of_match,
      p_str_id_filter_fn, p_str_id_filter_fn_argu, a_p_string_id,
      a_p_str_id_info, a_numeric_id, p_is_more_ids_to_match,
      p_ecode )
      gw_U32bit                 user_id; 
      gl_sim_db_id_t            sim_db_id; 
      gl_pvoid_t                *p_p_handle;
      gw_U8bit                  *p_string_to_match; 
      gw_U8bit                  *p_num_of_match; 
      gl_sim_str_id_filter_fn_t p_str_id_filter_fn; 
      gl_pvoid_t                p_str_id_filter_fn_argu; 
      gw_U8bit                  *a_p_string_id[]; 
      gl_pvoid_t                a_p_str_id_info[]; 
      gw_U32bit                 a_numeric_id[];
      gw_boolean                *p_is_more_ids_to_match;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
    gw_U8bit                    i;
    gw_U32bit                   numeric_id;
    gl_sim_ln_array_node_t      *p_ln_node;
    gl_sim_filter_fn_argument_t filter_fn_argu;
    gl_hash_key_ut              *p_hash_key;
    gw_U8bit                    *p_str_id;
    gl_pvoid_t                  p_str_id_info;
    gw_U8bit                    *p_temp_str_id;
    
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
    if(GL_NULL == p_string_to_match)
    {
        *p_ecode = GL_SIM_ERR_INV_STRING_TO_MATCH_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    if(GL_NULL == g_a_p_sim_db[sim_db_id]->curr_num_of_map)
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_DB_EMPTY;
        return GL_FAILURE;
    }
    /*
     * If string-id are case-insensitive change the string to lower case.
     */
    if(GL_TRUE == g_a_p_sim_db[sim_db_id]->str_id_case_insensitive)
    {
        p_temp_str_id = p_string_to_match;
        while('\0' != *p_temp_str_id)
        {
            *p_temp_str_id = (gw_U8bit)tolower((gw_S32bit)*p_temp_str_id);
            p_temp_str_id++;
        }
    }

    if(GL_NULL != gl_stm_strchr(p_string_to_match, (gw_U8bit)'[', p_ecode))
    {
        if(GL_FAILURE == sim_get_map_list_by_range(sim_db_id,
                          p_p_handle,
                          p_string_to_match,
                          p_num_of_match,
                          p_str_id_filter_fn,
                          p_str_id_filter_fn_argu,
                          a_p_string_id,
                          a_p_str_id_info,
                          a_numeric_id,
                          p_is_more_ids_to_match,
                          p_ecode ))
        {
            GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
                GL_FAILURE);
        
            *p_ecode = GL_SIM_ERR_INTERNAL;
            return GL_FAILURE;
        }
    }
    else if(GL_NULL != gl_stm_strchr(p_string_to_match, (gw_U8bit)'*',
        p_ecode))
    {
        /*
         * Fill the filter function argument, 
         * As the user defined filter function will be invoked
         * internally by the SIM function "sim_string_id_filter_func"
         * The "sim_string_id_filter_func" is invoked by Hash-Table Manager
         * for each string-id.
         * So Two level of fitering is used for each string-id.
         * 1st level - To find string-ids which have a given string as prefix.
         * 2nd level - User defined filter function, take decision on string-id
         *             and user information structure.
         */             
        filter_fn_argu.p_user_fn = p_str_id_filter_fn;
        filter_fn_argu.p_user_fn_argu = p_str_id_filter_fn_argu;
        filter_fn_argu.p_str_to_match = p_string_to_match;
        filter_fn_argu.p_ln_node_array = g_a_p_sim_db[sim_db_id]->a_ln_array.
            a_ln_nodes;

        /*
         * Get the key-list from the hash-table, key is string-id of map.
         * Numeric-id array is used (to save dynamic allocation of memory)
         * to get the string-ids from hash-table
         * String-ids are stored in form of hash-keys in hash-table.
         */ 
        if(GL_FAILURE == gl_htm_get_key_list_from_hash_table(user_id,
            g_a_p_sim_db[sim_db_id]->hash_table_id, 
            p_p_handle, p_num_of_match, (gl_htm_key_select_fn_t)
            sim_string_id_filter_func, (gl_pvoid_t) &(filter_fn_argu), 
            (gl_hash_key_ut *)a_numeric_id, 
            a_p_str_id_info, p_is_more_ids_to_match, p_ecode))
        {
            GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
                GL_FAILURE);
        
            *p_ecode = GL_SIM_ERR_INTERNAL;
            return GL_FAILURE;
        
        }
        /*
         * Fill the array of string-id, numeric-id and user information strcuture.
         */
        for(i=0; i < *p_num_of_match; i++)
        {
            /*
             * Read the key array, too getb string-ids, retuned in form of
             * hash-keys.
             */
            p_hash_key  = ((gl_hash_key_ut *)&a_numeric_id[i]);
            p_str_id    = (gw_U8bit *)(p_hash_key->p_user_def_id);
            /*
             * Read the handle to hash-node, to get address corresponding 
             * linear array node.
             */
            p_ln_node   = (gl_sim_ln_array_node_t *)a_p_str_id_info[i];
            p_str_id_info = p_ln_node->p_str_id_info;
            /*
             * Calculate the numeric-id.
             */ 
            numeric_id = (p_ln_node - g_a_p_sim_db[sim_db_id]->a_ln_array.
                a_ln_nodes);
        
            a_p_string_id[i]    = p_str_id;
            a_p_str_id_info[i]  = p_str_id_info;
            a_numeric_id[i]     = numeric_id + 1;
        }
    }
    else
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
    }

    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);

    return GL_SUCCESS;
}




/*****************************************************************************
* Function name    : gl_sim_terminate_get_map_list_search 
* Arguments        : gl_sim_db_id_t     sim_db_id,
*                    gl_pvoid_t         *p_p_map_list_handle,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Returns the number of maps currently stored in the 
*                    database. 
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_terminate_get_map_list_search

#ifdef _ANSI_C_
    ( gl_sim_db_id_t            sim_db_id,
      gl_pvoid_t                *p_p_map_list_handle,
      gl_error_t                *p_ecode
    )
#else
    ( sim_db_id, p_p_map_list_handle, p_ecode )
      gl_sim_db_id_t            sim_db_id;
      gl_pvoid_t                *p_p_map_list_handle;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( sim_db_id >= GL_SIM_MAX_DB
        || GL_NULL == g_a_p_sim_db[sim_db_id])
    {
        *p_ecode = GL_SIM_ERR_INV_DB_ID; 
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
        GL_FAILURE);
    
    if(GL_FAILURE == gl_htm_terminate_get_key_list_search(g_a_p_sim_db
        [sim_db_id]->hash_table_id, p_p_map_list_handle, p_ecode))
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
    }
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode,
        GL_FAILURE);
    
    return GL_SUCCESS;
} 


/*****************************************************************************
* Function name    : gl_sim_get_map_count
* Arguments        : gl_sim_db_id_t     sim_db_id,
*                    gw_U32bit          *p_num_of_maps,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Returns the number of maps currently stored in the 
*                    database. 
* Functions called :
*****************************************************************************/
gl_return_t
gl_sim_get_map_count

#ifdef _ANSI_C_
    ( gl_sim_db_id_t            sim_db_id,
      gw_U32bit                 *p_num_of_maps,
      gl_error_t                *p_ecode
    )
#else
    ( sim_db_id, p_num_of_maps, p_ecode )
      gl_sim_db_id_t               sim_db_id;
      gw_U32bit                 *p_num_of_maps;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( sim_db_id >= GL_SIM_MAX_DB
        || GL_NULL == g_a_p_sim_db[sim_db_id])
    {
        *p_ecode = GL_SIM_ERR_INV_DB_ID; 
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MT_LOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    if(GL_NULL == g_a_p_sim_db[sim_db_id]->curr_num_of_map)
    {
        GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, 
            GL_FAILURE);
        
        *p_ecode = GL_SIM_ERR_DB_EMPTY;
        return GL_FAILURE;
    }
    
    *p_num_of_maps = g_a_p_sim_db[sim_db_id]->curr_num_of_map;
    
    GL_MT_UNLOCK(&(g_a_p_sim_db[sim_db_id]->sim_db_mutex), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
} 

