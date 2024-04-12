/*
    Copyright 2000-2010 Broadcom Corporation

    <:label-BRCM:2011:DUAL/GPL:standard
    
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

#include "boardparms.h"

/* Typedefs */

/*
You can add a new enum entry anywhere in the enum list below.
Then, you can use the enums in the board parm arrays, with the following restrictions:
-An API must be created for accessing any data from the array except for bp_elemTemplate.
-bp_cpBoardId  must be the first element of the array.
-bp_elemTemplate if used should be the last or second to last (just before bp_last) element
 of the array
-Most enums that are used only once in the array can be placed anywhere after bp_cpBoardId.
-These enums are read with the BpGetUc/BpGetUs/BpGetUl private functions.
-If there is a board parm array that is very similar to one you are adding, then you
 can use the bp_elemTemplate to point to that the board parm array BUT the restriction
 below still applies - that is do not split up the "groups"
 (see g_bcm96362advngr and g_bcm96362advngr2 or g_bcm963168vx and g_bcm963168vx_p300)
-Certain enums can appear multiple times in the board parm arrays
-These enums have special APIs that understand how to loop through each occurence
 They are:
 *packet switch related parameters (from bp_ucPhyAddress to bp_ulPhyId7) which can
  appear once per packet switch (bp_ucPhyType0 and bp_ucPhyType1)
 *led info related parameters (bp_usDuplexLed, bp_usSpeedLed100 and bp_usSpeedLed1000) which can
  appear once per internal led (bp_ulPhyId0 and bp_ulPhyId1)
 *add a new id bp_usPhyConnType to specify the phy connection type as we are running out phy id flag
 *bits. use this id initially for PLC MAC connection type, but will use it for MOCA, EPON, GPON in the
 *futures and remove the phy id connecton type flags.
 *voip dsp related parameters (from bp_ucDspAddress to bp_usGpioDectLed) which can
  appear once per dsp (bp_ucDspType0 and bp_ucDspType1)
*/

#define bp_usGpioOverlay     bp_ulGpioOverlay

enum bp_id {
  bp_cpBoardId,
  bp_cpComment,
  bp_ulGpioOverlay,
  bp_usGpioLedAdsl,
  bp_usGpioLedAdslFail,
  bp_usGpioSecLedAdsl,
  bp_usGpioSecLedAdslFail,
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support VDSL LED
  bp_usGpioLedVdsl,
  bp_usGpioLedVdslFail,
  bp_usGpioSecLedVdsl,
  bp_usGpioSecLedVdslFail,
#endif    
  bp_usGpioLedSesWireless,
  bp_usGpioLedWanData,
#ifdef MSTC_MODIFY // __MSTC__, Richard Huang
  bp_usGpioLedInternetData,
  bp_usGpioLedWirelessFail,
  bp_usGpioLedTV,
  bp_usGpioLedGPHY,
#endif    
  bp_usGpioSecLedWanData,
  bp_usGpioLedWanError,
  bp_usGpioLedBlPowerOn,
  bp_usGpioLedBlStop,
  bp_usGpioFpgaReset,
  bp_usGpioLedGpon,
  bp_usGpioLedGponFail,
  bp_usGpioLedMoCA,
  bp_usGpioLedMoCAFail,
#ifdef MSTC_MODIFY //__MSTC__, RaynorChung: Support USB LED
  bp_usGpioLedUSB0,
  bp_usGpioLedUSB0Fail,
  bp_usGpioLedUSB1,
  bp_usGpioLedUSB1Fail,
#endif
#ifdef MSTC_MODIFY //__MSTC__, Paul Ho, Wireless On/Off Button
  bp_usGpioWirelessBtn,
  bp_usGpioLedETH0,
  bp_usGpioLedETH1,
  bp_usGpioLedETH2,
  bp_usGpioLedETH3,  
#endif  
  bp_usGpioLedEpon,
  bp_usGpioLedEponFail,
  bp_usExtIntrResetToDefault,
  bp_usExtIntrResetToDefault2,
#ifdef MSTC_MODIFY //__MSTC__, Eason, WLAN_WPS_BTN_SEPARATE
  bp_usExtIntrWlanEnableBtnWireless,
#endif    
  bp_usExtIntrSesBtnWireless,
  bp_usAntInUseWireless,
  bp_usWirelessFlags,
  bp_usGpioWirelessPowerDown,
  bp_ucPhyType0,
  bp_ucPhyType1,
  bp_ucPhyAddress,
  bp_usConfigType,
  bp_ulPortMap,
  bp_ulPhyId0,
  bp_ulPhyId1,
  bp_ulPhyId2,
  bp_ulPhyId3,
  bp_ulPhyId4,
  bp_ulPhyId5,
  bp_ulPhyId6,
  bp_ulPhyId7,
  bp_ulPhyId8,
  bp_ulPhyId9,
  bp_ulPhyId10,
  bp_ulPhyId11,
  bp_ulPhyId12,
  bp_ulPhyId13,
  bp_ulPhyId14,
  bp_ulPhyId15,
#ifdef MSTC_MODIFY //__MSTC__  
  bp_usGpioLedEphy0Duplex,
  bp_usGpioLedEphy1Duplex,
  bp_usGpioLedEphy2Duplex,
  bp_usGpioLedGphyDuplex,
  bp_usGpioLedEphy0Act,
  bp_usGpioLedEphy1Act,
  bp_usGpioLedEphy2Act,
  bp_usGpioLedGphyAct,
  bp_usGpioLedEphy0Speed,
  bp_usGpioLedEphy1Speed,
  bp_usGpioLedEphy2Speed,
  bp_usGpioLedGphySpeed,
  bp_usGpioLedDslLink,
#endif  
  bp_usDuplexLed,
  bp_usLinkLed, // Link/Activity
  bp_usSpeedLed100,
  bp_usSpeedLed1000,
  bp_usPhyConnType,
  bp_usGpioPhyReset,    // used if each phy has a different gpio pin for reset
  bp_ucPhyDevName,
  bp_ucDspType0,
  bp_ucDspType1,
  bp_ucDspAddress,
  bp_usGpioLedVoip,
  bp_usGpioVoip1Led,
  bp_usGpioVoip1LedFail,
  bp_usGpioVoip2Led,
  bp_usGpioVoip2LedFail,
#ifdef MSTC_MODIFY //__MSTC__ , MWI LED
  bp_usGpioPhone1MWILed,
  bp_usGpioPhone2MWILed,
  bp_usGpioGWAN100MLed,
  bp_usGpioGWAN1000MLed,
#endif    
  bp_usGpioPotsLed,
  bp_usGpioDectLed,
  bp_usGpioPassDyingGasp,
  bp_ulAfeId0,
  bp_ulAfeId1,
  bp_usGpioExtAFEReset,
  bp_usGpioExtAFELDPwr,
  bp_usGpioExtAFELDMode,
  bp_usGpioIntAFELDPwr,
  bp_usGpioIntAFELDMode,
  bp_usGpioAFELDRelay,
  bp_usGpioAFEVR5P3PwrEn,
  bp_usGpioUart2Sdin,
  bp_usGpioUart2Sdout,
  bp_usGpioLaserDis,
  bp_usGpioLaserTxPwrEn,
  bp_usGpioLaserRxPwrEn,
  bp_usGpioEponOpticalSD,
  bp_usVregSel1P2,
  bp_ucVreg1P8,
  bp_usVregAvsMin,
  bp_usGponOpticsType,
  bp_usGpioFemtoReset,
  bp_cpDefaultOpticalParams,
  bp_usEphyBaseAddress,
  bp_usGpioSpiSlaveReset,  
  bp_usGpioSpiSlaveBootMode,
  bp_usSpiSlaveBusNum,  
  bp_usSpiSlaveSelectNum,
  bp_usSpiSlaveSelectGpioNum,
  bp_usSpiSlaveMode,
  bp_ulSpiSlaveCtrlState,
  bp_ulSpiSlaveMaxFreq,
  bp_usSpiSlaveProtoRev,
  bp_usGpioIntAFELDData,
  bp_usGpioIntAFELDClk,
  bp_usGpioExtAFELDData,
  bp_usGpioExtAFELDClk,
  bp_ulNumFePorts,
  bp_ulNumGePorts,
  bp_ulNumVoipPorts,
  bp_usGpioI2cScl,
  bp_usGpioI2cSda,
  bp_elemTemplate,
  bp_usSerialLEDMuxSel,
#ifdef MSTC_MODIFY //__MSTC__, hpna
  bp_usGpioHpnaReset, 
#endif  
  bp_ulDeviceOptions,
  bp_pPhyInit,
  bp_usGpioLaserReset,  
  bp_usGpio_Intr,
  bp_usGphyBaseAddress,
#ifdef MSTC_MODIFY //__MSTC__  
  bp_usGpioLedWpsFail,
  bp_usGpioLedSysFail,
  bp_usGpioLed5GWps,
  bp_usGpioLed5GWpsFail,
#endif   
  bp_cpPersonalityName,
  bp_usExtIntrMocaHostIntr,
  bp_usExtIntrMocaSBIntr0,
  bp_usExtIntrMocaSBIntr1,
  bp_usExtIntrMocaSBIntr2,
  bp_usExtIntrMocaSBIntr3,
  bp_usExtIntrMocaSBIntr4,
  bp_usExtIntrMocaSBIntrAll,
  bp_usMocaType0,
  bp_usMocaType1,
  bp_pMocaInit,
  bp_ulDyingGaspIntrPin,
  bp_usGpioPLCPwrEn,
  bp_usGpioPLCReset,
  bp_usExtIntrPLCStandBy,
  bp_ulPortFlags,
  bp_usMocaRfBand,
  bp_usGpioLedOpticalLinkFail,
  bp_usGpioLedLan,
  bp_usGpioLedWL2_4GHz,
  bp_usGpioLedWL5GHz, 
  bp_usGpioLedUSB,
  bp_usGpioBoardReset, // used to reset the entire board
  bp_usGpioUsb0,  	   // used for USB 0	
  bp_usGpioUsb1,       // used for USB 0
  bp_ulOpticalWan,
  bp_ulSimInterfaces,
  bp_ulSlicInterfaces,
  bp_usGpioPonTxEn,
  bp_usGpioPonRxEn,
  bp_usRogueOnuEn,
  bp_usGpioLedSim,
  bp_usGpioLedSim_ITMS,
  bp_usPinMux,
  bp_usGpioSpromClk, /* for BRCM_FTTDP */
  bp_usGpioSpromData, /* for BRCM_FTTDP */
  bp_usGpioSpromRst, /* for BRCM_FTTDP */
  bp_usTsync1025mhz,
  bp_usTsync8khz,
  bp_usTsync1pps,
  bp_usGpioTsyncPonUnstable,
  bp_usGpioLedReserved,
  bp_usGpioReserved,
  bp_usSerialLedData,
  bp_usSerialLedClk,
  bp_usSerialLedMask,
  bp_ReservedDslCtl, // used only internally
  bp_ReservedAnyGpio, // used only internally
  bp_ReservedAnyLed, // used only internally
  bp_ulCrossbar,
  bp_ulCrossbarPhyId,
  bp_ulInterfaceEnable,
  bp_ulMemoryConfig,
  bp_usBatteryEnable,
  bp_ulPinmuxTableSelect,
  bp_usExtIntrPmdAlarm,
  bp_usGpioPmdReset,
  bp_usPmdMACEwakeEn,
  bp_ulPortMaxRate,
  bp_ulAttachedIdx,
  bp_usExtIntrTrplxrTxFail,
  bp_usExtIntrTrplxrSd,
  bp_usTxLaserOnOutN,
  bp_usGpioPonReset,
  bp_usGpio1ppsStable,
  bp_usWanNco10MClk,
  bp_usGpioLteReset,
  bp_usTrxSignalDetect,
  bp_usGpioStrapTxEn,
  bp_usExtIntrWifiOnOff,
  bp_usExtIntrLTE,
  bp_usMiiInterfaceEn,
  bp_usFxsFxo1SpiSSNum,   //
  bp_usFxsFxo2SpiSSNum,   // Enum order must not be changed, also must be contiguous       
  bp_usFxsFxo3SpiSSNum,   //
  bp_usFxsFxoRst1,   //     
  bp_usFxsFxoRst2,   // Enum order must not be changed, also must be contiguous       
  bp_usFxsFxoRst3,   //     
  bp_usDectRst,           
  bp_usVoipRelayCtrl1,    
  bp_usVoipRelayCtrl2,    
  bp_iDectCfg,             
  bp_daughterCardList,    
  bp_usHvgMaxPwm,
  bp_usZarIfSclk,
  bp_usZarIfSdout,
  bp_usZarIfSdin,
  bp_usZarIfEnable,
  bp_usLe9540Reset,
  bp_usSi32392SpiSSNum,
  bp_usSgmiiDetect,
  bp_usVregSync,
  bp_usGpioLedPwmReserved,
  bp_usGpioWlanEN,
  bp_last
};



typedef struct bp_elem {
  enum bp_id id;
  union {
    char * cp;
    unsigned char * ucp;
    unsigned char uc;
    unsigned short us;
    unsigned long ul;
    void * ptr;
#ifdef MSTC_MODIFY //__MSTC__  	
    bp_mdio_init_t * phyinitp;
#endif		
    struct bp_elem * bp_elemp;
  } u;
} bp_elem_t;

extern bp_elem_t * g_pCurrentBp;
extern bp_elem_t * g_BoardParms[];
extern WLAN_SROM_PATCH_INFO wlanPaInfo[];
extern WLAN_PCI_PATCH_INFO wlanPciInfo[];

