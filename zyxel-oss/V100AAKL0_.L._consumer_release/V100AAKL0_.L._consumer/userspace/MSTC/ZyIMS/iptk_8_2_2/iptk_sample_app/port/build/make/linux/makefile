#***************************************************************************************
#               Top level makefile for Sample Program
#               
#
#                       
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        07/12/2007         Amit Sikka                       Initial Creation
#               
#        Copyright (c) 2007 Aricent Inc . All Rights Reserved
#               
#***************************************************************************************  
include		makefile.mk

IPTKAPP_EXE	=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)port$(IPTKAPP_SLASH)build$(IPTKAPP_SLASH)bin$(IPTKAPP_SLASH)sample_program.exe

all:
	$(MAKE) -f make_cfg -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C cfg$(IPTKAPP_SLASH) -C make$(IPTKAPP_SLASH) -C linux
	$(MAKE) -f make_core -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C core$(IPTKAPP_SLASH) -C make$(IPTKAPP_SLASH) -C linux
	$(MAKE) -f make_port -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C port$(IPTKAPP_SLASH) -C make$(IPTKAPP_SLASH) -C linux
	$(CC) -o $(IPTKAPP_EXE) $(IPTKAPP_LIB_PATH) $(IPTKAPP_AR)

clean_all:
	$(MAKE) -f make_cfg clean_all -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C cfg$(IPTKAPP_SLASH) -C make$(IPTKAPP_SLASH) -C linux
	$(MAKE) -f make_core clean_all -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C core$(IPTKAPP_SLASH) -C make$(IPTKAPP_SLASH) -C linux
	$(MAKE) -f make_port clean_all -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C ..$(IPTKAPP_SLASH) -C port$(IPTKAPP_SLASH) -C make$(IPTKAPP_SLASH) -C linux
	$(RM) $(IPTKAPP_EXE)
	


