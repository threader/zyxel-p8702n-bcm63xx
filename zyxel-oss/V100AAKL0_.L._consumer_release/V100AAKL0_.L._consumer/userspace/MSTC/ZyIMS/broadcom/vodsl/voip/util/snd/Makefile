ifeq ($(strip $(BUILD_VODSL)), static)
CFLAGS += -DBUILD_STATIC
endif

EXTRA_CFLAGS += \
-I$(TOPDIR)/voip/inc \
-fno-exceptions

OBJS =  sndapi.o announce.o

snd.o: $(OBJS) 
	$(LD) $(EXTRA_LDFLAGS) -r -o $@ $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -c -o $@ $<

clean:
	rm -f $(OBJS) snd.o sndapi.o announce.o
