#!/bin/bash

. projectUtils.sh

### This assumes it is being run from the project root directory

FILE=$1
if [ -z "$FILE" ]; then
    echo "Please provide a filepath to the xml"
elif [ -f "$FILE" ]; then
    rm -rf output
    mkdir -p output
    makeCLibs
    echo "Starting Phase 1"
    doPhase1 $FILE
    echo "Starting Phase 2"
    doPhase2
    echo "Starting Phase 3"
    doPhase3
else
    echo "Please provide the absolute filepath to file"
fi
