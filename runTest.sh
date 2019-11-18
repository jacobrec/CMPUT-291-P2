#!/bin/bash

cd src/c
make
cd ../ # in src

cd ../testing

function comp () {
    diff $2-$1.txt $1.txt
}
function testProject () {
    cd ../
    ./buildAndRun.sh $1
    cd testing

    # Test Phase 1
    comp terms $1
    comp emails $1
    comp dates $1
    comp recs $1

}
testProject 10
# testProject 1k
echo "Testing Complete"

