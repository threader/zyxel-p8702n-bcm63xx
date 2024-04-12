#!/bin/sh
rm -f conf.out
rm -f d.out

awk -f extract_from_conf.awk usb_modeswitch.conf > conf.out
cat conf.out | sort | uniq > 3g.1

cd usb_modeswitch.d

for i in *
do
    awk -f ../extract_from_d.awk "$i" >> ../d.out
done
cd ..
cat d.out | sort | uniq > 3g.2

cat 3g.2 >> 3g.1

cat 3g.1 | sort | uniq > 3g-modeswitch

cp -f ../../vendors/Ralink/RT3052/mwr/3g-modeswitch ../../vendors/Ralink/RT3052/mwr/3g-modeswitch.old
cp -f 3g-modeswitch ../../vendors/Ralink/RT3052/mwr/3g-modeswitch

rm -f conf.out
rm -f d.out
rm -f 3g.1
rm -f 3g.2