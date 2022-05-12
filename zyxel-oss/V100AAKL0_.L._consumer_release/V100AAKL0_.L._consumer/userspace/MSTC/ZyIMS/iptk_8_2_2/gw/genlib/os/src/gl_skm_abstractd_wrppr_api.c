/*****************************************************************************
* 
* FUNCTION    : Source file for Generic library - Socket manager 
*               implementation.
*            
******************************************************************************
*
* FILENAME    : gl_skm_abstractd_wrppr_api.c
*
* DESCRIPTION : This file implements socket management functionality of 
*               generic libraries. 
*
*   DATE            NAME        REFERENCE           REASON
*-----------    ------------   -----------      ------------------------------
* April'01      Rajesh Jain     New file        Generic Libraries
*
* 18/08/2001    Prashant Jain   SPR 2276        Compiling with CC on Solaris.
*
* 04/09/2001    Prashant Jain   SPR-2560        Adding OSE Operating System 
*                                               Support.
* 04/10/2001    Ravi Jindal     SPR-2751        Remove warnings with cc,
*                                               gcc,CC on Solaris. 
* 18/10/2001    Ravi Jindal     SPR-2788        Packaging of Genlib-Code. 
* 19/10/2001    Ravi Jindal     SPR-2802        Adding Pre-ANSI-C features.
* 17/11/2001    Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                               cosmmetics of file.
*
* 04/04/2002    Ravi Jindal     SPR-3959        Enhancements for Rel-1.0.
*       
* 11/11/2002    Ravi Jindal     SPR-5123        Provide Socket APIs for Solaris.
* 
* 29/11/2002    Sudhir Gupta    SPR-5123        Provide Socket APIs for WinNT,
*                                               Linux.
*
* 23/12/2003    Sudhanshu Garg  SPR-6265        Support for SCTP in socket
*                                               manager
*
* 24/02/2004    Sudhanshu Garg  SPR-6614        If socket is not set already
*                                               then do not change databases
*                                               in gl_sm_slm_fd_clear
*
* 27/02/2004    Sudhanshu Garg  SPR-6265        Insure related problems removal
* 12/07/2004    Navneet Gupta   SPR 6946        QNX port
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_skm_interfaces.h"


gl_sm_tcp_udp_lnk_id_info_t         *g_a_p_tcp_udp_lnk_id_info = GL_NULL;

gl_sm_sctp_lnk_id_info_t            *g_a_p_sctp_lnk_id_info = GL_NULL;

gl_sm_tcp_lnk_map_info_t            *g_a_p_tcp_lnk_map_info = GL_NULL;

gl_sm_sctp_lnk_map_info_t           *g_a_p_sctp_lnk_map_info = GL_NULL;

gl_sm_udp_lnk_map_info_t            g_udp_lnk_map_info;

gl_sm_sctp_listen_lnk_map_info_t    g_sctp_listen_lnk_map_info;

gw_U32bit                           g_gl_sctp_fd_setsize;

gw_U32bit       gl_max_bit_value[4] = {0xff, 0xff00, 0xff0000, 0xff000000};

gl_sm_bit_val_t             g_a_bit_values[256]= { 
    {0,    {0,0,0,0,0,0,0,0}},        /* 0,0,0,0,0,0,0,0 */
    {1,    {1,0,0,0,0,0,0,0}},        /* 0,0,0,0,0,0,0,1 */
    {1,    {2,0,0,0,0,0,0,0}},        /* 0,0,0,0,0,0,1,0 */
    {2,    {1,2,0,0,0,0,0,0}},        /* 0,0,0,0,0,0,1,1 */
    {1,    {3,0,0,0,0,0,0,0}},        /* 0,0,0,0,0,1,0,0 */
    {2,    {1,3,0,0,0,0,0,0}},        /* 0,0,0,0,0,1,0,1 */
    {2,    {2,3,0,0,0,0,0,0}},        /* 0,0,0,0,0,1,1,0 */
    {3,    {1,2,3,0,0,0,0,0}},        /* 0,0,0,0,0,1,1,1 */
    {1,    {4,0,0,0,0,0,0,0}},        /* 0,0,0,0,1,0,0,0 */
    {2,    {1,4,0,0,0,0,0,0}},        /* 0,0,0,0,1,0,0,1 */
    {2,    {2,4,0,0,0,0,0,0}},        /* 0,0,0,0,1,0,1,0 */
    {3,    {1,2,4,0,0,0,0,0}},        /* 0,0,0,0,1,0,1,1 */
    {2,    {3,4,0,0,0,0,0,0}},        /* 0,0,0,0,1,1,0,0 */
    {3,    {1,3,4,0,0,0,0,0}},        /* 0,0,0,0,1,1,0,1 */
    {3,    {2,3,4,0,0,0,0,0}},        /* 0,0,0,0,1,1,1,0 */
    {4,    {1,2,3,4,0,0,0,0}},        /* 0,0,0,0,1,1,1,1 */

    {1,    {5,0,0,0,0,0,0,0}},        /* 0,0,0,1,0,0,0,0 */
    {2,    {1,5,0,0,0,0,0,0}},        /* 0,0,0,1,0,0,0,1 */
    {2,    {2,5,0,0,0,0,0,0}},        /* 0,0,0,1,0,0,1,0 */
    {3,    {1,2,5,0,0,0,0,0}},        /* 0,0,0,1,0,0,1,1 */
    {2,    {3,5,0,0,0,0,0,0}},        /* 0,0,0,1,0,1,0,0 */
    {3,    {1,3,5,0,0,0,0,0}},        /* 0,0,0,1,0,1,0,1 */
    {3,    {2,3,5,0,0,0,0,0}},        /* 0,0,0,1,0,1,1,0 */
    {4,    {1,2,3,5,0,0,0,0}},        /* 0,0,0,1,0,1,1,1 */
    {2,    {4,5,0,0,0,0,0,0}},        /* 0,0,0,1,1,0,0,0 */
    {3,    {1,4,5,0,0,0,0,0}},        /* 0,0,0,1,1,0,0,1 */
    {3,    {2,4,5,0,0,0,0,0}},        /* 0,0,0,1,1,0,1,0 */
    {4,    {1,2,4,5,0,0,0,0}},        /* 0,0,0,1,1,0,1,1 */
    {3,    {3,4,5,0,0,0,0,0}},        /* 0,0,0,1,1,1,0,0 */
    {4,    {1,3,4,5,0,0,0,0}},        /* 0,0,0,1,1,1,0,1 */
    {4,    {2,3,4,5,0,0,0,0}},        /* 0,0,0,1,1,1,1,0 */
    {5,    {1,2,3,4,5,0,0,0}},        /* 0,0,0,1,1,1,1,1 */
            
    {1,    {6,0,0,0,0,0,0,0}},        /* 0,0,1,0,0,0,0,0 */
    {2,    {1,6,0,0,0,0,0,0}},        /* 0,0,1,0,0,0,0,1 */
    {2,    {2,6,0,0,0,0,0,0}},        /* 0,0,1,0,0,0,1,0 */
    {3,    {1,2,6,0,0,0,0,0}},        /* 0,0,1,0,0,0,1,1 */
    {2,    {3,6,0,0,0,0,0,0}},        /* 0,0,1,0,0,1,0,0 */
    {3,    {1,3,6,0,0,0,0,0}},        /* 0,0,1,0,0,1,0,1 */
    {3,    {2,3,6,0,0,0,0,0}},        /* 0,0,1,0,0,1,1,0 */
    {4,    {1,2,3,6,0,0,0,0}},        /* 0,0,1,0,0,1,1,1 */
    {2,    {4,6,0,0,0,0,0,0}},        /* 0,0,1,0,1,0,0,0 */
    {3,    {1,4,6,0,0,0,0,0}},        /* 0,0,1,0,1,0,0,1 */
    {3,    {2,4,6,0,0,0,0,0}},        /* 0,0,1,0,1,0,1,0 */
    {4,    {1,2,4,6,0,0,0,0}},        /* 0,0,1,0,1,0,1,1 */
    {3,    {3,4,6,0,0,0,0,0}},        /* 0,0,1,0,1,1,0,0 */
    {4,    {1,3,4,6,0,0,0,0}},        /* 0,0,1,0,1,1,0,1 */
    {4,    {2,3,4,6,0,0,0,0}},        /* 0,0,1,0,1,1,1,0 */
    {5,    {1,2,3,4,6,0,0,0}},        /* 0,0,1,0,1,1,1,1 */
      
    {2,    {5,6,0,0,0,0,0,0}},        /* 0,0,1,1,0,0,0,0 */
    {3,    {1,5,6,0,0,0,0,0}},        /* 0,0,1,1,0,0,0,1 */
    {3,    {2,5,6,0,0,0,0,0}},        /* 0,0,1,1,0,0,1,0 */
    {4,    {1,2,5,6,0,0,0,0}},        /* 0,0,1,1,0,0,1,1 */
    {3,    {3,5,6,0,0,0,0,0}},        /* 0,0,1,1,0,1,0,0 */
    {4,    {1,3,5,6,0,0,0,0}},        /* 0,0,1,1,0,1,0,1 */
    {4,    {2,3,5,6,0,0,0,0}},        /* 0,0,1,1,0,1,1,0 */
    {5,    {1,2,3,5,6,0,0,0}},        /* 0,0,1,1,0,1,1,1 */
    {3,    {4,5,6,0,0,0,0,0}},        /* 0,0,1,1,1,0,0,0 */
    {4,    {1,4,5,6,0,0,0,0}},        /* 0,0,1,1,1,0,0,1 */
    {4,    {2,4,5,6,0,0,0,0}},        /* 0,0,1,1,1,0,1,0 */
    {5,    {1,2,4,5,6,0,0,0}},        /* 0,0,1,1,1,0,1,1 */
    {4,    {3,4,5,6,0,0,0,0}},        /* 0,0,1,1,1,1,0,0 */
    {5,    {1,3,4,5,6,0,0,0}},        /* 0,0,1,1,1,1,0,1 */
    {5,    {2,3,4,5,6,0,0,0}},        /* 0,0,1,1,1,1,1,0 */
    {6,    {1,2,3,4,5,6,0,0}},        /* 0,0,1,1,1,1,1,1 */
     

      
    {1,    {7,0,0,0,0,0,0,0}},        /* 0,1,0,0,0,0,0,0 */
    {2,    {1,7,0,0,0,0,0,0}},        /* 0,1,0,0,0,0,0,1 */
    {2,    {2,7,0,0,0,0,0,0}},        /* 0,1,0,0,0,0,1,0 */
    {3,    {1,2,7,0,0,0,0,0}},        /* 0,1,0,0,0,0,1,1 */
    {2,    {3,7,0,0,0,0,0,0}},        /* 0,1,0,0,0,1,0,0 */
    {3,    {1,3,7,0,0,0,0,0}},        /* 0,1,0,0,0,1,0,1 */
    {3,    {2,3,7,0,0,0,0,0}},        /* 0,1,0,0,0,1,1,0 */
    {4,    {1,2,3,7,0,0,0,0}},        /* 0,1,0,0,0,1,1,1 */
    {2,    {4,7,0,0,0,0,0,0}},        /* 0,1,0,0,1,0,0,0 */
    {3,    {1,4,7,0,0,0,0,0}},        /* 0,1,0,0,1,0,0,1 */
    {3,    {2,4,7,0,0,0,0,0}},        /* 0,1,0,0,1,0,1,0 */
    {4,    {1,2,4,7,0,0,0,0}},        /* 0,1,0,0,1,0,1,1 */
    {3,    {3,4,7,0,0,0,0,0}},        /* 0,1,0,0,1,1,0,0 */
    {4,    {1,3,4,7,0,0,0,0}},        /* 0,1,0,0,1,1,0,1 */
    {4,    {2,3,4,7,0,0,0,0}},        /* 0,1,0,0,1,1,1,0 */
    {5,    {1,2,3,4,7,0,0,0}},        /* 0,1,0,0,1,1,1,1 */
      
    {2,    {5,7,0,0,0,0,0,0}},        /* 0,1,0,1,0,0,0,0 */
    {3,    {1,5,7,0,0,0,0,0}},        /* 0,1,0,1,0,0,0,1 */
    {3,    {2,5,7,0,0,0,0,0}},        /* 0,1,0,1,0,0,1,0 */
    {4,    {1,2,5,7,0,0,0,0}},        /* 0,1,0,1,0,0,1,1 */
    {3,    {3,5,7,0,0,0,0,0}},        /* 0,1,0,1,0,1,0,0 */
    {4,    {1,3,5,7,0,0,0,0}},        /* 0,1,0,1,0,1,0,1 */
    {4,    {2,3,5,7,0,0,0,0}},        /* 0,1,0,1,0,1,1,0 */
    {5,    {1,2,3,5,7,0,0,0}},        /* 0,1,0,1,0,1,1,1 */
    {3,    {4,5,7,0,0,0,0,0}},        /* 0,1,0,1,1,0,0,0 */
    {4,    {1,4,5,7,0,0,0,0}},        /* 0,1,0,1,1,0,0,1 */
    {4,    {2,4,5,7,0,0,0,0}},        /* 0,1,0,1,1,0,1,0 */
    {5,    {1,2,4,5,7,0,0,0}},        /* 0,1,0,1,1,0,1,1 */
    {4,    {3,4,5,7,0,0,0,0}},        /* 0,1,0,1,1,1,0,0 */
    {5,    {1,3,4,5,7,0,0,0}},        /* 0,1,0,1,1,1,0,1 */
    {5,    {2,3,4,5,7,0,0,0}},        /* 0,1,0,1,1,1,1,0 */
    {6,    {1,2,3,4,5,7,0,0}},        /* 0,1,0,1,1,1,1,1 */
      
    {2,    {6,7,0,0,0,0,0,0}},        /* 0,1,1,0,0,0,0,0 */
    {3,    {1,6,7,0,0,0,0,0}},        /* 0,1,1,0,0,0,0,1 */
    {3,    {2,6,7,0,0,0,0,0}},        /* 0,1,1,0,0,0,1,0 */
    {4,    {1,2,6,7,0,0,0,0}},        /* 0,1,1,0,0,0,1,1 */
    {3,    {3,6,7,0,0,0,0,0}},        /* 0,1,1,0,0,1,0,0 */
    {4,    {1,3,6,7,0,0,0,0}},        /* 0,1,1,0,0,1,0,1 */
    {4,    {2,3,6,7,0,0,0,0}},        /* 0,1,1,0,0,1,1,0 */
    {5,    {1,2,3,6,7,0,0,0}},        /* 0,1,1,0,0,1,1,1 */
    {3,    {4,6,7,0,0,0,0,0}},        /* 0,1,1,0,1,0,0,0 */
    {4,    {1,4,6,7,0,0,0,0}},        /* 0,1,1,0,1,0,0,1 */
    {4,    {2,4,6,7,0,0,0,0}},        /* 0,1,1,0,1,0,1,0 */
    {5,    {1,2,4,6,7,0,0,0}},        /* 0,1,1,0,1,0,1,1 */
    {4,    {3,4,6,7,0,0,0,0}},        /* 0,1,1,0,1,1,0,0 */
    {5,    {1,3,4,6,7,0,0,0}},        /* 0,1,1,0,1,1,0,1 */
    {5,    {2,3,4,6,7,0,0,0}},        /* 0,1,1,0,1,1,1,0 */
    {6,    {1,2,3,4,6,7,0,0}},        /* 0,1,1,0,1,1,1,1 */
      
    {3,    {5,6,7,0,0,0,0,0}},        /* 0,1,1,1,0,0,0,0 */
    {4,    {1,5,6,7,0,0,0,0}},        /* 0,1,1,1,0,0,0,1 */
    {4,    {2,5,6,7,0,0,0,0}},        /* 0,1,1,1,0,0,1,0 */
    {5,    {1,2,5,6,7,0,0,0}},        /* 0,1,1,1,0,0,1,1 */
    {4,    {3,5,6,7,0,0,0,0}},        /* 0,1,1,1,0,1,0,0 */
    {5,    {1,3,5,6,7,0,0,0}},        /* 0,1,1,1,0,1,0,1 */
    {5,    {2,3,5,6,7,0,0,0}},        /* 0,1,1,1,0,1,1,0 */
    {6,    {1,2,3,5,6,7,0,0}},        /* 0,1,1,1,0,1,1,1 */
    {4,    {4,5,6,7,0,0,0,0}},        /* 0,1,1,1,1,0,0,0 */
    {5,    {1,4,5,6,7,0,0,0}},        /* 0,1,1,1,1,0,0,1 */
    {5,    {2,4,5,6,7,0,0,0}},        /* 0,1,1,1,1,0,1,0 */
    {6,    {1,2,4,5,6,7,0,0}},        /* 0,1,1,1,1,0,1,1 */
    {5,    {3,4,5,6,7,0,0,0}},        /* 0,1,1,1,1,1,0,0 */
    {6,    {1,3,4,5,6,7,0,0}},        /* 0,1,1,1,1,1,0,1 */
    {6,    {2,3,4,5,6,7,0,0}},        /* 0,1,1,1,1,1,1,0 */
    {7,    {1,2,3,4,5,6,7,0}},        /* 0,1,1,1,1,1,1,1 */
      

      

    {1,    {8,0,0,0,0,0,0,0}},        /* 1,0,0,0,0,0,0,0 */
    {2,    {1,8,0,0,0,0,0,0}},        /* 1,0,0,0,0,0,0,1 */
    {2,    {2,8,0,0,0,0,0,0}},        /* 1,0,0,0,0,0,1,0 */
    {3,    {1,2,8,0,0,0,0,0}},        /* 1,0,0,0,0,0,1,1 */
    {2,    {3,8,0,0,0,0,0,0}},        /* 1,0,0,0,0,1,0,0 */
    {3,    {1,3,8,0,0,0,0,0}},        /* 1,0,0,0,0,1,0,1 */
    {3,    {2,3,8,0,0,0,0,0}},        /* 1,0,0,0,0,1,1,0 */
    {4,    {1,2,3,8,0,0,0,0}},        /* 1,0,0,0,0,1,1,1 */
    {2,    {4,8,0,0,0,0,0,0}},        /* 1,0,0,0,1,0,0,0 */
    {3,    {1,4,8,0,0,0,0,0}},        /* 1,0,0,0,1,0,0,1 */
    {3,    {2,4,8,0,0,0,0,0}},        /* 1,0,0,0,1,0,1,0 */
    {4,    {1,2,4,8,0,0,0,0}},        /* 1,0,0,0,1,0,1,1 */
    {3,    {3,4,8,0,0,0,0,0}},        /* 1,0,0,0,1,1,0,0 */
    {4,    {1,3,4,8,0,0,0,0}},        /* 1,0,0,0,1,1,0,1 */
    {4,    {2,3,4,8,0,0,0,0}},        /* 1,0,0,0,1,1,1,0 */
    {5,    {1,2,3,4,8,0,0,0}},        /* 1,0,0,0,1,1,1,1 */
      
    {2,    {5,8,0,0,0,0,0,0}},        /* 1,0,0,1,0,0,0,0 */
    {3,    {1,5,8,0,0,0,0,0}},        /* 1,0,0,1,0,0,0,1 */
    {3,    {2,5,8,0,0,0,0,0}},        /* 1,0,0,1,0,0,1,0 */
    {4,    {1,2,5,8,0,0,0,0}},        /* 1,0,0,1,0,0,1,1 */
    {3,    {3,5,8,0,0,0,0,0}},        /* 1,0,0,1,0,1,0,0 */
    {4,    {1,3,5,8,0,0,0,0}},        /* 1,0,0,1,0,1,0,1 */
    {4,    {2,3,5,8,0,0,0,0}},        /* 1,0,0,1,0,1,1,0 */
    {5,    {1,2,3,5,8,0,0,0}},        /* 1,0,0,1,0,1,1,1 */
    {3,    {4,5,8,0,0,0,0,0}},        /* 1,0,0,1,1,0,0,0 */
    {4,    {1,4,5,8,0,0,0,0}},        /* 1,0,0,1,1,0,0,1 */
    {4,    {2,4,5,8,0,0,0,0}},        /* 1,0,0,1,1,0,1,0 */
    {5,    {1,2,4,5,8,0,0,0}},        /* 1,0,0,1,1,0,1,1 */
    {4,    {3,4,5,8,0,0,0,0}},        /* 1,0,0,1,1,1,0,0 */
    {5,    {1,3,4,5,8,0,0,0}},        /* 1,0,0,1,1,1,0,1 */
    {5,    {2,3,4,5,8,0,0,0}},        /* 1,0,0,1,1,1,1,0 */
    {6,    {1,2,3,4,5,8,0,0}},        /* 1,0,0,1,1,1,1,1 */
      
    {2,    {6,8,0,0,0,0,0,0}},        /* 1,0,1,0,0,0,0,0 */
    {3,    {1,6,8,0,0,0,0,0}},        /* 1,0,1,0,0,0,0,1 */
    {3,    {2,6,8,0,0,0,0,0}},        /* 1,0,1,0,0,0,1,0 */
    {4,    {1,2,6,8,0,0,0,0}},        /* 1,0,1,0,0,0,1,1 */
    {3,    {3,6,8,0,0,0,0,0}},        /* 1,0,1,0,0,1,0,0 */
    {4,    {1,3,6,8,0,0,0,0}},        /* 1,0,1,0,0,1,0,1 */
    {4,    {2,3,6,8,0,0,0,0}},        /* 1,0,1,0,0,1,1,0 */
    {5,    {1,2,3,6,8,0,0,0}},        /* 1,0,1,0,0,1,1,1 */
    {3,    {4,6,8,0,0,0,0,0}},        /* 1,0,1,0,1,0,0,0 */
    {4,    {1,4,6,8,0,0,0,0}},        /* 1,0,1,0,1,0,0,1 */
    {4,    {2,4,6,8,0,0,0,0}},        /* 1,0,1,0,1,0,1,0 */
    {5,    {1,2,4,6,8,0,0,0}},        /* 1,0,1,0,1,0,1,1 */
    {4,    {3,4,6,8,0,0,0,0}},        /* 1,0,1,0,1,1,0,0 */
    {5,    {1,3,4,6,8,0,0,0}},        /* 1,0,1,0,1,1,0,1 */
    {5,    {2,3,4,6,8,0,0,0}},        /* 1,0,1,0,1,1,1,0 */
    {6,    {1,2,3,4,6,8,0,0}},        /* 1,0,1,0,1,1,1,1 */
      
    {3,    {5,6,8,0,0,0,0,0}},        /* 1,0,1,1,0,0,0,0 */
    {4,    {1,5,6,8,0,0,0,0}},        /* 1,0,1,1,0,0,0,1 */
    {4,    {2,5,6,8,0,0,0,0}},        /* 1,0,1,1,0,0,1,0 */
    {5,    {1,2,5,6,8,0,0,0}},        /* 1,0,1,1,0,0,1,1 */
    {4,    {3,5,6,8,0,0,0,0}},        /* 1,0,1,1,0,1,0,0 */
    {5,    {1,3,5,6,8,0,0,0}},        /* 1,0,1,1,0,1,0,1 */
    {5,    {2,3,5,6,8,0,0,0}},        /* 1,0,1,1,0,1,1,0 */
    {6,    {1,2,3,5,6,8,0,0}},        /* 1,0,1,1,0,1,1,1 */
    {4,    {4,5,6,8,0,0,0,0}},        /* 1,0,1,1,1,0,0,0 */
    {5,    {1,4,5,6,8,0,0,0}},        /* 1,0,1,1,1,0,0,1 */
    {5,    {2,4,5,6,8,0,0,0}},        /* 1,0,1,1,1,0,1,0 */
    {6,    {1,2,4,5,6,8,0,0}},        /* 1,0,1,1,1,0,1,1 */
    {5,    {3,4,5,6,8,0,0,0}},        /* 1,0,1,1,1,1,0,0 */
    {6,    {1,3,4,5,6,8,0,0}},        /* 1,0,1,1,1,1,0,1 */
    {6,    {2,3,4,5,6,8,0,0}},        /* 1,0,1,1,1,1,1,0 */
    {7,    {1,2,3,4,5,6,8,0}},        /* 1,0,1,1,1,1,1,1 */
     

      
    {2,    {7,8,0,0,0,0,0,0}},        /* 1,1,0,0,0,0,0,0 */
    {3,    {1,7,8,0,0,0,0,0}},        /* 1,1,0,0,0,0,0,1 */
    {3,    {2,7,8,0,0,0,0,0}},        /* 1,1,0,0,0,0,1,0 */
    {4,    {1,2,7,8,0,0,0,0}},        /* 1,1,0,0,0,0,1,1 */
    {3,    {3,7,8,0,0,0,0,0}},        /* 1,1,0,0,0,1,0,0 */
    {4,    {1,3,7,8,0,0,0,0}},        /* 1,1,0,0,0,1,0,1 */
    {4,    {2,3,7,8,0,0,0,0}},        /* 1,1,0,0,0,1,1,0 */
    {5,    {1,3,7,8,0,0,0,0}},        /* 1,1,0,0,0,1,1,1 */
    {3,    {4,7,8,0,0,0,0,0}},        /* 1,1,0,0,1,0,0,0 */
    {4,    {1,4,7,8,0,0,0,0}},        /* 1,1,0,0,1,0,0,1 */
    {4,    {2,4,7,8,0,0,0,0}},        /* 1,1,0,0,1,0,1,0 */
    {5,    {1,2,4,7,8,0,0,0}},        /* 1,1,0,0,1,0,1,1 */
    {4,    {3,4,7,8,0,0,0,0}},        /* 1,1,0,0,1,1,0,0 */
    {5,    {1,3,4,7,8,0,0,0}},        /* 1,1,0,0,1,1,0,1 */
    {5,    {2,3,4,7,8,0,0,0}},        /* 1,1,0,0,1,1,1,0 */
    {6,    {1,2,3,4,7,8,0,0}},        /* 1,1,0,0,1,1,1,1 */
      
    {3,    {5,7,8,0,0,0,0,0}},        /* 1,1,0,1,0,0,0,0 */
    {4,    {1,5,7,8,0,0,0,0}},        /* 1,1,0,1,0,0,0,1 */
    {4,    {2,5,7,8,0,0,0,0}},        /* 1,1,0,1,0,0,1,0 */
    {5,    {1,2,5,7,8,0,0,0}},        /* 1,1,0,1,0,0,1,1 */
    {4,    {3,5,7,8,0,0,0,0}},        /* 1,1,0,1,0,1,0,0 */
    {5,    {1,3,5,7,8,0,0,0}},        /* 1,1,0,1,0,1,0,1 */
    {5,    {2,3,5,7,8,0,0,0}},        /* 1,1,0,1,0,1,1,0 */
    {6,    {1,2,3,5,7,8,0,0}},        /* 1,1,0,1,0,1,1,1 */
    {4,    {4,5,7,8,0,0,0,0}},        /* 1,1,0,1,1,0,0,0 */
    {5,    {1,4,5,7,8,0,0,0}},        /* 1,1,0,1,1,0,0,1 */
    {5,    {2,4,5,7,8,0,0,0}},        /* 1,1,0,1,1,0,1,0 */
    {6,    {1,2,4,5,7,8,0,0}},        /* 1,1,0,1,1,0,1,1 */
    {5,    {3,4,5,7,8,0,0,0}},        /* 1,1,0,1,1,1,0,0 */
    {6,    {1,3,4,5,7,8,0,0}},        /* 1,1,0,1,1,1,0,1 */
    {6,    {2,3,4,5,7,8,0,0}},        /* 1,1,0,1,1,1,1,0 */
    {7,    {1,2,3,4,5,7,8,0}},        /* 1,1,0,1,1,1,1,1 */
      
    {3,    {6,7,8,0,0,0,0,0}},        /* 1,1,1,0,0,0,0,0 */
    {4,    {1,6,7,8,0,0,0,0}},        /* 1,1,1,0,0,0,0,1 */
    {4,    {2,6,7,8,0,0,0,0}},        /* 1,1,1,0,0,0,1,0 */
    {5,    {1,2,6,7,8,0,0,0}},        /* 1,1,1,0,0,0,1,1 */
    {4,    {3,6,7,8,0,0,0,0}},        /* 1,1,1,0,0,1,0,0 */
    {5,    {1,3,6,7,8,0,0,0}},        /* 1,1,1,0,0,1,0,1 */
    {5,    {2,3,6,7,8,0,0,0}},        /* 1,1,1,0,0,1,1,0 */
    {6,    {1,2,3,6,7,8,0,0}},        /* 1,1,1,0,0,1,1,1 */
    {4,    {4,6,7,8,0,0,0,0}},        /* 1,1,1,0,1,0,0,0 */
    {5,    {1,4,6,7,8,0,0,0}},        /* 1,1,1,0,1,0,0,1 */
    {5,    {2,4,6,7,8,0,0,0}},        /* 1,1,1,0,1,0,1,0 */
    {6,    {1,2,4,6,7,8,0,0}},        /* 1,1,1,0,1,0,1,1 */
    {5,    {3,4,6,7,8,0,0,0}},        /* 1,1,1,0,1,1,0,0 */
    {6,    {1,3,4,6,7,8,0,0}},        /* 1,1,1,0,1,1,0,1 */
    {6,    {2,3,4,6,7,8,0,0}},        /* 1,1,1,0,1,1,1,0 */
    {7,    {1,2,3,4,6,7,8,0}},        /* 1,1,1,0,1,1,1,1 */
      
    {4,    {5,6,7,8,0,0,0,0}},        /* 1,1,1,1,0,0,0,0 */
    {5,    {1,5,6,7,8,0,0,0}},        /* 1,1,1,1,0,0,0,1 */
    {5,    {2,5,6,7,8,0,0,0}},        /* 1,1,1,1,0,0,1,0 */
    {6,    {1,2,5,6,7,8,0,0}},        /* 1,1,1,1,0,0,1,1 */
    {5,    {3,5,6,7,8,0,0,0}},        /* 1,1,1,1,0,1,0,0 */
    {6,    {1,3,5,6,7,8,0,0}},        /* 1,1,1,1,0,1,0,1 */
    {6,    {2,3,5,6,7,8,0,0}},        /* 1,1,1,1,0,1,1,0 */
    {7,    {1,2,3,5,6,7,8,0}},        /* 1,1,1,1,0,1,1,1 */
    {5,    {4,5,6,7,8,0,0,0}},        /* 1,1,1,1,1,0,0,0 */
    {6,    {1,4,5,6,7,8,0,0}},        /* 1,1,1,1,1,0,0,1 */
    {6,    {2,4,5,6,7,8,0,0}},        /* 1,1,1,1,1,0,1,0 */
    {7,    {1,2,4,5,6,7,8,0}},        /* 1,1,1,1,1,0,1,1 */
    {6,    {3,4,5,6,7,8,0,0}},        /* 1,1,1,1,1,1,0,0 */
    {7,    {1,3,4,5,6,7,8,0}},        /* 1,1,1,1,1,1,0,1 */
    {7,    {2,3,4,5,6,7,8,0}},        /* 1,1,1,1,1,1,1,0 */
    {8,    {1,2,3,4,5,6,7,8}}         /* 1,1,1,1,1,1,1,1 */
    }; 




/*****************************************************************************
* Function Name    : gl_sm_setup_tcp_server
* Arguments        : gw_S8bit           *p_server_ip_addr,
*                    gw_U16bit          server_portno
* Return value     : gw_S32bit
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_setup_tcp_server

#ifdef _ANSI_C_
    ( gw_S8bit                  *p_server_ip_addr, 
      gw_U16bit                 server_portno
    )
#else
    ( p_server_ip_addr, server_portno )
    gw_S8bit                    *p_server_ip_addr;
    gw_U16bit                   server_portno;
#endif
    
/****************************************************************************/
{
    gw_S32bit                      sockid;
    gw_S32bit                   ioctl_arg = 1;
    gw_S8bit                    sock_optval = 1;
    
    if(GW_ERROR == (sockid = gl_sm_create_socket(GL_SM_AF_INET, 
                GL_SM_SOCK_STREAM, GL_SM_IPPROTO_TCP)))
    {
        return GW_ERROR;
    }
    else if(GL_FAILURE == gl_sm_ioctl_socket(sockid, GL_SM_FIONBIO, 
                &ioctl_arg))
    {
        gl_sm_close_socket(sockid);
        return GW_ERROR;
    }
    else if(GL_FAILURE == gl_sm_setsockopt(sockid, GL_SM_LVL_SOL_SOCKET,
                GL_SM_SO_REUSEADDR, &sock_optval, (gw_S32bit)sizeof(gw_S32bit)))
    {
        gl_sm_close_socket(sockid);
        return GW_ERROR;
    }
    else if(GL_FAILURE == gl_sm_bind_socket(sockid, p_server_ip_addr, 
                server_portno))
    {
        gl_sm_close_socket(sockid);
        return GW_ERROR;
    }
    else if(GL_FAILURE == gl_sm_listen_socket(sockid, 5))
    {
        gl_sm_close_socket(sockid);
        return GW_ERROR;
    }
    else
    {
        return sockid;
    }
}





/*****************************************************************************
* Function Name    : gl_sm_setup_tcp_client
* Arguments        : gw_S8bit           *p_server_ip_addr,
*                    gw_U16bit          server_portno
* Return value     : gw_S32bit
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_setup_tcp_client

#ifdef _ANSI_C_
    ( gw_S8bit                  *p_server_ip_addr, 
      gw_U16bit                 server_portno
    )
#else
    ( p_server_ip_addr, server_portno )
    gw_S8bit                    *p_server_ip_addr;
    gw_U16bit                   server_portno;
#endif
    
/****************************************************************************/
{
    gw_S32bit                      sockid;
    
    if(GW_ERROR == (sockid = gl_sm_create_socket(GL_SM_AF_INET, 
                GL_SM_SOCK_STREAM, GL_SM_IPPROTO_TCP)))
    {
        return GW_ERROR;
    }
    else if(GL_FAILURE == gl_sm_connect_socket(sockid, p_server_ip_addr, 
                server_portno))
    {
        gl_sm_close_socket(sockid);
        return GW_ERROR;
    }
    else
    {
        return sockid;
    }
}






/*****************************************************************************
* Function Name    : gl_sm_setup_udp_server_or_client
* Arguments        : gw_S8bit           *p_ip_addr,
*                    gw_U16bit          portno
* Return value     : gw_S32bit
* Functionality    : Connects a specified socket to a specified remote address.
* Functions called :
*****************************************************************************/
gw_S32bit 
gl_sm_setup_udp_server_or_client

#ifdef _ANSI_C_
    ( gw_S8bit                  *p_ip_addr,
      gw_U16bit                 portno 
    )
#else
    ( p_ip_addr, portno )
      gw_S8bit                  *p_ip_addr;
      gw_U16bit                 portno;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   sockid;
    
    if(GW_ERROR == (sockid = gl_sm_create_socket(GL_SM_AF_INET, 
                GL_SM_SOCK_DGRAM, GL_SM_IPPROTO_UDP)))
    {
        return GW_ERROR;
    }
    /* 
     * To be confirmed.
     * else if(GL_FAILURE == gl_sm_setsocketopt(sockid, GL_SM_LVL_SOL_SOCKET,
     *           GL_SM_SO_REUSEADDR, &sock_optval, sizeof(gw_S8bit)))
     * {
     *    return GW_ERROR;
     * }
     */
    else if(GL_FAILURE == gl_sm_bind_socket(sockid, p_ip_addr, portno))
    {
        gl_sm_close_socket(sockid);
        return GW_ERROR;
    }
    else 
    {
        return sockid;
    }
}





/*****************************************************************************
* Function Name    : gl_sm_tlm_init
* Arguments        : gw_U32bit                  num_of_tpt_link_ids, 
*                    gw_U32bit                  num_of_tpt_link_keys, 
*                    gl_sm_tpt_type_et          transport_type,
*                    gl_error_t                 *p_ecode 
* Return value     : gl_return_t
* Functionality    : Intializes the transport link mapping databases for the 
*                    given transport type.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_init

#ifdef _ANSI_C_
    ( gw_U32bit                 num_of_tpt_link_ids, 
      gw_U32bit                 num_of_tpt_link_keys, 
      gl_sm_tpt_type_et         transport_type, 
      gl_error_t                *p_ecode    
    )
#else
    ( num_of_tpt_link_ids, num_of_tpt_link_keys, transport_type, p_ecode )
      gw_U32bit                 num_of_tpt_link_ids;
      gw_U32bit                 num_of_tpt_link_keys; 
      gl_sm_tpt_type_et         transport_type; 
      gl_error_t                *p_ecode;    
#endif
    
/****************************************************************************/
{
    gw_U32bit                   link_index = 0;

    if ((GL_SM_TLM_TPT_TYPE_UDP         == transport_type) ||
        (GL_SM_TLM_TPT_TYPE_TCP         == transport_type))
    {
#if (defined GW_SOLARIS ) || (defined GW_LINUX) || (defined GW_QNX)
        if(FD_SETSIZE < num_of_tpt_link_ids)
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }
#endif
        if(GL_NULL == g_a_p_tcp_udp_lnk_id_info)
        {
            g_a_p_tcp_udp_lnk_id_info = (gl_sm_tcp_udp_lnk_id_info_t *)
                SYS_MALLOC(FD_SETSIZE * sizeof(gl_sm_tcp_udp_lnk_id_info_t));

            if(GL_NULL == g_a_p_tcp_udp_lnk_id_info)
            {
                *p_ecode = GL_SM_ERR_NO_RESOURCES;
                return GL_FAILURE;
            }
#ifdef GW_WINNT
            /*
             * Initialize the first entry.
             */
            g_a_p_tcp_udp_lnk_id_info[0].link_id = IPPORT_RESERVED -1;
#else
            gl_mm_memset( (gw_U8bit *)g_a_p_tcp_udp_lnk_id_info, 0,
                    FD_SETSIZE * sizeof(gl_sm_tcp_udp_lnk_id_info_t));
#endif
        }
    }
    else if ((GL_SM_TLM_TPT_TYPE_SCTP_UDP == transport_type) ||
            (GL_SM_TLM_TPT_TYPE_SCTP_TCP == transport_type))
    {
        if(GL_NULL == g_a_p_sctp_lnk_id_info)
        {
            g_a_p_sctp_lnk_id_info = (gl_sm_sctp_lnk_id_info_t *)SYS_MALLOC(
                num_of_tpt_link_ids * sizeof(gl_sm_sctp_lnk_id_info_t));

            if(GL_NULL == g_a_p_sctp_lnk_id_info)
            {
                *p_ecode = GL_SM_ERR_NO_RESOURCES;
                return GL_FAILURE;
            }
            for(link_index = 0;link_index < num_of_tpt_link_ids; link_index++)
            {
                g_a_p_sctp_lnk_id_info[link_index].link_id = -1;
                g_a_p_sctp_lnk_id_info[link_index].p_tpt_lnk_id_data = GL_NULL;
            }
            g_gl_sctp_fd_setsize = num_of_tpt_link_ids;
        }
    }
    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at 
         * present.
         */ 
        *p_ecode = GL_SM_ERR_TRANSPORT_NOT_SUPPORTED;
        return GL_FAILURE;
    }

    if(GL_SM_TLM_TPT_TYPE_UDP == transport_type)
    {
        if(GL_FAILURE == gl_htm_create_hash_table(
                            GL_NULL,
                            &(g_udp_lnk_map_info.hash_table_id),
                            num_of_tpt_link_keys,
                            num_of_tpt_link_keys,
                            GL_HTM_USER_DEFINED_KEY_TYPE,
                                /* default type of key */
                            GL_HTM_HASH_FUNC_FOR_STRING_KEYS,
                            GL_NULL,
                            (gl_htm_free_key_elem_fn_t)gl_sm_htm_free_func, 
                            GL_NULL,
                            GL_NULL,
                            p_ecode))
        {
            *p_ecode = GL_SM_ERR_NO_RESOURCES;
            return GL_FAILURE;
        }
    }
    else if(GL_SM_TLM_TPT_TYPE_TCP == transport_type)
    {
        if(GL_NULL == g_a_p_tcp_lnk_map_info)
        {
            g_a_p_tcp_lnk_map_info = (gl_sm_tcp_lnk_map_info_t *)
                SYS_MALLOC(sizeof(gl_sm_tcp_lnk_map_info_t)*FD_SETSIZE);
            if(GL_NULL == g_a_p_tcp_lnk_map_info)
            {
                *p_ecode = GL_SM_ERR_NO_RESOURCES;
                return GL_FAILURE;
            }
#ifdef GW_WINNT
            /*
             * Initialize the first entry
             */
            g_a_p_tcp_lnk_map_info[0].link_id = IPPORT_RESERVED -1;
#else
            for(link_index = 0;link_index < FD_SETSIZE; link_index++)
            {
                g_a_p_tcp_lnk_map_info[link_index].su_lnk_id = 0;
                g_a_p_tcp_lnk_map_info[link_index].link_id = 0;
                g_a_p_tcp_lnk_map_info[link_index].p_su_lnk_id_data = GL_NULL;
            }
#endif
        }
        
    } 
    else if(GL_SM_TLM_TPT_TYPE_SCTP_UDP == transport_type)
    {
        if(GL_FAILURE == gl_htm_create_hash_table(
                            GL_NULL,
                            &(g_sctp_listen_lnk_map_info.hash_table_id),
                            num_of_tpt_link_keys,
                            num_of_tpt_link_keys,
                            GL_HTM_USER_DEFINED_KEY_TYPE,
                                /* default type of key */
                            GL_HTM_HASH_FUNC_FOR_STRING_KEYS,
                            GL_NULL,
                            (gl_htm_free_key_elem_fn_t)gl_sm_htm_free_func, 
                            GL_NULL,
                            GL_NULL,
                            p_ecode))
        {
            *p_ecode = GL_SM_ERR_NO_RESOURCES;
            return GL_FAILURE;
        }
    }
    else if(GL_SM_TLM_TPT_TYPE_SCTP_TCP == transport_type)
    {
        if(GL_NULL == g_a_p_sctp_lnk_map_info)
        {
            g_a_p_sctp_lnk_map_info = (gl_sm_sctp_lnk_map_info_t *)
                SYS_MALLOC(sizeof(gl_sm_sctp_lnk_map_info_t)*g_gl_sctp_fd_setsize);
            if(GL_NULL == g_a_p_sctp_lnk_map_info)
            {
                *p_ecode = GL_SM_ERR_NO_RESOURCES;
                return GL_FAILURE;
            }
            for(link_index = 0;link_index < g_gl_sctp_fd_setsize; link_index++)
            {
                g_a_p_sctp_lnk_map_info[link_index].su_lnk_id = 0;
                g_a_p_sctp_lnk_map_info[link_index].link_id = -1;
                g_a_p_sctp_lnk_map_info[link_index].p_su_lnk_id_data = GL_NULL;
            }
        }
    }
    return GL_SUCCESS;
}





/*****************************************************************************
* Function Name    : gl_sm_tlm_set_tpt_link_id_data
* Arguments        : gl_sm_lnk_type_et         lnk_type,
*                    gw_U32bit                  tpt_lnk_id,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data,
*                    gl_error_t                 *p_ecode 
* Return value     : gl_return_t
* Functionality    : Set the transport link-id data corresponding to given 
*                    transport link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_set_tpt_lnk_id_data

#ifdef _ANSI_C_
    
    ( gl_sm_lnk_type_et         lnk_type, 
      gw_U32bit                 tpt_lnk_id, 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
      gl_error_t                *p_ecode    
    )
#else
    ( lnk_type, tpt_lnk_id, p_tpt_lnk_id_data, p_ecode )
      gl_sm_lnk_type_et         lnk_type;
      gw_U32bit                 tpt_lnk_id; 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data; 
      gl_error_t                *p_ecode;    
#endif
    
/****************************************************************************/
{
    gw_U32bit                   link_index = 0;

    if(GL_SM_TLM_LNK_TYPE_UDP_TCP == lnk_type)
        
    {
#if (defined GW_SOLARIS ) || (defined GW_LINUX) || (defined GW_QNX) || (defined GW_VXWORKS)
        link_index = tpt_lnk_id;
#elif GW_WINNT
        /*
         * using Socket ID less than IPPORT_RESERVED to identify the sentinal 
         * for list of socket id's since Id's less than IPPORT_RESERVED  are 
         * reserved for system in WINNT.
         */
        while((link_index < FD_SETSIZE ) && 
              (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != 
               IPPORT_RESERVED -1)
              && (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != (gw_S32bit)
                  tpt_lnk_id ))
        {
            link_index++;
        }
        if(((FD_SETSIZE-1) > link_index) && (g_a_p_tcp_udp_lnk_id_info[
                link_index].link_id != (gw_S32bit)tpt_lnk_id))
        {
            g_a_p_tcp_udp_lnk_id_info[link_index + 1].link_id =
                IPPORT_RESERVED - 1;
        }
#endif
        if(FD_SETSIZE <= link_index )
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        } 
        g_a_p_tcp_udp_lnk_id_info[link_index].link_id = (gw_S32bit)tpt_lnk_id;
        /*
         * Set the Link Id Data field.
         */
        g_a_p_tcp_udp_lnk_id_info[link_index].p_tpt_lnk_id_data = 
            p_tpt_lnk_id_data;
    }
    else if(GL_SM_TLM_LNK_TYPE_SCTP_UDP == lnk_type)
    {
        while((link_index < g_gl_sctp_fd_setsize ) && 
              (g_a_p_sctp_lnk_id_info[link_index].link_id != -1) &&
              (g_a_p_sctp_lnk_id_info[link_index].link_id != (gw_S32bit)
                  tpt_lnk_id ))
        {
            link_index++;
        }
        if(g_gl_sctp_fd_setsize <= link_index )
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }
        g_a_p_sctp_lnk_id_info[link_index].link_id = (gw_S32bit)tpt_lnk_id;
        /*
         * Set the Link Id Data field.
         */
        g_a_p_sctp_lnk_id_info[link_index].p_tpt_lnk_id_data = 
            p_tpt_lnk_id_data;
    }
    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at 
         * present.
         */ 
        *p_ecode = GL_SM_ERR_TRANSPORT_NOT_SUPPORTED;
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}
        


/*****************************************************************************
* Function Name    : gl_sm_tlm_get_tpt_link_id_data
* Arguments        : gl_sm_lnk_type_et         lnk_type,
*                    gw_U32bit                  tpt_lnk_id,
*                    gl_sm_tpt_lnk_id_data_st   **p_p_tpt_lnk_id_data,
*                    gl_error_t                 *p_ecode 
* Return value     : gl_return_t
* Functionality    : Get the transport link-id data corresponding to given G
*                    transport link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_get_tpt_lnk_id_data

#ifdef _ANSI_C_
    ( gl_sm_lnk_type_et         lnk_type,
      gw_U32bit                 tpt_lnk_id, 
      gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data, 
      gl_error_t                *p_ecode    
    )
#else
    ( lnk_type, tpt_lnk_id, p_p_tpt_lnk_id_data, p_ecode )
      gl_sm_lnk_type_et         lnk_type,
      gw_U32bit                 tpt_lnk_id; 
      gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data; 
      gl_error_t                *p_ecode;    
#endif
    
/****************************************************************************/
{
    gw_U32bit                   link_index = 0;

    if(GL_SM_TLM_LNK_TYPE_UDP_TCP == lnk_type)
    {
#if (defined GW_SOLARIS ) || (defined GW_LINUX) || (defined GW_QNX) || (defined GW_VXWORKS)
        link_index = tpt_lnk_id;
#elif GW_WINNT
        /*
         * using Socket ID less than IPPORT_RESERVED to identify the sentinal for 
         * list of socket id's since Id's less than IPPORT_RESERVED  are reserved 
         * for system in WINNT.
         */
        while((link_index < FD_SETSIZE ) && 
              (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != IPPORT_RESERVED -1)
              && (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != (gw_S32bit)
                  tpt_lnk_id ))
        {
            link_index++;
        }
#endif
        if((FD_SETSIZE <= link_index ) ||
           (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != (gw_S32bit)
            tpt_lnk_id ))
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }
        
        *p_p_tpt_lnk_id_data = g_a_p_tcp_udp_lnk_id_info[link_index].
            p_tpt_lnk_id_data;
    }
    else if (GL_SM_TLM_LNK_TYPE_SCTP_UDP == lnk_type)
    {
        while((link_index < g_gl_sctp_fd_setsize) && 
              (g_a_p_sctp_lnk_id_info[link_index].link_id != -1)
              && (g_a_p_sctp_lnk_id_info[link_index].link_id != (gw_S32bit)
                  tpt_lnk_id ))
        {
            link_index++;
        }
        if((g_gl_sctp_fd_setsize <= link_index ) ||
           (g_a_p_sctp_lnk_id_info[link_index].link_id != (gw_S32bit)
            tpt_lnk_id ))
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }
        
        *p_p_tpt_lnk_id_data = g_a_p_sctp_lnk_id_info[link_index].
            p_tpt_lnk_id_data;
    }
    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at 
         * present.
         */ 
        *p_ecode = GL_SM_ERR_TRANSPORT_NOT_SUPPORTED;
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}
        
       
       


/*****************************************************************************
* Function Name    : gl_sm_tlm_delete_tpt_link_id_data
* Arguments        : gl_sm_lnk_type_et         lnk_type,
*                    gw_U32bit                  tpt_lnk_id,
*                    gl_sm_tpt_lnk_id_data_st   **p_p_tpt_lnk_id_data,
*                    gl_error_t                 *p_ecode 
* Return value     : gl_return_t
* Functionality    : Delete the transport link-id data corresponding to given 
*                    transport link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_delete_tpt_lnk_id_data

#ifdef _ANSI_C_
    ( gl_sm_lnk_type_et         lnk_type,
      gw_U32bit                 tpt_lnk_id, 
      gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data, 
      gl_error_t                *p_ecode    
    )
#else
    ( lnk_type, tpt_lnk_id, p_p_tpt_lnk_id_data, p_ecode )
      gl_sm_lnk_type_et         lnk_type;
      gw_U32bit                 tpt_lnk_id; 
      gl_sm_tpt_lnk_id_data_st  **p_p_tpt_lnk_id_data; 
      gl_error_t                *p_ecode;    
#endif
    
/****************************************************************************/
{
    gw_U32bit                   link_index = 0;

    if(GL_SM_TLM_LNK_TYPE_UDP_TCP == lnk_type)
    {
#if (defined GW_SOLARIS ) || (defined GW_LINUX) || (defined GW_QNX) || (defined GW_VXWORKS)
        link_index = tpt_lnk_id;
#elif GW_WINNT
        /*
         * using Socket ID less than IPPORT_RESERVED to identify the sentinal for 
         * list of socket id's since Id's less than IPPORT_RESERVED  are reserved 
         * for system in WINNT.
         */
        while((link_index < FD_SETSIZE ) && 
              (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != IPPORT_RESERVED -1)
              && (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != (gw_S32bit)
                  tpt_lnk_id ))
        {
            link_index++;
        }
#endif
        if((FD_SETSIZE <= link_index ) || 
           (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != (gw_S32bit)
            tpt_lnk_id ))
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }
        *p_p_tpt_lnk_id_data = g_a_p_tcp_udp_lnk_id_info[link_index].
            p_tpt_lnk_id_data;

#ifdef GW_WINNT
        while((link_index < (FD_SETSIZE - 1) ) && 
              (g_a_p_tcp_udp_lnk_id_info[link_index].link_id != IPPORT_RESERVED -1))
        {
            g_a_p_tcp_udp_lnk_id_info[link_index].link_id = 
                g_a_p_tcp_udp_lnk_id_info[link_index + 1].link_id;
            g_a_p_tcp_udp_lnk_id_info[link_index].p_tpt_lnk_id_data = 
                g_a_p_tcp_udp_lnk_id_info[link_index + 1].p_tpt_lnk_id_data;
            link_index++;
        }
        g_a_p_tcp_udp_lnk_id_info[link_index].link_id = IPPORT_RESERVED -1;
#else
        g_a_p_tcp_udp_lnk_id_info[link_index].link_id = 0;
#endif
        g_a_p_tcp_udp_lnk_id_info[link_index].p_tpt_lnk_id_data = GL_NULL;
    }
    else if (GL_SM_TLM_LNK_TYPE_SCTP_UDP == lnk_type)
    {
        while((link_index < g_gl_sctp_fd_setsize) && 
              (g_a_p_sctp_lnk_id_info[link_index].link_id != -1)
              && (g_a_p_sctp_lnk_id_info[link_index].link_id != (gw_S32bit)
                  tpt_lnk_id ))
        {
            link_index++;
        }
        if((g_gl_sctp_fd_setsize <= link_index) || 
           (g_a_p_sctp_lnk_id_info[link_index].link_id != (gw_S32bit)
            tpt_lnk_id ))
        {
            *p_ecode = GL_SM_ERR_INV_NUM_OF_LKN_ID_VAL;
            return GL_FAILURE;
        }
        *p_p_tpt_lnk_id_data = g_a_p_sctp_lnk_id_info[link_index].
            p_tpt_lnk_id_data;

        while((link_index < (g_gl_sctp_fd_setsize - 1) ) && 
              (g_a_p_sctp_lnk_id_info[link_index].link_id != -1))
        {
            g_a_p_sctp_lnk_id_info[link_index].link_id = 
                g_a_p_sctp_lnk_id_info[link_index + 1].link_id;
            g_a_p_sctp_lnk_id_info[link_index].p_tpt_lnk_id_data = 
                g_a_p_sctp_lnk_id_info[link_index + 1].p_tpt_lnk_id_data;
            link_index++;
        }
        g_a_p_sctp_lnk_id_info[link_index].link_id = -1;
        g_a_p_sctp_lnk_id_info[link_index].p_tpt_lnk_id_data = GL_NULL;
    }
    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at 
         * present.
         */ 
        *p_ecode = GL_SM_ERR_TRANSPORT_NOT_SUPPORTED;
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}
        
       
       

/*****************************************************************************
* Function Name    : gl_sm_tlm_set_tpt_link_key_map
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data, 
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gw_U32bit                  su_lnk_id,
*                    gl_pvoid_t                 p_su_lnk_id_data,
*                    gl_error_t                 *p_ecode 
* Return value     : gl_return_t
* Functionality    : Set the mapping of transport link key (transport 
*                    parameters) and system user provided logical link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_set_tpt_link_key_map

#ifdef _ANSI_C_
    ( gl_sm_tpt_type_et         transport_type, 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
      gw_U32bit                 su_lnk_id,
      gl_pvoid_t                p_su_lnk_id_data,
      gl_error_t                *p_ecode    
    )
#else
    ( transport_type, p_tpt_lnk_id_data, p_tpt_lnk_key, su_lnk_id,
      p_su_lnk_id_data, p_ecode )
      gl_sm_tpt_type_et         transport_type; 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data; 
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
      gw_U32bit                 su_lnk_id;
      gl_pvoid_t                p_su_lnk_id_data;
      gl_error_t                *p_ecode;    
#endif
    
/****************************************************************************/
{
    gw_U8bit                    *p_udp_str_key = GL_NULL;
    gl_sm_tcp_lnk_map_info_t    *p_map_info = GL_NULL;
    gl_pvoid_t                  p_key_node_handle;
    gl_hash_key_ut              hash_key;
    gw_U32bit                   link_index = 0; 
        
    if(GL_SM_TLM_TPT_TYPE_UDP == transport_type) 
    {
        p_udp_str_key = (gw_U8bit *)gl_mm_get_buffer(GL_NULL,
                GL_SM_UDP_SCTP_STR_KEY, p_ecode);
        
        if(GL_NULL == p_udp_str_key)
        {
            return GL_FAILURE;
        }
        gl_mm_memset(p_udp_str_key,'$',GL_SM_UDP_SCTP_STR_KEY);
        p_map_info =  (gl_sm_tcp_lnk_map_info_t *)gl_mm_get_buffer(GL_NULL, 
                sizeof(gl_sm_tcp_lnk_map_info_t), p_ecode);
        
        if(GL_NULL == p_map_info)
        {
            return GL_FAILURE;
        }
        
        p_map_info->su_lnk_id = su_lnk_id;
        p_map_info->p_su_lnk_id_data = p_su_lnk_id_data;
        
        if(GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key(p_udp_str_key
                    ,p_tpt_lnk_key, GL_SM_TLM_TPT_TYPE_UDP, p_ecode))
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = p_udp_str_key;
        
        if(GL_FAILURE == gl_htm_add_key_to_hash_table(
                    GL_NULL,
                    g_udp_lnk_map_info.hash_table_id,
                    &hash_key,
                    (gl_pvoid_t)p_map_info,
                    &p_key_node_handle,
                    p_ecode ))
        {
            return GL_FAILURE;
        }
    }
    else if(GL_SM_TLM_TPT_TYPE_TCP == transport_type) 
    {
#if (defined GW_SOLARIS ) || (defined GW_LINUX) || (defined GW_QNX) || (defined GW_VXWORKS)
        link_index =  p_tpt_lnk_key->tcp_tpt_info.socket_id;
#elif GW_WINNT
        /*
         * using Socket ID less than IPPORT_RESERVED to identify the sentinal for 
         * list of socket id's since Id's less than IPPORT_RESERVED  are reserved 
         * for system in WINNT.
         */
        while((link_index < FD_SETSIZE ) && 
              (g_a_p_tcp_lnk_map_info[link_index].link_id != IPPORT_RESERVED -1) &&
              (g_a_p_tcp_lnk_map_info[link_index].link_id != 
               p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            link_index++;
        }
#endif
        if(FD_SETSIZE <= link_index)
        {
            return GL_FAILURE;
        }
        g_a_p_tcp_lnk_map_info[link_index].su_lnk_id 
            = su_lnk_id;
        g_a_p_tcp_lnk_map_info[link_index].
            p_su_lnk_id_data = p_su_lnk_id_data;
        g_a_p_tcp_lnk_map_info[link_index].link_id = 
             p_tpt_lnk_key->tcp_tpt_info.socket_id;
#ifdef GW_WINNT
        if( link_index < FD_SETSIZE -1 )
        {
            g_a_p_tcp_lnk_map_info[link_index + 1].link_id 
                = IPPORT_RESERVED - 1;
        }
#endif

    } 
    else if(GL_SM_TLM_TPT_TYPE_SCTP_UDP == transport_type) 
    {
        p_udp_str_key = (gw_U8bit *)gl_mm_get_buffer(GL_NULL,
                GL_SM_UDP_SCTP_STR_KEY, p_ecode);
        
        if(GL_NULL == p_udp_str_key)
        {
            return GL_FAILURE;
        }
        gl_mm_memset(p_udp_str_key,'$',GL_SM_UDP_SCTP_STR_KEY);
        p_map_info =  (gl_sm_sctp_lnk_map_info_t *)gl_mm_get_buffer(GL_NULL,
                sizeof(gl_sm_sctp_lnk_map_info_t), p_ecode);
        
        if(GL_NULL == p_map_info)
        {
            return GL_FAILURE;
        }
        
        p_map_info->su_lnk_id = su_lnk_id;
        p_map_info->p_su_lnk_id_data = p_su_lnk_id_data;
        
        if(GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key(p_udp_str_key
                    ,p_tpt_lnk_key, GL_SM_TLM_TPT_TYPE_SCTP_UDP, p_ecode))
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = p_udp_str_key;
        
        if(GL_FAILURE == gl_htm_add_key_to_hash_table(
                    GL_NULL,
                    g_sctp_listen_lnk_map_info.hash_table_id,
                    &hash_key,
                    (gl_pvoid_t)p_map_info,
                    &p_key_node_handle,
                    p_ecode ))
        {
            return GL_FAILURE;
        }
    }
    else if(GL_SM_TLM_TPT_TYPE_SCTP_TCP == transport_type) 
    {
        while((link_index < g_gl_sctp_fd_setsize) &&
              (g_a_p_sctp_lnk_map_info[link_index].link_id != -1) &&
              (g_a_p_sctp_lnk_map_info[link_index].link_id !=
               p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            link_index++;
        }
        if(g_gl_sctp_fd_setsize <= link_index)
        {
            return GL_FAILURE;
        }
        g_a_p_sctp_lnk_map_info[link_index].su_lnk_id 
            = su_lnk_id;
        g_a_p_sctp_lnk_map_info[link_index].
            p_su_lnk_id_data = p_su_lnk_id_data;
        g_a_p_sctp_lnk_map_info[link_index].link_id = 
             p_tpt_lnk_key->tcp_tpt_info.socket_id;
    }
    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at 
         * present.
         */ 
        /*
         * To remove warning.
         */
        *p_ecode = GL_SM_ERR_TRANSPORT_NOT_SUPPORTED;
        p_tpt_lnk_id_data = p_tpt_lnk_id_data;
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}
        
    

/*****************************************************************************
* Function Name    : gl_sm_tlm_get_tpt_link_key_map
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data, 
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gw_U32bit                  *p_su_lnk_id,
*                    gl_pvoid_t                 *p_p_su_lnk_id_data,
*                    gl_error_t                 *p_ecode 
* Return value     : gl_return_t
* Functionality    : Get the mapping of transport link key (transport 
*                    parameters) and system user provided logical link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_get_tpt_link_key_map

#ifdef _ANSI_C_
    ( gl_sm_tpt_type_et         transport_type, 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
      gw_U32bit                 *p_su_lnk_id,
      gl_pvoid_t                *p_p_su_lnk_id_data,
      gl_error_t                *p_ecode    
    )
#else
    ( transport_type, p_tpt_lnk_id_data, p_tpt_lnk_key, p_su_lnk_id,
      p_p_su_lnk_id_data, p_ecode )
      gl_sm_tpt_type_et         transport_type; 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data; 
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
      gw_U32bit                 *p_su_lnk_id;
      gl_pvoid_t                *p_p_su_lnk_id_data;
      gl_error_t                *p_ecode;    
#endif
    
/****************************************************************************/
{
    gw_U8bit                    udp_str_key[GL_SM_UDP_SCTP_STR_KEY];
    gl_sm_tcp_lnk_map_info_t    *p_map_info = GL_NULL;
    gl_hash_key_ut              hash_key;
    gw_U32bit                   link_index = 0; 
    
    if(GL_SM_TLM_TPT_TYPE_UDP == transport_type) 
    {
        gl_mm_memset(udp_str_key, '$', GL_SM_UDP_SCTP_STR_KEY);

        if(GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key(udp_str_key, 
                    p_tpt_lnk_key, GL_SM_TLM_TPT_TYPE_UDP, p_ecode))
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = udp_str_key;
        if(GL_FAILURE == gl_htm_get_key_ref_from_hash_table(
                    GL_NULL,
                    g_udp_lnk_map_info.hash_table_id,
                    &hash_key,
                    (gl_pvoid_t *)&p_map_info,
                    p_ecode ))
        {
            return GL_FAILURE;
        }
        *p_su_lnk_id = p_map_info->su_lnk_id;
        *p_p_su_lnk_id_data = p_map_info->p_su_lnk_id_data;
    }
    else if(GL_SM_TLM_TPT_TYPE_TCP == transport_type) 
    {
#if (defined GW_SOLARIS ) || (defined GW_LINUX) || (defined GW_QNX) || (defined GW_VXWORKS)
        link_index =  p_tpt_lnk_key->tcp_tpt_info.socket_id;
#elif GW_WINNT
        /*
         * using Socket ID less than IPPORT_RESERVED to identify the sentinal for 
         * list of socket id's since Id's less than IPPORT_RESERVED  are reserved 
         * for system in WINNT.
         */
        while((link_index < FD_SETSIZE ) && 
              (g_a_p_tcp_lnk_map_info[link_index].link_id != IPPORT_RESERVED -1) &&
              (g_a_p_tcp_lnk_map_info[link_index].link_id != 
               p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            link_index++;
        }
        if( g_a_p_tcp_lnk_map_info[link_index].link_id != 
                p_tpt_lnk_key->tcp_tpt_info.socket_id)
        {
            return GL_FAILURE;
        }
#endif
        if((FD_SETSIZE <= link_index) || (g_a_p_tcp_lnk_map_info[link_index].link_id 
                    != p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            return GL_FAILURE;
        }
        *p_su_lnk_id = g_a_p_tcp_lnk_map_info[link_index].su_lnk_id; 
        *p_p_su_lnk_id_data = g_a_p_tcp_lnk_map_info[link_index].
            p_su_lnk_id_data;
    }
    else if(GL_SM_TLM_TPT_TYPE_SCTP_UDP == transport_type) 
    {
        gl_mm_memset(udp_str_key, '$', GL_SM_UDP_SCTP_STR_KEY);

        if(GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key(udp_str_key, 
                    p_tpt_lnk_key, GL_SM_TLM_TPT_TYPE_SCTP_UDP, p_ecode))
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = udp_str_key;
        if(GL_FAILURE == gl_htm_get_key_ref_from_hash_table(
                    GL_NULL,
                    g_sctp_listen_lnk_map_info.hash_table_id,
                    &hash_key,
                    (gl_pvoid_t *)&p_map_info,
                    p_ecode ))
        {
            return GL_FAILURE;
        }
        *p_su_lnk_id = p_map_info->su_lnk_id;
        *p_p_su_lnk_id_data = p_map_info->p_su_lnk_id_data;
    }
    else if(GL_SM_TLM_TPT_TYPE_SCTP_TCP == transport_type) 
    {
        while((link_index < g_gl_sctp_fd_setsize ) && 
              (g_a_p_sctp_lnk_map_info[link_index].link_id != -1) &&
              (g_a_p_sctp_lnk_map_info[link_index].link_id != 
               p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            link_index++;
        }
        if((g_gl_sctp_fd_setsize <= link_index) || (g_a_p_sctp_lnk_map_info[link_index].link_id 
                    != p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            return GL_FAILURE;
        }
        *p_su_lnk_id = g_a_p_sctp_lnk_map_info[link_index].su_lnk_id; 
        *p_p_su_lnk_id_data = g_a_p_sctp_lnk_map_info[link_index].
            p_su_lnk_id_data;
    }
    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at 
         * present.
         */ 
        /*
         * To remove warning.
         */
        *p_ecode = GL_SM_ERR_TRANSPORT_NOT_SUPPORTED;
        p_tpt_lnk_id_data = p_tpt_lnk_id_data;
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}





/*****************************************************************************
* Function Name    : gl_sm_tlm_delete_tpt_link_key_map
* Arguments        : gl_sm_tpt_type_et          transport_type,
*                    gl_sm_tpt_lnk_id_data_st   *p_tpt_lnk_id_data, 
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gw_U32bit                  *p_su_lnk_id,
*                    gl_pvoid_t                 *p_p_su_lnk_id_data,
*                    gl_error_t                 *p_ecode 
* Return value     : gl_return_t
* Functionality    : Delete the mapping of transport link key (transport 
*                    parameters) and system user provided logical link-id.
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_tlm_delete_tpt_link_key_map

#ifdef _ANSI_C_
    ( gl_sm_tpt_type_et         transport_type, 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data, 
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
      gw_U32bit                 *p_su_lnk_id,
      gl_pvoid_t                *p_p_su_lnk_id_data,
      gl_error_t                *p_ecode    
    )
#else
    ( transport_type, p_tpt_lnk_id_data, p_tpt_lnk_key, p_su_lnk_id,
      p_p_su_lnk_id_data, p_ecode )
      gl_sm_tpt_type_et         transport_type; 
      gl_sm_tpt_lnk_id_data_st  *p_tpt_lnk_id_data; 
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
      gw_U32bit                 *p_su_lnk_id;
      gl_pvoid_t                *p_p_su_lnk_id_data;
      gl_error_t                *p_ecode;    
#endif
    
/****************************************************************************/
{
    gw_U8bit                    udp_str_key[GL_SM_UDP_SCTP_STR_KEY];
    gl_sm_tcp_lnk_map_info_t    *p_map_info = GL_NULL;
    gl_hash_key_ut              hash_key;
    gl_pvoid_t                  p_stored_key;
    gw_U32bit                   link_index = 0; 
    
    if(GL_SM_TLM_TPT_TYPE_UDP == transport_type) 
    {
        gl_mm_memset(udp_str_key, '$', GL_SM_UDP_SCTP_STR_KEY);

        if(GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key(udp_str_key, 
                    p_tpt_lnk_key, GL_SM_TLM_TPT_TYPE_UDP, p_ecode))
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = udp_str_key;
        if(GL_FAILURE == gl_htm_remove_key_from_hash_table(
                    GL_NULL,
                    g_udp_lnk_map_info.hash_table_id,
                    &hash_key,
                    &p_stored_key,
                    (gl_pvoid_t *)&p_map_info,
                    p_ecode ))
        {
            return GL_FAILURE;
        }
        *p_su_lnk_id = p_map_info->su_lnk_id;
        *p_p_su_lnk_id_data = p_map_info->p_su_lnk_id_data;

        /*
         * Free the memory allocated for the String key.
         */ 
        if(GL_FAILURE == gl_mm_release_buffer(GL_NULL, p_stored_key, p_ecode))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_mm_release_buffer(GL_NULL, ( gl_pvoid_t )p_map_info, p_ecode))
        {
            return GL_FAILURE;
        }
    }
    else if(GL_SM_TLM_TPT_TYPE_TCP == transport_type) 
    {
#if (defined GW_SOLARIS ) || (defined GW_LINUX) || (defined GW_QNX) || (defined GW_VXWORKS)
        link_index =  p_tpt_lnk_key->tcp_tpt_info.socket_id;
#elif GW_WINNT
        /*
         * using Socket ID less than IPPORT_RESERVED to identify the sentinal for 
         * list of socket id's since Id's less than IPPORT_RESERVED  are reserved 
         * for system in WINNT.
         */
        while((link_index < FD_SETSIZE ) && 
              (g_a_p_tcp_lnk_map_info[link_index].link_id != IPPORT_RESERVED -1) &&
              (g_a_p_tcp_lnk_map_info[link_index].link_id != 
               p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            link_index++;
        }
        if( g_a_p_tcp_lnk_map_info[link_index].link_id != 
                p_tpt_lnk_key->tcp_tpt_info.socket_id)
        {
            return GL_FAILURE;
        }
#endif
        *p_su_lnk_id = g_a_p_tcp_lnk_map_info[link_index].
            su_lnk_id; 
        *p_p_su_lnk_id_data = g_a_p_tcp_lnk_map_info[link_index].
            p_su_lnk_id_data;
#ifdef GW_WINNT
        while((link_index < (FD_SETSIZE - 1) ) && 
              (g_a_p_tcp_lnk_map_info[link_index].link_id != IPPORT_RESERVED -1))
        {
            g_a_p_tcp_lnk_map_info[link_index].link_id = 
                g_a_p_tcp_lnk_map_info[link_index + 1].link_id;
            g_a_p_tcp_lnk_map_info[link_index].p_su_lnk_id_data = 
                g_a_p_tcp_lnk_map_info[link_index + 1].p_su_lnk_id_data;
            g_a_p_tcp_lnk_map_info[link_index].su_lnk_id = 
                g_a_p_tcp_lnk_map_info[link_index + 1].su_lnk_id;
            link_index++;
        }
        g_a_p_tcp_lnk_map_info[link_index].link_id = IPPORT_RESERVED -1;
#else
        g_a_p_tcp_lnk_map_info[link_index].link_id = 0;
#endif
        g_a_p_tcp_lnk_map_info[link_index].p_su_lnk_id_data = GL_NULL;
        g_a_p_tcp_lnk_map_info[link_index].su_lnk_id = 0;
    }
    else if(GL_SM_TLM_TPT_TYPE_SCTP_UDP == transport_type) 
    {
        gl_mm_memset(udp_str_key, '$', GL_SM_UDP_SCTP_STR_KEY);

        if(GL_FAILURE == gl_sm_convert_udp_tpt_key_to_string_key(udp_str_key, 
                    p_tpt_lnk_key, GL_SM_TLM_TPT_TYPE_SCTP_UDP, p_ecode))
        {
            return GL_FAILURE;
        }

        hash_key.p_user_def_id = udp_str_key;
        if(GL_FAILURE == gl_htm_remove_key_from_hash_table(
                    GL_NULL,
                    g_sctp_listen_lnk_map_info.hash_table_id,
                    &hash_key,
                    &p_stored_key,
                    (gl_pvoid_t *)&p_map_info,
                    p_ecode ))
        {
            return GL_FAILURE;
        }
        *p_su_lnk_id = p_map_info->su_lnk_id;
        *p_p_su_lnk_id_data = p_map_info->p_su_lnk_id_data;

        /*
         * Free the memory allocated for the String key.
         */ 
        if(GL_FAILURE == gl_mm_release_buffer(GL_NULL, p_stored_key, p_ecode))
        {
            return GL_FAILURE;
        }
        if(GL_FAILURE == gl_mm_release_buffer(GL_NULL, ( gl_pvoid_t )p_map_info, p_ecode))
        {
            return GL_FAILURE;
        }
    }
    else if(GL_SM_TLM_TPT_TYPE_SCTP_TCP == transport_type) 
    {
        while((link_index < g_gl_sctp_fd_setsize) && 
              (g_a_p_sctp_lnk_map_info[link_index].link_id != -1) &&
              (g_a_p_sctp_lnk_map_info[link_index].link_id != 
               p_tpt_lnk_key->tcp_tpt_info.socket_id))
        {
            link_index++;
        }
        if( g_a_p_sctp_lnk_map_info[link_index].link_id != 
                p_tpt_lnk_key->tcp_tpt_info.socket_id)
        {
            return GL_FAILURE;
        }
        *p_su_lnk_id = g_a_p_sctp_lnk_map_info[link_index].
            su_lnk_id; 
        *p_p_su_lnk_id_data = g_a_p_sctp_lnk_map_info[link_index].
            p_su_lnk_id_data;
        while((link_index < (g_gl_sctp_fd_setsize - 1) ) && 
              (g_a_p_sctp_lnk_map_info[link_index].link_id != -1))
        {
            g_a_p_sctp_lnk_map_info[link_index].link_id = 
                g_a_p_sctp_lnk_map_info[link_index + 1].link_id;
            g_a_p_sctp_lnk_map_info[link_index].p_su_lnk_id_data = 
                g_a_p_sctp_lnk_map_info[link_index + 1].p_su_lnk_id_data;
            g_a_p_sctp_lnk_map_info[link_index].su_lnk_id = 
                g_a_p_sctp_lnk_map_info[link_index + 1].su_lnk_id;
            link_index++;
        }
        g_a_p_sctp_lnk_map_info[link_index].link_id = -1;
        g_a_p_sctp_lnk_map_info[link_index].p_su_lnk_id_data = GL_NULL;
        g_a_p_sctp_lnk_map_info[link_index].su_lnk_id = 0;
    }
    else
    {
        /*
         * Other transports as (SCTP/MTP-3/ATM etc) are not supported at 
         * present.
         */ 
        /*
         * To remove warning.
         */
        *p_ecode = GL_SM_ERR_TRANSPORT_NOT_SUPPORTED;
        p_tpt_lnk_id_data = p_tpt_lnk_id_data;
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}


/*****************************************************************************
* Function Name    : gl_sm_convert_udp_tpt_key_to_string_key
* Arguments        : gw_U8bit                   *p_str_key,
*                    gl_sm_tlm_tpt_lnk_key_ut   *p_tpt_lnk_key,
*                    gl_error_t                 *p_ecope
* Return value     : gl_return_t
* Functionality    : Convert UDP transport parameters to string key. 
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_convert_udp_tpt_key_to_string_key

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_str_key, 
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key,
      gl_sm_tpt_type_et         tpt_type,
      gl_error_t                *p_ecode
    )
#else
    ( p_str_key, p_tpt_lnk_key, tpt_type, p_ecode )
      gw_U8bit                  *p_str_key;
      gl_sm_tlm_tpt_lnk_key_ut  *p_tpt_lnk_key;
      gl_sm_tpt_type_et         tpt_type;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U8bit                    socket_id_str_array[11] = {0};
    gw_U8bit                    ip_addr_str_array[11] = {0};
    gw_U8bit                    port_num_str_array[6] = {0};
    gw_S32bit                   str_len;
    gw_S32bit                   i;

    if (GL_SM_TLM_TPT_TYPE_UDP == tpt_type)
    {
        p_str_key[0] = 'U';
    }
    else
    {
        p_str_key[0] = 'S';
    }
    if(GL_FAILURE == gl_stm_itoa(p_tpt_lnk_key->udp_tpt_info.socket_id, 
                socket_id_str_array, p_ecode))
    {
        return GL_FAILURE;
    }
    
    if(GW_ERROR == (str_len = gl_stm_strlen(socket_id_str_array, p_ecode)))
    {
        return GL_FAILURE;
    }
    for(i=9,--str_len;(str_len>=0) && (i>=1);i--, str_len--)
    {
        p_str_key[i] = socket_id_str_array[str_len];
    }

    if(GL_FAILURE == gl_stm_itoa(p_tpt_lnk_key->udp_tpt_info.remote_port_number
                ,port_num_str_array, p_ecode))
    {
        return GL_FAILURE;
    }
    if(GW_ERROR == (str_len = gl_stm_strlen(port_num_str_array, p_ecode)))
    {
        return GL_FAILURE;
    }
    for(i=14,--str_len;(str_len>=0) && (i>=10);i--, str_len--)
    {
        p_str_key[i] = port_num_str_array[str_len];
    }

    if(p_tpt_lnk_key->udp_tpt_info.bitmask & GL_SM_UDP_TPT_KEY_IP)
    {
        /* 
         * If IP Address is part of the Transport key.
         */
        if(GL_FAILURE == gl_stm_itoa(p_tpt_lnk_key->udp_tpt_info.remote_ip_addr, 
                    ip_addr_str_array, p_ecode))
        {
            return GL_FAILURE;
        }

        if(GW_ERROR == (str_len = gl_stm_strlen(ip_addr_str_array, p_ecode)))
        {
            return GL_FAILURE;
        }

        for(i=24,--str_len;(str_len>=0) && (i>=15);i--, str_len--)
        {
            p_str_key[i] = ip_addr_str_array[str_len];
        }
        p_str_key[25]='\0';
    }
    else if(p_tpt_lnk_key->udp_tpt_info.bitmask & GL_SM_UDP_TPT_KEY_DNAME)
    {
        p_str_key[25]='\0';
        /* 
         * If Domain name is part of the Transport key.
         */
        if(GW_ERROR == (str_len = gl_stm_strlen(
                        p_tpt_lnk_key->udp_tpt_info.remote_dname,
                        p_ecode)))
        {
            return GL_FAILURE;
        }
        if( str_len > 64 )
        {
            return GL_FAILURE;
        }
        strcat((gw_S8bit *)p_str_key,
                (gw_S8bit *)p_tpt_lnk_key->udp_tpt_info.remote_dname);
    }
    else
    {
        /*
         * If Neither type of key is present return error.
         */
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}
        
/*****************************************************************************
* Function Name    : gl_sm_htm_free_func
* Arguments        : gw_U8bit                   *p_str_key,
*                    gl_pvoid_t                 p_elem_buf_ptr,
*                    gl_error_t                 *p_ecope
* Return value     : gl_return_t
* Functionality    : Convert UDP transport parameters to string key. 
* Functions called :
*****************************************************************************/
gl_return_t
gl_sm_htm_free_func

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_pvoid_t                p_elem_buf_ptr,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, p_elem_buf_ptr, p_ecode )
      gw_U32bit                 task_id; 
      gl_pvoid_t                p_elem_buf_ptr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    /*
     * As this function is to be called only for
     * API "gl_htm_remove_all_keys_from_hash_table"
     */
    task_id = task_id;
    p_elem_buf_ptr = p_elem_buf_ptr;
    p_ecode = p_ecode;

    return GL_SUCCESS;
}
        

#if defined(GW_SOLARIS) || defined(GW_LINUX) || defined(GW_QNX) || defined(GW_VXWORKS)

void
gl_sm_slm_fd_clear

#ifdef _ANSI_C_
    ( gl_fd_set_t               *p_rd_fd,
      gw_S32bit                 a_sock_id[],
      gw_U32bit                 num
    )
#else
    (p_rd_fd, a_sock_id, num)
      gl_fd_set_t               *p_rd_fd;
      gw_S32bit                 a_sock_id[];
      gw_U32bit                 num;
#endif

/****************************************************************************/
{
    gw_U16bit       loop_index;
    gw_U16bit       loop_index_1;
    gw_U16bit       prev_index;
    gw_S32bit       socket_id;
    gl_fd_info_st   *p_fd_info;
    for(loop_index=0; loop_index < num; loop_index++)
    {
        socket_id = (a_sock_id)[loop_index];
        FD_CLR(socket_id, &((p_rd_fd)->read_fd));
        p_fd_info = &((p_rd_fd)->a_fd_bit_index[socket_id/32]);
        if (0 < p_fd_info->num_of_fd_bits_set)
        {
            p_fd_info->num_of_fd_bits_set--;
            if (0 == p_fd_info->num_of_fd_bits_set)
            {
                if ((p_rd_fd)->first_index == (socket_id/32)+1)
                {
                    if (GL_NULL == p_fd_info->next_index)
                    {
                        (p_rd_fd)->first_index =
                            (p_rd_fd)->last_index = GL_NULL;
                    }
                    else
                    {
                        (p_rd_fd)->first_index = p_fd_info->next_index;
                        p_fd_info->next_index = GL_NULL;
                    }
                }
                else
                {
                    for (loop_index_1 = (p_rd_fd)->first_index-1;
                        loop_index_1 != (socket_id/32);
                        loop_index_1 = (p_rd_fd)->a_fd_bit_index[loop_index_1].
                                        next_index-1)
                    {
                        prev_index = loop_index_1;
                    }
                    (p_rd_fd)->a_fd_bit_index[prev_index].next_index =
                                p_fd_info->next_index;
                    if (GL_NULL == p_fd_info->next_index)
                    {
                        (p_rd_fd)->last_index = prev_index + 1;
                    }
                }
                p_fd_info->next_index = GL_NULL;
            }
        }
    }
}



void
gl_sm_slm_fd_set

#ifdef _ANSI_C_
    ( gl_fd_set_t               *p_rd_fd, 
      gw_S32bit                 a_sock_id[],
      gw_U32bit                 num
    )
#else
    ( p_rd_fd, a_sock_id, num )
      gl_fd_set_t               *p_rd_fd;
      gw_S32bit                 a_sock_id[];
      gw_U32bit                 num;
#endif
    
/****************************************************************************/
{
    gw_U16bit       loop_index; 
    gw_S32bit       socket_id; 
    gl_fd_info_st   *p_fd_info; 
    for(loop_index=0; loop_index < num; loop_index++) 
    { 
        socket_id = (a_sock_id)[loop_index]; 
        FD_SET(socket_id, &((p_rd_fd)->read_fd)); 
        p_fd_info = &((p_rd_fd)->a_fd_bit_index[socket_id/32]); 
        p_fd_info->num_of_fd_bits_set++; 
        if(1 == p_fd_info->num_of_fd_bits_set) 
        { 
            if(GL_NULL == (p_rd_fd)->first_index) 
            { 
                (p_rd_fd)->first_index = (p_rd_fd)->last_index = (socket_id/32)+1; 
            }
            else 
            { 
                (p_rd_fd)->a_fd_bit_index[(p_rd_fd)->last_index-1].next_index = (socket_id/32)+1; 
                (p_rd_fd)->last_index = (socket_id/32)+1; 
            } 
            p_fd_info->next_index = GL_NULL; 
        } 
    } 

}



void
gl_sm_slm_fd_get

#ifdef _ANSI_C_
    ( gl_fd_set_t               *p_rd_fd,
      gw_S32bit                 a_sock_id[],
      gw_U32bit                 *p_num
    )
#else
    ( p_rd_fd, a_sock_id, p_num )
      gl_fd_set_t               *p_rd_fd;
      gw_S32bit                 a_sock_id[];
      gw_U32bit                 *p_num;
#endif
    
/****************************************************************************/
{
    gw_U32bit       loop_index;
    gw_U32bit       l_index_1;
    gw_U32bit       ll_index;
    gw_U32bit       ret_sock_num_val=0;
    gw_U8bit        bit_value;
    for(loop_index = (p_rd_fd)->first_index; 
        (GL_NULL != loop_index) && (ret_sock_num_val < *(p_num)); 
        loop_index = (p_rd_fd)->a_fd_bit_index[loop_index].next_index)
    { 
        loop_index = loop_index-1;
#ifdef GW_LINUX 
        if(0 != (p_rd_fd)->read_fd.__fds_bits[loop_index])
#else
        if(0 != (p_rd_fd)->read_fd.fds_bits[loop_index])
#endif
        {
            for(ll_index = 0 ; ll_index < 4; ll_index++) 
            { 
#ifdef GW_LINUX
                bit_value = ((p_rd_fd)->read_fd.__fds_bits[loop_index] & gl_max_bit_value[ll_index])>>(8*ll_index); 
#else
                bit_value = ((p_rd_fd)->read_fd.fds_bits[loop_index] & gl_max_bit_value[ll_index])>>(8*ll_index); 
#endif

                if(0 < bit_value) 
                {
                    for(l_index_1 = 0; l_index_1 < g_a_bit_values[bit_value].num_of_bits; l_index_1++)
                    {
                        ((a_sock_id)[ret_sock_num_val++]) = (loop_index*32)+
                            (g_a_bit_values[bit_value].a_bit_val[l_index_1]-1)+(ll_index*8);
                    }
                } 
            }
        }
    }
    *p_num = ret_sock_num_val;
}
#endif
