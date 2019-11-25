#!/bin/bash

cd testing
cat 100k.xml | head -2 > 1m.xml

for i in {1..10}; do
    cat 100k.xml | tail -n +3 | head -n -1 | grep -Po "(?<=</row>).*" | awk -v i="$i" '{printf "<mail><row>%d</row>%s\n", NR+((i-1)*100000), $0}' >> 1m.xml
  echo "${i}0%"
done

cat 100k.xml | tail -1 >> 1m.xml

