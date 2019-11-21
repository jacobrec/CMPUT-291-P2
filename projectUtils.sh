function doPhase1() {
    cd output
    ln -s "../$1" file.xml
    cat file.xml | ./phase1 # Does phase 1
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
    make > /dev/null
    cd ../../
    cp src/phase1-cpp/xmlparserfast output/phase1
}
