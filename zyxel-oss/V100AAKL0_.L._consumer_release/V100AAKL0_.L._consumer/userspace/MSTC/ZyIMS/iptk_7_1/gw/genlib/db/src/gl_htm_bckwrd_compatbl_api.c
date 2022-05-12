/*****************************************************************************
*
*    FUNCTION    : Source file for Generic Library - Hash Table Manager
*                  backward compatibility of Rel-1.0 with Rel-0.2 
*                  implementation.
******************************************************************************
*
*    FILENAME    : gl_htm_bckwrd_compatbl_api.c
*
*    DESCRIPTION : Give defination of functions which requires backward
*                  compatibilty of Rel-1.0 with Rel-0.2.
*
*
* DATE          NAME           REFERENCE          REASON
*----------- --------------  ---------------    ----------------------
*
* 23/04/2002 Ravi Jindal        -               Intial Draft.
* 
*
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/

     
#include "gl_htm.h"


/*****************************************************************************
* Function name    : gl_htm_hash_create
* Arguments        : gw_U32bit          user_id,
*                    gw_S8bit           hash_direction_flag,
*                    gw_U8bit           array_bit_qty,
*                    gw_U8bit           linked_list_bit_qty,
*                    gl_hash_table_t    *p_hash_table_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the hash table structure and initializes the.
*                    hash table structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_hash_create

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gw_S8bit                  hash_direction_flag,
      gw_U8bit                  array_bit_qty,
      gw_U8bit                  linked_list_bit_qty,
      gl_hash_table_t*          p_hash_table_id,
      gl_error_t*               p_ecode
    )
#else
    ( user_id, hash_direction_flag, array_bit_qty, linked_list_bit_qty,
      p_hash_table_id, p_ecode )
      gw_U32bit                 user_id;
      gw_S8bit                  hash_direction_flag;
      gw_U8bit                  array_bit_qty;
      gw_U8bit                  linked_list_bit_qty;
      gl_hash_table_t*          p_hash_table_id;
      gl_error_t*               p_ecode;
#endif
      
/****************************************************************************/
{
    gw_U32bit                   array_size=GL_NULL;
    gw_U32bit                   list_size=GL_NULL;
    gw_U16bit                   tmp_index=GL_NULL;
    gl_htm_hash_arg_t           hash_arg;

    for(array_size=1, tmp_index=0; tmp_index < array_bit_qty; tmp_index++)
    {
        array_size=array_size*2;
    }
    for(list_size=1, tmp_index=0; tmp_index < linked_list_bit_qty; tmp_index++)
    {
        list_size=list_size*2;
    }

    /*
     * To remove warnings.
     */
    hash_direction_flag = hash_direction_flag;
    /*
     * As function Pointer to free memory of element allocated by User is
     * required. The User of Rel-0.2 needs to defined a Poratble function
     * to free memory.
     * GL_HTM_FREE_KEY_ELEMENT_MEMORY_FUNC_PTR should be a function pointer of type
     * "gl_htm_free_key_elem_fn_t".
     */ 
    if(GL_FAILURE == gl_htm_create_hash_table( user_id, p_hash_table_id,
                array_size*list_size/* Max key qty */,
                array_size/* Max number of buckets */, 
                GL_HTM_NUMERIC_KEY_TYPE, GL_NULL, 
                GL_HTM_FREE_KEY_ELEM_MEMORY_FUNC_PTR,
                GL_NULL, GL_NULL, p_ecode))
    {
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}




/*****************************************************************************
* Function name    : gl_htm_hash_delete
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the hash table and resets the
*                    hash table structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_hash_delete

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gl_error_t*               p_ecode
    )
#else
    ( user_id, hash_table_id, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gl_error_t*               p_ecode;
#endif
   
/****************************************************************************/
{
    gw_U32bit                   *p_hash_arg;

    return(gl_htm_delete_hash_table(user_id, hash_table_id, &p_hash_arg,
                p_ecode)); 
}


/*****************************************************************************
* Function name    : gl_htm_add_key_element
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          key,
*                    gl_pvoid_t         p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Adds an element at the key in the hash table and
*                    increments the element count.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_add_key_element

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 key,
      gl_pvoid_t                p_element,
      gl_error_t*               p_ecode
    )
#else
    ( user_id, hash_table_id, key, p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 key;
      gl_pvoid_t                p_element;
      gl_error_t*               p_ecode;
#endif

/****************************************************************************/
{
    return(gl_htm_add_key_to_hash_table(user_id, hash_table_id, &key,
                p_element, p_ecode));
}



/*****************************************************************************
* Function name    : gl_htm_get_key_element
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the element at the key in the hash table.
* Functions called :
*******************************************************************************/
gl_return_t
gl_htm_get_key_element

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 key,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, key, p_p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 key;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    return(gl_htm_get_key_ref_from_hash_table(user_id, hash_table_id, &key,
                p_p_element, p_ecode));
}



/*****************************************************************************
* Function name    : gl_htm_replace_key_element
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          key,
*                    gl_pvoid_t         p_new_element,
*                    gl_pvoid_t         *p_p_old_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Replaces the element at the key in the hash table.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_replace_key_element

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 key,
      gl_pvoid_t                p_new_element,
      gl_pvoid_t                *p_p_old_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, key, p_new_element, p_p_old_element, p_ecode )
      gw_U32bit                     user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 key;
      gl_pvoid_t                p_new_element;
      gl_pvoid_t                *p_p_old_element;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    return(gl_htm_replace_key_elem_in_hash_table(user_id, hash_table_id, &key,
                p_new_element, p_p_old_element, p_ecode));
}





/*****************************************************************************
* Function name    : gl_htm_remove_key_element
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes the element at the key in the hash table and
*                    decrement the count.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_remove_key_element

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 key,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, key, p_p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 key;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    return(gl_htm_remove_key_from_hash_table(user_id, hash_table_id, &key,
                p_p_element, p_ecode));
}




/*****************************************************************************
* Function name    : gl_htm_flush_all
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U16bit          *p_key,
                     gl_pvoid_t         *p_p_element,
                     gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Flush all the elements in the hash table and reset the
*                    hash table structure elements.This function is called
*                    in a while loop.
* Functions called :
*******************************************************************************/
gl_return_t
gl_htm_flush_all

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 *p_key,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 *p_key;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    /*
     * As in Rel-1.0 this function returns after removeal
     * of all the elements.
     * To remove warnings.
     */
    p_key = p_key;
    p_p_element = p_p_element;
    
    return(gl_htm_remove_all_keys_from_hash_table(user_id, hash_table_id,
                p_ecode));
}

 

/*******************************************************************************
* Function name    : gl_htm_get_count
* Arguments        : gl_hash_table_t    hash_table_id,
*                    gl_error_t         *p_ecode
* Return Value     : gw_S32bit
* Functionality    : Gets the count of the elements in the hash table
*                    specified.
* Functions called :
*******************************************************************************/
gw_S32bit
gl_htm_get_count

#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id,
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_ecode )
      gl_hash_table_t           hash_table_id;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gw_U32bit                   number_of_keys;

    if(GL_FAILURE == gl_htm_get_key_count_of_hash_tabel( hash_table_id, 
                &number_of_keys, p_ecode))
    {
        return GL_ERROR;
    }
    /*
     * As return type is gw_S32bit.
     */ 
    return ((gw_S32bit)number_of_keys);
}




/*******************************************************************************
* Function name    : gl_htm_get_start_position
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          *p_key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the first key and corresponding element in
*                    the hash table.
* Functions called :
*******************************************************************************/
gl_return_t
gl_htm_get_start_position

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 *p_key,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, p_key, p_p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 *p_key;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    return(gl_htm_get_first_key_ref_from_hash_table(user_id, hash_table_id, &p_key,
                p_p_element, p_ecode));
}






/*******************************************************************************
* Function name    : gl_htm_get_next_association
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          key,
*                    gw_U32bit          *p_next_key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the key and corresponding element next to the given
*                    key in the hash table.
* Functions called :
*******************************************************************************/
gl_return_t
gl_htm_get_next_association

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 key,
      gw_U32bit                 *p_next_key,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, key, p_next_key, p_p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 key;
      gw_U32bit                 *p_next_key;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    return(gl_htm_get_next_key_ref_from_hash_table(user_id, hash_table_id, 
                &key, &p_next_key, p_p_element, p_ecode));
}






/*******************************************************************************
* Function name    : gl_htm_get_prev_association
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_table_t    hash_table_id,
*                    gw_U32bit          key,
*                    gw_U32bit          *p_prev_key,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the key and corresponding element, which is previous
*                    to the given key in the hash table.
* Functions called :
*******************************************************************************/
gl_return_t
gl_htm_get_prev_association

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_table_t           hash_table_id,
      gw_U32bit                 key,
      gw_U32bit                 *p_prev_key,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, hash_table_id, key, p_prev_key, p_p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_table_t           hash_table_id;
      gw_U32bit                 key;
      gw_U32bit                 *p_prev_key;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    return(gl_htm_get_prev_key_ref_from_hash_table(user_id, hash_table_id, &key, 
                &p_prev_key, p_p_element, p_ecode));
}



