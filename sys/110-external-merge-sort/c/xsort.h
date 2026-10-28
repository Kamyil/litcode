#ifndef XSORT_H
#define XSORT_H

#include <stddef.h>

// Sorts the int64 values in input_path into output_path (ascending), holding at most
// mem_limit values in memory during run generation. Run files go to tmp_dir and are
// removed before returning. Returns the number of runs, or -1 on error.
long external_sort(const char *input_path, const char *output_path, const char *tmp_dir, size_t mem_limit);

#endif
