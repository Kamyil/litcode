#include "heap.h"
#include "test.h"

static int cmp_int_min(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}
static int cmp_int_max(const void *a, const void *b) { return cmp_int_min(b, a); }

typedef struct {
    int priority;
    char task[20];
} Job;

static int cmp_job(const void *a, const void *b) {
    return cmp_int_min(&((const Job *)a)->priority, &((const Job *)b)->priority);
}

static unsigned rng_state = 12345;
static int rnd(int n) {
    rng_state = rng_state * 1103515245u + 12345u;
    return (int)((rng_state >> 8) % (unsigned)n);
}

TEST(test_empty) {
    Heap h;
    heap_init(&h, sizeof(int), cmp_int_min);
    CHECK_EQ(heap_len(&h), 0);
    CHECK(heap_peek(&h) == NULL);
    CHECK(!heap_pop(&h, NULL));
    heap_free(&h);
    CHECK_NO_LEAKS();
}

TEST(test_min_and_max) {
    int xs[] = {5, 3, 8, 1, 9, 2, 7};
    Heap mn, mx;
    heap_init(&mn, sizeof(int), cmp_int_min);
    heap_init(&mx, sizeof(int), cmp_int_max);
    for (int i = 0; i < 7; i++) {
        heap_push(&mn, &xs[i]);
        heap_push(&mx, &xs[i]);
    }
    CHECK_EQ(*(const int *)heap_peek(&mn), 1);
    CHECK_EQ(*(const int *)heap_peek(&mx), 9);
    int want[] = {1, 2, 3, 5, 7, 8, 9}, out;
    for (int i = 0; i < 7; i++) {
        CHECK(heap_pop(&mn, &out));
        CHECK_EQ(out, want[i]);
    }
    CHECK(heap_pop(&mx, &out));
    CHECK_EQ(out, 9);
    heap_free(&mn);
    heap_free(&mx);
    CHECK_NO_LEAKS();
}

TEST(test_structs) {
    Heap h;
    heap_init(&h, sizeof(Job), cmp_job);
    Job jobs[] = {{3, "write tests"}, {1, "fix prod"}, {2, "review pr"}};
    for (int i = 0; i < 3; i++) heap_push(&h, &jobs[i]);
    Job j;
    heap_pop(&h, &j);
    CHECK_STR(j.task, "fix prod");
    heap_pop(&h, &j);
    CHECK_STR(j.task, "review pr");
    heap_free(&h);
    CHECK_NO_LEAKS();
}

TEST(test_from_array_matches_sorting) {
    enum { N = 20000 };
    static int xs[N], sorted[N];
    for (int i = 0; i < N; i++) sorted[i] = xs[i] = rnd(1000);
    qsort(sorted, N, sizeof(int), cmp_int_min);
    Heap h;
    CHECK(heap_from_array(&h, xs, N, sizeof(int), cmp_int_min));
    CHECK_EQ(heap_len(&h), N);
    int bad = 0, out;
    for (int i = 0; i < N; i++) {
        heap_pop(&h, &out);
        if (out != sorted[i]) bad++;
    }
    CHECK_EQ(bad, 0);
    heap_free(&h);
    CHECK_NO_LEAKS();
}

TEST(test_heap_sort_in_place) {
    enum { N = 50000 };
    static int xs[N], sorted[N];
    for (int i = 0; i < N; i++) sorted[i] = xs[i] = rnd(100000) - 50000;
    qsort(sorted, N, sizeof(int), cmp_int_min);
    size_t before = ta_total_allocs();
    heap_sort(xs, N, sizeof(int), cmp_int_min);
    CHECK_EQ(ta_total_allocs() - before, 0);
    CHECK(memcmp(xs, sorted, sizeof xs) == 0);
    heap_sort(xs, 0, sizeof(int), cmp_int_min); // no-op
    heap_sort(xs, 1, sizeof(int), cmp_int_min);
    CHECK_NO_LEAKS();
}

TEST(test_heap_sort_structs) {
    Job jobs[] = {{5, "e"}, {1, "a"}, {4, "d"}, {2, "b"}, {3, "c"}};
    heap_sort(jobs, 5, sizeof(Job), cmp_job);
    for (int i = 0; i < 5; i++) {
        CHECK_EQ(jobs[i].priority, i + 1);
        CHECK_EQ(jobs[i].task[0], 'a' + i);
    }
}

int main(void) {
    RUN(test_empty);
    RUN(test_min_and_max);
    RUN(test_structs);
    RUN(test_from_array_matches_sorting);
    RUN(test_heap_sort_in_place);
    RUN(test_heap_sort_structs);
    TEST_SUMMARY();
}
