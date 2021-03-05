#!/bin/bash
if [ ! "$#" -eq 2 ]
then
	echo "please provide exactly two arguments"
	exit 1
fi

INFILE="$1"
OUTFILE="$2"
# !: not
# w: write permission are granted
if [ ! -w "$OUTFILE" ]
then
	echo "$OUTFILE can not be written: permission denied" 
	exit 1
fi

# if not exists -> log error
if [ ! -e "$INFILE" ]
then
	echo "$INFILE does not exist"
	exit 1
fi

if [ ! -r "$INFILE" ]
then
	echo "$INFILE can not be read: permission denied"
	exit 1
fi
# execute if passed so far
cat "$INFILE" >> "$OUTFILE"