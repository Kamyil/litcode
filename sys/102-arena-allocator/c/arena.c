#include "arena.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void arena_init(Arena *a, size_t block_size) {
    (void)a; (void)block_size; // TODO
}

void *arena_alloc(Arena *a, size_t size, size_t align) {
    (void)a; (void)size; (void)align;
    return NULL; // TODO
}

char *arena_strdup(Arena *a, const char *s) {
    (void)a; (void)s;
    return NULL; // TODO
}

void arena_reset(Arena *a) {
    (void)a; // TODO
}

void arena_free(Arena *a) {
    (void)a; // TODO
}

size_t arena_block_count(const Arena *a) {
    (void)a;
    return 0; // TODO
}
