#!/bin/bash

# This script dynamically generates a gnuplot script and runs gnuplot
# on it.  It CANNOT depend on an externally present gnuplot file.

# The gnuplot script generated should plot the data given in the file
# thrpt-data.txt in the current directory.  The data has two columns:
# time (in seconds) and network throughput (in Kbps).

# 2 marks: The script should take one argument: the title of plot.
# And it should generate the gnuplot script to output thrpt-plot.eps
# which plots the throughput versus time.  The specified title should
# show on the final plot output.

# 2 marks: The script should check for an environment variable called
# X_LABEL.  If this variable is present, then the script use its value
# as the x-axis label within the gnuplot script, else it must use the
# string "Time" for the x-axis label.

# 2 marks: The script should check for environment variable
# TIME_UNIT.  If this variable is present and is set to 'ms', then the
# x-axis should be in milli-seconds, even though the data is given in
# seconds (i.e. each value in the data file must be appropriately
# converted before plotting).  You CANNOT change the data file itself,
# and you CANNOT dynamically generate a new data file.  Hint: there is
# a way to do simple arithmetic within gnuplot.

# Marks: 6
# Marks break up: 2+2+2
# HP: 1
# 1 HP for getting the last part right

########## START ########## DO NOT EDIT THIS LINE OR LINES ABOVE THIS

get_gnu_script () {
	TITLE="$@"
	XLABEL="${X_LABEL=Time}"
	[[ $TIME_UNIT == 'ms' ]] && MULT=1000 || MULT=1
	cat <<EOF
	set term eps
	set output "thrpt-plot.eps"
	set xlabel "$XLABEL"
	plot 'thrpt-data.txt' using (\$1*$MULT):2 title "$TITLE"
EOF
# The EOF above is the terminator for a HEREDOC and has to be on a
# separate line by itself, and entirely by itself. Not even tabs or spaces 
# are allowed. Hence it isn't indented.
}

########## END ########## DO NOT EDIT THIS LINE OR LINES BELOW THIS

# You are supposed to provide the definition of a bash function called
# get_gnu_script between the START and END lines above.
get_gnu_script "$1" | gnuplot
