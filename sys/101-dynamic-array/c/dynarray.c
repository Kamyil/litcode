#include "dynarray.h"

#include <stdlib.h>
#include <string.h>

// Tip: write a static helper `slot(a, i)` returning a pointer to element i first.

bool da_init(DynArray *a, size_t elem_size, size_t initial_cap) {
    (void)a; (void)elem_size; (void)initial_cap;
    return false; // TODO
}

void da_free(DynArray *a) {
    (void)a; // TODO
}

bool da_reserve(DynArray *a, size_t min_cap) {
    (void)a; (void)min_cap;
    return false; // TODO
}

bool da_push(DynArray *a, const void *elem) {
    (void)a; (void)elem;
    return false; // TODO
}

bool da_pop(DynArray *a, void *out) {
    (void)a; (void)out;
    return false; // TODO
}

void *da_get(const DynArray *a, size_t i) {
    (void)a; (void)i;
    return NULL; // TODO
}

bool da_insert(DynArray *a, size_t i, const void *elem) {
    (void)a; (void)i; (void)elem;
    return false; // TODO
}

bool da_remove(DynArray *a, size_t i, void *out) {
    (void)a; (void)i; (void)out;
    return false; // TODO
}

void da_shrink_to_fit(DynArray *a) {
    (void)a; // TODO
}
