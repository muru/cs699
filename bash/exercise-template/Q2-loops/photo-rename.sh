#!/bin/bash

die ()
{
	echo "$@" 1>&2;
	exit -1;
}

USAGE="USAGE: $0 <DIRNAME>
Newlines in filenames aren't supported."


[[ $# -ne 1 ]] && die $USAGE
[[ ! -d $1 ]] && exit 2
TARGETDIR="$1"

cd $TARGETDIR

#/bin/ls -b *.jpg -1 | 
#grep -E '^[0-9]{8}.*.jpg$' | 
find -maxdepth 1 -regextype 'posix-egrep' -iregex '\.\/[0-9]{8}.*.jpg'| 
while read OLDFILE;
do
	NEWFILE=`echo $OLDFILE | sed 's/\([0-9]\{2\}\)\([0-9]\{2\}\)\([0-9]\{4\}\)\(.*\)/\3-\2-\1-\4/'`
	if [[ -n "$OLDFILE" && -n "$NEWFILE" ]] ; then
		echo -e "$OLDFILE" ' renamed as ' "$NEWFILE"
	#	mv $OLDFILE $NEWFILE
	fi
done


