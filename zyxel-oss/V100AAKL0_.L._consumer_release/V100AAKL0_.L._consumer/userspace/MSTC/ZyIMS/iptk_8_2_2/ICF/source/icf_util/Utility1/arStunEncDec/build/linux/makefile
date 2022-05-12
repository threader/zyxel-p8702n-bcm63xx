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
# 05-Jan-2009    Anuradha Gupta         None          Initial
# 07-Dec-2009    Anuradha Gupta         None          Code Start
# 13-Apr-2009    Alok Tiwari           IPTK Rel8.2    patch merge to resolve
#                                                     issue linking with purify. 
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
#include $(ARSTUNENCDEC_DEV_DIR)/build/linux/makefile.mk
include $(ICF_DEV_DIR)/source/icf_util/Utility1/arStunEncDec/build/linux/makefile.mk

# The make to be done in Bourne shell
#
SHELL           =       /bin/sh
ARSTUNENCDEC_LIB	=	$(ARSTUNENCDEC_DEV_DIR)/lib/linux

MODULES		=       $(ARSTUNENCDEC_SRC_DIR)/encoder \
			$(ARSTUNENCDEC_SRC_DIR)/decoder

TARGET=$(ARSTUNENCDEC_LIB)/libarStunEncDec.a

ARSTUNENCDEC_OBJ = $(CMN_SRC_DIR)/obj/common.o \
             $(ARSTUNENCDEC_SRC_DIR)/encoder/obj/arStunEncDec_enc.o \
             $(ARSTUNENCDEC_SRC_DIR)/decoder/obj/arStunEncDec_dec.o

all			: arstunencdec_modules common link_arstunencdec 
arstunencdec			: arstunencdec_modules common link_arstunencdec 

common			:
			@cd $(CMN_SRC_DIR)/make/linux;\
			touch depend.intel; $(MAKE) clean; $(MAKE) all

arstunencdec_modules		: setup

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

			if [ -d $(ARSTUNENCDEC_LIB) ] ; \
			then \
				: ; \
			else \
				mkdir -p $(ARSTUNENCDEC_LIB) ; \
			fi



			for i in $(MODULES); \
			do \
			if      [       -d $$i/bin      ] ;  then \
				: ; \
			else \
				mkdir -p $$i/bin ; \
			fi ;\
			done


link_arstunencdec	:
		$(FTR_AR) $(TARGET) $(ARSTUNENCDEC_OBJ)

clean			: clean_common clean_arstunencdec_mod clean_arstunencdec_lib

clean_common		:
			$(MAKE) -w -C $(CMN_SRC_DIR)/make/linux clean

clean_arstunencdec_mod		:;

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

clean_arstunencdec_lib		:
			$(FTR_RM) $(ARSTUNENCDEC_LIB)/*.a

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

