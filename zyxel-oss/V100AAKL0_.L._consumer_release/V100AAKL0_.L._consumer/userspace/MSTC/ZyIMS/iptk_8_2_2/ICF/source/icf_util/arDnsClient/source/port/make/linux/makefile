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

PORT_DEV_DIR=$(ARDNSCLIENT_DEV_DIR)/source/port

INCLUDE     +=      -I $(PORT_DEV_DIR)/h

SRCDIR	=	$(PORT_DEV_DIR)/src
OBJDIR	=	$(PORT_DEV_DIR)/obj

SRCFILES	=			$(SRCDIR)/arDnsClient_port_ipc.c \
                        $(SRCDIR)/arDnsClient_port_utils.c \
                        $(SRCDIR)/arDnsClient_port_pack.c \
                        $(SRCDIR)/arDnsClient_port_unpack.c \
                        $(SRCDIR)/arDnsClient_port_query.c \
                        $(SRCDIR)/res_mkquery.c

OBJFILES	=			$(OBJDIR)/arDnsClient_port_ipc.o \
                        $(OBJDIR)/arDnsClient_port_utils.o \
                        $(OBJDIR)/arDnsClient_port_pack.o \
                        $(OBJDIR)/arDnsClient_port_unpack.o \
                        $(OBJDIR)/arDnsClient_port_query.o \
                        $(OBJDIR)/res_mkquery.o

TARGET=$(OBJDIR)/arDnsClient_port.o

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
