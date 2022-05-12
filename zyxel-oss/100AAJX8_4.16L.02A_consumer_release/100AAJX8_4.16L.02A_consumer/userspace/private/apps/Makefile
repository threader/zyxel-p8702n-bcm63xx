#***********************************************************************
#
#  Copyright (c) 2006  Broadcom Corporation
#  All Rights Reserved
#
#
#***********************************************************************/


#
# You do not need to modify this Makefile to add an app.
# Simply create a .autodetect in the subdir and this Makefile will enter
# that directory during make and make clean.  (Auto detected directories
# are listed in the AUTODETECT_SUBDIRS variable.  Type make show_subdirs)
#
HARDCODED_SUBDIRS := smd ssk omcid ponwiz tr69c wlan httpd consoled telnetd \
           xdslctl gponif vlanctl atmctl ethctl ethswctl cmfctl cmfviz \
           ddnsd dnsprobe dnsspoof sntp upnp ippd x_dms dsldiagd\
           snmp tr64 hotplug pmonctl bounce soapserver \
           spuctl pwrctl gpond gponctl cmfd omcipmdrvctl mcpd fapctl\
           speedsvc fcctl omcipmd eponapp mdkshell stress \
           dumpsysinfo iqctl bpmctl arlctl mcpctl eponctl rastatus6 \
           plcnvm plcboot gmacctl tmsctl myping vcautohunt webipqd cpuload \
           udpechod uploadd link_updown celld sysdiagd hpna autoscheduling watchdog \
           vlanautohunt zycfgfilter linkstatus zyswitch twonky zywlan zycmd getconfig loadconfig set_iptables bootup


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

ifneq ($(strip $(BUILD_VODSL)),)
include $(BUILD_DIR)/make.voice
endif



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


# smd is the system management daemon, it replaces cfm.
# ssk is a helper daemon for smd.
ifneq ($(strip $(BUILD_BRCM_CMS)),)

smd: 
	$(MAKE) -C $@

ssk:
	$(MAKE) -C $@

else

smd:
	@echo "skipping smd (not configured)"

ssk:
	@echo "skipping ssk (not configured)"

endif

#build webipq daemon, add by Amy
webipqd:
	$(MAKE) -C $@

# __MSTC__, Richard Huang, for CPU usg,
cpuload:
	$(MAKE) -C $@

#
# There are three possible variables for tr69c in make menuconfig:
# BUILD_TR69C 
# BUILD_TR69C_SSL
# BUILD_TR69C_BCM_SSL
# (or none)
# If any of the tr69c variants are defined, just go into the tr69c directory
# and make the target.
#
ifneq ($(strip $(BUILD_TR69C)),)
tr69c:
	$(MAKE) -C $@
else
ifneq ($(strip $(BUILD_TR69C_SSL)),)
tr69c:
	$(MAKE) -C $@
else
ifneq ($(strip $(BUILD_TR69C_BCM_SSL)),)
tr69c:
	$(MAKE) -C $@
else
tr69c:
	@echo "skipping tr69c (not configured)"
endif
endif
endif


# omcid, omcipmd, omcipmdrvctl
ifneq ($(strip $(BUILD_PONWIZ)),)
ponwiz: 
	$(MAKE) -C $@ all
omcid:
	@echo "skipping omcid (not configured)"
omcipmd:
	@echo "skipping omcipmd (not configured)"
omcipmdrvctl:
	@echo "skipping omcipmdrvctl (not configured)"
endif

ifneq ($(strip $(BUILD_TWONKY)),)
twonky:
	mkdir -p $(INSTALL_DIR)/twonky
	cp -fra twonky/* $(INSTALL_DIR)/twonky
else
twonky:
	@echo "skipping httpd (not configured)"
endif

ifneq ($(strip $(BUILD_BRCM_OMCI)),)
ponwiz: 
	@echo "skipping ponwiz (not configured)"
omcid: 
	$(MAKE) -C $@
omcipmd: 
	$(MAKE) -C $@
omcipmdrvctl:
	$(MAKE) -C $@
endif

# httpd - Uses libraries built in wlan folder
ifneq ($(strip $(BUILD_HTTPD)),)
httpd: wlan 
	$(MAKE) -C $@
else
ifneq ($(strip $(BUILD_HTTPD_SSL)),)
httpd: wlan
	$(MAKE) -C $@
else
httpd:
	@echo "skipping httpd (not configured)"
endif
endif


# consoled
ifneq ($(strip $(BUILD_CONSOLED)),)
consoled: wlan
	$(MAKE) -C $@
else
consoled: wlan
	@echo "skipping consoled (not configured)"
endif

# telnetd
ifneq ($(strip $(BUILD_TELNETD)),)
telnetd: wlan
	$(MAKE) -C $@
else
telnetd: wlan
	@echo "skipping telnetd (not configured)"
endif


# atmctl
ifneq ($(strip $(BUILD_XTMCTL)),)
atmctl:
	$(MAKE) -C $@
else
atmctl:
	@echo "skipping atmctl (not configured)"
endif


# xdslctl
ifneq ($(strip $(DESKTOP_LINUX)),)
	@echo "skipping xdslctl (not used on desktop linux)"
else
ifneq ($(strip $(BUILD_XDSLCTL)),)
xdslctl: 
	$(MAKE) -C $@
else
xdslctl:
	@echo "skipping xdslctl (not configured)"
endif
endif


# vlanctl
ifneq ($(strip $(BUILD_VLANCTL)),)
vlanctl: 
	$(MAKE) -C $@
else
vlanctl:
	@echo "skipping vlanctl (not configured)"
endif

# gponif
ifneq ($(strip $(BUILD_GPONIF)),)
gponif: 
	$(MAKE) -C $@
else
gponif:
	@echo "skipping gponif (not configured)"
endif

# gpond
ifneq ($(strip $(BUILD_GPOND)),)
gpond: 
	$(MAKE) -C $@
else
gpond:
	@echo "skipping gpond (not configured)"
endif

# gponctl
ifneq ($(strip $(BUILD_GPONCTL)),)
gponctl:
	$(MAKE) -C $@
else
gponctl:
	@echo "skipping gponctl (not configured)"
endif

# ethswctl
ifneq ($(strip $(BUILD_ETHSWCTL)),)
ethswctl: 
	$(MAKE) -C $@
else
ethswctl:
	@echo "skipping ethswctl (not configured)"
endif

#dynamic dns
ifneq ($(strip $(BUILD_DDNSD)),)
ddnsd:
	$(MAKE) -C ddnsd $(BUILD_DDNSD)
else
ddnsd:
	@echo "skipping ddnsd (not configured)"
endif

# dnsprobe
ifneq ($(strip $(BUILD_DNSPROBE)),)
dnsprobe:
	$(MAKE) -C dnsprobe $(BUILD_DNSPROBE)
else
dnsprobe:
	@echo "skipping dnsprobe (not configured)"
endif


# dnsspoof
ifneq ($(strip $(BUILD_DNSSPOOF)),)
dnsspoof:
	$(MAKE) -C dnsspoof $(BUILD_DNSSPOOF)
else
dnsspoof:
	@echo "skipping dnsspoof (not configured)"
endif


#Always compile Ethernet control utility
ethctl:
	$(MAKE) -C ethctl dynamic

ifneq ($(strip $(BUILD_MCAST_PROXY)),)
mcpd:
	$(MAKE) -C mcpd $(BUILD_MCAST_PROXY)
else
mcpd:
	@echo "skipping multicast proxy (mcpd) (not configured)"
endif

ifneq ($(strip $(BUILD_MCAST_PROXY)),)
mcpctl:
	$(MAKE) -C mcpctl $(BUILD_MCAST_PROXY)
else
mcpctl:
	@echo "skipping mcpctl (not configured)"
endif

# Time synchronization daemon
ifneq ($(strip $(BUILD_SNTP)),)
sntp:
	$(MAKE) -C sntp $(BUILD_SNTP)
else
sntp:
	@echo "skipping sntp (not configured)"
endif


# UPNP is dependent on iptables
ifneq ($(strip $(BUILD_IPTABLES)),)
ifneq ($(strip $(BUILD_UPNP)),)
upnp:
	$(MAKE) -C upnp $(BUILD_UPNP)
else
upnp:
	@echo "skipping upnp (not configured)"
endif
else
upnp:
	@echo Warning: upnp requires iptables, please enable iptables.
endif


# IP Printing Daemon
ifneq ($(strip $(BUILD_IPPD)),)
ippd:
	$(MAKE) -C ippd $(BUILD_IPPD)
else
ippd:
	@echo "skipping ippd (not configured)"
endif


# DSL Diag Daemon
ifneq ($(strip $(BUILD_DSLDIAGD)),)
dsldiagd:
	$(MAKE) -C $@
else
dsldiagd:
	@echo "skipping dsldiagd (not configured)"
endif


# Packet CMF Control Utility
ifneq ($(strip $(BUILD_CMFCTL)),)
cmfctl:
	$(MAKE) -C $@ $(BUILD_CMFCTL)
else
cmfctl:
	@echo "Skipping cmfctl application (Packet CMF not configured)"
endif

# Packet CMF Viz Application
ifneq ($(strip $(BUILD_CMFVIZ)),)
cmfviz:
	$(MAKE) -C cmfviz $(BUILD_CMFVIZ)
else
cmfviz:
	@echo "Skipping cmfviz application (Packet CMF not configured)"
endif

# Packet CMF Daemon Application
ifneq ($(strip $(BUILD_CMFD)),)
cmfd:
	$(MAKE) -C cmfd $(BUILD_CMFD)
else
cmfd:
	@echo "Skipping CMF Daemon application"
endif

# FAP Control Utility
ifneq ($(strip $(BUILD_FAPCTL)),)
fapctl:
	$(MAKE) -C $@
else
fapctl:
	@echo "Skipping fapctl application (not configured)"
endif

# Speed Service application
ifneq ($(strip $(BUILD_FAPCTL)),)
speedsvc:
	$(MAKE) -C $@
else
speedsvc:
	@echo "Skipping speedsvc application (not configured)"
endif

# wlan software
ifneq ($(strip $(BRCM_DRIVER_WIRELESS)),)
wlan:
	$(MAKE) -C $@
else
wlan:
endif

# snmp
ifneq ($(strip $(BUILD_SNMP)),)
snmp:
#	$(MAKE) -C snmp $(BUILD_SNMP)
	$(MAKE) -C $@
else
snmp:
	@echo "skipping snmp (not configured)"
endif

# tr64
ifneq ($(strip $(BUILD_TR64)),)
tr64:
	$(MAKE) -C tr64
else
tr64:
	@echo "skipping tr64 (not configured)"
endif

# PLC NVM
ifneq ($(strip $(BUILD_HOMEPLUG)),)
plcnvm:
	$(MAKE) -C $@
else
plcnvm:
	@echo "skipping plcnvm (not configured)"
endif

# PLC BOOT
ifneq ($(strip $(BUILD_HOMEPLUG)),)
plcboot:
	$(MAKE) -C $@
else
plcboot:
	@echo "skipping plcboot (not configured)"
endif

#hotplug software
ifneq ($(strip $(BUILD_NAS)),)
hotplug:
	$(MAKE) -C $@ dynamic
else
ifneq ($(strip $(LINUX_KERNEL_USBMASS)),)
hotplug:
	$(MAKE) -C $@ dynamic
else
hotplug:
	@echo "skipping hotplug (not configured)"
endif
endif

# Build the PMON shell control utility
ifneq ($(strip $(BUILD_PMON)),)
pmonctl:
	$(MAKE) -C $@
else
pmonctl:
	@echo "skipping $@ (not configured)"
endif

# Build the BOUNCE shell control utility
ifneq ($(strip $(BUILD_BOUNCE)),)
bounce:
	$(MAKE) -C $@
else
bounce:
	@echo "skipping $@ (not configured)"
endif

# soapserver
ifneq ($(strip $(BUILD_SOAP)),)
ifeq ($(strip $(BUILD_SOAP_VER)),3)
soapserver:
	$(MAKE) -C $@
endif
else
soapserver:
	@echo "skipping soapserver (not configured)"
endif

# spuctl
ifneq ($(strip $(BUILD_SPUCTL)),)
spuctl:
	$(MAKE) -C $@
else
spuctl:
	@echo "skipping spuctl (not configured)"
endif

# pwrctl
ifneq ($(strip $(BUILD_PWRCTL)),)
pwrctl:
	$(MAKE) -C $@
else
pwrctl:
	@echo "skipping pwrctl (not configured)"
endif

# Flow Cache Control Utility
ifneq ($(strip $(BUILD_FCCTL)),)
fcctl:
	$(MAKE) -C $@ $(BUILD_FCCTL)
else
fcctl:
	@echo "Skipping fcctl application (Flow Cache not configured)"
endif

#x_dms
ifneq ($(strip $(BUILD_DLNA)),)
ifneq ($(strip $(BUILD_TWONKY)),)
x_dms:
	@echo "skipping dlna (not configured)"
else
x_dms:
	(tar xkfj x_dms.tar.bz2 2> /dev/null || true)
	$(MAKE) -j 1 -C x_dms dynamic
endif
else
x_dms:
	@echo "skipping dlna (not configured)"
endif

eponapp:
ifneq ($(strip $(BUILD_EPON_SDK)),)
	$(MAKE) -C $@
else
	@echo "Skipping EPON SDK APP "
endif

mdkshell:
ifneq ($(strip $(BUILD_MDK_SHELL)),)
	$(MAKE) -C $@
else
	@echo "Skipping MDK SHELL "
endif

#myping
ifneq ($(strip $(BUILD_MYPING)),)
myping:
	$(MAKE) -C $@
else
myping:
	@echo "Skipping myping application (configured)"
endif

# Stress application
stress:
ifneq ($(strip $(BUILD_STRESS)),)
	$(MAKE) -C $@
else
	@echo "Skipping $@ (not configured) "
endif

# Dump system info for bug reporting
# Lump it in with debug tools.  Don't know if we actually need
# a separate config option for this or not.
ifneq ($(strip $(BUILD_DEBUG_TOOLS)),)
dumpsysinfo:
	$(MAKE) -C $@
else
	@echo "Skipping $@ (not configured) "
endif


#osgid is now an autodetect subdir, type "make show_subdirs" to see


# Ingress QoS Control Utility
ifneq ($(strip $(BUILD_IQCTL)),)
iqctl: sanity_check
	$(MAKE) -C $@ $(BUILD_IQCTL)
else
iqctl: sanity_check
	@echo "Skipping iqctl application (Ingress QoS not configured)"
endif

# BPM Control Utility
ifneq ($(strip $(BUILD_BPMCTL)),)
bpmctl: sanity_check
	$(MAKE) -C $@ $(BUILD_BPMCTL)
else
bpmctl: sanity_check
	@echo "Skipping bpmctl application (BPM not configured)"
endif

# ARL Control Utility
ifneq ($(strip $(BUILD_ARLCTL)),)
arlctl:
	$(MAKE) -C $@
else
arlctl:
	@echo "Skipping arlctl application (not configured)"
endif

# EPON Control utility
ifneq ($(strip $(BUILD_EPONCTL)),)
eponctl: 
	$(MAKE) -C $@
else
eponctl:
	@echo "skipping eponctl (not configured)"
endif

# vcautohunt
ifneq ($(strip $(BUILD_MSTC_VCAUTOHUNT)),)
vcautohunt:
	$(MAKE) -C $@
else
vcautohunt:
	@echo "skipping vcautohunt (not configured)"
endif

#__MSTC__,Justin, Support TR143
udpechod:
	$(MAKE) -C $@

#__MSTC__,Justin, Support TR143
uploadd: sanity_check
	$(MAKE) -C $@

# __MSTC__, Richard Huang, celld, MitraStar Chehuai, for 3G application
ifneq ($(strip $(BUILD_MSTC_CELLD)),)
celld: wlan
	$(MAKE) -C $@
else
celld:  
	@echo "Skipping celld application (not configured)"
endif

# __MSTC__, Richard Huang, link_updown, MitraStar Chehuai, for 3G application
ifneq ($(strip $(BUILD_MSTC_LINK_UPDOWN)),)
link_updown: wlan celld
	$(MAKE) -C $@
else
link_updown:  
	@echo "Skipping link_updown application (not configured)"
endif

# hpna
ifneq ($(strip $(BUILD_HPNA)),)
hpna:
	$(MAKE) -C $@
else
hpna:
	@echo "skipping hpna (not configured)"
endif

#__MTS__,Ryan, Support SysDiag
sysdiagd: sanity_check

	$(MAKE) -C $@

#__MSTC__,Jeff, Support vlanautohunt
ifneq ($(strip $(BUILD_MSTC_VLANAUTOHUNT)),)
vlanautohunt: sanity_check
	$(MAKE) -C $@
else
	@echo "skipping vlanautohunt (not configured)"
endif

# router advertisement monitor
ifneq ($(strip $(BUILD_IPV6)),)
rastatus6:
	$(MAKE) -C rastatus6
else
rastatus6:
	@echo "skipping rastatus6 (not configured)"
endif

# GMAC Control Utility
ifneq ($(strip $(BUILD_GMACCTL)),)
gmacctl: sanity_check
	$(MAKE) -C $@ $(BUILD_GMACCTL)
else
gmacctl: sanity_check
	@echo "Skipping gmacctl application (GMAC configured)"
endif

ifneq ($(strip $(BUILD_MSTC_AUTO_SCHEDULING)),)
autoscheduling:
	$(MAKE) -C $@
else
autoscheduling:
	@echo "skipping schedule (not configured)"
endif

ifneq ($(strip $(MSTC_SW_WATCHDOG)),)
watchdog: sanity_check
	$(MAKE) -C $@
else
	@echo "skipping watchdog (not configured)"
endif

#configuration filter
ifneq ($(strip $(BUILD_MTSC_CONFIGURATION_FILTER)),)
zycfgfilter:
	$(MAKE) -C $@
else
zycfgfilter:
	@echo "skipping zycfgfilter (not configured)"
endif

ifneq ($(strip $(BUILD_MSTC_LINKSTATUS_HISTORY)),)
linkstatus:
	$(MAKE) -C $@
else
linkstatus:
	@echo "skipping linkstatus (not configured)"
endif
#linkstatus: sanity_check
	#$(MAKE) -C $@
#zyswitch cmd
ifneq ($(strip $(BUILD_MTSC_ZYSWITCH)),)
zyswitch:
	$(MAKE) -C $@
else
zyswitch:
	@echo "skipping zyswitch (not configured)"
endif

#zywlan cmd
ifneq ($(strip $(BUILD_MSTC_ZYWLAN)),)
zywlan:
	$(MAKE) -C $@
else
zywlan:
	@echo "skipping zywlan (not configured)"
endif

# TMS 802.3ah, 802.1ag, Y1731
ifneq ($(strip $(BUILD_TMS)),)
-include $(KERNEL_DIR)/.config
tmsctl:
	@ rm -f $@/bsp; ln -sn $(BRCMDRIVERS_DIR)/broadcom/char/tms/impl$(CONFIG_BCM_TMS_IMPL)/bsp $@/bsp
	@ rm -f $@/nci; ln -sn $(BRCMDRIVERS_DIR)/broadcom/char/tms/impl$(CONFIG_BCM_TMS_IMPL)/nci $@/nci
	$(MAKE) -C $@
else
tmsctl:
	@echo "skipping tms application (not configured)"
endif

#zycmd cmd
ifneq ($(strip $(BUILD_MSTC_ZYCMD)),)
zycmd:
	$(MAKE) -C $@
else
zycmd:
	@echo "skipping zycmd(not configured)"
endif


#getconfig cmd
ifneq ($(strip $(BUILD_NORWAY_CUSTOMIZATION)),)
getconfig:
	$(MAKE) -C $@
else
getconfig:
	@echo "skipping getconfig(not configured)"
endif

#loadconfig cmd
ifneq ($(strip $(BUILD_NORWAY_CUSTOMIZATION)),)
loadconfig:
	$(MAKE) -C $@
else
loadconfig:
	@echo "skipping loadconfig(not configured)"
endif

ifneq ($(strip $(BUILD_MSTC_REDUCE_SSK_LOCK_TIME)),)
set_iptables:
	$(MAKE) -C $@
else
set_iptables:
	@echo "Skipping set_iptables (not configured)"
endif
ifneq ($(strip $(BUILD_MSTC_REDUCE_BOOTUP_TIME)),)
bootup:
	$(MAKE) -C $@
else
bootup:
	@echo "Skipping set_iptables (not configured)"
endif