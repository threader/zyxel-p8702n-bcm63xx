#******************************************************************************

#        Makefile for CC module of ICF.

#

#

#

#        DATE               NAME            REFERENCE      REASON

#        ----               ----            ---------      ------

#        24-Dec-2004        Rohit Aggarwal                 Initial Version

#        14-Jan-2005        Rohit Aggarwal                 Added file

#        11-MAY-2005        Ram Narayan                    Added Defs for 

#              						   Interface Traces

#

#

#        Copyright (c) 2006, Aricent.

#******************************************************************************

ICF_IFX_LIB = $(ICF_SRC_DIR)/ifx_al/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk



INCLUDE					+=	-I $(GW_COMMON)/common/h \
						-I $(GW_COMMON)/genlib/common/h \
						-I $(GW_COMMON)/genlib/db/h \
						-I $(GW_COMMON)/genlib/os/h \
						-I $(NAT_LIB_COMMON)/ext_if/h \
						-I $(NAT_LIB_COMMON)/source/nat_glb_mgmt/h \
						-I $(NAT_LIB_COMMON)/source/nat_common/h \
						-I $(ICF_SRC_DIR)/dbm/h \
						-I $(ICF_SRC_DIR)/port/h \
						-I $(ICF_SRC_DIR)/cfg/h \
						-I $(ICF_SRC_DIR)/sm/h \
						-I $(ICF_SRC_DIR)/ifx_al/es/h \
						-I $(ICF_SRC_DIR)/ifx_al/mm_al/h \
						-I $(ICF_SRC_DIR)/stacks/sip_micro_ua/microua_3_0/ua_3_0_src/source/ares_linux/h \
						-I .



SRCDIR				=	$(ICF_SRC_DIR)/ifx_al/es/src

OBJDIR				=	$(ICF_SRC_DIR)/ifx_al/es/bin/$(CPU)



SRCFILES			=	$(SRCDIR)/icf_es_main.c \
					$(SRCDIR)/icf_es_util.c \
					$(SRCDIR)/icf_es_int_trace.c \
					$(SRCDIR)/remov_error.c



OBJFILES			=	$(OBJDIR)/icf_es_main.o \
					$(OBJDIR)/icf_es_util.o \
					$(OBJDIR)/icf_es_int_trace.o \
					$(OBJDIR)/remov_error.o



CFLAGS				+=  -UICF_SSA_ES_UT -D__ANSI_C__ -DIIP -DIPV4 -D_CONSOLE



TARGET=$(ICF_IFX_LIB)/libes.a



all	:$(TARGET)

$(TARGET):$(OBJFILES)

	$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c

	$(CC) $(CFLAGS)  -c  $< -o $@





clean	:

		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_IFX_LIB)/libes.a



depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)




include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.

