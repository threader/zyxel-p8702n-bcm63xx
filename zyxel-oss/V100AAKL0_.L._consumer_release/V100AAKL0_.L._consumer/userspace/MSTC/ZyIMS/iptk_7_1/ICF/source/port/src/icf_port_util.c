/*************************************************************************
 *
 *     FILENAME           :  icf_port_util.c
 *
 *     DESCRIPTION        :  This file contains utility routines for tk
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         25/11/04    Pankaj Negi        ICF LLD        Initial Version.
 *         06/01/05    Aashish Suchdev                      Updated for 
 *                                                          cmn changes
 *         07/01/05    Aashish Suchdev                      Removed def 
 *                                                          for loggin err
 *                                                          and traces
 *         28/01/05    Aashish Suchdev                      Added strcmp 
 *         20/04/05    Amit Sharma                          Added function for
 *                                                          random
 *         13/10/05    Rohit Aggarwal     ICF 4.0 CDD       IPSec changes
 *		   06/01/06	   Jyoti Narula	     SPR 8063/ 9043		For AKA all cases handling, changes made in:
 *															icf_port_aka_challenge_req
 *
 *	   1-Mar-2007      Deepti Goyal     ICF 6.4            SPR 14644
 *     25-Jun-2008     Rajiv Kumar      SPR - 18722        IPTK not compilable with gcc
 *                                                         version 4.2.3 	   
 *
 *     12-Dec-2008     Vinay Jindal     SPR 19234           Merge SPR 19199 fix
 * 09-Jul-2009  Rohit Aggarwal  SPR 19772   Changes for Big-endian Linux
 *
 *     Copyright 2008, Aricent.
 *
 *************************************************************************/
#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#elif ICF_PORT_LINUX
#include <time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdarg.h>
#endif

#include "icf_common_types.h"
#include "icf_es_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_port_defs.h"
#include "icf_common_prototypes.h"
#include "icf_port_macros.h"
#include "gl_types.h"
#include "gl_htm.h"
#include "gl_htm_interfaces.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif

/*Defined in Eshell*/
extern icf_glb_pdb_st        *p_persistent_glb_pdb;
extern icf_void_t* icf_cmn_memget(
        INOUT   icf_glb_pdb_st       *p_glb_pdb,
        IN      icf_uint32_t         size,
        IN      icf_pool_id_t        pool_id,
        OUT     icf_error_t          *p_ecode );

extern icf_return_t icf_cmn_memfree(
            INOUT   icf_glb_pdb_st        *p_glb_pdb,
            INOUT      icf_void_t*           p_buf,
            IN      icf_pool_id_t     pool_id,
            OUT     icf_error_t    *p_ecode );

extern icf_void_t icf_port_strlwr(INOUT icf_uint8_t *string);

extern icf_return_t icf_port_cbk_recv_from_wrpr(
        INOUT   icf_msg_st              *p_msg);

extern icf_uint32_t icf_port_hash_free_element(
	gl_hash_key_ut *p_key , icf_void_t *p_args);

#if defined IMS_CLIENT || defined ICF_NW_ACTIVATE
extern  icf_return_t initWrprSend(icf_return_t (*asmcWrprSend)(void *msg));

extern  icf_return_t icf_port_cbk_RsporIndCallBack(
				icf_msg_st * p_msg);
#endif
#ifdef ICF_PORT_VXWORKS
icf_ftp_info_st g_icf_ftp_info;
icf_ramdisk_param_st g_icf_ramdisk_param;
icf_boolean_t    g_icf_is_ramdisk_present;
#endif
/*#include "gl_error.h"*/
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_memset
*
*  DESCRIPTION    :   Utility function for setting the buffer to zero
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_port_memset(
        IN  icf_void_t       *p_buf,
        IN  icf_uint8_t      ch,
        IN  icf_uint32_t     n)
{
    icf_uint32_t     count = 0;
    icf_uint8_t      *ptr = (icf_uint8_t*)p_buf;

    for(; count < n; count++ )
    {
        ptr[count] = ch;
    }
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strcmp
*
*  DESCRIPTION    :   Utility function for string comparison.
*
*  RETURNS        :  0 if strings match, non zero if they don't
*
 *************************************************************************/
icf_int32_t icf_port_strcmp(
        IN icf_uint8_t   *p_str1,
        IN  icf_uint8_t   *p_str2)
{
    for(; *p_str1 == *p_str2; p_str1++, p_str2++ )
        if( *p_str1 == '\0' )
            return 0;
    return (*p_str1-*p_str2);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strncmp
*
*Purpose:
*       Compares two strings for lexical order.  The comparison stops
*       after: (1) a difference between the strings is found, (2) the end
*       of the strings is reached, or (3) count characters have been
*       compared.
*
*Entry:
*       char *first, *last - strings to compare
*       unsigned count - maximum number of characters to compare
*
*Exit:
*       returns <0 if first < last
*       returns  0 if first == last
*       returns >0 if first > las*
 *************************************************************************/
icf_int32_t icf_port_strncmp(
        IN icf_uint8_t   *p_first,
        IN icf_uint8_t   *p_last,
		IN icf_uint32_t   count)
{
        icf_uint32_t local_count = count;
	if (!local_count)
		return(0);
	
	while (--local_count && *p_first && *p_first == *p_last)
	{
		p_first++;
		p_last++;
	}
	
	return( *(unsigned char *)p_first - *(unsigned char *)p_last );
}



/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strcpy
*
*  DESCRIPTION    :   Utility function for string copy.
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_port_strcpy(
        INOUT icf_uint8_t   *p_dest,
        IN  icf_uint8_t   *p_src)
{
    while ((*p_dest++ = *p_src++) != '\0');
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strcat
*
*  DESCRIPTION    :   Utility function for string cat.
*
*  RETURNS        :  Void
*
 *************************************************************************/
icf_void_t icf_port_strcat(
        INOUT icf_uint8_t   *p_dest,
        IN  icf_uint8_t   *p_src)
{
    while(*p_dest)
        p_dest++;

    while((*p_dest++ = *p_src++) != '\0');
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strdup
*
*                     Duplicates the the string 
*Uses:
*
*Exceptions:
*
*******************************************************************************/

icf_uint8_t*  icf_port_strdup (
        IN icf_uint8_t* p_string
        )
{
        icf_uint8_t* p_memory=ICF_NULL ;
		icf_return_t  ret_val = ICF_SUCCESS;
		icf_error_t   ecode;
        if (!p_string)
                return(NULL);

		p_memory = icf_cmn_memget(p_persistent_glb_pdb, 
				icf_port_strlen(p_string)+1, 
               (icf_pool_id_t)ICF_MEM_COMMON, &ecode);
        if (ICF_NULL != p_memory)
		{
          icf_port_strcpy(p_memory,p_string);
		  return p_memory;
		}
        ret_val = ret_val;
        return(ICF_NULL);
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strfree
*
*                     Frees the string allocated in icf_port_strdup
*Uses:
*
*Exceptions:
*
*******************************************************************************/
icf_void_t  icf_port_strfree (
        IN icf_uint8_t* p_string
        )
{
			icf_error_t   ecode;

        icf_cmn_memfree(p_persistent_glb_pdb, (icf_void_t*)p_string,
                   (icf_pool_id_t)ICF_MEM_COMMON, &ecode); \

        return;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_memcpy
*
*  DESCRIPTION    :   Utility function for copying memory block.
*
*  RETURNS        :   Address of the destination memory location 
*
 *************************************************************************/
icf_void_t
icf_port_memcpy (
        OUT icf_void_t   *p_dest,
        IN  icf_void_t   *p_src,
        IN  icf_uint32_t count)
{
    icf_uint32_t num_of_bytes = count;
    icf_uint8_t *p_tmp = (icf_uint8_t *)p_dest, 
    *p_s = (icf_uint8_t *)p_src;

    while (num_of_bytes--)
        *p_tmp++ = *p_s++;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strlen
*
*  DESCRIPTION    :   Utility function for calculating string length.
*
*  RETURNS        :   A U32 holding value of the length of the string  
*
 *************************************************************************/
icf_uint32_t
icf_port_strlen (IN icf_uint8_t    *p_s)
{
    const icf_uint8_t    *p_sc;

    for(p_sc = p_s; *p_sc != '\0'; ++p_sc);

    return p_sc - p_s;
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strchr
*
*  DESCRIPTION    : Returns pointer to first occurrrnce of a character in a
*                    given string.
*
*  RETURNS        :   Returns pointer to first occurrrnce of a character in a
*                    given string.
*
*************************************************************************/
icf_uint8_t *
icf_port_strchr (
        IN icf_uint8_t                 *p_string,
        IN icf_uint8_t                  character)
{

    for( ; *p_string != '\0'; p_string++)
    {
        /*
         * * If char is same.
         */
        if(*p_string == character)
        {
            return (icf_uint8_t *)p_string;
        }
    }
    return ICF_NULL;

}


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_random
*
*  DESCRIPTION    :   function to calculate random number in given range
*
*  RETURNS        :
*
*************************************************************************/
icf_uint16_t icf_port_random(
        IN  icf_uint16_t   start_limit,
        IN  icf_uint16_t   end_limit)
{

    icf_time_n_date_st  curr_time;
    icf_uint16_t        seed;
    icf_uint16_t        rand_time;
    
    /*Get current system time*/
    if (ICF_SUCCESS == icf_port_get_curr_time(&curr_time))
    {
        /*Get the seed by summing up all time fields*/
         seed = (icf_uint16_t)(curr_time.seconds + curr_time.minutes + curr_time.hour
             + curr_time.date + curr_time.month + curr_time.year);

         seed = (icf_uint16_t)(seed % (end_limit - start_limit));

         rand_time = (icf_uint16_t)(seed + start_limit);
    } /* if (ICF_SUCCESS == icf_p... */
    else
    {
        rand_time = 0;
    } /* else */
    
    return (rand_time);
}


/**************************************************************
 * FUNCTION:    icf_port_get_curr_time
 *
 * DESCRIPTION: This is a utility function invoked by the
 *              call logging functions to obtain the timestamp
 *              from the system.
 *
 * RETURNS:     ICF_SUCCESS if the date n time obtained
 *              successfuly. ICF_FAILURE
 *              -If the time() call returned failure.
 *************************************************************/

icf_return_t icf_port_get_curr_time(
        INOUT   icf_time_n_date_st *p_time_details)
{

    /* Variables needed to get the value of time
     *  from system functions.*/
    time_t  sys_time;
    time_t *p_time = ICF_NULL;
    time_t time_ret =  (time_t)ICF_NULL;
    struct tm *p_tm =  ICF_NULL;

    icf_return_t ret_val = ICF_SUCCESS;
	#ifdef ICF_PORT_WINCE
		SYSTEMTIME sys_tm ; 
	#endif

    p_time = &sys_time;
#ifdef ICF_PORT_WINCE
		GetLocalTime(&sys_tm); 
    	/* Copying the values into the SM defined time_n_date format.*/
        p_time_details->seconds  =   sys_tm.wSecond;
        p_time_details->minutes  =   sys_tm.wMinute;
        p_time_details->hour =   sys_tm.wHour;
        p_time_details->date =   sys_tm.wDay;
        p_time_details->month=   sys_tm.wMonth+1;
        p_time_details->year =   sys_tm.wYear+1900;
#else

    time_ret = time(p_time);

    if ((time_t)-1 == time_ret)
    {
        /* time() system function returned failure. */
        ret_val = ICF_FAILURE;

    }/* time() call failed */
    else
    {
        /* time() call was SUCCESSFUL*/
        p_tm = localtime(p_time);

	if(ICF_NULL == p_tm)
	{
		return ICF_FAILURE;
	}

        /* Copying the values into the SM defined time_n_date format.*/

        p_time_details->seconds  =   (icf_uint8_t)p_tm->tm_sec;
        p_time_details->minutes  =   (icf_uint8_t)p_tm->tm_min;
        p_time_details->hour =   (icf_uint8_t)p_tm->tm_hour;
        p_time_details->date =   (icf_uint8_t)p_tm->tm_mday;
        p_time_details->month=   (icf_uint8_t)(p_tm->tm_mon+1);
        p_time_details->year =   p_tm->tm_year+1900;
    }/* time() call successful*/
#endif

    return ret_val;

}/* Function icf_port_get_curr_time() ends here. */


#if 0
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_map_genlib_ecode
*
*  DESCRIPTION    :   Maps Error Code returned by Genlib to ICF Error Code
*
*  RETURNS        :   
*
*************************************************************************/
icf_void_t
icf_port_map_genlib_ecode (
        IN  gl_error_t                   gl_ecode,
        OUT icf_error_t                *p_ecode)
{
    switch (gl_ecode)
    {
        case GL_MM_ERR_MALLOC_FAILED:
        case GL_MM_ERR_INV_POOL_ID:
        case GL_MM_ERR_INV_TASK_ID:
        case GL_MM_ERR_INV_BUF_PTR:
        case GL_MM_ERR_BUF_NOT_AVAIL:
        case GL_MM_ERR_INV_BUF_QTY:
        case GL_MM_ERR_INV_BUF_SIZE:
        case GL_MM_ERR_POOL_NOT_AVAIL: 
        case GL_MM_ERR_BUF_ALLOCATED:
        case GL_MM_ERR_INV_OLD_TASK_ID:
        case GL_MM_ERR_INV_NEW_TASK_ID:
        case GL_MM_CREATE_MUTEX_FAILED:
        case GL_MM_ERR_INV_CNGST_MGMT_SPECS_PTR:
        case GL_MM_ERR_INV_CNGST_MGMT_SPECS_VALUES:
        case GL_MM_ERR_INV_CNGST_MGMT_CALL_BACK_FUNC_PTR:
        case GL_MM_ERR_INV_NUM_OF_REQUESTS:
        case GL_MM_ERR_INV_BUFFER_ALOC_SIGNATURE:
        case GL_MM_ERR_MEM_OVERWRITE:
        case GL_MM_ERR_INV_POOL_USAGE_PTR:
        case GL_MM_ERR_INTERNAL:
        case GL_MM_ERR_AUDIT_ACTIVITY_NOT_SET:
        case GL_MM_ERR_NO_RESOURCES:
            *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
            break;
            
        case GL_MM_ERR_BUF_FREE:
            *p_ecode = ICF_ERROR_MEM_FREE_FAILURE;
            break;
            
        case GL_MM_ERR_INIT_REQD:
            *p_ecode = ICF_ERROR_MEM_INIT;
            break;
            
        default:
            *p_ecode = *p_ecode;
            break;
    }
}
#endif


/****************************************************************************
** Function: icf_port_fopen
**
****************************************************************************
** DESCRIPTION: Porting wrapper function for fopen
**    
** 
** 
**
***************************************************************************/
FILE *icf_port_fopen(const char *filename, const char *mode)
{
#ifdef ICF_PORT_VXWORKS
	char vx_filename[ICF_MAX_CONFIG_FILE_PATH_LEN] = "";
	
	icf_port_strcpy(vx_filename,ICF_VX_VOLUME_LABEL);
	icf_port_strcat(vx_filename,filename);
	
	if(ICF_FAILURE == icf_port_get_file_from_host(&g_icf_ftp_info,
                (icf_uint8_t *)filename,
                (icf_uint8_t *)filename))
	{
		ICF_PRINT(("\n Get file:%s from host failed ",filename));
		return ICF_NULL;
	}
	return fopen(vx_filename,mode);
#else
	return fopen(filename, mode); 
#endif
}


/****************************************************************************
** Function: icf_port_fgets
**
****************************************************************************
** DESCRIPTION: Porting wrapper function for fgets
**    
** 
** 
**
***************************************************************************/
char *icf_port_fgets(char *s, int n, FILE *stream)
{
	return fgets(s,n,stream); 
}

/****************************************************************************
** DESCRIPTION: Porting wrapper function for fgets
**    
***************************************************************************/
int icf_port_fgetc (FILE *stream, char  *c)
{
    *c = (icf_int8_t)fgetc(stream);
    return ICF_SUCCESS;
}

/****************************************************************************
** DESCRIPTION: Porting wrapper function for fputs
**    
***************************************************************************/
int icf_port_fputs (char *s, FILE *stream)
{
    fputs(s, stream);
    return ICF_SUCCESS;
}

/****************************************************************************
** DESCRIPTION: Porting wrapper function for fputc
**    
***************************************************************************/
int icf_port_fputc (char c, FILE *stream)
{
    fputc(c, stream);
    return ICF_SUCCESS;
}

/****************************************************************************
** DESCRIPTION: Porting wrapper function for fseek
**    
***************************************************************************/
int   icf_port_fseek 
(
    FILE    *stream,
    long    offset,
    int     whence
)
{
    fseek(stream, offset, whence);
    return ICF_SUCCESS;
}
    
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strmatch
*
*  DESCRIPTION    :   Utility function for string comparison.
*                     It checks if str1 comes at the start of str2
*                     This is slightly different than strcmp. 
*                     It lets str2 to be longer than str1.
*
*  RETURNS        :   0 if strings match, non zero if they don't
*
 *************************************************************************/
icf_int32_t icf_port_strmatch(
        IN icf_uint8_t   *p_str1,
        IN  icf_uint8_t   *p_str2)
{
    for(; *p_str1 == *p_str2; p_str1++, p_str2++ )
        if( *p_str1 == '\0' )
            return 0;
	
	if( *p_str1 == '\0' ) 
		return 0;
	else
        return (*p_str1-*p_str2);
}


/******************************************************************************
 ** FUNCTION: 	 icf_port_get_host_ip
 **
 ** DESCRIPTION: This function invokes the port wrapper for gethostbyname to
 **              resolve the HostName in pHost to an IP address.  The memory
 **              for IP address is allocated by the port and is released by
 **              UATK when it's processing is complete
 ******************************************************************************/
icf_return_t icf_port_get_host_ip
	(icf_int8_t* pHost,
	icf_int8_t** ppDest)
{
  icf_return_t ret_val = ICF_SUCCESS;
#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX)) || defined(ICF_PORT_VXWORKS)
    struct hostent  *p_host_ip = ICF_NULL;
    struct in_addr  *inaddrptr = ICF_NULL;
    p_host_ip = gethostbyname(pHost);
    if ( NULL == p_host_ip)
    {
        ICF_PRINT(((icf_uint8_t *)"\nPORT:Failure in gethostbyname call"));
        ret_val = ICF_FAILURE;
    }
    else
    {
        inaddrptr = (struct in_addr *)(*(p_host_ip->h_addr_list));
        *ppDest=(icf_int8_t*)icf_port_strdup((icf_uint8_t*) inet_ntoa(*inaddrptr));
        
    }
#endif

#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_SYMBIAN)
	icf_msg_st 			*p_pltfm_req=ICF_NULL,\
						*p_pltfm_rsp=ICF_NULL;
	icf_port_pltfm_host_to_ip_addr_req_st  * p_host_to_ip_req=ICF_NULL;
	icf_port_pltfm_host_to_ip_addr_resp_st * p_host_to_ip_resp=ICF_NULL;
	icf_port_pltfm_ret_val_t				 port_ret_val=ICF_PORT_PLTFM_SUCCESS;
	icf_error_t							 ecode; 	

	p_pltfm_req=icf_cmn_memget(p_persistent_glb_pdb, 
					  sizeof(icf_api_header_st) + 
					  sizeof(icf_port_pltfm_host_to_ip_addr_req_st),
		               (icf_pool_id_t)ICF_MEM_COMMON, &ecode);

	p_pltfm_rsp=icf_cmn_memget(p_persistent_glb_pdb, 
					  sizeof(icf_api_header_st) + 
					  sizeof(icf_port_pltfm_host_to_ip_addr_resp_st),
		               (icf_pool_id_t)ICF_MEM_COMMON, &ecode);

	p_host_to_ip_req =(icf_port_pltfm_host_to_ip_addr_req_st *) p_pltfm_req->payload;
	p_host_to_ip_resp =(icf_port_pltfm_host_to_ip_addr_resp_st *) p_pltfm_rsp->payload;


	if( (ICF_NULL != p_pltfm_req) && (ICF_NULL != p_pltfm_rsp))
	{
		p_pltfm_req->hdr.version = ICF_VERSION_1_0;
        p_pltfm_req->hdr.source_id = 0;
        p_pltfm_req->hdr.destination_id = 0;
        p_pltfm_req->hdr.call_id = 0;
        p_pltfm_req->hdr.app_id =  0;
        p_pltfm_req->hdr.api_length = sizeof(icf_api_header_st) + 
									  sizeof(icf_port_pltfm_host_to_ip_addr_req_st);
	}


	icf_port_strcpy((icf_uint8_t *)p_host_to_ip_req->str,(icf_uint8_t *)pHost);
	p_host_to_ip_req->str_len=\
		(icf_uint16_t)icf_port_strlen(p_host_to_ip_req->str);
	/*Call to platform wrapper*/
	p_pltfm_req->hdr.api_id=ICF_PORT_PLTFM_HOST_TO_IP_REQ;

	icf_port_pltfm_get_host_ip(p_pltfm_req,
							   &port_ret_val,
							   p_pltfm_rsp);

	if(ICF_PORT_PLTFM_FAILURE==port_ret_val)
	{
		*ppDest=ICF_NULL;
		ret_val=ICF_FAILURE;
	}
	else
	*ppDest=(icf_int8_t *)icf_port_strdup(p_host_to_ip_resp->str);

	icf_cmn_memfree(p_persistent_glb_pdb, (icf_void_t*)p_pltfm_req, ICF_MEM_COMMON, &ecode);
	icf_cmn_memfree(p_persistent_glb_pdb, (icf_void_t*)p_pltfm_rsp, ICF_MEM_COMMON,&ecode);
#endif/*PORT_WINDOWS-symbian*/	
	return ret_val;
    
}


/****************************************************************************************
 *      Porting Of Wrapper Functions
 *****************************************************************************************/
#ifdef IMS_CLIENT
/*------------------------------------------------------------------------------
 * FUNCTION:    icf_port_cbk_recv_from_wrpr
 *
 * DESCRIPTION: This function is the callback invoked by wrapper whenver it 
 *				receives a response or unsolicited indication from underlying 
 *				platform.
 *
 * RETURNS:     ICF_SUCCESS or ICF_FAILURE
 *              
 *----------------------------------------------------------------------------*/
icf_return_t icf_port_cbk_recv_from_wrpr(
	INOUT	icf_msg_st		*p_msg)
{
	return icf_es_cbk_recv_from_pltfm(p_msg);
} /* End function icf_port_cbk_recv_from_wrpr() */



#endif
/*******************************************************************************
 * FUNCTION:        icf_port_compare_ip_addr
 *
 * DESCRIPTION:     This function compares two transport addresses
 *                  
 * RETURNS          ICF_SUCCESS: In case src_addr == dest_addr
 *					ICF_FAILURE: In case src_addr != dest_addr
 ******************************************************************************/
icf_return_t  icf_port_compare_ip_addr(
IN		 icf_transport_addr_st src_addr,
IN		 icf_transport_addr_st dest_addr){
#ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage src_ip_addr;
	struct sockaddr_storage dest_ip_addr;
#endif
	if(src_addr.addr_type != dest_addr.addr_type){
		return ICF_FAILURE;
	}
	switch (src_addr.addr_type)
	{
		case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
			if ( (src_addr.addr.ipv4_addr.octet_1 != 
                 dest_addr.addr.ipv4_addr.octet_1) ||
                (src_addr.addr.ipv4_addr.octet_2 != 
                 dest_addr.addr.ipv4_addr.octet_2) ||
                (src_addr.addr.ipv4_addr.octet_3 != 
                 dest_addr.addr.ipv4_addr.octet_3) ||
                (src_addr.addr.ipv4_addr.octet_4 != 
                 dest_addr.addr.ipv4_addr.octet_4)
               )
			{
				return ICF_FAILURE;
			}
			break;
#ifdef ICF_IPV6_ENABLED
		case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
			memset((void *)&src_ip_addr,0,sizeof(struct sockaddr_storage));
			memset((void *)&dest_ip_addr,0,sizeof(struct sockaddr_storage));
			if(icf_port_pton(src_addr.addr.ipv6_addr.str,&src_ip_addr) == ICF_FAILURE){
				return ICF_FAILURE;
			}
			if(icf_port_pton(dest_addr.addr.ipv6_addr.str,&dest_ip_addr) == ICF_FAILURE){
				return ICF_FAILURE;
			}
			if(memcmp((void *)&src_ip_addr,(void *)&dest_ip_addr,sizeof(struct sockaddr_storage)) != 0) 
			{
				return ICF_FAILURE;
			}
			break;
#endif
	}
	return ICF_SUCCESS;
}

/*******************************************************************************
 * FUNCTION:        icf_port_pton
 *
 * DESCRIPTION:     This function converts string format of IP address
 *					to binary format. Converted address is returned in
 *					ip_binary_fmt.
 *                  
 * RETURNS          ICF_SUCCESS: In case conversion succeeds
 *					ICF_FAILURE: In case conversion fails
 ******************************************************************************/
icf_return_t icf_port_pton(
IN		char *ip_string_fmt, 
#ifdef ICF_IPV6_ENABLED
OUT		struct sockaddr_storage *ip_binary_fmt){
#else
OUT		struct sockaddr_in *ip_binary_fmt){
#endif

#ifdef ICF_IPV6_ENABLED
	struct sockaddr_in *sin = NULL;
	struct sockaddr_in6 *sin6 = NULL;
#ifdef ICF_PORT_WINDOWS
	icf_uint32_t len = 0;
#endif
#endif

#ifdef ICF_IPV6_ENABLED
	sin = (struct sockaddr_in *)ip_binary_fmt;
	sin6 = (struct sockaddr_in6 *)ip_binary_fmt;
#ifndef ICF_PORT_WINDOWS
	if(inet_pton(AF_INET, ip_string_fmt, (void *)&(sin->sin_addr))){
		ip_binary_fmt->ss_family = AF_INET;
		return ICF_SUCCESS;
	}

	if(inet_pton(AF_INET6, ip_string_fmt, (void *)&(sin6->sin6_addr))){
		ip_binary_fmt->ss_family = AF_INET6;
		return ICF_SUCCESS;
	}
#else
	len = sizeof(struct sockaddr_in);
	if(WSAStringToAddress(ip_string_fmt, AF_INET, 0,(struct sockaddr *)sin,&len) ==0){
		ip_binary_fmt->ss_family = AF_INET;
		return ICF_SUCCESS;
	}
	len = sizeof(struct sockaddr_in6);
	if(WSAStringToAddress(ip_string_fmt, AF_INET6, 0,(struct sockaddr *)sin6,&len) ==0){
		ip_binary_fmt->ss_family = AF_INET6;
		return ICF_SUCCESS;
	}
	return ICF_FAILURE;
#endif
#else
	/* Fix for SPR 19199 */
    /* System call inet_addr() is replaced by inet_aton() to resolve a UMR.
       The constant INADDR_NONE is returned by inet_addr() for malformed requests.
       The value INADDR_NONE (0xffffffff) is a valid broadcast address, but
       inet_addr() cannot return that value without indicating failure.  Also,
       inet_addr() should have been designed to return a struct in_addr.  The
       newer inet_aton() function does not share these problems.
       
       Prototype of inet_aton() :
           int inet_aton(const char *cp, struct in_addr *inp);
       
       inet_aton() converts the Internet host address 'cp' from the IPv4
       numbers-and-dots notation into binary form (in network byte order)
       and stores it in the structure that 'inp' points to. inet_aton()
       returns non-zero if the address is valid, zero if not.
    */   

    if(inet_aton((char*)ip_string_fmt,&(ip_binary_fmt->sin_addr)) == 0)
    {
        /* Malformed/invalid IP address passed as an argument */
        return ICF_FAILURE;
    }
	return ICF_SUCCESS;
#endif
}

/*******************************************************************************
 * FUNCTION:        icf_port_ntop
 *
 * DESCRIPTION:     This function converts binary format of IP address
 *					to string format. Converted address is returned in
 *					ip_string_fmt.
 *                  
 * RETURNS          ICF_SUCCESS: In case conversion succeeds
 *					ICF_FAILURE: In case conversion fails
 ******************************************************************************/
icf_return_t icf_port_ntop(
#ifdef ICF_IPV6_ENABLED
IN		struct sockaddr_storage *ip_binary_fmt, 
#else
IN		struct sockaddr_in *ip_binary_fmt,
#endif 
OUT		char *ip_string_fmt){
#ifdef ICF_IPV6_ENABLED
	struct sockaddr_in *sin = NULL;
	struct sockaddr_in6 *sin6 = NULL;
	icf_uint16_t	port = 0;
	icf_uint8_t temp_ip[ICF_MAX_IPV6_ADDRESS_LENGTH];

#ifdef ICF_PORT_WINDOWS
	icf_uint32_t len = 0;
	icf_uint32_t ip_len = ICF_MAX_IPV6_ADDRESS_LENGTH;
#endif
#else /* IPv6 check */
	struct sockaddr_in *sin = NULL;
	icf_uint32_t ip_len = ICF_MAX_IPV4_ADDRESS_LENGTH;
#endif

#ifdef ICF_IPV6_ENABLED
	sin = (struct sockaddr_in *)ip_binary_fmt;
	sin6 = (struct sockaddr_in6 *)ip_binary_fmt;
#ifndef ICF_PORT_WINDOWS
	if(ip_binary_fmt.ss_family == AF_INET){
		sin = (struct sockaddr_in *)ip_binary_fmt;
		if(inet_ntop(AF_INET, (void *)&(sin->sin_addr),ip_string_fmt,
			ICF_MAX_IPV6_ADDRESS_LENGTH)){
			return ICF_SUCCESS;
		}
	}
	else{
		sin6 = (struct sockaddr_in6 *)ip_binary_fmt;
		if(inet_ntop(AF_INET6, (void *)&(sin6->sin6_addr),ip_string_fmt,
			ICF_MAX_IPV6_ADDRESS_LENGTH)){
			return ICF_SUCCESS;
		}
	}
#else
	if(ip_binary_fmt->ss_family == AF_INET){
		port = ntohs(sin->sin_port);
		sin->sin_port = htons(0);
		len = sizeof(struct sockaddr_in);
		if(WSAAddressToString((struct sockaddr *)sin,len,NULL,
			ip_string_fmt,&ip_len) ==0){
			sin->sin_port = htons(port);
			return ICF_SUCCESS;
		}
	}
	else{
		port = ntohs(sin6->sin6_port);
		sin6->sin6_port = htons(0);
		len = sizeof(struct sockaddr_in6);
		if(WSAAddressToString((struct sockaddr *)sin6,len,NULL,
			ip_string_fmt,&ip_len) ==0){
			icf_port_sprintf(temp_ip,"[%s]",ip_string_fmt);
			icf_port_strcpy(ip_string_fmt,temp_ip);
			sin6->sin6_port = htons(port);
			return ICF_SUCCESS;
		}
	}
	
	return ICF_FAILURE;
#endif
#else
#ifndef ICF_PORT_WINDOWS
    sin = (struct sockaddr_in *)ip_binary_fmt;
	if(sin->sin_family == AF_INET)
    {
		if(inet_ntop(AF_INET, (void *)&(sin->sin_addr),ip_string_fmt,
			ip_len))
        {
			return ICF_SUCCESS;
		}
	}
#else
	if(sin->sin_family == AF_INET)
    {
		port = ntohs(sin->sin_port);
		sin->sin_port = htons(0);
		len = sizeof(struct sockaddr_in);
		if(WSAAddressToString((struct sockaddr *)sin,len,NULL,
			ip_str,&ip_len) == 0)
        {
            icf_port_strcpy(ip_string_fmt,ip_str);
			sin->sin_port = htons(port);
			return ICF_SUCCESS;
		}
	}
#endif
	return ICF_FAILURE;
#endif

}	
/*******************************************************************************
 * FUNCTION:        icf_port_str_to_trans_addr
 *
 * DESCRIPTION:     This function is used to convert string ip 
 *					address to icf_transport_addr_st structure
 *                  
 * RETURNS          ICF_SUCCESS: In case conversion succeeds
 *					ICF_FAILURE: In case conversion fails
 ******************************************************************************/
icf_return_t icf_port_str_to_trans_addr(
IN		icf_uint8_t *ip_address,
OUT		icf_transport_addr_st *ip_trans_addr)
{
	icf_return_t		ret_val = ICF_SUCCESS;
	icf_int8_t 		*pTemp = ICF_NULL;
	icf_uint8_t 		*pOctet = ICF_NULL;
	/* Fix for CSR 1-7423906: SPR 19772 */
#ifdef ICF_PORT_DEBIAN
	icf_uint8_t			ip_address1[ICF_MAX_STR_LEN] = "\0";
#else
	icf_uint8_t			*ip_address1 = ICF_NULL;
	icf_error_t 		ecode = ICF_ERROR_NONE;
#endif
	icf_uint8_t 		dOctet = 0;

    #ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage ss_addr;
	ret_val = icf_port_pton(ip_address, &ss_addr);
    if(ICF_SUCCESS==ret_val)
	{
	    if(ss_addr.ss_family == AF_INET){
#endif
            if ((ICF_NULL == ip_address) ||
                    ( 0 == icf_port_strlen(ip_address)))
                return ICF_FAILURE;
            if('.' == ip_address[icf_port_strlen(ip_address)-1])
        	 {
	               ret_val = ICF_FAILURE;
        	 }
			/* Fix for CSR 1-7423906: SPR 19772 */
#ifdef ICF_PORT_DEBIAN
			icf_port_strncpy(ip_address1, ip_address, (ICF_MAX_STR_LEN -1));
            ip_address1[ICF_MAX_STR_LEN-1] = '\0';
#else
	         ip_address1 = icf_port_strdup(ip_address);
#endif

        	 pOctet = (icf_uint8_t *)icf_port_strtokr((icf_int8_t *)ip_address1,(const icf_int8_t *)".",&pTemp);
	         if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
        	 {
	              ip_trans_addr->addr.ipv4_addr.octet_1 = dOctet;
        	 }
	         else
	         {
        	     ret_val = ICF_FAILURE;
	         }
        	 pOctet =ICF_NULL;
	         pOctet = (icf_uint8_t *)icf_port_strtokr(pTemp,".",&pTemp);
	         if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
	         {
	              ip_trans_addr->addr.ipv4_addr.octet_2 = dOctet;
        	 }
	         else
        	 {
	             ret_val = ICF_FAILURE;
        	 }

	         pOctet =ICF_NULL;
	         pOctet = (icf_uint8_t *)icf_port_strtokr(pTemp,".",&pTemp);
        	 if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
	         {
        	      ip_trans_addr->addr.ipv4_addr.octet_3 = dOctet;
	         }	
	         else
        	 {
	             ret_val = ICF_FAILURE;
        	 }

	         pOctet =ICF_NULL;
        	 pOctet = (icf_uint8_t *)icf_port_strtokr(pTemp,".",&pTemp);
	         if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
	         {
        	      ip_trans_addr->addr.ipv4_addr.octet_4 = dOctet;
                  }
	         else
        	 {
	             ret_val = ICF_FAILURE;
        	 }

	         pOctet =ICF_NULL;
        	 pOctet = (icf_uint8_t *)icf_port_strtokr(pTemp,".",&pTemp);

	         if(ICF_SUCCESS == icf_is_valid_octet(pOctet,&dOctet))
        	 {
	             ret_val = ICF_FAILURE;
        	 }
	         else if(ICF_NULL != pOctet)
        	 {
	                 ret_val = ICF_FAILURE;
	         }
	         ip_trans_addr->addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR ;

#ifdef ICF_IPV6_ENABLED
	}
	else{
		strcpy(ip_trans_addr->addr.ipv6_addr.str, ip_address);
		ip_trans_addr->addr.ipv6_addr.str_len = strlen(ip_address);
		ip_trans_addr->addr_type = ICF_TRANSPORT_ADDRESS_IPV6_ADDR;
	}
	}
#endif

	if (ICF_FAILURE == ret_val)
	{
		ip_trans_addr->addr.domain.str_len = (icf_uint16_t)icf_port_strlen(ip_address);
                if (ip_trans_addr->addr.domain.str_len >= ICF_MAX_LONG_STR_LEN)
                {
                    ret_val = ICF_FAILURE;
                }
               else
                {
		    icf_port_strcpy(ip_trans_addr->addr.domain.str,ip_address);
		    ip_trans_addr->addr_type = ICF_TRANSPORT_ADDRESS_DOMAIN_NAME;
                    ret_val = ICF_SUCCESS;
                }
	}

	/* Fix for CSR 1-7423906: SPR 19772 */
#ifndef ICF_PORT_DEBIAN
    icf_cmn_memfree(p_persistent_glb_pdb, ip_address1,
	                (icf_pool_id_t)ICF_MEM_COMMON, &ecode);
#endif

	return ret_val;
}

/***************************************************************************
 **
 ** FUNCTION:     icf_port_check_if_ip
 ** DESCRIPTION:  This function checks if the string passed is an IP address
 **               
 **
 ***************************************************************************/
icf_return_t icf_port_check_if_ip(icf_uint8_t *p_ip_address)
{

#ifdef ICF_IPV6_ENABLED
	struct sockaddr_storage binary_addr;
#else
	struct sockaddr_in binary_addr;
#endif
	if(p_ip_address == NULL){
		return ICF_FAILURE;
	}
	return icf_port_pton((icf_int8_t *)p_ip_address,&binary_addr);
}

#ifdef ICF_IPV6_ENABLED
/***************************************************************************
 **
 ** FUNCTION:     icf_port_get_family_type
 ** DESCRIPTION:  This Function will set  ICF_PORT_IPV6_FAMILY for Ipv6 
 **				  Address, ICF_PORT_IPV4_FAMILY for IPv4 address 
 **				  in dType
 **
 ***************************************************************************/
icf_return_t	icf_port_get_family_type(
IN	icf_uint8_t *p_ip_address, 
IN icf_uint16_t dPort,
OUT	icf_uint32_t *dType)
{
#if defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_VXWORKS)
	icf_port_addrinfo *p_ai_response = ICF_NULL;
	icf_uint8_t  *p_port = ICF_NULL;
	icf_uint32_t error_id = 0;
	if(dType == 0 && p_ip_address == ICF_NULL)
	{
		return ICF_FAILURE;
	}
	else if(dType != 0)
	{
		p_port = (icf_uint8_t*)malloc(6);
		if(p_port == ICF_NULL)
		{
			return ICF_FAILURE;
		}
		icf_port_sprintf(p_port,"%d",dPort);
	}
	if((error_id = getaddrinfo(p_ip_address,p_port,ICF_NULL,&p_ai_response)) == 0){
		if(p_ai_response->ai_family == AF_INET6)
		{
			*dType = ICF_PORT_IPV6_FAMILY;
		}
		else if(p_ai_response->ai_family == AF_INET)
		{
			*dType = ICF_PORT_IPV4_FAMILY;
		}
		else
		{
			if(p_port != ICF_NULL)
			{
				free((icf_void_t*)p_port);
			}
			return ICF_FAILURE;
		}
	}
	else
	{
		printf("%d",WSAGetLastError());
		if(p_port != ICF_NULL)
		{
			free((icf_void_t*)p_port);
		}
		return ICF_FAILURE;
	}
	if(p_port != ICF_NULL)
	{
		free((icf_void_t*)p_port);
	}
	return ICF_SUCCESS;
#endif
	return ICF_SUCCESS;
}

#endif

/***************************************************************************
 **
 ** FUNCTION:     icf_port_address_match
 ** DESCRIPTION:  This Function compares 2 variables of type 
 **				   icf_transport_address_st
 **
 ***************************************************************************/
icf_return_t icf_port_address_match(
				IN	icf_transport_address_st  *p_address1,
				IN	icf_transport_address_st  *p_address2)
{
    icf_return_t                         ret_val = ICF_SUCCESS;

	if (p_address1->port_num == p_address2->port_num)
	{
		if (p_address1->addr.addr_type == p_address2->addr.addr_type)
		{
			switch(p_address1->addr.addr_type)
			{
				case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
					if((p_address1->addr.addr.domain.str_len != p_address2->addr.addr.domain.str_len)
						|| (ICF_NULL != icf_port_strcmp(p_address1->addr.addr.domain.str, 
									p_address2->addr.addr.domain.str)))
					{
						ret_val = ICF_FAILURE;
					}
					break;
				
				case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
					if((p_address1->addr.addr.ipv4_addr.octet_1 != 
						p_address2->addr.addr.ipv4_addr.octet_1)
						|| (p_address1->addr.addr.ipv4_addr.octet_2 != 
						p_address2->addr.addr.ipv4_addr.octet_2)
						|| (p_address1->addr.addr.ipv4_addr.octet_3 != 
						p_address2->addr.addr.ipv4_addr.octet_3)
						|| (p_address1->addr.addr.ipv4_addr.octet_4 != 
						p_address2->addr.addr.ipv4_addr.octet_4))
					{
						ret_val = ICF_FAILURE;
					}
					break;
				
				case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
					if((p_address1->addr.addr.ipv6_addr.str_len != p_address2->addr.addr.ipv6_addr.str_len)
						|| (ICF_NULL != icf_port_strcmp(p_address1->addr.addr.ipv6_addr.str, 
									p_address2->addr.addr.ipv6_addr.str)))
					{
						ret_val = ICF_FAILURE;
					}
					break;
				case ICF_TRANSPORT_ADDRESS_INVALID:
				default:
					{
						ret_val = ICF_FAILURE;
					}
					break;
			}
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
    else
	{
		ret_val = ICF_FAILURE;
	}

	return(ret_val);
}

/*************************************************************************
*  FUNCTION NAME  :   icf_port_strcmpi
*  DESCRIPTION    :   Utility function for performing case-insensitive
*  			string comparison
 *************************************************************************/
icf_uint8_t icf_port_strcmpi(
		IN  icf_uint8_t	        *p_target,
		IN  icf_uint8_t			*p_match)
{
	icf_uint32_t							i = 0;
	icf_uint8_t							temp1 = 0;
	icf_uint8_t							temp2 = 0;

	while(p_target[i])
	{ 
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((p_target[i] >= 97) && (p_target[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp1 = (icf_uint8_t)(p_target[i] - 32);
		}
		else
		{
			temp1 = p_target[i];
		}
		/* lower case alphabets... ascii value ... 97 .. 122 */
		if ((p_match[i] >= 97) && (p_match[i] <= 122))
		{
			/* lower case - upper case = 32 */
			temp2 = (icf_uint8_t)(p_match[i] - 32);
		}
		else
		{
			temp2 = p_match[i];
		}

		if (temp1 != temp2)
			return((icf_uint8_t)(temp1 - temp2));
		i++;
	}

	if(p_match[i])
		return((icf_uint8_t)(0 - p_match[i]));

	return 0;
}


/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strstri
*
*  DESCRIPTION    :   Utility function for finding sub-string 
*                     within a string without case senstivity
*
*  RETURNS        :   Pointer to the location of match, or NULL if no match
*
 *************************************************************************/
icf_uint8_t *icf_port_strstri(
		IN	icf_void_t           *p_port_info,
		IN 	icf_uint8_t			*p_target,
		IN	icf_uint8_t			*p_match,
		OUT icf_error_t			*p_ecode)
{
    icf_return_t							ret_val = ICF_FAILURE;
	icf_uint8_t							*p_target_temp = ICF_NULL;
	icf_uint8_t							*p_match_temp = ICF_NULL;
	icf_uint8_t							*p_temp = ICF_NULL;
	icf_uint8_t                          *p_temp_1 = ICF_NULL;
	icf_uint32_t                         len1, len2, i=0;

	len1 = icf_port_strlen(p_target)+1;
	len2 = icf_port_strlen(p_match)+1;

	/* Allocate memory */
	if ( ICF_NULL == (p_target_temp = (icf_uint8_t*)icf_port_memget(p_port_info, len1,
		ICF_MEM_COMMON, p_ecode)))
	{
		return(ICF_NULL);
	}
	else if ( ICF_NULL == (p_match_temp = (icf_uint8_t*)icf_port_memget(p_port_info, len2,
		ICF_MEM_COMMON, p_ecode)))
	{
		icf_port_memfree (p_port_info, (icf_void_t*)p_target_temp, ICF_MEM_COMMON, p_ecode);
		return(ICF_NULL);
	}
	else
	{
		icf_port_memset(p_target_temp, 0x00, len1);
		icf_port_memset(p_match_temp, 0x00, len2);

		for (i=0; p_target[i]!='\0'; i++)
		{ 
			/* lower case alphabets... ascii value ... 97 .. 122 */
			if ((p_target[i] >= 97) && (p_target[i] <= 122))
			{
				/* lower case - upper case = 32 */
				p_target_temp[i] = (icf_uint8_t)  (p_target[i] - 32);
			}
			else
			{
				p_target_temp[i] = p_target[i];
			}
		}
		p_target_temp[i] = '\0';

		for (i=0; p_match[i]!='\0'; i++)
		{ /* lower case alphabets... ascii value ... 97 .. 122 */
			if ((p_match[i] >= 97) && (p_match[i] <= 122))
			{
				/* lower case - upper case = 32 */
				p_match_temp[i] = (icf_uint8_t)(p_match[i] - 32);
			}
			else
			{
				p_match_temp[i] = p_match[i];
			}
		}
		p_match_temp[i] = '\0';
		
		p_temp = (icf_uint8_t *)icf_port_strstr((const icf_int8_t *)p_target_temp,(const icf_int8_t *)p_match_temp);

		if(ICF_NULL != p_temp)
		{
			p_temp_1 = ((icf_uint8_t *)p_target + (p_temp - p_target_temp));
		}
		else
		{
			p_temp_1 = ICF_NULL;
		}

		icf_port_memfree (p_port_info, (icf_void_t*)p_target_temp, ICF_MEM_COMMON, p_ecode);
		icf_port_memfree (p_port_info, (icf_void_t*)p_match_temp, ICF_MEM_COMMON, p_ecode);
		ret_val = ret_val;
		return p_temp_1;
	}
}

icf_void_t icf_port_strlwr(
    INOUT icf_uint8_t *string)
{
  icf_uint8_t str_len = 0,counter = 0;
  str_len = (icf_uint8_t) icf_port_strlen(string);
  for(;counter<str_len;counter++)
  {
    if(65<= *(string + counter) && 90>= *(string + counter))
    {
      *(string + counter) += 32;
    }
  }
  
}

/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_itoa
*
*  DESCRIPTION    :   Utility function for converting integet value to string
*
*  RETURNS        :   Pointer to the string formed
*
 *************************************************************************/

icf_uint8_t *icf_port_itoa(icf_int32_t num,icf_uint8_t *p_dest)
{
    icf_uint8_t  str_index;
    icf_uint8_t  temp[100];
    icf_boolean_t is_neg = ICF_FALSE;
    icf_uint8_t digit=0;

    str_index = 0;
    if(num < 0)
    {
        is_neg = ICF_TRUE;
        num *= -1;
    }

    
    do
    {
        digit = (icf_uint8_t) (num % 10);
        temp[str_index++] =  (icf_uint8_t) (digit + '0');
        num /= 10;
    }while(num);

    if(ICF_TRUE == is_neg)
    {
        *p_dest = '-';
        p_dest++;
    }
    while(str_index--)
    {
        *p_dest = temp[str_index];
        p_dest++;
    }
    *p_dest = '\0';
    return p_dest;
}
#ifdef ICF_REL_OLD
/*-------------------------------------------------------------------
 *
 *  FUNCTION NAME  :   icf_port_atoi
 *
 *  DESCRIPTION    : Returns the integer value for the string input set by the
 *                   customer on the LFGU command line
 *
 *  RETURNS        : Returns the integer value for the string input 
 *---------------------------------------------------------------------*/
icf_uint32_t icf_port_atoi 
(
    IN      icf_uint8_t     *p_string
)
{
    /**p_val = atoi(p_string);*/
    return (atoi(p_string)); 
}
#endif
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_strstri2
*
*  DESCRIPTION    :   Utility function for finding sub-string 
*                     within a string without case senstivity
*
*  RETURNS        :   Pointer to the location of match, or NULL if no match
*
 *************************************************************************/
icf_uint8_t *icf_port_strstri2(
		IN 	icf_uint8_t			*p_target,
		IN	icf_uint8_t			*p_match)
{
    icf_uint8_t target[ICF_MAX_STR_LEN],match[ICF_MAX_STR_LEN];
    icf_uint8_t *p_ch = ICF_NULL;
    icf_port_strcpy(target,p_target);
    icf_port_strcpy(match,p_match);
    icf_port_strlwr(match);
    icf_port_strlwr(target);
    p_ch = (icf_uint8_t *)icf_port_strstr((const icf_int8_t *)target,(const icf_int8_t *)match);
    if(ICF_NULL != p_ch)
        p_ch = (icf_uint8_t*)p_target + (target - p_ch);

    return p_ch;
}

icf_return_t icf_port_cbk_RsporIndCallBack(
				icf_msg_st      *p_msg)
{
	icf_es_cbk_recv_from_pltfm(p_msg);
	return ICF_SUCCESS;
}

icf_return_t icf_port_pltfm_send(void * p_msg)
{

   icf_return_t		        ret_val = ICF_SUCCESS;
   icf_error_t			ecode = ICF_NULL;
  /* Send to GPRS network */
   icf_cmn_send((icf_glb_pdb_st*)p_persistent_glb_pdb,
					(icf_msg_st*)p_msg,
					&ecode);

   return ret_val;
}



/******************************************************************************
 ** FUNCTION:    icf_is_valid_octet
 **
 ** DESCRIPTION: This function check if valid octet is recived in string
 ******************************************************************************/
icf_return_t icf_is_valid_octet(icf_uint8_t* p_string,icf_uint8_t *p_number)
{

      icf_uint32_t num=0;
      icf_uint8_t i=0;
      icf_return_t  ret_val=ICF_SUCCESS;

      if(ICF_NULL == p_string)
      {
          return ICF_FAILURE;
      }
      for(i=0;i<strlen((const icf_int8_t *)p_string);i++)
      {
          if(!isdigit(p_string[i]))
          {
               return ICF_FAILURE;
          }
      }
      num = atoi((const icf_int8_t *)p_string);
      if(num>255)
      {
           return ICF_FAILURE;
      }
      else
      {
           *p_number = (icf_uint8_t)num;
      }
      return ret_val;
}


#ifdef ICF_DNS_LOOKUP_ENABLED
/*****************************************************************************
 * FUNCTION:        icf_port_is_nameserver_pres
 *
 * DESCRIPTION:     The function checks for the presence of nameserver in the 
 *                  resolv.conf file
 * 
 * RETURNS:         Returns ICF_TRUE if the nameserver is present else returns 
 *                  false
 *
 *****************************************************************************/

icf_boolean_t icf_port_is_nameserver_pres(icf_void_t)
{
    icf_boolean_t ret_val = ICF_FALSE;

#ifndef ICF_PORT_VXWORKS
    char buf[ICF_CONF_MAX_LINE_SIZE]={'\0'};
    char temp_str[ICF_CONF_MAX_LINE_SIZE]={'\0'};
    char val_str[ICF_CONF_MAX_LINE_SIZE]={'\0'};
    FILE *fp;

    if ((fp = icf_port_fopen (ICF_PATH_RESOLV_CONF, "r")) == NULL)
    {
        /* add a trace here */
        ICF_PRINT(((icf_uint8_t *)"ERROR: Could not open configuration file %s \n", 
            ICF_PATH_RESOLV_CONF));
	ret_val = ICF_FALSE;
	/* exit */
    }
    else
    {
	for(;;)
	{
	     if (icf_port_fgets(buf, ICF_CONF_MAX_LINE_SIZE, fp) == \
                              ICF_NULL )
	     {
		 if ( feof(fp))
		 {
		       break;
		 }
	     }
	     else
	     {
		 if(icf_port_strmatch( (icf_uint8_t *)"nameserver",(icf_uint8_t *)buf )==0)
		 { 
		      icf_port_sscanf(buf, "%s %s\n", 
				      temp_str, val_str);
		      if(icf_port_strmatch( (icf_uint8_t *)val_str,(icf_uint8_t *)"" )!=0)
		      {
			  ret_val = ICF_TRUE;
		      }     
		 }
		 buf[0]='\0';
		 temp_str[0]='\0';
		 val_str[0]='\0';
	     }
	}
	fclose(fp);
    }

#else
    /* In VxWorks, /etc/resolv.conf does not exist. */
    ret_val = ICF_TRUE;
#endif

   return ret_val;
}
#endif
#ifdef ICF_PORT_VXWORKS
/*******************************************************************************
 * Function name         :      icf_port_get_file_from_host
 * Return Value          :      icf_return_t
 * Functionality         :      To get the any file from host.
 *                              Default implementation is to FTP the file.
 * Functions called      :      None.
 ******************************************************************************/
icf_return_t icf_port_get_file_from_host(IN icf_ftp_info_st *p_ftp_info,
                                 IN icf_uint8_t *p_host_file,
                                 IN icf_uint8_t *p_target_file)
{
    icf_int32_t    ctrlSock;
    icf_int32_t    dataSock;
    icf_int16_t    nBytes;
    icf_int32_t    fileptr;
    icf_uint8_t     file_vx[ICF_CONF_FILE_NAME_SIZE];
    icf_uint8_t     file_buf[ICF_BYTES_PER_BLOCK];
       
    ICF_PRINT(("\nEntering Function icf_get_file_from_host"));
    if (ICF_FALSE == g_icf_is_ramdisk_present)
    {
        ICF_PRINT(("\nFAILURE :: RAM drive not present"));
        ICF_PRINT(("\nExiting Function icf_get_file_from_host"));
        return(ICF_FAILURE);
    }

    strcpy((icf_int8_t*)file_vx, ICF_VX_VOLUME_LABEL);
    strcat((icf_int8_t*)file_vx,(icf_int8_t*)p_target_file);
    fileptr = creat(file_vx, O_RDWR);
    if(ERROR == fileptr)
    {
        ICF_PRINT(("\nFAILURE :: Unable to create file on ram drive!"));
        ICF_PRINT(("\nExiting Function icf_get_file_from_host"));
        return ICF_FAILURE;
    }

    /*STATUS ftpXfer
             (
              char *host,        name of server host 
              char *user,        user name for host login 
              char *passwd,      password for host login 
              char *acct,        account for host login 
              char *cmd,         command to send to host 
              char *dirname,     directory to 'cd' to before sending command 
              char *filename,    filename to send with command 
              int  *pCtrlSock,   where to return control socket fd 
              int  *pDataSock    where to return data socket fd, 
                                 (NULL == don't open data connection) 
             )*/ 
    if (ftpXfer((icf_int8_t*)(p_ftp_info->ftp_server_ip),
                (icf_int8_t*)(p_ftp_info->login),
                (icf_int8_t*)(p_ftp_info->password),
                "", "RETR %s", (icf_int8_t*)(p_ftp_info->config_path),
                (icf_int8_t*)p_host_file, &ctrlSock,
                &dataSock) == ERROR)
    {
        ICF_PRINT(("\nFAILURE :: FTP to server failed!"));
        ICF_PRINT(("\nExiting Function icf_get_file_from_host"));
        close(fileptr);
        return(ICF_FAILURE);
    }

    while ((nBytes = read (dataSock, file_buf, sizeof (file_buf))) > 0)
    {
        if (write(fileptr, file_buf, nBytes) != nBytes)
        {
            ICF_PRINT(("\nWrite on RAM file failed"));
            close(fileptr);
            close (dataSock);
            close (ctrlSock);
            return(ICF_FAILURE);
        }
        ICF_PRINT(("\n%d Bytes read successfully", nBytes));
    }
    ICF_PRINT(("\nFile %s read successfully", p_host_file));

    close(fileptr);
    close (dataSock);

    if (ftpReplyGet(ctrlSock, TRUE) != FTP_COMPLETE)
    {
        ICF_PRINT(("\nFAILURE :: FTP Reply failed!"));
        ICF_PRINT(("\nExiting Function icf_get_file_from_host"));
        close (ctrlSock);
        return ICF_FAILURE;
    }
    
    if (ftpCommand (ctrlSock, "QUIT", 0, 0, 0, 0, 0, 0) != FTP_COMPLETE)
    {
        ICF_PRINT(("\nFAILURE :: Control socket Quit failed!"));
        ICF_PRINT(("\nExiting Function icf_get_file_from_host"));
        close (ctrlSock);
        return ICF_FAILURE;
    }
    
    close (ctrlSock);
 
    ICF_PRINT(("\nSUCCESS :: FTP File successful"));
    ICF_PRINT(("\nExiting Function icf_get_file_from_host"));
    return(ICF_SUCCESS);
}

#endif

/***************************************************************************
 ** FUNCTION:     icf_port_create_numeric_hash_table
 ** DESCRIPTION:  This function is used to create and initalize numeric   
 **               hash table. On success this function will return an id to
 **               the newly created hash table. this will be used for 
 **				  adding/removing/fetching data from this hash table. 
 **
 ***************************************************************************/
icf_return_t icf_port_create_numeric_hash_table(
	IN	icf_int_module_id_t module_id,
	OUT	icf_hash_table_id_t *p_hash_table_id,
	IN	icf_uint32_t max_key_qty,
	IN	icf_uint32_t max_num_bucket,
	OUT	icf_error_t	*p_ecode)
{
    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    icf_return_t                icf_retval = ICF_SUCCESS;

    /* The default hash function provided by the genlib is being used at present. */
    /* Calling the function in the genlib to create the hash table */
	if ( ICF_NULL == p_ecode)
	{
		return ICF_FAILURE;
	}
    gw_retval = gl_htm_create_hash_table (  module_id,
                                            p_hash_table_id,
                                            max_key_qty,
                                            max_num_bucket,
                                            GL_HTM_NUMERIC_KEY_TYPE, /* Numeric key type */
                                            GL_HTM_HASH_FUNC_FOR_NUMERIC_KEYS, 
                                            GW_NULL, /* hash function will be of genlib's */
                                            (gl_htm_free_key_elem_fn_t)icf_port_hash_free_element, /* pointer to element free func. */
                                            GW_NULL, /* pointer to hash argument*/
                                            GW_NULL, /* pointer to key compare function */
                                            &dErr    /* pointer to error code,in case of failure */
                                         );
	if ( GW_SUCCESS == gw_retval )                                             
    { 
    	return ( icf_retval );
    }
    else
    {
    	icf_retval = ICF_FAILURE;
         return ( icf_retval );
	} 
	return ICF_SUCCESS;
}

/***************************************************************************
 ** FUNCTION:     icf_port_create_string_hash_table
 ** DESCRIPTION:  This function is used to create and initalize string   
 **               hash table. On success this function will return an id to
 **               the newly created hash table. this will be used for 
 **				  adding/removing/fetching data from this hash table. 
 **
 ***************************************************************************/
icf_return_t icf_port_create_string_hash_table(
	IN	icf_int_module_id_t module_id,
	OUT	icf_hash_table_id_t *p_hash_table_id,
	IN	icf_uint32_t max_key_qty,
	IN	icf_uint32_t max_num_bucket,
	OUT	icf_error_t	*p_ecode)
{
	gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;

    icf_return_t                icf_retval = ICF_SUCCESS;

    /* The default hash function provided by the genlib is being used at present. */
    /* Calling the function in the genlib to create the hash table */
	if ( ICF_NULL == p_ecode)
	{
		return ICF_FAILURE;
	}

    gw_retval = gl_htm_create_hash_table (  module_id,
                                            p_hash_table_id,
                                            max_key_qty,
                                            max_num_bucket,
                                            GL_HTM_USER_DEFINED_KEY_TYPE, /* String key type */
                                            GL_HTM_HASH_FUNC_FOR_STRING_KEYS, 
                                            GW_NULL, /* hash function will be of genlib's */
                                            (gl_htm_free_key_elem_fn_t)icf_port_hash_free_element, /* pointer to element free func. */
                                            GW_NULL, /* pointer to hash argument*/
                                            GW_NULL, /* pointer to key compare function */
                                            &dErr    /* pointer to error code,in case of failure */
                                         );
  	if ( GW_SUCCESS == gw_retval )                                             
    { 
    	return ( icf_retval );
	}    
    else
    {
    	icf_retval = ICF_FAILURE;
    	 return ( icf_retval );
	} 	

}

/***************************************************************************
 ** FUNCTION:     icf_port_numeric_hash_add
 ** DESCRIPTION:  This function is used to add  p_element into the     
 **               numeric hash table identified by the id. 
 **
 ***************************************************************************/
icf_return_t icf_port_numeric_hash_add(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t hash_table_id,			
	IN	icf_uint32_t key,
	IN	icf_void_t *p_element,
	OUT	icf_error_t *p_ecode)
{
    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    gw_pvoid_t                   p_key_node_handle = GW_NULL;

    gl_hash_key_ut                hash_key;
    icf_return_t                icf_retval = ICF_SUCCESS;
	if ( ICF_NULL == p_ecode)
	{
		return ICF_FAILURE;
	}

    hash_key.numeric_id = key;

    /* Calling the function in the genlib to add a key,value pair to hash table */

    gw_retval = gl_htm_add_key_to_hash_table (  module_id,
                                                hash_table_id,
                                                &(hash_key),
                                                (gw_pvoid_t)p_element,
                                                &(p_key_node_handle),
                                                &dErr    /* pointer to error code,in case of failure */
                                             );
  	if ( GW_SUCCESS == gw_retval )                                             
    { 
    	return ( icf_retval );
    }
    else
    {
    	icf_retval = ICF_FAILURE;
        return ( icf_retval );
	} 

}

/***************************************************************************
 ** FUNCTION:     icf_port_string_hash_add
 ** DESCRIPTION:  This function is used to add  p_element into the     
 **               string hash table identified by the id. 
 **
 ***************************************************************************/
icf_return_t icf_port_string_hash_add(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t hash_table_id,			
	IN	icf_uint8_t *key,
	IN	icf_void_t *p_element,
	OUT	icf_error_t *p_ecode)
{
    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    gw_pvoid_t                   p_key_node_handle = GW_NULL;

    gl_hash_key_ut                hash_key;
    icf_return_t                icf_retval = ICF_SUCCESS;

	if ( ICF_NULL == p_ecode)
	{
		return ICF_FAILURE;
	}

	
    hash_key.p_user_def_id = (gl_pvoid_t)icf_cmn_memget(p_persistent_glb_pdb,
								icf_port_strlen(key) + 1,
								(icf_pool_id_t)ICF_MEM_COMMON,p_ecode);

   if (ICF_NULL == hash_key.p_user_def_id) 
   {
       return ICF_FAILURE;
   }
	icf_port_strcpy((icf_uint8_t*)(hash_key.p_user_def_id),key); 

    /* Calling the function in the genlib to add a key,value pair to hash table */

    gw_retval = gl_htm_add_key_to_hash_table (  module_id,
                                                hash_table_id,
                                                &(hash_key),
                                                (gw_pvoid_t)p_element,
                                                &(p_key_node_handle),
                                                &dErr    /* pointer to error code,in case of failure */
                                             );
	if ( GW_SUCCESS == gw_retval )                                             
 	{ 
 	 	return ( icf_retval );
  	}
  	else
  	{
  		icf_retval = ICF_FAILURE;
  	 	return ( icf_retval );
	} 
	return ICF_SUCCESS;

}

/***************************************************************************
 ** FUNCTION:     icf_port_numeric_hash_fetch
 ** DESCRIPTION:  This function is used to fetch p_element from the     
 **               numeric hash table identified by the id. returns p_elenent
 **				  on success. returns ICF_NULL on failure.
 **
 ***************************************************************************/
icf_void_t* icf_port_numeric_hash_fetch(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t hash_table_id,			
	IN	icf_uint32_t key,
	OUT	icf_error_t *p_ecode)
{
    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    icf_void_t                  *p_value   = GW_NULL;
    gl_hash_key_ut                hash_key;

    hash_key.numeric_id = key;

    /* Calling the function in the genlib to fetch the value associated with the supplied key */
	if ( ICF_NULL == p_ecode)
	{
		return ICF_NULL;
	}

    gw_retval = gl_htm_get_key_ref_from_hash_table (  module_id,
                                                  hash_table_id,
                                                  &(hash_key),
                                                  &(p_value),
                                                  &dErr    /* pointer to error code,in case of failure */
                                             );
  	if ( GW_FAILURE == gw_retval )                                             
    { 
    	p_value = GW_NULL; /*Just to make sure if failure occures,ptr returned will be NULL */
    }

	return p_value;
}

/***************************************************************************
 ** FUNCTION:     icf_port_string_hash_fetch
 ** DESCRIPTION:  This function is used to get  p_element from the     
 **               string hash table identified by the id. returns p_element
 **				  on success. returns ICF_NULL on failure  
 **
 ***************************************************************************/
icf_void_t* icf_port_string_hash_fetch(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t hash_table_id,			
	IN	icf_uint8_t *key,
	OUT	icf_error_t *p_ecode)
{
    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    icf_void_t                        *p_value   = GW_NULL;
	gl_hash_key_ut hash_key;

	hash_key.p_user_def_id = (gl_pvoid_t)key;
	if ( ICF_NULL == p_ecode)
	{
		return ICF_NULL;
	}

    /* Calling the function in the genlib to fetch the value associated with the supplied key */

    gw_retval = gl_htm_get_key_ref_from_hash_table (  module_id,
                                                  hash_table_id,
                                                  &(hash_key),
                                                  &(p_value),
                                                  &dErr    /* pointer to error code,in case of failure */
                                             );
  	if ( GW_FAILURE == gw_retval )                                             
    { 
    	p_value = GW_NULL; /*Just to make sure if failure occures,ptr returned will be NULL */
		return p_value;
    }
    else
    {
    	return(p_value);
    } 

}

/***************************************************************************
 ** FUNCTION:     icf_port_numeric_hash_remove
 ** DESCRIPTION:  This function is used to remove p_element from the     
 **               numeric hash table identified by the id. returns p_elenent
 **				  on success. returns ICF_NULL on failure.
 **
 ***************************************************************************/
icf_void_t* icf_port_numeric_hash_remove(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t hash_table_id,			
	IN	icf_uint32_t key,
	OUT	icf_error_t *p_ecode)
{
   gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                 dErr;
    gl_pvoid_t                  p_key_buf = GW_NULL;
    icf_void_t                        *p_value   = GW_NULL;
    gl_hash_key_ut                hash_key;

    hash_key.numeric_id = key;
	if ( ICF_NULL == p_ecode)
	{
		return ICF_NULL;
	}

    /* Calling the function in the genlib to remove the value associated with the supplied key */

    gw_retval = gl_htm_remove_key_from_hash_table (  module_id,
                                                     hash_table_id,
                                                     &(hash_key),
                                                     &(p_key_buf),
                                                     &(p_value),
                                                     &dErr    /* pointer to error code,in case of failure */
                                                  );
  	if ( GW_FAILURE == gw_retval )                                             
    { 
    	p_value = GW_NULL; /*Just to make sure if failure occures,ptr returned will be NULL */
    }
	return p_value;
}

/***************************************************************************
 ** FUNCTION:     icf_port_string_hash_remove
 ** DESCRIPTION:  This function is used to remove  p_element from the     
 **               string hash table identified by the id. returns p_element
 **				  on success. returns ICF_NULL on failure  
 **
 ***************************************************************************/
icf_void_t* icf_port_string_hash_remove(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t hash_table_id,			
	IN	icf_uint8_t *key,
	OUT	icf_error_t *p_ecode)
{

    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    gl_pvoid_t                  p_key_buf = GW_NULL;
    icf_void_t                        *p_value   = GW_NULL;
	gl_hash_key_ut hash_key;
	
	hash_key.p_user_def_id = (gl_pvoid_t)key;

    /* Calling the function in the genlib to remove the value associated with the supplied key */
	if ( ICF_NULL == p_ecode)
	{
		return ICF_NULL;
	}

    gw_retval = gl_htm_remove_key_from_hash_table (  module_id,
                                                     hash_table_id,
                                                     &(hash_key),
                                                     &(p_key_buf),
                                                     &(p_value),
                                                     &dErr    /* pointer to error code,in case of failure */
                                                  );
    icf_cmn_memfree(p_persistent_glb_pdb,
					(icf_void_t*)p_key_buf,
					(icf_pool_id_t)ICF_MEM_COMMON,p_ecode);
	
  	if ( GW_FAILURE == gw_retval )                                             
    { 
    	p_value = GW_NULL; /*Just to make sure if failure occures,ptr returned will be NULL */
    }
	return p_value;

}

/***************************************************************************
 ** FUNCTION:     icf_port_get_iterator_from_hash_table
 ** DESCRIPTION:  This function is used to get all the elements from    
 **               string hash table identified by the id. returns list of 
 **				  elements on success.  
 **
 ***************************************************************************/
icf_list_st* icf_port_get_iterator_from_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	OUT	icf_error_t *p_ecode)
{
	icf_int_module_id_t l_module_id = module_id;
	icf_hash_table_id_t l_id = id;
	l_module_id = l_module_id;
	l_id = l_id;
	p_ecode = p_ecode;
	return ICF_NULL; 
}

/************************************************************************************************
 * FUNCTION    icf_port_get_first_entry_from_string_hash_table
 *
 * DESCRIPTION:  This function returns the first element for the hash table
 *
 * *******************************************************************************************/
icf_void_t *icf_port_get_first_entry_from_string_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
    OUT icf_uint8_t  **p_p_key)
{
    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    /* gl_pvoid_t                  p_key_buf = GW_NULL; */
    icf_void_t                        *p_value   = GW_NULL;
    gl_hash_key_ut hash_key;


    /* Calling the function in the genlib to remove the value associated with the supplied key */

    gw_retval = gl_htm_get_first_key_ref_from_hash_table (  module_id,
            id,
            &(hash_key),
            &(p_value),
            &dErr    /* pointer to error code,in case of failure */
            );
    *p_p_key  = (icf_uint8_t *)hash_key.p_user_def_id;
    if ( GW_FAILURE == gw_retval )                                             
    { 
        p_value = GW_NULL; /*Just to make sure if failure occures,ptr returned will be NULL */
    }
    return p_value;

}

/************************************************************************************************
 * FUNCTION    icf_port_get_next_entry_from_string_hash_table
 *
 * DESCRIPTION:  This function returns the next element for the hash table
 * from the key provided
 *
 * *******************************************************************************************/
icf_void_t *icf_port_get_next_entry_from_string_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
    IN icf_uint8_t  *p_key,
    INOUT icf_uint8_t  **p_p_next_key)
{
    gw_return_t                 gw_retval = GW_SUCCESS;
    gw_error_t                  dErr;
    /* gl_pvoid_t                  p_key_buf = GW_NULL; */
    icf_void_t                        *p_value   = GW_NULL;
    gl_hash_key_ut hash_key;
    gl_hash_key_ut next_key;

    hash_key.p_user_def_id = (icf_void_t *)p_key;
    /* Calling the function in the genlib to remove the value associated with the supplied key */

    gw_retval = gl_htm_get_next_key_ref_from_hash_table (  module_id,
            id,
            &(hash_key),
            &(next_key),
            &(p_value),
            &dErr    /* pointer to error code,in case of failure */
            );
    *p_p_next_key  = (icf_uint8_t *)next_key.p_user_def_id;
    if ( GW_FAILURE == gw_retval )                                             
    { 
        p_value = GW_NULL; /*Just to make sure if failure occures,ptr returned will be NULL */
    }
    return p_value;

}

/***************************************************************************
 ** FUNCTION:     icf_port_hash_table_init
 ** DESCRIPTION:  This function initializes hash table manager    
 **
 ***************************************************************************/
icf_return_t icf_port_hash_table_init(icf_void_t)
{

	if ( GW_SUCCESS == gl_htm_init())
	{
		return ICF_SUCCESS;
	}
	return ICF_FAILURE; 
}

/*icf_return_t icf_port_hash_free_element(icf_void_t)*/
icf_uint32_t icf_port_hash_free_element(gl_hash_key_ut *p_key , icf_void_t *p_args)
{
	p_key = p_key;
	p_args = p_args;
	return ICF_SUCCESS;
}

icf_return_t icf_port_delete_hash_table(
	IN	icf_int_module_id_t module_id,
	IN	icf_hash_table_id_t id,			
	OUT	icf_error_t *p_ecode)
{

	gl_return_t gl_return = GL_SUCCESS;
	gl_error_t  dErr;

	if ( ICF_NULL == p_ecode)
	{
		return ICF_FAILURE;
	}	
	gl_htm_remove_all_keys_from_hash_table(module_id,id,&dErr);

	gl_return = gl_htm_delete_hash_table(module_id,id,ICF_NULL,&dErr);
	
	return gl_return;

}

#ifdef ICF_DNS_LOOKUP_ENABLED
/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_set_ares_fd
 *
 *  DESCRIPTION     :    This function sets the ares fd in the port data.
 *                       This is required in case when DNS REQ timer expires
 *                       IPTK should clear that FD from the read_fd_set.
 *  RETURNS         :    ICF_SUCCESS
 *************************************************************************/

icf_return_t icf_port_set_ares_fd(icf_global_port_info_st* p_glb_port_data,
                                  icf_uint32_t fd_id)
{
  icf_ares_fds_st* p_ares_port_info =
                        p_glb_port_data->p_ares_port_info;

  p_ares_port_info->curr_fd = fd_id;

 return ICF_SUCCESS;
}
#endif


#ifdef ICF_TRACE_ENABLE
/***************************************************************************
 ** FUNCTION:     icf_port_populate_trace_database
 ** DESCRIPTION:  This function populates the trace database values.
 **
 ***************************************************************************/
icf_void_t    icf_port_populate_trace_database(
        IN    icf_trace_database_st *p_trace_database_val)
{
    icf_error_t    ecode;

    icf_port_info_st *p_port_info =
        ((icf_port_info_st *)p_persistent_glb_pdb->p_glb_data->p_glb_port_info);

    p_port_info->p_trace_db_val = (icf_trace_database_st *)
            icf_port_memget(p_port_info,
            sizeof(icf_trace_database_st),
            ICF_MEM_COMMON, &ecode);

    icf_port_memcpy(p_port_info->p_trace_db_val,
                    p_trace_database_val,
                    ICF_PORT_SIZEOF(icf_trace_database_st));
    return;
}


/***************************************************************************
 ** FUNCTION:     icf_port_trace_match_criteria_value
 ** DESCRIPTION:  This function is the trace criteria matching function.
 **               The decision is taken on the basis of configured values
 **               of Trace Type/ Trace Criteria and Trace Rules.
 **               Complete Criteria is matching is valid for CALL_CONTROL.
 **               For REGISTRATION and SUBSCRIPTION traces LineID and Calling
 **               Party Address are the only vallid criterias. For other
 **               Trace types, Traces are always reported.
 **
 ***************************************************************************/
icf_return_t icf_port_trace_match_criteria_value(
        IN      icf_trace_type_t  trace_type,
        IN      icf_trace_criteria_t  trace_criteria,
        IN      icf_trace_rule_t  trace_rule,
        IN      icf_trace_criteria_data_st  *p_criteria_data)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_boolean_t    match_found = ICF_FALSE;
    icf_uint8_t        count = 0;

    icf_port_info_st *p_port_info =
        ((icf_port_info_st *)p_persistent_glb_pdb->p_glb_data->p_glb_port_info);
    icf_trace_database_st *p_trace_db = p_port_info->p_trace_db_val;

    /*
    For the following trace types, no filtering is required and traces are
    always reported.
    */
       if((ICF_TRACE_TYPE_CONFIGURATION == trace_type) ||
       (ICF_TRACE_TYPE_ERROR == trace_type) ||
       (ICF_TRACE_TYPE_SYSTEM == trace_type))
    {
       return ICF_SUCCESS;
    }
    else if(ICF_TRACE_CRITERIA_UNDEFINED == trace_criteria)
    {
        return ICF_SUCCESS;
    }
    else if(ICF_NULL == p_criteria_data)
    {
        return ICF_FAILURE;
    }

   /*
    For the following trace types , only LineID and Calling Party Address
    are the valid trace criteria. So the traces will be reported on the basis
    of these Criterias only.
   */
    if((ICF_TRACE_TYPE_REGISTRATION == trace_type) ||
       (ICF_TRACE_TYPE_SUBSCRIPTION == trace_type))
    {
    switch(trace_rule)
    {
    case ICF_TRACE_RULE_AND:
        if(ICF_TRACE_CRITERIA_LINE_ID ==
            (trace_criteria & ICF_TRACE_CRITERIA_LINE_ID))
    {
        if(ICF_TRACE_LINE_ID_PRESENT ==
            (ICF_TRACE_LINE_ID_PRESENT & p_criteria_data->bitmask))
            {
            for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
            {
                if(p_criteria_data->line_id == p_trace_db->line_id[count])
                {
                    match_found = ICF_TRUE;
                    break;
                }
            }

            if(ICF_FALSE == match_found)
            {
                return ICF_FAILURE;
            }
            else
            {
                match_found = ICF_FALSE;
            }

          }
          else
           {
              return ICF_FAILURE;
           }
        }

        if(ICF_TRACE_CRITERIA_CALLING_PARTY_ADDRESS ==
            (trace_criteria & ICF_TRACE_CRITERIA_CALLING_PARTY_ADDRESS))
        {
            if(ICF_TRACE_CALLING_PARTY_ADDR_PRESENT ==
            (ICF_TRACE_CALLING_PARTY_ADDR_PRESENT & p_criteria_data->bitmask))
            {
            for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
            {
                if(0 == icf_port_strcmp(
                    p_criteria_data->p_calling_party_address->addr_val.str,
                    p_trace_db->calling_party_address[count].addr_val.str))
                {
                    match_found = ICF_TRUE;
                    break;
                }
            }

            if(ICF_FALSE == match_found)
            {
                return ICF_FAILURE;
            }
            else
            {
                match_found = ICF_FALSE;
            }
          }
          else
          {
              return ICF_FAILURE;
          }
       }
        match_found = ICF_TRUE;
        break;

    case ICF_TRACE_RULE_OR:
        /* still needs to be verified here */
        match_found = ICF_TRUE;
        break;
    }
    }
    else if((ICF_TRACE_TYPE_SIP_INTERFACE == trace_type) ||
            (ICF_TRACE_TYPE_APP_INTERFACE == trace_type) ||
            (ICF_TRACE_TYPE_MEDIA_INTERFACE == trace_type) ||
            (ICF_TRACE_TYPE_CALL_CONTROL == trace_type))
    {
    switch(trace_rule)
    {
    case ICF_TRACE_RULE_AND:
        if(ICF_TRACE_CRITERIA_CALLING_PARTY_ADDRESS ==
           (trace_criteria & ICF_TRACE_CRITERIA_CALLING_PARTY_ADDRESS))
        {
                  if(ICF_TRACE_CALLING_PARTY_ADDR_PRESENT ==
                   (ICF_TRACE_CALLING_PARTY_ADDR_PRESENT & p_criteria_data->bitmask))
           {
           for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
           {
               if(0 == icf_port_strcmp(
                  p_criteria_data->p_calling_party_address->addr_val.str,
                  p_trace_db->calling_party_address[count].addr_val.str))
               {
                match_found = ICF_TRUE;
                break;
               }
            }

            if(ICF_FALSE == match_found)
            {
                return ICF_FAILURE;
            }
            else
            {
               match_found = ICF_FALSE;
            }
          }
          else
          {
             return ICF_FAILURE;
          }
        }

        if(ICF_TRACE_CRITERIA_CALLED_PARTY_ADDRESS ==
                  (trace_criteria & ICF_TRACE_CRITERIA_CALLED_PARTY_ADDRESS))
        {
            if (ICF_TRACE_CALLED_PARTY_ADDR_PRESENT ==
               (ICF_TRACE_CALLED_PARTY_ADDR_PRESENT & p_criteria_data->bitmask))
                  {
                    for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                    {
                       if(0 == icf_port_strcmp(
                          p_criteria_data->p_called_party_address->addr_val.str,
                          p_trace_db->called_party_address[count].addr_val.str))
                       {
                          match_found = ICF_TRUE;
                          break;
                       }
                     }

                     if(ICF_FALSE == match_found)
                     {
                        return ICF_FAILURE;
                     }
             else
             {
               match_found = ICF_FALSE;
             }

         }
         else
         {
             return ICF_FAILURE;
         }
      }

        if(ICF_TRACE_CRITERIA_LINE_ID ==
                   (trace_criteria & ICF_TRACE_CRITERIA_LINE_ID))
        {
            if (ICF_TRACE_LINE_ID_PRESENT ==
                  (ICF_TRACE_LINE_ID_PRESENT & p_criteria_data->bitmask))
            {
                   for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                   {
                        if(p_criteria_data->line_id == p_trace_db->line_id[count])
                        {
                            match_found = ICF_TRUE;
                            break;
                        }
                    }

                    if(ICF_FALSE == match_found)
                    {
                       return ICF_FAILURE;
                    }
                    else
                    {
                      match_found = ICF_FALSE;
                    }

              }
              else
              {
                 return ICF_FAILURE;
              }

        }

        if(ICF_TRACE_CRITERIA_INCOMING_CALL ==
                   (trace_criteria & ICF_TRACE_CRITERIA_INCOMING_CALL))
        {
           if(ICF_TRACE_CALL_DIR_PRESENT ==
                   (ICF_TRACE_CALL_DIR_PRESENT & p_criteria_data->bitmask))
           {
               if(ICF_CALL_DIRECTION_INCOMING ==
               (ICF_CALL_DIRECTION_INCOMING & p_criteria_data->call_direction))
               {
                  match_found = ICF_TRUE;
                }
                if(ICF_FALSE == match_found)
                {
                        return ICF_FAILURE;
                }
                else
                {
                   match_found = ICF_FALSE;
                 }

            }
            else
            {
                return ICF_FAILURE;
            }
        }

        if(ICF_TRACE_CRITERIA_OUTGOING_CALL ==
                   (trace_criteria & ICF_TRACE_CRITERIA_OUTGOING_CALL))
        {
            if(ICF_TRACE_CALL_DIR_PRESENT ==
                (ICF_TRACE_CALL_DIR_PRESENT & p_criteria_data->bitmask))
            {
                 if(ICF_CALL_DIRECTION_OUTGOING ==
                      (ICF_CALL_DIRECTION_OUTGOING & p_criteria_data->call_direction))
                    {
                       match_found = ICF_TRUE;
                    }

                    if(ICF_FALSE == match_found)
                    {
                       return ICF_FAILURE;
                    }
                    else
                    {
                        match_found = ICF_FALSE;
                    }

             }
             else
             {
                return ICF_FAILURE;
             }

        }

        if(ICF_TRACE_CRITERIA_CONNECTED_CALL ==
                  (trace_criteria & ICF_TRACE_CRITERIA_CONNECTED_CALL))
        {
                if(ICF_TRACE_CALL_TYPE_PRESENT ==
                   (ICF_TRACE_CALL_TYPE_PRESENT & p_criteria_data->bitmask))
                {
                    if(ICF_TRACE_CONNECTED_CALL ==
                      (ICF_TRACE_CONNECTED_CALL & p_criteria_data->call_type))
                    {
                       match_found = ICF_TRUE;
                    }

                    if(ICF_FALSE == match_found)
                    {
                       return ICF_FAILURE;
                     }
                     else
                     {
                        match_found = ICF_FALSE;
                     }

                 }
                 else
                 {
                    return ICF_FAILURE;
                 }
         }

        if(ICF_TRACE_CRITERIA_ALERTED_CALL ==
                   (trace_criteria & ICF_TRACE_CRITERIA_ALERTED_CALL))
         {
                 if(ICF_TRACE_CALL_TYPE_PRESENT ==
                   (ICF_TRACE_CALL_TYPE_PRESENT & p_criteria_data->bitmask))
                {
                    if(ICF_TRACE_ALERTED_CALL ==
                       (ICF_TRACE_ALERTED_CALL & p_criteria_data->call_type))
                    {
                       match_found = ICF_TRUE;
                    }

                    if(ICF_FALSE == match_found)
                    {
                       return ICF_FAILURE;
                    }
                    else
                    {
                       match_found = ICF_FALSE;
                    }

                 }
                 else
                 {
                   return ICF_FAILURE;
                 }

         }

        if(ICF_TRACE_CRITERIA_CALL_ON_UDP ==
                  (trace_criteria & ICF_TRACE_CRITERIA_CALL_ON_UDP))
        {
                   if(ICF_TRACE_TRANSPORT_MODE_PRESENT ==
                   (ICF_TRACE_TRANSPORT_MODE_PRESENT & p_criteria_data->bitmask))
                  {
                    if(ICF_CALL_TRANSPORT_MODE_UDP ==
                     (ICF_CALL_TRANSPORT_MODE_UDP & p_criteria_data->call_transport_mode))
                    {
                       match_found = ICF_TRUE;
                    }

                    if(ICF_FALSE == match_found)
                    {
                       return ICF_FAILURE;
                    }
                    else
                    {
                      match_found = ICF_FALSE;
                    }

                 }
                 else
                 {
                       return ICF_FAILURE;
                 }
         }

            if(ICF_TRACE_CRITERIA_CALL_ON_TCP ==
                  (trace_criteria & ICF_TRACE_CRITERIA_CALL_ON_TCP))
         {
               if(ICF_TRACE_TRANSPORT_MODE_PRESENT ==
                   (ICF_TRACE_TRANSPORT_MODE_PRESENT & p_criteria_data->bitmask))
                {
                    if(ICF_CALL_TRANSPORT_MODE_TCP ==
                      (ICF_CALL_TRANSPORT_MODE_TCP & p_criteria_data->call_transport_mode))
                    {
                       match_found = ICF_TRUE;
                    }

                    if(ICF_FALSE == match_found)
                    {
                       return ICF_FAILURE;
                    }
                    else
                     {
                        match_found = ICF_FALSE;
                    }

                }
                else
                {
                   return ICF_FAILURE;
                }
          }

         if(ICF_TRACE_CRITERIA_CALL_ON_TLS ==
                   (trace_criteria & ICF_TRACE_CRITERIA_CALL_ON_TLS))
         {
            if(ICF_TRACE_TRANSPORT_MODE_PRESENT ==
                   (ICF_TRACE_TRANSPORT_MODE_PRESENT & p_criteria_data->bitmask))
                {
                    if(ICF_CALL_TRANSPORT_MODE_TLS ==
                       (ICF_CALL_TRANSPORT_MODE_TLS & p_criteria_data->call_transport_mode))
                    {
                        match_found = ICF_TRUE;
                    }

                    if(ICF_FALSE == match_found)
                    {
                        return ICF_FAILURE;
                    }
                    else
                    {
                       match_found = ICF_FALSE;
                    }

                }
                else
                {
                  return ICF_FAILURE;
                }
        }

        if(ICF_TRACE_CRITERIA_SERVICE_CALL ==
                  (trace_criteria & ICF_TRACE_CRITERIA_SERVICE_CALL))
        {
            if(ICF_TRACE_IS_SERVICE_CALL_PRESENT ==
                   (ICF_TRACE_IS_SERVICE_CALL_PRESENT & p_criteria_data->bitmask))

            {
                    if(ICF_TRUE == p_criteria_data->is_service_call)
                    {
                      match_found = ICF_TRUE;
                    }

                    if(ICF_FALSE == match_found)
                    {
                       return ICF_FAILURE;
                    }
                    else
                    {
                       match_found = ICF_FALSE;
                    }

             }
             else
             {
                return ICF_FAILURE;
             }
        }

        if(ICF_TRACE_CRITERIA_LOCAL_SIP_IP_ADDRESS ==
                  (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_SIP_IP_ADDRESS))
        {
                if(ICF_TRACE_LOCAL_SIP_IP_PRESENT ==
                   (ICF_TRACE_LOCAL_SIP_IP_PRESENT & p_criteria_data->bitmask))
                {
                    for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                    {
                       if(ICF_SUCCESS == icf_port_compare_ip_addr(
                               p_criteria_data->p_local_sip_ip_address->addr,
                              p_trace_db->local_sip_ip_address[count]))
                       {
                          match_found = ICF_TRUE;
                          break;
                       }
                     }

                    if(ICF_FALSE == match_found)
                    {
                        return ICF_FAILURE;
                    }
                    else
                    {
                      match_found = ICF_FALSE;
                     }

                }
                else
                {
                      return ICF_FAILURE;
                }
        }

        if(ICF_TRACE_CRITERIA_LOCAL_SIP_PORT ==
                   (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_SIP_PORT))
        {
                if(ICF_TRACE_LOCAL_SIP_PORT_PRESENT ==
                   (ICF_TRACE_LOCAL_SIP_PORT_PRESENT & p_criteria_data->bitmask))
                 {
                  for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                   {
                     if(p_criteria_data->p_local_sip_ip_address->port_num
                           ==  p_trace_db->local_sip_port[count])
                        {
                           match_found = ICF_TRUE;
                           break;
                        }
                   }

                   if(ICF_FALSE == match_found)
                   {
                      return ICF_FAILURE;
                   }
                   else
                   {
                      match_found = ICF_FALSE;
                   }

                }
                else
                {
                     return ICF_FAILURE;
                }
         }

         if(ICF_TRACE_CRITERIA_OFFERED_CODEC ==
                 (trace_criteria & ICF_TRACE_CRITERIA_OFFERED_CODEC))
         {
             if(ICF_TRACE_OFFERED_CODEC_PRESENT ==
                   (ICF_TRACE_OFFERED_CODEC_PRESENT & p_criteria_data->bitmask))
             {
                  icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;
                 while(ICF_NULL != p_stream_list)
                 {
                     icf_stream_data_st *p_stream_data =
                       (icf_stream_data_st *)(p_stream_list->p_data);

                    icf_list_st *p_offered_codec_list =
                       p_stream_data->p_offered_codec;

                     while(ICF_NULL != p_offered_codec_list)
                     {
                        icf_codec_attrib_st *p_offered_codec =
                          (icf_codec_attrib_st *)(p_offered_codec_list->p_data);

                        for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                        {
                           if(p_offered_codec->codec_num ==
                               p_trace_db->offered_codec[count].codec_num)
                           {
                              match_found = ICF_TRUE;
                              break;
                           }
                           else if(0 == icf_port_strcmp(
                              p_offered_codec->codec_name.str,
                              p_trace_db->offered_codec[count].codec_name.str))
                          {
                              match_found = ICF_TRUE;
                              break;
                          }

                       }

                       if(ICF_TRUE == match_found)
                       {
                         break;
                       }
                       else
                       {
                         p_offered_codec_list = p_offered_codec_list->p_next;
                      }
                 }

                 if(ICF_TRUE == match_found)
                 {
                   break;
                 }
                 else
                 {
                    p_stream_list = p_stream_list->p_next;
                 }

             }

             if(ICF_FALSE == match_found)
             {
                 return ICF_FAILURE;
             }
             else
             {
                match_found = ICF_FALSE;
             }

         }
         else
         {
             return ICF_FAILURE;
         }
      }

     if(ICF_TRACE_CRITERIA_NEGOTIATED_CODEC ==
                  (trace_criteria & ICF_TRACE_CRITERIA_NEGOTIATED_CODEC))
     {
          if(ICF_TRACE_NEGOTIATED_CODEC_PRESENT ==
              (ICF_TRACE_NEGOTIATED_CODEC_PRESENT & p_criteria_data->bitmask))
          {
            icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

            while(ICF_NULL != p_stream_list)
            {
                  icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


                  for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                  {
                      if(p_stream_data->p_negotiated_codec->codec_num ==
                         p_trace_db->negotiated_codec[count].codec_num)
                      {
                         match_found = ICF_TRUE;
                         break;
                      }
                      else if(0 == icf_port_strcmp(
                        p_stream_data->p_negotiated_codec->codec_name.str,
                        p_trace_db->negotiated_codec[count].codec_name.str))
                     {
                         match_found = ICF_TRUE;
                         break;
                     }

                 }

                 if(ICF_TRUE == match_found)
                 {
                    break;
                 }
                 else
                 {
                   p_stream_list = p_stream_list->p_next;
                 }

             }

             if(ICF_FALSE == match_found)
             {
                  return ICF_FAILURE;
             }
             else
             {
                match_found = ICF_FALSE;
             }
         }
         else
         {
             return ICF_FAILURE;
         }
     }

    if(ICF_TRACE_CRITERIA_LOCAL_RTP_IP_ADDRESS ==
            (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_RTP_IP_ADDRESS))
      {
         if(ICF_TRACE_LOCAL_RTP_IP_PRESENT ==
          (ICF_TRACE_LOCAL_RTP_IP_PRESENT & p_criteria_data->bitmask))
         {
            icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

            while(ICF_NULL != p_stream_list)
            {
             icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


              for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
              {
                if(ICF_SUCCESS == icf_port_compare_ip_addr(
                    p_stream_data->local_rtp_address.addr,
                     p_trace_db->local_rtp_ip_address[count]))
                 {
                      match_found = ICF_TRUE;
                      break;
                 }

               }

              if(ICF_TRUE == match_found)
              {
                break;
              }
              else
              {
                p_stream_list = p_stream_list->p_next;
              }

            }

           if(ICF_FALSE == match_found)
           {
               return ICF_FAILURE;
           }
           else
           {
                match_found = ICF_FALSE;
            }

        }
        else
        {
             return ICF_FAILURE;
        }
     }
     if(ICF_TRACE_CRITERIA_REMOTE_RTP_IP_ADDRESS ==
              (trace_criteria & ICF_TRACE_CRITERIA_REMOTE_RTP_IP_ADDRESS))
     {
          if(ICF_TRACE_REMOTE_RTP_IP_PRESENT ==
             (ICF_TRACE_REMOTE_RTP_IP_PRESENT & p_criteria_data->bitmask))

          {
            icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

            while(ICF_NULL != p_stream_list)
            {
             icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


              for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
              {
                if(ICF_SUCCESS == icf_port_compare_ip_addr(
                      p_stream_data->remote_rtp_address.addr,
                      p_trace_db->remote_rtp_ip_address[count]))
                 {
                      match_found = ICF_TRUE;
                      break;
                 }

               }

              if(ICF_TRUE == match_found)
              {
                break;
              }
              else
              {
                p_stream_list = p_stream_list->p_next;
              }

            }

            if(ICF_FALSE == match_found)
            {
                 return ICF_FAILURE;
            }
            else
            {
                match_found = ICF_FALSE;
            }

         }
         else
         {
             return ICF_FAILURE;
         }
    }

    if(ICF_TRACE_CRITERIA_LOCAL_RTP_PORT ==
                   (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_RTP_PORT))
    {
        if(ICF_TRACE_LOCAL_RTP_PORT_PRESENT ==
            (ICF_TRACE_LOCAL_RTP_PORT_PRESENT & p_criteria_data->bitmask))
        {
           icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

           while(ICF_NULL != p_stream_list)
           {
             icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


              for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
              {
                if(p_stream_data->local_rtp_address.port_num ==
                    p_trace_db->local_rtp_port[count])
                {
                     match_found = ICF_TRUE;
                     break;
                }
              }

              if(ICF_TRUE == match_found)
              {
                  break;
              }
              else
              {
                  p_stream_list = p_stream_list->p_next;
              }
            }

            if(ICF_FALSE == match_found)
            {
                 return ICF_FAILURE;
            }
            else
            {
                match_found = ICF_FALSE;
            }

        }
        else
        {
             return ICF_FAILURE;
        }
     }

     if(ICF_TRACE_CRITERIA_REMOTE_RTP_PORT ==
             (trace_criteria & ICF_TRACE_CRITERIA_REMOTE_RTP_PORT))
     {
         if(ICF_TRACE_REMOTE_RTP_PORT_PRESENT ==
             (ICF_TRACE_REMOTE_RTP_PORT_PRESENT & p_criteria_data->bitmask))

        {
           icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

           while(ICF_NULL != p_stream_list)
           {
              icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


              for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
              {
                if(p_stream_data->remote_rtp_address.port_num ==
                    p_trace_db->remote_rtp_port[count])
                                {
                                     match_found = ICF_TRUE;
                                     break;
                            }
              }

              if(ICF_TRUE == match_found)
              {
                break;
              }
              else
              {
                p_stream_list = p_stream_list->p_next;
              }
            }

            if(ICF_FALSE == match_found)
            {
                 return ICF_FAILURE;
            }
            else
            {
                match_found = ICF_FALSE;
            }

         }
         else
         {
             return ICF_FAILURE;
         }
    }
     break;

    case ICF_TRACE_RULE_OR:
        /* In OR condition, ret_val should be initialized to FAILURE, otherwise
	 * this func will always return SUCCESS even if none of the condition 
	 * matches
	 */
		ret_val = ICF_FAILURE; 
        if((ICF_TRACE_CRITERIA_CALLING_PARTY_ADDRESS ==
           (trace_criteria & ICF_TRACE_CRITERIA_CALLING_PARTY_ADDRESS)) &&
                  (ICF_TRACE_CALLING_PARTY_ADDR_PRESENT ==
                  (ICF_TRACE_CALLING_PARTY_ADDR_PRESENT & p_criteria_data->bitmask)))
                {
            for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
            {
                if(0 == icf_port_strcmp(
                    p_criteria_data->p_calling_party_address->addr_val.str,
                    p_trace_db->calling_party_address[count].addr_val.str))
                {
                    match_found = ICF_TRUE;
                    break;
                }
            }

            if(ICF_TRUE == match_found)
            {
                return ICF_SUCCESS;
            }
        }
           if((ICF_TRACE_CRITERIA_CALLED_PARTY_ADDRESS ==
                  (trace_criteria & ICF_TRACE_CRITERIA_CALLED_PARTY_ADDRESS)) &&
                  (ICF_TRACE_CALLED_PARTY_ADDR_PRESENT ==
                  (ICF_TRACE_CALLED_PARTY_ADDR_PRESENT & p_criteria_data->bitmask)))
               {
                    for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                   {
                         if(0 == icf_port_strcmp(
                            p_criteria_data->p_called_party_address->addr_val.str,
                            p_trace_db->called_party_address[count].addr_val.str))
                        {
                           match_found = ICF_TRUE;
                           break;
                        }
                   }
                   if(ICF_TRUE == match_found)
                   {
                       return ICF_SUCCESS;
                   }
               }

          if((ICF_TRACE_CRITERIA_LINE_ID ==
                   (trace_criteria & ICF_TRACE_CRITERIA_LINE_ID))  &&
                  (ICF_TRACE_LINE_ID_PRESENT ==
                  (ICF_TRACE_LINE_ID_PRESENT & p_criteria_data->bitmask)))
              {
                  for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
                  {
                      if(p_criteria_data->line_id == p_trace_db->line_id[count])
                        {
                           match_found = ICF_TRUE;
                           break;
                        }
                  }

                  if(ICF_TRUE == match_found)
                  {
                       return ICF_SUCCESS;
                  }
              }

          if((ICF_TRACE_CRITERIA_INCOMING_CALL ==
                 (trace_criteria & ICF_TRACE_CRITERIA_INCOMING_CALL))   &&
                  (ICF_TRACE_CALL_DIR_PRESENT ==
                  (ICF_TRACE_CALL_DIR_PRESENT & p_criteria_data->bitmask)))
              {
                 if(ICF_CALL_DIRECTION_INCOMING ==
                 (ICF_CALL_DIRECTION_INCOMING & p_criteria_data->call_direction))
                {
                  match_found = ICF_TRUE;
                }
                if(ICF_TRUE == match_found)
                {
                   return ICF_SUCCESS;
                }
              }

          if((ICF_TRACE_CRITERIA_OUTGOING_CALL ==
                (trace_criteria & ICF_TRACE_CRITERIA_OUTGOING_CALL))  &&
                  (ICF_TRACE_CALL_DIR_PRESENT ==
                  (ICF_TRACE_CALL_DIR_PRESENT & p_criteria_data->bitmask)))
              {
                  if(ICF_CALL_DIRECTION_OUTGOING ==
                    (ICF_CALL_DIRECTION_OUTGOING & p_criteria_data->call_direction))
                  {
                      match_found = ICF_TRUE;
                  }

                  if(ICF_TRUE == match_found)
                  {
                     return ICF_SUCCESS;
                  }
              }

          if((ICF_TRACE_CRITERIA_CONNECTED_CALL ==
                (trace_criteria & ICF_TRACE_CRITERIA_CONNECTED_CALL)) &&
                (ICF_TRACE_CALL_TYPE_PRESENT ==
                (ICF_TRACE_CALL_TYPE_PRESENT & p_criteria_data->bitmask)))
              {
                 if(ICF_TRACE_CONNECTED_CALL ==
                   (ICF_TRACE_CONNECTED_CALL & p_criteria_data->call_type))
                 {
                    match_found = ICF_TRUE;
                 }
                 if(ICF_TRUE == match_found)
                 {
                    return ICF_SUCCESS;
                 }
             }

         if((ICF_TRACE_CRITERIA_ALERTED_CALL ==
                (trace_criteria & ICF_TRACE_CRITERIA_ALERTED_CALL)) &&
                (ICF_TRACE_CALL_TYPE_PRESENT ==
                (ICF_TRACE_CALL_TYPE_PRESENT & p_criteria_data->bitmask)))
             {
                if(ICF_TRACE_ALERTED_CALL ==
                   (ICF_TRACE_ALERTED_CALL & p_criteria_data->call_type))
                {
                     match_found = ICF_TRUE;
                }
                if(ICF_TRUE == match_found)
                {
                  return ICF_SUCCESS;
                }
             }

         if((ICF_TRACE_CRITERIA_CALL_ON_UDP ==
               (trace_criteria & ICF_TRACE_CRITERIA_CALL_ON_UDP)) &&
               (ICF_TRACE_TRANSPORT_MODE_PRESENT ==
                (ICF_TRACE_TRANSPORT_MODE_PRESENT & p_criteria_data->bitmask)))
             {
                if(ICF_CALL_TRANSPORT_MODE_UDP ==
                (ICF_CALL_TRANSPORT_MODE_UDP & p_criteria_data->call_transport_mode))
               {
                  match_found = ICF_TRUE;
               }
               if(ICF_TRUE == match_found)
               {
                   return ICF_SUCCESS;
               }
            }

       if((ICF_TRACE_CRITERIA_CALL_ON_TCP ==
            (trace_criteria & ICF_TRACE_CRITERIA_CALL_ON_TCP)) &&
            (ICF_TRACE_TRANSPORT_MODE_PRESENT ==
             (ICF_TRACE_TRANSPORT_MODE_PRESENT & p_criteria_data->bitmask)))
           {
                if(ICF_CALL_TRANSPORT_MODE_TCP ==
                  (ICF_CALL_TRANSPORT_MODE_TCP & p_criteria_data->call_transport_mode))
                {
                   match_found = ICF_TRUE;
                }
                if(ICF_TRUE == match_found)
                {
                  return ICF_SUCCESS;
                }
           }

       if((ICF_TRACE_CRITERIA_CALL_ON_TLS ==
            (trace_criteria & ICF_TRACE_CRITERIA_CALL_ON_TLS)) &&
            (ICF_TRACE_TRANSPORT_MODE_PRESENT ==
             (ICF_TRACE_TRANSPORT_MODE_PRESENT & p_criteria_data->bitmask)))

           {
               if(ICF_CALL_TRANSPORT_MODE_TLS ==
                  (ICF_CALL_TRANSPORT_MODE_TLS & p_criteria_data->call_transport_mode))
               {
                  match_found = ICF_TRUE;
               }

               if(ICF_TRUE == match_found)
               {
                 return ICF_SUCCESS;
               }
           }

       if((ICF_TRACE_CRITERIA_SERVICE_CALL ==
             (trace_criteria & ICF_TRACE_CRITERIA_SERVICE_CALL)) &&
            (ICF_TRACE_IS_SERVICE_CALL_PRESENT ==
             (ICF_TRACE_IS_SERVICE_CALL_PRESENT & p_criteria_data->bitmask)))
          {
              if(ICF_TRUE == p_criteria_data->is_service_call)
              {
                  match_found = ICF_TRUE;
              }
              if(ICF_TRUE == match_found)
              {
                 return ICF_SUCCESS;
              }
        }

       if((ICF_TRACE_CRITERIA_LOCAL_SIP_IP_ADDRESS ==
            (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_SIP_IP_ADDRESS)) &&
             (ICF_TRACE_LOCAL_SIP_IP_PRESENT ==
             (ICF_TRACE_LOCAL_SIP_IP_PRESENT & p_criteria_data->bitmask)))
        {
            for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
            {
                if(ICF_SUCCESS == icf_port_compare_ip_addr(
                    p_criteria_data->p_local_sip_ip_address->addr,
                     p_trace_db->local_sip_ip_address[count]))
                {
                    match_found = ICF_TRUE;
                    break;
                }
            }
            if(ICF_TRUE == match_found)
            {
               return ICF_SUCCESS;
            }
        }

        if((ICF_TRACE_CRITERIA_LOCAL_SIP_PORT ==
            (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_SIP_PORT)) &&
            (ICF_TRACE_LOCAL_SIP_PORT_PRESENT ==
             (ICF_TRACE_LOCAL_SIP_PORT_PRESENT & p_criteria_data->bitmask)))
        {
            for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
            {
                if(p_criteria_data->p_local_sip_ip_address->port_num
                    ==  p_trace_db->local_sip_port[count])
                {
                    match_found = ICF_TRUE;
                    break;
                }
            }
            if(ICF_TRUE == match_found)
            {
               return ICF_SUCCESS;
            }
        }

       if((ICF_TRACE_CRITERIA_OFFERED_CODEC ==
            (trace_criteria & ICF_TRACE_CRITERIA_OFFERED_CODEC)) &&
        (ICF_TRACE_OFFERED_CODEC_PRESENT ==
             (ICF_TRACE_OFFERED_CODEC_PRESENT & p_criteria_data->bitmask)))

      {
       icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

       while(ICF_NULL != p_stream_list)
       {
         icf_stream_data_st *p_stream_data =
            (icf_stream_data_st *)(p_stream_list->p_data);

          icf_list_st *p_offered_codec_list =
            p_stream_data->p_offered_codec;

          while(ICF_NULL != p_offered_codec_list)
          {
                icf_codec_attrib_st *p_offered_codec =
                (icf_codec_attrib_st *)(p_offered_codec_list->p_data);

              for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
              {
                if(p_offered_codec->codec_num ==
                p_trace_db->offered_codec[count].codec_num)
              {
                match_found = ICF_TRUE;
                break;
              }
              else if(0 == icf_port_strcmp(
                p_offered_codec->codec_name.str,
                p_trace_db->offered_codec[count].codec_name.str))
             {
                match_found = ICF_TRUE;
                break;
             }

             }

             if(ICF_TRUE == match_found)
             {
                 break;
             }
             else
             {
                p_offered_codec_list = p_offered_codec_list->p_next;
             }
          }

          if(ICF_TRUE == match_found)
           {
             break;
          }
          else
          {
            p_stream_list = p_stream_list->p_next;
          }

        }
        if(ICF_TRUE == match_found)
        {
            return ICF_SUCCESS;
        }
     }

       if((ICF_TRACE_CRITERIA_NEGOTIATED_CODEC ==
            (trace_criteria & ICF_TRACE_CRITERIA_NEGOTIATED_CODEC)) &&
             (ICF_TRACE_NEGOTIATED_CODEC_PRESENT ==
             (ICF_TRACE_NEGOTIATED_CODEC_PRESENT & p_criteria_data->bitmask)))

          {
           icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

           while(ICF_NULL != p_stream_list)
           {
             icf_stream_data_st *p_stream_data =
            (icf_stream_data_st *)(p_stream_list->p_data);
           for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
           {
            if(p_stream_data->p_negotiated_codec->codec_num ==
                p_trace_db->negotiated_codec[count].codec_num)
            {
                match_found = ICF_TRUE;
                break;
            }
            else if(0 == icf_port_strcmp(
                p_stream_data->p_negotiated_codec->codec_name.str,
                p_trace_db->negotiated_codec[count].codec_name.str))
            {
                match_found = ICF_TRUE;
                break;
            }

          }

          if(ICF_TRUE == match_found)
           {
            break;
          }
          else
          {
            p_stream_list = p_stream_list->p_next;
          }

        }
        if(ICF_TRUE == match_found)
        {
            return ICF_SUCCESS;
        }
          }

      if((ICF_TRACE_CRITERIA_LOCAL_RTP_IP_ADDRESS ==
            (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_RTP_IP_ADDRESS)) &&
            (ICF_TRACE_LOCAL_RTP_IP_PRESENT ==
             (ICF_TRACE_LOCAL_RTP_IP_PRESENT & p_criteria_data->bitmask)))

          {
        icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

        while(ICF_NULL != p_stream_list)
        {
             icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


              for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
              {
                if(ICF_SUCCESS == icf_port_compare_ip_addr(
                    p_stream_data->local_rtp_address.addr,
                     p_trace_db->local_rtp_ip_address[count]))
                  {
                       match_found = ICF_TRUE;
                        break;
                   }

              }
              if(ICF_TRUE == match_found)
              {
                break;
              }
              else
              {
                p_stream_list = p_stream_list->p_next;
              }

        }
        if(ICF_TRUE == match_found)
        {
            return ICF_SUCCESS;
        }
   }

   if((ICF_TRACE_CRITERIA_REMOTE_RTP_IP_ADDRESS ==
             (trace_criteria & ICF_TRACE_CRITERIA_REMOTE_RTP_IP_ADDRESS)) &&
             (ICF_TRACE_REMOTE_RTP_IP_PRESENT ==
             (ICF_TRACE_REMOTE_RTP_IP_PRESENT & p_criteria_data->bitmask)))
      {
        icf_list_st    *p_stream_list = p_criteria_data->p_stream_data;

        while(ICF_NULL != p_stream_list)
        {
             icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


             for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
             {
                 if(ICF_SUCCESS == icf_port_compare_ip_addr(
                    p_stream_data->remote_rtp_address.addr,
                     p_trace_db->remote_rtp_ip_address[count]))
                         {
                                 match_found = ICF_TRUE;
                                 break;
                         }

             }

             if(ICF_TRUE == match_found)
              {
                break;
             }
             else
             {
                p_stream_list = p_stream_list->p_next;
             }

        }
        if(ICF_TRUE == match_found)
        {
            return ICF_SUCCESS;
        }
     }

      if((ICF_TRACE_CRITERIA_LOCAL_RTP_PORT ==
            (trace_criteria & ICF_TRACE_CRITERIA_LOCAL_RTP_PORT)) &&
             (ICF_TRACE_LOCAL_RTP_PORT_PRESENT ==
             (ICF_TRACE_LOCAL_RTP_IP_PRESENT & p_criteria_data->bitmask)))

      {
        icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

        while(ICF_NULL != p_stream_list)
        {
              icf_stream_data_st *p_stream_data =
            (icf_stream_data_st *)(p_stream_list->p_data);


           for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
           {
              if(p_stream_data->local_rtp_address.port_num ==
                  p_trace_db->local_rtp_port[count])
                 {
                            match_found = ICF_TRUE;
                            break;
                 }
           }

           if(ICF_TRUE == match_found)
            {
            break;
           }
           else
           {
            p_stream_list = p_stream_list->p_next;
           }
        }
        if(ICF_TRUE == match_found)
        {
            return ICF_SUCCESS;
        }
     }

    if((ICF_TRACE_CRITERIA_REMOTE_RTP_PORT ==
            (trace_criteria & ICF_TRACE_CRITERIA_REMOTE_RTP_PORT)) &&
             (ICF_TRACE_REMOTE_RTP_PORT_PRESENT ==
             (ICF_TRACE_REMOTE_RTP_IP_PRESENT & p_criteria_data->bitmask)))
    {
        icf_list_st        *p_stream_list = p_criteria_data->p_stream_data;

        while(ICF_NULL != p_stream_list)
        {
             icf_stream_data_st *p_stream_data =
                (icf_stream_data_st *)(p_stream_list->p_data);


                  for(count = 0; count < ICF_TRACE_MAX_DB_VAL; count++)
             {
                if(p_stream_data->remote_rtp_address.port_num ==
                    p_trace_db->remote_rtp_port[count])
                        {
                                 match_found = ICF_TRUE;
                                  break;
                         }
             }

             if(ICF_TRUE == match_found)
              {
                break;
             }
             else
             {
                p_stream_list = p_stream_list->p_next;
             }
         }
        if(ICF_TRUE == match_found)
        {
            return ICF_SUCCESS;
        }
    }

        break;

    default:
        ret_val = ICF_FAILURE;
        break;
    }
    }
    return ret_val;
}

#endif

/***************************************************************************
 ** FUNCTION:     icf_port_isnumeric
 ** DESCRIPTION:  This function checks if the input string is a number or not
 **
 ***************************************************************************/
icf_boolean_t icf_port_isnumeric(
    IN  icf_uint8_t     *p_str)
{
    icf_boolean_t   is_numeric = ICF_TRUE;
    icf_uint16_t    len = ICF_NULL, ctr = ICF_NULL;

    if (ICF_NULL == p_str)
    {
        is_numeric = ICF_FALSE;
    }
    else
    {
        len = (icf_uint16_t) icf_port_strlen(p_str);

        for (ctr = 0; ctr < len; ctr++)
        {
            if ((p_str[ctr] < '0') || (p_str[ctr] > '9'))
            {
                is_numeric = ICF_FALSE;
                break;
            }
        }
    }

    return is_numeric;
}


/******************************************************************************
 * icf_port_filter_printf
 *
 * It is wrapper over printf to check that only printlable characters are
 * goint to stdout when %s option is getting used
 *
 * ***************************************************************************/

icf_void_t icf_port_filter_printf(icf_uint8_t *p_buff, ...)
{
#define STATE_START 0
#define STATE_PERCENT_RECEIVED 1
#define STATE_NEXT_CHAR_AWAIT 2
#define STATE_LONG_RECV 3
    va_list  list;
    icf_uint8_t *p_char, *p_temp;
    icf_uint32_t state = STATE_START;
    icf_int32_t int_val, i = 0;
    icf_uint32_t uint_val;
    icf_uint8_t pad_char[10];
    icf_uint8_t temp_str[10];
    va_start (list, p_buff);
    pad_char[0] = '\0';
    for (p_char = p_buff; *p_char != '\0'; p_char++)
    {
        if (*p_char == '%')
        {
            if (state == STATE_START)
                state = STATE_PERCENT_RECEIVED;
            else
            {
                printf ("%%");
                state = STATE_START;
            }
        }
        else if (state != STATE_START)
        {
            if  ('s' == *p_char)
            {
                p_temp = (icf_uint8_t *)va_arg(list, icf_uint8_t *);
                for (i = 0; p_temp[i] != '\0'; i++)
                {
                    if ((p_temp[i] > ICF_MAX_NON_PRINTABLE_CHARACTER )
                            || (p_temp[i] =='\n')
                            || (p_temp[i] == '\r')
                            || (p_temp[i] == '\t')
                            || (p_temp[i] == '\b')
                            || (p_temp[i] == '\v'))
                        putchar(p_temp[i]);
                }
                state = STATE_START;
            }
            else if (('d' == *p_char) ||
                    ('i' == *p_char))
            {
                if (state == STATE_LONG_RECV)
                    int_val = (icf_int32_t) va_arg (list, icf_int32_t);
                else
                    int_val = (icf_int32_t)va_arg (list,  icf_int32_t);
                strcpy((icf_int8_t *)temp_str, (const icf_int8_t *)"%");
                strcat((icf_int8_t *) temp_str,(const icf_int8_t *) pad_char);
                if (state == STATE_LONG_RECV)
                    strcat((icf_int8_t *)temp_str,(const icf_int8_t *)"l");
                strcat((icf_int8_t *)temp_str,(const icf_int8_t *)"d");
                printf((const icf_int8_t *)temp_str,int_val);
                pad_char[0] = '\0';
                state = STATE_START;
            }
            else if ('u' == *p_char)
            {
                if (state == STATE_LONG_RECV)
                    uint_val = (icf_uint32_t) va_arg (list, icf_uint32_t);
                else
                    uint_val = (icf_uint32_t)va_arg (list, icf_uint32_t);
                strcpy((icf_int8_t *)temp_str,(const icf_int8_t *) "%");
                strcat((icf_int8_t *) temp_str, (const icf_int8_t *)pad_char);
                if (state == STATE_LONG_RECV)
                    strcat((icf_int8_t *)temp_str,(const icf_int8_t *)"l");
                strcat((icf_int8_t *)temp_str,(const icf_int8_t *)"u");
                printf((const icf_int8_t *) temp_str, uint_val);
                pad_char[0] = '\0';
                state = STATE_START;
            }
            else if ('c' == *p_char)
            {
                uint_val = va_arg(list, icf_uint32_t);
                strcpy((icf_int8_t *)temp_str, (const icf_int8_t *)"%");
                strcat((icf_int8_t *)temp_str, (const icf_int8_t *)pad_char);
                strcat((icf_int8_t *)temp_str, "c");
                printf((const icf_int8_t *)temp_str,(const icf_int8_t *)uint_val);
                state = STATE_START;
            }
            else if ('o' == *p_char)
            {
                int_val = va_arg(list, icf_int32_t);
                printf("%o",*p_char);
                state = STATE_START;
            }
            else if ((*p_char >= '0')
                    &&(*p_char <='9'))
            {
                pad_char[i++] = *p_char;
                pad_char[i] = '\0';
                state = STATE_NEXT_CHAR_AWAIT;
            }
            else if ('l' == *p_char)
            {
                state = STATE_LONG_RECV;
            }
#if 0
            else if ('f' == *p_char)
            {
                ft_val = va_arg(list, double);
                strcpy(temp_str, "%");
                strcat(temp_str, pad_char);
                strcat(temp_str, "f");
                printf(temp_str, ft_val);
                state = STATE_START;
            }
#endif
            else
            {
                state = STATE_START;
            }


        }
        else
            putchar(*p_char);
    }
    va_end(list);
}

