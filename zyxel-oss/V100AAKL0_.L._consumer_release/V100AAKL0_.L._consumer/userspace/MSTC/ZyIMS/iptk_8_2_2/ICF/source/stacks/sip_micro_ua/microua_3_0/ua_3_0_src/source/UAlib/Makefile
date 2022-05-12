export CRYPTO_BASE_DIR=$(ICF_DEV_DIR)/source/icf_util/Crypto
export ARDNSCLIENT_BASE_DIR=$(ICF_DEV_DIR)/source/icf_util/arDnsClient
ICF_STACK_DIR=$(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0
include $(ICF_STACK_DIR)/MakeVars
include $(ICF_STACK_DIR)/ua_3_0_src/source/MakeVars
ICF_SRC_DIR=$(ICF_DEV_DIR)/source
INCLUDES	+=	-I $(ICF_DEV_DIR)/source/icf_util/arDnsClient/source/port/h
INCLUDES	+=	-I $(ICF_DEV_DIR)/source/icf_util/arDnsClient/ext_if
IPTK_INCLUDES=	-I$(ICF_DEV_DIR)/ext_if \
		-I$(ICF_SRC_DIR)/common/h \
		-I$(ICF_SRC_DIR)/ifx_al/es/h \
		-I$(ICF_SRC_DIR)/port/h \
		-I$(ICF_SRC_DIR)/mnl/nm/h \
		-I$(ICF_SRC_DIR)/ssa/h \
		-I$(ICF_SRC_DIR)/regm/h \
		-I$(ICF_SRC_DIR)/cfg/h \
		-I$(NAT_LIB_COMMON)/ext_if/h

WRAPPER_SSL_INCLUDE	+=	-I $(ICF_DEV_DIR)/../Wrapper_OPENSSL/h
OPENSSL_INCLUDE += -I $(BUILD_DIR)/userspace/public/libs/openssl/include

UASRC =	sdf_reqresp.c\
		sdf_init.c\
        sdf_txn.c\
		sdf.c\
		sdf_free.c\
		sdf_hash.c\
		sdf_list.c\
		sdf_debug.c\
		sdf_statistics.c\
		sdf_accessor.c\
		sdf_trace.c\
		sdf_configs.c\
		sdf_sdpinternal.c\
	 	sdf_authorization.c\
		sdf_sdp.c\
		sdf_internal.c\
		sdf_network.c\
		sdf_callapis.c\
		sdf_mempool.c\
		sdf_sdpinit.c\
		sdf_sdpfree.c\
		sdf_sdpaccessor.c\
		sdf_sessTimer.c

UALOOKUPSRC =	sdf_lookup.c

UAPORTSRC = sdf_networkport.c\
			sdf_lookupport.c \
			sdf_portlayer.c
	

UA_SRC =        src/sdf_reqresp.c\
		src/sdf_init.c\
        src/sdf_txn.c\
		src/sdf.c\
		src/sdf_free.c\
		src/sdf_hash.c\
		src/sdf_list.c\
		src/sdf_debug.c\
		src/sdf_statistics.c\
		src/sdf_accessor.c\
		src/sdf_trace.c\
		src/sdf_configs.c\
		src/sdf_sdpinternal.c\
		src/sdf_authorization.c\
		src/sdf_sdp.c\
		src/sdf_internal.c\
		src/sdf_network.c\
		src/sdf_callapis.c\
		src/sdf_mempool.c\
		src/sdf_sdpinit.c\
		src/sdf_sdpfree.c\
		src/sdf_sdpaccessor.c\
		src/sdf_sessTimer.c
UALOOKUP_SRC =   src/sdf_lookup.c

UAPORT_SRC = src/sdf_networkport.c\
                        src/sdf_lookupport.c \
                        src/sdf_portlayer.c

OBJECTS=$(UASRC:%.c=../obj/%.o) 
PORT_OBJECTS=$(UAPORTSRC:%.c=../obj/%.o) 
LOOKUPOBJECTS=$(UALOOKUPSRC:%c=../obj/%o)

OBJDIR  =       ../obj
LIBDIR  =	../lib

UALIBOBJS	=	$(OBJDIR)/sdf_accessor.o \
			$(OBJDIR)/sdf_authorization.o \
			$(OBJDIR)/sdf_callapis.o  \
			$(OBJDIR)/sdf_configs.o  \
			$(OBJDIR)/sdf_debug.o  \
			$(OBJDIR)/sdf_free.o  \
			$(OBJDIR)/sdf_hash.o  \
			$(OBJDIR)/sdf_init.o  \
			$(OBJDIR)/sdf_internal.o  \
			$(OBJDIR)/sdf_list.o  \
			$(OBJDIR)/sdf_lookup.o  \
			$(OBJDIR)/sdf_mempool.o  \
			$(OBJDIR)/sdf_network.o  \
			$(OBJDIR)/sdf.o  \
			$(OBJDIR)/sdf_reqresp.o  \
			$(OBJDIR)/sdf_sdpaccessor.o  \
			$(OBJDIR)/sdf_sdpfree.o  \
			$(OBJDIR)/sdf_sdpinit.o  \
			$(OBJDIR)/sdf_sdpinternal.o  \
			$(OBJDIR)/sdf_sdp.o  \
			$(OBJDIR)/sdf_sessTimer.o  \
			$(OBJDIR)/sdf_statistics.o  \
			$(OBJDIR)/sdf_trace.o  \
			$(OBJDIR)/sdf_txn.o \
			$(CRYPTO_BASE_DIR)/build/obj/crypto.o \
			$(ADIGEST_DEV_DIR)/../Common/obj/common.o \
			$(ADIGEST_DEV_DIR)/source/calc/obj/aDigestCalc.o \
			$(ADIGEST_DEV_DIR)/source/port/obj/aDigestPort.o
all: libMD5 libaDigest libarDnsClient libua libuaport

all_wt_tls: libMD5 libaDigest libarDnsClient wrapper_ssl libua libuaport_wt_tls

libMD5:
	@cd $(CRYPTO_BASE_DIR)/build/linux;make
    
libaDigest:
	@cd $(ADIGEST_DEV_DIR)/build/linux;make

libarDnsClient:
	@cd $(ARDNSCLIENT_BASE_DIR)/build/linux;make    

wrapper_ssl:
	@cd $(ICF_DEV_DIR)/../Wrapper_OPENSSL/make/linux;make

libua	: 
	${CC} -fPIC ${CFLAGS} ${INCLUDES} ${IPTK_INCLUDES} ${OPENSSL_INCLUDE} $(LIB) ${UA_SRC} ${UALOOKUP_SRC}
	mv *.o ../obj
	$(CC) -shared -rdynamic -o libua.so $(UALIBOBJS)
	mv libua.so $(LIBDIR)

libuaport: 
	${CC} ${CFLAGS} ${INCLUDES} ${IPTK_INCLUDES} $(LIB) ${UAPORT_SRC}
	mv *.o ../obj
	$(AR) $(LIBDIR)/libuaport.a $(PORT_OBJECTS)

libuaport_wt_tls: 
	${CC} ${CFLAGS} ${INCLUDES} ${WRAPPER_SSL_INCLUDE} ${OPENSSL_INCLUDE} ${IPTK_INCLUDES} $(LIB) ${UAPORT_SRC}
	mv *.o ../obj
	$(AR) $(LIBDIR)/libuaport.a $(PORT_OBJECTS)

depend.$(CPU):
		touch depend.$(CPU)

depend  :	
		touch depend1.intel depend2.intel 
		$(CC) -M $(CFLAGS) ${INCLUDES} ${IPTK_INCLUDES} $(wildcard $(UA_SRC)) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)	
		
		$(CC) -M $(CFLAGS) ${INCLUDES} ${IPTK_INCLUDES} $(wildcard $(UALOOKUP_SRC)) > depend2.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build2.sh  depend2.$(CPU) $(OBJDIR)

		$(CC) -M $(CFLAGS) ${INCLUDES} ${IPTK_INCLUDES} $(wildcard $(UAPORT_SRC)) > depend1.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend1.$(CPU) $(OBJDIR)

		@cd $(ADIGEST_DEV_DIR)/build/linux;make depend
		@cd $(CRYPTO_BASE_DIR)/build/linux;make depend
		@cd $(ARDNSCLIENT_BASE_DIR)/build/linux;make depend
		@cd $(ICF_DEV_DIR)/../Wrapper_OPENSSL/make/linux;make depend

		cat depend1.intel >> depend.intel
		cat depend2.intel >> depend.intel
		rm depend1.intel depend2.intel
clean:
	$(ICF_RM) ../obj/*.o 
	$(ICF_RM) ../lib/libua.so
	$(ICF_RM) ../lib/libuaport.a
	@cd $(ADIGEST_DEV_DIR)/build/linux;make clean
	@cd $(CRYPTO_BASE_DIR)/build/linux;make clean
	@cd $(ARDNSCLIENT_BASE_DIR)/build/linux;make clean
	@cd $(ICF_DEV_DIR)/../Wrapper_OPENSSL/make/linux;make clean

include depend.$(CPU)
#do not delete this line -Make depend depends on it.
