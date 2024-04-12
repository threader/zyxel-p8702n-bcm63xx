#***********************************************************************
#
#  Copyright (c) 2012  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

all dynamic install: conditional_build


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

# Download from http://www.sqlite.org/sqlite-3.6.23.1.tar.gz
APP = sqlite-3.6.23.1
LIB = libsqlite3.so.0.8.6
INS_LIB = libsqlite3.so.0

.PHONY: check_untar_patch_configure conditional_build check_versions

ifneq ($(strip $(BUILD_DLNA)),)
conditional_build: 
	@if [ ! -e $(APP)/INSTALL ]; then \
	   mkdir $(APP); cd $(APP); \
	   tar --strip-components=1 -xzf ../$(APP).tar.gz; \
	   ./configure --disable-readline --host=$(TOOLCHAIN_PREFIX) --prefix=$(CURR_DIR); \
	   cd ..; \
	fi; \
	make -C $(APP) ; \
	make -C $(APP) install; \
	cp -d lib/$(LIB) $(INSTALL_DIR)/lib/$(INS_LIB); \
	echo "Done building $(APP)"
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -f $(INSTALL_DIR)/lib/$(INS_LIB)
	rm -rf bin share include lib $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean
	
