/***************************************************************************
*
*    FUNCTION    :    Header file for Generic Library - Hash Table 
*                     implementation.  
****************************************************************************
*
*    FILENAME    :    gl_htm.h
*
*    DESCRIPTION :    This file contains the constants and function 
*                     declarations of hash table sub-module of the generic 
*                     library implemntation. The functions include those for 
*                     creation of hash table and operations such as addition, 
*                     subtraction of keys/elements etc. (header file)
*
* DATE             NAME        REFERENCE        REASON
*-------     -------------    ----------    ----------------------
*
* Oct'99        Kapil Nayar    HSS:40600026    Generic Libraries
*                              HSS:40600036
* 18/10/2001    Ravi Jindal    SPR-2788        Packaging of Genlib-Code.
* 17/11/2001    Ravi Jindal    SPR-3107        Incoprate the defects and change
*                                              cosmmetics of file.
* 30/11/2001    Ravi Jindal    SPR-2788        Re-Packaging of Genlib-Code.
* 18/12/2001    Ravi Jindal    SPR-3363        Compilation with CC.
* 
* 27/12/2001    Ravi Jindal    SPR-3414        Intial Development for Rel-1.0.
*                                           
* 27/12/2001    Prashant Jain  SPR-3414        Incorporated Code Review
*                                              Comments.
*
* 05/04/2002    Ravi Jindal    SPR-3414        Enhancements for Rel-1.0.
*  
* 10/09/2002    Ravi Jindal    SPR-4841        To change the reserve memory
*                                              function prototype for
*                                              user_mem_info.
*COPYRIGHT , Aricent, 2006
****************************************************************************/

#ifndef _GL_HTM_H_
#define _GL_HTM_H_

#include "gl_htm_interfaces.h"
#include "gl_ulm_stats_intrn_types.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Structure for the hashed key node in the hash list.
 */
typedef struct gl_htm_key_node_s
{
    /*
     * Pointer to the previous key in the hashed list.
     */
    struct gl_htm_key_node_s    *p_prev; 
    /*
     * Pointer to the next key in the hashed list.
     */
    struct gl_htm_key_node_s    *p_next;
    /*
     * key, to be stored in hash-table.
     */
    gl_hash_key_ut              key;
    /*
     * To store the hash value of key for better searching.
     */
    gw_U32bit                   key_hash_val;
    /*
     * Pointer to the hashed element stored at the key.
     */
    gl_pvoid_t                  p_elem;
    /*
     * Reference count to key. 
     * It get increased at fetch key and
     * decrease at each release key for respective key.
     * before removing key, if reference count > 0 
     * key is not removed from hash-table.
     */ 
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT    
    gw_U8bit                    ref_count;
#endif
    
}gl_htm_key_node_t;



/*
 * Structure for the bucket array in the hash table.
 */
typedef struct gl_htm_bucket_array_s
{
    /*
     * Pointer to the first key node in the bucket.
     */
    gl_htm_key_node_t          *p_first;
    /*
     * Pointer to the last key node in the bucket.
     */
    gl_htm_key_node_t          *p_last;
    /*
     * Total number of keys currently stored in the bucket.
     */
    gw_U32bit                   key_qty;
    /*
     * Mutex to protect mdification of bucket data structures
     * in multi-threaded environment.
     */
#ifdef GL_TOTAL_MT_SAFE
    GL_MUTEX_ID                 bucket_mutex;
#endif
    
}gl_htm_bucket_array_t;



/*
 * Structure for the hash table.
 */
typedef struct gl_htm_table_s
{
    /*
     * Maximum number of valid keys that can be stored in Hash Table.
     */
    gw_U32bit                   max_key_qty;
    /*
     * To store type of key, used for hash-table.
     */
    gw_U8bit                    key_type;
    /*
     * Task_id of the owner of the hash table.
     */
    gw_U32bit                   owner_user_id; 
    /*
     * Maximum number of buckets in the hash table.
     */
    gw_U32bit                   bucket_qty; 
    /*
     * Pointer to the hash function. 
     * For this hash table each key is hashed with given hash function.
     */
    gl_htm_hash_fn_t            p_hash_fn;
    /*
     * Pointer to the key comparison function. 
     * For this hash table two keys in a bucket are compared with given
     * key comparison function.
     */
    gl_htm_key_cmp_fn_t         p_key_cmp_fn;
    /*
     * Pointer to the Key Element Release Function. 
     * This function would be called internally by Hash Table Manager
     * to free the Element Buffers associated with the respective keys
     * while executing the routine "gl_htm_remove_all_keys_from_hash_table". 
     */
    gl_htm_free_key_elem_fn_t   p_elem_free_fn;
    /*
     * Pointer to an array of size bucket_qty.
     */
    gl_htm_bucket_array_t       *a_bucket_array; 
    /* 
     * Set when GL_TRUE - table created 
     * reset when GL_FALSE - table deleted.
     */
    gw_U8bit                    alloc_flag;
    /*
     * Pointer to Hash Table Specific Argument.
     * Hash Table Manager only Stores this pointer and return in
     * "gl_htm_resize_hash_table" and argument to Hashing Function.
     */ 
    gl_pvoid_t                  p_hash_arg;
    /*
     * The "max_buket_size_thrsh" would specify the 
     * the maximum allowable number of key entries in
     * any buket.
     */
    gw_U32bit                   max_buket_size_thrsh;
    /*
     * Top store information required for Memory allocation/deallocation.
     */
    gw_U32bit                   user_mem_info;
    
    
}gl_htm_table_t;



/*
 * Structure used for Hash-Walk Proto environment Information.
 */
typedef struct gl_htm_hash_walk_handle_s
{
    /*
     * To store the bucket index and Key node index,
     * from where next call should proceed.
     */
    gw_U32bit                   bucket_number;
    gw_U32bit                   key_node_number;
    
}gl_htm_hash_walk_handle_t;


/*************************************************************
 * Global Variables DECLARATION.
 ************************************************************/


extern  gl_htm_table_t          *g_a_p_hashtable[GL_HTM_MAX_HASH_TABLE];


extern gl_boolean               htm_mgr_init_flag;


/*************************************************************
 * LOCAL FUNCTIONS DECLARATION.
 ************************************************************/

extern  gw_U32bit
gl_htm_def_numeric_key_hash_fn

    _ARGS_( (   gl_hash_key_ut            *p_key,
                gl_pvoid_t                p_hash_arg        ) );
    


extern  gl_return_t
gl_htm_def_numeric_key_cmp_fn

    _ARGS_( (   gl_hash_key_ut            *p_key_to_be_compared,
                gl_hash_key_ut            *p_node_key,        
                gw_U32bit                 key_hash_val,
                gw_U32bit                 node_key_hash_val     ) );


    
extern  gw_U32bit
gl_htm_def_string_key_hash_fn

    _ARGS_( (   gl_hash_key_ut            *p_key,
                gl_pvoid_t                p_hash_arg        ) );
    


extern  gl_return_t
gl_htm_def_string_key_cmp_fn

    _ARGS_( (   gl_hash_key_ut            *p_key_to_be_compared,
                gl_hash_key_ut            *p_node_key,
                gw_U32bit                 key_hash_val,
                gw_U32bit                 node_key_hash_val     ) );

    

extern  gl_return_t
htm_allocate_key_node

    _ARGS_( (   gl_hash_table_t           hash_table_id,
                gl_htm_key_node_t         **p_p_key_node,
                gl_hash_key_ut            *p_key,
                gl_error_t                *p_ecode          ) );
    


extern  gl_return_t
htm_remove_key_node

    _ARGS_( (   gl_hash_table_t           hash_table_id,
                gl_htm_bucket_array_t     *p_bucket,
                gl_htm_key_node_t         *p_temp_node,
                gl_error_t                *p_ecode          ) );
    




extern gl_return_t
gl_htm_reserve_memory

    _ARGS_( (   gw_U32bit                 user_id,
                gw_U32bit                 *p_user_mem_info,
                gw_U32bit                 buf_qty,
                gw_U32bit                 buf_size,
                gl_error_t                *p_ecode      ) );

    
     
extern gl_pvoid_t
gl_htm_get_memory_buffer

    _ARGS_( (   gw_U32bit                 user_id,
                gw_U32bit                 user_mem_info,
                gw_U32bit                 buf_size,
                gl_error_t                *p_ecode      ) );


    
    
    
extern gl_return_t
gl_htm_release_memory_buffer

    _ARGS_( (   gw_U32bit                 user_id,
                gw_U32bit                 user_mem_info,
                gl_pvoid_t                p_buffer,
                gl_error_t                *p_ecode      ) );

    
    

extern gl_return_t
gl_htm_free_memory

    _ARGS_( (   gw_U32bit                 user_id,
                gw_U32bit                 user_mem_info,
                gw_U32bit                 buf_qty,
                gw_U32bit                 buf_size,
                gl_error_t                *p_ecode      ) );



#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif

