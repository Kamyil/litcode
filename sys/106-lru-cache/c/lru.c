#include "lru.h"

#include <stdlib.h>

// TODO: design your node and cache structs.
struct LruCache {
    size_t capacity;
};

LruCache *lru_new(size_t capacity) {
    (void)capacity;
    return NULL; // TODO
}

void lru_free(LruCache *c) {
    (void)c; // TODO
}

bool lru_get(LruCache *c, int64_t key, int64_t *out) {
    (void)c; (void)key; (void)out;
    return false; // TODO
}

bool lru_put(LruCache *c, int64_t key, int64_t value, int64_t *evicted_key) {
    (void)c; (void)key; (void)value; (void)evicted_key;
    return false; // TODO
}

size_t lru_len(const LruCache *c) {
    (void)c;
    return 0; // TODO
}
