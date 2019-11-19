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

void getItem(DB* db) {
    int ret;
    DBC* dbcp;
    ret = db->cursor(db, NULL, &dbcp, 0);
    errorif(ret, "cursor");
    DBT key, data;
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    char* ks = "s-vision";
    key.data = ks;
    key.size = sizeof(ks);
    ret = dbcp->c_get(dbcp, &key, &data, DB_SET_RANGE);
    errorif(ret, "db->get");
    if (data.data != NULL) {
        printf("data: %s\n", (char*)data.data);
    } else {
        printf("no data\n");
    }
}

int query() {
    JDB* jdb = malloc(sizeof(JDB));
    setup(jdb);

    getItem(jdb->terms);

    printf("Hello from C!\nTime for DB!\n");
    return 3;
}

int main() {
    query();
}
