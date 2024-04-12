// <:copyright-BRCM:2014:DUAL/GPL:standard
// 
//    Copyright (c) 2014 Broadcom Corporation
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
 * spdsvc object header file.
 * This header file is generated automatically. Do not edit!
 */
#ifndef _RDPA_AG_SPDSVC_H_
#define _RDPA_AG_SPDSVC_H_

/** \addtogroup spdsvc
 * @{
 */


/** Get spdsvc type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a spdsvc object.
 * \return spdsvc type handle
 */
bdmf_type_handle rdpa_spdsvc_drv(void);

/* spdsvc: Attribute types */
typedef enum {
    rdpa_spdsvc_attr_local_ip_addr = 0, /* local_ip_addr : MRI : ip/20 : Local IP Address */
    rdpa_spdsvc_attr_local_port_nbr = 1, /* local_port_nbr : MRI : number/2 : Local UDP Port Number */
    rdpa_spdsvc_attr_remote_ip_addr = 2, /* remote_ip_addr : MRI : ip/20 : Remote IP Address */
    rdpa_spdsvc_attr_remote_port_nbr = 3, /* remote_port_nbr : MRI : number/2 : Remote UDP Port Number */
    rdpa_spdsvc_attr_direction = 4, /* direction : MRI : enum/4 : Test Direction */
    rdpa_spdsvc_attr_flow_index = 5, /* flow_index : R : number/4 : Analyzer Flow Index */
    rdpa_spdsvc_attr_result = 6, /* result : RF : aggregate/20[2] spdsvc_result(rdpa_spdsvc_result_t) : Test Results */
} rdpa_spdsvc_attr_types;

extern int (*f_rdpa_spdsvc_get)(bdmf_object_handle *pmo);

/** Get spdsvc object.

 * This function returns spdsvc object instance.
 * \param[out] spdsvc_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_spdsvc_get(bdmf_object_handle *spdsvc_obj);

/** Get spdsvc/local_ip_addr attribute.
 *
 * Get Local IP Address.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[out]  local_ip_addr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_local_ip_addr_get(bdmf_object_handle mo_, bdmf_ip_t * local_ip_addr_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_spdsvc_attr_local_ip_addr, local_ip_addr_, sizeof(*local_ip_addr_));
}


/** Set spdsvc/local_ip_addr attribute.
 *
 * Set Local IP Address.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[in]   local_ip_addr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_local_ip_addr_set(bdmf_object_handle mo_, const bdmf_ip_t * local_ip_addr_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_spdsvc_attr_local_ip_addr, local_ip_addr_, sizeof(*local_ip_addr_));
}


/** Get spdsvc/local_port_nbr attribute.
 *
 * Get Local UDP Port Number.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[out]  local_port_nbr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_local_port_nbr_get(bdmf_object_handle mo_, bdmf_number *local_port_nbr_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_spdsvc_attr_local_port_nbr, &_nn_);
    *local_port_nbr_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set spdsvc/local_port_nbr attribute.
 *
 * Set Local UDP Port Number.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[in]   local_port_nbr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_local_port_nbr_set(bdmf_object_handle mo_, bdmf_number local_port_nbr_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_spdsvc_attr_local_port_nbr, local_port_nbr_);
}


/** Get spdsvc/remote_ip_addr attribute.
 *
 * Get Remote IP Address.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[out]  remote_ip_addr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_remote_ip_addr_get(bdmf_object_handle mo_, bdmf_ip_t * remote_ip_addr_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_spdsvc_attr_remote_ip_addr, remote_ip_addr_, sizeof(*remote_ip_addr_));
}


/** Set spdsvc/remote_ip_addr attribute.
 *
 * Set Remote IP Address.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[in]   remote_ip_addr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_remote_ip_addr_set(bdmf_object_handle mo_, const bdmf_ip_t * remote_ip_addr_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_spdsvc_attr_remote_ip_addr, remote_ip_addr_, sizeof(*remote_ip_addr_));
}


/** Get spdsvc/remote_port_nbr attribute.
 *
 * Get Remote UDP Port Number.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[out]  remote_port_nbr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_remote_port_nbr_get(bdmf_object_handle mo_, bdmf_number *remote_port_nbr_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_spdsvc_attr_remote_port_nbr, &_nn_);
    *remote_port_nbr_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set spdsvc/remote_port_nbr attribute.
 *
 * Set Remote UDP Port Number.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[in]   remote_port_nbr_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_remote_port_nbr_set(bdmf_object_handle mo_, bdmf_number remote_port_nbr_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_spdsvc_attr_remote_port_nbr, remote_port_nbr_);
}


/** Get spdsvc/direction attribute.
 *
 * Get Test Direction.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[out]  direction_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_direction_get(bdmf_object_handle mo_, rdpa_traffic_dir *direction_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_spdsvc_attr_direction, &_nn_);
    *direction_ = (rdpa_traffic_dir)_nn_;
    return _rc_;
}


/** Set spdsvc/direction attribute.
 *
 * Set Test Direction.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[in]   direction_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_direction_set(bdmf_object_handle mo_, rdpa_traffic_dir direction_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_spdsvc_attr_direction, direction_);
}


/** Get spdsvc/flow_index attribute.
 *
 * Get Analyzer Flow Index.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[out]  flow_index_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_spdsvc_flow_index_get(bdmf_object_handle mo_, bdmf_number *flow_index_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_spdsvc_attr_flow_index, &_nn_);
    *flow_index_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Get spdsvc/result attribute entry.
 *
 * Get Test Results.
 * \param[in]   mo_ spdsvc object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  result_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_spdsvc_result_get(bdmf_object_handle mo_, bdmf_index ai_, rdpa_spdsvc_result_t * result_)
{
    return bdmf_attrelem_get_as_buf(mo_, rdpa_spdsvc_attr_result, (bdmf_index)ai_, result_, sizeof(*result_));
}

/** @} end of spdsvc Doxygen group */




#endif /* _RDPA_AG_SPDSVC_H_ */
