# File: bcmdrivers/opensource/char/bdmf/impl1/Makefile
#
# Makefile for Broadlight Device Management Framework (BDMF), includes build for CPE and Maple. CPE build includes BDMF
# kernel module, BDMF simulator library for x86, and BDMF shell user-space client.
#

# Set the following options according to configuration
src 				?= $(shell pwd)
BDMFDIR				?= $(src)
CONFIG_SYSTEM		?= linux
CONFIG_XGPON 		?= n
CONFIG_BDMF_SHELL	?= y
CONFIG_REMOTE_SHELL	?= n
CONFIG_DB			?= n
CONFIG_BDMF_RPC		?= n
CONFIG_RPC			?= n
CONFIG_TRANSPORT_SOCKET ?= y
CONFIG_TRANSPORT_SHM ?= n
CONFIG_TRANSPORT_PROXY ?= n
CONFIG_DRIVER_TEST	?= n
CONFIG_BDMF_CODEGEN ?= y
CONFIG_BDMF_HISTORY	?= y
 
REMOTE_SHELL_CC ?= gcc
REMOTE_SHELL_CFLAGS ?= -g -Wall -Werror -I$(BDMFDIR)/system -I$(BDMFDIR)/system/$(CONFIG_SYSTEM)
REMOTE_SHELL_LIBS = -lrt -lpthread 

#
# End of configuration section
#
EXTRA_INCLUDES  = -I$(BDMFDIR) -I$(BDMFDIR)/framework 
EXTRA_INCLUDES += -I$(BDMFDIR)/system -I$(BDMFDIR)/system/$(CONFIG_SYSTEM) 
EXTRA_INCLUDES += -I$(BDMFDIR)/platform/$(RDPA_PLATFORM)
EXTRA_INCLUDES += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
EXTRA_INCLUDES += -I$(INC_BRCMSHARED_PUB_PATH)/bcm963xx
EXTRA_INCLUDES += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
EXTRA_INCLUDES += -I$(KERNEL_DIR)/net/core

ifeq ("$(RDPA_PLATFORM)" ,"oren")
	EXTRA_INCLUDES += -I$(INC_BRCMSHARED_PUB_PATH)/rdp -I$(INC_BRCMSHARED_PUB_PATH)/rdp/base
endif
ifeq ("$(RDPA_PLATFORM)" ,"dsl")
	EXTRA_INCLUDES += -I$(INC_BRCMSHARED_PUB_PATH)/rdp -I$(INC_BRCMSHARED_PUB_PATH)/rdp/base
endif
ifneq (x"$(V)", x"1")
	SILENT_BUILD = @
endif

ifeq ("$(CONFIG_SYSTEM)", "sim")
	CC 	   ?= gcc
	AR	   ?= ar 
	CFLAGS = -g -Wall -Werror -std=gnu99 -m32 -march=i386 $(EXTRA_OPTIONS) $(EXTRA_INCLUDES) $(EXTRA_DEFINES)
	LFLAGS = -m32 -march=i386 -g 
	ARFLAGS = -cqs 
	LIBS   = -lrt -lpthread
	EXTRA_DEFINES += -DBDMF_SYSTEM_SIM
	REMOTE_SHELL_CFLAGS += -DBDMF_SYSTEM_SIM
	CONFIG_REMOTE_SHELL = y
	BDMF_OUTDIR = $(PROFILE_DIR)/sim/bdmf
	CC_CMD = $(CC) -c $(CFLAGS) -o
endif

OBJS = 	$(BDMF_OUTDIR)/framework/bdmf_init.o
OBJS += $(SYSTEM_OBJS) $(FRAMEWORK_OBJS) $(DRIVER_OBJS)
OBJS += $(BDMF_OUTDIR)/framework/bdmf_fini.o

ifeq ("$(CONFIG_SYSTEM)", "linux")
	CONFIG_BDMF_CODEGEN := n
	EXTRA_CFLAGS = -Werror $(EXTRA_INCLUDES) $(EXTRA_DEFINES)
	SYSTEM_OBJS += system/linux/bdmf_chrdev.o
	obj-$(CONFIG_BCM_BDMF) = bdmf.o
	bdmf-objs = $(OBJS)
	EXTRA_DEFINES += -DBDMF_SYSTEM_LINUX
	REMOTE_SHELL_CFLAGS += -DBDMF_SYSTEM_LINUX
	REMOTE_SHELL_CC = $(CROSS)gcc
	REMOTE_SHELL_DEPS += $(BDMFDIR)/system/$(CONFIG_SYSTEM)/bdmf_chrdev.h
	BDMF_OUTDIR = .
endif

FRAMEWORK_OBJS = \
	$(BDMF_OUTDIR)/framework/bdmf_attr.o		\
	$(BDMF_OUTDIR)/framework/bdmf_memarea.o	\
	$(BDMF_OUTDIR)/framework/bdmf_utils.o	\
	$(BDMF_OUTDIR)/framework/bdmf_type.o		\
	$(BDMF_OUTDIR)/framework/bdmf_object.o	\
	$(BDMF_OUTDIR)/framework/bdmf_trace.o

SYSTEM_OBJS += \
	$(BDMF_OUTDIR)/system/bdmf_session.o		\
	$(BDMF_OUTDIR)/system/bdmf_errno.o			\
	$(BDMF_OUTDIR)/system/$(CONFIG_SYSTEM)/bdmf_system.o \
	$(BDMF_OUTDIR)/platform/$(RDPA_PLATFORM)/bdmf_platform.o

SRCS = $(OBJS:.o=.c)
DOC_HDRS = 	$(wildcard framework/*.h)
DOC_HDRS += $(wildcard shell/*.h)
DOC_HDRS += $(wildcard db_engine/*.h)
DOC_HDRS += $(wildcard transport/*.h)
DOC_HDRS += $(wildcard rpc/*.h)
REFMAN_DIR = doc/latex
REFMAN_FILE = bdmf_refman.pdf
REFMAN = $(REFMAN_DIR)/$(REFMAN_FILE)

ifeq ("$(CONFIG_XGPON)", "y")
	EXTRA_DEFINES += -DCONFIG_XGPON
endif

ifeq ("$(RDPA_PLATFORM)", "maple")
	EXTRA_DEFINES += -DBDMF_PLATFORM_MAPLE
endif

ifeq ("$(RDPA_PLATFORM)", "dsl")
	EXTRA_DEFINES += -DBDMF_PLATFORM_DSL
ifeq ($(strip $(BRCM_CHIP)),63138)
	EXTRA_DEFINES += -DDSL_63138
endif
ifeq ($(strip $(BRCM_CHIP)),63148)
	EXTRA_DEFINES += -DDSL_63148
endif

endif

ifeq ("$(CONFIG_DB)", "y")
	EXTRA_DEFINES += -DBDMF_DB_ENGINE
	EXTRA_INCLUDES += -I$(BDMFDIR)/db_engine
	FRAMEWORK_OBJS += $(BDMF_OUTDIR)/db_engine/db_engine.o
endif

ifeq ("$(CONFIG_BDMF_SHELL)", "y")
	EXTRA_DEFINES += -DBDMF_SHELL
	EXTRA_INCLUDES += -I$(BDMFDIR)/shell
	FRAMEWORK_OBJS += $(BDMF_OUTDIR)/framework/bdmf_mon.o
	SYSTEM_OBJS += $(BDMF_OUTDIR)/shell/bdmf_shell.o 
	ifeq ("$(CONFIG_REMOTE_SHELL)", "y")
		EXTRA_DEFINES += -DBDMF_SHELL_SERVER
		SYSTEM_OBJS += $(BDMF_OUTDIR)/shell/bdmf_shell_server.o 
	endif
endif

ifeq ("$(CONFIG_BDMF_RPC)", "y")
	CONFIG_RPC = y
	EXTRA_DEFINES += -DBDMF_RPC
	FRAMEWORK_OBJS += $(BDMF_OUTDIR)/framework/bdmf_rpc.o
	FRAMEWORK_OBJS += $(BDMF_OUTDIR)/framework/bdmf_proxy.o 
	FRAMEWORK_OBJS += $(BDMF_OUTDIR)/framework/bdmf_server.o
endif

ifeq ("$(CONFIG_RPC)", "y")
	CONFIG_TRANSPORT = y
	EXTRA_INCLUDES += -I$(BDMFDIR)/rpc
	SYSTEM_OBJS += $(BDMF_OUTDIR)/rpc/bdmf_rpc_engine.o $(BDMF_OUTDIR)/rpc/bdmf_rpc_binary.o
ifeq ("$(CONFIG_BDMF_SHELL)", "y")
	SYSTEM_OBJS += $(BDMF_OUTDIR)/rpc/bdmf_rpc_mon.o
endif
endif

ifeq ("$(CONFIG_TRANSPORT)", "y")
	EXTRA_DEFINES += -DBDMF_TRANSPORT
	EXTRA_INCLUDES += -I$(BDMFDIR)/transport
	SYSTEM_OBJS	+= $(BDMF_OUTDIR)/transport/bdmf_transport.o

ifeq ("$(CONFIG_TRANSPORT_SOCKET)", "y")
	EXTRA_DEFINES += -DBDMF_TRANSPORT_SOCKET
	SYSTEM_OBJS	+= $(BDMF_OUTDIR)/transport/bdmf_transport_socket.o
endif
ifeq ("$(CONFIG_TRANSPORT_SHM)", "y")
	EXTRA_DEFINES += -DBDMF_TRANSPORT_SHM
	SYSTEM_OBJS	+= $(BDMF_OUTDIR)/transport/bdmf_transport_shm.o
endif
ifeq ("$(CONFIG_TRANSPORT_PROXY)", "y")
	EXTRA_DEFINES += -DBDMF_TRANSPORT_PROXY
	SYSTEM_OBJS	+= $(BDMF_OUTDIR)/transport/bdmf_transport_proxy.o
endif
ifeq ("$(CONFIG_BDMF_SHELL)", "y")
	SYSTEM_OBJS += $(BDMF_OUTDIR)/transport/bdmf_transport_mon.o
endif

endif

ifeq ("$(CONFIG_DRIVER_TEST)", "y")
	DRIVER_OBJS += $(BDMF_OUTDIR)/drivers/test1_drv.o $(BDMF_OUTDIR)/drivers/test2_drv.o
endif

ifeq ("$(CONFIG_SYSTEM)", "sim")
	SYSTEM_OBJS += $(BDMF_OUTDIR)/system/$(CONFIG_SYSTEM)/bdmf_buf.o
	LFLAGS += -lpthread
endif

ifeq ("$(CONFIG_BDMF_DEBUG)", "y")
	EXTRA_DEFINES += -DBDMF_DEBUG
endif

ifeq ("$(CONFIG_BDMF_CODEGEN)", "y")
	EXTRA_DEFINES += -DBDMF_CODEGEN
	FRAMEWORK_OBJS += $(BDMF_OUTDIR)/framework/bdmf_codegen.o
endif

ifeq ("$(CONFIG_BDMF_HISTORY)", "y")
	EXTRA_DEFINES += -DBDMF_HISTORY
	FRAMEWORK_OBJS += $(BDMF_OUTDIR)/framework/bdmf_history.o
endif

# 
# Simulation build
#
ifeq ("$(CONFIG_SYSTEM)", "sim")

bdmf_sim: $(BDMF_OUTDIR)/libbdmf.a $(BDMF_OUTDIR)/bdmf

$(BDMF_OUTDIR)/.dir_created: 
	mkdir -p $(BDMF_OUTDIR)/
	mkdir -p $(BDMF_OUTDIR)/framework
	mkdir -p $(BDMF_OUTDIR)/transport
	mkdir -p $(BDMF_OUTDIR)/system/sim
	mkdir -p $(BDMF_OUTDIR)/rpc
	mkdir -p $(BDMF_OUTDIR)/shell
	mkdir -p $(BDMF_OUTDIR)/platform/$(RDPA_PLATFORM)
	$(SILENT_BUILD)touch $(BDMF_OUTDIR)/.dir_created

-include $(OBJS:.o=.d)

$(BDMF_OUTDIR)/%.d: %.c Makefile $(BDMF_OUTDIR)/.dir_created
	@echo "Making dependency for $< --> $@"
	$(SILENT_BUILD)$(CC) -MM -MT $(BDMF_OUTDIR)/$(<:.c=.o) $(CFLAGS) $< | sed 's,\($*\.o\)[ :]*,\1 $@ : ,g' > $@

$(BDMF_OUTDIR)/%.o: %.c $(BDMF_OUTDIR)/.dir_created
	@echo "CC $< --> $@"
	$(SILENT_BUILD)$(CC_CMD) $@ $<

endif

$(BDMF_OUTDIR)/bdmf: $(OBJS) main.o
	@echo LD $(BDMF_OUTDIR)bdmf 
	$(SILENT_BUILD)$(CC) -o $@ $(LFLAGS) $(OBJS) main.o $(LIBS)

libbdmf.a: $(BDMF_OUTDIR)/libbdmf.a

$(BDMF_OUTDIR)/libbdmf.a: $(OBJS)
	$(SILENT_BUILD)$(AR) $(ARFLAGS) $@ $(OBJS)

docs: $(REFMAN) 

$(REFMAN): $(DOC_HDRS) Doxyfile DoxygenLayout.xml
	$(SILENT_BUILD)doxygen
	$(SILENT_BUILD)make -C $(REFMAN_DIR)
	$(SILENT_BUILD)mv -f $(REFMAN_DIR)/refman.pdf $(REFMAN)

clean:
	$(SILENT_BUILD)rm -fr `find $(BDMF_OUTDIR)/. -name '*.[o,d]'`
	$(SILENT_BUILD)rm -fr `find . -name '*~'`
	$(SILENT_BUILD)rm -fr `find . -name '*.ko'`
	$(SILENT_BUILD)rm -fr `find . -name '.*.cmd'`
	$(SILENT_BUILD)rm -fr bdmf.mod.c modules.order Module.symvers .tmp_versions
	$(SILENT_BUILD)rm -fr $(BDMF_OUTDIR)/bdmf $(BDMF_OUTDIR)/bdmf_shell $(BDMF_OUTDIR)/libbdmf.a

clobber: clean
	$(SILENT_BUILD)rm -fr doc
