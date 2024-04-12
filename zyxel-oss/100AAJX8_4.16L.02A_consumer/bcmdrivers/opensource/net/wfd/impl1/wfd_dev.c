/*
* <:copyright-BRCM:2012:DUAL/GPL:standard
* 
*    Copyright (c) 2012 Broadcom Corporation
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

/****************************************************************************/
/**                                                                        **/
/** Software unit Wlan accelerator dev                                     **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   Wlan accelerator interface.                                          **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Mediation layer between the wifi / PCI interface and the Accelerator **/
/**  (Runner/FAP)                                                          **/
/**                                                                        **/
/** Allocated requirements:                                                **/
/**                                                                        **/
/** Allocated resources:                                                   **/
/**                                                                        **/
/**   A thread.                                                            **/
/**   An interrupt.                                                        **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/


/****************************************************************************/
/******************** Operating system include files ************************/
/****************************************************************************/
#include <linux/types.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <net/route.h>
#include <linux/moduleparam.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/etherdevice.h>
#include <linux/kthread.h>
#include "wfd_dev.h"

#if defined(PKTC) && defined(CONFIG_BCM_WFD_CHAIN_SUPPORT)
#include <linux_osl_dslcpe_pktc.h>
#include <linux/bcm_skb_defines.h>
#endif

/****************************************************************************/
/***************************** Module Version *******************************/
/****************************************************************************/
static const char *version = "Wifi Forwarding Driver";

#define WFD_MAX_OBJECTS   2
#define WFD_QUEUE_TO_WFD_IDX_MASK 0x1
#define WIFI_MW_MAX_NUM_IF    ( 16 )
#define WIFI_IF_NAME_STR_LEN  ( IFNAMSIZ )
#define WLAN_CHAINID_OFFSET 8
#define WFD_INTERRUPT_COALESCING_TIMEOUT_US 500
#define WFD_INTERRUPT_COALESCING_MAX_PKT_CNT 32

/****************************************************************************/
/*********************** Multiple SSID FUNCTIONALITY ************************/
/****************************************************************************/
static struct net_device *wifi_net_devices[WIFI_MW_MAX_NUM_IF]={NULL, };

static struct proc_dir_entry *proc_directory, *proc_file_conf;

static struct task_struct *wfd_rx_thread;
static wait_queue_head_t   wfd_rx_thread_wqh;
static int                 wfd_rx_work_avail=0;
static int                 wfd_queue_mask=0;
extern void replace_upper_layer_packet_destination( void * cb, void * napi_cb );
extern void unreplace_upper_layer_packet_destination( void );

static int wfd_tasklet_handler(void  *context);

/****************************************************************************/
/***************************** Module parameters*****************************/
/****************************************************************************/
/* Initial maximum queue size */
static int packet_threshold = 0;
module_param (packet_threshold, int, 0);
/* Number of packets to read in each tasklet iteration */
#define NUM_PACKETS_TO_READ_MAX 128
static int num_packets_to_read = NUM_PACKETS_TO_READ_MAX;
module_param (num_packets_to_read, int, 0);
/* Initial number of configured PCI queues */
static int number_of_queues = 1;
module_param (number_of_queues, int, 0);
/* first Cpu ring queue - Currently pci CPU ring queues must be sequentioal */
static int first_pci_queue = 8;
module_param (first_pci_queue,int,0);
/* wifi Broadcom prefix */
static char wifi_prefix [WIFI_IF_NAME_STR_LEN] = "wl";
module_param_string (wifi_prefix, wifi_prefix, WIFI_IF_NAME_STR_LEN, 0);

/* Counters */
static unsigned int gs_count_rx_no_wifi_interface = 0 ;
static unsigned int gs_count_rx_invalid_ssid_vector = 0 ;
static unsigned int gs_count_rx_queue_packets [WIFI_MW_MAX_NUM_IF] = {0, } ;
static unsigned int gs_count_tx_packets [WIFI_MW_MAX_NUM_IF] = {0, } ;

typedef struct
{
    struct net_device *wl_dev_p;
    enumWFD_WlFwdHookType eFwdHookType; 
    bool isTxChainingReqd;
	bool isSkbQ;
    HOOK3PARM wfd_fwdHook;
    HOOKV wfd_completeHook;
	unsigned int wl_chained_packets;
	unsigned int wl_unchained_packets;
	void * wfd_acc_info_p;
} wfd_object_t;

static wfd_object_t wfd_objects[WFD_MAX_OBJECTS];
static int wfd_idx=0;
static spinlock_t wfd_irq_lock;

#define WFD_IRQ_LOCK(flags) spin_lock_irqsave(&wfd_irq_lock, flags)
#define WFD_IRQ_UNLOCK(flags) spin_unlock_irqrestore(&wfd_irq_lock, flags)

#define WFD_WAKEUP_RXWORKER do { \
            wake_up_interruptible(&wfd_rx_thread_wqh); \
          } while (0)

int (*send_packet_to_upper_layer)(struct sk_buff *skb) = netif_rx;
EXPORT_SYMBOL(send_packet_to_upper_layer); 
int (*send_packet_to_upper_layer_napi)(struct sk_buff *skb) = netif_receive_skb;
EXPORT_SYMBOL(send_packet_to_upper_layer_napi);
int inject_to_fastpath = 0;
EXPORT_SYMBOL(inject_to_fastpath);

static void wfd_dump(void);

/****************************************************************************/
/******************* Other software units include files *********************/
/****************************************************************************/
#if (defined(CONFIG_BCM_RDPA)||defined(CONFIG_BCM_RDPA_MODULE)) 
#include "runner_wfd_inline.h"
void (*wfd_dump_fn)(void) = 0;
#elif (defined(CONFIG_BCM_FAP) || defined(CONFIG_BCM_FAP_MODULE))
#include "fap_wfd_inline.h"
extern void (*wfd_dump_fn)(void);
#endif

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_tasklet_handler.                                                 **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wlan accelerator - tasklet handler                                   **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Reads all the packets from the Rx queue and send it to the wifi      **/
/**   interface.                                                           **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wfd_tasklet_handler(void *context)
{
    int rx_pktcnt = 0;
    int qid, qidx = 0;
	wfd_object_t * wfd_p;
    unsigned long flags;
    void *rx_pkts[NUM_PACKETS_TO_READ_MAX];

    while (1)
    {
        wait_event_interruptible(wfd_rx_thread_wqh, wfd_rx_work_avail || kthread_should_stop());
        if (kthread_should_stop())
        {
            printk(KERN_INFO "kthread_should_stop detected in wfd\n");
            break;
        }

        for (qidx = 0; qidx < number_of_queues; qidx++)
        {
            qid = wfd_get_qid(qidx);
            wfd_p = &wfd_objects[wfd_get_objidx(qid, qidx)];

            local_bh_disable();

            if (wfd_p->isSkbQ) 
            {
                rx_pktcnt = wfd_bulk_skb_get(qid, num_packets_to_read, wfd_p, rx_pkts);
            }
            else
            {
                /* Reads all configured PCI queues in strict priority */
                rx_pktcnt = wfd_bulk_fkb_get(qid, num_packets_to_read, wfd_p, rx_pkts);
            }

            local_bh_enable();

            if (wfd_queue_not_empty(qid, qidx))
            {
                schedule();
            } 
            else
            {
                /* Queue is empty: no more packets, enable interrupts */
                WFD_IRQ_LOCK(flags);
                wfd_rx_work_avail &= (~(1 << qidx));
                WFD_IRQ_UNLOCK(flags);
                wfd_int_enable(qid, qidx);
            }
        } /*for pci queue*/

#if 0
        WFD_IRQ_LOCK(flags);
        if (wfd_rx_work_avail == 0)
        {
            for (qidx = 0; qidx < number_of_queues; qidx++)
            {
               qid = wfd_get_qid(qidx);

               //Enable interrupts
               wfd_int_enable(qid);
            }
        }
#endif
    }

    return 0;
}

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wifi_mw_proc_read_func_conf                                          **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi mw - proc read                                                  **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Procfs callback method.                                              **/
/**      Called when someone reads proc command                            **/
/**   using: cat /proc/wifi_mw                                             **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/**   page  -  Buffer where we should write                                **/
/**   start -  Never used in the kernel.                                   **/
/**   off   -  Where we should start to write                              **/
/**   count -  How many character we could write.                          **/
/**   eof   -  Used to signal the end of file.                             **/
/**   data  -  Only used if we have defined our own buffer                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wifi_mw_proc_read_func_conf(char* page, char** start, off_t off, int count, int* eof, void* data)
{
    int wifi_index ;
    unsigned int count_rx_queue_packets_total=0 ;
    unsigned int count_tx_bridge_packets_total=0 ;
    int len = 0 ;

    page+=off;
    page[0]=0;

    for(wifi_index=0;wifi_index < WIFI_MW_MAX_NUM_IF;wifi_index++)
    {
        if( wifi_net_devices[wifi_index] != NULL )
        {
            len += sprintf((page+len),"WFD Registered Interface %d:%s\n",
                            wifi_index,wifi_net_devices[wifi_index]->name);
        }
    }

    /*RX-MW from WiFi queues*/
    for (wifi_index=0; wifi_index<WIFI_MW_MAX_NUM_IF; wifi_index++)
    {
        if (gs_count_rx_queue_packets[wifi_index]!=0)
        {
            count_rx_queue_packets_total += gs_count_rx_queue_packets[wifi_index] ;
            if (wifi_index==0)
            {
                len += sprintf((page+len), "RX-MW from WiFi queues      [WiFi %d] = %d\n", 
                    wifi_index, gs_count_rx_queue_packets[wifi_index]) ;
            }
            else
            {
                len += sprintf((page +len), "                            [WiFi %d] = %d\n", 
                    wifi_index, gs_count_rx_queue_packets[wifi_index]) ;
            }

        }

    }

    /*TX-MW to bridge*/
    for (wifi_index=0; wifi_index<WIFI_MW_MAX_NUM_IF; wifi_index++)
    {
        if ( gs_count_tx_packets[wifi_index]!=0)
        {
            count_tx_bridge_packets_total += gs_count_tx_packets[wifi_index] ;
            if (wifi_index == 0)
            {
                len += sprintf((page+len), "TX-MW to bridge             [WiFi %d] = %d\n", 
                    wifi_index, gs_count_tx_packets[wifi_index]) ;
            }
            else
            {
                len += sprintf((page+len ), "                            [WiFi %d] = %d\n",      
                    wifi_index, gs_count_tx_packets[wifi_index]) ;
            }
        }
    }

    for (wifi_index = 0 ; wifi_index < WFD_MAX_OBJECTS ;wifi_index++ )
    {
    	if (wfd_objects[wifi_index].wl_dev_p)
    	{
    		len += sprintf((page+len),"\nWFD Object %d",wifi_index);
    		len += sprintf((page+len), "\nwl_chained_counters		=%d", wfd_objects[wifi_index].wl_chained_packets) ;
    		len += sprintf((page+len), "\nwl_unchained_counters		=%d", wfd_objects[wifi_index].wl_unchained_packets) ;
    	}

    }

    len += sprintf((page+len), "\nRX-MW from WiFi queues      [SUM] = %d\n", count_rx_queue_packets_total) ;
    len += sprintf((page+len), "TX-MW to bridge             [SUM] = %d\n", count_tx_bridge_packets_total) ;
    len += sprintf((page+len), "No WIFI interface                 = %d\n", gs_count_rx_no_wifi_interface) ;
    len += sprintf((page+len), "Invalid SSID vector               = %d\n", gs_count_rx_invalid_ssid_vector) ;


    memset(gs_count_rx_queue_packets, 0, sizeof(gs_count_rx_queue_packets));
    memset(gs_count_tx_packets, 0, sizeof(gs_count_tx_packets));

    gs_count_rx_no_wifi_interface = 0 ;
    gs_count_rx_invalid_ssid_vector = 0 ;

    *eof = 1;
    return len;
}


/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wifi_proc_init.                                                      **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi mw - proc init                                                  **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function initialize the proc entry                               **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wifi_proc_init(void)
{
    /* make a directory in /proc */
    proc_directory = proc_mkdir("wfd", NULL) ;

    if (proc_directory==NULL) goto fail_dir ;

    /* make conf file */
    proc_file_conf = create_proc_entry("stats", 0444, proc_directory) ;

    if (proc_file_conf==NULL ) goto fail_entry ;

    /* set callback function on file */
    proc_file_conf->read_proc = wifi_mw_proc_read_func_conf ;

    return (0) ;

fail_entry:
    printk("%s %s: Failed to create proc entry in wifi_mw\n", __FILE__, __FUNCTION__);
    remove_proc_entry("wfd" ,NULL); /* remove already registered directory */

fail_dir:
    printk("%s %s: Failed to create directory wifi_mw\n", __FILE__, __FUNCTION__) ;
    return (-1) ;
}


/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   create_wfd_os_resources                                              **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi mw - Init                                                       **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function initializes the OS resources                            **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**   bool - Error code:                                                   **/
/**             true - No error                                            **/
/**             false - Error                                              **/
/**                                                                        **/
/****************************************************************************/
static int create_wfd_os_resources(void)
{
    /* Initialize the tasklet according to the module parameter */
    wfd_rx_work_avail = 0;
    init_waitqueue_head(&wfd_rx_thread_wqh);
    wfd_rx_thread = kthread_create(wfd_tasklet_handler, NULL, "wfd");
  //  kthread_bind(rx_thread, 0);
    wake_up_process(wfd_rx_thread);

    return (0);
}

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   release_wfd_os_resources                                             **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function releases the OS resources                               **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**   bool - Error code:                                                   **/
/**             true - No error                                            **/
/**             false - Error                                              **/
/**                                                                        **/
/****************************************************************************/
static int release_wfd_os_resources(void)
{
    /* stop kernel thread */
    kthread_stop(wfd_rx_thread);
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
    bdmf_destroy(rdpa_cpu_obj);
#endif
    return (0) ;
}

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   init_wifi_drv_resources.                                             **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function initializes the driver resources                        **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**   int -  Error code:                                                   **/
/**             true - No error                                            **/
/**             false - Error                                              **/
/**                                                                        **/
/****************************************************************************/
static int init_wifi_drv_resources(void)
{
    int rc = 0 ;

    /* Initialize all the drivers OS dependant resources */
    rc = create_wfd_os_resources() ;

    if (rc)
    {
        printk("%s %s: Failed to create WiFi mw os resources, status %d\n",   
            __FILE__, __FUNCTION__, rc);
    }


    return (rc);

}


/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_dev_close                                                        **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi accelerator - close                                             **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function closes all the driver resources.                        **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static void wfd_dev_close(void)
{
    int qid, qidx;

    /* Disable the interrupt */
    for (qidx = 0; qidx < number_of_queues; qidx++)
    {
        qid = wfd_get_qid(qidx);

        /* Deconfigure PCI RX queue */
        wfd_config_rx_queue(qid, 0, WFD_WL_FWD_HOOKTYPE_SKB, 0);
      
	/* interrupt mask only */
        wfd_int_disable (qid, qidx); 
    }
     
    /* Release the OS driver resources */
    release_wfd_os_resources();

    remove_proc_entry("stats", proc_directory);

    remove_proc_entry("wfd", NULL);

#if defined(CONFIG_BCM96838)
    /*Free PCI resources*/
    release_wfd_interfaces();
#endif
}


/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_dev_init                                                         **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/**   wifi accelerator - init                                              **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   The function initialize all the driver resources.                    **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
static int wfd_dev_init(void)
{
	if (num_packets_to_read > NUM_PACKETS_TO_READ_MAX)
	{
        printk("%s %s Invalid num_packets_to_read %d\n",__FILE__, __FUNCTION__, num_packets_to_read);    
        return -1;
	}

    spin_lock_init(&wfd_irq_lock);

    /* Initialize the proc interface for debugging information */
    if (wifi_proc_init()!=0)
    {
        printk("\n%s %s: wifi_proc_init() failed \n", __FILE__, __FUNCTION__) ;
        goto proc_release;
    }

    /* Queues will be initialized during wfd_bind() */
    number_of_queues = 0;

    /* Initialize the WFD */
    if (init_wifi_drv_resources()!=0)
    {
        printk("%s %s: init_wifi_drv_resources() failed \n", __FILE__, __FUNCTION__) ;
        goto proc_release;
    }

    /* Initialize accelerator(Runner/FAP) specific data structures, Queues */
    if (wfd_accelerator_init() != 0)
    {
        printk("%s %s: wfd_platform_init() failed \n", __FILE__, __FUNCTION__) ;
        goto proc_release;    
    }

    wfd_dump_fn = wfd_dump;

    printk("\033[1m\033[34m%s is initialized!\033[0m\n", version);
        
    return 0;

proc_release:
    remove_proc_entry("stats", proc_directory);
    remove_proc_entry("wfd", NULL);

    return -1;
}

/****************************************************************************/
/**                                                                        **/
/** Name:                                                                  **/
/**                                                                        **/
/**   wfd_bind                                                             **/
/**                                                                        **/
/** Title:                                                                 **/
/**                                                                        **/
/** Abstract:                                                              **/
/**                                                                        **/
/**   Bind the function hooks and other attributes that are needed by      **/
/**   wfd to forward packets to WLAN.                                      **/
/**                                                                        **/
/** Input:                                                                 **/
/**                                                                        **/
/** Output:                                                                **/
/**                                                                        **/
/**                                                                        **/
/****************************************************************************/
int wfd_bind(struct net_device *wl_dev_p, 
             enumWFD_WlFwdHookType eFwdHookType, 
             bool isTxChainingReqd,
             HOOK3PARM wfd_fwdHook, 
             HOOKV wfd_completeHook)
{
    int rc=0;
    int qid;
    int numQCreated=0;
    int qidx = number_of_queues;

    if (wfd_idx >= WFD_MAX_OBJECTS)
    {
        printk("%s ERROR. WFD_MAX_OBJECTS(%d) limit reached\n", __FUNCTION__, WFD_MAX_OBJECTS);
        return rc;
    }

    memset(&wfd_objects[wfd_idx], 0, sizeof(wfd_objects[wfd_idx]));

    wfd_objects[wfd_idx].wl_dev_p         = wl_dev_p;
    wfd_objects[wfd_idx].eFwdHookType     = eFwdHookType;
    wfd_objects[wfd_idx].isTxChainingReqd = isTxChainingReqd;
    wfd_objects[wfd_idx].isSkbQ           =
		(wfd_objects[wfd_idx].eFwdHookType == WFD_WL_FWD_HOOKTYPE_SKB);
    wfd_objects[wfd_idx].wfd_fwdHook      = wfd_fwdHook;
    wfd_objects[wfd_idx].wfd_completeHook = wfd_completeHook;
	wfd_objects[wfd_idx].wl_chained_packets = 0;
	wfd_objects[wfd_idx].wl_unchained_packets = 0;
	wfd_objects[wfd_idx].wfd_acc_info_p  = wfd_acc_info_get();

    wfd_idx++;

    packet_threshold = WFD_WLAN_QUEUE_MAX_SIZE;

    /* Configure WFD RX queue */
    if (qidx < WFD_NUM_QUEUE_SUPPORTED)
    {
        qid = wfd_get_qid(qidx);

        if ((rc = wfd_config_rx_queue(qid, 
                                      packet_threshold,
                                      eFwdHookType,
                                      &numQCreated)) != 0)
        {
            printk("%s %s: Cannot configure WFD CPU Rx queue (%d), status (%d)\n",
               __FILE__, __FUNCTION__, qid, rc);
            return rc;
        }

        wfd_int_enable (qid, qidx); 

        for (qidx = number_of_queues; qidx < (number_of_queues + numQCreated); qidx++)
        {
            wfd_queue_mask |= (1 << qidx);
        }

        number_of_queues = number_of_queues + numQCreated;

        printk("\033[1m\033[34m %s: Dev %s wfd_idx %d Type %s configured WFD CPU "
               "RxQId (%d), status (%d) number_of_queues %d qmask 0x%x\033[0m\n",
               __FUNCTION__, wl_dev_p->name, (wfd_idx-1), 
               ((eFwdHookType == WFD_WL_FWD_HOOKTYPE_SKB) ? "skb" : "fkb"), 
               qid, rc, number_of_queues, wfd_queue_mask);
    }
    else
    {
        printk("%s: ERROR qidx %d numq %d maxq %d\n", __FUNCTION__, 
               (int)qidx, (int)number_of_queues, (int)WFD_NUM_QUEUE_SUPPORTED);
    }

    return (wfd_idx - 1);
}
EXPORT_SYMBOL(wfd_bind);

void wfd_unbind(int idx, enumWFD_WlFwdHookType hook_type)
{
    int qidx, qid, n = 0;
    int numQCreated;

    // simple reclaim iff idx of last bind
    if (idx != wfd_idx - 1)
    {
        printk("%s idx %d wfd_idx %d\n", __func__, idx, wfd_idx);
        return;
    }

    /* free the pci rx queue(s); disable the interrupt(s) */
    do {
        /* Deconfigure PCI RX queue(s) */
        qidx = --number_of_queues;
        qid = wfd_get_qid(qidx);
        wfd_config_rx_queue(qid, 0, hook_type, &numQCreated);
        wfd_queue_mask &= ~(1 << qidx);
        wfd_int_disable(qid, qidx);
	// loop if numQCreated > 1
    } while (++n < numQCreated);

    wfd_idx = idx;
    memset(&wfd_objects[idx], 0, sizeof wfd_objects[idx]);
}
EXPORT_SYMBOL(wfd_unbind);

static void wfd_dump(void)
{
    unsigned long flags;

    WFD_IRQ_LOCK(flags);
    printk("wfd_rx_work_avail 0x%x qmask 0x%x number_of_queues %d\n", wfd_rx_work_avail, wfd_queue_mask, number_of_queues);
    WFD_IRQ_UNLOCK(flags);
}

MODULE_DESCRIPTION("WLAN Forwarding Driver");
MODULE_AUTHOR("Broadcom");
MODULE_LICENSE("GPL");

module_init(wfd_dev_init);
module_exit(wfd_dev_close);
