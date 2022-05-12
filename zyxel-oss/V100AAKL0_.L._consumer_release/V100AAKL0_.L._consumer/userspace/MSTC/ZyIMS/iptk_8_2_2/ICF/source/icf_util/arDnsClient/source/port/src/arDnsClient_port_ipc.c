/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_ipc.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient porting IPC routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 16-Feb-2009    Anuradha Gupta         LLD           IPC Porting Interface
* 11-Jun-2009    Ashutosh Mohan         SPR 19737     Changes done for GCC 4.1 warnings removal
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#include "arDnsClient_api.h"
#include "arDnsClient_port_prototypes.h"
#ifdef ARDNSCL_PORT_LINUX
#include <unistd.h>
#endif

/****************************************************************************
 * API Name: arDnsClient_port_create_ipc
 * 
 * Description:    This function shall be  used open socket 
 * 
 * INPUT       : pDnsCl_glb_data is the global data.
 *             : transport_type is the transport typr.
 * 		       : address is the the address for bind.
 * OUTPUT      :p_fd is the returned FD
 *       	   :pError is the returned error
 *
 * Return :    ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.

 ******************************************************************************/
dnscl_return_t   arDnsClient_port_create_ipc (
			dnscl_void_t                *pDns_glb_data,
			arDnsClient_address_st      address,
			dnscl_int32_t               *p_fd,	  
			dnscl_error_t               *pError)
{
    /*Max send buffer size on UDP*/
    dnscl_uint32_t   send_buf_size = ARDNSCL_MAX_SEND_BUFF_SIZE;

    /*Max recvs buffer size on UDP*/
    dnscl_uint32_t    recv_buf_size = ARDNSCL_MAX_RECV_BUFF_SIZE;

    arDnsClient_global_data_st   *pDnsCl_glb_data = 
          (arDnsClient_global_data_st *)pDns_glb_data;


    /*Check for porting paltforms*/
#if defined(ARDNSCL_PORT_LINUX) || defined(ARDNSCL_PORT_WINDOWS) || defined(ARDNSCL_PORT_VXWORKS)
    /*Socket address structure*/    

    struct sockaddr_in    recv_addr;

    dnscl_int32_t          on = 1;

    /*Variable for socket fd*/
    dnscl_int32_t          fd = ARDNSCL_INIT_VAL;

    /*Socket address length*/
    dnscl_uint32_t  recv_addr_len = ARDNSCL_INIT_VAL;
         

    /*Check for  error pointer*/
    if(ARDNSCL_NULL == pError)
    {
        return ARDNSCL_FAILURE;
    }

    /*Check for  global data pointer*/
    if(ARDNSCL_NULL == pDnsCl_glb_data)
    {
        /*retun FAILURE with appropriate error*/
        *pError =  ARDNSCL_ERROR_INVALID_GLB_DATA;
        return ARDNSCL_FAILURE;
    }

    /*Type cast global data*/       
    pDnsCl_glb_data =(dnscl_void_t*)pDnsCl_glb_data;

    ARDNSCL_CLIB_PRINT(pDnsCl_glb_data, ("Entered function arDnsClient_port_create_ipc\n"))

   /*Open a Socket for transport type UDP*/
   fd = (dnscl_int32_t)socket(AF_INET,SOCK_DGRAM, 0);

    /*Check for FD value*/
    if ( 0 > fd)
    {
        ARDNSCL_CLIB_PRINT (pDnsCl_glb_data, ("Error Socket Creation \n"))
            /*could not create socket,so return failure*/
            *pError = ARDNSCL_ERROR_SOCKET_CREATE;
        return ARDNSCL_FAILURE;
    }
	ARDNSCL_CLIB_PRINT(pDnsCl_glb_data, ("Open fd is %d\n",fd))

    /*Setting the socket options */  
    if ( - 1 == setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*)(&on), 
                ARDNSCL_SIZEOF(dnscl_int32_t)))
    {
       arDnsClient_port_close_ipc(pDnsCl_glb_data, fd,pError);
        ARDNSCL_CLIB_PRINT (pDnsCl_glb_data, ("Error Set Socket Options \n"))
            /* Could not set socket option,so return failure*/
            *pError = ARDNSCL_ERROR_SET_SOCKET_OPTION;
        return ARDNSCL_FAILURE;
    }

    /*Socket address structure length*/
    recv_addr_len = ARDNSCL_SIZEOF(struct sockaddr_in);
/*ZyXEL porting*/
#if 1
    recv_addr.sin_addr.s_addr = INADDR_ANY;
#else
    recv_addr.sin_addr.s_addr = inet_addr((char *)
            (address.ipAddress.str_val));
#endif
/*ZyXEL porting end*/
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = ARDNSCL_HTONS((dnscl_uint16_t) address.port);
    ARDNSCL_MEMSET(&(recv_addr.sin_zero), '\0', 8);


    /*Binding to  assign a local protocol address to a socket*/
    if ( -1 == bind( fd,(struct sockaddr*)(&recv_addr), 
                recv_addr_len))
    {
       arDnsClient_port_close_ipc(pDnsCl_glb_data, fd,pError);
        ARDNSCL_CLIB_PRINT (pDnsCl_glb_data, ("Error Bind Failure \n"))
            /*Socket bind failed ,so return failure*/
            *pError = ARDNSCL_ERROR_SOCKET_BIND;
        /*return FAILURE*/
        return ARDNSCL_FAILURE;
    }

        /*Setting the socket options for send buffer*/     
        if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_SNDBUF, (char*)    
                    (&send_buf_size), ARDNSCL_SIZEOF(send_buf_size)))
        {
            arDnsClient_port_close_ipc(pDnsCl_glb_data, fd,pError);

            ARDNSCL_CLIB_PRINT (pDnsCl_glb_data, ("Error Set Socket Option Send Buffer \n"));
            /*could not set socket option for send buffer and hence  
              send failure*/
            *pError = ARDNSCL_ERROR_SET_SOCKET_OPTION;
            return ARDNSCL_FAILURE;
        }

    /*Setting the socket options for RECV buffer*/  
    if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_RCVBUF, (char*) 
                (&recv_buf_size), ARDNSCL_SIZEOF(recv_buf_size)))
    {
        arDnsClient_port_close_ipc(pDnsCl_glb_data, fd,pError);
        ARDNSCL_CLIB_PRINT (pDnsCl_glb_data, ("Error Set Socket Option Recv Buffer \n"));
        /*could not set socket option for recv buffer and hence 
          send failure*/
        *pError = ARDNSCL_ERROR_SET_SOCKET_OPTION;
        return ARDNSCL_FAILURE;
    }

        /*Assigning the socket FD*/
        *p_fd = fd;
#endif
    ARDNSCL_CLIB_PRINT (pDnsCl_glb_data, ("Exiting function arDnsClient_port_create_ipc \n"))
        /*return SUCCESS*/
        return ARDNSCL_SUCCESS;
}



/****************************************************************************
 * API Name: arDnsClient_port_send_message
 * 
 * Description:    This function shall be to send message on IPC  
 * 
 * INPUT       : pDnsCl_glb_data is the global data.
 *       :      p_fd is the  FD
 *       :      buffer is the  buffer to be sent
 *       :      msg_len is the  length of message to be sent
 *       :      send_addr is the address
 * OUTPUT:		pError is the returned error
 *
 * Return :    ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.

 ******************************************************************************/
dnscl_return_t    arDnsClient_port_send_message (
		dnscl_void_t                 *pDns_glb_data,
		dnscl_int32_t                fd,
		dnscl_uint8_t                *pBuffer,
		dnscl_uint16_t                msg_len,
		arDnsClient_address_st       *pSend_addr,
		dnscl_error_t                *pError)
{
   arDnsClient_global_data_st   *pDnsCl_glb_data = 
          (arDnsClient_global_data_st *)pDns_glb_data;

#if defined(ARDNSCL_PORT_LINUX) || defined(ARDNSCL_PORT_WINDOWS) || defined(ARDNSCL_PORT_VXWORKS)
	
        /*Socket address structure*/ 
	struct sockaddr_in  remote_addr;

	/*Socket address length*/
	dnscl_uint32_t  remote_addr_len = ARDNSCL_INIT_VAL;

	/*Number of bytes written by sendto*/
	dnscl_int32_t     bytes_written;


	/*Socket address structure length*/
	remote_addr_len = ARDNSCL_SIZEOF(struct sockaddr_in);

	/*Assigning address family to socket data structure*/
	remote_addr.sin_family = AF_INET;

	/*Assign destination Port to socket address structure*/
	remote_addr.sin_port =         	 	
		ARDNSCL_HTONS((dnscl_uint16_t)pSend_addr->port);

	/*Assign Destination Address to socket address structure*/
	remote_addr.sin_addr.s_addr = 
		inet_addr((char *)(pSend_addr->ipAddress.str_val));

	/*ARDNSCL_MEMSET the sin zero value to 0*/
	ARDNSCL_MEMSET(&(remote_addr.sin_zero),'\0',8);
   

	/*Sending data on UDP*/
	bytes_written = sendto (fd, 
			(dnscl_int8_t *)pBuffer,
			msg_len,
			0,
			(struct sockaddr*)&remote_addr,
			remote_addr_len);

     if(-1 == bytes_written)
	{
		*pError = ARDNSCL_ERROR_MESSAGE_SEND;
		return ARDNSCL_FAILURE ;
	}
        
         /*The message length is not equal to actuial 
	  number of bytes copied in send buffer so send failure*/

	if (msg_len != bytes_written) 
	{
		*pError = ARDNSCL_ERROR_MESSAGE_SEND;
		return ARDNSCL_FAILURE ;
	}

#endif
    pDnsCl_glb_data = pDnsCl_glb_data;

	/*return SUCCESS*/
	return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * API Name: arDnsClient_port_recv_message
 * 
 * Description:    This function shall be  used recv message on IPC 
 * 
 * INPUT       : pDnsCl_glb_data is the global data.
 *       :      fd is the  FD
 *       :      buffer is the  buffer to be recv
 *       :      msg_len is the  length of message to be recv
 *       :      recv_addr is the address
 * OUTPUT:		pError is the returned error
 *
 * Return :    ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.

 ******************************************************************************/
dnscl_return_t
arDnsClient_port_recv_message(
		dnscl_void_t              *pDns_glb_data,
		dnscl_int32_t 			   fd,
		dnscl_uint8_t 			  *pBuffer,
		dnscl_uint32_t 			  *msg_len,
		dnscl_error_t 			  *pError)
{
	/*Porting for Linux, Vxworks, windows*/
#if defined(ARDNSCL_PORT_LINUX) || defined(ARDNSCL_PORT_WINDOWS) || defined(ARDNSCL_PORT_VXWORKS)

	/*Socket address structure*/ 
	struct sockaddr_in  source_addr;

	/*Socket address length*/
	dnscl_uint32_t  source_addr_len = ARDNSCL_INIT_VAL;
	dnscl_uint32_t  buffer_len = ARDNSCL_MAX_RECV_BUFF_SIZE;

	/*Number of bytes recived */
	dnscl_int32_t     bytes_recv;

	source_addr_len = sizeof (struct sockaddr_in);
	
    /*recv message on UDP*/
	bytes_recv = recvfrom (fd, pBuffer, buffer_len,0,
			(struct sockaddr*)(&source_addr), &source_addr_len);
	if ( -1 == bytes_recv)
	{
		*pError = ARDNSCL_ERROR_MESSAGE_RECV;
		/*No data present on recv buffer,so send failure */
		return ARDNSCL_FAILURE;
	}

    *msg_len = bytes_recv;
#endif

    pDns_glb_data = pDns_glb_data;

	/*return SUCCESS*/
	return ARDNSCL_SUCCESS;
}

/****************************************************************************
 * API Name: arDnsClient_port_close_ipc
 * 
 * Description:    This function shall be  used to close socket 
 * 
 *INPUT        : fd is the  FD
 * OUTPUT	:pError is the returned error
 *
 * Return :    ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.

 ******************************************************************************/
dnscl_return_t
arDnsClient_port_close_ipc(
        dnscl_void_t   *pDns_glb_data,
        dnscl_int32_t  fd,
		dnscl_error_t  *pError)
{
    arDnsClient_global_data_st   *pDnsCl_glb_data = 
          (arDnsClient_global_data_st *)pDns_glb_data;


    ARDNSCL_CLIB_PRINT(pDnsCl_glb_data, ("Entered function arDnsClient_port_close_ipc\n"))

/*close socket for linux*/
#if defined(ARDNSCL_PORT_LINUX)
    /*close socket fd*/
     close(fd);
#endif

#if defined(ARDNSCL_PORT_VXWORKS)
	 close(fd);
#endif
    

/*Close fd for eindows*/
#ifdef ARDNSCL_PORT_WINDOWS
    closesocket(fd);
#endif
    
     /*to remove warning */
    *pError   =  *pError;
    pDnsCl_glb_data = pDnsCl_glb_data;
    
    ARDNSCL_CLIB_PRINT(pDnsCl_glb_data, ("Exiting function arDnsClient_port_close_ipc\n"))   

    /*return SUCCESS*/
    return ARDNSCL_SUCCESS;
}


