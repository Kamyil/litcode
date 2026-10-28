#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum { SLOT_EMPTY = 0, SLOT_FULL, SLOT_TOMBSTONE } SlotState;

typedef struct {
    char *key; // owned copy (malloc'd) when FULL
    int64_t value;
    uint8_t state; // SlotState
} HmEntry;

typedef struct {
    HmEntry *entries;
    size_t cap;        // power of two
    size_t len;        // FULL slots
    size_t tombstones; // TOMBSTONE slots
} HashMap;

uint64_t hm_hash(const char *key); // FNV-1a 64

bool hm_init(HashMap *m, size_t min_cap); // cap = next power of two >= max(min_cap, 8)
void hm_free(HashMap *m);

bool hm_put(HashMap *m, const char *key, int64_t value); // insert or overwrite; false on OOM
bool hm_get(const HashMap *m, const char *key, int64_t *out);
bool hm_remove(HashMap *m, const char *key); // false if absent
size_t hm_len(const HashMap *m);

typedef struct {
    const HashMap *m;
    size_t i;
} HmIter;

HmIter hm_iter(const HashMap *m);
bool hm_next(HmIter *it, const char **key, int64_t *value); // false when done

#endif
