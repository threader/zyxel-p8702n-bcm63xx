ReadMe for P8702N V1.00(AAJX.8)


0. Introduction

  This file will show you how to build the  P8702N linux system,
  please note, the download image will overwrite the original image existed in the flash memory of EV board.


1. Package file 

   A. 100AAJX8_4.16L.02A_consumer_release.tar.gz     - P8702N open source code & Toolchains open source code
 
   B. ReadMe 					     - This file


2. Build up compiler environment.

   A. We suggest use the Linux distribution to setup your environment for reduce compatible issue.

	Install Ubuntu 12.04 Desktop 32bit

   B. Update system

       Use GUI interface to update system or use command to update system.

	$ sudo apt-get update && sudo apt-get upgrade

   C. Change default /bin/sh link to /bin/bash

	$ sudo dpkg-reconfigure dash
	choice "no" and pass enter.     


3. Prepare the compiler environment

   Install the following necessary tools 

	$ sudo apt-get install -y g++ flex bison gawk make autoconf zlib1g-dev libcurses-ocaml-dev libncurses-dev liblzo2-dev uuid-dev patch


4. Extract the source code and Install Toolchains
   NOTE: You must do following things as "root" 

	$ sudo su
	# tar -zxvf 100AAJX8_4.16L.02A_consumer_release.tar.gz
	# ./consumer_install
        Press "y" 3 time

	The source code has been installed in folder 100AAJX8 & Toolchain has been installed in /opt/toolchains/uclibc-crosstools-gcc-4.4.2-1


4. Build the firmware for Web-GUI upgrade using
   NOTE: You must do following things as "root" 

	# cd 100AAJX8
	# make PROFILE=DSL-2492GNAU-B3BC 

    The firmware image will locate at "100AAJX8/images".
    You can use it update P8702N by using the firmware update procedure.

