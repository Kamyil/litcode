# 403 · Quiz: Memory in C vs Rust

> **Difficulty:** Medium · **Topics:** stack/heap, alignment & padding, UB, ownership, borrowing, Drop, unsafe · **Mode:** `lc solution 403`

Do the sys track first (or alongside) — many answers are things you hit there.

**1.** What's `sizeof` this struct on x86-64 / arm64, and why? How do you make it smaller?
```c
struct S { char a; int64_t b; char c; };
```

**2.** Which of these are **undefined behaviour** in C? (a) signed integer overflow, (b) unsigned
overflow, (c) reading an uninitialized local, (d) `memcpy` with overlapping buffers, (e) `free(NULL)`,
(f) comparing pointers into two different arrays with `<`.

**3.** What's wrong here, and what would ASan report?
```c
int *make(void) { int x = 42; return &x; }
char *s = malloc(5); strcpy(s, "hello");
```

**4.** Rust: why does this not compile, and what's the C equivalent bug it prevents?
```rust
let mut v = vec![1, 2, 3];
let first = &v[0];
v.push(4);
println!("{first}");
```

**5.** What does "move" mean in Rust? After `let b = a;` with `a: String`, what happens if you use `a`?
What's different with `a: i32`, and why (which trait)?

**6.** Who frees memory in Rust, and when exactly? Name a situation where Rust still leaks memory safely.

**7.** `Box<T>`, `Rc<T>`, `Arc<T>`, `RefCell<T>`, `Cell<T>`: one sentence each.

**8.** Why can't you easily build a doubly linked list with `Box` in safe Rust? List the three
workarounds (you met them in sys/106).

**9.** What does `unsafe` allow you to do that safe Rust doesn't? Does it turn off the borrow checker?

**10.** In your `MyVec` (sys/101) `pop` used `ptr::read`. Why not just return `*ptr`? What would go wrong
if `Drop` for `MyVec` dropped `cap` elements instead of `len`?

**11.** What's `MaybeUninit<T>` for? Why is `let x: T = std::mem::zeroed()` UB for some `T` (e.g. `&u8`, `bool`, `NonNull`)?

**12.** What's the "niche optimization"? Why is `size_of::<Option<Box<T>>>() == size_of::<Box<T>>()`?

**13.** C `malloc` returns memory aligned for any type. What alignment does Rust's `alloc` give you,
and why do you pass a `Layout`?

**14.** What's a memory leak vs a use-after-free vs a double free, and which of them can occur in
**safe** Rust?

**15.** Postgres is C. How does it avoid leaks without a borrow checker? (Hint: sys/102.)
