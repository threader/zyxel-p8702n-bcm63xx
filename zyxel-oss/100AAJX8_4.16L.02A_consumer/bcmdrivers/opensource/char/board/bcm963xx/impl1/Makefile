# Makefile for the bcm963xx board drivers
#


obj-y := board.o bcm63xx_flash.o bcm63xx_led.o bcm63xx_gpio.o pushbutton.o

ifneq ($(strip $(BUILD_IEEE1905)),)
obj-y += ieee1905_module.o    
endif

ifneq ($(findstring _$(strip $(BRCM_CHIP))_,,_63268_63138_63148_6838_),)

ifneq ($(strip $(BUILD_6802_MOCA)),)
    obj-y += bbsi/bbsi.o    
    obj-y += bbsi/bbsiExecSeq.o   
    obj-y += moca/6802/init.o   
    
else
    obj-y += spidevices.o
endif

endif

ifneq ($(strip $(CONFIG_BCM_CPLD1)),)
    obj-y += bcmcpld1.o
	EXTRA_CFLAGS += -DCONFIG_BCM_CPLD1
endif

ifneq ($(strip $(BUILD_NORWAY_CUSTOMIZATION)),)
	EXTRA_CFLAGS += -DBUILD_NORWAY_CUSTOMIZATION
endif

ifneq ($(strip $(BUILD_BRCM_FTTDP)),)
    obj-y += dsldsp_operation.o
	EXTRA_CFLAGS += -DBRCM_FTTDP
endif
ifneq ($(strip $(CONFIG_MSTC_WDT)),)
	obj-y += mstc_wdt.o
endif

EXTRA_CFLAGS += -I. -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
ifneq ($(findstring _$(strip $(BRCM_CHIP))_,,_63138_63148_6838_),)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/pmc
endif



# __MSTC__, Paul Ho, for WLAN/WPS Button
ifneq ($(strip $(CONFIG_BCM_WLAN)),)
	ifneq ($(strip $(BUILD_MSTC_WLAN_BTN_PER)),)
		EXTRA_CFLAGS += -DMTS_WLAN_BTN_PER=$(BUILD_MSTC_WLAN_BTN_PER)
	endif
	ifneq ($(strip $(BUILD_MSTC_WPS_BTN_PER)),)
		EXTRA_CFLAGS += -DMTS_WPS_BTN_PER=$(BUILD_MSTC_WPS_BTN_PER)
	endif
	ifneq ($(strip $(BUILD_MSTC_WLAN_WPS_BTN_SEPARATE)),)
		EXTRA_CFLAGS += -DMSTC_WLAN_WPS_BTN_SEPARATE
	endif
	ifneq ($(strip $(BUILD_MSTC_WPS_BTN_SCE)),)
		EXTRA_CFLAGS += -DMSTC_WPS_BTN_SCE=$(BUILD_MSTC_WPS_BTN_SCE)
	endif
endif
# end, __MSTC__

#__MSTC__, FuChia, Reset button time
ifneq ($(strip $(MSTC_RESTORE_BTN_PER)),)
EXTRA_CFLAGS += -DCV_RESTORE_BTN_PER=$(MSTC_RESTORE_BTN_PER)
endif
#end, __MSTC__, FuChia


EXTRA_CFLAGS += -Werror -Wfatal-errors
EXTRA_CFLAGS += -g

ifneq ($(strip $(BUILD_NORWAY_BTN_LED_OFF)),)
EXTRA_CFLAGS += -DNORWAY_BTN_LED_OFF
endif

ifneq ($(strip $(CONFIG_BCM_WLAN)),)
	EXTRA_CFLAGS += -DWIRELESS
endif
# __MSTC__, RaynorChung: Support 963268, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11
ifeq ($(strip $(INC_NAND_FLASH_DRIVER)),1)
EXTRA_CFLAGS += -DINC_NAND_FLASH_DRIVER=$(INC_NAND_FLASH_DRIVER)
else
EXTRA_CFLAGS += -DINC_NAND_FLASH_DRIVER=0
endif
# end __MSTC__, RaynorChung: Support 963268, patch form SVN#3597 on http://svn.zyxel.com.tw/svn/CPE_SW1/BCM96368/trunk/P-870HA/branches/cht/fttb8/4.11 
# __MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
ifeq ($(strip $(MSTC_ROM_D)), y)
EXTRA_CFLAGS += -DMSTC_ROM_D
endif

ifeq ($(strip $(MSTC_OBM_IMAGE_DEFAULT)), y)
EXTRA_CFLAGS += -DMSTC_OBM_IMAGE_DEFAULT
endif
# end __MSTC__, Dennis ZyXEL OBM ImageDefault feature, zongyue
#__MSTC__, Richard Huang
ifeq ($(strip $(BUILD_MSTC_ALL_WAN_SAME_MAC)),y)
EXTRA_CFLAGS += -DMSTC_ALL_WAN_SAME_MAC
endif
ifeq ($(strip $(BUILD_11ac_throughput_patch_from_412L07)),y)
EXTRA_CFLAGS += -DBUILD_11ac_throughput_patch_from_412L07
endif
ifeq ($(strip $(BUILD_MSTC_DIFF_L2_SAME_MAC)),y)
EXTRA_CFLAGS += -DMSTC_DIF_L2_SAME_MAC
endif
ifeq ($(strip $(BUILD_MTSC_DUAL_IMAGE_SELECT)),y)
EXTRA_CFLAGS += -DMTSC_DUAL_IMAGE_SELECT
endif
ifeq ($(strip $(BUILD_NORWAY_CUSTOMIZATION)),y)
EXTRA_CFLAGS += -DMTSC_NORWAY_CUSTOMIZATION
endif

ifeq ($(strip $(BUILD_TMPC2_VOICE_REINJECTION)),y)
EXTRA_CFLAGS += -DBUILD_TMPC2_VOICE_REINJECTION
endif

#__MSTC__, Richard Huang
ifeq ($(strip $(BUILD_MSTC_MODIFY)),y)
EXTRA_CFLAGS += -DMSTC_MODIFY
endif
-include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s

