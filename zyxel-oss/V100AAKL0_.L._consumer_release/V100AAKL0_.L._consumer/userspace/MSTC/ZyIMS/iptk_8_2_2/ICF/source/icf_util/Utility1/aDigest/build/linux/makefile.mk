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
# 20-Jan-2009    Anuradha Gupta         None          Open Source Guideline changes
# 02-Mar-2009    Alok Tiwari           IPTK Rel8.1    Changes done to integrate
#                                                     with IPTK.
# 30-Apr-2009    Anuradha Gupta         SPR 19443     Integration of rules with IPTK      
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
include $(ICF_DEV_DIR)/build/linux/makefile.mk
#CPU = intel
export CMN_DEV_DIR = $(ADIGEST_DEV_DIR)

ADIGEST_SRC_DIR = $(ADIGEST_DEV_DIR)/source
CMN_SRC_DIR = $(ADIGEST_DEV_DIR)/../Common
ADIGEST_DEPEND_BUILD_PATH =  $(ICF_DEV_DIR)/build/linux/build.sh
CMN_DEPEND_BUILD_PATH =  $(ADIGEST_DEPEND_BUILD_PATH)

#CC=gcc

FTR_LD = $(ICF_LD) $(ICF_LD_FLAGS) 
FTR_RM = $(ICF_RM)
FTR_CP = $(ICF_CP)
FTR_CD = $(ICF_CD)
FTR_AR = $(ICF_AR) $(ICF_AR_FLAGS)
FTR_MV = $(ICF_MV)
FTR_RANLIB = ranlib

COPTIONS += -fPIC

INCLUDE		= -I$(CMN_SRC_DIR)/h \
				-I$(CRYPTO_BASE_DIR)/common \
				-I$(CRYPTO_BASE_DIR)/algo/md5/h \
				-I$(ADIGEST_DEV_DIR)/ext_if \
				-I$(ADIGEST_SRC_DIR)/port/h

#Flag: UTIL_TRACE_ENABLE:This will enable the tracing functionality

CFLAGS		+= $(COPTIONS) -DUTIL_PORT_LINUX  -UADIGEST_MD5_THIRD_PARTY -UUTIL_TRACE_ENABLE

FTR_LIBS	= -lsocket -lnsl


