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
# 03-Dec-2008    Anuradha Gupta         None          Initial
# 07-Dec-2008    Anuradha Gupta         None          Single Library Creation
# 20-Jan-2009    Anuradha Gupta         None          Open Source Guideline changes
# 02-Mar-2009    Alok Tiwari           IPTK Rel8.1    Changes done to integrate
#                                                     with IPTK. 
# 13-Apr-2009    Alok Tiwari           IPTK Rel8.2    patch merge to resolve
#                                                     linking with purify.       
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
export ADIGEST_DEV_DIR=$(ICF_DEV_DIR)/source/icf_util/Utility1/aDigest
export CRYPTO_BASE_DIR=$(ICF_DEV_DIR)/source/icf_util/Crypto
include $(ADIGEST_DEV_DIR)/build/linux/makefile.mk

# The make to be done in Bourne shell
#
SHELL           =       /bin/sh
BASE_DIR	=	$(ADIGEST_SRC_DIR)
ADIGEST_LIB	=	$(ADIGEST_DEV_DIR)/lib/linux

MODULES		=       $(BASE_DIR)/calc \
			$(BASE_DIR)/port

TARGET=$(ADIGEST_LIB)/libadigest.a

ADIGEST_OBJ = $(CMN_SRC_DIR)/obj/common.o \
             $(ADIGEST_SRC_DIR)/calc/obj/aDigestCalc.o \
             $(ADIGEST_SRC_DIR)/port/obj/aDigestPort.o

adigest			: adigest_modules common link_adigest 

common			:
			@cd $(CMN_SRC_DIR)/make/linux;\
			touch depend.intel; $(MAKE) clean;$(MAKE) all

adigest_modules		: setup

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

			if [ -d $(ADIGEST_LIB) ] ; \
			then \
				: ; \
			else \
				mkdir -p $(ADIGEST_LIB) ; \
			fi



			for i in $(MODULES); \
			do \
			if      [       -d $$i/bin      ] ;  then \
				: ; \
			else \
				mkdir -p $$i/bin ; \
			fi ;\
			done


link_adigest	:
		$(FTR_AR) $(TARGET) $(ADIGEST_OBJ)

clean			: clean_common clean_adigest_mod clean_adigest_lib

clean_common		:
			$(MAKE) -w -C $(CMN_SRC_DIR)/make/linux clean

clean_adigest_mod		:;
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

clean_adigest_lib		:
			$(FTR_RM) $(ADIGEST_LIB)/*.a

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

