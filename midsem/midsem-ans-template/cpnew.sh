#!/bin/bash

# This script must take two arguments, both assumed to be
# subdirectories of the current directory.  The first directory is
# assumed to be the 'source' and the second directory, the
# 'destination' of the copy.

# 1 mark: If it does not get exactly two arguments, it must exit with
# an exit value of 1

# 1 mark: If either argument is not a valid sub-directory of the
# current directory, it must exit with an exit value of 2

# 2 marks: If both are valid directories, it must copy all NEWER files
# (and only NEWER files) from the source directory to the destination
# directory.  You can further assume that the source directory does
# not have any further subdirectories, i.e. it has only plain files.

# Definition of NEWER: a file present only in the source directory, or
# one which has a more recent timestamp than the file with the same
# name in the destination directory

# Marks: 4
# Marks break-up: 1+1+2
# HP: 1
# 1 HP for handling spaces in file names
# Hint: learn about the bash command for file comparison

########## START ########## DO NOT EDIT THIS LINE OR LINES ABOVE THIS
[[ $# -ne 2 ]] && exit 1

[[ -d $PWD/$1 && -d  $PWD/$2 ]] || exit 2

cp --update --verbose "$1"/* "$2"/

########## END ########## DO NOT EDIT THIS LINE OR LINES BELOW THIS
