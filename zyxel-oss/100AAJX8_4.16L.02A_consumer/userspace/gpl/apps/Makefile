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
HARDCODED_SUBDIRS := atm2684 bridge-utils busybox sysstat dproxy-nexgen \
           ftpd iproute2 radvd udhcp urlfilterd zebra \
           dynahelper samba iperf conntrack \
           openl2tpd rng-tools-2 pptp \
           mtd-util \
           dnsmasq mailsend wput wpa-supplicant inadyn \
           ndisc6 lsusb usb-modeswitch-1.1.0


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


#
# We need to declare these directory names as phony targets,
# otherwise make will think the directories themselves are
# the targets and will never execute the cmds.
# See GNU Make, 4.6 
#
.PHONY : $(SUBDIRS)


base : $(SUBDIRS)

make_all_subdirs : base


clean: generic_subdir_clean



##############################################################
# 
# Rules for inidividual apps
# apps can be compiled or not depending on the profile (which
# is modified via make menuconfig)
#
##############################################################


$(AUTODETECT_SUBDIRS_BASE): 
	$(MAKE) -C $@


ifneq ($(strip $(BUILD_BRCTL)),)
bridge-utils: sanity_check
	mkdir -p $(INSTALL_DIR)/bin
	(tar xkfj bridge-utils.tar.bz2 2> /dev/null || true)
	$(MAKE) -C bridge-utils $(BUILD_BRCTL)
else
bridge-utils: sanity_check
endif


ifeq ($(strip $(BRCM_IKOS)),y)
BRCMCONFIG=ikos.config
else
ifneq ($(strip $(BUILD_MSTC_WWAN)),)
BRCMCONFIG=WWAN.config
else
BRCMCONFIG=brcm.config
endif
endif

export BRCMCONFIG

PERL5LIB=$(HOSTTOOLS_PERL_LIB)
export PERL5LIB

ifneq ($(strip $(BUILD_BUSYBOX)),)
busybox: sanity_check
	mkdir -p $(INSTALL_DIR)/bin
	(tar xkfj busybox.tar.bz2 2> /dev/null || true)
	cd busybox; cp -f .config .config.old; cat $(BRCMCONFIG) > .config ; \
	for i in config.d/[0-9]*.conf ; do echo "config script $$i" ; $$i ; done
	$(MAKE) -C busybox include/autoconf.h
#__MSTC__, Dennis
ifneq ($(strip $(BUILD_MSTC_WGET)),)
	cd busybox; \
	echo "# CONFIG_FEATURE_WGET_STATUSBAR is not set" >> .config; \
	echo "# CONFIG_FEATURE_WGET_AUTHENTICATION is not set" >> .config; \
	echo "# CONFIG_FEATURE_WGET_IP6_LITERAL is not set" >> .config; \
	cd busybox; sed -i 's/# CONFIG_WGET is not set/CONFIG_WGET=y/' .config
else
	cd busybox;	 \
	echo "# CONFIG_FEATURE_WGET_STATUSBAR is not set" >> .config; \
	echo "# CONFIG_FEATURE_WGET_AUTHENTICATION is not set" >> .config; \
	echo "# CONFIG_FEATURE_WGET_IP6_LITERAL is not set" >> .config; \
	cd busybox; sed -i 's/CONFIG_WGET=y/# CONFIG_WGET is not set/' .config 
endif
	$(MAKE) -C busybox install
else
busybox: sanity_check
	@echo "skipping busybox (not configured)"
endif

ifneq ($(strip $(BUILD_SYSSTAT)),)
sysstat: sanity_check
	mkdir -p $(INSTALL_DIR)/bin
	(tar xkfj sysstat.tar.bz2 2> /dev/null || true)
	cd sysstat
	$(MAKE) -C sysstat install
else
sysstat: sanity_check
	@echo "skipping sysstat (not configured)"
endif


ifneq ($(strip $(BUILD_DPROXY)),)
dproxy-nexgen: sanity_check
	mkdir -p $(INSTALL_DIR)/bin
	(tar xkfj dproxy-nexgen.tar.bz2 2> /dev/null || true)
	$(MAKE) -C dproxy-nexgen $(BUILD_DPROXY)
else
dproxy-nexgen: sanity_check
	@echo "skipping dproxy (not configured)"
endif


# in userapps->Dynamic ALG loader
ifneq ($(strip $(BUILD_DYNAHELPER)),)
dynahelper: sanity_check
	$(MAKE) -C dynahelper $(BUILD_DYNAHELPER)
else
dynahelper: sanity_check
	@echo "skipping dynahelper (not configured)"
endif


# ftpd does sanity check in its own Makefile
ifneq ($(strip $(BUILD_FTPD)),)
ftpd:
	(tar xkfj ftpd.tar.bz2 2> /dev/null || true)
	$(MAKE) -C ftpd $(BUILD_FTPD)
else
ftpd:
	@echo "skipping ftpd (not configured)"
endif


# Build the iproute2 package whenever IPROUTE2, IPV6, or BRCM_KERNEL_NETQOS
# is defined.  In make menuconfig, "traffic control" enables BRCM_KERNEL_NETQOS
ifneq ("$(BUILD_IPROUTE2)$(BUILD_IPV6)$(BRCM_KERNEL_NETQOS)$(BUILD_GPONRG_OMCI_FULL)","")
DO_BUILD_IPROUTE2=y
endif

ifneq ($(strip $(DO_BUILD_IPROUTE2)),)
iproute2: sanity_check
	(tar xkfj iproute2.tar.bz2 2> /dev/null || true)
	$(MAKE) -C iproute2 install 
else
iproute2: sanity_check
	@echo "skipping iproute2 (not configured)"
endif

ifneq ($(strip $(BRCM_KERNEL_AUXFS_JFFS2)),)
# Always make flash utilities and installed executables
mtd-util:
	(cd mtd-util; $(MAKE); $(MAKE) install)
else
mtd-util:
	@echo "BRCM_KERNEL_AUXFS_JFFS2 not configured, skipping"
endif

ifneq ($(strip $(BUILD_IPTABLES)),)
ifneq ($(strip $(BUILD_URLFILTER)),)
urlfilterd: sanity_check
	$(MAKE) -C urlfilterd $(BUILD_URLFILTER)
else
urlfilterd: sanity_check
endif
else
urlfilterd: sanity_check
	@echo Warning: To use urlfilterd, you need to enable iptables!!!!!
endif



ifneq ($(strip $(BUILD_IPV6)),)
radvd: sanity_check
	(tar xkjf radvd.tar.bz2 2> /dev/null || true)
	$(MAKE) -C radvd
else
radvd: sanity_check
	@echo "skipping radvd (not configured)"
endif



ifneq ($(strip $(BUILD_UDHCP)),)
udhcp: sanity_check
	$(MAKE) -C udhcp $(BUILD_UDHCP)
else
udhcp: sanity_check
	@echo "skipping udhcp (not configured)"
endif


ifneq ($(strip $(BUILD_ZEBRA)),)
zebra: sanity_check
	(tar xkfj zebra.tar.bz2 2> /dev/null || true)
	$(MAKE) -C zebra $(BUILD_ZEBRA)
else
zebra: sanity_check
	@echo "skipping zebra (not configured)"
endif


ifneq ($(strip $(BUILD_SAMBA)),)
samba: sanity_check
	(tar xkfz samba-3.0.37.tar.gz 2> /dev/null || true)
	$(MAKE) -C ./samba -j 1
else
samba: sanity_check 
	@echo "skipping $@ (not configured)"
endif

ifneq ($(strip $(BUILD_IPERF)),)
iperf: sanity_check
	$(MAKE) -C ./iperf -j 1
	install -m 755 $(EXTRALIBDIR)/libstdc++.so.6 $(INSTALL_DIR)/lib	
else
iperf: sanity_check
	@echo "skipping $@ (not configured)"
endif

ifneq ($(strip $(BUILD_L2TPAC)),)
openl2tpd: sanity_check
	(tar xkjf openl2tpd.tar.bz2 2> /dev/null || true)
	$(MAKE) -C openl2tpd
else
openl2tpd: sanity_check
	@echo "skipping openl2tpd (not configured)"
endif

ifneq ($(strip $(BUILD_RNGD)),)
rng-tools-2: sanity_check
	mkdir -p $(INSTALL_DIR)/bin
	(tar xkfz rng-tools-2.tar.gz 2> /dev/null || true)
	($(MAKE) -C rng-tools-2 $(BUILD_RNGD))
else
rngd: sanity_check
	@echo "skipping rng-tools-2 (not configured)"
endif

ifneq ($(strip $(BUILD_PPTPAC)),)
pptp: sanity_check
	(tar xkjf pptp.tar.bz2 2> /dev/null || true)
	$(MAKE) -C pptp
else
pptp: sanity_check
	@echo "skipping pptp (not configured)"
endif

ifneq ($(strip $(BUILD_CONNTRACK_TOOLS)),)
conntrack: sanity_check
	(tar xkfz conntrack.tar.gz 2> /dev/null || true)
	$(MAKE) -C ./conntrack
else
conntrack: sanity_check
	@echo "skipping $@ (not configured)"
endif

ifneq ($(strip $(BUILD_OSGI_FELIX)),)
felix_framework: sanity_check
	cd felix_framework; (tar xkfz ../felix.tar.gz 2> /dev/null || true)
	$(MAKE) -C ./felix_framework
#this bundle is used for debugging only.  Felix will run in the background, so this needs to be removed.
#	rm -f $(INSTALL_DIR)/usr/local/felix/bundle/org.apache.felix.gogo.shell-0.7.0-SNAPSHOT.jar
else
felix_framework: sanity_check
	@echo "skipping $@ (not configured)"
endif

ifneq ($(strip $(BUILD_DNSMASQ)),)
export BUILD_MSTC_DNS_ROUTE
export BUILD_MSTC_DNS_ROUND_ROBIN
dnsmasq: sanity_check
	mkdir -p $(INSTALL_DIR)/bin
	(tar xkfj dnsmasq.tar.bz2 2> /dev/null || true)
	$(MAKE) -C dnsmasq $(BUILD_DNSMASQ)
else
dnsmasq: sanity_check
	@echo "skipping dnsmasq (not configured)"
endif

#__MSTC__, TengChang
ifneq ($(strip $(BUILD_MAILSEND)),)
mailsend: sanity_check
	(tar xkfz mailsend.tar.gz 2> /dev/null || true)
	$(MAKE) -C ./mailsend install
else
mailsend: sanity_check
	@echo "skipping mailsend (not configured)"
endif

#__MSTC__, Dennis
ifneq ($(strip $(BUILD_MSTC_WPUT)),)
wput: sanity_check
	$(MAKE) -C ./wput all
	install -m 755 ./wput/wput $(INSTALL_DIR)/bin
else
wput: sanity_check
	@echo "skipping $@ (not configured)"
endif

#__MSTC__, Amy, 802.1x
ifneq ($(strip $(BUILD_MSTC_WAN_8021X)),)
wpa-supplicant:
	$(MAKE) -C ./wpa-supplicant install
	cp ./wpa-supplicant/wpa_supplicant.conf $(INSTALL_DIR)/etc
else
wpa-supplicant:
endif
#__MSTC__, Amy, 802.1x

ifeq ($(strip $(MSTC_DDNS_INADYN)),y)
inadyn: sanity_check
	$(MAKE) -C inadyn	
else
inadyn: sanity_check
	@echo "skipping inadyn (not configured)"
endif

ifneq ($(strip $(BUILD_IPV6)),)
ndisc6: sanity_check
	(tar xkjf ndisc6.tar.bz2 2> /dev/null || true)
	$(MAKE) -C ndisc6
	install -m 755 ./ndisc6/src/ndisc6 $(INSTALL_DIR)/bin
	install -m 755 $(TOOLCHAIN_TOP)/lib/librt.so.0 $(INSTALL_DIR)/lib
	install -m 755 $(TOOLCHAIN_TOP)/lib/librt-0.9.32.so $(INSTALL_DIR)/lib
else
ndisc6: sanity_check 
	@echo "skipping $@ (not configured)"
endif

ifeq ($(strip $(MSTC_DDNS_EZ_IPUPDATE)),y)
ez-ipupdate: sanity_check
	(tar xkjf ez-ipupdate.tar.bz2 2> /dev/null || true)
	cd ez-ipupdate && ./configure --build=i686-linux --host=mips-linux --target=mips-linux-uclibc
	$(MAKE) -C ez-ipupdate
	install -m 755 ./ez-ipupdate/ez-ipupdate $(INSTALL_DIR)/bin
else
ez-ipupdate: sanity_check
	@echo "skipping ez-ipupdate (not configured)"
endif

#__ZyXEL__, I-Wei, support lsusb
ifneq ($(strip $(BUILD_LSUSB)),)
lsusb:
	$(MAKE) -C $@ all
	cd $@; cp -f lsusb $(INSTALL_DIR)/bin/lsusb
	cp -f $(LIBDIR)/librt-0.9.32.so $(INSTALL_DIR)/lib
	cp -f $(LIBDIR)/librt.so.0 $(INSTALL_DIR)/lib
else
lsusb:
	@echo "skipping lsusb (not configured)"
endif

#__ZyXEL__, I-Wei, support usb-modeswitch-1.1.0
ifneq ($(strip $(BUILD_USB_MODESWITCH)),)
usb-modeswitch-1.1.0:
	$(MAKE) -C $@ all
	cd $@; cp -f usb_modeswitch $(INSTALL_DIR)/bin/usb_modeswitch
	mkdir -p $(INSTALL_DIR)/etc/wwan
	cd $@; cp -a usb_modeswitch.d $(INSTALL_DIR)/etc/wwan/
	chmod -R 777 $(INSTALL_DIR)/etc/wwan/usb_modeswitch.d
else
usb-modeswitch-1.1.0:
	@echo "skipping usb-modeswitch-1.1.0 (not configured)"
endif

# Support UPNPC __MSTC__,Yic
miniupnpc-1.7.20120830: sanity_check
	$(MAKE) -C miniupnpc-1.7.20120830