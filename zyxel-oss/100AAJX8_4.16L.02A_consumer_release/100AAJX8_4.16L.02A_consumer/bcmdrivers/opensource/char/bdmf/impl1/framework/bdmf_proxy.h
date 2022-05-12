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
 * bdmf_proxy.h
 *
 * Data path builder - object proxy
 * Allows local access to remote BDMF objects and types
 *
 * This file is Copyright (c) 2011, Broadlight Communications.
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
 *
 * Author: Igor Ternovsky
 *******************************************************************/
#ifndef _BDMF_PROXY_H_
#define _BDMF_PROXY_H_

/** Connect to BDMF running at remote system
 *  \param[in]  target_system   Remote system id
 * \return
 *     0 - OK\n
 *     error code otherwise
 */
int bdmfproxy_connect(bdmf_subsystem_t target_system);

/** Disconnect from BDMF running at remote system
 *  \param[in]  target_system   Remote system id
 * \return
 *     0 - OK\n
 *     error code otherwise
 */
int bdmfproxy_disconnect(bdmf_subsystem_t target_system);


/** Find managed object
 *
 * When no longer needed, the managed object handle must be released by bdmf_put() function.
 *
 * \param[in]   drv     Managed object type handle
 * \param[in]   owner   Object's owner (parent). If not set, the framework tries to identify
 *                      the parent using attributes in the discr string.
 * \param[in]   discr   List of attributes with values in the same format as in bdmf_configure()\n
 *                      attrs string is passed transparently to the driver and must be
 *                      sufficient to uniquely identify the object.\n
 *                      discr string can contain also attributes necessary to identify
 *                      object's parent, parent's parent, etc.
 * \param[out]  pmo     Managed object handle
 * \return
 *     0      - OK \n
 *    <0      - error
 */
int bdmfproxy_find_get(bdmf_type_handle drv,
                  bdmf_object_handle owner, const char *discr,
                  bdmf_object_handle *pmo);


/** Managed object iterator.
 *
 * Get next managed object.
 *
 * \param[in]   drv     Managed object type handle
 * \param[in]   mo      Current managed object or NULL
 * \param[in]   filter  Optional filter string in the same format as in bdmf_configure. Can be NULL
 * \return
 *     0         - next object not found\n
 *     otherwise - next object handle
 */
bdmf_object_handle bdmf_get_next(bdmf_type_handle drv,
                                 bdmf_object_handle mo, const char *filter);


/** Set a number of attributes in a single call.
 * see description in bdmf_configure()
 * \param[in]   mo      Managed object handle
 * \param[in]   attr    Comma delimited list of name=value pairs
 * \return
 *     0 - OK\n
 *     error code otherwise
 */
int bdmfproxy_configure(bdmf_object_handle mo, const char *attr);

/** Create a new managed object of the specified type
 *
 * \param[in]   mo      Local managed object handle
 * \param[in]   attr    An ASCII string of attribute values to be set for the new object.\n
 *                      The string is comma-delimited list in format name=value.\n
 *                      See attr format description in bdmf_configure()
 * \return
 *     0      - OK \n
 *    <0      - error
 */
int bdmfproxy_new(struct bdmf_object *mo, const char *attr);


/** Child iterator.
 *
 * Get next child.
 *
 * \param[in]   owner   Owner's handle
 * \param[in]   type    Optional type name. If !=NULL, only childs of the given type are iterated.
 * \param[in]   mo      Current managed object or NULL
 * \return
 *     0         - next object not found\n
 *     otherwise - next object handle
 */
bdmf_object_handle bdmfproxy_get_next_child(bdmf_object_handle owner,
                                       const char *type, bdmf_object_handle mo);

#endif /* _BDMF_PROXY_H_ */
