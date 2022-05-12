export PRJ_BASE=$(shell pwd)

#Platform porting should export the following definition.
#export CC = $(CROSS_CC)
#export STRIP = $(CROSS_STRIP)
#export INSTALL_DIR = $(ROOTFS_INSTALL_PATH)
export VOICE_HW = BRCM6368

#export IPTK_7_1 = yes
#export IPTK_8_0 = yes
export IPTK_8_2_2 = yes

#Choose the IPTK Rel version
ifeq ($(IPTK_7_1),yes)
export IPTK_BASE = $(PRJ_BASE)/iptk_7_1
export IPTK_COMPILE_FLAG = -DIPTK_REL_7_1
else ifeq ($(IPTK_8_0),yes)
export IPTK_BASE = $(PRJ_BASE)/iptk_8_0
export IPTK_COMPILE_FLAG = -DIPTK_REL_8_0
else ifeq ($(IPTK_8_2_2),yes)
export IPTK_BASE = $(PRJ_BASE)/iptk_8_2_2
export IPTK_COMPILE_FLAG = -DIPTK_REL_8_2_2
endif

# Support SIP TLS
ifdef ZYXEL_SIP_OVER_TLS_ENABLE
export IPTK_SUPPORT_TLS = yes
else
export IPTK_SUPPORT_TLS = no
endif

IPTK_EXECUTE:= icf.exe

export ICF_DEV_DIR = $(IPTK_BASE)/ICF
export STACK = $(IPTK_BASE)/ICF/source/stacks/sip_micro_ua/microua_3_0
export SRTP_LIB_DIR = ${USERSPACE_PUBLIC_LIBS_DIR}/srtp
export IPTK_COMMON_INC
VOICEAPP:= $(PRJ_BASE)/voiceApp/build/bin/voiceApp
IPTK_SHELL:= $(ICF_DEV_DIR)/source/ifx_al/bin/intel/$(IPTK_EXECUTE)

#IPTK_SUPPORT_TLS
ifeq ($(IPTK_SUPPORT_TLS), yes)
IPTK_CFG:= $(ICF_DEV_DIR)/source/ifx_al/bin/icf_tls.cfg
else
IPTK_CFG:= $(ICF_DEV_DIR)/source/ifx_al/bin/icf.cfg
endif

#IPTK_DISABLE_PRACK, NORWAY CUSTOMIZATION remove 100rel
ifneq ($(strip $(BUILD_NORWAY_VOIP_CUSTOMIZATION)),)
export IPTK_DISABLE_PRACK = yes
else
export IPTK_DISABLE_PRACK = no
endif

MM_SHELL:= $(PRJ_BASE)/mm/port/build/bin/mm.exe
MM_IVR_SYS_BIN:= $(PRJ_BASE)/mm/port/build/bin/ivrsys.bin
MM_IVR_HOWLER_TONE_BIN:= $(PRJ_BASE)/mm/port/build/bin/howler_ulaw.bin
ifeq ($(IPTK_8_2_2),yes)
UA_LIB:= $(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0/ua_3_0_src/source/lib
IPTK_COMMON_INC = -I$(STACK)/ua_3_0_src/source/UAlib/h
endif
all: ims postinstall

ims:
ifeq ($(IPTK_SUPPORT_TLS), yes)
	$(MAKE) -f makefile iptk_tls -C $(ICF_DEV_DIR)/build/linux
	$(MAKE) -f makefile iptk_exe_tls -C $(ICF_DEV_DIR)/source/ifx_al/make/linux
else
	$(MAKE) -f makefile -C $(ICF_DEV_DIR)/build/linux
	$(MAKE) -f makefile -C $(ICF_DEV_DIR)/source/ifx_al/make/linux
endif	
	cp $(ICF_DEV_DIR)/lib/linux/libclib.a $(PRJ_BASE)/voiceApp/build/lib/linux
	cp $(ICF_DEV_DIR)/lib/linux/libvldt.a $(PRJ_BASE)/voiceApp/build/lib/linux
ifneq ($(strip $(BUILD_MSTC_VOICE_SRTP)),)
	$(MAKE) libsrtp.a -C $(SRTP_LIB_DIR)
endif
	$(MAKE) -f Makefile -C $(PRJ_BASE)/broadcom/vodsl
	$(MAKE) -f makefile -C $(PRJ_BASE)/common/make all
	$(MAKE) -f makefile -C $(PRJ_BASE)/sys_itf/make
	$(MAKE) -f makefile -C $(PRJ_BASE)/driver_itf/make/linux
	$(MAKE) -f makefile -C $(PRJ_BASE)/config/make
	$(MAKE) -f makefile -C $(PRJ_BASE)/voice/make

ifneq ($(strip $(BUILD_MSTC_VOICE_SRTP)),)
	cp $(SRTP_LIB_DIR)/libsrtp.a $(PRJ_BASE)/mm/port/build/lib/linux
endif
	cp $(PRJ_BASE)/common/make/libcommon.a $(PRJ_BASE)/mm/port/build/lib/linux
	cp $(PRJ_BASE)/sys_itf/make/libsys_itf.a $(PRJ_BASE)/mm/port/build/lib/linux
	cp $(ICF_DEV_DIR)/lib/linux/libclib.a $(PRJ_BASE)/mm/port/build/lib/linux
	cp $(ICF_DEV_DIR)/lib/linux/libvldt.a $(PRJ_BASE)/mm/port/build/lib/linux
	$(MAKE) -f makefile -C $(PRJ_BASE)/mm/port/build/make/linux
	$(MAKE) -f makefile -C $(PRJ_BASE)/voiceApp/build/make/linux
postinstall:
	install -m 755 $(VOICEAPP) $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/voiceApp
	install -m 755 $(IPTK_SHELL) $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/$(IPTK_EXECUTE)
	install -m 755 -T $(IPTK_CFG) $(INSTALL_DIR)/bin/icf.cfg
	install -m 755 $(MM_SHELL) $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/mm.exe
	install -m 755 $(MM_IVR_SYS_BIN) $(INSTALL_DIR)/bin

ifeq ($(IPTK_8_2_2),yes)
	install -m 755 $(UA_LIB)/libmicrosipaccessor.so $(INSTALL_DIR)/lib
	install -m 755 $(UA_LIB)/libmicrosipclone.so $(INSTALL_DIR)/lib
	install -m 755 $(UA_LIB)/libmicrosipcore.so $(INSTALL_DIR)/lib
	install -m 755 $(UA_LIB)/libua.so $(INSTALL_DIR)/lib
	install -m 755 $(UA_LIB)/libuaport.a $(INSTALL_DIR)/lib
endif

ifeq ($(IPTK_SUPPORT_TLS), yes)
	mkdir -p $(INSTALL_DIR)/etc/tls
	install -m 755 $(PRJ_BASE)/tls/sipcacert.pem $(INSTALL_DIR)/etc/tls
	install -m 755 $(PRJ_BASE)/tls/sipcert.pem $(INSTALL_DIR)/etc/tls
	install -m 755 $(PRJ_BASE)/tls/sippriv.pem $(INSTALL_DIR)/etc/tls	
endif

#	install -m 755 $(MM_IVR_HOWLER_TONE_BIN) $(INSTALL_DIR)/bin

clean:
	$(MAKE) -f makefile clean -C $(ICF_DEV_DIR)/build/linux
	$(MAKE) -f makefile clean -C $(ICF_DEV_DIR)/source/ifx_al/make/linux
	$(MAKE) -f Makefile clean -C $(PRJ_BASE)/broadcom/vodsl
	$(MAKE) -f makefile clean -C $(PRJ_BASE)/common/make
	$(MAKE) -f makefile clean -C $(PRJ_BASE)/sys_itf/make
	$(MAKE) -f makefile clean -C $(PRJ_BASE)/driver_itf/make/linux
	$(MAKE) -f makefile clean -C $(PRJ_BASE)/config/make
	$(MAKE) -f makefile clean -C $(PRJ_BASE)/voice/make
	$(MAKE) -f makefile clean -C $(PRJ_BASE)/voiceApp/build/make/linux
	$(MAKE) -f makefile clean_all -C $(PRJ_BASE)/mm/port/build/make/linux
