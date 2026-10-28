# 033 · Implement a Binary Heap

> **Difficulty:** Medium · **Topics:** heap, array-backed tree · **Target:** O(log n) push/pop, O(n) heapify

JavaScript has no priority queue in its standard library, so you'll need your own for
interviews. Build a generic min-heap with a comparator:

```ts
const h = new Heap<number>((a, b) => a - b);   // min-heap
const m = new Heap<number>((a, b) => b - a);   // max-heap
```

API: `push(x)`, `pop()` (returns `undefined` when empty), `peek()`, `size`, and a static
`Heap.from(items, cmp)` that builds a heap in **O(n)** (bottom-up heapify, not n pushes).

The next three problems import this heap — once it passes, reuse it.

## Think about

- Array layout: children of `i` are `2i+1` and `2i+2`, parent is `(i-1) >> 1`. No pointers,
  great cache locality. Same trick in C/Rust (sys/104).
- Why is bottom-up heapify O(n) and not O(n log n)? (Most nodes are near the bottom and sift
  down only a level or two; the sum converges.)
- Postgres uses a binary heap for merge appends, top-N sorts (`LIMIT` + `ORDER BY`), and
  merging sorted runs in external sorts: `src/backend/lib/binaryheap.c`.
