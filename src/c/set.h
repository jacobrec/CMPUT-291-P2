#include <stdbool.h>
#ifndef JSET
#define JSET

struct set {
    int* buckets;
    int bucketLength;
    int size;
};
typedef struct set Set;

Set* set_new();
void set_add(Set* set, int item);
void set_remove(Set* set, int item);
bool set_has(Set* set, int item);
void set_clear(Set* set);
void set_intersect(Set* set, Set* other);
void set_delete(Set* set);
#endif
