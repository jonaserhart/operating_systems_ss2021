#!/bin/zsh

if [ "$#" -lt 1 ]
then
    echo "provide at least one argument"
    exit 1
fi

if [ $1 = "clean" ]
then
    FILETOTEST="$2"
    FILENAME="$(echo $FILETOTEST | rev | cut -f 1 -d '/' | rev)"
    DIRNAME="$(echo $(dirname ${FILETOTEST}))"
    if [ -e "$DIRNAME/test/cleanup_test_$FILENAME" ]
    then
        eval "sh $DIRNAME/test/cleanup_test_$FILENAME $DIRNAME"
        exit 0
    fi
fi

FILETOTEST="$1"

if [ ! -e $FILETOTEST ]
then
    echo "'$FILETOTEST' does not exist"
    exit 1
fi

FILENAME="$(echo $FILETOTEST | rev | cut -f 1 -d '/' | rev)"
DIRNAME="$(echo $(dirname ${FILETOTEST}))"

FILETOTEST="$1"

FILENAME="$(echo $FILETOTEST | rev | cut -f 1 -d '/' | rev)"
echo $FILENAME
DIRNAME="$(echo $(dirname ${FILETOTEST}))"
echo $DIRNAME

if [ -e "$DIRNAME/test/setup_test_$FILENAME" ]
then
    eval "sh $DIRNAME/test/setup_test_$FILENAME $DIRNAME"
fi

shift

cd $DIRNAME

eval "sh $FILENAME $@"
