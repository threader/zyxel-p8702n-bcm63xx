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


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/ctype.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/in.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/delay.h>
#include <linux/fdtable.h>
#include <linux/vmalloc.h>
#include "bcm_intr.h"
#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
#include "board.h"
#endif
#include "bdmf_system.h"
#include "bdmf_platform.h"
#include "bcm_intr.h"

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#if defined(CONFIG_BCM_PKTRUNNER_GSO)
#include "kmap_skb.h"
#include "bcm_mm.h"
#endif

/* Magic header for memory allocated by vmalloc() */
#define VMALLOC_MAGIC 0x55555555
/* Magic header for memory allocated by kmalloc() */
#define KMALLOC_MAGIC 0x77777777

#define MAX_KMALLOC_SIZE (128 * 1024)

/*
 * Memory allocation
 */

void *bdmf_alloc(size_t size)
{
    void *p = NULL;

    if (size < MAX_KMALLOC_SIZE - sizeof(uint32_t))
    {
	p = kmalloc(size + sizeof(uint32_t), GFP_KERNEL);
	if (p)
	    *((uint32_t *)p) = KMALLOC_MAGIC;
    }
    else
    {
	p = vmalloc(size + sizeof(uint32_t));
	if (p)
	    *((uint32_t *)p) = VMALLOC_MAGIC;
    }

    return p ? p + sizeof(long) : NULL;
}

void bdmf_free(void *p)
{
    if (!p)
	return;

    p = p - sizeof(uint32_t);

    if (*(uint32_t*)p == KMALLOC_MAGIC)
    kfree(p);
    else
	vfree(p);
}

/*
 * Tasks
 */

int bdmf_task_create(const char *name, int priority, int stack,
    int (*handler)(void *arg), void *arg, bdmf_task *ptask)
{
    struct task_struct *t;
    t = kthread_run(handler, arg, name);
    if (t == ERR_PTR(-ENOMEM))
        return BDMF_ERR_NOMEM;
    *ptask = t;
    return 0;
}

int bdmf_task_destroy(bdmf_task task)
{
    kthread_stop(task);
    return 0;
}

const char *bdmf_task_get_name(bdmf_task task, char *name)
{
    sprintf(name, "%s (%d)", task->comm, task->pid);
    return name;
}

const bdmf_task bdmf_task_get_current(void)
{
    return current;
}

/*
 * Mutex support
 */

void bdmf_mutex_init(bdmf_mutex *pmutex)
{
    struct mutex *m=(struct mutex *)pmutex;
    BUG_ON(sizeof(struct mutex) > sizeof(bdmf_mutex));
    mutex_init(m);
}

int bdmf_mutex_lock(bdmf_mutex *pmutex)
{
    struct mutex *m=(struct mutex *)pmutex;
    WARN(in_interrupt(), "Attempt to take mutex in interrupt context\n");
    if (mutex_lock_interruptible(m) == -EINTR)
        return BDMF_ERR_INTR;
    return 0;
}

void bdmf_mutex_unlock(bdmf_mutex *pmutex)
{
    struct mutex *m=(struct mutex *)pmutex;
    mutex_unlock(m);
}

/*
 * Recursive mutex support
 */

typedef struct {
    pid_t pid;
    int count;
    struct mutex m;
} bdmf_linux_ta_mutex;

static DEFINE_SPINLOCK(ta_mutex_lock);

void bdmf_ta_mutex_init(bdmf_ta_mutex *pmutex)
{
    bdmf_linux_ta_mutex *tam = (bdmf_linux_ta_mutex *)pmutex;
    BUG_ON(sizeof(bdmf_linux_ta_mutex) > sizeof(bdmf_ta_mutex));
    tam->pid = -1;
    tam->count = 0;
    mutex_init(&tam->m);
}

int bdmf_ta_mutex_lock(bdmf_ta_mutex *pmutex)
{
    bdmf_linux_ta_mutex *tam = (bdmf_linux_ta_mutex *)pmutex;
    unsigned long flags;
    BUG_ON(!current);

    WARN(in_interrupt(), "Attempt to take ta_mutex in interrupt context\n");

    spin_lock_irqsave(&ta_mutex_lock, flags);
    if (tam->pid == current->pid)
    {
        ++tam->count;
        spin_unlock_irqrestore(&ta_mutex_lock, flags);
        return 0;
    }
    spin_unlock_irqrestore(&ta_mutex_lock, flags);

    /* not-recurring request */
    if (mutex_lock_interruptible(&tam->m) == -EINTR)
        return BDMF_ERR_INTR;

    tam->pid = current->pid;
    tam->count = 1;

    return 0;
}

void bdmf_ta_mutex_unlock(bdmf_ta_mutex *pmutex)
{
    bdmf_linux_ta_mutex *tam = (bdmf_linux_ta_mutex *)pmutex;
    BUG_ON(!current);
    BUG_ON(tam->pid != current->pid);
    BUG_ON(tam->count < 1);
    if (--tam->count == 0)
    {
        tam->pid = -1;
        mutex_unlock(&tam->m);
    }
}

void bdmf_ta_mutex_delete(bdmf_ta_mutex *pmutex)
{
}

/*
 * Recursive fastlock support
 */
#ifndef NR_CPUS
#define NR_CPUS 2
#endif
typedef struct {
    int refcnt[NR_CPUS];
    spinlock_t lock;
} bdmf_linux_reent_fastlock;

void bdmf_reent_fastlock_init(bdmf_reent_fastlock *lock)
{
    bdmf_linux_reent_fastlock *reent_fastlock = (bdmf_linux_reent_fastlock *)lock;

    memset(lock, 0, sizeof(bdmf_reent_fastlock));
    BUG_ON(sizeof(bdmf_linux_reent_fastlock) > sizeof(bdmf_reent_fastlock));
    spin_lock_init(&reent_fastlock->lock);
}

int bdmf_reent_fastlock_lock(bdmf_reent_fastlock *lock)
{
    bdmf_linux_reent_fastlock *reent_fastlock = (bdmf_linux_reent_fastlock *)lock;
    int cpu_id;

    local_bh_disable(); /* Disable preemption */
    cpu_id = smp_processor_id();
    BUG_ON(cpu_id < 0 || cpu_id >= NR_CPUS);
    BUG_ON(in_irq());
    reent_fastlock->refcnt[cpu_id]++;
    if (reent_fastlock->refcnt[cpu_id] == 1) /* First time lock is taken */
        spin_lock_bh(&reent_fastlock->lock);
    return 0;
}

void bdmf_reent_fastlock_unlock(bdmf_reent_fastlock *lock)
{
    bdmf_linux_reent_fastlock *reent_fastlock = (bdmf_linux_reent_fastlock *)lock;
    int cpu_id;

    cpu_id = smp_processor_id();
    BUG_ON(cpu_id < 0 || cpu_id >= NR_CPUS);
    BUG_ON(!reent_fastlock->refcnt[cpu_id]); /* Bad call - unlock w/o lock */
    reent_fastlock->refcnt[cpu_id]--;
    if (!reent_fastlock->refcnt[cpu_id]) /* Last time - release the lock */
        spin_unlock_bh(&reent_fastlock->lock);
    local_bh_enable(); /* Enable preemption */
}

/*
 * Print to the current process' stdout
 */

static int _bdmf_write(struct file *fd, const unsigned char *buf, int len)
{
    mm_segment_t oldfs;
    int len0 = len;
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    while(len)
    {
        int lw;
        lw = vfs_write(fd, (char *)buf, len, &fd->f_pos);
        if (lw < 0)
            break;
        len -= lw;
        if (len)
        {
            if (msleep_interruptible(1) < 0)
                break;
            buf += lw;
        }
    }
    set_fs(oldfs);
    return (len0 - len);
}

int bdmf_vprint(const char *format, va_list args)
{
    struct file *f;
    int rc=0;

    /* Get stdout file handle if any */
    if (in_interrupt() || !current || !current->files)
        f = 0;
    else
        f = fcheck(STDOUT_FILENO);
    if (!f)
    {
         rc = vprintk(format, args);
    }
    else
    {
        char printbuf[1024];
        char *p1=printbuf, *p2;
        vscnprintf(printbuf, sizeof(printbuf)-1, format, args);
        printbuf[sizeof(printbuf)-1]=0;
        do
        {
            p2 = strchr(p1, '\n');
            if (p2)
            {
                rc += _bdmf_write(f, (unsigned char*) p1, p2-p1+1);
                rc += _bdmf_write(f, (unsigned char*)"\r", 1);
            }
            else
                rc += _bdmf_write(f, (unsigned char*)p1, strlen(p1));
            //rc += p2 - p1 + 1;
            p1 = p2 + 1;
        } while(p2);
    }
    return rc;
}

int bdmf_print(const char *format, ...)
{
    int rc;
    va_list args;
    va_start(args, format);
    rc = bdmf_vprint(format, args);
    va_end(args);
    return rc;
}

/*
 * File IO
 */

bdmf_file bdmf_file_open(const char *fname, int flags)
{
    int oflags=0;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    struct file *fd;

    if ((flags & BDMF_FMODE_RDWR) == BDMF_FMODE_RDWR)
        oflags |= O_RDWR;
    else if ((flags & BDMF_FMODE_RDONLY))
        oflags |= O_RDONLY;
    else if ((flags & BDMF_FMODE_WRONLY))
        oflags |= O_WRONLY;

    if ((flags & BDMF_FMODE_CREATE))
        oflags |= O_CREAT;
    if ((flags & BDMF_FMODE_APPEND))
        oflags |= O_APPEND;
    if ((flags & BDMF_FMODE_TRUNCATE))
        oflags |= O_TRUNC;

    fd = filp_open(fname, oflags, mode);

    if (IS_ERR(fd))
        return NULL;

    return fd;
}

void bdmf_file_close(bdmf_file fd)
{
    filp_close(fd, 0);
}

int bdmf_file_read(bdmf_file fd, void *buf, uint32_t size)
{
    mm_segment_t oldfs;
    int n;
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    n = vfs_read(fd, buf, size, &fd->f_pos);
    set_fs(oldfs);
    return ((n >= 0) ? n : BDMF_ERR_IO);
}

int bdmf_file_write(bdmf_file fd, const void *buf, uint32_t size)
{
    mm_segment_t oldfs;
    int n;
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    n = vfs_write(fd, buf, size, &fd->f_pos);
    set_fs(oldfs);
    return ((n >= 0) ? n : BDMF_ERR_IO);
}

#if 1 
/*TODO  check with Igor and remove this
headroom should be same for all types of buffers,
as one type can be covnverted to other. And also this
is unnecessary as headroom is decided at compile time.*/ 
/*
 * System buffer abstraction
 */

static uint32_t sysb_headroom[bdmf_sysb_type__num_of];

/** Set headroom size for system buffer
 * \param[in]   sysb_type   System buffer type
 * \param[in]   headroom    Headroom size
 */
void bdmf_sysb_headroom_size_set(bdmf_sysb_type sysb_type, uint32_t headroom)
{
    sysb_headroom[sysb_type] = BCM_PKT_HEADROOM;
}
#endif


/** Recycle the data buffer
 * \param[in]   data   Data buffer 
 * \param[in]   context -unused,for future use 
 */
void bdmf_sysb_databuf_recycle(void *datap, uint32_t context)
{
    __bdmf_sysb_databuf_recycle(datap, context);
}

/** Recycle the system buffer and associated data buffer
 * \param[in]   data   Data buffer 
 * \param[in]   context - unused,for future use 
 * \param[in]   flags   - indicates what to recyle
 */
void bdmf_sysb_recycle(bdmf_sysb sysb, uint32_t context, uint32_t flags)
{
    if ( IS_FKBUFF_PTR(sysb) )
    {
        __bdmf_sysb_databuf_recycle(PNBUFF_2_FKBUFF(sysb), context);
    }
    else /* skb */
    {
        struct sk_buff *skb = PNBUFF_2_SKBUFF(sysb);
        
        if (flags & SKB_DATA_RECYCLE)
        {
            void *data_endp;
            void *data_startp = skb->head + BCM_PKT_HEADROOM;

#if defined(CC_NBUFF_FLUSH_OPTIMIZATION)
            void *dirty_p = skb_shinfo(skb)->dirty_p;
            void *shinfoBegin = (uint8_t *)skb_shinfo(skb);
            void *shinfoEnd;
            if (skb_shinfo(skb)->nr_frags == 0)
            {
                // no frags was used on this skb, so can shorten amount of data
                // flushed on the skb_shared_info structure
                shinfoEnd = shinfoBegin + offsetof(struct skb_shared_info, frags);
            } 
            else
            {
                shinfoEnd = shinfoBegin + sizeof(struct skb_shared_info);
            }
            //cache_flush_region(shinfoBegin, shinfoEnd);
            cache_invalidate_region(shinfoBegin, shinfoEnd);

            // If driver returned this buffer to us with a valid dirty_p,
            // then we can shorten the flush length.
            if (dirty_p)
            {
                if ((dirty_p < (void *)skb->head) || (dirty_p > shinfoBegin))
                {
                    printk("invalid dirty_p detected: %p valid=[%p %p]\n",
                           dirty_p, skb->head, shinfoBegin);
                    data_endp = shinfoBegin;
                } 
                else
                {
                    data_endp = (dirty_p < data_startp) ? data_startp : dirty_p;
                }
            } 
            else
            {
                data_endp = shinfoBegin;
            }
#else
            /*its ok not to invalidate skb->head to skb->data as this area is
             * not used by runner or DMA 
             */
            data_endp = (void *)(skb_shinfo(skb)) + sizeof(struct skb_shared_info);
#endif
            cache_invalidate_region(data_startp, data_endp);

            /* free the buffer to global BPM pool */
            __bdmf_sysb_databuf_recycle(PDATA_TO_PFKBUFF(data_startp, BCM_PKT_HEADROOM), context);
        }
        else
        {
            printk("%s: Error only DATA recycle is supported\n", __FUNCTION__);
        }
    }
}

#if defined(CONFIG_BCM_PKTRUNNER_GSO)

static DEFINE_SPINLOCK(bdmf_runner_gso_desc_pool_lock);

#define BDMF_RUNNER_GSO_DESC_POOL_LOCK(flags)    spin_lock_irqsave(&bdmf_runner_gso_desc_pool_lock, flags)
#define BDMF_RUNNER_GSO_DESC_POOL_UNLOCK(flags)  spin_unlock_irqrestore(&bdmf_runner_gso_desc_pool_lock, flags)

runner_gso_desc_t *runner_gso_desc_pool=NULL;


/* bdmf_kmap_skb_frag bdmf_kunmap_skb_frag  functions are copied 
 * from linux. Here we are not using local_bh_disable/local_bh_enable
 * when HIGHMEM is enabled. This is because currently these functions
 * are called with irq's disabled (f_rdd_lock_irq), kernel will generate
 * a warning if local_bh_enable is called when irq's are disabled 
 */

/*TODO: optimize bdmf_kmap_skb_frag bdmf_kunmap_skb_frag */

/** map a page to virtual address 
 * \param[in]   skb_frag_t   
 * return : virtual address
 */
static inline void *bdmf_kmap_skb_frag(const skb_frag_t *frag)
{
#ifdef CONFIG_HIGHMEM
    BUG_ON(in_irq());

    //local_bh_disable();
#endif
    return kmap_atomic(skb_frag_page(frag));
}

/** Free virtual address mapping 
 * \param[in]   vaddr virtual address 
 */
static inline void bdmf_kunmap_skb_frag(void *vaddr)
{
    kunmap_atomic(vaddr);
#ifdef CONFIG_HIGHMEM
    //local_bh_enable();
#endif
}

//#define CONFIG_BCM_PKTRUNNER_GSO_DEBUG 1

/** Recycle a GSO Descriptor 
 * \param[in]   gso_desc_p   pointer to GSO Descriptor 
 */
void bdmf_runner_gso_desc_free(runner_gso_desc_t *gso_desc_p)
{
    if(gso_desc_p)
    {
        unsigned long flags;

        BDMF_RUNNER_GSO_DESC_POOL_LOCK(flags);
        gso_desc_p->isAllocated = 0;
        cache_flush_len(gso_desc_p, sizeof(runner_gso_desc_t));
        BDMF_RUNNER_GSO_DESC_POOL_UNLOCK(flags);
    }
}

/** Allocate a GSO Descriptor 
 * \returns pointer to allocated GSO Descriptor
 */
static inline runner_gso_desc_t *bdmf_runner_gso_desc_alloc(void)
{
    static int alloc_index = 0;
    int i=0;
    unsigned long flags;
    runner_gso_desc_t *gso_desc_p;

    BDMF_RUNNER_GSO_DESC_POOL_LOCK(flags);

    while(i < RUNNER_MAX_GSO_DESC)
    {
        if(alloc_index == RUNNER_MAX_GSO_DESC)
            alloc_index = 0;

        gso_desc_p = &runner_gso_desc_pool[alloc_index];
        cache_invalidate_len(gso_desc_p, sizeof(runner_gso_desc_t));
        if(!gso_desc_p->isAllocated)
        {
	#if 1
            memset(gso_desc_p, 0, sizeof(runner_gso_desc_t));
	#else
            gso_desc_p->nr_frags = 0;
            gso_desc_p->flags = 0;
            gso_desc_p->mss = 0;
	#endif

            gso_desc_p->isAllocated = 1;
            alloc_index++;
            BDMF_RUNNER_GSO_DESC_POOL_UNLOCK(flags);
            return gso_desc_p;
        }
        alloc_index++;
        i++;
    }
    BDMF_RUNNER_GSO_DESC_POOL_UNLOCK(flags);
    return NULL;
}

#if defined(CONFIG_BCM_PKTRUNNER_GSO_DEBUG)
static int bdmf_gso_desc_dump_enabled=0;

/**  Print information in GSO Descriptor 
 * \param[in]   gso_desc_p   pointer to GSO Descriptor 
 */
void bdmf_gso_desc_dump(runner_gso_desc_t *gso_desc_p)
{
    int i;

    if(bdmf_gso_desc_dump_enabled)
    {
        printk("******* Dumping gso_desc_p =%p  runner phys-addr =%x*********\n", gso_desc_p, swap4bytes(VIRT_TO_PHYS(gso_desc_p)));

        printk("gso_desc_p ->len:        host(%d ) runner(%d) \n", swap2bytes(gso_desc_p->len), gso_desc_p->len);
        printk("gso_desc_p ->linear_len: host(%d ) runner(%d) \n", swap2bytes(gso_desc_p->linear_len), gso_desc_p->linear_len);
        printk("gso_desc_p ->mss:        host(%d ) runner(%d)\n", swap2bytes(gso_desc_p->mss), gso_desc_p->mss);
        printk("gso_desc_p ->nr_frags:   %d \n", gso_desc_p->nr_frags);

        printk("gso_desc_p ->data:       host(%x ) runner(%x)\n", swap4bytes(gso_desc_p->data), gso_desc_p->data);
        for (i=0; i < gso_desc_p ->nr_frags; i++)
        {
            printk("Hostfrag_data[%d]=%x, frag_len=%d \n", i, swap4bytes(gso_desc_p->frag_data[i]), swap2bytes(gso_desc_p->frag_len[i]));
            printk("frag_data[%d]=%x, frag_len=%d \n", i, gso_desc_p->frag_data[i], gso_desc_p->frag_len[i]);
        } 
        cache_invalidate_len(gso_desc_p, sizeof(runner_gso_desc_t));
    }
}
#endif

/** Create a pool of GSO Descriptors 
 * \param[in]   num_desc  number of descriptors to be created 
 * \returns 0=Success -1=failure
 */
int bdmf_gso_desc_pool_create( uint32_t num_desc)
{
    uint32_t mem_size = num_desc * sizeof(runner_gso_desc_t);

    runner_gso_desc_pool = CACHED_MALLOC(mem_size);
    if(!runner_gso_desc_pool)
    {
        printk(KERN_ERR "###### ERROR:Failed to allocate runner_gso_desc_pool\n");
        return -1;
    }
    memset(runner_gso_desc_pool, 0, mem_size);
    cache_flush_len(runner_gso_desc_pool,  mem_size);

    printk("++++Runner gso_desc_pool created successfully\n");
#if defined(CONFIG_BCM_PKTRUNNER_GSO_DEBUG)
    printk("&bdmf_gso_desc_dump_enabled=%p bdmf_gso_desc_dump_enabled=%d\n",
            &bdmf_gso_desc_dump_enabled, bdmf_gso_desc_dump_enabled); 
#endif

    return 0; 
}
EXPORT_SYMBOL(bdmf_gso_desc_pool_create);

/** Free a pool of GSO Descriptors 
 */
void bdmf_gso_desc_pool_destroy(void)
{
    CACHED_FREE(runner_gso_desc_pool);
}
EXPORT_SYMBOL(bdmf_gso_desc_pool_destroy);


/** Checks if a packet needs GSO processing and convert skb to GSO Descriptor  
 * \param[in]   sysb  system buffer 
 * \param[out]  is_gso_pkt_p indicates to caller if sysb is a GSO packet 
 * \returns for Non-GSO: sysb->data GSO: GSO Desciptor or NULL
 */
void* bdmf_sysb_data_to_gsodesc(const bdmf_sysb sysb, uint32_t *is_gso_pkt_p)
{
#if 0
    if(IS_FKBUFF_PTR(sysb))
    {
        printk("GSO not supported with fkbs's \n");
        return NULL;
    }
    else
#endif
    {
        const struct sk_buff *skb = PNBUFF_2_SKBUFF(sysb);
        uint32 linear_len, total_frag_len;
        uint16 nr_frags, i;
        runner_gso_desc_t *gso_desc_p;
        skb_frag_t *frag;
        uint8 *vaddr;

        if((skb->ip_summed == CHECKSUM_PARTIAL) || skb_is_gso(skb) || skb_shinfo(skb)->nr_frags)
        {

#if 0
            /*TODO remove this check after verfiying with 8K page sizes */
            if( skb->len > 64*1024)
            {
                printk("%s: ERROR: skb->len %d  >64K\n",__FUNCTION__, skb->len);
            }
#endif

            /*TODO when FRAGLIST is supported add nr_frags of all skb's in fraglist */
            nr_frags = skb_shinfo(skb)->nr_frags;

            if(nr_frags >  RUNNER_MAX_GSO_FRAGS)
            {
                printk("%s: ERROR: nr_frags %d exceed max\n",__FUNCTION__, nr_frags);
                return NULL;
            }

            gso_desc_p = bdmf_runner_gso_desc_alloc();
            if(!gso_desc_p)
            {
                printk("%s: ERROR: failed to allocate gso_desc_p\n",__FUNCTION__);
                return NULL;
            }

            if(nr_frags)
            {

                total_frag_len = 0;

                for(i=0; i < nr_frags; i++ )
                {
                    frag = &skb_shinfo(skb)->frags[i];
                    vaddr = bdmf_kmap_skb_frag(frag);
                    cache_flush_len((vaddr + frag->page_offset), frag->size);
                    gso_desc_p->frag_data[i]= swap4bytes(VIRT_TO_PHYS(vaddr + frag->page_offset));
                    gso_desc_p->frag_len[i] = swap2bytes((uint16_t)(frag->size));
                    total_frag_len += frag->size;
                    bdmf_kunmap_skb_frag(vaddr);
                }

                linear_len = skb->len - total_frag_len;

            }
            else
            {
                linear_len = skb->len;
            }
            /*We expect skb->data to be flushed already in RDPA */
            //cache_flush_len(skb->data, linear_len);


            gso_desc_p->data = swap4bytes(VIRT_TO_PHYS(skb->data));
            gso_desc_p->linear_len = swap2bytes(linear_len);
            gso_desc_p->len = swap2bytes(skb->len);
            gso_desc_p->nr_frags = nr_frags;
            gso_desc_p->mss = swap2bytes(skb_shinfo(skb)->gso_size);
            *is_gso_pkt_p = 1;
            cache_flush_len(gso_desc_p, sizeof(runner_gso_desc_t));

#if defined(CONFIG_BCM_PKTRUNNER_GSO_DEBUG)
            bdmf_gso_desc_dump(gso_desc_p);
#endif

            return gso_desc_p;
        }
        else
        {
            *is_gso_pkt_p = 0;
            return skb->data;
        }
    }
}
EXPORT_SYMBOL(bdmf_sysb_data_to_gsodesc);
#endif


/*
 * Platform buffer support
 */
static void *tm_ddr_base;
static void *tm_ddr_end;
static uint32_t tm_pbuf_size;
static uint32_t tm_pbuf_offset;

static inline void *bdmf_get_tm_ddr_base(void)
{
    uint32_t tm_ddr_size = 0;
    int rc = 0;

    if (tm_ddr_base)
        return tm_ddr_base;
//TODO:replace the config by something else
#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
    rc = BcmMemReserveGetByName(TM_BASE_ADDR_STR, &tm_ddr_base, &tm_ddr_size);
    if (rc == -1 || ((uint32_t)tm_ddr_base % 0x200000) != 0)
         printk( "Failed to get valid DDR TM address, rc = %d  validate_ddr_address: ddr_tm_base_address=%x\n", rc,
                  (int) tm_ddr_base);
#else
    rc = bl_lilac_mem_rsrv_get_by_name( TM_BASE_ADDR_STR , &tm_ddr_base, (unsigned long *)&tm_ddr_size);
    if (rc)
        printk("%s: bl_lilac_mem_rsrv_get_by_name failed.\n", __FUNCTION__);
#endif
    tm_ddr_end = tm_ddr_base + tm_ddr_size - 1;
    return tm_ddr_base;
}

/** Initialize platform buffer support
 * \param[in]   size    buffer size
 * \param[in]   offset  min offset
 */
void bdmf_pbuf_init(uint32_t size, uint32_t offset)
{
    bdmf_get_tm_ddr_base();
    tm_pbuf_size = size;
    tm_pbuf_offset = offset;
}

/** Allocate pbuf and fill with data
 * The function allocates platform buffer and copies data into it
 * \param[in]   data        data pointer
 * \param[in]   length      data length
 * \param[in]   source      source port as defined by RDD
 * \param[out]  pbuf        Platform buffer
 * \return 0 if OK or error < 0
 */
int bdmf_pbuf_alloc(void *data, uint32_t length, uint16_t source, bdmf_pbuf_t *pbuf)
{
    uint32_t bn;
    void *buffer_ptr;

    BUG_ON(!tm_pbuf_size);
    if (tm_pbuf_offset + length > tm_pbuf_size)
        return BDMF_ERR_OVERFLOW;
    if (fi_bl_drv_bpm_req_buffer(source, &bn))
        return BDMF_ERR_NOMEM;
    buffer_ptr = bdmf_get_tm_ddr_base() + bn * tm_pbuf_size + tm_pbuf_offset;
    /* ToDo: copy via cache */
    memcpy(buffer_ptr, data, length);
    pbuf->bpm_bn = bn;
    pbuf->length = length;
    pbuf->source = source;
    pbuf->data = buffer_ptr;
    pbuf->offset = tm_pbuf_offset;
    return 0;
}

/** Convert sysb to platform buffer
 * \param[in]   sysb        System buffer. Released regardless on the outcome
 * \param[in]   pbuf_source BPM source port as defined by RDD
 * \param[out]  pbuf        Platform buffer
 * \return 0=OK, <0-error (sysb doesn't contain pbuf info)
 */
int bdmf_pbuf_from_sysb(const bdmf_sysb sysb, uint16_t pbuf_source, bdmf_pbuf_t *pbuf)
{
    /* ToDo: for now do copy.
     * When adding zero copy not to forget cache flush
     */
    uint32_t length;
    void *data;
    int rc;

    length = bdmf_sysb_length(sysb);
    data = bdmf_sysb_data(sysb);
    rc = bdmf_pbuf_alloc(data, length, pbuf_source, pbuf);
    bdmf_sysb_free(sysb);
    return rc;
}

/** Convert platform buffer to sysb
 * \param[in]   sysb_type   System buffer type
 * \param[in]   pbuf        Platform buffer. Released regardless on the outcome or becomes "owned" by sysb
 * \return sysb pointer or NULL
 */
bdmf_sysb bdmf_pbuf_to_sysb(bdmf_sysb_type sysb_type, bdmf_pbuf_t *pbuf)
{
    /* ToDo: currently do a copy. Add zero-copy next.
     * remember about cache invalidation!
     */
    bdmf_sysb sysb;
    uint32_t data_bufp;
    uint32_t context=0;
    void *data = (pbuf->bpm_bn == INVALID_BPM_BUFFER) ? bdmf_sysb_data((bdmf_sysb)pbuf->data) : pbuf->data;
    
    if(!bdmf_sysb_databuf_alloc(&data_bufp, 1 , context)) 
    {
        bdmf_pbuf_free(pbuf);
        return NULL;
    }
    /* ToDo: copy via cache */
    memcpy((void*)data_bufp, data, pbuf->length);

    sysb = bdmf_sysb_header_alloc(sysb_type, (void*)data_bufp, pbuf->length, context);
    if (!sysb)
    {
        bdmf_sysb_databuf_free((void*)data_bufp, context);
    }
    bdmf_pbuf_free(pbuf);
    return sysb;
}

/*
 * Timer support
 */

static void _bdmf_timer_cb(unsigned long priv)
{
    bdmf_timer_t *timer = (bdmf_timer_t *)priv;
    timer->cb(timer, timer->priv);
}

void bdmf_timer_init(bdmf_timer_t *timer, bdmf_timer_cb_t cb, unsigned long priv)
{
    timer->cb = cb;
    timer->priv = priv;
    setup_timer(&timer->timer, _bdmf_timer_cb, (long)timer);
}

/*
 * Interrupt control
 */


int bdmf_int_connect(int irq, int cpu, int flags,
    int (*isr)(int irq, void *priv), const char *name, void *priv)
{
    int rc;
    /* Supposingly should work for all BCM platforms.
     * If it is not the case - mode ifdefs can be added later.
     * We might also switch to
     * BcmHalMapInterruptEx in order to handle affinity.
     */
    rc = BcmHalMapInterrupt((FN_HANDLER)isr, (unsigned int)priv, irq);
    return rc ? BDMF_ERR_INTERNAL : 0;
}

/** Disconnect system interrupt
 * \param[in]   irq     IRQ number
 * \param[in]   priv    Private cookie passed in bdmf_int_connect()
 * \returns 0=OK, <0- error
 */
void bdmf_int_disconnect(int irq, void *priv)
{
    free_irq(irq, priv);
}

/** Unmask IRQ
 * \param[in]   irq IRQ
 */
void bdmf_int_enable(int irq)
{
#if defined(CONFIG_BCM96838)
	/*
	 * in case of Oren the BcmHalMapInterrupt already enables the interrupt.
	 */
#else
    /* Supposingly should work for all BCM platforms.
     * If it is not the case - mode ifdefs can be added later.
     */
#endif
}

/** Mask IRQ
 * \param[in]   irq IRQ
 */
void bdmf_int_disable(int irq)
{
    /* Supposingly should work for all BCM platforms.
     * If it is not the case - mode ifdefs can be added later.
     */
    BcmHalInterruptDisable(irq);
}

/*
 * Queues
 */

struct bdmf_queue_message_t
{
    struct list_head messages_list;
    char *p_message_buffer;
    uint32_t message_length;			
};

bdmf_error_t bdmf_queue_create(bdmf_queue_t *queue, uint32_t number_of_messages, uint32_t max_message_length)
{
    bdmf_queue_message_t *p_structs_buffer = NULL;
    bdmf_queue_message_t *p_message_struct;
    char *p_messages_data_buffer = NULL;
    int32_t i;
    bdmf_error_t bdmf_error;

    memset(queue, 0, sizeof(*queue));
    INIT_LIST_HEAD(&queue->free_message_list);
    INIT_LIST_HEAD(&queue->full_message_list);
    bdmf_fastlock_init(&queue->lock);

    /* Allocate buffer for messages data */
    p_messages_data_buffer = bdmf_alloc(number_of_messages * max_message_length);
    if (!p_messages_data_buffer)
        return BDMF_ERR_NOMEM;

    /* allocate  all messages structs */
    p_structs_buffer = bdmf_alloc(number_of_messages * sizeof(bdmf_queue_message_t));
    if (!p_structs_buffer)
    {
        bdmf_error = BDMF_ERR_NOMEM;
        goto Error;
    }

    /* Create receive semaphore */
    sema_init(&queue->receive_waiting_semaphore, 0);

    bdmf_fastlock_lock(&queue->lock);

    /* Create queue */
    for (i = 0; i < number_of_messages; i++)
    {
        p_message_struct = &p_structs_buffer[i];
        /* Set all message pointers to actual buffers */
        p_message_struct->p_message_buffer = &p_messages_data_buffer[i * max_message_length];
        /* Add all messages to free list */
        list_add(&p_message_struct->messages_list, &queue->free_message_list);
    }

    /* Update queue struct */
    queue->number_of_free_messages = number_of_messages;
    queue->p_messages_data_buffer = p_messages_data_buffer;
    queue->p_structs_buffer = p_structs_buffer;

    bdmf_fastlock_unlock(&queue->lock);

    return 0;

Error:
    if (p_structs_buffer)
        bdmf_free(p_structs_buffer);
    if (p_messages_data_buffer)
        bdmf_free(p_messages_data_buffer);
    return bdmf_error;
}

bdmf_error_t bdmf_queue_delete(bdmf_queue_t *queue)
{
    int32_t i;
    void *p_messages_data_buffer;
    bdmf_queue_message_t *p_structs_buffer;

    if (!queue->free_message_list.next)
        return 0; /* queue is uninitialized */

    bdmf_fastlock_lock(&queue->lock);

    /* Release all free message structs */
    for (i = 0; i < queue->number_of_free_messages; i++)
    {
        if (list_empty(&queue->free_message_list))
            break;
        list_del_init(&queue->free_message_list);
    }

    /* Release all full message structs */
    for (i = 0; i < queue->number_of_messages; i++)
    {
        if (list_empty(&queue->full_message_list))
            break;
        list_del_init(&queue->full_message_list);
    }

    p_messages_data_buffer = queue->p_messages_data_buffer;
    p_structs_buffer = queue->p_structs_buffer;

    bdmf_fastlock_unlock(&queue->lock);

    /* Free allocated memory */
    if (p_messages_data_buffer) 
        bdmf_free(p_messages_data_buffer);

    if (p_structs_buffer)
        bdmf_free(p_structs_buffer);

    return 0;
}

bdmf_error_t bdmf_queue_send(bdmf_queue_t *queue, char *buffer, uint32_t length)
{
    bdmf_error_t bdmf_error;
    bdmf_queue_message_t *p_message_struct;

    bdmf_fastlock_lock(&queue->lock);

    if (!queue->number_of_free_messages)
    {
        bdmf_error = BDMF_ERR_NORES; /* No resources error */
        goto Exit;
    }

    /* Update full & free lists */
    p_message_struct = list_entry(queue->free_message_list.next, bdmf_queue_message_t, messages_list);

    /* Remove from empty list */
    list_del_init(&p_message_struct->messages_list); 								
    /* Add to full list end */
    list_add_tail(&p_message_struct->messages_list, &queue->full_message_list);		

    /* Copy Data from User to queue buffer */
    memcpy(p_message_struct->p_message_buffer, buffer, length);
    p_message_struct->message_length = length;

    /* Free any tasks waiting for messages */
    up(&queue->receive_waiting_semaphore);

    /* Upadte message counters */
    queue->number_of_free_messages--;
    queue->number_of_messages++;

    bdmf_error = 0;

Exit:
    bdmf_fastlock_unlock(&queue->lock);
    return bdmf_error;
}

bdmf_error_t bdmf_queue_receive(bdmf_queue_t *queue, char *buffer, uint32_t *length)
{
    bdmf_queue_message_t *p_message_struct;

    /* Taking counting semaphore to make sure we are not accessing an empty queue*/
    down(&queue->receive_waiting_semaphore);

    bdmf_fastlock_lock(&queue->lock);

    /* Update full & free lists */
    p_message_struct = list_entry(queue->full_message_list.next, bdmf_queue_message_t, messages_list);

    list_del_init(&p_message_struct->messages_list); /* Remove from full list */
    list_add_tail(&p_message_struct->messages_list, &queue->free_message_list); /* Add to free list */

    memcpy(buffer, p_message_struct->p_message_buffer, p_message_struct->message_length);

    /* Upadte message counters */
    queue->number_of_free_messages++;
    queue->number_of_messages--;

    /* Return the number of bytes actually read */
    *length = p_message_struct->message_length;

    bdmf_fastlock_unlock(&queue->lock);

    return 0;
}

/*
 * Semaphores
 */

void bdmf_semaphore_create(bdmf_semaphore_t *semaphore, int val)
{
    sema_init(semaphore, val);
}

void bdmf_semaphore_give(bdmf_semaphore_t *sem)
{
    up(sem);
}

void bdmf_semaphore_take(bdmf_semaphore_t *sem)
{
    down(sem);
}

/*
 * Exports
 */

EXPORT_SYMBOL(bdmf_alloc);
EXPORT_SYMBOL(bdmf_calloc);
EXPORT_SYMBOL(bdmf_free);
EXPORT_SYMBOL(bdmf_task_create);
EXPORT_SYMBOL(bdmf_task_destroy);
EXPORT_SYMBOL(bdmf_task_get_name);
EXPORT_SYMBOL(bdmf_task_get_current);
EXPORT_SYMBOL(bdmf_mutex_init);
EXPORT_SYMBOL(bdmf_mutex_lock);
EXPORT_SYMBOL(bdmf_mutex_unlock);
EXPORT_SYMBOL(bdmf_ta_mutex_init);
EXPORT_SYMBOL(bdmf_ta_mutex_lock);
EXPORT_SYMBOL(bdmf_ta_mutex_unlock);
EXPORT_SYMBOL(bdmf_ta_mutex_delete);
EXPORT_SYMBOL(bdmf_reent_fastlock_init);
EXPORT_SYMBOL(bdmf_reent_fastlock_lock);
EXPORT_SYMBOL(bdmf_reent_fastlock_unlock);
EXPORT_SYMBOL(bdmf_vprint);
EXPORT_SYMBOL(bdmf_print);
EXPORT_SYMBOL(bdmf_file_open);
EXPORT_SYMBOL(bdmf_file_close);
EXPORT_SYMBOL(bdmf_file_read);
EXPORT_SYMBOL(bdmf_file_write);
EXPORT_SYMBOL(bdmf_sysb_headroom_size_set);
EXPORT_SYMBOL(bdmf_sysb_databuf_recycle);
EXPORT_SYMBOL(bdmf_sysb_recycle);
EXPORT_SYMBOL(bdmf_pbuf_init);
EXPORT_SYMBOL(bdmf_pbuf_alloc);
EXPORT_SYMBOL(bdmf_pbuf_free);
EXPORT_SYMBOL(bdmf_pbuf_from_sysb);
EXPORT_SYMBOL(bdmf_pbuf_to_sysb);
EXPORT_SYMBOL(bdmf_sysb_is_pbuf);
EXPORT_SYMBOL(bdmf_timer_init);
EXPORT_SYMBOL(bdmf_int_connect);
EXPORT_SYMBOL(bdmf_int_disconnect);
EXPORT_SYMBOL(bdmf_int_enable);
EXPORT_SYMBOL(bdmf_int_disable);
EXPORT_SYMBOL(bdmf_queue_create);
EXPORT_SYMBOL(bdmf_queue_delete);
EXPORT_SYMBOL(bdmf_queue_send);
EXPORT_SYMBOL(bdmf_queue_receive);
EXPORT_SYMBOL(bdmf_semaphore_create);
EXPORT_SYMBOL(bdmf_semaphore_give);
EXPORT_SYMBOL(bdmf_semaphore_take);
