function doPhase1() {
    cd output
    if [ -f "$1" ]; then
      cat $1 | ./phase1 # Does phase 1
    elif [ -f "../$1" ]; then
      cat "../$1" | ./phase1 # Does phase 1
    fi
    cd ../
}

function doPhase2() {
    cd output
    ../src/phase2.sh # Does phase 2
    cd ../
}

function doPhase3() {
    cd output
    racket ../src/main.rkt # Does phase 3
    cd ../
}

function makeCLibs() {
    cd src/c
    make > /dev/null
    cd ../../
    cp src/jbdblib.so output/jbdblib.so

    cd src/phase1-cpp
    make > /dev/null 2>&1
    cd ../../
    cp src/phase1-cpp/xmlparserfast output/phase1
}
