
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

/*
 *******************************************************************************
 * File Name  : rdpa_cmd_tm.c
 *
 * Description: This file contains the FAP Traffic Manager configuration API.
 *
 * The FAP Driver maintains two sets of TM configuration parameters: One for the
 * AUTO mode, and another for the MANUAL mode. The AUTO mode settings are
 * managed by the Ethernet driver based on the auto-negotiated PHY rates.
 * The MANUAL settings should be used by the user to configure the FAP TM.
 *
 * The mode can be set dynamically. Changing the mode does not apply the
 * corresponding settings into the FAP, it simply selects the current mode.
 * The settings corresponding to the current mode will take effect only when
 * explicitly applied to the FAP(s). This allows the caller to have a complete
 * configuration before activating it.
 *
 *******************************************************************************
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/bcm_log.h>
#include "bcmenet.h"
#include "bcmtypes.h"
#include "bcmnet.h"
#include "rdpa_types.h"
#include "rdpa_api.h"
#include "rdpa_ag_port.h"
#include "rdpa_drv.h"
#include "rdpa_cmd_tm.h"

#define __BDMF_LOG__
//#define __DUMP_PORTS__

#define CMD_TM_LOG_ID_RDPA_CMD_DRV BCM_LOG_ID_RDPA_CMD_DRV

#if defined(__BDMF_LOG__)
#define CMD_TM_LOG_ERROR(fmt, args...)                                         \
    do {                                                                            \
        if (bdmf_global_trace_level >= bdmf_trace_level_error)                      \
            bdmf_trace("ERR: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);    \
    } while(0)
#define CMD_TM_LOG_INFO(fmt, args...)                                         \
    do {                                                                            \
        if (bdmf_global_trace_level >= bdmf_trace_level_info)                          \
            bdmf_trace("INF: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);    \
    } while(0)
#define CMD_TM_LOG_DEBUG(fmt, args...)                                         \
    do {                                                                            \
        if (bdmf_global_trace_level >= bdmf_trace_level_debug)                          \
            bdmf_trace("DBG: %s#%d: " fmt "\n", __FUNCTION__, __LINE__, ## args);    \
    } while(0)
#else
#define CMD_TM_LOG_ERROR(fmt, arg...) BCM_LOG_ERROR(fmt, arg...)
#define CMD_TM_LOG_INFO(fmt, arg...)   BCM_LOG_INFO(fmt, arg...)
#define CMD_TM_LOG_DEBUG(fmt, arg...) BCM_LOG_DEBUG(fmt, arg...)
#endif

#define MAX_TM_PER_PORT    32
#define MAX_RUT_QOS_PORTS  16 /* rdpa_if_lan_max is the maximum */
#define MAX_Q_PER_TM       8
#define MAX_WRR_WEIGHT     64

#define QUEUE_ID(CMSqid) (CMSqid -1)   
#define QUEUE_SIZE 256
#define VACANT     -1

#define INVALID_ID -1

/* global definition */
bdmf_index WRR_SUB=7;  //the WRR subsidiary on Root TM

typedef struct {
    uint32 tm_index;
} rdpa_cmd_tm_ctrl_t;

typedef struct {
   BOOL        alloc;
   uint32_t    tm_id;
   bdmf_number qid;
   uint32_t    weight;
} queue_st;

typedef struct {
   BOOL               alloc;
   int                root_tm_id;
   int                root_sp_tm_id;
   int                root_wrr_tm_id;
   int                orl_id;
   BOOL               orl_linked;
   rdpa_if            port_id;
   rdpa_tm_sched_mode arbiter_mode;    /* root TM arbiter mode */
   queue_st           queue_list[MAX_Q_PER_TM];
   int                queue_alloc_cnt;
} port_st;

typedef struct {
   int q_num; 
   int subs[MAX_Q_PER_TM];
} wrr_tm_st;

static rdpa_cmd_tm_ctrl_t rdpa_cmd_tm_ctrl_g;
static port_st port_list[MAX_RUT_QOS_PORTS];
static bdmf_object_handle orig_sched = NULL; /* Original scheduling object - before configuration */
static int tmw_id[MAX_WRR_WEIGHT]={0}; 

static wrr_tm_st wrr_tm = {0, {VACANT, VACANT, VACANT, VACANT, VACANT, VACANT, VACANT, VACANT}}; 

/*******************************************************************************/
/* static routines Functions                                                   */
/*******************************************************************************/

static int get_free_subsidiary(wrr_tm_st *wrr_tm)
{
    int i;
    for (i = 0; i < MAX_Q_PER_TM; i++)
    {
        if (wrr_tm->subs[i] == VACANT)
        {
           CMD_TM_LOG_DEBUG(" return subsidiary=[%d]", i);
           return i;
        }
    }

    return -1;
}

static int get_wrr_sub(bdmf_object_handle root_sched, bdmf_object_handle *psubsidiary)
{
   rdpa_tm_sched_mode mode;
   rdpa_tm_level_type level;

   if (rdpa_egress_tm_subsidiary_get(root_sched, WRR_SUB, psubsidiary) == BDMF_ERR_OK)
   {
     bdmf_object_handle subsidiary = *psubsidiary;

     if ((rdpa_egress_tm_mode_get(subsidiary, &mode)))
       CMD_TM_LOG_ERROR(" rdpa_egress_tm_mode_get FAILED");

     if ((rdpa_egress_tm_level_get(subsidiary, &level)))
       CMD_TM_LOG_ERROR(" rdpa_egress_tm_level_get FAILED \n");
     
     if ((mode == rdpa_tm_sched_wrr) && (level == rdpa_tm_level_egress_tm))
       return 0;
   }
   else CMD_TM_LOG_ERROR(" rdpa_egress_tm_subsidiary_get FAILED \n");

   return -1;
}

static int check_rdpa_mode(rdpa_wan_type mode)
{
    rdpa_system_init_cfg_t sys_init_cfg;
    bdmf_object_handle system_obj = NULL;

    rdpa_system_init_cfg_get(system_obj, &sys_init_cfg);

    return (sys_init_cfg.wan_type == (mode ? TRUE : FALSE));
}

static int get_port_tm_parms(
				uint32_t port_id,
            uint32_t dir,
            uint32_t *pport_sched_type,
            uint32_t *pmax_queues,
            uint32_t *pmax_sp_queues,
            BOOL     *pport_shaper,
            BOOL     *pqueue_shaper)
{
   CMD_TM_LOG_DEBUG("IN: GET PORT TM port_id(%u)", port_id);

   if (port_id >= MAX_RUT_QOS_PORTS) {
      CMD_TM_LOG_ERROR("FAIL: port_id(%u) is not valid", port_id);
      return RDPA_DRV_PORT_ID_NOT_VALID;
   }

   *pmax_queues    = MAX_Q_PER_TM;
   *pmax_sp_queues = MAX_Q_PER_TM;
   *pport_shaper   = TRUE;

   if (dir == rdpa_dir_us) {
      *pqueue_shaper    = TRUE;
      *pport_sched_type = RDPA_TM_SP_CAPABLE | RDPA_TM_WRR_CAPABLE |
                          RDPA_TM_SP_WRR_CAPABLE;
   }
   else {
      *pqueue_shaper    = FALSE;
      *pport_sched_type = RDPA_TM_SP_CAPABLE;
   }

   return 0;
}

static int add_root_tm_to_port(
                uint32_t port_id,
                uint32_t tm_id,
                uint32_t arbiter_mode)
{
    port_st    *pport = NULL;

    CMD_TM_LOG_DEBUG("IN: ADDED ROOT TM(%u) port_id(%u)", tm_id,  port_id);

    if (port_id >= MAX_RUT_QOS_PORTS) {
        CMD_TM_LOG_ERROR("FAIL: port_id(%u) is not valid", port_id);
        return RDPA_DRV_PORT_ID_NOT_VALID;
    }

    pport = &port_list[port_id];

    if (pport->alloc) {
        CMD_TM_LOG_ERROR("FAIL: TM(%u) port_id(%u) is ALLOCATED", tm_id,  port_id);
        return RDPA_DRV_PORT_NOT_ALLOC;
    }

    pport->alloc        = TRUE;
    pport->root_tm_id   = tm_id;
    pport->port_id      = port_id;
    pport->arbiter_mode = arbiter_mode;

    return 0;
}

static int queue_allocate(
            uint32_t dev_type,
            uint32_t dev_id,
            uint32_t q_id,
            BOOL     alloc)
{
    CMD_TM_LOG_DEBUG("IN: dev_id=%u q_id=%u setalloc=%d", dev_id, q_id, alloc);

    if (dev_type != RDPA_IOCTL_DEV_PORT) {
        CMD_TM_LOG_ERROR("not implement for such dev type(%u) still", dev_type);
        return RDPA_DRV_ERROR;
    }

    if ((dev_id >= MAX_RUT_QOS_PORTS) || (q_id >= MAX_Q_PER_TM)) {
	     CMD_TM_LOG_ERROR("FAIL: port_id(%u) is out of range", dev_id);
        return RDPA_DRV_PORT_ID_NOT_VALID;
    }

    port_list[dev_id].queue_list[q_id].alloc = alloc;

    return 0;
}

static int get_orl(
				uint32_t port_id,
            uint32_t dir,
				uint32_t *porl_id,
            uint32_t *pshaping_rate,
            BOOL     *porl_linked)
{
	int rc = 0;
	bdmf_object_handle   sched  = NULL;
	rdpa_egress_tm_key_t tm_key = {};
	rdpa_tm_rl_cfg_t     rl_cfg = {};
	port_st              *pport = NULL;

	CMD_TM_LOG_DEBUG("IN: port_id(%u)", port_id);

	*porl_id       = INVALID_ID;
   *pshaping_rate = 0;
   *porl_linked   = FALSE;
   
	if (port_id >= MAX_RUT_QOS_PORTS) {
		CMD_TM_LOG_ERROR("FAIL: port_id(%u) is out of range", port_id);
		return RDPA_DRV_PORT_ID_NOT_VALID;
	}

	pport = &port_list[port_id];

   if (pport->orl_id == INVALID_ID)
      return 0;   /* orl does not exist */

   /* get the orl shaping rate */      
	tm_key.dir 	 = dir;
	tm_key.index = pport->orl_id;

	if ((rc = rdpa_egress_tm_get(&tm_key, &sched)) || (sched == NULL)) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: orl_tm(%u) rc(%d)", pport->orl_id, rc);
		return RDPA_DRV_TM_GET;
	}

	if ((rc = rdpa_egress_tm_rl_get(sched, &rl_cfg))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_rl_get() failed: orl_tm(%u) af(%u) rc(%d)",
			pport->orl_id, rl_cfg.af_rate, rc);
      bdmf_put(sched);
		return RDPA_DRV_TM_CFG_GET;
	}

   bdmf_put(sched);

	*porl_id       = pport->orl_id;
   *porl_linked   = pport->orl_linked;
   *pshaping_rate = rl_cfg.af_rate / 1000;

	return 0;
}

static int add_q_to_port(
				uint32_t port_id,
				uint32_t tm_id,
				uint32_t q_id,
            uint32_t weight)
{
	port_st	*pport;
	queue_st *pq;

	CMD_TM_LOG_DEBUG("IN: TM(%u) port_id(%u) q_id(%u)",  tm_id,  port_id, q_id);

	if ((port_id >= MAX_RUT_QOS_PORTS) || (q_id >= MAX_Q_PER_TM))  {
		CMD_TM_LOG_ERROR("FAIL: Invalid port_id(%u) OR q_id(%u)", port_id, q_id);
		return RDPA_DRV_PORT_ID_NOT_VALID;
	}

	pport = &port_list[port_id];
	pq    = &(pport->queue_list[q_id]);

	if (!pport->alloc)  {
		CMD_TM_LOG_ERROR("FAIL: port_id(%u) NOT ALLOCATED", port_id);
		return RDPA_DRV_PORT_NOT_ALLOC;
	}

	pq->tm_id  = tm_id;
	pq->qid    = q_id;
	pq->weight = weight;

	if (!pq->alloc)
	{
	    pq->alloc  = TRUE;
	    pport->queue_alloc_cnt++;
	}

	return 0;
}

static int remove_q_from_port(
				uint32_t port_id,
				uint32_t q_id)
{
	port_st	*pport;
	queue_st *pq;

	CMD_TM_LOG_DEBUG("IN: port_id(%u) q_id(%u)", port_id, q_id);

	if ((port_id >= MAX_RUT_QOS_PORTS) || (q_id >= MAX_Q_PER_TM))  {
		CMD_TM_LOG_ERROR("FAIL: Invalid port_id(%u) OR q_id(%u)", port_id, q_id);
		return RDPA_DRV_PORT_ID_NOT_VALID;
	}

	pport = &port_list[port_id];
	pq    = &(pport->queue_list[q_id]);

	if (!pport->alloc)  {
		CMD_TM_LOG_ERROR("FAIL: port_id(%u) NOT ALLOCATED", port_id);
		return RDPA_DRV_PORT_NOT_ALLOC;
	}

	pq->alloc  = FALSE;
   pq->tm_id  = INVALID_ID;
   pq->qid    = q_id;
   pq->weight = 0;

	pport->queue_alloc_cnt--;

	return 0;
}

static int get_tm_by_qid(
        uint32_t dev_type,
        uint32_t dev_id,
        uint32_t q_id,
        uint32_t *ptm_id,
        BOOL     *pfound)
{
    int ret = RDPA_DRV_SUCCESS;
    
    CMD_TM_LOG_DEBUG("IN: dev_type(%u) dev_id(%u) q_id(%u)", dev_type, dev_id, q_id);

    /* initialize return values */
    *ptm_id = INVALID_ID;
    *pfound = FALSE;
    
    switch (dev_type)
    {
        case RDPA_IOCTL_DEV_PORT:
        {
            port_st  *pport;
            queue_st *pq;

            if (dev_id >= MAX_RUT_QOS_PORTS || q_id >= MAX_Q_PER_TM) {
	             CMD_TM_LOG_ERROR("FAIL: Invalid dev_id(%u) or q_id(%u)", dev_id, q_id);
                ret = RDPA_DRV_PORT_ID_NOT_VALID;
                break;
            }

            pport = &port_list[dev_id];
            if (!pport->alloc) {
                /* Port tm has not been configured.
                 * Return as SUCCESS, but not found.
                 */
                break;
            }
            
            pq = &(pport->queue_list[q_id]);
            *ptm_id = pq->tm_id;
            *pfound = pq->alloc;
            
            break;
        }
        default:
            ret = RDPA_DRV_ERROR;
            CMD_TM_LOG_ERROR("not implement for such dev type(%u) still", dev_type);
            break;
    }
    
    return ret;
}

static int get_q_cfg(
      uint32_t dev_type,
		uint32_t dev_id,
		uint32_t q_id,
      uint32_t dir,
		uint32_t *ptm_id,
      uint32_t *pqsize,
      uint32_t *pweight,
      uint32_t *pshaping_rate,
		BOOL     *pfound)
{
	bdmf_object_handle   sched = NULL;
   rdpa_tm_sched_mode   tm_mode;
	rdpa_egress_tm_key_t tm_key = {};
	rdpa_tm_queue_cfg_t  q_cfg  = {};
	rdpa_tm_rl_cfg_t     rl_cfg = {};
	bdmf_error_t         rc     = BDMF_ERR_OK;
	port_st	*pport;
	queue_st *pq;
   uint32_t q_index;
   int      ret = RDPA_DRV_SUCCESS;

	CMD_TM_LOG_DEBUG("IN: dev_id(%u) q_id(%u)", dev_id, q_id);

   /* initialize return values */
   *ptm_id        = INVALID_ID;
   *pshaping_rate = 0;
	*pfound        = FALSE;

   if (dev_type != RDPA_IOCTL_DEV_PORT) {
      CMD_TM_LOG_ERROR("not implement for such dev type(%u) still", dev_type);
      return RDPA_DRV_ERROR;
   }

   if (dev_id >= MAX_RUT_QOS_PORTS || q_id >= MAX_Q_PER_TM) {
	   CMD_TM_LOG_ERROR("FAIL: Invalid dev_id(%u) or q_id(%u)", dev_id, q_id);
      return RDPA_DRV_PORT_ID_NOT_VALID;
   }

   pport = &port_list[dev_id];
   if (!pport->alloc) {
      /* Port tm has not been configured.
       * Return as SUCCESS, but not found.
       */
      return RDPA_DRV_SUCCESS;
   }
   
   pq = &(pport->queue_list[q_id]);
   if (pq->tm_id == INVALID_ID) {
      /* Runner queue has not been configured.
       * Return as SUCCESS, but not found.
       */
      return RDPA_DRV_SUCCESS;
   }

   /* Runner queue has been configured for this qid. Get queue config. */
               
	tm_key.dir 	 = dir;
	tm_key.index = pq->tm_id;

	if ((rc = rdpa_egress_tm_get(&tm_key, &sched))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: tm(%d) rc(%d)", pq->tm_id, rc);
		return RDPA_DRV_TM_GET;
	}

   /* get tm mode */
   if ((rc = rdpa_egress_tm_mode_get(sched, &tm_mode))) {
	   CMD_TM_LOG_ERROR("rdpa_egress_tm_mode_get() failed: rc(%d)", rc);
	   ret = RDPA_DRV_MODE_GET;
      goto get_tm_exit;
   }

   if (tm_mode == rdpa_tm_sched_disabled) {
      /* If weight is 0, the queue is SP. Get the queue shaping rate */
      if (pq->weight == 0) {      
         /* get the shaping rate */
	      if ((rc = rdpa_egress_tm_rl_get(sched, &rl_cfg))) {
		      CMD_TM_LOG_ERROR("rdpa_egress_tm_rl_get() failed: tm(%u) rc(%d)", pq->tm_id, rc);
		      ret = RDPA_DRV_TM_CFG_GET;
            goto get_tm_exit;
	      }

         *pshaping_rate = rl_cfg.af_rate / 1000;
      }
   
      /* each shaper has only one queue. i.e. q_index = 0 */
      q_index = 0;
   }
   else {
      /* derive queue index from qid */
      if (dir == rdpa_dir_ds)
         q_index = q_id;
      else
         q_index = MAX_Q_PER_TM - q_id - 1;
   }

   /* get qsize from q_cfg.drop_threshold. */
	if ((rc = rdpa_egress_tm_queue_cfg_get(sched, q_index, &q_cfg))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_queue_cfg_get() failed: rc(%d)", rc);
		ret = RDPA_DRV_Q_CFG_GET;
      goto get_tm_exit;
	}

   *ptm_id  = pq->tm_id;
   *pqsize  = q_cfg.drop_threshold;
   *pweight = pq->weight;
   *pfound  = pq->alloc;
      
get_tm_exit:
   bdmf_put(sched);
   return ret;
}

static int get_q_stats(
      uint32_t dev_type,
      uint32_t dev_id,
      uint32_t q_id,
      uint32_t dir,
      rdpa_stat_1way_t *pstats)
{
   bdmf_error_t rc = BDMF_ERR_OK;
   bdmf_object_handle    sched = NULL;
   rdpa_egress_tm_key_t  tm_key = {};
   rdpa_tm_queue_index_t q_index = {};
   port_st	*pport;
   queue_st *pq;
   
   int ret = RDPA_DRV_SUCCESS;

	CMD_TM_LOG_DEBUG("IN: dev_type(%u) dev_id(%u) q_id(%u)", dev_type, dev_id, q_id);

   /* initialize return values */
   memset(pstats, 0, sizeof(rdpa_stat_1way_t));

   if (dev_type != RDPA_IOCTL_DEV_PORT) {
      CMD_TM_LOG_ERROR("not implement for such dev type(%u)", dev_type);
      return RDPA_DRV_ERROR;
   }

   if (dev_id >= MAX_RUT_QOS_PORTS || q_id >= MAX_Q_PER_TM) {
	   CMD_TM_LOG_ERROR("FAIL: Invalid dev_id(%u) or q_id(%u)", dev_id, q_id);
      return RDPA_DRV_PORT_ID_NOT_VALID;
   }

   if (dev_id == rdpa_if_wan1) {
      tm_key.dir   = dir;
      /* For xtm wan, the queue's root tm index is q_id. See xtmrt_runner.c. */
      tm_key.index = q_id;
   }
   else {
      /* either ethLan or ethWan */
      pport = &port_list[dev_id];
      if (!pport->alloc) {
         /* Port tm has not been configured.
          * Return as SUCCESS.
          */
         return RDPA_DRV_SUCCESS;
      }
   
      pq = &(pport->queue_list[q_id]);
      if (pq->tm_id == INVALID_ID) {
         /* Runner queue has not been configured.
          * Return as SUCCESS.
          */
         return RDPA_DRV_SUCCESS;
      }

	   tm_key.dir 	 = dir;
      /* use the root tm index.
       * queue stats are returned only by querring the queue's root tm.
       */
	   tm_key.index = pport->root_tm_id;
   }
   
   /* Get the root tm object of the queue */
	if ((rc = rdpa_egress_tm_get(&tm_key, &sched))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: tm(%d) rc(%d)", (int)tm_key.index, rc);
		return RDPA_DRV_TM_GET;
	}

   if (dir == rdpa_dir_us) {
      if (dev_id == rdpa_if_wan1)
         /* For xtmWan, the queue's channel id is q_id. See xtmrt_runner.c. */
         q_index.channel = q_id;
      else
         /* ethWan channel id = RDD_WAN_CHANNEL_0 (0) */
         q_index.channel = rdpa_if_wan0;
   }
   else {
      /* ethLan channel id is its emac id.
       * lan0 channel id = BL_LILAC_RDD_EMAC_ID_0 (1) = rdpa_if_lan0 - rdpa_if_wan1
       * lan1 channel id = BL_LILAC_RDD_EMAC_ID_1 (2) = rdpa_if_lan1 - rdpa_if_wan1
       * etc....
       */
      q_index.channel = dev_id - rdpa_if_wan1;
   }
   
   q_index.queue_id = q_id;
   
   /* get queue statistics */
   if ((rc = rdpa_egress_tm_queue_statistics_get(sched, &q_index, pstats))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_queue_statistics_get() failed: rc(%d)", rc);
      ret = RDPA_DRV_QUEUE_STATS_GET;
   }

   bdmf_put(sched);
   return ret;
}

#ifdef __DUMP_PORTS__
static int dump_ports(void)
{
    port_st    *pport = NULL;
    int i;

    for (i = 0; i < MAX_RUT_QOS_PORTS; i++) {
        pport = &port_list[i];
        CMD_TM_LOG_DEBUG("PORT(%u) STAT: alloc(%u) queue_cnt(%u) root_id(%u)",
            i, pport->alloc, pport->queue_alloc_cnt, pport->root_tm_id);
    }

    return 0;
}
#endif /* __DUMP_PORTS__ */

static int tm_set(
        bdmf_mattr_handle sched_attrs,
        rdpa_drv_ioctl_tm_t *pTm,
        uint32_t *ptm_id,
        bdmf_object_handle root_sched,
        bdmf_object_handle *psched)
{
    bdmf_error_t rc = BDMF_ERR_OK;
    bdmf_number new_tm_id;
    bdmf_object_handle tmp_psched = NULL;
    rdpa_tm_service_queue_t  service_queue = {.enable = 0};

    CMD_TM_LOG_DEBUG("IN: ROOT_tm(%u) dir(%u) dev_type(%u) dev_id(%u) arbiter mode(%u) level(%u)",
		pTm->root_tm_id, pTm->dir, pTm->dev_type, pTm->dev_id, pTm->arbiter_mode, pTm->level);

    rdpa_egress_tm_dir_set(sched_attrs, pTm->dir);
    rdpa_egress_tm_level_set(sched_attrs, pTm->level); /* rdpa_tm_level_queue / rdpa_tm_level_egress_tm */
    rdpa_egress_tm_mode_set(sched_attrs, pTm->arbiter_mode);
    if(pTm->service_queue)
           service_queue.enable = 1;
       rdpa_egress_tm_service_queue_set(sched_attrs, &service_queue);

    if ((rc = bdmf_new_and_set(rdpa_egress_tm_drv(), root_sched, sched_attrs, &tmp_psched)) || 
        (tmp_psched == NULL)) {
        CMD_TM_LOG_ERROR("bdmf_new_and_set() failed: ROOT_tm(%u) rc(%d)", pTm->root_tm_id, rc);
        return RDPA_DRV_NEW_TM_ALLOC;
    }

    *psched = tmp_psched;

    if ((rc = rdpa_egress_tm_index_get(*psched, &new_tm_id))) {
        CMD_TM_LOG_ERROR("rdpa_egress_tm_index_get() failed: ROOT_tm(%u) rc(%d)", pTm->root_tm_id, rc);
        return RDPA_DRV_TM_INDEX_GET;
    }

    *ptm_id = (uint32_t) new_tm_id;

    return 0;
}

static int tm_replace(
        bdmf_object_handle port_obj,
        bdmf_object_handle sched)
{
    rdpa_port_tm_cfg_t orig_tm_cfg = {};
    bdmf_error_t rc = BDMF_ERR_OK;

    CMD_TM_LOG_DEBUG("IN");

    /* Removing an old TM from Port Object (and storing it in a cool & dark place) */
    if ((rc = rdpa_port_tm_cfg_get(port_obj, &orig_tm_cfg))) {
        CMD_TM_LOG_ERROR("rdpa_port_tm_cfg_get() failed: rc(%d)", rc);
        return RDPA_DRV_TM_CFG_GET;
    }

    orig_sched = orig_tm_cfg.sched;

    if (orig_tm_cfg.sched) {
        if ((rc = bdmf_destroy(orig_tm_cfg.sched))) {
            CMD_TM_LOG_ERROR("bdmf_destroy() failed: rc(%d)", rc);
        }
    } else {
        CMD_TM_LOG_DEBUG("REPLACE-DESTROY avoided - sched == NULL");
    }

    orig_tm_cfg.sched = sched;
    if ((rc = rdpa_port_tm_cfg_set(port_obj, &orig_tm_cfg))) {
        CMD_TM_LOG_ERROR("rdpa_port_tm_cfg_set() failed: rc(%d)", rc);
        return RDPA_DRV_TM_CFG_SET;
    }

    return 0;
}

static int orl_config(
			rdpa_drv_ioctl_tm_t *ptm)
{
	BDMF_MATTR(sched_attrs, rdpa_egress_tm_drv());
	bdmf_error_t       rc         = BDMF_ERR_OK;
	bdmf_object_handle system_obj = NULL;
	bdmf_object_handle sched      = NULL;
	bdmf_number        new_tm_id;
   uint32_t orl_id       = INVALID_ID;
   uint32_t shaping_rate = 0;
   BOOL     orl_linked   = FALSE;
	int ret = 0;

	CMD_TM_LOG_DEBUG("orl_config: port_id(%u) dir(%u)", ptm->dev_id, ptm->dir);

   if ((ret = get_orl(ptm->dev_id, ptm->dir,
                      &orl_id, &shaping_rate, &orl_linked))) {
      CMD_TM_LOG_ERROR("get_orl() failed: port_id(%u) rc(%d)", ptm->dev_id, ret);
		return RDPA_DRV_TM_GET;
   }
   if (orl_id != INVALID_ID)
   {
      CMD_TM_LOG_ERROR("port_id(%u) already has an orl(%u)", ptm->dev_id, orl_id);
		return RDPA_DRV_TM_GET;
   }
      
	if ((rc = rdpa_system_get(&system_obj)) || (system_obj == NULL)) {
		CMD_TM_LOG_ERROR("rdpa_system_get() failed: rc(%d)", rc);
		return RDPA_DRV_PORT_GET;
	}
   
	rdpa_egress_tm_dir_set(sched_attrs, ptm->dir);
	rdpa_egress_tm_level_set(sched_attrs, rdpa_tm_level_egress_tm);
	rdpa_egress_tm_mode_set(sched_attrs, rdpa_tm_sched_disabled);
   rdpa_egress_tm_overall_rl_set(sched_attrs, TRUE);

	if ((rc = bdmf_new_and_set(rdpa_egress_tm_drv(), system_obj, sched_attrs, &sched)) ||
       (sched == NULL)) {
		CMD_TM_LOG_ERROR("bdmf_new_and_set() failed: port_id(%u) rc(%d)", ptm->dev_id, rc);
	   bdmf_put(system_obj);
		return RDPA_DRV_NEW_TM_ALLOC;
	}

	bdmf_put(system_obj);

   if ((rc = rdpa_egress_tm_index_get(sched, &new_tm_id))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_index_get() failed: port_id(%u) rc(%d)", ptm->dev_id, rc);
		ret = RDPA_DRV_TM_INDEX_GET;

      /* Error! destroy the orl just created */
	   if ((rc = bdmf_destroy(sched))) {
		   CMD_TM_LOG_ERROR("bdmf_destroy() failed: orl_tm(%u) rc(%d)", ptm->tm_id, rc);
	   }
	}
   else {
      ptm->tm_id = (uint32_t)new_tm_id;
      port_list[ptm->dev_id].orl_id = ptm->tm_id;
   }

	return ret;
}

static int orl_remove(
         uint32_t port_id,
         uint32_t dir)
{
	bdmf_error_t         rc     = BDMF_ERR_OK;
	bdmf_object_handle   sched  = NULL;
	rdpa_egress_tm_key_t tm_key = {};
   uint32_t orl_id     = INVALID_ID;
   BOOL     orl_linked = FALSE;
	port_st  *pport     = NULL;
	int ret = 0;

	CMD_TM_LOG_DEBUG("orl_remove: port_id(%u) dir(%u)", port_id, dir);

	if (port_id >= MAX_RUT_QOS_PORTS) {
		CMD_TM_LOG_ERROR("FAIL: port_id(%u) is out of range", port_id);
		return RDPA_DRV_PORT_ID_NOT_VALID;
	}

	pport      = &port_list[port_id];
	orl_id     = pport->orl_id;
	orl_linked = pport->orl_linked;

   if (orl_id == INVALID_ID)
   {
      CMD_TM_LOG_DEBUG("port_id(%u) does not have an orl.", port_id);
		return 0;   /* do nothing */
   }
      
	tm_key.dir   = dir;
	tm_key.index = orl_id;

	if ((rc = rdpa_egress_tm_get(&tm_key, &sched)) || (sched == NULL)) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: orl_tm(%u) rc(%d)",
         orl_id, rc);
		return RDPA_DRV_TM_GET;
	}

   /* If the orl is linked to the port, unlink it first. */      
	if (orl_linked) {
   	bdmf_object_handle port_obj = NULL;
      
      if ((rc = rdpa_port_get(port_id, &port_obj)) || (port_obj == NULL)) {
		   CMD_TM_LOG_ERROR("rdpa_port_get() failed: port(%d) rc(%d)", port_id, rc);
		   ret = RDPA_DRV_PORT_GET;
	   }
      else {
         if ((rc = bdmf_unlink(port_obj, sched))) {
		      CMD_TM_LOG_ERROR("bdmf_unlink() failed: port(%u) orl_tm(%u) rc(%d)",
               port_id, orl_id, rc);
		      ret = RDPA_DRV_ORL_UNLINK;
	      }
         else {
            port_list[port_id].orl_linked = FALSE;
         }
         bdmf_put(port_obj);
      }
   }
      
	bdmf_put(sched);
   
   if (!ret && !port_list[port_id].orl_linked) {
	   if ((rc = bdmf_destroy(sched))) {
		   CMD_TM_LOG_ERROR("bdmf_destroy() failed: orl_tm(%u) rc(%d)", orl_id, rc);
		   ret = RDPA_DRV_SH_DESTROY;
	   }
      else {
         port_list[port_id].orl_id = INVALID_ID;
      }
   }

	return ret;
}

static int orl_link(
			rdpa_drv_ioctl_tm_t *ptm)
{
	bdmf_error_t         rc     = BDMF_ERR_OK;
	bdmf_object_handle   sched  = NULL;
	rdpa_egress_tm_key_t tm_key = {};
	rdpa_tm_rl_cfg_t     rl_cfg = {};
   uint32_t orl_id       = INVALID_ID;
   uint32_t shaping_rate = 0;
   BOOL     orl_linked   = FALSE;
	int ret = 0;

	rl_cfg.af_rate = ptm->shaping_rate * 1000; 	/* Best Effort: shaping_rate is in kbit/s: 1 kilobit = 1000 bits */

	CMD_TM_LOG_DEBUG("orl_link: port(%u) orl_tm(%u) af(%u)",
      ptm->dev_id, ptm->tm_id, rl_cfg.af_rate);

   if ((ret = get_orl(ptm->dev_id, ptm->dir,
                      &orl_id, &shaping_rate, &orl_linked))) {
      CMD_TM_LOG_ERROR("get_orl() failed: port_id(%u) rc(%d)", ptm->dev_id, ret);
		return RDPA_DRV_ORL_LINK;
   }
   if (orl_id != ptm->tm_id)
   {
      CMD_TM_LOG_ERROR("orl(%u) was not allocated for port_id(%u)", ptm->tm_id, ptm->dev_id);
		return RDPA_DRV_ORL_LINK;
   }
         
	tm_key.dir 	 = ptm->dir;
	tm_key.index = ptm->tm_id;

	if ((rc = rdpa_egress_tm_get(&tm_key, &sched)) || (sched == NULL)) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: orl_tm(%u) rc(%d)", ptm->tm_id, rc);
		return RDPA_DRV_TM_GET;
	}

	if ((rc = rdpa_egress_tm_rl_set(sched, &rl_cfg))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_rl_set() failed: orl_tm(%u) af(%u) rc(%d)",
			ptm->tm_id, rl_cfg.af_rate, rc);
		ret = RDPA_DRV_TM_CFG_SET;
	}
   else if (!orl_linked) {
	   bdmf_object_handle   port_obj = NULL;
      
	   if ((rc = rdpa_port_get(ptm->dev_id, &port_obj)) || (port_obj == NULL)) {
		   CMD_TM_LOG_ERROR("rdpa_port_get() failed: port(%d) rc(%d)", ptm->dev_id, rc);
		   ret = RDPA_DRV_PORT_GET;
	   }
      else {
         if ((rc = bdmf_link(port_obj, sched, NULL))) {
		      CMD_TM_LOG_ERROR("bdmf_link() failed: port(%u) orl_tm(%u) rc(%d)",
               ptm->dev_id, ptm->tm_id, rc);
		      ret = RDPA_DRV_ORL_LINK;
         }
         else {
            port_list[ptm->dev_id].orl_linked = TRUE;
         }
	      bdmf_put(port_obj);
      }
	}

	bdmf_put(sched);

	return ret;
}

static int orl_unlink(
			rdpa_drv_ioctl_tm_t *ptm)
{
	bdmf_error_t         rc     = BDMF_ERR_OK;
	bdmf_object_handle   sched  = NULL;
	rdpa_egress_tm_key_t tm_key = {};
   uint32_t orl_id       = INVALID_ID;
   uint32_t shaping_rate = 0;
   BOOL     orl_linked   = FALSE;
	int ret = 0;

	CMD_TM_LOG_DEBUG("orl_unlink: port(%u) orl_tm(%u)", ptm->dev_id, ptm->tm_id);

   if ((ret = get_orl(ptm->dev_id, ptm->dir,
                      &orl_id, &shaping_rate, &orl_linked))) {
      CMD_TM_LOG_ERROR("get_orl() failed: port_id(%u) rc(%d)", ptm->dev_id, ret);
		return RDPA_DRV_ORL_UNLINK;
   }
   if (orl_id != ptm->tm_id)
   {
      CMD_TM_LOG_ERROR("orl(%u) was not allocated for port_id(%u)", ptm->tm_id, ptm->dev_id);
		return RDPA_DRV_ORL_UNLINK;
   }
         
	tm_key.dir 	 = ptm->dir;
	tm_key.index = ptm->tm_id;

	if ((rc = rdpa_egress_tm_get(&tm_key, &sched)) || (sched == NULL)) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: orl_tm(%u) rc(%d)", ptm->tm_id, rc);
		return RDPA_DRV_TM_GET;
	}

   if (orl_linked) {
	   bdmf_object_handle   port_obj = NULL;
   
	   if ((rc = rdpa_port_get(ptm->dev_id, &port_obj))) {
		   CMD_TM_LOG_ERROR("rdpa_port_get() failed: port(%d) rc(%d)", ptm->dev_id, rc);
		   ret = RDPA_DRV_PORT_GET;
	   }
      else {
         if ((rc = bdmf_unlink(port_obj, sched))) {
		      CMD_TM_LOG_ERROR("bdmf_unlink() failed: port(%u) orl_tm(%u) rc(%d)",
               ptm->dev_id, ptm->tm_id, rc);
		      ret = RDPA_DRV_ORL_UNLINK;
         }
         else {
            port_list[ptm->dev_id].orl_linked = FALSE;
         }
   	   bdmf_put(port_obj);
      }
	}

   if (!ret && !port_list[ptm->dev_id].orl_linked)
   {
	   rdpa_tm_rl_cfg_t  rl_cfg = {};
      
      /* clear af rate when orl tm is unlinked from a port. */
	   rl_cfg.af_rate = 0;

	   if ((rc = rdpa_egress_tm_rl_set(sched, &rl_cfg))) {
		   CMD_TM_LOG_ERROR("rdpa_egress_tm_rl_set() failed: orl_tm(%u) af(%u) rc(%d)",
			   ptm->tm_id, rl_cfg.af_rate, rc);
//		   ret = RDPA_DRV_TM_CFG_SET;
         /* don't need to return as error since the orl is already unlinke. */
	   }
   }

	bdmf_put(sched);

	return ret;
}

static int root_tm_remove(
            uint32_t dev_type,
            uint32_t dev_id,
            uint32_t root_tm_id,
            uint32_t dir)
{
    bdmf_error_t rc = BDMF_ERR_OK;
    bdmf_object_handle sched = NULL;
    int ret = 0;

    CMD_TM_LOG_DEBUG("IN: dev_type(%u) dev_id(%u) root_tm_id(%u) dir(%u)", dev_type, dev_id, root_tm_id, dir);

    switch (dev_type)
    {
        case RDPA_IOCTL_DEV_PORT:
        {
            rdpa_egress_tm_key_t tm_key = {};
	         port_st *pport;
            
#ifdef __DUMP_PORTS__
            dump_ports();
#endif

	         if (dev_id >= MAX_RUT_QOS_PORTS) {
		          CMD_TM_LOG_ERROR("FAIL: dev_id(%u) is out of range",  dev_id);
                ret = RDPA_DRV_PORT_ID_NOT_VALID;
                goto root_tm_rm_exit;
	         }
                
            tm_key.dir   = dir;
            tm_key.index = root_tm_id;

            if ((rc = rdpa_egress_tm_get(&tm_key, &sched))) {
                CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: tm(%u) rc(%d)", root_tm_id, rc);
                ret = RDPA_DRV_TM_GET;
                goto root_tm_rm_exit;
            }
            if (sched) {
                bdmf_put(sched);
                if ((rc = bdmf_destroy(sched))) {
                    CMD_TM_LOG_ERROR("bdmf_destroy() failed: tm(%u) rc(%d)", root_tm_id, rc);
                    ret = RDPA_DRV_SH_DESTROY;
                    goto root_tm_rm_exit;
                }
            }

	         pport = &port_list[dev_id];
            if (pport->root_tm_id == root_tm_id) {

                queue_st *pq = &(pport->queue_list[0]);
                int j;
                
                /* If an orl is linked to the port, remove it. */      
                orl_remove(dev_id, dir);
                
                /* remove root tm from port list */
                memset(pport, 0, sizeof(port_st));
                pport->root_sp_tm_id = pport->root_wrr_tm_id = pport->orl_id = INVALID_ID;
                for (j = 0; j < MAX_Q_PER_TM; j++, pq++)
                    pq->tm_id = INVALID_ID;
            }
            /* else, something wrong. */
            
            break;
        }
        case RDPA_IOCTL_DEV_LLID:
        {
            bdmf_object_handle llid = NULL;    
            if ((rc = rdpa_llid_get(dev_id, &llid))) 
            {
                CMD_TM_LOG_ERROR("rdpa_llid_get() failed: llid(%u) rc(%d)", dev_id, rc);
                ret = RDPA_DRV_LLID_GET;
                goto root_tm_rm_exit;
            }
            if ((rc = rdpa_llid_egress_tm_get(llid, &sched)))
            {
                CMD_TM_LOG_ERROR("rdpa_llid_egress_tm_get() failed: llid(%u) rc(%d)", dev_id, rc);
                bdmf_put(llid);
                ret = RDPA_DRV_LLID_TM_GET;
                goto root_tm_rm_exit;
            }
            if (sched)
            {
                if ((rc = rdpa_llid_egress_tm_set(llid, NULL)))
                {
                    CMD_TM_LOG_ERROR("rdpa_llid_egress_tm_set() failed: llid(%u) rc(%d)", dev_id, rc);
                    bdmf_put(llid);
                    ret = RDPA_DRV_LLID_TM_SET;
                    goto root_tm_rm_exit;
                }
                
                if (sched) 
                {
                    if ((rc = bdmf_destroy(sched)))
                    {
                        bdmf_put(llid);
                        CMD_TM_LOG_ERROR("bdmf_destroy() failed: rc(%d)", rc);
                        ret = RDPA_DRV_SH_DESTROY;
                        goto root_tm_rm_exit;
                    }
                }
            }
            bdmf_put(llid);
            break;
        }
        default:
            ret = RDPA_DRV_ERROR;
            break;
        
    }
    
root_tm_rm_exit:
    return ret;
}

static int root_tm_config(
            rdpa_drv_ioctl_tm_t *ptm,
            bdmf_object_handle *psched)
{
    BDMF_MATTR(sched_attrs, rdpa_egress_tm_drv());
    bdmf_object_handle dev_obj = NULL;
    bdmf_object_handle tmp_psched = NULL;
    bdmf_error_t rc = BDMF_ERR_OK;
    int ret = 0;

    CMD_TM_LOG_DEBUG("IN: dev_type(%u) dev_id(%u)", ptm->dev_type, ptm->dev_id);

    switch (ptm->dev_type)
    {
        case RDPA_IOCTL_DEV_PORT:
        {
            /* ------------------- PORT Configuration: Acquire the port object ------------------- */
            if ((rc = rdpa_port_get(ptm->dev_id, &dev_obj))) {
                CMD_TM_LOG_ERROR("rdpa_port_get() failed: port(%d) rc(%d)", ptm->dev_id, rc);
                ret = RDPA_DRV_PORT_GET;
                goto root_tm_cfg_exit;
            }

            if (!check_rdpa_mode(rdpa_wan_gbe)) { /* Perform the TCONT binding only in GPON or EPON mode */
                CMD_TM_LOG_DEBUG("TM to PORT bind removing");

                if ((ret = tm_replace(dev_obj, NULL))) {
                    CMD_TM_LOG_ERROR("tm_replace() failed: port(%d) ret(%d)", ptm->dev_id, ret);
                    goto root_tm_cfg_exit;
                }
            }

            if ((ret = tm_set(sched_attrs, ptm, &ptm->root_tm_id, dev_obj, &tmp_psched))) {
                CMD_TM_LOG_ERROR("rdpa_egress_tm_index_set() failed: ret(%d)", ret);
                goto root_tm_cfg_exit;
            }

            //JJC- TBD
            rdpa_egress_tm_enable_set(tmp_psched, TRUE);

            CMD_TM_LOG_DEBUG("ADDING ROOT TM: ROOT_tm(%u) port_id(%d)", ptm->root_tm_id, ptm->dev_id);

            *psched = tmp_psched;
            if ((ret = add_root_tm_to_port(ptm->dev_id, ptm->root_tm_id, ptm->arbiter_mode))) {
                CMD_TM_LOG_ERROR("add_root_tm_to_port() failed: ROOT_tm(%u) ret(%d)", ptm->root_tm_id, ret);
                goto root_tm_cfg_exit;
            }
         
            break;
        }
        case RDPA_IOCTL_DEV_LLID:
        {
            if ((rc = rdpa_llid_get(ptm->dev_id, &dev_obj))) {
                CMD_TM_LOG_ERROR("rdpa_llid_get() failed: llid(%u) rc(%d)", ptm->dev_id, rc);
                ret = RDPA_DRV_LLID_GET;
                goto root_tm_cfg_exit;
            }
            if ((ret = tm_set(sched_attrs, ptm, &ptm->root_tm_id, NULL, &tmp_psched))) 
            {
                CMD_TM_LOG_ERROR("tm_set() failed: ret(%d)", ret);
                goto root_tm_cfg_exit;
            }
            if ((rc = rdpa_llid_egress_tm_set(dev_obj, tmp_psched))) 
            {
                CMD_TM_LOG_ERROR("rdpa_llid_egress_tm_set() failed: rc(%d)", rc);
                ret = RDPA_DRV_LLID_TM_SET;
                if ((rc = bdmf_destroy(tmp_psched))) 
                    CMD_TM_LOG_ERROR("bdmf_destroy() failed: rc(%d)", rc);
        
                goto root_tm_cfg_exit;
            }
            *psched = tmp_psched;
            break;
        }
     case RDPA_IOCTL_DEV_NONE:
        {
           ptm->service_queue = 1;
            if ((ret = tm_set(sched_attrs, ptm, &ptm->root_tm_id, NULL, &tmp_psched))) 
            {
                CMD_TM_LOG_ERROR("tm_set() failed: ret(%d)", ret);
                goto root_tm_cfg_exit;
            }
           
            *psched = tmp_psched;
            break;
        }
        default:
            ret = RDPA_DRV_ERROR;
            break;
    }

root_tm_cfg_exit:
    if (dev_obj)
        bdmf_put(dev_obj);

    return ret;
}

static int tm_remove(rdpa_drv_ioctl_tm_t *ptm)
{
    int ret = 0;
    bdmf_error_t rc = BDMF_ERR_OK;
    rdpa_egress_tm_key_t tm_key = {};
    bdmf_object_handle sched = NULL;
    port_st *pport = NULL;

#ifdef __DUMP_PORTS__
    dump_ports();
#endif

    if (ptm->dev_id >= MAX_RUT_QOS_PORTS) {
        CMD_TM_LOG_ERROR("FAIL: dev_id(%u) is out of range", ptm->dev_id);
        ret = RDPA_DRV_PORT_ID_NOT_VALID;
        goto tm_remove_exit;
    }
    
    pport = &port_list[ptm->dev_id];
    if (ptm->tm_id == pport->root_tm_id ||
        ptm->tm_id == pport->root_sp_tm_id ||
        ptm->tm_id == pport->root_wrr_tm_id)
    {
        /* this function cannot remove any of the root TMs. Use root_tm_remove()
         * instead. root_tm_remove() will remove all the three root TMs.
         */ 
        CMD_TM_LOG_ERROR("FAIL: tm(%u) cannot be one of the root TMs of dev_id(%u).",
                         ptm->tm_id, ptm->dev_id);
        ret = RDPA_DRV_ERROR;
        goto tm_remove_exit;
    }

    tm_key.dir   = ptm->dir;
    tm_key.index = ptm->tm_id;
    
    if ((rc = rdpa_egress_tm_get(&tm_key, &sched))) {
        CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: dir(%u) tm(%u) rc(%d)",
                         ptm->dir, ptm->tm_id, rc);
        ret = RDPA_DRV_TM_GET;
        goto tm_remove_exit;
    }
    
    if (sched) {
        bdmf_put(sched);
        if ((rc = bdmf_destroy(sched))) {
            CMD_TM_LOG_ERROR("bdmf_destroy() failed: dir(%u) tm(%u) rc(%d)",
                             ptm->dir, ptm->tm_id, rc);
            ret = RDPA_DRV_SH_DESTROY;
            goto tm_remove_exit;
        }
    }
    
    switch (ptm->dev_type)
    {
        case RDPA_IOCTL_DEV_PORT:
        {
            queue_st *pq = &pport->queue_list[0];
            int i;

            for (i = 0; i < MAX_Q_PER_TM; i++, pq++) {
                if (pq->tm_id == ptm->tm_id) {
                    if (pq->alloc)
                        pport->queue_alloc_cnt--;
                        
                    pq->tm_id = INVALID_ID;
                    pq->qid   = 0;
                    pq->alloc = FALSE;
                }
            }
#ifdef __NEW_REPLACE__
            CMD_TM_LOG_INFO("PORT_TM_REMOVE: queue count(%u)", pport_list->queue_alloc_cnt);
            
            if (!pport->queue_alloc_cnt) {
            
                bdmf_object_handle port_obj = NULL;

                if (!check_rdpa_mode(rdpa_wan_gbe)) { /* Perform the TCONT binding only in GPON or EPON mode */
                    CMD_TM_LOG_DEBUG("TM to PORT binding replace");

                    /* ------------------- Acquire the port object and set sched object back ------------------- */
                    if ((rc = rdpa_port_get(ptm->dev_id, &port_obj))) {
                        CMD_TM_LOG_ERROR("rdpa_port_get() failed: port(%d) rc(%d)", ptm->dev_id, rc);
                        ret = RDPA_DRV_PORT_GET;
                        goto tm_remove_exit;
                    }

                    if ((ret = tm_replace(port_obj, orig_sched))) {
                        CMD_TM_LOG_ERROR("tm_replace() failed: port(%d) rc(%d)", ptm->dev_id, ret);
                        bdmf_put(port_obj);
                        goto tm_remove_exit;
                    }

                    bdmf_put(port_obj);
                }

                /* ------------------- Remove root tm ------------------- */
                if ((ret = root_tm_remove(ptm->dev_type, ptm->dev_id, port_list[ptm->dev_id].root_tm_id, ptm->dir))) {
                    CMD_TM_LOG_ERROR("root_tm_remove() failed: ROOT_tm(%u) rc(%d)", port_list[ptm->dev_id].root_tm_id, ret);
                    goto tm_remove_exit;
                }
            }
#endif /* __NEW_REPLACE__ */
            break;
        }
        case RDPA_IOCTL_DEV_LLID:
        default:
            break;
    }

tm_remove_exit:
    return ret;
}

static int port_q_cfg(
            rdpa_drv_ioctl_tm_t *ptm)
{
	bdmf_object_handle sched = NULL;
	rdpa_egress_tm_key_t tm_key = {};
	rdpa_tm_queue_cfg_t queue_cfg = {};
	rdpa_tm_sched_mode arbiter_mode = rdpa_tm_sched_disabled;
	bdmf_error_t rc = BDMF_ERR_OK;
   int q_index;
   int ret = 0;

	CMD_TM_LOG_DEBUG("Q CONFIG: tm(%u) dir(%u) q_id(%u) qsize(%u) shapingrate(%u)",
                    ptm->tm_id, ptm->dir, ptm->q_id, ptm->qsize, ptm->shaping_rate);

	tm_key.dir 	 = ptm->dir;
	tm_key.index = ptm->tm_id;

	if ((rc = rdpa_egress_tm_get(&tm_key, &sched))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: tm(%u) rc(%d)", ptm->tm_id, rc);
		return RDPA_DRV_TM_GET;
	}

	if ((rc = rdpa_egress_tm_mode_get(sched, &arbiter_mode))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_mode_get() failed: tm(%u) rc(%d)", ptm->tm_id, rc);
		ret = RDPA_DRV_MODE_GET;
      goto port_q_cfg_exit;
	}

	queue_cfg.queue_id           = ptm->q_id;
	queue_cfg.drop_alg           = rdpa_tm_drop_alg_dt;
	queue_cfg.drop_threshold     = ptm->qsize;
	queue_cfg.weight             = 0;
	queue_cfg.red_high_threshold = 0; /**< Relevant for ::rdpa_tm_drop_alg_red/::rdpa_tm_drop_alg_wred : packet threshold */
	queue_cfg.red_low_threshold  = 0;

   q_index = ptm->index;
   if (q_index == -1)
   {
      /* queue index is not given. derive it from qid. */
      if (arbiter_mode == rdpa_tm_sched_disabled)
         q_index = 0;   /* rate limiter has only one queue */
      else
      {
         if (ptm->dir == rdpa_dir_ds)
            q_index = ptm->q_id;
         else
            q_index = MAX_Q_PER_TM - ptm->q_id - 1;
      }
   }
   
	CMD_TM_LOG_DEBUG("rdpa_egress_tm_queue_cfg_set(): tm(%u) q_index(%u) q_id(%u) drop_alg(%u) qsize(%u) weight(%u)",
		ptm->tm_id, ptm->index, ptm->q_id, queue_cfg.drop_alg, ptm->qsize, ptm->weight);

	if ((rc = rdpa_egress_tm_queue_cfg_set(sched, q_index, &queue_cfg))) {
		CMD_TM_LOG_ERROR("rdpa_egress_tm_queue_cfg_set() failed: mode(%u) rc(%d)", arbiter_mode, rc);
		ret = RDPA_DRV_Q_CFG_SET;
      goto port_q_cfg_exit;
	}
    
   if (ptm->qsize)
   {
      if ((ret = add_q_to_port(ptm->dev_id, ptm->tm_id, ptm->q_id, ptm->weight)))
         CMD_TM_LOG_ERROR("add_q_to_port() failed: "
               "port_id(%u) tm_id(%u) q_id(%u) ret(%d)", 
                ptm->dev_id, ptm->tm_id, ptm->q_id, ret);
   }
   else
   {
      if ((ret = remove_q_from_port(ptm->dev_id, ptm->q_id)))
         CMD_TM_LOG_ERROR("remove_q_from_port() failed: "
                "port_id(%u) q_id(%u) rc(%d)", 
                ptm->dev_id, ptm->q_id, ret);
   }

port_q_cfg_exit:
	bdmf_put(sched);
   return ret;
}

static int llid_q_cfg(rdpa_drv_ioctl_tm_t *ptm)
{
    bdmf_object_handle sched = NULL;
    rdpa_egress_tm_key_t tm_key = {};
    rdpa_tm_queue_cfg_t queue_cfg = {};
    bdmf_error_t rc = BDMF_ERR_OK;
    int ret = 0;
   
    tm_key.dir = ptm->dir;
    tm_key.index = ptm->tm_id;
    if ((rc = rdpa_egress_tm_get(&tm_key, &sched)))
    {
        CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: tm(%u) rc(%d)", 
            ptm->tm_id, rc);
        ret = RDPA_DRV_TM_GET;
        goto llid_q_cfg_exit;
    }
    
    queue_cfg.queue_id           = ptm->q_id;
    queue_cfg.drop_alg           = rdpa_tm_drop_alg_dt;
    queue_cfg.drop_threshold     = ptm->qsize;
    queue_cfg.weight             = ptm->weight;
    queue_cfg.red_high_threshold = 0; 
    queue_cfg.red_low_threshold  = 0;

    CMD_TM_LOG_DEBUG("rdpa_egress_tm_queue_cfg_set(): "
        "tm(%u) q_id(%u) drop_alg(%u) drop_tresh(%u) weight(%u)",
        ptm->tm_id, queue_cfg.queue_id, queue_cfg.drop_alg, 
        queue_cfg.drop_threshold, queue_cfg.weight);
    
    if ((rc = rdpa_egress_tm_queue_cfg_set(sched, ptm->index, &queue_cfg)))
    {
        CMD_TM_LOG_ERROR("rdpa_egress_tm_queue_cfg_set() failed: rc(%d)", rc);
        ret = RDPA_DRV_Q_CFG_SET;
        goto llid_q_cfg_exit;
    }            
            
llid_q_cfg_exit:
    if (sched)
        bdmf_put(sched);
    return ret;
}

static int q_config(
            rdpa_drv_ioctl_tm_t *ptm)
{
    int ret = 0;
    switch (ptm->dev_type)
    {
        case RDPA_IOCTL_DEV_PORT:
            ret = port_q_cfg(ptm);
            break;
        case RDPA_IOCTL_DEV_LLID:
        case RDPA_IOCTL_DEV_NONE:
            ret = llid_q_cfg(ptm);
            break;
        default:
            ret = RDPA_DRV_ERROR;
            break;
    }      
    return ret;
}

int remove_q_from_wrr_sub(bdmf_object_handle wrr_sub, bdmf_number index, rdpa_traffic_dir traffic_dir)
{
    rdpa_egress_tm_key_t  egress_tm_key;
    bdmf_object_handle sub;
    int rc;

    egress_tm_key.dir = traffic_dir;
    egress_tm_key.index = index;

    if ((rc = rdpa_egress_tm_get(&egress_tm_key, &sub)))
    {
        CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: index=%d, rc(%d)", (int)index, rc);
        return rc;
    }

    CMD_TM_LOG_DEBUG(" rdpa_egress_tm_get() got sub=%p. (wrr_sub=%p)", sub, wrr_sub);
    bdmf_put(sub);
    rc =  bdmf_destroy(sub);
    if (!rc)
    {
      int i;

      //vacate the subsidiary place
      for (i=0; i < MAX_Q_PER_TM; i++)
      {
        if (wrr_tm.subs[i] == index)
        {
            wrr_tm.subs[i] = VACANT;
            break;
        }
      }
      --wrr_tm.q_num;
    }

    return rc;
}


int create_root_subsidiary(bdmf_object_handle root_sched, bdmf_index  ind, rdpa_tm_sched_mode arbiter_mode, bdmf_object_handle *psubsidiary)
{
    BDMF_MATTR(sched_attrs, rdpa_egress_tm_drv());
    rdpa_drv_ioctl_tm_t tm = {};
    int rc;

    tm.arbiter_mode = arbiter_mode;
    
    if (arbiter_mode == rdpa_tm_sched_disabled)
      tm.level = rdpa_tm_level_queue;
    else 
      tm.level = rdpa_tm_level_egress_tm;
    tm.dir = rdpa_dir_us;

    if ((rc = tm_set(sched_attrs, &tm, &tm.tm_id, root_sched, psubsidiary))) 
    {
        CMD_TM_LOG_ERROR("***tm_set() failed: ROOT_tm(%u) rc(%d)", tm.root_tm_id, rc);
        bdmf_put(root_sched);
        return rc;
    }

    CMD_TM_LOG_DEBUG("**: tm_set(): new TM id=%u", tm.tm_id);
              
    if ((rc = rdpa_egress_tm_subsidiary_set(root_sched,  ind, *psubsidiary))) 
    {
        CMD_TM_LOG_ERROR("***rdpa_egress_tm_subsidiary_set() failed: index=%d rc(%d)\n", (int)ind,  rc);
    }
    CMD_TM_LOG_DEBUG(" rdpa_egress_tm_subsidiary ind=%d is set. \n", (int)ind);

    return rc;
}

//substitute root subsidiary:
//delete it and create a new one mode=arbiter_mode 
int substitute_root_subsidiary(bdmf_object_handle root_sched, bdmf_index  ind, rdpa_tm_sched_mode arbiter_mode, bdmf_object_handle *psubsidiary)
{
    bdmf_object_handle subsidiary=NULL;
    int rc;

    rc = rdpa_egress_tm_subsidiary_get(root_sched, ind, &subsidiary);
    if (rc==0)
    {
       bdmf_destroy(subsidiary);
       rc = create_root_subsidiary(root_sched, ind, arbiter_mode, psubsidiary);
    }
    else CMD_TM_LOG_ERROR("***rdpa_egress_tm_subsidiary_get() ind[%d] failed rc(%d)\n", (int)ind,  rc);

    return rc;
}

static
int port_root_tm_get(uint32_t port_id, uint32_t *root_tm_id, BOOL *found)
{
    int ret = 0;

    CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_GET_ROOT_TM: port_id(%d)", port_id);

#ifdef __DUMP_PORTS__
    dump_ports();
#endif

    *found      = FALSE;
    *root_tm_id = 0;

    if (port_id >= MAX_RUT_QOS_PORTS) {
	     CMD_TM_LOG_ERROR("FAIL: port_id(%u) is out of range", port_id);
	     return RDPA_DRV_PORT_ID_NOT_VALID;
	 }

	 if (port_list[port_id].alloc)
    {
        *found      = TRUE;
        *root_tm_id = port_list[port_id].root_tm_id;
    }
    else
    {
        bdmf_object_handle port_obj = NULL;
        bdmf_number        tr69_root_tm_id;
        rdpa_port_tm_cfg_t tm_cfg;
        bdmf_error_t       rc = BDMF_ERR_OK;

        /* looking for RootTM set by TR69 */
        if ((rc = rdpa_port_get(port_id, &port_obj)))
        {
            CMD_TM_LOG_ERROR("rdpa_port_get FAILED: rc(%d)", rc);
            ret = RDPA_DRV_PORT_GET;
            goto port_root_tm_exit;
        }

        if ((rc = rdpa_port_tm_cfg_get(port_obj, &tm_cfg)))
        {
            CMD_TM_LOG_ERROR("rdpa_port_tm_cfg_get FAILED rc=%d", rc);
            ret = RDPA_DRV_TM_CFG_GET;
            goto port_root_tm_exit;
        }

        if ((rc = rdpa_egress_tm_index_get(tm_cfg.sched, &tr69_root_tm_id)))
        {
            CMD_TM_LOG_INFO("**: RDPA_IOCTL_TM_CMD_GET_ROOT_TM: ROOT NOT FOUND");
            ret = 0; //this is not an error, just Root TM is not set yet!
        }
        else 
        {
            rdpa_tm_sched_mode mode;

            *found = TRUE;
            *root_tm_id = tr69_root_tm_id;
            CMD_TM_LOG_INFO("**: RDPA_IOCTL_TM_CMD_GET_ROOT_TM: ROOT TM id=[%u]", *root_tm_id);
            rdpa_egress_tm_mode_get(tm_cfg.sched, &mode);
            ret = add_root_tm_to_port(port_id, *root_tm_id, mode);
        }

port_root_tm_exit:
        if (port_obj)
            bdmf_put(port_obj);
    }
    
    return ret;
}

static
int llid_root_tm_get(uint32_t llid_id, uint32_t *root_tm_id, BOOL *found)
{
    int ret = 0;
    bdmf_object_handle   llid_obj = NULL;
    bdmf_object_handle   tm_obj = NULL;
    bdmf_number tmp_id;
    bdmf_error_t rc = BDMF_ERR_OK;
    
    *found = FALSE;
    if ((rc = rdpa_llid_get(llid_id, &llid_obj)))
    {
        CMD_TM_LOG_ERROR("rdpa_llid_get FAILED: llid(%u) rc(%d)", llid_id, rc);
        ret = RDPA_DRV_LLID_GET;
        goto llid_root_tm_exit;
    }

    if ((rc = rdpa_llid_egress_tm_get(llid_obj, &tm_obj)))
    {
        CMD_TM_LOG_ERROR("rdpa_llid_egress_tm_get FAILED: llid(%u) rc(%d)", llid_id, rc);
        ret = RDPA_DRV_LLID_TM_GET;
        goto llid_root_tm_exit;
    }

    if (!tm_obj)
    {
        goto llid_root_tm_exit;
    }
    
    if ((rc = rdpa_egress_tm_index_get(tm_obj, &tmp_id)))
    {
        CMD_TM_LOG_ERROR("rdpa_egress_tm_index_get FAILED: rc(%d)", rc);
        ret = RDPA_DRV_LLID_TM_ID_GET;
        goto llid_root_tm_exit;
    }

    *root_tm_id = tmp_id;
    *found = TRUE;
    CMD_TM_LOG_DEBUG("get llid(%u) root tm successfully", llid_id);

llid_root_tm_exit:
    if (llid_obj)
        bdmf_put(llid_obj);
    return ret;
}

static
int dev_root_tm_get(uint32_t dev_type, uint32_t dev_id, uint32_t *root_tm_id, BOOL *found)
{       
    int ret = 0;
    switch (dev_type)
    {
        case RDPA_IOCTL_DEV_PORT:
            ret = port_root_tm_get(dev_id, root_tm_id, found);
            break;
        case RDPA_IOCTL_DEV_LLID:
            ret = llid_root_tm_get(dev_id, root_tm_id, found);
            break;
        default:
            ret = RDPA_DRV_ERROR;
            break;
    }
    return ret;
}

/*******************************************************************************/
/* global routines                                                             */
/*******************************************************************************/

/*******************************************************************************
 *
 * Function: rdpa_cmd_tm_ioctl
 *
 * IOCTL interface to the RDPA TM API.
 *
 *******************************************************************************/
int rdpa_cmd_tm_ioctl(unsigned long arg)
{
    rdpa_drv_ioctl_tm_t *userTm_p = (rdpa_drv_ioctl_tm_t *)arg;
    rdpa_drv_ioctl_tm_t tm;
    bdmf_object_handle sched = NULL;
    bdmf_object_handle root_sched = NULL;
    bdmf_object_handle    subsidiary=NULL;
    rdpa_egress_tm_key_t  egress_tm_key;
    bdmf_error_t rc = BDMF_ERR_OK;
    int ret = 0;

    copy_from_user(&tm, userTm_p, sizeof(rdpa_drv_ioctl_tm_t));

    CMD_TM_LOG_DEBUG("RDPA TM CMD(%d)", tm.cmd);

    bdmf_lock();

    switch(tm.cmd)
    {
        case RDPA_IOCTL_TM_CMD_GET_ROOT_TM: 
        {
            CMD_TM_LOG_DEBUG("*** RDPA_IOCTL_TM_CMD_GET_ROOT_TM: ROOT_tm(%u) dev_type(%u) dev_id(%u)", tm.root_tm_id, tm.dev_type, tm.dev_id);
            if ((ret = dev_root_tm_get(tm.dev_type, tm.dev_id, &tm.root_tm_id, &tm.found)))
                CMD_TM_LOG_ERROR("dev_root_tm_get() failed: dev_type(%u) dev_id(%u) rc(%d)",tm.dev_type, tm.dev_id, ret);

            break;
        }
        case RDPA_IOCTL_TM_CMD_GET_ROOT_SP_TM: {

            port_st *pport = NULL;
            
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_GET_ROOT_SP_TM: dev_id(%d)", tm.dev_id);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

	         tm.tm_id = INVALID_ID;
            tm.found = FALSE;
            
	         if (tm.dev_id >= MAX_RUT_QOS_PORTS) {
		          CMD_TM_LOG_ERROR("FAIL: dev_id(%u) is out of range", tm.dev_id);
		          ret = RDPA_DRV_PORT_ID_NOT_VALID;
                break;
	         }

	         pport = &port_list[tm.dev_id];
	         if (!pport->alloc) {
		          CMD_TM_LOG_DEBUG("dev_id(%u) NOT allocated.", tm.dev_id);
                break;
	         }
         
            tm.tm_id = pport->root_sp_tm_id;
            if (tm.tm_id != INVALID_ID)
                tm.found = TRUE;

            break;
        }
        case RDPA_IOCTL_TM_CMD_GET_ROOT_WRR_TM: {

            port_st *pport = NULL;
            
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_GET_ROOT_WRR_TM: dev_id(%d)", tm.dev_id);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

	         tm.tm_id = INVALID_ID;
            tm.found = FALSE;
            
	         if (tm.dev_id >= MAX_RUT_QOS_PORTS) {
		          CMD_TM_LOG_ERROR("FAIL: dev_id(%u) is out of range", tm.dev_id);
		          ret = RDPA_DRV_PORT_ID_NOT_VALID;
                break;
	         }

	         pport = &port_list[tm.dev_id];
	         if (!pport->alloc) {
		          CMD_TM_LOG_DEBUG("dev_id(%u) NOT allocated.", tm.dev_id);
                break;
	         }
         
            tm.tm_id = pport->root_wrr_tm_id;
            if (tm.tm_id != INVALID_ID)
                tm.found = TRUE;

            break;
        }
        case RDPA_IOCTL_TM_CMD_GET_PORT_ORL: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_GET_PORT_ORL: port_id(%d)", tm.dev_id);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

            tm.tm_id        = INVALID_ID;
            tm.shaping_rate = 0;
            tm.orl_linked   = FALSE;
            tm.found        = FALSE;

            if ((ret = get_orl(tm.dev_id, tm.dir,
                               &tm.tm_id, &tm.shaping_rate, &tm.orl_linked))) {
               CMD_TM_LOG_ERROR("get_orl() failed: port_id(%u) rc(%d)", tm.dev_id, ret);
               break;
            }

            if (tm.tm_id != INVALID_ID)
               tm.found = TRUE;

            break;
        }
        case RDPA_IOCTL_TM_CMD_TM_Q_CONFIG: 
        {
            rdpa_tm_rl_cfg_t rl_cfg;
            rdpa_tm_sched_mode arbiter_mode;
            bdmf_index q_index = tm.priority;

            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_TM_Q_CONFIG: dir(%s)", (tm.dir == rdpa_dir_us)?"US":"DS");

#ifdef __DUMP_PORTS__
            dump_ports();
#endif
            egress_tm_key.dir = tm.dir;
            egress_tm_key.index = tm.root_tm_id; //it's tr69_root_tm_id;

            if ((rc = rdpa_egress_tm_get(&egress_tm_key, &root_sched))) 
            {
                CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: ROOT_tm(%u) rc(%d)", tm.root_tm_id, rc);
                ret = RDPA_DRV_TM_GET;
                goto ioctl_exit;
            }
            CMD_TM_LOG_INFO("Got ROOT SHCED = %p", root_sched);

            //get a subsidiary for choosen queue 
            rc = rdpa_egress_tm_subsidiary_get(root_sched, q_index, &subsidiary);
            if (rc!=0)
            {
                bdmf_object_handle wrr_sub=NULL;
                subsidiary = NULL;

                //No subsidiary - check if Root TM subsidiary #WRR_SUB is actually set as WRR mode
                if (get_wrr_sub(root_sched, &wrr_sub) == 0)
                {
                    //THERE IS a WRR Subsidiary on ROOT

                    if ((rc=remove_q_from_wrr_sub(wrr_sub, tmw_id[tm.weight], tm.dir)))
                      CMD_TM_LOG_ERROR("**: remove_q_from_wrr_sub #%d FAILED", tm.priority);

                    if ((create_root_subsidiary(root_sched, tm.priority, rdpa_tm_sched_disabled, &subsidiary)))
                      CMD_TM_LOG_ERROR("**: create_root_subsidiary #%d FAILED", tm.priority);
                }
                else {
                  CMD_TM_LOG_ERROR("rdpa_egress_tm_subsidiary_get FAILED for subsidiary[%d]. ret=%d", (int)q_index, rc);
                  ret = RDPA_DRV_TM_GET;
                  goto ioctl_exit;
                }
            }

            if ((tm.arbiter_mode == rdpa_tm_sched_wrr) && (port_list[tm.dev_id].arbiter_mode == rdpa_tm_sched_sp))
            {
                //We set WRR queue on SP root. That requires to have an WRR ETM on subsidiary #WRR_SUB.
                //Do we have it?
                bdmf_object_handle wrr_subsidiary=NULL;
                bdmf_object_handle q_sched;
                bdmf_object_handle tmp_subs=NULL;
                BDMF_MATTR(sched_attrs, rdpa_egress_tm_drv());
                rdpa_tm_queue_cfg_t queue_cfg = {};
                int free_subs_num;

                CMD_TM_LOG_DEBUG("*** It's WRR with SP root.");

                if (get_wrr_sub(root_sched, &wrr_subsidiary) != 0)
                {
                   //We do NOT have a WRR Subsidiary on ROOT: let's make it   
                   if (substitute_root_subsidiary(root_sched, WRR_SUB, rdpa_tm_sched_wrr, &wrr_subsidiary))
                     CMD_TM_LOG_ERROR("create_root_subsidiary[%d] mode=WRR FAILED", (int)WRR_SUB);
                }

                //Now we have a required WRR subsidiary, time to set a queue (WRR) 
                tm.level = rdpa_tm_level_queue;
                tm.arbiter_mode = rdpa_tm_sched_disabled; 
                tm.dir = rdpa_dir_us;

                //set queue tm
                if ((ret = tm_set(sched_attrs, &tm, &tm.tm_id, wrr_subsidiary, &q_sched))) 
                {
                    CMD_TM_LOG_ERROR("***tm_set() failed...");
                    bdmf_put(root_sched);
                    ret = RDPA_DRV_Q_CFG_SET;
					goto ioctl_exit;
                }
                
                tmw_id[tm.weight] = tm.tm_id;
                free_subs_num = get_free_subsidiary(&wrr_tm);
                CMD_TM_LOG_DEBUG("*** get_free_subsidiary() : got %d", free_subs_num);
                if (free_subs_num < 0)
                {
                    bdmf_put(root_sched);
                    ret = RDPA_DRV_Q_CFG_SET;
					goto ioctl_exit;
                }

                //mark subsidiary as busy (set tm.id)
                wrr_tm.subs[free_subs_num] = tm.tm_id;
                if ((rc = rdpa_egress_tm_weight_set(q_sched, tm.weight)))
                {
                    CMD_TM_LOG_ERROR("*** rdpa_egress_tm_weight_set() failed. rc(%d)", rc);
                    bdmf_put(root_sched);
                    ret =  RDPA_DRV_Q_CFG_SET;
					goto ioctl_exit;
                }

                if ((rc = rdpa_egress_tm_subsidiary_set(wrr_subsidiary, free_subs_num, q_sched))) 
                {
                    CMD_TM_LOG_ERROR("***rdpa_egress_tm_subsidiary_set() failed: ind[%d] rc[%d]", (int)tm.priority, rc);
                    bdmf_put(root_sched);
                    ret = RDPA_DRV_Q_CFG_SET;
					goto ioctl_exit;                    
                }

                //delete SP TM. We would not be here if this TM was set as SP intentionally.
                rc = rdpa_egress_tm_subsidiary_get(root_sched, WRR_SUB - wrr_tm.q_num, &tmp_subs);
                if (rc)
                {
                    CMD_TM_LOG_ERROR("***rdpa_egress_tm_subsidiary_set() failed: ind[%d] rc[%d]", (int)WRR_SUB - wrr_tm.q_num, rc);
                    bdmf_put(root_sched);
                    ret = RDPA_DRV_Q_CFG_SET;
					goto ioctl_exit;
                }

                //do not destroy very first TM (which is WRR)
                if (wrr_tm.q_num > 0)
                {
                  bdmf_destroy(tmp_subs);
                }
                
                //now configure queue. 
                queue_cfg.queue_id           = QUEUE_ID(tm.q_id);
                queue_cfg.drop_alg           = rdpa_tm_drop_alg_dt;
                queue_cfg.drop_threshold     = QUEUE_SIZE;
                queue_cfg.weight             = 0;
                queue_cfg.red_high_threshold = 0; //Relevant for ::rdpa_tm_drop_alg_red/::rdpa_tm_drop_alg_wred : packet threshold
                queue_cfg.red_low_threshold  = 0;

                CMD_TM_LOG_DEBUG(" rdpa_egress_tm_queue_cfg_set(): tm(%u) q_id(%u) drop_alg(%u) drop_tresh(%u) weight(%u)",
                    tm.tm_id, queue_cfg.queue_id, queue_cfg.drop_alg, queue_cfg.drop_threshold, queue_cfg.weight);

                if ((rc = rdpa_egress_tm_queue_cfg_set(q_sched, 0/*Index is always 0: one queue per subsidiary*/ , &queue_cfg))) 
                {
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_queue_cfg_set() failed: rc(%d)", rc);
                    bdmf_put(root_sched);
                    ret = RDPA_DRV_Q_CFG_SET;
					goto ioctl_exit;
                }

                //increase WRR queues number
                ++wrr_tm.q_num;
            }
            else { /* simple case WRR root /gpon*/
               CMD_TM_LOG_DEBUG("*** Got ROOT.SUBSIDIARY[%d] = %p", tm.priority, subsidiary);

               arbiter_mode = port_list[tm.dev_id].arbiter_mode;
               CMD_TM_LOG_DEBUG("**: mode(%u)[SP=1; WRR=2]", arbiter_mode);

               if (arbiter_mode == rdpa_tm_sched_sp)
               {
                 rl_cfg.af_rate = tm.shaping_rate * 1000;     /* Best Effort: shaping_rate is in kbit/s: 1 kilobit = 1000 bits */
                 rc = rdpa_egress_tm_rl_set(subsidiary, &rl_cfg);
                 if (rc != 0)
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_rl_set FAILED");
               }
               else if (arbiter_mode == rdpa_tm_sched_wrr)
               {
                 rc = rdpa_egress_tm_weight_set(subsidiary, tm.weight);
                 if (rc != 0)
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_weight_set() failed. rc(%d)", rc);
               }
           }
            
           bdmf_put(root_sched);
           break;
        }

        case RDPA_IOCTL_TM_CMD_TM_Q_REMOVE: 
        {
            bdmf_index q_index = tm.priority;
            bdmf_object_handle wrr_subsidiary=NULL;
            rdpa_tm_sched_mode arbiter_mode = tm.arbiter_mode;
            rdpa_tm_rl_cfg_t rl_cfg;

            //bdmf_object_handle q_sched;

            CMD_TM_LOG_DEBUG(" RDPA_IOCTL_TM_CMD_TM_Q_REMOVE: q_index=%d (it's tm.priority)",    (int)q_index);

            egress_tm_key.dir = tm.dir;
            egress_tm_key.index = tm.root_tm_id; //it's tr69_root_tm_id;

            if ((rc = rdpa_egress_tm_get(&egress_tm_key, &root_sched))) 
            {
                CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: ROOT_tm(%u) rc(%d)", tm.root_tm_id, rc);
                ret = RDPA_DRV_TM_GET;
                goto ioctl_exit;
            }

            //get a subsidiary for choosen queue 
            rc = rdpa_egress_tm_subsidiary_get(root_sched, q_index, &subsidiary);
            if (rc!=0)
            {
                  CMD_TM_LOG_ERROR("rdpa_egress_tm_subsidiary_get FAILED for subsidiary[%d]. ret=%d", (int)q_index, rc);
                  ret = RDPA_DRV_TM_GET;
                  goto ioctl_exit;
            }

            if (arbiter_mode == rdpa_tm_sched_wrr)
            { /* remove WRR queue */
              if (get_wrr_sub(root_sched, &wrr_subsidiary) == 0)
              {
                 remove_q_from_wrr_sub(wrr_subsidiary, wrr_tm.subs[tm.q_id -1], tm.dir);
                 if (wrr_tm.q_num != 0)
                 {
                    bdmf_object_handle q_sched;
                    rdpa_tm_queue_cfg_t queue_cfg = {0};
                    int sub_num = WRR_SUB - wrr_tm.q_num;

                    // the removed WRR queue was created instead of SP queue. Restore 'original' SP queue
                    CMD_TM_LOG_DEBUG("*** Restory SP queue (q-id=[%d]) on subsidiary [%d] (WRR - wrr_tm.q_num)", tm.q_id, sub_num);
                    rc = create_root_subsidiary(root_sched, sub_num, rdpa_tm_sched_disabled, &q_sched);

                    //configure the queue
                    queue_cfg.queue_id           = QUEUE_ID(tm.q_id);
                    queue_cfg.drop_alg           = rdpa_tm_drop_alg_dt;
                    queue_cfg.drop_threshold     = QUEUE_SIZE;
                    //queue_cfg.weight             = 0;
                    //queue_cfg.red_high_threshold = 0; 
                    //queue_cfg.red_low_threshold  = 0;

                    CMD_TM_LOG_DEBUG("*** rdpa_egress_tm_queue_cfg_set(): tm(%u) q_id(%u) drop_alg(%u) drop_tresh(%u) weight(%u)",
                                        tm.tm_id, queue_cfg.queue_id, queue_cfg.drop_alg, queue_cfg.drop_threshold, queue_cfg.weight);

                    if ((rc = rdpa_egress_tm_queue_cfg_set(q_sched, 0/*Index is always 0: one queue per subsidiary*/ , &queue_cfg))) 
                    {
                       CMD_TM_LOG_ERROR("rdpa_egress_tm_queue_cfg_set() failed: rc(%d)", rc);
                       bdmf_put(root_sched);
                       ret = RDPA_DRV_Q_CFG_SET;
					   goto ioctl_exit;
                    }
                 }
              }
            }
            else if (tm.arbiter_mode == rdpa_tm_sched_sp)
            {
               rl_cfg.af_rate = 0;     //no limit 
               rc = rdpa_egress_tm_rl_set(subsidiary, &rl_cfg);
               if (rc != 0)
                  CMD_TM_LOG_ERROR("rdpa_egress_tm_rl_set FAILED");
            }
            break;
        }
        case RDPA_IOCTL_TM_GET_BY_QID: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_GET_BY_QID: ROOT_tm(%u) dev_type(%d) dev_id(%d) q_id(%u)", 
                tm.root_tm_id, tm.dev_type, tm.dev_id, tm.q_id);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif
            tm.found = FALSE;

            if ((ret = get_tm_by_qid(tm.dev_type, tm.dev_id, tm.q_id,
                                     &tm.tm_id, &tm.found))) {
                CMD_TM_LOG_ERROR("get_tm_by_qid() failed: dev_type(%d) dev_id(%u) q_id(%u) ret(%d)",
                                 tm.dev_type, tm.dev_id, tm.q_id, ret);
                goto ioctl_exit;
            }
            break;
        }
        case RDPA_IOCTL_TM_CMD_GET_QUEUE_CONFIG: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_GET_QUEUE_CONFIG: dev_type(%d) dev_id(%d) q_id(%u)", 
                tm.dev_type, tm.dev_id, tm.q_id);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif
            tm.found = FALSE;

            if ((ret = get_q_cfg(tm.dev_type, tm.dev_id, tm.q_id, tm.dir,
                                 &tm.tm_id,
                                 &tm.qsize,
                                 &tm.weight,
                                 &tm.shaping_rate,
                                 &tm.found))) {
                CMD_TM_LOG_ERROR("get_q_cfg() failed: dev_type(%d) dev_id(%u) q_id(%u) ret(%d)", 
                                 tm.dev_type, tm.dev_id, tm.q_id, ret);
                goto ioctl_exit;
            }
            break;
        }
        case RDPA_IOCTL_TM_CMD_ROOT_TM_CONFIG: {

            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_ROOT_TM_CONFIG: dev_type(%u) dev_id(%u) dir(%s)",
                tm.dev_type, tm.dev_id, (tm.dir == rdpa_dir_us)?"US":"DS");

            if ((ret = root_tm_config(&tm, &sched))) {
                CMD_TM_LOG_ERROR("root_tm_config() failed: dev_type(%u) dev_id(%u) ret(%d)", tm.dev_type, tm.dev_id, ret);
                goto ioctl_exit;
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_TM_CONFIG: {
            if (tm.dev_type == RDPA_IOCTL_DEV_NONE){
                rdpa_egress_tm_key_t egress_tm_key;

                CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_TM_CONFIG: root_tm(%u) dev_type(%u) sub_tm(%u) dir(%s)",
                    tm.root_tm_id, tm.dev_type, tm.tm_id, (tm.dir == rdpa_dir_us)?"US":"DS");


                egress_tm_key.dir = tm.dir;
                egress_tm_key.index = tm.root_tm_id;

                if ((rc = rdpa_egress_tm_get(&egress_tm_key, &root_sched))) {
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: ROOT_tm(%u) rc(%d)", tm.root_tm_id, rc);
                    ret = RDPA_DRV_TM_GET;
                    goto ioctl_exit;
                }

                egress_tm_key.dir = tm.dir;
                egress_tm_key.index = tm.tm_id;

                if ((rc = rdpa_egress_tm_get(&egress_tm_key, &sched))) {
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: sub_tm(%u) rc(%d)", tm.tm_id, rc);
                    bdmf_put(root_sched);
                    ret = RDPA_DRV_TM_GET;
                    goto ioctl_exit;
                }

                if ((rc = rdpa_egress_tm_subsidiary_set(root_sched,  tm.priority, sched))) {
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_subsidiary_set() failed: rootId(%u) rc(%d)", tm.root_tm_id, rc);
                    bdmf_put(root_sched);
                    bdmf_put(sched);
                    ret = RDPA_DRV_SUBS_SET;
                    goto ioctl_exit;
                }

                bdmf_put(root_sched);
                bdmf_put(sched);
                
                break;
            }
            else {
                BDMF_MATTR(sched_attrs, rdpa_egress_tm_drv());
                rdpa_egress_tm_key_t egress_tm_key;

                CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_TM_CONFIG: root_tm(%u) dev_type(%u) dev_id(%u) dir(%s)",
                tm.root_tm_id, tm.dev_type, tm.dev_id, (tm.dir == rdpa_dir_us)?"US":"DS");

#ifdef __DUMP_PORTS__
                dump_ports();
#endif

                egress_tm_key.dir = tm.dir;
                egress_tm_key.index = tm.root_tm_id;

                if ((rc = rdpa_egress_tm_get(&egress_tm_key, &root_sched))) {
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: ROOT_tm(%u) rc(%d)", tm.root_tm_id, rc);
                    ret = RDPA_DRV_TM_GET;
                    goto ioctl_exit;
                }

                if ((ret = tm_set(sched_attrs, &tm, &tm.tm_id, root_sched, &sched))) {
                    CMD_TM_LOG_ERROR("tm_set() failed: ROOT_tm(%u) rc(%d)", tm.root_tm_id, rc);
                    bdmf_put(root_sched);
                    goto ioctl_exit;
                }

                if (tm.arbiter_mode == rdpa_tm_sched_disabled)
                {
                    rdpa_tm_rl_cfg_t rl_cfg = {};
                    
                    rdpa_egress_tm_weight_set(sched, tm.weight);

                    rl_cfg.af_rate = tm.shaping_rate * 1000;
                    CMD_TM_LOG_DEBUG("SET tm.shaping_rate=[%d]\n", rl_cfg.af_rate);
                    rc = rdpa_egress_tm_rl_set(sched, &rl_cfg);
    			   }
                
                if ((rc = rdpa_egress_tm_subsidiary_set(root_sched,  tm.priority, sched))) {
                    CMD_TM_LOG_ERROR("rdpa_egress_tm_subsidiary_set() failed: rootId(%u) rc(%d)", tm.root_tm_id, rc);
                    bdmf_put(root_sched);
                    bdmf_destroy(sched);
                    ret = RDPA_DRV_SUBS_SET;
                    goto ioctl_exit;
                }

                bdmf_put(root_sched);

                if (tm.dev_type == RDPA_IOCTL_DEV_PORT)
                {
                    port_st *pport = NULL;
                    
    	             if (tm.dev_id >= MAX_RUT_QOS_PORTS) {
    		              CMD_TM_LOG_ERROR("FAIL: port_id(%u) is out of range", tm.dev_id);
                        ret = RDPA_DRV_PORT_ID_NOT_VALID;
                        goto ioctl_exit;
    	             }

                    pport = &port_list[tm.dev_id];
                    if (pport->alloc) {
                        /* If the parent of this tm is the root tm, then this tm must be
                         * either the root sp tm or the root wrr tm. Save the tm id to
                         * the port list.
                         */ 
                        if (tm.root_tm_id == pport->root_tm_id) {
                            if (tm.arbiter_mode == rdpa_tm_sched_sp)
                                pport->root_sp_tm_id = tm.tm_id;
                            else if (tm.arbiter_mode == rdpa_tm_sched_wrr)
                                pport->root_wrr_tm_id = tm.tm_id;
                        }
                    }
                }

                break;
            }
        }
        case RDPA_IOCTL_TM_CMD_ROOT_TM_REMOVE: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_ROOT_TM_REMOVE: tm(%u)", tm.root_tm_id);

            if ((ret = root_tm_remove(tm.dev_type, tm.dev_id, tm.root_tm_id, tm.dir))) {
                CMD_TM_LOG_ERROR("root_tm_remove() failed: ROOT_tm(%u) ret(%d)", tm.root_tm_id, ret);
                goto ioctl_exit;
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_TM_REMOVE: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_TM_REMOVE: tm(%u)", tm.tm_id);

            if ((ret = tm_remove(&tm)))
                CMD_TM_LOG_ERROR("tm_remove() failed: tm(%u) rc(%d)", tm.tm_id, ret);

            break;
        }
        case RDPA_IOCTL_TM_CMD_QUEUE_CONFIG: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_QUEUE_CONFIG: dev_type(%u) dev_id(%u) tm(%u) dir(%s) q_id(%u) index(%u)",
                tm.dev_type, tm.dev_id, tm.tm_id, (tm.dir == rdpa_dir_us)?"US":"DS", tm.q_id, tm.index);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

            if ((ret = q_config(&tm))) {
                CMD_TM_LOG_ERROR("q_config() failed: dev_type(%u) dev_id(%u) tm_id(%u) q_id(%u) index(%u) ret(%d)", 
                    tm.dev_type, tm.dev_id, tm.tm_id, tm.q_id, tm.index, ret);
                goto ioctl_exit;
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_QUEUE_REMOVE: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_QUEUE_REMOVE: dev_type(%u) dev_id(%u) tm(%u) dir(%s) q_id(%u) index(%u)",
                tm.dev_type, tm.dev_id, tm.tm_id, (tm.dir == rdpa_dir_us)?"US":"DS", tm.q_id, tm.index);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

            if ((ret = q_config(&tm))) {
                CMD_TM_LOG_ERROR("q_config() failed: dev_type(%u) dev_id(%u) tm_id(%u) q_id(%u) index(%u) rc(%d)", 
                    tm.dev_type, tm.dev_id, tm.tm_id, tm.q_id, tm.index, ret);
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_QUEUE_ALLOCATE: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_QUEUE_ALLOCATE: dev_type(%u) dev_id(%u) q_id(%u)",
                tm.dev_type, tm.dev_id, tm.q_id);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

            ret = queue_allocate(tm.dev_type, tm.dev_id, tm.q_id, TRUE);

            break;
        }
        case RDPA_IOCTL_TM_CMD_QUEUE_DISLOCATE: {
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_QUEUE_DISLOCATE: dev_type(%u) dev_id(%u) q_id(%u)",
                tm.dev_type, tm.dev_id, tm.q_id);

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

            ret = queue_allocate(tm.dev_type, tm.dev_id, tm.q_id, FALSE);

            break;
        }
        case RDPA_IOCTL_TM_CMD_ORL_CONFIG: {

            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_ORL_TM_CONFIG: port_id(%u) dir(%s)",
                tm.dev_id, (tm.dir == rdpa_dir_us)?"US":"DS");

            if ((ret = orl_config(&tm))) {
                CMD_TM_LOG_ERROR("orl_config() failed: port_id(%u) rc(%d)", tm.dev_id, rc);
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_ORL_REMOVE: {
            bdmf_error_t rc = BDMF_ERR_OK;

            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_ORL_TM_REMOVE: port_id(%u) orl_tm(%u) dir(%s)",
                tm.dev_id, tm.tm_id, (tm.dir == rdpa_dir_us)?"US":"DS");

#ifdef __DUMP_PORTS__
            dump_ports();
#endif

            if ((ret = orl_remove(tm.dev_id, tm.dir))) {
                CMD_TM_LOG_ERROR("orl_remove() failed: orl_tm(%u) rc(%d)", tm.tm_id, rc);
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_ORL_LINK: {

            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_ORL_LINK: port_id(%u) orl_tm_id(%u)",
                tm.dev_id, tm.tm_id);

            if ((ret = orl_link(&tm))) {
                CMD_TM_LOG_ERROR("orl_config() failed: port_id(%u) orl_tm_id(%u) rc(%d)",
                   tm.dev_id, tm.tm_id, rc);
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_ORL_UNLINK: {

            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_ORL_UNLINK: port_id(%u) orl_tm_id(%u)",
                tm.dev_id, tm.tm_id);

            if ((ret = orl_unlink(&tm))) {
                CMD_TM_LOG_ERROR("orl_config() failed: port_id(%u) orl_tm_id(%u) rc(%d)",
                   tm.dev_id, tm.tm_id, rc);
            }

            break;
        }
        case RDPA_IOCTL_TM_CMD_TM_RL_CONFIG: {
   
            rdpa_tm_rl_cfg_t rl_cfg = {};
            
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_TM_RL_CONFIG: tm(%u) dir(%s)", 
                tm.tm_id, (tm.dir == rdpa_dir_us)?"US":"DS");

            egress_tm_key.dir = tm.dir;
            egress_tm_key.index = tm.tm_id;

            if ((rc = rdpa_egress_tm_get(&egress_tm_key, &sched)))
            {
                CMD_TM_LOG_ERROR("rdpa_egress_tm_get() failed: tm(%u) rc(%d)", tm.tm_id, rc);
                ret = RDPA_DRV_TM_GET;
                goto ioctl_exit;
            }
            
            rl_cfg.af_rate = tm.shaping_rate * 1000;    /* Best Effort: shaping_rate is in kbit/s: 1 kilobit = 1000 bits */
            rl_cfg.be_rate = 0;
            rl_cfg.burst_size = tm.burst * 1000; /* kbits -> bps */

            if ((rc = rdpa_egress_tm_rl_set(sched, &rl_cfg))) 
            {
                CMD_TM_LOG_ERROR("rdpa_egress_tm_rl_set() failed: tm(%u) af(%u) rc(%d)", tm.tm_id, rl_cfg.af_rate, rc);
                bdmf_put(sched);
                ret = RDPA_DRV_Q_RATE_SET;
                goto ioctl_exit;
            }
            bdmf_put(sched);    
            break;
        }
        case RDPA_IOCTL_TM_CMD_GET_PORT_TM: {

           if (tm.dev_type != RDPA_IOCTL_DEV_PORT) {
            
              ret = RDPA_DRV_ERROR;
              CMD_TM_LOG_ERROR("not implement for such dev type(%u) still", tm.dev_type);
              break;
           }
           
           CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_GET_PORT_TM: port_id(%d)", tm.dev_id);
#ifdef __DUMP_PORTS__
           dump_ports();
#endif
           if ((ret = get_port_tm_parms(tm.dev_id,
                                        tm.dir,
                                        &tm.port_sched_type,
                                        &tm.max_queues,
                                        &tm.max_sp_queues,
                                        &tm.port_shaper,
                                        &tm.queue_shaper))) {
              tm.found = FALSE;
              if (ret == RDPA_DRV_PORT_NOT_ALLOC) {
                 ret = 0; /* Hide this error from upper layer */
              } else {
                 CMD_TM_LOG_ERROR("get_port_tm_parms() failed: port_id(%u) rc(%d)", tm.dev_id, ret);
              }
           }
           else {
              tm.found = TRUE;
           }
           break;
        }
        case RDPA_IOCTL_TM_CMD_GET_QUEUE_STATS: {
           
            CMD_TM_LOG_INFO("RDPA_IOCTL_TM_CMD_GET_QUEUE_STATS: dev_type(%d) dev_id(%d) q_id(%u)", 
                            tm.dev_type, tm.dev_id, tm.q_id);

            if ((ret = get_q_stats(tm.dev_type, tm.dev_id, tm.q_id, tm.dir, &tm.qstats))) {
                CMD_TM_LOG_ERROR("get_q_stats() failed: dev_type(%d) dev_id(%u) q_id(%u) ret(%d)", 
                                 tm.dev_type, tm.dev_id, tm.q_id, ret);
                goto ioctl_exit;
            }
            break;
        }
        
        default:
            CMD_TM_LOG_ERROR("Invalid IOCTL cmd %d", tm.cmd);
            ret = RDPA_DRV_ERROR;
    }

ioctl_exit:
    if (ret) {
      CMD_TM_LOG_ERROR("rdpa_cmd_tm_ioctl() OUT: FAILED: cmd=%d tm(%u) rc(%d)", tm.cmd, tm.tm_id, rc);
    }

    bdmf_unlock();

    copy_to_user(userTm_p, &tm, sizeof(rdpa_drv_ioctl_tm_t));

    return ret;
}

/*******************************************************************************
 *
 * Function: rdpa_cmd_tm_init
 *
 * Initializes the RDPA TM API.
 *
 *******************************************************************************/
void rdpa_cmd_tm_init(void)
{
#ifdef __EARLY_CONFIG__
   int q_id = 0;
   rdpa_drv_ioctl_tm_t tm;
   bdmf_object_handle root_sched = NULL;
   int ret = 0;
#endif
   port_st  *pport;
   queue_st *pq;
   int i, j;

   CMD_TM_LOG_DEBUG("RDPA TM INIT");

   memset(&rdpa_cmd_tm_ctrl_g, 0, sizeof(rdpa_cmd_tm_ctrl_t));
   rdpa_cmd_tm_ctrl_g.tm_index = 0;

   /* Initialize the port list. */   
   pport = &port_list[0];
   memset(pport, 0, sizeof(port_list));
   
   for (i = 0; i < MAX_RUT_QOS_PORTS; i++, pport++) {
       pport->root_sp_tm_id = pport->root_wrr_tm_id = pport->orl_id = INVALID_ID;

       pq = &(pport->queue_list[0]);
       for (j = 0; j < MAX_Q_PER_TM; j++, pq++)
           pq->tm_id = INVALID_ID;
   }

#ifdef __EARLY_CONFIG__
    tm.dev_type     = RDPA_IOCTL_DEV_PORT;
    tm.dev_id       = rdpa_if_wan0;
    tm.dir          = rdpa_dir_us;
    tm.arbiter_mode = rdpa_tm_sched_sp;
    tm.level        = rdpa_tm_level_egress_tm;

    if (!(ret = root_tm_config(&tm, &root_sched))) {
        CMD_TM_LOG_INFO("DEFAULT ROOT TM CONFIGURED OK");
        for (q_id = 0; q_id <= 7; q_id++) {
            tm.tm_id = tm.root_tm_id;
            tm.q_id  = q_id;
            tm.index = q_id;
            if ((ret = q_config(&tm))) {
                CMD_TM_LOG_ERROR("q_config() failed: tm_id(%u) q_id(%u) rc(%d)", tm.tm_id, q_id, ret);
                return;
            }
        }
    } else {
        CMD_TM_LOG_INFO("DEFAULT ROOT TM CONFIGURATION FAILED: rc(%d)", ret);
    }
#endif
}

EXPORT_SYMBOL(rdpa_cmd_tm_ioctl);
EXPORT_SYMBOL(rdpa_cmd_tm_init);
