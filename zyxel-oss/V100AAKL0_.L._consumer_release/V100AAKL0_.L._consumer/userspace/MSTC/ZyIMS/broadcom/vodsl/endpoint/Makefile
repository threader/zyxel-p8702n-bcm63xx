
CFLAGS= -s -Os -fomit-frame-pointer
ifeq ($(strip $(BUILD_VODSL)), static)
CFLAGS += -DBUILD_STATIC
endif

EXTRA_CFLAGS += \
-fno-exceptions

OBJS = endpoint_user.o endpoint_test.o

endpoint.o: $(OBJS)
	$(LD) $(EXTRA_LDFLAGS) -r -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -c -o $@ $<

clean:
	rm -f $(OBJS) endpoint.o endpoint_test.o
