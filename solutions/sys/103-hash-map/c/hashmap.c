#include "hashmap.h"

#include <stdlib.h>
#include <string.h>

uint64_t hm_hash(const char *key) {
    uint64_t h = 0xcbf29ce484222325ULL;
    for (const unsigned char *p = (const unsigned char *)key; *p; p++) {
        h ^= *p;
        h *= 0x100000001b3ULL;
    }
    return h;
}

static bool alloc_table(HashMap *m, size_t cap) {
    HmEntry *e = calloc(cap, sizeof *e); // calloc: every state = SLOT_EMPTY
    if (!e) return false;
    m->entries = e;
    m->cap = cap;
    m->len = m->tombstones = 0;
    return true;
}

bool hm_init(HashMap *m, size_t min_cap) {
    size_t cap = 8;
    while (cap < min_cap) cap *= 2;
    return alloc_table(m, cap);
}

void hm_free(HashMap *m) {
    for (size_t i = 0; i < m->cap; i++)
        if (m->entries[i].state == SLOT_FULL) free(m->entries[i].key);
    free(m->entries);
    *m = (HashMap){0};
}

// Returns the slot holding `key`, or (if absent) the slot where it should be inserted:
// the first tombstone on the probe path, else the terminating empty slot.
static HmEntry *find(const HashMap *m, const char *key, bool *found) {
    size_t mask = m->cap - 1;
    HmEntry *tomb = NULL;
    for (size_t i = hm_hash(key) & mask;; i = (i + 1) & mask) {
        HmEntry *e = &m->entries[i];
        if (e->state == SLOT_EMPTY) {
            *found = false;
            return tomb ? tomb : e;
        }
        if (e->state == SLOT_TOMBSTONE) {
            if (!tomb) tomb = e;
        } else if (strcmp(e->key, key) == 0) {
            *found = true;
            return e;
        }
    }
}

static bool resize(HashMap *m, size_t cap) {
    HashMap old = *m;
    if (!alloc_table(m, cap)) {
        *m = old;
        return false;
    }
    for (size_t i = 0; i < old.cap; i++) {
        if (old.entries[i].state != SLOT_FULL) continue;
        bool found;
        HmEntry *e = find(m, old.entries[i].key, &found);
        *e = old.entries[i]; // moves the key pointer, no copy
        m->len++;
    }
    free(old.entries);
    return true;
}

bool hm_put(HashMap *m, const char *key, int64_t value) {
    if ((m->len + m->tombstones + 1) * 10 > m->cap * 7) {
        // lots of tombstones: rehash at the same size; otherwise grow
        size_t cap = (m->len + 1) * 10 > m->cap * 5 ? m->cap * 2 : m->cap;
        if (!resize(m, cap)) return false;
    }
    bool found;
    HmEntry *e = find(m, key, &found);
    if (found) {
        e->value = value;
        return true;
    }
    char *copy = strdup(key);
    if (!copy) return false;
    if (e->state == SLOT_TOMBSTONE) m->tombstones--;
    *e = (HmEntry){.key = copy, .value = value, .state = SLOT_FULL};
    m->len++;
    return true;
}

bool hm_get(const HashMap *m, const char *key, int64_t *out) {
    bool found;
    HmEntry *e = find(m, key, &found);
    if (found && out) *out = e->value;
    return found;
}

bool hm_remove(HashMap *m, const char *key) {
    bool found;
    HmEntry *e = find(m, key, &found);
    if (!found) return false;
    free(e->key);
    e->key = NULL;
    e->state = SLOT_TOMBSTONE;
    m->len--;
    m->tombstones++;
    return true;
}

size_t hm_len(const HashMap *m) { return m->len; }

HmIter hm_iter(const HashMap *m) { return (HmIter){.m = m, .i = 0}; }

bool hm_next(HmIter *it, const char **key, int64_t *value) {
    while (it->i < it->m->cap) {
        const HmEntry *e = &it->m->entries[it->i++];
        if (e->state == SLOT_FULL) {
            *key = e->key;
            *value = e->value;
            return true;
        }
    }
    return false;
}
