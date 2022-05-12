#******************************************************************************
#        Top Level Makefile for Packetphone Product.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        25-Feb-2005        Jalaj Negi                       Initial Version
#        12-Apr-2005        Amit Sharma                      Added services
#        30-Jun-2006        Umang Singh                      Additions for TLS 
#        30-Apr-2008        Tarun Gupta     SPR 18496        Changed name from
#                                                            icf.exe to iptk.exe
#        10-Feb-2009        Alok Tiwari     ICF Rel-8.1      changes done for
#                                                    inclusion of shared library
#                                                    and port library of micro UATK
#                                                    and micro SIP-stack. 
#        03-Mar-2009        Rajiv Kumar     IPTK Rel-8.1     Changes done for
#                                                            replacing ARES
#                                                            with DNS CLIENT
#
#        13-Jul-2009        Rajiv Kumar     IPTK Rel-8.2     Changes done for
#                                                            Openssl Removal 
#        21-Aug-2009        Tarun Gupta     SPR 19932        Changed path of
#                                                            OPENSSL_LIB_PATH
#        25-Aug-2009        Rajiv Kumar     SPR: 19982       OPENSSL include Path
#                                                            removed for Linux
#       Copyright (c) 2009 Aricent .
#******************************************************************************
ICF_LIB = $(ICF_DEV_DIR)/lib/linux
ICF_BUILD=$(ICF_DEV_DIR)/build/linux
ICF_SRC_DIR = $(ICF_DEV_DIR)/source
ICF_IFX = $(ICF_SRC_DIR)/ifx_al
ICF_IFX_BIN = $(ICF_SRC_DIR)/ifx_al/bin/$(CPU)
ICF_IFX_LIB = $(ICF_SRC_DIR)/ifx_al/lib/linux
GW_COMMON = $(ICF_DEV_DIR)/../gw
NAT_LIB_COMMON = $(ICF_DEV_DIR)/utils/natlib
STACK = $(ICF_SRC_DIR)/stacks/sip_micro_ua/microua_3_0
GL_MAKE_DIR = $(GW_COMMON)/genlib/make/linux
GL_LIB = $(GW_COMMON)/lib/linux
NAT_LIB = $(NAT_LIB_COMMON)/lib/linux
STUN_LIB = -lnat
GENLIB_LIB  = -lgl_tml -lgl_ulml -lgl_html
STACK_LIB = $(STACK)/sip_stack_3_0_src/source/lib
UATK_LIB = $(STACK)/ua_3_0_src/source/lib
DNSCLIENT_LIB = $(ICF_SRC_DIR)/icf_util/arDnsClient/lib/linux
WRAPPERSSL_LIB = $(ICF_DEV_DIR)/../Wrapper_OPENSSL/lib/linux
ARDNSCLIENT_LIB = -larDnsClient
OPENSSL_LIB = $(BUILD_DIR)/userspace/public/libs/openssl/
SSL_LIBS = -lssl -lcrypto
WRAPPER_SSL_LIB = -lwrapperssl
SIP_LIB = -lssa -lua -luaport -lmicrosipaccessor -lmicrosipclone -lmicrosipcore -lmicrosipport -lua -luaport -lssa
IPP_LIB = -lmrm -lcfg -lregm -lcm -lcc -ldbm -lregm -lcm -lcfg -lsm -ladm $(SIP_LIB) -lmmi -lnm -lwrpr -lcmn -les -lport -lscm -lchr -ltwc -lctu -lcta -lrnw -lcfw -lclib -lcmn -lsic -lvldt -lxconf
IPP_AL = -les 
IPP_AR = $(ICF_LIB)/libmrm.a $(ICF_LIB)/libcfg.a \
	$(ICF_LIB)/libregm.a $(ICF_LIB)/libcm.a \
    $(ICF_LIB)/libcc.a $(ICF_LIB)/libdbm.a \
    $(ICF_LIB)/libsm.a \
    $(ICF_LIB)/libadm.a $(ICF_LIB)/libcmn.a \
    $(ICF_LIB)/libport.a $(ICF_LIB)/libmmi.a \
    $(ICF_LIB)/libes.a \
    $(ICF_LIB)/libscm.a $(ICF_IFX_LIB)/libchr.a \
    $(ICF_LIB)/libtwc.a $(ICF_IFX_LIB)/libctu.a \
	$(ICF_IFX_LIB)/libcta.a $(ICF_IFX_LIB)/librnw.a \
	$(ICF_IFX_LIB)/libcfw.a \
	$(ICF_IFX_LIB)/libxconf.a \
	$(ICF_IFX_LIB)/libclib.a \
    $(ICF_IFX_LIB)/libsic.a \
    $(ICF_LIB)/libmph.a \
    $(ICF_LIB)/libnm.a \
    $(ICF_LIB)/libvldt.a \
    $(ICF_LIB)/libwrpr.a

include $(STACK)/MakeVars
include $(STACK)/ua_3_0_src/source/MakeVars
include $(ICF_DEV_DIR)/build/linux/makefile.mk



INCLUDE				+=	-I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(GW_COMMON)/genlib/db/h \
					-I $(GW_COMMON)/genlib/os/h \
					-I $(NAT_LIB_COMMON)/ext_if/h \
					-I $(STACK)/ua_3_0_src/uatk_headers \
					-I $(STACK)/sip_stack_3_0_src/stack_headers \
					-I $(ICF_DEV_DIR)/ext_if \
					-I $(ICF_SRC_DIR)/common/h \
					-I $(ICF_SRC_DIR)/ssa/h \
                    -I $(ICF_SRC_DIR)/port/h \
					-I .

CFLAGS		+=	-D__ANSI_C__ -D_ANSI_C_ -DGW_SOLARIS -DGW_INTEGER_IS_32 -DGENLIB_TIMER -DICF_PORT_LINUX -DICF_UNIT_TEST -DICF_CHR_SERVICE

iptk_exe		: genlib ippstk port es dnsclient link_dns 

iptk_exe_tls	: genlib ippstk_tls port es dnsclient link_tls

iptk_exe_nat	: genlib ippstk port es dnsclient natlib link_dns_nat 

clean_exe	:
		#cd $(ICF_IFX)/bin/$(CPU) ; \rm -f iptk.exe
		cd $(ICF_IFX)/bin/$(CPU) ; \rm -f icf.exe

link_dns	:
		cd $(ICF_IFX)/bin ; mkdir -p $(CPU)/ ; \
#		$(CC) -o $(ICF_IFX_BIN)/iptk.exe  -L$(ICF_IFX_LIB) -L$(STACK_LIB) -L$(UATK_LIB) -L$(ICF_LIB) -L$(GL_LIB) -L$(DNSCLIENT_LIB) $(IPP_AL) $(IPP_LIB) $(GENLIB_LIB) -lpthread $(ARDNSCLIENT_LIB) -lresolv

		$(CC) -o $(ICF_IFX_BIN)/icf.exe  -L$(ICF_IFX_LIB) -L$(STACK_LIB) -L$(UATK_LIB) -L$(ICF_LIB) -L$(GL_LIB) -L$(DNSCLIENT_LIB) $(IPP_AL) $(IPP_LIB) $(GENLIB_LIB) -lpthread $(ARDNSCLIENT_LIB) -lresolv


link_dns_nat	:
		cd $(ICF_IFX)/bin ; mkdir -p $(CPU)/ ; \
#		$(CC) -o $(ICF_IFX_BIN)/iptk.exe  -L$(ICF_IFX_LIB) -L$(STACK_LIB) -L$(UATK_LIB) -L$(ICF_LIB) -L$(GL_LIB) -L$(DNSCLIENT_LIB) -L$(NAT_LIB) $(IPP_AL) $(IPP_LIB) $(GENLIB_LIB) $(STUN_LIB) -lpthread $(ARDNSCLIENT_LIB) -lresolv

		$(CC) -o $(ICF_IFX_BIN)/icf.exe  -L$(ICF_IFX_LIB) -L$(STACK_LIB) -L$(UATK_LIB) -L$(ICF_LIB) -L$(GL_LIB) -L$(DNSCLIENT_LIB) -L$(NAT_LIB) $(IPP_AL) $(IPP_LIB) $(GENLIB_LIB) $(STUN_LIB) -lpthread $(ARDNSCLIENT_LIB) -lresolv


link_tls	:
		cd $(ICF_IFX)/bin ; mkdir -p $(CPU)/ ; \
#		$(CC) -o $(ICF_IFX_BIN)/iptk.exe  -L$(ICF_IFX_LIB) -L$(STACK_LIB) -L$(UATK_LIB) -L$(ICF_LIB) -L$(GL_LIB) -L$(DNSCLIENT_LIB) -L$(WRAPPERSSL_LIB) $(IPP_AL) $(IPP_LIB) $(GENLIB_LIB) -lpthread $(ARDNSCLIENT_LIB) -lresolv $(WRAPPER_SSL_LIB) $(SSL_LIBS)

		$(CC) -o $(ICF_IFX_BIN)/icf.exe  -L$(ICF_IFX_LIB) -L$(STACK_LIB) -L$(UATK_LIB) -L$(ICF_LIB) -L$(GL_LIB) -L$(DNSCLIENT_LIB) -L$(WRAPPERSSL_LIB) -L$(OPENSSL_LIB) $(IPP_AL) $(IPP_LIB) $(GENLIB_LIB) -lpthread $(ARDNSCLIENT_LIB) -lresolv $(WRAPPER_SSL_LIB) $(SSL_LIBS)

gl_clean:
			@echo "###############################################"
			@echo "Cleaning GL Library for Timer Manager"
			@echo "###############################################"
			@cd $(GL_MAKE_DIR); \
			$(MAKE) -f makegl_os clean

			@echo "###############################################"
			@echo "Cleaning GL Library for Utility Manager"
			@echo "###############################################"
			@cd $(GL_MAKE_DIR); \
			$(MAKE) -f makegl_cmn clean

			@echo "###############################################"
			@echo "Cleaning GL Library for Hash Table Manager"
			@echo "###############################################"
			@cd $(GL_MAKE_DIR); \
			$(MAKE) -f makegl_db clean


genlib:
			@echo "###############################################"
			@echo "Building GL Library for Timer Manager"
			@echo "###############################################"
			@cd $(GL_MAKE_DIR); \
			$(MAKE) -f makegl_os gl_tm

			@echo "###############################################"
			@echo "Building GL Library for Utility Manager"
			@echo "###############################################"
			@cd $(GL_MAKE_DIR); \
			$(MAKE) -f makegl_cmn gl_ulm

			@echo "###############################################"
			@echo "Building GL Library for Hash table Manager"
			@echo "###############################################"
			@cd $(GL_MAKE_DIR); \
			CFLAGS_CMN=-UGL_STATIC_VALIDATION_ENABLED; \
			export CFLAGS_CMN; \
			$(MAKE) -f makegl_db gl_htm


port		:
			cd $(ICF_SRC_DIR)/port/make/linux/ ; \
			$(MAKE) all ;\

ippstk		:
			cd $(ICF_BUILD)/ ; \
			$(MAKE) iptk ;\

ippstk_tls	:
			cd $(ICF_BUILD)/ ; \
			$(MAKE) iptk_tls ;\

natlib		:
			cd $(NAT_LIB_COMMON)/build/linux/ ; mkdir -p bin/$(CPU); \
			$(MAKE);\

al		:	mmi_al

clean_al	:	clean_mmi_al

dnsclient		:
			@cd $(ICF_SRC_DIR)/icf_util/arDnsClient/build/linux ; \
			$(MAKE) all ;\
            
es		:
			@cd $(ICF_IFX)/lib ; mkdir -p linux/ ; \
			cd $(ICF_IFX)/es/bin ; mkdir -p $(CPU)/ ; \
			cd $(ICF_IFX)/es/make/linux/ ; touch depend.$(CPU) ;\
			$(MAKE) all ;\

clean		: 	clean_ipp clean_es clean_clib clean_vldt clean_exe clean_dns
 
clean_wt_natlib		: 	clean_ipp clean_es clean_clib clean_vldt clean_nat clean_dns clean_exe 

clean_ipp	:
			cd $(ICF_BUILD)/; \
			$(MAKE) clean ;

clean_clib	:
			cd $(ICF_SRC_DIR)/icf_clib/build/linux/; \
			$(MAKE) clean

clean_vldt	:
			cd $(ICF_SRC_DIR)/vldt/make/linux/; \
			$(MAKE) clean

clean_es	:
			cd $(ICF_IFX)/es/make/linux/ ; \
			$(MAKE) clean ;

clean_nat	:	
			cd $(ICF_DEV_DIR)/../utils/natlib/build/linux/ ; \
			$(MAKE) clean ;

clean_dns	:	
			cd $(ICF_SRC_DIR)/icf_util/arDnsClient/build/linux/ ; \
			$(MAKE) clean ;

depend		:	depend_ipp depend_genlib  depend_clib depend_vldt depend_es depend_natlib depend_stack

depend_ipp	:
			@cd $(ICF_DEV_DIR)/build/linux; \
			$(MAKE) depend ;

depend_genlib   :      
                    
			@cd $(GL_MAKE_DIR); \
			$(MAKE) -f makegl_cmn depend

			@cd $(GL_MAKE_DIR); \
                        CFLAGS_CMN=-UGL_STATIC_VALIDATION_ENABLED; \
                        export CFLAGS_CMN; \
			$(MAKE) -f makegl_os depend

			@cd $(GL_MAKE_DIR); \
                        CFLAGS_CMN=-UGL_STATIC_VALIDATION_ENABLED; \
                        export CFLAGS_CMN; \
			$(MAKE) -f makegl_db depend ;

depend_clib	:	
			@cd $(ICF_DEV_DIR)/source/icf_clib/build/linux; \
			$(MAKE) depend ;	   
	     
depend_vldt	:	
			@cd $(ICF_DEV_DIR)/source/vldt/make/linux; \
			$(MAKE) depend ;	   
	     
depend_es              :
			@cd $(ICF_IFX)/lib ; mkdir -p linux/ ; \
			cd $(ICF_IFX)/es/bin ; mkdir -p $(CPU)/ ; \
			cd $(ICF_IFX)/es/make/linux/ ; touch depend.$(CPU) ;\
			$(MAKE) depend ;

depend_natlib		:
			@cd $(ICF_DEV_DIR)/../utils/natlib/build/linux/ ; \
			$(MAKE) depend ;

depend_stack	:	
			@cd $(STACK)/ua_3_0_src/source/UAlib; \
			$(MAKE) depend ;
			@cd $(STACK)/sip_stack_3_0_src/source; \
			$(MAKE) depend;
# DO NOT DELETE THIS LINE -- make depend depends on it.




