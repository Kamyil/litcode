`valid(node, lo, hi)`: node must satisfy `lo < node.val < hi`; recurse left with `(lo, node.val)`
and right with `(node.val, hi)`. Start with `(-Infinity, Infinity)`.
---
kthSmallest: iterative in-order with a stack — push all left children, pop, count, go right. Stop at k.
