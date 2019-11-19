#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>

#define PATH "../testing/"

struct database {
    DB* terms;
};
typedef struct database JDB;

// For debugging only
void ls() {
    char *args[2];
    args[0] = "/bin/ls";
    args[1] = NULL;
    if (fork() == 0)
        execv(args[0], args);
}

void errorif(int x, const char* msg) {
    if (x)
        printf("%s: %s\n", msg, db_strerror(x));
}

void setupDB(DB** db, const char* loc) {
    int x = db_create(db, NULL, 0);
    errorif(x, "db_create");

    (*db)->set_flags(*db, DB_DUP);
    x = (*db)->open(*db, NULL, loc, NULL, DB_BTREE, DB_RDONLY, 0664);

    errorif(x, "db->open");
}

void setup(JDB* jdb) {
    setupDB(&jdb->terms, PATH"te.idx");
}


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
void getTerms(DB* db, char* ks, int kl, bool isWild) {
    int ret;
    DBC* dbcp;
    ret = db->cursor(db, NULL, &dbcp, 0);
    errorif(ret, "cursor");
    DBT key, data;
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    key.data = ks;
    key.size = kl-1;
    ret = dbcp->c_get(dbcp, &key, &data, DB_SET_RANGE);
    do {
        errorif(ret, "dbcp->c_get");
        if (data.data != NULL && (strncmp(ks, key.data, kl-1) == 0) &&
                (isWild || strlen(ks) == key.size)) {
            printf("key: %.*s | data: %d\n",
                     key.size, (char*)key.data,
                    dataToNumber(data.data, data.size));
        } else {
            break;
        }
        ret = dbcp->c_get(dbcp, &key, &data, DB_NEXT);
    } while (ret != DB_NOTFOUND);
}

#define SEARCH "b-out"
int query() {
    JDB* jdb = malloc(sizeof(JDB));
    setup(jdb);


    printf("Hello from C!\nTime for DB!\n");
    return 3;
}

int main() {
    query();
}
