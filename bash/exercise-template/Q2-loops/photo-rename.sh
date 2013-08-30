#!/bin/bash

die ()
{
	echo "$@" 1>&2;
	exit -1;
}

USAGE="USAGE: $0 <DIRNAME>"

[ $# -le 1 ] && die $USAGE

TARGETDIR="$1"

cd $TARGETDIR
ls *.jpg -1 > .files.tmp

sed -i '/[0-9]\{8\}*.jpg/!d' .files.tmp
sed 's/\([0-9]\{2\}\)\([0-9]\{2\}\)\([0-9]\{4\}\)\(.*\)/\3-\2-\1-\4/' .files.tmp .newfiles.tmp

