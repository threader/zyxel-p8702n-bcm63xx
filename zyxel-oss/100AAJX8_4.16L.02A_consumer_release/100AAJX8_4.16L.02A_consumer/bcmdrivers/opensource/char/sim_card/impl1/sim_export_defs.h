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
#ifndef _SIM_EXPORT_DEFS_H
#define _SIM_EXPORT_DEFS_H


#include "simio_atr.h"

/* open/close debug prints*/
#define DEBUG_SIM_CARDI 0

#define SIM_CARD_DONT_CARE 0xff
#define OSSUSPEND_PRIORITY SIM_CARD_DONT_CARE 
#define LOGID_SIM BCM_LOG_ID_SIM_CARD
#undef NULL
#define NULL 0

#define PADCTRL_SIMDAT_PULLUP_4p5kOHM ((1<<5)|(0<<12)|(0<<11)|(0<<6))
#define PADCTRL_SIMDAT_PULLUP_7p5kOHM ((1<<5)|(0<<12)|(1<<11)|(0<<6))
#define PMU_DRV_IsSIMReady(a) a
#define PMU_DRV_ActivateSIM(a, b) 
#define PMUXDRV_Config_Pad_Resistor(...);
#define _base(x) ((x >= '0' && x <= '9') ? '0' : \
    (x >= 'a' && x <= 'f') ? 'a' - 10 : \
    (x >= 'A' && x <= 'F') ? 'A' - 10 : \
    '\255')
#define HEXOF(x) (x - _base(x))

#define NO_SIM_RET()       \
    do {                   \
        if (!sim_online()) \
        {                  \
            printk (KERN_EMERG "smart card err: there isn't a sim card connected. Please insert one.\n"); \
            return SIM_CARD_NO_SIM_CARD_PRESENT;     \
        }                  \
    } while(0)

typedef enum 
{
    SIMLDO1,
    SIMLDO2
} PMU_SIMLDO_t;

typedef enum {
    PMU_SIM3P0Volt = 0,
    PMU_SIM1P8Volt,
    PMU_SIM0P0Volt
} PMU_SIMVolt_t;

#define MIN(a,b)        (((a)<(b))?(a):(b))
#define IPRIORITY_MIDDLE 1
#define SIM_IRQ INTERRUPT_ID_USIM
#define SIM2_IRQ INTERRUPT_ID_USIM

#if DEBUG_SIM_CARDI
#define OSTASK_Sleep(time) {printk (KERN_EMERG "smart card err: trying to sleep %s:%d\n",__FILE__, __LINE__ ); msleep(time);}
#define Log_DebugPrintf(a,s,v1,v2) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %s %d\n",s, v1, (int)v2)
#define dprintf(a,s) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, s)
#define chal_dprintf(a,s1...) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s\n", #s1)
#define SIM_LOG(b) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, b)
#define SIM_LOGV(s,v1)  BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %d\n",s, (int)v1)
#define SIM_LOGV2(s,v1,v2)  BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %d %d\n",s, (int)v1, (int)v2)
#define SIM_LOGV4(s,v1,v2,v3,v4)  BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %d %d %d %d\n",s, (int)v1, (int)v2, (int)v3, (int)v4)
#define SIM_LOGV5(s,v1,v2,v3,v4,v5) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %d %d %d %d %d\n",s, (int)v1, (int)v2, (int)v3, (int)v4, (int)v5)
#define SIM_LOGV6(s,v1,v2,v3,v4,v5,v6) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %d %d %d %d %d %d\n",s, (int)v1, (int)v2, (int)v3, (int)v4, (int)v5, (int)v6)
#define SIM_LOGV7(s,v1,v2,v3,v4,v5,v6,v7) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %d %d %d %d %d %d %d\n",s, (int)v1, (int)v2, (int)v3, (int)v4, (int)v5, (int)v6, (int)v7)
#define CAL_LOG(s) SIM_LOG(s)
#define CAL_LOGV(s,v1) SIM_LOGV(s,v1)
#define CAL_LOGV4(s,v1,v2,v3,v4) SIM_LOGV4(s,v1,v2,v3,v4)
#define SIM_LOG_ARRAY(s1, s2, v1) BCM_LOG_DEBUG(BCM_LOG_ID_SIM_CARD, "%s %s %d\n",s1, s2, (int)v1)
#define _TP_OUTPUT(...);
#else
#define OSTASK_Sleep(time) msleep(time)
#define Log_DebugPrintf(a,s,v1,v2)
#define dprintf(a,s)
#define chal_dprintf(a,s1...)
#define SIM_LOG(b)
#define SIM_LOGV(s,v1)
#define SIM_LOGV2(s,v1,v2)
#define SIM_LOGV4(s,v1,v2,v3,v4)
#define SIM_LOGV5(s,v1,v2,v3,v4,v5)
#define SIM_LOGV6(s,v1,v2,v3,v4,v5,v6)
#define SIM_LOGV7(s,v1,v2,v3,v4,v5,v6,v7)
#define CAL_LOG(s)
#define CAL_LOGV(s,v1)
#define CAL_LOGV4(s,v1,v2,v3,v4)
#define SIM_LOG_ARRAY(s1, s2, v1)
#define _TP_OUTPUT(...);
#endif

/*these correspond to a service which is used to allow/disallow low power mode. It is typically disabled when a SIM transaction is ongoing*/
#define SLEEP_AllocId() SIM_CARD_DONT_CARE
#define SLEEP_DisableDeepSleep(a) 
#define SLEEP_EnableDeepSleep(s)

#define OSDAL_SYSMAP_Get_BaseAddr(a) 0xb4e00700
#define USIM_Control_Register_Address 0xb4e00170

#define OSDAL_SYSMAP_SIM SIM_CARD_DONT_CARE
#define OSDAL_SYSMAP_SIM2 SIM_CARD_DONT_CARE

#define TICKS_ONE_SECOND	((Ticks_t)1000)

#define __RAND_SIM_NAME(x,a) a##_sim_##x
#define _RAND_SIM_NAME(x,a) __RAND_SIM_NAME(x,a)
#define RAND_SIM_NAME(a) _RAND_SIM_NAME(__LINE__,a)

#define TASKNAME_SIMIO "SIMIO"
typedef struct task_struct *Task_t;

#define OSTASK_Create(func,name,c,d) ({Task_t ret=kthread_create(func, NULL, name); wake_up_process(ret); ret;})
        
#define OSTASK_Suspend(name)
#define OSTASK_Destroy(name)

typedef UInt8 *TName_t;
typedef void* CHAL_HANDLE; 
typedef struct mutex *Semaphore_t;
typedef struct timer_list *Timer_t;
typedef Timer_t Interrupt_t;
typedef int CLIENT_ID;
typedef int (*TEntry_t)( void *);
typedef FN_HANDLER (*isr_t)(int irq, void *sim_card_prm);

typedef UInt32 TimerID_t;
typedef void (* TimerEntry_t)( TimerID_t id );
typedef TimerEntry_t IEntry_t;
typedef UINT32 Ticks_t;
typedef int InterruptId_t;

#define	assert(expression) {if (!(expression)) {(void) panic("assertion \"%s\" failed: file \"%s\", line %d\n", \
#expression, __FILE__, __LINE__); } }

#define __nop()

#define IRQ_Clear(a) chal_simio_read_intr_status(dev->chal_handle)
#define IRQ_Register(irq,isr) BcmHalMapInterrupt((FN_HANDLER)isr, NULL, irq)
#define IRQ_Disable(irq) BcmHalInterruptDisable(irq)
#define IRQ_Enable(irq) BcmHalInterruptEnable(irq)
#define IRQ_IsEnabled(a) a

#define TIMER_GetAccuValue() jiffies 
#define TIMER_GetValue() jiffies 

#define OSSEMAPHORE_Create(a,b) ({static DEFINE_MUTEX(RAND_SIM_NAME(mutex)); mutex_lock(&RAND_SIM_NAME(mutex)); &RAND_SIM_NAME(mutex);})
#define	OSSEMAPHORE_Destroy(s) mutex_destroy(s)
#define OSSEMAPHORE_Release(s) mutex_unlock(s)
#define OSSEMAPHORE_ChangeName(s,f)
#define OSSEMAPHORE_Obtain(a,b) ({mutex_lock(a); OSSEMAPHORE_RESULT_SUCCESS;})
#define OSSEMAPHORE_GetCnt(s) mutex_is_locked(s)

#define TICKS_FOREVER ((Ticks_t)0xFFFFFFFF)


#define	OSTIMER_Destroy(timer) del_timer(timer)
#define OSTIMER_Start(timer) mod_timer(timer, jiffies + msecs_to_jiffies(timer->data))
#define OSTIMER_Create(func,b,time,d) ({static struct timer_list RAND_SIM_NAME(timer); setup_timer(&RAND_SIM_NAME(timer), func, NULL); RAND_SIM_NAME(timer).data = time; (Timer_t)&RAND_SIM_NAME(timer);})
#define OSTIMER_Reset(s)
#define OSTIMER_Stop(s) del_timer(s)


#define OSINTERRUPT_Create(func,b,c,d) OSTIMER_Create(func,b,c,d)
#define	OSINTERRUPT_Destroy(t)
#define OSINTERRUPT_Trigger(timer) mod_timer(timer, jiffies)

/*you can ignore these. pedestal was on ARM11 to support bus clock gating*/
#define PEDESTAL_AllocId() SIM_CARD_DONT_CARE
#define PEDESTAL_DisablePedestalMode(a)
#define PEDESTAL_EnablePedestalMode(s)

typedef struct 
{
    SIMIO_ID_t id;
}sim_card_info;

typedef enum
{
    SIMIO_CLK_4P16MHZ,   // SIM interface clock 25MHz, SIM clock 4.16MHz
    SIMIO_CLK_3P12HZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_2P5MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_2P08MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_1P78MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_1P5MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_1P3MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_1P25MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_1P13MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_1P04MHZ,   // SIM interface clock 25MHz, SIM clock 
    SIMIO_CLK_LAST,
} SIMIO_DIVISOR_t;

enum
{
    RESOURCE_SIM,
    RESOURCE_SIM2,
};

enum
{
    SIMDAT_PAD,
    SSPDO_PAD,
};

enum
{
    SIM_CARD_DATA_IS_INVALID = -1,
    SIM_CARD_INPUT_IS_INVALID = -2,
    SIM_CARD_NO_SIM_CARD_PRESENT = -3,
    SIM_CARD_CONNECTION_GOT_LOST_AND_RECOVERED = -4,
    SIM_CARD_CONNECTION_GOT_LOST_AND_DIDNT_RECOVER = -5,
    SIM_CARD_INPUTER_BUFFER_IS_TOO_SHORT = -6,
    SIM_CARD_NO_OUTPUT_DATA_FROM_SIM_CARD = -7,
    SIM_CARD_UNSUCCESSFUL_PPS_EXCHANGE = -8,
    SIM_CARD_INTERNAL_ERROR = -9,
} ;

#define SIM_CARD_MAX_BUFFER_SIZE        300
#define PTSS_CHAR                       0xFF

/* Format character in PTS request, see Section 7.4 of ISO/IEC 7816-3 */
#define PTS0_INDICATE_NO_PTS1           0x00
#define PTS0_INDICATE_PTS1_EXIST        0x10
#define PTS0_INDICATE_PTS2_EXIST        0x20
#define PTS0_INDICATE_PTS1PTS2_EXIST    0x30

#define PTS0_INDICATE_T1_SUPPORT        0x01            
#define PTS0_INDICATE_T1_NO_PTS1        0x01
#define PTS0_INDICATE_T1_PTS1_EXIST     0x11
#define PTS0_INDICATE_T1_PTS2_EXIST     0x21
#define PTS0_INDICATE_T1_PTS1PTS2_EXIST 0x31

/* Parameter character 1 (PTS1) value, see Section 7.4 of ISO/IEC 7816-3 */
#define PTS1_F512_D8                    0x94
#define PTS1_F512_D16                   0x95
#define PTS1_F512_D32                   0x96
#define PTS1_F512_D64					0x97

/* Parameter character 2 (PTS2) value */
#define PTS2_LIB                        0x90   // Low Impedance Buffer (LIB) support
#define PTS2_UICC_CLF                   0xA0   // UICC-CLF Interface support. 
#define PTS2_NO_INTERFACE               0x00   // Global Interface Not Supported. 

// Max. history character
#define MAX_HIST_CHARACTERS             15
#define TASKPRI_SIMIO 1

static const UInt8 Pts_Req_F372_D1[] = {PTSS_CHAR, PTS0_INDICATE_NO_PTS1, (PTSS_CHAR ^ PTS0_INDICATE_NO_PTS1)};
static const UInt8 Pts_Req_F372_D1_PPS2_LIB[] = {PTSS_CHAR, PTS0_INDICATE_PTS2_EXIST, PTS2_LIB, (PTSS_CHAR^PTS0_INDICATE_PTS2_EXIST^PTS2_LIB)};

static const UInt8 Pts_Req_F512_D8[] = 
{PTSS_CHAR, PTS0_INDICATE_PTS1_EXIST, PTS1_F512_D8, (PTSS_CHAR ^ PTS0_INDICATE_PTS1_EXIST ^ PTS1_F512_D8)};
static const UInt8 Pts_Req_F512_D8_PPS2_LIB[] = 
{PTSS_CHAR, PTS0_INDICATE_PTS1PTS2_EXIST, PTS1_F512_D8, PTS2_LIB, (PTSS_CHAR ^ PTS0_INDICATE_PTS1PTS2_EXIST ^ PTS2_LIB ^ PTS1_F512_D8)};

static const UInt8 Pts_Req_F512_D16[] = 
{PTSS_CHAR, PTS0_INDICATE_PTS1_EXIST, PTS1_F512_D16, (PTSS_CHAR ^ PTS0_INDICATE_PTS1_EXIST ^ PTS1_F512_D16)};
static const UInt8 Pts_Req_F512_D16_PPS2_LIB[] = 
{PTSS_CHAR, PTS0_INDICATE_PTS1PTS2_EXIST, PTS1_F512_D16, PTS2_LIB, (PTSS_CHAR ^ PTS0_INDICATE_PTS1PTS2_EXIST ^ PTS2_LIB ^ PTS1_F512_D16)};

static const UInt8 Pts_Req_F512_D32[] = 
{PTSS_CHAR, PTS0_INDICATE_PTS1_EXIST, PTS1_F512_D32, (PTSS_CHAR ^ PTS0_INDICATE_PTS1_EXIST ^ PTS1_F512_D32)};
static const UInt8 Pts_Req_F512_D32_PPS2_LIB[] = 
{PTSS_CHAR, PTS0_INDICATE_PTS1PTS2_EXIST, PTS1_F512_D32, PTS2_LIB, (PTSS_CHAR ^ PTS0_INDICATE_PTS1PTS2_EXIST ^ PTS2_LIB ^ PTS1_F512_D32)};

int SIMIO_set_frequency(SIMIO_ID_t id, SIMIO_DIVISOR_t freq);
int SIMIO_set_pps(SIMIO_ID_t id, CHAL_SIMIO_SPEED_t val, int divisor);
int SIMIO_command(SIMIO_ID_t id, UInt16 val, UInt8 *data, int len);
int SIMIO_activeCard(SIMIO_ID_t id);
int SIMIO_read_detection_status(SIMIO_ID_t id, int *detection_status);
int SIMIO_reset_sim(SIMIO_ID_t id, UInt8 *buf, int len);

int simio_activesim(SIMIO_ID_t id, UInt8 *data, int len);
int sim_card_control(int control);
int sim_card_reset(int reset_mode, SimVoltageLevel_t activation_voltage, SIMIO_DIVISOR_t frequency);
int sim_set_baud(int F, int D);
int sim_card_command(UInt16 command, UInt8 *data, int len);
int sim_card_protocol(PROTOCOL_t protocol);
int sim_online(void);
int sim_active(UInt8 *data, int len);
int sim_read(UInt8 *data, int len);
int sim_write(UInt8 *data, int len);


void simio_user_register(void);

#endif /*_SIM_EXPORT_DEFS_H*/




