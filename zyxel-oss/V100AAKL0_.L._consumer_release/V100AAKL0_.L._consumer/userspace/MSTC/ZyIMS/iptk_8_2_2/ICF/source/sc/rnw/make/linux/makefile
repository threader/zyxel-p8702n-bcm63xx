#******************************************************************************
#        Makefile for RNW module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        Mar,2005         Amit Sharma                    Initial Version
#        30-Apr-2009        Anuradha Gupta   SPR 19443       Removing local options
#        
#
#
#        Copyright (c) 2009, Aricent .
#******************************************************************************
ICF_LIB= $(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE                         +=      -I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(GW_COMMON)/genlib/db/h \
					-I $(GW_COMMON)/genlib/os/h \
					-I $(ICF_SRC_DIR)/cfg/h \
					-I $(ICF_SRC_DIR)/dbm/h \
					-I $(ICF_SRC_DIR)/regm/h \
					-I $(ICF_SRC_DIR)/sc/rnw/h \
					-I .

SRCDIR	=	$(ICF_SRC_DIR)/sc/rnw/src
OBJDIR	=	$(ICF_SRC_DIR)/sc/rnw/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_rnw_fsm.c \
					$(SRCDIR)/icf_rnw_util.c \
					$(SRCDIR)/icf_rnw_main.c


OBJFILES	=			$(OBJDIR)/icf_rnw_fsm.o \
					$(OBJDIR)/icf_rnw_util.o \
					$(OBJDIR)/icf_rnw_main.o


CFLAGS		+=			-D__ANSI_C__  -DICF_RNW_SERVICE

TARGET=$(ICF_LIB)/librnw.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@

clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/librnw.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
