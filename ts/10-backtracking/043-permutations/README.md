# 043 · Permutations

> **Difficulty:** Medium · **Topics:** backtracking, swapping · **Target:** O(n · n!)

- `permute(nums)`: all permutations of distinct integers.
- `permuteUnique(nums)`: `nums` may contain duplicates; return only unique permutations.
- `nextPermutation(nums)`: rearrange `nums` **in place** into the next lexicographically
  greater permutation (wrap around to the smallest if it's the last). Returns nothing.

## Think about

- Backtracking with a `used[]` array, or in-place swapping (no extra array). 
- `nextPermutation` is a classic "know the algorithm" question (it's how C++'s
  `std::next_permutation` works): find the rightmost ascent, swap with the next-larger
  element to its right, reverse the suffix.
