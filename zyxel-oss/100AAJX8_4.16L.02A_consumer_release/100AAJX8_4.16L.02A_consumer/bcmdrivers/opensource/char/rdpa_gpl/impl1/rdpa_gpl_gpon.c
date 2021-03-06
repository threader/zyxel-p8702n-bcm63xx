// <:copyright-BRCM:2013:DUAL/GPL:standard
// 
//    Copyright (c) 2013 Broadcom Corporation
//    All Rights Reserved
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2, as published by
// the Free Software Foundation (the "GPL").
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// 
// A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
// writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
// 
// :>
/*
 * gpon object GPL shim file.
 * This file is generated automatically. Do not edit!
 */

bdmf_type_handle (*f_rdpa_gpon_drv)(void);

EXPORT_SYMBOL(f_rdpa_gpon_drv);

/** Get gpon type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a gpon object.
 * \return gpon type handle
 */
bdmf_type_handle rdpa_gpon_drv(void)
{
   if (!f_rdpa_gpon_drv)
       return NULL;
   return f_rdpa_gpon_drv();
}

EXPORT_SYMBOL(rdpa_gpon_drv);

int (*f_rdpa_gpon_get)(bdmf_object_handle *pmo);
EXPORT_SYMBOL(f_rdpa_gpon_get);

/** Get gpon object.

 * This function returns gpon object instance.
 * \param[out] gpon_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_gpon_get(bdmf_object_handle *gpon_obj)
{
   if (!f_rdpa_gpon_get)
       return BDMF_ERR_STATE;
   return f_rdpa_gpon_get(gpon_obj);
}
EXPORT_SYMBOL(rdpa_gpon_get);

bdmf_type_handle (*f_rdpa_tcont_drv)(void);

EXPORT_SYMBOL(f_rdpa_tcont_drv);

/** Get tcont type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a tcont object.
 * \return tcont type handle
 */
bdmf_type_handle rdpa_tcont_drv(void)
{
   if (!f_rdpa_tcont_drv)
       return NULL;
   return f_rdpa_tcont_drv();
}

EXPORT_SYMBOL(rdpa_tcont_drv);

int (*f_rdpa_tcont_get)(bdmf_number index_, bdmf_object_handle *pmo);
EXPORT_SYMBOL(f_rdpa_tcont_get);

/** Get tcont object by key.

 * This function returns tcont object instance by key.
 * \param[in] index_    Object key
 * \param[out] tcont_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_tcont_get(bdmf_number index_, bdmf_object_handle *tcont_obj)
{
   if (!f_rdpa_tcont_get)
       return BDMF_ERR_STATE;
   return f_rdpa_tcont_get(index_, tcont_obj);
}
EXPORT_SYMBOL(rdpa_tcont_get);

bdmf_type_handle (*f_rdpa_gem_drv)(void);

EXPORT_SYMBOL(f_rdpa_gem_drv);

/** Get gem type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a gem object.
 * \return gem type handle
 */
bdmf_type_handle rdpa_gem_drv(void)
{
   if (!f_rdpa_gem_drv)
       return NULL;
   return f_rdpa_gem_drv();
}

EXPORT_SYMBOL(rdpa_gem_drv);

int (*f_rdpa_gem_get)(bdmf_number index_, bdmf_object_handle *pmo);
EXPORT_SYMBOL(f_rdpa_gem_get);

/** Get gem object by key.

 * This function returns gem object instance by key.
 * \param[in] index_    Object key
 * \param[out] gem_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_gem_get(bdmf_number index_, bdmf_object_handle *gem_obj)
{
   if (!f_rdpa_gem_get)
       return BDMF_ERR_STATE;
   return f_rdpa_gem_get(index_, gem_obj);
}
EXPORT_SYMBOL(rdpa_gem_get);

MODULE_LICENSE("GPL");
