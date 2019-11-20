#!/bin/bash

### This assumes it is being run from the project root directory

function makeCLib() {
    cd src/c
    make > /dev/null
    cd ../../
    cp src/jbdblib.so output/jbdblib.so
}

FILE=$1
if [ -z "$FILE" ]; then
    echo "Please provide a filepath to the xml"
else
    mkdir -p output
    # Build jbdblib.so
    makeCLib

    # Do project
    cat $FILE | racket src/phase1.rkt # Does phase 1
    cd output
    ../src/phase2.sh # Does phase 2
    racket ../src/main.rkt # Does phase 3
fi
