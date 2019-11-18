#!/bin/bash

function make_btree () {
    sort -u $1              |
        ../src/break.pl     |
        db_load -T -t btree $2
}

make_btree terms.txt te.idx

sort -u recs.txt        |
    ../src/break.pl     |
    db_load -T -t hash re.idx

make_btree emails.txt em.idx

make_btree dates.txt da.idx

