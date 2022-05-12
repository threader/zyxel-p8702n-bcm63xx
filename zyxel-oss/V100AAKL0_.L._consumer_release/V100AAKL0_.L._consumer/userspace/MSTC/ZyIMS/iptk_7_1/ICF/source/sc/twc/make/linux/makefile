#******************************************************************************
#        Makefile for TWC module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        Mar,2005         Amit Sharma                    Initial Version
#        
#
#
#        Copyright (c) 2006, Aricent .
#******************************************************************************
ICF_LIB= $(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk


INCLUDE                         +=      -I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(GW_COMMON)/genlib/db/h \
					-I $(GW_COMMON)/genlib/os/h \
					-I $(ICF_SRC_DIR)/extif \
					-I $(ICF_SRC_DIR)/common/h \
					-I $(ICF_SRC_DIR)/cfg/h \
					-I $(ICF_SRC_DIR)/regm/h \
					-I $(ICF_SRC_DIR)/dbm/h \
					-I $(ICF_SRC_DIR)/port/h \
					-I $(ICF_SRC_DIR)/sc/twc/h \
					-I .

SRCDIR	=	$(ICF_SRC_DIR)/sc/twc/src
OBJDIR	=	$(ICF_SRC_DIR)/sc/twc/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_twc_fsm.c \
					$(SRCDIR)/icf_twc_utils.c \
					$(SRCDIR)/icf_twc_main.c


OBJFILES	=			$(OBJDIR)/icf_twc_fsm.o \
					$(OBJDIR)/icf_twc_utils.o \
					$(OBJDIR)/icf_twc_main.o


CFLAGS		+=			-D__ANSI_C__  -DICF_TWC_SERVICE

TARGET=$(ICF_LIB)/libtwc.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@

clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libtwc.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
