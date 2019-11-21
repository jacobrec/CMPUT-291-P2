#!/bin/bash

### This assumes it is being run from the project root directory

function makeCLib() {
    cd src/c
    make > /dev/null
    cd ../../
    cp src/jbdblib.so output/jbdblib.so
}

FILE=$1
function run() {
    if [ -z "$FILE" ]; then
        echo "Please provide a filepath to the xml"
    else
        date
        mkdir -p output
        # Build jbdblib.so
        echo ""
        echo ""
        echo "===== Make ====="
        time makeCLib

        echo ""
        echo ""
        echo "===== Phase 1 ====="
        time cat $FILE | racket src/phase1.rkt # Does phase 1
        cd output
        echo ""
        echo ""
        echo "===== Phase 2 ====="
        time ../src/phase2.sh # Does phase 2
    fi

    echo ""
    echo ""
    echo "===== Total ====="
}

time run
