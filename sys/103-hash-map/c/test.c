#include "hashmap.h"
#include "test.h"

TEST(test_fnv1a_vectors) {
    CHECK(hm_hash("") == 0xcbf29ce484222325ULL);
    CHECK(hm_hash("a") == 0xaf63dc4c8601ec8cULL);
    CHECK(hm_hash("foobar") == 0x85944171f73967e8ULL);
}

TEST(test_init) {
    HashMap m;
    CHECK(hm_init(&m, 0));
    CHECK_EQ(m.cap, 8);
    hm_free(&m);
    CHECK(hm_init(&m, 100));
    CHECK_EQ(m.cap, 128);
    CHECK_EQ(hm_len(&m), 0);
    hm_free(&m);
    CHECK_NO_LEAKS();
}

TEST(test_put_get_overwrite) {
    HashMap m;
    hm_init(&m, 0);
    CHECK(hm_put(&m, "alice", 1));
    CHECK(hm_put(&m, "bob", 2));
    CHECK(hm_put(&m, "alice", 10));
    int64_t v = 0;
    CHECK(hm_get(&m, "alice", &v));
    CHECK_EQ(v, 10);
    CHECK(hm_get(&m, "bob", &v));
    CHECK_EQ(v, 2);
    CHECK(!hm_get(&m, "carol", &v));
    CHECK_EQ(hm_len(&m), 2);
    hm_free(&m);
    CHECK_NO_LEAKS();
}

TEST(test_owns_key_copies) {
    HashMap m;
    hm_init(&m, 0);
    char buf[16] = "temp";
    hm_put(&m, buf, 7);
    strcpy(buf, "changed");
    int64_t v = 0;
    CHECK(hm_get(&m, "temp", &v));
    CHECK_EQ(v, 7);
    hm_free(&m);
    CHECK_NO_LEAKS();
}

TEST(test_remove_and_probe_chains) {
    HashMap m;
    hm_init(&m, 8);
    char key[32];
    for (int i = 0; i < 5; i++) {
        snprintf(key, sizeof key, "k%d", i);
        hm_put(&m, key, i);
    }
    CHECK(hm_remove(&m, "k2"));
    CHECK(!hm_remove(&m, "k2"));
    CHECK_EQ(hm_len(&m), 4);
    int64_t v;
    for (int i = 0; i < 5; i++) {
        snprintf(key, sizeof key, "k%d", i);
        CHECK_EQ(hm_get(&m, key, &v), i != 2); // removal must not break other probe chains
    }
    hm_put(&m, "k2", 22);
    CHECK(hm_get(&m, "k2", &v));
    CHECK_EQ(v, 22);
    hm_free(&m);
    CHECK_NO_LEAKS();
}

TEST(test_many_keys) {
    HashMap m;
    hm_init(&m, 0);
    char key[32];
    enum { N = 100000 };
    for (int i = 0; i < N; i++) {
        snprintf(key, sizeof key, "key-%d", i);
        CHECK(hm_put(&m, key, i * 3));
    }
    CHECK_EQ(hm_len(&m), N);
    CHECK(m.cap >= N / 0.7 && m.cap <= 4 * N); // grew geometrically, load factor respected
    int64_t v;
    int bad = 0;
    for (int i = 0; i < N; i++) {
        snprintf(key, sizeof key, "key-%d", i);
        if (!hm_get(&m, key, &v) || v != i * 3) bad++;
    }
    CHECK_EQ(bad, 0);
    for (int i = 0; i < N; i += 2) {
        snprintf(key, sizeof key, "key-%d", i);
        hm_remove(&m, key);
    }
    CHECK_EQ(hm_len(&m), N / 2);
    CHECK(!hm_get(&m, "key-0", &v));
    CHECK(hm_get(&m, "key-1", &v));
    hm_free(&m);
    CHECK_NO_LEAKS();
}

TEST(test_tombstone_churn) {
    // Insert/remove the same few keys many times: tombstones must be reused or
    // cleaned up by rehashing, so capacity stays small and lookups terminate.
    HashMap m;
    hm_init(&m, 8);
    char key[32];
    for (int round = 0; round < 200000; round++) {
        snprintf(key, sizeof key, "churn-%d", round);
        hm_put(&m, key, round);
        hm_remove(&m, key);
    }
    CHECK_EQ(hm_len(&m), 0);
    CHECK(m.cap <= 64);
    int64_t v;
    CHECK(!hm_get(&m, "nope", &v)); // would loop forever if the table were all tombstones
    hm_free(&m);
    CHECK_NO_LEAKS();
}

TEST(test_iterate) {
    HashMap m;
    hm_init(&m, 0);
    char key[32];
    for (int i = 0; i < 50; i++) {
        snprintf(key, sizeof key, "%d", i);
        hm_put(&m, key, i);
    }
    hm_remove(&m, "7");
    int seen[50] = {0};
    int count = 0;
    HmIter it = hm_iter(&m);
    const char *k;
    int64_t v;
    while (hm_next(&it, &k, &v)) {
        CHECK_EQ(atoi(k), v);
        seen[v]++;
        count++;
    }
    CHECK_EQ(count, 49);
    CHECK_EQ(seen[7], 0);
    CHECK_EQ(seen[49], 1);
    hm_free(&m);
    CHECK_NO_LEAKS();
}

int main(void) {
    RUN(test_fnv1a_vectors);
    RUN(test_init);
    RUN(test_put_get_overwrite);
    RUN(test_owns_key_copies);
    RUN(test_remove_and_probe_chains);
    RUN(test_many_keys);
    RUN(test_tombstone_churn);
    RUN(test_iterate);
    TEST_SUMMARY();
}
