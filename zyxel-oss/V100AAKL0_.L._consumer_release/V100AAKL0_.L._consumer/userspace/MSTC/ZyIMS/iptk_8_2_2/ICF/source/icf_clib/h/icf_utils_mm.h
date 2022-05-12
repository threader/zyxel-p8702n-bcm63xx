
/*****************************************************************************
*										
* FILE NAME    :icf_utils_mm.h
*										
* DESCRIPTION  :The file contains utility functions used by packing/
*               unpacking routines
*										
* Revision History :				
*				
*   DATE             NAME               REFERENCE              REASON
* -----------    ---------------      -------------      ------------------
* 05-JUL-2005     Raveen Sharma	       ICF REL 6.0		       Initial
*   										
* Copyright 2006, Aricent.   					
*										
*****************************************************************************/



#ifndef __ICF_UTILS_MM_H__
#define __ICF_UTILS_MM_H__



/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

icf_return_t icf_free_mm_msg(icf_msg_st* p_msg);
	
icf_void_t icf_free_list(icf_list_st* p_list);

icf_void_t icf_free_modify_stream(icf_list_st* p_list);

icf_void_t icf_free_req_stream(icf_list_st* p_list);

icf_void_t icf_free_resp_stream(icf_list_st* p_list);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /*__ICF_UTILS_MM_H__*/

