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
 *     Copyright 2006, Aricent.
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

#ifdef ICF_DNS_LOOKUP_ENABLED
#include "ares.h"
#include "ares_dns.h"
#endif

#ifdef GENLIB_TIMER
#include "gl_tm_interfaces.h"
#endif

#ifdef GENLIB_MEMORY
extern  gl_mm_pool_t *g_a_p_app_sp_pool_table[GL_MM_MAX_TASK_POOL_QTY];
#endif
#ifdef ICF_DEBUG_MEMPOOL
extern FILE            *p_memget_file_ptr ;
/*extern FILE            *p_mem_free_file_ptr ;*/
/*extern FILE            *p_mem_cpy_file_ptr ;*/
#endif

/*#define ICF_MEMORY_CALCULATION*/
#ifdef ICF_MEMORY_CALCULATION
icf_uint32_t g_max_size ;
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

    p_buf = (icf_void_t*) malloc(size);

    if(ICF_NULL != p_buf) 
    {
        icf_port_memset(p_buf,0,size);
    }

    *((icf_uint32_t *)p_buf) = size;


    g_max_size += size;
	printf("\nMEMGET : %u	Total Mem:%u",size,g_max_size);
#else

    p_buf = (icf_void_t*) malloc(size);
    if(ICF_NULL != p_buf) 
    {
        icf_port_memset(p_buf,0,size);
    }

#endif 
#ifdef ICF_DEBUG_MEMPOOL
   p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
   if(p_memget_file_ptr)
   {
   	fprintf(p_memget_file_ptr,",,,,,,,,,MEMGET,0x%x,%d\n",p_buf,size);
	fclose(p_memget_file_ptr);p_memget_file_ptr=ICF_NULL;
   }
#endif
#endif
   
#ifdef ICF_MEMORY_CALCULATION
    return (p_buf + 4);
#else
	return p_buf;
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
    gl_pool_id_t         gl_pool_id;
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
#ifdef ICF_DEBUG_MEMPOOL
    
       p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
       if(p_memget_file_ptr)
       {
           fprintf(p_memget_file_ptr,"MEMFREE,0x%x\n",p_buf);
           fclose(p_memget_file_ptr);p_memget_file_ptr=ICF_NULL;
       }
     
#endif

#ifdef ICF_MEMORY_CALCULATION
	icf_uint32_t	size;
	p_buf -= 4;
	size = *((icf_uint32_t *)p_buf);
	g_max_size -= size;
	printf("\nMEMFREE: %u	Total Memory: %u",size, g_max_size);
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
#ifdef ICF_DNS_LOOKUP_ENABLED
    icf_return_t    		ret_val = ICF_SUCCESS;
#endif 
    
	p_glb_port_info = (icf_global_port_info_st*)icf_port_static_memget(\
            sizeof(icf_global_port_info_st), p_ecode);

    if ( ICF_NULL == p_glb_port_info)
    {
        *p_ecode = ICF_ERROR_MEM_INIT;
        return (ICF_FAILURE);
    }

	icf_port_memset(p_glb_port_info, ICF_NULL, 
						sizeof(icf_global_port_info_st));
#ifdef ICF_DNS_LOOKUP_ENABLED
	/* 
	 * Check if any name server is present in resolv.conf
	 * if it is then call ares init else the same would be 
	 * called next time when DNS query would be made
	 */ 
	if(ICF_TRUE == icf_port_is_nameserver_pres())
	{
		ret_val = icf_port_ares_init(p_glb_port_info,p_ecode);
	}
#endif
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

    p_buf = (icf_void_t*)malloc(size);


    if(p_buf == (icf_void_t*) ICF_NULL)
    {
         *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
    }
    else
    {
        icf_port_memset(p_buf,0,size);
    }
#ifdef ICF_DEBUG_MEMPOOL
    p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
    if(p_memget_file_ptr)
    {
        fprintf(p_memget_file_ptr,",,,,,,STATIC_MEMGET,0x%x,%d\n",p_buf,size);
        fclose(p_memget_file_ptr);
    }
#endif

    return p_buf;
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
#ifdef ICF_DEBUG_MEMPOOL  
    p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "a" );
    if(p_memget_file_ptr)
    {
        fprintf(p_memget_file_ptr,",,,,,,STATIC_MEMFREE,0x%x\n",p_buf);
        fclose(p_memget_file_ptr);
    }
#endif

    if ( ICF_NULL != p_buf)
    {
        free ((void*)p_buf);
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
#endif /* ICF_PORT_VXWORKS && GENLIB_MEMORY */
    return;
}



#ifdef ICF_DNS_LOOKUP_ENABLED    
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_ares_init
*
*  DESCRIPTION    :   This function initializes the ares module which 
*                     would be doing the DNS query
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_ares_init(
        INOUT icf_global_port_info_st      *p_glb_port_info,
        OUT icf_error_t                    *p_ecode )
{
    icf_uint8_t                   counter = ICF_NULL;     
    icf_ares_fds_st		**p_p_ares_port_info = ICF_NULL;


	p_p_ares_port_info = (icf_ares_fds_st **)(&(p_glb_port_info->p_ares_port_info));
       
	if ( ICF_NULL == (*p_p_ares_port_info = 
                   (icf_ares_fds_st*)icf_port_static_memget(\
                    sizeof(icf_ares_fds_st), p_ecode)))
	{
		icf_port_static_memfree(p_glb_port_info, p_ecode);
		*p_ecode = ICF_ERROR_MEM_INIT;
		return (ICF_FAILURE);
	}
	else
        {
	       icf_int32_t	status;
	       (*p_p_ares_port_info)->count = ICF_NULL;
	       (*p_p_ares_port_info)->curr_fd = ICF_NULL;
	       (*p_p_ares_port_info)->timer_duration = ICF_NULL;

	       for( counter = 0;counter < ICF_MAX_DNS_SERVER_FDS; counter++)
	       {
		       (*p_p_ares_port_info)->ares_fds[counter][0] = ICF_NULL;
		       (*p_p_ares_port_info)->ares_fds[counter][1] = ICF_NULL;
	       }

#ifdef ICF_PORT_VXWORKS                  
           status = ares_init ((ares_channel *)&(
                       ((*p_p_ares_port_info)->p_ares_channel)
                       ));
#else                            
	       status = ares_init ((ares_channel *)&((*p_p_ares_port_info)->p_ares_channel),
			       AF_INET);
#endif     

		   if (status != ARES_SUCCESS)
	       {
	             if(ARES_ECONNREFUSED == status)
	             {
			   ICF_PRINT(((icf_uint8_t *)"\n[PORT]:LookUpInit:Name servers not \
                                      compatible with selected Network Type"));
		     }
		     else
		     {
		            ICF_PRINT(((icf_uint8_t *)"\n[PORT]:LookUpInit:Intialization of ares \
                                         library failed"));
		     }

                       /* Klocwork warning removal. */
                       if (ICF_NULL != (*p_p_ares_port_info)->p_ares_channel)
                           icf_port_static_memfree((*p_p_ares_port_info)->p_ares_channel, p_ecode);

		       icf_port_static_memfree((*p_p_ares_port_info), p_ecode);
			       icf_port_static_memfree(p_glb_port_info, p_ecode);
			       *p_ecode = ICF_ERROR_MEM_INIT;
		       return (ICF_FAILURE);
	       }
	}
    
    return ICF_SUCCESS;
}
#endif


