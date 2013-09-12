#!/bin/bash

# This script takes as argument a 'csv' file which contains
# comma-separated-value (csv) information about research papers
# accepted for publication in a particular conference.  The csv file
# has the following format: first field is paper-id, second-field is
# author name, third field is author affiliation.  A particular paper
# has a unique paper-id, and can have authors from multiple
# institutions involved.  The file has one line per author, in the
# case of multi-author papers.

# Example csv files are given in papers-example1.csv,
# papers-example2.csv, papers-example3.csv

# The script should count the number of papers with at least one
# author with affiliation "IITB".  The affiliation field check must be
# case-insensitive, but you can assume that the string "IITB" does not
# appear as part of any authors' name.

# Marks: 5
# HP: 3
# Marking scheme: 1 mark if it works for the case where there are only
# single paper authors, 1 mark if it works for case-insensitive
# checking of affiliation, 1 additional mark if it works for cases of
# multi-author papers too.  2 additional marks, and 3HP, if your code
# is just a single line bash command (with appropriate pipes).  (The
# script need not check if it indeed has one command-line argument).

# Hint: check the command-line options of the sort command

########## START ########## DO NOT EDIT THIS LINE OR LINES ABOVE THIS

grep -i 'iitb' $1 | cut -d',' -f1 | sort -nu | wc -l

########## END ########## DO NOT EDIT THIS LINE OR LINES BELOW THIS
