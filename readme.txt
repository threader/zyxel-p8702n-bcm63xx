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
https://books.google.no/books?id=l3JIEAAAQBAJ&pg=PA13&lpg=PA13&dq=100AAJX14D0.bin.md&source=bl&ots=ZVkgWi3_oU&sig=ACfU3U3Qq8HI2KXKYWJKc7WrB2TO3HgWeQ&hl=en&sa=X&ved=2ahUKEwjC7OeZh7WFAxXoBxAIHVB3AL8Q6AF6BAgJEAM#v=onepage&q=00AAJX13D0.bin.md&f=false

(bmips: rename BCM63168 to BCM63268 
BCM63168 and BCM63268 are very similar and Broadcom refers to them as BCM63268
in GPL sources (e.g. 63268_map_part.h).
Use alphabetical order for supported bmips quirks.
Add BCM63268 to devicetree documentation.)

https://openwrt.org/docs/techref/hardware/soc/soc.broadcom.bcm63xx
https://openwrt.org/docs/guide-developer/toolchain/use-buildsystem
---

Firmware: 
Under zyxel-p8702n-bcm63xx/firmware/p8702/ are all the official Telenor Firmwares 
I have, the latest are missing, the latest known working not to be locked to Telenor WAN
is 100AAJX14D0.bin , to flash this, older, or different firmwares the router needs to be put
in a special mode, i'll get the techincal details later, but it involves holding the 'reset'
button untill a light blinks red, set a static ip via ethernet cable to i believe 192.160.1.x
and connect to 192.168.1.1. This should be in the linked documentation above.
---

Unlock Bootloader:

*Nix:
Compile zynpass to generate bootloader unlock key - Thanks mr. goldyfruit!
gcc zynpass.c -o zynpass
./zynpass 00044320D52F

Windows - Untested:
doc/jstic.com/Newsgroup/Zyxel/ZynPass.zip is a windows version from 2012:
---

Serial connection:
< I'll provice pictures or something and pinouts to connect >
Connect via ttl serial - ie. 'cu -l /dev/ttyUSB0 -s 115200' or putty

CFE> ATHE
<snip>
CFE> ATSE DSL-2492GNAU-B3BC

00044320D52F
OK
*** command status = 0

CFE> ATEN 1 820FFBDD

OK

*** command status = 0
CFE> ATHE

OPTIONAL: Make FS writeable (I believe, I read the documentation at the time but found this in a log)
CFE> ATBT 1
<snip>
