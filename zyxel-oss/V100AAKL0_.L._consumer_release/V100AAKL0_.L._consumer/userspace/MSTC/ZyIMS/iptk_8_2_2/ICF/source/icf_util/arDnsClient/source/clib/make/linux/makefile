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
# 06-Feb-2009    Anuradha Gupta         LLD           makefile
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
ARDNSCLIENT_DEV_DIR = $(ICF_DEV_DIR)/source/icf_util/arDnsClient
include $(ARDNSCLIENT_DEV_DIR)/build/linux/makefile.mk

CLIB_DEV_DIR=$(ARDNSCLIENT_DEV_DIR)/source/clib

INCLUDE     +=      -I $(CLIB_DEV_DIR)/h

SRCDIR	=	$(CLIB_DEV_DIR)/src
OBJDIR	=	$(CLIB_DEV_DIR)/obj

SRCFILES	=			$(SRCDIR)/arDnsClient_clib_intf.c \
                        $(SRCDIR)/arDnsClient_clib_utils.c

OBJFILES	=			$(OBJDIR)/arDnsClient_clib_intf.o \
                        $(OBJDIR)/arDnsClient_clib_utils.o

TARGET=$(OBJDIR)/arDnsClient_clib.o

all     :$(TARGET)

$(TARGET):$(OBJFILES)
		 $(FTR_LD) -o $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@


clean	:
		$(FTR_RM) $(OBJDIR)/*.o

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ARDNSCLIENT_DEPEND_BUILD_PATH)  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
#
#
