#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    void *data;       // buffer of cap * elem_size bytes (NULL when cap == 0)
    size_t len;       // number of elements in use
    size_t cap;       // number of elements that fit in the buffer
    size_t elem_size; // size of one element in bytes
} DynArray;

bool da_init(DynArray *a, size_t elem_size, size_t initial_cap);
void da_free(DynArray *a);

bool da_push(DynArray *a, const void *elem);
bool da_pop(DynArray *a, void *out);
void *da_get(const DynArray *a, size_t i);

bool da_insert(DynArray *a, size_t i, const void *elem);
bool da_remove(DynArray *a, size_t i, void *out);

bool da_reserve(DynArray *a, size_t min_cap);
void da_shrink_to_fit(DynArray *a);

#endif
