#!/bin/bash

# This script must implement a flexible file open.  It will take one
# argument, which is the file to be opened.  And it will read a
# configuration file whose format is as given in file myopen.cfg.
# Each line of myopen.cfg corresponds to the specification of which
# program to use to open a file of a particular extension.

# Running the script with argument somefile.someext should search
# through the configuration file, find the program to use for opening
# the given file (by matching the extension), and then use the program
# to open the given file.

# Your script should exit with exit value of 1 if no argument is
# given.  It should exit with exit value of 2 if the configuration
# file is not found.  It should exit with exit value of 3 if no
# program can be found corresponding to the given extension.  It
# should exit with exit value 4 if the program corresponding to given
# extension does not exist.

# The configuration file myopen.cfg can be assumed to be in the HOME
# directory (not the current working directory).
