#!/bin/bash

function make_btree () {
    sort -u $1              |
        ../src/break.pl     |
        db_load -c duplicates=1 -T -t btree $2
}

function make_hash () {
    sort -u $1              |
        ../src/break.pl     |
        db_load -T -t hash $2
}

rm *.idx -f

make_btree terms.txt te.idx &
pids[0]=$!

make_hash recs.txt re.idx &
pids[1]=$!

make_btree emails.txt em.idx &
pids[2]=$!

make_btree dates.txt da.idx &
pids[3]=$!

for pid in ${pids[*]}; do
    wait $pid
done
