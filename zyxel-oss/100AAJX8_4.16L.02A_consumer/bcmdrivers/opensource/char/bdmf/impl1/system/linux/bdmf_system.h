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


#ifndef _BDMF_SYSTEM_H_
#define _BDMF_SYSTEM_H_

/* Linux header files referenced */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/irq.h>
#include <linux/in.h>
#include <linux/random.h>
#include <linux/netdevice.h>
#include <linux/nbuff.h>
#if (defined(CONFIG_BCM_BPM) || defined(CONFIG_BCM_BPM_MODULE))
#include <linux/gbpm.h>
#endif
#ifdef CONFIG_MIPS
#include <asm/r4kcache.h>
#endif
#include <bdmf_queue.h>
#include <bdmf_errno.h>
#include "rdp_drv_bpm.h"

#define __PACKING_ATTRIBUTE_STRUCT_END__ __attribute__ ((packed))
#define __PACKING_ATTRIBUTE_FIELD_LEVEL__

#define BDMF_FALSE 0
#define BDMF_TRUE 1

/*TODO check with Igor ,if we should create a bdmf_sysb_inline.h file
   I think we are uncessarily inlcuding these functions at several palces
   when its not needed
*/

/** \defgroup bdmf_system System Abstraction Layer
 *
 * This module implements a simple system abstraction.
 * BDMF currently supports the following systems
 * - linux kernel
 * - linux user space (simulation)
 * - VxWorks
 * @{
 */

/** \defgroup bdmf_system_mem Dynamic memory allocation
 * \ingroup bdmf_system
 * @{
 */

/** Allocate memory
 * \param[in]   size    block size
 * \returns memory block pointer or NULL
 */
void *bdmf_alloc(size_t size);

/** Allocate memory and clear
 * \param[in]   size    block size
 * \returns memory block pointer or NULL
 */
static inline void *bdmf_calloc(size_t size)
{
    void *p = bdmf_alloc(size);
    if (p)
        memset(p, 0, size);
    return p;
}

/** Release memory block allocated by
 * bdmf_alloc() or bdmf_calloc()
 * \param[in]   p   memory block pointer
 */
void bdmf_free(void *p);

/** @} end of bdmf_system_mem group */

/** \defgroup bdmf_system_bin_mutex Binary mutex
 * \ingroup bdmf_system
 * Task that attempts to take mutex that is already taken bdmfocks,
 * regardless on who holds the mutex.
 * @{
 */

typedef struct { char b[128]; } bdmf_mutex;

/** Initialize binary mutex
 * \param[in]   pmutex  mutex handle
 */
void bdmf_mutex_init(bdmf_mutex *pmutex);

/** Lock binary mutex
 * \param[in]   pmutex  mutex handle
 * The caller blocks until the mutex becomes available
 * \return  0=mutex taken, <0-error (interrupted)
 */
int bdmf_mutex_lock(bdmf_mutex *pmutex);

/** Unlock binary mutex
 * \param[in]   pmutex  mutex handle
 */
void bdmf_mutex_unlock(bdmf_mutex *pmutex);

/** Delete binary mutex
 * \param[in]   pmutex  mutex handle
 */
void bdmf_mutex_delete(bdmf_mutex *pmutex);

/** @} */


/** \defgroup bdmf_system_ta_mutex Task-aware mutex
 * \ingroup bdmf_system
 * The same task can take the same mutex multiple times.
 * However, if task B attempts to take mutex that is already taken by task A,
 * it locks until the mutex becomes available.
 * If task takes task aware mutex multiple times, number of
 * bdmf_ta_mutex_unlock() calls must match bdmf_ta_mutex_lock()
 * @{
 */

typedef struct { char b[128]; } bdmf_ta_mutex;

/** Initialize task-aware mutex
 * \param[in]   pmutex  mutex handle
 */
void bdmf_ta_mutex_init(bdmf_ta_mutex *pmutex);

/** Lock task-aware mutex
 * \param[in]   pmutex  mutex handle
 * If the mutex is taken by different task, the caller blocks
 * \return  0=mutex taken, <0-error (interrupted)
 */
int bdmf_ta_mutex_lock(bdmf_ta_mutex *pmutex);

/** Unlock task-aware mutex
 * \param[in]   pmutex  mutex handle
 */
void bdmf_ta_mutex_unlock(bdmf_ta_mutex *pmutex);

/** Delete task-aware mutex
 * \param[in]   pmutex  mutex handle
 */
void bdmf_ta_mutex_delete(bdmf_ta_mutex *pmutex);

/** @} */

typedef struct { char b[128]; } bdmf_reent_fastlock;

void bdmf_reent_fastlock_init(bdmf_reent_fastlock *lock);
int bdmf_reent_fastlock_lock(bdmf_reent_fastlock *lock);
void bdmf_reent_fastlock_unlock(bdmf_reent_fastlock *lock);

/** \defgroup bdmf_system_fastlock Fast lock
 * \ingroup bdmf_system
 * Fastlock primitives can be used in interrupt context
 * @{
 */
/* Fast lock/unlock */
typedef spinlock_t bdmf_fastlock;

/** Init fast lock
 * \param[in]   plock   Fastlock handle
 */

#define DEFINE_BDMF_FASTLOCK(lock) bdmf_fastlock lock = __SPIN_LOCK_INITIALIZER(lock)

#define bdmf_fastlock_init(plock)               \
    do {                                        \
        spin_lock_init(plock);       \
    } while(0)

/** Take fast lock
 * \param[in]   plock   Fastlock handle
 */
#define bdmf_fastlock_lock(plock)               \
    do { \
        BUG_ON(in_irq()); \
        spin_lock_bh(plock); \
    } while (0)

/** Release fast lock
 * \param[in]   plock   Fastlock handle
 */
#define bdmf_fastlock_unlock(plock)             \
    spin_unlock_bh(plock)

/** Take fast lock in interrupt level
 * \param[in]   plock   Fastlock handle
 * \param[out]   flags   IRQ flags to save
 */
#define bdmf_fastlock_lock_irq(plock, flags)               \
    spin_lock_irqsave(plock, flags)

/** Release fast lock in interrupt level
 * \param[in]   plock   Fastlock handle
 * \param[in]   flags   IRQ flags to restore
 */
#define bdmf_fastlock_unlock_irq(plock, flags)             \
    spin_unlock_irqrestore(plock, flags)

/** @} */

/* Tasks */
typedef struct task_struct *bdmf_task;
int bdmf_task_create(const char *name, int priority, int stack,
    int (*handler)(void *arg), void *arg, bdmf_task *ptask);
#define BDMFSYS_DEFAULT_TASK_PRIORITY     (-1)
#define BDMFSYS_DEFAULT_TASK_STACK        (-1)
int bdmf_task_destroy(bdmf_task task);
#define bdmf_usleep(_us)       usleep(_us)

const char *bdmf_task_get_name(bdmf_task task, char *name);
const bdmf_task bdmf_task_get_current(void);

/** \defgroup bdmf_system_file Printing and file IO
 * \ingroup bdmf_system
 * @{
 */

/** Print
 * \param[in]   format  printf-like format
 */
int bdmf_print(const char *format, ...) __attribute__((format(printf, 1, 2)));


/** Print
 * \param[in]   format  printf-like format
 * \param[in]   args    argument list
 */
int bdmf_vprint(const char *format, va_list args);

/** Print error message
 * \param[in]   format  printf-like format
 * \param[in]   args    arguments
 */
#define bdmf_print_error(format,args...)       bdmf_print("***Error in %s:%d>"format, __FUNCTION__, __LINE__, ## args)\

/* File IO */
typedef struct file *bdmf_file;

#define BDMF_FMODE_RDONLY   0x1     /**< Read-only file mode */
#define BDMF_FMODE_WRONLY   0x2     /**< Write-only file mode */
#define BDMF_FMODE_RDWR     0x3     /**< Read-Write file mode */
#define BDMF_FMODE_CREATE   0x4     /**< Create file if doesn't exist */
#define BDMF_FMODE_APPEND   0x8     /**< Append to existing file */
#define BDMF_FMODE_TRUNCATE 0x10    /**< Truncate file */
#define BDMF_FMODE_SYNC     0x20    /**< Sync file */

/**< Open file
 * \param[in]   fname   File name
 * \param[in]   flags   Combination of BDMF_FMODE_ flags
 * \returns file handle or NULL
 */
bdmf_file bdmf_file_open(const char *fname, int flags);

/** Close file
 * \param[in]   fd      File handle returned by bdmf_file_open
 */
void bdmf_file_close(bdmf_file fd);

/** Read from file
 * \param[in]   fd      File handle returned by bdmf_file_open
 * \param[in]   buf     Buffer to read into
 * \param[in]   size    Number of bytes to read
 * \returns number of bytes read >= 0 or error < 0
 */
int bdmf_file_read(bdmf_file fd, void *buf, uint32_t size);

/** Write to file
 * \param[in]   fd      File handle returned by bdmf_file_open
 * \param[in]   buf     Buffer
 * \param[in]   size    Number of bytes to write
 * \returns number of bytes written >= 0 or error < 0
 */
int bdmf_file_write(bdmf_file fd, const void *buf, uint32_t size);

/** @} */


/** \defgroup bdmf_system_cache Data cache flush / invalidation
 * \ingroup bdmf_system
 * @{
 */

/** Invalidate dcache range
 * \param[in]   addr    start address
 * \param[in]   size    size
 */
static inline void bdmf_dcache_inv(unsigned long addr, unsigned long size)
{
    //blast_inv_dcache_range(addr, addr+size);
    cache_invalidate_len((void*)addr, size);
}

/** Flush dcache range
 * \param[in]   addr    start address
 * \param[in]   size    size
 */
static inline void bdmf_dcache_flush(unsigned long addr, unsigned long size)
{
    //blast_dcache_range(addr, addr+size);
    cache_flush_len((void*)addr, size);
}

/** @} */

/** \defgroup bdmf_system_sysb System network buffer support
 * \ingroup bdmf_system
 * @{
 */

/** System buffer type */
typedef enum
{
    bdmf_sysb_skb,          /**< sk_buff */
    bdmf_sysb_fkb,          /**< fkbuff */

    bdmf_sysb_type__num_of
} bdmf_sysb_type;

/** System buffer */
typedef void *bdmf_sysb;

void bdmf_sysb_recycle(bdmf_sysb sysb, uint32_t context, uint32_t flags);

/** Get sysb type
 * \param[in]   sysb        System buffer
 * \return system buffer type
 */
static inline bdmf_sysb_type bdmf_sysb_typeof(bdmf_sysb sysb)
{
    if ( IS_FKBUFF_PTR(sysb) )
    {
        return bdmf_sysb_fkb;
    }
    else
    {
        return bdmf_sysb_skb;
    }
}

/** Set headroom size for system buffer
 * \param[in]   sysb_type   System buffer type
 * \param[in]   headroom    Headroom size
 */
void bdmf_sysb_headroom_size_set(bdmf_sysb_type sysb_type, uint32_t headroom);

/** convert sysb to skb or fkb
 * \param[in]   sysb        System buffer
 * \return skb or fkb
 */
static inline void *bdmf_sysb_2_fkb_or_skb( bdmf_sysb sysb )
{
    return PNBUFF_2_PBUF(sysb);
}

extern struct sk_buff * skb_header_alloc(void);

/** Allocate system buffer header i.e skb or fkb structure
 *  and initilize it with the provided len & data buffer 
 * \param[in]   sysb_type   System buffer type
 * \param[in]   len         Data length
 * \return system buffer pointer.
 * If the function returns NULL, caller is responsible for "data" deallocation
 */
static inline bdmf_sysb bdmf_sysb_header_alloc(bdmf_sysb_type sysb_type, void* datap, uint32_t len, uint32_t context)
{
    if( sysb_type == bdmf_sysb_fkb ) 
    {
        FkBuff_t *fkb;

        fkb = fkb_init(datap, BCM_PKT_HEADROOM, datap, len);

        /*set the recyle hook */
        fkb->recycle_hook = bdmf_sysb_recycle;
        fkb->recycle_context = context;

        return (bdmf_sysb)FKBUFF_2_PNBUFF(fkb);
    }
    else if (sysb_type == bdmf_sysb_skb)
    {
        struct sk_buff *skb;
        /* allocate skb structure*/
        skb = skb_header_alloc();
        if(!skb)
        {
            return NULL;
        }
        
        /* initialize the skb */
        /*skb_headerinit(BCM_PKT_HEADROOM, len, skb, datap, bdmf_sysb_recycle,
                   context, NULL);*/
        
    skb_headerinit(BCM_PKT_HEADROOM,
#if defined(ENET_CACHE_SMARTFLUSH)
            SKB_DATA_ALIGN(len+BCM_SKB_TAILROOM),
#else
            BCM_MAX_PKT_LEN,
#endif
            skb, datap, bdmf_sysb_recycle,
            context, NULL);

        skb_trim(skb, len);
        skb->recycle_flags &= SKB_NO_RECYCLE;/* no skb recycle,just do data recyle */

        return (bdmf_sysb)SKBUFF_2_PNBUFF(skb);
    }
    printk("%s: sysbtype=%d not supported\n", __FUNCTION__, sysb_type);
    return NULL;
}

/** Allocate data buffers.
 * \param[out]  bufp        Array to hold allocated data buffers   
 * \param[in]   num_buffs   number of buffers to allocate 
 * \param[in]   context     currently unused
 * \returns     number of buffers allocated.
 */
static inline uint32_t bdmf_sysb_databuf_alloc(uint32_t *bufp, uint32_t num_buffs, uint32_t context)
{
#if (defined(CONFIG_BCM_BPM) || defined(CONFIG_BCM_BPM_MODULE))
    int i;
    if(gbpm_alloc_mult_buf(num_buffs, bufp) == GBPM_ERROR)
    {
        /* BPM returns either all the buffers requested or none */
        return 0;
    }
    
    /* no cache invalidation of buffers is needed for buffers coming from BPM */
    
    /*reserve space for headroom & FKB */ 
    for(i=0; i < num_buffs; i++ )
    {
        bufp[i]= (uint32_t)PFKBUFF_TO_PDATA((void *)(bufp[i]),BCM_PKT_HEADROOM);
    }   

    return num_buffs;
#else
    uint32_t *datap;
       /* allocate from kernel directly */
    datap = kmalloc(BCM_PKTBUF_SIZE, GFP_ATOMIC);

   if(!datap)  
   {
        return 0;
   }
    /* do a cache invalidate of the buffer */
    bdmf_dcache_inv((unsigned long)datap, BCM_PKTBUF_SIZE );

    /*reserve space for headroom & FKB */ 
    bufp[0] = (uint32_t)PFKBUFF_TO_PDATA((void *)(datap),BCM_PKT_HEADROOM);

    /* always return only one buffer when BPM is not enabled */
    return 1;
#endif
}


/** Recycle system buffer.
 * \param[in]   sysb        System buffer
 * \param[in]   context     unused 
 */
static inline void __bdmf_sysb_databuf_recycle(void *datap, uint32_t context)
{
#if (defined(CONFIG_BCM_BPM) || defined(CONFIG_BCM_BPM_MODULE))
    gbpm_free_buf(datap);
#else
    kfree(datap);
#endif
}

/** Free the datap poniter actual pointer allocated(before headroom) and
 then recyle
 * \param[in]   sysb        System buffer
 * \param[in]   context     unused 
 */
static inline void bdmf_sysb_databuf_free(void *datap, uint32_t context)
{
     /*do cache invalidate */
      bdmf_dcache_inv((unsigned long)datap, BCM_MAX_PKT_LEN);
    __bdmf_sysb_databuf_recycle((void *)PDATA_TO_PFKBUFF(datap, BCM_PKT_HEADROOM), context);
}

/** Release system buffer.
 * \param[in]   sysb        System buffer
 */
static inline void bdmf_sysb_free(bdmf_sysb sysb)
{
        nbuff_free(sysb);
}

/** Get sysb data pointer
 * \param[in]   sysb        System buffer
 * \return data pointer
 */
static inline void *bdmf_sysb_data(const bdmf_sysb sysb)
{
    if(IS_FKBUFF_PTR(sysb))
    {
        return (PNBUFF_2_FKBUFF(sysb))->data;
    }
    else
    {
        return (PNBUFF_2_SKBUFF(sysb))->data;
    }
}

/** Flush sysb data 
 * \param[in]   sysb        System buffer
 * \return data pointer
 */
static inline void bdmf_sysb_data_flush(const bdmf_sysb sysb, void *data, uint32_t len)
{
    nbuff_flush(sysb, data, len);
}


/** Get sysb data length
 * \param[in]   sysb        System buffer
 * \return data length
 */
static inline uint32_t bdmf_sysb_length(const bdmf_sysb sysb)
{
    if(IS_FKBUFF_PTR(sysb))
    {
        return (PNBUFF_2_FKBUFF(sysb))->len;
    }
    else
    {
        return (PNBUFF_2_SKBUFF(sysb))->len;
    }
}

/** Get sysb linear data length
 * \param[in]   sysb        System buffer
 * \return data length
 */
static inline uint32_t bdmf_sysb_data_length(const bdmf_sysb sysb)
{
    if(IS_FKBUFF_PTR(sysb))
    {
        return (PNBUFF_2_FKBUFF(sysb))->len;
    }
    else
    {
        /*length of skb->data only, does not include data in skb->frags or fraglist*/
        return skb_headlen(PNBUFF_2_SKBUFF(sysb));
    }
}


#if defined(CONFIG_BCM_PKTRUNNER_GSO)

#define RUNNER_MAX_GSO_DESC 512 

/* 
*** RUNNER_MAX_GSO_FRAGS should match RDD_GSO_DESC_ENTRY_FRAG_DATA_NUMBER
*/
#define RUNNER_MAX_GSO_FRAGS 18


extern void* bdmf_sysb_data_to_gsodesc(const bdmf_sysb sysb, uint32_t *is_gso_pkt_p);
extern int bdmf_gso_desc_pool_create( uint32_t num_desc);
extern void bdmf_gso_desc_pool_destroy(void);


#ifdef _BYTE_ORDER_LITTLE_ENDIAN_
typedef struct {
    union{
        uint32_t word0;
        uint32_t data;
    };
    union{
        uint32_t word1;
        struct{
            uint16_t len; 
            uint16_t linear_len; /*TODO change to total_len */
        };
    };

    union{
        uint32_t word2;
        struct{
            uint16_t mss;
            struct {
                uint8_t isAllocated:1;
                uint8_t reserved0:7;
            }; 
            uint8_t nr_frags;
        }; 
    };

    union{
        uint32_t word3;
        uint32_t reserved1;
    };

    uint32_t frag_data[RUNNER_MAX_GSO_FRAGS];
    uint16_t frag_len[RUNNER_MAX_GSO_FRAGS]; 
    uint32_t reserved2;
}runner_gso_desc_t;

#else
typedef struct {
    union{
        uint32_t word0;
        uint32_t data;
    };

    union{
        uint32_t word1;
        struct{
            uint16_t linear_len;
            uint16_t len; /*TODO change to total_len */
        };
    };

    union{
        uint32_t word2;
        struct{
            uint8_t nr_frags;
            struct {
                uint8_t isAllocated:1;
                uint8_t reserved0:7;
            }; 
            uint16_t mss;
        }; 
    };

    union{
        uint32_t word3;
        uint32_t reserved1;
    };
    uint32_t frag_data[RUNNER_MAX_GSO_FRAGS];
    uint16_t frag_len[RUNNER_MAX_GSO_FRAGS]; 
    uint32_t reserved2;
}runner_gso_desc_t;

#endif

/** Checks if a packet needs GSO processing and convert skb to GSO Descriptor  
 * \param[in]   sysb  system buffer 
 * \param[out]  is_gso_pkt_p indicates to caller if sysb is a GSO packet 
 * \returns for Non-GSO: sysb->data GSO: GSO Desciptor or NULL
 */
static inline void *bdmf_sysb_gso_data(const bdmf_sysb sysb, uint32_t *is_gso_pkt_p)
{
    if(IS_FKBUFF_PTR(sysb))
    {
        *is_gso_pkt_p = 0;
        return (PNBUFF_2_FKBUFF(sysb))->data;
    }
    else
    {
        return bdmf_sysb_data_to_gsodesc(sysb, is_gso_pkt_p);
    }
}

#endif


# if 0

/** Reserve headroom
 *
 * The function will assert if called for sysb containing data
 *
 * \param[in]   sysb        System buffer
 * \param[in]   bytes       Bytes to reserve
 */
static inline void bdmf_sysb_reserve(const bdmf_sysb sysb, uint32_t bytes)
{
    BUG_ON(bdmf_sysb_typeof(sysb) != bdmf_sysb_skb);
    skb_reserve((struct sk_buff *)sysb, bytes);
}
#endif

/** Add data to sysb
 *
 * The function will is similar to skb_put()
 *
 * \param[in]   sysb        System buffer
 * \param[in]   bytes       Bytes to add
 * \returns added block pointer
 */
static inline void *bdmf_sysb_put(const bdmf_sysb sysb, uint32_t bytes)
{
    return nbuff_put((struct sk_buff *)sysb, bytes);
}

/** @} */

/** \defgroup bdmf_system_pbuf Platform buffer support
 * \ingroup bdmf_system
 * @{
 */

/** Platform buffer */
typedef struct
{
    void *data;             /**< Buffer pointer */
    uint16_t bpm_bn;        /**< Buffer number */
    uint16_t source;        /**< Buffer source */
    uint16_t offset;        /**< Buffer offset */
    uint16_t length;        /**< Buffer length */
} bdmf_pbuf_t;

/** Initialize platform buffer support
 * \param[in]   size    buffer size
 * \param[in]   offset  min offset
 */
void bdmf_pbuf_init(uint32_t size, uint32_t offset);

/** Allocate pbuf and fill with data
 * The function allocates platform buffer and copies data into it
 * \param[in]   data        data pointer
 * \param[in]   length      data length
 * \param[in]   source      source port
 * \param[out]  pbuf        Platform buffer
 * \return 0 if OK or error < 0
 */
int bdmf_pbuf_alloc(void *data, uint32_t length, uint16_t source, bdmf_pbuf_t *pbuf);


#define INVALID_BPM_BUFFER 0xFFFF

/** Release pbuf
 * \param[in]   pbuf        Platform buffer
 * \return 0=OK, <0-error (sysb doesn't contain pbuf info)
 */
static inline void bdmf_pbuf_free(bdmf_pbuf_t *pbuf)
{
    if (pbuf->bpm_bn == INVALID_BPM_BUFFER)
        bdmf_sysb_free((bdmf_sysb)pbuf->data);
    else
        fi_bl_drv_bpm_free_buffer(pbuf->source, pbuf->bpm_bn);
}

/** Convert sysb to platform buffer
 * \param[in]   sysb        System buffer. Released regardless on the outcome
 * \param[in]   pbuf_source BPM source port as defined by RDD
 * \param[out]  pbuf        Platform buffer
 * \return 0=OK, <0-error (sysb doesn't contain pbuf info)
 */
int bdmf_pbuf_from_sysb(const bdmf_sysb sysb, uint16_t pbuf_source, bdmf_pbuf_t *pbuf);

/** Convert platform buffer to sysb
 * \param[in]   sysb_type   System buffer type
 * \param[in]   pbuf        Platform buffer. Released regardless on the outcome or becomes "owned" by sysb
 * \return sysb pointer or NULL
 */
bdmf_sysb bdmf_pbuf_to_sysb(bdmf_sysb_type sysb_type, bdmf_pbuf_t *pbuf);

/** Determine if sysb contains pbuf
 * \patam[in]   sysb        System buffer
 * \return 1 if sysb contains pbuf
 */
static inline int bdmf_sysb_is_pbuf(bdmf_sysb sysb)
{
    return 0;
}

/** @} */

/** \defgroup bdmf_system_timer Timer support
 * \ingroup bdmf_system
 * @{
 */

/** timer handle */
typedef struct bdmf_timer bdmf_timer_t;

/** timer callback function
 * \param[in]   timer   timer that has expired
 * \param[in]   priv    private cookie passed in bdmf_timer_init()
 */
typedef void (*bdmf_timer_cb_t)(bdmf_timer_t *timer, unsigned long priv);

struct bdmf_timer
{
    struct timer_list timer;
    bdmf_timer_cb_t cb;
    unsigned long priv;
};

/** Initialize timer
 * \param[in]   timer   timer handle
 * \param[in]   cb      callback to be called upon expiration
 * \param[in]   priv    private cooke to be passed to cb()
 */
void bdmf_timer_init(bdmf_timer_t *timer, bdmf_timer_cb_t cb, unsigned long priv);

/** Start timer
 * \param[in]   timer   timer handle that has been initialized using bdmf_timer_init()
 * \param[in]   ticks   number of ticks from now to expiration
 * \returns 0=OK or error code <0
 */
static inline int bdmf_timer_start(bdmf_timer_t *timer, uint32_t ticks)
{
    mod_timer(&timer->timer, jiffies + ticks);
    return 0;
}

/** stop timer
 * \param[in]   timer   timer to be stopped
 * The function is safe to call even if timer is not running
 */
static inline void bdmf_timer_stop(bdmf_timer_t *timer)
{
    del_timer_sync(&timer->timer);
}

/** Delete timer
 * \param[in]   timer   timer to be deleted
 * The timer is stopped if running
 */
static inline void bdmf_timer_delete(bdmf_timer_t *timer)
{
    bdmf_timer_stop(timer);
}

/* Get CPU frequency. */
static inline uint32_t bdmf_get_cpu_frequency(void)
{
    return HZ;
}

/** Convert ms to ticks
 * \param[in]   ms  ms
 * \returns timer ticks
 */
static inline uint32_t bdmf_ms_to_ticks(uint32_t ms)
{
    return msecs_to_jiffies(ms);
}

/** @} */


/** \defgroup bdmf_system_endian Big/little Endian support
 * \ingroup bdmf_system
 *
 * Endian-related macros and constants. The following constants must be defined
 *
 *  - __BYTE_ORDER define is set by GCC compilation environment. No need to do anything here.
 *  The following must be defined:
 *  - __BYTE_ORDER
 *  - __LITTLE_ENDIAN
 *  - __BIG_ENDIAN
 *  - __bswap_16(x)
 *  - __bswap_32(x)
 */

/*-----------------------------------------------------------------------
 *
 *----------------------------------------------------------------------*/
#ifdef __LITTLE_ENDIAN
# define __BYTE_ORDER   __LITTLE_ENDIAN
#endif
#ifdef __BIG_ENDIAN
# define __BYTE_ORDER   __BIG_ENDIAN
#endif
#define __bswap_16(x) ___arch__swab16(x)
#define __bswap_32(x) ___arch__swab32(x)



/*
 * Interrupt control
 */

#define BDMF_IRQ_NONE       IRQ_NONE        /**< IRQ is not from this device */
#define BDMF_IRQ_HANDLED    IRQ_HANDLED     /**< IRQ has been handled */

#define BDMF_IRQF_DISABLED  IRQF_DISABLED   /**< All interrupt are disabled when ISR is called */

/** Connect system interrupt
 * \param[in]   irq     IRQ number
 * \param[in]   cpu     CPU number (for SMP)
 * \param[in]   flags   IRQ flags
 * \param[in]   isr     ISR
 * \param[in]   name    device name
 * \param[in]   priv    Private cookie
 * \returns 0=OK, <0- error
 */
int bdmf_int_connect(int irq, int cpu, int flags,
    int (*isr)(int irq, void *priv), const char *name, void *priv);

/** Disconnect system interrupt
 * \param[in]   irq     IRQ number
 * \param[in]   priv    Private cookie passed in bdmf_int_connect()
 * \returns 0=OK, <0- error
 */
void bdmf_int_disconnect(int irq, void *priv);

/** Unmask IRQ
 * \param[in]   irq IRQ
 */
void bdmf_int_enable(int irq);

/** Mask IRQ
 * \param[in]   irq IRQ
 */
void bdmf_int_disable(int irq);

/*-----------------------------------------------------------------------
 * A few standard C library functions that are absent in linux kernel
 *----------------------------------------------------------------------*/
#define strtoul simple_strtoul
#define strtol simple_strtol
#define strtoull simple_strtoull
#define strtoll simple_strtoll
static inline const char *strerror(int rc)
{
        static char serr[20];
        sprintf(serr, "err -%d", rc);
        return serr;
}

#define assert(cond)   BUG_ON(!(cond))

/** \defgroup bdmf_rand Random numbers generation support
 * \ingroup bdmf_system
 * @{
 */

/** Return 16 bit random number
 * \return random number
 */
static inline uint16_t bdmf_rand16(void)
{
    return random32() & 0xFFFF;
}

/** Seed random number generation
 * \param[in]   seed      seed
 */
static inline void bdmf_srand(unsigned int seed)
{
    srandom32(seed);
}
/** @} end of bdmf_rand group */

/** \defgroup bdmf_queue message queue support
 * \ingroup bdmf_system
 * @{
 */
typedef struct bdmf_queue_message_t bdmf_queue_message_t;

typedef struct bdmf_queue
{
    struct list_head free_message_list;
    struct list_head full_message_list;
    uint32_t number_of_messages;
    uint32_t number_of_free_messages;
    void *p_messages_data_buffer;
    bdmf_queue_message_t *p_structs_buffer;
    struct semaphore receive_waiting_semaphore;
    bdmf_fastlock lock;
} bdmf_queue_t;

bdmf_error_t bdmf_queue_create(bdmf_queue_t *queue, uint32_t xi_number_of_messages, uint32_t xi_max_message_length);
bdmf_error_t bdmf_queue_delete(bdmf_queue_t *queue);
bdmf_error_t bdmf_queue_send(bdmf_queue_t *queue, char *xi_buffer, uint32_t xi_length);
bdmf_error_t bdmf_queue_receive(bdmf_queue_t *queue, char *xo_buffer, uint32_t *xio_length);

/** @} end of bdmf_queue group */

/** \defgroup bdmf_queue message queue support
 * \ingroup bdmf_system
 * @{
 */

typedef struct semaphore bdmf_semaphore_t;

void bdmf_semaphore_create(bdmf_semaphore_t *semaphore, int val);
void bdmf_semaphore_give(bdmf_semaphore_t *sem);
void bdmf_semaphore_take(bdmf_semaphore_t *sem);

/** @} end of bdmf_queue group */

/** @} end of bdmf_system group */

#endif /* _BDMF_SYSTEM_H_ */
