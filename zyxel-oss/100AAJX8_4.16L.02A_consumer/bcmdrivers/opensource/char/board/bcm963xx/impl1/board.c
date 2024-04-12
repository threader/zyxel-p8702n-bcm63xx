/*
<:copyright-BRCM:2002:GPL/GPL:standard

   Copyright (c) 2002 Broadcom Corporation
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
/***************************************************************************
* File Name  : board.c
*
* Description: This file contains Linux character device driver entry
*              for the board related ioctl calls: flash, get free kernel
*              page and dump kernel memory, etc.
*
*
***************************************************************************/

/* Includes. */
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/pagemap.h>
#include <asm/uaccess.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/if.h>
#include <linux/pci.h>
#include <linux/ctype.h>
#include <linux/proc_fs.h>
#include <linux/smp.h>
#include <linux/version.h>
#include <linux/reboot.h>
#include <linux/bcm_assert_locks.h>
#include <asm/delay.h>
#include <asm/io.h>
#include <linux/fcntl.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/mtd/mtd.h>

#include <bcmnetlink.h>
#include <net/sock.h>
#include <bcmtypes.h>
#include <bcm_map_part.h>
#include <board.h>
#if defined(CONFIG_BCM_6802_MoCA)
#include "./bbsi/bbsi.h"
#else
#include <spidevices.h>
#endif

#define  BCMTAG_EXE_USE
#include <bcmTag.h>
#include <boardparms.h>
#include <boardparms_voice.h>
#include <flash_api.h>
#include <bcm_intr.h>
#include <flash_common.h>
#include <shared_utils.h>
#include <bcm_pinmux.h>
#include <bcmpci.h>
#include <linux/bcm_log.h>
#include <bcmSpiRes.h>
//extern unsigned int flash_get_reserved_bytes_at_end(const FLASH_ADDR_INFO *fInfo);
#include <pushbutton.h>

#ifdef MSTC_MODIFY // __MSTC__, Paul Ho, for WLAN/WPS Button
#include <linux/signal.h>
#endif

#if defined(CONFIG_BCM96838)
#include "pmc_drv.h"
#endif

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
#if defined(CONFIG_SMP)
#include <linux/cpu.h>
#endif
#include "pmc_dsl.h"
#include "pmc_apm.h"
#endif

#if defined(CONFIG_BCM_EXT_TIMER)
#include "bcm_ext_timer.h"
#endif

#if defined(BRCM_FTTDP)
#include <dsldsp_operation.h>	
#endif

/* Typedefs. */

#define SES_EVENT_BTN_PRESSED      0x00000001
#define SES_EVENTS                 SES_EVENT_BTN_PRESSED /*OR all values if any*/
#if defined (WIRELESS)
#define SES_LED_OFF                0
#define SES_LED_ON                 1
#define SES_LED_BLINK              2

#if defined(CONFIG_BCM96362) || defined(CONFIG_BCM963268) || defined(CONFIG_BCM96318) || defined(CONFIG_BCM96828)
#define WLAN_ONBOARD_SLOT	WLAN_ONCHIP_DEV_SLOT
#else
#define WLAN_ONBOARD_SLOT       1 /* Corresponds to IDSEL -- EBI_A11/PCI_AD12 */
#endif

#define BRCM_VENDOR_ID       0x14e4
#define BRCM_WLAN_DEVICE_IDS 0x4300
#define BRCM_WLAN_DEVICE_IDS_DEC 43

#define WLAN_ON   1
#define WLAN_OFF  0
#endif

typedef struct
{
    unsigned long ulId;
    char chInUse;
    char chReserved[3];
} MAC_ADDR_INFO, *PMAC_ADDR_INFO;

typedef struct
{
    unsigned long ulNumMacAddrs;
    unsigned char ucaBaseMacAddr[NVRAM_MAC_ADDRESS_LEN];
    MAC_ADDR_INFO MacAddrs[1];
} MAC_INFO, *PMAC_INFO;

typedef struct
{
    unsigned char gponSerialNumber[NVRAM_GPON_SERIAL_NUMBER_LEN];
    unsigned char gponPassword[NVRAM_GPON_PASSWORD_LEN];
} GPON_INFO, *PGPON_INFO;

typedef struct
{
    unsigned long eventmask;
} BOARD_IOC, *PBOARD_IOC;


/*Dyinggasp callback*/
typedef void (*cb_dgasp_t)(void *arg);
typedef struct _CB_DGASP__LIST
{
    struct list_head list;
    char name[IFNAMSIZ];
    cb_dgasp_t cb_dgasp_fn;
    void *context;
}CB_DGASP_LIST , *PCB_DGASP_LIST;


/*watchdog timer callback*/
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
typedef int (*cb_watchdog_t)(void *arg);
typedef struct _CB_WDOG__LIST
{
    struct list_head list;
    char name[IFNAMSIZ];
    cb_watchdog_t cb_wd_fn;
    void *context;
}CB_WDOG_LIST , *PCB_WDOG_LIST;
#endif

/* Externs. */
extern struct file *fget_light(unsigned int fd, int *fput_needed);
extern unsigned long getMemorySize(void);
extern void __init boardLedInit(void);
extern void boardLedCtrl(BOARD_LED_NAME, BOARD_LED_STATE);
#if defined(CONFIG_BCM96838)
extern int otp_is_boot_secure(void);
#endif
#ifdef CONFIG_MSTC_WDT
extern int mstc_wdt_init(void);
extern void mstc_wdt_exit(void);
extern int mstc_wdt_add_proc(void);
extern void mstc_wdt_del_proc(void);
#endif

/* Prototypes. */
static void set_mac_info( void );
static void set_gpon_info( void );
static int board_open( struct inode *inode, struct file *filp );
static ssize_t board_read(struct file *filp,  char __user *buffer, size_t count, loff_t *ppos);
static unsigned int board_poll(struct file *filp, struct poll_table_struct *wait);
static int board_release(struct inode *inode, struct file *filp);
static int board_ioctl( struct inode *inode, struct file *flip, unsigned int command, unsigned long arg );
#if defined(HAVE_UNLOCKED_IOCTL)
static long board_unlocked_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
#endif

static BOARD_IOC* borad_ioc_alloc(void);
static void borad_ioc_free(BOARD_IOC* board_ioc);

/*
 * flashImageMutex must be acquired for all write operations to
 * nvram, CFE, or fs+kernel image.  (cfe and nvram may share a sector).
 */
DEFINE_MUTEX(flashImageMutex);
static void writeNvramDataCrcLocked(PNVRAM_DATA pNvramData);
static PNVRAM_DATA readNvramData(void);
#ifdef MSTC_MODIFY //__MSTC__, Dennis
static void mustUpdateNvramfield(PNVRAM_DATA nData,PNVRAM_DATA oData);
#endif
#if defined(HAVE_UNLOCKED_IOCTL)
static DEFINE_MUTEX(ioctlMutex);
#endif

/* DyingGasp function prototype */
#if  !defined(CONFIG_BCM960333) 
static irqreturn_t kerSysDyingGaspIsr(int irq, void * dev_id);
static void kerSysEnableDyingGaspInterrupt( void );
static void kerSysDisableDyingGaspInterrupt( void );
#endif
static void __init kerSysInitDyingGaspHandler( void );
static void __exit kerSysDeinitDyingGaspHandler( void );
/* -DyingGasp function prototype - */
/* dgaspMutex protects list add and delete, but is ignored during isr. */
static DEFINE_MUTEX(dgaspMutex);
static volatile int isDyingGaspTriggered = 0;

static int ConfigCs(BOARD_IOCTL_PARMS *parms);

#if defined (WIRELESS) && !defined(MSTC_WPS_BTN_SCE) // __MSTC__, Paul Ho, __MSTC__, Autumn
#if defined(CONFIG_BCM96368) || defined(CONFIG_BCM96328) || defined(CONFIG_BCM963268)
static GPIO_STATE_t GetGpio(int gpio);
#endif
#endif /* !defined(MSTC_WPS_BTN_SCE) */

#if defined(CONFIG_BCM96318)
static void __init kerSysInit6318Reset( void );
#endif

static irqreturn_t mocaHost_isr(int irq, void *dev_id);

static irqreturn_t  sesBtn_isr(int irq, void *dev_id);
static Bool         sesBtn_pressed(void);
static void __init  sesBtn_mapIntr(int context);
static void __init  ses_board_init(void);
static void __exit  ses_board_deinit(void);

#if defined (WIRELESS)
static unsigned int sesBtn_poll(struct file *file, struct poll_table_struct *wait);
static ssize_t sesBtn_read(struct file *file,  char __user *buffer, size_t count, loff_t *ppos);
static void __init sesLed_mapGpio(void);
#ifdef MSTC_MODIFY
static void sesLed_ctrl(int action, BOARD_LED_NAME ledName);
#else
static void sesLed_ctrl(int action);
#endif
static void __init kerSysScreenPciDevices(void);
static void kerSetWirelessPD(int state);
#endif

static void str_to_num(char* in, char *out, int len);
static int add_proc_files(void);
static int del_proc_files(void);
static int proc_get_param(char *page, char **start, off_t off, int cnt, int *eof, void *data);
static int proc_set_param(struct file *f, const char *buf, unsigned long cnt, void *data);
static int proc_set_led(struct file *f, const char *buf, unsigned long cnt, void *data);

static irqreturn_t reset_isr(int irq, void *dev_id);

#if defined(CONFIG_BCM_WATCHDOG_TIMER)
static void __init kerSysInitWatchdogCBList( void );
static int proc_get_watchdog(char *page, char **start, off_t off, int cnt, int *eof, void *data);
static int proc_set_watchdog(struct file *f, const char *buf, unsigned long cnt, void *data);
#endif /* defined(CONFIG_BCM_WATCHDOG_TIMER) */
#if defined(CONFIG_BCM_WATCHDOG_TIMER) || !defined(CONFIG_BCM960333)
static void start_watchdog(unsigned int timer, unsigned int reset);
#endif

// macAddrMutex is used by kerSysGetMacAddress and kerSysReleaseMacAddress
// to protect access to g_pMacInfo
static DEFINE_MUTEX(macAddrMutex);
static PMAC_INFO g_pMacInfo = NULL;
static PGPON_INFO g_pGponInfo = NULL;
static unsigned long g_ulSdramSize;
static int g_ledInitialized = 0;
static wait_queue_head_t g_board_wait_queue;
static CB_DGASP_LIST *g_cb_dgasp_list_head = NULL;
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
static CB_WDOG_LIST *g_cb_wdog_list_head = NULL;
#endif

#define MAX_PAYLOAD_LEN 64
static struct sock *g_monitor_nl_sk;
static int g_monitor_nl_pid = 0 ;
static void kerSysInitMonitorSocket( void );
static void kerSysCleanupMonitorSocket( void );



#if defined(CONFIG_BCM_6802_MoCA)
void board_mocaInit(int mocaChipNum);
#endif

static kerSysMacAddressNotifyHook_t kerSysMacAddressNotifyHook = NULL;
#ifdef MSTC_MODIFY
#if defined(CV_RESTORE_BTN_PER) && !defined(CONFIG_AUXFS_JFFS2)
/* restore default work structure */
static struct work_struct restoreDefaultWork;
#endif
#else
/* restore default work structure */
static struct work_struct restoreDefaultWork;
#endif
static struct file_operations board_fops =
{
    open:       board_open,
#if defined(HAVE_UNLOCKED_IOCTL)
    unlocked_ioctl: board_unlocked_ioctl,
#else
    ioctl:      board_ioctl,
#endif
    poll:       board_poll,
    read:       board_read,
    release:    board_release,
};

uint32 board_major = 0;
#ifdef MSTC_MODIFY //__MSTC__, Leo, support LED power management
static int pwrMngtOn_usb0 = 1;
static int pwrMngtOn_usb1 = 1;
#endif

#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Ssupport USB LED   
int chkUsbtimer=0;
static int isUSBDevice0Inserted=0;
static int isUSBDevice1Inserted=0;
struct timer_list gUsbTimer;
#endif

static unsigned short sesBtn_irq = BP_NOT_DEFINED;
static unsigned short sesBtn_gpio = BP_NOT_DEFINED;
static unsigned short sesBtn_polling = 0;
static struct timer_list sesBtn_timer;
static atomic_t sesBtn_active;
static atomic_t sesBtn_forced;
static unsigned short resetBtn_gpio = BP_NOT_DEFINED;
#ifndef MSTC_MODIFY
static unsigned short resetBtn2_gpio = BP_NOT_DEFINED;
#endif
#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
#define NUM_EXT_INT    (INTERRUPT_ID_EXTERNAL_5-INTERRUPT_ID_EXTERNAL_0+1)
#elif defined(CONFIG_BCM963381) 
#define NUM_EXT_INT    (INTERRUPT_ID_EXTERNAL_7-INTERRUPT_ID_EXTERNAL_0+1)
#else
#define NUM_EXT_INT    (INTERRUPT_ID_EXTERNAL_3-INTERRUPT_ID_EXTERNAL_0+1)
#endif
static unsigned int extIntrInfo[NUM_EXT_INT];

typedef struct
{
    int dev;
    MocaHostIntrCallback mocaCallback;
    void * userArg;
    int irq;
    int intrGpio;
    atomic_t enableCount;
} MOCA_INTR_ARG, *PMOCA_INTR_ARG;

static DEFINE_SPINLOCK(mocaint_spinlock);
static MOCA_INTR_ARG mocaIntrArg[BP_MOCA_MAX_NUM];
static BP_MOCA_INFO mocaInfo[BP_MOCA_MAX_NUM];
static int mocaChipNum = BP_MOCA_MAX_NUM;
#ifndef MSTC_MODIFY
static int restore_in_progress = 0;
#endif
#if defined (WIRELESS)
static unsigned short sesLed_gpio = BP_NOT_DEFINED;
#ifdef MSTC_MODIFY
#ifdef MSTC_WLAN_WPS_BTN_SEPARATE //__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
static unsigned short wlanBtn_irq = BP_NOT_DEFINED;
#endif
#if 1 // __MSTC__, Paul Ho, for WLAN/WPS Button
struct timer_list gResetTimer;
#if defined(MSTC_WPS_BTN_SCE)
static int chkWlanEnCounter = 0;
#else
int chkWlanBtnTimer;  /* add for WLAN Button */
struct timer_list gWlanTimer; /* add for WLAN Button */
#endif /* defined(MSTC_WPS_BTN_SCE) */
#ifdef MSTC_WLAN_WPS_BTN_SEPARATE //__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
struct timer_list gResetTimerWlanBtn;
static int chkWlanBtnCounter = 0;
#endif

pid_t  pid_no;

#endif // end, __MSTC__, Paul Ho
#endif
#endif /* defined (WIRELESS) */

#ifdef MSTC_MODIFY
#if 1 //__MSTC__, FuChia
uint8 stopblinkwl = 0;
uint8 stopblink = 0;
uint8 buttonTest = 0;
#define ktimems (HZ/10)
int ktimems_v = ktimems;
static unsigned short resetBtn_irq = BP_NOT_DEFINED;
int chkResetCounter = 0;
#endif //__MSTC__, FuChia

#ifdef NORWAY_BTN_LED_OFF	
struct timer_list LedoffTimer; /* add for Led all off */
static uint32 chkLedAllOffCounter = 0;
static uint8 chkintenLed = 2;
static uint16 LedAllOffMinutes = 30;
static uint8 LedAllOffMode = 3;
static uint8 LedAllOff = 0;
static uint8 ledBootFlag = 1;
#endif


#ifdef CONFIG_MSTC_RECORD_CALL_TRACE
extern char oops_folder_path[64];
extern int oops_file_num;

static void mstc_koops_init(void)
{
	strcpy(oops_folder_path, CONFIG_MSTC_RECORD_CALL_TRACE_PATH);
	oops_file_num = CONFIG_MSTC_OOPS_FILE_NUM;
	return;
}

static int proc_koops_folder_get(char *page, char **start, off_t off, int cnt, int *eof, void *data)
{
	int len;

	len = sprintf(page, "Default folder: %s\nCurrent folder: %s\n", CONFIG_MSTC_RECORD_CALL_TRACE_PATH, oops_folder_path);

	return len;
}

static int proc_koops_folder_set(struct file *f, const char *buf, unsigned long cnt, void *data)
{
	char input[64];
	char tmp[64];
	int len = 0;
	int ret = -1;

	memset(input, 0, sizeof(input));
	memset(tmp, 0, sizeof(tmp));

	if ((cnt > 63) || (copy_from_user(input, buf, cnt) != 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	ret = sscanf(input, "%s\n", tmp);
	len = strlen(tmp);
	if((ret != 1) || (len <= 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	if(tmp[len - 1] != '/') {
		if(len < 63) {
			tmp[len] = '/';
		} else {
			printk("Invalid value!");
			return -EFAULT;
		}
	}

	strcpy(oops_folder_path, tmp);

	return cnt;
}

static int proc_koops_fn_get(char *page, char **start, off_t off, int cnt, int *eof, void *data)
{
	int len;

	len = sprintf(page, "Default file_num: %d\nCurrent file_num: %d\n", CONFIG_MSTC_OOPS_FILE_NUM, oops_file_num);

	return len;
}

static int proc_koops_fn_set(struct file *f, const char *buf, unsigned long cnt, void *data)
{
	char input[64];
	int fn;
	int ret = -1;

	memset(input, 0, sizeof(input));

	if ((cnt > 63) || (copy_from_user(input, buf, cnt) != 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	ret = sscanf(input, "%d\n", &fn);
	if((ret != 1) || (fn <= 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	oops_file_num = fn;

	return cnt;
}

static int mstc_koops_add_proc(void)
{
	struct proc_dir_entry *koops_dir = NULL;
	struct proc_dir_entry *koops_folder = NULL;
	struct proc_dir_entry *koops_file_num = NULL;

	koops_dir = proc_mkdir("mstc_koops", NULL);
	if (koops_dir == NULL) {
		printk(KERN_ERR "add_proc_files: failed to create mstc_kd proc files!\n");
		return -1;
	}

	koops_folder = create_proc_entry("oops_folder", 0644, koops_dir);
	if (koops_folder == NULL) {
		printk(KERN_ERR "add_proc_files: failed to create koops_folder proc files!\n");
		return -1;
	}
	koops_folder->read_proc = proc_koops_folder_get;
	koops_folder->write_proc = proc_koops_folder_set;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	/* New linux no longer requires proc_dir_entry->owner field. */
	#else
	koops_folder->owner = THIS_MODULE;
	#endif

	koops_file_num = create_proc_entry("file_num", 0644, koops_dir);
	if (koops_file_num == NULL) {
		printk(KERN_ERR "add_proc_files: failed to create coredump_preThreshold proc files!\n");
		return -1;
	}
	koops_file_num->read_proc = proc_koops_fn_get;
	koops_file_num->write_proc = proc_koops_fn_set;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	/* New linux no longer requires proc_dir_entry->owner field. */
	#else
	koops_file_num->owner = THIS_MODULE;
	#endif

	return 0;
}

static void mstc_koops_del_proc(void)
{
	remove_proc_entry("oops_folder", NULL);
	remove_proc_entry("file_num", NULL);

	return;
}
#endif

#ifdef CONFIG_MSTC_DEC_AND_GZIP_CORE
extern char core_folder_path[64];
extern int core_pre_threshold;
extern int core_post_threshold;

static void mstc_coredump_init(void)
{
	strcpy(core_folder_path, CONFIG_MSTC_COREDUMP_FOLDER_PATH);
	core_pre_threshold = CONFIG_MSTC_CORE_SIZE_PRE_THRESHOLD;
	core_post_threshold = CONFIG_MSTC_CORE_SIZE_POST_THRESHOLD;
	return;
}

static int proc_coredump_folder_get(char *page, char **start, off_t off, int cnt, int *eof, void *data)
{
	int len;

	len = sprintf(page, "Default folder: %s\nCurrent folder: %s\n", CONFIG_MSTC_COREDUMP_FOLDER_PATH, core_folder_path);

	return len;
}

static int proc_coredump_folder_set(struct file *f, const char *buf, unsigned long cnt, void *data)
{
	char input[64];
	char tmp[64];
	int len = 0;
	int ret = -1;

	memset(input, 0, sizeof(input));
	memset(tmp, 0, sizeof(tmp));

	if ((cnt > 63) || (copy_from_user(input, buf, cnt) != 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	ret = sscanf(input, "%s\n", tmp);
	len = strlen(tmp);
	if((ret != 1) || (len <= 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	if(tmp[len - 1] != '/') {
		if(len < 63) {
			tmp[len] = '/';
		} else {
			printk("Invalid value!");
			return -EFAULT;
		}
	}

	strcpy(core_folder_path, tmp);

	return cnt;
}

static int proc_coredump_preThreshold_get(char *page, char **start, off_t off, int cnt, int *eof, void *data)
{
	int len;

	len = sprintf(page, "Default preThreshold: %d\nCurrent preThreshold: %d\n", CONFIG_MSTC_CORE_SIZE_PRE_THRESHOLD, core_pre_threshold);

	return len;
}

static int proc_coredump_preThreshold_set(struct file *f, const char *buf, unsigned long cnt, void *data)
{
	char input[64];
	int threshold;
	int ret = -1;

	memset(input, 0, sizeof(input));

	if ((cnt > 63) || (copy_from_user(input, buf, cnt) != 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	ret = sscanf(input, "%d\n", &threshold);
	if((ret != 1) || (threshold <= 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	core_pre_threshold = threshold;

	return cnt;
}

static int proc_coredump_postThreshold_get(char *page, char **start, off_t off, int cnt, int *eof, void *data)
{
	int len;

	len = sprintf(page, "Default postThreshold: %d\nCurrent postThreshold: %d\n", CONFIG_MSTC_CORE_SIZE_POST_THRESHOLD, core_post_threshold);

	return len;
}

static int proc_coredump_postThreshold_set(struct file *f, const char *buf, unsigned long cnt, void *data)
{
	char input[64];
	int threshold;
	int ret = -1;

	memset(input, 0, sizeof(input));

	if ((cnt > 63) || (copy_from_user(input, buf, cnt) != 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	ret = sscanf(input, "%d\n", &threshold);
	if((ret != 1) || (threshold <= 0)) {
		printk("Invalid value!");
		return -EFAULT;
	}

	core_post_threshold = threshold;

	return cnt;
}

static int mstc_coredump_add_proc(void)
{
	struct proc_dir_entry *coredump_dir = NULL;
	struct proc_dir_entry *coredump_folder = NULL;
	struct proc_dir_entry *coredump_preThreshold = NULL;
	struct proc_dir_entry *coredump_postThreshold = NULL;

	coredump_dir = proc_mkdir("mstc_coredump", NULL);
	if (coredump_dir == NULL) {
		printk(KERN_ERR "add_proc_files: failed to create mstc_kd proc files!\n");
		return -1;
	}

	coredump_folder = create_proc_entry("coredump_folder", 0644, coredump_dir);
	if (coredump_folder == NULL) {
		printk(KERN_ERR "add_proc_files: failed to create coredump_folder proc files!\n");
		return -1;
	}
	coredump_folder->read_proc = proc_coredump_folder_get;
	coredump_folder->write_proc = proc_coredump_folder_set;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	/* New linux no longer requires proc_dir_entry->owner field. */
	#else
	coredump_folder->owner = THIS_MODULE;
	#endif

	coredump_preThreshold = create_proc_entry("preThreshold", 0644, coredump_dir);
	if (coredump_preThreshold == NULL) {
		printk(KERN_ERR "add_proc_files: failed to create coredump_preThreshold proc files!\n");
		return -1;
	}
	coredump_preThreshold->read_proc = proc_coredump_preThreshold_get;
	coredump_preThreshold->write_proc = proc_coredump_preThreshold_set;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	/* New linux no longer requires proc_dir_entry->owner field. */
	#else
	coredump_preThreshold->owner = THIS_MODULE;
	#endif

	coredump_postThreshold = create_proc_entry("postThreshold", 0644, coredump_dir);
	if (coredump_postThreshold == NULL) {
		printk(KERN_ERR "add_proc_files: failed to create coredump_postThreshold proc files!\n");
		return -1;
	}
	coredump_postThreshold->read_proc = proc_coredump_postThreshold_get;
	coredump_postThreshold->write_proc = proc_coredump_postThreshold_set;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	/* New linux no longer requires proc_dir_entry->owner field. */
	#else
	coredump_postThreshold->owner = THIS_MODULE;
	#endif

	return 0;
}

static void mstc_coredump_del_proc(void)
{
	remove_proc_entry("coredump_folder", NULL);
	remove_proc_entry("preThreshold", NULL);
	remove_proc_entry("postThreshold", NULL);

	return;
}

#endif //CONFIG_MSTC_DEC_AND_GZIP_CORE

#if 1 //__MSTC__, RaynorChung: Ssupport USB LED   
void setUsbDeviceInserted(BOARD_LED_NAME ledName, int status){
	if (status == 1)
	{
	  #if 1  //__MSTC__, Leo, support LED power management
	    if (ledName == kLedUSB0){
			isUSBDevice0Inserted = 1;
            pwrMngtOn_usb0 = 1;
	    }
		if (ledName == kLedUSB1){
			isUSBDevice1Inserted = 1;
			pwrMngtOn_usb1 = 1;
		}
	  #else
		if (ledName == kLedUSB0)
			isUSBDevice0Inserted = 1;
		else if (ledName == kLedUSB1)
			isUSBDevice1Inserted = 1;
	  #endif
	} else if (status == 0)
	{
		if (ledName == kLedUSB0)
			isUSBDevice0Inserted = 0;
		else if (ledName == kLedUSB1)
			isUSBDevice1Inserted = 0;
	}
	return;
}


void UsbTimerExpire(void)
{
    chkUsbtimer++;
    if (chkUsbtimer > 5) /*check every 500 mini seconds*/
	{
	  #if 1  //__MSTC__, Leo, support LED power management
		if (isUSBDevice0Inserted && pwrMngtOn_usb0)
	  #else
		if (isUSBDevice0Inserted)
	  #endif
			kerSysLedCtrl(kLedUSB0,kLedStateOn);
		else
			kerSysLedCtrl(kLedUSB0,kLedStateOff);

      #if 1  //__MSTC__, Leo, support LED power management
		if (isUSBDevice1Inserted && pwrMngtOn_usb1)
	  #else
		if (isUSBDevice1Inserted)
	  #endif
			kerSysLedCtrl(kLedUSB1,kLedStateOn);
		else
			kerSysLedCtrl(kLedUSB1,kLedStateOff);

		chkUsbtimer =0;
    }

	// start timer
	init_timer(&gUsbTimer);
	gUsbTimer.function = (void*)UsbTimerExpire;
	gUsbTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
	add_timer (&gUsbTimer);

}
#endif


#if defined (WIRELESS) && !defined(MSTC_WPS_BTN_SCE) // __MSTC__, Paul Ho, for WLAN Button
void WlanTimerExpire(void)
{
   struct task_struct *p;
   const int CV_WLAN_BTN_PERIOD = 1; /* "Pressing the button for at least 1 second is enabling / disabling the WLAN." */
   unsigned short wlBtnGpio;

   if (BpGetWirelessBtnGpio(&wlBtnGpio) != BP_SUCCESS)
   {
      printk(KERN_ERR "Cannot get WLAN on/off button GPIO!!\n");   
      return;
   }
   
   if (GetGpio(wlBtnGpio) == 0)
   {
      chkWlanBtnTimer++;
   }
   else
   {
      if ( chkWlanBtnTimer >= CV_WLAN_BTN_PERIOD*10 ) {
         printk(KERN_WARNING "\r\n WLAN Behavior %d\n", chkWlanBtnTimer);
         /*if ( buttonTest == 1 ) {
            if ( stopblinkwl == 0 ) {
               kerSysLedCtrl(kLedPowerG, kLedStateFastBlinkContinues);
               stopblinkwl = 1;
            }
            else {
               kerSysLedCtrl(kLedPowerG, kLedStateOn);
               stopblinkwl = 0;
            }
         }
         else*/ {
            printk(KERN_WARNING "\r\n WLAN Button Pressed, send SIGUSR1 to wlmngr pid_no:%d\n", pid_no);
            p = find_task_by_vpid(pid_no);
            if(p!=NULL)
                send_sig(SIGUSR1,p,0);
            else
                printk(KERN_ERR "WLAN on/off can't work.\n");
         }
      }
      /*else if ( chkWlanBtnTimer >0 && buttonTest == 1 ) 
      {
         printk(KERN_ERR "\r\n WLAN Button Pressed\n");
         if ( stopblinkwl == 0 ) {
            kerSysLedCtrl(kLedPowerG, kLedStateFastBlinkContinues);
            stopblinkwl = 1;
         }
         else {
            kerSysLedCtrl(kLedPowerG, kLedStateOn);
            stopblinkwl = 0;
         }
      }*/
      chkWlanBtnTimer = 0;
   }

	// start timer
	init_timer(&gWlanTimer);
	gWlanTimer.function = (void*)WlanTimerExpire;
	gWlanTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
	add_timer (&gWlanTimer);
}
#endif /* defined (WIRELESS) */
#endif //MSTC_MODIFY


#ifdef NORWAY_BTN_LED_OFF	 // __MSTC__, Paul Ho, for WPS Button (Merged from 406 common trunk)
void LedAllOffTimerExpire ( void )
{
	uint32 LedAllOffMinutes_t = 0;
	if (ledBootFlag == 1){
		LedAllOffMinutes_t = (LedAllOffMinutes*60*10 + 1250);
	}else{
		LedAllOffMinutes_t = (LedAllOffMinutes*60*10);
	}
	if (LedAllOffMinutes == 0){
		LedAllOffMinutes_t = LedAllOffMinutes_t + 20;
	}
	
	if ( LedAllOffMode != 0 ) {
		if ( chkLedAllOffCounter <= LedAllOffMinutes_t + 30 ) {	
			if ( chkLedAllOffCounter == LedAllOffMinutes_t ) {	
				LED->ledLinkActSelLow &= 0xFF00FFFF;							 // disable wan
				GPIO->LEDCtrl &= 0xFFDFFFFF;	                                 //disable intel 	
				chkintenLed = 2;
				LedAllOff = 1;
				kerSysSendtoMonitorTask(MSG_EXTSW_LED_OFF, "1", sizeof(char));  
			}

			chkLedAllOffCounter++;
			if ((ledBootFlag==1) && (chkLedAllOffCounter==(LedAllOffMinutes_t+30))) {
				ledBootFlag = 0;
			}
		}
	}

	// start timer
	init_timer(&LedoffTimer);
	LedoffTimer.function = (void*)LedAllOffTimerExpire;
	LedoffTimer.expires = jiffies+ktimems_v;	   // timer expires in ~100ms
	add_timer (&LedoffTimer);

	return;
}
#endif


#if defined(CONFIG_BCM_WATCHDOG_TIMER)
static DEFINE_SPINLOCK(watchdog_spinlock);
volatile static struct watchdog_struct {
    unsigned int enabled;       // enable watchdog
    unsigned int timer;         // unit is ns
    unsigned int suspend;       // watchdog function is suspended
    unsigned int userMode;      // enable user mode watchdog
    unsigned int userThreshold; // user mode watchdog threshold to reset cpe
    unsigned int userTimeout;   // user mode timeout
} watchdog_data = {0, 5000000, 0, 0, 8, 0};
/* watchdog restart work */
static struct work_struct watchdogRestartWork;
static int watchdog_restart_in_progress = 0;
#endif

#if defined(MODULE)
int init_module(void)
{
    return( brcm_board_init() );
}

void cleanup_module(void)
{
    if (MOD_IN_USE)
        printk("brcm flash: cleanup_module failed because module is in use\n");
    else
        brcm_board_cleanup();
}
#endif //MODULE


static int map_external_irq (int irq)
{
    int map_irq;
    irq &= ~BP_EXT_INTR_FLAGS_MASK;

    switch (irq) {
    case BP_EXT_INTR_0   :
        map_irq = INTERRUPT_ID_EXTERNAL_0;
        break ;
    case BP_EXT_INTR_1   :
        map_irq = INTERRUPT_ID_EXTERNAL_1;
        break ;
    case BP_EXT_INTR_2   :
        map_irq = INTERRUPT_ID_EXTERNAL_2;
        break ;
    case BP_EXT_INTR_3   :
        map_irq = INTERRUPT_ID_EXTERNAL_3;
        break ;
#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
    case BP_EXT_INTR_4   :
        map_irq = INTERRUPT_ID_EXTERNAL_4;
        break ;
    case BP_EXT_INTR_5   :
        map_irq = INTERRUPT_ID_EXTERNAL_5;
        break ;
#endif
#if defined(CONFIG_BCM963381)
    case BP_EXT_INTR_6   :
        map_irq = INTERRUPT_ID_EXTERNAL_6;
        break ;
    case BP_EXT_INTR_7   :
        map_irq = INTERRUPT_ID_EXTERNAL_7;
        break ;
#endif
    default           :
        printk ("Invalid External Interrupt definition \n") ;
        map_irq = 0 ;
        break ;
    }

    return (map_irq) ;
}

static int set_ext_irq_info(unsigned short ext_irq)
{
	int irq_idx, rc = 0;

	irq_idx = (ext_irq&~BP_EXT_INTR_FLAGS_MASK)-BP_EXT_INTR_0;

	if( extIntrInfo[irq_idx] == (unsigned int)(-1) )
		extIntrInfo[irq_idx] = ext_irq;
	else
	{
		/* make sure all the interrupt sharing this irq number has the trigger type and shared */
		if( ext_irq != (unsigned int)extIntrInfo[irq_idx] )
		{
			printk("Invalid ext intr type for BP_EXT_INTR_%d: 0x%x vs 0x%x\r\n", irq_idx, ext_irq, extIntrInfo[irq_idx]);
			extIntrInfo[irq_idx] |= BP_EXT_INTR_CONFLICT_MASK;
			rc = -1;
		}
	}

	return rc;
}

static void init_ext_irq_info(void)
{
	int i, j;
	unsigned short intr;

	/* mark each entry invalid */
	for(i=0; i<NUM_EXT_INT; i++)
		extIntrInfo[i] = (unsigned int)(-1);

	/* collect all the external interrupt info from bp */
	if( BpGetResetToDefaultExtIntr(&intr) == BP_SUCCESS )
		set_ext_irq_info(intr);

	if( BpGetResetToDefault2ExtIntr(&intr) == BP_SUCCESS )
		set_ext_irq_info(intr);

	if( BpGetWirelessSesExtIntr(&intr) == BP_SUCCESS )
		set_ext_irq_info(intr);

#if defined(CONFIG_BCM96838)
	if( BpGetPmdAlarmExtIntr(&intr) == BP_SUCCESS )
		set_ext_irq_info(intr);
#endif

	for( i = 0; i < mocaChipNum; i++ )
	{
		for( j = 0; j < BP_MOCA_MAX_INTR_NUM; j++ )
		{
			if( mocaInfo[i].intr[j] != BP_NOT_DEFINED )
			{
#if defined(CONFIG_BCM96838)
				unsigned short irqIdx = map_external_irq(mocaInfo[i].intr[j]) - INTERRUPT_ID_EXTERNAL_0;
				unsigned short gpio = mocaInfo[i].intrGpio[j];
				gpio &= BP_GPIO_NUM_MASK;
				PERF->ext_irq_muxsel0 |= ( (gpio&EXT_IRQ_MASK_LOW) << (irqIdx*EXT_IRQ_OFF_LOW) );
				DBGPERF->Dbg_extirqmuxsel0_1 |= ( ((gpio&EXT_IRQ_MASK_HIGH)>>EXT_IRQ_OFF_LOW) << (irqIdx*EXT_IRQ_OFF_HIGH) );
#endif
				set_ext_irq_info(mocaInfo[i].intr[j]);
			}
		}
	}
	return;
}

PBP_MOCA_INFO boardGetMocaInfo(int dev)
{
	if( dev >= mocaChipNum)
		return NULL;
	else
		return &mocaInfo[dev];
}

#if defined(CONFIG_BCM960333)
static void mapBcm960333GpioToIntr( unsigned int gpio, unsigned long extIrq )
{
    unsigned int extIrqIdx = map_external_irq(extIrq) - INTERRUPT_ID_EXTERNAL_0;
    volatile uint32 * pMuxReg = &(GPIO->GPIOMuxCtrl_0) + gpio/4;
    int gpioShift = (gpio % 4) * 8;
    uint32 gpioMask = 0x7f << gpioShift;

    BcmHalExternalIrqMask(extIrqIdx);
    *pMuxReg = (*pMuxReg & (~gpioMask)) | (extIrqIdx << gpioShift);
    GPIO->GPIOFuncMode |= 1 << gpio;
}
#endif

static int kerSysIsBatteryEnabled(void)
{
#if defined(CONFIG_BCM_BMU)
    unsigned short bmuen;

    if (BpGetBatteryEnable(&bmuen) == BP_SUCCESS) {
        return (bmuen);
    }
#endif
    return 0;
}

static void __init kerSysInitBatteryManagementUnit(void)
{
#if defined(CONFIG_BCM_BMU)
    if (kerSysIsBatteryEnabled()) {
        pmc_apm_power_up();
#if defined(CONFIG_BCM963148)
        // APM_ANALOG_BG_BOOST defaults to 0 in 63148 and needs to be set
        APM_PUB->reg_apm_analog_bg |= APM_ANALOG_BG_BOOST;
#endif
    }
#endif
}

/* A global variable used by Power Management and other features to determine if Voice is idle or not */
volatile int isVoiceIdle = 1;
EXPORT_SYMBOL(isVoiceIdle);

int ext_irq_connect(int irq, unsigned int param, FN_HANDLER isr)
{
    int rc = 0;

    irq = map_external_irq(irq);
    rc = BcmHalMapInterrupt(isr, param, irq);
#if !defined(CONFIG_ARM)
    if (!rc)
        BcmHalInterruptEnable(irq);
#endif
    return rc;
}
EXPORT_SYMBOL(ext_irq_connect);

void ext_irq_enable(int irq, int enable)
{
    irq = map_external_irq(irq);
#if !defined(CONFIG_ARM)
    if (enable)
        BcmHalInterruptEnable(irq);
    else
        BcmHalInterruptDisable(irq);
#endif
}
EXPORT_SYMBOL(ext_irq_enable);

static int __init brcm_board_init( void )
{
	int ret;
#ifdef MSTC_MODIFY // __MSTC__, FuChia
#else		
    unsigned short rstToDflt_irq;
    unsigned short rstToDflt2_irq;
#endif		
    int ext_irq_idx;
#ifndef MSTC_MODIFY// __MSTC__, FuChia	
    int ext_irq2_idx;
#endif		
    bcmLogSpiCallbacks_t loggingCallbacks;

#ifdef CONFIG_MSTC_WDT
	/* Start hardware watchdog */
	mstc_wdt_init();
#endif

#ifdef CONFIG_MSTC_RECORD_CALL_TRACE
	mstc_koops_init();
#endif

#ifdef CONFIG_MSTC_DEC_AND_GZIP_CORE
	mstc_coredump_init();
#endif
    ret = register_chrdev(BOARD_DRV_MAJOR, "brcmboard", &board_fops );
    if (ret < 0)
        printk( "brcm_board_init(major %d): fail to register device.\n",BOARD_DRV_MAJOR);
    else
    {
        printk("brcmboard: brcm_board_init entry\n");

#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Ssupport USB LED   
		init_timer(&gUsbTimer);
		gUsbTimer.function = (void*)UsbTimerExpire;
		gUsbTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
		add_timer (&gUsbTimer);
#endif

#ifdef NORWAY_BTN_LED_OFF		
		// start Led all off timer
		init_timer(&LedoffTimer);
		LedoffTimer.function = (void*)LedAllOffTimerExpire;
		LedoffTimer.expires = jiffies+ktimems_v; 	   // timer expires in ~100ms
		add_timer (&LedoffTimer);
#endif

#if defined (WIRELESS) && !defined(MSTC_WPS_BTN_SCE) // __MSTC__, Paul Ho, for WLAN Button
		// start WLAN button timer
		init_timer(&gWlanTimer);
		gWlanTimer.function = (void*)WlanTimerExpire;
		gWlanTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
		add_timer (&gWlanTimer);
#endif			
        board_major = BOARD_DRV_MAJOR;

        g_ulSdramSize = getMemorySize();
        set_mac_info();
        set_gpon_info();

        BpGetMocaInfo(mocaInfo, &mocaChipNum);

        init_ext_irq_info();

        init_waitqueue_head(&g_board_wait_queue);
#if defined (WIRELESS)
        kerSysScreenPciDevices();
        kerSetWirelessPD(WLAN_ON);
#endif
        ses_board_init();

        kerSysInitMonitorSocket();
        kerSysInitDyingGaspHandler();
#if defined(CONFIG_BCM96318)
        kerSysInit6318Reset();
#endif
        kerSysInitBatteryManagementUnit();

        boardLedInit();
        g_ledInitialized = 1;
#ifdef MSTC_MODIFY// __MSTC__, FuChia
////MSTC, we only need to chang original code "rstToDflt_irq" to "resetBtn_irq"
		kerSysLedCtrl(kLedPowerG, kLedStateSlowBlinkContinues);

		if( BpGetResetToDefaultExtIntr(&resetBtn_irq) == BP_SUCCESS )
		{
			ext_irq_idx = (resetBtn_irq&~BP_EXT_INTR_FLAGS_MASK)-BP_EXT_INTR_0;
#if defined(CONFIG_BCM963381)
			kerSysInitPinmuxInterface(BP_PINMUX_FNTYPE_IRQ | ext_irq_idx);
#endif


#if defined(CONFIG_BCM960333)
			if (BpGetResetToDefaultExtIntrGpio(&resetBtn_gpio) == BP_SUCCESS) {
				resetBtn_gpio &= BP_GPIO_NUM_MASK;
				if ( ext_irq_idx  != BP_NOT_DEFINED && resetBtn_gpio != BP_NOT_DEFINED ) {
					mapBcm960333GpioToIntr( resetBtn_gpio, resetBtn_irq );
				}
			}
#endif
			
			if (!IsExtIntrConflict(extIntrInfo[ext_irq_idx]))
			{
				static int dev = -1;
				int hookisr = 1;

				if (IsExtIntrShared(resetBtn_irq))
				{
					/* get the gpio and make it input dir */
					if( (resetBtn_gpio != BP_NOT_DEFINED) || (BpGetResetToDefaultExtIntrGpio(&resetBtn_gpio) == BP_SUCCESS) )
					{
						resetBtn_gpio &= BP_GPIO_NUM_MASK;
						printk("brcm_board_init: Reset config Interrupt gpio is %d\n", resetBtn_gpio);
						kerSysSetGpioDirInput(resetBtn_gpio); // Set gpio for input.
						dev = resetBtn_gpio;
					}
					else
					{
						printk("brcm_board_init: Reset config gpio definition not found \n");
						hookisr= 0;
					}
				}
				
				if(hookisr)
				{
					resetBtn_irq = map_external_irq (resetBtn_irq);
					BcmHalMapInterrupt((FN_HANDLER)reset_isr, (unsigned int)&dev, resetBtn_irq);
#if !defined(CONFIG_ARM)
					BcmHalInterruptEnable(resetBtn_irq);
#endif
				}
			}
		}

#else

        if( BpGetResetToDefaultExtIntr(&rstToDflt_irq) == BP_SUCCESS )
        {
            ext_irq_idx = (rstToDflt_irq&~BP_EXT_INTR_FLAGS_MASK)-BP_EXT_INTR_0;
#if defined(CONFIG_BCM963381)
            kerSysInitPinmuxInterface(BP_PINMUX_FNTYPE_IRQ | ext_irq_idx);
#endif


#if defined(CONFIG_BCM960333)
            if (BpGetResetToDefaultExtIntrGpio(&resetBtn_gpio) == BP_SUCCESS) {
                resetBtn_gpio &= BP_GPIO_NUM_MASK;
                if ( ext_irq_idx  != BP_NOT_DEFINED && resetBtn_gpio != BP_NOT_DEFINED ) {
                    mapBcm960333GpioToIntr( resetBtn_gpio, rstToDflt_irq );
                }
            }
#endif
            
            if (!IsExtIntrConflict(extIntrInfo[ext_irq_idx]))
            {
                static int dev = -1;
                int hookisr = 1;

                if (IsExtIntrShared(rstToDflt_irq))
                {
                    /* get the gpio and make it input dir */
                    if( (resetBtn_gpio != BP_NOT_DEFINED) || (BpGetResetToDefaultExtIntrGpio(&resetBtn_gpio) == BP_SUCCESS) )
                    {
                    	resetBtn_gpio &= BP_GPIO_NUM_MASK;
                        printk("brcm_board_init: Reset config Interrupt gpio is %d\n", resetBtn_gpio);
                        kerSysSetGpioDirInput(resetBtn_gpio); // Set gpio for input.
                        dev = resetBtn_gpio;
                    }
                    else
                    {
                        printk("brcm_board_init: Reset config gpio definition not found \n");
                        hookisr= 0;
                    }
                }
                
                if(hookisr)
                {
                    ext_irq_connect(rstToDflt_irq, (unsigned int)&dev, (FN_HANDLER)reset_isr);
                }
            }
        }

        if( BpGetResetToDefault2ExtIntr(&rstToDflt2_irq) == BP_SUCCESS )
        {
            ext_irq2_idx = (rstToDflt2_irq&~BP_EXT_INTR_FLAGS_MASK)-BP_EXT_INTR_0;
#if defined(CONFIG_BCM960333)            
            if (BpGetResetToDefault2ExtIntrGpio(&resetBtn2_gpio) == BP_SUCCESS) {
                resetBtn2_gpio &= BP_GPIO_NUM_MASK;
                if ( ext_irq2_idx  != BP_NOT_DEFINED && resetBtn2_gpio != BP_NOT_DEFINED ) {
                    mapBcm960333GpioToIntr( resetBtn2_gpio, rstToDflt2_irq );
                }
            }
#endif

            if (!IsExtIntrConflict(extIntrInfo[ext_irq2_idx]))
            {
                static int dev = -1;
                int hookisr = 1;

                if (IsExtIntrShared(rstToDflt2_irq))
                {
                    /* get the gpio and make it input dir */
                    if( (resetBtn2_gpio != BP_NOT_DEFINED) || (BpGetResetToDefault2ExtIntrGpio(&resetBtn2_gpio) == BP_SUCCESS) )
                    {
                        resetBtn2_gpio &= BP_GPIO_NUM_MASK;
                        kerSysSetGpioDirInput(resetBtn2_gpio); // Set gpio for input.
                        dev = resetBtn2_gpio;
                    }
                    else
                    {
                        hookisr= 0;
                    }
                }
                
                if(hookisr)
                {
                    ext_irq_connect(rstToDflt2_irq, (unsigned int)&dev, (FN_HANDLER)reset_isr);
                }
            }
            else 
            {
                printk("[%s.%d]: conflict exists %d (%08x)\n", __func__, __LINE__, ext_irq2_idx, extIntrInfo[ext_irq2_idx]);
            }
        }
#endif
#if defined(CONFIG_BCM_EXT_TIMER)
        init_hw_timers();
#endif
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
        kerSysInitWatchdogCBList();
#endif

#if defined(CONFIG_BCM_CPLD1)
        // Reserve SPI bus to control external CPLD for Standby Timer
        BcmCpld1Initialize();
#endif
    }

    add_proc_files();

#if   defined(CONFIG_BCM_6802_MoCA)
    board_mocaInit(mocaChipNum);
    loggingCallbacks.kerSysSlaveRead   = kerSysBcmSpiSlaveRead;
    loggingCallbacks.kerSysSlaveWrite  = kerSysBcmSpiSlaveWrite;
    loggingCallbacks.bpGet6829PortInfo = NULL;
#endif
    loggingCallbacks.reserveSlave      = BcmSpiReserveSlave;
    loggingCallbacks.syncTrans         = BcmSpiSyncTrans;
    bcmLog_registerSpiCallbacks(loggingCallbacks);

    return ret;
}

static void __init set_mac_info( void )
{
    NVRAM_DATA *pNvramData;
    unsigned long ulNumMacAddrs;

    if (NULL == (pNvramData = readNvramData()))
    {
        printk("set_mac_info: could not read nvram data\n");
        return;
    }

    ulNumMacAddrs = pNvramData->ulNumMacAddrs;

    if( ulNumMacAddrs > 0 && ulNumMacAddrs <= NVRAM_MAC_COUNT_MAX )
    {
        unsigned long ulMacInfoSize =
            sizeof(MAC_INFO) + ((sizeof(MAC_ADDR_INFO)) * (ulNumMacAddrs-1));

        g_pMacInfo = (PMAC_INFO) kmalloc( ulMacInfoSize, GFP_KERNEL );

        if( g_pMacInfo )
        {
            memset( g_pMacInfo, 0x00, ulMacInfoSize );
            g_pMacInfo->ulNumMacAddrs = pNvramData->ulNumMacAddrs;
            memcpy( g_pMacInfo->ucaBaseMacAddr, pNvramData->ucaBaseMacAddr,
                NVRAM_MAC_ADDRESS_LEN );
        }
        else
            printk("ERROR - Could not allocate memory for MAC data\n");
    }
    else
        printk("ERROR - Invalid number of MAC addresses (%ld) is configured.\n",
        ulNumMacAddrs);
    kfree(pNvramData);
}

static int gponParamsAreErased(NVRAM_DATA *pNvramData)
{
    int i;
    int erased = 1;

    for(i=0; i<NVRAM_GPON_SERIAL_NUMBER_LEN-1; ++i) {
        if((pNvramData->gponSerialNumber[i] != (char) 0xFF) &&
            (pNvramData->gponSerialNumber[i] != (char) 0x00)) {
                erased = 0;
                break;
        }
    }

    if(!erased) {
        for(i=0; i<NVRAM_GPON_PASSWORD_LEN-1; ++i) {
            if((pNvramData->gponPassword[i] != (char) 0xFF) &&
                (pNvramData->gponPassword[i] != (char) 0x00)) {
                    erased = 0;
                    break;
            }
        }
    }

    return erased;
}

static void __init set_gpon_info( void )
{
    NVRAM_DATA *pNvramData;

    if (NULL == (pNvramData = readNvramData()))
    {
        printk("set_gpon_info: could not read nvram data\n");
        return;
    }

    g_pGponInfo = (PGPON_INFO) kmalloc( sizeof(GPON_INFO), GFP_KERNEL );

    if( g_pGponInfo )
    {
        if ((pNvramData->ulVersion < NVRAM_FULL_LEN_VERSION_NUMBER) ||
            gponParamsAreErased(pNvramData))
        {
            strcpy( g_pGponInfo->gponSerialNumber, DEFAULT_GPON_SN );
            strcpy( g_pGponInfo->gponPassword, DEFAULT_GPON_PW );
        }
        else
        {
            strncpy( g_pGponInfo->gponSerialNumber, pNvramData->gponSerialNumber,
                NVRAM_GPON_SERIAL_NUMBER_LEN );
            g_pGponInfo->gponSerialNumber[NVRAM_GPON_SERIAL_NUMBER_LEN-1]='\0';
            strncpy( g_pGponInfo->gponPassword, pNvramData->gponPassword,
                NVRAM_GPON_PASSWORD_LEN );
            g_pGponInfo->gponPassword[NVRAM_GPON_PASSWORD_LEN-1]='\0';
        }
    }
    else
    {
        printk("ERROR - Could not allocate memory for GPON data\n");
    }
    kfree(pNvramData);
}

void __exit brcm_board_cleanup( void )
{
    printk("brcm_board_cleanup()\n");
    del_proc_files();

    if (board_major != -1)
    {
#if !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)
        ses_board_deinit();
#endif
        kerSysDeinitDyingGaspHandler();
        kerSysCleanupMonitorSocket();
        unregister_chrdev(board_major, "board_ioctl");

    }
}

static BOARD_IOC* borad_ioc_alloc(void)
{
    BOARD_IOC *board_ioc =NULL;
    board_ioc = (BOARD_IOC*) kmalloc( sizeof(BOARD_IOC) , GFP_KERNEL );
    if(board_ioc)
    {
        memset(board_ioc, 0, sizeof(BOARD_IOC));
    }
    return board_ioc;
}

static void borad_ioc_free(BOARD_IOC* board_ioc)
{
    if(board_ioc)
    {
        kfree(board_ioc);
    }
}


static int board_open( struct inode *inode, struct file *filp )
{
    filp->private_data = borad_ioc_alloc();

    if (filp->private_data == NULL)
        return -ENOMEM;

    return( 0 );
}

static int board_release(struct inode *inode, struct file *filp)
{
    BOARD_IOC *board_ioc = filp->private_data;

    wait_event_interruptible(g_board_wait_queue, 1);
    borad_ioc_free(board_ioc);

    return( 0 );
}


static unsigned int board_poll(struct file *filp, struct poll_table_struct *wait)
{
    unsigned int mask = 0;
#if defined (WIRELESS)
    BOARD_IOC *board_ioc = filp->private_data;
#endif

    poll_wait(filp, &g_board_wait_queue, wait);
#if defined (WIRELESS)
    if(board_ioc->eventmask & SES_EVENTS){
        mask |= sesBtn_poll(filp, wait);
    }
#endif

    return mask;
}

static ssize_t board_read(struct file *filp,  char __user *buffer, size_t count, loff_t *ppos)
{
#if defined (WIRELESS)
    BOARD_IOC *board_ioc = filp->private_data;
    if(board_ioc->eventmask & SES_EVENTS){
        return sesBtn_read(filp, buffer, count, ppos);
    }
#endif
    return 0;
}

/***************************************************************************
// Function Name: getCrc32
// Description  : caculate the CRC 32 of the given data.
// Parameters   : pdata - array of data.
//                size - number of input data bytes.
//                crc - either CRC32_INIT_VALUE or previous return value.
// Returns      : crc.
****************************************************************************/
static UINT32 getCrc32(byte *pdata, UINT32 size, UINT32 crc)
{
    while (size-- > 0)
        crc = (crc >> 8) ^ Crc32_table[(crc ^ *pdata++) & 0xff];

    return crc;
}

/** calculate the CRC for the nvram data block and write it to flash.
 * Must be called with flashImageMutex held.
 */
static void writeNvramDataCrcLocked(PNVRAM_DATA pNvramData)
{
    UINT32 crc = CRC32_INIT_VALUE;

    BCM_ASSERT_HAS_MUTEX_C(&flashImageMutex);

    pNvramData->ulCheckSum = 0;
    crc = getCrc32((char *)pNvramData, sizeof(NVRAM_DATA), crc);
    pNvramData->ulCheckSum = crc;
    kerSysNvRamSet((char *)pNvramData, sizeof(NVRAM_DATA), 0);
}


/** read the nvramData struct from the in-memory copy of nvram.
 * The caller is not required to have flashImageMutex when calling this
 * function.  However, if the caller is doing a read-modify-write of
 * the nvram data, then the caller must hold flashImageMutex.  This function
 * does not know what the caller is going to do with this data, so it
 * cannot assert flashImageMutex held or not when this function is called.
 *
 * @return pointer to NVRAM_DATA buffer which the caller must free
 *         or NULL if there was an error
 */
static PNVRAM_DATA readNvramData(void)
{
    UINT32 crc = CRC32_INIT_VALUE, savedCrc;
    NVRAM_DATA *pNvramData;

    // use GFP_ATOMIC here because caller might have flashImageMutex held
    if (NULL == (pNvramData = kmalloc(sizeof(NVRAM_DATA), GFP_ATOMIC)))
    {
        printk("readNvramData: could not allocate memory\n");
        return NULL;
    }

    kerSysNvRamGet((char *)pNvramData, sizeof(NVRAM_DATA), 0);
    savedCrc = pNvramData->ulCheckSum;
    pNvramData->ulCheckSum = 0;
    crc = getCrc32((char *)pNvramData, sizeof(NVRAM_DATA), crc);
    if (savedCrc != crc)
    {
        // this can happen if we write a new cfe image into flash.
        // The new image will have an invalid nvram section which will
        // get updated to the inMemNvramData.  We detect it here and
        // commonImageWrite will restore previous copy of nvram data.
        kfree(pNvramData);
        pNvramData = NULL;
    }

    return pNvramData;
}



//**************************************************************************************
// Utitlities for dump memory, free kernel pages, mips soft reset, etc.
//**************************************************************************************

/***********************************************************************
* Function Name: dumpaddr
* Description  : Display a hex dump of the specified address.
***********************************************************************/
void dumpaddr( unsigned char *pAddr, int nLen )
{
    static char szHexChars[] = "0123456789abcdef";
    char szLine[80];
    char *p = szLine;
    unsigned char ch, *q;
    int i, j;
    unsigned long ul;

    while( nLen > 0 )
    {
        sprintf( szLine, "%8.8lx: ", (unsigned long) pAddr );
        p = szLine + strlen(szLine);

        for(i = 0; i < 16 && nLen > 0; i += sizeof(long), nLen -= sizeof(long))
        {
            ul = *(unsigned long *) &pAddr[i];
            q = (unsigned char *) &ul;
            for( j = 0; j < sizeof(long); j++ )
            {
                *p++ = szHexChars[q[j] >> 4];
                *p++ = szHexChars[q[j] & 0x0f];
                *p++ = ' ';
            }
        }

        for( j = 0; j < 16 - i; j++ )
            *p++ = ' ', *p++ = ' ', *p++ = ' ';

        *p++ = ' ', *p++ = ' ', *p++ = ' ';

        for( j = 0; j < i; j++ )
        {
            ch = pAddr[j];
            *p++ = (ch > ' ' && ch < '~') ? ch : '.';
        }

        *p++ = '\0';
        printk( "%s\r\n", szLine );

        pAddr += i;
    }
    printk( "\r\n" );
} /* dumpaddr */


/* On 6318, the Sleep mode is used to force a reset on PVT Monitor and ASB blocks */
#if defined(CONFIG_BCM96318)
static void __init kerSysInit6318Reset( void )
{
    /* Re-initialize the sleep registers because they are not cleared on reset */
    RTC->RtcSleepRtcEnable &= ~RTC_SLEEP_RTC_ENABLE;
    RTC->RtcSleepRtcEvent &= ~RTC_SLEEP_RTC_EVENT;

    /* A magic word is saved in scratch register to identify unintentional resets */
    /* (Scratch register is not cleared on reset) */
    if (RTC->RtcSleepCpuScratchpad == 0x600DBEEF) {
        /* When Magic word is seen during reboot, there was an unintentional reset */
        printk("Previous reset was unintentional, performing full reset sequence\n");
        kerSysMipsSoftReset();
    }
    RTC->RtcSleepCpuScratchpad = 0x600DBEEF;
}

static void kerSys6318Reset( void )
{
    unsigned short plcGpio;

    /* Use GPIO to control the PLC and wifi chip reset on 6319 PLC board */
    if( BpGetPLCPwrEnGpio(&plcGpio) == BP_SUCCESS )
    {
        kerSysSetGpioState(plcGpio, kGpioInactive);
        /* Delay to ensure WiFi and PLC are powered down */
        udelay(5000);
    }

    /* On 6318, the Sleep mode is used to force a reset on PVT Monitor and ASB blocks */
    /* Configure the sleep mode and duration */
    /* then wait for system to come out of reset when the timer expires */
    PLL_PWR->PllPwrControlIddqCtrl &= ~IDDQ_SLEEP;
    RTC->RtcSleepWakeupMask = RTC_SLEEP_RTC_IRQ;
    RTC->RtcSleepCpuScratchpad = 0x01010101; // We are intentionally starting the reset sequence
    RTC->RtcSleepRtcCountL = 0x00020000; // Approx 5 ms
    RTC->RtcSleepRtcCountH = 0x0;
    RTC->RtcSleepRtcEnable = RTC_SLEEP_RTC_ENABLE;
    RTC->RtcSleepModeEnable = RTC_SLEEP_MODE_ENABLE;
    // while(1); // Spin occurs in calling function.  Do not spin here
}
#endif


/** this function actually does two things, stop other cpu and reset mips.
 * Kept the current name for compatibility reasons.  Image upgrade code
 * needs to call the two steps separately.
 */
void kerSysMipsSoftReset(void)
{
	unsigned long cpu;
	cpu = smp_processor_id();
	printk(KERN_INFO "kerSysMipsSoftReset: called on cpu %lu\n", cpu);
	// FIXME - Once in many thousands of reboots, this function could 
	// fail to complete a reboot.  Arm the watchdog just in case.
#if defined (CONFIG_BCM96838)
	WDTIMER->WD0Ctl = 0xEE00;
	WDTIMER->WD0Ctl = 0x00EE;
	WDTIMER->WD0DefCount = 5 * (FPERIPH/1000000);
	WDTIMER->WD0Ctl = 0xFF00;
	WDTIMER->WD0Ctl = 0x00FF;
        BcmHalInterruptDisable(INTERRUPT_ID_TIMER);
#elif !defined(CONFIG_BCM96318)
	TIMER->WatchDogCtl = 0xEE00;
	TIMER->WatchDogCtl = 0x00EE;
	TIMER->WatchDogDefCount = 5 * (FPERIPH/1000000);
	TIMER->WatchDogCtl = 0xFF00;
	TIMER->WatchDogCtl = 0x00FF;
        BcmHalInterruptDisable(INTERRUPT_ID_TIMER);
#endif
	stopOtherCpu();
	local_irq_disable();  // ignore interrupts, just execute reset code now
	resetPwrmgmtDdrMips();
}

extern void stop_other_cpu(void);  // in arch/mips/kernel/smp.c

void stopOtherCpu(void)
{
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
#if defined(CONFIG_SMP)
    /* in ARM, CPU#0 should be the last one to get shut down, and for
     * both 63138 and 63148, we have dualcore system, so we can hardcode
     * cpu_down() on CPU#1. Also, if this function is handled by the 
     * CPU which is going to be shut down, kernel will transfer the
     * current task to another CPU.  Thus when we return from cpu_down(),
     * the task is still running. */
    cpu_down(1);
#endif
#else
#if defined(CONFIG_SMP)
    stop_other_cpu();
#elif defined(CONFIG_BCM_ENDPOINT_MODULE) && defined(CONFIG_BCM_BCMDSP_MODULE)
    unsigned long cpu = (read_c0_diag3() >> 31) ? 0 : 1;

	// Disable interrupts on the other core and allow it to complete processing 
	// and execute the "wait" instruction
    printk(KERN_INFO "stopOtherCpu: stopping cpu %lu\n", cpu);	
    PERF->IrqControl[cpu].IrqMask = 0;
    mdelay(5);
#endif
#endif /* !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148) */
}

void resetPwrmgmtDdrMips(void)
{
#if   defined(CONFIG_BCM96838)
    WDTIMER->WD0DefCount = 0;
    WDTIMER->WD0Ctl = 0xee00;
    WDTIMER->WD0Ctl = 0x00ee;
    WDTIMER->WD1DefCount = 0;
    WDTIMER->WD1Ctl = 0xee00;
    WDTIMER->WD1Ctl = 0x00ee;
    PERF->TimerControl |= SOFT_RESET_0;
#else
#if defined (CONFIG_BCM963268)
    MISC->miscVdslControl &= ~(MISC_VDSL_CONTROL_VDSL_MIPS_RESET | MISC_VDSL_CONTROL_VDSL_MIPS_POR_RESET );
#endif
#if !defined (CONFIG_BCM96838) && !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM960333) && !defined(CONFIG_BCM963148) && !defined(CONFIG_BCM963381)
    // Power Management on Ethernet Ports may have brought down EPHY PLL
    // and soft reset below will lock-up 6362 if the PLL is not up
    // therefore bring it up here to give it time to stabilize
    GPIO->RoboswEphyCtrl &= ~EPHY_PWR_DOWN_DLL;
#endif
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
    /* stop SF2 switch from sending packet to runner, or the DMA might get stuck.
     * Also give it time to complete the ongoing DMA transaction. */
    ETHSW_CORE->imp_port_state &= ~ETHSW_IPS_LINK_PASS;
#endif

    // let UART finish printing
    udelay(100);


#if defined(CONFIG_BCM_CPLD1)
    // Determine if this was a request to enter Standby mode
    // If yes, this call won't return and a hard reset will occur later
    BcmCpld1CheckShutdownMode();
#endif


#if defined(CONFIG_BCM96328) || defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148) || defined(CONFIG_BCM963381)
    TIMER->SoftRst = 1;
#elif defined(CONFIG_BCM96318)
    kerSys6318Reset();
#elif defined(CONFIG_BCM960333)
    /*
     * After a soft-reset, one of the reserved bits of TIMER->SoftRst remains
     * enabled and the next soft-reset won't work unless TIMER->SoftRst is
     * set to 0.
     */
    TIMER->SoftRst = 0;
    TIMER->SoftRst |= SOFT_RESET;
#else
#if !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)
    PERF->pll_control |= SOFT_RESET;    // soft reset mips
#endif
#endif
#endif
    for(;;) {} // spin mips and wait soft reset to take effect
}

unsigned long kerSysGetMacAddressType( unsigned char *ifName )
{
    unsigned long macAddressType = MAC_ADDRESS_ANY;
#if defined(MSTC_ALL_WAN_SAME_MAC) || defined(MSTC_DIF_L2_SAME_MAC) // __MSTC__, Richard Huang (ALL WAN Using the same MAC)
	if(strstr(ifName, IF_NAME_WETH))
	{
		macAddressType = MAC_ADDRESS_WAN;
#if defined(MSTC_DIF_L2_SAME_MAC)
		macAddressType |= 0x01;
#endif
	}
	else if(strstr(ifName, IF_NAME_WMOCA))
	{
		macAddressType = MAC_ADDRESS_WAN;
#if defined(MSTC_DIF_L2_SAME_MAC)
		macAddressType |= 0x02;
#endif
	}
	else if(strstr(ifName, IF_NAME_WPTM))
	{
		macAddressType = MAC_ADDRESS_WAN;
#if defined(MSTC_DIF_L2_SAME_MAC)
		macAddressType |= 0x04;
#endif
	}
	else if(strstr(ifName, IF_NAME_WATM))
	{
		macAddressType = MAC_ADDRESS_WAN;
#if defined(MSTC_DIF_L2_SAME_MAC)
		macAddressType |= 0x08;
#endif
	}
	else
#endif
    if(strstr(ifName, IF_NAME_ETH))
    {
        macAddressType = MAC_ADDRESS_ETH;
    }
    else if(strstr(ifName, IF_NAME_USB))
    {
        macAddressType = MAC_ADDRESS_USB;
    }
    else if(strstr(ifName, IF_NAME_WLAN))
    {
        macAddressType = MAC_ADDRESS_WLAN;
    }
    else if(strstr(ifName, IF_NAME_MOCA))
    {
        macAddressType = MAC_ADDRESS_MOCA;
    }
    else if(strstr(ifName, IF_NAME_ATM))
    {
        macAddressType = MAC_ADDRESS_ATM;
    }
    else if(strstr(ifName, IF_NAME_PTM))
    {
        macAddressType = MAC_ADDRESS_PTM;
    }
    else if(strstr(ifName, IF_NAME_GPON) || strstr(ifName, IF_NAME_VEIP))
    {
        macAddressType = MAC_ADDRESS_GPON;
    }
    else if(strstr(ifName, IF_NAME_EPON))
    {
        macAddressType = MAC_ADDRESS_EPON;
    }

    return macAddressType;
}

static inline void kerSysMacAddressNotify(unsigned char *pucaMacAddr, MAC_ADDRESS_OPERATION op)
{
    if(kerSysMacAddressNotifyHook)
    {
        kerSysMacAddressNotifyHook(pucaMacAddr, op);
    }
}

int kerSysMacAddressNotifyBind(kerSysMacAddressNotifyHook_t hook)
{
    int nRet = 0;

    if(hook && kerSysMacAddressNotifyHook)
    {
        printk("ERROR: kerSysMacAddressNotifyHook already registered! <0x%08lX>\n",
               (unsigned long)kerSysMacAddressNotifyHook);
        nRet = -EINVAL;
    }
    else
    {
        kerSysMacAddressNotifyHook = hook;
    }

    return nRet;
}
#ifdef MSTC_MODIFY // __MSTC__, Richard Huang
int kerSysGetBaseMacAddress( unsigned char *pucaMacAddr, unsigned int index )
{
    int nRet = 0;

    memcpy((unsigned char *) pucaMacAddr+index,
            &g_pMacInfo->ucaBaseMacAddr[index],
            NVRAM_MAC_ADDRESS_LEN - index);

    return( nRet );
}
#endif

#if defined (WIRELESS)
void kerSysSesEventTrigger( int forced )
{
   if (forced) {
      atomic_set (&sesBtn_forced, 1);
   }
   wake_up_interruptible(&g_board_wait_queue);
}
#endif
static void getNthMacAddr( unsigned char *pucaMacAddr, unsigned long n)
{
    unsigned long macsequence = 0;
    /* Work with only least significant three bytes of base MAC address */
    macsequence = (pucaMacAddr[3] << 16) | (pucaMacAddr[4] << 8) | pucaMacAddr[5];
    macsequence = (macsequence + n) & 0xffffff;
    pucaMacAddr[3] = (macsequence >> 16) & 0xff;
    pucaMacAddr[4] = (macsequence >> 8) & 0xff;
    pucaMacAddr[5] = (macsequence ) & 0xff;

}
static unsigned long getIdxForNthMacAddr( const unsigned char *pucaBaseMacAddr, unsigned char *pucaMacAddr)
{
    unsigned long macSequence = 0;
    unsigned long baseMacSequence = 0;
    
    macSequence = (pucaMacAddr[3] << 16) | (pucaMacAddr[4] << 8) | pucaMacAddr[5];
    baseMacSequence = (pucaBaseMacAddr[3] << 16) | (pucaBaseMacAddr[4] << 8) | pucaBaseMacAddr[5];

    return macSequence - baseMacSequence;
}
/* Allocates requested number of consecutive MAC addresses */
int kerSysGetMacAddresses( unsigned char *pucaMacAddr, unsigned int num_addresses, unsigned long ulId )
{
    int nRet = -EADDRNOTAVAIL;
    PMAC_ADDR_INFO pMai = NULL;
    PMAC_ADDR_INFO pMaiFreeId = NULL, pMaiFreeIdTemp;
    unsigned long i = 0, j = 0, ulIdxId = 0;

    mutex_lock(&macAddrMutex);

    /* Start with the base address */
    memcpy( pucaMacAddr, g_pMacInfo->ucaBaseMacAddr, NVRAM_MAC_ADDRESS_LEN);

#if defined(CONFIG_BCM96828)||defined(CONFIG_BCM96838)
    /*As epon mac should not be dynamicly changed, always use last 1(SLLID) or 8(MLLID) mac address(es)*/
    if (ulId == MAC_ADDRESS_EPONONU)
    {
        i = g_pMacInfo->ulNumMacAddrs - num_addresses; 

        for (j = 0, pMai = &g_pMacInfo->MacAddrs[i]; j < num_addresses; j++) {
            pMaiFreeIdTemp = pMai + j;
            if (pMaiFreeIdTemp->chInUse != 0 && pMaiFreeIdTemp->ulId != MAC_ADDRESS_EPONONU) {
                printk("kerSysGetMacAddresses: epon mac address allocate failed, g_pMacInfo[%ld] reserved by 0x%lx\n", i+j, pMaiFreeIdTemp->ulId);	
                break;
            }
        }
		
        if (j >= num_addresses) {
            pMaiFreeId = pMai;
            ulIdxId = i;
        }
    }
    else
#endif	
    {
        for( i = 0, pMai = g_pMacInfo->MacAddrs; i < g_pMacInfo->ulNumMacAddrs;
            i++, pMai++ )
        {
#ifdef MSTC_MODIFY        
	        if( ulId == pMai->ulId || ulId == MAC_ADDRESS_ANY 
#if defined(MSTC_ALL_WAN_SAME_MAC) || defined(MSTC_DIF_L2_SAME_MAC) // __MSTC__, Richard Huang
	            || CHECK_WANID(pMai->ulId, ulId)
#endif
	        )       
#else   		
            if( ulId == pMai->ulId || ulId == MAC_ADDRESS_ANY )
#endif				
            {
                /* This MAC address has been used by the caller in the past. */
                getNthMacAddr( pucaMacAddr, i );
#if defined(MSTC_ALL_WAN_SAME_MAC) || defined(MSTC_DIF_L2_SAME_MAC) // __MSTC__, Richard Huang
	            if ( CHECK_WANID(pMai->ulId, ulId) ) {
	               pMai->ulId = ulId;
	            }
	            pMai->chInUse = (pMai->chInUse < 0)? 1 : pMai->chInUse + 1;
#else					
                pMai->chInUse = 1;
#endif				
                pMaiFreeId = NULL;
                nRet = 0;
                break;
            } else if( pMai->chInUse == 0 ) {
                /* check if it there are num_addresses to be checked starting from found MAC address */
                if ((i + num_addresses - 1) >= g_pMacInfo->ulNumMacAddrs) {
                    break;
                }
    
                for (j = 1; j < num_addresses; j++) {
                    pMaiFreeIdTemp = pMai + j;
                    if (pMaiFreeIdTemp->chInUse != 0) {
                        break;
                    }
                }
                if (j == num_addresses) {
                    pMaiFreeId = pMai;
                    ulIdxId = i;
                    break;
                }
            }
        }
    }

    if(pMaiFreeId )
    {
        /* An available MAC address was found. */
        getNthMacAddr( pucaMacAddr, ulIdxId );
        pMaiFreeIdTemp = pMai;
        for (j = 0; j < num_addresses; j++) {
            pMaiFreeIdTemp->ulId = ulId;
#if defined(MSTC_ALL_WAN_SAME_MAC) || defined(MSTC_DIF_L2_SAME_MAC) // __MSTC__, Richard Huang
            pMaiFreeIdTemp->chInUse = (pMaiFreeIdTemp->chInUse < 0)? 1 : pMaiFreeIdTemp->chInUse + 1;
#else			
            pMaiFreeIdTemp->chInUse = 1;
#endif			
            pMaiFreeIdTemp++;
        }
        nRet = 0;
    }

    mutex_unlock(&macAddrMutex);

    return( nRet );
} /* kerSysGetMacAddr */
int kerSysGetMacAddress( unsigned char *pucaMacAddr, unsigned long ulId )
{
    return kerSysGetMacAddresses(pucaMacAddr,1,ulId); /* Get only one address */
} /* kerSysGetMacAddr */


int kerSysReleaseMacAddresses( unsigned char *pucaMacAddr, unsigned int num_addresses )
{
    int i, nRet = -EINVAL;
    unsigned long ulIdx = 0;

    mutex_lock(&macAddrMutex);

    ulIdx = getIdxForNthMacAddr(g_pMacInfo->ucaBaseMacAddr, pucaMacAddr);

    if( ulIdx < g_pMacInfo->ulNumMacAddrs )
    {
        for(i=0; i<num_addresses; i++) {
            if ((ulIdx + i) < g_pMacInfo->ulNumMacAddrs) {
                PMAC_ADDR_INFO pMai = &g_pMacInfo->MacAddrs[ulIdx + i];
#if defined(MSTC_ALL_WAN_SAME_MAC) || defined(MSTC_DIF_L2_SAME_MAC) // __MSTC__, Richard Huang
		        if( pMai->chInUse > 0 )
		        {
		            pMai->chInUse--;
		            nRet = 0;
		        }
		        else {
		            pMai->chInUse = 0;
		            nRet = 0;
		        }
#else				
                if( pMai->chInUse == 1 )
                {
                    pMai->chInUse = 0;
                    nRet = 0;
                }
#endif					
            } else {
                printk("Request to release %d addresses failed as "
                    " the one or more of the addresses, starting from"
                    " %dth address from given address, requested for release"
                    " is not in the list of available MAC addresses \n", num_addresses, i);
                break;
            }
        }
    }

    mutex_unlock(&macAddrMutex);

    return( nRet );
} /* kerSysReleaseMacAddr */


int kerSysReleaseMacAddress( unsigned char *pucaMacAddr )
{
    return kerSysReleaseMacAddresses(pucaMacAddr,1); /* Release only one MAC address */

} /* kerSysReleaseMacAddr */


void kerSysGetGponSerialNumber( unsigned char *pGponSerialNumber )
{
    strcpy( pGponSerialNumber, g_pGponInfo->gponSerialNumber );
}


void kerSysGetGponPassword( unsigned char *pGponPassword )
{
    strcpy( pGponPassword, g_pGponInfo->gponPassword );
}

int kerSysGetSdramSize( void )
{
    return( (int) g_ulSdramSize );
} /* kerSysGetSdramSize */




/*Read Wlan Params data from CFE */
int kerSysGetWlanSromParams( unsigned char *wlanParams, unsigned short len)
{
    NVRAM_DATA *pNvramData;

    if (NULL == (pNvramData = readNvramData()))
    {
        printk("kerSysGetWlanSromParams: could not read nvram data\n");
        return -1;
    }

    memcpy( wlanParams,
           (char *)pNvramData + ((size_t) &((NVRAM_DATA *)0)->wlanParams),
            len );
    kfree(pNvramData);

    return 0;
}


unsigned char kerSysGetWlanFeature(void)
{
    NVRAM_DATA *pNvramData;

    unsigned char wlfeature=0;
    if (NULL == (pNvramData = readNvramData()))
    {
        printk("kerSysGetWlanSromParams: could not read nvram data\n");
        return -1;
    }
    wlfeature= (unsigned char)(pNvramData ->wlanParams[NVRAM_WLAN_PARAMS_LEN-1]);
    kfree(pNvramData);
    return wlfeature;
    
}

/*Read Wlan Params data from CFE */
int kerSysGetAfeId( unsigned long *afeId )
{
    NVRAM_DATA *pNvramData;

    if (NULL == (pNvramData = readNvramData()))
    {
        printk("kerSysGetAfeId: could not read nvram data\n");
        return -1;
    }

    afeId [0] = pNvramData->afeId[0];
    afeId [1] = pNvramData->afeId[1];
    kfree(pNvramData);

    return 0;
}

void kerSysLedCtrl(BOARD_LED_NAME ledName, BOARD_LED_STATE ledState)
{
#ifdef NORWAY_BTN_LED_OFF	
	uint32 LedAllOffMinutes_t = 0;
#endif
#ifdef MSTC_MODIFY  //__MSTC__, Leo, support LED power management
	if (ledName == kLedUSB0 && ledState == kLedStateOff) 
		pwrMngtOn_usb0 = 0;
	if (ledName == kLedUSB1 && ledState == kLedStateOff)
		pwrMngtOn_usb1 = 0;
	if (ledName == kLedUSB0 && ledState == kLedStateOn)
		pwrMngtOn_usb0 = 1;
	if (ledName == kLedUSB1 && ledState == kLedStateOn)
		pwrMngtOn_usb1 = 1;
#endif

#ifdef NORWAY_BTN_LED_OFF	
		if (ledBootFlag == 1){
		LedAllOffMinutes_t = (LedAllOffMinutes*60*10 + 1250);
		}else{
			LedAllOffMinutes_t = (LedAllOffMinutes*60*10);
		}
	if (LedAllOffMinutes == 0){
		LedAllOffMinutes_t = LedAllOffMinutes_t + 20;
	}

	
		if (( ledName == kLedInternetData ) && ( ledState == kLedStateOn )) {
			chkintenLed = 1;
		}else if (( ledName == kLedInternetData ) && ( ledState == kLedStateOff )) {
			chkintenLed =0;
		}
	
		if ( LedAllOffMode != 3 ) {
		if ((chkLedAllOffCounter >= LedAllOffMinutes_t + 30) && (ledName != kLedPowerG)){
				return;
			}
		}else {
		if ((chkLedAllOffCounter >= LedAllOffMinutes_t + 30) && ((ledName != kLedWlanEn) && (ledName != kLedPowerG))){
				return;
			}
		}
	
		if ((ledName == kLedInternetData) && ((ledState == kLedStateFastBlinkContinues) || (ledState == kLedStateSlowBlinkContinues)) ){
			return;
		}
#endif

    if (g_ledInitialized)
       boardLedCtrl(ledName, ledState);
}

/*functionto receive message from usersapce
 * Currently we dont expect any messages fromm userspace
 */
void kerSysRecvFrmMonitorTask(struct sk_buff *skb)
{

   /*process the message here*/
   printk(KERN_WARNING "unexpected skb received at %s \n",__FUNCTION__);
   kfree_skb(skb);
   return;
}

void kerSysInitMonitorSocket( void )
{
   g_monitor_nl_sk = netlink_kernel_create(&init_net, NETLINK_BRCM_MONITOR, 0, kerSysRecvFrmMonitorTask, NULL, THIS_MODULE);

   if(!g_monitor_nl_sk)
   {
      printk(KERN_ERR "Failed to create a netlink socket for monitor\n");
      return;
   }

}


void kerSysSendtoMonitorTask(int msgType, char *msgData, int msgDataLen)
{

   struct sk_buff *skb =  NULL;
   struct nlmsghdr *nl_msgHdr = NULL;
   unsigned int nl_msgLen;

   if(!g_monitor_nl_pid)
   {
      printk(KERN_INFO "message received before monitor task is initialized %s \n",__FUNCTION__);
      return;
   } 

   if(msgData && (msgDataLen > MAX_PAYLOAD_LEN))
   {
      printk(KERN_ERR "invalid message len in %s",__FUNCTION__);
      return;
   } 

   nl_msgLen = NLMSG_SPACE(msgDataLen);

   /*Alloc skb ,this check helps to call the fucntion from interrupt context */

   if(in_atomic())
   {
      skb = alloc_skb(nl_msgLen, GFP_ATOMIC);
   }
   else
   {
      skb = alloc_skb(nl_msgLen, GFP_KERNEL);
   }

   if(!skb)
   {
      printk(KERN_ERR "failed to alloc skb in %s",__FUNCTION__);
      return;
   }

   nl_msgHdr = (struct nlmsghdr *)skb->data;
   nl_msgHdr->nlmsg_type = msgType;
   nl_msgHdr->nlmsg_pid=0;/*from kernel */
   nl_msgHdr->nlmsg_len = nl_msgLen;
   nl_msgHdr->nlmsg_flags =0;

   if(msgData)
   {
      memcpy(NLMSG_DATA(nl_msgHdr),msgData,msgDataLen);
   }

   NETLINK_CB(skb).pid = 0; /*from kernel */

   skb->len = nl_msgLen; 

   netlink_unicast(g_monitor_nl_sk, skb, g_monitor_nl_pid, MSG_DONTWAIT);
   return;
}

void kerSysCleanupMonitorSocket(void)
{
   g_monitor_nl_pid = 0 ;
   sock_release(g_monitor_nl_sk->sk_socket);
}

// Must be called with flashImageMutex held
static PFILE_TAG getTagFromPartition(int imageNumber)
{
    // Define space for file tag structures for two partitions.  Make them static
    // so caller does not have to worry about allocation/deallocation.
    // Make sure they're big enough for the file tags plus an block number
    // (an integer) appended.
    static unsigned char sectAddr1[sizeof(FILE_TAG) + sizeof(int)];
    static unsigned char sectAddr2[sizeof(FILE_TAG) + sizeof(int)];

    int blk = 0;
    UINT32 crc;
    PFILE_TAG pTag = NULL;
    unsigned char *pBase = flash_get_memptr(0);
    unsigned char *pSectAddr = NULL;
#ifdef MSTC_MODIFY /* fix bootup with wrong rootfs address,  Mitrastar Kid, 20120509. */
	int newRootfsAddr = 0;
	int tagRootfsAddr = 0;
	int tagKernelAddr = 0;
#endif
    unsigned int reserverdBytersAuxfs = flash_get_reserved_bytes_auxfs();
#if (defined(INC_NAND_FLASH_DRIVER)) && (INC_NAND_FLASH_DRIVER==1)	
#else
    unsigned int sectSize = (unsigned int) flash_get_sector_size(0);
#endif	
    unsigned int offset;

    /* The image tag for the first image is always after the boot loader.
     * The image tag for the second image, if it exists, is at one half
     * of the flash size.
     */

#if defined(INC_NAND_FLASH_DRIVER) && (INC_NAND_FLASH_DRIVER==1) //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
	 
    if( imageNumber == 1 )
    {
        // Get the flash info and block number for parition 1 at the base of the flash
        FLASH_ADDR_INFO flash_info;

        kerSysFlashAddrInfoGet(&flash_info);
		if( flash_get_flash_type() !=  FLASH_IFC_NAND ) {
			blk = flash_get_blk((int)(pBase+flash_info.flash_rootfs_start_offset));
		}
		else {
			blk = getNandBlock((int)(pBase+flash_info.flash_rootfs_start_offset), 0);
		}		
        pSectAddr = sectAddr1;
    }
    else if( imageNumber == 2 )
    {
        // Get block number for partition2 at middle of the device (not counting space for aux
        // file system).
        offset = ((flash_get_total_size()-reserverdBytersAuxfs) / 2);

        /* make sure offset is on sector boundary, image starts on sector boundary */
		if( flash_get_flash_type() !=  FLASH_IFC_NAND ) {
			blk = flash_get_blk((int) (pBase + ((flash_get_total_size()-reserverdBytersAuxfs) / 2)+IMAGE_OFFSET));
		}
		else {
			blk = getNandBlock((int) (pBase + (flash_get_total_size() / 2)), 0);
		} 	
        pSectAddr = sectAddr2;
    }
#else     
    if( imageNumber == 1 )
    {
        // Get the flash info and block number for parition 1 at the base of the flash
        FLASH_ADDR_INFO flash_info;

        kerSysFlashAddrInfoGet(&flash_info);
        blk = flash_get_blk((int)(pBase+flash_info.flash_rootfs_start_offset));
        pSectAddr = sectAddr1;
    }
    else if( imageNumber == 2 )
    {
		/* make sure offset is on sector boundary, image starts on sector boundary */
        if( offset % sectSize )
            offset = ((offset/sectSize)+1)*sectSize;
        blk = flash_get_blk((int) (pBase + offset + IMAGE_OFFSET));
    }
#endif        
    // Now that you have a block number, use it to read in the file tag
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
    if( blk > 0 )
#else    	
    if( blk )
#endif	
    {
        int *pn;    // Use to append block number at back of file tag
        
        // Clear out space for file tag structures
        memset(pSectAddr, 0x00, sizeof(FILE_TAG));
        
        // Get file tag
        flash_read_buf((unsigned short) blk, 0, pSectAddr, sizeof(FILE_TAG));
        
        // Figure out CRC of file tag so we can check it below
        crc = CRC32_INIT_VALUE;
        crc = getCrc32(pSectAddr, (UINT32)TAG_LEN-TOKEN_LEN, crc);
        
        // Get ready to return file tag pointer
        pTag = (PFILE_TAG) pSectAddr;
        
        // Append block number after file tag
        pn = (int *) (pTag + 1);
        *pn = blk;
        
        // One final check - if the file tag CRC is not OK, return NULL instead
        if (crc != (UINT32)(*(UINT32*)(pTag->tagValidationToken)))
            pTag = NULL;

#ifdef MSTC_MODIFY /* fix bootup with wrong rootfs address,  Mitrastar Kid, 20120509. */
		if (NULL != pTag) {
			newRootfsAddr = ((blk+1)*flash_get_sector_size(0)) + (unsigned long)IMAGE_BASE;
			tagRootfsAddr = simple_strtoul(pTag->rootfsAddress, NULL, 10);
			tagKernelAddr = simple_strtoul(pTag->kernelAddress, NULL, 10);
			offset = (newRootfsAddr - tagRootfsAddr);
			if ((0 != tagRootfsAddr) && (0 < offset)) {
				tagKernelAddr += offset;

				sprintf(pTag->rootfsAddress,"%lu", (unsigned long)newRootfsAddr);
				sprintf(pTag->kernelAddress,"%lu", (unsigned long)tagKernelAddr);

				// Update the tagValidationToken value because rootfsAddress and kernelAddress changed, Mitrastar, Delon, 20121115
				crc = CRC32_INIT_VALUE;
				crc = getCrc32((unsigned char *)pTag,(UINT32)TAG_LEN-TOKEN_LEN,crc);
				*(unsigned long *) &pTag->tagValidationToken[0] = crc;
			}
		}
#endif
				
    }
    
    // All done - return file tag pointer
    return( pTag );
}

// must be called with flashImageMutex held
static int getPartitionFromTag( PFILE_TAG pTag )
{
    int ret = 0;

    if( pTag )
    {
        PFILE_TAG pTag1 = getTagFromPartition(1);
        PFILE_TAG pTag2 = getTagFromPartition(2);
        int sequence = simple_strtoul(pTag->imageSequence,  NULL, 10);
        int sequence1 = (pTag1) ? simple_strtoul(pTag1->imageSequence, NULL, 10)
            : -1;
        int sequence2 = (pTag2) ? simple_strtoul(pTag2->imageSequence, NULL, 10)
            : -1;

        if( pTag1 && sequence == sequence1 )
            ret = 1;
        else
            if( pTag2 && sequence == sequence2 )
                ret = 2;
    }

    return( ret );
}

// must be called with flashImageMutex held
static PFILE_TAG getBootImageTag(void)
{
    static int displayFsAddr = 1;
    PFILE_TAG pTag = NULL;
    PFILE_TAG pTag1 = getTagFromPartition(1);
    PFILE_TAG pTag2 = getTagFromPartition(2);
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
	NVRAM_DATA *pNvramData;
#endif
    BCM_ASSERT_HAS_MUTEX_C(&flashImageMutex);
#ifdef MSTC_MODIFY
    if( pTag1 && pTag2 )
    {
 		//__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
        /* Two images are flashed. */
        int sequence1 = simple_strtoul(pTag1->imageSequence, NULL, 10);
        int sequence2 = simple_strtoul(pTag2->imageSequence, NULL, 10);
        char *p;
        char bootPartition = BOOT_LATEST_IMAGE;

        if (NULL == (pNvramData = readNvramData()))
        {
            return pTag;
        }

        for( p = pNvramData->szBootline; p[2] != '\0'; p++ )
        {
            if( p[0] == 'p' && p[1] == '=' )
            {
                bootPartition = p[2];
                break;
            }
        }

#ifdef MTSC_DUAL_IMAGE_SELECT
        if( pNvramData->partitionNum == '0' )
		{
			pTag = pTag1;
		}
		else if( pNvramData->partitionNum == '1' )
		{
			pTag = pTag2;
		}
		else {
#endif
        if( bootPartition == BOOT_LATEST_IMAGE )
            pTag = (sequence2 > sequence1) ? pTag2 : pTag1;
        else /* Boot from the image configured. */
            pTag = (sequence2 < sequence1) ? pTag2 : pTag1;
		
#ifdef MTSC_DUAL_IMAGE_SELECT			
		}
#endif
		kfree(pNvramData);
    }
#else
    if( pTag1 && pTag2 )
    {
        /* Two images are flashed. */
        int sequence1 = simple_strtoul(pTag1->imageSequence, NULL, 10);
        int sequence2 = simple_strtoul(pTag2->imageSequence, NULL, 10);
        int imgid = 0;

        kerSysBlParmsGetInt(BOOTED_IMAGE_ID_NAME, &imgid);
        if( imgid == BOOTED_OLD_IMAGE )
            pTag = (sequence2 < sequence1) ? pTag2 : pTag1;
        else
            pTag = (sequence2 > sequence1) ? pTag2 : pTag1;
    }
#endif		
    else
        /* One image is flashed. */
        pTag = (pTag2) ? pTag2 : pTag1;

    if( pTag && displayFsAddr )
    {
        displayFsAddr = 0;
        printk("File system address: 0x%8.8lx\n",
            simple_strtoul(pTag->rootfsAddress, NULL, 10) + BOOT_OFFSET + IMAGE_OFFSET);
    }

    return( pTag );
}

#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
static PIMAGE_TAG getIMGDEFTagFromPartition(int imageNumber, unsigned int curImgSize)
{
    static unsigned char IMGDEFsectAddr1[sizeof(IMAGE_TAG)];
    static unsigned char IMGDEFsectAddr2[sizeof(IMAGE_TAG)];
    int blk = 0;
    UINT32 crc;
    PIMAGE_TAG pTag = NULL;
    unsigned char *pBase = flash_get_memptr(0);
    unsigned char *pSectAddr = NULL;

    /* The image tag for the first image is always after the boot loader.
    * The image tag for the second image, if it exists, is at one half
    * of the flash size.
    */
    if( imageNumber == 1 )
    {
        FLASH_ADDR_INFO flash_info;

        kerSysFlashAddrInfoGet(&flash_info);
        blk = flash_get_blk((int) (pBase+flash_info.flash_rootfs_start_offset));
        pSectAddr = IMGDEFsectAddr1;
    }
    else
        if( imageNumber == 2 )
        {
            blk = flash_get_blk((int) (pBase + (flash_get_total_size() / 2)));
            pSectAddr = IMGDEFsectAddr2;
        }

        if( blk )
        {
            memset(pSectAddr, 0x00, sizeof(FILE_TAG));
            flash_read_buf((unsigned short) blk, curImgSize, pSectAddr, sizeof(IMAGE_TAG));
            crc = CRC32_INIT_VALUE;
            crc = getCrc32(pSectAddr, (UINT32)TAG_LEN-TOKEN_LEN, crc);
            pTag = (PIMAGE_TAG) pSectAddr;
            if (crc != (UINT32)(*(UINT32*)(pTag->tagValidationToken)))
                pTag = NULL;
        }

        return( pTag );
}

static PIMAGE_TAG getBootImageDefTag(unsigned int curImgSize)
{
    PIMAGE_TAG pTag = NULL;
    PFILE_TAG pTag1 = getTagFromPartition(1);
    PFILE_TAG pTag2 = getTagFromPartition(2);
    NVRAM_DATA *pNvramData;

    if( pTag1 && pTag2 )
    {
        /* Two images are flashed. */
        int sequence1 = simple_strtoul(pTag1->imageSequence, NULL, 10);
        int sequence2 = simple_strtoul(pTag2->imageSequence, NULL, 10);
        char *p;
        char bootPartition = BOOT_LATEST_IMAGE;

        pNvramData = readNvramData();
        if (pNvramData == NULL)
        {
           return pTag;
        }

        for( p = pNvramData->szBootline; p[2] != '\0'; p++ )
        {
            if( p[0] == 'p' && p[1] == '=' )
            {
                bootPartition = p[2];
                break;
            }
        }

        kfree(pNvramData);

        if( bootPartition == BOOT_LATEST_IMAGE )
            pTag = (sequence2 > sequence1) ? getIMGDEFTagFromPartition(2,curImgSize) : getIMGDEFTagFromPartition(1,curImgSize);
        else /* Boot from the image configured. */
            pTag = (sequence2 < sequence1) ? getIMGDEFTagFromPartition(2,curImgSize) : getIMGDEFTagFromPartition(1,curImgSize);
    }
    else
        /* One image is flashed. */
        pTag = (pTag2) ? getIMGDEFTagFromPartition(2,curImgSize) : getIMGDEFTagFromPartition(1,curImgSize);

    return( pTag );
}
#endif
// Must be called with flashImageMutex held
static void UpdateImageSequenceNumber( unsigned char *imageSequence )
{
    int newImageSequence = 0;
    PFILE_TAG pTag = getTagFromPartition(1);

    if( pTag )
        newImageSequence = simple_strtoul(pTag->imageSequence, NULL, 10);

    pTag = getTagFromPartition(2);
    if(pTag && simple_strtoul(pTag->imageSequence, NULL, 10) > newImageSequence)
        newImageSequence = simple_strtoul(pTag->imageSequence, NULL, 10);

    newImageSequence++;
    sprintf(imageSequence, "%d", newImageSequence);
}

/* Must be called with flashImageMutex held */
static int flashFsKernelImage( unsigned char *imagePtr, int imageLen,
    int flashPartition, int *numPartitions )
{

    int status = 0;
    PFILE_TAG pTag = (PFILE_TAG) imagePtr;
    int rootfsAddr = simple_strtoul(pTag->rootfsAddress, NULL, 10);
    int kernelAddr = simple_strtoul(pTag->kernelAddress, NULL, 10);
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
    char *p;
#endif	
    char *tagFs = imagePtr;
    unsigned int baseAddr = (unsigned int) flash_get_memptr(0);
    unsigned int totalSize = (unsigned int) flash_get_total_size();
    unsigned int sectSize = (unsigned int) flash_get_sector_size(0);
    unsigned int reservedBytesAtEnd;
    unsigned int reserverdBytersAuxfs;
    unsigned int availableSizeOneImg;
    unsigned int reserveForTwoImages;
    unsigned int availableSizeTwoImgs;
    unsigned int newImgSize = simple_strtoul(pTag->rootfsLen, NULL, 10) +
#ifdef MSTC_MODIFY
	simple_strtoul(pTag->kernelLen, NULL, 10) + TAG_BLOCK_LEN;
#else
	simple_strtoul(pTag->kernelLen, NULL, 10);
#endif			
    PFILE_TAG pCurTag = getBootImageTag();
#ifndef MSTC_MODIFY	
    int nCurPartition = getPartitionFromTag( pCurTag );
    int should_yield =
        (flashPartition == 0 || flashPartition == nCurPartition) ? 0 : 1;
#endif	 
	UINT32 crc;
    unsigned int curImgSize = 0;
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
    unsigned int rootfsOffset = (unsigned int) rootfsAddr - IMAGE_BASE - TAG_BLOCK_LEN;
#else		
    unsigned int rootfsOffset = (unsigned int) rootfsAddr - IMAGE_BASE - TAG_LEN + IMAGE_OFFSET;
#endif	
    FLASH_ADDR_INFO flash_info;
    NVRAM_DATA *pNvramData;
#if defined(MSTC_OBM_IMAGE_DEFAULT) || defined(MSTC_ROM_D) //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
    PIMAGE_TAG pTag_Img = NULL;
    UINT32 image_len = 0;
    UINT32 TagCrc;
#endif
#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis merge from ZyXEL imagedefault feature, zongyue
    char *imgdefPtr = NULL;
#endif
#ifdef MSTC_ROM_D //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
    UINT32 search_tag_addr = 0;
    char *romdPtr = NULL;
    unsigned char romd_sig = 0;
#endif		
    BCM_ASSERT_HAS_MUTEX_C(&flashImageMutex);

#ifdef MSTC_ROM_D //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
	if (0x1 == pTag->imageNext[0]) {
		search_tag_addr = newImgSize;
		do {
			pTag_Img = (PIMAGE_TAG)(tagFs + search_tag_addr);
			/* 4-byte boundary protection */
			memcpy(&TagCrc, pTag_Img->tagValidationToken, CRC_LEN);
			crc = CRC32_INIT_VALUE;
			crc = getCrc32((unsigned char *)pTag_Img, (UINT32)(IMAGE_TAG_LEN-CRC_LEN), crc);
			if (crc != TagCrc) {
				printk(KERN_ERR "IMAGE_TAG CRC error. Corrupted image? %x!=%x\n",(unsigned int)crc,(unsigned int)TagCrc);
				break;
			}
			if (IMAGE_TYPE_ROMD == simple_strtoul(pTag_Img->imageType, NULL, 10)) {
				romd_sig = simple_strtoul(pTag_Img->imageSignature, NULL, 10);
				image_len = simple_strtoul(pTag_Img->imageLen, NULL, 10);
				romdPtr = tagFs + search_tag_addr + IMAGE_TAG_LEN;
				break;
			}
			else {
				search_tag_addr += (IMAGE_TAG_LEN + simple_strtoul(pTag_Img->imageLen, NULL, 10));
			}
		} while (0x1 == pTag_Img->imageNext[0]);
	}

	if( 0 != romd_sig ) {
		memcpy(&TagCrc, pTag_Img->imageValidationToken, CRC_LEN);
		crc = CRC32_INIT_VALUE;
		crc = getCrc32(romdPtr, (UINT32)image_len, crc);
		if (crc != TagCrc) {
			printk(KERN_ERR "ROM-D CRC error. Corrupted image? %x!=%x\n",(unsigned int)crc,(unsigned int)TagCrc);
		}
		else {
			if (1 == romd_sig) {
				printk(KERN_INFO "attached ROM-D file will drop and erase device ROM-D!\n");
				kerSysRomdErase();
			}
			else if (2 == romd_sig) {
				printk(KERN_INFO "attached ROM-D file will replease device ROM-D!\n");
				kerSysRomdSet(romdPtr, image_len, 0);
			}
			else {
				printk(KERN_INFO "ROM-D signature = %d, donothing!\n", romd_sig);
			}
		}
	}
#endif
#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
	if (0x1 == pTag->imageNext[0]) {
		/* first attached image is ImageDeafult */
		pTag_Img = (PIMAGE_TAG)(tagFs + newImgSize);
		/* 4-byte boundary protection */
		memcpy(&TagCrc, pTag_Img->tagValidationToken, CRC_LEN);
		crc = CRC32_INIT_VALUE;
		crc = getCrc32((unsigned char *)pTag_Img, (UINT32)(IMAGE_TAG_LEN-CRC_LEN), crc);
		if (crc != TagCrc) {
			printk(KERN_ERR "IMAGE_TAG CRC error, ignore attached ImageDefault image.\n");
			((PFILE_TAG) tagFs)->imageNext[0] = 0;
		}
		if (IMAGE_TYPE_IMGDEF == simple_strtoul(pTag_Img->imageType, NULL, 10)) {
			/* at now, only ImageDefault will attach behind fs_kernel image in flash */
			/* no other images behine ImageDefault */
			image_len = simple_strtoul(pTag_Img->imageLen, NULL, 10);
			imgdefPtr = tagFs + newImgSize + IMAGE_TAG_LEN;
			/* check ImageDefault image CRC, 4-byte boundary protection */
			memcpy(&TagCrc, pTag_Img->imageValidationToken, CRC_LEN);
			crc = CRC32_INIT_VALUE;
			crc = getCrc32((unsigned char *)(imgdefPtr), (UINT32)image_len, crc);
			if (crc != TagCrc) {
				printk(KERN_ERR "ImageDefault CRC error. Ignore\n");
				((PFILE_TAG) tagFs)->imageNext[0] = 0;
			}
			else {
				newImgSize += (IMAGE_TAG_LEN + simple_strtoul(pTag_Img->imageLen, NULL, 10));
				pTag_Img->imageNext[0] = 0;
				/* update IMAGE_TAG CRC */
				crc = CRC32_INIT_VALUE;
				crc = getCrc32((unsigned char *)pTag_Img, (UINT32)(IMAGE_TAG_LEN-CRC_LEN), crc);
				/* 4-byte boundary protection */
				memcpy(pTag_Img->tagValidationToken, &crc, CRC_LEN);
			}
		}
	}
#endif //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue

    if (NULL == (pNvramData = readNvramData()))
    {
        return -ENOMEM;
    }

    kerSysFlashAddrInfoGet(&flash_info);
    if( rootfsOffset < flash_info.flash_rootfs_start_offset )
    {
        // Increase rootfs and kernel addresses by the difference between
        // rootfs offset and what it needs to be.
        rootfsAddr += flash_info.flash_rootfs_start_offset - rootfsOffset;
        kernelAddr += flash_info.flash_rootfs_start_offset - rootfsOffset;
        sprintf(pTag->rootfsAddress,"%lu", (unsigned long) rootfsAddr);
        sprintf(pTag->kernelAddress,"%lu", (unsigned long) kernelAddr);
        crc = CRC32_INIT_VALUE;
        crc = getCrc32((unsigned char *)pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
        *(unsigned long *) &pTag->tagValidationToken[0] = crc;
    }

    rootfsAddr += BOOT_OFFSET+IMAGE_OFFSET;
    kernelAddr += BOOT_OFFSET+IMAGE_OFFSET;

    reservedBytesAtEnd = flash_get_reserved_bytes_at_end(&flash_info);
    reserverdBytersAuxfs = flash_get_reserved_bytes_auxfs();
    availableSizeOneImg = totalSize - ((unsigned int) rootfsAddr - baseAddr) -
        reservedBytesAtEnd- reserverdBytersAuxfs;  
        
    reserveForTwoImages =
        (flash_info.flash_rootfs_start_offset > reservedBytesAtEnd)
        ? flash_info.flash_rootfs_start_offset : reservedBytesAtEnd;
    availableSizeTwoImgs = ((totalSize-reserverdBytersAuxfs)/ 2) - reserveForTwoImages - sectSize;

    printk("availableSizeOneImage=%dKB availableSizeTwoImgs=%dKB reserverdBytersAuxfs=%dKB reserve=%dKB\n",
        availableSizeOneImg/1024, availableSizeTwoImgs/1024, reserverdBytersAuxfs/1024, reserveForTwoImages/1024);
	   
    if( pCurTag )
    {
#ifdef MSTC_MODIFY //__MSTC__, Dennis merge from Autumn
				curImgSize = simple_strtoul(pCurTag->rootfsLen, NULL, 10) +
						   simple_strtoul(pCurTag->kernelLen, NULL, 10) + TAG_BLOCK_LEN;
#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
			if (0x1 == pCurTag->imageNext[0]) {
				pTag_Img = getBootImageDefTag( curImgSize );
				if ( pTag_Img ) {
					/* 4-byte boundary protection */
					if (IMAGE_TYPE_IMGDEF == simple_strtoul(pTag_Img->imageType, NULL, 10)) {
						/* at now, only ImageDefault will attach behind fs_kernel image in flash */
						/* no other images behine ImageDefault */
						curImgSize += (IMAGE_TAG_LEN + simple_strtoul(pTag_Img->imageLen, NULL, 10));
					}
				}
			}
#endif //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
#else	
        curImgSize = simple_strtoul(pCurTag->rootfsLen, NULL, 10) +
            simple_strtoul(pCurTag->kernelLen, NULL, 10);
#endif			
    }

    if( newImgSize > availableSizeOneImg)
    {
        printk("Illegal image size %d.  Image size must not be greater "
            "than %d.\n", newImgSize, availableSizeOneImg);
        kfree(pNvramData);
        return -1;
    }
#ifndef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
    *numPartitions = (curImgSize <= availableSizeTwoImgs &&
         newImgSize <= availableSizeTwoImgs &&
         flashPartition != nCurPartition) ? 2 : 1;
#endif


    // If the current image fits in half the flash space and the new
    // image to flash also fits in half the flash space, then flash it
    // in the partition that is not currently being used to boot from.
	
#ifdef MSTC_MODIFY

#ifdef MTSC_DUAL_IMAGE_SELECT
	//if first partition is choose, do not change to partition two
	if( pNvramData->partitionNum != '0' ){
		if( curImgSize <= availableSizeTwoImgs &&
			newImgSize <= availableSizeTwoImgs &&
			(getPartitionFromTag( pCurTag ) == 1 || pNvramData->partitionNum == '1') )
#else
    if( curImgSize <= availableSizeTwoImgs &&
        newImgSize <= availableSizeTwoImgs &&
        getPartitionFromTag( pCurTag ) == 1 )
#endif
    {
        // Update rootfsAddr to point to the second boot partition.
		//__MSTC__, Paul Ho: Support 963268 nand flash, patch form SVN#3781 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
        int offset = 0;
        int sblk = 0; 
        int rblk = 0; //if rblk is not equal to sblk, it means block #512 is a bad block
                      //we need to shift our boot address to skip bad block
        offset = (totalSize / 2) + TAG_BLOCK_LEN;
        sblk = flash_get_blk((int) (IMAGE_BASE + (totalSize / 2) + BOOT_OFFSET));
        rblk = getNandBlock( (unsigned long) (IMAGE_BASE + (totalSize / 2) + BOOT_OFFSET), 0 );
        offset += (rblk - sblk)*128*1024;

#if defined(INC_NAND_FLASH_DRIVER) && (INC_NAND_FLASH_DRIVER==1) //__MSTC__, Dennis
        if( flash_get_flash_type() !=  FLASH_IFC_NAND )
        {
#endif
        sprintf(((PFILE_TAG) tagFs)->kernelAddress, "%lu",
            (unsigned long) IMAGE_BASE + offset + (kernelAddr - rootfsAddr));
        kernelAddr = baseAddr + offset + (kernelAddr - rootfsAddr);
#if defined(INC_NAND_FLASH_DRIVER) && (INC_NAND_FLASH_DRIVER==1) //__MSTC__, Dennis
        }
#endif
        sprintf(((PFILE_TAG) tagFs)->rootfsAddress, "%lu",
            (unsigned long) IMAGE_BASE + offset);
        rootfsAddr = baseAddr + offset;
    }
	
#ifdef MTSC_DUAL_IMAGE_SELECT
	}
#endif
#else	
    if( curImgSize <= availableSizeTwoImgs &&
        newImgSize <= availableSizeTwoImgs &&
        ((nCurPartition == 1 && flashPartition != 1) || flashPartition == 2) )
    {
        // Update rootfsAddr to point to the second boot partition.
        int offset = ((totalSize-reserverdBytersAuxfs) / 2);

        /* make sure offset is on sector boundary, image starts on sector boundary */
        if( offset % sectSize )
            offset = ((offset/sectSize)+1)*sectSize;
        offset += TAG_LEN;

        sprintf(((PFILE_TAG) tagFs)->kernelAddress, "%lu",
            (unsigned long) IMAGE_BASE + offset + (kernelAddr - rootfsAddr));
        kernelAddr = baseAddr + offset + (kernelAddr - rootfsAddr) + IMAGE_OFFSET;

        sprintf(((PFILE_TAG) tagFs)->rootfsAddress, "%lu",
            (unsigned long) IMAGE_BASE + offset);
        rootfsAddr = baseAddr + offset + IMAGE_OFFSET;
    }
#endif
#ifdef MTSC_DUAL_IMAGE_SELECT
	if( pNvramData->partitionNum != '0' && pNvramData->partitionNum != '1' )
	{
		UpdateImageSequenceNumber( ((PFILE_TAG) tagFs)->imageSequence );
	}
#else
    UpdateImageSequenceNumber( ((PFILE_TAG) tagFs)->imageSequence );
#endif	
    crc = CRC32_INIT_VALUE;
    crc = getCrc32((unsigned char *)tagFs, (UINT32)TAG_LEN-TOKEN_LEN, crc);
    *(unsigned long *) &((PFILE_TAG) tagFs)->tagValidationToken[0] = crc;
#ifdef CONFIG_MSTC_WDT
	/* Stop hardware watchdog */
	mstc_wdt_exit();
#endif
    
    // Now, perform the actual flash write
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11

#ifdef MTSC_NORWAY_CUSTOMIZATION
	if( (status = kerSysBcmImageSet((rootfsAddr-TAG_BLOCK_LEN), tagFs,
        newImgSize, 0)) != 0 )
#else
    if( (status = kerSysBcmImageSet((rootfsAddr-TAG_BLOCK_LEN), tagFs,
        newImgSize)) != 0 )
#endif
		
#else    
    if( (status = kerSysBcmImageSet((rootfsAddr-TAG_LEN), tagFs,
        TAG_LEN + newImgSize, should_yield)) != 0 )
#endif   		
    {
        printk("Failed to flash root file system. Error: %d\n", status);
        kfree(pNvramData);
        return status;
    }
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
    for( p = pNvramData->szBootline; p[2] != '\0'; p++ )
    {
        if( p[0] == 'p' && p[1] == '=' && p[2] != BOOT_LATEST_IMAGE )
        {
            // Change boot partition to boot from new image.
            p[2] = BOOT_LATEST_IMAGE;
            writeNvramDataCrcLocked(pNvramData);
            break;
        }
    }
#endif       
    // Free buffers
    kfree(pNvramData);
    return(status);
}

#ifdef MTSC_NORWAY_CUSTOMIZATION
static int flashFsKernelImageForTr69( unsigned char *imagePtr, int imageLen,
    int flashPartition, int *numPartitions )
{
    int status = 0;
    PFILE_TAG pTag = (PFILE_TAG) imagePtr;
    int rootfsAddr = simple_strtoul(pTag->rootfsAddress, NULL, 10);
    int kernelAddr = simple_strtoul(pTag->kernelAddress, NULL, 10);

    char *tagFs = imagePtr;
    unsigned int baseAddr = (unsigned int) flash_get_memptr(0);
    unsigned int totalSize = (unsigned int) flash_get_total_size();
    unsigned int reservedBytesAtEnd;
    unsigned int availableSizeOneImg;
    unsigned int reserveForTwoImages;
    unsigned int availableSizeTwoImgs;
    unsigned int newImgSize = simple_strtoul(pTag->rootfsLen, NULL, 10) +
        simple_strtoul(pTag->kernelLen, NULL, 10) + TAG_BLOCK_LEN;
    PFILE_TAG pCurTag = getBootImageTag();

	int should_yield = 1;
	
    UINT32 crc;
    unsigned int curImgSize = 0;
#if 1 //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
    unsigned int rootfsOffset = (unsigned int) rootfsAddr - IMAGE_BASE - TAG_BLOCK_LEN;
#else
    unsigned int rootfsOffset = (unsigned int) rootfsAddr - IMAGE_BASE - TAG_LEN;
#endif
    FLASH_ADDR_INFO flash_info;
    NVRAM_DATA *pNvramData;
#if defined(MSTC_OBM_IMAGE_DEFAULT) || defined(MSTC_ROM_D) //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
    PIMAGE_TAG pTag_Img = NULL;
    UINT32 image_len = 0;
    UINT32 TagCrc;
#endif
#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis merge from ZyXEL imagedefault feature, zongyue
    char *imgdefPtr = NULL;
#endif
#ifdef MSTC_ROM_D //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
    UINT32 search_tag_addr = 0;
    char *romdPtr = NULL;
    unsigned char romd_sig = 0;
#endif

    BCM_ASSERT_HAS_MUTEX_C(&flashImageMutex);
#ifdef MSTC_ROM_D //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
    if (0x1 == pTag->imageNext[0]) {
        search_tag_addr = newImgSize;
        do {
            pTag_Img = (PIMAGE_TAG)(tagFs + search_tag_addr);
            /* 4-byte boundary protection */
            memcpy(&TagCrc, pTag_Img->tagValidationToken, CRC_LEN);
            crc = CRC32_INIT_VALUE;
            crc = getCrc32((unsigned char *)pTag_Img, (UINT32)(IMAGE_TAG_LEN-CRC_LEN), crc);
            if (crc != TagCrc) {
                printk(KERN_ERR "IMAGE_TAG CRC error. Corrupted image? %x!=%x\n",(unsigned int)crc,(unsigned int)TagCrc);
                break;
            }
            if (IMAGE_TYPE_ROMD == simple_strtoul(pTag_Img->imageType, NULL, 10)) {
                romd_sig = simple_strtoul(pTag_Img->imageSignature, NULL, 10);
                image_len = simple_strtoul(pTag_Img->imageLen, NULL, 10);
                romdPtr = tagFs + search_tag_addr + IMAGE_TAG_LEN;
                break;
            }
            else {
                search_tag_addr += (IMAGE_TAG_LEN + simple_strtoul(pTag_Img->imageLen, NULL, 10));
            }
        } while (0x1 == pTag_Img->imageNext[0]);
    }
	
    if( 0 != romd_sig ) {
        memcpy(&TagCrc, pTag_Img->imageValidationToken, CRC_LEN);
        crc = CRC32_INIT_VALUE;
        crc = getCrc32(romdPtr, (UINT32)image_len, crc);
        if (crc != TagCrc) {
            printk(KERN_ERR "ROM-D CRC error. Corrupted image? %x!=%x\n",(unsigned int)crc,(unsigned int)TagCrc);
        }
        else {
            if (1 == romd_sig) {
                printk(KERN_INFO "attached ROM-D file will drop and erase device ROM-D!\n");
                kerSysRomdErase();
            }
            else if (2 == romd_sig) {
                printk(KERN_INFO "attached ROM-D file will replease device ROM-D!\n");
                kerSysRomdSet(romdPtr, image_len, 0);
            }
            else {
                printk(KERN_INFO "ROM-D signature = %d, donothing!\n", romd_sig);
            }
        }
    }
#endif
#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
    if (0x1 == pTag->imageNext[0]) {
        /* first attached image is ImageDeafult */
        pTag_Img = (PIMAGE_TAG)(tagFs + newImgSize);
        /* 4-byte boundary protection */
        memcpy(&TagCrc, pTag_Img->tagValidationToken, CRC_LEN);
        crc = CRC32_INIT_VALUE;
        crc = getCrc32((unsigned char *)pTag_Img, (UINT32)(IMAGE_TAG_LEN-CRC_LEN), crc);
        if (crc != TagCrc) {
            printk(KERN_ERR "IMAGE_TAG CRC error, ignore attached ImageDefault image.\n");
            ((PFILE_TAG) tagFs)->imageNext[0] = 0;
        }
        if (IMAGE_TYPE_IMGDEF == simple_strtoul(pTag_Img->imageType, NULL, 10)) {
            /* at now, only ImageDefault will attach behind fs_kernel image in flash */
            /* no other images behine ImageDefault */
            image_len = simple_strtoul(pTag_Img->imageLen, NULL, 10);
            imgdefPtr = tagFs + newImgSize + IMAGE_TAG_LEN;
            /* check ImageDefault image CRC, 4-byte boundary protection */
            memcpy(&TagCrc, pTag_Img->imageValidationToken, CRC_LEN);
            crc = CRC32_INIT_VALUE;
            crc = getCrc32((unsigned char *)(imgdefPtr), (UINT32)image_len, crc);
            if (crc != TagCrc) {
                printk(KERN_ERR "ImageDefault CRC error. Ignore\n");
                ((PFILE_TAG) tagFs)->imageNext[0] = 0;
            }
            else {
                newImgSize += (IMAGE_TAG_LEN + simple_strtoul(pTag_Img->imageLen, NULL, 10));
                pTag_Img->imageNext[0] = 0;
                /* update IMAGE_TAG CRC */
                crc = CRC32_INIT_VALUE;
                crc = getCrc32((unsigned char *)pTag_Img, (UINT32)(IMAGE_TAG_LEN-CRC_LEN), crc);
                /* 4-byte boundary protection */
                memcpy(pTag_Img->tagValidationToken, &crc, CRC_LEN);
            }
        }
    }
#endif //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
    if (NULL == (pNvramData = readNvramData()))
    {
        return -ENOMEM;
    }

    kerSysFlashAddrInfoGet(&flash_info);
    if( rootfsOffset < flash_info.flash_rootfs_start_offset )
    {
        // Increase rootfs and kernel addresses by the difference between
        // rootfs offset and what it needs to be.
        rootfsAddr += flash_info.flash_rootfs_start_offset - rootfsOffset;
        kernelAddr += flash_info.flash_rootfs_start_offset - rootfsOffset;
        sprintf(pTag->rootfsAddress,"%lu", (unsigned long) rootfsAddr);
        sprintf(pTag->kernelAddress,"%lu", (unsigned long) kernelAddr);
        crc = CRC32_INIT_VALUE;
        crc = getCrc32((unsigned char *)pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
        *(unsigned long *) &pTag->tagValidationToken[0] = crc;
    }

    rootfsAddr += BOOT_OFFSET;
    kernelAddr += BOOT_OFFSET;

    reservedBytesAtEnd = flash_get_reserved_bytes_at_end(&flash_info);
    availableSizeOneImg = totalSize - ((unsigned int) rootfsAddr - baseAddr) -
        reservedBytesAtEnd;
    reserveForTwoImages =
        (flash_info.flash_rootfs_start_offset > reservedBytesAtEnd)
        ? flash_info.flash_rootfs_start_offset : reservedBytesAtEnd;
    availableSizeTwoImgs = (totalSize / 2) - reserveForTwoImages;

    //    printk("availableSizeOneImage=%dKB availableSizeTwoImgs=%dKB reserve=%dKB\n",
    //            availableSizeOneImg/1024, availableSizeTwoImgs/1024, reserveForTwoImages/1024);
    if( pCurTag )
    {
#if 1 //__MSTC__, Dennis merge from Autumn
        curImgSize = simple_strtoul(pCurTag->rootfsLen, NULL, 10) +
                   simple_strtoul(pCurTag->kernelLen, NULL, 10) + TAG_BLOCK_LEN;
#else
        curImgSize = simple_strtoul(pCurTag->rootfsLen, NULL, 10) +
            simple_strtoul(pCurTag->kernelLen, NULL, 10);
#endif
    }

    if( newImgSize > availableSizeOneImg)
    {
        printk("Illegal image size %d.  Image size must not be greater "
            "than %d.\n", newImgSize, availableSizeOneImg);
        kfree(pNvramData);
        return -1;
    }

    // If the current image fits in half the flash space and the new
    // image to flash also fits in half the flash space, then flash it
    // in the partition that is not currently being used to boot from.
#if 1 //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11

#ifdef MTSC_DUAL_IMAGE_SELECT
	//if first partition is choose, do not change to partition two
	if( flashPartition == 2 )
		if( curImgSize <= availableSizeTwoImgs &&
			newImgSize <= availableSizeTwoImgs &&
			(getPartitionFromTag( pCurTag ) == 1 || flashPartition == 2) )
#else
    if( curImgSize <= availableSizeTwoImgs &&
        newImgSize <= availableSizeTwoImgs &&
        getPartitionFromTag( pCurTag ) == 1 )
#endif
		
#else
    if( curImgSize <= availableSizeTwoImgs &&
        newImgSize <= availableSizeTwoImgs &&
        ((nCurPartition == 1 && flashPartition != 1) || flashPartition == 2) )
#endif
    {
        // Update rootfsAddr to point to the second boot partition.
#if 1 //__MSTC__, Paul Ho: Support 963268 nand flash, patch form SVN#3781 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
        int offset = 0;
        int sblk = 0; 
        int rblk = 0; //if rblk is not equal to sblk, it means block #512 is a bad block
                      //we need to shift our boot address to skip bad block
        offset = (totalSize / 2) + TAG_BLOCK_LEN;
        sblk = flash_get_blk((int) (IMAGE_BASE + (totalSize / 2) + BOOT_OFFSET));
        rblk = getNandBlock( (unsigned long) (IMAGE_BASE + (totalSize / 2) + BOOT_OFFSET), 0 );
        offset += (rblk - sblk)*128*1024;
#else
        int offset = (totalSize / 2) + TAG_LEN;
#endif
#if (INC_NAND_FLASH_DRIVER==1) //__MSTC__, Dennis
        if( flash_get_flash_type() !=  FLASH_IFC_NAND )
        {
#endif
        sprintf(((PFILE_TAG) tagFs)->kernelAddress, "%lu",
            (unsigned long) IMAGE_BASE + offset + (kernelAddr - rootfsAddr));
        kernelAddr = baseAddr + offset + (kernelAddr - rootfsAddr);
#if (INC_NAND_FLASH_DRIVER==1) //__MSTC__, Dennis
        }
#endif
        sprintf(((PFILE_TAG) tagFs)->rootfsAddress, "%lu",
            (unsigned long) IMAGE_BASE + offset);
        rootfsAddr = baseAddr + offset;
    }

    crc = CRC32_INIT_VALUE;
    crc = getCrc32((unsigned char *)tagFs, (UINT32)TAG_LEN-TOKEN_LEN, crc);
    *(unsigned long *) &((PFILE_TAG) tagFs)->tagValidationToken[0] = crc;
	#ifdef CONFIG_MSTC_WDT
	/* Stop hardware watchdog */
	//mstc_wdt_exit(); //marcus
	#endif

#if 1 //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11

    if( (status = kerSysBcmImageSet((rootfsAddr-TAG_BLOCK_LEN), tagFs,
        newImgSize, should_yield)) != 0 )	
#else
    if( (status = kerSysBcmImageSet((rootfsAddr-TAG_LEN), tagFs,
        TAG_LEN + newImgSize, should_yield)) != 0 )
#endif
	
    {
        printk("Failed to flash root file system. Error: %d\n", status);
        kfree(pNvramData);
        return status;
    }

    kfree(pNvramData);
    return(status);
}
#endif

#define IMAGE_VERSION_FILE_NAME "/etc/image_version"
#define IMAGE_VERSION_MAX_SIZE  64
#ifndef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11

static int getImageVersion( int imageNumber, char *verStr, int verStrSize)
{
    static char imageVersions[2][IMAGE_VERSION_MAX_SIZE] = {{'\0'}, {'\0'}};
    int ret = 0; /* zero bytes copied so far */

    if( !((imageNumber == 1 && imageVersions[0][0] != '\0') ||
        (imageNumber == 2 && imageVersions[1][0] != '\0')) )
    {
        /* get up to IMAGE_VERSION_MAX_SIZE and save it in imageVersions[][] */
        unsigned long rootfs_ofs;

        memset(imageVersions[imageNumber - 1], 0, IMAGE_VERSION_MAX_SIZE);
        
        if( kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *) &rootfs_ofs) == -1 )
        {
            /* NOR Flash */
            PFILE_TAG pTag = NULL;

            if( imageNumber == 1 )
                pTag = getTagFromPartition(1);
            else
                if( imageNumber == 2 )
                    pTag = getTagFromPartition(2);

            if( pTag )
            {
                /* Save version string for subsequent calls to this function. 
                 MAX length  IMAGE_VER_LEN = 32 bytes */
                memcpy(imageVersions[imageNumber - 1], pTag->imageVersion, IMAGE_VER_LEN);
            }
        }
        else
        {
            /* NAND Flash */
            NVRAM_DATA *pNvramData;

            if( (imageNumber == 1 || imageNumber == 2) &&
                (pNvramData = readNvramData()) != NULL )
            {
                char *pImgVerFileName = NULL;

                mm_segment_t fs;
                struct file *fp;
                int updatePart, getFromCurPart;

                // updatePart is the partition number that is not booted
                // getFromCurPart is 1 to retrieve info from the booted partition
                updatePart =
                    (rootfs_ofs==pNvramData->ulNandPartOfsKb[NP_ROOTFS_1])
                    ? 2 : 1;
                getFromCurPart = (updatePart == imageNumber) ? 0 : 1;

                fs = get_fs();
                set_fs(get_ds());
                if( getFromCurPart == 0 )
                {
                    struct mtd_info *mtd;
                    pImgVerFileName = "/mnt/" IMAGE_VERSION_FILE_NAME;

                    mtd = get_mtd_device_nm("bootfs_update");
                    if( !IS_ERR_OR_NULL(mtd) )
                    {
                        sys_mount("mtd:bootfs_update", "/mnt","jffs2",MS_RDONLY,NULL);
                        put_mtd_device(mtd);
                    }
                    else
                        sys_mount("mtd:rootfs_update", "/mnt","jffs2",MS_RDONLY,NULL);
                }
                else
                    pImgVerFileName = IMAGE_VERSION_FILE_NAME;

                fp = filp_open(pImgVerFileName, O_RDONLY, 0);
                if( !IS_ERR(fp) )
                {
                    /* File open successful, read version string from file. */
                    if(fp->f_op && fp->f_op->read)
                    {
                        fp->f_pos = 0;
                        ret = fp->f_op->read(fp, (void *) imageVersions[imageNumber - 1], 
                            IMAGE_VERSION_MAX_SIZE,
                            &fp->f_pos);

                        if (ret > 0)
                        {
                            int i;
                            for (i = 0; i < ret; i ++)
                            {
                                if (imageVersions[imageNumber - 1][i] == 0xa)//line feed
                                {
                                    imageVersions[imageNumber - 1][i] = '\0';//end
                                    ret = i+1;
                                    break;
                                }
                            }
                        }
                    }
                    
                    filp_close(fp, NULL);
                }

                if( getFromCurPart == 0 )
                    sys_umount("/mnt", 0);

                set_fs(fs);
                kfree(pNvramData);
            }
        }
    }
    
    /* copy the first verStrSize bytes of the stored version to the caller's buffer */
    if( verStrSize > IMAGE_VERSION_MAX_SIZE )
        ret = IMAGE_VERSION_MAX_SIZE;
    else
        ret = verStrSize;
    memcpy(verStr, imageVersions[imageNumber - 1], ret);

    return( ret );
}
#endif
PFILE_TAG kerSysUpdateTagSequenceNumber(int imageNumber)
{
    PFILE_TAG pTag = NULL;
    UINT32 crc;

    switch( imageNumber )
    {
    case 0:
        pTag = getBootImageTag();
        break;

    case 1:
        pTag = getTagFromPartition(1);
        break;

    case 2:
        pTag = getTagFromPartition(2);
        break;

    default:
        break;
    }

    if( pTag )
    {
        UpdateImageSequenceNumber( pTag->imageSequence );
        crc = CRC32_INIT_VALUE;
        crc = getCrc32((unsigned char *)pTag, (UINT32)TAG_LEN-TOKEN_LEN, crc);
        *(unsigned long *) &pTag->tagValidationToken[0] = crc;
    }

    return(pTag);
}

int kerSysGetSequenceNumber(int imageNumber)
{
    int seqNumber = -1;
    unsigned long rootfs_ofs;
    if( kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *) &rootfs_ofs) == -1 )
    {
        /* NOR Flash */
        PFILE_TAG pTag = NULL;

        switch( imageNumber )
        {
        case 0:
            pTag = getBootImageTag();
            break;

        case 1:
            pTag = getTagFromPartition(1);
            break;

        case 2:
            pTag = getTagFromPartition(2);
            break;

        default:
            break;
        }

        if( pTag )
            seqNumber= simple_strtoul(pTag->imageSequence, NULL, 10);
    }
    else
    {
        /* NAND Flash */
        NVRAM_DATA *pNvramData;

        if( (pNvramData = readNvramData()) != NULL )
        {
            char fname[] = NAND_CFE_RAM_NAME;
            char cferam_buf[32], cferam_fmt[32]; 
            int i;

            mm_segment_t fs;
            struct file *fp;
            int updatePart, getFromCurPart;

#if defined(CONFIG_BCM96838)
            /* If full secure boot is in play, the CFE RAM file is the encrypted version */
            int boot_secure = otp_is_boot_secure();
            if (boot_secure)
               strcpy(fname, NAND_CFE_RAM_SECBT_NAME);
#endif

            // updatePart is the partition number that is not booted
            // getFromCurPart is 1 to retrieive info from the booted partition
            updatePart = (rootfs_ofs==pNvramData->ulNandPartOfsKb[NP_ROOTFS_1])
                ? 2 : 1;
            getFromCurPart = (updatePart == imageNumber) ? 0 : 1;

            fs = get_fs();
            set_fs(get_ds());
            if( getFromCurPart == 0 )
            {
                struct mtd_info *mtd;
                strcpy(cferam_fmt, "/mnt/");
                mtd = get_mtd_device_nm("bootfs_update");
                if( !IS_ERR_OR_NULL(mtd) )
                {
                    sys_mount("mtd:bootfs_update", "/mnt","jffs2",MS_RDONLY,NULL);
                    put_mtd_device(mtd);
                }
                else
                    sys_mount("mtd:rootfs_update", "/mnt","jffs2",MS_RDONLY,NULL);
            }
            else
            {
                struct mtd_info *mtd;
                mtd = get_mtd_device_nm("bootfs");
                if( !IS_ERR_OR_NULL(mtd) )
                {
                    strcpy(cferam_fmt, "/bootfs/");
                    put_mtd_device(mtd);
                }
                else
                    cferam_fmt[0] = '\0';
            }

            /* Find the sequence number of the specified partition. */
            fname[strlen(fname) - 3] = '\0'; /* remove last three chars */
            strcat(cferam_fmt, fname);
            strcat(cferam_fmt, "%3.3d");

            for( i = 0; i < 999; i++ )
            {
                sprintf(cferam_buf, cferam_fmt, i);
                fp = filp_open(cferam_buf, O_RDONLY, 0);
                if (!IS_ERR(fp) )
                {
                    filp_close(fp, NULL);

                    /* Seqence number found. */
                    seqNumber = i;
                    break;
                }
            }

            if( getFromCurPart == 0 )
                sys_umount("/mnt", 0);

            set_fs(fs);
            kfree(pNvramData);
        }
    }

    return(seqNumber);
}

static int getBootedValue(int getBootedPartition)
{
    static int s_bootedPartition = -1;
    int ret = -1;
    int imgId = -1;

    kerSysBlParmsGetInt(BOOTED_IMAGE_ID_NAME, &imgId);

    /* The boot loader parameter will only be "new image", "old image" or "only
     * image" in order to be compatible with non-OMCI image update. If the
     * booted partition is requested, convert this boot type to partition type.
     */
    if( imgId != -1 )
    {
        if( getBootedPartition )
        {
            if( s_bootedPartition != -1 )
                ret = s_bootedPartition;
            else
            {
                /* Get booted partition. */
                int seq1 = kerSysGetSequenceNumber(1);
                int seq2 = kerSysGetSequenceNumber(2);

                switch( imgId )
                {
                case BOOTED_NEW_IMAGE:
                    if( seq1 == -1 || seq2 > seq1 )
                        ret = BOOTED_PART2_IMAGE;
                    else
                        if( seq2 == -1 || seq1 >= seq2 )
                            ret = BOOTED_PART1_IMAGE;
                    break;

                case BOOTED_OLD_IMAGE:
                    if( seq1 == -1 || seq2 < seq1 )
                        ret = BOOTED_PART2_IMAGE;
                    else
                        if( seq2 == -1 || seq1 <= seq2 )
                            ret = BOOTED_PART1_IMAGE;
                    break;

                case BOOTED_ONLY_IMAGE:
                    ret = (seq1 == -1) ? BOOTED_PART2_IMAGE : BOOTED_PART1_IMAGE;
                    break;

                default:
                    break;
                }

                s_bootedPartition = ret;
            }
        }
        else
            ret = imgId;
    }

    return( ret );
}


#if !defined(CONFIG_BRCM_IKOS)
PFILE_TAG kerSysImageTagGet(void)
{
    PFILE_TAG tag;

    mutex_lock(&flashImageMutex);
    tag = getBootImageTag();
    mutex_unlock(&flashImageMutex);

    return tag;
}

#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
int kerSysImageTagPartitionGet(PFILE_TAG pTag)
{
    return getPartitionFromTag(pTag);
}
#endif
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
PFILE_TAG kerSysImageTagGetByPartition(int imageNumber)
{
    return( getTagFromPartition(imageNumber) );
}
#endif

#else
PFILE_TAG kerSysImageTagGet(void)
{
    return( (PFILE_TAG) (FLASH_BASE + FLASH_LENGTH_BOOT_ROM));
}
#endif

/*
 * Common function used by BCM_IMAGE_CFE and BCM_IMAGE_WHOLE ioctls.
 * This function will acquire the flashImageMutex
 *
 * @return 0 on success, -1 on failure.
 */
#ifndef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
 
static int commonImageWrite(int flash_start_addr, char *string, int size,
    int *pnoReboot, int partition)
{
    NVRAM_DATA * pNvramDataOrig;
    NVRAM_DATA * pNvramDataNew=NULL;
    int ret;


    mutex_lock(&flashImageMutex);

    // Get a copy of the nvram before we do the image write operation
    if (NULL != (pNvramDataOrig = readNvramData()))
    {
        unsigned long rootfs_ofs;

        if( kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *) &rootfs_ofs) == -1 )
        {
            /* NOR flash */
            ret = kerSysBcmImageSet(flash_start_addr, string, size, 0);
        }
        else
        {
            /* NAND flash */
            char *rootfs_part = "image_update";

            if( partition && rootfs_ofs == pNvramDataOrig->ulNandPartOfsKb[
                NP_ROOTFS_1 + partition - 1] )
            {
                /* The image to be flashed is the booted image. Force board
                 * reboot.
                 */
                rootfs_part = "image";
                if( pnoReboot )
                    *pnoReboot = 0;
            }

            ret = kerSysBcmNandImageSet(rootfs_part, string, size,
                (pnoReboot) ? *pnoReboot : 0);
        }

        /*
         * After the image is written, check the nvram.
         * If nvram is bad, write back the original nvram.
         */
        pNvramDataNew = readNvramData();
        if ((0 != ret) ||
            (NULL == pNvramDataNew) ||
            (BpSetBoardId(pNvramDataNew->szBoardId) != BP_SUCCESS)
#if defined (CONFIG_BCM_ENDPOINT_MODULE)
            || (BpSetVoiceBoardId(pNvramDataNew->szVoiceBoardId) != BP_SUCCESS)
#endif
            )
        {
            // we expect this path to be taken.  When a CFE or whole image
            // is written, it typically does not have a valid nvram block
            // in the image.  We detect that condition here and restore
            // the previous nvram settings.  Don't print out warning here.
            writeNvramDataCrcLocked(pNvramDataOrig);

            // don't modify ret, it is return value from kerSysBcmImageSet
        }
    }
    else
    {
        ret = -1;
    }

    mutex_unlock(&flashImageMutex);

    if (pNvramDataOrig)
        kfree(pNvramDataOrig);
    if (pNvramDataNew)
        kfree(pNvramDataNew);

    return ret;
}
#endif

struct file_operations monitor_fops;

#if defined(HAVE_UNLOCKED_IOCTL)
static long board_unlocked_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct inode *inode;
    long rt;
    
    inode = filep->f_dentry->d_inode;

    mutex_lock(&ioctlMutex);
    rt = board_ioctl( inode, filep, cmd, arg );
    mutex_unlock(&ioctlMutex);
    return rt;
    
}
#endif

#ifdef BUILD_TMPC2_VOICE_REINJECTION
/*
*  enable = 1, voice enable
* enable = 2, voice and reinjection enable
* enable = 3, voice, reinjection, pstn detection enabled
*/
static void setReinjectionEnable(int enable){
	if(enable){	
		printk(KERN_ERR "\r\n  VOIP enabled\n");
		kerSysSetGpioState(BP_GPIO_17_AH, 1);		
		kerSysSetGpioState(BP_GPIO_18_AH, 1);	
	}else{
		kerSysSetGpioState(BP_GPIO_18_AH, 0);   
		kerSysSetGpioState(BP_GPIO_17_AH, 0);
		printk(KERN_ERR "\r\n  PSTN enabled\n");
	}
}
#endif

//********************************************************************************************
// misc. ioctl calls come to here. (flash, led, reset, kernel memory access, etc.)
//********************************************************************************************
static int board_ioctl( struct inode *inode, struct file *flip,
                       unsigned int command, unsigned long arg )
{
    int ret = 0;
    BOARD_IOCTL_PARMS ctrlParms;
    unsigned char ucaMacAddr[NVRAM_MAC_ADDRESS_LEN];
#ifdef NORWAY_BTN_LED_OFF	
	uint32 LedAllOffMinutes_t = 0;
#endif
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
	PFILE_TAG tag = NULL;
	int i=0;
	NVRAM_DATA * pNvramData;
	NVRAM_DATA * pTmpNvramData;
#endif
    switch (command) {
#ifdef BRCM_FTTDP
    case BOARD_IOCTL_FTTDP_DSP_BOOTER:	
        download_dsp_booter();
        break;
#endif
    case BOARD_IOCTL_FLASH_WRITE:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {

            switch (ctrlParms.action) {
            case SCRATCH_PAD:
                if (ctrlParms.offset == -1)
                    ret =  kerSysScratchPadClearAll();
                else
                    ret = kerSysScratchPadSet(ctrlParms.string, ctrlParms.buf, ctrlParms.offset);
                break;

            case PERSISTENT:
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
                for ( i = 0 ; i < 10 ; i ++ ) {
                    if ( kerSysPersistentSet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset) != 0 ) {
                        udelay(100);
                    }
                    else {
                        break;
                    }
                }
#else			
                ret = kerSysPersistentSet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
#endif				
                break;

            case BACKUP_PSI:
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
                for ( i = 0 ; i < 10 ; i ++ ) {
                    if ( kerSysBackupPsiSet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset) != 0 ) {
                        udelay(100);
                    }
                    else {
                        break;
                    }
                }
#else				
                ret = kerSysBackupPsiSet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
#endif				
                break;
#ifdef MSTC_ROM_D //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
			case ROMD_PSI:
				for ( i = 0 ; i < 10 ; i ++ ) {
					if ( kerSysRomdSet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset) != 0 ) {
						udelay(100);
					}
					else {
						break;
					}
				}
				break;
#endif
            case SYSLOG:
                ret = kerSysSyslogSet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
                break;

            case NVRAM:
            {
#ifdef MSTC_MODIFY  //__MSTC__, Dennis
#else				
                NVRAM_DATA * pNvramData;
#endif				

                /*
                 * Note: even though NVRAM access is protected by
                 * flashImageMutex at the kernel level, this protection will
                 * not work if two userspaces processes use ioctls to get
                 * NVRAM data, modify it, and then use this ioctl to write
                 * NVRAM data.  This seems like an unlikely scenario.
                 */
                mutex_lock(&flashImageMutex);
                if (NULL == (pNvramData = readNvramData()))
                {
                    mutex_unlock(&flashImageMutex);
                    return -ENOMEM;
                }
                if ( !strncmp(ctrlParms.string, "WLANFEATURE", 11 ) ) { //Wlan Data data
                    pNvramData->wlanParams[NVRAM_WLAN_PARAMS_LEN-1]= *(unsigned char *)(ctrlParms.string+11);
                    writeNvramDataCrcLocked(pNvramData);
                }
                else if ( !strncmp(ctrlParms.string, "WLANDATA", 8 ) ) { //Wlan Data data
                    int t_strlen=ctrlParms.strLen-8;
                    if(t_strlen>NVRAM_WLAN_PARAMS_LEN-1)
                        t_strlen=NVRAM_WLAN_PARAMS_LEN-1;
                    memset((char *)pNvramData + ((size_t) &((NVRAM_DATA *)0)->wlanParams),
                        0, sizeof(pNvramData->wlanParams)-1 );
                    memcpy( (char *)pNvramData + ((size_t) &((NVRAM_DATA *)0)->wlanParams),
                        ctrlParms.string+8,
                        t_strlen);
                    writeNvramDataCrcLocked(pNvramData);
                }
                else {
                    // assumes the user has calculated the crc in the nvram struct
                    ret = kerSysNvRamSet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
                }
                mutex_unlock(&flashImageMutex);
                kfree(pNvramData);
                break;
            }

            case BCM_IMAGE_CFE:
                {
                unsigned long not_used;

                if(kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *)&not_used)==0)
                {
                    printk("\nERROR: Image does not support a NAND flash device.\n\n");
                    ret = -1;
                    break;
                }
                if( ctrlParms.strLen <= 0 || ctrlParms.strLen > FLASH_LENGTH_BOOT_ROM )
                {
                    printk("Illegal CFE size [%d]. Size allowed: [%d]\n",
                        ctrlParms.strLen, FLASH_LENGTH_BOOT_ROM);
                    ret = -1;
                    break;
                }
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
				mutex_lock(&flashImageMutex);
				if (NULL == (pNvramData = readNvramData()))
				{
				   mutex_unlock(&flashImageMutex);
				   return -ENOMEM;
				}
			
#ifdef CONFIG_MSTC_WDT
				/* Stop hardware watchdog */
				mstc_wdt_exit();
#endif
							
#ifdef MTSC_NORWAY_CUSTOMIZATION
				ret = kerSysBcmImageSet(ctrlParms.offset + BOOT_OFFSET, ctrlParms.string, ctrlParms.strLen, 0);
#else
				ret = kerSysBcmImageSet(ctrlParms.offset + BOOT_OFFSET, ctrlParms.string, ctrlParms.strLen);
#endif

				// Check if the new image has valid NVRAM
				pTmpNvramData = kmalloc(sizeof(NVRAM_DATA), GFP_KERNEL);
				if (pTmpNvramData == NULL)
				{
				   mutex_unlock(&flashImageMutex);
				   kfree(pNvramData);
				   return -ENOMEM;
				}
				memset((char *)pTmpNvramData, 0, sizeof(NVRAM_DATA));
				memcpy ( (char *)pTmpNvramData, ctrlParms.string + NVRAM_DATA_OFFSET, sizeof(NVRAM_DATA) );
				memcpy( pNvramData->BuildInfo, pTmpNvramData->BuildInfo, NVRAM_BUILDINFO_LEN );
#if 1 //__MSTC__, Dennis
				// We need restore nvram data.
				// 1. debug flag is zero
				// 2. nvram data is invalid.
				/* __MSTC__, zongyue: fix logic issue */
				if (0 == pNvramData->EngDebugFlag)
				{
					if (NULL != (pTmpNvramData = readNvramData()))
						mustUpdateNvramfield(pTmpNvramData, pNvramData);
					writeNvramDataCrcLocked(pNvramData); 
				}
				else
				{
					if(
#if defined (CONFIG_BCM_ENDPOINT_MODULE)
				   (NULL == (pTmpNvramData = readNvramData()) )|| 
				   (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS) || 
				   (BpSetVoiceBoardId(pTmpNvramData->szVoiceBoardId) != BP_SUCCESS)
#else
				   (NULL == (pTmpNvramData = readNvramData()) ) || 
				   (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS)
#endif
				)
				   writeNvramDataCrcLocked(pNvramData); 
				}
#else
#if defined (CONFIG_BCM_ENDPOINT_MODULE)
				if ((NULL != (pTmpNvramData = readNvramData()) ) || (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS) || (BpSetVoiceBoardId(pTmpNvramData->szVoiceBoardId) != BP_SUCCESS))
#else
				if ((NULL != (pTmpNvramData = readNvramData()) ) || (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS))
#endif
					writeNvramDataCrcLocked(pNvramData);
#endif //__MSTC__, Dennis
				mutex_unlock(&flashImageMutex);
				kfree(pTmpNvramData);
				kfree(pNvramData);
#else
                ret = commonImageWrite(ctrlParms.offset + BOOT_OFFSET + IMAGE_OFFSET,
                    ctrlParms.string, ctrlParms.strLen, NULL, 0);
#endif
                }
                break;

            case BCM_IMAGE_FS:
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11

#ifdef MTSC_NORWAY_CUSTOMIZATION
				{
					int noReboot = FLASH_IS_NO_REBOOT(ctrlParms.offset);
					int partition = FLASH_GET_PARTITION(ctrlParms.offset);
					
					if( noReboot == 1 )
					{
						ret = flashFsKernelImageForTr69(ctrlParms.string, ctrlParms.strLen, partition, 0);
					}else{
						ret = flashFsKernelImage(ctrlParms.string, ctrlParms.strLen, 0, 0);
					}
					
					if (ret == 0 && noReboot == 0)
					{
						resetPwrmgmtDdrMips();
					}
				}
#else
                ret = flashFsKernelImage(ctrlParms.string, ctrlParms.strLen, 0, 0);
#if 1 //MitraStar, Elina
                if (ret == 0)
                {
                    resetPwrmgmtDdrMips();
                }
#endif
#endif

#else				
                {
                int numPartitions = 1;
                int noReboot = FLASH_IS_NO_REBOOT(ctrlParms.offset);
                int partition = FLASH_GET_PARTITION(ctrlParms.offset);
                unsigned long not_used;
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
                int resumeWD;
#endif
              
                if(kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *)&not_used)==0)
                {
                    printk("\nERROR: Image does not support a NAND flash device.\n\n");
                    ret = -1;
                    break;
                }

#if defined(CONFIG_BCM_WATCHDOG_TIMER)
                resumeWD = bcm_suspend_watchdog();
#endif

                mutex_lock(&flashImageMutex);

                ret = flashFsKernelImage(ctrlParms.string, ctrlParms.strLen,
                    partition, &numPartitions);

                mutex_unlock(&flashImageMutex);

#if defined(CONFIG_BCM_WATCHDOG_TIMER)
                if (resumeWD)
                    bcm_resume_watchdog();
#endif
                if(ret == 0 && (numPartitions == 1 || noReboot == 0))
                    resetPwrmgmtDdrMips();
                }
#endif				
                break;

            case BCM_IMAGE_KERNEL:  // not used for now.
                break;

            case BCM_IMAGE_WHOLE:
                {
#ifndef MSTC_MODIFY				
                int noReboot = FLASH_IS_NO_REBOOT(ctrlParms.offset);
                int partition = FLASH_GET_PARTITION(ctrlParms.offset);
#endif					
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
                int resumeWD;
#endif

                if(ctrlParms.strLen <= 0)
                {
                    printk("Illegal flash image size [%d].\n", ctrlParms.strLen);
                    ret = -1;
                    break;
                }
#ifdef MSTC_MODIFY
				if (ctrlParms.offset == 0)
                {
                    ctrlParms.offset = FLASH_BASE;
                }
#if 1 //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
                mutex_lock(&flashImageMutex);
                if (NULL == (pNvramData = readNvramData()))
                {
                    mutex_unlock(&flashImageMutex);
                    return -ENOMEM;
                }
		#ifdef CONFIG_MSTC_WDT
                /* Stop hardware watchdog */
                mstc_wdt_exit();
                #endif
#ifdef MTSC_NORWAY_CUSTOMIZATION
				ret = kerSysBcmImageSet(ctrlParms.offset, ctrlParms.string, ctrlParms.strLen, 0);
#else
               ret = kerSysBcmImageSet(ctrlParms.offset, ctrlParms.string, ctrlParms.strLen);
#endif

               pTmpNvramData = kmalloc(sizeof(NVRAM_DATA), GFP_KERNEL);                
               if ( pTmpNvramData == NULL)
               {
                  mutex_unlock(&flashImageMutex);
                  kfree(pNvramData);
                  return -ENOMEM;
               }

                // Check if the new image has valid NVRAM
#if 1 //__MSTC__, Dennis
               // We need restore nvram data.
               // 1. debug flag is zero
               // 2. nvram data is invalid.
				/* __MSTC__, zongyue: fix logic issue */
                if (0 == pNvramData->EngDebugFlag)
				{
					if (NULL != (pTmpNvramData = readNvramData()))
						mustUpdateNvramfield(pTmpNvramData, pNvramData);
					writeNvramDataCrcLocked(pNvramData); 
				}
				else
				{
					if(
#if defined (CONFIG_BCM_ENDPOINT_MODULE)
                  (NULL == (pTmpNvramData = readNvramData()) )|| 
                  (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS) || 
                  (BpSetVoiceBoardId(pTmpNvramData->szVoiceBoardId) != BP_SUCCESS)
#else
                  (NULL == (pTmpNvramData = readNvramData()) ) || 
                  (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS)
#endif
               )
                  writeNvramDataCrcLocked(pNvramData); 
               }
#else
#if defined (CONFIG_BCM_ENDPOINT_MODULE)
                if ( (NULL != (pTmpNvramData = readNvramData()) )|| (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS) || (BpSetVoiceBoardId(pTmpNvramData->szVoiceBoardId) != BP_SUCCESS))
#else
                if ((NULL != (pTmpNvramData = readNvramData()) ) || (BpSetBoardId(pTmpNvramData->szBoardId) != BP_SUCCESS))
#endif
                    writeNvramDataCrcLocked(pNvramData);
#endif //__MSTC__, Dennis
                mutex_unlock(&flashImageMutex);
                kfree(pTmpNvramData);
                kfree(pNvramData);
#else
                ret = commonImageWrite(ctrlParms.offset, ctrlParms.string, ctrlParms.strLen);
#endif
                if (ret == 0)
                {
                    resetPwrmgmtDdrMips();
                }
                else
                {
                    printk("flash of whole image failed, ret=%d\n", ret);
                }

#else
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
                resumeWD = bcm_suspend_watchdog();
#endif
                ret = commonImageWrite(FLASH_BASE, ctrlParms.string,
                    ctrlParms.strLen, &noReboot, partition );

#if defined(CONFIG_BCM_WATCHDOG_TIMER)
                if (resumeWD)
                    bcm_resume_watchdog();
#endif
                if(ret == 0 && noReboot == 0)
                {
                    resetPwrmgmtDdrMips();
                }
                else
                {
                    if (ret != 0)
                        printk("flash of whole image failed, ret=%d\n", ret);
                }
#endif				
                }
                break;

            default:
                ret = -EINVAL;
                printk("flash_ioctl_command: invalid command %d\n", ctrlParms.action);
                break;
            }
            ctrlParms.result = ret;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_FLASH_READ:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            switch (ctrlParms.action) {
#ifdef MSTC_MODIFY //__MSTC__, Dennis merge from Elina
            case UPDATE_CONFIG:
                ret = kerSysUpdateConfigGet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
                break;
#endif				
            case SCRATCH_PAD:
                ret = kerSysScratchPadGet(ctrlParms.string, ctrlParms.buf, ctrlParms.offset);
                break;

            case PERSISTENT:
                ret = kerSysPersistentGet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
                break;

            case BACKUP_PSI:
                ret = kerSysBackupPsiGet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
                break;

            case SYSLOG:
                ret = kerSysSyslogGet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
                break;

            case NVRAM:
                kerSysNvRamGet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset);
                ret = 0;
                break;
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
			case FILETAG:
			   if ( *((int*)ctrlParms.buf) == 0 ) { //get boot
				  if ( (tag = kerSysImageTagGet()) != NULL ) {
					 memcpy(ctrlParms.string, tag, sizeof(FILE_TAG));
					 ret = 0;
				  }
					 else {
					 ret = -1;
				  }
			   }
			   else {
				  if ( (tag = kerSysImageTagGetByPartition(*((int*)ctrlParms.buf))) != NULL ) {
					 memcpy(ctrlParms.string, tag, sizeof(FILE_TAG));
					 ret = 0;
				  }
				  else {
					 ret = -1;
				  }
			   }
			   break;
#endif
            case FLASH_SIZE:
                ret = kerSysFlashSizeGet();
                break;
#ifdef MSTC_ROM_D //__MSTC__, Dennis merge from ZyXEL ROM-D feature, zongyue
			case ROMD_PSI:
				ret = kerSysRomdGet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset); 
				break;
#endif
#ifdef MSTC_OBM_IMAGE_DEFAULT //__MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
			case IMGDEF_PSI:
				ret = kerSysImgDefGet(ctrlParms.string, ctrlParms.strLen, ctrlParms.offset); 
				break;

			case IMAGETAG:
				ret = kerSysImgDefTagGet(ctrlParms.string);
				break;
#endif
            default:
                ret = -EINVAL;
                printk("Not supported.  invalid command %d\n", ctrlParms.action);
                break;
            }
            ctrlParms.result = ret;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_FLASH_LIST:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            switch (ctrlParms.action) {
            case SCRATCH_PAD:
                ret = kerSysScratchPadList(ctrlParms.buf, ctrlParms.offset);
                break;

            default:
                ret = -EINVAL;
                printk("Not supported.  invalid command %d\n", ctrlParms.action);
                break;
            }
            ctrlParms.result = ret;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_DUMP_ADDR:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            dumpaddr( (unsigned char *) ctrlParms.string, ctrlParms.strLen );
            ctrlParms.result = 0;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_SET_MEMORY:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            unsigned long  *pul = (unsigned long *)  ctrlParms.string;
            unsigned short *pus = (unsigned short *) ctrlParms.string;
            unsigned char  *puc = (unsigned char *)  ctrlParms.string;
            switch( ctrlParms.strLen ) {
            case 4:
                *pul = (unsigned long) ctrlParms.offset;
                break;
            case 2:
                *pus = (unsigned short) ctrlParms.offset;
                break;
            case 1:
                *puc = (unsigned char) ctrlParms.offset;
                break;
            }
            /* This is placed as MoCA blocks are 32-bit only
            * accessible and following call makes access in terms
            * of bytes. Probably MoCA address range can be checked
            * here.
            */
            dumpaddr( (unsigned char *) ctrlParms.string, sizeof(long) );
            ctrlParms.result = 0;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_MIPS_SOFT_RESET:
        kerSysMipsSoftReset();
        break;

    case BOARD_IOCTL_LED_CTRL:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            kerSysLedCtrl((BOARD_LED_NAME)ctrlParms.strLen, (BOARD_LED_STATE)ctrlParms.offset);
            ret = 0;
        }
        break;
#ifdef NORWAY_BTN_LED_OFF	
	case BOARD_IOCTL_LED_MNGT:

		if (ledBootFlag == 1){
			LedAllOffMinutes_t = (LedAllOffMinutes*60*10 + 1250);
		}else{
			LedAllOffMinutes_t = (LedAllOffMinutes*60*10);
		}
		if (LedAllOffMinutes == 0){
			LedAllOffMinutes_t = LedAllOffMinutes_t + 20;
		}

		
		if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
		{

			if ( (LedAllOff == 1) || ((LedAllOffMinutes == 0) && (ctrlParms.offset != 0)) || (chkLedAllOffCounter >= LedAllOffMinutes_t) ){
				LED->ledLinkActSelLow |= 0x210000;
				GPIO->LEDCtrl |= 0x200000;
				if(chkintenLed ==1 ){
					boardLedCtrl(kLedInternetData, kLedStateOn);
				}else if (chkintenLed ==0 ) {
					boardLedCtrl(kLedInternetData, kLedStateOff);
				}

				LedAllOff = 0;
				kerSysSendtoMonitorTask(MSG_EXTSW_LED_OFF, "0", sizeof(char));	
			}

			LedAllOffMode = ctrlParms.strLen;
			LedAllOffMinutes = ctrlParms.offset;
			chkLedAllOffCounter = 0; 

			if ( LedAllOffMinutes == 0 && LedAllOffMode != 0 ) {
				LED->ledLinkActSelLow &= 0xFF00FFFF;							 // disable wan
				GPIO->LEDCtrl &= 0xFFDFFFFF;									 //disable intel	
				chkintenLed = 2;
				LedAllOff = 1;
				kerSysSendtoMonitorTask(MSG_EXTSW_LED_OFF, "1", sizeof(char));	
			}
								
			ret = 0;
		}
		break;
#endif
    case BOARD_IOCTL_GET_ID:
        if (copy_from_user((void*)&ctrlParms, (void*)arg,
            sizeof(ctrlParms)) == 0)
        {
            if( ctrlParms.string )
            {
                char p[NVRAM_BOARD_ID_STRING_LEN];
                kerSysNvRamGetBoardId(p);
                if( strlen(p) + 1 < ctrlParms.strLen )
                    ctrlParms.strLen = strlen(p) + 1;
                __copy_to_user(ctrlParms.string, p, ctrlParms.strLen);
            }

            ctrlParms.result = 0;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,
                sizeof(BOARD_IOCTL_PARMS));
        }
        break;
    
    case BOARD_IOCTL_GET_MAC_ADDRESS:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            ctrlParms.result = kerSysGetMacAddress( ucaMacAddr,
                ctrlParms.offset );

            if( ctrlParms.result == 0 )
            {
                __copy_to_user(ctrlParms.string, ucaMacAddr,
                    sizeof(ucaMacAddr));
            }

            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,
                sizeof(BOARD_IOCTL_PARMS));
            ret = ctrlParms.result;
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_ALLOC_MAC_ADDRESSES:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            ctrlParms.result = kerSysGetMacAddresses( ucaMacAddr,
                *((UINT32 *)ctrlParms.buf), ctrlParms.offset );

            if( ctrlParms.result == 0 )
            {
                __copy_to_user(ctrlParms.string, ucaMacAddr,
                    sizeof(ucaMacAddr));
            }

            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,
                sizeof(BOARD_IOCTL_PARMS));
            ret = ctrlParms.result;
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_RELEASE_MAC_ADDRESSES:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            if (copy_from_user((void*)ucaMacAddr, (void*)ctrlParms.string, \
                NVRAM_MAC_ADDRESS_LEN) == 0)
            {
                ctrlParms.result = kerSysReleaseMacAddresses( ucaMacAddr, *((UINT32 *)ctrlParms.buf) );
            }
            else
            {
                ctrlParms.result = -EACCES;
            }

            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,
                sizeof(BOARD_IOCTL_PARMS));
            ret = ctrlParms.result;
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_RELEASE_MAC_ADDRESS:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            if (copy_from_user((void*)ucaMacAddr, (void*)ctrlParms.string, \
                NVRAM_MAC_ADDRESS_LEN) == 0)
            {
                ctrlParms.result = kerSysReleaseMacAddress( ucaMacAddr );
            }
            else
            {
                ctrlParms.result = -EACCES;
            }

            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,
                sizeof(BOARD_IOCTL_PARMS));
            ret = ctrlParms.result;
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_GET_PSI_SIZE:
        {
            FLASH_ADDR_INFO fInfo;
            kerSysFlashAddrInfoGet(&fInfo);
            ctrlParms.result = fInfo.flash_persistent_length;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        break;

    case BOARD_IOCTL_GET_BACKUP_PSI_SIZE:
        {
            FLASH_ADDR_INFO fInfo;
            kerSysFlashAddrInfoGet(&fInfo);
            // if number_blks > 0, that means there is a backup psi, but length is the same
            // as the primary psi (persistent).

            ctrlParms.result = (fInfo.flash_backup_psi_number_blk > 0) ?
                fInfo.flash_persistent_length : 0;
#ifndef MSTC_MODIFY							
            printk("backup_psi_number_blk=%d result=%d\n", fInfo.flash_backup_psi_number_blk, fInfo.flash_persistent_length);
#endif			
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        break;

    case BOARD_IOCTL_GET_SYSLOG_SIZE:
        {
            FLASH_ADDR_INFO fInfo;
            kerSysFlashAddrInfoGet(&fInfo);
            ctrlParms.result = fInfo.flash_syslog_length;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        break;

    case BOARD_IOCTL_GET_SDRAM_SIZE:
        ctrlParms.result = (int) g_ulSdramSize;
        __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        ret = 0;
        break;

    case BOARD_IOCTL_GET_BASE_MAC_ADDRESS:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            __copy_to_user(ctrlParms.string, g_pMacInfo->ucaBaseMacAddr, NVRAM_MAC_ADDRESS_LEN);
            ctrlParms.result = 0;

            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,
                sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        else
            ret = -EFAULT;
        break;

    case BOARD_IOCTL_GET_CHIP_ID:
        ctrlParms.result = kerSysGetChipId();


        __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        ret = 0;
        break;

    case BOARD_IOCTL_GET_CHIP_REV:
        ctrlParms.result = UtilGetChipRev();
        __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        ret = 0;
        break;

    case BOARD_IOCTL_GET_NUM_ENET_MACS:
    case BOARD_IOCTL_GET_NUM_ENET_PORTS:
        {
            const ETHERNET_MAC_INFO *EnetInfos;
            int i, cnt, numEthPorts = 0;
            if ( ( EnetInfos = BpGetEthernetMacInfoArrayPtr() ) != NULL ) {
                for( i = 0; i < BP_MAX_ENET_MACS; i++) {
                    if (EnetInfos[i].ucPhyType != BP_ENET_NO_PHY) {
                        bitcount(cnt, EnetInfos[i].sw.port_map);
                        numEthPorts += cnt;
                    }
                }
                ctrlParms.result = numEthPorts;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,  sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else {
                ret = -EFAULT;
            }
            break;
        }

    case BOARD_IOCTL_GET_CFE_VER:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            unsigned char vertag[CFE_VERSION_MARK_SIZE+CFE_VERSION_SIZE];
            kerSysCfeVersionGet(vertag, sizeof(vertag));
            if (ctrlParms.strLen < CFE_VERSION_SIZE) {
                ctrlParms.result = 0;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = -EFAULT;
            }
            else if (strncmp(vertag, "cfe-v", 5)) { // no tag info in flash
                ctrlParms.result = 0;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else {
                ctrlParms.result = 1;
                __copy_to_user(ctrlParms.string, vertag+CFE_VERSION_MARK_SIZE, CFE_VERSION_SIZE);
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
        }
        else {
            ret = -EFAULT;
        }
        break;

#if defined (WIRELESS)
    case BOARD_IOCTL_GET_WLAN_ANT_INUSE:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            unsigned short antInUse = 0;
            if (BpGetWirelessAntInUse(&antInUse) == BP_SUCCESS) {
                if (ctrlParms.strLen == sizeof(antInUse)) {
                    __copy_to_user(ctrlParms.string, &antInUse, sizeof(antInUse));
                    ctrlParms.result = 0;
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                    ret = 0;
                } else
                    ret = -EFAULT;
            }
            else {
                ret = -EFAULT;
            }
            break;
        }
        else {
            ret = -EFAULT;
        }
        break;
#endif
    case BOARD_IOCTL_SET_TRIGGER_EVENT:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            BOARD_IOC *board_ioc = (BOARD_IOC *)flip->private_data;
            ctrlParms.result = -EFAULT;
            ret = -EFAULT;
            if (ctrlParms.strLen == sizeof(unsigned long)) {
                board_ioc->eventmask |= *((int*)ctrlParms.string);
#if defined (WIRELESS)
                if((board_ioc->eventmask & SES_EVENTS)) {
                    ctrlParms.result = 0;
                    ret = 0;
                }
				#ifdef MSTC_WLAN_WPS_BTN_SEPARATE //__MSTC__, WLAN_WPS_BTN_SEPARATE
				if(wlanBtn_irq != BP_NOT_DEFINED) {
                    BcmHalInterruptEnable(wlanBtn_irq);
                    ctrlParms.result = 0;
                    ret = 0;
                }
				#endif					
#endif
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            }
            break;
        }
        else {
            ret = -EFAULT;
        }
        break;

    case BOARD_IOCTL_GET_TRIGGER_EVENT:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            BOARD_IOC *board_ioc = (BOARD_IOC *)flip->private_data;
            if (ctrlParms.strLen == sizeof(unsigned long)) {
                __copy_to_user(ctrlParms.string, &board_ioc->eventmask, sizeof(unsigned long));
                ctrlParms.result = 0;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            } else
                ret = -EFAULT;

            break;
        }
        else {
            ret = -EFAULT;
        }
        break;

    case BOARD_IOCTL_UNSET_TRIGGER_EVENT:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            if (ctrlParms.strLen == sizeof(unsigned long)) {
                BOARD_IOC *board_ioc = (BOARD_IOC *)flip->private_data;
                board_ioc->eventmask &= (~(*((int*)ctrlParms.string)));
                ctrlParms.result = 0;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            } else
                ret = -EFAULT;

            break;
        }
        else {
            ret = -EFAULT;
        }
        break;
#if defined (WIRELESS)
    case BOARD_IOCTL_SET_SES_LED:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
#ifdef MSTC_MODIFY
            sesLed_ctrl(*(int*)ctrlParms.string, (BOARD_LED_NAME)ctrlParms.strLen);
#ifdef CONFIG_WPS5GLED
			if(ctrlParms.offset){
				// wl0 wl1 wps work at the same time.
				sesLed_ctrl(*(int*)ctrlParms.string, kLed5GWps);
			}
#endif
            ctrlParms.result = 0;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;

#else		
            if (ctrlParms.strLen == sizeof(int)) {
                sesLed_ctrl(*(int*)ctrlParms.string);
                ctrlParms.result = 0;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            } else
                ret = -EFAULT;
#endif
            break;
        }
        else {
            ret = -EFAULT;
        }
        break;
#endif

    case BOARD_IOCTL_GET_GPIOVERLAYS:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
	    unsigned long GPIOOverlays = 0;
	    ret = 0;
            if (BP_SUCCESS == (ctrlParms.result = BpGetGPIOverlays(&GPIOOverlays) )) {
	      __copy_to_user(ctrlParms.string, &GPIOOverlays, sizeof(unsigned long));

	        if(__copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS))!=0)
		    ret = -EFAULT;	    		  
	    }
	}else
            ret = -EFAULT;

        break;

    case BOARD_IOCTL_SET_MONITOR_FD:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {

           g_monitor_nl_pid =  ctrlParms.offset;
           printk(KERN_INFO "monitor task is initialized pid= %d \n",g_monitor_nl_pid);
        }
        break;

    case BOARD_IOCTL_WAKEUP_MONITOR_TASK:
        kerSysSendtoMonitorTask(MSG_NETLINK_BRCM_WAKEUP_MONITOR_TASK, NULL, 0);
        break;
    case BOARD_IOCTL_SET_CS_PAR:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            ret = ConfigCs(&ctrlParms);
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        }
        else {
            ret = -EFAULT;
        }
        break;
#ifdef MSTC_MODIFY //__MSTC__
	case BOARD_IOCTL_GET_GPIO:
				if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
#if defined(CONFIG_BCM96328)
					printk(KERN_ERR "GPIO 0x%lx\n", GPIO->GPIOio);
#else
					printk(KERN_ERR "GPIO 0x%llu\n", GPIO->GPIOio);
#endif
					__copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
					ret = 0;
				}
				else {
					ret = -EFAULT;
				}
				break;
#endif
#ifdef BUILD_TMPC2_VOICE_REINJECTION
	case BOARD_IOCTL_REINJECTION:
				if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {	
					if(ctrlParms.strLen == -1){
						if(GPIO->GPIOio & 1<<17){ //check reinjection enabled or disabled
							ctrlParms.result = 0;
						}
						else{
							ctrlParms.result = 1;
						}
					}
					else{
						setReinjectionEnable(ctrlParms.offset);					
					}
					__copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
					ret = 0;
				}
				else {
					ret = -EFAULT;
				}
				break;
#endif
    case BOARD_IOCTL_SET_GPIO:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            kerSysSetGpioState(ctrlParms.strLen, ctrlParms.offset);
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        else {
            ret = -EFAULT;
        }
        break;
#ifdef MSTC_MODIFY //__MSTC__, Dennis
		   case BOARD_IOCTL_SET_BUTTONTEST:
			  if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
				 if ( ctrlParms.strLen == 1 ) {
					buttonTest = 1;
					stopblink = 0;
					stopblinkwl = 0;
#if defined (WIRELESS) || defined(CONFIG_WIRELESS) // __P870HN51D_STD__, Richard, add CONFIG_WIRELESS
					BcmHalInterruptEnable(sesBtn_irq);
#ifdef MSTC_WLAN_WPS_BTN_SEPARATE //__MSTC__, WLAN_WPS_BTN_SEPARATE
					BcmHalInterruptEnable(wlanBtn_irq);
#endif
#endif
					ctrlParms.result = buttonTest;
				 }else if ( ctrlParms.strLen == 0 ) {
					buttonTest = 0;
					stopblink = 0;
					stopblinkwl = 0;
					ctrlParms.result = buttonTest;
				 }else if ( ctrlParms.strLen == 2 ) {
					ctrlParms.result = buttonTest;
				 }else if ( ctrlParms.strLen == 3 ) {
					ctrlParms.result = stopblink;
				 }else if ( ctrlParms.strLen == 4 ) {
					ctrlParms.result = stopblinkwl;
				 }
				 __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
				 ret = 0;
			  } 
			  else {
				 ret = -EFAULT;  
			  }
			  break;
#endif //__MSTC__, Dennis
#ifdef MSTC_MODIFY//__MSTC__, RaynorChung: Support USB test CMD for 963268 platform
			case BOARD_IOCTL_SET_USBTEST:
				if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
					if ( ctrlParms.offset ) {
            #if defined(CONFIG_BCM963268)
						volatile int *USB20H_EHCI_PORTSC_0 = (int*)(USB_EHCI_BASE+0xa0000054);
						volatile int *USB20H_EHCI_PORTSC_1 = (int*)(USB_EHCI_BASE+0xa0000058);
						*USB20H_EHCI_PORTSC_0 &= ~(0xf<<16);
						*USB20H_EHCI_PORTSC_0 |= (0x4<<16);
						printk("USB20H_EHCI_PORTSC_0(0x%x) = 0x%x\n", (int)USB20H_EHCI_PORTSC_0, *USB20H_EHCI_PORTSC_0);
		
						
						*USB20H_EHCI_PORTSC_1 &= ~(0xf<<16);
						*USB20H_EHCI_PORTSC_1 |= (0x4<<16);
						printk("USB20H_EHCI_PORTSC_0(0x%x) = 0x%x\n", (int)USB20H_EHCI_PORTSC_1, *USB20H_EHCI_PORTSC_1);
            #endif
					}
					else {	
            #if defined(CONFIG_BCM963268)
						volatile int *USB20H_EHCI_PORTSC_0 = (int*)(USB_EHCI_BASE+0xa0000054);
						volatile int *USB20H_EHCI_PORTSC_1 = (int*)(USB_EHCI_BASE+0xa0000058);
					
						*USB20H_EHCI_PORTSC_0 &= ~(0xf<<16);
						printk("USB20H_EHCI_PORTSC_0(0x%x) = 0x%x\n", (int)USB20H_EHCI_PORTSC_0, *USB20H_EHCI_PORTSC_0);
		
						
						*USB20H_EHCI_PORTSC_1 &= ~(0xf<<16);
						printk("USB20H_EHCI_PORTSC_0(0x%x) = 0x%x\n", (int)USB20H_EHCI_PORTSC_1, *USB20H_EHCI_PORTSC_1);
            #endif
					}
					ctrlParms.result = 1;
					__copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
					ret = 0;
				} 
				else {
					ret = -EFAULT;	
				}
				break;
#endif
#if defined(CONFIG_BCM_CPLD1)
    case BOARD_IOCTL_SET_SHUTDOWN_MODE:
        BcmCpld1SetShutdownMode();
        ret = 0;
        break;

    case BOARD_IOCTL_SET_STANDBY_TIMER:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            BcmCpld1SetStandbyTimer(ctrlParms.offset);
            ret = 0;
        }
        else {
            ret = -EFAULT;
        }
        break;
#endif

    case BOARD_IOCTL_BOOT_IMAGE_OPERATION:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
            switch(ctrlParms.offset)
            {
            case BOOT_SET_PART1_IMAGE:
            case BOOT_SET_PART2_IMAGE:
            case BOOT_SET_PART1_IMAGE_ONCE:
            case BOOT_SET_PART2_IMAGE_ONCE:
            case BOOT_SET_OLD_IMAGE:
            case BOOT_SET_NEW_IMAGE:
#ifndef MSTC_MODIFY //__MSTC__, RaynorChung: Support 963268 nand flash, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11			
            case BOOT_SET_NEW_IMAGE_ONCE:
                ctrlParms.result = kerSysSetBootImageState(ctrlParms.offset);
                break;

            case BOOT_GET_BOOT_IMAGE_STATE:
                ctrlParms.result = kerSysGetBootImageState();
                break;

            case BOOT_GET_IMAGE_VERSION:
                /* ctrlParms.action is parition number */
                ctrlParms.result = getImageVersion((int) ctrlParms.action,
                    ctrlParms.string, ctrlParms.strLen);
                break;
#endif

            case BOOT_GET_BOOTED_IMAGE_ID:
                /* ctrlParm.strLen == 1: partition or == 0: id (new or old) */
                ctrlParms.result = getBootedValue(ctrlParms.strLen);
                break;

            default:
                ctrlParms.result = -EFAULT;
                break;
            }
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
            ret = 0;
        }
        else {
            ret = -EFAULT;
        }
        break;

    case BOARD_IOCTL_GET_TIMEMS:
        ret = jiffies_to_msecs(jiffies - INITIAL_JIFFIES);
        break;

    case BOARD_IOCTL_GET_DEFAULT_OPTICAL_PARAMS:
    {
        unsigned char ucDefaultOpticalParams[BP_OPTICAL_PARAMS_LEN];
            
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
            ret = 0;
            if (BP_SUCCESS == (ctrlParms.result = BpGetDefaultOpticalParams(ucDefaultOpticalParams)))
            {
                __copy_to_user(ctrlParms.string, ucDefaultOpticalParams, BP_OPTICAL_PARAMS_LEN);

                if (__copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS)) != 0)
                {
                    ret = -EFAULT;
                }
            }                        
        }
        else
        {
            ret = -EFAULT;
        }

        break;
    }
    
    break;
    case BOARD_IOCTL_GET_GPON_OPTICS_TYPE:
     
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) 
        {
            unsigned short Temp=0;
            BpGetGponOpticsType(&Temp);
            *((UINT32*)ctrlParms.buf) = Temp;
            __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
        }        
        ret = 0;

        break;

#if defined(CONFIG_BCM963268) || defined(CONFIG_BCM_6802_MoCA)
    case BOARD_IOCTL_SPI_SLAVE_INIT:  
        ret = 0;
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
             /* Using the 'result' field to specify the SPI device */
             if (kerSysBcmSpiSlaveInit(ctrlParms.result) != SPI_STATUS_OK)
             {
                 ret = -EFAULT;
             }        
        }
        else
        {
            ret = -EFAULT;
        }        
        break;   
        
    case BOARD_IOCTL_SPI_SLAVE_READ:  
        ret = 0;
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
             /* Using the 'result' field to specify the SPI device for reads */
             if (kerSysBcmSpiSlaveRead(ctrlParms.result, ctrlParms.offset, (unsigned long *)ctrlParms.buf, ctrlParms.strLen) != SPI_STATUS_OK)
             {
                 ret = -EFAULT;
             } 
             else
             {
                   __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));    
             }
        }
        else
        {
            ret = -EFAULT;
        }                 
        break;    
        
    case BOARD_IOCTL_SPI_SLAVE_WRITE_BUF:
        ret = 0;
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
             /* Using the 'result' field to specify the SPI device for write buf */
             if (kerSysBcmSpiSlaveWriteBuf(ctrlParms.result, ctrlParms.offset, (unsigned long *)ctrlParms.buf, ctrlParms.strLen, 4) != SPI_STATUS_OK)
             {
                 ret = -EFAULT;
             }
             else
             {
                 __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
             }
        }
        else
        {
            ret = -EFAULT;
        }
        break;

    case BOARD_IOCTL_SPI_SLAVE_WRITE:  
        ret = 0;
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
             /* Using the 'buf' field to specify the SPI device for writes */
             if (kerSysBcmSpiSlaveWrite((uint32_t)ctrlParms.buf, ctrlParms.offset, ctrlParms.result, ctrlParms.strLen) != SPI_STATUS_OK)
             {
                 ret = -EFAULT;
             } 
             else
             {
                 __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));    
             }
        }
        else
        {
            ret = -EFAULT;
        }                 
        break;
    case BOARD_IOCTL_SPI_SLAVE_SET_BITS:
        ret = 0;
#if defined(CONFIG_BCM_6802_MoCA)
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
             /* Using the 'buf' field to specify the SPI device for set bits */
             if (kerSysBcmSpiSlaveModify((uint32_t)ctrlParms.buf, ctrlParms.offset, ctrlParms.result, ctrlParms.result, ctrlParms.strLen) != SPI_STATUS_OK)
             {
                 ret = -EFAULT;
             } 
             else
             {
                 __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));    
             }
        }
        else
        {
            ret = -EFAULT;
        }                 
#endif
        break;
    case BOARD_IOCTL_SPI_SLAVE_CLEAR_BITS:
        ret = 0;
#if defined(CONFIG_BCM_6802_MoCA)
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
        {
             /* Using the 'buf' field to specify the SPI device for clear bits */
             if (kerSysBcmSpiSlaveModify((uint32_t)ctrlParms.buf, ctrlParms.offset, 0, ctrlParms.result, ctrlParms.strLen) != SPI_STATUS_OK)
             {
                 ret = -EFAULT;
             } 
             else
             {
                   __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));    
             }
        }
        else
        {
            ret = -EFAULT;
        }                 
#endif
        break;    
#endif

#if defined(CONFIG_EPON_SDK)
     case BOARD_IOCTL_GET_PORT_MAC_TYPE:
        {
            unsigned short port;
            unsigned long mac_type;

            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
            {
                port = ctrlParms.offset;
                if (BpGetPortMacType(port, &mac_type) == BP_SUCCESS) {
                    ctrlParms.result = (unsigned int)mac_type; 
                    __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                    ret = 0;
                }
                else {
                    ret = -EFAULT;
                }
                break;
            }
        }

    case BOARD_IOCTL_GET_NUM_FE_PORTS:
        {
            unsigned long fe_ports;
            if (BpGetNumFePorts(&fe_ports) == BP_SUCCESS) {
                ctrlParms.result = fe_ports;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,  sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else {
                ret = -EFAULT;
            }
            break;
        }

    case BOARD_IOCTL_GET_NUM_GE_PORTS:
        {
            unsigned long ge_ports;
            if (BpGetNumGePorts(&ge_ports) == BP_SUCCESS) {
                ctrlParms.result = ge_ports;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,  sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else {
                ret = -EFAULT;
            }
            break;
        }

    case BOARD_IOCTL_GET_NUM_VOIP_PORTS:
        {
            unsigned long voip_ports;
            if (BpGetNumVoipPorts(&voip_ports) == BP_SUCCESS) {
                ctrlParms.result = voip_ports;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,  sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else {
                ret = -EFAULT;
            }
            break;
        }

    case BOARD_IOCTL_GET_SWITCH_PORT_MAP:
        {
            unsigned long port_map;
            if (BpGetSwitchPortMap(&port_map) == BP_SUCCESS) {
                ctrlParms.result = port_map;
                __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms,  sizeof(BOARD_IOCTL_PARMS));
                ret = 0;
            }
            else {
                ret = -EFAULT;
            }
            break;
        }


    case BOARD_IOCTL_GET_EPON_GPIOS:
        {
        	int i, rc = 0, gpionum;
        	unsigned short *pusGpio, gpio;
            if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0)
            {
                if( ctrlParms.string )
                {
                    /* walk through the epon gpio list */
                	i = 0;
                	pusGpio = (unsigned short *)ctrlParms.string;
                	gpionum =  ctrlParms.strLen/sizeof(unsigned short);
                    for(;;)
                    {
                     	rc = BpGetEponGpio(i, &gpio);
                       	if( rc == BP_MAX_ITEM_EXCEEDED || i >= gpionum )
                       		break;
                       	else
                       	{
                       		if( rc == BP_SUCCESS )
                       			*pusGpio = gpio;
                       		else
                       			*pusGpio = BP_GPIO_NONE;
                       		pusGpio++;
                       	}
                       	i++;
                     }
                     ctrlParms.result = 0;
                     __copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));
                }
                else
                {
                    ret = -EFAULT;
                }
            }
            break;
        }
#endif

    case BOARD_IOCTL_GET_BATTERY_ENABLE:
        ret = kerSysIsBatteryEnabled();
        break;
#ifdef MSTC_MODIFY // __MSTC__, Paul Ho, __AT&T__, __P870HNP__ ,Autumn
#if defined (WIRELESS) && ( defined(CONFIG_BCM96368) || defined(CONFIG_BCM96328) || defined(CONFIG_BCM963268))
	  case BOARD_IOCTL_GET_PROCCESS_PID:
		 if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
			if(ctrlParms.strLen>0){
			   int i=0;
			   pid_no = 0;
			   for(i=0;i<strlen(ctrlParms.string);i++){
					 pid_no=pid_no*10+(ctrlParms.string[i]-'0');
			   }
				  ctrlParms.result = 0;
			   ret = 0;
			}
			else{
			   ctrlParms.result = -1;
			   ret = -EFAULT;
			}
			__copy_to_user((BOARD_IOCTL_PARMS*)arg, &ctrlParms, sizeof(BOARD_IOCTL_PARMS));

		 } 
		 else
			ret = -EFAULT;
		 break;   
#endif /* defined (WIRELESS) && ( defined(CONFIG_BCM96368) || defined(CONFIG_BCM96328)  || defined(CONFIG_BCM963268)) */
#endif
    case BOARD_IOCTL_MEM_ACCESS:
        {
            BOARD_MEMACCESS_IOCTL_PARMS parms;
            void *va;
            int i;
            int j;
            int blen;
            unsigned char *cp,*bcp;
            unsigned short *sp,*bsp;
            unsigned long *lp,*blp;
            if (copy_from_user((void*)&parms, (void*)arg, sizeof(parms))) 
            {
                ret = -EFAULT;
                break;
            }
            if (parms.op == BOARD_MEMACCESS_IOCTL_OP_FILL) {
                blen = parms.size;
            } else {
                blen = parms.size * parms.count;
            }
            bcp = (unsigned char *)kmalloc(blen, GFP_KERNEL);
            bsp = (unsigned short *)bcp;
            blp = (unsigned long *)bcp;
            if (copy_from_user((void*)bcp, (void*)parms.buf, blen)) 
            {
                ret = -EFAULT;
                kfree(bcp);
                break;
            }
            switch(parms.space) {
            case BOARD_MEMACCESS_IOCTL_SPACE_REG:
                va = ioremap_nocache((int)parms.address, blen);
                break;
            case BOARD_MEMACCESS_IOCTL_SPACE_KERN:
                va = parms.address;
                break;
            default:
                va = NULL;
                ret = -EFAULT;
            }
            // printk("memacecssioctl address started %08x mapped to %08x size is %d count is %d\n",(int)parms.address, (int)va,parms.size, parms.count);
            cp = (unsigned char *)va;
            sp = (unsigned short *)((int)va & ~1);
            lp = (unsigned long *)((int)va & ~3);
            for (i=0; i < parms.count; i++) {
                if ((parms.op == BOARD_MEMACCESS_IOCTL_OP_WRITE) 
                   || (parms.op == BOARD_MEMACCESS_IOCTL_OP_FILL)) {
                    j = 0;
                    if (parms.op == BOARD_MEMACCESS_IOCTL_OP_WRITE) 
                    {
                        j = i;
                    }
                    switch(parms.size) {
                    case 1:
                        cp[i] = bcp[j];
                        break;
                    case 2:
                        sp[i] = bsp[j];
                        break;
                    case 4:
                        lp[i] = blp[j];
                        break;
                    }
                } else {
                    switch(parms.size) {
                    case 1:
                        bcp[i] = cp[i];
                        break;
                    case 2:
                        bsp[i] = sp[i];
                        break;
                    case 4:
                        blp[i] = lp[i];
                        break;
                    }
                }
            }
            __copy_to_user((void *)parms.buf, (void*)bcp, blen);
            if (va != parms.address)
            {
                iounmap(va);
            }
            kfree(bcp);
        }
        break;

#if !defined(CONFIG_BCM960333)
    case BOARD_IOCTL_SET_DYING_GASP_INTERRUPT:
        if (copy_from_user((void*)&ctrlParms, (void*)arg, sizeof(ctrlParms)) == 0) {
           if (ctrlParms.offset)
              kerSysEnableDyingGaspInterrupt();
           else
              kerSysDisableDyingGaspInterrupt();
        }
        break;
#endif

    default:
        ret = -EINVAL;
        ctrlParms.result = 0;
        printk("board_ioctl: invalid command %x, cmd %d .\n",command,_IOC_NR(command));
        break;

    } /* switch */

    return (ret);

} /* board_ioctl */
#ifdef MSTC_MODIFY //__MSTC__, Dennis
static void mustUpdateNvramfield(PNVRAM_DATA nData,PNVRAM_DATA oData)
{
   if(nData->ulVersion!=oData->ulVersion)
      oData->ulVersion = nData->ulVersion;
   if(strcmp(nData->BuildInfo, oData->BuildInfo)!=0)
      memcpy(oData->BuildInfo,nData->BuildInfo,NVRAM_BUILDINFO_LEN);
   if(strcmp(nData->VendorName, oData->VendorName)!=0)
      memcpy(oData->VendorName,nData->VendorName,NVRAM_VENDORNAME_LEN);
   if(strcmp(nData->ProductName, oData->ProductName)!=0)
      memcpy(oData->ProductName,nData->ProductName,NVRAM_PRODUCTNAME_LEN);
}
#endif


/***************************************************************************
* SES Button ISR/GPIO/LED functions.
***************************************************************************/
static Bool sesBtn_pressed(void)
{
	unsigned int intSts = 0, extIntr, value = 0;
	int actHigh = 0;
	Bool pressed = 1;

	if( sesBtn_polling == 0 )
	{
#if defined(CONFIG_BCM96838)
		if ((sesBtn_irq >= INTERRUPT_ID_EXTERNAL_0) && (sesBtn_irq <= INTERRUPT_ID_EXTERNAL_5)) {
#else
		if ((sesBtn_irq >= INTERRUPT_ID_EXTERNAL_0) && (sesBtn_irq <= INTERRUPT_ID_EXTERNAL_3)) {
#endif
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
			intSts = kerSysGetGpioValue(MAP_EXT_IRQ_TO_GPIO( sesBtn_irq - INTERRUPT_ID_EXTERNAL_0));
#elif defined(CONFIG_BCM963381)
			intSts = PERF->ExtIrqSts & (1 << (sesBtn_irq - INTERRUPT_ID_EXTERNAL_0 + EI_STATUS_SHFT));
#else
			intSts = PERF->ExtIrqCfg & (1 << (sesBtn_irq - INTERRUPT_ID_EXTERNAL_0 + EI_STATUS_SHFT));
#endif

		}
		else
			return 0;

		extIntr = extIntrInfo[sesBtn_irq-INTERRUPT_ID_EXTERNAL_0];
		actHigh = IsExtIntrTypeActHigh(extIntr);

		if( ( actHigh && intSts ) || (!actHigh && !intSts ) )
		{
			//check the gpio status here too if shared.
			if( IsExtIntrShared(extIntr) )
			{
				 value = kerSysGetGpioValue(sesBtn_gpio);
				 if( (value&&!actHigh) || (!value&&actHigh) )
					 pressed = 0;
			}
		}
		else
			pressed = 0;
	}
	else
	{
		pressed = 0;
		if( sesBtn_gpio != BP_NOT_DEFINED )
		{
			actHigh = sesBtn_gpio&BP_ACTIVE_LOW ? 0 : 1;
			value = kerSysGetGpioValue(sesBtn_gpio);
		    if( (value&&actHigh) || (!value&&!actHigh) )
			    pressed = 1;
		}
	}

    return pressed;
}

static void sesBtn_timer_handler(unsigned long arg)
{
    unsigned long currentJiffies = jiffies;
    if ( sesBtn_pressed() ) {
        doPushButtonHold(PB_BUTTON_1, currentJiffies);
        mod_timer(&sesBtn_timer, currentJiffies + msecs_to_jiffies(100)); 
    }
    else {
        atomic_set(&sesBtn_active, 0);
        doPushButtonRelease(PB_BUTTON_1, currentJiffies);
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
        BcmHalExternalIrqUnmask(sesBtn_irq);
#else
        BcmHalInterruptEnable(sesBtn_irq);
#endif
    }
}


void sesBtn_defaultAction(unsigned long time, unsigned long param) {
    wake_up_interruptible(&g_board_wait_queue);
}

#ifdef MSTC_WLAN_WPS_BTN_SEPARATE //__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
static Bool wlanBtn_pressed(void)
{
    if ((wlanBtn_irq >= INTERRUPT_ID_EXTERNAL_0) && (wlanBtn_irq <= INTERRUPT_ID_EXTERNAL_3)) {
        if (!(PERF->ExtIrqCfg & (1 << (wlanBtn_irq - INTERRUPT_ID_EXTERNAL_0 + EI_STATUS_SHFT)))) {
            return 1;
        }
    }
#if defined(CONFIG_BCM96368) || defined(CONFIG_BCM96816)
    else if ((wlanBtn_irq >= INTERRUPT_ID_EXTERNAL_4) || (wlanBtn_irq <= INTERRUPT_ID_EXTERNAL_5)) {
        if (!(PERF->ExtIrqCfg1 & (1 << (wlanBtn_irq - INTERRUPT_ID_EXTERNAL_4 + EI_STATUS_SHFT)))) {
            return 1;
        }
    }
#endif
    return 0;
}

#endif

#ifdef MSTC_MODIFY // __MSTC__, Paul Ho, for WPS Button (Merged from 406 common trunk)
void sesBtn_count ( void )
{

#if defined(MSTC_WPS_BTN_SCE)
   struct task_struct *p;
	if ( sesBtn_pressed() ) {

		chkWlanEnCounter++;
		if (chkWlanEnCounter%10 == 0 && chkWlanEnCounter < 200) // do not display "." continuously, just within 20 sec (avoid to affect some other boards)
			printk(".");
		// start timer
#ifdef MTS_WPS_BTN_PER
		if ( chkWlanEnCounter == MTS_WPS_BTN_PER*10 )
		{
			//if ( buttonTest == 0 ) {
				//struct task_struct *p;

				if(pid_no > 0){
					p = find_task_by_vpid(pid_no);
					if(p != NULL) {
						send_sig(SIGALRM,p,0);
					}
				}
			//}
		}
#endif /* MTS_WPS_BTN_PER */

#ifdef NORWAY_BTN_LED_OFF
		if ( LedAllOff == 0 &&  chkWlanEnCounter==2){
			kerSysSendtoMonitorTask(MSG_EXTSW_WLAN_LED_CTL, "1", sizeof(char));  			
		}
#endif
		init_timer(&gResetTimer);
		gResetTimer.function = (void*)sesBtn_count;
		gResetTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
		add_timer (&gResetTimer);
	}
	else {
		
#ifdef NORWAY_BTN_LED_OFF		
		uint32 LedAllOffMinutes_t = 0;
		if (ledBootFlag == 1){
			LedAllOffMinutes_t = (LedAllOffMinutes*60*10 + 1250);
		}else{
			LedAllOffMinutes_t = (LedAllOffMinutes*60*10);
		}
		if (LedAllOffMinutes == 0){
			LedAllOffMinutes_t = LedAllOffMinutes_t + 20;
		}

		if ( chkLedAllOffCounter >= LedAllOffMinutes_t ){
			chkLedAllOffCounter = 0;
			LED->ledLinkActSelLow |= 0x210000;
			GPIO->LEDCtrl |= 0x200000;
			if(chkintenLed ==1 ){
				boardLedCtrl(kLedInternetData, kLedStateOn);
			}else if (chkintenLed ==0 ) {
				boardLedCtrl(kLedInternetData, kLedStateOff);
			}
			LedAllOff = 0;
			kerSysSendtoMonitorTask(MSG_EXTSW_LED_OFF, "0", sizeof(char));	
		}else
#endif

#ifdef MTS_WPS_BTN_PER
		if ( chkWlanEnCounter >= MTS_WPS_BTN_PER*10 )
		{
			printk("\n");
			printk(KERN_WARNING "WPS toogle behavior\n");
#ifdef NORWAY_BTN_LED_OFF
			kerSysSendtoMonitorTask(MSG_EXTSW_WLAN_LED_CTL, "1", sizeof(char));  		
#endif
			//if ( buttonTest == 0 ) {
				kerSysLedCtrl(kLedSes, kLedStateOff);
			//}
			//struct task_struct *p;

			if(pid_no > 0){
				p = find_task_by_vpid(pid_no);
				if(p != NULL) {
					send_sig(SIGUSR2,p,0);
				}
			}
		} 
		else
#endif /* MTS_WPS_BTN_PER */

#ifndef MSTC_WLAN_WPS_BTN_SEPARATE
#ifdef MTS_WLAN_BTN_PER
#ifdef NORWAY_BTN_LED_OFF
		if ( (chkWlanEnCounter >= 2)  &&  (chkWlanEnCounter < MTS_WPS_BTN_PER*10) )
#else
		if ( chkWlanEnCounter >= MTS_WLAN_BTN_PER*10 )
#endif  /* NORWAY_BTN_LED_OFF */
#endif /* MTS_WLAN_BTN_PER */
		{
			printk("\n");
			printk(KERN_WARNING "WLAN toogle behavior\n");
			//if ( buttonTest == 0 ) {
			//struct task_struct *p;

			printk(KERN_WARNING "\r\n WLAN Button Pressed, send SIGUSR1 to wlmngr pid_no:%d\n", pid_no);
			if(pid_no > 0) {            
				p = find_task_by_vpid(pid_no);
				if (p != NULL)
					send_sig(SIGUSR1,p,0);
				else
					printk(KERN_ERR "WLAN on/off can't work.\n");
			}

			//}
			/*else {
				if ( stopblinkwl == 0 ) {
					kerSysLedCtrl(kLedPowerG, kLedStateFastBlinkContinues);
					stopblinkwl = 1;
				}
				else {
					kerSysLedCtrl(kLedPowerG, kLedStateOn);
					stopblinkwl = 0;
				}
			}*/
		}
#ifdef MTS_WLAN_BTN_PER
		else 
#endif /* MTS_WLAN_BTN_PER */
#endif //MSTC_WLAN_WPS_BTN_SEPARATE
		{
			printk("\n");
		}
		chkWlanEnCounter = 0;
		BcmHalInterruptEnable(sesBtn_irq);
	}
#else /* defined(MSTC_WPS_BTN_SCE) */
	if (sesBtn_pressed()) {
		init_timer(&gResetTimer);
		gResetTimer.function = (void*)sesBtn_count;
		gResetTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
		add_timer (&gResetTimer);
	}
	/*else {
		if ( stopblinkwl == 0 ) {
			kerSysLedCtrl(kLedPowerG, kLedStateFastBlinkContinues);
			stopblinkwl = 1;
		}
		else {
			kerSysLedCtrl(kLedPowerG, kLedStateOn);
			stopblinkwl = 0;
		}
		BcmHalInterruptEnable(sesBtn_irq);
	}*/
#endif /* defined(MSTC_WPS_BTN_SCE) */
}

#ifdef MSTC_WLAN_WPS_BTN_SEPARATE //__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
void wlanBtn_count ( void )
{
   struct task_struct *p;
	if ( wlanBtn_pressed() ) {
		chkWlanBtnCounter++;
		if (chkWlanBtnCounter%10 == 0 && chkWlanBtnCounter < 200) // do not display "." continuously, just within 20 sec (avoid to affect some other boards)
			printk(".");
		// start timer
		init_timer(&gResetTimerWlanBtn);
		gResetTimerWlanBtn.function = (void*)wlanBtn_count;
		gResetTimerWlanBtn.expires = jiffies+ktimems_v;        // timer expires in ~100ms
		add_timer (&gResetTimerWlanBtn);
	}
	else {


#ifdef MTS_WLAN_BTN_PER
		if ( chkWlanBtnCounter >= MTS_WLAN_BTN_PER*10 )
#endif /* MTS_WLAN_BTN_PER */
		{
			printk("\n");
			printk(KERN_WARNING "WLAN toogle behavior\n");
			//if ( buttonTest == 0 ) {
			//struct task_struct *p;

			printk(KERN_WARNING "\r\n WLAN Button Pressed, send SIGUSR1 to wlmngr pid_no:%d\n", pid_no);
			if(pid_no > 0) {            
				p = find_task_by_vpid(pid_no);
				if (p != NULL)
					send_sig(SIGUSR1,p,0);
				else
					printk(KERN_ERR "WLAN on/off can't work.\n");
			}

			//}
			/*else {
				if ( stopblinkwl == 0 ) {
					kerSysLedCtrl(kLedPowerG, kLedStateFastBlinkContinues);
					stopblinkwl = 1;
				}
				else {
					kerSysLedCtrl(kLedPowerG, kLedStateOn);
					stopblinkwl = 0;
				}
			}*/
		}
#ifdef MTS_WLAN_BTN_PER
		else 
#endif /* MTS_WLAN_BTN_PER */
		{
			printk("\n");
		}
		chkWlanBtnCounter = 0;
		BcmHalInterruptEnable(wlanBtn_irq);
	}

}

#endif

static irqreturn_t sesBtn_isr(int irq, void *dev_id)
{
    if (sesBtn_pressed()){
#if defined(MSTC_WPS_BTN_SCE)
		wake_up_interruptible(&g_board_wait_queue);

		if (0 == chkWlanEnCounter) {
			chkWlanEnCounter++;
			// start timer
			init_timer(&gResetTimer);
			gResetTimer.function = (void*)sesBtn_count;
			gResetTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
			add_timer (&gResetTimer);
			#ifdef MSTC_WLAN_WPS_BTN_SEPARATE
			printk(KERN_WARNING "WPS btn pressed detected\n");
			#else
			printk(KERN_WARNING "WLAN btn pressed detected\n");
			#endif
		}
#else /* defined(MSTC_WPS_BTN_SCE) */
        wake_up_interruptible(&g_board_wait_queue);

/*        if ( buttonTest == 1 ) {
            // start timer
            init_timer(&gResetTimer);
            gResetTimer.function = (void*)sesBtn_count;
            gResetTimer.expires = jiffies+ktimems_v;        // timer expires in ~100ms
            add_timer (&gResetTimer);
            printk(KERN_WARNING "WLAN btn pressed detected (buttonTest=1)\n");
        }*/
#endif /* defined(MSTC_WPS_BTN_SCE) */
        return IRQ_RETVAL(1);
    } else {
        return IRQ_RETVAL(0);
    }
}
#else // original

static irqreturn_t sesBtn_isr(int irq, void *dev_id)
{
    int ext_irq_idx = 0, value=0;
    irqreturn_t ret = IRQ_NONE;
    unsigned long currentJiffies = jiffies;

    ext_irq_idx = irq - INTERRUPT_ID_EXTERNAL_0;
    if (IsExtIntrShared(extIntrInfo[ext_irq_idx]))
    {
        value = kerSysGetGpioValue(*(int *)dev_id);
        if( (IsExtIntrTypeActHigh(extIntrInfo[ext_irq_idx]) && value) || (IsExtIntrTypeActLow(extIntrInfo[ext_irq_idx]) && !value) )
        {
            ret = IRQ_HANDLED;
        }
    }
    else
    {
        ret = IRQ_HANDLED;
    }

    if (IRQ_HANDLED == ret) {   
        int timerSet = mod_timer(&sesBtn_timer, (currentJiffies + msecs_to_jiffies(100)));	/* 100 msec */
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
        BcmHalExternalIrqMask(irq);
#endif
        if ( 0 == timerSet ) { 
            atomic_set(&sesBtn_active, 1);
            doPushButtonPress(PB_BUTTON_1, currentJiffies);
        }
    }

#if !defined(CONFIG_BCM_6802_MoCA) && !defined(CONFIG_ARM)
    if (IsExtIntrShared(extIntrInfo[ext_irq_idx])) {
        BcmHalInterruptEnable(sesBtn_irq);
    }
#endif

    return ret;
}
#endif

#ifdef MSTC_WLAN_WPS_BTN_SEPARATE //__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
static irqreturn_t wlanBtn_isr(int irq, void *dev_id)
{
    if (wlanBtn_pressed()){

		wake_up_interruptible(&g_board_wait_queue);

		if (0 == chkWlanBtnCounter) {
			chkWlanBtnCounter++;
			// start timer
			init_timer(&gResetTimerWlanBtn);
			gResetTimerWlanBtn.function = (void*)wlanBtn_count;
			gResetTimerWlanBtn.expires = jiffies+ktimems_v;        // timer expires in ~100ms
			add_timer (&gResetTimerWlanBtn);
			printk(KERN_WARNING "WLAN btn pressed detected\n");
		}

        return IRQ_RETVAL(1);
    } else {
        return IRQ_RETVAL(0);
    }
}

#endif
static void __init sesBtn_mapIntr(int context)
{
    int ext_irq_idx;

    if( BpGetWirelessSesExtIntr(&sesBtn_irq) == BP_SUCCESS )
    {
        BpGetWirelessSesExtIntrGpio(&sesBtn_gpio);
        if( sesBtn_irq != BP_EXT_INTR_NONE )
        {
#if defined(CONFIG_BCM960333)
            if( sesBtn_gpio != BP_NOT_DEFINED && sesBtn_irq != BP_EXT_INTR_NONE) 
                mapBcm960333GpioToIntr(sesBtn_gpio & BP_GPIO_NUM_MASK, sesBtn_irq);
#endif      
            printk("SES: Button Interrupt 0x%x is enabled\n", sesBtn_irq);
        }
        else
        {
            if( sesBtn_gpio != BP_NOT_DEFINED )
            {
                printk("SES: Button Polling is enabled on gpio %x\n", sesBtn_gpio);
                kerSysSetGpioDirInput(sesBtn_gpio);
                sesBtn_polling = 1;
            }
        }
    }
    else
        return;

    if( sesBtn_irq != BP_EXT_INTR_NONE )
    {
        ext_irq_idx = (sesBtn_irq&~BP_EXT_INTR_FLAGS_MASK)-BP_EXT_INTR_0;
#if defined(CONFIG_BCM963381)
        kerSysInitPinmuxInterface(BP_PINMUX_FNTYPE_IRQ | ext_irq_idx);
#endif
        if (!IsExtIntrConflict(extIntrInfo[ext_irq_idx]))
        {
            static int dev = -1;
            int hookisr = 1;

            if (IsExtIntrShared(sesBtn_irq))
            {
                /* get the gpio and make it input dir */
                if( sesBtn_gpio != BP_NOT_DEFINED )
                {
                    sesBtn_gpio &= BP_GPIO_NUM_MASK;;
                    printk("SES: Button Interrupt gpio is %d\n", sesBtn_gpio);
                    kerSysSetGpioDirInput(sesBtn_gpio);
                    dev = sesBtn_gpio;
                }
                else
                {
                    printk("SES: Button Interrupt gpio definition not found \n");
                    hookisr = 0;
                }
            }

            if(hookisr)
            {
                init_timer(&sesBtn_timer);
                sesBtn_timer.function = sesBtn_timer_handler;
                sesBtn_timer.expires  = jiffies + msecs_to_jiffies(100);	/* 100 msec */
                sesBtn_timer.data     = 0;
                atomic_set(&sesBtn_active, 0);
                atomic_set(&sesBtn_forced, 0);
                sesBtn_irq = map_external_irq (sesBtn_irq);
                BcmHalMapInterrupt((FN_HANDLER)sesBtn_isr, (unsigned int)&dev, sesBtn_irq);
#if !defined(CONFIG_ARM)
                BcmHalInterruptEnable(sesBtn_irq);
#endif
            }
        }
    }

    return;
}
#ifdef MSTC_WLAN_WPS_BTN_SEPARATE//__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
static void __init wlanBtn_mapIntr(int context)
{
    if( BpGetWirelessBtnExtIntr(&wlanBtn_irq) == BP_SUCCESS )
    {
        printk("WLAN BTN: Button Interrupt 0x%x is enabled\n", wlanBtn_irq);
    }
    else
        return;

    wlanBtn_irq = map_external_irq (wlanBtn_irq) ;

    if (BcmHalMapInterrupt((FN_HANDLER)wlanBtn_isr, context, wlanBtn_irq)) {
        printk(KERN_ERR "WLAN BTN: Interrupt mapping failed\n");
    }
    BcmHalInterruptEnable(wlanBtn_irq);
}
#endif


#if defined(WIRELESS)
#ifdef MSTC_MODIFY  //__MSTC__, for 63168 WPS push button
static unsigned int sesBtn_poll(struct file *file, struct poll_table_struct *wait)
{
    if (sesBtn_pressed()){
        return POLLIN;
    }
    return 0;
}

static ssize_t sesBtn_read(struct file *file,  char __user *buffer, size_t count, loff_t *ppos)
{
    volatile unsigned int event=0;
    ssize_t ret=0;
    //printk("sesBtn_polling is %d",sesBtn_polling);
    if(!sesBtn_pressed()){
        BcmHalInterruptEnable(sesBtn_irq);
        return ret;
    }
    event = SES_EVENTS;
    __copy_to_user((char*)buffer, (char*)&event, sizeof(event));
    BcmHalInterruptEnable(sesBtn_irq);
    count -= sizeof(event);
    buffer += sizeof(event);
    ret += sizeof(event);
    return ret;
}
#else
static unsigned int sesBtn_poll(struct file *file, struct poll_table_struct *wait)
{
    if ( sesBtn_polling ) {
        if ( sesBtn_pressed() ) {
            return POLLIN;
        }
    }
    else if (atomic_read(&sesBtn_active)) {
        return POLLIN;
    }
    else if (atomic_read(&sesBtn_forced)) {
        atomic_set(&sesBtn_forced,0);
        return POLLIN;
    }
    return 0;
}

static ssize_t sesBtn_read(struct file *file,  char __user *buffer, size_t count, loff_t *ppos)
{
    volatile unsigned int event=0;
    ssize_t ret=0;

    if ( sesBtn_polling ) {
        if ( 0 == sesBtn_pressed() ) {
            return ret;
        }
    }
    else if ( 0 == atomic_read(&sesBtn_active) ) {
        return ret;
    }
    event = SES_EVENTS;
    __copy_to_user((char*)buffer, (char*)&event, sizeof(event));
    count -= sizeof(event);
    buffer += sizeof(event);
    ret += sizeof(event);
    return ret;
}
#endif
#endif /* WIRELESS */


void kerSysRegisterMocaHostIntrCallback(MocaHostIntrCallback callback, void * userArg, int dev)
{
    int ext_irq_idx;
    unsigned short  mocaHost_irq;
    PBP_MOCA_INFO  pMocaInfo;
    PMOCA_INTR_ARG pMocaInt;
    unsigned long flags;

    if( dev >=  mocaChipNum )
    {
        printk("kerSysRegisterMocaHostIntrCallback: Error, invalid dev %d\n", dev);
        return;
    }

    pMocaInfo = &mocaInfo[dev];
    mocaHost_irq = pMocaInfo->intr[BP_MOCA_HOST_INTR_IDX];
    if( mocaHost_irq == BP_NOT_DEFINED )
    {
        printk("kerSysRegisterMocaHostIntrCallback: Error, no mocaHost_irq defined in boardparms\n");    
        return;
    }

    printk("kerSysRegisterMocaHostIntrCallback: mocaHost_irq = 0x%x, is_mocaHostIntr_shared=%d\n", mocaHost_irq, IsExtIntrShared(mocaHost_irq));

    ext_irq_idx = (mocaHost_irq&~BP_EXT_INTR_FLAGS_MASK)-BP_EXT_INTR_0;
    if (!IsExtIntrConflict(extIntrInfo[ext_irq_idx]))
    {
        int hookisr = 1;

        pMocaInt = &mocaIntrArg[dev];
        pMocaInt->dev = dev;
        pMocaInt->intrGpio = -1;
        pMocaInt->userArg = userArg;
        pMocaInt->mocaCallback = callback;
        if (IsExtIntrShared(mocaHost_irq))
        {
            /* get the gpio and make it input dir */
            unsigned short gpio;
            if( (gpio = pMocaInfo->intrGpio[BP_MOCA_HOST_INTR_IDX]) != BP_NOT_DEFINED )
            {
                gpio &= BP_GPIO_NUM_MASK;
                printk("MoCA Interrupt gpio is %d\n", gpio);
                kerSysSetGpioDirInput(gpio);
                pMocaInt->intrGpio = gpio;
            }
            else
            {
                  printk("MoCA interrupt gpio definition not found \n");
                  /* still need to hook it because the early bhr board does not have
                   * gpio pin for the shared LAN moca interrupt
                   */
                  hookisr = 1;
            }
        }

        spin_lock_irqsave(&mocaint_spinlock, flags);
        atomic_set(&pMocaInt->enableCount, 0);
        if(hookisr)
        {
            pMocaInt->irq = map_external_irq(mocaHost_irq);
            BcmHalMapInterrupt((FN_HANDLER)mocaHost_isr, (unsigned int)pMocaInt, pMocaInt->irq);
#if !defined(CONFIG_ARM)
            BcmHalInterruptDisable(pMocaInt->irq);
#else
            BcmHalExternalIrqMask(pMocaInt->irq);
#endif
        }
        spin_unlock_irqrestore(&mocaint_spinlock, flags);
    }
}

void kerSysMocaHostIntrEnable(int dev)
{
    PMOCA_INTR_ARG  pMocaInt;
    unsigned long flags;

    spin_lock_irqsave(&mocaint_spinlock, flags);
    if( dev <  mocaChipNum )
    {
        pMocaInt = &mocaIntrArg[dev];

        if (atomic_inc_return(&pMocaInt->enableCount) > 0)
        {
#if !defined(CONFIG_ARM)
           BcmHalInterruptEnable(pMocaInt->irq);
#else
           BcmHalExternalIrqUnmask(pMocaInt->irq);
#endif
        }
    }
    spin_unlock_irqrestore(&mocaint_spinlock, flags);
}

void kerSysMocaHostIntrDisable(int dev)
{
    PMOCA_INTR_ARG  pMocaInt;
    int i;
    unsigned long flags;

    spin_lock_irqsave(&mocaint_spinlock, flags);
    if( dev <  mocaChipNum )
    {
        pMocaInt = &mocaIntrArg[dev];
        
        atomic_dec(&pMocaInt->enableCount);

        for (i=0; i<BP_MOCA_MAX_NUM; i++)
        {
            if ((mocaIntrArg[i].irq == pMocaInt->irq) &&
                (atomic_read(&mocaIntrArg[i].enableCount) > 0))
            {
                // Don't disable this interrupt. It's shared and
                // the other MoCA interface still needs it. 
                spin_unlock_irqrestore(&mocaint_spinlock, flags);
                return;
            }
        }

#if !defined(CONFIG_ARM)
        BcmHalInterruptDisable(pMocaInt->irq);
#else
        BcmHalExternalIrqMask(pMocaInt->irq);
#endif
    }
    spin_unlock_irqrestore(&mocaint_spinlock, flags);
}

static irqreturn_t mocaHost_isr(int irq, void *dev_id)
{
    PMOCA_INTR_ARG pMocaIntrArg = (PMOCA_INTR_ARG)dev_id;
    int isOurs = 1;
    PBP_MOCA_INFO pMocaInfo;
    int ext_irq_idx = 0, value=0, valueReset = 0, valueMocaW = 0;
    unsigned short gpio;

    //printk("mocaHost_isr called for chip %d, irq %d, gpio %d\n", pMocaIntrArg->dev, irq, pMocaIntrArg->intrGpio);
    ext_irq_idx = irq - INTERRUPT_ID_EXTERNAL_0;

    /* When MoCA and SES button share the interrupt, the MoCA handler must be called
       so that the interrupt is re-enabled */
#if defined (WIRELESS)
    if (IsExtIntrShared(extIntrInfo[ext_irq_idx]) && (irq != sesBtn_irq))
#else
    if (IsExtIntrShared(extIntrInfo[ext_irq_idx]))
#endif
    {
        if( pMocaIntrArg->intrGpio != -1 )
        {
            value = kerSysGetGpioValue(pMocaIntrArg->intrGpio);
            if( (IsExtIntrTypeActHigh(extIntrInfo[ext_irq_idx]) && value) || (IsExtIntrTypeActLow(extIntrInfo[ext_irq_idx]) && !value) )
                isOurs = 1;
            else
                isOurs = 0;
        }
        else
        {
            /* for BHR board, the L_HOST_INTR does not have gpio pin. this really sucks! have to check all other interrupt sharing gpio status,
             * if they are not triggering, then it is L_HOST_INTR.  next rev of the board will add gpio for L_HOST_INTR. in the future, all the
             * shared interrupt will have a dedicated gpio pin.
             */
            if( resetBtn_gpio != BP_NOT_DEFINED )
                valueReset = kerSysGetGpioValue(resetBtn_gpio);

               pMocaInfo = &mocaInfo[BP_MOCA_TYPE_WAN];
            if( (gpio = pMocaInfo->intrGpio[BP_MOCA_HOST_INTR_IDX]) != BP_NOT_DEFINED )
                valueMocaW = kerSysGetGpioValue(gpio);

            if( IsExtIntrTypeActHigh(extIntrInfo[ext_irq_idx]) )
            {
                if( (value = (valueReset|valueMocaW)) )
                    isOurs = 0;
            }
            else
            {
                if( (value = (valueReset&valueMocaW)) == 0 )
                    isOurs = 0;
            }

            //printk("BHR board moca_l interrupt: reset %d:%d, ses %d:%d, moca_w %d:%d, isours %d\n", resetBtn_gpio, valueReset,
            //    sesBtn_gpio, valueSes, gpio&BP_GPIO_NUM_MASK, valueMocaW, isOurs);
        }
    }

    if (isOurs)
    {
       if (atomic_read(&pMocaIntrArg->enableCount) > 0)
       {
          pMocaIntrArg->mocaCallback(irq, pMocaIntrArg->userArg);
          return IRQ_HANDLED;
       }
    }

    return IRQ_NONE;
}

#if defined(WIRELESS)
static void __init sesLed_mapGpio()
{
    if( BpGetWirelessSesLedGpio(&sesLed_gpio) == BP_SUCCESS )
    {
        printk("SES: LED GPIO 0x%x is enabled\n", sesLed_gpio);
    }
}
#ifdef MSTC_MODIFY
static void sesLed_ctrl(int action, BOARD_LED_NAME ledName)
#else
static void sesLed_ctrl(int action)
#endif
{
    char blinktype = ((action >> 24) & 0xff); /* extract blink type for SES_LED_BLINK  */

    BOARD_LED_STATE led;

    if(sesLed_gpio == BP_NOT_DEFINED)
        return;

    action &= 0xff; /* extract led */

    switch (action) {
    case SES_LED_ON:
        led = kLedStateOn;
        break;
    case SES_LED_BLINK:
        if(blinktype)
            led = blinktype;
        else
            led = kLedStateSlowBlinkContinues;
        break;
    case SES_LED_OFF:
    default:
        led = kLedStateOff;
    }
#ifdef MSTC_MODIFY
	kerSysLedCtrl(ledName, led);
#else
    kerSysLedCtrl(kLedSes, led);
#endif	
}
#endif

static void __init ses_board_init()
{
    sesBtn_mapIntr(0);
#if defined(WIRELESS)
    sesLed_mapGpio();
#endif
#ifdef MSTC_WLAN_WPS_BTN_SEPARATE	
    wlanBtn_mapIntr(0);
#endif
    registerPushButtonPressNotifyHook(PB_BUTTON_1, sesBtn_defaultAction, 0);
}

static void __exit ses_board_deinit()
{
    if( sesBtn_polling == 0 && sesBtn_irq != BP_NOT_DEFINED )
    {
        int ext_irq_idx = sesBtn_irq - INTERRUPT_ID_EXTERNAL_0;
        if(sesBtn_irq) {
            del_timer(&sesBtn_timer);
            atomic_set(&sesBtn_active, 0);
            atomic_set(&sesBtn_forced, 0);
            if (!IsExtIntrShared(extIntrInfo[ext_irq_idx])) {
                BcmHalInterruptDisable(sesBtn_irq);
            }
#ifdef MSTC_WLAN_WPS_BTN_SEPARATE		
			if(wlanBtn_irq)
				BcmHalInterruptDisable(wlanBtn_irq);
#endif			
        }
    }
}


/***************************************************************************
* Dying gasp ISR and functions.
***************************************************************************/

/* For any driver running on another cpu that needs to know if system is losing
   power */
int kerSysIsDyingGaspTriggered(void)
{
    return isDyingGaspTriggered;
}


#if !defined(CONFIG_BCM960333) 
static irqreturn_t kerSysDyingGaspIsr(int irq, void * dev_id)
{
#if defined(CONFIG_BCM96318)
    unsigned short plcGpio;
#endif
    struct list_head *pos;
    CB_DGASP_LIST *tmp = NULL, *dslOrGpon = NULL;
    unsigned short usPassDyingGaspGpio;		// The GPIO pin to propogate a dying gasp signal

    isDyingGaspTriggered = 1;
    UART->Data = 'D';
    UART->Data = '%';
    UART->Data = 'G';

#if defined (WIRELESS)
    kerSetWirelessPD(WLAN_OFF);
#endif
    /* first to turn off everything other than dsl or gpon */
    list_for_each(pos, &g_cb_dgasp_list_head->list) {
        tmp = list_entry(pos, CB_DGASP_LIST, list);
        if(strncmp(tmp->name, "dsl", 3) && strncmp(tmp->name, "gpon", 4)) {
            (tmp->cb_dgasp_fn)(tmp->context);
        }else {
            dslOrGpon = tmp;
        }
    }
	
    // Invoke dying gasp handlers
    if(dslOrGpon)
        (dslOrGpon->cb_dgasp_fn)(dslOrGpon->context);

    /* reset and shutdown system */


#if defined (CONFIG_BCM96318)
    /* Use GPIO to control the PLC and wifi chip reset on 6319 PLC board*/
    if( BpGetPLCPwrEnGpio(&plcGpio) == BP_SUCCESS )
    {
    	kerSysSetGpioState(plcGpio, kGpioInactive);
    }
#endif

    /* Set WD to fire in 1 sec in case power is restored before reset occurs */
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
    bcm_suspend_watchdog();
#endif
    start_watchdog(1000000, 1);

	// If configured, propogate dying gasp to other processors on the board
	if(BpGetPassDyingGaspGpio(&usPassDyingGaspGpio) == BP_SUCCESS)
	    {
	    // Dying gasp configured - set GPIO
	    kerSysSetGpioState(usPassDyingGaspGpio, kGpioInactive);
	    }

    // If power is going down, nothing should continue!
    while (1);
	return( IRQ_HANDLED );
}
#endif /* !defined(CONFIG_BCM963138) && !defined(CONFIG_BCM963148)*/

#if !defined(CONFIG_BCM960333)
static int dg_enabled = 0;
static void kerSysDisableDyingGaspInterrupt( void )
{
    if (!dg_enabled) {
        return;
    }

    BcmHalInterruptDisable(INTERRUPT_ID_DG);
    dg_enabled = 0;
}

static void kerSysEnableDyingGaspInterrupt( void )
{
    static int dg_mapped = 0;

    /* Ignore requests to enable DG if it is already enabled */
    if (dg_enabled) {
        return;
    }
    
    if (dg_mapped) { 
        BcmHalInterruptEnable(INTERRUPT_ID_DG);
    }
    else {
        BcmHalMapInterrupt((FN_HANDLER)kerSysDyingGaspIsr, 0, INTERRUPT_ID_DG);
#if !defined(CONFIG_ARM)
        BcmHalInterruptEnable( INTERRUPT_ID_DG );
#endif
        dg_mapped = 1;
    }
    dg_enabled = 1;
}
#endif

static void __init kerSysInitDyingGaspHandler( void )
{
    CB_DGASP_LIST *new_node;


    if( g_cb_dgasp_list_head != NULL) {
        printk("Error: kerSysInitDyingGaspHandler: list head is not null\n");
        return;
    }
    new_node= (CB_DGASP_LIST *)kmalloc(sizeof(CB_DGASP_LIST), GFP_KERNEL);
    memset(new_node, 0x00, sizeof(CB_DGASP_LIST));
    INIT_LIST_HEAD(&new_node->list);
    g_cb_dgasp_list_head = new_node;

#if   defined(CONFIG_BCM96838)
    GPIO->dg_control |= (1 << DG_EN_SHIFT); 
    kerSysEnableDyingGaspInterrupt();
    
#elif !defined(CONFIG_BCM960333) 
    {
        int dg_active = 0;
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
        pmc_dsl_power_up();
        msleep(5);
        pmc_dsl_core_reset();
        msleep(5);
        DSLPHY_AFE->BgBiasReg[0] = (DSLPHY_AFE->BgBiasReg[0] & ~0xffff) | 0x0b;
        DSLPHY_AFE->AfeReg[0] = (DSLPHY_AFE->AfeReg[0] & ~0xffff) | 0xc8;
        msleep(5);
        dg_active =  PERF->IrqStatus[0] & (1 << (INTERRUPT_ID_DG - ISR_TABLE_OFFSET));
#elif defined(CONFIG_BCM963381)
        dg_active =  PERF->IrqStatus & (((uint64)1) << (INTERRUPT_ID_DG - INTERNAL_ISR_TABLE_OFFSET));
#else
        dg_active =  PERF->IrqControl[0].IrqStatus & (1 << (INTERRUPT_ID_DG - INTERNAL_ISR_TABLE_OFFSET));
#endif

        if (dg_active) {
            printk("DYING GASP active already -- disabled\n");
        } else if (kerSysIsBatteryEnabled()) {
            printk("DYING GASP enabling postponed until battery is initialised\n");
        } else {
            kerSysEnableDyingGaspInterrupt();
            printk("DYING GASP IRQ initialized \n");
        }
    }
#endif
} /* kerSysInitDyingGaspHandler */

static void __exit kerSysDeinitDyingGaspHandler( void )
{
    struct list_head *pos;
    CB_DGASP_LIST *tmp;

    if(g_cb_dgasp_list_head == NULL)
        return;

    list_for_each(pos, &g_cb_dgasp_list_head->list) {
        tmp = list_entry(pos, CB_DGASP_LIST, list);
        list_del(pos);
        kfree(tmp);
    }

    kfree(g_cb_dgasp_list_head);
    g_cb_dgasp_list_head = NULL;

} /* kerSysDeinitDyingGaspHandler */

void kerSysRegisterDyingGaspHandler(char *devname, void *cbfn, void *context)
{
    CB_DGASP_LIST *new_node;

    // do all the stuff that can be done without the lock first
    if( devname == NULL || cbfn == NULL ) {
        printk("Error: kerSysRegisterDyingGaspHandler: register info not enough (%s,%x,%x)\n", devname, (unsigned int)cbfn, (unsigned int)context);
        return;
    }

    if (strlen(devname) > (IFNAMSIZ - 1)) {
        printk("Warning: kerSysRegisterDyingGaspHandler: devname too long, will be truncated\n");
    }

    new_node= (CB_DGASP_LIST *)kmalloc(sizeof(CB_DGASP_LIST), GFP_KERNEL);
    memset(new_node, 0x00, sizeof(CB_DGASP_LIST));
    INIT_LIST_HEAD(&new_node->list);
    strncpy(new_node->name, devname, IFNAMSIZ-1);
    new_node->cb_dgasp_fn = (cb_dgasp_t)cbfn;
    new_node->context = context;

    // OK, now acquire the lock and insert into list
    mutex_lock(&dgaspMutex);
    if( g_cb_dgasp_list_head == NULL) {
        printk("Error: kerSysRegisterDyingGaspHandler: list head is null\n");
        kfree(new_node);
    } else {
        list_add(&new_node->list, &g_cb_dgasp_list_head->list);
        printk("dgasp: kerSysRegisterDyingGaspHandler: %s registered \n", devname);
    }
    mutex_unlock(&dgaspMutex);

    return;
} /* kerSysRegisterDyingGaspHandler */

void kerSysDeregisterDyingGaspHandler(char *devname)
{
    struct list_head *pos;
    CB_DGASP_LIST *tmp;
    int found=0;

    if(devname == NULL) {
        printk("Error: kerSysDeregisterDyingGaspHandler: devname is null\n");
        return;
    }

    printk("kerSysDeregisterDyingGaspHandler: %s is deregistering\n", devname);

    mutex_lock(&dgaspMutex);
    if(g_cb_dgasp_list_head == NULL) {
        printk("Error: kerSysDeregisterDyingGaspHandler: list head is null\n");
    } else {
        list_for_each(pos, &g_cb_dgasp_list_head->list) {
            tmp = list_entry(pos, CB_DGASP_LIST, list);
            if(!strcmp(tmp->name, devname)) {
                list_del(pos);
                kfree(tmp);
                found = 1;
                printk("kerSysDeregisterDyingGaspHandler: %s is deregistered\n", devname);
                break;
            }
        }
        if (!found)
            printk("kerSysDeregisterDyingGaspHandler: %s not (de)registered\n", devname);
    }
    mutex_unlock(&dgaspMutex);

    return;
} /* kerSysDeregisterDyingGaspHandler */


/***************************************************************************
 *
 *
 ***************************************************************************/
static int ConfigCs (BOARD_IOCTL_PARMS *parms)
{
    int                     retv = 0;
    return( retv );
}

#if defined (WIRELESS) &&!defined(MSTC_WPS_BTN_SCE) // __MSTC__, Paul Ho, ZyXEL Autumn
#if defined(CONFIG_BCM96368) || defined(CONFIG_BCM96328) || defined(CONFIG_BCM963268)
static GPIO_STATE_t GetGpio(int gpio)
{
	GPIO->GPIODir &= ~GPIO_NUM_TO_MASK(gpio);
	if (GPIO->GPIOio & GPIO_NUM_TO_MASK(gpio))
		return kGpioActive;
	else
		return kGpioInactive;
}
#endif /* defined(CONFIG_BCM96368) || defined(CONFIG_BCM96328) || defined(CONFIG_BCM963268) */
#endif /* !defined(MSTC_WPS_BTN_SCE) */

/***************************************************************************
* Handle push of restore to default button
***************************************************************************/
#ifdef MSTC_MODIFY //__MSTC__ , __P870HNP__, __AT&T__ Autumn
#if defined(CONFIG_BCM96816) || defined(CONFIG_BCM96328)|| defined(CONFIG_BCM96368)|| defined(CONFIG_BCM963268)
#if defined(CV_RESTORE_BTN_PER) && !defined(CONFIG_AUXFS_JFFS2)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20)
	static void restore_to_default_thread(struct work_struct *work)
#else
	static void restore_to_default_thread(void *arg)
#endif
	{
		char buf[256]={0};
		// Do this in a kernel thread so we don't have any restriction
		printk ( KERN_WARNING "Clean Configuration" );
		while ( kerSysPersistentSet( buf, 256, 0 ) != 0 ) {
			printk ( "." );
			udelay(100);
		}
		printk ( "\n" );
#ifdef SUPPORT_BACKUP_PSI
		printk ( KERN_WARNING "Clean Backup Configuration" );
		while ( kerSysBackupPsiSet( buf, 256, 0 ) != 0 ) {
			printk ( "." );
			udelay(100);
		}
		printk ( "\n" );
#endif
		kernel_restart(NULL);
		return;
	}
#endif
	
	static Bool resetBtn_pressed(void)
	{
		if ((resetBtn_irq >= INTERRUPT_ID_EXTERNAL_0) && (resetBtn_irq <= INTERRUPT_ID_EXTERNAL_3)) {
			if (!(PERF->ExtIrqCfg & (1 << (resetBtn_irq - INTERRUPT_ID_EXTERNAL_0 + EI_STATUS_SHFT)))) {
				return 1;
			}
		}
#if defined(CONFIG_BCM96368) || defined(CONFIG_BCM96816)
		else if ((resetBtn_irq >= INTERRUPT_ID_EXTERNAL_4) || (resetBtn_irq <= INTERRUPT_ID_EXTERNAL_5)) {
			if (!(PERF->ExtIrqCfg1 & (1 << (resetBtn_irq - INTERRUPT_ID_EXTERNAL_4 + EI_STATUS_SHFT)))) {
				return 1;
			}
		}
#endif
		return 0;
	}
	
	void ResetCounterAdd(void)
	{
		if ( resetBtn_pressed() ) {
			chkResetCounter++;
	
#ifdef CV_RESTORE_BTN_PER
			if ( chkResetCounter >= CV_RESTORE_BTN_PER )
			{
				if ( buttonTest == 0 ) {
					printk("\n");
					printk(KERN_WARNING "Restore to default setting...\n");
					chkResetCounter = -1000;
#ifdef CONFIG_AUXFS_JFFS2 //__MSTC__, Jeff
					kerSysSendtoMonitorTask(MSG_NETLINK_MTS_RESET_DEFAULT, NULL, 0);
#else
					INIT_WORK(&restoreDefaultWork, restore_to_default_thread);
					schedule_work(&restoreDefaultWork);
#endif
					kerSysLedCtrl(kLedPowerG, kLedStateFail);
					return;
				}
			}
#endif
	
			if (chkResetCounter%10 == 0 )
				printk(".");
			// start timer
			init_timer(&gResetTimer);
			gResetTimer.function = (void*)ResetCounterAdd;
			gResetTimer.expires = jiffies+ktimems_v;		// timer expires in ~100ms
			add_timer (&gResetTimer);
		}
		else {
#ifdef CV_RESTORE_BTN_PER
			if ( chkResetCounter >= CV_RESTORE_BTN_PER )
			{
				printk("\n");
				printk(KERN_WARNING "Restore to default setting...\n");
				chkResetCounter = -1000;
				if ( buttonTest == 0 ) {
					kerSysLedCtrl(kLedPowerG, kLedStateOn);
#ifdef CONFIG_AUXFS_JFFS2 //__MSTC__, Jeff
					kerSysSendtoMonitorTask(MSG_NETLINK_MTS_RESET_DEFAULT, NULL, 0);
#else
					INIT_WORK(&restoreDefaultWork, restore_to_default_thread);
					schedule_work(&restoreDefaultWork);
#endif
					return;
				}
		}
#endif

#ifndef MTSC_NORWAY_CUSTOMIZATION
#ifdef CV_RESTORE_BTN_PER

		else if ( chkResetCounter >= 0 ) {
#else
			if ( chkResetCounter >= 0 ) {
#endif
				printk("\n");
				printk(KERN_WARNING "System reset....\n");
				if ( buttonTest == 0 ) {
#ifdef CONFIG_AUXFS_JFFS2 //__MSTC__, Jeff
					kerSysSendtoMonitorTask(MSG_NETLINK_MTS_REBOOT, NULL, 0);
#else
					kernel_restart(NULL);
#endif
					return;
				}
				else {
					if ( stopblink == 0 ) {
						kerSysLedCtrl(kLedPowerG, kLedStateFastBlinkContinues);
						stopblink = 1;
					}
					else {
						kerSysLedCtrl(kLedPowerG, kLedStateOn);
						stopblink = 0;
					}
				}
			}
#endif
			BcmHalInterruptEnable(resetBtn_irq);
		}
	}
	
	static irqreturn_t reset_isr(int irq, void *dev_id)
	{
		if ( resetBtn_pressed() ) {
			chkResetCounter = 0;
			if ( buttonTest == 1 ) {
				wake_up_interruptible(&g_board_wait_queue);
			}
	
			printk(KERN_WARNING "reset btn pressed detected");
	
			// start timer
			init_timer(&gResetTimer);
			gResetTimer.function = (void*)ResetCounterAdd;
			gResetTimer.expires = jiffies+ktimems_v;		// timer expires in ~100ms
			add_timer (&gResetTimer);
			return IRQ_RETVAL(1);
		}
		else {
			return IRQ_RETVAL(0);
		}
	}
#else

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20)
static void restore_to_default_thread(struct work_struct *work)
#else
static void restore_to_default_thread(void *arg)
#endif
{
    char buf[256];

    memset(buf, 0, sizeof(buf));

    // Do this in a kernel thread so we don't have any restriction
    printk("Restore to Factory Default Setting ***\n\n");
    kerSysPersistentSet( buf, sizeof(buf), 0 );
#if defined(CONFIG_BCM_PLC_BOOT)
    kerSysFsFileSet("/data/plc/plc_pconfig_state", buf, 1);
#endif

    // kernel_restart is a high level, generic linux way of rebooting.
    // It calls a notifier list and lets sub-systems know that system is
    // rebooting, and then calls machine_restart, which eventually
    // calls kerSysMipsSoftReset.
    kernel_restart(NULL);

    return;
}

static irqreturn_t reset_isr(int irq, void *dev_id)
{
    int isOurs = 1, ext_irq_idx = 0, value=0;

    //printk("reset_isr called irq %d, gpio %d 0x%lx\n", irq, *(int *)dev_id, PERF->IrqControl32[0].IrqMaskHi);

    ext_irq_idx = irq - INTERRUPT_ID_EXTERNAL_0;
    if (IsExtIntrShared(extIntrInfo[ext_irq_idx]))
    {
       value = kerSysGetGpioValue(*(int *)dev_id);
       if( (IsExtIntrTypeActHigh(extIntrInfo[ext_irq_idx]) && value) || (IsExtIntrTypeActLow(extIntrInfo[ext_irq_idx]) && !value) )
    	   isOurs = 1;
       else
    	   isOurs = 0;
    }

    if (isOurs)
    {
    	if( !restore_in_progress )
    	{
            printk("\n***reset button press detected***\n\n");

            INIT_WORK(&restoreDefaultWork, restore_to_default_thread);
            schedule_work(&restoreDefaultWork);
            restore_in_progress  = 1;
     	}
        return IRQ_HANDLED;
     }

     return IRQ_NONE;
}
#endif
#endif
#if defined(WIRELESS)
/***********************************************************************
* Function Name: kerSysScreenPciDevices
* Description  : Screen Pci Devices before loading modules
***********************************************************************/
static void __init kerSysScreenPciDevices(void)
{
    unsigned short wlFlag;

    if((BpGetWirelessFlags(&wlFlag) == BP_SUCCESS) && (wlFlag & BP_WLAN_EXCLUDE_ONBOARD)) {
        /*
        * scan all available pci devices and delete on board BRCM wireless device
        * if external slot presents a BRCM wireless device
        */
        int foundPciAddOn = 0;
        struct pci_dev *pdevToExclude = NULL;
        struct pci_dev *dev = NULL;

        while((dev=pci_get_device(PCI_ANY_ID, PCI_ANY_ID, dev))!=NULL) {
            printk("kerSysScreenPciDevices: 0x%x:0x%x:(slot %d) detected\n", dev->vendor, dev->device, PCI_SLOT(dev->devfn));
            if((dev->vendor == BRCM_VENDOR_ID) &&
                (((dev->device & 0xff00) == BRCM_WLAN_DEVICE_IDS)|| 
                ((dev->device/1000) == BRCM_WLAN_DEVICE_IDS_DEC))) {
                    if(PCI_SLOT(dev->devfn) != WLAN_ONBOARD_SLOT) {
                        foundPciAddOn++;
                    } else {
                        pdevToExclude = dev;
                    }                
            }
        }

#ifdef CONFIG_PCI 
        if(((wlFlag & BP_WLAN_EXCLUDE_ONBOARD_FORCE) || foundPciAddOn) && pdevToExclude) {
            printk("kerSysScreenPciDevices: 0x%x:0x%x:(onboard) deleted\n", pdevToExclude->vendor, pdevToExclude->device);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
            pci_remove_bus_device(pdevToExclude);
#else
            __pci_remove_bus_device(pdevToExclude);
#endif
        }
#else
#error ATTEMPT TO COMPILE WIRELESS WITHOUT PCI
#endif
    }
}

/***********************************************************************
* Function Name: kerSetWirelessPD
* Description  : Control Power Down by Hardware if the board supports
***********************************************************************/
static void kerSetWirelessPD(int state)
{
    unsigned short wlanPDGpio;
    if((BpGetWirelessPowerDownGpio(&wlanPDGpio)) == BP_SUCCESS) {
        if (wlanPDGpio != BP_NOT_DEFINED) {
            if(state == WLAN_OFF)
                kerSysSetGpioState(wlanPDGpio, kGpioActive);
            else
                kerSysSetGpioState(wlanPDGpio, kGpioInactive);
        }
    }
}
#endif

extern unsigned char g_blparms_buf[];

/***********************************************************************
 * Function Name: kerSysBlParmsGetInt
 * Description  : Returns the integer value for the requested name from
 *                the boot loader parameter buffer.
 * Returns      : 0 - success, -1 - failure
 ***********************************************************************/
int kerSysBlParmsGetInt( char *name, int *pvalue )
{
    char *p2, *p1 = g_blparms_buf;
    int ret = -1;

    *pvalue = -1;

    /* The g_blparms_buf buffer contains one or more contiguous NULL termianted
     * strings that ends with an empty string.
     */
    while( *p1 )
    {
        p2 = p1;

        while( *p2 != '=' && *p2 != '\0' )
            p2++;

        if( *p2 == '=' )
        {
            *p2 = '\0';

            if( !strcmp(p1, name) )
            {
                *p2++ = '=';
                *pvalue = simple_strtol(p2, &p1, 0);
                if( *p1 == '\0' )
                    ret = 0;
                break;
            }

            *p2 = '=';
        }

        p1 += strlen(p1) + 1;
    }

    return( ret );
}

/***********************************************************************
 * Function Name: kerSysBlParmsGetStr
 * Description  : Returns the string value for the requested name from
 *                the boot loader parameter buffer.
 * Returns      : 0 - success, -1 - failure
 ***********************************************************************/
int kerSysBlParmsGetStr( char *name, char *pvalue, int size )
{
    char *p2, *p1 = g_blparms_buf;
    int ret = -1;

    /* The g_blparms_buf buffer contains one or more contiguous NULL termianted
     * strings that ends with an empty string.
     */
    while( *p1 )
    {
        p2 = p1;

        while( *p2 != '=' && *p2 != '\0' )
            p2++;

        if( *p2 == '=' )
        {
            *p2 = '\0';

            if( !strcmp(p1, name) )
            {
                *p2++ = '=';
                strncpy(pvalue, p2, size);
                ret = 0;
                break;
            }

            *p2 = '=';
        }

        p1 += strlen(p1) + 1;
    }

    return( ret );
}

static int proc_get_wan_type(char *buf, char **start, off_t off, int cnt, int *eof, void *data)
{
    int n = 0;

#if defined(CONFIG_BCM96818) || defined(CONFIG_BCM96816)
    sprintf(buf, "gpon%n", &n);
    return n;
#else   /* defined(CONFIG_BCM96818) || defined(CONFIG_BCM96816) */
    unsigned long wan_type = 0, t;
    int i, j, len = 0;

    BpGetOpticalWan(&wan_type);
    if (wan_type == BP_NOT_DEFINED)
    {
        sprintf(buf, "none%n", &n);
        return n;
    }

    for (i = 0, j = 0; wan_type; i++)
    {
        t = wan_type & (1 << i);
        if (!t)
            continue;

        wan_type &= ~(1 << i);
        if (j++)
        {
            sprintf(buf + len, "\n");
            len++;
        }

        switch (t)
        {
        case BP_OPTICAL_WAN_GPON:
            sprintf(buf + len, "gpon%n", &n);
            break;
        case BP_OPTICAL_WAN_EPON:
            sprintf(buf + len, "epon%n", &n);
            break;
        case BP_OPTICAL_WAN_AE:
            sprintf(buf + len, "ae%n", &n);
            break;
        default:
            sprintf(buf + len, "unknown%n", &n);
            break;
        }
        len += n;
    }

    return len;
#endif   /* defined(CONFIG_BCM96818) || defined(CONFIG_BCM96816) */
}

static int add_proc_files(void)
{
#define offset(type, elem) ((int)&((type *)0)->elem)

    static int BaseMacAddr[2] = {offset(NVRAM_DATA, ucaBaseMacAddr), NVRAM_MAC_ADDRESS_LEN};
#ifdef MSTC_MODIFY // __MSTC__, zongyue: reduce manufacture bootup time for wireless calibration
#define offset_idx(type, elem, index) ((int)&((type *)0)->elem[index])
	static int ReduceBootTime[2] = {offset_idx(NVRAM_DATA, FeatureBits, ATMT_FEATUREBITS_IDX), 1};
#endif
    struct proc_dir_entry *p0;
    struct proc_dir_entry *p1;
    struct proc_dir_entry *p2;
#ifdef CONFIG_MSTC_WDT
	/* Add hardware watchdog proc entry */
	mstc_wdt_add_proc();
#endif

#ifdef CONFIG_MSTC_RECORD_CALL_TRACE
	mstc_koops_add_proc();
#endif

#ifdef CONFIG_MSTC_DEC_AND_GZIP_CORE
	mstc_coredump_add_proc();
#endif
    p0 = proc_mkdir("nvram", NULL);

    if (p0 == NULL)
    {
        printk("add_proc_files: failed to create proc files!\n");
        return -1;
    }

    p1 = create_proc_entry("BaseMacAddr", 0644, p0);

    if (p1 == NULL)
    {
        printk("add_proc_files: failed to create proc files!\n");
        return -1;
    }

    p1->data        = BaseMacAddr;
    p1->read_proc   = proc_get_param;
    p1->write_proc  = proc_set_param;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	//New linux no longer requires proc_dir_entry->owner field.
#else
    p1->owner       = THIS_MODULE;
#endif

    p1 = create_proc_entry("led", 0644, NULL);
    if (p1 == NULL)
        return -1;

    p1->write_proc  = proc_set_led;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	//New linux no longer requires proc_dir_entry->owner field.
#else
    p1->owner       = THIS_MODULE;
#endif

    p2 = create_proc_entry("supported_optical_wan_types", 0444, p0);
    if (p2 == NULL)
        return -1;
    p2->read_proc = proc_get_wan_type;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	//New linux no longer requires proc_dir_entry->owner field.
#else
    p2->owner       = THIS_MODULE;
#endif

#if defined(CONFIG_BCM_WATCHDOG_TIMER)
    p2 = create_proc_entry("watchdog", 0644, NULL);
    if (p1 == NULL)
    {
        printk("add_proc_files: failed to create watchdog proc file!\n");
        return -1;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
	//New linux no longer requires proc_dir_entry->owner field.
#else
    p2->owner       = THIS_MODULE;
#endif

    p2->data        = NULL;
    p2->read_proc   = proc_get_watchdog;
    p2->write_proc  = proc_set_watchdog;
#endif /* defined(CONFIG_BCM_WATCHDOG_TIMER) */

#ifdef MSTC_MODIFY // __MSTC__, zongyue: reduce manufacture bootup time for wireless calibration
		p1 = create_proc_entry("ReduceTimeFlag", 0644, p0);
	
		if (p1 == NULL)
		{
			printk("add_proc_files: failed to create proc files!\n");
			return -1;
		}
	
		p1->data		= ReduceBootTime;
		p1->read_proc	= proc_get_param;
		p1->write_proc	= proc_set_param;
	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
		//New linux no longer requires proc_dir_entry->owner field.
#else
		p1->owner		= THIS_MODULE;
#endif
#endif // __MSTC__, zongyue: reduce manufacture bootup time for wireless calibration

    return 0;
}

static int del_proc_files(void)
{
#ifdef CONFIG_MSTC_WDT
	/* Remove hardware watchdog proc entry */
	mstc_wdt_del_proc();
#endif

#ifdef CONFIG_MSTC_RECORD_CALL_TRACE
	mstc_koops_del_proc();
#endif

#ifdef CONFIG_MSTC_DEC_AND_GZIP_CORE
	mstc_coredump_del_proc();
#endif

    remove_proc_entry("nvram", NULL);
    remove_proc_entry("led", NULL);
    return 0;
}

// Use this ONLY to convert strings of bytes to strings of chars
// use functions from linux/kernel.h for everything else
static void str_to_num(char* in, char* out, int len)
{
    int i;
    memset(out, 0, len);

    for (i = 0; i < len * 2; i ++)
    {
        if ((*in >= '0') && (*in <= '9'))
            *out += (*in - '0');
        else if ((*in >= 'a') && (*in <= 'f'))
            *out += (*in - 'a') + 10;
        else if ((*in >= 'A') && (*in <= 'F'))
            *out += (*in - 'A') + 10;
        else
            *out += 0;

        if ((i % 2) == 0)
            *out *= 16;
        else
            out ++;

        in ++;
    }
    return;
}

static int proc_get_param(char *page, char **start, off_t off, int cnt, int *eof, void *data)
{
    int i = 0;
    int r = 0;
    int offset  = ((int *)data)[0];
    int length  = ((int *)data)[1];
    NVRAM_DATA *pNvramData;

    *eof = 1;

    if ((offset < 0) || (offset + length > sizeof(NVRAM_DATA)))
        return 0;

    if (NULL != (pNvramData = readNvramData()))
    {
        for (i = 0; i < length; i ++)
            r += sprintf(page + r, "%02x ", ((unsigned char *)pNvramData)[offset + i]);
    }

    r += sprintf(page + r, "\n");
    if (pNvramData)
        kfree(pNvramData);
    return (r < cnt)? r: 0;
}

static int proc_set_param(struct file *f, const char *buf, unsigned long cnt, void *data)
{
    NVRAM_DATA *pNvramData;
    char input[32];

    int i = 0;
    int r = cnt;
    int offset  = ((int *)data)[0];
    int length  = ((int *)data)[1];

    if ((offset < 0) || (offset + length > sizeof(NVRAM_DATA)))
        return 0;

    if ((cnt > 32) || (copy_from_user(input, buf, cnt) != 0))
        return -EFAULT;

    for (i = 0; i < r; i ++)
    {
        if (!isxdigit(input[i]))
        {
            memmove(&input[i], &input[i + 1], r - i - 1);
            r --;
            i --;
        }
    }

    mutex_lock(&flashImageMutex);

    if (NULL != (pNvramData = readNvramData()))
    {
	
#ifdef MSTC_MODIFY /* __MSTC__, zongyue: protect nvram data */
		if (1 == pNvramData->EngDebugFlag) {
	        str_to_num(input, ((char *)pNvramData) + offset, length);
	        writeNvramDataCrcLocked(pNvramData);
		}
#else
		str_to_num(input, ((char *)pNvramData) + offset, length);
		writeNvramDataCrcLocked(pNvramData);
#endif
    }
    else
    {
        cnt = 0;
    }

    mutex_unlock(&flashImageMutex);

    if (pNvramData)
        kfree(pNvramData);

    return cnt;
}

/*
 * This function expect input in the form of:
 * echo "xxyy" > /proc/led
 * where xx is hex for the led number
 * and   yy is hex for the led state.
 * For example,
 *     echo "0301" > led
 * will turn on led 3
 */
static int proc_set_led(struct file *f, const char *buf, unsigned long cnt, void *data)
{
    // char leddata[16];
    unsigned int leddata;
    char input[32];
    int i;

    if (cnt > 31)
        cnt = 31;

    if (copy_from_user(input, buf, cnt) != 0)
        return -EFAULT;


    for (i = 0; i < cnt; i ++)
    {
        if (!isxdigit(input[i]))
        {
            input[i] = 0;
        }
    }
    input[i] = 0;

    if (0 != kstrtouint(input, 16, &leddata)) 
        return -EFAULT;

    kerSysLedCtrl ((leddata & 0xff00) >> 8, leddata & 0xff);
    return cnt;
}

#if defined(CONFIG_BCM_WATCHDOG_TIMER) || !defined(CONFIG_BCM960333)
static void start_watchdog(unsigned int timer, unsigned int reset) 
{
#if defined(CONFIG_BCM_WATCHDOG_TIMER)
    unsigned long flags;

    spin_lock_irqsave(&watchdog_spinlock, flags); 

    /* if watch dog is disabled and reset is 0, do nothing */
    if (!reset && !watchdog_data.enabled)
    {
        spin_unlock_irqrestore(&watchdog_spinlock, flags); 
        return;
    }
#endif /* defined(CONFIG_BCM_WATCHDOG_TIMER) */

#if defined (CONFIG_BCM96838)
    WDTIMER->WD0Ctl = 0xEE00;
    WDTIMER->WD0Ctl = 0x00EE;
    WDTIMER->WD0DefCount = timer * (FPERIPH/1000000);
    WDTIMER->WD0Ctl = 0xFF00;
    WDTIMER->WD0Ctl = 0x00FF;
#else
    TIMER->WatchDogCtl = 0xEE00;
    TIMER->WatchDogCtl = 0x00EE;
    TIMER->WatchDogDefCount = timer * (FPERIPH/1000000);
    TIMER->WatchDogCtl = 0xFF00;
    TIMER->WatchDogCtl = 0x00FF;
#endif

#if defined(CONFIG_BCM_WATCHDOG_TIMER)
    /* when reset is 1, disable interrupt */
    if (reset && watchdog_data.enabled)
    {
#if defined(INTERRUPT_ID_WDTIMER)
        BcmHalInterruptDisable(INTERRUPT_ID_WDTIMER);
#elif defined(CONFIG_BCM_EXT_TIMER)
        watchdog_callback_register(NULL);
#else
        BcmHalInterruptDisable(INTERRUPT_ID_TIMER);
#endif
    }
    else
    {
        /* reset userTimeout value */
        if (watchdog_data.userMode)
            watchdog_data.userTimeout = 0;
    }
    spin_unlock_irqrestore(&watchdog_spinlock, flags); 
#endif /* defined(CONFIG_BCM_WATCHDOG_TIMER)*/
}
#endif

#if defined(CONFIG_BCM_WATCHDOG_TIMER)
int bcm_suspend_watchdog() 
{
    unsigned long flags;
    int needResume = 0;

    spin_lock_irqsave(&watchdog_spinlock, flags); 

    if (watchdog_data.enabled && !watchdog_data.suspend)
    {
#if defined (CONFIG_BCM96838)
        WDTIMER->WD0Ctl = 0xEE00;
        WDTIMER->WD0Ctl = 0x00EE;
#else
        TIMER->WatchDogCtl = 0xEE00;
        TIMER->WatchDogCtl = 0x00EE;
#endif

        watchdog_data.suspend = 1;
        needResume = 1;
    } 

    spin_unlock_irqrestore(&watchdog_spinlock, flags); 
    return needResume;
}

void bcm_resume_watchdog() 
{
    unsigned long flags;

    start_watchdog(watchdog_data.timer, 0);

    spin_lock_irqsave(&watchdog_spinlock, flags); 
    watchdog_data.suspend = 0;
    spin_unlock_irqrestore(&watchdog_spinlock, flags); 
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20)
static void watchdog_restart_thread(struct work_struct *work)
#else
static void watchdog_restart_thread(void *arg)
#endif
{
    // kernel_restart is a high level, generic linux way of rebooting.
    // It calls a notifier list and lets sub-systems know that system is
    // rebooting, and then calls machine_restart, which eventually
    // calls kerSysMipsSoftReset.
    kernel_restart(NULL);

    return;
}

#if defined(INTERRUPT_ID_WDTIMER) || !defined(CONFIG_BCM_EXT_TIMER)
static irqreturn_t watchdog_isr(int irq, void *ignore)
#else
static void watchdog_isr(int param)
#endif
{
    unsigned long flags;
    struct list_head *pos;
    CB_WDOG_LIST *tmp = NULL;
    int reboot = 0;

#if !defined(INTERRUPT_ID_WDTIMER) && !defined(CONFIG_BCM_EXT_TIMER)
    /* 
     * if WD shares timer interrupt and EXT_TIMER is disabled, 
     * need to check if it is WD interrupt
     */ 
    if (!(TIMER->TimerInts & WATCHDOG))
        return IRQ_NONE;
#endif

    spin_lock_irqsave(&watchdog_spinlock, flags); 

#if defined(INTERRUPT_ID_WDTIMER) || !defined(CONFIG_BCM_EXT_TIMER)
    /* 
     * if WD shares timer interrupt and EXT_TIMER is enabled, 
     * interrupt be cleared in ext_timer_isr.
     */ 

    /* clear the interrupt */
    TIMER->TimerInts |= WATCHDOG;

#if !defined(CONFIG_ARM)
#if defined(INTERRUPT_ID_WDTIMER)
    BcmHalInterruptEnable(INTERRUPT_ID_WDTIMER);
#else
    BcmHalInterruptEnable(INTERRUPT_ID_TIMER);
#endif /* defined(INTERRUPT_ID_WDTIMER) */
#endif /* !defined(CONFIG_ARM) */
#endif /*  defined(INTERRUPT_ID_WDTIMER) || !defined(CONFIG_BCM_EXT_TIMER) */

#if defined (CONFIG_BCM96838)
    WDTIMER->WD0Ctl = 0xEE00;
    WDTIMER->WD0Ctl = 0x00EE;
    WDTIMER->WD0Ctl = 0xFF00;
    WDTIMER->WD0Ctl = 0x00FF;
#else
    /* stop and reload timer counter then start WD */
    TIMER->WatchDogCtl = 0xEE00;
    TIMER->WatchDogCtl = 0x00EE;
    TIMER->WatchDogCtl = 0xFF00;
    TIMER->WatchDogCtl = 0x00FF;
#endif

    /* check watchdog callback function */
    list_for_each(pos, &g_cb_wdog_list_head->list) 
    {
        tmp = list_entry(pos, CB_WDOG_LIST, list);
        if ((tmp->cb_wd_fn)(tmp->context))
        {
            reboot = 1;
            printk("\nwatchdog cb of %s return 1, reset CPE!!!\n", tmp->name);
            break;
        }
    }

    if (!reboot && watchdog_data.userMode)
    {
        watchdog_data.userTimeout++;
        if (watchdog_data.userTimeout >= watchdog_data.userThreshold)
        {
            reboot = 1;
            printk("\nHit userMode watchdog threshold, reset CPE!!!\n");
        }
    }

    if (reboot)
    {
        spin_unlock_irqrestore(&watchdog_spinlock, flags); 
        bcm_suspend_watchdog();
        /* 
         *  If call kerSysMipsSoftReset() in interrupt,  
         *  kernel smp pops out warning.  
         */
        if( !watchdog_restart_in_progress )
        {
            INIT_WORK(&watchdogRestartWork, watchdog_restart_thread);
            schedule_work(&watchdogRestartWork);
            watchdog_restart_in_progress  = 1;
        }

#if defined(INTERRUPT_ID_WDTIMER) || !defined(CONFIG_BCM_EXT_TIMER)
        return IRQ_HANDLED;
#else
        return;
#endif
    }

    spin_unlock_irqrestore(&watchdog_spinlock, flags); 

#if defined(INTERRUPT_ID_WDTIMER) || !defined(CONFIG_BCM_EXT_TIMER)
    return IRQ_HANDLED;
#endif
}

static int proc_get_watchdog(char *page, char **start, off_t off, int cnt, int *eof, void *data)
{

    int r = 0;
    *eof = 1;
    r += sprintf(page + r, "watchdog enabled=%u timer=%u ns suspend=%u\n", 
                           watchdog_data.enabled, 
                           watchdog_data.timer, 
                           watchdog_data.suspend);
    r += sprintf(page + r, "         userMode=%u userThreshold=%u userTimeout=%u\n", 
                           watchdog_data.userMode, 
                           watchdog_data.userThreshold/2, 
                           watchdog_data.userTimeout/2);
    return (r < cnt)? r: 0;
}

static int proc_set_watchdog(struct file *f, const char *buf, unsigned long cnt, void *data)
{
    unsigned long flags;
    char input[64];
    unsigned int enabled, timer;
    unsigned int userMode, userThreshold;
   
    if (cnt > 64) 
    {
        cnt = 64;
    }

    if (copy_from_user(input, buf, cnt) != 0) 
    {
        return -EFAULT;
    }

    if (strncasecmp(input, "OK", 2) == 0)
    {
        spin_lock_irqsave(&watchdog_spinlock, flags); 
        if (watchdog_data.userMode)
            watchdog_data.userTimeout = 0;
        spin_unlock_irqrestore(&watchdog_spinlock, flags); 
        return cnt;
    }

    if (sscanf(input, "%u %u %u %u", &enabled, &timer, &userMode, &userThreshold) != 4)
    {
        printk("\nError format, it is as:\n");
        printk("\"enabled(0|1) timer(ns) userMode(0|1) userThreshold\"\n");
        return -EFAULT;
    }

    spin_lock_irqsave(&watchdog_spinlock, flags); 
    
    watchdog_data.userMode = userMode;
    watchdog_data.userThreshold = userThreshold * 2; // watchdog interrupt is half of timer
    watchdog_data.userTimeout = 0;             // reset userTimeout
    if (watchdog_data.enabled != enabled) 
    { 
        watchdog_data.timer = timer;
        if (enabled)
        {
#if defined(INTERRUPT_ID_WDTIMER)
            BcmHalMapInterrupt((FN_HANDLER)watchdog_isr , 0, INTERRUPT_ID_WDTIMER);
#if !defined(CONFIG_ARM)
            BcmHalInterruptEnable(INTERRUPT_ID_WDTIMER);
#endif /* !defined(CONFIG_ARM) */
#elif defined(CONFIG_BCM_EXT_TIMER)
            watchdog_callback_register(&watchdog_isr);
#else
            /* 
             *  The 2nd parameter must be unique to share same IRQ.
             *  We need to pass the same magic value when call free_irq().
             */
            BcmHalMapInterrupt((FN_HANDLER)watchdog_isr, 0xabcd1212, INTERRUPT_ID_TIMER);
            BcmHalInterruptEnable(INTERRUPT_ID_TIMER);
#endif
            watchdog_data.enabled = enabled;
            watchdog_data.suspend = 0;
            spin_unlock_irqrestore(&watchdog_spinlock, flags); 
            start_watchdog(watchdog_data.timer, 0);
        }
        else
        {
            spin_unlock_irqrestore(&watchdog_spinlock, flags); 
            bcm_suspend_watchdog();
#if defined(INTERRUPT_ID_WDTIMER)
            free_irq(INTERRUPT_ID_WDTIMER, NULL);
#elif defined(CONFIG_BCM_EXT_TIMER)
            watchdog_callback_register(NULL);
#else
            free_irq(INTERRUPT_ID_TIMER, (void *)0xabcd1212);
#endif
            watchdog_data.enabled = enabled;
        }
    }
    else if (watchdog_data.timer != timer)
    {
        watchdog_data.timer = timer;
        if (watchdog_data.enabled)
        {
            watchdog_data.suspend = 0;
            spin_unlock_irqrestore(&watchdog_spinlock, flags); 
            start_watchdog(watchdog_data.timer, 0);
        }    
    }
    else
        spin_unlock_irqrestore(&watchdog_spinlock, flags); 

    return cnt;
}

static void __init kerSysInitWatchdogCBList( void )
{
    CB_WDOG_LIST *new_node;
    unsigned long flags;

    if( g_cb_wdog_list_head != NULL) 
    {
        printk("Error: kerSysInitWatchdogCBList: list head is not null\n");
        return;
    }
    new_node= (CB_WDOG_LIST *)kmalloc(sizeof(CB_WDOG_LIST), GFP_KERNEL);
    memset(new_node, 0x00, sizeof(CB_WDOG_LIST));
    INIT_LIST_HEAD(&new_node->list);
    spin_lock_irqsave(&watchdog_spinlock, flags); 
    g_cb_wdog_list_head = new_node;
    spin_unlock_irqrestore(&watchdog_spinlock, flags); 
} 

void kerSysRegisterWatchdogCB(char *devname, void *cbfn, void *context)
{
    CB_WDOG_LIST *new_node;
    unsigned long flags;

    // do all the stuff that can be done without the lock first
    if( devname == NULL || cbfn == NULL ) 
    {
        printk("Error: kerSysRegisterWatchdogCB: register info not enough (%s,%x,%x)\n", devname, (unsigned int)cbfn, (unsigned int)context);
        return;
    }

    if (strlen(devname) > (IFNAMSIZ - 1))
        printk("Warning: kerSysRegisterWatchdogCB: devname too long, will be truncated\n");

    new_node= (CB_WDOG_LIST *)kmalloc(sizeof(CB_WDOG_LIST), GFP_KERNEL);
    memset(new_node, 0x00, sizeof(CB_WDOG_LIST));
    INIT_LIST_HEAD(&new_node->list);
    strncpy(new_node->name, devname, IFNAMSIZ-1);
    new_node->cb_wd_fn = (cb_watchdog_t)cbfn;
    new_node->context = context;

    // OK, now acquire the lock and insert into list
    spin_lock_irqsave(&watchdog_spinlock, flags); 
    if( g_cb_wdog_list_head == NULL) 
    {
        printk("Error: kerSysRegisterWatchdogCB: list head is null\n");
        kfree(new_node);
    } 
    else 
    {
        list_add(&new_node->list, &g_cb_wdog_list_head->list);
        printk("watchdog: kerSysRegisterWatchdogCB: %s registered \n", devname);
    }
    spin_unlock_irqrestore(&watchdog_spinlock, flags); 

    return;
} 

void kerSysDeregisterWatchdogCB(char *devname)
{
    struct list_head *pos;
    CB_WDOG_LIST *tmp;
    int found=0;
    unsigned long flags;

    if(devname == NULL) {
        printk("Error: kerSysDeregisterWatchdogCB: devname is null\n");
        return;
    }

    printk("kerSysDeregisterWatchdogCB: %s is deregistering\n", devname);

    spin_lock_irqsave(&watchdog_spinlock, flags); 
    if(g_cb_wdog_list_head == NULL) 
    {
        printk("Error: kerSysDeregisterWatchdogCB: list head is null\n");
    } 
    else 
    {
        list_for_each(pos, &g_cb_wdog_list_head->list) 
        {
            tmp = list_entry(pos, CB_WDOG_LIST, list);
            if(!strcmp(tmp->name, devname)) 
            {
                list_del(pos);
                kfree(tmp);
                found = 1;
                printk("kerSysDeregisterWatchdogCB: %s is deregistered\n", devname);
                break;
            }
        }
        if (!found)
            printk("kerSysDeregisterWatchdogCB: %s not (de)registered\n", devname);
    }
    spin_unlock_irqrestore(&watchdog_spinlock, flags); 

    return;
} 

#endif /* defined(CONFIG_BCM_WATCHDOG_TIMER) */

static DEFINE_SPINLOCK(pinmux_spinlock);

void kerSysInitPinmuxInterface(unsigned long interface) {
    unsigned long flags;
    spin_lock_irqsave(&pinmux_spinlock, flags); 
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148) || defined(CONFIG_BCM963381)
    bcm_init_pinmux_interface(interface);
#endif
    spin_unlock_irqrestore(&pinmux_spinlock, flags); 
}



/***************************************************************************
 * Function Name: kerSysGetUbusFreq
 * Description  : Chip specific computation.
 * Returns      : the UBUS frequency value in MHz.
 ***************************************************************************/
unsigned long kerSysGetUbusFreq(unsigned long miscStrapBus)
{
   unsigned long ubus = UBUS_BASE_FREQUENCY_IN_MHZ;

#if defined(CONFIG_BCM96362)
   /* Ref RDB - 6362 */
   switch (miscStrapBus) {

      case 0x4 :
      case 0xc :
      case 0x14:
      case 0x1c:
      case 0x15:
      case 0x1d:
         ubus = 100;
         break;
      case 0x2 :
      case 0xa :
      case 0x12:
      case 0x1a:
         ubus = 96;
         break;
      case 0x1 :
      case 0x9 :
      case 0x11:
      case 0xe :
      case 0x16:
      case 0x1e:
         ubus = 200;
         break;
      case 0x6:
         ubus = 183;
         break;
      case 0x1f:
         ubus = 167;
         break;
      default:
         ubus = 160;
         break;
   }
#endif

   return (ubus);

}  /* kerSysGetUbusFreq */


/***************************************************************************
 * Function Name: kerSysGetChipId
 * Description  : Map id read from device hardware to id of chip family
 *                consistent with  BRCM_CHIP
 * Returns      : chip id of chip family
 ***************************************************************************/
int kerSysGetChipId() { 
        int r;
#if   defined(CONFIG_BCM96838)
        r = 0x6838;
#elif defined(CONFIG_BCM960333)
        r = 0x60333;
#else
        r = (int) ((PERF->RevID & CHIP_ID_MASK) >> CHIP_ID_SHIFT);
        /* Force BCM63168, BCM63169, and BCM63269 to be BCM63268) */
        if( ( (r & 0xffffe) == 0x63168 )
          || ( (r & 0xffffe) == 0x63268 ))
            r = 0x63268;

        /* Force 6319 to be BCM6318 */
        if (r == 0x6319)
            r = 0x6318;

#endif

        return(r);
}

#if !defined(CONFIG_BCM960333)
/***************************************************************************
 * Function Name: kerSysGetDslPhyEnable
 * Description  : returns true if device should permit Phy to load
 * Returns      : true/false
 ***************************************************************************/
int kerSysGetDslPhyEnable() {
        int id;
        int r = 1;
        id = (int) ((PERF->RevID & CHIP_ID_MASK) >> CHIP_ID_SHIFT);
        if ((id == 0x63169) || (id == 0x63269)) {
	    r = 0;
        }
        return(r);
}
#endif

/***************************************************************************
 * Function Name: kerSysGetChipName
 * Description  : fills buf with the human-readable name of the device
 * Returns      : pointer to buf
 ***************************************************************************/
char *kerSysGetChipName(char *buf, int n) {
	return(UtilGetChipName(buf, n));
}

/***************************************************************************
 * Function Name: kerSysGetExtIntInfo
 * Description  : return the external interrupt information which includes the
 *                trigger type, sharing enable.
 * Returns      : pointer to buf
 ***************************************************************************/
unsigned int kerSysGetExtIntInfo(unsigned int irq)
{
	return extIntrInfo[irq-INTERRUPT_ID_EXTERNAL_0];
}


int kerSysGetPciePortEnable(int port)
{
	int ret = 1;
#if defined (CONFIG_BCM96838)
	unsigned int chipId = (PERF->RevID & CHIP_ID_MASK) >> CHIP_ID_SHIFT;

    switch (chipId)
	{
		case 1:		// 68380
		case 6:		// 68380M
			ret = 1;
			break;
			
		case 3:		// 68380F
			if(port == 0)
				ret = 1;
			else
				ret = 0;
			break;
		
		case 4:		// 68385
		case 5:		// 68381
		default:
			ret = 0;
			break;
    }
#endif	
	return ret;
}
EXPORT_SYMBOL(kerSysGetPciePortEnable);

int kerSysGetUsbHostPortEnable(int port)
{
	int ret = 1;
#if defined (CONFIG_BCM96838)
	unsigned int chipId = (PERF->RevID & CHIP_ID_MASK) >> CHIP_ID_SHIFT;
	
    switch (chipId)
	{
		case 1:		// 68380
		case 6:		// 68380M
			ret = 1;
			break;
			
		case 3:		// 68380F
			if(port == 0)
				ret = 1;
			else
				ret = 0;
			break;
		
		case 4:		// 68385
		case 5:		// 68381
		default:
			ret = 0;
			break;
    }
#endif	
	return ret;
}
EXPORT_SYMBOL(kerSysGetUsbHostPortEnable);

int kerSysGetUsbDeviceEnable(void)
{
	int ret = 1;
	
#if defined (CONFIG_BCM96838)
	ret = 0;
#endif	

	return ret;
}
EXPORT_SYMBOL(kerSysGetUsbDeviceEnable);

int kerSysGetUsb30HostEnable(void)
{
	int ret = 0;
	
#if defined(CONFIG_BCM963138)|| defined(CONFIG_BCM963148)
	ret = 1;
#endif	

	return ret;
}
EXPORT_SYMBOL(kerSysGetUsb30HostEnable);

int kerSysSetUsbPower(int on, USB_FUNCTION func)
{
	int status = 0;
#if !defined(CONFIG_BRCM_IKOS)
#if defined (CONFIG_BCM96838)
	static int usbHostPwr = 1;
	static int usbDevPwr = 1;
	
	if(on)
	{
		if(!usbHostPwr && !usbDevPwr)
			status = PowerOnZone(PMB_ADDR_USB30_2X, USB30_2X_Zone_Common);
		
		if(((func == USB_HOST_FUNC) || (func == USB_ALL_FUNC)) && !usbHostPwr && !status)
		{
			status = PowerOnZone(PMB_ADDR_USB30_2X, USB30_2X_Zone_USB_Host);
			if(!status)
				usbHostPwr = 1;
		}
		
		if(((func == USB_DEVICE_FUNC) || (func == USB_ALL_FUNC)) && !usbDevPwr && !status)
		{
			status = PowerOnZone(PMB_ADDR_USB30_2X, USB30_2X_Zone_USB_Device);
			if(!status)
				usbDevPwr = 1;
		}
	}
	else
	{
		if(((func == USB_HOST_FUNC) || (func == USB_ALL_FUNC)) && usbHostPwr)
		{
			status = PowerOffZone(PMB_ADDR_USB30_2X, USB30_2X_Zone_USB_Host);
			if(!status)
				usbHostPwr = 0;
		}
		
		if(((func == USB_DEVICE_FUNC) || (func == USB_ALL_FUNC)) && usbDevPwr)
		{
			status = PowerOffZone(PMB_ADDR_USB30_2X, USB30_2X_Zone_USB_Device);
			if(!status)
					usbDevPwr = 0;
		}
		
		if(!usbHostPwr && !usbDevPwr)
			status = PowerOffZone(PMB_ADDR_USB30_2X, USB30_2X_Zone_Common);
	}
#endif
#endif

	return status;
}
EXPORT_SYMBOL(kerSysSetUsbPower);

extern const struct obs_kernel_param __setup_start[], __setup_end[];
extern const struct kernel_param __start___param[], __stop___param[];

void kerSysSetBootParm(char *name, char *value)
{
    const struct obs_kernel_param *okp = __setup_start;
    const struct kernel_param *kp = __start___param;

    do {
        if (!strcmp(name, okp->str)) {
            if (okp->setup_func) {
                okp->setup_func(value);
                return;
            }
        }
        okp++;
    } while (okp < __setup_end);

    do {
        if (!strcmp(name, kp->name)) {
            if (kp->ops->set) {
                kp->ops->set(value, kp);
                return;
            }
        }
        kp++;
    } while (kp < __stop___param);
}
EXPORT_SYMBOL(kerSysSetBootParm);

/***************************************************************************
* MACRO to call driver initialization and cleanup functions.
***************************************************************************/
module_init( brcm_board_init );
module_exit( brcm_board_cleanup );

EXPORT_SYMBOL(dumpaddr);
EXPORT_SYMBOL(kerSysGetChipId);
EXPORT_SYMBOL(kerSysGetChipName);
EXPORT_SYMBOL(kerSysMacAddressNotifyBind);
EXPORT_SYMBOL(kerSysGetMacAddressType);
#ifdef MSTC_MODIFY // __MSTC__, Richard Huang
EXPORT_SYMBOL(kerSysGetBaseMacAddress);
#endif
EXPORT_SYMBOL(kerSysGetMacAddress);
EXPORT_SYMBOL(kerSysReleaseMacAddress);
EXPORT_SYMBOL(kerSysGetGponSerialNumber);
EXPORT_SYMBOL(kerSysGetGponPassword);
EXPORT_SYMBOL(kerSysGetSdramSize);
#if !defined(CONFIG_BCM960333)
EXPORT_SYMBOL(kerSysGetDslPhyEnable);
EXPORT_SYMBOL(kerSysGetExtIntInfo);
#endif
EXPORT_SYMBOL(kerSysSetOpticalPowerValues);
EXPORT_SYMBOL(kerSysGetOpticalPowerValues);
EXPORT_SYMBOL(kerSysLedCtrl);
EXPORT_SYMBOL(kerSysRegisterDyingGaspHandler);
EXPORT_SYMBOL(kerSysDeregisterDyingGaspHandler);
EXPORT_SYMBOL(kerSysIsDyingGaspTriggered);
EXPORT_SYMBOL(kerSysSendtoMonitorTask);
EXPORT_SYMBOL(kerSysGetWlanSromParams);
EXPORT_SYMBOL(kerSysGetWlanFeature);
EXPORT_SYMBOL(kerSysGetAfeId);
EXPORT_SYMBOL(kerSysGetUbusFreq);
EXPORT_SYMBOL(kerSysInitPinmuxInterface);
#if defined(CONFIG_BCM96816) || defined(CONFIG_BCM96818) || defined(CONFIG_BCM963268) || defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148) || defined(CONFIG_BCM96838)
EXPORT_SYMBOL(kerSysBcmSpiSlaveRead);
EXPORT_SYMBOL(kerSysBcmSpiSlaveReadReg32);
EXPORT_SYMBOL(kerSysBcmSpiSlaveWrite);
EXPORT_SYMBOL(kerSysBcmSpiSlaveWriteReg32);
EXPORT_SYMBOL(kerSysBcmSpiSlaveWriteBuf);
#if defined(CONFIG_BCM_6802_MoCA)
EXPORT_SYMBOL(kerSysBcmSpiSlaveReadBuf);
EXPORT_SYMBOL(kerSysBcmSpiSlaveModify);
#endif
EXPORT_SYMBOL(kerSysRegisterMocaHostIntrCallback);
EXPORT_SYMBOL(kerSysMocaHostIntrEnable);
EXPORT_SYMBOL(kerSysMocaHostIntrDisable);
#endif


#if defined(CONFIG_BCM_WATCHDOG_TIMER)
EXPORT_SYMBOL(kerSysRegisterWatchdogCB);
EXPORT_SYMBOL(kerSysDeregisterWatchdogCB);
EXPORT_SYMBOL(bcm_suspend_watchdog);
EXPORT_SYMBOL(bcm_resume_watchdog);
#endif

EXPORT_SYMBOL(BpGetSimInterfaces);
EXPORT_SYMBOL(BpGetBoardId);
EXPORT_SYMBOL(BpGetBoardIds);
EXPORT_SYMBOL(BpGetGPIOverlays);
EXPORT_SYMBOL(BpGetFpgaResetGpio);
EXPORT_SYMBOL(BpGetEthernetMacInfo);
EXPORT_SYMBOL(BpGetEthernetMacInfoArrayPtr);
EXPORT_SYMBOL(BpGetGphyBaseAddress);
EXPORT_SYMBOL(BpGetDeviceOptions);
#ifdef MSTC_MODIFY
/*Add flag here,when CONFIG_BCM_EXT_SWITCH is not set in profile,will cause compile error. No need adding flag here,bcm driver will automaticly detect ext switch.*/
#if (defined(CONFIG_BCM963268) || defined(CONFIG_BCM_6362_PORTS_INT_EXT_SW))// && defined(CONFIG_BCM_EXT_SWITCH)
EXPORT_SYMBOL(BpGetPortConnectedToExtSwitch);
#endif
#else
EXPORT_SYMBOL(BpGetPortConnectedToExtSwitch);
#endif
EXPORT_SYMBOL(BpGetRj11InnerOuterPairGpios);
EXPORT_SYMBOL(BpGetRtsCtsUartGpios);
EXPORT_SYMBOL(BpGetAdslLedGpio);
EXPORT_SYMBOL(BpGetAdslFailLedGpio);
EXPORT_SYMBOL(BpGetWanDataLedGpio);
EXPORT_SYMBOL(BpGetWanErrorLedGpio);
EXPORT_SYMBOL(BpGetVoipLedGpio);
EXPORT_SYMBOL(BpGetPotsLedGpio);
EXPORT_SYMBOL(BpGetVoip2FailLedGpio);
EXPORT_SYMBOL(BpGetVoip2LedGpio);
EXPORT_SYMBOL(BpGetVoip1FailLedGpio);
EXPORT_SYMBOL(BpGetVoip1LedGpio);
EXPORT_SYMBOL(BpGetDectLedGpio);
EXPORT_SYMBOL(BpGetMoCALedGpio);
EXPORT_SYMBOL(BpGetMoCAFailLedGpio);
EXPORT_SYMBOL(BpGetWirelessSesExtIntr);
#if defined(MTSC_NORWAY_CUSTOMIZATION)  //Telenor_Norway
EXPORT_SYMBOL(BpGetWlanEnLedGpio);
#endif
#ifdef MSTC_MODIFY//__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
EXPORT_SYMBOL(BpGetWirelessBtnExtIntr);
#endif
EXPORT_SYMBOL(BpGetWirelessSesLedGpio);
EXPORT_SYMBOL(BpGetWirelessFlags);
EXPORT_SYMBOL(BpGetWirelessPowerDownGpio);
EXPORT_SYMBOL(BpUpdateWirelessSromMap);
EXPORT_SYMBOL(BpGetSecAdslLedGpio);
EXPORT_SYMBOL(BpGetSecAdslFailLedGpio);
EXPORT_SYMBOL(BpGetDslPhyAfeIds);
EXPORT_SYMBOL(BpGetExtAFEResetGpio);
EXPORT_SYMBOL(BpGetExtAFELDPwrGpio);
EXPORT_SYMBOL(BpGetExtAFELDModeGpio);
EXPORT_SYMBOL(BpGetIntAFELDPwrGpio);
EXPORT_SYMBOL(BpGetIntAFELDModeGpio);
EXPORT_SYMBOL(BpGetAFELDRelayGpio);
EXPORT_SYMBOL(BpGetExtAFELDDataGpio);
EXPORT_SYMBOL(BpGetExtAFELDClkGpio);
EXPORT_SYMBOL(BpGetAFEVR5P3PwrEnGpio);
EXPORT_SYMBOL(BpGetUart2SdoutGpio);
EXPORT_SYMBOL(BpGetUart2SdinGpio);
EXPORT_SYMBOL(BpGet6829PortInfo);
EXPORT_SYMBOL(BpGetEthSpdLedGpio);
EXPORT_SYMBOL(BpGetLaserDisGpio);
EXPORT_SYMBOL(BpGetLaserTxPwrEnGpio);
EXPORT_SYMBOL(BpGetVregSel1P2);
EXPORT_SYMBOL(BpGetVregAvsMin);
EXPORT_SYMBOL(BpGetGponOpticsType);
EXPORT_SYMBOL(BpGetDefaultOpticalParams);
EXPORT_SYMBOL(BpGetI2cGpios);
EXPORT_SYMBOL(BpGetMiiOverGpioFlag);
EXPORT_SYMBOL(BpGetSwitchPortMap);
EXPORT_SYMBOL(BpGetMocaInfo);
EXPORT_SYMBOL(BpGetPhyResetGpio);
EXPORT_SYMBOL(BpGetPhyAddr);
EXPORT_SYMBOL(BpGetBatteryEnable);

#if defined (CONFIG_BCM_ENDPOINT_MODULE)
EXPORT_SYMBOL(BpGetVoiceBoardId);
EXPORT_SYMBOL(BpGetVoiceBoardIds);
EXPORT_SYMBOL(BpGetVoiceParms);
#endif

#if defined(CONFIG_EPON_SDK)
EXPORT_SYMBOL(BpGetNumFePorts);
EXPORT_SYMBOL(BpGetNumGePorts);
EXPORT_SYMBOL(BpGetNumVoipPorts);
#endif
#if defined(CONFIG_BCM96838)
EXPORT_SYMBOL(BpGetPonTxEnGpio);
EXPORT_SYMBOL(BpGetPonRxEnGpio);
#endif
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963381) || defined(CONFIG_BCM963148)
EXPORT_SYMBOL(BpGetExtAFELDPwrDslCtl);
EXPORT_SYMBOL(BpGetExtAFELDModeDslCtl);
EXPORT_SYMBOL(BpGetIntAFELDPwrDslCtl);
EXPORT_SYMBOL(BpGetIntAFELDModeDslCtl);
EXPORT_SYMBOL(BpGetIntAFELDDataDslCtl);
EXPORT_SYMBOL(BpGetIntAFELDClkDslCtl);
EXPORT_SYMBOL(BpGetExtAFELDDataDslCtl);
EXPORT_SYMBOL(BpGetExtAFELDClkDslCtl);
EXPORT_SYMBOL(BpGetSgmiiGpios);
#endif

EXPORT_SYMBOL(BpGetOpticalWan);
EXPORT_SYMBOL(BpGetRogueOnuEn);
EXPORT_SYMBOL(BpGetGpioLedSim);
EXPORT_SYMBOL(BpGetGpioLedSimITMS);

#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Ssupport USB LED   
EXPORT_SYMBOL(setUsbDeviceInserted);
#endif
#if defined(CONFIG_BCM96838)
EXPORT_SYMBOL(BpGetTsyncPonUnstableGpio);
EXPORT_SYMBOL(BpGetTsync1ppsPin);
EXPORT_SYMBOL(BpGetPmdAlarmExtIntr);
EXPORT_SYMBOL(BpGetPmdAlarmExtIntrGpio);
EXPORT_SYMBOL(BpGetPmdMACEwakeEn);
EXPORT_SYMBOL(BpGetGpioPmdReset);

EXPORT_SYMBOL(BpGetTrplxrTxFailExtIntr);
EXPORT_SYMBOL(BpGetTrplxrTxFailExtIntrGpio);
EXPORT_SYMBOL(BpGetTrplxrSdExtIntr);
EXPORT_SYMBOL(BpGetTrplxrSdExtIntrGpio);
EXPORT_SYMBOL(BpGetTxLaserOnOutN);
EXPORT_SYMBOL(BpGet1ppsStableGpio);
EXPORT_SYMBOL(BpGetLteResetGpio);
EXPORT_SYMBOL(BpGetStrapTxEnGpio);
EXPORT_SYMBOL(BpGetWifiOnOffExtIntr);
EXPORT_SYMBOL(BpGetWifiOnOffExtIntrGpio);
EXPORT_SYMBOL(BpGetLteExtIntr);
EXPORT_SYMBOL(BpGetLteExtIntrGpio);

#endif

EXPORT_SYMBOL(BpGetAttachedInfo);

MODULE_LICENSE("GPL");
