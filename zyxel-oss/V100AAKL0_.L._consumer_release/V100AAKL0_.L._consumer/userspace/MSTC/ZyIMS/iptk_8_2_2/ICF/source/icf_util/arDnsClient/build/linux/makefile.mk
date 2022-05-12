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
# 06-Feb-2009    Anuradha Gupta         None          Makefile 
# 30-Apr-2009    Anuradha Gupta         SPR 19443     Integration of rules with IPTK
#
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
include $(ICF_DEV_DIR)/build/linux/makefile.mk


ARDNSCLIENT_DEV_DIR = $(ICF_DEV_DIR)/source/icf_util/arDnsClient
ARDNSCLIENT_SRC_DIR = $(ARDNSCLIENT_DEV_DIR)/source
ARDNSCLIENT_DEPEND_BUILD_PATH =  $(ICF_DEV_DIR)/build/linux/build.sh

FTR_LD = $(ICF_LD) $(ICF_LD_FLAGS) 
FTR_RM = $(ICF_RM)
FTR_CP = $(ICF_CP)
FTR_CD = $(ICF_CD)
FTR_AR = $(ICF_AR) $(ICF_AR_FLAGS)
FTR_MV = $(ICF_MV)
FTR_RANLIB = ranlib


INCLUDE		= -I$(ARDNSCLIENT_SRC_DIR)/port/h \
				-I$(ARDNSCLIENT_SRC_DIR)/common/h \
				-I$(ARDNSCLIENT_DEV_DIR)/ext_if

#Flag: ARDNSCLIENT_TRACE_ENABLE:This will enable the tracing functionality

CFLAGS		+=  -D_ANSI_C_ -DARDNSCL_PORT_LINUX -UARDNSCL_TRACE_ENABLE

