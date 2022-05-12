/*****************************************************************************
* FUNCTION    : Source file for Generic library - Statistic APIs
*               implementation
******************************************************************************
*
* FILENAME    : gl_ulm_stats_api.c
*
* DESCRIPTION : This file implements string manipulation functionality of 
*               generic libraries. 
*
* DATE            NAME         REFERENCE        REASON
*----------   -------------    ---------    -------------------
* April'01      Rajesh Jain                 New file
*                                           Generic Libraries
* July' 01      Shobhit Bansal              Functions for case 
*                                           insensitive string compare
*                                           added
* 18/10/2001    Ravi Jindal     SPR-2788    Packaging of Genlib-Code.
* 19/10/2001    Ravi Jindal     SPR-2802    Adding Pre-ANSI-C features.
* 17/11/2001    Ravi Jindal     SPR-3107    Incoprate the defects and change
*                                           cosmmetics of file.
*
* 03/04/2002    Ravi Jindal     SPR-3959    Enhancements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/


#include "gl_ulm_interfaces.h"
#include "gl_ulm_stats_intrn_types.h"

#ifdef GL_STATS_ENABLED

/**********************************************************
 * Stats Data Defination for Memory Manager.
 **********************************************************/ 

 gl_stat_mm_bfit_glb_res_avl_st      g_p_mm_bfit_glb_res_avl = {0,0};
 gl_stat_mm_bfit_glb_api_err_st      g_p_mm_bfit_glb_api_err= {0,0,0,0};
 gl_stat_mm_bfit_glb_usage_st        g_p_mm_bfit_glb_usage={0,0,0,0,0};

 gl_stat_mm_bfit_ctx_sp_st            *g_a_mm_bfit_ctx_sp
    [GL_MM_MAX_COMMON_POOL_QTY]={GL_NULL};


 gl_stat_mm_usrsp_glb_res_avl_st     g_p_mm_usrsp_glb_res_avl={0,0};
 gl_stat_mm_usrsp_glb_api_err_st     g_p_mm_usrsp_glb_api_err={0,0,0,0};
 gl_stat_mm_usrsp_glb_usage_st       g_p_mm_usrsp_glb_usage={0,0,0,0};

 gl_stat_mm_usrsp_ctx_sp_st          *g_a_mm_usrsp_ctx_sp
    [GL_MM_MAX_TASK_POOL_QTY]={GL_NULL};


/**********************************************************
 * Stats Data Defination for Timer Manager.
 **********************************************************/ 

 gl_stat_tm_glb_res_avl_st      g_p_tm_glb_res_avl={0,0};
 gl_stat_tm_glb_api_err_st      g_p_tm_glb_api_err={0,0};
 gl_stat_tm_glb_usage_st        g_p_tm_glb_usage={0,0,0};

 gl_stat_tm_ctx_sp_st           *g_a_tm_ctx_sp[GL_TM_MAX_TIMER_LIST]={GL_NULL};


/**********************************************************
 * Stats Data Defination for Hash Table Manager.
 **********************************************************/ 

 gl_stat_htm_glb_res_avl_st      g_p_htm_glb_res_avl={0,0};
 gl_stat_htm_glb_api_err_st      g_p_htm_glb_api_err={0,0};
 gl_stat_htm_glb_usage_st        g_p_htm_glb_usage={0,0,0};

 gl_stat_htm_ctx_sp_st           *g_a_htm_ctx_sp[GL_HTM_MAX_HASH_TABLE]={GL_NULL};



/**********************************************************
 * Stats Data Defination for Link List Manager.
 **********************************************************/ 

 gl_stat_llm_glb_res_avl_st      g_p_llm_glb_res_avl={0,0};
 gl_stat_llm_glb_api_err_st      g_p_llm_glb_api_err={0,0};
 gl_stat_llm_glb_usage_st        g_p_llm_glb_usage={0};

 gl_stat_llm_ctx_sp_st           *g_a_llm_ctx_sp[GL_LLM_MAX_LIST]={GL_NULL};



/**********************************************************
 * Stats Data Defination for Message Queue Manager.
 **********************************************************/ 

 gl_stat_mqm_glb_res_avl_st      g_p_mqm_glb_res_avl={0,0};
 gl_stat_mqm_glb_api_err_st      g_p_mqm_glb_api_err={0,0};
 gl_stat_mqm_glb_usage_st        g_p_mqm_glb_usage={0,0};

 gl_stat_mqm_ctx_sp_st           *g_a_mqm_ctx_sp[GL_MQM_MAX_MESG_Q]={GL_NULL};


#ifdef GL_MT_ENABLED
 gw_boolean                     g_mm_stat_init  = GL_FALSE;
 gw_boolean                     g_tm_stat_init  = GL_FALSE;
 gw_boolean                     g_htm_stat_init = GL_FALSE;
 gw_boolean                     g_llm_stat_init = GL_FALSE;
 gw_boolean                     g_mqm_stat_init = GL_FALSE;
 
 gl_error_t                     g_gl_stat_error_code;

 GL_MUTEX_ID                    g_mm_bfit_stat_sem;
 GL_MUTEX_ID                    g_mm_usrsp_stat_sem;
 GL_MUTEX_ID                    g_tm_stat_sem;
 GL_MUTEX_ID                    g_htm_stat_sem;
 GL_MUTEX_ID                    g_llm_stat_sem;
 GL_MUTEX_ID                    g_mqm_stat_sem;
#endif
 

 gw_boolean                g_stats_state_table[GL_STAT_TYPE_ALL]
    [GL_STAT_IDS_ALL_STATS] = { {GL_FALSE,GL_FALSE},
                                {GL_FALSE,GL_FALSE},
                                {GL_FALSE,GL_FALSE},
                                {GL_FALSE,GL_FALSE},
                                {GL_FALSE,GL_FALSE},
                                {GL_FALSE,GL_FALSE},
                                {GL_FALSE,GL_FALSE}
                               };
    
 gl_stat_reinit_func       g_stat_reinit_fn_table[GL_STAT_TYPE_ALL] = 
                            { gl_mm_stat_bfit_reinit_fn,
                              gl_mm_stat_usrsp_reinit_fn,
                              gl_tm_stat_reinit_fn,
                              gl_htm_stat_reinit_fn,
                              gl_llm_stat_reinit_fn,
                              gl_mqm_stat_reinit_fn,
                              gl_all_stat_reinit_fn
                            };
 
 gl_stat_get_func          g_stat_get_fn_table[GL_STAT_TYPE_ALL] =
                            { gl_mm_stat_bfit_get_fn,
                              gl_mm_stat_usrsp_get_fn,
                              gl_tm_stat_get_fn,
                              gl_htm_stat_get_fn,
                              gl_llm_stat_get_fn,
                              gl_mqm_stat_get_fn 
                            };
 
    
/*****************************************************************************
* Function Name    : gl_reinit_stats
* Arguments        : gl_stat_type_et    stat_type,
*                    gl_stat_ids_et     stat_id,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Intialize the stats.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_reinit_stats

#ifdef _ANSI_C_
    ( gl_stat_type_et           stat_type,
      gl_stat_ids_et            stat_id, 
      gl_error_t                *p_ecode
    )
#else
    ( stat_type, stat_id, p_ecode )
      gl_stat_type_et           stat_type;
      gl_stat_ids_et            stat_id; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    if((GL_NULL == stat_type) || (GL_STAT_TYPE_ALL < stat_type))
    {
        *p_ecode = GL_ULM_ERR_INV_STAT_TYPE;
        return GL_FAILURE;
    }
    if((GL_NULL == stat_id) || (GL_STAT_IDS_ALL_STATS < stat_id))
    {
        *p_ecode = GL_ULM_ERR_INV_STAT_ID;
        return GL_FAILURE;
    }
    
    g_stat_reinit_fn_table[stat_type - 1](stat_id);

    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_get_stats
* Arguments        : gw_boolean         reset_flag,
*                    gl_stat_type_et    stat_type,
*                    gl_stat_scope_st   *p_stat_scope,
*                    gl_pvoid_t         p_stats_data,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Return the stats.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_get_stats

#ifdef _ANSI_C_
    ( gw_boolean                reset_flag,
      gl_stat_type_et           stat_type,
      gl_stat_scope_st          *p_stat_scope,
      gl_pvoid_t                p_stats_data,
      gl_error_t                *p_ecode
    )
#else
    ( reset_flag, stat_type, p_stat_scope, p_stats_data, p_ecode )
      gw_boolean                reset_flag;
      gl_stat_type_et           stat_type;
      gl_stat_scope_st          *p_stat_scope; 
      gl_pvoid_t                p_stats_data;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    if((GL_NULL == stat_type) || (GL_STAT_TYPE_ALL <= stat_type))
    {
        *p_ecode = GL_ULM_ERR_INV_STAT_TYPE;
        return GL_FAILURE;
    }
    
    if((GL_NULL == p_stat_scope->stat_id) || 
       (GL_STAT_IDS_ALL_STATS <= p_stat_scope->stat_id))
    {
        *p_ecode = GL_ULM_ERR_INV_STAT_ID;
        return GL_FAILURE;
    }
    
    if(GL_TRUE == g_stats_state_table[stat_type - 1][p_stat_scope->stat_id])
    {
        g_stat_get_fn_table[stat_type - 1] (p_stat_scope, p_stats_data);
    }
    
    if(GL_TRUE == reset_flag)
    {
        g_stat_reinit_fn_table[stat_type - 1](p_stat_scope->stat_id);
    }
    return GL_SUCCESS;
}




/*****************************************************************************
* Function Name    : gl_set_stats_activity
* Arguments        : gw_boolean         reset_flag,
*                    gl_stat_type_et    stat_type,
*                    gl_stat_ids_et     stat_id,
*                    gw_boolean         enable_stats_flag,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Return the stats.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_set_stats_activity

#ifdef _ANSI_C_
    ( gw_boolean                reset_flag,
      gl_stat_type_et           stat_type,
      gl_stat_ids_et            stat_id,
      gw_boolean                enable_stats_flag,
      gl_error_t                *p_ecode
    )
#else
    ( reset_flag, stat_type, stat_id, enable_stats_flag, p_ecode )
      gw_boolean                reset_flag;
      gl_stat_type_et           stat_type;
      gl_stat_ids_et            stat_id;
      gw_boolean                enable_stats_flag;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   i;
    gw_S32bit                   j;
    
    if((GL_NULL == stat_type) || (GL_STAT_TYPE_ALL < stat_type))
    {
        *p_ecode = GL_ULM_ERR_INV_STAT_TYPE;
        return GL_FAILURE;
    }
    if((GL_NULL == stat_id) || (GL_STAT_IDS_ALL_STATS < stat_id))
    {
        *p_ecode = GL_ULM_ERR_INV_STAT_ID;
        return GL_FAILURE;
    }
    
    if(GL_TRUE == enable_stats_flag)
    {
        if(GL_STAT_TYPE_ALL == stat_type)
        {
            for(i=0; i<GL_STAT_TYPE_ALL; i++)
            {
                if(GL_STAT_IDS_ALL_STATS == stat_id)
                {
                    for(j=0; j<GL_STAT_IDS_ALL_STATS; j++)
                    {
                        g_stats_state_table[i][j] = GL_TRUE;
                    }
                }
                else
                {
                    g_stats_state_table[i][stat_id - 1] = GL_TRUE;
                }
            }
        }
        else
        {
            g_stats_state_table[stat_type - 1][stat_id - 1] = GL_TRUE;
        }
    }
    else
    {
        if(GL_STAT_TYPE_ALL == stat_type)
        {
            for(i=0; i<GL_STAT_TYPE_ALL; i++)
            {
                if(GL_STAT_IDS_ALL_STATS == stat_id)
                {
                    for(j=0; j<GL_STAT_IDS_ALL_STATS; j++)
                    {
                        g_stats_state_table[i][j] = GL_FALSE;
                    }
                }
                else
                {
                    g_stats_state_table[i][stat_id - 1] = GL_FALSE;
                }
            }
        }
        else
        {
            g_stats_state_table[stat_type - 1][stat_id - 1] = GL_FALSE;
        }
    }
    
    if(GL_TRUE == reset_flag)
    {
        g_stat_reinit_fn_table[stat_type - 1](stat_id);
    }
        
    return GL_SUCCESS;
}



/****************************************
 * Local Functions Defination.
 ***************************************/


/*****************************************************************************
* Function Name    : gl_mm_stat_allocte_bfit_memory
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mm_stat_allocte_bfit_memory
 
#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_MM_BFIT_MT_LOCK()
    
    g_a_mm_bfit_ctx_sp[pool_id] = (gl_stat_mm_bfit_ctx_sp_st *)
        SYS_MALLOC(sizeof(gl_stat_mm_bfit_ctx_sp_st));
    
    if(GL_NULL == g_a_mm_bfit_ctx_sp[pool_id])
    {
        GL_STAT_MM_BFIT_MT_UNLOCK()
        
        *p_ecode = GL_MM_ERR_MALLOC_FAILED; 
        return GL_FAILURE;
    }
    
    g_a_mm_bfit_ctx_sp[pool_id]->g_p_mm_bfit_ctx_sp_usage.
        num_busy_buf_peak_reached = GL_NULL;
    g_a_mm_bfit_ctx_sp[pool_id]->g_p_mm_bfit_ctx_sp_usage.
        num_buf_corrupted = GL_NULL;
    g_a_mm_bfit_ctx_sp[pool_id]->g_p_mm_bfit_ctx_sp_usage.
        num_buf_corrected_af_corruption = GL_NULL;
    g_a_mm_bfit_ctx_sp[pool_id]->g_p_mm_bfit_ctx_sp_usage.
        num_get_buf_reqs = GL_NULL;
    g_a_mm_bfit_ctx_sp[pool_id]->g_p_mm_bfit_ctx_sp_usage.
        num_busy_bufs = GL_NULL;
    g_a_mm_bfit_ctx_sp[pool_id]->g_p_mm_bfit_ctx_sp_usage.
        num_bufs_total = GL_NULL;
    
    
    g_a_mm_bfit_ctx_sp[pool_id]->g_p_mm_bfit_ctx_sp_res_avl.
        num_get_buf_fail_bufs_busy = GL_NULL;

    GL_STAT_MM_BFIT_MT_UNLOCK()
    
    return GL_SUCCESS;
    
}



/*****************************************************************************
* Function Name    : gl_mm_stat_allocte_usrsp_memory
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mm_stat_allocte_usrsp_memory
 
#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_MM_USRSP_MT_LOCK()
    
    g_a_mm_usrsp_ctx_sp[pool_id] = (gl_stat_mm_usrsp_ctx_sp_st *)
        SYS_MALLOC(sizeof(gl_stat_mm_usrsp_ctx_sp_st));
    
    if(GL_NULL == g_a_mm_usrsp_ctx_sp[pool_id])
    {
        GL_STAT_MM_USRSP_MT_UNLOCK()
    
        *p_ecode = GL_MM_ERR_MALLOC_FAILED; 
        return GL_FAILURE;
    }
    
    g_a_mm_usrsp_ctx_sp[pool_id]->g_p_mm_usrsp_ctx_sp_usage.
        num_busy_buf_peak_reached = GL_NULL;
    g_a_mm_usrsp_ctx_sp[pool_id]->g_p_mm_usrsp_ctx_sp_usage.
        num_buf_corrupted = GL_NULL;
    g_a_mm_usrsp_ctx_sp[pool_id]->g_p_mm_usrsp_ctx_sp_usage.
        num_buf_corrected_af_corruption = GL_NULL;
    g_a_mm_usrsp_ctx_sp[pool_id]->g_p_mm_usrsp_ctx_sp_usage.
        num_get_buf_reqs = GL_NULL;
    g_a_mm_usrsp_ctx_sp[pool_id]->g_p_mm_usrsp_ctx_sp_usage.
        num_busy_bufs = GL_NULL;
    g_a_mm_usrsp_ctx_sp[pool_id]->g_p_mm_usrsp_ctx_sp_usage.
        num_bufs_total = GL_NULL;
    
    
    g_a_mm_usrsp_ctx_sp[pool_id]->g_p_mm_usrsp_ctx_sp_res_avl.
        num_get_buf_fail_bufs_busy = GL_NULL;

    GL_STAT_MM_USRSP_MT_UNLOCK()

    return GL_SUCCESS;
}
    


/*****************************************************************************
* Function Name    : gl_tm_stat_allocte_memory
* Arguments        : gl_timer_list_t    timer_list_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
gl_return_t
gl_tm_stat_allocte_memory
 
#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, p_ecode )
      gl_timer_list_t           timer_list_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_TM_MT_LOCK()
    
    g_a_tm_ctx_sp[timer_list_id] = (gl_stat_tm_ctx_sp_st *)
        SYS_MALLOC(sizeof(gl_stat_tm_ctx_sp_st));
    
    if(GL_NULL == g_a_tm_ctx_sp[timer_list_id])
    {
        GL_STAT_TM_MT_UNLOCK()

        *p_ecode = GL_TM_ERR_MALLOC_FAILED; 
        return GL_FAILURE;
    }
    
    g_a_tm_ctx_sp[timer_list_id]->g_p_tm_ctx_sp_usage.
        num_timer_runing_peak_reached = GL_NULL;
    g_a_tm_ctx_sp[timer_list_id]->g_p_tm_ctx_sp_usage.
        num_start_timer_reqs = GL_NULL;
    g_a_tm_ctx_sp[timer_list_id]->g_p_tm_ctx_sp_usage.
        num_timer_runing = GL_NULL;
    g_a_tm_ctx_sp[timer_list_id]->g_p_tm_ctx_sp_usage.
        num_total_timer_cfg = GL_NULL;
    
    g_a_tm_ctx_sp[timer_list_id]->g_p_tm_ctx_sp_res_avl.
        num_start_timer_req_fails = GL_NULL;

    GL_STAT_TM_MT_UNLOCK()
    
    return GL_SUCCESS;
}
    


/*****************************************************************************
* Function Name    : gl_htm_stat_allocte_memory
* Arguments        : gl_hash_table_t    hash_table_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_stat_allocte_memory
 
#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id,
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_ecode )
      gl_hash_table_t           hash_table_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_HTM_MT_LOCK()
    
    g_a_htm_ctx_sp[hash_table_id] = (gl_stat_htm_ctx_sp_st *)
        SYS_MALLOC(sizeof(gl_stat_htm_ctx_sp_st));
    
    if(GL_NULL == g_a_htm_ctx_sp[hash_table_id])
    {
        GL_STAT_HTM_MT_UNLOCK()

        *p_ecode = GL_HTM_ERR_NO_RESOURCES; 
        return GL_FAILURE;
    }
    
    g_a_htm_ctx_sp[hash_table_id]->g_p_htm_ctx_sp_usage.
        num_keys_peak_reached = GL_NULL;
    g_a_htm_ctx_sp[hash_table_id]->g_p_htm_ctx_sp_usage.
        num_key_max_any_bucket = GL_NULL;
    g_a_htm_ctx_sp[hash_table_id]->g_p_htm_ctx_sp_usage.
        num_key_min_any_bucket = 100000;
    g_a_htm_ctx_sp[hash_table_id]->g_p_htm_ctx_sp_usage.
        num_total_key_cfg = GL_NULL;
    
    g_a_htm_ctx_sp[hash_table_id]->g_p_htm_ctx_sp_res_avl.
        num_add_key_req_fails = GL_NULL;

    GL_STAT_HTM_MT_UNLOCK()
    
    return GL_SUCCESS;
}
    



/*****************************************************************************
* Function Name    : gl_llm_stat_allocte_memory
* Arguments        : gl_list_t          link_list_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
gl_return_t
gl_llm_stat_allocte_memory
 
#ifdef _ANSI_C_
    ( gl_list_t                 link_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( link_list_id, p_ecode )
      gl_list_t                 link_list_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_LLM_MT_LOCK()
    
    g_a_llm_ctx_sp[link_list_id] = (gl_stat_llm_ctx_sp_st *)
        SYS_MALLOC(sizeof(gl_stat_llm_ctx_sp_st));
    
    if(GL_NULL == g_a_llm_ctx_sp[link_list_id])
    {
        GL_STAT_LLM_MT_UNLOCK()

        *p_ecode = GL_LLM_ERR_NO_RESOURCE; 
        return GL_FAILURE;
    }
    
    g_a_llm_ctx_sp[link_list_id]->g_p_llm_ctx_sp_usage.
        num_node_peak_reached = GL_NULL;
    
    g_a_llm_ctx_sp[link_list_id]->g_p_llm_ctx_sp_res_avl.
        num_add_node_req_fails = GL_NULL;

    GL_STAT_LLM_MT_UNLOCK()
    
    return GL_SUCCESS;
}
    



/*****************************************************************************
* Function Name    : gl_mqm_stat_allocte_memory
* Arguments        : gl_mq_id_t         mesg_q_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_stat_allocte_memory
 
#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_MQM_MT_LOCK()
    
    g_a_mqm_ctx_sp[mesg_q_id] = (gl_stat_mqm_ctx_sp_st *)
        SYS_MALLOC(sizeof(gl_stat_mqm_ctx_sp_st));
    
    if(GL_NULL == g_a_mqm_ctx_sp[mesg_q_id])
    {
        GL_STAT_MQM_MT_UNLOCK()

        *p_ecode = GL_MQM_ERR_NO_RESOURCE; 
        return GL_FAILURE;
    }
    
    g_a_mqm_ctx_sp[mesg_q_id]->g_p_mqm_ctx_sp_usage.
        num_msg_peak_reached = GL_NULL;
    g_a_mqm_ctx_sp[mesg_q_id]->g_p_mqm_ctx_sp_usage.
        num_msg_max_any_priority_level = GL_NULL;
    g_a_mqm_ctx_sp[mesg_q_id]->g_p_mqm_ctx_sp_usage.
        num_msg_min_any_priority_level = 100000;
    g_a_mqm_ctx_sp[mesg_q_id]->g_p_mqm_ctx_sp_usage.
        num_curr_msg_qty = GL_NULL;
    g_a_mqm_ctx_sp[mesg_q_id]->g_p_mqm_ctx_sp_usage.
        num_total_msg_cfg = GL_NULL;
    
    g_a_mqm_ctx_sp[mesg_q_id]->g_p_mqm_ctx_sp_res_avl.
        num_send_msg_req_fails = GL_NULL;
    g_a_mqm_ctx_sp[mesg_q_id]->g_p_mqm_ctx_sp_res_avl.
        num_recv_msg_req_fails = GL_NULL;

    GL_STAT_MQM_MT_UNLOCK()
    
    return GL_SUCCESS;
}
    





/*****************************************************************************
* Function Name    : gl_mm_stat_free_bfit_memory
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Free the memory allocated for the context specific
*                    Stats for the given pool-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mm_stat_free_bfit_memory
 
#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_MM_BFIT_MT_LOCK()
    
    if(GL_NULL != g_a_mm_bfit_ctx_sp[pool_id])
    {
        SYS_FREE(g_a_mm_bfit_ctx_sp[pool_id]);
        g_a_mm_bfit_ctx_sp[pool_id] = GL_NULL;
    }
    
    GL_STAT_MM_BFIT_MT_UNLOCK()
    
    /*
     * To remove warning.
     */
    (void)*p_ecode;

    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_mm_stat_free_usrsp_memory
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Free the memory allocated for the context specific
*                    Stats for the given pool-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mm_stat_free_usrsp_memory
 
#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_MM_USRSP_MT_LOCK()
    
    if(GL_NULL != g_a_mm_usrsp_ctx_sp[pool_id])
    {
        SYS_FREE(g_a_mm_usrsp_ctx_sp[pool_id]);
        g_a_mm_usrsp_ctx_sp[pool_id] = GL_NULL;
    }
    
    GL_STAT_MM_USRSP_MT_UNLOCK()

    /*
     * To remove warning.
     */
    (void)*p_ecode;

    return GL_SUCCESS;
}
    


/*****************************************************************************
* Function Name    : gl_tm_stat_free_memory
* Arguments        : gl_timer_list_t    timer_list_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Free the memory allocated for the context specific
*                    Stats for the given timer-list-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_tm_stat_free_memory
 
#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, p_ecode )
      gl_timer_list_t           timer_list_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_TM_MT_LOCK()
    
    if(GL_NULL != g_a_tm_ctx_sp[timer_list_id])
    {
        SYS_FREE(g_a_tm_ctx_sp[timer_list_id]);
        g_a_tm_ctx_sp[timer_list_id] = GL_NULL;
    }
    
    GL_STAT_TM_MT_UNLOCK()
    
    /*
     * To remove warning.
     */
    (void)*p_ecode;

    return GL_SUCCESS;
}
    


/*****************************************************************************
* Function Name    : gl_htm_stat_free_memory
* Arguments        : gl_hash_table_t    hash_table_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Free the memory allocated for the context specific
*                    Stats for the given hash-table-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_htm_stat_free_memory
 
#ifdef _ANSI_C_
    ( gl_hash_table_t           hash_table_id,
      gl_error_t                *p_ecode
    )
#else
    ( hash_table_id, p_ecode )
      gl_hash_table_t           hash_table_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_HTM_MT_LOCK()
    
    if(GL_NULL != g_a_htm_ctx_sp[hash_table_id])
    {
        SYS_FREE(g_a_htm_ctx_sp[hash_table_id]);
        g_a_htm_ctx_sp[hash_table_id] = GL_NULL;
    }
    
    /*
     * To remove warning.
     */
    (void)*p_ecode;

    GL_STAT_HTM_MT_UNLOCK()
    
    return GL_SUCCESS;
}
    



/*****************************************************************************
* Function Name    : gl_llm_stat_free_memory
* Arguments        : gl_list_t          link_list_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Free the memory allocated for the context specific
*                    Stats for the given link-list-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_llm_stat_free_memory
 
#ifdef _ANSI_C_
    ( gl_list_t                 link_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( link_list_id, p_ecode )
      gl_list_t                 link_list_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_LLM_MT_LOCK()
    
    if(GL_NULL != g_a_llm_ctx_sp[link_list_id])
    {
        SYS_FREE(g_a_llm_ctx_sp[link_list_id]);
        g_a_llm_ctx_sp[link_list_id] = GL_NULL;
    }

    GL_STAT_LLM_MT_UNLOCK()
    
    /*
     * To remove warning.
     */
    (void)*p_ecode;

    return GL_SUCCESS;
}
    



/*****************************************************************************
* Function Name    : gl_mqm_stat_free_memory
* Arguments        : gl_mq_id_t         mesg_q_id,
*                    gl_error_t         *p_ecode 
* Return value     : gl_return_t
* Functionality    : Free the memory allocated for the context specific
*                    Stats for the given message-queue-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_stat_free_memory
 
#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gl_error_t                *p_ecode;
#endif
    
 
/****************************************************************************/
{
    GL_STAT_MQM_MT_LOCK()
    
    if(GL_NULL != g_a_mqm_ctx_sp[mesg_q_id])
    {
        SYS_FREE(g_a_mqm_ctx_sp[mesg_q_id]);
        g_a_mqm_ctx_sp[mesg_q_id] = GL_NULL;
    }
    
    GL_STAT_MQM_MT_UNLOCK()
    
    /*
     * To remove warning.
     */
    (void)*p_ecode;

    return GL_SUCCESS;
}
    





/*****************************************************************************
* Function Name    : gl_mm_stat_bfit_reinit_fn
* Arguments        : gl_stat_ids_et   stat_id
* Return value     : void
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
 void gl_mm_stat_bfit_reinit_fn
 
#ifdef _ANSI_C_
    ( gl_stat_ids_et          stat_id )
#else
    ( stat_id )
      gl_stat_ids_et          stat_id;
#endif
    
/****************************************************************************/
{
    gl_pool_id_t                pool_id;
    
    GL_STAT_MM_BFIT_MT_LOCK()

    switch(stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            g_p_mm_bfit_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_mm_bfit_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            g_p_mm_bfit_glb_api_err.num_inv_buf_ptr_in_rels = GL_NULL;
            g_p_mm_bfit_glb_api_err.num_buf_rels_req_free_buf = GL_NULL;
            g_p_mm_bfit_glb_api_err.num_in_param_validation_fails =
                GL_NULL;
            g_p_mm_bfit_glb_api_err.num_delete_req_non_empty_pool = 
                GL_NULL;
            break;
            
            
        case GL_STAT_ID_GLB_USAGE:
            g_p_mm_bfit_glb_usage.num_oversize_buf_aloc = GL_NULL;
            g_p_mm_bfit_glb_usage.num_get_buf_fails = GL_NULL;
            g_p_mm_bfit_glb_usage.num_buf_corrupted = GL_NULL;
            g_p_mm_bfit_glb_usage.num_buf_corrected_af_corruption = 
                GL_NULL;
            g_p_mm_bfit_glb_usage.num_get_buf_reqs = GL_NULL;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            for(pool_id = 0; pool_id < GL_MM_MAX_COMMON_POOL_QTY; pool_id++)
            {         
                if(GL_NULL != g_a_mm_bfit_ctx_sp[pool_id])
                { 
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_busy_buf_peak_reached = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_buf_corrupted = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_buf_corrected_af_corruption = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_get_buf_reqs = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_busy_bufs = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_bufs_total = GL_NULL;
                }
            }
            
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            
            for(pool_id = 0; pool_id < GL_MM_MAX_COMMON_POOL_QTY; pool_id++)
            {         
                if(GL_NULL != g_a_mm_bfit_ctx_sp[pool_id])
                {
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_res_avl.
                        num_get_buf_fail_bufs_busy = GL_NULL;
                }
            }
            
            break;
            
        case GL_STAT_IDS_ALL_STATS:
            g_p_mm_bfit_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_mm_bfit_glb_res_avl.num_mutex_opr_fails = GL_NULL;


            g_p_mm_bfit_glb_api_err.num_inv_buf_ptr_in_rels = GL_NULL;
            g_p_mm_bfit_glb_api_err.num_buf_rels_req_free_buf = GL_NULL;
            g_p_mm_bfit_glb_api_err.num_in_param_validation_fails =
                GL_NULL;
            g_p_mm_bfit_glb_api_err.num_delete_req_non_empty_pool = 
                GL_NULL;


            g_p_mm_bfit_glb_usage.num_oversize_buf_aloc = GL_NULL;
            g_p_mm_bfit_glb_usage.num_get_buf_fails = GL_NULL;
            g_p_mm_bfit_glb_usage.num_buf_corrupted = GL_NULL;
            g_p_mm_bfit_glb_usage.num_buf_corrected_af_corruption = GL_NULL;
            g_p_mm_bfit_glb_usage.num_get_buf_reqs = GL_NULL;


            for(pool_id = 0; pool_id < GL_MM_MAX_COMMON_POOL_QTY; pool_id++)
            {         
                if(GL_NULL != g_a_mm_bfit_ctx_sp[pool_id])
                {
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_busy_buf_peak_reached = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_buf_corrupted = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_buf_corrected_af_corruption = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_get_buf_reqs = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_busy_bufs = GL_NULL;
                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.
                        num_bufs_total = GL_NULL;


                    g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_res_avl.
                        num_get_buf_fail_bufs_busy = GL_NULL;
                }
            }
            break;
            
        default:
            break;
            
    }
    
    GL_STAT_MM_BFIT_MT_UNLOCK()
}



      
/*****************************************************************************
* Function Name    : gl_mm_stat_usrsp_reinit_fn
* Arguments        : gl_stat_ids_et   stat_id
* Return value     : void
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
 void gl_mm_stat_usrsp_reinit_fn
 
#ifdef _ANSI_C_
    ( gl_stat_ids_et          stat_id )
#else
    ( stat_id )
      gl_stat_ids_et          stat_id;
#endif
    
/****************************************************************************/
{
    gl_pool_id_t                pool_id;
    
    GL_STAT_MM_USRSP_MT_LOCK()

    switch(stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            g_p_mm_usrsp_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_mm_usrsp_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            g_p_mm_usrsp_glb_api_err.num_inv_buf_ptr_in_rels = GL_NULL;
            g_p_mm_usrsp_glb_api_err.num_buf_rels_req_free_buf = GL_NULL;
            g_p_mm_usrsp_glb_api_err.num_in_param_validation_fails = 
                GL_NULL;
            g_p_mm_usrsp_glb_api_err.num_delete_req_non_empty_pool = 
                GL_NULL;
            break;
            
            
        case GL_STAT_ID_GLB_USAGE:
            g_p_mm_usrsp_glb_usage.num_get_buf_fails = GL_NULL;
            g_p_mm_usrsp_glb_usage.num_buf_corrupted = GL_NULL;
            g_p_mm_usrsp_glb_usage.num_buf_corrected_af_corruption = GL_NULL;
            g_p_mm_usrsp_glb_usage.num_get_buf_reqs = GL_NULL;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            for(pool_id = 0; pool_id < GL_MM_MAX_COMMON_POOL_QTY; pool_id++)
            {         
                if(GL_NULL != g_a_mm_usrsp_ctx_sp[pool_id])
                {
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_busy_buf_peak_reached = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_buf_corrupted = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_buf_corrected_af_corruption = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_get_buf_reqs = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_busy_bufs = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_bufs_total = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            for(pool_id = 0; pool_id < GL_MM_MAX_COMMON_POOL_QTY; pool_id++)
            {         
                if(GL_NULL != g_a_mm_usrsp_ctx_sp[pool_id])
                {
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_res_avl
                        .num_get_buf_fail_bufs_busy = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
            g_p_mm_usrsp_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_mm_usrsp_glb_res_avl.num_mutex_opr_fails = GL_NULL;


            g_p_mm_usrsp_glb_api_err.num_inv_buf_ptr_in_rels = GL_NULL;
            g_p_mm_usrsp_glb_api_err.num_buf_rels_req_free_buf = GL_NULL;
            g_p_mm_usrsp_glb_api_err.num_in_param_validation_fails = 
                GL_NULL;
            g_p_mm_usrsp_glb_api_err.num_delete_req_non_empty_pool =
                GL_NULL;


            g_p_mm_usrsp_glb_usage.num_get_buf_fails = GL_NULL;
            g_p_mm_usrsp_glb_usage.num_buf_corrupted = GL_NULL;
            g_p_mm_usrsp_glb_usage.num_buf_corrected_af_corruption = GL_NULL;
            g_p_mm_usrsp_glb_usage.num_get_buf_reqs = GL_NULL;

            
            for(pool_id = 0; pool_id < GL_MM_MAX_COMMON_POOL_QTY; pool_id++)
            {         
                if(GL_NULL != g_a_mm_bfit_ctx_sp[pool_id])
                {
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_busy_buf_peak_reached = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_buf_corrupted = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_buf_corrected_af_corruption = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_get_buf_reqs = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_busy_bufs = GL_NULL;
                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.
                        num_bufs_total = GL_NULL;


                    g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_res_avl
                        .num_get_buf_fail_bufs_busy = GL_NULL;
                }
            }
            break;
            
        default:
            break;
            
    }
    
    GL_STAT_MM_USRSP_MT_LOCK()
}



/*****************************************************************************
* Function Name    : gl_tm_stat_reinit_fn
* Arguments        : gl_stat_ids_et   stat_id
* Return value     : void
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
 void gl_tm_stat_reinit_fn
 
#ifdef _ANSI_C_
    ( gl_stat_ids_et          stat_id )
#else
    ( stat_id )
      gl_stat_ids_et          stat_id;
#endif
    
/****************************************************************************/
{
    gl_timer_list_t             timer_list_id;
    
    GL_STAT_TM_MT_LOCK()

    switch(stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            g_p_tm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_tm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            g_p_tm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_tm_glb_api_err.num_delete_req_non_empty_list = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            g_p_tm_glb_usage.num_stop_timer_timer_exp = GL_NULL;
            g_p_tm_glb_usage.num_start_timer_fails = GL_NULL;
            g_p_tm_glb_usage.num_start_timer_reqs = GL_NULL;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            for(timer_list_id = 0; timer_list_id < GL_TM_MAX_TIMER_LIST;
                   timer_list_id++)
            {         
                if(GL_NULL != g_a_tm_ctx_sp[timer_list_id])
                {
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_timer_runing_peak_reached = GL_NULL;
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_start_timer_reqs = GL_NULL;
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_timer_runing = GL_NULL;
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_total_timer_cfg = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            for(timer_list_id = 0; timer_list_id < GL_TM_MAX_TIMER_LIST; 
                    timer_list_id++)
            {         
                if(GL_NULL != g_a_tm_ctx_sp[timer_list_id])
                {
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_res_avl
                        .num_start_timer_req_fails = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
            g_p_tm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_tm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            
            g_p_tm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_tm_glb_api_err.num_delete_req_non_empty_list = GL_NULL;
            
            g_p_tm_glb_usage.num_stop_timer_timer_exp = GL_NULL;
            g_p_tm_glb_usage.num_start_timer_fails = GL_NULL;
            g_p_tm_glb_usage.num_start_timer_reqs = GL_NULL;
            
            for(timer_list_id = 0; timer_list_id < GL_TM_MAX_TIMER_LIST; 
                    timer_list_id++)
            {         
                if(GL_NULL != g_a_tm_ctx_sp[timer_list_id])
                {
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_timer_runing_peak_reached = GL_NULL;
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_start_timer_reqs = GL_NULL;
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_timer_runing = GL_NULL;
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.
                        num_total_timer_cfg = GL_NULL;
            
                    g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_res_avl
                        .num_start_timer_req_fails = GL_NULL;
                }
            }
            break;
            
        default:
            break;
            
    }
    
    GL_STAT_TM_MT_UNLOCK()
    
}



/*****************************************************************************
* Function Name    : gl_htm_stat_reinit_fn
* Arguments        : gl_stat_ids_et   stat_id
* Return value     : void
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
 void gl_htm_stat_reinit_fn
 
#ifdef _ANSI_C_
    ( gl_stat_ids_et          stat_id )
#else
    ( stat_id )
      gl_stat_ids_et          stat_id;
#endif
    
/****************************************************************************/
{
    gl_hash_table_t             hash_table_id;
    
    GL_STAT_HTM_MT_LOCK()

    switch(stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            g_p_htm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_htm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            g_p_htm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_htm_glb_api_err.num_delete_req_non_empty_table = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            g_p_htm_glb_usage.num_duplicate_key_at_insert = GL_NULL;
            g_p_htm_glb_usage.num_key_not_found = GL_NULL;
            g_p_htm_glb_usage.num_add_key_reqs = GL_NULL;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            for(hash_table_id = 0; hash_table_id < GL_HTM_MAX_HASH_TABLE;
                    hash_table_id++)
            {         
                if(GL_NULL != g_a_htm_ctx_sp[hash_table_id])
                {
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage
                        .num_keys_peak_reached = GL_NULL;
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage
                        .num_key_max_any_bucket = GL_NULL;
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage
                        .num_key_min_any_bucket = 100000;
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage
                        .num_total_key_cfg = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            for(hash_table_id = 0; hash_table_id < GL_HTM_MAX_HASH_TABLE; 
                    hash_table_id++)
            {         
                if(GL_NULL != g_a_htm_ctx_sp[hash_table_id])
                {   
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_res_avl.
                        num_add_key_req_fails = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
            g_p_htm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_htm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            
            g_p_htm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_htm_glb_api_err.num_delete_req_non_empty_table = GL_NULL;
            
            g_p_htm_glb_usage.num_duplicate_key_at_insert = GL_NULL;
            g_p_htm_glb_usage.num_key_not_found = GL_NULL;
            g_p_htm_glb_usage.num_add_key_reqs = GL_NULL;
            
            for(hash_table_id = 0; hash_table_id < GL_HTM_MAX_HASH_TABLE;
                    hash_table_id++)
            {         
                if(GL_NULL != g_a_htm_ctx_sp[hash_table_id])
                {   
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.
                        num_keys_peak_reached = GL_NULL;
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.
                        num_key_max_any_bucket = GL_NULL;
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.
                        num_key_min_any_bucket = 100000;
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.
                        num_total_key_cfg = GL_NULL;
            
                    g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_res_avl.
                        num_add_key_req_fails = GL_NULL;
                }
            }
            break;
            
        default:
            break;
            
    }
    
    GL_STAT_HTM_MT_UNLOCK()
    
}



/*****************************************************************************
* Function Name    : gl_llm_stat_reinit_fn
* Arguments        : gl_stat_ids_et   stat_id
* Return value     : void
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
 void gl_llm_stat_reinit_fn
 
#ifdef _ANSI_C_
    ( gl_stat_ids_et          stat_id )
#else
    ( stat_id )
      gl_stat_ids_et          stat_id;
#endif
    
/****************************************************************************/
{
    gl_list_t                   link_list_id;
    
    GL_STAT_LLM_MT_LOCK()

    switch(stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            g_p_llm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_llm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            g_p_llm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_llm_glb_api_err.num_delete_req_non_empty_list = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            g_p_llm_glb_usage.num_add_node_reqs = GL_NULL;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            for(link_list_id = 0; link_list_id < GL_LLM_MAX_LIST; 
                    link_list_id++)
            {         
                if(GL_NULL != g_a_llm_ctx_sp[link_list_id])
                {
                    g_a_llm_ctx_sp[link_list_id]-> g_p_llm_ctx_sp_usage.
                        num_node_peak_reached = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            for(link_list_id = 0; link_list_id < GL_LLM_MAX_LIST; 
                    link_list_id++)
            {         
                if(GL_NULL != g_a_llm_ctx_sp[link_list_id])
                {
                    g_a_llm_ctx_sp[link_list_id]-> g_p_llm_ctx_sp_res_avl.
                        num_add_node_req_fails = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
            g_p_llm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_llm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            
            g_p_llm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_llm_glb_api_err.num_delete_req_non_empty_list = GL_NULL;
            
            g_p_llm_glb_usage.num_add_node_reqs = GL_NULL;
            
            for(link_list_id = 0; link_list_id < GL_LLM_MAX_LIST; 
                    link_list_id++)
            {         
                if(GL_NULL != g_a_llm_ctx_sp[link_list_id])
                {
                    g_a_llm_ctx_sp[link_list_id]-> g_p_llm_ctx_sp_usage.
                        num_node_peak_reached = GL_NULL;
            
                    g_a_llm_ctx_sp[link_list_id]-> g_p_llm_ctx_sp_res_avl.
                        num_add_node_req_fails = GL_NULL;
                }
            }
            break;
            
        default:
            break;
            
    }
    
    GL_STAT_LLM_MT_UNLOCK()
    
}



/*****************************************************************************
* Function Name    : gl_mqm_stat_reinit_fn
* Arguments        : gl_stat_ids_et   stat_id
* Return value     : void
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
 void gl_mqm_stat_reinit_fn
 
#ifdef _ANSI_C_
    ( gl_stat_ids_et          stat_id )
#else
    ( stat_id )
      gl_stat_ids_et          stat_id;
#endif
    
/****************************************************************************/
{
    gl_mq_id_t                  mesg_q_id;
    
    GL_STAT_MQM_MT_LOCK()

    switch(stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            g_p_mqm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_mqm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            g_p_mqm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_mqm_glb_api_err.num_delete_req_non_empty_queue = GL_NULL;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            g_p_mqm_glb_usage.num_send_msg_reqs = GL_NULL;
            g_p_mqm_glb_usage.num_recv_msg_reqs = GL_NULL;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            for(mesg_q_id = 0; mesg_q_id < GL_MQM_MAX_MESG_Q; mesg_q_id++)
            {         
                if(GL_NULL != g_a_mqm_ctx_sp[mesg_q_id])
                {
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_msg_peak_reached = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_msg_max_any_priority_level = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_msg_min_any_priority_level = 100000;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_curr_msg_qty = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_total_msg_cfg = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            for(mesg_q_id = 0; mesg_q_id < GL_MQM_MAX_MESG_Q; mesg_q_id++)
            {         
                if(GL_NULL != g_a_mqm_ctx_sp[mesg_q_id])
                {
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_res_avl.
                        num_send_msg_req_fails = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_res_avl.
                        num_recv_msg_req_fails = GL_NULL;
                }
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
            g_p_mqm_glb_res_avl.num_malloc_fails = GL_NULL;
            g_p_mqm_glb_res_avl.num_mutex_opr_fails = GL_NULL;
            
            g_p_mqm_glb_api_err.num_in_param_validation_fails = GL_NULL;
            g_p_mqm_glb_api_err.num_delete_req_non_empty_queue = GL_NULL;
            
            g_p_mqm_glb_usage.num_send_msg_reqs = GL_NULL;
            g_p_mqm_glb_usage.num_recv_msg_reqs = GL_NULL;
            
            for(mesg_q_id = 0; mesg_q_id < GL_MQM_MAX_MESG_Q; mesg_q_id++)
            {         
                if(GL_NULL != g_a_mqm_ctx_sp[mesg_q_id])
                {
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_msg_peak_reached = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_msg_max_any_priority_level = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_msg_min_any_priority_level = 100000;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_curr_msg_qty = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_usage.
                        num_total_msg_cfg = GL_NULL;
            
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_res_avl.
                        num_send_msg_req_fails = GL_NULL;
                    g_a_mqm_ctx_sp[mesg_q_id]-> g_p_mqm_ctx_sp_res_avl.
                        num_recv_msg_req_fails = GL_NULL;
                }
            }
            break;

        default:
            break;
            
    }
    
    GL_STAT_MQM_MT_UNLOCK()
    
}



/*****************************************************************************
* Function Name    : gl_all_stat_reinit_fn
* Arguments        : gl_stat_ids_et   stat_id
* Return value     : void
* Functionality    : Reset the stats value to Zero.
* Functions called :
*****************************************************************************/
 void gl_all_stat_reinit_fn
 
#ifdef _ANSI_C_
    ( gl_stat_ids_et          stat_id )
#else
    ( stat_id )
      gl_stat_ids_et          stat_id;
#endif
    
/****************************************************************************/
{
    GL_STAT_MM_BFIT_MT_LOCK()
        gl_mm_stat_bfit_reinit_fn(stat_id);
    GL_STAT_MM_BFIT_MT_UNLOCK()

    GL_STAT_MM_USRSP_MT_LOCK()
        gl_mm_stat_usrsp_reinit_fn(stat_id);
    GL_STAT_MM_USRSP_MT_UNLOCK()
    
    GL_STAT_TM_MT_LOCK()
        gl_tm_stat_reinit_fn(stat_id);
    GL_STAT_TM_MT_UNLOCK()
    
    GL_STAT_HTM_MT_LOCK()
        gl_htm_stat_reinit_fn(stat_id);
    GL_STAT_HTM_MT_UNLOCK()
        
    GL_STAT_LLM_MT_LOCK()
        gl_llm_stat_reinit_fn(stat_id);
    GL_STAT_LLM_MT_UNLOCK()
        
    GL_STAT_MQM_MT_LOCK()
        gl_mqm_stat_reinit_fn(stat_id);
    GL_STAT_MQM_MT_UNLOCK()

}
        
    




/*****************************************************************************
* Function Name    : gl_mm_stat_bfit_get_fn
* Arguments        : gl_stat_scope_st   *p_stat_scope,
*                    gl_pvoid_t         p_stats_data 
* Return value     : void
* Functionality    : Get the stats value.
* Functions called :
*****************************************************************************/
 void gl_mm_stat_bfit_get_fn
 
#ifdef _ANSI_C_
    ( gl_stat_scope_st          *p_stat_scope,
      gl_pvoid_t                p_stats_data
    )
#else
    ( p_stat_scope, p_stats_data )
      gl_stat_scope_st          *p_stat_scope;
      gl_pvoid_t                p_stats_data;
#endif
    
/****************************************************************************/
{
    gl_stat_mm_bfit_glb_res_avl_st     *p_out_glb_res_avl;
    gl_stat_mm_bfit_glb_api_err_st *p_out_glb_api_err;
    gl_stat_mm_bfit_glb_usage_st       *p_out_glb_usage;
    gl_stat_mm_bfit_ctx_sp_usage_st     *p_out_ctx_sp_usage;
    gl_stat_mm_bfit_ctx_sp_res_avl_st   *p_out_ctx_sp_res_avl;
    
    GL_STAT_MM_BFIT_MT_LOCK()

    switch(p_stat_scope->stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            p_out_glb_res_avl = (gl_stat_mm_bfit_glb_res_avl_st *)
                p_stats_data;
            
            p_out_glb_res_avl->num_malloc_fails = 
                g_p_mm_bfit_glb_res_avl.num_malloc_fails;
            p_out_glb_res_avl->num_mutex_opr_fails = 
                g_p_mm_bfit_glb_res_avl.num_mutex_opr_fails;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            p_out_glb_api_err = (gl_stat_mm_bfit_glb_api_err_st *)
                p_stats_data;
            
            p_out_glb_api_err->num_inv_buf_ptr_in_rels = 
                g_p_mm_bfit_glb_api_err.num_inv_buf_ptr_in_rels;
            p_out_glb_api_err->num_buf_rels_req_free_buf = 
                g_p_mm_bfit_glb_api_err.num_buf_rels_req_free_buf;
            p_out_glb_api_err->num_in_param_validation_fails = 
                g_p_mm_bfit_glb_api_err.num_in_param_validation_fails;
            p_out_glb_api_err->num_delete_req_non_empty_pool = 
                g_p_mm_bfit_glb_api_err.num_delete_req_non_empty_pool;
            break;
            
            
        case GL_STAT_ID_GLB_USAGE:
            p_out_glb_usage = (gl_stat_mm_bfit_glb_usage_st *)
                p_stats_data;
            
            p_out_glb_usage->num_oversize_buf_aloc = 
                g_p_mm_bfit_glb_usage.num_oversize_buf_aloc;
            p_out_glb_usage->num_get_buf_fails = 
                g_p_mm_bfit_glb_usage.num_get_buf_fails;
            p_out_glb_usage->num_buf_corrupted = 
                g_p_mm_bfit_glb_usage.num_buf_corrupted;
            p_out_glb_usage->num_buf_corrected_af_corruption = 
                g_p_mm_bfit_glb_usage.num_buf_corrected_af_corruption;
            p_out_glb_usage->num_get_buf_reqs = 
                g_p_mm_bfit_glb_usage.num_get_buf_reqs;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            if(GL_NULL != g_a_mm_bfit_ctx_sp[p_stat_scope->stat_type_scope.
                    pool_id-1])
            {
                p_out_ctx_sp_usage = (gl_stat_mm_bfit_ctx_sp_usage_st *)
                    p_stats_data;
            
                p_out_ctx_sp_usage->num_busy_buf_peak_reached = 
                    g_a_mm_bfit_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                    ->g_p_mm_bfit_ctx_sp_usage.num_busy_buf_peak_reached;
                p_out_ctx_sp_usage->num_buf_corrupted = g_a_mm_bfit_ctx_sp
                    [p_stat_scope->stat_type_scope.pool_id-1]->
                    g_p_mm_bfit_ctx_sp_usage.num_buf_corrupted;
                p_out_ctx_sp_usage->num_buf_corrected_af_corruption = 
                    g_a_mm_bfit_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                    ->g_p_mm_bfit_ctx_sp_usage.num_buf_corrected_af_corruption;
                p_out_ctx_sp_usage->num_get_buf_reqs = g_a_mm_bfit_ctx_sp
                    [p_stat_scope->stat_type_scope.pool_id-1]->
                    g_p_mm_bfit_ctx_sp_usage.num_get_buf_reqs;
                p_out_ctx_sp_usage->num_busy_bufs =  g_a_mm_bfit_ctx_sp
                    [p_stat_scope->stat_type_scope.pool_id-1]->
                    g_p_mm_bfit_ctx_sp_usage.num_busy_bufs;
                p_out_ctx_sp_usage->num_bufs_total = g_a_mm_bfit_ctx_sp
                    [p_stat_scope->stat_type_scope.pool_id-1]->
                    g_p_mm_bfit_ctx_sp_usage.num_bufs_total;
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            if(GL_NULL != g_a_mm_bfit_ctx_sp[p_stat_scope->stat_type_scope.
                    pool_id-1])
            {
                p_out_ctx_sp_res_avl = (gl_stat_mm_bfit_ctx_sp_res_avl_st *)
                    p_stats_data;
            
                p_out_ctx_sp_res_avl->num_get_buf_fail_bufs_busy = 
                    g_a_mm_bfit_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                    ->g_p_mm_bfit_ctx_sp_res_avl.num_get_buf_fail_bufs_busy;
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
        default:
            break;
            
    }
    
    GL_STAT_MM_BFIT_MT_UNLOCK()
    
}


/*****************************************************************************
* Function Name    : gl_mm_stat_usrsp_get_fn
* Arguments        : gl_stat_scope_st   *p_stat_scope,
*                    gl_pvoid_t         p_stats_data 
* Return value     : void
* Functionality    : Get the stats value.
* Functions called :
*****************************************************************************/
 void gl_mm_stat_usrsp_get_fn
 
#ifdef _ANSI_C_
    ( gl_stat_scope_st          *p_stat_scope,
      gl_pvoid_t                p_stats_data
    )
#else
    ( p_stat_scope, p_stats_data )
      gl_stat_scope_st          *p_stat_scope;
      gl_pvoid_t                p_stats_data;
#endif
 
/****************************************************************************/
{
    gl_stat_mm_usrsp_glb_res_avl_st     *p_out_glb_res_avl;
    gl_stat_mm_usrsp_glb_api_err_st *p_out_glb_api_err;
    gl_stat_mm_usrsp_glb_usage_st       *p_out_glb_usage;
    gl_stat_mm_usrsp_ctx_sp_usage_st     *p_out_ctx_sp_usage;
    gl_stat_mm_usrsp_ctx_sp_res_avl_st   *p_out_ctx_sp_res_avl;
    
    GL_STAT_MM_USRSP_MT_LOCK()

    switch(p_stat_scope->stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            p_out_glb_res_avl = (gl_stat_mm_usrsp_glb_res_avl_st *)
                p_stats_data;
            
            p_out_glb_res_avl->num_malloc_fails = 
                g_p_mm_usrsp_glb_res_avl.num_malloc_fails;
            p_out_glb_res_avl->num_mutex_opr_fails = 
                g_p_mm_usrsp_glb_res_avl.num_mutex_opr_fails;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            p_out_glb_api_err = (gl_stat_mm_usrsp_glb_api_err_st *)
                p_stats_data;
            
            p_out_glb_api_err->num_inv_buf_ptr_in_rels = 
                g_p_mm_usrsp_glb_api_err.num_inv_buf_ptr_in_rels;
            p_out_glb_api_err->num_buf_rels_req_free_buf = 
                g_p_mm_usrsp_glb_api_err.num_buf_rels_req_free_buf;
            p_out_glb_api_err->num_in_param_validation_fails = 
                g_p_mm_usrsp_glb_api_err.num_in_param_validation_fails;
            p_out_glb_api_err->num_delete_req_non_empty_pool = 
                g_p_mm_usrsp_glb_api_err.num_delete_req_non_empty_pool;
            break;
            
            
        case GL_STAT_ID_GLB_USAGE:
            p_out_glb_usage = (gl_stat_mm_usrsp_glb_usage_st *)
                p_stats_data;

            p_out_glb_usage->num_get_buf_fails = 
                g_p_mm_usrsp_glb_usage.num_get_buf_fails;
            p_out_glb_usage->num_buf_corrupted = 
                g_p_mm_usrsp_glb_usage.num_buf_corrupted;
            p_out_glb_usage->num_buf_corrected_af_corruption = 
                g_p_mm_usrsp_glb_usage.num_buf_corrected_af_corruption;
            p_out_glb_usage->num_get_buf_reqs = 
                g_p_mm_usrsp_glb_usage.num_get_buf_reqs;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            if(GL_NULL != g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.
                    pool_id-1])
            {
                p_out_ctx_sp_usage = (gl_stat_mm_usrsp_ctx_sp_usage_st *)
                    p_stats_data;
            
                p_out_ctx_sp_usage->num_busy_buf_peak_reached = 
                   g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                   ->g_p_mm_usrsp_ctx_sp_usage.num_busy_buf_peak_reached;
                p_out_ctx_sp_usage->num_buf_corrupted = 
                   g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                   ->g_p_mm_usrsp_ctx_sp_usage.num_buf_corrupted;
                p_out_ctx_sp_usage->num_buf_corrected_af_corruption = 
                   g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                   ->g_p_mm_usrsp_ctx_sp_usage.num_buf_corrected_af_corruption;
                p_out_ctx_sp_usage->num_get_buf_reqs = 
                   g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                   ->g_p_mm_usrsp_ctx_sp_usage.num_get_buf_reqs;
                p_out_ctx_sp_usage->num_busy_bufs = 
                   g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                   ->g_p_mm_usrsp_ctx_sp_usage.num_busy_bufs;
                p_out_ctx_sp_usage->num_bufs_total = 
                   g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                   ->g_p_mm_usrsp_ctx_sp_usage.num_bufs_total;
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            if(GL_NULL != g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.
                    pool_id-1])
            {
                p_out_ctx_sp_res_avl = (gl_stat_mm_usrsp_ctx_sp_res_avl_st *)
                    p_stats_data;
            
                p_out_ctx_sp_res_avl->num_get_buf_fail_bufs_busy = 
                   g_a_mm_usrsp_ctx_sp[p_stat_scope->stat_type_scope.pool_id-1]
                   ->g_p_mm_usrsp_ctx_sp_res_avl.num_get_buf_fail_bufs_busy;
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
        default:
            break;
            
    }

    GL_STAT_MM_USRSP_MT_UNLOCK()
    
}




/*****************************************************************************
* Function Name    : gl_tm_stat_get_fn
* Arguments        : gl_stat_scope_st   *p_stat_scope,
*                    gl_pvoid_t         p_stats_data 
* Return value     : void
* Functionality    : Get the stats value.
* Functions called :
*****************************************************************************/
void gl_tm_stat_get_fn
 
#ifdef _ANSI_C_
    ( gl_stat_scope_st          *p_stat_scope,
      gl_pvoid_t                p_stats_data
    )
#else
    ( p_stat_scope, p_stats_data )
      gl_stat_scope_st          *p_stat_scope;
      gl_pvoid_t                p_stats_data;
#endif
    
/****************************************************************************/
{
    gl_stat_tm_glb_res_avl_st     *p_out_glb_res_avl;
    gl_stat_tm_glb_api_err_st *p_out_glb_api_err;
    gl_stat_tm_glb_usage_st       *p_out_glb_usage;
    gl_stat_tm_ctx_sp_usage_st     *p_out_ctx_sp_usage;
    gl_stat_tm_ctx_sp_res_avl_st   *p_out_ctx_sp_res_avl;
    
    GL_STAT_TM_MT_LOCK()

    switch(p_stat_scope->stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            p_out_glb_res_avl = (gl_stat_tm_glb_res_avl_st *)
                p_stats_data;

            p_out_glb_res_avl->num_malloc_fails = 
                g_p_tm_glb_res_avl.num_malloc_fails;
            p_out_glb_res_avl->num_mutex_opr_fails = 
                g_p_tm_glb_res_avl.num_mutex_opr_fails;

            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            p_out_glb_api_err = (gl_stat_tm_glb_api_err_st *)
                p_stats_data;

            p_out_glb_api_err->num_in_param_validation_fails = 
                g_p_tm_glb_api_err.num_in_param_validation_fails;
            p_out_glb_api_err->num_delete_req_non_empty_list = 
                g_p_tm_glb_api_err.num_delete_req_non_empty_list;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            p_out_glb_usage = (gl_stat_tm_glb_usage_st *)
                p_stats_data;

            p_out_glb_usage->num_stop_timer_timer_exp = 
                g_p_tm_glb_usage.num_stop_timer_timer_exp;
            p_out_glb_usage->num_start_timer_fails = 
                g_p_tm_glb_usage.num_start_timer_fails;
            p_out_glb_usage->num_start_timer_reqs = 
                g_p_tm_glb_usage.num_start_timer_reqs;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            if(GL_NULL != g_a_tm_ctx_sp[p_stat_scope->stat_type_scope.
                    timer_list_id])
            { 
                p_out_ctx_sp_usage = (gl_stat_tm_ctx_sp_usage_st *)
                    p_stats_data;

                p_out_ctx_sp_usage->num_timer_runing_peak_reached = 
                   g_a_tm_ctx_sp[p_stat_scope->stat_type_scope.timer_list_id]->
                   g_p_tm_ctx_sp_usage.num_timer_runing_peak_reached;
                p_out_ctx_sp_usage->num_start_timer_reqs = 
                   g_a_tm_ctx_sp[p_stat_scope->stat_type_scope.timer_list_id]->
                   g_p_tm_ctx_sp_usage.num_start_timer_reqs;
                p_out_ctx_sp_usage->num_timer_runing = 
                   g_a_tm_ctx_sp[p_stat_scope->stat_type_scope.timer_list_id]->
                   g_p_tm_ctx_sp_usage.num_timer_runing;
                p_out_ctx_sp_usage->num_total_timer_cfg = 
                   g_a_tm_ctx_sp[p_stat_scope->stat_type_scope.timer_list_id]->
                   g_p_tm_ctx_sp_usage.num_total_timer_cfg;
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            if(GL_NULL != g_a_tm_ctx_sp[p_stat_scope->stat_type_scope.
                    timer_list_id])
            { 
                p_out_ctx_sp_res_avl = (gl_stat_tm_ctx_sp_res_avl_st *)
                    p_stats_data;

                p_out_ctx_sp_res_avl->num_start_timer_req_fails = 
                   g_a_tm_ctx_sp[p_stat_scope->stat_type_scope.timer_list_id]->
                   g_p_tm_ctx_sp_res_avl.num_start_timer_req_fails;
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
        default:
            break;
            
    }
    
    GL_STAT_TM_MT_UNLOCK()
    
}


                               
/*****************************************************************************
* Function Name    : gl_htm_stat_get_fn
* Arguments        : gl_stat_scope_st   *p_stat_scope,
*                    gl_pvoid_t         p_stats_data 
* Return value     : void
* Functionality    : Get the stats value.
* Functions called :
*****************************************************************************/
void gl_htm_stat_get_fn
 
#ifdef _ANSI_C_
    ( gl_stat_scope_st          *p_stat_scope,
      gl_pvoid_t                p_stats_data
    )
#else
    ( p_stat_scope, p_stats_data )
      gl_stat_scope_st          *p_stat_scope;
      gl_pvoid_t                p_stats_data;
#endif
    
/****************************************************************************/
{
    gl_stat_htm_glb_res_avl_st     *p_out_glb_res_avl;
    gl_stat_htm_glb_api_err_st *p_out_glb_api_err;
    gl_stat_htm_glb_usage_st       *p_out_glb_usage;
    gl_stat_htm_ctx_sp_usage_st     *p_out_ctx_sp_usage;
    gl_stat_htm_ctx_sp_res_avl_st   *p_out_ctx_sp_res_avl;
    
    GL_STAT_HTM_MT_LOCK()

    switch(p_stat_scope->stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            p_out_glb_res_avl = (gl_stat_htm_glb_res_avl_st *)
                p_stats_data;

            p_out_glb_res_avl->num_malloc_fails = 
                g_p_htm_glb_res_avl.num_malloc_fails;
            p_out_glb_res_avl->num_mutex_opr_fails = 
                g_p_htm_glb_res_avl.num_mutex_opr_fails;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            p_out_glb_api_err = (gl_stat_htm_glb_api_err_st *)
                p_stats_data;

            p_out_glb_api_err->num_in_param_validation_fails = 
                g_p_htm_glb_api_err.num_in_param_validation_fails;
            p_out_glb_api_err->num_delete_req_non_empty_table = 
                g_p_htm_glb_api_err.num_delete_req_non_empty_table;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            p_out_glb_usage = (gl_stat_htm_glb_usage_st *)
                p_stats_data;

            p_out_glb_usage->num_duplicate_key_at_insert = 
                g_p_htm_glb_usage.num_duplicate_key_at_insert;
            p_out_glb_usage->num_key_not_found = 
                g_p_htm_glb_usage.num_key_not_found;
            p_out_glb_usage->num_add_key_reqs = 
                g_p_htm_glb_usage.num_add_key_reqs;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            if(GL_NULL != g_a_htm_ctx_sp[p_stat_scope->stat_type_scope.
                    hash_table_id])
            {
                p_out_ctx_sp_usage = (gl_stat_htm_ctx_sp_usage_st*)
                    p_stats_data;

                p_out_ctx_sp_usage->num_keys_peak_reached = 
                  g_a_htm_ctx_sp[p_stat_scope->stat_type_scope.hash_table_id]->
                  g_p_htm_ctx_sp_usage.num_keys_peak_reached;
                p_out_ctx_sp_usage->num_key_max_any_bucket = 
                  g_a_htm_ctx_sp[p_stat_scope->stat_type_scope.hash_table_id]->
                  g_p_htm_ctx_sp_usage.num_key_max_any_bucket;
                p_out_ctx_sp_usage->num_key_min_any_bucket = 
                  g_a_htm_ctx_sp[p_stat_scope->stat_type_scope.hash_table_id]->
                  g_p_htm_ctx_sp_usage.num_key_min_any_bucket;
                p_out_ctx_sp_usage->num_total_key_cfg = 
                  g_a_htm_ctx_sp[p_stat_scope->stat_type_scope.hash_table_id]->
                  g_p_htm_ctx_sp_usage.num_total_key_cfg;
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            if(GL_NULL != g_a_htm_ctx_sp[p_stat_scope->stat_type_scope.
                    hash_table_id])
            {
                p_out_ctx_sp_res_avl = (gl_stat_htm_ctx_sp_res_avl_st *)
                    p_stats_data;

                p_out_ctx_sp_res_avl->num_add_key_req_fails = 
                  g_a_htm_ctx_sp[p_stat_scope->stat_type_scope.hash_table_id]->
                  g_p_htm_ctx_sp_res_avl.num_add_key_req_fails;
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
        default:
            break;
            
    }
    
    GL_STAT_HTM_MT_UNLOCK()
    
}



/*****************************************************************************
* Function Name    : gl_llm_stat_get_fn
* Arguments        : gl_stat_scope_st   *p_stat_scope,
*                    gl_pvoid_t         p_stats_data 
* Return value     : void
* Functionality    : Get the stats value.
* Functions called :
*****************************************************************************/
void gl_llm_stat_get_fn
 
#ifdef _ANSI_C_
    ( gl_stat_scope_st          *p_stat_scope,
      gl_pvoid_t                p_stats_data
    )
#else
    ( p_stat_scope, p_stats_data )
      gl_stat_scope_st          *p_stat_scope;
      gl_pvoid_t                p_stats_data;
#endif
    
/****************************************************************************/
{
    gl_stat_llm_glb_res_avl_st     *p_out_glb_res_avl;
    gl_stat_llm_glb_api_err_st *p_out_glb_api_err;
    gl_stat_llm_glb_usage_st       *p_out_glb_usage;
    gl_stat_llm_ctx_sp_usage_st     *p_out_ctx_sp_usage;
    gl_stat_llm_ctx_sp_res_avl_st   *p_out_ctx_sp_res_avl;
    
    GL_STAT_LLM_MT_LOCK()

    switch(p_stat_scope->stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            p_out_glb_res_avl = (gl_stat_llm_glb_res_avl_st *)
                p_stats_data;

            p_out_glb_res_avl->num_malloc_fails = 
                g_p_llm_glb_res_avl.num_malloc_fails;
            p_out_glb_res_avl->num_mutex_opr_fails =
                g_p_llm_glb_res_avl.num_mutex_opr_fails;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            p_out_glb_api_err = (gl_stat_llm_glb_api_err_st *)
                p_stats_data;

            p_out_glb_api_err->num_in_param_validation_fails = 
                g_p_llm_glb_api_err.num_in_param_validation_fails;
            p_out_glb_api_err->num_delete_req_non_empty_list = 
                g_p_llm_glb_api_err.num_delete_req_non_empty_list;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            p_out_glb_usage = (gl_stat_llm_glb_usage_st *)
                p_stats_data;

            p_out_glb_usage->num_add_node_reqs = 
                g_p_llm_glb_usage.num_add_node_reqs;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            if(GL_NULL != g_a_llm_ctx_sp[p_stat_scope->stat_type_scope.
                    link_list_id])
            {
                p_out_ctx_sp_usage = (gl_stat_llm_ctx_sp_usage_st *)
                    p_stats_data;

                p_out_ctx_sp_usage->num_node_peak_reached = 
                   g_a_llm_ctx_sp[p_stat_scope->stat_type_scope.link_list_id]->
                   g_p_llm_ctx_sp_usage.num_node_peak_reached;
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            if(GL_NULL != g_a_llm_ctx_sp[p_stat_scope->stat_type_scope.
                    link_list_id])
            {
                p_out_ctx_sp_res_avl = (gl_stat_llm_ctx_sp_res_avl_st *)
                    p_stats_data;

                p_out_ctx_sp_res_avl->num_add_node_req_fails =  
                   g_a_llm_ctx_sp[p_stat_scope->stat_type_scope.link_list_id]->
                   g_p_llm_ctx_sp_res_avl.num_add_node_req_fails;
            }
            break;
            
        case GL_STAT_IDS_ALL_STATS:
        default:
            break;
    }
    
    GL_STAT_LLM_MT_UNLOCK()
    
}


                                
/*****************************************************************************
* Function Name    : gl_mqm_stat_get_fn
* Arguments        : gl_stat_scope_st   *p_stat_scope,
*                    gl_pvoid_t         p_stats_data 
* Return value     : void
* Functionality    : Get the stats value.
* Functions called :
*****************************************************************************/
void gl_mqm_stat_get_fn
 
#ifdef _ANSI_C_
    ( gl_stat_scope_st          *p_stat_scope,
      gl_pvoid_t                p_stats_data
    )
#else
    ( p_stat_scope, p_stats_data )
      gl_stat_scope_st          *p_stat_scope;
      gl_pvoid_t                p_stats_data;
#endif
    
/****************************************************************************/
{
    gl_stat_mqm_glb_res_avl_st     *p_out_glb_res_avl;
    gl_stat_mqm_glb_api_err_st *p_out_glb_api_err;
    gl_stat_mqm_glb_usage_st       *p_out_glb_usage;
    gl_stat_mqm_ctx_sp_usage_st     *p_out_ctx_sp_usage;
    gl_stat_mqm_ctx_sp_res_avl_st   *p_out_ctx_sp_res_avl;
    
    GL_STAT_MQM_MT_LOCK()

    switch(p_stat_scope->stat_id)
    {
        case GL_STAT_ID_GLB_RSRC_AVAL_ERRS:
            p_out_glb_res_avl = (gl_stat_mqm_glb_res_avl_st *)
                p_stats_data;

            p_out_glb_res_avl->num_malloc_fails = 
                g_p_mqm_glb_res_avl.num_malloc_fails;
            p_out_glb_res_avl->num_mutex_opr_fails = 
                g_p_mqm_glb_res_avl.num_mutex_opr_fails;
            break;
            
        case GL_STAT_ID_GLB_API_PROC_ERRS:
            p_out_glb_api_err = (gl_stat_mqm_glb_api_err_st *)
                p_stats_data;

            p_out_glb_api_err->num_in_param_validation_fails = 
                g_p_mqm_glb_api_err.num_in_param_validation_fails;
            p_out_glb_api_err->num_delete_req_non_empty_queue = 
                g_p_mqm_glb_api_err.num_delete_req_non_empty_queue;
            break;
            
        case GL_STAT_ID_GLB_USAGE:
            p_out_glb_usage = (gl_stat_mqm_glb_usage_st *)
                p_stats_data;

            p_out_glb_usage->num_send_msg_reqs = 
                g_p_mqm_glb_usage.num_send_msg_reqs;
            p_out_glb_usage->num_recv_msg_reqs = 
                g_p_mqm_glb_usage.num_recv_msg_reqs;
            break;
            
        case GL_STAT_ID_CTX_SP_USAGE:
            if(GL_NULL != g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.
                    mesg_q_id])
            {
                p_out_ctx_sp_usage = (gl_stat_mqm_ctx_sp_usage_st *)
                    p_stats_data;

                p_out_ctx_sp_usage->num_msg_peak_reached = 
                    g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.mesg_q_id]->
                    g_p_mqm_ctx_sp_usage.num_msg_peak_reached;
                p_out_ctx_sp_usage->num_msg_max_any_priority_level = 
                    g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.mesg_q_id]->
                    g_p_mqm_ctx_sp_usage.num_msg_max_any_priority_level;
                p_out_ctx_sp_usage->num_msg_min_any_priority_level = 
                    g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.mesg_q_id]->
                    g_p_mqm_ctx_sp_usage.num_msg_min_any_priority_level;
                p_out_ctx_sp_usage->num_curr_msg_qty = 
                    g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.mesg_q_id]->
                    g_p_mqm_ctx_sp_usage.num_curr_msg_qty;
                p_out_ctx_sp_usage->num_total_msg_cfg = 
                    g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.mesg_q_id]->
                    g_p_mqm_ctx_sp_usage.num_total_msg_cfg;
            }
            break;
            
        case GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR:
            if(GL_NULL != g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.
                    mesg_q_id])
            {
                p_out_ctx_sp_res_avl = (gl_stat_mqm_ctx_sp_res_avl_st *)
                    p_stats_data;

                p_out_ctx_sp_res_avl->num_send_msg_req_fails = 
                    g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.mesg_q_id]->
                    g_p_mqm_ctx_sp_res_avl.num_send_msg_req_fails;
                p_out_ctx_sp_res_avl->num_recv_msg_req_fails = 
                    g_a_mqm_ctx_sp[p_stat_scope->stat_type_scope.mesg_q_id]->
                    g_p_mqm_ctx_sp_res_avl.num_recv_msg_req_fails;
            }
            break;
            
        default:
            break;
    }
    
    GL_STAT_MQM_MT_UNLOCK()
    
}



#endif /* Endif for GL_STATISTICS_ENABLED Flag */
