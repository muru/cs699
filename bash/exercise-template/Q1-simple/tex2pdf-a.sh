#/bin/bash

# This program should compile the latex file given as argument, a tex
# file assumed to be in the current/working directory.  And it should
# tar-gzip the entire current directory (from the current directory)
# to a tgz file called paper.tgz.  It should then move the paper.tgz
# file to /tmp.

die ()
{
	echo "$@" 1>&2;
	exit -1;
}

[[ $# -lt 1 ]] && die "Usage: $0 <FILENAME>"

TEXFILE"=$1"
FILE=`basename $1 .tex`
latex $TEXFILE
bibtex $FILE
latex $TEXFILE
latex $TEXFILE

tar -cvzf paper.tgz * && mv paper.tgz 
