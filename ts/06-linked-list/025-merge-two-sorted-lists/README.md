# 025 · Merge Two Sorted Lists

> **Difficulty:** Easy · **Topics:** linked list, merge · **Target:** O(m + n), O(1) extra

Merge two sorted linked lists into one sorted list by **splicing nodes** (don't allocate new
nodes, except a dummy head). Return the head.

## Examples

```
[1,2,4] + [1,3,4] -> [1,1,2,3,4,4]
[]      + [0]     -> [0]
```

## Think about

- The merge step is the heart of merge sort (sys/110 external merge sort, and Postgres's
  Merge Join, which walks two sorted inputs exactly like this).
- The dummy-head trick removes every "is the result empty yet?" branch.
