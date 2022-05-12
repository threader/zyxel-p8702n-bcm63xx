/*****************************************************************************
* 
*  FUNCTION   : Common typedefs and constants used in context of 
*               Generic Library Shared Memory Manager (GL_SHM).    
*
******************************************************************************
*
* FILE NAME   : gl_shm.h
*
* DESCRIPTION : This file contains the declarations of data structures used
*               for Shared Memory manager implementation.
*              
*
*    DATE           NAME            REFERENCE           REASON
*------------   --------------     --------------   -------------------------
* 12/01/2000    Kushanava Laha      HSS:xxxxxxxx    Initial
* 16/08/2001    Prashant Jain       SPR-2276        Compiling with CC
*                                                   Complier on Solaris.
* 18/10/2001    Ravi Jindal         SPR-2788        Packaging of Genlib-Code.
* 30/10/2001    Ravi Jindal         SPR-2798        Change shmem_node_pool_id
*                                                   and shm_pool_id to U16bit.
* 17/11/2001    Ravi Jindal         SPR-3107        Incoprate the defects and 
*                                                   change cosmmetics of file.
* 30/11/2001    Ravi Jindal         SPR-2788        Re-Packaging of Genlib-Code.
*
* 22/03/2002    Ravi Jindal         SPR-3955        Enhamncements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/


#ifndef _GL_SHM_H_
#define _GL_SHM_H_

#include "gl_gscim_interfaces.h"
#include "gl_mm_interfaces.h"

#ifdef GL_SHARED_SHM
#ifdef GL_WINNT
    #include "gl_lm_interfaces.h"
    #include "gl_lm.h"
#endif
#endif

#include "gw_pragma_defs.h"

/*
 * C declarations in C++.
 */
#if defined(__cplusplus)
extern "C"
{
#endif

/* 
 * Structure definition for storing information of each reserved shared memory.
 */

typedef struct
{
    gw_U32bit                   task_id;
    gw_U8bit                    alloc_flag;
    gw_S8bit                    key_shmem[GL_SHM_MAX_NAME_SIZE];
    gw_U8bit                    acc_mode;
    gw_U32bit                   shmem_chunk_size;
    gw_U32bit                   shmem_chunk_qty;
    gw_U32bit                   alloc_chunk_qty;
#ifdef GL_SHM_SHARED
#ifdef GL_WINNT
    gw_U8bit                    *p_base_addr;
    gw_U8bit                    *p_free_start_ptr; 
    gl_pool_id_t                shmem_node_pool_id;
    gl_list_t                   free_list_id;   
#endif
#else
    gl_pool_id_t                shmem_pool_id;
    
#endif

}gl_shm_id_t;

#ifdef GL_SHM_SHARED
#ifdef GL_WINNT
typedef struct gl_shm_node_s
{
    gl_lm_header_t              link_list_header;
    gl_pvoid_t                  p_alloc_adds;
}gl_shm_node_t;
#endif
#endif

#if defined(__cplusplus)
}
#endif

#include "gw_pragma_undefs.h"
#endif
