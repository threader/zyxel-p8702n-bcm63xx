EXE = vlanctl
dynamic: generic_exe_install

clean:
	rm -f vlanctl


vlanctl:
	cp vlanctl_$(PROFILE)_save vlanctl

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
