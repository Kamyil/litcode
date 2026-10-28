#ifndef LRU_H
#define LRU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct LruCache LruCache; // opaque: defined in lru.c

LruCache *lru_new(size_t capacity); // capacity >= 1; NULL on OOM
void lru_free(LruCache *c);

// Returns true and stores the value if present; marks the key most recently used.
bool lru_get(LruCache *c, int64_t key, int64_t *out);

// Insert or update (marks MRU). If inserting exceeds capacity, evicts the LRU entry and returns
// true with its key in *evicted_key (if non-NULL). Returns false otherwise.
bool lru_put(LruCache *c, int64_t key, int64_t value, int64_t *evicted_key);

size_t lru_len(const LruCache *c);

#endif
