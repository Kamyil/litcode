#include "dynarray.h"

#include <stdlib.h>
#include <string.h>

static char *slot(const DynArray *a, size_t i) { return (char *)a->data + i * a->elem_size; }

// Reallocates the buffer to exactly `cap` elements. On failure the array is untouched.
static bool set_capacity(DynArray *a, size_t cap) {
    if (cap == 0) {
        free(a->data);
        a->data = NULL;
        a->cap = 0;
        return true;
    }
    void *fresh = realloc(a->data, cap * a->elem_size);
    if (!fresh) return false;
    a->data = fresh;
    a->cap = cap;
    return true;
}

bool da_init(DynArray *a, size_t elem_size, size_t initial_cap) {
    *a = (DynArray){.elem_size = elem_size};
    return set_capacity(a, initial_cap);
}

void da_free(DynArray *a) {
    free(a->data);
    a->data = NULL;
    a->len = a->cap = 0;
}

bool da_reserve(DynArray *a, size_t min_cap) {
    if (a->cap >= min_cap) return true;
    size_t cap = a->cap ? a->cap : 4;
    while (cap < min_cap) cap *= 2;
    return set_capacity(a, cap);
}

bool da_push(DynArray *a, const void *elem) {
    if (!da_reserve(a, a->len + 1)) return false;
    memcpy(slot(a, a->len++), elem, a->elem_size);
    return true;
}

bool da_pop(DynArray *a, void *out) {
    if (a->len == 0) return false;
    a->len--;
    if (out) memcpy(out, slot(a, a->len), a->elem_size);
    return true;
}

void *da_get(const DynArray *a, size_t i) { return i < a->len ? slot(a, i) : NULL; }

bool da_insert(DynArray *a, size_t i, const void *elem) {
    if (i > a->len || !da_reserve(a, a->len + 1)) return false;
    memmove(slot(a, i + 1), slot(a, i), (a->len - i) * a->elem_size);
    memcpy(slot(a, i), elem, a->elem_size);
    a->len++;
    return true;
}

bool da_remove(DynArray *a, size_t i, void *out) {
    if (i >= a->len) return false;
    if (out) memcpy(out, slot(a, i), a->elem_size);
    memmove(slot(a, i), slot(a, i + 1), (a->len - i - 1) * a->elem_size);
    a->len--;
    return true;
}

void da_shrink_to_fit(DynArray *a) { set_capacity(a, a->len); }
