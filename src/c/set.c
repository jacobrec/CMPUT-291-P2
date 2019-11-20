#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

void set_overwrite(Set* set, Set* from) {
    free(set->buckets);
    set->buckets = from->buckets;
    set->load = from->load;
    set->size = from->size;
    set->bucketLength = from->bucketLength;
    set->isUsed = from->isUsed;
    free(from);
}


Set* set_new_size(int bl) {
    Set* s = malloc(sizeof(Set));

    s->bucketLength = bl;
    s->buckets = malloc(sizeof(int) * s->bucketLength);
    memset(s->buckets, 0, sizeof(int) * s->bucketLength);
    s->load = 0;
    s->size = 0;
    s->isUsed = true;
    return s;
}

void rebuild(Set* set) {
    Set* ns = set_new_size(set->bucketLength * 3.14);

    for (int i = 0; i < set->bucketLength; i++) {
        if (set->buckets[i] > 0) {
            set_add(ns, set->buckets[i]);
        }
    }

    set_overwrite(set, ns);
    set->isUsed = true;
}

bool find(Set* set, int item, int** ptrptr) {
    int* ptr = *ptrptr;

    int index = item;
    ptr = &set->buckets[index % set->bucketLength];
    while (*ptr != 0) {
        if (*ptr == item) {
            *ptrptr = ptr;
            return true;
        }
        index++;
        ptr = &set->buckets[index % set->bucketLength];
    }

    *ptrptr = ptr;
    return false;
}

Set* set_new() {
    Set* s = set_new_size(128);
    s->isUsed = false;
    return s;
}

void set_add(Set* set, int item) {
    int* ptr;
    set->isUsed = true;
    if (!find(set, item, &ptr)) {
        *ptr = item;
        set->load++;
        set->size++;
    }

    if (set->load > set->bucketLength * 0.75) {
        rebuild(set);
    }
}

void set_remove(Set* set, int item) {
    int* ptr;
    set->isUsed = true;
    if (find(set, item, &ptr)) {
        *ptr = -1;
        set->size--;
    }
}

bool set_has(Set* set, int item) {
    int* ptr;
    return find(set, item, &ptr);
}

void set_clear(Set* s) {
    s->bucketLength = 100;
    s->load = 0;
    s->size = 0;
    s->buckets = realloc(s->buckets, sizeof(int) * s->bucketLength);
    s->isUsed = false;
    memset(s->buckets, 0, sizeof(int) * s->bucketLength);
}

void set_intersect(Set* set, Set* other) {
    Set* ns = set_new();
    for (int i = 0; i < other->bucketLength; i++) {
        if (other->buckets[i] > 0) {
            if (set_has(set, other->buckets[i])) {
                set_add(ns, other->buckets[i]);
            }
        }
    }
    set_delete(other);
    set_overwrite(set, ns);
    set->isUsed = true;
}

void set_union(Set* set, Set* other) {
    for (int i = 0; i < other->bucketLength; i++) {
        if (other->buckets[i] > 0) {
            set_add(set, other->buckets[i]);
        }
    }
    set_delete(other);
    set->isUsed = true;
}

void set_delete(Set* set) {
    free(set->buckets);
    free(set);
}

Set* set_copy(Set* set) {
    Set* other = set_new();
    for (int i = 0; i < set->bucketLength; i++) {
        if (set->buckets[i] > 0) {
            set_add(other, set->buckets[i]);
        }
    }
    other->isUsed = set->isUsed;
    return other;
}

