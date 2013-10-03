#!/bin/bash

UPLOADDIR=forupload-revised

if [ -d "$UPLOADDIR" ]; then
    echo "Directory '$UPLOADDIR' already exists, remove/rename it and rerun this script"
    exit 1
fi

if [ -f "$UPLOADDIR.tgz" ]; then
    echo "File '$UPLOADDIR.tgz' already exists, remove/rename it and rerun this script"
    exit 2
fi

FILES="\
 cpnew-revised.sh\
 genplot-revised.sh\
 iitb-papers-revised.sh\
 tabulate-marks-revised.sh\
 zrn-revised.sh\
"

if [ ! -f "complaints.txt" ]; then
    echo "Expecting to find file complaints.txt explaining your complaints"
    exit 3
fi

for file in $FILES
do
    if [ -f $file ]; then
	echo "Found $file for revised submission"
    else
	echo "$file not found; perhaps you don't have a complaint in that question, that's great!"
    fi
done

mkdir $UPLOADDIR

cp "complaints.txt" $UPLOADDIR/
echo "Copied 'complaints.txt' to '$UPLOADDIR'"
for file in $FILES
do
    if [ -f $file ]; then
	cp $file $UPLOADDIR/
	echo "Copied file '$file' to '$UPLOADDIR'"
    fi
done
tar zcvf $UPLOADDIR.tgz $UPLOADDIR
echo "Created file '$UPLOADDIR.tgz' for uploading"

echo "md5sum of $UPLOADDIR.tgz is as below: check on browser after successful upload"
md5sum "$UPLOADDIR.tgz"
