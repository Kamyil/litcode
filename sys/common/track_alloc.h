// Allocation tracker. This header is force-included (-include) into every C file
// of a challenge, so plain malloc/calloc/realloc/free/strdup calls in YOUR code go
// through counting wrappers. That lets tests catch leaks, double frees and frees of
// pointers malloc never returned -- portable leak checking, even on macOS where
// LeakSanitizer is unavailable.
#ifndef LC_TRACK_ALLOC_H
#define LC_TRACK_ALLOC_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

size_t ta_live_allocs(void);   // allocations not yet freed
size_t ta_live_bytes(void);    // bytes not yet freed
size_t ta_total_allocs(void);  // every malloc/calloc/realloc that returned memory
size_t ta_peak_bytes(void);    // highest ta_live_bytes() since start / last reset
void ta_reset_peak(void);      // set the peak to the current live bytes

void *ta_malloc(size_t size, const char *file, int line);
void *ta_calloc(size_t n, size_t size, const char *file, int line);
void *ta_realloc(void *ptr, size_t size, const char *file, int line);
void ta_free(void *ptr, const char *file, int line);
char *ta_strdup(const char *s, const char *file, int line);

#define malloc(n) ta_malloc((n), __FILE__, __LINE__)
#define calloc(n, s) ta_calloc((n), (s), __FILE__, __LINE__)
#define realloc(p, n) ta_realloc((p), (n), __FILE__, __LINE__)
#define free(p) ta_free((p), __FILE__, __LINE__)
#define strdup(s) ta_strdup((s), __FILE__, __LINE__)

#endif
