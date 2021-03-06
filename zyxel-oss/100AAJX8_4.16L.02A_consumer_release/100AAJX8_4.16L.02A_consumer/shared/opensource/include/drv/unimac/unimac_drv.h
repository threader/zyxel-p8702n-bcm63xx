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
/* This file contains the definition for the Unimac block 			          */
/*                                                                            */
/******************************************************************************/
#ifndef __UNIMAC_DRV_H
#define __UNIMAC_DRV_H

/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/

#include "bl_os_wraper.h"
#include "access_macros.h"
#include "hwapi_mac.h"
#include "bcm_map_part.h"

/******************************************************************************/
/*                                                                            */
/* Types and values definitions                                               */
/*                                                                            */
/******************************************************************************/
/* Macros for reading and writing UNIMAC_CONFIGURATION registers*/

#define UNIMAC_READ32_REG(e,r,o) READ_32( UNIMAC_CONFIGURATION_UMAC_0_RDP_##r + (UNIMAC_CONF_EMAC_OFFSET * (e)) , (o) )

#define UNIMAC_WRITE32_REG(e,r,i) WRITE_32( UNIMAC_CONFIGURATION_UMAC_0_RDP_##r  + (UNIMAC_CONF_EMAC_OFFSET * (e)) , (i) )

#define UNIMAC_READ_FIELD(e,r,f,o) \
	 ( *(volatile uint32_t*)&(o) = (swap4bytes(VAL32(UNIMAC_CONFIGURATION_UMAC_0_RDP_##r + (UNIMAC_CONF_EMAC_OFFSET * (e)) ))   & \
		 UNIMAC_CONFIGURATION_UMAC_0_RDP_##r##_##f##_MASK ) >> UNIMAC_CONFIGURATION_UMAC_0_RDP_##r##_##f##_SHIFT  )

#define UNIMAC_WRITE_FIELD(e,r,f,i) \
	    ( ( VAL32(UNIMAC_CONFIGURATION_UMAC_0_RDP_##r + (UNIMAC_CONF_EMAC_OFFSET * (e)) ) ) = \
		( VAL32(UNIMAC_CONFIGURATION_UMAC_0_RDP_##r +  UNIMAC_CONF_EMAC_OFFSET * (e))  & swap4bytes(~UNIMAC_CONFIGURATION_UMAC_0_RDP_##r##_##f##_MASK) ) | \
		( swap4bytes(( (i) << UNIMAC_CONFIGURATION_UMAC_0_RDP_##r##_##f##_SHIFT ) & UNIMAC_CONFIGURATION_UMAC_0_RDP_##r##_##f##_MASK)) )

/*Macros for reading and Writing UNIMAC_MIB counters*/

#define UNIMAC_READ32_MIB(e,r,o) READ_32( UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r + (UNIMAC_MIB_EMAC_OFFSET * (e)) , (o) )

#define UNIMAC_WRITE32_MIB(e,r,i) WRITE_32( UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r  + (UNIMAC_MIB_EMAC_OFFSET * (e)) , (i) )

#define UNIMAC_READ_MIB_FIELD(e,r,f,o) \
	 ( *(volatile uint32_t*)&(o) = (* ((volatile uint32_t*) DEVICE_ADDRESS(UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r + (UNIMAC_MIB_EMAC_OFFSET * (e)) ) )  & \
		 UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r##_##f##_MASK ) >> UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r##_##f##_SHIFT  )

#define UNIMAC_WRITE_MIB_FIELD(e,r,f,i) \
	    ( ( VAL32(UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r + (UNIMAC_MIB_EMAC_OFFSET * (e))) ) = \
		( VAL32(UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r +  (UNIMAC_MIB_EMAC_OFFSET * (e)))  & swap4bytes(~UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r##_##f##_MASK) ) | \
		( swap4bytes(( (i) << UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r##_##f##_SHIFT ) & UNIMAC_CONFIGURATION_UMAC_MIB_0Module_RDP_##r##_##f##_MASK)) )


typedef enum
{
	UNIMAC_SPEED_10,
	UNIMAC_SPEED_100,
	UNIMAC_SPEED_1000,
	UNIMAC_SPEED_2500
}UNIMAC_SPEED;

typedef enum
{
	UNIMAC_DUPLEX_HALF,
	UNIMAC_DUPLEX_FULL
}UNIMAC_DUPLEX;


#ifndef _BYTE_ORDER_LITTLE_ENDIAN_
typedef struct 
{
	/*Reserved bit must be written with 0.  A read returns an unknown value.*/
	uint32_t	reserved3:1;
	/*Disable RX side RUNT filtering. 0: Enable  RUNT filtering.Reset value is 0x0. */
	uint32_t	runt_filter_dis:1;
	/*	This mode only works in auto-config mode: 
		0: After auto-config, TX_ENA and RX_ENA bits are set to 1. 1: After auto-config, TX_ENA and RX_ENA bits are set to 0,    
		meaning SW will have to come in and enable TX and RX.Reset value is 0x0. */
	uint32_t	txrx_en_config:1;
	/*Ignore TX PAUSE frame transmit request.Reset value is 0x0. */
	uint32_t	tx_pause_ignore:1;
	/*Enable extract/insert of EFM headers.Reset value is 0x0. */
	uint32_t	prbl_ena:1;
	/*This bit currently not used.Reset value is 0x0.*/
	uint32_t	rx_err_disc:1;  
	/*Enable remote loopback at the fifo system side. 0: Normal operation.Reset value is 0x0. */
	uint32_t	rmt_loop_ena:1;
	/*Payload length check. 0: Check payload length with Length/Type field. 1: Check disabled.Reset value is 0x1. */
	uint32_t	no_lgth_check:1;
	/*	MAC control frame enable. 1: MAC control frames with opcode other than 0x0001 are accepted and forwarded to the client interface.
		0: MAC control frames with opcode other than 0x0000 and 0x0001 are silently discarded.
		Reset value is 0x0.
	*/
	uint32_t	cntl_frm_ena:1; 
	/*Enable/Disable auto-configuration. 1: Enable  0: Disable Reset value is 0x0. */
	uint32_t	ena_ext_config:1;
	/*Reserved bits must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved2:6;
	/*Enable GMII/MII loopback 1: Loopback enabled. 0: Normal operation.Reset value is 0x0. */
	uint32_t 	lcl_loop_ena:1;
	/*Reserved bit must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved1:1;
	/*RX and RX engines are put in reset. 0: come out of SW reset.Reset value is 0x0. */
	uint32_t	sw_reset:1; 
	/* Reserved bits must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved0:2;
	/* Ignored when RTH_SPEED[1]==1, gigabit mode. 1: half duplex 0: full duplex Reset value is 0x0.*/
	uint32_t	hd_ena:1;
	/*	The MAC overwrites the source MAC address with a programmed MAC address in register MAC_0 and MAC_1." 
		0: Not modify the source address received from the transmit application client.
		Reset value is 0x0.
	*/
	uint32_t	tx_addr_ins:1;
	/*	Receive PAUSE frames are ignored by the MAC. 
		0: The tramsmit process is stiooed for the amount of time specified in the pause wuanta received within the PAUSE frame.
		Reset value is 0x0. 
	*/
	uint32_t	rx_pause_ignore:1;
	/* 	PAUSE frames are forwarded to the user application. 
		0: The PAUSE frames are terminated and discarded in the MAC.Reset value is 0x1. 
	*/
	uint32_t	pause_fwd:1;
	/*	The CRC field of received frames is transmitted to the user application. 
		0: The CRC field is stripped from the frame.Reset value is 0x1. 
	*/
	uint32_t	crc_fwd:1;
	/*  Padding is removed along with crc field before the frame is sent to the user application. 
		0: No padding is removed by the MAC.
		Reset value is 0x0.
	*/
	uint32_t	pad_en:1; 
	/* All frames are received without Unicast address filtering. 
	   0: Reset value is 0x1. */
	uint32_t 	promis_en:1;
	/*00: 10Mbps, 01: 100Mbps, 10: 1000Mbps, 11: 2500Mbps Reset value is 0x2. */
	uint32_t	eth_speed:2;
	/* 	The MAC receive function is enabled. 0: The MAC receive function is disabled. 
		The enable works on packet boundary meaning that only on the assertion on the bit during every 0->1 transition of rx_dv.
		Reset value is 0x0. 
	*/	
	uint32_t	rx_ena:1;
	/*	The MAC transmit function is enabled. 0: The MAC transmit function is disabled.
		The enable works on packet boundary meaning that only on the assertion of the bit during every SOP.
		Reset value is 0x0. 
	*/
	uint32_t    tx_ena:1;

}S_UNIMAC_CMD_REG;

typedef struct
{

	/* Reserved bits must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved:26;
	/*Link status indication.Reset value is 0x0. */
	uint32_t	mac_link_stat:1; 
	/* 1: MAC Tx pause enabled. 0: MAC Tx pause disabled.Reset value is 0x1. */
	uint32_t	mac_tx_pause:1;
	/*1: MAC Rx pause enabled. 0: MAC Rx pause disabled.Reset value is 0x1. */
	uint32_t	mac_rx_pause:1; 
	/*1: Half duplex. 0: Full duplex. Reset value is 0x0. */
	uint32_t	mac_duplex:1;
	/* 00: 10Mbps, 01: 100Mbps, 10: 1Gbps, 11: 2.5Gbps Reset value is 0x2. 	*/
	uint32_t	mac_speed:2;

}S_UNIMAC_STAT_REG;

typedef struct
{
	uint32_t	reserved:15;
	uint32_t	pp_gen:1; 
	uint32_t	pp_pse_en:8; 
	uint32_t	launch_en:1; 
	uint32_t	ext_tx_flow:1; 
	uint32_t	mac_crc_owrt:1; 
	uint32_t	mac_crc_fwd:1; 
	uint32_t	txcrcerr:1;
	uint32_t	ss_mode_mii:1; 
	uint32_t	gport_mode:1;
	uint32_t	gmii_direct:1;

}S_UNIMAC_CFG_REG;

typedef struct
{
    uint32_t    reserved:7;
    uint32_t    rxfifo_congestion_threshold:9;
    uint32_t    reserved1:2;
    uint32_t    max_pkt_size:14;

}S_UNIMAC_TOP_CFG1_REG;

#else /* little endian */

typedef struct 
{
	/*	The MAC transmit function is enabled. 0: The MAC transmit function is disabled.
		The enable works on packet boundary meaning that only on the assertion of the bit during every SOP.
		Reset value is 0x0. 
	*/
	uint32_t    tx_ena:1;
	/* 	The MAC receive function is enabled. 0: The MAC receive function is disabled. 
		The enable works on packet boundary meaning that only on the assertion on the bit during every 0->1 transition of rx_dv.
		Reset value is 0x0. 
	*/	
	uint32_t	rx_ena:1;
	/*00: 10Mbps, 01: 100Mbps, 10: 1000Mbps, 11: 2500Mbps Reset value is 0x2. */
	uint32_t	eth_speed:2;
	/* All frames are received without Unicast address filtering. 
	   0: Reset value is 0x1. */
	uint32_t 	promis_en:1;
	/*  Padding is removed along with crc field before the frame is sent to the user application. 
		0: No padding is removed by the MAC.
		Reset value is 0x0.
	*/
	uint32_t	pad_en:1; 
	/*	The CRC field of received frames is transmitted to the user application. 
		0: The CRC field is stripped from the frame.Reset value is 0x1. 
	*/
	uint32_t	crc_fwd:1;
	/* 	PAUSE frames are forwarded to the user application. 
		0: The PAUSE frames are terminated and discarded in the MAC.Reset value is 0x1. 
	*/
	uint32_t	pause_fwd:1;
	/*	Receive PAUSE frames are ignored by the MAC. 
		0: The tramsmit process is stiooed for the amount of time specified in the pause wuanta received within the PAUSE frame.
		Reset value is 0x0. 
	*/
	uint32_t	rx_pause_ignore:1;
	/*	The MAC overwrites the source MAC address with a programmed MAC address in register MAC_0 and MAC_1." 
		0: Not modify the source address received from the transmit application client.
		Reset value is 0x0.
	*/
	uint32_t	tx_addr_ins:1;
	/* Ignored when RTH_SPEED[1]==1, gigabit mode. 1: half duplex 0: full duplex Reset value is 0x0.*/
	uint32_t	hd_ena:1;
	/* Reserved bits must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved0:2;
	/*RX and RX engines are put in reset. 0: come out of SW reset.Reset value is 0x0. */
	uint32_t	sw_reset:1; 
	/*Reserved bit must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved1:1;
	/*Enable GMII/MII loopback 1: Loopback enabled. 0: Normal operation.Reset value is 0x0. */
	uint32_t 	lcl_loop_ena:1;
	/*Reserved bits must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved2:6;
	/*Enable/Disable auto-configuration. 1: Enable  0: Disable Reset value is 0x0. */
	uint32_t	ena_ext_config:1;
	/*	MAC control frame enable. 1: MAC control frames with opcode other than 0x0001 are accepted and forwarded to the client interface.
		0: MAC control frames with opcode other than 0x0000 and 0x0001 are silently discarded.
		Reset value is 0x0.
	*/
	uint32_t	cntl_frm_ena:1; 
	/*Payload length check. 0: Check payload length with Length/Type field. 1: Check disabled.Reset value is 0x1. */
	uint32_t	no_lgth_check:1;
	/*Enable remote loopback at the fifo system side. 0: Normal operation.Reset value is 0x0. */
	uint32_t	rmt_loop_ena:1;
	/*This bit currently not used.Reset value is 0x0.*/
	uint32_t	rx_err_disc:1;  
	/*Enable extract/insert of EFM headers.Reset value is 0x0. */
	uint32_t	prbl_ena:1;
	/*Ignore TX PAUSE frame transmit request.Reset value is 0x0. */
	uint32_t	tx_pause_ignore:1;
	/*	This mode only works in auto-config mode: 
		0: After auto-config, TX_ENA and RX_ENA bits are set to 1. 1: After auto-config, TX_ENA and RX_ENA bits are set to 0,    
		meaning SW will have to come in and enable TX and RX.Reset value is 0x0. */
	uint32_t	txrx_en_config:1;
	/*Disable RX side RUNT filtering. 0: Enable  RUNT filtering.Reset value is 0x0. */
	uint32_t	runt_filter_dis:1;
	/*Reserved bit must be written with 0.  A read returns an unknown value.*/
	uint32_t	reserved3:1;

}S_UNIMAC_CMD_REG;

typedef struct
{

	/* 00: 10Mbps, 01: 100Mbps, 10: 1Gbps, 11: 2.5Gbps Reset value is 0x2. 	*/
	uint32_t	mac_speed:2;
	/*1: Half duplex. 0: Full duplex. Reset value is 0x0. */
	uint32_t	mac_duplex:1;
	/*1: MAC Rx pause enabled. 0: MAC Rx pause disabled.Reset value is 0x1. */
	uint32_t	mac_rx_pause:1; 
	/* 1: MAC Tx pause enabled. 0: MAC Tx pause disabled.Reset value is 0x1. */
	uint32_t	mac_tx_pause:1;
	/*Link status indication.Reset value is 0x0. */
	uint32_t	mac_link_stat:1; 
	/* Reserved bits must be written with 0.  A read returns an unknown value. */
	uint32_t	reserved:26;

}S_UNIMAC_STAT_REG;

typedef struct
{
	uint32_t	gmii_direct:1;
	uint32_t	gport_mode:1;
	uint32_t	ss_mode_mii:1; 
	uint32_t	txcrcerr:1;
	uint32_t	mac_crc_fwd:1; 
	uint32_t	mac_crc_owrt:1; 
	uint32_t	ext_tx_flow:1; 
	uint32_t	launch_en:1; 
	uint32_t	pp_pse_en:8; 
	uint32_t	pp_gen:1; 
	uint32_t	reserved:15;

}S_UNIMAC_CFG_REG;

typedef struct
{
    uint32_t    max_pkt_size:14;
    uint32_t    reserved1:2;
    uint32_t    rxfifo_congestion_threshold:9;
    uint32_t    reserved:7;

}S_UNIMAC_TOP_CFG1_REG;

#endif

#endif

