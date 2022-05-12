/*******************************************************************************
*
* FILE NAME    : icf_adm_prototypes.h
*
* DESCRIPTION  : Contains the prototypes of functions used in ADM
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------       ------------   ---------      ---------------
*  22-Feb-2005  Mayank Rastogi    None           Created Origional
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_ADM_PROTOTYPES_H__
#define __ICF_ADM_PROTOTYPES_H__

#include "icf_common_types.h"

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


/*******************************************************************************
*
* FUNCTION NAME    : icf_adm_process_mesg
*
* DESCRIPTION      : This is the entry point function of ADM which dispatched the 
*                    messages to PA or SCM. All internal messages are dispatched
*                    SCM and all external messages are dispatched to PA.
*
* RETURNS          : (ICF_SUCCESS/ICF_FAILURE)
*
*******************************************************************************/

icf_return_t   icf_adm_process_mesg(
        INOUT   icf_internal_msg_st   *p_msg
);



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_ADM_PROTOTYPES_H__  */
