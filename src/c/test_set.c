#include <assert.h>
#include <stdio.h>
#include "set.h"

int test_set() {
    Set* s = set_new();
    set_add(s, 1);
    set_add(s, 101);
    set_add(s, 201);
    set_remove(s, 101);

    assert(set_has(s, 1));
    assert(!set_has(s, 101));
    assert(set_has(s, 201));

    set_remove(s, 101);
    set_remove(s, 101);
    set_remove(s, 201);
    set_remove(s, 1);

    assert(!set_has(s, 1));
    assert(!set_has(s, 101));
    assert(!set_has(s, 201));

    for (int i = 0; i < 1000; i++) {
        set_add(s, 2 + 100 * i);
    }

    assert(set_has(s, 2));
    assert(set_has(s, 1002));
    assert(set_has(s, 10002));

    fprintf(stderr, "Tested Set Implementation\n");
}
