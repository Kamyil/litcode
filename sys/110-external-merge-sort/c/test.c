#include <dirent.h>
#include <stdint.h>
#include <sys/stat.h>

#include "test.h"
#include "xsort.h"

static char base[64];
static char in_path[128], out_path[128], tmp_dir[128];

static void setup(void) {
    snprintf(base, sizeof base, "/tmp/lc-xsort-XXXXXX");
    if (!mkdtemp(base)) { perror("mkdtemp"); exit(2); }
    snprintf(in_path, sizeof in_path, "%s/input.bin", base);
    snprintf(out_path, sizeof out_path, "%s/output.bin", base);
    snprintf(tmp_dir, sizeof tmp_dir, "%s/runs", base);
    mkdir(tmp_dir, 0700);
}

static int count_files(const char *dir) {
    DIR *d = opendir(dir);
    if (!d) return -1;
    int n = 0;
    struct dirent *e;
    while ((e = readdir(d)))
        if (e->d_name[0] != '.') n++;
    closedir(d);
    return n;
}

static void teardown(void) {
    DIR *d = opendir(tmp_dir);
    struct dirent *e;
    char p[256];
    while (d && (e = readdir(d))) {
        if (e->d_name[0] == '.') continue;
        snprintf(p, sizeof p, "%s/%s", tmp_dir, e->d_name);
        remove(p);
    }
    if (d) closedir(d);
    rmdir(tmp_dir);
    remove(in_path);
    remove(out_path);
    rmdir(base);
}

static uint64_t rng = 0x9E3779B97F4A7C15ULL;
static int64_t next_rand(void) {
    rng ^= rng << 13;
    rng ^= rng >> 7;
    rng ^= rng << 17;
    return (int64_t)rng;
}

// Writes n values; returns a order-independent checksum.
static uint64_t write_input(size_t n, int64_t mod) {
    FILE *f = fopen(in_path, "wb");
    uint64_t sum = 0;
    for (size_t i = 0; i < n; i++) {
        int64_t x = mod ? next_rand() % mod : next_rand();
        sum += (uint64_t)x * 2654435761u;
        fwrite(&x, sizeof x, 1, f);
    }
    fclose(f);
    return sum;
}

// Checks the output is sorted, has n values and the same checksum.
static bool output_ok(size_t n, uint64_t sum) {
    FILE *f = fopen(out_path, "rb");
    if (!f) return false;
    int64_t prev = INT64_MIN, x;
    size_t count = 0;
    uint64_t s = 0;
    bool sorted = true;
    while (fread(&x, sizeof x, 1, f) == 1) {
        if (x < prev) sorted = false;
        prev = x;
        s += (uint64_t)x * 2654435761u;
        count++;
    }
    fclose(f);
    if (!sorted) fprintf(stderr, "    output is not sorted\n");
    if (count != n) fprintf(stderr, "    output has %zu values, expected %zu\n", count, n);
    return sorted && count == n && s == sum;
}

TEST(test_empty_input) {
    setup();
    write_input(0, 0);
    CHECK_EQ(external_sort(in_path, out_path, tmp_dir, 100), 0);
    CHECK(output_ok(0, 0));
    teardown();
    CHECK_NO_LEAKS();
}

TEST(test_fits_in_memory) {
    setup();
    uint64_t sum = write_input(500, 1000);
    CHECK_EQ(external_sort(in_path, out_path, tmp_dir, 1000), 1);
    CHECK(output_ok(500, sum));
    CHECK_EQ(count_files(tmp_dir), 0);
    teardown();
    CHECK_NO_LEAKS();
}

TEST(test_many_runs_with_duplicates) {
    setup();
    size_t n = 100000;
    uint64_t sum = write_input(n, 50); // lots of equal values
    CHECK_EQ(external_sort(in_path, out_path, tmp_dir, 7000), 15); // ceil(100000 / 7000)
    CHECK(output_ok(n, sum));
    CHECK_EQ(count_files(tmp_dir), 0);
    teardown();
    CHECK_NO_LEAKS();
}

TEST(test_bounded_memory) {
    setup();
    size_t n = 300000, mem = 10000;
    uint64_t sum = write_input(n, 0);
    ta_reset_peak();
    size_t base_bytes = ta_live_bytes();
    long runs = external_sort(in_path, out_path, tmp_dir, mem);
    size_t peak = ta_peak_bytes() - base_bytes;
    CHECK_EQ(runs, 30);
    CHECK(output_ok(n, sum));
    size_t budget = mem * sizeof(int64_t) + (size_t)runs * 32 + 1024;
    if (peak > budget) fprintf(stderr, "    peak heap %zu bytes > budget %zu\n", peak, budget);
    CHECK(peak <= budget);
    CHECK(peak >= mem * sizeof(int64_t) / 2); // you did use the memory you were given
    CHECK_EQ(count_files(tmp_dir), 0);
    teardown();
    CHECK_NO_LEAKS();
}

TEST(test_missing_input) {
    setup();
    CHECK_EQ(external_sort("/nonexistent/file.bin", out_path, tmp_dir, 10), -1);
    teardown();
    CHECK_NO_LEAKS();
}

int main(void) {
    RUN(test_empty_input);
    RUN(test_fits_in_memory);
    RUN(test_many_runs_with_duplicates);
    RUN(test_bounded_memory);
    RUN(test_missing_input);
    TEST_SUMMARY();
}
