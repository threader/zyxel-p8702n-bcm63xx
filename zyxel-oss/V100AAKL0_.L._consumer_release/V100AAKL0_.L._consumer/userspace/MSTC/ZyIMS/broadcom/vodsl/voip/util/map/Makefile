ifeq ($(strip $(BUILD_VODSL)), static)
CFLAGS += -DBUILD_STATIC
endif

EXTRA_CFLAGS += \
-I$(TOPDIR)/voip/inc \
-fno-exceptions

OBJS =  map.o

maputil.o: $(OBJS) 
	$(LD) $(EXTRA_LDFLAGS) -r -o $@ $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -c -o $@ $<

clean:
	rm -f $(OBJS) maputil.o
