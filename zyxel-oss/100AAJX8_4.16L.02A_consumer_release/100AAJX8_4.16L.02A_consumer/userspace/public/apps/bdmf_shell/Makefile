# This Makefile builds bdmf_shell application that enables
# CLI capabilities in BDMF-based management system (ie, RDPA)
#
EXE=bdmf_shell
OBJS=bdmf_shell_client.o
LIBS = -lpthread

all dynamic install: build

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
BDMF_DIR=$(BRCMDRIVERS_DIR)/opensource/char/bdmf/bcm9$(BRCM_CHIP)

include $(BUILD_DIR)/make.common

ALLOWED_INCLUDE_PATHS := -I. -I$(BDMF_DIR)/system -I$(BDMF_DIR)/system/linux 
ALLOWED_LIB_DIRS := /lib:/lib/private:/lib/public
CUSTOM_CFLAGS += -Werror -Wfatal-errors -DBDMF_SYSTEM_LINUX

ifneq ($(strip $(BUILD_BDMF_SHELL)),)
build: $(EXE) generic_exe_install
	install scripts/bs $(INSTALL_DIR)/bin
	install scripts/runner $(INSTALL_DIR)/bin
	install scripts/rdpa_common_init.sh $(INSTALL_DIR)/etc
	install scripts/rdpa_common_filter_init.sh $(INSTALL_DIR)/etc
ifdef BRCM_EPON_STACK
	install scripts/rdpa_epon_init.sh $(INSTALL_DIR)/etc/
ifeq ($(strip $(EPON_ONU_TYPE)),EPON_SFU)
	perl -pi -e 's|switching_mode=none,ip_class_method=fc|switching_mode=mac_based,ip_class_method=none|g' $(INSTALL_DIR)/etc/rdpa_epon_init.sh
	perl -pi -e 's|vlan_isolation={us=yes,ds=yes}|vlan_isolation={us=no,ds=no}|g' $(INSTALL_DIR)/etc/rdpa_epon_init.sh
	perl -pi -e 's|car_mode=true|car_mode=false}|g' $(INSTALL_DIR)/etc/rdpa_epon_init.sh
endif	
endif
ifneq ($(strip $(BRCM_DRIVER_GPON_STACK)),)
ifneq ($(strip $(BRCM_EXT_SWITCH)),)
	install scripts/rdpa_gpon_ext_sw_init.sh $(INSTALL_DIR)/etc/
else
	install scripts/rdpa_gpon_init.sh $(INSTALL_DIR)/etc/
endif
ifeq ($(strip $(GPON_ONU_TYPE)),GPON_SFU)
	perl -pi -e 's|ip_class_method=fc|ip_class_method=none|g' $(INSTALL_DIR)/etc/rdpa_gpon_init.sh
	perl -e 'print "bs /bdmf/new iptv/lookup_method=group_ip\n"' >>  $(INSTALL_DIR)/etc/rdpa_gpon_init.sh
	perl -e 'print "bs /bdmf/configure system cfg={headroom_size=0}\n"' >>  $(INSTALL_DIR)/etc/rdpa_gpon_init.sh
endif
else
ifneq ($(strip $(BUILD_DSL_RUNNER)),)
	install scripts/rdpa_dsl_common_init.sh $(INSTALL_DIR)/etc/
	install scripts/rdpa_dsl_init.sh $(INSTALL_DIR)/etc/
else
ifneq ($(strip $(WAN_OE_0)),)
	install scripts/rdpa_gbe_wan0_init.sh $(INSTALL_DIR)/etc/
endif
ifneq ($(strip $(WAN_OE_5)),)
	install scripts/rdpa_gbe_wan5_init.sh $(INSTALL_DIR)/etc/
endif
ifneq ($(strip $(WAN_OE_4)),)
	install scripts/rdpa_gbe_init.sh $(INSTALL_DIR)/etc/
endif
endif
endif
else
build:
	@echo "skipping $@ (not configured)"
endif

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

clean: generic_clean
	rm -f $(INSTALL_DIR)/bin/$(EXE)

