# 053 · Implement a Trie (Prefix Tree)

> **Difficulty:** Medium · **Topics:** trie · **Target:** O(length) per operation

Implement `Trie` with `insert(word)`, `search(word)` (exact match), `startsWith(prefix)`,
and `wordsWithPrefix(prefix)` returning all inserted words with that prefix in
**lexicographic order** (autocomplete!).

## Think about

- Node = `{ children: Map<string, Node>, end: boolean }`. A 26-slot array is faster for a
  fixed alphabet but wastes memory on sparse nodes. (You'll feel this trade-off for real in
  C in sys/107.)
- Database angle: a B-tree index on `text` answers `LIKE 'abc%'` (with `text_pattern_ops` or
  C collation) as a range scan — a prefix search. `LIKE '%abc%'` needs trigrams instead (pg_trgm).
  Postgres also has `SP-GiST`, whose built-in `text_ops` is literally a radix tree (compressed trie).
