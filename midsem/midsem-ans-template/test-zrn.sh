#!/bin/bash

# Run this script under 'sudo' to test the expected behaviour of
# zrn.sh
# This script must be run from the same directory as zrn.sh

# Marks: 2

rm -rf "testzrn"
mkdir "testzrn"
cp "./zrn.sh" "testzrn/"
chmod +x "testzrn/zrn.sh"

cd "testzrn"
sed '/^########## END/q' "./zrn.sh" > "./zrn-mod.sh"
echo "diff -q './zrn-temp.jpg' '../zrn-orig.jpg'" >> "./zrn-mod.sh"
chmod +x "./zrn-mod.sh"
./zrn-mod.sh
r=$?
if [ $r -eq 0 ]; then
    echo "Your script is most likely running right"
else
    echo "Your script isn't generating the exact file as expected"
fi
