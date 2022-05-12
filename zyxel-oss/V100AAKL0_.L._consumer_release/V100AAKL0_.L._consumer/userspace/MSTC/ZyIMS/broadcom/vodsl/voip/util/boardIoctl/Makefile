ifeq ($(strip $(BUILD_VODSL)), static)
CFLAGS += -DBUILD_STATIC 
endif

EXTRA_CFLAGS += \
-I$(TOPDIR)/voip/inc \
-fno-exceptions 

ifneq ($(strip $(BUILD_MSTC_IVR)),)
EXTRA_CFLAGS += -DZYXEL_IVR_PORTING
endif

OBJS =  ivrFlashIoctl.o

boardIoctl.o: $(OBJS) 
	$(LD) $(EXTRA_LDFLAGS) -r -o $@ $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -c -o $@ $<

clean:
	rm -f $(OBJS) boardIoctl.o
