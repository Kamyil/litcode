#include "btree.h"

#include <stdlib.h>
#include <string.h>

// TODO: define your node type and the tree struct.
struct BTree {
    int max_keys;
};

BTree *bt_new(int max_keys) {
    (void)max_keys;
    return NULL; // TODO
}

void bt_free(BTree *t) {
    (void)t; // TODO
}

bool bt_insert(BTree *t, int64_t key, int64_t value) {
    (void)t; (void)key; (void)value;
    return false; // TODO
}

bool bt_get(const BTree *t, int64_t key, int64_t *out) {
    (void)t; (void)key; (void)out;
    return false; // TODO
}

bool bt_delete(BTree *t, int64_t key) {
    (void)t; (void)key;
    return false; // TODO
}

size_t bt_len(const BTree *t) {
    (void)t;
    return 0; // TODO
}

int bt_height(const BTree *t) {
    (void)t;
    return 0; // TODO
}

size_t bt_range(const BTree *t, int64_t lo, int64_t hi, int64_t *keys, int64_t *vals, size_t max) {
    (void)t; (void)lo; (void)hi; (void)keys; (void)vals; (void)max;
    return 0; // TODO
}
