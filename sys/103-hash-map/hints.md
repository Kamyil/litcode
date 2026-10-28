Find routine used by everything: start at `h & mask`; remember the first tombstone you pass;
stop at an Empty slot (key absent) or a Full slot with an equal key (found).
---
Insert: if found, overwrite the value. If absent, insert at the first tombstone you passed (if any)
else at the Empty slot. Check the load factor *before* inserting.
---
Resize: allocate a fresh array of 2x, re-insert every Full entry (keep the same key pointers in C —
no strdup needed), free the old array. Tombstones disappear.
---
C iterator: just scan slots `i` from the current position to `cap`, returning Full ones.
---
Rust: `Vec<Slot<K, V>>` initialized with `(0..cap).map(|_| Slot::Empty).collect()`. On resize,
`std::mem::replace(&mut self.slots, new_slots)` then move entries out of the old Vec with `into_iter()`.
---
Rust hashing a key: `let mut h = FnvHasher::default(); key.hash(&mut h); h.finish()`.
Note `str::hash` writes the bytes plus a 0xff terminator — fine, just be consistent between K and Q
(`String` and `str` hash identically, that's what `Borrow` guarantees).
