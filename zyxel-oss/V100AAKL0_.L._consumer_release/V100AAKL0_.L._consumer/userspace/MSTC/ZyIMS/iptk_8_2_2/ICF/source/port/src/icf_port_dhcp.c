/*************************************************************************
 *
 *     FILENAME           :  icf_port_dhcp.c
 *
 *     DESCRIPTION        :  This file contains the functions used for processing 
 *                           of DHCP Query/Response .
 *
 *     Revision History   :
 *    DATE        NAME                REFERENCE            REASON
 *   ----------------------------------------------------------
 *   22-Jan-2008  Anurag Khare        ICF LLD        Initial Version.
 *   03-Jul-2008  Anurag Khare        SPR 18684      Compilation issue on gcc 4.2.3
 *   04-Mar-2009 Saurabh Gupta        SPR 19349      Klocworks fix
 *   11-Jun-2009 Ashutosh Mohan       SPR 19737      Changes done for GCC 4.1 warnings removal
 *   09-Jul-2009 Anuradha Gupta       SPR 19795      Modified 
 *                                                    icf_port_populate_sip_server_addresses  
 *   18-Jul-2009 Rajiv Kumar          IPTK Rel 8.2   Vxwork 5.4 Porting    
 *   04-Aug-2009 Anuradha Gupta       UT Defect      Modified icf_port_fetch_hw_addr 
 * 
 *     Copyright 2009, Aricent.
 *
 *************************************************************************/


#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#elif ICF_PORT_LINUX
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <stdlib.h>
#endif


#include "icf_port_dhcp_info.h"
#include "icf_macro.h"
#include "icf_port_intf_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_port_macros.h"
/*Include VxWorks specific files*/
#ifdef ICF_PORT_VXWORKS
#include "ioctl.h"
#include "if.h"
#include "ifLib.h"
#include "if_arp.h"
#include "etherLib.h"
#endif

extern icf_glb_pdb_st        *p_persistent_glb_pdb;


/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_process_dhcp_query
 *
 * DESCRIPTION:     This function performs a DHCP query on the configured DHCP 
 *                  Server address and fill the successful response into 
 *                  p_dhcp_resp in icf_config_data_st.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
*****************************************************************************/

icf_return_t icf_port_process_dhcp_query(
        icf_dhcp_resp_st**         p_p_dhcp_resp,
        icf_transport_address_st*  p_dhcp_server_addr,
        icf_duration_t             dhcp_response_timer,
        icf_transport_address_st   *p_self_ip,
        icf_uint16_t               dhcp_client_port,
        icf_error_t*               p_ecode)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_port_dhcp_client_node_st    *p_dhcp_client_node = ICF_NULL;
    icf_uint32_t                    client_fd = ICF_NULL;
    fd_set                          read_fd_set;
    fd_set                          write_fd_set;
    fd_set                          except_fd_set;
    icf_uint8_t                     *p_dhcp_msg = ICF_NULL;
    icf_uint8_t                     p_self_addr_str[ICF_MAX_STR_LEN] = "";
    icf_uint8_t                     p_dhcp_addr_str[ICF_MAX_STR_LEN] = "";
    icf_uint16_t                    msg_len = 0;
    icf_boolean_t                   resp_recvd = ICF_FALSE;
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)
    ICF_PORT_FD_ZERO(&read_fd_set);
    ICF_PORT_FD_ZERO(&write_fd_set);
    ICF_PORT_FD_ZERO(&except_fd_set);
    /* Allocate memory to the p_dhcp_client_node */
    p_dhcp_client_node =
            icf_port_memget(p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                            sizeof(icf_port_dhcp_client_node_st),
                            ICF_MEM_COMMON,
                            p_ecode);
    if (ICF_NULL == p_dhcp_client_node)
    {
       /*  Error Code need to be set because port_memget 
           does not set the error code */
       *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
       return ICF_FAILURE; 
    }
    
    /*  Allocate the Memory for the outgoing message */ 
    p_dhcp_msg =
            icf_port_memget(p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                            ICF_BLK_SIZE_MAX+1,
                            ICF_MEM_COMMON,
                            p_ecode);
    /*  The NULL check to handle the memget failure */
    if (ICF_NULL == p_dhcp_msg)
    {
        /*  Error Code need to be set because port_memget 
           does not set the error code */
        *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
        ret_val = ICF_FAILURE;
    }

    if(ICF_FAILURE == ret_val)
    {}
    else if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR ==
                    p_self_ip->addr.addr_type)
    {
        icf_port_sprintf((icf_int8_t *)p_self_addr_str,"%d.%d.%d.%d",
            p_self_ip->addr.addr.ipv4_addr.octet_1,
            p_self_ip->addr.addr.ipv4_addr.octet_2,
            p_self_ip->addr.addr.ipv4_addr.octet_3,
            p_self_ip->addr.addr.ipv4_addr.octet_4);

    }
#ifdef ICF_IPV6_ENABLED
    else if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR ==
                                p_self_ip->addr.addr_type)
    {
        ret_val = ICF_FAILURE;
    }
#endif
    if(ICF_FAILURE == ret_val)
    {}
    /* Open channel for DHCP client. This function will open a listening port for
     * DHCP query responses */
    else if (ICF_FAILURE == icf_port_open_broadcast_channel(
            dhcp_client_port, /*Port for which channel to be opened*/
            ICF_TRANSPORT_MODE_UDP, /*Transport mode*/
            ICF_TRUE, /*is_ip_present*/
            p_self_addr_str, /*Self IP address*/
            &client_fd, /*FD to be returned by this function*/
            p_ecode))
    {
        /* Set the error code */
        *p_ecode = ICF_CAUSE_DHCP_CLIENT_PORT_BIND_FAILURE;
        ret_val = ICF_FAILURE;
    }
    /*  if channel has been opened successfully set this fd in read fd set */
    else 
    {
        FD_SET(client_fd, &read_fd_set);

        icf_port_sprintf((icf_int8_t *)p_dhcp_addr_str,"%d.%d.%d.%d",
            p_dhcp_server_addr->addr.addr.ipv4_addr.octet_1,
            p_dhcp_server_addr->addr.addr.ipv4_addr.octet_2,
            p_dhcp_server_addr->addr.addr.ipv4_addr.octet_3,
            p_dhcp_server_addr->addr.addr.ipv4_addr.octet_4);
    }

    if (ICF_FAILURE == ret_val)
    {}
    /*  This function will form and send DHCP INFORM message to DHCP server */
    else if (ICF_FAILURE == icf_port_send_dhcp_inform(
                &client_fd, /*FD opened for this socket*/
                p_dhcp_client_node, /*DHCP client information*/
                p_dhcp_addr_str, /*DHCP server address*/ 
                p_self_addr_str, /*Self IP address*/
                p_ecode))
    {
        ret_val = ICF_FAILURE;    
    }
    else
    {
        icf_time_t     start_time = 0;
        icf_time_t     end_time = 0;
        icf_int32_t    temp_dur = 0;

        temp_dur = (icf_int32_t)dhcp_response_timer;

        /*Get Current System time in ms*/
        icf_port_get_current_time(&start_time);
    
        while (1)
        {
            /*  Now wait in select for any DHCP response  */
            if(ICF_FAILURE == icf_port_select(
                &read_fd_set, /*p_client_fd is set in this parameter*/
                &write_fd_set, /*Not set*/
                &except_fd_set, /*Not set*/
                temp_dur, /*Timeout value*/
                p_ecode))
            {
                if (ICF_TRUE == resp_recvd)
                {
                    /*  Fill the error cause DHCP FAILURE only when
                        some other error cause was not already filled */
                    if (ICF_NULL == *p_ecode)
                    {
                        /*  Set the error cause */
                        *p_ecode = ICF_CAUSE_DHCP_FAILURE;
                    }
                }
                else
                {
                    /*  Set the error cause */
                    *p_ecode = ICF_CAUSE_DHCP_SERVER_NOT_RESPONDING;
                }
                    ret_val = ICF_FAILURE;
                break;
            }
            /*  Now receive response from DHCP server and store in dhcp_msg */
            else if (ICF_FAILURE == (msg_len = icf_port_recvfrom(
                    client_fd, 
                    p_dhcp_msg)))
            {
                ret_val = ICF_FAILURE;
                /*  IPTK was stucking into infinite loop */
                break;
            }
            /*  Parse DHCP response and fill in p_p_dhcp_resp. If success response 
            received then this function will allocate p_p_dhcp_resp */
            else if (ICF_FAILURE == icf_port_parse_dhcp_resp(
                                                p_dhcp_msg, 
                                                p_dhcp_client_node,
                                                p_p_dhcp_resp,
                                                msg_len,
                                                p_ecode))
            {
                 resp_recvd = ICF_TRUE;
                 /*Below functionality should be done when failure occured and 
                  *it is required to be wait for further DHCPACK*/
                 /*Fetch Current system time in ms*/
                 icf_port_get_current_time(&end_time);
                 temp_dur = dhcp_response_timer - ((end_time - start_time) * 1000);

                 if (0 >= temp_dur)
                 {
                     ret_val = ICF_FAILURE;
                     break;
                 }
            }
            else
            {
                 break;
            }
        }
    }
    /*  Close the Socket after sending the DHCP Query */
    icf_port_close_socket(client_fd,p_ecode);
    /*  Precautionory check for avoiding the Klocworks warning */
    if ((ICF_NULL != p_dhcp_client_node) && (ICF_NULL !=
         p_dhcp_client_node->DhcpClientConfInfo.pDefaultOptions))
    {
        /*  Free the memory of options because we are going to free
         *  the p_dhcp_client_node
         */
        icf_port_memfree (p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                (icf_void_t*)p_dhcp_client_node->DhcpClientConfInfo.pDefaultOptions,
                ICF_MEM_COMMON,
                p_ecode);
        /*  The function icf_port_memfree did not make it NULL */
        p_dhcp_client_node->DhcpClientConfInfo.pDefaultOptions = ICF_NULL;
    }

     /*Free p_dhcp_client_node as it is not required now*/           
     icf_port_memfree (p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                (icf_void_t*)p_dhcp_client_node,
                ICF_MEM_COMMON,
                p_ecode);
     p_dhcp_client_node = ICF_NULL;

     /*Free p_dhcp_msg*/
     icf_port_memfree (p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                (icf_void_t*)p_dhcp_msg,
                ICF_MEM_COMMON,
                p_ecode);
     /*  The function icf_port_memfree did not make it NULL */
     p_dhcp_msg = ICF_NULL;

    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}




/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_send_dhcp_inform 
 *
 * DESCRIPTION:     This function form a DHCP INFORM message and send to 
 *                  DHCP server.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in
 *                  case of failure.
 *
*****************************************************************************/
icf_return_t icf_port_send_dhcp_inform(
                icf_uint32_t    *p_fd,
                icf_port_dhcp_client_node_st    *p_dhcp_client_node,
                icf_uint8_t            *p_dhcp_server_addr,
                icf_uint8_t       *p_self_ip_addr,
                icf_error_t        *p_ecode)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_port_dhcp_pkt_info_st    *p_out_pkt = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)

    /*  Allocate memory for an outgoing packet p_out_pkt  */
    p_out_pkt =
            icf_port_memget(p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                            sizeof(icf_port_dhcp_pkt_info_st),
                            ICF_MEM_COMMON,
                            p_ecode);
    /*  Check for MEMGET Failure */
    if (ICF_NULL == p_out_pkt)
    {
       /*Error Code need to be set because port_memget 
         does not set the error code */
       *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
        ret_val = ICF_FAILURE;    
    }
    else
    {
        /*  Fill DHCP header information in p_out_pkt->dhcp_hdr   */
        ret_val = icf_port_fill_dhcp_header(
                  p_out_pkt, /*Outgoing message structure*/
                  p_dhcp_client_node, /*DHCP client information structure*/
                  p_dhcp_server_addr, /*DHCP server address*/
                  p_self_ip_addr); /*Self IP address*/
        if (ICF_SUCCESS == ret_val)
        {
            /*  Fill required options in outgoing message  */
            icf_port_dhcp_fill_required_options(
                        p_dhcp_client_node,
                        p_out_pkt);
            /*  It would be NULL then Dhcp Query will be sent without options 
                So its better to not send the query and return failure */
            if (ICF_NULL == p_dhcp_client_node->DhcpClientConfInfo.pDefaultOptions)
            {
               /*Error Code need to be set because port_memget
                 does not set the error code */
                *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
                ret_val = ICF_FAILURE;
            }
            else
            {
                /*  Form network PDU and send message to DHCP server address  */
                ret_val = icf_port_send_dhcp_message(
                                               p_fd, 
                                               p_out_pkt, 
                                               ICF_PORT_DHCP_INFORM); /*DHCP message type*/
            
            }
        }
        
        /*  Free memory for p_out_pkt  */    
        icf_port_memfree (p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                         (icf_void_t*)p_out_pkt,
                         ICF_MEM_COMMON,
                         p_ecode);
        /*  After getting freed it needs to be NULL */
        p_out_pkt = ICF_NULL;
    } 
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    
    return ret_val;
        
}



/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_fill_dhcp_header
 *
 * DESCRIPTION:     This function will fill DHCP message header information in 
 *                  an outgoing DHCP packet. This function will be called from
 *                  icf_port_dhcp_send_inform            
 *
 * RETURNS:         icf_return_t it will return failure if hw address did not
 *                  fetched accuratly 
 *
*****************************************************************************/


icf_return_t icf_port_fill_dhcp_header(
            icf_port_dhcp_pkt_info_st *         p_out_pkt,
            icf_port_dhcp_client_node_st*       p_dhcp_client_node,
            icf_uint8_t*                        p_dhcp_server_addr,
            icf_uint8_t*                        p_self_ip_addr)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_time_t      u4CurrentTime = 0;
    icf_port_dhcp_hdr_st        *p_out_hdr = ICF_NULL;


    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)
    /*  Fetch DHCP header pointer from p_out_pkt  */
    p_out_hdr = &(p_out_pkt->dhcp_hdr);

    /*  Fill BOOTREQUEST as operation code as IPTK is sending DHCPINFORM  */
    p_out_hdr->Op = ICF_PORT_DHCP_BOOTREQUEST;

    /*  Fill hardware type as ETHERNET */
    p_out_hdr->htype = ICF_PORT_HWTYPE_ETHERNET;

    /*  Fill hardware address length  */  
    p_out_hdr->hlen = ICF_PORT_ENET_ADDR_LEN;

    /*  Fill number of hops  */
    p_out_hdr->hops = 0;

    /*  Fill the Transation ID in the packet and record the transaction ID in     
     *  the global Interface Table  
     */
    p_dhcp_client_node->u4Xid = (icf_uint32_t)icf_port_random (
                                     (icf_uint16_t)(ICF_PORT_MINIMUM_SEED_FOR_RANDOM),
                                     (icf_uint16_t)(ICF_PORT_MAXIMUM_SEED_FOR_RANDOM));
    p_out_hdr->xid = p_dhcp_client_node->u4Xid;

    /*  Fill the secs field in the packet and record the secs in the global 
     *  Interface Table Fetch current system time in u4CurrentTime 
     */
    icf_port_get_current_time(&u4CurrentTime);
    p_dhcp_client_node->u4Secs = u4CurrentTime;
    p_out_hdr->secs = (icf_uint16_t) u4CurrentTime;


    p_out_pkt->u4DstIpAddr = inet_addr((icf_int8_t *)p_dhcp_server_addr);
    
    /*  Fill Broadcast flag as 1 if DHCP server address is broadcast IP address
     *  otherwise fill 0 
     */
    if(ICF_NULL == (icf_port_strcmp(p_dhcp_server_addr,(icf_uint8_t*)"255.255.255.255")))
    {
        p_out_hdr->flags = htons(ICF_PORT_DHCP_BROADCAST_MASK);
    }
    else
    {
        p_out_hdr->flags = htons(0);
    }

    /*  Fill Self IP address  */
    p_dhcp_client_node->u4IpAddressOfferred = inet_addr((icf_int8_t *)p_self_ip_addr);

    p_out_hdr->ciaddr = p_dhcp_client_node->u4IpAddressOfferred;

    p_out_hdr->siaddr = 0;
    p_out_hdr->yiaddr = p_out_hdr->ciaddr;
    p_out_hdr->giaddr = 0;
    
    icf_port_memset(p_out_hdr->sname, 0, ICF_PORT_DHCP_SERVER_NAME_LEN);
    icf_port_memset(p_out_hdr->file, 0, ICF_PORT_DHCP_FILE_NAME_LEN);
    /*  Fill hardware address  */
    ret_val = icf_port_fetch_hw_addr(p_out_hdr->chaddr);
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_dhcp_fill_required_options
 *
 * DESCRIPTION:     This function will fill required options into DHCP message. 
 *                  Default option will be ICF_PORT_DHCP_OPT_SIP_SERVER_DISCOVERY 
 *                  for SIP server discovery.  
 *
 * RETURNS:         icf_void_t
 *
*****************************************************************************/
icf_void_t icf_port_dhcp_fill_required_options(
                icf_port_dhcp_client_node_st*    p_dhcp_client_node,
                icf_port_dhcp_pkt_info_st *      p_out_pkt)
{
    icf_uint16_t         u1Offset = 0;
    icf_error_t          ecode;
    icf_uint8_t          *pTempDefaultOpt = ICF_NULL;

    pTempDefaultOpt =
    p_dhcp_client_node->DhcpClientConfInfo.pDefaultOptions;

    if (ICF_NULL == pTempDefaultOpt)
    {
        pTempDefaultOpt = 
            icf_port_memget(p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                            ICF_PORT_DHCP_MAX_OPT_LEN, 
                            ICF_MEM_COMMON, 
                            &ecode);
        /*  If OPtions is still NULL then no need to do further 
            processing and return , its memget failure is checked 
            already in the function, from where it is called*/ 
        if (ICF_NULL == pTempDefaultOpt)
        {}
        else
        {
            p_dhcp_client_node->DhcpClientConfInfo.pDefaultOptions = pTempDefaultOpt;
    
            /*  Following options will be set in DHCP request */
            *(icf_uint8_t*)(pTempDefaultOpt + u1Offset) = ICF_PORT_DHCP_OPT_PARAMETER_LIST;
        
            u1Offset++;

            *(icf_uint8_t*)(pTempDefaultOpt + u1Offset) = 1; /*Length of options*/
            u1Offset++;

            /*  Fill option for SIP server discovery  */
            *(icf_uint8_t*)(pTempDefaultOpt + u1Offset) = ICF_PORT_DHCP_OPT_SIP_SERVER_DISCOVERY;
            u1Offset++;
        
            p_dhcp_client_node-> DhcpClientConfInfo.u2LenOfOptions =
            u1Offset;
            p_out_pkt->dhcp_hdr.pOptions = pTempDefaultOpt;
            p_out_pkt->u2Len = u1Offset;
            
        }
    }
}
    
/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_fetch_hw_addr
 *
 * DESCRIPTION:     This function will find the MAC Address of the local machine
 *
 *
 *
 * RETURNS:         icf_return_t
 *
*****************************************************************************/
icf_return_t icf_port_fetch_hw_addr(
                               icf_uint8_t* chaddr)
{

    icf_return_t   ret_val = ICF_FAILURE;
	/*Rel8.1 Windows porting*/
#ifdef ICF_PORT_LINUX
    /* Make the struture of type ifreq */
    icf_uint8_t i = 0;    
    struct ifreq *ifr = ICF_NULL, *ifend = ICF_NULL;
    struct ifreq ifreq ; 
    struct ifconf ifc  ;
    struct ifreq ifs[ICF_PORT_MAX_IFS] ;
    icf_int32_t SockFD;
    
    icf_port_memset((&ifreq), 0, sizeof(struct ifreq));
    icf_port_memset((&ifc), 0, sizeof(struct ifconf));
    for (i = 0; i<ICF_PORT_MAX_IFS; i++)
    {
        icf_port_memset((&ifs[i]), 0, sizeof(struct ifreq));   
    }
    /* Make a socket to pass in ioctl */
    SockFD = socket(AF_INET, SOCK_DGRAM, 0);
	if ( -1 == SockFD) return ICF_FAILURE;    

    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(SockFD, SIOCGIFCONF, &ifc) < 0)
    {
        ICF_PRINT("ioctl(SIOCGIFCONF): %m\n");
        ret_val = ICF_FAILURE;
    }
    else
    {
        ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
        /*  Loop traversal for getting the MAC Address */
        for (ifr = ifc.ifc_req; ifr < ifend; ifr++)
        {
            if (ifr->ifr_addr.sa_family == AF_INET)
            {
                strncpy(ifreq.ifr_name, ifr->ifr_name,sizeof(ifreq.ifr_name));
                if (ioctl (SockFD, SIOCGIFHWADDR, &ifreq) < 0)
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ret_val = ICF_SUCCESS;
                }
            }
        }
       /*  Last execution of this loop contains the MAC ADdress 
           if last time ioct get failed the ret_val would be 
           failure and in that situation we would not be able to
           fetch the MAC address so return failure*/
        if (ICF_SUCCESS == ret_val)
        {
            /*  Prepare the final MAC Address */
            icf_port_memcpy(chaddr, &ifreq.ifr_hwaddr.sa_data, 6);
        }
    }
	if (SockFD) close(SockFD); 

#else
	/*Rel8.1 Windows porting */
#ifdef ICF_PORT_VXWORKS
    /* Make the struture of type ifreq */
    icf_uint8_t    i = 0;    
    struct ifreq   *ifr = ICF_NULL, *ifend = ICF_NULL;
    struct ifconf  ifc  ;
    struct ifreq   ifs[ICF_PORT_MAX_IFS] ;
    icf_int32_t   SockFD;
    struct ifnet   *pIf = ICF_NULL;
    icf_uint8_t   eth_addr[ICF_MAX_STR_LEN] ={"\0"};
    icf_uint8_t   dstAddress[ICF_MAX_STR_LEN] ={"\0"};
    
    icf_port_memset((&ifc), 0, sizeof(struct ifconf));

    for (i = 0; i<ICF_PORT_MAX_IFS; i++)
    {
        icf_port_memset((&ifs[i]), 0, sizeof(struct ifreq));   
    }

    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;

    /* Make a socket to pass in ioctl */
    SockFD = socket(AF_INET, SOCK_DGRAM, 0);

    /*IOCTL option to get Configuration information*/
    if ((0 > SockFD) || (ioctl(SockFD, SIOCGIFCONF, (icf_uint32_t)&ifc) < 0))
    {
        ICF_PRINT(("\n[PORT]: \n ### failure in ioctl\n"));
        ret_val = ICF_FAILURE;
    }
    else
    {
        ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));

        /*  Loop traversal for getting the MAC Address */
        for (ifr = ifc.ifc_req; ifr < ifend; ifr++)
        {
            /*Fetched name is same as Application mentioned interface name then
             *fetch ifnet structure thru ifunit() function, if interface found then fetch
             *ethernet address*/
            if (0 == icf_port_strcmp(ifr->ifr_name, ICF_PORT_NETWORK_INTERFACE_NAME))
            {
                /*ifunit funciton returns ifnet structure*/
                pIf = ifunit(ifr->ifr_name);
    
                /*If this is NULL then return failure*/
                if (ICF_NULL == pIf)
                {
                    ret_val = ICF_FAILURE;
                    ICF_PRINT(("\n[PORT]: \n ### pIf is NULL, ifunit fails \n"));
                    break;
                }
                else
                {

                    if(OK != ifAddrGet(ifr->ifr_name,dstAddress))
                    {
                       ret_val = ICF_FAILURE;
                       ICF_PRINT(("\n[PORT]: \n ### ifDstAddrGet fails \n"));
                       break;
                    } 
                    /*Changes done for Vxwork 5.4 Porting*/
                    else if(OK != etherAddrResolve(pIf,dstAddress,
                               eth_addr, 1,1))
                    {
                       ret_val = ICF_FAILURE;
                       ICF_PRINT(("\n[PORT]: \n ### etherAddrResolve fails \n"));
                       break;
                    }
                    else
                    {
                       /*Copy 6 byte ethernet address*/
                       memcpy(chaddr, eth_addr, 6);
                       ret_val = ICF_SUCCESS;
                       break;
                    }

    
                }
            }
        }
    }
	if (SockFD) close(SockFD); 
#endif
#endif
    return ret_val;
}


/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_send_dhcp_message
 *
 * DESCRIPTION:     This function will pack and send the DHCP Message 
 *
 *
 *
 * RETURNS:         ICF_SUCCESS if successfully send otherwise ICF_FAILURE
 *
*****************************************************************************/
icf_return_t icf_port_send_dhcp_message(
                        icf_uint32_t                   *p_fd,
                        icf_port_dhcp_pkt_info_st      *p_out_pkt,
                        icf_uint8_t                    msg_type)
{
    icf_return_t         ret_val =ICF_SUCCESS;
    icf_uint8_t          message[ICF_PORT_MAX_OUT_MSG_SIZE] = {'\0'};
    icf_uint8_t          dhcp_ip_addr[16] = {'\0'};
    icf_error_t          ecode = 0;
    icf_uint16_t         msg_len = 0;
    icf_config_data_st   *p_cfg_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)    

    ret_val = icf_port_fill_dhcp_request(p_out_pkt, msg_type, message, &msg_len);
    if(ICF_SUCCESS == ret_val)
    {
        /* EXtract the Config data */
        p_cfg_data = (icf_config_data_st *)(p_persistent_glb_pdb-> \
                        p_glb_data->p_module_data[ICF_INT_MODULE_CFG]);        
        if(ICF_NULL != p_cfg_data)
        {
             /*  Extract the IP Address as a unsigned integer  */
             icf_port_sprintf((icf_int8_t *)dhcp_ip_addr,"%u.%u.%u.%u", \
                 p_cfg_data->p_dhcp_server_addr->addr.addr.ipv4_addr.octet_1, \
                 p_cfg_data->p_dhcp_server_addr->addr.addr.ipv4_addr.octet_2,
                 p_cfg_data->p_dhcp_server_addr->addr.addr.ipv4_addr.octet_3,
                 p_cfg_data->p_dhcp_server_addr->addr.addr.ipv4_addr.octet_4);
             /* Actual sending of the message */
             ret_val = icf_port_udp_send(*p_fd, \
                               message, \
                               msg_len, \
                               p_cfg_data->p_dhcp_server_addr->port_num, \
                               dhcp_ip_addr,
                               &ecode);
             
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}



/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_fill_dhcp_request
 *
 * DESCRIPTION:     This function will pack the DHCP Message for sending
 *
 *
 *
 * RETURNS:         ICF_SUCCESS if successfully pack otherwise ICF_FAILURE
 *
*****************************************************************************/
icf_return_t icf_port_fill_dhcp_request(
                         icf_port_dhcp_pkt_info_st   *p_out_pkt, 
                         icf_uint8_t                  msg_type,
                         icf_uint8_t                  *pMessage,
                         icf_uint16_t                 *p_msg_size)
{
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_uint16_t               Offset = 0;
    icf_port_dhcp_hdr_st       *pHdr;
    icf_uint8_t                gau1DhcpCMagicCookie[] = { 99,130,83,99};

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb) 
    pHdr = &(p_out_pkt->dhcp_hdr);
    /* pack the BOOTREQUEST Message */
    pMessage[Offset++] = ICF_PORT_DHCP_BOOTREQUEST;

    pMessage[Offset++] = pHdr->htype;
    pMessage[Offset++] = pHdr->hlen;
    pMessage[Offset++] = pHdr->hops;
    
    *(icf_uint32_t *) (pMessage + Offset) = pHdr->xid;
    Offset += 4;
    
    *(icf_uint16_t *) (pMessage + Offset) = (icf_uint16_t)(pHdr->secs);
    Offset += 2;

    *(icf_uint16_t *) (pMessage + Offset) = (icf_uint16_t)(pHdr->flags);
    Offset += 2;

    *(icf_uint32_t *) (pMessage + Offset) = pHdr->ciaddr;
    Offset += 4;

    *(icf_uint32_t *) (pMessage + Offset) = (icf_uint32_t)(pHdr->yiaddr);
    Offset += 4;

    *(icf_uint32_t *) (pMessage + Offset) = (icf_uint32_t)(pHdr->siaddr);
    Offset += 4;

    *(icf_uint32_t *) (pMessage + Offset) = (icf_uint32_t)(pHdr->giaddr);
    Offset += 4;

    memcpy(pMessage + Offset, pHdr->chaddr, 16);
    Offset += 16;

    memcpy(pMessage + Offset, pHdr->sname, 64);
    Offset += 64;

    memcpy(pMessage + Offset, pHdr->file, 128);
    Offset += 128;

    /* Add DHCP Magic cookie */
    memcpy(pMessage + Offset, gau1DhcpCMagicCookie, ICF_PORT_DHCP_LEN_MAGIC);
    Offset += ICF_PORT_DHCP_LEN_MAGIC;

    if ( ICF_PORT_DHCP_BOOTP_PKT != msg_type )
    {
        /* Add DHCP message type option tag */
        pMessage[Offset] = ICF_PORT_DHCP_OPT_MSG_TYPE;
        pMessage[Offset + 1] = 1;
        pMessage[Offset + 2] = msg_type;
        Offset += 3;
    }

    /* copy the options field */
    memcpy(pMessage + Offset, pHdr->pOptions, p_out_pkt->u2Len);
    Offset += p_out_pkt->u2Len;

    /* Add the end option */
    pMessage[Offset++] = (char) ICF_PORT_DHCP_OPT_END;

    *p_msg_size = Offset;
    
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)     
    return ret_val;

}


/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_parse_dhcp_resp 
 *
 * DESCRIPTION:     This function parses incoming DHCP response(DHCP ACK) and
 *                  populate it in given p_p_dhcp_resp.
 *                  
 *
 * RETURNS:         ICF_SUCCESS if successfully parse otherwise ICF_FAILURE
 *
 *****************************************************************************/
icf_return_t icf_port_parse_dhcp_resp(
        icf_uint8_t *p_dhcp_msg,
        icf_port_dhcp_client_node_st* p_dhcp_client_node,
        icf_dhcp_resp_st**  p_p_dhcp_resp,
        icf_uint16_t        msg_len,
        icf_error_t*   p_ecode)
{
    icf_return_t           ret_val =ICF_SUCCESS;
    icf_port_dhcp_hdr_st   Hdr;
    icf_uint16_t           Offset = 0;   
    icf_uint8_t            msg_type = 0;
    icf_uint8_t            gau1DhcpCMagicCookie[] = {0,0,0,0};
    icf_port_dhcp_opt_data_st   *p_dhcp_options = ICF_NULL;

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)

    if (ICF_PORT_MIN_DHCP_MSG >= msg_len)
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*  Allocate memory for icf_dhcp_resp_st in *p_p_dhcp_resp  */
        *p_p_dhcp_resp = icf_port_memget(p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                sizeof(icf_dhcp_resp_st), ICF_MEM_COMMON, p_ecode);
        
        if(ICF_NULL == *p_p_dhcp_resp)
        {
           /*Error Code need to be set because port_memget 
             does not set the error code */
           *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
            ret_val = ICF_FAILURE;
        }
        else
        {
            /*  skip the first opcode */ 
            Offset++;
            /* fetch hardware ttpe */
            Hdr.htype = p_dhcp_msg[Offset++];
        
            /*  Fetch hardware address length  */  
            Hdr.hlen  = p_dhcp_msg[Offset++];
    
            Hdr.hops  = p_dhcp_msg[Offset++];
    
            /*  Fetch the transactio ID */
            Hdr.xid =  *(icf_uint32_t *) (p_dhcp_msg + Offset);
            Offset += 4;
    
            /*  Check if Transaction ID is not same as sent in DHCPINFORM */
            if(p_dhcp_client_node->u4Xid != Hdr.xid)
            {
                /*Return error_code that Transaction-ID is invalid*/
                ret_val = ICF_FAILURE;
            }
            else
            {
                /*fetch the time in seconds took to give the response */
                (Hdr.secs) = *(icf_uint16_t *) (p_dhcp_msg + Offset);
                Offset += 2;
        
                (Hdr.flags) = *(icf_uint16_t *) (p_dhcp_msg + Offset);
                Offset += 2;
    
                /*  Fetch the ci address */
                Hdr.ciaddr = *(icf_uint32_t *) (p_dhcp_msg + Offset);
                Offset += 4;
        
                (Hdr.yiaddr) = *(icf_uint32_t *) (p_dhcp_msg + Offset);
                Offset += 4;
        
                /*  Fetch the service Identifier address */
                (Hdr.siaddr) = *(icf_uint32_t *) (p_dhcp_msg + Offset);
                Offset += 4;
        
                (Hdr.giaddr) = *(icf_uint32_t *) (p_dhcp_msg + Offset);
                Offset += 4;
        
                icf_port_memcpy(Hdr.chaddr , p_dhcp_msg + Offset, 16);
                Offset += 16;
                icf_port_memcpy(Hdr.sname , p_dhcp_msg + Offset, 64);
                Offset += 64;
                icf_port_memcpy(Hdr.file , p_dhcp_msg + Offset, 128);
                Offset += 128;
        
                /* Add DHCP Magic cookie */
                icf_port_memcpy(&gau1DhcpCMagicCookie , p_dhcp_msg + Offset, ICF_PORT_DHCP_LEN_MAGIC);
                Offset += ICF_PORT_DHCP_LEN_MAGIC;
        
                /*  Fetch the Message type */
                msg_type = p_dhcp_msg[Offset + 2];
                Offset += 3;
        
                /*  Check if it is a DHCPNACK if so then ignore it and return failure */
                if( ICF_PORT_DHCP_NACK == msg_type)
                {
                    /*Return failure that remote rejected the request*/
                    ret_val = ICF_FAILURE;
                }
                /*  Check is it DHCKACK if so then fetch the options */
                else if( ICF_PORT_DHCP_ACK == msg_type )
                {
                    if (ICF_FAILURE == 
                            icf_port_parse_dhcp_options(&(p_dhcp_msg[Offset]), 
                                &p_dhcp_options, 
                                msg_len - ICF_PORT_MIN_DHCP_MSG,
                                p_ecode))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else if (ICF_FAILURE == 
                            icf_port_populate_sip_server_addresses(*p_p_dhcp_resp, 
                                p_dhcp_options))
                    {
                        ret_val = ICF_FAILURE;
                    }
/*  THis feature is out of scope now so 
    if dns server address came then also no need to parse it */
#if 0
                    else if (ICF_FAILURE == 
                            icf_port_populate_dns_server_addresses(*p_p_dhcp_resp, 
                                p_dhcp_options))
                    {
                        ret_val = ICF_FAILURE;
                    }
#endif        
                    icf_port_free_dhcp_options(p_dhcp_options);
                }
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
            if (ICF_FAILURE == ret_val)
            {
                icf_port_memfree (p_persistent_glb_pdb->p_glb_data->p_glb_port_info, 
                        (icf_void_t*)*p_p_dhcp_resp, 
                        ICF_MEM_COMMON, 
                        p_ecode);
                /*  Make it NULL because the memory has been freed */
                *p_p_dhcp_resp = ICF_NULL;
            }
        }
    }
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_parse_dhcp_options
 *
 * DESCRIPTION:     This function will parse OPTIONS from received DHCP message
 *
 * RETURNS:         icf_return_t
 *
*****************************************************************************/
icf_return_t   icf_port_parse_dhcp_options(
        icf_uint8_t                 *p_dhcp_msg,
        icf_port_dhcp_opt_data_st   **p_p_dhcp_opt,
        icf_uint16_t                msg_len,
        icf_error_t*                p_ecode)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_port_dhcp_opt_data_st   *p_opt_data_curr = ICF_NULL;
    icf_port_dhcp_opt_data_st   *p_opt_data_prev = ICF_NULL;
    icf_error_t                 ecode = ICF_ERROR_NONE;
    icf_uint32_t                offset = 0;
    icf_uint16_t                rem_len = 0;
    icf_uint8_t                 op_code = 0;
    icf_uint8_t                 op_len = 0;

    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)
    rem_len = msg_len;

    /*Iterate thru all DHCP options and populate options in 
     * icf_port_dhcp_opt_data_st*/
    while(1)
    {
        if (0 == rem_len)
        {
            break;
        }

        /*  fetch the opcode and check it with ICF_PORT_DHCP_OPT_END */
        op_code = p_dhcp_msg[offset++];
        if( ICF_PORT_DHCP_OPT_END == op_code)
        {
            break;
        }
        else
        {
            /*  THis specifies that any option could never be of 
                less than equal to 2 bytes */
            if (rem_len <= ICF_PORT_MIN_DHCP_OPTION_LEN)
            {
                ret_val = ICF_FAILURE;
                break;
            }
            op_len = p_dhcp_msg[offset++];

            if (0 == op_len)
            {
                ret_val = ICF_FAILURE;
                break;
            }

            if (rem_len <= op_len)
            {
                ret_val = ICF_FAILURE;
                break;
            }

            p_opt_data_curr = icf_port_memget(p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                    sizeof(icf_port_dhcp_opt_data_st), ICF_MEM_COMMON, &ecode);

            if (ICF_NULL == p_opt_data_curr)
            {
               /*Error Code need to be set because port_memget 
                 does not set the error code */
                *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
                ret_val = ICF_FAILURE;
                break;
            }
            if (ICF_NULL == *p_p_dhcp_opt)
            {
                *p_p_dhcp_opt = p_opt_data_curr;
                p_opt_data_prev = p_opt_data_curr;
            }
            else
            {
                if ( p_opt_data_prev ) { p_opt_data_prev->p_next = p_opt_data_curr; }
                p_opt_data_prev = p_opt_data_curr;
            }
            p_opt_data_curr->option_id = op_code;
            p_opt_data_curr->option_len = op_len;
            p_opt_data_curr->p_option_str = 
            icf_port_memget(p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                    p_opt_data_curr->option_len + 1, ICF_MEM_COMMON, &ecode);
            /*  If there is a memory failure then we cant store the options 
                So return failure */
            if (ICF_NULL == p_opt_data_curr->p_option_str)
            {
                /*Error Code need to be set because port_memget 
                  does not set the error code */
                 *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;
                 ret_val = ICF_FAILURE;
                 break;
            } 

            icf_port_memcpy(p_opt_data_curr->p_option_str, 
                    &(p_dhcp_msg[offset]),
                    p_opt_data_curr->option_len);

            p_opt_data_curr->p_option_str[p_opt_data_curr->option_len] = '\0';

            offset += p_opt_data_curr->option_len;
            rem_len -= p_opt_data_curr->option_len;
        }
    }
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}
#if 0
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|120|27 | 0 | 7 |'e'|'x'|'a'|'m'|'p'|'l'|'e'| 3 |'c'|'o'|'m'| 0 |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
+---+---+---+---+---+---+---+---+---+---+---+---+---+ | 7
|'e'|'x'|'a'|'m'|'p'|'l'|'e'| 3 |'n'|'e'|'t'| 0 | +---+---+---
+---+---+---+---+---+---+---+---+---+---+

Code   Len   enc   Address 1               Address 2
+-----+-----+-----+-----+-----+-----+-----+-----+--
| 120 |  n  |  1  | a1  | a2  | a3  | a4  | a1  |  ...
+-----+-----+-----+-----+-----+-----+-----+-----+--
#endif

/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_populate_sip_server_addresses
 *
 * DESCRIPTION:     This function will populated SIP server addresses into config_data
 *
 * RETURNS:         icf_return_t
 *
*****************************************************************************/
icf_return_t     icf_port_populate_sip_server_addresses(
        icf_dhcp_resp_st       *p_dhcp_resp,
        icf_port_dhcp_opt_data_st   *p_dhcp_opt)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_uint8_t         proxy_cnt = 0;
    icf_uint8_t         rem_len = 0;
    icf_uint8_t         i =0, j =0;
    icf_string_st       temp_str = {0,"\0"};
    icf_uint16_t        dom_len = 0, temp_len = 0, count = 0;
    
    
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb) 
    /*Iterate thru DHCP OPTIONS list*/
    while (ICF_NULL != p_dhcp_opt)
    {
        /*Check if OPTION is SIP_SERVER_DISCOVERY*/
        if (ICF_PORT_DHCP_OPT_SIP_SERVER_DISCOVERY
                == p_dhcp_opt->option_id)
        {
            /*If SIP Server address is received as Domain Name
             * then parse it and populare address in p_dhcp_resp*/
            if (0 == p_dhcp_opt->p_option_str[0])
            {
                rem_len = p_dhcp_opt->option_len;

                while ((1 < rem_len) && (proxy_cnt <  ICF_MAX_SERVERS_FROM_DHCP_RESP))
                {
                    /*start_len is 2 as first two bytes will be enc and length*/
                    icf_port_strcpy(temp_str.str, 
                            &(p_dhcp_opt->p_option_str[p_dhcp_opt->option_len - rem_len + 1]));
                    rem_len -= (icf_port_strlen(temp_str.str) + 1);

                    i = 0;
                    j = 0;
                    dom_len = 0;
                    temp_len = 0;

                    /* SPR 19795: In case no domain name is received but the DHCP query
                     * response was ACK then return failure as proxy address is marked as
                     * domain name but no string value has been received from DHCP server.
                     */ 
                    if('\0' == temp_str.str[0])
                    {
                        ret_val = ICF_FAILURE;
                        break;
                    }

                    while ((i<ICF_MAX_STR_LEN) && ('\0' != temp_str.str[i]) && (proxy_cnt < ICF_MAX_SERVERS_FROM_DHCP_RESP))
                    {
                        dom_len = temp_str.str[i];
                        /* Check that atleast remaining message is of dom_len
                           length */
                        if (dom_len > p_dhcp_opt->option_len)
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        /*  Check for the next chars for domain_length */
                        for (count = 0; count <dom_len; count++)
                        {
                            /*  Check for that the character is an alpha numeric or '-' */
                            if ( ((i + 1) <ICF_MAX_STR_LEN)  && ( j < ICF_MAX_STR_LEN) && (ICF_TRUE == 
                                icf_port_is_valid_domain_name_char(temp_str.str[i+1])))
                             { 
                                 p_dhcp_resp->proxy_list[proxy_cnt].str[j] = temp_str.str[i+1];
                                 j++;
                                 i++;
                                 temp_len++;
                             }
                             /*  If it is not alph numeric then break the loop 
                                 and return failure */
                             else
                             {
                                 ret_val = ICF_FAILURE;
                                 break;
                             }
                        }
                        count = 0;
                        if ( ((i + 1) <ICF_MAX_STR_LEN)  && ( j < ICF_MAX_STR_LEN) && (temp_str.str[i+1]!='\0'))
                        {
                            p_dhcp_resp->proxy_list[proxy_cnt].str[j] = '.';
                            temp_len = 0;
                            j++;
                        }
                        i++;
                        if (ICF_FAILURE == ret_val)
                        {
                            break;
                        }
                    }
                    if (ICF_FAILURE == ret_val)
                    {
                        break;
                    }
                    if ( j < ICF_MAX_STR_LEN) 
					{ 
						p_dhcp_resp->proxy_list[proxy_cnt].str[j] = '\0';
					}
                    p_dhcp_resp->proxy_list[proxy_cnt].str_len =  
                       icf_port_strlen(p_dhcp_resp->proxy_list[proxy_cnt].str);
                    proxy_cnt++;
                    if (ICF_MAX_SERVERS_FROM_DHCP_RESP == proxy_cnt)
                        break;
                }
            }/* End of if */
            /*IP Addresses*/
            else if (1 == p_dhcp_opt->p_option_str[0])
            {
                /*  The options_length must be the (multiple of 4) +1 */
                if (0 != (p_dhcp_opt->option_len - 1)%4)
                {
                    ret_val = ICF_FAILURE;
                    break;
                }
                while ((i < p_dhcp_opt->option_len - 1) && (proxy_cnt < ICF_MAX_SERVERS_FROM_DHCP_RESP))
                {
                    icf_port_sprintf((icf_int8_t *)p_dhcp_resp->proxy_list[proxy_cnt].str,
                            "%u.%u.%u.%u", 
                            p_dhcp_opt->p_option_str[i + 1],
                            p_dhcp_opt->p_option_str[i + 2],
                            p_dhcp_opt->p_option_str[i + 3],
                            p_dhcp_opt->p_option_str[i + 4]);
                    p_dhcp_resp->proxy_list[proxy_cnt].str_len =  
                       icf_port_strlen(p_dhcp_resp->proxy_list[proxy_cnt].str);
                    proxy_cnt++;
                    i += 4;
                    if (ICF_MAX_SERVERS_FROM_DHCP_RESP == proxy_cnt)
                        break;
                }/* End of While */
            } /* End of else if */
            else
            {
                /*Ignore options Invalid OPtions*/
            }
        }/* End of if */
        if (ICF_FAILURE == ret_val)
        {
            break;
        }
        if (ICF_MAX_SERVERS_FROM_DHCP_RESP == proxy_cnt)
            break;
        p_dhcp_opt = p_dhcp_opt->p_next;
    }/* End of While */

    /*If proxy addresses are not received then return failure*/
    if (0 == proxy_cnt)
    {
        ret_val = ICF_FAILURE;
    }
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}

/*  THis feature is now out of scope of 8.0 */
#if 0

/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_populate_dns_server_addresses
 *
 * DESCRIPTION:     This function will populated DNS server addresses into config_data
 *
 * RETURNS:         icf_return_t
 *
*****************************************************************************/
icf_return_t     icf_port_populate_dns_server_addresses(
        icf_dhcp_resp_st       *p_dhcp_resp,
        icf_port_dhcp_opt_data_st   *p_dhcp_opt)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_uint8_t         proxy_cnt = 0;
    icf_uint8_t         i =0;
    
    ICF_FUNCTION_ENTER(p_persistent_glb_pdb)
    /*Iterate thru DHCP OPTIONS list*/
    while (ICF_NULL != p_dhcp_opt)
    {
        /*Check if OPTION is SIP_SERVER_DISCOVERY*/
        if (ICF_PORT_DHCP_OPT_DNS_NS
                == p_dhcp_opt->option_id)
        {
                /*  The length of options must be multiple of 4 */
                if (0 != (p_dhcp_opt->option_len)%4)
                {
                    ret_val = ICF_FAILURE;
                    break;
                }
                while (i < p_dhcp_opt->option_len - 1)
                {
                    icf_port_sprintf(p_dhcp_resp->dns_server_addr[proxy_cnt].str,
                            "%u.%u.%u.%u", 
                            p_dhcp_opt->p_option_str[i + 0],
                            p_dhcp_opt->p_option_str[i + 1],
                            p_dhcp_opt->p_option_str[i + 2],
                            p_dhcp_opt->p_option_str[i + 3]);
                    i += 4;
                    p_dhcp_resp->dns_server_addr[proxy_cnt].str_len = 
                       icf_port_strlen(p_dhcp_resp->dns_server_addr[proxy_cnt].str);
                    proxy_cnt++;
                    if (ICF_MAX_SERVERS_FROM_DHCP_RESP == proxy_cnt)
                        break;
                }
        }
        if (ICF_MAX_SERVERS_FROM_DHCP_RESP == proxy_cnt)
            break;
        p_dhcp_opt = p_dhcp_opt->p_next;
    }
    ICF_FUNCTION_EXIT(p_persistent_glb_pdb)
    return ret_val;
}
#endif

/*****************************************************************************
 *
 *
 * FUNCTION:        icf_port_free_dhcp_options
 *
 * DESCRIPTION:     This function will free parsed DHCP options
 *
 * RETURNS:         icf_void_t
 *
*****************************************************************************/
icf_void_t    icf_port_free_dhcp_options(
        icf_port_dhcp_opt_data_st   *p_dhcp_opt)
{
    icf_port_dhcp_opt_data_st  *p_temp = ICF_NULL;
    icf_error_t                ecode;
    
   
    while (ICF_NULL != p_dhcp_opt)
    {
        p_temp = p_dhcp_opt;
        p_dhcp_opt = p_temp->p_next;

        icf_port_memfree (p_persistent_glb_pdb->p_glb_data->p_glb_port_info, 
                (icf_void_t*)p_temp->p_option_str,
                ICF_MEM_COMMON, 
                &ecode);
        /*  Make the option_str NULL because the memory is already get freed */
        p_temp->p_option_str = ICF_NULL;

        icf_port_memfree (p_persistent_glb_pdb->p_glb_data->p_glb_port_info, 
                (icf_void_t*)p_temp,
                ICF_MEM_COMMON, 
                &ecode);
        /*  Make the p_temp NULL because the memory is already get freed */
        p_temp = ICF_NULL;
    }
    
}




/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_open_broadcast_channel
*
*  DESCRIPTION    :   This function opens the TCP/UDP n/w channel
*                       ->It allocates a fd from system
*                       ->It binds the fd to the port specified
*                       For TCP,it will be the server port
*                       -> THis function will set the socket options
*                       for BROADCAST
*                       Note: This assumes IPv4 only
*
*  RETURNS:         Either ICF_SUCCESS or ICF_FAILURE
*
*************************************************************************/
icf_return_t
icf_port_open_broadcast_channel(icf_uint32_t    port_num,

        /*ICF_TRANSPORT_TYPE_UDP/ICF_TRANSPORT_TYPE_TCP*/
        icf_uint8_t      transport_type,

        /*TRUE->Tk is passing a dotted IP address
         * FALSE->port should use IPADDR_ANY
         */
        icf_boolean_t    is_ipaddr_present,
        icf_uint8_t      *p_ipaddr,

        icf_uint32_t     *p_fd,
        icf_error_t      *p_ecode)
{
       icf_return_t        ret_val = ICF_SUCCESS;
       icf_uint32_t        send_buf_size = ICF_MAX_SEND_BUFF_SIZE;
       icf_uint32_t        recv_buf_size = ICF_MAX_RECV_BUFF_SIZE;

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))|| defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
    icf_int32_t     fd = 0;
    int                 on = 1;
#ifdef ICF_IPV6_ENABLED
    struct sockaddr_storage recv_addr;
    char port[6] = "\0";
    struct addrinfo ai_hints;
    struct addrinfo *p_ai_response = NULL;
#else
    struct sockaddr_in  recv_addr;
#endif
    size_t  remote_addr_len = sizeof(struct sockaddr_in);

#ifndef ICF_IPV6_ENABLED
    if ( ICF_TRUE == is_ipaddr_present)
    {
        recv_addr.sin_addr.s_addr = inet_addr((icf_int8_t *)p_ipaddr);
    }
    else
    {
#ifndef ICF_PORT_WINDOWS
        recv_addr.sin_addr.s_addr = INADDR_ANY;
#else
        recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
    }
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons((icf_uint16_t)port_num);
    memset(&(recv_addr.sin_zero), '\0', 8);
#else
    memset((void *)&ai_hints,0,sizeof(ai_hints));
    ai_hints.ai_flags = AI_PASSIVE;
    itoa(port_num,port,10);
    if(getaddrinfo(p_ipaddr, port, &ai_hints,&p_ai_response) != 0)
    {
        ret_val = ICF_FAILURE;
    }
    if(p_ai_response == NULL)
    {
        ret_val = ICF_FAILURE;
    }
    if(ICF_FALSE == is_ipaddr_present && p_ai_response->ai_next != ICF_NULL)
    {
         memcpy((void *)&recv_addr,(void *)(p_ai_response->ai_next->ai_addr),
                        p_ai_response->ai_next->ai_addrlen);
         remote_addr_len = p_ai_response->ai_next->ai_addrlen;
    }
    else
    {
        memcpy((void *)&recv_addr,(void *)(p_ai_response->ai_addr),
                        p_ai_response->ai_addrlen);
        remote_addr_len = p_ai_response->ai_addrlen;
    }
    freeaddrinfo(p_ai_response);
    p_ai_response = NULL;
#endif

    if ( ICF_TRANSPORT_TYPE_UDP == transport_type)
    {
        fd = socket(
#ifdef ICF_IPV6_ENABLED
                        recv_addr.ss_family,
#else
                        AF_INET,
#endif
                        SOCK_DGRAM, 0);
    }
    if ( 0 > fd)
    {
        *p_ecode = ICF_ERROR_SOCKET_CREATE;
        ret_val = ICF_FAILURE;
    }
    if ( - 1 == setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*)(&on), sizeof(int)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        ret_val = ICF_FAILURE;
    }
    if ( -1 == setsockopt(fd,SOL_SOCKET,SO_BROADCAST, (char*)(&on), sizeof(int)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        ret_val = ICF_FAILURE;
    }
    if ( -1 == bind( fd,(struct sockaddr*)(&recv_addr),
                remote_addr_len))
    {
        *p_ecode = ICF_ERROR_SOCKET_BIND;
        ret_val = ICF_FAILURE;
    }
    if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_SNDBUF, (char *)(&send_buf_size), (socklen_t)sizeof(send_buf_size)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        ret_val = ICF_FAILURE;
    }
    if ( - 1 == setsockopt (fd, SOL_SOCKET, SO_RCVBUF, (char *)(&recv_buf_size), (socklen_t)sizeof(recv_buf_size)))
    {
        *p_ecode = ICF_ERROR_SET_SOCKET;
        ret_val = ICF_FAILURE;
    }

    *p_fd = (icf_uint32_t)fd;
     ICF_PRINT(("\n[PORT]: icf_port_open_channel created fd = %d for mode = %d\n",
                                fd, transport_type));
#endif
    return ret_val;
}


