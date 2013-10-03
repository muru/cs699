#!/bin/bash

echo -n "Are you sure you are ready? (e.g. have you committed your working directory svn-work-quiz1 to the repository svn-repo-quiz1)? (y/n) "
read check
if test $check = "y"
then
	echo "Ok good, this script will prepare the tgz to be uploaded"
else
	echo "Ok, later..."
	exit 0
fi

UPLOAD_FILE=forupload.tgz

if [ -f "$UPLOAD_FILE" ]; then
    echo "File '$UPLOAD_FILE' already exists, remove/rename it and rerun this script"
    exit 2
fi

tar zcvf $UPLOAD_FILE svn-repo-quiz1 svn-work-quiz1
echo "Created file '$UPLOAD_FILE' for uploading"

