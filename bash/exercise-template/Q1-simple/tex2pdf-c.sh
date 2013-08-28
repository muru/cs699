#/bin/bash

# This program should compile the latex file given as argument.  The
# tex file can be in any directory; its full path name or relative
# path name may be given as argument.  And if the latex compilation
# succeeds, it should tar-gzip the entire directory in which the latex
# file resides, from the parent directory, to a tgz file.  The name of
# the tgz file should be <dirname>.tgz where <dirname> is the name of
# the directory in which the latex file resides.  It should then move
# the tgz file to /tmp.
