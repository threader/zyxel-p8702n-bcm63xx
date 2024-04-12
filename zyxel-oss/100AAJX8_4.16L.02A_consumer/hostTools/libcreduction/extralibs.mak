#
# Extra run-time libraries
#
# Note: this file is only used if LIBOPT=n, called from libcreduction Makefile
# 
# Copyright (C) 2004 Broadcom Corporation
#


TARGETS := $(LIBDIR)/$(LIBC) $(LIBDIR)/$(LINKER)

TARGETS += $(LIBDIR)/libresolv.so.0
TARGETS += $(EXTRALIBDIR)/libgcc_s.so.1

TARGETS += $(LIBDIR)/libcrypt.so.0
TARGETS += $(LIBDIR)/libutil.so.0
TARGETS += $(LIBDIR)/libm.so.0
TARGETS += $(LIBDIR)/librt.so.0
TARGETS += $(LIBDIR)/libpthread.so.0


#
# These following libraries will only be included if certain features are
# enabled.
#

INCLUDE_LIBTHREAD := n
INCLUDE_STDCPP := n


ifneq ($(strip $(BUILD_GDBSERVER)),)
  INCLUDE_LIBTHREAD := y
endif

ifneq ($(strip $(BUILD_OPROFILE)),)
  INCLUDE_STDCPP := y
endif

ifneq ($(strip $(BUILD_BMU)),)
  INCLUDE_STDCPP := y
endif


ifeq ($(strip $(INCLUDE_LIBTHREAD)),y)
  TARGETS += $(LIBDIR)/libthread_db.so.1
endif

ifeq ($(strip $(INCLUDE_STDCPP)),y)
  TARGETS += $(EXTRALIBDIR)/libstdc++.so.6
endif
