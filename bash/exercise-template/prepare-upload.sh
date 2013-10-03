#!/bin/bash

UPLOADDIR=forupload

if [ -d "$UPLOADDIR" ]; then
    echo "Directory '$UPLOADDIR' already exists, remove/rename it and rerun this script"
    exit 1
fi

if [ -f "$UPLOADDIR.tgz" ]; then
    echo "File '$UPLOADDIR.tgz' already exists, remove/rename it and rerun this script"
    exit 2
fi

FILES="
Q1-simple/myopen-a.sh
Q1-simple/myopen.sh
Q1-simple/tcd.sh
Q1-simple/tex2pdf-a.sh
Q1-simple/tex2pdf-b.sh
Q1-simple/tex2pdf-c.sh
Q1-simple/tex2pdf.sh
Q2-loops/myopen-generic-a.sh
Q2-loops/myopen-generic.sh
Q2-loops/photo-rename.sh
Q3-pipes/get-ip-a.sh
Q3-pipes/get-ip.sh
Q3-pipes/num-uploaded.sh
Q3-pipes/photo-dates-a.sh
Q3-pipes/photo-dates.sh
Q3-pipes/tar-scp.sh
"

for file in $FILES
do
    if [ ! -f $file ]; then
	echo "Error: cannot find file '$file'"
	echo "Even if you are not attempting the question, you need to have the original template sh file in the submission"
	echo "All 3 sub-directories Q1-simple Q2-loops Q3-pipes are expected to be in the current directory"
	exit 3
    fi
done

mkdir $UPLOADDIR
mkdir $UPLOADDIR/Q1-simple
mkdir $UPLOADDIR/Q2-loops
mkdir $UPLOADDIR/Q3-pipes

for file in $FILES
do
    cp $file $UPLOADDIR/$file
    echo "Copied file '$file' to '$UPLOADDIR'"
done

tar zcvf $UPLOADDIR.tgz $UPLOADDIR
echo "Created file '$UPLOADDIR.tgz' for uploading"

