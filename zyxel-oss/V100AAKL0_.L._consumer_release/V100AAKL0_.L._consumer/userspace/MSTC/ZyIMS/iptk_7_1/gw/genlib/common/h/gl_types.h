/*******************************************************************************
*
* FUNCTION : Generic Libraries type definitions.
*
********************************************************************************
*
* FILENAME : gl_types.h
*
* DESCRIPTION : Contains global type definitions.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* 19/1/2001  Sachin Mittal                    Initial
* 18/08/2001 Prashant Jain   SPR 2276         Compiling with CC on Solaris.
*
* 04/09/2001 Prashant Jain   SPR-2560         Adding OSE Operating System Support.
* 18/10/2001 Ravi Jindal     SPR-2788         Packaging of Genlib-Code.
* 19/10/2001 Ravi Jindal     SPR-2802         Adding Pre-ANSI-C features.
* 24/10/2001 Ravi Jindal     SPR-2791         typedef the timestamp related
*                                             data structures.
* 06/11/2001 Ravi Jindal        -             Add gl_pool_id_t. 
* 17/11/2001 Ravi Jindal     SPR-3107         Incoprate the defects and change
*                                             cosmmetics of file.
* 30/11/2001 Ravi Jindal     SPR-2788         Re-Packaging of Genlib-Code.
*
* 15/04/2002 Ravi Jindal     SPR-3928         To add Linux OS Port.
* 
* 12/07/2004 B Venkat        SPR 6946         QNX port
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

/* 
 * Check to see if this file has been included already.
 */

#ifndef _GL_TYPES_H_
#define _GL_TYPES_H_

#include "gw_types.h"
#include "gw_macros.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef gw_boolean          gl_boolean_t;
typedef gw_boolean          gl_boolean;
typedef gw_error_t          gl_error_t;
typedef gw_return_t         gl_return_t;
typedef gw_pvoid_t          gl_pvoid_t;


#define GL_TRUE             GW_TRUE
#define GL_FALSE            GW_FALSE

#define GL_SUCCESS          GW_SUCCESS
#define GL_FAILURE          GW_FAILURE

#define GL_ERROR            GW_ERROR
#define GL_OK               GW_OK

#define GL_NULL             GW_NULL

#ifndef GL_MT_ENABLED
#ifdef  GW_MT_ENABLED
#define GL_MT_ENABLED       GW_MT_ENABLED
#endif
#endif


#ifdef GL_TOTAL_MT_SAFE
#ifndef GL_MT_ENABLED
#undef GL_TOTAL_MT_SAFE
#endif
#endif


#ifndef GL_WINNT
#ifdef  GW_WINNT
#define GL_WINNT            GW_WINNT
#endif
#endif

#ifndef GL_VXWORKS
#ifdef  GW_VXWORKS
#define GL_VXWORKS          GW_VXWORKS
#endif
#endif

#ifndef GL_SOLARIS
#ifdef  GW_SOLARIS
#define GL_SOLARIS          GW_SOLARIS
#endif
#endif

#ifndef GL_LINUX
#ifdef  GW_LINUX
#define GL_LINUX            GW_LINUX
#endif
#endif

#ifndef GL_QNX
#ifdef  GW_QNX
#define GL_QNX            GW_QNX
#endif
#endif

#ifndef GL_OSE
#ifdef  GW_OSE
#define GL_OSE              GW_OSE
#endif
#endif


typedef struct
{
    gw_U32bit    sec; 
    /* 
     *  Seconds elasped since 1 January 1970.
     */
    gw_U32bit    msec; 
    /* 
     * Miliseconds elasped after the above number of seconds.
     */
} gl_time_t;

/*
 * Typedef for Pool identifier.
 */
typedef gw_U16bit           gl_pool_id_t;

/*
 * Typedef for timer identifier.
 */ 
typedef gw_U32bit           gl_timer_id_t;

/*
 * Id for the timer list.
 */
typedef gw_U16bit           gl_timer_list_t; 

/*
 * Typedef for message queue identifier.
 */
typedef gw_S32bit           gl_mq_id_t;

/*
 * Typedef for identifier of the hash table.
 */
typedef gw_U16bit           gl_hash_table_t;

/*
 * Typedef for identifier of the list linked list or work list.
 */
typedef gw_U16bit           gl_list_t; 

/*
 * Typedef for identifier of work-list index.
 */
typedef gw_U32bit	    gl_wl_index_t;


/*
 * The Termination Identifier Database-id type,
 * As more than one termdb are supported.
 */
typedef gw_U16bit           gl_sim_db_id_t;


/*
 * Define the structure that Trie Table will keep as Value_type.
 */
/*
 * This is termination ID in Media Gateway product.
 */
typedef gw_U32bit           Value_type;

/*
 * Typedef for idefntifier of shared-memory.
 */
typedef gw_U16bit           gl_shmem_id_t;


typedef gw_time_notation_t  gl_time_notation_t;

typedef gw_time_stamp_t     gl_time_stamp_t;

typedef gw_time_stamp_ut    gl_time_stamp_ut;


#ifdef __cplusplus
}
#endif

#endif  /*
         * End of _GL_TYPES_H_ 
         */
