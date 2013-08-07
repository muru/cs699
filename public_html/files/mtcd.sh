#! /bin/bash                                         
# ↄ⃝ Murukesh Mohanan
# This script will mount the arguments as ISO9660 in subdirectory of ~/cdrom.
# Given no argument, it will clean up ~/cdrom and unmount any mounted ISOs.  

ISODIR=$PWD
CDROOT=~/cdrom
if [ ! -d $CDROOT ]; then
    mkdir -p $CDROOT
fi

COUNT=`/usr/bin/ls -1 --reverse $CDROOT/ | head -1`
#cd $CDROOT

for ISO in $@; do
    let COUNT=$COUNT+1
    mkdir $CDROOT/$COUNT
    sudo mount -t iso9660 $ISO $CDROOT/$COUNT
    if [[ $? -ne 0 ]]; then
        rmdir $CDROOT/$COUNT
    fi
done

if [[ $# -eq 0 ]]; then
    cd $CDROOT
    for i in `/usr/bin/ls`; do
        sudo umount $i
        rmdir $i
    done
fi