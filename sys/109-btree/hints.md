Write two helpers first: `lower_bound(keys, n, k)` (first index with keys[i] >= k) and
`upper_bound(keys, n, k)` (first index with keys[i] > k). Descend internals with `upper_bound`.
---
Give each node array room for one extra key (`max_keys + 1`) and child (`max_keys + 2`), so you can
insert first and split after — much simpler than splitting "around" the new key.
---
Recursive insert returns "did I split?" plus the separator and the new right node. The caller
inserts that separator at `i` and the right child at `i + 1`, then checks for its own overflow.
---
Leaf split at `mid = n / 2`: right gets `keys[mid..n)`, separator = right's first key,
`right->next = left->next; left->next = right`.
Internal split: separator = `keys[mid]` moves up; right gets `keys[mid+1..n)` and `kids[mid+1..n]`.
---
Range scan: descend to the leaf where `lo` would live, then loop: emit keys in `[lo, hi]`, stop at the first
key > hi, follow `next`. Leaves emptied by deletes simply contribute nothing.
---
Rust: recursion like `fn insert_at(&mut self, id: usize, k, v) -> (Option<i64>, Option<(i64, usize)>)`.
Read the child id out of `self.nodes[id]` before recursing so the borrow ends; push new nodes with
`self.nodes.push(..)` and use `self.nodes.len() - 1` as the id.
