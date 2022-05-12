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

#ifndef _RDPA_IP_FLOW_BASIC_H_
#define _RDPA_IP_FLOW_BASIC_H_

#include <bdmf_data_types.h>

/** \addtogroup ip_class
 * @{
 */


/* Actions of the optional actions vector */
typedef enum
{
    rdpa_fc_act_forward, 
    rdpa_fc_act_reserved, 
    rdpa_fc_act_ttl,
    rdpa_fc_act_policer,
    rdpa_fc_act_dscp_remark,
    rdpa_fc_act_nat,
    rdpa_fc_act_gre_remark,
    rdpa_fc_act_opbit_remark,
    rdpa_fc_act_ipbit_remark,
    rdpa_fc_act_tunnel,
    rdpa_fc_act_pppoe,
    rdpa_fc_act_service_q,
}
rdpa_fc_action;

/** Bitmask of actions applied on 5-tuple based IP flow entry */ 
typedef enum
{
    /** Disables forwarding action if set */
    rdpa_fc_action_forward = (1 << rdpa_fc_act_forward),         
    /** Reserved for future use */
    rdpa_fc_action_reserved = (1 << rdpa_fc_act_reserved),  
    /** Enables ttl decrement if set */
    rdpa_fc_action_ttl = (1 << rdpa_fc_act_ttl),
    /** Enables flow based policer if set */
    rdpa_fc_action_policer = (1 << rdpa_fc_act_policer),
    /** Enables DSCP remarking if set */
    rdpa_fc_action_dscp_remark = (1 << rdpa_fc_act_dscp_remark),
    /** Enables NAT operation if set */
    rdpa_fc_action_nat = (1 << rdpa_fc_act_nat),
    /** Enables GRE remarking if set */
    rdpa_fc_action_gre_remark = (1 << rdpa_fc_act_gre_remark),
    /** Enables Outer pbit remarking if set */
    rdpa_fc_action_opbit_remark = (1 << rdpa_fc_act_opbit_remark),
    /** Enables Inner pbit remarking if set */
    rdpa_fc_action_ipbit_remark = (1 << rdpa_fc_act_ipbit_remark),
    /** Enables DS Lite operation if set */
    rdpa_fc_action_tunnel = (1 << rdpa_fc_act_tunnel),
    /** Enables pppoe operation if set */
    rdpa_fc_action_pppoe = (1 << rdpa_fc_act_pppoe),
    /** Forward to service queue */
    rdpa_fc_action_service_q = (1 << rdpa_fc_act_service_q)
}
rdpa_fc_action_vector;

/** Vector of \ref rdpa_fc_action_vector "actions". All configured actions are applied on the 5-tuple based IP flow entry */
typedef uint16_t rdpa_fc_action_vec_t;

/** 5-tuple based IP flow key.\n
 * This key is used to classify traffic.\n
 */
typedef struct {
    bdmf_ip_t src_ip;    /**< Source IP address, in GRE mode should be 0 */
    bdmf_ip_t dst_ip;    /**< Destination IP address, in GRE mode should be call ID*/
    uint8_t prot;        /**< Protocol */
    uint16_t src_port;   /**< Source port */
    uint16_t dst_port;   /**< Destination port */
    rdpa_traffic_dir dir;/**< Traffic direction */
} rdpa_ip_flow_key_t;

/** @} end of ip_class Doxygen group. */

#endif

