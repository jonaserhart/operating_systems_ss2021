#!/bin/zsh

if [ ! "$#" -eq 1 ]
then
    exit 1
fi

DIRNAME="$1"
mkdir "$DIRNAME/test/testenvir/backup"

echo "Some text in file with extension" > "$DIRNAME/test/testenvir/file.withextension"
echo "Some text in file without extension" > "$DIRNAME/test/testenvir/filewithoutextension"
