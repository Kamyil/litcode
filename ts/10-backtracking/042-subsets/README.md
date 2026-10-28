# 042 · Subsets (with and without duplicates)

> **Difficulty:** Medium · **Topics:** backtracking, bitmasks · **Target:** O(n · 2ⁿ)

- `subsets(nums)`: all subsets of distinct integers (the power set).
- `subsetsWithDup(nums)`: `nums` may contain duplicates; no duplicate subsets in the result.

Order doesn't matter (tests normalize).

## Think about

- Three ways: backtracking (include/exclude), iterative (start with `[[]]`, extend each
  subset with each number), and bitmasks (`for mask in 0..2ⁿ`). Write at least two.
- Duplicates: sort first, then at each recursion level skip a value equal to the previous
  one *at the same level*. This "skip duplicates at the same depth" trick reappears in
  permutations and combination sum.
