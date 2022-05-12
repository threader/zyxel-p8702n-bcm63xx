/*******************************************************************************
*
*    FUNCTION    :    Header file for Generic Library - Hash Table 
*                     implementation.  
*******************************************************************************
*
* FILENAME    :    gl_htm_interfaces.h
*
* DESCRIPTION :    This file contains the constants and function 
*                  declarations of hash table sub-module of the generic 
*                  library implemntation. The functions include those for 
*                  creation of hash table and operations such as addition, 
*                  subtraction of keys/elements etc.
*
* DATE       NAME           REFERENCE     REASON
*----------- -------------- ----------    -------------------------------------
* 29/11/2001 Ravi Jindal    SPR-2788      Packaging of Genlib-Code.
*
* 06/12/2001 Prashant Jain  SPR-3363      Compilation with CC.
*
* 27/12/2001 Ravi Jindal    SPR - 3414    Intial Development.
*
* 27/12/2001 Prashant Jain  SPR - 3414    Incorporated Code Review Comments.
*
* 05/04/2002 Ravi Jindal    SPR - 3414    Enhancements for Rel-1.0.
*    
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

#ifndef _GL_HTM_INTERFACES_H_
#define _GL_HTM_INTERFACES_H_

#include "gl_config.h"
#include "gl_types.h"
#include "gl_error.h"
#include "gl_os_port.h"
#include "gl_gscim_interfaces.h"
#include "gl_ulm_interfaces.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Union is used for key type to be stored in the hash-table
 * As if hash table is created for numeric keys, then only 
 * key is stored by assignment but for string or other type 
 * of keys, key are stored by user and HTM only stores the 
 * pointer to key.
 * So key for hash table can be a union of U32bit or pvoid.
 */

/*
 * In all the get-key API, for hash-key user should give pointer 
 * to a variable of hash-key union type.
 * The memory for union is to be allocated by user, HTM only 
 * fills the value of union members to return the key.
 */

typedef union gl_hash_key_u
{
    gw_U32bit        numeric_id;
    gl_pvoid_t       p_user_def_id;
}gl_hash_key_ut;
  
/*
 * Enum values used to set value of key-type.
 * Key_type is used to indicate whether numeric_id or p_user_def_id
 * member of union is valid for given key.
 */
typedef enum gl_htm_key_type_e
{
    GL_HTM_USER_DEFINED_KEY_TYPE = 0,
    GL_HTM_NUMERIC_KEY_TYPE
        
}gl_htm_key_type_et;


/*
 * Enum values used to set the action corresponding to the key by 
 * callback function or jkey select function.
 */
typedef enum gl_htm_key_action_e
{
    GL_HTM_NO_ACTION                = 0,
    GL_HTM_INCLUDE_KEY_IN_RET_LIST  = 1,
    GL_HTM_DELETE_KEY_NODE          = 2

}gl_htm_key_action_et;
        
/*
 * Enum values used to set value of default hash function type
 * based on type of key.
 * hash_func_type is used configure default Hashing function
 * and Key Comparision function, provided by Hash Table Manager.
 */
typedef enum
{
    GL_HTM_HASH_FUNC_FOR_USER_DEFINED_KEYS = 0,
    GL_HTM_HASH_FUNC_FOR_NUMERIC_KEYS,
    GL_HTM_HASH_FUNC_FOR_STRING_KEYS
        
}gl_htm_hash_func_type_et;

/*
 * Typedef for key hash function.
 */
typedef gw_U32bit (*gl_htm_hash_fn_t)

    ( gl_hash_key_ut   *p_key,
      gl_pvoid_t       p_hash_arg );    

/* 
 * Typedef for key comparison function.
 */
typedef gl_return_t (*gl_htm_key_cmp_fn_t)

    ( gl_hash_key_ut   *p_key_to_be_compared, 
      gl_hash_key_ut   *p_node_key,
      gw_U32bit        key_hash_val,
      gw_U32bit        node_key_hash_val    );    


/* 
 * Typedef for key and related element buffer memory free function.
 * The key type is passed to the the interface as User should free
 * Memory of key only, if key type is "GL_HTM_USER_DEFINED_KEY_TYPE".
 */
typedef gl_return_t (*gl_htm_free_key_elem_fn_t)

    ( gw_U32bit        user_id,
      gl_hash_key_ut   *p_key,
      gw_U8bit         key_type,
      gl_pvoid_t       p_elem_buf_ptr,
      gl_pvoid_t       p_hash_arg,
      gl_error_t       *p_ecode );


/*
 * If user want to filter the keys to be returned in hash-walk function
 * based on key or data element pointer then user hash to define a 
 * function and pass the pointer to function in 
 * "gl_htm_get_key_list_from_hash_table". 
 * Key is included, if this returmn value of flag = GL_TRUE.
 * 
 * If not required then value can be GL_NULL, then all the keys are returned.
 */
typedef gl_return_t (*gl_htm_key_select_fn_t)

    ( gl_pvoid_t            p_key_select_fn_argu,
      gl_hash_key_ut        *p_key,
      gl_pvoid_t            p_elem_buffer,
      /*
       * If return value of flag = GL_HTM_INCLUDE_KEY_IN_RET_LIST, 
       * then key is included in return list.
       * else for any other value of flag key is not included in returned list.
       * and this procedure repeats till not reached to last key.
       */  
      gl_htm_key_action_et  *p_is_key_to_include_in_list,
      gl_error_t            *p_ecode    );
      

typedef gl_htm_key_select_fn_t  gl_htm_key_callback_fn_t;


/**********************************************************************
 * External Interfaces
 **********************************************************************/
    
/*
 * To initialize data structure for all hash tables with default values.
 */ 
extern gl_return_t
gl_htm_init( void );

/*
 * To create a hash table.
 */ 
extern gl_return_t
gl_htm_create_hash_table

    _ARGS_( (
                /*
                 * Identifier to the owner of hash table.
                 */ 
                gw_U32bit               user_id,
                /*
                 * Id of hash table, used for further operations on hash-table.
                 */ 
                gl_hash_table_t         *p_hash_table_id,
                /*
                 * Maximum number of keys that can be stored in  hash-table.
                 */ 
                gw_U32bit               max_key_qty,
                /*
                 * Maximum number of buckets allowed for hash-table.
                 */ 
                gw_U32bit               max_num_buckets,
                /*
                 * As if key type is NUMERIC, then to store key dynamic memory 
                 * is not required.
                 */ 
                gw_U8bit                key_type,
                /*
                 * If hash_func_type = GL_HTM_HASH_FUNC_FOR_NUMERIC_KEYS, 
                 * then hash function to process integer keys is used.
                 * If hash_func_type = GL_HTM_HASH_FUNC_FOR_STRING_KEYS
                 * then hashing function to process string keys is used.
                 */ 
                 gw_U8bit               hash_func_type,
                /*
                 * Pointer to hash function, used to hash each key.
                 */ 
                gl_htm_hash_fn_t        p_hash_fn,
                /*
                 * Pointer to the Key and key-element memory release Function.
                 * This function would be called internally by Hash Table 
                 * Manager to free memory of key (if key type is User defined)
                 * and the Element Buffers associated with the respective keys
                 * while executing the routine 
                 * "gl_htm_remove_all_keys_from_hash_table".
                 */
                gl_htm_free_key_elem_fn_t    p_elem_free_fn,
                /*
                 * Hash function Argument Provided by the User, if any.
                 * It would be passed as it is to the hash function at the
                 * time of its invocation. The memory for this variable 
                 * would be allocated by the user and would have to be freed
                 * by the user whne the has table is deleted.
                 */ 
                gl_pvoid_t                  p_hash_arg,
                /*
                 * Pointer to key comparison functiuon, used to compare keys
                 * in a bucket.
                 */ 
                gl_htm_key_cmp_fn_t         p_key_cmp_fn,

                gl_error_t                  *p_ecode           ) );

                

/*
 * To add key to hash-table.
 */ 
extern gl_return_t
gl_htm_add_key_to_hash_table

    _ARGS_( (
                gw_U32bit               user_id,
        
                gl_hash_table_t         hash_table_id,
         
                /*
                 * Pointer to key, required to be hashed.
                 * Hash Table Manager whould store the value of key only if
                 * key-type is numeric. For user-defined key type the memory
                 * of key is to be allocated by the user and in hash-table 
                 * only pointer to key is stored.
                 */ 
                gl_hash_key_ut          *p_key,
                /*
                 * Pointer to data element associated with key.
                 * The memory for the element would always be 
                 * allocated by the User. The hash Table node 
                 * would just stire this pointer variable. This
                 * memory would be freed by the user after retrieving
                 * it through the "gl_htm_remove_key_from_hash_table".
                 */ 
                 gl_pvoid_t              p_element,
                /*
                 * Pointer top key node handle returned by Hash Table Manager
                 * The returned pointer to key node handle can be used by
                 * user to get key and element pointer by API
                 * "gl_htm_get_key_by_key_node_handle".
                 */
                gl_pvoid_t              *p_p_key_node_handle,
        
                gl_error_t              *p_ecode            ) );

                

/*
 * To fetch a key from hash-table.
 * This function increases the reference count of the key node by 1.
 * The fetched key should be released after completeing all processing
 * related to required key.
 */ 
extern gl_return_t
gl_htm_get_key_ref_from_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_hash_key_ut          *p_key,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode            ) );


/*
 * To fetch a key and element pointer from hash-table.
 * Key is traced by key node handle in which key and element pointers
 * are stored, key node handle is returned in "gl_htm_add_key_to_hash_table"
 * This function increases the reference count of the key node by 1.
 * The fetched key should be released after completeing all processing
 * related to required key.
 */ 
extern gl_return_t
gl_htm_get_key_ref_by_key_node_handle

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_pvoid_t              p_key_node_handle,
                gl_hash_key_ut          *p_key,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode            ) );


    
/*
 * As this API is usefull only if Key Reference count is used.
 */
#ifdef GL_HTM_ENABLE_KEY_REF_COUNT
/*
 * To release the key in hash-table.
 * This function decreases the reference count of the key node by 1.
 * If Reference count after decrement = 0, then delete the key node from 
 * Hash-table and return the associated pointer to element in p_p_element.
 * If Reference count after decrement > 0, then do not delete the key node 
 * from Hash-table and return GL_NULL in p_p_element.
 * 
 */ 
extern gl_return_t
gl_htm_remove_key_ref_from_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_hash_key_ut          *p_key,
                gl_pvoid_t              *p_p_key_buf,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode            ) );

#endif


/*
 * This function remove the key node from hash-table
 * and return the pointer to key, as memory of key and 
 * data element is allocated by user, it is to be released by the user.
 * If reference count of key node > 0, donot remove the key node.
 */ 
extern gl_return_t
gl_htm_remove_key_from_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_hash_key_ut          *p_key,
                gl_pvoid_t              *p_p_key_buf,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode            ) );
    

/*
 * To replace the key maped data element in hash-table.
 */ 
extern gl_return_t
gl_htm_replace_key_elem_in_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_hash_key_ut          *p_key,
                gl_pvoid_t              p_new_element,
                gl_pvoid_t              *p_p_old_element,
                gl_error_t              *p_ecode            ) );
    

    
/*
 * To remove all keys from hash table.
 */ 
extern gl_return_t
gl_htm_remove_all_keys_from_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_error_t              *p_ecode           ) );
    

    
/*
 * To fetch first key(valid key in first non-empty bucket) from hash table.
 * This function increases the reference count of fetched key node by 1.
 * The fetched key should be released after completeing all processing
 * related to required key.
 */ 
extern gl_return_t
gl_htm_get_first_key_ref_from_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_hash_key_ut          *p_key,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode          )  );
    

    
/*
 * To fetch next key(key next in same bucket or first key of next 
 * non empty bucket) from hash table.
 * This function increases the reference count of fetched key node by 1.
 * The fetched key should be released after completeing all processing
 * related to required key.
 */ 
extern gl_return_t
gl_htm_get_next_key_ref_from_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                /*
                 * Pointer to key whose next key is required.
                 */ 
                gl_hash_key_ut          *p_key,
                gl_hash_key_ut          *p_next_key,
                gl_pvoid_t              *p_p_next_element,
                gl_error_t              *p_ecode            ) );
    

    
/*
 * To fetch previous key(key previous in same bucket or last key of previous 
 * non empty bucket) from hash table.
 * This function increases the reference count of fetched key node by 1.
 * The fetched key should be released after completeing all processing
 * related to required key.
 */ 
extern gl_return_t
gl_htm_get_prev_key_ref_from_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_hash_key_ut          *p_key,
                /*
                 * Pointer to key whose previous key is required.
                 */ 
                gl_hash_key_ut          *p_prev_key,
                gl_pvoid_t              *p_p_prev_element,
                gl_error_t              *p_ecode            ) );

    

/*
 * To Traverse all the keys stored in Hash-Table,
 * the keys should be returned in a set for given number of keys.
 * It returns the keys based on value of flag 'start_from_first_key'
 */ 
extern gl_return_t
gl_htm_get_key_list_from_hash_table

    _ARGS_( (   gw_U32bit               user_id, 
                
                gl_hash_table_t         hash_table_id,
                /*
                 * If a new hash-walk is to start, set p_hash_walk_handle to
                 * GL_NULL and to start from the last key returned by the 
                 * previous call call the API with the same handle with which
                 * it is called previously.
                 */ 
                gl_pvoid_t              *p_p_hash_walk_handle,
                /*
                 * As an IN parameter, indicates the number of keys to be
                 * traversed in current call to API.
                 * As OUT parameter, indicates the number of keys returned
                 * in the key array and element pointer array.
                 */ 
                gw_U8bit                *p_num_of_nodes,
                /*
                 * It is pointer to user defined function, which is invoked
                 * for each key with pointer to user given argument, pointer
                 * to key, key type and pointer to data element corresponding
                 * to key. 
                 * If the function returns GL_SUCCESS, then key and 
                 * respective data element pointer is included in the returned
                 * list of keys.
                 * If function returns GL_FAILURE, then key and data element 
                 * pointer is not included in the returned list.
                 *
                 * If user does not want to use this function, then value of
                 * function pointer can be GL_NULL. For GL_NULL value of 
                 * function pointer function is not invoked inside the 
                 * function.
                 */ 
                gl_htm_key_select_fn_t  p_key_select_fn,
                /*
                 * Argument to the key select function. This pointer is passed
                 * as it is to the select key function at invocation of 
                 * function. It can be GL_NULL (if not required).
                 */ 
                gl_pvoid_t              p_key_select_fn_argu,
                /*
                 * pointer to memory of size 
                 * ( *p_num_of_nodes * sizeof (gl_hash_key_ut)).
                 * Keys to be returned are filled in this memory.
                 */ 
                gl_hash_key_ut          a_khash_key[], 
                /*
                 * pointer to memory of size
                 * ( *p_num_of_nodes * sizeof (gl_pvoid_t)).
                 * Pointer to data elements correspondinng to keys to be 
                 * returned are filled in this memory.
                 */
                gl_pvoid_t              a_p_element[],
                /*
                 * Flag to indicate whether more keys are still
                 * left to traverse (GL_TRUE).
                 */ 
                gw_boolean              *p_is_more_key_to_traverse,

                gl_error_t              *p_ecode      ) );
    

/*
 * To terminate a hash-walk, initiated by API
 * "gl_htm_get_key_list_from_hash_table"
 */
extern gl_return_t
gl_htm_terminate_get_key_list_search 

    _ARGS_( (   gl_hash_table_t     hash_table_id, 
                gl_pvoid_t          *p_p_hash_walk_handle, 
                gl_error_t          *p_ecode            ) );
                       
                    
                


/*
 * To Traverse all the keys stored in Hash-Table in one shot.
 */ 
extern gl_return_t
gl_htm_hash_walk_in_one_shot

    _ARGS_( (   gw_U32bit               user_id, 
                
                gl_hash_table_t         hash_table_id,
                /*
                 * It is pointer to user defined function, which is invoked
                 * for each key with pointer to user given argument, pointer
                 * to key, key type and pointer to data element corresponding
                 * to key. 
                 * If the function returns GL_SUCCESS, then key and 
                 * respective data element pointer is included in the returned
                 * list of keys.
                 * If function returns GL_FAILURE, then key and data element 
                 * pointer is not included in the returned list.
                 *
                 * If user does not want to use this function, then value of
                 * function pointer can be GL_NULL. For GL_NULL value of 
                 * function pointer function is not invoked inside the 
                 * function.
                 */ 
                gl_htm_key_callback_fn_t  p_key_callback_fn,               /*
                 * Argument to the key select function. This pointer is passed
                 * as it is to the select key function at invocation of 
                 * function. It can be GL_NULL (if not required).
                 */ 
                gl_pvoid_t              p_key_callback_fn_argu,
                /*
                 * It indicates the number of keys nodes deleted in the 
                 * current hash-walk (key node is deleted only if the return 
                 * values of flag by callback function are satified).
                 */ 
                gw_U32bit                *p_num_of_nodes_deleted,
                
                gl_error_t              *p_ecode      ) );


/*
 * To resize the hash-table.
 */ 
extern gl_return_t
gl_htm_resize_hash_table

    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gw_U32bit               max_buket_size_thrsh,
                
                /*
                 * The "max_buket_size_thrsh" would specify the 
                 * the maximum allowable number of key entries in
                 * any buket.
                 */
                gw_U32bit               *p_new_num_buckets,
                /*
                 * The variable "p_new_num_buckets" would return
                 * the number of buckets in the specified Hash
                 * Table after the RESIZE operation is done.
                 */        
                gl_error_t              *p_ecode            ) );



/*
 * To get the number of key node currently present in hash-table.
 */ 
extern gl_return_t
gl_htm_get_key_count_of_hash_table

    _ARGS_( (   gl_hash_table_t         hash_table_id,
                gw_U32bit               *p_number_of_keys,
                gl_error_t              *p_ecode            ) );


    
/*
 * To check if hash-table is empty or not.
 */ 
extern gl_boolean_t
gl_htm_is_hash_table_empty

    _ARGS_( (   gl_hash_table_t         hash_table_id,
                gl_error_t              *p_ecode            ) );


    
/*
 * To delete the hash-table.
 * If hash-table contains a key in any bucket, donot delete the hash table.
 */ 
extern gl_return_t
gl_htm_delete_hash_table


    _ARGS_( (   gw_U32bit               user_id,
                gl_hash_table_t         hash_table_id,
                gl_pvoid_t              *p_p_hash_arg,
                gl_error_t              *p_ecode            ) );


    
/*
 * To delete all the hash tables.
 * If forceful_flag=GL_FALSE, donot delete the hash tables which are not empty.
 * otherwise delete all empty and non-empty hash-tables. 
 */ 
extern gl_return_t
gl_htm_destroy_all_hash_tables

    _ARGS_( (   gw_U8bit                is_forceful_cleanup,
                gl_error_t              *p_ecode            ) );


    
    

    
#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif

