/****************************************************************************
*
*    File Name   : gl_gscim_shrd_mm_op_wrppr_api.c
*
*    Description : Media Gateway Generic Library Shared Memory manager APIs
*                  and Auxiliary functions
*
* DATE              NAME        REFERENCE       REASON
* ----------    ------------    ----------  ---------------------
* Jan,2000    Kushanava Laha     -          GL_SHM (Media Gateway)
* 04/10/2001  Ravi Jindal       SPR-2751    Remove warnings with gcc,cc,     
*                                           CC on Solaris.
* 16/10/2001  Ravi Jindal       SPR-2785    Remove errors in function 
*                                           gl_shm_create,with GL_SHARED
*                                           _SHM flag set on WINNT.
* 18/10/2001  Ravi Jindal       SPR-2788    Packaging of Genlib-Code. 
* 19/10/2001  Ravi Jindal       SPR-2802    Adding Pre-ANSI-C features.
* 30/10/2001  Ravi Jindal       SPR-2744    Adding de-init functions.
* 17/11/2001  Ravi Jindal       SPR-3107    Incoprate the defects and change
*                                           cosmmetics of file.
*
* 22/03/2002  Ravi Jindal       SPR-3955    Enhamncements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/

#include "gl_shm.h"

/*--------------------------------------------------------------------------

  Variables static to GL_SHM

---------------------------------------------------------------------------*/

_STATIC_ gl_shm_id_t            *g_a_p_shr_mem_table[GL_MAX_SHM];
    /*
     * Array to store information for each mapped or 
     * reserved shared memory, indexed by shmem id.
     */

_STATIC_ gw_U16bit              gl_shm_free_id = 0;
    /* 
     * Counter to keep track of allocable shmem id.
     */


_STATIC_ gl_boolean             shm_mgr_init_flag = GL_FALSE;

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
_STATIC_ gl_return_t shm_link_list_element_memory_free(
              gl_pvoid_t        p_link_list_arg,
              gl_pvoid_t        p_element,
              gl_error_t        *p_ecode    );
#endif
#endif
/*--------------------------------------------------------------------------

  GL_SHM Function Definitions

---------------------------------------------------------------------------*/

/****************************************************************************
* Function name : gl_shm_init
* Arguments IN  : None
*           OUT : None
* Return Value  : GL_SUCCESS or GL_FAILURE
* Functionality : API for Initializing GL_SHM static database
*****************************************************************************/
gl_return_t 
gl_shm_init( void )
{
    gw_U16bit                   i;
    
    if(shm_mgr_init_flag == GL_FALSE)
    {
        for (i=0; i<GL_MAX_SHM; i++)
        {
            g_a_p_shr_mem_table[i] = GL_NULL;
        }
        shm_mgr_init_flag = GL_TRUE;
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}

/****************************************************************************
* Function name : gl_shm_create_shared_memory
* Arguments IN  : Pointer to gl_shm_create_option_t
*           OUT : Pointer to gl_shm_create_return_t
*                 Pointer to gl_error_t
* Return Value  : GL_SUCCESS or GL_FAILURE
* Functionality : API for Creating a Shared Memory
*****************************************************************************/
gl_return_t
gl_shm_create_shared_memory

#ifdef _ANSI_C_
    ( gl_shm_create_option_t        *p_gl_cr,
      gl_shm_create_return_t        *p_gl_cr_ret,
      gl_error_t                    *p_ecode
    )
#else
    ( p_gl_cr, p_gl_cr_ret, p_ecode )
      gl_shm_create_option_t        *p_gl_cr;
      gl_shm_create_return_t        *p_gl_cr_ret;
      gl_error_t                    *p_ecode;
#endif
    
/****************************************************************************/
{
    gl_shmem_id_t               shmem_id;
    gw_U8bit                    acc_mode;
    gl_error_t                  ecode;
#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    gw_U32bit                   page_acc_mode;
    gw_U32bit                   file_acc_mode;
    HANDLE                      hfilemap;
    PBYTE                       pbfile;
    gw_U32bit                   mem_att_mode;
#else
    /*
     * Currently no other OS supported.
     */
    *p_ecode = GL_SHM_ERR_NOT_IMPLEMENTED;
    return GL_FAILURE;
#endif
#endif

    shmem_id = gl_shm_free_id;
    gl_shm_free_id ++;
    
    g_a_p_shr_mem_table[shmem_id] = (gl_shm_id_t *)SYS_MALLOC(sizeof
            (gl_shm_id_t));
    if(GL_NULL == g_a_p_shr_mem_table[shmem_id])
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;  
    }
    
    switch (p_gl_cr->acc_mode) 
    {
        case    GL_SHM_READ :
#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
            page_acc_mode = PAGE_READONLY;
            file_acc_mode = FILE_MAP_READ;
#endif
#endif
            acc_mode = GL_SHM_READ;
            break;
            
        case    GL_SHM_WRITE :
        case    GL_SHM_READWRITE :
#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
            page_acc_mode = PAGE_READWRITE;
            file_acc_mode = FILE_MAP_WRITE;
#endif
#endif
            acc_mode = GL_SHM_READWRITE;
            break;
            
    default :
            *p_ecode = GL_SHM_ERR_INV_ACC_MODE;
            return GL_FAILURE;
    }

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    mem_att_mode = SEC_RESERVE;
    hfilemap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, 
                page_acc_mode|mem_att_mode, 0, 
                (p_gl_cr->shm_chunk_size*p_gl_cr->shm_chunk_qty),
                p_gl_cr->key_shmem);

    if(hfilemap == NULL) 
    {
        *p_ecode = GL_SHM_ERR_SHMEM_MAP_FAIL;
        return GL_FAILURE;
    }
    if((hfilemap != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
    {
        CloseHandle(hfilemap);
        *p_ecode = GL_SHM_ERR_SHMEM_ID_IN_USE;
        return GL_FAILURE;
    }
    
    pbfile = (PBYTE) MapViewOfFile (hfilemap,file_acc_mode,0,0,0);
    
    if(pbfile == NULL) 
    {
        CloseHandle(hfilemap);
        *p_ecode = GL_SHM_ERR_SHMEM_MAP_FAIL;
        return GL_FAILURE;
    }
    
    if(GL_FAILURE == gl_mm_pool_create(p_gl_cr->task_id, 
            p_gl_cr->shm_chunk_qty, sizeof(gl_shm_node_t),
            &(g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id), &ecode))
    {
        CloseHandle(hfilemap);
        UnmapViewOfFile(pbfile);
        pbfile = GL_NULL;
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }

    if(GL_FAILURE == gl_llm_create_link_list (p_gl_cr->task_id, 
            &(g_a_p_shr_mem_table[shmem_id]->free_list_id), 
            (gl_llm_free_element_fn_t)shm_link_list_element_memory_free,
            &(g_a_p_shr_mem_table[shmem_id]), &ecode))
    {
        CloseHandle(hfilemap);
        UnmapViewOfFile(pbfile);
        pbfile = GL_NULL;
        if(GL_FAILURE == gl_mm_pool_delete(p_gl_cr->task_id, 
            g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id, &ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;
        }
        *p_ecode = GL_SHM_ERR_MEM_ALLOC_FAILS;
        return GL_FAILURE;
    }
    g_a_p_shr_mem_table[shmem_id]->p_base_addr = g_a_p_shr_mem_table[shmem_id]->
    p_free_start_ptr = (gw_U8bit *)pbfile;
    
#endif
#else
    if(GL_FAILURE == gl_mm_pool_create(p_gl_cr->task_id,
        p_gl_cr->shm_chunk_qty, p_gl_cr->shm_chunk_size, 
        &(g_a_p_shr_mem_table[shmem_id]->shmem_pool_id), &ecode)) 
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }
#endif

    g_a_p_shr_mem_table[shmem_id]->shmem_chunk_qty = p_gl_cr->shm_chunk_qty;
    g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size = p_gl_cr->shm_chunk_size;
    g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty = 0;
    g_a_p_shr_mem_table[shmem_id]->task_id = p_gl_cr->task_id;
    g_a_p_shr_mem_table[shmem_id]->acc_mode = acc_mode;
    strcpy(g_a_p_shr_mem_table[shmem_id]->key_shmem,p_gl_cr->key_shmem);
    
    p_gl_cr_ret->shmem_id = shmem_id;
    return GL_SUCCESS;
}



/****************************************************************************
* Function name : gl_shm_map_shared_memory
* Arguments IN  : Pointer to gl_shm_map_option_t
*           OUT : Pointer to gl_shm_map_return_t
*                 Pointer to gl_error_t
* Return Value  : GL_SUCCESS or GL_FAILURE
* Functionality : API for Mapping an already created Shared Memory
*****************************************************************************/
gl_return_t
gl_shm_map_shared_memory

#ifdef _ANSI_C_
    ( gl_shm_map_option_t           *p_gl_map,
      gl_shm_map_return_t           *p_gl_map_ret,
      gl_error_t                    *p_ecode
    )
#else
    ( p_gl_map, p_gl_map_ret, p_ecode )
      gl_shm_map_option_t           *p_gl_map;
      gl_shm_map_return_t           *p_gl_map_ret;
      gl_error_t                    *p_ecode;
#endif
    
/****************************************************************************/
{

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    gl_shmem_id_t               shmem_id;
    gw_U8bit                    acc_mode;
    gw_U32bit                   file_acc_mode;
    HANDLE                      hfilemap;
    PBYTE                       pbfile;
#else
    *p_ecode = GL_SHM_ERR_NOT_IMPLEMENTED;
    return GL_FAILURE;
#endif
#else
    gw_U16bit                    i;
    
    for(i=0; i< GL_MAX_SHM; i++)
    {
        if((GL_NULL != g_a_p_shr_mem_table[i]) && 
           (!strcmp(g_a_p_shr_mem_table[i]->key_shmem, p_gl_map->key_shmem)))
        {
            break;
        }
    }
    if(i == GL_MAX_SHM)
    {
        *p_ecode = GL_SHM_ERR_SHMEM_MAP_FAIL;
        return GL_FAILURE;
    }
    p_gl_map_ret->shmem_id = i;
#endif

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    switch (p_gl_map->acc_mode)
    {
        case GL_SHM_READ :
            file_acc_mode = FILE_MAP_READ;
            acc_mode = GL_SHM_READ;
            break;
            
        case GL_SHM_WRITE :
        case GL_SHM_READWRITE :
            file_acc_mode = FILE_MAP_WRITE;
            acc_mode = GL_SHM_READWRITE;
            break;
            
    default :
            *p_ecode = GL_SHM_ERR_INV_ACC_MODE;
            return GL_FAILURE;
    }

    hfilemap = OpenFileMapping(file_acc_mode, FALSE,p_gl_map->key_shmem);

    if(hfilemap == NULL) 
    {
        *p_ecode = GL_SHM_ERR_SHMEM_MAP_FAIL;
        return GL_FAILURE;
    }

    pbfile = (PBYTE) MapViewOfFile (hfilemap,file_acc_mode,0,0,0);
        
    if(pbfile == NULL)
    {
        CloseHandle(hfilemap);
        *p_ecode = GL_SHM_ERR_SHMEM_MAP_FAIL;
        return GL_FAILURE;
    }
    shmem_id = gl_shm_free_id;
    gl_shm_free_id ++;
    
    if(GL_FAILURE == gl_mm_pool_create(p_gl_map->task_id, 
            p_gl_map->shm_chunk_qty, sizeof(gl_shm_node_t),
            &(g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id), p_ecode))
    {
        CloseHandle(hfilemap);
        UnmapViewOfFile(pbfile);
        pbfile = GL_NULL;
        return GL_FAILURE;
    }

    if(GL_FAILURE == gl_llm_create_link_list (p_gl_map->task_id, 
            &(g_a_p_shr_mem_table[shmem_id]->free_list_id), 
            (gl_llm_free_element_fn_t)shm_link_list_element_memory_free,
            &(g_a_p_shr_mem_table[shmem_id]), p_ecode))
    {
        CloseHandle(hfilemap);
        UnmapViewOfFile(pbfile);
        pbfile = GL_NULL;
        if(GL_FAILURE == gl_mm_pool_delete(p_gl_map->task_id, 
            g_a_p_shr_mem_table[shmem_id]-> shmem_node_pool_id, p_ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;
        }
        *p_ecode = GL_SHM_ERR_MEM_ALLOC_FAILS;
        return GL_FAILURE;
    }
    g_a_p_shr_mem_table[shmem_id]->p_base_addr = g_a_p_shr_mem_table[shmem_id]->
    p_free_start_ptr = (gw_U8bit *)pbfile;
    g_a_p_shr_mem_table[shmem_id]->task_id = p_gl_map->task_id;
    strcpy(g_a_p_shr_mem_table[shmem_id]->key_shmem,p_gl_map->key_shmem);
    g_a_p_shr_mem_table[shmem_id]->acc_mode = acc_mode;
    
    g_a_p_shr_mem_table[shmem_id]->shmem_chunk_qty = p_gl_map->shm_chunk_qty;
    g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size = p_gl_map->shm_chunk_size;
    g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty = 0;
    
    p_gl_map_ret->shmem_id = shmem_id;

#endif
#endif

    return GL_SUCCESS;
}



/****************************************************************************
* Function name : gl_shm_alloc_shared_memory
* Arguments IN  : Pointer to gl_shm_alloc_option_t
*           OUT : Pointer to gl_shm_alloc_return_t
*                 Pointer to gl_error_t
* Return Value  : GL_SUCCESS or GL_FAILURE
* Functionality : API for allocating memory from an already created or mapped
*                 shared memory area.
*****************************************************************************/
gl_return_t
gl_shm_alloc_shared_memory

#ifdef _ANSI_C_
    ( gl_shm_alloc_option_t         *p_gl_al,
      gl_shm_alloc_return_t         *p_gl_al_ret,
      gl_error_t                    *p_ecode
    )
#else
    ( p_gl_al, p_gl_al_ret, p_ecode )
      gl_shm_alloc_option_t         *p_gl_al;
      gl_shm_alloc_return_t         *p_gl_al_ret;
      gl_error_t                    *p_ecode;
#endif
    
/****************************************************************************/
{
    gl_shmem_id_t               shmem_id;
    gl_pvoid_t                  p_alloc_adds;
    gl_error_t                  ecode;

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    gl_shm_node_t               *shm_node;
#else
    *p_ecode = GL_SHM_ERR_NOT_IMPLEMENTED;
    return GL_FAILURE;
#endif
#endif
    shmem_id = p_gl_al->shmem_id;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_MAX_SHM <= shmem_id)
       ||(GL_NULL == g_a_p_shr_mem_table[shmem_id])) 
    {
        *p_ecode = GL_SHM_ERR_INV_SHM_ID;
        return GL_FAILURE;
    }
    if(g_a_p_shr_mem_table[shmem_id]->acc_mode != p_gl_al->acc_mode) 
    {
        *p_ecode = GL_SHM_ERR_INV_ACC_MODE;
        return GL_FAILURE;
    }
    if(g_a_p_shr_mem_table[shmem_id]->task_id != p_gl_al->task_id) 
    {
        *p_ecode = GL_SHM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
    if(g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size < p_gl_al->size) 
    {
        *p_ecode = GL_SHM_ERR_INV_MEM_SIZE;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    if(GL_FALSE == gl_llm_is_empty(g_a_p_shr_mem_table[shmem_id]->free_list_id, 
       &ecode))
    {
        if(GL_FAILURE == gl_llm_remove_head(g_a_p_shr_mem_table[shmem_id]->
            task_id, g_a_p_shr_mem_table[shmem_id]->free_list_id, 
            (gl_pvoid_t *)&shm_node,&ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;
        }

        p_alloc_adds = shm_node->p_alloc_adds;
        gl_mm_release_buffer_to_pool(g_a_p_shr_mem_table[shmem_id]->task_id, 
           g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id, shm_node, &ecode);
    }
    else if(g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty < 
            g_a_p_shr_mem_table[shmem_id]->shmem_chunk_qty)
    {
        if(NULL == VirtualAlloc((void *)g_a_p_shr_mem_table[shmem_id]->
            p_free_start_ptr, g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size, 
            MEM_COMMIT,PAGE_READWRITE)) 
        {
            *p_ecode = GL_SHM_ERR_MEM_ALLOC_FAILS;
            return GL_FAILURE;
        }
        p_alloc_adds = g_a_p_shr_mem_table[shmem_id]->p_free_start_ptr;
        g_a_p_shr_mem_table[shmem_id]->p_free_start_ptr += 
            g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size;
    }
    else
    {
        *p_ecode = GL_SHM_ERR_MEM_ALLOC_FAILS;
        return GL_FAILURE;
    }
#endif
#else
    if(g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty < 
       g_a_p_shr_mem_table[shmem_id]->shmem_chunk_qty )
    {
        p_alloc_adds = gl_mm_get_buffer_from_pool(g_a_p_shr_mem_table[shmem_id]
            ->task_id, g_a_p_shr_mem_table[shmem_id]->shmem_pool_id, &ecode);
        
        if(GL_NULL == p_alloc_adds)
        {
            *p_ecode = GL_SHM_ERR_MEM_ALLOC_FAILS;
            return GL_FAILURE;
        }
    }
    else
    {
        *p_ecode = GL_SHM_ERR_MEM_ALLOC_FAILS;
        return GL_FAILURE;
    }
#endif
    g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty += 1;

    p_gl_al_ret->p_alloc_adds = p_alloc_adds;
    return GL_SUCCESS;
}

/****************************************************************************
* Function name : gl_shm_dealloc_shared_memory
* Arguments IN  : Pointer to gl_shm_alloc_option_t
*           OUT : Pointer to gl_shm_alloc_return_t
*                 Pointer to gl_error_t
* Return Value  : GL_SUCCESS or GL_FAILURE
* Functionality : API for allocating memory from an already created or mapped
*                 shared memory area
*****************************************************************************/
gl_return_t
gl_shm_dealloc_shared_memory

#ifdef _ANSI_C_
    ( gl_shm_dealloc_option_t       *p_gl_dal,
      gl_error_t                    *p_ecode
    ) 
#else
    ( p_gl_dal, p_ecode ) 
      gl_shm_dealloc_option_t       *p_gl_dal;
      gl_error_t                    *p_ecode; 
#endif
    
/****************************************************************************/
{
    gl_shmem_id_t               shmem_id;
    gl_error_t                  ecode;

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    gl_shm_node_t               *shm_node;
#else
    *p_ecode = GL_SHM_ERR_NOT_IMPLEMENTED;
    return GL_FAILURE;
#endif
#endif
    shmem_id = p_gl_dal->shmem_id;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_MAX_SHM <= shmem_id)
       ||(GL_NULL == g_a_p_shr_mem_table[shmem_id])) 
    {
        *p_ecode = GL_SHM_ERR_INV_SHM_ID;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    if (((gw_U8bit*)p_gl_dal->p_alloc_adds < g_a_p_shr_mem_table[shmem_id]->
        p_base_addr)||
        ((gw_U8bit*)p_gl_dal->p_alloc_adds > (g_a_p_shr_mem_table[shmem_id]->
        p_base_addr + (g_a_p_shr_mem_table[shmem_id]->shmem_chunk_qty * 
        g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size)))) 
    {
        *p_ecode = GL_SHM_ERR_INV_ADDR;
        return GL_FAILURE;
    }
    
    if(GL_NULL ==(shm_node = (gl_shm_node_t *)gl_mm_get_buffer_from_pool
        (g_a_p_shr_mem_table[shmem_id]->task_id, g_a_p_shr_mem_table
        [shmem_id]->shmem_node_pool_id, &ecode)))
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }
    
    shm_node->p_alloc_adds = p_gl_dal->p_alloc_adds;

    if(GL_FAILURE == gl_llm_add_head(g_a_p_shr_mem_table[shmem_id]->task_id, 
        g_a_p_shr_mem_table[shmem_id]->free_list_id, shm_node, &ecode)) 
    {
        gl_mm_release_buffer_to_pool(g_a_p_shr_mem_table[shmem_id]->task_id, 
           g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id, shm_node, &ecode);
        
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }
#endif
#else
    if (GL_NULL == p_gl_dal->p_alloc_adds)
    {
        *p_ecode = GL_SHM_ERR_INV_ADDR;
        return GL_FAILURE;
    }
    if(GL_FAILURE == gl_mm_release_buffer_to_pool(g_a_p_shr_mem_table[shmem_id]
        ->task_id, g_a_p_shr_mem_table[shmem_id]->shmem_pool_id, 
        p_gl_dal->p_alloc_adds, &ecode))
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;   
    }
#endif
    g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty -= 1;
    
    return GL_SUCCESS;
}


/****************************************************************************
* Function name    : gl_shm_delete_shared_memory
* Arguments        : gw_U32bit          task_id, 
*                    gl_U32bit          shmem_id, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the shared memory and resets the 
*                    structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_shm_delete_shared_memory

#ifdef _ANSI_C_
    ( gw_U32bit                     task_id,
      gw_U32bit                     shmem_id, 
      gl_error_t                    *p_ecode
    )
#else
    ( task_id, shmem_id, p_ecode )
      gw_U32bit                     task_id;
      gw_U32bit                     shmem_id;
      gl_error_t                    *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_MAX_SHM <= shmem_id)
       ||(GL_NULL == g_a_p_shr_mem_table[shmem_id])) 
    {
        *p_ecode = GL_SHM_ERR_INV_SHM_ID;
        return GL_FAILURE;
    }
    if( g_a_p_shr_mem_table[shmem_id]->task_id != task_id)
    {
        *p_ecode = GL_SHM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * If any shared memory region is allocated return error.
     */
    if(g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty != 0)
    {
        *p_ecode = GL_SHM_ERR_SHMEM_ID_IN_USE;
        return GL_FAILURE;
    }
    /*
     * Else reset the parameters free the resources.
     */
    strcpy(g_a_p_shr_mem_table[shmem_id]->key_shmem,"");
    g_a_p_shr_mem_table[shmem_id]->shmem_chunk_qty = GL_NULL;
    g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size = GL_NULL;
    
#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    if(GL_FAILURE == gl_mm_release_all_task_pool_buffers(task_id,
        g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id, p_ecode))
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;  
    }
    if(GL_FAILURE == gl_llm_remove_all_elements(task_id,
        g_a_p_shr_mem_table[shmem_id]->free_list_id, p_ecode))
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }
    if(GL_FAILURE == gl_llm_list_delete(task_id,
        g_a_p_shr_mem_table[shmem_id]->free_list_id, p_ecode))
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }
    if(GL_FAILURE == gl_mm_pool_delete(task_id, 
        g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id, p_ecode))
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }
#endif
#else
    if(GL_FAILURE == gl_mm_pool_delete(task_id, 
        g_a_p_shr_mem_table[shmem_id]->shmem_pool_id, p_ecode))
    {
        *p_ecode = GL_SHM_ERR_INTERNAL;
        return GL_FAILURE;
    }
#endif
    SYS_FREE(g_a_p_shr_mem_table[shmem_id]);
    g_a_p_shr_mem_table[shmem_id] = GL_NULL;

    return GL_SUCCESS;
}


/****************************************************************************
* Function name    : gl_shm_destroy_all_shared_memory
* Arguments        : gw_U8bit           is_forceful_cleanup
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes all the shared-memory regions and free all 
*                    the corresponding occupied resources.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_shm_destroy_all_shared_memory

#ifdef _ANSI_C_
    ( gw_U8bit                      is_forceful_cleanup,
      gl_error_t                    *p_ecode
    )
#else
    ( is_forceful_cleanup, p_ecode )
      gw_U8bit                      is_forceful_cleanup;
      gl_error_t                    *p_ecode;
#endif
    
/****************************************************************************/
{
    gl_shmem_id_t               shmem_id= GL_NULL;
    gw_boolean                  is_not_empty_flag = GL_FALSE;
/*
 * #ifdef GL_MT_ENABLED
 *  gl_lock_mutex (&shm_mgr_sem, GL_NULL);
 * #endif
 */
        
    for(shmem_id = 0; shmem_id < GL_MAX_SHM; shmem_id++)
    {
        if(GL_NULL == g_a_p_shr_mem_table[shmem_id])
        {
            continue;
        }                 
        if((is_forceful_cleanup == GL_FALSE ) &&
           (g_a_p_shr_mem_table[shmem_id]->alloc_chunk_qty != 0))
        {
            is_not_empty_flag = GL_TRUE;
            continue;
        }
        strcpy(g_a_p_shr_mem_table[shmem_id]->key_shmem,"");
        g_a_p_shr_mem_table[shmem_id]->shmem_chunk_qty = GL_NULL;
        g_a_p_shr_mem_table[shmem_id]->shmem_chunk_size = GL_NULL;
    
#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
        if(GL_FAILURE == gl_mm_release_all_task_pool_buffers(g_a_p_shr_mem_table
            [shmem_id]->task_id, g_a_p_shr_mem_table[shmem_id]->
            shmem_node_pool_id, p_ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;  
        }
        if(GL_FAILURE == gl_llm_remove_all_elements(g_a_p_shr_mem_table
            [shmem_id]->task_id, g_a_p_shr_mem_table[shmem_id]->free_list_id, 
            p_ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_llm_delete_link_list(g_a_p_shr_mem_table[shmem_id]
            ->task_id, g_a_p_shr_mem_table[shmem_id]->free_list_id, p_ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_mm_pool_delete(g_a_p_shr_mem_table[shmem_id]->
           task_id, g_a_p_shr_mem_table[shmem_id]->shmem_node_pool_id, p_ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;
        }
#endif
#else
        if(GL_FAILURE == gl_mm_release_all_task_pool_buffers(g_a_p_shr_mem_table
            [shmem_id]->task_id, g_a_p_shr_mem_table[shmem_id]->shmem_pool_id,
            p_ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;  
        }
        if(GL_FAILURE == gl_mm_pool_delete(g_a_p_shr_mem_table[shmem_id]->
            task_id, g_a_p_shr_mem_table[shmem_id]->shmem_pool_id, p_ecode))
        {
            *p_ecode = GL_SHM_ERR_INTERNAL;
            return GL_FAILURE;
        }
#endif
        SYS_FREE(g_a_p_shr_mem_table[shmem_id]);
        g_a_p_shr_mem_table[shmem_id] = GL_NULL;

/*
 * #ifdef GL_MT_ENABLED
 *      gl_delete_mutex(&(g_a_p_shr_mem_table[link_list_id]->lm_sem));
 * #endif
 */
    }
        
    if( is_not_empty_flag == GL_TRUE)
    {
        /* 
         * Some list are not empty and no forcefull destroy.
         */
/*
 * #ifdef GL_MT_ENABLED
 *      gl_unlock_mutex (GL_NULL,&shm_mgr_sem);
 * #endif
 */ 
        *p_ecode = GL_SHM_ERR_SHMEM_ID_IN_USE;
        return GL_FAILURE;
    }
/*    else
 *   {
 * #ifdef GL_MT_ENABLED
 *      gl_delete_mutex(&(shm_mgr_sem));
 * #endif
 *   }
 */
 
    shm_mgr_init_flag = GL_FALSE;    
    return GL_SUCCESS;
}



#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
gl_return_t shm_link_list_element_memory_free(
              gl_pvoid_t        p_link_list_arg,
              gl_pvoid_t        p_element,
              gl_error_t        *p_ecode    )
{
    gl_shm_id_t             *shm_ptr = (gl_shm_id_t *)p_link_list_arg;
    
    return(gl_mm_release_buffer_to_pool(p_link_list_arg->task_id, 
        p_link_list_arg->shmem_node_pool_id, p_element, p_ecode));
        
}
#endif
#endif
