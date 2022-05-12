/*************************************************************************
 *
 *     FILENAME           :  icf_port_dhcp_info.h
 *
 *     DESCRIPTION        :  This file holds the macros related to the 
 *                           DHCP
 *
 *     Revision History   :
 *     DATE        NAME                REFERENCE     REASON
 *     ----------------------------------------------------------
 *     22-JAN-2008 Anurag Khare     ICF LLD    Initial Version.
 *
 *     Copyright 2008, Aricent.
 *
 *************************************************************************/

#ifndef __ICF_PORT_DHCP_INFO_H__
#define __ICF_PORT_DHCP_INFO_H__


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

/*  Following is DHCP message types            */

#define  ICF_PORT_DHCP_DISCOVER                      1
#define  ICF_PORT_DHCP_OFFER                         2
#define  ICF_PORT_DHCP_REQUEST                       3
#define  ICF_PORT_DHCP_DECLINE                       4
#define  ICF_PORT_DHCP_ACK                           5
#define  ICF_PORT_DHCP_NACK                          6
#define  ICF_PORT_DHCP_RELEASE                       7
#define  ICF_PORT_DHCP_INFORM                        8
#define  ICF_PORT_DHCP_BOOTP_PKT                     9

/*    Following is DHCP broadcast mask         */
#define  ICF_PORT_DHCP_BROADCAST_MASK            0x8000

/*    Following are DHCP request type          */
#define  ICF_PORT_DHCP_BOOTREQUEST                   1
#define  ICF_PORT_DHCP_BOOTREPLY                     2

/*    Following are DHCP options for which IPTK sent query */
#define ICF_PORT_DHCP_OPTION_SIP_SERVER_QUERY      0x01
#define ICF_PORT_DHCP_OPTION_DNS_SERVER_QUERY      0x02


/*    Following are DHCP option types          */
#define  ICF_PORT_DHCP_OPT_PAD                        0
#define  ICF_PORT_DHCP_OPT_SUBNET_MASK                1
#define  ICF_PORT_DHCP_OPT_ROUTER_OPTION              3
#define  ICF_PORT_DHCP_OPT_DNS_NS                     6
#define  ICF_PORT_DHCP_OPT_DNS_NAME                   15
#define  ICF_PORT_DHCP_OPT_REQUESTED_IP               50
#define  ICF_PORT_DHCP_OPT_LEASE_TIME                 51
#define  ICF_PORT_DHCP_OPT_OVERLOAD                   52
#define  ICF_PORT_DHCP_OPT_MSG_TYPE                   53
#define  ICF_PORT_DHCP_OPT_SERVER_ID                  54
#define  ICF_PORT_DHCP_OPT_PARAMETER_LIST             55
#define  ICF_PORT_DHCP_OPT_CLIENT_ID                  61
#define  ICF_PORT_DHCP_OPT_MAX_MESSAGE_SIZE           57
#define  ICF_PORT_DHCP_OPT_TFTP_SNAME                 66
#define  ICF_PORT_DHCP_OPT_BOOT_FILE_NAME             67
#define  ICF_PORT_DHCP_OPT_USER_CLASS                 77
#define  ICF_PORT_DHCP_OPT_SIP_SERVER_DISCOVERY       120
#define  ICF_PORT_DHCP_OPT_END                        255

#define  ICF_PORT_DHCP_MAX_OPT_LEN                    255

#define  ICF_PORT_ENET_ADDR_LEN                       6

#define  ICF_PORT_DHCP_SERVER_NAME_LEN                6
#define  ICF_PORT_DHCP_FILE_NAME_LEN                  6

#define  ICF_PORT_HWTYPE_ETHERNET                     1

#define  ICF_PORT_DHCP_LEN_MAGIC                      4

#define  ICF_PORT_MAX_OUT_MSG_SIZE                    500 

#define  ICF_PORT_MAX_IFS                             64

#define ICF_PORT_MIN_DHCP_MSG (236 + ICF_PORT_DHCP_LEN_MAGIC + 3)

#define ICF_PORT_MIN_DHCP_OPTION_LEN                   2

/*  Max Ethernet address length possible */
#define ICF_PORT_CLIENT_HARDWARE_ADDRESS_LEN           18

/*  Max File name length      */
#define ICF_PORT_MAX_FILE_NAME_LEN                     128

/*  Max SI name length           */
#define ICF_PORT_MAX_SI_NAME_LEN                       64 

/* --------------------------------------------------------------------
 * Type Name   :  icf_port_dhcp_hdr_st 
 * Description :  Following structure defines the DHCP header information
 *                to be send to DHCP server and also DHCP response received
 *                from DHCP server. This includes all DHCP parameters
 *                mentioned in RFC-2131 DHCP header
 * ------------------------------------------------------------------------*/
typedef struct 
{
    /* 1 = BOOTREQUEST, 2 = BOOTREPLY*/
    icf_uint8_t  Op;           
    /* Hardware address type*/
    icf_uint8_t  htype;
    /* Hardware address length.*/
    icf_uint8_t  hlen;         
    icf_uint8_t  hops;
    /* Transaction ID - random number*/
    icf_uint32_t  xid;          
    icf_uint16_t  secs;
    icf_uint16_t  flags;
    /* Client IP address*/
    icf_uint32_t  ciaddr;       
    /* 'your' (client) IP address*/
    icf_uint32_t  yiaddr;       
    icf_uint32_t  siaddr;
    /* Relay agent IP address*/
    icf_uint32_t  giaddr;       
    /* client hardware address*/
    icf_uint8_t  chaddr[ICF_PORT_CLIENT_HARDWARE_ADDRESS_LEN];   
    icf_uint8_t  sname[ICF_PORT_MAX_SI_NAME_LEN];
    icf_uint8_t  file[ICF_PORT_MAX_FILE_NAME_LEN];
    /* options - does not includes magic cookie*/
    icf_uint8_t  *pOptions;   
} icf_port_dhcp_hdr_st;



/* --------------------------------------------------------------------
 * Type Name   :  icf_port_dhcp_pkt_info_st
 * Description :  Following structure defines the DHCP request message
 *                to be send. This includes the DHCP header to be send
 *                to DHCP server and DHCP server address
 * ------------------------------------------------------------------------*/
typedef struct 
{
    /* dhcp message header and options*/
    icf_port_dhcp_hdr_st      dhcp_hdr;
    /* Interface number for the dhcp packet*/
    icf_uint32_t           u4IfIndex; 
    /* destination Ip-Address of the packet*/
    icf_uint32_t           u4DstIpAddr;
    /* length of the options in the dhcp message */
    icf_uint16_t           u2Len;   
    /* length of the dhcp message*/ 
    icf_uint16_t           u2BufLen; 
    /* Max DHCP Message size*/
    icf_uint16_t           u2MaxMessageSize;
    /*DHCP overload option*/
    icf_uint8_t           u1OverLoad; 
    /* DHCP Client Message type*/
    icf_uint8_t              u1Type;
} icf_port_dhcp_pkt_info_st;



/* --------------------------------------------------------------------
 * Type Name   :  icf_port_dhcp_client_cfg_st
 * Description :  Following structure defines the DHCP client options 
 *                sent to DHCP server and time when the query is sent
 * ------------------------------------------------------------------------*/
typedef struct
{
    /* Start of the lease time*/
    icf_uint32_t   u4StartOfLeaseTime;
    /* Length of the options*/
    icf_uint8_t     u2LenOfOptions;
    /* Default options pointer*/
    icf_uint8_t     *pDefaultOptions;
}icf_port_dhcp_client_cfg_st;



/* --------------------------------------------------------------------
 * Type Name   :  icf_port_dhcp_client_node_st 
 * Description :  Following structure defines the DHCP client context
 *                information for a DHCPINFORM message
 * ------------------------------------------------------------------------*/
typedef struct 
{
    /* Last transmitted Xid */
    icf_uint32_t        u4Xid;  
    /* Network address acquisition initialisation time */
    icf_uint32_t        u4Secs; 
    icf_uint32_t        u4IpAddressOfferred;/* IP Address Leased by server */
    /* Client config. Info */
    icf_port_dhcp_client_cfg_st DhcpClientConfInfo;
    /* Interface state */
    icf_uint8_t        u1State;        
}icf_port_dhcp_client_node_st;



/* --------------------------------------------------------------------
 * Type Name   :  icf_port_dhcp_opt_data_st
 * Description :  Following structure defines the DHCP options data received
 *                from DHCP server in DHCPACK message
 * ------------------------------------------------------------------------*/
typedef struct icf_port_dhcp_opt_data_st
{
    /* Start of the lease time*/
    icf_uint32_t   option_id;
    /* Length of the options*/
    icf_uint8_t     option_len;
    /* Default options pointer*/
    icf_uint8_t     *p_option_str; 

    struct icf_port_dhcp_opt_data_st *p_next;
}icf_port_dhcp_opt_data_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_PORT_DHCP_INFO_H__ */
