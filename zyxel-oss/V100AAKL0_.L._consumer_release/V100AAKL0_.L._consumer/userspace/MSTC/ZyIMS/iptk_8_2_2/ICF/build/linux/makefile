#******************************************************************************
#        Makefile for ICF for Linux platform.
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        Feb,2005           Jalaj Negi                 Initial Version
#        13-Jul-2009        Rajiv Kumar     IPTK Rel-8.2     Changes done for
#                                                            Openssl Removal  
#        Copyright (c) 2009, Aricent 
#******************************************************************************

# The make to be done in Bourne shell
#
SHELL           =       /bin/sh

include $(ICF_DEV_DIR)/build/linux/makefile.mk

BASE_DIR	=	$(ICF_SRC_DIR)

ICF_LIB	=	$(ICF_DEV_DIR)/lib/linux

MODULES		=       $(BASE_DIR)/ssa \
			$(BASE_DIR)/mnl/nm \
			$(BASE_DIR)/mnl/platf_wrpr \
			$(BASE_DIR)/adm \
			$(BASE_DIR)/cc \
			$(BASE_DIR)/cfg \
			$(BASE_DIR)/cm \
			$(BASE_DIR)/dbm \
			$(BASE_DIR)/mmi \
			$(BASE_DIR)/mrm \
			$(BASE_DIR)/sm \
			$(BASE_DIR)/regm \
			$(BASE_DIR)/common \
			$(BASE_DIR)/port \
			$(BASE_DIR)/sic \
			$(BASE_DIR)/sc/scm \
			$(BASE_DIR)/sc/chr \
			$(BASE_DIR)/sc/twc \
			$(BASE_DIR)/sc/ctu \
			$(BASE_DIR)/sc/cta \
			$(BASE_DIR)/sc/rnw \
			$(BASE_DIR)/sc/xconf \
			$(BASE_DIR)/sc/cfw

iptk			: ipp_modules clib vldt stacks

iptk_tls		: ipp_modules clib vldt stacks_tls

clib			:
			@cd $(ICF_DEV_DIR)/source/icf_clib/bin; mkdir -p $(CPU);\
			cd ../build/linux; touch depend.intel; make all

vldt			:
			@cd $(ICF_DEV_DIR)/source/vldt/bin; mkdir -p $(CPU);\
			cd ../make/linux; touch depend.intel; make all

ipp_modules		: setup

			@for i in $(MODULES); \
			do \
			if      [       -d $$i/make     ] ; then \
				(echo "----> Making $$i\ncd $$i\/make\/linux; $(MAKE) all"; \
				cd $$i/make/linux; mkdir -p ../../bin/intel;touch depend.intel;$(MAKE) all ; \
				echo "----> Finished Making $$i <----\n"); \
			else \
				echo "Directory $$i\/make\/linux not found"; \
			fi; \
			done

stacks			:

			@cd $(STACK)/; $(MAKE) all ;

stacks_tls		:

			@cd $(STACK)/; $(MAKE) all_wt_tls ;

setup			:

			if [ -d $(ICF_LIB) ] ; \
			then \
				: ; \
			else \
				mkdir -p $(ICF_LIB) ; \
			fi

			for i in $(MODULES); \
			do \
			if      [       -d $$i/bin      ] ;  then \
				: ; \
			else \
				mkdir -p $$i/bin ; \
			fi ;\
			done

clean			: clean_clib clean_vldt clean_ipp clean_stacks

clean_clib		:

			@cd $(ICF_DEV_DIR)/source/icf_clib/build/linux; make clean; cd -;

clean_vldt		:

			@cd $(ICF_DEV_DIR)/source/vldt/make/linux; make clean; cd -;

clean_stacks		:

			@cd $(STACK) ; $(MAKE) allclean;

clean_ipp		:;

			for i in $(MODULES); \
			do \
			if      [       -d $$i  ] ; then \
				(echo "----> Making $$i\ncd $$i\/make\/linux; $(MAKE) clean"; \
				cd $$i/make/linux; $(MAKE) clean ; \
				echo "----> Finished Making $$i <----\n"); \
			else \
				echo "Directory $$i\/make\/linux not found"; \
			fi; \
			done

depend			:;

			@for i in $(MODULES); \
			do \
			if      [       -d $$i/make/linux     ] ; then \
				(echo "----> Making $$i\ncd $$i\/make\/linux; $(MAKE) depend"; \
				cd $$i/make/linux; rm -f depend.$(CPU); touch depend.$(CPU); \
				rm -f $$i/src/makefile.tmp*;  $(MAKE) depend ; \
				echo "----> Finished Making $$i <----\n"); \
			else \
				echo "Directory $$i not found"; \
			fi; \
			done

