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

QUERYH_DEV_DIR=$(ARDNSCLIENT_DEV_DIR)/source/queryh

INCLUDE     +=      -I $(QUERYH_DEV_DIR)/h

SRCDIR	=	$(QUERYH_DEV_DIR)/src
OBJDIR	=	$(QUERYH_DEV_DIR)/obj

SRCFILES	=			$(SRCDIR)/arDnsClient_queryh_cache.c \
                        $(SRCDIR)/arDnsClient_queryh_utils.c \
                        $(SRCDIR)/arDnsClient_queryh_parse.c \
                        $(SRCDIR)/arDnsClient_queryh_intf.c

OBJFILES	=			$(OBJDIR)/arDnsClient_queryh_cache.o \
                        $(OBJDIR)/arDnsClient_queryh_utils.o \
                        $(OBJDIR)/arDnsClient_queryh_parse.o \
                        $(OBJDIR)/arDnsClient_queryh_intf.o

TARGET=$(OBJDIR)/arDnsClient_queryh.o

all     :$(TARGET)

$(TARGET):$(OBJFILES)
		 $(FTR_LD) -o $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@


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
