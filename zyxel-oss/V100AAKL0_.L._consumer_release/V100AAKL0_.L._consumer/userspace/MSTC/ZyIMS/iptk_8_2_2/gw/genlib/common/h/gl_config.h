/******************************************************************************
*
* FUNCTION    : Generic Libraries configuration parameters.
*
*******************************************************************************
*
* FILENAME    : gl_config.h
*
* DESCRIPTION : This file contains the configuration parameters which need to be
*               set by the user before compiling the generic libraries
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* 19/1/2001  Sachin Mittal                   Initial
* 18/08/2001 Prashant Jain   SPR 2276        Compiling with CC on Solaris.
* 17/11/2001 Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                            cosmmetics of file.
* 22/11/2001 Ravi Jindal     SPR-3225        Add Congestion Management 
*                                            modules related structures.
* 30/11/2001 Ravi Jindal     SPR-2788        Packaging of Genlib-Code.
*
* 02/01/2001 Ravi Jindal     SPR-3427        Adding GL_INVALID_TASK_ID.
* 
* 10/03/2002 Ravi Jindal     SPR-3656        Enhancements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*
******************************************************************************/

/*
 * Check to see if this file has been included already.
 */

#ifndef _GL_CONFIG_H_
#define _GL_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*************************************************************************/

 /**************************************************** 
 * Complile time options for all Managers.
 ****************************************************/
    
/*
 *
#define	GL_STATIC_VALIDATION_ENABLED

#define GL_MM_ADD_DEBUGGER 

#define GL_MM_ADD_AUDIT_MGT

#define GL_MM_ADD_MEM_OVERW_OPR

#define GL_HTM_ENABLE_KEY_REF_COUNT

#define GL_MQM_BLOCKING_ON

#define GL_STATS_ENABLED

#define GL_TOTAL_MT_SAFE
 *
 */
    
/****************************************************************************/

    
/****************************************************************************/

    
/**************************************************** 
 * Configuration parameters of Common to all Managers.
 ****************************************************/
    
    
    /*
     * Generic Libraries error codes base.
     */
#define GL_ERR_BASE                     0

    /*
     * The invalid task-id used as default value in GL.
     */
#define GL_INVALID_TASK_ID              0xffffffff

    
/****************************************************************************/

    
/**************************************************** 
 * Configuration parameters of MEMORY MANAGER.
 ****************************************************/

    /*
     * Affects initial memory requirement of GL 
     * Maximum number of common memory pools supported
     * permissible value upto 200
     */
#define GL_MM_MAX_COMMON_POOL_QTY       30 
    
    /* 
     * Affects initial memory requirement of GL 
     * Maximum number of task specific memory pools supported
     * permissible value upto 200
     */
#define GL_MM_MAX_TASK_POOL_QTY         1000 

    /*
     * Maximum size of the buffer in bytes a pool can have
     * permissible value system dependent.
     */
#define GL_MM_MAX_BUF_SIZE              100000000
    
    /*
     * Maximum quantity of buffers a pool can have
     * permissible value system dependent.
     */
#define GL_MM_MAX_NO_OF_BUF             25000000

    /*
     * Maximum number of congestion region supported.
     */ 
#define GL_MM_CNGST_MGMT_MAX          1 

    /*
     * Value of Check Bits Used at Start and End of Buffer Region.
     */
#define GL_MM_CHECK_BIT                137 /* binary 10001001 */

    /*
     * Max number of Function address to be stored from Syatem Stack.
     */ 
#define GL_MM_STK_TR_DEPTH              20
    
    /*
     * Max number of Pool Usage regions, used to calculate extent of 
     * internal fragmentaion on different levels.
     */ 
#define GL_MM_POOL_USAGE_MAX_REGIONS    5 
    
    /*
     * Portable Macro Used to get the debugging information.
     */ 
#define GL_MM_DEBUG_INFO_TRACE(x)       printf x

    
/****************************************************************************/

    
/**************************************************** 
 * Configuration parameters of TIMER MANAGER.
 ****************************************************/

    /*
     * Affects initial memory requirement of GL 
     * Maximum number of timer lists supported
     * permissible value upto 200.
     */
#define GL_TM_MAX_TIMER_LIST            10 
    
    /*
     * Maximum number of active timers in a timer list
     * permissible value upto 0xFFFFFFFF.
     */
#define GL_TM_MAX_TIMER_QTY             4000 
    
    /*
     * Value determining timer resolution ,i.e., 
     * the minimum timer duration would be 10 milliseconds.
     */
#define GL_TM_TIMER_RESOLUTION          1

    /*
     * Maximum value of system timestamp returned in seconds.
     * After that value timestamp will roll over.
     */
#define GL_TM_MAX_SYSTEM_TIME_IN_SECS   0xFFFFFFFF

    
/****************************************************************************/

/******** START: CHANGES FOR NTP FIX ***********/

	/*
	 * This value is in sync with the timer of the application
	 * using GENLIB. This is used as a default value and the 
	 * application is advised to use the exposed function to 
	 * initialise the structure according to its need.
	 */
#define GL_TIMER_STEP_IN_MS			200	

	/*
	 * This value is assumed to be the logical application  
	 * processing time. This is used as a default value and  
	 * the application is advised to use the exposed function 
	 * to initialise the structure according to its need.
	 */
#define GL_TIMER_DELTA_IN_MS	  		1

/*********** END: CHANGES FOR NTP *************/ 
    
/**************************************************** 
 * Configuration parameters of LIST MANAGER.
 ****************************************************/

    /* 
     * Affects initial memory requirement of GENLIB 
     * Maximum number of Link lists supported.
     * permissible value upto 200.
     */
#define GL_LLM_MAX_LIST                  10 
    
    /* 
     * Affects initial memory requirement of GENLIB 
     * Maximum number of Work lists supported.
     * permissible value upto 200.
     */
#define GL_WLM_MAX_LIST                  10 
    
    /*
     * The maximum number of elements in the link-list
     * permisible value upto 0x7FFFFFFF (as elem_qty is U32bit). 
     */
#define GL_LLM_MAX_ELEMENTS             0x7FFFFFFF
    
    /*
     * The maximum number of elements in the link-list
     * permisible value upto 0x7FFFFFFF (as new_index is U32bit). 
     */
#define GL_WLM_MAX_RECORDS              0xFFFF

    /*
     * It is Assumed that after getting GL_WLM_MAX_INDEX_VALUE
     * the index value will rap up and no node with rapped value 
     * will be in the Work list.
     * e.g. If the index of last added node = GL_WLM_MAX_INDEX_VALUE
     * then next node will get index = 1.
     * So, it is advisable to keep this value as large as possible.
     * permisible value upto 0xFFFFFFFF (as new_index is U32bit).
     * The value of GLWLM_MAX_INDEX_VALUE must be > GL_WLM_MAX_RECORDS.
     */ 
#define GL_WLM_MAX_INDEX_VALUE           0x3b9ac9ff /* = decimal 999999999 */

    
/****************************************************************************/

    
/**************************************************** 
 * Configuration parameters of HASH TABLE MANAGER.
 ****************************************************/

    /* 
     * Affects initial memory requirement of GL 
     * Maximum number of hash tables supported
     * permissible value upto 200.
     */
#define GL_HTM_MAX_HASH_TABLE          50 

    /*
     * Hash defines value for the Invalid hash table-id value.
     * It is used to check the value of hash-id parameter.
     */
#define GL_HTM_INVALID_TABLE_ID         0xffff

    
/****************************************************************************/

    
/**************************************************** 
 * Configuration parameters of MESSAGE QUEUE MANAGER.
 ****************************************************/

    /* 
     * Affects initial memory requirement of GL 
     * Maximum number of message queues supported 
     * permissible value upto 200.
     */
#define GL_MQM_MAX_MESG_Q               10 

    /*
     * Maximum Number of Messages in Any Message Queue.
     * Valid value is in range of gw_U32bit.
     */ 
#define GL_MQM_MAX_MESG_QTY             1024  

    /*
     * Maximum Size of Messages in Any Message Queue.
     * Valid value is in range of gw_U32bit.
     */ 
#define GL_MQM_MAX_MESG_SIZE            1024  

    /*
     * Maximum Number of Priority Level in Any Message Queue.
     * Valid value is in range of gw_U8bit.
     * (0) is the Highest Priority Level and
     * (5) is the Lowest Priority Level.
     */ 
#define GL_MQM_MAX_PRIORITY_LEVELS      5

    
/****************************************************************************/

    
/**************************************************** 
 * Configuration parameters of SHARED MEMORY MANAGER.
 ****************************************************/

    /* 
     * Affects initial memory requirement of GL 
     * Maximum number of shared memory instances.
     * Valid Range from 1-65535 as (gw_U16bit).
     */
#define GL_MAX_SHM                      50 
    
    /* 
     * Affects initial memory requirement of GL 
     * Maximum permissible size of the shared memory name.
     */ 
#define GL_SHM_MAX_NAME_SIZE            20 

    
/****************************************************************************/

    
/**************************************************** 
 * Configuration parameters of String Identifier 
 * Mapping MANAGER.
 ****************************************************/
    
    /*
     * Maximum number of Database, that can be created.
     * range is any U16bit number.
     */ 
#define GL_SIM_MAX_DB                      50

    
/****************************************************************************/

    
    
/**************************************************** 
 * Configuration parameters of STRING MANAGER.
 ****************************************************/

    /*
     * The base value for '0', used as a reference value in the functions.
     */
#define GL_ZERO_ASCII_VAL               48

    /*
     * Maximum nyumber of characters that can be present in a String.
     * Range value is gw_S32bit.
     */ 
#define GL_STM_MAX_CHARS_IN_STRING       0x7fffffff

    
/****************************************************************************/

    
/****************************************************************************/

    
/**************************************************** 
 * Macros for Malloc and Free system calls.
 ****************************************************/

#define SYS_MALLOC(x)           malloc(x)
#define SYS_FREE(x)             free(x)

    
/****************************************************************************/


#ifdef __cplusplus
}
#endif

#endif

