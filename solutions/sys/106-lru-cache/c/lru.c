#include "lru.h"

#include <stdlib.h>

typedef struct Node {
    int64_t key, value;
    struct Node *hnext;      // next node in the same hash bucket
    struct Node *prev, *next; // recency list
} Node;

struct LruCache {
    size_t capacity, len;
    size_t mask; // buckets - 1
    Node **buckets;
    Node head; // sentinel: head.next = MRU, head.prev = LRU
};

static size_t bucket_of(const LruCache *c, int64_t key) {
    return (size_t)(((uint64_t)key * 0x9E3779B97F4A7C15ULL) >> 32) & c->mask;
}

static void list_unlink(Node *n) {
    n->prev->next = n->next;
    n->next->prev = n->prev;
}

static void list_push_front(LruCache *c, Node *n) {
    n->prev = &c->head;
    n->next = c->head.next;
    c->head.next->prev = n;
    c->head.next = n;
}

// Pointer to the link that points at `key`'s node (or at NULL where it would go).
static Node **find_slot(LruCache *c, int64_t key) {
    Node **slot = &c->buckets[bucket_of(c, key)];
    while (*slot && (*slot)->key != key) slot = &(*slot)->hnext;
    return slot;
}

LruCache *lru_new(size_t capacity) {
    LruCache *c = malloc(sizeof *c);
    if (!c) return NULL;
    size_t nb = 8;
    while (nb < capacity) nb *= 2;
    c->buckets = calloc(nb, sizeof *c->buckets);
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    c->capacity = capacity;
    c->len = 0;
    c->mask = nb - 1;
    c->head.prev = c->head.next = &c->head;
    return c;
}

void lru_free(LruCache *c) {
    if (!c) return;
    Node *n = c->head.next;
    while (n != &c->head) {
        Node *next = n->next;
        free(n);
        n = next;
    }
    free(c->buckets);
    free(c);
}

bool lru_get(LruCache *c, int64_t key, int64_t *out) {
    Node *n = *find_slot(c, key);
    if (!n) return false;
    list_unlink(n);
    list_push_front(c, n);
    if (out) *out = n->value;
    return true;
}

bool lru_put(LruCache *c, int64_t key, int64_t value, int64_t *evicted_key) {
    Node **slot = find_slot(c, key);
    if (*slot) {
        (*slot)->value = value;
        list_unlink(*slot);
        list_push_front(c, *slot);
        return false;
    }
    Node *n;
    bool evicted = false;
    if (c->len == c->capacity) {
        // reuse the LRU node instead of free + malloc
        n = c->head.prev;
        list_unlink(n);
        Node **old = find_slot(c, n->key);
        *old = n->hnext;
        if (evicted_key) *evicted_key = n->key;
        evicted = true;
        c->len--;
        slot = find_slot(c, key); // the chain may have changed
    } else {
        n = malloc(sizeof *n);
        if (!n) return false;
    }
    n->key = key;
    n->value = value;
    n->hnext = NULL;
    *slot = n;
    list_push_front(c, n);
    c->len++;
    return evicted;
}

size_t lru_len(const LruCache *c) { return c->len; }
