#******************************************************************************
#        Makefile for CFG module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi                       Initial Version
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
						-I $(ICF_SRC_DIR)/mnl/nm/h \
						-I $(ICF_SRC_DIR)/dbm/h \
						-I $(ICF_SRC_DIR)/regm/h \
						-I $(ICF_SRC_DIR)/cfg/h \
						-I $(ICF_SRC_DIR)/port/h \
						-I $(ICF_SRC_DIR)/mmi/h \
						-I $(ICF_SRC_DIR)/mnl/platf_wrpr/h \
						-I $(ICF_SRC_DIR)/sm/h \
						-I .

SRCDIR				=	$(ICF_SRC_DIR)/mnl/nm/src
OBJDIR				=	$(ICF_SRC_DIR)/mnl/nm/bin/$(CPU)

SRCFILES			=	$(SRCDIR)/icf_nm_services.c\
					$(SRCDIR)/icf_nm_entry_pt.c

OBJFILES			=	$(OBJDIR)/icf_nm_services.o\
					$(OBJDIR)/icf_nm_entry_pt.o

CFLAGS				+=	-D__ANSI_C__

TARGET=$(ICF_LIB)/libnm.a

all     :$(TARGET)

$(TARGET):	$(OBJFILES)
		$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
		$(CC) $(CFLAGS)  -c  $< -o $@


clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libnm.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
