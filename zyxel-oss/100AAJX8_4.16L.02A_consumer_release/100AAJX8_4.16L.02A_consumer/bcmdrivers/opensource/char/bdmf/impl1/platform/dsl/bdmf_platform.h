/*
* <:copyright-BRCM:2013:GPL/GPL:standard
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


/*******************************************************************
 * bdmf_platform.h
 *
 * Lilac platform file
 *
 * This file is Copyright (c) 2011, Broadlight.
 * This file is licensed under GNU Public License, except that if
 * you have entered in to a signed, written license agreement with
 * Broadlight covering this file, that agreement applies to this
 * file instead of the GNU Public License.
 *
 * This file is free software: you can redistribute and/or modify it
 * under the terms of the GNU Public License, Version 2, as published
 * by the Free Software Foundation, unless a different license
 * applies as provided above.
 *
 * This program is distributed in the hope that it will be useful,
 * but AS-IS and WITHOUT ANY WARRANTY; without even the implied
 * warranties of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 * TITLE or NONINFRINGEMENT. Redistribution, except as permitted by
 * the GNU Public License or another license agreement between you
 * and Broadlight, is prohibited.
 *
 * You should have received a copy of the GNU Public License,
 * Version 2 along with this file; if not, see
 * <http://www.gnu.org/licenses>.
 *******************************************************************/
 
#ifndef BDMF_PLATFORM_DSL_H

#define BDMF_PLATFORM_DSL_H

#include <bdmf_errno.h>

#if !defined(BDMF_SYSTEM_SIM) && !defined(FIRMWARE_INIT)
#include <bl_os_wraper.h>
#include <rdp_drv_bpm.h>
#endif

/** Memory types
 */
typedef enum
{
    BDMF_MEM_CACHE,         /**< Cacheable MIPS DDR */
    BDMF_MEM_NCACHE,        /**< Non-cacheable MIPS DDR */
    BDMF_MEM_TM_SRAM,       /**< TM SRAM */

    BDMF_MEM__NUMBER_OF
} bdmf_mem_type_t;


/** sub-system for inter-processor communication
 */
typedef enum
{
    BDMF_SUBSYSTEM_LOCAL,        /**< Local subsystem */

    BDMF_SUBSYSTEM_MIPSC,        /**< MIPS-C */
    BDMF_SUBSYSTEM_MIPSD,        /**< MIPS-D */

    BDMF_SUBSYSTEM__NUMBER_OF,
    BDMF_SUBSYSTEM_ANY = BDMF_SUBSYSTEM__NUMBER_OF
} bdmf_subsystem_t;


#ifdef BDMF_SHELL

#include <bdmf_session.h>
#include <bdmf_shell.h>

extern bdmfmon_enum_val_t subsystem_type_enum_table[];

#endif


/* External declarations of maple-specific enumeration types for use as attribute types */


/* Platform-level initialization */
int bdmf_platform_module_init(void);
void bdmf_platform_module_exit(void);


/** Sub-system name
 * \param[in]   subsystem   Subsystem id
 * \return  subsystem name
 */
const char *bdmf_subsystem_name(bdmf_subsystem_t subsystem);

/* Local subsystem */
extern bdmf_subsystem_t bdmf_local_subsystem;

/** Local subsystem
 * \return  local subsystem
 */
static inline bdmf_subsystem_t bdmf_subsystem(void)
{
    return bdmf_local_subsystem;
}

/** Get subsystem id by its name
 * \param[in]   name    Subsystem name
 * \param[out]  subs    Subsystem id
 * \return
 *      0 = ok\n
 *     <0 - error
 */
int bdmf_subsystem_by_name(const char *name, bdmf_subsystem_t *subs);

/** Set local subsystem
 * The function can be called only once.
 * \param[in]   subs    Local subsystem id
 * \return
 *      0 = ok\n
 *     <0 - error
 */
int bdmf_subsystem_set(bdmf_subsystem_t subs);

#endif /* BDMF_PLATFORM_DSL_H */

