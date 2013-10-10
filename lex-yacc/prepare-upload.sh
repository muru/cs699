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

FILES="\
 Makefile\
 calc.l\
 calc.y\
 count-lines.l\
 count-lwc.l\
 count-num.l\
 ex1.l\
 ex1.y\
 ex2.l\
 ex2.y\
 ex3.l\
 ex3.y\
 ex4.l\
 ex4.y\
 lex-yacc.html\
 prepare-upload.sh\
 text-cleanup.l\
"

for file in $FILES
do
    if [ ! -f $file ]; then
        echo "Error: cannot find file '$file'"
        echo "Even if you are not attempting the question, you need to have the original template file in the submission"
        exit 3
    fi
done

mkdir $UPLOADDIR

for file in $FILES
do
    cp $file $UPLOADDIR/
    echo "Copied file '$file' to '$UPLOADDIR'"
done
tar zcvf $UPLOADDIR.tgz $UPLOADDIR
echo "Created file '$UPLOADDIR.tgz' for uploading"

echo "md5sum of $UPLOADDIR.tgz is as below: check on browser after successful upload"
md5sum "$UPLOADDIR.tgz"
