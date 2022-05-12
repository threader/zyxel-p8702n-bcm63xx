/*************************************************************************
 *
 *     FILENAME           :  icf_port_macros.h
 *
 *     DESCRIPTION        :  This file holds the mapping of system macros
 *                           being used by ICF to actual OS macros
 *
 *     Revision History   :
 *     DATE        NAME                REFERENCE     REASON
 *     ----------------------------------------------------------
 *     29/01/05    Aashish Suchdev     ICF LLD    Initial Version.
 *     07/12/2005  Rohit Aggarwal      ICF 4.0 CDD   Ported time_t
 *
 *     Copyright 2006, Aricent.
 *
 *************************************************************************/

#ifndef __ICF_PORT_MACROS_H__
#define __ICF_PORT_MACROS_H__


/*#if (defined ICF_PORT_WINDOWS || defined ICF_PORT_SYMBIAN || defined ICF_PORT_WINCE) */
#include <time.h>
/*#endif*/

#if (defined ICF_PORT_SOLARIS || defined ICF_PORT_LINUX)
#include <sys/types.h>
#include <sys/time.h>
#include <utime.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#include "icf_port_defs.h"
#include "icf_common_defs.h"

/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */

#ifdef ICF_PORT_WINDOWS
#define ICF_ISXDIGIT(var) \
	(((var >= 48) && (var <= 57))|| \
	((var >= 65) && (var <= 70))|| \
	((var >= 'a') && (var <= 'f')))
#else
#define ICF_ISXDIGIT(var) isxdigit(var)
#endif

#define ICF_PORT_FD_ZERO(x) FD_ZERO(x)

#define ICF_PORT_FD_ISSET(x,y)  FD_ISSET(x,y)

#define ICF_PORT_FD_SET(x,y)  FD_SET(x,y)

/*------------------------------------------------------
 *  Macros used for License Management
 *---------------------------------------------------*/
#define ICF_PORT_LOCALTIME    localtime
#define ICF_PORT_GMTIME       gmtime
#define ICF_PORT_MKTIME       mktime
#define ICF_PORT_SPRINTF(c)   sprintf c
#define ICF_PORT_ATOL         atol
#define ICF_PORT_STRTOUL      strtoul
#define ICF_PORT_FFLUSH       fflush
#define ICF_PORT_STDIN        stdin
#define ICF_PORT_CTIME        ctime
#define ICF_PORT_FPRINTF      fprintf
#define ICF_PORT_SSCANF       sscanf
#define ICF_PORT_FSEEK        fseek
#define ICF_PORT_FWRITE       fwrite
#define ICF_PORT_FREAD        fread
#define ICF_PORT_GETHOSTID    gethostid
#define ICF_PORT_STAT         stat
#define ICF_PORT_RAND         rand
#define ICF_PORT_SLEEP        sleep
#define ICF_PORT_WCSTOMBS     wcstombs 


#if (defined ICF_PORT_SOLARIS || defined ICF_PORT_LINUX || defined ICF_PORT_WINDOWS ) 

#define ICF_PORT_CLRSCR system("clear")

#define ICF_PORT_FOPEN(p_fd, file_name, mode, ret_val)\
    {\
        p_fd = fopen(file_name, mode);\
        if( ICF_NULL != p_fd )\
            {\
                ret_val = ICF_SUCCESS;\
            }\
        else\
        {\
            ret_val = ICF_FAILURE;\
         }\
    }
 
#define ICF_PORT_FCLOSE(p_fd)\
    {\
        fclose(p_fd);\
    }

#define ICF_PORT_FGETS(p_line, length,p_fd,\
                ret_val)\
    {\
        ret_val = fgets(p_line, length,(icf_file_t *)p_fd);\
    }  


#ifndef ICF_PORT_WINCE
#define ICF_PORT_TIME(param, result, ret_val)\
    {\
        result = time(ICF_NULL);\
        if((time_t)-1 == result)\
            {\
                ret_val = ICF_FAILURE;\
             }\
        else\
        {\
            ret_val = ICF_SUCCESS;\
        }\
    }
#endif
	

#define ICF_PORT_GETCHAR(in_char, ret_val)\
        {\
            in_char = getchar();\
            ret_val = ICF_SUCCESS;\
        }
        
        
#define ICF_PORT_EXIT(ret_val) exit(ret_val)  
        
#else 

#define ICF_PORT_CLRSCR 

#define ICF_PORT_FOPEN(p_fd, file_name, mode, ret_val )\
    {\
        p_fd = ICF_NULL;\
        ret_val = ICF_FAILURE;\
    }  

 
#define ICF_PORT_FCLOSE(p_fd)\
    {\
        fclose(p_fd);\
    }

#define ICF_PORT_FGETS(p_line, length,p_fd,\
                ret_val)\
    {\
        ret_val = ICF_NULL;\
    } 

#if ((defined ICF_PORT_WINDOWS && !defined ICF_PORT_WINCE ) || defined ICF_PORT_SYMBIAN)
#define ICF_PORT_TIME(param, result, ret_val)\
    {\
        result = time(ICF_NULL);\
        if((time_t)-1 == result)\
            {\
                ret_val = ICF_FAILURE;\
             }\
        else\
        {\
            ret_val = ICF_SUCCESS;\
        }\
    }
#else 
#define ICF_PORT_TIME(param, result, ret_val)\
    {\
        result = (time_t)-1; \
        ret_val = ICF_FAILURE;\
    }
#endif /* port-windows or port-symbian */
            
#define ICF_PORT_EXIT(ret_val) 

#define ICF_PORT_GETCHAR(in_char, ret_val)\
        {\
            ret_val = ICF_FAILURE;\
        }



#endif /* For ifdef PORT_SOLARIS || PORT_LINUX */

#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_PORT_APP_TRACE
 * Description      : New Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#define ICF_PORT_APP_TRACE(p_port_pdb, trace_id)   \
{\
    icf_port_app_trace(p_port_pdb, (icf_trace_id_t)(trace_id));\
}
#else
#define ICF_PORT_APP_TRACE(p_port_pdb, trace_id)

#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_IPV6_ENABLED
/* This Macro indicates IP String is IPv6 address */
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_UNIX) || defined(ICF_PORT_VXWORKS)
#define ICF_PORT_IPV6_FAMILY		AF_INET6
#endif
/* This Macro indicates IP String is IPv4 address */
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_UNIX) || defined(ICF_PORT_VXWORKS)
#define ICF_PORT_IPV4_FAMILY		AF_INET
#endif
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __ICF_PORT_MACROS_H__ */
