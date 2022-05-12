/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, dIsclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/
#ifndef _IVR_COMMON_H
#define _IVR_COMMON_H


#define	NUM_IVRSYS			20
#define	NUM_IVRUSR			5
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define	NUM_IVRDEFEM			4				/* number of default early media */
#endif
#define 	NUM_IVR_UPGRADE	10
#define	IVRSYS_OFFSET		50		
/* IVRSYS_OFFSET means the offset by ivrsys number needs to add into the logical number.*/

#ifdef IVRCHECKCODEC_BUGFIX
#define   CODEC_ERROR	255
#endif

#define	Max_Voice_Elements				((NUM_IVRSYS > NUM_IVRUSR) ? NUM_IVRSYS : NUM_IVRUSR)
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define	Max_Single_Element_Size		(60*1024)
#else
#define	Max_Single_Element_Size		(180*1024)
#endif
#define	Max_Assem_Voice				(50*1024)
#define	LEN_VOICE_HEADER				sizeof(VoiceHeader_t)
#define RTP_FIXED_HEADER_LEN	12 // eason
#define	LEN_VOICE_ELEMENT_PRE		(4 + RTP_FIXED_HEADER_LEN)

#define	TYPE_IVR_SYS_DEFAULT			0
#define	TYPE_IVR_USR_DEFINED			1
#define	TYPE_IVR_UPGRADE_DEFINED		2
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define 	TYPE_IVR_EARLYMEDIA_DEFINED		3
#endif

#define ZYXEL_IVR_MAX_LEN (512 * 1024)  /* same as bcmdrivers/opensource/include/bcm963xx/board.h */

#define MAX_IVR_USR_BUF_SIZE (LEN_VOICE_HEADER+Max_Single_Element_Size*2)

#define MAX_IVR_SYS_BUF_SIZE (ZYXEL_IVR_MAX_LEN - MAX_IVR_USR_BUF_SIZE)

#define IVR_SYS_FLASH_OFFSET MAX_IVR_USR_BUF_SIZE
//#define MAX_IVR_SYS_BUF_SIZE LEN_VOICE_HEADER+Max_Single_Element_Size*NUM_IVRSYS



/* type =TYPE_IVR_SYS_DEFAULT definition */
/* NOTE: don't move 0~9 definition. */
#define	IVRPRON_NUMBER_0				0
#define	IVRPRON_NUMBER_1				1
#define	IVRPRON_NUMBER_2				2
#define	IVRPRON_NUMBER_3				3
#define	IVRPRON_NUMBER_4				4
#define	IVRPRON_NUMBER_5				5
#define	IVRPRON_NUMBER_6				6
#define	IVRPRON_NUMBER_7				7
#define	IVRPRON_NUMBER_8				8
#define	IVRPRON_NUMBER_9				9
#define	IVRPRON_DOT					10
#define	IVRPRON_MAIN_PROMPT			11
#define	IVRPRON_INVALID				12
#define	IVRPRON_USEPSTN				13
#define	IVRPRON_USEVOIP				14
#define	IVRPRON_DEF_RINGBACKTONE  	15
#define	IVRPRON_HOWLER_TONE			16

#if 0
#define	IVRPRON_PHONE_CONFIG_SUCCESS	16
#define	IVRPRON_PHONE_CONFIG_FAIL  	17
#endif
#define IVRPRON_BUSYTONE  17
#define IVRPRON_ENTERPASWWORD 18

#define	IVRPRON_PHONE_CONFIG_SUCCESS	19 //Temp
#define	IVRPRON_PHONE_CONFIG_FAIL  	       20 //Temp



#define	IVR_G711_SIZE	160
#define	IVR_G711_TIME	20
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define	IVR_G711_TIMESTAMP	160
#endif
#define	IVR_G723_SIZE	20
#define	IVR_G723_TIME	30
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define	IVR_G723_TIMESTAMP	240
#endif
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define	IVR_G726_SIZE	80
#else
#define	IVR_G726_SIZE	20
#endif
#define	IVR_G726_TIME	20
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define	IVR_G726_TIMESTAMP	160
#endif
#define	IVR_G729_SIZE	20
#define	IVR_G729_TIME	20
#ifdef DEFAULT_EARLYMEDIA_RINGTONE
#define	IVR_G729_TIMESTAMP	160
#endif


/* type =TYPE_IVR_USER_DEFAULT definition */
#define	IVR_USER_ONHOLD				0

/*	Voice Data
**	
**	IVRPRON_MAIN_PROMPT: 	ZyXEL configuration menu, please enter option followed by the pound key or hang on to exit.
**	IVRPRON_INVALID: 		Invalid option.
**	IVRPRON_USEPSTN:		You are using PSTN call now.
**	IVRPRON_USEVOIP:		You are using VOIP call now.
**	IVRPRON_DEF_RINGBACKTONE:		record default romote ringback tone, used for early media
*/

#define IVR_FUNC_NONE				0x0
#define IVR_FUNC_RECORD			0x1
#define IVR_FUNC_PLAY				0x2
#define IVR_FUNC_DEL_VOICE			0x3
#define IVR_FUNC_DEL_ALL			0x4
#define IVR_FUNC_UPGRADE_ALARM	0x5
#define IVR_FUNC_UNKNOWN			0x9

#ifdef DEFAULT_EARLYMEDIA_RINGTONE
/* IVR Task events */
#define	IVR_PLAY_START				EVENT( EVT_VOIP_IVR, 0x01)
#define	IVR_MULTIPLAY_START			EVENT( EVT_VOIP_IVR, 0x02)
#define	IVR_DELETE_CHANNEL			EVENT( EVT_VOIP_IVR, 0x03)
#endif

/* debug message */
extern int ivrDebug;
#define IVR_FSM_DBG(fs)				if (ivrDebug & 1) {	dbg_pline_1((fs));			\
													dbg_pline_1("\n\r"); }
#define IVR_FSM_DBG1(fs, v1)			if (ivrDebug & 1) {	dbg_plinel_1((fs), (v1));	\
													dbg_pline_1("\n\r"); }
#define IVR_FSM_DBG2(fs, v1, f2, v2)	if (ivrDebug & 1) {	dbg_plinel_1((fs), (v1));	\
													dbg_plinel_1((f2), (v2));	\
													dbg_pline_1("\n\r"); }
#define IVR_FUNC_ACTION_STOP 0
#define IVR_FUNC_ACTION_START 1
#define IVR_FUNC_ACTION_STOP_SAVE 2

#endif

