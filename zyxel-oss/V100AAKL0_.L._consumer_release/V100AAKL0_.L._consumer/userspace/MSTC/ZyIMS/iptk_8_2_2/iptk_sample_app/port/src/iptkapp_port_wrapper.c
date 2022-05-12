/*****************************************************************************
 **
 ** FILENAME:		iptkapp_port_wrapper.c
 **
 ** DESCRIPTION:	This file contains the ipc implementation using BSD udp
 **					sockets and the wrapper functions for memory and string 
 **                 related Operations.
 **
 ** DATE			NAME	        	REFERENCE		REASON
 ** ----			----		        ---------		------
 **	05-DEC-07		Vidyut Gupta		            	Initial Creation
 ** 06-DEC_07       Vipul Varshney                     
 *****************************************************************************
 **			Copyright (c) 2007 Aricent Inc . All Rights Reserved.
 *****************************************************************************/
#include "iptkapp_port_wrapper.h"
#include "iptkapp_port_prototypes.h"
#include "iptkapp_cfg_types.h"
#include "iptkapp_glb.h"
#include "icf_clib.h"
#ifdef ICF_PORT_WINDOWS 
#include<winsock2.h>
#include<windows.h>
#include <stdio.h>
#endif
#include <errno.h>
#include <sys/un.h> //ZyXEL porting

icf_int32_t g_fd ;
icf_int32_t ttyfd ;
fd_set		fdset;
/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_udp_init
*
*  DESCRIPTION    : This function is called to initialize the ipc. It  
*					opens socket and bind it to given port.
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
/*ZyXEL porting*/
#if 0 
iptkapp_return_t 
iptkapp_port_udp_init(void)
{

    iptkapp_return_t    ret_val = IPTKAPP_SUCCESS;
    struct sockaddr_in  recv_addr;
#ifdef ICF_PORT_WINDOWS
    WSADATA                     wsaData; 
    
    if (WSAStartup( 1, &wsaData ) != 0 )
    {
        /* 
         * Tell the user that we could not find a usable WinSock DLL.
         */
        return IPTKAPP_FAILURE;
    }
#endif
    g_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(0 > g_fd)
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        recv_addr.sin_family = AF_INET;
        recv_addr.sin_port = htons(g_iptkapp_cfg_db_t.iptk_app_port);
        recv_addr.sin_addr.s_addr = inet_addr(g_iptkapp_cfg_db_t.iptk_app_ip);
        iptkapp_port_memset(&(recv_addr.sin_zero), '\0', 8);
        if ( -1 == bind(g_fd,(struct sockaddr*)(&recv_addr), sizeof(struct sockaddr_in)))
        {
            IPTKAPP_PRINTF("\n\tIPTKAPP > Port is currently being used by some other application \n");
            ret_val = ICF_FAILURE;
        }
    }
#ifndef ICF_PORT_WINDOWS

    ttyfd = open("/dev/tty",O_RDWR, 0644);
#endif
    return ret_val;
}
#else
iptkapp_return_t iptkapp_port_udp_init(void) {

	iptkapp_return_t    ret_val = IPTKAPP_SUCCESS;
	struct sockaddr_un addr;

	g_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);

	if(g_fd < 0) {
		ret_val = ICF_FAILURE;
	} else {
		bzero(&addr, sizeof(addr));
		addr.sun_family = PF_LOCAL;
		strncpy(addr.sun_path, IPTK_SAMPLE_APP_CHANNEL_PATH, sizeof(IPTK_SAMPLE_APP_CHANNEL_PATH));
		unlink(IPTK_SAMPLE_APP_CHANNEL_PATH);
		if ( bind( g_fd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
			IPTKAPP_PRINTF("\n\tIPTKAPP > Port is currently being used by some other application \n");
			ret_val = ICF_FAILURE;
		}
		
	}

	IPTKAPP_PRINTF("\n\tIPTKAPP > icf_port_open_ipc_channel created fd = %d for channel = %s\n", g_fd, IPTK_SAMPLE_APP_CHANNEL_PATH);
	return ret_val;
}
#endif
/*ZyXEL porting end*/


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_close
*
*  DESCRIPTION    : This function is called to close the ipc.  
*
*  RETURNS		  : void
*************************************************************************/

icf_void_t 
iptkapp_port_close(void)
{
    
#ifndef ICF_PORT_WINDOWS
	close(g_fd);
	unlink(IPTK_SAMPLE_APP_CHANNEL_PATH);  //ZyXEL porting
    close(ttyfd);
#else
	closesocket(g_fd);
#endif
}


/*************************************************************************
*
*  FUNCTION NAME  :  iptkapp_port_udp_recv
*
*  DESCRIPTION    : This functions implements the udp recieve. It will 
*					first receive the message from UDP and then decode it
*					by calling IPTK clib decode function. Finally it will
*					assign the decoded message pointer to p_buf.
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t 
iptkapp_port_udp_recv(icf_uint8_t **p_buf)
{

	struct sockaddr_in  source_addr;
#ifdef ICF_PORT_WINDOWS 
    icf_uint32_t          source_addr_len;
#else 
socklen_t           source_addr_len;
#endif 
    icf_uint16_t        len_of_msg_read ;
    icf_uint8_t         buffer[MAX_MTU_SIZE];

	icf_uint8_t         *p_icf_decoded_buf = IPTKAPP_NULL;
	iptkapp_return_t	    ret_val ;
    iptkapp_error_t         *p_ecode = IPTKAPP_NULL;

    source_addr_len = sizeof (struct sockaddr_in);
    ret_val = IPTKAPP_SUCCESS;
	
	len_of_msg_read = recvfrom (g_fd, buffer, MAX_MTU_SIZE, 0,
								(struct sockaddr*)(&source_addr), &source_addr_len);

	if(len_of_msg_read > 0)
    {
		ret_val=icf_decode_msg((icf_msg_st**)&p_icf_decoded_buf,
                                buffer,
                                len_of_msg_read,
                                p_ecode);
		
        if(ret_val == ICF_FAILURE)
		{
		    ret_val = ICF_FAILURE;
		}
		
        else
        {
            *p_buf = p_icf_decoded_buf;
        }

	}
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_iptk_ipc_send
*
*  DESCRIPTION    : This is the function that sends the UDP packet to n/w
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t 
iptkapp_iptk_ipc_send(icf_void_t *p_msg,icf_uint32_t buf_len, iptkapp_error_t *p_error)
				  
{
	/*struct sockaddr_in  remote_addr;*/  //ZyXEL porting
	icf_uint32_t		payload_length,packet_length;
	icf_uint16_t		bytes_written = 0;  //ZyXEL porting
    icf_uint8_t		*p_buf;
	struct sockaddr_un addr;	// ZyXEL porting
	printf("iptkapp_iptk_ipc_send\r\n");// ZyXEL porting

	
	if(p_msg ==NULL)
	{
		return IPTKAPP_FAILURE;
	}

	/* get the port corresponding to the dest-id from cfg */
/* ZyXEL porting*/
#if 0
	/*remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(g_iptkapp_cfg_db_t.iptk_port);
    remote_addr.sin_addr.s_addr = inet_addr(g_iptkapp_cfg_db_t.iptk_ip);
    	memset(&(remote_addr.sin_zero),'\0',8);*/
#else
	addr.sun_family = PF_LOCAL;
	strncpy(addr.sun_path, ICF_ES_CHANNEL_PATH, sizeof(ICF_ES_CHANNEL_PATH));
#endif
/* ZyXEL porting end*/
    
	packet_length = payload_length = buf_len;
		
	p_buf = ( icf_uint8_t *)p_msg;

	while( packet_length )
    {
		printf("packet_length = %d\r\n", packet_length);  //ZyXEL porting
		if( packet_length > MAX_MTU_SIZE ) 
        {
			packet_length = MAX_MTU_SIZE;
		}

		printf("bytes_written byte = %d\r\n", bytes_written);  //ZyXEL porting
		/*bytes_written = sendto(g_fd,(icf_void_t*)p_buf,packet_length,0,
									(struct sockaddr*)&remote_addr,
									 sizeof(struct sockaddr_in));*/    //ZyXEL porting
		bytes_written = sendto(g_fd,(icf_void_t*)p_buf,packet_length,0,(struct sockaddr*)&addr,sizeof(struct sockaddr_un));    //ZyXEL porting
		printf("sendto byte = %d\r\n", bytes_written);    //ZyXEL porting

		if ( packet_length != bytes_written)
		{
			return (IPTKAPP_FAILURE);
		}
		
		p_buf +=packet_length;

		payload_length -= packet_length;

		packet_length = payload_length;	

	}
    p_error = p_error;

    return (IPTKAPP_SUCCESS);
}



/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_select
*
*  DESCRIPTION    : This is the wrapper function to the select. it returns 
*				    ICF_SUCCESS whether the data is there on the fd or
*					if the select is timed out.
* 
*  PARAMETER	  :	 p_timeout : will be TRUE if the select has timed out and no
*					 data was there on the FD
*				  :  usec : is the time in micro sec				
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t
iptkapp_port_select(icf_uint32_t usec, icf_boolean_t *p_timeout_flag)
{
	struct timeval     timeout;
	struct timeval	   *p_timeout ;
	icf_int8_t   retval;

    p_timeout = &timeout;


	
	FD_ZERO(&fdset);
	
#ifndef ICF_PORT_WINDOWS
	FD_SET(g_fd,&fdset);
    FD_SET(ttyfd,&fdset);
#else
	FD_SET((icf_uint32_t)g_fd,&fdset);
#endif 


	if(usec == IPTKAPP_SELECT_INFINITE)
	{
#ifndef ICF_PORT_WINDOWS
		/* blocking wait for select */
		p_timeout = IPTKAPP_NULL;
#else
		timeout.tv_sec = 0;
		timeout.tv_usec = 50;
#endif

	}
	
	else
	{
		/* wait for the time specified */
		timeout.tv_sec = usec/1000000;  /* get the time in seconds */
		timeout.tv_usec = usec % 1000000 ;/* rest of the time in micro sec */
	}
	
	retval = select(FD_SETSIZE,&fdset,(fd_set *)0,NULL,p_timeout);
	if(-1 == retval)
	{

		return IPTKAPP_FAILURE;

	}
	else if(0 == retval)
	{
		/* fd is not set select timed out*/
		*p_timeout_flag = ICF_TRUE;
		return IPTKAPP_SUCCESS;
	}
	/* fd is set return the success */
	*p_timeout_flag = ICF_FALSE;
	return ICF_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_malloc
*
*  DESCRIPTION    : This function is the wrapper for malloc
*
*  RETURNS:       : void pointer to the allocated memory
*************************************************************************/
icf_void_t * iptkapp_port_malloc(icf_uint32_t size)
{
    icf_void_t  *p_mem_to_return = ICF_NULL;
    p_mem_to_return = malloc(size);
    memset(p_mem_to_return, 0, size);
    return p_mem_to_return;

}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_free
*
*  DESCRIPTION    : This function is the wrapper for free
*
*  RETURNS:       : no return value 
*************************************************************************/
icf_void_t iptkapp_port_free(icf_void_t * ptr)
{
	free(ptr);
}

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
						 icf_uint32_t num_bytes_to_copy)
{
	return memcpy(p_destination,p_source,num_bytes_to_copy);
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_memset
*
*  DESCRIPTION    : This function is the wrapper for memset 
*
*  RETURNS:       : return the pointer(ptr), given in the argument
*************************************************************************/
icf_void_t * iptkapp_port_memset( icf_void_t * p_mem_block, \
        icf_uint32_t value_to_set, icf_uint32_t num)
{
	return memset(p_mem_block,value_to_set, num);
} 

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strcpy
*
*  DESCRIPTION    : This function is the wrapper for strcpy 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
icf_int8_t * iptkapp_port_strcpy(icf_int8_t *p_destination,\
        const icf_int8_t *p_source)
{
	return strcpy(p_destination,p_source);
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strncpy
*
*  DESCRIPTION    : This function is the wrapper for strncpy 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
icf_int8_t * iptkapp_port_strncpy(icf_int8_t *p_destination,\
        const icf_int8_t *p_source, 
			icf_uint32_t max_num_bytes)
{
	return strncpy(p_destination,p_source,max_num_bytes);
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strcat
*
*  DESCRIPTION    : This function is the wrapper for strcat 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
icf_int8_t * iptkapp_port_strcat(icf_int8_t *p_destination,\
        const icf_int8_t *p_source)
{
	return strcat(p_destination,p_source);
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strtok
*
*  DESCRIPTION    : This function is the wrapper for strtok 
*
*  RETURNS:       : pointer to the last token found in string
*************************************************************************/
icf_int8_t * iptkapp_port_strtok(icf_int8_t * p_source, \
        const icf_int8_t * p_delimiters)
{
	return strtok(p_source,p_delimiters);
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strcmp
*
*  DESCRIPTION    : This function is the wrapper for strncmp 
*
*  RETURNS:       : It returns an integer less than, equal to, or greater 
*		            than zero if first n charaters of  string1  is  found, 
*	      	        respectively, to be less than, to match, or be greater 
*		            than string2.
*************************************************************************/

icf_uint32_t iptkapp_port_strcmp(const icf_int8_t *p_string_1,\
        const icf_int8_t *p_string_2)
{
	return strcmp(p_string_1,p_string_2);
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_strlen
*
*  DESCRIPTION    : This function is the wrapper for strlen 
*
*  RETURNS:       : It returns the number of characters in string
*************************************************************************/
icf_uint32_t iptkapp_port_strlen(const icf_int8_t *p_string)
{
	return strlen(p_string);
}

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_fgetc
*
*  DESCRIPTION    : This function is the wrapper for fgetc 
*
*  RETURNS:       : character read is returned as an int value,EOF or error 
*************************************************************************/
icf_uint32_t iptkapp_port_fgetc(FILE *p_file_object)
{
	return fgetc(p_file_object);
}


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
						         FILE * p_file_object)
{
	return fgets(p_buffer, max_read_size, p_file_object);
}



/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_fopen
*
*  DESCRIPTION    : This function is the wrapper for fopen 
*
*  RETURNS:       : It returns a pointer to a FILE object
*************************************************************************/
FILE * iptkapp_port_fopen(const icf_int8_t * p_filename,
				 const icf_int8_t * p_operation_mode)
{
	return fopen(p_filename,p_operation_mode);
}


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_fclose
*
*  DESCRIPTION    : This function is the wrapper for fclose 
*
*  RETURNS:       : return 'o' if successful and EOF On failure
*************************************************************************/
icf_void_t iptkapp_port_fclose(FILE * p_file_object)
{
	fclose(p_file_object);
}



/*************************************************************************
*
*  FUNCTION NAME  : icf_memget
*
*  DESCRIPTION    : This function will allocate memory for icf client library
*
*  RETURNS		  : Allocated buffer
*************************************************************************/

icf_void_t* icf_memget(icf_uint32_t size)
{

	return iptkapp_port_malloc(size);
}

/*************************************************************************
*
*  FUNCTION NAME  : icf_memfree
*
*  DESCRIPTION    : This function will free allocated memory for icf client library
*
*  RETURNS		  : NONE
*************************************************************************/

icf_void_t icf_memfree(icf_void_t* buffer)
{

	iptkapp_port_free(buffer);
	return;
}

/*************************************************************************
*
*  FUNCTION NAME  : icf_memcpy
*
*  DESCRIPTION    : This function will copy data from one memory location to other 
*					 for icf client library
*
*  RETURNS		  : Either IMSC_SUCCESS or IMSC_FAILURE
*************************************************************************/

icf_void_t* icf_memcpy(icf_void_t* s1, icf_void_t* s2,icf_uint32_t size)
{

	iptkapp_port_memcpy(s1,s2,size);
	return s1;
}



/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_port_getline
*
*  DESCRIPTION    : This is the function that take read a line
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

iptkapp_return_t iptkapp_port_getline(icf_uint8_t str[],icf_int32_t limit)
{
    icf_int32_t ctr,c;
    ctr = 0;
    c=getchar();
    do
    {
        c = getchar();
        str[ctr] = (icf_uint8_t)c;
        ctr++;
    } while(ctr<(limit-1) && c!=EOF && c!='\n');

    if(c=='\n')
    {
        str[ctr] = (icf_uint8_t)c;
        ++ctr;
    }
    str[ctr] = '\0';
    
    if(ctr)
    {
        return IPTKAPP_SUCCESS;
    }
    else
    {
        return IPTKAPP_FAILURE;
    }

}

