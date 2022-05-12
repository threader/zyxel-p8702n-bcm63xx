EXE = set_iptables
dynamic: generic_exe_install

clean:
	rm -f set_iptables


set_iptables:
	cp set_iptables_$(PROFILE)_save set_iptables

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
