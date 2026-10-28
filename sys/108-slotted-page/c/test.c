#include "page.h"
#include "test.h"

#define ALIGN8(n) (((n) + 7u) & ~7u)

static _Alignas(8) uint8_t page[PAGE_SIZE];

static const PageHeader *hdr(void) { return (const PageHeader *)page; }

// Free space if the page were compacted right now.
static size_t free_after_compact(void) {
    size_t used = 0;
    const ItemId *items = (const ItemId *)(page + sizeof(PageHeader));
    for (int i = 0; i < hdr()->nslots; i++)
        if (items[i].len) used += ALIGN8(items[i].len);
    return PAGE_SIZE - hdr()->lower - used;
}

static bool tuple_is(uint16_t slot, const char *want) {
    uint16_t len = 0;
    const uint8_t *p = page_get(page, slot, &len);
    return p && len == strlen(want) && memcmp(p, want, len) == 0;
}

TEST(test_init) {
    page_init(page);
    CHECK_EQ(hdr()->lower, 8);
    CHECK_EQ(hdr()->upper, PAGE_SIZE);
    CHECK_EQ(hdr()->nslots, 0);
    CHECK_EQ(page_free_space(page), PAGE_SIZE - 8);
    CHECK(page_get(page, 0, NULL) == NULL);
}

TEST(test_insert_layout) {
    page_init(page);
    CHECK_EQ(page_insert(page, "abc", 3), 0);
    CHECK_EQ(hdr()->lower, 12);
    CHECK_EQ(hdr()->upper, PAGE_SIZE - 8); // 3 bytes, aligned to 8
    const ItemId *it = (const ItemId *)(page + 8);
    CHECK_EQ(it[0].offset, PAGE_SIZE - 8);
    CHECK_EQ(it[0].len, 3);
    CHECK(memcmp(page + PAGE_SIZE - 8, "abc", 3) == 0);
    CHECK_EQ(page_insert(page, "hello world", 11), 1);
    CHECK_EQ(it[1].offset, PAGE_SIZE - 8 - 16);
    CHECK(tuple_is(0, "abc"));
    CHECK(tuple_is(1, "hello world"));
    CHECK_EQ(page_free_space(page), PAGE_SIZE - 16 - 8 - 16);
}

TEST(test_alignment) {
    page_init(page);
    char buf[64];
    memset(buf, 'x', sizeof buf);
    for (uint16_t len = 1; len < 60; len++) {
        int s = page_insert(page, buf, len);
        CHECK(s >= 0);
        uint16_t got;
        const uint8_t *p = page_get(page, (uint16_t)s, &got);
        CHECK(p && (p - page) % TUPLE_ALIGN == 0);
    }
}

TEST(test_fill_until_full) {
    page_init(page);
    char tuple[100];
    int n = 0;
    for (;;) {
        memset(tuple, 'A' + n % 26, sizeof tuple);
        if (page_insert(page, tuple, sizeof tuple) < 0) break;
        n++;
    }
    CHECK_EQ(n, (PAGE_SIZE - 8) / (104 + 4)); // 75 tuples
    CHECK_EQ(page_free_space(page), (PAGE_SIZE - 8) % 108);
    CHECK(page_insert(page, "tiny", 4) >= 0); // 84 bytes left: a 4-byte tuple needs 8 + 4
}

TEST(test_delete_and_reuse_with_auto_compaction) {
    page_init(page);
    char tuple[100] = {0};
    for (int i = 0; i < 75; i++) {
        snprintf(tuple, sizeof tuple, "row-%d", i);
        page_insert(page, tuple, sizeof tuple);
    }
    CHECK(page_delete(page, 3));
    CHECK(!page_delete(page, 3));  // already unused
    CHECK(!page_delete(page, 999)); // out of range
    CHECK(page_get(page, 3, NULL) == NULL);
    // contiguous gap (84 bytes) is too small, but the deleted tuple's 104 bytes can be reclaimed
    snprintf(tuple, sizeof tuple, "new-row");
    CHECK_EQ(page_insert(page, tuple, sizeof tuple), 3); // reuses the lowest unused slot
    CHECK_EQ(page_num_slots(page), 75);
    uint16_t len;
    for (int i = 0; i < 75; i++) {
        const uint8_t *p = page_get(page, (uint16_t)i, &len);
        char want[16];
        snprintf(want, sizeof want, i == 3 ? "new-row" : "row-%d", i);
        CHECK(p && len == 100 && strcmp((const char *)p, want) == 0);
    }
}

TEST(test_compact_keeps_slots) {
    page_init(page);
    char buf[32];
    for (int i = 0; i < 40; i++) {
        snprintf(buf, sizeof buf, "tuple number %d", i);
        page_insert(page, buf, (uint16_t)strlen(buf));
    }
    for (int i = 0; i < 40; i += 3) page_delete(page, (uint16_t)i);
    size_t expected = free_after_compact();
    page_compact(page);
    CHECK_EQ(page_free_space(page), expected);
    for (int i = 0; i < 40; i++) {
        snprintf(buf, sizeof buf, "tuple number %d", i);
        if (i % 3 == 0) CHECK(page_get(page, (uint16_t)i, NULL) == NULL);
        else CHECK(tuple_is((uint16_t)i, buf));
    }
}

TEST(test_update) {
    page_init(page);
    page_insert(page, "aaaa", 4);
    page_insert(page, "bbbb", 4);
    uint16_t upper = hdr()->upper;
    CHECK(page_update(page, 0, "AAAAAAA", 7)); // still fits in 8 aligned bytes: in place
    CHECK_EQ(hdr()->upper, upper);
    CHECK(tuple_is(0, "AAAAAAA"));
    CHECK(page_update(page, 0, "a much longer tuple value", 25)); // relocated, same slot
    CHECK(tuple_is(0, "a much longer tuple value"));
    CHECK(tuple_is(1, "bbbb"));
    CHECK(!page_update(page, 7, "x", 1)); // no such slot
    page_delete(page, 1);
    CHECK(!page_update(page, 1, "x", 1)); // unused slot
}

TEST(test_update_too_big_leaves_page_unchanged) {
    page_init(page);
    static char big[4000];
    memset(big, 'z', sizeof big);
    page_insert(page, big, 4000);
    page_insert(page, "keep me", 7);
    static uint8_t before[PAGE_SIZE];
    memcpy(before, page, PAGE_SIZE);
    static char huge[8000];
    CHECK(!page_update(page, 1, huge, sizeof huge));
    CHECK(memcmp(before, page, PAGE_SIZE) == 0);
    // growing slot 1 to 4100 works only if the dead space is reclaimed by compaction
    page_delete(page, 0);
    static char mid[4100];
    memset(mid, 'm', sizeof mid);
    CHECK(page_update(page, 1, mid, sizeof mid));
    uint16_t len;
    const uint8_t *p = page_get(page, 1, &len);
    CHECK(p && len == 4100 && p[4099] == 'm');
}

TEST(test_random_against_model) {
    enum { MAXS = 400 };
    static char model[MAXS][48];
    static int mlen[MAXS];
    page_init(page);
    unsigned s = 1;
    int bad = 0;
    for (int step = 0; step < 20000; step++) {
        s = s * 1103515245u + 12345u;
        int op = (int)((s >> 16) % 3);
        uint16_t n = page_num_slots(page);
        char buf[48];
        int len = (int)((s >> 8) % 40) + 1;
        for (int i = 0; i < len; i++) buf[i] = (char)('a' + (step + i) % 26);
        if (op == 0 || n == 0) {
            int slot = page_insert(page, buf, (uint16_t)len);
            if (slot >= 0 && slot < MAXS) {
                memcpy(model[slot], buf, (size_t)len);
                mlen[slot] = len;
            } else if (slot < 0 && free_after_compact() >= ALIGN8((unsigned)len) + 4) {
                bad++; // refused although it would fit after compaction (and a new slot)
            }
        } else if (op == 1) {
            uint16_t slot = (uint16_t)((s >> 4) % n);
            bool ok = page_delete(page, slot);
            if (ok != (mlen[slot] > 0)) bad++;
            mlen[slot] = 0;
        } else {
            uint16_t slot = (uint16_t)((s >> 4) % n);
            if (page_update(page, slot, buf, (uint16_t)len)) {
                memcpy(model[slot], buf, (size_t)len);
                mlen[slot] = len;
            }
        }
        if (step % 97 == 0) {
            for (int i = 0; i < page_num_slots(page) && i < MAXS; i++) {
                uint16_t got;
                const uint8_t *p = page_get(page, (uint16_t)i, &got);
                if (mlen[i] == 0 ? p != NULL : (!p || got != mlen[i] || memcmp(p, model[i], got) != 0)) bad++;
            }
        }
    }
    CHECK_EQ(bad, 0);
}

int main(void) {
    RUN(test_init);
    RUN(test_insert_layout);
    RUN(test_alignment);
    RUN(test_fill_until_full);
    RUN(test_delete_and_reuse_with_auto_compaction);
    RUN(test_compact_keeps_slots);
    RUN(test_update);
    RUN(test_update_too_big_leaves_page_unchanged);
    RUN(test_random_against_model);
    TEST_SUMMARY();
}
