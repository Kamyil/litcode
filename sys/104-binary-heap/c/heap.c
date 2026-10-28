#include "heap.h"

#include <stdlib.h>
#include <string.h>

void heap_init(Heap *h, size_t elem_size, heap_cmp cmp) {
    (void)h; (void)elem_size; (void)cmp; // TODO
}

void heap_free(Heap *h) {
    (void)h; // TODO
}

bool heap_push(Heap *h, const void *elem) {
    (void)h; (void)elem;
    return false; // TODO
}

bool heap_pop(Heap *h, void *out) {
    (void)h; (void)out;
    return false; // TODO
}

const void *heap_peek(const Heap *h) {
    (void)h;
    return NULL; // TODO
}

size_t heap_len(const Heap *h) {
    (void)h;
    return 0; // TODO
}

bool heap_from_array(Heap *h, const void *items, size_t n, size_t elem_size, heap_cmp cmp) {
    (void)h; (void)items; (void)n; (void)elem_size; (void)cmp;
    return false; // TODO
}

void heap_sort(void *base, size_t n, size_t elem_size, heap_cmp cmp) {
    (void)base; (void)n; (void)elem_size; (void)cmp; // TODO
}
