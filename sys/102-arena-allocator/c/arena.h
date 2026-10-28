#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct ArenaBlock {
    struct ArenaBlock *next; // previous (older) block
    size_t cap;              // payload bytes in data[]
    size_t used;             // payload bytes handed out so far
    unsigned char data[];    // flexible array member: payload follows the header
} ArenaBlock;

typedef struct {
    ArenaBlock *head; // current block (NULL before the first allocation)
    size_t block_size;
} Arena;

void arena_init(Arena *a, size_t block_size);
void *arena_alloc(Arena *a, size_t size, size_t align);
char *arena_strdup(Arena *a, const char *s);
void arena_reset(Arena *a);
void arena_free(Arena *a);
size_t arena_block_count(const Arena *a);

// Convenience: allocate one T / n Ts with the right alignment.
#define ARENA_NEW(a, T) ((T *)arena_alloc((a), sizeof(T), _Alignof(T)))
#define ARENA_ARRAY(a, T, n) ((T *)arena_alloc((a), sizeof(T) * (n), _Alignof(T)))

#endif
