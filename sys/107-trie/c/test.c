#include "test.h"
#include "trie.h"

typedef struct {
    char words[64][32];
    int n;
} Collected;

static void collect_cb(const char *word, void *ctx) {
    Collected *c = ctx;
    if (c->n < 64) snprintf(c->words[c->n++], 32, "%s", word);
}

TEST(test_insert_contains) {
    Trie t;
    CHECK(trie_init(&t));
    CHECK_EQ(t.nodes, 1);
    CHECK(trie_insert(&t, "apple"));
    CHECK(trie_contains(&t, "apple"));
    CHECK(!trie_contains(&t, "app"));
    CHECK(trie_insert(&t, "app"));
    CHECK(trie_contains(&t, "app"));
    CHECK(trie_insert(&t, "app")); // duplicate: no-op
    CHECK_EQ(t.words, 2);
    CHECK_EQ(t.nodes, 6); // root + a-p-p-l-e
    CHECK(!trie_insert(&t, "Bad"));
    CHECK(!trie_insert(&t, "no space"));
    CHECK_EQ(t.nodes, 6); // invalid words leave no nodes behind
    trie_free(&t);
    CHECK_NO_LEAKS();
}

TEST(test_count_prefix) {
    Trie t;
    trie_init(&t);
    const char *ws[] = {"car", "card", "care", "careful", "cat", "dog"};
    for (int i = 0; i < 6; i++) trie_insert(&t, ws[i]);
    CHECK_EQ(trie_count_prefix(&t, "car"), 4);
    CHECK_EQ(trie_count_prefix(&t, "ca"), 5);
    CHECK_EQ(trie_count_prefix(&t, ""), 6);
    CHECK_EQ(trie_count_prefix(&t, "x"), 0);
    CHECK_EQ(trie_count_prefix(&t, "careful"), 1);
    trie_free(&t);
    CHECK_NO_LEAKS();
}

TEST(test_remove_prunes) {
    Trie t;
    trie_init(&t);
    trie_insert(&t, "car");
    trie_insert(&t, "careful");
    size_t with_both = t.nodes; // root c a r e f u l = 8
    CHECK_EQ(with_both, 8);
    CHECK(!trie_remove(&t, "ca"));    // not a word
    CHECK(!trie_remove(&t, "caring")); // not present
    CHECK_EQ(trie_count_prefix(&t, "c"), 2);
    CHECK(trie_remove(&t, "careful"));
    CHECK_EQ(t.nodes, 4); // e-f-u-l freed
    CHECK(trie_contains(&t, "car"));
    CHECK(!trie_contains(&t, "careful"));
    CHECK(trie_remove(&t, "car"));
    CHECK_EQ(t.nodes, 1);
    CHECK_EQ(t.words, 0);
    CHECK_EQ(ta_live_allocs() - lc_live_at_start, 1); // only the root left
    trie_free(&t);
    CHECK_NO_LEAKS();
}

TEST(test_remove_keeps_shared_prefix) {
    Trie t;
    trie_init(&t);
    trie_insert(&t, "car");
    trie_insert(&t, "cart");
    trie_insert(&t, "cat");
    CHECK(trie_remove(&t, "car"));
    CHECK_EQ(t.nodes, 6); // root c a r t + t; "car" node still needed for "cart"
    CHECK(trie_contains(&t, "cart"));
    CHECK(trie_contains(&t, "cat"));
    CHECK_EQ(trie_count_prefix(&t, "car"), 1);
    trie_free(&t);
    CHECK_NO_LEAKS();
}

TEST(test_collect_sorted) {
    Trie t;
    trie_init(&t);
    const char *ws[] = {"careful", "cat", "car", "dog", "card", "ca", "care"};
    for (int i = 0; i < 7; i++) trie_insert(&t, ws[i]);
    Collected c = {.n = 0};
    CHECK_EQ(trie_collect(&t, "ca", collect_cb, &c), 6);
    const char *want[] = {"ca", "car", "card", "care", "careful", "cat"};
    for (int i = 0; i < 6 && i < c.n; i++) CHECK_STR(c.words[i], want[i]);
    c.n = 0;
    CHECK_EQ(trie_collect(&t, "z", collect_cb, &c), 0);
    CHECK_EQ(trie_collect(&t, "", collect_cb, &c), 7);
    trie_free(&t);
    CHECK_NO_LEAKS();
}

TEST(test_bulk_insert_remove) {
    Trie t;
    trie_init(&t);
    char w[16];
    for (int i = 0; i < 20000; i++) {
        int x = i, k = 0;
        do { w[k++] = (char)('a' + x % 26); x /= 26; } while (x);
        w[k] = '\0';
        trie_insert(&t, w);
    }
    CHECK_EQ(t.words, 20000);
    for (int i = 0; i < 20000; i++) {
        int x = i, k = 0;
        do { w[k++] = (char)('a' + x % 26); x /= 26; } while (x);
        w[k] = '\0';
        CHECK(trie_remove(&t, w));
    }
    CHECK_EQ(t.words, 0);
    CHECK_EQ(t.nodes, 1);
    trie_free(&t);
    CHECK_NO_LEAKS();
}

int main(void) {
    RUN(test_insert_contains);
    RUN(test_count_prefix);
    RUN(test_remove_prunes);
    RUN(test_remove_keeps_shared_prefix);
    RUN(test_collect_sorted);
    RUN(test_bulk_insert_remove);
    TEST_SUMMARY();
}
