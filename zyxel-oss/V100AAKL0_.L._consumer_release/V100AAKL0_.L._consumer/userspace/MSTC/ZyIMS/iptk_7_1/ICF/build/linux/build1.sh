#!/bin/bash
if [ $# -le 1 ]; then
echo "Usage: $0 <filename> <objdir> "
exit 127;
fi
PATTERN=`grep o: $1 | cut -d: -f1`

for i in $PATTERN
do
sed -e "s|$i|$2/$i|g" "$1" > "$1".bk
mv "$1".bk "$1"
done
sed -e "s|$2/sdf_lookup.ort.h|sdf_lookupport.h|g" "$1" > "$1".bk
mv "$1".bk "$1"

