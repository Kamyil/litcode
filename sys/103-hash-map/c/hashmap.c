#include "hashmap.h"

#include <stdlib.h>
#include <string.h>

uint64_t hm_hash(const char *key) {
    (void)key;
    return 0; // TODO: FNV-1a 64
}

bool hm_init(HashMap *m, size_t min_cap) {
    (void)m; (void)min_cap;
    return false; // TODO
}

void hm_free(HashMap *m) {
    (void)m; // TODO
}

bool hm_put(HashMap *m, const char *key, int64_t value) {
    (void)m; (void)key; (void)value;
    return false; // TODO
}

bool hm_get(const HashMap *m, const char *key, int64_t *out) {
    (void)m; (void)key; (void)out;
    return false; // TODO
}

bool hm_remove(HashMap *m, const char *key) {
    (void)m; (void)key;
    return false; // TODO
}

size_t hm_len(const HashMap *m) {
    (void)m;
    return 0; // TODO
}

HmIter hm_iter(const HashMap *m) {
    return (HmIter){.m = m, .i = 0};
}

bool hm_next(HmIter *it, const char **key, int64_t *value) {
    (void)it; (void)key; (void)value;
    return false; // TODO
}
