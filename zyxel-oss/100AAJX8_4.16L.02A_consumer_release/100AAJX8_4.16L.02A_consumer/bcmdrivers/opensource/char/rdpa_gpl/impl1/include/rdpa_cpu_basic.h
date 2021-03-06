/*
* <:copyright-BRCM:2013:DUAL/GPL:standard
* 
*    Copyright (c) 2013 Broadcom Corporation
*    All Rights Reserved
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License, version 2, as published by
* the Free Software Foundation (the "GPL").
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* 
* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
* 
* :> 
*/


#ifndef RDPA_CPU_BASIC_H_
#define RDPA_CPU_BASIC_H_

/** \addtogroup cpu CPU Interface
 *
 * @{
 */

/** CPU trap reasons */
typedef enum
{
    rdpa_cpu_reason_min = 0,

    rdpa_cpu_rx_reason_oam                 = 0, /**< OAM packet */
    rdpa_cpu_rx_reason_omci                = 1, /**< OMCI packet */
    rdpa_cpu_rx_reason_flow                = 2,
    rdpa_cpu_rx_reason_mcast               = 3, /**< Multicat packet */
    rdpa_cpu_rx_reason_bcast               = 4, /**< Broadcast packet */
    rdpa_cpu_rx_reason_igmp                = 5, /**< Igmp packet */
    rdpa_cpu_rx_reason_icmpv6              = 6, /**< Icmpv6 packet */
    rdpa_cpu_rx_reason_mac_trap_0          = 7,
    rdpa_cpu_rx_reason_mac_trap_1          = 8,
    rdpa_cpu_rx_reason_mac_trap_2          = 9,
    rdpa_cpu_rx_reason_mac_trap_3          = 10,
    rdpa_cpu_rx_reason_dhcp                = 11, /**< DHCP packet */
    rdpa_cpu_rx_reason_local_ip            = 13,
    rdpa_cpu_rx_reason_hdr_err             = 14, /**< Packet with IP header error */
    rdpa_cpu_rx_reason_sa_moved            = 15, /**< SA move indication*/
    rdpa_cpu_rx_reason_unknown_sa          = 16, /**< Unknown SA indication */
    rdpa_cpu_rx_reason_unknown_da          = 17, /**< Unknown DA indication */
    rdpa_cpu_rx_reason_ip_frag             = 18, /**< Packet is fragmented */
    rdpa_cpu_rx_reason_direct_queue_0      = 20, /* */
    rdpa_cpu_rx_reason_direct_queue_1      = 21, /* */
    rdpa_cpu_rx_reason_direct_queue_2      = 22, /* */
    rdpa_cpu_rx_reason_direct_queue_3      = 23, /* */
    rdpa_cpu_rx_reason_direct_queue_4      = 24, /* */
    rdpa_cpu_rx_reason_direct_queue_5      = 25, /* */
    rdpa_cpu_rx_reason_direct_queue_6      = 26, /* */
    rdpa_cpu_rx_reason_direct_queue_7      = 27, /* */
    rdpa_cpu_rx_reason_etype_udef_0        = 28, /**< User defined ethertype 1 */
    rdpa_cpu_rx_reason_etype_udef_1        = 29, /**< User defined ethertype 2 */
    rdpa_cpu_rx_reason_etype_udef_2        = 30, /**< User defined ethertype 3 */
    rdpa_cpu_rx_reason_etype_udef_3        = 31, /**< User defined ethertype 4 */
    rdpa_cpu_rx_reason_etype_pppoe_d       = 32, /**< PPPoE Discovery */
    rdpa_cpu_rx_reason_etype_pppoe_s       = 33, /**< PPPoE Source */
    rdpa_cpu_rx_reason_etype_arp           = 34, /**< Packet with ethertype Arp */
    rdpa_cpu_rx_reason_etype_ptp_1588      = 35, /**< Packet with ethertype 1588 */
    rdpa_cpu_rx_reason_etype_802_1x        = 36, /**< Packet with ethertype 802_1x */
    rdpa_cpu_rx_reason_etype_801_1ag_cfm   = 37, /**< Packet with ethertype v801 Lag CFG*/
    rdpa_cpu_rx_reason_non_tcp_udp         = 40, /**< Packet is non TCP or UDP */
    rdpa_cpu_rx_reason_ip_flow_miss        = 41, /**< Flow miss indication */
    rdpa_cpu_rx_reason_tcp_flags           = 42, /**< TCP flag indication */
    rdpa_cpu_rx_reason_ttl_expired         = 43, /**< TTL expired indication */
    rdpa_cpu_rx_reason_mtu_exceeded        = 44, /**< MTU exceeded indication */
    rdpa_cpu_rx_reason_l4_icmp             = 45, /**< layer-4 ICMP protocol */
    rdpa_cpu_rx_reason_l4_esp              = 46, /**< layer-4 ESP protocol */
    rdpa_cpu_rx_reason_l4_gre              = 47, /**< layer-4 GRE protocol */
    rdpa_cpu_rx_reason_l4_ah               = 48, /**< layer-4 AH protocol */
    rdpa_cpu_rx_reason_l4_ipv6             = 50, /**< layer-4 IPV6 protocol */
    rdpa_cpu_rx_reason_l4_udef_0           = 51, /**< User defined layer-4 1 */
    rdpa_cpu_rx_reason_l4_udef_1           = 52, /**< User defined layer-4 2 */
    rdpa_cpu_rx_reason_l4_udef_2           = 53, /**< User defined layer-4 3 */
    rdpa_cpu_rx_reason_l4_udef_3           = 54, /**< User defined layer-4 4 */
    rdpa_cpu_rx_reason_firewall_match      = 55, /* */
    rdpa_cpu_rx_reason_connection_trap_0   = 56, /**< User defined connection 1 */
    rdpa_cpu_rx_reason_connection_trap_1   = 57, /**< User defined connection 2 */
    rdpa_cpu_rx_reason_connection_trap_2   = 58, /**< User defined connection 3 */
    rdpa_cpu_rx_reason_connection_trap_3   = 59, /**< User defined connection 4 */
    rdpa_cpu_rx_reason_connection_trap_4   = 60, /**< User defined connection 5 */
    rdpa_cpu_rx_reason_connection_trap_5   = 61, /**< User defined connection 6 */
    rdpa_cpu_rx_reason_connection_trap_6   = 62, /**< User defined connection 7 */
    rdpa_cpu_rx_reason_connection_trap_7   = 63, /**< User defined connection 8 */

    rdpa_cpu_reason__num_of
} rdpa_cpu_reason;

/** @} end of cpu Doxygen group */

#endif /* RDPA_CPU_BASIC_H_ */
