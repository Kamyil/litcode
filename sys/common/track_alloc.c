// Implementation of the allocation tracker. Every block gets a 16-byte header
// (keeps the user pointer 16-byte aligned, like real malloc) holding the size and
// a magic value we use to detect double frees and invalid frees.
#undef malloc
#undef calloc
#undef realloc
#undef free
#undef strdup

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TA_ALIVE 0xA110CA7EDULL
#define TA_DEAD 0xDEADBEEFULL

typedef struct {
    size_t size;
    uint64_t magic;
} ta_header;

_Static_assert(sizeof(ta_header) == 16, "header must keep 16-byte alignment");

static size_t live_allocs, live_bytes, total_allocs, peak_bytes;

size_t ta_live_allocs(void) { return live_allocs; }
size_t ta_live_bytes(void) { return live_bytes; }
size_t ta_total_allocs(void) { return total_allocs; }
size_t ta_peak_bytes(void) { return peak_bytes; }
void ta_reset_peak(void) { peak_bytes = live_bytes; }

void *ta_malloc(size_t size, const char *file, int line) {
    (void)file;
    (void)line;
    ta_header *h = malloc(sizeof *h + size);
    if (!h) return NULL;
    h->size = size;
    h->magic = TA_ALIVE;
    live_allocs++;
    live_bytes += size;
    if (live_bytes > peak_bytes) peak_bytes = live_bytes;
    total_allocs++;
    return h + 1;
}

void *ta_calloc(size_t n, size_t size, const char *file, int line) {
    if (size && n > SIZE_MAX / size) return NULL;
    void *p = ta_malloc(n * size, file, line);
    if (p) memset(p, 0, n * size);
    return p;
}

void ta_free(void *ptr, const char *file, int line) {
    if (!ptr) return;
    ta_header *h = (ta_header *)ptr - 1;
    if (h->magic == TA_DEAD) {
        fprintf(stderr, "\n  !! double free at %s:%d\n", file, line);
        abort();
    }
    if (h->magic != TA_ALIVE) {
        fprintf(stderr, "\n  !! free() of a pointer malloc never returned at %s:%d\n", file, line);
        abort();
    }
    h->magic = TA_DEAD;
    live_allocs--;
    live_bytes -= h->size;
    free(h);
}

void *ta_realloc(void *ptr, size_t size, const char *file, int line) {
    if (!ptr) return ta_malloc(size, file, line);
    if (size == 0) {
        ta_free(ptr, file, line);
        return NULL;
    }
    ta_header *old = (ta_header *)ptr - 1;
    if (old->magic != TA_ALIVE) {
        fprintf(stderr, "\n  !! realloc() of an invalid pointer at %s:%d\n", file, line);
        abort();
    }
    void *fresh = ta_malloc(size, file, line);
    if (!fresh) return NULL;
    memcpy(fresh, ptr, old->size < size ? old->size : size);
    ta_free(ptr, file, line);
    total_allocs--; // a realloc counts as one allocation, not two
    return fresh;
}

char *ta_strdup(const char *s, const char *file, int line) {
    size_t n = strlen(s) + 1;
    char *p = ta_malloc(n, file, line);
    if (p) memcpy(p, s, n);
    return p;
}
