#include "arena.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void arena_init(Arena *a, size_t block_size) { *a = (Arena){.block_size = block_size}; }

// Returns an aligned pointer for `size` bytes inside block b, or NULL if it doesn't fit.
static void *bump(ArenaBlock *b, size_t size, size_t align) {
    uintptr_t start = (uintptr_t)(b->data + b->used);
    uintptr_t p = (start + align - 1) & ~(uintptr_t)(align - 1);
    uintptr_t end = (uintptr_t)(b->data + b->cap);
    if (p > end || size > end - p) return NULL;
    b->used = (size_t)(p - (uintptr_t)b->data) + size;
    return (void *)p;
}

void *arena_alloc(Arena *a, size_t size, size_t align) {
    if (a->head) {
        void *p = bump(a->head, size, align);
        if (p) return p;
    }
    size_t cap = size + align > a->block_size ? size + align : a->block_size;
    ArenaBlock *b = malloc(sizeof *b + cap);
    if (!b) return NULL;
    b->next = a->head;
    b->cap = cap;
    b->used = 0;
    a->head = b;
    return bump(b, size, align);
}

char *arena_strdup(Arena *a, const char *s) {
    size_t n = strlen(s) + 1;
    char *p = arena_alloc(a, n, 1);
    if (p) memcpy(p, s, n);
    return p;
}

static void free_chain(ArenaBlock *b) {
    while (b) {
        ArenaBlock *next = b->next;
        free(b);
        b = next;
    }
}

void arena_reset(Arena *a) {
    if (!a->head) return;
    free_chain(a->head->next);
    a->head->next = NULL;
    a->head->used = 0;
}

void arena_free(Arena *a) {
    free_chain(a->head);
    a->head = NULL;
}

size_t arena_block_count(const Arena *a) {
    size_t n = 0;
    for (ArenaBlock *b = a->head; b; b = b->next) n++;
    return n;
}
