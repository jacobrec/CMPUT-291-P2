#!/bin/bash

. projectUtils.sh

### This assumes it is being run from the project root directory

function header () {
        echo ""
        echo ""
        echo "===== $1 ====="
}

FILE=$1
function run() {
    if [ -z "$FILE" ]; then
        echo "Please provide a filepath to the xml"
    else
        date
        rm -rf output
        mkdir -p output

        header "Make"
        time makeCLibs

        header "Phase 1"
        time doPhase1 $FILE

        header "Phase 2"
        time doPhase2
    fi

    header "Total"
}

time run
