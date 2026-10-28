#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct BTree BTree; // opaque: you define it (and the node type) in btree.c

BTree *bt_new(int max_keys); // max keys per node, >= 3; NULL on OOM or bad argument
void bt_free(BTree *t);

bool bt_insert(BTree *t, int64_t key, int64_t value); // insert or overwrite; false on OOM
bool bt_get(const BTree *t, int64_t key, int64_t *out);
bool bt_delete(BTree *t, int64_t key); // no rebalancing (like Postgres nbtree)

size_t bt_len(const BTree *t);
int bt_height(const BTree *t); // 1 = the root is a leaf

// Writes up to `max` pairs with lo <= key <= hi, ascending; returns how many were written.
size_t bt_range(const BTree *t, int64_t lo, int64_t hi, int64_t *keys, int64_t *vals, size_t max);

#endif
