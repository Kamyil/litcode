#include <math.h>

#include "btree.h"
#include "test.h"

static uint64_t rng = 88172645463325252ULL;
static int64_t rnd(int64_t n) {
    rng ^= rng << 13;
    rng ^= rng >> 7;
    rng ^= rng << 17;
    return (int64_t)(rng % (uint64_t)n);
}

TEST(test_bad_args) {
    CHECK(bt_new(2) == NULL);
    CHECK_NO_LEAKS();
}

TEST(test_small) {
    BTree *t = bt_new(4);
    CHECK(t != NULL);
    if (!t) return;
    CHECK_EQ(bt_height(t), 1);
    int64_t v;
    CHECK(!bt_get(t, 1, &v));
    int64_t ks[] = {50, 20, 80, 10, 30, 70, 90, 60, 40};
    for (int i = 0; i < 9; i++) CHECK(bt_insert(t, ks[i], ks[i] * 10));
    CHECK_EQ(bt_len(t), 9);
    CHECK(bt_height(t) >= 2); // 9 keys can't fit in one leaf of 4
    for (int i = 0; i < 9; i++) {
        CHECK(bt_get(t, ks[i], &v));
        CHECK_EQ(v, ks[i] * 10);
    }
    CHECK(!bt_get(t, 55, &v));
    CHECK(bt_insert(t, 30, -1)); // overwrite
    CHECK_EQ(bt_len(t), 9);
    CHECK(bt_get(t, 30, &v));
    CHECK_EQ(v, -1);
    bt_free(t);
    CHECK_NO_LEAKS();
}

TEST(test_sequential_height_bounds) {
    for (int m = 3; m <= 64; m *= 2) {
        BTree *t = bt_new(m);
        if (!t) { CHECK(t != NULL); return; }
        int n = 100000;
        for (int i = 0; i < n; i++) bt_insert(t, i, i);
        CHECK_EQ(bt_len(t), (size_t)n);
        int min_fanout = (m + 2) / 2; // ceil((m + 1) / 2) children per internal node
        int max_height = 2 + (int)ceil(log((double)n) / log((double)min_fanout));
        int min_height = (int)ceil(log((double)n / m) / log((double)(m + 1))) + 1;
        CHECK(bt_height(t) <= max_height);
        CHECK(bt_height(t) >= min_height);
        int64_t v;
        int bad = 0;
        for (int i = 0; i < n; i += 7)
            if (!bt_get(t, i, &v) || v != i) bad++;
        CHECK_EQ(bad, 0);
        bt_free(t);
    }
    CHECK_NO_LEAKS();
}

static int cmp64(const void *a, const void *b) {
    int64_t x = *(const int64_t *)a, y = *(const int64_t *)b;
    return (x > y) - (x < y);
}

TEST(test_random_and_range) {
    BTree *t = bt_new(5);
    if (!t) { CHECK(t != NULL); return; }
    enum { N = 30000 };
    static int64_t keys[N], uniq[N];
    for (int i = 0; i < N; i++) {
        keys[i] = rnd(1000000) - 500000;
        bt_insert(t, keys[i], keys[i] * 2);
    }
    memcpy(uniq, keys, sizeof keys);
    qsort(uniq, N, sizeof *uniq, cmp64);
    size_t u = 0;
    for (size_t i = 0; i < N; i++)
        if (i == 0 || uniq[i] != uniq[i - 1]) uniq[u++] = uniq[i];
    CHECK_EQ(bt_len(t), u);

    static int64_t ok[N], ov[N];
    size_t got = bt_range(t, INT64_MIN, INT64_MAX, ok, ov, N);
    CHECK_EQ(got, u);
    int bad = 0;
    for (size_t i = 0; i < got && i < u; i++)
        if (ok[i] != uniq[i] || ov[i] != uniq[i] * 2) bad++;
    CHECK_EQ(bad, 0);

    // a window in the middle
    got = bt_range(t, -1000, 1000, ok, ov, N);
    size_t want = 0;
    for (size_t i = 0; i < u; i++)
        if (uniq[i] >= -1000 && uniq[i] <= 1000) want++;
    CHECK_EQ(got, want);
    for (size_t i = 1; i < got; i++) CHECK(ok[i - 1] < ok[i]);
    // max limits output
    CHECK_EQ(bt_range(t, INT64_MIN, INT64_MAX, ok, ov, 10), 10);
    CHECK_EQ(ok[0], uniq[0]);
    bt_free(t);
    CHECK_NO_LEAKS();
}

TEST(test_delete_without_rebalancing) {
    BTree *t = bt_new(4);
    if (!t) { CHECK(t != NULL); return; }
    for (int i = 0; i < 1000; i++) bt_insert(t, i, i);
    int h = bt_height(t);
    for (int i = 0; i < 1000; i += 2) CHECK(bt_delete(t, i));
    CHECK(!bt_delete(t, 0));
    CHECK(!bt_delete(t, 5000));
    CHECK_EQ(bt_len(t), 500);
    CHECK_EQ(bt_height(t), h); // no merging
    int64_t v, ks[600], vs[600];
    CHECK(!bt_get(t, 10, &v));
    CHECK(bt_get(t, 11, &v));
    size_t got = bt_range(t, 0, 99, ks, vs, 600);
    CHECK_EQ(got, 50);
    CHECK_EQ(ks[0], 1);
    // delete everything in a few leaves entirely: range scans must step over empty leaves
    for (int i = 101; i < 300; i += 2) bt_delete(t, i);
    got = bt_range(t, 90, 320, ks, vs, 600);
    CHECK_EQ(got, 5 + 10); // 91..99 and 301..319
    CHECK(bt_insert(t, 200, 2)); // inserting into an emptied leaf works
    CHECK(bt_get(t, 200, &v));
    bt_free(t);
    CHECK_NO_LEAKS();
}

int main(void) {
    RUN(test_bad_args);
    RUN(test_small);
    RUN(test_sequential_height_bounds);
    RUN(test_random_and_range);
    RUN(test_delete_without_rebalancing);
    TEST_SUMMARY();
}
