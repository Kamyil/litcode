Stack of indices. For each day `i`: while `temps[i] > temps[stack.top]`, pop `j` and set `out[j] = i - j`.
---
Then push `i`. Each index is pushed and popped at most once -> O(n).
