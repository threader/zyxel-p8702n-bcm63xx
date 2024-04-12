
#ifndef __FAP4KE_BPM_H_INCLUDED__
#define __FAP4KE_BPM_H_INCLUDED__

/************************************************************
 *
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
 ************************************************************/

/*
 *******************************************************************************
 * File Name  : fap4ke_bpm.h
 *
 *******************************************************************************
 */

//#define CC_FAP4KE_BPM_DEBUG
//#define CC_FAP4KE_BPM_DEBUG2
//#define CC_FAP4KE_BPM_DEBUG3

#if defined(CC_FAP4KE_BPM_DEBUG)
#define FAP4KE_BPM_ASSERT(condition) fap4kePrt_Assert(condition)
#define FAP4KE_BPM_DEBUG(fmt, arg...) fap4kePrt_Debug(fmt, ##arg)
#define FAP4KE_BPM_INFO(fmt, arg...) fap4kePrt_Info(fmt, ##arg)
#define FAP4KE_BPM_NOTICE(fmt, arg...) fap4kePrt_Notice(fmt, ##arg)
#define FAP4KE_BPM_PRINT(fmt, arg...) fap4kePrt_Print(fmt, ##arg)
#define FAP4KE_BPM_ERROR(fmt, arg...) fap4kePrt_Error(fmt, ##arg)
#define FAP4KE_BPM_DUMP_PACKET(_packet_p, _length) dumpHeader(_packet_p)
#else
#define FAP4KE_BPM_ASSERT(condition)
#define FAP4KE_BPM_DEBUG(fmt, arg...)
#define FAP4KE_BPM_INFO(fmt, arg...)
#define FAP4KE_BPM_NOTICE(fmt, arg...)
#define FAP4KE_BPM_PRINT(fmt, arg...)
#define FAP4KE_BPM_ERROR(fmt, arg...)
#define FAP4KE_BPM_DUMP_PACKET(_packet_p, _length)
#endif

//#define CC_FAP4KE_BPM_PMON

#if defined(CC_FAP4KE_BPM_PMON)
#define FAP4KE_BPM_PMON_DECLARE() FAP4KE_PMON_DECLARE()
#define FAP4KE_BPM_PMON_BEGIN(_pmonId) FAP4KE_PMON_BEGIN(_pmonId)
#define FAP4KE_BPM_PMON_END(_pmonId) FAP4KE_PMON_END(_pmonId)
#else
#define FAP4KE_BPM_PMON_DECLARE()
#define FAP4KE_BPM_PMON_BEGIN(_pmonId)
#define FAP4KE_BPM_PMON_END(_pmonId)
#endif

//#define CC_FAP4KE_BPM_TRACE

#if defined(CC_FAP4KE_BPM_TRACE)
#define FAP4KE_BPM_TRACE(_id, _arg, _type) fap4keTrace_record(_id, _arg, _type)
#else
#define FAP4KE_BPM_TRACE(_id, _arg, _type)
#endif


/*----- Defines -----*/
#define FAP_BPM_ERROR               (-1)
#define FAP_BPM_SUCCESS             0

void enetBpmStatus(void);
void enetBpmSetTxQThresh( int channel, uint16 *txDropThr );
void enetBpmSetRxThresh(int channel, uint16 allocTrig, uint16 bulkAlloc);

void xtmBpmStatus(void);
void xtmBpmSetTxQThresh( int channel, uint16 loThresh, uint16 hiThresh );
void xtmBpmSetRxThresh( int channel, uint16 allocTrig, uint16 bulkAlloc );

void fap4keBpm_dumpStatus(void);
void fap4keBpm_dumpTxQThresh(void);
void fap4keBpm_dumpEnetTxQThresh(void);

void fap4keBpm_allocBuf_Dqm( uint8 drv, uint8 channel, 
            uint8 seqId, uint16 numBufs);
void fap4keBpm_freeBuf_Dqm(uint8 seqId, uint16 numBufs);

void fap4keBpm_BufCacheResetAllocResp_wait(void);
int  fap4keBpm_BufCacheAllocResp( uint32 channel, uint32 seqId, uint32 numBufs);
void fap4keBpm_FblFreeResp( uint32 seqId );
void fap4keBpm_init( void );

#endif /*  __FAP4KE_BPM_H_INCLUDED__ */
