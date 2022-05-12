#****************************************************************************
#
# Copyright ?2008 Broadcom Corporation
#
# This program is the proprietary software of Broadcom Corporation and/or its
# licensors, and may only be used, duplicated, modified or distributed
# pursuant to the terms and conditions of a separate, written license
# agreement executed between you and Broadcom (an "Authorized License").
# Except as set forth in an Authorized License, Broadcom grants no license
# (express or implied), right to use, or waiver of any kind with respect to
# the Software, and Broadcom expressly reserves all rights in and to the
# Software and all intellectual property rights therein.  IF YOU HAVE NO
# AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
# AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
# SOFTWARE.
#
# Except as expressly set forth in the Authorized License,
#
# 1.     This program, including its structure, sequence and organization,
#        constitutes the valuable trade secrets of Broadcom, and you shall
#        use all reasonable efforts to protect the confidentiality thereof,
#        and to use this information only in connection with your use of
#        Broadcom integrated circuit products.
#
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
#        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
#        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
#        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
#        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
#        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
#        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
#        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
#        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
#
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
#        OR ITS LICENSORS BE LIABLE FOR
#        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
#             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
#             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
#             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
#        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
#             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
#             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
#             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
#
#****************************************************************************
#
#  $Id:
#
#   Filename: Makefile
#   Author:
#   Creation Date: 2008
#
#
#****************************************************************************
# Makefile for vodsl application.
#

TOPDIR = $(PRJ_BASE)/broadcom/vodsl

SUBDIRS = endpoint bos voip

LIB_BROADCOM_A = libbroadcom.a

LIB = $(AR) -ruv

OBJS = $(foreach obj,$(SUBDIRS),$(obj)/$(obj).o)

EXTRA_CFLAGS += -I$(TOPDIR)/voip/inc

EXTRA_CFLAGS +=  -DFLEXIBLE_CONFERENCING_SUPPORTED=1                                           \
                 -DLINUX -DBOS_OS_LINUXUSER                                                    \
                 $(CMS_COMPILE_FLAGS)                                                          \
                 $(BRCM_DSP_CODEC_DEFINES)                                                     \
                 $(BRCM_DSP_CHAN_DEFINES)                                                      \
                 $(LOCALEFLAGS)                                                                \
                 -I$(INC_BRCMDRIVER_PRIV_PATH)/$(BRCM_BOARD) -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(KERNEL_DIR)/include/asm/mach-generic                                    \
                 -I$(INC_KERNEL_PATH) -I$(INC_KERNEL_PATH2) \
                 -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PRIV_PATH)/$(BRCM_BOARD) \
                 -I$(INC_ENDPOINT_PATH)                                                        \
                 -I$(INC_ENDPOINT_PATH)/cfg                                                    \
                 -I$(XCHANGE_DIR)/dslx/src/mta/inc/cfg                    \
                 -I$(XCHANGE_DIR)/dslx/apps/$(XCHANGE_BUILD_APP)/cfginc/mta                    \
                 -I$(XCHANGE_DIR)/dslx/apps/$(XCHANGE_BUILD_APP)/cfginc/xchg_common            \
                 -I$(XCHANGE_DIR)/dslx/apps/$(XCHANGE_BUILD_APP)/cfginc/xdrv                   \
                 -I$(XCHANGE_DIR)/dslx/apps/$(XCHANGE_BUILD_APP)/cfginc/vrg                    \
                 -I$(XCHANGE_DIR)/dslx/apps/cfginc                                             \
                 -I$(XCHANGE_DIR)/dslx/apps/cfginc/vodsl                                       \
                 -I$(XCHANGE_DIR)/dslx/apps/cfginc/mta                                         \
                 -I$(XCHANGE_DIR)/dslx/apps/cfginc/default                                     \
                 -I$(XCHANGE_DIR)/dslx/apps/cfginc/xchg_common                                 \
                 -I$(XCHANGE_DIR)/dslx/apps/cfginc/xdrv                                        \
                 -I$(XCHANGE_DIR)/dslx/apps/cfginc/vrg                                         \
                 -I$(XCHANGE_DIR)/dslx_common/xchg_common/bos/publicInc                        \
                 -I$(XCHANGE_DIR)/dslx_common/xchg_common/assert/cfginc                        \
                 -I$(XCHANGE_DIR)/dslx_common/xchg_common/assert/inc                           \
                 -I$(XCHANGE_DIR)/dslx_common/xchg_common/str                                  \
                 -I$(XCHANGE_DIR)/dslx_common/voice_res_gw/inc                                 \
                 -I$(XCHANGE_DIR)/dslx_common/voice_res_gw/casCtl/inc                          \
                 -I$(XCHANGE_DIR)/dslx_common/voice_res_gw/endpt/inc                           \
                 -I$(XCHANGE_DIR)/dslx_common/voice_res_gw/endpt/vrg/cfginc                    \
                 -I$(XCHANGE_DIR)/dslx_common/voice_res_gw/endpt/vrg/inc                       \
                 -I$(XCHANGE_DIR)/dslx_common/voice_res_gw/codec                               \
                 -I$(XCHANGE_DIR)/dslx_common/xchg_drivers/inc                                 \
                 -I$(XCHANGE_DIR)/dslx_common/prot_callctrl/apps/vrg/dslx/cfginc               \
                 -I$(XCHANGE_DIR)/dslx_common/prot_callctrl/apps/vrg/vrgcallmgr/cfginc         \
                 -I$(XCHANGE_DIR)/dslx_common/prot_callctrl/apps/vrg/vrgcallmgr/inc/public     \
                 -I$(XCHANGE_DIR)/dslx_common/prot_callctrl/stacks/cfginc                      \
                 -I$(XCHANGE_DIR)/dslx_common/prot_callctrl/stacks/callctrl/inc                \
                 -I$(BUILD_DIR)/userspace/public/include                                       \
                 -I$(BUILD_DIR)/userspace/public/include/linux                                 \
                 -I$(BUILD_DIR)/userspace/private/include                                      \
                 -I$(BUILD_DIR)/userspace/private/include/linux


#
# Adding Feature specific Build flags 
#

export EXTRA_CFLAGS	TOPDIR


ifeq ($(strip $(BRCM_VOIP_TO_FXO_SUPPORT)),1)
EXTRA_CFLAGS += -DBRCM_VOIP_TO_FXO
endif

ifeq ($(strip $(BRCM_PSTN_SUPPORT)),1)
EXTRA_CFLAGS += -DBRCM_PSTN
endif

ifeq ($(strip $(BRCM_VODSL_ISDN)),1)
EXTRA_CFLAGS += -DISDN_SUPPORT=1
endif

ifeq ($(strip $(BRCM_VODSL_VOICE_ANNOUNCEMENT)),1)
EXTRA_CFLAGS += -DBRCM_VOICE_ANNOUNCEMENT_SUPPORT=1
endif

ifeq ($(strip $(BRCM_SIP_TLS)),1)
EXTRA_CFLAGS += -DXCFG_SIP_TLS
endif

ifeq ($(strip $(BRCM_SRTP_SUPPORT)),1)
EXTRA_CFLAGS += -DSRTP_SUPPORT
endif

ifeq ($(strip $(BRCM_VODSL_DECT)),1)
EXTRA_CFLAGS += -DDECT_SUPPORTED=1
endif

ifeq ($(strip $(BRCM_VODSL_CALL_FEATURE_PERSIST_STORE_SUPPORT)),1)
EXTRA_CFLAGS += -DCALL_FEATURE_PERSIST_STORE_SUPPORT=1
endif

ifeq ($(strip $(BRCM_VODSL_PSTN_LIFE_LINE_SUPPORT)),1)
EXTRA_CFLAGS += -DPSTN_LIFE_LINE_SUPPORT=1
endif

ifeq ($(strip $(MGCPLOAD)),1)
EXTRA_CFLAGS += -DIP_MGCP
endif

ifeq ($(strip $(SIPLOAD)),1)
EXTRA_CFLAGS += -DIP_SIP -DSIPLOAD
endif

ifeq ($(strip $(BRCM_VODSL_RANDOM_PORT)),1)
EXTRA_CFLAGS += -DRANDOM_PORT
endif

ifeq ($(strip $(BRCM_VODSL_MULTI_SIP_ACCNT)),1)
EXTRA_CFLAGS += -DMULTI_SIP_ACCNT
endif

ifeq ($(strip $(BRCM_SRTP_SUPPORT)),1)
EXTRA_CFLAGS += -DSEC_SRTP
endif

ifeq ($(strip $(BRCM_SIPROXD)),1)
EXTRA_CFLAGS += -DEXTERNAL_SIP_ALG
endif

OBJS = $(foreach obj,$(SUBDIRS),$(obj)/$(obj).o)


EXTRA_CFLAGS += -Wall

EXTRA_SHARED_LIB+= -L$(INSTALL_DIR)/lib/private -L$(INSTALL_DIR)/lib/public $(CMS_CORE_LIBS) $(CMS_COMMON_LIBS) -lcms_dal -lcms_msg 

all: subdirs $(OBJS) $(LIB_BROADCOM_A)

$(LIB_BROADCOM_A):
	$(LIB) $(LIB_BROADCOM_A) $(OBJS)
	cp $(LIB_BROADCOM_A) $(PRJ_BASE)/voiceApp/build/lib/linux
	cp $(LIB_BROADCOM_A) $(PRJ_BASE)/mm/port/build/lib/linux

subdirs: $(patsubst %, _dir_%, $(SUBDIRS))

$(patsubst %, _dir_%, $(SUBDIRS)) :
	$(MAKE) -C $(patsubst _dir_%, %, $@) $(TGT)

clean:
	$(MAKE) -i TGT=clean subdirs
	rm -f $(LIB_BROADCOM_A)
	rm -f $(PROGRAM_NAME) $(PROGRAM_NAME).o $(PROGRAM_NAME).a $(PROGRAM_NAME)_map.txt
