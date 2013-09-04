#!/bin/bash

# This script should take exactly one argument, a file.  It should
# invoke emacs on the file if the file has a txt extension.  It should
# invoke firefox on the file if the file has a html extension.  It
# should invoke evince on the file if the file has a pdf extension.
# It should exit reporting error if the file has no extension or if it
# has some other extension.  The script should handle arguments which
# have multiple '.' characters.  For example, the extension for file
# a.b.txt is 'txt'.

usage="Usage: $0 <FILENAME>"

die ()
{
	echo "$@" 1>&2;
	exit -1;
}

[ $# -ne 1 ] && die $usage

[ `expr index '.' "$1"` -eq 0 ] && die "$1 doesn't have a valid extension."

FILE_TYPE=`echo ${1##*.} | tr 'A-Z' 'a-z'`

case "$FILE_TYPE" in
	txt|sh) APP='emacs'
		;;
	pdf|epub|ps) APP='evince'
		;;
	html) APP='firefox'
		;;
	.*)
		die '*.' $FILE_TYPE ' files not supported yet.'
		;;
esac

exec APP $1
