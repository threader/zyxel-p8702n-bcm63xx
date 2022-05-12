/*------------------------------------------------------------------------------
 *
 * File name        : icf_port_types.h
 *
 * Purpose          : This file contains all the port structures
 *                    or typedefs that are being used across the whole ICF.
 *
 * Revision History :
 *
 *   Date        Name           Ref#         Comments
 * --------      ------------   ----          ---------------
 * 23-Dec-2004  R. Ezhirpavai   ICF LLD      Initial Version
 * 04-Aug-2005	Rohit Aggarwal	ICF			 Changes for ICF compilation
 * 07-Dec-2005	Rohit Aggarwal	ICF			 Ported time_t type
 * 08-Apr-2008  Tarun Gupta     SPR 18355    Merged Windows Porting Changes
 * 18-Jul-2009  Rajiv Kumar     IPTK Rel8.2  Vxwork 5.4 Porting 
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_PORT_TYPES_H__
#define __ICF_PORT_TYPES_H__

#ifndef ICF_PORT_VXWORKS
#include <time.h>
#endif

#ifdef ICF_PORT_VXWORKS
#include "vxWorks.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "memLib.h"
#include "timers.h"
#include "inetLib.h"
#include "in.h"
#include "arpa/inet.h"
#include "msgQLib.h"
#include "tickLib.h"
#include "time.h"
#include "sys/times.h"
#include "sockLib.h"
#include "ctype.h"
#include "string.h"
#include "ioLib.h"
#include "ioctl.h"
#include "assert.h"
#include "hostLib.h"
#include "taskLib.h"
#include "kernelLib.h"
#include "errnoLib.h"
#include "resolvLib.h"
#include "netdb.h"
#include "types.h"
#include "mbuf.h"
#include "socket.h"
#include "socketvar.h"
#include "semaphore.h"
#include "signal.h"
#include "ramDrv.h"
#include "dosFsLib.h"
#include "ftpLib.h"
#include "ioLib.h"
#ifdef GL_IPV6
#include "netinet6/in6.h"
#include "netinet6/ip6.h"
#endif
#endif

#ifdef ICF_PORT_VXWORKS
#include "icf_port_defs.h"
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

/* --------------------------------------------------------------------
 * Type Name   :  icf_timer_id_t
 * Description :  A typedef for timer id stored for every timer 
 *                started. 
 * ------------------------------------------------------------------------*/
typedef icf_uint32_t  icf_timer_id_t;


/* --------------------------------------------------------------------
 * Type Name   :  icf_pool_id_t
 * Description :  A typedef for pool id used for memory allocation.
 *  
 * ------------------------------------------------------------------------*/
typedef icf_uint8_t  icf_pool_id_t;

/* --------------------------------------------------------------------
 * Type Name   :  icf_hash_table_id_t
 * Description :  A typedef for hash table id used for hash table indexing
 *                initialization and deletion.
 *  
 * ------------------------------------------------------------------------*/
/*typedef icf_uint8_t  icf_hash_table_id_t;*/


/* --------------------------------------------------------------------
 * Type Name   :  icf_time_t
 * Description :  A typedef for time_t basic type
 *  
 * ------------------------------------------------------------------------*/
typedef time_t  icf_time_t;

typedef struct sockaddr_in icf_sockaddr_in_st;

#ifdef ICF_IPV6_ENABLED 
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_SOLARIS)
typedef struct addrinfo icf_port_addrinfo;
typedef struct sockaddr_storage icf_port_sockaddr_storage;
#endif
#endif

#ifdef ICF_PORT_VXWORKS
typedef struct{
    icf_uint8_t ftp_server_ip[ICF_MAX_IP_ADDR_SIZE];
    icf_uint8_t login[ICF_MAX_NAME_SIZE];
    icf_uint8_t password[ICF_MAX_NAME_SIZE];
    icf_uint8_t config_path[ICF_MAX_CONFIG_FILE_PATH_LEN];
} icf_ftp_info_st;

#define ICF_MAX_CONFIG_FILE_PATH_LEN	200
#define ICF_CONF_FILE_NAME_SIZE			64

	/* CFG files to be read from host */ 
#define ICF_ICF_CFG				"icf.cfg"
#define ICF_MM_CFG				"mm.cfg"
#define ICF_WRAPPER_CFG			"wrapper.cfg"
#define ICF_DOMAINMAP_CFG       "domainmap.cfg"

#define ICF_VX_VOLUME_LABEL     "VXVOL:"
	/*Files to be made on target in which the host files are copied */
#define ICF_VX_ICF_CFG			"icf.cfg"
#define ICF_VX_MM_CFG			"mm.cfg"
#define ICF_VX_WRAPPER_CFG		"wrapper.cfg"
#define ICF_VX_DOMAINMAP_CFG    "domainmap.cfg"

#define ICF_FTP_SERVER_IP        "10.203.154.127" 
#define ICF_FTP_LOGIN            "icf"
#define ICF_FTP_PASSWORD         "icf123"
#define ICF_FTP_CONFIG_PATH      "/"

#define ICF_MAX_CONFIG_FILE_SIZE      2500
#define ICF_BYTES_PER_BLOCK           512
#define ICF_BLOCK_PER_TRACK           400
#define ICF_NUM_OF_BLOCKS             400
#define ICF_MAX_IP_ADDR_SIZE          16
#define ICF_MAX_NAME_SIZE             10
/*    
typedef struct{
    icf_uint8_t ftp_server_ip[ICF_MAX_IP_ADDR_SIZE];
    icf_uint8_t login[ICF_MAX_NAME_SIZE];
    icf_uint8_t password[ICF_MAX_NAME_SIZE];
    icf_uint8_t config_path[ICF_MAX_CONFIG_FILE_PATH_LEN];
} icf_ftp_info_st;
*/

typedef struct{
    icf_uint16_t   bytes_per_block;
    icf_uint16_t   blocks_per_track;
    icf_uint16_t   number_of_blocks_on_device;
} icf_ramdisk_param_st;

extern icf_ftp_info_st g_icf_ftp_info;
extern icf_ramdisk_param_st g_icf_ramdisk_param;
extern icf_boolean_t    g_icf_is_ramdisk_present;
#endif

#if defined ICF_PORT_WINDOWS || defined (ICF_PORT_VXWORKS)
    typedef int socklen_t;
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_PORT_TYPES_H__ */
