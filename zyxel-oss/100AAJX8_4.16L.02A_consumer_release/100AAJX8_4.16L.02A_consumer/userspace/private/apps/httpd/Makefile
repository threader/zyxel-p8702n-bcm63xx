EXE = httpd
dynamic: generic_exe_install


httpd:
	cp httpd_$(PROFILE)_save httpd

	mkdir -p $(INSTALL_DIR)/webs/Brick
	cp -fr html/Common/* $(INSTALL_DIR)/webs/Brick


	cp -fr html/ZyXEL_Brick/* $(INSTALL_DIR)/webs/Brick/


	cp -fr html/ZyXEL_Brick/pages/$(MSTC_INTERNAL_MODEL_NAME)/* $(INSTALL_DIR)/webs/Brick/pages/


clean:
	rm -f httpd


CURR_DIR := $(shell pwd)
BUILD_DIR := $(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR := $(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
