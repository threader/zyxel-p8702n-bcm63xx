/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library -  Common Utilities
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_ulm_interfaces.h
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

#ifndef _GL_ULM_INTERFACES_H_
#define _GL_ULM_INTERFACES_H_

#include "gl_config.h"
#include "gl_types.h"
#include "gl_error.h"
#include "gl_os_port.h"
#include "gl_gscim_interfaces.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*
 * Timestamp related structure.
 */
typedef enum
{
    GL_TIME_STAMP_UTC_STR=1,
    /*
     * Timestamp in nano seconds
     */
    GL_TIME_STAMP_IN_NS

}gl_timestamp_type_et;



/*----------------------------------------------------------------------------
 * EXTERNAL INTERFACE APIs - String Manipulation
 *---------------------------------------------------------------------------*/

extern gl_return_t    
gl_stm_strcpy
            
    _ARGS_( (   gw_U8bit                *p_dest_buffer,
                gw_U8bit                *p_source_buffer,
                gl_error_t              *p_ecode     ) );
            
                 
extern gl_return_t    
gl_stm_strncpy
            
    _ARGS_( (   gw_U8bit                *p_dest_buffer,
                gw_U8bit                *p_source_buffer,
                gw_U32bit               num_of_characters,
                gl_error_t              *p_ecode     ) );
            
    
extern gl_return_t    
gl_stm_strcat
            
    _ARGS_( (   gw_U8bit                *p_dest_buffer, 
                gw_U8bit                *p_source_buffer,
                gl_error_t              *p_ecode     ) );
            
                 
extern gl_return_t    
gl_stm_strncat
            
    _ARGS_( (   gw_U8bit                *p_dest_buffer, 
                gw_U8bit                *p_source_buffer,
                gw_U32bit               num_of_characters,
                gl_error_t              *p_ecode     ) );
            
  
extern gw_S32bit    
gl_stm_strlen
            
    _ARGS_( (   gw_U8bit                *p_buffer,
                gl_error_t              *p_ecode     ) );
            
                 

extern gw_S8bit
gl_stm_strcmp
            
    _ARGS_( (   gw_U8bit                *p_string1, 
                gw_U8bit                *p_string2,
                gl_error_t              *p_ecode     ) );
            
                 

extern gw_S8bit    
gl_stm_strncmp
            
    _ARGS_( (   gw_U8bit                *p_string1,
                gw_U8bit                *p_string2, 
                gw_U32bit               compare_len,
                gl_error_t              *p_ecode     ) );
            
                   

extern gl_return_t 
gl_stm_atoi
            
    _ARGS_( (   gw_U8bit                *p_str,
                gw_U32bit               *p_num,
                gl_error_t              *p_ecode     ) );
            
            

extern gl_return_t 
gl_stm_itoa
            
    _ARGS_( (   gw_U32bit               num,
                gw_U8bit                str[],
                gl_error_t              *p_ecode     ) );
            
                   

extern gw_S8bit 
gl_stm_strcmpi
            
    _ARGS_( (   gw_U8bit                *p_string1,
                gw_U8bit                *p_string2,
                gl_error_t              *p_ecode     ) );
            
                  

extern gw_S8bit 
gl_stm_strncmpi
            
    _ARGS_( (   gw_U8bit                *p_string1,
                gw_U8bit                *p_string2,
                gw_U32bit               compare_len,
                gl_error_t              *p_ecode     ) );



extern gw_U8bit * 
gl_stm_strchr
            
    _ARGS_( (   gw_U8bit                *p_string,
                gw_U8bit                character,
                gl_error_t              *p_ecode     ) );



extern gw_U8bit * 
gl_stm_strrchr
            
    _ARGS_( (   gw_U8bit                *p_string,
                gw_U8bit                character,
                gl_error_t              *p_ecode     ) );



extern gw_U8bit * 
gl_stm_strpbrk
            
    _ARGS_( (   gw_U8bit                *p_string,
                gw_U8bit                *p_search_string,
                gl_error_t              *p_ecode     ) );



extern gw_U8bit * 
gl_stm_strstr
            
    _ARGS_( (   gw_U8bit                *p_string,
                gw_U8bit                *p_search_string,
                gl_error_t              *p_ecode     ) );


/* 
 * Memory copy.
 */
extern void    
gl_mm_memcpy     
        
    _ARGS_( (   gw_U8bit                *p_dest_buffer, 
                gw_U8bit                *p_source_buffer, 
                gw_U32bit               memory_size     ) );
 
       

/*
 * Set the memory.
 */
extern void    
gl_mm_memset     
        
    _ARGS_( (   gw_U8bit                *p_dest_buffer, 
                gw_U8bit                rep_char, 
                gw_U32bit               memory_size     ) );



/*****************************************************
* Time-Stamp Related functions Prototypes.
******************************************************/

extern gl_return_t 
gl_tm_get_timeval
            
    _ARGS_( (   gl_time_stamp_t         *p_timeval      ) );
      
            

extern void
gl_tm_get_abs_time
            
    _ARGS_( (   gw_U8bit                time_flag,
                gl_time_stamp_ut        *p_time_stamp   ) );
       
             

extern void
gl_tm_convert_sec_to_date
            
    _ARGS_( (   gl_time_stamp_ut        *p_time_stamp, 
                gl_time_notation_t      *p_date_time    ) );
        
             

extern gw_S8bit
gl_tm_comp_time
            
    _ARGS_( (   gl_time_stamp_t         *p_time1,
                gl_time_stamp_t         *p_time2       ) );
       
             
extern void
gl_tm_get_relative_time_diff

    _ARGS_( (   gl_time_stamp_t         *p_new_time,
                gl_time_stamp_t         *p_old_time,
                gl_time_stamp_t         *p_diff_time     ) );


extern void 
gl_tm_diff_time
        
    _ARGS_( (   gl_time_stamp_t         *p_time1,
                gl_time_stamp_t         *p_time2,
                gl_time_stamp_t         *p_diff_time     ) );
        
              

extern void 
gl_tm_add_time
        
    _ARGS_( (   gl_time_stamp_t         *p_time1,
                gl_time_stamp_t         *p_time2,
                gl_time_stamp_t         *p_total_time     ) );
   
    

    
/***************************************************
 * Data-Structures and APIs for Statistics.
 ***************************************************/
   

typedef enum
{
    GL_STAT_TYPE_MM_BFIT_SCHM       = 1,
    GL_STAT_TYPE_MM_USR_SP_SCHM,
    GL_STAT_TYPE_TM,
    GL_STAT_TYPE_HTM,
    GL_STAT_TYPE_LLM,
    GL_STAT_TYPE_MQM,
    GL_STAT_TYPE_ALL

} gl_stat_type_et;
 

typedef enum
{
    GL_STAT_ID_GLB_RSRC_AVAL_ERRS	    = 1,
    GL_STAT_ID_GLB_API_PROC_ERRS	    = 2,
    GL_STAT_ID_GLB_USAGE		    = 3,

    GL_STAT_ID_CTX_SP_USAGE		    = 4,
    GL_STAT_ID_CTX_SP_RSRC_AVAL_ERR	    = 5,
    GL_STAT_IDS_ALL_STATS 		    = 6  /* Used only during INIT API */
    
} gl_stat_ids_et;


typedef struct
{
    gl_stat_ids_et		stat_id;

    union
    {
    	gl_pool_id_t	pool_id;	/* For Statistics Type MM */
    	gl_hash_table_t	hash_table_id; /* Instance of Hash Table for HTM */
        gl_timer_list_t timer_list_id; /* For Timer List */
        gl_list_t       link_list_id; /* For Link List */
        gl_mq_id_t      mesg_q_id; /* For Message Queue */
    		
    }stat_type_scope;

} gl_stat_scope_st;




/*********************************************
 * Stats Structures for the Memory Manager.
 ********************************************/ 

typedef struct gl_stat_mm_bfit_glb_res_avl_s
{
    gw_U32bit                   num_malloc_fails;
    gw_U32bit                   num_mutex_opr_fails;
    
}gl_stat_mm_bfit_glb_res_avl_st;


typedef struct gl_stat_mm_bfit_glb_api_err_s
{
    gw_U32bit                   num_inv_buf_ptr_in_rels;
    gw_U32bit                   num_buf_rels_req_free_buf;
    gw_U32bit                   num_in_param_validation_fails;
    gw_U32bit                   num_delete_req_non_empty_pool;
    
}gl_stat_mm_bfit_glb_api_err_st;


typedef struct gl_stat_mm_bfit_glb_usage_s
{
    gw_U32bit                   num_oversize_buf_aloc;
    gw_U32bit                   num_get_buf_fails;
    gw_U32bit                   num_buf_corrupted;
    gw_U32bit                   num_buf_corrected_af_corruption;
    gw_U32bit                   num_get_buf_reqs;
    
}gl_stat_mm_bfit_glb_usage_st;


typedef struct gl_stat_mm_bfit_ctx_sp_usage_s
{
    gw_U32bit                   num_busy_buf_peak_reached;
    gw_U32bit                   num_buf_corrupted;
    gw_U32bit                   num_buf_corrected_af_corruption;
    gw_U32bit                   num_get_buf_reqs;
    gw_U32bit                   num_busy_bufs;
    gw_U32bit                   num_bufs_total;
    
}gl_stat_mm_bfit_ctx_sp_usage_st;


typedef struct gl_stat_mm_bfit_ctx_sp_res_avl_s
{
    gw_U32bit                   num_get_buf_fail_bufs_busy;
    
}gl_stat_mm_bfit_ctx_sp_res_avl_st;


typedef gl_stat_mm_bfit_glb_res_avl_st gl_stat_mm_usrsp_glb_res_avl_st; 
typedef gl_stat_mm_bfit_glb_api_err_st gl_stat_mm_usrsp_glb_api_err_st;

typedef struct gl_stat_mm_usrsp_glb_usage_s
{
    gw_U32bit                   num_get_buf_fails;
    gw_U32bit                   num_buf_corrupted;
    gw_U32bit                   num_buf_corrected_af_corruption;
    gw_U32bit                   num_get_buf_reqs;
}gl_stat_mm_usrsp_glb_usage_st;

typedef gl_stat_mm_bfit_ctx_sp_usage_st gl_stat_mm_usrsp_ctx_sp_usage_st;
typedef gl_stat_mm_bfit_ctx_sp_res_avl_st gl_stat_mm_usrsp_ctx_sp_res_avl_st;


/*********************************************
 * Stats Structures for the Timer Manager.
 ********************************************/ 

typedef struct gl_stat_tm_glb_res_avl_s
{
    gw_U32bit                   num_malloc_fails;
    gw_U32bit                   num_mutex_opr_fails;
    
}gl_stat_tm_glb_res_avl_st;

typedef struct gl_stat_tm_glb_api_err_s
{
    gw_U32bit                   num_in_param_validation_fails;
    gw_U32bit                   num_delete_req_non_empty_list;
    
}gl_stat_tm_glb_api_err_st;


typedef struct gl_stat_tm_glb_usage_s
{
    gw_U32bit                   num_stop_timer_timer_exp;
    gw_U32bit                   num_start_timer_fails;
    gw_U32bit                   num_start_timer_reqs;
    
}gl_stat_tm_glb_usage_st;


typedef struct gl_stat_tm_ctx_sp_usage_s
{
    gw_U32bit                   num_timer_runing_peak_reached;
    gw_U32bit                   num_start_timer_reqs;
    gw_U32bit                   num_timer_runing;
    gw_U32bit                   num_total_timer_cfg;
    
}gl_stat_tm_ctx_sp_usage_st;


typedef struct gl_stat_tm_ctx_sp_res_avl_s
{
    gw_U32bit                   num_start_timer_req_fails;
    
}gl_stat_tm_ctx_sp_res_avl_st;


    

/*********************************************
 * Stats Structures for the Hash Table Manager.
 ********************************************/ 

typedef struct gl_stat_htm_glb_res_avl_s
{
    gw_U32bit                   num_malloc_fails;
    gw_U32bit                   num_mutex_opr_fails;
    
}gl_stat_htm_glb_res_avl_st;

typedef struct gl_stat_htm_glb_api_err_s
{
    gw_U32bit                   num_in_param_validation_fails;
    gw_U32bit                   num_delete_req_non_empty_table;
    
}gl_stat_htm_glb_api_err_st;


typedef struct gl_stat_htm_glb_usage_s
{
    gw_U32bit                   num_duplicate_key_at_insert;
    gw_U32bit                   num_key_not_found;
    gw_U32bit                   num_add_key_reqs;
    
}gl_stat_htm_glb_usage_st;


typedef struct gl_stat_htm_ctx_sp_usage_s
{
    gw_U32bit                   num_keys_peak_reached;
    gw_U32bit                   num_key_max_any_bucket;
    gw_U32bit                   num_key_min_any_bucket;
    gw_U32bit                   num_total_key_cfg;
    
}gl_stat_htm_ctx_sp_usage_st;


typedef struct gl_stat_htm_ctx_sp_res_avl_s
{
    gw_U32bit                   num_add_key_req_fails;
    
}gl_stat_htm_ctx_sp_res_avl_st;



/*********************************************
 * Stats Structures for the Link List Manager.
 ********************************************/ 

typedef struct gl_stat_llm_glb_res_avl_s
{
    gw_U32bit                   num_malloc_fails;
    gw_U32bit                   num_mutex_opr_fails;
    
}gl_stat_llm_glb_res_avl_st;

typedef struct gl_stat_llm_glb_api_err_s
{
    gw_U32bit                   num_in_param_validation_fails;
    gw_U32bit                   num_delete_req_non_empty_list;
    
}gl_stat_llm_glb_api_err_st;


typedef struct gl_stat_llm_glb_usage_s
{
    gw_U32bit                   num_add_node_reqs;
    
}gl_stat_llm_glb_usage_st;


typedef struct gl_stat_llm_ctx_sp_usage_s
{
    gw_U32bit                   num_node_peak_reached;
    
}gl_stat_llm_ctx_sp_usage_st;


typedef struct gl_stat_llm_ctx_sp_res_avl_s
{
    gw_U32bit                   num_add_node_req_fails;
    
}gl_stat_llm_ctx_sp_res_avl_st;



/************************************************
 * Stats Structures for the Message Queue Manager.
 ***********************************************/ 

typedef struct gl_stat_mqm_glb_res_avl_s
{
    gw_U32bit                   num_malloc_fails;
    gw_U32bit                   num_mutex_opr_fails;
    
}gl_stat_mqm_glb_res_avl_st;

typedef struct gl_stat_mqm_glb_api_err_s
{
    gw_U32bit                   num_in_param_validation_fails;
    gw_U32bit                   num_delete_req_non_empty_queue;
    
}gl_stat_mqm_glb_api_err_st;


typedef struct gl_stat_mqm_glb_usage_s
{
    gw_U32bit                   num_send_msg_reqs;
    gw_U32bit                   num_recv_msg_reqs;
    
}gl_stat_mqm_glb_usage_st;


typedef struct gl_stat_mqm_ctx_sp_usage_s
{
    gw_U32bit                   num_msg_peak_reached;
    gw_U32bit                   num_msg_max_any_priority_level;
    gw_U32bit                   num_msg_min_any_priority_level;
    gw_U32bit                   num_curr_msg_qty;
    gw_U32bit                   num_total_msg_cfg;
    
}gl_stat_mqm_ctx_sp_usage_st;


typedef struct gl_stat_mqm_ctx_sp_res_avl_s
{
    gw_U32bit                   num_send_msg_req_fails;
    gw_U32bit                   num_recv_msg_req_fails;
    
}gl_stat_mqm_ctx_sp_res_avl_st;


/*************************************************
* APIs Prototypes.
**************************************************/

extern gl_return_t    
gl_reinit_stats

    _ARGS_( (   gl_stat_type_et           stat_type,
                gl_stat_ids_et            stat_id, 
                gl_error_t                *p_ecode          ) );


extern gl_return_t    
gl_get_stats

    _ARGS_( (   gw_boolean                reset_flag,
                gl_stat_type_et           stat_type,
                gl_stat_scope_st          *p_stat_scope,
                gl_pvoid_t                p_stats_data,
                gl_error_t                *p_ecode          ) );


extern gl_return_t    
gl_set_stats_activity

    _ARGS_( (   gw_boolean                reset_flag,
                gl_stat_type_et           stat_type,
                gl_stat_ids_et            stat_id,
                gw_boolean                enable_stats_flag,
                gl_error_t                *p_ecode          ) );
    


#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
