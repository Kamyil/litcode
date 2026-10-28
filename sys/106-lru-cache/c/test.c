#include "lru.h"
#include "test.h"

TEST(test_example) {
    LruCache *c = lru_new(2);
    CHECK(c != NULL);
    if (!c) return;
    int64_t v, ev;
    CHECK(!lru_put(c, 1, 1, &ev));
    CHECK(!lru_put(c, 2, 2, &ev));
    CHECK(lru_get(c, 1, &v));
    CHECK_EQ(v, 1);
    CHECK(lru_put(c, 3, 3, &ev)); // evicts 2
    CHECK_EQ(ev, 2);
    CHECK(!lru_get(c, 2, &v));
    CHECK(lru_put(c, 4, 4, &ev)); // evicts 1
    CHECK_EQ(ev, 1);
    CHECK(lru_get(c, 3, &v));
    CHECK_EQ(v, 3);
    CHECK(lru_get(c, 4, &v));
    CHECK_EQ(v, 4);
    CHECK_EQ(lru_len(c), 2);
    lru_free(c);
    CHECK_NO_LEAKS();
}

TEST(test_update_refreshes) {
    LruCache *c = lru_new(2);
    if (!c) { CHECK(c != NULL); return; }
    int64_t v, ev = -1;
    lru_put(c, 1, 1, NULL);
    lru_put(c, 2, 2, NULL);
    CHECK(!lru_put(c, 1, 10, &ev)); // update, no eviction
    CHECK(lru_put(c, 3, 3, &ev));
    CHECK_EQ(ev, 2);
    CHECK(lru_get(c, 1, &v));
    CHECK_EQ(v, 10);
    lru_free(c);
    CHECK_NO_LEAKS();
}

TEST(test_memory_stays_bounded) {
    LruCache *c = lru_new(100);
    if (!c) { CHECK(c != NULL); return; }
    size_t base = ta_live_allocs();
    for (int64_t i = 0; i < 200000; i++) lru_put(c, i, i * 2, NULL);
    CHECK_EQ(lru_len(c), 100);
    CHECK(ta_live_allocs() - base <= 110); // evicted nodes were freed (or reused)
    int64_t v;
    CHECK(lru_get(c, 199999, &v));
    CHECK_EQ(v, 399998);
    CHECK(!lru_get(c, 199899, &v));
    CHECK(lru_get(c, 199900, &v));
    lru_free(c);
    CHECK_NO_LEAKS();
}

TEST(test_against_model) {
    // reference model: arrays with timestamps (O(n) but obviously correct)
    enum { CAP = 16, KEYS = 40 };
    LruCache *c = lru_new(CAP);
    if (!c) { CHECK(c != NULL); return; }
    int64_t mval[KEYS];
    long mtime[KEYS];
    bool present[KEYS] = {false};
    size_t count = 0;
    unsigned s = 7;
    int bad = 0;
    for (long t = 1; t < 50000; t++) {
        s = s * 1103515245u + 12345u;
        int k = (int)((s >> 8) % KEYS);
        if ((s >> 20) & 1) {
            int64_t v;
            bool hit = lru_get(c, k, &v);
            if (hit != present[k] || (hit && v != mval[k])) bad++;
            if (present[k]) mtime[k] = t;
        } else {
            int64_t ev = -1;
            bool evicted = lru_put(c, k, t, &ev);
            if (!present[k] && count == CAP) {
                int lru = -1;
                for (int i = 0; i < KEYS; i++)
                    if (present[i] && (lru < 0 || mtime[i] < mtime[lru])) lru = i;
                if (!evicted || ev != lru) bad++;
                present[lru] = false;
                count--;
            } else if (evicted) {
                bad++;
            }
            if (!present[k]) count++;
            present[k] = true;
            mval[k] = t;
            mtime[k] = t;
        }
    }
    CHECK_EQ(bad, 0);
    CHECK_EQ(lru_len(c), count);
    lru_free(c);
    CHECK_NO_LEAKS();
}

int main(void) {
    RUN(test_example);
    RUN(test_update_refreshes);
    RUN(test_memory_stays_bounded);
    RUN(test_against_model);
    TEST_SUMMARY();
}
