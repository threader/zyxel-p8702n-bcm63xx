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
 * epon object header file.
 * This header file is generated automatically. Do not edit!
 */
#ifndef _RDPA_AG_EPON_H_
#define _RDPA_AG_EPON_H_

/** \addtogroup epon
 * @{
 */


/** Get epon type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create an epon object.
 * \return epon type handle
 */
bdmf_type_handle rdpa_epon_drv(void);

/* epon: Attribute types */
typedef enum {
    rdpa_epon_attr_mode = 0, /* mode : RW : enum/4 : epon oam: ctc/dpoe/bcm */
    rdpa_epon_attr_mcast_link = 1, /* mcast_link : RWF : aggregate/5[8] epon_mcast_link(rdpa_epon_mcast_link_t) : EPON multicast link properties */
    rdpa_epon_attr_fec_enable = 2, /* fec_enable : RWF : aggregate/2[8] epon_fec_enable(rdpa_epon_fec_enable_t) : enable EPON MAC FEC for us/ds or both */
} rdpa_epon_attr_types;

extern int (*f_rdpa_epon_get)(bdmf_object_handle *pmo);

/** Get epon object.

 * This function returns epon object instance.
 * \param[out] epon_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_epon_get(bdmf_object_handle *epon_obj);

/** Get epon/mode attribute.
 *
 * Get epon oam: ctc/dpoe/bcm.
 * \param[in]   mo_ epon object handle or mattr transaction handle
 * \param[out]  mode_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_epon_mode_get(bdmf_object_handle mo_, rdpa_epon_mode *mode_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_epon_attr_mode, &_nn_);
    *mode_ = (rdpa_epon_mode)_nn_;
    return _rc_;
}


/** Set epon/mode attribute.
 *
 * Set epon oam: ctc/dpoe/bcm.
 * \param[in]   mo_ epon object handle or mattr transaction handle
 * \param[in]   mode_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_epon_mode_set(bdmf_object_handle mo_, rdpa_epon_mode mode_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_epon_attr_mode, mode_);
}


/** Get epon/mcast_link attribute entry.
 *
 * Get EPON multicast link properties.
 * \param[in]   mo_ epon object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  mcast_link_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_epon_mcast_link_get(bdmf_object_handle mo_, bdmf_index ai_, rdpa_epon_mcast_link_t * mcast_link_)
{
    return bdmf_attrelem_get_as_buf(mo_, rdpa_epon_attr_mcast_link, (bdmf_index)ai_, mcast_link_, sizeof(*mcast_link_));
}


/** Set epon/mcast_link attribute entry.
 *
 * Set EPON multicast link properties.
 * \param[in]   mo_ epon object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[in]   mcast_link_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_epon_mcast_link_set(bdmf_object_handle mo_, bdmf_index ai_, const rdpa_epon_mcast_link_t * mcast_link_)
{
    return bdmf_attrelem_set_as_buf(mo_, rdpa_epon_attr_mcast_link, (bdmf_index)ai_, mcast_link_, sizeof(*mcast_link_));
}


/** Get epon/fec_enable attribute entry.
 *
 * Get enable EPON MAC FEC for us/ds or both.
 * \param[in]   mo_ epon object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  fec_enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_epon_fec_enable_get(bdmf_object_handle mo_, bdmf_index ai_, rdpa_epon_fec_enable_t * fec_enable_)
{
    return bdmf_attrelem_get_as_buf(mo_, rdpa_epon_attr_fec_enable, (bdmf_index)ai_, fec_enable_, sizeof(*fec_enable_));
}


/** Set epon/fec_enable attribute entry.
 *
 * Set enable EPON MAC FEC for us/ds or both.
 * \param[in]   mo_ epon object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[in]   fec_enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_epon_fec_enable_set(bdmf_object_handle mo_, bdmf_index ai_, const rdpa_epon_fec_enable_t * fec_enable_)
{
    return bdmf_attrelem_set_as_buf(mo_, rdpa_epon_attr_fec_enable, (bdmf_index)ai_, fec_enable_, sizeof(*fec_enable_));
}

/** @} end of epon Doxygen group */




#endif /* _RDPA_AG_EPON_H_ */
