#!/bin/bash

function make_btree () {
    LC_ALL=C sort -u $1              |
        ../src/break.pl     |
        db_load -c duplicates=1 -T -t btree $2
}

function make_hash () {
    LC_ALL=C sort -u $1              |
        ../src/break.pl     |
        db_load -T -t hash $2
}

rm *.idx -f

make_btree terms.txt te.idx &
make_hash recs.txt re.idx &
make_btree emails.txt em.idx &
make_btree dates.txt da.idx &

wait
