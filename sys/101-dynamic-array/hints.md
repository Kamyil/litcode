C: element `i` lives at `(char *)a->data + i * a->elem_size`. You can't do arithmetic on `void *`
in standard C — cast to `char *` (or `unsigned char *`).
---
C growth: compute `new_cap`, call `realloc` into a **temporary** pointer. If it returns NULL the
old buffer is still valid — don't overwrite `a->data` until you know it succeeded.
---
C insert: grow first, then `memmove(slot(i+1), slot(i), (len - i) * elem_size)`. `memcpy` with
overlapping ranges is undefined behaviour — that's what memmove is for.
---
Rust: `Layout::array::<T>(cap).unwrap()`. Grow with `alloc` the first time and `realloc` after;
`handle_alloc_error(layout)` if you get null.
---
Rust push: `ptr.as_ptr().add(len).write(value)`. pop: `len -= 1; Some(ptr.as_ptr().add(len).read())`.
---
Rust Drop: drop the live elements with `ptr::drop_in_place(slice_from_raw_parts_mut(ptr, len))`,
then `dealloc` if `cap > 0`.
