EXE = swmdk
dynamic: generic_exe_install

clean:
	rm -f swmdk


swmdk:
	cp swmdk_$(PROFILE)_save swmdk

CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
