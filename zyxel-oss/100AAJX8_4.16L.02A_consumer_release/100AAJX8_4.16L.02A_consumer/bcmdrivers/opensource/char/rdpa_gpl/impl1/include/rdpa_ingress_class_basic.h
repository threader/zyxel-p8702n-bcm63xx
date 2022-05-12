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

#ifndef _RDPA_INGRESS_CLASS_BASIC_H_
#define _RDPA_INGRESS_CLASS_BASIC_H_

/** \addtogroup ingress_class Ingress Classification
 *  Ingress classifier supports up to 16 classifiers per direction
 *
 * @{
 */

 /* Actions of the optional actions vector */
typedef enum{
    /*4 bytes fields*/
    RDPA_IC_SRC_IP          = 0,
    RDPA_IC_DST_IP          = 1,
    RDPA_IC_IPV6_FLOW_LABEL = 2,
    /*2 bytes fields*/
    RDPA_IC_OUTER_TPID      = 3,
    RDPA_IC_INNER_TPID      = 4,
    RDPA_IC_SRC_PORT        = 5,
    RDPA_IC_DST_PORT        = 6,
    RDPA_IC_OUTER_VID       = 7,
    RDPA_IC_INNER_VID       = 8,
    RDPA_IC_DST_MAC         = 9,
    RDPA_IC_SRC_MAC         = 10,
    RDPA_IC_ETHER_TYPE      = 11,
    /*1 byte fields*/
    RDPA_IC_IP_PROTOCOL     = 12,
    RDPA_IC_DSCP            = 13,
    RDPA_IC_SSID            = 14,
    RDPA_IC_INGRESS_PORT    = 15,
    RDPA_IC_OUTER_PBIT      = 16,
    RDPA_IC_INNER_PBIT      = 17,
    RDPA_IC_NUM_OF_VLANS    = 18,
    RDPA_IC_L3_PROTOCOL     = 19 
} rdpa_ic_value ;

/** Ingress classification rule mask fields bitmask */
typedef enum
{
    RDPA_IC_MASK_SRC_IP          = ( 1 << RDPA_IC_SRC_IP ),          /**< Source IP address */
    RDPA_IC_MASK_DST_IP          = ( 1 << RDPA_IC_DST_IP ),          /**< Destination IP address */
    RDPA_IC_MASK_IPV6_FLOW_LABEL = ( 1 << RDPA_IC_IPV6_FLOW_LABEL ), /**< IPv6 Flow Label field */
    RDPA_IC_MASK_OUTER_TPID      = ( 1 << RDPA_IC_OUTER_TPID ),      /**< Outer TPID */
    RDPA_IC_MASK_INNER_TPID      = ( 1 << RDPA_IC_INNER_TPID ),      /**< Inner TPID */
    RDPA_IC_MASK_SRC_PORT        = ( 1 << RDPA_IC_SRC_PORT ),        /**< Source port */
    RDPA_IC_MASK_DST_PORT        = ( 1 << RDPA_IC_DST_PORT ),        /**< Destination port */
    RDPA_IC_MASK_OUTER_VID       = ( 1 << RDPA_IC_OUTER_VID ),       /**< Outer VID */
    RDPA_IC_MASK_INNER_VID       = ( 1 << RDPA_IC_INNER_VID ),       /**< Inner VID */
    RDPA_IC_MASK_DST_MAC         = ( 1 << RDPA_IC_DST_MAC ),         /**< Destination MAC address */
    RDPA_IC_MASK_SRC_MAC         = ( 1 << RDPA_IC_SRC_MAC ),         /**< Source MAC address */
    RDPA_IC_MASK_ETHER_TYPE      = ( 1 << RDPA_IC_ETHER_TYPE ),      /**< Ether Type */
    RDPA_IC_MASK_IP_PROTOCOL     = ( 1 << RDPA_IC_IP_PROTOCOL ),     /**< Protocol */
    RDPA_IC_MASK_DSCP            = ( 1 << RDPA_IC_DSCP ),            /**< IP Differentiated Services Code Point */
    RDPA_IC_MASK_SSID            = ( 1 << RDPA_IC_SSID ),            /**< Ingress SSID */
    RDPA_IC_MASK_INGRESS_PORT    = ( 1 << RDPA_IC_INGRESS_PORT ),    /**< LAN port or GEM/LLID */
    RDPA_IC_MASK_OUTER_PBIT      = ( 1 << RDPA_IC_OUTER_PBIT ),      /**< Outer PBIT */
    RDPA_IC_MASK_INNER_PBIT      = ( 1 << RDPA_IC_INNER_PBIT ),      /**< Inner PBIT */
    RDPA_IC_MASK_NUM_OF_VLANS    = ( 1 << RDPA_IC_NUM_OF_VLANS ),    /**< Number of VLAN in packet */
    RDPA_IC_MASK_L3_PROTOCOL     = ( 1 << RDPA_IC_L3_PROTOCOL ),     /**< L3 Protocol (Other-0, IPv4-1, IPv6-2) */

} rdpa_ic_fields;

/** Ingress classification rule type */
typedef enum
{
    RDPA_IC_TYPE_ACL =1, /**< Classification type ACL */
    RDPA_IC_TYPE_FLOW,   /**< Classification type Flow */
    RDPA_IC_TYPE_QOS,    /**< Classification type QoS */
} rdpa_ic_type;

/** Ingress classification rule type */
typedef enum
{
    RDPA_IC_L3_PROTOCOL_OTHER = 0, /**< IC L3 Protocol field = Other */
    RDPA_IC_L3_PROTOCOL_IPV4  = 1, /**< IC L3 Protocol field = IPv4 */
    RDPA_IC_L3_PROTOCOL_IPV6  = 2  /**< IC L3 Protocol field = IPv6 */
} rdpa_ic_l3_protocol;


/** Ingress classification dei command */
typedef enum 
{
    RDPA_IC_DEI_COPY  = 0,
    RDPA_IC_DEI_CLEAR = 1,
    RDPA_IC_DEI_SET   = 2
} rdpa_ic_dei_command;

/** Ingress classification flow key 
 * This key is used to classify traffic.\n
 */
typedef struct
{
    bdmf_ip_t           src_ip;          /**< source ipv4/ipv6 ip */
    bdmf_ip_t           dst_ip;          /**< dest ipv4/ipv6 ip */
    uint16_t            src_port;        /**< source port */
    uint16_t            dst_port;        /**< destination port */
    uint8_t             protocol;        /**< IP protocols. For example, UDP(17) */
    uint16_t            outer_vid;       /**< Outer VID */
    uint16_t            inner_vid;       /**< Inner VID */
    bdmf_mac_t          dst_mac;         /**<DA  MAC address */
    bdmf_mac_t          src_mac;         /**<SA MAC address */
    uint16_t            etype;           /**< Ethernet type */
    uint8_t             dscp;            /**< dscp val */
    uint8_t             ssid;            /** Wifi ssid */
    bdmf_index          ingress_port;    /**<DS- GEM or LLID index US - ingress port index */
    uint8_t             outer_pbits;     /**< Outer pbit */
    uint8_t             inner_pbits;     /**< Inner PBIT */
    uint8_t             number_of_vlans; /**< number of vlans */
    uint32_t            ipv6_flow_label; /**<  IPv6 Flow Label field */
    uint16_t            outer_tpid;      /**< Outer TPID */
    uint16_t            inner_tpid;      /**< Inner TPID */
    rdpa_ic_l3_protocol l3_protocol;     /**< L3 protocol (other, IPv4, IPv6) */
    
} rdpa_ic_key_t ;

/** @} end of ingress_classification Doxygen group. */
#define RDPA_IC_PHY_DS_PQ_MAX    4
#endif /* _RDPA_INGRESS_CLASS_BASIC_H_ */
