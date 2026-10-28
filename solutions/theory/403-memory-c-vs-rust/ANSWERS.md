# 403 · Answers

**1.** 24 bytes: `a` at 0, 7 bytes padding, `b` at 8 (8-byte alignment), `c` at 16, plus 7 bytes tail
padding so arrays of S keep `b` aligned. Reorder largest-first (`int64_t b; char a; char c;`) for 16 bytes.
Rust reorders fields automatically unless you write `#[repr(C)]`. Postgres orders table columns
the same way to save space (fixed-width 8-byte columns first).

**2.** UB: (a) signed overflow, (c) reading an uninitialized local (indeterminate value), (d) overlapping
`memcpy`, (f) relational comparison of unrelated pointers. Defined: (b) unsigned wraps modulo 2ⁿ,
(e) `free(NULL)` is a no-op.

**3.** `make` returns the address of a dead stack variable (dangling pointer, ASan: stack-use-after-return
when that detection is enabled). `strcpy` writes 6 bytes ("hello" + NUL) into a 5-byte buffer: heap-buffer-overflow,
1 byte past the end.

**4.** `first` borrows `v` immutably, and `push` needs a mutable borrow while `first` is still used. Push may
reallocate, which would leave `first` dangling. In C: keeping a pointer from `da_get()` across `da_push()`,
a use-after-free after `realloc` moves the buffer.

**5.** Ownership transfers: `a` becomes unusable (compile error "borrow of moved value"), and only `b`
will drop the string. Bitwise copy, no deep copy. `i32` implements `Copy`, so the assignment copies and both
remain usable. Types that own resources (`String`, `Vec`, `Box`) can't be `Copy`.

**6.** The owner drops the value when it goes out of scope (deterministically, in reverse declaration order),
running `Drop` and then freeing. No GC. Safe leaks: `Rc` reference cycles, `Box::leak`, `mem::forget`,
or a long-lived collection that keeps growing. Leaking is "safe" in Rust's definition (no UB).

**7.** `Box<T>`: single owner of a heap allocation. `Rc<T>`: shared ownership via reference counting,
single-threaded. `Arc<T>`: atomic refcount, thread-safe sharing. `RefCell<T>`: borrow checking moved to
runtime (panics on conflicting borrows), single-threaded. `Cell<T>`: interior mutability by copying values in
and out, no references handed out.

**8.** Every node would need two owners (the previous node's `next` and the next node's `prev`), but `Box`
means exactly one owner. Workarounds: `Rc<RefCell<Node>>` + `Weak` for back pointers; raw pointers in
`unsafe`; indices into a `Vec` arena.

**9.** Five extra powers: dereference raw pointers, call `unsafe` functions (incl. FFI), access or modify
`static mut`, implement `unsafe` traits (`Send`/`Sync`), and access union fields. The borrow checker still
runs on references inside `unsafe` blocks. You take responsibility for invariants the compiler can't
check (validity, aliasing, initialization).

**10.** `*ptr` on a non-`Copy` T would be a move out of a raw pointer, which isn't allowed. `ptr::read`
does a bitwise copy and transfers ownership to the caller, and decrementing `len` first means the vector
forgets that slot so the value isn't dropped twice. Dropping `cap` elements would run destructors on
uninitialized memory (UB, typically a crash or freeing garbage pointers).

**11.** Memory that may not yet hold a valid `T`, so you can allocate first and initialize later without
UB (ring buffers, arrays built element by element). All-zero bits aren't a valid value for references or
`NonNull` (null) or for many enums, and `bool` must be 0 or 1. Merely *creating* an invalid value is UB in Rust.

**12.** If a type has invalid bit patterns (a "niche": e.g. `Box` can never be null), the compiler stores the
enum discriminant in that niche: `None` is represented as the null pointer. So `Option<Box<T>>`,
`Option<&T>` and `Option<NonZeroU32>` cost nothing extra.

**13.** Exactly the alignment requested in the `Layout` (size + align). Rust's allocator API needs both
for `alloc` *and* `dealloc` (allocators may use the size to find the bucket). Over-aligned types
(`#[repr(align(64))]`) get what they need.

**14.** Leak: memory never freed. Use-after-free: accessing memory after it was freed. Double free: freeing
the same allocation twice (corrupts the allocator). Safe Rust prevents the last two at compile time.
Leaks are possible (Q6).

**15.** Memory contexts (arena allocators): `palloc` allocates in the current context (per query, per tuple,
per transaction...). The whole context is reset or deleted at the end of the operation, so forgetting
`pfree` only wastes memory until then. On error (`ereport(ERROR)` uses longjmp), contexts are cleaned up
too. It's region-based memory management, similar in spirit to an arena + RAII.
