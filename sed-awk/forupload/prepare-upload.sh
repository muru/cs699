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
code-lines-a.awk
code-lines-a.sed
code-lines.awk
code-lines.sed
count-lines.awk
count-words-html-a.sh
count-words-html.sh
html-body.sed
prepare-upload.sh
wheels-on-the-bus-a.awk
wheels-on-the-bus.awk
wheels-on-the-bus-b.awk
"

for file in $FILES
do
    if [ ! -f $file ]; then
	echo "Error: cannot find file '$file'"
	echo "Even if you are not attempting the question, you need to have the original template sh file in the submission"
	exit 3
    fi
done

mkdir $UPLOADDIR

for file in $FILES
do
    cp -a $file $UPLOADDIR/$file
    echo "Copied file '$file' to '$UPLOADDIR'"
done

tar zcvf $UPLOADDIR.tgz $UPLOADDIR
echo "Created file '$UPLOADDIR.tgz' for uploading"

echo "md5sum of $UPLOADDIR.tgz is as below: check on browser after successful upload"
md5sum "$UPLOADDIR.tgz"

