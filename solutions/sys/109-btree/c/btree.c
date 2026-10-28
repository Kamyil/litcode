#include "btree.h"

#include <stdlib.h>
#include <string.h>

typedef struct BNode {
    bool leaf;
    int n;                // keys in use
    int64_t *keys;        // capacity max_keys + 1 (one spare for overflow before a split)
    int64_t *vals;        // leaves: capacity max_keys + 1
    struct BNode **kids;  // internals: capacity max_keys + 2
    struct BNode *next;   // leaves: right sibling
} BNode;

struct BTree {
    BNode *root;
    int max_keys;
    int height;
    size_t len;
};

static int lower_bound(const int64_t *keys, int n, int64_t k) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (keys[mid] < k) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

static int upper_bound(const int64_t *keys, int n, int64_t k) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (keys[mid] <= k) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

static void node_free(BNode *x) {
    if (!x) return;
    free(x->keys);
    free(x->vals);
    free(x->kids);
    free(x);
}

static BNode *node_new(const BTree *t, bool leaf) {
    BNode *x = calloc(1, sizeof *x);
    if (!x) return NULL;
    x->leaf = leaf;
    x->keys = malloc(sizeof(int64_t) * (size_t)(t->max_keys + 1));
    if (leaf) x->vals = malloc(sizeof(int64_t) * (size_t)(t->max_keys + 1));
    else x->kids = malloc(sizeof(BNode *) * (size_t)(t->max_keys + 2));
    if (!x->keys || (leaf ? !x->vals : !x->kids)) {
        node_free(x);
        return NULL;
    }
    return x;
}

static void subtree_free(BNode *x) {
    if (!x) return;
    if (!x->leaf)
        for (int i = 0; i <= x->n; i++) subtree_free(x->kids[i]);
    node_free(x);
}

BTree *bt_new(int max_keys) {
    if (max_keys < 3) return NULL;
    BTree *t = malloc(sizeof *t);
    if (!t) return NULL;
    *t = (BTree){.max_keys = max_keys, .height = 1};
    t->root = node_new(t, true);
    if (!t->root) {
        free(t);
        return NULL;
    }
    return t;
}

void bt_free(BTree *t) {
    if (!t) return;
    subtree_free(t->root);
    free(t);
}

static const BNode *find_leaf(const BTree *t, int64_t key) {
    const BNode *x = t->root;
    while (!x->leaf) x = x->kids[upper_bound(x->keys, x->n, key)];
    return x;
}

// Result of inserting into a subtree.
typedef enum { INS_OOM, INS_UPDATED, INS_ADDED, INS_SPLIT } InsResult;

// On INS_SPLIT, *sep and *right describe the new right sibling to add to the parent.
static InsResult insert_rec(BTree *t, BNode *x, int64_t key, int64_t val, int64_t *sep, BNode **right) {
    if (x->leaf) {
        int i = lower_bound(x->keys, x->n, key);
        if (i < x->n && x->keys[i] == key) {
            x->vals[i] = val;
            return INS_UPDATED;
        }
        if (x->n == t->max_keys) {
            *right = node_new(t, true); // allocate before modifying anything
            if (!*right) return INS_OOM;
        }
        memmove(&x->keys[i + 1], &x->keys[i], sizeof(int64_t) * (size_t)(x->n - i));
        memmove(&x->vals[i + 1], &x->vals[i], sizeof(int64_t) * (size_t)(x->n - i));
        x->keys[i] = key;
        x->vals[i] = val;
        x->n++;
        if (x->n <= t->max_keys) return INS_ADDED;
        int mid = x->n / 2;
        BNode *r = *right;
        r->n = x->n - mid;
        memcpy(r->keys, &x->keys[mid], sizeof(int64_t) * (size_t)r->n);
        memcpy(r->vals, &x->vals[mid], sizeof(int64_t) * (size_t)r->n);
        x->n = mid;
        r->next = x->next;
        x->next = r;
        *sep = r->keys[0];
        return INS_SPLIT;
    }

    int i = upper_bound(x->keys, x->n, key);
    int64_t child_sep;
    BNode *child_right = NULL;
    BNode *mine = NULL;
    if (x->n == t->max_keys) {
        mine = node_new(t, false); // we might have to split too
        if (!mine) return INS_OOM;
    }
    InsResult r = insert_rec(t, x->kids[i], key, val, &child_sep, &child_right);
    if (r != INS_SPLIT) {
        node_free(mine);
        return r;
    }
    memmove(&x->keys[i + 1], &x->keys[i], sizeof(int64_t) * (size_t)(x->n - i));
    memmove(&x->kids[i + 2], &x->kids[i + 1], sizeof(BNode *) * (size_t)(x->n - i));
    x->keys[i] = child_sep;
    x->kids[i + 1] = child_right;
    x->n++;
    if (x->n <= t->max_keys) {
        node_free(mine);
        return INS_ADDED;
    }
    int mid = x->n / 2;
    mine->n = x->n - mid - 1;
    memcpy(mine->keys, &x->keys[mid + 1], sizeof(int64_t) * (size_t)mine->n);
    memcpy(mine->kids, &x->kids[mid + 1], sizeof(BNode *) * (size_t)(mine->n + 1));
    *sep = x->keys[mid];
    x->n = mid;
    *right = mine;
    return INS_SPLIT;
}

bool bt_insert(BTree *t, int64_t key, int64_t value) {
    BNode *new_root = node_new(t, false); // in case the root splits
    if (!new_root) return false;
    int64_t sep;
    BNode *right = NULL;
    InsResult r = insert_rec(t, t->root, key, value, &sep, &right);
    if (r == INS_SPLIT) {
        new_root->n = 1;
        new_root->keys[0] = sep;
        new_root->kids[0] = t->root;
        new_root->kids[1] = right;
        t->root = new_root;
        t->height++;
    } else {
        node_free(new_root);
    }
    if (r == INS_ADDED || r == INS_SPLIT) t->len++;
    return r != INS_OOM;
}

bool bt_get(const BTree *t, int64_t key, int64_t *out) {
    const BNode *leaf = find_leaf(t, key);
    int i = lower_bound(leaf->keys, leaf->n, key);
    if (i == leaf->n || leaf->keys[i] != key) return false;
    if (out) *out = leaf->vals[i];
    return true;
}

bool bt_delete(BTree *t, int64_t key) {
    BNode *leaf = (BNode *)find_leaf(t, key);
    int i = lower_bound(leaf->keys, leaf->n, key);
    if (i == leaf->n || leaf->keys[i] != key) return false;
    memmove(&leaf->keys[i], &leaf->keys[i + 1], sizeof(int64_t) * (size_t)(leaf->n - i - 1));
    memmove(&leaf->vals[i], &leaf->vals[i + 1], sizeof(int64_t) * (size_t)(leaf->n - i - 1));
    leaf->n--;
    t->len--;
    return true;
}

size_t bt_len(const BTree *t) { return t->len; }

int bt_height(const BTree *t) { return t->height; }

size_t bt_range(const BTree *t, int64_t lo, int64_t hi, int64_t *keys, int64_t *vals, size_t max) {
    size_t out = 0;
    for (const BNode *leaf = find_leaf(t, lo); leaf && out < max; leaf = leaf->next) {
        for (int i = lower_bound(leaf->keys, leaf->n, lo); i < leaf->n && out < max; i++) {
            if (leaf->keys[i] > hi) return out;
            keys[out] = leaf->keys[i];
            vals[out] = leaf->vals[i];
            out++;
        }
    }
    return out;
}
