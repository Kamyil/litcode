C element address: `(char *)h->data + i * h->elem_size`. Write `static void swap_bytes(void *a, void *b, size_t n)`.
---
sift_down(i, n): pick the child that should come out first (`cmp(child, best) < 0`), swap, continue.
Write it once as a function taking `(base, n, elem_size, cmp, i)` and reuse it for both the heap and heap_sort.
---
heap_sort: you want ascending output, so heapify with the *inverted* order (largest first), then
repeatedly swap element 0 with the last unsorted element and sift down within the shrunk range.
In C, inverting means calling `cmp(b, a)` instead of `cmp(a, b)`.
---
Rust: same shape with `self.data.swap(i, j)` and `self.data[a] > self.data[b]`.
`into_sorted_vec`: `let mut v = self.data; heap_sort(&mut v); v`.
