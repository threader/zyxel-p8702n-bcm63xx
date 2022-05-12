/*************************************************************************
 *
 * 	FILENAME	: 	icf_common_prototypes.h
 *
 * 	DESCRIPTION	:	This file contains the prototypes that are used for 
 * 	                common utility functions.
 * 					
 * 	Revision History	:
 * 		DATE		NAME				REFERENCE			REASON
 * 		----------------------------------------------------------
 *      18-Dec-2004   R. Ezhirpavai      ICF LLD         Initial version.
 *      04-Jan-2005   A. Suchdev                        Updated prototype
 *                                                      cmn_memget and memfree
 *      06-Jan-2005   A. Suchdev                        Updated prototype
 *                                                      for start/stop timers
 *      10-Jan-2005   A. Suchdev                        removed unused proto
 *      11-Jan-2005   A. Suchdev                        corrected name for
 *                                                      init stats
 *      11-Feb-2005   Amit Sharma                       Adding a function to 
 *      												send resp structure
 *      25-Feb-2005   Navneet Gupta                     Added prototype for SCM 
 *                                                      de-init function.
 *      12-May-2005   Sumit Gupta                       Added extern definition 
 *                                                      for SM call logging 
 *                                                      functions
 *      29-May-2005   Amit Sharma       SPR:7623        p_ecode is assigned
 *		04-Aug-2005	  Rohit Aggarwal	ICF				Added compile time flags
 *														for errors and traces
 *		10-Aug-2005	  Rohit Aggarwal	ICF		        Added certain utility 
 *														functions
 *		23-Aug-2005	  Ashutos Rath  	ICF		        Added init and deinit 
 *														function for SIGSA
 *
 *		12-Dec-2005  Ashutos Rath		ICF 4.0			XML parser related 
 *														prototypes added
 *      02-Feb-2006  Amit Sharma                        CFG merged with ICF 2.0
 *		25-July-2006 Aman Arora			ICF rel 6.0     Added functions for 
 *													    managing the generic lists
 *													    within ICF.
 *      31-July-2006 Aman Aggarwal      ICF rel 6.0     Changed icf_list_st to 
 *                                                      icf_internal_list_st in 
 *                                                      free_stream_sdp_info_list.
 *
 * 4-Dec-2006       Deepti Goyal        ICF Rel 6.2     NAT Feature Changes
 * 9-Jul-2007       Neha Choraria       ICF Rel 7.0     Changes Related to Forking
 *                                                      and Multiple Answer
 * 11-Jul-2007      Neha Choraria       ICF Rel 7.0     Added icf_cmn_generate_timer
 *                                                      for changes in Retry-After
 * 16-Jan-2008      Shruti Thakral                      Changes for secondary 
 *                                                      server support in Rel8.0
 *
 * 24-Jan-2008      Tarun Gupta       IPTK Rel 8.0 LLD  Modified function
 *                                                      signatures and added 
 *                                                      new prototypes for AKA.
 * 10-Mar-2008      Divij Agarwal       SPR 18122       Support for async AKA
 *                                                      negotiation
 * 29_Apr-2008      Shruti Thakral      SPR 18455       Klocwork warning removal
 * 04-Nov-2008      Rajiv Kumar         SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                                      19001) 
 * 25-Nov-2008  Abhishek Dhammawat      SPR 19189       Merge SPR 18882 cycle3
 * 01-Dec-2008  Abhishek Dhammawat      SPR 19218       Merge CSR1-6214356 
 * 09-Feb-2009      Tarun Gupta         Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 02-Mar-2009      Kamal Ashraf        Rel 8.1         Prototype added for 
 *                                                      icf_cmn_convert_ip_str_to_transport_addr
 *                                                      for open source replacement 
 * 30-Mar-2009      Alok Tiwari         Rel 8.2         Modified prototype:
 *                                           icf_cmn_process_aka_challenge
 * 01-Apr-2009      Kamal Ashraf        Rel 8.2         Prototype added for icf_cmn_init_
 *                                                      trace_data_st and icf_cmn_init_
 *                                                      error_data_st for klocwork fix.
 * 23-Apr-2009 Abhishek Dhammawat       SPR 19502      Added icf_cmn_map_interface_reason_code
 * 26-May-2009 Preksha                  Rel 8.2         Asynchronus Message enhancements
 * 08-Jun-2009 Abhishek Dhammawat       SPR 19590      Added  icf_cmn_make_tag_list_node
 * 28-Jul-2009 Rajiv Kumar              SPR 19886      Fix merged for
 *                                                     CSR-1-7302347
 * 29-May-2010 Alok Tiwari              SPR 20412       Merged CSR 1-8223807
 *
 * 	Copyright 2010, Aricent.
 *************************************************************************/

#ifndef __ICF_COMMON_PROTOTYPES_H__
#define __ICF_COMMON_PROTOTYPES_H__

/******************** STANDARD INCLUDE FILES *****************************/

/******************** USER INCLUDE FILES *********************************/
#include "icf_common_types.h"
#include "icf_common_interface_types.h"
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
 *   */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */

extern "C"
{
#endif /* __cplusplus */

#ifdef ICF_ERROR_ENABLE
extern void icf_cmn_raise_error_n_trace(
        INOUT icf_glb_pdb_st *p_glb_pdb, 
        INOUT icf_error_t *p_ecode,
        IN icf_error_t error_id,
        INOUT icf_error_data_st *p_error_data);
#endif

#ifdef ICF_TRACE_ENABLE
extern icf_void_t icf_cmn_trace( 
        INOUT icf_glb_pdb_st *p_glb_pdb, 
        IN    icf_trace_id_t  trace_id, 
        IN    icf_trace_data_st *p_trace_data); 

extern icf_void_t icf_cmn_app_trace(
         icf_glb_pdb_st *p_glb_pdb,
         icf_trace_id_t  trace_id,
         icf_trace_data_st *p_trace_data,
         icf_trace_criteria_data_st *p_criteria_data);

extern icf_return_t icf_cmn_capture_app_trace(
        INOUT  icf_glb_pdb_st *p_glb_pdb,
        IN     icf_trace_id_t  trace_id,
        IN    icf_msg_st      *p_msg_to_be_traced);

#endif


extern icf_void_t* icf_cmn_memget(
        INOUT   icf_glb_pdb_st       *p_glb_pdb,
        IN      icf_uint32_t         size,
        IN      icf_pool_id_t        pool_id,
        OUT     icf_error_t          *p_ecode );

extern icf_return_t icf_cmn_memfree(
            INOUT   icf_glb_pdb_st        *p_glb_pdb,
            INOUT      icf_void_t*           p_buf,
            IN      icf_pool_id_t     pool_id,
            OUT     icf_error_t    *p_ecode );

extern icf_return_t icf_cmn_static_memfree(
        INOUT  icf_glb_pdb_st   *p_glb_pdb,
        IN icf_void_t *p_buff, 
        OUT icf_error_t *p_ecode);

extern icf_void_t* icf_cmn_static_memget(
        INOUT  icf_glb_pdb_st   *p_glb_pdb,
        IN  icf_uint32_t size, 
        OUT icf_error_t *p_ecode);

extern icf_return_t icf_cmn_start_timer(
        INOUT  icf_glb_pdb_st   *p_glb_pdb,
        IN icf_timer_data_st *p_buff, 
        IN icf_duration_t   duration, 
        OUT icf_timer_id_t  *p_timer_id, 
        OUT icf_error_t *p_ecode);

extern icf_return_t icf_cmn_stop_timer(
        INOUT  icf_glb_pdb_st   *p_glb_pdb,
        IN  icf_timer_id_t timer_id, 
        OUT icf_error_t *p_ecode);
extern icf_return_t icf_cmn_convert_ip_string_to_transport_addr(
        IN icf_uint8_t *p_IP,
        OUT icf_transport_addr_st *p_address);

extern icf_return_t icf_cmn_convert_ip_string_to_transport_address(
        IN icf_uint8_t *p_IP,
        OUT icf_transport_address_st *p_address);

extern icf_return_t icf_cmn_convert_transport_address_to_ip_string(
        IN icf_transport_address_st *p_rtp_address,
        OUT icf_uint8_t *p_remoteIP);

extern icf_return_t  icf_adm_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t  icf_mmi_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t  icf_nm_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_mmi_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t  icf_ssa_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);


extern icf_return_t icf_ssa_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t  icf_sic_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_sic_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t icf_nm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#ifdef ICF_SIGCOMP_SUPPORT

extern icf_return_t  icf_sigsa_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_sigsa_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#endif
extern icf_return_t  icf_sm_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_sm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t icf_mph_process_mesg(
		INOUT icf_internal_msg_st *p_internal_msg);

extern icf_return_t icf_mph_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_p_mph_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t  icf_cfg_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_cfg_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t  icf_regm_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_regm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t icf_dbm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_error_t    *p_ecode);

#if defined ICF_CW_SERVICE || ICF_CHR_SERVICE || ICF_TWC_SERVICE || ICF_CTA_SERVICE || ICF_CTU_SERVICE
extern icf_return_t  icf_chr_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_chr_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#endif

extern icf_return_t   icf_scm_init(
                INOUT   icf_glb_pdb_st        *p_glb_pdb,
                OUT     icf_void_t            **p_p_scm_data,
                OUT     icf_error_t           *p_ecode );

extern icf_return_t   icf_scm_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg);

extern icf_return_t  icf_cc_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_cc_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);


extern icf_return_t  icf_cm_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_cm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#ifdef ICF_TWC_SERVICE
extern icf_return_t  icf_twc_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_twc_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CTA_SERVICE
extern icf_return_t  icf_cta_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_cta_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CTU_SERVICE
extern icf_return_t  icf_ctu_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_ctu_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_RNW_SERVICE
extern icf_return_t  icf_rnw_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_rnw_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CW_SERVICE
extern icf_return_t  icf_cw_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_cw_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CFW_SERVICE
extern icf_return_t  icf_cfw_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_cfw_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);
#endif
extern icf_return_t  icf_xconf_process_mesg(
       INOUT icf_internal_msg_st  *p_msg);

extern icf_return_t icf_xconf_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_xconf_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t icf_cmn_error_init(
        INOUT icf_glb_data_st *p_glb_data);

extern icf_return_t icf_cmn_init_stats(
        INOUT   icf_glb_pdb_st   *p_glb_pdb,
        OUT     icf_error_t      *p_ecode);

extern icf_return_t icf_cmn_deinit_stats(
        INOUT   icf_glb_pdb_st    *p_glb_pdb,
        OUT     icf_error_t       *p_ecode);

extern icf_return_t icf_cmn_trace_init(
        INOUT icf_glb_data_st *p_glb_data);

extern icf_return_t icf_cmn_app_trace_init(
        INOUT icf_glb_data_st *p_glb_data);

extern icf_void_t icf_mmi_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_ssa_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_nm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#ifdef ICF_SIGCOMP_SUPPORT
extern icf_void_t icf_sigsa_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);
#endif
extern icf_void_t icf_sm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_mph_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_cfg_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_regm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t 
icf_dbm_deinit(
        INOUT icf_glb_pdb_st            *p_glb_pdb,
        INOUT icf_error_t               *p_ecode);


#if defined ICF_CW_SERVICE || ICF_CHR_SERVICE || ICF_TWC_SERVICE || ICF_CTA_SERVICE || ICF_CTU_SERVICE

extern icf_void_t icf_chr_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#endif

extern icf_void_t icf_cc_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_cm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

extern icf_void_t icf_scm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#ifdef ICF_TWC_SERVICE

extern icf_void_t icf_twc_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CTA_SERVICE

extern icf_void_t icf_cta_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CTU_SERVICE

extern icf_void_t icf_ctu_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_RNW_SERVICE

extern icf_void_t icf_rnw_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CW_SERVICE

extern icf_void_t icf_cw_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);

#endif

#ifdef ICF_CFW_SERVICE

extern icf_void_t icf_cfw_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode);
#endif

icf_return_t icf_nm_process_mesg(
		INOUT   icf_internal_msg_st	*p_msg);

/* ICF_INIT_START */
extern icf_return_t icf_nm_init(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_void_t     **p_data,
        OUT   icf_error_t    *p_ecode);
/* ICF_INIT_END */

/************************************************************************
 * NAME:          icf_cmn_fill_resp_msg 
 * DESCRIPTION:   Function is used to fill the response structure
 *                (icf_msg_resp_st) to be sent
 *                to PA, RM, CFG etc modules 
 ***********************************************************************/
extern  icf_void_t     icf_cmn_fill_resp_msg
(
        INOUT        icf_glb_pdb_st        *p_glb_pdb,
        INOUT        icf_msg_st            *p_msg,
        IN           icf_module_id_t       dest_id,
        IN           icf_api_id_t          resp_api_id,
        IN           icf_result_t          result,
        IN           icf_error_t           error_cause
);

/************************************************************************
 * NAME:          icf_cmn_send 
 * DESCRIPTION:   Function is used to send message to PA/RM/RTP
 * 
 ***********************************************************************/

extern icf_return_t icf_cmn_send(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        INOUT icf_msg_st *p_msg_data,
        OUT icf_error_t *p_ecode);        

/****************************************************************************
*
* DESCRIPTION :   This function reports the genlib timer expiry to a module.
*
*****************************************************************************/
extern icf_void_t
icf_mrm_timer_cb_fn(
        INOUT icf_void_t *p_buf);

/****************************************************************************
*
* DESCRIPTION :   Adds a call into missed call list
*
*****************************************************************************/
extern icf_return_t icf_sm_set_missed_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb);

/****************************************************************************
*
* DESCRIPTION :   Moves a call from missed call list to received call list
*
*****************************************************************************/
extern icf_return_t icf_sm_set_recd_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb);

/****************************************************************************
*
* DESCRIPTION :   Adds a call into dialed call list
*
*****************************************************************************/
extern icf_return_t icf_sm_set_dialed_call(
        INOUT   icf_glb_pdb_st *p_glb_pdb);

#ifdef ICF_STAT_ENABLED
extern icf_void_t icf_stat_count(
        IN   icf_glb_pdb_st  *p_glb_pdb,
        IN   icf_stats_id_t stat_id,
        IN   icf_uint32_t count);
#endif

#ifdef ICF_PORT_WINDOWS
/* this is added  by vidyut */

char * self_implemented_strtok_r(char *s, const char *delim, char **last);
#endif 



ICF_SESSION_CONTROL_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_header_list
 *
 * DESCRIPTION:     This is a utility function that frees the header list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_header_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_header_list_st	*p_header_list,
		OUT		icf_error_t			*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_tag_list
 *
 * DESCRIPTION:     This is a utility function that frees the tag list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_tag_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_tag_list_st		*p_tag_list,
		OUT		icf_error_t			*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_msg_body_list
 *
 * DESCRIPTION:     This is a utility function that frees the body list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_msg_body_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_msg_body_list_st	*p_body_list,
		OUT		icf_error_t			*p_ecode);

#ifdef ICF_REL_OLD
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_stream_list
 *
 * DESCRIPTION:     This is a utility function that frees the stream list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_stream_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        INOUT	icf_stream_id_list_st	*p_stream_list,
		OUT		icf_error_t			*p_ecode);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_header_list
 *
 * DESCRIPTION:     This function makes a copy of header list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_header_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
		IN		icf_header_list_st	*p_src_header_list,
        OUT		icf_header_list_st	*p_dst_header_list,
		OUT		icf_error_t			*p_ecode);

ICF_CRM_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_header_name_list
 *
 * DESCRIPTION:     This function makes a copy of header list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_header_name_list(
		INOUT	icf_glb_pdb_st		    *p_glb_pdb,
		IN		icf_header_name_list_st	*p_src_header_name_list,
        OUT		icf_header_name_list_st	*p_dst_header_name_list,
		OUT		icf_error_t	    		*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_header_name_list
 *
 * DESCRIPTION:     This is a utility function that frees the header list. It is
 *					upto the calling function to free the input ptr if needed.
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_header_name_list(
		INOUT	icf_glb_pdb_st   		*p_glb_pdb,
        INOUT	icf_header_name_list_st	*p_header_list,
		OUT		icf_error_t		    	*p_ecode);
ICF_CRM_END

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_tag_list
 *
 * DESCRIPTION:     This function makes a copy of tag list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_tag_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN		icf_tag_list_st		*p_src_tag_list,
		OUT		icf_tag_list_st		*p_dst_tag_list,
		OUT		icf_error_t			*p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_body_list
 *
 * DESCRIPTION:     This function makes a copy of message body list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_body_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN		icf_msg_body_list_st	*p_src_body_list,
		OUT		icf_msg_body_list_st	*p_dst_body_list,
		OUT		icf_error_t			*p_ecode);

#ifdef ICF_REL_OLD
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_clone_stream_list
 *
 * DESCRIPTION:     This function makes a copy of stream list
 *
 ******************************************************************************/
icf_return_t icf_cmn_clone_stream_list(
		INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN		icf_stream_id_list_st	*p_src_stream_list,
		OUT		icf_stream_id_list_st	*p_dst_stream_list,
		OUT		icf_error_t			*p_ecode);
#endif

ICF_SESSION_CONTROL_END

ICF_CRM_START
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_get_uri_status
 * 
 * DESCRIPTION:     This function returns all associated URIs          
 *                  
 *                  
 *                   
 * RETURNS:         ICF_SUCCESS: In case URI status is found.       
 *					ICF_FAILURE: In case no URI status is found
 ******************************************************************************/
icf_return_t	
icf_rgm_get_uri_status (
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	IN		icf_address_st	*p_uri, 
	IN		icf_app_id_t		app_id, 
	INOUT	icf_uint8_t		*p_state,
	INOUT	icf_error_t		*p_ecode);

/****************************************************************************
 * FUNCTION:        icf_cfg_fetch_app_conf
 *
 * DESCRIPTION:     This function fetches icf_app_conf_st for a given app_id
 *
 * RETURNS:         ICF_FAILURE if no application configuration is found for 
 *                  a given app_id. p_app_conf is not to be refered in this case.
 *                  ICF_SUCCESS if application specific configuration is found. 
 *
 ***************************************************************************/
icf_return_t 
icf_cfg_fetch_app_conf(
   INOUT    icf_glb_pdb_st   *p_glb_pdb,
   IN       icf_app_id_t     app_id, 
   OUT      icf_app_conf_st  **p_p_app_conf,
   OUT      icf_error_t      *p_ecode);
/******************************************************************************
 *
 * FUNCTION:        icf_rgm_fetch_ctxt_for_uri
 * 
 * DESCRIPTION:     This function returns pointer to regn context for  
 *                  a user (uri). In case no regn context exists then
 *                  ICF_FAILURE is returned.
 *                   
 * RETURNS:         ICF_SUCCESS: In case regm ctxt is found.       
 *					ICF_FAILURE: In case no regm_ctxt is found.
 ******************************************************************************/

icf_return_t
icf_rgm_fetch_ctxt_for_uri(
        INOUT icf_glb_pdb_st         *p_glb_pdb,
		IN    icf_address_st         *p_rgm_uri,
        OUT   icf_rgm_context_st     **p_p_rgn_cntxt_blk,
        OUT   icf_error_t            *p_ecode);


/*******************************************************************************
 * FUNCTION:        icf_ssa_parse_regm_notify_body
 *
 * DESCRIPTION:     This function parse message body obtained
 *                  in notify message for reg event package
 *
 ******************************************************************************/
 icf_return_t icf_ssa_parse_regm_notify_body(
 INOUT   icf_uint32_t     *p_state,
 INOUT   icf_uint32_t     *p_event,
 INOUT   icf_int8_t       *pBuffer);

ICF_CRM_END

#ifdef ICF_NW_ACTIVATE
/***************************************************************************
 *
 * FUNCTION:        icf_cfg_get_network_status
 *
 * DESCRIPTION:     This fn can be called by any module to check the 
 *                  status of network associated with this app_id
 * 
 **************************************************************************/
icf_return_t icf_cfg_get_network_status(
                INOUT icf_glb_pdb_st    *p_glb_pdb,
                IN    icf_app_id_t 	  app_id);
#endif

/***************************************************************************
 *
 * FUNCTION:        icf_cfg_get_app_id_validity
 *
 * DESCRIPTION:     This fn can be called by any module to check the 
 *                  validity of given app_id
 * 
 **************************************************************************/
icf_return_t icf_cfg_get_app_id_validity(
                INOUT   icf_glb_pdb_st    *p_glb_pdb,
                IN   icf_app_id_t 	  app_id);


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_transport_addr_to_ip_string
 *
 * DESCRIPTION:     This function Converts IP string to Transport Address
 *                  Currently handles only for ICF_TRANSPORT_ADDRESS_IPV4_ADDR
 ******************************************************************************/
icf_return_t icf_cmn_convert_transport_addr_to_ip_string(
        IN icf_transport_addr_st *p_address,
		OUT icf_uint8_t *p_IP);


ICF_4_0_CRM_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_transport_addr_to_string
 *
 * DESCRIPTION:     This function Converts transport Address to a string
 *                  Currently handles only for ICF_TRANSPORT_ADDRESS_IPV4_ADDR
 *					And domain name
 ******************************************************************************/
icf_return_t icf_cmn_convert_transport_addr_to_string(
        IN icf_transport_addr_st *p_address,
		OUT icf_uint8_t *p_addr_string);

/******************************************************************************
 * 
 *              XML PARSER : Currently Used for Parsing RegEvent XML Body
 *
 *****************************************************************************/

 /****************************************************************************
 *          UTILITY Functions of XML parser
 ****************************************************************************/

/**************************************************************** 
** FUNCTION:  icf_cmn_skip_chars_forward
**
** DESCRIPTION: Skips caharcters till a matching pattern
**
***************************************************************/ 
icf_boolean_t icf_cmn_skip_chars_forward
                    	(IN    icf_int8_t* pInput,
	                     INOUT icf_int32_t* pPosition,
	                     IN    icf_int8_t* pPattern);


/***************************************************************
** FUNCTION:  icf_cmn_skip_spaces
**
** DESCRIPTION: This function skips spaces in given buffer and at a 
**				given position. It advances the pointer to the next 
**				non-space character.
***************************************************************/ 
icf_void_t icf_cmn_skip_spaces
	            (IN    icf_int8_t* pInput,
	             INOUT icf_int32_t* pPosition);

/***************************************************************
** FUNCTION:  icf_cmn_xml_parse_attribute
**
** DESCRIPTION: This function parse Attribute of the Node and 
**              calls the callback function towards the applcation
**              context
***************************************************************/ 
icf_return_t icf_cmn_xml_parse_attribute 
                     (IN icf_uint8_t  *pInput, 
                     INOUT icf_uint32_t *pPosition,
                     INOUT icf_uint8_t  *pNode,
                     INOUT icf_void_t   *p_app_ctxt,
                     INOUT icf_error_t  *pErr);


/***************************************************************
** FUNCTION:  icf_cmn_xml_parse_tag
**
** DESCRIPTION: This function parse Xml Tag and 
**              calls the callback function towards the applcation
**              context
***************************************************************/ 

icf_return_t icf_cmn_xml_parse_tag(IN icf_uint8_t	*pInput,
                                    INOUT icf_uint32_t   *pPosition,
                                    INOUT icf_void_t     *p_app_ctxt,
                                    INOUT icf_error_t    *p_ecode);


/***************************************************************
** FUNCTION:  icf_cmn_parse_xml_body
**
** DESCRIPTION: This function parse XML body and 
**              calls the callback function towards the applcation
**              context
***************************************************************/ 
icf_return_t icf_cmn_parse_xml_body(
                                    IN      icf_uint8_t		  *pbuf,
                                    IN      icf_uint32_t		  buf_length,
                                    INOUT   icf_void_t		  *p_reginfo,
                                    INOUT   icf_error_t	      *p_ecode);

/******************************************************************************
 * 
 *           END   XML PARSER : Currently Used for Parsing RegEvent XML Body
 *
 *****************************************************************************/

/******************************************************************
 *  Implementation Of call Back Function Of XML Parser for RegInfo
 ******************************************************************/

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_attribute
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides Attrbute and value of the attribute 
 *              for a Node
 *              
 *****************************************************************************/
icf_return_t icf_cmn_xmlparser_cbk_node_attribute(
									INOUT	icf_void_t  *p_app_ctxt,
									IN		icf_uint8_t *p_nodeName,
									IN		icf_uint8_t *p_attribute,
									IN		icf_uint8_t *p_Value,
									INOUT	icf_error_t *p_ecode);

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_tagdata
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides the TagData of a Node
 *              
 *****************************************************************************/
icf_return_t	icf_cmn_xmlparser_cbk_node_tagdata(
                                    INOUT icf_void_t     *p_app_ctxt,
									IN	  icf_uint8_t    *p_nodeName,
									IN	  icf_uint8_t    *p_tagData,
									INOUT icf_error_t     *p_ecode);

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_start
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides the indication for strating of parsing
 *              for a Node
 *****************************************************************************/
icf_return_t icf_cmn_xmlparser_cbk_node_start(
                                    INOUT  icf_void_t  *p_app_ctxt,
									IN	   icf_uint8_t *p_nodeName,
									INOUT  icf_error_t     *p_ecode);

/******************************************************************************
 * FUNCTION:  icf_cmn_xmlparser_cbk_node_end
 *
 * DESCRIPTION: This function is the call back function of XML Parser
 *              which provides the indication for ending of parsing
 *              for a Node
 *****************************************************************************/
icf_return_t icf_cmn_xmlparser_cbk_node_end(
                                    INOUT  icf_void_t  *p_app_ctxt,
									IN	   icf_uint8_t *p_nodeName,
									INOUT  icf_error_t *p_ecode);



/*******************************************************************************
 * FUNCTION:        icf_cmn_is_null_ip_address
 *
 * DESCRIPTION:     This function checks for NULL ip address
 *                  
 * RETURNS          ICF_SUCCESS: In case ip address is NULL
 *					ICF_FAILURE: In case ip address is not NULL
 ******************************************************************************/
icf_return_t icf_cmn_is_null_ip_address( 
IN		icf_transport_addr_st ip_address);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_make_request_digest
 *
 * DESCRIPTION:     This creates request digest for digest authentication
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_make_request_digest(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        IN      icf_www_authenticate_param_st *p_authenticate_params,
        INOUT   icf_authorisation_param_st   *p_authorization_params,
        IN	    icf_uint32_t   msg_body_length, 
        IN	    icf_uint8_t   *p_msg_body,
		OUT     icf_error_t    *p_ecode);


/******************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_trnspt_addr
 *
 * DESCRIPTION:     This function validate the transport address recevied
 *                  from PA for address type
 *
 *
 *****************************************************************************/
icf_return_t icf_cmn_validate_trnspt_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_called_party_addr
 *
 * DESCRIPTION:     This function validate the called party  address recevied
 *                  from PA for address type
 *
 *
 *****************************************************************************/
icf_return_t icf_cmn_validate_called_party_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode);


ICF_4_0_CRM_END

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_compare_transport_addresses
 *
 * DESCRIPTION:     This function compares two transport addresses and returns
 *                  success if they are same, else returns failure     
 *
 ******************************************************************************/
icf_return_t icf_cmn_compare_transport_addresses(
        IN      icf_transport_address_st    *p_address_1,
        IN      icf_transport_address_st    *p_address_2,
        INOUT   icf_glb_pdb_st              *p_glb_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_copy_transport_address
 *
 * DESCRIPTION:     This function copies transport address from p_address_2 
 *                  to p_address_1 and returns success if copy is successful, 
 *                  else returns failure     
 *
 ******************************************************************************/
icf_return_t icf_cmn_copy_transport_address(
        INOUT icf_transport_address_st    *p_address_1,
        IN    icf_transport_address_st    *p_address_2,
        INOUT icf_glb_pdb_st              *p_glb_pdb,
        INOUT icf_error_t                 *p_ecode);

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_string
 *
 * DESCRIPTION:    Initializes a structure of type icf_string_st 
 *
 *
 *****************************************************************************/
icf_void_t icf_cmn_init_string(
        INOUT icf_string_st *p_address);

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_transport_address
 *
 * DESCRIPTION:    Initializes a structure of type icf_transport_address_st 
 *
 *
 *****************************************************************************/
icf_void_t icf_cmn_init_transport_address(
        INOUT icf_transport_address_st *p_transport_address);

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_address
 *
 * DESCRIPTION:    Initializes a structure of type icf_address_st
 *
 *
 *****************************************************************************/
icf_void_t icf_cmn_init_address(
        INOUT icf_address_st *p_address);

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_check_if_param_present
 *
 * DESCRIPTION:    Function parses the given string for given parameter and
 *                 returns true if param present and value else false
 *
 *****************************************************************************/
icf_boolean_t icf_cmn_check_if_param_present(
                                                   icf_string_st  *p_addr,
                                                   icf_uint8_t    *p_param,
                                                   icf_uint8_t    *p_param_val,
                                                   icf_uint8_t    *p_hdr_name,
                                                   icf_tag_list_st   *p_tag_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_tokenize_string_list
 *
 * DESCRIPTION:     This function takes a string list as input and breaks up 
 *                  all the comma separated entries into single string nodes
 *
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cmn_tokenize_string_list(
                icf_glb_pdb_st       *p_glb_pdb,
                icf_string_list_st   *p_str_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_strlist_from_hdrlist
 *
 * DESCRIPTION:     This function takes a header list and a header name as input
 *                  and returns a string list corresponding to that header name 
 *                  from the header list. It also breaks up all the comma 
 *                  separated entries in the string list into single string 
 *                  nodes before returning it to the calling function.
 *                  CAUTION: In case a node corresponding to the input header 
 *                  name is found, this function will delete that node from the 
 *                  passed header list. This will be useful so that the same 
 *                  header is not inserted twice in SIP message, one from 
 *                  SIP profile and other while copying header list.
 *
 * RETURNS:         nothing 
 *
 ******************************************************************************/
icf_return_t icf_cmn_get_strlist_from_hdrlist(
                icf_glb_pdb_st       *p_glb_pdb,
                icf_header_list_st   *p_hdr_list,
                icf_uint8_t          *p_hdr_name,
                icf_string_list_st   **p_p_str_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_string_list
 *
 * DESCRIPTION:     This function frees a string list structure
 *
 * RETURNS:         ICF_SUCCESS 
 *
 ******************************************************************************/
icf_return_t icf_cmn_free_string_list(
                icf_glb_pdb_st	    *p_glb_pdb,
                icf_string_list_st   *p_str_list,
                icf_error_t          *p_ecode);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_apiProcFailure 
*
*  DESCRIPTION    :  This function is used to fill 
*					 pcor_failure_st structure for 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t     icf_cmn_fill_apiProcFailure
(
        INOUT	icf_glb_pdb_st			      *p_glb_pdb,
        INOUT	icf_void_t					  *p_recv_payload,
		INOUT	icf_api_proc_failure_st        *p_resp_payload,
        IN		icf_api_id_t					   api_id,    
        IN      icf_error_t					   error_cause
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_register_cfm 
*
*  DESCRIPTION    :  This function is used to fill 
*					 register_cfm structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t     icf_cmn_fill_register_cfm
(
        INOUT	icf_glb_pdb_st			      *p_glb_pdb,
        INOUT	icf_register_req_st			  *p_register_req,
		INOUT	icf_register_cfm_st		      *p_register_cfm,
        IN		icf_return_t					   result,    
        IN      icf_error_t					   error_cause
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_subs_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 subscription status ind structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_subs_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_subscribe_req_st			*p_subs_req,
		INOUT	icf_subscription_status_ind_st	*p_subs_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause);


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_message_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 icf_message_resp_st structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_message_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_message_req_st				*p_message_req,
		INOUT	icf_message_resp_st				*p_message_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_publish_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 publish_resp structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_publish_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_publish_req_st				*p_publish_req,
		INOUT	icf_publish_resp_st				*p_publish_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_refer_resp 
*
*  DESCRIPTION    :  This function is used to fill 
*					 refer_resp structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_refer_resp(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_refer_req_st				*p_refer_req,
		INOUT	icf_refer_resp_st				*p_refer_resp,
		IN		icf_return_t					result,
		IN		icf_error_t					error_cause);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_call_term_ind
*
*  DESCRIPTION    :  This function is used to fill 
*					 call_term_ind structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_call_term_ind(
		INOUT	icf_glb_pdb_st				*p_glb_pdb,
		INOUT	icf_terminate_call_ind_st	*p_term_call_ind,		
		IN		icf_call_clear_reason_t		clear_reason);


/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_create_conn_resp
*
*  DESCRIPTION    :  This function is used to fill 
*					 create_conn_resp structure
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_fill_create_conn_resp(
		INOUT	icf_glb_pdb_st			    	*p_glb_pdb,
		INOUT	icf_create_secure_conn_req_st	*p_create_conn_req,		
		INOUT   icf_create_secure_conn_resp_st	*p_create_conn_resp,
        IN      icf_result_t                    conn_req_status,
        IN      icf_error_t                     error_cause);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_chk_n_free_lists 
*
*  DESCRIPTION    :  This function is used to free the lists 
*                                        depending on the api for which the response
*                                        payload has to be freed.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t      icf_cmn_chk_n_free_lists(
                INOUT   icf_glb_pdb_st  *p_glb_pdb,
                INOUT   icf_msg_st              *p_msg,
                INOUT   icf_error_t             *p_ecode);


#ifdef ICF_NW_ACTIVATE

/*************************************************************
 *
 * FUNCTION:    icf_cmn_chk_gprs_status
 *
 * DESCRIPTION: This will be a utility function 
 *              offered by CFG towards other internal modules, CC,
 *              REGM and SIC.
 *              This function will be 
 *              invoked in all API handlers 
 *              wherein before creating the contexts , 
 *              this function will be invoked.
 *              The function parameter p_status will 
 *              contain the value ICF_FALSE if
 *              the GPRS state is suspended.
 *
 *  RETURNS:    Success/ Failure and the status of GPRS:
 *              False: Suspended; True: Active
 *
*******************************************************************/
icf_return_t	icf_cmn_chk_gprs_status(
            INOUT   icf_glb_pdb_st       *p_glb_pdb,
            INOUT   icf_boolean_t        *p_status,
            INOUT   icf_error_t          *p_ecode);

/*********************************************************************
 *
 * FUNCTION:        icf_cmn_chk_api_for_gprs
 *
 * DESCRIPTION:     This function returns TRUE if the API id is any
 *                  one of those that need not be processed if GPRS
 *                  is suspended
 *
 * RETURNS:         ICF_TRUE/ ICF_FALSE 
 *
 ************************************************************************/
icf_boolean_t icf_cmn_chk_api_for_gprs(
            INOUT   icf_glb_pdb_st       *p_glb_pdb,
            INOUT   icf_msg_st           *p_msg,
            INOUT   icf_error_t          *p_ecode);

/*******************************************************************
 *
 * FUNCTION:        icf_cmn_handle_gprs_fail
 *
 * DESCRIPTION:     This function returns send response to any API
 *                  that need not be processed if GPRS
 *                  is suspended. The status is FAILURE and error_cause
 *                  is ICF_CAUSE_GPRS_SUSPENDED.
 *
 * RETURNS:         ICF_SUCCESS/ ICF_FAILURE
 *
 ********************************************************************/
icf_return_t icf_cmn_handle_gprs_fail(
            INOUT   icf_glb_pdb_st       *p_glb_pdb,
            INOUT   icf_msg_st           *p_msg,
            INOUT   icf_error_t          *p_ecode);

#endif/* ICF_NW_ACTIVATE if defined */

            
/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_fill_api_resp 
*
*  DESCRIPTION    :  This function is used to fill common response structure 
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t     icf_cmn_fill_api_resp
(
        INOUT        icf_glb_pdb_st        *p_glb_pdb,
        INOUT        icf_msg_st            *p_msg,
        IN           icf_module_id_t       dest_id,
        IN           icf_api_id_t          resp_api_id,
        IN           icf_result_t          result,
        IN           icf_error_t           error_cause,
        INOUT        icf_error_t           *p_ecode
);

/* ICF REL 6.0: multiple m lines */
/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_internal_list_count 
*
*  DESCRIPTION    :  This function is used to get the count/size of the 
*					 list. 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_get_internal_list_count 
(
	INOUT	icf_internal_list_st	*p_list,
	INOUT	icf_uint8_t		*p_count
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_list_count 
*
*  DESCRIPTION    :  This function is used to get the count/size of the 
*					 list. 
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t	icf_cmn_get_list_count 
(
	INOUT	icf_list_st		*p_list,
	INOUT	icf_uint8_t		*p_count
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_add_node_to_internal_list 
*
*  DESCRIPTION    :  This function will be used to insert a node into the 
*					 list of the type icf_internal_list_st. The function will 
*					 always insert the node at the end of the list
*					 If the identifier is of the type icf_uint8_t/icf_uint16_t/
*					 icf_uint32_t, the calling function can pass the identifier
*					 and the comparison function ptr as NULL to this function.
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_add_node_to_internal_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	**p_list_start, 
			  /* incase we r adding the first node 
			   * in the list then we will be allocating
			   * memory for the node, that will be
			   * returned to the calling function
			   */
	INOUT	icf_void_t		*p_payload,
	INOUT	icf_void_t		*p_identifier,
	IN		icf_void_t		*p_func_ptr
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_add_node_to_list 
*
*  DESCRIPTION    :  This function will be used to insert a node into the 
*					 list of the type icf_list_st. The function will always
*					 insert the node at the end of the list
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_add_node_to_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_list_start, 
									  /* incase we r adding the first node 
									   * in the list then we will be allocating
									   * memory for the node, that will be
									   * returned to the calling function
									   */
	IN		icf_void_t		*p_payload
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_node_frm_internal_list 
*
*  DESCRIPTION    :  This function will be used to delete a node from the 
*					 list of the type icf_internal_list_st. The function will 
*					 return the pointer to the payload and also the identifier
*					 incase the identifier is not of the type icf_uint8_t/
*					 icf_uint16_t/icf_uint32_t, so that the calling 
*					 module can free that memory as the payload was allocated 
*					 by the calling module.
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t icf_cmn_delete_node_frm_internal_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	**p_list_start,
	INOUT	icf_void_t		**p_payload,
	INOUT	icf_void_t		**p_identifier
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_node_frm_list 
*
*  DESCRIPTION    :  This function will be used to delete a node from the 
*					 list of the type icf_list_st. The function will return
*					 the pointer to the payload, so that the calling module
*					 can free that memory as the payload was allocated mem.
*					 by the calling module.
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t icf_cmn_delete_node_frm_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_list_start,
	INOUT	icf_void_t		**p_payload
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_internal_list 
*
*  DESCRIPTION    :  This function will be used to delete all nodes in the 
*					 list of the type icf_internal_list_st. This function will 
*					 also delete the payload in the list, based on the pool_id
*					 passed by the calling function/module
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_delete_internal_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	**p_list_start,
	IN		icf_pool_id_t	pool_id
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_list 
*
*  DESCRIPTION    :  This function will be used to delete all nodes in the 
*					 list of the type icf_list_st. This function will also
*					 delete the payload in the list, based on the pool_id
*					 passed by the calling function/module
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_delete_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_list_start,
	IN		icf_pool_id_t	pool_id
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_node_in_internal_list 
*
*  DESCRIPTION    :  This function will return a node from the list that 
*					 matches the identifier in the list node in a  
*					 list of the type icf_internal_list_st.
*
*  RETURNS        :
*
*************************************************************************/
icf_internal_list_st*	icf_cmn_get_node_in_internal_list 
(
	INOUT	icf_internal_list_st	*p_list_start,
	IN		icf_void_t				*p_identifier
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_replace_node_in_internal_list 
*
*  DESCRIPTION    :  This function will be used to replace an existing node 
*  					 with a new node. the calling function will give the new
*  					 node to be inserted in the list. Also the calling funct.
*  					 will delete the old node, as the calling function had
*  					 allocated memory for it. The old payload addrs will be 
*  					 returned in the p_old_payload ptr.
*  					 NOTE: Incase the calling function is passing a valid addr
*  					 in the old_payload ptr, at the time of calling this funct
*  					 it shud pass the p_identifier as ICF_NULL
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_replace_node_in_internal_list 
(
	INOUT	icf_internal_list_st	*p_list_start,
	INOUT	icf_void_t		**p_old_payload,
	INOUT	icf_void_t		*p_rep_payload,
	IN		icf_void_t		*p_identifier
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_next_node_in_internal_list 
*
*  DESCRIPTION    :  This function will be used to fetch the next node in the
*  					 internal list. The calling function will pass the payload of the
*  					 current node. This function will return the pointer to the
*  					 payload of the next node.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t*	icf_cmn_get_next_node_in_internal_list 
(
	INOUT	icf_internal_list_st	*p_list_start,
	IN		icf_void_t		*p_payload /* current payload */
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_next_node_in_list 
*
*  DESCRIPTION    :  This function will be used to fetch the next node in the
*  					 list. The calling function will pass the payload of the
*  					 current node. This function will return the pointer to the
*  					 payload of the next node.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t*	icf_cmn_get_next_node_in_list 
(
	INOUT	icf_list_st	*p_list_start,
	IN		icf_void_t		*p_payload /* current payload */
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_set_identifier_in_node
*
*  DESCRIPTION    :  This function will allocate memory for the identifier
*					 and then save the identifier in the node as per the 
*					 type of the identifier.
*
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_set_identifier_in_node
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	*p_tmp_node,
	IN		icf_void_t		*p_identifier
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_compare_int_identifier
*
*  DESCRIPTION    :  This utility function will compare with received 
*					 identifier with the identifier saved within the node. 
*					 This is a default comparison function to compare interger
*					 type identifier's
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_compare_int_identifier
(		
	INOUT	icf_void_t	*p_tmp_node,
	INOUT	icf_void_t				*p_identifier
);


/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_media_sdp_info_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_inc_media_sdp_info_st
 **
 *****************************************************************************/
icf_return_t icf_cmn_free_media_sdp_info_list(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_list_st        **p_p_media_sdp_info,
    OUT     icf_error_t                 *p_ecode);


/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_media_sdp_info_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_inc_media_sdp_info_st
 **
 *****************************************************************************/
icf_return_t  icf_cmn_delete_inc_media_sdp_info_list(
	icf_glb_pdb_st	*p_glb_pdb,
    icf_list_st	    **p_p_inc_media_sdp,
	icf_error_t     *p_ecode
);


/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_stream_sdp_info_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_stream_sdp_info_st
 **
 *****************************************************************************/
icf_return_t icf_cmn_free_stream_sdp_info_list(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_internal_list_st	    **p_p_stream_sdp_info,
    OUT     icf_error_t                 *p_ecode);

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_cc_fsm_func_ptr_t
 * Description      : CC's function pointer table type
 *
 *----------------------------------------------------------------------------*/
typedef icf_return_t (*icf_cmn_func_ptr_t)(
        INOUT icf_glb_pdb_st *p_glb_pdb );

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_sdp_in_list_for_stream_id
 * 
 * DESCRIPTION:     This is a utility function that will be used to fetch the
 * 					SDP node from within the CC's icf_sdp_stream_info_st 
 * 					list for the given stream_id. The type of node returned 
 * 					in the function depends on the SDP_TYPE passed as an 
 * 					argument to this function
 *
 *****************************************************************************/
icf_return_t	icf_cmn_fetch_sdp_in_list_for_stream_id
(
	INOUT icf_glb_pdb_st				*p_glb_pdb,
	IN	  icf_stream_id_t				stream_id,
	IN	  icf_uint8_t					sdp_type,
	INOUT icf_internal_sdp_stream_st	**p_p_dest_sdp
);

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_update_codecs_for_stream_id_in_call_ctxt
 * 
 * DESCRIPTION:     This is a utility function that will copy the contents of
 * 					source sdp (for a given stream_id) into the designated
 * 					SDP node in the CC call context. 
 * 					This function will only copy the contents of 
 * 					icf_codec_attrib_st into the destination
 *
 *****************************************************************************/
icf_return_t	icf_cmn_update_codecs_for_stream_id_in_call_ctxt
(
	INOUT icf_glb_pdb_st		*p_glb_pdb,
	IN	  icf_stream_id_t		stream_id,
	IN	  icf_uint8_t			sdp_type,
	INOUT icf_sdp_stream_st		*p_src_sdp
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_delete_codec_attrib_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_string_st
 **
 *****************************************************************************/
icf_return_t icf_cmn_delete_codec_attrib_list
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_list_st		**p_p_codec_attrib,
	INOUT	icf_error_t    	*p_ecode
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_codec_attrib_list
 **
 ** DESCRIPTION:  This function clones the icf_codec_attrib_st from the
 **				  source to the destination sdp_stream
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_codec_attrib_list
(
 	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_list_st 	**p_p_dest_codec_attrib,
	INOUT	icf_list_st			*p_src_codec_attrib
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_codec_attrib_list_count
*
*  DESCRIPTION    :  This function is used to get the count/size of the
*                    codec attrib list, excluding the telephone-event.
*
*************************************************************************/
icf_void_t  icf_cmn_get_codec_attrib_list_count
(
    INOUT   icf_list_st     *p_list,
    INOUT   icf_uint8_t     *p_count
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_update_media_params_for_stream_id_in_call_ctxt
 **
 ** DESCRIPTION:  This function clones the parameters of icf_sdp_stream_st
 **				  minus the codec_attribute_st. the codec parameters are
 **	              seperately in another function. The function also fetches
 **				  the node corresponding to the given stream_id from the
 **				  icf_sdp_stream_info_st in the call_ctxt
 **
 *****************************************************************************/
icf_return_t	icf_cmn_update_media_params_for_stream_id_in_call_ctxt
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_uint8_t			sdp_type,
	INOUT	icf_sdp_stream_st	*p_src_sdp
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_sdp_stream_params
 **
 ** DESCRIPTION:  This function clones the parameters of icf_sdp_stream_st
 **				  minus the codec_attribute_st. the codec parameters are
 **	              seperately in another function.
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_sdp_stream_params
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
    IN		icf_uint8_t			sdp_type,
	INOUT	icf_sdp_stream_st	*p_dest_sdp_stream,
	INOUT	icf_sdp_stream_st	*p_src_sdp_stream
);


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_copy_string_list
 *
 * DESCRIPTION:     This function makes a copy of list of type icf_string_st.
 *
 ******************************************************************************/
icf_return_t icf_cmn_copy_string_list
(
	icf_glb_pdb_st	*p_glb_pdb,
	icf_list_st		**p_p_string_list_new,
	icf_list_st		*p_string_list_old,
	icf_error_t     *p_ecode
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_delete_internal_sdp_stream
 **
 ** DESCRIPTION:  This function frees a list of type icf_string_st

  THIS FUNCTION:	will be freeing the memory assigned to each pointer 
  * 				inside the icf_internal_stream_sdp and also within those pointers if 
  * 				any pointers are present those will also be freed and so on.
  *
  *					In icf_internal_ sdp_stream, we don't have any pointer to be freed. 
  *	
  *					Within icf_internal_stream_sdp there is variable of type 
  * 				icf_stream_sdp, we need to go in it and free the memory assigned 
  * 				to pointer p_codec_info_list by calling some function written for it.
  *
  *					And also within icf_sdp_stream, there is pointer 
  * 				p_encoded_parameters which needs to be freed by calling some 
  * 				function written for it.
 **
 *****************************************************************************/

icf_return_t icf_cmn_delete_internal_sdp_stream
(
	icf_glb_pdb_st *p_glb_pdb,
	icf_internal_sdp_stream_st **p_p_internal_sdp_stream,
	icf_error_t                 *p_ecode
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_node_from_stream_capab_list
 *
 * DESCRIPTION:     This function will return pointer node of list of type
 *					icf_stream_capab_st, if corresponding matching stream type
 *					is found in the list.
******************************************************************************/
icf_list_st* icf_cmn_get_node_from_stream_capab_list
(
	icf_list_st			*p_list_node,
	icf_media_type_t	stream_type
);


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_copy_string_list
 *
 * DESCRIPTION:     This function makes a copy of list of type icf_string_st.
 *
 ******************************************************************************/
icf_return_t icf_cmn_copy_string_list
(
	icf_glb_pdb_st	*p_glb_pdb,
	icf_list_st		**p_p_string_list_new,
	icf_list_st		*p_string_list_old,
	icf_error_t     *p_ecode
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_delete_internal_sdp_stream
 **
 ** DESCRIPTION:  This function frees a memory of type icf_internal_sdp_stream_st
 *****************************************************************************/

icf_return_t icf_cmn_delete_internal_sdp_stream
(
	icf_glb_pdb_st *p_glb_pdb,
	icf_internal_sdp_stream_st **p_p_internal_sdp_stream,
	icf_error_t                 *p_ecode
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_node_from_stream_capab_list
 *
 * DESCRIPTION:     This function will return pointer node of list of type
 *					icf_stream_capab_st, if corresponding matching stream type
 *					is found in the list.
******************************************************************************/
icf_list_st* icf_cmn_get_node_from_stream_capab_list
(
	icf_list_st			*p_list_node,
	icf_media_type_t	stream_type
);


/*****************************************************************************
 ** FUNCTION:     icf_cmn_delete_codec_attrib_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_string_st
 **
 *****************************************************************************/
icf_return_t icf_cmn_delete_codec_attrib_list
(
				icf_glb_pdb_st	*p_glb_pdb,
				icf_list_st		**p_p_codec_attrib,
				icf_error_t     *p_ecode
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_delete_internal_list 
*
*  DESCRIPTION    :  This function will be used to delete all nodes in the 
*					 list of the type icf_internal_list_st. This function will 
*					 also delete the payload in the list, based on the pool_id
*					 passed by the calling function/module
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	icf_cmn_delete_internal_list 
(
	INOUT	icf_glb_pdb_st	*p_glb_pdb,
	INOUT	icf_internal_list_st	**p_list_start,
	IN		icf_pool_id_t	pool_id
);

/*************************************************************************
*
*  FUNCTION NAME  :  icf_cmn_get_next_node_in_internal_list 
*
*  DESCRIPTION    :  This function will be used to fetch the next node in the
*  					 list. The calling function will pass the payload of the
*  					 current node. This function will return the pointer to the
*  					 payload of the next node.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t*	icf_cmn_get_next_node_in_internal_list 
(
	INOUT	icf_internal_list_st	*p_list_start,
	IN		icf_void_t		*p_payload /* current payload */
);

/*******************************************************************************
 * FUNCTION:        icf_cmn_free_stream_capab_list
 *
 * DESCRIPTION:     Function to free list of stream_capab_st.
 *
 ******************************************************************************/

icf_return_t icf_cmn_free_stream_capab_list
(
	INOUT icf_glb_pdb_st         	*p_glb_pdb,
	INOUT icf_internal_list_st 		*p_stream_list
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_media_sdp_info_internal_list
 **
 ** DESCRIPTION:  This function frees remote_recvd sdp in mmi ctxt
 **
 *****************************************************************************/
icf_return_t icf_cmn_free_media_sdp_info_internal_list
(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_internal_list_st        **p_p_media_sdp_info,
    OUT     icf_error_t                 *p_ecode
);


/* ICF_6_1_OPTIONS_SUPPORT start */
/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_stream_capab_list
 **
 ** DESCRIPTION:  This function clones the parameters of icf_stream_capab_st
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_stream_capab_list
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_list_st	**p_dest_stream_capab_list,
	INOUT	icf_list_st	*p_src_stream_capab_list
);


/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_icf_stream_capab_list
 **
 ** DESCRIPTION:  This function frees the parameters of icf_stream_capab_st
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_cmn_free_icf_stream_capab_list
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_list_st			**p_stream_capab_list
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_free_icf_stream_capab_node
 **
 ** DESCRIPTION:  This function frees the parameters of icf_stream_capab_st
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_cmn_free_icf_stream_capab_node
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_stream_capab_st **p_stream_capab_node
);

/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_stream_capab
 **
 ** DESCRIPTION:  This function clones the parameters of icf_stream_capab_st
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_cmn_clone_stream_capab
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_stream_capab_st	**p_dest_stream_capab,
	INOUT	icf_stream_capab_st	*p_src_stream_capab
);
/* ICF_6_1_OPTIONS_SUPPORT end */

#if defined (ICF_PORT_WINDOWS) || defined (ICF_PORT_LINUX)
char * self_implemented_strtok_r(char *s, const char *delim, char **last);
#endif

/***************************************************************
 ** FUNCTION:  icf_cmn_get_valid_node
 **
 ** DESCRIPTION: This function advances the pointer to the next 
 **     non-space character, and put the word to the output
 **************************************************************/
icf_return_t icf_cmn_get_valid_node
(IN    icf_int8_t*  pInput,
 INOUT icf_int32_t* pPosition,
 INOUT icf_int8_t*  pOutPut);

#ifdef ICF_NAT_MEDIA_TRAVERSAL

/******************************************************************************
 *
 * FUNCTION:    icf_cmn_fetch_stream_id_from_nat_context_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_stream_id_t	icf_cmn_fetch_stream_id_from_nat_context_id
(
	icf_uint32_t	context_id
);


/******************************************************************************
 *
 * FUNCTION:    icf_cmn_fetch_call_id_from_nat_context_id
 *
 * DESCRIPTION: 
 *****************************************************************************/

icf_call_id_t	icf_cmn_fetch_call_id_from_nat_context_id
(
	icf_uint32_t	context_id
);

/******************************************************************************
 *
 * FUNCTION:    icf_mmi_fetch_req_type_from_nat_context_id
 *
 * DESCRIPTION: Function to send the nat request type : RTP/UDP or RTCP from
 *              context id.
 *****************************************************************************/

icf_uint8_t	icf_cmn_fetch_req_type_from_nat_context_id
(
	icf_uint32_t	context_id
);

#endif

/*SPR 13978 Aman 03-01-07*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_open_nw_server
 *
 * DESCRIPTION:     This fucntion checks that whether the sockets are open or 
 *                  not.It checks that if the call is on UDP, then UDP socket 
 *                  should be opened or if the call is on TCP, then TCP socket
 *                  should be opened.   
 *                  
 ******************************************************************************/
icf_return_t icf_cmn_validate_open_nw_server(
        INOUT   icf_glb_pdb_st *p_glb_pdb,
        INOUT   icf_line_id_t  line_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_send_pa_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is used to send an unexpected
 *                  event indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cmn_send_pa_unexpected_msg_ind(
              INOUT   icf_glb_pdb_st       *p_glb_pdb,
              INOUT   icf_msg_st           *p_msg,
              INOUT   icf_error_t          *p_ecode );

#ifdef ICF_LOAD_STAT
/*******************************************************************************
 * FUNCTION:        icf_cmn_print_load_stats
 * DESCRIPTION:      
 *                  
 ******************************************************************************/
icf_void_t icf_cmn_print_load_stats
(
    INOUT   icf_glb_pdb_st *p_glb_pdb
);
#endif


/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_deinit_dns_buffer
 ** DESCRIPTION:  This function will be registered by the DNS delete all the
 **               Entreis in the DNS buffer and the DNS purge timer will be stopped
 **               automatically
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_deinit_dns_buffer(
                        INOUT   icf_glb_pdb_st   *p_glb_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_send_mmi_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is used to send an unexpected
 *                  event indication to the MMI
 *
 *
 ******************************************************************************/
icf_return_t icf_cmn_send_mmi_unexpected_msg_ind(
              INOUT   icf_glb_pdb_st       *p_glb_pdb,
              INOUT   icf_msg_st           *p_msg,
              INOUT   icf_error_t          *p_ecode );

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_sip_addr
 *
 *
 *****************************************************************************/
icf_return_t icf_cmn_validate_sip_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode);

/******************************************************************************
 *
 * FUNCTION:        Interface Trace Prototypes
 *
 *
 *****************************************************************************/

        icf_return_t icf_cmn_trace_msg(icf_glb_pdb_st *p_glb_pdb,icf_error_t *p_ecode);
        icf_uint8_t* icf_cmn_trace_common_msg(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_no_payload_msg(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_resp_msg(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_api_header(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_codec_list(icf_codec_attrib_st* p_codec_list, icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_set_scale_params(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_trans_params(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_call_params(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_self_id_params(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_reg_params(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_config_params_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_service_params(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_incoming_call(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_create_call(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_alert_user(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_user_alerted(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_term_call_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_term_call_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_display_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_call_log_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_call_redirection_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_early_inc_call_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_early_inc_call_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_conference_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_ctu_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_cta_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_string_list(icf_string_list_st *p_str_list,icf_uint8_t *p_list_name, icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_rem_call_transfer_init(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_call_merged_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_party_replaced_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t*    icf_cmn_trace_get_codec_list_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t*    icf_cmn_trace_gcl_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_rtp_resp_msg(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_rtp_recv_req_msg(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_rem_call_transfer_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_unlock_codec_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t*    icf_cmn_trace_get_codec_list_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_codec_commit_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_cms_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_dms_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_merge_media_req_msg(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_set_trace_level(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_error_level(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_get_stats(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_stats(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_lock_codec_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_gcl_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_lock_codec_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_unlock_codec_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_cms_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_mms_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_rtp_err_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_rm_err_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_return_t icf_cmn_trace_api_from_api_id(icf_glb_pdb_st *p_glb_pdb,icf_msg_st *p_msg,icf_error_t *p_ecode);
        icf_uint8_t* icf_cmn_trace_msg_hexdump(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_set_line_param_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_config_params_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_media_capabilities_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_error_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_trace_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_get_stats_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t*  icf_cmn_trace_error_details(int p, icf_uint8_t* p_sprintf_count);
#ifdef ICF_TRACE_ENABLE
	icf_uint8_t* icf_cmn_trace_trace_data(icf_trace_data_st* p_trace_data, icf_uint8_t* p_sprintf_count);
#endif
#ifdef ICF_ERROR_ENABLE
       	icf_uint8_t* icf_cmn_trace_error_data(icf_error_data_st* p_error_data, icf_uint8_t* p_sprintf_count);
#endif
#ifdef ICF_STATS_ENABLE
	icf_uint8_t* icf_cmn_trace_stats_data(icf_stats_st* p_stats, icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_tcp_connections_stats(icf_tcp_connections_stats_st* p_tcp_con, icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_udp_connections_stats(icf_udp_connections_stats_st* p_udp_con, icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_tls_connections_stats(icf_tls_connections_stats_st* p_tls_con, icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_register_stats(icf_register_stats_st* p_reg_stat, icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_request_msg_stats(icf_request_message_stats_st* p_req_stat, icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_resp_msg_stats(icf_respone_message_stats_st* p_resp_stat, icf_uint8_t* p_sprintf_count);
	icf_uint8_t* icf_cmn_trace_misc_stats(icf_misc_stats_st* p_misc_stat, icf_uint8_t* p_sprintf_count);
#endif

        icf_uint8_t* icf_cmn_trace_enable_conf_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_disable_conf_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_enable_conf_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_disable_conf_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_pub_uri_list(icf_pub_uri_list_node_st *p_list,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_sec_pdp_activate_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_pdp_deactivate_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_platform_event_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_pdp_modify_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_pdp_deactivate_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_platform_event_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_icf_address_st(icf_address_st *p_uri,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_start_fax_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_start_fax_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_fax_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_start_fax_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_fax_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_remote_video_diaplay_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_preview_video_diaplay_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_connect_media_device_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_mms_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_codec_commit_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_cfg_media_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_play_media_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_media_play_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_media_play_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_vibration_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_echo_canc_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_vad_ss_cng_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_vibration_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_sec_pdp_activate_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_pdp_deactivate_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_pdp_deactivate_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);


        icf_uint8_t* icf_cmn_trace_transport_addr(icf_transport_addr_st* p_trans, icf_uint8_t* p_sprintf_count);
        
		/* Fix for CSR 1-6214356 : SPR 18849 */
/******************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_route_info
 *
 * DESCRIPTION:     This function is used to trace route info structure.
 *
 *****************************************************************************/
        icf_uint8_t* icf_cmn_trace_route_info(
                icf_route_info_st   *p_rt_info,
                icf_uint8_t         *p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_header_list(icf_header_st* p_hdr,
                                          icf_int32_t hdr_count, icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_tag_list(icf_tag_st* p_tag,
                                       icf_int32_t tag_count,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_body_list(icf_msg_body_st* p_body,
                                        icf_int32_t body_count,icf_uint8_t* p_sprintf_count);
        #ifdef ICF_REL_OLD
        icf_uint8_t* icf_cmn_trace_stream_list(icf_stream_sdp_info_st* p_stream,
                                          icf_uint8_t stream_count, icf_uint8_t* p_sprintf_count);
        #endif
        icf_uint8_t* icf_cmn_trace_header_name_list(icf_header_name_st* p_hdr,
                                               icf_uint8_t hdr_count,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_media_connected(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_connect_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_connect_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_connect_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);


         icf_uint8_t* icf_cmn_trace_get_sys_settings(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_get_line_settings(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_get_line_settings_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_get_sys_settings_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        /*ICF REL 6.0 */
        icf_uint8_t* icf_cmn_trace_audio_codec_param(icf_uint8_t *p_audio_param,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_video_codec_param(icf_uint8_t *p_video_param,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_t38_attrib_param(icf_uint8_t *p_t38_param,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_audio_profile(icf_uint8_t *p_audio_profile,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_video_profile(icf_uint8_t *p_video_profile,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stream_capab(icf_stream_capab_st* p_stream_capab_st,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stream_sdp(icf_sdp_stream_st* p_sdp_stream_st,icf_media_type_t stream_type,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_resp_stream(icf_resp_stream_st* p_resp_stream_st, icf_uint8_t i,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_modify_media_change_req(icf_call_modify_media_data_st* p_modify_media_change_req,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_app_add_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
         icf_uint8_t* icf_cmn_trace_app_add_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
         icf_uint8_t* icf_cmn_trace_app_remove_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_app_remove_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_network_activate_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_event_indication(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_network_activate_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_network_deactivate_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_network_deactivate_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_register_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_register_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_register_status_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_aka_auth_resp(icf_msg_st *p_msg,icf_uint8_t *p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_subscribe_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_subscribe_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_notify_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_publish_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_publish_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_message_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_message_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_message_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_codec_info_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_ringer_tones_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_jbm_mgmt_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_max_earp_vol_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_max_speaker_vol_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_port_range_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_start_ringing_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
#ifdef ICF_REL_OLD
        icf_uint8_t* icf_cmn_trace_media_session_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
#endif
        icf_uint8_t* icf_cmn_trace_get_stats_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_ringing_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
#ifdef ICF_REL_OLD
        icf_uint8_t* icf_cmn_trace_mute_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_play_media_spkr_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
#endif
        icf_uint8_t* icf_cmn_trace_set_earp_volume_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_speaker_volume_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_send_app_data_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_send_stats_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_stats_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_stop_ringing_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_rcv_app_data_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_codec_info_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_ringer_tones_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_jbm_mgmt_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_max_earp_vol_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_max_speaker_vol_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_port_range_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_ringing_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_media_session_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_get_stats_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_ringing_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_mute_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_play_media_spkr_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_earp_volume_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_set_speaker_volume_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_send_app_data_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_send_stats_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_stop_stats_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_nw_buff_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_med_cap_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_rtp_start_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_max_mic_gain_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_mic_gain_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_max_mic_gain_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_mic_gain_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_max_mic_gain_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_mic_gain_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_mm_msg_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_refer_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_refer_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

        icf_uint8_t* icf_cmn_trace_options_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_options_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_options_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_options_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

/* ICF_HANDLE_REINVITE_START */
        icf_uint8_t* icf_cmn_trace_incoming_call_modify_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_call_modify_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_incoming_call_modify_cancel_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_call_modify_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
        icf_uint8_t* icf_cmn_trace_call_modify_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
/* ICF_HANDLE_REINVITE_END  */


icf_uint8_t* icf_cmn_trace_create_secure_conn_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_close_secure_conn_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_set_close_secure_conn_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_secure_conn_status_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_mgmt_plane_api_proc_failure(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_set_mgmt_plane_nw_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_info_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_info_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_chd_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_crm_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

#ifdef ICF_STAT_ENABLED
icf_uint8_t* icf_cmn_trace_get_icf_stats_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_get_icf_stats_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
#endif

icf_uint8_t* icf_cmn_trace_api_resp_msg(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_ctu_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_cta_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_api_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_open_channel_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_open_media_channel_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_close_media_channel_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_send_udp_data_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_recv_udp_data_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_media_record_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_pause_media_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_set_media_playback_device_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_mute_media_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_stop_media_record_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_dgts_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_self_ip_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_digit_trans_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_stop_vibration_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_send_dgts_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_send_dgts_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_call_fwd_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_dms_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_info_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_notify_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_info_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_rem_call_transfer_init_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_nw_status(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_api_string(icf_api_id_t api_id,icf_uint8_t* p_sprintf_count);

icf_uint8_t* icf_cmn_trace_info_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_call_modify_cancel_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_progress_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_set_trace_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_report_trace_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_set_error_report_level_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_report_error_ind(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_stats_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_stats_resp(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_configure_cfm(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);
icf_uint8_t* icf_cmn_trace_media_connect_req(icf_msg_st* p_msg,icf_uint8_t* p_sprintf_count);


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_node_from_stream_data_list
 *
 * DESCRIPTION:     This function will return pointer node of list of type
 *                                      icf_stream_data_st, if corresponding matching stream type
 *                                      is found in the list.
******************************************************************************/
icf_stream_data_st* icf_cmn_get_node_from_stream_data_list
(
        icf_list_st                     *p_list_node,
        icf_media_type_t        stream_type
);

/*********** Changes for Retry-After Start (Rel7.0) ***********/

/******************************************************************************
*
* FUNCTION NAME    : icf_cmn_generate_timer
*
* DESCRIPTION  : This function is used to generate random timer
*
* RETURNS:
******************************************************************************/

icf_return_t icf_cmn_generate_timer(
        INOUT icf_duration_t     *p_timer_val,
        IN    icf_uint16_t     st_val,
        IN    icf_uint16_t     end_val
);

/*********** Changes for Retry-After End (Rel7.0) ***********/

/******** Changes for Forking and Multiple Answer Start (Rel7.0) *************/

/*************************************************************************
 *
 *  FUNCTION NAME  :  icf_cmn_get_node_in_list 
 *
 *  DESCRIPTION    :  This function will return a node from the list that 
 *		     matches the identifier in the list node.The function 
 *                    returns the node in the list
 *
 *************************************************************************/

icf_list_st*	icf_cmn_get_node_in_list 
(
	INOUT	icf_list_st	*p_list_start,
	IN	icf_uint8_t	identifier
);

/*********** Changes for Forking and Multiple Answer End (Rel7.0) *************/

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_check_if_auth_params_present
 *
 * Description:		This function return success if the user/line combination
 *					has auth key and auth password present.
 ******************************************************************************/
icf_return_t icf_cmn_check_if_auth_params_present
(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st *p_addr,
		IN	  icf_line_id_t  line_id);
       

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_get_auth_params
 *
 * Description:		This function returns the auth params that are configured
 *					in user db or line depending on the PA specification		
 ******************************************************************************/
icf_return_t icf_cmn_get_auth_params
(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st *p_addr,
		IN	  icf_line_id_t  line_id,
		INOUT icf_string_st  *p_key,
		INOUT icf_string_st  *p_passwd);
       

/******************************************************************************
 *
 * FUNCTION:        icf_cmn_parse_transport_addr
 *
 * DESCRIPTION:     This function parses the transport address recevied
 *                  from PA and determines its address type.
 *                  Validation Critteria:
 *                  
 *                  E164: An E164 number may or maynot start with '+' and must 
 *                  have only hexadecimal digits in it 
 *
 *                  Tel URI: A Tel URI may or may not start with '+' and must 
 *                  have only hexadecimal numbers with atleast on '-' seperator
 *
 *                  SIP URI: A number which is niether a tel uri or tel uri 
 *                  would be SIP URI if it has domain seperated by '@'
 *
 *
 *****************************************************************************/
icf_return_t icf_cmn_parse_transport_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,    
        INOUT icf_address_st* p_addr,
        OUT   icf_error_t    *p_ecode);

/********* Traces Add Start as per SPR 17213 **********/

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_mm_unexpected_msg_ind
 *
 * DESCRIPTION:     Print mm unexpected message indication
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_mm_unexpected_msg_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_get_register_status_resp
 *
 * DESCRIPTION:     Print register status response
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_get_register_status_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_get_register_status_req
 *
 * DESCRIPTION:     Print register status request
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_get_register_status_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_remote_call_transfer_initiated_cfm
 *
 * DESCRIPTION:     Print remote call transfer initiated confirmation
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_remote_call_transfer_initiated_cfm(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_unexpected_msg_ind
 *
 * DESCRIPTION:     Print unexpected message indication
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_unexpected_msg_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_early_incoming_call_cfm
 *
 * DESCRIPTION:     Print early incoming call confirmation
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_early_incoming_call_cfm(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_early_incoming_call_ind
 *
 * DESCRIPTION:     Print early incoming call indication
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_early_incoming_call_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_xconf_release_ind
 *
 * DESCRIPTION:     Print xconf release indication
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_xconf_release_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_delete_xconf_party_resp
 *
 * DESCRIPTION:     Print delete xconf party response messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_delete_xconf_party_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_add_xconf_party_resp
 *
 * DESCRIPTION:     Print add xconf party response messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_add_xconf_party_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_create_ext_xconf_resp
 *
 * DESCRIPTION:     Print create ext xconf response messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_create_ext_xconf_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_release_xconf_resp
 *
 * DESCRIPTION:     Print release xconf response messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_release_xconf_resp(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_release_xconf_req
 *
 * DESCRIPTION:     Print release xconf request messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_release_xconf_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_delete_xconf_party_req
 *
 * DESCRIPTION:     Print delete xconf party request messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_delete_xconf_party_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_add_xconf_party_req
 *
 * DESCRIPTION:     Print add xconf party request messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_add_xconf_party_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/*****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_create_ext_xconf_req
 *
 * DESCRIPTION:     Print create xconf request messages
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_create_ext_xconf_req(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/********* Traces Add End as per SPR 17213 **********/

#ifdef ICF_TRACE_ENABLE

/**********************************************************************
 * FUNCTION:    icf_cmn_check_app_trace
 *
 * DESCRIPTION: This function is called by the function icf_cmn_trace
 *              to check whether the trace criteria matches and the type
 *              requested is enabled and is to be reported or not.
 *
 * RETURNS:     ICF_SUCCESS to indicate that the received trace_id
 *              can be reported.
 *              ICF_FAILURE to indicate:
 *                  -Received trace_id is INAVLID
 *                  -DBM failure to obtain SM Global data.
 *                  -Received trace_id cannot be reported.
 *
 ******************************************************/
 icf_return_t icf_cmn_check_app_trace(
        INOUT    icf_glb_pdb_st *p_glb_pdb,
        IN       icf_trace_id_t trace_id,
        IN       icf_trace_criteria_data_st  *p_criteria_data);
#endif

/*****************************************************************************
 ** FUNCTION:     icf_cmn_clone_media_sdp_info_list
 **
 ** DESCRIPTION:  The function clones  p_p_src_media_sdp_info to 
 **               p_p_target_media_sdp_info
 **
 *****************************************************************************/
icf_return_t icf_cmn_clone_media_sdp_info_list(
    INOUT   icf_glb_pdb_st              *p_glb_pdb,
    INOUT   icf_list_st	                **p_p_target_media_sdp_info,
    INOUT   icf_list_st	                *p_src_media_sdp_info,
    OUT     icf_error_t                 *p_ecode);

/**************************************************************************
 *
 * FUNCTION:        icf_cmn_process_aka_challenge
 * 
 * DESCRIPTION:     This function is a utility function which initiates the
 *                  AKA challenge processing.It receives as input,the auth data
 *                  received from network.It operates on it and populates
 *                  auth data sent.It also sets appropriate error codes
 *                  on the basis of which the calling function can take
 *                  decisions.
 *                   
 * RETURNS:         ICF_SUCCESS/ICF_FAILUE.
 *************************************************************************/
icf_return_t icf_cmn_process_aka_challenge(
	INOUT  icf_glb_pdb_st           *p_glb_pdb,
	INOUT  icf_cmn_auth_data_st     *p_auth_data_recd,
	INOUT  icf_cmn_auth_data_st     *p_auth_data_sent,
    INOUT  icf_address_st           *p_addr,
	IN	   icf_line_id_t            line_id,
    IN     icf_app_id_t             app_id,
    INOUT  icf_large_string_st      *p_security_client,
    INOUT  icf_large_string_st      *p_security_verify,
    INOUT  icf_large_string_st      *p_security_server,
    IN     icf_uint32_t             security_association_status,
    IN     icf_uint8_t              sip_method_name,
    OUT    icf_boolean_t            *p_resp_sync_flag,
    OUT    icf_error_t              *p_ecode);

/**************************************************************************
 *
 * FUNCTION:        icf_cmn_validate_challenge_algo
 * 
 * DESCRIPTION:     This function is a utility function which validates 
 *                  the challenge algo supported.The algos supported
 *                  are MD5,md5,MD5-sess and AKAv1-MD5.
 *
 * RETURNS:         ICF_SUCCESS/ICF_FAILUE.
 *************************************************************************/
icf_return_t icf_cmn_validate_challenge_algo(
	IN icf_uint8_t     *p_algo);


/*******************************************************************************
 * FUNCTION:        icf_cmn_free_stream_capab_list_stored_as_icf_list
 *
 * DESCRIPTION:     Function to free list of stream_capab_st.
 *                  This functionn is similar to icf_cmn_free_stream_capab_list
 *                  with only one difference.
 *                  1.icf_cmn_free_stream_capab_list function frees the 
 *                  stream_capab_st stored as icf_internal_list_st 
 *                  2.icf_cmn_free_stream_capab_list_stored_as_icf_list frees
 *                  the stream_capab_st stored as icf_list_st
 *
 ******************************************************************************/

icf_return_t icf_cmn_free_stream_capab_list_stored_as_icf_list
(
	INOUT icf_glb_pdb_st       *p_glb_pdb,
	INOUT icf_list_st 		   *p_src_list
);
/******************************************************************************
 *
 * FUNCTION:        icf_cmn_init_reg_auth_data
 *
 * DESCRIPTION:     This function initializes the RGM Authentication data
 * 
 *****************************************************************************/
icf_return_t icf_cmn_init_reg_auth_data(
        INOUT icf_cmn_auth_data_st *p_rgm_auth_data);

/******************************************************************************
 * FUNCTION:        icf_cmn_append_msg_to_list
 * DESCRIPTION:     This function appends a message to MRM's message list.
 *****************************************************************************/
icf_return_t icf_cmn_append_msg_to_list(
             icf_msg_st        *p_msg_data);

/******************************************************************************
 * FUNCTION:        icf_cmn_fill_ext_msg
 * DESCRIPTION:     This function creates an internal message for the
 *                  API ID passed to it.
 *****************************************************************************/
icf_return_t icf_cmn_fill_ext_msg(
		     icf_api_id_t    api_id,
             icf_call_id_t   call_id,
             icf_app_id_t    app_id,
             icf_msg_st      **p_p_dummy_resp);

#ifdef ICF_SESSION_TIMER
/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_session_timer_toggle_req
 *
 * DESCRIPTION:     Prints the session timer toggle req API
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_session_timer_toggle_req(
	INOUT icf_msg_st *p_msg,
    OUT   icf_uint8_t *p_sprintf_count);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_session_timer_toggle_resp
 *
 * DESCRIPTION:     Prints the session timer toggle resp API
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_session_timer_toggle_resp(
	INOUT icf_msg_st *p_msg,
    OUT   icf_uint8_t *p_sprintf_count);
#endif

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_req
 *
 * DESCRIPTION:     Prints the generic message  req API
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_req(
		INOUT icf_msg_st *p_msg,
		OUT   icf_uint8_t *p_sprintf_count);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_resp
 *
 * DESCRIPTION:     Prints the generic message  resp API
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_resp(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_ind
 *
 * DESCRIPTION:     Prints the generic message  ind API
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_ind(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);
	
/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_generic_msg_cfm
 *
 * DESCRIPTION:     Prints the generic message cfm API
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_generic_msg_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

	


#ifdef ICF_DNS_LOOKUP_ENABLED
/*----prototype added for open source replacement(IPTK REL 8.1)---start--*/
/******************************************************************************
 * FUNCTION   :     icf_cmn_convert_ip_str_to_transport_addr
 * DESCRIPTION:     This function converts ip address to transport address
 *                  of IPV4 format. It fills the address field of transport
 *                  address structure. 
 *****************************************************************************/

icf_return_t icf_cmn_convert_ip_str_to_transport_addr(
IN               icf_uint8_t *ip_address,
OUT              icf_transport_addr_st *ip_trans_addr);

/*----prototype added for open source replacement(IPTK REL 8.1)---end--*/
#endif

/*****************************************************************************
 ** FUNCTION:           icf_cmn_ssa_map_cc_reason_code
 **
 ** DESCRIPTION:        This function maps the CC reason codes to
 **                     SIP response codes. Previously this function was
 **                     in SSA and we need to call it from CC.
 **
 *****************************************************************************/
icf_return_t icf_cmn_ssa_map_cc_reason_code(
        icf_ssa_reason_t reason_code,
        icf_uint16_t *p_respcode);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_req_recv_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_REQ_RECV_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_req_recv_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_resp_sent_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_RESP_SENT_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_resp_sent_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_req_sent_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_REQ_SENT_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_req_sent_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_prack_resp_recv_ind
 *
 * DESCRIPTION:     make the buffer of ICF_PRACK_RESP_RECV_IND api
 *
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_prack_resp_recv_ind(
        INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);

/* Rel 8.2 Klocworks fix */
#ifdef ICF_TRACE_ENABLE
/******************************************************************************
 * FUNCTION:        icf_cmn_init_trace_data_st
 * DESCRIPTION:     This function is used to initialize icf_trace_data_st
 *                  type variable.
 *****************************************************************************/
icf_void_t icf_cmn_init_trace_data_st(icf_trace_data_st *p_trace_data);
#endif
/* Rel 8.2 Klocworks fix end */

/* Rel 8.2 Klocworks fix */
#ifdef ICF_ERROR_ENABLE
/******************************************************************************
 * FUNCTION:        icf_cmn_init_error_data_st
 * DESCRIPTION:     This function is used to initialize icf_error_data_st
 *                  type variable.
 *****************************************************************************/
icf_void_t icf_cmn_init_error_data_st(icf_error_data_st *p_error_data);
#endif
/* Rel 8.2 Klocworks fix end */
/*****************************************************************************
 ** FUNCTION:           icf_cmn_map_interface_reason_code
 **
 ** DESCRIPTION:        This function maps the reason codes of interface messages 
 **                     to SIP response codes
 **
 *****************************************************************************/
icf_return_t icf_cmn_map_interface_reason_code(
        icf_ssa_reason_t reason_code,
        icf_uint16_t *p_respcode);

/****************************************************************************
 *
 * FUNCTION:        icf_cmn_trace_message_cfm
 *
 * DESCRIPTION:     Prints the message cfm API
 *
 * Functions called: 
 *                     
 *
 ****************************************************************************/
icf_uint8_t* icf_cmn_trace_message_cfm(
	INOUT icf_msg_st *p_msg,
        OUT   icf_uint8_t *p_sprintf_count);


/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_make_hdr_list_node
 *
 * DESCRIPTION:     This function takes as input pointer to header list and
 *                  creates a nodse for the same in list of header lists in
 *                  call context.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_make_hdr_list_node(
        INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN   icf_header_list_st  *p_header_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_make_body_list_node
 *
 * DESCRIPTION:     This function takes as input pointer to body list and
 *                  creates a nodse for the same in list of body lists in
 *                  call context.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_make_body_list_node(
        INOUT	icf_glb_pdb_st		  *p_glb_pdb,
        IN   icf_msg_body_list_st  *p_body_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_make_tag_list_node
 *
 * DESCRIPTION:     This function takes as input pointer to tag list and
 *                  creates a nodse for the same in list of tag lists in
 *                  call context.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_make_tag_list_node(
        INOUT	icf_glb_pdb_st		*p_glb_pdb,
        IN   icf_tag_list_st     *p_tag_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_del_header_list_node_in_call_ctx
 *
 * DESCRIPTION:     This function deletes the input node from the call ctx
 *                  list of header lists 
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_del_header_list_node_in_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        INOUT   icf_list_st     *p_temp_list_node);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_del_body_list_node_in_call_ctx
 *
 * DESCRIPTION:     This function deletes the input node from the call ctx
 *                  list of body lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_del_body_list_node_in_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        INOUT   icf_list_st     *p_temp_list_node);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_del_tag_list_node_in_call_ctx
 *
 * DESCRIPTION:     This function deletes the input node from the call ctx
 *                  list of tag lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_del_tag_list_node_in_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        INOUT   icf_list_st     *p_temp_list_node);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_curr_hdr_list_node_from_call_ctx
 *
 * DESCRIPTION:     This function fetches the current node from the call ctx
 *                  list of hdr lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_fetch_curr_hdr_list_node_from_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        OUT   icf_list_st     **p_p_temp_list_node);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_curr_body_list_node_from_call_ctx
 *
 * DESCRIPTION:     This function fetches the current node from the call ctx
 *                  list of body lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_fetch_curr_body_list_node_from_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        OUT   icf_list_st     **p_p_temp_list_node);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_fetch_curr_tag_list_node_from_call_ctx
 *
 * DESCRIPTION:     This function fetches the current node from the call ctx
 *                  list of tag lists.
 * RETURN Value     ICF_SUCCESS/ICF_FAILURE
 ******************************************************************************/
icf_return_t icf_cmn_fetch_curr_tag_list_node_from_call_ctx(
        INOUT	icf_glb_pdb_st	*p_glb_pdb,
        OUT   icf_list_st     **p_p_temp_list_node);

/*Fix for SPR:19886 (Merged CSR-1-7302347)*/
/****************************************************************************
 * FUNCTION:           icf_cmn_compare_address
 *
 * DESCRIPTION:        This is a utility function to compare two
 *                     icf_address_st type addresses
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/
icf_boolean_t
icf_cmn_compare_address(
        IN icf_address_st *p_adress1,
        IN icf_address_st *p_adress2);

/****************************************************************************
 * FUNCTION:           icf_cmn_remove_parameter_and_port_from_address
 *
 * DESCRIPTION:        This is a utility function to compare two
 *                     icf_address_st type addresses
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/
icf_void_t
icf_cmn_remove_parameter_and_port_from_address(
        icf_address_st *p_address);

/* Fix for SPR 20412: CSR 1-8223807 merged*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_convert_hdr_name_list_into_str_buffer
 *
 * DESCRIPTION:     The function converts the header list present in 
 *                  icf_header_name_list_st into string buffer by appending
 *                  all the headers and seperated by a space followed by coomma.
 *                  Example:{header1 ,header2 ,header3 }
 *
 ******************************************************************************/
icf_void_t icf_cmn_convert_hdr_name_list_into_str_buffer(
           OUT icf_uint8_t              **p_hdr_name_str_buffer,
           IN  icf_header_name_list_st  *p_hdr_name_list);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_COMMON_PROTOTYPES_H__ */
