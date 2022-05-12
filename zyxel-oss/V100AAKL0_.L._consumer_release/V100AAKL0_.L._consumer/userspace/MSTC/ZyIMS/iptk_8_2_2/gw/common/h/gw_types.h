/*******************************************************************************
*
* FUNCTION : Global type definations for the HSS Gateway.
*
********************************************************************************
*
* FILENAME : gw_types.h
*
* DESCRIPTION : Contains global type definitions. This include file contains
*               general C language definitions that provide a common, platform
*               independent, environment for C language programming.
*               These definitions aid the portability of the C code which uses
*               these definitions.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* 19/1/2001  Sachin Mittal                    Initial
* 06/06/2001 Prashant Jain                    Prefixed all the basic data 
*                                             types with "gw_". Removed Tabs.
*                                             Added structure for Time Stamp.
*                                             Added structure _RPC_ASYNC_STATE
*                                             to remove compile time warning. 
* 27/07/2001 R. Ezhirpavai                    Removed ctrl-M's for compilation.
*
* 15/04/2002 Ravi Jindal        SPR-3928      To add Linux OS Port.
* 
* 06/12/2002 Ravi Jindal        SPR-5117      Compilation on 64bit machine.
* 12/07/2004 Navneet Gupta      SPR 6946      QNX por
*
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

/* Check to see if this file has been included already. */

#ifndef _GW_TYPES_H_
#define _GW_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef GW_INTEGER_IS_32
#ifdef GW_WINNT
#define GW_INTEGER_IS_32
#endif
#ifdef GW_VXWORKS
#define GW_INTEGER_IS_32
#endif
#ifdef GW_SOLARIS
#define GW_INTEGER_IS_32
#endif
#ifdef GW_LINUX
#define GW_INTEGER_IS_32
#endif
#ifdef GW_QNX
#define GW_INTEGER_IS_32
#endif
#endif

#include <stdarg.h>
#include "gw_pragma_defs.h"

/* Define standard data types. These definitions allow HSS products to
   perform in the same manner on different target platforms. */


/* For machines with 16-bit 'int's */
#ifdef GW_INTEGER_IS_16

/* Number of bytes : range */

typedef char                    gw_boolean;    
/* 1 : -128 - 127 (But will use only GW_TRUE and GW_FALSE */

typedef unsigned long int       gw_U32bit;        
/* 4 : 0 - 4,294,967,295 */

typedef unsigned int            gw_U16bit;
typedef unsigned char           gw_U8bit;         
/* 1 : 0 - 255 */

typedef long int                gw_S32bit;        
/* 4 : -2,147,483,648 - 2,147,483,648 */

typedef int                     gw_S16bit;
typedef char                    gw_S8bit;         
/* 1 : -128 - 127 */
typedef va_list                 gw_va_list_t;

#endif

/* For machines with 32-bit 'int's */
#ifdef GW_INTEGER_IS_32
/* Number of bytes : range */

typedef char                    gw_boolean;    
/* 1 : -128 - 127 (But will use only GW_TRUE and GW_FALSE */

typedef unsigned int            gw_U32bit;        
/* 4 : 0 - 4,294,967,295 */

typedef unsigned short int      gw_U16bit;
typedef unsigned char           gw_U8bit;         
/* 1 : 0 - 255 */

typedef int                     gw_S32bit;        
/* 4 : -2,147,483,648 - 2,147,483,648 */

typedef short int               gw_S16bit;
typedef char                    gw_S8bit;         
/* 1 : -128 - 127 */

typedef va_list                 gw_va_list_t;

#endif

#define GW_TRUE                 1
#define GW_FALSE                0

/* Either use GW_SUCCESS and GW_FAILURE 
 or use GW_ERROR and GW_OK. Do not inter-mix */

#define GW_SUCCESS              GW_TRUE
#define GW_FAILURE              GW_FALSE

#define GW_ERROR                -1
#define GW_OK                   0

/* Please avoid using NULL, use GW_NULL instead. */
#ifdef NULL
#undef NULL
#endif
#define NULL                    0L
#define GW_NULL                 0L

/* Use these structure assignment macros */
#ifdef GW_STRUCT_ASSIGNMENT_ENABLED

#define GW_STRUCT_ASSIGN(dest, src)\
            memcpy((char *)&(dest), (char *)&(src), sizeof(src))
#else

#define GW_STRUCT_ASSIGN(dest, src)\
            ((dest) = (src))
#endif

typedef gw_U8bit                gw_return_t;
typedef void                    gw_void_t;
typedef void *                  gw_pvoid_t;


typedef gw_U32bit               gw_len_t;
typedef gw_S16bit               gw_error_t;    
/* Should also use GW_ERROR or GW_OK */

typedef gw_U8bit *              gw_pbuffer_t;
typedef gw_U16bit               gw_msg_id_t;
typedef gw_U16bit               gw_module_id_t;
typedef gw_U32bit               gw_count_t;

typedef struct 
{
    gw_U32bit        ms4b;
    gw_U32bit        ls4b;

} gw_U64bit;

typedef gw_U64bit               gw_U64bit_t;

/* Timestamp related structure */
#define GW_TIME_STRN_SIZE       40

typedef struct 
{
    gw_U32bit        sec;
    gw_U32bit        nsec; /* nano seconds. */

}gw_time_stamp_t;
        

typedef struct
{
    gw_U8bit           date[GW_TIME_STRN_SIZE];

    /* The format of date field is yyyymmdd, i.e., first four characters are meant
       for year, next two for month and the last two for the day */

    gw_U8bit           time[GW_TIME_STRN_SIZE];
 
    /* The format of time field is hhmmssss, i.e., first two characters are meant
       for hour, next two for minutes and the last four for seconds */

}gw_time_notation_t;

typedef union
{
    gw_time_stamp_t    time_in_ns;
    gw_time_notation_t *p_utc_time_str;

}gw_time_stamp_ut;
 
/* for stretchable array X(num_val) implementation. 
This is done to remove any byte alignment problems*/
typedef struct
{
	gw_U8bit dummy;
} gw_stretch_array_t;

#ifdef GW_WINNT
    /* This is added to remove the warning (waring number 4115)*/
    /* The warning was coming due to this structure used in rpcasync.h file */
    struct _RPC_ASYNC_STATE;
#endif

#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif  /* end of _GW_TYPES_H_ */
