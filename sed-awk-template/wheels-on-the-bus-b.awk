#! /usr/bin/awk -f

BEGIN	{FS=":";}
		{print "The " $1 " on the bus go " $2 ", " $2 ", "  $2 ", "}
		{print $2 ", " $2 ", "  $2 ","}
		{print $2 ", " $2 ", "  $2 "."}
		{print "The " $1 " on the bus go " $2 ", " $2 ", "  $2 ", "}
		{print "all through the town.\n"}
END		{print "\t --Anonymous"}
