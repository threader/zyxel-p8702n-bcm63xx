#******************************************************************************
#        Makefile for PORT module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi                       Initial 
#        
#
#
#        Copyright (c) 2006, Aricent 
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
					-I $(OPEN_SSL_INCLUDE_PATH) \
					-I $(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0/ua_3_0_src/source/ares_linux/h \
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
					$(SRCDIR)/icf_port_ipc.c

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
					$(OBJDIR)/icf_port_ipc.o


CFLAGS		+=	-D_ANSI_C_ -DGW_INTEGER_IS_32 -DGENLIB_TIMER

TARGET=$(ICF_LIB)/libport.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@


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
