# 105 · Ring Buffer

> **Difficulty:** Easy · **Languages:** C + Rust · **Topics:** circular indexing, power-of-two masking, wrap-around copies, `MaybeUninit`

A fixed-size FIFO that never allocates after creation. Used for I/O buffers, audio, logging,
lock-free queues between threads, and in Postgres: the **WAL buffers** (`wal_buffers`) are a
ring of pages that backends write into and the WAL writer flushes; `pg_stat_statements` and
the shared-memory message queues (`shm_mq.c`) use the same idea.

**The trick to learn:** keep two ever-increasing counters, `head` (total read) and `tail`
(total written). Then `len = tail - head`, `empty = len == 0`, `full = len == cap` — no
wasted slot, no ambiguous "head == tail" state. The physical index is `counter & (cap - 1)`,
which is why capacity must be a power of two. Unsigned overflow wraps, and the subtraction
still works. (This is exactly how Linux's `kfifo` works.)

## Part 1 — C (`c/ringbuf.c`) — a byte ring

- `rb_init(rb, cap)`: `cap` must be a non-zero power of two, else return `false`.
- `rb_write(rb, src, n)`: copy up to `n` bytes (as many as fit), return how many were written.
  Wrap-around = **at most two `memcpy` calls**, no byte-by-byte loop.
- `rb_read(rb, dst, n)`, `rb_peek(rb, dst, n)` (read without consuming), `rb_len`, `rb_space`.

## Part 2 — Rust (`rust/src/lib.rs`) — a typed ring

`RingBuffer<T>` holding values of any type in `Box<[MaybeUninit<T>]>`:
`with_capacity(cap)` (power of two, panics otherwise), `push(v) -> Result<(), T>` (gives the value
back when full — nothing is lost), `pop() -> Option<T>`, `peek() -> Option<&T>`, `len`,
`capacity`, `is_full`, `iter()` (oldest → newest), and `Drop` (drop only the live elements!).

## Compare when done

- C stores bytes, so "dropping" is meaningless. Rust stores `T`s, so it must track exactly
  which slots are initialized — that's what `MaybeUninit` is for. Why would `Vec<Option<T>>`
  work too, and what does it cost?
- What happens to `tail - head` when `tail` overflows `usize::MAX`? (Use `wrapping_sub` /
  `wrapping_add` in Rust — debug builds panic on overflow, C unsigned arithmetic just wraps.)
- Bonus: make the Rust version a single-producer/single-consumer queue shared between two
  threads using `AtomicUsize` for head and tail. Which memory orderings do you need?
