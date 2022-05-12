####################################################################
# 	This is the main makefile for the mu-Sip Protocol Stack   	
####################################################################
#
#	Creates libmicrosipcore
#		
####################################################################

ICF_STACK_DIR=$(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0
include $(ICF_DEV_DIR)/build/linux/makefile.mk
include $(ICF_STACK_DIR)/MakeVars
include $(ICF_STACK_DIR)/ua_3_0_src/source/MakeVars

COMMONSRC =microsip_list.c \
			microsip_init.c \
			microsip_free.c \
			microsip_bcptinit.c \
			microsip_bcptfree.c \
			microsip_trace.c \
			microsip_telinit.c \
			microsip_telfree.c \
			microsip_imurlinit.c \
			microsip_imurlfree.c \
			microsip_statistics.c 

PORTSRC = microsip_portlayer.c

PARSERSRC =	microsip_subparsers.c \
			microsip_decode.c \
			microsip_encode.c \
			microsip_hdrencoders.c \
			microsip_decodeinternal.c \
			microsip_encodeinternal.c \
			microsip_hdrparsers.c \
			microsip_sendmessage.c \
			microsip_telparser.c \
			microsip_imurl.c\
			microsip_msgbodyparser.c 
TXNSRC	= microsip_txnclone.c\
	microsip_txninit.c\
	microsip_txnfree.c\
	microsip_txndecodeintrnl.c\
	microsip_txnmidway.c\
	microsip_txndecode.c

ACCESSORSRC = microsip_cloneintrnl.c \
			 microsip_accessor.c	
CLONESRC = 	microsip_clone.c			 
MEMAPISRC = microsip_header.c \
 			microsip_imapi.c \
			microsip_msgbody.c \
			microsip_startline.c \
 			microsip_telapi.c\
			microsip_subapi.c 

TESTINCLUDE= -Itest/parser
INCLUDES =  -I$(ICF_STACK_DIR)/sip_stack_3_0_src/source/siplib/common/h \
			-I$(ICF_STACK_DIR)/sip_stack_3_0_src/source/siplib/accessor/h \
			-I$(ICF_STACK_DIR)/sip_stack_3_0_src/source/siplib/txn/h \
			-I$(ICF_STACK_DIR)/sip_stack_3_0_src/source/siplib/parser/h \
			-I$(ICF_STACK_DIR)/sip_stack_3_0_src/source/siplib/clone/h

IPTK_INCLUDES=	-I$(ICF_DEV_DIR)/ext_if \
		-I$(ICF_SRC_DIR)/common/h \
        -I$(STACK)/ua_3_0_src/source/UAlib/h \
		-I$(ICF_SRC_DIR)/port/h 

TXNINCLUDE= $(INCLUDES) $(TESTINCLUDE)			

COMMON_SRC = siplib/common/src/microsip_list.c \
			siplib/common/src/microsip_init.c \
			siplib/common/src/microsip_free.c \
			siplib/common/src/microsip_bcptinit.c \
			siplib/common/src/microsip_bcptfree.c \
			siplib/common/src/microsip_trace.c \
			siplib/common/src/microsip_telinit.c \
			siplib/common/src/microsip_telfree.c \
			siplib/common/src/microsip_imurlinit.c \
			siplib/common/src/microsip_imurlfree.c \
			siplib/common/src/microsip_statistics.c

PORT_SRC = siplib/common/src/microsip_portlayer.c

PARSER_SRC =     siplib/parser/src/microsip_subparsers.c \
                        siplib/parser/src/microsip_decode.c \
                        siplib/parser/src/microsip_encode.c \
                        siplib/parser/src/microsip_hdrencoders.c \
                        siplib/parser/src/microsip_decodeinternal.c \
                        siplib/parser/src/microsip_encodeinternal.c \
                        siplib/parser/src/microsip_hdrparsers.c \
                        siplib/parser/src/microsip_sendmessage.c \
                        siplib/parser/src/microsip_telparser.c \
                        siplib/parser/src/microsip_imurl.c\
                        siplib/parser/src/microsip_msgbodyparser.c
TXN_SRC  = siplib/txn/src/microsip_txnclone.c\
        siplib/txn/src/microsip_txninit.c\
        siplib/txn/src/microsip_txnfree.c\
        siplib/txn/src/microsip_txndecodeintrnl.c\
        siplib/txn/src/microsip_txnmidway.c\
        siplib/txn/src/microsip_txndecode.c

ACCESSOR_SRC = siplib/accessor/src/microsip_cloneintrnl.c \
                         siplib/accesssor/src/microsip_accessor.c
CLONE_SRC =      siplib/clone/src/microsip_clone.c
MEMAPI_SRC = siplib/accessor/src/microsip_header.c \
		siplib/accessor/src/microsip_imapi.c \
                        siplib/accessor/src/microsip_msgbody.c \
                        siplib/accessor/src/microsip_startline.c \
                        siplib/accessor/src/microsip_telapi.c\
                        siplib/accessor/src/microsip_subapi.c





COMMONOBJS 	= $(COMMONSRC:%.c=obj/common/%.o)
PORTOBJS 	= $(PORTSRC:%.c=obj/common/%.o) 
PARSEROBJS 	= $(PARSERSRC:%.c=obj/parser/%.o) 
ACCESSOROBJS 	= $(ACCESSORSRC:%.c=obj/accessor/%.o)
TXNOBJS		= $(TXNSRC:%.c=obj/txn/%.o)
CLONEOBJS 	= $(CLONESRC:%.c=obj/clone/%.o) 
MEMAPIOBJS = $(MEMAPISRC:%.c=obj/accessor/%.o)


OBJECTS 	= $(COMMONOBJS) $(PARSEROBJS) $(ACCESSOROBJS) $(TXNOBJS)
CLONEOBJECTS = $(CLONEOBJS)	
MEMAPIOBJECTS =	$(MEMAPIOBJS)


all: libmicrosipcore \
	libmicrosipclone\
	libmicrosipaccessor\
	libmicrosipport

libmicrosipcore: $(OBJECTS)
		$(CC) -shared -rdynamic -o libmicrosipcore.so $(OBJECTS)
		mv libmicrosipcore.so $(ICF_STACK_DIR)/ua_3_0_src/source/lib

libmicrosipclone: $(CLONEOBJS)
		$(CC) -shared -rdynamic -o libmicrosipclone.so $(CLONEOBJS)
		mv libmicrosipclone.so $(ICF_STACK_DIR)/ua_3_0_src/source/lib

libmicrosipaccessor: $(MEMAPIOBJS)
		$(CC) -shared -rdynamic -o libmicrosipaccessor.so $(MEMAPIOBJS)
		mv libmicrosipaccessor.so $(ICF_STACK_DIR)/ua_3_0_src/source/lib

libmicrosipport: lib/libmicrosipport.a


test: test/bin/siptest \
	test/bin/sendmessage

test/bin/siptest:lib/libmicrosipcore.so lib/libmicrosipport.a test/parser/microsip_txntimer.c test/parser/microsip_test.c
	$(PURIFYCC) test/parser/microsip_txntimer.c  test/parser/microsip_test.c $(CFLAGS) $(TXNINCLUDE) $(IPTK_INCLUDES) -o $@ $(LDFLAGS) -lmicrosipcore -lmicrosipport
test/bin/sendmessage: test/parser/sendmessage.c
	$(CC) $(CFLAGS) $(LDFLAGS) test/parser/sendmessage.c -o $@

lib/libmicrosipport.a: $(PORTOBJS)
	$(AR)  lib/libmicrosipport.a $(PORTOBJS)
	
#ZyXEL porting
ifeq ($(strip $(BUILD_ZYXEL_406_VOICE)),y)
obj/parser/%.o: siplib/parser/src/%.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/common/%.o: siplib/common/src/%.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/common/microsip_portlayer.o: siplib/common/src/microsip_portlayer.c
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/accessor/%.o: siplib/accessor/src/%.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/txn/%.o: siplib/txn/src/%.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/clone/%.o: siplib/clone/src/%.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/accessor/%.o: siplib/accessor/src/%.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
else
obj/parser/%.o: siplib/parser/src/%.c
#	$(PURIFYCC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/common/%.o: siplib/common/src/%.c
#	$(PURIFYCC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/common/microsip_portlayer.o: siplib/common/src/microsip_portlayer.c
#	$(PURIFYCC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/accessor/%.o: siplib/accessor/src/%.c
#	$(PURIFYCC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/txn/%.o: siplib/txn/src/%.c
#	$(PURIFYCC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/clone/%.o: siplib/clone/src/%.c
#	$(PURIFYCC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
obj/accessor/%.o: siplib/accessor/src/%.c
#	$(PURIFYCC) -fPIC $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
	$(CC) $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $< -o $@
endif
#ZyXEL porting end


clean:
	$(ICF_RM) obj/parser/*.o
	$(ICF_RM) obj/accessor/*.o
	$(ICF_RM) obj/common/*.o
	$(ICF_RM) obj/clone/*.o
	$(ICF_RM) obj/txn/*.o
	$(ICF_RM) lib/*.a
	$(ICF_RM) lib/*.so
	$(ICF_RM) lib/lib*.*
	$(ICF_RM) test/bin/siptest
	$(ICF_RM) test/bin/txntest
	$(ICF_RM) test/bin/sendmessage
	$(ICF_RM) test/bin/parsetime
	$(ICF_RM) test/bin/encodetime
	$(ICF_RM) test/bin/perftest

	$(ICF_RM) core

allclean: clean
	$(ICF_RM) test/bin/core


depend.$(CPU):
		touch depend.$(CPU) 




depend  :	
		touch depend1.intel depend2.intel depend3.intel depend4.intel depend5.intel

		$(CC) -M $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $(wildcard $(COMMON_SRC)) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) obj/common

		$(CC) -M $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $(wildcard $(PORT_SRC)) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) obj/common

		$(CC) -M $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $(wildcard $(PARSER_SRC)) > depend1.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend1.$(CPU) obj/parser
		
		$(CC) -M $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $(wildcard $(TXN_SRC)) > depend2.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend2.$(CPU) obj/txn

		$(CC) -M $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $(wildcard $(CLONE_SRC)) > depend3.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend3.$(CPU) obj/clone

		$(CC) -M $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $(wildcard $(ACCESSOR_SRC)) > depend4.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend4.$(CPU) obj/accessor

		$(CC) -M $(CFLAGS) $(INCLUDES) $(IPTK_INCLUDES) $(wildcard $(MEMAPI_SRC)) > depend5.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend5.$(CPU) obj/accessor
		cat depend1.intel >> depend.intel
		cat depend2.intel >> depend.intel
		cat depend3.intel >> depend.intel
		cat depend4.intel >> depend.intel
		cat depend5.intel >> depend.intel
		rm depend1.intel depend2.intel depend3.intel depend4.intel depend5.intel


include depend.$(CPU)
#Do not delete this line-Make depend depends on it.
