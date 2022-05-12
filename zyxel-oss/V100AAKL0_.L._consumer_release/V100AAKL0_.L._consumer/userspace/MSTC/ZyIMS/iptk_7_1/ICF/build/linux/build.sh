#!/bin/bash
if [ $# -le 1 ]; then
echo "Usage: $0 <filename> <objdir> "
exit 127;
fi
PATTERN=`grep o: $1 | cut -d: -f1`

for i in $PATTERN
do
sed -e "s|$i|$2/$i|g" "$1" > "$1".bk
sed -e "s|gcc version |#gcc version|g" "$1" > "$1".bk
mv "$1".bk "$1"
done
