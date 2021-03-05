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
        echo "⚡️ cleaning up test for $FILETOTEST"
        eval "sh $DIRNAME/test/cleanup_test_$FILENAME $DIRNAME" && echo "✅ cleaned up test for $FILETOTEST" || echo "❌ cleanup failed"
        exit 0
    else 
        echo "❌ no cleanup file found, place it as '$DIRNAME/test/cleanup_test_$FILENAME'"
        exit 1
    fi
fi

FILETOTEST="$1"

if [ ! -e $FILETOTEST ]
then
    echo "'$FILETOTEST' does not exist"
    exit 1
fi

if [ ! -f $FILETOTEST ]
then
    echo "❌ '$FILETOTEST' is not a file"
    exit 1
fi

FILENAME="$(echo $FILETOTEST | rev | cut -f 1 -d '/' | rev)"
DIRNAME="$(echo $(dirname ${FILETOTEST}))"

FILETOTEST="$1"

FILENAME="$(echo $FILETOTEST | rev | cut -f 1 -d '/' | rev)"
DIRNAME="$(echo $(dirname ${FILETOTEST}))"

echo "⚡️ executing setup_test for $FILENAME in $DIRNAME"

if [ -e "$DIRNAME/test/setup_test_$FILENAME" ]
then
    eval "sh $DIRNAME/test/setup_test_$FILENAME $DIRNAME"
else 
    echo "⚠️  no setup test file found, if one is needed, place it as '$DIRNAME/test/setup_test_$FILENAME'"
fi

shift

cd $DIRNAME
echo "\nTEST-OUTPUT:"
eval "sh $FILENAME $@" && echo "TEST-END\n\n✅ succeeded" || echo "TEST-END\n\n❌ failed"
