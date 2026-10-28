# 104 · Binary Heap & Heapsort

> **Difficulty:** Medium · **Languages:** C + Rust · **Topics:** array-backed trees, function pointers vs traits, in-place algorithms

You built a heap in TypeScript (033). Now do it where memory layout is explicit.

## Part 1 — C (`c/heap.c`)

A generic heap like `qsort`: elements of `elem_size` bytes, ordered by a comparator
`int cmp(const void *a, const void *b)` (negative = `a` comes out first). See `heap.h`.

- `heap_push`, `heap_pop`, `heap_peek`, `heap_len`, `heap_free`
- `heap_from_array`: copy `n` items and heapify bottom-up in **O(n)**
- `heap_sort(base, n, elem_size, cmp)`: **in-place** heapsort of a plain array. **Zero heap
  allocations** (the test checks). That means swapping elements of unknown size without a temp
  buffer from `malloc` — swap byte by byte (or via a small stack buffer in chunks).

Sorted order of `heap_sort` = ascending according to `cmp` (so build a *max*-heap inside).

## Part 2 — Rust (`rust/src/lib.rs`)

`BinaryHeap<T: Ord>` — a **max**-heap like `std::collections::BinaryHeap` — backed by `Vec<T>`,
plus `heap_sort<T: Ord>(&mut [T])`. Use `std::cmp::Reverse` in the tests for min-heaps.
`into_sorted_vec` returns ascending order.

## Compare when done

- C: comparator through a function pointer = an indirect call per comparison, and `memcpy` of
  `elem_size` bytes per move. Rust: `T: Ord` is monomorphized and inlined. That's why `qsort` is
  slower than `std::sort` / `slice::sort` — measure it if you're curious.
- `slice.swap(i, j)` in Rust vs your byte swap in C.
- Heapsort is in-place and O(n log n) worst case, but rarely the default sort. Why? (cache-unfriendly
  jumps; not stable). Postgres uses quicksort for in-memory sorts but a **bounded heap** for
  `ORDER BY ... LIMIT n` — look for `Sort Method: top-N heapsort` in `EXPLAIN ANALYZE`.
