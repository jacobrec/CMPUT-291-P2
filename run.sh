#!/bin/bash

cd src/c
make
cd ../ # in src

cd ../testing

echo "Testing Phase 1"
function comp () {
    diff $2-$1.txt $1.txt
}
function testPhase1 () {
    cat $1.xml | racket ../src/phase1.rkt
    comp terms $1
    comp emails $1
    comp dates $1
    comp recs $1
}
# testPhase1 10
testPhase1 1k
echo "Phase 1 complete"
