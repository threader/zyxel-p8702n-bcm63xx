/******************************************************************************
*                                    
* FILE NAME    : nat_glb_mgmt_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types used in NAT GLOBAL
*                MGMT module
*                                    
* Revision History :                            
*
*  DATE            NAME           REFERENCE       REASON        
* ------         ------------    ----------     ----------
* 16-Nov-2006	Anuradha Gupta	   LLD      	Initial version
*
* Copyright 2006, Aricent               
*                                    
*******************************************************************************/


#ifndef __NAT_GLB_MGMT_TYPES_H__
#define __NAT_GLB_MGMT_TYPES_H__


#include "nat_types.h"
#include "nat_glb_mgmt_defs.h"
#include "nat_common_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */




/* --------------------------------------------------------------------
 * Type Name   :  nat_conf_line_st
 * Description :  structure for storing line of confifuration file
 *               
 * ------------------------------------------------------------------------*/
typedef struct 
{
  	nat_uint8_t data[NAT_CONF_MAX_LINE_SIZE];
} nat_conf_line_st;

/* --------------------------------------------------------------------
 * Type Name   :  nat_conf_file_st
 * Description :  structure for storing for configuration file & 
 *                number of lines in it
 * ------------------------------------------------------------------------*/
typedef struct 
{
  	nat_conf_line_st  *line;
  	nat_uint8_t          count;
} nat_conf_file_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* __NAT_GLB_MGMT_TYPES_H__ */


