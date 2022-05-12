/*
   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2013:DUAL/GPL:standard
    
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

/******************************************************************************/
/*                                                                            */
/* File Description:                                                          */
/*                                                                            */
/* This file contains the implementation of the Runner CPU ring interface     */
/*                                                                            */
/******************************************************************************/

#ifndef _RDP_CPU_RING_INLINE_H_
#define _RDP_CPU_RING_INLINE_H_

#include "rdp_cpu_ring.h"
#if defined(__KERNEL__)
#include "linux/prefetch.h"
#include "linux/bcm_skb_defines.h"
#include "wfd_dev.h"
#endif

#define D_WLAN_CHAINID_OFFSET	8
#if defined(__KERNEL__)

static inline void* rdp_databuf_alloc(RING_DESCTIPTOR *pDescriptor)
{
   if (likely(pDescriptor->buff_cache_cnt))
   {
      return (void *)(pDescriptor->buff_cache[--pDescriptor->buff_cache_cnt]);
   } 
   else
   {
      uint32_t alloc_cnt;
      /* refill the local cache from global pool */
      alloc_cnt = bdmf_sysb_databuf_alloc(pDescriptor->buff_cache, MAX_BUFS_IN_CACHE, 0);
      if (alloc_cnt)
      {
         pDescriptor->buff_cache_cnt = alloc_cnt;
         return (void *)(pDescriptor->buff_cache[--pDescriptor->buff_cache_cnt]);
      }
   }
   return NULL;
}

static inline void rdp_databuf_free(void *pBuf, uint32_t context)
{
   bdmf_sysb_databuf_free(pBuf, context);
}

#elif defined(_CFE_)

static inline void* rdp_databuf_alloc(RING_DESCTIPTOR *pDescriptor)
{
   void *pBuf = KMALLOC(BCM_PKTBUF_SIZE, 16);

   if (pBuf)
   {
      INV_RANGE(pBuf, BCM_PKTBUF_SIZE);
      return pBuf;
   }
   return NULL;
}

static inline void rdp_databuf_free(void *pBuf, uint32_t context)
{
   KFREE(pBuf);
}

#endif


#if defined(__KERNEL__) || defined(_CFE_)
extern RING_DESCTIPTOR host_ring[D_NUM_OF_RING_DESCRIPTORS];

static inline void AssignPacketBuffertoRing(RING_DESCTIPTOR *pDescriptor, volatile RING_DESC_UNION *pTravel, void *pBuf)
{
   /* assign the buffer address to ring and set the ownership to runner
    * by clearing  bit 31 which is used as ownership flag */

   pTravel->cpu_rx.word2 = swap4bytes(((VIRT_TO_PHYS(pBuf)) & 0x7fffffff));

   /* advance the head ptr, wrap around if needed*/
   if (++pDescriptor->head == pDescriptor->end) 
      pDescriptor->head = pDescriptor->base;
}

#if defined(CONFIG_BCM963138) || defined(_BCM963138_) || defined(CONFIG_BCM963148) || defined(_BCM963148_)
static inline int ReadPacketFromRing(RING_DESCTIPTOR *pDescriptor, volatile RING_DESC_UNION *pTravel, CPU_RX_PARAMS *rxParams)
{
   /* pTravel is in uncached mem so reading 32bits at a time into
      cached mem improves performance*/
   CPU_RX_DESCRIPTOR	    rxDesc;

   rxDesc.word2 = pTravel->cpu_rx.word2;
   //printk("ReadPacketFromRing bufaddr= %x\n", rxDesc.word2);
   rxDesc.word2 = swap4bytes(rxDesc.word2);

   //printk("ReadPacketFromRing swapped bufaddr= %x\n", rxDesc.word2);
   if ((rxDesc.ownership == OWNERSHIP_HOST))
   {
      rxDesc.ownership = 0; /*clear the ownership bit */
      rxParams->data_ptr = (uint8_t *)PHYS_TO_CACHED(rxDesc.word2);

      rxDesc.word0 = pTravel->cpu_rx.word0;
      rxDesc.word0 = swap4bytes(rxDesc.word0);

      rxParams->packet_size = rxDesc.packet_length;
      rxParams->src_bridge_port = (BL_LILAC_RDD_BRIDGE_PORT_DTE)rxDesc.source_port;
      rxParams->flow_id = rxDesc.flow_id;

#if defined(__KERNEL__)
      cache_invalidate_len_outer_first(rxParams->data_ptr, rxParams->packet_size);
#endif

      rxDesc.word1 = pTravel->cpu_rx.word1;
      rxDesc.word1 = swap4bytes(rxDesc.word1);

      rxParams->reason = (rdpa_cpu_reason)rxDesc.reason;
      rxParams->dst_ssid = rxDesc.dst_ssid;

      rxDesc.word3 = pTravel->cpu_rx.word3;
      rxDesc.word3 = swap4bytes(rxDesc.word3);

      rxParams->wl_metadata = rxDesc.wl_metadata;
      rxParams->ptp_index = pTravel->cpu_rx.ip_sync_1588_idx;
#if defined(_CFE_)
      rxParams->wl_metadata = 0;
#endif

      return 0;
   }

   return BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_EMPTY;
}
static inline int ReadPacketFromRing2(RING_DESCTIPTOR *pDescriptor, volatile RING_DESC_UNION *pTravel, CPU_RX_DESCRIPTOR *rxDesc)
{
   rxDesc->word2 = pTravel->cpu_rx.word2;

   /* Process word2 */
   rxDesc->word2 = swap4bytes(rxDesc->word2);

   if ((rxDesc->ownership == OWNERSHIP_HOST))
   {
      rxDesc->word0 = pTravel->cpu_rx.word0;

      rxDesc->ownership = 0; /*clear the ownership bit */
      rxDesc->data_ptr = (uint8_t *)PHYS_TO_CACHED(rxDesc->word2);
      rxDesc->word0 = swap4bytes(rxDesc->word0);

#if defined(__KERNEL__)
#if 0   /* Get cpu rx reason from rxDesc.word1 */
      rxDesc->word1  = swap4bytes(pTravel->cpu_rx.word1);
#else
      /* Hard coded the reason to inscrease performance. */
      rxDesc->reason = rdpa_cpu_rx_reason_ip_flow_miss;
#endif

      cache_invalidate_len_outer_first(rxDesc->data_ptr, rxDesc->packet_length);
#if defined(CONFIG_BCM963138)
      /* Prefetch the first 64 bytes of the packet */
      bcm_prefetch(rxDesc->data_ptr, 2);
#else // CONFIG_BCM963148
      /* Prefetch the first 64 bytes of the packet */
      bcm_prefetch(rxDesc->data_ptr, 1);
#endif
#endif /* __KERNEL__ */

      return 0;
   }

   return BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_EMPTY;
}
#else
static inline int ReadPacketFromRing(RING_DESCTIPTOR *pDescriptor, volatile RING_DESC_UNION *pTravel, CPU_RX_PARAMS *rxParams)
{
   /* pTravel is in uncached mem so reading 32bits at a time into
      cached mem improves performance*/
   CPU_RX_DESCRIPTOR	    rxDesc;

   rxDesc.word2 = pTravel->cpu_rx.word2;
   if ((rxDesc.ownership == OWNERSHIP_HOST))
   {
      rxParams->data_ptr = (uint8_t *)PHYS_TO_CACHED(rxDesc.word2);

      rxDesc.word0 = pTravel->cpu_rx.word0;
      rxParams->packet_size = rxDesc.packet_length;
      rxParams->src_bridge_port = (BL_LILAC_RDD_BRIDGE_PORT_DTE)rxDesc.source_port;
      rxParams->flow_id = rxDesc.flow_id;

        rxDesc.word1 = pTravel->cpu_rx.word1 ;
        rxParams->reason = (rdpa_cpu_reason)rxDesc.reason;
        rxParams->dst_ssid = rxDesc.dst_ssid;
        rxDesc.word3 = pTravel->cpu_rx.word3 ;
    	rxParams->wl_metadata = rxDesc.wl_metadata;
        rxParams->ptp_index = pTravel->cpu_rx.ip_sync_1588_idx;

      return 0;
   }

   return BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_EMPTY;
}
#endif /* defined(CONFIG_BCM963138) || defined(_BCM963138_) || defined(CONFIG_BCM963148) || defined(_BCM963148_) */

#ifndef _CFE_
/*this API get the pointer of the next available packet and reallocate buffer in ring
 * in the descriptor is optimized to 16 bytes cache line, 6838 has 16 bytes cache line
 * while 68500 has 32 bytes cache line, so we don't prefetch the descriptor to cache
 * Also on ARM platform we are not sure of how to skip L2 cache, and use only L1 cache
 * so for now  always use uncached accesses to Packet Descriptor(pTravel)
 */

static inline int rdp_cpu_ring_read_packet_refill(uint32_t ring_id, CPU_RX_PARAMS *rxParams)
{
   uint32_t 					ret;
   RING_DESCTIPTOR *pDescriptor		= &host_ring[ring_id];
   RING_DESC_UNION *pTravel 		= pDescriptor->head;
   void *pNewBuf;


#ifdef __KERNEL__
   if (unlikely(pDescriptor->admin_status == 0)) 
      return BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_EMPTY;
#endif

   ret = ReadPacketFromRing(pDescriptor, pTravel, rxParams);
   if (ret)
   {
      return  ret;
   }

   /* A valid packet is recieved try to allocate a new data buffer and
    * refill the ring before giving the packet to upper layers
    */

   pNewBuf	= rdp_databuf_alloc(pDescriptor);

   /*validate allocation*/
   if (unlikely(!pNewBuf))
   {
      //printk("ERROR:system buffer allocation failed!\n");
      /*assign old data buffer back to ring*/
      pNewBuf	= rxParams->data_ptr;
      rxParams->data_ptr = NULL;
      ret = 1;
   }

   AssignPacketBuffertoRing(pDescriptor, pTravel, pNewBuf);

   return ret;
}

#if defined(CONFIG_BCM963138) || defined(_BCM963138_) || defined(CONFIG_BCM963148) || defined(_BCM963148_)
static inline int rdp_cpu_ring_read_packet_refill2(uint32_t ring_id, CPU_RX_DESCRIPTOR *rxDesc)
{
   uint32_t ret;
   RING_DESCTIPTOR *pDescriptor = &host_ring[ring_id];
   RING_DESC_UNION *pTravel = pDescriptor->head;
   void *pNewBuf;

#ifdef __KERNEL__
   if (unlikely(pDescriptor->admin_status == 0))
   {
      return BL_LILAC_RDD_ERROR_CPU_RX_QUEUE_EMPTY;
   }
#endif

   ret = ReadPacketFromRing2(pDescriptor, pTravel, rxDesc);
   if (ret)
   {
      return  ret;
   }

   /* A valid packet is recieved try to allocate a new data buffer and
    * refill the ring before giving the packet to upper layers
    */
   pNewBuf = rdp_databuf_alloc(pDescriptor);

   /*validate allocation*/
   if (unlikely(!pNewBuf))
   {
      //printk("ERROR:system buffer allocation failed!\n");
      /*assign old data buffer back to ring*/
      pNewBuf	= rxDesc->data_ptr;
      rxDesc->data_ptr = NULL;
      ret = 1;
   }

   AssignPacketBuffertoRing(pDescriptor, pTravel, pNewBuf);

   return ret;
}
#endif /* defined(CONFIG_BCM963138) || defined(_BCM963138_) || defined(CONFIG_BCM963148) || defined(_BCM963148_) */

static inline int
rdp_cpu_ring_bulk_skb_get(uint32_t ring_id, unsigned int budget, void **rx_pkts)
{
   RING_DESCTIPTOR *pDescriptor = &host_ring[ring_id];
   RING_DESC_UNION *pTravel = NULL;
   CPU_RX_DESCRIPTOR rxDesc;
   struct sk_buff *skb_p;
   uint8_t *data;
   uint32_t len;
   void *pNewBuf;
   unsigned int rx_pktcnt = 0;

   while (budget)
   {
      pTravel = pDescriptor->head;

      rxDesc.word2 = pTravel->cpu_rx.word2;

      rxDesc.word2 = swap4bytes(rxDesc.word2);

      if ((rxDesc.ownership == OWNERSHIP_HOST))
      {
         //Initiate Uncached read of the descriptor words to save CPU cycles
         rxDesc.word0 = pTravel->cpu_rx.word0;
         rxDesc.word3 = pTravel->cpu_rx.word3;
#if defined(CONFIG_BCM96838)
         rxDesc.word1 = pTravel->cpu_rx.word1;
#endif

         rxDesc.ownership = 0; /*clear the ownership bit */
         data = (uint8_t *)PHYS_TO_CACHED(rxDesc.word2);

         pNewBuf = rdp_databuf_alloc(pDescriptor);
         if (unlikely(!pNewBuf))
         {
            /* Push back the rxDesc */
            AssignPacketBuffertoRing(pDescriptor, pTravel, data);
            break;
         }

         rxDesc.word0 = swap4bytes(rxDesc.word0);

         len = (uint32_t)rxDesc.packet_length;

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
         //cache_invalidate_len_outer_first(data, 32);
         cache_invalidate_len_outer_first(data, len);
#endif

         bcm_prefetch(data, 1);
         rxDesc.word3 = swap4bytes(rxDesc.word3);

         /* allocate skb structure*/
         skb_p = skb_header_alloc();
         if (!skb_p)
         {
            printk("%s : SKB Allocation failure\n", __FUNCTION__);
            break;
         }

         /* initialize the skb */
         skb_headerinit(BCM_PKT_HEADROOM,
#if defined(CC_NBUFF_FLUSH_OPTIMIZATION)
                        SKB_DATA_ALIGN(len + BCM_SKB_TAILROOM),
#else
                        BCM_MAX_PKT_LEN,
#endif
                        skb_p, data, bdmf_sysb_recycle, 0, NULL);

         skb_trim(skb_p, len);
         skb_p->recycle_flags &= SKB_NO_RECYCLE; /* no skb recycle,just do data recyle */

#if defined(CC_NBUFF_FLUSH_OPTIMIZATION)
         /* Set dirty pointer to optimize cache invalidate */
         skb_shinfo((struct sk_buff *)(skb_p))->dirty_p = skb_p->data + ETH_HLEN;
#endif

 
         skb_p->queue_mapping = rxDesc.wl_chain_id; /* Store WL chainid in queue_mapping field temporarily */
         skb_p->mark = SET_WLAN_PRIORITY(skb_p->mark, rxDesc.wl_tx_priority);

#if defined(CONFIG_BCM96838)
         skb_p->metadata |= (rxDesc.flow_id << 16) | rxDesc.dst_ssid;
#endif

         rx_pkts[rx_pktcnt] = (void *)skb_p;

         AssignPacketBuffertoRing(pDescriptor, pTravel, pNewBuf);
      } 
      else
      {
         /* No more packets to read. Ring is empty */
         break;
      }
      budget--;
      rx_pktcnt++;
   }

   return rx_pktcnt;
}

static inline int
rdp_cpu_ring_bulk_fkb_get(uint32_t ring_id, unsigned int budget, void **rx_pkts)
{
   RING_DESCTIPTOR *pDescriptor = &host_ring[ring_id];
   RING_DESC_UNION *pTravel = NULL;
   CPU_RX_DESCRIPTOR rxDesc;
   FkBuff_t *fkb_p;
   uint8_t *data;
   uint32_t len;
   void *pNewBuf;
   unsigned int rx_pktcnt = 0;

   while (budget)
   {
      pTravel = pDescriptor->head;

      rxDesc.word2 = pTravel->cpu_rx.word2;

      rxDesc.word2 = swap4bytes(rxDesc.word2);

      if ((rxDesc.ownership == OWNERSHIP_HOST))
      {
         //Initiate Uncached read of the descriptor words to save CPU cycles
         rxDesc.word0 = pTravel->cpu_rx.word0;
         rxDesc.word3 = pTravel->cpu_rx.word3;

         rxDesc.ownership = 0; /*clear the ownership bit */
         data = (uint8_t *)PHYS_TO_CACHED(rxDesc.word2);

         pNewBuf = rdp_databuf_alloc(pDescriptor);
         if (unlikely(!pNewBuf))
         {
            /* Push back the rxDesc */
            AssignPacketBuffertoRing(pDescriptor, pTravel, data);
            break;
         }

         rxDesc.word0 = swap4bytes(rxDesc.word0);
         len = (uint32_t)rxDesc.packet_length;

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
         // cache_invalidate_len_outer_first(data, 32);
         cache_invalidate_len_outer_first(data, len);
#endif
         bcm_prefetch(data, 1);

         rxDesc.word3 = swap4bytes(rxDesc.word3);

         /* Convert descriptor to FkBuff */
         fkb_p = fkb_init(data, BCM_PKT_HEADROOM, data, len);
#if defined(CC_NBUFF_FLUSH_OPTIMIZATION)
         fkb_p->dirty_p = _to_dptr_from_kptr_(data + ETH_HLEN);
#endif
         fkb_p->recycle_hook = bdmf_sysb_recycle;
         fkb_p->recycle_context = 0;
         fkb_p->metadata = rxDesc.wl_metadata;

         rx_pkts[rx_pktcnt] = (void *)fkb_p;

         //BUZZZ_DPL1(WFD_PKT_GET_PROG, 1, rx_pktcnt);
         AssignPacketBuffertoRing(pDescriptor, pTravel, pNewBuf);
      } 
      else
      {
         /* No more packets to read. Ring is empty */
         break;
      }
      budget--;
      rx_pktcnt++;
   }

   return rx_pktcnt;
}

#endif /*ifndef _CFE_*/
#endif /* defined(__KERNEL__) || defined(_CFE_) */
#endif /* _RDP_CPU_RING_INLINE_H_ */
