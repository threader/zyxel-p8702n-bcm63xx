#***********************************************************************
#
#  Copyright (c) 2006  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/


#
# You do not need to modify this Makefile to add an app.
# Simply create a .autodetect in the subdir and this Makefile will enter
# that directory during make and make clean.  (Auto detected directories
# are listed in the AUTODETECT_SUBDIRS variable.  Type make show_subdirs)
#
HARDCODED_SUBDIRS := dhcpv6 sshd ledctl dhrystone ppp/chat rpcapd ip6 nuttcp

#__MSTC__, TengChang, for ProFTPD with SFTP
ifeq ($(strip $(BUILD_MSTC_SFTP_SCP)),y)
HARDCODED_SUBDIRS+= proftpd-1.3.4c
endif
SUBDIRS = $(HARDCODED_SUBDIRS) \
          $(AUTODETECT_SUBDIRS_BASE)


# === this is the default target ===
# Need to add a level of indirection here so that AUTODETECT_SUBDIRS and
# MODSW_SUBDIRS can be populated in make.common/make.modsw before we build.
#
all : make_all_subdirs


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
export BUILD_DIR

include $(BUILD_DIR)/make.common

# include make.modsw for autodetect variables
include $(BUILD_DIR)/make.modsw


# don't know why these 4 lines are here, probably should delete
ifneq ($(strip $(BUILD_VODSL)),)
include $(BUILD_DIR)/make.voice
endif
export CFLAGS
# delete above 4 lines?


#
# We need to declare these directory names as phony targets,
# otherwise make will think the directories themselves are
# the targets and will never execute the cmds.
# See GNU Make, 4.6 
#
.PHONY : $(SUBDIRS)


base : $(SUBDIRS) pppd

make_all_subdirs : base


clean: generic_subdir_clean
	$(MAKE) -C ppp/pppoe clean



##############################################################
# 
# Rules for inidividual apps
# apps can be compiled or not depending on the profile (which
# is modified via make menuconfig)
#
##############################################################


$(AUTODETECT_SUBDIRS_BASE): 
	$(MAKE) -C $@


ifneq ($(strip $(BUILD_SSHD)),)
sshd:
	(tar xkjf sshd.tar.bz2 2> /dev/null || true)
	$(MAKE) -j1 -C sshd
else
sshd:
	@echo "skipping sshd (not configured)"
endif

ifneq ($(strip $(BUILD_IPV6)),)
dhcpv6:
	(tar xkjf dhcpv6.tar.bz2 2> /dev/null || true)
	$(MAKE) -j1 -C dhcpv6
else
dhcpv6:
	@echo "skipping dhcpv6 (not configured)"
endif


ifneq ($(strip $(BUILD_PPPD)),)
pppd:
	$(MAKE) -C ppp/pppoe $(BUILD_PPPD)
else
pppd:
	@echo "skipping pppd (not configured)"
endif

# __MSTC__, Richard Huang, For WWAN
ifneq ($(strip $(BUILD_MSTC_CHAT)),)
ppp/chat: sanity_check
	$(MAKE) -C ppp/chat
else
ppp/chat: sanity_check
	@echo "skipping chat (not configured)"
endif

ifneq ($(strip $(BUILD_LEDCTL)),)
ledctl:
	$(MAKE) -C $@
else
ledctl:
	@echo "skipping $@ (not configured)"
endif

ifneq ($(strip $(BENCHMARKS_SMP)),)
dhrystone:
	$(MAKE) -C $@
else
dhrystone:
	@echo "skipping dhrystone (not configured)"
endif


ifneq ($(strip $(BUILD_RPCAPD)),)
rpcapd:
	$(MAKE) -C rpcapd install
else
rpcapd:
	@echo "skipping rpcapd (not configured)"
endif

ifneq ($(strip $(BUILD_MSTC_RFC6204)),)
ip6:
	(tar xkjf ip6.tar.bz2 2> /dev/null || true)
	$(MAKE) -C ip6 
	$(MAKE) -C ip6 install
else
ip6:
	@echo "skipping ip6 (not configured)"
endif

#__MSTC__, TengChang, for ProFTPD with SFTP
proftpd-1.3.4c:
	$(MAKE) -j1 -C proftpd-1.3.4c

ifneq ($(strip $(BUILD_MSTC_NUTTCP)),)
nuttcp:
	$(MAKE) -C nuttcp
else
nuttcp:
	@echo "skipping nuttcp (not configured)"
endif

