#!/bin/bash

. projectUtils.sh

### This assumes it is being run from the project root directory

FILE=$1
if [ -z "$FILE" ]; then
    echo "Please provide a filepath to the xml"
else
    rm -rf output
    mkdir -p output
    makeCLibs
    doPhase1 $FILE
    doPhase2
    doPhase3
fi
