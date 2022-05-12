/*------------------------------------------------------------------------------
 *
 * File name        : icf_es_types.h
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
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __ICF_ES_TYPES_H__
#define __ICF_ES_TYPES_H__


#include "icf_es_defs.h"
#include "icf_port.h"
#ifdef ICF_PORT_VXWORKS
#include "icf_port_types.h"
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

/* --------------------------------------------------------------------------
 * Constants to describe function arguments
 * --------------------------------------------------------------------------*/
#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN   const
#endif

/* --------------------------------------------------------------------------
 * Constant to set and check for NULL
 * --------------------------------------------------------------------------*/
#define ICF_NULL 0

/* --------------------------------------------------------------------------
 * Constants to check the return value of functions
 * --------------------------------------------------------------------------*/
/*#define ICF_FAILURE   0
#define ICF_SUCCESS   1
*/


/* --------------------------------------------------------------------------
 * Constants to set and check the boolean varibles
 * --------------------------------------------------------------------------*/
/*#define ICF_TRUE 1
#define ICF_FALSE 0
*/
    

#define ICF_MAX_EXT_MODULES  30
 
    /* Data structures and hash define constants
     * needed to make a RAM drive on the VxWorks Platform*/
#ifdef ICF_PORT_VXWORKS 
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




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_ES_PROTOTYPES_H__ */    
