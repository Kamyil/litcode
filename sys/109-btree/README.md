# 109 · B+Tree Index

> **Difficulty:** Hard · **Languages:** C + Rust · **Topics:** node splits, fan-out, leaf chains, range scans, linked structures in Rust

The default Postgres index (`CREATE INDEX` = `USING btree`) is a **B+tree** (Lehman & Yao's
variant, `src/backend/access/nbtree/` — the `README` there is one of the best docs in the
codebase). You'll build an in-memory one with `int64` keys and values:

- **Internal nodes** hold up to `max_keys` separator keys and `max_keys + 1` children.
- **Leaves** hold up to `max_keys` (key, value) pairs, sorted, and a `next` pointer to the
  right sibling — so a range scan descends **once** and then walks leaves sideways.
- Insert into the leaf; when a node overflows (`max_keys + 1` keys), **split** it in half and
  push a separator up. A leaf split *copies* the right half's first key up; an internal split
  *moves* the middle key up. When the root splits, the tree grows one level taller (the only way height grows).
- Convention: keys `>= separator[i]` go to child `i + 1` (use "upper bound" when descending).
- **Delete** just removes the key from its leaf — **no merging/rebalancing**. That's not
  laziness, it's what nbtree does: half-empty pages stay until VACUUM can delete entirely
  empty ones. (Merging needs locks across siblings; databases avoid it.)

## Part 1 — C (`c/btree.c`)

Opaque `BTree`, API in `btree.h`. `bt_range` writes matching pairs (inclusive `[lo, hi]`,
ascending) into caller arrays, up to `max` of them, and returns the count.

## Part 2 — Rust (`rust/src/lib.rs`)

Same semantics. The leaf `next` pointer makes `Box` ownership awkward (who owns a leaf — its
parent or its left sibling?), so store nodes in a `Vec<Node>` arena and link them by `usize`
id, like 106. The scaffold suggests an `enum Node { Leaf {..}, Internal {..} }`.

## Interview connection

- Height = O(log_fanout N). Postgres pages are 8 KB; with 16-byte index tuples that's ~400
  keys per page, so a **billion-row index is 4 levels deep**. 3 of those levels are usually
  cached; one random read per lookup. That's why B-trees win on disk.
- Why does a B-tree support `<`, `BETWEEN`, `ORDER BY`, `LIKE 'abc%'` but a hash index doesn't?
  (Sorted leaves + leaf chain = your `bt_range`.)
- Composite index `(a, b)`: keys are compared lexicographically. Why can't it serve `WHERE b = 5`
  efficiently? (No contiguous key range — the "leftmost prefix" rule.)
- Inserting monotonically increasing keys (serial IDs, timestamps) always hits the rightmost leaf;
  with half/half splits, every leaf ends up 50% full. Postgres special-cases this: a rightmost
  split leaves the left page ~90% full (`fillfactor`). Random UUIDv4 keys scatter inserts and
  bloat the index — one reason UUIDv7 (time-ordered) exists.
