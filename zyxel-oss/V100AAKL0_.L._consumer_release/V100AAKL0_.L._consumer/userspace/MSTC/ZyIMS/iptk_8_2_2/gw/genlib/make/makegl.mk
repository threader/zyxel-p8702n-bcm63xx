#*******************************************************************************
#               Makefile MACROS for all GENLIB Features
# 
#
# 
# DATE        NAME            REFERENCE        REASON
# ----------- --------------- ---------------- ------------------------------
# 18/07/2002  Ravi Jindal     HSS:xxxxxxxx     Initial
#
# COPYRIGHT , Aricent, 2006
#
#*******************************************************************************

CMN_BASE        =       $(GW_BASE)$(GW_SLASH)common
GL_BASE         =       $(GW_BASE)$(GW_SLASH)genlib

GL_CMN_DIR      =       $(GL_BASE)$(GW_SLASH)common
GL_OBJ_DIR      =       $(GL_BASE)$(GW_SLASH)obj

GL_OS_DIR       =       $(GL_BASE)$(GW_SLASH)os
GL_OS_SRC_DIR   =       $(GL_OS_DIR)$(GW_SLASH)src

GL_DB_DIR       =       $(GL_BASE)$(GW_SLASH)db
GL_DB_SRC_DIR   =       $(GL_DB_DIR)$(GW_SLASH)src

GL_CMN_SRC_DIR  =       $(GL_CMN_DIR)$(GW_SLASH)src


#***********************************************************
# MACRO For Source files and Object Files of COMMON Dir.
#**********************************************************

GL_CMN_SRC_FILES = $(GL_CMN_SRC_DIR)$(GW_SLASH)gl_ulm_str_manipultn_api.c\
		   $(GL_CMN_SRC_DIR)$(GW_SLASH)gl_ulm_gnrl_utils_api.c\
		   $(GL_CMN_SRC_DIR)$(GW_SLASH)gl_ulm_stats_api.c

GL_CMN_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_ulm_str_manipultn_api.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_ulm_gnrl_utils_api.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_ulm_stats_api.$(OBJ_SUFFIX)



#***********************************************************
# MACRO For Source files and Object Files of OS Dir.
#**********************************************************


GL_MM_SRC_FILES = $(GL_OS_SRC_DIR)$(GW_SLASH)gl_mm_glb_best_fit_alloc_schm.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_mm_appl_best_fit_alloc_schm.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_mm_appl_sp_slectv_aloc_schm.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_mm_glb_defs_n_intrnl_funcs.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_mm_init_deint_api_funcs.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_mm_cngst_mgmt_cnfg_api.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_mm_usage_mgmt_n_debug_ctrl.c

GL_MM_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_mm_glb_best_fit_alloc_schm.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_mm_appl_best_fit_alloc_schm.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_mm_appl_sp_slectv_aloc_schm.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_mm_glb_defs_n_intrnl_funcs.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_mm_init_deint_api_funcs.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_mm_cngst_mgmt_cnfg_api.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_mm_usage_mgmt_n_debug_ctrl.$(OBJ_SUFFIX)



GL_TM_SRC_FILES	= $(GL_OS_SRC_DIR)$(GW_SLASH)gl_tm_basic_timer_oprns_api.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_tm_timer_server_impl_api.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_tm_glb_defs_n_intrnl_funcs.c\
		  $(GL_OS_SRC_DIR)$(GW_SLASH)gl_tm_init_deinit_api_funcs.c

GL_TM_OBJ_FILES	= $(GL_OBJ_DIR)$(GW_SLASH)gl_tm_basic_timer_oprns_api.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_tm_timer_server_impl_api.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_tm_glb_defs_n_intrnl_funcs.$(OBJ_SUFFIX)\
		  $(GL_OBJ_DIR)$(GW_SLASH)gl_tm_init_deinit_api_funcs.$(OBJ_SUFFIX)


GL_TMPORT_SRC_FILES = $(GL_OS_SRC_DIR)$(GW_SLASH)gl_tmport.c

GL_TMPORT_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_tmport.$(OBJ_SUFFIX)


GL_GSCIM_SRC_FILES = $(GL_OS_SRC_DIR)$(GW_SLASH)gl_gscim_mutex_oprns_wrppr_api.c\
		     $(GL_OS_SRC_DIR)$(GW_SLASH)gl_gscim_semphore_op_wrppr_api.c\
		     $(GL_OS_SRC_DIR)$(GW_SLASH)gl_gscim_thrd_oprns_wrppr_api.c\
		     $(GL_OS_SRC_DIR)$(GW_SLASH)gl_gscim_file_io_opr_wrppr_api.c


GL_GSCIM_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_gscim_mutex_oprns_wrppr_api.$(OBJ_SUFFIX)\
		     $(GL_OBJ_DIR)$(GW_SLASH)gl_gscim_semphore_op_wrppr_api.$(OBJ_SUFFIX)\
		     $(GL_OBJ_DIR)$(GW_SLASH)gl_gscim_thrd_oprns_wrppr_api.$(OBJ_SUFFIX)\
		     $(GL_OBJ_DIR)$(GW_SLASH)gl_gscim_file_io_opr_wrppr_api.$(OBJ_SUFFIX)



GL_SKM_SRC_FILES = $(GL_OS_SRC_DIR)$(GW_SLASH)gl_skm_abstractd_wrppr_api.c\
		   $(GL_OS_SRC_DIR)$(GW_SLASH)gl_skm_sys_calls_wrppr_api.c

GL_SKM_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_skm_abstractd_wrppr_api.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_skm_sys_calls_wrppr_api.$(OBJ_SUFFIX)



#***********************************************************
# MACRO For Source files and Object Files of DB Dir.
#**********************************************************

GL_HTM_SRC_FILES = $(GL_DB_SRC_DIR)$(GW_SLASH)gl_htm_user_key_oprns_api.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_htm_glb_defs_n_intrnl_funcs.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_htm_init_deinit_api_funcs.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_htm_port_api_funcs.c

GL_HTM_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_htm_user_key_oprns_api.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_htm_glb_defs_n_intrnl_funcs.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_htm_init_deinit_api_funcs.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_htm_port_api_funcs.$(OBJ_SUFFIX)



GL_LLM_SRC_FILES = $(GL_DB_SRC_DIR)$(GW_SLASH)gl_llm_glb_defs_n_intrnl_funcs.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_llm_init_deinit_api_funcs.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_llm_basic_list_oprns_api.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_llm_advncd_list_oprns_api.c

GL_LLM_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_llm_glb_defs_n_intrnl_funcs.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_llm_init_deinit_api_funcs.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_llm_basic_list_oprns_api.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_llm_advncd_list_oprns_api.$(OBJ_SUFFIX)



GL_MQM_SRC_FILES = $(GL_DB_SRC_DIR)$(GW_SLASH)gl_mqm_glb_defs_n_intrnl_funcs.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_mqm_init_deinit_api_funcs.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_mqm_basic_snd_rcv_oprns_api.c

GL_MQM_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_mqm_glb_defs_n_intrnl_funcs.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_mqm_init_deinit_api_funcs.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_mqm_basic_snd_rcv_oprns_api.$(OBJ_SUFFIX)


GL_SIM_SRC_FILES = $(GL_DB_SRC_DIR)$(GW_SLASH)gl_simm_fwd_bkwd_map_oprns_api.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_simm_glb_defs_n_intrn_funcs.c\
		   $(GL_DB_SRC_DIR)$(GW_SLASH)gl_simm_init_deinit_api_funcs.c

GL_SIM_OBJ_FILES = $(GL_OBJ_DIR)$(GW_SLASH)gl_simm_fwd_bkwd_map_oprns_api.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_simm_glb_defs_n_intrn_funcs.$(OBJ_SUFFIX)\
		   $(GL_OBJ_DIR)$(GW_SLASH)gl_simm_init_deinit_api_funcs.$(OBJ_SUFFIX)


