/*
 Copyright 2004-2010 Broadcom Corp. All Rights Reserved.

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
#ifndef _ETHSW_RUNNER_H_
#define _ETHSW_RUNNER_H_

#include "bcmtypes.h"
#include "bcmmii.h"

int bcmeapi_ethsw_dump_mib(int port, int type);
#define bcmeapi_ioctl_ethsw_dscp_to_priority_mapping(e) 0
#define bcmeapi_ioctl_ethsw_pcp_to_priority_mapping(e) 0
#define bcmeapi_ioctl_ethsw_set_multiport_address(e) 0


/****************************************************************************
    Prototypes
****************************************************************************/

#if defined(STAR_FIGHTER2)
void sf2_mdio_master_enable(void);
void sf2_mdio_master_disable(void);
#endif

#define bcmeapi_ethsw_init_config() {}
#define ethsw_port_mirror_get(p1, p2, p3, p4, p5) {}
#define ethsw_port_mirror_set(p1, p2, p3, p4, p5) {}

#endif /* _ETHSW_RUNNER_H_ */
