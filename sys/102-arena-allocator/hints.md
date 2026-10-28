Align the absolute address: `uintptr_t p = (uintptr_t)(b->data + b->used); p = (p + align - 1) & ~(uintptr_t)(align - 1);`
Then check `p + size <= (uintptr_t)(b->data + b->cap)`.
---
New block: `malloc(sizeof(ArenaBlock) + payload)`, link it in front: `blk->next = a->head; a->head = blk;`.
The current block is always `a->head`.
---
Reset: walk `a->head->next` freeing each block, set `a->head->next = NULL`, `a->head->used = 0`.
---
Rust: fields like `blocks: RefCell<Vec<(NonNull<u8>, Layout)>>` + `used: Cell<usize>`. Allocate with
`alloc(Layout::from_size_align(size, 16))`, free them all in `Drop`.
---
Rust alloc<T>: `let p = self.alloc_raw(Layout::new::<T>()).cast::<T>(); unsafe { p.as_ptr().write(value); &mut *p.as_ptr() }`.
The signature `fn alloc<T: Copy>(&self, value: T) -> &mut T` ties the returned lifetime to `&self`.
