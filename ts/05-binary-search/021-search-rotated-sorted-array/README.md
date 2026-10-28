# 021 · Search in Rotated Sorted Array

> **Difficulty:** Medium · **Topics:** binary search · **Target:** O(log n)

A sorted array of **distinct** values was rotated at an unknown pivot
(`[0,1,2,4,5,6,7]` -> `[4,5,6,7,0,1,2]`). Implement:

- `findMin(nums)`: the minimum element.
- `searchRotated(nums, target)`: index of `target`, or `-1`.

## Think about

- At any `mid`, at least one half `[lo, mid]` or `[mid, hi]` is sorted. Which one, and is
  the target inside it?
- `findMin`: compare `nums[mid]` with `nums[hi]` — not with `nums[lo]`. Why?
