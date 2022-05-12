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
#include <bdmf_dev.h>

#ifdef BDMF_TRANSPORT_SHM
#include <bdmf_transport_shm.h>
#endif

/* Local subsystem */
bdmf_subsystem_t bdmf_local_subsystem;

#ifdef BDMF_SHELL
bdmfmon_enum_val_t subsystem_type_enum_table[] = {
    { .name="mipsc", .val=BDMF_SUBSYSTEM_MIPSC},
    { .name="mipsd", .val=BDMF_SUBSYSTEM_MIPSD},
    BDMFMON_ENUM_LAST
};
#endif


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
                    [BDMF_SUBSYSTEM_MIPSC]    = "mipsc",
                    [BDMF_SUBSYSTEM_MIPSD]    = "mipsd",
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
    if (bdmf_local_subsystem != BDMF_SUBSYSTEM_LOCAL)
        return BDMF_ERR_ALREADY;
    if ((subs <= BDMF_SUBSYSTEM_LOCAL) || (subs >= BDMF_SUBSYSTEM__NUMBER_OF))
        return BDMF_ERR_PARM;
    bdmf_local_subsystem = subs;
    return 0;
}
