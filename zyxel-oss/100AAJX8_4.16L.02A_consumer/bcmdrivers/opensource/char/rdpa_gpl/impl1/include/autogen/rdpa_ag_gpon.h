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
 * gpon object header file.
 * This header file is generated automatically. Do not edit!
 */
#ifndef _RDPA_AG_GPON_H_
#define _RDPA_AG_GPON_H_

/** \addtogroup gpon
 * @{
 */


/** Get gpon type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a gpon object.
 * \return gpon type handle
 */
bdmf_type_handle rdpa_gpon_drv(void);

/* gpon: Attribute types */
typedef enum {
    rdpa_gpon_attr_link_activate = 0, /* link_activate : RW : enum/4 : PON Link_activate / deactivate */
    rdpa_gpon_attr_link_cfg = 1, /* link_cfg : RW : aggregate/64 gpon_link_cfg(rdpa_gpon_link_cfg_t) : PON Link Configuration */
    rdpa_gpon_attr_pon_link_state = 2, /* pon_link_state : R : aggregate/12 gpon_link_state(rdpa_pon_link_state) : Link state */
    rdpa_gpon_attr_onu_id = 3, /* onu_id : R : number/4 : ONU ID */
    rdpa_gpon_attr_onu_sn = 4, /* onu_sn : RW : aggregate/8 onu_sn(rdpa_onu_sn_t) : ONU serial number */
    rdpa_gpon_attr_password = 5, /* password : RW : buffer/10 : ONU password */
    rdpa_gpon_attr_user_ic = 6, /* user_ic : RW : pointer/4 : User indication callback */
    rdpa_gpon_attr_link_stat = 7, /* link_stat : R : aggregate/32 gpon_link_stat(rdpa_gpon_stat_t) : Link statistics */
    rdpa_gpon_attr_overhead_cfg = 8, /* overhead_cfg : R : aggregate/19 gpon_overhead_cfg(rdpa_gpon_overhead_cfg_t) : Overhead configuration */
    rdpa_gpon_attr_transceiver_power_level = 9, /* transceiver_power_level : R : number/1 : Transceiver power level configuration */
    rdpa_gpon_attr_equalization_delay = 10, /* equalization_delay : R : number/4 : Equalization delay */
    rdpa_gpon_attr_encryption_key = 11, /* encryption_key : R : buffer/16 : Encryption key */
    rdpa_gpon_attr_tcont_counter_assigment = 12, /* tcont_counter_assigment : RW : number/4 : TCONT counter assigment */
    rdpa_gpon_attr_ds_fec_state = 13, /* ds_fec_state : R : bool/1 : DS FEC enable state */
    rdpa_gpon_attr_fec_stat = 14, /* fec_stat : R : aggregate/12 fec_stat(rdpa_fec_stat_t) : FEC statistics */
    rdpa_gpon_attr_send_ploam = 15, /* send_ploam : W : aggregate/16 send_ploam(rdpa_send_ploam_params_t) : Send ploam */
    rdpa_gpon_attr_dba_interval = 16, /* dba_interval : RW : enum/4 : DBA Status Report */
    rdpa_gpon_attr_prbs = 17, /* prbs : RW : aggregate/10 prbs(rdpa_prbs_t) : PRBS */
    rdpa_gpon_attr_rogue_onu = 18, /* rogue_onu : RW : aggregate/16 rogue_onu(rdpa_rogue_onu_t) : rogue onu */
    rdpa_gpon_attr_gem_block_size = 19, /* gem_block_size : RW : number/4 : Gem block size */
} rdpa_gpon_attr_types;

extern int (*f_rdpa_gpon_get)(bdmf_object_handle *pmo);

/** Get gpon object.

 * This function returns gpon object instance.
 * \param[out] gpon_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_gpon_get(bdmf_object_handle *gpon_obj);

/** Get gpon/link_activate attribute.
 *
 * Get PON Link_activate / deactivate.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  link_activate_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_link_activate_get(bdmf_object_handle mo_, rdpa_link_activate_t *link_activate_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_link_activate, &_nn_);
    *link_activate_ = (rdpa_link_activate_t)_nn_;
    return _rc_;
}


/** Set gpon/link_activate attribute.
 *
 * Set PON Link_activate / deactivate.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   link_activate_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_link_activate_set(bdmf_object_handle mo_, rdpa_link_activate_t link_activate_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gpon_attr_link_activate, link_activate_);
}


/** Get gpon/link_cfg attribute.
 *
 * Get PON Link Configuration.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  link_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_link_cfg_get(bdmf_object_handle mo_, rdpa_gpon_link_cfg_t * link_cfg_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_link_cfg, link_cfg_, sizeof(*link_cfg_));
}


/** Set gpon/link_cfg attribute.
 *
 * Set PON Link Configuration.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   link_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_link_cfg_set(bdmf_object_handle mo_, const rdpa_gpon_link_cfg_t * link_cfg_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gpon_attr_link_cfg, link_cfg_, sizeof(*link_cfg_));
}


/** Get gpon/pon_link_state attribute.
 *
 * Get Link state.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  pon_link_state_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_pon_link_state_get(bdmf_object_handle mo_, rdpa_pon_link_state * pon_link_state_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_pon_link_state, pon_link_state_, sizeof(*pon_link_state_));
}


/** Get gpon/onu_id attribute.
 *
 * Get ONU ID.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  onu_id_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_onu_id_get(bdmf_object_handle mo_, bdmf_number *onu_id_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_onu_id, &_nn_);
    *onu_id_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Get gpon/onu_sn attribute.
 *
 * Get ONU serial number.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  onu_sn_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_onu_sn_get(bdmf_object_handle mo_, rdpa_onu_sn_t * onu_sn_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_onu_sn, onu_sn_, sizeof(*onu_sn_));
}


/** Set gpon/onu_sn attribute.
 *
 * Set ONU serial number.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   onu_sn_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_onu_sn_set(bdmf_object_handle mo_, const rdpa_onu_sn_t * onu_sn_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gpon_attr_onu_sn, onu_sn_, sizeof(*onu_sn_));
}


/** Get gpon/password attribute.
 *
 * Get ONU password.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  password_ Attribute value
 * \param[in]   size_ buffer size
 * \return number of bytes read >=0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_password_get(bdmf_object_handle mo_, void * password_, uint32_t size_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_password, password_, size_);
}


/** Set gpon/password attribute.
 *
 * Set ONU password.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   password_ Attribute value
 * \param[in]   size_ buffer size
 * \return number of bytes written >=0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_password_set(bdmf_object_handle mo_, const void * password_, uint32_t size_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gpon_attr_password, password_, size_);
}


/** Get gpon/user_ic attribute.
 *
 * Get User indication callback.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  user_ic_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_user_ic_get(bdmf_object_handle mo_, void * *user_ic_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_user_ic, &_nn_);
    *user_ic_ = (void *)(long)_nn_;
    return _rc_;
}


/** Set gpon/user_ic attribute.
 *
 * Set User indication callback.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   user_ic_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_user_ic_set(bdmf_object_handle mo_, void * user_ic_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gpon_attr_user_ic, (long)user_ic_);
}


/** Get gpon/link_stat attribute.
 *
 * Get Link statistics.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  link_stat_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_link_stat_get(bdmf_object_handle mo_, rdpa_gpon_stat_t * link_stat_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_link_stat, link_stat_, sizeof(*link_stat_));
}


/** Get gpon/overhead_cfg attribute.
 *
 * Get Overhead configuration.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  overhead_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_overhead_cfg_get(bdmf_object_handle mo_, rdpa_gpon_overhead_cfg_t * overhead_cfg_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_overhead_cfg, overhead_cfg_, sizeof(*overhead_cfg_));
}


/** Get gpon/transceiver_power_level attribute.
 *
 * Get Transceiver power level configuration.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  transceiver_power_level_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_transceiver_power_level_get(bdmf_object_handle mo_, bdmf_number *transceiver_power_level_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_transceiver_power_level, &_nn_);
    *transceiver_power_level_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Get gpon/equalization_delay attribute.
 *
 * Get Equalization delay.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  equalization_delay_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_equalization_delay_get(bdmf_object_handle mo_, bdmf_number *equalization_delay_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_equalization_delay, &_nn_);
    *equalization_delay_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Get gpon/encryption_key attribute.
 *
 * Get Encryption key.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  encryption_key_ Attribute value
 * \param[in]   size_ buffer size
 * \return number of bytes read >=0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_encryption_key_get(bdmf_object_handle mo_, void * encryption_key_, uint32_t size_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_encryption_key, encryption_key_, size_);
}


/** Get gpon/tcont_counter_assigment attribute.
 *
 * Get TCONT counter assigment.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  tcont_counter_assigment_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_tcont_counter_assigment_get(bdmf_object_handle mo_, bdmf_number *tcont_counter_assigment_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_tcont_counter_assigment, &_nn_);
    *tcont_counter_assigment_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set gpon/tcont_counter_assigment attribute.
 *
 * Set TCONT counter assigment.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   tcont_counter_assigment_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_tcont_counter_assigment_set(bdmf_object_handle mo_, bdmf_number tcont_counter_assigment_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gpon_attr_tcont_counter_assigment, tcont_counter_assigment_);
}


/** Get gpon/ds_fec_state attribute.
 *
 * Get DS FEC enable state.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  ds_fec_state_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_ds_fec_state_get(bdmf_object_handle mo_, bdmf_boolean *ds_fec_state_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_ds_fec_state, &_nn_);
    *ds_fec_state_ = (bdmf_boolean)_nn_;
    return _rc_;
}


/** Get gpon/fec_stat attribute.
 *
 * Get FEC statistics.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  fec_stat_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_fec_stat_get(bdmf_object_handle mo_, rdpa_fec_stat_t * fec_stat_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_fec_stat, fec_stat_, sizeof(*fec_stat_));
}


/** Set gpon/send_ploam attribute.
 *
 * Set Send ploam.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   send_ploam_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_gpon_send_ploam_set(bdmf_object_handle mo_, const rdpa_send_ploam_params_t * send_ploam_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gpon_attr_send_ploam, send_ploam_, sizeof(*send_ploam_));
}


/** Get gpon/dba_interval attribute.
 *
 * Get DBA Status Report.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  dba_interval_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_dba_interval_get(bdmf_object_handle mo_, rdpa_dba_interval_t *dba_interval_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_dba_interval, &_nn_);
    *dba_interval_ = (rdpa_dba_interval_t)_nn_;
    return _rc_;
}


/** Set gpon/dba_interval attribute.
 *
 * Set DBA Status Report.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   dba_interval_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_dba_interval_set(bdmf_object_handle mo_, rdpa_dba_interval_t dba_interval_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gpon_attr_dba_interval, dba_interval_);
}


/** Get gpon/prbs attribute.
 *
 * Get PRBS.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  prbs_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_prbs_get(bdmf_object_handle mo_, rdpa_prbs_t * prbs_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_prbs, prbs_, sizeof(*prbs_));
}


/** Set gpon/prbs attribute.
 *
 * Set PRBS.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   prbs_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_prbs_set(bdmf_object_handle mo_, const rdpa_prbs_t * prbs_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gpon_attr_prbs, prbs_, sizeof(*prbs_));
}


/** Get gpon/rogue_onu attribute.
 *
 * Get rogue onu.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  rogue_onu_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_rogue_onu_get(bdmf_object_handle mo_, rdpa_rogue_onu_t * rogue_onu_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gpon_attr_rogue_onu, rogue_onu_, sizeof(*rogue_onu_));
}


/** Set gpon/rogue_onu attribute.
 *
 * Set rogue onu.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   rogue_onu_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_rogue_onu_set(bdmf_object_handle mo_, const rdpa_rogue_onu_t * rogue_onu_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gpon_attr_rogue_onu, rogue_onu_, sizeof(*rogue_onu_));
}


/** Get gpon/gem_block_size attribute.
 *
 * Get Gem block size.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[out]  gem_block_size_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_gem_block_size_get(bdmf_object_handle mo_, bdmf_number *gem_block_size_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gpon_attr_gem_block_size, &_nn_);
    *gem_block_size_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set gpon/gem_block_size attribute.
 *
 * Set Gem block size.
 * \param[in]   mo_ gpon object handle or mattr transaction handle
 * \param[in]   gem_block_size_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gpon_gem_block_size_set(bdmf_object_handle mo_, bdmf_number gem_block_size_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gpon_attr_gem_block_size, gem_block_size_);
}

/** @} end of gpon Doxygen group */


/** \addtogroup tcont
 * @{
 */


/** Get tcont type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a tcont object.
 * \return tcont type handle
 */
bdmf_type_handle rdpa_tcont_drv(void);

/* tcont: Attribute types */
typedef enum {
    rdpa_tcont_attr_index = 0, /* index : KRI : number/4 : TCONT index */
    rdpa_tcont_attr_management = 1, /* management : RI : bool/1 : Yes: OMCI management TCONT */
    rdpa_tcont_attr_alloc_id = 2, /* alloc_id : RW : number/2 : ALLOC ID */
    rdpa_tcont_attr_assign_ploam_flag = 3, /* assign_ploam_flag : R : bool/1 : Assign alloc id message arrived */
    rdpa_tcont_attr_egress_tm = 4, /* egress_tm : RW : object/4 : US scheduler object */
    rdpa_tcont_attr_enable = 5, /* enable : RW : bool/1 : Enable TCONT */
    rdpa_tcont_attr_flush = 6, /* flush : W : bool/1 : Flush TCONT */
    rdpa_tcont_attr_orl_prty = 7, /* orl_prty : RW : enum/4 : Priority for overall rate limiter */
    rdpa_tcont_attr_stat = 8, /* stat : R : aggregate/16 tcont_stat(rdpa_tcont_stat_t) : TCONT statistics */
} rdpa_tcont_attr_types;

extern int (*f_rdpa_tcont_get)(bdmf_number index_, bdmf_object_handle *pmo);

/** Get tcont object by key.

 * This function returns tcont object instance by key.
 * \param[in] index_    Object key
 * \param[out] tcont_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_tcont_get(bdmf_number index_, bdmf_object_handle *tcont_obj);

/** Get tcont/index attribute.
 *
 * Get TCONT index.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  index_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_tcont_index_get(bdmf_object_handle mo_, bdmf_number *index_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_tcont_attr_index, &_nn_);
    *index_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set tcont/index attribute.
 *
 * Set TCONT index.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[in]   index_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_tcont_index_set(bdmf_object_handle mo_, bdmf_number index_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_tcont_attr_index, index_);
}


/** Get tcont/management attribute.
 *
 * Get Yes: OMCI management TCONT.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  management_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_tcont_management_get(bdmf_object_handle mo_, bdmf_boolean *management_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_tcont_attr_management, &_nn_);
    *management_ = (bdmf_boolean)_nn_;
    return _rc_;
}


/** Set tcont/management attribute.
 *
 * Set Yes: OMCI management TCONT.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[in]   management_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_tcont_management_set(bdmf_object_handle mo_, bdmf_boolean management_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_tcont_attr_management, management_);
}


/** Get tcont/alloc_id attribute.
 *
 * Get ALLOC ID.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  alloc_id_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_alloc_id_get(bdmf_object_handle mo_, bdmf_number *alloc_id_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_tcont_attr_alloc_id, &_nn_);
    *alloc_id_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set tcont/alloc_id attribute.
 *
 * Set ALLOC ID.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[in]   alloc_id_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_alloc_id_set(bdmf_object_handle mo_, bdmf_number alloc_id_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_tcont_attr_alloc_id, alloc_id_);
}


/** Get tcont/assign_ploam_flag attribute.
 *
 * Get Assign alloc id message arrived.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  assign_ploam_flag_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_assign_ploam_flag_get(bdmf_object_handle mo_, bdmf_boolean *assign_ploam_flag_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_tcont_attr_assign_ploam_flag, &_nn_);
    *assign_ploam_flag_ = (bdmf_boolean)_nn_;
    return _rc_;
}


/** Get tcont/egress_tm attribute.
 *
 * Get US scheduler object.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  egress_tm_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_egress_tm_get(bdmf_object_handle mo_, bdmf_object_handle *egress_tm_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_tcont_attr_egress_tm, &_nn_);
    *egress_tm_ = (bdmf_object_handle)(long)_nn_;
    return _rc_;
}


/** Set tcont/egress_tm attribute.
 *
 * Set US scheduler object.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[in]   egress_tm_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_egress_tm_set(bdmf_object_handle mo_, bdmf_object_handle egress_tm_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_tcont_attr_egress_tm, (long)egress_tm_);
}


/** Get tcont/enable attribute.
 *
 * Get Enable TCONT.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_tcont_enable_get(bdmf_object_handle mo_, bdmf_boolean *enable_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_tcont_attr_enable, &_nn_);
    *enable_ = (bdmf_boolean)_nn_;
    return _rc_;
}


/** Set tcont/enable attribute.
 *
 * Set Enable TCONT.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[in]   enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_tcont_enable_set(bdmf_object_handle mo_, bdmf_boolean enable_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_tcont_attr_enable, enable_);
}


/** Set tcont/flush attribute.
 *
 * Set Flush TCONT.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[in]   flush_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_flush_set(bdmf_object_handle mo_, bdmf_boolean flush_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_tcont_attr_flush, flush_);
}


/** Get tcont/orl_prty attribute.
 *
 * Get Priority for overall rate limiter.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  orl_prty_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_orl_prty_get(bdmf_object_handle mo_, rdpa_tm_orl_prty *orl_prty_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_tcont_attr_orl_prty, &_nn_);
    *orl_prty_ = (rdpa_tm_orl_prty)_nn_;
    return _rc_;
}


/** Set tcont/orl_prty attribute.
 *
 * Set Priority for overall rate limiter.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[in]   orl_prty_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_orl_prty_set(bdmf_object_handle mo_, rdpa_tm_orl_prty orl_prty_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_tcont_attr_orl_prty, orl_prty_);
}


/** Get tcont/stat attribute.
 *
 * Get TCONT statistics.
 * \param[in]   mo_ tcont object handle or mattr transaction handle
 * \param[out]  stat_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_tcont_stat_get(bdmf_object_handle mo_, rdpa_tcont_stat_t * stat_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_tcont_attr_stat, stat_, sizeof(*stat_));
}

/** @} end of tcont Doxygen group */


/** \addtogroup gem
 * @{
 */


/** Get gem type handle.
 *
 * This handle should be passed to bdmf_new_and_set() function in
 * order to create a gem object.
 * \return gem type handle
 */
bdmf_type_handle rdpa_gem_drv(void);

/* gem: Attribute types */
typedef enum {
    rdpa_gem_attr_index = 0, /* index : KRI : number/4 : GEM index */
    rdpa_gem_attr_gem_port = 1, /* gem_port : MRI : number/2 : GEM port ID */
    rdpa_gem_attr_flow_type = 2, /* flow_type : RI : enum/4 : GEM flow type */
    rdpa_gem_attr_ds_encryption = 3, /* ds_encryption : RW : bool/1 : downstream GEM flow encryption mode */
    rdpa_gem_attr_enable = 4, /* enable : RW : bool/1 : enable gem */
    rdpa_gem_attr_ds_def_flow = 5, /* ds_def_flow : RW : aggregate/56 classification_result(rdpa_ic_result_t ) : downstream default flow configuration */
    rdpa_gem_attr_port_action = 6, /* port_action : RWF : aggregate/5[49(rdpa_if)] gem_port_action(rdpa_gem_port_action_t) : Per port vlan action configuration */
    rdpa_gem_attr_us_cfg = 7, /* us_cfg : RW : aggregate/4 gem_us_cfg(rdpa_gem_flow_us_cfg_t) : upstream GEM configuration */
    rdpa_gem_attr_ds_cfg = 8, /* ds_cfg : RW : aggregate/8 gem_ds_cfg(rdpa_gem_flow_ds_cfg_t) : downstream GEM configuration */
    rdpa_gem_attr_stat = 9, /* stat : R : aggregate/24 gem_stat(rdpa_gem_stat_t) : GEM statistics */
    rdpa_gem_attr_accumulate_stat = 10, /* accumulate_stat : R : aggregate/24 gem_stat(rdpa_gem_stat_t) : Accumulate GEM statistics */
} rdpa_gem_attr_types;

extern int (*f_rdpa_gem_get)(bdmf_number index_, bdmf_object_handle *pmo);

/** Get gem object by key.

 * This function returns gem object instance by key.
 * \param[in] index_    Object key
 * \param[out] gem_obj    Object handle
 * \return    0=OK or error <0
 */
int rdpa_gem_get(bdmf_number index_, bdmf_object_handle *gem_obj);

/** Get gem/index attribute.
 *
 * Get GEM index.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  index_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_gem_index_get(bdmf_object_handle mo_, bdmf_number *index_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gem_attr_index, &_nn_);
    *index_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set gem/index attribute.
 *
 * Set GEM index.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   index_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_gem_index_set(bdmf_object_handle mo_, bdmf_number index_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gem_attr_index, index_);
}


/** Get gem/gem_port attribute.
 *
 * Get GEM port ID.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  gem_port_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_gem_gem_port_get(bdmf_object_handle mo_, bdmf_number *gem_port_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gem_attr_gem_port, &_nn_);
    *gem_port_ = (bdmf_number)_nn_;
    return _rc_;
}


/** Set gem/gem_port attribute.
 *
 * Set GEM port ID.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   gem_port_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_gem_gem_port_set(bdmf_object_handle mo_, bdmf_number gem_port_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gem_attr_gem_port, gem_port_);
}


/** Get gem/flow_type attribute.
 *
 * Get GEM flow type.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  flow_type_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_gem_flow_type_get(bdmf_object_handle mo_, rdpa_gem_flow_type *flow_type_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gem_attr_flow_type, &_nn_);
    *flow_type_ = (rdpa_gem_flow_type)_nn_;
    return _rc_;
}


/** Set gem/flow_type attribute.
 *
 * Set GEM flow type.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   flow_type_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task and softirq contexts.
 */
static inline int rdpa_gem_flow_type_set(bdmf_object_handle mo_, rdpa_gem_flow_type flow_type_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gem_attr_flow_type, flow_type_);
}


/** Get gem/ds_encryption attribute.
 *
 * Get downstream GEM flow encryption mode.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  ds_encryption_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_ds_encryption_get(bdmf_object_handle mo_, bdmf_boolean *ds_encryption_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gem_attr_ds_encryption, &_nn_);
    *ds_encryption_ = (bdmf_boolean)_nn_;
    return _rc_;
}


/** Set gem/ds_encryption attribute.
 *
 * Set downstream GEM flow encryption mode.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   ds_encryption_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_ds_encryption_set(bdmf_object_handle mo_, bdmf_boolean ds_encryption_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gem_attr_ds_encryption, ds_encryption_);
}


/** Get gem/enable attribute.
 *
 * Get enable gem.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_enable_get(bdmf_object_handle mo_, bdmf_boolean *enable_)
{
    bdmf_number _nn_;
    int _rc_;
    _rc_ = bdmf_attr_get_as_num(mo_, rdpa_gem_attr_enable, &_nn_);
    *enable_ = (bdmf_boolean)_nn_;
    return _rc_;
}


/** Set gem/enable attribute.
 *
 * Set enable gem.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   enable_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_enable_set(bdmf_object_handle mo_, bdmf_boolean enable_)
{
    return bdmf_attr_set_as_num(mo_, rdpa_gem_attr_enable, enable_);
}


/** Get gem/ds_def_flow attribute.
 *
 * Get downstream default flow configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  ds_def_flow_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_ds_def_flow_get(bdmf_object_handle mo_, rdpa_ic_result_t  * ds_def_flow_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gem_attr_ds_def_flow, ds_def_flow_, sizeof(*ds_def_flow_));
}


/** Set gem/ds_def_flow attribute.
 *
 * Set downstream default flow configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   ds_def_flow_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_ds_def_flow_set(bdmf_object_handle mo_, const rdpa_ic_result_t  * ds_def_flow_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gem_attr_ds_def_flow, ds_def_flow_, sizeof(*ds_def_flow_));
}


/** Get gem/port_action attribute entry.
 *
 * Get Per port vlan action configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[out]  port_action_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_port_action_get(bdmf_object_handle mo_, rdpa_if ai_, rdpa_gem_port_action_t * port_action_)
{
    return bdmf_attrelem_get_as_buf(mo_, rdpa_gem_attr_port_action, (bdmf_index)ai_, port_action_, sizeof(*port_action_));
}


/** Set gem/port_action attribute entry.
 *
 * Set Per port vlan action configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   ai_ Attribute array index
 * \param[in]   port_action_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_port_action_set(bdmf_object_handle mo_, rdpa_if ai_, const rdpa_gem_port_action_t * port_action_)
{
    return bdmf_attrelem_set_as_buf(mo_, rdpa_gem_attr_port_action, (bdmf_index)ai_, port_action_, sizeof(*port_action_));
}


/** Get gem/us_cfg attribute.
 *
 * Get upstream GEM configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  us_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_us_cfg_get(bdmf_object_handle mo_, rdpa_gem_flow_us_cfg_t * us_cfg_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gem_attr_us_cfg, us_cfg_, sizeof(*us_cfg_));
}


/** Set gem/us_cfg attribute.
 *
 * Set upstream GEM configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   us_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_us_cfg_set(bdmf_object_handle mo_, const rdpa_gem_flow_us_cfg_t * us_cfg_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gem_attr_us_cfg, us_cfg_, sizeof(*us_cfg_));
}


/** Get gem/ds_cfg attribute.
 *
 * Get downstream GEM configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  ds_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_ds_cfg_get(bdmf_object_handle mo_, rdpa_gem_flow_ds_cfg_t * ds_cfg_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gem_attr_ds_cfg, ds_cfg_, sizeof(*ds_cfg_));
}


/** Set gem/ds_cfg attribute.
 *
 * Set downstream GEM configuration.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[in]   ds_cfg_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_ds_cfg_set(bdmf_object_handle mo_, const rdpa_gem_flow_ds_cfg_t * ds_cfg_)
{
    return bdmf_attr_set_as_buf(mo_, rdpa_gem_attr_ds_cfg, ds_cfg_, sizeof(*ds_cfg_));
}


/** Get gem/stat attribute.
 *
 * Get GEM statistics.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  stat_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_stat_get(bdmf_object_handle mo_, rdpa_gem_stat_t * stat_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gem_attr_stat, stat_, sizeof(*stat_));
}


/** Get gem/accumulate_stat attribute.
 *
 * Get Accumulate GEM statistics.
 * \param[in]   mo_ gem object handle or mattr transaction handle
 * \param[out]  accumulate_stat_ Attribute value
 * \return 0 or error code < 0
 * The function can be called in task context only.
 */
static inline int rdpa_gem_accumulate_stat_get(bdmf_object_handle mo_, rdpa_gem_stat_t * accumulate_stat_)
{
    return bdmf_attr_get_as_buf(mo_, rdpa_gem_attr_accumulate_stat, accumulate_stat_, sizeof(*accumulate_stat_));
}

/** @} end of gem Doxygen group */




#endif /* _RDPA_AG_GPON_H_ */
