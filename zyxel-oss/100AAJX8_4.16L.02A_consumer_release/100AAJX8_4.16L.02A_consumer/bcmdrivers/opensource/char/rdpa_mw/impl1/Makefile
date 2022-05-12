#
# <:copyright-BRCM:2013:DUAL/GPL:standard
# 
#    Copyright (c) 2013 Broadcom Corporation
#    All Rights Reserved
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation (the "GPL").
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# 
# A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
# writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
# 
# :> 
#
# Makefile for the RDPA MW
#
obj-$(CONFIG_BCM_RDPA_MW) := rdpa_mw.o

SRCS := $(subst rdpa_mw.mod.c,,$(notdir $(shell ls $(src)/*.c)))
OBJS := $(SRCS:.c=.o)
rdpa_mw-objs += $(OBJS)


ifeq ("$(RDPA_PLATFORM)" ,"oren")
	EXTRA_INCLUDES += -I$(INC_BRCMSHARED_PUB_PATH)/rdp -I$(INC_BRCMSHARED_PUB_PATH)/rdp/base
endif

ifeq ("$(RDPA_PLATFORM)" ,"dsl")
	EXTRA_INCLUDES += -I$(INC_BRCMSHARED_PUB_PATH)/rdp -I$(INC_BRCMSHARED_PUB_PATH)/rdp/base
endif

EXTRA_CFLAGS += -I$(BRCMDRIVERS_DIR)/opensource/net/enet/shared
EXTRA_CFLAGS += -I$(INC_BDMF_PATH)/platform/$(RDPA_PLATFORM) -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += $(EXTRA_INCLUDES) $(EXTRA_CFLAGS_RDPA)
EXTRA_CFLAGS += -Werror -Wall

-include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s .*.cmd *.ko *.mod.c modules.*

