/*****************************************************************************
*
*    FUNCTION    : Source file for Generic Library - Socket Manager
*                  backward compatibility of Rel-1.0 with Rel-0.2 
*                  implementation.
******************************************************************************
*
*    FILENAME    : gl_skm_bckwrd_compatbl_api.c
*
*    DESCRIPTION : Give defination of functions which requires backward
*                  compatibilty of Rel-1.0 with Rel-0.2.
*
*
* DATE          NAME           REFERENCE          REASON
*----------- --------------  ---------------    ----------------------
*
* 22/04/2002 Ravi Jindal        -               Intial Draft.
* 
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/


/*
 * As in Rel-1.0 only the function names are changed.
 * So #define the old name with new name.
 */

#define gl_sm_bind_inet_socket          gl_sm_bind_socket

#define gl_sm_connect_inet_socket       gl_sm_connect_socket

#define gl_sm_host_to_nw_long           gl_sm_htonl

#define gl_sm_host_to_nw_short          gl_sm_htons        

#define gl_sm_nw_to_host_long           gl_sm_ntohl

#define gl_sm_nw_to_host_short          gl_sm_ntohs


