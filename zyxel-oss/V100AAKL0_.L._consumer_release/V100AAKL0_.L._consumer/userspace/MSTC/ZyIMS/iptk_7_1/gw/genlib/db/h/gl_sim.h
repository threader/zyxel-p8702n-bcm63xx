/***************************************************************************
*
*    FUNCTION    :    Header file for Generic Library - String Identifier  
*                     Mapping Manager implementation.  
****************************************************************************
*
*    FILENAME    :    gl_sim.h
*
*    DESCRIPTION :    This file contains the constants and function 
*                     declarations of string identifier Mapping sub-module of
*                     the generic library implemntation. The functions include
*                     those for creation of database and operations such as 
*                     addition, subtraction of map etc. (header file)
*
* DATE             NAME        REFERENCE        REASON
*-------     -------------    ----------    ----------------------
*
* 27/05/2002    Ravi Jindal     SPR-3960        Intial Draft, Enhancements 
*                                               for Rel-1.0.
*  
* 09/08/2002    Ravi Jindal     SPR-4808        To add the range based 
*                                               searching. 
*
* COPYRIGHT , Aricent, 2006
* 
****************************************************************************/

#ifndef _GL_SIM_H_
#define _GL_SIM_H_

#include "gl_sim_interfaces.h"
#include "gl_htm.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Structure for the linear array nodes.
 */
typedef struct gl_sim_ln_array_node_s
{
    /*
     * Pointer to the next free index in linear array.
     */
    gw_U32bit                   next_free_index; 
    /*
     * Pointer to the previous free index in linear array.
     */
    gw_U32bit                   prev_free_index; 
    /*
     * Pointer to the hash-node handle in which string-id is stored.
     * It is retuned by the "gl_htm_add_key_to_hash_table" API.
     */
    gl_pvoid_t                  p_hash_node_handle;
    /*
     * Pointer to user information structure.
     * Memory for the information structure is allocated by the user.
     */
    gl_pvoid_t                  p_str_id_info;
    
}gl_sim_ln_array_node_t;



/*
 * Structure for the linear array.
 */
typedef struct gl_sim_ln_array_s
{
    /*
     * Pointer to the first free index in the linear array;
     */
    gw_U32bit                   first_free_index;
    /*
     * Pointer to the last free index in the linear array;
     */
    gw_U32bit                   last_free_index;
    /*
     * Pointer to the linear array nodes.
     */
    gl_sim_ln_array_node_t      *a_ln_nodes;
    
}gl_sim_ln_array_t;



/*
 * Structure for the String Identifier Mapping Database.
 */
typedef struct gl_sim_db_s
{
    /*
     * User_id of the owner of the database.
     */
    gw_U32bit                   user_id; 
    /*
     * Maximum number of valid maps that can be stored in Database.
     */
    gw_U32bit                   max_num_of_map;
    /*
     * Maximum number of buckets in the hash table, used for database.
     */
    gw_U32bit                   max_num_of_bucket; 
    /*
     * Pointer to the user information structure Memory Release Function.
     * This function would be called internally by String Identifier Mapping 
     * Manager to free the string id info associated with the respective 
     * string-ids, while executing the routine "gl_sim_remove_all_maps".
     */
    gl_sim_free_mem_fn_t        p_free_mem_fn;
    /*
     * Hash-Table id, Hash-table is used to store string-id to numeric-id 
     * mapping.
     */
    gl_hash_table_t             hash_table_id;
    /*
     * Linear array structure, used to store numeric-id to string-id mapping.
     */
    gl_sim_ln_array_t           a_ln_array;
    /*
     * Number of maps currently stored in the dataabse.
     */
    gw_U32bit                   curr_num_of_map;
    /*
     * If str_id_case_insensitive = GL_TRUE, string-ids are case-insensitive.
     * If str_id_case_insensitive = GL_FALSE, string-ids are case-sensitive.
     */ 
    gw_boolean                  str_id_case_insensitive;
    /*
     * Mutex, to use data-structure in MT-Safe Enviornment.
     */ 
#ifdef GL_MT_ENABLED
    GL_MUTEX_ID                 sim_db_mutex;
#endif
    
    
}gl_sim_db_t;


/*
 * Structure used in the API "gl_sim_get_string_id_map_list".
 * This structure is used in the function "gl_sim_get_string_id_map_list".
 */
typedef struct gl_sim_filter_fn_argument_s
{
    /*
     * Pointer to string to be matched;
     */ 
    gw_U8bit                    *p_str_to_match;
    /*
     * Pointer to user defined function argument, to filter keys.
     */ 
    gl_pvoid_t                  p_user_fn_argu;
    /*
     * Pointer to user defined function, to filter keys.
     */ 
    gl_sim_str_id_filter_fn_t   p_user_fn;
    /*
     * Pointer to liner array nodes, required to calculate numeric_id.
     */
    gl_sim_ln_array_node_t      *p_ln_node_array;

}gl_sim_filter_fn_argument_t;

/*
 * Struncture used to store information for range searching of string.
 */ 
typedef struct gl_sim_map_range_handle_s
{
    /*
     * Value of range start and range end.
     */ 
    gw_U32bit                   start_range;
    gw_U32bit                   end_range;
    /*
     * Index of the prefix and suffix of tremination, prefix and suffix 
     * remians same for all strings during range expansion.
     */ 
    gw_U32bit                   start_ptr_index;
    gw_U32bit                   end_ptr_index;
}gl_sim_map_range_handle_t;
    




/**********************************************
 * Extern Declaration for Array of databases. 
 *********************************************/

extern gl_sim_db_t             *g_a_p_sim_db[GL_SIM_MAX_DB];

extern gl_boolean               sim_mgr_init_flag;


/**********************************************
 * LOCAL FUNCTIONS DECLARATION.
 *********************************************/

gw_U32bit
sim_get_free_index

    _ARGS_( (   gl_sim_ln_array_t       *p_ln_array,
                gw_U32bit               max_num_of_map       ) );
    

void
sim_set_index_free

    _ARGS_( (   gl_sim_ln_array_t       *p_ln_array,
                gw_U32bit               numeric_id,
                gw_U32bit               max_num_of_map      ) );


gl_return_t
sim_set_index_allocated

    _ARGS_( (   gl_sim_ln_array_t       *p_ln_array,
                gw_U32bit               numeric_id,
                gw_U32bit               max_num_of_map      ) );


gl_return_t
sim_string_id_filter_func

    _ARGS_( (   gl_pvoid_t              p_strinig_id_filter_fn_argu, 
                gl_hash_key_ut          *p_hash_key, 
                gl_pvoid_t              p_ln_array_node,
                gl_htm_key_action_et    *p_is_key_to_include_in_list,
                gl_error_t              *p_ecode    ) );


gl_return_t
sim_memory_free_func    

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_key_ut          *p_key,
                gw_U8bit                key_type,
                gl_pvoid_t              p_element,
                gl_error_t              p_ecode           ) );

gl_return_t
sim_get_map_list_by_range

    _ARGS_( (   gl_sim_db_id_t            sim_db_id,
                gl_pvoid_t                *p_p_handle,
                gw_U8bit                  *p_string_to_match, 
                gw_U8bit                  *p_num_of_match, 
                gl_sim_str_id_filter_fn_t p_str_id_filter_fn, 
                gl_pvoid_t                p_str_id_filter_fn_argu, 
                gw_U8bit                  *a_p_string_id[], 
                gl_pvoid_t                a_p_str_id_info[], 
                gw_U32bit                 a_numeric_id[], 
                gw_boolean                *p_is_more_ids_to_match,
                gl_error_t                *p_ecode          ) );


#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif

