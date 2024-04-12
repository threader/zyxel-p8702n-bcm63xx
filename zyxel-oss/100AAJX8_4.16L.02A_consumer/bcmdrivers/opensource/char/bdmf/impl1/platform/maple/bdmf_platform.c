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
 * bdmf_platform.c
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
#include <bdmf_dev.h>

#ifdef CONFIG_TRANSPORT
#include <bdmf_transport_shm.h>
#endif

/* Local subsystem */
bdmf_subsystem_t bdmf_local_subsystem;

#ifdef BDMF_SHELL
bdmfmon_enum_val_t subsystem_type_enum_table[] = {
    { .name="ehost", .val=BDMF_SUBSYSTEM_EHOST},
    { .name="cmips", .val=BDMF_SUBSYSTEM_CMIPS},
    { .name="pmips1",.val=BDMF_SUBSYSTEM_PMIPS1},
    { .name="pmips2",.val=BDMF_SUBSYSTEM_PMIPS2},
    { .name="pmips3",.val=BDMF_SUBSYSTEM_PMIPS3},
    { .name="pmips4",.val=BDMF_SUBSYSTEM_PMIPS4},
    BDMFMON_ENUM_LAST
};
#endif


/* ISR
 */
static int bdmf_platform_config_isr(int irq, void *data)
{
    bdmf_platform_config_area_t *config_area = data;
    bdmf_platform_config_cmd_execute(config_area);
    return 0;
}

/* Platform-level initialization */
int bdmf_platform_module_init(void)
{
    /* Register memory area types */

    return 0;
}

void bdmf_platform_module_exit(void)
{
}

/** Sub-system name
 * \param[in]   subsystem   Subsystem id
 * \return  subsystem name
 */
const char *bdmf_subsystem_name(bdmf_subsystem_t subsystem)
{
    static char *subsystem_name[] = {
                    [BDMF_SUBSYSTEM_EHOST]    = "ehost",
                    [BDMF_SUBSYSTEM_CMIPS]    = "cmips",
                    [BDMF_SUBSYSTEM_PMIPS1]   = "pmips1",
                    [BDMF_SUBSYSTEM_PMIPS2]   = "pmips1",
                    [BDMF_SUBSYSTEM_PMIPS3]   = "pmips1",
                    [BDMF_SUBSYSTEM_PMIPS4]   = "pmips1"
    };
    static const char *invalid="*invalid*";
    if (subsystem >= BDMF_SUBSYSTEM__NUMBER_OF)
        return invalid;
    return subsystem_name[subsystem];
}

/** Get subsystem id by its name
 * \param[in]   name    Subsystem name
 * \param[out]  subs    Subsystem id
 * \return
 *      0 = ok\n
 *     <0 - error
 */
int bdmf_subsystem_by_name(const char *name, bdmf_subsystem_t *subs)
{
    bdmf_subsystem_t s;
    if (!name || !subs)
        return BDMF_ERR_PARM;
    for(s=BDMF_SUBSYSTEM_LOCAL+1; s<BDMF_SUBSYSTEM__NUMBER_OF; s++)
    {
        if (!strcmp(bdmf_subsystem_name(s), name))
        {
            *subs = s;
            return 0;
        }
    }
    return BDMF_ERR_NO_SUBS;
}


/** Set local subsystem
 * The function can be called only once.
 * \param[in]   subs    Local subsystem id
 * \return
 *      0 = ok\n
 *     <0 - error
 */
int bdmf_subsystem_set(bdmf_subsystem_t subs)
{
    bdmf_platform_config_area_t *cfg;
    if (bdmf_local_subsystem != BDMF_SUBSYSTEM_LOCAL)
        return BDMF_ERR_ALREADY;
    if ((subs <= BDMF_SUBSYSTEM_LOCAL) || (subs >= BDMF_SUBSYSTEM__NUMBER_OF))
        return BDMF_ERR_PARM;
#ifdef BDMF_SYSTEM_SIM
    {
        FILE *f;
        char fname[32];
        snprintf(fname, sizeof(fname), "/tmp/bdmf_%s.pid", bdmf_subsystem_name(subs));
        f = fopen(fname, "w");
        if (!f)
            return BDMF_ERR_INTERNAL;
        fprintf(f, "%d", getpid());
        fclose(f);
    }
#endif
    bdmf_local_subsystem = subs;

    /* Register ISR for configuration. In real system will probably be done using polling.
     * Note that the following code assumes configuration by a single
     * configuration manager.
     */
    cfg = bdmf_platform_config_get(subs);
    if (cfg)
        bdmf_irq_connect(BDMF_IRQ_CONFIG_AREA, bdmf_platform_config_isr, (void *)cfg);
    else
        bdmf_print_error("Can't get config area for subs %s\n", bdmf_subsystem_name(subs));

    return 0;
}

/** Get irq base for interrupts sent from the given subsystem
 * \param[in]   subs    Local subsystem id
 * \return
 *     >=0 = IRQ base\n
 *     <0  - error
 */
int bdmf_subsystem_irq_base(bdmf_subsystem_t subs)
{
#ifdef BDMF_SYSTEM_SIM
    return 1 + subs* BDMF_IRQ_PER_SUBS;
#else
#error Platform is not supported
#endif
}

/** Get configuration area address */
bdmf_platform_config_area_t *bdmf_platform_config_get(bdmf_subsystem_t subs)
{
#if defined(BDMF_SYSTEM_SIM) && defined(CONFIG_TRANSPORT)
#define MAPLE_SIM_CFG_SHM_NAME    "/maple_cfg%d.shm"
    static bdmf_platform_config_area_t *bdmf_cfg_array[BDMF_SUBSYSTEM__NUMBER_OF];
    char shm_name[64];

    if (subs >= BDMF_SUBSYSTEM__NUMBER_OF)
    {
        bdmf_print_error("Invalid target subsystem %d\n", subs);
        return NULL;
    }

    if (bdmf_cfg_array[subs])
        return bdmf_cfg_array[subs];

    /* We can only do meaningful memory mapping if *this* subsystem is set */
    if (bdmf_subsystem()==BDMF_SUBSYSTEM_LOCAL)
    {
        bdmf_print_error("Local subsystem must be set\n");
        return NULL;
    }

    snprintf(shm_name, sizeof(shm_name), MAPLE_SIM_CFG_SHM_NAME, subs);
    /* do memory mapping */
    bdmf_cfg_array[subs] = bdmf_mmap(shm_name, sizeof(bdmf_platform_config_area_t));
    if (!bdmf_cfg_array[subs])
        return NULL;
    return bdmf_cfg_array[subs];
#endif
    return NULL;
}

/** Wait for configuration command
 */
void bdmf_platform_config_cmd_wait(bdmf_subsystem_t subs)
{
    bdmf_platform_config_area_t *cfg_area = bdmf_platform_config_get(subs);
    if (!cfg_area)
    {
        bdmf_print_error("aborted\n");
        return;
    }
    while(!cfg_area->cmd_resp.cmd_resp)
        usleep(1000);
}

/** Execute configuration command
 */
int bdmf_platform_config_cmd_execute(bdmf_platform_config_area_t *cfg_area)
{
    int rc = 0;
    assert(cfg_area->cmd_resp.cmd_resp);
    switch(cfg_area->cmd_resp.cmd)
    {
    case BDMF_CONFIG_INIT:
        break;
#ifdef CONFIG_TRANSPORT
    case BDMF_CONFIG_MSG_QUEUE:
        /* configure message queue */
        rc = bdmfts_shm_connect_request(&cfg_area->cmd_resp);
        break;
#endif
    default:
        rc = BDMF_ERR_NOT_SUPPORTED;
        break;
    }
    cfg_area->cmd_resp.error = rc;
    cfg_area->cmd_resp.cmd_resp = 0;
    return rc;
}
