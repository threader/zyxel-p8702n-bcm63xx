#******************************************************************************
#                                    
# FILE NAME    : makefile                
#                                    
# DESCRIPTION  : Contains the linux makefile. 
#                                    
# Revision History :                            
#
#  DATE            NAME                 REFERENCE       REASON        
# ------         ------------           ---------     ----------
# 05-Feb-2009    Anuradha Gupta         None          Initial
# 06-Feb-2009    Anuradha Gupta         None          makefile
# 30-Apr-2009    Anuradha Gupta         SPR 19443     Integration of rules with IPTK
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
ARDNSCLIENT_DEV_DIR = $(ICF_DEV_DIR)/source/icf_util/arDnsClient

include $(ARDNSCLIENT_DEV_DIR)/build/linux/makefile.mk

# The make to be done in Bourne shell
#
SHELL           =       /bin/sh
ARDNSCLIENT_LIB	=	$(ARDNSCLIENT_DEV_DIR)/lib/linux

MODULES		=       $(ARDNSCLIENT_SRC_DIR)/clib \
			$(ARDNSCLIENT_SRC_DIR)/queryh \
			$(ARDNSCLIENT_SRC_DIR)/port

TARGET=$(ARDNSCLIENT_LIB)/libarDnsClient.a

ARDNSCLIENT_OBJ = $(ARDNSCLIENT_SRC_DIR)/port/obj/arDnsClient_port.o \
             $(ARDNSCLIENT_SRC_DIR)/clib/obj/arDnsClient_clib.o \
             $(ARDNSCLIENT_SRC_DIR)/queryh/obj/arDnsClient_queryh.o

all			: arDnsClient_modules link_arDnsClient 
arDnsClient			: arDnsClient_modules link_arDnsClient 

arDnsClient_modules		: setup

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

setup			:

			if [ -d $(ARDNSCLIENT_LIB) ] ; \
			then \
				: ; \
			else \
				mkdir -p $(ARDNSCLIENT_LIB) ; \
			fi



			for i in $(MODULES); \
			do \
			if      [       -d $$i/bin      ] ;  then \
				: ; \
			else \
				mkdir -p $$i/bin ; \
			fi ;\
			done


link_arDnsClient	:
		$(FTR_AR) $(TARGET) $(ARDNSCLIENT_OBJ)

clean			: clean_arDnsClient_mod clean_arDnsClient_lib

clean_arDnsClient_mod		:;

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

clean_arDnsClient_lib		:
			$(FTR_RM) $(ARDNSCLIENT_LIB)/*.a

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

