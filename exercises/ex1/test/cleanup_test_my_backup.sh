#!/bin/zsh

if [ ! "$#" -eq 1 ]
then
    exit 1
fi

DIRNAME="$1"

rm -rf "$DIRNAME/test/testenvir"
rm -rf "$DIRNAME/test/testenvir/backup"
