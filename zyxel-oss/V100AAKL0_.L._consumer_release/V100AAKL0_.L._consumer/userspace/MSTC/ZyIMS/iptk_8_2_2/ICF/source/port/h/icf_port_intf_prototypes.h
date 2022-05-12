/*************************************************************************
 *
 * 	FILENAME	: 	icf_port_intf_prototypes.h
 *
 * 	DESCRIPTION	:This file contains the prototypes that are used for 
 * 	                port utility functions.
 * 					
 * 	Revision History	:
 *   DATE	NAME		REFERENCE			REASON
 *----------------------------------------------------------
 * 18-Feb-2005        Pankaj Negi      ICF LLD       Initial version.
 *Copyright 2006, Aricent .
 *
 *************************************************************************/

#ifndef __ICF_PORT_INTF_PROTOTYPES_H__
#define __ICF_PORT_INTF_PROTOTYPES_H__

/******************** STANDARD INCLUDE FILES *****************************/
#include <string.h>

/******************** USER INCLUDE FILES *********************************/
#include "icf_common_types.h"
#include "icf_port_struct.h"

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

/*
extern icf_return_t icf_port_receive(
                    INOUT icf_void_t *p_glb_port_info, 
                    IN icf_uint16_t timer,  in milliseconds
                    OUT icf_msg_list_st  *p_msg_list,
                    IN      icf_error_t          *p_ecode);
*/

/*
 * Following are used for port init
 */
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_init
*
*  DESCRIPTION    :   This function initializes the eshell and porting layer .
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
extern icf_return_t icf_port_init(
        INOUT icf_void_t        **p_p_port_data,
        INOUT icf_error_t      *p_ecode);

        
extern icf_return_t icf_mem_deinit(
         IN icf_void_t            *p_port_info,
         OUT icf_error_t *p_ecode );


extern icf_return_t icf_port_timer_deinit(
         IN icf_void_t            *p_port_info,
        OUT   icf_error_t    *p_ecode);

#ifndef __ICF_PORT_H__
/* these are some common function prorotypes in this file and also in file
 * icf_port.h, hence this precautionary check in the file */
        
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_memory_init
*
*  DESCRIPTION    :   This function initializes common memory pools.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
extern icf_return_t icf_port_memory_init(
         IN icf_void_t                *p_port_info,
         IN icf_memory_pool_table_st  *p_cmn_pool_data,
         INOUT icf_error_t              *p_ecode );


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_task_memory_init
*
*  DESCRIPTION    :   This function initializes  memory pools.
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE depending on
*                  whether the pointer was successfull or not.
*
*************************************************************************/
extern  icf_return_t
icf_port_task_memory_init(
         IN icf_void_t               *p_port_info,
         IN icf_memory_pool_table_st *p_task_pool_data,
         INOUT icf_error_t            *p_ecode);


/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_timer_init
 *
 *  DESCRIPTION     :    This function is the port function for initializing 
 *                       the timer. It attaches a call back function to the
 *                       same.
 * DESCRIPTION	:  	This function initializes the genlib timer library.
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
***************************************************************************/
extern  icf_return_t icf_port_timer_init(
         IN icf_void_t            *p_port_info,
         IN icf_timer_cb_fn_t     p_timer_cb_fn,
         IN icf_uint8_t           no_of_timer_list,
         IN icf_uint16_t          no_of_timers,
         INOUT icf_error_t        *p_ecode);

#endif /* __ICF_PORT_h__ */

/*
 * Following are for memory alloc/dealloc, timer start/stop
 */
#ifndef __ICF_PORT_PROTOTYPES_H__
extern icf_void_t* icf_port_memget(
        IN  icf_void_t           *p_port_info,
        IN  icf_uint32_t         size,
        IN  icf_pool_id_t        pool_id,
        OUT icf_error_t          *p_ecode);

extern icf_return_t icf_port_memfree(
        IN  icf_void_t       *p_port_info,
        IN  icf_void_t       *p_buf,
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
        IN    icf_timer_data_st    *p_buff,
        IN	   icf_uint32_t         duration,
        OUT   icf_timer_id_t       *p_timer_id,
        OUT   icf_error_t  	       *p_ecode);

extern icf_return_t icf_port_stop_timer(
        INOUT    icf_void_t     	  *p_glb_port_info,
        INOUT icf_timer_id_t     timer_id, 
        OUT   icf_error_t        *p_ecode);

#endif /* __ICF_PORT_PROTOTYPES_H__ */

/*
 * Following are for IPC interface porting
 */

extern icf_return_t icf_port_create_fifo(
        icf_uint8_t		*p_fifo_name,
        icf_uint32_t    permssions,
        icf_error_t    *p_ecode);

extern icf_return_t icf_port_open_fifo(
        icf_uint8_t      *p_fifo_name,
        icf_uint32_t     *p_fd,
        icf_uint32_t     permissions,
        icf_error_t      *p_ecode);

extern icf_return_t icf_port_open_channel(
        icf_uint32_t     port_num,
        icf_uint8_t      transport_type,
        icf_boolean_t    is_ipaddr_present,
        icf_uint8_t      *p_ipaddr,
        icf_uint32_t     *p_fd,
        icf_error_t      *p_ecode);

/*ZyXEL porting*/
extern icf_return_t icf_port_open_ipc_channel(
	icf_uint32_t *p_fd,
	icf_error_t *p_ecode,
	icf_string_st channel);
/*ZyXEL porting end*/
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

extern icf_return_t icf_port_tcp_send(
        icf_uint32_t     fd,
        icf_void_t       *p_msg,
        icf_uint32_t     msg_len,
        icf_error_t      *p_ecode);

extern icf_return_t icf_port_udp_recv(
        icf_interface_info_st  *p_interface_info,
        icf_void_t     *p_msg,
        icf_error_t    *p_ecode);

extern icf_return_t icf_port_fifo_recv(
        icf_uint32_t    fd,
        icf_void_t     *p_msg,
        icf_uint32_t   msg_len,
        icf_error_t    *p_ecode);


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_close_socket
*
*  DESCRIPTION    :   This function is used to close udp/tcp socket
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_void_t
icf_port_close_socket(
        icf_uint32_t             fd,
        icf_error_t              *p_ecode);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_PORT_INTF_PROTOTYPES_H__ */
