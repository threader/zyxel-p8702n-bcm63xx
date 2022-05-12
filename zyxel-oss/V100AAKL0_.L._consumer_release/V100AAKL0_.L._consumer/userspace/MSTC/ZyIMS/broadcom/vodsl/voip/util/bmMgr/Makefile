ifeq ($(strip $(BUILD_VODSL)), static)
CFLAGS += -DBUILD_STATIC
endif

EXTRA_CFLAGS += \
-I$(TOPDIR)/voip/inc \
-fno-exceptions

OBJS = bmmgr.o

bmMgr.o: $(OBJS) 
	$(LD) $(EXTRA_LDFLAGS) -r -o $@ $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -c -o $@ $<

clean:
	rm -f $(OBJS) bmMgr.o
