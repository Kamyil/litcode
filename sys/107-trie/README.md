# 107 · Trie with Deletion & Pruning

> **Difficulty:** Medium · **Languages:** C + Rust · **Topics:** recursive ownership, freeing trees, callbacks, memory accounting

A 26-way trie over lowercase ASCII. You built one in TS (053) with maps; now with fixed child
arrays, where every node costs real bytes and deleting a word must **free the nodes it no
longer needs**.

## Part 1 — C (`c/trie.c`)

- `trie_insert`: returns `false` for characters outside `a-z` (and inserts nothing) or on OOM.
  Inserting an existing word is a no-op that returns `true`.
- `trie_contains`, `trie_count_prefix` (how many stored words start with `prefix`; keep a
  per-node counter so this is O(len(prefix)) — not a subtree walk).
- `trie_remove`: unmark the word and **free every node that no longer leads to any word**.
- `trie_collect(prefix, cb, ctx)`: call `cb(word, ctx)` for every word with that prefix in
  lexicographic order; return the count. (Callbacks + `void *ctx` = C's closures.)
- `t->nodes` must always equal the number of allocated nodes (the test checks it after removals).

## Part 2 — Rust (`rust/src/lib.rs`)

Same semantics with `children: [Option<Box<Node>>; 26]`: `insert -> bool`, `contains`,
`count_prefix`, `remove -> bool`, `words_with_prefix -> Vec<String>`, `node_count`, `len`.

## Compare when done

- Freeing: in C, `trie_free` is a post-order walk you write by hand. In Rust, dropping the root
  `Box` frees the whole tree — but **recursively**. Insert one 1,000,000-letter word and drop the
  trie: stack overflow. Real code (`std::collections::LinkedList`, `serde_json::Value`) writes a
  manual iterative `Drop`. Try it if you want to see it happen.
- Memory: `sizeof(TrieNode)` is ~216 bytes on 64-bit. 100k English words create ~250k nodes =
  ~50 MB. How would you shrink it? (sorted child arrays, a bitmap + popcount index (HAMT),
  radix compression of single-child chains — that last one is exactly Postgres SP-GiST's
  `text_ops` radix tree.)
- Remove in C: what does the recursion return so the parent knows to free a child?
