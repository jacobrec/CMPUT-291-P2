#!/bin/bash

rm src/jbdblib.so
cd src/c
make runtest > /dev/null
make > /dev/null
cd ../ # in src

cd ../testing
rm *.idx -f

function comp () {
    diff $2-$1.txt ../output/$1.txt
}
function testProject () {
    cd ../
    ./doProject.sh testing/$1.xml
    cd testing

    # Test Phase 1
    comp terms $1
    comp emails $1
    comp dates $1
    comp recs $1

}
# testProject 10
testProject 1k
echo "Testing Complete"

