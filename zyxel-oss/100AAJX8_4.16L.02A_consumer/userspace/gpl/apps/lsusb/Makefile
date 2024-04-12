# CHIA 9/29/2011 porting
#CC=/opt/crosstool/arm-none-linux-gnueabi-4.4.0_ARMv5TE/bin/arm-none-linux-gnueabi-gcc
#ROOTDIR=../..
#CFLAGS += -I$(ROOTDIR)/lib/libusb-1.0.0 -L$(ROOTDIR)/lib/libusb-1.0.0/libusb/.libs
#CFLAGS += -I$(ROOTDIR)/lib/libusb-1.0.0
LDFLAGS += -lpthread -lusb

all: lsusb 

# I-Wei 05/07/2012 porting
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
include $(BUILD_DIR)/make.common
export BUILD_DIR
CFLAGS += -I$(BUILD_DIR)/userspace/gpl/libs/libusb-1.0.0 -L$(BUILD_DIR)/userspace/gpl/libs/libusb-1.0.0/libusb/.libs

lsusb: lsusb.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

# CHIA 9/29/2011 porting
romfs:
	cp -f lsusb $(INSTALL_DIR)/bin/lsusb
	cp -f librt-0.9.29.so $(INSTALL_DIR)/lib
	cp -f librt.so.0 $(INSTALL_DIR)/lib

clean:
	-rm -f lsusb *.o
