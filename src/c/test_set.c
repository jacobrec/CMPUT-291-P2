#include <assert.h>
#include <stdio.h>
#include "set.h"

void test_set() {
    Set* s = set_new();
    set_add(s, 1);
    assert(s->size == 1);
    set_add(s, 101);
    set_add(s, 201);
    assert(s->size == 3);
    set_remove(s, 101);
    assert(s->size == 2);

    assert(set_has(s, 1));
    assert(!set_has(s, 101));
    assert(set_has(s, 201));

    assert(s->size == 2);
    set_remove(s, 101);
    set_remove(s, 101);
    assert(s->size == 2);
    set_remove(s, 201);
    set_remove(s, 1);

    assert(!set_has(s, 1));
    assert(!set_has(s, 101));
    assert(!set_has(s, 201));
    assert(s->size == 0);

    for (int i = 0; i < 1000; i++) {
        set_add(s, 2 + 100 * i);
    }

    assert(set_has(s, 2));
    assert(set_has(s, 1002));
    assert(set_has(s, 10002));
    assert(s->size == 1000);

    set_delete(s);

    // Test union
    Set* a = set_new();
    Set* b = set_new();
    set_add(a, 1);
    set_add(a, 2);
    set_add(a, 3);
    set_add(b, 3);
    set_add(b, 4);
    set_add(b, 5);

    set_union(a, b);
    assert(set_has(a, 1));
    assert(set_has(a, 2));
    assert(set_has(a, 3));
    assert(set_has(a, 4));
    assert(set_has(a, 5));
    assert(a->size == 5);

    set_clear(a);
    b = set_new();
    set_add(a, 1);
    set_add(a, 2);
    set_add(a, 3);
    set_add(b, 3);
    set_add(b, 4);
    set_add(b, 5);
    set_intersect(a, b);
    assert(!set_has(a, 1));
    assert(!set_has(a, 2));
    assert(set_has(a, 3));
    assert(!set_has(a, 4));
    assert(!set_has(a, 5));

    assert(a->size == 1);
    set_delete(a);

    s = set_new();
    int edgecount = 50;
    for (int i = 0; i < edgecount; i++) {
        set_add(s, 127+128*i);
    }
    for (int i = 0; i < edgecount; i++) {
        assert(set_has(s, 127+128*i));
    }
    assert(s->size == edgecount);
    for (int i = 0; i < edgecount; i++) {
        set_remove(s, 127+128*i);
    }
    for (int i = 0; i < edgecount; i++) {
        assert(!set_has(s, 127+128*i));
    }
    assert(s->load == edgecount);
    assert(s->size == 0);

    fprintf(stderr, "Tested Set Implementation\n");
}
