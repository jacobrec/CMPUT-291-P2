#!/bin/bash
cd src/c
make clean
make test
mv test ../
cd ../
gdb test
rm test
