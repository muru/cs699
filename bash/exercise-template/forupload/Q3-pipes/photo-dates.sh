#!/bin/bash

# Write a script called photo-dates.sh which takes one argument, a
# photo file, and prints the file's orignal date and time of creation.
# The format of the output can match the relevant line in the output
# of the 'identify' program.  Hint: use the verbose option for the
# identify program.

[[ $# -ne 1 ]] && exit 1

DATE="`identify -verbose $1 | grep -i create | sed 's/.*date:create: *//'`"
date --date "$DATE" +"%T %Z, %A, %B %d, %Y"
