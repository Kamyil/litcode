`tails = []`. For each x: `i = lowerBound(tails, x)`; `tails[i] = x` (this appends if `i === tails.length`).
---
Reconstruction: also keep `tailIdx[k]` = index in `nums` of `tails[k]`, and `parent[i] = tailIdx[k - 1]`.
Walk back from `tailIdx[last]`.
