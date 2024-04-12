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

#ifndef _RDPA_INGRESS_CLASS_H_
#define _RDPA_INGRESS_CLASS_H_

#include "bdmf_interface.h"
#include "rdpa_egress_tm.h"
#include "rdpa_ingress_class_basic.h"

/** \defgroup ingress_class Ingress Classification
 *  Ingress classifier supports up to 16 classifiers per direction
 *
 * @{
 */
#define RDPA_IC_MAX_PRIORITY 63

/** ACL classifier mode */
typedef enum{
    RDPA_ACL_MODE_BLACK, /**< Black list, matched packets are dropped */
    RDPA_ACL_MODE_WHITE, /**< White list, unmatched packets are dropped */
    RDPA_ACL_MODE_NONE
} rdpa_acl_mode;

/** Classifier configuration */
typedef struct{
    rdpa_ic_type type; /**<Classification type - ACL/flow/QoS. */
    uint32_t field_mask; /**< Fields used for classification. A combination of rdpa_ic_fields. */
    uint32_t prty;  /**< Defines the priority of classifier inside rdpa_ic_value . value between 0 - 64. */
    rdpa_acl_mode acl_mode; /**< Black/White list. relevant only for cfg type acl. */
    rdpa_ports port_mask; /**< LAN ports mask, - reserved for future use. */
} rdpa_ic_cfg_t ;

/** Vector of \ref rdpa_ic_action_vector_t "actions". All configured actions are applied on the flow entry */
typedef uint16_t rdpa_ic_action_vec_t;

/** Ingress classification flow result */
typedef struct
{
    rdpa_qos_method qos_method; /**< QoS classification method flow / pbit */
    uint8_t wan_flow;  /**< WAN flow : Gem Flow or LLID */
    rdpa_forward_action  action; /**< forward/drop/cpu */
    bdmf_object_handle policer; /**< Policer object */
    rdpa_forwarding_mode forw_mode;  /**< flow/pkt based */
    rdpa_if egress_port; /**< Egress port */
    uint32_t queue_id; /**< Egress queue ID */
    bdmf_object_handle vlan_action; /**< VLAN action, can be overwritten by VLAN action per egress port */
    bdmf_boolean opbit_remark; /**< Enable outer pbit remark */
    rdpa_pbit opbit_val;	/**< Outer PBIT remark value */
    bdmf_boolean ipbit_remark; /**< Enable inner PBIT remark */
    rdpa_pbit ipbit_val; /**< Inner PBIT remark value */
    bdmf_boolean dscp_remark; /**< Enable DSCP remark */
    rdpa_dscp dscp_val; /**< DSCP remark value */
    uint8_t ecn_val;
#if !defined(DSL_63138) && !defined(DSL_63148)
    bdmf_object_handle pbit_to_gem_table; /**< pBit to GEM Mapping table handle. Set null for flow based mapping */
#endif
    rdpa_ic_action_vec_t action_vec; /**< Enabled action to be performed on the matched flow. */
    bdmf_index service_q_id; /**< Service queue id, for none: service_q_id=-1 */
    rdpa_ic_dei_command dei_command; /**< Set the DEI bits modification **/
} rdpa_ic_result_t ;

/** Ingress classification info (key + result).\n
 */
typedef struct 
{
    rdpa_ic_key_t  key; /**< Ingress classification key */
    rdpa_ic_result_t  result; /**< Ingress classification result */
} rdpa_ic_info_t ;

/** Ingress classification per port key */
typedef struct
{
    bdmf_index flow; /**< Index of flow to add VLAN action to */
    rdpa_if port;  /**< Egress port */
} rdpa_port_action_key_t;

/** Ingress classification per port action */
typedef struct
{
    bdmf_object_handle vlan_action; /**< VLAN action object */
    bdmf_boolean drop; /**< Drop action - true/false */
} rdpa_port_action_t;

/** Actions of the optional actions vector */
typedef enum
{
	rdpa_ic_act_none, /**< action none */
	rdpa_ic_act_service_q, /**< action service queue */
}
rdpa_ic_action;

/** Bitmask of actions */
typedef enum
{
    /** Forward through service queue */
	rdpa_ic_action_service_q = (1 << rdpa_ic_act_service_q), /**< service queue bitmask*/
}
rdpa_ic_action_vector_t;

/** @} end of ingress_classification Doxygen group. */

#endif /* _RDPA_INGRESS_CLASS_H_ */
