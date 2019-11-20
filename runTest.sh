#!/bin/bash


function cleanAll () {
    rm src/jbdblib.so -f
    rm output/ -rf
    cd src/c
    make clean > /dev/null
    cd ../../
}

function comp () {
    diff $2-$1.txt ../output/$1.txt
}

function testProject () {
    ./doProject.sh testing/$1.xml
    cd testing

    # Test Phase 1
    if comp terms $1 && comp emails $1 && comp dates $1 && comp recs $1; then
        echo "Indices are correct"
    else
        echo "Indices do not match"
    fi
    cd ../
}

cleanAll
# testProject 10
testProject 1k
cleanAll

