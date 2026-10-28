# 011 · Trapping Rain Water

> **Difficulty:** Hard · **Topics:** two pointers, prefix max · **Target:** O(n) time, O(1) space

Given bar heights, compute how much rain water is trapped between them.

```
height = [0,1,0,2,1,0,1,3,2,1,2,1]  ->  6

        #
    #   ## #
  # ## ######
```

## Think about

- Water above bar `i` = `min(maxLeft[i], maxRight[i]) - height[i]`.
- Version 1: two prefix arrays (O(n) space). Version 2: two pointers (O(1) space).
  Write both; the jump from one to the other is the insight interviewers look for.
- A third approach uses a monotonic stack (see 018/019). Try it after the stack section.
