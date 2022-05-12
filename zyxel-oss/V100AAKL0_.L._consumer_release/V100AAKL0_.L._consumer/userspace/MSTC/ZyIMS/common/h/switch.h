
/*_____ SLIC TYPE _________________________________________*/
#define SLIC_NONE	0
#define SLIC_SI3210	1
#define SLIC_SI3220	2
#define SLIC_SI3215	3
#define SLIC_PEF4268 4

/*_____DECT TYPE_________________________________________*/
#define DECT_NONE	0
#define WB_3519		1
#ifndef SUPPORT_DECT
#define SUPPORT_DECT				DECT_NONE
#endif
#ifndef PH_DECT_NUM_CHANNELS
#define PH_DECT_NUM_CHANNELS		0
#endif

/*_____ Voice DSP Type _________________________________________*/
#define VDSP_NONE	0
#define VDSP_TI5402	1
#define VDSP_TI5409	2
#define VDSP_VP1001	3
#define VDSP_TI921  4
#define VDSP_PMC4200 5
#define VDSP_DANUBE 6
#define VDSP_BRCM6358 7

/*_____ DAA Type _________________________________________*/
#define DAA_NONE	0
#define DAA_SI3019	1
#define DAA_SI3011	2

/*_____ Caller ID Type _________________________________________*/
#define Polarity_Reverse	0
#define First_Ring			1

/*_____ Caller ID Payload Type _________________________________________*/
#define CID_DTMF			0
#define CID_FSK				1

/*_____ Trunk Definition ____________________________________*/
#define TRUNK340			0
#define TRUNK350			1
#define TRUNK350NEW		2
#define TRUNK370			3
#define TRUNK_DEFINE		TRUNK370

/* Eric Chen, merge from Jacky */
#define LONG_DEFINED

/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/
#define VOICEAPP_ICF_MAX_REREG_REC	10
#define VOICEAPP_ICF_MAX_ERR_REC	15
#define VOICEAPP_ICF_MAX_DNS_REC	5

/*                      VoIP                     */
		#define VOIP		   	1
		#define VOIP_NEW      1
		#define RTP 		   	1
		#define VOIP_SIP	   	1
	#ifdef VOIP_SIP
		#define    USER_AGENT_HDR  ""
	#endif
		#define MULTI_SIP_SESSION	1
		#define SIP_OVER_TCP_SUPPORT	1
		#define CALLHISTORY_SUPPORT 1
		#define CENTRALIZE_LOG 1
#ifdef MSTC_VOICE_KSOCKET_RTP
                #define KSOCKET_RTP_SUPPORT 1
#else
		#define KSOCKET_RTP_SUPPORT 0
#endif
		#define KSOCKET_RTP_ENABLE 1
		/*support for Blind-Transfer, Consult-Transfer*/
		#define ZYNOS_CALL_TRANSFER 1 
	#if 1 /*Jason , syslog*/	
		#define VOIP_SYSLOG	1
	#endif
		#define TERMINAL_PORTABILITY 1
#ifdef MSTC_VOICE_SUPPORT_SRTP		
		#define SRTP_SUPPORT 1
#else
		#define SRTP_SUPPORT 0
#endif
#ifdef SUPPORT_412_VOICE
		#define SUPPORT_412 1
#else
		#define SUPPORT_412 0
#endif

//		#define MISSED_CALL_EMAIL_NOTIFICATION 1
//		#define VLAN_TAG_VOIP	1
//		#define VOIP_TE_TEST	1
//		#define VOIP_IAD_DEBUG 1
//#define STUN_SUPPORT		1	 
//		#define ENABLE_CACHE	1
//		#define EARLY_MEDIA 1
//		#define DEFAULT_EARLYMEDIA_RINGTONE	1
		#define EARLY_MEDIA_NO_CHANGE_SDP	1
//		#define HOWLER_TONE 1
		#define EMBEDDED_IVR 1
		/*#define PSTN_PHONEBOOK 1 */
//	#ifdef EARLY_MEDIA
//		#define IVR 			   1
//	#endif		 
//	#ifdef EMBEDDED_IVR
//		#define IVR 			   1
//	#endif		 
		
		/* The following definition must be reference through TR104*/
		/*#define FXO	0*/
		/*#define FXO_DSPRELAY	 0*/
		/*#define PH_SLIC_NUM_CHANNELS    	1*/
		/*#define PH_FXS_NUM_CHANNELS			PH_SLIC_NUM_CHANNELS*/
		/*#define PH_FXO_NUM_CHANNELS			0*/
		/*#define PH_TOTAL_NUM_CHANNELS		( PH_FXS_NUM_CHANNELS + PH_FXO_NUM_CHANNELS)*/
/* Eric Chen, for fxo temp */
#if 1
		/*#define DBG_NUMBERLING_PLAN 1*/

		#define SUPPORT_SLIC				SLIC_NONE
		#define SUPPORT_DAA				DAA_NONE
		#define SUPPORT_VDSP				VDSP_BRCM6358
#else
		#define SUPPORT_SLIC				SLIC_PEF4268
		#define SUPPORT_DAA				DAA_SI3019
		#define SUPPORT_VDSP				VDSP_DANUBE
#endif
		#define CALLERID_TYPE				First_Ring
		#define CALLERID_PAYLOAD			CID_FSK
		#define STOP_RING_BY_FW				1

        	#define VOIPSIPACCOUNT          		2
		#define VOIP_PHASEII	1
		#define CALL_TRANSFER	1
//		#define CALL_FORWARD	1
//		#define THREE_WAY_CALL	1
		#define CALL_CONFERENCE	1

		#define CODEC_THIRD_SELECTION		1
//		#define AUTOPRO					1
//		#define TFTP_CLNT					1		/* TFTP Client Support */
//		#define HTTP_AUTO_PROVISION		1
//		#define AUTOPRO_PINCODE	1
//		#define SPTGEN						1		/* SPTGEN support*/
		#define DIAL_PLAN          			1      	 /* for DialPlan Support */
		#ifdef IPTK_REL_8_0
		#define HEARTBEAT_MECHANISM_ENABLE 	1	/* 1:on , 0:off */
		#endif
		#define SPEED_DIAL          				1      	 /* for Speed dial/Phone book Support */
		#define CALL_RETURN_SUPPORT	1/* Call return feature used with phone config */
		#define DO_NOT_DISTURB 1              /*Do not disturb support*/
		#define CCBS_SUPPORT 1
//		#define SMTP_AUTH	1
//		#define LIFELINE_AUTODETECT 		1		/* LifeLine Auto chcek */
//		#define SUPPORT_COUNTRY_CODE    	1
//		#define CALL_PARK_PICKUP	1
//		#define CALL_RETURN_SUPPORT	1/* Call return feature used with phone config */
//		#define MUSIC_ON_HOLD	1
//		#define VOIP_ROUTE_SELECT	1
//      	#define VDSP_DETECT_DIAL_DIGITS     1	
//		#define DONT_REINVITE_AFTER_REBOOT_FIX	1
//		#define IVRCHECKCODEC_BUGFIX	1
//	#ifndef ZYXM
//		#define ZYXM			-1
//	#endif

/* Eric Chen */	
#if 0
	/* For New GUI */ // penpen
		#define DAYLIGHT_SAVING_ENHANCEMENT 1
		//#define TRIGGER_PORT			1			/* Support Web Trigger port forwarding */
		#define IDNS				1				/* Integrated DNS For NewGUI Only*/	
		#define STDIO_TIMEOUT			1			/* Support WebGUI configure IdleTimeOut timer*/
		#define NAT_ALG					1			/* Support ALG pages for new GUI */
		#define VOIP_STATISTICS			1			/* Support VoIP Statistic */
	/* For New GUI End */

 		#define DSLFORUM_CFG		1  
        	
        	#ifdef DSLFORUM_CFG
	        	#define TR069				1
	        #endif
	        
	        #if defined(TR069) && defined(VOIP)
	        	#define TR104				1
	        #endif

#endif
