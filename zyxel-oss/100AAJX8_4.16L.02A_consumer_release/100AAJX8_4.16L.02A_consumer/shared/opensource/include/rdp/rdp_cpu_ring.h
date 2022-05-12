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

#ifndef _RDP_CPU_RING_H_
#define _RDP_CPU_RING_H_

#if defined(__KERNEL__) || defined(_CFE_)

/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/
#include<bcm_pkt_lengths.h>

#ifdef _CFE_
#include "bl_os_wraper.h"
#endif
#include "rdpa_types.h"
#include "rdd.h"
#include <bcm_mm.h>


/*****************************************************************************/
/*                                                                           */
/* defines and structures                                                    */
/*                                                                           */
/*****************************************************************************/


#ifdef _CFE_

#include "lib_malloc.h"
#include "cfe_iocb.h"
#define RDP_CPU_RING_MAX_QUEUES	1
#define RDP_WLAN_MAX_QUEUES		0

#elif defined(__KERNEL__)

#include "rdpa_cpu.h"
#include "bdmf_system.h"
#include "bdmf_shell.h"
#include "bdmf_dev.h"

#define RDP_CPU_RING_MAX_QUEUES		RDPA_CPU_MAX_QUEUES
#define RDP_WLAN_MAX_QUEUES			RDPA_WLAN_MAX_QUEUES

extern const bdmf_attr_enum_table_t rdpa_cpu_reason_enum_table;

#endif

typedef enum
{
	sysb_type_skb,
	sysb_type_fkb,
	sysb_type_raw,
}cpu_ring_sysb_type;

typedef enum
{
	type_cpu_rx,
	type_pci_tx
}cpu_ring_type;


typedef struct
{
	uint8_t*                        data_ptr;
	uint16_t                        packet_size;
	uint16_t                        flow_id;
	uint16_t						reason;
	uint16_t					    src_bridge_port;
    uint16_t                        dst_ssid;
    uint16_t						wl_metadata; 
    uint16_t                        ptp_index;    
}
CPU_RX_PARAMS;

typedef void* (sysbuf_alloc_cb)(cpu_ring_sysb_type, uint32_t *pDataPtr);
typedef void (sysbuf_free_cb)(void* packetPtr);

typedef void* t_sysb_ptr;

typedef enum
{
	OWNERSHIP_RUNNER,
	OWNERSHIP_HOST
}E_DESCRIPTOR_OWNERSHIP;

#ifndef _BYTE_ORDER_LITTLE_ENDIAN_
typedef struct
{
	union{
		uint32_t word0;
		struct{
			uint32_t	flow_id:12;
			uint32_t	reserved0:1;
			uint32_t	source_port:5;
			uint32_t	packet_length:14;
		};
	};
	union{
		uint32_t word1;
		struct{
			uint32_t	payload_offset_flag:1;
			uint32_t	reason:6;
			uint32_t	dst_ssid:16;
			uint32_t	reserved1:5;
			uint32_t	descriptor_type:4;
			//uint32_t	abs_flag:1;
			//uint32_t	flow_id:8;

		};
	};
	union{
		uint32_t word2;
		struct{
			uint32_t	ownership:1;
			uint32_t	reserved2:2;
			uint32_t	host_buffer_data_pointer:29;
		};
	};
	union{
		uint32_t word3;
        struct{
            uint16_t	reserved4:12;
            uint16_t    ip_sync_1588_idx:4;
            union {
		   uint16_t wl_metadata;
		   struct{
		           uint16_t    reserved5:4;
		           uint16_t    wl_tx_priority:4;
			   uint16_t    wl_chain_id:8;
	           };
            };
        };
    };
}
CPU_RX_DESCRIPTOR;
#else
typedef struct
{
	union{
		uint32_t word0;
		struct{
			uint32_t	packet_length:14;
			uint32_t	source_port:5;
			uint32_t	reserved0:1;
			uint32_t	flow_id:12;
			//uint32_t	descriptor_type:4;
			//uint32_t	reserved0:9;
		};
	};

	union{
		uint32_t word1;
		struct{
			//uint32_t	flow_id:8;
			//uint32_t	abs_flag:1;
			uint32_t	descriptor_type:4;
			uint32_t	reserved1:5;
			uint32_t	dst_ssid:16;
			uint32_t	reason:6;
			uint32_t	payload_offset_flag:1;
		};
	};

	union{
		uint32_t word2;
                uint8_t* data_ptr;
		struct{
			uint32_t	host_buffer_data_pointer:31;
			uint32_t	ownership:1;
		};
	};
	union{
		uint32_t word3;
        struct{
            union {
                uint16_t wl_metadata;
                struct {
                    uint16_t	wl_chain_id:8;
                    uint16_t	wl_tx_priority:4;
                    uint16_t    reserved5:4;
                };
            };
            uint16_t    ip_sync_1588_idx:4;
            uint16_t	reserved4:12;
        };
    };
}
CPU_RX_DESCRIPTOR;
#endif



typedef union
{
	CPU_RX_DESCRIPTOR cpu_rx;
	//PCI_TX_DESCRIPTOR pci_tx; not ready yet
}
RING_DESC_UNION;

#define MAX_BUFS_IN_CACHE 32 
typedef struct
{
	uint32_t				ring_id;
	uint32_t				admin_status;
	uint32_t				num_of_entries;
	uint32_t				size_of_entry;
	uint32_t				packet_size;
	cpu_ring_sysb_type		buff_type;
	RING_DESC_UNION*		head;
	RING_DESC_UNION*		base;
	RING_DESC_UNION*		end;
	uint32_t               buff_cache_cnt;
	uint32_t*              buff_cache;
}
RING_DESCTIPTOR;

/*array of possible rings private data*/
#define D_NUM_OF_RING_DESCRIPTORS (RDP_CPU_RING_MAX_QUEUES + RDP_WLAN_MAX_QUEUES)


int rdp_cpu_ring_create_ring(uint32_t ringId,uint32_t entries, cpu_ring_sysb_type buff_type,
							 void *ic_cfg_voidp);

int rdp_cpu_ring_delete_ring(uint32_t ringId);

int rdp_cpu_ring_read_packet_copy(uint32_t ringId, CPU_RX_PARAMS* rxParams);

int rdp_cpu_ring_get_queue_size(uint32_t ringId);

int rdp_cpu_ring_get_queued(uint32_t ringId);

int rdp_cpu_ring_flush(uint32_t ringId);

int rdp_cpu_ring_not_empty(uint32_t ringId);

#endif /* if defined(__KERNEL__) || defined(_CFE_) */

#endif /* _RDP_CPU_RING_H_ */
