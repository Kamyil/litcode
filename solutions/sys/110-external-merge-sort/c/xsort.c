#include "xsort.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int64_t value;
    size_t run;
} HeapItem;

static int cmp_i64(const void *a, const void *b) {
    int64_t x = *(const int64_t *)a, y = *(const int64_t *)b;
    return (x > y) - (x < y);
}

static void run_path(char *buf, size_t size, const char *dir, size_t i) {
    snprintf(buf, size, "%s/run-%zu.bin", dir, i);
}

static void sift_down(HeapItem *h, size_t n, size_t i) {
    for (;;) {
        size_t m = i, l = 2 * i + 1, r = l + 1;
        if (l < n && h[l].value < h[m].value) m = l;
        if (r < n && h[r].value < h[m].value) m = r;
        if (m == i) return;
        HeapItem t = h[i];
        h[i] = h[m];
        h[m] = t;
        i = m;
    }
}

// Phase 1: sorted runs of at most mem_limit values. Returns the run count or -1.
static long make_runs(FILE *in, const char *tmp_dir, size_t mem_limit) {
    int64_t *buf = malloc(mem_limit * sizeof *buf);
    if (!buf) return -1;
    long runs = 0;
    size_t n;
    char path[512];
    while ((n = fread(buf, sizeof *buf, mem_limit, in)) > 0) {
        qsort(buf, n, sizeof *buf, cmp_i64);
        run_path(path, sizeof path, tmp_dir, (size_t)runs);
        FILE *out = fopen(path, "wb");
        if (!out || fwrite(buf, sizeof *buf, n, out) != n) {
            if (out) fclose(out);
            free(buf);
            return -1;
        }
        fclose(out);
        runs++;
    }
    free(buf);
    return runs;
}

// Phase 2: k-way merge of the runs into out.
static int merge_runs(FILE *out, const char *tmp_dir, size_t k) {
    FILE **files = calloc(k, sizeof *files);
    HeapItem *heap = malloc(k * sizeof *heap);
    int rc = -1;
    size_t n = 0;
    char path[512];
    if (!files || !heap) goto done;
    for (size_t i = 0; i < k; i++) {
        run_path(path, sizeof path, tmp_dir, i);
        if (!(files[i] = fopen(path, "rb"))) goto done;
        if (fread(&heap[n].value, sizeof(int64_t), 1, files[i]) == 1) heap[n++].run = i;
    }
    for (size_t i = n / 2; i-- > 0;) sift_down(heap, n, i);
    while (n > 0) {
        fwrite(&heap[0].value, sizeof(int64_t), 1, out);
        if (fread(&heap[0].value, sizeof(int64_t), 1, files[heap[0].run]) != 1) heap[0] = heap[--n];
        sift_down(heap, n, 0);
    }
    rc = 0;
done:
    for (size_t i = 0; files && i < k; i++) {
        if (files[i]) fclose(files[i]);
    }
    free(files);
    free(heap);
    return rc;
}

long external_sort(const char *input_path, const char *output_path, const char *tmp_dir, size_t mem_limit) {
    FILE *in = fopen(input_path, "rb");
    if (!in) return -1;
    long runs = make_runs(in, tmp_dir, mem_limit);
    fclose(in);
    FILE *out = runs >= 0 ? fopen(output_path, "wb") : NULL;
    int rc = out ? merge_runs(out, tmp_dir, (size_t)runs) : -1;
    if (out) fclose(out);
    char path[512];
    for (long i = 0; i < runs; i++) {
        run_path(path, sizeof path, tmp_dir, (size_t)i);
        remove(path);
    }
    return rc == 0 ? runs : -1;
}
