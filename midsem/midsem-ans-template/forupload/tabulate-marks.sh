#!/bin/bash

# This script must use a single line grep command (using pipes with
# other commands as necessary) to tabulate the marks of each relevant
# script in this midsem directory.  Note that each script template has
# a line specifying the marks for that script.  Note that the test
# script files may also have such lines, which you are NOT supposed to
# change or edit.

# Marks: 2
# Marks break-up: 1 mark for correctness, 1 mark for the command
# without actually listing the list of relevant files

########## START ########## DO NOT EDIT THIS LINE OR LINES ABOVE THIS

grep -E '# Marks: [0-9]+' *.sh | grep -Ev '^test.*.sh'

########## END ########## DO NOT EDIT THIS LINE OR LINES BELOW THIS
