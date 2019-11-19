#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <db.h>
#include "set.h"

#define PATH "../testing/"

struct database {
    DB* terms;
    DB* recs;
};
typedef struct database JDB;

// ls: For debugging only
void ls() {
    char *args[2];
    args[0] = "/bin/ls";
    args[1] = NULL;
    if (fork() == 0)
        execv(args[0], args);
}

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
    setupDB(&jdb->terms, PATH"te.idx", DB_BTREE);
    setupDB(&jdb->recs, PATH"re.idx", DB_HASH);
}

// clean up the struct containing all the databases
void cleanup_databases(JDB* jdb) {
    cleanup(&jdb->terms);
    cleanup(&jdb->recs);
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
    key.size = kl-1;
    ret = dbcp->c_get(dbcp, &key, &data, DB_SET_RANGE);
    do {
        errorif(ret, "dbcp->c_get");
        if (data.data != NULL && (strncmp(ks, key.data, kl-1) == 0) &&
                (isWild || strlen(ks) == key.size)) {
            set_add(s, dataToNumber(data.data, data.size));
            /* // debug printing
            printf("key: %.*s | data: %d\n",
                     key.size, (char*)key.data,
                    dataToNumber(data.data, data.size));
            // */
        } else {
            break;
        }
        ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
    } while (ret != DB_NOTFOUND);
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

    printf("Row[%d]: %.*s\n", row, data.size, data.data);
}

// Display the whole set, if time, do some nice box drawing formatting
void display_set(JDB* jdb, Set* s, bool fullMode) {
    for (int i = 0; i < s->bucketLength; i++) {
        if (s->buckets[i] > 0) {
            display_row(jdb, s->buckets[i], fullMode);
        }
    }
}

Set* queryTerm(JDB* jdb, Set* set,
        char* search, int searchLen, bool isWild) {
    getTerms(jdb->terms, set, search, searchLen, isWild);
    return set;
}

Set* query(Set* set, JDB* jdb) {

    Set* s = set_new();
    set_add(s, 5);
    set_add(s, 11);
    set_add(s, 12);
    set_add(s, 13);
    //display_set(jdb, s, false);

    return s;
}

Set* emptyset() {
    return set_new();
}

JDB* setup_databases() {
    JDB* jdb = malloc(sizeof(JDB));
    setup(jdb);
    return jdb;
}

