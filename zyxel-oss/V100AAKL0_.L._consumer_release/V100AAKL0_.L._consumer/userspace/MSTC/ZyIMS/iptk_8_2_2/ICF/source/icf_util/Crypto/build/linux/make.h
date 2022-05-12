######################################################################
# Copyright (C) 2008 Aricent Inc . All Rights Reserved                #
# ------------------------------------------                          #
# $Id: make.h,v 1.1 2009/04/10 14:16:57 gur16603 Exp $                     #
#    DESCRIPTION            : Specifies the options and modules to be #
#                             included for building the Aricent Crypto#
#######################################################################

WARNINGS = -Wall -W -Wno-unused-parameter 
BUILD_PATH = $(CRYPTO_BASE_DIR)/build
OBJ_PATH = $(CRYPTO_BASE_DIR)/build/obj
LIB_PATH = $(CRYPTO_BASE_DIR)/lib
CRYPTO_ALGO_DIR = $(CRYPTO_BASE_DIR)/algo

#The modules which needs to be compiled should be made YES here
MD5 = YES
AES = NO
DES = NO
SHA1 = NO
HMAC = NO


