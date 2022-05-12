/*****************************************************************************
 ** FUNCTION:	
 **			 Print Api handler protoypes
 **                    
 **                
 **
 ******************************************************************************
 **
 ** FILENAME:		icf_es_int_trace.h
 **
 ** DESCRIPTION:	This file contains API Handler prototypes
 **
 ** DATE			NAME		REFERENCE	   REASON
 ** ----			----		---------	   ------
 ** 11-APR-2005	   Ram Narayan	 None           Initial
 *****************************************************************************
 ** 			Copyrights 2006, Aricent.
 *****************************************************************************/


#ifndef __ICF__INT__TRACE__H__
#define __ICF__INT__TRACE__H__



/* Prohibit Name mangling by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "icf_defs.h"
#include "icf_types.h"
#include "icf_port.h"
#include "icf_port_defs.h"
#include "icf_common_defs.h"
#include "icf_common_api_defs.h"
#include "icf_common_api_types.h"
#include "icf_pa_types.h"
#include "icf_pa_api.h"
#include "icf_cfg_types.h"
#include "icf_cfg_api.h"
#include "icf_mm_types.h"
#include "icf_mm_api.h"
#include "icf_sm_types.h"
#include "icf_sm_api.h"

#include "icf_api_id.h"

#include "ifx_rtp.h"





#define ICF_MAX_BUFF_LEN (256)

#define ICF_MODULE_RTP (8)

#define NUM_API_MAP_ENTRIES (125)

typedef icf_return_t (*icf_print_api_msg_handler)(icf_msg_st*);

icf_return_t icf_interface_trace(icf_msg_st* p_msg);

icf_return_t icf_ifx_interface_trace(icf_return_t src_mod, 
                                               icf_return_t dst_mod, 
                                               icf_return_t api_id, 
                                               void* ifx_msg,
                                               icf_uint16_t len);





#define ICF_INTERFACE_TRACE(p_msg)  \
icf_interface_trace((icf_msg_st*)p_msg);



#define ICF_IFX_INTERFACE_TRACE(src_mod, dst_mod, api_id, ifx_msg, len) \
icf_ifx_interface_trace(src_mod, dst_mod, api_id, ifx_msg, len);




char* icf_get_module_name_from_id(icf_module_id_t id);

char* icf_get_api_name(icf_api_id_t api_id, 
                          icf_module_id_t source_id, 
                          icf_module_id_t destination_id);

icf_print_api_msg_handler icf_get_print_api_handler(char* name);





	/* ims_print_message.c
	   Prototypes of All API print handlers */
	
	icf_return_t icf_print_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_common_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_no_payload_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_resp_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_api_header(icf_msg_st* p_msg);


	icf_return_t icf_print_set_scale_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_trans_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_call_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_self_id_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_reg_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_config_params_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_set_service_params(icf_msg_st* p_msg);
	
	icf_return_t icf_print_incoming_call(icf_msg_st *p_msg);
	icf_return_t icf_print_create_call(icf_msg_st *p_msg);
	icf_return_t icf_print_alert_user(icf_msg_st *p_msg);
	icf_return_t icf_print_user_alerted(icf_msg_st *p_msg);
	
	icf_return_t icf_print_term_call_req(icf_msg_st *p_msg);
	icf_return_t icf_print_term_call_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_display_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_call_log_resp(icf_msg_st *p_msg);
	
	icf_return_t icf_print_call_redirection_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_conference_req(icf_msg_st *p_msg);
	icf_return_t icf_print_ctu_req(icf_msg_st *p_msg);
	icf_return_t icf_print_cta_req(icf_msg_st *p_msg);
	
	
	icf_return_t icf_print_rem_call_transfer_init(icf_msg_st *p_msg);
	icf_return_t icf_print_call_merged_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_party_replaced_ind(icf_msg_st *p_msg);
	

	icf_return_t	icf_print_get_codec_list_resp(icf_msg_st *p_msg);
	icf_return_t	icf_print_gcl_resp(icf_msg_st *p_msg);

	icf_return_t icf_print_rtp_resp_msg(icf_msg_st *p_msg);
	icf_return_t icf_print_rtp_recv_req_msg(icf_msg_st *p_msg);  

	icf_return_t icf_print_unlock_codec_resp(icf_msg_st *p_msg);
	icf_return_t	icf_print_get_codec_list_req(icf_msg_st *p_msg);
    icf_return_t	icf_print_codec_commit_req(icf_msg_st *p_msg);
        
    
    icf_return_t icf_print_cms_req(icf_msg_st *p_msg);
	icf_return_t icf_print_dms_ind(icf_msg_st *p_msg);  
    icf_return_t icf_print_merge_media_req_msg(icf_msg_st *p_msg);    


    icf_return_t icf_print_set_trace_level(icf_msg_st *p_msg);  
    icf_return_t icf_print_set_error_level(icf_msg_st *p_msg);  
    icf_return_t icf_print_get_stats(icf_msg_st *p_msg);  
    icf_return_t icf_print_set_stats(icf_msg_st *p_msg);  

	icf_return_t icf_print_lock_codec_resp(icf_msg_st* p_msg);  
	icf_return_t icf_print_gcl_req(icf_msg_st* p_msg);
	icf_return_t icf_print_lock_codec_req(icf_msg_st* p_msg);
	icf_return_t icf_print_unlock_codec_req(icf_msg_st* p_msg);  
	icf_return_t icf_print_cms_resp(icf_msg_st* p_msg);      
	icf_return_t icf_print_mms_req(icf_msg_st* p_msg);      
   
	icf_return_t icf_print_rtp_err_resp(icf_msg_st* p_msg);      
	icf_return_t icf_print_rm_err_resp(icf_msg_st* p_msg);      


	icf_return_t icf_print_stop_fax_ind(icf_msg_st* p_msg);      
	icf_return_t icf_print_stop_fax_resp(icf_msg_st* p_msg);      
	icf_return_t icf_print_rm_err_resp(icf_msg_st* p_msg);     
 
        icf_return_t icf_print_call_mod_cancel_resp(icf_msg_st* p_msg);


	/* Prohibit Name mangling by C++ compilers */
#ifdef __cplusplus
}
#endif 




#endif
