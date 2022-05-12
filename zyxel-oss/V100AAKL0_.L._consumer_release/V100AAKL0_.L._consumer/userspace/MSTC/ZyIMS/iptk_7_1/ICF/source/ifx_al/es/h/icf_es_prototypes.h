/*------------------------------------------------------------------------------
 *
 * File name        : icf_es_prototypes.h
 *
 * Purpose          : This file contains the prototypes of all functions 
 *                    used by ES 
 *                    
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 14-Feb-2005  Pankaj Negi     Non Service LLD     Initial
 *
 * 08-Dec-2005	Jyoti Narula	ICF 4.0 LLD			Auto register related changes 
 * 08-Dec-2005  Rohit Aggarwal  ICF 4.0 CDD         Prototype of callback for
 *                                                  platform messages
 * 03-Jul-2006	Aman Aggarwal	ICF 5.2 	        Added prototypes for asynchronous DNS 
 * 						    						query
 * Copyright (c) 2006, Aricent .
 *---------------------------------------------------------------------------*/

#ifndef __ICF_ES_PROTOTYPES_H__
#define __ICF_ES_PROTOTYPES_H__


#include "icf_es_types.h"
#include "icf_port_types.h"
#include "icf_port_struct.h"

#ifdef ICF_PORT_VXWORKS
#include <sys/times.h>
#endif

#ifndef ICF_PORT_WINDOWS
#ifndef ICF_PORT_VXWORKS
#include <sys/time.h>
#endif
#endif


#ifdef ICF_NAT_MEDIA_TRAVERSAL
#include "../../../../../utils/natlib/ext_if/h/nat_types.h" 
#endif

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */


typedef void icf_interface_msg_st;

icf_void_t icf_recv(fd_set *p_fifo_fd_set,
        icf_msg_list_st *p_icf_msg_list,
        icf_void_t *p_interface_data);

/*************************************************************************
*
*  FUNCTION NAME  :   icf_es_init
*
*  DESCRIPTION    :   This function initializes the eshell and porting layer .
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
extern icf_return_t icf_es_init(
        INOUT icf_void_t       **p_p_port_data,
        INOUT icf_error_t      *p_ecode);

icf_return_t icf_send(IN icf_void_t *p_port_data,
                            INOUT icf_msg_st *p_msg_data,
                            OUT icf_error_t *p_ecode);

icf_return_t icf_os_send(IN icf_interface_info_st *p_interface_data,
                 icf_void_t *p_msg,
                 icf_error_t *p_ecode);

icf_return_t ipp_es_read_fifo_n_call_msg_handler(fd_set *p_fifo_fd_set);

icf_return_t icf_es_init_n_open_fifos(fd_set *p_fifo_fd_set);

extern	icf_void_t icf_free_mem_and_exit(
								   icf_glb_pdb_st	*p_glb_data,
								   icf_conf_file_st conf,
								   icf_error_t		ecode,
								   icf_return_t		ret_val);


extern 	icf_return_t icf_process_message(icf_void_t * p_icf_glb_data);

extern icf_return_t icf_mmial_rm_process_incoming_msg(
        IN icf_interface_info_st *p_interface_data,
        INOUT icf_interface_msg_st *p_msg,
        OUT icf_msg_list_st *p_icf_msg_list,
        OUT icf_error_t *p_ecode);

extern icf_return_t icf_mmial_mm_process_incoming_msg(
        IN icf_interface_info_st *p_interface_data,
        INOUT icf_interface_msg_st *p_msg,
        OUT icf_msg_list_st *p_icf_msg_list,
        OUT icf_error_t *p_ecode);

extern icf_return_t icf_mmial_process_outgoing_msg(
        IN icf_interface_info_st *p_interface_data, /* interface info */
        IN icf_msg_st *p_icf_out_msg,
        OUT icf_error_t *p_ecode);

extern icf_return_t icf_port_tcp_send(
        icf_uint32_t     fd,
        icf_void_t       *p_msg,
        icf_uint32_t     msg_len,
        icf_error_t      *p_ecode);

extern icf_return_t icf_port_udp_send(
        icf_uint32_t    fd,
        icf_void_t     *p_msg,
        icf_uint32_t    msg_len,
        icf_uint32_t    dest_port_num,
        icf_uint8_t*    dest_ip_addr,
        icf_error_t    *p_ecode);

extern icf_return_t icf_port_fifo_send(
        icf_uint32_t     fd,
        icf_void_t       *p_msg,
        icf_uint32_t     msg_len,
        icf_error_t      *p_ecode);

extern icf_return_t icf_port_fifo_recv(
        icf_uint32_t    fd,
        icf_void_t     *p_msg,
        icf_uint32_t   msg_len,
        icf_error_t    *p_ecode);

icf_return_t icf_sip_nw_process_outgoing_msg(
       IN  icf_interface_info_st *p_interface_data,
       IN  icf_void_t  *p_msg_data,
       INOUT  icf_error_t *p_ecode);

#ifdef ICF_PORT_WINDOWS
icf_return_t icf_wsa_init(icf_void_t);
#endif
icf_return_t get_port_info_from_file(icf_port_info_st  *p_port_info,
							  icf_error_t	*err_code);

#ifdef ICF_PA_TRIGGERED
icf_return_t icf_send_init_resp_to_pa(icf_int32_t status);
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_es_recv
 *
 * DESCRIPTION:    This function will read message from socket, give the message
 *                 to corresponding AL and return message to MRM
 *                 of port to write msg on NW interface
 * 
*******************************************************************************/
icf_return_t icf_es_recv(IN icf_void_t *p_port_data,
       icf_uint16_t select_timer_duration,
       icf_msg_list_st *p_icf_msg_list,
        icf_error_t *p_ecode);


icf_return_t icf_cfgal_process_incoming_msg(
                        icf_interface_info_st *p_interface_info,        
                        icf_uint8_t      *p_nw_interface_msg,
                        icf_msg_list_st  *p_icf_msg_list,
                        icf_error_t      *p_ecode);

icf_return_t icf_paal_process_incoming_msg(
                        icf_interface_info_st *p_interface_info,        
                        icf_uint8_t      *p_nw_interface_msg,
                        icf_msg_list_st  *p_icf_msg_list,
                        icf_error_t      *p_ecode);


icf_return_t icf_smal_process_incoming_msg(
                        icf_interface_info_st *p_interface_info,                                     icf_uint8_t      *p_api_header,
                        icf_msg_list_st  *p_icf_msg_list,
                        icf_error_t      *p_ecode);

icf_return_t icf_sipal_process_incoming_msg(
                        icf_interface_info_st *p_interface_info,
                        icf_uint8_t      *p_nw_interface_msg,
                        icf_msg_list_st  *p_icf_msg_list,
                        icf_error_t      *p_ecode);

extern icf_return_t 
   icf_port_open_fifo(
        icf_uint8_t      *p_fifo_name,
        icf_uint32_t     *p_fd,
        icf_uint32_t     permissions,
        icf_error_t      *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_es_open_nw_port
 *
 * DESCRIPTION:    This function will open up a UDP/TCP socket ( as specified)
 *                  by transport_type, and bind to it and add the FD in the li
 *                  istening FDSET maintained in global port info. It will 
 *                  also return the bound fd to calling context through the 
 *                  pointer of the nw interface structure
 *                  Only the Port and transport type info is passed
*******************************************************************************/
icf_return_t icf_es_open_nw_port(
        INOUT icf_void_t                 *p_port_data,
        INOUT icf_nw_interface_info_st   *p_nw_intf_info,
		INOUT icf_int8_t                 *p_self_ip,
        OUT   icf_error_t                *p_ecode);


/*******************************************************************************
 *
 * FUNCTION:        icf_es_deinit
 *
 * DESCRIPTION:    This function will close udp/tcp socket/ delete timer list
 *                 and delete task specific memory
*******************************************************************************/
icf_return_t icf_es_deinit(
        INOUT icf_void_t                 *p_port_data,
        OUT   icf_error_t                *p_ecode);

extern icf_return_t   icf_es_add_fd_to_es_db(
        icf_uint16_t     *p_fd_array,
        icf_uint16_t     fd);

extern icf_return_t   icf_es_rem_fd_from_es_db(
        icf_uint16_t     *p_fd_array,
        icf_uint32_t     fd);

icf_return_t icf_send_to_app(icf_glb_pdb_st*	p_glb_pdb,
				icf_msg_st*	p_msg_data,
				icf_void_st*	p_routing_info,
				icf_error_t*	p_ecode);

ICF_INIT_START
extern icf_return_t icf_read_conf_file(FILE* fp, icf_conf_file_st* p_file);
extern icf_void_t icf_str_trim(INOUT char * str);
extern icf_return_t icf_isnumeric( icf_uint8_t *myStr );
ICF_INIT_END


/*------------------------------------------------------------------------------
 * FUNCTION:    icf_es_cbk_recv_from_pltfm
 *
 * DESCRIPTION: This function is the callback invoked by wrapper whenver it 
 *				receives a response or unsolicited indication from Port.
 *
 * RETURNS:     ICF_SUCCESS or ICF_FAILURE
 *              
 *----------------------------------------------------------------------------*/
icf_return_t icf_es_cbk_recv_from_pltfm(
		INOUT	icf_msg_st	*p_port_resp);


/*******************************************************************************
 *
 * FUNCTION:        icf_es_append_to_msg_list
 *
 * DESCRIPTION:     This is a utility function that appends an external message 
 *                  at the end of MRM message processing list.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_es_append_to_msg_list(
        INOUT      icf_msg_st           *p_msg,
        INOUT   icf_glb_pdb_st       *p_glb_pdb,
        OUT     icf_error_t          *p_ecode);

icf_return_t   icf_es_add_fd_to_es_tcp_db(

        icf_tcp_data_st     *p_fd_array,

        icf_uint32_t        fd,

        icf_boolean_t       is_secure_fd);



icf_return_t   icf_es_rem_fd_from_es_tcp_db(

        icf_tcp_data_st         *p_fd_array,

        icf_global_port_info_st *p_port_info,

        icf_uint32_t            fd);

#ifdef ICF_TRACE_ENABLE
icf_uint32_t icf_es_get_max_log_file_size(icf_void_t);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_send_to_app_for_logs
 *
 * DESCRIPTION:     This function is used for sending LOGS to the application.
 *
*******************************************************************************/
icf_return_t icf_send_to_app_for_logs(icf_glb_pdb_st*	p_glb_pdb,
				icf_msg_st*	    p_msg_data,
				icf_void_st*	p_routing_info,
				icf_error_t*	p_ecode);

#ifdef ICF_DNS_LOOKUP_ENABLED



icf_return_t icf_processing_dns_query_resp(icf_void_t *p_port_data);

icf_return_t icf_es_set_ares_fds(icf_global_port_info_st* p_glb_port_data,

				 icf_uint32_t fd_id,

 				 icf_uint8_t index,

				icf_uint16_t timer_duration);



icf_return_t icf_es_set_ares_fds_fr_select(icf_fd_set* p_read_fds,

                                   icf_fd_set* p_write_fds,

                                   icf_ares_fds_st ares_port_info);



icf_uint8_t icf_es_chk_ares_fd_present(icf_ares_fds_st ares_data,

                                        icf_uint32_t fd);



icf_return_t icf_es_rmv_ares_fd(icf_global_port_info_st* p_glb_port_data);

#endif

#ifdef ICF_PORT_VXWORKS

icf_int32_t icf_main(void);

icf_return_t icf_set_ftp_info(
    IN icf_uint8_t *ftp_server_ip,
    IN icf_uint8_t *login,
    IN icf_uint8_t *password,
    IN icf_uint8_t *config_path
);
icf_return_t icf_set_ramdrive_info
(
    IN icf_uint16_t   bytes_per_block,
    IN icf_uint16_t   blocks_per_track,
    IN icf_uint16_t   number_of_blocks_on_device
);
icf_return_t icf_setup_ram_disk
(IN icf_ramdisk_param_st *icf_ramdisk_param);

#endif

#ifndef ICF_PORT_FWD
icf_return_t icf_print_api_string(icf_api_id_t api_id);

#endif
icf_return_t icf_print_msg_hdr(icf_msg_st *p_msg_data);

icf_return_t icf_al_send(IN icf_interface_info_st *p_interface_info,

                 icf_msg_st *p_msg,

                 icf_error_t *p_ecode);

#ifdef ICF_NAT_MEDIA_TRAVERSAL

/******************************************************************************

 * SOCKET HANDLLING FUNCTIIONS TO BE REGISTERED WITH GLB DATA OF NAT LIBRARY

 ******************************************************************************/
/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_open_udp_socket 
 * Description :  function to open a UDP socket
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_open_udp_socket( 
                                nat_ip_port_address_st *p_local_addr);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_close_udp_socket 
 * Description :  function to close a UDP socket
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_close_udp_socket( 
                                 nat_ip_port_address_st *p_local_addr);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_send_udp_data 
 * Description :  function to send data on UDP socket
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_send_udp_data(
                              nat_ip_port_address_st *p_local_addr,
                              nat_ip_port_address_st *p_dest_addr,
                              nat_void_t *p_msg_data);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_send_app_data 
 * Description :  function to send data to APP
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_send_app_data( 
                              nat_void_t *p_msg_data);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_send_mm_data 
 * Description :  function to send data to APP
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_send_mm_data(nat_ip_port_address_st *p_local_addr,
							 nat_ip_port_address_st * p_dest_addr,
                              nat_void_t *p_msg_data);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_start_timer 
 * Description :  function pointer to start a timer
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_start_timer(nat_void_t *p_timer_data, 
                                         nat_duration_t  timer_dur,
                                         nat_timer_id_t *p_timer_id);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_stop_timer 
 * Description :  function pointer to stop a timer
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_stop_timer(nat_timer_id_t timer_id);

#endif
#ifdef ICF_TRACE_ENABLE
icf_void_t icf_es_init_trace_db(icf_void_t);
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_ES_PROTOTYPES_H__ */    
