/*
* <:copyright-BRCM:2013:GPL/GPL:standard
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

/*
 * CPU interface
 */
#include <bdmf_interface.h>
#include <rdpa_types.h>

/** Enable CPU queue interrupt */
void (*f_rdpa_cpu_int_enable)(rdpa_cpu_port port, int queue);
EXPORT_SYMBOL(f_rdpa_cpu_int_enable);
void rdpa_cpu_int_enable(rdpa_cpu_port port, int queue)
{
    if (!f_rdpa_cpu_int_enable)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return;
    }
    f_rdpa_cpu_int_enable(port, queue);
}
EXPORT_SYMBOL(rdpa_cpu_int_enable);

/** Disable CPU queue interrupt */
void (*f_rdpa_cpu_int_disable)(rdpa_cpu_port port, int queue);
EXPORT_SYMBOL(f_rdpa_cpu_int_disable);
void rdpa_cpu_int_disable(rdpa_cpu_port port, int queue)
{
    if (!f_rdpa_cpu_int_disable)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return;
    }
    f_rdpa_cpu_int_disable(port, queue);
}
EXPORT_SYMBOL(rdpa_cpu_int_disable);

/** Clear CPU queue interrupt */
void (*f_rdpa_cpu_int_clear)(rdpa_cpu_port port, int queue);
EXPORT_SYMBOL(f_rdpa_cpu_int_clear);
void rdpa_cpu_int_clear(rdpa_cpu_port port, int queue)
{
    if (!f_rdpa_cpu_int_clear)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return;
    }
    f_rdpa_cpu_int_clear(port, queue);
}
EXPORT_SYMBOL(rdpa_cpu_int_clear);

/** Pull a single received packet from host queue. */
int (*f_rdpa_cpu_packet_get)(rdpa_cpu_port port, bdmf_index queue,
    bdmf_sysb *sysb, rdpa_cpu_rx_info_t *info);
EXPORT_SYMBOL(f_rdpa_cpu_packet_get);
int rdpa_cpu_packet_get(rdpa_cpu_port port, bdmf_index queue,
    bdmf_sysb *sysb, rdpa_cpu_rx_info_t *info)
{
    if (!f_rdpa_cpu_packet_get)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_packet_get(port, queue, sysb, info);
}
EXPORT_SYMBOL(rdpa_cpu_packet_get);

/** Get the Time Of Day from the FW FIFO, by the ptp index */
int (*f_rdpa_cpu_ptp_1588_get_tod)(uint16_t ptp_index, uint32_t *tod_h,
    uint32_t *tod_l, uint16_t *local_counter_delta);
EXPORT_SYMBOL(f_rdpa_cpu_ptp_1588_get_tod);
int rdpa_cpu_ptp_1588_get_tod(uint16_t ptp_index, uint32_t *tod_h,
    uint32_t *tod_l, uint16_t *local_counter_delta)
{
    if (!f_rdpa_cpu_ptp_1588_get_tod)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_ptp_1588_get_tod(ptp_index, tod_h, tod_l, local_counter_delta);
}
EXPORT_SYMBOL(rdpa_cpu_ptp_1588_get_tod);

/** similar to rdpa_cpu_send_sysb, but treats only ptp-1588 packets */
int (*f_rdpa_cpu_send_sysb_ptp)(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info);
EXPORT_SYMBOL(f_rdpa_cpu_send_sysb_ptp);
int rdpa_cpu_send_sysb_ptp(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info)
{
    if (!f_rdpa_cpu_send_sysb_ptp)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_send_sysb_ptp(sysb, info);
}
EXPORT_SYMBOL(rdpa_cpu_send_sysb_ptp);

/** Send system buffer */
int (*f_rdpa_cpu_send_sysb)(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info);
EXPORT_SYMBOL(f_rdpa_cpu_send_sysb);
int rdpa_cpu_send_sysb(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info)
{
    if (!f_rdpa_cpu_send_sysb)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_send_sysb(sysb, info);
}
EXPORT_SYMBOL(rdpa_cpu_send_sysb);

/** Send system buffer from WFD*/
int (*f_rdpa_cpu_send_wfd_to_bridge)(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info);
EXPORT_SYMBOL(f_rdpa_cpu_send_wfd_to_bridge);

int rdpa_cpu_send_wfd_to_bridge(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info)
{
    if (unlikely(!f_rdpa_cpu_send_wfd_to_bridge))
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_send_wfd_to_bridge(sysb, info);
}
EXPORT_SYMBOL(rdpa_cpu_send_wfd_to_bridge);

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
/** Send system buffer to WAN */
int (*f_rdpa_cpu_tx_port_enet_wan)(bdmf_sysb sysb, uint32_t egress_queue);
EXPORT_SYMBOL(f_rdpa_cpu_tx_port_enet_wan);
int rdpa_cpu_tx_port_enet_wan(bdmf_sysb sysb, uint32_t egress_queue)
{
    return f_rdpa_cpu_tx_port_enet_wan(sysb, egress_queue);
}
EXPORT_SYMBOL(rdpa_cpu_tx_port_enet_wan);

/** Send system buffer to LAN */
int (*f_rdpa_cpu_tx_port_enet_lan)(bdmf_sysb sysb, uint32_t egress_queue, uint32_t phys_port);
EXPORT_SYMBOL(f_rdpa_cpu_tx_port_enet_lan);
int rdpa_cpu_tx_port_enet_lan(bdmf_sysb sysb, uint32_t egress_queue, uint32_t phys_port)
{
    return f_rdpa_cpu_tx_port_enet_lan(sysb, egress_queue, phys_port);
}
EXPORT_SYMBOL(rdpa_cpu_tx_port_enet_lan);

/** Receive Ethernet system buffer */
int (*f_rdpa_cpu_host_packet_get_enet)(bdmf_index queue, bdmf_sysb *sysb, rdpa_if *src_port);
EXPORT_SYMBOL(f_rdpa_cpu_host_packet_get_enet);
int rdpa_cpu_host_packet_get_enet(bdmf_index queue, bdmf_sysb *sysb, rdpa_if *src_port)
{
    return f_rdpa_cpu_host_packet_get_enet(queue, sysb, src_port);
}
EXPORT_SYMBOL(rdpa_cpu_host_packet_get_enet);

void (*f_rdpa_cpu_tx_reclaim)(void);
EXPORT_SYMBOL(f_rdpa_cpu_tx_reclaim);
void rdpa_cpu_tx_reclaim(void)
{
    f_rdpa_cpu_tx_reclaim();
}
EXPORT_SYMBOL(rdpa_cpu_tx_reclaim);
#endif

/** Receive bulk ethernet system buffers for WFD */
int (*f_rdpa_cpu_wfd_bulk_fkb_get)(bdmf_index queue_id, unsigned int budget, void ** rx_pkts, void * wfd_acc_info_p);
EXPORT_SYMBOL(f_rdpa_cpu_wfd_bulk_fkb_get);
int rdpa_cpu_wfd_bulk_fkb_get(bdmf_index queue_id, unsigned int budget, void ** rx_pkts, void * wfd_acc_info_p)
{
    return f_rdpa_cpu_wfd_bulk_fkb_get(queue_id, budget, rx_pkts, wfd_acc_info_p);
}
EXPORT_SYMBOL(rdpa_cpu_wfd_bulk_fkb_get);

int (*f_rdpa_cpu_wfd_bulk_skb_get)(bdmf_index queue_id, unsigned int budget, void ** rx_pkts, void * wfd_acc_info_p);
EXPORT_SYMBOL(f_rdpa_cpu_wfd_bulk_skb_get);
int rdpa_cpu_wfd_bulk_skb_get(bdmf_index queue_id, unsigned int budget, void ** rx_pkts, void * wfd_acc_info_p)
{
    return f_rdpa_cpu_wfd_bulk_skb_get(queue_id, budget, rx_pkts, wfd_acc_info_p);
}
EXPORT_SYMBOL(rdpa_cpu_wfd_bulk_skb_get);

void *(*f_rdpa_cpu_data_get)(int rdpa_cpu_type);
EXPORT_SYMBOL(f_rdpa_cpu_data_get);
void *rdpa_cpu_data_get(int rdpa_cpu_type)
{
    return f_rdpa_cpu_data_get(rdpa_cpu_type);
}
EXPORT_SYMBOL(rdpa_cpu_data_get);


/** Send raw packet */
int (*f_rdpa_cpu_send_raw)(void *data, uint32_t length, const rdpa_cpu_tx_info_t *info);
EXPORT_SYMBOL(f_rdpa_cpu_send_raw);
int rdpa_cpu_send_raw(void *data, uint32_t length, const rdpa_cpu_tx_info_t *info)
{
    if (!f_rdpa_cpu_send_raw)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_send_raw(data, length, info);
}
EXPORT_SYMBOL(rdpa_cpu_send_raw);

/** Map from HW port to rdpa_if */
rdpa_if (*f_rdpa_port_map_from_hw_port)(int hw_port, bdmf_boolean emac_only);
EXPORT_SYMBOL(f_rdpa_port_map_from_hw_port);
rdpa_if rdpa_port_map_from_hw_port(int hw_port, bdmf_boolean emac_only)
{
    if (!f_rdpa_port_map_from_hw_port)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return rdpa_if_none;
    }
    return f_rdpa_port_map_from_hw_port(hw_port, emac_only);
}
EXPORT_SYMBOL(rdpa_port_map_from_hw_port);


int (*f_rdpa_cpu_queue_not_empty)(rdpa_cpu_port port, bdmf_index queue);
EXPORT_SYMBOL(f_rdpa_cpu_queue_not_empty);
int rdpa_cpu_queue_not_empty(rdpa_cpu_port port, bdmf_index queue)
{
	if (!f_rdpa_cpu_queue_not_empty)
	{
	   BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
	   return BDMF_ERR_STATE;
	}
	return f_rdpa_cpu_queue_not_empty(port,queue);
}
EXPORT_SYMBOL(rdpa_cpu_queue_not_empty);

/** Send EPON Dgasp */
int (*f_rdpa_cpu_send_epon_dying_gasp)(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info);
EXPORT_SYMBOL(f_rdpa_cpu_send_epon_dying_gasp);
int rdpa_cpu_send_epon_dying_gasp(bdmf_sysb sysb, const rdpa_cpu_tx_info_t *info)
{
    if (!f_rdpa_cpu_send_epon_dying_gasp)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_send_epon_dying_gasp(sysb, info);
}
EXPORT_SYMBOL(rdpa_cpu_send_epon_dying_gasp);

int (*f_rdpa_cpu_is_per_port_metering_supported)(rdpa_cpu_reason reason);
EXPORT_SYMBOL(f_rdpa_cpu_is_per_port_metering_supported);
int rdpa_cpu_is_per_port_metering_supported(rdpa_cpu_reason reason)
{
    if (!f_rdpa_cpu_is_per_port_metering_supported)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_cpu_is_per_port_metering_supported(reason);
}
EXPORT_SYMBOL(rdpa_cpu_is_per_port_metering_supported);

rdpa_ports (*f_rdpa_ports_all_lan)(void);
EXPORT_SYMBOL(f_rdpa_ports_all_lan);
rdpa_ports rdpa_ports_all_lan()
{
    if (!f_rdpa_ports_all_lan)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return BDMF_ERR_STATE;
    }
    return f_rdpa_ports_all_lan();
}
EXPORT_SYMBOL(rdpa_ports_all_lan);

/** Run time mapping from HW port to rdpa_if using array */
rdpa_if (*f_rdpa_physical_port_to_rdpa_if)(rdpa_physical_port port);
EXPORT_SYMBOL(f_rdpa_physical_port_to_rdpa_if);
rdpa_if rdpa_physical_port_to_rdpa_if(rdpa_physical_port port)
{
    if (!f_rdpa_physical_port_to_rdpa_if)
    {
        BDMF_TRACE_ERR("rdpa.ko is not loaded\n");
        return rdpa_if_none;
    }
    return f_rdpa_physical_port_to_rdpa_if(port);
}
EXPORT_SYMBOL(rdpa_physical_port_to_rdpa_if);

int (*f_rdpa_egress_tm_queue_id_by_lan_port_queue)(rdpa_if port, int queue, uint32_t *queue_id);
EXPORT_SYMBOL (f_rdpa_egress_tm_queue_id_by_lan_port_queue);

int rdpa_egress_tm_queue_id_by_lan_port_queue(rdpa_if port, int queue, uint32_t *queue_id)
{
   if (!f_rdpa_egress_tm_queue_id_by_lan_port_queue)
       return BDMF_ERR_STATE;
   return f_rdpa_egress_tm_queue_id_by_lan_port_queue(port, queue, queue_id);
}
EXPORT_SYMBOL(rdpa_egress_tm_queue_id_by_lan_port_queue);


int (*f_rdpa_egress_tm_queue_id_by_wan_flow_index)(int *wan_flow, int ind, uint32_t *queue_id);
EXPORT_SYMBOL (f_rdpa_egress_tm_queue_id_by_wan_flow_index);

int rdpa_egress_tm_queue_id_by_wan_flow_index(int *wan_flow, int ind, uint32_t *queue_id)
{
	if (!f_rdpa_egress_tm_queue_id_by_wan_flow_index)
       return BDMF_ERR_STATE;
	return f_rdpa_egress_tm_queue_id_by_wan_flow_index(wan_flow, ind, queue_id);
}
EXPORT_SYMBOL (rdpa_egress_tm_queue_id_by_wan_flow_index);


int (*f_rdpa_egress_tm_cfg_queue_threshold_by_tcont_queue )(int portId,int priority,int minThreshold,int maxThreshold);
EXPORT_SYMBOL (f_rdpa_egress_tm_cfg_queue_threshold_by_tcont_queue);

int rdpa_egress_tm_cfg_queue_threshold_by_tcont_queue(int portId, int priority, int minThreshold, int maxThreshold)
{
	if (!f_rdpa_egress_tm_cfg_queue_threshold_by_tcont_queue)
       return BDMF_ERR_STATE;
	return f_rdpa_egress_tm_cfg_queue_threshold_by_tcont_queue(portId, priority, minThreshold, maxThreshold);
}
EXPORT_SYMBOL (rdpa_egress_tm_cfg_queue_threshold_by_tcont_queue);

int (*f_rdpa_egress_tm_cfg_queue_threshold_by_lan_port_queue)(int portId,int priority,int minThreshold,int maxThreshold);
EXPORT_SYMBOL (f_rdpa_egress_tm_cfg_queue_threshold_by_lan_port_queue);

int rdpa_egress_tm_cfg_queue_threshold_by_lan_port_queue(int portId, int priority, int minThreshold, int maxThreshold)
{
	if (!f_rdpa_egress_tm_cfg_queue_threshold_by_lan_port_queue)
       return BDMF_ERR_STATE;
	return f_rdpa_egress_tm_cfg_queue_threshold_by_lan_port_queue(portId, priority, minThreshold, maxThreshold);
}
EXPORT_SYMBOL (rdpa_egress_tm_cfg_queue_threshold_by_lan_port_queue);


