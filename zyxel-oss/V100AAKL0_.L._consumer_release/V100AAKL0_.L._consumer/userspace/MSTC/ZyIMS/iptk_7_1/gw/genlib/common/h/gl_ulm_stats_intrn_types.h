/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library -  Common Utilities
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_ulm_stats_intrn_types.h
*
*    DESCRIPTION : This file contains the interface level prototype and 
*                  data structure declarations of string manager sub-module
*                  of the generic library implementation.
*                  This file is to be include in the application module, in
*                  order to use the supported APIs.
*
*   DATE            NAME        REFERENCE           REASON
*-----------   -------------  ---------------  ----------------------
*
* 29/11/2001 Ravi Jindal        SPR-2788        Packaging of Genlib-Code.
* 
* 06/12/2001 Prashant Jain      SPR-3363        Compilation with CC.
*
* 11/07/2002 Ravi Jindal        SPR-3959        Enhancements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/

#ifndef _GL_ULM_STATS_INTRN_TYPES_H_
#define _GL_ULM_STATS_INTRN_TYPES_H_

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
 * Internal Used Structres.
 */ 
typedef struct gl_stat_mm_bfit_ctx_sp_s
{
    gl_stat_mm_bfit_ctx_sp_usage_st     g_p_mm_bfit_ctx_sp_usage;
    gl_stat_mm_bfit_ctx_sp_res_avl_st   g_p_mm_bfit_ctx_sp_res_avl;
}gl_stat_mm_bfit_ctx_sp_st;

typedef struct gl_stat_mm_usrsp_ctx_sp_s
{
    gl_stat_mm_usrsp_ctx_sp_usage_st    g_p_mm_usrsp_ctx_sp_usage;
    gl_stat_mm_usrsp_ctx_sp_res_avl_st  g_p_mm_usrsp_ctx_sp_res_avl;
}gl_stat_mm_usrsp_ctx_sp_st;

typedef struct gl_stat_tm_ctx_sp_s
{
    gl_stat_tm_ctx_sp_usage_st          g_p_tm_ctx_sp_usage;
    gl_stat_tm_ctx_sp_res_avl_st        g_p_tm_ctx_sp_res_avl;
}gl_stat_tm_ctx_sp_st;

typedef struct gl_stat_htm_ctx_sp_s
{
    gl_stat_htm_ctx_sp_usage_st         g_p_htm_ctx_sp_usage;
    gl_stat_htm_ctx_sp_res_avl_st       g_p_htm_ctx_sp_res_avl;
}gl_stat_htm_ctx_sp_st;

typedef struct gl_stat_llm_ctx_sp_s
{
    gl_stat_llm_ctx_sp_usage_st         g_p_llm_ctx_sp_usage;
    gl_stat_llm_ctx_sp_res_avl_st       g_p_llm_ctx_sp_res_avl;
}gl_stat_llm_ctx_sp_st;

typedef struct gl_stat_mqm_ctx_sp_s
{
    gl_stat_mqm_ctx_sp_usage_st         g_p_mqm_ctx_sp_usage;
    gl_stat_mqm_ctx_sp_res_avl_st       g_p_mqm_ctx_sp_res_avl;
}gl_stat_mqm_ctx_sp_st;



/**********************************************************
 * Stats Data Declearation for Memory Manager.
 **********************************************************/ 

#ifdef GL_MT_ENABLED
extern gw_boolean               g_mm_stat_init;
extern gw_boolean               g_tm_stat_init;
extern gw_boolean               g_htm_stat_init;
extern gw_boolean               g_llm_stat_init;
extern gw_boolean               g_mqm_stat_init;

extern gl_error_t               g_gl_stat_error_code;

extern GL_MUTEX_ID              g_mm_bfit_stat_sem;
extern GL_MUTEX_ID              g_mm_usrsp_stat_sem;
extern GL_MUTEX_ID              g_tm_stat_sem;
extern GL_MUTEX_ID              g_htm_stat_sem;
extern GL_MUTEX_ID              g_llm_stat_sem;
extern GL_MUTEX_ID              g_mqm_stat_sem;
#endif


extern gl_stat_mm_bfit_glb_res_avl_st      g_p_mm_bfit_glb_res_avl;
extern gl_stat_mm_bfit_glb_api_err_st      g_p_mm_bfit_glb_api_err;
extern gl_stat_mm_bfit_glb_usage_st        g_p_mm_bfit_glb_usage;

extern gl_stat_mm_bfit_ctx_sp_st            *g_a_mm_bfit_ctx_sp
    [GL_MM_MAX_COMMON_POOL_QTY];


extern gl_stat_mm_usrsp_glb_res_avl_st     g_p_mm_usrsp_glb_res_avl;
extern gl_stat_mm_usrsp_glb_api_err_st     g_p_mm_usrsp_glb_api_err;
extern gl_stat_mm_usrsp_glb_usage_st       g_p_mm_usrsp_glb_usage;

extern gl_stat_mm_usrsp_ctx_sp_st           *g_a_mm_usrsp_ctx_sp
    [GL_MM_MAX_TASK_POOL_QTY];


/**********************************************************
 * Stats Data Declearation for Timer Manager.
 **********************************************************/ 

extern gl_stat_tm_glb_res_avl_st      g_p_tm_glb_res_avl;
extern gl_stat_tm_glb_api_err_st      g_p_tm_glb_api_err;
extern gl_stat_tm_glb_usage_st        g_p_tm_glb_usage;

extern gl_stat_tm_ctx_sp_st            *g_a_tm_ctx_sp[GL_TM_MAX_TIMER_LIST];


/**********************************************************
 * Stats Data Declearation for Hash Table Manager.
 **********************************************************/ 

extern gl_stat_htm_glb_res_avl_st      g_p_htm_glb_res_avl;
extern gl_stat_htm_glb_api_err_st      g_p_htm_glb_api_err;
extern gl_stat_htm_glb_usage_st        g_p_htm_glb_usage;

extern gl_stat_htm_ctx_sp_st            *g_a_htm_ctx_sp[GL_HTM_MAX_HASH_TABLE];



/**********************************************************
 * Stats Data Declearation for Link List Manager.
 **********************************************************/ 

extern gl_stat_llm_glb_res_avl_st      g_p_llm_glb_res_avl;
extern gl_stat_llm_glb_api_err_st      g_p_llm_glb_api_err;
extern gl_stat_llm_glb_usage_st        g_p_llm_glb_usage;

extern gl_stat_llm_ctx_sp_st            *g_a_llm_ctx_sp[GL_LLM_MAX_LIST];



/**********************************************************
 * Stats Data Declearation for Message Queue Manager.
 **********************************************************/ 

extern gl_stat_mqm_glb_res_avl_st      g_p_mqm_glb_res_avl;
extern gl_stat_mqm_glb_api_err_st      g_p_mqm_glb_api_err;
extern gl_stat_mqm_glb_usage_st        g_p_mqm_glb_usage;

extern gl_stat_mqm_ctx_sp_st            *g_a_mqm_ctx_sp[GL_MQM_MAX_MESG_Q];



/***************************************
 * Local function Decleration.
 ***************************************/ 

typedef void    (*gl_stat_reinit_func)
                    ( gl_stat_ids_et stat_id );
                    
typedef void    (*gl_stat_get_func)
                    ( gl_stat_scope_st *p_stat_scope,
                      gl_pvoid_t       p_stats_data );

extern gw_boolean               g_stats_state_table[GL_STAT_TYPE_ALL]
    [GL_STAT_IDS_ALL_STATS];

extern gl_return_t gl_mm_stat_allocte_bfit_memory
            _ARGS_( ( gl_pool_id_t   pool_id,
                      gl_error_t     *p_ecode ) );
                    
extern gl_return_t gl_mm_stat_allocte_usrsp_memory
            _ARGS_( ( gl_pool_id_t   pool_id,
                      gl_error_t     *p_ecode ) );
                    
extern gl_return_t gl_tm_stat_allocte_memory

            _ARGS_( ( gl_timer_list_t timer_list_id,
                      gl_error_t      *p_ecode ) );
                    
extern gl_return_t gl_htm_stat_allocte_memory

            _ARGS_( ( gl_hash_table_t  hash_table_id,
                      gl_error_t       *p_ecode ) );
                    
extern gl_return_t gl_llm_stat_allocte_memory

            _ARGS_( ( gl_list_t      link_list_id,
                      gl_error_t     *p_ecode ) );
                    
extern gl_return_t gl_mqm_stat_allocte_memory

            _ARGS_( ( gl_mq_id_t     mesg_q_id,
                      gl_error_t     *p_ecode ) );




extern gl_return_t gl_mm_stat_free_bfit_memory
            _ARGS_( ( gl_pool_id_t   pool_id,
                      gl_error_t     *p_ecode ) );
                    
extern gl_return_t gl_mm_stat_free_usrsp_memory
            _ARGS_( ( gl_pool_id_t   pool_id,
                      gl_error_t     *p_ecode ) );
                    
extern gl_return_t gl_tm_stat_free_memory

            _ARGS_( ( gl_timer_list_t timer_list_id,
                      gl_error_t      *p_ecode ) );
                    
extern gl_return_t gl_htm_stat_free_memory

            _ARGS_( ( gl_hash_table_t  hash_table_id,
                      gl_error_t       *p_ecode ) );
                    
extern gl_return_t gl_llm_stat_free_memory

            _ARGS_( ( gl_list_t      link_list_id,
                      gl_error_t     *p_ecode ) );
                    
extern gl_return_t gl_mqm_stat_free_memory

            _ARGS_( ( gl_mq_id_t     mesg_q_id,
                      gl_error_t     *p_ecode ) );


                    
                    
extern void gl_mm_stat_bfit_reinit_fn
            _ARGS_( ( gl_stat_ids_et stat_id ) );
extern void gl_mm_stat_usrsp_reinit_fn
            _ARGS_( ( gl_stat_ids_et stat_id ) );
extern void gl_tm_stat_reinit_fn
            _ARGS_( ( gl_stat_ids_et stat_id ) );
extern void gl_htm_stat_reinit_fn
            _ARGS_( ( gl_stat_ids_et stat_id ) );
extern void gl_llm_stat_reinit_fn
            _ARGS_( ( gl_stat_ids_et stat_id ) );
extern void gl_mqm_stat_reinit_fn
            _ARGS_( ( gl_stat_ids_et stat_id ) );
extern void gl_all_stat_reinit_fn
            _ARGS_( ( gl_stat_ids_et stat_id ) );



extern void gl_mm_stat_bfit_get_fn
            _ARGS_( ( gl_stat_scope_st *p_stat_scope,
                      gl_pvoid_t       p_stats_data   ) );

extern void gl_mm_stat_usrsp_get_fn
            _ARGS_( ( gl_stat_scope_st *p_stat_scope,
                      gl_pvoid_t       p_stats_data   ) );

extern void gl_tm_stat_get_fn
            _ARGS_( ( gl_stat_scope_st *p_stat_scope,
                      gl_pvoid_t       p_stats_data   ) );
                               
extern void gl_htm_stat_get_fn
            _ARGS_( ( gl_stat_scope_st *p_stat_scope,
                      gl_pvoid_t       p_stats_data   ) );

extern void gl_llm_stat_get_fn
            _ARGS_( ( gl_stat_scope_st *p_stat_scope,
                      gl_pvoid_t       p_stats_data   ) );
                                
extern void gl_mqm_stat_get_fn
            _ARGS_( ( gl_stat_scope_st *p_stat_scope,
                      gl_pvoid_t       p_stats_data   ) );




#ifdef GL_STATS_ENABLED


#ifdef GL_MT_ENABLED
/*
 * MAcros for LOCK Mutex.
 */ 
#define GL_STAT_MM_BFIT_MT_LOCK()   {\
    if(GL_TRUE == g_mm_stat_init)\
    {\
        gl_lock_mutex(&g_mm_bfit_stat_sem, GL_NULL, &g_gl_stat_error_code );\
    }\
                                    }
                                    
#define GL_STAT_MM_USRSP_MT_LOCK()  {\
    if(GL_TRUE == g_mm_stat_init)\
    {\
        gl_lock_mutex(&g_mm_usrsp_stat_sem, GL_NULL, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_TM_MT_LOCK()        {\
    if(GL_TRUE == g_tm_stat_init)\
    {\
        gl_lock_mutex(&g_tm_stat_sem, GL_NULL, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_HTM_MT_LOCK()       {\
    if(GL_TRUE == g_htm_stat_init)\
    {\
        gl_lock_mutex(&g_htm_stat_sem, GL_NULL, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_LLM_MT_LOCK()       {\
    if(GL_TRUE == g_llm_stat_init)\
    {\
        gl_lock_mutex(&g_llm_stat_sem, GL_NULL, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_MQM_MT_LOCK()       {\
    if(GL_TRUE == g_mqm_stat_init)\
    {\
        gl_lock_mutex(&g_mqm_stat_sem, GL_NULL, &g_gl_stat_error_code);\
    }\
                                    }

/*
 * Macro for UNLOCK Mutex.
 */
#define GL_STAT_MM_BFIT_MT_UNLOCK()  {\
    if(GL_TRUE == g_mm_stat_init)\
    {\
        gl_unlock_mutex(GL_NULL, &g_mm_bfit_stat_sem, &g_gl_stat_error_code );\
    }\
                                    }
                                    
#define GL_STAT_MM_USRSP_MT_UNLOCK() {\
    if(GL_TRUE == g_mm_stat_init)\
    {\
        gl_unlock_mutex(GL_NULL, &g_mm_usrsp_stat_sem, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_TM_MT_UNLOCK()      {\
    if(GL_TRUE == g_tm_stat_init)\
    {\
        gl_unlock_mutex(GL_NULL, &g_tm_stat_sem, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_HTM_MT_UNLOCK()     {\
    if(GL_TRUE == g_htm_stat_init)\
    {\
        gl_unlock_mutex(GL_NULL, &g_htm_stat_sem, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_LLM_MT_UNLOCK()     {\
    if(GL_TRUE == g_llm_stat_init)\
    {\
        gl_unlock_mutex(GL_NULL, &g_llm_stat_sem, &g_gl_stat_error_code);\
    }\
                                    }
                                    
#define GL_STAT_MQM_MT_UNLOCK()     {\
    if(GL_TRUE == g_mqm_stat_init)\
    {\
        gl_unlock_mutex(GL_NULL, &g_mqm_stat_sem, &g_gl_stat_error_code);\
    }\
                                    }


#else


#define GL_STAT_MM_BFIT_MT_LOCK()
#define GL_STAT_MM_USRSP_MT_LOCK()
#define GL_STAT_TM_MT_LOCK()
#define GL_STAT_HTM_MT_LOCK()
#define GL_STAT_LLM_MT_LOCK()
#define GL_STAT_MQM_MT_LOCK()


#define GL_STAT_MM_BFIT_MT_UNLOCK()
#define GL_STAT_MM_USRSP_MT_UNLOCK()
#define GL_STAT_TM_MT_UNLOCK()
#define GL_STAT_HTM_MT_UNLOCK()
#define GL_STAT_LLM_MT_UNLOCK()
#define GL_STAT_MQM_MT_UNLOCK()


#endif /* Endif for GL_MT_ENABLED */


                                    
/*
 * Memory Manager Best-Fit Allocation Schm Stats Increment-Decrement Macros. 
 */ 
#define GL_INC_MM_B_G_MALOC_FAIL()    {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_mm_bfit_glb_res_avl.num_malloc_fails += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_MUTX_FAIL()     {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_mm_bfit_glb_res_avl.num_mutex_opr_fails += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
                                    
#define GL_INC_MM_B_G_REL_INV_PTR()   {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_bfit_glb_api_err.num_inv_buf_ptr_in_rels += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_REL_FREE_BUF()  {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_bfit_glb_api_err.num_buf_rels_req_free_buf += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_PARM_VLD_FAIL() {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_bfit_glb_api_err.num_in_param_validation_fails += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_DEL_RQ_POOL_BSY()   {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_bfit_glb_api_err.num_delete_req_non_empty_pool += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_MM_B_G_OVERSIZE_BUF_ALOC() {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_bfit_glb_usage.num_oversize_buf_aloc += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_MEMGET_FAIL()   {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_bfit_glb_usage.num_get_buf_fails += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_BUF_FND_CRRPTD()    {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_bfit_glb_usage.num_buf_corrupted += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_CRRPTD_BUF_CRCTD()  {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_bfit_glb_usage.num_buf_corrected_af_corruption += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_G_GET_BUF_RQ()    {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_bfit_glb_usage.num_get_buf_reqs += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_MM_B_C_PEAK_BSY_BUFS(pool_id, busy_buf_qty) {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.\
                        num_busy_buf_peak_reached < busy_buf_qty)\
        {\
            g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.\
                        num_busy_buf_peak_reached = busy_buf_qty;\
        }\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_C_BUF_FND_CRRPTD(pool_id)    {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.\
                        num_buf_corrupted += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_C_CRRPTD_BUF_CRCTD(pool_id)  {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.\
                        num_buf_corrected_af_corruption += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_B_C_GET_BUF_RQ(pool_id)    {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.\
                        num_get_buf_reqs += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_MM_B_C_BSY_BUFS(pool_id, busy_buf_qty)      {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.\
                        num_busy_bufs = busy_buf_qty;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
#define GL_MM_B_C_BUF_QTY(pool_id, buf_qty)      {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_usage.\
                        num_bufs_total = buf_qty;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_MM_B_C_MEMGET_FAIL(pool_id)   {\
    GL_STAT_MM_BFIT_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_BFIT_SCHM]\
            [GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR])\
    {\
        g_a_mm_bfit_ctx_sp[pool_id]-> g_p_mm_bfit_ctx_sp_res_avl.\
                        num_get_buf_fail_bufs_busy += 1;\
    }\
    GL_STAT_MM_BFIT_MT_UNLOCK()\
                                             }

                                    
/*
 * Memory Manager User Specific Allocation Scheme - Stats Macros. 
 */ 
#define GL_INC_MM_U_G_MALOC_FAIL()    {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_mm_usrsp_glb_res_avl.num_malloc_fails += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_G_MUTX_FAIL()     {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_mm_usrsp_glb_res_avl.num_mutex_opr_fails += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
                                    
#define GL_INC_MM_U_G_REL_INV_PTR()   {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_usrsp_glb_api_err.num_inv_buf_ptr_in_rels += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_G_REL_FREE_BUF()  {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_usrsp_glb_api_err.num_buf_rels_req_free_buf += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_G_PARM_VLD_FAIL() {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_usrsp_glb_api_err.num_in_param_validation_fails += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_G_DEL_RQ_POOL_BSY()   {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mm_usrsp_glb_api_err.num_delete_req_non_empty_pool += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_MM_U_G_MEMGET_FAIL()   {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_usrsp_glb_usage.num_get_buf_fails += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_G_BUF_FND_CRRPTD()    {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_usrsp_glb_usage.num_buf_corrupted += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_G_CRRPTD_BUF_CRCTD()  {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_usrsp_glb_usage.num_buf_corrected_af_corruption += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_G_GET_BUF_RQ()    {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mm_usrsp_glb_usage.num_get_buf_reqs += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_MM_U_C_PEAK_BSY_BUFS(pool_id, busy_buf_qty) {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.\
                        num_busy_buf_peak_reached < busy_buf_qty)\
        {\
            g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.\
                        num_busy_buf_peak_reached = busy_buf_qty;\
        }\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_C_BUF_FND_CRRPTD(pool_id)    {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.\
                        num_buf_corrupted += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_C_CRRPTD_BUF_CRCTD(pool_id)  {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.\
                        num_buf_corrected_af_corruption += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MM_U_C_GET_BUF_RQ(pool_id)    {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.\
                        num_get_buf_reqs += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_MM_U_C_BSY_BUFS(pool_id, busy_buf_qty)      {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.\
                        num_busy_bufs = busy_buf_qty;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
#define GL_MM_U_C_BUF_QTY(pool_id, buf_qty)      {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_usage.\
                        num_bufs_total = buf_qty;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_MM_U_C_MEMGET_FAIL(pool_id)   {\
    GL_STAT_MM_USRSP_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\
            [GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR])\
    {\
        g_a_mm_usrsp_ctx_sp[pool_id]-> g_p_mm_usrsp_ctx_sp_res_avl.\
                        num_get_buf_fail_bufs_busy += 1;\
    }\
    GL_STAT_MM_USRSP_MT_UNLOCK()\
                                             }



/*
 * Timer Manager - Stats Macros. 
 */ 
#define GL_INC_TM_G_MALOC_FAIL()    {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_tm_glb_res_avl.num_malloc_fails += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_TM_G_MUTX_FAIL()     {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_tm_glb_res_avl.num_mutex_opr_fails += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_TM_G_PARM_VLD_FAIL() {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_tm_glb_api_err.num_in_param_validation_fails += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_TM_G_DEL_RQ_LIST_BSY()   {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_tm_glb_api_err.num_delete_req_non_empty_list += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
                                    
#define GL_INC_TM_G_STOP_RQ_TIMER_ALDY_EXP()    {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_tm_glb_usage.num_stop_timer_timer_exp += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_TM_G_START_TIMER_FAIL()  {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_tm_glb_usage.num_start_timer_fails += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_TM_G_START_TIMER_RQ()    {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_tm_glb_usage.num_start_timer_reqs += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_TM_C_PEAK_TIMER_RUN(timer_list_id, timer_qty_running) {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.\
                        num_timer_runing_peak_reached < timer_qty_running)\
        {\
            g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.\
                        num_timer_runing_peak_reached = timer_qty_running;\
        }\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_TM_C_START_TIMER_RQ(timer_list_id)    {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.\
                        num_start_timer_reqs += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
#define GL_TM_C_TIMER_RUNING(timer_list_id, busy_timer_qty)      {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.\
                       num_timer_runing = busy_timer_qty;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
#define GL_TM_C_TIMER_QTY(timer_list_id, total_timer_qty)      {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_usage.\
                        num_total_timer_cfg = total_timer_qty;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_TM_C_START_TIMER_FAIL(timer_list_id)   {\
    GL_STAT_TM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_TM]\
            [GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR])\
    {\
        g_a_tm_ctx_sp[timer_list_id]-> g_p_tm_ctx_sp_res_avl.\
                       num_start_timer_req_fails  += 1;\
    }\
    GL_STAT_TM_MT_UNLOCK()\
                                             }




/*
 * Hash-Table Manager - Stats Macros. 
 */ 
#define GL_INC_HTM_G_MALOC_FAIL()    {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_htm_glb_res_avl.num_malloc_fails += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_HTM_G_MUTX_FAIL()     {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_htm_glb_res_avl.num_mutex_opr_fails += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_HTM_G_PARM_VLD_FAIL() {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_htm_glb_api_err.num_in_param_validation_fails += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_HTM_G_DEL_RQ_TABLE_BSY()   {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_htm_glb_api_err.num_delete_req_non_empty_table += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
                                    
#define GL_INC_HTM_G_DUP_KEY_INSRT()    {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_htm_glb_usage.num_duplicate_key_at_insert += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_HTM_G_KEY_NOT_FOUND()  {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_htm_glb_usage.num_key_not_found += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_HTM_G_ADD_KEY_RQ()    {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_htm_glb_usage.num_add_key_reqs += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_HTM_C_PEAK_KEY_INSRT(hash_table_id, ht_key_qty) {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.\
                        num_keys_peak_reached < ht_key_qty)\
        {\
            g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.\
                        num_keys_peak_reached = ht_key_qty;\
        }\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
#define GL_HTM_C_MAX_KEY_AY_BKT(hash_table_id, bkt_key_qty)    {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.\
                       num_key_max_any_bucket < bkt_key_qty)\
        {\
            g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.\
                       num_key_max_any_bucket = bkt_key_qty;\
        }\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
#define GL_HTM_C_MIN_KEY_AY_BKT(hash_table_id, bkt_key_qty)      {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.\
                       num_key_min_any_bucket > bkt_key_qty)\
        {\
            g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.\
                      num_key_min_any_bucket  = bkt_key_qty;\
        }\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_HTM_C_TOTAL_KEY_QTY(hash_table_id, total_key_cfg)      {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_usage.\
                        num_total_key_cfg = total_key_cfg;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_HTM_C_ADD_KEY_FAIL(hash_table_id)   {\
    GL_STAT_HTM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_HTM]\
            [GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR])\
    {\
        g_a_htm_ctx_sp[hash_table_id]-> g_p_htm_ctx_sp_res_avl.\
                       num_add_key_req_fails  += 1;\
    }\
    GL_STAT_HTM_MT_UNLOCK()\
                                             }



/*
 * Link-List Manager - Stats Macros. 
 */ 
#define GL_INC_LLM_G_MALOC_FAIL()    {\
    GL_STAT_LLM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_LLM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_llm_glb_res_avl.num_malloc_fails += 1;\
    }\
    GL_STAT_LLM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_LLM_G_MUTX_FAIL()     {\
    GL_STAT_LLM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_LLM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_llm_glb_res_avl.num_mutex_opr_fails += 1;\
    }\
    GL_STAT_LLM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_LLM_G_PARM_VLD_FAIL() {\
    GL_STAT_LLM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_LLM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_llm_glb_api_err.num_in_param_validation_fails += 1;\
    }\
    GL_STAT_LLM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_LLM_G_DEL_RQ_LIST_BSY()   {\
    GL_STAT_LLM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_LLM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_llm_glb_api_err.num_delete_req_non_empty_list += 1;\
    }\
    GL_STAT_LLM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
                                    
#define GL_INC_LLM_G_ADD_NODE_RQ()    {\
    GL_STAT_LLM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_LLM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_llm_glb_usage.num_add_node_reqs += 1;\
    }\
    GL_STAT_LLM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_LLM_C_PEAK_NODE_ADDED(link_list_id, node_qty) {\
    GL_STAT_LLM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_LLM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_llm_ctx_sp[link_list_id]-> g_p_llm_ctx_sp_usage.\
                        num_node_peak_reached < node_qty)\
        {\
            g_a_llm_ctx_sp[link_list_id]-> g_p_llm_ctx_sp_usage.\
                        num_node_peak_reached = node_qty;\
        }\
    }\
    GL_STAT_LLM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_LLM_C_ADD_NODE_FAIL(link_list_id)   {\
    GL_STAT_LLM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_LLM]\
            [GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR])\
    {\
        g_a_llm_ctx_sp[link_list_id]-> g_p_llm_ctx_sp_res_avl.\
                       num_add_node_req_fails  += 1;\
    }\
    GL_STAT_LLM_MT_UNLOCK()\
                                             }


/*
 * Mesaage-Queue Manager - Stats Macros. 
 */ 
#define GL_INC_MQM_G_MALOC_FAIL()    {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_mqm_glb_res_avl.num_malloc_fails += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MQM_G_MUTX_FAIL()     {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_GLB_RSRC_AVAL_ERRS])\
    {\
        g_p_mqm_glb_res_avl.num_mutex_opr_fails += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
#define GL_INC_MQM_G_PARM_VLD_FAIL() {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mqm_glb_api_err.num_in_param_validation_fails += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MQM_G_DEL_RQ_QUEUE_BSY()   {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_GLB_API_PROC_ERRS])\
    {\
        g_p_mqm_glb_api_err.num_delete_req_non_empty_queue += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
                                    
#define GL_INC_MQM_G_SEND_MSG_RQ()    {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mqm_glb_usage.num_send_msg_reqs += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
#define GL_INC_MQM_G_RECV_MSG_RQ()  {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_GLB_USAGE])\
    {\
        g_p_mqm_glb_usage.num_recv_msg_reqs += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
                                    
                                    
                                    
#define GL_MQM_C_PEAK_MSG_REACH(meag_q_id, msg_qty) {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                        num_msg_peak_reached < msg_qty)\
        {\
            g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                        num_msg_peak_reached = msg_qty;\
        }\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
#define GL_MQM_C_MAX_MSG_AY_PR_LVL(meag_q_id, prt_msg_qty)    {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                       num_msg_max_any_priority_level < prt_msg_qty)\
        {\
            g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                       num_msg_max_any_priority_level = prt_msg_qty;\
        }\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
#define GL_MQM_C_MIN_MSG_AY_PR_LVL(meag_q_id, prt_msg_qty)      {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        if(g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                        num_msg_min_any_priority_level > prt_msg_qty)\
        {\
            g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                        num_msg_min_any_priority_level = prt_msg_qty;\
        }\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
#define GL_MQM_C_CURR_MSG_QTY(meag_q_id, curr_msg_qty)      {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                        num_curr_msg_qty = curr_msg_qty;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    
#define GL_MQM_C_TOTAL_MSG_QTY(meag_q_id, total_msg_cfg)      {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_CTX_SP_USAGE])\
    {\
        g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_usage.\
                        num_total_msg_cfg = total_msg_cfg;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                    }
                                    

                                    
#define GL_INC_MQM_C_SEND_MSG_FAIL(meag_q_id)   {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR])\
    {\
        g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_res_avl.\
                        num_send_msg_req_fails += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                             }

#define GL_INC_MQM_C_RECV_MSG_FAIL(meag_q_id)   {\
    GL_STAT_MQM_MT_LOCK()\
    if(g_stats_state_table[GL_STAT_TYPE_MQM]\
            [GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR])\
    {\
        g_a_mqm_ctx_sp[meag_q_id]-> g_p_mqm_ctx_sp_res_avl.\
                        num_send_msg_req_fails += 1;\
    }\
    GL_STAT_MQM_MT_UNLOCK()\
                                             }







#else /* Else for GL_STATS_ENABLED */


#define GL_INC_MM_B_G_MALOC_FAIL()
#define GL_INC_MM_B_G_MUTX_FAIL()       /* Not Supported for lock,unlock*/

#define GL_INC_MM_B_G_REL_INV_PTR()
#define GL_INC_MM_B_G_REL_FREE_BUF()
#define GL_INC_MM_B_G_PARM_VLD_FAIL()
#define GL_INC_MM_B_G_DEL_RQ_POOL_BSY()

#define GL_INC_MM_B_G_OVERSIZE_BUF_ALOC()
#define GL_INC_MM_B_G_MEMGET_FAIL()
#define GL_INC_MM_B_G_BUF_FND_CRRPTD()
#define GL_INC_MM_B_G_CRRPTD_BUF_CRCTD()  /*Not Supported */
#define GL_INC_MM_B_G_GET_BUF_RQ()

#define GL_MM_B_C_PEAK_BSY_BUFS(pool_id, peak_busy_buf)
#define GL_INC_MM_B_C_BUF_FND_CRRPTD(pool_id)
#define GL_INC_MM_B_C_CRRPTD_BUF_CRCTD(pool_id)
#define GL_INC_MM_B_C_GET_BUF_RQ(pool_id)
#define GL_MM_B_C_BSY_BUFS(pool_id, busy_buf_qty)
#define GL_MM_B_C_BUF_QTY(pool_id, buf_qty)

#define GL_INC_MM_B_C_MEMGET_FAIL(pool_id)  



/*
 * Memory Manager User Specific Allocation Scheme - Stats Macros. 
 */
#define GL_INC_MM_U_G_MALOC_FAIL()
#define GL_INC_MM_U_G_MUTX_FAIL()       /* Not Supported for lock,unlock*/

#define GL_INC_MM_U_G_REL_INV_PTR()
#define GL_INC_MM_U_G_REL_FREE_BUF()
#define GL_INC_MM_U_G_PARM_VLD_FAIL()
#define GL_INC_MM_U_G_DEL_RQ_POOL_BSY()

#define GL_INC_MM_U_G_MEMGET_FAIL()
#define GL_INC_MM_U_G_BUF_FND_CRRPTD()
#define GL_INC_MM_U_G_CRRPTD_BUF_CRCTD()  /*Not Supported */
#define GL_INC_MM_U_G_GET_BUF_RQ()

#define GL_MM_U_C_PEAK_BSY_BUFS(pool_id, peak_busy_buf)
#define GL_INC_MM_U_C_BUF_FND_CRRPTD(pool_id)
#define GL_INC_MM_U_C_CRRPTD_BUF_CRCTD(pool_id)
#define GL_INC_MM_U_C_GET_BUF_RQ(pool_id)
#define GL_MM_U_C_BSY_BUFS(pool_id, busy_buf_qty)
#define GL_MM_U_C_BUF_QTY(pool_id, buf_qty)

#define GL_INC_MM_U_C_MEMGET_FAIL(pool_id)  




/*
 * Timer Manager - Stats Macros. 
 */ 
#define GL_INC_TM_G_MALOC_FAIL()
#define GL_INC_TM_G_MUTX_FAIL()

#define GL_INC_TM_G_PARM_VLD_FAIL()
#define GL_INC_TM_G_DEL_RQ_LIST_BSY()

#define GL_INC_TM_G_STOP_RQ_TIMER_ALDY_EXP()
#define GL_INC_TM_G_START_TIMER_FAIL()
#define GL_INC_TM_G_START_TIMER_RQ()

#define GL_TM_C_PEAK_TIMER_RUN(timer_list_id, peak_timer_qty)
#define GL_INC_TM_C_START_TIMER_RQ(timer_list_id)
#define GL_TM_C_TIMER_RUNING(timer_list_id, busy_timer_qty)
#define GL_TM_C_TIMER_QTY(timer_list_id, total_timer_qty)

#define GL_INC_TM_C_START_TIMER_FAIL(timer_list_id)


/*
 * Hash-Table Manager - Stats Macros. 
 */ 
#define GL_INC_HTM_G_MALOC_FAIL()
#define GL_INC_HTM_G_MUTX_FAIL()

#define GL_INC_HTM_G_PARM_VLD_FAIL()
#define GL_INC_HTM_G_DEL_RQ_TABLE_BSY()

#define GL_INC_HTM_G_DUP_KEY_INSRT()
#define GL_INC_HTM_G_KEY_NOT_FOUND()
#define GL_INC_HTM_G_ADD_KEY_RQ()

#define GL_HTM_C_PEAK_KEY_INSRT(hash_table_id, ht_key_qty)
#define GL_HTM_C_MAX_KEY_AY_BKT(hash_table_id, bkt_key_qty)
#define GL_HTM_C_MIN_KEY_AY_BKT(hash_table_id, bkt_key_qty)
#define GL_INC_HTM_C_TOTAL_KEY_QTY(hash_table_id, num_total_key_cfg)

#define GL_INC_HTM_C_ADD_KEY_FAIL(hash_table_id)



/*
 * Link-List Manager - Stats Macros. 
 */ 
#define GL_INC_LLM_G_MALOC_FAIL()
#define GL_INC_LLM_G_MUTX_FAIL()

#define GL_INC_LLM_G_PARM_VLD_FAIL()
#define GL_INC_LLM_G_DEL_RQ_LIST_BSY()

#define GL_INC_LLM_G_ADD_NODE_RQ()

#define GL_LLM_C_PEAK_NODE_ADDED(link_list_id, node_qty)

#define GL_INC_LLM_C_ADD_NODE_FAIL(link_list_id)



/*
 * Mesaage-Queue Manager - Stats Macros. 
 */ 
#define GL_INC_MQM_G_MALOC_FAIL()
#define GL_INC_MQM_G_MUTX_FAIL()

#define GL_INC_MQM_G_PARM_VLD_FAIL()
#define GL_INC_MQM_G_DEL_RQ_QUEUE_BSY()

#define GL_INC_MQM_G_SEND_MSG_RQ()
#define GL_INC_MQM_G_RECV_MSG_RQ()

#define GL_MQM_C_PEAK_MSG_REACH(meag_q_id, msg_qty)
#define GL_MQM_C_MAX_MSG_AY_PR_LVL(meag_q_id, prt_msg_qty) 
#define GL_MQM_C_MIN_MSG_AY_PR_LVL(meag_q_id, prt_msg_qty)
#define GL_MQM_C_CURR_MSG_QTY(meag_q_id, curr_msg_qty)
#define GL_MQM_C_TOTAL_MSG_QTY(meag_q_id, num_total_msg_cfg)

#define GL_INC_MQM_C_SEND_MSG_FAIL(meag_q_id)
#define GL_INC_MQM_C_RECV_MSG_FAIL(meag_q_id)



#endif /* Endif for GL_STATS_ENABLED */


/*
    if(g_stats_state_table[GL_STAT_TYPE_MM_USR_SP_SCHM]\[])\`
    {\
        \
    }\
                                    }
                                    */
                                    
                                    
#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
