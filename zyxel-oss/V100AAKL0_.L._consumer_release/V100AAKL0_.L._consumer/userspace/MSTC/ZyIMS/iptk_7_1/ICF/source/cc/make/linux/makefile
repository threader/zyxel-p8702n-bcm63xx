#******************************************************************************

#        Makefile for CC module of ICF.

#

#

#

#        DATE               NAME            REFERENCE      REASON

#        ----               ----            ---------      ------

#        24-Feb-2005        Jalaj Negi                     Initial Version

#

#

#        Copyright (c) 2006, Aricent.

#******************************************************************************

ICF_LIB= $(ICF_DEV_DIR)/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk



INCLUDE					+=	-I $(GW_COMMON)/common/h \
						-I $(GW_COMMON)/genlib/common/h \
						-I $(GW_COMMON)/genlib/db/h \
						-I $(GW_COMMON)/genlib/os/h \
						-I $(ICF_SRC_DIR)/icf_extif \
						-I $(ICF_SRC_DIR)/dbm/h \
						-I $(ICF_SRC_DIR)/cfg/h \
						-I $(ICF_SRC_DIR)/mnl/nm/h \
						-I $(ICF_SRC_DIR)/regm/h \
						-I $(ICF_SRC_DIR)/common/h \
						-I $(ICF_SRC_DIR)/cc/h \
						-I $(ICF_SRC_DIR)/sm/h \
						-I .



SRCDIR				=	$(ICF_SRC_DIR)/cc/src

OBJDIR				=	$(ICF_SRC_DIR)/cc/bin/$(CPU)



SRCFILES			=	$(SRCDIR)/icf_cc_fsm_handler.c \
					$(SRCDIR)/icf_cc_util.c \
					$(SRCDIR)/icf_cc_media_util.c \
					$(SRCDIR)/icf_cc_send_adm.c \
					$(SRCDIR)/icf_cc_send_ssa.c \
					$(SRCDIR)/icf_cc_send_mmi.c \
					$(SRCDIR)/icf_cc_fsm_00.c \
					$(SRCDIR)/icf_cc_fsm_01.c \
					$(SRCDIR)/icf_cc_fsm_02.c \
					$(SRCDIR)/icf_cc_fsm_03.c \
					$(SRCDIR)/icf_cc_fsm_04.c \
					$(SRCDIR)/icf_cc_fsm_05.c \
					$(SRCDIR)/icf_cc_fsm_06.c \
					$(SRCDIR)/icf_cc_fsm_07.c \
					$(SRCDIR)/icf_cc_fsm_08.c \
					$(SRCDIR)/icf_cc_fsm_09.c \
					$(SRCDIR)/icf_cc_fsm_10.c \
					$(SRCDIR)/icf_cc_fsm_11.c \
					$(SRCDIR)/icf_cc_fsm_12.c \
					$(SRCDIR)/icf_cc_fsm_13.c \
					$(SRCDIR)/icf_cc_fsm_14.c \
					$(SRCDIR)/icf_cc_fsm_15.c \
					$(SRCDIR)/icf_cc_fsm_16.c \
					$(SRCDIR)/icf_cc_fsm_20.c





OBJFILES			=	$(OBJDIR)/icf_cc_fsm_handler.o \
					$(OBJDIR)/icf_cc_util.o \
					$(OBJDIR)/icf_cc_media_util.o \
					$(OBJDIR)/icf_cc_send_adm.o \
					$(OBJDIR)/icf_cc_send_ssa.o \
					$(OBJDIR)/icf_cc_send_mmi.o \
					$(OBJDIR)/icf_cc_fsm_00.o \
					$(OBJDIR)/icf_cc_fsm_01.o \
					$(OBJDIR)/icf_cc_fsm_02.o \
					$(OBJDIR)/icf_cc_fsm_03.o \
					$(OBJDIR)/icf_cc_fsm_04.o \
					$(OBJDIR)/icf_cc_fsm_05.o \
					$(OBJDIR)/icf_cc_fsm_06.o \
					$(OBJDIR)/icf_cc_fsm_07.o \
					$(OBJDIR)/icf_cc_fsm_08.o \
					$(OBJDIR)/icf_cc_fsm_09.o \
					$(OBJDIR)/icf_cc_fsm_10.o \
					$(OBJDIR)/icf_cc_fsm_11.o \
					$(OBJDIR)/icf_cc_fsm_12.o \
					$(OBJDIR)/icf_cc_fsm_13.o \
					$(OBJDIR)/icf_cc_fsm_14.o \
					$(OBJDIR)/icf_cc_fsm_15.o \
					$(OBJDIR)/icf_cc_fsm_16.o \
					$(OBJDIR)/icf_cc_fsm_20.o



CFLAGS				+=	-D__ANSI_C__


TARGET=$(ICF_LIB)/libcc.a



all     :$(TARGET)

$(TARGET):$(OBJFILES) 
	$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@



clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libcc.a


 
depend.$(CPU):
	    touch depend.$(CPU)

depend	: 
	  $(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU) 
	  $(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)

include depend.$(CPU)

#DO NOT DELETE THIS LINE -- make depend depends on it.


