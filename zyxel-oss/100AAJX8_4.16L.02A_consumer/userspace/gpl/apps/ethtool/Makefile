#***********************************************************************
#
#  Copyright (c) 2012  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

all dynamic install: conditional_build

ifeq ($(V),1)
  $(info building ethtool verbosely)
  Q =
else
  Q = @
endif

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

APP = ethtool-3.10
REQUIRED_AUTOCONF_VERSION := 2.59
REQUIRED_AUTOMAKE_VERSION := 1.10.2

check_versions:
	$(HOSTTOOLS_DIR)/scripts/checkver.pl -e autoconf -r $(REQUIRED_AUTOCONF_VERSION)
	$(HOSTTOOLS_DIR)/scripts/checkver.pl -e automake -r $(REQUIRED_AUTOMAKE_VERSION)


.PHONY: untar patch configure conditional_build check_versions



Makefile.in:
	@echo "Untarring original $(APP) source";
	$(Q)tar xkjf $(APP).tar.bz2 2> /dev/null || true;

untar: Makefile.in

patch: Makefile.in
	$(Q)if [ -e $(APP).patch ]; then \
	    echo "Applying patches to $(APP)"; \
	    patch -p1 -b -s -d$(APP) < $(APP).patch; \
	    if [ $$? -ne 0 ]; then \
	         exit 1; \
	    fi; \
	else \
	    echo "No patch file for $(APP), not patching"; \
	fi

Makefile:  patch Makefile.in check_versions
	@echo "Running configure on $(APP)";
	@echo "building to $(INSTALL_DIR)";
	$(Q)cd $(APP); \
	 export PKG_CONFIG_LIBDIR=$(INSTALL_DIR)/lib/gpl; \
	 ./configure --host=mips-linux-uclibc \
	        --target=mips-linux-uclibc \
	        --disable-devel \
	        --disable-shared \
	        --prefix=$(INSTALL_DIR) \
	        --sbindir=$(INSTALL_DIR)/bin \
	        --libdir=$(INSTALL_DIR)/lib/gpl \
	        --with-kbuild=$(TOOLCHAIN_INCLUDE_DIR) \
	        LDFLAGS=-L$(INSTALL_DIR)/lib/gpl \
	        CFLAGS=-s\ -O2\ -Wno-strict-aliasing; \

configure: Makefile; 

ifneq ($(strip $(BUILD_ETHTOOL_APP)),)

conditional_build: untar patch configure check_versions
	$(Q)cd $(APP); $(MAKE) -j1
	$(Q)install -m 755 $(APP)/ethtool $(INSTALL_DIR)/bin

else

conditional_build:
	@echo "skipping $(APP) (not configured)"

endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -rf $(APP)
	rm -f $(INSTALL_DIR)/bin/ethtool

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean


check1:
	@echo INSTALL_DIR is $(INSTALL_DIR);

mk_patch: clean
	cp $(APP) $(APP).bk;
	cd $(APP); make distclean;
	
	
	@echo building patch for $(APP)
	mv $(APP) $(APP).new
	tar xkf $(APP).tar.bz2 2> /dev/null || true
	
	


