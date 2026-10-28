#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>

// Negative if a should come out of the heap before b (qsort-style).
typedef int (*heap_cmp)(const void *a, const void *b);

typedef struct {
    void *data;
    size_t len, cap, elem_size;
    heap_cmp cmp;
} Heap;

void heap_init(Heap *h, size_t elem_size, heap_cmp cmp); // no allocation
void heap_free(Heap *h);
bool heap_push(Heap *h, const void *elem);
bool heap_pop(Heap *h, void *out); // false when empty; out may be NULL
const void *heap_peek(const Heap *h); // NULL when empty
size_t heap_len(const Heap *h);

// Builds a heap from n items (copied) in O(n).
bool heap_from_array(Heap *h, const void *items, size_t n, size_t elem_size, heap_cmp cmp);

// In-place heapsort, ascending by cmp. Must not allocate.
void heap_sort(void *base, size_t n, size_t elem_size, heap_cmp cmp);

#endif
