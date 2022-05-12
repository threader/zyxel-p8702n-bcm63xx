Read:
---
https://oldwiki.archive.openwrt.org/toh/zyxel/zyxel_p8702n or doc/
https://forum.archive.openwrt.org/viewtopic.php?id=51212
https://openwrt.org/toh/zyxel/vmg1312-b30a
https://github.com/openwrt-es/cfe-bcm63xx/blob/master/README.md
https://openwrt.org/docs/techref/bootloader/cfe
https://forum.openwrt.org/t/boot-to-cfe-prompt-from-ram-on-bcm63xx/88788
https://gist.github.com/goldyfruit/3a0c1500cb539fabe2c4105373b836ae
https://forum.kitz.co.uk/index.php?topic=13939.0
https://web.archive.org/web/20200930145735/openwrt.org/toh/zyxel/zyxel_p8702n
https://patchwork.ozlabs.org/project/devicetree-bindings/patch/1459684846-11308-1-git-send-email-noltari@gmail.com/
(bmips: rename BCM63168 to BCM63268 
BCM63168 and BCM63268 are very similar and Broadcom refers to them as BCM63268
in GPL sources (e.g. 63268_map_part.h).
Use alphabetical order for supported bmips quirks.
Add BCM63268 to devicetree documentation.)

https://openwrt.org/docs/techref/hardware/soc/soc.broadcom.bcm63xx
https://openwrt.org/docs/guide-developer/toolchain/use-buildsystem
---

Connect via ttl serial - ie. 'cu -l /dev/ttyUSB0 -s 115200' or putty

CFE> ATHE
<snip>
CFE> ATSE DSL-2492GNAU-B3BC

00044320D52F
OK
*** command status = 0

CFE> ATEN 1 820FFBDD

OK

Compile zynpass to generate bootloader unlock key - Thanks mr. goldyfruit!
gcc zynpass.c
./a.out 00044320D52F

*** command status = 0
CFE> ATHE
<snip>
