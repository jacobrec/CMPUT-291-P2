#!/bin/bash

cd src/c
make
cd ../ # in src

cd ../testing

function comp () {
    diff $2-$1.txt $1.txt
}
function testProject () {
    # Phase 1
    cat $1.xml | racket ../src/phase1.rkt
    comp terms $1
    comp emails $1
    comp dates $1
    comp recs $1

    # Phase 2
    ../src/phase2.sh
}
testProject 10
# testProject 1k
echo "Testing Complete"

