#include "heap.h"

#include <stdlib.h>
#include <string.h>

static void swap_bytes(void *a, void *b, size_t n) {
    unsigned char *x = a, *y = b;
    for (size_t i = 0; i < n; i++) {
        unsigned char t = x[i];
        x[i] = y[i];
        y[i] = t;
    }
}

#define AT(base, i, size) ((char *)(base) + (i) * (size))

// Restores the heap property below index i in base[0..n). `invert` flips the order.
static void sift_down(void *base, size_t n, size_t size, heap_cmp cmp, bool invert, size_t i) {
    for (;;) {
        size_t best = i, l = 2 * i + 1, r = l + 1;
        if (l < n && (invert ? cmp(AT(base, best, size), AT(base, l, size)) : cmp(AT(base, l, size), AT(base, best, size))) < 0) best = l;
        if (r < n && (invert ? cmp(AT(base, best, size), AT(base, r, size)) : cmp(AT(base, r, size), AT(base, best, size))) < 0) best = r;
        if (best == i) return;
        swap_bytes(AT(base, i, size), AT(base, best, size), size);
        i = best;
    }
}

static void sift_up(Heap *h, size_t i) {
    while (i > 0) {
        size_t p = (i - 1) / 2;
        if (h->cmp(AT(h->data, i, h->elem_size), AT(h->data, p, h->elem_size)) >= 0) return;
        swap_bytes(AT(h->data, i, h->elem_size), AT(h->data, p, h->elem_size), h->elem_size);
        i = p;
    }
}

void heap_init(Heap *h, size_t elem_size, heap_cmp cmp) {
    *h = (Heap){.elem_size = elem_size, .cmp = cmp};
}

void heap_free(Heap *h) {
    free(h->data);
    h->data = NULL;
    h->len = h->cap = 0;
}

bool heap_push(Heap *h, const void *elem) {
    if (h->len == h->cap) {
        size_t cap = h->cap ? h->cap * 2 : 8;
        void *p = realloc(h->data, cap * h->elem_size);
        if (!p) return false;
        h->data = p;
        h->cap = cap;
    }
    memcpy(AT(h->data, h->len, h->elem_size), elem, h->elem_size);
    sift_up(h, h->len++);
    return true;
}

bool heap_pop(Heap *h, void *out) {
    if (h->len == 0) return false;
    if (out) memcpy(out, h->data, h->elem_size);
    h->len--;
    if (h->len) {
        memcpy(h->data, AT(h->data, h->len, h->elem_size), h->elem_size);
        sift_down(h->data, h->len, h->elem_size, h->cmp, false, 0);
    }
    return true;
}

const void *heap_peek(const Heap *h) { return h->len ? h->data : NULL; }

size_t heap_len(const Heap *h) { return h->len; }

bool heap_from_array(Heap *h, const void *items, size_t n, size_t elem_size, heap_cmp cmp) {
    heap_init(h, elem_size, cmp);
    if (n == 0) return true;
    h->data = malloc(n * elem_size);
    if (!h->data) return false;
    memcpy(h->data, items, n * elem_size);
    h->len = h->cap = n;
    for (size_t i = n / 2; i-- > 0;) sift_down(h->data, n, elem_size, cmp, false, i);
    return true;
}

void heap_sort(void *base, size_t n, size_t elem_size, heap_cmp cmp) {
    if (n < 2) return;
    for (size_t i = n / 2; i-- > 0;) sift_down(base, n, elem_size, cmp, true, i);
    for (size_t end = n - 1; end > 0; end--) {
        swap_bytes(base, AT(base, end, elem_size), elem_size);
        sift_down(base, end, elem_size, cmp, true, 0);
    }
}
