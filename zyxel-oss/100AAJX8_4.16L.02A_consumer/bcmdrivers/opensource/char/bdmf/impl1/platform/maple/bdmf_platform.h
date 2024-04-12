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
 * Maple platform file
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
 
#ifndef _BL_PLATFORM_H_
#define _BL_PLATFORM_H_

#include <bdmf_errno.h>

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

    BDMF_SUBSYSTEM_EHOST,        /**< External host */
    BDMF_SUBSYSTEM_CMIPS,        /**< Core 0 (CMIPS) */
    BDMF_SUBSYSTEM_PMIPS1,       /**< Core 1 (PMIPS1) */
    BDMF_SUBSYSTEM_PMIPS2,       /**< Core 2 (PMIPS2) */
    BDMF_SUBSYSTEM_PMIPS3,       /**< Core 3 (PMIPS3) */
    BDMF_SUBSYSTEM_PMIPS4,       /**< Core 4 (PMIPS4) */

    BDMF_SUBSYSTEM__NUMBER_OF,
    BDMF_SUBSYSTEM_ANY = BDMF_SUBSYSTEM__NUMBER_OF
} bdmf_subsystem_t;

#define BDMF_CONFIG_CMD_AREA_SIZE     64      /**< Configuration command/response area size */

/** System configuration commands
 */
typedef enum
{
    BDMF_CONFIG_NONE,                         /**< No command */
    BDMF_CONFIG_INIT,                         /**< System init */
    BDMF_CONFIG_MSG_QUEUE,                    /**< Configure message queue command */
} bdmf_config_cmd_t;

/** Command/response area
 */
typedef volatile struct bdmf_config_cmd_resp
{
    char cmd_resp;          /**< 1=command-owned by responder, 0=response-owned by commander */
    bdmf_subsystem_t client;  /**< requester subsystem */
    bdmf_config_cmd_t cmd;    /**< Configuration command */
    bdmf_error_t error;       /**< Completion status */
    union {
        /** Configure message queue command area */
        struct
        {
            uint16_t id;        /**< cmd: message queue id (priority) */
            uint16_t size;      /**< cmd,resp: Message queue size (entries). 0=default */
            uint16_t entry_size;/**< cmd,resp: Message queue entry size. 0=default */
            long dsc_address;   /**< resp: descriptor array address */
            long req_address;   /**< resp: request array address */
            long rep_address;   /**< resp: reply array address */
        } msgq_init;
        char cparms[BDMF_CONFIG_CMD_AREA_SIZE]; /* Command/response parameters */
    } x;
} bdmf_config_cmd_resp_t;


/** Interrupt assignment */
#define BDMF_IRQ_PER_SUBS     8   /**< Number of interrupts per source core */
#define BDMF_IRQ_CONFIG_AREA  0   /**< IRQ reserved for configuration area */


/** Platform configuration area
 */
typedef volatile struct bdmf_platform_config_area
{
    bdmf_config_cmd_resp_t cmd_resp;  /**< Configuration command/response area */
} bdmf_platform_config_area_t;

/** Get configuration area address */
bdmf_platform_config_area_t *bdmf_platform_config_get(bdmf_subsystem_t subs);

/** Wait for configuration command
 */
void bdmf_platform_config_cmd_wait(bdmf_subsystem_t subs);

/** Execute configuration command
 */
int bdmf_platform_config_cmd_execute(bdmf_platform_config_area_t *);

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

/** Get irq base for interrupts sent from the given subsystem
 * \param[in]   subs    Local subsystem id
 * \return
 *     >=0 = IRQ base\n
 *     <0  - error
 */
int bdmf_subsystem_irq_base(bdmf_subsystem_t subs);


#endif /* _BL_PLATFORM_H_ */

