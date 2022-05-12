## This libcap (for proftpd) is originally from libcap-1.10,
## at ftp://linux.kernel.org/pub/libs/security/linux-privs.
## This interface is SPECIFIC TO THE LINUX 2.2 KERNEL!!!  IT IS NOT GUARANTEED
## TO WORK ON ANY PRIOR OR LATER VERSION (ie: 2.1.x or 2.3.x).
## If this library stops working, please contact core@proftpd.org.

#
# defines
#
#topdir=$(shell pwd)/..
#include ../../Make.rules


TARGET = libcap.a
CFLAGS += -DHAVE_CONFIG_H -DLINUX -I../include
OBJS = cap_alloc.o cap_proc.o cap_extint.o cap_flag.o cap_text.o cap_sys.o

all: $(OBJS) cap_names.h
	$(AR) cr $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) *.o