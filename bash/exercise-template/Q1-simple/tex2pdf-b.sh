#/bin/bash

# This program should compile the latex file given as argument, a tex
# file assumed to be in the current/working directory.  And if the
# latex compilation succeeds, it should tar-gzip the entire current
# directory (from the current directory) to a tgz file called
# paper.tgz.  It should then move the paper.tgz file to /tmp.

## I'm a fan of Perl.
die ()
{
	echo "$@" 1>&2;
	exit -1;
}

[ $# -lt 1 ] && die "Usage: $0 <FILENAME>"

TEXFILE=$1
FILE=`basename $1 .tex`
latex $TEXFILE
bibtex $FILE
latex $TEXFILE
# Why use if ... then .. fi when you can chain the commands?
# (Yes, there're reasons why, but none relevant to the current level.)
latex $TEXFILE && tar -cvzf paper.tgz * && mv paper.tgz 
