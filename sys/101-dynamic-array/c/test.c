#include "dynarray.h"
#include "test.h"

TEST(test_init_empty) {
    DynArray a;
    CHECK(da_init(&a, sizeof(int), 0));
    CHECK_EQ(a.len, 0);
    CHECK_EQ(a.cap, 0);
    CHECK(da_get(&a, 0) == NULL);
    int x;
    CHECK(!da_pop(&a, &x));
    da_free(&a);
    da_free(&a); // must be safe
    CHECK_NO_LEAKS();
}

TEST(test_push_get_pop) {
    DynArray a;
    CHECK(da_init(&a, sizeof(int), 0));
    for (int i = 0; i < 100; i++) CHECK(da_push(&a, &i));
    CHECK_EQ(a.len, 100);
    for (int i = 0; i < 100; i++) {
        int *p = da_get(&a, (size_t)i);
        CHECK(p != NULL);
        if (p) CHECK_EQ(*p, i);
    }
    CHECK(da_get(&a, 100) == NULL);
    int out = -1;
    CHECK(da_pop(&a, &out));
    CHECK_EQ(out, 99);
    CHECK(da_pop(&a, NULL));
    CHECK_EQ(a.len, 98);
    da_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_growth_is_geometric) {
    DynArray a;
    CHECK(da_init(&a, sizeof(long), 0));
    size_t before = ta_total_allocs();
    for (long i = 0; i < 1000000; i++) da_push(&a, &i);
    size_t allocs = ta_total_allocs() - before;
    CHECK(allocs <= 25); // doubling from 4 -> ~18 reallocations
    CHECK(a.cap >= a.len && a.cap < 2 * a.len + 4);
    long *last = da_get(&a, 999999);
    CHECK(last && *last == 999999);
    da_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_initial_capacity_and_reserve) {
    DynArray a;
    CHECK(da_init(&a, sizeof(int), 10));
    CHECK_EQ(a.cap, 10);
    size_t before = ta_total_allocs();
    for (int i = 0; i < 10; i++) da_push(&a, &i);
    CHECK_EQ(ta_total_allocs() - before, 0); // no growth needed
    CHECK(da_reserve(&a, 100));
    CHECK(a.cap >= 100);
    CHECK(da_reserve(&a, 5)); // never shrinks
    CHECK(a.cap >= 100);
    da_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_insert_remove) {
    DynArray a;
    da_init(&a, sizeof(int), 0);
    int vals[] = {1, 2, 4, 5};
    for (int i = 0; i < 4; i++) da_push(&a, &vals[i]);
    int three = 3, zero = 0, six = 6;
    CHECK(da_insert(&a, 2, &three)); // 1 2 3 4 5
    CHECK(da_insert(&a, 0, &zero));  // 0 1 2 3 4 5
    CHECK(da_insert(&a, 6, &six));   // 0 1 2 3 4 5 6
    CHECK(!da_insert(&a, 99, &six)); // out of range
    CHECK_EQ(a.len, 7);
    for (int i = 0; i < 7; i++) CHECK_EQ(*(int *)da_get(&a, (size_t)i), i);
    int out;
    CHECK(da_remove(&a, 0, &out));
    CHECK_EQ(out, 0);
    CHECK(da_remove(&a, 2, &out)); // 1 2 4 5 6
    CHECK_EQ(out, 3);
    CHECK(!da_remove(&a, 5, &out));
    int want[] = {1, 2, 4, 5, 6};
    for (int i = 0; i < 5; i++) CHECK_EQ(*(int *)da_get(&a, (size_t)i), want[i]);
    da_free(&a);
    CHECK_NO_LEAKS();
}

typedef struct {
    char name[24];
    double score;
} Record;

TEST(test_struct_elements) {
    DynArray a;
    da_init(&a, sizeof(Record), 0);
    for (int i = 0; i < 50; i++) {
        Record r = {.score = i * 1.5};
        snprintf(r.name, sizeof r.name, "rec-%d", i);
        da_push(&a, &r);
    }
    Record *r = da_get(&a, 42);
    CHECK(r != NULL);
    if (r) {
        CHECK_STR(r->name, "rec-42");
        CHECK(r->score == 63.0);
    }
    da_free(&a);
    CHECK_NO_LEAKS();
}

TEST(test_shrink_to_fit) {
    DynArray a;
    da_init(&a, sizeof(int), 0);
    for (int i = 0; i < 100; i++) da_push(&a, &i);
    for (int i = 0; i < 90; i++) da_pop(&a, NULL);
    da_shrink_to_fit(&a);
    CHECK_EQ(a.cap, 10);
    CHECK_EQ(*(int *)da_get(&a, 9), 9);
    while (a.len) da_pop(&a, NULL);
    da_shrink_to_fit(&a);
    CHECK_EQ(a.cap, 0);
    CHECK(a.data == NULL);
    CHECK_NO_LEAKS(); // shrink to 0 must free the buffer
    da_free(&a);
}

int main(void) {
    RUN(test_init_empty);
    RUN(test_push_get_pop);
    RUN(test_growth_is_geometric);
    RUN(test_initial_capacity_and_reserve);
    RUN(test_insert_remove);
    RUN(test_struct_elements);
    RUN(test_shrink_to_fit);
    TEST_SUMMARY();
}
