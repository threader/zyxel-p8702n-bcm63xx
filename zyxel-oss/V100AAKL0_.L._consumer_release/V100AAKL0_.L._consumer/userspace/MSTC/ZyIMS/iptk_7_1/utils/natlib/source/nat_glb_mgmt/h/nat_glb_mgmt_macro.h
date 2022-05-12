/*******************************************************************************
 *
 * 	FILENAME	: 	nat_glb_mgmt_macro.h
 *
 * 	DESCRIPTION	:	This file contains the macros used globally
 * 					across all modules of NAT.
 * 					
 * 	Revision History	:
 * 	DATE		       NAME		        REFERENCE		REASON
 * 	----------- ---------------      -----------------  ------------
 *  12-Jul-2006 	Anuradha Gupta      NAT LLD      Initial version.
 *  24-Nov-2006		Anuradha Gupta		LLD			 New macros added
 *
 * 	Copyright 2006,Aricent
 ******************************************************************************/

#ifndef __NAT_GLB_MGMT_MACRO_H__
#define __NAT_GLB_MGMT_MACRO_H__

/******************** STANDARD INCLUDE FILES *****************************/

/******************** USER INCLUDE FILES *********************************/

#include "nat_glb_mgmt_types.h" 
#include "nat_types.h"
#include "nat_common_prototypes.h"


/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the header information.
 *   
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 * */
 
 

extern "C"
{
#endif /* __cplusplus */

/******************** MACROS *********************************************/ 

#define NAT_GLB_ALLOC_CONTEXT(p_context_data_map,context_id,max_contexts)\
{\
	nat_uint8_t i = 0;\
\
	for(i =3; i < max_contexts + 3; i++)\
	{\
		if(p_context_data_map[i].is_free == NAT_TRUE)\
		{\
			p_context_data_map[i].is_free = NAT_FALSE;\
			break;\
		}\
	}\
	context_id = i;\
}

#define NAT_GLB_FIND_CONTEXT(p_context_data_map,app_context_id,context_id,max_contexts)\
{\
	nat_uint8_t i = 0;\
\
	for(i =3; i < max_contexts + 3; i++)\
	{\
		if(p_context_data_map[i].app_context_id == app_context_id)\
		{\
			break;\
		}\
	}\
	context_id = i;\
}

#define NAT_GLB_FIND_APP_CONTEXT_ON_ADDR(p_context_data_map,dest_addr,context_id,max_contexts)\
{\
	nat_uint8_t i = 0;\
    nat_error_t ecode = NAT_ERR_NONE;\
\
	for(i =3; i < max_contexts + 3; i++)\
	{\
        if(NAT_RETURN_SUCCESS == nat_cmn_compare_address(\
                    p_context_data_map[i].local_address,\
                    dest_addr,ecode))\
        {\
            break;\
        }\
	}\
	context_id = i;\
}

#define NAT_GLB_DEALLOC_CONTEXT(context_data_map)\
{\
	nat_port_memset((nat_void_t *)&context_data_map,0,sizeof(nat_context_data_map_st));\
	context_data_map.is_free = NAT_TRUE;\
}

#define NAT_GLB_FILL_CONTEXT(context_data_map,context_id,local_address,module_id,is_free,destination_id,source_id)\
{\
	context_data_map.app_context_id 	= context_id;\
	nat_port_memcpy(&context_data_map.local_address,&local_address,nat_port_sizeof(nat_ip_port_address_st));\
	context_data_map.module_id	 	= module_id;\
	context_data_map.is_free 		= is_free;\
	context_data_map.destination_id 	= destination_id;\
	context_data_map.source_id 		= source_id;\
}
	
#define NAT_GLB_FILL_CONTEXT_TRANSACTION_ID(p_context_data_map,transaction_id)\
{\
     nat_port_memcpy(&(p_context_data_map.transaction_id.octet),&(transaction_id.octet),\
           nat_port_sizeof(nat_uint128_t));\
}

#define NAT_GLB_GET_CONTEXT_ID_FRM_TRANSACTION_ID(p_context_data_map,max_contexts,transaction_id,context_id)\
{\
	nat_uint8_t i = 0,j=0;\
\
	for(i =3; i < max_contexts + 3 ; i++)\
	{\
		nat_uint128_t trans_id = p_context_data_map[i].transaction_id;\
\
		for(j=0;j<16;j++)\
		{\
			if(trans_id.octet[j] != transaction_id.octet[j])\
			{\
				break;\
			}\
                }\
		if(j==16)\
		{\
			break;\
		}\
	}\
	context_id = i;\
}
 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*  __NAT_GLB_MGMT_MACRO_H__ */
