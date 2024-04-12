#ifndef __RUNNER_WFD_INLINE_H_INCLUDED__
#define __RUNNER_WFD_INLINE_H_INCLUDED__

/*
<:copyright-BRCM:2014:DUAL/GPL:standard 

   Copyright (c) 2014 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:> 
*/

/****************************************************************************/
/******************* Other software units include files *********************/
/****************************************************************************/
#include "rdpa_api.h"
#include "rdpa_mw_blog_parse.h"

/****************************************************************************/
/***************************** Definitions  *********************************/
/****************************************************************************/
/* wlan0 if configuration params */
#define INIT_FILTERS_ARRY_SIZE 5
#define INIT_FILTER_EAP_FILTER_VAL 0x888E

static bdmf_object_handle rdpa_cpu_obj;

static int wifi_prefix_len = 0 ;

#define WFD_WLAN_QUEUE_MAX_SIZE RDPA_CPU_WLAN_QUEUE_MAX_SIZE
#define WFD_NUM_QUEUE_SUPPORTED 2

#if defined(CONFIG_BCM96838)
static int wifi_netdev_event (struct notifier_block *n, unsigned long event, void *v);

static struct notifier_block wifi_netdev_notifer = {
    .notifier_call = wifi_netdev_event ,
};

void replace_upper_layer_packet_destination(void *cb, void *napi_cb) 
{
    send_packet_to_upper_layer = cb;
    send_packet_to_upper_layer_napi = napi_cb;
    inject_to_fastpath = 1;
}

void unreplace_upper_layer_packet_destination(void) 
{
    send_packet_to_upper_layer = netif_rx;
    send_packet_to_upper_layer_napi = netif_receive_skb;
    inject_to_fastpath = 0;
}
#endif

/*****************************************************************************/
/****************** Wlan Accelerator Device implementation *******************/
/*****************************************************************************/

static inline void map_ssid_vector_to_ssid_index (uint16_t *bridge_port_ssid_vector, uint32_t *wifi_drv_ssid_index)
{
   *wifi_drv_ssid_index = __ffs ( * bridge_port_ssid_vector) ;
}

#if defined(CONFIG_BCM96838)
static void rdpa_port_ssid_update(int index, int create)
{
    BDMF_MATTR(rdpa_port_attrs, rdpa_port_drv());
    bdmf_object_handle rdpa_port_obj;
    int rc;

    if (create)
    {
        rdpa_port_index_set(rdpa_port_attrs, rdpa_if_ssid0 + index);
        rc = bdmf_new_and_set(rdpa_port_drv(), NULL, rdpa_port_attrs, &rdpa_port_obj);
        if (rc)
            printk("%s %s Failed to create rdpa port ssid object rc(%d)\n", __FILE__, __FUNCTION__, rc);
    }
    else
    {
        rc = rdpa_port_get(rdpa_if_ssid0 + index, &rdpa_port_obj);
        if (!rc)
        {
            bdmf_put(rdpa_port_obj);
            bdmf_destroy(rdpa_port_obj);
        }
    }
}

static int wifi_netdev_event (struct notifier_block *n, unsigned long event, void *v)
{
    struct net_device *dev = (struct net_device *) v ;
    int ret;
    uint32_t wifi_dev_index ;

    ret = NOTIFY_DONE;

    /*Check for wifi net device*/
    if (!strncmp ( wifi_prefix, dev->name, wifi_prefix_len))
    {
        wifi_dev_index = netdev_path_get_hw_port(dev);

        switch (event)
        {
           case NETDEV_REGISTER:
               if (! wifi_net_devices [wifi_dev_index])
               {
                   wifi_net_devices [wifi_dev_index] = dev;
                   dev_hold (dev) ;
                   rdpa_port_ssid_update(wifi_dev_index, 1);
               }
               ret = NOTIFY_OK ;
               break;
            case NETDEV_UNREGISTER  :
               if (wifi_net_devices [wifi_dev_index])
               {
                   dev_put(wifi_net_devices[wifi_dev_index]);
                   wifi_net_devices [wifi_dev_index] = NULL;
                   rdpa_port_ssid_update(wifi_dev_index, 0);
               }
               ret = NOTIFY_OK;
               break;
        }
    }

    return ret ;
}
#endif


/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_dev_rx_isr_callback.                                             **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   Wlan accelerator - ISR callback                                      **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   ISR callback for the PCI queues handler                              **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static inline void wfd_dev_rx_isr_callback(long qidx)
{
    unsigned long flags;

    /* Disable PCI interrupt */
    rdpa_cpu_int_disable (rdpa_cpu_wlan0, qidx );
    rdpa_cpu_int_clear (rdpa_cpu_wlan0, qidx );

    WFD_IRQ_LOCK(flags); 
    wfd_rx_work_avail |= (1 << qidx);
    WFD_IRQ_UNLOCK(flags);

    /* Call the RDPA receiving packets handler (thread or tasklet) */
    WFD_WAKEUP_RXWORKER;
}

static inline int wfd_config_rx_queue(int qid, uint32_t qsize, 
                                      enumWFD_WlFwdHookType eFwdHookType,
                                      int *numQCreated)
{
    rdpa_cpu_rxq_cfg_t rxq_cfg;
    int rc=0;
    bdmf_sysb_type qsysb_type = bdmf_sysb_skb;
    int qidx = qid - first_pci_queue;
    
    if (eFwdHookType == WFD_WL_FWD_HOOKTYPE_FKB)
    {
        qsysb_type = bdmf_sysb_fkb;
    }

    /* Read current configuration, set new drop threshold and ISR and write back. */
    bdmf_lock();
    rc = rdpa_cpu_rxq_cfg_get(rdpa_cpu_obj, qid, &rxq_cfg);
    if (rc)
        goto unlock_exit;
    rxq_cfg.size = qsize;
    rxq_cfg.isr_priv = qidx;
    rxq_cfg.rx_isr = qsize ? wfd_dev_rx_isr_callback : 0;
    rxq_cfg.sysb_type = qsysb_type;
    rxq_cfg.ic_cfg.ic_enable = qsize ? true : false;
    rxq_cfg.ic_cfg.ic_timeout_us = WFD_INTERRUPT_COALESCING_TIMEOUT_US;
    rxq_cfg.ic_cfg.ic_max_pktcnt = WFD_INTERRUPT_COALESCING_MAX_PKT_CNT;
    rc = rdpa_cpu_rxq_cfg_set(rdpa_cpu_obj, qid, &rxq_cfg);

    if (numQCreated) 
    {
        *numQCreated = 1;
    }
unlock_exit:
    bdmf_unlock();
    return rc;
}

#if defined(CONFIG_BCM96838)
static void release_wfd_interfaces(void)
{
    int wifi_index ;

    for (wifi_index=0; wifi_index<WIFI_MW_MAX_NUM_IF; wifi_index++)
    {
        if (wifi_net_devices[wifi_index])
        {
            rdpa_port_ssid_update(wifi_index, 0);
            dev_put(wifi_net_devices[wifi_index]) ;
            wifi_net_devices [wifi_index] = NULL;
        }
    }

    bdmf_destroy(rdpa_cpu_obj); 

    /*Unregister for NETDEV_REGISTER and NETDEV_UNREGISTER for wifi driver*/
    unregister_netdevice_notifier (&wifi_netdev_notifer) ;
}

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   send_packet_to_bridge                                                **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wlan accelerator - Rx PCI path                                       **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Sends packet to bridge and free skb buffer                           **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static void send_packet_to_bridge(struct sk_buff *skb)
{
    rdpa_cpu_tx_info_t  cpu_tx_info= {};

    /* Send the packet to the RDPA bridge for fast path forwarding */
    cpu_tx_info.port = netdev_path_get_hw_port(skb->dev) + rdpa_if_ssid0;

    rdpa_cpu_send_wfd_to_bridge(skb, &cpu_tx_info);
}
#endif

static inline void wfd_bulk_fwd_mcast(unsigned int rx_pktcnt, 
                                      void **rx_pkts,
                                      wfd_object_t * wfd_p)
{
    uint32_t wifi_drv_if_index;
    uint16_t wifi_ssid_vector;
    struct sk_buff *out_skb = NULL;    
#if defined(PKTC) && defined(CONFIG_BCM_WFD_CHAIN_SUPPORT)
    uint16_t chainIdx = INVALID_WL_METADATA, prevChainIdx = INVALID_WL_METADATA;
    uint32_t brc_hot_ptr = 0;
#endif
    int loopcnt;
    struct sk_buff *skb_p;

    for (loopcnt = 0; loopcnt < rx_pktcnt; loopcnt++)
    {
        skb_p = (struct sk_buff *)rx_pkts[loopcnt];

#if defined(PKTC) && defined(CONFIG_BCM_WFD_CHAIN_SUPPORT)
        chainIdx = skb_p->queue_mapping;
        if (chainIdx != INVALID_WL_METADATA && chainIdx != prevChainIdx)
        {
            if ((brc_hot_ptr = wl_pktc_req_hook(BRC_HOT_GET_BY_IDX, chainIdx, 0, 0)) == 0)
            {
                printk("Invalid ChainIdx %d received\n", chainIdx);
                nbuff_free(skb_p);
                prevChainIdx = INVALID_WL_METADATA;
                continue;
            }
            prevChainIdx = chainIdx;
        }
#endif
        wifi_ssid_vector = (uint16_t)(skb_p->metadata);
        if (!wifi_ssid_vector)
        {
            gs_count_rx_invalid_ssid_vector++;
            nbuff_free(skb_p);
            continue;        
        }
        else
        {
            map_ssid_vector_to_ssid_index(&wifi_ssid_vector, &wifi_drv_if_index);
            if (!wifi_net_devices[wifi_drv_if_index])
            {
                gs_count_rx_no_wifi_interface++;
                printk("%s wifi_net_devices[%d] returned NULL\n", __FUNCTION__, wifi_drv_if_index);
                nbuff_free(skb_p);
                continue;        
            }
            skb_p->protocol = eth_type_trans(skb_p, wifi_net_devices[wifi_drv_if_index]);
            skb_push(skb_p, ETH_HLEN);

            do
            {
                map_ssid_vector_to_ssid_index(&wifi_ssid_vector, &wifi_drv_if_index);

                /*Clear the bit we found*/
                wifi_ssid_vector &= ~(1 << wifi_drv_if_index);

                /*Check if device was initialized */
                if (!wifi_net_devices[wifi_drv_if_index])
                {
                    gs_count_rx_no_wifi_interface++;
                    nbuff_free(skb_p);
                    printk("%s wifi_net_devices[%d] returned NULL\n", __FUNCTION__, wifi_drv_if_index);
                    continue; 
                }

                if (wifi_ssid_vector)
                {
                    /* clone skb */
                    out_skb = skb_clone(skb_p, GFP_ATOMIC);

                    if (!out_skb)
                    {
                        printk("%s %s: Failed to clone skb \n", __FILE__, __FUNCTION__);
                        nbuff_free(skb_p);
                        continue;                     
                    }
                } 
                else
                {
                    out_skb = skb_p;
                }
                gs_count_rx_queue_packets[wifi_drv_if_index]++;

#if defined(PKTC) && defined(CONFIG_BCM_WFD_CHAIN_SUPPORT)
                if (chainIdx != INVALID_WL_METADATA )
                {
                    wfd_p->wfd_fwdHook((uint32_t)skb_p, (uint32_t)brc_hot_ptr, chainIdx);
                    wfd_p->wl_chained_packets++;
                } 
                else
#endif
                {
                    wifi_net_devices[wifi_drv_if_index]->netdev_ops->ndo_start_xmit(out_skb, wifi_net_devices[wifi_drv_if_index]);
                    wfd_p->wl_unchained_packets++;
                }
            }
            while (wifi_ssid_vector);
        }
    }

#if defined(PKTC) && defined(CONFIG_BCM_WFD_CHAIN_SUPPORT)
    if(chainIdx != INVALID_WL_METADATA )
        wfd_p->wfd_completeHook();
#endif
}


static inline uint32_t
wfd_bulk_fkb_get(unsigned long qid, unsigned long budget, wfd_object_t * wfd_p, void **rx_pkts)
{
	unsigned int rx_pktcnt;

	rx_pktcnt = rdpa_cpu_wfd_bulk_fkb_get(qid, budget, rx_pkts, wfd_p->wfd_acc_info_p);

	if (rx_pktcnt) {
		(void) wfd_p->wfd_fwdHook(rx_pktcnt, (uint32_t)rx_pkts, 0);
	}

	return rx_pktcnt;
}

static inline uint32_t
wfd_bulk_skb_get(unsigned long qid, unsigned long budget, wfd_object_t * wfd_p, void **rx_pkts)
{
	unsigned int rx_pktcnt;

	rx_pktcnt = rdpa_cpu_wfd_bulk_skb_get(qid, budget, rx_pkts, wfd_p->wfd_acc_info_p);

	if (rx_pktcnt) 
    {
#if defined(CONFIG_BCM96838)
        wfd_bulk_fwd_mcast(rx_pktcnt, rx_pkts, wfd_p);
#else
		(void) wfd_p->wfd_fwdHook(rx_pktcnt, (uint32_t)rx_pkts, 0);
#endif
	}

	return rx_pktcnt;
}

#if !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)
static int wfd_rdpa_wlan_init_filter(void)
{
	bdmf_object_handle rdpa_filter_obj;
	rdpa_filter filter[INIT_FILTERS_ARRY_SIZE] = {RDPA_FILTER_ETYPE_ARP, RDPA_FILTER_BCAST, RDPA_FILTER_IP_FRAG,
				RDPA_FILTER_IGMP, RDPA_FILTER_ETYPE_UDEF_0};
	rdpa_filter_key_t filter_key = {RDPA_FILTERS_BEGIN, rdpa_if_id(rdpa_if_wlan0)};
	rdpa_filter_ctrl_t entry_params = {1, rdpa_forward_action_host};
	int count;
	bdmf_error_t rc = BDMF_ERR_OK;

	/* Get the filter object */
	rc = rdpa_filter_get(&rdpa_filter_obj);
	if (rc)
	{
		printk("%s %s Failed to get filter object rc(%d)\n", __FILE__, __FUNCTION__, rc);
		goto exit;
	}

	/* Set the EAP etype */
	rc = rdpa_filter_etype_udef_set(rdpa_filter_obj, 0, INIT_FILTER_EAP_FILTER_VAL);
	if (rc)
	{
		printk("%s %s Failed to configure filter etype udef 0 - 0x888E rc(%d)\n", __FILE__, __FUNCTION__, rc);
		goto exit;
	}

	/* Configure the rdpa filters for wlan port */
	for (count = 0; count < INIT_FILTERS_ARRY_SIZE; ++count)
	{
		filter_key.filter = filter[count];
		/* Set the filter entry */
		rc = rdpa_filter_entry_set(rdpa_filter_obj, &filter_key, &entry_params);
		if (rc)
		{
			printk("%s %s Failed to configure rdpa filter entry rc(%d)\n", __FILE__, __FUNCTION__, rc);
			goto exit;
		}
	}

	if(rdpa_filter_obj)
		bdmf_put(rdpa_filter_obj);
	return rc;
exit:
	if(rdpa_filter_obj)
		bdmf_put(rdpa_filter_obj);
	return rc;
}
#endif
/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_accelerator_init                                                 **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi accelerator - init                                              **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function initialize all the runner resources.                    **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wfd_accelerator_init(void)
{
#if defined(CONFIG_BCM96838)
    int error ;
    char wifi_if_name[WIFI_IF_NAME_STR_LEN] ;
    int wifi_dev_index;
#endif
    BDMF_MATTR(cpu_wlan0_attrs, rdpa_cpu_drv());

    wifi_prefix_len = strlen( wifi_prefix ) ;

    /* create cpu */
    rdpa_cpu_index_set(cpu_wlan0_attrs, rdpa_cpu_wlan0);
    if (bdmf_new_and_set(rdpa_cpu_drv(), NULL, cpu_wlan0_attrs, &rdpa_cpu_obj))
    {
        printk("%s %s Failed to create cpu wlan0 object\n",__FILE__, __FUNCTION__);    
        return -1;
    }
#if !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)
    if (wfd_rdpa_wlan_init_filter())
    	return -1;
#endif

    /* Init wifi driver callback */
#if defined(CONFIG_BCM96838)
	replace_upper_layer_packet_destination(send_packet_to_bridge, send_packet_to_bridge);

    for (wifi_dev_index=0; wifi_dev_index<WIFI_MW_MAX_NUM_IF; wifi_dev_index++)
    {
        if (wifi_dev_index % 8 )
            error = (wifi_dev_index > 8) ?
                snprintf(wifi_if_name, WIFI_IF_NAME_STR_LEN, "%s1.%u", wifi_prefix, wifi_dev_index-7) :
                snprintf(wifi_if_name, WIFI_IF_NAME_STR_LEN, "%s0.%u", wifi_prefix, wifi_dev_index) ;
        else
            error = (wifi_dev_index == 0) ?
                snprintf(wifi_if_name, WIFI_IF_NAME_STR_LEN, "%s0", wifi_prefix) :
                snprintf(wifi_if_name, WIFI_IF_NAME_STR_LEN, "%s1", wifi_prefix) ;

        if (error == -1)
        {
            printk("%s %s: wifi interface name retrieval failed \n", __FILE__, __FUNCTION__) ;
            goto error_handling;
        }

        if (!wifi_net_devices[wifi_dev_index])
        {
           wifi_net_devices[wifi_dev_index] = dev_get_by_name(&init_net, wifi_if_name) ;
            if (wifi_net_devices[wifi_dev_index])
                rdpa_port_ssid_update(wifi_dev_index, 1);
        }
    }

    /*Register for NETDEV_REGISTER and NETDEV_UNREGISTER for wifi driver*/
    register_netdevice_notifier(&wifi_netdev_notifer);
#endif

    return 0;

#if defined(CONFIG_BCM96838)
error_handling:
    release_wfd_interfaces ( ) ;
    return -1;
#endif
}

static inline int wfd_queue_not_empty(long qid, int qidx)
{
    return rdpa_cpu_queue_not_empty(rdpa_cpu_wlan0, qid);
}

static inline void wfd_int_enable(long qid, int qidx)
{
    rdpa_cpu_int_enable(rdpa_cpu_wlan0, qidx);
}

static inline void wfd_int_disable(long qid, int qidx)
{
    rdpa_cpu_int_disable(rdpa_cpu_wlan0, qidx);
}

static inline void *wfd_acc_info_get(void)
{
    return rdpa_cpu_data_get(rdpa_cpu_wlan0);
}

static inline int wfd_get_qid(int qidx)
{
    return (first_pci_queue + qidx);
}

static inline int wfd_get_objidx(int qid, int qidx)
{
    return qidx;
}
#endif /* __RUNNER_WFD_INLINE_H_INCLUDED__ */
