Serialize pre-order: emit `val`, then left subtree, then right subtree; `#` for null.
---
Deserialize with an index into the split tokens: read one token; if `#` return null; else build
the node, then recursively build left, then right.
---
For deep trees, BFS (level order with nulls) avoids recursion entirely in both directions.
