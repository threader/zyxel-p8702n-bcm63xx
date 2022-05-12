/************************************************************************* 
 *
 *     FILENAME           :  icf_port_memory.c
 *
 *     DESCRIPTION        :  This file contains memrory allocation
 *                           deallocation related porting functions that
 *                           shall be used across all modules of ICF.
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         25/11/04    Pankaj Negi        ICF LLD        Initial Version.
 *         06/01/05    Aashish Suchdev                      Compilation changes
 *         07/01/05    Aashish Suchdev                      replaced retval
 *         24/02/05    Jalaj Negi                           Compilation changes
 *                                                          for Linux
 *         29/11/056   Umang Singh	 SPR 13633
 *         
 *         08-Feb-2008 Anurag Khare      IPTK Rel 8.0     Changes for SEC
 *                                                        Dns Server Support
 *         28-Mar-2008 Shruti Thakral     SPR 18316       Klocwork warning 
 *                                                        removal
 *         7-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 *         01-May-2008 Shruti Thakral     SPR 18316       Klocwork warning 
 *                                                        removal
 *         02-May-2008 Shruti Thakral     SPR 18316       Klocwork warning 
 *         24-June-2008 Jagmohan Chauhan  SPR 18684       CSR Merge 1-6048221
 *         -3-Jul-2008  Anurag Khare      SPR 18684       Compliation issue on gcc 4.2.3
 *         03-Nov-2008  Rajiv Kumar       SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                                        18963)
 *         12-Nov-2008  Tarun Gupta       SPR 19189       Merged Rel 7.1.2 SPR 18872
 *         08-Dec-2008 Abhishek Dhammawat SPR 19223       Klocwork warning
 *                                                        removal
 *         02-Mar-2009 Alok Tiwari      IPTK Rel8.1       Changes done for
 *                                           replacing the ares implementation
 *                                           with arDnsClient.New function
 *                                           icf_port_dns_server_data_init. 
 *        03-Mar-2009 Kamal Ashraf     IPTK Rel 8.1       changes done for open
 *                                                        source replacement   
 *        11-Jun-2009 Ashutosh Mohan   SPR 19737          Changes done for GCC 
 *                                                        4.1 warnings removal 
 *        12-Aug-2009 Anuradha Gupta   SPR 19748         Modified icf_port_memget
 *        01-Sep-2009 Tarun Gupta      SPR 20008         Memory profiling updations
 *        28-Oct-2009 Abhishek Dhammawat SPR 20185       Merged coverty fix
 *
 *   Copyright 2009, Aricent.
 *
 *************************************************************************/
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))
#include <stdlib.h>
#endif

#ifdef GENLIB_MEMORY
#include "gl_types.h"
#include "gl_mm_interfaces.h"
#include "gl_tm_interfaces.h"
#include "gl_mm.h"
#endif
#include "icf_feature_flags.h"
#include "icf_common_types.h"
#include "icf_ecode.h"
#include "icf_port_struct.h"
#include "icf_port_types.h"
#include "icf_port_prototypes.h"
#include "icf_port_intf_prototypes.h"
#include "icf_macro.h"

#ifdef GENLIB_TIMER
#include "gl_tm_interfaces.h"
#endif

#ifdef GENLIB_MEMORY
extern  gl_mm_pool_t *g_a_p_app_sp_pool_table[GL_MM_MAX_TASK_POOL_QTY];
#endif
#ifdef ICF_DEBUG_MEMPOOL
extern FILE            *p_memget_file_ptr ;
#endif

/* SPR 20008 : Define the following macro for memory profiling */
/* #define ICF_MEMORY_CALCULATION  */

#ifdef ICF_MEMORY_CALCULATION
icf_uint32_t g_max_size ;
#endif

#ifdef ICF_UT_TEST
#include "icf_macro.h"
#endif

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_memget
*
*  DESCRIPTION    :   This function is called to allocate buffers in the system.
*  Arguments      :   size      - Size of memory buffer required
*                     pool_id   - Pool from which memory is required
*                     p_eocde   - pointer to variable to return error code
*
*  RETURNS        :   Pointer to the allocated buffer if there is available 
*                     memory in the system, else ICF_NULL with ecode as 
*                     ICF_OUT_OF_MEM.
*
 *************************************************************************/

icf_void_t* 
icf_port_memget(
        IN  icf_void_t           *p_port_info,
        IN  icf_uint32_t         size,
        IN  icf_pool_id_t        pool_id,
        OUT icf_error_t          *p_ecode)
{
    icf_void_t*       p_buf = ICF_NULL;

#ifdef ICF_UT_TEST
    icf_return_t ret_val = ICF_SUCCESS;
#endif
#ifdef ICF_UT_TEST
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)
#endif

#ifdef GENLIB_MEMORY
    icf_global_port_info_st  *p_glb_port_info = 
        (icf_global_port_info_st*)p_port_info;

    gl_error_t           gl_ecode;
    gl_pool_id_t         gl_pool_id;
    gl_pool_info_t       gl_pool_info;
#endif 
	if ( ICF_NULL == p_port_info)
	{
	}
	if ( ICF_NULL == p_ecode)
	{
	}

    /* SPR 19748: In case the request memory allocation size
     * is more than the following macro, return failre.
     * As System malloc should not be allowed to reach uint32
     * max size.
     */ 
    if( ICF_PORT_MAX_MEMORY_ALLOC_SIZE < size)
    {
       ICF_PRINT(((icf_uint8_t*)"\n[PORT] Memory Allocation failure , MAX size exceeded"))
        return ICF_NULL;
    }

#ifdef GENLIB_MEMORY
     if(ICF_MEM_COMMON == pool_id)
     {
         gl_pool_info.pool_id = 0;
         gl_pool_info.task_local_data = GL_MM_MAX_TASK_POOL_QTY - 1;
        p_buf = (icf_void_t*) gl_mm_get_task_specific_cmn_pool_buffer ((gw_U32bit)ICF_PORT_CMN_TASK_ID, &gl_pool_info, size, &gl_ecode);
     }
     else
     {
         /* 
          * A check on valid pool_id value?
          */
        gl_pool_id = p_glb_port_info->gl_pool_id_map[pool_id][1];

        p_buf = (icf_void_t*) gl_mm_get_buffer_from_pool(ICF_PORT_CMN_TASK_ID, 
                                                            gl_pool_id, 
                                                            &gl_ecode);
     }

    if(p_buf == (icf_void_t*) GW_NULL)
    {
        ICF_GL_ECODE_MAP(p_ecode,gl_ecode)

#if 0
        icf_port_map_genlib_ecode(gl_ecode,
                p_ecode);
#endif    
#ifdef ICF_GL_MM_DEBUG
        /* If memory allocation fails, print the current usage for analysis */

        ICF_GL_MM_DEBUG_PRINT(("\n[PORT]: !! MEMORY ALLOCATION FAILURE FOR " \
            "pool_id = %d, size = %d bytes !!\n", pool_id, size));
        icf_port_print_mem_pool_stats();
#endif

    }
	else
    {
        icf_port_memset(p_buf, 0, size);       
    }
#else  /* ifdef GENLIB_MEMORY */
    (void)pool_id;
    
#ifdef ICF_MEMORY_CALCULATION

    /* SPR 20008 : Allocating size + 4 bytes of memory, the first 4 bytes will 
       be used to store the value of the size variable. This will be used 
       in memory profiling to write the size into a file.
    */   

    p_buf = (icf_void_t*) malloc(size + 4);

    if(ICF_NULL != p_buf) 
    {
        /* If able to get the memory and p_buf is not null */
        icf_port_memset(p_buf,0,size + 4);
        /* storing the size in the first 4 bytes */
        *((icf_uint32_t *)p_buf) = size;
        g_max_size += size;
	    printf("\nMEMGET        : %u Total Memory: %u",size,g_max_size);
    }
    else 
    {
       /* If unable to get the memory print failure */
        perror("malloc");
	    printf("\n PORT_MEMGET failed for buffer size %u\n",size);
    }
#else

    p_buf = (icf_void_t*) malloc(size);
    if(ICF_NULL != p_buf) 
    {
        /* If able to get the memory and p_buf is not null */
        icf_port_memset(p_buf,0,size);
    }
    else
    {
        /* If unable to get the memory print failure */
	    perror("malloc");
	    printf("\n PORT_MEMGET failed for buffer size %u\n",size);
    }
#endif 
#ifdef ICF_DEBUG_MEMPOOL
    /* SPR 20008 : Writing the size and other information in a file */
   p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
   if(p_memget_file_ptr)
   {
       fprintf(p_memget_file_ptr,",,,,,,,,,MEMGET,0x%x,%d,%d\n",
               p_buf,size,g_max_size);
       fclose(p_memget_file_ptr);
       p_memget_file_ptr = ICF_NULL;
   }
#endif
#endif
   
#ifdef ICF_MEMORY_CALCULATION
    return (p_buf + 4);
#else
	return p_buf;
#endif
#ifdef ICF_UT_TEST
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
#endif
    return p_buf;
} /* End of icf_memget */

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_memfree
*
*  DESCRIPTION    :   This function is called to deallocate buffers.
*  RETURNS        :   void.
*
 *************************************************************************/

icf_return_t
icf_port_memfree(
        IN  icf_void_t       *p_port_info,
        INOUT  icf_void_t       *p_buf,
        IN  icf_pool_id_t    pool_id,
        OUT icf_error_t      *p_ecode)
{
#ifdef GENLIB_MEMORY
    icf_global_port_info_st  *p_glb_port_info = 
        (icf_global_port_info_st*)p_port_info;

    gw_return_t          ret_val;
    gl_error_t           gl_ecode;
    /* Fix for CSR 1-7436195 */
    /* Coverity bug fix */
    gl_pool_id_t         gl_pool_id = 0;
#endif 
    if(ICF_NULL == p_buf)
    {
        return ICF_SUCCESS;
    }
#ifdef GENLIB_MEMORY
    if(ICF_MEM_COMMON == pool_id)
    {
        ret_val = gl_mm_release_task_specific_cmn_pool_buffer( ICF_PORT_CMN_TASK_ID,
                                                    (gl_pvoid_t)p_buf,
                                                    &gl_ecode);
    }
    else
    {
        /* 
         * A check on valid pool_id value?
         */
        gl_pool_id = p_glb_port_info->gl_pool_id_map[pool_id][1];

        ret_val = gl_mm_release_buffer_to_pool(ICF_PORT_CMN_TASK_ID,
                gl_pool_id,
                (gl_pvoid_t)p_buf,
                &gl_ecode);
    }

    if(GW_FAILURE == ret_val)
    {
#ifdef ICF_GL_MM_DEBUG
        ICF_GL_MM_DEBUG_PRINT(("\n[ALARM]: Genlib free failed for pool_id=%d"\
                    ", gl_pool_id=%d, \ p_buf=%x with gl_ecode=%d!!\n", \
                    pool_id, gl_pool_id, p_buf, gl_ecode));
#endif
        ICF_GL_ECODE_MAP(p_ecode,gl_ecode)
#if 0
        icf_port_map_genlib_ecode(gl_ecode,
                p_ecode);
#endif
        return ICF_FAILURE;
    }
    else
    {
        return ICF_SUCCESS;
    }
#else

#ifdef ICF_MEMORY_CALCULATION
	icf_uint32_t	size = 0;
    /* SPR 20008 : Calculating size from the first 4 bytes */
	p_buf -= 4;
	size = *((icf_uint32_t *)p_buf);
	g_max_size -= size;
	printf("\nMEMFREE       : %u Total Memory: %u",size, g_max_size);
#endif
#ifdef ICF_DEBUG_MEMPOOL
    /* SPR 20008 : Writing the size and other information in a file */
    p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
    if(p_memget_file_ptr)
    {
        fprintf(p_memget_file_ptr,",,,,,,,,,MEMFREE,0x%x,%d,%d\n",
                p_buf,(-size),g_max_size);
        fclose(p_memget_file_ptr);
        p_memget_file_ptr=ICF_NULL;
    }
#endif
    
    p_ecode = p_ecode;
    if (ICF_NULL == p_port_info)
    {}/* warning removal*/

    (void)pool_id;

    if(p_buf != ICF_NULL)
    {
        free(p_buf);
		p_buf = ICF_NULL;
        return ICF_SUCCESS;
    }
    else
    {
        return ICF_FAILURE;
    }

#endif

} /* End of icf_port_memfree */

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_init
*
*  DESCRIPTION    :   This function initializes the porting layer .
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_init(
        INOUT icf_void_t             **p_p_port_info,
        OUT icf_error_t              *p_ecode )
{
    icf_global_port_info_st      *p_glb_port_info = ICF_NULL;
    icf_uint8_t                   counter = ICF_NULL;
    
    p_glb_port_info = (icf_global_port_info_st*)icf_port_static_memget(\
            sizeof(icf_global_port_info_st), p_ecode);

    if ( ICF_NULL == p_glb_port_info)
    {
        *p_ecode = ICF_ERROR_MEM_INIT;
        return ICF_FAILURE;
    }

    icf_port_memset(p_glb_port_info, ICF_NULL, 
                    sizeof(icf_global_port_info_st));

    /*
     * In case of success add a trace
     */
    *p_p_port_info = (icf_void_t*)p_glb_port_info;

#ifndef ICF_SSA_ES_UT
    /*Initialize the timner list with null */
#ifdef GENLIB_TIMER
    gl_tm_init();
#endif
#endif
 
     for( counter = 0;counter < ICF_MAX_TIMER_LIST_COUNT; counter++)
     {
        p_glb_port_info->glb_timer_data.timer_id_list[counter]=ICF_NULL;
     }

#ifdef ICF_NAT_MEDIA_TRAVERSAL
     for( counter = 0;counter < ICF_MAX_NAT_CONTEXT; counter++)
     {
        p_glb_port_info->win_ipc_info.nat_ipc_info[counter].is_free=ICF_TRUE;
     }
#endif
     return ICF_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_memory_init
*
*  DESCRIPTION    :   This function initializes common memory pools.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
icf_return_t
icf_port_memory_init(
        IN icf_void_t              *p_port_info,
        IN icf_memory_pool_table_st *p_cmn_pool_data,
        OUT icf_error_t            *p_ecode )
{
#ifdef GENLIB_MEMORY   
    icf_uint8_t              pool_counter = 0;
	icf_uint16_t				max_num_of_pools = 0;

    gl_error_t                gl_ecode;
    gl_pool_info_t            gl_pool_info;
    gl_return_t               ret_val = GL_SUCCESS;
#endif

#ifdef GENLIB_MEMORY

    p_port_info = p_port_info;

    /* Init Genlib memory mananger */    
    gl_mm_init();

    max_num_of_pools = p_cmn_pool_data->pool_count;
    /* Create common memory pool */
    for (pool_counter = 0; pool_counter < max_num_of_pools; pool_counter++)
    {
        ret_val = gl_mm_task_specific_cmn_pool_create(ICF_PORT_CMN_TASK_ID,
                &gl_pool_info,
                p_cmn_pool_data->pools[pool_counter].num_buffers,
                p_cmn_pool_data->pools[pool_counter].pool_buffer_size,
                &gl_ecode);

        if( GL_SUCCESS != ret_val )
        {
            *p_ecode = ICF_ERROR_MEM_INIT;
            return (ICF_FAILURE);
        }
    }
#else
    p_port_info = p_port_info;
    p_cmn_pool_data = p_cmn_pool_data;
    p_ecode = p_ecode;   
#endif    
    return (ICF_SUCCESS);
}


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_task_memory_init
*
*  DESCRIPTION    :   This function initializes  memory pools.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
icf_return_t
icf_port_task_memory_init(
         IN icf_void_t               *p_port_info,
         IN icf_memory_pool_table_st *p_task_pool_data,
         INOUT icf_error_t            *p_ecode)
{
#ifdef GENLIB_MEMORY    
    icf_uint8_t              pool_counter = 0;
    icf_uint16_t		     max_num_of_pools = 0;
    icf_global_port_info_st  *p_glb_port_info = 
        (icf_global_port_info_st*)p_port_info;

    gl_error_t                gl_ecode;
    gl_pool_id_t              gl_pool_id;
    gl_return_t               ret_val = GL_SUCCESS;
    icf_uint16_t		      pool_id = 0;
#endif

#ifdef GENLIB_MEMORY


    if ( ICF_MAX_NUM_POOL <= p_task_pool_data->pool_count)
    {
        *p_ecode = ICF_ERROR_MEM_INIT;
        return (ICF_FAILURE);
    }

    /* Create task/size specific memory pool */
    max_num_of_pools = p_task_pool_data->pool_count;
    for(pool_counter = 0; pool_counter < max_num_of_pools; pool_counter++)
    {
        ret_val = gl_mm_pool_create(ICF_PORT_CMN_TASK_ID,
                p_task_pool_data->pools[pool_counter].num_buffers,
                p_task_pool_data->pools[pool_counter].pool_buffer_size,
                &gl_pool_id,
                &gl_ecode);

        if( GL_SUCCESS != ret_val )
        {
            *p_ecode = ICF_ERROR_MEM_INIT;
            return (ICF_FAILURE);
        }
	pool_id = (icf_uint8_t)p_task_pool_data->pools[pool_counter].pool_id;
        p_glb_port_info->gl_pool_id_map[pool_id][0]
            = pool_id;
        p_glb_port_info->gl_pool_id_map[pool_id][1] = (icf_uint8_t)gl_pool_id;
    }
#else
    if (ICF_NULL == p_port_info)
    {}/* warning removal*/
    p_task_pool_data = p_task_pool_data;
    p_ecode = p_ecode;   
#endif    
    return (ICF_SUCCESS);

} /* End of icf_mem_init */

/*************************************************************************
*
*  FUNCTION NAME  :   icf_mem_deinit
*
*  DESCRIPTION    :   This function destroys memory pools.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_mem_deinit(
         IN icf_void_t               *p_port_info,
         INOUT icf_error_t            *p_ecode)
{
#ifdef GENLIB_MEMORY
    icf_uint8_t              pool_counter = 0;
    icf_global_port_info_st  *p_glb_port_info = 
        (icf_global_port_info_st*)p_port_info;

    gl_error_t      gl_ecode;
    gl_return_t     ret_val = GL_SUCCESS; 

    for(pool_counter=0; pool_counter<=6; pool_counter++)
    {
       if(ICF_NULL != p_glb_port_info->gl_pool_id_map[pool_counter][0])
       {
           ret_val = gl_mm_pool_delete (ICF_PORT_CMN_TASK_ID,
               p_glb_port_info->gl_pool_id_map[pool_counter][1], &gl_ecode);
       }
    }

    if ( GL_FAILURE == ret_val)
    {
        *p_ecode = ICF_ERROR_MEM_DEINIT;
        return (ICF_FAILURE);
    }
#else
    (void)p_port_info;
    p_port_info = p_port_info;
    p_ecode = p_ecode;   
#endif
    return (ICF_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_static_memget
*
*  DESCRIPTION    :   This function is called to allocate static buffers
*                     in the system.
*
*
*  RETURNS        :   Pointer to the allocated buffer if there is available
*                     memory in the system, else ICF_NULL with ecode as
*                     ICF_OUT_OF_MEM.
*
 *************************************************************************/

icf_void_t*
icf_port_static_memget(
        IN  icf_uint32_t          size,
        OUT icf_error_t    *p_ecode )
{
    icf_void_t*       p_buf = ICF_NULL;
#ifdef ICF_UT_TEST
    icf_return_t      ret_val = ICF_SUCCESS;
    if (ICF_NULL != p_persistent_glb_pdb)
    {
        ICF_FUNCTION_ENTER(p_persistent_glb_pdb)
    }
#endif

    /* SPR 19748: In case the request memory allocation size
     * is more than the following macro, return failre.
     * As System malloc should not be allowed to reach uint32
     * max size.
     */ 
    if( ICF_PORT_MAX_MEMORY_ALLOC_SIZE < size)
    {
       ICF_PRINT(((icf_uint8_t*)"\n[PORT] Memory Allocation failure , MAX size exceeded"))
       return ICF_NULL;
    }

#ifdef ICF_MEMORY_CALCULATION

    /* SPR 20008 : Allocating size + 4 bytes of memory, the first 4 bytes will 
       be used to store the value of the size variable. This will be used 
       in memory profiling to write the size into a file.
    */
    p_ecode = p_ecode;

    p_buf = (icf_void_t*) malloc(size + 4);

    if(ICF_NULL != p_buf) 
    {
        /* If able to get the memory and p_buf is not null */
        icf_port_memset(p_buf,0,size + 4);
        *((icf_uint32_t *)p_buf) = size;
        g_max_size += size;
	    printf("\nSTATIC MEMGET  : %u Total Memory:%u",size,g_max_size);
    }
    else 
    {
       /* If unable to get the memory print failure */
        perror("malloc");
    	printf("\n PORT_MEMGET failed for buffer size %u\n",size);
    }
#else

    p_buf = (icf_void_t*)malloc(size);


    if(p_buf == (icf_void_t*) ICF_NULL)
    {
        perror("malloc");
	    printf("\n STATIC_MEMGET failed for buffer size %u\n",size);
        *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
    }
    else
    {
        icf_port_memset(p_buf,0,size);
    }
#endif

#ifdef ICF_DEBUG_MEMPOOL
    /* SPR 20008 : Writing the size and other information in a file */
    p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
    if(p_memget_file_ptr)
    {
        fprintf(p_memget_file_ptr,",,,,,,,,,STATIC MEMGET,0x%x,%d,%d\n",
                p_buf,size,g_max_size);
        fclose(p_memget_file_ptr);
        p_memget_file_ptr = ICF_NULL;
    }
#endif
#ifdef ICF_UT_TEST
    if (ICF_NULL != p_persistent_glb_pdb)
    {
        ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    }
#endif

#ifdef ICF_MEMORY_CALCULATION
    return (p_buf + 4);
#else
	return p_buf;
#endif
} /* End of icf_static_memget */

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_static_memfree
*
*  DESCRIPTION    :   This function is called to free static buffers
*                     in the system.
*
*
*  RETURNS        :   ICF_SUCCESS/ICF_FAILURE
*
 *************************************************************************/

icf_return_t
icf_port_static_memfree(
        IN  icf_void_t     *p_buf,
        OUT icf_error_t    *p_ecode )
{
#ifdef ICF_MEMORY_CALCULATION
    /* SPR 20008 : Calculating size from the first 4 bytes */
	icf_uint32_t	size = 0;
	p_buf -= 4;
	size = *((icf_uint32_t *)p_buf);
	g_max_size -= size;
	printf("\nSTATIC MEMFREE : %u Total Memory: %u",size, g_max_size);
#endif

#ifdef ICF_DEBUG_MEMPOOL
    /* SPR 20008 : Writing the size and other information in a file */
    p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
    if(p_memget_file_ptr)
    {
        fprintf(p_memget_file_ptr,",,,,,,,,,STATIC MEMFREE,0x%x,%d,%d\n",
                p_buf,(-size),g_max_size);
        fclose(p_memget_file_ptr);
        p_memget_file_ptr=ICF_NULL;
    }
#endif

    if ( ICF_NULL != p_buf)
    {
        free ((void*)p_buf);
        /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18871)*/
        p_buf = ICF_NULL;
        return ICF_SUCCESS;
    }
    else
    {
         *p_ecode = ICF_ERROR_MEM_FREE_FAILURE;
        return ICF_FAILURE;
    }

} /* End of icf_static_memfree */

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_realloc
*
*  DESCRIPTION    :   reallocates memory
*
*  RETURNS        :   
*
*************************************************************************/

icf_void_t* 
icf_port_realloc(
        INOUT  icf_void_t           *p_port_info,
        IN  icf_uint32_t				size
        )
{

	return realloc(p_port_info,size);

}
/*************************************************************************
*
*  FUNCTION NAME  :  icf_port_print_mem_pool_stats 
*
*  DESCRIPTION    :   Prints Mem Pool Statistic
*
*  RETURNS        :   NONE
*
*************************************************************************/
icf_void_t icf_port_print_mem_pool_stats(icf_void_t)
{

#if (defined(ICF_PORT_VXWORKS) && defined(GENLIB_MEMORY))
/* For usage of Genlib Memory on VxWorks */

    ICF_GL_MM_DEBUG_PRINT(("\n--------------------------------------"));
    ICF_GL_MM_DEBUG_PRINT(("\n#### IPTK Memory Usage Statistics ####"));
    ICF_GL_MM_DEBUG_PRINT(("\n--------------------------------------"));

    ICF_GL_MM_DEBUG_PRINT(("\nBUF_SIZE\tBUF_QTY\t\tBUSY_BUF_QTY"));
#ifdef ICF_GL_MM_ADD_DEBUGGER
    ICF_GL_MM_DEBUG_PRINT(("\tCURR_MAX_USED"));
#endif
    for (i = 0; i < (ICF_MAX_NUM_COMMON_POOL + ICF_MAX_NUM_POOL); i++)
    {
        if(ICF_NULL != g_a_p_app_sp_pool_table[i])
        {
            ICF_GL_MM_DEBUG_PRINT(("\n%d\t\t%d\t\t%d",
                g_a_p_app_sp_pool_table[i]->buf_size,
                g_a_p_app_sp_pool_table[i]->buf_qty,
                g_a_p_app_sp_pool_table[i]->busy_buf_qty));
#ifdef ICF_GL_MM_ADD_DEBUGGER
            ICF_GL_MM_DEBUG_PRINT(("\t\t%d",
                g_a_p_app_sp_pool_table[i]->icf_curr_max_used));
#endif
			curr_mem_in_use += (g_a_p_app_sp_pool_table[i]->buf_size *
								g_a_p_app_sp_pool_table[i]->busy_buf_qty);
        }
    }
    ICF_GL_MM_DEBUG_PRINT(("\nCurrent Genlib Memory Usage = %ld bytes", 
								curr_mem_in_use));
    ICF_GL_MM_DEBUG_PRINT(("\n--------------------------------------\n"));

#else /* ICF_PORT_VXWORKS && GENLIB_MEMORY */
#ifdef ICF_DEBUG_MEMPOOL
#if !defined(ICF_MEMORY_CALCULATION)
    p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
    if(p_memget_file_ptr)
        fprintf(p_memget_file_ptr,"BUF_SIZE,BUF_QTY,BUSY_BUF_QTY,CURR_MAX_USED\n");
    for(;i<20;i++)
    {
#ifdef GENLIB_MEMORY
        if(ICF_NULL != g_a_p_app_sp_pool_table[i])
        {
            if(p_memget_file_ptr) 
            {
                fprintf(p_memget_file_ptr,"%d,%d,%d",g_a_p_app_sp_pool_table[i]->buf_size
                        ,g_a_p_app_sp_pool_table[i]->buf_qty
                        ,g_a_p_app_sp_pool_table[i]->busy_buf_qty);
#ifdef GL_MM_ADD_DEBUGGER
                fprintf(p_memget_file_ptr,",%d",g_a_p_app_sp_pool_table[i]->curr_max_used);
#endif
                fprintf(p_memget_file_ptr,"\n");

            }
        }
#endif        
    }
    fclose(p_memget_file_ptr);p_memget_file_ptr=ICF_NULL;
#endif
#endif    
#endif /* ICF_PORT_VXWORKS && GENLIB_MEMORY */
    return;
}



#ifdef ICF_DNS_LOOKUP_ENABLED    
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_dns_server_data_init
*
*  DESCRIPTION    :   This function initializes the ares module which 
*                     would be doing the DNS query
*
*  RETURNS:           It will return the address of tne new dns_server_data 
*
*************************************************************************/
icf_dns_server_data_st*
icf_port_dns_server_data_init(
        INOUT icf_global_port_info_st      *p_glb_port_info,
        OUT icf_error_t                    *p_ecode )
{
    icf_dns_server_data_st        **p_p_new_node = ICF_NULL;
    icf_config_data_st            *p_config_data = ICF_NULL;
    icf_dns_server_data_st        *p_dns_server_data = ICF_NULL;
    /* SPR 19189 */
    if(ICF_NULL == p_glb_port_info)
    {
        ICF_PRINT(((icf_uint8_t*)"\n[PORT] p_glb_port_info is NULL"))
        return ICF_NULL;
    }

    p_dns_server_data = p_glb_port_info->p_dns_server_data;

    /*If p_dns_server_data in global port in not NULL then fetch last node
     *in the list*/
    if(ICF_NULL != p_dns_server_data)
    {
        while(ICF_NULL != p_dns_server_data->p_next)
        {
            p_dns_server_data = (icf_dns_server_data_st *) \
                                p_dns_server_data->p_next;
        }
        p_p_new_node = &(p_dns_server_data->p_next);
    } /*Else Allocate list head */
    else
    {
	    p_p_new_node = &(p_glb_port_info->p_dns_server_data);
    }
 
    /*Allocate node pointer*/
    if ( ICF_NULL == (*p_p_new_node = 
                   (icf_dns_server_data_st*)icf_port_static_memget(\
                    sizeof(icf_dns_server_data_st), p_ecode)))
    {
          *p_ecode = ICF_ERROR_MEM_INIT;
          return (ICF_NULL);
    }
    else
    {
        if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                                    p_persistent_glb_pdb,
                                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                                    (icf_void_t *)&p_config_data, p_ecode))
        {
                  ICF_PRINT(((icf_uint8_t*)"\n[port_dns_client_init ]Failed in getting cfg data"));
                  icf_port_static_memfree((*p_p_new_node), p_ecode);
                  /*  static_memget did not make the value NULL */
                  *p_p_new_node = ICF_NULL; 
                  return ICF_NULL;
        }
        /*Check if DNS server address is configured by application*/
        if(ICF_NULL != p_config_data->p_dns_server_addr)
        {
            /*Then copy active server address to new allocated node dns_server_ip*/
            if(ICF_TRUE == p_config_data->sec_dns_server_active) 
            {
                /* SPR 19189 */
                if(ICF_NULL == p_config_data->p_sec_dns_server_addr)
                {
                    ICF_PRINT(((icf_uint8_t*)"\n[PORT] p_config_data->p_sec_dns_server_addr is NULL"))
                    /*SPR 19223 klocwork warning removal Added code to free
                     * new node */
                    icf_port_static_memfree((*p_p_new_node), p_ecode);
                    /*  static_memget did not make the value NULL */
                    *p_p_new_node = ICF_NULL; 
                    return ICF_NULL;
                }

                /* Assuming that DNS address will always be IPV4Address */
                icf_port_snprintf((*p_p_new_node)->dns_server_ip,
                        ICF_PORT_SIZEOF((*p_p_new_node)->dns_server_ip),(const icf_uint8_t*)"%d.%d.%d.%d", \
                        p_config_data->p_sec_dns_server_addr->addr.addr.ipv4_addr.octet_1, \
                        p_config_data->p_sec_dns_server_addr->addr.addr.ipv4_addr.octet_2, \
                        p_config_data->p_sec_dns_server_addr->addr.addr.ipv4_addr.octet_3, \
                        p_config_data->p_sec_dns_server_addr->addr.addr.ipv4_addr.octet_4);
               (*p_p_new_node)->dns_server_port = p_config_data->p_sec_dns_server_addr->port_num;
            }
            else
            {
                /* SPR 19189 */
                if(ICF_NULL == p_config_data->p_dns_server_addr)
                {
                    ICF_PRINT(((icf_uint8_t*)"\n[PORT] p_config_data->p_dns_server_addr is NULL"))
                    return ICF_NULL;
                }

                /*   Assuming that DNS address will always be IPV4Address */
                icf_port_snprintf((*p_p_new_node)->dns_server_ip,
                        ICF_PORT_SIZEOF((*p_p_new_node)->dns_server_ip),(const icf_uint8_t*)"%d.%d.%d.%d", \
                        p_config_data->p_dns_server_addr->addr.addr.ipv4_addr.octet_1, \
                        p_config_data->p_dns_server_addr->addr.addr.ipv4_addr.octet_2, \
                        p_config_data->p_dns_server_addr->addr.addr.ipv4_addr.octet_3, \
                        p_config_data->p_dns_server_addr->addr.addr.ipv4_addr.octet_4);
                (*p_p_new_node)->dns_server_port = p_config_data->p_dns_server_addr->port_num;
            }
        }
     }
    return (*p_p_new_node);
}
#endif
