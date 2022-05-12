#if 0 
*------------------------------------------------------------------------------

 *

 * File name        : icf_cc_macro.h

 *

 * Purpose          : This file contains the macro definitions.

 *

 * Revision History :

 *

 * Date         Name            Ref#                Comments

 * --------     ------------    ----                ---------------

 * 15-Dec-2004  Rohit Aggarwal  Non Service LLD     Initial

 * 22-Dec-2004  Rohit Aggarwal                      Added macro for ecode

 * 23-Dec-2004  Rohit Aggarwal                      Corrected certain #defines

 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes

 * 25-Dec-2004  Pankaj, Rohit                       Compilation fixes

 * 28-Dec-2004  Rohit Aggarwal                      Added trace compilation flag

 * 

 *

 * Copyright (c) 2006, Aricent.

 *----------------------------------------------------------------------------*/





#ifndef __ICF_CC_MACRO_H__

#define __ICF_CC_MACRO_H__





/*#include "icf_macro.h"*/





/*

 * Determine if a C++ compiler is being used. If so, ensure that standard C is 

 * used to process the header information.

 */

#ifdef __cplusplus

/* Yes, C++ compiler is present. Use standard C */

extern "C"

{

#endif /* __cplusplus */





/*------------------------------------------------------------------------------

 *

 * Macro Name       : ICF_NM_ACS_INFO_DEL

 * Description      : Macro for deleting network access information.

 *

 *----------------------------------------------------------------------------*/



#define ICF_NM_ACS_INFO_DEL(p_nm_ref,p_acc_info,p_ecode)					\
{                                                                   \
	if(ICF_NULL != p_acc_info->p_apn_list)						\
	ICF_STATIC_MEMFREE(p_nm_ref->p_icf_glb_ptr,				\
			p_acc_info->p_apn_list,									\
			p_ecode,ret_val);				\
	if( 0 != p_acc_info->nofpbuids)								\
	ICF_STATIC_MEMFREE(p_nm_ref->p_icf_glb_ptr,				\
			p_acc_info->p_public_usr_id,							\
			p_ecode,ret_val);				\
	if( 0 != p_acc_info->nofprids)								\
	ICF_STATIC_MEMFREE(p_nm_ref->p_icf_glb_ptr,				\
			p_acc_info->p_private_usr_id,							\
			p_ecode,ret_val);				\
}	 

																			



/*------------------------------------------------------------------------------

 *

 * Macro Name       : ICF_NM_PDP_INFO_DEL

 * Description      : Macro for deleting active PDP contexts information.

 *

 *----------------------------------------------------------------------------*/

#define 	ICF_NM_PDP_INFO_DEL(p_nm_ref,p_ecode)						\
{																	\
	if(0 <	p_nm_ref->p_n_info_list->nofsec_pdp_cntxt)			\
	ICF_STATIC_MEMFREE(p_nm_ref->p_icf_glb_ptr,				\
			p_nm_ref->p_n_info_list->p_sec_c_list,					\
			p_ecode,ret_val);				\
}





 /*------------------------------------------------------------------------------

 *

 * Macro Name       : ICF_NM_ASY_INFO_DEL

 * Description      : Macro for deleting asynchronous requests information.

 *

 *----------------------------------------------------------------------------*/

#define ICF_NM_ASY_INFO_DEL(p_nm_ref,p_ecode)							\
{																	\
	if(0 <	p_nm_ref->p_n_info_list->nofsec_pdp_cntxt)			\
	ICF_STATIC_MEMFREE(p_nm_ref->p_icf_glb_ptr,				\
			p_nm_ref->p_req_list,									\
			p_ecode,ret_val);				\
}

#ifdef __cplusplus

}

#endif /* __cplusplus */



#endif /* End of ifndef __ICF_CC_MACRO_H__ */

#endif
