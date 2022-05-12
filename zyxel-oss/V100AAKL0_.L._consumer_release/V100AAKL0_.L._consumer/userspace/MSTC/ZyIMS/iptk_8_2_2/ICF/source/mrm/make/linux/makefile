#******************************************************************************
#        Makefile for MRM module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi                       Initial Version
#        30-Apr-2009        Anuradha Gupta   SPR 19443       Removing local options
#
#
#        Copyright (c) 2009, Aricent.
#******************************************************************************

ICF_LIB=$(ICF_DEV_DIR)/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk



INCLUDE					+=	-I $(ICF_SRC_DIR)/mrm/h\
                        -I $(NAT_LIB_COMMON)/ext_if/h\
						-I $(ICF_SRC_DIR)/mmi/h\
						-I $(ICF_SRC_DIR)/dbm/h



SRCDIR				=	$(ICF_SRC_DIR)/mrm/src

OBJDIR				=	$(ICF_SRC_DIR)/mrm/bin/$(CPU)



SRCFILES			=	$(SRCDIR)/icf_mrm.c





OBJFILES			=	$(OBJDIR)/icf_mrm.o



CFLAGS				+=	-D__ANSI_C__ 

# for unit testing include flag -D__MRM_UT__



TARGET=$(ICF_LIB)/libmrm.a



all     :$(TARGET)

$(TARGET):$(OBJFILES)

	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c

	$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@



clean	:

		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libmrm.a



depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)




include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.
