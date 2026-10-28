# 106 · LRU Cache (hash table + intrusive doubly linked list)

> **Difficulty:** Medium · **Languages:** C + Rust · **Topics:** opaque types, intrusive lists, chaining, linked structures in Rust

You did this in TypeScript (027). In C there's no `Map` to lean on; in Rust, doubly linked
lists fight the borrow checker. This challenge is mostly about **those two experiences**.

## Part 1 — C (`c/lru.c`)

`LruCache` is an **opaque type**: the header only declares `typedef struct LruCache LruCache;`
and you define the struct in `lru.c`. Callers can't touch the internals — the standard C way
to get encapsulation (Postgres does this everywhere, e.g. `Relation`, `Tuplesortstate`).

Keys and values are `int64_t`. Build:
- a **chained hash table** of buckets (power-of-two count, ~1 bucket per entry), and
- an **intrusive** doubly linked list: each node is *both* a hash-chain entry and a list
  element (`hnext`, `prev`, `next` pointers in the same struct, one `malloc` per entry).

`lru_put` returns `true` and sets `*evicted_key` when it evicted something.

## Part 2 — Rust (`rust/src/lib.rs`)

Generic `LruCache<K: Hash + Eq + Clone, V>`. You may use `std::collections::HashMap`.
For the list, use an **index-based arena**: nodes live in a `Vec<Node<K, V>>` and link to each
other by `usize` index instead of pointers (freed slots go on a free list for reuse).

API: `new(capacity)`, `get(&K) -> Option<&V>` (marks as used), `put(K, V) -> Option<(K, V)>`
(returns the evicted entry), `len`, `keys_mru()` (keys from most to least recently used — for tests).

## Compare when done

There are three ways to build a doubly linked list in Rust. Think through each:
1. `Rc<RefCell<Node>>` with `Weak` back-pointers — safe, but refcount + runtime borrow checks per step.
2. Raw pointers (`*mut Node`) in `unsafe` — what `std::collections::LinkedList` does; C-like.
3. Indices into a `Vec` (what you built) — safe, cache-friendly, but a stale index is a logic bug
   the compiler can't catch (the Rust version of a dangling pointer, minus the UB).

Required reading after: *Learning Rust With Entirely Too Many Linked Lists* (rust-unofficial.github.io/too-many-lists).

Database angle: Postgres's buffer manager is a hash table (`buf_table.c`, tag → buffer id) plus
a replacement policy, but it uses **clock-sweep**, not LRU. Why? A strict LRU list needs a global
lock on every buffer *hit* to move the node to the front; clock-sweep only bumps a usage counter
with an atomic op. Contention matters more than precision.
