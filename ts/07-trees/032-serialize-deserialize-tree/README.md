# 032 · Serialize and Deserialize a Binary Tree

> **Difficulty:** Hard · **Topics:** DFS/BFS, encoding · **Target:** O(n)

Write `serialize(root): string` and `deserialize(data: string): TreeNode | null` such that
`deserialize(serialize(t))` reproduces the exact same tree shape and values. The format is
up to you (don't just call `treeToArray` + JSON — design it).

## Think about

- Pre-order with a null marker (`"1,2,#,#,3,4,#,#,5,#,#"`) is the simplest. Why do you need
  the null markers? (Pre-order alone is ambiguous; pre-order + in-order isn't, but only with unique values.)
- How compact can you make it? Binary encodings matter for real systems: Postgres serializes
  plan trees with `nodeToString()` / `stringToNode()` — `src/backend/nodes/outfuncs.c`. Look
  at `EXPLAIN (FORMAT JSON)` vs the stored form in `pg_rewrite.ev_action` for views.
- Your deserializer should be iterative-safe or at least handle 10k-deep trees.
