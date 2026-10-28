# 101 · Dynamic Array (Vec from scratch)

> **Difficulty:** Easy · **Languages:** C + Rust · **Topics:** heap allocation, growth strategy, amortized O(1), ownership

Every language's growable array (`std::vector`, `Vec<T>`, JS `Array`, Go slices, Postgres's
`List` since PG13) is the same idea: a contiguous buffer with `len <= cap`; when it's full,
allocate a bigger one (usually 2x), copy, free the old one.

## Part 1 — C (`c/dynarray.c`)

A **generic** array storing elements of any size (`elem_size` bytes each), using `void *`
and `memcpy`. API in `c/dynarray.h`:

| function | behaviour |
|---|---|
| `da_init(a, elem_size, cap)` | empty array; allocate `cap` slots up front (`cap` may be 0 → no allocation) |
| `da_free(a)` | release the buffer; leave `a` in a zeroed state (safe to free twice) |
| `da_push(a, elem)` | append a copy of `elem`, growing **2x** (min 4) when full |
| `da_pop(a, out)` | remove the last element into `out` (if non-NULL). `false` if empty |
| `da_get(a, i)` | pointer to element `i`, `NULL` if out of bounds |
| `da_insert(a, i, elem)` | insert at `i` (`0 <= i <= len`), shifting the tail right (`memmove`!) |
| `da_remove(a, i, out)` | remove at `i`, shifting left |
| `da_reserve(a, n)` | ensure `cap >= n` |
| `da_shrink_to_fit(a)` | make `cap == len` (free entirely when `len == 0`) |

Every allocating function returns `false` on allocation failure **without corrupting the array**.

## Part 2 — Rust (`rust/src/lib.rs`)

`MyVec<T>` built on `std::alloc::{alloc, realloc, dealloc, Layout}` and `NonNull<T>` —
**no `Vec` inside**. This is `unsafe` Rust: you are doing exactly what the C version does,
but you must also uphold Rust's rules: elements must be dropped exactly once, `pop` moves
ownership out (`ptr::read`), `insert`/`remove` use `ptr::copy` (memmove).

Implement `new`, `with_capacity`, `push`, `pop`, `get`, `get_mut`, `insert`, `remove`, `len`,
`capacity`, `as_slice`, and `Drop`. Zero-sized types may panic (bonus: support them).

## Run

```
lc test 101 c       # make -C c test   (ASan + UBSan + leak tracker)
lc test 101 rust    # cargo test -p dynamic-array
lc watch 101 c
```

## Compare when done

- In C, `elem_size` is a runtime value; in Rust, `T` is known at compile time, so
  `MyVec<u8>` and `MyVec<String>` are separate monomorphized copies. Pros/cons?
- Where does C rely on *you* to not use a pointer from `da_get` after a push (realloc may
  move the buffer = use-after-free)? How does Rust's borrow checker make that a compile error?
  Try it: `let r = v.get(0); v.push(1); println!("{r:?}");`
- `realloc` vs alloc+copy+free: why can realloc be faster? (it may grow in place)
- Postgres: `src/include/nodes/pg_list.h` — since PG13 `List` is an array, not a linked list.
  Read the commit message idea: cache-friendliness beat O(1) insertion at the head.
