/*****************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - String Identifier 
*                     Mapping Manager implementation.  
******************************************************************************
*
*    FILENAME    :    gl_simm_glb_defs_n_intrn_funcs.c
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
* 09/08/2002    Ravi Jindal     SPR-4808        To add the range based
*                                               searching.
*
* COPYRIGHT , Aricent, 2006
* 
*****************************************************************************/

#include "gl_sim.h"

/*
 * Array of databases. 
 */
gl_sim_db_t              *g_a_p_sim_db[GL_SIM_MAX_DB];

gl_boolean               sim_mgr_init_flag = GL_FALSE;




/*****************************************************************************
* Function name    : sim_get_free_index
* Arguments        : gl_sim_ln_array_t  *p_ln_array,
                     gw_U32bit          max_num_of_map
* Return Value     : gw_U32bit
* Functionality    : Returns the first free index of linear array,
*                    and update the link-list of free indexes for the array.
* Functions called :
*****************************************************************************/
gw_U32bit
sim_get_free_index

#ifdef _ANSI_C_
    ( gl_sim_ln_array_t         *p_ln_array,
      gw_U32bit                 max_num_of_map )
#else
    ( p_ln_array, max_num_of_map )
      gl_sim_ln_array_t         *p_ln_array;
      gw_U32bit                 max_num_of_map;
#endif

/******************************************************************************/
{
    gw_U32bit                   numeric_id;
    /*
     * Get the first free index of the linear array.
     */
    numeric_id = p_ln_array->first_free_index;
    p_ln_array->first_free_index = p_ln_array->a_ln_nodes[numeric_id].
        next_free_index;
    /*
     * Set the next and prev free index to invalid number (max_num_of_map)
     * As max_num_of_map can not be allocated from code valid indexs range is
     * (0 - max_num_of_map-1).
     */
    if(max_num_of_map != p_ln_array->first_free_index)
    {
        p_ln_array->a_ln_nodes[p_ln_array->first_free_index].prev_free_index = 
            max_num_of_map;
    }
    p_ln_array->a_ln_nodes[numeric_id].next_free_index = max_num_of_map;
    p_ln_array->a_ln_nodes[numeric_id].prev_free_index = max_num_of_map;

    return numeric_id;
}




/*****************************************************************************
* Function name    : sim_set_index_free
* Arguments        : gl_sim_ln_array_t  *p_ln_array,
*                    gw_U32bit          numeric_id,
*                    gw_U32bit          max_num_of_map
* Return Value     : void
* Functionality    : Set the numeric-id in free index list of linear array.
* Functions called :
*****************************************************************************/
void
sim_set_index_free

#ifdef _ANSI_C_
    ( gl_sim_ln_array_t         *p_ln_array,
      gw_U32bit                 numeric_id,
      gw_U32bit                 max_num_of_map
    )
#else
    ( p_ln_array, numeric_id, max_num_of_map )
      gl_sim_ln_array_t         *p_ln_array;
      gw_U32bit                 numeric_id;
      gw_U32bit                 max_num_of_map;
#endif

/******************************************************************************/
{
    /*
     * Set the numeric-id as last free index of the linear array.
     */
    if(max_num_of_map == p_ln_array->first_free_index)
    {
        p_ln_array->first_free_index = numeric_id;
        p_ln_array->a_ln_nodes[numeric_id].prev_free_index = max_num_of_map;
    }
    else
    {
        p_ln_array->a_ln_nodes[numeric_id].prev_free_index = p_ln_array->
            last_free_index; 
        p_ln_array->a_ln_nodes[p_ln_array->last_free_index].next_free_index =
            numeric_id;
    }
    p_ln_array->last_free_index = numeric_id;
    p_ln_array->a_ln_nodes[numeric_id].next_free_index = max_num_of_map;
    /*
     * Set the values at numeric-id by GL_NULL.
     */
    p_ln_array->a_ln_nodes[numeric_id].p_hash_node_handle = GL_NULL;
    p_ln_array->a_ln_nodes[numeric_id].p_str_id_info = GL_NULL;

    return;
}




/*****************************************************************************
* Function name    : sim_set_index_allocated
* Arguments        : gl_sim_ln_array_t  *p_ln_array,
*                    gw_U32bit          numeric_id,
*                    gw_U32bit          max_num_of_map
* Return Value     : gl_return_t
* Functionality    : Set the numeric-id as allocated and maintained the
*                    free index list of linear array.
* Functions called :
*****************************************************************************/
gl_return_t
sim_set_index_allocated

#ifdef _ANSI_C_
    ( gl_sim_ln_array_t         *p_ln_array,
      gw_U32bit                 numeric_id,
      gw_U32bit                 max_num_of_map
    )
#else
    ( p_ln_array, numeric_id, max_num_of_map )
      gl_sim_ln_array_t         *p_ln_array;
      gw_U32bit                 numeric_id;
      gw_U32bit                 max_num_of_map;
#endif

/******************************************************************************/
{
    /*
     * Set the numeric-id as allocated.
     */
    if((max_num_of_map == p_ln_array->a_ln_nodes[numeric_id].next_free_index)
     &&(max_num_of_map == p_ln_array->a_ln_nodes[numeric_id].prev_free_index))
    {
        return GL_FAILURE; 
    }        
        
    if(numeric_id == p_ln_array->first_free_index)
    {
        p_ln_array->first_free_index = p_ln_array->a_ln_nodes[numeric_id].
            next_free_index;
        p_ln_array->a_ln_nodes[p_ln_array->first_free_index].prev_free_index =
             max_num_of_map;
    }
    else if(numeric_id == p_ln_array->last_free_index)
    {
        p_ln_array->last_free_index = p_ln_array->a_ln_nodes[numeric_id].
            prev_free_index;
        p_ln_array->a_ln_nodes[p_ln_array->last_free_index].next_free_index = 
            max_num_of_map;
    }
    else
    {
        p_ln_array->a_ln_nodes[p_ln_array->a_ln_nodes[numeric_id].
            prev_free_index].next_free_index = p_ln_array->a_ln_nodes
            [numeric_id].next_free_index;
        
        p_ln_array->a_ln_nodes[p_ln_array->a_ln_nodes[numeric_id].
            next_free_index].prev_free_index = p_ln_array->a_ln_nodes
            [numeric_id].prev_free_index;

    }
    
    p_ln_array->a_ln_nodes[numeric_id].next_free_index = max_num_of_map;
    p_ln_array->a_ln_nodes[numeric_id].prev_free_index = max_num_of_map;
    
    return GL_SUCCESS;
}





/*****************************************************************************
* Function name    : sim_string_id_filter_func
* Arguments        : gl_pvoid_t         p_string_id_filter_fn_argu,
*                    gl_hash_key_ut     *p_hash_key,
*                    gl_pvoid_t         p_ln_array_node,
*                    gl_htm_key_action_et *p_is_key_to_include_in_list,
*                    gl_error_t         *p_ecode  
* Return Value     : gl_return_t
* Functionality    : Filter the string-id,
*                    First check wheter the string_to_match is a prefix of the
*                    string-id or not. If Yes, then call the user defined 
*                    filter function with string-id and poinetr to information
*                    structure, if function returns Yes for the string-id then
*                    include the string-id and corresponding map into returned
*                    array.
* Functions called :
*****************************************************************************/
gl_return_t
sim_string_id_filter_func

#ifdef _ANSI_C_
    ( gl_pvoid_t                p_string_id_filter_fn_argu, 
      gl_hash_key_ut            *p_hash_key, 
      gl_pvoid_t                p_ln_array_node,
      gl_htm_key_action_et      *p_is_key_to_include_in_list,
      gl_error_t                *p_ecode
    )
#else
    ( p_string_id_filter_fn_argu, p_hash_key, p_ln_array_node,
      p_is_key_to_include_in_list, p_ecode )
      gl_pvoid_t                p_string_id_filter_fn_argu; 
      gl_hash_key_ut            *p_hash_key; 
      gl_pvoid_t                p_ln_array_node;
      gl_htm_key_action_et      *p_is_key_to_include_in_list;
      gl_error_t                *p_ecode;  
#endif

/******************************************************************************/
{
    gl_sim_filter_fn_argument_t *p_filter_func_argu = 
        (gl_sim_filter_fn_argument_t *) p_string_id_filter_fn_argu; 
    gw_U32bit                   numeric_id;
    gw_U8bit                    *p_str_to_match;
    gw_U8bit                    *p_string_id;
    gw_boolean                  is_key_selected = GL_TRUE;
    gl_sim_ln_array_node_t      *p_ln_node = (gl_sim_ln_array_node_t *)
                                             p_ln_array_node;

    p_str_to_match = p_filter_func_argu->p_str_to_match;
    p_string_id = (gw_U8bit *)(p_hash_key->p_user_def_id);
    /*
     * Match the prefix part for the string-ids.
     */ 
    while('*' != *p_str_to_match)
    {
        if((*p_str_to_match != *p_string_id) || ('\0' == *p_string_id))
        {
            *p_is_key_to_include_in_list = GL_HTM_NO_ACTION;
            return GL_SUCCESS;
        }
        p_str_to_match++;
        p_string_id++;
    }
    /*
     * Now match the suffix part for the string-id.
     */
    if('\0' != *(p_str_to_match + 1))
    {
        /*
         * First move the pointer to last of strings.
         */ 
        while(*(++p_str_to_match) != '\0');
    
        while(*(++p_string_id) != '\0');

        p_string_id--;
        p_str_to_match--;
    
        while('*' != *p_str_to_match)
        {
            if((*p_str_to_match != *p_string_id) || 
               (p_hash_key->p_user_def_id == p_string_id))
            {
                *p_is_key_to_include_in_list = GL_HTM_NO_ACTION;
                return GL_SUCCESS;
            }
            p_str_to_match--;
            p_string_id--;
        }
    }
    *p_is_key_to_include_in_list = GL_HTM_INCLUDE_KEY_IN_RET_LIST;
    /*
     * Call the user defined function to filter the string-id,
     * with the function argument.
     */
    if(GL_NULL != p_filter_func_argu->p_user_fn)
    {
        numeric_id = (p_ln_node - p_filter_func_argu->p_ln_node_array) + 1;

        if(GL_FAILURE == p_filter_func_argu->p_user_fn( p_filter_func_argu
            ->p_user_fn_argu, (gw_U8bit *)(p_hash_key->p_user_def_id),
            p_ln_node->p_str_id_info, numeric_id, &is_key_selected,
            p_ecode))
        {
            return GL_FAILURE;
        }

        if(GL_FALSE == is_key_selected)
        {
            *p_is_key_to_include_in_list = GL_HTM_NO_ACTION;
        }
    }
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : sim_memory_free_func
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_key_ut     *p_key,
*                    gw_U8bit           key_type,
*                    gl_pvoid_t         p_element,
*                    gl_error_t         p_ecode
* Return Value     : gl_return_t
* Functionality    : Dummy function defination for free memory of pointer to
*                    elements used in hash-table.
* Functions called :
*****************************************************************************/
gl_return_t
sim_memory_free_func

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_key_ut            *p_key,
      gw_U8bit                  key_type,
      gl_pvoid_t                p_element,
      gl_error_t                p_ecode
    )  
#else
    ( user_id, p_key, key_type, p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_key_ut            *p_key;
      gw_U8bit                  key_type;
      gl_pvoid_t                p_element;
      gl_error_t                p_ecode;   
#endif

/******************************************************************************/
{
    /*
     * To remove warnings.
     */
    (void)user_id;
    (void)p_key;
    (void)key_type;
    (void)p_element;
    (void)p_ecode;
    return GL_SUCCESS;
}




/*******************************************************************************
* Function name    : sim_get_map_list_by_range
* Arguments        : gl_sim_db_id_t     sim_db_id,
*                    gl_pvoid_t         *p_p_handle,
*                    gw_U8bit           *p_string_to_match,
*                    gw_U8bit           *p_num_of_match,
*                    gl_sim_str_id_filter_fn_t   p_str_id_filter_fn,
*                    gl_pvoid_t         p_str_id_filter_fn_argu,
*                    gw_U8bit   	    *a_p_string_id[],
*                    gl_pvoid_t         a_p_str_id_info[],
*                    gw_U32bit          a_numeric_id[], 
*                    gw_boolean         *p_is_more_ids_to_match,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : This API is used to find out the String Identifiers, 
*                    which have string to match after expansion of a given
*                    ranges with common prefix and suffix and 
*                    associated pointer to information structures in a 
*                    set of length num_of_match.
*                    The format of string_to_match = prefix + [1-25] + suffix
*                    suffix or prefix or both can be any thing.
*                    Prefix and suffix assumed to be fully specified.
* Functions called :
*******************************************************************************/
gl_return_t
sim_get_map_list_by_range

#ifdef _ANSI_C_
    ( gl_sim_db_id_t            sim_db_id,
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
    ( sim_db_id, p_p_handle, p_string_to_match, p_num_of_match,
      p_str_id_filter_fn, p_str_id_filter_fn_argu, a_p_string_id,
      a_p_str_id_info, a_numeric_id, p_is_more_ids_to_match,
      p_ecode )
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
    
/*****************************************************************************/
{
    gw_U32bit                   i,j;
    gw_U32bit                   numeric_id;
    gw_U32bit                   start_range;
    gw_U32bit                   end_range;
    gw_boolean                  is_key_selected;

    gl_sim_ln_array_node_t      *p_ln_node;
    gl_hash_key_ut              hash_key;
    gl_sim_ln_array_node_t      *p_temp_ln_node;
    gl_hash_key_ut              temp_hash_key;
    gl_pvoid_t                  p_hash_node_handle;
    gl_sim_map_range_handle_t   *p_map_handle;

    gw_U8bit                    *p_start_ptr;
    gw_U8bit                    *p_middle_ptr;
    gw_U8bit                    *p_end_ptr;
    
    gw_U8bit                    p_str_id[500]      = {0};
    gw_U8bit                    p_num_str[20]      = {0};
    gw_U8bit                    p_range_start[20]  = {0};
    gw_U8bit                    p_range_end[20]    = {0};
    gw_U8bit                    p_prefix_str[500]  = {0};
    gw_U8bit                    p_suffix_str[500]  = {0};
    
    if(GL_NULL == *p_p_handle)
    {
        /*
         * As handle to hash walk is NULL, So start a new hash-walk
         * from the first stored key in the hash-table.
         */
        if(GL_FAILURE == (*p_p_handle = (gl_sim_map_range_handle_t *)
            gl_htm_get_memory_buffer(g_a_p_sim_db[sim_db_id]->user_id, 
            g_a_p_hashtable[g_a_p_sim_db[sim_db_id]->hash_table_id]->
            user_mem_info, sizeof(gl_sim_map_range_handle_t), p_ecode)))
        {
            *p_ecode = GL_HTM_ERR_GET_MEMORY_FAILS;
            return GL_FAILURE;
        }

        if(GL_NULL == (p_start_ptr = gl_stm_strchr(p_string_to_match, 
                        (gw_U8bit)'[', p_ecode)))
        {
            return GL_FAILURE;
        }
        if(GL_NULL == (p_middle_ptr = gl_stm_strchr(p_string_to_match, 
                        (gw_U8bit)'-', p_ecode)))
        {
            return GL_FAILURE;
        }
        if(GL_NULL == (p_end_ptr = gl_stm_strchr(p_string_to_match,
                        (gw_U8bit)']', p_ecode)))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_stm_strncpy(p_range_start, (gw_U8bit *)
                    (p_start_ptr+1), (gw_U32bit)(p_middle_ptr-1-p_start_ptr),
                    p_ecode))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_stm_strncpy(p_range_end, (gw_U8bit *)
                    (p_middle_ptr+1), (gw_U32bit)(p_end_ptr-1-p_middle_ptr),
                    p_ecode))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_stm_atoi(p_range_start, &start_range, p_ecode))
        {
            /* return GL_FAILURE; */
        }
        if(GL_FAILURE == gl_stm_atoi(p_range_end, &end_range, p_ecode))
        {
            return GL_FAILURE;
        }
        p_map_handle = (gl_sim_map_range_handle_t *)*p_p_handle;
        
        p_map_handle->start_range = start_range;
        p_map_handle->end_range = end_range;
        p_map_handle->start_ptr_index = p_start_ptr - p_string_to_match;
        p_map_handle->end_ptr_index = p_end_ptr - p_string_to_match;
    }
    else
    {
        p_map_handle = (gl_sim_map_range_handle_t *)*p_p_handle;
        
        start_range = p_map_handle->start_range;
        end_range = p_map_handle->end_range;
        p_start_ptr = &(p_string_to_match[p_map_handle->start_ptr_index]);
        p_end_ptr = &(p_string_to_match[p_map_handle->end_ptr_index]);
    }

    if(start_range > end_range)
    {
        /*
         * As start_range is more than end-range so we have reached at the 
         * end of searching.
         * So Free the memory allocated for p_map_handle.
         */ 
        gl_htm_terminate_get_key_list_search( g_a_p_sim_db[sim_db_id]->
                hash_table_id, p_p_handle, p_ecode );
        
        *p_p_handle = GL_NULL;
        *p_is_more_ids_to_match = GL_FALSE;
        *p_num_of_match = GL_NULL;
        return GL_SUCCESS;
    }

    /*
     * Find out the prefix and suffix from the string to match.
     */ 
    if(GL_FAILURE == gl_stm_strncpy(p_prefix_str, p_string_to_match, 
        (gw_U32bit)(p_start_ptr-p_string_to_match),  p_ecode))
    {
        return GL_FAILURE;
    }
    if(GL_FAILURE == gl_stm_strcpy(p_suffix_str, p_end_ptr+1, p_ecode))
    {
        return GL_FAILURE;
    }

    for(i=start_range, j=0; (j<*p_num_of_match) && (i<=end_range); i++)
    {
        if(GL_FAILURE == gl_stm_strcpy(p_str_id, p_prefix_str, p_ecode))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_stm_itoa(i, p_num_str, p_ecode))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_stm_strcat(p_str_id, p_num_str, p_ecode))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_stm_strcat(p_str_id, p_suffix_str, p_ecode))
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = (gl_pvoid_t)p_str_id;
        /*
         * Search the map from hash-table, by the given string-id pointer.
         */ 
        if(GL_FAILURE == gl_htm_get_key_ref_from_hash_table(g_a_p_sim_db
            [sim_db_id]->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id,
            &hash_key, (gl_pvoid_t *)(&p_ln_node), p_ecode))
        {
            /*
             * As string-id obtained after expansion may not be stored in 
             * database.
             */ 
            if(GL_HTM_ERR_KEY_NOT_FOUND != *p_ecode)
            {
                *p_ecode = GL_SIM_ERR_INTERNAL;
                return GL_FAILURE;
            }
        }
        /*
         * Get the index of linear for the given node pointer,
         * the index is the numeric-id for the map.
         */
        numeric_id = (p_ln_node - g_a_p_sim_db[sim_db_id]->a_ln_array.
                a_ln_nodes);

        p_hash_node_handle = g_a_p_sim_db[sim_db_id]->a_ln_array.a_ln_nodes
            [numeric_id].p_hash_node_handle;
        /*
         * Search the map from hash-table, by the key node handle.
         */ 
        if(GL_FAILURE == gl_htm_get_key_ref_by_key_node_handle(g_a_p_sim_db
            [sim_db_id]->user_id, g_a_p_sim_db[sim_db_id]->hash_table_id, 
            p_hash_node_handle, &temp_hash_key, (gl_pvoid_t *)(&p_temp_ln_node)
            , p_ecode))
        {
            *p_ecode = GL_SIM_ERR_INTERNAL;
            return GL_FAILURE;
        }

        is_key_selected = GL_TRUE;

        /*
         * If used given filter function pointer is non-null, 
         * then call the function to fiter the selection of string.
         */ 
        if(GL_NULL != p_str_id_filter_fn)
        {
            is_key_selected = GL_FALSE;
            
            if(GL_FAILURE == p_str_id_filter_fn( p_str_id_filter_fn_argu,
                (gw_U8bit *)(temp_hash_key.p_user_def_id),
                p_ln_node->p_str_id_info, numeric_id + 1, &is_key_selected,
                p_ecode))
            {   
                return GL_FAILURE;
            }
        }

        if(GL_TRUE == is_key_selected)
        {
            /*
             * The string-id is stored in the returned hash-key.
             */
            a_p_string_id[j] = (gw_U8bit *)(temp_hash_key.p_user_def_id);

            /*
             * Add 1 to numeric-id as numeric-id to be returned to user should
             * be in range of 1 - max_num_of_map, 
             * but the numeric-id used for internal processing is in range
             * 0 - (max_num_of_map - 1).
             */ 
            a_numeric_id[j] = numeric_id + 1;
            a_p_str_id_info[j] = p_ln_node->p_str_id_info;
            j++;
        }
    }

    if(i > end_range)
    {
        /*
         * As start_range is more than end-range so we have reached at the 
         * end of searching.
         * So Free the memory allocated for p_map_handle.
         */ 
        gl_htm_terminate_get_key_list_search( g_a_p_sim_db[sim_db_id]->
                hash_table_id, p_p_handle, p_ecode );
        
        *p_p_handle = GL_NULL;
        *p_is_more_ids_to_match = GL_FALSE;
        *p_num_of_match = j;
        return GL_SUCCESS;
    }
    else if(j == *p_num_of_match)
    {
        /*
         * Set the start range in the p_map_handel, to start from the next 
         * string in the next call for the same string to match.
         */
        p_map_handle->start_range = i;
        *p_is_more_ids_to_match = GL_TRUE;
        *p_num_of_match = j;
        return GL_SUCCESS;
    }
    else
    {
        *p_ecode = GL_SIM_ERR_INTERNAL;
        return GL_FAILURE;
    }
}
