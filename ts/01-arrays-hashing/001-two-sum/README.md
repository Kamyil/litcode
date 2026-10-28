# 001 · Two Sum

> **Difficulty:** Easy · **Topics:** hash map · **Target:** O(n) time, O(n) space

Given an array of integers `nums` and an integer `target`, return the **indices** of the
two numbers that add up to `target`, as `[i, j]` with `i < j`.

Exactly one valid answer exists. You may not use the same element twice.

## Examples

```
nums = [2, 7, 11, 15], target = 9   ->  [0, 1]
nums = [3, 2, 4],      target = 6   ->  [1, 2]
nums = [3, 3],         target = 6   ->  [0, 1]
```

## Constraints

- `2 <= nums.length <= 10^5`, values can be negative.

## Think about

- The brute force is O(n²). What do you need to *remember* while scanning to make it O(n)?
- Interview follow-up: what if the array is **sorted**? (O(1) extra space — see 009 three-sum.)
