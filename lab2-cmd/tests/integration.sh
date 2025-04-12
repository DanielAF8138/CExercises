#!/bin/bash

EXE="../shell"

function run_test {

    # parameters
    TAG=$1
    ARGS=$2

    # file paths
    OUTPUT=outputs/$TAG.txt
    DIFF=outputs/$TAG.diff
    EXPECT=expected/$TAG.txt
    VALGRND=valgrind/$TAG.txt

    # print tag format
    PTAG=$(printf '%-30s' "$TAG")

    # check for expected text that needs to be fixed
    if [ ! -z "$(egrep "<<<<.*>>>>" "$EXPECT")" ] ; then
        echo "$PTAG FAIL ($EXPECT not correct)"
        return
    fi

    # compare the file
    SHAOUT=$(shasum "$EXPECT" | awk '{print $1}')
    SHAEXP=$(cat "expected/.$TAG.sha")
    if [ "$SHAOUT" != "$SHAEXP" ] ; then
        echo "$PTAG FAIL ($EXPECT not correct)"
        return
    fi

    # run test and compare output to the expected version
    $EXE "$ARGS" 2>/dev/null >"$OUTPUT"
    diff -u "$OUTPUT" "$EXPECT" >"$DIFF"
    if [ -s "$DIFF" ]; then

        # try alternative solution (if it exists)
        EXPECT=expected/$TAG-2.txt
        if [ -e "$EXPECT" ]; then
            diff -u "$OUTPUT" "$EXPECT" >"$DIFF"
            if [ -s "$DIFF" ]; then
                echo "$PTAG FAIL (see $DIFF for details)"
            else
                echo "$PTAG pass"
            fi
        else
            echo "$PTAG FAIL (see $DIFF for details)"
        fi
    else
        echo "$PTAG pass"
    fi

    # run valgrind
    valgrind $EXE $ARGS &>$VALGRND
}

# initialize output folders
mkdir -p outputs
mkdir -p valgrind
rm -f outputs/* valgrind/*

# run individual tests
source itests.include

# check for memory leaks
LEAK=`cat valgrind/*.txt | grep 'definitely lost' | grep -v ' 0 bytes in 0 blocks'`
if [ -z "$LEAK" ]; then
    echo "No memory leak found."
else
    echo "Memory leak(s) found. See files listed below for details."
    grep 'definitely lost' valgrind/*.txt | grep -v ' 0 bytes in 0 blocks' | sed -e 's/:.*$//g' | sed -e 's/^/  - /g'
fi

