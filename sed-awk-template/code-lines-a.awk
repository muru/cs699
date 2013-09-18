#! /usr/bin/awk -f 
BEGIN {line_count=0;}
{
	/^([ \t]*)(#.*)?([ \t]*)$/ || line_count++;
}
END {print line_count}
