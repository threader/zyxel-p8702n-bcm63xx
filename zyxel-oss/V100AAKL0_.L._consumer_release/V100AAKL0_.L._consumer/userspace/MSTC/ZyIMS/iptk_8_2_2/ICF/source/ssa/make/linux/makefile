#******************************************************************************
#        Makefile for SSA module of ICF.
#
#
#
#        DATE              NAME            REFERENCE        REASON
#        ----              ----            ---------        ------
#        28-Jan,2005       Umang Singh                    Initial Version
#        
#        03-Mar-2009       Kamal Ashraf    IPTK Rel 8.1   changes done for 
#                                                         open source replacement
#        30-Apr-2009        Anuradha Gupta   SPR 19443    Removing local options     
#        25-Aug-2009        Rajiv Kumar      SPR: 19982   OPENSSL include Path
#                                                         removed for Linux
#
#        Copyright (c) 2009, Aricent.
#******************************************************************************
ICF_SRC_DIR = $(ICF_DEV_DIR)/source
STACK=$(ICF_SRC_DIR)/stacks/sip_micro_ua/microua_3_0
#CPU, ICF_SRC_DIR, GW_COMMON and STACKS should have been exported prior to invocation
# of this file.
#ICF_SRC_DIR->root for ICF code
#GW_COMMON->root for genlib code
#STACK->root for SIP STACK and UATK code

ICF_LIB = $(ICF_DEV_DIR)/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE                         +=	-I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(GW_COMMON)/genlib/db/h \
					-I $(GW_COMMON)/genlib/os/h \
					-I $(STACK)/ua_3_0_src/source/UAlib/h \
					-I $(STACK)/sip_stack_3_0_src/source/siplib/accessor/h \
					-I $(STACK)/sip_stack_3_0_src/source/siplib/common/h \
					-I $(STACK)/sip_stack_3_0_src/source/siplib/parser/h \
					-I $(STACK)/sip_stack_3_0_src/source/siplib/clone/h \
					-I $(STACK)/sip_stack_3_0_src/source/siplib/txn/h \
					-I $(ICF_SRC_DIR)/icf_extif \
					-I $(ICF_SRC_DIR)/port/h \
					-I $(ICF_SRC_DIR)/regm/h \
					-I $(ICF_SRC_DIR)/cfg/h \
					-I $(ICF_SRC_DIR)/mnl/nm/h \
					-I $(ICF_SRC_DIR)/common/h \
					-I $(ICF_SRC_DIR)/dbm/h \
					-I $(ICF_SRC_DIR)/ssa/h \
					-I $(ICF_SRC_DIR)/icf_clib/h \
					-I $(ICF_SRC_DIR)/icf_util/arDnsClient/source/port/h \
					-I $(ICF_SRC_DIR)/icf_util/arDnsClient/ext_if \
					-I .

SRCDIR	=	$(ICF_SRC_DIR)/ssa/src
OBJDIR	=	$(ICF_SRC_DIR)/ssa/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_ssa_uaccallbacks.c \
					$(SRCDIR)/icf_ssa_uascallbacks.c \
					$(SRCDIR)/icf_ssa_commoncallbacks.c \
					$(SRCDIR)/icf_ssa_apihandlers.c \
					$(SRCDIR)/icf_ssa_entry_point.c \
					$(SRCDIR)/icf_ssa_timer.c \
					$(SRCDIR)/icf_ssa_tcpip.c \
					$(SRCDIR)/icf_ssa_sdp_util.c \
					$(SRCDIR)/icf_ssa_dns_util.c \
					$(SRCDIR)/icf_ssa_util.c



OBJFILES	=			$(OBJDIR)/icf_ssa_uaccallbacks.o \
					$(OBJDIR)/icf_ssa_uascallbacks.o \
					$(OBJDIR)/icf_ssa_commoncallbacks.o \
					$(OBJDIR)/icf_ssa_apihandlers.o \
					$(OBJDIR)/icf_ssa_entry_point.o \
					$(OBJDIR)/icf_ssa_timer.o \
					$(OBJDIR)/icf_ssa_tcpip.o \
					$(OBJDIR)/icf_ssa_sdp_util.o \
					$(OBJDIR)/icf_ssa_dns_util.o \
					$(OBJDIR)/icf_ssa_util.o

CFLAGS		+=			-D__ANSI_C__ -D_REENTRANT

TARGET=$(ICF_LIB)/libssa.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@


clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libssa.a

depend.$(CPU):
		touch depend.$(CPU)

depend	:
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU) 
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)

include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
