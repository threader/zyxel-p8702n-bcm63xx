/*****************************************************************************
 **   FILE NAME       : IFIN_SIP_AddrIf.h
 **   PROJECT         : INCA IP
 **   MODULES         : SIP 
 **   SRC VERSION     : V0.1
 **   DATE            : 18-12-2003
 **   AUTHOR          : Murali
 **   DESCRIPTION     : This file contains SIP address defns
 **   FUNCTIONS       :
 **   COMPILER        : gcc
 **   REFERENCE       : Coding guide lines for IFIN COM
 **   COPYRIGHT       : Infineon Technologies AG 2003 - 2004

 **  Version Control Section  **
 **   $Author: achhibber $
 **   $Date: 2006/04/17 04:02:28 $
 **   $Revisions$
 **   $Log: IFIN_SIP_AddrIf.h,v $
 **   Revision 1.3  2006/04/17 04:02:28  achhibber
 **   no message
 **       Revision history
 *****************************************************************************/
#ifndef __IFIN_SIP_ADDRIF_H__
#define __IFIN_SIP_ADDRIF_H__

#include "ifx_common_defs.h"
/* Some macro's to define the sizes */
#define IFIN_SIP_MAX_REG_ADDR		16
#define IFIN_SIP_MAX_USR_NAME		80
#define IFIN_SIP_MAX_DISP_NAME		128


/* Address type being given by SIP module to Phone Application */
typedef  enum {	
	IFX_IP_ADDR=1,
	IFX_TEL_NUM,
	IFX_URL,
    IFX_EXTN
}e_AddrType;

/* Format in which the SIP module expects Address from Phone Application */
typedef struct
{
	e_AddrType	eAddrType;/* Type of address being given */
	char8		szDisplayName[IFIN_SIP_MAX_DISP_NAME];/* Display Name of the destination */
	char8		szUserName[IFIN_SIP_MAX_USR_NAME];/* User Name of the destination/Phone Number*/
	char8		cCalledAddr[IFIN_SIP_MAX_REG_ADDR];/* IP address/FQDN */
  uint32  uiPort;	/* Port Number on which th destination can be reached */
} x_IFIN_SIP_CalledAddr;

#endif /*__IFIN_SIP_ADDRIF_H__ */










