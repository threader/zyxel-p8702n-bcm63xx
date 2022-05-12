

ifeq ($(strip $(CONFIG_BRCM_IKOS)),)

obj-$(CONFIG_BCM963138) += bcm_pinmux_impl1.o
obj-$(CONFIG_BCM963138) += bcm_led_impl1.o
obj-$(CONFIG_BCM963138) += bcm_gpio_impl1.o
obj-$(CONFIG_BCM963138) += bcm_misc_hw_init_impl1.o

obj-$(CONFIG_BCM963148) += bcm_pinmux_impl1.o
obj-$(CONFIG_BCM963148) += bcm_led_impl3.o
obj-$(CONFIG_BCM963148) += bcm_gpio_impl1.o
obj-$(CONFIG_BCM963148) += bcm_misc_hw_init_impl1.o

obj-$(CONFIG_BCM963381) += bcm_gpio_impl1.o
obj-$(CONFIG_BCM963381) += bcm_pinmux_impl1.o
obj-$(CONFIG_BCM963381) += bcm_led_impl1.o
obj-$(CONFIG_BCM963381) += bcm_misc_hw_init_impl2.o

obj-$(CONFIG_BCM960333) += bcm_led_impl2.o
obj-$(CONFIG_BCM960333) += bcm_pinmux_impl2.o

EXTRA_CFLAGS += -DCONFIG_BCM9$(BRCM_CHIP) -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)  -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)

-include $(TOPDIR)/Rules.make

endif

