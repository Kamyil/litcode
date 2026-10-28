C write: `n = min(n, space)`. Physical start `t = tail & mask`. First chunk = `min(n, cap - t)` bytes
to `buf + t`; the remainder (if any) goes to `buf + 0`. Then `tail += n`.
---
peek is read without moving `head`; implement read as peek + `head += got`.
---
Rust: `let buf = (0..cap).map(|_| MaybeUninit::uninit()).collect::<Box<[_]>>()`.
push: `self.buf[self.tail & mask].write(v)`. pop: `unsafe { self.buf[self.head & mask].assume_init_read() }`.
---
Rust Drop: `while self.pop().is_some() {}` is a perfectly fine implementation.
