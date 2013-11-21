#! /bin/bash

for FILE_LINE in `grep -nf files.txt src/* | grep -Ev '(\/\/)' | cut -d: -f1,2`
do
	FILE=`echo $FILE_LINE | sed 's/:.*//'`
	LINE=`echo $FILE_LINE | sed 's/.*://'`
	echo $FILE +$LINE
	read BLAH
	gvim --remote +:$LINE $FILE 
done
