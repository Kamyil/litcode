# 103 · Hash Map (open addressing)

> **Difficulty:** Medium · **Languages:** C + Rust · **Topics:** hashing, linear probing, tombstones, load factor, ownership of keys

The most important data structure in practice. You'll build an **open-addressing** table
(all entries live in one flat array, collisions probe to the next slot) — the design used by
Rust's `HashMap` (SwissTable), Python's `dict`, and Postgres's `simplehash.h`
(`src/include/lib/simplehash.h`, used by hash aggregation and hash joins).

Rules for both languages:
- Capacity is a **power of two**; slot = `hash & (cap - 1)`.
- **Linear probing**: on collision try `i+1, i+2, ...` (wrapping).
- Deleting leaves a **tombstone** (slot is "deleted", probes continue past it; inserts may reuse it).
- Grow (double) when `(len + tombstones) / cap > 0.7`. Growing rehashes everything and drops tombstones.
- Hash: **FNV-1a 64-bit** (`hash = 0xcbf29ce484222325; for each byte: hash ^= b; hash *= 0x100000001b3`).

## Part 1 — C (`c/hashmap.c`)

String keys → `int64_t` values. The map **owns copies of keys** (`strdup` on insert, `free` on
remove / overwrite-never-reallocates / `hm_free`). See `hashmap.h` for the API, including a
simple iterator.

## Part 2 — Rust (`rust/src/lib.rs`)

Generic `HashMap<K: Hash + Eq, V>`, slots as an enum:

```rust
enum Slot<K, V> { Empty, Tombstone, Full(K, V) }
```

Safe Rust, no `unsafe` needed. Implement `fnv1a`, an `FnvHasher` (`impl std::hash::Hasher`),
and the map: `new`, `insert -> Option<V>` (returns the old value), `get`, `get_mut`,
`remove -> Option<V>`, `len`, `capacity`, `iter`. `get`/`remove` take `&Q where K: Borrow<Q>`
so a `HashMap<String, _>` can be queried with `&str` — exactly like std.

## Compare when done

- In C, who frees the key on overwrite? On remove? Forgetting one is a leak, doing both a double
  free. In Rust, `Slot::Full(K, V)` *owns* both; replacing a slot drops the old one automatically.
- C needs a separate `state` byte (or a sentinel key pointer) for tombstones. Rust's enum
  carries the tag for you — what does `size_of::<Slot<String, i64>>()` print? (Niche optimization!)
- Why does linear probing beat chaining on modern CPUs despite longer probe sequences? (cache lines)
- Why does the load factor count tombstones? Try removing that and run `tombstone_churn`.
- Postgres: hash indexes (`CREATE INDEX ... USING hash`) only support `=`. Why can't a hash
  structure answer `WHERE k > 10` or `ORDER BY k`?
