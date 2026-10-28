#include "trie.h"

#include <stdlib.h>
#include <string.h>

bool trie_init(Trie *t) {
    (void)t;
    return false; // TODO
}

void trie_free(Trie *t) {
    (void)t; // TODO
}

bool trie_insert(Trie *t, const char *word) {
    (void)t; (void)word;
    return false; // TODO
}

bool trie_contains(const Trie *t, const char *word) {
    (void)t; (void)word;
    return false; // TODO
}

size_t trie_count_prefix(const Trie *t, const char *prefix) {
    (void)t; (void)prefix;
    return 0; // TODO
}

bool trie_remove(Trie *t, const char *word) {
    (void)t; (void)word;
    return false; // TODO
}

size_t trie_collect(const Trie *t, const char *prefix, trie_visit cb, void *ctx) {
    (void)t; (void)prefix; (void)cb; (void)ctx;
    return 0; // TODO
}
