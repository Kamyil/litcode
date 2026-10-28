Keep a stack of indices with increasing heights.
---
At index `i` (with `heights[n] = 0` as sentinel): while `heights[i] < heights[top]`, pop `j`;
`width = stack.empty ? i : i - stack.top - 1`; area = `heights[j] * width`.
