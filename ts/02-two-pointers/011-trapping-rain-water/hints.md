Precompute `maxLeft[i]` (max of height[0..i]) and `maxRight[i]` (max of height[i..n-1]).
---
Two pointers: keep `leftMax` and `rightMax`. If `leftMax < rightMax`, the water at `l` is
determined by `leftMax` alone (the right side is guaranteed to be at least as tall). Process `l`, move it.
