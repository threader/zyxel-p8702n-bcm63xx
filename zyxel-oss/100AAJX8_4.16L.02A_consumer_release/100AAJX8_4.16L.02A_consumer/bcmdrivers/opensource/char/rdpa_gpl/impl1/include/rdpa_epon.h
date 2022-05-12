/*
* <:copyright-BRCM:2013:DUAL/GPL:standard
* 
*    Copyright (c) 2013 Broadcom Corporation
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
*/

#ifndef _RDPA_EPON_H
#define _RDPA_EPON_H

/** \defgroup epon Commonly used types and constants
 * Epon object represent the EPON MAC. By setting its attributes EPON MAC
 * functionality can be controlled. 
 * @{
 */

/** 
 * Each unicast link has to be associated with a mcast link. In the BCM OAM
 * mode, each unicast link may have its own multicast link. In other mode, all
 * unicast link will share the same multicast link. This function is called by
 * OAM stack for each unicast link in the system. The EPON MAC gets configured
 * only when "enable" will set to "yes" 
 */

typedef struct
{
    uint32_t llid;
    bdmf_boolean enable;
} rdpa_epon_mcast_link_t;

/**
 * Enable EPON MAC FEC in ds and/or us
 */
typedef struct
{
    bdmf_boolean ds;
    bdmf_boolean us;
} rdpa_epon_fec_enable_t;

/** @} end of epon Doxygen group */

#endif //_RDPA_EPON_H

