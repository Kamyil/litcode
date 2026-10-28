#include "trie.h"

#include <stdlib.h>
#include <string.h>

static void free_subtree(TrieNode *n, size_t *nodes) {
    if (!n) return;
    for (int i = 0; i < 26; i++) free_subtree(n->children[i], nodes);
    free(n);
    (*nodes)--;
}

static bool valid_word(const char *w) {
    for (; *w; w++)
        if (*w < 'a' || *w > 'z') return false;
    return true;
}

static const TrieNode *walk(const Trie *t, const char *s) {
    const TrieNode *n = t->root;
    for (; n && *s; s++) {
        if (*s < 'a' || *s > 'z') return NULL;
        n = n->children[*s - 'a'];
    }
    return n;
}

bool trie_init(Trie *t) {
    *t = (Trie){0};
    t->root = calloc(1, sizeof *t->root);
    if (!t->root) return false;
    t->nodes = 1;
    return true;
}

void trie_free(Trie *t) {
    free_subtree(t->root, &t->nodes);
    *t = (Trie){0};
}

bool trie_contains(const Trie *t, const char *word) {
    const TrieNode *n = walk(t, word);
    return n && n->end;
}

bool trie_insert(Trie *t, const char *word) {
    if (!valid_word(word)) return false;
    if (trie_contains(t, word)) return true;
    // allocate the missing tail first, so OOM leaves the trie unchanged
    TrieNode *n = t->root;
    const char *p = word;
    while (*p && n->children[*p - 'a']) n = n->children[*p++ - 'a'];
    size_t missing = strlen(p);
    TrieNode *chain = NULL, *last = NULL;
    for (size_t i = 0; i < missing; i++) {
        TrieNode *fresh = calloc(1, sizeof *fresh);
        if (!fresh) {
            size_t dummy = 0;
            free_subtree(chain, &dummy);
            return false;
        }
        if (!chain) chain = fresh;
        else last->children[p[i] - 'a'] = fresh;
        last = fresh;
    }
    if (chain) n->children[*p - 'a'] = chain;
    t->nodes += missing;
    // second pass: counters and end flag
    n = t->root;
    n->prefix_count++;
    for (p = word; *p; p++) {
        n = n->children[*p - 'a'];
        n->prefix_count++;
    }
    n->end = true;
    t->words++;
    size_t len = strlen(word);
    if (len > t->max_len) t->max_len = len;
    return true;
}

size_t trie_count_prefix(const Trie *t, const char *prefix) {
    const TrieNode *n = walk(t, prefix);
    return n ? n->prefix_count : 0;
}

bool trie_remove(Trie *t, const char *word) {
    if (!trie_contains(t, word)) return false;
    TrieNode *n = t->root;
    n->prefix_count--;
    for (const char *p = word; *p; p++) {
        TrieNode **child = &n->children[*p - 'a'];
        if (--(*child)->prefix_count == 0) {
            free_subtree(*child, &t->nodes); // only this word lived below here
            *child = NULL;
            t->words--;
            return true;
        }
        n = *child;
    }
    n->end = false;
    t->words--;
    return true;
}

static size_t dfs(const TrieNode *n, char *buf, size_t depth, trie_visit cb, void *ctx) {
    size_t count = 0;
    if (n->end) {
        buf[depth] = '\0';
        cb(buf, ctx);
        count++;
    }
    for (int i = 0; i < 26; i++) {
        if (!n->children[i]) continue;
        buf[depth] = (char)('a' + i);
        count += dfs(n->children[i], buf, depth + 1, cb, ctx);
    }
    return count;
}

size_t trie_collect(const Trie *t, const char *prefix, trie_visit cb, void *ctx) {
    const TrieNode *n = walk(t, prefix);
    if (!n) return 0;
    size_t plen = strlen(prefix);
    char *buf = malloc(plen + t->max_len + 1);
    if (!buf) return 0;
    memcpy(buf, prefix, plen);
    size_t count = dfs(n, buf, plen, cb, ctx);
    free(buf);
    return count;
}
