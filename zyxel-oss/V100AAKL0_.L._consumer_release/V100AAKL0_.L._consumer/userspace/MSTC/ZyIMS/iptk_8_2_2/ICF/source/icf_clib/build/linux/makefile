#******************************************************************************
#        Makefile for clib module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi                       Initial Version
#        30-Apr-2009        Anuradha Gupta  SPR 19443        Removing local options
#
#
#        Copyright (c) 2006, Aricent.
#*****************************************************************i*************

ICF_SRC_DIR	=	$(ICF_DEV_DIR)/source	





ICF_LIB= $(ICF_DEV_DIR)/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk




INCLUDE					+=	 -I $(ICF_SRC_DIR)/icf_clib/h\
						-I $(ICF_SRC_DIR)/port/h\
						-I $(ICF_DEV_DIR)/ext_if\
						-I $(ICF_SRC_DIR)/vldt/h





SRCDIR				=	$(ICF_SRC_DIR)/icf_clib/src

OBJDIR				=	$(ICF_SRC_DIR)/icf_clib/bin/$(CPU)



SRCFILES			=	$(SRCDIR)/icf_lib_main.c\
					$(SRCDIR)/icf_lib_mm_main.c\
					$(SRCDIR)/icf_pack_mm_api.c\
					$(SRCDIR)/icf_unpack_mm_api.c\
					$(SRCDIR)/icf_unpack_api.c\
					$(SRCDIR)/icf_pack_api.c\
					$(SRCDIR)/icf_utils_mm.c\
					$(SRCDIR)/icf_utils.c

                                                





OBJFILES			=	$(OBJDIR)/icf_lib_main.o\
					$(OBJDIR)/icf_lib_mm_main.o\
					$(OBJDIR)/icf_pack_mm_api.o\
					$(OBJDIR)/icf_unpack_mm_api.o\
					$(OBJDIR)/icf_unpack_api.o\
					$(OBJDIR)/icf_pack_api.o\
					$(OBJDIR)/icf_utils_mm.o\
					$(OBJDIR)/icf_utils.o



CFLAGS				+=	-D__ANSI_C__



TARGET=$(ICF_LIB)/libclib.a



all     :$(TARGET)

$(TARGET):$(OBJFILES)

	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c

	$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@



clean	:

		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libclib.a



depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)




include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.
