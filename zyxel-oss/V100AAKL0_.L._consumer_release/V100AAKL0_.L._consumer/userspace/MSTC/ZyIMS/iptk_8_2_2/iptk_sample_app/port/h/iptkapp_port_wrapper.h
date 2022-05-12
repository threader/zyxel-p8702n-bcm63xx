/*****************************************************************************
 **
 ** FILENAME:           iptkapp_port_wrapper.h
 **
 ** DESCRIPTION:        This file contains the prototypes for all the wrapper
 **                    functions 
 **
 ** DATE                        NAME            REFERENCE               REASON
 ** ----                        ----            ---------               ------
 **   06-DEC-07            vipul varshney                       Initial Creation
 *****************************************************************************
 **                     Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/

#ifndef __IPTKAPP_PORT_WRAPPER_H__
#define __IPTKAPP_PORT_WRAPPER_H__

#include "icf_types.h"
#include "icf_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#if (defined ICF_PORT_SOLARIS || defined ICF_PORT_LINUX)
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>
#include <sys/msg.h>
#endif
#include <errno.h>
#include <sys/types.h>
#ifdef  ICF_PORT_WINDOWS
#include <winsock2.h>
#endif



#include <time.h>
#include <limits.h>
#include <fcntl.h>


#include <signal.h>


#define IPTKAPP_PRINTF printf
#define IPTKAPP_SCANF scanf
#define IPTKAPP_SSCANF sscanf
#define IPTKAPP_SPRINTF sprintf
#define IPTKAPP_FPRINTF fprintf
#define IPTKAPP_NULL NULL

typedef icf_return_t iptkapp_return_t;
typedef icf_error_t  iptkapp_error_t;

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_malloc
*
*  DESCRIPTION    : This function is the wrapper for malloc
*
*  RETURNS:       : icf_void_t pointer to the allocated memory
*************************************************************************/
icf_void_t * iptkapp_port_malloc(icf_uint32_t size);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_free
*
*  DESCRIPTION    : This function is the wrapper for free
*
*  RETURNS:       : no return value
*************************************************************************/
icf_void_t iptkapp_port_free(icf_void_t * ptr);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_memcpy
*
*  DESCRIPTION    : This function is the wrapper for memcpy 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
icf_void_t * iptkapp_port_memcpy(icf_void_t *p_destination, \
        const icf_void_t *p_source,
						 icf_uint32_t num_bytes_to_copy);



/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_memset
*
*  DESCRIPTION    : This function is the wrapper for memset
*
*  RETURNS:       : return the pointer(ptr), given in the argument
*************************************************************************/
icf_void_t * iptkapp_port_memset( icf_void_t * , icf_uint32_t , icf_uint32_t );


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strcpy
*
*  DESCRIPTION    : This function is the wrapper for strcpy
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
icf_int8_t * iptkapp_port_strcpy(icf_int8_t *p_destination, 
				const icf_int8_t *p_source);

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strcat
*
*  DESCRIPTION    : This function is the wrapper for strcat 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
icf_int8_t * iptkapp_port_strcat(icf_int8_t *p_destination,
                                  const icf_int8_t *p_source);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strncpy
*
*  DESCRIPTION    : This function is the wrapper for strncpy
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
icf_int8_t * iptkapp_port_strncpy(icf_int8_t *p_destination, 
				const icf_int8_t *p_source,
                icf_uint32_t max_num_bytes);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strtok
*
*  DESCRIPTION    : This function is the wrapper for strtok
*
*  RETURNS:       : pointer to the last token found in string
*************************************************************************/
icf_int8_t * iptkapp_port_strtok(icf_int8_t * p_source, 
				const icf_int8_t * p_delimiters);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strcmp
*
*  DESCRIPTION    : This function is the wrapper for strcmp
*
*  RETURNS:       : It returns an integer less than, equal to, or greater
*                   than zero if  string1 is found, respectively, to be less
*                   than, to match, or be greater than string2.
*************************************************************************/
icf_uint32_t iptkapp_port_strcmp(const icf_int8_t *p_string_1, 
			const icf_int8_t *p_string_2);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strncmp
*
*  DESCRIPTION    : This function is the wrapper for strncmp
*
*  RETURNS:       : It returns an integer less than, equal to, or greater
*                   than zero if first n charaters of  string1  is  found,
*                   respectively, to be less than, to match, or be greater
*                   than string2.
*************************************************************************/
icf_uint32_t iptkapp_port_strncmp(const icf_int8_t *p_string_1, 
			const icf_int8_t *p_string_2);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strlen
*
*  DESCRIPTION    : This function is the wrapper for strlen
*
*  RETURNS:       : It returns the number of characters in string
*************************************************************************/
 icf_uint32_t iptkapp_port_strlen(const icf_int8_t *p_string);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_fgetc
*
*  DESCRIPTION    : This function is the wrapper for fgetc
*
*  RETURNS:       : character read is returned as an int value,EOF or error
************************************************************************/
icf_uint32_t iptkapp_port_fgetc(FILE *p_file_object);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_fgets
*
*  DESCRIPTION    : This function is the wrapper for fgets
*
*  RETURNS:       : It returns the pointer to the buffer where read string
                         is stored
*************************************************************************/
icf_int8_t * iptkapp_port_fgets(icf_int8_t *p_buffer, 
			                     icf_uint32_t max_read_size,
                                 FILE * p_file_opject);

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_fopen
*
*  DESCRIPTION    : This function is the wrapper for fopen
*
*  RETURNS:       : It returns a pointer to a FILE object
*************************************************************************/
FILE * iptkapp_port_fopen(const icf_int8_t * p_filename,
                                 const icf_int8_t * p_operation_mode);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_fclose
*
*  DESCRIPTION    : This function is the wrapper for fclose
*
*  RETURNS:       : return 'o' if successful and EOF On failure
*************************************************************************/
icf_void_t iptkapp_port_fclose(FILE * p_file_object);



#endif

