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

#ifndef _RDPA_UCAST_H_
#define _RDPA_UCAST_H_

#include <bdmf_interface.h>
#include "rdpa_ip_class_basic.h"
#include "rdpa_cpu.h"
#include "rdpa_egress_tm.h"
#include "rdpa_cmd_list.h"

/** \defgroup ip_class IP Flow Classification
 * IP flows are used for fast TCP/UDP routing and 5-tuple-based bridging.\n
 * The classifier identifies IP flows using 5-tuple key\n
 * { src_ip, dst_ip, protocol, src_port, dst_port }.\n
 * @{
 */

#define RDPA_UCAST_MAX_FLOWS 16512

#define RDPA_UCAST_IP_ADDRESSES_TABLE_SIZE           4
#define RDPA_UCAST_IP_ADDRESSES_TABLE_INDEX_INVALID  RDPA_UCAST_IP_ADDRESSES_TABLE_SIZE

#define RDPA_UCAST_MAX_DS_WAN_UDP_FILTERS            32

/** 5-tuple based IP flow classifaction result.\n
 * Each result determines L2/3/4 header manipulation, forwarding decision and QoS mapping information.\n
 */
typedef struct {
    rdpa_if egress_if;                                     /**< RDPA Egress Interface */
    uint32_t queue_id;                                     /**< Egress queue id */
    uint8_t wan_flow;                                      /**< DSL ATM/PTM US channel */
    uint8_t is_routed;                                     /**< 1: Routed Flow; 0: Bridged Flow */
    uint8_t drop;                                          /**< 1: Drop packets; 0: Forward packets */
    uint8_t ip_addresses_table_index;                      /**< IP Addresses Table index assigned to flow */
    uint16_t mtu;                                          /**< Egress Port MTU */
    union {
        uint16_t wl_metadata;                              /**< WL metadata */
        struct {
#ifndef _BYTE_ORDER_LITTLE_ENDIAN_
                uint16_t reserved:4;
                uint16_t wlan_tx_priority:4;               /**< wlan tx priority */
                uint16_t wlan_chain_id:8;                  /**< wlan chain id */
#else
                uint16_t wlan_chain_id:8;                  /**< wlan chain id */
                uint16_t wlan_tx_priority:4;               /**< wlan tx priority */
                uint16_t reserved:4;
#endif
        };
    };
    uint8_t wfd_queue;                                     /**< WFD Module queue */
    uint8_t cmd_list_length;                               /**< Command List Length, in bytes */
    uint16_t cmd_list[RDPA_CMD_LIST_UCAST_LIST_SIZE_16];   /**< Command List */
} rdpa_ip_flow_result_t;

/** 5-tuple based IP flow classifaction info (key + result).\n
 */
typedef struct {
    rdpa_ip_flow_key_t key;          /**< 5-tuple based IP flow key */
    rdpa_ip_flow_result_t result;    /**< 5-tuple based IP flow result */
} rdpa_ip_flow_info_t;

/** IP SA/DA Address Table.\n
 */
typedef struct
{
    bdmf_ip_t src_addr;
    bdmf_ip_t dst_addr;
    uint16_t reference_count;
    uint16_t sram_address;
} rdpa_ip_addresses_table_t;

/** IP SA/DA Address Table.\n
 */
typedef struct
{
    uint32_t offset;
    uint32_t value;
    uint32_t mask;
    uint32_t hits;
} rdpa_ds_wan_udp_filter_t;

/** @} end of ip_class Doxygen group. */

#endif /* _RDPA_UCAST_H_ */
