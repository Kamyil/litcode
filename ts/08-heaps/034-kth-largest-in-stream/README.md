# 034 · Kth Largest Element (array and stream)

> **Difficulty:** Medium · **Topics:** heap, quickselect · **Target:** O(n log k)

- `findKthLargest(nums, k)`: the k-th largest element (k = 1 is the max).
- `class KthLargest(k, nums)` with `add(val)` returning the current k-th largest after adding.

Import your heap: `import { Heap } from "../033-binary-heap/solution";`

## Think about

- Keep a **min-heap of size k**: its top is the k-th largest. Why min and not max?
- Quickselect solves `findKthLargest` in O(n) average without a heap (you'll write it in
  the C track's sorting challenge as well). What's its worst case and how do you avoid it?
