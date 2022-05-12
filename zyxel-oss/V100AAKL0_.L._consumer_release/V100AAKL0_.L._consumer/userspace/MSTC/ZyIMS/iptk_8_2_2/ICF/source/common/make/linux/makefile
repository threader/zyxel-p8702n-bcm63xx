#******************************************************************************
#        Makefile for Common module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi 	                     Initial 
#        30-Apr-2009        Anuradha Gupta   SPR 19443       Removing local options        
#
#
#        Copyright (c) 2009, Aricent.
#******************************************************************************

ICF_LIB=$(ICF_DEV_DIR)/lib/linux



include $(ICF_DEV_DIR)/build/linux/makefile.mk



INCLUDE                         +=	-I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(ICF_SRC_DIR)/dbm/h \
					-I $(ICF_SRC_DIR)/port/h \
					-I $(ICF_SRC_DIR)/sm/h \
					-I .



SRCDIR	=	$(ICF_SRC_DIR)/common/src

OBJDIR	=	$(ICF_SRC_DIR)/common/bin/$(CPU)



SRCFILES	=			$(SRCDIR)/icf_cmn_error.c \
					$(SRCDIR)/icf_cmn_stat.c \
					$(SRCDIR)/icf_cmn_util.c \
					$(SRCDIR)/icf_cmn_media_util.c \
					$(SRCDIR)/icf_cmn_trace.c



OBJFILES	=			$(OBJDIR)/icf_cmn_error.o \
					$(OBJDIR)/icf_cmn_stat.o \
					$(OBJDIR)/icf_cmn_util.o \
					$(OBJDIR)/icf_cmn_media_util.o \
					$(OBJDIR)/icf_cmn_trace.o





CFLAGS		+=	-D_ANSI_C_ -DGW_INTEGER_IS_32 -DGENLIB_TIMER -DGENLIB_MEMORY



TARGET=$(ICF_LIB)/libcmn.a



all     :$(TARGET)

$(TARGET):$(OBJFILES)

	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c

	$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@





clean	:

		$(ICF_RM) $(OBJDIR)/*.o
		$(ICF_RM) $(ICF_LIB)/libcmn.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)

include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.
