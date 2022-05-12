/*****************************************************************************
*
* FUNCTION    : Generic Libraries error codes.
*
******************************************************************************
*
* FILENAME    : gl_error.h
*
* DESCRIPTION : Contains error codes for both OS interface and DB interface.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- --------------------------------
*
* 19/1/2001  Sachin Mittal                    Initial
*
* 18/08/2001 Prashant Jain   SPR 2276        Compiling with CC on Solaris.
*
* 24/10/2001 Ravi Jindal     SPR-2792        Adding return failure for 
*                                            semaphore fail operation in 
*                                            all functions.
* 29/10/2001 Ravi Jindal     SPR-2798        Adding error code for gl_pot.c 
* 30/10/2001 Ravi Jindal     SPR-2744        Adding de-init functions required 
*                                            failures.
* 17/11/2001 Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                            cosmmetics of file.
* 22/11/2001 Ravi Jindal     SPR-3225        Add Congestion Management
*                                            modules related structures.
*
* 31/12/2001 Prashant Jain   SPR-2744        When de-initialization is invoked,
*                                            INIT flag should be checked to
*                                            take care of the out-of-sequence
*                                            calling of DE-INIT APIs. So added
*                                            two more error code for the same.
*                                            
* 10/03/2002 Ravi Jindal     SPR-3656        Enhancements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/

/*
 * Check to see if this file has been included already.
 */

#ifndef _GL_ERROR_H_
#define _GL_ERROR_H_

#include "gl_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Default vaLUE OF GL_ERR_BASE = GL_NULL (in gl_config.h).
 */
    
/*
 * Error types for the generic libraries - list manager.
 */
typedef enum gl_lm_error_e
{
    GL_LLM_ERR_NO_RESOURCE = GL_ERR_BASE,           /* GL_ERR_BASE + 0 */
    GL_LLM_ERR_NOT_EMPTY,                           /* GL_ERR_BASE + 1 */
    GL_LLM_ERR_INV_LINK_LIST,                       /* GL_ERR_BASE + 2 */
    GL_LLM_ERR_NO_ELEMENT,                          /* GL_ERR_BASE + 3 */
    GL_LLM_ERR_LIST_FULL,                           /* GL_ERR_BASE + 4 */
    GL_LLM_ERR_INV_PTR,                             /* GL_ERR_BASE + 5 */
    GL_LLM_ERR_INV_TASK_ID,                         /* GL_ERR_BASE + 6 */
    GL_LLM_ERR_LAST_ELEM,                           /* GL_ERR_BASE + 7 */
    GL_LLM_ERR_FIRST_ELEM,                          /* GL_ERR_BASE + 8 */

    GL_LLM_ERR_INV_ELEM_FREE_FUNC_PTR,              /* GL_ERR_BASE + 9 */
    GL_LLM_ERR_NULL_NUM_OF_ELEM_PTR,                /* GL_ERR_BASE + 10 */
    
    GL_WLM_ERR_NO_RESOURCE,                         /* GL_ERR_BASE + 11 */
    GL_WLM_ERR_LIST_FULL,                           /* GL_ERR_BASE + 12 */
    GL_WLM_ERR_INV_WORK_LIST,                       /* GL_ERR_BASE + 13 */
    GL_WLM_ERR_NOT_EMPTY,                           /* GL_ERR_BASE + 14 */
    GL_WLM_ERR_INV_TASK_ID,                         /* GL_ERR_BASE + 15 */
    GL_WLM_ERR_INDEX_NOT_FOUND,                     /* GL_ERR_BASE + 16 */
    GL_WLM_ERR_INV_INDEX,                           /* GL_ERR_BASE + 17 */
    GL_WLM_ERR_INV_RECORD,                          /* GL_ERR_BASE + 18 */    
    GL_WLM_ERR_NO_RECORD,                           /* GL_ERR_BASE + 19 */
    GL_WLM_CREATE_MUTEX_FAILED,                     /* GL_ERR_BASE + 20 */
    GL_LM_ERR_NOT_EMPTY_LIST,                       /* GL_ERR_BASE + 21 */
    
    GL_WLM_ERR_INV_RECORD_FREE_FUNC_PTR,            /* GL_ERR_BASE + 22 */
    GL_WLM_ERR_NULL_NUM_OF_REC_PTR,                 /* GL_ERR_BASE + 23 */
    
    GL_LM_ERR_MAX                                   /* GL_ERR_BASE + 24 */
                  /* 
                   * Just a placeholder to indicate the last error in 
                   * this category.
                   */
} gl_lm_error_et;


/*
 * Error types for the generic libraries hash table manager.
 */
typedef enum gl_htm_error_e
{
    GL_HTM_ERR_NO_RESOURCES = GL_LM_ERR_MAX,        /* GL_ERR_BASE + 24 */        
    GL_HTM_ERR_INV_BIT_QTY,                         /* GL_ERR_BASE + 25 */
    GL_HTM_ERR_KEY_HAS_ELEMENT,                     /* GL_ERR_BASE + 26 */
    GL_HTM_ERR_NO_ELEMENT,                          /* GL_ERR_BASE + 27 */
    GL_HTM_ERR_INV_HASH_TABLE,                      /* GL_ERR_BASE + 28 */
    GL_HTM_ERR_KEY_NOT_FOUND,                       /* GL_ERR_BASE + 29 */
    GL_HTM_ERR_INV_TASK_ID,                         /* GL_ERR_BASE + 30 */
    GL_HTM_ERR_HASH_TABLE_EMPTY,                    /* GL_ERR_BASE + 31 */
    GL_HTM_ERR_TABLE_NOT_EMPTY,                     /* GL_ERR_BASE + 32 */
    GL_HTM_ERR_NO_NEXT_KEY,                         /* GL_ERR_BASE + 33 */
    GL_HTM_ERR_INV_KEY,                             /* GL_ERR_BASE + 34 */
    GL_HTM_ERR_NO_PREVIOUS_KEY,                     /* GL_ERR_BASE + 35 */
    
    GL_HTM_ERR_INV_KEY_SIZE,                        /* GL_ERR_BASE + 36 */
    GL_HTM_ERR_INV_KEY_PTR,                         /* GL_ERR_BASE + 37 */
    GL_HTM_ERR_INV_KEY_QTY,                         /* GL_ERR_BASE + 38 */
    GL_HTM_ERR_INV_BUCKET_NUM,                      /* GL_ERR_BASE + 39 */
    GL_HTM_ERR_INV_ELEM_FREE_FUNC_PTR,              /* GL_ERR_BASE + 40 */
    GL_HTM_ERR_INV_HASH_FUNC_PTR,                   /* GL_ERR_BASE + 41 */
    GL_HTM_ERR_INV_KEY_COMP_FUNC_PTR,               /* GL_ERR_BASE + 42 */              
    GL_HTM_ERR_DUPLICATE_KEY,                       /* GL_ERR_BASE + 43 */
    GL_HTM_ERR_RESIZE_NOT_REQUIRED,                 /* GL_ERR_BASE + 44 */
    GL_HTM_ERR_NULL_KEY_NODE_HANDLE_PTR,            /* GL_ERR_BASE + 45 */
    GL_HTM_ERR_HASH_WALK_IN_PROGRESS,               /* GL_ERR_BASE + 46 */
    GL_HTM_ERR_GET_MEMORY_FAILS,                    /* GL_ERR_BASE + 47 */
 
    GL_HTM_ERR_RELEASE_MEMORY_FAILS,                /* GL_ERR_BASE + 48 */
    GL_HTM_ERR_INV_HANDLE_TO_HASH_WALK,             /* GL_ERR_BASE + 49 */
    GL_HTM_ERR_KEY_REF_NOT_ZERO,                    /* GL_ERR_BASE + 50 */
    GL_HTM_ERR_NULL_CALLBACK_FUNC_PTR,              /* GL_ERR_BASE + 51 */
    
    GL_HTM_ERR_MAX                                  /* GL_ERR_BASE + 52 */
        
} gl_htm_error_et;


/*
 * Error types for the generic libraries - message queue manager.
 */
typedef enum gl_mqm_error_e
{
    GL_MQM_ERR_INV_MESG_QTY = GL_HTM_ERR_MAX,       /* GL_ERR_BASE + 52 */ 
    GL_MQM_ERR_INV_MESG_SIZE,                       /* GL_ERR_BASE + 53 */
    GL_MQM_ERR_NO_RESOURCE,                         /* GL_ERR_BASE + 54 */
    GL_MQM_ERR_INV_MESG_Q_ID,                       /* GL_ERR_BASE + 55 */
    GL_MQM_ERR_INV_MESG_LEN,                        /* GL_ERR_BASE + 56 */
    GL_MQM_ERR_MESG_Q_NOT_EMPTY,                    /* GL_ERR_BASE + 57 */
    GL_MQM_ERR_INV_PRIORITY,                        /* GL_ERR_BASE + 58 */
    GL_MQM_ERR_MESG_Q_FULL,                         /* GL_ERR_BASE + 59 */
    GL_MQM_ERR_MESG_Q_EMPTY,                        /* GL_ERR_BASE + 60 */
    GL_MQM_ERR_SYS_ERROR,                           /* GL_ERR_BASE + 61 */
    GL_MQM_ERR_INV_PRIORITY_LEVEL,                  /* GL_ERR_BASE + 62 */
    GL_MQM_ERR_INV_MESG_PTR,                        /* GL_ERR_BASE + 63 */ 
    GL_MQM_ERR_NO_MESG_AT_REQ_PRIORITY_LEVEL,       /* GL_ERR_BASE + 64 */
    GL_MQM_ERR_DEADLOCK_ON_READ_WRITE,              /* GL_ERR_BASE + 65 */
    GL_MQM_ERR_MAX                                  /* GL_ERR_BASE + 66 */
} gl_mqm_error_et;


/*
 * Error types for the generic libraries - memory manager.
 */
typedef enum gl_mm_error_e
{
    GL_MM_ERR_MALLOC_FAILED = GL_MQM_ERR_MAX,       /* GL_ERR_BASE + 66 */
    GL_MM_ERR_INV_POOL_ID,                          /* GL_ERR_BASE + 67 */
    GL_MM_ERR_INV_TASK_ID ,                         /* GL_ERR_BASE + 68 */
    GL_MM_ERR_INV_BUF_PTR,                          /* GL_ERR_BASE + 69 */
    GL_MM_ERR_BUF_NOT_AVAIL,                        /* GL_ERR_BASE + 70 */
    GL_MM_ERR_INV_BUF_QTY,                          /* GL_ERR_BASE + 71 */
    GL_MM_ERR_INV_BUF_SIZE,                         /* GL_ERR_BASE + 72 */
    GL_MM_ERR_POOL_NOT_AVAIL,                       /* GL_ERR_BASE + 73 */
    GL_MM_ERR_BUF_ALLOCATED,                        /* GL_ERR_BASE + 74 */
    GL_MM_ERR_BUF_FREE,                             /* GL_ERR_BASE + 75 */
    GL_MM_ERR_INV_OLD_TASK_ID,                      /* GL_ERR_BASE + 76 */
    GL_MM_ERR_INV_NEW_TASK_ID,                      /* GL_ERR_BASE + 77 */
    GL_MM_CREATE_MUTEX_FAILED,                      /* GL_ERR_BASE + 78 */
    GL_MM_ERR_INV_CNGST_MGMT_SPECS_PTR,             /* GL_ERR_BASE + 79 */
    GL_MM_ERR_INV_CNGST_MGMT_SPECS_VALUES,          /* GL_ERR_BASE + 80 */
    GL_MM_ERR_INV_CNGST_MGMT_CALL_BACK_FUNC_PTR,    /* GL_ERR_BASE + 81 */
    GL_MM_ERR_INIT_REQD,                            /* GL_ERR_BASE + 82 */
    GL_MM_ERR_INV_NUM_OF_REQUESTS,                  /* GL_ERR_BASE + 83 */
    GL_MM_ERR_INV_BUFFER_ALOC_SIGNATURE,            /* GL_ERR_BASE + 84 */
    GL_MM_ERR_MEM_OVERWRITE,                        /* GL_ERR_BASE + 85 */
    GL_MM_ERR_INV_POOL_USAGE_PTR,                   /* GL_ERR_BASE + 86 */

    GL_MM_ERR_INTERNAL,                             /* GL_ERR_BASE + 87 */
    GL_MM_ERR_AUDIT_ACTIVITY_NOT_SET,               /* GL_ERR_BASE + 88 */
    GL_MM_ERR_NO_RESOURCES,                         /* GL_ERR_BASE + 89 */
    
    GL_MM_ERR_MAX                                   /* GL_ERR_BASE + 90 */
} gl_mm_error_et;

/*
 * Error types for the generic libraries - timer manager.
 */
typedef enum gl_tm_error_e
{
    GL_TM_ERR_INV_TIMER_LIST = GL_MM_ERR_MAX,       /* GL_ERR_BASE + 90 */
    GL_TM_ERR_INV_DURATION,                         /* GL_ERR_BASE + 91 */
    GL_TM_ERR_INV_TIMER_ID,                         /* GL_ERR_BASE + 92 */
    GL_TM_ERR_SEND_TO_TIMER_SVR,                    /* GL_ERR_BASE + 93 */
    GL_TM_ERR_EXPIRED,                              /* GL_ERR_BASE + 94 */
    GL_TM_ERR_NO_RESOURCE,                          /* GL_ERR_BASE + 95 */
    GL_TM_ERR_INV_FUNCPTR ,                         /* GL_ERR_BASE + 96 */
    GL_TM_ERR_INV_TIMER_QTY,                        /* GL_ERR_BASE + 97 */
    GL_TM_ERR_ACTIVE_TIMERS_PRESENT,                /* GL_ERR_BASE + 98 */
    GL_TM_ERR_INIT_REQD,                            /* GL_ERR_BASE + 99 */
    GL_TM_ERR_MALLOC_FAILED,                        /* GL_ERR_BASE + 100 */
    GL_TM_ERR_MAX                                   /* GL_ERR_BASE + 101 */
} gl_tm_error_et;


/*
 * Error types for the generic libraries - shared memory manager.
 */
typedef enum gl_shm_error_e
{
    GL_SHM_ERR_INV_MEM_SIZE = GL_TM_ERR_MAX,        /* GL_ERR_BASE + 101 */
    GL_SHM_ERR_INV_ACC_MODE,                        /* GL_ERR_BASE + 102 */
    GL_SHM_ERR_INV_ADDR,                            /* GL_ERR_BASE + 103 */
    GL_SHM_ERR_INV_BASE_ADDR,                       /* GL_ERR_BASE + 104 */
    GL_SHM_ERR_SHMEM_ID_IN_USE,                     /* GL_ERR_BASE + 105 */
    GL_SHM_ERR_SHMEM_MAP_FAIL,                      /* GL_ERR_BASE + 106 */
    GL_SHM_ERR_INV_SHM_ID,                          /* GL_ERR_BASE + 107 */
    GL_SHM_ERR_MEM_ALLOC_FAILS,                     /* GL_ERR_BASE + 108 */
    GL_SHM_ERR_NOT_IMPLEMENTED,                     /* GL_ERR_BASE + 109 */
    GL_SHM_ERR_INTERNAL,                            /* GL_ERR_BASE + 110 */
    GL_SHM_ERR_INV_TASK_ID,                         /* GL_ERR_BASE + 111 */
    GL_SHM_ERR_MAX                                  /* GL_ERR_BASE + 112 */
} gl_shm_error_et;

/*
 * Error types for the generic libraries - String Identifier Mapping Manager.
 */
typedef enum gl_sim_error_e
{
    GL_SIM_ERR_INTERNAL = GL_SHM_ERR_MAX,           /* GL_ERR_BASE + 112 */
    GL_SIM_ERR_INV_DB_ID,                           /* GL_ERR_BASE + 113 */
    GL_SIM_ERR_NULL_STRING_ID_PTR,                  /* GL_ERR_BASE + 114 */
    GL_SIM_ERR_DUPLICATE_STRING_ID,                 /* GL_ERR_BASE + 115 */
    GL_SIM_ERR_INV_NUMERIC_ID,                      /* GL_ERR_BASE + 116 */
    GL_SIM_ERR_INV_FREE_MEM_FUNC_PTR,               /* GL_ERR_BASE + 117 */
    GL_SIM_ERR_INV_STRING_TO_MATCH_PTR,             /* GL_ERR_BASE + 118 */
    GL_SIM_ERR_INV_TASK_ID,                         /* GL_ERR_BASE + 119 */
    GL_SIM_ERR_INV_BUCKET_NUM,                      /* GL_ERR_BASE + 120 */
    GL_SIM_ERR_NO_RESOURCES,                        /* GL_ERR_BASE + 121 */
    GL_SIM_ERR_INV_MAP_QTY,                         /* GL_ERR_BASE + 122 */
    GL_SIM_ERR_STRING_ID_NOT_FOUND,                 /* GL_ERR_BASE + 123 */
    GL_SIM_ERR_DB_NOT_EMPTY,                        /* GL_ERR_BASE + 124 */
    GL_SIM_ERR_DB_FULL,                             /* GL_ERR_BASE + 125 */
    GL_SIM_ERR_DB_EMPTY,                            /* GL_ERR_BASE + 126 */
    GL_SIM_ERR_NUMERIC_ID_ALREADY_ALLOCATED,        /* GL_ERR_BASE + 127 */
    GL_SIM_ERR_MAX                                  /* GL_ERR_BASE + 128 */
} gl_sim_error_et;

typedef enum gl_ttm_error_e
{
    GL_TTM_ERR_INTERNAL = GL_SIM_ERR_MAX,           /* GL_ERR_BASE + 128 */
    GL_TTM_ERR_INV_TTM_ID,                          /* GL_ERR_BASE + 129 */
    GL_TTM_ERR_NOT_IMPLEMENTED,                     /* GL_ERR_BASE + 130 */
    GL_TTM_ERR_DUPLICATE_KEY,                       /* GL_ERR_BASE + 131 */
    GL_TTM_ERR_INV_ACC_MODE,                        /* GL_ERR_BASE + 132 */
    GL_TTM_ERR_NO_MATCH,                            /* GL_ERR_BASE + 133 */
    GL_TTM_ERR_INV_TASK_ID,                         /* GL_ERR_BASE + 134 */
    GL_TTM_ERR_MAX                                  /* GL_ERR_BASE + 135 */
} gl_ttm_error_et;

/*
 * Error types for the generic libraries - String Manager.
 */
typedef enum gl_stm_error_e
{
    GL_STM_ERR_NULL_LESS_STRING=GL_TTM_ERR_MAX,     /* GL_ERR_BASE + 135 */
    GL_STM_ERR_NULL_STRING_PTR,                     /* GL_ERR_BASE + 136 */
    GL_STM_ERR_NULL_SRC_STRING_PTR,                 /* GL_ERR_BASE + 137 */
    GL_STM_ERR_NULL_DEST_STRING_PTR,                /* GL_ERR_BASE + 138 */
    GL_STM_ERR_NULL_SEARCH_STRING_PTR,              /* GL_ERR_BASE + 139 */
    GL_STM_ERR_NULL_NUMBER_PTR,                     /* GL_ERR_BASE + 140 */
    GL_STM_ERR_INV_NUM_OF_CHARS,                    /* GL_ERR_BASE + 141 */
    GL_STM_ERR_MAX                                  /* GL_ERR_BASE + 142 */     
}gl_stm_error_et;



/*
 * Error types for the generic libraries - Port Manager.
 */
typedef enum gl_port_error_e
{
    GL_PORT_ERR_NULL_SEMAPHORE_PTR = GL_STM_ERR_MAX,    /* GL_ERR_BASE + 142 */
    GL_PORT_ERR_SEM_CREATE_FAIL,                    /* GL_ERR_BASE + 143 */
    GL_PORT_ERR_SEM_DELETE_FAIL,                    /* GL_ERR_BASE + 144 */
    GL_PORT_ERR_SEM_WAIT_FAIL,                      /* GL_ERR_BASE + 145 */
    GL_PORT_ERR_SEM_TRY_WAIT_FAIL,                  /* GL_ERR_BASE + 146 */
    GL_PORT_ERR_SEM_RELEASE_FAIL,                   /* GL_ERR_BASE + 147 */

    GL_PORT_ERR_NULL_THREAD_INFO_PTR,               /* GL_ERR_BASE + 148 */
    GL_PORT_ERR_THREAD_CREATE_FAILS,                /* GL_ERR_BASE + 149 */
    GL_PORT_ERR_THREAD_SUSPEND_FAILS,               /* GL_ERR_BASE + 150 */
    GL_PORT_ERR_THREAD_RESUME_FAILS,                /* GL_ERR_BASE + 151 */
    GL_PORT_ERR_THREAD_DESTROY_FAILS,               /* GL_ERR_BASE + 152 */

    GL_PORT_ERR_NULL_MUTEX_PTR,                     /* GL_ERR_BASE + 153 */
    GL_PORT_ERR_MUTEX_CREATE_FAILS,                 /* GL_ERR_BASE + 154 */
    GL_PORT_ERR_MUTEX_DELETE_FAILS,                 /* GL_ERR_BASE + 155 */
    GL_PORT_ERR_MUTEX_LOCK_FAILS,                   /* GL_ERR_BASE + 156 */
    GL_PORT_ERR_MUTEX_UNLOCK_FAILS,                 /* GL_ERR_BASE + 157 */
    GL_PORT_ERR_MAX                                 /* GL_ERR_BASE + 158 */           
}gl_port_error_et;

/*
 * Error types for the generic libraries - File Manager.
 */
typedef enum gl_fm_error_e
{
    GL_FM_ERR_NULL_FILE_PTR = GL_PORT_ERR_MAX,      /* GL_ERR_BASE + 158 */
    GL_FM_ERR_NULL_BUFFER_PTR,                      /* GL_ERR_BASE + 159 */
    GL_FM_ERR_INV_FILE_MODE,                        /* GL_ERR_BASE + 160 */
    GL_FM_ERR_INV_SEEK_ORIGIN,                      /* GL_ERR_BASE + 161 */
    GL_FM_ERR_MAX                                   /* GL_ERR_BASE + 162 */
}gl_fm_error_et;

/*
 * Error types for generic libraries - Utility Manager.
 */
typedef enum gl_ulm_error_e
{
    GL_ULM_ERR_INV_STAT_TYPE = GL_FM_ERR_MAX,       /* GL_ERR_BASE + 162 */
    GL_ULM_ERR_INV_STAT_ID,                         /* GL_ERR_BASE + 163 */
    GL_ULM_ERR_MAX                                  /* GL_ERR_BASE + 164 */   
}gl_ulm_error_et;


/*
 *  Error types for generic libraries - Socket Manager.
 */
typedef enum gl_sm_error_e
{
    GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL = GL_ULM_ERR_MAX, /* GL_ERR_BASE + 165 */
    GL_SM_ERR_NO_RESOURCES,                           /* GL_ERR_BASE + 166 */
    GL_SM_ERR_TRANSPORT_NOT_SUPPORTED,                /* GL_ERR_BASE + 167 */
    GL_SM_ERR_NULL_TPT_ID_DATA_PTR,                   /* GL_ERR_BASE + 168 */
    
    GL_ERR_MAX  /* Max gl error */                    /* GL_ERR_BASE + 169 */  
}gl_sm_error_et;


#ifdef __cplusplus
}
#endif

#endif
