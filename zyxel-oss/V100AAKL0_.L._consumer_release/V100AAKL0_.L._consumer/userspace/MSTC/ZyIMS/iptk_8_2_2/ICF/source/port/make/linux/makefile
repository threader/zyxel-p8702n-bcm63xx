#******************************************************************************
#        Makefile for PORT module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi                       Initial 
#        22-JAN-2008        Anurag Khare                     Change for new
#                                                            file icf_port_dhcp.c
#        03-Mar-2009        Rajiv            IPTK Rel 8.1    Changes done for
#                                                            replacing ARES
#                                                            with DNS CLIENT
#        30-Apr-2009        Anuradha Gupta   SPR 19443       Removing local options    
#        13-Jul-2009        Rajiv Kumar      IPTK Rel 8.2    Openssl Removal
#        25-Aug-2009        Rajiv Kumar      SPR: 19982      OPENSSL include Path
#                                                            removed for Linux
#       
#        Copyright (c) 2009, Aricent 
#******************************************************************************
ICF_LIB=$(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk
INCLUDE                         +=      -I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(GW_COMMON)/genlib/db/h \
					-I $(NAT_LIB_COMMON)/ext_if/h \
					-I $(ICF_SRC_DIR)/port/h \
					-I $(ICF_SRC_DIR)/ifx_al/es/h \
					-I $(ICF_SRC_DIR)/cc/h \
                    -I $(ICF_DEV_DIR)/../Wrapper_OPENSSL/h \
					-I $(ICF_DEV_DIR)/source/icf_util/arDnsClient/source/port/h \
					-I $(ICF_DEV_DIR)/source/icf_util/arDnsClient/ext_if \
					-I $(ICF_SRC_DIR)/mmi/h

SRCDIR	=	$(ICF_SRC_DIR)/port/src
OBJDIR	=	$(ICF_SRC_DIR)/port/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_port_memory.c \
					$(SRCDIR)/icf_port_timer.c \
					$(SRCDIR)/icf_port_util.c \
					$(SRCDIR)/icf_port_log_cc.c \
					$(SRCDIR)/icf_port_log_adm.c \
					$(SRCDIR)/icf_port_log_cf.c \
					$(SRCDIR)/icf_port_log_cfg.c \
					$(SRCDIR)/icf_port_log_chr.c \
					$(SRCDIR)/icf_port_log_cm.c \
					$(SRCDIR)/icf_port_log_cta.c \
					$(SRCDIR)/icf_port_log_ctua.c \
					$(SRCDIR)/icf_port_log_cw.c \
					$(SRCDIR)/icf_port_log_dbm.c \
					$(SRCDIR)/icf_port_log_mmi.c \
					$(SRCDIR)/icf_port_log_regm.c \
					$(SRCDIR)/icf_port_log_rfn.c \
					$(SRCDIR)/icf_port_log_scm.c \
					$(SRCDIR)/icf_port_log_sm.c \
					$(SRCDIR)/icf_port_log_ssa.c \
					$(SRCDIR)/icf_port_log_twc.c \
					$(SRCDIR)/icf_port_trace_error.c \
					$(SRCDIR)/icf_port_ipc.c \
					$(SRCDIR)/icf_port_dhcp.c 


OBJFILES	=			$(OBJDIR)/icf_port_memory.o \
					$(OBJDIR)/icf_port_timer.o \
					$(OBJDIR)/icf_port_util.o \
					$(OBJDIR)/icf_port_log_cc.o \
					$(OBJDIR)/icf_port_log_adm.o \
					$(OBJDIR)/icf_port_log_cf.o \
					$(OBJDIR)/icf_port_log_cfg.o \
					$(OBJDIR)/icf_port_log_chr.o \
					$(OBJDIR)/icf_port_log_cm.o \
					$(OBJDIR)/icf_port_log_cta.o \
					$(OBJDIR)/icf_port_log_ctua.o \
					$(OBJDIR)/icf_port_log_cw.o \
					$(OBJDIR)/icf_port_log_dbm.o \
					$(OBJDIR)/icf_port_log_mmi.o \
					$(OBJDIR)/icf_port_log_regm.o \
					$(OBJDIR)/icf_port_log_rfn.o \
					$(OBJDIR)/icf_port_log_scm.o \
					$(OBJDIR)/icf_port_log_sm.o \
					$(OBJDIR)/icf_port_log_ssa.o \
					$(OBJDIR)/icf_port_log_twc.o \
					$(OBJDIR)/icf_port_trace_error.o \
					$(OBJDIR)/icf_port_ipc.o \
					$(OBJDIR)/icf_port_dhcp.o 


CFLAGS		+=	-D_ANSI_C_ -DGW_INTEGER_IS_32 -DGENLIB_TIMER

TARGET=$(ICF_LIB)/libport.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@


clean	:
		$(ICF_RM) $(OBJDIR)/*.o
		$(ICF_RM) $(ICF_LIB)/libport.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
