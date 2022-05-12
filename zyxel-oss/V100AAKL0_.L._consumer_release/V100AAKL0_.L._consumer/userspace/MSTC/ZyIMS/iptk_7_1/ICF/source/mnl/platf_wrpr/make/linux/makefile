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
						-I $(ICF_SRC_DIR)/mnl/platf_wrpr/h \
						-I $(ICF_SRC_DIR)/mnl/nm/h \
						-I $(ICF_SRC_DIR)/dbm/h \
						-I $(ICF_SRC_DIR)/regm/h \
						-I $(ICF_SRC_DIR)/cfg/h \
						-I $(ICF_SRC_DIR)/port/h \
						-I $(ICF_SRC_DIR)/mmi/h \
						-I $(ICF_SRC_DIR)/sm/h \
						-I .

SRCDIR				=	$(ICF_SRC_DIR)/mnl/platf_wrpr/src
OBJDIR				=	$(ICF_SRC_DIR)/mnl/platf_wrpr/bin/$(CPU)

SRCFILES			=	$(SRCDIR)/wrapper.cpp
OBJFILES			=	$(OBJDIR)/wrapper.o
CFLAGS				+=	-D__ANSI_C__ -DWRAPPER_LINUX


TARGET=$(ICF_LIB)/libwrpr.a



all     :$(TARGET)

$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
		$(CC) $(CFLAGS)  -c  $< -o $@



clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libwrpr.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
