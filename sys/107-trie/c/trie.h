#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct TrieNode {
    struct TrieNode *children[26];
    uint32_t prefix_count; // words in this subtree (including one ending here)
    bool end;              // a word ends here
} TrieNode;

typedef struct {
    TrieNode *root;
    size_t words;   // number of distinct words
    size_t nodes;   // allocated nodes, including root
    size_t max_len; // longest word ever inserted (handy for collect buffers)
} Trie;

bool trie_init(Trie *t);
void trie_free(Trie *t);

bool trie_insert(Trie *t, const char *word);
bool trie_contains(const Trie *t, const char *word);
size_t trie_count_prefix(const Trie *t, const char *prefix);
bool trie_remove(Trie *t, const char *word);

typedef void (*trie_visit)(const char *word, void *ctx);
size_t trie_collect(const Trie *t, const char *prefix, trie_visit cb, void *ctx);

#endif
