# 102 · Arena (Bump) Allocator

> **Difficulty:** Medium · **Languages:** C + Rust · **Topics:** alignment, memory blocks, lifetimes, interior mutability

`malloc`/`free` per object is slow and leak-prone. An **arena** grabs big blocks and hands
out memory by bumping a pointer; you never free individual objects — you free (or reset) the
whole arena at once. Compilers, game engines, request handlers and... **Postgres** live on this:
every query runs in a tree of `MemoryContext`s (`palloc` = arena alloc). When the query ends,
the context is reset and everything allocated during it vanishes — no leaks possible.
(`src/backend/utils/mmgr/aset.c`, `src/backend/utils/mmgr/README` — read the README, it's great.)

## Part 1 — C (`c/arena.c`)

The block struct is in `arena.h` and uses a **flexible array member** (`unsigned char data[]`) so
header and payload come from one `malloc`.

| function | behaviour |
|---|---|
| `arena_init(a, block_size)` | no allocation yet |
| `arena_alloc(a, size, align)` | `align` is a power of two. Returns an aligned pointer inside the current block; if it doesn't fit, `malloc` a new block of `max(block_size, size + align)` bytes of payload and make it current. `NULL` on OOM |
| `arena_strdup(a, s)` | copy of `s` inside the arena |
| `arena_reset(a)` | free every block except the current one, whose `used` goes back to 0 |
| `arena_free(a)` | free everything |
| `arena_block_count(a)` | blocks currently held |

## Part 2 — Rust (`rust/src/lib.rs`)

```rust
let arena = Arena::new(4096);
let x: &mut u64 = arena.alloc(42u64);
let s: &str = arena.alloc_str("hello");
let ys: &mut [i32] = arena.alloc_slice_copy(&[1, 2, 3]);
arena.reset(); // needs &mut self: won't compile while x/s/ys are alive!
```

`alloc` takes `&self` but returns `&mut T` — that requires **interior mutability**
(`Cell`/`RefCell`/`UnsafeCell`), and the returned references borrow the arena so they can't
outlive it. Only `T: Copy` is accepted, so you never have to run destructors (that's what
the `bumpalo` crate does too). Allocate blocks with `std::alloc::alloc` and keep raw pointers
(not `Box<[u8]>` — handing out `&mut` into a Box you later borrow again is UB under Rust's
aliasing rules).

## Compare when done

- C: nothing stops you from using a pointer after `arena_reset` (use-after-free). Rust: the
  borrow checker rejects `arena.reset()` while any allocation is still borrowed. Try it and
  read the error.
- Why must `align` be a power of two? What is `(addr + align - 1) & ~(align - 1)`?
- What happens to alignment if you forget that `data[]` itself may only be 8- or 16-aligned?
  (Align the *address*, not the offset.)
- Postgres `palloc` has per-chunk headers and freelists (so `pfree` works); a pure bump arena
  doesn't. When is each the right choice?
