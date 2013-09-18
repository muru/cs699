#! /usr/bin/awk -f

BEGIN	{FS=":";}
	{
		ORS=""
		print "The " $1 " on the bus go "
		for (i=1; i<=2; i++) {
			for (j=1; j <= 3; j++) {
				print $2 ", "
			}
			print "\n"
		}
	}
	{
		ORS="\r\n"
		print $2 ", " $2 ", "  $2 "."
		print "The " $1 " on the bus go " $2 ", " $2 ", "  $2 ", "
		print "all through the town.\n"
	}
