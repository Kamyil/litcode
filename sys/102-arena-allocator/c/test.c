#include <stdint.h>

#include "arena.h"
#include "test.h"

TEST(test_lazy_init) {
    Arena a;
    arena_init(&a, 1024);
    CHECK_EQ(arena_block_count(&a), 0);
    CHECK_EQ(ta_live_allocs(), lc_live_at_start);
    arena_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_alignment) {
    Arena a;
    arena_init(&a, 4096);
    size_t aligns[] = {1, 2, 4, 8, 16, 32, 64, 1, 8, 4096};
    for (int round = 0; round < 50; round++) {
        for (size_t i = 0; i < sizeof aligns / sizeof *aligns; i++) {
            void *p = arena_alloc(&a, (size_t)(round % 7) + 1, aligns[i]);
            CHECK(p != NULL);
            CHECK_EQ((uintptr_t)p % aligns[i], 0);
        }
    }
    arena_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_few_mallocs) {
    Arena a;
    arena_init(&a, 64 * 1024);
    size_t before = ta_total_allocs();
    for (int i = 0; i < 1000; i++) CHECK(ARENA_NEW(&a, int64_t) != NULL);
    CHECK_EQ(ta_total_allocs() - before, 1); // 8000 bytes fit in one 64 KiB block
    CHECK_EQ(arena_block_count(&a), 1);
    arena_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_no_overlap) {
    Arena a;
    arena_init(&a, 256); // small blocks -> many blocks
    enum { N = 2000 };
    unsigned char *ptrs[N];
    size_t sizes[N];
    for (int i = 0; i < N; i++) {
        sizes[i] = (size_t)(i * 37 % 61) + 1;
        ptrs[i] = arena_alloc(&a, sizes[i], 8);
        CHECK(ptrs[i] != NULL);
        if (ptrs[i]) memset(ptrs[i], i & 0xff, sizes[i]);
    }
    for (int i = 0; i < N; i++) {
        if (!ptrs[i]) continue;
        for (size_t j = 0; j < sizes[i]; j++) {
            if (ptrs[i][j] != (unsigned char)(i & 0xff)) {
                CHECK(!"allocation was overwritten by a later one");
                i = N;
                break;
            }
        }
    }
    CHECK(arena_block_count(&a) > 1);
    arena_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_big_allocation) {
    Arena a;
    arena_init(&a, 128);
    char *big = arena_alloc(&a, 10000, 16);
    CHECK(big != NULL);
    if (big) {
        memset(big, 'x', 10000); // ASan catches it if the block is too small
        CHECK_EQ(big[9999], 'x');
    }
    arena_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_strdup) {
    Arena a;
    arena_init(&a, 64);
    const char *words[] = {"select", "from", "where", "a-much-longer-string-than-one-block-of-64-bytes-can-hold-ok"};
    char *copies[4];
    for (int i = 0; i < 4; i++) copies[i] = arena_strdup(&a, words[i]);
    for (int i = 0; i < 4; i++) {
        CHECK_STR(copies[i], words[i]);
        CHECK(copies[i] != words[i]);
    }
    arena_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_reset_keeps_one_block) {
    Arena a;
    arena_init(&a, 128);
    for (int i = 0; i < 100; i++) arena_alloc(&a, 50, 8);
    CHECK(arena_block_count(&a) > 10);
    arena_reset(&a);
    CHECK_EQ(arena_block_count(&a), 1);
    CHECK_EQ(ta_live_allocs() - lc_live_at_start, 1);
    size_t before = ta_total_allocs();
    arena_alloc(&a, 50, 8); // fits in the kept block
    CHECK_EQ(ta_total_allocs() - before, 0);
    arena_free(&a);
    CHECK_EQ(arena_block_count(&a), 0);
    CHECK_NO_LEAKS();
}

int main(void) {
    RUN(test_lazy_init);
    RUN(test_alignment);
    RUN(test_few_mallocs);
    RUN(test_no_overlap);
    RUN(test_big_allocation);
    RUN(test_strdup);
    RUN(test_reset_keeps_one_block);
    TEST_SUMMARY();
}
