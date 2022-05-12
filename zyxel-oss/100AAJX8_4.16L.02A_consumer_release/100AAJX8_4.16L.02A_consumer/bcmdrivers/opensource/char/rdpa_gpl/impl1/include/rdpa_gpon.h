/*
* <:copyright-BRCM:2013:DUAL/GPL:standard
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


#ifndef _RDPA_GPON_H_
#define _RDPA_GPON_H_

/** \defgroup xgpon xGPON Management
 * Objects in this group control xGPON-related configuration
 */

/**
 * \defgroup gpon Link Management
 * \ingroup xgpon
 * Objects and functions in this group are used for configuration and
 * status monitoring on PON link and ONU level:
 * - PON link configuration
 * - PON link statistics
 * - ONU SN and password configuration
 * @{
 */

 #define RDPA_PLOAM_LENGTH 12

/** Link Polarity */
typedef enum {
    rdpa_polarity_active_low,       /**< Active low */
    rdpa_polarity_active_high,      /**< Active high */
} rdpa_polarity;

/** GPON Link Configuration */
typedef struct
{
    uint32_t to1_timeout;                           /**< TO1 timer */
    uint32_t to2_timeout;                           /**< TO2 timer */
    uint32_t ber_interval;                          /**< Ber interval */
    uint32_t min_response_time;                     /**< Min response time */
    bdmf_boolean tx_data_polarity;                  /**< Tx data polarity */
    uint32_t dv_setup_pattern;                      /**< Transceiver DV setup pattern */
    uint32_t dv_hold_pattern;                       /**< Transceiver DV hold pattern */
    rdpa_polarity dv_polarity;                      /**< Transceiver DV polarity */
    bdmf_boolean power_calibration_mode;            /**< Transceiver power calibration mode */
    uint32_t power_calibration_pattern;             /**< Transceiver power calibration pattern */
    uint32_t power_calibration_size;                /**< Transceiver power calibration size */
    bdmf_boolean rx_din_polarity;                   /**< RX Data Polarity */
    uint32_t ber_threshold_for_sd_assertion;        /**< BER threshold for SD assertion */
    uint32_t ber_threshold_for_sf_assertion;        /**< BER threshold for SF assertion */
    uint32_t number_of_psyncs_for_lof_assertion;    /**< Number of PSYNCs for LOF assertion */
    uint32_t number_of_psyncs_for_lof_clear;        /**< Number of PSYNCs for LOF clear */
} rdpa_gpon_link_cfg_t;

/** GPON Overhead Configuration */
typedef struct
{
    uint8_t overhead[16];                           /**< Overhead */
    uint8_t overhead_len;                           /**< Overhead length */
    uint8_t overhead_repetition;                    /**< Overhead repetition */
    uint8_t overhead_repetition_len;                /**< Overhead repetition length */
}rdpa_gpon_overhead_cfg_t;

/** PON indications */
typedef enum
{
    rdpa_indication_lof,                            /**< LOF indication */
    rdpa_indication_err,                            /**< ERR indication */
    rdpa_indication_sf,                             /**< SF indication */
    rdpa_indication_sd,                             /**< SD indication */
    rdpa_indication_lcdg,                           /**< LCDG indication */
    rdpa_indication_dact,                           /**< DACT indication */
    rdpa_indication_dis,                            /**< DIS indication */
    rdpa_indication_pee,                            /**< PEE indication */
    rdpa_indication_mem,                            /**< MEM indication */ 
    rdpa_indication_pst_msg,                        /**< PST Message indication */
    rdpa_indication_ranging_start,                  /**< Ranging start indication */
    rdpa_indication_ranging_stop,                   /**< Ranging stop indication */
    rdpa_indication_assign_alloc_id_msg,            /**< Assign Alloc ID Message indication */
    rdpa_indication_cfg_port_id_msg,                /**< Configure Port ID Message indication */
    rdpa_indication_encrypted_port_id_msg,          /**< Encrypted Port ID Message indication */
    rdpa_indication_key_switched_out_of_sync,       /**< Key switched out of sync indication */
    rdpa_indication_key_switched,                   /**< Key switched indication */
    rdpa_indication_state_transition,               /**< State transition indication */
    rdpa_indication_assign_onu_id_msg,              /**< Assign ONU ID Message indication */
    rdpa_indication_link_state_transition,          /**< Link state transition indication */
    rdpa_indication_rouge_onu,                      /**< Rouge ONU indication */
    rdpa_indication_none = 0xffff
}rdpa_pon_indication;

/** PON link administrative state */
typedef enum {
    rdpa_pon_admin_state_not_ready,     /**< Not ready */
    rdpa_pon_admin_state_inactive,      /**< Inactive */
    rdpa_pon_admin_state_active         /**< Active */
} rdpa_pon_admin_state;

/** PON link sub state */
typedef enum {
    rdpa_pon_oper_state_none,               /**< Not set */
    rdpa_pon_oper_state_standby,            /**< Standby */
    rdpa_pon_oper_state_operational,        /**< Operational */
    rdpa_pon_oper_state_stop                /**< Stop */
}  rdpa_pon_sub_state;

/** PON link Operational state */
typedef enum {
    rdpa_pon_init_o1,          /**< o1 */
    rdpa_pon_standby_o2,       /**< standby_o2*/
    rdpa_pon_serial_num_o3,    /**< serial_num_o3*/
    rdpa_pon_ranging_o4,       /**< ranging_o4*/
    rdpa_pon_operational_o5,   /**< operational_o5*/
    rdpa_pon_popup_o6,         /**< popup_o6*/
    rdpa_pon_emergency_stop_o7,/**< emergency_stop_o7*/
}  rdpa_pon_link_operational_state;

/** PON Link state */
typedef struct{
    rdpa_pon_admin_state admin_state;                   /**< Administrative state */
    rdpa_pon_sub_state sub_state;                       /**< Sub state */       
    rdpa_pon_link_operational_state link_oper_state;    /**< Link operational state */
}rdpa_pon_link_state;

/** Ranging stop reason */
typedef enum
{
    rdpa_ranging_stop_to1_timeout,              /** TO1  timeout */
    rdpa_ranging_stop_link_folt,                /** Link folt */
    rdpa_ranging_stop_deactivate_onu,           /** Deactivate ONU */
    rdpa_ranging_stop_disable_onu,              /** Disable ONU */
    rdpa_ranging_stop_successful_ranging,       /** Successful ranging */
    rdpa_ranging_stop_unmatch_sn,               /** Unmatched serial number */
    rdpa_ranging_stop_link_deactivate           /** Link deactivated */
}rdpa_ranging_stop_reason;

/** Pon status indication enable*/
typedef enum
{
    rdpa_indication_on,                         /** Indication enabled */
    rdpa_indication_off                         /** Indication disabled */

}rdpa_pon_status_indication;

typedef struct
{
    uint32_t alloc_id ;
    rdpa_pon_status_indication status;
}rdpa_alloc_id_param;

typedef struct
{
    uint32_t port_id;
    rdpa_pon_status_indication status;
}rdpa_cfg_port_id_param;

typedef struct
{
    uint32_t port_id;
    bdmf_boolean encryption;
}rdpa_encrypted_port_id_param;

typedef struct
{
    uint32_t current_superframe ;
    uint32_t switch_over_superframe ;
}rdpa_key_switch_param;

typedef struct
{
    rdpa_pon_sub_state old_state;
    rdpa_pon_sub_state new_state;
}rdpa_oper_state_param;

typedef struct
{
    rdpa_pon_link_operational_state old_state;
    rdpa_pon_link_operational_state new_state;
}rdpa_link_state_param;

typedef enum
{
    rdpa_monitor_rouge_mode,
    rdpa_fault_rouge_mode
}rdpa_rouge_onu_mode;

typedef struct
{
    bdmf_boolean status;
    rdpa_rouge_onu_mode type;
}rdpa_rlouge_onu_param;

typedef enum
{
    rdpa_pon_tx,
    rdpa_pon_rx,
    rdpa_pon_none
}
rdpa_pon_transmit;

typedef union
{
    rdpa_pon_status_indication lof_status;                      /* LOF status */
    uint32_t bip8_errors;                                       /* ERR parameters */
    rdpa_pon_status_indication sf_status;                       /* SF status */
    rdpa_pon_status_indication sd_status;                       /* SD status */
    rdpa_pon_status_indication lcdg_status;                     /* LCDG status */
    rdpa_pon_status_indication dis_status;                      /* DIS status */
    uint8_t unkown_ploam_mem[RDPA_PLOAM_LENGTH];                /* MEM parameters */
    uint8_t pst_ploam[RDPA_PLOAM_LENGTH];                       /* PST parameters */
    rdpa_ranging_stop_reason ranging_stop;                      /* Ranging stop parameters */
    rdpa_alloc_id_param assign_alloc_id_parameters;             /* Assign alloc ID parameters */
    uint8_t onu_id;                                             /* Assign ONU ID parameters */
    rdpa_cfg_port_id_param configure_port_id_parameters;        /* Configure port ID parameters */
    rdpa_encrypted_port_id_param encrypted_port_id_parameters;  /* Encrypted port ID parameters */
    rdpa_key_switch_param key_switch_parameters;                /* Key switch parameters */
    rdpa_oper_state_param state_transition_parameters;          /* State transition parameters */
    rdpa_link_state_param link_state_transition_parameters;     /* Link State transition parameters */
    rdpa_rlouge_onu_param rogue_onu ;                           /* Rogue ONU parameters */
}
rdpa_callback_indication;

/** PON-level counters.
 * Underlying type for gpon_stat aggregate
 */
typedef struct {
    uint32_t bip_errors;        /**< BIP error counter */
    uint32_t crc_errors;        /**< PLOAM CRC errors */
    uint32_t rx_onu_id;         /**< Valid ONU id PLOAM counter */
    uint32_t rx_broadcast;      /**< Broadcast PLOAM counter */
    uint32_t rx_unknown;        /**< Rx unknown PLOAM counter */
    uint32_t tx_illegal_access; /**< Tx illegal access PLOAM counter */
    uint32_t tx_idle;           /**< Tx idle PLOAM counter */
    uint32_t tx_ploam;         /**< Tx PLOAM counter */
} rdpa_gpon_stat_t;

/** ONU serial number.
 * Underlying type for onu_sn aggregate
 */
typedef struct {
    uint8_t vendor_id[4];       /**< Vendor id */
    uint8_t vendor_specific[4]; /**< Vendor-specific number */
} rdpa_onu_sn_t;

/** ONU password */
typedef struct {
    uint8_t password[10];
} rdpa_onu_password_t;

/** @} end of pon Doxygen group */

/**
 * \defgroup tcont T-CONT Management
 * \ingroup xgpon
 * @{
 */

#define RDPA_MAX_TCONT  32      /**< Max number of T-CONTs */

/** T-CONT statistics.
 * Underlying type for tcont_stat aggregate
 */
typedef struct {
    uint32_t idle_gem_counter;
    uint32_t non_idle_gem_counter;
    uint32_t packet_counter;
    uint32_t valid_access_counter;
} rdpa_tcont_stat_t;

/** @} end of tcont Doxygen group */

/**
 * \defgroup gem GEM Flow Management
 * \ingroup xgpon
 * @{
 */

#if defined(G9991) || defined(BRCM_FTTDP)
#define G9991_SID_PORTS_DS 17
#define RDPA_MAX_GEM_FLOW (256 - G9991_SID_PORTS_DS) /**< Max number of GEM flows */
#else
#define RDPA_MAX_GEM_FLOW 256 /**< Max number of GEM flows */
#endif

/** GEM flow type */
typedef enum {
    rdpa_gem_flow_ethernet,         /**< Ethernet flow. MAC calculates Ethernet FCS */
    rdpa_gem_flow_omci              /**< OMCI flow */
} rdpa_gem_flow_type;

/** GEM flow US configuration.
 * Underlying type for gem_us_cfg aggregate
 */
typedef struct {
    bdmf_object_handle tcont;       /**< T-CONT id */
} rdpa_gem_flow_us_cfg_t;

/** GEM flow DS configuration.
 * Underlying type for gem_flow_ds_cfg_t aggregate
 */
typedef struct {
    rdpa_discard_prty discard_prty;     /**< Discard priority */
    rdpa_flow_destination destination;  /**< Flow destination */
} rdpa_gem_flow_ds_cfg_t;

/** GEM flow statistics */
typedef struct {
    uint32_t rx_packets;            /**< Rx Packets */
    uint32_t rx_bytes;              /**< Rx Bytes */
    uint32_t tx_packets;            /**< Tx Packets */
    uint32_t tx_bytes;              /**< Tx bytes */
    uint32_t rx_packets_discard;    /**< Rx Packet discard */  
    uint32_t tx_packets_discard;    /**< Tx Packet discard */ 
} rdpa_gem_stat_t;

/** PON FEC statistics */
typedef struct
{
    uint32_t corrected_bytes;          /**< FEC corrected bytes */
    uint32_t corrected_codewords;      /**< FEC corrected codewords */
    uint32_t uncorrectable_codewords;  /**< FEC uncorrectable codewords */
} rdpa_fec_stat_t;

/** Send PLOAM type */
typedef enum
{
    gpon_ploam_type_dgasp, /**< Dying Gasp */
    gpon_ploam_type_pee, /**< PEE */
    gpon_ploam_type_pst, /**< PST */
} rdpa_ploam_type_t;

/** PST parameters */
typedef struct
{
    uint32_t k1_value; /**< K1 */
    uint32_t k2_value; /**< K2 */
    uint32_t line_number; /**< Line number */
} rdpa_pst_params_t;

/** Send PLOAM parameters */
typedef struct
{
    rdpa_ploam_type_t ploam_type;  /**< PLOAM Type: Dying Gasp / PEE / PST */
    rdpa_pst_params_t pst_params; /**< PST parameters: K1, K2, Line number */
} rdpa_send_ploam_params_t;

/** DBA status report */
typedef enum
{
    gpon_dba_disable, /**< Disable  */
    gpon_dba_interval_1_msec, /**< 1 milisec */
    gpon_dba_interval_2_msec, /**< 2 milisec */
    gpon_dba_interval_3_msec, /**< 3 milisec */
    gpon_dba_interval_4_msec, /**< 4 milisec */
    gpon_dba_interval_5_msec, /**< 5 milisec */
    gpon_dba_interval_6_msec, /**< 6 milisec */
    gpon_dba_interval_7_msec, /**< 7 milisec */
    gpon_dba_interval_8_msec, /**< 8 milisec */
    gpon_dba_interval_9_msec, /**< 9 milisec */
    gpon_dba_interval_10_msec, /**< 10 milisec */
    gpon_dba_interval_125_usec = 125, /**< 125 microsec */
    gpon_dba_interval_250_usec = 250, /**< 250 microsec */
    gpon_dba_interval_500_usec = 500, /**< 500 microsec */
} rdpa_dba_interval_t;

/** PRBS modes */
typedef enum
{
    rdpa_prbs_mode_prbs7,
    rdpa_prbs_mode_prbs15,
    rdpa_prbs_mode_prbs23,
    rdpa_prbs_mode_prbs31,
    rdpa_prbs_mode_alternate,
} rdpa_prbs_mode_t;

/** US PRBS generator */
typedef enum
{
   rdpa_us_prbs_disable,
   rdpa_us_prbs_per_access,
   rdpa_us_prbs_constant,
} rdpa_prbs_us_generator_cfg_t;

/** PRBS ds_comparator_valid struct */
typedef struct
{
    bdmf_boolean cfg; /**< DS PRBS comparator: disable / enable */
    bdmf_boolean status; /**< DS PRBS comparator signal validation: valid/not valid */
}rdpa_prbs_ds_comperator_cfg_t;


/** PRBS parameters */
typedef struct
{
    rdpa_prbs_mode_t mode;  /**< PRBS Mode: PRBS7 / PRBS15 / PRBS23 / PRBS31 / ALTERNATE */
    rdpa_prbs_us_generator_cfg_t us_cfg; /**< US PRBS generator: disable / enable_per_access / enable_constant */
    rdpa_prbs_ds_comperator_cfg_t ds_comparator_cfg; /**< DS PRBS comparator cfg */
} rdpa_prbs_t;

/** Rogue ONU Detection Mode */
typedef enum
{
    rdpa_rogue_tx_monitor_mode,
    rdpa_rogue_tx_fault_mode,
} rdpa_rogue_mode_t;

/** Rogue ONU Default Pin Configuration */
typedef struct
{
    uint32_t default_pin_mode_reg;  /**< Default pin mode cfg */
    uint32_t default_loop_sel_cfg;  /**< Default loop select reg configuration */
    uint32_t default_pin_mux_data;  /**< Default pin mux data */
} rdpa_rogue_default_pin_cfg_t;

/** Rogue ONU parameters */
typedef struct
{
    bdmf_boolean enable;  /**< Rogue State: disable / enable */
    rdpa_rogue_mode_t mode; /**< Rogue ONU Detection Mode: TX_MONITOR / TX_FAULT */
    uint32_t clock_cycle; /**< Rogue ONU window_size in clock cycles */
    uint32_t gpio_pin; /**< Rogue ONU the wanted GPIO for this feature,
                            this GPIO should be connected to the tx monitor/error transceiver pin */
    rdpa_rogue_default_pin_cfg_t default_pin_cfg; /**< Rogue ONU Default Pin Configuration */
} rdpa_rogue_onu_t;

/** Link activate / deactivate */
typedef enum
{
    rdpa_link_deactivate, /**< Deactivate */
    rdpa_link_activate_O1, /**< Activate: init at initial state */
    rdpa_link_activate_O7, /**< Activate: init at Emergency stop state */
} rdpa_link_activate_t;

/** gem def flow per port action */
typedef struct{
    bdmf_object_handle vlan_action; /**< VLAN action object */
    bdmf_boolean drop; /**< Drop action - true/false */
} rdpa_gem_port_action_t;

/** @} end of gem Doxygen group */

/* gem object private data */
typedef struct {
    bdmf_index index;               /**< Gem index */
    uint16_t gem_port;              /**< GEM port (external number) */
    rdpa_gem_flow_type type;        /**< Flow type: Ethernet, OMCI, etc. */
    bdmf_boolean ds_encryption;        /**< True=encrypted */
    bdmf_boolean enable;    /**< True=enable */
    bdmf_index ds_def_flow;  /**Gem default flow configuration index*/
    rdpa_gem_flow_ds_cfg_t ds_cfg;  /**< GEM DS configuration */
    rdpa_gem_flow_us_cfg_t us_cfg;  /**< GEM US configuration */
} gem_drv_priv_t;

/***************************************************************************
 * tcont object type
 **************************************************************************/

/* tcont object private data */
typedef struct {
    bdmf_index index;                   /* TCONT index */
    bdmf_boolean mgmt;                  /* YES:OMCI Management tcont */	
    uint16_t alloc_id;                  /* Alloc id */
    bdmf_boolean assign_ploam_flag;     /* true if assign alloc id message arrived */
    bdmf_object_handle egress_tm;       /* Scheduler object instance */
    bdmf_boolean enable;                /* enable TCONT */
    rdpa_tm_orl_prty orl_prty;          /* priority for overall rate limiter */
} tcont_drv_priv_t;

extern struct bdmf_object *gem_objects[];

/* The function returns id of tcont associated with gem flow, given
 * the gem flow id.
 * The function can be called in interrupt context.
 * Returns 0 if OK or error < 0
 */
static inline int _rdpa_gpon_gem_flow_to_tcont_id(int gem_flow, int *tcont_id)
{
    struct bdmf_object *gem_obj;
    gem_drv_priv_t *gem_priv;
    tcont_drv_priv_t *tcont_priv;

    BUG_ON((unsigned)gem_flow >= RDPA_MAX_GEM_FLOW);
    gem_obj = gem_objects[gem_flow];
    if (!gem_obj)
        return BDMF_ERR_NOENT;
    gem_priv = (gem_drv_priv_t *)bdmf_obj_data(gem_obj);
    if (!gem_priv->us_cfg.tcont)
        return BDMF_ERR_NOENT;
    tcont_priv = (tcont_drv_priv_t *)bdmf_obj_data(gem_priv->us_cfg.tcont);
    *tcont_id = tcont_priv->index;
    return 0;
}

uint8_t tcont_tc_table_get(bdmf_object_handle tcont);
uint8_t tcont_pbit_table_get(bdmf_object_handle tcont);

void _pmd_gpon_isr_wrapper(void);

#endif /* _RDPA_GPON_H_ */
