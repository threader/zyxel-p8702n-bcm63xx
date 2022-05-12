/*************************************************************************
 *     
 *     FILENAME           :  icf_port_defs.h
 *
 *     DESCRIPTION        : 
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         29/12/04    Aashish Suchdev     ICF LLD        Initial Version.
 *         06/01/05    Aashish Suchdev                       Compilation 
 *                                                           changes
 *         25/02/05    Jalaj Negi                            Linux Compilation
 *                                                           changes
 *         26/02/05    Jalaj Negi                            Definition of
 *                                                           GL_ECODE_MAP Macro
 *         06/05/05	   Vidyut Gupta						 port on windows 
 *         27/01/08    Tarun Gupta      IPTK Rel 8.0 LLD   Added constants for 
 *                                                        AKA handling.
 *         04/06/08    Tarun Gupta      SPR 18585        Runtime enable / disable
 *                                                       control for console traces
 *                                                       ,error traces
 *         12/08/09    Anuradha Gupta   SPR 19748        Added new macro for max
 *                                                       allowed memory allocation.
 *         26/08/09   Abhishe Dhammawat SPR 19987        Modified ICF_ALARM
 *                                                       for typecasting 
 *     Copyright 2009, Aricent.
 *
 *************************************************************************/
#ifndef __ICF_PORT_DEFS_H__
#define __ICF_PORT_DEFS_H__

#define ICF_PORT_CMN_TASK_ID 1

#define ICF_DELTA_PROCESSING_TIMER 1
#define ICF_MAX_TIMER_LIST_COUNT 1

/*#define ICF_MAX_NUM_POOL 5*/


#define ICF_GL_ECODE_MAP(p_icf_ecode, gl_ecode) \
{ \
    *p_icf_ecode = gl_ecode; \
}

/* add the ICF_PORT_WINDOWS flag --- vidyut */

/* CSR 1-5905629 , SPR 18548 : Clubbed both the error ALARMs and Trace printing
 * on console so that they are dirven as per similar flag sets */
 
/*ZyXEL porting*/ //When the ZyXEL_debug define , ICF_PRINT will be support
#define ZyXEL_debug
#if defined ZyXEL_debug	
#if	(defined ICF_TRACE_ENABLE) && (defined ICF_ERROR_ENABLE)
#if defined(ICF_PORT_WINCE)
#define ICF_PRINT(str) icf_port_logs_ind str
#else 
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)
#define ICF_PRINT(str) \
{ \
	if (ICF_NULL != p_persistent_glb_pdb) \
	{ \
		if ((ICF_TRUE == p_persistent_glb_pdb->trace_info.status) && \
			(ICF_TRUE == p_persistent_glb_pdb->error_info.status)) \
		{ \
			icf_port_filter_printf str; \
		} \
	} \
}
#endif
#define ICF_ALARM(str)   ICF_PRINT(((icf_uint8_t *)"\n[ICF_ALARM]"));ICF_PRINT(str)
#endif
#elif (defined ICF_TRACE_ENABLE)
#define ICF_ALARM(str)
#if defined(ICF_PORT_WINCE)
#define ICF_PRINT(str) icf_port_printf str
#else 
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)
#define ICF_PRINT(str) \
{ \
	if (ICF_NULL != p_persistent_glb_pdb) \
	{ \
		if (ICF_TRUE == p_persistent_glb_pdb->trace_info.status) \
		{ \
			icf_port_filter_printf str; \
		} \
	} \
}
#endif
#endif
#elif (defined ICF_ERROR_ENABLE)
#define ICF_PRINT(str) \
{\
}
#if defined(ICF_PORT_WINCE)
#define ICF_ALARM(str)   icf_port_printf("\n[ICF_ALARM]");icf_port_printf(str)
#else 
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)
#define ICF_ALARM(str) \
{ \
	if (ICF_NULL != p_persistent_glb_pdb) \
	{ \
		if (ICF_TRUE == p_persistent_glb_pdb->error_info.status) \
		{ \
			icf_port_filter_printf((icf_uint8_t *)"\n[ICF_ALARM]"); \
            icf_port_filter_printf str; \
		} \
	} \
}
#endif
#endif
#else
#define ICF_PRINT(str) \
{\
}
#define ICF_ALARM(str)
#endif
#else
#define ICF_PRINT(str)	
#define ICF_ALARM(str)	
#endif
/*ZyXEL porting end*/


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
#endif

#define ICF_MAX_SEND_BUFF_SIZE       65535

#define ICF_MAX_RECV_BUFF_SIZE       65535

#define ICF_TRACE_MAX_DB_VAL		2

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS))
#define ICF_SPRINTF(str) sprintf str;
#else
#define ICF_SPRINTF(str)
#endif

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS))
#define ICF_SSCANF(str) sscanf str;
#else
#define ICF_SPRINTF(str)
#endif

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS))
#include <stdio.h>
#define ICF_PORT_SIZEOF sizeof
#else
#define ICF_PORT_SIZEOF 
#endif

#ifdef ICF_DNS_LOOKUP_ENABLED
    #ifdef ICF_PORT_WINDOWS
        #define ICF_PATH_RESOLV_CONF "c:\\resolv.conf"
    #else
        #define ICF_PATH_RESOLV_CONF "/etc/resolv.conf"
    #endif
#endif
#ifdef ICF_GL_MM_DEBUG
#define ICF_GL_MM_DEBUG_PRINT(str) \
{ \
        printf str; \
}
#endif

/* The following fields are used in AKA challenge.These lengths
 * indicate the length of the fields in bytes.
 */
#define ICF_AKA_BLOCK_CIPHER_LEN     16
#define ICF_AKA_RES_LENGTH           16
#define ICF_AKA_AUTS_LENGTH          12
#define ICF_AKA_CIPHER_KEY_LENGTH    16  
#define ICF_AKA_INTEGRITY_KEY_LENGTH 16
#define ICF_AKA_FIELDS_IN_WRAPPER    5

/* THe minimum value used for passing in icf_port_random */
#define ICF_PORT_MINIMUM_SEED_FOR_RANDOM         1

/* The Maximum valu used for passing in icf_port_random */
#define ICF_PORT_MAXIMUM_SEED_FOR_RANDOM         65535

/*  The range for Numeric ascii values */
#define ICF_PORT_MIN_NUMERIC_ASCII               48
          
#define ICF_PORT_MAX_NUMERIC_ASCII               57

/*  The range for Upper case character set */
#define ICF_PORT_MIN_UPPER_ALPHA_ASCII           65

#define ICF_PORT_MAX_UPPER_ALPHA_ASCII           90

/* The range for Lower case characher set */
#define ICF_PORT_MIN_ALPHA_ASCII                 97

#define ICF_PORT_MAX_ALPHA_ASCII                 122

/* SPR 19748: The maximum memory allocation allowed else
 * for any size beyond the following macro
 * icf_port_memget will return failure. As of now it is 100 MB
 */ 
#define ICF_PORT_MAX_MEMORY_ALLOC_SIZE           100000000

#ifdef ICF_PORT_VXWORKS
#define ICF_PORT_NETWORK_INTERFACE_NAME "elPci0"
#endif

#endif /* __ICF_PORT_DEFS_H__ */

