#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <db.h>
#include "set.h"
#include "parser_api.h"


struct database {
    DB* terms;
    DB* recs;
    DB* emails;
    DB* dates;
};
typedef struct database JDB;

// Print database error message only if there is one
// Used on the result of any db-> call
void errorif(int x, const char* msg) {
    if (x)
        printf("%s: %s\n", msg, db_strerror(x));
}

// Initialize a database from a file
void setupDB(DB** db, const char* loc, int type) {
    int x = db_create(db, NULL, 0);
    errorif(x, "db_create");

    if (type == DB_BTREE) {
        (*db)->set_flags(*db, DB_DUP);
    }
    x = (*db)->open(*db, NULL, loc, NULL, type, DB_RDONLY, 0664);

    errorif(x, "db->open");
}

// Close a database connection
void cleanup(DB** db) {
    (*db)->close(*db, 0);
}

// setup the struct containing all databases
void setup(JDB* jdb) {
    setupDB(&jdb->terms, "te.idx", DB_BTREE);
    setupDB(&jdb->emails, "em.idx", DB_BTREE);
    setupDB(&jdb->dates, "da.idx", DB_BTREE);
    setupDB(&jdb->recs, "re.idx", DB_HASH);
}

// clean up the struct containing all the databases
void cleanup_databases(JDB* jdb) {
    cleanup(&jdb->terms);
    cleanup(&jdb->recs);
    cleanup(&jdb->dates);
    cleanup(&jdb->emails);
    free(jdb);
}


// Helper that converts a numeric data value to an actual number
int dataToNumber(char* data, int len) {
    char buf[len+1];
    strncpy(buf, data, len);
    buf[len] = 0;
    return atoi(buf);
}

/**
 * This function will get all items in the database whose keys are between two
 * search paramaters. The ranges are [start, end)
 */
void getItemsInRange(DB* db, Set* join,
        char* start, int lstart, char* end, int lend) {
    int ret;
    DBC* dbcp;
    ret = db->cursor(db, NULL, &dbcp, 0);
    errorif(ret, "cursor");
    DBT key, data;
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));
    bool isNewSet = !join->isUsed;
    Set* s;
    if (isNewSet) {
        s = join;
    } else {
        s = set_new();
    }

    key.data = start;
    key.size = lstart;
    ret = dbcp->c_get(dbcp, &key, &data, DB_SET_RANGE);
    // printf("search key: %s-%s\n", start, end);
    do {
        errorif(ret, "dbcp->c_get");
        if (data.data != NULL &&
                strncmp(end, key.data, lend) > 0) {
            set_add(s, dataToNumber(data.data, data.size));
        } else {
            break;
        }
        ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
    } while (ret != DB_NOTFOUND);
    if (!isNewSet) {
        set_intersect(join, s);
    }
}


/**
 * This function will get iterate over all the items whose term is equal to the
 * key. If isWild is set, it will also iterate over items whose key begins with
 * the key. This will be used for the term queries.
 */
void getTerms(DB* db, Set* join, char* ks, int kl, bool isWild) {
    int ret;
    DBC* dbcp;
    ret = db->cursor(db, NULL, &dbcp, 0);
    errorif(ret, "cursor");
    DBT key, data;
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));
    bool isNewSet = !join->isUsed;
    Set* s;
    if (isNewSet) {
        s = join;
    } else {
        s = set_new();
    }

    key.data = ks;
    key.size = kl;
    ret = dbcp->c_get(dbcp, &key, &data, DB_SET_RANGE);
    do {
        errorif(ret, "dbcp->c_get");
        //printf("search key: %s[%d]\n", ks, kl);
        if (data.data != NULL && (strncmp(ks, key.data, kl) == 0) &&
                (isWild || strlen(ks) == key.size)) {
            set_add(s, dataToNumber(data.data, data.size));
            /* // debug printing
            printf("key: %.*s | data: %d | match with(%s): %d\n",
                     key.size, (char*)key.data,
                    dataToNumber(data.data, data.size),
                    ks, strncmp(ks, key.data, kl-1));
            // */
        } else {
            break;
        }
        ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
    } while (ret != DB_NOTFOUND);
    // printf("new? %d. Set size: %d\n", isNewSet, join->size);
    if (!isNewSet) {
        set_intersect(join, s);
    }
}


// Gets the xml from the database and prints it for a single row
void display_row(JDB* jdb, int row, bool fullMode) {
    DBC* dbcp;
    DB* db = jdb->recs;
    int ret;

    ret = db->cursor(db, NULL, &dbcp, 0);
    errorif(ret, "cursor");

    DBT key, data;
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));
    key.size = 1 + log10(row);
    char buf[key.size+1];
    sprintf(buf, "%d", row);
    key.data = &buf[0];

    // Debug printing
    // printf("Key: data[%.*s] len[%d]\n", key.size, (char*)key.data, key.size);
    // printf("Buf: %s\n", buf);

    ret = dbcp->c_get(dbcp, &key, &data, DB_SET);
    errorif(ret, "cursor");

    //parser->parseRow((char*)data.data, data.size);

    if (fullMode) {
        //No pretty printing...
        printf("Row[%d]: %.*s\n", row, data.size, (char*)data.data);
    } else {
        //Hazardous if the xml is malformed
        char* openP = strstr((char*) data.data, "<subj>")+6;
        char* closeP = strstr((char*) data.data, "</subj>");
        printf("Row[%d]: %.*s\n", row, closeP-openP, openP);
    }
}

// Display the whole set, if time, do some nice box drawing formatting
void display_set(JDB* jdb, Set* s, bool fullMode) {
    //struct RSaxParser* parser = makeParser(fullMode);
    for (int i = 0; i < s->bucketLength; i++) {
        if (s->buckets[i] > 0) {
            display_row(jdb, s->buckets[i], fullMode);
        }
    }
    //cleanParser(parser);
}

// Queries by term, expects search to be in the form s-term
// or b-term
Set* queryTerm(JDB* jdb, Set* set,
        char* search, int searchLen, bool isWild) {
    getTerms(jdb->terms, set, search, searchLen, isWild);
    return set;
}

// Queries by term taking the union of the results, expects
// search to be in the form s-term or b-term. search should
// probably be s-term, and search2 should be b-term, or other
// way around
Set* queryTerm2(JDB* jdb, Set* set,
        char* search, char* search2, int searchLen, bool isWild) {
    Set* other = set_copy(set);
    getTerms(jdb->terms, set, search, searchLen, isWild);
    getTerms(jdb->terms, other, search2, searchLen, isWild);
    set_union(set, other);
    return set;
}

// Queries by email, expects search to be in the form s-term
// or b-term
Set* queryEmail(JDB* jdb, Set* set, char* search, int searchLen) {
    getTerms(jdb->emails, set, search, searchLen, false);
    return set;
}

// Queries by email, expects search to be in the form s-term
// or b-term
Set* queryDate(JDB* jdb, Set* set, char* datestart, char* dateend) {
    getItemsInRange(jdb->dates, set, datestart, 10, dateend, 10);
    return set;
}

Set* emptyset() {
    return set_new();
}

bool setsize(Set* set) {
    return set->size;
}

JDB* setup_databases() {
    JDB* jdb = malloc(sizeof(JDB));
    setup(jdb);
    return jdb;
}

