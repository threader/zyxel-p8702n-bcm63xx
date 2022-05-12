/*************************************************************************
 *
 * 	FILENAME	: 	icf_port_prototypes.h
 *
 * 	DESCRIPTION	:This file contains the prototypes that are used for 
 * 	                port utility functions.
 * 					
 * 	Revision History	:
 *   DATE	NAME		REFERENCE			REASON
 *----------------------------------------------------------
 * 18-Dec-2004   R. Ezhirpavai      ICF LLD         Initial version.
 * 23-Dec-2004   Amit Sharma                          Add new prototypes
 * 31-Dec-2004   Aashish Suchdev                      Porting prototypes
 * 04-Jan-2005   Aashish Suchdev                      Removed cmn_memget
 *                                                    prototypes
 * 06-Jan-2005   Aashish Suchdev                      Added port_memget
 * 06-Jan-2005   Aashish Suchdev                      Updated check_timer
 *                                                    prototype
 * 07-Jan-2005   Aashish Suchdev                      Updated prototype
 *                                                    for module id
 * 10-Jan-2005   Aashish Suchdev                      Updated prototypes
 *                                                    for module specific
 *                                                    errors/traces
 * 11-Jan-2005   Aashish Suchdev                      Updated port receive 
 *                                          for temp compialtion of MRM
 * 28-Jan-2005   Aashish Suchdev                      Addition for strcmp  
 * 20-Apr-2005   Amit Sharma                      Added get_curr_time
 * 13-Dec-2005   Rohit Aggarwal     ICF 4.0 CDD       IPSec coding
 * 04-Jan-2005   Aman Arora          IPSEC            added bind func
 * 30-Jun-2006   Umang Singh        Rel 5.2	      TLS support 	
 * 23/02/2007    Umang Singh        Solving the race condition
 *                                          for timer expiries
 *                                                    
 *Copyright 2006, Aricent.
 *
 *************************************************************************/

#ifndef __ICF_PORT_PROTOTYPES_H__
#define __ICF_PORT_PROTOTYPES_H__

/******************** STANDARD INCLUDE FILES *****************************/
#include <string.h>

/******************** USER INCLUDE FILES *********************************/
#include "icf_common_types.h"
#include "icf_port_struct.h"
#include "icf_port_pltfm_types.h"
#include "icf_port_types.h"
#if defined(ICF_IPV6_ENABLED) && defined(ICF_PORT_WINDOWS)
#include<ws2tcpip.h>
#include<tpipv6.h>
#endif


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

extern icf_return_t icf_dbm_get_module_glb_data(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        IN    icf_int_module_id_t   module_id,
        OUT   icf_void_t            **p_p_data,
        OUT   icf_error_t           *p_ecode);    
    
extern icf_return_t icf_port_flush(
        icf_uint32_t             fd,
        icf_uint32_t             msg_len,
        icf_error_t              *p_ecode);

extern icf_return_t icf_port_select(
        icf_fd_set		*p_read_fdset,
        icf_fd_set       *p_write_fdset,
		icf_fd_set		*p_except_fdset, /* added to capture Connect 
											  * failure for a non-blking
											  * connect in windows */
        icf_uint32_t     timeout_val,
        icf_error_t      *p_ecode);

extern icf_return_t icf_port_open_socket(
        /*ICF_TRANSPORT_TYPE_UDP/ICF_TRANSPORT_TYPE_TCP*/
#ifdef ICF_IPV6_ENABLED
		icf_uint32_t		proto_family,
#endif
        icf_uint8_t      transport_type,
        icf_uint32_t     *p_fd,
        icf_error_t      *p_ecode);

extern icf_void_t icf_port_memset(
        IN  icf_void_t       *p_buf,
        IN  icf_uint8_t      ch,
        IN  icf_uint32_t     n);

extern icf_int32_t icf_port_strcmp(
        IN icf_uint8_t* p_str1,
        IN  icf_uint8_t* p_str2);

icf_uint8_t icf_port_strcmpi(
	IN icf_uint8_t	*p_target,
	IN icf_uint8_t	*p_match);

extern icf_void_t icf_port_strcpy(
        INOUT icf_uint8_t* dest_str,
        IN  icf_uint8_t* src_str);

extern icf_void_t icf_port_strcat(
        INOUT icf_uint8_t* dest_str,
        IN  icf_uint8_t* src_str);

extern icf_uint32_t icf_port_strlen( 
        IN  icf_uint8_t* src_str);

extern icf_uint8_t * icf_port_strchr (
        IN icf_uint8_t   *p_string,
        IN icf_uint8_t    character);

extern icf_uint16_t icf_port_random(
        IN  icf_uint16_t   start_limit,
        IN  icf_uint16_t   end_limit);

extern icf_void_t icf_port_memcpy(
               OUT icf_void_t *dest_buff, 
              IN       icf_void_t *src_buff,
              IN       icf_uint32_t  size);

/*
extern icf_return_t icf_port_receive(
                    INOUT icf_void_t *p_glb_port_info, 
                    IN icf_uint16_t timer,  in milliseconds
                    OUT icf_msg_list_st  *p_msg_list,
                    IN      icf_error_t          *p_ecode);
*/

extern icf_int32_t icf_port_strncmp(
        IN icf_uint8_t   *p_first,
        IN icf_uint8_t   *p_last,
		IN icf_uint32_t   count);
extern icf_uint8_t*  icf_port_strdup (
        IN icf_uint8_t* p_string
        );
extern icf_void_t  icf_port_strfree (
        IN icf_uint8_t* p_string
        );

#ifdef ICF_PORT_WINDOWS
extern icf_return_t icf_port_recv_udp_win(icf_global_port_info_st *p_glb_port_data,
						 icf_uint16_t			fd_id,
						 icf_msg_list_st			*p_icf_msg_list,
						 icf_error_t				*p_ecode);
#endif



/*
 * Following are used for port init
 */

/*
 * Following are for memory alloc/dealloc, timer start/stop
 */

extern icf_void_t* icf_port_memget(
        IN  icf_void_t           *p_port_info,
        IN  icf_uint32_t         size,
        IN  icf_pool_id_t        pool_id,
        OUT icf_error_t          *p_ecode);

extern icf_return_t icf_port_memfree(
        IN  icf_void_t       *p_port_info,
        INOUT  icf_void_t       *p_buf,
        IN  icf_pool_id_t    pool_id,
        OUT icf_error_t      *p_ecode);

extern icf_void_t* icf_port_static_memget(
        IN  icf_uint32_t          size,
        OUT icf_error_t    *p_ecode);

extern icf_return_t icf_port_static_memfree(
        IN  icf_void_t     *p_buf,
        OUT icf_error_t    *p_ecode);

extern icf_void_t icf_port_check_timer_expiry(
        INOUT   icf_void_t       *p_port_info,
        OUT     icf_error_t      *p_ecode);

extern icf_return_t icf_port_start_timer(
        INOUT   icf_void_t     	  *p_glb_port_info,
        INOUT   icf_timer_data_st    *p_buff,
        IN	   icf_uint32_t         duration,
        OUT   icf_timer_id_t       *p_timer_id,
        OUT   icf_error_t  	       *p_ecode);

extern icf_return_t icf_port_stop_timer(
        INOUT    icf_void_t     	  *p_glb_port_info,
        INOUT icf_timer_id_t     timer_id, 
        OUT   icf_error_t        *p_ecode);

/**************************************************************
 * FUNCTION:    icf_port_get_curr_time
 *
 * DESCRIPTION: This is a utility function invoked by the
 *              call logging functions to obtain the timestamp
 *              from the system.
 *
 * RETURNS:     ICF_SUCCESS if the date n time obtained
 *              successfuly. ICF_FAILURE
 *              -If the time() call returned failure.
 *************************************************************/

extern icf_return_t icf_port_get_curr_time(
        INOUT   icf_time_n_date_st *p_time_details);



#ifdef ICF_ERROR_ENABLE
/*
 * Following two prototypes are exposed at the port interface
 * for the toolkit so that relevant errors/alarms/traces
 * can be printed/sent to SM
 */
extern icf_void_t icf_port_log_error(
        INOUT   icf_void_t       *p_glb_port_info,
        IN icf_uint8_t           *p_error_string, 
        IN icf_error_t           error_id,
        IN icf_int_module_id_t   module_id,
        IN icf_error_data_st     *p_error_data);

#ifndef ICF_SM_LOG_ENABLE
/*
 * Following are functions for printing errors
 * in module specific format
 */

extern icf_void_t icf_port_log_mrm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_cc_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_cm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_dbm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_sm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_regm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_cfg_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_ssa_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_mmi_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_scm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_adm_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_chr_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_rfn_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_twc_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_cta_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_ctua_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);


extern icf_void_t icf_port_log_cfw_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_cf_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);

extern icf_void_t icf_port_log_cw_error(
        IN icf_error_id_t    error_id,
        IN icf_error_data_st *p_error_data);
#endif /*ifndef ICF_SM_LOG_ENABLE*/

#endif /*ICF_ERROR_ENABLE*/

#ifdef ICF_TRACE_ENABLE

/*
 * Following two prototypes are exposed at the port interface
 * for the toolkit so that relevant errors/alarms/traces
 * can be printed/sent to SM
 */

extern icf_void_t icf_port_log_trace(
        INOUT   icf_void_t       *p_glb_port_info,
        IN icf_uint8_t           *p_trace_string, 
        IN icf_trace_id_t        trace_id,
        IN icf_int_module_id_t   module_id,
        IN icf_trace_data_st     *p_trace_data);

#ifndef ICF_SM_LOG_ENABLE
/*
 * Following are functions for printing errors
 * in module specific format
 */

extern icf_void_t icf_port_log_mrm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_state_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a state and
 *                  is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_state_to_str(
        IN icf_cc_fsm_state_t state_val,
        OUT icf_uint8_t *p_state_str);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_mmtyp_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to media modify
 *                  type and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_mmtyp_to_str(
        IN icf_uint8_t mms_type,
        OUT icf_uint8_t *p_mms_type_str);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_event_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to an event and
 *                  is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_event_to_str(
        IN icf_uint8_t event_val,
        OUT icf_uint8_t *p_event_str);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_media_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a media and
 *                  is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_media_to_str(
        IN icf_media_mode_t mode_val,
        OUT icf_uint8_t *p_mode_str);


/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_module_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a module id
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_module_to_str(
        IN icf_uint8_t module_id,
        OUT icf_uint8_t *p_module_str);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_cc_timer_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a timer 
 *                  type and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_cc_timer_to_str(
        IN icf_timer_type_t timer_type,
        OUT icf_uint8_t *p_timer_str);



/*******************************************************************************
*
* FUNCTION:        icf_port_app_trace
*
* DESCRIPTION:     This function is used by SSA for trace handling.
*
******************************************************************************/
extern icf_void_t icf_port_app_trace(
          INOUT icf_trace_id_t trace_id);

extern icf_void_t icf_port_log_cc_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_cm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_dbm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_sm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_regm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_cfg_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_ssa_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_cf_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_cw_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_mmi_event_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to an event
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_mmi_event_to_str(
        IN icf_mmi_event_t event_val,
        OUT icf_uint8_t *p_event_str);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_mmi_state_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a state and
 *                  is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_mmi_state_to_str(
        IN icf_mmi_state_t state_val,
        OUT icf_uint8_t *p_state_str);

/*******************************************************************************
 *
 * FUNCTION:        icf_port_log_mmi_api_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to an API ID
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_port_log_mmi_api_to_str(
        IN icf_api_id_t api_val,
        OUT icf_uint8_t *p_api_str);

#if 0
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_map_genlib_ecode
*
*  DESCRIPTION    :   Maps Error Code returned by Genlib to ICF Error Code
*
*  RETURNS        :   
*
*************************************************************************/
icf_void_t
icf_port_map_genlib_ecode (
        IN  gl_error_t                   gl_ecode,
        OUT icf_error_t                *p_ecode);
#endif

extern icf_void_t icf_port_log_mmi_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_scm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_adm_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_chr_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_rfn_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_twc_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_cta_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_ctua_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

extern icf_void_t icf_port_log_cfw_trace(
        IN icf_trace_id_t    trace_id,
        IN icf_trace_data_st *p_trace_data);

#endif /*ifndef ICF_SM_LOG_ENABLE*/

#endif /*ICF_TRACE_ENABLE*/

extern icf_void_t icf_port_dummy(icf_void_t);

extern icf_void_t icf_port_close_fd(
        icf_uint32_t    fd);

extern icf_return_t icf_port_nonblk_connect(
        INOUT  icf_uint16_t  fd,
        IN     icf_uint16_t  dest_port_num,
        IN     icf_uint8_t   *p_dest_ip,
        IN     icf_uint16_t  self_prot_port,
        IN     icf_uint8_t   *p_self_ip,
        OUT    icf_error_t   *p_ecode);

extern icf_boolean_t icf_port_isconnected(
        IN      icf_uint32_t         fd);

extern icf_return_t icf_port_accept(
        IN      icf_uint32_t         server_fd,
        INOUT   icf_uint32_t     *p_fd,
        INOUT   icf_uint8_t      *p_peer_ip,
        INOUT   icf_uint16_t     *p_peer_port);

extern icf_return_t icf_port_tcp_recv(
        icf_uint16_t    fd,
        icf_void_t     *p_msg,
        icf_uint32_t  msg_len);

extern icf_return_t icf_port_tcp_peek(
        icf_interface_info_st  *p_interface_info,
        icf_void_t     *p_msg,
        icf_error_t    *p_ecode);

extern icf_return_t icf_port_tcp_non_blocking_peek(
        icf_interface_info_st  *p_interface_info,
        icf_void_t     *p_msg,
        icf_error_t    *p_ecode);

/*************************************************************************
*  FUNCTION NAME  :   icf_port_gethostbyname
*  DESCRIPTION    :   Wrapper for gethostbyname.p_p_addr is the address
*           of memory allocated by calling routine for storing the IP
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
extern icf_return_t     icf_port_gethostbyname(
        IN  icf_uint8_t*     p_hostname,
        INOUT   icf_uint8_t*    p_addr);
extern icf_return_t icf_port_get_host_ip
	        (icf_int8_t* pHost,
	         icf_int8_t** ppDest);


extern icf_void_t icf_port_printf(char* fmt, ...);

extern FILE *icf_port_fopen(const char *filename, const char *mode);
extern char *icf_port_fgets(char *s, int n, FILE *stream);
extern int  icf_port_fseek 
(
    FILE    *stream,
    long    offset,
    int     whence
) ;
/****************************************************************************
** DESCRIPTION: Porting wrapper function for fgets
**    
***************************************************************************/
int icf_port_fgetc (FILE *stream, char  *c) ;

/****************************************************************************
** DESCRIPTION: Porting wrapper function for fputc
**    
***************************************************************************/
int icf_port_fputc (char c, FILE *stream);

/****************************************************************************
** DESCRIPTION: Porting wrapper function for fputs
**    
***************************************************************************/
int icf_port_fputs (char *s, FILE *stream);

extern icf_int32_t icf_port_strmatch(
        IN icf_uint8_t* p_str1,
        IN  icf_uint8_t* p_str2);


extern icf_void_t*
icf_port_realloc(
        INOUT  icf_void_t       *p_port_info,
        IN  icf_uint32_t         size
        );

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strstri
*
*  DESCRIPTION    :   Utility function for finding sub-string 
*                     within a string without case senstivity
*
*  RETURNS        :   Pointer to the location of match, or NULL if no match
*
 *************************************************************************/
icf_uint8_t *icf_port_strstri(
		IN	icf_void_t           *p_port_info,
		IN 	icf_uint8_t			*p_target,
		IN	icf_uint8_t			*p_match,
		OUT icf_error_t			*p_ecode);


/*------------------------------------------------------------------------------
 * FUNCTION:    ippstk_port_sa_fetch_enc_algo
 *
 * DESCRIPTION: This function is used to fetch Encryption algos from Wrapper
 *
 * RETURNS:     IPPSTK_SUCCESS or IPPSTK_FAILURE
 *              
 *----------------------------------------------------------------------------*/
icf_return_t icf_port_pltfm_fetch_enc_algo_req (icf_msg_st					*p_port_req,
												icf_port_pltfm_ret_val_t	*p_port_ret_val,
												icf_msg_st					*p_port_resp);



/*------------------------------------------------------------------------------
 * FUNCTION:    ippstk_port_sa_fetch_auth_algo
 *
 * DESCRIPTION: This function is used to fetch Authentication algos from Wrapper
 *
 * RETURNS:     IPPSTK_SUCCESS or IPPSTK_FAILURE
 *              
 *----------------------------------------------------------------------------*/
icf_return_t
				icf_port_pltfm_fetch_auth_algo_req (
				icf_msg_st					*p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st					*p_port_resp);


/*------------------------------------------------------------------------------
 * FUNCTION:    ippstk_port_sa_create_req
 *
 * DESCRIPTION: This function is used to send Create SA Request to Wrapper
 *
 * RETURNS:     IPPSTK_SUCCESS or IPPSTK_FAILURE
 *              
 *----------------------------------------------------------------------------*/
icf_return_t	icf_port_pltfm_sa_create_req(
				icf_msg_st					*p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st					* p_port_resp);

/*------------------------------------------------------------------------------
 * FUNCTION:    ippstk_port_sa_modify_req
 *
 * DESCRIPTION: This function is used to send Modify SA Request to Wrapper
 *
 * RETURNS:     IPPSTK_SUCCESS or IPPSTK_FAILURE
 *              
 *----------------------------------------------------------------------------*/
icf_return_t	icf_port_pltfm_sa_delete_req(
				icf_msg_st					*p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st					* p_port_resp);


/*------------------------------------------------------------------------------
 * FUNCTION:    ippstk_port_sa_delete_req
 *
 * DESCRIPTION: This function is used to send Delete SA Request to Wrapper
 *
 * RETURNS:     IPPSTK_SUCCESS or IPPSTK_FAILURE
 *              
 *----------------------------------------------------------------------------*/
icf_return_t icf_port_pltfm_sa_modify_req(
				icf_msg_st					*p_port_req,
				icf_port_pltfm_ret_val_t	*p_port_ret_val,
				icf_msg_st					* p_port_resp);

icf_return_t	icf_port_pltfm_init(
									icf_msg_st					*p_port_req,
									icf_port_pltfm_ret_val_t	*p_port_ret_val,
									icf_msg_st					* p_port_resp,
									icf_return_t (*aRsporIndCallBack)(icf_msg_st *pMsg));

icf_void_t	icf_port_pltfm_deinit(icf_void_t);

icf_return_t	icf_port_pltfm_get_device_params (
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);

icf_return_t	icf_port_pltfm_get_access_nw_info(
											 icf_msg_st* p_port_req,
											 icf_port_pltfm_ret_val_t	*p_port_ret_val,
											 icf_msg_st* p_port_resp);

#ifdef ICF_NW_ACTIVATE
icf_return_t	icf_port_pltfm_pdp_activate_req (
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);


icf_return_t	icf_port_pltfm_pdp_deactivate_req (
											icf_msg_st* p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st* p_port_resp);
#endif

#ifdef ICF_QOS_SUPPORT
icf_return_t	icf_port_pltfm_qos_reserve_req_non_ims (
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);

icf_return_t	icf_port_pltfm_qos_release_req_non_ims (
											icf_msg_st* p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st* p_port_resp);
#endif

icf_return_t	icf_port_pltfm_run_aka_procedure (
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);

icf_return_t	icf_port_pltfm_get_host_ip(
											icf_msg_st					*p_port_req,
											icf_port_pltfm_ret_val_t	*p_port_ret_val,
											icf_msg_st					*p_port_resp);


/*******************************************************************************
 * FUNCTION:        icf_port_compare_ip_addr
 *
 * DESCRIPTION:     This function compares two transport addresses
 *                  
 * RETURNS          ICF_SUCCESS: In case src_addr == dest_addr
 *					ICF_SUCCESS: In case src_addr != dest_addr
 ******************************************************************************/
icf_return_t  icf_port_compare_ip_addr(
IN		 icf_transport_addr_st src_addr,
IN		 icf_transport_addr_st dest_addr);

/*******************************************************************************
 * FUNCTION:        icf_port_pton
 *
 * DESCRIPTION:     This function converts string format of IP address
 *					to binary format. Converted address is returned in
 *					ip_binary_fmt.
 *                  
 * RETURNS          ICF_SUCCESS: In case conversion succeeds
 *					ICF_SUCCESS: In case conversion fails
 ******************************************************************************/
icf_return_t icf_port_pton(
IN		char *ip_string_fmt, 
#ifdef ICF_IPV6_ENABLED
OUT		struct sockaddr_storage *ip_binary_fmt);
#else
OUT		icf_sockaddr_in_st  *ip_binary_fmt);
#endif
/*******************************************************************************
 * FUNCTION:        icf_port_ntop
 *
 * DESCRIPTION:     This function converts binary format of IP address
 *					to string format. Converted address is returned in
 *					ip_string_fmt.
 *                  
 * RETURNS          ICF_SUCCESS: In case conversion succeeds
 *					ICF_SUCCESS: In case conversion fails
 ******************************************************************************/
icf_return_t icf_port_ntop(
#ifdef ICF_IPV6_ENABLED
OUT		struct sockaddr_storage *ip_binary_fmt, 
#else
IN		icf_sockaddr_in_st      *ip_binary_fmt,
#endif 
OUT		char *ip_string_fmt);


/*******************************************************************************
 * FUNCTION:        icf_port_str_to_trans_addr
 *
 * DESCRIPTION:     This function is used to convert string ip 
 *					address to icf_transport_addr_st structure
 *                  
 * RETURNS          ICF_SUCCESS: In case conversion succeeds
 *					ICF_SUCCESS: In case conversion fails
 ******************************************************************************/
icf_return_t icf_port_str_to_trans_addr(
IN		icf_uint8_t *ip_address,
OUT		icf_transport_addr_st *ip_trans_addr);

/***************************************************************************
 **
 ** FUNCTION:     icf_port_check_if_ip
 ** DESCRIPTION:  This function checks if the string passed is an IP address
 **               
 **
 ***************************************************************************/
icf_return_t icf_port_check_if_ip(icf_uint8_t *p_ip_address);

/*************************************************************************
*  FUNCTION NAME  :   icf_port_bind
*  DESCRIPTION    :   Wrapper for bind system call. This would primarily be
*  					  required incase of IPSec
*************************************************************************/
icf_return_t
icf_port_bind(INOUT  icf_uint16_t  fd,
 		IN	   icf_uint8_t	*p_self_ip,
		IN	   icf_uint16_t	self_port_num,
        OUT    icf_error_t   *p_ecode );

#ifdef ICF_IPV6_ENABLED
/***************************************************************************
 **
 ** FUNCTION:     icf_port_get_family_type
 ** DESCRIPTION:  This Function will set  ICF_IPV6_FAMILY for Ipv6 
 **				  Address, ICF_IPV4_FAMILY for IPv4 address 
 **				  in dType
 **
 ***************************************************************************/
icf_return_t	icf_port_get_family_type(
IN	icf_uint8_t *p_ip_address, 
IN icf_uint16_t dPort,
OUT	icf_uint32_t *dType);


#endif

/***************************************************************************
 **
 ** FUNCTION:     icf_port_address_match
 ** DESCRIPTION:  This Function compares 2 variables of type 
 **				   icf_transport_address_st
 **
 ***************************************************************************/
icf_return_t	icf_port_address_match(
				IN	icf_transport_address_st  *p_address1,
				IN	icf_transport_address_st  *p_address2);

/***************************************************************************
 ** FUNCTION:     icf_port_create_secure_conn
 ** DESCRIPTION:  following is used for creating a TLS conn
 ***************************************************************************/
icf_return_t icf_port_create_secure_conn (
		icf_void_t   *p_port_info,               
												icf_uint32_t	*p_conn_handle,
												icf_uint8_t  *dest_ip_addr,
												icf_uint32_t	dest_port_num,
												icf_error_t  *p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_close_secure_conn
 ** DESCRIPTION:  following is used for closing a TLS conn
 ***************************************************************************/
icf_return_t icf_port_close_secure_conn (
                    icf_void_t          *p_port_info,               
                    icf_uint32_t	    *p_conn_handle,
                    icf_error_t         *p_ecode);


/*****************************************************************************
** FUNCTION:        icf_port_send_http_req
** DESCRIPTION:     This function is used for sending an HTTP request over the 
**   			    network over TCP.
*****************************************************************************/
icf_return_t icf_port_send_http_req (
					icf_void_t          *p_port_info, 
					icf_uint32_t        connHandle, 			
					icf_uint8_t         *p_http_msg,
					icf_uint32_t        msg_len,
					icf_error_t         *p_ecode);


/***************************************************************************
 ** FUNCTION:     icf_port_set_sa_fd
 ** DESCRIPTION:  The function is invoked from SSA to set the SA sserver 
 **               sock FD in the win_ipc_info structures
 ***************************************************************************/
icf_void_t  icf_port_set_sa_fd (
            icf_void_t               *p_glb_port_info,
            icf_transport_mode_t     trans_mode,
            icf_uint16_t             fd_id);

/***************************************************************************
 ** FUNCTION:     icf_port_set_sa_enabled
 ** DESCRIPTION:  The function is invoked from SSA to identify if the SA 
 **               server is setup or not. Incase the SA server is setup, we 
 **               will use that socket to send messages at the protected port
 **
 ***************************************************************************/
icf_void_t  icf_port_set_sa_enabled (
            icf_void_t        *p_glb_port_info,
            icf_boolean_t     mode);

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_itoa
*
*  DESCRIPTION    :   Utility function for converting integet value to string
*
*  RETURNS        :   Pointer to the string formed
*
 *************************************************************************/
icf_uint8_t *icf_port_itoa(icf_int32_t num,icf_uint8_t *p_dest);
#ifdef ICF_REL_OLD
/*-------------------------------------------------------------------
 *
 *  FUNCTION NAME  :   icf_port_atoi
 *
 *  DESCRIPTION    : Returns the integer value for the string input set by the
 *                   customer on the LFGU command line
 *
 *  RETURNS        : Returns the integer value for the string input 
 *---------------------------------------------------------------------*/
icf_uint32_t icf_port_atoi 
(
    IN      icf_uint8_t     *p_string
);
#endif
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strstri2
*
*  DESCRIPTION    :   Utility function for finding sub-string 
*                     within a string without case senstivity
*
*  RETURNS        :   Pointer to the location of match, or NULL if no match
*
 *************************************************************************/
icf_uint8_t *icf_port_strstri2(
		IN 	icf_uint8_t			*p_target,
		IN	icf_uint8_t			*p_match);

icf_void_t icf_port_icf_port_strlwr(
    INOUT icf_uint8_t *string);

icf_return_t icf_port_cbk_RsporIndCallBack(
				icf_msg_st      *p_msg);


icf_return_t icf_port_pltfm_send(void * p_msg);


/*************************************************************************
*  FUNCTION NAME  :   icf_port_blk_connect
*  DESCRIPTION    :   Wrapper for connect system call
*  This routine is invoked by IPPHONETK to make a blocking connect
*  to a TCP server.
*************************************************************************/
icf_return_t
icf_port_blk_connect(
        INOUT  icf_uint16_t  fd,
        IN     icf_uint16_t  dest_port_num,
        IN     icf_uint8_t   *p_dest_ip,
        IN     icf_uint16_t  self_prot_port,
        IN     icf_uint8_t   *p_self_ip,
        OUT    icf_error_t   *p_ecode
        );
/******************************************************************************
 ** FUNCTION:    icf_is_valid_octet
 **
 ** DESCRIPTION: This function check if valid octet is recived in string
 ******************************************************************************/
icf_return_t icf_is_valid_octet(icf_uint8_t* p_string,icf_uint8_t *p_number);
#ifdef ICF_SECURE_SUPPORT

/***************************************************************************
 ** FUNCTION:     icf_port_secure_connect
 ** DESCRIPTION:  following is used for issuing an SSL connect
 ***************************************************************************/
icf_void_t* icf_port_secure_connect(
            icf_void_t               *p_secure_ctx,
            icf_uint32_t             fd_id,
            icf_error_t              *p_ecode);

icf_void_t icf_port_free_secure_data(
            icf_void_t               *p_secure_data);

/***************************************************************************
 ** FUNCTION:     icf_port_secure_accept
 ** DESCRIPTION:  following is used for issuing an SSL accept
 ***************************************************************************/
icf_void_t* icf_port_secure_accept(
            icf_void_t               *p_secure_ctx,
            icf_uint32_t             fd_id,
            icf_error_t              *p_ecode);

/*************************************************************************
*  FUNCTION NAME  :   icf_port_secure_read
*  DESCRIPTION    :   This function reads the given number of bytes from 
*  given TLS conn fd
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_secure_read(icf_void_t    *p_secure_data,
                  icf_uint32_t  fd_id,         
                  icf_void_t    *p_msg,
                  icf_uint32_t  msg_len);

/*************************************************************************
*  FUNCTION NAME  :   icf_port_secure_peek
*  DESCRIPTION    :   This function peeks a message from tls socket
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
icf_port_secure_peek(icf_void_t *p_secure_data,       
                  icf_interface_info_st  *p_interface_info,
                  icf_void_t     *p_msg,
                  icf_error_t    *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_secure_send
*
*  DESCRIPTION    :   This function sends the message on the SSL conn
*                       corresponding to the fd passed.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_secure_send(icf_void_t *p_secure_data,
                     icf_void_t     *p_msg,
                     icf_uint32_t    msg_len,
                     icf_error_t    *p_ecode);



#endif

#ifdef ICF_DNS_LOOKUP_ENABLED
/*****************************************************************************
 * FUNCTION:        icf_port_is_nameserver_pres
 *
 * DESCRIPTION:     The function checks for the presence of nameserver in the 
 *                  resolv.conf file
 * 
 * RETURNS:         Returns ICF_TRUE if the nameserver is present else returns 
 *                  false
 *
 *****************************************************************************/

icf_boolean_t icf_port_is_nameserver_pres(icf_void_t);
#endif
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
        OUT icf_error_t                    *p_ecode );

#endif

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_get_current_time
 *
 *  DESCRIPTION     :    This function gets the current time in seconds
 *  RETURNS         :    Nothing 
 *************************************************************************/
icf_return_t 
icf_port_get_current_time(
		OUT		icf_time_t		 *p_curr_time);

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_get_current_timestamp
 *
 *  DESCRIPTION     :    This function gets the current time in seconds
 *  RETURNS         :    Nothing 
 *************************************************************************/
icf_return_t 
icf_port_get_current_timestamp(
		OUT		icf_time_stamp_st		 *p_curr_time);

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_get_current_timestamp
 *
 *  DESCRIPTION     :    This function gets the current time in seconds
 *  RETURNS         :    Nothing
 *************************************************************************/
icf_return_t
icf_port_get_current_timestamp(
                OUT             icf_time_stamp_st                *p_curr_time);


icf_void_t icf_port_print_mem_pool_stats(icf_void_t);

#if defined ICF_PORT_WINCE
icf_void_t icf_port_logs_ind(char* fmt, ...);
#endif

#ifdef ICF_PORT_VXWORKS
icf_return_t icf_port_get_file_from_host(IN icf_ftp_info_st *p_ftp_info,
                                 IN icf_uint8_t *p_host_file,
                                 IN icf_uint8_t *p_target_file);

#endif

/* Hash functions prototypes start*/

/***************************************************************************
 ** FUNCTION:     icf_port_create_numeric_hash_table
 ** DESCRIPTION:  This function is used to create and initalize numeric   
 **               hash table. On success this function will return an id to
 **               the newly created hash table. this will be used for 
 **				  adding/removing/fetching data from this hash table. 
 **
 ***************************************************************************/
icf_return_t icf_port_create_numeric_hash_table(
	IN	icf_int_module_id_t module_id,
	OUT	icf_hash_table_id_t *id,
	IN	icf_uint32_t max_key_qty,
	IN	icf_uint32_t max_num_bucket,
	OUT	icf_error_t	*p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_create_string_hash_table
 ** DESCRIPTION:  This function is used to create and initalize string   
 **               hash table. On success this function will return an id to
 **               the newly created hash table. this will be used for 
 **				  adding/removing/fetching data from this hash table. 
 **
 ***************************************************************************/
icf_return_t icf_port_create_string_hash_table(
	IN	icf_int_module_id_t module_id,
	OUT	icf_hash_table_id_t *id,
	IN	icf_uint32_t max_key_qty,
	IN	icf_uint32_t max_num_bucket,
	OUT	icf_error_t	*p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_numeric_hash_add
 ** DESCRIPTION:  This function is used to add  p_element into the     
 **               numeric hash table identified by the id. 
 **
 ***************************************************************************/
icf_return_t icf_port_numeric_hash_add(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	IN	icf_uint32_t key,
	IN	icf_void_t *p_element,
	OUT	icf_error_t *p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_string_hash_add
 ** DESCRIPTION:  This function is used to add  p_element into the     
 **               string hash table identified by the id. 
 **
 ***************************************************************************/
icf_return_t icf_port_string_hash_add(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	IN	icf_uint8_t *key,
	IN	icf_void_t *p_element,
	OUT	icf_error_t *p_ecode);
	
/***************************************************************************
 ** FUNCTION:     icf_port_numeric_hash_fetch
 ** DESCRIPTION:  This function is used to fetch p_element from the     
 **               numeric hash table identified by the id. returns p_elenent
 **				  on success. returns ICF_NULL on failure.
 **
 ***************************************************************************/
icf_void_t* icf_port_numeric_hash_fetch(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	IN	icf_uint32_t key,
	OUT	icf_error_t *p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_string_hash_fetch
 ** DESCRIPTION:  This function is used to get  p_element from the     
 **               string hash table identified by the id. returns p_element
 **				  on success. returns ICF_NULL on failure  
 **
 ***************************************************************************/
icf_void_t* icf_port_string_hash_fetch(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	IN	icf_uint8_t *key,
	OUT	icf_error_t *p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_numeric_hash_remove
 ** DESCRIPTION:  This function is used to remove p_element from the     
 **               numeric hash table identified by the id. returns p_elenent
 **				  on success. returns ICF_NULL on failure.
 **
 ***************************************************************************/
icf_void_t* icf_port_numeric_hash_remove(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	IN	icf_uint32_t key,
	OUT	icf_error_t *p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_string_hash_remove
 ** DESCRIPTION:  This function is used to remove  p_element from the     
 **               string hash table identified by the id. returns p_element
 **				  on success. returns ICF_NULL on failure  
 **
 ***************************************************************************/
icf_void_t* icf_port_string_hash_remove(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	IN	icf_uint8_t *key,
	OUT	icf_error_t *p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_get_iterator_from_hash_table
 ** DESCRIPTION:  This function is used to get all the elements from    
 **               string hash table identified by the id. returns list of 
 **				  elements on success.  
 **
 ***************************************************************************/
icf_list_st* icf_port_get_iterator_from_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	OUT	icf_error_t *p_ecode);

/***************************************************************************
 ** FUNCTION:     icf_port_hash_table_init
 ** DESCRIPTION:  This function initializes hash table manager    
 **
 ***************************************************************************/
icf_return_t icf_port_hash_table_init(icf_void_t);


icf_return_t icf_port_delete_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	OUT	icf_error_t *p_ecode);


/* Hash functions prototypes end*/

#ifdef ICF_DNS_LOOKUP_ENABLED
/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_set_ares_fd
 *
 *  DESCRIPTION     :    This function sets the ares fd in the port data.
 *                       This is required in case when DNS REQ timer expires
 *                       IPTK should clear that FD from the read_fd_set.
 *  RETURNS         :    ICF_SUCCESS
 *************************************************************************/

icf_return_t icf_port_set_ares_fd(icf_global_port_info_st* p_glb_port_data,
                                  icf_uint32_t fd_id);

#endif

#ifdef ICF_TRACE_ENABLE

icf_void_t  icf_port_populate_trace_database(
        IN  icf_trace_database_st *p_trace_database_val);

icf_return_t icf_port_trace_match_criteria_value(
        IN      icf_trace_type_t        trace_type,
        IN      icf_trace_criteria_t  trace_criteria,
        IN      icf_trace_rule_t  trace_rule,
        IN      icf_trace_criteria_data_st  *p_criteria_data);
#endif

/***************************************************************************
 ** FUNCTION:     icf_port_isnumeric
 ** DESCRIPTION:  This function checks if the input string is a number or not
 **
 ***************************************************************************/
icf_boolean_t icf_port_isnumeric(
        IN      icf_uint8_t     *p_str);


icf_void_t icf_port_filter_printf(icf_uint8_t *p_buff, ...);
icf_void_t *icf_port_get_first_entry_from_string_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
    OUT icf_uint8_t  **p_p_key);
icf_void_t *icf_port_get_next_entry_from_string_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
    IN icf_uint8_t  *p_key,
    INOUT icf_uint8_t  **p_p_next_key);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_PORT_PROTOTYPES_H__ */
