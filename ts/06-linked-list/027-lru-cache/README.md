# 027 · LRU Cache

> **Difficulty:** Medium · **Topics:** hash map + doubly linked list, design · **Target:** O(1) get/put

Design a Least-Recently-Used cache with a fixed capacity:

- `get(key)`: return the value or `-1`. A hit makes the key *most recently used*.
- `put(key, value)`: insert or update; this also makes it most recently used. If the cache
  exceeds capacity, evict the least recently used key.

**Build the doubly linked list yourself.** (JS `Map` preserves insertion order, so
delete+re-set gives a 5-line LRU — fine to know and mention, but the interviewer wants the
real structure. You'll build it again in C and Rust in sys/106.)

## Think about

- Why do you need a *doubly* linked list? (O(1) unlink of an arbitrary node.)
- Sentinel head/tail nodes delete all the null checks.
- Postgres's buffer pool does **not** use LRU — it uses a *clock sweep* with usage counts
  (`src/backend/storage/buffer/freelist.c`). Why might strict LRU be bad for a database?
  (A single sequential scan would flush the whole cache.)
