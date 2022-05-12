 #ifndef __PKT_CMF_PUBLIC_H_INCLUDED__
#define __PKT_CMF_PUBLIC_H_INCLUDED__
/*
<:copyright-BRCM:2010:DUAL/GPL:standard

   Copyright (c) 2010 Broadcom Corporation
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
#if defined(CONFIG_BCM96368) || defined(CHIP_6368)
#include "pktCmf_6368_public.h"
#endif

#if defined(CONFIG_BCM96816) || defined(CHIP_6816)
#include "pktCmf_6816_public.h"
#endif

#if defined(CONFIG_BCM96818) || defined(CHIP_6818)
#include "pktCmf_6816_public.h"
#endif

typedef struct {
    uint32_t rxDropped;
    uint32_t txDropped;
} PktCmfVportStats_t;

typedef struct {
    uint32_t vport;
    uint32_t *rxDropped_p;
    uint32_t *txDropped_p;
} PktCmfStatsParam_t;

#if defined(CONFIG_BCM_FAP_MODULE) || defined(CONFIG_BCM_FAP) || \
    defined(CONFIG_BCM_ARL_MODULE) || defined(CONFIG_BCM_ARL)

// No need for fap.h right now.  Should be fap_public.h anyways.
//#include "fap.h"
#endif

#endif  /* defined(__PKT_CMF_PUBLIC_H_INCLUDED__) */

