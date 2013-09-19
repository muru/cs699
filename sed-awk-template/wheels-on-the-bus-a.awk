#! /usr/bin/awk -f

BEGIN {FS=":"; OLDORS=ORS; GO_VERB="goes"}
{
	ORS=""
	if ($1 ~ /[a-ZA-Z]*s/) {
		GO_VERB="go " 
	} else {
		GO_VERB="goes "
	}
	print "The " $1 " on the bus " GO_VERB
	for (i=1; i<=2; i++) {
		for (j=1; j <= 3; j++) {
			print $2 ", "
		}
		print "\n"
	}
	ORS=OLDORS
	print $2 ", " $2 ", "  $2 "."
	print "The " $1 " on the bus " GO_VERB $2 ", " $2 ", "  $2 ", "
	print "all through the town.\n"
}
