#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "set.h"

Set* set_new_size(int bl) {
    Set* s = malloc(sizeof(Set));

    s->bucketLength = bl;
    s->buckets = malloc(sizeof(int) * s->bucketLength);
    s->size = 0;
}
void rebuild(Set* set) {
    Set* ns = set_new_size(set->bucketLength * 3.14);

    for (int i = 0; i < set->bucketLength; i++) {
        if (set->buckets[i] > 0) {
            set_add(ns, set->buckets[i]);
        }
    }


    free(set->buckets);
    set->size = ns->size;
    set->buckets = ns->buckets;
    set->bucketLength = ns->bucketLength;
    free(ns);
}

bool find(Set* set, int item, int** ptrptr) {
    int key = item % set->bucketLength;
    int* ptr = *ptrptr;
    ptr = &set->buckets[key];

    while (*ptr != 0) {
        if (*ptr == item) {
            *ptrptr = ptr;
            return true;
        }
        ptr += sizeof(int);
    }

    *ptrptr = ptr;
    return false;
}

Set* set_new() {
    return set_new_size(100);
}

void set_add(Set* set, int item) {
    int* ptr;
    if (!find(set, item, &ptr)) {
        *ptr = item;
        set->size++;
    }

    if (set->size * 2 > set->bucketLength) {
        rebuild(set);
    }
}

void set_remove(Set* set, int item) {
    int* ptr;
    if (find(set, item, &ptr)) {
        *ptr = -1;
    }
}

bool set_has(Set* set, int item) {
    int* ptr;
    return find(set, item, &ptr);
}

void set_clear(Set* s) {
    s->bucketLength = 100;
    s->size = 0;
    s->buckets = realloc(s->buckets, sizeof(int) * s->bucketLength);
}

void set_intersect(Set* set, Set* other) {

}

void set_delete(Set* set) {
    free(set->buckets);
    free(set);
}



