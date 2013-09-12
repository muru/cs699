#!/bin/bash

# Timed cd

# A student has 3 lab courses: CS599, CS699, CS799 with the following
# schedule:
# CS599: Mon 2-5pm
# CS699: Wed 2-5pm
# CS799: Fri 2-5pm
# The student has 3 sub-directories CS599, CS699, CS799 in the current
# directory

# Write a script to take two arguments:
# Argument-1: day of the week in 3-letter capitalized format
# (e.g. Wed, Fri, Sat, etc.)
# Argument-2: hour of the day in 24-hour format (0-23)
# The script should change directory to the correct directory
# depending on the time
# The script should print an appropriate error message if it does not
# get exactly two arguments
# If the time window does not match any of the lab timings, then the
# script should not change directory

usage="Usage: $0 DDD HH"

die ()
{
	echo "$@" 1>&2;
	exit -1;
}

[[ $# -ne 2 ]] && die $usage

declare -A DIRS
DIRS["MON"]="CS599"
DIRS["WED"]="CS699"
DIRS["FRI"]="CS799"

case "$1" in 
	MON|WED|FRI) TARGETDIR=${DIRS["$1"]}
		;;
	*) die "Not changing directory."
		;;
esac

if [[ ($2 -ge 14) && ($2 -le 17) ]] ; then
	cd $TARGETDIR
	pwd
fi
