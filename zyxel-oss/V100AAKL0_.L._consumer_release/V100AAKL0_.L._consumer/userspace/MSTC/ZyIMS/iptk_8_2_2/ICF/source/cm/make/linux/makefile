#******************************************************************************
#        Makefile for CM module of ICF.
#
#
#
#        DATE               NAME            REFERENCE      REASON
#        ----               ----            ---------      ------
#        24-Feb-2005        Jalaj Negi                     Initial Version
#        30-Apr-2009        Anuradha Gupta   SPR 19443     Removing local options
#
#
#        Copyright (c) 2009, Aricent.
#******************************************************************************
ICF_LIB= $(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE					+=	-I $(GW_COMMON)/common/h \
						-I $(GW_COMMON)/genlib/common/h \
						-I $(GW_COMMON)/genlib/db/h \
						-I $(GW_COMMON)/genlib/os/h \
						-I $(ICF_SRC_DIR)/dbm/h \
						-I $(ICF_SRC_DIR)/cm/h \
						-I .

SRCDIR				=	$(ICF_SRC_DIR)/cm/src
OBJDIR				=	$(ICF_SRC_DIR)/cm/bin/$(CPU)

SRCFILES			=	$(SRCDIR)/icf_cm_entry_pt.c \
					$(SRCDIR)/icf_cm_init.c \
					$(SRCDIR)/icf_cm_db.c \
					$(SRCDIR)/icf_cm_process.c \
					$(SRCDIR)/icf_cm_util.c


OBJFILES			=	$(OBJDIR)/icf_cm_entry_pt.o \
					$(OBJDIR)/icf_cm_init.o \
					$(OBJDIR)/icf_cm_db.o \
					$(OBJDIR)/icf_cm_process.o \
					$(OBJDIR)/icf_cm_util.o

CFLAGS				+=	-D__ANSI_C__

TARGET=$(ICF_LIB)/libcm.a

all	:$(TARGET)

$(TARGET):$(OBJFILES)
		$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
		$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@
clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libcm.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.
