/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: provCfg.h
*
****************************************************************************
*    Description:
*
*     Provisioning default build options.  To override defaults in
*     provCfg.h make a file called provCfgCustom.h in your build
*     directory.
*
****************************************************************************/

#ifndef PROV_CFG_H
#define PROV_CFG_H

#ifdef __cplusplus
extern "C" {
#endif



/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/


/* Task priority for MTA provisioning */
#define PROV_CFG_TASK_PRIORITY         MTA_TASK_MANAGEMENT


/* Task priority for MTA DHCP */
#define PROV_CFG_DHCP_TASK_PRIORITY    MTA_TASK_MANAGEMENT

/* Secure provisioning support */
#define PROV_CFG_SECURITY              1

/* Use SIP configuration information from non-volatile storage */
#define PROV_CFG_USE_SIP_NONVOL        1

/*
*****************************************************************************
** CUSTOM CONFIGURATION - This must be included after all default
** build configuration options. Custom include files should undef default
** config options and then re-define them.
*****************************************************************************
*/
#include <provCfgCustom.h>


#ifdef __cplusplus
    }
#endif

#endif  /* PROV_CFG_H  */


