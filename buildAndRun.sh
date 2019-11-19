#!/bin/bash
cd src/c
make > /dev/null
cd ../ # in src

cd ../testing
cat $1.xml | racket ../src/phase1.rkt # Does phase 1
../src/phase2.sh # Does phase 2
cd ../src
racket ../src/main.rkt # Does phase 3
cd ../testing
