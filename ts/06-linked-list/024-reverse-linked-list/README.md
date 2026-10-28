# 024 · Reverse Linked List

> **Difficulty:** Easy · **Topics:** linked list, pointer manipulation · **Target:** O(n), O(1) space

Reverse a singly linked list, **iteratively** (`reverseList`) and **recursively**
(`reverseListRecursive`). Then `reverseBetween(head, left, right)`: reverse only the nodes at
1-based positions `left..right`, in one pass.

`ListNode`, `listFrom` and `listToArray` live in `ts/lib/list.ts`.

## Examples

```
[1,2,3,4,5]                -> [5,4,3,2,1]
reverseBetween([1,2,3,4,5], 2, 4) -> [1,4,3,2,5]
```

## Think about

- Draw the three pointers `prev`, `curr`, `next` on paper. Interviewers watch for that.
- The recursive version uses O(n) **stack** space. With 100k nodes, does it blow the stack in
  Bun? (Try it! The tests use a short list for the recursive version for this reason.)
- You'll do this again in C (pointer-to-pointer tricks) in the sys track. Compare.
