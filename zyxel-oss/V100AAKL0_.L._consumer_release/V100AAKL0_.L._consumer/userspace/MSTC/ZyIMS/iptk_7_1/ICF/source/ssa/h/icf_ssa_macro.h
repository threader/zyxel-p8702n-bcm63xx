/***************************************************************************
 *
 * File name        : icf_ssa_macro.h
 *
 * Purpose          : This file contains the prototypes
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 29-Jan-2005  Umang Singh    SSA LLD              Initial
 ** 07-JUL-2005     Aashish Suchdev                     TCP checkin            
 * 06-Mar-2006  Amit Sharma                         ICF merging with ICF2.0
 *
 * Copyright (c) 2006, Aricent.
 **************************************************************************/
#ifndef __ICF_SSA_MACRO_H__
#define __ICF_SSA_MACRO_H__
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus
/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
#endif /* __cplusplus */

#ifdef ICF_DEBUG_TRACE_ENABLE
#include "icf_sm_int_types.h"
#endif

extern icf_glb_pdb_st     *p_persistent_glb_pdb;

#define ICF_SSA_INIT_TRANS_ADDR(p_addr)\
{\
    (p_addr)->addr.addr_type = 0;\
    (p_addr)->addr.addr.domain.str[0] = '\0';\
    (p_addr)->port_num = 0;\
}
#define ICF_SSA_RESET_CONN_TIMER_COUNT(p_conn_table_entry)\
    p_conn_table_entry->timer_count = 0;

#define ICF_SSA_INC_CONN_REF_COUNT(p_conn_table)\
    p_conn_table->ref_count++;

#define ICF_SSA_DEC_CONN_REF_COUNT(p_conn_table)\
    if (p_conn_table->ref_count > 0) \
        p_conn_table->ref_count--;

#define ICF_SSA_SET_COMMON_BITMASK(common_bitmask, bit)\
     common_bitmask |= bit; \

#define ICF_SSA_GET_COMMON_BITMASK(common_bitmask,bit) \
     (common_bitmask & bit)


#define ICF_SSA_RESET_COMMON_BITMASK(common_bitmask,bit)\
    common_bitmask &= ~bit; \

#ifdef ICF_DNS_LOOKUP_ENABLED
#define ICF_SSA_SET_TXN_DNS_STATE(p_txn, p_overlap_txn, state)\
{\
    if (Sdf_co_null == p_txn) \
    { \
	p_overlap_txn->dTxnDnsState = state;\
    } \
    else \
    { \
	p_txn->dTxnDnsState = state;\
    } \
} /* End*/
#endif

/* 7.0 Printing the connection DB table entry */
#ifdef ICF_DETAILED_DEBUG_TRACE
#define ICF_SSA_PRINT_CONN_DB(p_conn_db) icf_ssa_print_conn_db (p_conn_db);
#else
#define ICF_SSA_PRINT_CONN_DB(p_conn_db)
#endif
#ifdef ICF_DETAILED_DEBUG_TRACE
#define ICF_SSA_PRINT_ALL_CONNECTION(p_ssa_pdb) icf_ssa_print_all_connection (p_ssa_pdb);
#else
#define ICF_SSA_PRINT_ALL_CONNECTION(p_ssa_pdb)
#endif
/******************************************************************************
 *
 * Macro Name       : ICF_SSA_SET_ECODE
 * Description      : Macro for ecode handling
 *
 ******************************************************************************/
#ifdef ICF_ERROR_ENABLE
#define ICF_SSA_SET_ECODE(p_ssa_pdb,p_method,ecode)\
{\
icf_ssa_error(p_ssa_pdb,p_method,(icf_error_t)ecode);\
}
#else
#define ICF_SSA_SET_ECODE(p_ssa_pdb,p_method, error)\
{\
    *(p_ssa_pdb->p_ecode) = (icf_error_t)(error);\
}
#endif
/*
#define ICF_SSA_RESET_CALLID_CTXT_MAP(p_ssa_ctxt)      \
{                                                                      \
    if (p_ssa_ctxt->callobj_map_index != ICF_SSA_MAX_CALL_OBJ && \
               p_ssa_ctxt == \ 
                  p_ssa_ctxt->p_ssa_glb_data->call_id_obj_map[p_ssa_ctxt->\
callobj_map_index].p_ssa_ctxt) \ 
   { \
        p_ssa_ctxt->p_ssa_glb_data->call_id_obj_map[p_ssa_ctxt->callobj_map_index].p_ssa_ctxt = ICF_NULL; \
        p_ssa_ctxt->callobj_map_index = ICF_SSA_MAX_CALL_OBJ;\
    } \
}*/ /* End*/

#define ICF_SSA_GET_GLB_PDB_FRM_INTRNL_MSG(p_glb_pdb,p_internal_msg)\
{\
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;\
}/*End*/

#define ICF_SSA_GET_RECV_MSG_FRM_INTRNL_MSG(p_recvd_msg,p_internal_msg)\
{\
    p_recvd_msg = p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg;\
}

#define ICF_SSA_GET_EXT_API_HDR_FRM_INTRNL_MSG(p_ext_api_hdr,p_internal_msg)\
{\
    p_ext_api_hdr = &(p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->hdr);\
}

#define ICF_SSA_GET_EXT_PAYLOAD_FRM_INTRNL_MSG(p_ext_msg,p_internal_msg)\
{\
    p_ext_msg = p_internal_msg->msg_hdr.p_glb_pdb->p_recv_msg->payload;\
}

#define ICF_SSA_FREE_SSA_SCOPE_MEM_CHUNKS(p_glb_pdb, p_ssa_pdb, p_ecode, ret_val)\
{\
    icf_uint8_t  count = 0, max = p_ssa_pdb->alloc_index;\
        icf_ssa_scope_mem_list_st   *p_mem_list_entry = \
        ICF_NULL;\
        for ( count =0 ; count < max;)\
        {\
            p_mem_list_entry = &(p_ssa_pdb->allocated_memory[count]);\
                if ( ICF_NULL == p_mem_list_entry->p_mem_chunk)\
                {\
                    break;\
                }\
            if ( ICF_SSA_MEM_TYPE_DBM_SDP == \
                    p_mem_list_entry->ssa_mem_type)\
            {\
                icf_internal_sdp_stream_st *p_int_sdp = \
                    (icf_internal_sdp_stream_st *)(p_mem_list_entry->p_mem_chunk); \
                if (ICF_NULL != \
                    p_int_sdp->sdp_stream.p_encoded_attributes) \
                { \
                    icf_cmn_delete_list( \
                        p_ssa_pdb->p_glb_pdb, \
                        &(p_int_sdp->sdp_stream.p_encoded_attributes), \
                        ICF_MEM_COMMON); \
                } \
                icf_dbm_ret_mem_to_pool(\
                    p_glb_pdb, ICF_MEM_SDP,\
                    (icf_void_t *)(p_mem_list_entry->p_mem_chunk),\
                    p_ssa_pdb->p_ecode);\
            }\
            else if ( ICF_SSA_MEM_TYPE_GL_CMN == \
                    p_mem_list_entry->ssa_mem_type)\
            {\
                ICF_MEMFREE(p_glb_pdb, p_mem_list_entry->p_mem_chunk, \
                        ICF_MEM_COMMON,\
                        p_ecode, ret_val)\
            }\
            else if ( ICF_SSA_MEM_TYPE_MEDIA_SDP_INFO_LIST == \
                    p_mem_list_entry->ssa_mem_type)\
            {\
                icf_cmn_free_media_sdp_info_list(\
                    p_glb_pdb, p_mem_list_entry->p_mem_chunk, p_ecode); \
            }\
            count++;\
        }\
}

#define ICF_SSA_ADD_MEM_CHUNK_TO_PTR_LIST(p_ssa_pdb, p_alloc_chunk,type)\
{\
    if ( ICF_SSA_MAX_MMRY_CHUNK_LST == \
            p_ssa_pdb->alloc_index)\
    {\
        ret_val = ICF_FAILURE; /*Raise Critical Error here*/\
    }\
    else \
    {\
        p_ssa_pdb->allocated_memory[p_ssa_pdb->alloc_index].p_mem_chunk\
            = p_alloc_chunk;\
        p_ssa_pdb->allocated_memory[p_ssa_pdb->alloc_index].ssa_mem_type\
            = type;\
            p_ssa_pdb->alloc_index++;\
    }\
}

#define ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_blk)\
{\
    p_ssa_timer_blk->timer_id = ICF_TIMER_ID_INVALID;\
    p_ssa_timer_blk->p_timer_data = ICF_NULL;\
    p_ssa_timer_blk->p_ssa_ctxt = ICF_NULL;\
\
    p_ssa_timer_blk->p_uatk_timer_cbk = Sdf_co_null;\
    p_ssa_timer_blk->p_uatk_ctxt_info = Sdf_co_null;\
}
#define ICF_SSA_FREE_DIV_DATA_LIST(p_ssa_pdb, p_div_info)\
{\
    icf_glb_pdb_st           *p_glb_pdb = \
        p_ssa_pdb->p_glb_pdb;\
    icf_diversion_data_st    *p_traverse = \
        p_div_info->p_diversion_data;\
    icf_diversion_data_st    *p_trav = ICF_NULL;\
    if ( ICF_NULL != p_traverse)\
    {\
        do\
        {\
            p_trav = p_traverse;\
            p_traverse = p_trav->p_next;\
            ICF_MEMFREE(p_glb_pdb, (p_trav), \
                    ICF_MEM_COMMON,\
                    (p_ssa_pdb->p_ecode), ret_val)\
        }while(p_traverse != NULL);\
        p_div_info->no_of_diversion_data = 0;\
        p_div_info->no_of_hops = 0;\
    }\
}

#define ICF_SSA_ADD_DIV_DATA_TO_DIV_INFO(p_div_info, p_div_data)\
{\
    icf_diversion_data_st    *p_traverse = \
        p_div_info->p_diversion_data;\
    icf_uint8_t              div_count = 0;\
    if ( ICF_NULL == p_traverse)\
    {\
        p_div_info->p_diversion_data = p_div_data;\
    }\
    else\
    {\
        div_count++; \
        while(ICF_NULL != p_traverse->p_next)\
        {\
            p_traverse = (icf_diversion_data_st*)\
                (p_traverse->p_next);\
            div_count++;\
        }\
        p_traverse->p_next = p_div_data;\
    }\
    p_div_info->no_of_diversion_data = div_count+1;\
}

#define ICF_SSA_IVK_TCPSM(p_ssa_pdb,event, ret_val)\
{\
    icf_uint8_t  state = p_ssa_pdb->\
        p_conn_table_entry->tcpm_sm_state;\
    ret_val = ((icf_ssa_tcpm_fsm_func_ptr_t)\
        (p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table\
        [state][event]))(p_ssa_pdb);\
}

#define ICF_SSA_IVK_TCPSM_FRM_CTX(p_ssa_pdb, p_ssa_ctxt, event, ret_val)\
{\
    icf_uint8_t  state = p_ssa_pdb->\
        p_conn_table_entry->tcpm_sm_state;\
    ret_val = ((icf_ssa_tcpm_fsm_func_ptr_t)\
        (p_ssa_pdb->p_ssa_glb_data->ssa_tcpm_fsm_table\
        [state][event]))(p_ssa_pdb);\
}

#ifdef ICF_DEBUG_TRACE_ENABLE
#define ICF_SSA_DEBUG(str)\
{\
    icf_sm_glb_data_st               *p_sm_data =  ICF_NULL;\
    icf_trace_type_t glb_trace_type =  ICF_NULL;\
\
    p_sm_data = \
     p_persistent_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_SM];\
\
     glb_trace_type = p_sm_data->trace_type;\
\
    if (ICF_TRACE_TYPE_SIP_STACK_DEBUG & glb_trace_type)\
        printf str;\
}
#else
#define ICF_SSA_DEBUG(str)
#endif

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SSA_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_SSA_TRACE(p_ssa_pdb, ssa_trace_id) \
{\
    icf_ssa_trace(p_ssa_pdb, (icf_trace_id_t)(ssa_trace_id)); \
} /* #define ICF_SSA_TRACE(p_pdb... */
#else
#define ICF_SSA_TRACE(p_ssa_pdb, ssa_trace_id)
#endif


#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SSA_APP_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
#define ICF_SSA_APP_TRACE(p_ssa_pdb, ssa_trace_id) \
{ \
    icf_ssa_app_trace(p_ssa_pdb, (icf_trace_id_t)(ssa_trace_id)); \
} /* #define ICF_SSA_APP_TRACE(p_pdb... */
#else
#define ICF_SSA_APP_TRACE(p_ssa_pdb, ssa_trace_id)
#endif

#ifdef ICF_SESSION_TIMER
#define ICF_SSA_SET_SESSION_REFRESH_REQ(p_ssa_ctxt,method)\
{\
	p_ssa_ctxt->refresher_method_type = method;\
}
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*  __ICF_SSA_MACRO_H__ */

